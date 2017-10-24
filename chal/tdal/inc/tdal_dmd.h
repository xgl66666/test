/******************************************************************************
 *            COPYRIGHT 2004 IWEDIA TECHNOLOGIES       *
 ******************************************************************************
 *
 * MODULE NAME: TDAL_DMD
 *
 * FILE NAME: $URL: http://10.50.1.219/svncomedia/sources/chal/core/trunk/tdal/inc/tdal_dmd.h $
 *      $Rev: 4432 $
 *      $Date: 2010-12-14 09:55:14 +0100 (mar., 14 déc. 2010) $
 *
 * Description     : Front-End TDAL library primitives header.
 * Target      : All
 * Author      : Xavier RAZAVET.
 * Date   of creation   : 18 / 02 /   2004.
 *
 *****************************************************************************/

#ifndef _TDAL_DMD_H_
#define _TDAL_DMD_H_


/*******************************************************/
/*       Includes              */
/*******************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************/
/*       Defines              */
/*******************************************************/

/*******************************************************/
/*       Macros               */
/*******************************************************/

/*******************************************************/
/*       Typedefs              */
/*******************************************************/

typedef enum
{
   eTDAL_DMD_NO_ERROR = 0,
   eTDAL_DMD_BAD_DEMODULATOR,
   eTDAL_DMD_BAD_PARAMETER,
   eTDAL_DMD_DEVICE_BUSY,
   eTDAL_DMD_NOT_SCANNING,
   eTDAL_DMD_NOT_LOCKED
}tTDAL_DMD_Error;

typedef enum
{
   eTDAL_DMD_FE_0 = 0x00000000,
   eTDAL_DMD_FE_1 = 0x00000001,
   eTDAL_DMD_FE_2 = 0x00000002,
   eTDAL_DMD_FE_3 = 0x00000003,
   eTDAL_DMD_NB_MAX_FE_ID 
}tTDAL_DMD_FE;

typedef struct
{
   uint8_t       uiFENumber;
}tTDAL_DMD_Capability;

typedef enum
{
   eTDAL_DMD_TYPE_QPSK = 0,
   eTDAL_DMD_TYPE_QAM,
   eTDAL_DMD_TYPE_OFDM
}tTDAL_DMD_Type;

typedef enum
{
   eTDAL_DMD_DIGITAL,
   eTDAL_DMD_ANALOG,
   eTDAL_DMD_HYBRID
}tTDAL_DMD_SourceSupport;  

typedef struct
{
   tTDAL_DMD_Type       eType;
   tTDAL_DMD_SourceSupport   eSourceSupport;
}tTDAL_DMD_FECapability;

typedef enum
{
   eTDAL_DMD_LOCKED = 0,
   eTDAL_DMD_LOCK_FAILED,
   eTDAL_DMD_FOUND,
   eTDAL_DMD_END_OF_RANGE,
   eTDAL_DMD_SIGNAL_LOST
}tTDAL_DMD_NotifyStatus;


/*---------------------------------------*/
/*  COMMON Front-End types Declarations  */
/*---------------------------------------*/
typedef enum
{
   eTDAL_DMD_MOD_BPSK = 0,
   eTDAL_DMD_MOD_QPSK,
   eTDAL_DMD_MOD_QAM4 = eTDAL_DMD_MOD_QPSK,
   eTDAL_DMD_MOD_8PSK,
   eTDAL_DMD_MOD_QAM16,
   eTDAL_DMD_MOD_QAM32,
   eTDAL_DMD_MOD_QAM64,
   eTDAL_DMD_MOD_QAM128,
   eTDAL_DMD_MOD_QAM256,
   eTDAL_DMD_MOD_AUTO,
   eTDAL_DMD_MOD_MAX
}tTDAL_DMD_Modulation;

typedef enum
{
   eTDAL_DMD_FEC_NONE,
   eTDAL_DMD_FEC_1_2,
   eTDAL_DMD_FEC_2_3,     /* only used for VSB (US market) */
   eTDAL_DMD_FEC_3_4,
   eTDAL_DMD_FEC_4_5,     /* only used for DVB-T2 */
   eTDAL_DMD_FEC_5_6,
   eTDAL_DMD_FEC_6_7,     /* not used normally in terrestrial */
   eTDAL_DMD_FEC_7_8,
   eTDAL_DMD_FEC_8_9,     /* not used normally in satellite and terrestrial */
   eTDAL_DMD_FEC_3_5,     /* only used for DVB-T2 */
   eTDAL_DMD_FEC_AUTO,
   eTDAL_DMD_FEC_MAX
}tTDAL_DMD_Convolution;

