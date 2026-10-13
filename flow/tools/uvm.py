"""UVM simulation tasks.

The UVM testbench in tests/uvm/ has its own Makefile, which pins the UVM
library versions and the simulator flags for xezim and Verilator. These tasks
run that Makefile as a SiliconCompiler node, so the UVM flows share the build
directory, logs, metrics and pass/fail handling of the other flows without
duplicating the compile options.
"""

import os
import re

from siliconcompiler import Task


class _UvmMakeTask(Task):
    _target = None
    _log_prefix = None

    def __init__(self):
        super().__init__()
        self.add_parameter("repo", "str", "repository root")
        self.add_parameter("testname", "str", "UVM test to run", "packet_test")
        self.add_parameter("seed", "int", "random seed", 1)
        self.add_parameter("coverage", "bool",
                           "collect code coverage (Verilator only)", False)

    def tool(self):
        return "make"

    def parse_version(self, stdout):
        # 'GNU Make 4.3'
        return stdout.split()[2]

    def setup(self):
        super().setup()
        self.set_exe("make", vswitch="--version")
        self.add_required_key("var", "repo")
        self.add_required_key("var", "testname")
        # The UVM report summary and the Makefile's own checks
        self.add_regex("errors", r"^UVM_(ERROR|FATAL) :\s+[1-9]")
        self.add_regex("warnings", r"^UVM_WARNING :\s+[1-9]")

    def runtime_options(self):
        opts = super().runtime_options()
        opts += ["-C", os.path.join(self.get("var", "repo"), "tests", "uvm"),
                 self._target,
                 f"UVM_TESTNAME={self.get('var', 'testname')}",
                 f"SEED={self.get('var', 'seed')}",
                 f"COV={1 if self.get('var', 'coverage') else 0}"]
        return opts

    def post_process(self):
        super().post_process()
        log = os.path.join(self.get("var", "repo"), "tests", "uvm", "logs",
                           f"{self._log_prefix}_{self.get('var', 'testname')}.log")
        if not os.path.exists(log):
            return
        with open(log) as f:
            text = f.read()
        for sev in ("ERROR", "FATAL", "WARNING"):
            match = re.search(rf"^UVM_{sev} :\s+(\d+)", text, re.MULTILINE)
            if match and sev != "WARNING":
                # Count UVM errors and fatals as node errors.
                self.record_metric("errors", int(match.group(1)), source_file=log)
            elif match:
                self.record_metric("warnings", int(match.group(1)), source_file=log)


class UvmVerilatorTask(_UvmMakeTask):
    """tests/uvm on Verilator (v5.052+) with the Accellera UVM library."""
    _target = "verilator"
    _log_prefix = "verilator"

    def task(self):
        return "uvm_verilator"


class UvmXezimTask(_UvmMakeTask):
    """tests/uvm on the xezim simulator (xezim must be on PATH)."""
    _target = "sim"
    _log_prefix = "xezim"

    def task(self):
        return "uvm_xezim"
