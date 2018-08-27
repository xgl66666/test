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
//#include "Board.h"
#include "Frontend_Enum_Device.h"
#if IS_THIS_DEMOD_PICKED(DEMOD_MSINTERN_DVBS)
#if defined(CHIP_KRATOS) || defined(CHIP_KAYLA) || defined(CHIP_KRIS) || defined(CHIP_KELTIC) || defined(CHIP_K6LITE) || defined(CHIP_K5TN)
#include "MsCommon.h"
#include "HbCommon.h"
#include "drvDemod.h"
#include "drvDMD_INTERN_DVBS.h"
#include "drvDemodNull.h"
#include "drvDMD_VD_MBX.h"
#include "drvSAR.h"
#include "drvTuner.h"
#include "MsMemory.h"
#include <string.h>
#include <math.h>

#define COFDMDMD_MUTEX_TIMEOUT       (2000)
//#define DBG_MODE_AGC_OUT  0x03
//#define DBG_MODE_AGC_ERR  0x05
//#define DBG_MODE_AGC_MEAN 0x0a
//------------------------------DEMOD_DVBC_timeout/DEMOD_DVBS_timeout--------
#ifndef AutoSymbol_Timeout
#define AutoSymbol_Timeout          10000
#endif
#ifndef FixSymbol_AutoQam_Timeout
#define FixSymbol_AutoQam_Timeout   2000
#endif
#ifndef FixSymbol_FixQam_Timeout
#define FixSymbol_FixQam_Timeout    2000
#endif

static MS_BOOL bInited = FALSE;
static MS_S32 _s32MutexId = -1;
static MS_BOOL bIQSwap = FALSE;
static DEMOD_MS_INIT_PARAM  MS_DVBS_INIT_PARAM;
DRV_DEMOD_TABLE_TYPE GET_DEMOD_ENTRY_NODE(DEMOD_MSINTERN_DVBS) DDI_DRV_TABLE_ENTRY(demodtab);
static MS_U16 Blindscan_EndFreq = 2150;
static MS_BOOL bIsBlindGetTPList;
static MS_BOOL bDiSeqc_Tx22K_OFF = FALSE;
static DMD_RFAGC_SSI* pRFAGC_SSI_TBL = NULL;
static MS_U16 u16RFAGC_SSI_TBL_Size = 0;


//RDA5815
static DMD_RFAGC_SSI TUNER_RfagcSsi_RDA5815M[] =
{
    {  256,      92},
    {  21736,    90},
    {  23336,    88},
    {  24246,    86},
    {  25236,    84},
    {  25696,    82},
    {  26926,    80},
    {  28336 ,   78},
    {  29186 ,   76},
    {  29606 ,   74},
    {  30176 ,   72},
    {  30516 ,   70},
    {  30856 ,   68},
    {  31196 ,   66},
    {  31556 ,   64},
    {  31956 ,   62},
    {  32376 ,   60},
    {  32726 ,   58},
    {  32996 ,   56},
    {  33576 ,   54},
    {  34086 ,   52},
    {  34396 ,   50},
    {  34956 ,   48},
    {  35456 ,   46},
    {  35946 ,   44},
    {  36396 ,   42},
    {  36886 ,   40},
    {  37196 ,   38},
    {  37576 ,   36},
    {  37846 ,   34},
    {  38236 ,   32},
    {  38756 ,   30},
    {  39326 ,   28},
    {  39996 ,   26},
    {  40486 ,   24},
    {  40936 ,   22},
    {  41466 ,   20},
    {  41956 ,   18},
    {  42366 ,   16},
    {  42766 ,   14},
    {  43096 ,   12},
    {  43486 ,   10},
    {  43706 ,    8},
    {  43836 ,    6},
    {  43956 ,    4},
    {  44016 ,    2},
    {  44086 ,    0},

};

