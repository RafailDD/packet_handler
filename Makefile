# Unified Makefile for Packet Handler Testbenches
# Supports cocotb, icarus, verilator, and UVM (xezim / Verilator) flows.

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
	@echo "  make xezim_uvm       : Run full UVM testbench via xezim."
	@echo "  make verilator_uvm   : Run full UVM testbench via Verilator (v5.052+)"
	@echo "                         with the Accellera UVM library + code coverage."
	@echo "  make verilator_uvm_all : Same, running every test in ALL_TESTNAMES."
	@echo "  make synth           : Run Yosys synthesis (Sky130 HD) followed by OpenSTA."
	@echo "  make sta             : Run OpenSTA on the existing synth/synth_netlist.v."
	@echo "  make synth_parse     : Parse and summarize Yosys synthesis log."
	@echo "  make sc ARGS=\"...\"   : Run a SiliconCompiler flow (flow/sc_flow.py --help)."
	@echo "  make sc_image        : Build the packet-handler-eda toolchain image."
	@echo "  make benchmark       : Run performance benchmark test for python logic."
	@echo "  make clean           : Remove generated simulation files and artifacts."
	@echo ""
	@echo "Options:"
	@echo "  WAVE=1               : Enable waveform generation (.vcd)."
	@echo "                         Applicable to all targets."
	@echo "  GUI=1                : Enable waveform generation and open it in GTKWave."
	@echo "                         Applicable to all targets."
	@echo "  TB=<testbench>       : Specify testbench file for icarus and verilator."
	@echo "                         e.g., TB=tb_smoke"
	@echo "  UVM_TESTNAME=<test>  : UVM test for verilator_uvm."
	@echo "                         Defaults to packet_test."
	@echo "  COV=0                : Disable code coverage for verilator_uvm."
	@echo "  SEED=<n>             : Randomization seed for verilator_uvm (default 1)."
	@echo "  CLK_PERIOD_NS=<ns>   : Target clock period for synth / sta (default 10)."
	@echo "  SRAM=1               : Use the Sky130 SRAM macro for the FIFO (cocotb, icarus,"
	@echo "                         verilator, synth, sta). Default: behavioral model."
	@echo "======================================================================"

# ==========================================
# GUI implied WAVE
# ==========================================
ifeq ($(GUI),1)
    WAVE=1
endif

# ==========================================
# FIFO memory implementation (SRAM=1)
# ==========================================
# SRAM=1 defines PH_SKY130_SRAM, so ph_fifo_mem instantiates the Sky130
# OpenRAM macro instead of its behavioral model. Simulation adds the macro
# model in macros/sky130_sram_model.v; synthesis / STA need the macro's
# Liberty file, fetched into macros/sky130_sram/ by `make fetch_sram`.
SRAM ?= 0
SRAM_MACRO = sky130_sram_2kbyte_1rw1r_32x512_8
SRAM_DIR   = macros/sky130_sram
SRAM_URL   = https://raw.githubusercontent.com/siliconcompiler/lambdapdk/v0.2.22/lambdapdk/sky130/libs/sky130sram/$(SRAM_MACRO)
SRAM_LIB   = $(SRAM_DIR)/$(SRAM_MACRO)_TT_1p8V_25C.lib

ifeq ($(SRAM),1)
    SIM_RTL        = packet_handler.v macros/sky130_sram_model.v
    IVERILOG_SRAM  = -D PH_SKY130_SRAM
    VERILATOR_SRAM = +define+PH_SKY130_SRAM
else
    SIM_RTL        = packet_handler.v
    IVERILOG_SRAM  =
    VERILATOR_SRAM =
endif

