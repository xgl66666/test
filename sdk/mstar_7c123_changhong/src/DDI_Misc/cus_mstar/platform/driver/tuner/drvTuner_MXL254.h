#ifndef  _MXL254_H_
#define _MXL254_H_
#include "MxL_HRCLS_CommonApi.h"
#include "MxL_HRCLS_OEM_Drv.h"
#include "MxL_HRCLS_SOC_Registers.h"
#include "MxL_HRCLS_PhyCtrl.h"

DRV_TUNER_TABLE_TYPE GET_TUNER_ENTRY_NODE(TUNER_MXL254) DDI_DRV_TUNER_TABLE_ENTRY(tunertab);
DRV_DEMOD_TABLE_TYPE GET_DEMOD_ENTRY_NODE(DEMOD_MXL254) DDI_DRV_TABLE_ENTRY(demodtab);


//----------------------------------------------------------//
//                   Define                                            //
//----------------------------------------------------------//
#define EXAMPLE_DEV_MAX   2
#define MXL254_I2C_ADDR   0x50 //0x50 0x51 0x52 0x53 depend on HW setting
#define MAX_CHANNEL_COUNT 4
#define MxL254_4_WIRE_MODE 0
#define MxL254_FREQ_SEARCH_RANGE 800UL //kHz

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
#ifdef __KERNEL__
MS_BOOL MXL254_Demod_GetSNR(MS_U8 u8DemodIndex, MS_S32 *ps32SNR);
MS_BOOL MXL254_Demod_GetBER(MS_U8 u8DemodIndex, MS_S32 *ps32BER);
#else
MS_BOOL MXL254_Demod_GetSNR(MS_U8 u8DemodIndex, float *pfSNR);
MS_BOOL MXL254_Demod_GetBER(MS_U8 u8DemodIndex, float *pfBER);
#endif
MS_BOOL MXL254_Demod_GetPWR(MS_U8 u8DemodIndex, MS_S32 *ps32Signal);
MS_BOOL MXL254_Demod_Restart(MS_U8 u8DemodIndex, DEMOD_MS_FE_CARRIER_PARAM *pParam, MS_U32 u32BroadCastType);
#endif
