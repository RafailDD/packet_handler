// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vpacket_handler.h for the primary calling header

#include "Vpacket_handler__pch.h"
#include "Vpacket_handler__Syms.h"
#include "Vpacket_handler___024unit__03a__03adriver__Vclpkg.h"
#include "Vpacket_handler___024unit__03a__03aenv__Vclpkg.h"
#include "Vpacket_handler___024unit__03a__03agenerator__Vclpkg.h"
#include "Vpacket_handler___024unit__03a__03amonitor__Vclpkg.h"
#include "Vpacket_handler___024unit__03a__03ascoreboard__Vclpkg.h"

Vpacket_handler___024unit__03a__03aenv::Vpacket_handler___024unit__03a__03aenv(Vpacket_handler__Syms* __restrict vlSymsp, IData/*31:0*/ count) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vpacket_handler___024unit__03a__03aenv::new\n"); );
    // Init
    _ctor_var_reset(vlSymsp);
    // Body
    this->__PVT__gen = VL_NEW(Vpacket_handler___024unit__03a__03agenerator, vlSymsp, count);
    this->__PVT__drv = VL_NEW(Vpacket_handler___024unit__03a__03adriver, vlSymsp);
    this->__PVT__scb = VL_NEW(Vpacket_handler___024unit__03a__03ascoreboard, vlSymsp);
    this->__PVT__mon = VL_NEW(Vpacket_handler___024unit__03a__03amonitor, vlSymsp);
}
