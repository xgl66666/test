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
#ifndef _MSD7M36_H_
#define _MSD7M36_H_

#define PAD_NC 9999
#define PAD_GND 0

#define PIN_2 PAD_GPIO_PM9
#define PAD_GPIO_PM9 168
#define GPIO_PAD_168 GPIO167
#ifndef PIN_2_IS_GPIO
#define PIN_2_IS_GPIO 0
#endif
#ifndef PAD_GPIO_PM9_IS_GPIO
#define PAD_GPIO_PM9_IS_GPIO PIN_2_IS_GPIO
#endif

#define PIN_3 PAD_GPIO_PM10
#define PAD_GPIO_PM10 169
#define GPIO_PAD_169 GPIO168
#ifndef PIN_3_IS_GPIO
#define PIN_3_IS_GPIO 0
#endif
#ifndef PAD_GPIO_PM10_IS_GPIO
#define PAD_GPIO_PM10_IS_GPIO PIN_3_IS_GPIO
#endif

#define PIN_4 PAD_GPIO_PM11
#define PAD_GPIO_PM11 170
#define GPIO_PAD_170 GPIO169
#ifndef PIN_4_IS_GPIO
#define PIN_4_IS_GPIO 0
#endif
#ifndef PAD_GPIO_PM11_IS_GPIO
#define PAD_GPIO_PM11_IS_GPIO PIN_4_IS_GPIO
#endif

#define PIN_5 PAD_GPIO_PM12
#define PAD_GPIO_PM12 171
#define GPIO_PAD_171 GPIO170
#ifndef PIN_5_IS_GPIO
#define PIN_5_IS_GPIO 0
#endif
#ifndef PAD_GPIO_PM12_IS_GPIO
#define PAD_GPIO_PM12_IS_GPIO PIN_5_IS_GPIO
#endif

#define PIN_6 PAD_IRIN
#define PAD_IRIN 172
#define GPIO_PAD_172 GPIO171
#ifndef PIN_6_IS_GPIO
#define PIN_6_IS_GPIO 0
#endif
#ifndef PAD_IRIN_IS_GPIO
#define PAD_IRIN_IS_GPIO PIN_6_IS_GPIO
#endif

#define PIN_8 PAD_VSYNC0
#define PAD_VSYNC0 173
#define GPIO_PAD_173 GPIO172
#ifndef PIN_8_IS_GPIO
#define PIN_8_IS_GPIO 0
#endif
#ifndef PAD_VSYNC0_IS_GPIO
#define PAD_VSYNC0_IS_GPIO PIN_8_IS_GPIO
#endif

#define PIN_10 PAD_VSYNC1
#define PAD_VSYNC1 174
#define GPIO_PAD_174 GPIO173
#ifndef PIN_10_IS_GPIO
#define PIN_10_IS_GPIO 0
#endif
#ifndef PAD_VSYNC1_IS_GPIO
#define PAD_VSYNC1_IS_GPIO PIN_10_IS_GPIO
#endif

#define PIN_11 PAD_CEC
#define PAD_CEC 175
#define GPIO_PAD_175 GPIO174
#ifndef PIN_11_IS_GPIO
#define PIN_11_IS_GPIO 0
#endif
#ifndef PAD_CEC_IS_GPIO
#define PAD_CEC_IS_GPIO PIN_11_IS_GPIO
#endif

#define PIN_12 PAD_TSO_CLK
#define PAD_TSO_CLK 11
#define GPIO_PAD_11 GPIO10
#ifndef PIN_12_IS_GPIO
#define PIN_12_IS_GPIO 0
#endif
#ifndef PAD_TSO_CLK_IS_GPIO
#define PAD_TSO_CLK_IS_GPIO PIN_12_IS_GPIO
#endif

#define PIN_13 PAD_TSO_SYNC
#define PAD_TSO_SYNC 10
#define GPIO_PAD_10 GPIO9
#ifndef PIN_13_IS_GPIO
#define PIN_13_IS_GPIO 0
#endif
#ifndef PAD_TSO_SYNC_IS_GPIO
#define PAD_TSO_SYNC_IS_GPIO PIN_13_IS_GPIO
#endif

#define PIN_14 PAD_TSO_VLD
#define PAD_TSO_VLD 9
#define GPIO_PAD_9 GPIO8
#ifndef PIN_14_IS_GPIO
#define PIN_14_IS_GPIO 0
#endif
#ifndef PAD_TSO_VLD_IS_GPIO
#define PAD_TSO_VLD_IS_GPIO PIN_14_IS_GPIO
#endif

#define PIN_15 PAD_TSO_D7
#define PAD_TSO_D7 8
#define GPIO_PAD_8 GPIO7
#ifndef PIN_15_IS_GPIO
#define PIN_15_IS_GPIO 0
#endif
#ifndef PAD_TSO_D7_IS_GPIO
#define PAD_TSO_D7_IS_GPIO PIN_15_IS_GPIO
#endif

#define PIN_16 PAD_TSO_D6
#define PAD_TSO_D6 7
#define GPIO_PAD_7 GPIO6
#ifndef PIN_16_IS_GPIO
#define PIN_16_IS_GPIO 0
#endif
#ifndef PAD_TSO_D6_IS_GPIO
#define PAD_TSO_D6_IS_GPIO PIN_16_IS_GPIO
#endif

#define PIN_17 PAD_TSO_D5
#define PAD_TSO_D5 6
#define GPIO_PAD_6 GPIO5
#ifndef PIN_17_IS_GPIO
#define PIN_17_IS_GPIO 0
#endif
#ifndef PAD_TSO_D5_IS_GPIO
#define PAD_TSO_D5_IS_GPIO PIN_17_IS_GPIO
#endif

