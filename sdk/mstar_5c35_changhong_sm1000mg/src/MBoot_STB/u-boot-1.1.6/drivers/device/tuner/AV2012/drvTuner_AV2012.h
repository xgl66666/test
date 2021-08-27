#ifndef _DRVTUNER_AV2012_H_
#define _DRVTUNER_AV2012_H_

extern MS_BOOL MDrv_Tuner_SetFreq(MS_U16 u16CenterFreq, MS_U32 u32SymbolRate_Hz);
extern MS_BOOL MDrv_Tuner_Initial(void);
extern MS_BOOL MDrv_Tuner_CheckLock(void);

#endif
