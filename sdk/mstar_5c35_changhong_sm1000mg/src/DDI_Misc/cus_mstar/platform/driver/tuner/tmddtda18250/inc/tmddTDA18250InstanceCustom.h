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
/*
  Copyright (C) 2006-2009 NXP B.V., All Rights Reserved.
  This source code and any compilation or derivative thereof is the proprietary
  information of NXP B.V. and is confidential in nature. Under no circumstances
  is this software to be  exposed to or placed under an Open Source License of
  any type without the expressed written permission of NXP B.V.
 *
 * \file          tmddTDA18250InstanceCustom.h
 *
 *                1
 *
 * \date          %modify_time%
 *
 * \brief         Describe briefly the purpose of this file.
 *
 * REFERENCE DOCUMENTS :
 *                
 *
 * Detailed description may be added here.
 *
 * \section info Change Information
 *
*/

#ifndef _TMDD_TDA18250_INSTANCE_CUSTOM_H
#define _TMDD_TDA18250_INSTANCE_CUSTOM_H


#ifdef __cplusplus
extern "C"
{
#endif


/*============================================================================*/
/* Custom Driver Instance Parameters: (Path 0)                                */
/*============================================================================*/

#define TMDD_TDA18250_INSTANCE_CUSTOM_0                                                                     \
    tmddTDA18250_PowerStandbyWithXtalOn,     /* curPowerState */                                            \
    {                                               /* Config */                                            \
        {                                           /* MAIN_PLL_Map */                                      \
            {33000000, 0x57, 0xF0},                                                                         \
            {36000000, 0x56, 0xE0},                                                                         \
            {38000000, 0x55, 0xD0},                                                                         \
            {42000000, 0x54, 0xC0},                                                                         \
            {45000000, 0x53, 0xB0},                                                                         \
            {50000000, 0x52, 0xA0},                                                                         \
            {56000000, 0x51, 0x90},                                                                         \
            {63000000, 0x50, 0x80},                                                                         \
            {67000000, 0x47, 0x78},                                                                         \
            {72000000, 0x46, 0x70},                                                                         \
            {77000000, 0x45, 0x68},                                                                         \
            {84000000, 0x44, 0x60},                                                                         \
            {91000000, 0x43, 0x58},                                                                         \
            {100000000, 0x42, 0x50},                                                                        \
            {112000000, 0x41, 0x48},                                                                        \
            {126000000, 0x40, 0x40},                                                                        \
            {134000000, 0x37, 0x3C},                                                                        \
            {144000000, 0x36, 0x38},                                                                        \
            {155000000, 0x35, 0x34},                                                                        \
            {168000000, 0x34, 0x30},                                                                        \
            {183000000, 0x33, 0x2C},                                                                        \
            {201000000, 0x32, 0x28},                                                                        \
            {224000000, 0x31, 0x24},                                                                        \
            {252000000, 0x30, 0x20},                                                                        \
            {268000000, 0x27, 0x1E},                                                                        \
            {288000000, 0x26, 0x1C},                                                                        \
            {310000000, 0x25, 0x1A},                                                                        \
            {336000000, 0x24, 0x18},                                                                        \
            {366000000, 0x23, 0x16},                                                                        \
            {403000000, 0x22, 0x14},                                                                        \
            {448000000, 0x21, 0x12},                                                                        \
            {504000000, 0x20, 0x10},                                                                        \
            {537000000, 0x17, 0x0F},                                                                        \
            {576000000, 0x16, 0x0E},                                                                        \
            {620000000, 0x15, 0x0D},                                                                        \
            {672000000, 0x14, 0x0C},                                                                        \
            {733000000, 0x13, 0x0B},                                                                        \
            {806000000, 0x12, 0x0A},                                                                        \
            {896000000, 0x11, 0x09},                                                                        \
            {1008000000, 0x10, 0x08}                                                                       \
        }                                                                                                  \
    }                                                                                                       \

/*============================================================================*/ 
/* Custom Driver Instance Parameters: (Path 1)                                */
/*============================================================================*/

#define TMDD_TDA18250_INSTANCE_CUSTOM_1                                                                     \
    tmddTDA18250_PowerStandbyWithXtalOn,    /* curPowerState */                                             \
    {                                               /* Config */                                            \
        {                                           /* MAIN_PLL_Map */                                      \
            {33000000, 0x57, 0xF0},                                                                         \
            {36000000, 0x56, 0xE0},                                                                         \
            {38000000, 0x55, 0xD0},                                                                         \
            {42000000, 0x54, 0xC0},                                                                         \
            {45000000, 0x53, 0xB0},                                                                         \
            {50000000, 0x52, 0xA0},                                                                         \
            {56000000, 0x51, 0x90},                                                                         \
            {63000000, 0x50, 0x80},                                                                         \
            {67000000, 0x47, 0x78},                                                                         \
            {72000000, 0x46, 0x70},                                                                         \
            {77000000, 0x45, 0x68},                                                                         \
            {84000000, 0x44, 0x60},                                                                         \
            {91000000, 0x43, 0x58},                                                                         \
            {100000000, 0x42, 0x50},                                                                        \
            {112000000, 0x41, 0x48},                                                                        \
            {126000000, 0x40, 0x40},                                                                        \
            {134000000, 0x37, 0x3C},                                                                        \
            {144000000, 0x36, 0x38},                                                                        \
            {155000000, 0x35, 0x34},                                                                        \
            {168000000, 0x34, 0x30},                                                                        \
            {183000000, 0x33, 0x2C},                                                                        \
            {201000000, 0x32, 0x28},                                                                        \
            {224000000, 0x31, 0x24},                                                                        \
            {252000000, 0x30, 0x20},                                                                        \
            {268000000, 0x27, 0x1E},                                                                        \
            {288000000, 0x26, 0x1C},                                                                        \
            {310000000, 0x25, 0x1A},                                                                        \
            {336000000, 0x24, 0x18},                                                                        \
            {366000000, 0x23, 0x16},                                                                        \
            {403000000, 0x22, 0x14},                                                                        \
            {448000000, 0x21, 0x12},                                                                        \
            {504000000, 0x20, 0x10},                                                                        \
            {537000000, 0x17, 0x0F},                                                                        \
            {576000000, 0x16, 0x0E},                                                                        \
            {620000000, 0x15, 0x0D},                                                                        \
            {672000000, 0x14, 0x0C},                                                                        \
            {733000000, 0x13, 0x0B},                                                                        \
            {806000000, 0x12, 0x0A},                                                                        \
            {896000000, 0x11, 0x09},                                                                        \
            {1008000000, 0x10, 0x08}                                                                        \
        }                                                                                                   \
    }                                                                                                       \



#ifdef __cplusplus
}
#endif

#endif /* _TMDD_TDA18250_INSTANCE_CUSTOM_H */

