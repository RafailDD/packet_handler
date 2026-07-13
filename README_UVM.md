# Verification Environment

The UVM verification environment is provided in the `uvm/` directory.

The UVM tests are written entirely in SystemVerilog (IEEE 1800.2). However, open source simulators (like Verilator or Icarus Verilog) do not fully support the complete UVM standard for executing simulation yet.

To solve this, I have provided two test environments:
1. `uvm/tb_top.sv` and `uvm/Makefile`: Contains the complete **SystemVerilog UVM environment** (driver, monitor, sequencer, agent, scoreboard, sequences, and coverage covergroups) capable of exhaustive testing. Run this with a commercial simulator (VCS, Questa, Xcelium).
2. `tb_icarus_sv.sv` and the `icarus` make target: Contains a **stand-alone SystemVerilog testbench** that verifies the logic by sending scenarios and catching data loss conditions, providing a runnable open-source testing pathway on Icarus Verilog.
3. `tb_verilator_uvm.sv` and the `verilator-uvm` make target: Contains a pure SV **UVM-style testbench** (generator, driver, monitor, scoreboard) that is compiled with Verilator for fast execution.

Run the SV fallback verification:
```
make icarus TB=tb_icarus_sv
```

Run the Verilator UVM-style verification:
```
make verilator-uvm TB=tb_verilator_uvm
```
