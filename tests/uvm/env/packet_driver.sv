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

        header_w1 = {item.streamId[15:8], item.streamId[7:0], item.msgLength[15:8], item.msgLength[7:0]};
        header_w2 = {item.seqNumber[31:24], item.seqNumber[23:16], item.seqNumber[15:8], item.seqNumber[7:0]};

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
