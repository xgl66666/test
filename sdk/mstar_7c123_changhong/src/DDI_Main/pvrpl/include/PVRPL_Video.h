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
// Copyright (c) 2008-2009 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// ("MStar Confidential Information") by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   PVRPL_Video.h
/// @brief  CPL API
/// @author MStar Semiconductor,Inc.
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef __API_PVRPL_VIDEO_H__
#define __API_PVRPL_VIDEO_H__

#include "MsCommon.h"
#include "PVRPL_Common.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define VIDEO_ERROR_Handler 0xff

//@Define video play mode like smooth scan or normal scan mode
#define VIDEO_MODE  MS_U16
#define VIDEO_NORMAL_SCAN_MODE  0x0001
#define VIDEO_SMOOTH_SCAN_MODE  0x0002
#define VIDEO_MODE_MASK         0xFFFF

typedef void* (*VDEC_CB_PLAY)(MS_U32 u32VCodec, MS_BOOL bStart);
typedef void* (*DISP_CB_ENABLE)(MS_BOOL bEnable);

typedef enum
{
    EN_PVRPL_XC_INPUT_SOURCE_DTV              = 0,
    EN_PVRPL_XC_INPUT_SOURCE_DTV2             = 1,
    EN_PVRPL_XC_INPUT_SOURCE_HDMI             = 2,
    EN_PVRPL_XC_INPUT_SOURCE_SCALER0_OP       = 3
}EN_PVRPL_XC_INPUT_SOURCE;

typedef struct
{
    MS_U32  u32X_Pos;
    MS_U32  u32Y_Pos;
    MS_U32  u32Width;
    MS_U32  u32Height;
    MS_BOOL bDef;
}ST_PVRPL_VDEC_DISPLAYWININFO;

typedef struct
{
    MS_U32                      u32Version;  // Store Version got from Demo_VDEC_GetStreamID.
    MS_U32                      u32Id;       // Store Id got from Demo_VDEC_GetStreamID.
    MS_S32                      s32VSpeed;   // for _PVRPL_Video_SetDecodeMode
    PVR_PATH                    u8PathIdx;   // main or sub. for almost all PVRPL_Video functions.
    MS_BOOL                     bVInit;      // If Vdec initialized. after PVRPL_Video_Init is called, this value should be TRUE.
    MS_BOOL                     bPCRClosed;  // for PVRPL_Video_SetSync. PCR Pid filter is stopped when enter the trick mode.
    MS_BOOL                     bIsSeamless; // for PVRPL_Video_Init. This value is used to judge if initialze Vdec is necessary. If this value is TRUE, Vdec shouldn't be initialzed again.
    EN_PVRPL_AVSYNC_MODE        eAVSyncMode; // AV sync mode
} POOL_VDEC_StreamInfo;

/// PVR CPL status indicator
typedef enum
{
    PVRPL_VIDEO_STATUS_OK,               ///< PVR CPL status OK
    PVRPL_VIDEO_STATUS_ERROR,            ///< PVR CPL status ERROR
} PVRPL_VIDEO_STATUS;

typedef enum
{
    E_PVRPL_VIDEO_DBG_NONE,
    E_PVRPL_VIDEO_DBG_MUST,
    E_PVRPL_VIDEO_DBG_ERR,
    E_PVRPL_VIDEO_DBG_WARN,
    E_PVRPL_VIDEO_DBG_INFO,
    E_PVRPL_VIDEO_DBG_FUNC,
    E_PVRPL_VIDEO_DBG_TRACE,
}EN_PVRPL_VIDEO_DBG_LEVEL;


typedef enum
{
    EN_VIDEO_CPL_CMD_NONE,
    EN_VIDEO_CPL_CMD_LASTFRAMESHOW,
    EN_VIDEO_CPL_CMD_ESBUFFMALLOC,
    EN_VIDEO_CPL_CMD_SEETTOPTS,
    EN_VDEC_CPL_CMD_TIMESHIFTSEAMLESS
}EN_VIDEO_CPL_CMD;

// video CPL enum
typedef enum
{
    EN_VIDEO_CPL_INFO_NONE,
    EN_VIDEO_CPL_INFO_IS264,
    EN_VIDEO_CPL_INFO_ISSTEPDISPDONE,
    EN_VIDEO_CPL_INFO_ISREACHSYNC,
    EN_VIDEO_CPL_INFO_ISAVSYNCON,
    EN_VIDEO_CPL_INFO_GET_PTS_HIGH_BIT, //High bit of PTS in Hz
    EN_VIDEO_CPL_INFO_GET_PTS,          // PTS in ms
    EN_VIDEO_CPL_INFO_GET_NEXTPTS,
    EN_VIDEO_CPL_INFO_GET_DISP_CNT,
    EN_VIDEO_CPL_INFO_GET_FRAME_CNT,
    EN_VIDEO_CPL_INFO_GET_SKIP_CNT,
    EN_VIDEO_CPL_INFO_DROP_CNT,
    EN_VIDEO_CPL_INFO_ERR_CNT,
    EN_VIDEO_CPL_INFO_ES_WP,
    EN_VIDEO_CPL_INFO_ES_RP,
    EN_VIDEO_CPL_INFO_WATERLEVEL,
    EN_VIDEO_CPL_INFO_GET_PTSSTC_DELTA,
    EN_VIDEO_CPL_INFO_GET_FRAME_RATE,
    EN_VIDEO_CPL_INFO_GET_TOLERANCE,
    EN_VIDEO_CPL_INFO_IS_FFX2_CAP_ENOUGH,
    EN_VIDEO_CPL_INFO_GET_MIN_TSP_DATA_SIZE
}EN_VIDEO_CPL_INFO;

