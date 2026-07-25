// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vpacket_handler.h for the primary calling header

#include "Vpacket_handler__pch.h"
#include "Vpacket_handler___024root.h"

VlCoroutine Vpacket_handler___024root___eval_initial__TOP__Vtiming__0(Vpacket_handler___024root* vlSelf);
VlCoroutine Vpacket_handler___024root___eval_initial__TOP__Vtiming__1(Vpacket_handler___024root* vlSelf);

void Vpacket_handler___024root___eval_initial(Vpacket_handler___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vpacket_handler__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vpacket_handler___024root___eval_initial\n"); );
    // Body
    Vpacket_handler___024root___eval_initial__TOP__Vtiming__0(vlSelf);
    Vpacket_handler___024root___eval_initial__TOP__Vtiming__1(vlSelf);
    vlSelf->__Vtrigprevexpr___TOP__tb_verilator_uvm__DOT__clk__0
        = vlSelf->tb_verilator_uvm__DOT__clk;
    vlSelf->__Vtrigprevexpr___TOP__tb_verilator_uvm__DOT__rst_n__0
        = vlSelf->tb_verilator_uvm__DOT__rst_n;
}

VL_INLINE_OPT VlCoroutine Vpacket_handler___024root___eval_initial__TOP__Vtiming__0(Vpacket_handler___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vpacket_handler__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vpacket_handler___024root___eval_initial__TOP__Vtiming__0\n"); );
    // Body
    vlSelf->tb_verilator_uvm__DOT__clk = 0U;
    while (1U) {
        co_await vlSelf->__VdlySched.delay(5ULL, nullptr,
                                           "tests/tb_uvm_sv.sv",
                                           184);
        vlSelf->tb_verilator_uvm__DOT__clk = (1U &
                                              (~ (IData)(vlSelf->tb_verilator_uvm__DOT__clk)));
    }
}

extern const VlUnpacked<CData/*3:0*/, 128> Vpacket_handler__ConstPool__TABLE_hba08ba0f_0;

VL_INLINE_OPT void Vpacket_handler___024root___act_comb__TOP__0(Vpacket_handler___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vpacket_handler__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vpacket_handler___024root___act_comb__TOP__0\n"); );
    // Init
    CData/*6:0*/ __Vtableidx1;
    __Vtableidx1 = 0;
    // Body
    vlSelf->tb_verilator_uvm__DOT__dut__DOT__in_seqNumber_be
        = ((vlSelf->tb_verilator_uvm__DOT__i_data << 0x18U)
           | ((0xff0000U & (vlSelf->tb_verilator_uvm__DOT__i_data
                            << 8U)) | ((0xff00U & (vlSelf->tb_verilator_uvm__DOT__i_data
                                                   >> 8U))
                                       | (vlSelf->tb_verilator_uvm__DOT__i_data
                                          >> 0x18U))));
    vlSelf->tb_verilator_uvm__DOT__dut__DOT__fifo_wr_en
        = ((~ ((IData)(vlSelf->tb_verilator_uvm__DOT__dut__DOT__fifo_count)
               >> 8U)) & (IData)(vlSelf->tb_verilator_uvm__DOT__i_valid));
    __Vtableidx1 = (((0U != (IData)(vlSelf->tb_verilator_uvm__DOT__dut__DOT__fifo_count))
                     << 6U) | ((((0U != (IData)(vlSelf->tb_verilator_uvm__DOT__dut__DOT__fifo_count))
                                 & (IData)((vlSelf->tb_verilator_uvm__DOT__dut__DOT____VdfgTmp_h7a21bf1e__0
                                            >> 0x20U)))
                                << 5U) | (((IData)(vlSelf->tb_verilator_uvm__DOT__i_ready)
                                           << 4U) | (IData)(vlSelf->tb_verilator_uvm__DOT__dut__DOT__state))));
    vlSelf->tb_verilator_uvm__DOT__dut__DOT__next_state
        = Vpacket_handler__ConstPool__TABLE_hba08ba0f_0
        [__Vtableidx1];
}

void Vpacket_handler___024root___eval_act(Vpacket_handler___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vpacket_handler__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vpacket_handler___024root___eval_act\n"); );
    // Body
    if ((5ULL & vlSelf->__VactTriggered.word(0U))) {
        Vpacket_handler___024root___act_comb__TOP__0(vlSelf);
    }
}

