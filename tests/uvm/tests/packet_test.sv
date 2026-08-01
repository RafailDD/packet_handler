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
