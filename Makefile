# Unified Makefile for Packet Handler Testbenches
# Supports cocotb, icarus, and verilator-uvm flows.

# Default Target
.PHONY: help
help:
	@echo "======================================================================"
	@echo "Packet Handler Unified Testbench Environment"
	@echo "======================================================================"
	@echo "Usage: make <target> [OPTIONS]"
	@echo ""
	@echo "Targets:"
	@echo "  make cocotb          : Run cocotb test flow."
	@echo "                         Uses tb_cocotb.py."
	@echo "  make icarus          : Run Icarus Verilog test flow."
	@echo "                         Requires TB parameter."
	@echo "  make verilator-uvm   : Run Verilator pure SV UVM test flow."
	@echo "                         Requires TB parameter."
	@echo "  make clean           : Remove generated simulation files and artifacts."
	@echo ""
	@echo "Options:"
	@echo "  WAVES=1              : Enable waveform generation (.vcd)."
	@echo "                         Applicable to all targets."
	@echo "  TB=<testbench>       : Specify testbench file for icarus and verilator-uvm."
	@echo "                         e.g., TB=tb_icarus_sv or TB=tb_smoke"
	@echo "                         e.g., TB=tb_verilator_uvm or TB=tb_smoke"
	@echo "======================================================================"

# ==========================================
# Cocotb Flow
# ==========================================
.PHONY: cocotb run_cocotb

# Setup cocotb variables
SIM ?= icarus
TOPLEVEL_LANG ?= verilog
PWD=$(shell pwd)
VERILOG_SOURCES = $(PWD)/packet_handler.v
TOPLEVEL = packet_handler
COCOTB_TEST_MODULES = tb_cocotb

ifeq ($(WAVES),1)
    export WAVES=1
    COMPILE_ARGS += -D WAVES=1
endif

cocotb: run_cocotb
	@if [ "$(WAVES)" = "1" ]; then cp sim_build/*.fst waves_cocotb.fst || true; fi

run_cocotb:
	$(MAKE) sim

include $(shell cocotb-config --makefiles)/Makefile.sim

# ==========================================
# Icarus Flow
# ==========================================
.PHONY: icarus

icarus:
ifndef TB
	$(error "TB parameter must be provided for icarus flow. e.g., make icarus TB=tb_smoke")
endif
	@echo "Running Icarus Verilog with testbench: $(TB)"
ifeq ($(WAVES),1)
	iverilog -g2012 -D WAVES=1 -D WAVE_FILE=\"waves_icarus_$(TB).vcd\" -o sim_icarus_$(TB).out packet_handler.v $(wildcard $(TB).v) $(wildcard $(TB).sv)
else
	iverilog -g2012 -o sim_icarus_$(TB).out packet_handler.v $(wildcard $(TB).v) $(wildcard $(TB).sv)
endif
	vvp sim_icarus_$(TB).out


# ==========================================
# Verilator UVM Flow
# ==========================================
.PHONY: verilator-uvm

verilator-uvm:
ifndef TB
	$(error "TB parameter must be provided for verilator-uvm flow. e.g., make verilator-uvm TB=tb_verilator_uvm")
endif
	@echo "Running Verilator with testbench: $(TB)"
ifeq ($(WAVES),1)
	verilator --binary -j 0 -Wall --trace +define+WAVES=1 +define+WAVE_FILE=\"waves_verilator_$(TB).vcd\" packet_handler.v $(wildcard $(TB).v) $(wildcard $(TB).sv)
else
	verilator --binary -j 0 -Wall packet_handler.v $(wildcard $(TB).v) $(wildcard $(TB).sv)
endif
	./obj_dir/Vpacket_handler

# ==========================================
# Clean
# ==========================================
.PHONY: clean
clean::
	rm -rf sim_build/ obj_dir/ sim_*.out wave*.vcd waves_*.vcd waves_*.fst results.xml __pycache__/ *.vcd
