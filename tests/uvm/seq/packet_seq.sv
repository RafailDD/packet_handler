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

        // Random stream packets
        for(int i = 1; i <= 10; i++) begin
            req = packet_item::type_id::create("req");
            start_item(req);
            assert(req.randomize());
            req.seqNumber = i;
            finish_item(req);
        end
    endtask
endclass
