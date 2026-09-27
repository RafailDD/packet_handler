# Verification Environment

This repository provides a unified Makefile to manage several simulation flows, enabling robust verification of the `packet_handler` module. Each flow serves a different purpose, and every testbench file has a specific role.

The UVM tests are written entirely in SystemVerilog (IEEE 1800.2). Icarus Verilog cannot run UVM, and until recently neither could Verilator. Recent Verilator releases (v5.052) and the xezim simulator can now run the standard UVM library.

To solve this, we provide multiple pathways in the `tests/` directory:

## Testbench Files Explained

*   **`tb_smoke.v`**: A legacy Verilog testbench used for basic "smoke testing" and visual waveform inspection. Provides very basic stimulus.
*   **`tb_basic_sv.sv`**: A basic, standalone SystemVerilog testbench. It uses basic procedures and arrays to verify logic, bypassing advanced class features that are unsupported in Icarus Verilog.
*   **`tb_uvm_sv.sv`**: A highly structured, pure SystemVerilog class-based environment (similar to UVM architecture: generator, driver, monitor, scoreboard, and coverage). Built to be compiled and run with Verilator for fast execution and functional coverage.
*   **`tb_cocotb.py`**: A Python-based testing environment using Cocotb. It uses randomized, edge-case, and basic packets to stress test the RTL.
*   **`uvm/` Directory**: Contains the complete **SystemVerilog UVM environment** (driver, monitor, sequencer, agent, scoreboard, sequences, and coverage covergroups). It runs on two open-source simulators, xezim and Verilator (flows 4 and 5 below), and should also run unchanged on commercial simulators (VCS, Questa, Xcelium). See [uvm/README.md](uvm/README.md).

## Running Tests
You can view the available targets and options by running:
```bash
make help
```

### 1. Cocotb Flow
- **Command:** `make cocotb`
- **Description:** Runs the `tb_cocotb.py` test suite. Great for agile, Python-scripted scenarios.
- **Dependencies:** Icarus Verilog (`iverilog`), Python 3, `cocotb`, and `pytest`.

### 2. Icarus Verilog Flow
- **Command:** `make icarus [TB=<testbench>]`
- **Description:** Compiles and runs the specified testbench using the standard, event-driven Icarus simulator. Defaults to `TB=tb_basic_sv` if not specified.
- **Dependencies:** Icarus Verilog (`iverilog`).

### 3. Verilator Flow
- **Command:** `make verilator [TB=<testbench>]`
- **Description:** Uses Verilator to compile the specified testbench into C++ for extremely fast simulation. Defaults to `TB=tb_uvm_sv` if not specified.
- **Dependencies:** Verilator (`verilator`).

### 4. Xezim UVM Flow
- **Command:** `make xezim_uvm`
- **Description:** Runs the standard UVM testbench in `uvm/` on the xezim simulator.
- **Dependencies:** xezim (built from source with Rust/cargo), `git`.

### 5. Verilator UVM Flow
- **Command:** `make verilator_uvm [UVM_TESTNAME=<test>] [SEED=<n>] [COV=0]`
- **Description:** Runs the same UVM testbench on Verilator with the unmodified Accellera UVM library, and produces code coverage (`tests/uvm/logs/annotated_src/`, `tests/uvm/logs/coverage.info`). `make verilator_uvm_all` runs every test in `ALL_TESTNAMES`. The first build takes about 2 minutes.
- **Dependencies:** Verilator **v5.052** or newer (the Ubuntu apt package is too old; build from source or use the `verilator/verilator:v5.052` container), `z3`, `git`.

Both UVM flows fail the `make` target if the UVM report summary counts any `UVM_ERROR` or `UVM_FATAL`. Setup and options are covered in [uvm/README.md](uvm/README.md).

## Waveforms & Logs

All flows support waveform generation by appending `WAVE=1` to the make command. The output format is standard `.vcd`, and the output files will be named predictively (e.g., `waves_icarus_tb_smoke.vcd`).

You can also pass `GUI=1` to automatically open the generated waveform in GTKWave upon test completion. `GUI=1` automatically implies `WAVE=1`.

```bash
make icarus TB=tb_smoke WAVE=1
make verilator GUI=1
```

Simulation execution logs will automatically be saved inside `tests/logs/` (UVM flows: `tests/uvm/logs/`). For `verilator_uvm`, `WAVE=1` writes `waves_verilator_uvm.vcd`. The xezim flow does not generate waveforms.

---

# Architecture of `tb_uvm_sv.sv`

This project attempts to provide a pure SystemVerilog Object-Oriented Programming (OOP) verification environment, heavily inspired by the Universal Verification Methodology (UVM) standard.

True IEEE 1800.2 UVM is the industry standard for verifying complex hardware designs. However, open-source simulators often struggle to compile the massive UVM base class library. To bridge this gap, this project provides a custom, "UVM-style" environment written from scratch in pure SystemVerilog. It utilizes the same architectural concepts (Generators, Drivers, Monitors, Scoreboards) but avoids the heavyweight standard library dependencies.

- **packet_item:** The transaction class. It contains the data payload, header fields, and logic to randomize itself.
- **generator:** Creates sequences of `packet_item` objects and stores them in a queue to be processed.
- **driver:** Fetches items from the generator queue and drives the physical pin-level signaling to the Device Under Test (DUT).
- **monitor:** Passively observes the DUT outputs, samples functional coverage, and constructs reconstructed items.
- **scoreboard:** Compares the reconstructed items from the monitor against the expected items sent by the generator to verify functional correctness.
- **env:** The top-level container that instantiates and connects all the above components.
