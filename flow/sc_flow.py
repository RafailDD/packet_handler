#!/usr/bin/env python3
"""SiliconCompiler entry point for every packet_handler flow.

One design description (sources, testbenches, defines, constraints) feeds
every flow, so a new RTL file or define is added in one place:

    python3 flow/sc_flow.py lint                 # slang + Verilator lint
    python3 flow/sc_flow.py sim --tool icarus    # tests/tb_basic_sv.sv
    python3 flow/sc_flow.py sim --tool verilator --tb tb_uvm_sv
    python3 flow/sc_flow.py cocotb               # tests/tb_cocotb.py on Icarus
    python3 flow/sc_flow.py uvm --tool verilator # tests/uvm via its Makefile
    python3 flow/sc_flow.py uvm --tool xezim
    python3 flow/sc_flow.py syn                  # slang -> Yosys -> OpenSTA
    python3 flow/sc_flow.py asic                 # full RTL-to-GDS (OpenROAD)

Add --sram to any flow to build the FIFO around the Sky130 SRAM macro
(PH_SKY130_SRAM) instead of the behavioral memory, and --clk to change the
clock period (ns) used by the SDC. Results land in build/<design>/<jobname>/.

The tools come from the SiliconCompiler runner image (see `make sc_*` in the
top-level Makefile), or from PATH when they are installed locally.
"""

import argparse
import os
import sys

from siliconcompiler import ASIC, Design, Lint, Sim, Flowgraph
from siliconcompiler.flows.dvflow import DVFlow
from siliconcompiler.flows.lintflow import LintFlow
from siliconcompiler.flows.synflow import SynthesisFlow
from siliconcompiler.targets import skywater130_demo
from siliconcompiler.tools.icarus.compile import CompileTask as IcarusCompileTask
from siliconcompiler.tools.verilator.compile import CompileTask as VerilatorCompileTask

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from tools.uvm import UvmVerilatorTask, UvmXezimTask  # noqa: E402
from tools.tardigrade import TardigradeSynthesis  # noqa: E402

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

# Plain testbenches: file under tests/ -> top module name.
TESTBENCHES = {
    "tb_basic_sv": ("tests/tb_basic_sv.sv", "tb_basic_sv"),
    "tb_smoke": ("tests/tb_smoke.v", "tb_smoke"),
    "tb_uvm_sv": ("tests/tb_uvm_sv.sv", "tb_verilator_uvm"),
}


class PacketHandler(Design):
    """packet_handler sources, testbenches and constraints as filesets."""

    def __init__(self, sram: bool = False):
        super().__init__("packet_handler")
        self.set_dataroot("packet_handler", ROOT)

        with self.active_dataroot("packet_handler"):
            # Synthesizable RTL
            with self.active_fileset("rtl"):
                self.set_topmodule("packet_handler")
                self.add_file("packet_handler.v")
                if sram:
                    self.add_define("PH_SKY130_SRAM")

            # Simulation model of the SRAM macro
            with self.active_fileset("sram_model"):
                self.add_file("macros/sky130_sram_model.v")

            # Timing constraints
            with self.active_fileset("sdc"):
                self.add_file("synth/packet_handler.sdc")

            # Plain SystemVerilog / Verilog testbenches
            for name, (path, top) in TESTBENCHES.items():
                with self.active_fileset(name):
                    self.set_topmodule(top)
                    self.add_idir("tests")
                    self.add_file(path)
                    if sram:
                        self.add_define("PH_SKY130_SRAM")

            # cocotb: the DUT is the top level, tests come from tb_cocotb.py
            with self.active_fileset("cocotb"):
                self.set_topmodule("packet_handler")
                self.add_file("packet_handler.v")
                if sram:
                    self.add_define("PH_SKY130_SRAM")
                self.add_file("tests/tb_cocotb.py", filetype="python")


def _sim_filesets(design_sram: bool, tb: str):
    filesets = [tb]
    if tb != "cocotb":
        filesets.append("rtl")
    if design_sram:
        filesets.append("sram_model")
    return filesets


def run_lint(args):
    proj = Lint(PacketHandler(sram=args.sram))
    proj.add_fileset("rtl")
    proj.set_flow(LintFlow(tool="all"))
    return proj


def run_sim(args):
    proj = Sim(PacketHandler(sram=args.sram))
    proj.add_fileset(_sim_filesets(args.sram, args.tb))
    proj.set_flow(DVFlow(tool=args.tool))

    if args.tool == "icarus":
        IcarusCompileTask.find_task(proj).set_icarus_veriloggeneration("2012")
    else:
        compile_task = VerilatorCompileTask.find_task(proj)
        compile_task.set_verilator_main(True)
        # Same warning policy as `make verilator` (-Wall); the RTL is clean.
        compile_task.add_commandline_option("-Wall")
    return proj


