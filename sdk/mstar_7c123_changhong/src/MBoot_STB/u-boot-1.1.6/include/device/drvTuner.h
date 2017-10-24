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

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   drvTuner.h
/// @brief  Tuner Common Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DRV_TUNER_H_
#define _DRV_TUNER_H_


//-------------------------------------------------------------------------------------------------
//  Driver Capability
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
#if (ENABLE_HUMMINGBIRD_CUSTOMER)
typedef enum
{
     E_TUNE_TYPE_T =0,
     E_TUNE_TYPE_T2,
     E_TUNE_TYPE_ISDBT,
}EN_TUNE_TYPE;
typedef enum _R828_SetFreq_Type
{
    FAST_MODE,
    TUNER_NORMAL_MODE,
}R828_SetFreq_Type;

typedef struct FEMode
{
     EN_TUNE_TYPE          Fetype;
     R828_SetFreq_Type    Femode;
}FEMode;

#endif //end ENABLE_HUMMINGBIRD_CUSTOMER
//-------------------------------------------------------------------------------------------------
//  Function Prototype
//-------------------------------------------------------------------------------------------------

MS_BOOL MDrv_Tuner_Init(void);
MS_BOOL MDrv_Tuner_PowerOnOff(MS_BOOL bPowerOn);
#if (ENABLE_HUMMINGBIRD_CUSTOMER)
#if ENABLE_DVB_C_SYS

MS_BOOL MDrv_Tuner_SetFreq(MS_U32 u32Freq);
#endif
#else
MS_BOOL MDrv_Tuner_SetFreq(MS_U32 u32Freq);
#endif //end ENABLE_HUMMINGBIRD_CUSTOMER
MS_BOOL MDrv_Tuner_GetLock(void);
MS_U32 MDrv_Tuner_SetTuner(MS_U32 dwFreq /*Khz*/, MS_U8 ucBw /*MHz*/);


#endif // _DRV_TUNER_H_

