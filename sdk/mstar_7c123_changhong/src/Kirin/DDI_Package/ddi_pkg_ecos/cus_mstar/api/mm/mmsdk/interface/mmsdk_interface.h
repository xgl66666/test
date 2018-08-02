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

#ifndef __MMSDK_INTERFACE_H__
#define __MMSDK_INTERFACE_H__

#include "mmsdk_interface_def.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @file
 * @defgroup mmsdk_interface mmsdk_interface functions
 * @{
 */

//-------------------------------------------------------------------------------------------------
/// Initialize the MM when it starts, should call it before any function called
/// Note: vdplayer will use BDMA, so you must init BDMA(MDrv_BDMA_Init) before MMSDK_Initialized
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL MMSDK_Initialize(void);

//-------------------------------------------------------------------------------------------------
/// Finalize the MM when it quits
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL MMSDK_Finalize(void);

//-------------------------------------------------------------------------------------------------
/// Set debug message level
/// @param eLevel              \b IN: debug message level (none/error/info/debug/trace/FW)
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL MMSDK_DebugMSGLevel(const EN_MMSDK_DBG_LEVEL eLevel);

//-------------------------------------------------------------------------------------------------
/// Set DumpESData or not.
/// @param bDumpESData              \b IN: decide to dump es data to ESData.mkv or not
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL MMSDK_DumpESData(const MMSDK_BOOL bDumpESData);

//-------------------------------------------------------------------------------------------------
/// Crate media item. Appliation need create one media item before perform any process.
/// @param pstMedia          \b IN: The information of media item.
/// @param astExtenInfoParam        \b IN: Extensive create media information parameters array (default NULL) (Option:EN_MMSDK_MEDIAINFO_EXT)
/// @param u32ExtenInfoParamNum  \b IN: The number of parameters (default 0)
/// @param pItem          \b OUT: Return media item.
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL MMSDK_CreateMediaItem(const ST_MMSDK_CREATE_MEDIA_INFO *pstMedia, const ST_MMSDK_PARAM* astExtenInfoParam, const MMSDK_U32 u32ExtenInfoParamNum, MEDIAITEM * pItem);

//-------------------------------------------------------------------------------------------------
/// Delete media item. Appliation need delete media item after perform all process.
/// @param Item          \b IN: Media item.
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL MMSDK_DeleteMediaItem(MEDIAITEM Item);

//-------------------------------------------------------------------------------------------------
// get some parameter or status of the MM
/// @param Item          \b IN: Media item.
/// @param astParam      \b OUT: The parameters array (astParam.Option: EN_MMSDK_OPTION_TYPE)
/// @param u32ParamNum   \b IN: The number of parameters array
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL MMSDK_GetOptionByPara(MEDIAITEM Item, ST_MMSDK_PARAM* astParam, const MMSDK_U32 u32ParamNum);

//-------------------------------------------------------------------------------------------------
// Set some parameter or status of the MM (Reserved)
/// @param Item          \b IN: Media item.
/// @param astParam        \b IN: The parameters array (astParam.Option: EN_MMSDK_OPTION_TYPE)
/// @param u32ParamNum  \b IN: The number of parameters array
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL MMSDK_SetOptionByPara(MEDIAITEM Item, const ST_MMSDK_PARAM* astParam, const MMSDK_U32 u32ParamNum);

//-------------------------------------------------------------------------------------------------
/// Play media item.
/// @param Item          \b IN: Media item.
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL MMSDK_Play(MEDIAITEM Item);

//-------------------------------------------------------------------------------------------------
/// Stop media item.
/// @param Item          \b IN: Media item.
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL MMSDK_Stop(MEDIAITEM Item);

//-------------------------------------------------------------------------------------------------
/// Set current media item program for TS with input index (for movie item only)
/// @param Item                \b IN: Media item.
/// @param u8ProgramID         \b IN: Program index(TS).
/// @param pstTSInfo        \b IN: Program info. (Default: NULL; Only for video transport stream (TS)).
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL MMSDK_SetProgramID(MEDIAITEM Item, const MMSDK_U8 u8ProgramID, const ST_MMSDK_TS_INFO* pstTSInfo);

//-------------------------------------------------------------------------------------------------
/// Get current media item program of TS index (for movie item only)
/// @param Item          \b IN: Media item.
/// @param pu8ProgramID     \b OUT: Return program index for TS.
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL MMSDK_GetProgramID(MEDIAITEM Item, MMSDK_U8 * pu8ProgramID);

//------------------------------------------------------------------------------------------------
/// Set media item audio track index (for movie item only).
/// @param Item          \b IN: Media item.
/// @param u8TrackID        \b IN: Audio track index.
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL MMSDK_SetAudioTrackID(MEDIAITEM Item, const MMSDK_U8 u8TrackID);

//------------------------------------------------------------------------------------------------
/// Get media item audio track index (for movie item only).
/// @param Item          \b IN: Media item.
/// @param pu8TrackID     \b OUT: Return audio track index
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL MMSDK_GetAudioTrackID(MEDIAITEM Item, MMSDK_U8 * pu8TrackID);

//------------------------------------------------------------------------------------------------
/// Set media item audio track index (for movie TS/DSS only).
/// @param Item          \b IN: Media item.
/// @param pstTSInfo        \b IN: TS/DSS audio pid and codec info.
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL MMSDK_SetAudioTrackPID(MEDIAITEM Item, const ST_MMSDK_TS_INFO* pstTSInfo);

//------------------------------------------------------------------------------------------------
/// Get media item audio track index (for movie TS/DSS only).
/// @param Item          \b IN: Media item.
/// @param pstTSInfo        \b OUT: TS/DSS audio pid and codec info.
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL MMSDK_GetAudioTrackPID(MEDIAITEM Item, ST_MMSDK_TS_INFO* pstTSInfo);

//------------------------------------------------------------------------------------------------
/// Set media item subtitle track index (for movie item only).
/// @param Item          \b IN: Media item.
/// @param u8TrackID        \b IN: Subtitle track index.
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL MMSDK_SetSubtitleTrackID(MEDIAITEM Item, const MMSDK_U8 u8TrackID);

//-------------------------------------------------------------------------------------------------
/// Get current media item subtitle track information (for movie item only).
/// @param Item          \b IN: Media item.
/// @param pu8TrackID     \b OUT: Return subtitle track index
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL MMSDK_GetSubtitleTrackID(MEDIAITEM Item, MMSDK_U8 * pu8TrackID);

//------------------------------------------------------------------------------------------------
/// Set media item subtitle on off (for movie item only).
/// @param Item          \b IN: Media item.
/// @param bOnOff        \b IN: TRUE: subtitle on; FALSE: subtitle off
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL MMSDK_SetSubtitleOnOff(MEDIAITEM Item, const MMSDK_BOOL bOnOff);

//------------------------------------------------------------------------------------------------
/// Set media item subtitle on off (for movie item only).
/// @param Item          \b IN: Media item.
/// @param pstSubDispRect \b IN: Subtitle display rectaangle.
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL MMSDK_SetSubtitleRectangle(MEDIAITEM Item, const ST_MMSDK_RECT* pstSubDispRect);

//-------------------------------------------------------------------------------------------------
/// Set media preview mode.
/// @param Item             \b IN: Media item.
/// @param pstPreview       \b IN: Meida preview mode.
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL MMSDK_SetPreviewMode(MEDIAITEM Item, const ST_MMSDK_PREVIEW_SETTING * pstPreview);

//=================  Display Begin =====================/

//-------------------------------------------------------------------------------------------------
/// ES Player Push Payload
/// @param Item          \b IN: Media item.
/// @param pdata            \b IN: push audio/video data to buffer
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL MMSDK_ES_PushPayload(MEDIAITEM Item, const ST_MMSDK_ES_DATA_INFO *pdata);

//-------------------------------------------------------------------------------------------------
/// ES Player Push Payload with 64 bit based pts
/// @param Item          \b IN: Media item.
/// @param pdata            \b IN: push audio/video data to buffer with 64 bit based pts
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL MMSDK_ES_PushPayload64(MEDIAITEM Item, const ST_MMSDK_ES_DATA_INFO64 *pdata);

//-------------------------------------------------------------------------------------------------
/// Set Crypto header for each encrypted ES data(TEE Info)
/// @deprecated new:#MMSDK_ES_SetESCusHeaderInfoEx
/// @param p_header          \b IN: customized header start address
/// @param header_len            \b IN: customized header size
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL MMSDK_ES_SetESCusHeaderInfo(void* p_header, MMSDK_U32 header_len);

//-------------------------------------------------------------------------------------------------
/// Set customized header info for push ES data
/// @param Item          \b IN: Media item.
/// @param pHeader       \b IN: customized header start address
/// @param u32Length     \b IN: customized header size
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL MMSDK_ES_SetESCusHeaderInfoEx(MEDIAITEM Item, const void* pHeader, MMSDK_U32 u32Length);

//-------------------------------------------------------------------------------------------------
/// ES Playerset flush data
/// Note: check Callback message(E_MMSDK_CALLBACK_ES_FLUSH_DATA_RDY) whether flushing finished or not
/// @param Item          \b IN: Media item.
/// @param datatype         \b IN: datatype
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL MMSDK_ES_Flush(MEDIAITEM Item,const EN_MMSDK_ES_DATA_TYPE datatype);

//------------------------------------------------------------------------------------------------
/// ES Player Switch media item audio track with given audio parameter
/// @param Item          \b IN: Media item.
/// @param pstAudioParam     \b IN: Audio track information.
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL MMSDK_ES_SwitchAudioTrack(MEDIAITEM Item,const ST_MMSDK_ES_AUDIO_INFO * pstAudioParam);

//-------------------------------------------------------------------------------------------------
/// ES Player Get Data Buffer Info (ES data buffer manager)
/// @param Item    \b IN: Media item.
/// @param eVideoBuffInfo      \b IN: info type of buffer
/// @param pu32Data      \b OUT: Return data buffer inofrmation.
///
/// @return TRUE: Success.
/// @return FALSE: Failure
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL MMSDK_ES_GetDataBufferInfo(MEDIAITEM Item, const EN_MMSDK_VIDEO_BUFFER_INFO eVideoBuffInfo, MMSDK_U32* const pu32Data);

//-------------------------------------------------------------------------------------------------
/// Set data to end. no more data to input.
/// @param Item      \b IN: Media item.
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL MMSDK_SetDataEnd(MEDIAITEM Item);

//-------------------------------------------------------------------------------------------------
/// Reset cpcodec demux (for movie item only).
/// @param Item          \b IN: Media item.
/// @param u32CtrlFlag
//**********************************************************
/// Bit0 = TRUE, disable AV sync
/// Bit1 = TRUE, Only Decode I frame
/// Bit2 = TRUE, disable Audio
//**********************************************************
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL MMSDK_DemuxReset(MEDIAITEM Item, const MMSDK_U32 u32CtrlFlag);

//-------------------------------------------------------------------------------------------------
/// Reset cpcodec demux asynchronously(for movie item only).
/// @param Item          \b IN: Media item.
/// @param u32CtrlFlag
//**********************************************************
/// Bit0 = TRUE, disable AV sync
/// Bit1 = TRUE, Only Decode I frame
/// Bit2 = TRUE, disable Audio
//**********************************************************
/// @return E_MMSDK_OK: Success.
/// @return E_MMSDK_FAIL: Failure.
/// @return E_MMSDK_ASYNC: Asynchronous.
//-------------------------------------------------------------------------------------------------
E_MMSDK_RESULT MMSDK_DemuxResetAsync(MEDIAITEM Item, const MMSDK_U32 u32CtrlFlag);

//-------------------------------------------------------------------------------------------------
/// Get thumbnail (movie/photo)
/// @param Item          \b IN: Media item.
/// @param pstRequestParam \b IN : input parameter:
/// @param pBuffAddr \b IN/OUT : start virtual address for thumbnail buffer(if NULL, player will use co-buffer)
/// @param pu32Pitch \b OUT : output buffer pitch
///
/// thumbnail data getting need before MMSDK_DeleteMediaItem()
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL MMSDK_GetThumbnail(MEDIAITEM Item, const ST_MMSDK_THUMBNAIL_PARAM* pstRequestParam, void** pBuffAddr, MMSDK_U32* pu32Pitch);

//-------------------------------------------------------------------------------------------------
/// Get photo media item Zoom map.
/// @param Item          \b IN: Media item.
/// @param pstCaptureParam  \b IN: zoom map setting.
/// @param pBuffAddr        \b IN/OUT : start virtual address for zoom map buffer(if NULL, just return the current crop region)
/// @param pu32CapBufPitch  \b OUT: Output pitch information.
/// @param pstZoomMapRect   \b OUT: Setting size for zoom map (a ratio of panel size)
/// @param pstDisplayAreaRect \b OUT: a zoom-in area of zoom map
///
/// zoom map data getting need After zoom-in feature is executed.
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL MMSDK_GetZoomMap(MEDIAITEM Item, ST_MMSDK_THUMBNAIL_PARAM* pstCaptureParam, void* pBuffAddr, MMSDK_U32* pu32CapBufPitch, ST_MMSDK_RECT* pstZoomMapRect, ST_MMSDK_RECT* pstDisplayAreaRect);


//-------------------------------------------------------------------------------------------------
/// Capture display as jpeg(photo)
/// @param Item           \b IN: Media item.
/// @param jpegFileName   \b IN: output jpeg file name.
/// @param u32LogoWidth   \b IN: output jpeg width.
/// @param u32LogoHeight  \b IN: output jpeg height.
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL MMSDK_CaptureLogo(MEDIAITEM Item, const MMSDK_U8* jpegFileName, const MMSDK_U32 u32LogoWidth, const MMSDK_U32 u32LogoHeight);

//-------------------------------------------------------------------------------------------------
/// Set seamless playing mode
/// @param eSeamlessMode          \b IN: seamless playing mode
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL MMSDK_SetSeamlessMode(const EN_MMSDK_SEAMLESS_MODE eSeamlessMode);

//-------------------------------------------------------------------------------------------------
/// Set time out ifnormation to player. Player will Use this infomation for error concealment control.
/// Player will use default value if the application was not invked.
/// @param pstTimeOutInfo     \b IN: Pointer time out information.
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL MMSDK_SetPlayerTimeOutValue(ST_MMSDK_TIMEOUT_VALUE* pstTimeOutInfo);

//-------------------------------------------------------------------------------------------------
/// Clear photo display buffer.
/// @param u8R              \b IN: Red color (0 ~ 255).
/// @param u8G              \b IN: Green color (0 ~ 255).
/// @param u8B              \b IN: Blue color (0 ~ 255).
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL MMSDK_ClearPhotoDisplay(const MMSDK_U8 u8R, const MMSDK_U8 u8G, const MMSDK_U8 u8B);

//-------------------------------------------------------------------------------------------------
/// Set movie backward replay mode (for movie item only).
/// @param bEnable          \b IN: TRUE to enable backward replay mode. FALSE to disable it.
///
/// @return E_PLAYER_OK: Success.
/// @return E_PLAYER_FAIL: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL MMSDK_SetBackwardRePlayMode(const MMSDK_BOOL bEnable);

//-------------------------------------------------------------------------------------------------
/// Parser header, this header parsering is for Netflix ASF stream used only
/// @param u8IsVideoOrAudioStream      \b IN: 0--video, 1--audio
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL MMSDK_DemuxHeader(const MMSDK_U8 u8IsVideoOrAudioStream);

//-------------------------------------------------------------------------------------------------
/// Set the total time when playing ts stream  and it can reduce the initialization time
/// @param u32TotalTime      \b IN: the total time of ts stream
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL MMSDK_SetStreamTotalTime(const MMSDK_U32 u32TotalTime);

//-------------------------------------------------------------------------------------------------
/// set the start time from when the stream will be decoded
/// @param u32StartTime      \b IN: the start time of the stream
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL MMSDK_SetStreamStartTime(const MMSDK_U32 u32StartTime);

//------------------------------------------------------------------------------------------------
/// Set media item teletext track index (for movie item only).
/// @param Item             \b IN: Media item.
/// @param u8TrackID        \b IN: Teletext track index.
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL MMSDK_SetTtxTrackID(MEDIAITEM Item, const MMSDK_U8 u8TrackID);

//-------------------------------------------------------------------------------------------------
/// Get current media item teletext track information (for movie item only).
/// @param Item             \b IN: Media item.
/// @param pTtxTrackInfo    \b OUT: Return teletext track information.
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL MMSDK_GetTtxTrackInfo(MEDIAITEM Item, ST_MMSDK_TTX_TRACK_INFO *pTtxTrackInfo);

//-------------------------------------------------------------------------------------------------
/// Enable/Disable media item teletext display (for movie item only).
/// @param Item             \b IN: Media item.
/// @param bEnable          \b IN: Control teletext decode or not decode.
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL MMSDK_EnableTtxDecode(MEDIAITEM Item, const MMSDK_BOOL bEnable);

//-------------------------------------------------------------------------------------------------
/// media item teletext show or not show (for movie item only).
/// @param Item             \b IN: Media item.
/// @param bEnable          \b IN: Control teletext show or not show.
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL MMSDK_EnableTtxShow(MEDIAITEM Item, const MMSDK_BOOL bEnable);

//-------------------------------------------------------------------------------------------------
/// Enable/Disable media item closed caption display (for movie item only).
/// @param Item             \b IN: Media item.
/// @param eCmdType         \b IN: Control teletext decode or not decode.
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL MMSDK_EnableCC(MEDIAITEM Item, const EN_MMSDK_CC_CMD_TYPE eCmdType);

//=================  Display Begin =====================/
//-------------------------------------------------------------------------------------------------
/// [Display] Set media item crop and display rectangle
/// Set window will reset these show effects (exp: zoom/move)
/// @param Item          \b IN: Media item.
/// @param pstInfo        \b IN: crop and display setting (stCropRect/stDisplayRect/bKeepRatio)
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL MMSDK_SetCropDisplayRectangle(MEDIAITEM Item,const ST_MMSDK_CROP_DISPLAY_WINDOW * pstInfo);

//-------------------------------------------------------------------------------------------------
/// [Display] Set display aspect ratio.(for video only)
/// @deprecated new:#MMSDK_SetAspectRatioEx
/// @param eMediaType          \b IN: Media type
/// @param eRatio           \b IN: Video display aspect ratio.
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL MMSDK_SetAspectRatio(const EN_MMSDK_MEDIA_TYPE eMediaType, const EN_MMSDK_ASPECT_RATIO eRatio);

//-------------------------------------------------------------------------------------------------
/// [Display] Set display aspect ratio.(for video only)
/// @param Item          \b IN: Media item.
/// @param eMediaType          \b IN: Media type
/// @param eRatio           \b IN: Video display aspect ratio.
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL MMSDK_SetAspectRatioEx(MEDIAITEM Item, const EN_MMSDK_MEDIA_TYPE eMediaType, const EN_MMSDK_ASPECT_RATIO eRatio);

//-------------------------------------------------------------------------------------------------
/// [Display] Get display aspect ratio.(for video only)
/// @deprecated new:#MMSDK_GetAspectRatioEx
/// @param eMediaType          \b IN: Media type
/// @param peRatio           \b OUT: Video display aspect ratio.
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL MMSDK_GetAspectRatio(const EN_MMSDK_MEDIA_TYPE eMediaType, EN_MMSDK_ASPECT_RATIO * peRatio);

//-------------------------------------------------------------------------------------------------
/// [Display] Get display aspect ratio.(for video only)
/// @param Item          \b IN: Media item.
/// @param eMediaType          \b IN: Media type
/// @param peRatio           \b OUT: Video display aspect ratio.
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL MMSDK_GetAspectRatioEx(MEDIAITEM Item, const EN_MMSDK_MEDIA_TYPE eMediaType, EN_MMSDK_ASPECT_RATIO * peRatio);

//-------------------------------------------------------------------------------------------------
/// [Display] Move media item display view (for photo and movie item only). If the current zoom factor is bigger than original size, application can use this function move view.
/// @param Item          \b IN: Media item.
/// @param eDirection       \b IN: Move direction.
/// @param u16Offset        \b IN: Move offset.
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL MMSDK_MoveViewWindow(MEDIAITEM Item, const EN_MMSDK_VIEW_MOVE_DIRECTION eDirection, const MMSDK_U16 u16Offset);

//-------------------------------------------------------------------------------------------------
/// [Display] Move view window.
/// @param Item          \b IN: Media item.
/// @param s32XOffset       \b IN: horizontal offset. Positive value is the right offset.
/// @param s32YOffset        \b IN: vertical offset. Positive value is the bottom offset.
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL MMSDK_SetViewOffset(MEDIAITEM Item, const MMSDK_S32 s32XOffset, const MMSDK_S32 s32YOffset);

//-------------------------------------------------------------------------------------------------
/// [Display] Get media item view offset (for photo and movie item only). If the current zoom factor is bigger than original size, application can use this function to get view offset.
/// @param Item          \b IN: Media item.
/// @param ps32XOffset      \b OUT: Return horizontal offset. Positive value is the right offset.
/// @param ps32YOffset      \b OUT: Return vertical offset. Positive value is the bottom offset.
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL MMSDK_GetViewOffset(MEDIAITEM Item, MMSDK_S32 * ps32XOffset, MMSDK_S32 * ps32YOffset);

//-------------------------------------------------------------------------------------------------
/// [Display] Get media item zoom in/out factor (for photo and movie item only).
/// @param Item          \b IN: Media item.
/// @param eFactor          \b IN: Return zoom factor.
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL MMSDK_SetZoomFactor(MEDIAITEM Item, const EN_MMSDK_VIEW_ZOOM_FACTOR eFactor);

//-------------------------------------------------------------------------------------------------
/// [Display] Get media item zoom in/out factor (for photo and movie item only).
/// @param Item          \b IN: Media item.
/// @param pFactor          \b OUT: Return zoom factor.
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL MMSDK_GetZoomFactor(MEDIAITEM Item, EN_MMSDK_VIEW_ZOOM_FACTOR * pFactor);

//-------------------------------------------------------------------------------------------------
/// [Display] Set photo media item slide show effect (for photo item only).
/// @param eEffect          \b IN: Slide show effect.
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL MMSDK_SetSlideShowEffect(const EN_MMSDK_PHOTO_SLIDE_SHOW_EFFECT eEffect);

//-------------------------------------------------------------------------------------------------
/// [Display] Get current photo media item slide show effect (for photo item only).
/// @param pEffect          \b OUT: Return slide show effect.
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL MMSDK_GetSlideShowEffect(EN_MMSDK_PHOTO_SLIDE_SHOW_EFFECT * pEffect);

//-------------------------------------------------------------------------------------------------
/// [Display] Set Rotate media item and rotate the photo(currently only implement in photo item).
/// @param Item          \b IN: Media item.
/// @param eAngle           \b IN: Rotation angle.
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL MMSDK_SetRotateAngle(MEDIAITEM Item, const EN_MMSDK_ROTATE_ANGLE eAngle);

//-------------------------------------------------------------------------------------------------
/// [Display] Get Rotate photo media item (for photo item only).
/// @param Item          \b IN: Media item.
/// @param peAngle           \b OUT: Rotation angle.
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL MMSDK_GetRotateAngle(MEDIAITEM Item, EN_MMSDK_ROTATE_ANGLE * peAngle);

//-------------------------------------------------------------------------------------------------
/// [Display] Suspend Dynamic Scaling before enable 3D (for movie item only)
/// Currently dynamic scaling will conflicted with 3D, so display as 3D should call this API to prevent unexpected result
/// When eable 3D mmsdk will notify FW to suspend dynamic scaling to prevent exepct.
/// @param bEnable        \b IN: TRUE for suspend dynamic scaling. FALSE for resume dynamic scaling
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//------------------------------------------------------------------------------------------------
MMSDK_BOOL MMSDK_SuspendDynamicScaling(const MMSDK_BOOL bEnable);

//-------------------------------------------------------------------------------------------------
/// [Display] Query if suspend dynamic scaling procedure is ongoing or not (for movie item only)
///
/// @return TRUE: suspend ds procedure is on going, not finished
/// @return FALSE: suspend ds procedure is finished
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL MMSDK_IsSuspendDSProcessing(void);
//=================  Display  End =====================/



//=================  TimePlay  Begin =====================/
//-------------------------------------------------------------------------------------------------
/// [TimePlay] Set media item trick mode process (for audio and movie item only).
/// @param Item          \b IN: Media item.
/// @param eTrickMode      \b IN: Trick mode information.
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL MMSDK_SetTrickMode(MEDIAITEM Item, const EN_MMSDK_TRICK_MODE eTrickMode);

//-------------------------------------------------------------------------------------------------
/// [TimePlay] Get media item trick mode information (for audio and movie item only).
/// @param Item          \b IN: Media item.
/// @param pstTrickInfo     \b Out: Return trick mode information.
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL MMSDK_GetTrickMode(MEDIAITEM Item, EN_MMSDK_TRICK_MODE * pstTrickInfo);

//-------------------------------------------------------------------------------------------------
/// [TimePlay] Play media item from input time setting
/// @param Item          \b IN: Media item.
/// @param u32TimeInMs      \b IN: Time position in millisecond.
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL MMSDK_SetPlayPosition(MEDIAITEM Item,const  MMSDK_U32 u32TimeInMs);

//-------------------------------------------------------------------------------------------------
/// [TimePlay] Get media item current play time
/// @param Item          \b IN: Media item.
/// @param pu32TimeInMs     \b OUT: Return time position in millisecond.
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL MMSDK_GetPlayPosition(MEDIAITEM Item, MMSDK_U32 * pu32TimeInMs);

//------------------------------------------------------------------------------------------------
/// [TimePlay] Set resume playback (for movie item only).
/// Application need invoke this function before play function. The media will playback from resume information.
/// @param Item          \b IN: Media item.
/// @param pstResumeInfo     \b IN: Resume playback inofrmation.
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL MMSDK_SetResumePlay(MEDIAITEM Item, const ST_MMSDK_RESUME_INFO * pstResumeInfo);

//------------------------------------------------------------------------------------------------
/// [TimePlay] Get resume playback information (for movie item only).
/// Application need invoke this function before stop function. The resume information will keep last playback information.
/// @param Item          \b IN: Media item.
/// @param pstResumeInfo    \b OUT: Return resume playback inofrmation.
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL MMSDK_GetResumePlayInfo(MEDIAITEM Item, ST_MMSDK_RESUME_INFO * pstResumeInfo);

//-------------------------------------------------------------------------------------------------
/// [TimePlay] Set repeat A to B playback. Application set A point for repeat start and set B point for repeat end (for movie item only).
/// @param Item          \b IN: Media item.
/// @param eRepeatAB        \b IN: Repeat A to B information.
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL MMSDK_SetRepeatAB(MEDIAITEM Item, const EN_MMSDK_REPEAT_AB eRepeatAB);

//-------------------------------------------------------------------------------------------------
/// [TimePlay] Set loop mode (replay after end of stream)
/// @param Item    \b IN: Media item.
/// @param bLoop      \b IN: loop or not
///
/// @return TRUE: Success.
/// @return FALSE: Failure
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL MMSDK_SetLoop(MEDIAITEM Item, const MMSDK_BOOL bLoop);
//=================  TimePlay  End =====================/




//=================  Divx Related API Begin =====================/
//-------------------------------------------------------------------------------------------------
/// [Divx] Set DivX title by index
/// @param Item      \b IN: Media item.
/// @param u32ID         \b IN: Index
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL MMSDK_SetTitle(MEDIAITEM Item, const MMSDK_U32 u32ID);

//-------------------------------------------------------------------------------------------------
/// [Divx] Get current DivX title
/// @param Item      \b IN: Media item.
/// @param pu32ID         \b OUT: Index
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL MMSDK_GetTitle(MEDIAITEM Item, MMSDK_U32 *pu32ID);

//-------------------------------------------------------------------------------------------------
/// [Divx] Set DivX Title by index
/// @param Item      \b IN: Media item.
/// @param u32ID         \b IN: Index
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL MMSDK_SetEdition(MEDIAITEM Item, const MMSDK_U32 u32ID);

//-------------------------------------------------------------------------------------------------
/// [Divx] Get current DivX edition
/// @param Item      \b IN: Media item.
/// @param pu32ID         \ b OUT: Index
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL MMSDK_GetEdition(MEDIAITEM Item, MMSDK_U32 *pu32ID);

//-------------------------------------------------------------------------------------------------
/// [Divx] Set DivX Chapter by index(default in file)
/// @param Item      \b IN: Media item.
/// @param u32ID         \b IN: Index
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL MMSDK_SetChapter(MEDIAITEM Item, const MMSDK_U32 u32ID);

//-------------------------------------------------------------------------------------------------
/// [Divx] Get current DivX chapter(default in file)
/// @param Item      \b IN: Media item.
/// @param pu32ID         \b OUT: Index
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL MMSDK_GetChapter(MEDIAITEM Item, MMSDK_U32 *pu32ID);

//-------------------------------------------------------------------------------------------------
/// [Divx] Get current DivX chapter (default in file) time by index
/// @param Item      \b IN: Media item.
/// @param u32ID         \b IN: Index
/// @param pu32IdTime         \b OUT: time of index
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL MMSDK_GetChapterTime(MEDIAITEM Item,const MMSDK_U32 u32ID ,MMSDK_U32 *pu32IdTime);

//------------------------------------------------------------------------------------------------
/// [Divx] Get DRM ST_DIVXDRM_INFO information.
/// @param Item          \b IN: Media item.
/// @param pstDrmInfo       \b OUT: Specify the pointer to write out ST_DIVXDRM_INFO.
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL MMSDK_GetDrmInfo(MEDIAITEM Item, ST_MMSDK_DIVXDRM_INFO * pstDrmInfo);

//-------------------------------------------------------------------------------------------------
/// [Divx] Set DivX auto-generation chapter by index
/// @param Item      \b IN: Media item.
/// @param u32ID         \b IN: Index
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL MMSDK_SetAutochapter(MEDIAITEM Item, MMSDK_U32 u32ID);

//-------------------------------------------------------------------------------------------------
/// [Divx] Get current DivX auto chapter time by index
/// @param Item      \b IN: Media item.
/// @param pu32ID         \b IN: Index
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL MMSDK_GetAutochapter(MEDIAITEM Item, MMSDK_U32 *pu32ID);

//-------------------------------------------------------------------------------------------------
/// [Divx] Get current DivX auto-generation chapter time by index
/// @param Item      \b IN: Media item.
/// @param u32ID         \b IN: Index
/// @param pu32IdTime         \b OUT: time of index
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL MMSDK_GetAutochapterTime(MEDIAITEM Item,const MMSDK_U32 u32ID ,MMSDK_U32 *pu32IdTime);

//-------------------------------------------------------------------------------------------------
/// [Divx] DivX Get information common API
/// @param Item          \b IN: Media item.
/// @param pstOperatePara         \b IN: Operate Parameter
/// @param pstDivxInfo               \b OUT: GET DIVX INFO
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL MMSDK_GetDivXInfo(MEDIAITEM Item, const ST_MMSDK_DIVX_OPERATE_PARAMETER * pstOperatePara, ST_MMSDK_DIVX_INFO * pstDivxInfo);
//=================  Divx Related API End =====================/




//=================  Download Module Related API  Begin =====================/
//-------------------------------------------------------------------------------------------------
///[Download Module] Set information to Download Module
/// @param pstDownloadModuleInfo          \b IN: Download Module Info
///
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL MMSDK_SetDownloadModuleInfo( const ST_MMSDK_DOWNLOAD_MODULE_INFO * pstDownloadModuleInfo);
//=================  Download Module Related API End =====================/


/** @} */ // end of mmsdk_interface

#ifdef __cplusplus
}
#endif
#endif
///*** please do not remove change list tag ***///
///***$Change: 1429000 $***///
