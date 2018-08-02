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

#if defined(CHIP_KELTIC)
#include "MsCommon.h"
#include "HbCommon.h"
#include "drvDMD_ATSC.h"
#include "drvDMD_common.h"
#include "drvDemod.h"
#include "drvDemodNull.h"
#include "drvDMD_VD_MBX.h"
#include "drvSAR.h"
#include <string.h>

#define COFDMDMD_MUTEX_TIMEOUT       (2000)
#define MAX_TUNER_NUM                2

#ifdef MS_DEBUG
#define DBG_MSB(x)  x
#else
#define DBG_MSB(x)
#endif

#undef DUAL_DMD
#define DUAL_DMD 0

//static MS_BOOL bInited = FALSE;
static MS_BOOL bInited[MAX_TUNER_NUM] = {FALSE,FALSE};
static MS_S32 _s32MutexId = -1;
DRV_DEMOD_TABLE_TYPE GET_DEMOD_ENTRY_NODE(DEMOD_MSKELTIC_ATSC) DDI_DRV_TABLE_ENTRY(demodtab);

const static DEMOD_MS_FE_IF TUNER_IF[] =
{
    {TUNER_MXL603,             5000},
    {TUNER_PHILIPS_TDA18250HN, 5000},
    {TUNER_TDA18250A,          5000},
    {TUNER_TDA18250B,          5000},
};

static MS_U32 _u32IFrequency[MAX_TUNER_NUM] = {36130, 36130};

//#if IF_THIS_TUNER_INUSE(TUNER_PHILIPS_TDA18250HN)||IF_THIS_TUNER_INUSE(TUNER_MXL603)||IF_THIS_TUNER_INUSE(TUNER_TDA18250A)||IF_THIS_TUNER_INUSE(TUNER_TDA18250B)
//const static MS_U32 _u32IFrequency = 5000;//KHz for demod restart&init
//const static MS_U32 _u32AGCReference = 0;
//#else
//const static MS_U32 _u32IFrequency = 36130;//KHz for demod restart&init
//#endif

#if 0
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
#endif

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
            DMD_ERR(("%s: Create mutex failed.\n", __FUNCTION__));
            return FALSE;
        }

    }


    MDrv_DMD_PreInit(); //register DEMOD base address
    _u32IFrequency[u8DemodIndex] = _getTunerIF(u8DemodIndex,pParam->pstTunertab->data);
    MDrv_SYS_DMD_VD_MBX_Init();
    MDrv_SYS_DMD_VD_MBX_SetType(E_DMD_VD_MBX_TYPE_ATSC);


    memset(&sDMD_ATSC_InitData, 0, sizeof(DMD_ATSC_InitData));

    // register init
    sDMD_ATSC_InitData.u8DMD_ATSC_DSPRegInitExt=NULL; // TODO use system variable type
    sDMD_ATSC_InitData.u8DMD_ATSC_DSPRegInitSize=0;
    sDMD_ATSC_InitData.u8DMD_ATSC_InitExt=NULL; // TODO use system variable type

    sDMD_ATSC_InitData.u16IF_KHZ = _u32IFrequency[u8DemodIndex];
    sDMD_ATSC_InitData.bIQSwap = FALSE;
    sDMD_ATSC_InitData.u8IS_DUAL = FALSE;
    sDMD_ATSC_InitData.bIsExtDemod = FALSE;
    //sDMD_ATSC_InitData.u16AGC_REFERENCE = _u32AGCReference;  //mark provisionally

    MDrv_DMD_ATSC_Initial_Hal_Interface();

    ret = MDrv_DMD_ATSC_Init(&sDMD_ATSC_InitData, sizeof(sDMD_ATSC_InitData)); // _UTOPIA

    ret &= MDrv_DMD_ATSC_SetConfig(DMD_ATSC_DEMOD_ATSC_256QAM, TRUE);

    if(ret == TRUE)
    {
        bInited[u8DemodIndex] = TRUE;
    }

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
        DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    //MDrv_DMD_DVBC_ActiveDmdSwitch(u8DemodIndex);
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

    //MDrv_DMD_DVBC_ActiveDmdSwitch(u8DemodIndex);
    MDrv_DMD_ATSC_SEL_DMD((eDMD_SEL)u8DemodIndex);
    //ret = MDrv_DMD_DVBC_Exit();
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
    //MDrv_DMD_DVBC_ActiveDmdSwitch(u8DemodIndex);
    MDrv_DMD_ATSC_SEL_DMD((eDMD_SEL)u8DemodIndex);
    //ret = MDrv_DMD_DVBC_SetSerialControl(bEnable);
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
    //MDrv_DMD_DVBC_ActiveDmdSwitch(u8DemodIndex);
    MDrv_DMD_ATSC_SEL_DMD((eDMD_SEL)u8DemodIndex);
    HB_ReleaseMutex(_s32MutexId);
    return TRUE;
}

