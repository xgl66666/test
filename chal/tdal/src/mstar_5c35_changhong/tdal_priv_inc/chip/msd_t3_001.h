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
// (��MStar Confidential Information��) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

// This file is generated by script, please do not edit it directly
#ifndef _MSD3303GX_H_
#define _MSD3303GX_H_

// PIN number to PAD name mapping
#define BALL_G6             PAD_GPIO_PM5        //
#define BALL_F7             PAD_GPIO_PM6        //
#define BALL_N2             PAD_HSYNC0          //
#define BALL_N1             PAD_VSYNC0          //
#define BALL_K3             PAD_HSYNC1          //
#define BALL_K2             PAD_VSYNC1          //
#define BALL_E1             PAD_DDCDB_DA        //
#define BALL_E2             PAD_HOTPLUGB        //
#define BALL_F3             PAD_DDCDB_CK        //
#define BALL_A2             PAD_HOTPLUGA        //
#define BALL_A1             PAD_DDCDA_DA        //
#define BALL_B2             PAD_DDCDA_CK        //
#define BALL_J5             PAD_HSYNC2          //
#define BALL_M3             PAD_CVBS7P          //
#define BALL_M2             PAD_CVBS6P          //
#define BALL_N3             PAD_CVBS5P          //
#define BALL_M1             PAD_CVBS4P          //
#define BALL_T1             PAD_CVBS3P          //
#define BALL_U2             PAD_CVBS2P          //
#define BALL_U3             PAD_CVBS1P          //
#define BALL_W1             PAD_CVBS0P          //
#define BALL_J2             PAD_DDCA_DA         //
#define BALL_J1             PAD_DDCA_CK         //
#define BALL_R4             PAD_TS0_D0          //
#define BALL_T4             PAD_TS0_D1          //
#define BALL_U4             PAD_TS0_D2          //
#define BALL_V4             PAD_TS0_D3          //
#define BALL_W4             PAD_TS0_D4          //
#define BALL_Y4             PAD_TS0_D5          //
#define BALL_AA4            PAD_TS0_D6          //
#define BALL_AB4            PAD_TS0_D7          //
#define BALL_W5             PAD_TS0_VLD         //
#define BALL_Y5             PAD_TS0_SYNC        //
#define BALL_AA5            PAD_TS0_CLK         //
#define BALL_AA14           PAD_PCM_D3          //
#define BALL_AB14           PAD_PCM_D4          //
#define BALL_AC14           PAD_PCM_D5          //
#define BALL_AD14           PAD_PCM_D6          //
#define BALL_AE14           PAD_PCM_D7          //
#define BALL_AA10           PAD_PCM_CE_N        //
#define BALL_AB12           PAD_PCM_A10         //
#define BALL_AC6            PAD_PCM_OE_N        //
#define BALL_AC12           PAD_PCM_A11         //
#define BALL_AA8            PAD_PCM_IORD_N      //
#define BALL_AA12           PAD_PCM_A9          //
#define BALL_AB8            PAD_PCM_IOWR_N      //
#define BALL_AE13           PAD_PCM_A8          //
#define BALL_AE12           PAD_PCM_A13         //
#define BALL_AB11           PAD_PCM_A14         //
#define BALL_AC11           PAD_PCM_WE_N        //
#define BALL_AB7            PAD_PCM_IRQA_N      //
#define BALL_AD12           PAD_PCM_A12         //
#define BALL_AD13           PAD_PCM_A7          //
#define BALL_AC13           PAD_PCM_A6          //
#define BALL_AB13           PAD_PCM_A5          //
#define BALL_AB10           PAD_PCM_WAIT_N      //
#define BALL_AA13           PAD_PCM_A4          //
#define BALL_AA9            PAD_PCM2_CE_N       //
#define BALL_AC10           PAD_PCM2_IRQA_N     //
#define BALL_AD18           PAD_PCM2_WAIT_N     //
#define BALL_H6             PAD_UART_RX2        //
#define BALL_J6             PAD_UART_TX2        //
#define BALL_AB16           PAD_PF_AD15         //
#define BALL_AB9            PAD_PF_ALE          //
#define BALL_AD6            PAD_PF_CE0Z         //
#define BALL_AE6            PAD_PF_CE1Z         //
#define BALL_AC15           PAD_PF_OEZ          //
#define BALL_AB6            PAD_PF_WEZ          //
#define BALL_AB15           PAD_F_RBZ           //
#define BALL_AE7            PAD_DDCDC_CK        //
#define BALL_AD7            PAD_DDCDC_DA        //
#define BALL_AC7            PAD_HOTPLUGC        //
#define BALL_AE15           PAD_SPI_CK          //
#define BALL_AD15           PAD_SPI_DI          //
#define BALL_AE16           PAD_SPI_DO          //
#define BALL_AD16           PAD_SPI_CZ          //
#define BALL_AE17           PAD_PWM0            //
#define BALL_AD17           PAD_PWM1            //
#define BALL_AC17           PAD_PWM2            //
#define BALL_AB17           PAD_PWM3            //
#define BALL_AB21           PAD_DDCR_DAT        //
#define BALL_AB22           PAD_DDCR_CLK        //
#define BALL_AE21           PAD_B_ODD0          //
#define BALL_AD21           PAD_B_ODD1          //
#define BALL_AC21           PAD_B_ODD2          //
#define BALL_AE22           PAD_B_ODD3          //
#define BALL_AD22           PAD_B_ODD4          //
#define BALL_AC22           PAD_B_ODD5          //
#define BALL_AE23           PAD_B_ODD6          //
#define BALL_AD23           PAD_B_ODD7          //
#define BALL_AC23           PAD_G_ODD0          //
#define BALL_AE24           PAD_G_ODD1          //
#define BALL_AD24           PAD_G_ODD2          //
#define BALL_AC24           PAD_G_ODD3          //
#define BALL_AE25           PAD_G_ODD4          //
#define BALL_AD25           PAD_G_ODD5          //
#define BALL_AC25           PAD_G_ODD6          //
#define BALL_AB25           PAD_G_ODD7          //
#define BALL_AB24           PAD_R_ODD0          //
#define BALL_AB23           PAD_R_ODD1          //
#define BALL_AA25           PAD_R_ODD2          //
#define BALL_AA24           PAD_R_ODD3          //
#define BALL_AA23           PAD_R_ODD4          //
#define BALL_Y25            PAD_R_ODD5          //
#define BALL_Y24            PAD_R_ODD6          //
#define BALL_Y23            PAD_R_ODD7          //
#define BALL_AA21           PAD_LCK             //
#define BALL_AA22           PAD_LDE             //
#define BALL_AC16           PAD_LHSYNC          //
#define BALL_AE18           PAD_LVSYNC          //
#define BALL_AE19           PAD_PCM_A3          //
#define BALL_AD19           PAD_PCM_A2          //
#define BALL_AE20           PAD_PCM_REG_N       //
#define BALL_AC19           PAD_PCM_A1          //
#define BALL_AB19           PAD_PCM_A0          //
#define BALL_AD20           PAD_PCM_D0          //
#define BALL_AC20           PAD_PCM_D1          //
#define BALL_AB20           PAD_PCM_D2          //
#define BALL_AC18           PAD_PCM_RESET       //
#define BALL_AB18           PAD_PCM2_RESET      //
#define BALL_AA20           PAD_PCM_CD_N        //
#define BALL_AA19           PAD_PCM2_CD_N       //
#define BALL_V22            PAD_TS1_D0          //
#define BALL_Y22            PAD_TS1_VLD         //
#define BALL_W22            PAD_TS1_SYNC        //
#define BALL_U22            PAD_TS1_CLK         //
#define BALL_A11            PAD_ET_COL          //
#define BALL_B11            PAD_ET_TXD1         //
#define BALL_C10            PAD_ET_TXD0         //
#define BALL_B10            PAD_ET_TX_EN        //
#define BALL_A9             PAD_ET_TX_CLK       //
#define BALL_C11            PAD_ET_RXD0         //
#define BALL_A7             PAD_I2S_IN_WS       //
#define BALL_B8             PAD_I2S_IN_BCK      //
#define BALL_C8             PAD_I2S_IN_SD       //
#define BALL_F11            PAD_SPDIF_IN        //
#define BALL_A8             PAD_I2S_OUT_MCK     //
#define BALL_B7             PAD_I2S_OUT_WS      //
#define BALL_D10            PAD_ET_CRS          //
#define BALL_D7             PAD_ET_TXD3         //
#define BALL_C7             PAD_I2S_OUT_BCK     //
#define BALL_E11            PAD_ET_TXD2         //
#define BALL_D8             PAD_I2S_OUT_SD      //
#define BALL_D9             PAD_ET_TXER         //
#define BALL_E9             PAD_SPDIF_OUT       //
#define BALL_E8             PAD_ET_RXER         //
#define BALL_D11            PAD_DDCR_DA2        //
#define BALL_E10            PAD_ET_RX_CLK       //
#define BALL_F8             PAD_DDCR_CK2        //
#define BALL_D6             PAD_ET_RX_DV        //
#define BALL_E7             PAD_LHSYNC2         //
#define BALL_C6             PAD_LVSYNC2         //
#define BALL_C9             PAD_ET_RXD1         //
#define BALL_F9             PAD_GPIO12          //
#define BALL_C5             PAD_ET_RXD2         //
#define BALL_F10            PAD_GPIO13          //
#define BALL_D5             PAD_ET_RXD3         //
#define BALL_A6             PAD_GPIO14          //
#define BALL_A10            PAD_ET_MDC          //
#define BALL_B6             PAD_GPIO15          //
#define BALL_B9             PAD_ET_MDIO         //
#define BALL_A4             PAD_SAR0            //
#define BALL_B4             PAD_SAR1            //
#define BALL_F4             PAD_SAR2            //
#define BALL_E4             PAD_SAR3            //
#define BALL_E5             PAD_GPIO_PM0        //
#define BALL_F5             PAD_GPIO_PM1        //
#define BALL_G5             PAD_GPIO_PM2        //
#define BALL_H5             PAD_GPIO_PM3        //
#define BALL_F6             PAD_GPIO_PM4        //

