//<MStar Software>
//******************************************************************************
// MStar Software
// Copyright (c) 2010 - 2012 MStar Semiconductor, Inc. All rights reserved.
// All software, firmware and related documentation herein ("MStar Software") are
// intellectual property of MStar Semiconductor, Inc. ("MStar") and protected by
// law, including, but not limited to, copyright law and international treaties.
// Any use, modification, reproduction, retransmission, or republication of all 
// or part of MStar Software is expressly prohibited, unless prior written 
// permission has been granted by MStar. 
//
// By accessing, browsing and/or using MStar Software, you acknowledge that you
// have read, understood, and agree, to be bound by below terms ("Terms") and to
// comply with all applicable laws and regulations:
//
// 1. MStar shall retain any and all right, ownership and interest to MStar
//    Software and any modification/derivatives thereof.
//    No right, ownership, or interest to MStar Software and any
//    modification/derivatives thereof is transferred to you under Terms.
//
// 2. You understand that MStar Software might include, incorporate or be
//    supplied together with third party`s software and the use of MStar
//    Software may require additional licenses from third parties.  
//    Therefore, you hereby agree it is your sole responsibility to separately
//    obtain any and all third party right and license necessary for your use of
//    such third party`s software. 
//
// 3. MStar Software and any modification/derivatives thereof shall be deemed as
//    MStar`s confidential information and you agree to keep MStar`s 
//    confidential information in strictest confidence and not disclose to any
//    third party.  
//
// 4. MStar Software is provided on an "AS IS" basis without warranties of any
//    kind. Any warranties are hereby expressly disclaimed by MStar, including
//    without limitation, any warranties of merchantability, non-infringement of
//    intellectual property rights, fitness for a particular purpose, error free
//    and in conformity with any international standard.  You agree to waive any
//    claim against MStar for any loss, damage, cost or expense that you may
//    incur related to your use of MStar Software.
//    In no event shall MStar be liable for any direct, indirect, incidental or
//    consequential damages, including without limitation, lost of profit or
//    revenues, lost or damage of data, and unauthorized system use.
//    You agree that this Section 4 shall still apply without being affected
//    even if MStar Software has been modified by MStar in accordance with your
//    request or instruction for your use, except otherwise agreed by both
//    parties in writing.
//
// 5. If requested, MStar may from time to time provide technical supports or
//    services in relation with MStar Software to you for your use of
//    MStar Software in conjunction with your or your customer`s product
//    ("Services").
//    You understand and agree that, except otherwise agreed by both parties in
//    writing, Services are provided on an "AS IS" basis and the warranty
//    disclaimer set forth in Section 4 above shall apply.  
//
// 6. Nothing contained herein shall be construed as by implication, estoppels
//    or otherwise:
//    (a) conferring any license or right to use MStar name, trademark, service
//        mark, symbol or any other identification;
//    (b) obligating MStar or any of its affiliates to furnish any person,
//        including without limitation, you and your customers, any assistance
//        of any kind whatsoever, or any information; or 
//    (c) conferring any license or right under any intellectual property right.
//
// 7. These terms shall be governed by and construed in accordance with the laws
//    of Taiwan, R.O.C., excluding its conflict of law rules.
//    Any and all dispute arising out hereof or related hereto shall be finally
//    settled by arbitration referred to the Chinese Arbitration Association,
//    Taipei in accordance with the ROC Arbitration Law and the Arbitration
//    Rules of the Association by three (3) arbitrators appointed in accordance
//    with the said Rules.
//    The place of arbitration shall be in Taipei, Taiwan and the language shall
//    be English.  
//    The arbitration award shall be final and binding to both parties.
//
//******************************************************************************
//<MStar Software>
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

#ifndef _Dapi_GIF_DECODER_H_
#define _Dapi_GIF_DECODER_H_