//AV2017
static DMD_RFAGC_SSI TUNER_RfagcSsi_AV2017[] =
{
    {  256,      92},
    {  256,      90},
    {  256,      88},
    {  22156,    86},
    {  28651,    84},
    {  29207,    82},
    {  29593,    80},
    {  29897 ,   78},
    {  30149 ,   76},
    {  30441 ,   74},
    {  30737 ,   72},
    {  30879 ,   70},
    {  31143 ,   68},
    {  31360 ,   66},
    {  31568 ,   64},
    {  31812 ,   62},
    {  32050 ,   60},
    {  32395 ,   58},
    {  32580 ,   56},
    {  33137 ,   54},
    {  33798 ,   52},
    {  33987 ,   50},
    {  34103 ,   48},
    {  34904 ,   46},
    //{  35485 ,   44},
    //{  35364 ,   42},
    //{  35226 ,   40},
    {  35489 ,   42},
    {  36074 ,   38},
    {  36672 ,   36},
    {  37092 ,   34},
    {  37450 ,   32},
    {  37770 ,   30},
    {  38049 ,   28},
    {  38305 ,   26},
    {  38532 ,   24},
    {  38812 ,   22},
    {  39044 ,   20},
    {  39209 ,   18},
    {  39379 ,   16},
    {  39533 ,   14},
    {  39651 ,   12},
    {  39783 ,   10},
    {  39896 ,    8},
    {  40008 ,    6},
    {  40130 ,    4},
    {  40230 ,    2},
    {  40295 ,    0},

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

static MS_BOOL _IQ_SWAP(MS_U32 u32TunerType)
{
#if defined(CHIP_KRATOS)
   if((u32TunerType == TUNER_AV2018) || (u32TunerType == TUNER_AV2012) || (u32TunerType == TUNER_RDA5815M))
       return TRUE;
   else
       return FALSE;
#else
   return FALSE;
#endif
}

#if (defined(UFO_DEMOD_DVBS_SUPPORT_DMD_INT) && INTERN_DVBS_SUPPORT_DMD_INT)
static void _mdrv_msdvbs_demod_cb(MS_U8 u8arg)
{
    if(MS_DVBS_INIT_PARAM.fpCB!= NULL)
    {
        MS_DVBS_INIT_PARAM.fpCB(MS_DVBS_INIT_PARAM.u8FrontendIndex,u8arg);
    }
}
#endif

MS_BOOL MDrv_MSDVBS_Demod_Init(MS_U8 u8DemodIndex,DEMOD_MS_INIT_PARAM* pParam)
{
    DMD_DVBS_InitData sDMD_DVBS_InitData;
    MS_BOOL ret = TRUE;
    MS_U16 u16FWVer=0;

    if (_s32MutexId < 0)
    {
        _s32MutexId = MsOS_CreateMutex(E_MSOS_FIFO, "OfDmd_Mutex", MSOS_PROCESS_SHARED);

        if (_s32MutexId < 0)
        {
            return FALSE;
        }

    }

    if(pParam->pstTunertab == NULL)
        return FALSE;
    else
    {
        memcpy(&MS_DVBS_INIT_PARAM, pParam, sizeof(DEMOD_MS_INIT_PARAM));
    }

    memset(&sDMD_DVBS_InitData, 0x00, sizeof(DMD_DVBS_InitData));
    MDrv_SYS_DMD_VD_MBX_Init();
    MDrv_SAR_Kpd_Init();
    bIQSwap = _IQ_SWAP(pParam->pstTunertab->data);

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
       0, // IQ Swap
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
    if(TUNER_RDA5815M == pParam->pstTunertab->data)
    {
      pRFAGC_SSI_TBL = TUNER_RfagcSsi_RDA5815M;
      u16RFAGC_SSI_TBL_Size = sizeof(TUNER_RfagcSsi_RDA5815M)/sizeof(DMD_RFAGC_SSI);
    }
    else if(TUNER_AV2017 == pParam->pstTunertab->data)
    {
      pRFAGC_SSI_TBL = TUNER_RfagcSsi_AV2017;
      u16RFAGC_SSI_TBL_Size = sizeof(TUNER_RfagcSsi_AV2017)/sizeof(DMD_RFAGC_SSI);
    }
    else //deafult
    {
      pRFAGC_SSI_TBL = TUNER_RfagcSsi_AV2017;
      u16RFAGC_SSI_TBL_Size = sizeof(TUNER_RfagcSsi_AV2017)/sizeof(DMD_RFAGC_SSI);
    }
    u8DMD_DVBS_InitExt[12] = bIQSwap;
    sDMD_DVBS_InitData.u8SarChannel=0xff; // 0xFF means un-connected
    sDMD_DVBS_InitData.pTuner_RfagcSsi= pRFAGC_SSI_TBL;
    sDMD_DVBS_InitData.u16Tuner_RfagcSsi_Size=u16RFAGC_SSI_TBL_Size;
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
#ifdef UFO_SUPPORT_VCM
#ifdef INTERNAL_DVBS2_DJB_ADR
    ret &=  MDrv_DMD_DVBS2_VCM_Init(VCM_MODE, 0, INTERNAL_DVBS2_DJB_ADR);
#endif
#endif
    ret &=  MDrv_DMD_DVBS_Init(&sDMD_DVBS_InitData, sizeof(sDMD_DVBS_InitData)); // _UTOPIA

    MsOS_DelayTask(10);
    ret &= MDrv_DMD_DVBS_DiSEqC_Init();
    ret &= MDrv_DMD_DVBS_DiSEqC_SetTxToneMode(bDiSeqc_Tx22K_OFF);

    if(ret == TRUE)
    {
        bInited = TRUE;
        #if (defined(UFO_DEMOD_DVBS_SUPPORT_DMD_INT) && INTERN_DVBS_SUPPORT_DMD_INT)
        MDrv_DMD_DVBS_Reg_INT_CB(_mdrv_msdvbs_demod_cb);
        #endif
    }
    //MDrv_DMD_DVBC_IQ(FRONTEND_DEMOD_IQ_TYPE);

    MDrv_DMD_DVBS_GetFWVer(&u16FWVer);
    DMD_DBG(("DVBS DMD FW Ver. %x\n", u16FWVer));
    bIsBlindGetTPList = FALSE;

    return ret;
}

MS_BOOL MDrv_MSDVBS_Demod_Open(MS_U8 u8DemodIndex)
{
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    HB_ReleaseMutex(_s32MutexId);

    return TRUE;
}

MS_BOOL MDrv_MSDVBS_Demod_Close(MS_U8 u8DemodIndex)
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

MS_BOOL MDrv_MSDVBS_Demod_SetSerialControl(MS_U8 u8DemodIndex,MS_BOOL bEnable)
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

MS_BOOL MDrv_MSDVBS_Demod_PowerOnOff(MS_U8 u8DemodIndex,MS_BOOL bPowerOn)
{
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    HB_ReleaseMutex(_s32MutexId);
    return TRUE;
}

MS_BOOL MDrv_MSDVBS_Demod_GetLock(MS_U8 u8DemodIndex,EN_LOCK_STATUS *peLockStatus)
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

MS_BOOL MDrv_MSDVBS_Demod_GetSNR(MS_U8 u8DemodIndex,float *pfSNR)
{
    MS_BOOL ret;
    #ifdef UFO_SUPPORT_KERNEL_FLOATING
    MS_FLOAT_ST stSNR;
    #endif

    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    if(bInited == FALSE)
    {
        *pfSNR = 0;
        HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }


    #ifdef UFO_SUPPORT_KERNEL_FLOATING
    ret = MDrv_DMD_DVBS_GetSNR_Kernel(&stSNR);
    if(ret)
    {
        *pfSNR = (float)(stSNR.DATA*pow(2,stSNR.EXP));
    }
    #else
    ret = MDrv_DMD_DVBS_GetSNR(pfSNR);
    #endif
    HB_ReleaseMutex(_s32MutexId);
    return ret;
}

MS_BOOL MDrv_MSDVBS_Demod_GetBER(MS_U8 u8DemodIndex,float *pfBER)
{
    MS_BOOL ret;
    #ifdef UFO_SUPPORT_KERNEL_FLOATING
    MS_FLOAT_ST stBER;
    #endif

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

#ifdef UFO_SUPPORT_KERNEL_FLOATING
    ret = MDrv_DMD_DVBS_GetBER_Kernel(&stBER);
    if(ret)
    {
        *pfBER = (float)(stBER.DATA*pow(2,stBER.EXP));
    }

#else
    ret = MDrv_DMD_DVBS_GetPostViterbiBer(pfBER);
#endif

    HB_ReleaseMutex(_s32MutexId);
    return ret;

}

MS_BOOL MDrv_MSDVBS_Demod_GetPWR(MS_U8 u8DemodIndex,MS_S32 *ps32Signal)
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
    

    #ifdef UFO_DEMOD_GET_SIGNAL_LEVEL_PWR
    float fPWR;   
    if(TUNER_RT710 == MS_DVBS_INIT_PARAM.pstTunertab->data)
    {
        ret = MS_DVBS_INIT_PARAM.pstTunertab->Extension_Function(u8DemodIndex,TUNER_EXT_FUNC_GET_POWER_LEVEL, ps32Signal);
    }
    else
    {
        ret = MDrv_DMD_DVBS_GetTunrSignalLevel_PWR(&fPWR);
        *ps32Signal = (MS_S32)(fPWR);
    }
    #else
        ret = MS_DVBS_INIT_PARAM.pstTunertab->Extension_Function(u8DemodIndex,TUNER_EXT_FUNC_GET_POWER_LEVEL, ps32Signal);
    #endif

    HB_ReleaseMutex(_s32MutexId);
    return ret;
}