typedef enum
{
   eTDAL_DMD_SPECTRUM_AUTO = 0,
   eTDAL_DMD_SPECTRUM_NORMAL,
   eTDAL_DMD_SPECTRUM_INVERTED,
   eTDAL_DMD_SPECTRUM_MAX
}tTDAL_DMD_Spectrum;

/*------------------------------------------*/
/*  SATTELITE Front-End types Declarations  */
/*------------------------------------------*/
typedef enum
{
   eTDAL_DMD_POLAR_HORIZONTAL,
   eTDAL_DMD_POLAR_VERTICAL,
   eTDAL_DMD_POLAR_CIRCULAR_LEFT,   /* not implemented */
   eTDAL_DMD_POLAR_CIRCULAR_RIGHT,   /* not implemented */
   eTDAL_DMD_POLAR_ALL        /* not implemented */
}tTDAL_DMD_LNB_Polarization;

typedef struct
{
   uint32_t         SymbolRateBw;
   tTDAL_DMD_Modulation    Modulation;
   tTDAL_DMD_Spectrum     SpecInv;
   tTDAL_DMD_Convolution    Convolution;
   tTDAL_DMD_LNB_Polarization  Polarization;
}tTDAL_DMD_SatTunData;

typedef enum
{
   eTDAL_DMD_CFG_SAT_INIT,
   eTDAL_DMD_CFG_SAT_OL_BAND,
   eTDAL_DMD_CFG_STAND_BY_MODE,
   eTDAL_DMD_CFG_SAT_ODU_TONE_STATE,
   eTDAL_DMD_CFG_SAT_ODU_POWER,
   eTDAL_DMD_CFG_SAT_ODU_TONE_BURST,
   eTDAL_DMD_CFG_SAT_ODU_DISEQC,
   eTDAL_DMD_CFG_SAT_OTHER
}tTDAL_DMD_SatConfigType;

typedef enum
{
   eTDAL_DMD_BANDTYPE_KU = 0,
    eTDAL_DMD_BANDTYPE_C,
    eTDAL_DMD_BANDTYPE_L
}tTDAL_DMD_BandType;

typedef enum
{
   eTDAL_DMD_LNB_POWER_13V,
   eTDAL_DMD_LNB_POWER_18V
}tTDAL_DMD_LNB_Power;

typedef struct
{
   uint32_t      SwitchFreq;     /* usually 22000Hz */
   tTDAL_DMD_LNB_Power Vertical;      /* usually 13V */
   tTDAL_DMD_LNB_Power Horizontal;     /* usually 18V */
   uint32_t*      pSymbolRateList;
}tTDAL_DMD_SatInitData;

typedef enum
{
   eTDAL_DMD_STANDBYMODE_OFF,
   eTDAL_DMD_STANDBYMODE_ON
}tTDAL_DMD_CFG_StandByMode;

typedef struct
{
   tTDAL_DMD_BandType  BandType;
   uint32_t      LowLNBFreq;
   uint32_t      HighLNBFreq;
}tTDAL_DMD_SatOlBand;

typedef enum
{
   eTDAL_SatelliteA,
   eTDAL_SatelliteB
}tTDAL_DMD_ODU_ToneBurst;

typedef   struct
{
   uint8_t  Frm;
   uint8_t  Add;
   uint8_t  Cmd;
   uint8_t  Data0;
   uint8_t  Data1;
   uint8_t  Data2;
}tTDAL_DMD_ODU_DiSEqCFrame;

typedef   struct
{
   uint8_t         NbByte;
   tTDAL_DMD_ODU_DiSEqCFrame Frame;
}tTDAL_DMD_ODU_DiSEqC;

typedef enum
{
   eTDAL_DMD_ODU_TONE_NONE,
   eTDAL_DMD_ODU_TONE_OFF,
   eTDAL_DMD_ODU_TONE_ON
}tTDAL_DMD_ODU_ToneState;


typedef enum
{
   eTDAL_DMD_ODU_POWER_NONE,
   eTDAL_DMD_ODU_POWER_13V,
   eTDAL_DMD_ODU_POWER_18V
}tTDAL_DMD_ODU_Power;

