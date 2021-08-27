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

//////////////////////////////////////////////////////////////////////////////
///
/// @file   apiDigiTuner.h
/// @brief  Digital Tuner Low-Level API
/// @author MStar Semiconductor Inc.
//////////////////////////////////////////////////////////////////////////////

#ifndef _API_DIGI_TUNER_H_
#define _API_DIGI_TUNER_H_

//---------------------------------------------------------------------------
//  Include file
//---------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

/// Define converlution code rate for DVB-T and DVB-S
typedef enum
{
    CONV_CODE_RATE_1_2,                                                 ///< Code rate = 1/2
    CONV_CODE_RATE_2_3,                                                 ///< Code rate = 2/3
    CONV_CODE_RATE_3_4,                                                 ///< Code rate = 3/4
    CONV_CODE_RATE_5_6,                                                 ///< Code rate = 5/6
    CONV_CODE_RATE_7_8,                                                 ///< Code rate = 7/8
} EN_CONV_CODE_RATE_TYPE;

/// Define terrestrial band width
#if (ENABLE_HUMMINGBIRD_CUSTOMER)
typedef enum
{
	BW_MODE_6MHZ = 0,                                                   ///< 6 MHz
	BW_MODE_7MHZ,                                                       ///< 7 MHz
	BW_MODE_8MHZ                                                        ///< 8 MHz
} EN_TER_BW_MODE;
#endif 

#if ENABLE_ATSC_SYS
typedef enum
{
      DEMOD_MODE_NTSC,
      DEMOD_MODE_256QAM,
      DEMOD_MODE_64QAM,
      DEMOD_MODE_8VSB,
} EN_CONSTEL_MODE;
#endif

/// Define terrestrial constellation type
typedef enum
{
    TER_QPSK,                                                           ///< QPSK type
    TER_QAM16,                                                          ///< QAM 16 type
    TER_QAM64                                                           ///< QAM 64 type
} EN_TER_CONSTEL_TYPE;

/// Define terrestrial hierarchy information
typedef enum
{
    TER_HIE_NONE,                                                       ///< Non-hierarchy
    TER_HIE_ALPHA_1,                                                    ///< Hierarchy alpha = 1
    TER_HIE_ALPHA_2,                                                    ///< Hierarchy alpha = 2
    TER_HIE_ALPHA_4                                                     ///< Hierarchy alpha = 4
} EN_TER_HIE_TYPE;

/// Define terrestrial guard interval
typedef enum
{
    TER_GI_1_32,                                                        ///< Guard interval value = 1/32
    TER_GI_1_16,                                                        ///< Guard interval value = 1/16
    TER_GI_1_8,                                                         ///< Guard interval value = 1/8
    TER_GI_1_4                                                          ///< Guard interval value = 1/4
} EN_TER_GI_TYPE;

/// Define terrestrial transmission mode
typedef enum
{
    TER_FFT_2K,                                                         ///< 2k FFT mode
    TER_FFT_8K                                                          ///< 8k FFT mode
} EN_TER_FFT_MODE;

/// Define terrestrial transmission mode
typedef enum
{
    TER_HP_SEL,                                                         ///< High priority level selection
    TER_LP_SEL                                                          ///< Low priority level selection
} EN_TER_LEVEL_SEL;

/// Define DVB-C modulation scheme
typedef enum
{
    CAB_QAM16,                                                          ///< QAM 16
    CAB_QAM32,                                                          ///< QAM 32
    CAB_QAM64,                                                          ///< QAM 64
    CAB_QAM128,                                                         ///< QAM 128
    CAB_QAM256
} EN_CAB_CONSTEL_TYPE;


/// Define DVB-S IQ tuning mode

typedef enum
{
    CAB_IQ_NORMAL,                                                      ///< Normal
    CAB_IQ_INVERT                                                       ///< Inverse
} EN_CAB_IQ_MODE;


/// Define DVB-S modulatiopn scheme
typedef enum
{
    SAT_QPSK,                                                           ///< QPSK
    SAT_8PSK,                                                           ///< 8PSK
    SAT_QAM16                                                           ///< QAM16
} EN_SAT_CONSTEL_TYPE;

/// Define DVB-S Roll-Off factor
typedef enum
{
    SAT_RO_35,                                                          ///< roll-off factor = 0.35
    SAT_RO_25,                                                          ///< roll-off factor = 0.25
    SAT_RO_20                                                           ///< roll-off factor = 0.20
} EN_SAT_ROLL_OFF_TYPE;

/// Define DVB-S IQ tuning mode
typedef enum
{
    SAT_IQ_NORMAL,                                                      ///< Normal
    SAT_IQ_INVERSE                                                      ///< Inverse
} EN_SAT_IQ_MODE;

/// Define Bit Error Rate range measure from signal
typedef enum
{
    BIT_ERR_RATIO_LOW ,                                                 ///< Low BER
    BIT_ERR_RATIO_MEDIUM ,                                              ///< Medium BER
    BIT_ERR_RATIO_HIGH                                                  ///< High BER
} EN_BIT_ERR_RATIO;

/// Define lock status of front end
typedef enum
{
    FE_UNLOCKED = 0,                                                    ///< Frontend is unlocked
    FE_LOCKED                                                           ///< Frontend is locked
} EN_FE_LOCK_STATUS;


/// Define tuning mode
/// NOTE: When this typedef is modified, the apiChScan should be rebuild.
typedef enum
{
    FE_TUNE_MANUAL,                                                     ///< Manual tuning to carrier
    FE_TUNE_AUTO,                                                       ///< Auto tuning to carrier
} EN_FE_TUNE_MODE;

/// Define symbol rate for DVB-S, unit in symbol/sec
typedef MS_U32                      FE_SYMBOL_RATE;                     ///< unit in symbol/sec