VL_INLINE_OPT void Vpacket_handler___024root___nba_sequent__TOP__0(Vpacket_handler___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vpacket_handler__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vpacket_handler___024root___nba_sequent__TOP__0\n"); );
    // Body
    vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__fifo_wr_ptr
        = vlSelf->tb_verilator_uvm__DOT__dut__DOT__fifo_wr_ptr;
    vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__in_state
        = vlSelf->tb_verilator_uvm__DOT__dut__DOT__in_state;
    vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__fifo_count
        = vlSelf->tb_verilator_uvm__DOT__dut__DOT__fifo_count;
    vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[0U]
        = vlSelf->tb_verilator_uvm__DOT__dut__DOT__shiftReg[0U];
    vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[1U]
        = vlSelf->tb_verilator_uvm__DOT__dut__DOT__shiftReg[1U];
    vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[2U]
        = vlSelf->tb_verilator_uvm__DOT__dut__DOT__shiftReg[2U];
    vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[3U]
        = vlSelf->tb_verilator_uvm__DOT__dut__DOT__shiftReg[3U];
    vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[4U]
        = vlSelf->tb_verilator_uvm__DOT__dut__DOT__shiftReg[4U];
    vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[5U]
        = vlSelf->tb_verilator_uvm__DOT__dut__DOT__shiftReg[5U];
    vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[6U]
        = vlSelf->tb_verilator_uvm__DOT__dut__DOT__shiftReg[6U];
    vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[7U]
        = vlSelf->tb_verilator_uvm__DOT__dut__DOT__shiftReg[7U];
    vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[8U]
        = vlSelf->tb_verilator_uvm__DOT__dut__DOT__shiftReg[8U];
    vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[9U]
        = vlSelf->tb_verilator_uvm__DOT__dut__DOT__shiftReg[9U];
    if (vlSelf->tb_verilator_uvm__DOT__rst_n) {
        if (vlSelf->tb_verilator_uvm__DOT__dut__DOT__fifo_wr_en) {
            vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__fifo_wr_ptr
                = (0xffU & ((IData)(1U) + (IData)(vlSelf->tb_verilator_uvm__DOT__dut__DOT__fifo_wr_ptr)));
        }
        if (vlSelf->tb_verilator_uvm__DOT__dut__DOT__fifo_rd_en) {
            vlSelf->tb_verilator_uvm__DOT__dut__DOT__fifo_rd_ptr
                = (0xffU & ((IData)(1U) + (IData)(vlSelf->tb_verilator_uvm__DOT__dut__DOT__fifo_rd_ptr)));
        }
        vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__fifo_count
            = (0x1ffU & ((2U == (((IData)(vlSelf->tb_verilator_uvm__DOT__dut__DOT__fifo_wr_en)
                                  << 1U) | (IData)(vlSelf->tb_verilator_uvm__DOT__dut__DOT__fifo_rd_en)))
                          ? ((IData)(1U) + (IData)(vlSelf->tb_verilator_uvm__DOT__dut__DOT__fifo_count))
                          : ((1U == (((IData)(vlSelf->tb_verilator_uvm__DOT__dut__DOT__fifo_wr_en)
                                      << 1U) | (IData)(vlSelf->tb_verilator_uvm__DOT__dut__DOT__fifo_rd_en)))
                              ? ((IData)(vlSelf->tb_verilator_uvm__DOT__dut__DOT__fifo_count)
                                 - (IData)(1U)) : (IData)(vlSelf->tb_verilator_uvm__DOT__dut__DOT__fifo_count))));
    } else {
        vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__fifo_wr_ptr = 0U;
        vlSelf->tb_verilator_uvm__DOT__dut__DOT__fifo_rd_ptr = 0U;
        vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__fifo_count = 0U;
    }
}

extern const VlWide<10>/*319:0*/ Vpacket_handler__ConstPool__CONST_ha8c708d0_0;
extern const VlWide<9>/*287:0*/ Vpacket_handler__ConstPool__CONST_h52851867_0;

