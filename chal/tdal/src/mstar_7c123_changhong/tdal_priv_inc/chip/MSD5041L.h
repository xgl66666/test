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
#ifndef _MSD5041L_H_
#define _MSD5041L_H_

#define PAD_NC 9999
#define PAD_GND 0

#define PAD_HSYNC_OUT 9999
#ifndef PAD_HSYNC_OUT_IS_GPIO
#define PAD_HSYNC_OUT_IS_GPIO 0
#endif

#define PAD_VSYNC_OUT 9999
#ifndef PAD_VSYNC_OUT_IS_GPIO
#define PAD_VSYNC_OUT_IS_GPIO 0
#endif

#define PAD_SM1_RST 9999
#ifndef PAD_SM1_RST_IS_GPIO
#define PAD_SM1_RST_IS_GPIO 0
#endif

#define PIN_256 PAD_SM0_IO
#define PAD_SM0_IO 24
#define GPIO_PAD_24 GPIO23
#ifndef PIN_256_IS_GPIO
#define PIN_256_IS_GPIO 0
#endif
#ifndef PAD_SM0_IO_IS_GPIO
#define PAD_SM0_IO_IS_GPIO PIN_256_IS_GPIO
#endif

#define PAD_SM1_CD 9999
#ifndef PAD_SM1_CD_IS_GPIO
#define PAD_SM1_CD_IS_GPIO 0
#endif

#define PIN_1 PAD_SM0_CLK
#define PAD_SM0_CLK 25
#define GPIO_PAD_25 GPIO24
#ifndef PIN_1_IS_GPIO
#define PIN_1_IS_GPIO 0
#endif
#ifndef PAD_SM0_CLK_IS_GPIO
#define PAD_SM0_CLK_IS_GPIO PIN_1_IS_GPIO
#endif

#define PAD_SM1_VCC 9999
#ifndef PAD_SM1_VCC_IS_GPIO
#define PAD_SM1_VCC_IS_GPIO 0
#endif

#define PAD_SM0_RST 9999
#ifndef PAD_SM0_RST_IS_GPIO
#define PAD_SM0_RST_IS_GPIO 0
#endif

#define PAD_SM1_GPIO0 9999
#ifndef PAD_SM1_GPIO0_IS_GPIO
#define PAD_SM1_GPIO0_IS_GPIO 0
#endif

#define PIN_2 PAD_SM0_CD
#define PAD_SM0_CD 27
#define GPIO_PAD_27 GPIO26
#ifndef PIN_2_IS_GPIO
#define PIN_2_IS_GPIO 0
#endif
#ifndef PAD_SM0_CD_IS_GPIO
#define PAD_SM0_CD_IS_GPIO PIN_2_IS_GPIO
#endif

#define PAD_SM1_GPIO1 9999
#ifndef PAD_SM1_GPIO1_IS_GPIO
#define PAD_SM1_GPIO1_IS_GPIO 0
#endif

#define PAD_SM0_VCC 9999
#ifndef PAD_SM0_VCC_IS_GPIO
#define PAD_SM0_VCC_IS_GPIO 0
#endif

#define PAD_SM0_GPIO0 9999
#ifndef PAD_SM0_GPIO0_IS_GPIO
#define PAD_SM0_GPIO0_IS_GPIO 0
#endif

#define PIN_4 PAD_TS1_CLK
#define PAD_TS1_CLK 53
#define GPIO_PAD_53 GPIO52
#ifndef PIN_4_IS_GPIO
#define PIN_4_IS_GPIO 0
#endif
#ifndef PAD_TS1_CLK_IS_GPIO
#define PAD_TS1_CLK_IS_GPIO PIN_4_IS_GPIO
#endif

#define PAD_SM0_GPIO1 9999
#ifndef PAD_SM0_GPIO1_IS_GPIO
#define PAD_SM0_GPIO1_IS_GPIO 0
#endif

#define PIN_5 PAD_TS1_SYNC
#define PAD_TS1_SYNC 54
#define GPIO_PAD_54 GPIO53
#ifndef PIN_5_IS_GPIO
#define PIN_5_IS_GPIO 0
#endif
#ifndef PAD_TS1_SYNC_IS_GPIO
#define PAD_TS1_SYNC_IS_GPIO PIN_5_IS_GPIO
#endif

#define PAD_SM1_IO 9999
#ifndef PAD_SM1_IO_IS_GPIO
#define PAD_SM1_IO_IS_GPIO 0
#endif

#define PIN_6 PAD_TS1_VLD
#define PAD_TS1_VLD 55
#define GPIO_PAD_55 GPIO54
#ifndef PIN_6_IS_GPIO
#define PIN_6_IS_GPIO 0
#endif
#ifndef PAD_TS1_VLD_IS_GPIO
#define PAD_TS1_VLD_IS_GPIO PIN_6_IS_GPIO
#endif

#define PAD_SM1_CLK 9999
#ifndef PAD_SM1_CLK_IS_GPIO
#define PAD_SM1_CLK_IS_GPIO 0
#endif

#define PIN_7 PAD_TS1_D7
#define PAD_TS1_D7 56
#define GPIO_PAD_56 GPIO55
#ifndef PIN_7_IS_GPIO
#define PIN_7_IS_GPIO 0
#endif
#ifndef PAD_TS1_D7_IS_GPIO
#define PAD_TS1_D7_IS_GPIO PIN_7_IS_GPIO
#endif

#define PAD_TS1_D2 9999
#ifndef PAD_TS1_D2_IS_GPIO
#define PAD_TS1_D2_IS_GPIO 0
#endif

#define PIN_10 PAD_TS1_D6
#define PAD_TS1_D6 57
#define GPIO_PAD_57 GPIO56
#ifndef PIN_10_IS_GPIO
#define PIN_10_IS_GPIO 0
#endif
#ifndef PAD_TS1_D6_IS_GPIO
#define PAD_TS1_D6_IS_GPIO PIN_10_IS_GPIO
#endif