/// Define the cellule identifier
typedef MS_U16                      FE_CELL_ID;

/// Define tuning paramter of DVB-T front-end
typedef struct
{
    EN_TER_BW_MODE                  eBandWidth;                         ///< Band width
#if  (ENABLE_ATSC_SYS)
    EN_CONSTEL_MODE	eDemodMode; 
#else
    EN_TER_CONSTEL_TYPE             eConstellation;                     ///< Constellation type
#endif
    EN_TER_HIE_TYPE                 eHierarchy;                         ///< Hierarchy
    EN_TER_GI_TYPE                  eGuardInterval;                     ///< Guard interval
    EN_TER_FFT_MODE                 eFFT_Mode;                          ///< Transmission mode
    EN_CONV_CODE_RATE_TYPE          eHPCodeRate;                        ///< HP code rate
    EN_CONV_CODE_RATE_TYPE          eLPCodeRate;                        ///< LP code rate
    EN_TER_LEVEL_SEL                eLevelSel;                          ///< Select HP or LP level
} MS_TER_CARRIER_PARAM;

/// Define tuning paramter of DVB-C front-end
typedef struct
{
    EN_CAB_CONSTEL_TYPE             eConstellation;                     ///< Constellation type
    MS_U16                          u16SymbolRate;                      ///< Symbol rate (Ksym/sec)

    EN_CAB_IQ_MODE                  eIQMode;                            ///< IQ Mode
    MS_U8                           u8TapAssign;                        ///< Tap assign
    MS_U32                          u32FreqOffset;                      ///< Carrier frequency offset
	MS_U8							u8TuneFreqOffset;					///< Requeset tuner freq offset
} MS_CAB_CARRIER_PARAM;

/// Define tuning paramter of DVB-S front-end
typedef struct
{
    EN_SAT_CONSTEL_TYPE             eConstellation;                     ///< Constellation type
    EN_SAT_ROLL_OFF_TYPE            eRollOff;                           ///< Roll-Off factor
    EN_SAT_IQ_MODE                  eIQ_Mode;                           ///< IQ mode
    EN_CONV_CODE_RATE_TYPE          eCodeRate;                          ///< Converlution code rate
    MS_U32                          u32SymbolRate;                      ///< Symbol rate (Ksym/sec)
    MS_U8                            u8Polarity; // 0: Horizon; > 0(default 1): Vertical;
    MS_U8                            u8SatID;
} MS_SAT_CARRIER_PARAM;

/// Define carrier paramter of digital tuner
/// NOTE: When this typedef is modified, the apiChScan should be rebuild.
typedef struct
{
    MS_U32                          u32Frequency;                       ///< RF frequency
    union
    {
	    MS_TER_CARRIER_PARAM        TerParam;                           ///< Paramters for DVB-T front-end
	    MS_CAB_CARRIER_PARAM        CabParam;                           ///< Paramters for DVB-C front-end
	    MS_SAT_CARRIER_PARAM        SatParam;                           ///< Paramters for DVB-S front-end
    };
} MS_FE_CARRIER_PARAM;

/// Define the quality report
typedef struct
{
    EN_FE_LOCK_STATUS               eLock;
    EN_BIT_ERR_RATIO                eBER;                               ///< Bit error rate
    MS_U8                           u8SNR;
    MS_U8                           u8Signal;
} MS_FE_CARRIER_STATUS;

/// Define the carrier information
typedef struct
{
    MS_FE_CARRIER_PARAM             Param;                              ///< Carrier parameter
    MS_FE_CARRIER_STATUS            Status;                             ///< Quality report
} MS_FE_CARRIER_INFO;


//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------

extern MS_BOOL MApi_DigiTuner_Init(void);
//extern MS_BOOL MApi_DigiTuner_Tune2RfCh(MS_FE_CARRIER_PARAM *pParam, EN_FE_TUNE_MODE eMode);
extern MS_BOOL MApi_DigiTuner_ForceTune2RfCh(MS_FE_CARRIER_PARAM *pParam, EN_FE_TUNE_MODE eMode);
extern MS_BOOL MApi_DigiTuner_GetMaxLockTime(EN_FE_TUNE_MODE eMode, MS_U32 *u32LockTime);
extern MS_BOOL MApi_DigiTuner_PowerOnOff(MS_BOOL bEnPower);
extern MS_BOOL MApi_DigiTuner_GetTPSInfo(MS_FE_CARRIER_PARAM* pParam);
extern MS_BOOL MApi_DigiTuner_SetBW(EN_TER_BW_MODE eBandWidth);
extern MS_BOOL MApi_DigiTuner_GetLock(void);
extern MS_BOOL MApi_DigiTuner_GetSNR(MS_U16 *pu32SNR);
extern MS_BOOL MApi_DigiTuner_GetPWR(MS_S16 *ps16PWR);
extern MS_BOOL MApi_DigiTuner_GetBER(float *pfltBER);

#if defined(DVBS)
extern MS_BOOL MApi_DigiTuner_SetLOF(MS_U16 u16Lof);
extern MS_BOOL MApi_DigiTuner_Satellite_Set22K(MS_BOOL b22KOn);
extern MS_BOOL MApi_DigiTuner_Satellite_SetLNBPower(MS_U8 u8Polarity);
extern MS_BOOL MApi_DigiTuner_DiSEqC_SendCmd(MS_U8* pCmd,MS_U8 u8CmdSize);
#endif
MS_BOOL MApi_DigiTuner_SetTsSerial(MS_BOOL bSerial);
void MApi_DigiTuner_HWReset(MS_BOOL bReset);

#endif // _API_DIGI_TUNER_H_

