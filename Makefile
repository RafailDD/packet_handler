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
	@echo "                         Defaults to TB=tb_icarus_sv."
	@echo "  make verilator-uvm   : Run Verilator pure SV UVM test flow."
	@echo "                         Defaults to TB=tb_verilator_uvm."
	@echo "  make clean           : Remove generated simulation files and artifacts."
	@echo ""
	@echo "Options:"
	@echo "  WAVE=1               : Enable waveform generation (.vcd/.fst)."
	@echo "                         Applicable to all targets."
	@echo "  GUI=1                : Enable waveform generation and open it in GTKWave."
	@echo "                         Applicable to all targets."
	@echo "  TB=<testbench>       : Specify testbench file for icarus and verilator-uvm."
	@echo "                         e.g., TB=tb_smoke"
	@echo "======================================================================"

# ==========================================
# GUI implied WAVE
# ==========================================
ifeq ($(GUI),1)
    WAVE=1
endif

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

ifeq ($(WAVE),1)
    export WAVES=1
    COMPILE_ARGS += -D WAVE=1
endif

cocotb: run_cocotb
	@if [ "$(WAVE)" = "1" ]; then cp sim_build/*.fst waves_cocotb.fst || true; fi
	@if [ "$(GUI)" = "1" ]; then gtkwave waves_cocotb.fst & fi

run_cocotb:
	$(MAKE) sim

include $(shell cocotb-config --makefiles)/Makefile.sim

# ==========================================
# Icarus Flow
# ==========================================
.PHONY: icarus

# Default TB for Icarus
icarus: TB ?= tb_icarus_sv
icarus:
	@echo "Running Icarus Verilog with testbench: $(TB)"
ifeq ($(WAVE),1)
	iverilog -g2012 -D WAVE=1 -D WAVE_FILE=\"waves_icarus_$(TB).vcd\" -o sim_icarus_$(TB).out packet_handler.v $(wildcard $(TB).v) $(wildcard $(TB).sv)
else
	iverilog -g2012 -o sim_icarus_$(TB).out packet_handler.v $(wildcard $(TB).v) $(wildcard $(TB).sv)
endif
	vvp sim_icarus_$(TB).out
	@if [ "$(GUI)" = "1" ]; then gtkwave waves_icarus_$(TB).vcd & fi


# ==========================================
# Verilator UVM Flow
# ==========================================
.PHONY: verilator-uvm

# Default TB for Verilator
verilator-uvm: TB ?= tb_verilator_uvm
verilator-uvm:
	@echo "Running Verilator with testbench: $(TB)"
ifeq ($(WAVE),1)
	verilator --binary -j 0 -Wall --trace +define+WAVE=1 +define+WAVE_FILE=\"waves_verilator_$(TB).vcd\" packet_handler.v $(wildcard $(TB).v) $(wildcard $(TB).sv)
else
	verilator --binary -j 0 -Wall packet_handler.v $(wildcard $(TB).v) $(wildcard $(TB).sv)
endif
	./obj_dir/Vpacket_handler
	@if [ "$(GUI)" = "1" ]; then gtkwave waves_verilator_$(TB).vcd & fi

# ==========================================
# Clean
# ==========================================
.PHONY: clean
clean::
	rm -rf sim_build/ obj_dir/ sim_*.out wave*.vcd waves_*.vcd waves_*.fst results.xml __pycache__/ *.vcd
