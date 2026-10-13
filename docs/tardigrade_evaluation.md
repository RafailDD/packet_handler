# Evaluation: Tardigrade ASIC synthesis for `packet_handler`

*Evaluated October 2026. Sources: the [launch post](https://www.zeroasic.com/blog/tardigrade_launch), the [product page](https://www.zeroasic.com/tardigrade), and the Tardigrade wrapper in [LogikBench](https://github.com/zeroasiccorp/logikbench) (`logikbench/tools/tardigrade/tardigrade.py`).*

## Verdict

**Don't integrate it yet.** The tool fits the roadmap item *"look into synthesis options, explore libraries for maximum clock frequency"*, but three things block integration now:

1. **We can't get the binary.** Tardigrade has no public download, repository, or pricing. Zero ASIC says it "will never be a traditional commercial EDA product" and offers access only through partnership or source-license talks. Nothing could be installed or tested in CI.
2. **Our flow can't measure what Tardigrade improves.** Tardigrade's selling point is timing-driven synthesis: it claims a 2.5-5x Fmax gain over Yosys/ABC, driven by SDC constraints. `synth/synth.tcl` has no SDC, no clock, and no STA, so it reports area only. We would have no baseline Fmax to compare against.
3. **Something else dominates our QoR.** The 256 x 33-bit FIFO and the 32 x 32-bit tracker are built from flip-flops (more than 9k flops), and the `o_data` shift register is 296 bits wide. Area, and probably the critical path through the FIFO read mux, depend on these architecture choices more than on the synthesis engine.

**Recommendation:** Build the timing infrastructure Tardigrade would plug into. That work pays off now with Yosys. Once it exists, adding Tardigrade is a one-line target change, so we can revisit if Zero ASIC grants access.

## What Tardigrade is

| Aspect | Details |
| ------ | ------- |
| Function | RTL to gate-level netlist ASIC synthesis (standard-cell mapping, timing-driven optimization) |
| Input | Synthesizable SystemVerilog (our Verilog-2001 RTL is a subset), SDC constraints, Liberty files |
| Output | Gate-level Verilog netlist (`.vg`), QoR metrics as JSON (`--qor`) |
| CLI (from LogikBench wrapper) | `tardigrade -f cmd.f -t <top> -l <lib.lib> -s <sdc> -o <out.vg> --qor qor.json [--pdk <lambdapdk name>]` |
| Ecosystem | SiliconCompiler `Task`, LambdaPDK PDK names, OpenSTA for sign-off timing |
| Published results | asap7, slow corner, OpenSTA: PicoRV32 314 -> 1280 MHz, BlackParrot 168 -> 870 MHz, TPU 571 -> 1575 MHz versus Yosys |
| License / access | Not public. Partnerships or royalty-free source licenses by negotiation. No license-token limits. |
| Maturity | Used internally at Zero ASIC. No public version or general-availability date. |

Zero ASIC published the benchmark numbers itself, and nobody has reproduced them independently. They are measured at asap7, a 7 nm predictive PDK. We target Sky130 HD at the typical corner, and Sky130 is in LambdaPDK, so the tool should support it. No Sky130 numbers are published.

## Fit with this project

| Question | Assessment |
| -------- | ---------- |
| Would it accept our RTL? | Very likely. The design is about 270 lines of plain synthesizable Verilog. |
| Does it target our PDK? | Probably. Sky130 is in LambdaPDK, but no published Sky130 results exist. |
| Does it address a current goal? | Yes, in principle. The roadmap asks for maximum clock frequency. |
| Can we measure its benefit today? | No. There is no SDC or STA in our flow. |
| Can CI run it? | No. The binary is unavailable, and if we got it, it would likely come with redistribution limits. |
| Cost to wire in once available | Low, if we adopt SiliconCompiler or LogikBench-style flows first. Otherwise a custom Makefile target plus an SDC. |
| Risk | Vendor lock-in for a closed tool, on a project whose whole flow is open source. Keep Yosys as the reference flow either way. |

## Suggested path

These steps are useful with or without Tardigrade, and they lower its integration cost to almost nothing:

1. **Add an SDC** (`synth/packet_handler.sdc`) with a clock on `i_clk`, plus input and output delays.
2. **Add timing to `make synth`.** Run OpenSTA on the Yosys netlist and report WNS and Fmax. Also hand the clock period to ABC (`abc -D <ps>`) so mapping is timing-aware.
3. **Optionally move the synthesis flow to SiliconCompiler or LogikBench** (`lb syn --target skywater130 --tool yosys`). After that, trying Tardigrade is just `--tool tardigrade`, and the QoR JSON gives a like-for-like comparison.
4. **Look at the architecture.** FIFO depth, an SRAM macro (e.g. OpenRAM / `sky130_sram`), and the read mux / 296-bit output path probably matter more for area and Fmax than the choice of synthesis engine.
5. **Revisit Tardigrade** if Zero ASIC grants access, or if it becomes available through SiliconCompiler. Its Fmax would then be compared against the Yosys baseline from step 2 on the same SDC and corner.
