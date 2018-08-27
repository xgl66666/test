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

#if IS_THIS_DEMOD_PICKED(DEMOD_MSATSC_C)
#if defined(CHIP_KAISER)
#include "MsCommon.h"
#include "HbCommon.h"
#include "drvDMD_ATSC.h"
#include "drvDMD_common.h"
#include "drvDemod.h"
#include "drvDemodNull.h"
#include "drvDMD_VD_MBX.h"
#include "drvSAR.h"

#define COFDMDMD_MUTEX_TIMEOUT       (2000)
#define MAX_TUNER_NUM                2

#ifdef MS_DEBUG
#define DBG_MSB(x)  x
#else
#define DBG_MSB(x)
#endif

//static MS_BOOL bInited = FALSE;
static MS_BOOL bInited[MAX_TUNER_NUM] = {FALSE,FALSE};
static MS_S32 _s32MutexId = -1;

#if IF_THIS_TUNER_INUSE(TUNER_PHILIPS_TDA18250HN)||IF_THIS_TUNER_INUSE(TUNER_MXL603)||IF_THIS_TUNER_INUSE(TUNER_TDA18250A)||IF_THIS_TUNER_INUSE(TUNER_TDA18250B)
const static MS_U32 _u32IFrequency = 5000;//KHz for demod restart&init
//const static MS_U32 _u32AGCReference = 0;
#else
const static MS_U32 _u32IFrequency = 36130;//KHz for demod restart&init
#endif

DRV_DEMOD_TABLE_TYPE GET_DEMOD_ENTRY_NODE(DEMOD_MSATSC_C) DDI_DRV_TABLE_ENTRY(demodtab);

static MS_BOOL MDrv_Demod_MSATSC_Public_Init(void )
{
    DMD_ATSC_InitData sDMD_ATSC_InitData;
    MS_BOOL ret;

    if (_s32MutexId < 0)
    {
        _s32MutexId = MsOS_CreateMutex(E_MSOS_FIFO, "OfDmd_Mutex", MSOS_PROCESS_SHARED);

        if (_s32MutexId < 0)
        {
            DMD_ERR(("%s: _s32MutexId < 0!!!\n", __FUNCTION__));
            return FALSE;
        }

    }

    MDrv_DMD_PreInit(); //register DEMOD base address

    MDrv_SYS_DMD_VD_MBX_Init();
    MDrv_SYS_DMD_VD_MBX_SetType(E_DMD_VD_MBX_TYPE_ATSC);


    sDMD_ATSC_InitData.u8DMD_ATSC_DSPRegInitExt=NULL; // TODO use system variable type
    sDMD_ATSC_InitData.u8DMD_ATSC_DSPRegInitSize=0;
    sDMD_ATSC_InitData.u8DMD_ATSC_InitExt=NULL; // TODO use system variable type

    sDMD_ATSC_InitData.u16IF_KHZ = _u32IFrequency;
    sDMD_ATSC_InitData.bIQSwap = NULL;
    sDMD_ATSC_InitData.bIsExtDemod = NULL;
    //sDMD_ATSC_InitData.u16AGC_REFERENCE = _u32AGCReference;  //mark provisionally

    sDMD_ATSC_InitData.u8IS_DUAL = TRUE;  //default use DUAL config always

    MDrv_DMD_ATSC_Initial_Hal_Interface();

    // init DEMOD0
    ret = MDrv_DMD_ATSC_MD_Init(0, &sDMD_ATSC_InitData, sizeof(sDMD_ATSC_InitData));
    // init DEMOD1
    ret &= MDrv_DMD_ATSC_MD_Init(1, &sDMD_ATSC_InitData, sizeof(sDMD_ATSC_InitData));

    return ret ;
}

static MS_BOOL MDrv_Demod_MSATSC_Individual_Init(MS_U8 u8DemodIndex)
{
    MS_BOOL ret = FALSE;

    ret = MDrv_DMD_ATSC_MD_SetConfig(u8DemodIndex, DMD_ATSC_DEMOD_ATSC_256QAM, TRUE);

    if (ret)
        bInited[u8DemodIndex] = TRUE;

    return ret;

}

