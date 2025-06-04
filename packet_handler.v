module packet_handler(
    /* clocks and resets */
    i_clk,
    i_rst_n,

    /* input signals */
    i_data,
    i_valid,
    i_ready,
    i_last,

    /* output signals */
    o_data,
    o_ready,
    o_valid,
    o_packetLost
);
    /* clocks and resets */
    input              i_clk;
    input              i_rst_n;
    /* input signals */
    input [31:0]       i_data;
    input              i_valid;
    input              i_ready;
    input              i_last;
    /* output signals */
    output reg [295:0] o_data;
    output reg         o_ready;
    output reg         o_valid;
    output             o_packetLost;

    /* internal signals */
    /* header format values */
    reg [15:0]  msgLength;
    reg [15:0]  streamId;
    reg [31:0]  seqNumber;
    /* using unpacked array to track seqNumber based on streamId
    using 32 bits to store seqNumber and 32 elements for each streamId */
    reg [31:0]  packetTracker [31:0];
    /* serial to paraller shift register */
    reg [295:0] shiftReg;
    /* state and next state registers for FSM */
    reg [3:0]   state;
    reg [3:0]   next_state;
    /* registers to help generate o_packLost pulse */
    reg         o_packetLostReg;
    reg         o_packetLostReg_d;

    /* one-hot encoding for FSM states*/
    localparam IDLE     = 4'b0001;
    localparam HEADER   = 4'b0010;
    localparam DATA     = 4'b0100;
    localparam DONE     = 4'b1000;

    /* FSM initialization - sequential block - state registers definition */
    always @(posedge i_clk or negedge i_rst_n) begin
        if (!i_rst_n) begin
            state <= IDLE;
        end else begin
            state <= next_state;
        end
    end

    /* FSM state logic - combinational block - next state logic */
    always @(*) begin
        case (state)
            IDLE: begin
                /* transmitter asserts i_valid, data transaction
                can be initiated since o_ready is high */
                if (i_valid) begin
                    next_state = HEADER;
                end else begin
                    next_state = IDLE;
                end
            end
            /* extra state to receive and store the next 4 bytes 
            of header */
            HEADER: begin
                /* unconditional transition to next state */
                next_state = DATA;
            end
            DATA: begin
                /* transmitter asserts i_last so its done sending data */
                if (i_last) begin
                    next_state = DONE;
                end else begin
                    next_state = DATA;
                end
            end
            DONE: begin
                /* receiver asserts i_ready, data can be sent */
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

    //FSM output logic - sequential block
    always @(posedge i_clk or negedge i_rst_n) begin
        if (!i_rst_n) begin
            o_ready <= 1'b1;
            o_valid <= 1'b0;
        end else begin
            case (state)
                IDLE: begin
                    /* FPGA is ready to receive data from transmitter */
                    o_ready <= 1'b1;
                    o_valid <= 1'b0;
                end
                HEADER: begin
                    /* FPGA is ready to receive data from transmitter */
                    o_ready <= 1'b1;
                    o_valid <= 1'b0;
                end
                DATA: begin
                    /* FPGA is ready to receive data from transmitter */
                    o_ready <= 1'b1;
                    /* transmitter has sent all data, FPGA is ready to
                    send data to receiver */
                    if (i_last) begin
                        o_valid <= 1'b1;
                    end else begin
                        o_valid <= 1'b0;
                    end
                end
                DONE: begin
                    /* receiver is ready to receive data, FPGA can go
                    back to being ready to receive data from transmitter*/
                    if (i_ready) begin
                        o_valid <= 1'b0;
                        o_ready <= 1'b1;
                    end else begin
                        o_valid <= 1'b1;
                        o_ready <= 1'b0;
                    end
                end
                default: begin
                    o_ready <= 1'b0;
                    o_valid <= 1'b0;
                end
            endcase
        end
    end

    /* serial to parallel data with shift register
    and logic to extract header information
    the indexes on i_data signal are based on the fact
    that bytes are stored in little endian for each field
     of the header*/
    always @(posedge i_clk or negedge i_rst_n) begin
        if (!i_rst_n) begin
            o_data <= 32'b0;
            msgLength <= 16'b0;
            streamId <= 16'b0;
            seqNumber <= 32'b0;
            shiftReg <= 296'b0;
        end else begin
            case (state)
                IDLE: begin
                    shiftReg <= 296'b0;
                    if (i_valid && o_ready) begin
                        /* extract first 4 bytes of header fields
                        stored in little endian */
                        msgLength <= {i_data[23:16], i_data[31:24]};
                        streamId <= {i_data[7:0], i_data[15:8]};
                    end else begin
                        msgLength <= 16'b0;
                        streamId <= 16'b0;
                    end
                end
                HEADER: begin
                    /* extract last 4 bytes of header field
                    sotred in little endian */
                    seqNumber <= {i_data[7:0], i_data[15:8], i_data[23:16], i_data[31:24]};
                end
                DATA: begin
                    /* shifting 32 bits of incoming data into register */
                    shiftReg <= {shiftReg[263:0], i_data};
                end
                DONE: begin
                    /* transimitter is done sending data
                    data can be outputed to receiver as long as
                    receiver is ready */
                    if (i_ready) begin
                        o_data <= shiftReg;
                    end else begin
                        o_data <= o_data;
                    end
                end
                default: begin
                    o_data <= 32'b0;
                    msgLength <= 16'b0;
                    streamId <= 16'b0;
                    seqNumber <= 32'b0;
                    shiftReg <= 296'b0;
                end
            endcase
        end
    end

    /* Lost packet detection logic */
    integer i;
    always @(posedge i_clk or negedge i_rst_n) begin
        if (!i_rst_n) begin
            /* initializing packetTracker to 0s */
            for (i = 0; i < 32; i = i + 1) begin
                packetTracker[i] <= 32'b0;
            end
        end else begin
            case (state)
                IDLE: begin
                    o_packetLostReg <= 1'b0;
                end
                HEADER: begin
                    /* updating internal counter for incoming streamId 
                    streamId range is 1-32 and index range is 0-31 */
                    o_packetLostReg <= 1'b0;
                    packetTracker[streamId-1] <= packetTracker[streamId-1]+1;
                end
                DATA: begin
                    /* comparing internal counter for incoming streamId
                    with incoming seqNumber and asserting o_packetLostReg
                    if it is not the same */
                    if (packetTracker[streamId-1] == seqNumber) begin
                        o_packetLostReg <= 1'b0;
                    end else begin
                        o_packetLostReg <= 1'b1;
                    end
                end
                DONE: begin
                    o_packetLostReg <= 1'b0;
                end
                default: begin
                    for (i = 0; i < 32; i = i + 1) begin
                        packetTracker[i] <= 32'b0;
                    end
                end
            endcase
        end
    end

    /* o_packetLost needs to be a pulse so an 
    edge detector is used to help generate it  */
    always @(posedge i_clk or negedge i_rst_n) begin
        if (!i_rst_n) begin
            o_packetLostReg_d <= 1'b0;
        end else begin
            o_packetLostReg_d <= o_packetLostReg;
        end
    end

    /* generation of o_packetLost pulse from regisered signals */
    assign o_packetLost = o_packetLostReg & ~o_packetLostReg_d;

endmodule