#define PIN_18 PAD_TSO_D4
#define PAD_TSO_D4 5
#define GPIO_PAD_5 GPIO4
#ifndef PIN_18_IS_GPIO
#define PIN_18_IS_GPIO 0
#endif
#ifndef PAD_TSO_D4_IS_GPIO
#define PAD_TSO_D4_IS_GPIO PIN_18_IS_GPIO
#endif

#define PIN_19 PAD_TSO_D3
#define PAD_TSO_D3 4
#define GPIO_PAD_4 GPIO3
#ifndef PIN_19_IS_GPIO
#define PIN_19_IS_GPIO 0
#endif
#ifndef PAD_TSO_D3_IS_GPIO
#define PAD_TSO_D3_IS_GPIO PIN_19_IS_GPIO
#endif

#define PIN_20 PAD_TSO_D2
#define PAD_TSO_D2 3
#define GPIO_PAD_3 GPIO2
#ifndef PIN_20_IS_GPIO
#define PIN_20_IS_GPIO 0
#endif
#ifndef PAD_TSO_D2_IS_GPIO
#define PAD_TSO_D2_IS_GPIO PIN_20_IS_GPIO
#endif

#define PIN_21 PAD_TSO_D1
#define PAD_TSO_D1 2
#define GPIO_PAD_2 GPIO1
#ifndef PIN_21_IS_GPIO
#define PIN_21_IS_GPIO 0
#endif
#ifndef PAD_TSO_D1_IS_GPIO
#define PAD_TSO_D1_IS_GPIO PIN_21_IS_GPIO
#endif

#define PIN_22 PAD_TSO_D0
#define PAD_TSO_D0 1
#define GPIO_PAD_1 GPIO0
#ifndef PIN_22_IS_GPIO
#define PIN_22_IS_GPIO 0
#endif
#ifndef PAD_TSO_D0_IS_GPIO
#define PAD_TSO_D0_IS_GPIO PIN_22_IS_GPIO
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

#define PIN_57 PAD_I2CM_SCL1
#define PAD_I2CM_SCL1 23
#define GPIO_PAD_23 GPIO22
#ifndef PIN_57_IS_GPIO
#define PIN_57_IS_GPIO 0
#endif
#ifndef PAD_I2CM_SCL1_IS_GPIO
#define PAD_I2CM_SCL1_IS_GPIO PIN_57_IS_GPIO
#endif

#define PIN_58 PAD_I2CM_SDA1
#define PAD_I2CM_SDA1 24
#define GPIO_PAD_24 GPIO23
#ifndef PIN_58_IS_GPIO
#define PIN_58_IS_GPIO 0
#endif
#ifndef PAD_I2CM_SDA1_IS_GPIO
#define PAD_I2CM_SDA1_IS_GPIO PIN_58_IS_GPIO
#endif

#define PIN_59 PAD_RF_AGC
#define PAD_RF_AGC 25
#define GPIO_PAD_25 GPIO24
#ifndef PIN_59_IS_GPIO
#define PIN_59_IS_GPIO 0
#endif
#ifndef PAD_RF_AGC_IS_GPIO
#define PAD_RF_AGC_IS_GPIO PIN_59_IS_GPIO
#endif

#define PIN_60 PAD_IF_AGC
#define PAD_IF_AGC 26
#define GPIO_PAD_26 GPIO25
#ifndef PIN_60_IS_GPIO
#define PIN_60_IS_GPIO 0
#endif
#ifndef PAD_IF_AGC_IS_GPIO
#define PAD_IF_AGC_IS_GPIO PIN_60_IS_GPIO
#endif

#define PIN_75 PAD_DDCA_CK_PM
#define PAD_DDCA_CK_PM 176
#define GPIO_PAD_176 GPIO175
#ifndef PIN_75_IS_GPIO
#define PIN_75_IS_GPIO 0
#endif
#ifndef PAD_DDCA_CK_PM_IS_GPIO
#define PAD_DDCA_CK_PM_IS_GPIO PIN_75_IS_GPIO
#endif

#define PIN_76 PAD_DDCA_DA_PM
#define PAD_DDCA_DA_PM 177
#define GPIO_PAD_177 GPIO176
#ifndef PIN_76_IS_GPIO
#define PIN_76_IS_GPIO 0
#endif
#ifndef PAD_DDCA_DA_PM_IS_GPIO
#define PAD_DDCA_DA_PM_IS_GPIO PIN_76_IS_GPIO
#endif

#define PIN_77 PAD_DDCDA_CK_PM
#define PAD_DDCDA_CK_PM 178
#define GPIO_PAD_178 GPIO177
#ifndef PIN_77_IS_GPIO
#define PIN_77_IS_GPIO 0
#endif
#ifndef PAD_DDCDA_CK_PM_IS_GPIO
#define PAD_DDCDA_CK_PM_IS_GPIO PIN_77_IS_GPIO
#endif

#define PIN_78 PAD_DDCDA_DA_PM
#define PAD_DDCDA_DA_PM 179
#define GPIO_PAD_179 GPIO178
#ifndef PIN_78_IS_GPIO
#define PIN_78_IS_GPIO 0
#endif
#ifndef PAD_DDCDA_DA_PM_IS_GPIO
#define PAD_DDCDA_DA_PM_IS_GPIO PIN_78_IS_GPIO
#endif

#define PAD_DDCDB_CK_PM 9999
#ifndef PAD_DDCDB_CK_PM_IS_GPIO
#define PAD_DDCDB_CK_PM_IS_GPIO 0
#endif

#define PAD_DDCDB_DA_PM 9999
#ifndef PAD_DDCDB_DA_PM_IS_GPIO
#define PAD_DDCDB_DA_PM_IS_GPIO 0
#endif

