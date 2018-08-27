//<MStar Software>
//******************************************************************************
// MStar Software
// Copyright (c) 2010 - 2015 MStar Semiconductor, Inc. All rights reserved.
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

#ifndef __mmsdk_porting_downloadurl__
#define __mmsdk_porting_downloadurl__
#include "mmsdk_interface_def.h"

#ifdef __cplusplus
extern "C"
{
#endif

/// download module instance
typedef struct _client_instance ST_CLIENT_INSTANCE;
/// download module url info
typedef struct _client_handle_info ST_CLIENT_HANDLE_INFO;
/// download module buffer manager info
typedef struct _ClientBufferInfo ST_ClientBufferInfo;
/// callback function for push ES data
typedef MMSDK_BOOL(*ClientPushModeReadFunc)(int nStreamType, const MMSDK_U8* pu8Buff, MMSDK_U32 uReadSize, MMSDK_U32 u32TimeStamp , void* pUserData);
/// callback function for notify download module event.
typedef MMSDK_BOOL(*ClientEventNotifyFunc)(int nEvent, void* pUserData);

typedef struct
{
    /// Initialize all related library, internal data structure.
    MMSDK_BOOL(*Client_Init)(const MMSDK_U32 u32Version);
    /// Uinitialize all related library, free all allocated memory, close all opened sessions.
    MMSDK_BOOL(*Client_Uninit)(void);
    /// Build a session through the url.
    ST_CLIENT_INSTANCE* (*Client_Open)(const MMSDK_U8* url, MMSDK_U32 *pnHttpStatusCode, ST_ClientBufferInfo *pstBufferInfo);
    /// Close the specific opened session.
    MMSDK_BOOL(*Client_Close)(ST_CLIENT_INSTANCE* pstClient);
    /// Get how many sessions do you open?
    MMSDK_U32(*Client_Get_SessionCount)(void);
    /// Get the session information. EX : protocol type, streaming mode, content length, etc.
    MMSDK_BOOL(*Client_Get_HandleInfo)(ST_CLIENT_INSTANCE* pstClient, ST_CLIENT_HANDLE_INFO *pstHandleInfo);
    /// Set the download bandwidth
    MMSDK_BOOL(*Client_Set_NetworkBandwidth)(ST_CLIENT_INSTANCE* pstClient, MMSDK_S64 u64NetworkBandwidth);
    /// Get the data from Server, this function only works with E_CLIENT_STREAM_PULL_MODE
    MMSDK_U32(*Client_PullMode_Read)(ST_CLIENT_INSTANCE* pstClient, MMSDK_U8* pu8Buff, MMSDK_U32 u32RequestSize, MMSDK_BOOL bWaitFillData, MMSDK_BOOL *pbIsEOF);
    /// Seek from Server, this function only works with E_CLIENT_STREAM_PULL_MODE
    MMSDK_BOOL(*Client_Byte_Seek)(ST_CLIENT_INSTANCE* pstClient, MMSDK_U64 u64FileSeekPos);
    /// Get the current offset, this function only works with E_CLIENT_STREAM_PULL_MODE
    MMSDK_U64(*Client_Get_CurrentOffset)(ST_CLIENT_INSTANCE* pstClient);
    /// Set push mode callback to push data
    MMSDK_BOOL(*Client_Set_PushMode_ReadCallback)(ST_CLIENT_INSTANCE* pstClient, ClientPushModeReadFunc pFunc, void* pUserData);
    /// Set notify callback
    MMSDK_BOOL(*Client_Set_EventNotifyCallback)(ST_CLIENT_INSTANCE* pstClient, ClientEventNotifyFunc pFunc, void* pUserData, MMSDK_BOOL bReviseItem);
    /// Get video codec info, for push mode
    MMSDK_BOOL(*Client_Get_VideoInfo)(ST_CLIENT_INSTANCE* pstClient, ST_MMSDK_ES_VIDEO_INFO* stVideoInfo);
    /// Get audio codec info, for push mode
    MMSDK_BOOL(*Client_Get_AudioInfo)(ST_CLIENT_INSTANCE* pstClient, ST_MMSDK_ES_AUDIO_INFO* stAudioInfo);
    /// Get the data from Server with the timestamp which you want to seek
    MMSDK_BOOL (*Client_Time_Seek)(ST_CLIENT_INSTANCE* pstClient, MMSDK_U64 u64TimeStamp);
    /// Stop Download Module
    MMSDK_BOOL (*Client_Stop_Connect)(void);
    void* pFptrReserved1;
    void* pFptrReserved2;
    void* pFptrReserved3;
    void* pFptrReserved4;
    void* pFptrReserved5;
    void* pFptrReserved6;
    void* pFptrReserved7;
    void* pFptrReserved8;
    void* pFptrReserved9;
    void* pFptrReserved10;
} ST_MMSDK_DOWNLOAD_URL_PLUGIN;

//-------------------------------------------------------------------------------------------------
/// Get Download Module Plugin.
/// @param pstUrlHandle       \b OUT: return download module function pointers list
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL PT_DownloadUrl_GetPlugin(ST_MMSDK_DOWNLOAD_URL_PLUGIN* pstUrlHandle) __attribute__((weak)) ;
//-------------------------------------------------------------------------------------------------
/// If using dynamic loading, release resource here.
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL PT_DownloadUrl_ReleasePlugin(void) __attribute__((weak)) ;
#ifdef __cplusplus
}
#endif
#endif // __mmsdk_porting_downloadurl__
///*** please do not remove change list tag ***///
///***$Change: 1425021 $***///
