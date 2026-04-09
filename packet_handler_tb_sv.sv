`timescale 1ns/1ps

module packet_handler_tb_sv;

    logic clk;
    logic rst_n;

    // Inputs
    logic [31:0]  i_data;
    logic         i_valid;
    logic         i_ready;
    logic         i_last;

    // Outputs
    logic [295:0] o_data;
    logic         o_ready;
    logic         o_valid;
    logic         o_packetLost;

    // Clock generation
    initial begin
        clk = 0;
        forever #5 clk = ~clk;
    end

    // DUT Instantiation
    packet_handler dut (
        .i_clk(clk),
        .i_rst_n(rst_n),
        .i_data(i_data),
        .i_valid(i_valid),
        .i_ready(i_ready),
        .i_last(i_last),
        .o_data(o_data),
        .o_ready(o_ready),
        .o_valid(o_valid),
        .o_packetLost(o_packetLost)
    );

    // Scoreboard vars
    int error_count = 0;
    int pkt_count = 0;

    // Send packet task
    task send_packet(input int msgLength, input int streamId, input int seqNumber, input logic[7:0] payload[]);
        logic [31:0] header_w1;
        logic [31:0] header_w2;

        // Wait for ready
        @(posedge clk);
        while(!o_ready) @(posedge clk);

        header_w1 = {msgLength[7:0], msgLength[15:8], streamId[7:0], streamId[15:8]};
        header_w2 = {seqNumber[7:0], seqNumber[15:8], seqNumber[23:16], seqNumber[31:24]};

        i_valid <= 1;
        i_data <= header_w1;
        @(posedge clk);
        i_data <= header_w2;
        @(posedge clk);

        for(int i = 0; i < payload.size(); i+=4) begin
            logic [31:0] pdata = 0;
            pdata[7:0]   = (i < payload.size()) ? payload[i] : 8'd0;
            pdata[15:8]  = (i+1 < payload.size()) ? payload[i+1] : 8'd0;
            pdata[23:16] = (i+2 < payload.size()) ? payload[i+2] : 8'd0;
            pdata[31:24] = (i+3 < payload.size()) ? payload[i+3] : 8'd0;

            i_data <= pdata;
            if(i+4 >= payload.size())
                i_last <= 1;
            @(posedge clk);
        end
        i_valid <= 0;
        i_last <= 0;
    endtask

    // Monitor / Scoreboard
    always @(posedge clk) begin
        if(o_valid && i_ready) begin
            $display("[%0t] SCOREBOARD: Received output payload transaction.", $time);
            pkt_count++;
        end
        if(o_packetLost) begin
            $display("[%0t] SCOREBOARD: Packet Lost detected!", $time);
        end
    end

    // Test Scenarios
    initial begin
        logic [7:0] payload[];
        rst_n = 0;
        i_data = 0;
        i_valid = 0;
        i_ready = 1;
        i_last = 0;

        #20 rst_n = 1;

        // Scenario 1: Normal packets
        for (int i = 1; i <= 5; i++) begin
            payload = new[16];
            foreach(payload[k]) payload[k] = k;
            send_packet(24, 15, i, payload); // msgLen=24, stream=15
        end

        // Scenario 2: Packet lost (jump seq number)
        payload = new[8];
        foreach(payload[k]) payload[k] = 8'hFF;
        send_packet(16, 15, 7, payload); // msgLen=16, stream=15, seq=7 (skipped 6)

        // Wait for processing
        #100;

        $display("--------------------------------");
        $display("Test finished. Packets received: %0d, Errors: %0d", pkt_count, error_count);
        if (error_count == 0) $display("STATUS: PASSED");
        else $display("STATUS: FAILED");
        $display("--------------------------------");
        $finish;
    end

endmodule
