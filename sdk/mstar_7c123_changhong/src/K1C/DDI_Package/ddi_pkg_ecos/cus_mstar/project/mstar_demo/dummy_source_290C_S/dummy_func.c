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
// Copyright (c) 2006-2007 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// ("MStar Confidential Information",__FUNCTION__) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    dummy_func.c
/// @brief  dummy functions to prevnet build error under CAM solution
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include "MsTypes.h"
#include "stdio.h"
#include "apiDigiTuner.h"
#include "drvHWI2C.h"

void FLASHRegisterToUtopia(FUtopiaOpen ModuleType)
{
    printf("Nonsupport module under this board: %s! \n",__FUNCTION__);
}

void IRRegisterToUtopia(FUtopiaOpen ModuleType)
{
    printf("Nonsupport module under this board! %s \n",__FUNCTION__);
}

void MVOPRegisterToUtopia(FUtopiaOpen ModuleType)
{
    printf("Nonsupport module under this board! %s \n",__FUNCTION__);
}

void MSPIRegisterToUtopia(FUtopiaOpen ModuleType)
{
    printf("Nonsupport module under this board! %s \n",__FUNCTION__);
}

void PWMRegisterToUtopia(FUtopiaOpen ModuleType)
{
    printf("Nonsupport module under this board! %s \n",__FUNCTION__);
}

void PWSRegisterToUtopia(FUtopiaOpen ModuleType)
{
    printf("Nonsupport module under this board! %s \n",__FUNCTION__);
}

void RTCRegisterToUtopia(FUtopiaOpen ModuleType)
{
    printf("Nonsupport module under this board! %s \n",__FUNCTION__);
}

void VBIRegisterToUtopia(FUtopiaOpen ModuleType)
{
    printf("Nonsupport module under this board! %s \n",__FUNCTION__);
}

void ACERegisterToUtopia(FUtopiaOpen ModuleType)
{
    printf("Nonsupport module under this board! %s \n",__FUNCTION__);
}

void DLCRegisterToUtopia(FUtopiaOpen ModuleType)
{
    printf("Nonsupport module under this board! %s \n",__FUNCTION__);
}

void GFXRegisterToUtopia(FUtopiaOpen ModuleType)
{
    printf("Nonsupport module under this board! %s \n",__FUNCTION__);
}

void GOPRegisterToUtopia(FUtopiaOpen ModuleType)
{
    printf("Nonsupport module under this board! %s \n",__FUNCTION__);
}

void PNLRegisterToUtopia(FUtopiaOpen ModuleType)
{
    printf("Nonsupport module under this board! %s \n",__FUNCTION__);
}

void XCRegisterToUtopia(FUtopiaOpen ModuleType)
{
    printf("Nonsupport module under this board! %s \n",__FUNCTION__);
}

void VDEC_EXRegisterToUtopia(FUtopiaOpen ModuleType)
{
    printf("Nonsupport module under this board! %s \n",__FUNCTION__);
}

void TVENCODERRegisterToUtopia(FUtopiaOpen ModuleType)
{
    printf("Nonsupport module under this board! %s \n",__FUNCTION__);
}

void JPEG_EXRegisterToUtopia(FUtopiaOpen ModuleType)
{
    printf("Nonsupport module under this board! %s \n",__FUNCTION__);
}

void GPDRegisterToUtopia(FUtopiaOpen ModuleType)
{
    printf("Nonsupport module under this board! %s \n",__FUNCTION__);
}

void ACPRegisterToUtopia(FUtopiaOpen ModuleType)
{
    printf("Nonsupport module under this board! %s \n",__FUNCTION__);
}

void DACRegisterToUtopia(FUtopiaOpen ModuleType)
{
    printf("Nonsupport module under this board! %s \n",__FUNCTION__);
}

void AESDMARegisterToUtopia(FUtopiaOpen ModuleType)
{
    printf("Nonsupport module under this board! %s \n",__FUNCTION__);
}

void AUDIORegisterToUtopia(FUtopiaOpen ModuleType)
{
    printf("Nonsupport module under this board! %s \n",__FUNCTION__);
}

void HDMITXRegisterToUtopia(FUtopiaOpen ModuleType)
{
    printf("Nonsupport module under this board! %s \n",__FUNCTION__);
}

void CECRegisterToUtopia(FUtopiaOpen ModuleType)
{
    printf("Nonsupport module under this board! %s \n",__FUNCTION__);
}

MS_BOOL appDemo_DigiTuner_SetIndex(MS_U32 *TunerIndex)
{
    printf("Dummy function to prevent compile error for CAM solution: %s\n",__FUNCTION__);
    return FALSE;
}


MS_BOOL appDemo_DigiTuner_SetFreq(const MS_U32 *Freq, const MS_U32 *u32Arg1, const MS_U32 *u32Arg2)
{
    printf("Dummy function to prevent compile error for CAM solution: %s\n",__FUNCTION__);
    return FALSE;
}

MS_BOOL appDemo_Frontend_SetDetectMode(const MS_U32 *u32DetectMode)
{
    printf("Dummy function to prevent compile error for CAM solution: %s\n",__FUNCTION__);
    return FALSE;
}