typedef struct
{
   tTDAL_DMD_SatConfigType     ConfigType;
   union
   {
    tTDAL_DMD_SatInitData    Init;
    tTDAL_DMD_SatOlBand     OlBand;
    tTDAL_DMD_CFG_StandByMode   StandByMode;
    tTDAL_DMD_ODU_ToneBurst   ToneBurst;
    tTDAL_DMD_ODU_DiSEqC    DiSEqC;
    tTDAL_DMD_ODU_ToneState   ToneState;
    tTDAL_DMD_ODU_Power     Power;
    uint32_t*         pOther;
   }uCfg;
}tTDAL_DMD_SatCfgData;

/*------------------------------------------*/
/*   CABLE Front-End types Declarations   */
/*------------------------------------------*/
typedef struct
{
   uint32_t         SymbolRateBw;
   tTDAL_DMD_Modulation    Modulation;
   tTDAL_DMD_Spectrum     SpecInv;
}tTDAL_DMD_CabTunData;

typedef enum
{
   eTDAL_DMD_CFG_CAB_INIT,
   eTDAL_DMD_CFG_CAB_OTHER
}tTDAL_DMD_CabConfigType;

typedef struct
{
   uint32_t*   pSymbolRateList;
}tTDAL_DMD_CabInitData;

typedef struct
{
   tTDAL_DMD_CabConfigType   ConfigType;
   union
   {
    tTDAL_DMD_CabInitData   Init;
    uint32_t*       pOther;
   }uCfg;
}tTDAL_DMD_CabCfgData;

/*------------------------------------------*/
/* TERRESTRIAL Front-End types Declarations */
/*------------------------------------------*/
typedef enum
{
   eTDAL_DMD_CFG_SOURCE_IS_ATV,
   eTDAL_DMD_CFG_SOURCE_IS_DTV
}tTDAL_DMD_TerSourceState;

typedef enum
{
   eTDAL_DMD_MODE_2K,
   eTDAL_DMD_MODE_8K,
   eTDAL_DMD_MODE_4K,       /* only used for DVB-T2 */
   eTDAL_DMD_MODE_1K,       /* only used for DVB-T2 */
   eTDAL_DMD_MODE_16K,      /* only used for DVB-T2 */
   eTDAL_DMD_MODE_32K,      /* only used for DVB-T2 */
   eTDAL_DMD_MODE_AUTO,
   eTDAL_DMD_MODE_MAX
}tTDAL_DMD_TerMode;

typedef enum
{
   eTDAL_DMD_GUARD_1_32,
   eTDAL_DMD_GUARD_1_16,
   eTDAL_DMD_GUARD_1_8,
   eTDAL_DMD_GUARD_1_4,
   eTDAL_DMD_GUARD_1_128,   /* only used for DVB-T2 */
   eTDAL_DMD_GUARD_19_128,  /* only used for DVB-T2 */
   eTDAL_DMD_GUARD_19_256,  /* only used for DVB-T2 */
   eTDAL_DMD_GUARD_AUTO,
   eTDAL_DMD_GUARD_MAX
}tTDAL_DMD_TerGuard;

typedef enum
{
   eTDAL_DMD_HIER_NONE,
   eTDAL_DMD_HIER_1,
   eTDAL_DMD_HIER_2,
   eTDAL_DMD_HIER_4,
   eTDAL_DMD_HIER_AUTO,
   eTDAL_DMD_HIER_MAX
}tTDAL_DMD_TerHierarchy;

typedef enum
{
   eTDAL_DMD_PRIORITY_STREAM_HIGH,
   eTDAL_DMD_PRIORITY_STREAM_LOW,
   eTDAL_DMD_PRIORITY_STREAM_MAX
}tTDAL_DMD_TerPrioStream;


typedef enum
{
   eTDAL_DMD_OFFSET_NONE,
   eTDAL_DMD_OFFSET_1_6_MORE,   /* + 1/6 Mhz */
   eTDAL_DMD_OFFSET_1_6_MINUS,   /* - 1/6 Mhz */
   eTDAL_DMD_OFFSET_2_6_MORE,   /* + 2/6 Mhz */
   eTDAL_DMD_OFFSET_2_6_MINUS,   /* - 2/6 Mhz */
   eTDAL_DMD_OFFSET_3_6_MORE,   /* + 3/6 Mhz */
   eTDAL_DMD_OFFSET_3_6_MINUS,   /* - 3/6 Mhz */
   eTDAL_DMD_OFFSET_AUTO,
   eTDAL_DMD_OFFSET_MAX
}tTDAL_DMD_TerFreqOff;



