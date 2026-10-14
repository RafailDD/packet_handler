# Functionality

A packet arrives at the input data, which includes a header with a fixed format and the data with a variable length.

packet format

| 8 bytes |  variable  |
| ------- | ---------- |
| Header  |  Data      |

header format

|  2 bytes    |  2 bytes    |  4 bytes     |
| ----------- | ----------- | ------------ |
|  msgLength  |  streamId   |  seqNumber   |

For the design, the byte order in each field is assumed to be in little endian.

**msgLength**: 2 bytes, with a range of 9-45, describes the size of the packet (header+data) in bytes

**streamId**: 2 bytes, with a range of 1-32, it identifies the stream that the packet belongs to

**seqNumber**: 4 bytes, starting from 1, it increases for each packet arriving from a particular streamId. For example, a packet with streamId 15 arrives first with a seqNumber 2. The next packet arriving with a streamId 15 is expected to have a seqNumber 3.

## Transmitter Interface

| Name     | Direction | Width | Description                                                                                                                                                        |
| -------- | --------- | ----- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------ |
| i_clk    | in        | 1     | Clock                                                                                                                                                              |
| i_rst_n  | in        | 1     | Active low reset                                                                                                                                                   |
| i_data   | in        | 32    | Input data for packet                                                                                                                                              |
| i_valid  | in        | 1     | Active High. Indicates that the transmitter is ready to start a data transaction which is considered to take place when both `i_valid` and `o_ready` are asserted. |
| o_ready  | out       | 1     | Active High. Indicates that the handler is ready to accept data.                                                                                                   |
| i_last   | in        | 1     | Active High. Indicates the last set of `i_data` sent by the transmitter.                                                                                           |

## Receiver Interface

| Name         | Direction | Width | Description                                                                                                                                                    |
| ------------ | --------- | ----- | -------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| i_clk        | in        | 1     | Clock                                                                                                                                                          |
| i_rst_n      | in        | 1     | Active low reset                                                                                                                                               |
| o_data       | out       | 296   | Output data for payload of packet                                                                                                                              |
| o_valid      | out       | 1     | Active High. Indicates that the handler is ready to start a data transaction which is considered to take place when both `o_valid` and `i_ready` are asserted. |
| i_ready      | in        | 1     | Active High. Indicates that the receiver is ready to accept data.                                                                                              |
| o_packetLost | out       | 1     | Active high pulse for one clock cycle. Indicates seqNumber that is not continuous for the particular streamId of a packet.                                     |

## Design Overview

The RTL (`packet_handler.v`) can be broken into 3 main parts:

1. **Input FIFO**: 256 words deep, stores `{i_last, i_data}`. `o_ready` is simply `~fifo_full`, so the transmitter is only back-pressured when the FIFO is full. The storage is `ph_fifo_mem`, a synchronous-read memory followed by a 2-entry prefetch buffer. By default it is a behavioral model (flip-flops after synthesis). Defining `PH_SKY130_SRAM` (`make ... SRAM=1`) swaps in the Sky130 OpenRAM macro; see [FIFO memory options](docs/memory_options.md).
2. **Lost packet detection** (input side): a small FSM (`in_state`: `IN_W1` / `IN_W2` / `IN_DATA`) watches words as they are *written* into the FIFO, keeps the last seqNumber of each of the 32 streams (`packetTracker`, plus a reset `tracker_valid` flag per stream) and raises the 1-cycle `o_packetLost` pulse. Because this happens at FIFO write time, the pulse is not aligned with the `o_valid` of the same packet.
3. **Output FSM** (read side): a one-hot FSM with 4 states (`IDLE`, `HEADER`, `DATA`, `DONE`) that pops the FIFO, separates the header, shifts the payload into a 296-bit shift register and presents it on `o_data` with `o_valid`, holding it in `DONE` until `i_ready`.

RTL contains comments to help navigate through its logic and understand each part.

