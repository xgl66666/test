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
#if defined(CHIP_KRITI)
#include "MsCommon.h"
#include "HbCommon.h"
#include "drvDMD_INTERN_DVBT.h"
#if (MS_DVBT2_INUSE == 1)
#include "drvDMD_INTERN_DVBT2.h"
#include "MsMemory.h"
#endif
#include "drvDemod.h"
#include "drvDemodNull.h"
#include "drvDMD_VD_MBX.h"
#include "drvSAR.h"
#include "drvTuner.h"



#define COFDMDMD_MUTEX_TIMEOUT       (2000)
static MS_BOOL KritiDMD_Init = FALSE;
static MS_S32 _s32MutexId = -1;
static EN_DEMOD_TYPE eCurDemodType = E_DEMOD_TYPE_T;

#define IF_FREQUENCY  5000//KHz for demod restart&init
#define FS_FREQUENCY 24000
#define usleep(x)                   MsOS_DelayTask(x/1000)

MS_S32 _u32LockTimeMax;
DEMOD_MS_INIT_PARAM Kriti_DVBT_INIT_PARAM;
static DMD_RFAGC_SSI TUNER_RfagcSsi[] =
{
    {0,    0xff},  
    {0,    0xff},  

};

static DMD_IFAGC_SSI TUNER_IfagcSsi_LoRef[] =
{
//current agc ref setting:150
    {10.0,     0x00},  //default setting
    {10.0,     0x00},  //default setting
    {-00.0,    0x31},
    {-05.0,    0x42},
    {-10.0,    0x57},
    {-15.0,    0x6e},
    {-20.0,    0x80},
    {-25.0,    0x87},
    {-30.0,    0x8d},
    {-35.0,    0x93},
    {-40.0,    0x98},
    {-45.0,    0x9d},
    {-50.0,    0xa3},
    {-55.0,    0xa8},
    {-60.0,    0xad},
    {-65.0,    0xb2},
    {-70.0,    0xb7},
    {-75.0,    0xbc},
    {-80.0,    0xc1},
    {-85.0,    0xc6},
    {-90.0,    0xff}, //end setting
};

static DMD_IFAGC_SSI TUNER_IfagcSsi_HiRef[] =
{
    { -48,   0x3A},
    { -49,   0x3B},
    { -50,   0x3C},
    { -51,   0x3E},
    { -52,   0x40},
    { -53,   0x43},
    { -54,   0x45},
    { -55,   0x47},
    { -56,   0x49},
    { -57,   0x4C},
    { -58,   0x4f},
    { -59,   0x51},
    { -60,   0x54},
    { -61,   0x56},
    { -62,   0x59},
    { -63,   0x5C},
    { -64,   0x5F},
    { -65,   0x62},
    { -66,   0x65},
    { -67,   0x69},
    { -68,   0x6E},
    { -69,   0x73},
    { -70,   0x78},
    { -71,   0x7F},
    { -72,   0x85},
    { -73,   0x8b},
    { -74,   0x9c},
    { -85,   0xc0},
    { -99,   0xFF},
};

static DMD_IFAGC_ERR TUNER_IfagcErr_LoRef[] =
{
    {0.00,        0x0A},
    {0.00,        0xFF},

};

static DMD_IFAGC_ERR TUNER_IfagcErr_HiRef[] =
{
    {0.00,        0x2D},
    {0.00,        0xFF},        // stopper
};

static DMD_SQI_CN_NORDIGP1 DvbtSqiCnNordigP1[] =
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

