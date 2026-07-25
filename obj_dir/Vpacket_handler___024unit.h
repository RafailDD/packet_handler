// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design internal header
// See Vpacket_handler.h for the primary calling header

#ifndef VERILATED_VPACKET_HANDLER___024UNIT_H_
#define VERILATED_VPACKET_HANDLER___024UNIT_H_  // guard

#include "verilated.h"
#include "verilated_timing.h"


class Vpacket_handler__Syms;

class alignas(VL_CACHE_LINE_BYTES) Vpacket_handler___024unit final : public VerilatedModule {
  public:

    // INTERNAL VARIABLES
    Vpacket_handler__Syms* const vlSymsp;

    // CONSTRUCTORS
    Vpacket_handler___024unit(Vpacket_handler__Syms* symsp, const char* v__name);
    ~Vpacket_handler___024unit();
    VL_UNCOPYABLE(Vpacket_handler___024unit);

    // INTERNAL METHODS
    void __Vconfigure(bool first);
};


#endif  // guard
