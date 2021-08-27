////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2009 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (¡§MStar Confidential Information¡¨) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStaR Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef MSAPI_TUNER_H
#define MSAPI_TUNER_H

//#include "debug.h"
#include <common.h>
#include <device/Tuner.h>
//#include "msAPI_Global.h"
#include <sys/api/apiDMX.h>
#if ((CONFIG_COMMANDS&(CFG_CMD_TUNER | CFG_CMD_DEMOD)) == (CFG_CMD_TUNER | CFG_CMD_DEMOD))

/******************************************************************************/
/*                 Macro                                                      */
/******************************************************************************/
#define FE_NOT_LOCK     0
#define FE_LOCK         1
#define FE_AGC_NOT_LOCK 2

typedef enum
{
    DEMOD_MODE_PAL,
    DEMOD_MODE_DVB,
    DEMOD_MODE_NUM
} EN_DEMOD_MODE;


typedef enum
{
    PAL_SCAN,
    DVB_SCAN,
} EN_SCAN_MODE;

typedef enum
{
    DEMOD_SPECTRUM_NORMAL,
    DEMOD_SPECTRUM_INVERTER,
    DEMOD_SPECTRUM_AUTO
} EN_DEMOD_SPECTRUM_MODE;

typedef enum
{
    SIGNAL_NO = 0,          // little or no input power detected
    SIGNAL_WEAK,            // some power detected.
    SIGNAL_MODERATE,        // lock achieved, SNR < 15 dB (approx)
    SIGNAL_STRONG,          // lock achieved, SNR < 24 dB (approx)
    SIGNAL_VERY_STRONG,     // lock achieved, SNR > 24 dB (approx)
} EN_SIGNAL_CONDITION;

typedef enum
{
    STATE_VSB_CHECK_INIT,
    STATE_VSB_CHECK_CR_LOCK,
    STATE_VSB_CHECK_WAIT_INIT,
    STATE_VSB_CHECK_SYNC_LOCK,
    STATE_VSB_CHECK_SNR,
} EN_VSB_CHECK_LOCK_STATE;

typedef enum
{
    STATE_QAM_CHECK_INIT,
    STATE_QAM_CHECK_WAIT_INIT,
    STATE_QAM_CHANGE_MODE,
    STATE_QAM_CHECK_FEC_LOCK,
    STATE_QAM_CHECK_MPEG_LOCK,
} EN_QAM_CHECK_LOCK_STATE;

typedef enum
{
    STATE_VSB_STABLE_INIT,
    STATE_VSB_STABLE_WAIT_INIT,
    STATE_VSB_STABLE_SYNC_LOCK,
} EN_VSB_CHECK_STABLE_STATE;
typedef enum
{
    EN_DVB_T_TYPE=0,
    EN_DVB_C_TYPE
} EN_DVB_TYPE;
/// TP Setting
typedef struct
{
    U32 u32Frequency; ///< Frequency, 50,000 ~ 860,000 Khz
    RF_CHANNEL_BANDWIDTH enBandWidth;   ///< Bandwidth
    U8 u8PLPID;
    U8 u8HpLp;
#if DVB_C_ENABLE
    U8 u8Modulation;
    U32 u32Symbol_rate;
    BOOLEAN bAutoSRFlag;
    BOOLEAN bAutoQamFlag;
#endif
} MS_TP_SETTING;

#if 1//(FRONTEND_DEMOD_TYPE == MSTAR_MSB123x_DEMOD)
typedef enum
{
    E_DEVICE_DEMOD_ATV = 0,
    E_DEVICE_DEMOD_DVB_T,
    E_DEVICE_DEMOD_DVB_C,
    E_DEVICE_DEMOD_DVB_S,
    E_DEVICE_DEMOD_DTMB,
    E_DEVICE_DEMOD_ATSC,
    E_DEVICE_DEMOD_ATSC_VSB,
    E_DEVICE_DEMOD_ATSC_QPSK,
    E_DEVICE_DEMOD_ATSC_16QAM,
    E_DEVICE_DEMOD_ATSC_64QAM,
    E_DEVICE_DEMOD_ATSC_256QAM,
    E_DEVICE_DEMOD_DVB_T2,
    E_DEVICE_DEMOD_MAX,
    E_DEVICE_DEMOD_NULL = E_DEVICE_DEMOD_MAX,

} EN_DEVICE_DEMOD_TYPE;

