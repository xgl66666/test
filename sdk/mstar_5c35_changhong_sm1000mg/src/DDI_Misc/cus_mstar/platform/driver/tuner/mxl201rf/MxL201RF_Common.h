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
/*******************************************************************************
 *
 * FILE NAME          : MxL_Common.h
 *
 * AUTHOR             : Howard Chan / Patrick Dizon
 * DATE CREATED       : June 4, 2010
 *
 * DESCRIPTION        : MxL201RF V6 Type enumeration & struct declaration
 *
 *******************************************************************************
 *                Copyright (c) 2010, MaxLinear, Inc.
 ******************************************************************************/

#ifndef __MxL201RF_COMMON_H
#define __MxL201RF_COMMON_H

//#include "MxL_User_Define.h"

#ifndef UINT8
#define UINT8       unsigned char
#endif
#ifndef UINT16
#define UINT16      unsigned short
#endif
#ifndef UINT32
#define UINT32      unsigned int
#endif
#ifndef SINT8
#define SINT8       char
#endif
#ifndef SINT16
#define SINT16      short
#endif
#ifndef SINT32
#define SINT32      int
#endif
#ifndef REAL32
#define REAL32      float
#endif

#define MxlBOOL    unsigned char



/******************************/
/*    MxL201RF Err message        */
/******************************/
typedef enum{
    MxL_OK                    =   0x0,
    MxL_ERR_INIT            =   0x1,
    MxL_ERR_RFTUNE            =   0x2,
    MxL_ERR_SET_REG            =   0x3,
    MxL_ERR_GET_REG            =   0x4,
    MxL_ERR_MODE            =   0x10,
    MxL_ERR_IF_FREQ            =   0x11,
    MxL_ERR_XTAL_FREQ        =   0x12,
    MxL_ERR_BANDWIDTH        =   0x13,
    MxL_GET_ID_FAIL            =   0x14,
    MxL_ERR_OTHERS            =   0x0A
}MxL_ERR_MSG;

/******************************/
/*    MxL201RF Chip verstion     */
/******************************/
typedef enum{
    MxL_UNKNOWN_ID        = 0x00,
    MxL_201RF_ES4        = 0x06
}MxL201RF_ChipVersion;


/******************************************************************************
    CONSTANTS
******************************************************************************/

#ifndef MHz
    #define MHz 1000000
#endif

#define MAX_ARRAY_SIZE 100


/* Enumeration of Mode */
typedef enum
{
    MxL_MODE_DVBT    = 0x01,
    MxL_MODE_ATSC    = 0x02,
    MxL_MODE_CAB_OPT1 = 0x10,
    MxL_MODE_CAB_STD = 0x11  /*Cable "Standard" setting */
} MxL201RF_Mode ;

/* Enumeration of Acceptable IF Frequencies */
typedef enum
{
    MxL_IF_4_MHZ      = 4000000,
    MxL_IF_4_5_MHZ      = 4500000,
    MxL_IF_4_57_MHZ      =    4570000,
    MxL_IF_5_MHZ      =    5000000,
    MxL_IF_5_38_MHZ      =    5380000,
    MxL_IF_6_MHZ      =    6000000,
    MxL_IF_6_28_MHZ      =    6280000,
    MxL_IF_7_2_MHZ    = 7200000,
    MxL_IF_35_25_MHZ  = 35250000,
    MxL_IF_36_MHZ      = 36000000,
    MxL_IF_36_15_MHZ  = 36150000,
    MxL_IF_44_MHZ      = 44000000
} MxL201RF_IF_Freq ;

/* Enumeration of Acceptable Crystal Frequencies */
typedef enum
{
    MxL_XTAL_16_MHZ        = 16000000,
    MxL_XTAL_20_MHZ        = 20000000,
    MxL_XTAL_20_25_MHZ    = 20250000,
    MxL_XTAL_20_48_MHZ    = 20480000,
    MxL_XTAL_24_MHZ        = 24000000,
    MxL_XTAL_25_MHZ        = 25000000,
    MxL_XTAL_25_14_MHZ    = 25140000,
    MxL_XTAL_28_8_MHZ    = 28800000,
    MxL_XTAL_32_MHZ        = 32000000,
    MxL_XTAL_40_MHZ        = 40000000,
    MxL_XTAL_44_MHZ        = 44000000,
    MxL_XTAL_48_MHZ        = 48000000,
    MxL_XTAL_49_3811_MHZ = 49381100
} MxL201RF_Xtal_Freq ;

