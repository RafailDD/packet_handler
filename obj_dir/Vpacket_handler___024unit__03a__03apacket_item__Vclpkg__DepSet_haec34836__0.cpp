// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vpacket_handler.h for the primary calling header

#include "Vpacket_handler__pch.h"
#include "Vpacket_handler__Syms.h"
#include "Vpacket_handler___024unit__03a__03apacket_item__Vclpkg.h"

Vpacket_handler___024unit__03a__03apacket_item::Vpacket_handler___024unit__03a__03apacket_item(Vpacket_handler__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vpacket_handler___024unit__03a__03apacket_item::new\n"); );
    // Init
    _ctor_var_reset(vlSymsp);
}

void Vpacket_handler___024unit__03a__03apacket_item::_ctor_var_reset(Vpacket_handler__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vpacket_handler___024unit__03a__03apacket_item::_ctor_var_reset\n"); );
    // Body
    if (false && vlSymsp) {}  // Prevent unused
    __PVT__msgLength = 0;
    __PVT__streamId = 0;
    __PVT__seqNumber = 0;
    __PVT__data_size = 0;
    for (int __Vi0 = 0; __Vi0 < 9; ++__Vi0) {
        __PVT__data_arr[__Vi0] = 0;
    }
    __Vlvbound_h5504670c__0 = 0;
}
