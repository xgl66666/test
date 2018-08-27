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

#ifndef IR_FORMAT_H
#define IR_FORMAT_H

//-------------------------------------------------------------------------------------------
// Customer IR Specification parameter define (Please modify them by IR SPEC)
//-------------------------------------------------------------------------------------------
#define IR_MODE_SEL             IR_TYPE_SWDECODE_MODE
// IR Header code define
#define IR_HEADER_CODE0         0x80    // Custom 0
#define IR_HEADER_CODE1         0x7F    // Custom 1

// IR Timing define
#define IR_HEADER_CODE_TIME     9000    // us
#define IR_OFF_CODE_TIME        4500    // us
#define IR_OFF_CODE_RP_TIME     2500    // us
#define IR_LOGI_01H_TIME        560     // us
#define IR_LOGI_0_TIME          1120    // us
#define IR_LOGI_1_TIME          2240    // us
#define IR_TIMEOUT_CYC          140000  // us
#define IR_LOGI_TIME            969
#define IR_NIBBLE_TIME          137

// IR Format define
#define IRKEY_DUMY              0xFF
#define IRDA_KEY_MAPPING_POWER  IRKEY_POWER

typedef enum _IrCommandType
{
    IRKEY_POWER             = 0x0F,  // 1
    IRKEY_MUTE              = 0x0C,  // 2
    IRKEY_CHANNEL_PLUS      = 0x0D,  // 3
    IRKEY_CHANNEL_MINUS     = 0x0E,  // 4
    IRKEY_MENU              = 0x20,  // 5
    IRKEY_EPG               = 0x28,  // 6
    IRKEY_VOLUME_PLUS       = 0x0A,  // 7
    IRKEY_VOLUME_MINUS      = 0x0B,  // 8
    IRKEY_PLAY              = 0x30,  // 9
    IRKEY_BACKWARD          = 0x32,  // 10
    IRKEY_FORWARD           = 0x33,  // 11
    IRKEY_RECORD            = 0x37,  // 12
    IRKEY_PAUSE             = 0x31,  // 13
    IRKEY_STOP              = 0x36,  // 14   (key table : key code is 0x35)
    IRKEY_UP                = 0x21,  // 15
    IRKEY_DOWN              = 0x22,  // 16
    IRKEY_LEFT              = 0x23,  // 17
    IRKEY_RIGHT             = 0x24,  // 18
    IRKEY_SELECT            = 0x25,  // 19
    IRKEY_RETURN            = 0x27,  // 20  (new key)
    IRKEY_EXIT              = 0x26,  // 21
    IRKEY_NUM_1             = 0x01,  // 22
    IRKEY_NUM_2             = 0x02,  // 23
    IRKEY_NUM_3             = 0x03,  // 24
    IRKEY_NUM_4             = 0x04,  // 25
    IRKEY_NUM_5             = 0x05,  // 26
    IRKEY_NUM_6             = 0x06,  // 27
    IRKEY_NUM_7             = 0x07,  // 28
    IRKEY_NUM_8             = 0x08,  // 29
    IRKEY_NUM_9             = 0x09,  // 30
    IRKEY_PIP               = 0x55,  // 31
    IRKEY_NUM_0             = 0x00,  // 32
    IRKEY_CHANNEL_RETURN    = 0x29,  // 33
    IRKEY_GREEN             = 0x41,  // 34
    IRKEY_YELLOW            = 0x42,  // 35
    IRKEY_BLUE              = 0x43,  // 36
    IRKEY_RED               = 0x40,  // 37
    IRKEY_PVR               = 0x52,  // 38
    IRKEY_INFO              = 0x2A,  // 39
    IRKEY_LANGUAGE          = 0x50,  // 40  (new key)
    //IRKEY_HD              = 0x51,  // 40  (new key)
    IRKEY_F1                = 0x54,  // 41  (new key)
    IRKEY_FORWARD_SKIP      = 0x35,  // 42  (new key)
    IRKEY_BACKWARD_SKIP     = 0x34,  // 43  (new key)
    IRKEY_RESOLUTION        = 0x51,  // phase 2 12.13.14  44
    IRKEY_PROGDISTRIBUTE    = 0x2B,  // phase 2 12.13.14  45

}IrCommandType;
//-------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------
// IR system parameter define for H/W setting (Please don't modify them)
//-------------------------------------------------------------------------------------------
#define IR_CKDIV_NUM                        ((XTAL_CLOCK_FREQ+500000)/1000000)
#define IR_CKDIV_NUM_BOOT                   XTAL_CLOCK_FREQ
#define IR_CLK_BOOT                         (XTAL_CLOCK_FREQ/1000000)
#define IR_CLK                              IR_CLK_BOOT