MS_BOOL MDrv_Demod_MSATSC_51_Init(MS_U8 u8DemodIndex,DEMOD_MS_INIT_PARAM* pParam)
{
    MS_BOOL ret = FALSE;

    if (bInited[0] || bInited[1])
    {
        ret = TRUE;
    }
    else
    {
        ret = MDrv_Demod_MSATSC_Public_Init();
    }

    ret &= MDrv_Demod_MSATSC_Individual_Init(u8DemodIndex);

    return ret;
}

/*
static MS_BOOL MDrv_Demod_MSATSC_Restart(void)
{
    return FALSE;
}
*/

MS_BOOL MDrv_Demod_MSATSC_51_Open(MS_U8 u8DemodIndex)
{
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    HB_ReleaseMutex(_s32MutexId);

    return TRUE;
}

MS_BOOL MDrv_Demod_MSATSC_51_Close(MS_U8 u8DemodIndex)
{
    MS_BOOL ret;

    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    ret = MDrv_DMD_ATSC_MD_Exit(u8DemodIndex);
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
        DBG_MSB(HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    if(bInited[u8DemodIndex] == FALSE)
    {
        HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }

    ret = MDrv_DMD_ATSC_MD_SetSerialControl(u8DemodIndex, bEnable);
    HB_ReleaseMutex(_s32MutexId);
    return ret;
}

MS_BOOL MDrv_Demod_MSATSC_51_PowerOnOff(MS_U8 u8DemodIndex, MS_BOOL bPowerOn)
{
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    HB_ReleaseMutex(_s32MutexId);
    return TRUE;
}

MS_BOOL MDrv_Demod_MSATSC_51_GetLock(MS_U8 u8DemodIndex, EN_LOCK_STATUS *peLockStatus)
{
    DMD_ATSC_LOCK_STATUS LockStatus;

    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    printf("[%s][%d] \n",__FUNCTION__,__LINE__);
    if(bInited[u8DemodIndex] == FALSE)
    {
        *peLockStatus = E_DEMOD_NULL;
        HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }

    LockStatus = MDrv_DMD_ATSC_MD_GetLock(u8DemodIndex, DMD_ATSC_GETLOCK);
    printf("[%s][%d] LockStatus %d \n",__FUNCTION__,__LINE__,LockStatus);
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
    //MS_BOOL ret;

    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    if(bInited[u8DemodIndex] == FALSE)
    {
        *pfSNR = 0;
        HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }

    MDrv_DMD_ATSC_MD_GET_QAM_SNR(u8DemodIndex, pfSNR);
    HB_ReleaseMutex(_s32MutexId);

    //return ret;
    return TRUE;

}

MS_BOOL MDrv_Demod_MSATSC_51_GetBER(MS_U8 u8DemodIndex, float *pfBER)
{
    MS_BOOL ret;

    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    if(bInited[u8DemodIndex] == FALSE)
    {
        *pfBER = 0;
        HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }

    ret = MDrv_DMD_ATSC_MD_GetPostViterbiBer(u8DemodIndex, pfBER);

    HB_ReleaseMutex(_s32MutexId);
    return ret;

}

MS_BOOL MDrv_Demod_MSATSC_51_GetPWR(MS_U8 u8DemodIndex, MS_S32 *ps32Signal)
{
    MS_BOOL ret;
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    if(bInited[u8DemodIndex] == FALSE)
    {
        *ps32Signal = 0;
        HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }

    ret = MDrv_DMD_ATSC_MD_GetSignalStrength(u8DemodIndex, (MS_U16*)ps32Signal);

    HB_ReleaseMutex(_s32MutexId);
    return ret;
}

MS_BOOL MDrv_Demod_MSATSC_51_GetSSI(MS_U8 u8DemodIndex, MS_U16 *pu16SSI)
{
    MS_BOOL ret;
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    if(bInited[u8DemodIndex] == FALSE)
    {
        *pu16SSI = 0;
        HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }

    ret = MDrv_DMD_ATSC_MD_GetSignalStrength(u8DemodIndex, pu16SSI);

    HB_ReleaseMutex(_s32MutexId);
    return ret;
}


MS_BOOL MDrv_Demod_MSATSC_51_GetSignalQuality(MS_U8 u8DemodIndex, MS_U16 *pu16quality)
{
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    if(bInited[u8DemodIndex] == FALSE)
    {
        *pu16quality = 0;
        HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }


    if( MDrv_DMD_ATSC_MD_GetLock(u8DemodIndex, DMD_ATSC_GETLOCK)== DMD_ATSC_LOCK)
    {
        *pu16quality = (MS_U16)MDrv_DMD_ATSC_MD_GetSNRPercentage(u8DemodIndex);
    }
    else
    {
        *pu16quality = 0;
    }

    HB_ReleaseMutex(_s32MutexId);
    return TRUE;

}

MS_BOOL MDrv_Demod_MSATSC_51_Config(MS_U8 u8DemodIndex, MS_U8 *pRegParam)
{
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    HB_ReleaseMutex(_s32MutexId);
    return TRUE;
}


MS_BOOL MDrv_Demod_MSATSC_51_GetParam(MS_U8 u8DemodIndex, DEMOD_MS_FE_CARRIER_PARAM* pParam)
{
    #if 0
    DMD_DVBC_MODULATION_TYPE QAMMode;
    MS_U16 u16SymbolRate;
    float FreqOff;

    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }


    if(bInited == FALSE)
    {
        HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }
    MDrv_DMD_DVBC_ActiveDmdSwitch(u8DemodIndex);
    if(FALSE == MDrv_DMD_DVBC_GetStatus(&QAMMode, &u16SymbolRate, &FreqOff))
    {
        HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }

    switch (QAMMode)
    {
        case DMD_DVBC_QAM16:
            pParam->CabParam.eConstellation = DEMOD_CAB_QAM16;
            break;
        case DMD_DVBC_QAM32:
            pParam->CabParam.eConstellation = DEMOD_CAB_QAM32;
            break;
        case DMD_DVBC_QAM64:
            pParam->CabParam.eConstellation = DEMOD_CAB_QAM64;
            break;
        case DMD_DVBC_QAM128:
            pParam->CabParam.eConstellation = DEMOD_CAB_QAM128;
            break;
        case DMD_DVBC_QAM256:
            pParam->CabParam.eConstellation = DEMOD_CAB_QAM256;
            break;
        case DMD_DVBC_QAMAUTO:
            pParam->CabParam.eConstellation = DEMOD_CAB_QAMAUTO;
            break;
        default:
            pParam->CabParam.eConstellation = DEMOD_CAB_QAM128;
            break;
    }

    pParam->CabParam.u16SymbolRate = u16SymbolRate;
    pParam->CabParam.u32FreqOffset = FreqOff;

    HB_ReleaseMutex(_s32MutexId);
    #endif

    return TRUE;

}

