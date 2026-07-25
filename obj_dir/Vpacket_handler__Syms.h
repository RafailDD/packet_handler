// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Symbol table internal header
//
// Internal details; most calling programs do not need this header,
// unless using verilator public meta comments.

#ifndef VERILATED_VPACKET_HANDLER__SYMS_H_
#define VERILATED_VPACKET_HANDLER__SYMS_H_  // guard

#include "verilated.h"

// INCLUDE MODEL CLASS

#include "Vpacket_handler.h"

// INCLUDE MODULE CLASSES
#include "Vpacket_handler___024root.h"
#include "Vpacket_handler___024unit.h"
#include "Vpacket_handler___024unit__03a__03apacket_item__Vclpkg.h"
#include "Vpacket_handler___024unit__03a__03agenerator__Vclpkg.h"
#include "Vpacket_handler___024unit__03a__03adriver__Vclpkg.h"
#include "Vpacket_handler___024unit__03a__03ascoreboard__Vclpkg.h"
#include "Vpacket_handler___024unit__03a__03amonitor__Vclpkg.h"
#include "Vpacket_handler___024unit__03a__03aenv__Vclpkg.h"

// SYMS CLASS (contains all model state)
class alignas(VL_CACHE_LINE_BYTES)Vpacket_handler__Syms final : public VerilatedSyms {
  public:
    // INTERNAL STATE
    Vpacket_handler* const __Vm_modelp;
    VlDeleter __Vm_deleter;
    bool __Vm_didInit = false;

    // MODULE INSTANCE STATE
    Vpacket_handler___024root      TOP;
    Vpacket_handler___024unit__03a__03adriver__Vclpkg TOP____024unit__03a__03adriver__Vclpkg;
    Vpacket_handler___024unit__03a__03aenv__Vclpkg TOP____024unit__03a__03aenv__Vclpkg;
    Vpacket_handler___024unit__03a__03agenerator__Vclpkg TOP____024unit__03a__03agenerator__Vclpkg;
    Vpacket_handler___024unit__03a__03amonitor__Vclpkg TOP____024unit__03a__03amonitor__Vclpkg;
    Vpacket_handler___024unit__03a__03apacket_item__Vclpkg TOP____024unit__03a__03apacket_item__Vclpkg;
    Vpacket_handler___024unit__03a__03ascoreboard__Vclpkg TOP____024unit__03a__03ascoreboard__Vclpkg;
    Vpacket_handler___024unit      TOP____024unit;

    // SCOPE NAMES
    VerilatedScope __Vscope___024unit__scoreboard;
    VerilatedScope __Vscope___024unit__scoreboard__check_actual;

    // CONSTRUCTORS
    Vpacket_handler__Syms(VerilatedContext* contextp, const char* namep, Vpacket_handler* modelp);
    ~Vpacket_handler__Syms();

    // METHODS
    const char* name() { return TOP.name(); }
};

#endif  // guard
