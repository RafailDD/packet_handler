# Evaluation: SiliconCompiler vs Bender vs Edalize vs LogikBench

*Evaluated October 2026, hands-on, against this repository: each tool was installed and pointed at `packet_handler.v` and the existing testbenches.*

## Decision

**SiliconCompiler** now drives every flow in the repo (`flow/sc_flow.py`, `make sc ARGS=...`). Its main advantages over the other three:

- **One package covers all our flows.** It's the only candidate whose built-in tasks cover lint (slang, Verilator), simulation (Icarus, Verilator), cocotb on *both* simulators, Yosys synthesis, OpenSTA and full OpenROAD RTL-to-GDS on Sky130. For the two flows it doesn't ship (UVM on Verilator and on xezim), a project task is about 40 lines (`flow/tools/uvm.py`).
- **It uses the same Sky130 SRAM macro as our memory option.** Its PDK package, LambdaPDK, already ships Liberty, LEF and GDS views for that macro, so `--sram` reaches place-and-route without hand-written macro setup.
- **Tardigrade is a one-flag swap.** Zero ASIC's tool integrates through a SiliconCompiler task, and `--syn-tool tardigrade` is already wired in for when the binary is available. See [the Tardigrade evaluation](tardigrade_evaluation.md).
- **The toolchain comes in one pinned container** (`sc_runner:v0.38.10`, 1.6 GB compressed). It has Verilator 5.052, which our UVM flow needs and Ubuntu doesn't package, and it provides the OpenSTA binary that `make sta` falls back to.

The Makefile flows stay as the quick local path, and CI runs both. Keeping both costs little because the RTL is a single file; the SiliconCompiler design class is the canonical file and define list.

## Comparison

| | SiliconCompiler | Bender | Edalize (+ FuseSoC) | LogikBench |
| --- | --- | --- | --- | --- |
| What it is | Python build system: design schema, flowgraphs, tool tasks, PDKs, metrics | Dependency manager + file-list / script generator | Python tool abstraction (EDAM) driving tool backends; FuseSoC adds packaging | Benchmark suite + `lb` CLI on top of SiliconCompiler |
| Runs tools itself | Yes | No, it emits scripts | Yes | Yes, only its own benchmark designs |
| Maturity | 142 PyPI releases since Dec 2021, 0.38.10 (Oct 2026). Apache-2.0. API reworked around 0.34; still pre-1.0 | 0.32.1, Rust binary. Widely used by the PULP platform for large SoCs | Edalize 0.6.8 (34 releases since 2018), FuseSoC 2.4.7 (since 2016). Both "Production/Stable" | 1.0.0, first release June 2026, 2 releases |
| Icarus / Verilator sim | Built in (`dvflow`) | Script only (verilator / flist) | Built in | Benchmarks only |
| cocotb | Built in, Icarus **and** Verilator | No | No (needs hooks or a separate runner) | No |
| UVM | Not built in. A project task wraps `tests/uvm/Makefile` | No | Not for open-source simulators | No |
| xezim | Project task (same wrapper) | No | No | No |
| Lint | slang + Verilator | No | Verilator, Verible | Yes (own designs) |
| ASIC synthesis + STA | Yosys + OpenSTA, Sky130 / GF180 / IHP130 / ASAP7 / FreePDK45 targets | No | Yosys backend is FPGA-oriented (`arch` = xilinx / ice40 / ecp5); no STA | Yosys or Tardigrade + OpenSTA |
| Place & route | OpenROAD, KLayout, Magic / Netgen sign-off | No | OpenLane / ORFS wrapper (external checkout) | Yes (own designs) |
| SRAM macros | LambdaPDK libraries (Liberty / LEF / GDS) | No | Manual | Manual |
| Tardigrade | Via a task (LogikBench's wrapper pattern) | No | No | Built in (`--tool tardigrade`) |
| Flexibility | High: any flowgraph, custom tasks in Python, parallel nodes, Docker / Slurm schedulers | High for file lists and targets, nothing else | High for simulators and FPGAs, low for ASIC | Low: fixed benchmark set |

### What each one did on this repo

- **SiliconCompiler:**
  - Lint, Icarus and Verilator simulation, cocotb on both simulators, UVM on Verilator, and the `SRAM=1` variants all passed in the `sc_runner` container.
  - Synthesis + STA and RTL-to-GDS fetch LambdaPDK data from `codeload.github.com`, which this development session's egress policy blocks, so they were validated in CI instead. All CI jobs passed, including RTL-to-GDS with 0 routing DRCs for both FIFO builds (results in [FIFO memory options](memory_options.md)).
- **Edalize:** `get_edatool("icarus")` ran `tb_basic_sv` unchanged (PASSED). Its Yosys backend only targets FPGA architectures, and its OpenROAD backend expects a separate OpenROAD-flow-scripts checkout. There's no STA step and no cocotb runner.
- **Bender:** `Bender.yml` with `simulation` / `sky130_sram` targets produced correct `flist` and Verilator scripts. It is a file-list and dependency tool only: Make would still run every tool, which is what the repo already does. It's worth adding only if the project starts pulling in IP packages.
- **LogikBench:** `lb syn --target sky130 --tool tardigrade` runs only LogikBench's own benchmark groups (`basic`, `memory`, `epfl`, ...). It can't take an external design, so it can't drive this project. Its Tardigrade wrapper was the template for `flow/tools/tardigrade.py`.

## Costs and risks of SiliconCompiler

- **Pre-1.0 API.** The 0.3x series reworked the Python API (`Design` / `ASIC` / `Sim` projects replaced the single `Chip` object). The image and the CI `pip install` are pinned to 0.38.10; upgrades should be deliberate.
- **Network at run time.** PDK and library data (LambdaPDK) download on first use from GitHub archives. That's fine on GitHub Actions, but environments that block `codeload.github.com` need a pre-populated cache.
- **Large image.** `sc_runner` is 6.4 GB unpacked. CI builds `flow/Dockerfile` on top of it in each job that needs it.
- **Two known image issues, both handled:**
  - The image has no cocotb or z3. `flow/Dockerfile` adds them.
  - Its `verilator_coverage` is misinstalled (`verilator_coverage_bin_dbg` is missing), so the SC UVM flow defaults to `COV=0`. Coverage still comes from the `verilator-uvm` CI job, which uses the official Verilator image.

## How to use it

```bash
make sc_image                                    # once: build packet-handler-eda
make sc ARGS="lint"
make sc ARGS="sim --tool verilator --tb tb_uvm_sv"
make sc ARGS="cocotb --tool verilator --sram"
make sc ARGS="uvm --tool verilator"              # or --tool xezim (xezim on PATH)
make sc ARGS="syn --clk 8"                       # Yosys + OpenSTA, all Sky130 corners
make sc ARGS="asic --sram"                       # RTL-to-GDS with the SRAM macro
make sc ARGS="syn --syn-tool tardigrade"         # needs the tardigrade binary
SC_LOCAL=1 make sc ARGS="..."                    # local python + tools on PATH
```

Results go to `build/packet_handler/<job>/`, one directory per node, with logs, reports, outputs and a `replay.sh`.