typedef struct
{
   uint32_t         SymbolRateBw;
   tTDAL_DMD_Modulation    Modulation;
   tTDAL_DMD_Spectrum     SpecInv;   
   tTDAL_DMD_TerMode      Mode;
   tTDAL_DMD_TerGuard     Guard;
   tTDAL_DMD_TerHierarchy   Hierarchy;
   tTDAL_DMD_TerPrioStream   Priority;
   tTDAL_DMD_Convolution    ConvolutionHP;
   tTDAL_DMD_Convolution    ConvolutionLP;
   tTDAL_DMD_TerFreqOff    Offset;
   int32_t         PlpID;      /* only used for DVB-T2 */
   int32_t         PlpIdx;     /* only used for DVB-T2 */
   int32_t         PlpNB;      /* only used for DVB-T2, ouput param given by TDAL_DMD_GetInfo */
}tTDAL_DMD_TerTunData;


typedef enum
{
   eTDAL_DMD_CFG_TER_INIT,
   eTDAL_DMD_CFG_TER_OTHER,
   eTDAL_DMD_CFG_TER_ANTENNA_POWER_STATUS,
   eTDAL_DMD_CFG_TER_SOURCE_TRANSITION,
   eTDAL_DMD_CFG_TER_STAND_BY_MODE
}tTDAL_DMD_TerConfigType;


typedef struct
{
   uint32_t*   pBandWidthList;
}tTDAL_DMD_TerInitData;



typedef struct
{
   tTDAL_DMD_TerConfigType     ConfigType;
   union
   {
    tTDAL_DMD_TerInitData    Init;
    uint32_t*         pOther;
    uint8_t         bAntennaPowerStatus;
    tTDAL_DMD_TerSourceState   SourceTransition;
    tTDAL_DMD_CFG_StandByMode   StandByMode;
   }uCfg;
}tTDAL_DMD_TerCfgData;


/*-------------------------------------------------*/
/* TERRESTRIAL ANALOG Front-End types Declarations */
/*-------------------------------------------------*/
typedef enum
{
   eTDAL_DMD_ANALOG_STD_PAL_BG     = 0x00000001,  /* Picture carrier : 39.875 MHz / Sound Carrier : 34.375 MHz*/
   eTDAL_DMD_ANALOG_STD_PAL_I     = 0x00000002,  /* Picture carrier : 39.875 MHz / Sound Carrier : 33.875 MHz*/
   eTDAL_DMD_ANALOG_STD_PAL_DK     = 0x00000004,  /* Picture carrier : 39.875 MHz / Sound Carrier : 33.375 MHz*/
   eTDAL_DMD_ANALOG_STD_SECAM_L    = 0x00000008,  /* Picture carrier : 39.875 MHz / Sound Carrier : 33.375 MHz*/
   eTDAL_DMD_ANALOG_STD_SECAM_L_prime  = 0x00000010,  /* Picture carrier : 33.625 MHz / Sound Carrier : 40.125 MHz*/
   eTDAL_DMD_ANALOG_STD_SECAM_BG    = 0x00000020,  /* ? */
   eTDAL_DMD_ANALOG_STD_SECAM_DK    = 0x00000040  /* ? */
}tTDAL_DMD_AnalogStandardSystem;

typedef struct
{   
   uint32_t            SymbolRateBw;   
   tTDAL_DMD_AnalogStandardSystem   eStandardSystem;      /* Current standard */
   bool              bEnableDriftMonitoring;   /* Enable/Disable the automatic drift detection */
   int32_t             lManualDriftToApply;    /* In Hz _ Not used if bEnableDriftMonitoring = true */
}tTDAL_DMD_TerAnalogTunData; 

/*---------------------------------------*/
/*  COMMON Front-End types Declarations  */
/*---------------------------------------*/
typedef union
{
   tTDAL_DMD_SatTunData    Sat;
   tTDAL_DMD_CabTunData    Cab;
   tTDAL_DMD_TerTunData    Ter;
   tTDAL_DMD_TerAnalogTunData  AnalogTer;
}tTDAL_DMD_TunData;

