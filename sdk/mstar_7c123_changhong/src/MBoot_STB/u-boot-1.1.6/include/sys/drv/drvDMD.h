////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2007 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (¡§MStar Confidential Information¡¨) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   drvDMD.h
/// @brief  DVBC Demodulator Driver Interface
/// @author MStar Semiconductor,Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DRV_DMD_H_
#define _DRV_DMD_H_



#ifdef __cplusplus
extern "C"
{
#endif

//-------------------------------------------------------------------------------------------------
//  Driver Capability
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

/// Mdulation constellation scheme
typedef enum _DMD_QamType
{
    E_DMD_QAM16  = 0,                                                   ///< QAM 16
    E_DMD_QAM32  = 1,                                                   ///< QAM 32
    E_DMD_QAM64  = 2,                                                   ///< QAM 64
    E_DMD_QAM128 = 3,                                                   ///< QAM 128
    E_DMD_QAM256 = 4,                                                   ///< QAM 256

    E_DMD_QAM_LASTNUM,
} __attribute__((packed)) DMD_QAMType;

/// Demodulation IQ type
typedef enum _DMD_IQType
{
    E_DMD_IQ_NORMAL = 0,                                                ///< IQ Normal
    E_DMD_IQ_INVERT = 1,                                                ///< IQ Invert
} __attribute__((packed)) DMD_IQType;

/// Demodulation tap assign
typedef enum _DMD_TapAssign
{
    E_DMD_TAP_PRE  = 0,                                                 ///< PreTap assign
    E_DMD_TAP_POST = 1,                                                 ///< PostTap assign
} __attribute__((packed)) DMD_TapAssign;


#if   1//(FRONTEND_DEMOD_TYPE == DEMOD_MSDVBC)

/// Demodulator auto tune control
typedef struct
{
    // Demodulator option
    MS_BOOL                         bX4CFE_en;                          ///< Carrier frequency estimation
    MS_BOOL                         bPPD_en;                            ///< Tap assign estimation
    MS_BOOL                         bIQAutoSwap_en;                     ///< IQ mode auto swap
    MS_BOOL                         bQAMScan_en;                        ///< QAM type auto scan
    MS_BOOL                         bFHO_en;                            ///< FHO
	MS_BOOL							(*fptTunerSet)(MS_U32);				///< Tuner set freq function pointer
} DMD_Mode;

/// Demodulator tune parameter
typedef struct
{
    // Channel setting
    MS_U32                          u32SymRate;                         ///< Symbol rate (Ksym/sec)

    // Channel characteristic
    DMD_QAMType                     eQamType;                           ///< QAM type
    DMD_IQType                      eIQSwap;                            ///< IQ type
    DMD_TapAssign                   eTapAssign;                         ///< Tap assign
    MS_U32                          u32FreqOffset;                      ///< Carrier frequency offset
	MS_U8							u8TuneFreqOffset;					///< Request Tuner offset
	MS_U32							u32TunerFreq;						///< Request Tuner offset
} DMD_Param;

/// Demodulator special status parameter
typedef struct
{
	MS_BOOL							bRtyTunerFreq;						///< Tuner freq retry flag
	MS_BOOL							bRtyTunerOvr;						///< Retry over indicator for freq offset retry
	MS_S32							s32TunerFreqOffset;					///< Demod request tuner freq offset
}DMD_Status;

#else

typedef struct
{
    // N/A
} DMD_Mode;

// Device dependant parameter
typedef struct
{
    // N/A
} DMD_Param;

typedef struct
{
	// N/A
}DMD_Status;

#endif


//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
extern MS_BOOL MDrv_Dmd_Init(void);
extern MS_BOOL MDrv_Dmd_Open(void);
extern MS_BOOL MDrv_Dmd_Close(void);
extern MS_BOOL MDrv_Dmd_Reset(void);
extern MS_BOOL MDrv_Dmd_Restart(DMD_Param *pParam);
extern MS_BOOL MDrv_Dmd_SetMode(DMD_Mode *pMode);
extern MS_BOOL MDrv_Dmd_TsOut(MS_BOOL bEnable);
extern MS_BOOL MDrv_Dmd_PowerOnOff(MS_BOOL bPowerOn);
extern MS_BOOL MDrv_Dmd_SetBW(MS_U32 u32BW);
extern MS_BOOL MDrv_Dmd_GetBW(MS_U32 *pu32BW);
extern MS_BOOL MDrv_Dmd_GetLock(MS_BOOL *pbLock);
extern MS_BOOL MDrv_Dmd_GetSNR(MS_U32 *pu32SNR);
extern MS_BOOL MDrv_Dmd_GetBER(float *pfBER);
extern MS_BOOL MDrv_Dmd_GetPWR(MS_S32 *ps32Signal);
extern MS_BOOL MDrv_Dmd_GetParam(DMD_Param *pParam);
extern MS_BOOL MDrv_Dmd_Config(MS_U8 *pRegParam);

#ifdef __cplusplus
}
#endif


#endif // _DRV_DMD_H_
