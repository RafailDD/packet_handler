interface packet_if (input logic clk, input logic rst_n);
    logic [31:0]  data;
    logic         valid;
    logic         ready_in; // i_ready
    logic         last;

    logic [295:0] o_data;
    logic         o_ready;
    logic         o_valid;
    logic         o_packetLost;
endinterface
