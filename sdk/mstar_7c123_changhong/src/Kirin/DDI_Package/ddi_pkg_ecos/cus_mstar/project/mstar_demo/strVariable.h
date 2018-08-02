//<MStar Software>
//******************************************************************************
// MStar Software
// Copyright (c) 2010 - 2012 MStar Semiconductor, Inc. All rights reserved.
// All software, firmware and related documentation herein ("MStar Software") are
// intellectual property of MStar Semiconductor, Inc. ("MStar") and protected by
// law, including, but not limited to, copyright law and international treaties.
// Any use, modification, reproduction, retransmission, or republication of all
// or part of MStar Software is expressly prohibited, unless prior written
// permission has been granted by MStar.
//
// By accessing, browsing and/or using MStar Software, you acknowledge that you
// have read, understood, and agree, to be bound by below terms ("Terms") and to
// comply with all applicable laws and regulations:
//
// 1. MStar shall retain any and all right, ownership and interest to MStar
//    Software and any modification/derivatives thereof.
//    No right, ownership, or interest to MStar Software and any
//    modification/derivatives thereof is transferred to you under Terms.
//
// 2. You understand that MStar Software might include, incorporate or be
//    supplied together with third party`s software and the use of MStar
//    Software may require additional licenses from third parties.
//    Therefore, you hereby agree it is your sole responsibility to separately
//    obtain any and all third party right and license necessary for your use of
//    such third party`s software.
//
// 3. MStar Software and any modification/derivatives thereof shall be deemed as
//    MStar`s confidential information and you agree to keep MStar`s
//    confidential information in strictest confidence and not disclose to any
//    third party.
//
// 4. MStar Software is provided on an "AS IS" basis without warranties of any
//    kind. Any warranties are hereby expressly disclaimed by MStar, including
//    without limitation, any warranties of merchantability, non-infringement of
//    intellectual property rights, fitness for a particular purpose, error free
//    and in conformity with any international standard.  You agree to waive any
//    claim against MStar for any loss, damage, cost or expense that you may
//    incur related to your use of MStar Software.
//    In no event shall MStar be liable for any direct, indirect, incidental or
//    consequential damages, including without limitation, lost of profit or
//    revenues, lost or damage of data, and unauthorized system use.
//    You agree that this Section 4 shall still apply without being affected
//    even if MStar Software has been modified by MStar in accordance with your
//    request or instruction for your use, except otherwise agreed by both
//    parties in writing.
//
// 5. If requested, MStar may from time to time provide technical supports or
//    services in relation with MStar Software to you for your use of
//    MStar Software in conjunction with your or your customer`s product
//    ("Services").
//    You understand and agree that, except otherwise agreed by both parties in
//    writing, Services are provided on an "AS IS" basis and the warranty
//    disclaimer set forth in Section 4 above shall apply.
//
// 6. Nothing contained herein shall be construed as by implication, estoppels
//    or otherwise:
//    (a) conferring any license or right to use MStar name, trademark, service
//        mark, symbol or any other identification;
//    (b) obligating MStar or any of its affiliates to furnish any person,
//        including without limitation, you and your customers, any assistance
//        of any kind whatsoever, or any information; or
//    (c) conferring any license or right under any intellectual property right.
//
// 7. These terms shall be governed by and construed in accordance with the laws
//    of Taiwan, R.O.C., excluding its conflict of law rules.
//    Any and all dispute arising out hereof or related hereto shall be finally
//    settled by arbitration referred to the Chinese Arbitration Association,
//    Taipei in accordance with the ROC Arbitration Law and the Arbitration
//    Rules of the Association by three (3) arbitrators appointed in accordance
//    with the said Rules.
//    The place of arbitration shall be in Taipei, Taiwan and the language shall
//    be English.
//    The arbitration award shall be final and binding to both parties.
//
//******************************************************************************
//<MStar Software>
////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2007 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (¡§MStar Confidential Information¡¨) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    strVariable.h
/// @brief  use to apply fast boot flow
/// @author MStar Semiconductor Inc.
///
/// suspend to ram
///
/// Features:
/// - Dispatch the register setting
///
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _STRVARIABLE_H_
#define _STRVARIABLE_H_

