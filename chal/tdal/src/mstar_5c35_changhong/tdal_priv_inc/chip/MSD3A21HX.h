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
// Copyright (c) 2006-2009 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// ("MStar Confidential Information") by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

// This file is generated by script, please do not edit it directly
#ifndef _MSD3A21HX_H_
#define _MSD3A21HX_H_

#define PAD_NC 9999
#define PAD_GND 0

#define BALL_K4 PAD_GPIO_PM9
#define PAD_GPIO_PM9 168
#define GPIO_PAD_168 GPIO167
#ifndef BALL_K4_IS_GPIO
#define BALL_K4_IS_GPIO 0
#endif
#ifndef PAD_GPIO_PM9_IS_GPIO
#define PAD_GPIO_PM9_IS_GPIO BALL_K4_IS_GPIO
#endif

#define BALL_J4 PAD_GPIO_PM10
#define PAD_GPIO_PM10 169
#define GPIO_PAD_169 GPIO168
#ifndef BALL_J4_IS_GPIO
#define BALL_J4_IS_GPIO 0
#endif
#ifndef PAD_GPIO_PM10_IS_GPIO
#define PAD_GPIO_PM10_IS_GPIO BALL_J4_IS_GPIO
#endif

#define BALL_E4 PAD_GPIO_PM11
#define PAD_GPIO_PM11 170
#define GPIO_PAD_170 GPIO169
#ifndef BALL_E4_IS_GPIO
#define BALL_E4_IS_GPIO 0
#endif
#ifndef PAD_GPIO_PM11_IS_GPIO
#define PAD_GPIO_PM11_IS_GPIO BALL_E4_IS_GPIO
#endif

#define BALL_F4 PAD_GPIO_PM12
#define PAD_GPIO_PM12 171
#define GPIO_PAD_171 GPIO170
#ifndef BALL_F4_IS_GPIO
#define BALL_F4_IS_GPIO 0
#endif
#ifndef PAD_GPIO_PM12_IS_GPIO
#define PAD_GPIO_PM12_IS_GPIO BALL_F4_IS_GPIO
#endif

#define BALL_D5 PAD_IRIN
#define PAD_IRIN 172
#define GPIO_PAD_172 GPIO171
#ifndef BALL_D5_IS_GPIO
#define BALL_D5_IS_GPIO 0
#endif
#ifndef PAD_IRIN_IS_GPIO
#define PAD_IRIN_IS_GPIO BALL_D5_IS_GPIO
#endif

#define BALL_R3 PAD_VSYNC0
#define PAD_VSYNC0 173
#define GPIO_PAD_173 GPIO172
#ifndef BALL_R3_IS_GPIO
#define BALL_R3_IS_GPIO 0
#endif
#ifndef PAD_VSYNC0_IS_GPIO
#define PAD_VSYNC0_IS_GPIO BALL_R3_IS_GPIO
#endif

#define BALL_P4 PAD_VSYNC1
#define PAD_VSYNC1 174
#define GPIO_PAD_174 GPIO173
#ifndef BALL_P4_IS_GPIO
#define BALL_P4_IS_GPIO 0
#endif
#ifndef PAD_VSYNC1_IS_GPIO
#define PAD_VSYNC1_IS_GPIO BALL_P4_IS_GPIO
#endif

#define BALL_G4 PAD_CEC
#define PAD_CEC 175
#define GPIO_PAD_175 GPIO174
#ifndef BALL_G4_IS_GPIO
#define BALL_G4_IS_GPIO 0
#endif
#ifndef PAD_CEC_IS_GPIO
#define PAD_CEC_IS_GPIO BALL_G4_IS_GPIO
#endif

#define BALL_P5 PAD_TSO_CLK
#define PAD_TSO_CLK 11
#define GPIO_PAD_11 GPIO10
#ifndef BALL_P5_IS_GPIO
#define BALL_P5_IS_GPIO 0
#endif
#ifndef PAD_TSO_CLK_IS_GPIO
#define PAD_TSO_CLK_IS_GPIO BALL_P5_IS_GPIO
#endif

#define BALL_R5 PAD_TSO_SYNC
#define PAD_TSO_SYNC 10
#define GPIO_PAD_10 GPIO9
#ifndef BALL_R5_IS_GPIO
#define BALL_R5_IS_GPIO 0
#endif
#ifndef PAD_TSO_SYNC_IS_GPIO
#define PAD_TSO_SYNC_IS_GPIO BALL_R5_IS_GPIO
#endif

#define BALL_T5 PAD_TSO_VLD
#define PAD_TSO_VLD 9
#define GPIO_PAD_9 GPIO8
#ifndef BALL_T5_IS_GPIO
#define BALL_T5_IS_GPIO 0
#endif
#ifndef PAD_TSO_VLD_IS_GPIO
#define PAD_TSO_VLD_IS_GPIO BALL_T5_IS_GPIO
#endif

#define BALL_N5 PAD_TSO_D7
#define PAD_TSO_D7 8
#define GPIO_PAD_8 GPIO7
#ifndef BALL_N5_IS_GPIO
#define BALL_N5_IS_GPIO 0
#endif
#ifndef PAD_TSO_D7_IS_GPIO
#define PAD_TSO_D7_IS_GPIO BALL_N5_IS_GPIO
#endif

#define BALL_K6 PAD_TSO_D6
#define PAD_TSO_D6 7
#define GPIO_PAD_7 GPIO6
#ifndef BALL_K6_IS_GPIO
#define BALL_K6_IS_GPIO 0
#endif
#ifndef PAD_TSO_D6_IS_GPIO
#define PAD_TSO_D6_IS_GPIO BALL_K6_IS_GPIO
#endif

#define BALL_M6 PAD_TSO_D5
#define PAD_TSO_D5 6
#define GPIO_PAD_6 GPIO5
#ifndef BALL_M6_IS_GPIO
#define BALL_M6_IS_GPIO 0
#endif
#ifndef PAD_TSO_D5_IS_GPIO
#define PAD_TSO_D5_IS_GPIO BALL_M6_IS_GPIO
#endif

#define BALL_M5 PAD_TSO_D4
#define PAD_TSO_D4 5
#define GPIO_PAD_5 GPIO4
#ifndef BALL_M5_IS_GPIO
#define BALL_M5_IS_GPIO 0
#endif
#ifndef PAD_TSO_D4_IS_GPIO
#define PAD_TSO_D4_IS_GPIO BALL_M5_IS_GPIO
#endif

#define BALL_L5 PAD_TSO_D3
#define PAD_TSO_D3 4
#define GPIO_PAD_4 GPIO3
#ifndef BALL_L5_IS_GPIO
#define BALL_L5_IS_GPIO 0
#endif
#ifndef PAD_TSO_D3_IS_GPIO
#define PAD_TSO_D3_IS_GPIO BALL_L5_IS_GPIO
#endif

#define BALL_K5 PAD_TSO_D2
#define PAD_TSO_D2 3
#define GPIO_PAD_3 GPIO2
#ifndef BALL_K5_IS_GPIO
#define BALL_K5_IS_GPIO 0
#endif
#ifndef PAD_TSO_D2_IS_GPIO
#define PAD_TSO_D2_IS_GPIO BALL_K5_IS_GPIO
#endif

#define BALL_J5 PAD_TSO_D1
#define PAD_TSO_D1 2
#define GPIO_PAD_2 GPIO1
#ifndef BALL_J5_IS_GPIO
#define BALL_J5_IS_GPIO 0
#endif
#ifndef PAD_TSO_D1_IS_GPIO
#define PAD_TSO_D1_IS_GPIO BALL_J5_IS_GPIO
#endif

#define BALL_L6 PAD_TSO_D0
#define PAD_TSO_D0 1
#define GPIO_PAD_1 GPIO0
#ifndef BALL_L6_IS_GPIO
#define BALL_L6_IS_GPIO 0
#endif
#ifndef PAD_TSO_D0_IS_GPIO
#define PAD_TSO_D0_IS_GPIO BALL_L6_IS_GPIO
#endif

#define PAD_TSI_CLK 9999
#ifndef PAD_TSI_CLK_IS_GPIO
#define PAD_TSI_CLK_IS_GPIO 0
#endif

#define PAD_TSI_SYNC 9999
#ifndef PAD_TSI_SYNC_IS_GPIO
#define PAD_TSI_SYNC_IS_GPIO 0
#endif

#define PAD_TSI_VLD 9999
#ifndef PAD_TSI_VLD_IS_GPIO
#define PAD_TSI_VLD_IS_GPIO 0
#endif

#define PAD_TSI_D7 9999
#ifndef PAD_TSI_D7_IS_GPIO
#define PAD_TSI_D7_IS_GPIO 0
#endif

