
#ifndef __ATBM2040DRIVER_H__
#define __ATBM2040DRIVER_H__


#ifdef __cplusplus
extern "C"
{
#endif

#define ATBM_LEOG_CHIP_ID (0xAA)
#define ATBM_LEOF_CHIP_ID (0xE1) /*LeoF*/
#define ATBM_LEOE_CHIP_ID (0xE2) /*LeoE V2*/
#define ATBM_LEOD_CHIP_ID (0xCD)
#define ATBM_LEOC_CHIP_ID (0xCC)
#define ATBM_LEOB_CHIP_ID (0xF0)
#define ATBM_LEO_CHIP_ID (0x5D)

typedef struct
{
    UINT32 Low;
    UINT32 High;
} ATBM2040Data64_t;

#define ATBM2040_ARRAY_NUM(array) (sizeof(array)/sizeof(array[0]))

typedef struct
{
    UINT32 FreqStart; /*in KHz*/
    UINT32 FreqEnd; /*in KHz*/
    UINT8 NMixer;
    UINT8 NLO;
} ATBM2040NLOMixer_t;

/*list registers struct for a special module,for example PLL, and so on*/
typedef struct
{
    UINT8 BaseReg;
    UINT8 OffReg;
    UINT8 Val;
} ATBM2040Reg_t;

typedef struct
{
    UINT8 BaseReg;
    UINT8 OffReg;
    UINT8 Data[64];
    UINT8 Len;
} ATBM2040RegGroup_t;

typedef struct
{
    UINT8 BaseReg;
    UINT8 OffReg;
    UINT8 Data[256];
    UINT32 Len;
} ATBM2040Firmware_t;


typedef struct
{
    UINT32 FreqStart; /*in KHz*/
    UINT32 FreqEnd; /*in MHz*/
    UINT32 N_ADC;
} ATBM2040ADCDivider_t;

typedef struct
{
    UINT32 FreqKHz;/*center frequency in KHz*/
    UINT8 vhf_fltr_res1_sel;
    UINT8 vhf_fltr_data;
    UINT8 vhf_fltr_ref_code;
} ATBM2040VHFChIndex_t;

typedef struct
{
    UINT32 FreqMHz;/*center frequency in MHz*/
    UINT32 uhf_filt_freq;
    UINT32 uhf_fltr_gm_switch;
    UINT32 uhf_fltr_manual;
} ATBM2040UHFChIndex_t;

typedef struct
{
    UINT32 FreqMHz;/*center frequency in MHz*/
    UINT32 vhf2_filt_freq;
    UINT32 vhf2_fltr_gm_switch;
} ATBM2040VHF2ChIndex_t;


typedef struct
{
    UINT32 FreqKHz;/*center frequency in KHz*/
    UINT8 vhf_fltr_ref_code;
} ATBM2040VHF1ChIndex_t;


typedef enum
{
    ATBM2040_SAR_ADC_CLK_6MHZ = 0,
    ATBM2040_SAR_ADC_CLK_1_5MHZ,

    ATBM2040_SAR_ADC_CLK_MAX
} ATBM2040SarAdcClk_e;

typedef enum
{
    ATBM2040_IF_PATH_1 = 0,
    ATBM2040_IF_PATH_2,

    ATBM2040_IF_PATH_MAX
} ATBM2040_IF_PATH_e;

typedef enum
{
    ATBM2040_RF_BAND_VHF1 = 0,
    ATBM2040_RF_BAND_VHF2,
    ATBM2040_RF_BAND_UHF,

    ATBM2040_RF_BAND_MAX,
}ATBM2040_RF_BAND_e;

typedef struct
{
    ATBM2040InitConfig_t Config; /*user config parameters*/
    UINT8 ChipID; /*Chip ID*/
    ATBM2040_SIGNAL_MODE_e SignalMode; /* signal mode*/
    UINT32 FreqKHz;/*Channel frequency in KHz, user passed in*/
    ATBM2040_RF_BAND_e Band; /*cur RF band*/
    ATBM2040_RF_BAND_e BandBak; /* RF band bakup*/
    UINT32 BWKHz; /*Bandwidth in KHz*/
    UINT32 BWKHzBak; /*Bandwidth backup*/
    BOOL InvertSpectrum; /*TRUE:invert spectrum;FALSE:normal*/

    ATBM2040NLOMixer_t *pNLOMixer;/*current MixerDiv*/
    ATBM2040NLOMixer_t *pNLOMixerHMD;
    ATBM2040NLOMixer_t *pNLOMixerLMD;
    UINT32 HMDAdcClkHz;
    UINT32 LMDAdcClkHz;

    SINT16 RFPower; /**/
    UINT8 DSPClkSel; /*0: select OSC; 1: OSC/4*/
    UINT8 ForceChipID; /*Chip ID*/
    UINT32 IFLevelSpinner; /*Spinner of IF out level*/
    UINT32 LNAGainMD;
    SINT32 MixerIF; /*Mixer IF in Hz*/
    SINT32 MixerIFBak; /*Mixer IF backup*/
    BOOL HighMD; /*TRUE:high */
    BOOL ADCIQSwap; /*TRUE:ADC IQ Swap;FALSE:normal*/
    UINT32 PllDivMode; /*pll_mod_mmd, 0:div/2; 1:div/1*/
    ATBM2040SarAdcClk_e SarAdcClk; /*SAR ADC Clock*/
    UINT32 LowCousumMd;
    BOOL StandbyState;/*TRUE:in standby state; FALSE:wakeup*/

    BOOL Inited;/*SDK init flag*/
} ATBM2040_t;

typedef struct
{
    UINT32 StartFreqMHz;
    UINT32 EndFreqMHz;
    SINT32 RegVal[32];
} ATBM2040AgcPowerTarget_t;

#define ATBM2040ABS(x) (((x)>0)?((x)):(-(x)))
#define ATBM2040_UHF_LOW_KHZ (400000)
#define ATBM2040_VHF_CHECK(FreqKHz) (((UINT32)(FreqKHz) <= ATBM2040_UHF_LOW_KHZ)?(TRUE):(FALSE))
#define ATBM2040_MIXER_MD_DIVISION_KHZ (400000)
#define ATBM2040_VHF2_LOW_KHZ (160000)

#define ATBM2040RegListWrite(u8TunerIndex,Id,RegList,Num) for(;;){\
    UINT32 n = 0;\
    ATBM2040Reg_t *pReg = RegList;\
    for(n=0;n<Num;n++){\
        ret |= ATBM2040RegWrite(u8TunerIndex,Id,pReg->BaseReg,pReg->OffReg,pReg->Val);\
        pReg++;}\
    break;\
}

