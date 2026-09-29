# UVM Flows (xezim and Verilator)

This directory contains a complete, standard SystemVerilog UVM environment for the `packet_handler` module.

Historically, open-source simulators struggled to compile and run the full IEEE 1800.2 UVM base class library, requiring heavy patching or workarounds. The same testbench (`tb_top.sv`) now runs on two open-source simulators:

| Flow | Command | Simulator | UVM library | Extras |
| ---- | ------- | --------- | ----------- | ------ |
| xezim | `make xezim_uvm` (or `make sim` here) | [xezim](https://github.com/aionhw/xezim), Rust-based | `chipsalliance/uvm-verilator`, branch `uvm-2020-3.1-vlt` | – |
| Verilator | `make verilator_uvm` (or `make verilator` here) | [Verilator](https://www.veripool.org/verilator/) v5.052 | Unmodified Accellera `uvm-core`, tag `2020.3.1` | Code coverage, waveforms, seeds, multi-test runs |

Running both is deliberate: the same UVM code on two independent simulators catches testbench bugs and simulator bugs that a single tool can hide.

Both flows **fail** unless the UVM report summary is printed with no non-zero `UVM_ERROR` / `UVM_FATAL` count (but see the xezim caveat under Known limitations). Simulation logs are written to `tests/uvm/logs/`.

## Implementation Overview

The environment is built using standard UVM components:
- **`packet_item`**: The transaction class that holds randomized packet fields (msgLength, streamId, seqNumber, payload).
- **`packet_seq`**: The sequence: 5 in-order packets on stream 15, one with a skipped seqNumber (7 instead of 6), then 10 randomized packets.
- **`packet_driver`**: Drives the UVM items onto the physical pins via a virtual interface.
- **`packet_monitor`**: Observes the *input* pins (`i_data`/`i_valid`/`o_ready`/`i_last`), reconstructs the header of each packet, samples the `packet_cg` covergroup and sends the transaction to the scoreboard via an analysis port.
- **`packet_scoreboard`**: Checks each monitored header is well formed (streamId in 1..32, seqNumber > 0). It does not look at the DUT outputs (see Known limitations).
- **`packet_agent` / `packet_env`**: Standard UVM hierarchical containers.
- **`packet_test`**: The top-level test that configures the environment and starts sequences on the sequencer.

> [!NOTE]
> `tb_top.sv` is self-contained: it declares every class inline, and it is the only file either flow compiles. The files in `env/`, `seq/`, `tests/` and `packet_if.sv` are split-out copies of the same classes. Keep them in sync when you edit `tb_top.sv`.

### Wire format

The driver and monitor follow the same byte order as `packet_handler.v` and `tb_cocotb.py`. Bytes are sent MSB-first on `i_data`, and each header field is little-endian:

| Word | `[31:24]` | `[23:16]` | `[15:8]` | `[7:0]` |
| ---- | --------- | --------- | -------- | ------- |
| 1 | msgLength[7:0] | msgLength[15:8] | streamId[7:0] | streamId[15:8] |
| 2 | seqNumber[7:0] | seqNumber[15:8] | seqNumber[23:16] | seqNumber[31:24] |

---

## Xezim Flow

### Installing Rust
`xezim` is written in Rust. If you don't have Rust installed, install it using `rustup`:
```bash
curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh
```

### Installing Xezim
`xezim` consists of two repositories that need to be cloned side-by-side:
```bash
# In an installation directory of your choice:
git clone https://github.com/aionhw/xezim-core.git
git clone https://github.com/aionhw/xezim.git

cd xezim
cargo build --release
```
Ensure the `xezim/target/release` directory is added to your system's `PATH`.

### Running
```bash
make xezim_uvm          # from the repository root
make sim                # from tests/uvm
```

#### What happens under the hood?
1. **Fetch UVM**: The Makefile clones the `chipsalliance/uvm-verilator` UVM 2020.3.1 branch into `uvm-src/`.
2. **Execute Simulation**: `xezim` compiles the `packet_handler` RTL along with the UVM library and the `tb_top.sv` testbench.
3. **Check**: The log (`logs/xezim_packet_test.log`) is checked for UVM errors.

---

## Verilator Flow

The Verilator flow adapts the recipe from [MikeCovrado/GettingVerilatorStartedWithUVM](https://github.com/MikeCovrado/GettingVerilatorStartedWithUVM), which tracks each Verilator release against the stock Accellera UVM library. We use its flag set, `UVM_NO_DPI` build and coverage flow. Lint waivers live in `verilator.vlt` and are scoped: they cover the UVM library plus a short list of rules for `tb_top.sv`, not the whole build. The RTL stays under full `-Wall`.

### Installing Verilator

A recent Verilator is required (tested with **v5.052**). Distro packages are too old. For example, Ubuntu 24.04 ships 5.020, which cannot compile the stock UVM library.

You also need **z3** on `PATH`. Verilator calls it at runtime to solve `randomize()` constraints:
```bash
sudo apt-get install z3
```

**Option A: build from source** (see the [install guide](https://verilator.org/guide/latest/install.html)):
```bash
sudo apt-get install git autoconf flex bison help2man perl python3 make g++ libfl-dev zlib1g-dev
git clone -b v5.052 https://github.com/verilator/verilator.git
cd verilator && autoconf && ./configure && make -j$(nproc) && sudo make install
```

**Option B: use the official container** (this is what CI does):
```bash
docker run --rm -it --entrypoint bash -v "$PWD":/work -w /work verilator/verilator:v5.052
apt-get update && apt-get install -y git make z3
make -C tests/uvm verilator
```

### Running
```bash
make verilator_uvm                         # from the repository root
make verilator_uvm UVM_TESTNAME=packet_test SEED=42
make verilator_uvm WAVE=1                  # writes waves_verilator_uvm.vcd (DUT scope only)
make verilator_uvm GUI=1                   # ...and opens it in GTKWave
make verilator_uvm COV=0                   # skip code coverage instrumentation
make verilator_uvm_all                     # run every test listed in ALL_TESTNAMES
```

Targets available from `tests/uvm` (run `make help` there):

| Target | Description |
| ------ | ----------- |
| `verilator` | Build (if needed), run `UVM_TESTNAME`, check the log, generate coverage |
| `verilator_all` | Same, for every test in `ALL_TESTNAMES` |
| `verilator_build` | Compile only |
| `verilator_cov` | Re-generate the coverage report from `logs/coverage/*.cov` |

| Option | Default | Description |
| ------ | ------- | ----------- |
| `UVM_TESTNAME` | `packet_test` | Test passed as `+UVM_TESTNAME` |
| `ALL_TESTNAMES` | `packet_test` | Tests run by `verilator_all` |
| `SEED` | `1` | `+verilator+seed+` value. Changes randomized stimulus without a rebuild |
| `COV` | `1` | Line/toggle/branch/expression coverage of the DUT and testbench |
| `WAVE` / `GUI` | off | VCD dump of `tb_top.dut` |
| `UVM_HOME` | `uvm-core/src` | Point at another UVM install, e.g. an Accellera 2020.3.2 tarball |
| `VERILATOR` | `verilator` | Verilator binary |
| `NJOBS` | `nproc` | Parallel C++ build jobs |

#### What happens under the hood?
1. **Fetch UVM**: The Makefile shallow-clones `accellera-official/uvm-core` at tag `2020.3.1` into `uvm-core/`.
2. **Compile**: Verilator compiles the UVM package, the RTL and `tb_top.sv` into `obj_dir/Vtb_top`. This takes about 2 minutes and 700 MB of RAM on 4 cores, and is dominated by the UVM library. The binary is rebuilt only when sources or build options (`WAVE`, `COV`) change.
3. **Run and check**: The binary runs the test and writes `logs/verilator_<test>.log`, which is checked for UVM errors.
4. **Coverage**: `verilator_coverage` writes annotated sources to `logs/annotated_src/` (lines marked `%00` are uncovered) and an lcov file to `logs/coverage.info`. Turn that into HTML with `genhtml logs/coverage.info -o logs/html`. The UVM library is excluded, so the numbers reflect only `packet_handler.v` and `tb_top.sv`.

### Known limitations
- Coverage covers code only (line, toggle, branch, expression). The monitor's covergroup compiles, but the flow does not report functional coverage yet.
- The scoreboard only checks input-side header fields. `o_data` and `o_packetLost` are not compared against a reference model yet (see roadmap below).
- **xezim pass/fail check**: the log check looks for the `UVM_ERROR : <n>` / `UVM_FATAL : <n>` lines of the UVM report summary. In the last CI run of this flow (xezim built from its default branch at the time), xezim's summary did not list `UVM_INFO` / `UVM_ERROR` / `UVM_FATAL` counts at all, although 16 `UVM_ERROR` messages had been printed. If that is still the case, the xezim target cannot detect scoreboard errors. The same log also showed a `[DRVCONNECT]` warning (driver not connected to a sequencer) even though items were driven, and identical "random" field values across packets.
- CI builds xezim and xezim-core from their default branches (not pinned), so the xezim results can change without any change in this repository.

## Roadmap
- [x] Verilator + stock Accellera UVM flow (`make verilator_uvm`), CI job, pass/fail gate on UVM errors for both flows
- [x] Code coverage restricted to DUT + testbench
- [ ] Receiver-side monitor and reference-model scoreboard: check `o_data`, `o_valid` and the `o_packetLost` pulse
- [ ] More tests in `ALL_TESTNAMES`: receiver back-pressure (`i_ready` low, `fifo_full`), mid-packet reset, multi-stream loss
- [ ] Make `tb_top.sv` include `env/`, `seq/` and `tests/` instead of duplicating them (one source of truth for both flows)
- [ ] HTML coverage report (`genhtml`) published as a CI artifact; functional coverage once Verilator reports covergroups
- [ ] Speed up CI builds (ccache for `obj_dir`)

## Cleaning Up
To remove downloaded UVM sources, build outputs and logs, run:
```bash
make clean
```