VL_INLINE_OPT void Vpacket_handler___024root___nba_sequent__TOP__2(Vpacket_handler___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vpacket_handler__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vpacket_handler___024root___nba_sequent__TOP__2\n"); );
    // Body
    vlSelf->tb_verilator_uvm__DOT__dut__DOT__fifo_wr_ptr
        = vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__fifo_wr_ptr;
    if (vlSelf->tb_verilator_uvm__DOT__rst_n) {
        if ((8U & (IData)(vlSelf->tb_verilator_uvm__DOT__dut__DOT__state))) {
            if ((4U & (IData)(vlSelf->tb_verilator_uvm__DOT__dut__DOT__state))) {
                vlSelf->tb_verilator_uvm__DOT__o_valid = 0U;
                vlSelf->tb_verilator_uvm__DOT__o_data[0U]
                    = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[0U];
                vlSelf->tb_verilator_uvm__DOT__o_data[1U]
                    = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[1U];
                vlSelf->tb_verilator_uvm__DOT__o_data[2U]
                    = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[2U];
                vlSelf->tb_verilator_uvm__DOT__o_data[3U]
                    = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[3U];
                vlSelf->tb_verilator_uvm__DOT__o_data[4U]
                    = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[4U];
                vlSelf->tb_verilator_uvm__DOT__o_data[5U]
                    = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[5U];
                vlSelf->tb_verilator_uvm__DOT__o_data[6U]
                    = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[6U];
                vlSelf->tb_verilator_uvm__DOT__o_data[7U]
                    = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[7U];
                vlSelf->tb_verilator_uvm__DOT__o_data[8U]
                    = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[8U];
                vlSelf->tb_verilator_uvm__DOT__o_data[9U]
                    = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[9U];
                vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[0U]
                    = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[0U];
                vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[1U]
                    = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[1U];
                vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[2U]
                    = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[2U];
                vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[3U]
                    = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[3U];
                vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[4U]
                    = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[4U];
                vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[5U]
                    = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[5U];
                vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[6U]
                    = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[6U];
                vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[7U]
                    = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[7U];
                vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[8U]
                    = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[8U];
                vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[9U]
                    = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[9U];
            } else if ((2U & (IData)(vlSelf->tb_verilator_uvm__DOT__dut__DOT__state))) {
                vlSelf->tb_verilator_uvm__DOT__o_valid = 0U;
                vlSelf->tb_verilator_uvm__DOT__o_data[0U]
                    = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[0U];
                vlSelf->tb_verilator_uvm__DOT__o_data[1U]
                    = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[1U];
                vlSelf->tb_verilator_uvm__DOT__o_data[2U]
                    = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[2U];
                vlSelf->tb_verilator_uvm__DOT__o_data[3U]
                    = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[3U];
                vlSelf->tb_verilator_uvm__DOT__o_data[4U]
                    = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[4U];
                vlSelf->tb_verilator_uvm__DOT__o_data[5U]
                    = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[5U];
                vlSelf->tb_verilator_uvm__DOT__o_data[6U]
                    = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[6U];
                vlSelf->tb_verilator_uvm__DOT__o_data[7U]
                    = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[7U];
                vlSelf->tb_verilator_uvm__DOT__o_data[8U]
                    = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[8U];
                vlSelf->tb_verilator_uvm__DOT__o_data[9U]
                    = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[9U];
                vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[0U]
                    = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[0U];
                vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[1U]
                    = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[1U];
                vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[2U]
                    = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[2U];
                vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[3U]
                    = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[3U];
                vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[4U]
                    = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[4U];
                vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[5U]
                    = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[5U];
                vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[6U]
                    = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[6U];
                vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[7U]
                    = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[7U];
                vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[8U]
                    = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[8U];
                vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[9U]
                    = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[9U];
            } else if ((1U & (IData)(vlSelf->tb_verilator_uvm__DOT__dut__DOT__state))) {
                vlSelf->tb_verilator_uvm__DOT__o_valid = 0U;
                vlSelf->tb_verilator_uvm__DOT__o_data[0U]
                    = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[0U];
                vlSelf->tb_verilator_uvm__DOT__o_data[1U]
                    = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[1U];
                vlSelf->tb_verilator_uvm__DOT__o_data[2U]
                    = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[2U];
                vlSelf->tb_verilator_uvm__DOT__o_data[3U]
                    = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[3U];
                vlSelf->tb_verilator_uvm__DOT__o_data[4U]
                    = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[4U];
                vlSelf->tb_verilator_uvm__DOT__o_data[5U]
                    = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[5U];
                vlSelf->tb_verilator_uvm__DOT__o_data[6U]
                    = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[6U];
                vlSelf->tb_verilator_uvm__DOT__o_data[7U]
                    = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[7U];
                vlSelf->tb_verilator_uvm__DOT__o_data[8U]
                    = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[8U];
                vlSelf->tb_verilator_uvm__DOT__o_data[9U]
                    = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[9U];
                vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[0U]
                    = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[0U];
                vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[1U]
                    = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[1U];
                vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[2U]
                    = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[2U];
                vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[3U]
                    = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[3U];
                vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[4U]
                    = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[4U];
                vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[5U]
                    = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[5U];
                vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[6U]
                    = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[6U];
                vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[7U]
                    = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[7U];
                vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[8U]
                    = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[8U];
                vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[9U]
                    = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[9U];
            } else if (vlSelf->tb_verilator_uvm__DOT__i_ready) {
                vlSelf->tb_verilator_uvm__DOT__o_valid = 0U;
            }
        } else {
            vlSelf->tb_verilator_uvm__DOT__o_valid
                = ((1U & ((IData)(vlSelf->tb_verilator_uvm__DOT__dut__DOT__state)
                          >> 2U)) && ((1U & (~ ((IData)(vlSelf->tb_verilator_uvm__DOT__dut__DOT__state)
                                                >> 1U)))
                                      && ((1U & (~ (IData)(vlSelf->tb_verilator_uvm__DOT__dut__DOT__state)))
                                          && ((0U != (IData)(vlSelf->tb_verilator_uvm__DOT__dut__DOT__fifo_count))
                                              & (IData)(
                                                        (vlSelf->tb_verilator_uvm__DOT__dut__DOT____VdfgTmp_h7a21bf1e__0
                                                         >> 0x20U))))));
            if ((4U & (IData)(vlSelf->tb_verilator_uvm__DOT__dut__DOT__state))) {
                if ((2U & (IData)(vlSelf->tb_verilator_uvm__DOT__dut__DOT__state))) {
                    vlSelf->tb_verilator_uvm__DOT__o_data[0U]
                        = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[0U];
                    vlSelf->tb_verilator_uvm__DOT__o_data[1U]
                        = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[1U];
                    vlSelf->tb_verilator_uvm__DOT__o_data[2U]
                        = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[2U];
                    vlSelf->tb_verilator_uvm__DOT__o_data[3U]
                        = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[3U];
                    vlSelf->tb_verilator_uvm__DOT__o_data[4U]
                        = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[4U];
                    vlSelf->tb_verilator_uvm__DOT__o_data[5U]
                        = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[5U];
                    vlSelf->tb_verilator_uvm__DOT__o_data[6U]
                        = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[6U];
                    vlSelf->tb_verilator_uvm__DOT__o_data[7U]
                        = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[7U];
                    vlSelf->tb_verilator_uvm__DOT__o_data[8U]
                        = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[8U];
                    vlSelf->tb_verilator_uvm__DOT__o_data[9U]
                        = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[9U];
                    vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[0U]
                        = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[0U];
                    vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[1U]
                        = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[1U];
                    vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[2U]
                        = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[2U];
                    vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[3U]
                        = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[3U];
                    vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[4U]
                        = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[4U];
                    vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[5U]
                        = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[5U];
                    vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[6U]
                        = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[6U];
                    vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[7U]
                        = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[7U];
                    vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[8U]
                        = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[8U];
                    vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[9U]
                        = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[9U];
                } else if ((1U & (IData)(vlSelf->tb_verilator_uvm__DOT__dut__DOT__state))) {
                    vlSelf->tb_verilator_uvm__DOT__o_data[0U]
                        = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[0U];
                    vlSelf->tb_verilator_uvm__DOT__o_data[1U]
                        = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[1U];
                    vlSelf->tb_verilator_uvm__DOT__o_data[2U]
                        = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[2U];
                    vlSelf->tb_verilator_uvm__DOT__o_data[3U]
                        = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[3U];
                    vlSelf->tb_verilator_uvm__DOT__o_data[4U]
                        = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[4U];
                    vlSelf->tb_verilator_uvm__DOT__o_data[5U]
                        = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[5U];
                    vlSelf->tb_verilator_uvm__DOT__o_data[6U]
                        = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[6U];
                    vlSelf->tb_verilator_uvm__DOT__o_data[7U]
                        = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[7U];
                    vlSelf->tb_verilator_uvm__DOT__o_data[8U]
                        = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[8U];
                    vlSelf->tb_verilator_uvm__DOT__o_data[9U]
                        = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[9U];
                    vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[0U]
                        = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[0U];
                    vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[1U]
                        = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[1U];
                    vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[2U]
                        = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[2U];
                    vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[3U]
                        = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[3U];
                    vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[4U]
                        = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[4U];
                    vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[5U]
                        = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[5U];
                    vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[6U]
                        = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[6U];
                    vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[7U]
                        = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[7U];
                    vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[8U]
                        = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[8U];
                    vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[9U]
                        = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[9U];
                } else {
                    if ((0U != (IData)(vlSelf->tb_verilator_uvm__DOT__dut__DOT__fifo_count))) {
                        vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[0U]
                            = (IData)(vlSelf->tb_verilator_uvm__DOT__dut__DOT____VdfgTmp_h7a21bf1e__0);
                        vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[1U]
                            = (Vpacket_handler__ConstPool__CONST_h52851867_0[0U]
                               & vlSelf->tb_verilator_uvm__DOT__dut__DOT__shiftReg[0U]);
                        vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[2U]
                            = (Vpacket_handler__ConstPool__CONST_h52851867_0[1U]
                               & vlSelf->tb_verilator_uvm__DOT__dut__DOT__shiftReg[1U]);
                        vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[3U]
                            = (Vpacket_handler__ConstPool__CONST_h52851867_0[2U]
                               & vlSelf->tb_verilator_uvm__DOT__dut__DOT__shiftReg[2U]);
                        vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[4U]
                            = (Vpacket_handler__ConstPool__CONST_h52851867_0[3U]
                               & vlSelf->tb_verilator_uvm__DOT__dut__DOT__shiftReg[3U]);
                        vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[5U]
                            = (Vpacket_handler__ConstPool__CONST_h52851867_0[4U]
                               & vlSelf->tb_verilator_uvm__DOT__dut__DOT__shiftReg[4U]);
                        vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[6U]
                            = (Vpacket_handler__ConstPool__CONST_h52851867_0[5U]
                               & vlSelf->tb_verilator_uvm__DOT__dut__DOT__shiftReg[5U]);
                        vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[7U]
                            = (Vpacket_handler__ConstPool__CONST_h52851867_0[6U]
                               & vlSelf->tb_verilator_uvm__DOT__dut__DOT__shiftReg[6U]);
                        vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[8U]
                            = (Vpacket_handler__ConstPool__CONST_h52851867_0[7U]
                               & vlSelf->tb_verilator_uvm__DOT__dut__DOT__shiftReg[7U]);
                        vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[9U]
                            = (Vpacket_handler__ConstPool__CONST_h52851867_0[8U]
                               & vlSelf->tb_verilator_uvm__DOT__dut__DOT__shiftReg[8U]);
                    }
                    if (((0U != (IData)(vlSelf->tb_verilator_uvm__DOT__dut__DOT__fifo_count))
                         & (IData)((vlSelf->tb_verilator_uvm__DOT__dut__DOT____VdfgTmp_h7a21bf1e__0
                                    >> 0x20U)))) {
                        vlSelf->tb_verilator_uvm__DOT__o_data[0U]
                            = (IData)(vlSelf->tb_verilator_uvm__DOT__dut__DOT____VdfgTmp_h7a21bf1e__0);
                        vlSelf->tb_verilator_uvm__DOT__o_data[1U]
                            = (Vpacket_handler__ConstPool__CONST_h52851867_0[0U]
                               & vlSelf->tb_verilator_uvm__DOT__dut__DOT__shiftReg[0U]);
                        vlSelf->tb_verilator_uvm__DOT__o_data[2U]
                            = (Vpacket_handler__ConstPool__CONST_h52851867_0[1U]
                               & vlSelf->tb_verilator_uvm__DOT__dut__DOT__shiftReg[1U]);
                        vlSelf->tb_verilator_uvm__DOT__o_data[3U]
                            = (Vpacket_handler__ConstPool__CONST_h52851867_0[2U]
                               & vlSelf->tb_verilator_uvm__DOT__dut__DOT__shiftReg[2U]);
                        vlSelf->tb_verilator_uvm__DOT__o_data[4U]
                            = (Vpacket_handler__ConstPool__CONST_h52851867_0[3U]
                               & vlSelf->tb_verilator_uvm__DOT__dut__DOT__shiftReg[3U]);
                        vlSelf->tb_verilator_uvm__DOT__o_data[5U]
                            = (Vpacket_handler__ConstPool__CONST_h52851867_0[4U]
                               & vlSelf->tb_verilator_uvm__DOT__dut__DOT__shiftReg[4U]);
                        vlSelf->tb_verilator_uvm__DOT__o_data[6U]
                            = (Vpacket_handler__ConstPool__CONST_h52851867_0[5U]
                               & vlSelf->tb_verilator_uvm__DOT__dut__DOT__shiftReg[5U]);
                        vlSelf->tb_verilator_uvm__DOT__o_data[7U]
                            = (Vpacket_handler__ConstPool__CONST_h52851867_0[6U]
                               & vlSelf->tb_verilator_uvm__DOT__dut__DOT__shiftReg[6U]);
                        vlSelf->tb_verilator_uvm__DOT__o_data[8U]
                            = (Vpacket_handler__ConstPool__CONST_h52851867_0[7U]
                               & vlSelf->tb_verilator_uvm__DOT__dut__DOT__shiftReg[7U]);
                        vlSelf->tb_verilator_uvm__DOT__o_data[9U]
                            = (Vpacket_handler__ConstPool__CONST_h52851867_0[8U]
                               & vlSelf->tb_verilator_uvm__DOT__dut__DOT__shiftReg[8U]);
                    }
                }
            } else if ((2U & (IData)(vlSelf->tb_verilator_uvm__DOT__dut__DOT__state))) {
                if ((1U & (IData)(vlSelf->tb_verilator_uvm__DOT__dut__DOT__state))) {
                    vlSelf->tb_verilator_uvm__DOT__o_data[0U]
                        = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[0U];
                    vlSelf->tb_verilator_uvm__DOT__o_data[1U]
                        = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[1U];
                    vlSelf->tb_verilator_uvm__DOT__o_data[2U]
                        = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[2U];
                    vlSelf->tb_verilator_uvm__DOT__o_data[3U]
                        = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[3U];
                    vlSelf->tb_verilator_uvm__DOT__o_data[4U]
                        = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[4U];
                    vlSelf->tb_verilator_uvm__DOT__o_data[5U]
                        = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[5U];
                    vlSelf->tb_verilator_uvm__DOT__o_data[6U]
                        = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[6U];
                    vlSelf->tb_verilator_uvm__DOT__o_data[7U]
                        = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[7U];
                    vlSelf->tb_verilator_uvm__DOT__o_data[8U]
                        = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[8U];
                    vlSelf->tb_verilator_uvm__DOT__o_data[9U]
                        = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[9U];
                    vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[0U]
                        = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[0U];
                    vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[1U]
                        = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[1U];
                    vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[2U]
                        = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[2U];
                    vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[3U]
                        = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[3U];
                    vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[4U]
                        = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[4U];
                    vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[5U]
                        = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[5U];
                    vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[6U]
                        = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[6U];
                    vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[7U]
                        = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[7U];
                    vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[8U]
                        = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[8U];
                    vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[9U]
                        = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[9U];
                }
            } else if ((1U & (IData)(vlSelf->tb_verilator_uvm__DOT__dut__DOT__state))) {
                vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[0U]
                    = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[0U];
                vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[1U]
                    = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[1U];
                vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[2U]
                    = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[2U];
                vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[3U]
                    = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[3U];
                vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[4U]
                    = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[4U];
                vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[5U]
                    = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[5U];
                vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[6U]
                    = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[6U];
                vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[7U]
                    = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[7U];
                vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[8U]
                    = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[8U];
                vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[9U]
                    = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[9U];
            } else {
                vlSelf->tb_verilator_uvm__DOT__o_data[0U]
                    = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[0U];
                vlSelf->tb_verilator_uvm__DOT__o_data[1U]
                    = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[1U];
                vlSelf->tb_verilator_uvm__DOT__o_data[2U]
                    = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[2U];
                vlSelf->tb_verilator_uvm__DOT__o_data[3U]
                    = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[3U];
                vlSelf->tb_verilator_uvm__DOT__o_data[4U]
                    = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[4U];
                vlSelf->tb_verilator_uvm__DOT__o_data[5U]
                    = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[5U];
                vlSelf->tb_verilator_uvm__DOT__o_data[6U]
                    = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[6U];
                vlSelf->tb_verilator_uvm__DOT__o_data[7U]
                    = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[7U];
                vlSelf->tb_verilator_uvm__DOT__o_data[8U]
                    = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[8U];
                vlSelf->tb_verilator_uvm__DOT__o_data[9U]
                    = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[9U];
                vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[0U]
                    = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[0U];
                vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[1U]
                    = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[1U];
                vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[2U]
                    = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[2U];
                vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[3U]
                    = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[3U];
                vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[4U]
                    = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[4U];
                vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[5U]
                    = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[5U];
                vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[6U]
                    = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[6U];
                vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[7U]
                    = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[7U];
                vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[8U]
                    = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[8U];
                vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[9U]
                    = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[9U];
            }
        }
        vlSelf->tb_verilator_uvm__DOT__dut__DOT__in_packetLost = 0U;
        if (vlSelf->tb_verilator_uvm__DOT__dut__DOT__fifo_wr_en) {
            if ((0U == (IData)(vlSelf->tb_verilator_uvm__DOT__dut__DOT__in_state))) {
                vlSelf->tb_verilator_uvm__DOT__dut__DOT__in_streamId
                    = ((0xff00U & (vlSelf->tb_verilator_uvm__DOT__i_data
                                   << 8U)) | (0xffU
                                              & (vlSelf->tb_verilator_uvm__DOT__i_data
                                                 >> 8U)));
                vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__in_state = 1U;
            } else if ((1U == (IData)(vlSelf->tb_verilator_uvm__DOT__dut__DOT__in_state))) {
                if (((vlSelf->tb_verilator_uvm__DOT__dut__DOT__tracker_valid
                      >> (IData)(vlSelf->tb_verilator_uvm__DOT__dut__DOT__in_stream_idx))
                     & (((IData)(1U) + vlSelf->tb_verilator_uvm__DOT__dut__DOT__packetTracker
                         [vlSelf->tb_verilator_uvm__DOT__dut__DOT__in_stream_idx])
                        != vlSelf->tb_verilator_uvm__DOT__dut__DOT__in_seqNumber_be))) {
                    vlSelf->tb_verilator_uvm__DOT__dut__DOT__in_packetLost = 1U;
                }
                vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__in_state
                    = ((IData)(vlSelf->tb_verilator_uvm__DOT__i_last)
                        ? 0U : 2U);
                vlSelf->tb_verilator_uvm__DOT__dut__DOT__tracker_valid
                    = (vlSelf->tb_verilator_uvm__DOT__dut__DOT__tracker_valid
                       | (0xffffffffULL & ((IData)(1U)
                                           << (IData)(vlSelf->tb_verilator_uvm__DOT__dut__DOT__in_stream_idx))));
            } else if ((2U == (IData)(vlSelf->tb_verilator_uvm__DOT__dut__DOT__in_state))) {
                if (vlSelf->tb_verilator_uvm__DOT__i_last) {
                    vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__in_state = 0U;
                }
            } else {
                vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__in_state = 0U;
            }
        }
        vlSelf->tb_verilator_uvm__DOT__dut__DOT__state
            = vlSelf->tb_verilator_uvm__DOT__dut__DOT__next_state;
    } else {
        vlSelf->tb_verilator_uvm__DOT__o_valid = 0U;
        vlSelf->tb_verilator_uvm__DOT__o_data[0U] =
            Vpacket_handler__ConstPool__CONST_ha8c708d0_0[0U];
        vlSelf->tb_verilator_uvm__DOT__o_data[1U] =
            Vpacket_handler__ConstPool__CONST_ha8c708d0_0[1U];
        vlSelf->tb_verilator_uvm__DOT__o_data[2U] =
            Vpacket_handler__ConstPool__CONST_ha8c708d0_0[2U];
        vlSelf->tb_verilator_uvm__DOT__o_data[3U] =
            Vpacket_handler__ConstPool__CONST_ha8c708d0_0[3U];
        vlSelf->tb_verilator_uvm__DOT__o_data[4U] =
            Vpacket_handler__ConstPool__CONST_ha8c708d0_0[4U];
        vlSelf->tb_verilator_uvm__DOT__o_data[5U] =
            Vpacket_handler__ConstPool__CONST_ha8c708d0_0[5U];
        vlSelf->tb_verilator_uvm__DOT__o_data[6U] =
            Vpacket_handler__ConstPool__CONST_ha8c708d0_0[6U];
        vlSelf->tb_verilator_uvm__DOT__o_data[7U] =
            Vpacket_handler__ConstPool__CONST_ha8c708d0_0[7U];
        vlSelf->tb_verilator_uvm__DOT__o_data[8U] =
            Vpacket_handler__ConstPool__CONST_ha8c708d0_0[8U];
        vlSelf->tb_verilator_uvm__DOT__o_data[9U] =
            Vpacket_handler__ConstPool__CONST_ha8c708d0_0[9U];
        vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[0U]
            = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[0U];
        vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[1U]
            = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[1U];
        vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[2U]
            = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[2U];
        vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[3U]
            = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[3U];
        vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[4U]
            = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[4U];
        vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[5U]
            = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[5U];
        vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[6U]
            = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[6U];
        vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[7U]
            = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[7U];
        vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[8U]
            = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[8U];
        vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[9U]
            = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[9U];
        vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__in_state = 0U;
        vlSelf->tb_verilator_uvm__DOT__dut__DOT__in_streamId = 0U;
        vlSelf->tb_verilator_uvm__DOT__dut__DOT__in_packetLost = 0U;
        vlSelf->tb_verilator_uvm__DOT__dut__DOT__tracker_valid = 0U;
        vlSelf->tb_verilator_uvm__DOT__dut__DOT__state = 1U;
    }
    vlSelf->tb_verilator_uvm__DOT__dut__DOT__shiftReg[0U]
        = vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[0U];
    vlSelf->tb_verilator_uvm__DOT__dut__DOT__shiftReg[1U]
        = vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[1U];
    vlSelf->tb_verilator_uvm__DOT__dut__DOT__shiftReg[2U]
        = vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[2U];
    vlSelf->tb_verilator_uvm__DOT__dut__DOT__shiftReg[3U]
        = vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[3U];
    vlSelf->tb_verilator_uvm__DOT__dut__DOT__shiftReg[4U]
        = vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[4U];
    vlSelf->tb_verilator_uvm__DOT__dut__DOT__shiftReg[5U]
        = vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[5U];
    vlSelf->tb_verilator_uvm__DOT__dut__DOT__shiftReg[6U]
        = vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[6U];
    vlSelf->tb_verilator_uvm__DOT__dut__DOT__shiftReg[7U]
        = vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[7U];
    vlSelf->tb_verilator_uvm__DOT__dut__DOT__shiftReg[8U]
        = vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[8U];
    vlSelf->tb_verilator_uvm__DOT__dut__DOT__shiftReg[9U]
        = vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg[9U];
    vlSelf->tb_verilator_uvm__DOT__dut__DOT__fifo_count
        = vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__fifo_count;
    vlSelf->tb_verilator_uvm__DOT__dut__DOT__in_state
        = vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__in_state;
    vlSelf->tb_verilator_uvm__DOT__dut__DOT__in_stream_idx
        = (0x1fU & ((IData)(vlSelf->tb_verilator_uvm__DOT__dut__DOT__in_streamId)
                    - (IData)(1U)));
    vlSelf->tb_verilator_uvm__DOT__dut__DOT__fifo_rd_en
        = ((0U != (IData)(vlSelf->tb_verilator_uvm__DOT__dut__DOT__fifo_count))
           & ((1U == (IData)(vlSelf->tb_verilator_uvm__DOT__dut__DOT__state))
              | ((2U == (IData)(vlSelf->tb_verilator_uvm__DOT__dut__DOT__state))
                 | (4U == (IData)(vlSelf->tb_verilator_uvm__DOT__dut__DOT__state)))));
}