MS_BOOL MDrv_MSDVBS_Demod_GetSSI(MS_U8 u8DemodIndex,MS_U16 *pu16SSI)
{
    MS_BOOL ret = TRUE;
    float fRFPowerDbm = 0;
    int PowerLevel = 0;
        
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    if(bInited == FALSE)
    {
        *pu16SSI = 0;
        HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }
    ret &= MS_DVBS_INIT_PARAM.pstTunertab->Extension_Function(u8DemodIndex,TUNER_EXT_FUNC_GET_POWER_LEVEL, &PowerLevel);
    fRFPowerDbm = (float)(PowerLevel);

    if(fRFPowerDbm == 0)
    {
        ret &= MDrv_DMD_DVBS_GetSignalStrength(pu16SSI);
    }
    else
    {
        ret &= MDrv_DMD_DVBS_GetSignalStrengthWithRFPower(pu16SSI, fRFPowerDbm);
    }
    HB_ReleaseMutex(_s32MutexId);
    return ret;
}


MS_BOOL MDrv_MSDVBS_Demod_GetSignalQuality(MS_U8 u8DemodIndex,MS_U16 *pu16quality)
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


MS_BOOL MDrv_MSDVBS_Demod_Config(MS_U8 u8DemodIndex,MS_U8 *pRegParam)
{
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    HB_ReleaseMutex(_s32MutexId);
    return TRUE;
}


