module packet_handler_tb;

    reg              clk;
    reg              rst_n;
    reg      [31:0]  dataIn;
    reg              validIn;
    reg              lastIn;
    wire             readyIn;
    wire     [295:0] dataOut;
    wire             validOut;
    reg              readyOut;
    wire             packetLost;

    packet_handler UTpacket_handler (
        /* clocks and resets */
        .i_clk         (clk         ),
        .i_rst_n       (rst_n       ),

        /* input interface */
        .i_data        (dataIn      ),
        .i_valid       (validIn     ),
        .i_last        (lastIn      ),
        .o_ready       (readyIn     ),

        /* output interface */
        .o_data        (dataOut     ),
        .o_valid       (validOut    ),
        .i_ready       (readyOut    ),
        .o_packetLost  (packetLost  )
    );

    initial begin
        clk        = 1'b0;
        rst_n      = 1'b0;
        dataIn     = 'bx;
        validIn    = 1'b0;
        readyOut   = 1'b0;
        lastIn     = 1'b0;
        $monitor("%t: dataIn=%h, validIn=%b, readyOut=%b, lastIn=%b, dataOut=%h, readyIn=%b, validOut=%b, packetLost=%b, state=%b, msgLength:%h, streamId:%h, seqNumber:%h, shiftReg:%h",
            $time, dataIn, validIn, readyOut, lastIn, dataOut, readyIn, validOut, packetLost, UTpacket_handler.state, UTpacket_handler.msgLength, UTpacket_handler.streamId, UTpacket_handler.seqNumber, UTpacket_handler.shiftReg);
    end

    always
    #5 clk = ~clk;

    initial begin
        #2.5 rst_n = 1'b0;
        #10  rst_n = 1'b1;

        //idle

        #10 dataIn='d0; validIn=1'b0; lastIn=1'b0; readyOut=1'b0;
        #10 dataIn='d0; validIn=1'b0; lastIn=1'b0; readyOut=1'b0;

        //sending data with valid seqNumber for streamId 15
        //sending msgLength (24) and streamId (15) in little endian
        #10 dataIn=32'h1800_0F00; validIn=1'b1; lastIn=1'b0; readyOut=1'b0;
        //sending seqNumber(1) in little endian
        #10 dataIn=32'h0100_0000; validIn=1'b1; lastIn=1'b0; readyOut=1'b0;
        //sending data 1512223444
        #10 dataIn=32'h5A22B2D4; validIn=1'b1; lastIn=1'b0; readyOut=1'b0;
        //sending data 1512223902
        #10 dataIn=32'h5A22B49E; validIn=1'b1; lastIn=1'b0; readyOut=1'b0;
        //sending data 1122223902
        #10 dataIn=32'h42E3C71E; validIn=1'b1; lastIn=1'b0; readyOut=1'b0;
        //sending data 1134123902
        #10 dataIn=32'h43995B7E; validIn=1'b1; lastIn=1'b1; readyOut=1'b0;
        //completing transaction
        #10 dataIn=32'h43995B7E; validIn=1'b0; lastIn=1'b0; readyOut=1'b0;
        //sending to receiver
        #10 dataIn=32'h43995B7E; validIn=1'b0; lastIn=1'b0; readyOut=1'b1;

        //back to idle
        #10 dataIn=32'h43995B7E; validIn=1'b0; lastIn=1'b0; readyOut=1'b0;

        //sending data with correct seqNumber for streamId 15
        //sending msgLength (24) and streamId (15) in little endian
        #10 dataIn=32'h1800_0F00; validIn=1'b1; lastIn=1'b0; readyOut=1'b0;
        //sending seqNumber(2) in little endian
        #10 dataIn=32'h0200_0000; validIn=1'b1; lastIn=1'b0; readyOut=1'b0;
        //sending data 1512223444
        #10 dataIn=32'h5A22B2D4; validIn=1'b1; lastIn=1'b0; readyOut=1'b0;
        //sending data 1512223902
        #10 dataIn=32'h5A22B49E; validIn=1'b1; lastIn=1'b0; readyOut=1'b0;
        //sending data 1122223902
        #10 dataIn=32'h42E3C71E; validIn=1'b1; lastIn=1'b0; readyOut=1'b0;
        //sending data 1134123902
        #10 dataIn=32'h43995B7E; validIn=1'b1; lastIn=1'b1; readyOut=1'b0;
        //completing transaction
        #10 dataIn=32'h43995B7E; validIn=1'b0; lastIn=1'b0; readyOut=1'b0;
        //sending to receiver
        #10 dataIn=32'h43995B7E; validIn=1'b0; lastIn=1'b0; readyOut=1'b1;

        //back to idle
        #10 dataIn=32'h43995B7E; validIn=1'b0; lastIn=1'b0; readyOut=1'b0;

        //sending data with wrong seqNumber for streamId 15 to generate o_packetLost
        //sending msgLength (24) and streamId (15) in little endian
        #10 dataIn=32'h1800_0F00; validIn=1'b1; lastIn=1'b0; readyOut=1'b0;
        //sending seqNumber(4) in little endian
        #10 dataIn=32'h0400_0000; validIn=1'b1; lastIn=1'b0; readyOut=1'b0;
        //sending data 1512223444
        #10 dataIn=32'h5A22B2D4; validIn=1'b1; lastIn=1'b0; readyOut=1'b0;
        //sending data 1512223902
        #10 dataIn=32'h5A22B49E; validIn=1'b1; lastIn=1'b0; readyOut=1'b0;
        //sending data 1122223902
        #10 dataIn=32'h42E3C71E; validIn=1'b1; lastIn=1'b0; readyOut=1'b0;
        //sending data 1134123902
        #10 dataIn=32'h43995B7E; validIn=1'b1; lastIn=1'b1; readyOut=1'b0;
        //completing transaction
        #10 dataIn=32'h43995B7E; validIn=1'b0; lastIn=1'b0; readyOut=1'b0;
        //sending to receiver
        #10 dataIn=32'h43995B7E; validIn=1'b0; lastIn=1'b0; readyOut=1'b1;

        //back to idle
        #10 dataIn=32'h43995B7E; validIn=1'b0; lastIn=1'b0; readyOut=1'b0;

        #10 rst_n   = 1'b1;
        $finish;
    end

endmodule
