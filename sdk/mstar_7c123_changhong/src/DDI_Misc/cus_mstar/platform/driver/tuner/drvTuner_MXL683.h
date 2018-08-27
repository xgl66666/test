#ifndef  _MXL683_H_
#define _MXL683_H_
#include "stdio.h"
#include "string.h"
#include "MxL68x_PhyCtrlApi.h"
#include "MxL68x_PhyCfg.h"
#include "MxL68x_OEM_Drv.h"

//----------------------------------------------------------//
//                   Define                                            //
//----------------------------------------------------------//

//----------------------------------------------------------//
//                   Internal Structure                            //
//----------------------------------------------------------//
//----------------------------------------------------------//
//                   Type Define                                    //
//----------------------------------------------------------//

MS_BOOL MXL683_Tuner_Init(MS_U8 u8TunerIndex,TUNER_MS_INIT_PARAM* pParam);
MS_BOOL MXL683_Tuner_SetTuner(MS_U8 u8TunerIndex,MS_U32 dwFreq /*Khz*/, MS_U8 ucBw /*MHz*/);
MS_BOOL MXL683_Demod_Init(MS_U8 u8DemodIndex,DEMOD_MS_INIT_PARAM* pParam);
MS_BOOL MXL683_Demod_GetLock(MS_U8 u8DemodIndex, EN_LOCK_STATUS *peLockStatus);
MS_BOOL MXL683_Demod_GetSNR(MS_U8 u8DemodIndex, float *pfSNR);
MS_BOOL MXL683_Demod_GetBER(MS_U8 u8DemodIndex, float *pfBER);
MS_BOOL MXL683_Demod_GetPWR(MS_U8 u8DemodIndex, MS_S32 *ps32Signal);
MS_BOOL MXL683_Demod_Restart(MS_U8 u8DemodIndex, DEMOD_MS_FE_CARRIER_PARAM *pParam, MS_U32 u32BroadCastType);
#endif