#define PAD_TS2_CLK 9999
#ifndef PAD_TS2_CLK_IS_GPIO
#define PAD_TS2_CLK_IS_GPIO 0
#endif

#define PIN_11 PAD_TS1_D5
#define PAD_TS1_D5 58
#define GPIO_PAD_58 GPIO57
#ifndef PIN_11_IS_GPIO
#define PIN_11_IS_GPIO 0
#endif
#ifndef PAD_TS1_D5_IS_GPIO
#define PAD_TS1_D5_IS_GPIO PIN_11_IS_GPIO
#endif

#define PAD_TS2_SYNC 9999
#ifndef PAD_TS2_SYNC_IS_GPIO
#define PAD_TS2_SYNC_IS_GPIO 0
#endif

#define PIN_12 PAD_TS1_D4
#define PAD_TS1_D4 59
#define GPIO_PAD_59 GPIO58
#ifndef PIN_12_IS_GPIO
#define PIN_12_IS_GPIO 0
#endif
#ifndef PAD_TS1_D4_IS_GPIO
#define PAD_TS1_D4_IS_GPIO PIN_12_IS_GPIO
#endif

#define PAD_TS2_VLD 9999
#ifndef PAD_TS2_VLD_IS_GPIO
#define PAD_TS2_VLD_IS_GPIO 0
#endif

#define PAD_TS1_D3 9999
#ifndef PAD_TS1_D3_IS_GPIO
#define PAD_TS1_D3_IS_GPIO 0
#endif

#define PAD_TS2_D0 9999
#ifndef PAD_TS2_D0_IS_GPIO
#define PAD_TS2_D0_IS_GPIO 0
#endif

#define PAD_TS1_D1 9999
#ifndef PAD_TS1_D1_IS_GPIO
#define PAD_TS1_D1_IS_GPIO 0
#endif

#define PAD_I2CM0_SDA 9999
#ifndef PAD_I2CM0_SDA_IS_GPIO
#define PAD_I2CM0_SDA_IS_GPIO 0
#endif

#define PAD_TS1_D0 9999
#ifndef PAD_TS1_D0_IS_GPIO
#define PAD_TS1_D0_IS_GPIO 0
#endif

#define PAD_I2CM0_SCL 9999
#ifndef PAD_I2CM0_SCL_IS_GPIO
#define PAD_I2CM0_SCL_IS_GPIO 0
#endif

#define PIN_123 PAD_S_GPIO0
#define PAD_S_GPIO0 163
#define GPIO_PAD_163 GPIO162
#ifndef PIN_123_IS_GPIO
#define PIN_123_IS_GPIO 0
#endif
#ifndef PAD_S_GPIO0_IS_GPIO
#define PAD_S_GPIO0_IS_GPIO PIN_123_IS_GPIO
#endif

#define PIN_124 PAD_S_GPIO2
#define PAD_S_GPIO2 164
#define GPIO_PAD_164 GPIO163
#ifndef PIN_124_IS_GPIO
#define PIN_124_IS_GPIO 0
#endif
#ifndef PAD_S_GPIO2_IS_GPIO
#define PAD_S_GPIO2_IS_GPIO PIN_124_IS_GPIO
#endif

#define PIN_125 PAD_S_GPIO4
#define PAD_S_GPIO4 165
#define GPIO_PAD_165 GPIO164
#ifndef PIN_125_IS_GPIO
#define PIN_125_IS_GPIO 0
#endif
#ifndef PAD_S_GPIO4_IS_GPIO
#define PAD_S_GPIO4_IS_GPIO PIN_125_IS_GPIO
#endif

#define PIN_128 PAD_CI_A14
#define PAD_CI_A14 69
#define GPIO_PAD_69 GPIO68
#ifndef PIN_128_IS_GPIO
#define PIN_128_IS_GPIO 0
#endif
#ifndef PAD_CI_A14_IS_GPIO
#define PAD_CI_A14_IS_GPIO PIN_128_IS_GPIO
#endif

#define PAD_I2CM1_SCL 9999
#ifndef PAD_I2CM1_SCL_IS_GPIO
#define PAD_I2CM1_SCL_IS_GPIO 0
#endif

#define PIN_129 PAD_CI_A13
#define PAD_CI_A13 70
#define GPIO_PAD_70 GPIO69
#ifndef PIN_129_IS_GPIO
#define PIN_129_IS_GPIO 0
#endif
#ifndef PAD_CI_A13_IS_GPIO
#define PAD_CI_A13_IS_GPIO PIN_129_IS_GPIO
#endif

#define PAD_I2CM1_SDA 9999
#ifndef PAD_I2CM1_SDA_IS_GPIO
#define PAD_I2CM1_SDA_IS_GPIO 0
#endif

#define PIN_130 PAD_CI_A12
#define PAD_CI_A12 71
#define GPIO_PAD_71 GPIO70
#ifndef PIN_130_IS_GPIO
#define PIN_130_IS_GPIO 0
#endif
#ifndef PAD_CI_A12_IS_GPIO
#define PAD_CI_A12_IS_GPIO PIN_130_IS_GPIO
#endif

#define PIN_131 PAD_CI_A11
#define PAD_CI_A11 72
#define GPIO_PAD_72 GPIO71
#ifndef PIN_131_IS_GPIO
#define PIN_131_IS_GPIO 0
#endif
#ifndef PAD_CI_A11_IS_GPIO
#define PAD_CI_A11_IS_GPIO PIN_131_IS_GPIO
#endif

#define PIN_132 PAD_CI_A10
#define PAD_CI_A10 73
#define GPIO_PAD_73 GPIO72
#ifndef PIN_132_IS_GPIO
#define PIN_132_IS_GPIO 0
#endif
#ifndef PAD_CI_A10_IS_GPIO
#define PAD_CI_A10_IS_GPIO PIN_132_IS_GPIO
#endif

#define PAD_PF_A25 9999
#ifndef PAD_PF_A25_IS_GPIO
#define PAD_PF_A25_IS_GPIO 0
#endif

