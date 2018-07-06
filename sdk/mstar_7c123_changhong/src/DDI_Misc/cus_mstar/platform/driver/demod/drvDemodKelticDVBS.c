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
#if (MS_DVB_TYPE_SEL == DVBS)

#if defined(CHIP_KELTIC)

#include "MsCommon.h"
#include "HbCommon.h"
#include "drvDemod.h"
#include "drvDMD_INTERN_DVBS.h"
#include "drvDemodNull.h"
#include "drvDMD_VD_MBX.h"
#include "drvSAR.h"
#include "drvTuner.h"
//#if FRONTEND_TUNER_TYPE == TUNER_AV2028
#include "drvTuner_AV2028.h"
//#endif
#define COFDMDMD_MUTEX_TIMEOUT       (2000)

static MS_BOOL bInited = FALSE;
static MS_S32 _s32MutexId = -1;
static DEMOD_MS_INIT_PARAM   InitParam[MAX_FRONTEND_NUM];
DRV_DEMOD_TABLE_TYPE GET_DEMOD_ENTRY_NODE(DEMOD_MSKELTIC_DVBS) DDI_DRV_TABLE_ENTRY(demodtab);

const static DEMOD_MS_FE_IF TUNER_IF[] =
{
    {TUNER_MXL603,             5000},
    {TUNER_AV2012,             5000},
    {TUNER_AV2018,             5000},
    {TUNER_AV2028,             5000},
    {TUNER_TDA18250A,          5000},
    {TUNER_TDA18260,           5000},
};

static MS_U32 _u32IFrequency[MAX_FRONTEND_NUM] = {36130};

//#if ((FRONTEND_TUNER_TYPE == TUNER_AV2028 || FRONTEND_TUNER_TYPE == TUNER_MXL603 || FRONTEND_TUNER_TYPE == TUNER_TDA18250A|| FRONTEND_TUNER_TYPE == TUNER_TDA18260))
//const static MS_U32 _u32IFrequency = 5000;//KHz for demod restart&init
//#else
//const static MS_U32 _u32IFrequency = 36130;//KHz for demod restart&init
//#endif


static DMD_RFAGC_SSI ALPS_TUNER_RfagcSsi[] =
{
    {-15.00,    0x19},
    {-25.00,    0x31},
    {-26.00,    0x33},
    {-27.00,    0x35},
    {-28.00,    0x35},
    {-29.00,    0x36},
    {-30.00,    0x37},
    {-31.00,    0x38},
    {-32.00,    0x3A},
    {-33.00,    0x3E},
    {-34.00,    0x40},
    {-35.00,    0x41},
    {-36.00,    0x43},
    {-37.00,    0x44},
    {-38.00,    0x46},
    {-39.00,    0x47},
    {-40.00,    0x49},
    {-41.00,    0x4B},
    {-42.00,    0x4E},
    {-43.00,    0x50},
    {-44.00,    0x53},
    {-45.00,    0x56},
    {-46.00,    0x59},
    {-46.50,    0x5B},
    {-47.00,    0x5D},
    {-48.00,    0x62},
    {-49.00,    0x67},
    {-50.00,    0x6B},
    {-51.00,    0x73},
    {-52.00,    0x7A},
    {-53.00,    0x85},
    {-53.50,    0x8E},
    {-54.00,    0x98},
    {-54.10,    0x9F},
    {-54.20,    0xA4},
    {-54.30,    0xA7},
    {-54.40,    0xAC},
    {-55.00,    0xAC},
    {-55.00,    0xFF},
};