MS_BOOL MDrv_Demod_MSATSC_51_Restart(MS_U8 u8DemodIndex, DEMOD_MS_FE_CARRIER_PARAM* pParam,MS_U32 u32BroadCastType)
{
    MDrv_DMD_ATSC_MD_SetConfig(u8DemodIndex, DMD_ATSC_DEMOD_ATSC_256QAM, TRUE);
    return TRUE;
}


MS_BOOL MDrv_Demod_MSATSC_51_SetMode(MS_U8 u8DemodIndex, Demod_Mode* pMode)
{
#if 0
    DMD_Mode dmdMod;
    dmdMod.bX4CFE_en            = pMode->bX4CFE_en;
    dmdMod.bPPD_en              = pMode->bPPD_en;
    dmdMod.bIQAutoSwap_en       = pMode->bIQAutoSwap_en;
    dmdMod.bQAMScan_en          = pMode->bQAMScan_en;
    dmdMod.bFHO_en              = pMode->bFHO_en;
    dmdMod.fptTunerSet          = pMode->fptTunerSet;
    return MDrv_Dmd_SetMode((DMD_Mode*)pMode);
#else
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    HB_ReleaseMutex(_s32MutexId);
    return TRUE;
#endif
}

MS_BOOL MDrv_Demod_MSATSC_51_SetOutoutPath(MS_U8 u8DemodIndex, DEMOD_INTERFACE_MODE path)
{
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    HB_ReleaseMutex(_s32MutexId);
    return TRUE;
}

