// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vpacket_handler.h for the primary calling header

#include "Vpacket_handler__pch.h"
#include "Vpacket_handler___024unit__03a__03agenerator__Vclpkg.h"

void Vpacket_handler___024unit__03a__03agenerator::__VnoInFunc_get_next(Vpacket_handler__Syms* __restrict vlSymsp, VlClassRef<Vpacket_handler___024unit__03a__03apacket_item> &get_next__Vfuncrtn) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vpacket_handler___024unit__03a__03agenerator::__VnoInFunc_get_next\n"); );
    // Body
    {
        if (VL_LTS_III(32, 0U, this->__PVT__pkt_q.size())) {
            get_next__Vfuncrtn = this->__PVT__pkt_q.pop_front();
            goto __Vlabel1;
        } else {
            get_next__Vfuncrtn = VlNull{};
            goto __Vlabel1;
        }
        __Vlabel1: ;
    }
}

Vpacket_handler___024unit__03a__03agenerator::~Vpacket_handler___024unit__03a__03agenerator() {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vpacket_handler___024unit__03a__03agenerator::~\n"); );
}

std::string VL_TO_STRING(const VlClassRef<Vpacket_handler___024unit__03a__03agenerator>& obj) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vpacket_handler___024unit__03a__03agenerator::VL_TO_STRING\n"); );
    // Body
    return (obj ? obj->to_string() : "null");
}

std::string Vpacket_handler___024unit__03a__03agenerator::to_string() const {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vpacket_handler___024unit__03a__03agenerator::to_string\n"); );
    // Body
    return (std::string{"'{"} + to_string_middle() + "}");
}

std::string Vpacket_handler___024unit__03a__03agenerator::to_string_middle() const {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vpacket_handler___024unit__03a__03agenerator::to_string_middle\n"); );
    // Body
    std::string out;
    out += "pkt_q:" + VL_TO_STRING(__PVT__pkt_q);
    out += ", count:" + VL_TO_STRING(__PVT__count);
    return out;
}
