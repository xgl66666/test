#ifndef _MSB131X_DEMOD_H_
#define _MSB131X_DEMOD_H_
#include <apiDTVSatTable.h>
#include <apiDigiTuner.h>
typedef enum
{
    COFDM_FEC_LOCK,
    COFDM_TPS_LOCK,
    COFDM_SYMBOL_TIMING_LOCK,
    COFDM_AGC_LOCK,
    COFDM_DESCRAMBLER_LOCK,
    COFDM_BYTE_ALIGNER_LOCK
} COFDM_LOCK_STATUS;
extern MS_BOOL  MApi_FE_CheckLock(void);
extern MS_U16 MApi_FE_GetSignalQuality(void);
extern MS_U32 MApi_FE_GetSNR(void);
extern void MApi_FE_SetFrequency(MS_U32 u32Frequency,MS_U8 eBandWidth);
extern void MApi_FE_Power_On_Initialization(void);
extern MS_BOOL MApi_FE_SetSATParam(MS_SAT_PARAM *pSATParam);
extern MS_BOOL MApi_FE_SetFECarrierParam(MS_FE_CARRIER_PARAM *pParam);



#endif  