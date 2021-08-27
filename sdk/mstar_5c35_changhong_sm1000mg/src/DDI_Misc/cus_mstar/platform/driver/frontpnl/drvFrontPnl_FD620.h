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

#ifndef FP_FD620_IO_H
#define FP_FD620_IO_H

#define COLON_ENABLE      1
#define FP_LED_MODE     0x02
// -------------- DIGITAL LED MACRO -----------------
#define LED_NUM           4
#define FP_DOT_ENABLE     0
#define FP_DOT_SEG_NUM    11
#define LED_WORD1_ADDR    2
#define LED_WORD2_ADDR    0
#define LED_WORD3_ADDR    4
#define LED_WORD4_ADDR    6
#define LED_COLON_ADDR     8
#define LED_COLON_ENABLE_LOW    0xFF
#define LED_COLON_ENABLE_HIGH   0xFF
#define LED_COLON_DISABLE_LOW   0
#define LED_COLON_DISABLE_HIGH    0

#define FRONTPNL_START_TIME_MS      3//((1000 / 50) / LED_NUM)
#define FRONTPNL_PERIOD_TIME_MS     150


typedef struct
{
    MS_U8 u8Char;
    MS_U8 u8SegmentLowByte;
    MS_U8 u8SegmentHighByte;
}Char2Segment;

static const MS_U8 FP_LED_BOOT[] =
{
  0xF0,
  0x00,
  0xD0,
  0x01,
  0xD0,
  0x01,
  0xF0,
  0x01
};

static const MS_U8 FP_LED_STANDBY[]=
{
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x04
};

static const Char2Segment _char2SegmentTable[] =
{
  // char,  Byte
  {'0',0x3F,0x00},
  {'1',0x06,0x00},
  {'I',0x30,0x00},
  {'i',0x30,0x00},
  {'2',0x5B,0x00},
  {'3',0x4F,0x00},
  {'4',0x66,0x00},
  {'5',0x6D,0x00},
  {'6',0x7D,0x00},
  {'7',0x07,0x00},
  {'8',0x7F,0x00},
  {'9',0x67,0x00},
  {'A',0x77,0x00},
  {'B',0x7F,0x00},
  {'C',0x39,0x00},
  {'b',0x7C,0x00},
  {'c',0x58,0x00},
  {'D',0x3F,0x00},
  {'d',0x5E,0x00},
  {'E',0x79,0x00},
  {'F',0x71,0x00},
  {'H',0x76,0x00},
  {'h',0x74,0x00},
  {'L',0x38,0x00},
  {'n',0x54,0x00},
  {'N',0x37,0x00},
  {'O',0x3F,0x00},
  {'o',0x5C,0x00},
  {'P',0x73,0x00},
  {'R',0x72,0x00},
  {'r',0x72,0x00},
  {'T',0x78,0x00},
  {'t',0x78,0x00},
  {'U',0x3E,0x00},
  {'V',0x3E,0x00},
  {'-',0x40,0x00},
  {' ',0X00,0x00},
  {'S',0x6D,0x00},
  {'s',0x6D,0x00},
};

#define FD620_KEY_MENU     20
#define FD620_KEY_OK         25
#define FD620_KEY_UP          17
#define FD620_KEY_DOWN    12
#define FD620_KEY_LEFT       9
#define FD620_KEY_RIGHT    4

typedef struct
{
  MS_U8 keyMapData;
  MS_U8 keyMapLevel;

} MS_KEYMAP;

#endif
#endif //#ifdef DDI_MISC_INUSE

