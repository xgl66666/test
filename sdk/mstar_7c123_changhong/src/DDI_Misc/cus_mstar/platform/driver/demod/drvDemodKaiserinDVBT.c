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
#if defined(CHIP_KAISERIN)
#include "MsCommon.h"
#include "HbCommon.h"
#include "drvDMD_INTERN_DVBT.h"
#include "drvDemod.h"
#include "drvDemodNull.h"
#include "drvDMD_VD_MBX.h"
#include "drvSAR.h"
#include "drvTuner.h"




#define COFDMDMD_MUTEX_TIMEOUT       (2000)
DRV_DEMOD_TABLE_TYPE GET_DEMOD_ENTRY_NODE(DEMOD_MSKAISERIN_DVBT) DDI_DRV_TABLE_ENTRY(demodtab);
static MS_BOOL bInited = FALSE;
static MS_S32 _s32MutexId = -1;

#if (FRONTEND_TUNER_TYPE == TUNER_PHILIPS_TDA18250HN || FRONTEND_TUNER_TYPE == TUNER_MXL603 || FRONTEND_TUNER_TYPE == TUNER_MXL608)
#define IF_FREQUENCY  5000//KHz for demod restart&init
#else
#define IF_FREQUENCY  4570//KHz for demod restart&init
#endif
MS_S32 _u32LockTimeMax;

static DMD_RFAGC_SSI ALPS_TUNER_RfagcSsi[] =
{
        //for mxl5007 tuner setting
        {0,    0xff},  //default setting bryan add for MXL5007
    {0,    0xff},  //default setting
            /*  bryan mark 110517

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
    */
};