VL_INLINE_OPT void Vpacket_handler___024root___nba_comb__TOP__0(Vpacket_handler___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vpacket_handler__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vpacket_handler___024root___nba_comb__TOP__0\n"); );
    // Body
    vlSelf->tb_verilator_uvm__DOT__dut__DOT____VdfgTmp_h7a21bf1e__0
        = vlSelf->tb_verilator_uvm__DOT__dut__DOT__fifo_mem
        [vlSelf->tb_verilator_uvm__DOT__dut__DOT__fifo_rd_ptr];
}

VL_INLINE_OPT void Vpacket_handler___024root___nba_comb__TOP__1(Vpacket_handler___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vpacket_handler__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vpacket_handler___024root___nba_comb__TOP__1\n"); );
    // Body
    vlSelf->tb_verilator_uvm__DOT__dut__DOT__in_seqNumber_be
        = ((vlSelf->tb_verilator_uvm__DOT__i_data << 0x18U)
           | ((0xff0000U & (vlSelf->tb_verilator_uvm__DOT__i_data
                            << 8U)) | ((0xff00U & (vlSelf->tb_verilator_uvm__DOT__i_data
                                                   >> 8U))
                                       | (vlSelf->tb_verilator_uvm__DOT__i_data
                                          >> 0x18U))));
}