#define PAD_TSI_D6 9999
#ifndef PAD_TSI_D6_IS_GPIO
#define PAD_TSI_D6_IS_GPIO 0
#endif

#define PAD_TSI_D5 9999
#ifndef PAD_TSI_D5_IS_GPIO
#define PAD_TSI_D5_IS_GPIO 0
#endif

#define PAD_TSI_D4 9999
#ifndef PAD_TSI_D4_IS_GPIO
#define PAD_TSI_D4_IS_GPIO 0
#endif

#define PAD_TSI_D3 9999
#ifndef PAD_TSI_D3_IS_GPIO
#define PAD_TSI_D3_IS_GPIO 0
#endif

#define PAD_TSI_D2 9999
#ifndef PAD_TSI_D2_IS_GPIO
#define PAD_TSI_D2_IS_GPIO 0
#endif

#define PAD_TSI_D1 9999
#ifndef PAD_TSI_D1_IS_GPIO
#define PAD_TSI_D1_IS_GPIO 0
#endif

#define PAD_TSI_D0 9999
#ifndef PAD_TSI_D0_IS_GPIO
#define PAD_TSI_D0_IS_GPIO 0
#endif

#define BALL_AB3 PAD_I2CM_SCL1
#define PAD_I2CM_SCL1 23
#define GPIO_PAD_23 GPIO22
#ifndef BALL_AB3_IS_GPIO
#define BALL_AB3_IS_GPIO 0
#endif
#ifndef PAD_I2CM_SCL1_IS_GPIO
#define PAD_I2CM_SCL1_IS_GPIO BALL_AB3_IS_GPIO
#endif

#define BALL_AA3 PAD_I2CM_SDA1
#define PAD_I2CM_SDA1 24
#define GPIO_PAD_24 GPIO23
#ifndef BALL_AA3_IS_GPIO
#define BALL_AA3_IS_GPIO 0
#endif
#ifndef PAD_I2CM_SDA1_IS_GPIO
#define PAD_I2CM_SDA1_IS_GPIO BALL_AA3_IS_GPIO
#endif

#define BALL_W2 PAD_RF_AGC
#define PAD_RF_AGC 25
#define GPIO_PAD_25 GPIO24
#ifndef BALL_W2_IS_GPIO
#define BALL_W2_IS_GPIO 0
#endif
#ifndef PAD_RF_AGC_IS_GPIO
#define PAD_RF_AGC_IS_GPIO BALL_W2_IS_GPIO
#endif

#define BALL_Y2 PAD_IF_AGC
#define PAD_IF_AGC 26
#define GPIO_PAD_26 GPIO25
#ifndef BALL_Y2_IS_GPIO
#define BALL_Y2_IS_GPIO 0
#endif
#ifndef PAD_IF_AGC_IS_GPIO
#define PAD_IF_AGC_IS_GPIO BALL_Y2_IS_GPIO
#endif

#define BALL_V4 PAD_DDCA_CK_PM
#define PAD_DDCA_CK_PM 176
#define GPIO_PAD_176 GPIO175
#ifndef BALL_V4_IS_GPIO
#define BALL_V4_IS_GPIO 0
#endif
#ifndef PAD_DDCA_CK_PM_IS_GPIO
#define PAD_DDCA_CK_PM_IS_GPIO BALL_V4_IS_GPIO
#endif

#define BALL_W4 PAD_DDCA_DA_PM
#define PAD_DDCA_DA_PM 177
#define GPIO_PAD_177 GPIO176
#ifndef BALL_W4_IS_GPIO
#define BALL_W4_IS_GPIO 0
#endif
#ifndef PAD_DDCA_DA_PM_IS_GPIO
#define PAD_DDCA_DA_PM_IS_GPIO BALL_W4_IS_GPIO
#endif

#define BALL_F3 PAD_DDCDA_CK_PM
#define PAD_DDCDA_CK_PM 178
#define GPIO_PAD_178 GPIO177
#ifndef BALL_F3_IS_GPIO
#define BALL_F3_IS_GPIO 0
#endif
#ifndef PAD_DDCDA_CK_PM_IS_GPIO
#define PAD_DDCDA_CK_PM_IS_GPIO BALL_F3_IS_GPIO
#endif

#define BALL_G3 PAD_DDCDA_DA_PM
#define PAD_DDCDA_DA_PM 179
#define GPIO_PAD_179 GPIO178
#ifndef BALL_G3_IS_GPIO
#define BALL_G3_IS_GPIO 0
#endif
#ifndef PAD_DDCDA_DA_PM_IS_GPIO
#define PAD_DDCDA_DA_PM_IS_GPIO BALL_G3_IS_GPIO
#endif

#define BALL_D4 PAD_DDCDB_CK_PM
#define PAD_DDCDB_CK_PM 180
#define GPIO_PAD_180 GPIO179
#ifndef BALL_D4_IS_GPIO
#define BALL_D4_IS_GPIO 0
#endif
#ifndef PAD_DDCDB_CK_PM_IS_GPIO
#define PAD_DDCDB_CK_PM_IS_GPIO BALL_D4_IS_GPIO
#endif

#define BALL_C3 PAD_DDCDB_DA_PM
#define PAD_DDCDB_DA_PM 181
#define GPIO_PAD_181 GPIO180
#ifndef BALL_C3_IS_GPIO
#define BALL_C3_IS_GPIO 0
#endif
#ifndef PAD_DDCDB_DA_PM_IS_GPIO
#define PAD_DDCDB_DA_PM_IS_GPIO BALL_C3_IS_GPIO
#endif

#define BALL_AE6 PAD_TS0_D0
#define PAD_TS0_D0 27
#define GPIO_PAD_27 GPIO26
#ifndef BALL_AE6_IS_GPIO
#define BALL_AE6_IS_GPIO 0
#endif
#ifndef PAD_TS0_D0_IS_GPIO
#define PAD_TS0_D0_IS_GPIO BALL_AE6_IS_GPIO
#endif

#define BALL_Y7 PAD_TS0_D1
#define PAD_TS0_D1 28
#define GPIO_PAD_28 GPIO27
#ifndef BALL_Y7_IS_GPIO
#define BALL_Y7_IS_GPIO 0
#endif
#ifndef PAD_TS0_D1_IS_GPIO
#define PAD_TS0_D1_IS_GPIO BALL_Y7_IS_GPIO
#endif

#define BALL_AA8 PAD_TS0_D2
#define PAD_TS0_D2 29
#define GPIO_PAD_29 GPIO28
#ifndef BALL_AA8_IS_GPIO
#define BALL_AA8_IS_GPIO 0
#endif
#ifndef PAD_TS0_D2_IS_GPIO
#define PAD_TS0_D2_IS_GPIO BALL_AA8_IS_GPIO
#endif

#define BALL_AA7 PAD_TS0_D3
#define PAD_TS0_D3 30
#define GPIO_PAD_30 GPIO29
#ifndef BALL_AA7_IS_GPIO
#define BALL_AA7_IS_GPIO 0
#endif
#ifndef PAD_TS0_D3_IS_GPIO
#define PAD_TS0_D3_IS_GPIO BALL_AA7_IS_GPIO
#endif

#define BALL_AE8 PAD_TS0_D4
#define PAD_TS0_D4 31
#define GPIO_PAD_31 GPIO30
#ifndef BALL_AE8_IS_GPIO
#define BALL_AE8_IS_GPIO 0
#endif
#ifndef PAD_TS0_D4_IS_GPIO
#define PAD_TS0_D4_IS_GPIO BALL_AE8_IS_GPIO
#endif

#define BALL_AD8 PAD_TS0_D5
#define PAD_TS0_D5 32
#define GPIO_PAD_32 GPIO31
#ifndef BALL_AD8_IS_GPIO
#define BALL_AD8_IS_GPIO 0
#endif
#ifndef PAD_TS0_D5_IS_GPIO
#define PAD_TS0_D5_IS_GPIO BALL_AD8_IS_GPIO
#endif

#define BALL_AC8 PAD_TS0_D6
#define PAD_TS0_D6 33
#define GPIO_PAD_33 GPIO32
#ifndef BALL_AC8_IS_GPIO
#define BALL_AC8_IS_GPIO 0
#endif
#ifndef PAD_TS0_D6_IS_GPIO
#define PAD_TS0_D6_IS_GPIO BALL_AC8_IS_GPIO
#endif

#define BALL_AD7 PAD_TS0_D7
#define PAD_TS0_D7 34
#define GPIO_PAD_34 GPIO33
#ifndef BALL_AD7_IS_GPIO
#define BALL_AD7_IS_GPIO 0
#endif
#ifndef PAD_TS0_D7_IS_GPIO
#define PAD_TS0_D7_IS_GPIO BALL_AD7_IS_GPIO
#endif

