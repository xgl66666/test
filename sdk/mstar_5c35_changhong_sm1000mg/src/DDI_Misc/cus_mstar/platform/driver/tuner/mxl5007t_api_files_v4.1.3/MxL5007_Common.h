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
 * AUTHOR             : Kyle Huang
 * DATE CREATED       : May 05, 2008
 *
 * DESCRIPTION        :
 *
 *******************************************************************************
 *                Copyright (c) 2006, MaxLinear, Inc.
 ******************************************************************************/

#ifndef __MXL5007_COMMON_H
#define __MXL5007_COMMON_H



/******************************************************************************
*						User-Defined Types (Typedefs)
******************************************************************************/


/****************************************************************************
*       Imports and definitions for WIN32
****************************************************************************/
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

/****************************************************************************\
*      Imports and definitions for non WIN32 platforms                   *
\****************************************************************************/
/*
// create a boolean
#ifndef __boolean__
#define __boolean__
typedef enum {FALSE=0,TRUE} BOOL;
#endif //boolean
*/
#ifndef BOOL
#define BOOL	MS_U8
#endif

/****************************************************************************\
*          Definitions for all platforms					                 *
\****************************************************************************/
#ifndef NULL
#define NULL (void*)0
#endif



/******************************/
/*	MxL5007 Err message  	  */
/******************************/
typedef enum{
	MxL_OK			=   0,
	MxL_ERR_INIT		=   1,
	MxL_ERR_RFTUNE		=   2,
	MxL_ERR_SET_REG		=   3,
	MxL_ERR_GET_REG		=	4,
	MxL_ERR_OTHERS		=   10
}MxL_ERR_MSG;

/******************************/
/*	MxL5007 Chip verstion     */
/******************************/
typedef enum{
	MxL_UNKNOWN_ID		= 0x00,
	MxL_5007T_V4		= 0x41,
	MxL_GET_ID_FAIL		= 0xFF
}MxL5007_ChipVersion;


/******************************************************************************
    CONSTANTS
******************************************************************************/

#ifndef MHz
	#define MHz 1000000
#endif

#define MAX_ARRAY_SIZE 100


// Enumeration of Mode
// Enumeration of Mode
typedef enum
{
	MxL_MODE_ISDBT = 0,
	MxL_MODE_DVBT = 1,
	MxL_MODE_ATSC = 2,
	MxL_MODE_CABLE = 0x10
} MxL5007_Mode ;

typedef enum
{
	MxL_IF_4_MHZ	  = 4000000,
	MxL_IF_4_5_MHZ	  = 4500000,
	MxL_IF_4_57_MHZ	  =	4570000,
	MxL_IF_5_MHZ	  =	5000000,
	MxL_IF_5_38_MHZ	  =	5380000,
	MxL_IF_6_MHZ	  =	6000000,
	MxL_IF_6_28_MHZ	  =	6280000,
	MxL_IF_9_1915_MHZ =	9191500,
	MxL_IF_35_25_MHZ  = 35250000,
	MxL_IF_36_15_MHZ  = 36150000,
	MxL_IF_44_MHZ	  = 44000000
} MxL5007_IF_Freq ;

typedef enum
{
	MxL_XTAL_16_MHZ		= 16000000,
	MxL_XTAL_20_MHZ		= 20000000,
	MxL_XTAL_20_25_MHZ	= 20250000,
	MxL_XTAL_20_48_MHZ	= 20480000,
	MxL_XTAL_24_MHZ		= 24000000,
	MxL_XTAL_25_MHZ		= 25000000,
	MxL_XTAL_25_14_MHZ	= 25140000,
	MxL_XTAL_27_MHZ		= 27000000,
	MxL_XTAL_28_8_MHZ	= 28800000,
	MxL_XTAL_32_MHZ		= 32000000,
	MxL_XTAL_40_MHZ		= 40000000,
	MxL_XTAL_44_MHZ		= 44000000,
	MxL_XTAL_48_MHZ		= 48000000,
	MxL_XTAL_49_3811_MHZ = 49381100
} MxL5007_Xtal_Freq ;

typedef enum
{
	MxL_BW_6MHz = 6,
	MxL_BW_7MHz = 7,
	MxL_BW_8MHz = 8
} MxL5007_BW_MHz;

typedef enum
{
	MxL_NORMAL_IF = 0 ,
	MxL_INVERT_IF

} MxL5007_IF_Spectrum ;

typedef enum
{
	MxL_LT_DISABLE = 0 ,
	MxL_LT_ENABLE

} MxL5007_LoopThru ;

typedef enum
{
	MxL_CLKOUT_DISABLE = 0 ,
	MxL_CLKOUT_ENABLE

} MxL5007_ClkOut;

typedef enum
{
	MxL_CLKOUT_AMP_0 = 0 ,
	MxL_CLKOUT_AMP_1,
	MxL_CLKOUT_AMP_2,
	MxL_CLKOUT_AMP_3,
	MxL_CLKOUT_AMP_4,
	MxL_CLKOUT_AMP_5,
	MxL_CLKOUT_AMP_6,
	MxL_CLKOUT_AMP_7
} MxL5007_ClkOut_Amp;

typedef enum
{
	MxL_I2C_ADDR_96 = 96 ,
	MxL_I2C_ADDR_97 = 97 ,
	MxL_I2C_ADDR_98 = 98 ,
	MxL_I2C_ADDR_99 = 99
} MxL5007_I2CAddr ;

//
// MxL5007 TunerConfig Struct
//
typedef struct _MxL5007_TunerConfigS
{
	MxL5007_I2CAddr		I2C_Addr;
	MxL5007_Mode		Mode;
	SINT32				IF_Diff_Out_Level;
	MxL5007_Xtal_Freq	Xtal_Freq;
	MxL5007_IF_Freq	    IF_Freq;
	MxL5007_IF_Spectrum IF_Spectrum;
	MxL5007_ClkOut		ClkOut_Setting;
    MxL5007_ClkOut_Amp	ClkOut_Amp;
	MxL5007_BW_MHz		BW_MHz;
	MS_U32				RF_Freq_Hz;
} MxL5007_TunerConfigS;




#endif /* __MXL5007_COMMON_H__*/