MS_BOOL appDemo_Frontend_SetBroadcastType(const MS_U32 *u32BroadcastType)
{
    printf("Dummy function to prevent compile error for CAM solution: %s\n",__FUNCTION__);
    return FALSE;
}

MS_BOOL appDemo_Frontend_SetInExteralPath(const MS_U32 *u32InExTernalPath)
{
    printf("Dummy function to prevent compile error for CAM solution: %s\n",__FUNCTION__);
    return FALSE;
}

MS_BOOL appDemo_Frontend_SetTunerDemod(const MS_U8 *u8IICPortIdx,const MS_U32 *u32TunerIndex, const MS_U32 *u32DemodIndex)
{
    printf("Dummy function to prevent compile error for CAM solution: %s\n",__FUNCTION__);
    return FALSE;
}

MS_BOOL appDemo_Frontend_HWReset(const MS_U32 *u32TunerIndex)
{
    printf("Dummy function to prevent compile error for CAM solution: %s\n",__FUNCTION__);
    return FALSE;
}

MS_BOOL appDemo_Frontend_FEListShow(void)
{
    printf("Dummy function to prevent compile error for CAM solution: %s\n",__FUNCTION__);
    return FALSE;
}

MS_BOOL appDemo_Frontend_Init(const MS_U32 *pu32TunerIndex)
{
    printf("Dummy function to prevent compile error for CAM solution: %s\n",__FUNCTION__);
    return FALSE;
}

MS_BOOL appDemo_Frontend_SetDMD_PIDFlt(const MS_U32 *pu32PID, const MS_U32 *pu32Drop, const MS_U32 *pu32PIDRemap)
{
    printf("Dummy function to prevent compile error for CAM solution: %s\n",__FUNCTION__);
    return FALSE;
}

MS_BOOL appDemo_DigiTuner_GetParam(const MS_U32 *u32Param)
{
    printf("Dummy function to prevent compile error for CAM solution: %s\n",__FUNCTION__);
    return FALSE;
}

MS_BOOL appDemo_DigiTuner_GetSignalStatus(const MS_U32 *pu32PollingCnt)
{
    printf("Dummy function to prevent compile error for CAM solution: %s\n",__FUNCTION__);
    return FALSE;
}

MS_BOOL appDemo_DigiTuner_SetLNBPower(const MS_U32 *LNBPower, const MS_U32 *CableID)
{
    printf("Dummy function to prevent compile error for CAM solution: %s\n",__FUNCTION__);
    return FALSE;
}

MS_BOOL appDemo_DigiTuner_Satellite_Set22K(const MS_U32 * b22KOn, const MS_U32 *CableID)
{
    printf("Dummy function to prevent compile error for CAM solution: %s\n",__FUNCTION__);
    return FALSE;
}

MS_BOOL appDemo_DigiTuner_Satellite_Set22KTone(const MS_U32* ToneType, const MS_U32 *CableID)
{
    printf("Dummy function to prevent compile error for CAM solution: %s\n",__FUNCTION__);
    return FALSE;
}

MS_BOOL appDemo_FE_DiseqcCmd(const MS_U32 *u32CmdType,const MS_U32 *u32Arg0,const MS_U32 *u32Arg1,const MS_U32 *u32Arg2, const MS_U32 *CableID)
{
    printf("Dummy function to prevent compile error for CAM solution: %s\n",__FUNCTION__);
    return FALSE;
}

MS_BOOL appDemo_DigiTuner_GetVCM_ISID_Info(void)
{
    printf("Dummy function to prevent compile error for CAM solution: %s\n",__FUNCTION__);
    return FALSE;
}

MS_BOOL appDemo_DigiTuner_SetVCM_ISID(const MS_U32 *u32ISID, const MS_U32 *IsNextCH)
{
    printf("Dummy function to prevent compile error for CAM solution: %s\n",__FUNCTION__);
    return FALSE;
}

MS_BOOL appDemo_DigiTuner_SetLNBLOF(const MS_U32 *HiLOF, const MS_U32 *LoLOF, const MS_U32 *CableID)
{
    printf("Dummy function to prevent compile error for CAM solution: %s\n",__FUNCTION__);
    return FALSE;
}

MS_BOOL appDemo_DigiTuner_CheckLock(void)
{
    printf("Dummy function to prevent compile error for CAM solution: %s\n",__FUNCTION__);
    return FALSE;
}

void MDrv_IIC_Init(MS_IIC_PORT ePort)
{
    printf("Dummy function to prevent compile error for CAM solution: %s\n",__FUNCTION__);
}

void MDrv_MVOP_Init(void)
{
    printf("Dummy function to prevent compile error for CAM solution: %s\n",__FUNCTION__);
}

void MDrv_SAR_Kpd_Init(void)
{
    printf("Dummy function to prevent compile error for CAM solution: %s\n",__FUNCTION__);
}

MS_BOOL MApi_DigiTuner_Set_MemPool(E_DDI_FE_SYSPOOLID eSYSPOOL,MS_S32 s32PoolID)
{
    printf("Dummy function to prevent compile error for CAM solution: %s\n",__FUNCTION__);
    return FALSE;
}

void EMAC_PowerOff(void)
{
    printf("Dummy function to prevent compile error for CAM solution: %s\n",__FUNCTION__);
}