#include "Board.h"

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
//#define _RV32_1(addr, value)   (MS_U8)(((addr) >> 16) & 0xFF), (MS_U8)(((addr) >> 8) & 0xFF), (MS_U8)(addr), (MS_U8)(value)
//#define _RV32_2(addr, value)   0x20 + _RV32_1(addr, value), (MS_U8)((value) >> 8)
#define _RV32_1(addr, value)            (addr<<1), ((value&0xFF)<<8 | 0xFF)
#define _RV32_2(addr, value)            (addr<<1),(value&0xFFFF)
#define _DELAYVALUE(value)              CHIPCFG_CMD_WAIT, value

#define CHIPCFG_CMD_BASE                0xFE00
#define CHIPCFG_CMD_BASE_8BITS          0xFE01
#define CHIPCFG_CMD_WAIT                0xFF00
#define CHIPCFG_CMD_STORE_TO_RECOVER    0xFF01 //0x1 : reg recover to original value ; 0x0 : use fixed value to write register
#define CHIPCFG_CMD_END                 0xFFF0

MS_U16 MIU_ENTER_SR[] __attribute__ ((aligned (4096))) =
{
    CHIPCFG_CMD_BASE, 0xbf20,
    _RV32_2(0x12E0,0x0000),
    _RV32_2(0x1246,0xFFFE),
    _RV32_2(0x1266,0xFFFF),
    _RV32_2(0x1286,0xFFFF),
    _RV32_2(0x12A6,0xFFFF),

    //wait 20    // delay 20ms
    _DELAYVALUE(71),

    _RV32_2(0x1218,0x0400),
    _RV32_2(0x1200,0x002F),
    _RV32_2(0x1200,0x042F),
    _RV32_2(0x1200,0x052F),
    _RV32_2(0x1200,0x002F),
    _RV32_2(0x1206,0x1538),

    //wait 20    // delay 20ms
    _DELAYVALUE(71),

    _RV32_2(0x1246,0xFFFF),
    _RV32_2(0x1200,0x202F),

    _DELAYVALUE(71),

    _RV32_2(0x0b3e,0x0001),

    CHIPCFG_CMD_END,  0xFFFF,
};


MS_U16 MIU_EXIT_SR[] __attribute__ ((aligned (4096)))=
{
    CHIPCFG_CMD_BASE, 0xbf20,

    _RV32_2(0x0b3e,0x0000),

    _DELAYVALUE(71),

    _RV32_2(0x1206,0x1538), //0x0292

    _RV32_2(0x1200,0x002F),

    _RV32_2(0x1246,0xfffe),

    _RV32_2(0x1200,0x022F),
    _RV32_2(0x1200,0x032F),

    _RV32_2(0x1200,0x002F),
    _RV32_2(0x1200,0x000F),

    //wait 50    // delay 50ms
    _DELAYVALUE(179),

    _RV32_2(0x1200,0x001F), //0x2428

    //wait 20    // delay 20ms
    _DELAYVALUE(71),

    _RV32_2(0x1246,0x0000),
    _RV32_2(0x1266,0x0000),
    _RV32_2(0x1286,0x0000),
    _RV32_2(0x12A6,0x0000),

    //wait 10    // delay 10ms
    _DELAYVALUE(36),

    CHIPCFG_CMD_END,  0xFFFF,
};

