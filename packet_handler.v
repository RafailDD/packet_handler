module packet_handler(
    /* clocks and resets */
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

    /* verilator lint_off UNUSEDSIGNAL */
    reg [15:0] in_streamId;
    /* verilator lint_on UNUSEDSIGNAL */
    reg in_packetLost;
    integer i;

    assign o_packetLost = in_packetLost;

`ifdef SYNTHESIS
    // BRAM instance for Synthesis
    // Note: BRAM inference requires synchronous read and write
    reg [31:0] packetTracker [0:31];
    reg [31:0] packetTracker_read_data;

    // Read logic using synchronous read to properly infer BRAM.
    // Address is generated in IN_W1 when fifo_wr_en is high.
    wire [4:0] stream_idx_w1 = i_data[12:8] - 5'd1;
    wire [4:0] stream_idx_w2 = in_streamId[4:0] - 5'd1;

    always @(posedge i_clk) begin
        // Read during IN_W1 so data is available in IN_W2
        if (fifo_wr_en && in_state == IN_W1) begin
            packetTracker_read_data <= packetTracker[stream_idx_w1];
        end

        // Write during IN_W2
        if (fifo_wr_en && in_state == IN_W2) begin
            packetTracker[stream_idx_w2] <= packetTracker_read_data + 1;
        end
    end

    always @(posedge i_clk or negedge i_rst_n) begin
        if (!i_rst_n) begin
            in_state <= IN_W1;
            in_streamId <= 16'd0;
            in_packetLost <= 1'b0;
        end else begin
            in_packetLost <= 1'b0;
            if (fifo_wr_en) begin
                case (in_state)
                    IN_W1: begin
                        in_streamId <= {i_data[7:0], i_data[15:8]};
                        in_state <= IN_W2;
                    end
                    IN_W2: begin
                        if ((packetTracker_read_data + 1) != {i_data[7:0], i_data[15:8], i_data[23:16], i_data[31:24]}) begin
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
`else
    // Original Logic for simulation
    reg [31:0] packetTracker [31:0];

    wire [4:0] stream_idx_w2 = in_streamId[4:0] - 5'd1;

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
                        packetTracker[stream_idx_w2] <= packetTracker[stream_idx_w2] + 1;
                        if ((packetTracker[stream_idx_w2] + 1) != {i_data[7:0], i_data[15:8], i_data[23:16], i_data[31:24]}) begin
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
`endif

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

    always @(posedge i_clk or negedge i_rst_n) begin
        if (!i_rst_n) begin
            state <= IDLE;
        end else begin
            state <= next_state;
        end
    end

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
            msgLength <= 16'b0;
            streamId <= 16'b0;
            seqNumber <= 32'b0;
            shiftReg <= 296'b0;
        end else begin
            case (state)
                IDLE: begin
                    shiftReg <= 296'b0;
                    if (fifo_valid) begin
                        msgLength <= {fifo_data[23:16], fifo_data[31:24]};
                        streamId <= {fifo_data[7:0], fifo_data[15:8]};
                    end else begin
                        msgLength <= 16'b0;
                        streamId <= 16'b0;
                    end
                end
                HEADER: begin
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

endmodule
