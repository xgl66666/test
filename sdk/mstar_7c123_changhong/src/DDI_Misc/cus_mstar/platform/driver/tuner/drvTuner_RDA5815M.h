#ifndef _DRVTUNER_RDA5815M_H_
#define _DRVTUNER_RDA5815M_H_
#define MAX_INPUT_FREQ 2150 //Mhz
#define MIN_INPUT_FREQ 950  //Mhz
MS_BOOL MDrv_Tuner_RDA5815M_SetFreq_S2(MS_U8 u8TunerIndex, MS_U32 u16CenterFreq, MS_U32 u32SymbolRate_Hz);
MS_BOOL  MDrv_Tuner_RDA5815M_Initial(MS_U8 u8TunerIndex,TUNER_MS_INIT_PARAM* pParam);
#endif