const MS_U16 HWIP_CLOSE[] __attribute__ ((aligned (4096))) =
{
    //DIG_audio_off_ausdm
    CHIPCFG_CMD_BASE, 0xbf22,
    _RV32_2(0x2ca4,0x59ff),

    //DIG_CA_CFB_OFF
    CHIPCFG_CMD_BASE, 0xbf20,
    _RV32_2(0x3348,0x001c),

    //DIG_CA_TSP_OFF
    _RV32_2(0x0b54,0x0001),

    //DIG_CVBS_YPbPr
    _RV32_2(0x0b24,0x0001),
    //reset TVS of ve
    _RV32_2(0x3b00,0x0010),
    CHIPCFG_CMD_BASE_8BITS, 0xbf20,
    //reset TVE of ve
    _RV32_1(0x3e07,0x01),

    //DIG_Demod_OFF
    CHIPCFG_CMD_BASE_8BITS, 0xbf20,
    _RV32_1(0x1e39,0x00),
    CHIPCFG_CMD_BASE_8BITS, 0xbf22,
    _RV32_1(0x2818,0x07),
    _RV32_1(0x286c,0x00),
    _RV32_1(0x2879,0xf0),
    CHIPCFG_CMD_BASE_8BITS, 0xbf20,
    _RV32_1(0x331e,0x01),
    _RV32_1(0x3309,0x01),
    _RV32_1(0x3314,0x01),
    CHIPCFG_CMD_BASE_8BITS, 0xbf22,
    _RV32_1(0x1f0a,0x01),
    _RV32_1(0x1f0c,0x01),
    _RV32_1(0x1f0d,0x01),
    _RV32_1(0x1f10,0x01),
    _RV32_1(0x1f11,0x01),
    _RV32_1(0x1f12,0x01),
    _RV32_1(0x1f15,0x01),
    _RV32_1(0x1f16,0x01),
    _RV32_1(0x1f17,0x01),
    _RV32_1(0x1f22,0x01),
    _RV32_1(0x1f23,0x01),
    _RV32_1(0x1f2a,0x01),

    //DIG_ephy_on2off_new
    CHIPCFG_CMD_BASE_8BITS, 0xbf00,
    _RV32_1(0x32b7,0x17),
    _RV32_1(0x32d4,0x20),
    CHIPCFG_CMD_BASE, 0xbf00,
    _RV32_2(0x333a,0x03F3),
    CHIPCFG_CMD_BASE_8BITS, 0xbf00,
    _RV32_1(0x33a1,0x10),
    _RV32_1(0x32cd,0xd9),
    _RV32_1(0x32ea,0x60),
    _RV32_1(0x32bb,0x05),
    _RV32_1(0x32cb,0x11),
    CHIPCFG_CMD_BASE, 0xbf00,
    _RV32_2(0x32fc,0x0102),
    CHIPCFG_CMD_BASE_8BITS, 0xbf00,
    _RV32_1(0x33c5,0x40),
    _RV32_1(0x3330,0x53),

    //DIG_FCIE_OFF
    CHIPCFG_CMD_BASE_8BITS, 0xbf20,
    _RV32_1(0x0bc8,0x01),

    //DIG_GE_OFF
    _RV32_1(0x0b90,0x01),

    //DIG_gmac_off
    _RV32_1(0x3364,0x01),
    CHIPCFG_CMD_BASE, 0xbf24,
    _RV32_2(0x1f22,0x1101),
    _RV32_2(0x1f24,0x0911),

    //DIG_GPD_OFF
    CHIPCFG_CMD_BASE_8BITS, 0xbf20,
    _RV32_1(0x0b7c,0x01),

    //DIG_HDMI_OFF
    CHIPCFG_CMD_BASE_8BITS, 0xbf22,
    _RV32_1(0x303c,0x00),
    _RV32_1(0x303d,0x00),

    //DIG_hvd_evd_core_off
    CHIPCFG_CMD_BASE_8BITS, 0xbf20,
    _RV32_1(0x0b68,0x05),
    _RV32_1(0x0b69,0x00),
    _RV32_1(0x1b02,0x01),
    _RV32_1(0x1b03,0x00),
    _RV32_1(0x1c02,0x21),
    _RV32_1(0x1c03,0x00),

    //DIG_JPD_OFF
    CHIPCFG_CMD_BASE_8BITS, 0xbf20,
    _RV32_1(0x0b6a,0x01),

    //DIG_MVD_OFF
    _RV32_1(0x0b72,0x01),
    _RV32_1(0x0b73,0x01),
    _RV32_1(0x0b04,0x01),
    _RV32_1(0x0b75,0x01),
    _RV32_1(0x0b76,0x01),
    _RV32_1(0x0b77,0x01),
    _RV32_1(0x0b78,0x01),
    _RV32_1(0x0b79,0x01),
    _RV32_1(0x1100,0x01),

    //DIG_SC_OFF
    _RV32_1(0x0ba3,0x05),
    _RV32_1(0x0ba5,0x0d),
    _RV32_1(0x0baa,0x0d),

    //DIG_SEC_51_OFF
    CHIPCFG_CMD_BASE_8BITS, 0xbf2c,
    _RV32_1(0x0f84,0xff),

    //DIG_SEC_R2_OFF
    CHIPCFG_CMD_BASE_8BITS, 0xbf20,
    _RV32_1(0x0b62,0x01),
    _RV32_1(0x3580,0x00),

    //DIG_smc_DIG_off
    _RV32_1(0x2927,0x10),
    _RV32_1(0x3314,0x01),

    //DIG_TSO_OFF
    CHIPCFG_CMD_BASE, 0xbf20,
    _RV32_2(0x0b5e,0x0101),
    _RV32_2(0x0b4e,0x0101),

    //DIG_USB_P0_alive_P1_off
    CHIPCFG_CMD_BASE_8BITS, 0xbf20,
    _RV32_1(0x3a08,0x83),
    _RV32_1(0x3a02,0x80),
    _RV32_1(0x3a03,0x90),
    _RV32_1(0x3a00,0xc7),
    _RV32_1(0x3a01,0xef),
    _RV32_1(0x0780,0x00),
    _RV32_1(0x0782,0x00),

    //DIG_VD_R2_160
    _RV32_1(0x0b60,0x01),

    //ANA_Audio_OFF
    CHIPCFG_CMD_BASE_8BITS, 0xbf22,
    _RV32_1(0x2c02,0x00),
    _RV32_1(0x2c48,0x00),
    CHIPCFG_CMD_BASE, 0xbf22,
    _RV32_2(0x2c5e,0x0000),
    _RV32_2(0x2c60,0x0000),
    _RV32_2(0x2c62,0x0000),
    _RV32_2(0x2c64,0x0000),
    CHIPCFG_CMD_BASE_8BITS, 0xbf22,
    _RV32_1(0x2c67,0x00),
    _RV32_1(0x2c68,0x00),
    _RV32_1(0x2c69,0x00),
    _RV32_1(0x2c6a,0x00),
    _RV32_1(0x2c6b,0x00),
    _RV32_1(0x2c6c,0x00),
    _RV32_1(0x2c6e,0x00),
    _RV32_1(0x2c8a,0x00),
    _RV32_1(0x2c8b,0x00),
    _RV32_1(0x2c8c,0x00),
    _RV32_1(0x2c8d,0x00),
    CHIPCFG_CMD_BASE, 0xbf22,
    _RV32_2(0x2ca4,0x0000),
    CHIPCFG_CMD_BASE_8BITS, 0xbf22,
    _RV32_1(0x2cb2,0x00),
    _RV32_1(0x2cb3,0x00),
    _RV32_1(0x2fa8,0x00),

    //ANA_Demod_OFF
    CHIPCFG_CMD_BASE_8BITS, 0xbf22,
    _RV32_1(0x2818,0x07),
    _RV32_1(0x286C,0x00),
    _RV32_1(0x2879,0xf0),

    //ANA_HDMI_OFF
    CHIPCFG_CMD_BASE_8BITS, 0xbf20,
    _RV32_1(0x1e38,0x00),
    _RV32_1(0x1e39,0x00),
    CHIPCFG_CMD_BASE_8BITS, 0xbf22,
    _RV32_1(0x2003,0x00),
    _RV32_1(0x286a,0x04),
    _RV32_1(0x286b,0x1e),
    _RV32_1(0x286a,0x04),
    _RV32_1(0x286b,0x06),
    _RV32_1(0x2866,0x00),
    _RV32_1(0x2867,0x24),
    _RV32_1(0x2860,0x01),
    _RV32_1(0x2861,0x1c),
    _RV32_1(0x2840,0x00),
    _RV32_1(0x2841,0x00),
    _RV32_1(0x2879,0x00),
    _RV32_1(0x0b02,0x01),
    _RV32_1(0x0b03,0x00),
    _RV32_1(0x3066,0xb0),
    _RV32_1(0x3067,0x19),
    _RV32_1(0x3054,0x3f),
    _RV32_1(0x3055,0x00),
    _RV32_1(0x3056,0x00),
    _RV32_1(0x3057,0x00),

    //ANA_LAN_OFF
    CHIPCFG_CMD_BASE_8BITS, 0xbf00,
    _RV32_1(0x32b7,0x17),
    _RV32_1(0x32d4,0x20),
    CHIPCFG_CMD_BASE, 0xbf00,
    _RV32_2(0x333a,0x03f3),
    CHIPCFG_CMD_BASE_8BITS, 0xbf00,
    _RV32_1(0x33a1,0x10),
    _RV32_1(0x32cd,0xd9),
    _RV32_1(0x32ea,0x60),
    _RV32_1(0x32bb,0x05),
    _RV32_1(0x32cb,0x11),
    CHIPCFG_CMD_BASE, 0xbf00,
    _RV32_2(0x32fc,0x0102),
    CHIPCFG_CMD_BASE_8BITS, 0xbf00,
    _RV32_1(0x33c5,0x40),
    _RV32_1(0x3330,0x53),

    //ANA_RF_OFF
    CHIPCFG_CMD_BASE_8BITS, 0xbf22,
    _RV32_1(0x28a2,0x00),
    _RV32_1(0x28a3,0x00),
    CHIPCFG_CMD_BASE_8BITS, 0xbf2c,
    _RV32_1(0x0be0,0x00),
    _RV32_1(0x0be1,0x00),
    CHIPCFG_CMD_BASE_8BITS, 0xbf20,
    _RV32_1(0x0b6c,0x01),
    _RV32_1(0x0b49,0x01),

    //ANA_RGMII_OFF
    CHIPCFG_CMD_BASE, 0xbf22,
    _RV32_2(0x0cc6,0xffff),
    CHIPCFG_CMD_BASE_8BITS, 0xbf20,
    _RV32_1(0x0bc6,0x01),

    //ANA_SAR_OFF
    CHIPCFG_CMD_BASE_8BITS, 0xbf00,
    _RV32_1(0x1401,0x01),

    //ANA_SMC_OFF
    CHIPCFG_CMD_BASE_8BITS, 0xbf20,
    _RV32_1(0x294e,0x14),
    _RV32_1(0x294f,0x00),
    _RV32_1(0x2958,0x40),
    _RV32_1(0x2959,0x00),
    _RV32_1(0x0b28,0x01),
    _RV32_1(0x0b29,0x00),

    //ANA_USB_0_OFF
    _RV32_1(0x3a88,0x83),
    _RV32_1(0x3a82,0x80),
    _RV32_1(0x3a83,0x90),
    _RV32_1(0x3a80,0xc7),
    _RV32_1(0x3a81,0xef),
    _RV32_1(0x0700,0x00),
    _RV32_1(0x0702,0x00),

    //ANA_USB_1_OFF
    _RV32_1(0x3a08,0x83),
    _RV32_1(0x3a02,0x80),
    _RV32_1(0x3a03,0x90),
    _RV32_1(0x3a00,0xc7),
    _RV32_1(0x3a01,0xef),
    _RV32_1(0x0780,0x00),
    _RV32_1(0x0782,0x00),

    //ANA_Video_OFF
    _RV32_1(0x1a00,0x00),
    _RV32_1(0x1a06,0x00),
    _RV32_1(0x1a22,0x00),
    _RV32_1(0x1a14,0x00),
    _RV32_1(0x1a16,0x00),

    CHIPCFG_CMD_END,  0xFFFF,
};

