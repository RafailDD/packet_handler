# UVM flows for packet_handler: xezim (sim) and Verilator (verilator).
# Both simulators compile the same tb_top.sv testbench.

# Run recipes with bash + pipefail so `sim | tee log` keeps the simulator's exit status.
SHELL := /bin/bash
.SHELLFLAGS := -o pipefail -c

RTL     = ../../packet_handler.v
TB_TOP  = tb_top.sv
LOG_DIR = logs

UVM_TESTNAME  ?= packet_test
ALL_TESTNAMES ?= packet_test

ifeq ($(GUI),1)
    WAVE=1
endif

.PHONY: all help sim verilator verilator_all verilator_build verilator_cov clean FORCE

all: sim

help:
	@echo "UVM flows (run from tests/uvm, or via the top-level Makefile):"
	@echo "  make sim             : xezim + chipsalliance/uvm-verilator (make xezim_uvm)"
	@echo "  make verilator       : Verilator + Accellera uvm-core, run UVM_TESTNAME (make verilator_uvm)"
	@echo "  make verilator_all   : Verilator, run every test in ALL_TESTNAMES"
	@echo "  make verilator_build : Verilator compile only"
	@echo "  make verilator_cov   : merge coverage from logs/coverage/*.cov"
	@echo "  make clean           : remove downloaded UVM sources and build outputs"
	@echo "Options: UVM_TESTNAME=<test> SEED=<n> COV=0|1 WAVE=1 GUI=1 UVM_HOME=<path> VERILATOR=<bin>"

# Fail unless the UVM report summary was printed with zero UVM_ERROR / UVM_FATAL.
# $(1): simulation log file
define check_uvm_log
	@grep -q "UVM Report Summary" $(1) || { echo "FAIL: no UVM report summary in $(1)"; exit 1; }
	@if grep -Eq "^UVM_(ERROR|FATAL) *: *[1-9]" $(1); then echo "FAIL: UVM errors reported, see $(1)"; exit 1; fi
	@echo "PASS: $(1)"
endef

# ==========================================
# Xezim UVM Flow
# ==========================================
UVM_URL ?= https://github.com/chipsalliance/uvm-verilator.git
UVM_BRANCH ?= uvm-2020-3.1-vlt
UVM_ROOT = uvm-src/src
XEZIM ?= xezim

uvm-src:
	@echo "Downloading standard UVM library (branch $(UVM_BRANCH))..."
	git clone -b $(UVM_BRANCH) $(UVM_URL) uvm-src

sim: uvm-src
	@echo "Running xezim UVM flow..."
	@mkdir -p $(LOG_DIR)
	$(XEZIM) --simulate --max-time 10000000 \
		-I$(UVM_ROOT) \
		-D UVM_NO_DPI \
		$(UVM_ROOT)/uvm_pkg.sv \
		$(RTL) \
		$(TB_TOP) 2>&1 | tee $(LOG_DIR)/xezim_$(UVM_TESTNAME).log
	$(call check_uvm_log,$(LOG_DIR)/xezim_$(UVM_TESTNAME).log)

# ==========================================
# Verilator UVM Flow
# ==========================================
# Recipe adapted from https://github.com/MikeCovrado/GettingVerilatorStartedWithUVM
# Needs a recent Verilator (tested with v5.052; distro packages such as
# Ubuntu 24.04's 5.020 are too old) and z3 on PATH (the constraint
# solver behind randomize()). Uses the unmodified Accellera UVM library.
VERILATOR          ?= verilator
VERILATOR_COVERAGE ?= verilator_coverage
UVM_CORE_URL ?= https://github.com/accellera-official/uvm-core.git
UVM_CORE_TAG ?= 2020.3.1
UVM_HOME     ?= uvm-core/src
NJOBS        ?= $(shell nproc 2>/dev/null || echo 4)
SEED         ?= 1
COV          ?= 1

VLT_OBJ_DIR   = obj_dir
VLT_BIN       = $(VLT_OBJ_DIR)/Vtb_top
VLT_STAMP     = $(VLT_OBJ_DIR)/.build_args
COV_DIR       = $(LOG_DIR)/coverage
VLT_WAVE_FILE = ../../waves_verilator_uvm.vcd

VLT_ARGS = --binary -j $(NJOBS) -Wall \
           --top-module tb_top \
           --timescale 1ns/1ps \
           --Mdir $(VLT_OBJ_DIR) \
           verilator.vlt \
           +define+UVM_NO_DPI \
           +define+UVM_REPORT_DISABLE_FILE_LINE \
           +incdir+$(UVM_HOME)
ifeq ($(COV),1)
    VLT_ARGS += --coverage
endif
ifeq ($(WAVE),1)
    VLT_ARGS += --trace +define+WAVE=1 +define+WAVE_FILE=\"$(VLT_WAVE_FILE)\"
endif
VLT_SOURCES = $(UVM_HOME)/uvm_pkg.sv $(RTL) $(TB_TOP)

# Run one UVM test on the Verilator binary and check its log.
# $(1): UVM test name
define run_vlt_test
	./$(VLT_BIN) +UVM_TESTNAME=$(1) +verilator+seed+$(SEED) \
		$(if $(filter 1,$(COV)),+verilator+coverage+file+$(COV_DIR)/$(1).cov) \
		2>&1 | tee $(LOG_DIR)/verilator_$(1).log
	$(call check_uvm_log,$(LOG_DIR)/verilator_$(1).log)

endef

uvm-core/src/uvm_pkg.sv:
	@echo "Downloading Accellera UVM library (tag $(UVM_CORE_TAG))..."
	git clone --depth 1 -b $(UVM_CORE_TAG) $(UVM_CORE_URL) uvm-core

# Rebuild when the Verilator arguments change (e.g. toggling WAVE or COV).
$(VLT_STAMP): FORCE
	@mkdir -p $(VLT_OBJ_DIR)
	@echo '$(VLT_ARGS)' | cmp -s - $@ || echo '$(VLT_ARGS)' > $@

$(VLT_BIN): $(VLT_SOURCES) verilator.vlt $(VLT_STAMP)
	@mkdir -p $(LOG_DIR)
	$(VERILATOR) $(VLT_ARGS) $(VLT_SOURCES) 2>&1 | tee $(LOG_DIR)/verilator_compile.log

verilator_build: $(VLT_BIN)

verilator: $(VLT_BIN)
	@rm -rf $(COV_DIR) && mkdir -p $(COV_DIR)
	$(call run_vlt_test,$(UVM_TESTNAME))
ifeq ($(COV),1)
	@$(MAKE) --no-print-directory verilator_cov
endif
	@if [ "$(GUI)" = "1" ]; then gtkwave $(VLT_WAVE_FILE) & fi

verilator_all: $(VLT_BIN)
	@rm -rf $(COV_DIR) && mkdir -p $(COV_DIR)
	$(foreach t,$(ALL_TESTNAMES),$(call run_vlt_test,$(t)))
ifeq ($(COV),1)
	@$(MAKE) --no-print-directory verilator_cov
endif

verilator_cov:
	$(VERILATOR_COVERAGE) --annotate $(LOG_DIR)/annotated_src \
		--write-info $(LOG_DIR)/coverage.info $(COV_DIR)/*.cov
	@echo "Annotated sources: tests/uvm/$(LOG_DIR)/annotated_src (lines marked %00 are uncovered)"
	@echo "lcov data:         tests/uvm/$(LOG_DIR)/coverage.info (genhtml-compatible)"

# ==========================================
# Clean
# ==========================================
clean:
	rm -rf uvm-src uvm-core $(VLT_OBJ_DIR) $(LOG_DIR)