// PAD name to index
#define PAD_NC              9999
#define PAD_GND             0

#define PAD_DDCROM_DA       179
#define PAD_DDCROM_CK       180
#define PAD_GPIO_PM7        142
#define PAD_GPIO_PM8        143
#define PAD_GPIO_PM9        144
#define PAD_GPIO_PM10       145
#define PAD_GPIO_PM5        140
#define PAD_GPIO_PM6        141
#define PAD_HSYNC0          1002
#define PAD_VSYNC0          1003
#define PAD_HSYNC1          1004
#define PAD_VSYNC1          1005
#define PAD_DDCDB_DA        175
#define PAD_HOTPLUGB        168
#define PAD_DDCDB_CK        176
#define PAD_HOTPLUGA        169
#define PAD_DDCDA_DA        173
#define PAD_DDCDA_CK        174
#define PAD_HSYNC2          1041
#define PAD_VSYNC2          1042
#define PAD_CVBS7P          1047
#define PAD_CVBS6P          1048
#define PAD_CVBS5P          1049
#define PAD_CVBS4P          1050
#define PAD_CVBS3P          1052
#define PAD_CVBS2P          1053
#define PAD_CVBS1P          1054
#define PAD_CVBS0P          1056
#define PAD_DDCA_DA         171
#define PAD_DDCA_CK         172
#define PAD_TS0_D0          3
#define PAD_TS0_D1          4
#define PAD_TS0_D2          5
#define PAD_TS0_D3          6
#define PAD_TS0_D4          7
#define PAD_TS0_D5          8
#define PAD_TS0_D6          9
#define PAD_TS0_D7          10
#define PAD_TS0_VLD         11
#define PAD_TS0_SYNC        12
#define PAD_TS0_CLK         13
#define PAD_PCM_D3          21
#define PAD_PCM_D4          22
#define PAD_PCM_D5          23
#define PAD_PCM_D6          24
#define PAD_PCM_D7          25
#define PAD_PCM_CE_N        26
#define PAD_PCM_A10         27
#define PAD_PCM_OE_N        28
#define PAD_PCM_A11         29
#define PAD_PCM_IORD_N      30
#define PAD_PCM_A9          31
#define PAD_PCM_IOWR_N      32
#define PAD_PCM_A8          33
#define PAD_PCM_A13         34
#define PAD_PCM_A14         35
#define PAD_PCM_WE_N        1099
#define PAD_PCM_IRQA_N      36
#define PAD_PCM_A12         37
#define PAD_PCM_A7          38
#define PAD_PCM_A6          39
#define PAD_PCM_A5          40
#define PAD_PCM_WAIT_N      41
#define PAD_PCM_A4          42
#define PAD_PCM2_CE_N       43
#define PAD_PCM2_IRQA_N     44
#define PAD_PCM2_WAIT_N     45
#define PAD_UART_RX2        1
#define PAD_UART_TX2        2
#define PAD_PF_AD15         14
#define PAD_PF_ALE          19
#define PAD_PF_CE0Z         15
#define PAD_PF_CE1Z         16
#define PAD_PF_OEZ          17
#define PAD_PF_WEZ          18
#define PAD_F_RBZ           20
#define PAD_DDCDC_CK        178
#define PAD_DDCDC_DA        177
#define PAD_HOTPLUGC        170
#define PAD_SPI_CK          81
#define PAD_SPI_DI          82
#define PAD_SPI_DO          84
#define PAD_SPI_CZ          83
#define PAD_PWM0            46
#define PAD_PWM1            47
#define PAD_PWM2            48
#define PAD_PWM3            49
#define PAD_DDCR_DAT        50
#define PAD_DDCR_CLK        51
#define PAD_B_ODD0          107
#define PAD_B_ODD1          108
#define PAD_B_ODD2          109
#define PAD_B_ODD3          110
#define PAD_B_ODD4          111
#define PAD_B_ODD5          112
#define PAD_B_ODD6          113
#define PAD_B_ODD7          114
#define PAD_G_ODD0          115
#define PAD_G_ODD1          116
#define PAD_G_ODD2          117
#define PAD_G_ODD3          118
#define PAD_G_ODD4          119
#define PAD_G_ODD5          120
#define PAD_G_ODD6          121
#define PAD_G_ODD7          122
#define PAD_R_ODD0          123
#define PAD_R_ODD1          124
#define PAD_R_ODD2          125
#define PAD_R_ODD3          126
#define PAD_R_ODD4          127
#define PAD_R_ODD5          128
#define PAD_R_ODD6          129
#define PAD_R_ODD7          130
#define PAD_LCK             131
#define PAD_LDE             132
#define PAD_LHSYNC          133
#define PAD_LVSYNC          134
#define PAD_TCON0           151
#define PAD_TCON1           152
#define PAD_TCON2           153
#define PAD_TCON3           154
#define PAD_TCON4           155
#define PAD_TCON5           156
#define PAD_TCON6           157
#define PAD_TCON7           158
#define PAD_TCON8           159
#define PAD_TCON9           160
#define PAD_TCON10          161
#define PAD_TCON11          162
#define PAD_TCON12          163
#define PAD_TCON13          164
#define PAD_TCON14          165
#define PAD_TCON15          166
#define PAD_TCON16          167
#define PAD_PCM_A3          52
#define PAD_PCM_A2          53
#define PAD_PCM_REG_N       54
#define PAD_PCM_A1          55
#define PAD_PCM_A0          56
#define PAD_PCM_D0          57
#define PAD_PCM_D1          58
#define PAD_PCM_D2          59
#define PAD_PCM_RESET       60
#define PAD_PCM2_RESET      61
#define PAD_PCM_CD_N        62
#define PAD_PCM2_CD_N       63
#define PAD_TS1_D0          64
#define PAD_TS1_VLD         65
#define PAD_TS1_SYNC        66
#define PAD_TS1_CLK         67
#define PAD_ET_COL          90
#define PAD_ET_TXD1         93
#define PAD_ET_TXD0         94
#define PAD_ET_TX_EN        95
#define PAD_ET_TX_CLK       96
#define PAD_ET_RXD0         101
#define PAD_I2S_IN_WS       68
#define PAD_I2S_IN_BCK      69
#define PAD_I2S_IN_SD       70
#define PAD_SPDIF_IN        71
#define PAD_I2S_OUT_MCK     72
#define PAD_I2S_OUT_WS      73
#define PAD_ET_CRS          89
#define PAD_ET_TXD3         91
#define PAD_I2S_OUT_BCK     74
#define PAD_ET_TXD2         92
#define PAD_I2S_OUT_SD      75
#define PAD_ET_TXER         97
#define PAD_SPDIF_OUT       76
#define PAD_ET_RXER         98
#define PAD_DDCR_DA2        77
#define PAD_ET_RX_CLK       99
#define PAD_DDCR_CK2        78
#define PAD_ET_RX_DV        100
#define PAD_LHSYNC2         79
#define PAD_LVSYNC2         80
#define PAD_ET_RXD1         102
#define PAD_GPIO12          85
#define PAD_ET_RXD2         103
#define PAD_GPIO13          86
#define PAD_ET_RXD3         104
#define PAD_GPIO14          87
#define PAD_ET_MDC          105
#define PAD_GPIO15          88
#define PAD_ET_MDIO         106
#define PAD_SAR0            147
#define PAD_SAR1            148
#define PAD_SAR2            149
#define PAD_SAR3            150
#define PAD_GPIO_PM0        135
#define PAD_GPIO_PM1        136
#define PAD_GPIO_PM2        137
#define PAD_GPIO_PM3        138
#define PAD_GPIO_PM4        139

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

