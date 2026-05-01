/* verilator lint_off DECLFILENAME */

class packet_item;
    /* verilator lint_off UNUSEDSIGNAL */
    int msgLength;
    int streamId;
    int seqNumber;
    int data_size;
    int data_arr[9];
    /* verilator lint_on UNUSEDSIGNAL */

    function new();
    endfunction

    function void randomize_me();
        data_size = ($urandom() % 9) + 1;
        /* verilator lint_off WIDTHTRUNC */
        msgLength = 8 + data_size * 4;
        streamId = ($urandom() % 32) + 1;
        /* verilator lint_on WIDTHTRUNC */
        seqNumber = $urandom();
    endfunction
endclass

module top;
    packet_item pkt;
    initial begin
        pkt = new();
        pkt.randomize_me();
        $display("Generated packet: streamId=%0d, seqNumber=%0d, msgLength=%0d, data_size=%0d", pkt.streamId, pkt.seqNumber, pkt.msgLength, pkt.data_size);
        $finish;
    end
endmodule
