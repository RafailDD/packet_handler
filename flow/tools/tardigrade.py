"""Tardigrade ASIC synthesis task (Zero ASIC).

Tardigrade is not publicly available (see docs/tardigrade_evaluation.md).
This task follows the command line used by Zero ASIC's own LogikBench
wrapper, so `sc_flow.py syn --syn-tool tardigrade` works once the binary is
on PATH; until then SiliconCompiler stops with "tool not found". It has not
been run against a real tardigrade binary.

    tardigrade -f cmd.f -t <top> --pdk <lambdapdk pdk> -l <lib> -s <sdc>
               -o <out.vg> --qor qor.json
"""

import json
import os

from siliconcompiler import Task


class TardigradeSynthesis(Task):

    def tool(self):
        return "tardigrade"

    def task(self):
        return "synthesis"

    def parse_version(self, stdout):
        # 'tardigrade <version>'
        return stdout.split()[1]

    def setup(self):
        super().setup()
        self.set_exe("tardigrade", vswitch="-v")
        self.add_regex("warnings", "Warning:")
        self.add_regex("errors", "^ERROR")
        for lib, key in (self.get_fileset_file_keys("systemverilog")
                         + self.get_fileset_file_keys("verilog")):
            self.add_required_key(lib, *key)
        # Same output name as Yosys, so the OpenSTA node after it is unchanged.
        self.add_output_file(ext="vg")

    def pre_process(self):
        super().pre_process()
        fileset = self.project.get("option", "fileset")[0]
        self.project.design.write_fileset("cmd.f", fileset=fileset)

    def runtime_options(self):
        opts = super().runtime_options()
        design = self.project.design
        fileset = self.project.get("option", "fileset")[0]
        top = design.get_topmodule(fileset)
        opts += ["-f", "cmd.f", "-t", top, "--pdk", self.project.get("asic", "pdk")]
        for sdc in design.get_file(fileset="sdc"):
            opts += ["-s", sdc]
        opts += ["-o", f"outputs/{top}.vg", "--qor", "qor.json"]
        return opts

    def post_process(self):
        super().post_process()
        if not os.path.exists("qor.json"):
            return
        with open("qor.json") as f:
            for metric, value in json.load(f).items():
                if value is not None:
                    self.record_metric(metric, value, source_file="qor.json")