#endif


#ifdef MSAPI_TUNER_C
#define INTERFACE
#else
#define INTERFACE extern
#endif

INTERFACE void msAPI_Tuner_InintCurrentTPSetting(void);
INTERFACE void msAPI_Tuner_Initialization(BOOL bSrcChg);
INTERFACE void msAPI_Tuner_InitExternDemod(void);
INTERFACE void msAPI_Tuner_ZigZagScan(void);
INTERFACE void msAPI_Tuner_Tune2RfCh(MS_TP_SETTING *pstTPSetting);
INTERFACE BOOLEAN msAPI_Tuner_IsSameRfChannel(MS_TP_SETTING *pstTPSetting);
INTERFACE void msAPI_Tuner_PowerOnOff(BOOLEAN bPower);
INTERFACE U8 msAPI_Tuner_GetSingalSNRPercentage(void);
INTERFACE WORD msAPI_Tuner_GetSignalQualityPercentage(void);
INTERFACE BOOLEAN msAPI_Tuner_GetBER(float *ber);
INTERFACE BOOLEAN msAPI_Tuner_GetSignalModulMode(WORD *wModul_Mode);
INTERFACE void msAPI_Check_Lock_State_Init(void);
INTERFACE BOOLEAN msAPI_Tuner_CheckLock(BOOLEAN *CheckLockResult,BOOLEAN fScan);
INTERFACE void msAPI_Demodulator_Exit(void);
INTERFACE BOOLEAN msAPI_Demodulator_Reset(void);
INTERFACE void msAPI_Tuner_Serial_Control(BOOLEAN bParallelMode);
INTERFACE FUNCTION_RESULT msAPI_Tuner_CheckSignalStrength(WORD *wStrength);
INTERFACE BOOLEAN msAPI_Tuner_Get_CELL_ID( WORD * cell_id);
INTERFACE BOOLEAN msAPI_Tuner_Get_PLP_ID( U8 * pu8plp_id);
INTERFACE U8 msAPI_Tuner_Get_HpLp( void);
INTERFACE BOOLEAN msAPI_Tuner_Is_HierarchyOn(void);
INTERFACE FUNCTION_RESULT msAPI_Tuner_GetCurTPSetting(MS_TP_SETTING *pstCurTPSetting);



INTERFACE void msAPI_Tuner_SwitchSource(EN_DVB_TYPE etype, BOOLEAN bInit);
INTERFACE EN_DVB_TYPE msAPI_Tuner_GetSourceType(void);
INTERFACE void msAPI_Tuner_SetAntenna(BOOLEAN bIsCATV);
INTERFACE DMX_FILTER_STATUS msAPI_Tuner_SetByPassMode(BOOLEAN bByPass, BOOLEAN bPVRMode);
INTERFACE BOOLEAN msAPI_Tuner_IsParallelMode(void);
INTERFACE BOOLEAN msAPI_Tuner_IsByPassMode(void);
INTERFACE U8 msAPI_Tuner_GetDspStatus(void);
#if (DVB_C_ENABLE)
INTERFACE void msAPI_Tuner_UpdateTPSetting(void);
#endif
#if (FRONTEND_DEMOD_TYPE == MSTAR_MSB123x_DEMOD)||(FRONTEND_DEMOD_TYPE == MSTAR_MSB122x_DEMOD)||(FRONTEND_DEMOD_TYPE == MSTAR_MSB1236C_DEMOD)
INTERFACE void msAPI_GetPlpBitMap(U8* u8PlpBitMap);
INTERFACE BOOLEAN msAPI_GetPlpGroupID(U8 u8PlpID, U8* u8GroupID);
INTERFACE BOOLEAN msAPI_SetPlpGroupID(U32 u32PlpID, U32 u32GroupID);
INTERFACE BOOLEAN msAPI_SetDemodType(EN_DEVICE_DEMOD_TYPE enDemodType);
INTERFACE EN_DEVICE_DEMOD_TYPE msAPI_GetDemodType(void);
#endif
#undef INTERFACE

#define ENABLE_SCAN_ONELINE_MSG 0
#endif
#endif // MSAPI_TUNER_H

