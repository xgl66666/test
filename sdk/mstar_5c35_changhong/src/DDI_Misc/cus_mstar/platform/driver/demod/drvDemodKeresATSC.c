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

#if defined(CHIP_KERES)
#include "MsCommon.h"
#include "HbCommon.h"
#include "drvDMD_common.h"
#include "drvDMD_ATSC.h"
#include "drvDemod.h"
#include "drvDemodNull.h"
#include "drvDMD_VD_MBX.h"
#include "drvSAR.h"
#include<string.h>


#define COFDMDMD_MUTEX_TIMEOUT       (2000)
#define MAX_TUNER_NUM                2


#undef DUAL_DMD
#define DUAL_DMD 0

DRV_DEMOD_TABLE_TYPE GET_DEMOD_ENTRY_NODE(DEMOD_MSKERES_ATSC) DDI_DRV_TABLE_ENTRY(demodtab);

//static MS_BOOL bInited = FALSE;
static MS_BOOL bInited[MAX_TUNER_NUM] = {FALSE,FALSE};
static MS_S32 _s32MutexId = -1;
static MS_U32 _u32IFrequency[MAX_TUNER_NUM] = {36130, 36130};

MS_BOOL MDrv_Demod_MSATSC_51_Init(MS_U8 u8DemodIndex,DEMOD_MS_INIT_PARAM* pParam)
{
    DMD_ATSC_InitData sDMD_ATSC_InitData;
    MS_BOOL ret = FALSE;

    if(NULL == pParam)
        return FALSE;
    
    if (_s32MutexId < 0)
    {
        _s32MutexId = MsOS_CreateMutex(E_MSOS_FIFO, "OfDmd_Mutex", MSOS_PROCESS_SHARED);

        if (_s32MutexId < 0)
        {
            GEN_EXCEP;
            return FALSE;
        }

    }

    MDrv_DMD_PreInit(); //register DEMOD base address
    if(FALSE == pParam->pstTunertab->GetTunerIF(u8DemodIndex, &_u32IFrequency[u8DemodIndex]))
    {
        DMD_DBG(("Get IF from Tuner FAIL !! Use Default\n"));
         _u32IFrequency[u8DemodIndex] = 5000;
    }

    MDrv_SYS_DMD_VD_MBX_Init();
    MDrv_SYS_DMD_VD_MBX_SetType(E_DMD_VD_MBX_TYPE_ATSC);
    
    MDrv_SAR_Kpd_Init();

    // register init
    memset(&sDMD_ATSC_InitData, 0, sizeof(DMD_ATSC_InitData));
    sDMD_ATSC_InitData.u8DMD_ATSC_DSPRegInitExt=NULL; // TODO use system variable type
    sDMD_ATSC_InitData.u8DMD_ATSC_DSPRegInitSize=0;
    sDMD_ATSC_InitData.u8DMD_ATSC_InitExt=NULL; // TODO use system variable type
    
    sDMD_ATSC_InitData.u16IF_KHZ = _u32IFrequency[u8DemodIndex];
    sDMD_ATSC_InitData.bIQSwap = FALSE;
    sDMD_ATSC_InitData.u8IS_DUAL = FALSE;  
    sDMD_ATSC_InitData.bIsExtDemod = FALSE;

    MDrv_DMD_ATSC_Initial_Hal_Interface();
    
    ret = MDrv_DMD_ATSC_Init(&sDMD_ATSC_InitData, sizeof(sDMD_ATSC_InitData)); // _UTOPIA
  
    ret &= MDrv_DMD_ATSC_SetConfig(DMD_ATSC_DEMOD_ATSC_256QAM, TRUE); 
    
    if(ret == TRUE)
    {
        bInited[u8DemodIndex] = TRUE;
    }

    return ret;
}


MS_BOOL MDrv_Demod_MSATSC_51_Open(MS_U8 u8DemodIndex)
{
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    MDrv_DMD_ATSC_SEL_DMD((eDMD_SEL)u8DemodIndex);
    HB_ReleaseMutex(_s32MutexId);

    return TRUE;
}

MS_BOOL MDrv_Demod_MSATSC_51_Close(MS_U8 u8DemodIndex)
{
    MS_BOOL ret;

    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    MDrv_DMD_ATSC_SEL_DMD((eDMD_SEL)u8DemodIndex);
    ret = MDrv_DMD_ATSC_Exit();
    if(ret == TRUE)
    {
        bInited[u8DemodIndex] = FALSE;
    }
    HB_ReleaseMutex(_s32MutexId);
    return ret;

}

MS_BOOL MDrv_Demod_MSATSC_51_SetSerialControl(MS_U8 u8DemodIndex, MS_BOOL bEnable)
{
    MS_BOOL ret;

    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    if(bInited[u8DemodIndex] == FALSE)
    {
        HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }
    MDrv_DMD_ATSC_SEL_DMD((eDMD_SEL)u8DemodIndex);
    ret = MDrv_DMD_ATSC_SetSerialControl(bEnable);
    HB_ReleaseMutex(_s32MutexId);
    return ret;
}

