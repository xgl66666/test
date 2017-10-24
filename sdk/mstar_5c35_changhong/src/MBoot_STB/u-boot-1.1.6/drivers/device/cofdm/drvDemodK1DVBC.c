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
#if (ENABLE_HUMMINGBIRD_CUSTOMER)
//#include "Board.h"
#include "frontend_config.h"
#if (FRONTEND_DEMOD_TYPE == EMBEDDED_DVBC51_DEMOD)
//#include "MsCommon.h"
//#include "HbCommon.h"
#include "sys/drv/drvDMD_INTERN_DVBC.h"
#include "device/drvDemod.h"
#include "sys/drv/drvDMD_VD_MBX.h"

#include "sys/drv/drvSAR.h"

#if (FRONTEND_TUNER_TYPE == TUNER_MXL603)
#include "../tuner/MxL603/MaxLinearDataTypes.h"
#include "device/MxL603.h"

#endif


#define AutoSymbol_Timeout   10000
#define FixSymbol_AutoQam_Timeout   2000
#define FixSymbol_FixQam_Timeout   2000

#define DEMOD_IQ_I_IN               0
#define DEMOD_IQ_Q_IN               1
#define DEMOD_IQ_IQ_IN              2
//added for customer

#ifdef FRONTEND_DEMOD_IQ_TYPE
#undef FRONTEND_DEMOD_IQ_TYPE
#endif 

#define FRONTEND_DEMOD_IQ_TYPE      DEMOD_IQ_I_IN // C:Q T:I

#define COFDMDMD_MUTEX_TIMEOUT       (2000)

//////////////////////////////////////////////////////////
//
// if the packet of mainchip is QFP, then set QFP_PK_ENABLE as "1"
// else  set QFP_PK_ENABLE as "0"
//
//////////////////////////////////////////////////////////
#define QFP_PK_ENABLE 0

//////////////////////////////////////////////////////////
//
// USE_6M_ACIFILTER for dvbc 6M bandwidth, if bw is 6M,set it as "1",
//else set it as "0"
//
//////////////////////////////////////////////////////////
#define USE_6M_ACIFILTER  0
#define _BIT4           (0x00000010UL)


#define DEMOD_ADJUST_SSI   0           //to get if agc for ssi
#define MS_DEBUG_ENABLE     0          //for debug
#if MS_DEBUG_ENABLE
#define DBG_MSB(x)  x
#else
#define DBG_MSB(x)
#endif


static MS_BOOL bInited = FALSE;


#if ((FRONTEND_TUNER_TYPE == TUNER_PHILIPS_TDA18250HN) || (FRONTEND_TUNER_TYPE == TUNER_MXL603))
const static MS_U32 u32IFrequency = 5000;      //KHz for demod restart&init
#elif(FRONTEND_TUNER_TYPE==TUNER_R820C)
const static MS_U32 u32IFrequency = 5070;
#else
const static MS_U32 u32IFrequency = 36130;    //KHz for demod restart&init
#endif
//defined but not used makes warning
/*static double _demod_log10(double flt_x)
{
    MS_U32      u32_temp = 1;
    MS_U8       indx = 0;

    do
    {
        u32_temp = u32_temp << 1;
        if (flt_x < (double)u32_temp)
            break;
    }
    while (++indx < 32);

    // 10*log10(X) ~= 0.3*N, when X ~= 2^N
    return (double)0.3 * indx;
}
*/

static DMD_RFAGC_SSI ALPS_TUNER_RfagcSsi[] =
{
    { -100.00,    0x0}, //default setting
    { -100.00,    0xff}, //default setting
    /*
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
        {-55.00,    0xFF},*/
};


