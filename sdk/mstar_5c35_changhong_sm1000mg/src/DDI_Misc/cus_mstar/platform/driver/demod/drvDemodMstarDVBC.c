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

#if  0
#include "MsCommon.h"
#include "drvDMD.h"
#include "drvDemod.h"

MS_BOOL MDrv_Demod_Init(void)
{
    return MDrv_Dmd_Init();
}

MS_BOOL MDrv_Demod_Open(void)
{
    return MDrv_Dmd_Open();
}

MS_BOOL MDrv_Demod_Close(void)
{
    return MDrv_Dmd_Close();
}

MS_BOOL MDrv_Demod_Reset(void)
{
    return MDrv_Dmd_Reset();
}

MS_BOOL MDrv_Demod_TsOut(MS_BOOL bEnable)
{
    return MDrv_Dmd_TsOut(bEnable);
}

MS_BOOL MDrv_Demod_PowerOnOff(MS_BOOL bPowerOn)
{
    return MDrv_Dmd_PowerOnOff(bPowerOn);
}

MS_BOOL MDrv_Demod_SetBW(MS_U32 u32BW)
{
    return MDrv_Dmd_SetBW(u32BW);
}

MS_BOOL MDrv_Demod_GetBW(MS_U32 *pu32BW)
{
    return MDrv_Dmd_GetBW(pu32BW);
}

MS_BOOL MDrv_Demod_GetLock(MS_BOOL *pbLock)
{
    return MDrv_Dmd_GetLock(pbLock);
}

MS_BOOL MDrv_Demod_GetSNR(MS_U32 *pu32SNR)
{
    return MDrv_Dmd_GetSNR(pu32SNR);
}

MS_BOOL MDrv_Demod_GetBER(float *pfBER)
{
    return MDrv_Dmd_GetBER(pfBER);
}

MS_BOOL MDrv_Demod_GetPWR(MS_S32 *ps32Signal)
{
    return MDrv_Dmd_GetPWR(ps32Signal);
}

MS_BOOL MDrv_Demod_Config(MS_U8 *pRegParam)
{
    return MDrv_Dmd_Config(pRegParam);
}

MS_BOOL MDrv_Demod_GetParam(DEMOD_MS_FE_CARRIER_PARAM* pParam)
{
    DMD_Param dmdParam;

    if (FALSE == MDrv_Dmd_GetParam(&dmdParam))
    {
        return FALSE;
    }

    switch (dmdParam.eQamType)
    {
    case E_DMD_QAM16:
        pParam->CabParam.eConstellation = DEMOD_CAB_QAM16;
        break;
    case E_DMD_QAM32:
        pParam->CabParam.eConstellation = DEMOD_CAB_QAM32;
        break;
    case E_DMD_QAM64:
        pParam->CabParam.eConstellation = DEMOD_CAB_QAM64;
        break;
    case E_DMD_QAM128:
        pParam->CabParam.eConstellation = DEMOD_CAB_QAM128;
        break;
    case E_DMD_QAM256:
        pParam->CabParam.eConstellation = DEMOD_CAB_QAM256;
        break;
    default:
        break;
    }
    switch (dmdParam.eIQSwap)
    {
    case E_DMD_IQ_NORMAL:
        pParam->CabParam.eIQMode = DEMOD_CAB_IQ_NORMAL;
        break;
    case E_DMD_IQ_INVERT:
        pParam->CabParam.eIQMode = DEMOD_CAB_IQ_INVERT;
        break;
    default:
        break;
    }

    pParam->CabParam.u16SymbolRate = (MS_U16)dmdParam.u32SymRate;
    pParam->CabParam.u8TapAssign = (MS_U8)dmdParam.eTapAssign;
    pParam->CabParam.u32FreqOffset = dmdParam.u32FreqOffset;
    pParam->CabParam.u8TuneFreqOffset = dmdParam.u8TuneFreqOffset;
    pParam->u32Frequency = dmdParam.u32TunerFreq;
    return TRUE;
}

MS_BOOL MDrv_Demod_Restart(DEMOD_MS_FE_CARRIER_PARAM* pParam)
{
    DMD_Param dmdParam;

    switch (pParam->CabParam.eConstellation)//  = DEMOD_CAB_QAM16;
    {
    case DEMOD_CAB_QAM16:
        dmdParam.eQamType = E_DMD_QAM16;
        break;
    case DEMOD_CAB_QAM32:
        dmdParam.eQamType = E_DMD_QAM32;
        break;
    case DEMOD_CAB_QAM64:
        dmdParam.eQamType = E_DMD_QAM64;
        break;
    case DEMOD_CAB_QAM128:
        dmdParam.eQamType = E_DMD_QAM128;
        break;
    case DEMOD_CAB_QAM256:
        dmdParam.eQamType = E_DMD_QAM256;
        break;
    default:
        break;
    }

    switch (pParam->CabParam.eIQMode)
    {
    case DEMOD_CAB_IQ_NORMAL:
        dmdParam.eIQSwap = E_DMD_IQ_NORMAL;
        break;
    case DEMOD_CAB_IQ_INVERT:
        dmdParam.eIQSwap = E_DMD_IQ_INVERT;
        break;
    default:
        break;
    }

    dmdParam.u32SymRate = pParam->CabParam.u16SymbolRate;
    dmdParam.eTapAssign = (DMD_TapAssign)pParam->CabParam.u8TapAssign;
    dmdParam.u32FreqOffset = pParam->CabParam.u32FreqOffset;
    dmdParam.u8TuneFreqOffset = pParam->CabParam.u8TuneFreqOffset;
    dmdParam.u32TunerFreq = pParam->u32Frequency;
    return MDrv_Dmd_Restart(&dmdParam);
}

MS_BOOL MDrv_Demod_SetMode(Demod_Mode* pMode)
{
    DMD_Mode dmdMod;
    dmdMod.bX4CFE_en            = pMode->bX4CFE_en;
    dmdMod.bPPD_en              = pMode->bPPD_en;
    dmdMod.bIQAutoSwap_en       = pMode->bIQAutoSwap_en;
    dmdMod.bQAMScan_en          = pMode->bQAMScan_en;
    dmdMod.bFHO_en              = pMode->bFHO_en;
    dmdMod.fptTunerSet          = pMode->fptTunerSet;
    return MDrv_Dmd_SetMode((DMD_Mode*)pMode);
}

MS_BOOL MDrv_Demod_SetOutoutPath(DEMOD_INTERFACE_MODE path)
{
    //Dummy function
    return TRUE;
}

DEMOD_INTERFACE_MODE MDrv_Demod_GetOutoutPath(void)
{
    //Dummy function
    return DEMOD_INTERFACE_PARALLEL;
}

#endif // (FRONTEND_DEMOD_TYPE == DEMOD_MSDVBC)
