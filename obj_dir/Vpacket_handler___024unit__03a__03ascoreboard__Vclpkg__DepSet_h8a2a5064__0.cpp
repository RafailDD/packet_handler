// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vpacket_handler.h for the primary calling header

#include "Vpacket_handler__pch.h"
#include "Vpacket_handler___024unit__03a__03ascoreboard__Vclpkg.h"

void Vpacket_handler___024unit__03a__03ascoreboard::__VnoInFunc_push_expected(Vpacket_handler__Syms* __restrict vlSymsp, VlClassRef<Vpacket_handler___024unit__03a__03apacket_item> pkt) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vpacket_handler___024unit__03a__03ascoreboard::__VnoInFunc_push_expected\n"); );
    // Body
    this->__PVT__expected_q.push_back(pkt);
}

Vpacket_handler___024unit__03a__03ascoreboard::~Vpacket_handler___024unit__03a__03ascoreboard() {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vpacket_handler___024unit__03a__03ascoreboard::~\n"); );
}

std::string VL_TO_STRING(const VlClassRef<Vpacket_handler___024unit__03a__03ascoreboard>& obj) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vpacket_handler___024unit__03a__03ascoreboard::VL_TO_STRING\n"); );
    // Body
    return (obj ? obj->to_string() : "null");
}

std::string Vpacket_handler___024unit__03a__03ascoreboard::to_string() const {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vpacket_handler___024unit__03a__03ascoreboard::to_string\n"); );
    // Body
    return (std::string{"'{"} + to_string_middle() + "}");
}

std::string Vpacket_handler___024unit__03a__03ascoreboard::to_string_middle() const {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vpacket_handler___024unit__03a__03ascoreboard::to_string_middle\n"); );
    // Body
    std::string out;
    out += "expected_q:" + VL_TO_STRING(__PVT__expected_q);
    out += ", packets_received:" + VL_TO_STRING(__PVT__packets_received);
    return out;
}
