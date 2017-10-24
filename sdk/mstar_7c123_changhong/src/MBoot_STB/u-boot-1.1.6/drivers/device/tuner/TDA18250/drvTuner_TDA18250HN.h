////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2007 MStar Semiconductor, Inc.
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

//////////////////////////////////////////////////////////////////////////////
///
/// file    drvTuner_TDA18250HN.c
/// @brief  PHILIPS CD1616 DVBC Tuner Driver Interface
/// @author MStar Semiconductor Inc.
//////////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#ifndef _DRVTUNER_NUTUNE_FJ2207_H_
#define _DRVTUNER_NUTUNE_FJ2207_H_
#include <sys/common/MsTypes.h>

#define INTERFACE extern

INTERFACE MS_BOOL MDrv_Tuner_SetFreq(MS_U32 u32Freq);
INTERFACE MS_BOOL MDrv_Tuner_Init(void);
INTERFACE MS_BOOL MDrv_Tuner_PowerOnOff(MS_BOOL bPowerOn);

#undef INTERFACE
#endif

