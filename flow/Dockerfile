# Toolchain image for every packet_handler flow: the SiliconCompiler runner
# (Yosys, OpenSTA, OpenROAD, KLayout, Icarus 13, Verilator 5.052, slang,
# siliconcompiler 0.38.10) plus cocotb for the cocotb flows and z3 for
# Verilator constrained randomization in the UVM testbench.
#
#   make sc_image          # docker build -t packet-handler-eda flow/
FROM ghcr.io/siliconcompiler/sc_runner:v0.38.10

RUN pip install --no-cache-dir "cocotb>=2.0,<3" \
 && apt-get update \
 && apt-get install -y --no-install-recommends z3 \
 && rm -rf /var/lib/apt/lists/*