#define PIN_133 PAD_CI_A9
#define PAD_CI_A9 74
#define GPIO_PAD_74 GPIO73
#ifndef PIN_133_IS_GPIO
#define PIN_133_IS_GPIO 0
#endif
#ifndef PAD_CI_A9_IS_GPIO
#define PAD_CI_A9_IS_GPIO PIN_133_IS_GPIO
#endif

#define PAD_PF_A24 9999
#ifndef PAD_PF_A24_IS_GPIO
#define PAD_PF_A24_IS_GPIO 0
#endif

#define PIN_134 PAD_CI_A8
#define PAD_CI_A8 75
#define GPIO_PAD_75 GPIO74
#ifndef PIN_134_IS_GPIO
#define PIN_134_IS_GPIO 0
#endif
#ifndef PAD_CI_A8_IS_GPIO
#define PAD_CI_A8_IS_GPIO PIN_134_IS_GPIO
#endif

#define PAD_PF_A23 9999
#ifndef PAD_PF_A23_IS_GPIO
#define PAD_PF_A23_IS_GPIO 0
#endif

#define PIN_135 PAD_CI_A7
#define PAD_CI_A7 76
#define GPIO_PAD_76 GPIO75
#ifndef PIN_135_IS_GPIO
#define PIN_135_IS_GPIO 0
#endif
#ifndef PAD_CI_A7_IS_GPIO
#define PAD_CI_A7_IS_GPIO PIN_135_IS_GPIO
#endif

#define PAD_PF_A22 9999
#ifndef PAD_PF_A22_IS_GPIO
#define PAD_PF_A22_IS_GPIO 0
#endif

#define PIN_136 PAD_CI_A6
#define PAD_CI_A6 77
#define GPIO_PAD_77 GPIO76
#ifndef PIN_136_IS_GPIO
#define PIN_136_IS_GPIO 0
#endif
#ifndef PAD_CI_A6_IS_GPIO
#define PAD_CI_A6_IS_GPIO PIN_136_IS_GPIO
#endif

#define PAD_PF_A21 9999
#ifndef PAD_PF_A21_IS_GPIO
#define PAD_PF_A21_IS_GPIO 0
#endif

#define PIN_137 PAD_CI_A5
#define PAD_CI_A5 78
#define GPIO_PAD_78 GPIO77
#ifndef PIN_137_IS_GPIO
#define PIN_137_IS_GPIO 0
#endif
#ifndef PAD_CI_A5_IS_GPIO
#define PAD_CI_A5_IS_GPIO PIN_137_IS_GPIO
#endif

#define PAD_PF_A20 9999
#ifndef PAD_PF_A20_IS_GPIO
#define PAD_PF_A20_IS_GPIO 0
#endif

#define PIN_138 PAD_CI_A4
#define PAD_CI_A4 79
#define GPIO_PAD_79 GPIO78
#ifndef PIN_138_IS_GPIO
#define PIN_138_IS_GPIO 0
#endif
#ifndef PAD_CI_A4_IS_GPIO
#define PAD_CI_A4_IS_GPIO PIN_138_IS_GPIO
#endif

#define PAD_PF_A19 9999
#ifndef PAD_PF_A19_IS_GPIO
#define PAD_PF_A19_IS_GPIO 0
#endif

#define PIN_139 PAD_CI_A3
#define PAD_CI_A3 80
#define GPIO_PAD_80 GPIO79
#ifndef PIN_139_IS_GPIO
#define PIN_139_IS_GPIO 0
#endif
#ifndef PAD_CI_A3_IS_GPIO
#define PAD_CI_A3_IS_GPIO PIN_139_IS_GPIO
#endif

#define PAD_PF_A18 9999
#ifndef PAD_PF_A18_IS_GPIO
#define PAD_PF_A18_IS_GPIO 0
#endif

#define PIN_142 PAD_CI_A2
#define PAD_CI_A2 81
#define GPIO_PAD_81 GPIO80
#ifndef PIN_142_IS_GPIO
#define PIN_142_IS_GPIO 0
#endif
#ifndef PAD_CI_A2_IS_GPIO
#define PAD_CI_A2_IS_GPIO PIN_142_IS_GPIO
#endif

#define PAD_PF_A17 9999
#ifndef PAD_PF_A17_IS_GPIO
#define PAD_PF_A17_IS_GPIO 0
#endif

#define PIN_143 PAD_CI_A1
#define PAD_CI_A1 82
#define GPIO_PAD_82 GPIO81
#ifndef PIN_143_IS_GPIO
#define PIN_143_IS_GPIO 0
#endif
#ifndef PAD_CI_A1_IS_GPIO
#define PAD_CI_A1_IS_GPIO PIN_143_IS_GPIO
#endif

#define PAD_PF_A16 9999
#ifndef PAD_PF_A16_IS_GPIO
#define PAD_PF_A16_IS_GPIO 0
#endif

#define PIN_144 PAD_CI_A0
#define PAD_CI_A0 83
#define GPIO_PAD_83 GPIO82
#ifndef PIN_144_IS_GPIO
#define PIN_144_IS_GPIO 0
#endif
#ifndef PAD_CI_A0_IS_GPIO
#define PAD_CI_A0_IS_GPIO PIN_144_IS_GPIO
#endif

#define PAD_PF_A15 9999
#ifndef PAD_PF_A15_IS_GPIO
#define PAD_PF_A15_IS_GPIO 0
#endif

#define PIN_145 PAD_CI_D7
#define PAD_CI_D7 84
#define GPIO_PAD_84 GPIO83
#ifndef PIN_145_IS_GPIO
#define PIN_145_IS_GPIO 0
#endif
#ifndef PAD_CI_D7_IS_GPIO
#define PAD_CI_D7_IS_GPIO PIN_145_IS_GPIO
#endif

#define PAD_PF_A14 9999
#ifndef PAD_PF_A14_IS_GPIO
#define PAD_PF_A14_IS_GPIO 0
#endif