VL_INLINE_OPT void Vpacket_handler___024root___nba_sequent__TOP__3(Vpacket_handler___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vpacket_handler__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vpacket_handler___024root___nba_sequent__TOP__3\n"); );
    // Body
    if (vlSelf->__Vdlyvset__tb_verilator_uvm__DOT__dut__DOT__packetTracker__v0) {
        vlSelf->tb_verilator_uvm__DOT__dut__DOT__packetTracker[vlSelf->__Vdlyvdim0__tb_verilator_uvm__DOT__dut__DOT__packetTracker__v0]
            = vlSelf->__Vdlyvval__tb_verilator_uvm__DOT__dut__DOT__packetTracker__v0;
    }
}

VL_INLINE_OPT void Vpacket_handler___024root___nba_comb__TOP__2(Vpacket_handler___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vpacket_handler__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vpacket_handler___024root___nba_comb__TOP__2\n"); );
    // Init
    CData/*6:0*/ __Vtableidx1;
    __Vtableidx1 = 0;
    // Body
    vlSelf->tb_verilator_uvm__DOT__dut__DOT__fifo_wr_en
        = ((~ ((IData)(vlSelf->tb_verilator_uvm__DOT__dut__DOT__fifo_count)
               >> 8U)) & (IData)(vlSelf->tb_verilator_uvm__DOT__i_valid));
    __Vtableidx1 = (((0U != (IData)(vlSelf->tb_verilator_uvm__DOT__dut__DOT__fifo_count))
                     << 6U) | ((((0U != (IData)(vlSelf->tb_verilator_uvm__DOT__dut__DOT__fifo_count))
                                 & (IData)((vlSelf->tb_verilator_uvm__DOT__dut__DOT____VdfgTmp_h7a21bf1e__0
                                            >> 0x20U)))
                                << 5U) | (((IData)(vlSelf->tb_verilator_uvm__DOT__i_ready)
                                           << 4U) | (IData)(vlSelf->tb_verilator_uvm__DOT__dut__DOT__state))));
    vlSelf->tb_verilator_uvm__DOT__dut__DOT__next_state
        = Vpacket_handler__ConstPool__TABLE_hba08ba0f_0
        [__Vtableidx1];
}

