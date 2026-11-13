# EDACrux Flow

This directory connects the project's existing flows to the [EDACrux](https://edacrux.app/) suite by Ferrite Engineering:

| Tool | Role here | Make target |
| ---- | --------- | ----------- |
| **LintCrux** | One lint report from Verilator, Verible, slang, Yosys, svlint and the CDC check | `make lintcrux`, `make lintcrux_gui` |
| **SimCrux** | Every simulation flow (Icarus, Verilator, cocotb, UVM) as one regression tree | `make simcrux`, `make simcrux_gui` |
| **WaveCrux** | Waveform viewer for the regression's waveforms | `make wavecrux` |
| **NetCrux** | Schematic / netlist browser for `packet_handler.v` | `make netcrux` |

The existing `make icarus`, `make verilator`, `make cocotb`, `make verilator_uvm`, `make synth` flows are unchanged. EDACrux is an additional front end. It runs the same testbenches and RTL from their original locations, and nothing here is a copy.

## Files

| File | Read by | Purpose |
| ---- | ------- | ------- |
| `packet_handler.crux-project` | all four tools | Design manifest: top, sources, and the lint, simulation and waveform artifacts. `wavecrux edacrux/` and `netcrux edacrux/` open it |
| `packet_handler.lintcrux` | LintCrux | Lint project: `packet_handler.v`, top `packet_handler`, the six engines |
| `lint_sram.json` | LintCrux (`-c`) | Overlay for the Sky130 SRAM build: adds `macros/sky130_sram_model.v` and defines `PH_SKY130_SRAM` (`make lintcrux SRAM=1`) |
| `.svlint.toml` | svlint (through LintCrux) | Rules for svlint. Without it, svlint enables every rule and reports ~250 SystemVerilog style findings on this Verilog-2001 file |
| `simcrux.yaml` | SimCrux | Regression: 4 suites, 10 tests, with pass/fail detectors (see [What each test proves](#what-each-test-proves)) |
| `cocotb/Makefile` | SimCrux `cocotb` suite | cocotb Makefile for SimCrux's per-test work directory |
| `collect_waves.py` | `make simcrux` | Copies each test's waveform from SimCrux's temporary work directory to `edacrux/waves/<suite>_<test>.fst` |
| `packet_handler.netcrux-project` | NetCrux | Schematic project: `packet_handler.v`, top `packet_handler` |
| `edacrux.mk` | top-level `Makefile` | The make targets below |
| `local.mk` (git-ignored, optional) | `edacrux.mk` | Machine-specific settings, e.g. `SIMCRUX_VERILATOR` |

Generated: `build/` (SARIF / HTML lint reports, `results.ndjson`, JUnit and HTML regression reports) and `waves/`. SimCrux also writes `results.summary.json` and `.simcrux/trends.db` (run history) next to `simcrux.yaml`. All of these are git-ignored, and `make clean` or `make edacrux_clean` removes them.

## Prerequisites

- **The EDACrux suite:** WaveCrux, NetCrux, LintCrux and SimCrux desktop apps, plus the `lintcrux` and `simcrux` CLIs. The Makefile calls them as `lintcrux`, `simcrux`, `lintcrux-app`, `simcrux-app`, `wavecrux` and `netcrux`. Override with `LINTCRUX=...`, `SIMCRUX=...`, `LINTCRUX_APP=...` and so on if yours are named differently.
- **Lint engines on PATH:** `verilator`, `verible-verilog-lint`, `slang`, `yosys` and `svlint` ([OSS CAD Suite](https://github.com/YosysHQ/oss-cad-suite-build) has all but svlint).
- **For `make simcrux`:** the same tools as the flows it runs. That means Icarus Verilog, Verilator, and Python 3 with cocotb. The `uvm` suite also needs **Verilator >= 5.052** and `z3`, as in [tests/uvm/README.md](../tests/uvm/README.md#installing-verilator). If the `verilator` on PATH is older, point `SIMCRUX_VERILATOR` at a newer binary. Only `verilator` is redirected:
  ```bash
  make simcrux SIMCRUX_VERILATOR=$HOME/opt/oss-cad-suite/bin/verilator
  # or once, in edacrux/local.mk:
  #   SIMCRUX_VERILATOR = $(HOME)/opt/oss-cad-suite/bin/verilator
  ```
- **cocotb and Icarus must be built for the same Python.** Use either the pip `cocotb` with a distro `iverilog`, or OSS CAD Suite's `iverilog` with OSS CAD Suite's bundled cocotb. Mixing the two fails at start-up with `No GPI_USERS specified` (see [Known issues](#known-issues)).

All targets run the tools with `LD_PRELOAD` unset. NoMachine remote sessions preload `libnxegl.so`, which breaks OSS CAD Suite binaries and the EDACrux apps' native libraries.

## Make targets

| Command | What it does | Outputs |
| ------- | ------------ | ------- |
| `make edacrux_help` | List these targets | - |
| `make lintcrux [SRAM=1] [LINT_GATE=1]` | Headless LintCrux run. `SRAM=1` lints the SRAM build. `LINT_GATE=1` fails on any finding | `edacrux/build/lintcrux.sarif`, `edacrux/build/lintcrux.html` |
| `make lintcrux_gui` | Open `packet_handler.lintcrux` in the LintCrux app | - |
| `make simcrux [SIMCRUX_FILTER=<suite/test>] [SIMCRUX_VERILATOR=..] [SIMCRUX_ARGS=..]` | Headless SimCrux regression (`simcrux --ci`). Fetches `tests/uvm/uvm-core` first, then copies the waveforms. Fails if any test fails | `edacrux/build/results.ndjson`, `simcrux_junit.xml`, `simcrux_report.html`, `edacrux/waves/*.fst` |
| `make simcrux_gui` | Open `simcrux.yaml` in the SimCrux app, with the same PATH as `make simcrux` | - |
| `make wavecrux [WAVE_FILE_CRUX=<file>]` | Open WaveCrux on the design manifest, which shows `waves/icarus_tb_smoke.fst`, or on any waveform file | - |
| `make netcrux` | Open the schematic in NetCrux | - |
| `make edacrux_clean` | Remove `edacrux/build/` and `edacrux/waves/` | - |

`SIMCRUX_FILTER` is a plain substring of the test id `<suite>/<test>`. For example, `icarus/` runs one suite and `uvm/` runs only the UVM test.

## LintCrux

`make lintcrux` runs all six engines on `packet_handler.v` and writes SARIF and HTML. The SARIF paths are relative to the repository root. The VS Code LintCrux extension shows them as Problems-panel diagnostics once `edacrux.lint.resultsPath` is set to `edacrux/build/lintcrux.sarif`.

Current results (LintCrux 1.1.0):

| Engine | Findings | Notes |
| ------ | -------- | ----- |
| verilator (`-Wall`) | 0 | Same check as CI's `verilator --lint-only -Wall` |
| slang | 0 | |
| yosys | 0 | 1 with `SRAM=1`: `memory-replaced` on the macro model's array (simulation-only file) |
| cdc | 2 notes | One clock domain (`i_clk`), no crossings |
| svlint | 0 | With `.svlint.toml`. Without it: ~246 style findings |
| verible | 15 | Style: localparam naming (`IDLE`, `IN_W1`, ...), explicit parameter types, `[0:N-1]` unpacked ranges |

The run is "clean" (exit 0) because no gate is requested. `LINT_GATE=1` turns any finding into a failure. Waivers and baselines (`.lintcrux-waivers.json`, `.lintcrux-baseline.json`) are LintCrux Pro features and are not used here.

**svlint rules.** `.svlint.toml` enables only rules that point at RTL mistakes, and that make sense for Verilog-2001:
- `case_default`, `explicit_case_default`;
- blocking assignment in an edge-triggered `always`, non-blocking assignment in a combinational `always`;
- `begin`/`end` on multi-line `if`/`for`;
- `loop_variable_declaration`, `operator_case_equality`.

The SystemVerilog-only rules (`always_ff`, `logic`, `var`, ...) and the template naming regexes are off. svlint finds the file by searching upwards from its working directory, which LintCrux sets to this folder. A relative `perEngineOptions.svlint.configPath` is not resolved against the project, so it is not used.

## SimCrux

`simcrux.yaml` turns the simulation flows into one regression of 4 suites and 10 tests:

| Suite / test | Same as | Simulator | Waveform |
| ------------ | ------- | --------- | -------- |
| `icarus/tb_basic_sv` | `make icarus` | Icarus | `waves/icarus_tb_basic_sv.fst` |
| `icarus/tb_basic_sv_sram` | `make icarus SRAM=1` | Icarus | `waves/icarus_tb_basic_sv_sram.fst` |
| `icarus/tb_smoke` | `make icarus TB=tb_smoke` | Icarus | `waves/icarus_tb_smoke.fst` |
| `verilator/tb_uvm_sv` | `make verilator` | Verilator | - |
| `verilator/tb_basic_sv` | `make verilator TB=tb_basic_sv` | Verilator | - |
| `verilator/tb_smoke` | `make verilator TB=tb_smoke` | Verilator | - |
| `cocotb/icarus` | `make cocotb` | cocotb + Icarus | `waves/cocotb_icarus.fst` |
| `cocotb/icarus_sram` | `make cocotb SRAM=1` | cocotb + Icarus | `waves/cocotb_icarus_sram.fst` |
| `cocotb/verilator` | `make sc ARGS="cocotb --tool verilator"` | cocotb + Verilator | - |
| `uvm/packet_test` | `make verilator_uvm COV=0` | Verilator >= 5.052 + Accellera UVM 2020.3.1 | - |

`xezim_uvm` is not included. SimCrux has no xezim driver, and xezim's UVM report summary cannot be checked (see [tests/uvm/README.md](../tests/uvm/README.md#known-limitations)).

### What each test proves

The testbenches differ a lot in how much they check (see [tests/README.md](../tests/README.md#testbench-files-explained)). `tb_basic_sv` and `tb_uvm_sv` print PASSED unconditionally, and `tb_smoke` checks nothing. A SimCrux pass that only looked for those lines, or at the exit code, would be meaningless.

Each test's `pass_fail` is therefore a `composite` detector. Every test needs a clean exit (or the cocotb / UVM summary) **and** evidence in the log that the DUT behaved as expected. The table states exactly what that evidence is. Treat anything not listed as untested.

| Test | Passes only if | Catches | Does **not** catch |
| ---- | -------------- | ------- | ------------------ |
| `*/tb_basic_sv`, `icarus/tb_basic_sv_sram` | exit 0; `Packets received: 6, Errors: 0` (5 in-order + 1 out-of-order packet all came out); `SCOREBOARD: Packet Lost detected!` (the skipped seqNumber raised `o_packetLost`) | lost or extra output packets, missing `o_packetLost` | wrong `o_data` (the testbench never compares it) |
| `*/tb_smoke` | exit 0; `packetLost=1` and `validOut=1` appear in the `$monitor` trace | missing `o_packetLost`, no output at all | wrong `o_data`, wrong packet count |
| `verilator/tb_uvm_sv` | exit 0; `TEST PASSED: All 100 packets` (it waits for one output per input, so it hangs → timeout if packets go missing); no `Received unexpected packet`; `Coverage 100 percent Achieved` (packet loss, 1-word and 9-word packets seen) | missing outputs, `o_packetLost` never pulsing over 100 random packets | wrong `o_data` (the scoreboard's comparison is commented out) |
| `cocotb/*` | the cocotb summary has no failures **and** reads `TESTS=8 PASS=8 FAIL=0`. A lower count means a test stopped being collected | wrong `o_data`, `o_packetLost`, handshake, back-pressure and reset behaviour: the 8 tests listed in [tests/README.md](../tests/README.md#1-cocotb-flow) | anything in the two undecorated tests (`test_fifo_full_backpressure`, `test_max_length_packet`) |
| `uvm/packet_test` | the UVM report summary shows no `UVM_ERROR` / `UVM_FATAL`, **and** `[SCOREBOARD] 16` (one scoreboard check per packet of `packet_seq`) | malformed headers on the *input* side, a sequence that stops early | any DUT output bug: the scoreboard only checks input-side headers |

The data path (`o_data`) is checked only by the cocotb suite. These claims were verified by running the regression against three deliberately broken copies of `packet_handler.v`. The results are in [Mutation check](#mutation-check).

### Mutation check

To check that a green regression means something, the regression was run against three broken copies of `packet_handler.v`. The UVM suite ran only on M2.

| Test | M1: `o_packetLost` tied to 0 | M2: `o_data` bit 0 flipped | M3: `o_valid` never set |
| ---- | --------------------------- | -------------------------- | ----------------------- |
| `icarus/tb_basic_sv`, `icarus/tb_basic_sv_sram` | fail | **pass** | fail |
| `icarus/tb_smoke`, `verilator/tb_smoke` | fail | **pass** | fail |
| `verilator/tb_basic_sv` | fail | **pass** | fail |
| `verilator/tb_uvm_sv` | fail (coverage: no packet loss seen) | **pass** | timeout (waits for outputs) |
| `cocotb/verilator` | fail (`test_dropped_lost_packet`) | fail (5 of 8 tests) | timeout |
| `uvm/packet_test` | - | **pass** | - |

`cocotb/icarus*` are left out because they fail on the development machine for toolchain reasons (see [Known issues](#known-issues)). They run the same Python tests as `cocotb/verilator`. The **pass** entries are the gaps listed above: only cocotb checks `o_data`. Within cocotb, `test_reset`, `test_multiple_valid_streams` and `test_dropped_lost_packet` don't compare data either, so they passed on M2. Closing these gaps means changing the testbenches, which is outside this integration. The UVM roadmap item for a receiver-side monitor and reference-model scoreboard ([tests/uvm/README.md](../tests/uvm/README.md#roadmap)) is the main one.

SimCrux records per-test cocotb results as metrics. Examples are `cocotb.test.tb_cocotb.test_reset.status` and `...sim_time_ns` in `results.ndjson`, so the SimCrux app shows all 8 cocotb tests even though they run as one SimCrux test.

### How SimCrux runs each simulator

Behaviour of SimCrux 1.1.0's drivers that this configuration depends on:
- **Icarus:**
  - Runs `iverilog -g2012 -s <top> -I.. -D..`, then `vvp ... -fst`.
  - The testbenches' `$dumpfile` is driven by the `WAVE` / `WAVE_FILE` defines, the same mechanism as `make icarus WAVE=1`. `WAVE_FILE` ends in `.fst` because the `-fst` flag makes Icarus write FST whatever the file is called.
- **Verilator:**
  - Builds `--cc --exe` with its own generated `sim_main.cpp`, adding `-Wall -Wno-fatal` (plus `--timing` on Verilator 5).
  - `simulators.verilator.options.args` adds `--timescale 1ns/1ps` and `-j 0`. The timescale is what tests/uvm needs, and it only applies to files without a `` `timescale``. Without `-j 0`, the driver's `-j 1` makes the UVM build take about 6× longer.
  - Lint warnings do not fail the build here, unlike `make verilator`. With Verilator 5.046, `make verilator TB=tb_basic_sv` fails on two `-Wall` warnings (`IMPLICITSTATIC`, `PROCASSINIT`), but `verilator/tb_basic_sv` passes.
- **cocotb:**
  - Copies the test's `sources` *flat*, by basename, into a fresh work directory, then runs `make` there with `SIM`, `TOPLEVEL`, `TOPLEVEL_LANG`, `MODULE` / `COCOTB_TEST_MODULES` and `COCOTB_RANDOM_SEED`.
  - It does not pass `VERILOG_SOURCES`, so `cocotb/Makefile` builds it from the copied files, and adds `-DPH_SKY130_SRAM` when `sky130_sram_model.v` is among them.
  - Other `parameters` only change the test id. They are not exported, so individual cocotb tests cannot be selected per SimCrux test.
- **Work directories** are under `/tmp/runs/<run_id>/`. `make simcrux` copies the waveforms out because SimCrux removes some of these folders.

## WaveCrux

`make simcrux` leaves FST files in `edacrux/waves/` (Icarus tests and cocotb-on-Icarus). `make wavecrux` opens the design manifest, which points at `waves/icarus_tb_smoke.fst`. To open a specific waveform:

```bash
make wavecrux WAVE_FILE_CRUX=edacrux/waves/icarus_tb_basic_sv.fst
make wavecrux WAVE_FILE_CRUX=waves_icarus_tb_smoke.vcd     # from `make icarus TB=tb_smoke WAVE=1`
make wavecrux WAVE_FILE_CRUX=waves_verilator_uvm.vcd       # from `make verilator_uvm WAVE=1`
```

Verilator waveforms are not produced by SimCrux (see [Known issues](#known-issues)). Use the existing `make verilator WAVE=1` / `make verilator_uvm WAVE=1` targets and open the VCD as above.

## NetCrux

`make netcrux` opens `packet_handler.netcrux-project`. NetCrux elaborates `packet_handler.v` with Yosys (top `packet_handler`) and shows the schematic, hierarchy, and the FSM and CDC analysis panes. `blackBoxMissingModules` is on, so a missing module shows as a black box instead of failing elaboration.

## Known issues

- **Verilator waveforms under SimCrux.** SimCrux 1.1.0's generated Verilator `sim_main.cpp` never calls `Verilated::traceEverOn(true)`. A testbench that calls `$dumpfile` therefore aborts with "Turning on wave traces requires Verilated::traceEverOn(true) call before time 0" (seen with Verilator 5.046 and 5.055). The Verilator tests set `waveform: { capture: never }` and don't define `WAVE`.
- **Verilator FST and zlib.** With `waveform.format: fst`, the default, the Verilator driver builds with `--trace-fst`, which needs the zlib development headers (`zlib.h`). Fedora's Verilator package does not pull them in (`zlib-ng-compat-devel`).
- **cocotb on OSS CAD Suite's Icarus.** OSS CAD Suite's `vvp` wrapper sets `PYTHONHOME` to its bundled Python 3.11. A pip-installed cocotb (built for the system Python) then fails to start with `No GPI_USERS specified`, and the existing `make cocotb` fails the same way. With OSS CAD Suite's own cocotb (2.1.0.dev, nightly 2026-11-12) the tests run, but `vvp` segfaults at exit: cocotb's GPI callback runs after the Python interpreter has shut down. cocotb then never writes `results.xml`, `make` exits 2, and `cocotb/icarus*` fail on the exit code even though all 8 tests passed. The fix is a matching pair. One option is a distro `iverilog` with pip cocotb (e.g. Fedora: `sudo dnf install iverilog`; CI's Ubuntu `iverilog` + cocotb 2.x works). The other is a cocotb release built for that Icarus.
- **Fixed per-test parameters.** Seeds and UVM test names are not swept. `uvm/packet_test` runs the testbench's default `run_test("packet_test")` with the seed SimCrux picks. Use `make verilator_uvm SEED=<n> UVM_TESTNAME=<t>` for other combinations.
