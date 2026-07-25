// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vpacket_handler.h for the primary calling header

#include "Vpacket_handler__pch.h"
#include "Vpacket_handler__Syms.h"
#include "Vpacket_handler___024unit__03a__03apacket_item__Vclpkg.h"
#include "Vpacket_handler___024unit__03a__03ascoreboard__Vclpkg.h"

extern const VlWide<10>/*319:0*/ Vpacket_handler__ConstPool__CONST_ha8c708d0_0;
extern const VlWide<10>/*319:0*/ Vpacket_handler__ConstPool__CONST_h3c9d8e1d_0;

void Vpacket_handler___024unit__03a__03ascoreboard::__VnoInFunc_check_actual(Vpacket_handler__Syms* __restrict vlSymsp, VlWide<10>/*295:0*/ actual_data) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vpacket_handler___024unit__03a__03ascoreboard::__VnoInFunc_check_actual\n"); );
    // Init
    VlWide<10>/*319:0*/ __Vtemp_1;
    VlWide<10>/*319:0*/ __Vtemp_2;
    // Body
    VlClassRef<Vpacket_handler___024unit__03a__03apacket_item> unnamedblk1__DOT__expected_pkt;
    VlWide<10>/*295:0*/ unnamedblk1__DOT__reconstructed_expected;
    VL_ZERO_W(296, unnamedblk1__DOT__reconstructed_expected);
    IData/*31:0*/ unnamedblk1__DOT__unnamedblk2__DOT__i;
    unnamedblk1__DOT__unnamedblk2__DOT__i = 0;
    IData/*31:0*/ unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__shift_amt;
    unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__shift_amt = 0;
    VlWide<10>/*295:0*/ unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__ext_data;
    VL_ZERO_W(296, unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__ext_data);
    if (VL_LIKELY(VL_LTS_III(32, 0U, this->__PVT__expected_q.size()))) {
        unnamedblk1__DOT__expected_pkt = this->__PVT__expected_q.pop_front();
        unnamedblk1__DOT__reconstructed_expected[0U]
            = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[0U];
        unnamedblk1__DOT__reconstructed_expected[1U]
            = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[1U];
        unnamedblk1__DOT__reconstructed_expected[2U]
            = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[2U];
        unnamedblk1__DOT__reconstructed_expected[3U]
            = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[3U];
        unnamedblk1__DOT__reconstructed_expected[4U]
            = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[4U];
        unnamedblk1__DOT__reconstructed_expected[5U]
            = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[5U];
        unnamedblk1__DOT__reconstructed_expected[6U]
            = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[6U];
        unnamedblk1__DOT__reconstructed_expected[7U]
            = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[7U];
        unnamedblk1__DOT__reconstructed_expected[8U]
            = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[8U];
        unnamedblk1__DOT__reconstructed_expected[9U]
            = Vpacket_handler__ConstPool__CONST_ha8c708d0_0[9U];
        unnamedblk1__DOT__unnamedblk2__DOT__i = 0U;
        while (VL_LTS_III(32, unnamedblk1__DOT__unnamedblk2__DOT__i, VL_NULL_CHECK(unnamedblk1__DOT__expected_pkt, "tests/tb_uvm_sv.sv", 76)
                          ->__PVT__data_size)) {
            unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__shift_amt
                = VL_MULS_III(32, (IData)(0x20U), ((IData)(
                                                           ((IData)(VL_NULL_CHECK(unnamedblk1__DOT__expected_pkt, "tests/tb_uvm_sv.sv", 77)
                                                                    ->__PVT__data_size)
                                                            - (IData)(1U)))
                                                   - unnamedblk1__DOT__unnamedblk2__DOT__i));
            unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__ext_data[0U]
                = ((8U >= (0xfU & unnamedblk1__DOT__unnamedblk2__DOT__i))
                    ? VL_NULL_CHECK(unnamedblk1__DOT__expected_pkt, "tests/tb_uvm_sv.sv", 79)
                   ->__PVT__data_arr[(0xfU & unnamedblk1__DOT__unnamedblk2__DOT__i)]
                    : 0U);
            unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__ext_data[1U] = 0U;
            unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__ext_data[2U] = 0U;
            unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__ext_data[3U] = 0U;
            unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__ext_data[4U] = 0U;
            unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__ext_data[5U] = 0U;
            unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__ext_data[6U] = 0U;
            unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__ext_data[7U] = 0U;
            unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__ext_data[8U] = 0U;
            unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__ext_data[9U] = 0U;
            VL_SHIFTL_WWI(296,296,32, __Vtemp_2, unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__ext_data, unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__shift_amt);
            __Vtemp_1[1U] = (Vpacket_handler__ConstPool__CONST_h3c9d8e1d_0[1U]
                             & (unnamedblk1__DOT__reconstructed_expected[1U]
                                | __Vtemp_2[1U]));
            __Vtemp_1[2U] = (Vpacket_handler__ConstPool__CONST_h3c9d8e1d_0[2U]
                             & (unnamedblk1__DOT__reconstructed_expected[2U]
                                | __Vtemp_2[2U]));
            __Vtemp_1[3U] = (Vpacket_handler__ConstPool__CONST_h3c9d8e1d_0[3U]
                             & (unnamedblk1__DOT__reconstructed_expected[3U]
                                | __Vtemp_2[3U]));
            __Vtemp_1[4U] = (Vpacket_handler__ConstPool__CONST_h3c9d8e1d_0[4U]
                             & (unnamedblk1__DOT__reconstructed_expected[4U]
                                | __Vtemp_2[4U]));
            __Vtemp_1[5U] = (Vpacket_handler__ConstPool__CONST_h3c9d8e1d_0[5U]
                             & (unnamedblk1__DOT__reconstructed_expected[5U]
                                | __Vtemp_2[5U]));
            __Vtemp_1[6U] = (Vpacket_handler__ConstPool__CONST_h3c9d8e1d_0[6U]
                             & (unnamedblk1__DOT__reconstructed_expected[6U]
                                | __Vtemp_2[6U]));
            __Vtemp_1[7U] = (Vpacket_handler__ConstPool__CONST_h3c9d8e1d_0[7U]
                             & (unnamedblk1__DOT__reconstructed_expected[7U]
                                | __Vtemp_2[7U]));
            __Vtemp_1[8U] = (Vpacket_handler__ConstPool__CONST_h3c9d8e1d_0[8U]
                             & (unnamedblk1__DOT__reconstructed_expected[8U]
                                | __Vtemp_2[8U]));
            __Vtemp_1[9U] = (Vpacket_handler__ConstPool__CONST_h3c9d8e1d_0[9U]
                             & (unnamedblk1__DOT__reconstructed_expected[9U]
                                | __Vtemp_2[9U]));
            unnamedblk1__DOT__reconstructed_expected[0U]
                = (Vpacket_handler__ConstPool__CONST_h3c9d8e1d_0[0U]
                   & (unnamedblk1__DOT__reconstructed_expected[0U]
                      | __Vtemp_2[0U]));
            unnamedblk1__DOT__reconstructed_expected[1U]
                = __Vtemp_1[1U];
            unnamedblk1__DOT__reconstructed_expected[2U]
                = __Vtemp_1[2U];
            unnamedblk1__DOT__reconstructed_expected[3U]
                = __Vtemp_1[3U];
            unnamedblk1__DOT__reconstructed_expected[4U]
                = __Vtemp_1[4U];
            unnamedblk1__DOT__reconstructed_expected[5U]
                = __Vtemp_1[5U];
            unnamedblk1__DOT__reconstructed_expected[6U]
                = __Vtemp_1[6U];
            unnamedblk1__DOT__reconstructed_expected[7U]
                = __Vtemp_1[7U];
            unnamedblk1__DOT__reconstructed_expected[8U]
                = __Vtemp_1[8U];
            unnamedblk1__DOT__reconstructed_expected[9U]
                = __Vtemp_1[9U];
            unnamedblk1__DOT__unnamedblk2__DOT__i =
                ((IData)(1U) + unnamedblk1__DOT__unnamedblk2__DOT__i);
        }
        this->__PVT__packets_received = ((IData)(1U)
                                         + this->__PVT__packets_received);
    } else {
        VL_WRITEF("[%0t] %%Error: tb_uvm_sv.sv:98: Assertion failed in %N$unit.scoreboard.check_actual: Scoreboard: Received unexpected packet!\n",
                  64,VL_TIME_UNITED_Q(1),-12,vlSymsp->name());
        VL_STOP_MT("tests/tb_uvm_sv.sv", 98, "");
    }
}
