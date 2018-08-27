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
/**********************************************************************
 Copyright (c) 2006-2009 MStar Semiconductor, Inc.
 All rights reserved.

 Unless otherwise stipulated in writing, any and all information contained
 herein regardless in any format shall remain the sole proprietary of
 MStar Semiconductor Inc. and be kept in strict confidence
 (MStar Confidential Information) by the recipient.
 Any unauthorized act including without limitation unauthorized disclosure,
 copying, use, reproduction, sale, distribution, modification, disassembling,
 reverse engineering and compiling of the contents of MStar Confidential
 Information is unlawful and strictly prohibited. MStar hereby reserves the
 rights to any and all damages, losses, costs and expenses resulting therefrom.

**********************************************************************/



#ifndef _MSB201X_H
#define _MSB201X_H

#define MSB201X_DEMOD_IIC 1
#define MSB201X_SPI_IIC 2
#define MSB201X_SLAVE_ID 0xD2
#define MSB201X_CHANNEL_COUNT 2

#define MSB2010_BONDING 0x38
#define MSB2011_BONDING 0x18
#define MSB1011_BONDING 0x08
#define MSB201X_BONDING_MASK 0x38

#define MSB201X_RST_PIN_NOT_SET 9999

typedef struct
{
    MS_BOOL   bInited[MSB201X_CHANNEL_COUNT];
    MS_S32    s32_MSB201x_Mutex;
    DEMOD_MS_INIT_PARAM  MSB201x_InitParam[MSB201X_CHANNEL_COUNT];
    MS_U8     u8SlaveID;
    MS_U8     u32DmxInputPath;
    MS_BOOL   bRest;
    int       RstPin;    
} MSB201X_CONFIG;

static sDMD_MSB201X_IFAGC_ERR TUNER_IfagcErr_LoRef[] =
{
    {0.00,        0x0A},
    {0.00,        0xFF},

};

static sDMD_MSB201X_IFAGC_SSI TUNER_IfagcSsi_LoRef[] =
{
    { -50.00,    0x00},
    { -51.00,    0x00},
    { -52.00,    0x00},
    { -53.00,    0x3F},
    { -53.50,    0x48},
    { -54.40,    0x4E},
    { -55.00,    0x51},
    { -56.00,    0x57},
    { -57.50,    0x5E},
    { -58.00,    0x60},
    { -58.50,    0x64},
    { -59.00,    0x6A},
    { -59.50,    0x6F},
    { -60.00,    0x73},
    { -61.00,    0x78},
    { -62.00,    0x7D},
    { -63.00,    0x80},
    { -64.00,    0x82},
    { -65.00,    0x85},
    { -66.00,    0x88},
    { -67.00,    0x89},
    { -68.00,    0x8b},
    { -69.00,    0x8C},
    { -70.00,    0x8E},
    { -71.00,    0x91},
    { -72.00,    0x94},
    { -73.00,    0x96},
    { -74.00,    0x97},
    { -75.00,    0x99},
    { -76.00,    0x9b},
    { -77.00,    0x9D},
    { -78.00,    0xA0},
    { -79.00,    0xa2},
    { -80.00,    0xa4},
    { -81.00,    0xa6},
    { -82.00,    0xa7},
    { -82.30,    0xa8},
    { -82.50,    0xA9},
    { -82.60,    0xAa},
    { -82.70,    0xAc},
    { -83.00,    0xFF},
};

#endif
