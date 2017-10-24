#ifndef  _MXL254_H_
#define _MXL254_H_

//----------------------------------------------------------//
//                   Define                                            //
//----------------------------------------------------------//

//----------------------------------------------------------//
//                   Internal Structure                            //
//----------------------------------------------------------//
//----------------------------------------------------------//
//                   Type Define                                    //
//----------------------------------------------------------//


//----------------------------------------------------------//
//                   R840 Public Parameter                     //
//----------------------------------------------------------//

//----------------------------------------------------------//
//                   R840 Public Function                       //
//----------------------------------------------------------//

MS_BOOL MXL254Tuner_Init(MS_U8 u8TunerIndex,TUNER_MS_INIT_PARAM* pParam);
MS_BOOL MXL254Tuner_SetTuner(MS_U8 u8TunerIndex,MS_U32 dwFreq, MS_U8 ucBw);


MS_BOOL MXL254_Demod_Init(MS_U8 u8DemodIndex,DEMOD_MS_INIT_PARAM* pParam);
MS_BOOL MXL254_Demod_GetLock(MS_U8 u8DemodIndex, EN_LOCK_STATUS *peLockStatus);
MS_BOOL MXL254_Demod_GetSNR(MS_U8 u8DemodIndex, float *pfSNR);
MS_BOOL MXL254_Demod_GetBER(MS_U8 u8DemodIndex, float *pfBER);
MS_BOOL MXL254_Demod_GetPWR(MS_U8 u8DemodIndex, MS_S32 *ps32Signal);
MS_BOOL MXL254_Demod_Restart(MS_U8 u8DemodIndex, DEMOD_MS_FE_CARRIER_PARAM *pParam, MS_U32 u32BroadCastType);
#endif
