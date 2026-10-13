//-----------------------------------------------------------------------------
// packet_handler.v
//
// 32-bit serial-to-parallel packet handler with a 256-deep input FIFO,
// header parsing (msgLength / streamId / seqNumber) and per-stream
// sequence-number continuity checking with a 1-cycle o_packetLost pulse.
//
// Synthesis notes:
//   * One-hot FSM encoding for high-frequency operation. A Vivado-style
//     attribute is included as a hint; other tools ignore unknown attributes.
//   * The FIFO storage is a synchronous-read, 1-write/1-read memory
//     (ph_fifo_mem, bottom of this file) followed by a 2-entry prefetch
//     buffer, so it can be a flop array, an FPGA block RAM or an SRAM
//     macro. By default ph_fifo_mem is a behavioral model; define
//     PH_SKY130_SRAM to instantiate the Sky130 OpenRAM macro instead (see
//     docs/memory_options.md). packetTracker is written from a reset-free
//     always block so synthesis can map it to distributed RAM.
//     A small `tracker_valid` register array carries the "first packet
//     observed" flag and IS reset, so o_packetLost is not asserted
//     spuriously after reset without paying the cost of resetting the data
//     array itself.
//   * Async-asserted, async-deasserted reset; the integrator is expected to
//     supply a synchronized reset.
//   * o_data is loaded on the DATA -> DONE transition so it is valid in the
//     same cycle that o_valid rises (AXI-stream handshake compliant).
//-----------------------------------------------------------------------------

