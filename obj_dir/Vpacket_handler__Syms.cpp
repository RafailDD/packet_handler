// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Symbol table implementation internals

#include "Vpacket_handler__pch.h"
#include "Vpacket_handler.h"
#include "Vpacket_handler___024root.h"
#include "Vpacket_handler___024unit.h"
#include "Vpacket_handler___024unit__03a__03apacket_item__Vclpkg.h"
#include "Vpacket_handler___024unit__03a__03agenerator__Vclpkg.h"
#include "Vpacket_handler___024unit__03a__03adriver__Vclpkg.h"
#include "Vpacket_handler___024unit__03a__03ascoreboard__Vclpkg.h"
#include "Vpacket_handler___024unit__03a__03amonitor__Vclpkg.h"
#include "Vpacket_handler___024unit__03a__03aenv__Vclpkg.h"

// FUNCTIONS
Vpacket_handler__Syms::~Vpacket_handler__Syms()
{
}

Vpacket_handler__Syms::Vpacket_handler__Syms(VerilatedContext* contextp, const char* namep, Vpacket_handler* modelp)
    : VerilatedSyms{contextp}
    // Setup internal state of the Syms class
    , __Vm_modelp{modelp}
    // Setup module instances
    , TOP{this, namep}
    , TOP____024unit__03a__03adriver__Vclpkg{this, Verilated::catName(namep, "$unit::driver__Vclpkg")}
    , TOP____024unit__03a__03aenv__Vclpkg{this, Verilated::catName(namep, "$unit::env__Vclpkg")}
    , TOP____024unit__03a__03agenerator__Vclpkg{this, Verilated::catName(namep, "$unit::generator__Vclpkg")}
    , TOP____024unit__03a__03amonitor__Vclpkg{this, Verilated::catName(namep, "$unit::monitor__Vclpkg")}
    , TOP____024unit__03a__03apacket_item__Vclpkg{this, Verilated::catName(namep, "$unit::packet_item__Vclpkg")}
    , TOP____024unit__03a__03ascoreboard__Vclpkg{this, Verilated::catName(namep, "$unit::scoreboard__Vclpkg")}
    , TOP____024unit{this, Verilated::catName(namep, "$unit")}
{
    // Configure time unit / time precision
    _vm_contextp__->timeunit(-12);
    _vm_contextp__->timeprecision(-12);
    // Setup each module's pointers to their submodules
    TOP.__024unit__03a__03adriver__Vclpkg = &TOP____024unit__03a__03adriver__Vclpkg;
    TOP.__024unit__03a__03aenv__Vclpkg = &TOP____024unit__03a__03aenv__Vclpkg;
    TOP.__024unit__03a__03agenerator__Vclpkg = &TOP____024unit__03a__03agenerator__Vclpkg;
    TOP.__024unit__03a__03amonitor__Vclpkg = &TOP____024unit__03a__03amonitor__Vclpkg;
    TOP.__024unit__03a__03apacket_item__Vclpkg = &TOP____024unit__03a__03apacket_item__Vclpkg;
    TOP.__024unit__03a__03ascoreboard__Vclpkg = &TOP____024unit__03a__03ascoreboard__Vclpkg;
    TOP.__PVT____024unit = &TOP____024unit;
    // Setup each module's pointer back to symbol table (for public functions)
    TOP.__Vconfigure(true);
    TOP____024unit__03a__03adriver__Vclpkg.__Vconfigure(true);
    TOP____024unit__03a__03aenv__Vclpkg.__Vconfigure(true);
    TOP____024unit__03a__03agenerator__Vclpkg.__Vconfigure(true);
    TOP____024unit__03a__03amonitor__Vclpkg.__Vconfigure(true);
    TOP____024unit__03a__03apacket_item__Vclpkg.__Vconfigure(true);
    TOP____024unit__03a__03ascoreboard__Vclpkg.__Vconfigure(true);
    TOP____024unit.__Vconfigure(true);
    // Setup scopes
    __Vscope___024unit__scoreboard.configure(this, name(), "$unit.scoreboard", "scoreboard", -12, VerilatedScope::SCOPE_OTHER);
    __Vscope___024unit__scoreboard__check_actual.configure(this, name(), "$unit.scoreboard.check_actual", "check_actual", -12, VerilatedScope::SCOPE_OTHER);
}
