// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Model implementation (design independent parts)

#include "Vpacket_handler__pch.h"

//============================================================
// Constructors

Vpacket_handler::Vpacket_handler(VerilatedContext* _vcontextp__, const char* _vcname__)
    : VerilatedModel{*_vcontextp__}
    , vlSymsp{new Vpacket_handler__Syms(contextp(), _vcname__, this)}
    , __PVT____024unit{vlSymsp->TOP.__PVT____024unit}
    , __024unit__03a__03apacket_item__Vclpkg{vlSymsp->TOP.__024unit__03a__03apacket_item__Vclpkg}
    , __024unit__03a__03agenerator__Vclpkg{vlSymsp->TOP.__024unit__03a__03agenerator__Vclpkg}
    , __024unit__03a__03adriver__Vclpkg{vlSymsp->TOP.__024unit__03a__03adriver__Vclpkg}
    , __024unit__03a__03ascoreboard__Vclpkg{vlSymsp->TOP.__024unit__03a__03ascoreboard__Vclpkg}
    , __024unit__03a__03amonitor__Vclpkg{vlSymsp->TOP.__024unit__03a__03amonitor__Vclpkg}
    , __024unit__03a__03aenv__Vclpkg{vlSymsp->TOP.__024unit__03a__03aenv__Vclpkg}
    , rootp{&(vlSymsp->TOP)}
{
    // Register model with the context
    contextp()->addModel(this);
}

Vpacket_handler::Vpacket_handler(const char* _vcname__)
    : Vpacket_handler(Verilated::threadContextp(), _vcname__)
{
}

//============================================================
// Destructor

Vpacket_handler::~Vpacket_handler() {
    delete vlSymsp;
}

//============================================================
// Evaluation function

#ifdef VL_DEBUG
void Vpacket_handler___024root___eval_debug_assertions(Vpacket_handler___024root* vlSelf);
#endif  // VL_DEBUG
void Vpacket_handler___024root___eval_static(Vpacket_handler___024root* vlSelf);
void Vpacket_handler___024root___eval_initial(Vpacket_handler___024root* vlSelf);
void Vpacket_handler___024root___eval_settle(Vpacket_handler___024root* vlSelf);
void Vpacket_handler___024root___eval(Vpacket_handler___024root* vlSelf);

void Vpacket_handler::eval_step() {
    VL_DEBUG_IF(VL_DBG_MSGF("+++++TOP Evaluate Vpacket_handler::eval_step\n"); );
#ifdef VL_DEBUG
    // Debug assertions
    Vpacket_handler___024root___eval_debug_assertions(&(vlSymsp->TOP));
#endif  // VL_DEBUG
    vlSymsp->__Vm_deleter.deleteAll();
    if (VL_UNLIKELY(!vlSymsp->__Vm_didInit)) {
        vlSymsp->__Vm_didInit = true;
        VL_DEBUG_IF(VL_DBG_MSGF("+ Initial\n"););
        Vpacket_handler___024root___eval_static(&(vlSymsp->TOP));
        Vpacket_handler___024root___eval_initial(&(vlSymsp->TOP));
        Vpacket_handler___024root___eval_settle(&(vlSymsp->TOP));
    }
    VL_DEBUG_IF(VL_DBG_MSGF("+ Eval\n"););
    Vpacket_handler___024root___eval(&(vlSymsp->TOP));
    // Evaluate cleanup
    Verilated::endOfEval(vlSymsp->__Vm_evalMsgQp);
}

//============================================================
// Events and timing
bool Vpacket_handler::eventsPending() { return !vlSymsp->TOP.__VdlySched.empty(); }

uint64_t Vpacket_handler::nextTimeSlot() { return vlSymsp->TOP.__VdlySched.nextTimeSlot(); }

//============================================================
// Utilities

const char* Vpacket_handler::name() const {
    return vlSymsp->name();
}

//============================================================
// Invoke final blocks

void Vpacket_handler___024root___eval_final(Vpacket_handler___024root* vlSelf);

VL_ATTR_COLD void Vpacket_handler::final() {
    Vpacket_handler___024root___eval_final(&(vlSymsp->TOP));
}

//============================================================
// Implementations of abstract methods from VerilatedModel

const char* Vpacket_handler::hierName() const { return vlSymsp->name(); }
const char* Vpacket_handler::modelName() const { return "Vpacket_handler"; }
unsigned Vpacket_handler::threads() const { return 1; }
void Vpacket_handler::prepareClone() const { contextp()->prepareClone(); }
void Vpacket_handler::atClone() const {
    contextp()->threadPoolpOnClone();
}

//============================================================
// Trace configuration

VL_ATTR_COLD void Vpacket_handler::trace(VerilatedVcdC* tfp, int levels, int options) {
    vl_fatal(__FILE__, __LINE__, __FILE__,"'Vpacket_handler::trace()' called on model that was Verilated without --trace option");
}
