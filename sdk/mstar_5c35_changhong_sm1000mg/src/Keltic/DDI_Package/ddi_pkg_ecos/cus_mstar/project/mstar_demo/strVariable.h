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

#define CHIPCFG_CMD_BASE            0xFE00
#define CHIPCFG_CMD_BASE_8BITS      0xFE01
#define CHIPCFG_CMD_WAIT            0xFF00
#define CHIPCFG_CMD_STORE_TO_RECOVER    0xFF01 //0x1 : reg recover to original value ; 0x0 : use fixed value to write register
#define CHIPCFG_CMD_END             0xFFF0

#ifdef CFG_CHIP_MSD5XX9   /*1333 MHz*/
MS_U16 MIU_ENTER_SR[] __attribute__ ((aligned (4096))) =
{
    CHIPCFG_CMD_BASE, 0xbf20,
    _RV32_2(0x1206,0x3434),
    _RV32_2(0x1246,0xFFFE),
    _RV32_2(0x1266,0xFFFF),
    _RV32_2(0x1286,0xFFFF),
    _RV32_2(0x12A6,0xFFFF),
    _DELAYVALUE(71),
    _RV32_2(0x1218,0x0400),
    _RV32_2(0x1200,0x002F),
    _RV32_2(0x1200,0x052E),
    _RV32_2(0x1200,0x002E),
    _RV32_2(0x1200,0x032E),
    _RV32_2(0x1200,0x002E),
    _RV32_2(0x1206,0x1434),

    //wait 20    // delay 20ms
    _DELAYVALUE(71),

    _RV32_2(0x1246,0xFFFF),
    _RV32_2(0x1200,0x202E),

    _RV32_2(0x1202,0xa292),

    CHIPCFG_CMD_END,  0xFFFF,
    
};

MS_U16 MIU_EXIT_SR[] __attribute__ ((aligned (4096)))=
{
    CHIPCFG_CMD_BASE, 0xbf20,

     _RV32_2(0x1202,0x0292), //0x0292

    _RV32_2(0x1200,0x002C),

    _RV32_2(0x1246,0xfffe),

    _RV32_2(0x1200,0x002E),
    
    //wait 20    // delay 20ms
    _DELAYVALUE(71),
    
    _RV32_2(0x1200,0x032E),
    _RV32_2(0x1200,0x002E),

    _RV32_2(0x1200,0x000E),

    //wait 50    // delay 50ms
    _DELAYVALUE(179),

    _RV32_2(0x1206,0x0434), //0x2428

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

#else
MS_U16 MIU_ENTER_SR[] __attribute__ ((aligned (4096))) =
{
    CHIPCFG_CMD_BASE, 0xbf20,
    _RV32_2(0x1206,0x3434),
    _RV32_2(0x1246,0xFFFE),
    _RV32_2(0x1266,0xFFFF),
    _RV32_2(0x1286,0xFFFF),
    _RV32_2(0x12A6,0xFFFF),

    //wait 20    // delay 20ms
    _DELAYVALUE(71),

    _RV32_2(0x1218,0x0400),
    _RV32_2(0x1200,0x002F),
    _RV32_2(0x1200,0x052E),
    _RV32_2(0x1200,0x002E),
    _RV32_2(0x1200,0x032E),
    _RV32_2(0x1200,0x002E),
    _RV32_2(0x1206,0x1434),

    //wait 20    // delay 20ms
    _DELAYVALUE(71),

    _RV32_2(0x1246,0xFFFF),
    _RV32_2(0x1200,0x202E),

    _RV32_2(0x1202,0xE292),

    CHIPCFG_CMD_END,  0xFFFF,
};


MS_U16 MIU_EXIT_SR[] __attribute__ ((aligned (4096)))=
{
    CHIPCFG_CMD_BASE, 0xbf20,

     _RV32_2(0x1202,0x0292), //0x0292

    _RV32_2(0x1200,0x002C),

    _RV32_2(0x1246,0xfffe),

    _RV32_2(0x1200,0x032E),
    _RV32_2(0x1200,0x002E),

    _RV32_2(0x1200,0x000E),

    //wait 50    // delay 50ms
    _DELAYVALUE(179),

    _RV32_2(0x1206,0x0434), //0x2428

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

#endif

const MS_U16 HWIP_CLOSE[] __attribute__ ((aligned (4096))) =
{
    CHIPCFG_CMD_END,  0xFFFF,
};

const MS_U16 HWIP_OPEN[] __attribute__ ((aligned (4096))) =
{
    CHIPCFG_CMD_END,  0xFFFF,
};

#endif // _APP_MAIN_H_