MS_BOOL MDrv_MSDVBS_Demod_GetParam(MS_U8 u8DemodIndex, DEMOD_MS_FE_CARRIER_PARAM* pParam)
{
#ifndef UFO_DEMOD_GetParam_NEW_FLOW
    DMD_DVBS_MODULATION_TYPE QAMMode;
    MS_U32 u32SymbolRate;
    float FreqOff;
#else    
    HAL_DEMOD_MS_SAT_CARRIER_PARAM stParam;
#endif

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

#ifdef UFO_DEMOD_GetParam_NEW_FLOW
    memset(&stParam, 0, sizeof(HAL_DEMOD_MS_SAT_CARRIER_PARAM));
    if(FALSE == MDrv_DMD_DVBS_GetParam(&stParam))
    {
        HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }
    pParam->SatParam.eConstellation = stParam.eConstellation;
    pParam->SatParam.eRollOff = stParam.eRollOff; 
    pParam->SatParam.fCFO = stParam.eFreqoff; 
    pParam->SatParam.eIQ_Mode = stParam.eIQ_Mode; 
    pParam->SatParam.eCodeRate = (DEMOD_EN_CONV_CODE_RATE_TYPE)stParam.eCodeRate;

    if(stParam.eDemodType == HAL_DEMOD_SAT_DVBS2)
        pParam->SatParam.bIsDVBS2 = TRUE;
    else
        pParam->SatParam.bIsDVBS2 = FALSE;
#else
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
            pParam->SatParam.eConstellation = DEMOD_SAT_8PSK;
            break;
        default:
            pParam->SatParam.eConstellation = DEMOD_SAT_QPSK;
            break;
    }

    pParam->SatParam.u32SymbolRate = u32SymbolRate;
    pParam->SatParam.fCFO = FreqOff;
#endif

    HB_ReleaseMutex(_s32MutexId);

    return TRUE;

}