typedef enum
{
    //MPEG 1/2
    EN_PL_VDEC_CODEC_TYPE_MPEG2 = 0,
    //H264
    EN_PL_VDEC_CODEC_TYPE_H264,
    //AVS
    EN_PL_VDEC_CODEC_TYPE_AVS,
    //HEVC
    EN_PL_VDEC_CODEC_TYPE_HEVC,
    //MPEG 1/2 for I-Frame
    EN_PL_VDEC_CODEC_TYPE_MPEG2_IFRAME,
    //H264 for I-Frame
    EN_PL_VDEC_CODEC_TYPE_H264_IFRAME,
    //HEVC for I-Frame
    EN_PL_VDEC_CODEC_TYPE_HEVC_IFRAME,

}EN_PL_VDEC_CodecType;

// Porting Layer Video
typedef enum
{
    EN_VIDEO_CPL_DECODE_TYPE_INVALID,
    EN_VIDEO_CPL_DECODE_TYPE_DECODE_I,
    EN_VIDEO_CPL_DECODE_TYPE_DECODE_ALL,
}EN_VIDEO_CPL_DECODE_TYPE;

typedef enum
{
    EN_VIDEO_CPL_DATA_TYPE_INVALID,
    EN_VIDEO_CPL_DATA_TYPE_CONTINUOUS_IPB,
    EN_VIDEO_CPL_DATA_TYPE_ONLY_I,
}EN_VIDEO_CPL_DATA_TYPE;

typedef enum
{
   EN_PL_VDEC_TRICK_TYPE_NORMAL=0,
   EN_PL_VDEC_TRICK_TYPE_FORWARD,
   EN_PL_VDEC_TRICK_TYPE_SCAN
}EN_PL_VDEC_TRICK_TYPE;

//need to sync mapi_pvr.h pidType Enum value
typedef enum
{
    PVRPL_PCR_PID = 0,
    PVRPL_AUDIO_PID
}PVRPL_PidType;

//@NOTE need to add [weak function attribute] for new PVRPL function to avoid compiling error without implementing new PVRPL function

// Video Module
PVRPL_VIDEO_STATUS PVRPL_Video_Init(POOL_VDEC_StreamInfo *pStreamVideoInfo,MS_U32 u32VCodec,MS_U8 u8STCEng,void *pVideoID);
PVRPL_VIDEO_STATUS PVRPL_Video_GetInfo(POOL_VDEC_StreamInfo *pStreamVideoInfo, EN_VIDEO_CPL_INFO eVideoInfoCmd, MS_U32 *u32pmtr, void* structure);
MS_U32             PVRPL_GetWaterLevel(POOL_VDEC_StreamInfo *pStreamVideoInfo,MS_U32 u32MmapSize);
PVRPL_VIDEO_STATUS PVRPL_Video_Mute(POOL_VDEC_StreamInfo *pStreamVideoInfo, MS_U8 u8PathID, MS_BOOL bEnable);
PVRPL_VIDEO_STATUS PVRPL_Video_EnableCMD(POOL_VDEC_StreamInfo *pStreamVideoInfo, EN_VIDEO_CPL_CMD eVideoCmd, MS_U32 u32pmtr);
PVRPL_VIDEO_STATUS PVRPL_Video_SetDecodeMode(POOL_VDEC_StreamInfo *pStreamVideoInfo, MS_S32 s32Speed, EN_VIDEO_CPL_DECODE_TYPE enDecodeType, EN_VIDEO_CPL_DATA_TYPE enDataType);
PVRPL_VIDEO_STATUS PVRPL_Video_StepDisp(POOL_VDEC_StreamInfo *pStreamVideoInfo);

PVRPL_VIDEO_STATUS PVRPL_Video_SetSync(POOL_VDEC_StreamInfo *pStreamVideoInfo, EN_PL_VDEC_TRICK_TYPE type, MS_U16 u16Pid,MS_U8 *u8PidFlt);

PVRPL_VIDEO_STATUS PVRPL_Video_SetReachSync(POOL_VDEC_StreamInfo *pStreamVideoInfo);
PVRPL_VIDEO_STATUS PVRPL_Video_SetSyncOnParam(POOL_VDEC_StreamInfo *pStreamVideoInfo, MS_U32 u32SyncDelay, MS_U16 u16SyncTolerance);
PVRPL_VIDEO_STATUS PVRPL_Video_CheckSync(POOL_VDEC_StreamInfo *pStreamVideoInfo);

PVRPL_VIDEO_STATUS PVRPL_Video_PlayXC(POOL_VDEC_StreamInfo *pStreamVideoInfo,MS_U8 u8PathID);
PVRPL_VIDEO_STATUS PVRPL_Video_FlushData(POOL_VDEC_StreamInfo *pStreamVideoInfo, MS_U16 u16VideoPID, MS_U16 u16PacketLen, MS_U32 u32VirAdr);
PVRPL_VIDEO_STATUS PVRPL_Video_FileEnd(POOL_VDEC_StreamInfo *pStreamVideoInfo, MS_U16 u16VideoPID, MS_U16 u16PacketLen, MS_U32 u32VirAdr);
PVRPL_VIDEO_STATUS PVRPL_Video_Exit(POOL_VDEC_StreamInfo *pStreamVideoInfo);
PVRPL_VIDEO_STATUS PVRPL_Video_ChangeDisplayWindow(POOL_VDEC_StreamInfo *pStreamVideoInfo,ST_PVRPL_VDEC_DISPLAYWININFO stDisplayWinInfo)__attribute__((weak));

PVRPL_VIDEO_STATUS PVRPL_Video_RegisterPlayCB(VDEC_CB_PLAY PlayCB);

#ifdef __cplusplus
}
#endif

#endif // #ifndef __API_PVRPL_VIDEO_H__