#define BALL_AC7 PAD_TS0_VLD
#define PAD_TS0_VLD 35
#define GPIO_PAD_35 GPIO34
#ifndef BALL_AC7_IS_GPIO
#define BALL_AC7_IS_GPIO 0
#endif
#ifndef PAD_TS0_VLD_IS_GPIO
#define PAD_TS0_VLD_IS_GPIO BALL_AC7_IS_GPIO
#endif

#define BALL_AB7 PAD_TS0_SYNC
#define PAD_TS0_SYNC 36
#define GPIO_PAD_36 GPIO35
#ifndef BALL_AB7_IS_GPIO
#define BALL_AB7_IS_GPIO 0
#endif
#ifndef PAD_TS0_SYNC_IS_GPIO
#define PAD_TS0_SYNC_IS_GPIO BALL_AB7_IS_GPIO
#endif

#define BALL_AB8 PAD_TS0_CLK
#define PAD_TS0_CLK 37
#define GPIO_PAD_37 GPIO36
#ifndef BALL_AB8_IS_GPIO
#define BALL_AB8_IS_GPIO 0
#endif
#ifndef PAD_TS0_CLK_IS_GPIO
#define PAD_TS0_CLK_IS_GPIO BALL_AB8_IS_GPIO
#endif

#define BALL_AE11 PAD_PCM_D3
#define PAD_PCM_D3 64
#define GPIO_PAD_64 GPIO63
#ifndef BALL_AE11_IS_GPIO
#define BALL_AE11_IS_GPIO 0
#endif
#ifndef PAD_PCM_D3_IS_GPIO
#define PAD_PCM_D3_IS_GPIO BALL_AE11_IS_GPIO
#endif

#define BALL_AA12 PAD_PCM_D4
#define PAD_PCM_D4 63
#define GPIO_PAD_63 GPIO62
#ifndef BALL_AA12_IS_GPIO
#define BALL_AA12_IS_GPIO 0
#endif
#ifndef PAD_PCM_D4_IS_GPIO
#define PAD_PCM_D4_IS_GPIO BALL_AA12_IS_GPIO
#endif

#define BALL_AC12 PAD_PCM_D5
#define PAD_PCM_D5 62
#define GPIO_PAD_62 GPIO61
#ifndef BALL_AC12_IS_GPIO
#define BALL_AC12_IS_GPIO 0
#endif
#ifndef PAD_PCM_D5_IS_GPIO
#define PAD_PCM_D5_IS_GPIO BALL_AC12_IS_GPIO
#endif

#define BALL_Y10 PAD_PCM_D6
#define PAD_PCM_D6 61
#define GPIO_PAD_61 GPIO60
#ifndef BALL_Y10_IS_GPIO
#define BALL_Y10_IS_GPIO 0
#endif
#ifndef PAD_PCM_D6_IS_GPIO
#define PAD_PCM_D6_IS_GPIO BALL_Y10_IS_GPIO
#endif

#define BALL_Y9 PAD_PCM_D7
#define PAD_PCM_D7 60
#define GPIO_PAD_60 GPIO59
#ifndef BALL_Y9_IS_GPIO
#define BALL_Y9_IS_GPIO 0
#endif
#ifndef PAD_PCM_D7_IS_GPIO
#define PAD_PCM_D7_IS_GPIO BALL_Y9_IS_GPIO
#endif

#define BALL_AA9 PAD_PCM_CE_N
#define PAD_PCM_CE_N 59
#define GPIO_PAD_59 GPIO58
#ifndef BALL_AA9_IS_GPIO
#define BALL_AA9_IS_GPIO 0
#endif
#ifndef PAD_PCM_CE_N_IS_GPIO
#define PAD_PCM_CE_N_IS_GPIO BALL_AA9_IS_GPIO
#endif

#define BALL_AD12 PAD_PCM_A10
#define PAD_PCM_A10 58
#define GPIO_PAD_58 GPIO57
#ifndef BALL_AD12_IS_GPIO
#define BALL_AD12_IS_GPIO 0
#endif
#ifndef PAD_PCM_A10_IS_GPIO
#define PAD_PCM_A10_IS_GPIO BALL_AD12_IS_GPIO
#endif

#define BALL_Y11 PAD_PCM_OE_N
#define PAD_PCM_OE_N 57
#define GPIO_PAD_57 GPIO56
#ifndef BALL_Y11_IS_GPIO
#define BALL_Y11_IS_GPIO 0
#endif
#ifndef PAD_PCM_OE_N_IS_GPIO
#define PAD_PCM_OE_N_IS_GPIO BALL_Y11_IS_GPIO
#endif

#define BALL_AB12 PAD_PCM_A11
#define PAD_PCM_A11 56
#define GPIO_PAD_56 GPIO55
#ifndef BALL_AB12_IS_GPIO
#define BALL_AB12_IS_GPIO 0
#endif
#ifndef PAD_PCM_A11_IS_GPIO
#define PAD_PCM_A11_IS_GPIO BALL_AB12_IS_GPIO
#endif

#define BALL_AB10 PAD_PCM_IORD_N
#define PAD_PCM_IORD_N 55
#define GPIO_PAD_55 GPIO54
#ifndef BALL_AB10_IS_GPIO
#define BALL_AB10_IS_GPIO 0
#endif
#ifndef PAD_PCM_IORD_N_IS_GPIO
#define PAD_PCM_IORD_N_IS_GPIO BALL_AB10_IS_GPIO
#endif

#define BALL_AC13 PAD_PCM_A9
#define PAD_PCM_A9 54
#define GPIO_PAD_54 GPIO53
#ifndef BALL_AC13_IS_GPIO
#define BALL_AC13_IS_GPIO 0
#endif
#ifndef PAD_PCM_A9_IS_GPIO
#define PAD_PCM_A9_IS_GPIO BALL_AC13_IS_GPIO
#endif

#define BALL_AA11 PAD_PCM_IOWR_N
#define PAD_PCM_IOWR_N 53
#define GPIO_PAD_53 GPIO52
#ifndef BALL_AA11_IS_GPIO
#define BALL_AA11_IS_GPIO 0
#endif
#ifndef PAD_PCM_IOWR_N_IS_GPIO
#define PAD_PCM_IOWR_N_IS_GPIO BALL_AA11_IS_GPIO
#endif

#define BALL_AD13 PAD_PCM_A8
#define PAD_PCM_A8 52
#define GPIO_PAD_52 GPIO51
#ifndef BALL_AD13_IS_GPIO
#define BALL_AD13_IS_GPIO 0
#endif
#ifndef PAD_PCM_A8_IS_GPIO
#define PAD_PCM_A8_IS_GPIO BALL_AD13_IS_GPIO
#endif

#define BALL_AC14 PAD_PCM_A13
#define PAD_PCM_A13 51
#define GPIO_PAD_51 GPIO50
#ifndef BALL_AC14_IS_GPIO
#define BALL_AC14_IS_GPIO 0
#endif
#ifndef PAD_PCM_A13_IS_GPIO
#define PAD_PCM_A13_IS_GPIO BALL_AC14_IS_GPIO
#endif

#define BALL_AD14 PAD_PCM_A14
#define PAD_PCM_A14 50
#define GPIO_PAD_50 GPIO49
#ifndef BALL_AD14_IS_GPIO
#define BALL_AD14_IS_GPIO 0
#endif
#ifndef PAD_PCM_A14_IS_GPIO
#define PAD_PCM_A14_IS_GPIO BALL_AD14_IS_GPIO
#endif

#define BALL_Y13 PAD_PCM_WE_N
#define PAD_PCM_WE_N 49
#define GPIO_PAD_49 GPIO48
#ifndef BALL_Y13_IS_GPIO
#define BALL_Y13_IS_GPIO 0
#endif
#ifndef PAD_PCM_WE_N_IS_GPIO
#define PAD_PCM_WE_N_IS_GPIO BALL_Y13_IS_GPIO
#endif

#define BALL_Y12 PAD_PCM_IRQA_N
#define PAD_PCM_IRQA_N 48
#define GPIO_PAD_48 GPIO47
#ifndef BALL_Y12_IS_GPIO
#define BALL_Y12_IS_GPIO 0
#endif
#ifndef PAD_PCM_IRQA_N_IS_GPIO
#define PAD_PCM_IRQA_N_IS_GPIO BALL_Y12_IS_GPIO
#endif

#define BALL_AB14 PAD_PCM_A12
#define PAD_PCM_A12 47
#define GPIO_PAD_47 GPIO46
#ifndef BALL_AB14_IS_GPIO
#define BALL_AB14_IS_GPIO 0
#endif
#ifndef PAD_PCM_A12_IS_GPIO
#define PAD_PCM_A12_IS_GPIO BALL_AB14_IS_GPIO
#endif