const MS_U16 HWIP_OPEN[] __attribute__ ((aligned (4096))) =
{
    //DIG_Demod_ON
    CHIPCFG_CMD_STORE_TO_RECOVER, 0x0,
    CHIPCFG_CMD_BASE_8BITS, 0xbf20,
    _RV32_1(0x1e39,0x00),
    CHIPCFG_CMD_BASE_8BITS, 0xbf22,
    _RV32_1(0x2818,0x00),
    _RV32_1(0x286c,0x11),
    _RV32_1(0x2879,0x00),
    CHIPCFG_CMD_BASE_8BITS, 0xbf20,
    _RV32_1(0x331e,0x00),
    _RV32_1(0x3309,0x00),
    _RV32_1(0x3314,0x00),
    CHIPCFG_CMD_BASE_8BITS, 0xbf22,
    _RV32_1(0x1f0a,0x00),
    _RV32_1(0x1f0c,0x00),
    _RV32_1(0x1f0d,0x00),
    _RV32_1(0x1f10,0x00),
    _RV32_1(0x1f11,0x00),
    _RV32_1(0x1f12,0x00),
    _RV32_1(0x1f15,0x00),
    _RV32_1(0x1f16,0x00),
    _RV32_1(0x1f17,0x00),
    _RV32_1(0x1f22,0x00),
    _RV32_1(0x1f23,0x00),
    _RV32_1(0x1f2a,0x00),
    CHIPCFG_CMD_STORE_TO_RECOVER, 0x1,

    //ANA_Audio_ON [ORG]
    CHIPCFG_CMD_BASE_8BITS, 0xbf22,
    _RV32_1(0x2c02,0x1f),
    _RV32_1(0x2c48,0x88),
    CHIPCFG_CMD_BASE, 0xbf22,
    _RV32_2(0x2c5e,0x4080),
    _RV32_2(0x2c60,0x8080),
    _RV32_2(0x2c62,0x8780),
    _RV32_2(0x2c64,0x8080),
    CHIPCFG_CMD_BASE_8BITS, 0xbf22,
    _RV32_1(0x2c67,0x81),
    _RV32_1(0x2c68,0x80),
    _RV32_1(0x2c69,0x83),
    _RV32_1(0x2c6a,0x80),
    _RV32_1(0x2c6b,0x87),
    _RV32_1(0x2c6c,0x00),
    _RV32_1(0x2c6e,0x89),
    _RV32_1(0x2c8a,0x90),
    _RV32_1(0x2c8b,0x40),
    _RV32_1(0x2c8c,0x66),
    _RV32_1(0x2c8d,0x80),
    CHIPCFG_CMD_BASE, 0xbf22,
    _RV32_2(0x2ca4,0x5fff),
    CHIPCFG_CMD_BASE_8BITS, 0xbf22,
    _RV32_1(0x2cb2,0x92),
    _RV32_1(0x2cb3,0x62),
    _RV32_1(0x2fa8,0x50),

    //ANA_Demod_ON
    CHIPCFG_CMD_STORE_TO_RECOVER, 0x0,
    CHIPCFG_CMD_BASE_8BITS, 0xbf22,
    _RV32_1(0x2818,0x00),
    _RV32_1(0x286C,0x11),
    _RV32_1(0x2879,0x00),
    CHIPCFG_CMD_STORE_TO_RECOVER, 0x1,

    //ANA_HDMI_ON
    CHIPCFG_CMD_BASE_8BITS, 0xbf20,
    _RV32_1(0x1e38,0xff),
    _RV32_1(0x1e39,0xff),
    CHIPCFG_CMD_BASE_8BITS, 0xbf22,
    _RV32_1(0x2003,0xff),
    _RV32_1(0x286a,0xff),
    _RV32_1(0x286b,0xff),
    _RV32_1(0x286a,0xff),
    _RV32_1(0x286b,0xff),
    _RV32_1(0x2866,0xff),
    _RV32_1(0x2867,0xff),
    _RV32_1(0x2860,0xff),
    _RV32_1(0x2861,0xff),
    _RV32_1(0x2840,0xff),
    _RV32_1(0x2841,0xff),
    _RV32_1(0x2879,0xff),
    _RV32_1(0x0b02,0xff),
    _RV32_1(0x0b03,0xff),
    _RV32_1(0x3066,0xff),
    _RV32_1(0x3067,0xff),
    _RV32_1(0x3054,0xff),
    _RV32_1(0x3055,0xff),
    _RV32_1(0x3056,0xff),
    _RV32_1(0x3057,0xff),

    //ANA_LAN_ON
    CHIPCFG_CMD_BASE_8BITS, 0xbf00,
    _RV32_1(0x32b7,0xff),
    _RV32_1(0x32d4,0xff),
    CHIPCFG_CMD_BASE, 0xbf00,
    _RV32_2(0x333a,0xffff),
    CHIPCFG_CMD_BASE_8BITS, 0xbf00,
    _RV32_1(0x33a1,0xff),
    _RV32_1(0x32cd,0xff),
    _RV32_1(0x32ea,0xff),
    _RV32_1(0x32bb,0xff),
    _RV32_1(0x32cb,0xff),
    CHIPCFG_CMD_BASE, 0xbf00,
    _RV32_2(0x32fc,0xffff),
    CHIPCFG_CMD_BASE_8BITS, 0xbf00,
    _RV32_1(0x33c5,0xff),
    _RV32_1(0x3330,0xff),

    //ANA_RF_ON
    CHIPCFG_CMD_BASE_8BITS, 0xbf22,
    _RV32_1(0x28a2,0xff),
    _RV32_1(0x28a3,0xff),
    CHIPCFG_CMD_BASE_8BITS, 0xbf2c,
    _RV32_1(0x0be0,0xff),
    _RV32_1(0x0be1,0xff),
    CHIPCFG_CMD_BASE_8BITS, 0xbf20,
    _RV32_1(0x0b6c,0xff),
    _RV32_1(0x0b49,0xff),

    //ANA_RGMII_ON
    CHIPCFG_CMD_BASE, 0xbf22,
    _RV32_2(0x0cc6,0xffff),
    CHIPCFG_CMD_BASE_8BITS, 0xbf20,
    _RV32_1(0x0bc6,0xff),

    //ANA_SAR_ON
    CHIPCFG_CMD_BASE_8BITS, 0xbf00,
    _RV32_1(0x1401,0xff),

    //ANA_SMC_ON
    CHIPCFG_CMD_BASE_8BITS, 0xbf20,
    _RV32_1(0x294e,0xff),
    _RV32_1(0x294f,0xff),
    _RV32_1(0x2958,0xff),
    _RV32_1(0x2959,0xff),
    _RV32_1(0x0b28,0xff),
    _RV32_1(0x0b29,0xff),

    //ANA_USB_0_ON
    _RV32_1(0x3a88,0x83),
    _RV32_1(0x3a82,0x80),
    _RV32_1(0x3a83,0x90),
    _RV32_1(0x3a80,0xc7),
    _RV32_1(0x3a81,0xef),
    _RV32_1(0x0700,0x00),
    _RV32_1(0x0702,0x00),

    //ANA_USB_1_ON
    _RV32_1(0x3a08,0x83),
    _RV32_1(0x3a02,0x80),
    _RV32_1(0x3a03,0x90),
    _RV32_1(0x3a00,0xc7),
    _RV32_1(0x3a01,0xef),
    _RV32_1(0x0780,0x00),
    _RV32_1(0x0782,0x00),

    //ANA_Video_ON
    _RV32_1(0x1a00,0xff),
    _RV32_1(0x1a06,0xff),
    _RV32_1(0x1a22,0xff),
    _RV32_1(0x1a14,0xff),
    _RV32_1(0x1a16,0xff),

    //DIG_audio_off_ausdm
    CHIPCFG_CMD_BASE, 0xbf22,
    _RV32_2(0x2ca4,0x1802),

    //DIG_CA_CFB_ON
    CHIPCFG_CMD_BASE, 0xbf20,
    _RV32_2(0x3348,0x001c),

    //DIG_CA_TSP_ON
    _RV32_2(0x0b54,0x0004),

    //DIG_CVBS_YPbPr
    _RV32_2(0x0b24,0x0000),
    //reset TVS of ve
    _RV32_2(0x3b00,0x0000),
    CHIPCFG_CMD_BASE_8BITS, 0xbf20,
    //reset TVE of ve
    _RV32_1(0x3e07,0x00),

    //DIG_ephy_on2off_new
    CHIPCFG_CMD_BASE_8BITS, 0xbf00,
    _RV32_1(0x32b7,0xff),
    _RV32_1(0x32d4,0xff),
    CHIPCFG_CMD_BASE, 0xbf00,
    _RV32_2(0x333a,0xffff),
    CHIPCFG_CMD_BASE_8BITS, 0xbf00,
    _RV32_1(0x33a1,0xff),
    _RV32_1(0x32cd,0xff),
    _RV32_1(0x32ea,0xff),
    _RV32_1(0x32bb,0xff),
    _RV32_1(0x32cb,0xff),
    CHIPCFG_CMD_BASE, 0xbf00,
    _RV32_2(0x32fc,0xffff),
    CHIPCFG_CMD_BASE_8BITS, 0xbf00,
    _RV32_1(0x33c5,0xff),
    _RV32_1(0x3330,0xff),

    //DIG_FCIE_ON
    CHIPCFG_CMD_BASE_8BITS, 0xbf20,
    _RV32_1(0x0bc8,0xff),

    //DIG_GE_ON
    _RV32_1(0x0b90,0xff),

    //DIG_gmac_on
    _RV32_1(0x3364,0xff),
    CHIPCFG_CMD_BASE, 0xbf24,
    _RV32_2(0x1f22,0xffff),
    _RV32_2(0x1f24,0xffff),

    //DIG_GPD_ON
    CHIPCFG_CMD_BASE_8BITS, 0xbf20,
    _RV32_1(0x0b7c,0xff),

    //DIG_HDMI_ON
    CHIPCFG_CMD_BASE_8BITS, 0xbf22,
    _RV32_1(0x303c,0xff),
    _RV32_1(0x303d,0xff),

    //DIG_hvd_evd_core_on
    CHIPCFG_CMD_BASE_8BITS, 0xbf20,
    _RV32_1(0x0b68,0xff),
    _RV32_1(0x0b69,0xff),
    _RV32_1(0x1b02,0xff),
    _RV32_1(0x1b03,0xff),
    _RV32_1(0x1c02,0xff),
    _RV32_1(0x1c03,0xff),

    //DIG_JPD_ON
    CHIPCFG_CMD_BASE_8BITS, 0xbf20,
    _RV32_1(0x0b6a,0xff),

    //DIG_MVD_ON
    _RV32_1(0x0b72,0xff),
    _RV32_1(0x0b73,0xff),
    _RV32_1(0x0b04,0xff),
    _RV32_1(0x0b75,0xff),
    _RV32_1(0x0b76,0xff),
    _RV32_1(0x0b77,0xff),
    _RV32_1(0x0b78,0xff),
    _RV32_1(0x0b79,0xff),
    _RV32_1(0x1100,0xff),

    //DIG_SC_ON
    _RV32_1(0x0ba3,0xff),
    _RV32_1(0x0ba5,0xff),
    _RV32_1(0x0baa,0xff),

    //DIG_SEC_51_ON
    CHIPCFG_CMD_BASE_8BITS, 0xbf2c,
    _RV32_1(0x0f84,0xff),

    //DIG_SEC_R2_ON
    CHIPCFG_CMD_BASE_8BITS, 0xbf20,
    _RV32_1(0x0b62,0xff),
    _RV32_1(0x3580,0xff),

    //DIG_smc_DIG_on
    _RV32_1(0x2927,0xff),
    _RV32_1(0x3314,0xff),

    //DIG_TSO_ON
    CHIPCFG_CMD_BASE, 0xbf20,
    _RV32_2(0x0b5e,0xffff),
    _RV32_2(0x0b4e,0xffff),

    //DIG_USB_P0_alive_P1_ON
    CHIPCFG_CMD_BASE_8BITS, 0xbf20,
    _RV32_1(0x3a08,0xff),
    _RV32_1(0x3a02,0xff),
    _RV32_1(0x3a03,0xff),
    _RV32_1(0x3a00,0xff),
    _RV32_1(0x3a01,0xff),
    _RV32_1(0x0780,0xff),
    _RV32_1(0x0782,0xff),

    //DIG_VD_R2_160
    _RV32_1(0x0b60,0xff),

    CHIPCFG_CMD_END,  0xFFFF,
};

#endif // _APP_MAIN_H_