// PAD index to GPIO number mapping
#define GPIO_PAD_91         GPIO90
#define GPIO_PAD_177        GPIO_DDCD2_SDA
#define GPIO_PAD_170        GPIO169
#define GPIO_PAD_79         GPIO78
#define GPIO_PAD_34         GPIO33
#define GPIO_PAD_32         GPIO31
#define GPIO_PAD_126        GPIO125
#define GPIO_PAD_109        GPIO108
#define GPIO_PAD_26         GPIO25
#define GPIO_PAD_5          GPIO4
#define GPIO_PAD_36         GPIO35
#define GPIO_PAD_8          GPIO7
#define GPIO_PAD_99         GPIO98
#define GPIO_PAD_82         GPIO81
#define GPIO_PAD_48         GPIO47
#define GPIO_PAD_103        GPIO102
#define GPIO_PAD_7          GPIO6
#define GPIO_PAD_27         GPIO26
#define GPIO_PAD_113        GPIO112
#define GPIO_PAD_83         GPIO82
#define GPIO_PAD_89         GPIO88
#define GPIO_PAD_3          GPIO2
#define GPIO_PAD_116        GPIO115
#define GPIO_PAD_98         GPIO97
#define GPIO_PAD_147        GPIO146
#define GPIO_PAD_105        GPIO104
#define GPIO_PAD_172        GPIO_DDCA0_SCL
#define GPIO_PAD_64         GPIO63
#define GPIO_PAD_12         GPIO11
#define GPIO_PAD_23         GPIO22
#define GPIO_PAD_168        GPIO167
#define GPIO_PAD_90         GPIO89
#define GPIO_PAD_69         GPIO68
#define GPIO_PAD_18         GPIO17
#define GPIO_PAD_149        GPIO148
#define GPIO_PAD_66         GPIO65
#define GPIO_PAD_59         GPIO58
#define GPIO_PAD_77         GPIO76
#define GPIO_PAD_65         GPIO64
#define GPIO_PAD_45         GPIO44
#define GPIO_PAD_123        GPIO122
#define GPIO_PAD_106        GPIO105
#define GPIO_PAD_176        GPIO_DDCD1_SCL
#define GPIO_PAD_117        GPIO116
#define GPIO_PAD_52         GPIO51
#define GPIO_PAD_95         GPIO94
#define GPIO_PAD_44         GPIO43
#define GPIO_PAD_37         GPIO36
#define GPIO_PAD_132        GPIO131
#define GPIO_PAD_86         GPIO85
#define GPIO_PAD_133        GPIO132
#define GPIO_PAD_11         GPIO10
#define GPIO_PAD_16         GPIO15
#define GPIO_PAD_139        GPIO138
#define GPIO_PAD_120        GPIO119
#define GPIO_PAD_97         GPIO96
#define GPIO_PAD_78         GPIO77
#define GPIO_PAD_46         GPIO45
#define GPIO_PAD_49         GPIO48
#define GPIO_PAD_25         GPIO24
#define GPIO_PAD_96         GPIO95
#define GPIO_PAD_39         GPIO38
#define GPIO_PAD_94         GPIO93
#define GPIO_PAD_88         GPIO87
#define GPIO_PAD_56         GPIO55
#define GPIO_PAD_40         GPIO39
#define GPIO_PAD_136        GPIO135
#define GPIO_PAD_128        GPIO127
#define GPIO_PAD_115        GPIO114
#define GPIO_PAD_28         GPIO27
#define GPIO_PAD_9          GPIO8
#define GPIO_PAD_85         GPIO84
#define GPIO_PAD_6          GPIO5
#define GPIO_PAD_31         GPIO30
#define GPIO_PAD_122        GPIO121
#define GPIO_PAD_75         GPIO74
#define GPIO_PAD_104        GPIO103
#define GPIO_PAD_17         GPIO16
#define GPIO_PAD_112        GPIO111
#define GPIO_PAD_178        GPIO_DDCD2_SCL
#define GPIO_PAD_57         GPIO56
#define GPIO_PAD_175        GPIO_DDCD1_SDA
#define GPIO_PAD_108        GPIO107
#define GPIO_PAD_150        GPIO149
#define GPIO_PAD_30         GPIO29
#define GPIO_PAD_70         GPIO69
#define GPIO_PAD_173        GPIO_DDCD0_SDA
#define GPIO_PAD_80         GPIO79
#define GPIO_PAD_74         GPIO73
#define GPIO_PAD_73         GPIO72
#define GPIO_PAD_130        GPIO129
#define GPIO_PAD_58         GPIO57
#define GPIO_PAD_119        GPIO118
#define GPIO_PAD_35         GPIO34
#define GPIO_PAD_101        GPIO100
#define GPIO_PAD_92         GPIO91
#define GPIO_PAD_21         GPIO20
#define GPIO_PAD_63         GPIO62
#define GPIO_PAD_13         GPIO12
#define GPIO_PAD_1          GPIO0
#define GPIO_PAD_24         GPIO23
#define GPIO_PAD_84         GPIO83
#define GPIO_PAD_19         GPIO18
#define GPIO_PAD_102        GPIO101
#define GPIO_PAD_111        GPIO110
#define GPIO_PAD_53         GPIO52
#define GPIO_PAD_41         GPIO40
#define GPIO_PAD_67         GPIO66
#define GPIO_PAD_20         GPIO19
#define GPIO_PAD_29         GPIO28
#define GPIO_PAD_42         GPIO41
#define GPIO_PAD_14         GPIO13
#define GPIO_PAD_124        GPIO123
#define GPIO_PAD_148        GPIO147
#define GPIO_PAD_134        GPIO133
#define GPIO_PAD_43         GPIO42
#define GPIO_PAD_169        GPIO168
#define GPIO_PAD_2          GPIO1
#define GPIO_PAD_4          GPIO3
#define GPIO_PAD_87         GPIO86
#define GPIO_PAD_118        GPIO117
#define GPIO_PAD_129        GPIO128
#define GPIO_PAD_62         GPIO61
#define GPIO_PAD_51         GPIO50
#define GPIO_PAD_54         GPIO53
#define GPIO_PAD_15         GPIO14
#define GPIO_PAD_22         GPIO21
#define GPIO_PAD_121        GPIO120
#define GPIO_PAD_114        GPIO113
#define GPIO_PAD_47         GPIO46
#define GPIO_PAD_61         GPIO60
#define GPIO_PAD_72         GPIO71
#define GPIO_PAD_10         GPIO9
#define GPIO_PAD_110        GPIO109
#define GPIO_PAD_125        GPIO124
#define GPIO_PAD_81         GPIO80
#define GPIO_PAD_60         GPIO59
#define GPIO_PAD_55         GPIO54
#define GPIO_PAD_38         GPIO37
#define GPIO_PAD_131        GPIO130
#define GPIO_PAD_76         GPIO75
#define GPIO_PAD_138        GPIO137
#define GPIO_PAD_100        GPIO99
#define GPIO_PAD_33         GPIO32
#define GPIO_PAD_50         GPIO49
#define GPIO_PAD_68         GPIO67
#define GPIO_PAD_135        GPIO134
#define GPIO_PAD_71         GPIO70
#define GPIO_PAD_127        GPIO126
#define GPIO_PAD_137        GPIO136
#define GPIO_PAD_171        GPIO_DDCA0_SDA
#define GPIO_PAD_93         GPIO92
#define GPIO_PAD_174        GPIO_DDCD0_SCL
#define GPIO_PAD_107        GPIO106
#define GPIO_PAD_9999       GPIO_NC