#define PIN_80 PAD_TS0_D0
#define PAD_TS0_D0 27
#define GPIO_PAD_27 GPIO26
#ifndef PIN_80_IS_GPIO
#define PIN_80_IS_GPIO 0
#endif
#ifndef PAD_TS0_D0_IS_GPIO
#define PAD_TS0_D0_IS_GPIO PIN_80_IS_GPIO
#endif

#define PIN_81 PAD_TS0_D1
#define PAD_TS0_D1 28
#define GPIO_PAD_28 GPIO27
#ifndef PIN_81_IS_GPIO
#define PIN_81_IS_GPIO 0
#endif
#ifndef PAD_TS0_D1_IS_GPIO
#define PAD_TS0_D1_IS_GPIO PIN_81_IS_GPIO
#endif

#define PIN_82 PAD_TS0_D2
#define PAD_TS0_D2 29
#define GPIO_PAD_29 GPIO28
#ifndef PIN_82_IS_GPIO
#define PIN_82_IS_GPIO 0
#endif
#ifndef PAD_TS0_D2_IS_GPIO
#define PAD_TS0_D2_IS_GPIO PIN_82_IS_GPIO
#endif

#define PIN_83 PAD_TS0_D3
#define PAD_TS0_D3 30
#define GPIO_PAD_30 GPIO29
#ifndef PIN_83_IS_GPIO
#define PIN_83_IS_GPIO 0
#endif
#ifndef PAD_TS0_D3_IS_GPIO
#define PAD_TS0_D3_IS_GPIO PIN_83_IS_GPIO
#endif

#define PIN_84 PAD_TS0_D4
#define PAD_TS0_D4 31
#define GPIO_PAD_31 GPIO30
#ifndef PIN_84_IS_GPIO
#define PIN_84_IS_GPIO 0
#endif
#ifndef PAD_TS0_D4_IS_GPIO
#define PAD_TS0_D4_IS_GPIO PIN_84_IS_GPIO
#endif

#define PIN_85 PAD_TS0_D5
#define PAD_TS0_D5 32
#define GPIO_PAD_32 GPIO31
#ifndef PIN_85_IS_GPIO
#define PIN_85_IS_GPIO 0
#endif
#ifndef PAD_TS0_D5_IS_GPIO
#define PAD_TS0_D5_IS_GPIO PIN_85_IS_GPIO
#endif

#define PIN_86 PAD_TS0_D6
#define PAD_TS0_D6 33
#define GPIO_PAD_33 GPIO32
#ifndef PIN_86_IS_GPIO
#define PIN_86_IS_GPIO 0
#endif
#ifndef PAD_TS0_D6_IS_GPIO
#define PAD_TS0_D6_IS_GPIO PIN_86_IS_GPIO
#endif

#define PIN_87 PAD_TS0_D7
#define PAD_TS0_D7 34
#define GPIO_PAD_34 GPIO33
#ifndef PIN_87_IS_GPIO
#define PIN_87_IS_GPIO 0
#endif
#ifndef PAD_TS0_D7_IS_GPIO
#define PAD_TS0_D7_IS_GPIO PIN_87_IS_GPIO
#endif

#define PIN_88 PAD_TS0_VLD
#define PAD_TS0_VLD 35
#define GPIO_PAD_35 GPIO34
#ifndef PIN_88_IS_GPIO
#define PIN_88_IS_GPIO 0
#endif
#ifndef PAD_TS0_VLD_IS_GPIO
#define PAD_TS0_VLD_IS_GPIO PIN_88_IS_GPIO
#endif

#define PIN_89 PAD_TS0_SYNC
#define PAD_TS0_SYNC 36
#define GPIO_PAD_36 GPIO35
#ifndef PIN_89_IS_GPIO
#define PIN_89_IS_GPIO 0
#endif
#ifndef PAD_TS0_SYNC_IS_GPIO
#define PAD_TS0_SYNC_IS_GPIO PIN_89_IS_GPIO
#endif

#define PIN_90 PAD_TS0_CLK
#define PAD_TS0_CLK 37
#define GPIO_PAD_37 GPIO36
#ifndef PIN_90_IS_GPIO
#define PIN_90_IS_GPIO 0
#endif
#ifndef PAD_TS0_CLK_IS_GPIO
#define PAD_TS0_CLK_IS_GPIO PIN_90_IS_GPIO
#endif

#define PAD_PCM_D3 9999
#ifndef PAD_PCM_D3_IS_GPIO
#define PAD_PCM_D3_IS_GPIO 0
#endif

#define PAD_PCM_D4 9999
#ifndef PAD_PCM_D4_IS_GPIO
#define PAD_PCM_D4_IS_GPIO 0
#endif

#define PIN_91 PAD_PCM_D5
#define PAD_PCM_D5 62
#define GPIO_PAD_62 GPIO61
#ifndef PIN_91_IS_GPIO
#define PIN_91_IS_GPIO 0
#endif
#ifndef PAD_PCM_D5_IS_GPIO
#define PAD_PCM_D5_IS_GPIO PIN_91_IS_GPIO
#endif

#define PIN_92 PAD_PCM_D6
#define PAD_PCM_D6 61
#define GPIO_PAD_61 GPIO60
#ifndef PIN_92_IS_GPIO
#define PIN_92_IS_GPIO 0
#endif
#ifndef PAD_PCM_D6_IS_GPIO
#define PAD_PCM_D6_IS_GPIO PIN_92_IS_GPIO
#endif

#define PIN_93 PAD_PCM_D7
#define PAD_PCM_D7 60
#define GPIO_PAD_60 GPIO59
#ifndef PIN_93_IS_GPIO
#define PIN_93_IS_GPIO 0
#endif
#ifndef PAD_PCM_D7_IS_GPIO
#define PAD_PCM_D7_IS_GPIO PIN_93_IS_GPIO
#endif