#if (MS_DVBT2_INUSE == 1)
static DMD_T2_SQI_CN_NORDIGP1 Dvbt2SqiCnNordigP1[] =
{
    {_T2_QPSK, _T2_CR1Y2, 3.5},
    {_T2_QPSK, _T2_CR3Y5, 4.7},
    {_T2_QPSK, _T2_CR2Y3, 5.6},
    {_T2_QPSK, _T2_CR3Y4, 6.6},
    {_T2_QPSK, _T2_CR4Y5, 7.2},
    {_T2_QPSK, _T2_CR5Y6, 7.7},

    {_T2_16QAM, _T2_CR1Y2, 8.7},
    {_T2_16QAM, _T2_CR3Y5, 10.1},
    {_T2_16QAM, _T2_CR2Y3, 11.4},
    {_T2_16QAM, _T2_CR3Y4, 12.5},
    {_T2_16QAM, _T2_CR4Y5, 13.3},
    {_T2_16QAM, _T2_CR5Y6, 13.8},

    {_T2_64QAM, _T2_CR1Y2, 13.0},
    {_T2_64QAM, _T2_CR3Y5, 14.8},
    {_T2_64QAM, _T2_CR2Y3, 16.2},
    {_T2_64QAM, _T2_CR3Y4, 17.7},
    {_T2_64QAM, _T2_CR4Y5, 18.7},
    {_T2_64QAM, _T2_CR5Y6, 19.4},

    {_T2_256QAM, _T2_CR1Y2, 17.0},
    {_T2_256QAM, _T2_CR3Y5, 19.4},
    {_T2_256QAM, _T2_CR2Y3, 20.8},
    {_T2_256QAM, _T2_CR3Y4, 22.9},
    {_T2_256QAM, _T2_CR4Y5, 24.3},
    {_T2_256QAM, _T2_CR5Y6, 25.1},
};
#endif
static MS_U8 u8DMD_DVBT_InitExt[]={ 4, // version
                                        0, // reserved
                                        0x15, // TS_CLK
                                        1 , // RFAGC tristate control default value, 1:trisate 0:non-tristate,never modify unless you know the meaning
                                        (MS_U8)(IF_FREQUENCY>>24), // IF Frequency
                                        (MS_U8)(IF_FREQUENCY>>16),
                                        (MS_U8)(IF_FREQUENCY>>8),
                                        (MS_U8)(IF_FREQUENCY>>0),
                                        (MS_U8)(24000>>24), // FS Frequency
                                        (MS_U8)(24000>>16),
                                        (MS_U8)(24000>>8),
                                        (MS_U8)(24000>>0),
                                        FRONTEND_DEMOD_IQ_SWAP, // IQ Swap
                                        FRONTEND_DEMOD_IQ_TYPE, // u8ADCIQMode : 0=I path, 1=Q path, 2=both IQ
                                        0, // u8PadSel : 0=Normal, 1=analog pad
                                        0, // bPGAEnable : 0=disable, 1=enable
                                        5, // u8PGAGain : default 5
                                        (MS_U8)(DVBT_TPS_timeout>>8), // TPS timeout 700ms~
                                        (MS_U8)(DVBT_TPS_timeout>>0),
                                        (MS_U8)(DVBT_FEC_timeout>>8), // FEC timeout 6000ms~
                                        (MS_U8)(DVBT_FEC_timeout>>0),
                                  };

static MS_U8 u8DMD_DVBT_DSPRegInitExt[]={ 1, // version, should be matched with library
                                                0, // reserved
                                                
                                                E_DMD_DVBT_CFG_FC_L,    // Addr_L
                                                E_DMD_DVBT_CFG_FC_L>>8, // Addr_H
                                                0xFF, // Mask
                                                0x88,
                                                E_DMD_DVBT_CFG_FC_H,    // Addr_L
                                                E_DMD_DVBT_CFG_FC_H>>8, // Addr_H
                                                0xFF, // Mask
                                                0x13,
                                                E_DMD_DVBT_CFG_IQ_SWAP,    // Addr_L
                                                E_DMD_DVBT_CFG_IQ_SWAP>>8, // Addr_H
                                                0xFF, // Mask
                                                0x01, // 0x00=I path, 0x01=Q path
}; // Value

#if (MS_DVBT2_INUSE == 1)
static MS_U8 u8DMD_DVBT2_InitExt[]={ 3, // version
                                        0, // reserved
                                        0x15, // TS_CLK
                                        1 , // RFAGC tristate control default value, 1:trisate 0:non-tristate,never modify unless you know the meaning
                                        (MS_U8)(IF_FREQUENCY>>24), // IF Frequency
                                        (MS_U8)(IF_FREQUENCY>>16),
                                        (MS_U8)(IF_FREQUENCY>>8),
                                        (MS_U8)(IF_FREQUENCY>>0),
                                        (MS_U8)(24000>>24), // FS Frequency
                                        (MS_U8)(24000>>16),
                                        (MS_U8)(24000>>8),
                                        (MS_U8)(24000>>0),
                                        FRONTEND_DEMOD_IQ_TYPE, // u8ADCIQMode : 0=I path, 1=Q path, 2=both IQ
                                        0, // u8PadSel : 0=Normal, 1=analog pad
                                        0, // bPGAEnable : 0=disable, 1=enable
                                        5, // u8PGAGain : default 5
                                        (MS_U8)(DVBT_TPS_timeout>>8), // P1 timeout 700ms~
                                        (MS_U8)(DVBT_TPS_timeout>>0),
                                        (MS_U8)(DVBT_FEC_timeout>>8), // FEC timeout 6000ms~
                                        (MS_U8)(DVBT_FEC_timeout>>0),
                                  };