typedef  tTDAL_DMD_TunData   tTDAL_DMD_ScanData;

typedef struct
{   
   tTDAL_DMD_TerSourceState   eSourceState;
   uint32_t         TunFrequency;
   tTDAL_DMD_TunData      TunData;
   tTDAL_DMD_NotifyStatus   CarrierStatus;
   uint8_t         SignalQuality;
   uint32_t         SignalBER;
   uint8_t         SignalLevel;
}tTDAL_DMD_Info;

typedef union
{
   tTDAL_DMD_SatCfgData    Sat;
   tTDAL_DMD_CabCfgData    Cab;
   tTDAL_DMD_TerCfgData    Ter;
}tTDAL_DMD_ConfigData;


/*------------------------------------------*/
/*   CALL BACK Definition         */
/*------------------------------------------*/
typedef void (*tTDAL_DMD_NotifyStatusFct)   (tTDAL_DMD_FE ucFeID,
                    tTDAL_DMD_NotifyStatus  Status);

typedef uint8_t tTDAL_DMD_NotifyProgress;   /* percent value */
typedef void (*tTDAL_DMD_NotifyProgressFct) (tTDAL_DMD_FE ucFeID,
                    tTDAL_DMD_NotifyProgress  Progress);


/*******************************************************/
/*       Variables Declarations (IMPORT)    */
/*******************************************************/

/*******************************************************/
/*       Functions Declarations        */
/*******************************************************/
tTDAL_DMD_Error TDAL_DMD_Init       (void);

tTDAL_DMD_Error TDAL_DMD_Terminate     (void);

const char *   TDAL_DMD_APIRevisionGet   (void);

const char *   TDAL_DMD_PlatformRevisionGet(void);

tTDAL_DMD_Error TDAL_DMD_GetCapability   (tTDAL_DMD_Capability      *pstCapability);

tTDAL_DMD_Error TDAL_DMD_GetFECapability   (tTDAL_DMD_FE eFeID,
                    tTDAL_DMD_FECapability     *pstFECapability);

tTDAL_DMD_Error TDAL_DMD_OpenFEInstance   (tTDAL_DMD_FE         eFeID,
                    tTDAL_DMD_NotifyStatusFct   pNotifyStatusFct,
                    tTDAL_DMD_NotifyProgressFct   pNotifyProgressFct);

tTDAL_DMD_Error TDAL_DMD_CloseFEInstance   (tTDAL_DMD_FE         eFeID);

tTDAL_DMD_Error TDAL_DMD_HandleGet     (tTDAL_DMD_FE         eFeID, 
                    void            *pHandle);

tTDAL_DMD_Error TDAL_DMD_Config      (tTDAL_DMD_FE         eFeID,
                    tTDAL_DMD_ConfigData      *psCfgData);

tTDAL_DMD_Error TDAL_DMD_Tune       (tTDAL_DMD_FE         eFeID,
                    tTDAL_DMD_TunData       *psTunData,
                    uint32_t           uiFrequency);

tTDAL_DMD_Error TDAL_DMD_Unlock      (tTDAL_DMD_FE         eFeID);

tTDAL_DMD_Error TDAL_DMD_StartScan     (tTDAL_DMD_FE         eFeID,
                    tTDAL_DMD_ScanData      *psScanData,
                    uint32_t           uiStartFreq,
                    uint32_t           uiEndFreq);

tTDAL_DMD_Error TDAL_DMD_ContScan      (tTDAL_DMD_FE         eFeID);


tTDAL_DMD_Error TDAL_DMD_StopScan      (tTDAL_DMD_FE         eFeID);

tTDAL_DMD_Error TDAL_DMD_GetInfo      (tTDAL_DMD_FE         eFeID,
                    tTDAL_DMD_Info        *psInfo);

tTDAL_DMD_Error TDAL_DMD_GetTSReliability   (tTDAL_DMD_FE         eFeID,
                    tTDAL_DMD_Info        *psInfo,
                    uint8_t           *pTSReliability, 
                    uint32_t           *pQuo, 
                    uint32_t           *pRem, 
                    int32_t           *pExp);


#ifdef __cplusplus
}
#endif

#endif /*_TDAL_DMD_H_*/