#define PIN_94 PAD_PCM_CE_N
#define PAD_PCM_CE_N 59
#define GPIO_PAD_59 GPIO58
#ifndef PIN_94_IS_GPIO
#define PIN_94_IS_GPIO 0
#endif
#ifndef PAD_PCM_CE_N_IS_GPIO
#define PAD_PCM_CE_N_IS_GPIO PIN_94_IS_GPIO
#endif

#define PIN_95 PAD_PCM_A10
#define PAD_PCM_A10 58
#define GPIO_PAD_58 GPIO57
#ifndef PIN_95_IS_GPIO
#define PIN_95_IS_GPIO 0
#endif
#ifndef PAD_PCM_A10_IS_GPIO
#define PAD_PCM_A10_IS_GPIO PIN_95_IS_GPIO
#endif

#define PIN_96 PAD_PCM_OE_N
#define PAD_PCM_OE_N 57
#define GPIO_PAD_57 GPIO56
#ifndef PIN_96_IS_GPIO
#define PIN_96_IS_GPIO 0
#endif
#ifndef PAD_PCM_OE_N_IS_GPIO
#define PAD_PCM_OE_N_IS_GPIO PIN_96_IS_GPIO
#endif

#define PIN_98 PAD_PCM_A11
#define PAD_PCM_A11 56
#define GPIO_PAD_56 GPIO55
#ifndef PIN_98_IS_GPIO
#define PIN_98_IS_GPIO 0
#endif
#ifndef PAD_PCM_A11_IS_GPIO
#define PAD_PCM_A11_IS_GPIO PIN_98_IS_GPIO
#endif

#define PIN_99 PAD_PCM_IORD_N
#define PAD_PCM_IORD_N 55
#define GPIO_PAD_55 GPIO54
#ifndef PIN_99_IS_GPIO
#define PIN_99_IS_GPIO 0
#endif
#ifndef PAD_PCM_IORD_N_IS_GPIO
#define PAD_PCM_IORD_N_IS_GPIO PIN_99_IS_GPIO
#endif

#define PIN_100 PAD_PCM_A9
#define PAD_PCM_A9 54
#define GPIO_PAD_54 GPIO53
#ifndef PIN_100_IS_GPIO
#define PIN_100_IS_GPIO 0
#endif
#ifndef PAD_PCM_A9_IS_GPIO
#define PAD_PCM_A9_IS_GPIO PIN_100_IS_GPIO
#endif

#define PAD_PCM_IOWR_N 9999
#ifndef PAD_PCM_IOWR_N_IS_GPIO
#define PAD_PCM_IOWR_N_IS_GPIO 0
#endif

#define PAD_PCM_A8 9999
#ifndef PAD_PCM_A8_IS_GPIO
#define PAD_PCM_A8_IS_GPIO 0
#endif

#define PAD_PCM_A13 9999
#ifndef PAD_PCM_A13_IS_GPIO
#define PAD_PCM_A13_IS_GPIO 0
#endif

#define PAD_PCM_A14 9999
#ifndef PAD_PCM_A14_IS_GPIO
#define PAD_PCM_A14_IS_GPIO 0
#endif

#define PAD_PCM_WE_N 9999
#ifndef PAD_PCM_WE_N_IS_GPIO
#define PAD_PCM_WE_N_IS_GPIO 0
#endif

#define PAD_PCM_IRQA_N 9999
#ifndef PAD_PCM_IRQA_N_IS_GPIO
#define PAD_PCM_IRQA_N_IS_GPIO 0
#endif

#define PAD_PCM_A12 9999
#ifndef PAD_PCM_A12_IS_GPIO
#define PAD_PCM_A12_IS_GPIO 0
#endif

#define PIN_101 PAD_PCM_A7
#define PAD_PCM_A7 46
#define GPIO_PAD_46 GPIO45
#ifndef PIN_101_IS_GPIO
#define PIN_101_IS_GPIO 0
#endif
#ifndef PAD_PCM_A7_IS_GPIO
#define PAD_PCM_A7_IS_GPIO PIN_101_IS_GPIO
#endif

#define PIN_102 PAD_PCM_A6
#define PAD_PCM_A6 45
#define GPIO_PAD_45 GPIO44
#ifndef PIN_102_IS_GPIO
#define PIN_102_IS_GPIO 0
#endif
#ifndef PAD_PCM_A6_IS_GPIO
#define PAD_PCM_A6_IS_GPIO PIN_102_IS_GPIO
#endif

#define PIN_103 PAD_PCM_A5
#define PAD_PCM_A5 44
#define GPIO_PAD_44 GPIO43
#ifndef PIN_103_IS_GPIO
#define PIN_103_IS_GPIO 0
#endif
#ifndef PAD_PCM_A5_IS_GPIO
#define PAD_PCM_A5_IS_GPIO PIN_103_IS_GPIO
#endif

#define PAD_PCM_WAIT_N 9999
#ifndef PAD_PCM_WAIT_N_IS_GPIO
#define PAD_PCM_WAIT_N_IS_GPIO 0
#endif

#define PIN_104 PAD_PCM_A4
#define PAD_PCM_A4 42
#define GPIO_PAD_42 GPIO41
#ifndef PIN_104_IS_GPIO
#define PIN_104_IS_GPIO 0
#endif
#ifndef PAD_PCM_A4_IS_GPIO
#define PAD_PCM_A4_IS_GPIO PIN_104_IS_GPIO
#endif

#define PAD_PCM2_CE_N 9999
#ifndef PAD_PCM2_CE_N_IS_GPIO
#define PAD_PCM2_CE_N_IS_GPIO 0
#endif

