`timescale 1ns/1ps

`include "uvm_macros.svh"
import uvm_pkg::*;

// Interface
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

// UVM Sequence Item
class packet_item extends uvm_sequence_item;
    rand bit [15:0] msgLength;
    rand bit [15:0] streamId;
    rand bit [31:0] seqNumber;
    bit [7:0] payload[64];

    // Constraints
    constraint c_msgLength { msgLength inside {[9:45]}; }
    constraint c_streamId { streamId inside {[1:32]}; }

    `uvm_object_utils_begin(packet_item)
        `uvm_field_int(msgLength, UVM_ALL_ON)
        `uvm_field_int(streamId, UVM_ALL_ON)
        `uvm_field_int(seqNumber, UVM_ALL_ON)
        //`uvm_field_array_int(payload, UVM_ALL_ON)
    `uvm_object_utils_end

    function new(string name = "packet_item");
        super.new(name);
    endfunction
endclass

class packet_driver extends uvm_driver #(packet_item);
    `uvm_component_utils(packet_driver)

    virtual packet_if vif;

    function new(string name, uvm_component parent);
        super.new(name, parent);
    endfunction

    function void build_phase(uvm_phase phase);
        super.build_phase(phase);
        if(!uvm_config_db#(virtual packet_if)::get(this, "", "vif", vif))
            `uvm_fatal("NO_VIF", "Virtual interface must be set for vif")
    endfunction

    task run_phase(uvm_phase phase);
        vif.valid <= 0;
        vif.last <= 0;
        vif.data <= 0;
        wait(vif.rst_n);

        forever begin
            seq_item_port.get_next_item(req);
            send_packet(req);
            seq_item_port.item_done();
        end
    endtask

    task send_packet(packet_item item);
        int words = (item.msgLength + 3) / 4;
        logic [31:0] header_w1;
        logic [31:0] header_w2;

        // Wait for ready
        @(posedge vif.clk);
        while(!vif.o_ready) @(posedge vif.clk);

        header_w1 = {item.msgLength[7:0], item.msgLength[15:8], item.streamId[7:0], item.streamId[15:8]};
        header_w2 = {item.seqNumber[7:0], item.seqNumber[15:8], item.seqNumber[23:16], item.seqNumber[31:24]};

        vif.valid <= 1;
        vif.data <= header_w1;
        @(posedge vif.clk);
        vif.data <= header_w2;
        @(posedge vif.clk);

        for(int i = 0; i < (item.msgLength - 8); i+=4) begin
            logic [31:0] pdata = 0;
            pdata[7:0]   = (i < (item.msgLength - 8)) ? item.payload[i] : 8'd0;
            pdata[15:8]  = (i+1 < (item.msgLength - 8)) ? item.payload[i+1] : 8'd0;
            pdata[23:16] = (i+2 < (item.msgLength - 8)) ? item.payload[i+2] : 8'd0;
            pdata[31:24] = (i+3 < (item.msgLength - 8)) ? item.payload[i+3] : 8'd0;

            vif.data <= pdata;
            if(i+4 >= (item.msgLength - 8))
                vif.last <= 1;
            @(posedge vif.clk);
        end
        vif.valid <= 0;
        vif.last <= 0;
    endtask
endclass

class packet_monitor extends uvm_monitor;
    `uvm_component_utils(packet_monitor)
    virtual packet_if vif;
    uvm_analysis_port #(packet_item) ap;
    packet_item req_sampled;

    // Covergroup for coverage



    function new(string name, uvm_component parent);
        super.new(name, parent);
        ap = new("ap", this);
    endfunction

    function void build_phase(uvm_phase phase);
        super.build_phase(phase);
        if(!uvm_config_db#(virtual packet_if)::get(this, "", "vif", vif))
            `uvm_fatal("NO_VIF", "Virtual interface must be set for vif")
    endfunction

    task run_phase(uvm_phase phase);
        forever begin
            @(posedge vif.clk);
            if(vif.valid && vif.o_ready) begin
                packet_item pkt = packet_item::type_id::create("pkt");
                logic [31:0] w1, w2;
                w1 = vif.data;
                pkt.msgLength = {w1[15:8], w1[7:0]};
                pkt.streamId  = {w1[31:24], w1[23:16]};
                @(posedge vif.clk);
                w2 = vif.data;
                pkt.seqNumber = {w2[31:24], w2[23:16], w2[15:8], w2[7:0]};
                // Wait for the packet to finish sending
                while(!vif.last) @(posedge vif.clk);
                // Send the transaction to the scoreboard
                ap.write(pkt);
                req_sampled = pkt;
            end
        end
    endtask
endclass

class packet_scoreboard extends uvm_scoreboard;
    `uvm_component_utils(packet_scoreboard)
    uvm_analysis_export #(packet_item) ap_export;
    uvm_tlm_analysis_fifo #(packet_item) exp_fifo;

    function new(string name, uvm_component parent);
        super.new(name, parent);
    endfunction

    function void build_phase(uvm_phase phase);
        super.build_phase(phase);
        ap_export = new("ap_export", this);
        exp_fifo = new("exp_fifo", this);
    endfunction

    function void connect_phase(uvm_phase phase);
        ap_export.connect(exp_fifo.analysis_export);
    endfunction

    task run_phase(uvm_phase phase);
        packet_item item;
        forever begin
            exp_fifo.get(item);
            if (item.streamId >= 1 && item.streamId <= 32 && item.seqNumber > 0)
                `uvm_info("SCOREBOARD", $sformatf("PASS: Packet monitored successfully. Stream: %0d, Seq: %0d, Length: %0d", item.streamId, item.seqNumber, item.msgLength), UVM_LOW)
            else
                `uvm_error("SCOREBOARD", $sformatf("FAIL: Bad packet format. Stream: %0d, Seq: %0d, Length: %0d", item.streamId, item.seqNumber, item.msgLength))
        end
    endtask
endclass

class packet_agent extends uvm_agent;
    `uvm_component_utils(packet_agent)

    packet_driver driver;
    packet_monitor monitor;
    uvm_sequencer #(packet_item) sequencer;

    function new(string name, uvm_component parent);
        super.new(name, parent);
    endfunction

    function void build_phase(uvm_phase phase);
        super.build_phase(phase);
        driver = packet_driver::type_id::create("driver", this);
        monitor = packet_monitor::type_id::create("monitor", this);
        sequencer = new("sequencer", this);
    endfunction

    function void connect_phase(uvm_phase phase);
        driver.seq_item_port.connect(sequencer.seq_item_export);
    endfunction
endclass

class packet_env extends uvm_env;
    `uvm_component_utils(packet_env)

    packet_agent agent;
    packet_scoreboard sb;

    function new(string name, uvm_component parent);
        super.new(name, parent);
    endfunction

    function void build_phase(uvm_phase phase);
        super.build_phase(phase);
        agent = packet_agent::type_id::create("agent", this);
        sb = packet_scoreboard::type_id::create("sb", this);
    endfunction

    function void connect_phase(uvm_phase phase);
        agent.monitor.ap.connect(sb.ap_export);
    endfunction
endclass

class packet_seq extends uvm_sequence #(packet_item);
    `uvm_object_utils(packet_seq)

    function new(string name = "packet_seq");
        super.new(name);
    endfunction

    task body();

        // Test normal packets
        for(int i = 1; i <= 5; i++) begin
            req = packet_item::type_id::create("req");
            start_item(req);
            assert(req.randomize());
            req.seqNumber = i;
            req.streamId = 15;
            finish_item(req);
        end
        // Test missing packet (seq 7 instead of 6) to trigger packetLost
        req = packet_item::type_id::create("req");
        start_item(req);
        assert(req.randomize());
        req.seqNumber = 7;
        req.streamId = 15;
        finish_item(req);
    endtask
endclass

class packet_test extends uvm_test;
    `uvm_component_utils(packet_test)

    packet_env env;

    function new(string name, uvm_component parent);
        super.new(name, parent);
    endfunction

    function void build_phase(uvm_phase phase);
        super.build_phase(phase);
        env = packet_env::type_id::create("env", this);
    endfunction

    task run_phase(uvm_phase phase);
        packet_seq seq;
        phase.raise_objection(this);
        seq = packet_seq::type_id::create("seq");
        seq.start(env.agent.sequencer);
        #100;
        phase.drop_objection(this);
    endtask
endclass

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
