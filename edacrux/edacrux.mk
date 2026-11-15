# EDACrux targets (LintCrux, SimCrux, WaveCrux, NetCrux), included by the
# top-level Makefile. Run `make edacrux_help` for the list. Everything the
# tools read lives in edacrux/; generated outputs go to edacrux/build/ and
# edacrux/waves/ (both git-ignored). See edacrux/README.md.

EDACRUX_DIR   := edacrux
EDACRUX_BUILD := $(EDACRUX_DIR)/build
EDACRUX_WAVES := $(EDACRUX_DIR)/waves

# Machine-specific settings (tool paths), git-ignored. See edacrux/README.md.
-include $(EDACRUX_DIR)/local.mk

# CLIs and desktop apps. Override if they are not on PATH under these names.
LINTCRUX     ?= lintcrux
SIMCRUX      ?= simcrux
LINTCRUX_APP ?= lintcrux-app
SIMCRUX_APP  ?= simcrux-app
WAVECRUX_APP ?= wavecrux
NETCRUX_APP  ?= netcrux

# NoMachine sessions preload libnxegl.so, which breaks OSS CAD Suite's bundled
# loader and the EDACrux apps' native libraries. Dropping it is harmless elsewhere.
EDACRUX_ENV = env -u LD_PRELOAD

# Verilator used by SimCrux. The `uvm` suite needs >= 5.052 (covergroups,
# stock Accellera UVM); point this at a newer binary when the one on PATH is
# older. Only verilator is redirected (via a symlink in edacrux/build/bin), so
# iverilog / cocotb keep coming from PATH.
SIMCRUX_VERILATOR ?=
SIMCRUX_BIN       := $(abspath $(EDACRUX_BUILD))/bin
SIMCRUX_PATH      := $(if $(SIMCRUX_VERILATOR),$(SIMCRUX_BIN):)$(PATH)

# Extra SimCrux CLI options, e.g. SIMCRUX_FILTER=icarus/ (substring of
# <suite>/<test>), SIMCRUX_ARGS="--max-parallel 2".
SIMCRUX_FILTER ?=
SIMCRUX_ARGS   ?=

# `make lintcrux SRAM=1` lints the Sky130 SRAM build (lint_sram.json overlay).
# `make lintcrux LINT_GATE=1` fails on any finding (LintCrux --exit-code).
LINT_GATE ?= 0
LINTCRUX_OPTS = $(if $(filter 1,$(SRAM)),-c lint_sram.json) $(if $(filter 1,$(LINT_GATE)),--exit-code)

# Waveform opened by `make wavecrux` (default: the design manifest, which
# opens edacrux/waves/icarus_tb_smoke.fst after `make simcrux`).
WAVE_FILE_CRUX ?= $(EDACRUX_DIR)/packet_handler.crux-project

.PHONY: edacrux_help lintcrux lintcrux_gui simcrux simcrux_gui simcrux_uvm_deps \
        simcrux_verilator wavecrux netcrux edacrux_clean

edacrux_help:
	@echo "EDACrux targets (see edacrux/README.md):"
	@echo "  make lintcrux [SRAM=1] [LINT_GATE=1] : LintCrux headless lint -> edacrux/build/lintcrux.{sarif,html}"
	@echo "  make lintcrux_gui                    : open the project in the LintCrux app"
	@echo "  make simcrux [SIMCRUX_FILTER=..]     : SimCrux regression -> edacrux/build/ (+ waveforms in edacrux/waves/)"
	@echo "  make simcrux_gui                     : open edacrux/simcrux.yaml in the SimCrux app"
	@echo "  make wavecrux [WAVE_FILE_CRUX=..]    : open a waveform (default: design manifest) in WaveCrux"
	@echo "  make netcrux                         : open the schematic in NetCrux"
	@echo "  make edacrux_clean                   : remove edacrux/build, edacrux/waves and SimCrux run history"
	@echo "Options: SIMCRUX_VERILATOR=<verilator >= 5.052>  SIMCRUX_ARGS=\"...\"  LINTCRUX=/SIMCRUX=<cli path>"

# ---------------------------------------------------------------- LintCrux
lintcrux:
	@mkdir -p $(EDACRUX_BUILD)
	cd $(EDACRUX_DIR) && $(EDACRUX_ENV) $(LINTCRUX) packet_handler.lintcrux $(LINTCRUX_OPTS) \
		--sarif build/lintcrux.sarif --quiet
	cd $(EDACRUX_DIR) && $(EDACRUX_ENV) $(LINTCRUX) packet_handler.lintcrux \
		$(if $(filter 1,$(SRAM)),-c lint_sram.json) --export html --out build/lintcrux.html --quiet
	@echo "SARIF: $(EDACRUX_BUILD)/lintcrux.sarif (VS Code: edacrux.lint.resultsPath)  HTML: $(EDACRUX_BUILD)/lintcrux.html"

lintcrux_gui:
	cd $(EDACRUX_DIR) && $(EDACRUX_ENV) $(LINTCRUX_APP) packet_handler.lintcrux >/dev/null 2>&1 &

# ---------------------------------------------------------------- SimCrux
# The uvm suite compiles the Accellera uvm-core checkout of tests/uvm.
simcrux_uvm_deps:
	@$(MAKE) --no-print-directory -C tests/uvm uvm-core/src/uvm_pkg.sv

simcrux_verilator:
ifneq ($(SIMCRUX_VERILATOR),)
	@mkdir -p $(SIMCRUX_BIN)
	@ln -sf $(abspath $(SIMCRUX_VERILATOR)) $(SIMCRUX_BIN)/verilator
endif
	@echo "SimCrux verilator: $$(PATH='$(SIMCRUX_PATH)' command -v verilator) ($$(PATH='$(SIMCRUX_PATH)' $(EDACRUX_ENV) verilator --version 2>/dev/null))"

simcrux: simcrux_uvm_deps simcrux_verilator
	@mkdir -p $(EDACRUX_BUILD)
	cd $(EDACRUX_DIR) && $(EDACRUX_ENV) PATH='$(SIMCRUX_PATH)' $(SIMCRUX) --ci simcrux.yaml \
		$(if $(SIMCRUX_FILTER),--filter $(SIMCRUX_FILTER)) $(SIMCRUX_ARGS) \
		--export junit=build/simcrux_junit.xml --export html=build/simcrux_report.html; \
		rc=$$?; python3 collect_waves.py build/results.ndjson waves; exit $$rc

simcrux_gui: simcrux_uvm_deps simcrux_verilator
	cd $(EDACRUX_DIR) && $(EDACRUX_ENV) PATH='$(SIMCRUX_PATH)' $(SIMCRUX_APP) simcrux.yaml >/dev/null 2>&1 &

# ---------------------------------------------------------------- WaveCrux / NetCrux
wavecrux:
	$(EDACRUX_ENV) $(WAVECRUX_APP) $(WAVE_FILE_CRUX) >/dev/null 2>&1 &

netcrux:
	$(EDACRUX_ENV) $(NETCRUX_APP) $(EDACRUX_DIR)/packet_handler.netcrux-project >/dev/null 2>&1 &

edacrux_clean:
	rm -rf $(EDACRUX_BUILD) $(EDACRUX_WAVES) $(EDACRUX_DIR)/results.summary.json $(EDACRUX_DIR)/.simcrux

clean:: edacrux_clean