#define PAD_PCM2_IRQA_N 9999
#ifndef PAD_PCM2_IRQA_N_IS_GPIO
#define PAD_PCM2_IRQA_N_IS_GPIO 0
#endif

#define PAD_PCM2_WAIT_N 9999
#ifndef PAD_PCM2_WAIT_N_IS_GPIO
#define PAD_PCM2_WAIT_N_IS_GPIO 0
#endif

#define PIN_106 PAD_UART_RX2
#define PAD_UART_RX2 135
#define GPIO_PAD_135 GPIO134
#ifndef PIN_106_IS_GPIO
#define PIN_106_IS_GPIO 0
#endif
#ifndef PAD_UART_RX2_IS_GPIO
#define PAD_UART_RX2_IS_GPIO PIN_106_IS_GPIO
#endif

#define PIN_107 PAD_UART_TX2
#define PAD_UART_TX2 136
#define GPIO_PAD_136 GPIO135
#ifndef PIN_107_IS_GPIO
#define PIN_107_IS_GPIO 0
#endif
#ifndef PAD_UART_TX2_IS_GPIO
#define PAD_UART_TX2_IS_GPIO PIN_107_IS_GPIO
#endif

#define PIN_108 PAD_PF_AD15
#define PAD_PF_AD15 80
#define GPIO_PAD_80 GPIO79
#ifndef PIN_108_IS_GPIO
#define PIN_108_IS_GPIO 0
#endif
#ifndef PAD_PF_AD15_IS_GPIO
#define PAD_PF_AD15_IS_GPIO PIN_108_IS_GPIO
#endif

#define PIN_109 PAD_PF_ALE
#define PAD_PF_ALE 85
#define GPIO_PAD_85 GPIO84
#ifndef PIN_109_IS_GPIO
#define PIN_109_IS_GPIO 0
#endif
#ifndef PAD_PF_ALE_IS_GPIO
#define PAD_PF_ALE_IS_GPIO PIN_109_IS_GPIO
#endif

#define PIN_110 PAD_PF_CE0Z
#define PAD_PF_CE0Z 81
#define GPIO_PAD_81 GPIO80
#ifndef PIN_110_IS_GPIO
#define PIN_110_IS_GPIO 0
#endif
#ifndef PAD_PF_CE0Z_IS_GPIO
#define PAD_PF_CE0Z_IS_GPIO PIN_110_IS_GPIO
#endif

#define PIN_111 PAD_PF_CE1Z
#define PAD_PF_CE1Z 82
#define GPIO_PAD_82 GPIO81
#ifndef PIN_111_IS_GPIO
#define PIN_111_IS_GPIO 0
#endif
#ifndef PAD_PF_CE1Z_IS_GPIO
#define PAD_PF_CE1Z_IS_GPIO PIN_111_IS_GPIO
#endif

#define PIN_112 PAD_PF_OEZ
#define PAD_PF_OEZ 83
#define GPIO_PAD_83 GPIO82
#ifndef PIN_112_IS_GPIO
#define PIN_112_IS_GPIO 0
#endif
#ifndef PAD_PF_OEZ_IS_GPIO
#define PAD_PF_OEZ_IS_GPIO PIN_112_IS_GPIO
#endif

#define PIN_113 PAD_PF_WEZ
#define PAD_PF_WEZ 84
#define GPIO_PAD_84 GPIO83
#ifndef PIN_113_IS_GPIO
#define PIN_113_IS_GPIO 0
#endif
#ifndef PAD_PF_WEZ_IS_GPIO
#define PAD_PF_WEZ_IS_GPIO PIN_113_IS_GPIO
#endif

#define PIN_114 PAD_F_RBZ
#define PAD_F_RBZ 86
#define GPIO_PAD_86 GPIO85
#ifndef PIN_114_IS_GPIO
#define PIN_114_IS_GPIO 0
#endif
#ifndef PAD_F_RBZ_IS_GPIO
#define PAD_F_RBZ_IS_GPIO PIN_114_IS_GPIO
#endif

#define PIN_141 PAD_SPI_CK
#define PAD_SPI_CK 87
#define GPIO_PAD_87 GPIO86
#ifndef PIN_141_IS_GPIO
#define PIN_141_IS_GPIO 0
#endif
#ifndef PAD_SPI_CK_IS_GPIO
#define PAD_SPI_CK_IS_GPIO PIN_141_IS_GPIO
#endif

#define PIN_142 PAD_SPI_DI
#define PAD_SPI_DI 88
#define GPIO_PAD_88 GPIO87
#ifndef PIN_142_IS_GPIO
#define PIN_142_IS_GPIO 0
#endif
#ifndef PAD_SPI_DI_IS_GPIO
#define PAD_SPI_DI_IS_GPIO PIN_142_IS_GPIO
#endif

#define PIN_143 PAD_SPI_DO
#define PAD_SPI_DO 89
#define GPIO_PAD_89 GPIO88
#ifndef PIN_143_IS_GPIO
#define PIN_143_IS_GPIO 0
#endif
#ifndef PAD_SPI_DO_IS_GPIO
#define PAD_SPI_DO_IS_GPIO PIN_143_IS_GPIO
#endif

#define PIN_144 PAD_SPI_CZ
#define PAD_SPI_CZ 90
#define GPIO_PAD_90 GPIO89
#ifndef PIN_144_IS_GPIO
#define PIN_144_IS_GPIO 0
#endif
#ifndef PAD_SPI_CZ_IS_GPIO
#define PAD_SPI_CZ_IS_GPIO PIN_144_IS_GPIO
#endif

#define PIN_145 PAD_PWM0
#define PAD_PWM0 137
#define GPIO_PAD_137 GPIO136
#ifndef PIN_145_IS_GPIO
#define PIN_145_IS_GPIO 0
#endif
#ifndef PAD_PWM0_IS_GPIO
#define PAD_PWM0_IS_GPIO PIN_145_IS_GPIO
#endif

