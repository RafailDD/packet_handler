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
//   * Memory arrays (fifo_mem, packetTracker) are written from a reset-free
//     always block so synthesis can map them to distributed/block RAM.
//     A small `tracker_valid` register array carries the "first packet
//     observed" flag and IS reset, so o_packetLost is not asserted
//     spuriously after reset without paying the cost of resetting the data
//     array itself.
//   * Async-asserted, async-deasserted reset; the integrator is expected to
//     supply a synchronized reset.
//   * o_data is loaded on the DATA -> DONE transition so it is valid in the
//     same cycle that o_valid rises (AXI-stream handshake compliant).
//-----------------------------------------------------------------------------

module packet_handler (
    /* clocks and resets */
<<<<<<< feat/rtl-synthesis-cleanup
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
    //--------------------------------------------------------------------------
    localparam integer FIFO_DEPTH = 256;
    localparam integer FIFO_AW    = 8;          // log2(FIFO_DEPTH)

    reg  [32:0]            fifo_mem [0:FIFO_DEPTH-1];
    reg  [FIFO_AW-1:0]     fifo_wr_ptr;
    reg  [FIFO_AW-1:0]     fifo_rd_ptr;
    reg  [FIFO_AW:0]       fifo_count;

    wire fifo_full  = fifo_count[FIFO_AW];                     // count == 2^FIFO_AW
    wire fifo_empty = (fifo_count == {(FIFO_AW+1){1'b0}});
    wire fifo_valid = ~fifo_empty;

    wire fifo_wr_en;
    wire fifo_rd_en;

    assign fifo_wr_en = i_valid & ~fifo_full;
    assign o_ready    = ~fifo_full;

    wire [31:0] fifo_data = fifo_mem[fifo_rd_ptr][31:0];
    wire        fifo_last = fifo_mem[fifo_rd_ptr][32];

    // Pointer / occupancy update.
=======
    input              i_clk,
    input              i_rst_n,

    /* input signals */
    input [31:0]       i_data,
    input              i_valid,
    input              i_last,
    input              i_ready,

    /* output signals */
    output reg [295:0] o_data,
    output             o_ready,
    output reg         o_valid,
    output             o_packetLost
);

    /* FIFO instantiation */
    reg [32:0] fifo_mem [0:255];
    reg [7:0] fifo_wr_ptr;
    reg [7:0] fifo_rd_ptr;
    reg [8:0] fifo_count;

    wire fifo_full = (fifo_count == 9'd256);
    wire fifo_empty = (fifo_count == 9'd0);

    wire fifo_wr_en = i_valid && !fifo_full;
    assign o_ready = !fifo_full;

    wire fifo_valid = !fifo_empty;
    wire [31:0] fifo_data = fifo_mem[fifo_rd_ptr][31:0];
    wire fifo_last = fifo_mem[fifo_rd_ptr][32];

    wire fifo_rd_en;

    always @(posedge i_clk or negedge i_rst_n) begin
        if (!i_rst_n) begin
            fifo_wr_ptr <= 8'd0;
            fifo_rd_ptr <= 8'd0;
            fifo_count <= 9'd0;
        end else begin
            case ({fifo_wr_en, fifo_rd_en})
                2'b10: fifo_count <= fifo_count + 1;
                2'b01: fifo_count <= fifo_count - 1;
                default: fifo_count <= fifo_count;
            endcase

            if (fifo_wr_en) begin
                fifo_mem[fifo_wr_ptr] <= {i_last, i_data};
                fifo_wr_ptr <= fifo_wr_ptr + 1;
            end

            if (fifo_rd_en) begin
                fifo_rd_ptr <= fifo_rd_ptr + 1;
            end
        end
    end

    /* Input tracking logic for o_packetLost */
    reg [1:0] in_state;
    localparam IN_W1 = 2'd0;
    localparam IN_W2 = 2'd1;
    localparam IN_DATA = 2'd2;

    reg [15:0] in_streamId;
    reg in_packetLost;
    reg [31:0] packetTracker [31:0];
    integer i;

    assign o_packetLost = in_packetLost;

    always @(posedge i_clk or negedge i_rst_n) begin
        if (!i_rst_n) begin
            in_state <= IN_W1;
            in_streamId <= 16'd0;
            in_packetLost <= 1'b0;
            for (i = 0; i < 32; i = i + 1) begin
                packetTracker[i] <= 32'b0;
            end
        end else begin
            in_packetLost <= 1'b0;
            if (fifo_wr_en) begin
                case (in_state)
                    IN_W1: begin
                        in_streamId <= {i_data[7:0], i_data[15:8]};
                        in_state <= IN_W2;
                    end
                    IN_W2: begin
                        packetTracker[in_streamId-1] <= packetTracker[in_streamId-1] + 1;
                        if ((packetTracker[in_streamId-1] + 1) != {i_data[7:0], i_data[15:8], i_data[23:16], i_data[31:24]}) begin
                            in_packetLost <= 1'b1;
                        end
                        if (i_last) begin
                            in_state <= IN_W1;
                        end else begin
                            in_state <= IN_DATA;
                        end
                    end
                    IN_DATA: begin
                        if (i_last) begin
                            in_state <= IN_W1;
                        end
                    end
                    default: in_state <= IN_W1;
                endcase
            end
        end
    end

    /* Internal FSM */
    /* verilator lint_off UNUSEDSIGNAL */ reg [15:0] msgLength; /* verilator lint_on UNUSEDSIGNAL */
    /* verilator lint_off UNUSEDSIGNAL */ reg [15:0] streamId; /* verilator lint_on UNUSEDSIGNAL */
    /* verilator lint_off UNUSEDSIGNAL */ reg [31:0] seqNumber; /* verilator lint_on UNUSEDSIGNAL */
    reg [295:0] shiftReg;
    reg [3:0]   state;
    reg [3:0]   next_state;

    localparam IDLE     = 4'b0001;
    localparam HEADER   = 4'b0010;
    localparam DATA     = 4'b0100;
    localparam DONE     = 4'b1000;

    assign fifo_rd_en = ((state == IDLE) && fifo_valid) ||
                        ((state == HEADER) && fifo_valid) ||
                        ((state == DATA) && fifo_valid);

>>>>>>> jules-experimenting
    always @(posedge i_clk or negedge i_rst_n) begin
        if (!i_rst_n) begin
            fifo_wr_ptr <= {FIFO_AW{1'b0}};
            fifo_rd_ptr <= {FIFO_AW{1'b0}};
            fifo_count  <= {(FIFO_AW+1){1'b0}};
        end else begin
            case ({fifo_wr_en, fifo_rd_en})
                2'b10:   fifo_count <= fifo_count + 1'b1;
                2'b01:   fifo_count <= fifo_count - 1'b1;
                default: fifo_count <= fifo_count; // 2'b00 / 2'b11 -> no change
            endcase

            if (fifo_wr_en) fifo_wr_ptr <= fifo_wr_ptr + 1'b1;
            if (fifo_rd_en) fifo_rd_ptr <= fifo_rd_ptr + 1'b1;
        end
    end

<<<<<<< feat/rtl-synthesis-cleanup
    // Memory write - reset-free for RAM inference.
    always @(posedge i_clk) begin
        if (fifo_wr_en) begin
            fifo_mem[fifo_wr_ptr] <= {i_last, i_data};
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
    reg  [15:0]               in_streamId;
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

    reg [295:0] shiftReg;

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
=======
    always @(*) begin
        case (state)
            IDLE: begin
                if (fifo_valid) begin
                    next_state = HEADER;
                end else begin
                    next_state = IDLE;
                end
            end
            HEADER: begin
                if (fifo_valid) begin
                    next_state = DATA;
                end else begin
                    next_state = HEADER;
                end
            end
            DATA: begin
                if (fifo_valid && fifo_last) begin
                    next_state = DONE;
                end else begin
                    next_state = DATA;
                end
            end
            DONE: begin
                if (i_ready) begin
                    next_state = IDLE;
                end else begin
                    next_state = DONE;
                end
            end
            default: begin
                next_state = IDLE;
            end
        endcase
    end

    always @(posedge i_clk or negedge i_rst_n) begin
        if (!i_rst_n) begin
            o_valid <= 1'b0;
        end else begin
            case (state)
                IDLE: begin
                    o_valid <= 1'b0;
                end
                HEADER: begin
                    o_valid <= 1'b0;
                end
                DATA: begin
                    if (fifo_valid && fifo_last) begin
                        o_valid <= 1'b1;
                    end else begin
                        o_valid <= 1'b0;
                    end
                end
                DONE: begin
                    if (i_ready) begin
                        o_valid <= 1'b0;
                    end else begin
                        o_valid <= 1'b1;
                    end
                end
                default: begin
                    o_valid <= 1'b0;
                end
            endcase
        end
    end

    always @(posedge i_clk or negedge i_rst_n) begin
        if (!i_rst_n) begin
            o_data <= 296'b0;
>>>>>>> jules-experimenting
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
<<<<<<< feat/rtl-synthesis-cleanup
                        streamId  <= {fifo_data[7:0],   fifo_data[15:8]};
=======
                        streamId <= {fifo_data[7:0], fifo_data[15:8]};
                    end else begin
                        msgLength <= 16'b0;
                        streamId <= 16'b0;
>>>>>>> jules-experimenting
                    end
                end

                HEADER: begin
<<<<<<< feat/rtl-synthesis-cleanup
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

=======
                    if (fifo_valid) begin
                        seqNumber <= {fifo_data[7:0], fifo_data[15:8], fifo_data[23:16], fifo_data[31:24]};
                    end
                end
                DATA: begin
                    if (fifo_valid) begin
                        shiftReg <= {shiftReg[263:0], fifo_data};
                    end
                end
                DONE: begin
                    if (i_ready) begin
                        o_data <= shiftReg;
                    end else begin
                        o_data <= o_data;
                    end
                end
                default: begin
                    o_data <= 296'b0;
                    msgLength <= 16'b0;
                    streamId <= 16'b0;
                    seqNumber <= 32'b0;
                    shiftReg <= 296'b0;
                end
            endcase
        end
    end

>>>>>>> jules-experimenting
endmodule
