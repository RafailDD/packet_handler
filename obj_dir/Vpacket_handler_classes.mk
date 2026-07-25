# Verilated -*- Makefile -*-
# DESCRIPTION: Verilator output: Make include file with class lists
#
# This file lists generated Verilated files, for including in higher level makefiles.
# See Vpacket_handler.mk for the caller.

### Switches...
# C11 constructs required?  0/1 (always on now)
VM_C11 = 1
# Timing enabled?  0/1
VM_TIMING = 1
# Coverage output mode?  0/1 (from --coverage)
VM_COVERAGE = 0
# Parallel builds?  0/1 (from --output-split)
VM_PARALLEL_BUILDS = 0
# Tracing output mode?  0/1 (from --trace/--trace-fst)
VM_TRACE = 0
# Tracing output mode in VCD format?  0/1 (from --trace)
VM_TRACE_VCD = 0
# Tracing output mode in FST format?  0/1 (from --trace-fst)
VM_TRACE_FST = 0

### Object file lists...
# Generated module classes, fast-path, compile with highest optimization
VM_CLASSES_FAST += \
	Vpacket_handler \
	Vpacket_handler___024root__DepSet_hae044369__0 \
	Vpacket_handler___024root__DepSet_h179d5d56__0 \
	Vpacket_handler___024root__DepSet_h0636c4b7__0 \
	Vpacket_handler___024root__DepSet_h9f8b798f__0 \
	Vpacket_handler___024unit__03a__03apacket_item__Vclpkg__DepSet_haec34836__0 \
	Vpacket_handler___024unit__03a__03apacket_item__Vclpkg__DepSet_h0577bd94__0 \
	Vpacket_handler___024unit__03a__03agenerator__Vclpkg__DepSet_h5f3b1cc0__0 \
	Vpacket_handler___024unit__03a__03agenerator__Vclpkg__DepSet_hb336a804__0 \
	Vpacket_handler___024unit__03a__03agenerator__Vclpkg__DepSet_h9770125e__0 \
	Vpacket_handler___024unit__03a__03adriver__Vclpkg__DepSet_ha81ca6ab__0 \
	Vpacket_handler___024unit__03a__03adriver__Vclpkg__DepSet_h044f67f9__0 \
	Vpacket_handler___024unit__03a__03ascoreboard__Vclpkg__DepSet_hc86e67c7__0 \
	Vpacket_handler___024unit__03a__03ascoreboard__Vclpkg__DepSet_h5271dec6__0 \
	Vpacket_handler___024unit__03a__03ascoreboard__Vclpkg__DepSet_h8a2a5064__0 \
	Vpacket_handler___024unit__03a__03amonitor__Vclpkg__DepSet_hc13be329__0 \
	Vpacket_handler___024unit__03a__03amonitor__Vclpkg__DepSet_h1d702477__0 \
	Vpacket_handler___024unit__03a__03aenv__Vclpkg__DepSet_hb8021c5a__0 \
	Vpacket_handler___024unit__03a__03aenv__Vclpkg__DepSet_h599adef0__0 \
	Vpacket_handler___024unit__03a__03aenv__Vclpkg__DepSet_h94d1504e__0 \
	Vpacket_handler__main \

# Generated module classes, non-fast-path, compile with low/medium optimization
VM_CLASSES_SLOW += \
	Vpacket_handler__ConstPool_0 \
	Vpacket_handler___024root__Slow \
	Vpacket_handler___024root__DepSet_hae044369__0__Slow \
	Vpacket_handler___024root__DepSet_h0636c4b7__0__Slow \
	Vpacket_handler___024unit__Slow \
	Vpacket_handler___024unit__DepSet_h6fd713b2__0__Slow \
	Vpacket_handler___024unit__03a__03apacket_item__Vclpkg__Slow \
	Vpacket_handler___024unit__03a__03apacket_item__Vclpkg__DepSet_h0577bd94__0__Slow \
	Vpacket_handler___024unit__03a__03agenerator__Vclpkg__Slow \
	Vpacket_handler___024unit__03a__03agenerator__Vclpkg__DepSet_h9770125e__0__Slow \
	Vpacket_handler___024unit__03a__03adriver__Vclpkg__Slow \
	Vpacket_handler___024unit__03a__03adriver__Vclpkg__DepSet_h044f67f9__0__Slow \
	Vpacket_handler___024unit__03a__03ascoreboard__Vclpkg__Slow \
	Vpacket_handler___024unit__03a__03ascoreboard__Vclpkg__DepSet_h8a2a5064__0__Slow \
	Vpacket_handler___024unit__03a__03amonitor__Vclpkg__Slow \
	Vpacket_handler___024unit__03a__03amonitor__Vclpkg__DepSet_h1d702477__0__Slow \
	Vpacket_handler___024unit__03a__03aenv__Vclpkg__Slow \
	Vpacket_handler___024unit__03a__03aenv__Vclpkg__DepSet_h94d1504e__0__Slow \

# Generated support classes, fast-path, compile with highest optimization
VM_SUPPORT_FAST += \

# Generated support classes, non-fast-path, compile with low/medium optimization
VM_SUPPORT_SLOW += \
	Vpacket_handler__Syms \

# Global classes, need linked once per executable, fast-path, compile with highest optimization
VM_GLOBAL_FAST += \
	verilated \
	verilated_timing \
	verilated_threads \

# Global classes, need linked once per executable, non-fast-path, compile with low/medium optimization
VM_GLOBAL_SLOW += \


# Verilated -*- Makefile -*-