#define PIN_146 PAD_PWM1
#define PAD_PWM1 138
#define GPIO_PAD_138 GPIO137
#ifndef PIN_146_IS_GPIO
#define PIN_146_IS_GPIO 0
#endif
#ifndef PAD_PWM1_IS_GPIO
#define PAD_PWM1_IS_GPIO PIN_146_IS_GPIO
#endif

#define PIN_147 PAD_PWM2
#define PAD_PWM2 139
#define GPIO_PAD_139 GPIO138
#ifndef PIN_147_IS_GPIO
#define PIN_147_IS_GPIO 0
#endif
#ifndef PAD_PWM2_IS_GPIO
#define PAD_PWM2_IS_GPIO PIN_147_IS_GPIO
#endif

#define PIN_148 PAD_PWM3
#define PAD_PWM3 140
#define GPIO_PAD_140 GPIO139
#ifndef PIN_148_IS_GPIO
#define PIN_148_IS_GPIO 0
#endif
#ifndef PAD_PWM3_IS_GPIO
#define PAD_PWM3_IS_GPIO PIN_148_IS_GPIO
#endif

#define PAD_DDCR_DA 9999
#ifndef PAD_DDCR_DA_IS_GPIO
#define PAD_DDCR_DA_IS_GPIO 0
#endif

#define PAD_DDCR_CK 9999
#ifndef PAD_DDCR_CK_IS_GPIO
#define PAD_DDCR_CK_IS_GPIO 0
#endif

#define PIN_163 PAD_TCON0
#define PAD_TCON0 91
#define GPIO_PAD_91 GPIO90
#ifndef PIN_163_IS_GPIO
#define PIN_163_IS_GPIO 0
#endif
#ifndef PAD_TCON0_IS_GPIO
#define PAD_TCON0_IS_GPIO PIN_163_IS_GPIO
#endif

#define PIN_164 PAD_TCON1
#define PAD_TCON1 92
#define GPIO_PAD_92 GPIO91
#ifndef PIN_164_IS_GPIO
#define PIN_164_IS_GPIO 0
#endif
#ifndef PAD_TCON1_IS_GPIO
#define PAD_TCON1_IS_GPIO PIN_164_IS_GPIO
#endif

#define PIN_165 PAD_TCON2
#define PAD_TCON2 93
#define GPIO_PAD_93 GPIO92
#ifndef PIN_165_IS_GPIO
#define PIN_165_IS_GPIO 0
#endif
#ifndef PAD_TCON2_IS_GPIO
#define PAD_TCON2_IS_GPIO PIN_165_IS_GPIO
#endif

#define PAD_TCON3 9999
#ifndef PAD_TCON3_IS_GPIO
#define PAD_TCON3_IS_GPIO 0
#endif

#define PAD_TCON4 9999
#ifndef PAD_TCON4_IS_GPIO
#define PAD_TCON4_IS_GPIO 0
#endif

#define PAD_TCON5 9999
#ifndef PAD_TCON5_IS_GPIO
#define PAD_TCON5_IS_GPIO 0
#endif

#define PAD_TCON6 9999
#ifndef PAD_TCON6_IS_GPIO
#define PAD_TCON6_IS_GPIO 0
#endif

#define PAD_TCON7 9999
#ifndef PAD_TCON7_IS_GPIO
#define PAD_TCON7_IS_GPIO 0
#endif

#define PAD_TCON8 9999
#ifndef PAD_TCON8_IS_GPIO
#define PAD_TCON8_IS_GPIO 0
#endif

#define PAD_TCON9 9999
#ifndef PAD_TCON9_IS_GPIO
#define PAD_TCON9_IS_GPIO 0
#endif

#define PAD_TCON10 9999
#ifndef PAD_TCON10_IS_GPIO
#define PAD_TCON10_IS_GPIO 0
#endif

#define PAD_TCON11 9999
#ifndef PAD_TCON11_IS_GPIO
#define PAD_TCON11_IS_GPIO 0
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

#define PIN_166 PAD_PCM_A3
#define PAD_PCM_A3 65
#define GPIO_PAD_65 GPIO64
#ifndef PIN_166_IS_GPIO
#define PIN_166_IS_GPIO 0
#endif
#ifndef PAD_PCM_A3_IS_GPIO
#define PAD_PCM_A3_IS_GPIO PIN_166_IS_GPIO
#endif

#define PIN_167 PAD_PCM_A2
#define PAD_PCM_A2 66
#define GPIO_PAD_66 GPIO65
#ifndef PIN_167_IS_GPIO
#define PIN_167_IS_GPIO 0
#endif
#ifndef PAD_PCM_A2_IS_GPIO
#define PAD_PCM_A2_IS_GPIO PIN_167_IS_GPIO
#endif

#define PAD_PCM_REG_N 9999
#ifndef PAD_PCM_REG_N_IS_GPIO
#define PAD_PCM_REG_N_IS_GPIO 0
#endif

#define PIN_168 PAD_PCM_A1
#define PAD_PCM_A1 68
#define GPIO_PAD_68 GPIO67
#ifndef PIN_168_IS_GPIO
#define PIN_168_IS_GPIO 0
#endif
#ifndef PAD_PCM_A1_IS_GPIO
#define PAD_PCM_A1_IS_GPIO PIN_168_IS_GPIO
#endif

#define PIN_169 PAD_PCM_A0
#define PAD_PCM_A0 69
#define GPIO_PAD_69 GPIO68
#ifndef PIN_169_IS_GPIO
#define PIN_169_IS_GPIO 0
#endif
#ifndef PAD_PCM_A0_IS_GPIO
#define PAD_PCM_A0_IS_GPIO PIN_169_IS_GPIO
#endif