/* verilator lint_off TIMESCALEMOD */
module packet_handler (
    /* clocks and resets */
    input  wire         i_clk,
    input  wire         i_rst_n,

    /* transmitter interface */
    input  wire [31:0]  i_data,
    input  wire         i_valid,
    input  wire         i_last,
    output wire         o_ready,

    /* receiver interface */
    output reg  [295:0] o_data,
    output reg          o_valid,
    input  wire         i_ready,

    /* status */
    output wire         o_packetLost
);

    //--------------------------------------------------------------------------
    // 256-deep input FIFO storing { i_last, i_data }
    //
    // Storage is a synchronous-read memory: a read issued in cycle N returns
    // its word in cycle N+1. A 2-entry register buffer (fbuf0 = head, fbuf1)
    // sits behind it and presents a first-word-fall-through interface
    // (fifo_valid / fifo_data / fifo_last, popped by fifo_rd_en) to the
    // output FSM. Reads are issued whenever the buffer will have room for the
    // returning word, which sustains one word per cycle.
    //
    // fifo_count counts every word written and not yet popped (memory +
    // in-flight read + buffer), so o_ready back-pressures after 256 words
    // exactly as a plain 256-deep FIFO would. Compared with a combinational
    // read FIFO, a word reaches the output FSM two cycles later.
    //--------------------------------------------------------------------------
    localparam integer FIFO_AW    = 8;          // depth = 2^FIFO_AW = 256

    reg  [FIFO_AW-1:0]     fifo_wr_ptr;
    reg  [FIFO_AW-1:0]     fifo_rd_ptr;         // next memory address to read
    reg  [FIFO_AW:0]       fifo_count;

    wire fifo_full  = fifo_count[FIFO_AW];                     // count == 2^FIFO_AW

    wire fifo_wr_en;
    wire fifo_rd_en;                            // pop from the prefetch buffer

    assign fifo_wr_en = i_valid & ~fifo_full;
    assign o_ready    = ~fifo_full;

    // Prefetch buffer state
    reg  [32:0]            fbuf0;
    reg  [32:0]            fbuf1;
    reg  [1:0]             fbuf_cnt;            // 0..2 valid entries
    reg                    mem_rd_pending;      // read issued last cycle
    wire [32:0]            mem_rdata;

    wire fifo_valid = (fbuf_cnt != 2'd0);
    wire fifo_pop   = fifo_rd_en & fifo_valid;

    wire [31:0] fifo_data = fbuf0[31:0];
    wire        fifo_last = fbuf0[32];

    // Words still in memory = total - buffered - in flight.
    wire [FIFO_AW:0] mem_words = fifo_count - {{(FIFO_AW-1){1'b0}}, fbuf_cnt}
                                            - {{FIFO_AW{1'b0}}, mem_rd_pending};

    // Issue a read when memory holds a word and the buffer will have space
    // for it next cycle: fbuf_cnt + mem_rd_pending - fifo_pop < 2.
    wire mem_rd_en = (mem_words != {(FIFO_AW+1){1'b0}}) &&
                     (({1'b0, fbuf_cnt} + {2'b00, mem_rd_pending}) <=
                      {2'b00, fifo_pop} + 3'd1);

    ph_fifo_mem #(
        .WIDTH (33),
        .AW    (FIFO_AW)
    ) u_fifo_mem (
        .i_clk   (i_clk),
        .i_we    (fifo_wr_en),
        .i_waddr (fifo_wr_ptr),
        .i_wdata ({i_last, i_data}),
        .i_re    (mem_rd_en),
        .i_raddr (fifo_rd_ptr),
        .o_rdata (mem_rdata)
    );

    // Pointer / occupancy update.
    always @(posedge i_clk or negedge i_rst_n) begin
        if (!i_rst_n) begin
            fifo_wr_ptr    <= {FIFO_AW{1'b0}};
            fifo_rd_ptr    <= {FIFO_AW{1'b0}};
            fifo_count     <= {(FIFO_AW+1){1'b0}};
            mem_rd_pending <= 1'b0;
        end else begin
            case ({fifo_wr_en, fifo_pop})
                2'b10:   fifo_count <= fifo_count + 1'b1;
                2'b01:   fifo_count <= fifo_count - 1'b1;
                default: fifo_count <= fifo_count; // 2'b00 / 2'b11 -> no change
            endcase

            if (fifo_wr_en) fifo_wr_ptr <= fifo_wr_ptr + 1'b1;
            if (mem_rd_en)  fifo_rd_ptr <= fifo_rd_ptr + 1'b1;
            mem_rd_pending <= mem_rd_en;
        end
    end

    // Prefetch buffer: the returning memory word (push) lands in the first
    // free slot after an optional pop of the head.
    always @(posedge i_clk or negedge i_rst_n) begin
        if (!i_rst_n) begin
            fbuf0    <= 33'd0;
            fbuf1    <= 33'd0;
            fbuf_cnt <= 2'd0;
        end else begin
            case ({mem_rd_pending, fifo_pop})
                2'b10: begin                            // push only
                    if (fbuf_cnt == 2'd0) fbuf0 <= mem_rdata;
                    else                  fbuf1 <= mem_rdata;
                    fbuf_cnt <= fbuf_cnt + 2'd1;
                end
                2'b01: begin                            // pop only
                    fbuf0    <= fbuf1;
                    fbuf_cnt <= fbuf_cnt - 2'd1;
                end
                2'b11: begin                            // push and pop
                    if (fbuf_cnt == 2'd1) fbuf0 <= mem_rdata;
                    else begin
                        fbuf0 <= fbuf1;
                        fbuf1 <= mem_rdata;
                    end
                end
                default: ;
            endcase
        end
    end

    //--------------------------------------------------------------------------
    // Per-stream sequence-number tracker / o_packetLost pulse generation
    //
    //   packetTracker[s]  - last observed seqNumber for streamId == s+1
    //   tracker_valid[s]  - high once the first packet of stream s+1 has been
    //                       seen, so the very first packet for any stream
    //                       does not raise a false o_packetLost.
    //--------------------------------------------------------------------------
    localparam integer NUM_STREAMS  = 32;
    localparam integer STREAM_IDX_W = 5;        // log2(NUM_STREAMS)

    localparam [1:0] IN_W1   = 2'd0;
    localparam [1:0] IN_W2   = 2'd1;
    localparam [1:0] IN_DATA = 2'd2;

    reg  [1:0]                in_state;
    /* verilator lint_off UNUSEDSIGNAL */
    reg  [15:0]               in_streamId;
    /* verilator lint_on  UNUSEDSIGNAL */
    reg                       in_packetLost;

    reg  [31:0]               packetTracker [0:NUM_STREAMS-1];
    reg  [NUM_STREAMS-1:0]    tracker_valid;

    // Bound the index to the array width to avoid out-of-range access.
    wire [STREAM_IDX_W-1:0]   in_stream_idx   = in_streamId[STREAM_IDX_W-1:0] - 1'b1;
    wire [31:0]               in_seqNumber_be = {i_data[7:0],   i_data[15:8],
                                                 i_data[23:16], i_data[31:24]};

    assign o_packetLost = in_packetLost;

    // Tracker memory write - no reset, RAM inference friendly.
    always @(posedge i_clk) begin
        if (fifo_wr_en && (in_state == IN_W2)) begin
            packetTracker[in_stream_idx] <= in_seqNumber_be;
        end
    end

    // Tracker control / packetLost pulse.
    always @(posedge i_clk or negedge i_rst_n) begin
        if (!i_rst_n) begin
            in_state      <= IN_W1;
            in_streamId   <= 16'd0;
            in_packetLost <= 1'b0;
            tracker_valid <= {NUM_STREAMS{1'b0}};
        end else begin
            in_packetLost <= 1'b0;          // default: deassert (1-cycle pulse)
            if (fifo_wr_en) begin
                case (in_state)
                    IN_W1: begin
                        in_streamId <= {i_data[7:0], i_data[15:8]};
                        in_state    <= IN_W2;
                    end
                    IN_W2: begin
                        tracker_valid[in_stream_idx] <= 1'b1;
                        if (tracker_valid[in_stream_idx] &&
                            (packetTracker[in_stream_idx] + 32'd1) != in_seqNumber_be) begin
                            in_packetLost <= 1'b1;
                        end
                        in_state <= i_last ? IN_W1 : IN_DATA;
                    end
                    IN_DATA: begin
                        if (i_last) in_state <= IN_W1;
                    end
                    default: in_state <= IN_W1;
                endcase
            end
        end
    end

    //--------------------------------------------------------------------------
    // Output FSM (one-hot, 4 states)
    //--------------------------------------------------------------------------
    localparam [3:0] IDLE   = 4'b0001;
    localparam [3:0] HEADER = 4'b0010;
    localparam [3:0] DATA   = 4'b0100;
    localparam [3:0] DONE   = 4'b1000;

    (* fsm_encoding = "one_hot" *)
    reg  [3:0] state;
    reg  [3:0] next_state;

    /* verilator lint_off UNUSEDSIGNAL */
    reg [15:0]  msgLength;
    reg [15:0]  streamId;
    reg [31:0]  seqNumber;
    /* verilator lint_on  UNUSEDSIGNAL */

    /* verilator lint_off UNUSEDSIGNAL */
    reg [295:0] shiftReg;
    /* verilator lint_on  UNUSEDSIGNAL */

    // FIFO is consumed in every state except DONE.
    assign fifo_rd_en = ((state == IDLE)   |
                         (state == HEADER) |
                         (state == DATA)) & fifo_valid;

    // State register
    always @(posedge i_clk or negedge i_rst_n) begin
        if (!i_rst_n) state <= IDLE;
        else          state <= next_state;
    end

    // Next-state logic
    always @(*) begin
        next_state = state;     // default: hold; covers any non-onehot state
        case (state)
            IDLE   : if (fifo_valid)              next_state = HEADER;
            HEADER : if (fifo_valid)              next_state = DATA;
            DATA   : if (fifo_valid && fifo_last) next_state = DONE;
            DONE   : if (i_ready)                 next_state = IDLE;
            default:                              next_state = IDLE;
        endcase
    end

    // Output and datapath registers (single sequential block).
    always @(posedge i_clk or negedge i_rst_n) begin
        if (!i_rst_n) begin
            o_valid   <= 1'b0;
            o_data    <= 296'b0;
            msgLength <= 16'b0;
            streamId  <= 16'b0;
            seqNumber <= 32'b0;
            shiftReg  <= 296'b0;
        end else begin
            case (state)
                IDLE: begin
                    o_valid  <= 1'b0;
                    shiftReg <= 296'b0;
                    if (fifo_valid) begin
                        msgLength <= {fifo_data[23:16], fifo_data[31:24]};
                        streamId  <= {fifo_data[7:0],   fifo_data[15:8]};
                    end
                end

                HEADER: begin
                    o_valid <= 1'b0;
                    if (fifo_valid) begin
                        seqNumber <= {fifo_data[7:0],   fifo_data[15:8],
                                      fifo_data[23:16], fifo_data[31:24]};
                    end
                end

                DATA: begin
                    if (fifo_valid) begin
                        shiftReg <= {shiftReg[263:0], fifo_data};
                    end
                    if (fifo_valid && fifo_last) begin
                        // Capture the full payload at completion of the data
                        // phase so o_data is valid in the same cycle that
                        // o_valid rises (AXI-stream compliant handshake).
                        o_data  <= {shiftReg[263:0], fifo_data};
                        o_valid <= 1'b1;
                    end else begin
                        o_valid <= 1'b0;
                    end
                end

                DONE: begin
                    if (i_ready) o_valid <= 1'b0;
                end

                default: begin
                    o_valid   <= 1'b0;
                    o_data    <= 296'b0;
                    msgLength <= 16'b0;
                    streamId  <= 16'b0;
                    seqNumber <= 32'b0;
                    shiftReg  <= 296'b0;
                end
            endcase
        end
    end

endmodule


//-----------------------------------------------------------------------------
// ph_fifo_mem
//
// 1-write / 1-read memory with a synchronous read: when i_re is high, o_rdata
// shows mem[i_raddr] from the next cycle on. A read of the address written in
// the same cycle returns the old data (the FIFO never does this).
//
//   default         Behavioral model. Simulators use it as is; synthesis turns
//                   it into flip-flops (or block RAM on an FPGA).
//   PH_SKY130_SRAM  Sky130 OpenRAM macro sky130_sram_2kbyte_1rw1r_32x512_8
//                   (port 0 writes, port 1 reads, address MSB tied low) for
//                   bits [31:0]; the remaining WIDTH-32 bits stay in flops.
//                   Synthesis needs the macro Liberty file (black box);
//                   simulation needs macros/sky130_sram_model.v.
//-----------------------------------------------------------------------------
/* verilator lint_off DECLFILENAME */
module ph_fifo_mem #(
    parameter integer WIDTH = 33,
    parameter integer AW    = 8
) (
    input  wire             i_clk,
    input  wire             i_we,
    input  wire [AW-1:0]    i_waddr,
    input  wire [WIDTH-1:0] i_wdata,
    input  wire             i_re,
    input  wire [AW-1:0]    i_raddr,
    output wire [WIDTH-1:0] o_rdata
);

`ifdef PH_SKY130_SRAM
    localparam integer MACRO_AW = 9;            // 512 words
    localparam integer XW       = WIDTH - 32;   // bits kept in flops

    wire [31:0] sram_dout0_unused;
    /* verilator lint_off PINCONNECTEMPTY */
    sky130_sram_2kbyte_1rw1r_32x512_8 u_sram (
        // Port 0: write only
        .clk0   (i_clk),
        .csb0   (~i_we),
        .web0   (1'b0),
        .wmask0 (4'hF),
        .addr0  ({{(MACRO_AW-AW){1'b0}}, i_waddr}),
        .din0   (i_wdata[31:0]),
        .dout0  (sram_dout0_unused),
        // Port 1: read only
        .clk1   (i_clk),
        .csb1   (~i_re),
        .addr1  ({{(MACRO_AW-AW){1'b0}}, i_raddr}),
        .dout1  (o_rdata[31:0])
    );
    /* verilator lint_on PINCONNECTEMPTY */

    reg [XW-1:0] xmem [0:(1<<AW)-1];
    reg [XW-1:0] xrdata;

    always @(posedge i_clk) begin
        if (i_we) xmem[i_waddr] <= i_wdata[WIDTH-1:32];
        if (i_re) xrdata        <= xmem[i_raddr];
    end

    assign o_rdata[WIDTH-1:32] = xrdata;
`else
    reg [WIDTH-1:0] mem [0:(1<<AW)-1];
    reg [WIDTH-1:0] rdata;

    always @(posedge i_clk) begin
        if (i_we) mem[i_waddr] <= i_wdata;
        if (i_re) rdata        <= mem[i_raddr];
    end

    assign o_rdata = rdata;
`endif

endmodule
/* verilator lint_on DECLFILENAME */
