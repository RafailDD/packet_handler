# Xezim UVM Flow

This directory contains a complete, standard SystemVerilog UVM environment for the `packet_handler` module.

Historically, open-source simulators struggled to compile and run the full IEEE 1800.2 UVM base class library, requiring heavy patching or workarounds. This flow leverages **xezim**, an open-source SystemVerilog simulator written in Rust that natively supports standard UVM, meaning we can run a "proper" UVM testbench without simulator-specific hacks.

## Implementation Overview

The environment is built using standard UVM components:
- **`packet_item`**: The transaction class that holds randomized packet fields (msgLength, streamId, seqNumber, payload).
- **`packet_sequence`**: Generates randomized items and sequences of items.
- **`packet_driver`**: Drives the UVM items onto the physical pins via a virtual interface.
- **`packet_monitor`**: Observes the physical pins and reconstructs the packet transactions, sending them to the scoreboard via an analysis port.
- **`packet_scoreboard`**: Compares the received packets against expected formats and sequence numbers.
- **`packet_agent` / `packet_env`**: Standard UVM hierarchical containers.
- **`packet_test`**: The top-level test that configures the environment and starts sequences on the sequencer.

## Installation and Dependencies

To run this flow, you must have `xezim` installed on your system.

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

## Flow Guide

The local `Makefile` in this directory handles downloading the standard UVM library and running the simulation using `xezim`.

To run the UVM simulation:
```bash
make sim
```

### What happens under the hood?
1. **Fetch UVM**: The Makefile automatically clones the `chipsalliance/uvm-verilator` standard UVM 2020.3.1 branch (which is standard IEEE 1800.2-2020 UVM).
2. **Execute Simulation**: `xezim` is invoked to compile the `packet_handler` RTL along with the UVM library and the `tb_top.sv` testbench.
3. **Execution**: The UVM test runs to completion, driving randomized stimulus and reporting the scoreboard results.

## Cleaning Up
To remove downloaded UVM sources and simulation artifacts, run:
```bash
make clean
```