MS_BOOL MDrv_Demod_MSATSC_51_GetLock(MS_U8 u8DemodIndex, EN_LOCK_STATUS *peLockStatus)
{
    //DMD_DVBC_LOCK_STATUS LockStatus;
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
    //MDrv_DMD_DVBC_ActiveDmdSwitch(u8DemodIndex);
    MDrv_DMD_ATSC_SEL_DMD((eDMD_SEL)u8DemodIndex);
    #if 0
    printf("[%s][%d] \n",__FUNCTION__,__LINE__);
    if(MDrv_DMD_DVBC_GetLock(DMD_DVBC_GETLOCK, &LockStatus) != TRUE)
    {
        HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }
    #endif

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
    //MS_BOOL ret;

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

    //MDrv_DMD_DVBC_ActiveDmdSwitch(u8DemodIndex);
    MDrv_DMD_ATSC_SEL_DMD((eDMD_SEL)u8DemodIndex);
    //ret = MDrv_DMD_DVBC_GetSignalQuality((MS_U16*)pu32SNR);
    *pfSNR = (float)MDrv_DMD_ATSC_GetSNRPercentage()*0.4;
    HB_ReleaseMutex(_s32MutexId);

    //return ret;
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
    //MDrv_DMD_DVBC_ActiveDmdSwitch(u8DemodIndex);
    MDrv_DMD_ATSC_SEL_DMD((eDMD_SEL)u8DemodIndex);
    //ret = MDrv_DMD_DVBC_GetPostViterbiBer(pfBER);
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
    //MDrv_DMD_DVBC_ActiveDmdSwitch(u8DemodIndex);
    MDrv_DMD_ATSC_SEL_DMD((eDMD_SEL)u8DemodIndex);
    //ret = MDrv_DMD_DVBC_GetSignalStrength((MS_U16*)ps32Signal);
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
    //MDrv_DMD_DVBC_ActiveDmdSwitch(u8DemodIndex);
    MDrv_DMD_ATSC_SEL_DMD((eDMD_SEL)u8DemodIndex);
    *pu16quality = (MS_U16)MDrv_DMD_ATSC_GetSNRPercentage();
    //ret = MDrv_DMD_ATSC_GetSignalQuality(pu16quality);

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
    //MDrv_DMD_DVBC_ActiveDmdSwitch(u8DemodIndex);
    MDrv_DMD_ATSC_SEL_DMD((eDMD_SEL)u8DemodIndex);
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
        DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
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
    #if 0
    DMD_DVBC_MODULATION_TYPE eModulationType;
    static DMD_DVBC_MODULATION_TYPE ePreModulationType = DMD_DVBC_QAMAUTO;
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
    MDrv_DMD_DVBC_ActiveDmdSwitch(u8DemodIndex);
    switch(pParam->CabParam.eConstellation)
    {
        case DEMOD_CAB_QAM16:
            eModulationType = DMD_DVBC_QAM16;
            break;
        case DEMOD_CAB_QAM32:
            eModulationType = DMD_DVBC_QAM32;
            break;
        case DEMOD_CAB_QAM64:
            eModulationType = DMD_DVBC_QAM64;
            break;
        case DEMOD_CAB_QAM128:
            eModulationType = DMD_DVBC_QAM128;
            break;
        case DEMOD_CAB_QAM256:
            eModulationType = DMD_DVBC_QAM256;
            break;
        default:
            eModulationType = DMD_DVBC_QAMAUTO;
            break;
    }

    //printf("pParam->CabParam.u16SymbolRate [%d], eModulationType [%d] \n",pParam->CabParam.u16SymbolRate,eModulationType);

    //if((ePreModulationType != eModulationType) || (u16PreSymbolRate != pParam->CabParam.u16SymbolRate))
    //{
        ePreModulationType = eModulationType;
        u16PreSymbolRate = pParam->CabParam.u16SymbolRate;
        DMD_DBG(("pParam->CabParam.u16SymbolRate [%d], eModulationType [%d] \n",pParam->CabParam.u16SymbolRate,eModulationType));

        if(FALSE == MDrv_DMD_DVBC_SetConfig(pParam->CabParam.u16SymbolRate, eModulationType, _u32IFrequency, FALSE, FALSE))
        {
            DMD_ERR(("MDrv_DMD_DVBC_SetConfig Fail \n"));
            HB_ReleaseMutex(_s32MutexId);
            return FALSE;
        }
        if(FALSE == MDrv_DMD_DVBC_SetActive(TRUE))
        {
            DMD_ERR(("MDrv_DMD_DVBC_SetActive Fail \n"));
            HB_ReleaseMutex(_s32MutexId);
            return FALSE;
        }
    //}

    HB_ReleaseMutex(_s32MutexId);
    #endif
    MDrv_DMD_ATSC_SEL_DMD((eDMD_SEL)u8DemodIndex);
    MDrv_DMD_ATSC_SetConfig(DMD_ATSC_DEMOD_ATSC_256QAM, TRUE);
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
        DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    //MDrv_DMD_DVBC_ActiveDmdSwitch(u8DemodIndex);
    MDrv_DMD_ATSC_SEL_DMD((eDMD_SEL)u8DemodIndex);
    HB_ReleaseMutex(_s32MutexId);
    return TRUE;
#endif
}