static DMD_IFAGC_SSI ALPS_TUNER_IfagcSsi_LoRef[] =
{
    {-54.30,    0x5F},
    {-54.40,    0x60},
    {-55.00,    0x63},
    {-56.00,    0x68},
    {-57.50,    0x6C},
    {-58.00,    0x70},
    {-59.00,    0x75},
    {-60.00,    0x79},
    {-61.40,    0x7D},
    {-62.00,    0x80},
    {-63.00,    0x81},
    {-64.00,    0x83},
    {-65.00,    0x84},
    {-66.00,    0x85},
    {-67.00,    0x87},
    {-68.00,    0x88},
    {-69.00,    0x89},
    {-70.00,    0x8A},
    {-71.00,    0x8C},
    {-72.00,    0x8D},
    {-73.00,    0x8F},
    {-74.00,    0x90},
    {-75.00,    0x92},
    {-76.00,    0x93},
    {-77.00,    0x95},
    {-78.00,    0x96},
    {-79.00,    0x98},
    {-80.00,    0x99},
    {-81.00,    0x9B},
    {-82.00,    0x9D},
    {-83.00,    0x9E},
    {-84.00,    0xA0},
    {-85.00,    0xA2},
    {-86.00,    0xA4},
    {-87.00,    0xA6},
    {-88.00,    0xA8},
    {-89.00,    0xAA},
    {-90.00,    0xAC},
    {-91.00,    0xAD},
    {-92.00,    0xAF},
    {-93.00,    0xB0},
    {-93.00,    0xFF},
};

static DMD_IFAGC_SSI ALPS_TUNER_IfagcSsi_HiRef[] =
{
    {-54.30,    0x5F},
    {-54.40,    0x60},
    {-55.00,    0x63},
    {-56.00,    0x68},
    {-57.50,    0x6C},
    {-58.00,    0x70},
    {-59.00,    0x75},
    {-60.00,    0x79},
    {-61.40,    0x7D},
    {-62.00,    0x80},
    {-63.00,    0x81},
    {-64.00,    0x83},
    {-65.00,    0x84},
    {-66.00,    0x85},
    {-67.00,    0x87},
    {-68.00,    0x88},
    {-69.00,    0x89},
    {-70.00,    0x8A},
    {-71.00,    0x8C},
    {-72.00,    0x8D},
    {-73.00,    0x8F},
    {-74.00,    0x90},
    {-75.00,    0x92},
    {-76.00,    0x93},
    {-77.00,    0x95},
    {-78.00,    0x96},
    {-79.00,    0x98},
    {-80.00,    0x99},
    {-81.00,    0x9B},
    {-82.00,    0x9D},
    {-83.00,    0x9E},
    {-84.00,    0xA0},
    {-85.00,    0xA2},
    {-86.00,    0xA4},
    {-87.00,    0xA6},
    {-88.00,    0xA8},
    {-89.00,    0xAA},
    {-90.00,    0xAC},
    {-91.00,    0xAD},
    {-92.00,    0xAF},
    {-93.00,    0xB0},
    {-93.00,    0xFF},
};

static DMD_IFAGC_ERR ALPS_TUNER_IfagcErr_LoRef[] =
{
    {0.00,        0x0A},
    {0.00,        0xFF},

};

static DMD_IFAGC_ERR ALPS_TUNER_IfagcErr_HiRef[] =
{
    {0.00,        0x2D},
    {0.00,        0xFF},        // stopper
};

static DMD_SQI_CN_NORDIGP1 SqiCnNordigP1[] =
{
    {_QPSK,  _CR1Y2, 5.1 },
    {_QPSK,  _CR2Y3, 6.9 },
    {_QPSK,  _CR3Y4, 7.9 },
    {_QPSK,  _CR5Y6, 8.9 },
    {_QPSK,  _CR7Y8, 9.7 },
    {_16QAM, _CR1Y2, 10.8},
    {_16QAM, _CR2Y3, 13.1},
    {_16QAM, _CR3Y4, 12.2},
    {_16QAM, _CR5Y6, 15.6},
    {_16QAM, _CR7Y8, 16.0},
    {_64QAM, _CR1Y2, 16.5},
    {_64QAM, _CR2Y3, 16.3},
    {_64QAM, _CR3Y4, 17.8},
    {_64QAM, _CR3Y4, 21.2},
    {_64QAM, _CR5Y6, 21.6},
    {_64QAM, _CR7Y8, 22.5},
};