DEMOD_INTERFACE_MODE MDrv_Demod_MSATSC_51_GetOutoutPath(MS_U8 u8DemodIndex)
{
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    HB_ReleaseMutex(_s32MutexId);
    return DEMOD_INTERFACE_PARALLEL;
}


MS_BOOL DEMOD_MSATSC_C_Extension_Function(MS_U8 u8DemodIndex, DEMOD_EXT_FUNCTION_TYPE fuction_type, void *data)
{
    switch(fuction_type)
    {
        case DEMOD_EXT_FUNC_OPEN:
            return MDrv_Demod_MSATSC_51_Open(u8DemodIndex);
            break;
        default:
            printf("Request extension function (%x) does not exist\n",fuction_type);
            return TRUE;
    }
}

MS_BOOL MDrv_Demod_MSATSC_51_I2C_ByPass(MS_U8 u8DemodIndex,MS_BOOL bOn)
{
  if(GET_DEMOD_ENTRY_NODE(DEMOD_MSATSC_C).I2CByPassPreSetting != NULL)
  {
      return GET_DEMOD_ENTRY_NODE(DEMOD_MSATSC_C).I2CByPassPreSetting(u8DemodIndex,bOn);
  }
  else
      return MDrv_Demod_null_I2C_ByPass(u8DemodIndex,bOn);
}

DRV_DEMOD_TABLE_TYPE GET_DEMOD_ENTRY_NODE(DEMOD_MSATSC_C) DDI_DRV_TABLE_ENTRY(demodtab) =
{
     .name                         = "DEMOD_MSATSC_C",
     .data                         = DEMOD_MSATSC_C,
     .init                         = MDrv_Demod_MSATSC_51_Init,
     .GetLock                      = MDrv_Demod_MSATSC_51_GetLock,
     .GetSNR                       = MDrv_Demod_MSATSC_51_GetSNR,
     .GetBER                       = MDrv_Demod_MSATSC_51_GetBER,
     .GetPWR                       = MDrv_Demod_MSATSC_51_GetPWR,
     .GetSSI                       = MDrv_Demod_MSATSC_51_GetSSI,
     .GetQuality                   = MDrv_Demod_MSATSC_51_GetSignalQuality,
     .GetParam                     = MDrv_Demod_MSATSC_51_GetParam,
     .Restart                      = MDrv_Demod_MSATSC_51_Restart,
     .I2CByPass                    = MDrv_Demod_MSATSC_51_I2C_ByPass,
     .I2CByPassPreSetting          = NULL,
     .Extension_Function           = DEMOD_MSATSC_C_Extension_Function,
     .Extension_FunctionPreSetting = NULL,
     .Get_Packet_Error             = MDrv_Demod_null_Get_Packet_Error,
     .CheckExist                   = MDrv_Demod_null_CheckExist,
#if MS_DVBT2_INUSE
     .SetCurrentDemodType          = MDrv_Demod_null_SetCurrentDemodType,
     .GetCurrentDemodType          = MDrv_Demod_null_GetCurrentDemodType,
     .GetPlpBitMap                 = MDrv_Demod_null_GetPlpBitMap,
     .GetPlpGroupID                = MDrv_Demod_null_GetPlpGroupID,
     .SetPlpGroupID                = MDrv_Demod_null_SetPlpGroupID,
     .GetNextPLPID                 = MDrv_Demod_null_GetNextPLPID,
     .GetPLPType                   = MDrv_Demod_null_GetPLPType,
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
     .DiSEqC_SendCmd               = MDrv_Demod_null_DiSEqC_SendCmd,
     .DiSEqC_GetReply              = MDrv_Demod_null_DiSEqC_GetReply,
     .GetISIDInfo                  = MDrv_Demod_null_GetVCM_ISID_INFO,
     .SetISID                      = MDrv_Demod_null_SetVCM_ISID
#endif
};

#endif // (FRONTEND_DEMOD_TYPE == DEMOD_MSATSC_C)
#endif