#define PIN_146 PAD_CI_D6
#define PAD_CI_D6 85
#define GPIO_PAD_85 GPIO84
#ifndef PIN_146_IS_GPIO
#define PIN_146_IS_GPIO 0
#endif
#ifndef PAD_CI_D6_IS_GPIO
#define PAD_CI_D6_IS_GPIO PIN_146_IS_GPIO
#endif

#define PAD_PF_A13 9999
#ifndef PAD_PF_A13_IS_GPIO
#define PAD_PF_A13_IS_GPIO 0
#endif

#define PIN_147 PAD_CI_D5
#define PAD_CI_D5 86
#define GPIO_PAD_86 GPIO85
#ifndef PIN_147_IS_GPIO
#define PIN_147_IS_GPIO 0
#endif
#ifndef PAD_CI_D5_IS_GPIO
#define PAD_CI_D5_IS_GPIO PIN_147_IS_GPIO
#endif

#define PAD_PF_A12 9999
#ifndef PAD_PF_A12_IS_GPIO
#define PAD_PF_A12_IS_GPIO 0
#endif

#define PIN_148 PAD_CI_D4
#define PAD_CI_D4 87
#define GPIO_PAD_87 GPIO86
#ifndef PIN_148_IS_GPIO
#define PIN_148_IS_GPIO 0
#endif
#ifndef PAD_CI_D4_IS_GPIO
#define PAD_CI_D4_IS_GPIO PIN_148_IS_GPIO
#endif

#define PAD_PF_A11 9999
#ifndef PAD_PF_A11_IS_GPIO
#define PAD_PF_A11_IS_GPIO 0
#endif

#define PIN_149 PAD_CI_D3
#define PAD_CI_D3 88
#define GPIO_PAD_88 GPIO87
#ifndef PIN_149_IS_GPIO
#define PIN_149_IS_GPIO 0
#endif
#ifndef PAD_CI_D3_IS_GPIO
#define PAD_CI_D3_IS_GPIO PIN_149_IS_GPIO
#endif

#define PAD_PF_A10 9999
#ifndef PAD_PF_A10_IS_GPIO
#define PAD_PF_A10_IS_GPIO 0
#endif

#define PIN_150 PAD_CI_D2
#define PAD_CI_D2 89
#define GPIO_PAD_89 GPIO88
#ifndef PIN_150_IS_GPIO
#define PIN_150_IS_GPIO 0
#endif
#ifndef PAD_CI_D2_IS_GPIO
#define PAD_CI_D2_IS_GPIO PIN_150_IS_GPIO
#endif

#define PAD_PF_A9 9999
#ifndef PAD_PF_A9_IS_GPIO
#define PAD_PF_A9_IS_GPIO 0
#endif

#define PIN_151 PAD_CI_D1
#define PAD_CI_D1 90
#define GPIO_PAD_90 GPIO89
#ifndef PIN_151_IS_GPIO
#define PIN_151_IS_GPIO 0
#endif
#ifndef PAD_CI_D1_IS_GPIO
#define PAD_CI_D1_IS_GPIO PIN_151_IS_GPIO
#endif

#define PAD_PF_A8 9999
#ifndef PAD_PF_A8_IS_GPIO
#define PAD_PF_A8_IS_GPIO 0
#endif

#define PIN_152 PAD_CI_D0
#define PAD_CI_D0 91
#define GPIO_PAD_91 GPIO90
#ifndef PIN_152_IS_GPIO
#define PIN_152_IS_GPIO 0
#endif
#ifndef PAD_CI_D0_IS_GPIO
#define PAD_CI_D0_IS_GPIO PIN_152_IS_GPIO
#endif

#define PAD_PF_A7 9999
#ifndef PAD_PF_A7_IS_GPIO
#define PAD_PF_A7_IS_GPIO 0
#endif

#define PIN_153 PAD_CI_IORDZ
#define PAD_CI_IORDZ 92
#define GPIO_PAD_92 GPIO91
#ifndef PIN_153_IS_GPIO
#define PIN_153_IS_GPIO 0
#endif
#ifndef PAD_CI_IORDZ_IS_GPIO
#define PAD_CI_IORDZ_IS_GPIO PIN_153_IS_GPIO
#endif

#define PAD_PF_A6 9999
#ifndef PAD_PF_A6_IS_GPIO
#define PAD_PF_A6_IS_GPIO 0
#endif

#define PIN_154 PAD_CI_IOWRZ
#define PAD_CI_IOWRZ 93
#define GPIO_PAD_93 GPIO92
#ifndef PIN_154_IS_GPIO
#define PIN_154_IS_GPIO 0
#endif
#ifndef PAD_CI_IOWRZ_IS_GPIO
#define PAD_CI_IOWRZ_IS_GPIO PIN_154_IS_GPIO
#endif

#define PAD_PF_A5 9999
#ifndef PAD_PF_A5_IS_GPIO
#define PAD_PF_A5_IS_GPIO 0
#endif

#define PIN_155 PAD_CI_OEZ
#define PAD_CI_OEZ 94
#define GPIO_PAD_94 GPIO93
#ifndef PIN_155_IS_GPIO
#define PIN_155_IS_GPIO 0
#endif
#ifndef PAD_CI_OEZ_IS_GPIO
#define PAD_CI_OEZ_IS_GPIO PIN_155_IS_GPIO
#endif

#define PAD_PF_A4 9999
#ifndef PAD_PF_A4_IS_GPIO
#define PAD_PF_A4_IS_GPIO 0
#endif

#define PIN_158 PAD_CI_WEZ
#define PAD_CI_WEZ 95
#define GPIO_PAD_95 GPIO94
#ifndef PIN_158_IS_GPIO
#define PIN_158_IS_GPIO 0
#endif
#ifndef PAD_CI_WEZ_IS_GPIO
#define PAD_CI_WEZ_IS_GPIO PIN_158_IS_GPIO
#endif

#define PAD_PF_A3 9999
#ifndef PAD_PF_A3_IS_GPIO
#define PAD_PF_A3_IS_GPIO 0
#endif