static MS_U32 _getTunerIF(MS_U8 u8DemodIndex,MS_U8 u8Tuner)
{
   MS_U8 u8TblSize;
   MS_U8 i;
   MS_U32 u32IF = _u32IFrequency[u8DemodIndex];//defalut

   u8TblSize = sizeof(TUNER_IF)/sizeof(DEMOD_MS_FE_IF);
   for(i = 0; i<u8TblSize; i++)
   {
      if(u8Tuner == TUNER_IF[i].u8Tuner)
      {
         u32IF = TUNER_IF[i].u32IFFreq;
         break;
      }
   }
   return u32IF;
}

MS_BOOL MDrv_Keltic_DVBS_Demod_Init(MS_U8 u8DemodIndex,DEMOD_MS_INIT_PARAM* pParam)
{
    DMD_DVBS_InitData sDMD_DVBS_InitData;
    MS_BOOL ret;

    if (_s32MutexId < 0)
    {
        _s32MutexId = MsOS_CreateMutex(E_MSOS_FIFO, "OfDmd_Mutex", MSOS_PROCESS_SHARED);

        if (_s32MutexId < 0)
        {
            GEN_EXCEP;
            return FALSE;
        }

    }

    if(pParam->pstTunertab == NULL)
        return FALSE;
    else
        InitParam[u8DemodIndex].pstTunertab = pParam->pstTunertab;

     _u32IFrequency[u8DemodIndex] = _getTunerIF(u8DemodIndex,pParam->pstTunertab->data);
    MDrv_SYS_DMD_VD_MBX_Init();
    MDrv_SAR_Kpd_Init();

    static MS_U8 u8DMD_DVBS_InitExt[]={
       4, // version
       0, // reserved
       0x15, // TS_CLK
       1, // RFAGC tristate control default value, 1:trisate 0:non-tristate,never modify unless you know the meaning
       (MS_U8)(36167>>24), // IF Frequency
       (MS_U8)(36167>>16),
       (MS_U8)(36167>>8),
       (MS_U8)(36167>>0),
       (MS_U8)(24000>>24), // FS Frequency
       (MS_U8)(24000>>16),
       (MS_U8)(24000>>8),
       (MS_U8)(24000>>0),
       FRONTEND_DEMOD_IQ_SWAP, // IQ Swap
       FRONTEND_DEMOD_IQ_TYPE,//0, // u8ADCIQMode : 0=I path, 1=Q path, 2=both IQ
       0, // u8PadSel : 0=Normal, 1=analog pad
       0, // bPGAEnable : 0=disable, 1=enable
       5, // u8PGAGain : default 5
       (MS_U8)(AutoSymbol_Timeout>>8), // AutoSymbol_Timeout  10000ms~
       (MS_U8)(AutoSymbol_Timeout>>0),
       (MS_U8)(FixSymbol_AutoQam_Timeout>>8), // FixSymbol_AutoQam_Timeout 2000ms~
       (MS_U8)(FixSymbol_AutoQam_Timeout>>0),
       (MS_U8)(FixSymbol_FixQam_Timeout>>8), // FixSymbol_FixQam_Timeout  2000ms~
       (MS_U8)(FixSymbol_FixQam_Timeout>>0),
       };                // tuner parameter

    // tuner parameter
    sDMD_DVBS_InitData.u8SarChannel=1; // 0xFF means un-connected
    sDMD_DVBS_InitData.pTuner_RfagcSsi=ALPS_TUNER_RfagcSsi;
    sDMD_DVBS_InitData.u16Tuner_RfagcSsi_Size=sizeof(ALPS_TUNER_RfagcSsi)/sizeof(DMD_RFAGC_SSI);
    sDMD_DVBS_InitData.pTuner_IfagcSsi_LoRef=ALPS_TUNER_IfagcSsi_LoRef;
    sDMD_DVBS_InitData.u16Tuner_IfagcSsi_LoRef_Size=sizeof(ALPS_TUNER_IfagcSsi_LoRef)/sizeof(DMD_IFAGC_SSI);
    sDMD_DVBS_InitData.pTuner_IfagcSsi_HiRef=ALPS_TUNER_IfagcSsi_HiRef;
    sDMD_DVBS_InitData.u16Tuner_IfagcSsi_HiRef_Size=sizeof(ALPS_TUNER_IfagcSsi_HiRef)/sizeof(DMD_IFAGC_SSI);
    sDMD_DVBS_InitData.pTuner_IfagcErr_LoRef=ALPS_TUNER_IfagcErr_LoRef;
    sDMD_DVBS_InitData.u16Tuner_IfagcErr_LoRef_Size=sizeof(ALPS_TUNER_IfagcErr_LoRef)/sizeof(DMD_IFAGC_SSI);
    sDMD_DVBS_InitData.pTuner_IfagcErr_HiRef=ALPS_TUNER_IfagcErr_HiRef;
    sDMD_DVBS_InitData.u16Tuner_IfagcErr_HiRef_Size=sizeof(ALPS_TUNER_IfagcErr_HiRef)/sizeof(DMD_IFAGC_SSI);
    sDMD_DVBS_InitData.pSqiCnNordigP1=SqiCnNordigP1;
    sDMD_DVBS_InitData.u16SqiCnNordigP1_Size=sizeof(SqiCnNordigP1)/sizeof(DMD_SQI_CN_NORDIGP1);

    // register init
    sDMD_DVBS_InitData.u8DMD_DVBS_DSPRegInitExt=NULL; // TODO use system variable type
    sDMD_DVBS_InitData.u8DMD_DVBS_DSPRegInitSize=0;
    sDMD_DVBS_InitData.u8DMD_DVBS_InitExt=u8DMD_DVBS_InitExt; // TODO use system variable type

    ret = MDrv_DMD_DVBS_Init(&sDMD_DVBS_InitData, sizeof(sDMD_DVBS_InitData)); // _UTOPIA
    if(ret == TRUE)
    {
        bInited = TRUE;
    }
    //MDrv_DMD_DVBC_IQ(FRONTEND_DEMOD_IQ_TYPE);

    return ret;
}