/*
static DMD_IFAGC_SSI ALPS_TUNER_IfagcSsi_LoRef[] =
{
    { -50.00,     0x5d},
    { -51.00,    0x5e},
    { -52.00,    0x5F},
    { -53.00,    0x5F},
    { -54.00,    0x60},
    { -55.00,    0x63},
    { -56.00,    0x68},
    { -57.50,    0x6C},
    { -58.00,    0x70},
    { -59.00,    0x75},
    { -60.00,    0x79},
    { -61.40,    0x7D},
    { -62.00,    0x80},
    { -63.00,    0x81},
    { -64.00,    0x83},
    { -65.00,    0x84},
    { -66.00,    0x85},
    { -67.00,    0x87},
    { -68.00,    0x88},
    { -69.00,    0x89},
    { -70.00,    0x8A},
    { -71.00,    0x8C},
    { -72.00,    0x8D},
    { -73.00,    0x8F},
    { -74.00,    0x90},
    { -75.00,    0x92},
    { -76.00,    0x93},
    { -77.00,    0x95},
    { -78.00,    0x96},
    { -79.00,    0x98},
    { -80.00,    0x99},
    { -81.00,    0x9B},
    { -82.00,    0x9D},
    { -83.00,    0x9E},
    { -84.00,    0xA0},
    { -85.00,    0xA2},
    { -86.00,    0xA4},
    { -87.00,    0xA6},
    { -88.00,    0xA8},
    { -89.00,    0xAA},
    { -90.00,    0xAC},
    { -91.00,    0xAD},
    { -92.00,    0xAF},
    { -93.00,    0xB0},
    { -93.00,    0xFF},
};

static DMD_IFAGC_SSI ALPS_TUNER_IfagcSsi_HiRef[] =
{
    { -54.30,    0x5F},
    { -54.40,    0x60},
    { -55.00,    0x63},
    { -56.00,    0x68},
    { -57.50,    0x6C},
    { -58.00,    0x70},
    { -59.00,    0x75},
    { -60.00,    0x79},
    { -61.40,    0x7D},
    { -62.00,    0x80},
    { -63.00,    0x81},
    { -64.00,    0x83},
    { -65.00,    0x84},
    { -66.00,    0x85},
    { -67.00,    0x87},
    { -68.00,    0x88},
    { -69.00,    0x89},
    { -70.00,    0x8A},
    { -71.00,    0x8C},
    { -72.00,    0x8D},
    { -73.00,    0x8F},
    { -74.00,    0x90},
    { -75.00,    0x92},
    { -76.00,    0x93},
    { -77.00,    0x95},
    { -78.00,    0x96},
    { -79.00,    0x98},
    { -80.00,    0x99},
    { -81.00,    0x9B},
    { -82.00,    0x9D},
    { -83.00,    0x9E},
    { -84.00,    0xA0},
    { -85.00,    0xA2},
    { -86.00,    0xA4},
    { -87.00,    0xA6},
    { -88.00,    0xA8},
    { -89.00,    0xAA},
    { -90.00,    0xAC},
    { -91.00,    0xAD},
    { -92.00,    0xAF},
    { -93.00,    0xB0},
    { -93.00,    0xFF},
};
*/
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