#define PIN_159 PAD_CI_REGZ
#define PAD_CI_REGZ 96
#define GPIO_PAD_96 GPIO95
#ifndef PIN_159_IS_GPIO
#define PIN_159_IS_GPIO 0
#endif
#ifndef PAD_CI_REGZ_IS_GPIO
#define PAD_CI_REGZ_IS_GPIO PIN_159_IS_GPIO
#endif

#define PAD_PF_A2 9999
#ifndef PAD_PF_A2_IS_GPIO
#define PAD_PF_A2_IS_GPIO 0
#endif

#define PIN_160 PAD_CI_CEZ
#define PAD_CI_CEZ 97
#define GPIO_PAD_97 GPIO96
#ifndef PIN_160_IS_GPIO
#define PIN_160_IS_GPIO 0
#endif
#ifndef PAD_CI_CEZ_IS_GPIO
#define PAD_CI_CEZ_IS_GPIO PIN_160_IS_GPIO
#endif

#define PAD_PF_A1 9999
#ifndef PAD_PF_A1_IS_GPIO
#define PAD_PF_A1_IS_GPIO 0
#endif

#define PIN_161 PAD_CI_IRQAZ
#define PAD_CI_IRQAZ 98
#define GPIO_PAD_98 GPIO97
#ifndef PIN_161_IS_GPIO
#define PIN_161_IS_GPIO 0
#endif
#ifndef PAD_CI_IRQAZ_IS_GPIO
#define PAD_CI_IRQAZ_IS_GPIO PIN_161_IS_GPIO
#endif

#define PAD_PF_A0 9999
#ifndef PAD_PF_A0_IS_GPIO
#define PAD_PF_A0_IS_GPIO 0
#endif

#define PIN_162 PAD_CI_WAITZ
#define PAD_CI_WAITZ 99
#define GPIO_PAD_99 GPIO98
#ifndef PIN_162_IS_GPIO
#define PIN_162_IS_GPIO 0
#endif
#ifndef PAD_CI_WAITZ_IS_GPIO
#define PAD_CI_WAITZ_IS_GPIO PIN_162_IS_GPIO
#endif

#define PAD_PF_CE0Z 9999
#ifndef PAD_PF_CE0Z_IS_GPIO
#define PAD_PF_CE0Z_IS_GPIO 0
#endif

#define PIN_163 PAD_CI_RST
#define PAD_CI_RST 100
#define GPIO_PAD_100 GPIO99
#ifndef PIN_163_IS_GPIO
#define PIN_163_IS_GPIO 0
#endif
#ifndef PAD_CI_RST_IS_GPIO
#define PAD_CI_RST_IS_GPIO PIN_163_IS_GPIO
#endif

#define PAD_PF_OEZ 9999
#ifndef PAD_PF_OEZ_IS_GPIO
#define PAD_PF_OEZ_IS_GPIO 0
#endif

#define PIN_164 PAD_CI_CDZ
#define PAD_CI_CDZ 101
#define GPIO_PAD_101 GPIO100
#ifndef PIN_164_IS_GPIO
#define PIN_164_IS_GPIO 0
#endif
#ifndef PAD_CI_CDZ_IS_GPIO
#define PAD_CI_CDZ_IS_GPIO PIN_164_IS_GPIO
#endif

#define PAD_PF_WEZ 9999
#ifndef PAD_PF_WEZ_IS_GPIO
#define PAD_PF_WEZ_IS_GPIO 0
#endif

#define PIN_165 PAD_DM_GPIO0
#define PAD_DM_GPIO0 161
#define GPIO_PAD_161 GPIO160
#ifndef PIN_165_IS_GPIO
#define PIN_165_IS_GPIO 0
#endif
#ifndef PAD_DM_GPIO0_IS_GPIO
#define PAD_DM_GPIO0_IS_GPIO PIN_165_IS_GPIO
#endif

#define PAD_DM_GPIO1 9999
#ifndef PAD_DM_GPIO1_IS_GPIO
#define PAD_DM_GPIO1_IS_GPIO 0
#endif

#define PIN_167 PAD_TS0_CLK
#define PAD_TS0_CLK 42
#define GPIO_PAD_42 GPIO41
#ifndef PIN_167_IS_GPIO
#define PIN_167_IS_GPIO 0
#endif
#ifndef PAD_TS0_CLK_IS_GPIO
#define PAD_TS0_CLK_IS_GPIO PIN_167_IS_GPIO
#endif

#define PIN_168 PAD_TS0_SYNC
#define PAD_TS0_SYNC 44
#define GPIO_PAD_44 GPIO43
#ifndef PIN_168_IS_GPIO
#define PIN_168_IS_GPIO 0
#endif
#ifndef PAD_TS0_SYNC_IS_GPIO
#define PAD_TS0_SYNC_IS_GPIO PIN_168_IS_GPIO
#endif

#define PIN_169 PAD_TS0_VLD
#define PAD_TS0_VLD 43
#define GPIO_PAD_43 GPIO42
#ifndef PIN_169_IS_GPIO
#define PIN_169_IS_GPIO 0
#endif
#ifndef PAD_TS0_VLD_IS_GPIO
#define PAD_TS0_VLD_IS_GPIO PIN_169_IS_GPIO
#endif

#define PIN_170 PAD_TS0_D7
#define PAD_TS0_D7 45
#define GPIO_PAD_45 GPIO44
#ifndef PIN_170_IS_GPIO
#define PIN_170_IS_GPIO 0
#endif
#ifndef PAD_TS0_D7_IS_GPIO
#define PAD_TS0_D7_IS_GPIO PIN_170_IS_GPIO
#endif

#define PIN_171 PAD_TS0_D6
#define PAD_TS0_D6 46
#define GPIO_PAD_46 GPIO45
#ifndef PIN_171_IS_GPIO
#define PIN_171_IS_GPIO 0
#endif
#ifndef PAD_TS0_D6_IS_GPIO
#define PAD_TS0_D6_IS_GPIO PIN_171_IS_GPIO
#endif