MS_BOOL MDrv_MSDVBS_Demod_Restart(MS_U8 u8DemodIndex, DEMOD_MS_FE_CARRIER_PARAM* pParam,MS_U32 u32BroadCastType)
{
    DMD_DVBS_MODULATION_TYPE eModulationType;

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
        //ePreModulationType = eModulationType;
        //u16PreSymbolRate = pParam->SatParam.u32SymbolRate;
 #if 0
        if(FALSE == MDrv_DMD_DVBS_Demod_Restart(pParam->SatParam.u32SymbolRate, pParam->u32Frequency))
        {
            printf("MDrv_DMD_DVBS_Demod_Restart Fail \n");
            HB_ReleaseMutex(_s32MutexId);
            return FALSE;
        }
#endif

#ifdef UFO_DEMOD_BLINDSCAN_NEW_FLOW
        if(    bIsBlindGetTPList)
        {
            if(FALSE == MDrv_DMD_DVBS_BlindScan_Config(pParam->SatParam.u32SymbolRate, eModulationType, 0, bIQSwap, FALSE))
            {
                DMD_ERR(("MDrv_DMD_DVBS_SetConfig Fail \n"));
                HB_ReleaseMutex(_s32MutexId);
                return FALSE;
            }   
        }

        else
#endif            
        {
        #ifdef UFO_SUPPORT_VCM
             MDrv_DMD_DVBS2_VCM_Change_Mode((DMD_DVBS_VCM_OPT)pParam->SatParam.u8VCM_MODE);
             MDrv_DMD_DVBS2_Set_IS_ID(pParam->SatParam.u8VCM_IS_ID);
        #endif
            if(FALSE == MDrv_DMD_DVBS_SetConfig(pParam->SatParam.u32SymbolRate, eModulationType, 0, bIQSwap, FALSE))
            {
                DMD_ERR(("MDrv_DMD_DVBS_SetConfig Fail \n"));
                HB_ReleaseMutex(_s32MutexId);
                return FALSE;
            }
        }

        if(FALSE == MDrv_DMD_DVBS_SetActive(TRUE))
        {
            DMD_ERR(("MDrv_DMD_DVBS_SetActive Fail \n"));
            HB_ReleaseMutex(_s32MutexId);
            return FALSE;
        }


    HB_ReleaseMutex(_s32MutexId);
    return TRUE;

}


MS_BOOL MDrv_MSDVBS_Demod_SetMode(MS_U8 u8DemodIndex, Demod_Mode* pMode)
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

MS_BOOL MDrv_MSDVBS_Demod_SetOutoutPath(MS_U8 u8DemodIndex, DEMOD_INTERFACE_MODE path)
{
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    HB_ReleaseMutex(_s32MutexId);
    return TRUE;
}

DEMOD_INTERFACE_MODE MDrv_MSDVBS_Demod_GetOutoutPath(MS_U8 u8DemodIndex)
{
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    HB_ReleaseMutex(_s32MutexId);
    return DEMOD_INTERFACE_PARALLEL;
}

MS_BOOL MDrv_MSDVBS_Demod_BlindScan_Start(MS_U8 u8DemodIndex, MS_U16 u16StartFreq,MS_U16 u16EndFreq)
{
    Blindscan_EndFreq = u16EndFreq;
    bIsBlindGetTPList = TRUE;
   // printf("MDrv_Demod_Kratos_BlindScan_Start\n");
    return MDrv_DMD_DVBS_BlindScan_Start(u16StartFreq,u16EndFreq);
}

MS_BOOL MDrv_MSDVBS_Demod_BlindScan_NextFreq(MS_U8 u8DemodIndex,MS_BOOL* bBlindScanEnd)
{
    MS_BOOL bRet;
    MS_U16 u16TunerCenterFreq = 0;
    MS_U16 u16TunerCutOffFreq = 0;


    MDrv_DMD_DVBS_BlindScan_GetTunerFreq(&u16TunerCenterFreq, &u16TunerCutOffFreq);
   // printf("MDrv_Demod_Kratos_BlindScan_NextFreq, u16TunerCenterFreq = %d\n",(int)u16TunerCenterFreq);

        if(u16TunerCenterFreq > Blindscan_EndFreq){
        *bBlindScanEnd = TRUE;
        return TRUE;
        }
    //MDrv_Tuner_SetFreq(u16TunerCenterFreq, 34000);
    bRet = MS_DVBS_INIT_PARAM.pstTunertab->SetFreqS2(u8DemodIndex,u16TunerCenterFreq, u16TunerCutOffFreq);
    bRet &= MDrv_DMD_DVBS_BlindScan_NextFreq(bBlindScanEnd);

    return bRet;
}

