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
#ifndef _DRVTUNER_STV6111_H_
#define _DRVTUNER_STV6111_H_
#include <math.h>
#include "drvIIC.h"
#include "drvTuner.h"
#include "drvTunerNull.h"
#include <string.h>

#define STV6111_MAX_INPUT_FREQ 2150 //Mhz
#define STV6111_MIN_INPUT_FREQ 950  //Mhz

#define FXTAL 30000000 //hz 

/*CTRL1*/
#define RSTV6111_CTRL1  0x0000
#define FSTV6111_K  0x000000f8
#define FSTV6111_RDIV  0x00000004
#define FSTV6111_ODIV  0x00000003

/*CTRL2*/
#define RSTV6111_CTRL2  0x0001
#define FSTV6111_BB_MAG  0x000100c0
#define FSTV6111_BB_MODE  0x00010030
#define FSTV6111_DCLOOP_OFF  0x00010008
#define FSTV6111_GPOUT  0x00010004
#define FSTV6111_BB_WBD  0x00010002
#define FSTV6111_RX_ON  0x00010001

/*CTRL3*/
#define RSTV6111_CTRL3  0x0002
#define FSTV6111_LNA_SEL_GAIN  0x00020080
#define FSTV6111_LNA_ADC_OFF  0x00020040
#define FSTV6111_LNA_ADC_GET  0x00020020
#define FSTV6111_LNA_AGC  0x0002001f

/*CTRL4*/
#define RSTV6111_CTRL4  0x0003
#define FSTV6111_LO_DIV  0x00030080
#define FSTV6111_LNA_AGC_MODE  0x00030060
#define FSTV6111_LNA_AGC_LOW  0x00030010
#define FSTV6111_LNA_AGC_HIGH  0x00030008
#define FSTV6111_LNA_AGC_REF  0x00030007

/*CTRL5*/
#define RSTV6111_CTRL5  0x0004
#define FSTV6111_N_LSB  0x000400ff

/*CTRL6*/
#define RSTV6111_CTRL6  0x0005
#define FSTV6111_F_L  0x000500fe
#define FSTV6111_N_MSB  0x00050001

/*CTRL7*/
#define RSTV6111_CTRL7  0x0006
#define FSTV6111_F_M  0x000600ff

/*CTRL8*/
#define RSTV6111_CTRL8  0x0007
#define FSTV6111_PLL_ICP  0x000700e0
#define FSTV6111_VCO_AMP  0x00070018
#define FSTV6111_F_H  0x00070007

/*CTRL9*/
#define RSTV6111_CTRL9  0x0008
#define FSTV6111_BB_FILT_SEL  0x000800fc
#define FSTV6111_PLL_SD_ON  0x00080002
#define FSTV6111_PLL_LOCK  0x00080001

/*CTRL10*/
#define RSTV6111_CTRL10  0x0009
#define FSTV6111_ID  0x000900e0
#define FSTV6111_BB_CAL_START  0x00090008
#define FSTV6111_VCO_CAL_START  0x00090004
#define FSTV6111_TCAL  0x00090003

/*CTRL11*/
#define RSTV6111_CTRL11  0x000a
#define FSTV6111_SPARE  0x000a00ff
extern int IIP3_MODE;

typedef struct
{
   MS_S32 STV6111_Mutex;
   MS_U8 I2C_SalveID;
   MS_U8 Initial_done_flag;
   MS_U32 satellite_bw;
   MS_U32 pre_satellite_bw;
   TUNER_MS_INIT_PARAM Init_Param;
}STV6111_Handle;

typedef struct
{
	MS_U8 RegAddr;
	MS_U8 Data[50];
	MS_U8 Len;
}STV6111_I2C_LEN_TYPE;

typedef enum 
{
	RFAGC_MODE=0,
	CAHNNEL_AGC_MODE
}STV6111_AGC_MODE;

#endif
