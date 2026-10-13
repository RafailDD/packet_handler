# FIFO memory options

The 256 x 33-bit input FIFO (`{i_last, i_data}`) was 8,448 flip-flops plus a 256:1 read mux. That made it about 80% of the design's area and most of the synthesis runtime. This page covers what was changed to let the FIFO use a memory macro, the options considered, and the results.

## What changed in the RTL

An SRAM macro has a **synchronous** read: the address is registered on the clock edge, and data comes back later in the cycle. The old FIFO read combinationally (`fifo_mem[fifo_rd_ptr]`), so no macro could replace it. The FIFO in `packet_handler.v` is now built in two parts:

1. **`ph_fifo_mem`** is a 1-write / 1-read memory with a synchronous read, defined at the bottom of `packet_handler.v`. It has two implementations, selected by a define:

   | Build | `ph_fifo_mem` is | Used for |
   | --- | --- | --- |
   | default | behavioral `reg` array with a registered read | every simulator, and synthesis to flip-flops (or FPGA block RAM) |
   | `PH_SKY130_SRAM` | OpenRAM `sky130_sram_2kbyte_1rw1r_32x512_8` for bits [31:0] (port 0 writes, port 1 reads, address MSB tied low) plus 256 flip-flops for `i_last` | Sky130 synthesis / P&R; simulation with `macros/sky130_sram_model.v` |

2. **A 2-entry prefetch buffer** turns the synchronous read back into the first-word-fall-through interface the output FSM expects (`fifo_valid` / `fifo_data` / `fifo_last` / `fifo_rd_en`). A read is issued only when the buffer will have room for the returning word (`fbuf_cnt + mem_rd_pending - pop < 2`), which still sustains one word per cycle.

The RTL itself never changes between the two builds. Only `ph_fifo_mem`'s body does, so every simulation of the default build also verifies the FIFO logic used with the macro.

### Behavior compared with the old FIFO

- **Same:** packet contents, `o_packetLost` (generated on the write side) and the back-pressure point. `fifo_count` still counts every word that is written and not yet popped, so `o_ready` drops after 256 buffered words. A probe that streams packets with `i_ready = 0` sees 260 words accepted in both the old and new RTL (256 + the 4 the FSM consumes).
- **Different:** a word reaches the output FSM **2 cycles later** (memory read + buffer). No testbench depends on the exact latency.

### Verification

| Flow | Default | `SRAM=1` |
| --- | --- | --- |
| `make cocotb`: 8 registered tests, plus the unregistered `test_fifo_full_backpressure` / `test_max_length_packet` | pass | pass |
| `make icarus`, `make verilator` | pass | pass |
| `make verilator_uvm` (UVM scoreboard, Verilator 5.052) | pass | not wired up |
| SiliconCompiler cocotb on Icarus 13 / Verilator 5.052, Verilator `tb_uvm_sv` | pass | pass |
| `verilator --lint-only -Wall` | clean | clean |

### The macro simulation model

`macros/sky130_sram_model.v` is a self-contained model with the macro's edge timing:

- inputs are registered on the rising edge;
- writes and reads happen on the falling edge;
- `dout` goes **X** on the next rising edge.

A design that expects the output to hold its value (OpenRAM does not guarantee that) therefore fails in simulation. The model was written because OpenRAM's own model doesn't compile on Icarus 13 (it uses `mem` before declaring it), needs `--timing` on Verilator, and prints every access.

## Options considered

| Option | Fits 256 x 33? | Views available | Status |
| --- | --- | --- | --- |
| Flip-flops (behavioral model) | yes | none needed | **default** |
| OpenRAM `sky130_sram_2kbyte_1rw1r_32x512_8` | 32 bits; half the words unused; `i_last` in flops | Liberty, LEF, GDS, LVS in LambdaPDK (SiliconCompiler uses them directly) | **implemented** (`PH_SKY130_SRAM`, `make ... SRAM=1`, `sc_flow.py --sram`) |
| OpenRAM `sky130_sram_1kbyte_1rw1r_32x256_8` | exact depth, about 2/3 the area of the 2 KB macro | efabless `sky130_sram_macros` repo; not in LambdaPDK | possible follow-up: needs its own SiliconCompiler library definition |
| DFFRAM (OpenLane latch / flop RAM compiler) | configurable | generated per configuration | not tried: needs the DFFRAM generator in the flow |
| FPGA block RAM | yes | — | the default model's registered read is the pattern Vivado / Yosys infer as BRAM; relevant to the "FPGA options" roadmap item |
| Smaller FIFO | — | — | an architecture decision: changes back-pressure behavior |