#define PIN_172 PAD_TS0_D5
#define PAD_TS0_D5 47
#define GPIO_PAD_47 GPIO46
#ifndef PIN_172_IS_GPIO
#define PIN_172_IS_GPIO 0
#endif
#ifndef PAD_TS0_D5_IS_GPIO
#define PAD_TS0_D5_IS_GPIO PIN_172_IS_GPIO
#endif

#define PIN_173 PAD_TS0_D4
#define PAD_TS0_D4 48
#define GPIO_PAD_48 GPIO47
#ifndef PIN_173_IS_GPIO
#define PIN_173_IS_GPIO 0
#endif
#ifndef PAD_TS0_D4_IS_GPIO
#define PAD_TS0_D4_IS_GPIO PIN_173_IS_GPIO
#endif

#define PIN_174 PAD_TS0_D3
#define PAD_TS0_D3 49
#define GPIO_PAD_49 GPIO48
#ifndef PIN_174_IS_GPIO
#define PIN_174_IS_GPIO 0
#endif
#ifndef PAD_TS0_D3_IS_GPIO
#define PAD_TS0_D3_IS_GPIO PIN_174_IS_GPIO
#endif

#define PIN_175 PAD_TS0_D2
#define PAD_TS0_D2 50
#define GPIO_PAD_50 GPIO49
#ifndef PIN_175_IS_GPIO
#define PIN_175_IS_GPIO 0
#endif
#ifndef PAD_TS0_D2_IS_GPIO
#define PAD_TS0_D2_IS_GPIO PIN_175_IS_GPIO
#endif

#define PIN_176 PAD_TS0_D1
#define PAD_TS0_D1 51
#define GPIO_PAD_51 GPIO50
#ifndef PIN_176_IS_GPIO
#define PIN_176_IS_GPIO 0
#endif
#ifndef PAD_TS0_D1_IS_GPIO
#define PAD_TS0_D1_IS_GPIO PIN_176_IS_GPIO
#endif

#define PIN_177 PAD_TS0_D0
#define PAD_TS0_D0 52
#define GPIO_PAD_52 GPIO51
#ifndef PIN_177_IS_GPIO
#define PIN_177_IS_GPIO 0
#endif
#ifndef PAD_TS0_D0_IS_GPIO
#define PAD_TS0_D0_IS_GPIO PIN_177_IS_GPIO
#endif

#define PAD_RF_AGC 9999
#ifndef PAD_RF_AGC_IS_GPIO
#define PAD_RF_AGC_IS_GPIO 0
#endif

#define PAD_IF_AGC 9999
#ifndef PAD_IF_AGC_IS_GPIO
#define PAD_IF_AGC_IS_GPIO 0
#endif

#define PIN_183 PAD_SAR0
#define PAD_SAR0 20
#define GPIO_PAD_20 GPIO19
#ifndef PIN_183_IS_GPIO
#define PIN_183_IS_GPIO 0
#endif
#ifndef PAD_SAR0_IS_GPIO
#define PAD_SAR0_IS_GPIO PIN_183_IS_GPIO
#endif

#define PIN_184 PAD_SAR1
#define PAD_SAR1 21
#define GPIO_PAD_21 GPIO20
#ifndef PIN_184_IS_GPIO
#define PIN_184_IS_GPIO 0
#endif
#ifndef PAD_SAR1_IS_GPIO
#define PAD_SAR1_IS_GPIO PIN_184_IS_GPIO
#endif

#define PIN_185 PAD_SAR2
#define PAD_SAR2 22
#define GPIO_PAD_22 GPIO21
#ifndef PIN_185_IS_GPIO
#define PIN_185_IS_GPIO 0
#endif
#ifndef PAD_SAR2_IS_GPIO
#define PAD_SAR2_IS_GPIO PIN_185_IS_GPIO
#endif

#define PAD_SAR3 9999
#ifndef PAD_SAR3_IS_GPIO
#define PAD_SAR3_IS_GPIO 0
#endif

#define PIN_188 PAD_PM_SPI_CK
#define PAD_PM_SPI_CK 2
#define GPIO_PAD_2 GPIO1
#ifndef PIN_188_IS_GPIO
#define PIN_188_IS_GPIO 0
#endif
#ifndef PAD_PM_SPI_CK_IS_GPIO
#define PAD_PM_SPI_CK_IS_GPIO PIN_188_IS_GPIO
#endif

#define PIN_189 PAD_PM_SPI_DI
#define PAD_PM_SPI_DI 3
#define GPIO_PAD_3 GPIO2
#ifndef PIN_189_IS_GPIO
#define PIN_189_IS_GPIO 0
#endif
#ifndef PAD_PM_SPI_DI_IS_GPIO
#define PAD_PM_SPI_DI_IS_GPIO PIN_189_IS_GPIO
#endif

#define PIN_190 PAD_PM_SPI_DO
#define PAD_PM_SPI_DO 4
#define GPIO_PAD_4 GPIO3
#ifndef PIN_190_IS_GPIO
#define PIN_190_IS_GPIO 0
#endif
#ifndef PAD_PM_SPI_DO_IS_GPIO
#define PAD_PM_SPI_DO_IS_GPIO PIN_190_IS_GPIO
#endif

#define PIN_191 PAD_PM_SPI_CZ
#define PAD_PM_SPI_CZ 1
#define GPIO_PAD_1 GPIO0
#ifndef PIN_191_IS_GPIO
#define PIN_191_IS_GPIO 0
#endif
#ifndef PAD_PM_SPI_CZ_IS_GPIO
#define PAD_PM_SPI_CZ_IS_GPIO PIN_191_IS_GPIO
#endif

#define PAD_GPIO_PM0 9999
#ifndef PAD_GPIO_PM0_IS_GPIO
#define PAD_GPIO_PM0_IS_GPIO 0
#endif

#define PAD_GPIO_PM1 9999
#ifndef PAD_GPIO_PM1_IS_GPIO
#define PAD_GPIO_PM1_IS_GPIO 0
#endif