static DMD_IFAGC_SSI ALPS_TUNER_IfagcSsi_LoRef[] =
{
		//for mxl5007 tuner setting
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
            /*  bryan mark 110517

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
    */
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
MS_BOOL MDrv_Demod_DVBT_Init(MS_U8 u8DemodIndex,DEMOD_MS_INIT_PARAM* pParam)
{
    DMD_DVBT_InitData sDMD_DVBT_InitData;
    MS_BOOL ret;

    if (_s32MutexId < 0)
    {
        _s32MutexId = MsOS_CreateMutex(E_MSOS_FIFO, "OfDmd_Mutex", MSOS_PROCESS_SHARED);

        if (_s32MutexId < 0)
        {
            DMD_ERR(("%s: Create mutex failed.\n", __FUNCTION__));
            return FALSE;
        }

    }


    MDrv_SYS_DMD_VD_MBX_Init();


	static MS_U8 u8DMD_DVBT_DSPRegInitExt[]={	1, // version, should be matched with library
												0, // reserved

												E_DMD_DVBT_CFG_FC_L,    // Addr_L
												E_DMD_DVBT_CFG_FC_L>>8, // Addr_H
												0xFF, // Mask
												0xC7,
                                                E_DMD_DVBT_CFG_FC_H,    // Addr_L
                                                E_DMD_DVBT_CFG_FC_H>>8, // Addr_H
                                                0xFF, // Mask
                                                0x9F,
                                                E_DMD_DVBT_CFG_IQ_SWAP,    // Addr_L
                                                E_DMD_DVBT_CFG_IQ_SWAP>>8, // Addr_H

                                                0xFF, // Mask
												0x01, // 0x00=I path, 0x01=Q path
											}; // Value
#if 0
	static MS_U8 u8DMD_DVBT_InitExt[]={	2, // version
										0, // reserved
										0x15, // TS_CLK
										1}; // RFAGC tristate control default value, 1:trisate 0:non-tristate,never modify unless you know the meaning
#else
    static MS_U8 u8DMD_DVBT_InitExt[]={ 4, // version
                                        0, // reserved
                                        0x15, // TS_CLK
                                        1 , // RFAGC tristate control default value, 1:trisate 0:non-tristate,never modify unless you know the meaning
                                        (MS_U8)(IF_FREQUENCY>>24), // IF Frequency
                                        (MS_U8)(IF_FREQUENCY>>16),
                                        (MS_U8)(IF_FREQUENCY>>8),
                                        (MS_U8)(IF_FREQUENCY>>0),
                                        (MS_U8)(45474>>24), // FS Frequency
                                        (MS_U8)(45474>>16),
                                        (MS_U8)(45474>>8),
                                        (MS_U8)(45474>>0),
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
#endif

    // tuner parameter
    sDMD_DVBT_InitData.u8SarChannel=0xff; // 0xFF means un-connected
    sDMD_DVBT_InitData.pTuner_RfagcSsi=ALPS_TUNER_RfagcSsi;
    sDMD_DVBT_InitData.u16Tuner_RfagcSsi_Size=sizeof(ALPS_TUNER_RfagcSsi)/sizeof(DMD_RFAGC_SSI);
    sDMD_DVBT_InitData.pTuner_IfagcSsi_LoRef=ALPS_TUNER_IfagcSsi_LoRef;
    sDMD_DVBT_InitData.u16Tuner_IfagcSsi_LoRef_Size=sizeof(ALPS_TUNER_IfagcSsi_LoRef)/sizeof(DMD_IFAGC_SSI);
    sDMD_DVBT_InitData.pTuner_IfagcSsi_HiRef=ALPS_TUNER_IfagcSsi_HiRef;
    sDMD_DVBT_InitData.u16Tuner_IfagcSsi_HiRef_Size=sizeof(ALPS_TUNER_IfagcSsi_HiRef)/sizeof(DMD_IFAGC_SSI);
    sDMD_DVBT_InitData.pTuner_IfagcErr_LoRef=ALPS_TUNER_IfagcErr_LoRef;
    sDMD_DVBT_InitData.u16Tuner_IfagcErr_LoRef_Size=sizeof(ALPS_TUNER_IfagcErr_LoRef)/sizeof(DMD_IFAGC_SSI);
    sDMD_DVBT_InitData.pTuner_IfagcErr_HiRef=ALPS_TUNER_IfagcErr_HiRef;
    sDMD_DVBT_InitData.u16Tuner_IfagcErr_HiRef_Size=sizeof(ALPS_TUNER_IfagcErr_HiRef)/sizeof(DMD_IFAGC_SSI);
    sDMD_DVBT_InitData.pSqiCnNordigP1=SqiCnNordigP1;
    sDMD_DVBT_InitData.u16SqiCnNordigP1_Size=sizeof(SqiCnNordigP1)/sizeof(DMD_SQI_CN_NORDIGP1);

	// register init
	sDMD_DVBT_InitData.u8DMD_DVBT_DSPRegInitExt=u8DMD_DVBT_DSPRegInitExt; // TODO use system variable type
	sDMD_DVBT_InitData.u8DMD_DVBT_DSPRegInitSize=(sizeof(u8DMD_DVBT_DSPRegInitExt)-2)/4;
	sDMD_DVBT_InitData.u8DMD_DVBT_InitExt=u8DMD_DVBT_InitExt; // TODO use system variable type
    ret = MDrv_DMD_DVBT_Init(&sDMD_DVBT_InitData, sizeof(sDMD_DVBT_InitData)); // _UTOPIA
    if(ret == TRUE)
    {
        bInited = TRUE;
    }

    return ret;

}

MS_BOOL MDrv_Demod_DVBT_Open(MS_U8 u8DemodIndex)
{
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    HB_ReleaseMutex(_s32MutexId);
    return TRUE;
}

MS_BOOL MDrv_Demod_DVBT_Close(MS_U8 u8DemodIndex)
{
    MS_BOOL ret;

    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    ret = MDrv_DMD_DVBT_Exit();
    if(ret == TRUE)
    {
        bInited = FALSE;
    }

    HB_ReleaseMutex(_s32MutexId);

    return ret;
}

MS_BOOL MDrv_Demod_DVBT_PowerOnOff(MS_U8 u8DemodIndex, MS_BOOL bPowerOn)
{
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    HB_ReleaseMutex(_s32MutexId);

    return TRUE;
}

MS_BOOL MDrv_Demod_DVBT_GetLock(MS_U8 u8DemodIndex, EN_LOCK_STATUS *peLockStatus)
{
    DMD_LOCK_STATUS LockStatus;

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

    MDrv_DMD_DVBT_GetLock(E_DMD_DMD_DVBT_GETLOCK, &LockStatus);
    switch (LockStatus)
    {
        case E_DMD_LOCK:
            *peLockStatus = E_DEMOD_LOCK;
            break;
        case E_DMD_CHECKEND:
            *peLockStatus = E_DEMOD_CHECKEND;
            break;
        case E_DMD_UNLOCK:
            *peLockStatus = E_DEMOD_UNLOCK;
            break;
        case E_DMD_CHECKING:
        default:
            *peLockStatus = E_DEMOD_CHECKING;
            break;
    }


    HB_ReleaseMutex(_s32MutexId);

    return TRUE;
}

MS_BOOL MDrv_Demod_DVBT_GetSNR(MS_U8 u8DemodIndex, float *pfSNR)
{
    MS_BOOL ret;

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

    ret = MDrv_DMD_DVBT_GetSNR(pfSNR);

    HB_ReleaseMutex(_s32MutexId);
    return ret;
}

MS_BOOL MDrv_Demod_DVBT_GetBER(MS_U8 u8DemodIndex, float *pfBER)
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

    ret = MDrv_DMD_DVBT_GetPostViterbiBer(pfBER);
    HB_ReleaseMutex(_s32MutexId);
    return ret;
}

MS_BOOL MDrv_Demod_DVBT_GetPWR(MS_U8 u8DemodIndex, MS_S32 *ps32Signal)
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

    ret = MDrv_DMD_DVBT_GetSignalStrength((MS_U16 *)ps32Signal);
    HB_ReleaseMutex(_s32MutexId);
    return ret;
}