MS_BOOL MDrv_Keltic_DVBS_Demod_Open(MS_U8 u8DemodIndex)
{
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    HB_ReleaseMutex(_s32MutexId);

    return TRUE;
}

MS_BOOL MDrv_Keltic_DVBS_Demod_Close(MS_U8 u8DemodIndex)
{
    MS_BOOL ret;

    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }


    ret = MDrv_DMD_DVBS_Exit();
    if(ret == TRUE)
    {
        bInited = FALSE;
    }
    HB_ReleaseMutex(_s32MutexId);
    return ret;

}

MS_BOOL MDrv_Keltic_DVBS_Demod_SetSerialControl(MS_U8 u8DemodIndex,MS_BOOL bEnable)
{
    MS_BOOL ret;

    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    if(bInited == FALSE)
    {
        HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }

    ret = MDrv_DMD_DVBS_SetSerialControl(bEnable);
    HB_ReleaseMutex(_s32MutexId);
    return ret;
}

MS_BOOL MDrv_Keltic_DVBS_Demod_PowerOnOff(MS_U8 u8DemodIndex,MS_BOOL bPowerOn)
{
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    HB_ReleaseMutex(_s32MutexId);
    return TRUE;
}

MS_BOOL MDrv_Keltic_DVBS_Demod_GetLock(MS_U8 u8DemodIndex,EN_LOCK_STATUS *peLockStatus)
{
    DMD_DVBS_LOCK_STATUS LockStatus;

    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    if(bInited == FALSE)
    {
        HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }


    if(MDrv_DMD_DVBS_GetLock(DMD_DVBS_GETLOCK, &LockStatus) != TRUE)
    {
        HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }
    switch (LockStatus)
    {
        case DMD_DVBS_LOCK:
            *peLockStatus = E_DEMOD_LOCK;
            break;
        case DMD_DVBS_CHECKEND:
            *peLockStatus = E_DEMOD_CHECKEND;
            break;
        case DMD_DVBS_UNLOCK:         
            *peLockStatus = E_DEMOD_UNLOCK;
            break;
        case DMD_DVBS_CHECKING:
        default:
            *peLockStatus = E_DEMOD_CHECKING;
            break;
    }

    HB_ReleaseMutex(_s32MutexId);
    return TRUE;
}

