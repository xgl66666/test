//<MStar Software>
//******************************************************************************
// MStar Software
// Copyright (c) 2010 - 2017 MStar Semiconductor, Inc. All rights reserved.
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
// Copyright (c) 2006-2017 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (¡uMStar Confidential Information¡v) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   apiVENC.h
/// @brief  Video Encoder API
/// @author MStar Semiconductor,Inc.
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __APIVENC_H__
#define __APIVENC_H__

#ifdef __cplusplus
extern "C"
{
#endif


#define VENC_HANDLE MS_U32

typedef enum
{
    E_VENC_SUCCESS = 0,
    E_VENC_FAIL,
    E_VENC_INVALID_PARAM,
    E_VENC_INVALID_HANDLE,
    E_VENC_INVALID_STATE,
    E_VENC_UNSUPPORTED,
} VENC_RESULT;

typedef enum
{
    E_VENC_STATE_CLOSED = 0,
    E_VENC_STATE_IDLE,
    E_VENC_STATE_BUSY,
} VENC_STATE;

typedef enum
{
    E_VENC_CODEC_AVC = 0,
    E_VENC_CODEC_HEVC,
} VENC_CODEC;

typedef enum
{
    E_VENC_RATE_CONTROL_CQ = 0,
    E_VENC_RATE_CONTROL_CBR,
    E_VENC_RATE_CONTROL_CVBR,
    E_VENC_RATE_CONTROL_VBR,
} VENC_RATE_CONTROL;

typedef enum
{
    E_VENC_SCAN_TYPE_PROGRESSIVE = 0,
    E_VENC_SCAN_TYPE_INTERLACED,
} VENC_SCAN_TYPE;

typedef enum
{
    E_VENC_INPUT_FORMAT_YUVTILE = 0,  // OBSOLETE
    E_VENC_INPUT_FORMAT_MSTTILE,
    E_VENC_INPUT_FORMAT_YUYV,
    E_VENC_INPUT_FORMAT_YVYU,
    E_VENC_INPUT_FORMAT_UYVY,
    E_VENC_INPUT_FORMAT_VYUY,
    E_VENC_INPUT_FORMAT_EVDTILE,
    E_VENC_INPUT_FORMAT_32x32TILE,
    E_VENC_INPUT_FORMAT_NV12,
    E_VENC_INPUT_FORMAT_NV21,
} VENC_INPUT_FORMAT;

typedef enum
{
	E_VENC_PROFILE_BASELINE = 0,  // AVC only
    E_VENC_PROFILE_MAIN,
} VENC_PROFILE;

typedef enum
{
    E_VENC_LEVEL_1 = 0,
    E_VENC_LEVEL_1b,     // AVC only
    E_VENC_LEVEL_1_1,    // AVC only
    E_VENC_LEVEL_1_2,    // AVC only
    E_VENC_LEVEL_1_3,    // AVC only
    E_VENC_LEVEL_2,
    E_VENC_LEVEL_2_1,
    E_VENC_LEVEL_2_2,    // AVC only
    E_VENC_LEVEL_3,
    E_VENC_LEVEL_3_1,
    E_VENC_LEVEL_3_2,    // AVC only
    E_VENC_LEVEL_4,
    E_VENC_LEVEL_4_1,
    E_VENC_LEVEL_4_2,    // AVC only
    E_VENC_LEVEL_5,
    E_VENC_LEVEL_5_1,
    E_VENC_LEVEL_5_2,
    E_VENC_LEVEL_6,
    E_VENC_LEVEL_6_1,
    E_VENC_LEVEL_6_2,
} VENC_LEVEL;

typedef enum
{
    E_VENC_FRAME_TYPE_I = 0,
    E_VENC_FRAME_TYPE_P,
    E_VENC_FRAME_TYPE_B,
} VENC_FRAME_TYPE;

typedef enum
{
    E_VENC_CONFIG_CODEC = 0,            // VENC_CODEC
    E_VENC_CONFIG_DIMENSION,            // VENC_Dimension
    E_VENC_CONFIG_HW_BUFFER,            // VENC_Buffer
    E_VENC_CONFIG_OUTPUT_BUFFER,        // VENC_Buffer
    E_VENC_CONFIG_PROFILE,              // VENC_PROFILE
    E_VENC_CONFIG_LEVEL,                // VENC_LEVEL
    E_VENC_CONFIG_FRAME_RATE_X_100,     // MS_U32
    E_VENC_CONFIG_INTRA_PERIOD,         // MS_S32
    E_VENC_CONFIG_RATE_CONTROL,         // VENC_RATE_CONTROL
    E_VENC_CONFIG_CEILING_BIT_RATE,     // MS_U32
    E_VENC_CONFIG_AVERAGE_BIT_RATE,     // MS_U32
    E_VENC_CONFIG_SCAN_TYPE,            // VENC_SCAN_TYPE
    E_VENC_CONFIG_CABAC,                // MS_BOOL
    E_VENC_CONFIG_ISR,                  // MS_BOOL
    E_VENC_CONFIG_INPUT_FORMAT,         // VENC_INPUT_FORMAT
    E_VENC_CONFIG_CROP_INFO,            // VENC_CropInfo
    E_VENC_CONFIG_SAMPLE_ASPECT_RATIO,  // VENC_SampleAspectRatio
    E_VENC_CONFIG_STREAM_CONTEXT,       // MS_U64
    E_VENC_CONFIG_FRAME_DONE_CALLBACK,  // void *
    E_VENC_CONFIG_REDUCE_BW_MODE,       // MS_BOOL
    E_VENC_CONFIG_FAST_MODE,            // MS_BOOL
    E_VENC_CONFIG_DIP_MFE_HANDSHAKE,    // MS_BOOL
    E_VENC_CONFIG_GN_DBF_IMI,           // MS_BOOL
} VENC_CONFIG;


typedef struct DLL_PACKED
{
    MS_U32 u32Version;
    MS_S32 u32Size;
} VENC_VerCtl;

typedef struct DLL_PACKED
{
    VENC_VerCtl verCtrl;
    MS_U32 u32Width;
    MS_U32 u32Height;
} VENC_Dimension;

typedef struct DLL_PACKED
{
    VENC_VerCtl verCtrl;
    MS_BOOL bCropEnable;
    MS_U32 u32CropTop;
    MS_U32 u32CropBottom;
    MS_U32 u32CropLeft;
    MS_U32 u32CropRight;
} VENC_CropInfo;

typedef struct DLL_PACKED
{
    VENC_VerCtl verCtrl;
    MS_BOOL bSarInfoPresent;
    MS_U32 u32SarInfoIdc;
    MS_U32 u32SarWidth;
    MS_U32 u32SarHeight;
} VENC_SampleAspectRatio;

typedef struct DLL_PACKED
{
    VENC_VerCtl verCtrl;
    MS_U8 u8HwBufMiu;
    MS_PHY phyHwBufAddr;
    MS_U32 u32HwBufSize;
    MS_BOOL bHwBufCached;
} VENC_HwBuf;

typedef struct DLL_PACKED
{
    VENC_VerCtl verCtrl;
    MS_U64 u64OutBufAddr;
    MS_U32 u32OutBufSize;
} VENC_OutputBuf;


typedef void (*VENCFrameDoneCallback)(VENC_HANDLE handle,
                                      MS_U64 u64OutBuf,
                                      MS_S32 s32Len,
                                      MS_S32 frameType,
                                      MS_U64 u64StreamCtx,
                                      MS_U64 u64FrameCtx);


VENC_RESULT MApi_VENC_CreateTask(VENC_HANDLE *pHandle);

VENC_RESULT MApi_VENC_StartTask(VENC_HANDLE handle);

VENC_RESULT MApi_VENC_CloseTask(VENC_HANDLE handle);


VENC_STATE MApi_VENC_GetState(void);


VENC_RESULT MApi_VENC_SetConfig(VENC_HANDLE handle,
                                VENC_CONFIG config,
                                void *pParam,
                                MS_S32 s32ParamSize);


VENC_RESULT MApi_VENC_ReConfig(VENC_HANDLE handle,
                               VENC_CONFIG config,
                               void *pParam,
                               MS_S32 s32ParamSize);


VENC_RESULT MApi_VENC_GetConfig(VENC_HANDLE handle,
                                VENC_CONFIG config,
                                void *pParam,
                                MS_S32 s32ParamSize);


VENC_RESULT MApi_VENC_GetVpsSpsPps(VENC_HANDLE handle,
                                   MS_S32 *ps32VpsLen,
                                   MS_S32 *ps32SpsLen,
                                   MS_S32 *ps32PpsLen);


VENC_RESULT MApi_VENC_CompressOnePicture(VENC_HANDLE handle,
                                         MS_U8 u8InputMiu,
                                         MS_PHY phyLumaAddr,
                                         MS_PHY phyChromaAddr,
                                         MS_U32 u32Pitch,
                                         MS_BOOL bForceI,
                                         void *pFrameCtx);


VENC_RESULT MApi_VENC_GetPictureES(VENC_HANDLE handle,
                                   MS_U64 *pu64OutBuf,
                                   MS_S32 *ps32Len,
                                   MS_S32 *ps32FrameType,
                                   MS_U64 *pu64StreamCtx,
                                   MS_U64 *pu64FrameCtx);

#ifdef __cplusplus
}
#endif

#endif // __APIVENC_H__