extern MS_BOOL MDrv_Demod_WriteDSPReg(MS_U16 RegAddr, MS_U16 RegData);
MS_BOOL MDrv_Demod_Init(void)
{
    DMD_DVBC_InitData sDMD_DVBC_InitData;
    MS_BOOL ret;
   /*
    if (_s32MutexId < 0)
    {
        _s32MutexId = MsOS_CreateMutex(E_MSOS_FIFO, "OfDmd_Mutex", MSOS_PROCESS_SHARED);

        if (_s32MutexId < 0)
        {
            GEN_EXCEP;
            return FALSE;
        }

    }
  */
    MDrv_SYS_DMD_VD_MBX_Init();
    MDrv_SAR_Kpd_Init();

    static MS_U8 u8DMD_DVBC_InitExt[] =
    {
        4, // version
        0, // reserved
        0x15, // TS_CLK
        1, // RFAGC tristate control default value, 1:trisate 0:non-tristate,never modify unless you know the meaning


        (MS_U8)(u32IFrequency >> 24), // IF Frequency
        (MS_U8)(u32IFrequency >> 16),
        (MS_U8)(u32IFrequency >> 8),
        (MS_U8)(u32IFrequency >> 0),

#if QFP_PK_ENABLE
        (MS_U8)(48000 >> 24), // FS Frequency
        (MS_U8)(48000 >> 16),
        (MS_U8)(48000 >> 8),
        (MS_U8)(48000 >> 0),
#else
        (MS_U8)(45474 >> 24), // FS Frequency
        (MS_U8)(45474 >> 16),
        (MS_U8)(45474 >> 8),
        (MS_U8)(45474 >> 0),
#endif

        0, // IQ Swap
        FRONTEND_DEMOD_IQ_TYPE,//0, // u8ADCIQMode : 0=I path, 1=Q path, 2=both IQ
        0, // u8PadSel : 0=Normal, 1=analog pad
        0, // bPGAEnable : 0=disable, 1=enable
        5, // u8PGAGain : default 5
        (MS_U8)(AutoSymbol_Timeout >> 8), // AutoSymbol_Timeout  10000ms~
        (MS_U8)(AutoSymbol_Timeout >> 0),
        (MS_U8)(FixSymbol_AutoQam_Timeout >> 8), // FixSymbol_AutoQam_Timeout 2000ms~
        (MS_U8)(FixSymbol_AutoQam_Timeout >> 0),
        (MS_U8)(FixSymbol_FixQam_Timeout >> 8), // FixSymbol_FixQam_Timeout  2000ms~
        (MS_U8)(FixSymbol_FixQam_Timeout >> 0),
    };                // tuner parameter

#if QFP_PK_ENABLE
    static MS_U8 u8DSPTable[] =
    {
        //addr_L add_H mask value
        0x01, 00,
        0x16, 0x00, 0xff, 0xBB, //FS_H  45.474M :0xB1  48M:0xBB
        0x15, 0x00, 0xff, 0x80, //FS_L  45.474M :0xA2  48M:0x80
    };                // tuner parameter
#else
    static MS_U8 u8DSPTable[] =
    {
        //addr_L add_H mask value
        0x01, 00,
        0x16, 0x00, 0xff, 0xB1, //FS_H  45.474M :0xB1  48M:0xBB
        0x15, 0x00, 0xff, 0xA2, //FS_L  45.474M :0xA2  48M:0x80
    };                // tuner parameter
#endif

    // tuner parameter
    sDMD_DVBC_InitData.u8SarChannel = 1; // 0xFF means un-connected
    sDMD_DVBC_InitData.pTuner_RfagcSsi = ALPS_TUNER_RfagcSsi;
    sDMD_DVBC_InitData.u16Tuner_RfagcSsi_Size = sizeof(ALPS_TUNER_RfagcSsi) / sizeof(DMD_RFAGC_SSI);
#if (FRONTEND_TUNER_TYPE != TUNER_MXL603)
    sDMD_DVBC_InitData.pTuner_IfagcSsi_LoRef = ALPS_TUNER_IfagcSsi_LoRef;
    sDMD_DVBC_InitData.u16Tuner_IfagcSsi_LoRef_Size = sizeof(ALPS_TUNER_IfagcSsi_LoRef) / sizeof(DMD_IFAGC_SSI);
    sDMD_DVBC_InitData.pTuner_IfagcSsi_HiRef = ALPS_TUNER_IfagcSsi_HiRef;
    sDMD_DVBC_InitData.u16Tuner_IfagcSsi_HiRef_Size = sizeof(ALPS_TUNER_IfagcSsi_HiRef) / sizeof(DMD_IFAGC_SSI);
#else
    sDMD_DVBC_InitData.pTuner_IfagcSsi_LoRef = NULL;
    sDMD_DVBC_InitData.u16Tuner_IfagcSsi_LoRef_Size = NULL;
    sDMD_DVBC_InitData.pTuner_IfagcSsi_HiRef = NULL;
    sDMD_DVBC_InitData.u16Tuner_IfagcSsi_HiRef_Size = NULL;
#endif
    sDMD_DVBC_InitData.pTuner_IfagcErr_LoRef = ALPS_TUNER_IfagcErr_LoRef;
    sDMD_DVBC_InitData.u16Tuner_IfagcErr_LoRef_Size = sizeof(ALPS_TUNER_IfagcErr_LoRef) / sizeof(DMD_IFAGC_SSI);
    sDMD_DVBC_InitData.pTuner_IfagcErr_HiRef = ALPS_TUNER_IfagcErr_HiRef;
    sDMD_DVBC_InitData.u16Tuner_IfagcErr_HiRef_Size = sizeof(ALPS_TUNER_IfagcErr_HiRef) / sizeof(DMD_IFAGC_SSI);
    sDMD_DVBC_InitData.pSqiCnNordigP1 = SqiCnNordigP1;
    sDMD_DVBC_InitData.u16SqiCnNordigP1_Size = sizeof(SqiCnNordigP1) / sizeof(DMD_SQI_CN_NORDIGP1);

    // register init
    sDMD_DVBC_InitData.u8DMD_DVBC_DSPRegInitExt = u8DSPTable; // TODO use system variable type
    sDMD_DVBC_InitData.u8DMD_DVBC_DSPRegInitSize = 2;
    sDMD_DVBC_InitData.u8DMD_DVBC_InitExt = u8DMD_DVBC_InitExt; // TODO use system variable type

    ret = MDrv_DMD_DVBC_Init(&sDMD_DVBC_InitData, sizeof(sDMD_DVBC_InitData)); // _UTOPIA
    if (ret == TRUE)
    {
        bInited = TRUE;
    }

#if MS_DEBUG_ENABLE
    MS_U16 demodfw;
    MDrv_DMD_DVBC_GetFWVer(&demodfw);
    printf("demod Fw=0x%x\n", demodfw);
#endif

    return ret;
}

MS_BOOL MDrv_Demod_Open(void)
{
   /*
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    HB_ReleaseMutex(_s32MutexId);
*/
    return TRUE;
}

