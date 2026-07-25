// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design internal header
// See Vpacket_handler.h for the primary calling header

#ifndef VERILATED_VPACKET_HANDLER___024ROOT_H_
#define VERILATED_VPACKET_HANDLER___024ROOT_H_  // guard

#include "verilated.h"
#include "verilated_timing.h"
class Vpacket_handler___024unit;
class Vpacket_handler___024unit__03a__03adriver;
class Vpacket_handler___024unit__03a__03adriver__Vclpkg;
class Vpacket_handler___024unit__03a__03aenv;
class Vpacket_handler___024unit__03a__03aenv__Vclpkg;
class Vpacket_handler___024unit__03a__03agenerator;
class Vpacket_handler___024unit__03a__03agenerator__Vclpkg;
class Vpacket_handler___024unit__03a__03amonitor;
class Vpacket_handler___024unit__03a__03amonitor__Vclpkg;
class Vpacket_handler___024unit__03a__03apacket_item;
class Vpacket_handler___024unit__03a__03apacket_item__Vclpkg;
class Vpacket_handler___024unit__03a__03ascoreboard;
class Vpacket_handler___024unit__03a__03ascoreboard__Vclpkg;


class Vpacket_handler__Syms;

class alignas(VL_CACHE_LINE_BYTES) Vpacket_handler___024root final : public VerilatedModule {
  public:
    // CELLS
    Vpacket_handler___024unit* __PVT____024unit;
    Vpacket_handler___024unit__03a__03apacket_item__Vclpkg* __024unit__03a__03apacket_item__Vclpkg;
    Vpacket_handler___024unit__03a__03agenerator__Vclpkg* __024unit__03a__03agenerator__Vclpkg;
    Vpacket_handler___024unit__03a__03adriver__Vclpkg* __024unit__03a__03adriver__Vclpkg;
    Vpacket_handler___024unit__03a__03ascoreboard__Vclpkg* __024unit__03a__03ascoreboard__Vclpkg;
    Vpacket_handler___024unit__03a__03amonitor__Vclpkg* __024unit__03a__03amonitor__Vclpkg;
    Vpacket_handler___024unit__03a__03aenv__Vclpkg* __024unit__03a__03aenv__Vclpkg;

    // DESIGN SPECIFIC STATE
    CData/*0:0*/ tb_verilator_uvm__DOT__clk;
    CData/*0:0*/ tb_verilator_uvm__DOT__rst_n;
    CData/*0:0*/ tb_verilator_uvm__DOT__i_valid;
    CData/*0:0*/ tb_verilator_uvm__DOT__i_last;
    CData/*0:0*/ tb_verilator_uvm__DOT__i_ready;
    CData/*0:0*/ tb_verilator_uvm__DOT__o_valid;
    CData/*7:0*/ tb_verilator_uvm__DOT__dut__DOT__fifo_wr_ptr;
    CData/*7:0*/ tb_verilator_uvm__DOT__dut__DOT__fifo_rd_ptr;
    CData/*0:0*/ tb_verilator_uvm__DOT__dut__DOT__fifo_wr_en;
    CData/*0:0*/ tb_verilator_uvm__DOT__dut__DOT__fifo_rd_en;
    CData/*1:0*/ tb_verilator_uvm__DOT__dut__DOT__in_state;
    CData/*0:0*/ tb_verilator_uvm__DOT__dut__DOT__in_packetLost;
    CData/*4:0*/ tb_verilator_uvm__DOT__dut__DOT__in_stream_idx;
    CData/*3:0*/ tb_verilator_uvm__DOT__dut__DOT__state;
    CData/*3:0*/ tb_verilator_uvm__DOT__dut__DOT__next_state;
    CData/*4:0*/ __Vdlyvdim0__tb_verilator_uvm__DOT__dut__DOT__packetTracker__v0;
    CData/*0:0*/ __Vdlyvset__tb_verilator_uvm__DOT__dut__DOT__packetTracker__v0;
    CData/*7:0*/ __Vdly__tb_verilator_uvm__DOT__dut__DOT__fifo_wr_ptr;
    CData/*1:0*/ __Vdly__tb_verilator_uvm__DOT__dut__DOT__in_state;
    CData/*0:0*/ __VstlFirstIteration;
    CData/*0:0*/ __Vtrigprevexpr___TOP__tb_verilator_uvm__DOT__clk__0;
    CData/*0:0*/ __Vtrigprevexpr___TOP__tb_verilator_uvm__DOT__rst_n__0;
    CData/*0:0*/ __VactContinue;
    SData/*8:0*/ tb_verilator_uvm__DOT__dut__DOT__fifo_count;
    SData/*15:0*/ tb_verilator_uvm__DOT__dut__DOT__in_streamId;
    SData/*8:0*/ __Vdly__tb_verilator_uvm__DOT__dut__DOT__fifo_count;
    IData/*31:0*/ tb_verilator_uvm__DOT__i_data;
    VlWide<10>/*295:0*/ tb_verilator_uvm__DOT__o_data;
    IData/*31:0*/ tb_verilator_uvm__DOT__unnamedblk2__DOT__unnamedblk3__DOT__unnamedblk4__DOT__j;
    IData/*31:0*/ tb_verilator_uvm__DOT__dut__DOT__tracker_valid;
    IData/*31:0*/ tb_verilator_uvm__DOT__dut__DOT__in_seqNumber_be;
    VlWide<10>/*295:0*/ tb_verilator_uvm__DOT__dut__DOT__shiftReg;
    IData/*31:0*/ __Vdlyvval__tb_verilator_uvm__DOT__dut__DOT__packetTracker__v0;
    VlWide<10>/*295:0*/ __Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg;
    IData/*31:0*/ __VactIterCount;
    QData/*32:0*/ tb_verilator_uvm__DOT__dut__DOT____VdfgTmp_h7a21bf1e__0;
    VlUnpacked<QData/*32:0*/, 256> tb_verilator_uvm__DOT__dut__DOT__fifo_mem;
    VlUnpacked<IData/*31:0*/, 32> tb_verilator_uvm__DOT__dut__DOT__packetTracker;
    VlDelayScheduler __VdlySched;
    VlTriggerScheduler __VtrigSched_h3539bd04__0;
    VlTriggerVec<1> __VstlTriggered;
    VlTriggerVec<3> __VactTriggered;
    VlTriggerVec<3> __VnbaTriggered;
    VlClassRef<Vpacket_handler___024unit__03a__03aenv> tb_verilator_uvm__DOT__environment;
    VlClassRef<Vpacket_handler___024unit__03a__03apacket_item> tb_verilator_uvm__DOT__unnamedblk2__DOT__unnamedblk3__DOT__p;

    // INTERNAL VARIABLES
    Vpacket_handler__Syms* const vlSymsp;

    // CONSTRUCTORS
    Vpacket_handler___024root(Vpacket_handler__Syms* symsp, const char* v__name);
    ~Vpacket_handler___024root();
    VL_UNCOPYABLE(Vpacket_handler___024root);

    // INTERNAL METHODS
    void __Vconfigure(bool first);
};


#endif  // guard
