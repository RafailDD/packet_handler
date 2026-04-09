SIM ?= icarus
TOPLEVEL_LANG ?= verilog

PWD=$(shell pwd)

VERILOG_SOURCES = $(PWD)/packet_handler.v

TOPLEVEL = packet_handler
MODULE = test_packet_handler

include $(shell cocotb-config --makefiles)/Makefile.sim