MS_BOOL MDrv_Demod_MSATSC_51_PowerOnOff(MS_U8 u8DemodIndex, MS_BOOL bPowerOn)
{
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    MDrv_DMD_ATSC_SEL_DMD((eDMD_SEL)u8DemodIndex);
    HB_ReleaseMutex(_s32MutexId);
    return TRUE;
}

MS_BOOL MDrv_Demod_MSATSC_51_GetLock(MS_U8 u8DemodIndex, EN_LOCK_STATUS *peLockStatus)
{
    DMD_ATSC_LOCK_STATUS LockStatus;

    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    DMD_DBG(("[%s][%d] \n",__FUNCTION__,__LINE__));
    if(bInited[u8DemodIndex] == FALSE)
    {
        HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }
    MDrv_DMD_ATSC_SEL_DMD((eDMD_SEL)u8DemodIndex);
  
    LockStatus = MDrv_DMD_ATSC_GetLock(DMD_ATSC_GETLOCK);
    DMD_DBG(("[%s][%d] LockStatus %d \n",__FUNCTION__,__LINE__,LockStatus));
    switch (LockStatus)
    {
        case DMD_ATSC_LOCK:
            *peLockStatus = E_DEMOD_LOCK;
            break;
        case DMD_ATSC_CHECKEND:
            *peLockStatus = E_DEMOD_CHECKEND;
            break;
        case DMD_ATSC_UNLOCK:       
            *peLockStatus = E_DEMOD_UNLOCK;
            break;
        case DMD_ATSC_CHECKING:
        default:
            *peLockStatus = E_DEMOD_CHECKING;
            break;
    }

    HB_ReleaseMutex(_s32MutexId);
    return TRUE;
}

MS_BOOL MDrv_Demod_MSATSC_51_GetSNR(MS_U8 u8DemodIndex, float *pfSNR)
{

    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    if(bInited[u8DemodIndex] == FALSE)
    {
        *pfSNR = 0;
        HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }

    MDrv_DMD_ATSC_SEL_DMD((eDMD_SEL)u8DemodIndex);
    *pfSNR = (float)MDrv_DMD_ATSC_GetSNRPercentage()*0.4;
    HB_ReleaseMutex(_s32MutexId);

    return TRUE;
    
}

MS_BOOL MDrv_Demod_MSATSC_51_GetBER(MS_U8 u8DemodIndex, float *pfBER)
{
    MS_BOOL ret = TRUE;

    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    if(bInited[u8DemodIndex] == FALSE)
    {
        *pfBER = 0;
        HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }
    MDrv_DMD_ATSC_SEL_DMD((eDMD_SEL)u8DemodIndex);
    ret = MDrv_DMD_ATSC_GetPostViterbiBer(pfBER);

    HB_ReleaseMutex(_s32MutexId);
    return ret;

}

MS_BOOL MDrv_Demod_MSATSC_51_GetPWR(MS_U8 u8DemodIndex, MS_S32 *ps32Signal)
{
    MS_BOOL ret;
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    if(bInited[u8DemodIndex] == FALSE)
    {
        *ps32Signal = 0;
        HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }
    MDrv_DMD_ATSC_SEL_DMD((eDMD_SEL)u8DemodIndex);
    ret = MDrv_DMD_ATSC_GetSignalStrength((MS_U16*)ps32Signal);
    
    HB_ReleaseMutex(_s32MutexId);
    return ret;
}

MS_BOOL MDrv_Demod_MSATSC_51_GetSignalQuality(MS_U8 u8DemodIndex, MS_U16 *pu16quality)
{
    MS_BOOL ret = TRUE;
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    if(bInited[u8DemodIndex] == FALSE)
    {
        *pu16quality = 0;
        HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }

    MDrv_DMD_ATSC_SEL_DMD((eDMD_SEL)u8DemodIndex);
    *pu16quality = (MS_U16)MDrv_DMD_ATSC_GetSNRPercentage(); 
    HB_ReleaseMutex(_s32MutexId);
    return ret;
}


MS_BOOL MDrv_Demod_MSATSC_51_Config(MS_U8 u8DemodIndex, MS_U8 *pRegParam)
{
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    MDrv_DMD_ATSC_SEL_DMD((eDMD_SEL)u8DemodIndex);
    HB_ReleaseMutex(_s32MutexId);
    return TRUE;
}


MS_BOOL MDrv_Demod_MSATSC_51_GetParam(MS_U8 u8DemodIndex, DEMOD_MS_FE_CARRIER_PARAM* pParam)
{
    return TRUE;
}

MS_BOOL MDrv_Demod_MSATSC_51_Restart(MS_U8 u8DemodIndex, DEMOD_MS_FE_CARRIER_PARAM* pParam,MS_U32 u32BroadCastType)
{
    MDrv_DMD_ATSC_SEL_DMD((eDMD_SEL)u8DemodIndex);
    MDrv_DMD_ATSC_SetConfig(DMD_ATSC_DEMOD_ATSC_256QAM, TRUE);
    return TRUE;

}


