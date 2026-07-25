// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vpacket_handler.h for the primary calling header

#include "Vpacket_handler__pch.h"
#include "Vpacket_handler__Syms.h"
#include "Vpacket_handler___024unit__03a__03amonitor__Vclpkg.h"

Vpacket_handler___024unit__03a__03amonitor::Vpacket_handler___024unit__03a__03amonitor(Vpacket_handler__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vpacket_handler___024unit__03a__03amonitor::new\n"); );
    // Init
    _ctor_var_reset(vlSymsp);
    // Body
    this->__PVT__cvg_packet_loss_hit = 0U;
    this->__PVT__cvg_small_packet_hit = 0U;
    this->__PVT__cvg_large_packet_hit = 0U;
}

void Vpacket_handler___024unit__03a__03amonitor::_ctor_var_reset(Vpacket_handler__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vpacket_handler___024unit__03a__03amonitor::_ctor_var_reset\n"); );
    // Body
    if (false && vlSymsp) {}  // Prevent unused
    __PVT__cvg_packet_loss_hit = 0;
    __PVT__cvg_small_packet_hit = 0;
    __PVT__cvg_large_packet_hit = 0;
}