void Vpacket_handler___024root___nba_sequent__TOP__1(Vpacket_handler___024root* vlSelf);

void Vpacket_handler___024root___eval_nba(Vpacket_handler___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vpacket_handler__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vpacket_handler___024root___eval_nba\n"); );
    // Body
    if ((2ULL & vlSelf->__VnbaTriggered.word(0U))) {
        Vpacket_handler___024root___nba_sequent__TOP__0(vlSelf);
    }
    if ((1ULL & vlSelf->__VnbaTriggered.word(0U))) {
        Vpacket_handler___024root___nba_sequent__TOP__1(vlSelf);
    }
    if ((2ULL & vlSelf->__VnbaTriggered.word(0U))) {
        Vpacket_handler___024root___nba_sequent__TOP__2(vlSelf);
    }
    if ((3ULL & vlSelf->__VnbaTriggered.word(0U))) {
        Vpacket_handler___024root___nba_comb__TOP__0(vlSelf);
    }
    if ((5ULL & vlSelf->__VnbaTriggered.word(0U))) {
        Vpacket_handler___024root___nba_comb__TOP__1(vlSelf);
    }
    if ((1ULL & vlSelf->__VnbaTriggered.word(0U))) {
        Vpacket_handler___024root___nba_sequent__TOP__3(vlSelf);
    }
    if ((7ULL & vlSelf->__VnbaTriggered.word(0U))) {
        Vpacket_handler___024root___nba_comb__TOP__2(vlSelf);
    }
}