#define BALL_AC15 PAD_PCM_A7
#define PAD_PCM_A7 46
#define GPIO_PAD_46 GPIO45
#ifndef BALL_AC15_IS_GPIO
#define BALL_AC15_IS_GPIO 0
#endif
#ifndef PAD_PCM_A7_IS_GPIO
#define PAD_PCM_A7_IS_GPIO BALL_AC15_IS_GPIO
#endif

#define BALL_AE15 PAD_PCM_A6
#define PAD_PCM_A6 45
#define GPIO_PAD_45 GPIO44
#ifndef BALL_AE15_IS_GPIO
#define BALL_AE15_IS_GPIO 0
#endif
#ifndef PAD_PCM_A6_IS_GPIO
#define PAD_PCM_A6_IS_GPIO BALL_AE15_IS_GPIO
#endif

#define BALL_AC16 PAD_PCM_A5
#define PAD_PCM_A5 44
#define GPIO_PAD_44 GPIO43
#ifndef BALL_AC16_IS_GPIO
#define BALL_AC16_IS_GPIO 0
#endif
#ifndef PAD_PCM_A5_IS_GPIO
#define PAD_PCM_A5_IS_GPIO BALL_AC16_IS_GPIO
#endif

#define BALL_AA10 PAD_PCM_WAIT_N
#define PAD_PCM_WAIT_N 43
#define GPIO_PAD_43 GPIO42
#ifndef BALL_AA10_IS_GPIO
#define BALL_AA10_IS_GPIO 0
#endif
#ifndef PAD_PCM_WAIT_N_IS_GPIO
#define PAD_PCM_WAIT_N_IS_GPIO BALL_AA10_IS_GPIO
#endif

#define BALL_AD16 PAD_PCM_A4
#define PAD_PCM_A4 42
#define GPIO_PAD_42 GPIO41
#ifndef BALL_AD16_IS_GPIO
#define BALL_AD16_IS_GPIO 0
#endif
#ifndef PAD_PCM_A4_IS_GPIO
#define PAD_PCM_A4_IS_GPIO BALL_AD16_IS_GPIO
#endif

#define BALL_AC10 PAD_PCM2_CE_N
#define PAD_PCM2_CE_N 75
#define GPIO_PAD_75 GPIO74
#ifndef BALL_AC10_IS_GPIO
#define BALL_AC10_IS_GPIO 0
#endif
#ifndef PAD_PCM2_CE_N_IS_GPIO
#define PAD_PCM2_CE_N_IS_GPIO BALL_AC10_IS_GPIO
#endif

#define BALL_AB16 PAD_PCM2_IRQA_N
#define PAD_PCM2_IRQA_N 76
#define GPIO_PAD_76 GPIO75
#ifndef BALL_AB16_IS_GPIO
#define BALL_AB16_IS_GPIO 0
#endif
#ifndef PAD_PCM2_IRQA_N_IS_GPIO
#define PAD_PCM2_IRQA_N_IS_GPIO BALL_AB16_IS_GPIO
#endif

#define BALL_AE13 PAD_PCM2_WAIT_N
#define PAD_PCM2_WAIT_N 77
#define GPIO_PAD_77 GPIO76
#ifndef BALL_AE13_IS_GPIO
#define BALL_AE13_IS_GPIO 0
#endif
#ifndef PAD_PCM2_WAIT_N_IS_GPIO
#define PAD_PCM2_WAIT_N_IS_GPIO BALL_AE13_IS_GPIO
#endif

#define BALL_Y6 PAD_UART_RX2
#define PAD_UART_RX2 135
#define GPIO_PAD_135 GPIO134
#ifndef BALL_Y6_IS_GPIO
#define BALL_Y6_IS_GPIO 0
#endif
#ifndef PAD_UART_RX2_IS_GPIO
#define PAD_UART_RX2_IS_GPIO BALL_Y6_IS_GPIO
#endif

#define BALL_AA6 PAD_UART_TX2
#define PAD_UART_TX2 136
#define GPIO_PAD_136 GPIO135
#ifndef BALL_AA6_IS_GPIO
#define BALL_AA6_IS_GPIO 0
#endif
#ifndef PAD_UART_TX2_IS_GPIO
#define PAD_UART_TX2_IS_GPIO BALL_AA6_IS_GPIO
#endif

#define BALL_AA16 PAD_PF_AD15
#define PAD_PF_AD15 80
#define GPIO_PAD_80 GPIO79
#ifndef BALL_AA16_IS_GPIO
#define BALL_AA16_IS_GPIO 0
#endif
#ifndef PAD_PF_AD15_IS_GPIO
#define PAD_PF_AD15_IS_GPIO BALL_AA16_IS_GPIO
#endif

#define BALL_AD9 PAD_PF_ALE
#define PAD_PF_ALE 85
#define GPIO_PAD_85 GPIO84
#ifndef BALL_AD9_IS_GPIO
#define BALL_AD9_IS_GPIO 0
#endif
#ifndef PAD_PF_ALE_IS_GPIO
#define PAD_PF_ALE_IS_GPIO BALL_AD9_IS_GPIO
#endif

#define BALL_Y15 PAD_PF_CE0Z
#define PAD_PF_CE0Z 81
#define GPIO_PAD_81 GPIO80
#ifndef BALL_Y15_IS_GPIO
#define BALL_Y15_IS_GPIO 0
#endif
#ifndef PAD_PF_CE0Z_IS_GPIO
#define PAD_PF_CE0Z_IS_GPIO BALL_Y15_IS_GPIO
#endif

#define BALL_AE9 PAD_PF_CE1Z
#define PAD_PF_CE1Z 82
#define GPIO_PAD_82 GPIO81
#ifndef BALL_AE9_IS_GPIO
#define BALL_AE9_IS_GPIO 0
#endif
#ifndef PAD_PF_CE1Z_IS_GPIO
#define PAD_PF_CE1Z_IS_GPIO BALL_AE9_IS_GPIO
#endif

#define BALL_AC9 PAD_PF_OEZ
#define PAD_PF_OEZ 83
#define GPIO_PAD_83 GPIO82
#ifndef BALL_AC9_IS_GPIO
#define BALL_AC9_IS_GPIO 0
#endif
#ifndef PAD_PF_OEZ_IS_GPIO
#define PAD_PF_OEZ_IS_GPIO BALL_AC9_IS_GPIO
#endif

#define BALL_Y8 PAD_PF_WEZ
#define PAD_PF_WEZ 84
#define GPIO_PAD_84 GPIO83
#ifndef BALL_Y8_IS_GPIO
#define BALL_Y8_IS_GPIO 0
#endif
#ifndef PAD_PF_WEZ_IS_GPIO
#define PAD_PF_WEZ_IS_GPIO BALL_Y8_IS_GPIO
#endif

#define BALL_Y14 PAD_F_RBZ
#define PAD_F_RBZ 86
#define GPIO_PAD_86 GPIO85
#ifndef BALL_Y14_IS_GPIO
#define BALL_Y14_IS_GPIO 0
#endif
#ifndef PAD_F_RBZ_IS_GPIO
#define PAD_F_RBZ_IS_GPIO BALL_Y14_IS_GPIO
#endif

#define BALL_Y16 PAD_SPI_CK
#define PAD_SPI_CK 87
#define GPIO_PAD_87 GPIO86
#ifndef BALL_Y16_IS_GPIO
#define BALL_Y16_IS_GPIO 0
#endif
#ifndef PAD_SPI_CK_IS_GPIO
#define PAD_SPI_CK_IS_GPIO BALL_Y16_IS_GPIO
#endif

#define BALL_Y18 PAD_SPI_DI
#define PAD_SPI_DI 88
#define GPIO_PAD_88 GPIO87
#ifndef BALL_Y18_IS_GPIO
#define BALL_Y18_IS_GPIO 0
#endif
#ifndef PAD_SPI_DI_IS_GPIO
#define PAD_SPI_DI_IS_GPIO BALL_Y18_IS_GPIO
#endif

#define BALL_Y17 PAD_SPI_DO
#define PAD_SPI_DO 89
#define GPIO_PAD_89 GPIO88
#ifndef BALL_Y17_IS_GPIO
#define BALL_Y17_IS_GPIO 0
#endif
#ifndef PAD_SPI_DO_IS_GPIO
#define PAD_SPI_DO_IS_GPIO BALL_Y17_IS_GPIO
#endif

#define BALL_AA17 PAD_SPI_CZ
#define PAD_SPI_CZ 90
#define GPIO_PAD_90 GPIO89
#ifndef BALL_AA17_IS_GPIO
#define BALL_AA17_IS_GPIO 0
#endif
#ifndef PAD_SPI_CZ_IS_GPIO
#define PAD_SPI_CZ_IS_GPIO BALL_AA17_IS_GPIO
#endif

