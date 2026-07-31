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
