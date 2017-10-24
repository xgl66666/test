#ifndef _NMI120_DEFINE_H
#define _NMI1h0_DEFINE_H
#include "./nmitypes.h"


uint8_t MDrv_Tuner_Init(void);
MS_U32 MDrv_Tuner_SetTuner(MS_U32 dwFreq, MS_U8 ucBw);
int NMI120_GetLockStatus(void);
int NMI120_GetRSSI(uint8_t outputChoice);
uint8_t MDrv_Tuner_PowerOnOff(uint8_t bPowerOn);
uint8_t NMI120_LoopThrough(void);

#endif


