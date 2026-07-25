// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vpacket_handler.h for the primary calling header

#include "Vpacket_handler__pch.h"
#include "Vpacket_handler___024unit__03a__03aenv__Vclpkg.h"

Vpacket_handler___024unit__03a__03aenv::~Vpacket_handler___024unit__03a__03aenv() {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vpacket_handler___024unit__03a__03aenv::~\n"); );
}

std::string VL_TO_STRING(const VlClassRef<Vpacket_handler___024unit__03a__03aenv>& obj) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vpacket_handler___024unit__03a__03aenv::VL_TO_STRING\n"); );
    // Body
    return (obj ? obj->to_string() : "null");
}

std::string Vpacket_handler___024unit__03a__03aenv::to_string() const {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vpacket_handler___024unit__03a__03aenv::to_string\n"); );
    // Body
    return (std::string{"'{"} + to_string_middle() + "}");
}

std::string Vpacket_handler___024unit__03a__03aenv::to_string_middle() const {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vpacket_handler___024unit__03a__03aenv::to_string_middle\n"); );
    // Body
    std::string out;
    out += "gen:" + VL_TO_STRING(__PVT__gen);
    out += ", drv:" + VL_TO_STRING(__PVT__drv);
    out += ", scb:" + VL_TO_STRING(__PVT__scb);
    out += ", mon:" + VL_TO_STRING(__PVT__mon);
    return out;
}