#define BALL_Y19 PAD_PWM0
#define PAD_PWM0 137
#define GPIO_PAD_137 GPIO136
#ifndef BALL_Y19_IS_GPIO
#define BALL_Y19_IS_GPIO 0
#endif
#ifndef PAD_PWM0_IS_GPIO
#define PAD_PWM0_IS_GPIO BALL_Y19_IS_GPIO
#endif

#define BALL_AA19 PAD_PWM1
#define PAD_PWM1 138
#define GPIO_PAD_138 GPIO137
#ifndef BALL_AA19_IS_GPIO
#define BALL_AA19_IS_GPIO 0
#endif
#ifndef PAD_PWM1_IS_GPIO
#define PAD_PWM1_IS_GPIO BALL_AA19_IS_GPIO
#endif

#define BALL_AA18 PAD_PWM2
#define PAD_PWM2 139
#define GPIO_PAD_139 GPIO138
#ifndef BALL_AA18_IS_GPIO
#define BALL_AA18_IS_GPIO 0
#endif
#ifndef PAD_PWM2_IS_GPIO
#define PAD_PWM2_IS_GPIO BALL_AA18_IS_GPIO
#endif

#define BALL_AA20 PAD_PWM3
#define PAD_PWM3 140
#define GPIO_PAD_140 GPIO139
#ifndef BALL_AA20_IS_GPIO
#define BALL_AA20_IS_GPIO 0
#endif
#ifndef PAD_PWM3_IS_GPIO
#define PAD_PWM3_IS_GPIO BALL_AA20_IS_GPIO
#endif

#define BALL_Y21 PAD_DDCR_DA
#define PAD_DDCR_DA 141
#define GPIO_PAD_141 GPIO140
#ifndef BALL_Y21_IS_GPIO
#define BALL_Y21_IS_GPIO 0
#endif
#ifndef PAD_DDCR_DA_IS_GPIO
#define PAD_DDCR_DA_IS_GPIO BALL_Y21_IS_GPIO
#endif

#define BALL_Y20 PAD_DDCR_CK
#define PAD_DDCR_CK 142
#define GPIO_PAD_142 GPIO141
#ifndef BALL_Y20_IS_GPIO
#define BALL_Y20_IS_GPIO 0
#endif
#ifndef PAD_DDCR_CK_IS_GPIO
#define PAD_DDCR_CK_IS_GPIO BALL_Y20_IS_GPIO
#endif

#define BALL_N21 PAD_TCON0
#define PAD_TCON0 91
#define GPIO_PAD_91 GPIO90
#ifndef BALL_N21_IS_GPIO
#define BALL_N21_IS_GPIO 0
#endif
#ifndef PAD_TCON0_IS_GPIO
#define PAD_TCON0_IS_GPIO BALL_N21_IS_GPIO
#endif

#define PAD_TCON1 9999
#ifndef PAD_TCON1_IS_GPIO
#define PAD_TCON1_IS_GPIO 0
#endif

#define PAD_TCON2 9999
#ifndef PAD_TCON2_IS_GPIO
#define PAD_TCON2_IS_GPIO 0
#endif

#define PAD_TCON3 9999
#ifndef PAD_TCON3_IS_GPIO
#define PAD_TCON3_IS_GPIO 0
#endif

#define PAD_TCON4 9999
#ifndef PAD_TCON4_IS_GPIO
#define PAD_TCON4_IS_GPIO 0
#endif

#define BALL_AC22 PAD_TCON5
#define PAD_TCON5 96
#define GPIO_PAD_96 GPIO95
#ifndef BALL_AC22_IS_GPIO
#define BALL_AC22_IS_GPIO 0
#endif
#ifndef PAD_TCON5_IS_GPIO
#define PAD_TCON5_IS_GPIO BALL_AC22_IS_GPIO
#endif

#define BALL_AD22 PAD_TCON6
#define PAD_TCON6 97
#define GPIO_PAD_97 GPIO96
#ifndef BALL_AD22_IS_GPIO
#define BALL_AD22_IS_GPIO 0
#endif
#ifndef PAD_TCON6_IS_GPIO
#define PAD_TCON6_IS_GPIO BALL_AD22_IS_GPIO
#endif

#define BALL_AE22 PAD_TCON7
#define PAD_TCON7 98
#define GPIO_PAD_98 GPIO97
#ifndef BALL_AE22_IS_GPIO
#define BALL_AE22_IS_GPIO 0
#endif
#ifndef PAD_TCON7_IS_GPIO
#define PAD_TCON7_IS_GPIO BALL_AE22_IS_GPIO
#endif

#define BALL_W21 PAD_TCON8
#define PAD_TCON8 99
#define GPIO_PAD_99 GPIO98
#ifndef BALL_W21_IS_GPIO
#define BALL_W21_IS_GPIO 0
#endif
#ifndef PAD_TCON8_IS_GPIO
#define PAD_TCON8_IS_GPIO BALL_W21_IS_GPIO
#endif

#define BALL_W22 PAD_TCON9
#define PAD_TCON9 100
#define GPIO_PAD_100 GPIO99
#ifndef BALL_W22_IS_GPIO
#define BALL_W22_IS_GPIO 0
#endif
#ifndef PAD_TCON9_IS_GPIO
#define PAD_TCON9_IS_GPIO BALL_W22_IS_GPIO
#endif

#define BALL_AA22 PAD_TCON10
#define PAD_TCON10 101
#define GPIO_PAD_101 GPIO100
#ifndef BALL_AA22_IS_GPIO
#define BALL_AA22_IS_GPIO 0
#endif
#ifndef PAD_TCON10_IS_GPIO
#define PAD_TCON10_IS_GPIO BALL_AA22_IS_GPIO
#endif

#define BALL_Y22 PAD_TCON11
#define PAD_TCON11 102
#define GPIO_PAD_102 GPIO101
#ifndef BALL_Y22_IS_GPIO
#define BALL_Y22_IS_GPIO 0
#endif
#ifndef PAD_TCON11_IS_GPIO
#define PAD_TCON11_IS_GPIO BALL_Y22_IS_GPIO
#endif

#define PAD_TCON12 9999
#ifndef PAD_TCON12_IS_GPIO
#define PAD_TCON12_IS_GPIO 0
#endif

#define PAD_TCON13 9999
#ifndef PAD_TCON13_IS_GPIO
#define PAD_TCON13_IS_GPIO 0
#endif

#define PAD_TCON14 9999
#ifndef PAD_TCON14_IS_GPIO
#define PAD_TCON14_IS_GPIO 0
#endif

#define PAD_TCON15 9999
#ifndef PAD_TCON15_IS_GPIO
#define PAD_TCON15_IS_GPIO 0
#endif

#define PAD_TCON16 9999
#ifndef PAD_TCON16_IS_GPIO
#define PAD_TCON16_IS_GPIO 0
#endif

#define BALL_AC17 PAD_PCM_A3
#define PAD_PCM_A3 65
#define GPIO_PAD_65 GPIO64
#ifndef BALL_AC17_IS_GPIO
#define BALL_AC17_IS_GPIO 0
#endif
#ifndef PAD_PCM_A3_IS_GPIO
#define PAD_PCM_A3_IS_GPIO BALL_AC17_IS_GPIO
#endif

#define BALL_AE17 PAD_PCM_A2
#define PAD_PCM_A2 66
#define GPIO_PAD_66 GPIO65
#ifndef BALL_AE17_IS_GPIO
#define BALL_AE17_IS_GPIO 0
#endif
#ifndef PAD_PCM_A2_IS_GPIO
#define PAD_PCM_A2_IS_GPIO BALL_AE17_IS_GPIO
#endif

#define BALL_AD17 PAD_PCM_REG_N
#define PAD_PCM_REG_N 67
#define GPIO_PAD_67 GPIO66
#ifndef BALL_AD17_IS_GPIO
#define BALL_AD17_IS_GPIO 0
#endif
#ifndef PAD_PCM_REG_N_IS_GPIO
#define PAD_PCM_REG_N_IS_GPIO BALL_AD17_IS_GPIO
#endif

#define BALL_AC18 PAD_PCM_A1
#define PAD_PCM_A1 68
#define GPIO_PAD_68 GPIO67
#ifndef BALL_AC18_IS_GPIO
#define BALL_AC18_IS_GPIO 0
#endif
#ifndef PAD_PCM_A1_IS_GPIO
#define PAD_PCM_A1_IS_GPIO BALL_AC18_IS_GPIO
#endif

#define BALL_AD18 PAD_PCM_A0
#define PAD_PCM_A0 69
#define GPIO_PAD_69 GPIO68
#ifndef BALL_AD18_IS_GPIO
#define BALL_AD18_IS_GPIO 0
#endif
#ifndef PAD_PCM_A0_IS_GPIO
#define PAD_PCM_A0_IS_GPIO BALL_AD18_IS_GPIO
#endif