#define PAD_PCM_D0 9999
#ifndef PAD_PCM_D0_IS_GPIO
#define PAD_PCM_D0_IS_GPIO 0
#endif

#define PAD_PCM_D1 9999
#ifndef PAD_PCM_D1_IS_GPIO
#define PAD_PCM_D1_IS_GPIO 0
#endif

#define PAD_PCM_D2 9999
#ifndef PAD_PCM_D2_IS_GPIO
#define PAD_PCM_D2_IS_GPIO 0
#endif

#define PAD_PCM_RESET 9999
#ifndef PAD_PCM_RESET_IS_GPIO
#define PAD_PCM_RESET_IS_GPIO 0
#endif

#define PAD_PCM2_RESET 9999
#ifndef PAD_PCM2_RESET_IS_GPIO
#define PAD_PCM2_RESET_IS_GPIO 0
#endif

#define PAD_PCM_CD_N 9999
#ifndef PAD_PCM_CD_N_IS_GPIO
#define PAD_PCM_CD_N_IS_GPIO 0
#endif

#define PAD_PCM2_CD_N 9999
#ifndef PAD_PCM2_CD_N_IS_GPIO
#define PAD_PCM2_CD_N_IS_GPIO 0
#endif

#define PAD_TS1_D0 9999
#ifndef PAD_TS1_D0_IS_GPIO
#define PAD_TS1_D0_IS_GPIO 0
#endif

#define PAD_TS1_VLD 9999
#ifndef PAD_TS1_VLD_IS_GPIO
#define PAD_TS1_VLD_IS_GPIO 0
#endif

#define PAD_TS1_SYNC 9999
#ifndef PAD_TS1_SYNC_IS_GPIO
#define PAD_TS1_SYNC_IS_GPIO 0
#endif

#define PAD_TS1_CLK 9999
#ifndef PAD_TS1_CLK_IS_GPIO
#define PAD_TS1_CLK_IS_GPIO 0
#endif

#define PAD_ET_COL 9999
#ifndef PAD_ET_COL_IS_GPIO
#define PAD_ET_COL_IS_GPIO 0
#endif

#define PAD_ET_TXD1 9999
#ifndef PAD_ET_TXD1_IS_GPIO
#define PAD_ET_TXD1_IS_GPIO 0
#endif

#define PAD_ET_TXD0 9999
#ifndef PAD_ET_TXD0_IS_GPIO
#define PAD_ET_TXD0_IS_GPIO 0
#endif

#define PAD_ET_TX_EN 9999
#ifndef PAD_ET_TX_EN_IS_GPIO
#define PAD_ET_TX_EN_IS_GPIO 0
#endif

#define PAD_ET_TX_CLK 9999
#ifndef PAD_ET_TX_CLK_IS_GPIO
#define PAD_ET_TX_CLK_IS_GPIO 0
#endif

#define PAD_ET_RXD0 9999
#ifndef PAD_ET_RXD0_IS_GPIO
#define PAD_ET_RXD0_IS_GPIO 0
#endif

#define PAD_I2S_IN_WS 9999
#ifndef PAD_I2S_IN_WS_IS_GPIO
#define PAD_I2S_IN_WS_IS_GPIO 0
#endif

#define PAD_I2S_IN_BCK 9999
#ifndef PAD_I2S_IN_BCK_IS_GPIO
#define PAD_I2S_IN_BCK_IS_GPIO 0
#endif

#define PAD_I2S_IN_SD 9999
#ifndef PAD_I2S_IN_SD_IS_GPIO
#define PAD_I2S_IN_SD_IS_GPIO 0
#endif

#define PAD_SPDIF_IN 9999
#ifndef PAD_SPDIF_IN_IS_GPIO
#define PAD_SPDIF_IN_IS_GPIO 0
#endif

#define PAD_I2S_OUT_MCK 9999
#ifndef PAD_I2S_OUT_MCK_IS_GPIO
#define PAD_I2S_OUT_MCK_IS_GPIO 0
#endif

#define PAD_I2S_OUT_WS 9999
#ifndef PAD_I2S_OUT_WS_IS_GPIO
#define PAD_I2S_OUT_WS_IS_GPIO 0
#endif

#define PAD_ET_CRS 9999
#ifndef PAD_ET_CRS_IS_GPIO
#define PAD_ET_CRS_IS_GPIO 0
#endif

#define PAD_ET_TXD3 9999
#ifndef PAD_ET_TXD3_IS_GPIO
#define PAD_ET_TXD3_IS_GPIO 0
#endif

#define PAD_I2S_OUT_BCK 9999
#ifndef PAD_I2S_OUT_BCK_IS_GPIO
#define PAD_I2S_OUT_BCK_IS_GPIO 0
#endif

#define PAD_ET_TXD2 9999
#ifndef PAD_ET_TXD2_IS_GPIO
#define PAD_ET_TXD2_IS_GPIO 0
#endif

#define PAD_I2S_OUT_SD 9999
#ifndef PAD_I2S_OUT_SD_IS_GPIO
#define PAD_I2S_OUT_SD_IS_GPIO 0
#endif

#define PAD_ET_TXER 9999
#ifndef PAD_ET_TXER_IS_GPIO
#define PAD_ET_TXER_IS_GPIO 0
#endif

#define PIN_279 PAD_SPDIF_OUT
#define PAD_SPDIF_OUT 130
#define GPIO_PAD_130 GPIO129
#ifndef PIN_279_IS_GPIO
#define PIN_279_IS_GPIO 0
#endif
#ifndef PAD_SPDIF_OUT_IS_GPIO
#define PAD_SPDIF_OUT_IS_GPIO PIN_279_IS_GPIO
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