static MS_U8 u8DMD_DVBT2_DSPRegInitExt[]={ 1, // version, should be matched with library
                                                0, // reserved

                                                E_DMD_T2_FC_L,    // Addr_L
                                                E_DMD_T2_FC_L>>8, // Addr_H
                                                0xFF, // Mask
                                                0x88,
                                                E_DMD_T2_FC_H,    // Addr_L
                                                E_DMD_T2_FC_H>>8, // Addr_H
                                                0xFF, // Mask
                                                0x13,
}; // Value
#endif

DRV_DEMOD_TABLE_TYPE GET_DEMOD_ENTRY_NODE(DEMOD_MSKRITI_DVBT2) DDI_DRV_TABLE_ENTRY(demodtab);

static MS_BOOL _InitInternalDVBT(void)
{
          MS_BOOL bret = TRUE;
          DMD_DVBT_InitData sDMD_DVBT_InitData;    
          // tuner parameter
          sDMD_DVBT_InitData.u8SarChannel=1; // 0xFF means un-connected
          sDMD_DVBT_InitData.pTuner_RfagcSsi=TUNER_RfagcSsi;
          sDMD_DVBT_InitData.u16Tuner_RfagcSsi_Size=sizeof(TUNER_RfagcSsi)/sizeof(DMD_RFAGC_SSI);
          sDMD_DVBT_InitData.pTuner_IfagcSsi_LoRef=TUNER_IfagcSsi_LoRef;
          sDMD_DVBT_InitData.u16Tuner_IfagcSsi_LoRef_Size=sizeof(TUNER_IfagcSsi_LoRef)/sizeof(DMD_IFAGC_SSI);
          sDMD_DVBT_InitData.pTuner_IfagcSsi_HiRef=TUNER_IfagcSsi_HiRef;
          sDMD_DVBT_InitData.u16Tuner_IfagcSsi_HiRef_Size=sizeof(TUNER_IfagcSsi_HiRef)/sizeof(DMD_IFAGC_SSI);
          sDMD_DVBT_InitData.pTuner_IfagcErr_LoRef=TUNER_IfagcErr_LoRef;
          sDMD_DVBT_InitData.u16Tuner_IfagcErr_LoRef_Size=sizeof(TUNER_IfagcErr_LoRef)/sizeof(DMD_IFAGC_SSI);
          sDMD_DVBT_InitData.pTuner_IfagcErr_HiRef=TUNER_IfagcErr_HiRef;
          sDMD_DVBT_InitData.u16Tuner_IfagcErr_HiRef_Size=sizeof(TUNER_IfagcErr_HiRef)/sizeof(DMD_IFAGC_SSI);
          sDMD_DVBT_InitData.pSqiCnNordigP1=DvbtSqiCnNordigP1;
          sDMD_DVBT_InitData.u16SqiCnNordigP1_Size=sizeof(DvbtSqiCnNordigP1)/sizeof(DMD_SQI_CN_NORDIGP1);
          
          // register init
          sDMD_DVBT_InitData.u8DMD_DVBT_DSPRegInitExt=u8DMD_DVBT_DSPRegInitExt; 
    // TODO use system variable type
          sDMD_DVBT_InitData.u8DMD_DVBT_DSPRegInitSize=(sizeof(u8DMD_DVBT_DSPRegInitExt)-2)/4;
          sDMD_DVBT_InitData.u8DMD_DVBT_InitExt=u8DMD_DVBT_InitExt; // TODO use system variable type
          bret &= MDrv_DMD_DVBT_Init(&sDMD_DVBT_InitData, sizeof(sDMD_DVBT_InitData));
          return bret;
}