MS_BOOL MDrv_MSDVBS_Demod_BlindScan_WaitCurFreqFinished(MS_U8 u8DemodIndex,MS_U8* u8Progress,MS_U8 *u8FindNum)
{
    return MDrv_DMD_DVBS_BlindScan_WaitCurFreqFinished(u8Progress,u8FindNum);
}

MS_BOOL MDrv_MSDVBS_Demod_BlindScan_GetChannel(MS_U8 u8DemodIndex,MS_U16 u16ReadStart,MS_U16* u16TPNum,DEMOD_MS_FE_CARRIER_PARAM *pTable)
{
    return MDrv_DMD_DVBS_BlindScan_GetChannel(u16ReadStart,u16TPNum,(HAL_DEMOD_MS_FE_CARRIER_PARAM*)pTable );
}

MS_BOOL MDrv_MSDVBS_Demod_BlindScan_Cancel(MS_U8 u8DemodIndex)
{
    bIsBlindGetTPList = FALSE;
    return MDrv_DMD_DVBS_BlindScan_Cancel();
}

MS_BOOL MDrv_MSDVBS_Demod_BlindScan_End(MS_U8 u8DemodIndex)
{
    bIsBlindGetTPList = FALSE;
    return MDrv_DMD_DVBS_BlindScan_End();
}

MS_BOOL MDrv_MSDVBS_Demod_BlindScan_GetCurrentFreq(MS_U8 u8DemodIndex,MS_U32 *u32CurrentFeq)
{
    return MDrv_DMD_DVBS_BlindScan_GetCurrentFreq(u32CurrentFeq);
}

MS_BOOL MDrv_MSDVBS_Demod_DiSEqC_SetTone(MS_U8 u8DemodIndex, MS_BOOL bTone1)
{
    return MDrv_DMD_DVBS_DiSEqC_SetTone(bTone1);
}

MS_BOOL MDrv_MSDVBS_Demod_DiSEqC_SetLNBOut(MS_U8 u8DemodIndex, MS_BOOL bLow)
{
    return MDrv_DMD_DVBS_DiSEqC_SetLNBOut(bLow);
}

MS_BOOL MDrv_MSDVBS_Demod_DiSEqC_GetLNBOut(MS_U8 u8DemodIndex, MS_BOOL* bLNBOutLow)
{
    return MDrv_DMD_DVBS_DiSEqC_GetLNBOut(bLNBOutLow);
}

MS_BOOL MDrv_MSDVBS_Demod_DiSEqC_Set22kOnOff(MS_U8 u8DemodIndex, MS_BOOL b22kOn)
{
    return MDrv_DMD_DVBS_DiSEqC_Set22kOnOff(b22kOn);
}

MS_BOOL MDrv_MSDVBS_Demod_DiSEqC_Get22kOnOff(MS_U8 u8DemodIndex, MS_BOOL* b22kOn)
{
    return MDrv_DMD_DVBS_DiSEqC_Get22kOnOff(b22kOn);
}

MS_BOOL MDrv_MSDVBS_Demod_DiSEqC_SendCmd(MS_U8 u8DemodIndex, MS_U8* pCmd,MS_U8 u8CmdSize)
{
    return MDrv_DMD_DVBS_DiSEqC_SendCmd(pCmd,u8CmdSize);
}

MS_BOOL MDrv_MSDVBS_Demod_Get_Packet_Error(MS_U8 u8DemodIndex, MS_U16 *u16_data)
{
    MS_BOOL bRet = FALSE;

    if (u16_data != NULL)
    {
        bRet = MDrv_DMD_DVBS_GetPacketErr(u16_data);
    }
    return bRet;
}

