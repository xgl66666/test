#ifndef __DRV_DEMOD_H__
#define __DRV_DEMOD_H__

//#include "Board.h"
#if (ENABLE_HUMMINGBIRD_CUSTOMER)
#include <sys/common/MsTypes.h>
#include <sys/common/MsOS.h>
#include <sys/drv/drvGPIO.h>
#include <sys/common/MsCommon.h>
#endif
#if 0
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
    MS_U8                           u8TuneFreqOffset;                   ///< Request Tuner offset
    MS_U32                          u32TunerFreq;                       ///< Request Tuner offset
} DMD_Param;
#endif
#if (ENABLE_HUMMINGBIRD_CUSTOMER)
#if (ENABLE_DVB_S2_SYS||ENABLE_DVB_S_SYS)
typedef enum
{
	COFDM_FEC_LOCK,
	COFDM_PSYNC_LOCK,
	COFDM_TPS_LOCK,
	COFDM_DCR_LOCK,
	COFDM_AGC_LOCK,
	COFDM_MODE_DET,

} COFDM_LOCK_STATUS;
#endif
#endif // end ENABLE_HUMMINGBIRD_CUSTOMER
/// Define converlution code rate for DVB-T and DVB-S
typedef enum
{
    DEMOD_CONV_CODE_RATE_1_2,                                                 ///< Code rate = 1/2
    DEMOD_CONV_CODE_RATE_2_3,                                                 ///< Code rate = 2/3
    DEMOD_CONV_CODE_RATE_3_4,                                                 ///< Code rate = 3/4
    DEMOD_CONV_CODE_RATE_5_6,                                                 ///< Code rate = 5/6
    DEMOD_CONV_CODE_RATE_7_8,                                                 ///< Code rate = 7/8
} DEMOD_EN_CONV_CODE_RATE_TYPE;

/// Define terrestrial band width
typedef enum
{
    DEMOD_BW_MODE_6MHZ = 0,                                                   ///< 6 MHz
    DEMOD_BW_MODE_7MHZ,                                                       ///< 7 MHz
    DEMOD_BW_MODE_8MHZ ,
    #if (ENABLE_HUMMINGBIRD_CUSTOMER)	                                                       ///< 8 MHz
    DEMOD_BW_MODE_1_7MHZ,
    #endif
} DEMOD_EN_TER_BW_MODE;

/// Define terrestrial constellation type
typedef enum
{
    DEMOD_TER_QPSK,                                                           ///< QPSK type
    DEMOD_TER_QAM16,                                                          ///< QAM 16 type
    DEMOD_TER_QAM64                                                           ///< QAM 64 type
} DEMOD_EN_TER_CONSTEL_TYPE;

/// Define terrestrial hierarchy information
typedef enum
{
    DEMOD_TER_HIE_NONE,                                                       ///< Non-hierarchy
    DEMOD_TER_HIE_ALPHA_1,                                                    ///< Hierarchy alpha = 1
    DEMOD_TER_HIE_ALPHA_2,                                                    ///< Hierarchy alpha = 2
    DEMOD_TER_HIE_ALPHA_4                                                     ///< Hierarchy alpha = 4
} DEMOD_EN_TER_HIE_TYPE;

/// Define terrestrial guard interval
typedef enum
{
    DEMOD_TER_GI_1_32,                                                        ///< Guard interval value = 1/32
    DEMOD_TER_GI_1_16,                                                        ///< Guard interval value = 1/16
    DEMOD_TER_GI_1_8,                                                         ///< Guard interval value = 1/8
    DEMOD_TER_GI_1_4                                                          ///< Guard interval value = 1/4
} DEMOD_EN_TER_GI_TYPE;

/// Define terrestrial transmission mode
typedef enum
{
    DEMOD_TER_FFT_2K,                                                         ///< 2k FFT mode
    DEMOD_TER_FFT_8K                                                          ///< 8k FFT mode
} DEMOD_EN_TER_FFT_MODE;

/// Define terrestrial transmission mode
typedef enum
{
    DEMOD_TER_HP_SEL,                                                         ///< High priority level selection
    DEMOD_TER_LP_SEL                                                          ///< Low priority level selection
} DEMOD_EN_TER_LEVEL_SEL;

