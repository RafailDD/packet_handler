yosys -import

# Read Verilog source
read_verilog -sv ../packet_handler.v

# Set top module
hierarchy -top packet_handler

# Perform synthesis
synth -top packet_handler

# Map registers to standard cells
dfflibmap -liberty lib/sky130_fd_sc_hd__tt_025C_1v80.lib

# Map combinatorial logic to standard cells
abc -liberty lib/sky130_fd_sc_hd__tt_025C_1v80.lib

# Clean up
clean

# Print statistics
stat -liberty lib/sky130_fd_sc_hd__tt_025C_1v80.lib

# Write structural netlist
write_verilog -noattr synth_netlist.v