MS_BOOL MDrv_MSDVBS_Demod_GetVCM_ISID_INFO(MS_U8 u8DemodIndex, MS_U8* pu8IS_ID, MS_U8* pu8Table)
{
    MS_BOOL bRet = FALSE;
#ifdef UFO_SUPPORT_VCM
    DMD_DVBS_LOCK_STATUS LockStatus;


    if(MDrv_DMD_DVBS_GetLock(DMD_DVBS_GETLOCK, &LockStatus) == TRUE)
    {
        if(LockStatus == DMD_DVBS_LOCK)
        {
            if(MDrv_DMD_DVBS2_VCM_Check())
            {
                if(MDrv_DMD_DVBS2_Get_IS_ID_INFO(pu8IS_ID, pu8Table))
                {
                    bRet = TRUE;
                }
            }
        }
    }
#endif
    return bRet;
}

MS_BOOL MDrv_MSDVBS_Demod_SetVCM_ISID(MS_U8 u8DemodIndex, MS_U8 u8IS_ID)
{
#ifdef UFO_SUPPORT_VCM
    MS_BOOL bret = TRUE;
    bret &= MDrv_DMD_DVBS2_Set_IS_ID(u8IS_ID);
    bret &= MDrv_DMD_DVBS2_TS_DivNum_Calculation();
    return bret;
#else
    return FALSE;
#endif
}


MS_BOOL MDrv_MSDVBS_Demod_Extension_Function(MS_U8 u8DemodIndex, DEMOD_EXT_FUNCTION_TYPE fuction_type, void *data)
{
    MS_BOOL bret = TRUE;

    switch(fuction_type)
    {
        case DEMOD_EXT_FUNC_FINALIZE:
            if (_s32MutexId >= 0)
            {
                bret &= MsOS_DeleteMutex(_s32MutexId);
                _s32MutexId = -1;
            }

            bInited = FALSE;
            bret &= MDrv_DMD_DVBS_Exit();
            break;
            
        case DEMOD_EXT_FUNC_SET_DISEQC_TX_22K_OFF:
            bDiSeqc_Tx22K_OFF = *(MS_BOOL*)(data);
            if(bInited == TRUE)
            {
                bret &= MDrv_DMD_DVBS_DiSEqC_SetTxToneMode(bDiSeqc_Tx22K_OFF);
            }
            break;

#if (defined(UFO_DEMOD_DVBS_SUPPORT_DMD_INT) && INTERN_DVBS_SUPPORT_DMD_INT)
        case DEMOD_EXT_FUNC_SET_INTERRUPT_CALLBACK:
            MS_DVBS_INIT_PARAM.fpCB= (fpDemodCB)(data);
            MDrv_DMD_DVBS_Reg_INT_CB(_mdrv_msdvbs_demod_cb);
            break; 
#endif

#ifdef UFO_DEMOD_GET_AGC_INFO
        case DEMOD_EXT_FUNC_GET_IFAGC_OUT:
            bret &= MDrv_DMD_DVBS_GetAGCInfo(DBG_MODE_AGC_OUT, (MS_U16*)(data));
            break;
#endif
        default:
            DMD_ERR(("Request extension function (%x) does not exist\n",fuction_type));
            return TRUE;
    }

    return bret;
}

MS_BOOL MDrv_MSDVBS_Demod_I2C_ByPass(MS_U8 u8DemodIndex,MS_BOOL bOn)
{
  if(GET_DEMOD_ENTRY_NODE(DEMOD_MSINTERN_DVBS).I2CByPassPreSetting != NULL)
  {
      return GET_DEMOD_ENTRY_NODE(DEMOD_MSINTERN_DVBS).I2CByPassPreSetting(u8DemodIndex,bOn);
  }
  else
      return MDrv_Demod_null_I2C_ByPass(u8DemodIndex,bOn);
}

