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
#ifdef DDI_MISC_INUSE

#ifndef FP_1628_IO_H
#define    FP_1628_IO_H
#include "Board.h"

#define FRONTPNL_START_TIME_MS      3//((1000 / 50) / LED_NUM)
#define FRONTPNL_PERIOD_TIME_MS     150

#define    COLON_ENABLE      0

#define    FP_LED_MODE          0x1
// -------------- DIGITAL LED MACRO -----------------
#define LED_NUM           4

#define LED_WORD1_ADDR    0
#define    LED_WORD2_ADDR    2
#define    LED_WORD3_ADDR    4
#define    LED_WORD4_ADDR    6
#define    LED_WORD5_ADDR    8//DVD+VCD...
#define    LED_WORD6_ADDR    10//DVD+VCD...
#define    LED_WORD7_ADDR    12//DVD+VCD...


typedef struct
{
    MS_U8 u8Char;
    MS_U16 u16SegmentByte;
}Char2Segment;


//  ---A---
//    |     |
//    F    B
//    |     |
//  ---G---
//    |     |
//    E     C
//    |     |
//  ---D---

#define DATA_NOT
#if 1//2261
#define BIT_A 0x1
#define BIT_B 0x2
#define BIT_C 0x4
#define BIT_D 0x8
#define BIT_E 0x10
#define BIT_F 0x20
#define BIT_G 0x40
#endif

//#define BIT_LED    0x20
// 定义字符编码，此处无需随方案不同而改动，只需改动上面配置的BIT_A、BIT_B等
#define DATA_0        (DATA_NOT (BIT_A | BIT_B | BIT_C | BIT_D | BIT_E | BIT_F))
#define DATA_1        (DATA_NOT (BIT_B | BIT_C))
#define DATA_2        (DATA_NOT (BIT_A | BIT_B | BIT_D | BIT_E | BIT_G))
#define DATA_3        (DATA_NOT (BIT_A | BIT_B | BIT_C | BIT_D | BIT_G))
#define DATA_4        (DATA_NOT (BIT_B | BIT_C | BIT_F | BIT_G))
#define DATA_5        (DATA_NOT (BIT_A | BIT_C | BIT_D | BIT_F | BIT_G))
#define DATA_6        (DATA_NOT (BIT_A | BIT_C | BIT_D | BIT_E | BIT_F | BIT_G))
#define DATA_7        (DATA_NOT (BIT_A | BIT_B | BIT_C))
#define DATA_8        (DATA_NOT (BIT_A | BIT_B | BIT_C | BIT_D | BIT_E | BIT_F | BIT_G))
#define DATA_9        (DATA_NOT (BIT_A | BIT_B | BIT_C | BIT_D | BIT_F | BIT_G))
#define DATA_A        (DATA_NOT (BIT_A | BIT_B | BIT_C | BIT_E | BIT_F | BIT_G))
#define DATA_b        (DATA_NOT (BIT_C | BIT_D | BIT_E | BIT_F | BIT_G))
#define DATA_C        (DATA_NOT (BIT_A | BIT_D | BIT_E | BIT_F))
#define DATA_c        (DATA_NOT (BIT_D | BIT_E | BIT_G))
#define DATA_d        (DATA_NOT (BIT_B | BIT_C | BIT_D | BIT_E | BIT_G))
#define DATA_E        (DATA_NOT (BIT_A | BIT_D | BIT_E | BIT_F | BIT_G))
#define DATA_F        (DATA_NOT (BIT_A | BIT_E | BIT_F | BIT_G))
#define DATA_H        (DATA_NOT (BIT_B | BIT_C | BIT_E | BIT_F | BIT_G))
#define DATA_h        (DATA_NOT (BIT_C | BIT_E | BIT_F | BIT_G))
#define DATA_I        (DATA_NOT (BIT_E | BIT_F))
#define DATA_i        (DATA_NOT (BIT_E))
#define DATA_L        (DATA_NOT (BIT_D | BIT_E | BIT_F))
#define DATA_N        (DATA_NOT (BIT_A | BIT_B | BIT_C | BIT_E | BIT_F))
#define DATA_n        (DATA_NOT (BIT_C | BIT_E | BIT_G))
#define DATA_O        (DATA_NOT (BIT_C | BIT_D | BIT_E | BIT_G))
#define DATA_o        (DATA_NOT (BIT_C | BIT_D | BIT_E | BIT_G))
#define DATA_P        (DATA_NOT (BIT_A | BIT_B | BIT_E | BIT_F | BIT_G))
#define DATA_R        (DATA_NOT (BIT_A | BIT_B | BIT_C | BIT_E | BIT_F | BIT_G))
#define DATA_r        (DATA_NOT (BIT_F | BIT_E | BIT_G))
#define DATA_S        (DATA_NOT (BIT_A | BIT_C | BIT_D | BIT_F | BIT_G))
#define DATA_s        (DATA_NOT (BIT_A | BIT_C | BIT_D | BIT_F | BIT_G))
#define DATA_T        (DATA_NOT (BIT_D | BIT_E | BIT_F | BIT_G))
#define DATA_t        (DATA_NOT (BIT_D | BIT_E | BIT_F | BIT_G))
#define DATA_U        (DATA_NOT (BIT_B | BIT_C | BIT_D | BIT_E | BIT_F))
#define DATA_u        (DATA_NOT (BIT_C | BIT_D | BIT_E))
//#define DATA_DP        (DATA_NOT (BIT_P))
#define DATA_HYPH    (DATA_NOT (BIT_G))
#define DATA_DARK    (DATA_NOT (0x00))
static const Char2Segment _char2SegmentTable[] =
{
    // char, low Byte, High Byte
    {'0', DATA_0},
    {'1', DATA_1},
    {'2', DATA_2},
    {'3', DATA_3},
    {'4', DATA_4},
    {'5', DATA_5},
    {'6', DATA_6},
    {'7', DATA_7},
    {'8', DATA_8},
    {'9', DATA_9},
    {'A', DATA_A},
    {'b', DATA_b},
    {'B', DATA_b},
    {'C', DATA_C},
    {'c', DATA_c},
    {'d', DATA_d},
    {'D', DATA_d},
    {'E', DATA_E},
    {'F', DATA_F},
    {'H', DATA_H},
    {'h', DATA_h},
    {'L', DATA_L},
    {'n', DATA_n},
    {'N', DATA_N},
    {'o', DATA_o},
    {'O', DATA_o},
    {'P', DATA_P},
    {'r', DATA_r},
    {'R', DATA_r},
    {'S', DATA_S},
    {'s', DATA_S},
    {'t', DATA_t},
    {'T', DATA_t},
    {'U', DATA_U},
    {'u', DATA_u},
    //{'.', DATA_DP, 0x00},
    {'-', DATA_HYPH},
    {' ', DATA_DARK},
};

#define TM1628_SELECT   10 //
#define TM1628_UP       20 //
#define TM1628_DOWN       5 //
#define TM1628_RIGHT       12 //
#define TM1628_LEFT       17 //
#define TM1628_MENU       2 //
#define TM1628_POWER       26 //
#define TM1628_EXIT        29
#define TM1628_STOP        9
#define TM1628_PLAYPAUSE    10
#define TM1628_OPENCLOSE    11


typedef struct
{
    MS_U8 keyMapData;
    MS_U8 keyMapLevel;

} MS_KEYMAP;

#endif
#endif //#ifdef DDI_MISC_INUSE