MS_BOOL MDrv_Keltic_DVBS_Demod_GetSNR(MS_U8 u8DemodIndex,float *pfBER)
{
    MS_BOOL ret;

    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    if(bInited == FALSE)
    {
        *pfBER = 0;
        HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }


    ret = MDrv_DMD_DVBS_GetSNR(pfBER);
    HB_ReleaseMutex(_s32MutexId);
    return ret;
}

MS_BOOL MDrv_Keltic_DVBS_Demod_GetBER(MS_U8 u8DemodIndex,float *pfBER)
{
    MS_BOOL ret;

    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    if(bInited == FALSE)
    {
        *pfBER = 0;
        HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }

    ret = MDrv_DMD_DVBS_GetPostViterbiBer(pfBER);

    HB_ReleaseMutex(_s32MutexId);
    return ret;

}

MS_BOOL MDrv_Keltic_DVBS_Demod_GetPWR(MS_U8 u8DemodIndex,MS_S32 *ps32Signal)
{
    MS_BOOL ret;
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    if(bInited == FALSE)
    {
        *ps32Signal = 0;
        HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }
    ret = MDrv_DMD_DVBS_GetSignalStrength((MS_U16*)ps32Signal);
    HB_ReleaseMutex(_s32MutexId);
    return ret;
}

MS_BOOL MDrv_Keltic_DVBS_Demod_GetSignalQuality(MS_U8 u8DemodIndex,MS_U16 *pu16quality)
{
    MS_BOOL ret;
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    if(bInited == FALSE)
    {
        *pu16quality = 0;
        HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }
    ret = MDrv_DMD_DVBS_GetSignalQuality(pu16quality);
    HB_ReleaseMutex(_s32MutexId);
    return ret;
}


MS_BOOL MDrv_Keltic_DVBS_Demod_Config(MS_U8 u8DemodIndex,MS_U8 *pRegParam)
{
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    HB_ReleaseMutex(_s32MutexId);
    return TRUE;
}


MS_BOOL MDrv_Keltic_DVBS_Demod_GetParam(MS_U8 u8DemodIndex, DEMOD_MS_FE_CARRIER_PARAM* pParam)
{
    DMD_DVBS_MODULATION_TYPE QAMMode;
    MS_U32 u32SymbolRate;
    float FreqOff;

    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }


    if(bInited == FALSE)
    {
        HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }

    if(FALSE == MDrv_DMD_DVBS_GetStatus(&QAMMode, &u32SymbolRate, &FreqOff))
    {
        HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }

    switch (QAMMode)
    {
        case DMD_DVBS_QPSK:
            pParam->SatParam.eConstellation = DEMOD_SAT_QPSK;
            break;
        case DMD_DVBS_8PSK:
            pParam->CabParam.eConstellation = DEMOD_SAT_8PSK;
            break;
        default:
            pParam->CabParam.eConstellation = DEMOD_SAT_QPSK;
            break;
    }

    pParam->CabParam.u16SymbolRate = (MS_U16)u32SymbolRate;
    pParam->CabParam.u32FreqOffset = FreqOff;

    HB_ReleaseMutex(_s32MutexId);

    return TRUE;

}