#if MS_DVBT2_INUSE 
static MS_BOOL _InitInternalDVBT2(void)
    {
          MS_BOOL bret = TRUE;
          DMD_DVBT2_InitData sDMD_DVBT2_InitData;    
          // tuner parameter
          sDMD_DVBT2_InitData.u8SarChannel=1; // 0xFF means un-connected
          sDMD_DVBT2_InitData.pTuner_RfagcSsi=TUNER_RfagcSsi;
          sDMD_DVBT2_InitData.u16Tuner_RfagcSsi_Size=sizeof(TUNER_RfagcSsi)/sizeof(DMD_RFAGC_SSI);
          sDMD_DVBT2_InitData.pTuner_IfagcSsi_LoRef=TUNER_IfagcSsi_LoRef;
          sDMD_DVBT2_InitData.u16Tuner_IfagcSsi_LoRef_Size=sizeof(TUNER_IfagcSsi_LoRef)/sizeof(DMD_IFAGC_SSI);
          sDMD_DVBT2_InitData.pTuner_IfagcSsi_HiRef=TUNER_IfagcSsi_HiRef;
          sDMD_DVBT2_InitData.u16Tuner_IfagcSsi_HiRef_Size=sizeof(TUNER_IfagcSsi_HiRef)/sizeof(DMD_IFAGC_SSI);
          sDMD_DVBT2_InitData.pTuner_IfagcErr_LoRef=TUNER_IfagcErr_LoRef;
          sDMD_DVBT2_InitData.u16Tuner_IfagcErr_LoRef_Size=sizeof(TUNER_IfagcErr_LoRef)/sizeof(DMD_IFAGC_SSI);
          sDMD_DVBT2_InitData.pTuner_IfagcErr_HiRef=TUNER_IfagcErr_HiRef;
          sDMD_DVBT2_InitData.u16Tuner_IfagcErr_HiRef_Size=sizeof(TUNER_IfagcErr_HiRef)/sizeof(DMD_IFAGC_SSI);
          sDMD_DVBT2_InitData.pSqiCnNordigP1=Dvbt2SqiCnNordigP1;
          sDMD_DVBT2_InitData.u16SqiCnNordigP1_Size=sizeof(Dvbt2SqiCnNordigP1)/sizeof(DMD_SQI_CN_NORDIGP1);
          
          // register init
          sDMD_DVBT2_InitData.u8DMD_DVBT2_DSPRegInitExt=u8DMD_DVBT2_DSPRegInitExt; // TODO use system variable type
          sDMD_DVBT2_InitData.u8DMD_DVBT2_DSPRegInitSize=(sizeof(u8DMD_DVBT2_DSPRegInitExt)-2)/4;
          sDMD_DVBT2_InitData.u8DMD_DVBT2_InitExt=u8DMD_DVBT2_InitExt; // TODO use system variable type
          sDMD_DVBT2_InitData.u32EqStartAddr = (MS_U32)INTERNAL_DVBT2_EQ_ADR;
          sDMD_DVBT2_InitData.u32TdiStartAddr= (MS_U32)INTERNAL_DVBT2_TDI_ADR;
          sDMD_DVBT2_InitData.u32DjbStartAddr= (MS_U32)INTERNAL_DVBT2_DJB_ADR;
          sDMD_DVBT2_InitData.u32FwStartAddr= (MS_U32)INTERNAL_DVBT2_FW_ADR;
          bret &= MDrv_DMD_DVBT2_Init(&sDMD_DVBT2_InitData, sizeof(sDMD_DVBT2_InitData));
          return bret;

    }
#endif
MS_BOOL MDrv_Kriti_DVBT2_Demod_Init(MS_U8 u8DemodIndex,DEMOD_MS_INIT_PARAM* pParam)
{
    MS_BOOL ret = FALSE;

    if (_s32MutexId < 0)
    {
        _s32MutexId = MsOS_CreateMutex(E_MSOS_FIFO, "OfDmd_Mutex", MSOS_PROCESS_SHARED);

        if (_s32MutexId < 0)
        {
            GEN_EXCEP;
            return FALSE;
        }

    }

    if(NULL == pParam)
        return FALSE;

    Kriti_DVBT_INIT_PARAM.pstTunertab = pParam->pstTunertab;
    MDrv_SYS_DMD_VD_MBX_Init();


    MDrv_SAR_Kpd_Init();

    if( E_DEMOD_TYPE_T == eCurDemodType)
        ret = _InitInternalDVBT();
#if MS_DVBT2_INUSE         
    else if(E_DEMOD_TYPE_T2 == eCurDemodType)
        ret = _InitInternalDVBT2();
#endif   
  
    if(ret == TRUE)
    {
        KritiDMD_Init = TRUE;
    }
     DMD_DBG(("Kriti Internal DMD init OK\n"));
    return ret;

}