MS_BOOL MDrv_Demod_MSATSC_51_SetOutoutPath(MS_U8 u8DemodIndex, DEMOD_INTERFACE_MODE path)
{
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    //MDrv_DMD_DVBC_ActiveDmdSwitch(u8DemodIndex);
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
    //MDrv_DMD_DVBC_ActiveDmdSwitch(u8DemodIndex);
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
  if(GET_DEMOD_ENTRY_NODE(DEMOD_MSKELTIC_ATSC).I2CByPassPreSetting != NULL)
  {
      return GET_DEMOD_ENTRY_NODE(DEMOD_MSKELTIC_ATSC).I2CByPassPreSetting(u8DemodIndex,bOn);
  }
  else
      return MDrv_Demod_null_I2C_ByPass(u8DemodIndex,bOn);
}

DRV_DEMOD_TABLE_TYPE GET_DEMOD_ENTRY_NODE(DEMOD_MSKELTIC_ATSC) DDI_DRV_TABLE_ENTRY(demodtab) =
{
     .name                         = "DEMOD_MSKELTIC_ATSC",
     .data                         = DEMOD_MSKELTIC_ATSC,
     .init                         = MDrv_Demod_MSATSC_51_Init,
     .GetLock                      = MDrv_Demod_MSATSC_51_GetLock,
     .GetSNR                       = MDrv_Demod_MSATSC_51_GetSNR,
     .GetBER                       = MDrv_Demod_MSATSC_51_GetBER,
     .GetPWR                       = MDrv_Demod_MSATSC_51_GetPWR,
     .GetQuality                   = MDrv_Demod_MSATSC_51_GetSignalQuality,
     .GetParam                     = MDrv_Demod_MSATSC_51_GetParam,
     .Restart                      = MDrv_Demod_MSATSC_51_Restart,
     .I2CByPass                    = MDrv_Demod_MSATSC_51_I2C_ByPass,
     .Extension_Function           = DEMOD_MSKELTIC_ATSC_Extension_Function,
     .I2CByPassPreSetting          = NULL,
     .Extension_Function           = DEMOD_MSKELTIC_ATSC_Extension_Function,
     .Extension_FunctionPreSetting = NULL,
     .Get_Packet_Error             = MDrv_Demod_null_Get_Packet_Error,     
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
     .DiSEqC_SendCmd               = MDrv_Demod_null_DiSEqC_SendCmd
#endif
};

#endif // (FRONTEND_DEMOD_TYPE == DEMOD_MSATSC_C)

