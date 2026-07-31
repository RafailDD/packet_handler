`timescale 1ns/1ps

`include "uvm_macros.svh"
import uvm_pkg::*;

`include "packet_if.sv"
`include "seq/packet_item.sv"
`include "seq/packet_seq.sv"
`include "env/packet_driver.sv"
`include "env/packet_monitor.sv"
`include "env/packet_scoreboard.sv"
`include "env/packet_agent.sv"
`include "env/packet_env.sv"
`include "tests/packet_test.sv"

module tb_top;
    logic clk;
    logic rst_n;

    packet_if vif(.clk(clk), .rst_n(rst_n));

    packet_handler dut (
        .i_clk(clk),
        .i_rst_n(rst_n),
        .i_data(vif.data),
        .i_valid(vif.valid),
        .i_ready(vif.ready_in),
        .i_last(vif.last),
        .o_data(vif.o_data),
        .o_ready(vif.o_ready),
        .o_valid(vif.o_valid),
        .o_packetLost(vif.o_packetLost)
    );

    initial begin
        clk = 0;
        forever #5 clk = ~clk;
    end

    initial begin
        rst_n = 0;
        #20 rst_n = 1;
    end

    initial begin
        vif.ready_in = 1; // Always ready receiver for simplicity
        uvm_config_db#(virtual packet_if)::set(null, "*", "vif", vif);
        run_test("packet_test");
    end
endmodule
