// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vpacket_handler.h for the primary calling header

#include "Vpacket_handler__pch.h"
#include "Vpacket_handler__Syms.h"
#include "Vpacket_handler___024root.h"

#ifdef VL_DEBUG
VL_ATTR_COLD void Vpacket_handler___024root___dump_triggers__stl(Vpacket_handler___024root* vlSelf);
#endif  // VL_DEBUG

VL_ATTR_COLD void Vpacket_handler___024root___eval_triggers__stl(Vpacket_handler___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vpacket_handler__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vpacket_handler___024root___eval_triggers__stl\n"); );
    // Body
    vlSelf->__VstlTriggered.set(0U, (IData)(vlSelf->__VstlFirstIteration));
#ifdef VL_DEBUG
    if (VL_UNLIKELY(vlSymsp->_vm_contextp__->debug())) {
        Vpacket_handler___024root___dump_triggers__stl(vlSelf);
    }
#endif
}