#define PIN_194 PAD_GPIO_PM4
#define PAD_GPIO_PM4 11
#define GPIO_PAD_11 GPIO10
#ifndef PIN_194_IS_GPIO
#define PIN_194_IS_GPIO 0
#endif
#ifndef PAD_GPIO_PM4_IS_GPIO
#define PAD_GPIO_PM4_IS_GPIO PIN_194_IS_GPIO
#endif

#define PAD_GPIO_PM2 9999
#ifndef PAD_GPIO_PM2_IS_GPIO
#define PAD_GPIO_PM2_IS_GPIO 0
#endif

#define PIN_195 PAD_GPIO_PM5
#define PAD_GPIO_PM5 12
#define GPIO_PAD_12 GPIO11
#ifndef PIN_195_IS_GPIO
#define PIN_195_IS_GPIO 0
#endif
#ifndef PAD_GPIO_PM5_IS_GPIO
#define PAD_GPIO_PM5_IS_GPIO PIN_195_IS_GPIO
#endif

#define PAD_GPIO_PM3 9999
#ifndef PAD_GPIO_PM3_IS_GPIO
#define PAD_GPIO_PM3_IS_GPIO 0
#endif

#define PIN_196 PAD_GPIO_PM6
#define PAD_GPIO_PM6 13
#define GPIO_PAD_13 GPIO12
#ifndef PIN_196_IS_GPIO
#define PIN_196_IS_GPIO 0
#endif
#ifndef PAD_GPIO_PM6_IS_GPIO
#define PAD_GPIO_PM6_IS_GPIO PIN_196_IS_GPIO
#endif

#define PAD_GPIO_PM7 9999
#ifndef PAD_GPIO_PM7_IS_GPIO
#define PAD_GPIO_PM7_IS_GPIO 0
#endif

#define PAD_IRIN 9999
#ifndef PAD_IRIN_IS_GPIO
#define PAD_IRIN_IS_GPIO 0
#endif

#define PAD_GPIO_PM8 9999
#ifndef PAD_GPIO_PM8_IS_GPIO
#define PAD_GPIO_PM8_IS_GPIO 0
#endif

#define PIN_197 PAD_CEC
#define PAD_CEC 6
#define GPIO_PAD_6 GPIO5
#ifndef PIN_197_IS_GPIO
#define PIN_197_IS_GPIO 0
#endif
#ifndef PAD_CEC_IS_GPIO
#define PAD_CEC_IS_GPIO PIN_197_IS_GPIO
#endif

#define PAD_GPIO_PM9 9999
#ifndef PAD_GPIO_PM9_IS_GPIO
#define PAD_GPIO_PM9_IS_GPIO 0
#endif

#define PAD_GPIO_PM10 9999
#ifndef PAD_GPIO_PM10_IS_GPIO
#define PAD_GPIO_PM10_IS_GPIO 0
#endif

#define PIN_199 PAD_GPIO_PM11
#define PAD_GPIO_PM11 18
#define GPIO_PAD_18 GPIO17
#ifndef PIN_199_IS_GPIO
#define PIN_199_IS_GPIO 0
#endif
#ifndef PAD_GPIO_PM11_IS_GPIO
#define PAD_GPIO_PM11_IS_GPIO PIN_199_IS_GPIO
#endif

#define PAD_GPIO_PM12 9999
#ifndef PAD_GPIO_PM12_IS_GPIO
#define PAD_GPIO_PM12_IS_GPIO 0
#endif

#define PIN_202 PAD_NF_CEZ
#define PAD_NF_CEZ 68
#define GPIO_PAD_68 GPIO67
#ifndef PIN_202_IS_GPIO
#define PIN_202_IS_GPIO 0
#endif
#ifndef PAD_NF_CEZ_IS_GPIO
#define PAD_NF_CEZ_IS_GPIO PIN_202_IS_GPIO
#endif

#define PAD_I2S_OUT_SD1 9999
#ifndef PAD_I2S_OUT_SD1_IS_GPIO
#define PAD_I2S_OUT_SD1_IS_GPIO 0
#endif

#define PIN_203 PAD_SPDIF_OUT
#define PAD_SPDIF_OUT 151
#define GPIO_PAD_151 GPIO150
#ifndef PIN_203_IS_GPIO
#define PIN_203_IS_GPIO 0
#endif
#ifndef PAD_SPDIF_OUT_IS_GPIO
#define PAD_SPDIF_OUT_IS_GPIO PIN_203_IS_GPIO
#endif

#define PAD_I2S_OUT_SD2 9999
#ifndef PAD_I2S_OUT_SD2_IS_GPIO
#define PAD_I2S_OUT_SD2_IS_GPIO 0
#endif

#define PIN_204 PAD_I2S_OUT_SD0
#define PAD_I2S_OUT_SD0 157
#define GPIO_PAD_157 GPIO156
#ifndef PIN_204_IS_GPIO
#define PIN_204_IS_GPIO 0
#endif
#ifndef PAD_I2S_OUT_SD0_IS_GPIO
#define PAD_I2S_OUT_SD0_IS_GPIO PIN_204_IS_GPIO
#endif

#define PAD_I2S_OUT_SD3 9999
#ifndef PAD_I2S_OUT_SD3_IS_GPIO
#define PAD_I2S_OUT_SD3_IS_GPIO 0
#endif

#define PIN_205 PAD_I2S_OUT_WS
#define PAD_I2S_OUT_WS 155
#define GPIO_PAD_155 GPIO154
#ifndef PIN_205_IS_GPIO
#define PIN_205_IS_GPIO 0
#endif
#ifndef PAD_I2S_OUT_WS_IS_GPIO
#define PAD_I2S_OUT_WS_IS_GPIO PIN_205_IS_GPIO
#endif

#define PAD_I2S_OUT_MCK 9999
#ifndef PAD_I2S_OUT_MCK_IS_GPIO
#define PAD_I2S_OUT_MCK_IS_GPIO 0
#endif