MS_BOOL MDrv_Demod_DVBT_GetSignalQuality(MS_U8 u8DemodIndex,MS_U16 *pu16quality)
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
    ret = MDrv_DMD_DVBT_GetSignalQuality(pu16quality);
    HB_ReleaseMutex(_s32MutexId);
    return ret;
}


MS_BOOL MDrv_Demod_DVBT_Config(MS_U8 u8DemodIndex, MS_U8 *pRegParam)
{
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    HB_ReleaseMutex(_s32MutexId);

    return TRUE;
}

MS_BOOL MDrv_Demod_DVBT_GetParam(MS_U8 u8DemodIndex, DEMOD_MS_FE_CARRIER_PARAM* pParam)
{
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    HB_ReleaseMutex(_s32MutexId);
    return TRUE;
}

MS_BOOL MDrv_Demod_DVBT_Restart(MS_U8 u8DemodIndex, DEMOD_MS_FE_CARRIER_PARAM* pParam,MS_U32 u32BroadCastType)
{
    MS_BOOL bPalBG = FALSE; //unknown variable
    MS_BOOL bLPSel = FALSE;
    DMD_RF_CHANNEL_BANDWIDTH BW;

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

    switch (pParam->TerParam.eLevelSel)
    {
       case DEMOD_TER_LP_SEL:
           bLPSel = TRUE;
           break;
       case DEMOD_TER_HP_SEL:
       default:
           bLPSel = FALSE;
           break;
    }

    switch (pParam->TerParam.eBandWidth)
    {
        case DEMOD_BW_MODE_6MHZ:
            BW = E_DMD_RF_CH_BAND_6MHz;
            break;
        case DEMOD_BW_MODE_7MHZ:
            BW = E_DMD_RF_CH_BAND_7MHz;
            break;
        case DEMOD_BW_MODE_8MHZ:
        default:
            BW = E_DMD_RF_CH_BAND_8MHz;
            break;
    }

    MDrv_DMD_DVBT_SetConfigHPLP(BW, FALSE, bPalBG, bLPSel);

    if(FALSE == MDrv_DMD_DVBT_SetActive(TRUE)) // _UTOPIA
    {
        HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }

    HB_ReleaseMutex(_s32MutexId);
    return TRUE;
}