## Results

### Synthesis only

From `make synth`, Yosys 0.33 + OpenSTA at a 10 ns target, Sky130 HD typical corner, no wire parasitics:

| | Flip-flop FIFO | SRAM macro FIFO |
| --- | --- | --- |
| Yosys CPU time | 7.4 s | **2.8 s** |
| Cells | 49,255 | **11,302** + 1 macro |
| Flip-flops | 10,203 | **1,979** |
| Area | 0.509 mm² | **0.384 mm²** (0.285 mm² is the macro) |
| Min period | 8.25 ns | 8.25 ns |

After synthesis, the SRAM build is about 2.6x faster to synthesize, with 4.4x fewer cells and 25% less area. **Fmax doesn't change** because the critical path isn't in the FIFO. It runs through the sequence-number tracker:

```
in_streamId -> packetTracker[idx] read mux -> +1 (ripple carry) -> != seqNumber -> in_packetLost
```

Storing the *expected* next sequence number (`seqNumber + 1`) in `packetTracker` at write time would move the adder off this path, leaving an equality compare. That is the next timing improvement, independent of the memory choice.

> [!CAUTION]
> The OpenRAM Liberty files are analytical estimates: 0.38 ns clock-to-out and a 1.96 ns minimum period at TT. Silicon measurements of the Sky130 OpenRAM macros are considerably slower, so treat timing through the macro as optimistic until it's checked against silicon data. The read path also launches on the **falling** edge, so the macro output to the prefetch register is a half-cycle path.

### Post-route (SiliconCompiler `asicflow`, OpenROAD, CI run 38466551924)

`make sc ARGS="asic"` vs `make sc ARGS="asic --sram"`, Sky130 HD, 10 ns clock, after detailed routing:

| | Flip-flop FIFO | SRAM macro FIFO |
| --- | --- | --- |
| RTL-to-GDS runtime (GitHub runner) | 80 min | **11 min** |
| Cells (incl. fill / tap) | 117,700 | 80,045 |
| Std-cell area | 0.443 mm² | **0.092 mm²** + 0.285 mm² macro |
| Die area | 0.961 mm² | 0.901 mm² |
| Wirelength | 1,291 mm | **238 mm** |
| Peak power (estimate) | 104.9 mW | **22.5 mW** |
| Routing DRCs | 0 | 0 |
| Setup slack: typical (tt 1.80 V 25 °C) | +4.71 ns (~189 MHz) | +4.05 ns (~168 MHz) |
| Setup slack: fast | +6.24 ns | +4.29 ns |
| Setup slack: slow (ss 1.40 V -40 °C) | -10.41 ns (~49 MHz) | -8.89 ns (~53 MHz) |
| Hold slack: slow | -0.02 ns | -2.40 ns |
| Slew / capacitance violations (DRVs) | 2,220 | 155 |

Both builds close timing at the typical and fast corners, and neither closes at the slow corner. The critical path is the same in both: `in_streamId → packetTracker → +1 → o_packetLost`.

The SRAM build's slow-corner hold violations are all at the block boundary: `i_data` input ports to the macro's `din0` and to `packetTracker`. After CTS, the propagated clock arrives several ns late at that corner, while the SDC's input delay is referenced to the ideal clock edge. In a chip, the top-level constraints (I/O timing, or a `set_clock_latency -source` budget) decide whether these are real. The SiliconCompiler job reports timing as metrics and does not fail on it.

## How to use it

```bash
make synth SRAM=1           # fetches the macro Liberty/LEF into macros/sky130_sram/
make cocotb SRAM=1          # also: make icarus SRAM=1, make verilator SRAM=1
make sc ARGS="syn --sram"   # SiliconCompiler: Yosys + OpenSTA on all corners
make sc ARGS="asic --sram"  # SiliconCompiler: place and route with the macro
```
