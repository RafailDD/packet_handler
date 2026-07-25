// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vpacket_handler.h for the primary calling header

#include "Vpacket_handler__pch.h"
#include "Vpacket_handler___024unit__03a__03amonitor__Vclpkg.h"

void Vpacket_handler___024unit__03a__03amonitor::__VnoInFunc_sample(Vpacket_handler__Syms* __restrict vlSymsp, IData/*31:0*/ data_size, CData/*0:0*/ packetLost) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vpacket_handler___024unit__03a__03amonitor::__VnoInFunc_sample\n"); );
    // Body
    if (packetLost) {
        this->__PVT__cvg_packet_loss_hit = ((IData)(1U)
                                            + this->__PVT__cvg_packet_loss_hit);
    }
    if ((1U == data_size)) {
        this->__PVT__cvg_small_packet_hit = ((IData)(1U)
                                             + this->__PVT__cvg_small_packet_hit);
    }
    if ((9U == data_size)) {
        this->__PVT__cvg_large_packet_hit = ((IData)(1U)
                                             + this->__PVT__cvg_large_packet_hit);
    }
}

void Vpacket_handler___024unit__03a__03amonitor::__VnoInFunc_report_coverage(Vpacket_handler__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vpacket_handler___024unit__03a__03amonitor::__VnoInFunc_report_coverage\n"); );
    // Body
    VL_WRITEF("=== Functional Coverage Report ===\nPacket Loss Hit: %0d\nSmall Packet (data_size=1) Hit: %0d\nLarge Packet (data_size=9) Hit: %0d\n",
              32,this->__PVT__cvg_packet_loss_hit,32,
              this->__PVT__cvg_small_packet_hit,32,
              this->__PVT__cvg_large_packet_hit);
    if (((VL_LTS_III(32, 0U, this->__PVT__cvg_packet_loss_hit)
          & VL_LTS_III(32, 0U, this->__PVT__cvg_small_packet_hit))
         & VL_LTS_III(32, 0U, this->__PVT__cvg_large_packet_hit))) {
        VL_WRITEF("Coverage 100 percent Achieved\n");
    } else {
        VL_WRITEF("Coverage Incomplete\n");
    }
    VL_WRITEF("==================================\n");
}

Vpacket_handler___024unit__03a__03amonitor::~Vpacket_handler___024unit__03a__03amonitor() {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vpacket_handler___024unit__03a__03amonitor::~\n"); );
}

std::string VL_TO_STRING(const VlClassRef<Vpacket_handler___024unit__03a__03amonitor>& obj) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vpacket_handler___024unit__03a__03amonitor::VL_TO_STRING\n"); );
    // Body
    return (obj ? obj->to_string() : "null");
}

std::string Vpacket_handler___024unit__03a__03amonitor::to_string() const {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vpacket_handler___024unit__03a__03amonitor::to_string\n"); );
    // Body
    return (std::string{"'{"} + to_string_middle() + "}");
}

std::string Vpacket_handler___024unit__03a__03amonitor::to_string_middle() const {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vpacket_handler___024unit__03a__03amonitor::to_string_middle\n"); );
    // Body
    std::string out;
    out += "cvg_packet_loss_hit:" + VL_TO_STRING(__PVT__cvg_packet_loss_hit);
    out += ", cvg_small_packet_hit:" + VL_TO_STRING(__PVT__cvg_small_packet_hit);
    out += ", cvg_large_packet_hit:" + VL_TO_STRING(__PVT__cvg_large_packet_hit);
    return out;
}
