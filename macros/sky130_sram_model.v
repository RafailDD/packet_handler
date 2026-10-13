//-----------------------------------------------------------------------------
// Simulation model of the Sky130 OpenRAM macro
// sky130_sram_2kbyte_1rw1r_32x512_8, used by ph_fifo_mem when PH_SKY130_SRAM
// is defined. Synthesis never reads this file: the macro is a black box there
// (Liberty view, fetched by `make fetch_sram`).
//
// It follows the read/write timing of OpenRAM's own Verilog model, which is
// not used directly because newer Icarus releases reject it (it uses `mem`
// before declaring it) and it prints every access:
//   * all inputs are registered on the rising clock edge;
//   * writes (port 0, csb0 = web0 = 0) update the array on the falling edge,
//     per byte of wmask0;
//   * reads (port 1, csb1 = 0) drive dout1 on the falling edge;
//   * dout1 goes X on every rising edge (after flops sampling on that edge
//     have seen the old value), so data is valid only from the falling edge
//     of the read cycle to the next rising edge. A design that relies on
//     dout1 holding its value fails in simulation.
// Port 0 reads (csb0 = 0, web0 = 1) are modeled the same way on dout0.
// OpenRAM's arbitrary #delays are left out, so no timescale or Verilator
// --timing is needed.
//-----------------------------------------------------------------------------

/* verilator lint_off DECLFILENAME */
module sky130_sram_2kbyte_1rw1r_32x512_8 #(
    parameter integer NUM_WMASKS = 4,
    parameter integer DATA_WIDTH = 32,
    parameter integer ADDR_WIDTH = 9
) (
`ifdef USE_POWER_PINS
    inout  wire                  vccd1,
    inout  wire                  vssd1,
`endif
    // Port 0: read/write
    input  wire                  clk0,
    input  wire                  csb0,
    input  wire                  web0,
    input  wire [NUM_WMASKS-1:0] wmask0,
    input  wire [ADDR_WIDTH-1:0] addr0,
    input  wire [DATA_WIDTH-1:0] din0,
    output reg  [DATA_WIDTH-1:0] dout0,
    // Port 1: read only
    input  wire                  clk1,
    input  wire                  csb1,
    input  wire [ADDR_WIDTH-1:0] addr1,
    output reg  [DATA_WIDTH-1:0] dout1
);

    reg [DATA_WIDTH-1:0] mem [0:(1<<ADDR_WIDTH)-1];

    reg                  csb0_reg;
    reg                  web0_reg;
    reg [NUM_WMASKS-1:0] wmask0_reg;
    reg [ADDR_WIDTH-1:0] addr0_reg;
    reg [DATA_WIDTH-1:0] din0_reg;
    reg                  csb1_reg;
    reg [ADDR_WIDTH-1:0] addr1_reg;

    integer b;

    // Input registers
    always @(posedge clk0) begin
        csb0_reg   <= csb0;
        web0_reg   <= web0;
        wmask0_reg <= wmask0;
        addr0_reg  <= addr0;
        din0_reg   <= din0;
    end

    always @(posedge clk1) begin
        csb1_reg  <= csb1;
        addr1_reg <= addr1;
    end

    // Port 0: on the rising edge the output goes X (valid for half a cycle
    // only); on the falling edge write, or read into dout0.
    always @(posedge clk0 or negedge clk0) begin
        if (clk0) begin
            dout0 <= {DATA_WIDTH{1'bx}};
        end else begin
            if (!csb0_reg && !web0_reg) begin
                for (b = 0; b < NUM_WMASKS; b = b + 1)
                    if (wmask0_reg[b])
                        mem[addr0_reg][b*8 +: 8] <= din0_reg[b*8 +: 8];
            end
            if (!csb0_reg && web0_reg)
                dout0 <= mem[addr0_reg];
        end
    end

    // Port 1: same output timing, read only.
    always @(posedge clk1 or negedge clk1) begin
        if (clk1)
            dout1 <= {DATA_WIDTH{1'bx}};
        else if (!csb1_reg)
            dout1 <= mem[addr1_reg];
    end

endmodule
/* verilator lint_on DECLFILENAME */
