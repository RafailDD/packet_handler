// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design internal header
// See Vpacket_handler.h for the primary calling header

#ifndef VERILATED_VPACKET_HANDLER___024UNIT__03A__03AENV__VCLPKG_H_
#define VERILATED_VPACKET_HANDLER___024UNIT__03A__03AENV__VCLPKG_H_  // guard

#include "verilated.h"
#include "verilated_timing.h"
class Vpacket_handler___024unit__03a__03adriver;
class Vpacket_handler___024unit__03a__03agenerator;
class Vpacket_handler___024unit__03a__03amonitor;
class Vpacket_handler___024unit__03a__03ascoreboard;


class Vpacket_handler__Syms;

class alignas(VL_CACHE_LINE_BYTES) Vpacket_handler___024unit__03a__03aenv__Vclpkg final : public VerilatedModule {
  public:

    // INTERNAL VARIABLES
    Vpacket_handler__Syms* const vlSymsp;

    // CONSTRUCTORS
    Vpacket_handler___024unit__03a__03aenv__Vclpkg(Vpacket_handler__Syms* symsp, const char* v__name);
    ~Vpacket_handler___024unit__03a__03aenv__Vclpkg();
    VL_UNCOPYABLE(Vpacket_handler___024unit__03a__03aenv__Vclpkg);

    // INTERNAL METHODS
    void __Vconfigure(bool first);
};


class Vpacket_handler__Syms;

class Vpacket_handler___024unit__03a__03aenv : public VlClass {
  public:

    // DESIGN SPECIFIC STATE
    VlClassRef<Vpacket_handler___024unit__03a__03agenerator> __PVT__gen;
    VlClassRef<Vpacket_handler___024unit__03a__03adriver> __PVT__drv;
    VlClassRef<Vpacket_handler___024unit__03a__03ascoreboard> __PVT__scb;
    VlClassRef<Vpacket_handler___024unit__03a__03amonitor> __PVT__mon;
  private:
    void _ctor_var_reset(Vpacket_handler__Syms* __restrict vlSymsp);
  public:
    Vpacket_handler___024unit__03a__03aenv(Vpacket_handler__Syms* __restrict vlSymsp, IData/*31:0*/ count);
    std::string to_string() const;
    std::string to_string_middle() const;
    ~Vpacket_handler___024unit__03a__03aenv();
};

std::string VL_TO_STRING(const VlClassRef<Vpacket_handler___024unit__03a__03aenv>& obj);

#endif  // guard