#ifdef FE_AUTO_TEST
MS_BOOL  MDrv_MSDVBS_Demod_ReadReg(MS_U8 u8DemodIndex, MS_U16 RegAddr, MS_U8 *pu8Data)
{
    MS_BOOL bret = TRUE;
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
         DMD_ERR( ("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    
    bret = MDrv_DMD_DVBS_GetReg(RegAddr, pu8Data);
    HB_ReleaseMutex(_s32MutexId);
    return bret;
}

MS_BOOL MDrv_MSDVBS_Demod_WriteReg(MS_U8 u8DemodIndex, MS_U16 RegAddr, MS_U16 RegData)
{
    MS_BOOL bret = TRUE;
    MS_U8 u8RegData;
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
         DMD_ERR( ("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    u8RegData = (MS_U8)RegData;
    bret = MDrv_DMD_DVBS_SetReg(RegAddr, u8RegData);
    HB_ReleaseMutex(_s32MutexId);
    return bret;
}
#endif

DRV_DEMOD_TABLE_TYPE GET_DEMOD_ENTRY_NODE(DEMOD_MSINTERN_DVBS) DDI_DRV_TABLE_ENTRY(demodtab) =
{
     .name                         = "DEMOD_MSINTERN_DVBS",
     .data                         = DEMOD_MSINTERN_DVBS,
#if (defined(UFO_DEMOD_DVBS_SUPPORT_DMD_INT) && INTERN_DVBS_SUPPORT_DMD_INT)     
     .SupportINT                   = TRUE,
#else
     .SupportINT                   = FALSE,
#endif
     .init                         = MDrv_MSDVBS_Demod_Init,
     .GetLock                      = MDrv_MSDVBS_Demod_GetLock,
     .GetSNR                       = MDrv_MSDVBS_Demod_GetSNR,
     .GetBER                       = MDrv_MSDVBS_Demod_GetBER,
     .GetPWR                       = MDrv_MSDVBS_Demod_GetPWR,
     .GetSSI                       = MDrv_MSDVBS_Demod_GetSSI,
     .GetQuality                   = MDrv_MSDVBS_Demod_GetSignalQuality,
     .GetParam                     = MDrv_MSDVBS_Demod_GetParam,
     .Restart                      = MDrv_MSDVBS_Demod_Restart,
     .I2CByPass                    = MDrv_MSDVBS_Demod_I2C_ByPass,
     .I2CByPassPreSetting          = NULL,
     .Extension_Function           = MDrv_MSDVBS_Demod_Extension_Function,
     .Extension_FunctionPreSetting = NULL,
     .Get_Packet_Error              = MDrv_MSDVBS_Demod_Get_Packet_Error,     
     .CheckExist                   = MDrv_Demod_null_CheckExist,
#ifdef FE_AUTO_TEST
     .ReadReg                       = MDrv_MSDVBS_Demod_ReadReg,
     .WriteReg                      = MDrv_MSDVBS_Demod_WriteReg,
#endif     
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
     .BlindScanStart               = MDrv_MSDVBS_Demod_BlindScan_Start,
     .BlindScanNextFreq            = MDrv_MSDVBS_Demod_BlindScan_NextFreq,
     .BlindScanWaitCurFreqFinished = MDrv_MSDVBS_Demod_BlindScan_WaitCurFreqFinished,
     .BlindScanCancel              = MDrv_MSDVBS_Demod_BlindScan_Cancel,
     .BlindScanEnd                 = MDrv_MSDVBS_Demod_BlindScan_End,
     .BlindScanGetChannel          = MDrv_MSDVBS_Demod_BlindScan_GetChannel,
     .BlindScanGetCurrentFreq      = MDrv_MSDVBS_Demod_BlindScan_GetCurrentFreq,
     .DiSEqCSetTone                = MDrv_MSDVBS_Demod_DiSEqC_SetTone,
     .DiSEqCSetLNBOut              = MDrv_MSDVBS_Demod_DiSEqC_SetLNBOut,
     .DiSEqCGetLNBOut              = MDrv_MSDVBS_Demod_DiSEqC_GetLNBOut,
     .DiSEqCSet22kOnOff            = MDrv_MSDVBS_Demod_DiSEqC_Set22kOnOff,
     .DiSEqCGet22kOnOff            = MDrv_MSDVBS_Demod_DiSEqC_Get22kOnOff,
     .DiSEqC_SendCmd               = MDrv_MSDVBS_Demod_DiSEqC_SendCmd,
     .DiSEqC_GetReply              = MDrv_Demod_null_DiSEqC_GetReply,
     .GetISIDInfo                  = MDrv_MSDVBS_Demod_GetVCM_ISID_INFO,
     .SetISID                      = MDrv_MSDVBS_Demod_SetVCM_ISID
#endif
};

#endif // (FRONTEND_DEMOD_TYPE == DEMOD_MSDVBS_51)
#endif