///////////////////////////////////////////////////////////////////////////////
/// @file   msapi_gif_decoder.h
/// This file contains the interface for GIF decoder
/// @author MStar Semiconductor Inc.
/// @brief  GIF decoder Module
///////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
extern "C" {
#endif  // #ifdef __cplusplus

//------------------------------------------------------------------------------
// Include Files
//------------------------------------------------------------------------------
//#include "mm_types.h"
#include "MsCommon.h"
#include "mapp_stillimage_sharememcfg.h"
#include "msapi_stillimage_dataio_file.h"
#include "msapi_stillimage_def.h"
//------------------------------------------------------------------------------
// Defines & Macros
//------------------------------------------------------------------------------
#undef INTERFACE
#ifdef _Dapi_GIF_DECODER_C_
#define INTERFACE
#else
#define INTERFACE extern
#endif

//------------------------------------------------------------------------------
// Type and Structure Declaration
//------------------------------------------------------------------------------
#if 0 // kiwi test move to msapi_stillimage_def.h 2013/11/11
typedef enum
{
    E_STILLIMAGE_RET_OK         = 0,
    E_STILLIMAGE_RET_FAIL,

    E_STILLIMAGE_RET_WORKING,
    E_STILLIMAGE_RET_DECODE_ONE_FRAME_DONE, //for gif animation, return done after decode every fame.
    E_STILLIMAGE_RET_DONE,
} EN_STILLIMAGE_RET;

typedef struct
{
    EN_STILLIMAGE_CMD_TYPE enCmdType;

    MS_U32 u32MinWidth;
    MS_U32 u32MinHeight;

    MS_U32 u32JpegBaseLineMaxWidth;
    MS_U32 u32JpegBaseLineMaxHeight;

    MS_U32 u32JpegProgressiveMaxWidth;
    MS_U32 u32JpegProgressiveMaxHeight;

    MS_U32 u32BmpMaxWidth;
    MS_U32 u32BmpMaxHeight;

    MS_U32 u32PngMaxWidth;
    MS_U32 u32PngMaxHeight;

    MS_U32 u32TiffMaxWidth;
    MS_U32 u32TiffMaxHeight;

    MS_U32 u32GifMaxWidth;
    MS_U32 u32GifMaxHeight;

    MS_U32 u32MPOBaseLineMaxWidth;
    MS_U32 u32MPOBaseLineMaxHeight;

    MS_U32 u32MPOProgressiveMaxWidth;
    MS_U32 u32MPOProgressiveMaxHeight;

    MS_BOOL bDecodeThumbnail;

    EN_STILLIMAGE_DATA_ACCESS_METHOD enDataAccessMethod;

    MS_U32 u32InputBuffAddr;
    MS_U32 u32InputBuffLen;

    MS_U32 u32OutputBuffAddr;
    MS_U32 u32OutputBuffLen;

    MS_U32 u32ProgressiveJpegBuffAddr;
    MS_U32 u32ProgressiveJpegBuffLen;

    MS_U32 u32MemPoolBuffAddr;
    MS_U32 u32MemPoolBuffLen;

    MS_BOOL bIsStreamMode;
    MS_U32 u32StreamMode_ReadTimeOut;

    //gif related
    MS_U32 u32FrameInfoArrayAddr;
    MS_U32 u32FrameInfoArraySize;
    MS_BOOL bOnlyDecodeFirstFrame;
    MS_BOOL bOnlyPlayFirstLoopForLoopAnimation;
    MS_U32 u32SrcBuffLen;
    MS_U32 u32SrcBuffAddr;
    // functionality control
    MS_U32 u32ControlFlags;
} ST_STILLIMAGE_CFG_PARAMS, *PST_STILLIMAGE_CFG_PARAMS;


typedef struct
{
    ST_STILLIMAGE_CFG_PARAMS stInputCfgParams;

    EN_STILLIMAGE_ERR_CODE enError;

    MS_U16 u16Progress;

    EN_STILLIMAGE_INPUT_DATA_FORMAT enInputFormat;

    MS_U32 u32OutputPitch;
    MS_U32 u32OutputWidth;
    MS_U32 u32OutputHeight;
    MS_U32 u32OutputSize;

    MS_U32 u32ImageWidth;
    MS_U32 u32ImageHeight;

    MS_BOOL bHasThumbnail;

    MS_U32 u32JpegExifWidth;
    MS_U32 u32JpegExifHeight;

    ST_STILLIMAGE_JPEG_EXIF stJpegEXIF;
    
    MS_U8 u8DownScaleFactor;

    EN_STILLIMAGE_OUTPUT_DATA_FORMAT enOutputFormat;

    MS_U32 u32FirstPixelPos;

    EN_STILLIMAGE_ORIENTATION enOrientation;

    // Shooting Date Time
    MS_BOOL bHasDateTime;
    MS_U32 u32Year, u32Month, u32Day;
    MS_U32 u32Hour, u32Minute, u32Second;

    //gif related
    MS_U32 u32DecodedFrameIndex, u32DisplayedFrameIndex;
    MS_U32 u32SlotTotalNum, u32OutputFrameSize;
    MS_U32 u32LoopTotalNumber; //zero means infinite loop
    U32 u32SrcBuffAddr;
    U32 u32SrcBuffLen;
    U32 delay_time;

    EN_STILLIMAGE_RET (*pfStillImage_ReadHeader)(void);
    EN_STILLIMAGE_RET (*pfStillImage_Task)(void);
    EN_STILLIMAGE_RET (*pfStillImage_Stop)(void);
} ST_STILLIMAGE, *PST_STILLIMAGE;
#endif

#define GIF_MAX_SCALE_DOWN_FACTOR 8

//------------------------------------------------------------------------------
// Extern Global Variabls
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Extern Functions
//------------------------------------------------------------------------------
///-----------------------------------------------------------------------------
/// @brief      Probe if data stream is GIF packed or not.
/// @return
///  A flag to indicate whether the data stream is GIF packed.
/// @retval     E_STILLIMAGE_RET_OK    the data stream is Tiff packed.
/// @retval     E_STILLIMAGE_RET_FAIL  the data stream is not Tiff packed.
/// @description
///  This function checks only first few bytes for Tiff signature
///-----------------------------------------------------------------------------
INTERFACE EN_STILLIMAGE_RET Dapi_GifDecoder_Probe(void);
INTERFACE EN_STILLIMAGE_RET Dapi_GifDecoder_ReadHeader(void);
INTERFACE EN_STILLIMAGE_RET Dapi_GifDecoder_Decode(void);
INTERFACE EN_STILLIMAGE_RET Dapi_GifDecoder_Stop(void);
INTERFACE EN_STILLIMAGE_RET Dapi_GifDecoder_SetOutBuf(U32 u32ObjNum);
INTERFACE EN_STILLIMAGE_RET Dapi_GifDecoder_SetBufferFrameCount(U32 u32FrameCount, U32 u32ObjNum);

INTERFACE EN_STILLIMAGE_RET Dapi_GifDecoder_Decode_Ex(U32 u32ObjNum);
INTERFACE EN_STILLIMAGE_RET Dapi_GifDecoder_Stop_Ex(U32 u32ObjNum);
INTERFACE EN_STILLIMAGE_RET Dapi_GifDecoder_Probe_Ex(U32 u32ObjNum);
INTERFACE EN_STILLIMAGE_RET Dapi_GifDecoder_ReadHeader_Ex(U32 u32ObjNum);

#undef INTERFACE

#ifdef __cplusplus
}
#endif  // #ifdef __cplusplus

#endif  // #ifndef _Dapi_GIF_DECODER_H_

///*** please do not remove change list tag ***///
///***$Change: 428882 $***///