def run_cocotb(args):
    proj = Sim(PacketHandler(sram=args.sram))
    proj.add_fileset(_sim_filesets(args.sram, "cocotb"))
    tool = "icarus-cocotb" if args.tool == "icarus" else "verilator-cocotb"
    proj.set_flow(DVFlow(tool=tool))
    if args.tool == "icarus":
        compile_task = IcarusCompileTask.find_task(proj)
        compile_task.set_icarus_veriloggeneration("2012")
        compile_task.set_icarus_timescale("1ns", "1ps")
    return proj


def run_uvm(args):
    proj = Sim(PacketHandler(sram=False))
    proj.add_fileset("rtl")

    flow = Flowgraph(f"uvm-{args.tool}")
    task = UvmVerilatorTask() if args.tool == "verilator" else UvmXezimTask()
    flow.node("simulate", task)
    proj.set_flow(flow)

    task = type(task).find_task(proj)
    task.set("var", "repo", ROOT)
    task.set("var", "testname", args.testname)
    task.set("var", "seed", args.seed)
    task.set("var", "coverage", args.coverage)
    return proj


def _asic_project(args):
    proj = ASIC(PacketHandler(sram=args.sram))
    proj.add_fileset(["rtl", "sdc"])
    skywater130_demo(proj)
    if args.sram:
        # The lambdapdk macro library brings the Liberty, LEF and GDS views.
        from lambdapdk.sky130.libs.sky130sram import Sky130_SRAM_32x512
        proj.add_asiclib(Sky130_SRAM_32x512())
    return proj


def run_syn(args):
    proj = _asic_project(args)
    if args.syn_tool == "tardigrade":
        flow = SynthesisFlow(name="synflow-tardigrade")
        flow.node("synthesis", TardigradeSynthesis())
        proj.set_flow(flow)
    else:
        proj.set_flow("synflow-verilog")
    return proj


def run_asic(args):
    proj = _asic_project(args)
    proj.set_flow("asicflow")
    return proj


FLOWS = {
    "lint": run_lint,
    "sim": run_sim,
    "cocotb": run_cocotb,
    "uvm": run_uvm,
    "syn": run_syn,
    "asic": run_asic,
}


def main():
    parser = argparse.ArgumentParser(
        description="Run a packet_handler flow with SiliconCompiler.")
    parser.add_argument("flow", choices=FLOWS)
    parser.add_argument("--tool", choices=["icarus", "verilator", "xezim"],
                        default=None,
                        help="simulator (sim, cocotb: icarus | verilator; "
                             "uvm: verilator | xezim)")
    parser.add_argument("--tb", choices=list(TESTBENCHES), default="tb_basic_sv",
                        help="testbench for the sim flow")
    parser.add_argument("--testname", default="packet_test", help="UVM test name")
    parser.add_argument("--seed", type=int, default=1, help="UVM random seed")
    parser.add_argument("--coverage", action="store_true",
                        help="collect UVM code coverage (needs verilator_coverage)")
    parser.add_argument("--syn-tool", choices=["yosys", "tardigrade"], default="yosys",
                        help="synthesis engine for the syn flow")
    parser.add_argument("--sram", action="store_true",
                        help="use the Sky130 SRAM macro for the FIFO (PH_SKY130_SRAM)")
    parser.add_argument("--clk", type=float, default=None,
                        help="clock period in ns (exported as CLK_PERIOD_NS for the SDC)")
    parser.add_argument("--jobname", default=None)
    parser.add_argument("--builddir", default=os.path.join(ROOT, "build"))
    args = parser.parse_args()

    if args.tool is None:
        args.tool = "verilator" if args.flow == "uvm" else "icarus"
    if args.flow in ("sim", "cocotb") and args.tool == "xezim":
        parser.error("xezim is only supported by the uvm flow")
    if args.flow == "uvm" and args.tool == "icarus":
        parser.error("Icarus cannot run UVM; use --tool verilator or xezim")
    if args.clk is not None:
        os.environ["CLK_PERIOD_NS"] = str(args.clk)

    proj = FLOWS[args.flow](args)
    proj.set("option", "builddir", args.builddir)
    jobname = args.jobname or "_".join(
        p for p in (args.flow, args.tool if args.flow in ("sim", "cocotb", "uvm") else None,
                    args.tb if args.flow == "sim" else None,
                    args.syn_tool if args.flow == "syn" else None,
                    "sram" if args.sram else None) if p)
    proj.set("option", "jobname", jobname)
    proj.set("option", "quiet", True)

    try:
        proj.run()
    except RuntimeError as e:
        print(f"{args.flow} failed: {e}", file=sys.stderr)
        return 1
    proj.summary()
    return 0


if __name__ == "__main__":
    sys.exit(main())
