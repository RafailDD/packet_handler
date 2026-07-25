// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vpacket_handler.h for the primary calling header

#include "Vpacket_handler__pch.h"
#include "Vpacket_handler__Syms.h"
#include "Vpacket_handler___024root.h"
#include "Vpacket_handler___024unit__03a__03adriver__Vclpkg.h"
#include "Vpacket_handler___024unit__03a__03aenv__Vclpkg.h"
#include "Vpacket_handler___024unit__03a__03agenerator__Vclpkg.h"
#include "Vpacket_handler___024unit__03a__03amonitor__Vclpkg.h"
#include "Vpacket_handler___024unit__03a__03apacket_item__Vclpkg.h"
#include "Vpacket_handler___024unit__03a__03ascoreboard__Vclpkg.h"

VL_INLINE_OPT VlCoroutine Vpacket_handler___024root___eval_initial__TOP__Vtiming__1(Vpacket_handler___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vpacket_handler__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vpacket_handler___024root___eval_initial__TOP__Vtiming__1\n"); );
    // Init
    IData/*31:0*/ tb_verilator_uvm__DOT__unnamedblk2__DOT__i;
    tb_verilator_uvm__DOT__unnamedblk2__DOT__i = 0;
    VlClassRef<Vpacket_handler___024unit__03a__03apacket_item> __Vtask_get_next__4__Vfuncout;
    VlClassRef<Vpacket_handler___024unit__03a__03apacket_item> __Vtask_tb_verilator_uvm__DOT__send_packet__7__p;
    IData/*31:0*/ __Vtask_tb_verilator_uvm__DOT__send_packet__7__unnamedblk1__DOT__i;
    __Vtask_tb_verilator_uvm__DOT__send_packet__7__unnamedblk1__DOT__i = 0;
    // Body
    vlSelf->tb_verilator_uvm__DOT__rst_n = 0U;
    vlSelf->tb_verilator_uvm__DOT__i_data = 0U;
    vlSelf->tb_verilator_uvm__DOT__i_valid = 0U;
    vlSelf->tb_verilator_uvm__DOT__i_last = 0U;
    vlSelf->tb_verilator_uvm__DOT__i_ready = 1U;
    co_await vlSelf->__VdlySched.delay(0x14ULL, nullptr,
                                       "tests/tb_uvm_sv.sv",
                                       247);
    vlSelf->tb_verilator_uvm__DOT__rst_n = 1U;
    vlSelf->tb_verilator_uvm__DOT__environment = VL_NEW(Vpacket_handler___024unit__03a__03aenv, vlSymsp, 0x64U);
    VL_NULL_CHECK(VL_NULL_CHECK(vlSelf->tb_verilator_uvm__DOT__environment, "tests/tb_uvm_sv.sv", 250)
                  ->__PVT__gen, "tests/tb_uvm_sv.sv", 250)->__VnoInFunc_generate_items(vlSymsp);
    tb_verilator_uvm__DOT__unnamedblk2__DOT__i = 0U;
    while (VL_GTS_III(32, 0x64U, tb_verilator_uvm__DOT__unnamedblk2__DOT__i)) {
        VL_NULL_CHECK(VL_NULL_CHECK(vlSelf->tb_verilator_uvm__DOT__environment, "tests/tb_uvm_sv.sv", 253)
                      ->__PVT__gen, "tests/tb_uvm_sv.sv", 253)->__VnoInFunc_get_next(vlSymsp, __Vtask_get_next__4__Vfuncout);
        vlSelf->tb_verilator_uvm__DOT__unnamedblk2__DOT__unnamedblk3__DOT__p
            = __Vtask_get_next__4__Vfuncout;
        VL_NULL_CHECK(VL_NULL_CHECK(vlSelf->tb_verilator_uvm__DOT__environment, "tests/tb_uvm_sv.sv", 254)
                      ->__PVT__scb, "tests/tb_uvm_sv.sv", 254)->__VnoInFunc_push_expected(vlSymsp, vlSelf->tb_verilator_uvm__DOT__unnamedblk2__DOT__unnamedblk3__DOT__p);
        VL_NULL_CHECK(VL_NULL_CHECK(vlSelf->tb_verilator_uvm__DOT__environment, "tests/tb_uvm_sv.sv", 256)
                      ->__PVT__mon, "tests/tb_uvm_sv.sv", 256)->__VnoInFunc_sample(vlSymsp, VL_NULL_CHECK(vlSelf->tb_verilator_uvm__DOT__unnamedblk2__DOT__unnamedblk3__DOT__p, "tests/tb_uvm_sv.sv", 256)
                                                                                ->__PVT__data_size, 0U);
        __Vtask_tb_verilator_uvm__DOT__send_packet__7__p
            = vlSelf->tb_verilator_uvm__DOT__unnamedblk2__DOT__unnamedblk3__DOT__p;
        while ((0x100U & (IData)(vlSelf->tb_verilator_uvm__DOT__dut__DOT__fifo_count))) {
            co_await vlSelf->__VtrigSched_h3539bd04__0.trigger(0U,
                                                               nullptr,
                                                               "@(posedge tb_verilator_uvm.clk)",
                                                               "tests/tb_uvm_sv.sv",
                                                               191);
        }
        vlSelf->tb_verilator_uvm__DOT__i_data = (((
                                                   (0xff000000U
                                                    & VL_SHIFTL_III(32,32,32, (IData)(VL_NULL_CHECK(__Vtask_tb_verilator_uvm__DOT__send_packet__7__p, "tests/tb_uvm_sv.sv", 195)
                                                                                ->__PVT__msgLength), 0x18U))
                                                   | (0xffff0000U
                                                      & VL_SHIFTL_III(32,32,32, (IData)(VL_NULL_CHECK(__Vtask_tb_verilator_uvm__DOT__send_packet__7__p, "tests/tb_uvm_sv.sv", 195)
                                                                                ->__PVT__msgLength), 8U)))
                                                  | (0xffff00U
                                                     & VL_SHIFTL_III(32,32,32, (IData)(VL_NULL_CHECK(__Vtask_tb_verilator_uvm__DOT__send_packet__7__p, "tests/tb_uvm_sv.sv", 196)
                                                                                ->__PVT__streamId), 8U)))
                                                 | (0xffffU
                                                    & VL_SHIFTR_III(32,32,32, VL_NULL_CHECK(__Vtask_tb_verilator_uvm__DOT__send_packet__7__p, "tests/tb_uvm_sv.sv", 196)
                                                                    ->__PVT__streamId, 8U)));
        vlSelf->tb_verilator_uvm__DOT__i_valid = 1U;
        vlSelf->tb_verilator_uvm__DOT__i_last = 0U;
        co_await vlSelf->__VtrigSched_h3539bd04__0.trigger(0U,
                                                           nullptr,
                                                           "@(posedge tb_verilator_uvm.clk)",
                                                           "tests/tb_uvm_sv.sv",
                                                           200);
        vlSelf->tb_verilator_uvm__DOT__i_data = (((
                                                   (0xff000000U
                                                    & VL_SHIFTL_III(32,32,32, (IData)(VL_NULL_CHECK(__Vtask_tb_verilator_uvm__DOT__send_packet__7__p, "tests/tb_uvm_sv.sv", 202)
                                                                                ->__PVT__seqNumber), 0x18U))
                                                   | (0xff0000U
                                                      & VL_SHIFTL_III(32,32,32, (IData)(VL_NULL_CHECK(__Vtask_tb_verilator_uvm__DOT__send_packet__7__p, "tests/tb_uvm_sv.sv", 202)
                                                                                ->__PVT__seqNumber), 8U)))
                                                  | (0xff00U
                                                     & VL_SHIFTR_III(32,32,32, VL_NULL_CHECK(__Vtask_tb_verilator_uvm__DOT__send_packet__7__p, "tests/tb_uvm_sv.sv", 203)
                                                                     ->__PVT__seqNumber, 8U)))
                                                 | (0xffU
                                                    & VL_SHIFTR_III(32,32,32, VL_NULL_CHECK(__Vtask_tb_verilator_uvm__DOT__send_packet__7__p, "tests/tb_uvm_sv.sv", 203)
                                                                    ->__PVT__seqNumber, 0x18U)));
        vlSelf->tb_verilator_uvm__DOT__i_valid = 1U;
        co_await vlSelf->__VtrigSched_h3539bd04__0.trigger(0U,
                                                           nullptr,
                                                           "@(posedge tb_verilator_uvm.clk)",
                                                           "tests/tb_uvm_sv.sv",
                                                           205);
        __Vtask_tb_verilator_uvm__DOT__send_packet__7__unnamedblk1__DOT__i = 0U;
        while (VL_LTS_III(32, __Vtask_tb_verilator_uvm__DOT__send_packet__7__unnamedblk1__DOT__i, VL_NULL_CHECK(__Vtask_tb_verilator_uvm__DOT__send_packet__7__p, "tests/tb_uvm_sv.sv", 207)
                          ->__PVT__data_size)) {
            vlSelf->tb_verilator_uvm__DOT__i_data =
                ((8U >= (0xfU & __Vtask_tb_verilator_uvm__DOT__send_packet__7__unnamedblk1__DOT__i))
                  ? VL_NULL_CHECK(__Vtask_tb_verilator_uvm__DOT__send_packet__7__p, "tests/tb_uvm_sv.sv", 208)
                 ->__PVT__data_arr[(0xfU & __Vtask_tb_verilator_uvm__DOT__send_packet__7__unnamedblk1__DOT__i)]
                  : 0U);
            vlSelf->tb_verilator_uvm__DOT__i_valid = 1U;
            vlSelf->tb_verilator_uvm__DOT__i_last =
                (__Vtask_tb_verilator_uvm__DOT__send_packet__7__unnamedblk1__DOT__i
                 == ((IData)(VL_NULL_CHECK(__Vtask_tb_verilator_uvm__DOT__send_packet__7__p, "tests/tb_uvm_sv.sv", 210)
                             ->__PVT__data_size) - (IData)(1U)));
            co_await vlSelf->__VtrigSched_h3539bd04__0.trigger(0U,
                                                               nullptr,
                                                               "@(posedge tb_verilator_uvm.clk)",
                                                               "tests/tb_uvm_sv.sv",
                                                               215);
            __Vtask_tb_verilator_uvm__DOT__send_packet__7__unnamedblk1__DOT__i
                = ((IData)(1U) + __Vtask_tb_verilator_uvm__DOT__send_packet__7__unnamedblk1__DOT__i);
        }
        vlSelf->tb_verilator_uvm__DOT__i_valid = 0U;
        vlSelf->tb_verilator_uvm__DOT__i_last = 0U;
        VL_NULL_CHECK(VL_NULL_CHECK(vlSelf->tb_verilator_uvm__DOT__environment, "tests/tb_uvm_sv.sv", 220)
                      ->__PVT__drv, "tests/tb_uvm_sv.sv", 220)->__PVT__packets_sent
            = ((IData)(1U) + (IData)(VL_NULL_CHECK(VL_NULL_CHECK(vlSelf->tb_verilator_uvm__DOT__environment, "tests/tb_uvm_sv.sv", 220)
                                                   ->__PVT__drv, "tests/tb_uvm_sv.sv", 220)
                                     ->__PVT__packets_sent));
        vlSelf->tb_verilator_uvm__DOT__unnamedblk2__DOT__unnamedblk3__DOT__unnamedblk4__DOT__j = 0U;
        while ((vlSelf->tb_verilator_uvm__DOT__unnamedblk2__DOT__unnamedblk3__DOT__unnamedblk4__DOT__j
                < VL_MODDIV_III(32, (IData)(VL_RANDOM_I()), (IData)(0xaU)))) {
            co_await vlSelf->__VtrigSched_h3539bd04__0.trigger(0U,
                                                               nullptr,
                                                               "@(posedge tb_verilator_uvm.clk)",
                                                               "tests/tb_uvm_sv.sv",
                                                               260);
            vlSelf->tb_verilator_uvm__DOT__unnamedblk2__DOT__unnamedblk3__DOT__unnamedblk4__DOT__j
                = ((IData)(1U) + vlSelf->tb_verilator_uvm__DOT__unnamedblk2__DOT__unnamedblk3__DOT__unnamedblk4__DOT__j);
        }
        tb_verilator_uvm__DOT__unnamedblk2__DOT__i
            = ((IData)(1U) + tb_verilator_uvm__DOT__unnamedblk2__DOT__i);
    }
    while (VL_LTS_III(32, VL_NULL_CHECK(VL_NULL_CHECK(vlSelf->tb_verilator_uvm__DOT__environment, "tests/tb_uvm_sv.sv", 263)
                                        ->__PVT__scb, "tests/tb_uvm_sv.sv", 263)
                      ->__PVT__packets_received, VL_NULL_CHECK(VL_NULL_CHECK(vlSelf->tb_verilator_uvm__DOT__environment, "tests/tb_uvm_sv.sv", 263)
                                                               ->__PVT__drv, "tests/tb_uvm_sv.sv", 263)
                      ->__PVT__packets_sent)) {
        co_await vlSelf->__VtrigSched_h3539bd04__0.trigger(0U,
                                                           nullptr,
                                                           "@(posedge tb_verilator_uvm.clk)",
                                                           "tests/tb_uvm_sv.sv",
                                                           264);
    }
    VL_NULL_CHECK(VL_NULL_CHECK(vlSelf->tb_verilator_uvm__DOT__environment, "tests/tb_uvm_sv.sv", 267)
                  ->__PVT__mon, "tests/tb_uvm_sv.sv", 267)->__VnoInFunc_report_coverage(vlSymsp);
    VL_WRITEF("TEST PASSED: All %0d packets generated, driven, monitored, and scored successfully.\n",
              32,VL_NULL_CHECK(VL_NULL_CHECK(vlSelf->tb_verilator_uvm__DOT__environment, "tests/tb_uvm_sv.sv", 269)
                               ->__PVT__scb, "tests/tb_uvm_sv.sv", 269)
              ->__PVT__packets_received);
    co_await vlSelf->__VdlySched.delay(0x32ULL, nullptr,
                                       "tests/tb_uvm_sv.sv",
                                       270);
    VL_FINISH_MT("tests/tb_uvm_sv.sv", 271, "");
}
