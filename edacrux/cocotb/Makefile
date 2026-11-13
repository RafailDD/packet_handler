# cocotb Makefile for the SimCrux `cocotb` suite (edacrux/simcrux.yaml).
#
# SimCrux copies this file and the test's `sources` (flat, by basename) into a
# per-test work directory and runs `make` there with SIM, TOPLEVEL and
# TOPLEVEL_LANG set (MODULE / COCOTB_TEST_MODULES from the test's
# `parameters.MODULE`). It does not pass VERILOG_SOURCES, so it is built here
# from the copied files. Not meant to be run from edacrux/cocotb/ directly:
# use `make cocotb` at the repository root for that.

SIM                 ?= icarus
TOPLEVEL_LANG       ?= verilog
TOPLEVEL            ?= packet_handler
COCOTB_TOPLEVEL     ?= $(TOPLEVEL)
COCOTB_TEST_MODULES ?= tb_cocotb

VERILOG_SOURCES = $(CURDIR)/packet_handler.v

# SRAM build: the test lists macros/sky130_sram_model.v in its sources.
ifneq ($(wildcard $(CURDIR)/sky130_sram_model.v),)
    VERILOG_SOURCES += $(CURDIR)/sky130_sram_model.v
    COMPILE_ARGS    += -DPH_SKY130_SRAM
endif

# tb_cocotb.py is copied next to this Makefile.
export PYTHONPATH := $(CURDIR):$(PYTHONPATH)

# Waveforms (FST) for Icarus, which SimCrux picks up from sim_build/.
# Verilator FST tracing needs zlib headers; left off (see edacrux/README.md).
ifeq ($(SIM),icarus)
    export WAVES ?= 1
endif

include $(shell cocotb-config --makefiles)/Makefile.sim
