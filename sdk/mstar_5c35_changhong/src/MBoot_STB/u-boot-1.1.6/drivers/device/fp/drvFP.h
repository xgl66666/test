#ifndef _DRVKEYPAD_H
#define _DRVKEYPAD_H

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2010-2015 MStar Semiconductor, Inc.
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
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   drvFrontPnl.h
/// @brief  FrontPnl driver
/// @author MStar Semiconductor Inc.
///
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// Inlucde Fils
///
///////////////////////////////////////////////////////////////////////////////////////////////////


MS_U8 MDrv_FP_Init(void);
MS_U8 MDrv_FP_GetKey(void);
void MDrv_FP_SetDigital(char ch, MS_U8 index);
void MDrv_FP_EnableDot(U8 isEnabled, U8 index);
void MDrv_FP_DispUpdate(void);
void MDrv_FP_ClearDispBuf(void);
void MDrv_FP_WriteAddrRam(MS_U8 addr, MS_U8 value);



#endif