#define ATBM2040RegListWriteExt(u8TunerIndex,Id,RegList,Num, RegNewValue) for(;;){\
    UINT32 n = 0;\
    ATBM2040Reg_t *pReg = RegList;\
    for(n=0;n<Num;n++){\
        if((pReg->OffReg != RegNewValue.OffReg)||(pReg->BaseReg != RegNewValue.BaseReg)){\
        ret |= ATBM2040RegWrite(u8TunerIndex,Id,pReg->BaseReg,pReg->OffReg,pReg->Val);}\
        else{ret |= ATBM2040RegWrite(u8TunerIndex,Id,RegNewValue.BaseReg,RegNewValue.OffReg,RegNewValue.Val);}\
        pReg++;}\
    break;\
}
#define ATBM2040RegRevise(u8TunerIndex,RegList,Num,RegBase,RegOff,NewVal) for(;;){\
        UINT32 n = 0;\
        ATBM2040Reg_t *pReg = RegList;\
        for(n=0;n<Num;n++){ if((pReg->BaseReg == RegBase)&&(pReg->OffReg == RegOff)){pReg->Val=NewVal;}\
            pReg++;}\
        break;\
}

#define ATBM2040_GAIN_PHASE_COMPENSATION (0) /*1: open gain and phase compensation;0: close gain and phase compensation;*/
#define ATBM2040_FIRMWARE_LOAD (1)

#ifdef  ATBM2040_CHIP_DEBUG_OPEN
extern ATBM2040_ERROR_e ATBM2040DrvChipDebugOption(UINT8 u8TunerIndex, ATBM2040_t *pTuner);
extern ATBM2040_ERROR_e ATBM2040DrvChipDebugSwConfigDone(UINT8 u8TunerIndex, ATBM2040_t *pTuner);
extern SINT32 ATBM2040DrvChipDebugPowerTargetSet(UINT8 u8TunerIndex, ATBM2040_t *pTuner, ATBM2040Reg_t *pReg, UINT32 RegCnt);
extern ATBM2040_ERROR_e ATBM2040DrvChipDebugUHFFilterGMManaulRatioGet(UINT8 u8TunerIndex, ATBM2040_t *pTuner, double *pGmRatio, double *pManaulRatio,
        double *pRFRatio, int *pRFOffset, BOOL *pDebugOpen);
