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
	@echo "                         Uses tests/tb_cocotb.py."
	@echo "  make icarus          : Run Icarus Verilog test flow."
	@echo "                         Defaults to TB=tb_basic_sv."
	@echo "  make verilator       : Run Verilator pure SV UVM test flow."
	@echo "                         Defaults to TB=tb_uvm_sv."
	@echo "  make clean           : Remove generated simulation files and artifacts."
	@echo ""
	@echo "Options:"
	@echo "  WAVE=1               : Enable waveform generation (.vcd)."
	@echo "                         Applicable to all targets."
	@echo "  GUI=1                : Enable waveform generation and open it in GTKWave."
	@echo "                         Applicable to all targets."
	@echo "  TB=<testbench>       : Specify testbench file for icarus and verilator."
	@echo "                         e.g., TB=tb_smoke"
	@echo "======================================================================"

# ==========================================
# GUI implied WAVE
# ==========================================
ifeq ($(GUI),1)
    WAVE=1
endif

# ==========================================
# Parse and Sanitize Inputs
# ==========================================
ifneq ($(TB),)
    # Strip extension (e.g. tb_smoke.v -> tb_smoke)
    TB_CLEAN = $(basename $(TB))
    ifneq ($(TB),$(TB_CLEAN))
        $(info Note: Stripped file extension from TB argument. Using TB=$(TB_CLEAN))
    endif
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
# Set python path so cocotb can find the module in tests/
export PYTHONPATH := $(PWD)/tests:$(PYTHONPATH)
COCOTB_TEST_MODULES = tb_cocotb

ifeq ($(WAVE),1)
    export WAVES=1
    # Force cocotb to generate vcd instead of fst
    export COCOTB_RESULTS_FILE=results.xml
    export COCOTB_RESOLVE_X=ZEROS
    export WAVE_FMT=vcd
    COMPILE_ARGS += -D WAVE=1
endif

cocotb: run_cocotb
	@if [ "$(WAVE)" = "1" ]; then cp sim_build/*.vcd waves_cocotb.vcd || true; fi
	@if [ "$(GUI)" = "1" ]; then gtkwave waves_cocotb.vcd & fi

run_cocotb:
	$(MAKE) sim

include $(shell cocotb-config --makefiles)/Makefile.sim

# ==========================================
# Icarus Flow
# ==========================================
.PHONY: icarus

# Default TB for Icarus
icarus: TB_CLEAN ?= tb_basic_sv
icarus:
	@echo "Running Icarus Verilog with testbench: $(TB_CLEAN)"
	@mkdir -p tests/logs
ifeq ($(WAVE),1)
	iverilog -g2012 -I tests -D WAVE=1 -D WAVE_FILE=\"waves_icarus_$(TB_CLEAN).vcd\" -o sim_icarus_$(TB_CLEAN).out packet_handler.v $(wildcard tests/$(TB_CLEAN).v) $(wildcard tests/$(TB_CLEAN).sv) 2>&1 | tee tests/logs/sim_icarus_$(TB_CLEAN).log
else
	iverilog -g2012 -I tests -o sim_icarus_$(TB_CLEAN).out packet_handler.v $(wildcard tests/$(TB_CLEAN).v) $(wildcard tests/$(TB_CLEAN).sv) 2>&1 | tee tests/logs/sim_icarus_$(TB_CLEAN).log
endif
	vvp sim_icarus_$(TB_CLEAN).out 2>&1 | tee -a tests/logs/sim_icarus_$(TB_CLEAN).log
	@if [ "$(GUI)" = "1" ]; then gtkwave waves_icarus_$(TB_CLEAN).vcd & fi


# ==========================================
# Verilator Flow
# ==========================================
.PHONY: verilator

# Default TB for Verilator
verilator: TB_CLEAN ?= tb_uvm_sv
verilator:
	@echo "Running Verilator with testbench: $(TB_CLEAN)"
	@mkdir -p tests/logs
ifeq ($(WAVE),1)
	verilator --binary -Itests -j 0 -Wall --trace +define+WAVE=1 +define+WAVE_FILE=\"waves_verilator_$(TB_CLEAN).vcd\" packet_handler.v $(wildcard tests/$(TB_CLEAN).v) $(wildcard tests/$(TB_CLEAN).sv) 2>&1 | tee tests/logs/sim_verilator_$(TB_CLEAN).log
else
	verilator --binary -Itests -j 0 -Wall packet_handler.v $(wildcard tests/$(TB_CLEAN).v) $(wildcard tests/$(TB_CLEAN).sv) 2>&1 | tee tests/logs/sim_verilator_$(TB_CLEAN).log
endif
	./obj_dir/Vpacket_handler 2>&1 | tee -a tests/logs/sim_verilator_$(TB_CLEAN).log
	@if [ "$(GUI)" = "1" ]; then gtkwave waves_verilator_$(TB_CLEAN).vcd & fi

# ==========================================
# Clean
# ==========================================
.PHONY: clean
clean::
	rm -rf sim_build/ obj_dir/ sim_*.out wave*.vcd waves_*.vcd waves_*.fst results.xml __pycache__/ *.vcd tests/__pycache__/