MS_BOOL MDrv_Demod_Close(void)
{
    MS_BOOL ret;

    /*if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }*/

    ret = MDrv_DMD_DVBC_Exit();
    if (ret == TRUE)
    {
        bInited = FALSE;
    }
    //HB_ReleaseMutex(_s32MutexId);
    return ret;

}

MS_BOOL MDrv_Demod_SetSerialControl(MS_BOOL bEnable)
{
    MS_BOOL ret;
    /*
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
	*/

    if (bInited == FALSE)
    {
      //  HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }

    ret = MDrv_DMD_DVBC_SetSerialControl(bEnable);
//    HB_ReleaseMutex(_s32MutexId);
    return ret;
}

MS_BOOL MDrv_Demod_PowerOnOff(MS_BOOL bPowerOn)
{
/*
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    HB_ReleaseMutex(_s32MutexId);
*/
    return TRUE;
}

MS_BOOL MDrv_Demod_GetLock(MS_BOOL *pbLock)
{
    DMD_DVBC_LOCK_STATUS LockStatus;
   /*
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
   */
    if (bInited == FALSE)
    {
        *pbLock = FALSE;
     //   HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }

    if (MDrv_DMD_DVBC_GetLock(DMD_DVBC_GETLOCK, &LockStatus) != TRUE)
    {
       // HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }
    switch (LockStatus)
    {
    case DMD_DVBC_LOCK:
        *pbLock = TRUE;
        break;
    default:
        *pbLock = FALSE;
        break;
    }
    //HB_ReleaseMutex(_s32MutexId);
    return TRUE;
}
/*
static MS_U16 DTV_GetSignalQuality(void)
{

    MS_U16 u16Signal = 0;
    float fBER;
    float fLogBER;
    MS_BOOL bLock = FALSE;
    DMD_DVBC_LOCK_STATUS LockStatus;

    MDrv_DMD_DVBC_GetLock(DMD_DVBC_GETLOCK, &LockStatus);
    switch (LockStatus)
    {
    case DMD_DVBC_LOCK:
        bLock = TRUE;
        break;
    default:
        bLock = FALSE;
        break;
    }

    if (bLock)
    {
        if (MDrv_DMD_DVBC_GetPostViterbiBer(&fBER) == FALSE)
        {
            DBG_MSB(HB_printf("MDrv_Dmd_GetBER = 0\n"));
            return 0;
        }

        fLogBER = (-1) * _demod_log10(1 / fBER); // Log10Approx() provide 1~2^32 input range only

        DBG_MSB(HB_printf("Log(BER) = %f\n", fLogBER));

        if (fLogBER <= (-7.0))              // PostVit BER < 1e-7
        {
            u16Signal = 100;
        }
        else  if (fLogBER < (-3.7))          // PostVit BER < 2e-4
        {
            u16Signal = 60 + (((-3.7) - fLogBER) / ((-3.7) - (-7.0)) * (100 - 60));
        }
        else  if (fLogBER < (-2.7))          // PostVit BER < 2e-3
        {
            u16Signal = 10 + (((-2.7) - fLogBER) / ((-2.7) - (-3.7)) * (60 - 10));
        }
        else
        {
            u16Signal = 10;
        }

    }
    else
    {
        u16Signal = 0;
    }

    DBG_MSB(HB_printf("Signal Quility = %d\n", (int)u16Signal));

    return u16Signal;
}*/

MS_BOOL MDrv_Demod_GetSNR(MS_U32 *pu32SNR)
{
MS_BOOL ret;
   /*
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
   */

    if (bInited == FALSE)
    {
        *pu32SNR = 0;
        //HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }

     
   ret = MDrv_DMD_DVBC_GetSignalQuality((MS_U16*)pu32SNR);
   // HB_ReleaseMutex(_s32MutexId);
    return ret;
}

MS_BOOL MDrv_Demod_GetBER(float *pfBER)
{
    MS_BOOL ret;
/*
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
*/
    if (bInited == FALSE)
    {
        *pfBER = 0;
  //      HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }

    ret = MDrv_DMD_DVBC_GetPostViterbiBer(pfBER);

 //   HB_ReleaseMutex(_s32MutexId);
    return ret;

}

#if DEMOD_ADJUST_SSI
MS_U8 MDrv_Demod_GetIFAGC(void)
{
    MS_U8 u8Data;
    MDrv_DMD_DVBC_SetReg(0x2100 + 0x22, 0x03);
    MDrv_DMD_DVBC_GetReg(0x2100 + 0x25, &u8Data);
    printf("====IF AGC=0x%x\n", u8Data);
    return u8Data;
}
#endif

MS_BOOL MDrv_Demod_GetPWR(MS_S32 *ps32Signal)
{
    MS_BOOL ret = TRUE;;
    MS_BOOL bLock = FALSE;
    DMD_DVBC_LOCK_STATUS LockStatus;
    /*if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }*/

    MDrv_DMD_DVBC_GetLock(DMD_DVBC_GETLOCK, &LockStatus);
    switch (LockStatus)
    {
    case DMD_DVBC_LOCK:
        bLock = TRUE;
        break;
    default:
        bLock = FALSE;
        break;
    }

    if (bInited == FALSE)
    {
        *ps32Signal = 0;
        //HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }
    if (bLock)
    {
        MS_U16 u16signal = 0;
#if (FRONTEND_TUNER_TYPE == TUNER_MXL603)
        float fSignal;
        *ps32Signal = 0;
        if (MXL_SUCCESS != MxLWare603_API_ReqTunerRxPower(MXL603_I2C_ADDR, &fSignal))
        {
            DBG_MSB(HB_printf("MxLWare603_API_ReqTunerRxPower failed !!!\n"));
            return FALSE;
        }
        MDrv_DMD_DVBC_GetSignalStrengthWithRFPower(&u16signal, fSignal);
#else
        ret = MDrv_DMD_DVBC_GetSignalStrength(&u16signal);
#endif
        *ps32Signal = u16signal;
#if DEMOD_ADJUST_SSI
        MDrv_Demod_GetIFAGC();
#endif
    }
    else
    {
        *ps32Signal = 0;
    }
    //HB_ReleaseMutex(_s32MutexId);
    return ret;
}


MS_BOOL MDrv_Demod_Config(MS_U8 * pRegParam)
{
/*
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    HB_ReleaseMutex(_s32MutexId);
*/
    return TRUE;
}

MS_BOOL MDrv_Demod_GetParam(DEMOD_MS_FE_CARRIER_PARAM * pParam)
{
    DMD_DVBC_MODULATION_TYPE QAMMode;
    MS_U16 u16SymbolRate;
    float FreqOff;
/*
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
*/

    if (bInited == FALSE)
    {
  //      HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }

    if (FALSE == MDrv_DMD_DVBC_GetStatus(&QAMMode, &u16SymbolRate, &FreqOff))
    {
    //    HB_ReleaseMutex(_s32MutexId);
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

//    HB_ReleaseMutex(_s32MutexId);

    return TRUE;

}

MS_BOOL MDrv_Demod_Restart(DEMOD_MS_FE_CARRIER_PARAM * pParam)
{
    DMD_DVBC_MODULATION_TYPE eModulationType;
    static DMD_DVBC_MODULATION_TYPE ePreModulationType = DMD_DVBC_QAMAUTO;
    static MS_U16 u16PreSymbolRate = 0;
/*
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
*/
    if (bInited == FALSE)
    {
  //      HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }

    switch (pParam->CabParam.eConstellation)
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

#if USE_6M_ACIFILTER
    MS_U8  u8Data;
    // bit0 means  SAWLESS_EN and bit4 means 6M_ACI_FILTER(1 means use 6M).
    MDrv_DMD_DVBC_GetDSPReg(0x0f, &u8Data);
    u8Data &= (~_BIT4);
    MDrv_Demod_WriteDSPReg(0x0f, u8Data | (USE_6M_ACIFILTER << 4));
#else
   // MDrv_Demod_WriteDSPReg(0x0f, pParam->CabParam.u8DemodConfig);
#endif

    //if((ePreModulationType != eModulationType) || (u16PreSymbolRate != pParam->CabParam.u16SymbolRate))
    //{
    ePreModulationType = eModulationType;
    u16PreSymbolRate = pParam->CabParam.u16SymbolRate;
    printf("pParam->CabParam.u16SymbolRate [%d], eModulationType [%d] \n", pParam->CabParam.u16SymbolRate, eModulationType);

    if (FALSE == MDrv_DMD_DVBC_SetConfig(pParam->CabParam.u16SymbolRate, eModulationType, u32IFrequency, FALSE, FALSE))
    {
        printf("MDrv_DMD_DVBC_SetConfig Fail \n");
    //    HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }
    if (FALSE == MDrv_DMD_DVBC_SetActive(TRUE))
    {
        printf("MDrv_DMD_DVBC_SetActive Fail \n");
      //  HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }
    //}

    //HB_ReleaseMutex(_s32MutexId);
    return TRUE;

}


MS_BOOL MDrv_Demod_I2C_ByPass(MS_BOOL bOn)
{
    return TRUE;
}

MS_BOOL MDrv_Demod_SetMode(Demod_Mode * pMode)
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
/*
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    HB_ReleaseMutex(_s32MutexId);
*/
    return TRUE;
#endif
}

MS_BOOL MDrv_Demod_SetOutoutPath(DEMOD_INTERFACE_MODE path)
{
/*
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    HB_ReleaseMutex(_s32MutexId);
*/
    return TRUE;
}

DEMOD_INTERFACE_MODE MDrv_Demod_GetOutoutPath(void)
{
/*
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    HB_ReleaseMutex(_s32MutexId);
*/
    return DEMOD_INTERFACE_PARALLEL;
}

MS_U16 MDrv_Demod_ReadReg(MS_U16 RegAddr)
{
    MS_U8 RegData = 0;
    MDrv_DMD_DVBC_GetReg(RegAddr, &RegData); //MS_BOOL MDrv_DMD_DVBC_GetReg(MS_U16 u16Addr, MS_U8 *pu8Data)
    return (MS_U16) RegData;
}

MS_BOOL MDrv_Demod_WriteReg(MS_U16 RegAddr, MS_U16 RegData)
{
    MS_U8 _RegData = RegData & 0x00FF;
    return MDrv_DMD_DVBC_SetReg(RegAddr, _RegData); //MS_BOOL MDrv_DMD_DVBC_SetReg(MS_U16 u16Addr, MS_U8 u8Data)
}

MS_U16 MDrv_Demod_ReadDSPReg(MS_U16 RegAddr)
{
    MS_U8 RegData = 0;
    MDrv_DMD_DVBC_GetDSPReg(RegAddr, &RegData); //MS_BOOL MDrv_DMD_DVBC_GetReg(MS_U16 u16Addr, MS_U8 *pu8Data)
    return (MS_U16) RegData;
}

MS_BOOL MDrv_Demod_WriteDSPReg(MS_U16 RegAddr, MS_U16 RegData)
{
    MS_U8 _RegData = RegData & 0x00FF;
    return MDrv_DMD_DVBC_SetDSPReg(RegAddr, _RegData); //MS_BOOL MDrv_DMD_DVBC_SetReg(MS_U16 u16Addr, MS_U8 u8Data)
}
#endif // (FRONTEND_DEMOD_TYPE == DEMOD_MSDVBC_51)




/////////////////////////////////////////////////////////////////////////////////
#else //ENABLE_HUMMINGBIRD_CUSTOMER
//#include "Board.h"

#if (FRONTEND_DEMOD_TYPE == EMBEDDED_DVBC51_DEMOD)

//#include "MsCommon.h"
//#include "HbCommon.h"
#include "sys/drv/drvDMD_INTERN_DVBC.h"
#include "device/drvDemod.h"
#include "sys/drv/drvDMD_VD_MBX.h"
#include "sys/drv/drvSAR.h"

#define DEMOD_IQ_I_IN 0
#define DEMOD_IQ_Q_IN 1

#define FRONTEND_DEMOD_IQ_TYPE      DEMOD_IQ_Q_IN // C:Q T:I
#define COFDMDMD_MUTEX_TIMEOUT       (2000)

#ifdef MS_DEBUG
#define DBG_MSB(x)  x
#else
#define DBG_MSB(x)
#endif

static MS_BOOL bInited = FALSE;
//static MS_S32 _s32MutexId = -1;

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

MS_BOOL MDrv_Demod_Init(void)
{
    DMD_DVBC_InitData sDMD_DVBC_InitData;
    MS_BOOL ret;
    /*
    if (_s32MutexId < 0)
    {
        _s32MutexId = MsOS_CreateMutex(E_MSOS_FIFO, "OfDmd_Mutex", MSOS_PROCESS_SHARED);

        if (_s32MutexId < 0)
        {
            GEN_EXCEP;
            return FALSE;
        }

    }
    */
    MDrv_SYS_DMD_VD_MBX_Init();
    MDrv_SAR_Kpd_Init();

    printf("------Power_On_Initialization------\n");
    static MS_U8 u8DMD_DVBC_InitExt[]={
       3, // version
       0, // reserved
       0x15, // TS_CLK
       1, // RFAGC tristate control default value, 1:trisate 0:non-tristate,never modify unless you know the meaning
       (MS_U8)(36167>>24), // IF Frequency
       (MS_U8)(36167>>16),
       (MS_U8)(36167>>8),
       (MS_U8)(36167>>0),
       (MS_U8)(45474>>24), // FS Frequency
       (MS_U8)(45474>>16),
       (MS_U8)(45474>>8),
       (MS_U8)(45474>>0),
       0, // IQ Swap
       FRONTEND_DEMOD_IQ_TYPE,//0, // u8ADCIQMode : 0=I path, 1=Q path, 2=both IQ
       0, // u8PadSel : 0=Normal, 1=analog pad
       0, // bPGAEnable : 0=disable, 1=enable
       5, // u8PGAGain : default 5
       };                // tuner parameter

    // tuner parameter
    sDMD_DVBC_InitData.u8SarChannel=1; // 0xFF means un-connected
    sDMD_DVBC_InitData.pTuner_RfagcSsi=ALPS_TUNER_RfagcSsi;
    sDMD_DVBC_InitData.u16Tuner_RfagcSsi_Size=sizeof(ALPS_TUNER_RfagcSsi)/sizeof(DMD_RFAGC_SSI);
    sDMD_DVBC_InitData.pTuner_IfagcSsi_LoRef=ALPS_TUNER_IfagcSsi_LoRef;
    sDMD_DVBC_InitData.u16Tuner_IfagcSsi_LoRef_Size=sizeof(ALPS_TUNER_IfagcSsi_LoRef)/sizeof(DMD_IFAGC_SSI);
    sDMD_DVBC_InitData.pTuner_IfagcSsi_HiRef=ALPS_TUNER_IfagcSsi_HiRef;
    sDMD_DVBC_InitData.u16Tuner_IfagcSsi_HiRef_Size=sizeof(ALPS_TUNER_IfagcSsi_HiRef)/sizeof(DMD_IFAGC_SSI);
    sDMD_DVBC_InitData.pTuner_IfagcErr_LoRef=ALPS_TUNER_IfagcErr_LoRef;
    sDMD_DVBC_InitData.u16Tuner_IfagcErr_LoRef_Size=sizeof(ALPS_TUNER_IfagcErr_LoRef)/sizeof(DMD_IFAGC_SSI);
    sDMD_DVBC_InitData.pTuner_IfagcErr_HiRef=ALPS_TUNER_IfagcErr_HiRef;
    sDMD_DVBC_InitData.u16Tuner_IfagcErr_HiRef_Size=sizeof(ALPS_TUNER_IfagcErr_HiRef)/sizeof(DMD_IFAGC_SSI);
    sDMD_DVBC_InitData.pSqiCnNordigP1=SqiCnNordigP1;
    sDMD_DVBC_InitData.u16SqiCnNordigP1_Size=sizeof(SqiCnNordigP1)/sizeof(DMD_SQI_CN_NORDIGP1);

    // register init
    sDMD_DVBC_InitData.u8DMD_DVBC_DSPRegInitExt=NULL; // TODO use system variable type
    sDMD_DVBC_InitData.u8DMD_DVBC_DSPRegInitSize=0;
    sDMD_DVBC_InitData.u8DMD_DVBC_InitExt= u8DMD_DVBC_InitExt; // TODO use system variable type
    MDrv_DMD_DVBC_SetDbgLevel(DMD_DVBC_DBGLV_INFO);
    ret = MDrv_DMD_DVBC_Init(&sDMD_DVBC_InitData, sizeof(sDMD_DVBC_InitData)); // _UTOPIA
    if(ret == TRUE)
    {
        bInited = TRUE;
    }
    //MDrv_DMD_DVBC_IQ(FRONTEND_DEMOD_IQ_TYPE);

    return ret;
}

MS_BOOL MDrv_Demod_Open(void)
{
    /*
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    HB_ReleaseMutex(_s32MutexId);
    */
    return TRUE;
}

MS_BOOL MDrv_Demod_Close(void)
{
    MS_BOOL ret;
    /*
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    */

    ret = MDrv_DMD_DVBC_Exit();
    if(ret == TRUE)
    {
        bInited = FALSE;
    }
    //HB_ReleaseMutex(_s32MutexId);
    return ret;

}

MS_BOOL MDrv_Demod_SetSerialControl(MS_BOOL bEnable)
{
    MS_BOOL ret;
    /*
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    */
    if(bInited == FALSE)
    {
    //    HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }

    ret = MDrv_DMD_DVBC_SetSerialControl(bEnable);
    //HB_ReleaseMutex(_s32MutexId);
    return ret;
}

MS_BOOL MDrv_Demod_PowerOnOff(MS_BOOL bPowerOn)
{
    /*
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    HB_ReleaseMutex(_s32MutexId);
    */
    return TRUE;
}

MS_BOOL MDrv_Demod_GetLock(MS_BOOL *pbLock)
{
    DMD_DVBC_LOCK_STATUS LockStatus;
    /*
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    */
    if(bInited == FALSE)
    {
        *pbLock = FALSE;
    //    HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }


    if(MDrv_DMD_DVBC_GetLock(DMD_DVBC_GETLOCK, &LockStatus) != TRUE)
    {
    //    HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }
    switch (LockStatus)
    {
        case DMD_DVBC_LOCK:
            *pbLock = TRUE;
            break;
        default:
            *pbLock = FALSE;
            break;
    }
    //HB_ReleaseMutex(_s32MutexId);
    return TRUE;
}

MS_BOOL MDrv_Demod_GetSNR(MS_U32 *pu32SNR)
{
    MS_BOOL ret;
    /*
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    */

    if(bInited == FALSE)
    {
        *pu32SNR = 0;
    //    HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }


    ret = MDrv_DMD_DVBC_GetSignalQuality((MS_U16*)pu32SNR);
    //HB_ReleaseMutex(_s32MutexId);
    return ret;
}

MS_BOOL MDrv_Demod_GetBER(float *pfBER)
{
    MS_BOOL ret;
    /*
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    */
    if(bInited == FALSE)
    {
        *pfBER = 0;
    //    HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }

    ret = MDrv_DMD_DVBC_GetPostViterbiBer(pfBER);

    //HB_ReleaseMutex(_s32MutexId);
    return ret;

}

MS_BOOL MDrv_Demod_GetPWR(MS_S32 *ps32Signal)
{
    MS_BOOL ret;
    /*
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    */
    if(bInited == FALSE)
    {
        *ps32Signal = 0;
    //    HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }
    ret = MDrv_DMD_DVBC_GetSignalStrength((MS_U16*)ps32Signal);
    //HB_ReleaseMutex(_s32MutexId);
    return ret;
}


MS_BOOL MDrv_Demod_Config(MS_U8 *pRegParam)
{
    /*
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    HB_ReleaseMutex(_s32MutexId);
    */
    return TRUE;
}


MS_BOOL MDrv_Demod_GetParam(DEMOD_MS_FE_CARRIER_PARAM* pParam)
{
    DMD_DVBC_MODULATION_TYPE QAMMode;
    MS_U16 u16SymbolRate;
    float FreqOff;
    /*
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    */

    if(bInited == FALSE)
    {
    //    HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }

    if(FALSE == MDrv_DMD_DVBC_GetStatus(&QAMMode, &u16SymbolRate, &FreqOff))
    {
    //    HB_ReleaseMutex(_s32MutexId);
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
        default:
            pParam->CabParam.eConstellation = DEMOD_CAB_QAM128;
            break;
    }

    pParam->CabParam.u16SymbolRate = u16SymbolRate;
    pParam->CabParam.u32FreqOffset = FreqOff;

    //HB_ReleaseMutex(_s32MutexId);

    return TRUE;

}

MS_BOOL MDrv_Demod_Restart(DEMOD_MS_FE_CARRIER_PARAM* pParam)
{
    DMD_DVBC_MODULATION_TYPE eModulationType;
    static DMD_DVBC_MODULATION_TYPE ePreModulationType = DMD_DVBC_QAMAUTO;
    static MS_U16 u16PreSymbolRate = 0;
    /*
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    */
    if(bInited == FALSE)
    {
    //    HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }

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

    if((ePreModulationType != eModulationType) || (u16PreSymbolRate != pParam->CabParam.u16SymbolRate))
    {
        ePreModulationType = eModulationType;
        u16PreSymbolRate = pParam->CabParam.u16SymbolRate;
        printf("pParam->CabParam.u16SymbolRate [%d], eModulationType [%d] \n",pParam->CabParam.u16SymbolRate,eModulationType);

        if(FALSE == MDrv_DMD_DVBC_SetConfig(pParam->CabParam.u16SymbolRate, eModulationType, 36130, FALSE, FALSE))
        {
            printf("MDrv_DMD_DVBC_SetConfig Fail \n");
        //    HB_ReleaseMutex(_s32MutexId);
            return FALSE;
        }
        if(FALSE == MDrv_DMD_DVBC_SetActive(TRUE))
        {
            printf("MDrv_DMD_DVBC_SetActive Fail \n");
        //    HB_ReleaseMutex(_s32MutexId);
            return FALSE;
        }
    }

    //HB_ReleaseMutex(_s32MutexId);
    return TRUE;

}


MS_BOOL MDrv_Demod_SetMode(Demod_Mode* pMode)
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
    /*
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    HB_ReleaseMutex(_s32MutexId);
    */
    return TRUE;
#endif
}

MS_BOOL MDrv_Demod_SetOutoutPath(DEMOD_INTERFACE_MODE path)
{
    /*
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    HB_ReleaseMutex(_s32MutexId);
    */
    return TRUE;
}

DEMOD_INTERFACE_MODE MDrv_Demod_GetOutoutPath(void)
{
    /*
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    HB_ReleaseMutex(_s32MutexId);
    */
    return DEMOD_INTERFACE_PARALLEL;
}

#endif // (FRONTEND_DEMOD_TYPE == DEMOD_MSDVBC_51)
#endif  //ENABLE_HUMMINGBIRD_CUSTOMER

