// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vpacket_handler.h for the primary calling header

#include "Vpacket_handler__pch.h"
#include "Vpacket_handler___024root.h"

VL_ATTR_COLD void Vpacket_handler___024root___eval_static(Vpacket_handler___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vpacket_handler__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vpacket_handler___024root___eval_static\n"); );
}

VL_ATTR_COLD void Vpacket_handler___024root___eval_final(Vpacket_handler___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vpacket_handler__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vpacket_handler___024root___eval_final\n"); );
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vpacket_handler___024root___dump_triggers__stl(Vpacket_handler___024root* vlSelf);
#endif  // VL_DEBUG
VL_ATTR_COLD bool Vpacket_handler___024root___eval_phase__stl(Vpacket_handler___024root* vlSelf);

VL_ATTR_COLD void Vpacket_handler___024root___eval_settle(Vpacket_handler___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vpacket_handler__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vpacket_handler___024root___eval_settle\n"); );
    // Init
    IData/*31:0*/ __VstlIterCount;
    CData/*0:0*/ __VstlContinue;
    // Body
    __VstlIterCount = 0U;
    vlSelf->__VstlFirstIteration = 1U;
    __VstlContinue = 1U;
    while (__VstlContinue) {
        if (VL_UNLIKELY((0x64U < __VstlIterCount))) {
#ifdef VL_DEBUG
            Vpacket_handler___024root___dump_triggers__stl(vlSelf);
#endif
            VL_FATAL_MT("tests/tb_uvm_sv.sv", 148, "", "Settle region did not converge.");
        }
        __VstlIterCount = ((IData)(1U) + __VstlIterCount);
        __VstlContinue = 0U;
        if (Vpacket_handler___024root___eval_phase__stl(vlSelf)) {
            __VstlContinue = 1U;
        }
        vlSelf->__VstlFirstIteration = 0U;
    }
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vpacket_handler___024root___dump_triggers__stl(Vpacket_handler___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vpacket_handler__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vpacket_handler___024root___dump_triggers__stl\n"); );
    // Body
    if ((1U & (~ (IData)(vlSelf->__VstlTriggered.any())))) {
        VL_DBG_MSGF("         No triggers active\n");
    }
    if ((1ULL & vlSelf->__VstlTriggered.word(0U))) {
        VL_DBG_MSGF("         'stl' region trigger index 0 is active: Internal 'stl' trigger - first iteration\n");
    }
}
#endif  // VL_DEBUG

extern const VlUnpacked<CData/*3:0*/, 128> Vpacket_handler__ConstPool__TABLE_hba08ba0f_0;

VL_ATTR_COLD void Vpacket_handler___024root___stl_sequent__TOP__0(Vpacket_handler___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vpacket_handler__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vpacket_handler___024root___stl_sequent__TOP__0\n"); );
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
    vlSelf->tb_verilator_uvm__DOT__dut__DOT__in_stream_idx
        = (0x1fU & ((IData)(vlSelf->tb_verilator_uvm__DOT__dut__DOT__in_streamId)
                    - (IData)(1U)));
    vlSelf->tb_verilator_uvm__DOT__dut__DOT__fifo_wr_en
        = ((~ ((IData)(vlSelf->tb_verilator_uvm__DOT__dut__DOT__fifo_count)
               >> 8U)) & (IData)(vlSelf->tb_verilator_uvm__DOT__i_valid));
    vlSelf->tb_verilator_uvm__DOT__dut__DOT__fifo_rd_en
        = ((0U != (IData)(vlSelf->tb_verilator_uvm__DOT__dut__DOT__fifo_count))
           & ((1U == (IData)(vlSelf->tb_verilator_uvm__DOT__dut__DOT__state))
              | ((2U == (IData)(vlSelf->tb_verilator_uvm__DOT__dut__DOT__state))
                 | (4U == (IData)(vlSelf->tb_verilator_uvm__DOT__dut__DOT__state)))));
    vlSelf->tb_verilator_uvm__DOT__dut__DOT____VdfgTmp_h7a21bf1e__0
        = vlSelf->tb_verilator_uvm__DOT__dut__DOT__fifo_mem
        [vlSelf->tb_verilator_uvm__DOT__dut__DOT__fifo_rd_ptr];
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

VL_ATTR_COLD void Vpacket_handler___024root___eval_stl(Vpacket_handler___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vpacket_handler__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vpacket_handler___024root___eval_stl\n"); );
    // Body
    if ((1ULL & vlSelf->__VstlTriggered.word(0U))) {
        Vpacket_handler___024root___stl_sequent__TOP__0(vlSelf);
    }
}

VL_ATTR_COLD void Vpacket_handler___024root___eval_triggers__stl(Vpacket_handler___024root* vlSelf);

VL_ATTR_COLD bool Vpacket_handler___024root___eval_phase__stl(Vpacket_handler___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vpacket_handler__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vpacket_handler___024root___eval_phase__stl\n"); );
    // Init
    CData/*0:0*/ __VstlExecute;
    // Body
    Vpacket_handler___024root___eval_triggers__stl(vlSelf);
    __VstlExecute = vlSelf->__VstlTriggered.any();
    if (__VstlExecute) {
        Vpacket_handler___024root___eval_stl(vlSelf);
    }
    return (__VstlExecute);
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vpacket_handler___024root___dump_triggers__act(Vpacket_handler___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vpacket_handler__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vpacket_handler___024root___dump_triggers__act\n"); );
    // Body
    if ((1U & (~ (IData)(vlSelf->__VactTriggered.any())))) {
        VL_DBG_MSGF("         No triggers active\n");
    }
    if ((1ULL & vlSelf->__VactTriggered.word(0U))) {
        VL_DBG_MSGF("         'act' region trigger index 0 is active: @(posedge tb_verilator_uvm.clk)\n");
    }
    if ((2ULL & vlSelf->__VactTriggered.word(0U))) {
        VL_DBG_MSGF("         'act' region trigger index 1 is active: @(posedge tb_verilator_uvm.clk or negedge tb_verilator_uvm.rst_n)\n");
    }
    if ((4ULL & vlSelf->__VactTriggered.word(0U))) {
        VL_DBG_MSGF("         'act' region trigger index 2 is active: @([true] __VdlySched.awaitingCurrentTime())\n");
    }
}
#endif  // VL_DEBUG

#ifdef VL_DEBUG
VL_ATTR_COLD void Vpacket_handler___024root___dump_triggers__nba(Vpacket_handler___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vpacket_handler__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vpacket_handler___024root___dump_triggers__nba\n"); );
    // Body
    if ((1U & (~ (IData)(vlSelf->__VnbaTriggered.any())))) {
        VL_DBG_MSGF("         No triggers active\n");
    }
    if ((1ULL & vlSelf->__VnbaTriggered.word(0U))) {
        VL_DBG_MSGF("         'nba' region trigger index 0 is active: @(posedge tb_verilator_uvm.clk)\n");
    }
    if ((2ULL & vlSelf->__VnbaTriggered.word(0U))) {
        VL_DBG_MSGF("         'nba' region trigger index 1 is active: @(posedge tb_verilator_uvm.clk or negedge tb_verilator_uvm.rst_n)\n");
    }
    if ((4ULL & vlSelf->__VnbaTriggered.word(0U))) {
        VL_DBG_MSGF("         'nba' region trigger index 2 is active: @([true] __VdlySched.awaitingCurrentTime())\n");
    }
}
#endif  // VL_DEBUG

VL_ATTR_COLD void Vpacket_handler___024root___ctor_var_reset(Vpacket_handler___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vpacket_handler__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vpacket_handler___024root___ctor_var_reset\n"); );
    // Body
    vlSelf->tb_verilator_uvm__DOT__clk = VL_RAND_RESET_I(1);
    vlSelf->tb_verilator_uvm__DOT__rst_n = VL_RAND_RESET_I(1);
    vlSelf->tb_verilator_uvm__DOT__i_data = VL_RAND_RESET_I(32);
    vlSelf->tb_verilator_uvm__DOT__i_valid = VL_RAND_RESET_I(1);
    vlSelf->tb_verilator_uvm__DOT__i_last = VL_RAND_RESET_I(1);
    vlSelf->tb_verilator_uvm__DOT__i_ready = VL_RAND_RESET_I(1);
    VL_RAND_RESET_W(296, vlSelf->tb_verilator_uvm__DOT__o_data);
    vlSelf->tb_verilator_uvm__DOT__o_valid = VL_RAND_RESET_I(1);
    vlSelf->tb_verilator_uvm__DOT__unnamedblk2__DOT__unnamedblk3__DOT__unnamedblk4__DOT__j = 0;
    for (int __Vi0 = 0; __Vi0 < 256; ++__Vi0) {
        vlSelf->tb_verilator_uvm__DOT__dut__DOT__fifo_mem[__Vi0] = VL_RAND_RESET_Q(33);
    }
    vlSelf->tb_verilator_uvm__DOT__dut__DOT__fifo_wr_ptr = VL_RAND_RESET_I(8);
    vlSelf->tb_verilator_uvm__DOT__dut__DOT__fifo_rd_ptr = VL_RAND_RESET_I(8);
    vlSelf->tb_verilator_uvm__DOT__dut__DOT__fifo_count = VL_RAND_RESET_I(9);
    vlSelf->tb_verilator_uvm__DOT__dut__DOT__fifo_wr_en = VL_RAND_RESET_I(1);
    vlSelf->tb_verilator_uvm__DOT__dut__DOT__fifo_rd_en = VL_RAND_RESET_I(1);
    vlSelf->tb_verilator_uvm__DOT__dut__DOT__in_state = VL_RAND_RESET_I(2);
    vlSelf->tb_verilator_uvm__DOT__dut__DOT__in_streamId = VL_RAND_RESET_I(16);
    vlSelf->tb_verilator_uvm__DOT__dut__DOT__in_packetLost = VL_RAND_RESET_I(1);
    for (int __Vi0 = 0; __Vi0 < 32; ++__Vi0) {
        vlSelf->tb_verilator_uvm__DOT__dut__DOT__packetTracker[__Vi0] = VL_RAND_RESET_I(32);
    }
    vlSelf->tb_verilator_uvm__DOT__dut__DOT__tracker_valid = VL_RAND_RESET_I(32);
    vlSelf->tb_verilator_uvm__DOT__dut__DOT__in_stream_idx = VL_RAND_RESET_I(5);
    vlSelf->tb_verilator_uvm__DOT__dut__DOT__in_seqNumber_be = VL_RAND_RESET_I(32);
    vlSelf->tb_verilator_uvm__DOT__dut__DOT__state = VL_RAND_RESET_I(4);
    vlSelf->tb_verilator_uvm__DOT__dut__DOT__next_state = VL_RAND_RESET_I(4);
    VL_RAND_RESET_W(296, vlSelf->tb_verilator_uvm__DOT__dut__DOT__shiftReg);
    vlSelf->tb_verilator_uvm__DOT__dut__DOT____VdfgTmp_h7a21bf1e__0 = 0;
    vlSelf->__Vdlyvdim0__tb_verilator_uvm__DOT__dut__DOT__packetTracker__v0 = 0;
    vlSelf->__Vdlyvval__tb_verilator_uvm__DOT__dut__DOT__packetTracker__v0 = VL_RAND_RESET_I(32);
    vlSelf->__Vdlyvset__tb_verilator_uvm__DOT__dut__DOT__packetTracker__v0 = 0;
    vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__fifo_count = VL_RAND_RESET_I(9);
    vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__fifo_wr_ptr = VL_RAND_RESET_I(8);
    vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__in_state = VL_RAND_RESET_I(2);
    VL_RAND_RESET_W(296, vlSelf->__Vdly__tb_verilator_uvm__DOT__dut__DOT__shiftReg);
    vlSelf->__Vtrigprevexpr___TOP__tb_verilator_uvm__DOT__clk__0 = VL_RAND_RESET_I(1);
    vlSelf->__Vtrigprevexpr___TOP__tb_verilator_uvm__DOT__rst_n__0 = VL_RAND_RESET_I(1);
}
