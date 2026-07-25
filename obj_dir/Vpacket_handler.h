// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Primary model header
//
// This header should be included by all source files instantiating the design.
// The class here is then constructed to instantiate the design.
// See the Verilator manual for examples.

#ifndef VERILATED_VPACKET_HANDLER_H_
#define VERILATED_VPACKET_HANDLER_H_  // guard

#include "verilated.h"

class Vpacket_handler__Syms;
class Vpacket_handler___024root;
class Vpacket_handler___024unit;
class Vpacket_handler___024unit__03a__03adriver;
class Vpacket_handler___024unit__03a__03adriver__Vclpkg;
class Vpacket_handler___024unit__03a__03aenv;
class Vpacket_handler___024unit__03a__03aenv__Vclpkg;
class Vpacket_handler___024unit__03a__03agenerator;
class Vpacket_handler___024unit__03a__03agenerator__Vclpkg;
class Vpacket_handler___024unit__03a__03amonitor;
class Vpacket_handler___024unit__03a__03amonitor__Vclpkg;
class Vpacket_handler___024unit__03a__03apacket_item;
class Vpacket_handler___024unit__03a__03apacket_item__Vclpkg;
class Vpacket_handler___024unit__03a__03ascoreboard;
class Vpacket_handler___024unit__03a__03ascoreboard__Vclpkg;


// This class is the main interface to the Verilated model
class alignas(VL_CACHE_LINE_BYTES) Vpacket_handler VL_NOT_FINAL : public VerilatedModel {
  private:
    // Symbol table holding complete model state (owned by this class)
    Vpacket_handler__Syms* const vlSymsp;

  public:

    // PORTS
    // The application code writes and reads these signals to
    // propagate new values into/out from the Verilated model.

    // CELLS
    // Public to allow access to /* verilator public */ items.
    // Otherwise the application code can consider these internals.
    Vpacket_handler___024unit* const __PVT____024unit;
    Vpacket_handler___024unit__03a__03apacket_item__Vclpkg* const __024unit__03a__03apacket_item__Vclpkg;
    Vpacket_handler___024unit__03a__03agenerator__Vclpkg* const __024unit__03a__03agenerator__Vclpkg;
    Vpacket_handler___024unit__03a__03adriver__Vclpkg* const __024unit__03a__03adriver__Vclpkg;
    Vpacket_handler___024unit__03a__03ascoreboard__Vclpkg* const __024unit__03a__03ascoreboard__Vclpkg;
    Vpacket_handler___024unit__03a__03amonitor__Vclpkg* const __024unit__03a__03amonitor__Vclpkg;
    Vpacket_handler___024unit__03a__03aenv__Vclpkg* const __024unit__03a__03aenv__Vclpkg;

    // Root instance pointer to allow access to model internals,
    // including inlined /* verilator public_flat_* */ items.
    Vpacket_handler___024root* const rootp;

    // CONSTRUCTORS
    /// Construct the model; called by application code
    /// If contextp is null, then the model will use the default global context
    /// If name is "", then makes a wrapper with a
    /// single model invisible with respect to DPI scope names.
    explicit Vpacket_handler(VerilatedContext* contextp, const char* name = "TOP");
    explicit Vpacket_handler(const char* name = "TOP");
    /// Destroy the model; called (often implicitly) by application code
    virtual ~Vpacket_handler();
  private:
    VL_UNCOPYABLE(Vpacket_handler);  ///< Copying not allowed

  public:
    // API METHODS
    /// Evaluate the model.  Application must call when inputs change.
    void eval() { eval_step(); }
    /// Evaluate when calling multiple units/models per time step.
    void eval_step();
    /// Evaluate at end of a timestep for tracing, when using eval_step().
    /// Application must call after all eval() and before time changes.
    void eval_end_step() {}
    /// Simulation complete, run final blocks.  Application must call on completion.
    void final();
    /// Are there scheduled events to handle?
    bool eventsPending();
    /// Returns time at next time slot. Aborts if !eventsPending()
    uint64_t nextTimeSlot();
    /// Trace signals in the model; called by application code
    void trace(VerilatedVcdC* tfp, int levels, int options = 0);
    /// Retrieve name of this model instance (as passed to constructor).
    const char* name() const;

    // Abstract methods from VerilatedModel
    const char* hierName() const override final;
    const char* modelName() const override final;
    unsigned threads() const override final;
    /// Prepare for cloning the model at the process level (e.g. fork in Linux)
    /// Release necessary resources. Called before cloning.
    void prepareClone() const;
    /// Re-init after cloning the model at the process level (e.g. fork in Linux)
    /// Re-allocate necessary resources. Called after cloning.
    void atClone() const;
};

#endif  // guard