The original smoke testbench (`tests/tb_smoke.v`) generates 3 packets of input. Each msgLength is 24 bytes, or 16 bytes of data plus 8 bytes for the header. The streamId is 15. These number are converted to hexadecimal and then put in little endian byte order. The data remain the same for each packet for simplicity and readability. The first packet has a seqNumber of 1, the second 2 and the third 4, to generate an o_packetLost pulse. The other testbenches are described in the [Verification Environment Guide](tests/README.md).


# Teros HDL module documentation

> [!WARNING]
> This section and the diagrams in `TerosHDL_diagrams/` and `sigasi_diagrams/` were generated from an earlier revision of the RTL, before the input FIFO was added. For example, they still list `o_packetLostReg` / `o_packetLostReg_d`, and they do not show `fifo_mem`, `fifo_*` pointers, `in_state`, `tracker_valid` or the second FSM. Regenerate them from the current `packet_handler.v` with TerosHDL / Sigasi (see [Documentation diagrams](#documentation-diagrams)).

## Entity: packet_handler 
- **File**: packet_handler.v

## Diagram
![Diagram](./TerosHDL_diagrams/packet_handler.svg)
## Ports

| Port name    | Direction | Type    | Description |
| ------------ | --------- | ------- | ----------- |
| i_clk        | input     |         |             |
| i_rst_n      | input     |         |             |
| i_data       | input     | [31:0]  |             |
| i_valid      | input     |         |             |
| i_ready      | input     |         |             |
| i_last       | input     |         |             |
| o_data       | output    | [295:0] |             |
| o_ready      | output    |         |             |
| o_valid      | output    |         |             |
| o_packetLost | output    |         |             |

## Signals

| Name                 | Type        | Description |
| -------------------- | ----------- | ----------- |
| msgLength            | reg [15:0]  |             |
| streamId             | reg [15:0]  |             |
| seqNumber            | reg [31:0]  |             |
| packetTracker [31:0] | reg [31:0]  |             |
| shiftReg             | reg [295:0] |             |
| state                | reg [3:0]   |             |
| next_state           | reg [3:0]   |             |
| o_packetLostReg      | reg         |             |
| o_packetLostReg_d    | reg         |             |

## Constants

| Name   | Type | Value   | Description |
| ------ | ---- | ------- | ----------- |
| IDLE   |      | 4'b0001 |             |
| HEADER |      | 4'b0010 |             |
| DATA   |      | 4'b0100 |             |
| DONE   |      | 4'b1000 |             |

## Processes
- unnamed: ( @(posedge i_clk or negedge i_rst_n) )
  - **Type:** always
- unnamed: ( @(*) )
  - **Type:** always
- unnamed: ( @(posedge i_clk or negedge i_rst_n) )
  - **Type:** always
- unnamed: ( @(posedge i_clk or negedge i_rst_n) )
  - **Type:** always
- unnamed: ( @(posedge i_clk or negedge i_rst_n) )
  - **Type:** always
- unnamed: ( @(posedge i_clk or negedge i_rst_n) )
  - **Type:** always

## State machines

![Diagram_state_machine_0](./TerosHDL_diagrams/fsm_packet_handler_00.svg)

# Notes
This is a work in progress. The goal is to proceed with synthesis and examine what technologies can be targeted and the max clock frequency it can be achieved. The design may also change (or include different versions) to include a storage element to maximize throughput.

The goal for the documentation is to be fully stand-alone and will be enriched with time.

Tools used for the project:
- [EDA Playground](https://edaplayground.com/)
- [Icarus Verilog](https://steveicarus.github.io/iverilog/)
- [Verilator](https://www.veripool.org/verilator/) (including the UVM flow, based on [GettingVerilatorStartedWithUVM](https://github.com/MikeCovrado/GettingVerilatorStartedWithUVM))
- [xezim](https://github.com/aionhw/xezim)
- [cocotb](https://www.cocotb.org/)
- [Yosys](https://yosyshq.net/yosys/) with the [SkyWater Sky130](https://github.com/efabless/skywater-pdk-libs-sky130_fd_sc_hd) standard cell library
- [GTKWave](https://gtkwave.sourceforge.net/)
- [TerosHDL](https://terostechnology.github.io/terosHDLdoc/)
- [Sigasi Visual HDL](https://www.sigasi.com/) Community Edition
- [Wavedrom](https://wavedrom.com/)

# Project Roadmap
- [x] Add URLs for tools used
- [x] Add project file structure
- [x] Create testbench with cocotb or other tools
- [x] Explore open source flows and tools (OSS Cad Suite, ProjectF, OpenROAD) and how they fit with the project's goals: every flow now also runs through SiliconCompiler, see the [flow manager evaluation](docs/flow_manager_evaluation.md)
- [ ] Create an installation script for OSS Cad Suite, cocotb, and xezim
- [ ] Verify design, post code and functional coverage (code coverage available via `make verilator_uvm`)
- [ ] Explore FPGA options to target specific technologies
- [ ] Look into synthesis options, explore libraries for maximum clock frequency: SDC + OpenSTA timing added, SRAM macro option in [FIFO memory options](docs/memory_options.md), synthesis engines in the [Tardigrade evaluation](docs/tardigrade_evaluation.md)

# Project Structure

```
.
├── packet_handler.v            # The RTL (DUT)
├── Makefile                    # Unified entry point for every flow (run `make` to list targets)
├── .github/workflows/ci.yml    # GitHub Actions CI (see "Continuous Integration" below)
├── flow/                       # SiliconCompiler flows (see "SiliconCompiler flows" below)
│   ├── sc_flow.py              #   one design description, every flow: lint/sim/cocotb/uvm/syn/asic
│   ├── tools/                  #   project SC tasks: UVM (Verilator, xezim), Tardigrade
│   └── Dockerfile              #   toolchain image: SC runner + cocotb + z3
├── macros/
│   ├── sky130_sram_model.v     #   simulation model of the Sky130 OpenRAM macro (SRAM=1)
│   └── sky130_sram/            #   downloaded macro Liberty / LEF (generated, git-ignored)
├── docs/                       # Evaluations: Tardigrade, flow managers, FIFO memory options
├── tests/                      # Verification, see tests/README.md
│   ├── tb_smoke.v              #   legacy Verilog smoke test (waveform inspection)
│   ├── tb_basic_sv.sv          #   standalone SystemVerilog testbench (Icarus / Verilator)
│   ├── tb_uvm_sv.sv            #   UVM-style class-based SV testbench without the UVM library (Verilator)
│   ├── tb_cocotb.py            #   cocotb test suite (Icarus)
│   ├── benchmark.py            #   micro-benchmark of a Python helper used by the cocotb tests
│   ├── logs/                   #   simulation logs (generated, git-ignored)
│   └── uvm/                    #   standard IEEE 1800.2 UVM testbench, see tests/uvm/README.md
│       ├── Makefile            #     xezim and Verilator UVM flows
│       ├── tb_top.sv           #     self-contained UVM testbench (the only file that is compiled)
│       ├── verilator.vlt       #     Verilator lint/coverage/trace control file
│       ├── packet_if.sv, env/, seq/, tests/   # split-out copies of the classes in tb_top.sv (not compiled)
│       └── uvm-src/, uvm-core/, obj_dir/, logs/  # downloaded UVM libraries and build outputs (generated)
├── synth/                      # Yosys synthesis + OpenSTA flow
│   ├── synth.tcl               #   synthesis script (Sky130 HD, typical corner, timing-driven ABC)
│   ├── packet_handler.sdc      #   timing constraints (clock, I/O delays), also used by SiliconCompiler
│   ├── sta.tcl                 #   OpenSTA script used by `make sta`
│   ├── scripts/synth_parse.py  #   log summarizer used by `make synth_parse`
│   └── lib/                    #   downloaded Liberty file (generated, git-ignored)
├── wavedrom_diagrams/          # Interface timing diagrams (WaveDrom .json sources + rendered .svg)
├── TerosHDL_diagrams/          # TerosHDL-generated module/FSM documentation (stale, see above)
└── sigasi_diagrams/            # Sigasi-generated block diagram and HTML documentation (stale, see above)
```

# Running Every Tool

All flows are driven from the top-level `Makefile`. Run `make` with no arguments to print the list of targets and options.

> [!NOTE]
> When cocotb is installed, `make help` is intercepted by cocotb's own `help` target (its Makefile is included by ours) and prints cocotb's variables instead, then exits with an error. Use plain `make` to see the project's help text.

## Prerequisites

| Tool | Needed for | Version known to work | Install (Ubuntu) |
| ---- | ---------- | --------------------- | ---------------- |
| GNU Make, git, wget | everything | - | `sudo apt-get install make git wget` |
| Icarus Verilog | `cocotb`, `icarus` | 12.0 | `sudo apt-get install iverilog` |
| Verilator | `verilator`, RTL lint | 5.020 (Ubuntu 24.04 package) | `sudo apt-get install verilator` |
| Verilator + z3 | `verilator_uvm`, `verilator_uvm_all` | v5.052 or newer (the apt package is too old) | see [tests/uvm/README.md](tests/uvm/README.md#installing-verilator) |
| Python 3 + cocotb | `cocotb`, `benchmark` (Python only) | Python 3.10/3.11, cocotb 2.x (the tests use the cocotb 2.0 API) | `pip install cocotb` |
| xezim (Rust/cargo) | `xezim_uvm` | built from source | see [tests/uvm/README.md](tests/uvm/README.md#xezim-flow) |
| Yosys | `synth` | 0.33 | `sudo apt-get install yosys` |
| OpenSTA | `sta` (run by `synth`) | 3.1 | not packaged; `make sta` uses the `sc_runner` Docker image when `sta` is not on PATH |
| Docker | `sc`, `sc_image`, `sta` without a local OpenSTA | - | see docs.docker.com |
| GTKWave (optional) | `GUI=1` | - | `sudo apt-get install gtkwave` |

Only the tools for the flows you want to run are required. cocotb is optional for all non-cocotb targets.

## Make targets

| Command | What it does | Outputs |
| ------- | ------------ | ------- |
| `make` | Print the project help text | - |
| `make cocotb` | Run the cocotb suite `tests/tb_cocotb.py` on Icarus | `results.xml`, `sim_build/` |
| `make icarus [TB=tb_basic_sv\|tb_smoke]` | Compile and run a SystemVerilog/Verilog testbench on Icarus (default `tb_basic_sv`) | `tests/logs/sim_icarus_<TB>.log` |
| `make verilator [TB=tb_uvm_sv\|tb_basic_sv\|tb_smoke]` | Compile with `verilator --binary -Wall` and run (default `tb_uvm_sv`) | `tests/logs/sim_verilator_<TB>.log`, `obj_dir/` |
| `make xezim_uvm` | Standard UVM testbench on xezim (downloads UVM on first run) | `tests/uvm/logs/xezim_packet_test.log` |
| `make verilator_uvm [UVM_TESTNAME=..] [SEED=..] [COV=0]` | Standard UVM testbench on Verilator v5.052+ with code coverage | `tests/uvm/logs/` |
| `make verilator_uvm_all` | Same, for every test in `ALL_TESTNAMES` | `tests/uvm/logs/` |
| `make synth [CLK_PERIOD_NS=10] [SRAM=1]` | Download the Sky130 HD Liberty file (first run), synthesize with Yosys, then run `make sta` | `synth/synth.log`, `synth/synth_netlist.v`, `synth/sta.rpt` |
| `make sta` | OpenSTA on `synth/synth_netlist.v` with `synth/packet_handler.sdc` | `synth/sta.rpt` |
| `make synth_parse` | Summarize `synth/synth.log` and `synth/sta.rpt` (errors, warnings, area, cells, WNS/TNS/Fmax). Run `make synth` first | stdout |
| `make sc ARGS="<flow> [options]"` | Run a SiliconCompiler flow in the `packet-handler-eda` image (see below) | `build/packet_handler/<job>/` |
| `make sc_image` | Build the `packet-handler-eda` image from `flow/Dockerfile` | Docker image |
| `make benchmark` | Time two ways of packing 32-bit words into an integer (Python helper used by the cocotb tests); not a hardware test | stdout |
| `make clean` | Remove simulation, synthesis and UVM outputs (including downloaded UVM sources). `tests/logs/` and `synth/lib/` are kept | - |
| `verilator --lint-only -Wall packet_handler.v` | Lint the RTL (run by CI; no make target) | stdout |

`SRAM=1` builds the FIFO around the Sky130 SRAM macro (`PH_SKY130_SRAM`) for `cocotb`, `icarus`, `verilator`, `synth` and `sta`; simulations add `macros/sky130_sram_model.v`. Options: `WAVE=1` dumps waveforms and `GUI=1` also opens them in GTKWave (simulation targets only, see [Waveforms & Logs](tests/README.md#waveforms--logs) for where each flow writes them). `TB=<name>` selects the testbench for `icarus` / `verilator` (a `.v`/`.sv` extension is stripped).

Details for each simulation flow are in the [Verification Environment Guide](tests/README.md); the UVM flows are covered in [tests/uvm/README.md](tests/uvm/README.md).

## Synthesis

```bash
make synth                    # Yosys + OpenSTA at CLK_PERIOD_NS=10 (first run downloads the ~13 MB Liberty file)
make synth CLK_PERIOD_NS=8    # tighter target
make synth SRAM=1             # FIFO in the Sky130 SRAM macro (fetches its Liberty / LEF)
make synth_parse              # area, cells and WNS / TNS / Fmax summary
```

`synth/synth.tcl` runs from inside `synth/`. It does generic `synth`, flip-flop mapping (`dfflibmap`) and **timing-driven** logic mapping (`abc -D <period>`, which buffers and sizes against the target) to the Sky130 HD typical corner (25 °C, 1.80 V). Then it makes the netlist STA friendly (tie cells, split nets, no `assign`s), runs `stat` with cell areas and writes `synth_netlist.v`.

`make sta` (run at the end of `make synth`) reads that netlist into OpenSTA with `synth/packet_handler.sdc`:
- one clock on `i_clk`, by default 10 ns;
- 100 ps uncertainty;
- 20% of the period as input and output delay;
- `i_rst_n` treated as a false path, since it is synchronized outside the block.

It writes `synth/sta.rpt`. Results are pre-layout: there are no wire parasitics or clock tree, and the high-fanout reset net reports slew violations that buffering during place-and-route removes. If `sta` is not on PATH, it runs from the `sc_runner` image.

Results at the typical corner (Yosys 0.33):

| FIFO storage | Area | Cells | Min period |
| --- | --- | --- | --- |
| flip-flops (default) | 0.509 mm² | 49,255 | 8.25 ns (121 MHz) |
| Sky130 SRAM macro (`SRAM=1`) | 0.384 mm² | 11,302 + macro | 8.25 ns (121 MHz) |

The critical path is the per-stream sequence check (`packetTracker` read, `+1`, compare), not the FIFO. See [FIFO memory options](docs/memory_options.md).

## SiliconCompiler flows

`flow/sc_flow.py` describes the design once (RTL, testbenches, cocotb module, SDC, the `PH_SKY130_SRAM` define) and runs every flow through [SiliconCompiler](https://github.com/siliconcompiler/siliconcompiler). It runs in the `packet-handler-eda` image (`flow/Dockerfile`: `sc_runner:v0.38.10` + cocotb + z3), so none of the tools need to be installed locally. Why SiliconCompiler was chosen over Bender, Edalize and LogikBench is in the [flow manager evaluation](docs/flow_manager_evaluation.md).

```bash
make sc_image                                        # once
make sc ARGS="lint"                                  # slang + Verilator
make sc ARGS="sim --tool icarus --tb tb_smoke"       # tb_basic_sv | tb_smoke | tb_uvm_sv
make sc ARGS="cocotb --tool verilator"               # cocotb on Icarus or Verilator
make sc ARGS="uvm --tool verilator"                  # tests/uvm (xezim: --tool xezim, needs xezim)
make sc ARGS="syn --clk 8"                           # Yosys + OpenSTA, all Sky130 corners
make sc ARGS="asic --sram"                           # RTL-to-GDS with OpenROAD
make sc ARGS="syn --syn-tool tardigrade"             # once a tardigrade binary is available
```

Add `--sram` to any flow to use the SRAM macro. SiliconCompiler times every Sky130 corner, and its summary reports the **slow** corner (ss, 1.40 V, -40 °C), so `syn` shows about 30 MHz where `make synth` (typical corner, 1.80 V) shows 121 MHz. After place-and-route, both FIFO builds meet 10 ns at the typical and fast corners. Results (logs, reports, netlists, GDS) go to `build/packet_handler/<job>/`. `SC_LOCAL=1 make sc ARGS=...` uses a local `pip install siliconcompiler==0.38.10` and the tools on PATH instead of Docker. SiliconCompiler downloads the Sky130 PDK data from GitHub on first use.

## Documentation diagrams

These are not generated by the Makefile:
- `wavedrom_diagrams/*.json` are [WaveDrom](https://wavedrom.com/) sources for the interface timing diagrams. Render them to SVG with the WaveDrom editor or `wavedrom-cli`.
- `TerosHDL_diagrams/` is produced by the TerosHDL VS Code extension (module documentation + FSM viewer), and `sigasi_diagrams/` by Sigasi Visual HDL (block diagram + HTML documentation export).

## Continuous Integration

`.github/workflows/ci.yml` runs on pushes to `main`, `jules-experimenting` and `claude-main`, on pull requests targeting them, and on manual dispatch. Pushes to `claude-experimenting` run it only when the head commit message contains `[run ci]`. It has five jobs:

| Job | Steps |
| --- | ----- |
| `test` (ubuntu-latest, apt `iverilog` + `verilator`, Python 3.10) | RTL lint, `make cocotb`, `make icarus`, `make verilator`, `make cocotb SRAM=1`, `make icarus SRAM=1`, build xezim from source, `make xezim_uvm`, then the same UVM test on xezim through SiliconCompiler |
| `verilator-uvm` (`verilator/verilator:v5.052` container) | `make -C tests/uvm verilator`, uploads `tests/uvm/logs/` as an artifact |
| `synthesis` (apt `yosys`, OpenSTA from `sc_runner`) | `make synth` + `make synth_parse` with the FIFO in flip-flops and with `SRAM=1`, uploads both reports |
| `siliconcompiler` (`packet-handler-eda` image) | lint, `sim` (Icarus `tb_basic_sv` / `tb_smoke`, Verilator `tb_uvm_sv` with and without `--sram`), `cocotb` (Icarus, Verilator, `--sram`), `uvm --tool verilator`, `syn` and `syn --sram`; uploads logs and reports |
| `asic` (matrix: flops, sram; manual dispatch and pushes to `main` / `claude-main` only) | `make sc ARGS="asic [--sram]"`, full RTL-to-GDS on Sky130 (about 80 min for flops, 11 min for sram); uploads reports and GDS. Post-route results are in [FIFO memory options](docs/memory_options.md#post-route-siliconcompiler-asicflow-openroad-ci-run-38466551924) |

`make icarus TB=tb_smoke` (it runs through SiliconCompiler instead), `make verilator TB=tb_basic_sv` and `make benchmark` are not run in CI.
