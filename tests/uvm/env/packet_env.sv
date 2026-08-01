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
