class packet_item extends uvm_sequence_item;
    rand bit [15:0] msgLength;
    rand bit [15:0] streamId;
    rand bit [31:0] seqNumber;
    rand bit [7:0]  payload[];

    // Constraints
    constraint c_msgLength { msgLength inside {[9:45]}; }
    constraint c_streamId { streamId inside {[1:32]}; }
    constraint c_payload_size { payload.size() == (msgLength - 8); }

    `uvm_object_utils_begin(packet_item)
        `uvm_field_int(msgLength, UVM_ALL_ON)
        `uvm_field_int(streamId, UVM_ALL_ON)
        `uvm_field_int(seqNumber, UVM_ALL_ON)
        `uvm_field_array_int(payload, UVM_ALL_ON)
    `uvm_object_utils_end

    function new(string name = "packet_item");
        super.new(name);
    endfunction
endclass
