// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vpacket_handler.h for the primary calling header

#include "Vpacket_handler__pch.h"
#include "Vpacket_handler___024unit__03a__03apacket_item__Vclpkg.h"

void Vpacket_handler___024unit__03a__03apacket_item::__VnoInFunc_randomize_me(Vpacket_handler__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vpacket_handler___024unit__03a__03apacket_item::__VnoInFunc_randomize_me\n"); );
    // Body
    IData/*31:0*/ unnamedblk1__DOT__i;
    unnamedblk1__DOT__i = 0;
    this->__PVT__data_size = ((IData)(1U) + (IData)(
                                                    VL_MODDIV_III(32, (IData)(
                                                                              VL_RANDOM_I()), (IData)(9U))));
    this->__PVT__msgLength = ((IData)(8U) + VL_MULS_III(32, (IData)(4U), this->__PVT__data_size));
    this->__PVT__streamId = ((IData)(1U) + (IData)(
                                                   (0x1fU
                                                    & VL_RANDOM_I())));
    this->__PVT__seqNumber = VL_RANDOM_I();
    if (VL_LTS_III(32, 0U, this->__PVT__data_size)) {
        this->__Vlvbound_h5504670c__0 = VL_RANDOM_I();
        this->__PVT__data_arr[0U] = this->__Vlvbound_h5504670c__0;
    } else {
        this->__PVT__data_arr[0U] = 0U;
    }
    if (VL_LTS_III(32, 1U, this->__PVT__data_size)) {
        this->__Vlvbound_h5504670c__0 = VL_RANDOM_I();
        this->__PVT__data_arr[1U] = this->__Vlvbound_h5504670c__0;
    } else {
        this->__PVT__data_arr[1U] = 0U;
    }
    if (VL_LTS_III(32, 2U, this->__PVT__data_size)) {
        this->__Vlvbound_h5504670c__0 = VL_RANDOM_I();
        this->__PVT__data_arr[2U] = this->__Vlvbound_h5504670c__0;
    } else {
        this->__PVT__data_arr[2U] = 0U;
    }
    if (VL_LTS_III(32, 3U, this->__PVT__data_size)) {
        this->__Vlvbound_h5504670c__0 = VL_RANDOM_I();
        this->__PVT__data_arr[3U] = this->__Vlvbound_h5504670c__0;
    } else {
        this->__PVT__data_arr[3U] = 0U;
    }
    if (VL_LTS_III(32, 4U, this->__PVT__data_size)) {
        this->__Vlvbound_h5504670c__0 = VL_RANDOM_I();
        this->__PVT__data_arr[4U] = this->__Vlvbound_h5504670c__0;
    } else {
        this->__PVT__data_arr[4U] = 0U;
    }
    if (VL_LTS_III(32, 5U, this->__PVT__data_size)) {
        this->__Vlvbound_h5504670c__0 = VL_RANDOM_I();
        this->__PVT__data_arr[5U] = this->__Vlvbound_h5504670c__0;
    } else {
        this->__PVT__data_arr[5U] = 0U;
    }
    if (VL_LTS_III(32, 6U, this->__PVT__data_size)) {
        this->__Vlvbound_h5504670c__0 = VL_RANDOM_I();
        this->__PVT__data_arr[6U] = this->__Vlvbound_h5504670c__0;
    } else {
        this->__PVT__data_arr[6U] = 0U;
    }
    if (VL_LTS_III(32, 7U, this->__PVT__data_size)) {
        this->__Vlvbound_h5504670c__0 = VL_RANDOM_I();
        this->__PVT__data_arr[7U] = this->__Vlvbound_h5504670c__0;
    } else {
        this->__PVT__data_arr[7U] = 0U;
    }
    if (VL_LTS_III(32, 8U, this->__PVT__data_size)) {
        this->__Vlvbound_h5504670c__0 = VL_RANDOM_I();
        this->__PVT__data_arr[8U] = this->__Vlvbound_h5504670c__0;
    } else {
        this->__PVT__data_arr[8U] = 0U;
    }
    unnamedblk1__DOT__i = 9U;
}

Vpacket_handler___024unit__03a__03apacket_item::~Vpacket_handler___024unit__03a__03apacket_item() {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vpacket_handler___024unit__03a__03apacket_item::~\n"); );
}

std::string VL_TO_STRING(const VlClassRef<Vpacket_handler___024unit__03a__03apacket_item>& obj) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vpacket_handler___024unit__03a__03apacket_item::VL_TO_STRING\n"); );
    // Body
    return (obj ? obj->to_string() : "null");
}

std::string Vpacket_handler___024unit__03a__03apacket_item::to_string() const {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vpacket_handler___024unit__03a__03apacket_item::to_string\n"); );
    // Body
    return (std::string{"'{"} + to_string_middle() + "}");
}

std::string Vpacket_handler___024unit__03a__03apacket_item::to_string_middle() const {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vpacket_handler___024unit__03a__03apacket_item::to_string_middle\n"); );
    // Body
    std::string out;
    out += "msgLength:" + VL_TO_STRING(__PVT__msgLength);
    out += ", streamId:" + VL_TO_STRING(__PVT__streamId);
    out += ", seqNumber:" + VL_TO_STRING(__PVT__seqNumber);
    out += ", data_size:" + VL_TO_STRING(__PVT__data_size);
    out += ", data_arr:" + VL_TO_STRING(__PVT__data_arr);
    out += ", __Vlvbound_h5504670c__0:" + VL_TO_STRING(__Vlvbound_h5504670c__0);
    return out;
}