extern ATBM2040_ERROR_e ATBM2040DrvFirmwareEnable(UINT8 u8TunerIndex, ATBM2040_t *pTuner);
extern ATBM2040_ERROR_e ATBM2040DrvFirmwareDisable(UINT8 u8TunerIndex, ATBM2040_t *pTuner);

#endif

ATBM2040_ERROR_e ATBM2040RegWrite(UINT8 u8TunerIndex, UINT8 I2CAddr, UINT8 BaseReg, UINT8 OffReg, UINT8 Value);
ATBM2040_ERROR_e ATBM2040RegRead(UINT8 u8TunerIndex, UINT8 I2CAddr, UINT8 BaseReg, UINT8 OffReg, UINT8 *pValue);
ATBM2040_ERROR_e ATBM2040DrvRegLatch(UINT8 u8TunerIndex, UINT8 I2CAddr, BOOL OnOff);
void ATBM2040DrvPLLDivCal(UINT8 u8TunerIndex, ATBM2040_t *pTuner, UINT8 *pNInt, UINT32 *pNFrac, BOOL HighMD);
ATBM2040_ERROR_e ATBM2040DrvSwReset(UINT8 u8TunerIndex, ATBM2040_t *pTuner);
ATBM2040_ERROR_e ATBM2040DrvInit(UINT8 u8TunerIndex, ATBM2040_t *pTuner);
ATBM2040_ERROR_e ATBM2040DrvFreqTune(UINT8 u8TunerIndex, ATBM2040_t *pTuner);
ATBM2040_ERROR_e ATBM2040DrvCfgSet(UINT8 u8TunerIndex, ATBM2040_t *pTuner, ATBM2040CfgCMD_t *pCfg);

void ATBM2040DrvADCClkCal(UINT8 u8TunerIndex, ATBM2040_t *pTuner, UINT32 *pHLOAdcClkHz, UINT32  *pLLOAdcClkHz);
ATBM2040_ERROR_e ATBM2040DrvGetRSSI(UINT8 u8TunerIndex, UINT8 I2CAddr, SINT16 *pRSSIx16);
ATBM2040_ERROR_e ATBM2040DrvGetATVCfo(UINT8 u8TunerIndex, UINT8 I2CAddr, SINT32 *pCfoKHz);
ATBM2040_ERROR_e ATBM2040DrvStandby(UINT8 u8TunerIndex, ATBM2040_t *pTuner);
ATBM2040_ERROR_e ATBM2040DrvWakeup(UINT8 u8TunerIndex, ATBM2040_t *pTuner);
ATBM2040_ERROR_e ATBM2040DrvGetSDKVer(UINT8 u8TunerIndex, ATBM2040_t *pTuner, UINT32 *pVer);
void ATBM2040DrvIIRCoefJudgeStateSet(UINT8 u8TunerIndex, BOOL Open);
ATBM2040_ERROR_e ATBM2040DrvIIRCoefJudge(UINT8 u8TunerIndex, ATBM2040_t *pTuner);
ATBM2040_ERROR_e ATBM2040DrvRSSICalibration(UINT8 u8TunerIndex, ATBM2040_t *pTuner);
ATBM2040_ERROR_e ATBM2040DrvProcess(UINT8 u8TunerIndex, ATBM2040_t *pTuner);
ATBM2040_ERROR_e ATBM2040DrvRegDataFlushWrite(UINT8 u8TunerIndex, UINT8 SlvAddr, UINT8 BaseAddr, UINT8 OffAddr, UINT8 *pData, UINT32 Size);
ATBM2040_ERROR_e ATBM2040DrvRegDataBurstWrite(UINT8 u8TunerIndex, UINT8 SlvAddr, UINT8 *pData, UINT32 Size);
ATBM2040_ERROR_e ATBM2040DrvRxFltAndSpectrumParse(UINT8 u8TunerIndex, ATBM2040_t *pTuner);

void ATBM2040CfgPrint(UINT8 u8TunerIndex, ATBM2040_t *pTuner);


#ifdef __cplusplus
}
#endif

#endif /*__ATBM2040DRIVER_H__*/