MS_BOOL MDrv_Demod_MSATSC_51_SetMode(MS_U8 u8DemodIndex, Demod_Mode* pMode)
{
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    MDrv_DMD_ATSC_SEL_DMD((eDMD_SEL)u8DemodIndex);
    HB_ReleaseMutex(_s32MutexId);
    return TRUE;
}

MS_BOOL MDrv_Demod_MSATSC_51_SetOutoutPath(MS_U8 u8DemodIndex, DEMOD_INTERFACE_MODE path)
{
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    MDrv_DMD_ATSC_SEL_DMD((eDMD_SEL)u8DemodIndex);
    HB_ReleaseMutex(_s32MutexId);
    return TRUE;
}

DEMOD_INTERFACE_MODE MDrv_Demod_MSATSC_51_GetOutoutPath(MS_U8 u8DemodIndex)
{
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    MDrv_DMD_ATSC_SEL_DMD((eDMD_SEL)u8DemodIndex);
    HB_ReleaseMutex(_s32MutexId);
    return DEMOD_INTERFACE_PARALLEL;
}

MS_BOOL DEMOD_MSKELTIC_ATSC_Extension_Function(MS_U8 u8DemodIndex, DEMOD_EXT_FUNCTION_TYPE fuction_type, void *data)
{
    switch(fuction_type)
    { 
        default:
            DMD_DBG(("Request extension function (%x) does not exist\n",fuction_type));
            return TRUE;
    } 
}

MS_BOOL MDrv_Demod_MSATSC_51_I2C_ByPass(MS_U8 u8DemodIndex,MS_BOOL bOn)
{
  if(GET_DEMOD_ENTRY_NODE(DEMOD_MSKERES_ATSC).I2CByPassPreSetting != NULL)
  {
      return GET_DEMOD_ENTRY_NODE(DEMOD_MSKERES_ATSC).I2CByPassPreSetting(u8DemodIndex,bOn);
  }
  else
      return MDrv_Demod_null_I2C_ByPass(u8DemodIndex,bOn);
}


DRV_DEMOD_TABLE_TYPE GET_DEMOD_ENTRY_NODE(DEMOD_MSKERES_ATSC) DDI_DRV_TABLE_ENTRY(demodtab) = 
{  
     .name                         = "DEMOD_MSKERES_ATSC",
     .data                         = DEMOD_MSKERES_ATSC,        
     .init                         = MDrv_Demod_MSATSC_51_Init,
     .GetLock                      = MDrv_Demod_MSATSC_51_GetLock,
     .GetSNR                       = MDrv_Demod_MSATSC_51_GetSNR,
     .GetBER                       = MDrv_Demod_MSATSC_51_GetBER,
     .GetPWR                       = MDrv_Demod_MSATSC_51_GetPWR,
     .GetQuality                   = MDrv_Demod_MSATSC_51_GetSignalQuality,
     .GetParam                     = MDrv_Demod_MSATSC_51_GetParam,
     .Restart                      = MDrv_Demod_MSATSC_51_Restart,
     .I2CByPass                    = MDrv_Demod_MSATSC_51_I2C_ByPass,
     .I2CByPassPreSetting          = NULL,
     .Extension_Function           = DEMOD_MSKELTIC_ATSC_Extension_Function,
     .Extension_FunctionPreSetting = NULL,
#if MS_DVBT2_INUSE
     .SetCurrentDemodType          = MDrv_Demod_null_SetCurrentDemodType,
     .GetCurrentDemodType          = MDrv_Demod_null_GetCurrentDemodType,
     .GetPlpBitMap                 = MDrv_Demod_null_GetPlpBitMap,
     .GetPlpGroupID                = MDrv_Demod_null_GetPlpGroupID,
     .SetPlpGroupID                = MDrv_Demod_null_SetPlpGroupID,
#endif
#if MS_DVBS_INUSE
     .BlindScanStart               = MDrv_Demod_null_BlindScan_Start,
     .BlindScanNextFreq            = MDrv_Demod_null_BlindScan_NextFreq,
     .BlindScanWaitCurFreqFinished = MDrv_Demod_null_BlindScan_WaitCurFreqFinished,
     .BlindScanCancel              = MDrv_Demod_null_BlindScan_Cancel,
     .BlindScanEnd                 = MDrv_Demod_null_BlindScan_End,
     .BlindScanGetChannel          = MDrv_Demod_null_BlindScan_GetChannel,
     .BlindScanGetCurrentFreq      = MDrv_Demod_null_BlindScan_GetCurrentFreq,
     .DiSEqCSetTone                = MDrv_Demod_null_DiSEqC_SetTone,
     .DiSEqCSetLNBOut              = MDrv_Demod_null_DiSEqC_SetLNBOut,
     .DiSEqCGetLNBOut              = MDrv_Demod_null_DiSEqC_GetLNBOut,
     .DiSEqCSet22kOnOff            = MDrv_Demod_null_DiSEqC_Set22kOnOff,
     .DiSEqCGet22kOnOff            = MDrv_Demod_null_DiSEqC_Get22kOnOff,
     .DiSEqC_SendCmd               = MDrv_Demod_null_DiSEqC_SendCmd
#endif
};

#endif // (FRONTEND_DEMOD_TYPE == DEMOD_MSATSC_C)