// default GPIO setting
#define PAD_TCON15_IS_GPIO 0
#ifndef BALL_D7_IS_GPIO
#define BALL_D7_IS_GPIO 0
#endif
#ifndef PAD_ET_TXD3_IS_GPIO
#define PAD_ET_TXD3_IS_GPIO BALL_D7_IS_GPIO
#endif
#ifndef BALL_AD7_IS_GPIO
#define BALL_AD7_IS_GPIO 0
#endif
#ifndef PAD_DDCDC_DA_IS_GPIO
#define PAD_DDCDC_DA_IS_GPIO BALL_AD7_IS_GPIO
#endif
#ifndef BALL_AC7_IS_GPIO
#define BALL_AC7_IS_GPIO 0
#endif
#ifndef PAD_HOTPLUGC_IS_GPIO
#define PAD_HOTPLUGC_IS_GPIO BALL_AC7_IS_GPIO
#endif
#ifndef BALL_E7_IS_GPIO
#define BALL_E7_IS_GPIO 0
#endif
#ifndef PAD_LHSYNC2_IS_GPIO
#define PAD_LHSYNC2_IS_GPIO BALL_E7_IS_GPIO
#endif
#ifndef BALL_AE12_IS_GPIO
#define BALL_AE12_IS_GPIO 0
#endif
#ifndef PAD_PCM_A13_IS_GPIO
#define PAD_PCM_A13_IS_GPIO BALL_AE12_IS_GPIO
#endif
#ifndef BALL_AB8_IS_GPIO
#define BALL_AB8_IS_GPIO 0
#endif
#ifndef PAD_PCM_IOWR_N_IS_GPIO
#define PAD_PCM_IOWR_N_IS_GPIO BALL_AB8_IS_GPIO
#endif
#ifndef BALL_AA24_IS_GPIO
#define BALL_AA24_IS_GPIO 0
#endif
#ifndef PAD_R_ODD3_IS_GPIO
#define PAD_R_ODD3_IS_GPIO BALL_AA24_IS_GPIO
#endif
#define PAD_GPIO_PM9_IS_GPIO 0
#define PAD_TCON13_IS_GPIO 0
#ifndef BALL_AC21_IS_GPIO
#define BALL_AC21_IS_GPIO 0
#endif
#ifndef PAD_B_ODD2_IS_GPIO
#define PAD_B_ODD2_IS_GPIO BALL_AC21_IS_GPIO
#endif
#ifndef BALL_AA10_IS_GPIO
#define BALL_AA10_IS_GPIO 0
#endif
#ifndef PAD_PCM_CE_N_IS_GPIO
#define PAD_PCM_CE_N_IS_GPIO BALL_AA10_IS_GPIO
#endif
#ifndef BALL_U4_IS_GPIO
#define BALL_U4_IS_GPIO 0
#endif
#ifndef PAD_TS0_D2_IS_GPIO
#define PAD_TS0_D2_IS_GPIO BALL_U4_IS_GPIO
#endif
#ifndef BALL_AB7_IS_GPIO
#define BALL_AB7_IS_GPIO 0
#endif
#ifndef PAD_PCM_IRQA_N_IS_GPIO
#define PAD_PCM_IRQA_N_IS_GPIO BALL_AB7_IS_GPIO
#endif
#ifndef BALL_Y4_IS_GPIO
#define BALL_Y4_IS_GPIO 0
#endif
#ifndef PAD_TS0_D5_IS_GPIO
#define PAD_TS0_D5_IS_GPIO BALL_Y4_IS_GPIO
#endif
#ifndef BALL_E10_IS_GPIO
#define BALL_E10_IS_GPIO 0
#endif
#ifndef PAD_ET_RX_CLK_IS_GPIO
#define PAD_ET_RX_CLK_IS_GPIO BALL_E10_IS_GPIO
#endif
#define PAD_TCON2_IS_GPIO 0
#ifndef BALL_AD15_IS_GPIO
#define BALL_AD15_IS_GPIO 0
#endif
#ifndef PAD_SPI_DI_IS_GPIO
#define PAD_SPI_DI_IS_GPIO BALL_AD15_IS_GPIO
#endif
#ifndef BALL_AC17_IS_GPIO
#define BALL_AC17_IS_GPIO 0
#endif
#ifndef PAD_PWM2_IS_GPIO
#define PAD_PWM2_IS_GPIO BALL_AC17_IS_GPIO
#endif
#ifndef BALL_C5_IS_GPIO
#define BALL_C5_IS_GPIO 0
#endif
#ifndef PAD_ET_RXD2_IS_GPIO
#define PAD_ET_RXD2_IS_GPIO BALL_C5_IS_GPIO
#endif
#define PAD_TCON7_IS_GPIO 0
#ifndef BALL_W4_IS_GPIO
#define BALL_W4_IS_GPIO 0
#endif
#ifndef PAD_TS0_D4_IS_GPIO
#define PAD_TS0_D4_IS_GPIO BALL_W4_IS_GPIO
#endif
#ifndef BALL_AB12_IS_GPIO
#define BALL_AB12_IS_GPIO 0
#endif
#ifndef PAD_PCM_A10_IS_GPIO
#define PAD_PCM_A10_IS_GPIO BALL_AB12_IS_GPIO
#endif
#ifndef BALL_AE23_IS_GPIO
#define BALL_AE23_IS_GPIO 0
#endif
#ifndef PAD_B_ODD6_IS_GPIO
#define PAD_B_ODD6_IS_GPIO BALL_AE23_IS_GPIO
#endif
#ifndef BALL_AD16_IS_GPIO
#define BALL_AD16_IS_GPIO 0
#endif
#ifndef PAD_SPI_CZ_IS_GPIO
#define PAD_SPI_CZ_IS_GPIO BALL_AD16_IS_GPIO
#endif
#ifndef BALL_D10_IS_GPIO
#define BALL_D10_IS_GPIO 0
#endif
#ifndef PAD_ET_CRS_IS_GPIO
#define PAD_ET_CRS_IS_GPIO BALL_D10_IS_GPIO
#endif
#define PAD_GPIO_PM7_IS_GPIO 0
#ifndef BALL_R4_IS_GPIO
#define BALL_R4_IS_GPIO 0
#endif
#ifndef PAD_TS0_D0_IS_GPIO
#define PAD_TS0_D0_IS_GPIO BALL_R4_IS_GPIO
#endif
#ifndef BALL_AE24_IS_GPIO
#define BALL_AE24_IS_GPIO 0
#endif
#ifndef PAD_G_ODD1_IS_GPIO
#define PAD_G_ODD1_IS_GPIO BALL_AE24_IS_GPIO
#endif
#ifndef BALL_E8_IS_GPIO
#define BALL_E8_IS_GPIO 0
#endif
#ifndef PAD_ET_RXER_IS_GPIO
#define PAD_ET_RXER_IS_GPIO BALL_E8_IS_GPIO
#endif
#ifndef BALL_A4_IS_GPIO
#define BALL_A4_IS_GPIO 0
#endif
#ifndef PAD_SAR0_IS_GPIO
#define PAD_SAR0_IS_GPIO BALL_A4_IS_GPIO
#endif
#ifndef BALL_A10_IS_GPIO
#define BALL_A10_IS_GPIO 0
#endif
#ifndef PAD_ET_MDC_IS_GPIO
#define PAD_ET_MDC_IS_GPIO BALL_A10_IS_GPIO
#endif
#ifndef BALL_J1_IS_GPIO
#define BALL_J1_IS_GPIO 0
#endif
#ifndef PAD_DDCA_CK_IS_GPIO
#define PAD_DDCA_CK_IS_GPIO BALL_J1_IS_GPIO
#endif
#ifndef BALL_V22_IS_GPIO
#define BALL_V22_IS_GPIO 0
#endif
#ifndef PAD_TS1_D0_IS_GPIO
#define PAD_TS1_D0_IS_GPIO BALL_V22_IS_GPIO
#endif
#ifndef BALL_Y5_IS_GPIO
#define BALL_Y5_IS_GPIO 0
#endif
#ifndef PAD_TS0_SYNC_IS_GPIO
#define PAD_TS0_SYNC_IS_GPIO BALL_Y5_IS_GPIO
#endif
#ifndef BALL_AC14_IS_GPIO
#define BALL_AC14_IS_GPIO 0
#endif
#ifndef PAD_PCM_D5_IS_GPIO
#define PAD_PCM_D5_IS_GPIO BALL_AC14_IS_GPIO
#endif
#ifndef BALL_E2_IS_GPIO
#define BALL_E2_IS_GPIO 0
#endif
#ifndef PAD_HOTPLUGB_IS_GPIO
#define PAD_HOTPLUGB_IS_GPIO BALL_E2_IS_GPIO
#endif
#ifndef BALL_A11_IS_GPIO
#define BALL_A11_IS_GPIO 0
#endif
#ifndef PAD_ET_COL_IS_GPIO
#define PAD_ET_COL_IS_GPIO BALL_A11_IS_GPIO
#endif
#ifndef BALL_B8_IS_GPIO
#define BALL_B8_IS_GPIO 0
#endif
#ifndef PAD_I2S_IN_BCK_IS_GPIO
#define PAD_I2S_IN_BCK_IS_GPIO BALL_B8_IS_GPIO
#endif
#ifndef BALL_AB6_IS_GPIO
#define BALL_AB6_IS_GPIO 0
#endif
#ifndef PAD_PF_WEZ_IS_GPIO
#define PAD_PF_WEZ_IS_GPIO BALL_AB6_IS_GPIO
#endif
#ifndef BALL_F4_IS_GPIO
#define BALL_F4_IS_GPIO 0
#endif
#ifndef PAD_SAR2_IS_GPIO
#define PAD_SAR2_IS_GPIO BALL_F4_IS_GPIO
#endif
#ifndef BALL_W22_IS_GPIO
#define BALL_W22_IS_GPIO 0
#endif
#ifndef PAD_TS1_SYNC_IS_GPIO
#define PAD_TS1_SYNC_IS_GPIO BALL_W22_IS_GPIO
#endif
#ifndef BALL_AB20_IS_GPIO
#define BALL_AB20_IS_GPIO 0
#endif
#ifndef PAD_PCM_D2_IS_GPIO
#define PAD_PCM_D2_IS_GPIO BALL_AB20_IS_GPIO
#endif
#ifndef BALL_D11_IS_GPIO
#define BALL_D11_IS_GPIO 0
#endif
#ifndef PAD_DDCR_DA2_IS_GPIO
#define PAD_DDCR_DA2_IS_GPIO BALL_D11_IS_GPIO
#endif
#ifndef BALL_Y22_IS_GPIO
#define BALL_Y22_IS_GPIO 0
#endif
#ifndef PAD_TS1_VLD_IS_GPIO
#define PAD_TS1_VLD_IS_GPIO BALL_Y22_IS_GPIO
#endif
#ifndef BALL_AD18_IS_GPIO
#define BALL_AD18_IS_GPIO 0
#endif
#ifndef PAD_PCM2_WAIT_N_IS_GPIO
#define PAD_PCM2_WAIT_N_IS_GPIO BALL_AD18_IS_GPIO
#endif
#ifndef BALL_AB24_IS_GPIO
#define BALL_AB24_IS_GPIO 0
#endif
#ifndef PAD_R_ODD0_IS_GPIO
#define PAD_R_ODD0_IS_GPIO BALL_AB24_IS_GPIO
#endif
#ifndef BALL_B9_IS_GPIO
#define BALL_B9_IS_GPIO 0
#endif
#ifndef PAD_ET_MDIO_IS_GPIO
#define PAD_ET_MDIO_IS_GPIO BALL_B9_IS_GPIO
#endif
#ifndef BALL_F3_IS_GPIO
#define BALL_F3_IS_GPIO 0
#endif
#ifndef PAD_DDCDB_CK_IS_GPIO
#define PAD_DDCDB_CK_IS_GPIO BALL_F3_IS_GPIO
#endif
#ifndef BALL_AD24_IS_GPIO
#define BALL_AD24_IS_GPIO 0
#endif
#ifndef PAD_G_ODD2_IS_GPIO
#define PAD_G_ODD2_IS_GPIO BALL_AD24_IS_GPIO
#endif
#ifndef BALL_AE19_IS_GPIO
#define BALL_AE19_IS_GPIO 0
#endif
#ifndef PAD_PCM_A3_IS_GPIO
#define PAD_PCM_A3_IS_GPIO BALL_AE19_IS_GPIO
#endif
#ifndef BALL_B10_IS_GPIO
#define BALL_B10_IS_GPIO 0
#endif
#ifndef PAD_ET_TX_EN_IS_GPIO
#define PAD_ET_TX_EN_IS_GPIO BALL_B10_IS_GPIO
#endif
#ifndef BALL_AC10_IS_GPIO
#define BALL_AC10_IS_GPIO 0
#endif
#ifndef PAD_PCM2_IRQA_N_IS_GPIO
#define PAD_PCM2_IRQA_N_IS_GPIO BALL_AC10_IS_GPIO
#endif
#ifndef BALL_AD12_IS_GPIO
#define BALL_AD12_IS_GPIO 0
#endif
#ifndef PAD_PCM_A12_IS_GPIO
#define PAD_PCM_A12_IS_GPIO BALL_AD12_IS_GPIO
#endif
#define PAD_TCON5_IS_GPIO 0
#ifndef BALL_AA22_IS_GPIO
#define BALL_AA22_IS_GPIO 0
#endif
#ifndef PAD_LDE_IS_GPIO
#define PAD_LDE_IS_GPIO BALL_AA22_IS_GPIO
#endif
#ifndef BALL_F10_IS_GPIO
#define BALL_F10_IS_GPIO 0
#endif
#ifndef PAD_GPIO13_IS_GPIO
#define PAD_GPIO13_IS_GPIO BALL_F10_IS_GPIO
#endif
#ifndef BALL_AC16_IS_GPIO
#define BALL_AC16_IS_GPIO 0
#endif
#ifndef PAD_LHSYNC_IS_GPIO
#define PAD_LHSYNC_IS_GPIO BALL_AC16_IS_GPIO
#endif
#ifndef BALL_W5_IS_GPIO
#define BALL_W5_IS_GPIO 0
#endif
#ifndef PAD_TS0_VLD_IS_GPIO
#define PAD_TS0_VLD_IS_GPIO BALL_W5_IS_GPIO
#endif
#ifndef BALL_AE6_IS_GPIO
#define BALL_AE6_IS_GPIO 0
#endif
#ifndef PAD_PF_CE1Z_IS_GPIO
#define PAD_PF_CE1Z_IS_GPIO BALL_AE6_IS_GPIO
#endif
#ifndef BALL_F6_IS_GPIO
#define BALL_F6_IS_GPIO 0
#endif
#ifndef PAD_GPIO_PM4_IS_GPIO
#define PAD_GPIO_PM4_IS_GPIO BALL_F6_IS_GPIO
#endif
#ifndef BALL_AD25_IS_GPIO
#define BALL_AD25_IS_GPIO 0
#endif
#ifndef PAD_G_ODD5_IS_GPIO
#define PAD_G_ODD5_IS_GPIO BALL_AD25_IS_GPIO
#endif
#ifndef BALL_D9_IS_GPIO
#define BALL_D9_IS_GPIO 0
#endif
#ifndef PAD_ET_TXER_IS_GPIO
#define PAD_ET_TXER_IS_GPIO BALL_D9_IS_GPIO
#endif
#ifndef BALL_F8_IS_GPIO
#define BALL_F8_IS_GPIO 0
#endif
#ifndef PAD_DDCR_CK2_IS_GPIO
#define PAD_DDCR_CK2_IS_GPIO BALL_F8_IS_GPIO
#endif
#ifndef BALL_AE17_IS_GPIO
#define BALL_AE17_IS_GPIO 0
#endif
#ifndef PAD_PWM0_IS_GPIO
#define PAD_PWM0_IS_GPIO BALL_AE17_IS_GPIO
#endif
#ifndef BALL_AB17_IS_GPIO
#define BALL_AB17_IS_GPIO 0
#endif
#ifndef PAD_PWM3_IS_GPIO
#define PAD_PWM3_IS_GPIO BALL_AB17_IS_GPIO
#endif
#define PAD_GPIO_PM10_IS_GPIO 0
#ifndef BALL_AE14_IS_GPIO
#define BALL_AE14_IS_GPIO 0
#endif
#ifndef PAD_PCM_D7_IS_GPIO
#define PAD_PCM_D7_IS_GPIO BALL_AE14_IS_GPIO
#endif
#ifndef BALL_A9_IS_GPIO
#define BALL_A9_IS_GPIO 0
#endif
#ifndef PAD_ET_TX_CLK_IS_GPIO
#define PAD_ET_TX_CLK_IS_GPIO BALL_A9_IS_GPIO
#endif
#ifndef BALL_AC13_IS_GPIO
#define BALL_AC13_IS_GPIO 0
#endif
#ifndef PAD_PCM_A6_IS_GPIO
#define PAD_PCM_A6_IS_GPIO BALL_AC13_IS_GPIO
#endif
#define PAD_TCON0_IS_GPIO 0
#define PAD_TCON10_IS_GPIO 0
#ifndef BALL_C10_IS_GPIO
#define BALL_C10_IS_GPIO 0
#endif
#ifndef PAD_ET_TXD0_IS_GPIO
#define PAD_ET_TXD0_IS_GPIO BALL_C10_IS_GPIO
#endif
#ifndef BALL_B6_IS_GPIO
#define BALL_B6_IS_GPIO 0
#endif
#ifndef PAD_GPIO15_IS_GPIO
#define PAD_GPIO15_IS_GPIO BALL_B6_IS_GPIO
#endif
#ifndef BALL_AB19_IS_GPIO
#define BALL_AB19_IS_GPIO 0
#endif
#ifndef PAD_PCM_A0_IS_GPIO
#define PAD_PCM_A0_IS_GPIO BALL_AB19_IS_GPIO
#endif
#ifndef BALL_AB13_IS_GPIO
#define BALL_AB13_IS_GPIO 0
#endif
#ifndef PAD_PCM_A5_IS_GPIO
#define PAD_PCM_A5_IS_GPIO BALL_AB13_IS_GPIO
#endif
#ifndef BALL_F5_IS_GPIO
#define BALL_F5_IS_GPIO 0
#endif
#ifndef PAD_GPIO_PM1_IS_GPIO
#define PAD_GPIO_PM1_IS_GPIO BALL_F5_IS_GPIO
#endif
#ifndef BALL_Y25_IS_GPIO
#define BALL_Y25_IS_GPIO 0
#endif
#ifndef PAD_R_ODD5_IS_GPIO
#define PAD_R_ODD5_IS_GPIO BALL_Y25_IS_GPIO
#endif
#define PAD_GPIO_PM6_IS_GPIO 0
#ifndef BALL_AC23_IS_GPIO
#define BALL_AC23_IS_GPIO 0
#endif
#ifndef PAD_G_ODD0_IS_GPIO
#define PAD_G_ODD0_IS_GPIO BALL_AC23_IS_GPIO
#endif
#ifndef BALL_AC6_IS_GPIO
#define BALL_AC6_IS_GPIO 0
#endif
#ifndef PAD_PCM_OE_N_IS_GPIO
#define PAD_PCM_OE_N_IS_GPIO BALL_AC6_IS_GPIO
#endif
#ifndef BALL_AA4_IS_GPIO
#define BALL_AA4_IS_GPIO 0
#endif
#ifndef PAD_TS0_D6_IS_GPIO
#define PAD_TS0_D6_IS_GPIO BALL_AA4_IS_GPIO
#endif
#define PAD_GPIO_PM8_IS_GPIO 0
#ifndef BALL_F9_IS_GPIO
#define BALL_F9_IS_GPIO 0
#endif
#ifndef PAD_GPIO12_IS_GPIO
#define PAD_GPIO12_IS_GPIO BALL_F9_IS_GPIO
#endif
#ifndef BALL_V4_IS_GPIO
#define BALL_V4_IS_GPIO 0
#endif
#ifndef PAD_TS0_D3_IS_GPIO
#define PAD_TS0_D3_IS_GPIO BALL_V4_IS_GPIO
#endif
#ifndef BALL_AA12_IS_GPIO
#define BALL_AA12_IS_GPIO 0
#endif
#ifndef PAD_PCM_A9_IS_GPIO
#define PAD_PCM_A9_IS_GPIO BALL_AA12_IS_GPIO
#endif
#ifndef BALL_AB25_IS_GPIO
#define BALL_AB25_IS_GPIO 0
#endif
#ifndef PAD_G_ODD7_IS_GPIO
#define PAD_G_ODD7_IS_GPIO BALL_AB25_IS_GPIO
#endif
#ifndef BALL_D8_IS_GPIO
#define BALL_D8_IS_GPIO 0
#endif
#ifndef PAD_I2S_OUT_SD_IS_GPIO
#define PAD_I2S_OUT_SD_IS_GPIO BALL_D8_IS_GPIO
#endif
#ifndef BALL_D5_IS_GPIO
#define BALL_D5_IS_GPIO 0
#endif
#ifndef PAD_ET_RXD3_IS_GPIO
#define PAD_ET_RXD3_IS_GPIO BALL_D5_IS_GPIO
#endif
#ifndef BALL_AC15_IS_GPIO
#define BALL_AC15_IS_GPIO 0
#endif
#ifndef PAD_PF_OEZ_IS_GPIO
#define PAD_PF_OEZ_IS_GPIO BALL_AC15_IS_GPIO
#endif
#ifndef BALL_AC22_IS_GPIO
#define BALL_AC22_IS_GPIO 0
#endif
#ifndef PAD_B_ODD5_IS_GPIO
#define PAD_B_ODD5_IS_GPIO BALL_AC22_IS_GPIO
#endif
#ifndef BALL_AE7_IS_GPIO
#define BALL_AE7_IS_GPIO 0
#endif
#ifndef PAD_DDCDC_CK_IS_GPIO
#define PAD_DDCDC_CK_IS_GPIO BALL_AE7_IS_GPIO
#endif
#define PAD_TCON3_IS_GPIO 0
#define PAD_TCON6_IS_GPIO 0
#ifndef BALL_AD20_IS_GPIO
#define BALL_AD20_IS_GPIO 0
#endif
#ifndef PAD_PCM_D0_IS_GPIO
#define PAD_PCM_D0_IS_GPIO BALL_AD20_IS_GPIO
#endif
#ifndef BALL_E1_IS_GPIO
#define BALL_E1_IS_GPIO 0
#endif
#ifndef PAD_DDCDB_DA_IS_GPIO
#define PAD_DDCDB_DA_IS_GPIO BALL_E1_IS_GPIO
#endif
#define PAD_TCON14_IS_GPIO 0
#ifndef BALL_AD21_IS_GPIO
#define BALL_AD21_IS_GPIO 0
#endif
#ifndef PAD_B_ODD1_IS_GPIO
#define PAD_B_ODD1_IS_GPIO BALL_AD21_IS_GPIO
#endif
#ifndef BALL_E4_IS_GPIO
#define BALL_E4_IS_GPIO 0
#endif
#ifndef PAD_SAR3_IS_GPIO
#define PAD_SAR3_IS_GPIO BALL_E4_IS_GPIO
#endif
#ifndef BALL_AA8_IS_GPIO
#define BALL_AA8_IS_GPIO 0
#endif
#ifndef PAD_PCM_IORD_N_IS_GPIO
#define PAD_PCM_IORD_N_IS_GPIO BALL_AA8_IS_GPIO
#endif
#ifndef BALL_C8_IS_GPIO
#define BALL_C8_IS_GPIO 0
#endif
#ifndef PAD_I2S_IN_SD_IS_GPIO
#define PAD_I2S_IN_SD_IS_GPIO BALL_C8_IS_GPIO
#endif
#ifndef BALL_A1_IS_GPIO
#define BALL_A1_IS_GPIO 0
#endif
#ifndef PAD_DDCDA_DA_IS_GPIO
#define PAD_DDCDA_DA_IS_GPIO BALL_A1_IS_GPIO
#endif
#ifndef BALL_C6_IS_GPIO
#define BALL_C6_IS_GPIO 0
#endif
#ifndef PAD_LVSYNC2_IS_GPIO
#define PAD_LVSYNC2_IS_GPIO BALL_C6_IS_GPIO
#endif
#ifndef BALL_C7_IS_GPIO
#define BALL_C7_IS_GPIO 0
#endif
#ifndef PAD_I2S_OUT_BCK_IS_GPIO
#define PAD_I2S_OUT_BCK_IS_GPIO BALL_C7_IS_GPIO
#endif
#ifndef BALL_B7_IS_GPIO
#define BALL_B7_IS_GPIO 0
#endif
#ifndef PAD_I2S_OUT_WS_IS_GPIO
#define PAD_I2S_OUT_WS_IS_GPIO BALL_B7_IS_GPIO
#endif
#ifndef BALL_Y23_IS_GPIO
#define BALL_Y23_IS_GPIO 0
#endif
#ifndef PAD_R_ODD7_IS_GPIO
#define PAD_R_ODD7_IS_GPIO BALL_Y23_IS_GPIO
#endif
#ifndef BALL_AC20_IS_GPIO
#define BALL_AC20_IS_GPIO 0
#endif
#ifndef PAD_PCM_D1_IS_GPIO
#define PAD_PCM_D1_IS_GPIO BALL_AC20_IS_GPIO
#endif
#ifndef BALL_AE25_IS_GPIO
#define BALL_AE25_IS_GPIO 0
#endif
#ifndef PAD_G_ODD4_IS_GPIO
#define PAD_G_ODD4_IS_GPIO BALL_AE25_IS_GPIO
#endif
#ifndef BALL_AB11_IS_GPIO
#define BALL_AB11_IS_GPIO 0
#endif
#ifndef PAD_PCM_A14_IS_GPIO
#define PAD_PCM_A14_IS_GPIO BALL_AB11_IS_GPIO
#endif
#ifndef BALL_C11_IS_GPIO
#define BALL_C11_IS_GPIO 0
#endif
#ifndef PAD_ET_RXD0_IS_GPIO
#define PAD_ET_RXD0_IS_GPIO BALL_C11_IS_GPIO
#endif
#ifndef BALL_E11_IS_GPIO
#define BALL_E11_IS_GPIO 0
#endif
#ifndef PAD_ET_TXD2_IS_GPIO
#define PAD_ET_TXD2_IS_GPIO BALL_E11_IS_GPIO
#endif
#define PAD_TCON16_IS_GPIO 0
#ifndef BALL_AA14_IS_GPIO
#define BALL_AA14_IS_GPIO 0
#endif
#ifndef PAD_PCM_D3_IS_GPIO
#define PAD_PCM_D3_IS_GPIO BALL_AA14_IS_GPIO
#endif
#ifndef BALL_AA19_IS_GPIO
#define BALL_AA19_IS_GPIO 0
#endif
#ifndef PAD_PCM2_CD_N_IS_GPIO
#define PAD_PCM2_CD_N_IS_GPIO BALL_AA19_IS_GPIO
#endif
#ifndef BALL_AA5_IS_GPIO
#define BALL_AA5_IS_GPIO 0
#endif
#ifndef PAD_TS0_CLK_IS_GPIO
#define PAD_TS0_CLK_IS_GPIO BALL_AA5_IS_GPIO
#endif
#define PAD_TCON12_IS_GPIO 0
#ifndef BALL_H6_IS_GPIO
#define BALL_H6_IS_GPIO 0
#endif
#ifndef PAD_UART_RX2_IS_GPIO
#define PAD_UART_RX2_IS_GPIO BALL_H6_IS_GPIO
#endif
#ifndef BALL_AD14_IS_GPIO
#define BALL_AD14_IS_GPIO 0
#endif
#ifndef PAD_PCM_D6_IS_GPIO
#define PAD_PCM_D6_IS_GPIO BALL_AD14_IS_GPIO
#endif
#ifndef BALL_AE16_IS_GPIO
#define BALL_AE16_IS_GPIO 0
#endif
#ifndef PAD_SPI_DO_IS_GPIO
#define PAD_SPI_DO_IS_GPIO BALL_AE16_IS_GPIO
#endif
#ifndef BALL_AB9_IS_GPIO
#define BALL_AB9_IS_GPIO 0
#endif
#ifndef PAD_PF_ALE_IS_GPIO
#define PAD_PF_ALE_IS_GPIO BALL_AB9_IS_GPIO
#endif
#ifndef BALL_C9_IS_GPIO
#define BALL_C9_IS_GPIO 0
#endif
#ifndef PAD_ET_RXD1_IS_GPIO
#define PAD_ET_RXD1_IS_GPIO BALL_C9_IS_GPIO
#endif
#ifndef BALL_AD22_IS_GPIO
#define BALL_AD22_IS_GPIO 0
#endif
#ifndef PAD_B_ODD4_IS_GPIO
#define PAD_B_ODD4_IS_GPIO BALL_AD22_IS_GPIO
#endif
#ifndef BALL_AD19_IS_GPIO
#define BALL_AD19_IS_GPIO 0
#endif
#ifndef PAD_PCM_A2_IS_GPIO
#define PAD_PCM_A2_IS_GPIO BALL_AD19_IS_GPIO
#endif
#ifndef BALL_AB10_IS_GPIO
#define BALL_AB10_IS_GPIO 0
#endif
#ifndef PAD_PCM_WAIT_N_IS_GPIO
#define PAD_PCM_WAIT_N_IS_GPIO BALL_AB10_IS_GPIO
#endif
#ifndef BALL_U22_IS_GPIO
#define BALL_U22_IS_GPIO 0
#endif
#ifndef PAD_TS1_CLK_IS_GPIO
#define PAD_TS1_CLK_IS_GPIO BALL_U22_IS_GPIO
#endif
#ifndef BALL_AB15_IS_GPIO
#define BALL_AB15_IS_GPIO 0
#endif
#ifndef PAD_F_RBZ_IS_GPIO
#define PAD_F_RBZ_IS_GPIO BALL_AB15_IS_GPIO
#endif
#ifndef BALL_AC12_IS_GPIO
#define BALL_AC12_IS_GPIO 0
#endif
#ifndef PAD_PCM_A11_IS_GPIO
#define PAD_PCM_A11_IS_GPIO BALL_AC12_IS_GPIO
#endif
#ifndef BALL_AA13_IS_GPIO
#define BALL_AA13_IS_GPIO 0
#endif
#ifndef PAD_PCM_A4_IS_GPIO
#define PAD_PCM_A4_IS_GPIO BALL_AA13_IS_GPIO
#endif
#ifndef BALL_AB16_IS_GPIO
#define BALL_AB16_IS_GPIO 0
#endif
#ifndef PAD_PF_AD15_IS_GPIO
#define PAD_PF_AD15_IS_GPIO BALL_AB16_IS_GPIO
#endif
#ifndef BALL_AB23_IS_GPIO
#define BALL_AB23_IS_GPIO 0
#endif
#ifndef PAD_R_ODD1_IS_GPIO
#define PAD_R_ODD1_IS_GPIO BALL_AB23_IS_GPIO
#endif
#ifndef BALL_B4_IS_GPIO
#define BALL_B4_IS_GPIO 0
#endif
#ifndef PAD_SAR1_IS_GPIO
#define PAD_SAR1_IS_GPIO BALL_B4_IS_GPIO
#endif
#ifndef BALL_AE18_IS_GPIO
#define BALL_AE18_IS_GPIO 0
#endif
#ifndef PAD_LVSYNC_IS_GPIO
#define PAD_LVSYNC_IS_GPIO BALL_AE18_IS_GPIO
#endif
#ifndef BALL_AA9_IS_GPIO
#define BALL_AA9_IS_GPIO 0
#endif
#ifndef PAD_PCM2_CE_N_IS_GPIO
#define PAD_PCM2_CE_N_IS_GPIO BALL_AA9_IS_GPIO
#endif
#ifndef BALL_A2_IS_GPIO
#define BALL_A2_IS_GPIO 0
#endif
#ifndef PAD_HOTPLUGA_IS_GPIO
#define PAD_HOTPLUGA_IS_GPIO BALL_A2_IS_GPIO
#endif
#ifndef BALL_J6_IS_GPIO
#define BALL_J6_IS_GPIO 0
#endif
#ifndef PAD_UART_TX2_IS_GPIO
#define PAD_UART_TX2_IS_GPIO BALL_J6_IS_GPIO
#endif
#ifndef BALL_T4_IS_GPIO
#define BALL_T4_IS_GPIO 0
#endif
#ifndef PAD_TS0_D1_IS_GPIO
#define PAD_TS0_D1_IS_GPIO BALL_T4_IS_GPIO
#endif
#define PAD_TCON9_IS_GPIO 0
#define PAD_TCON8_IS_GPIO 0
#ifndef BALL_A6_IS_GPIO
#define BALL_A6_IS_GPIO 0
#endif
#ifndef PAD_GPIO14_IS_GPIO
#define PAD_GPIO14_IS_GPIO BALL_A6_IS_GPIO
#endif
#ifndef BALL_AC24_IS_GPIO
#define BALL_AC24_IS_GPIO 0
#endif
#ifndef PAD_G_ODD3_IS_GPIO
#define PAD_G_ODD3_IS_GPIO BALL_AC24_IS_GPIO
#endif
#ifndef BALL_Y24_IS_GPIO
#define BALL_Y24_IS_GPIO 0
#endif
#ifndef PAD_R_ODD6_IS_GPIO
#define PAD_R_ODD6_IS_GPIO BALL_Y24_IS_GPIO
#endif
#ifndef BALL_AA20_IS_GPIO
#define BALL_AA20_IS_GPIO 0
#endif
#ifndef PAD_PCM_CD_N_IS_GPIO
#define PAD_PCM_CD_N_IS_GPIO BALL_AA20_IS_GPIO
#endif
#ifndef BALL_AB22_IS_GPIO
#define BALL_AB22_IS_GPIO 0
#endif
#ifndef PAD_DDCR_CLK_IS_GPIO
#define PAD_DDCR_CLK_IS_GPIO BALL_AB22_IS_GPIO
#endif
#ifndef BALL_AE20_IS_GPIO
#define BALL_AE20_IS_GPIO 0
#endif
#ifndef PAD_PCM_REG_N_IS_GPIO
#define PAD_PCM_REG_N_IS_GPIO BALL_AE20_IS_GPIO
#endif
#ifndef BALL_AD6_IS_GPIO
#define BALL_AD6_IS_GPIO 0
#endif
#ifndef PAD_PF_CE0Z_IS_GPIO
#define PAD_PF_CE0Z_IS_GPIO BALL_AD6_IS_GPIO
#endif
#ifndef BALL_AB14_IS_GPIO
#define BALL_AB14_IS_GPIO 0
#endif
#ifndef PAD_PCM_D4_IS_GPIO
#define PAD_PCM_D4_IS_GPIO BALL_AB14_IS_GPIO
#endif
#ifndef BALL_AC25_IS_GPIO
#define BALL_AC25_IS_GPIO 0
#endif
#ifndef PAD_G_ODD6_IS_GPIO
#define PAD_G_ODD6_IS_GPIO BALL_AC25_IS_GPIO
#endif
#ifndef BALL_AD23_IS_GPIO
#define BALL_AD23_IS_GPIO 0
#endif
#ifndef PAD_B_ODD7_IS_GPIO
#define PAD_B_ODD7_IS_GPIO BALL_AD23_IS_GPIO
#endif
#ifndef BALL_AD17_IS_GPIO
#define BALL_AD17_IS_GPIO 0
#endif
#ifndef PAD_PWM1_IS_GPIO
#define PAD_PWM1_IS_GPIO BALL_AD17_IS_GPIO
#endif
#define PAD_TCON1_IS_GPIO 0
#ifndef BALL_AB18_IS_GPIO
#define BALL_AB18_IS_GPIO 0
#endif
#ifndef PAD_PCM2_RESET_IS_GPIO
#define PAD_PCM2_RESET_IS_GPIO BALL_AB18_IS_GPIO
#endif
#ifndef BALL_A8_IS_GPIO
#define BALL_A8_IS_GPIO 0
#endif
#ifndef PAD_I2S_OUT_MCK_IS_GPIO
#define PAD_I2S_OUT_MCK_IS_GPIO BALL_A8_IS_GPIO
#endif
#ifndef BALL_AB4_IS_GPIO
#define BALL_AB4_IS_GPIO 0
#endif
#ifndef PAD_TS0_D7_IS_GPIO
#define PAD_TS0_D7_IS_GPIO BALL_AB4_IS_GPIO
#endif
#ifndef BALL_AE22_IS_GPIO
#define BALL_AE22_IS_GPIO 0
#endif
#ifndef PAD_B_ODD3_IS_GPIO
#define PAD_B_ODD3_IS_GPIO BALL_AE22_IS_GPIO
#endif
#ifndef BALL_AA25_IS_GPIO
#define BALL_AA25_IS_GPIO 0
#endif
#ifndef PAD_R_ODD2_IS_GPIO
#define PAD_R_ODD2_IS_GPIO BALL_AA25_IS_GPIO
#endif
#define PAD_TCON11_IS_GPIO 0
#ifndef BALL_AE15_IS_GPIO
#define BALL_AE15_IS_GPIO 0
#endif
#ifndef PAD_SPI_CK_IS_GPIO
#define PAD_SPI_CK_IS_GPIO BALL_AE15_IS_GPIO
#endif
#ifndef BALL_AC18_IS_GPIO
#define BALL_AC18_IS_GPIO 0
#endif
#ifndef PAD_PCM_RESET_IS_GPIO
#define PAD_PCM_RESET_IS_GPIO BALL_AC18_IS_GPIO
#endif
#ifndef BALL_AC19_IS_GPIO
#define BALL_AC19_IS_GPIO 0
#endif
#ifndef PAD_PCM_A1_IS_GPIO
#define PAD_PCM_A1_IS_GPIO BALL_AC19_IS_GPIO
#endif
#ifndef BALL_AD13_IS_GPIO
#define BALL_AD13_IS_GPIO 0
#endif
#ifndef PAD_PCM_A7_IS_GPIO
#define PAD_PCM_A7_IS_GPIO BALL_AD13_IS_GPIO
#endif
#ifndef BALL_AA21_IS_GPIO
#define BALL_AA21_IS_GPIO 0
#endif
#ifndef PAD_LCK_IS_GPIO
#define PAD_LCK_IS_GPIO BALL_AA21_IS_GPIO
#endif
#ifndef BALL_E9_IS_GPIO
#define BALL_E9_IS_GPIO 0
#endif
#ifndef PAD_SPDIF_OUT_IS_GPIO
#define PAD_SPDIF_OUT_IS_GPIO BALL_E9_IS_GPIO
#endif
#ifndef BALL_H5_IS_GPIO
#define BALL_H5_IS_GPIO 0
#endif
#ifndef PAD_GPIO_PM3_IS_GPIO
#define PAD_GPIO_PM3_IS_GPIO BALL_H5_IS_GPIO
#endif
#ifndef BALL_D6_IS_GPIO
#define BALL_D6_IS_GPIO 0
#endif
#ifndef PAD_ET_RX_DV_IS_GPIO
#define PAD_ET_RX_DV_IS_GPIO BALL_D6_IS_GPIO
#endif
#ifndef BALL_AE13_IS_GPIO
#define BALL_AE13_IS_GPIO 0
#endif
#ifndef PAD_PCM_A8_IS_GPIO
#define PAD_PCM_A8_IS_GPIO BALL_AE13_IS_GPIO
#endif
#ifndef BALL_AB21_IS_GPIO
#define BALL_AB21_IS_GPIO 0
#endif
#ifndef PAD_DDCR_DAT_IS_GPIO
#define PAD_DDCR_DAT_IS_GPIO BALL_AB21_IS_GPIO
#endif
#define PAD_TCON4_IS_GPIO 0
#ifndef BALL_A7_IS_GPIO
#define BALL_A7_IS_GPIO 0
#endif
#ifndef PAD_I2S_IN_WS_IS_GPIO
#define PAD_I2S_IN_WS_IS_GPIO BALL_A7_IS_GPIO
#endif
#ifndef BALL_E5_IS_GPIO
#define BALL_E5_IS_GPIO 0
#endif
#ifndef PAD_GPIO_PM0_IS_GPIO
#define PAD_GPIO_PM0_IS_GPIO BALL_E5_IS_GPIO
#endif
#ifndef BALL_F11_IS_GPIO
#define BALL_F11_IS_GPIO 0
#endif
#ifndef PAD_SPDIF_IN_IS_GPIO
#define PAD_SPDIF_IN_IS_GPIO BALL_F11_IS_GPIO
#endif
#ifndef BALL_AA23_IS_GPIO
#define BALL_AA23_IS_GPIO 0
#endif
#ifndef PAD_R_ODD4_IS_GPIO
#define PAD_R_ODD4_IS_GPIO BALL_AA23_IS_GPIO
#endif
#ifndef BALL_G5_IS_GPIO
#define BALL_G5_IS_GPIO 0
#endif
#ifndef PAD_GPIO_PM2_IS_GPIO
#define PAD_GPIO_PM2_IS_GPIO BALL_G5_IS_GPIO
#endif
#ifndef BALL_J2_IS_GPIO
#define BALL_J2_IS_GPIO 0
#endif
#ifndef PAD_DDCA_DA_IS_GPIO
#define PAD_DDCA_DA_IS_GPIO BALL_J2_IS_GPIO
#endif
#ifndef BALL_B11_IS_GPIO
#define BALL_B11_IS_GPIO 0
#endif
#ifndef PAD_ET_TXD1_IS_GPIO
#define PAD_ET_TXD1_IS_GPIO BALL_B11_IS_GPIO
#endif
#ifndef BALL_B2_IS_GPIO
#define BALL_B2_IS_GPIO 0
#endif
#ifndef PAD_DDCDA_CK_IS_GPIO
#define PAD_DDCDA_CK_IS_GPIO BALL_B2_IS_GPIO
#endif
#ifndef BALL_AE21_IS_GPIO
#define BALL_AE21_IS_GPIO 0
#endif
#ifndef PAD_B_ODD0_IS_GPIO
#define PAD_B_ODD0_IS_GPIO BALL_AE21_IS_GPIO
#endif
#define PAD_GPIO_PM5_IS_GPIO 0

