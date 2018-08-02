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
// Copyright (c) 2006-2008 MStar Semiconductor, Inc.
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

////////////////////////////////////////////////////////////////////////////////
//
/// @file MSB122x.c
/// @brief MSB122x DVBT, VIF Controller Interface
/// @author MStar Semiconductor, Inc.
//
////////////////////////////////////////////////////////////////////////////////




#ifndef _DIB8096_C_
#define _DIB8096_C_

#include "MsCommon.h"
#include "Board.h"


#if IF_THIS_DEMOD_INUSE(DIBCOM_DIB8096_DEMOD)



#include "drvIIC.h"
#include "drvGPIO.h"
#include "drvDemod.h"
#include "drvDemodDIB8096.h"
#include "dibcom809x_api_2_layer_simple.h"
#include "dibcom8090.h"

#define DBG_DIB8096(format, args...) //printf("%04d %s " format, __LINE__, __FUNCTION__, ##args)

#if 0
MS_BOOL bIsPowerOnInitialized = FALSE;


double pow(double a, double b) {
    int tmp = (*(1 + (int *)&a));
    int tmp2 = (int)(b * (tmp - 1072632447) + 1072632447);
    double p = 0.0;
    *(1 + (int * )&p) = tmp2;
    return p;
}


double log10(double flt_x)
{
	MS_U32		u32_temp = 1;
	MS_U8		indx = 0;

	do {
		u32_temp = u32_temp << 1;
		if (flt_x < (double)u32_temp)
			break;
	}while (++indx < 32);

	// 10*log10(X) ~= 0.3*N, when X ~= 2^N
	return (double)0.3 * indx;
}
#endif


MS_BOOL MDrv_Demod_Init(void)
{
    if(dib_init() == DIB_RETURN_SUCCESS)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

MS_BOOL MDrv_Demod_Open(void)
{
    return TRUE;
}

MS_BOOL MDrv_Demod_Close(void)
{
    return TRUE;
}

MS_BOOL MDrv_Demod_Reset(void)
{
    dib0090_reset();
    return TRUE;
}

MS_BOOL MDrv_Demod_TsOut(MS_BOOL bEnable)
{
    return TRUE;
}

MS_BOOL MDrv_Demod_PowerOnOff(MS_BOOL bPowerOn)
{
    return TRUE;
}

MS_BOOL MDrv_Demod_SetBW(MS_U32 u32BW)
{
    return TRUE;
}

MS_BOOL MDrv_Demod_GetBW(MS_U32 *pu32BW)
{
    return TRUE;
}

MS_BOOL MDrv_Demod_GetLock(MS_BOOL *pbLock)
{
    MS_U8 u8Return = dib_lockstatus();

    *pbLock = u8Return;
    return TRUE;
}

MS_BOOL MDrv_Demod_GetSNR(MS_U32 *pu32SNR)
{
    MS_U16  u8Lock = dib_lockstatus(), u16Signal = dib_signal_quality();

    DBG_DIB8096("%d %d\n", u8Lock, u16Signal);

    if( u8Lock != 0 && u16Signal > 0)
    {
        *pu32SNR = 80;
    }
    else
    {
        *pu32SNR = 0;
    }

    return TRUE;
}

MS_BOOL MDrv_Demod_GetBER(float *pfBER)
{
    *pfBER = 0;
    return TRUE;
}

MS_BOOL MDrv_Demod_GetPWR(MS_S32 *ps32Signal)
{
    MS_U8 u8Return = dib_signal_strength();

    *ps32Signal = u8Return;
    return TRUE;
}

MS_BOOL MDrv_Demod_Config(MS_U8 *pRegParam)
{
    return TRUE;
}

MS_BOOL MDrv_Demod_GetParam(DEMOD_MS_FE_CARRIER_PARAM* pParam)
{
    return TRUE;
}

MS_BOOL MDrv_Demod_Restart(DEMOD_MS_FE_CARRIER_PARAM* pParam)
{

    dib_tune(pParam->u32Frequency,6);

    return TRUE;
}

MS_BOOL MDrv_Demod_SetMode(Demod_Mode* pMode)
{
    return TRUE;
}



MS_U16 DTV_GetSignalQuality(void)
{
    MS_U16  u8Lock = dib_lockstatus(), u16Signal = dib_signal_quality();

    DBG_DIB8096("%d %d\n", u8Lock, u16Signal);

    if ( u8Lock )
    {
//        u16Signal = dib_signal_quality();
    }
    else
    {
        u16Signal = 0;
    }

    return u16Signal;
}




#if 0
/*@ <Operation ID=Im17018142m1221763cc7cmm46c6> @*/
MS_BOOL Power_On_Initialization(void)
{
    MS_BOOL bStatus = TRUE;
    MS_U8 u8RetryCount = 0;
    DBG_DIB8096("\n");
    if (bIsPowerOnInitialized == TRUE)
    {
        return TRUE;
    }
    else
    {
        bIsPowerOnInitialized = TRUE;
    }


    do
    {
        if(bStatus == FALSE)
        {
            u8RetryCount++;
            bStatus=TRUE;
        }
#ifdef PAD_RESET_DEMOD
        DBG_DIB8096("Reset Demodulator after program firmware\n");
        mdrv_gpio_set_low(PAD_RESET_DEMOD);
        DBG_DIB8096("mdrv_gpio_set_low(%d)\n", PAD_RESET_DEMOD);

        MsOS_DelayTask(1000);
        mdrv_gpio_set_high(PAD_RESET_DEMOD);
        DBG_DIB8096("mdrv_gpio_set_high(%d)\n", PAD_RESET_DEMOD);

        MDrv_IIC_Init();

#endif
        bStatus = TRUE;

    }while((u8RetryCount < 7) && (bStatus == FALSE));

    if(bStatus)
    {
        DBG_DIB8096(" SUCCESS\n");
    }
    else
    {
        DBG_DIB8096(" FAIL\n");
    }

    return bStatus;
}
#endif



#endif

#endif

