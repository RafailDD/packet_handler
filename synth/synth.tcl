yosys -import

# Clock period (ns) for timing-driven mapping, shared with packet_handler.sdc.
if {[info exists ::env(CLK_PERIOD_NS)] && $::env(CLK_PERIOD_NS) ne ""} {
    set clk_period $::env(CLK_PERIOD_NS)
} else {
    set clk_period 5.0
}
set lib lib/sky130_fd_sc_hd__tt_025C_1v80.lib

# Extra Verilog defines, e.g. SYNTH_DEFINES="PH_SKY130_SRAM"
set defines {}
if {[info exists ::env(SYNTH_DEFINES)]} {
    foreach d $::env(SYNTH_DEFINES) { lappend defines "-D$d" }
}

# Read Verilog source
read_verilog -sv {*}$defines ../packet_handler.v

# Set top module
hierarchy -top packet_handler

# Perform synthesis
synth -top packet_handler

# Map registers to standard cells
dfflibmap -liberty $lib

# Map combinatorial logic to standard cells. -D makes ABC timing driven
# (buffering, gate sizing) against the target period in picoseconds; the
# constraint file gives it the input drive strength and output load.
set abc_constr abc.constr
set fh [open $abc_constr w]
puts $fh "set_driving_cell sky130_fd_sc_hd__buf_4"
puts $fh "set_load 0.02"
close $fh
abc -D [expr {int($clk_period * 1000)}] -constr $abc_constr -liberty $lib

# Make the netlist STA friendly: no undriven bits, constants from tie cells,
# no multi-bit nets or assigns between ports.
setundef -zero
splitnets -ports
hilomap -hicell sky130_fd_sc_hd__conb_1 HI -locell sky130_fd_sc_hd__conb_1 LO
insbuf -buf sky130_fd_sc_hd__buf_2 A X

# Clean up
clean

# Print statistics
stat -liberty $lib

# Write structural netlist
write_verilog -noattr synth_netlist.v
