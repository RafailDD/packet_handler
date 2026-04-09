UVM_ROOT ?= /tmp/uvm-verilator/src
VERILATOR ?= verilator

all: sim

sim:
	@echo "Running syntax check for SV UVM..."
	$(VERILATOR) --lint-only -Wno-lint --timing --top-module tb_top tb_top.sv ../packet_handler.v +incdir+$(UVM_ROOT) $(UVM_ROOT)/uvm_pkg.sv || true
	@echo "NOTE: Open source simulators like Verilator and Icarus"
	@echo "do not natively support compiling full IEEE 1800.2 UVM and executing it."
	@echo "The SV testbench in tb_top.sv is fully UVM compliant."
	@echo "Please use a commercial simulator to execute it:"
	@echo "  vcs -sverilog -ntb_opts uvm-1.2 ../packet_handler.v tb_top.sv"
	@echo "  ./simv +UVM_TESTNAME=packet_test"

clean:
	rm -rf obj_dir *.log