MS_BOOL MDrv_Keltic_DVBS_Demod_Restart(MS_U8 u8DemodIndex, DEMOD_MS_FE_CARRIER_PARAM* pParam,MS_U32 u32BroadCastType)
{
    DMD_DVBS_MODULATION_TYPE eModulationType;
    static DMD_DVBS_MODULATION_TYPE ePreModulationType = DEMOD_SAT_QPSK;
    static MS_U16 u16PreSymbolRate = 0;

    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    if(bInited == FALSE)
    {
        HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }

    switch(pParam->SatParam.eConstellation)
    {
        case DEMOD_SAT_QPSK:
            eModulationType = DMD_DVBS_QPSK;
            break;
        case DEMOD_SAT_8PSK:
            eModulationType = DMD_DVBS_8PSK;
            break;
        default:
            eModulationType = DMD_DVBS_QPSK;
            break;
    }

    //printf("pParam->CabParam.u16SymbolRate [%d], eModulationType [%d] \n",pParam->CabParam.u16SymbolRate,eModulationType);

    //if((ePreModulationType != eModulationType) || (u16PreSymbolRate != pParam->CabParam.u16SymbolRate))
    //{
        ePreModulationType = eModulationType;
        u16PreSymbolRate = pParam->SatParam.u32SymbolRate;
 #if 0
        if(FALSE == MDrv_DMD_DVBS_Demod_Restart(pParam->SatParam.u32SymbolRate, pParam->u32Frequency))
        {
            printf("MDrv_DMD_DVBS_Demod_Restart Fail \n");
            HB_ReleaseMutex(_s32MutexId);
            return FALSE;
        }
#endif
        //while(1);
 #if 1
        if(FALSE == MDrv_DMD_DVBS_SetConfig(pParam->SatParam.u32SymbolRate, eModulationType, _u32IFrequency[u8DemodIndex], FRONTEND_DEMOD_IQ_SWAP, FALSE))
        {
            DMD_ERR(("MDrv_DMD_DVBS_SetConfig Fail \n"));
            HB_ReleaseMutex(_s32MutexId);
            return FALSE;
        }
        if(FALSE == MDrv_DMD_DVBS_SetActive(TRUE))
        {
            DMD_ERR(("MDrv_DMD_DVBS_SetActive Fail \n"));
            HB_ReleaseMutex(_s32MutexId);
            return FALSE;
        }
#endif
    //}

    HB_ReleaseMutex(_s32MutexId);
    return TRUE;

}


MS_BOOL MDrv_Keltic_DVBS_Demod_SetMode(MS_U8 u8DemodIndex, Demod_Mode* pMode)
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
        DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    HB_ReleaseMutex(_s32MutexId);
    return TRUE;
#endif
}

MS_BOOL MDrv_Keltic_DVBS_Demod_SetOutoutPath(MS_U8 u8DemodIndex, DEMOD_INTERFACE_MODE path)
{
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    HB_ReleaseMutex(_s32MutexId);
    return TRUE;
}

DEMOD_INTERFACE_MODE MDrv_Keltic_DVBS_Demod_GetOutoutPath(MS_U8 u8DemodIndex)
{
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    HB_ReleaseMutex(_s32MutexId);
    return DEMOD_INTERFACE_PARALLEL;
}

MS_BOOL MDrv_Demod_Keltic_BlindScan_Start(MS_U8 u8DemodIndex, MS_U16 u16StartFreq,MS_U16 u16EndFreq)
{
    return MDrv_DMD_DVBS_BlindScan_Start(u16StartFreq,u16EndFreq);
}

MS_BOOL MDrv_Demod_Keltic_BlindScan_NextFreq(MS_U8 u8DemodIndex,MS_BOOL* bBlindScanEnd)
{
    MS_BOOL bRet;
	MS_U16 u16TunerCenterFreq;

	MDrv_DMD_DVBS_BlindScan_GetTunerFreq(&u16TunerCenterFreq);

	//MDrv_Tuner_SetFreq(u16TunerCenterFreq, 34000);
    bRet = InitParam[u8DemodIndex].pstTunertab->SetFreqS2(u8DemodIndex,u16TunerCenterFreq, 34000);
    bRet &= MDrv_DMD_DVBS_BlindScan_NextFreq(bBlindScanEnd);

    return bRet;
}