/* Enumeration of Acceptable IF Bandwidths */
typedef enum
{
    MxL_BW_6MHz = 6,
    MxL_BW_7MHz = 7,
    MxL_BW_8MHz = 8
} MxL201RF_BW_MHz;

/* Enumeration of Inverted/Normal IF Spectrum */
typedef enum
{
    MxL_NORMAL_IF = 0 ,
    MxL_INVERT_IF

} MxL201RF_IF_Spectrum ;

/* Enumeration of Clock out Enable/Disable */
typedef enum
{
    MxL_CLKOUT_DISABLE = 0 ,
    MxL_CLKOUT_ENABLE

} MxL201RF_ClkOut;

/* Enumeration of Clock out Amplitude */
typedef enum
{
    MxL_CLKOUT_AMP_0 = 0 ,     /* min Clock out Amplitude */
    MxL_CLKOUT_AMP_1,
    MxL_CLKOUT_AMP_2,
    MxL_CLKOUT_AMP_3,
    MxL_CLKOUT_AMP_4,
    MxL_CLKOUT_AMP_5,
    MxL_CLKOUT_AMP_6,
    MxL_CLKOUT_AMP_7,
    MxL_CLKOUT_AMP_8,
    MxL_CLKOUT_AMP_9,
    MxL_CLKOUT_AMP_10,
    MxL_CLKOUT_AMP_11,
    MxL_CLKOUT_AMP_12,
    MxL_CLKOUT_AMP_13,
    MxL_CLKOUT_AMP_14,
    MxL_CLKOUT_AMP_15  /* max Clock out Amplitude */

} MxL201RF_ClkOut_Amp;

/* Enumeration of I2C Addresses */
typedef enum
{
    MxL_I2C_ADDR_96 = 96 ,
    MxL_I2C_ADDR_97 = 97 ,
    MxL_I2C_ADDR_98 = 98 ,
    MxL_I2C_ADDR_99 = 99
} MxL201RF_I2CAddr ;

/* Enumeration of Acceptable Crystal Capacitor values */
typedef enum
{
    MxL_XTAL_CAP_0_PF = 0,
    MxL_XTAL_CAP_1_PF = 1,
    MxL_XTAL_CAP_2_PF = 2,
    MxL_XTAL_CAP_3_PF = 3,
    MxL_XTAL_CAP_4_PF = 4,
    MxL_XTAL_CAP_5_PF = 5,
    MxL_XTAL_CAP_6_PF = 6,
    MxL_XTAL_CAP_7_PF = 7,
    MxL_XTAL_CAP_8_PF = 8,
    MxL_XTAL_CAP_9_PF = 9,
    MxL_XTAL_CAP_10_PF = 10,
    MxL_XTAL_CAP_11_PF = 11,
    MxL_XTAL_CAP_12_PF = 12,
    MxL_XTAL_CAP_13_PF = 13,
    MxL_XTAL_CAP_14_PF = 14,
    MxL_XTAL_CAP_15_PF = 15,
    MxL_XTAL_CAP_16_PF = 16,
    MxL_XTAL_CAP_17_PF = 17,
    MxL_XTAL_CAP_18_PF = 18,
    MxL_XTAL_CAP_19_PF = 19,
    MxL_XTAL_CAP_20_PF = 20,
    MxL_XTAL_CAP_21_PF = 21,
    MxL_XTAL_CAP_22_PF = 22,
    MxL_XTAL_CAP_23_PF = 23,
    MxL_XTAL_CAP_24_PF = 24,
    MxL_XTAL_CAP_25_PF = 25
}    MxL201RF_Xtal_Cap;

/* SHFLUT */
typedef struct
{
    UINT32    Ch_Freq_Hz;
    UINT8    Reg1_Val;
    UINT8    Reg2_Val;
} ShiftLUT;



/* =====================
   MxL201RF Tuner Struct
   ===================== */
typedef struct _MxL201RF_TunerConfigS
{
    MxL201RF_I2CAddr    I2C_Addr;
    MxL201RF_Mode        Mode;
    MxL201RF_Xtal_Freq    Xtal_Freq;
    MxL201RF_IF_Freq    IF_Freq;
    MxL201RF_IF_Spectrum IF_Spectrum;
    MxL201RF_ClkOut        ClkOut_Setting;
    MxL201RF_ClkOut_Amp    ClkOut_Amp;
    MxL201RF_BW_MHz        BW_MHz;
    UINT32                RF_Freq_Hz;
    MxL201RF_Xtal_Cap    Xtal_Cap;
} MxL201RF_TunerConfigS ;

#endif /* __MxL201RF_COMMON_H__*/

