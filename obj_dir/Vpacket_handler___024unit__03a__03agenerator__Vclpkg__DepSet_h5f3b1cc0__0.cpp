// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vpacket_handler.h for the primary calling header

#include "Vpacket_handler__pch.h"
#include "Vpacket_handler__Syms.h"
#include "Vpacket_handler___024unit__03a__03agenerator__Vclpkg.h"

Vpacket_handler___024unit__03a__03agenerator::Vpacket_handler___024unit__03a__03agenerator(Vpacket_handler__Syms* __restrict vlSymsp, IData/*31:0*/ c) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vpacket_handler___024unit__03a__03agenerator::new\n"); );
    // Init
    _ctor_var_reset(vlSymsp);
    // Body
    this->__PVT__count = c;
}

void Vpacket_handler___024unit__03a__03agenerator::_ctor_var_reset(Vpacket_handler__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vpacket_handler___024unit__03a__03agenerator::_ctor_var_reset\n"); );
    // Body
    if (false && vlSymsp) {}  // Prevent unused
    __PVT__count = 0;
}