// chip capbilities
#define CHIP_HAS_BT656_IN          0
#define CHIP_HAS_TS0               1 // BALL# AA5 Y5 W5 R4 T4 U4 V4 W4 Y4 AA4 AB4
#define CHIP_HAS_TS1               1 // BALL# U22 W22 Y22 V22
#define CHIP_HAS_LVDS_10BIT_X2     1
#define CHIP_HAS_LVDS_8BIT_X2      1
#define CHIP_HAS_LVDS_8BIT_X1      1
#define CHIP_HAS_CIMAX             0
#define CHIP_HAS_USB_P0            1
#define CHIP_HAS_USB_P1            1
#define CHIP_HAS_USBVBUS0          0
#define CHIP_HAS_USBVBUS1          0
#define CHIP_HAS_UART1_MODE1       1 // BALL# AB18 AA19
#define CHIP_HAS_UART1_MODE2       1 // BALL# E7 C6
#define CHIP_HAS_UART1_MODE3       1 // BALL# A6 B6
#define CHIP_HAS_PCMCIA1           1
#define CHIP_HAS_PCMCIA2           1
#define CHIP_HAS_PCMCIA            1
#define CHIP_HAS_UART2_MODE1       1 // BALL# H6 J6
#define CHIP_HAS_UART2_MODE2       1 // BALL# D11 F8
#define CHIP_HAS_UART2_MODE3       1 // BALL# F9 F10
#define CHIP_HAS_RGB_A             1 // BALL# P2 R3 R1 P3
#define CHIP_HAS_RGB_B             1 // BALL# L1 L3 K1 L2
#define CHIP_HAS_RGB_C             1 // BALL# V1 V2 U1 V3
#define CHIP_HAS_HDMI_A            1 // BALL# F2 F1 G3 G2 G1 H3 H2 H1 G4 A1 B2
#define CHIP_HAS_HDMI_B            1 // BALL# B1 C3 C2 C1 D3 D2 D1 E3 G4 E1 F3
#define CHIP_HAS_HDMI_C            1 // BALL# AC8 AD8 AE8 AC9 AD9 AE9 AD10 AE10 G4 AD7 AE7
#define CHIP_HAS_SPI_CZ1           1 // BALL# H6 AC14 B8
#define CHIP_HAS_SPI_CZ2           1 // BALL# J6 AD14 E7
#define CHIP_HAS_SPI_CZ3           1 // BALL# A7 C6 AE19
#define CHIP_HAS_DDCR              1 // BALL# AB21 AB22
#define CHIP_HAS_DDCR2             1 // BALL# D11 F8
#define CHIP_HAS_TCON              0
#define CHIP_HAS_ET_MODE0_MII      1 // BALL# D10 C5 D5 E11 D7 D9 E8 E10 D6 B10 A9 C10 B11 C11 C9 A11 A10 B9
#define CHIP_HAS_ET_MODE0_RMII     1 // BALL# C10 B11 C11 C9 A11 A9 B10 A10 B9
#define CHIP_HAS_ET_MODE1_MII      1 // BALL# D10 C5 D5 E11 D7 D9 E8 E10 D6 A7 B8 C8 A11 B11 C10 B10 A9 C11
#define CHIP_HAS_ET_MODE1_RMII     1 // BALL# A7 B8 C8 A11 B11 C10 B10 A9 C11
#define CHIP_HAS_ET_MODE2_MII      1 // BALL# D10 C5 D5 E11 D7 D9 E8 E10 D6 AE19 AD19 AE20 AC19 AB19 AD20 AC20 AB20 AC18
#define CHIP_HAS_ET_MODE2_RMII     1 // BALL# AE19 AD19 AE20 AC19 AB19 AD20 AC20 AB20 AC18
#define CHIP_HAS_I2S_IN            1 // BALL# A7 B8 C8
#define CHIP_HAS_I2S_OUT           1 // BALL# B7 C7 B7 D8
#define CHIP_HAS_SPDIF_IN          1 // BALL# F11
#define CHIP_HAS_SPDIF_OUT         1 // BALL# E9
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
#define CHIP_FAMILY_TYPE           CHIP_FAMILY_S7

#endif /* _MSD3303GX_H_ */
