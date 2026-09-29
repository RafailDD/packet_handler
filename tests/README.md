# Verification Environment

This repository provides a unified Makefile to manage several simulation flows for the `packet_handler` module. Each flow serves a different purpose, and every testbench file has a specific role. All commands below are run from the repository root. Tool installation is summarized in the [top-level README](../README.md#prerequisites).

The UVM tests are written entirely in SystemVerilog (IEEE 1800.2). Icarus Verilog cannot run UVM, and until recently neither could Verilator. Recent Verilator releases (v5.052) and the xezim simulator can now run the standard UVM library.

To cover all of this, we provide multiple pathways in the `tests/` directory:

## Testbench Files Explained

| File | Language | Runs on | Self-checking? |
| ---- | -------- | ------- | -------------- |
| `tb_cocotb.py` | Python (cocotb) | Icarus (`make cocotb`) | Yes, assertions per test |
| `tb_smoke.v` | Verilog | Icarus, Verilator (`TB=tb_smoke`) | No, visual inspection of `$monitor` output / waveforms |
| `tb_basic_sv.sv` | SystemVerilog | Icarus (default), Verilator (`TB=tb_basic_sv`) | No, see below |
| `tb_uvm_sv.sv` | SystemVerilog classes | Verilator (default). Does **not** compile on Icarus (queues of class handles are unsupported) | No, see below |
| `uvm/tb_top.sv` | SystemVerilog + UVM | xezim, Verilator v5.052+ | Partly, see [uvm/README.md](uvm/README.md) |

*   **`tb_smoke.v`**: A legacy Verilog testbench used for basic "smoke testing" and visual waveform inspection. It sends 3 packets on stream 15 with seqNumbers 1, 2 and 4, so the third packet raises `o_packetLost`. The file has no `` `timescale ``, so Icarus reports times in seconds.
*   **`tb_basic_sv.sv`**: A basic, standalone SystemVerilog testbench using tasks and dynamic arrays, avoiding class features that Icarus Verilog does not support. It sends 5 in-order packets and one with a skipped seqNumber, and prints each received payload and each `o_packetLost` pulse. It does not compare data, and `error_count` is never incremented, so it always prints `STATUS: PASSED`.
*   **`tb_uvm_sv.sv`**: A pure SystemVerilog class-based environment (similar to UVM architecture: generator, driver, monitor, scoreboard, and coverage counters) that does not need the UVM library. It sends 100 random packets and prints a small coverage report. The scoreboard computes the expected payload, but the comparison result is discarded (the mismatch message is commented out), so it always prints `TEST PASSED`.
*   **`tb_cocotb.py`**: A Python-based test suite using cocotb, with basic, edge-case and randomized packets (see the test list below).
*   **`uvm/` Directory**: Contains the complete **SystemVerilog UVM environment** (driver, monitor, sequencer, agent, scoreboard, sequences, and coverage covergroups). It runs on two open-source simulators, xezim and Verilator (flows 4 and 5 below), and should also run unchanged on commercial simulators (VCS, Questa, Xcelium). See [uvm/README.md](uvm/README.md).
*   **`benchmark.py`**: Not a testbench. `make benchmark` times two ways of packing 32-bit words into one integer, the helper used to build expected `o_data` values in `tb_cocotb.py`.

## Running Tests
You can view the available targets and options by running `make` with no arguments. (With cocotb installed, `make help` prints cocotb's help instead of the project's.)

### 1. Cocotb Flow
- **Command:** `make cocotb`
- **Description:** Runs the `tb_cocotb.py` test suite on Icarus Verilog through cocotb's Makefile flow. The target fails if any test fails. Results are also written to `results.xml` (JUnit format).
- **Run a subset:** `make cocotb COCOTB_TEST_FILTER=test_mid_packet_reset` (a regular expression matched against test names).
- **Dependencies:** Icarus Verilog (`iverilog`), Python 3, `cocotb` 2.x (`pip install cocotb`; the tests use the cocotb 2.0 API, e.g. `Timer(..., unit="ns")`). CI also installs `pytest`, but the Makefile flow does not use it.

Registered tests (run in this order):

| Test | Checks |
| ---- | ------ |
| `test_reset` | `o_ready=1`, `o_valid=0`, `o_packetLost=0` after reset |
| `test_basic_packet` | One 4-word packet, `o_data` matches |
| `test_multiple_valid_streams` | Interleaved streams 1 and 2 with in-order seqNumbers, no `o_packetLost` |
| `test_dropped_lost_packet` | seqNumber 1 then 3 on stream 5 raises `o_packetLost` |
| `test_min_length_packet` | msgLength 9 (1 data word) |
| `test_randomized_packets` | 50 random packets (1-8 words, random streams), data and no false `o_packetLost` |
| `test_receiver_not_ready` | `o_valid` / `o_data` held stable while `i_ready` is low, second packet delivered after |
| `test_mid_packet_reset` | Reset in the middle of a packet, recovery and next packet |

> [!NOTE]
> `tb_cocotb.py` also defines `test_fifo_full_backpressure` and `test_max_length_packet`, but they are missing the `@cocotb.test()` decorator, so cocotb does not run them.

### 2. Icarus Verilog Flow
- **Command:** `make icarus [TB=<testbench>]`
- **Description:** Compiles `packet_handler.v` plus `tests/<TB>.v` / `tests/<TB>.sv` with `iverilog -g2012` and runs it with `vvp`. Defaults to `TB=tb_basic_sv`. `TB=tb_smoke` also works; `tb_uvm_sv` does not compile on Icarus.
- **Dependencies:** Icarus Verilog (`iverilog`).

### 3. Verilator Flow
- **Command:** `make verilator [TB=<testbench>]`
- **Description:** Compiles the RTL and the testbench with `verilator --binary -Wall` into `obj_dir/Vpacket_handler` and runs it. Defaults to `TB=tb_uvm_sv`; `tb_basic_sv` and `tb_smoke` also work.
- **Dependencies:** Verilator (`verilator`). The Ubuntu 24.04 package (5.020) is enough for this flow.

> [!WARNING]
> In the `icarus` and `verilator` targets the compiler and simulator output is piped through `tee`, so the target returns success even when compilation fails. Check the log. With a `TB` name that does not exist, `make verilator` builds the DUT on its own and the resulting binary never finishes (stop it with Ctrl-C).

### 4. Xezim UVM Flow
- **Command:** `make xezim_uvm`
- **Description:** Runs the standard UVM testbench in `uvm/` on the xezim simulator.
- **Dependencies:** xezim (built from source with Rust/cargo), `git`.

### 5. Verilator UVM Flow
- **Command:** `make verilator_uvm [UVM_TESTNAME=<test>] [SEED=<n>] [COV=0]`
- **Description:** Runs the same UVM testbench on Verilator with the unmodified Accellera UVM library, and produces code coverage (`tests/uvm/logs/annotated_src/`, `tests/uvm/logs/coverage.info`). `make verilator_uvm_all` runs every test in `ALL_TESTNAMES`. The first build takes about 2 minutes.
- **Dependencies:** Verilator **v5.052** or newer (the Ubuntu apt package is too old; build from source or use the `verilator/verilator:v5.052` container), `z3`, `git`.

Both UVM flows fail the `make` target if the UVM report summary counts any `UVM_ERROR` or `UVM_FATAL`. Setup, options and known limitations are covered in [uvm/README.md](uvm/README.md).

## Waveforms & Logs

Append `WAVE=1` to a simulation target to dump waveforms. `GUI=1` implies `WAVE=1` and also opens the result in GTKWave when the run finishes.

| Flow | `WAVE=1` output | Log |
| ---- | --------------- | --- |
| `make cocotb` | `sim_build/packet_handler.fst` (FST, opens in GTKWave). The Makefile tries to copy a VCD to `waves_cocotb.vcd`, but cocotb writes FST, so that file is not created and `GUI=1` has nothing to open | console, `results.xml` |
| `make icarus TB=<tb>` | `waves_icarus_<tb>.vcd` | `tests/logs/sim_icarus_<tb>.log` |
| `make verilator TB=<tb>` | `waves_verilator_<tb>.vcd` | `tests/logs/sim_verilator_<tb>.log` |
| `make verilator_uvm` | `waves_verilator_uvm.vcd` (DUT scope only) | `tests/uvm/logs/verilator_<test>.log` |
| `make xezim_uvm` | not supported | `tests/uvm/logs/xezim_packet_test.log` |

```bash
make icarus TB=tb_smoke WAVE=1
make verilator GUI=1
gtkwave sim_build/packet_handler.fst   # after make cocotb WAVE=1
```

`make clean` removes the waveforms and build outputs; logs in `tests/logs/` are kept.

---

# Architecture of `tb_uvm_sv.sv`

This project attempts to provide a pure SystemVerilog Object-Oriented Programming (OOP) verification environment, heavily inspired by the Universal Verification Methodology (UVM) standard.

True IEEE 1800.2 UVM is the industry standard for verifying complex hardware designs. However, open-source simulators often struggle to compile the massive UVM base class library. To bridge this gap, this project provides a custom, "UVM-style" environment written from scratch in pure SystemVerilog. It utilizes the same architectural concepts (Generators, Drivers, Monitors, Scoreboards) but avoids the heavyweight standard library dependencies.

- **packet_item:** The transaction class. It contains the data payload, header fields, and logic to randomize itself.
- **generator:** Creates sequences of `packet_item` objects and stores them in a queue to be processed.
- **driver:** Only counts sent packets. The pin-level driving is done by the `send_packet` task in the `tb_verilator_uvm` module.
- **monitor:** Holds the coverage counters (packet loss, 1-word and 9-word packets) and prints the coverage report.
- **scoreboard:** Pops the expected item for each output transaction (`o_valid && i_ready`) and rebuilds the expected payload. The comparison is currently not reported (see above).
- **env:** The top-level container that instantiates all the above components.
