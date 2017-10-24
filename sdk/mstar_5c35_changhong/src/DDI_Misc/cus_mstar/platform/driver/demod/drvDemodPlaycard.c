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
#include "Board.h"

#if 0

#include "MsCommon.h"
#include "HbCommon.h"
#include "drvDemod.h"
#include "drvDemodNull.h"


MS_BOOL MDrv_Demod_PLAYCARD_Init(MS_U8 u8DemodIndex)
{
    return TRUE;
}

MS_BOOL MDrv_Demod_PLAYCARD_Open(MS_U8 u8DemodIndex)
{
    return TRUE;
}

MS_BOOL MDrv_Demod_PLAYCARD_Close(MS_U8 u8DemodIndex)
{
    return TRUE;
}

MS_BOOL MDrv_Demod_PLAYCARD_Reset(MS_U8 u8DemodIndex)
{
    return TRUE;
}

MS_BOOL MDrv_Demod_PLAYCARD_TsOut(MS_U8 u8DemodIndex, MS_BOOL bEnable)
{
    return TRUE;
}

MS_BOOL MDrv_Demod_PLAYCARD_PowerOnOff(MS_U8 u8DemodIndex, MS_BOOL bPowerOn)
{
    return TRUE;
}

MS_BOOL MDrv_Demod_PLAYCARD_SetBW(MS_U8 u8DemodIndex, MS_U32 u32BW)
{
    return TRUE;
}

MS_BOOL MDrv_Demod_PLAYCARD_GetBW(MS_U8 u8DemodIndex, MS_U32 *pu32BW)
{
    return TRUE;
}

MS_BOOL MDrv_Demod_PLAYCARD_GetLock(MS_U8 u8DemodIndex, MS_BOOL *pbLock)
{
    *pbLock = TRUE;
    return TRUE;
}

MS_BOOL MDrv_Demod_PLAYCARD_GetSNR(MS_U8 u8DemodIndex, MS_U32 *pu32SNR)
{
    *pu32SNR = 35;
    return TRUE;
}

MS_BOOL MDrv_Demod_PLAYCARD_GetBER(MS_U8 u8DemodIndex, float *pfBER)
{
    *pfBER = 0;
    return TRUE;
}

MS_BOOL MDrv_Demod_PLAYCARD_GetPWR(MS_U8 u8DemodIndex, MS_S32 *ps32Signal)
{
    *ps32Signal = -100;
    return TRUE;
}

MS_BOOL MDrv_Demod_PLAYCARD_Config(MS_U8 u8DemodIndex, MS_U8 *pRegParam)
{
    return TRUE;
}

MS_BOOL MDrv_Demod_PLAYCARD_GetParam(MS_U8 u8DemodIndex, DEMOD_MS_FE_CARRIER_PARAM* pParam)
{
    return TRUE;
}

MS_BOOL MDrv_Demod_PLAYCARD_Restart(MS_U8 u8DemodIndex, DEMOD_MS_FE_CARRIER_PARAM* pParam)
{
    return TRUE;
}

MS_BOOL MDrv_Demod_PLAYCARD_SetMode(MS_U8 u8DemodIndex, Demod_Mode* pMode)
{
    return TRUE;
}


DEMODTAB_ENTRY(demod_entry_DEMOD_PLAYCARD, "DEMOD_PLAYCARD", DEMOD_PLAYCARD,
            MDrv_Demod_PLAYCARD_Init,
            MDrv_Demod_PLAYCARD_Open,
            MDrv_Demod_PLAYCARD_Close,
            MDrv_Demod_PLAYCARD_Reset,
            MDrv_Demod_PLAYCARD_TsOut,
            MDrv_Demod_PLAYCARD_PowerOnOff,
            MDrv_Demod_PLAYCARD_SetBW,
            MDrv_Demod_PLAYCARD_GetBW,
            MDrv_Demod_PLAYCARD_GetLock,
            MDrv_Demod_PLAYCARD_GetSNR,
            MDrv_Demod_PLAYCARD_GetBER,
            MDrv_Demod_PLAYCARD_GetPWR,
            MDrv_Demod_PLAYCARD_Config,
            MDrv_Demod_PLAYCARD_GetParam,
            MDrv_Demod_PLAYCARD_Restart,
            MDrv_Demod_PLAYCARD_SetMode,
            MDrv_Demod_null_SetOutoutPath,
            MDrv_Demod_null_GetOutoutPath,//MDrv_Demod_GetOutoutPath,
            MDrv_Demod_null_I2C_ByPass,
            MDrv_Demod_null_SetCurrentDemodType,
            MDrv_Demod_null_GetCurrentDemodType,
            MDrv_Demod_null_GetPlpBitMap,
            MDrv_Demod_null_GetPlpGroupID,
            MDrv_Demod_null_SetPlpGroupID,
            MDrv_Demod_null_SetTsSerial,
            MDrv_Demod_null_TPSGetLock,
            MDrv_Demod_null_MPEGGetLock,
            MDrv_Demod_null_GetRollOff,
            MDrv_Demod_null_GetRFOffset,
            MDrv_Demod_null_BlindScan_Start,
            MDrv_Demod_null_BlindScan_NextFreq,
            MDrv_Demod_null_BlindScan_WaitCurFreqFinished,
            MDrv_Demod_null_BlindScan_Cancel,
            MDrv_Demod_null_BlindScan_End,
            MDrv_Demod_null_BlindScan_GetChannel,
            MDrv_Demod_null_BlindScan_GetCurrentFreq,
            MDrv_Demod_null_DiSEqC_SetTone,
            MDrv_Demod_null_DiSEqC_SetLNBOut,
            MDrv_Demod_null_DiSEqC_GetLNBOut,
            MDrv_Demod_null_DiSEqC_Set22kOnOff,
            MDrv_Demod_null_DiSEqC_Get22kOnOff,
            MDrv_Demod_null_DiSEqC_SendCmd
);


#endif // (FRONTEND_DEMOD_TYPE == DEMOD_PLAYCARD)