.PHONY: fetch_sram
fetch_sram:
	@mkdir -p $(SRAM_DIR)
	@for f in nldm/$(SRAM_MACRO)_TT_1p8V_25C.lib lef/$(SRAM_MACRO).lef; do \
		if [ ! -f $(SRAM_DIR)/$$(basename $$f) ]; then \
			echo "Fetching $$(basename $$f)..."; \
			wget -q -O $(SRAM_DIR)/$$(basename $$f) $(SRAM_URL)/$$f || exit 1; \
		fi; \
	done

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
VERILOG_SOURCES = $(addprefix $(PWD)/,$(SIM_RTL))
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
ifeq ($(SRAM),1)
    COMPILE_ARGS += -D PH_SKY130_SRAM
endif

cocotb: run_cocotb
	@if [ "$(WAVE)" = "1" ]; then cp sim_build/*.vcd waves_cocotb.vcd || true; fi
	@if [ "$(GUI)" = "1" ]; then gtkwave waves_cocotb.vcd & fi

run_cocotb:
	$(MAKE) sim

# Only pull in cocotb's rules when cocotb is installed, so the other flows
# (e.g. in a Verilator-only container) still work without it.
COCOTB_MAKEFILES := $(shell cocotb-config --makefiles 2>/dev/null)
ifneq ($(COCOTB_MAKEFILES),)
include $(COCOTB_MAKEFILES)/Makefile.sim
else
sim:
	$(error cocotb not found: install it with 'pip install cocotb')
endif

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
	iverilog -g2012 -I tests $(IVERILOG_SRAM) -D WAVE=1 -D WAVE_FILE=\"waves_icarus_$(TB_CLEAN).vcd\" -o sim_icarus_$(TB_CLEAN).out $(SIM_RTL) $(wildcard tests/$(TB_CLEAN).v) $(wildcard tests/$(TB_CLEAN).sv) 2>&1 | tee tests/logs/sim_icarus_$(TB_CLEAN).log
else
	iverilog -g2012 -I tests $(IVERILOG_SRAM) -o sim_icarus_$(TB_CLEAN).out $(SIM_RTL) $(wildcard tests/$(TB_CLEAN).v) $(wildcard tests/$(TB_CLEAN).sv) 2>&1 | tee tests/logs/sim_icarus_$(TB_CLEAN).log
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
	verilator --binary -Itests $(VERILATOR_SRAM) -j 0 -Wall --trace +define+WAVE=1 +define+WAVE_FILE=\"waves_verilator_$(TB_CLEAN).vcd\" $(SIM_RTL) $(wildcard tests/$(TB_CLEAN).v) $(wildcard tests/$(TB_CLEAN).sv) 2>&1 | tee tests/logs/sim_verilator_$(TB_CLEAN).log
else
	verilator --binary -Itests $(VERILATOR_SRAM) -j 0 -Wall $(SIM_RTL) $(wildcard tests/$(TB_CLEAN).v) $(wildcard tests/$(TB_CLEAN).sv) 2>&1 | tee tests/logs/sim_verilator_$(TB_CLEAN).log
endif
	./obj_dir/Vpacket_handler 2>&1 | tee -a tests/logs/sim_verilator_$(TB_CLEAN).log
	@if [ "$(GUI)" = "1" ]; then gtkwave waves_verilator_$(TB_CLEAN).vcd & fi

# ==========================================
# Xezim UVM Flow
# ==========================================
.PHONY: xezim_uvm

xezim_uvm:
	@echo "Running UVM tests via xezim..."
	$(MAKE) -C tests/uvm sim

# ==========================================
# Verilator UVM Flow
# ==========================================
.PHONY: verilator_uvm verilator_uvm_all

verilator_uvm:
	@echo "Running UVM tests via Verilator..."
	$(MAKE) -C tests/uvm verilator

verilator_uvm_all:
	@echo "Running all UVM tests via Verilator..."
	$(MAKE) -C tests/uvm verilator_all

# ==========================================
# Synthesis Flow (Yosys + OpenSTA)
# ==========================================
.PHONY: fetch_lib synth sta synth_parse

LIB_FILE=synth/lib/sky130_fd_sc_hd__tt_025C_1v80.lib
LIB_URL=https://raw.githubusercontent.com/efabless/skywater-pdk-libs-sky130_fd_sc_hd/master/timing/sky130_fd_sc_hd__tt_025C_1v80.lib

# Target clock period in ns, used by ABC (synth/synth.tcl) and by the SDC.
CLK_PERIOD_NS ?= 10
export CLK_PERIOD_NS

# Container with OpenSTA, OpenROAD, Yosys, Icarus and Verilator. Used by
# `make sta` when `sta` is not on PATH, and by the SiliconCompiler flows.
EDA_IMAGE ?= ghcr.io/siliconcompiler/sc_runner:v0.38.10
STA_BIN := $(shell command -v sta 2>/dev/null)
ifneq ($(STA_BIN),)
    STA_CMD = sta
else
    STA_CMD = docker run --rm -e CLK_PERIOD_NS -e EXTRA_LIBS -v "$(PWD)":/work -w /work/synth $(EDA_IMAGE) sta
endif

fetch_lib:
	@mkdir -p synth/lib
	@if [ ! -f $(LIB_FILE) ]; then \
		echo "Fetching Sky130 standard cell library..."; \
		wget -q -O $(LIB_FILE) $(LIB_URL); \
	else \
		echo "Sky130 standard cell library already present."; \
	fi

ifeq ($(SRAM),1)
    export SYNTH_DEFINES    = PH_SKY130_SRAM
    export SYNTH_EXTRA_LIBS = ../$(SRAM_LIB)
    export EXTRA_LIBS       = ../$(SRAM_LIB)
endif

synth: fetch_lib $(if $(filter 1,$(SRAM)),fetch_sram)
	@echo "Running Yosys Synthesis Flow (CLK_PERIOD_NS=$(CLK_PERIOD_NS) SRAM=$(SRAM))..."
	@cd synth && yosys -c synth.tcl -l synth.log > /dev/null
	$(MAKE) --no-print-directory sta

sta:
	@test -f synth/synth_netlist.v || { echo "synth/synth_netlist.v not found: run 'make synth' first"; exit 1; }
	@echo "Running OpenSTA (CLK_PERIOD_NS=$(CLK_PERIOD_NS))..."
	@cd synth && $(STA_CMD) -no_splash -exit sta.tcl > sta.rpt 2>&1; rc=$$?; \
		grep -E "^(wns|tns|worst slack)|period_min" sta.rpt; exit $$rc

synth_parse:
	@echo "Parsing Synthesis Log..."
	@python3 synth/scripts/synth_parse.py synth/synth.log synth/sta.rpt
# ==========================================
# SiliconCompiler Flows
# ==========================================
# flow/sc_flow.py runs every flow (lint, sim, cocotb, uvm, syn, asic) from one
# design description. `make sc ARGS="<flow> [options]"` runs it in the
# packet-handler-eda image (built by `make sc_image`), or with the local
# Python when SC_LOCAL=1 and siliconcompiler is installed.
#   make sc ARGS="sim --tool verilator --tb tb_uvm_sv"
#   make sc ARGS="syn --sram --clk 8"
.PHONY: sc sc_image

SC_IMAGE ?= packet-handler-eda
ARGS ?= lint
ifeq ($(SC_LOCAL),1)
    SC_CMD = python3
else
    SC_CMD = docker run --rm -e CLK_PERIOD_NS -v "$(PWD)":/work -w /work $(SC_IMAGE) python3
endif

sc_image:
	docker build -t $(SC_IMAGE) flow/

sc:
	$(SC_CMD) flow/sc_flow.py $(ARGS)

# ==========================================
# Benchmark Flow
# ==========================================
.PHONY: benchmark
benchmark:
	python3 tests/benchmark.py

# ==========================================
# Clean
# ==========================================
.PHONY: clean
clean::
	$(MAKE) -C tests/uvm clean
	rm -rf sim_build/ obj_dir/ sim_*.out wave*.vcd waves_*.vcd waves_*.fst results.xml __pycache__/ *.vcd tests/__pycache__/ synth/synth.log synth/synth_netlist.v synth/sta.rpt synth/abc.constr build/