void Vpacket_handler___024root___timing_resume(Vpacket_handler___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vpacket_handler__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vpacket_handler___024root___timing_resume\n"); );
    // Body
    if ((1ULL & vlSelf->__VactTriggered.word(0U))) {
        vlSelf->__VtrigSched_h3539bd04__0.resume("@(posedge tb_verilator_uvm.clk)");
    }
    if ((4ULL & vlSelf->__VactTriggered.word(0U))) {
        vlSelf->__VdlySched.resume();
    }
}

void Vpacket_handler___024root___timing_commit(Vpacket_handler___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vpacket_handler__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vpacket_handler___024root___timing_commit\n"); );
    // Body
    if ((! (1ULL & vlSelf->__VactTriggered.word(0U)))) {
        vlSelf->__VtrigSched_h3539bd04__0.commit("@(posedge tb_verilator_uvm.clk)");
    }
}

void Vpacket_handler___024root___eval_triggers__act(Vpacket_handler___024root* vlSelf);

bool Vpacket_handler___024root___eval_phase__act(Vpacket_handler___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vpacket_handler__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vpacket_handler___024root___eval_phase__act\n"); );
    // Init
    VlTriggerVec<3> __VpreTriggered;
    CData/*0:0*/ __VactExecute;
    // Body
    Vpacket_handler___024root___eval_triggers__act(vlSelf);
    Vpacket_handler___024root___timing_commit(vlSelf);
    __VactExecute = vlSelf->__VactTriggered.any();
    if (__VactExecute) {
        __VpreTriggered.andNot(vlSelf->__VactTriggered, vlSelf->__VnbaTriggered);
        vlSelf->__VnbaTriggered.thisOr(vlSelf->__VactTriggered);
        Vpacket_handler___024root___timing_resume(vlSelf);
        Vpacket_handler___024root___eval_act(vlSelf);
    }
    return (__VactExecute);
}

