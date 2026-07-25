// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design internal header
// See Vpacket_handler.h for the primary calling header

#ifndef VERILATED_VPACKET_HANDLER___024UNIT__03A__03AGENERATOR__VCLPKG_H_
#define VERILATED_VPACKET_HANDLER___024UNIT__03A__03AGENERATOR__VCLPKG_H_  // guard

#include "verilated.h"
#include "verilated_timing.h"
class Vpacket_handler___024unit__03a__03apacket_item;


class Vpacket_handler__Syms;

class alignas(VL_CACHE_LINE_BYTES) Vpacket_handler___024unit__03a__03agenerator__Vclpkg final : public VerilatedModule {
  public:

    // INTERNAL VARIABLES
    Vpacket_handler__Syms* const vlSymsp;

    // CONSTRUCTORS
    Vpacket_handler___024unit__03a__03agenerator__Vclpkg(Vpacket_handler__Syms* symsp, const char* v__name);
    ~Vpacket_handler___024unit__03a__03agenerator__Vclpkg();
    VL_UNCOPYABLE(Vpacket_handler___024unit__03a__03agenerator__Vclpkg);

    // INTERNAL METHODS
    void __Vconfigure(bool first);
};


class Vpacket_handler__Syms;

class Vpacket_handler___024unit__03a__03agenerator : public VlClass {
  public:

    // DESIGN SPECIFIC STATE
    IData/*31:0*/ __PVT__count;
    VlQueue<VlClassRef<Vpacket_handler___024unit__03a__03apacket_item>> __PVT__pkt_q;
    void __VnoInFunc_generate_items(Vpacket_handler__Syms* __restrict vlSymsp);
    void __VnoInFunc_get_next(Vpacket_handler__Syms* __restrict vlSymsp, VlClassRef<Vpacket_handler___024unit__03a__03apacket_item> &get_next__Vfuncrtn);
  private:
    void _ctor_var_reset(Vpacket_handler__Syms* __restrict vlSymsp);
  public:
    Vpacket_handler___024unit__03a__03agenerator(Vpacket_handler__Syms* __restrict vlSymsp, IData/*31:0*/ c);
    std::string to_string() const;
    std::string to_string_middle() const;
    ~Vpacket_handler___024unit__03a__03agenerator();
};

std::string VL_TO_STRING(const VlClassRef<Vpacket_handler___024unit__03a__03agenerator>& obj);

#endif  // guard
