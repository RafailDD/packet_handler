// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vpacket_handler.h for the primary calling header

#include "Vpacket_handler__pch.h"
#include "Vpacket_handler___024root.h"
#include "Vpacket_handler___024unit__03a__03aenv__Vclpkg.h"
#include "Vpacket_handler___024unit__03a__03amonitor__Vclpkg.h"
#include "Vpacket_handler___024unit__03a__03ascoreboard__Vclpkg.h"

VL_INLINE_OPT void Vpacket_handler___024root___nba_sequent__TOP__1(Vpacket_handler___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vpacket_handler__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vpacket_handler___024root___nba_sequent__TOP__1\n"); );
    // Init
    CData/*7:0*/ __Vdlyvdim0__tb_verilator_uvm__DOT__dut__DOT__fifo_mem__v0;
    __Vdlyvdim0__tb_verilator_uvm__DOT__dut__DOT__fifo_mem__v0 = 0;
    QData/*32:0*/ __Vdlyvval__tb_verilator_uvm__DOT__dut__DOT__fifo_mem__v0;
    __Vdlyvval__tb_verilator_uvm__DOT__dut__DOT__fifo_mem__v0 = 0;
    CData/*0:0*/ __Vdlyvset__tb_verilator_uvm__DOT__dut__DOT__fifo_mem__v0;
    __Vdlyvset__tb_verilator_uvm__DOT__dut__DOT__fifo_mem__v0 = 0;
    // Body
    __Vdlyvset__tb_verilator_uvm__DOT__dut__DOT__fifo_mem__v0 = 0U;
    vlSelf->__Vdlyvset__tb_verilator_uvm__DOT__dut__DOT__packetTracker__v0 = 0U;
    if (vlSelf->tb_verilator_uvm__DOT__rst_n) {
        if (((IData)(vlSelf->tb_verilator_uvm__DOT__o_valid)
             & (IData)(vlSelf->tb_verilator_uvm__DOT__i_ready))) {
            VL_NULL_CHECK(VL_NULL_CHECK(vlSelf->tb_verilator_uvm__DOT__environment, "tests/tb_uvm_sv.sv", 229)
                          ->__PVT__scb, "tests/tb_uvm_sv.sv", 229)->__VnoInFunc_check_actual(vlSymsp, vlSelf->tb_verilator_uvm__DOT__o_data);
            if (VL_LTES_III(32, 0U, VL_NULL_CHECK(VL_NULL_CHECK(vlSelf->tb_verilator_uvm__DOT__environment, "tests/tb_uvm_sv.sv", 232)
                                                  ->__PVT__scb, "tests/tb_uvm_sv.sv", 232)
                            ->__PVT__expected_q.size())) {
                VL_NULL_CHECK(VL_NULL_CHECK(vlSelf->tb_verilator_uvm__DOT__environment, "tests/tb_uvm_sv.sv", 233)
                              ->__PVT__mon, "tests/tb_uvm_sv.sv", 233)->__VnoInFunc_sample(vlSymsp, 0U, (IData)(vlSelf->tb_verilator_uvm__DOT__dut__DOT__in_packetLost));
            }
        }
    }
    if (vlSelf->tb_verilator_uvm__DOT__dut__DOT__fifo_wr_en) {
        __Vdlyvval__tb_verilator_uvm__DOT__dut__DOT__fifo_mem__v0
            = (((QData)((IData)(vlSelf->tb_verilator_uvm__DOT__i_last))
                << 0x20U) | (QData)((IData)(vlSelf->tb_verilator_uvm__DOT__i_data)));
        __Vdlyvset__tb_verilator_uvm__DOT__dut__DOT__fifo_mem__v0 = 1U;
        __Vdlyvdim0__tb_verilator_uvm__DOT__dut__DOT__fifo_mem__v0
            = vlSelf->tb_verilator_uvm__DOT__dut__DOT__fifo_wr_ptr;
    }
    if (((IData)(vlSelf->tb_verilator_uvm__DOT__dut__DOT__fifo_wr_en)
         & (1U == (IData)(vlSelf->tb_verilator_uvm__DOT__dut__DOT__in_state)))) {
        vlSelf->__Vdlyvval__tb_verilator_uvm__DOT__dut__DOT__packetTracker__v0
            = vlSelf->tb_verilator_uvm__DOT__dut__DOT__in_seqNumber_be;
        vlSelf->__Vdlyvset__tb_verilator_uvm__DOT__dut__DOT__packetTracker__v0 = 1U;
        vlSelf->__Vdlyvdim0__tb_verilator_uvm__DOT__dut__DOT__packetTracker__v0
            = (0x1fU & ((IData)(vlSelf->tb_verilator_uvm__DOT__dut__DOT__in_streamId)
                        - (IData)(1U)));
    }
    if (__Vdlyvset__tb_verilator_uvm__DOT__dut__DOT__fifo_mem__v0) {
        vlSelf->tb_verilator_uvm__DOT__dut__DOT__fifo_mem[__Vdlyvdim0__tb_verilator_uvm__DOT__dut__DOT__fifo_mem__v0]
            = __Vdlyvval__tb_verilator_uvm__DOT__dut__DOT__fifo_mem__v0;
    }
}
