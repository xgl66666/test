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
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __COFDM_DEMODULATOR_H__
#define __COFDM_DEMODULATOR_H__

//#include "debug.h"
//#include <common.h>
#include <configs/cmd_config.h>
#include <sys/common/MsTypes.h>
#include <device/Tuner.h>
#if (ENABLE_HUMMINGBIRD_CUSTOMER)
#include <frontend_config.h>
#endif
//#include "msAPI_Global.h"
#if ((CONFIG_COMMANDS&(CFG_CMD_TUNER | CFG_CMD_DEMOD)) == (CFG_CMD_TUNER | CFG_CMD_DEMOD))

#if 0
/// Data type unsigned long
#ifndef DWORD
#undef DWORD
typedef unsigned long DWORD;
#endif

/// Data type unsigned int
#ifndef WORD
#undef WORD
typedef unsigned short WORD;    // 16bit
#endif
#endif
// Add for MSB122x from supernova
/*
typedef enum
{
    CMD_SYSTEM_INIT = 0,
    CMD_DAC_CALI,
    CMD_DVBT_CONFIG,
    CMD_DVBC_CONFIG,
    CMD_VIF_CTRL,
    CMD_FSM_CTRL,
    CMD_INDIR_RREG,
    CMD_INDIR_WREG,
    CMD_GET_INFO,
    CMD_TS_CTRL,
    CMD_TUNED_VALUE,
    CMD_MAX_NUM
}E_CMD_CODE;


typedef enum
{
    E_SYS_UNKOWN = -1,
    E_SYS_DVBT,
    E_SYS_DVBC,
    E_SYS_ATSC,
    E_SYS_VIF,
    E_SYS_NUM
}E_SYSTEM;
*/
typedef enum
{
    E_SERIAL_INTERFACE,
    E_PARALLEL_INTERFACE
} TS_INTERFACE;

#if ((FRONTEND_DEMOD_TYPE == MSTAR_MSB123x_DEMOD) && (FRONTEND_TUNER_TYPE == TUNER_MXL201RF))
#include <device/MSB123x_TUNER_MXL201RF.h>
#elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB123x_DEMOD)
#include <device/MSB123x.h>
#elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB1236C_DEMOD)
#include <device/MSB1236C.h>
#elif (FRONTEND_DEMOD_TYPE == DIBCOM_DIB8096_DEMOD)
#include <device/DIB8096.h>
#elif (FRONTEND_DEMOD_TYPE == EMBEDDED_DVBC_DEMOD)
#include <device/Uranus4_Internal.h>
#elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB122x_DEMOD)
#include <device/MSB122x.h>
#elif (FRONTEND_DEMOD_TYPE == LGDT3305_DEMOD)
#include <device/lgdt3305.h>
#elif ((FRONTEND_DEMOD_TYPE == EMBEDDED_DVBT51_DEMOD)||(FRONTEND_DEMOD_TYPE == EMBEDDED_DVBC51_DEMOD))
#include "device/Kronus_Internal.h"
#elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB1400_DEMOD)

#if (ENABLE_HUMMINGBIRD_CUSTOMER)
#include "device/device_demodulator_msb1400.h"
#endif

#elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB131X_DEMOD)

#if (ENABLE_HUMMINGBIRD_CUSTOMER)
#include "device/MSB131x.h"
#endif //end ENABLE_HUMMINGBIRD_CUSTOMER

//#elif (FRONTEND_DEMOD_TYPE == TUNER_MXL603)
//#include "device/MxL603.h"
#endif



//******************************************************************************
// Functions prototype
//******************************************************************************

void devCOFDM_Init(void);
void devCOFDM_ControlPowerOnOff(BOOLEAN bPowerOn);
FUNCTION_RESULT devCOFDM_PowerSave(void);
void devCOFDM_SetFreqStep(FREQSTEP eFreqStep);
void devCOFDM_SetFrequency(DWORD dwFrequency, RF_CHANNEL_BANDWIDTH eBandWidth, RF_CHANNEL_HP_LP eHpLp, U8 u8PlpID);
BOOLEAN devCOFDM_GetLockStatus(COFDM_LOCK_STATUS eStatus);
BYTE devCOFDM_GetSignalToNoiseRatio(void);
void devCOFDM_PassThroughI2C(BOOLEAN bEnable);
BOOLEAN devCOFDM_PassThroughI2C_WriteBytes(U8 u8SlaveID, U8 u8AddrNum, U8* paddr, U16 u16size, U8* pu8data);
BOOLEAN devCOFDM_PassThroughI2C_ReadBytes(U8 u8SlaveID, U8 u8AddrNum, U8* paddr, U16 u16size, U8* pu8data);
#if (FRONTEND_DEMOD_TYPE == ZARLINK_CE6355_DEMOD)
void devCOFDM_SwitchTransportSteamInferface(TS_INTERFACE interface, RF_CHANNEL_BANDWIDTH BandWidth);
#else
void devCOFDM_SwitchTransportSteamInferface(TS_INTERFACE interface);
#endif
#if DTV_SCAN_AUTO_FINE_TUNE_ENABLE
BOOLEAN devCOFDM_GetFrequencyOffset(float *pFreqOff, RF_CHANNEL_BANDWIDTH u8BW);
BOOLEAN devCOFDM_SetAfcEnable(BOOLEAN bEnable);
#endif
FUNCTION_RESULT devCOFDM_GetSignal_Vit( DWORD *vit );
FUNCTION_RESULT devCOFDM_GetSignalStrength(WORD *strength);
BOOLEAN devCOFDM_Get_CELL_ID(WORD *wCell_id);
FUNCTION_RESULT devCOFDM_GetSignalQuality(WORD *quality);
BOOLEAN devCOFDM_GetBER(float *ber);
BOOLEAN devCOFDM_Get_Modul_Mode(WORD *Modul_Mode);
BOOLEAN devCOFDM_Is_Hierarchy_On(void);
void devCOFDM_TunerProgramming(U8 *cTunerData);

#if (FRONTEND_DEMOD_TYPE == MSTAR_MSB1200_DEMOD || FRONTEND_DEMOD_TYPE == MSTAR_MSB1210_DEMOD || FRONTEND_DEMOD_TYPE == EMBEDDED_DVBT_DEMOD || FRONTEND_DEMOD_TYPE == TOSHIBA_TC90517FG_DEMOD)
U16 devCOFDM_ReadReg(U16 RegAddr);
BOOLEAN devCOFDM_WriteReg(U16 RegAddr, U16 RegData);
#endif
#if (FRONTEND_DEMOD_TYPE == MSTAR_MSB123x_DEMOD)||(FRONTEND_DEMOD_TYPE == MSTAR_MSB122x_DEMOD)||(FRONTEND_DEMOD_TYPE == MSTAR_MSB1236C_DEMOD)
BOOLEAN devCOFDM_SetDemodType(EN_DEVICE_DEMOD_TYPE enDemodType);
EN_DEVICE_DEMOD_TYPE devCOFDM_GetDemodType(void);
#elif (FRONTEND_DEMOD_TYPE == DIBCOM_DIB8096_DEMOD || FRONTEND_DEMOD_TYPE == EMBEDDED_DVBC_DEMOD)
#define devCOFDM_SetDemodType __noop
#define devCOFDM_GetDemodType __noop
#endif
#if ((FRONTEND_DEMOD_TYPE == EMBEDDED_DVBC51_DEMOD )||(FRONTEND_DEMOD_TYPE == EMBEDDED_DVBT51_DEMOD ))
#else
//void MDrv_Tuner_Init(void);
#endif
#endif
#endif // __COFDM_DEMODULATOR_H__