#define PAD_LHSYNC2 9999
#ifndef PAD_LHSYNC2_IS_GPIO
#define PAD_LHSYNC2_IS_GPIO 0
#endif

#define PAD_LVSYNC2 9999
#ifndef PAD_LVSYNC2_IS_GPIO
#define PAD_LVSYNC2_IS_GPIO 0
#endif

#define PAD_ET_RXD1 9999
#ifndef PAD_ET_RXD1_IS_GPIO
#define PAD_ET_RXD1_IS_GPIO 0
#endif

#define PAD_GPIO12 9999
#ifndef PAD_GPIO12_IS_GPIO
#define PAD_GPIO12_IS_GPIO 0
#endif

#define PAD_ET_RXD2 9999
#ifndef PAD_ET_RXD2_IS_GPIO
#define PAD_ET_RXD2_IS_GPIO 0
#endif

#define PAD_GPIO13 9999
#ifndef PAD_GPIO13_IS_GPIO
#define PAD_GPIO13_IS_GPIO 0
#endif

#define PAD_ET_RXD3 9999
#ifndef PAD_ET_RXD3_IS_GPIO
#define PAD_ET_RXD3_IS_GPIO 0
#endif

#define PAD_GPIO14 9999
#ifndef PAD_GPIO14_IS_GPIO
#define PAD_GPIO14_IS_GPIO 0
#endif

#define PAD_ET_MDC 9999
#ifndef PAD_ET_MDC_IS_GPIO
#define PAD_ET_MDC_IS_GPIO 0
#endif

#define PAD_GPIO15 9999
#ifndef PAD_GPIO15_IS_GPIO
#define PAD_GPIO15_IS_GPIO 0
#endif

#define PAD_ET_MDIO 9999
#ifndef PAD_ET_MDIO_IS_GPIO
#define PAD_ET_MDIO_IS_GPIO 0
#endif

#define PIN_283 PAD_SAR0
#define PAD_SAR0 151
#define GPIO_PAD_151 GPIO150
#ifndef PIN_283_IS_GPIO
#define PIN_283_IS_GPIO 0
#endif
#ifndef PAD_SAR0_IS_GPIO
#define PAD_SAR0_IS_GPIO PIN_283_IS_GPIO
#endif

#define PIN_284 PAD_SAR1
#define PAD_SAR1 152
#define GPIO_PAD_152 GPIO151
#ifndef PIN_284_IS_GPIO
#define PIN_284_IS_GPIO 0
#endif
#ifndef PAD_SAR1_IS_GPIO
#define PAD_SAR1_IS_GPIO PIN_284_IS_GPIO
#endif

#define PIN_285 PAD_SAR2
#define PAD_SAR2 153
#define GPIO_PAD_153 GPIO152
#ifndef PIN_285_IS_GPIO
#define PIN_285_IS_GPIO 0
#endif
#ifndef PAD_SAR2_IS_GPIO
#define PAD_SAR2_IS_GPIO PIN_285_IS_GPIO
#endif

#define PIN_286 PAD_SAR3
#define PAD_SAR3 154
#define GPIO_PAD_154 GPIO153
#ifndef PIN_286_IS_GPIO
#define PIN_286_IS_GPIO 0
#endif
#ifndef PAD_SAR3_IS_GPIO
#define PAD_SAR3_IS_GPIO PIN_286_IS_GPIO
#endif

#define PAD_GPIO_PM0 9999
#ifndef PAD_GPIO_PM0_IS_GPIO
#define PAD_GPIO_PM0_IS_GPIO 0
#endif

#define PAD_GPIO_PM1 9999
#ifndef PAD_GPIO_PM1_IS_GPIO
#define PAD_GPIO_PM1_IS_GPIO 0
#endif

#define PAD_GPIO_PM2 9999
#ifndef PAD_GPIO_PM2_IS_GPIO
#define PAD_GPIO_PM2_IS_GPIO 0
#endif

#define PAD_GPIO_PM3 9999
#ifndef PAD_GPIO_PM3_IS_GPIO
#define PAD_GPIO_PM3_IS_GPIO 0
#endif

#define PIN_293 PAD_GPIO_PM4
#define PAD_GPIO_PM4 159
#define GPIO_PAD_159 GPIO158
#ifndef PIN_293_IS_GPIO
#define PIN_293_IS_GPIO 0
#endif
#ifndef PAD_GPIO_PM4_IS_GPIO
#define PAD_GPIO_PM4_IS_GPIO PIN_293_IS_GPIO
#endif

#define PIN_294 PAD_GPIO_PM5
#define PAD_GPIO_PM5 160
#define GPIO_PAD_160 GPIO159
#ifndef PIN_294_IS_GPIO
#define PIN_294_IS_GPIO 0
#endif
#ifndef PAD_GPIO_PM5_IS_GPIO
#define PAD_GPIO_PM5_IS_GPIO PIN_294_IS_GPIO
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

#define PIN_296 PAD_GPIO_PM7
#define PAD_GPIO_PM7 166
#define GPIO_PAD_166 GPIO165
#ifndef PIN_296_IS_GPIO
#define PIN_296_IS_GPIO 0
#endif
#ifndef PAD_GPIO_PM7_IS_GPIO
#define PAD_GPIO_PM7_IS_GPIO PIN_296_IS_GPIO
#endif

#define PIN_1 PAD_GPIO_PM8
#define PAD_GPIO_PM8 167
#define GPIO_PAD_167 GPIO166
#ifndef PIN_1_IS_GPIO
#define PIN_1_IS_GPIO 0
#endif
#ifndef PAD_GPIO_PM8_IS_GPIO
#define PAD_GPIO_PM8_IS_GPIO PIN_1_IS_GPIO
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

#endif /* _MSD7M36_H_*/
