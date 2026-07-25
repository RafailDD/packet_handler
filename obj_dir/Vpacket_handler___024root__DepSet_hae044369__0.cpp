// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vpacket_handler.h for the primary calling header

#include "Vpacket_handler__pch.h"
#include "Vpacket_handler__Syms.h"
#include "Vpacket_handler___024root.h"

#ifdef VL_DEBUG
VL_ATTR_COLD void Vpacket_handler___024root___dump_triggers__act(Vpacket_handler___024root* vlSelf);
#endif  // VL_DEBUG

void Vpacket_handler___024root___eval_triggers__act(Vpacket_handler___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vpacket_handler__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vpacket_handler___024root___eval_triggers__act\n"); );
    // Body
    vlSelf->__VactTriggered.set(0U, ((IData)(vlSelf->tb_verilator_uvm__DOT__clk)
                                     & (~ (IData)(vlSelf->__Vtrigprevexpr___TOP__tb_verilator_uvm__DOT__clk__0))));
    vlSelf->__VactTriggered.set(1U, (((IData)(vlSelf->tb_verilator_uvm__DOT__clk)
                                      & (~ (IData)(vlSelf->__Vtrigprevexpr___TOP__tb_verilator_uvm__DOT__clk__0)))
                                     | ((~ (IData)(vlSelf->tb_verilator_uvm__DOT__rst_n))
                                        & (IData)(vlSelf->__Vtrigprevexpr___TOP__tb_verilator_uvm__DOT__rst_n__0))));
    vlSelf->__VactTriggered.set(2U, vlSelf->__VdlySched.awaitingCurrentTime());
    vlSelf->__Vtrigprevexpr___TOP__tb_verilator_uvm__DOT__clk__0
        = vlSelf->tb_verilator_uvm__DOT__clk;
    vlSelf->__Vtrigprevexpr___TOP__tb_verilator_uvm__DOT__rst_n__0
        = vlSelf->tb_verilator_uvm__DOT__rst_n;
#ifdef VL_DEBUG
    if (VL_UNLIKELY(vlSymsp->_vm_contextp__->debug())) {
        Vpacket_handler___024root___dump_triggers__act(vlSelf);
    }
#endif
}