MS_BOOL MDrv_Demod_Keltic_BlindScan_WaitCurFreqFinished(MS_U8 u8DemodIndex,MS_U8* u8Progress,MS_U8 *u8FindNum)
{
    return MDrv_DMD_DVBS_BlindScan_WaitCurFreqFinished(u8Progress,u8FindNum);
}

MS_BOOL MDrv_Demod_Keltic_BlindScan_GetChannel(MS_U8 u8DemodIndex,MS_U16 u16ReadStart,MS_U16* u16TPNum,DEMOD_MS_FE_CARRIER_PARAM *pTable)
{
    return MDrv_DMD_DVBS_BlindScan_GetChannel(u16ReadStart,u16TPNum,(HAL_DEMOD_MS_FE_CARRIER_PARAM*)pTable );
}

MS_BOOL MDrv_Demod_Keltic_BlindScan_Cancel(MS_U8 u8DemodIndex)
{
    return MDrv_DMD_DVBS_BlindScan_Cancel();
}

MS_BOOL MDrv_Demod_Keltic_BlindScan_End(MS_U8 u8DemodIndex)
{
    return MDrv_DMD_DVBS_BlindScan_End();
}

MS_BOOL MDrv_Demod_Keltic_BlindScan_GetCurrentFreq(MS_U8 u8DemodIndex,MS_U32 *u32CurrentFeq)
{
    return MDrv_DMD_DVBS_BlindScan_GetCurrentFreq(u32CurrentFeq);
}

MS_BOOL MDrv_Demod_Keltic_DiSEqC_SetTone(MS_U8 u8DemodIndex, MS_BOOL bTone1)
{
    return MDrv_DMD_DVBS_DiSEqC_SetTone(bTone1);
}

MS_BOOL MDrv_Demod_Keltic_DiSEqC_SetLNBOut(MS_U8 u8DemodIndex, MS_BOOL bLow)
{
    return MDrv_DMD_DVBS_DiSEqC_SetLNBOut(bLow);
}

MS_BOOL MDrv_Demod_Keltic_DiSEqC_GetLNBOut(MS_U8 u8DemodIndex, MS_BOOL* bLNBOutLow)
{
    return MDrv_DMD_DVBS_DiSEqC_GetLNBOut(bLNBOutLow);
}

MS_BOOL MDrv_Demod_Keltic_DiSEqC_Set22kOnOff(MS_U8 u8DemodIndex, MS_BOOL b22kOn)
{
    return MDrv_DMD_DVBS_DiSEqC_Set22kOnOff(b22kOn); 
}

MS_BOOL MDrv_Demod_Keltic_DiSEqC_Get22kOnOff(MS_U8 u8DemodIndex, MS_BOOL* b22kOn)
{
    return MDrv_DMD_DVBS_DiSEqC_Get22kOnOff(b22kOn);
}

MS_BOOL MDrv_Demod_Keltic_DiSEqC_SendCmd(MS_U8 u8DemodIndex, MS_U8* pCmd,MS_U8 u8CmdSize)
{
    return MDrv_DMD_DVBS_DiSEqC_SendCmd(pCmd,u8CmdSize);
}

MS_BOOL DEMOD_MSKELTIC_DVBS_Extension_Function(MS_U8 u8DemodIndex, DEMOD_EXT_FUNCTION_TYPE fuction_type, void *data)
{
    switch(fuction_type)
    {
        default:
            DMD_ERR(("Request extension function (%x) does not exist\n",fuction_type));
            return TRUE;
    }
}

