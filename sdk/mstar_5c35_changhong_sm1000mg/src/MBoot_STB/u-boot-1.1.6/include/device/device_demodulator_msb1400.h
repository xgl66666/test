#ifndef _MSB1400_H_
#define _MSB1400_H_
typedef enum
{
    COFDM_FEC_LOCK,
    COFDM_TPS_LOCK,
    COFDM_SYMBOL_TIMING_LOCK,
    COFDM_AGC_LOCK,
    COFDM_DESCRAMBLER_LOCK,
    COFDM_BYTE_ALIGNER_LOCK
} COFDM_LOCK_STATUS;
extern void MApi_FE_Power_On_Initialization(void);
extern void MApi_FE_SetFrequency(MS_U32 u32Frequency, MS_U8 eBandWidth);
extern MS_BOOL  MApi_FE_CheckLock(void);
extern MS_U32 MApi_FE_GetSNR(void);
extern MS_U16 MApi_FE_GetSignalQuality(void);
#endif