MS_BOOL MDrv_Kriti_DVBT2_Demod_Open(MS_U8 u8DemodIndex)
{
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
         DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    HB_ReleaseMutex(_s32MutexId);
    return TRUE;
}

MS_BOOL MDrv_Kriti_DVBT2_Demod_Close(MS_U8 u8DemodIndex)
{
    MS_BOOL ret = true;

    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
         DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    ret &= MDrv_DMD_DVBT_Exit();
#if MS_DVBT2_INUSE 
    ret &= MDrv_DMD_DVBT2_Exit();
#endif
    if(ret == TRUE)
    {
        KritiDMD_Init = FALSE;
    }

    HB_ReleaseMutex(_s32MutexId);

    return ret;
}

MS_BOOL MDrv_Kriti_DVBT2_Demod_PowerOnOff(MS_U8 u8DemodIndex, MS_BOOL bPowerOn)
{
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
         DMD_ERR( ("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    HB_ReleaseMutex(_s32MutexId);

    return TRUE;
}

MS_BOOL MDrv_Kriti_DVBT2_Demod_GetLock(MS_U8 u8DemodIndex, EN_LOCK_STATUS *peLockStatus)
{
        DMD_LOCK_STATUS LockStatus;
#if MS_DVBT2_INUSE     
        DMD_T2_LOCK_STATUS T2LockStatus;
#endif
    
        if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
        {
             DMD_ERR( ("%s: Obtain mutex failed.\n", __FUNCTION__));
            return FALSE;
        }
    
    
        if(KritiDMD_Init == FALSE)
        {
            HB_ReleaseMutex(_s32MutexId);
            return FALSE;
        }
        if(E_DEMOD_TYPE_T == eCurDemodType)
        {
            MDrv_DMD_DVBT_GetLock(E_DMD_DMD_DVBT_GETLOCK, &LockStatus);
            switch (LockStatus)
            {
                case E_DMD_LOCK:
                    *peLockStatus = E_DEMOD_LOCK;
                    //_u32LockTimeMax = DVBT_FEC_timeout;
                    break;
                case E_DMD_CHECKEND:
                    *peLockStatus = E_DEMOD_CHECKEND;
                    break;
                case E_DMD_UNLOCK:         
                    *peLockStatus = E_DEMOD_UNLOCK;
                    //_u32LockTimeMax = DVBT_TPS_timeout;
                    break;
                case E_DMD_CHECKING:
                default:
                    *peLockStatus = E_DEMOD_CHECKING;
                    break;
            }
        }
#if MS_DVBT2_INUSE 
        else
        {
            MDrv_DMD_DVBT2_GetLock(E_DMD_DVBT2_GETLOCK, &T2LockStatus);
            switch (T2LockStatus)
            {
                case E_DMD_T2_LOCK:
                    *peLockStatus = E_DEMOD_LOCK;
                    //_u32LockTimeMax = DVBT_FEC_timeout;
                    break;
                case E_DMD_T2_CHECKEND:
                    *peLockStatus = E_DEMOD_CHECKEND;
                    break;
                case E_DMD_T2_UNLOCK:         
                    *peLockStatus = E_DEMOD_UNLOCK;
                    //_u32LockTimeMax = DVBT_TPS_timeout;
                    break;
                case E_DMD_T2_CHECKING:
                default:
                    *peLockStatus = E_DEMOD_CHECKING;
                    break;
            }
        }
#endif    
        HB_ReleaseMutex(_s32MutexId);
    
        return TRUE;

}

MS_BOOL MDrv_Kriti_DVBT2_Demod_GetSNR(MS_U8 u8DemodIndex, float *pfSNR)
{
        MS_BOOL ret = TRUE;
    
        if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
        {
             DMD_ERR( ("%s: Obtain mutex failed.\n", __FUNCTION__));
            return FALSE;
        }
    
        if(KritiDMD_Init == FALSE)
        {
            *pfSNR = 0;
            HB_ReleaseMutex(_s32MutexId);
            return FALSE;
        }
        
        if(E_DEMOD_TYPE_T == eCurDemodType)
            ret = MDrv_DMD_DVBT_GetSNR(pfSNR);
#if MS_DVBT2_INUSE 
        else    
            ret = MDrv_DMD_DVBT2_GetSNR(pfSNR);
#endif            
        HB_ReleaseMutex(_s32MutexId);
        return ret;

}

MS_BOOL MDrv_Kriti_DVBT2_Demod_GetBER(MS_U8 u8DemodIndex, float *pfBER)
    {
        MS_BOOL ret= TRUE;
    
        if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
        {
             DMD_ERR( ("%s: Obtain mutex failed.\n", __FUNCTION__));
            return FALSE;
        }
    
        if(KritiDMD_Init == FALSE)
        {
            *pfBER = 0;
            HB_ReleaseMutex(_s32MutexId);
            return FALSE;
        }
        
        if(E_DEMOD_TYPE_T == eCurDemodType)
            ret = MDrv_DMD_DVBT_GetPostViterbiBer(pfBER);  
#if MS_DVBT2_INUSE   
        else
            ret = MDrv_DMD_DVBT2_GetPostLdpcBer(pfBER);
#endif    
        HB_ReleaseMutex(_s32MutexId);
        return ret;
    }


MS_BOOL MDrv_Kriti_DVBT2_Demod_GetPWR(MS_U8 u8DemodIndex, MS_S32 *ps32Signal)
{
        MS_BOOL ret= TRUE;
    
        if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
        {
             DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
            return FALSE;
        }
    
        if(KritiDMD_Init == FALSE)
        {
            *ps32Signal = 0;
            HB_ReleaseMutex(_s32MutexId);
            return FALSE;
        }
        if(E_DEMOD_TYPE_T == eCurDemodType)
            ret = MDrv_DMD_DVBT_GetSignalStrength((MS_U16 *)ps32Signal);
#if MS_DVBT2_INUSE    
        else
            ret = MDrv_DMD_DVBT2_GetSignalStrength((MS_U16 *)ps32Signal); 
#endif    
        HB_ReleaseMutex(_s32MutexId);
        return ret;
}


MS_BOOL MDrv_Kriti_DVBT2_Demod_GetSignalQuality(MS_U8 u8DemodIndex,MS_U16 *pu16quality)
{
    MS_BOOL ret = FALSE;
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
         DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    if(KritiDMD_Init == FALSE)
    {
        *pu16quality = 0;
        HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }

    if(E_DEMOD_TYPE_T == eCurDemodType)    
        ret = MDrv_DMD_DVBT_GetSignalQuality(pu16quality);
#if MS_DVBT2_INUSE         
    else
        ret = MDrv_DMD_DVBT2_GetSignalQuality(pu16quality);
#endif        
    HB_ReleaseMutex(_s32MutexId);
    return ret;
}


MS_BOOL MDrv_Kriti_DVBT2_Demod_Restart(MS_U8 u8DemodIndex, DEMOD_MS_FE_CARRIER_PARAM* pParam,MS_U32 u32BroadCastType)
    {
        MS_BOOL bPalBG = FALSE; //unknown variable
        DMD_RF_CHANNEL_BANDWIDTH BW = E_DMD_RF_CH_BAND_8MHz;
#if MS_DVBT2_INUSE 
        DMD_DVBT2_RF_CHANNEL_BANDWIDTH BWT2 = E_DMD_T2_RF_BAND_8MHz;
        //DMD_T2_LOCK_STATUS LockStatus = E_DMD_T2_NULL;
#endif
        MS_BOOL bret = TRUE;
        //static DEMOD_EN_TER_BW_MODE cur_BW = 0xff;
    
    
        if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
        {
             DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
            return FALSE;
        }
    
        if(KritiDMD_Init == FALSE)
        {
            HB_ReleaseMutex(_s32MutexId);
            return FALSE;
        }

#if 0        
#ifdef DVBT2_STYLE    
        if (DEMOD_BW_MODE_1_7MHZ == pParam->TerParam.eBandWidth)
        {
            _SetCurrentDemodType(E_DEMOD_TYPE_T2);
        }
        else if ((E_DEMOD_TYPE_T == pParam->TerParam.u8ScanType) && (eCurDemodType != pParam->TerParam.u8ScanType))
        {
            _SetCurrentDemodType(E_DEMOD_TYPE_T);
        }
        else if ((E_DEMOD_TYPE_T2 == pParam->TerParam.u8ScanType) && (eCurDemodType != pParam->TerParam.u8ScanType))
        {
            _SetCurrentDemodType(E_DEMOD_TYPE_T2);
        }
#endif
         //set freq and bw to tuner
    
#ifdef DVBT2_STYLE
        if(MDrv_DMD_DVBT2_GetLock(E_DMD_DVBT2_GETLOCK, &LockStatus) && (eCurDemodType == E_DEMOD_TYPE_T2))
        {
            if((E_DMD_T2_LOCK != LockStatus) || (eCurDemodType != pParam->TerParam.u8ScanType) ||\
                (tuner_config.u32CurrFreq != pParam->u32Frequency) || (tuner_config.enCurrBW != pParam->TerParam.eBandWidth))
            {
                 MDrv_Tuner_SetTuner( pParam->u32Frequency, pParam->TerParam.eBandWidth);
            }
        }
        else       
        {
            MDrv_Tuner_SetTuner( pParam->u32Frequency, pParam->TerParam.eBandWidth);
        }
#else
        MDrv_Tuner_SetTuner( pParam->u32Frequency, pParam->TerParam.eBandWidth);
#endif
    
         tuner_config.u32CurrFreq = pParam->u32Frequency;
         tuner_config.enCurrBW = pParam->TerParam.eBandWidth;
#endif    
        // ("------pParam->u32Frequency [%d] pParam->TerParam.eBandWidth[%d] ------\n",pParam->u32Frequency,pParam->TerParam.eBandWidth);
      
        //if(cur_BW != pParam->TerParam.eBandWidth)
        {
            switch (pParam->TerParam.eBandWidth)
            {
                case DEMOD_BW_MODE_6MHZ:
                    BW = E_DMD_RF_CH_BAND_6MHz;
#if MS_DVBT2_INUSE
                    BWT2 = E_DMD_T2_RF_BAND_6MHz;
#endif
                    break;
                case DEMOD_BW_MODE_7MHZ:
                    BW = E_DMD_RF_CH_BAND_7MHz;
#if MS_DVBT2_INUSE
                    BWT2 = E_DMD_T2_RF_BAND_7MHz;
#endif
                    break;
#if MS_DVBT2_INUSE 
                case DEMOD_BW_MODE_1_7MHZ:
                    BWT2 = E_DMD_T2_RF_BAND_1p7MHz;
                    break;
#endif                
                case DEMOD_BW_MODE_8MHZ:
                default:
                    BW = E_DMD_RF_CH_BAND_8MHz;
#if MS_DVBT2_INUSE
                    BWT2 = E_DMD_T2_RF_BAND_8MHz;
#endif
                    break;
            }
    
        if(E_DEMOD_TYPE_T == eCurDemodType)
        {
            bret &= MDrv_DMD_DVBT_SetConfig(BW, FALSE, bPalBG );
            bret &= MDrv_DMD_DVBT_SetActive(TRUE);
        }
#if MS_DVBT2_INUSE    
        else
        {
            bret &= MDrv_DMD_DVBT2_SetConfig(BWT2, FALSE, pParam->TerParam.u8PlpID);
            bret &= MDrv_DMD_DVBT2_SetActive(TRUE);
        }
            //cur_BW = pParam->TerParam.eBandWidth;
    
#endif
        }
    
        HB_ReleaseMutex(_s32MutexId);
        return TRUE;
    }


#if MS_DVBT2_INUSE
MS_BOOL MDrv_Kriti_DVBT2_Demod_SetCurrentDemodType(MS_U8 u8DemodIndex, MS_U8  type)
{
    MS_BOOL bret = TRUE;
    if(eCurDemodType == type)
        return bret;
    else
    {
     if(E_DEMOD_TYPE_T == eCurDemodType)
     {
         bret &= MDrv_DMD_DVBT_Exit();
         bret = _InitInternalDVBT2();
         eCurDemodType = type;
     }
     else
     {
         bret &= MDrv_DMD_DVBT2_Exit();
         bret = _InitInternalDVBT();
         eCurDemodType = type;
     }
      

    }
    if(bret == FALSE)
         DMD_ERR(("Set Demod Type %x ERROR\n",eCurDemodType));
    return bret;
}

MS_U8 MDrv_Kriti_DVBT2_Demod_GetCurrentDemodType(MS_U8 u8DemodIndex)
{
  return eCurDemodType;
}

MS_BOOL MDrv_Kriti_DVBT2_Demod_GetPlpBitMap(MS_U8 u8DemodIndex,MS_U8* u8PlpBitMap)
{
    return MDrv_DMD_DVBT2_GetPlpBitMap(u8PlpBitMap);
}

MS_BOOL MDrv_Kriti_DVBT2_Demod_GetPlpGroupID(MS_U8 u8DemodIndex,MS_U8 u8PlpID, MS_U8* u8GroupID)
{
    MS_BOOL bRet = TRUE;
    MS_U16 u16Retry = 0;

    if ((u8PlpID != 0xFF) && (E_DEMOD_TYPE_T2 == eCurDemodType))
    {
        do
        {
            u16Retry++;
            usleep(100 * 1000);
            bRet = MDrv_DMD_DVBT2_GetPlpGroupID(u8PlpID, u8GroupID);
        }while ((bRet == FALSE) && (u16Retry < 60));
        
        if (bRet == FALSE)
        {
            return FALSE;
        }
        return TRUE;
    }
    return FALSE;
}

MS_BOOL MDrv_Kriti_DVBT2_Demod_SetPlpGroupID(MS_U8 u8DemodIndex,MS_U8 u8PlpID, MS_U8 u8GroupID)
{
    if(E_DEMOD_TYPE_T2 == eCurDemodType)
        return MDrv_DMD_DVBT2_SetPlpID(u8PlpID, u8GroupID);
    else
        return FALSE;
}

#endif
MS_BOOL DEMOD_MSKRITI_DVBT2_Extension_Function(MS_U8 u8DemodIndex, DEMOD_EXT_FUNCTION_TYPE fuction_type, void *data)
{
    switch(fuction_type)
    { 
        default:
             DMD_DBG(("Request extension function (%x) does not exist\n",fuction_type));
            return TRUE;
    } 
}

MS_BOOL MDrv_Kriti_DVBT2_Demod_I2C_ByPass(MS_U8 u8DemodIndex,MS_BOOL bOn)
{
  if(GET_DEMOD_ENTRY_NODE(DEMOD_MSKRITI_DVBT2).I2CByPassPreSetting != NULL)
  {
      return GET_DEMOD_ENTRY_NODE(DEMOD_MSKRITI_DVBT2).I2CByPassPreSetting(u8DemodIndex,bOn);
  }
  else
      return MDrv_Demod_null_I2C_ByPass(u8DemodIndex,bOn);
}


DRV_DEMOD_TABLE_TYPE GET_DEMOD_ENTRY_NODE(DEMOD_MSKRITI_DVBT2) DDI_DRV_TABLE_ENTRY(demodtab) = 
{  
     .name                         = "DEMOD_MSKRITI_DVBT2",
     .data                         = DEMOD_MSKRITI_DVBT2,        
     .init                         = MDrv_Kriti_DVBT2_Demod_Init,
     .GetLock                      = MDrv_Kriti_DVBT2_Demod_GetLock,
     .GetSNR                       = MDrv_Kriti_DVBT2_Demod_GetSNR,
     .GetBER                       = MDrv_Kriti_DVBT2_Demod_GetBER,
     .GetPWR                       = MDrv_Kriti_DVBT2_Demod_GetPWR,
     .GetQuality                   = MDrv_Kriti_DVBT2_Demod_GetSignalQuality,
     .GetParam                     = MDrv_Demod_null_GetParam,
     .Restart                      = MDrv_Kriti_DVBT2_Demod_Restart,
     .I2CByPass                    = MDrv_Kriti_DVBT2_Demod_I2C_ByPass,
     .I2CByPassPreSetting          = NULL,
     .Extension_Function           = DEMOD_MSKRITI_DVBT2_Extension_Function,
     .Extension_FunctionPreSetting = NULL,
#if MS_DVBT2_INUSE
     .SetCurrentDemodType          = MDrv_Kriti_DVBT2_Demod_SetCurrentDemodType,
     .GetCurrentDemodType          = MDrv_Kriti_DVBT2_Demod_GetCurrentDemodType,
     .GetPlpBitMap                 = MDrv_Kriti_DVBT2_Demod_GetPlpBitMap,
     .GetPlpGroupID                = MDrv_Kriti_DVBT2_Demod_GetPlpGroupID,
     .SetPlpGroupID                = MDrv_Kriti_DVBT2_Demod_SetPlpGroupID,
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

#endif // (FRONTEND_DEMOD_TYPE == DEMOD_MSKAPPA_DVBT)

