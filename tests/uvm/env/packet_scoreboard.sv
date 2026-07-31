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