#define BALL_AD10 PAD_PCM_D0
#define PAD_PCM_D0 70
#define GPIO_PAD_70 GPIO69
#ifndef BALL_AD10_IS_GPIO
#define BALL_AD10_IS_GPIO 0
#endif
#ifndef PAD_PCM_D0_IS_GPIO
#define PAD_PCM_D0_IS_GPIO BALL_AD10_IS_GPIO
#endif

#define BALL_AC11 PAD_PCM_D1
#define PAD_PCM_D1 71
#define GPIO_PAD_71 GPIO70
#ifndef BALL_AC11_IS_GPIO
#define BALL_AC11_IS_GPIO 0
#endif
#ifndef PAD_PCM_D1_IS_GPIO
#define PAD_PCM_D1_IS_GPIO BALL_AC11_IS_GPIO
#endif

#define BALL_AD11 PAD_PCM_D2
#define PAD_PCM_D2 72
#define GPIO_PAD_72 GPIO71
#ifndef BALL_AD11_IS_GPIO
#define BALL_AD11_IS_GPIO 0
#endif
#ifndef PAD_PCM_D2_IS_GPIO
#define PAD_PCM_D2_IS_GPIO BALL_AD11_IS_GPIO
#endif

#define BALL_AD15 PAD_PCM_RESET
#define PAD_PCM_RESET 73
#define GPIO_PAD_73 GPIO72
#ifndef BALL_AD15_IS_GPIO
#define BALL_AD15_IS_GPIO 0
#endif
#ifndef PAD_PCM_RESET_IS_GPIO
#define PAD_PCM_RESET_IS_GPIO BALL_AD15_IS_GPIO
#endif

#define BALL_AA14 PAD_PCM2_RESET
#define PAD_PCM2_RESET 78
#define GPIO_PAD_78 GPIO77
#ifndef BALL_AA14_IS_GPIO
#define BALL_AA14_IS_GPIO 0
#endif
#ifndef PAD_PCM2_RESET_IS_GPIO
#define PAD_PCM2_RESET_IS_GPIO BALL_AA14_IS_GPIO
#endif

#define BALL_AA13 PAD_PCM_CD_N
#define PAD_PCM_CD_N 74
#define GPIO_PAD_74 GPIO73
#ifndef BALL_AA13_IS_GPIO
#define BALL_AA13_IS_GPIO 0
#endif
#ifndef PAD_PCM_CD_N_IS_GPIO
#define PAD_PCM_CD_N_IS_GPIO BALL_AA13_IS_GPIO
#endif

#define BALL_AA15 PAD_PCM2_CD_N
#define PAD_PCM2_CD_N 79
#define GPIO_PAD_79 GPIO78
#ifndef BALL_AA15_IS_GPIO
#define BALL_AA15_IS_GPIO 0
#endif
#ifndef PAD_PCM2_CD_N_IS_GPIO
#define PAD_PCM2_CD_N_IS_GPIO BALL_AA15_IS_GPIO
#endif

#define BALL_P21 PAD_TS1_D0
#define PAD_TS1_D0 38
#define GPIO_PAD_38 GPIO37
#ifndef BALL_P21_IS_GPIO
#define BALL_P21_IS_GPIO 0
#endif
#ifndef PAD_TS1_D0_IS_GPIO
#define PAD_TS1_D0_IS_GPIO BALL_P21_IS_GPIO
#endif

#define BALL_U21 PAD_TS1_VLD
#define PAD_TS1_VLD 39
#define GPIO_PAD_39 GPIO38
#ifndef BALL_U21_IS_GPIO
#define BALL_U21_IS_GPIO 0
#endif
#ifndef PAD_TS1_VLD_IS_GPIO
#define PAD_TS1_VLD_IS_GPIO BALL_U21_IS_GPIO
#endif

#define BALL_T21 PAD_TS1_SYNC
#define PAD_TS1_SYNC 40
#define GPIO_PAD_40 GPIO39
#ifndef BALL_T21_IS_GPIO
#define BALL_T21_IS_GPIO 0
#endif
#ifndef PAD_TS1_SYNC_IS_GPIO
#define PAD_TS1_SYNC_IS_GPIO BALL_T21_IS_GPIO
#endif

#define BALL_R21 PAD_TS1_CLK
#define PAD_TS1_CLK 41
#define GPIO_PAD_41 GPIO40
#ifndef BALL_R21_IS_GPIO
#define BALL_R21_IS_GPIO 0
#endif
#ifndef PAD_TS1_CLK_IS_GPIO
#define PAD_TS1_CLK_IS_GPIO BALL_R21_IS_GPIO
#endif

#define BALL_B11 PAD_ET_COL
#define PAD_ET_COL 117
#define GPIO_PAD_117 GPIO116
#ifndef BALL_B11_IS_GPIO
#define BALL_B11_IS_GPIO 0
#endif
#ifndef PAD_ET_COL_IS_GPIO
#define PAD_ET_COL_IS_GPIO BALL_B11_IS_GPIO
#endif

#define BALL_D9 PAD_ET_TXD1
#define PAD_ET_TXD1 118
#define GPIO_PAD_118 GPIO117
#ifndef BALL_D9_IS_GPIO
#define BALL_D9_IS_GPIO 0
#endif
#ifndef PAD_ET_TXD1_IS_GPIO
#define PAD_ET_TXD1_IS_GPIO BALL_D9_IS_GPIO
#endif

#define BALL_A10 PAD_ET_TXD0
#define PAD_ET_TXD0 119
#define GPIO_PAD_119 GPIO118
#ifndef BALL_A10_IS_GPIO
#define BALL_A10_IS_GPIO 0
#endif
#ifndef PAD_ET_TXD0_IS_GPIO
#define PAD_ET_TXD0_IS_GPIO BALL_A10_IS_GPIO
#endif

#define BALL_C9 PAD_ET_TX_EN
#define PAD_ET_TX_EN 120
#define GPIO_PAD_120 GPIO119
#ifndef BALL_C9_IS_GPIO
#define BALL_C9_IS_GPIO 0
#endif
#ifndef PAD_ET_TX_EN_IS_GPIO
#define PAD_ET_TX_EN_IS_GPIO BALL_C9_IS_GPIO
#endif

#define BALL_B9 PAD_ET_TX_CLK
#define PAD_ET_TX_CLK 121
#define GPIO_PAD_121 GPIO120
#ifndef BALL_B9_IS_GPIO
#define BALL_B9_IS_GPIO 0
#endif
#ifndef PAD_ET_TX_CLK_IS_GPIO
#define PAD_ET_TX_CLK_IS_GPIO BALL_B9_IS_GPIO
#endif

#define BALL_C10 PAD_ET_RXD0
#define PAD_ET_RXD0 122
#define GPIO_PAD_122 GPIO121
#ifndef BALL_C10_IS_GPIO
#define BALL_C10_IS_GPIO 0
#endif
#ifndef PAD_ET_RXD0_IS_GPIO
#define PAD_ET_RXD0_IS_GPIO BALL_C10_IS_GPIO
#endif

#define BALL_B10 PAD_I2S_IN_WS
#define PAD_I2S_IN_WS 126
#define GPIO_PAD_126 GPIO125
#ifndef BALL_B10_IS_GPIO
#define BALL_B10_IS_GPIO 0
#endif
#ifndef PAD_I2S_IN_WS_IS_GPIO
#define PAD_I2S_IN_WS_IS_GPIO BALL_B10_IS_GPIO
#endif

#define BALL_C11 PAD_I2S_IN_BCK
#define PAD_I2S_IN_BCK 127
#define GPIO_PAD_127 GPIO126
#ifndef BALL_C11_IS_GPIO
#define BALL_C11_IS_GPIO 0
#endif
#ifndef PAD_I2S_IN_BCK_IS_GPIO
#define PAD_I2S_IN_BCK_IS_GPIO BALL_C11_IS_GPIO
#endif

#define BALL_D11 PAD_I2S_IN_SD
#define PAD_I2S_IN_SD 128
#define GPIO_PAD_128 GPIO127
#ifndef BALL_D11_IS_GPIO
#define BALL_D11_IS_GPIO 0
#endif
#ifndef PAD_I2S_IN_SD_IS_GPIO
#define PAD_I2S_IN_SD_IS_GPIO BALL_D11_IS_GPIO
#endif

#define BALL_F9 PAD_SPDIF_IN
#define PAD_SPDIF_IN 129
#define GPIO_PAD_129 GPIO128
#ifndef BALL_F9_IS_GPIO
#define BALL_F9_IS_GPIO 0
#endif
#ifndef PAD_SPDIF_IN_IS_GPIO
#define PAD_SPDIF_IN_IS_GPIO BALL_F9_IS_GPIO
#endif