bool Vpacket_handler___024root___eval_phase__nba(Vpacket_handler___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vpacket_handler__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vpacket_handler___024root___eval_phase__nba\n"); );
    // Init
    CData/*0:0*/ __VnbaExecute;
    // Body
    __VnbaExecute = vlSelf->__VnbaTriggered.any();
    if (__VnbaExecute) {
        Vpacket_handler___024root___eval_nba(vlSelf);
        vlSelf->__VnbaTriggered.clear();
    }
    return (__VnbaExecute);
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vpacket_handler___024root___dump_triggers__nba(Vpacket_handler___024root* vlSelf);
#endif  // VL_DEBUG
#ifdef VL_DEBUG
VL_ATTR_COLD void Vpacket_handler___024root___dump_triggers__act(Vpacket_handler___024root* vlSelf);
#endif  // VL_DEBUG

void Vpacket_handler___024root___eval(Vpacket_handler___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vpacket_handler__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vpacket_handler___024root___eval\n"); );
    // Init
    IData/*31:0*/ __VnbaIterCount;
    CData/*0:0*/ __VnbaContinue;
    // Body
    __VnbaIterCount = 0U;
    __VnbaContinue = 1U;
    while (__VnbaContinue) {
        if (VL_UNLIKELY((0x64U < __VnbaIterCount))) {
#ifdef VL_DEBUG
            Vpacket_handler___024root___dump_triggers__nba(vlSelf);
#endif
            VL_FATAL_MT("tests/tb_uvm_sv.sv", 148, "", "NBA region did not converge.");
        }
        __VnbaIterCount = ((IData)(1U) + __VnbaIterCount);
        __VnbaContinue = 0U;
        vlSelf->__VactIterCount = 0U;
        vlSelf->__VactContinue = 1U;
        while (vlSelf->__VactContinue) {
            if (VL_UNLIKELY((0x64U < vlSelf->__VactIterCount))) {
#ifdef VL_DEBUG
                Vpacket_handler___024root___dump_triggers__act(vlSelf);
#endif
                VL_FATAL_MT("tests/tb_uvm_sv.sv", 148, "", "Active region did not converge.");
            }
            vlSelf->__VactIterCount = ((IData)(1U)
                                       + vlSelf->__VactIterCount);
            vlSelf->__VactContinue = 0U;
            if (Vpacket_handler___024root___eval_phase__act(vlSelf)) {
                vlSelf->__VactContinue = 1U;
            }
        }
        if (Vpacket_handler___024root___eval_phase__nba(vlSelf)) {
            __VnbaContinue = 1U;
        }
    }
}

#ifdef VL_DEBUG
void Vpacket_handler___024root___eval_debug_assertions(Vpacket_handler___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vpacket_handler__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vpacket_handler___024root___eval_debug_assertions\n"); );
}
#endif  // VL_DEBUG