MS_BOOL MDrv_Keltic_DVBS_Demod_I2C_ByPass(MS_U8 u8DemodIndex,MS_BOOL bOn)
{
  if(GET_DEMOD_ENTRY_NODE(DEMOD_MSKELTIC_DVBS).I2CByPassPreSetting != NULL)
  {
      return GET_DEMOD_ENTRY_NODE(DEMOD_MSKELTIC_DVBS).I2CByPassPreSetting(u8DemodIndex,bOn);
  }
  else
      return MDrv_Demod_null_I2C_ByPass(u8DemodIndex,bOn);
}

DRV_DEMOD_TABLE_TYPE GET_DEMOD_ENTRY_NODE(DEMOD_MSKELTIC_DVBS) DDI_DRV_TABLE_ENTRY(demodtab) =
{
     .name                         = "DEMOD_MSKELTIC_DVBS",
     .data                         = DEMOD_MSKELTIC_DVBS,
     .init                         = MDrv_Keltic_DVBS_Demod_Init,
     .GetLock                      = MDrv_Keltic_DVBS_Demod_GetLock,
     .GetSNR                       = MDrv_Keltic_DVBS_Demod_GetSNR,
     .GetBER                       = MDrv_Keltic_DVBS_Demod_GetBER,
     .GetPWR                       = MDrv_Keltic_DVBS_Demod_GetPWR,
     .GetQuality                   = MDrv_Keltic_DVBS_Demod_GetSignalQuality,
     .GetParam                     = MDrv_Keltic_DVBS_Demod_GetParam,
     .Restart                      = MDrv_Keltic_DVBS_Demod_Restart,
     .I2CByPass                    = MDrv_Keltic_DVBS_Demod_I2C_ByPass,
     .I2CByPassPreSetting          = NULL,
     .Extension_Function           = DEMOD_MSKELTIC_DVBS_Extension_Function,
     .Extension_FunctionPreSetting = NULL,
#if MS_DVBT2_INUSE
     .SetCurrentDemodType          = MDrv_Demod_null_SetCurrentDemodType,
     .GetCurrentDemodType          = MDrv_Demod_null_GetCurrentDemodType,
     .GetPlpBitMap                 = MDrv_Demod_null_GetPlpBitMap,
     .GetPlpGroupID                = MDrv_Demod_null_GetPlpGroupID,
     .SetPlpGroupID                = MDrv_Demod_null_SetPlpGroupID,
#endif
#if MS_DVBS_INUSE
     .BlindScanStart               = MDrv_Demod_Keltic_BlindScan_Start,
     .BlindScanNextFreq            = MDrv_Demod_Keltic_BlindScan_NextFreq,
     .BlindScanWaitCurFreqFinished = MDrv_Demod_Keltic_BlindScan_WaitCurFreqFinished,
     .BlindScanCancel              = MDrv_Demod_Keltic_BlindScan_Cancel,
     .BlindScanEnd                 = MDrv_Demod_Keltic_BlindScan_End,
     .BlindScanGetChannel          = MDrv_Demod_Keltic_BlindScan_GetChannel,
     .BlindScanGetCurrentFreq      = MDrv_Demod_Keltic_BlindScan_GetCurrentFreq,
     .DiSEqCSetTone                = MDrv_Demod_Keltic_DiSEqC_SetTone,
     .DiSEqCSetLNBOut              = MDrv_Demod_Keltic_DiSEqC_SetLNBOut,
     .DiSEqCGetLNBOut              = MDrv_Demod_Keltic_DiSEqC_GetLNBOut,
     .DiSEqCSet22kOnOff            = MDrv_Demod_Keltic_DiSEqC_Set22kOnOff,
     .DiSEqCGet22kOnOff            = MDrv_Demod_Keltic_DiSEqC_Get22kOnOff,
     .DiSEqC_SendCmd               = MDrv_Demod_Keltic_DiSEqC_SendCmd
#endif
};

#endif // (FRONTEND_DEMOD_TYPE == DEMOD_MSDVBS_51)
#endif