/// Define DVB-C modulation scheme
typedef enum
{
    DEMOD_CAB_QAM16,                                                          ///< QAM 16
    DEMOD_CAB_QAM32,                                                          ///< QAM 32
    DEMOD_CAB_QAM64,                                                          ///< QAM 64
    DEMOD_CAB_QAM128,                                                         ///< QAM 128
    DEMOD_CAB_QAM256,
	#if (ENABLE_HUMMINGBIRD_CUSTOMER)
    DEMOD_CAB_QAMAUTO
	#endif
} DEMOD_EN_CAB_CONSTEL_TYPE;


/// Define DVB-S IQ tuning mode
typedef enum
{
    DEMOD_CAB_IQ_NORMAL,                                                      ///< Normal
    DEMOD_CAB_IQ_INVERT                                                       ///< Inverse
} DEMOD_EN_CAB_IQ_MODE;


/// Define DVB-S modulatiopn scheme
typedef enum
{
    DEMOD_SAT_QPSK,                                                           ///< QPSK
    DEMOD_SAT_8PSK,                                                           ///< 8PSK
    DEMOD_SAT_QAM16                                                           ///< QAM16
} DEMOD_EN_SAT_CONSTEL_TYPE;

/// Define DVB-S Roll-Off factor
typedef enum
{
    DEMOD_SAT_RO_35,                                                          ///< roll-off factor = 0.35
    DEMOD_SAT_RO_25,                                                          ///< roll-off factor = 0.25
    DEMOD_SAT_RO_20                                                           ///< roll-off factor = 0.20
} DEMOD_EN_SAT_ROLL_OFF_TYPE;

/// Define DVB-S IQ tuning mode
typedef enum
{
    DEMOD_SAT_IQ_NORMAL,                                                      ///< Normal
    DEMOD_SAT_IQ_INVERSE                                                      ///< Inverse
} DEMOD_EN_SAT_IQ_MODE;

/// Define Bit Error Rate range measure from signal
typedef enum
{
    DEMOD_BIT_ERR_RATIO_LOW ,                                                 ///< Low BER
    DEMOD_BIT_ERR_RATIO_MEDIUM ,                                              ///< Medium BER
    DEMOD_BIT_ERR_RATIO_HIGH                                                  ///< High BER
} DEMOD_EN_BIT_ERR_RATIO;

/// Define lock status of front end
typedef enum
{
    DEMOD_FE_UNLOCKED = 0,                                                    ///< Frontend is unlocked
    DEMOD_FE_LOCKED                                                           ///< Frontend is locked
} DEMOD_EN_FE_LOCK_STATUS;


/// Define tuning mode
/// NOTE: When this typedef is modified, the apiChScan should be rebuild.
typedef enum
{
    DEMOD_FE_TUNE_MANUAL,                                                     ///< Manual tuning to carrier
    DEMOD_FE_TUNE_AUTO,                                                       ///< Auto tuning to carrier
} DEMOD_EN_FE_TUNE_MODE;

/// Define output mode
/// NOTE: When this typedef is modified, the apiChScan should be rebuild.
typedef enum
{
    DEMOD_INTERFACE_SERIAL = 0,                                                   ///< Serial interface
    DEMOD_INTERFACE_PARALLEL                                                  ///< Parallel interface
} DEMOD_INTERFACE_MODE;

/// Define tuning paramter of DVB-T front-end
typedef struct
{
    DEMOD_EN_TER_BW_MODE                  eBandWidth;                         ///< Band width
    DEMOD_EN_TER_CONSTEL_TYPE             eConstellation;                     ///< Constellation type
    DEMOD_EN_TER_HIE_TYPE                 eHierarchy;                         ///< Hierarchy
    DEMOD_EN_TER_GI_TYPE                  eGuardInterval;                     ///< Guard interval
    DEMOD_EN_TER_FFT_MODE                 eFFT_Mode;                          ///< Transmission mode
    DEMOD_EN_CONV_CODE_RATE_TYPE          eHPCodeRate;                        ///< HP code rate
    DEMOD_EN_CONV_CODE_RATE_TYPE          eLPCodeRate;                        ///< LP code rate
    DEMOD_EN_TER_LEVEL_SEL                eLevelSel;                          ///< Select HP or LP level
} DEMOD_MS_TER_CARRIER_PARAM;

