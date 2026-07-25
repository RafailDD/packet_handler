// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design internal header
// See Vpacket_handler.h for the primary calling header

#ifndef VERILATED_VPACKET_HANDLER___024UNIT__03A__03APACKET_ITEM__VCLPKG_H_
#define VERILATED_VPACKET_HANDLER___024UNIT__03A__03APACKET_ITEM__VCLPKG_H_  // guard

#include "verilated.h"
#include "verilated_timing.h"


class Vpacket_handler__Syms;

class alignas(VL_CACHE_LINE_BYTES) Vpacket_handler___024unit__03a__03apacket_item__Vclpkg final : public VerilatedModule {
  public:

    // INTERNAL VARIABLES
    Vpacket_handler__Syms* const vlSymsp;

    // CONSTRUCTORS
    Vpacket_handler___024unit__03a__03apacket_item__Vclpkg(Vpacket_handler__Syms* symsp, const char* v__name);
    ~Vpacket_handler___024unit__03a__03apacket_item__Vclpkg();
    VL_UNCOPYABLE(Vpacket_handler___024unit__03a__03apacket_item__Vclpkg);

    // INTERNAL METHODS
    void __Vconfigure(bool first);
};


class Vpacket_handler__Syms;

class Vpacket_handler___024unit__03a__03apacket_item : public VlClass {
  public:

    // DESIGN SPECIFIC STATE
    IData/*31:0*/ __PVT__msgLength;
    IData/*31:0*/ __PVT__streamId;
    IData/*31:0*/ __PVT__seqNumber;
    IData/*31:0*/ __PVT__data_size;
    IData/*31:0*/ __Vlvbound_h5504670c__0;
    VlUnpacked<IData/*31:0*/, 9> __PVT__data_arr;
    void __VnoInFunc_randomize_me(Vpacket_handler__Syms* __restrict vlSymsp);
  private:
    void _ctor_var_reset(Vpacket_handler__Syms* __restrict vlSymsp);
  public:
    Vpacket_handler___024unit__03a__03apacket_item(Vpacket_handler__Syms* __restrict vlSymsp);
    std::string to_string() const;
    std::string to_string_middle() const;
    ~Vpacket_handler___024unit__03a__03apacket_item();
};

std::string VL_TO_STRING(const VlClassRef<Vpacket_handler___024unit__03a__03apacket_item>& obj);

#endif  // guard
