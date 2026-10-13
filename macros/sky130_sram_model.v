// Simulation wrapper for the Sky130 OpenRAM macro used by ph_fifo_mem when
// PH_SKY130_SRAM is defined. The vendor model has no `timescale and uses
// unitless delays, so pin it to 1ns/1ps here. `make fetch_sram` downloads
// the model into macros/sky130_sram/.
`timescale 1ns/1ps
/* verilator lint_off BLKSEQ */
/* verilator lint_off MULTIDRIVEN */
`include "sky130_sram/sky130_sram_2kbyte_1rw1r_32x512_8.v"
/* verilator lint_on MULTIDRIVEN */
/* verilator lint_on BLKSEQ */