#define irGetMinCnt_BOOT(time, tolerance)   ((MS_U32)(((double)time*((double)IR_CLK_BOOT)/(IR_CKDIV_NUM_BOOT+1))*((double)1-tolerance)))
#define irGetMaxCnt_BOOT(time, tolerance)   ((MS_U32)(((double)time*((double)IR_CLK_BOOT)/(IR_CKDIV_NUM_BOOT+1))*((double)1+tolerance)))
#define irGetMinCnt(time, tolerance)        ((MS_U32)(((double)time*((double)IR_CLK)/(IR_CKDIV_NUM+1))*((double)1-tolerance)))
#define irGetMaxCnt(time, tolerance)        ((MS_U32)(((double)time*((double)IR_CLK)/(IR_CKDIV_NUM+1))*((double)1+tolerance)))
#define irGetMinCnt2(time, tolerance)        ((MS_U32)((((double)time-((double)tolerance/2)))*((double)IR_CLK)/(IR_CKDIV_NUM+1)))
#define irGetMaxCnt2(time, tolerance)        ((MS_U32)((((double)time+((double)tolerance/2)))*((double)IR_CLK)/(IR_CKDIV_NUM+1)))

#define irGetCnt_BOOT(time)                 ((MS_U32)((double)time*((double)IR_CLK_BOOT)/(IR_CKDIV_NUM_BOOT+1)))
#define irGetCnt(time)                      ((MS_U32)((double)time*((double)IR_CLK)/(IR_CKDIV_NUM+1)))

// 12Mhz
#define IR_RP_TIMEOUT_BOOT      irGetCnt_BOOT(IR_TIMEOUT_CYC)
#define IR_HDC_UPB_BOOT         irGetMaxCnt_BOOT(IR_HEADER_CODE_TIME, 0.2)
#define IR_HDC_LOB_BOOT         irGetMinCnt_BOOT(IR_HEADER_CODE_TIME, 0.2)
#define IR_OFC_UPB_BOOT         irGetMaxCnt_BOOT(IR_OFF_CODE_TIME, 0.2)
#define IR_OFC_LOB_BOOT         irGetMinCnt_BOOT(IR_OFF_CODE_TIME, 0.2)
#define IR_OFC_RP_UPB_BOOT      irGetMaxCnt_BOOT(IR_OFF_CODE_RP_TIME, 0.2)
#define IR_OFC_RP_LOB_BOOT      irGetMinCnt_BOOT(IR_OFF_CODE_RP_TIME, 0.2)
#define IR_LG01H_UPB_BOOT       irGetMaxCnt_BOOT(IR_LOGI_01H_TIME, 0.35)
#define IR_LG01H_LOB_BOOT       irGetMinCnt_BOOT(IR_LOGI_01H_TIME, 0.3)
#define IR_LG0_UPB_BOOT         irGetMaxCnt_BOOT(IR_LOGI_0_TIME, 0.2)
#define IR_LG0_LOB_BOOT         irGetMinCnt_BOOT(IR_LOGI_0_TIME, 0.2)
#define IR_LG1_UPB_BOOT         irGetMaxCnt_BOOT(IR_LOGI_1_TIME, 0.2)
#define IR_LG1_LOB_BOOT         irGetMinCnt_BOOT(IR_LOGI_1_TIME, 0.2)

// 90Mhz
#define IR_RP_TIMEOUT           irGetCnt(IR_TIMEOUT_CYC)
#define IR_HDC_UPB              irGetMaxCnt(IR_HEADER_CODE_TIME, 0.2)
#define IR_HDC_LOB              irGetMinCnt(IR_HEADER_CODE_TIME, 0.2)
#define IR_OFC_UPB              irGetMaxCnt(IR_OFF_CODE_TIME, 0.2)
#define IR_OFC_LOB              irGetMinCnt(IR_OFF_CODE_TIME, 0.2)
#define IR_OFC_RP_UPB           irGetMaxCnt(IR_OFF_CODE_RP_TIME, 0.2)
#define IR_OFC_RP_LOB           irGetMinCnt(IR_OFF_CODE_RP_TIME, 0.2)
#define IR_LG01H_UPB            irGetMaxCnt(IR_LOGI_01H_TIME, 0.35)
#define IR_LG01H_LOB            irGetMinCnt(IR_LOGI_01H_TIME, 0.3)
#define IR_LG0_UPB              irGetMaxCnt(IR_LOGI_0_TIME, 0.2)
#define IR_LG0_LOB              irGetMinCnt(IR_LOGI_0_TIME, 0.2)
#define IR_LG1_UPB              irGetMaxCnt(IR_LOGI_1_TIME, 0.2)
#define IR_LG1_LOB              irGetMinCnt(IR_LOGI_1_TIME, 0.2)
//#define IR_LG_UPB(i)            irGetMaxCnt2(IR_LOGI_TIME+i*IR_NIBBLE_TIME, 0.05)
#define IR_LG_UPB(i)            irGetMaxCnt2(IR_LOGI_TIME+i*IR_NIBBLE_TIME, IR_NIBBLE_TIME)

//#define IR_LG_LOB(i)            irGetMinCnt2(IR_LOGI_TIME+i*IR_NIBBLE_TIME, 0.05)
#define IR_LG_LOB(i)            irGetMinCnt2(IR_LOGI_TIME+i*IR_NIBBLE_TIME, IR_NIBBLE_TIME)


//-------------------------------------------------------------------------------------------

#endif

