class packet_monitor extends uvm_monitor;
    `uvm_component_utils(packet_monitor)
    virtual packet_if vif;
    uvm_analysis_port #(packet_item) ap;
    packet_item req_sampled;

    // Covergroup for coverage
    covergroup packet_cg;
        option.per_instance = 1;
        cp_msgLength: coverpoint req_sampled.msgLength {
            bins min = {9};
            bins mid = {[10:44]};
            bins max = {45};
        }
        cp_streamId: coverpoint req_sampled.streamId {
            bins id1 = {1};
            bins others = {[2:31]};
            bins id32 = {32};
        }
    endgroup

    function new(string name, uvm_component parent);
        super.new(name, parent);
        ap = new("ap", this);
        packet_cg = new();
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
                pkt.msgLength = {w1[7:0], w1[15:8]};
                pkt.streamId  = {w1[23:16], w1[31:24]};
                @(posedge vif.clk);
                w2 = vif.data;
                pkt.seqNumber = {w2[7:0], w2[15:8], w2[23:16], w2[31:24]};
                // Wait for the packet to finish sending
                while(!vif.last) @(posedge vif.clk);
                // Send the transaction to the scoreboard
                ap.write(pkt);
                req_sampled = pkt;
                packet_cg.sample();
            end
        end
    endtask
endclass
