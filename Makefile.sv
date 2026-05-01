# Pure SystemVerilog UVM-style testbench Makefile for Verilator
# Uses Verilator to compile and execute testbench.sv and packet_handler.v

.PHONY: all run clean

all: run

run: obj_dir/Vpacket_handler
	./obj_dir/Vpacket_handler

obj_dir/Vpacket_handler: packet_handler.v testbench.sv
	verilator --binary -j 0 -Wall packet_handler.v testbench.sv

clean:
	rm -rf obj_dir