#define BALL_E11 PAD_I2S_OUT_MCK
#define PAD_I2S_OUT_MCK 131
#define GPIO_PAD_131 GPIO130
#ifndef BALL_E11_IS_GPIO
#define BALL_E11_IS_GPIO 0
#endif
#ifndef PAD_I2S_OUT_MCK_IS_GPIO
#define PAD_I2S_OUT_MCK_IS_GPIO BALL_E11_IS_GPIO
#endif

#define BALL_E12 PAD_I2S_OUT_WS
#define PAD_I2S_OUT_WS 132
#define GPIO_PAD_132 GPIO131
#ifndef BALL_E12_IS_GPIO
#define BALL_E12_IS_GPIO 0
#endif
#ifndef PAD_I2S_OUT_WS_IS_GPIO
#define PAD_I2S_OUT_WS_IS_GPIO BALL_E12_IS_GPIO
#endif

#define PAD_ET_CRS 9999
#ifndef PAD_ET_CRS_IS_GPIO
#define PAD_ET_CRS_IS_GPIO 0
#endif

#define PAD_ET_TXD3 9999
#ifndef PAD_ET_TXD3_IS_GPIO
#define PAD_ET_TXD3_IS_GPIO 0
#endif

#define BALL_F12 PAD_I2S_OUT_BCK
#define PAD_I2S_OUT_BCK 133
#define GPIO_PAD_133 GPIO132
#ifndef BALL_F12_IS_GPIO
#define BALL_F12_IS_GPIO 0
#endif
#ifndef PAD_I2S_OUT_BCK_IS_GPIO
#define PAD_I2S_OUT_BCK_IS_GPIO BALL_F12_IS_GPIO
#endif

#define PAD_ET_TXD2 9999
#ifndef PAD_ET_TXD2_IS_GPIO
#define PAD_ET_TXD2_IS_GPIO 0
#endif

#define BALL_E13 PAD_I2S_OUT_SD
#define PAD_I2S_OUT_SD 134
#define GPIO_PAD_134 GPIO133
#ifndef BALL_E13_IS_GPIO
#define BALL_E13_IS_GPIO 0
#endif
#ifndef PAD_I2S_OUT_SD_IS_GPIO
#define PAD_I2S_OUT_SD_IS_GPIO BALL_E13_IS_GPIO
#endif

#define PAD_ET_TXER 9999
#ifndef PAD_ET_TXER_IS_GPIO
#define PAD_ET_TXER_IS_GPIO 0
#endif

#define BALL_F8 PAD_SPDIF_OUT
#define PAD_SPDIF_OUT 130
#define GPIO_PAD_130 GPIO129
#ifndef BALL_F8_IS_GPIO
#define BALL_F8_IS_GPIO 0
#endif
#ifndef PAD_SPDIF_OUT_IS_GPIO
#define PAD_SPDIF_OUT_IS_GPIO BALL_F8_IS_GPIO
#endif

#define PAD_ET_RXER 9999
#ifndef PAD_ET_RXER_IS_GPIO
#define PAD_ET_RXER_IS_GPIO 0
#endif

#define PAD_DDCR_DA2 9999
#ifndef PAD_DDCR_DA2_IS_GPIO
#define PAD_DDCR_DA2_IS_GPIO 0
#endif

#define PAD_ET_RX_CLK 9999
#ifndef PAD_ET_RX_CLK_IS_GPIO
#define PAD_ET_RX_CLK_IS_GPIO 0
#endif

#define PAD_DDCR_CK2 9999
#ifndef PAD_DDCR_CK2_IS_GPIO
#define PAD_DDCR_CK2_IS_GPIO 0
#endif

#define PAD_ET_RX_DV 9999
#ifndef PAD_ET_RX_DV_IS_GPIO
#define PAD_ET_RX_DV_IS_GPIO 0
#endif

#define BALL_E8 PAD_LHSYNC2
#define PAD_LHSYNC2 145
#define GPIO_PAD_145 GPIO144
#ifndef BALL_E8_IS_GPIO
#define BALL_E8_IS_GPIO 0
#endif
#ifndef PAD_LHSYNC2_IS_GPIO
#define PAD_LHSYNC2_IS_GPIO BALL_E8_IS_GPIO
#endif

#define PAD_LVSYNC2 9999
#ifndef PAD_LVSYNC2_IS_GPIO
#define PAD_LVSYNC2_IS_GPIO 0
#endif

#define PAD_ET_RXD1 9999
#ifndef PAD_ET_RXD1_IS_GPIO
#define PAD_ET_RXD1_IS_GPIO 0
#endif

#define BALL_E5 PAD_GPIO12
#define PAD_GPIO12 147
#define GPIO_PAD_147 GPIO146
#ifndef BALL_E5_IS_GPIO
#define BALL_E5_IS_GPIO 0
#endif
#ifndef PAD_GPIO12_IS_GPIO
#define PAD_GPIO12_IS_GPIO BALL_E5_IS_GPIO
#endif

#define BALL_E10 PAD_ET_RXD2
#define PAD_ET_RXD2 115
#define GPIO_PAD_115 GPIO114
#ifndef BALL_E10_IS_GPIO
#define BALL_E10_IS_GPIO 0
#endif
#ifndef PAD_ET_RXD2_IS_GPIO
#define PAD_ET_RXD2_IS_GPIO BALL_E10_IS_GPIO
#endif

#define BALL_E6 PAD_GPIO13
#define PAD_GPIO13 148
#define GPIO_PAD_148 GPIO147
#ifndef BALL_E6_IS_GPIO
#define BALL_E6_IS_GPIO 0
#endif
#ifndef PAD_GPIO13_IS_GPIO
#define PAD_GPIO13_IS_GPIO BALL_E6_IS_GPIO
#endif

#define BALL_E9 PAD_ET_RXD3
#define PAD_ET_RXD3 116
#define GPIO_PAD_116 GPIO115
#ifndef BALL_E9_IS_GPIO
#define BALL_E9_IS_GPIO 0
#endif
#ifndef PAD_ET_RXD3_IS_GPIO
#define PAD_ET_RXD3_IS_GPIO BALL_E9_IS_GPIO
#endif

#define BALL_F6 PAD_GPIO14
#define PAD_GPIO14 149
#define GPIO_PAD_149 GPIO148
#ifndef BALL_F6_IS_GPIO
#define BALL_F6_IS_GPIO 0
#endif
#ifndef PAD_GPIO14_IS_GPIO
#define PAD_GPIO14_IS_GPIO BALL_F6_IS_GPIO
#endif

#define PAD_ET_MDC 9999
#ifndef PAD_ET_MDC_IS_GPIO
#define PAD_ET_MDC_IS_GPIO 0
#endif

#define BALL_F5 PAD_GPIO15
#define PAD_GPIO15 150
#define GPIO_PAD_150 GPIO149
#ifndef BALL_F5_IS_GPIO
#define BALL_F5_IS_GPIO 0
#endif
#ifndef PAD_GPIO15_IS_GPIO
#define PAD_GPIO15_IS_GPIO BALL_F5_IS_GPIO
#endif

#define PAD_ET_MDIO 9999
#ifndef PAD_ET_MDIO_IS_GPIO
#define PAD_ET_MDIO_IS_GPIO 0
#endif

#define BALL_B4 PAD_SAR0
#define PAD_SAR0 151
#define GPIO_PAD_151 GPIO150
#ifndef BALL_B4_IS_GPIO
#define BALL_B4_IS_GPIO 0
#endif
#ifndef PAD_SAR0_IS_GPIO
#define PAD_SAR0_IS_GPIO BALL_B4_IS_GPIO
#endif

#define BALL_C4 PAD_SAR1
#define PAD_SAR1 152
#define GPIO_PAD_152 GPIO151
#ifndef BALL_C4_IS_GPIO
#define BALL_C4_IS_GPIO 0
#endif
#ifndef PAD_SAR1_IS_GPIO
#define PAD_SAR1_IS_GPIO BALL_C4_IS_GPIO
#endif

#define BALL_B5 PAD_SAR2
#define PAD_SAR2 153
#define GPIO_PAD_153 GPIO152
#ifndef BALL_B5_IS_GPIO
#define BALL_B5_IS_GPIO 0
#endif
#ifndef PAD_SAR2_IS_GPIO
#define PAD_SAR2_IS_GPIO BALL_B5_IS_GPIO
#endif

#define BALL_C5 PAD_SAR3
#define PAD_SAR3 154
#define GPIO_PAD_154 GPIO153
#ifndef BALL_C5_IS_GPIO
#define BALL_C5_IS_GPIO 0
#endif
#ifndef PAD_SAR3_IS_GPIO
#define PAD_SAR3_IS_GPIO BALL_C5_IS_GPIO
#endif