/// Define tuning paramter of DVB-C front-end
typedef struct
{
    DEMOD_EN_CAB_CONSTEL_TYPE             eConstellation;                     ///< Constellation type
    MS_U16                          u16SymbolRate;                      ///< Symbol rate (Ksym/sec)

    DEMOD_EN_CAB_IQ_MODE                  eIQMode;                            ///< IQ Mode
    MS_U8                           u8TapAssign;                        ///< Tap assign
    MS_U32                          u32FreqOffset;                      ///< Carrier frequency offset
    MS_U8                           u8TuneFreqOffset;                       ///< Requeset tuner freq offset
} DEMOD_MS_CAB_CARRIER_PARAM;

/// Define tuning paramter of DVB-S front-end
typedef struct
{
    DEMOD_EN_SAT_CONSTEL_TYPE             eConstellation;                     ///< Constellation type
    DEMOD_EN_SAT_ROLL_OFF_TYPE            eRollOff;                           ///< Roll-Off factor
    DEMOD_EN_SAT_IQ_MODE                  eIQ_Mode;                           ///< IQ mode
    DEMOD_EN_CONV_CODE_RATE_TYPE          eCodeRate;                          ///< Converlution code rate
    MS_U32                                u32SymbolRate;
	#if (ENABLE_HUMMINGBIRD_CUSTOMER)
    MS_U8                            	  u8Polarity; // 0: Horizon; > 0(default 1): Vertical;
	#endif
} DEMOD_MS_SAT_CARRIER_PARAM;

/// Define carrier paramter of digital tuner
/// NOTE: When this typedef is modified, the apiChScan should be rebuild.
typedef struct
{
    MS_U32                          u32Frequency;                       ///< RF frequency
    #if ((FRONTEND_DEMOD_TYPE == MSTAR_MSB123x_DEMOD) && (FRONTEND_TUNER_TYPE == TUNER_MXL201RF))
    MS_U8                           u8PlpID;
    MS_U8                           u8BandWidth;
    #endif

    union
    {
        DEMOD_MS_TER_CARRIER_PARAM        TerParam;                           ///< Paramters for DVB-T front-end
        DEMOD_MS_CAB_CARRIER_PARAM        CabParam;                           ///< Paramters for DVB-C front-end
        DEMOD_MS_SAT_CARRIER_PARAM        SatParam;                           ///< Paramters for DVB-S front-end
    };
} DEMOD_MS_FE_CARRIER_PARAM;

/// Define the quality report
typedef struct
{
    DEMOD_EN_FE_LOCK_STATUS               eLock;
    DEMOD_EN_BIT_ERR_RATIO                eBER;                               ///< Bit error rate
    MS_U8                           u8SNR;
    MS_U8                           u8Signal;
} DEMOD_MS_FE_CARRIER_STATUS;

/// Define the carrier information
typedef struct
{
    DEMOD_MS_FE_CARRIER_PARAM             Param;                              ///< Carrier parameter
    DEMOD_MS_FE_CARRIER_STATUS            Status;                             ///< Quality report
} DEMOD_MS_FE_CARRIER_INFO;

typedef struct
{
    // Demodulator option
    MS_BOOL                         bX4CFE_en;                          ///< Carrier frequency estimation
    MS_BOOL                         bPPD_en;                            ///< Tap assign estimation
    MS_BOOL                         bIQAutoSwap_en;                     ///< IQ mode auto swap
    MS_BOOL                         bQAMScan_en;                        ///< QAM type auto scan
    MS_BOOL                         bFHO_en;                            ///< FHO
    MS_BOOL                         (*fptTunerSet)(MS_U32);             ///< Tuner set freq function pointer
// } DMD_Mode;
} Demod_Mode;

