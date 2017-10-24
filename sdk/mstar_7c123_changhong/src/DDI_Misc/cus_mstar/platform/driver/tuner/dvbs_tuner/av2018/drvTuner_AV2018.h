#ifndef _DRVTUNER_AV2018_H_
#define _DRVTUNER_AV2018_H_

#define VHF_LOWMIN_FREQ        48250L    // KHz
#define VHF_LOWMAX_FREQ        158000L    // KHz
#define VHF_HIGHMIN_FREQ    160000L    // KHz
#define VHF_HIGHMAX_FREQ    442000L    // KHz
#define UHF_MIN_FREQ_UK        470000L//470000L    // KHz for UK
#define UHF_MIN_FREQ        442000L    // KHz
#define UHF_MAX_FREQ        863250L    // KHz

#define TUNER_IF_FREQ_KHz            38000L    // KHz
#define TUNER_L_PRIME_IF_FREQ_KHz    33950L    // KHz

#define MAX_INPUT_FREQ 2150 //Mhz
#define MIN_INPUT_FREQ 950  //Mhz

/// Define DVB-S Roll-Off factor
typedef enum
{
    SAT_RO_35, ///< roll-off factor = 0.35
    SAT_RO_25, ///< roll-off factor = 0.25
    SAT_RO_20, ///< roll-off factor = 0.20
    E_SAT_RO_35 = SAT_RO_35,
    E_SAT_RO_25 = SAT_RO_25,
    E_SAT_RO_20 = SAT_RO_20,
} EN_SAT_ROLL_OFF_TYPE;


MS_BOOL MDrv_Tuner_SetFreq(MS_U16 u16CenterFreq, MS_U32 u32SymbolRate_Hz);
MS_BOOL MDrv_Tuner_Initial(void);
MS_BOOL MDrv_Tuner_CheckLock(void);
void MDrv_Tuner_SetRolloff(MS_U8 u8Rolloff);

#if 0
extern void devTunerInit(void);
extern void devTunerSetFreqStep(FREQSTEP eFreqStep);
extern void devTunerSetPLLData(WORD wPLLData, RFBAND eBand);
#endif

#endif