MS_BOOL MDrv_Demod_DVBT_SetMode(MS_U8 u8DemodIndex, Demod_Mode* pMode)
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

MS_BOOL MDrv_Demod_DVBT_SetOutoutPath(MS_U8 u8DemodIndex, DEMOD_INTERFACE_MODE path)
{
    //Dummy function
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    HB_ReleaseMutex(_s32MutexId);

    return TRUE;
}

DEMOD_INTERFACE_MODE MDrv_Demod_DVBT_GetOutoutPath(MS_U8 u8DemodIndex)
{
    //Dummy function

    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    HB_ReleaseMutex(_s32MutexId);
    return DEMOD_INTERFACE_PARALLEL;
}

#ifdef FE_AUTO_TEST
MS_U16  MDrv_Demod_DVBT_ReadReg(MS_U16 RegAddr)
{
    MS_U8 RegData = 0;

    MDrv_DMD_DVBT_GetReg(RegAddr, &RegData);

    return (MS_U16) RegData;
}

MS_BOOL MDrv_Demod_DVBT_WriteReg(MS_U16 RegAddr, MS_U16 RegData)
{
    return MDrv_DMD_DVBT_SetReg(RegAddr, RegData);
}

MS_BOOL MDrv_Demod_DVBT_Get_Packet_Error(MS_U16 *u16_data)
{
    return MDrv_DMD_DVBT_GetPacketErr(u16_data);
}
#endif

MS_BOOL DEMOD_MSKAISERIN_DVBT_Extension_Function(MS_U8 u8DemodIndex, DEMOD_EXT_FUNCTION_TYPE fuction_type, void *data)
{
    switch(fuction_type)
    {
        default:
             DMD_DBG(("Request extension function (%x) does not exist\n",fuction_type));
            return TRUE;
    }
}

MS_BOOL MDrv_Demod_DVBT_I2C_ByPass(MS_U8 u8DemodIndex,MS_BOOL bOn)
{
  if(GET_DEMOD_ENTRY_NODE(DEMOD_MSKAISERIN_DVBT).I2CByPassPreSetting != NULL)
  {
      return GET_DEMOD_ENTRY_NODE(DEMOD_MSKAISERIN_DVBT).I2CByPassPreSetting(u8DemodIndex,bOn);
  }
  else
      return MDrv_Demod_null_I2C_ByPass(u8DemodIndex,bOn);
}

DRV_DEMOD_TABLE_TYPE GET_DEMOD_ENTRY_NODE(DEMOD_MSKAISERIN_DVBT) DDI_DRV_TABLE_ENTRY(demodtab) =
{
     .name                         = "DEMOD_MSKAISERIN_DVBT",
     .data                         = DEMOD_MSKAISERIN_DVBT,
     .init                         = MDrv_Demod_DVBT_Init,
     .GetLock                      = MDrv_Demod_DVBT_GetLock,
     .GetSNR                       = MDrv_Demod_DVBT_GetSNR,
     .GetBER                       = MDrv_Demod_DVBT_GetBER,
     .GetPWR                       = MDrv_Demod_DVBT_GetPWR,
     .GetQuality                   = MDrv_Demod_DVBT_GetSignalQuality,
     .GetParam                     = MDrv_Demod_DVBT_GetParam,
     .Restart                      = MDrv_Demod_DVBT_Restart,
     .I2CByPass                    = MDrv_Demod_DVBT_I2C_ByPass,
     .I2CByPassPreSetting          = NULL,
     .Extension_Function           = DEMOD_MSKAISERIN_DVBT_Extension_Function,
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

#endif // (FRONTEND_DEMOD_TYPE == DEMOD_MSKAISERIN_DVBT)

