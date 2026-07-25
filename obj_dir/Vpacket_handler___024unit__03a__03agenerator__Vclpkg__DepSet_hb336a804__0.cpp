// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vpacket_handler.h for the primary calling header

#include "Vpacket_handler__pch.h"
#include "Vpacket_handler__Syms.h"
#include "Vpacket_handler___024unit__03a__03agenerator__Vclpkg.h"
#include "Vpacket_handler___024unit__03a__03apacket_item__Vclpkg.h"

void Vpacket_handler___024unit__03a__03agenerator::__VnoInFunc_generate_items(Vpacket_handler__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vpacket_handler___024unit__03a__03agenerator::__VnoInFunc_generate_items\n"); );
    // Body
    IData/*31:0*/ unnamedblk1__DOT__i;
    unnamedblk1__DOT__i = 0;
    VlClassRef<Vpacket_handler___024unit__03a__03apacket_item> unnamedblk1__DOT__unnamedblk2__DOT__pkt;
    unnamedblk1__DOT__i = 0U;
    while (VL_LTS_III(32, unnamedblk1__DOT__i, this->__PVT__count)) {
        unnamedblk1__DOT__unnamedblk2__DOT__pkt = VL_NEW(Vpacket_handler___024unit__03a__03apacket_item, vlSymsp);
        VL_NULL_CHECK(unnamedblk1__DOT__unnamedblk2__DOT__pkt, "tests/tb_uvm_sv.sv", 39)->__VnoInFunc_randomize_me(vlSymsp);
        this->__PVT__pkt_q.push_back(unnamedblk1__DOT__unnamedblk2__DOT__pkt);
        unnamedblk1__DOT__i = ((IData)(1U) + unnamedblk1__DOT__i);
    }
}
