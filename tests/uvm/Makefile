UVM_URL ?= https://github.com/chipsalliance/uvm-verilator.git
UVM_BRANCH ?= uvm-2020-3.1-vlt
UVM_ROOT = uvm-src/src
XEZIM ?= xezim

all: sim

uvm-src:
	@echo "Downloading standard UVM library (branch $(UVM_BRANCH))..."
	git clone -b $(UVM_BRANCH) $(UVM_URL) uvm-src

sim: uvm-src
	@echo "Running xezim UVM flow..."
	$(XEZIM) --simulate --max-time 10000000 \
		-I$(UVM_ROOT) \
		-D UVM_NO_DPI \
		$(UVM_ROOT)/uvm_pkg.sv \
		../../packet_handler.v \
		tb_top.sv

clean:
	rm -rf uvm-src
