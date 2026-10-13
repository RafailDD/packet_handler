# OpenSTA timing report for the Yosys netlist (run from synth/ by `make sta`).
set lib    [expr {[info exists ::env(LIB_FILE)] ? $::env(LIB_FILE) : "lib/sky130_fd_sc_hd__tt_025C_1v80.lib"}]
set netlst [expr {[info exists ::env(NETLIST)]  ? $::env(NETLIST)  : "synth_netlist.v"}]

read_liberty $lib
read_verilog $netlst
link_design packet_handler
read_sdc packet_handler.sdc

report_checks -path_delay max -fields {slew cap input_pins} -digits 3
report_checks -path_delay min -digits 3
report_tns -digits 3
report_wns -digits 3
report_worst_slack -max -digits 3
report_clock_min_period
report_check_types -max_slew -max_capacitance -max_fanout -violators
