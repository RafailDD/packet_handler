# Timing constraints for packet_handler (Sky130 HD).
#
# Read by OpenSTA (synth/sta.tcl) and by SiliconCompiler / OpenROAD. Override
# the clock period from the environment with CLK_PERIOD_NS (nanoseconds).

if {[info exists ::env(CLK_PERIOD_NS)] && $::env(CLK_PERIOD_NS) ne ""} {
    set clk_period $::env(CLK_PERIOD_NS)
} else {
    set clk_period 5.0
}

# Fraction of the period consumed outside the block on each I/O path.
set io_budget 0.2

create_clock -name clk -period $clk_period [get_ports i_clk]
set_clock_uncertainty 0.1 [get_clocks clk]
set_clock_transition  0.15 [get_clocks clk]

set non_clk_inputs [lsearch -inline -all -not -exact [all_inputs] [get_ports i_clk]]
set_input_delay  [expr {$clk_period * $io_budget}] -clock clk $non_clk_inputs
set_output_delay [expr {$clk_period * $io_budget}] -clock clk [all_outputs]

set_driving_cell -lib_cell sky130_fd_sc_hd__buf_4 -pin X $non_clk_inputs
set_load 0.02 [all_outputs]

# i_rst_n is asynchronously asserted and is expected to be synchronized
# (deassertion) by the integrator, so it is not timed here.
set_false_path -from [get_ports i_rst_n]