#define PAD_GPIO_PM0 9999
#ifndef PAD_GPIO_PM0_IS_GPIO
#define PAD_GPIO_PM0_IS_GPIO 0
#endif

#define PAD_GPIO_PM1 9999
#ifndef PAD_GPIO_PM1_IS_GPIO
#define PAD_GPIO_PM1_IS_GPIO 0
#endif

#define BALL_E7 PAD_GPIO_PM2
#define PAD_GPIO_PM2 157
#define GPIO_PAD_157 GPIO156
#ifndef BALL_E7_IS_GPIO
#define BALL_E7_IS_GPIO 0
#endif
#ifndef PAD_GPIO_PM2_IS_GPIO
#define PAD_GPIO_PM2_IS_GPIO BALL_E7_IS_GPIO
#endif

#define PAD_GPIO_PM3 9999
#ifndef PAD_GPIO_PM3_IS_GPIO
#define PAD_GPIO_PM3_IS_GPIO 0
#endif

#define BALL_B6 PAD_GPIO_PM4
#define PAD_GPIO_PM4 159
#define GPIO_PAD_159 GPIO158
#ifndef BALL_B6_IS_GPIO
#define BALL_B6_IS_GPIO 0
#endif
#ifndef PAD_GPIO_PM4_IS_GPIO
#define PAD_GPIO_PM4_IS_GPIO BALL_B6_IS_GPIO
#endif

#define BALL_N4 PAD_GPIO_PM5
#define PAD_GPIO_PM5 160
#define GPIO_PAD_160 GPIO159
#ifndef BALL_N4_IS_GPIO
#define BALL_N4_IS_GPIO 0
#endif
#ifndef PAD_GPIO_PM5_IS_GPIO
#define PAD_GPIO_PM5_IS_GPIO BALL_N4_IS_GPIO
#endif

#define PAD_GPIO_PM6 9999
#ifndef PAD_GPIO_PM6_IS_GPIO
#define PAD_GPIO_PM6_IS_GPIO 0
#endif

#define PAD_IRIN_T 9999
#ifndef PAD_IRIN_T_IS_GPIO
#define PAD_IRIN_T_IS_GPIO 0
#endif

#define PAD_VSYNC0_T 9999
#ifndef PAD_VSYNC0_T_IS_GPIO
#define PAD_VSYNC0_T_IS_GPIO 0
#endif

#define PAD_VSYNC1_T 9999
#ifndef PAD_VSYNC1_T_IS_GPIO
#define PAD_VSYNC1_T_IS_GPIO 0
#endif

#define PAD_CEC_T 9999
#ifndef PAD_CEC_T_IS_GPIO
#define PAD_CEC_T_IS_GPIO 0
#endif

#define BALL_M4 PAD_GPIO_PM7
#define PAD_GPIO_PM7 166
#define GPIO_PAD_166 GPIO165
#ifndef BALL_M4_IS_GPIO
#define BALL_M4_IS_GPIO 0
#endif
#ifndef PAD_GPIO_PM7_IS_GPIO
#define PAD_GPIO_PM7_IS_GPIO BALL_M4_IS_GPIO
#endif

#define BALL_L4 PAD_GPIO_PM8
#define PAD_GPIO_PM8 167
#define GPIO_PAD_167 GPIO166
#ifndef BALL_L4_IS_GPIO
#define BALL_L4_IS_GPIO 0
#endif
#ifndef PAD_GPIO_PM8_IS_GPIO
#define PAD_GPIO_PM8_IS_GPIO BALL_L4_IS_GPIO
#endif

#define PAD_HSYNC0 9998
#define PAD_HSYNC1 9997
#define PAD_HSYNC2 9996


// PADS group name to index
#define PADS_DDCR           1223
#define PADS_DDCR2          1224
#define PADS_TS0            1225
#define PADS_TS1            1226
#define PADS_GPIO1_GPIO2    1227
#define PADS_LHSYNC2_LVSYNC2 1228
#define PADS_PCI_AD20_AD21  1229
#define PADS_PCI            1230
#define PADS_PCM            1231
#define PADS_SAR            1232
#define PADS_DOUT           1233
#define PADS_DIN            1234
#define PADS_I2S            1235
#define PADS_PF             1236
#define PADS_EMAC           1237
#define PADS_TCON           1238
#define PADS_UART1_MODE1    1239
#define PADS_UART1_MODE2    1240
#define PADS_UART1_MODE3    1241
#define PADS_UART2_MODE1    1242
#define PADS_UART2_MODE2    1243
#define PADS_UART2_MODE3    1244
#define PADS_ET_MODE0       1245
#define PADS_ET_MODE1       1246
#define PADS_ET_MODE2       1247
#define PADS_ET_MODE3       1248
#define PADS_MIIC           1249
#define PADS_PF_MODE1       1250
#define PADS_PF_MODE2       1251
#define PADS_PF_MODE3       1252
#define PADS_SPI_CZ1        1253
#define PADS_SPI_CZ2        1254
#define PADS_SPI_CZ3        1255
#define PADS_DDCR3          1256
#define PADS_MPIF_MODE1     1257
#define PADS_MPIF_MODE2     1258
#define PADS_MPIF_MODE3     1259
#define PADS_RF_AGC         1260
#define PADS_IF_AGC         1261
#define PADS_TS0_PARALLEL_MODE  1262
#define PADS_TS0_SERIAL_MODE    1263
#define PADS_SM1_ENABLE     1264
#define PADS_OCP1_ENABLE    1265
#define PADS_UART3_MODE1    1266
#define PADS_UART3_MODE2    1267
#define PADS_SYNC_C0        1268
#define PADS_SYNC_C1        1269
#define PADS_SYNC_C0_T      1270
#define PADS_SYNC_C1_T      1271

// chip capbilities
#define CHIP_HAS_BT656_IN          0
#define CHIP_HAS_TS0               1
#define CHIP_HAS_TS1               0
#define CHIP_HAS_LVDS_10BIT_X2     1
#define CHIP_HAS_LVDS_8BIT_X2      1
#define CHIP_HAS_LVDS_8BIT_X1      1
#define CHIP_HAS_CIMAX             0
#define CHIP_HAS_USB_P0            1
#define CHIP_HAS_USB_P1            1
#define CHIP_HAS_USBVBUS0          0
#define CHIP_HAS_USBVBUS1          0
#define CHIP_HAS_UART1_MODE1       1
#define CHIP_HAS_UART1_MODE2       1
#define CHIP_HAS_UART1_MODE3       1
#define CHIP_HAS_PCMCIA1           0
#define CHIP_HAS_PCMCIA2           0
#define CHIP_HAS_PCMCIA            0
#define CHIP_HAS_UART2_MODE1       1
#define CHIP_HAS_UART2_MODE2       1
#define CHIP_HAS_UART2_MODE3       1
#define CHIP_HAS_RGB_A             1
#define CHIP_HAS_RGB_B             1
#define CHIP_HAS_RGB_C             1
#define CHIP_HAS_HDMI_A            1
#define CHIP_HAS_HDMI_B            1
#define CHIP_HAS_HDMI_C            1
#define CHIP_HAS_SPI_CZ1           1
#define CHIP_HAS_SPI_CZ2           0
#define CHIP_HAS_SPI_CZ3           0
#define CHIP_HAS_DDCR              1
#define CHIP_HAS_DDCR2             1
#define CHIP_HAS_TCON              1
#define CHIP_HAS_ET_MODE0_MII      1
#define CHIP_HAS_ET_MODE0_RMII     1
#define CHIP_HAS_ET_MODE1_MII      1
#define CHIP_HAS_ET_MODE1_RMII     1
#define CHIP_HAS_ET_MODE2_MII      1
#define CHIP_HAS_ET_MODE2_RMII     1
#define CHIP_HAS_I2S_IN            1
#define CHIP_HAS_I2S_OUT           1
#define CHIP_HAS_SPDIF_IN          0
#define CHIP_HAS_SPDIF_OUT         1
#define CHIP_HAS_DDR_A_CMD         1
#define CHIP_HAS_DDR_A_DATA        1
#define CHIP_HAS_DDR_B_CMD         1
#define CHIP_HAS_DDR_B_DATA        1
#define CHIP_USE_PCM_CDN_IN_PCM2   0
#define CHIP_USE_I2S_IN_IN_PCM     0
#define CHIP_USE_I2S_OUT_IN_PCM    0
#define CHIP_USE_SPDIF_IN_IN_PCM   0
#define CHIP_USE_SPDIF_OUT_IN_PCM  0
#define CHIP_USE_IIC_IN_DDCR2      0
#define CHIP_USE_PWM2_IN_GPIO14    0
#define CHIP_USE_PWM3_IN_GPIO15    0

// chip additional information
#define CHIP_FAMILY_TYPE           CHIP_FAMILY_U3

#endif /* _MSD3A21HX_H_*/
