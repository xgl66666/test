#ifndef  _MXL254_H_
#define _MXL542_H_
#include "MxLWare_HYDRA_CommonApi.h"
#include "MxLWare_HYDRA_DemodTunerApi.h"
#include "MxLWare_HYDRA_DeviceApi.h"
#include "MxLWare_HYDRA_OEM_Drv.h"
#include "MxLWare_HYDRA_PhyCtrl.h"

//----------------------------------------------------------//
//                   Define                                            //
//----------------------------------------------------------//
#define MxL542_MAX_INPUT_FREQ 2350
#define MxL542_MIN_INPUT_FREQ 250
//----------------------------------------------------------//
//                   Internal Structure                            //
//----------------------------------------------------------//
//----------------------------------------------------------//
//                   Type Define                                    //
//----------------------------------------------------------//
typedef struct
{
  MS_S16 signalLevel;
  MS_U32 frequency_inHz;
  MS_U32 ToneBW_N_inHz;//hz
  MS_U32 ToneBW_W_inHz;//hz
} TONE_DETECTION_INFO;

typedef struct
{
  TONE_DETECTION_INFO stTone_Info;
  MS_U32 u32SymbolRate;
  MXL_HYDRA_BCAST_STD_E eBCAST_STD;
} BLIND_SCAN_CHAN_INFO;


//----------------------------------------------------------//
//                   R840 Public Parameter                     //
//----------------------------------------------------------//

//----------------------------------------------------------//
//                   R840 Public Function                       //
//----------------------------------------------------------//

MS_BOOL MXL542_Tuner_Init(MS_U8 u8TunerIndex,TUNER_MS_INIT_PARAM* pParam);
MS_BOOL MXL542_Tuner_SetTuner(MS_U8 u8TunerIndex, MS_U32 u32CenterFreq, MS_U32 u32SymbolRate_Hz);
MS_BOOL MXL542_Demod_Init(MS_U8 u8DemodIndex,DEMOD_MS_INIT_PARAM* pParam);
MS_BOOL MXL542_Demod_GetLock(MS_U8 u8DemodIndex, EN_LOCK_STATUS *peLockStatus);
MS_BOOL MXL542_Demod_GetSNR(MS_U8 u8DemodIndex, float *pfSNR);
MS_BOOL MXL542_Demod_GetBER(MS_U8 u8DemodIndex, float *pfBER);
MS_BOOL MXL542_Demod_GetPWR(MS_U8 u8DemodIndex, MS_S32 *ps32Signal);
MS_BOOL MXL542_Demod_Restart(MS_U8 u8DemodIndex, DEMOD_MS_FE_CARRIER_PARAM *pParam, MS_U32 u32BroadCastType);
#endif