#define PIN_206 PAD_I2S_OUT_BCK
#define PAD_I2S_OUT_BCK 156
#define GPIO_PAD_156 GPIO155
#ifndef PIN_206_IS_GPIO
#define PIN_206_IS_GPIO 0
#endif
#ifndef PAD_I2S_OUT_BCK_IS_GPIO
#define PAD_I2S_OUT_BCK_IS_GPIO PIN_206_IS_GPIO
#endif

#define PIN_215 PAD_ET_COL
#define PAD_ET_COL 134
#define GPIO_PAD_134 GPIO133
#ifndef PIN_215_IS_GPIO
#define PIN_215_IS_GPIO 0
#endif
#ifndef PAD_ET_COL_IS_GPIO
#define PAD_ET_COL_IS_GPIO PIN_215_IS_GPIO
#endif

#define PAD_ET_CRS 9999
#ifndef PAD_ET_CRS_IS_GPIO
#define PAD_ET_CRS_IS_GPIO 0
#endif

#define PIN_216 PAD_ET_TXD1
#define PAD_ET_TXD1 137
#define GPIO_PAD_137 GPIO136
#ifndef PIN_216_IS_GPIO
#define PIN_216_IS_GPIO 0
#endif
#ifndef PAD_ET_TXD1_IS_GPIO
#define PAD_ET_TXD1_IS_GPIO PIN_216_IS_GPIO
#endif

#define PAD_ET_TXD3 9999
#ifndef PAD_ET_TXD3_IS_GPIO
#define PAD_ET_TXD3_IS_GPIO 0
#endif

#define PIN_217 PAD_ET_TXD0
#define PAD_ET_TXD0 138
#define GPIO_PAD_138 GPIO137
#ifndef PIN_217_IS_GPIO
#define PIN_217_IS_GPIO 0
#endif
#ifndef PAD_ET_TXD0_IS_GPIO
#define PAD_ET_TXD0_IS_GPIO PIN_217_IS_GPIO
#endif

#define PAD_ET_TXD2 9999
#ifndef PAD_ET_TXD2_IS_GPIO
#define PAD_ET_TXD2_IS_GPIO 0
#endif

#define PIN_218 PAD_ET_TX_EN
#define PAD_ET_TX_EN 139
#define GPIO_PAD_139 GPIO138
#ifndef PIN_218_IS_GPIO
#define PIN_218_IS_GPIO 0
#endif
#ifndef PAD_ET_TX_EN_IS_GPIO
#define PAD_ET_TX_EN_IS_GPIO PIN_218_IS_GPIO
#endif

#define PAD_ET_TXER 9999
#ifndef PAD_ET_TXER_IS_GPIO
#define PAD_ET_TXER_IS_GPIO 0
#endif

#define PIN_219 PAD_ET_TX_CLK
#define PAD_ET_TX_CLK 140
#define GPIO_PAD_140 GPIO139
#ifndef PIN_219_IS_GPIO
#define PIN_219_IS_GPIO 0
#endif
#ifndef PAD_ET_TX_CLK_IS_GPIO
#define PAD_ET_TX_CLK_IS_GPIO PIN_219_IS_GPIO
#endif

#define PAD_ET_RXER 9999
#ifndef PAD_ET_RXER_IS_GPIO
#define PAD_ET_RXER_IS_GPIO 0
#endif

#define PIN_220 PAD_ET_RXD0
#define PAD_ET_RXD0 145
#define GPIO_PAD_145 GPIO144
#ifndef PIN_220_IS_GPIO
#define PIN_220_IS_GPIO 0
#endif
#ifndef PAD_ET_RXD0_IS_GPIO
#define PAD_ET_RXD0_IS_GPIO PIN_220_IS_GPIO
#endif

#define PAD_ET_RX_CLK 9999
#ifndef PAD_ET_RX_CLK_IS_GPIO
#define PAD_ET_RX_CLK_IS_GPIO 0
#endif

#define PIN_221 PAD_ET_RXD1
#define PAD_ET_RXD1 146
#define GPIO_PAD_146 GPIO145
#ifndef PIN_221_IS_GPIO
#define PIN_221_IS_GPIO 0
#endif
#ifndef PAD_ET_RXD1_IS_GPIO
#define PAD_ET_RXD1_IS_GPIO PIN_221_IS_GPIO
#endif

#define PAD_ET_RX_DV 9999
#ifndef PAD_ET_RX_DV_IS_GPIO
#define PAD_ET_RX_DV_IS_GPIO 0
#endif

#define PIN_222 PAD_ET_MDC
#define PAD_ET_MDC 149
#define GPIO_PAD_149 GPIO148
#ifndef PIN_222_IS_GPIO
#define PIN_222_IS_GPIO 0
#endif
#ifndef PAD_ET_MDC_IS_GPIO
#define PAD_ET_MDC_IS_GPIO PIN_222_IS_GPIO
#endif

#define PAD_ET_RXD2 9999
#ifndef PAD_ET_RXD2_IS_GPIO
#define PAD_ET_RXD2_IS_GPIO 0
#endif

#define PIN_223 PAD_ET_MDIO
#define PAD_ET_MDIO 150
#define GPIO_PAD_150 GPIO149
#ifndef PIN_223_IS_GPIO
#define PIN_223_IS_GPIO 0
#endif
#ifndef PAD_ET_MDIO_IS_GPIO
#define PAD_ET_MDIO_IS_GPIO PIN_223_IS_GPIO
#endif

#define PAD_ET_RXD3 9999
#ifndef PAD_ET_RXD3_IS_GPIO
#define PAD_ET_RXD3_IS_GPIO 0
#endif

#define PADS_DDCR       1235
#define PADS_DDCR2      1236

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
#define CHIP_HAS_I2S_IN            0
#define CHIP_HAS_I2S_OUT           0
#define CHIP_HAS_SPDIF_IN          0
#define CHIP_HAS_SPDIF_OUT         0
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
#define CHIP_FAMILY_TYPE           CHIP_FAMILY_U4

#endif /* _MSD5041L_H_*/