MS_BOOL MDrv_Demod_Init(void);
MS_BOOL MDrv_Demod_Open(void);
MS_BOOL MDrv_Demod_Close(void);
MS_BOOL MDrv_Demod_Reset(void);
MS_BOOL MDrv_Demod_TsOut(MS_BOOL bEnable);
MS_BOOL MDrv_Demod_PowerOnOff(MS_BOOL bPowerOn);
MS_BOOL MDrv_Demod_SetBW(MS_U32 u32BW);
MS_BOOL MDrv_Demod_GetBW(MS_U32 *pu32BW);
MS_BOOL MDrv_Demod_GetLock(MS_BOOL *pbLock);
MS_BOOL MDrv_Demod_GetSNR(MS_U32 *pu32SNR);
MS_BOOL MDrv_Demod_GetBER(float *pfBER);
MS_BOOL MDrv_Demod_GetPWR(MS_S32 *ps32Signal);
MS_BOOL MDrv_Demod_Config(MS_U8 *pRegParam);

MS_BOOL MDrv_Demod_GetParam(DEMOD_MS_FE_CARRIER_PARAM* pParam); // MS_BOOL MDrv_Demod_GetParam(DMD_Param *pParam);
MS_BOOL MDrv_Demod_Restart(DEMOD_MS_FE_CARRIER_PARAM* pParam); // MS_BOOL MDrv_Demod_Restart(DMD_Param *pParam);
MS_BOOL MDrv_Demod_SetMode(Demod_Mode* pMode); // MS_BOOL MDrv_Demod_SetMode(DMD_Mode *pMode);

MS_BOOL MDrv_Demod_SetOutoutPath(DEMOD_INTERFACE_MODE path);
DEMOD_INTERFACE_MODE MDrv_Demod_GetOutoutPath(void);
#if (FRONTEND_DEMOD_TYPE == DEMOD_MSB122x)
MS_BOOL MDrv_Demod_I2C_ByPass(MS_BOOL bOn);
MS_BOOL MDrv_Demod_SetTsSerial(MS_BOOL bSerial);
#endif
#if (ENABLE_HUMMINGBIRD_CUSTOMER)
#if (FRONTEND_DEMOD_TYPE == DEMOD_AVL6211)
MS_BOOL MDrv_Demod_SetTsSerial(MS_BOOL bSerial);
MS_BOOL MDrv_Demod_TPSGetLock(MS_BOOL *pbLock);
MS_BOOL MDrv_Demod_MPEGGetLock(MS_BOOL *pbLock);
MS_BOOL MDrv_Demod_GetRollOff(MS_U8 *pRollOff);
MS_BOOL MDrv_Demod_GetRFOffset(MS_S16 *ps16RFOff);
MS_BOOL MDrv_Demod_BlindScan_Start(MS_U16 u16StartFreq,MS_U16 u16EndFreq);
MS_BOOL MDrv_Demod_BlindScan_NextFreq(MS_BOOL* bBlindScanEnd);
MS_BOOL MDrv_Demod_BlindScan_WaitCurFreqFinished(MS_U8* u8Progress,MS_U8 *u8FindNum);
MS_BOOL MDrv_Demod_BlindScan_Cancel(void);
MS_BOOL MDrv_Demod_BlindScan_End(void);
MS_BOOL MDrv_Demod_BlindScan_GetChannel(MS_U16 u16ReadStart,MS_U16* u16TPNum,DEMOD_MS_FE_CARRIER_PARAM *pTable);
MS_BOOL MDrv_Demod_BlindScan_GetCurrentFreq(MS_U32 *u32CurrentFeq);
MS_BOOL MDrv_DiSEqC_SetTone(MS_BOOL bTone1);
MS_BOOL MDrv_DiSEqC_SetLNBOut(MS_BOOL bLow);
MS_BOOL MDrv_DiSEqC_GetLNBOut(MS_BOOL* bLNBOutLow);
MS_BOOL MDrv_DiSEqC_Set22kOnOff(MS_BOOL b22kOn);
MS_BOOL MDrv_DiSEqC_Get22kOnOff(MS_BOOL* b22kOn);
MS_BOOL MDrv_DiSEqC_SendCmd(MS_U8* pCmd,MS_U8 u8CmdSize);
#endif
#endif // end ENABLE_HUMMINGBIRD_CUSTOMER
#endif // #define __DRV_DEMOD_H__
