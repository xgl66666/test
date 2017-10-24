////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2008 MStar Semiconductor, Inc.
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
#ifndef _DRV_TUNER_H_
#define _DRV_TUNER_H_


//--------------------------------------------------------------------


/*@ <Definitions> @*/

/*@ </Definitions> @*/

#if (!ENABLE_HUMMINGBIRD_CUSTOMER)
typedef enum
{
    BW_MODE_6MHZ = 0,                                                   ///< 6 MHz
    BW_MODE_7MHZ,                                                       ///< 7 MHz
    BW_MODE_8MHZ                                                        ///< 8 MHz
} EN_TER_BW_MODE;
#endif

typedef enum
{
	COFDM_FEC_LOCK,
	COFDM_PSYNC_LOCK,
	COFDM_TPS_LOCK,
	COFDM_DCR_LOCK,
	COFDM_AGC_LOCK,
	COFDM_MODE_DET,

} COFDM_LOCK_STATUS;

//-------------------------------------------------------------------------------------------------
//  Function Prototype
//-------------------------------------------------------------------------------------------------
extern MS_BOOL MDrv_Tuner_Init(void);
extern MS_BOOL MDrv_Tuner_PowerOnOff(MS_BOOL bPowerOn);
extern MS_BOOL MDrv_Tuner_SetFreq(MS_U32 u32Freq);
extern MS_BOOL MDrv_Tuner_GetLock(void);
extern MS_U32 MDrv_Tuner_SetTuner(MS_U32 dwFreq /*Khz*/, MS_U8 ucBw /*MHz*/);
//extern void devDigitalTuner_SetFreq ( double Freq, RF_CHANNEL_BANDWIDTH eBandWidth);
//--------------------------------------------------------------------
#endif
//--------------------------------------------------------------------
