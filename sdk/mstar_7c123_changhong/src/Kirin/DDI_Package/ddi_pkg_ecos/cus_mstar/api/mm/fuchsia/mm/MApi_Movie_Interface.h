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
// Copyright (c) 2006-2009 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (ˇ§MStar Confidential Informationˇ¨) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __MAPI_MOVIE_INTERFACE_H__
#define __MAPI_MOVIE_INTERFACE_H__
#include "mapp_videoplayer.h"
#ifdef NEW_VDPLAYER
#include "mmsdk_interface_def.h"
#endif
#ifdef __MAPI_MOVIE_INTERFACE_C__
#define INTERFACE
#else
#define INTERFACE extern
#endif

#ifdef NEW_VDPLAYER
typedef enum
{
    E_MOVIE_PLAY_STATE_NULL                          = 0,
    E_MOVIE_PLAY_STATE_INIT_OK                       = (1 << 0),
    E_MOVIE_PLAY_STATE_INIT_FAIL                     = (1 << 1),
    E_MOVIE_PLAY_STATE_INVALID_FILE                  = (1 << 2),
    E_MOVIE_PLAY_STATE_THUMBNAIL_DONE                = (1 << 3),
    E_MOVIE_PLAY_STATE_THUMBNAIL_ERROR               = (1 << 4),
    E_MOVIE_PLAY_STATE_DECODE_ERROR                  = (1 << 5),
    E_MOVIE_PLAY_STATE_DECODE_DONE                   = (1 << 6),
    E_MOVIE_PLAY_STATE_DECODE_FIRST_FRAME_DONE       = (1 << 7),
    E_MOVIE_PLAY_STATE_DECODE_BACKWARD_TO_FILE_START = (1 << 8),
    E_MOVIE_PLAY_STATE_INTERNAL_SUBTITLE_TYPE_TEXT   = (1 << 9),
    E_MOVIE_PLAY_STATE_PROGRAM_CHANGED               = (1 << 10),
    E_MOVIE_PLAY_STATE_AUDIO_TRACK_CHANGED           = (1 << 11),
    E_MOVIE_PLAY_STATE_DEMUX_HEADER_OK               = (1 << 12),
    E_MOVIE_PLAY_STATE_DEMUX_HEADER_FAIL             = (1 << 13),
    E_MOVIE_PLAY_STATE_AUDIO_UNSUPPORTED             = (1 << 14),
    E_MOVIE_PLAY_STATE_NOT_SUPPORT                   = (1 << 15),
    E_MOVIE_PLAY_STATE_PLAYING_OK                    = (1 << 16),
    E_MOVIE_PLAY_STATE_AUDIO_ONLY                    = (1 << 17),
    E_MOVIE_PLAY_STATE_SETPLAYPOSITION_ACK           = (1 << 18),
    E_MOVIE_PLAY_STATE_SETTRICKMODE_ACK              = (1 << 19),
}EN_MOVIE_PLAY_STATE;

/// DivX Operate and Get info (divx plus 1.1.1)
typedef enum
{
    /// none
    E_MOVIE_PLAY_DIVX_NONE = 0,

    //Get info
    //MKV feature
    /// Title info with auto generate title name.
    /// if ((ST_MMSDK_DIVX_INFO.u32Reserved & BIT0) == BIT0) means has ST_MMSDK_DIVX_INFO.strName info
    /// else auto generate name info
    E_MOVIE_PLAY_DIVX_GET_TITLE_INFO = 1,
    /// Edition info with auto generate edition name.
    /// if ((ST_MMSDK_DIVX_INFO.u32Reserved & BIT0) == BIT0) means has ST_MMSDK_DIVX_INFO.strName info
    /// else auto generate name info
    E_MOVIE_PLAY_DIVX_GET_EDITION_INFO = 2,
    /// chapter info with auto generate chapter name.
    /// if ((ST_MMSDK_DIVX_INFO.u32Reserved & BIT0) == BIT0) means has ST_MMSDK_DIVX_INFO.strName info
    /// else auto generate name info
    E_MOVIE_PLAY_DIVX_GET_CHAPTER_INFO= 3,
    /// auto-chapter info with auto generate auto-chapter name.
    /// if ((ST_MMSDK_DIVX_INFO.u32Reserved & BIT0) == BIT0) means has ST_MMSDK_DIVX_INFO.strName info
    /// else auto generate name info
    E_MOVIE_PLAY_DIVX_GET_AUTOCHAPTER_INFO =4,

    /// get title info without auto generate title name.
    /// if ((ST_MMSDK_DIVX_INFO.u32Reserved & BIT0) == BIT0) means has ST_MMSDK_DIVX_INFO.strName info
    E_MOVIE_PLAY_DIVX_GET_TITLE_INFO_NOAUTOGEN = 5,
    /// get edition info without auto generate editon name.
    /// if ((ST_MMSDK_DIVX_INFO.u32Reserved & BIT0) == BIT0) means has ST_MMSDK_DIVX_INFO.strName info
    E_MOVIE_PLAY_DIVX_GET_EDITION_INFO_NOAUTOGEN = 6,
    /// get chapter info without auto generate chapter name.
    /// if ((ST_MMSDK_DIVX_INFO.u32Reserved & BIT0) == BIT0) means has ST_MMSDK_DIVX_INFO.strName info
    E_MOVIE_PLAY_DIVX_GET_CHAPTER_INFO_NOAUTOGEN= 7,
    /// get auto-chapter info  without auto generate name.
    /// if ((ST_MMSDK_DIVX_INFO.u32Reserved & BIT0) == BIT0) means has ST_MMSDK_DIVX_INFO.strName info
    E_MOVIE_PLAY_DIVX_GET_AUTOCHAPTER_INFO_NOAUTOGEN =8,

    /// get audio info with auto generate audio name.
    /// if ((ST_MMSDK_DIVX_INFO.u32Reserved & BIT0) == BIT0) means has ST_MMSDK_DIVX_INFO.strName info
    /// else auto generate name info
    E_MOVIE_PLAY_DIVX_GET_AUDIO_INFO= 50,
    /// get audio info without auto generate audio name.
    /// if ((ST_MMSDK_DIVX_INFO.u32Reserved & BIT0) == BIT0) means has ST_MMSDK_DIVX_INFO.strName info
    E_MOVIE_PLAY_DIVX_GET_AUDIO_INFO_NOAUTOGEN,

    /// get subtitle info with auto generate subtitle name.
    /// if ((ST_MMSDK_DIVX_INFO.u32Reserved & BIT0) == BIT0) means has ST_MMSDK_DIVX_INFO.strName info
    /// else auto generate name info
    E_MOVIE_PLAY_DIVX_GET_SUBTITLE_INFO=60,
    /// get subtitle info without auto generate subtitle name.
    /// if ((ST_MMSDK_DIVX_INFO.u32Reserved & BIT0) == BIT0) means has ST_MMSDK_DIVX_INFO.strName info
    E_MOVIE_PLAY_DIVX_GET_SUBTITLE_INFO_NOAUTOGEN,
} EN_MOVIE_PLAY_DIVX_OPERATE;

/// DivX Language (divx plus 1.1.1)
typedef enum
{
    /// undefine
    E_MOVIE_PLAY_DIVX_UNDEFINE = 0,
    /// english
    E_MOVIE_PLAY_DIVX_ENGLISH,
    /// chinese
    E_MOVIE_PLAY_DIVX_CHINESE,
    /// korea
    E_MOVIE_PLAY_DIVX_KOREA,
    /// japan
    E_MOVIE_PLAY_DIVX_JAPAN,
    /// french
    E_MOVIE_PLAY_DIVX_FRENCH,
    /// german
    E_MOVIE_PLAY_DIVX_GERMANIC,
    /// spanish
    E_MOVIE_PLAY_DIVX_SPANISH,
    /// italian
    E_MOVIE_PLAY_DIVX_ITALIAN,
    /// greek
    E_MOVIE_PLAY_DIVX_GREEK,
    /// portuguese
    E_MOVIE_PLAY_DIVX_PORTUGUESE,
    /// russian
    E_MOVIE_PLAY_DIVX_RUSSIAN,
    /// polish
    E_MOVIE_PLAY_DIVX_POLISH,
    /// swedish
    E_MOVIE_PLAY_DIVX_SWEDISH,
    /// norwegian
    E_MOVIE_PLAY_DIVX_NORWEGIAN,
    /// total language number
    E_MOVIE_PLAY_DIVX_NUM,
} EN_MOVIE_PLAY_DIVX_LANG;

/// Divx MKV meta info (divx plus 1.1.1)
typedef struct
{
    /// Operate Type
    EN_MOVIE_PLAY_DIVX_OPERATE eOperateType;
    /// OSD language
    EN_MOVIE_PLAY_DIVX_LANG eOSDLanguage;
    /// operate index
    MS_U32 u32Idx;
} ST_MOVIE_PLAY_DIVX_OPERATE_PARAMETER;


/// Divx MKV meta info (divx plus 1.1.1)
typedef struct
{
    /// law rating
    char strLawRating[MMSDK_DIVX_LAWRATING_SIZE];
    /// name string
    char strName[MMSDK_DIVX_METADATA_NAME_SIZE];
    /// Total num
    MS_U32 u32TotalNum;
    /// Current index
    MS_U32 u32CurrentIdx;
    /// audio channel number
    MS_U32 u32ChannelConfiguration;
    /// Reserved
    MS_U32 u32Reserved;
} ST_MOVIE_PLAY_DIVX_INFO;

typedef enum
{
    /// auto
    E_MOVIE_ASPECT_RATIO_AUTO = 0,
    /// 4x3
    E_MOVIE_ASPECT_RATIO_4X3,
    /// 16x9
    E_MOVIE_ASPECT_RATIO_16X9,
    /// 4x3 pan & scan
    E_MOVIE_ASPECT_RATIO_4X3_PAN_SCAN,
    /// 16x9 pan & scan
    E_MOVIE_ASPECT_RATIO_16X9_PAN_SCAN,
    /// 4x3 letter box
    E_MOVIE_ASPECT_RATIO_4X3_LETTER_BOX,
    /// 16x9 pillar box
    E_MOVIE_ASPECT_RATIO_16X9_PILLARBOX,
}EN_MOVIE_ASPECT_RATIO;

typedef struct
{
    MS_U64 u64FilePos;
    MS_U32 u32TimeInMs;
    MS_U8 u8TitleID;
    MS_U8 u8EditionID;
    MS_U8 u8AudioID;
    MS_U8 u8SubtitleID;
    MS_U8 u8MoviePlayName[MAX_PATH_NAME_LEN];
    MS_BOOL bResumePlay;
}ST_MOVIE_RESUME_INFO;

typedef struct
{
    /// Teletext languge.
    MS_U32 u32Language;
    /// Teletext track index.
    MS_U8 u8TrackID;
    /// Teletext track count.
    MS_U8 u8TrackCount;
    /// Teletext type.
    MS_U8 u8Type;
    /// Teletext magazine number.
    MS_U8 u8Magazine;
    /// Teletext page number.
    MS_U8 u8Page;
} ST_MOVIE_TTX_TRACK_INFO;

typedef enum
{
    E_MOVIE_DRM_OP_NONE                   = 0x00000000,
    E_MOVIE_DRM_OP_CGMS_COPY_FREE         = 0x00000001,
    E_MOVIE_DRM_OP_CGMS_COPY_NO_MORE      = 0x00000002,
    E_MOVIE_DRM_OP_CGMS_COPY_ONCE         = 0x00000004,
    E_MOVIE_DRM_OP_CGMS_COPY_NEVER        = 0x00000008,
    E_MOVIE_DRM_OP_ACP_AGC                = 0x00000010,
    E_MOVIE_DRM_OP_ACP_AGC_2LINE_COLOR    = 0x00000020,
    E_MOVIE_DRM_OP_ACP_AGC_4LINE_COLOR    = 0x00000040,
    E_MOVIE_DRM_OP_HDCP                   = 0x00000080,
    E_MOVIE_DRM_OP_ICT                    = 0x00000100,
    E_MOVIE_DRM_OP_UNKNOW                 = 0x00000200,
} EN_MOVIE_OUTPUT_PROTECT_FLAG;

/// DivX DRM Info
typedef struct
{
    /// DivX DRM Renta lCode
    MS_U32 u32DrmRentalCode;
    /// DivX DRM Rental File or not
    MS_U32 u32DrmRentalFile;
    /// DivX DRM number of Rental Limit
    MS_U32 u32DrmRentalLimit;
    /// DivX DRM number of Rental Use Count
    MS_U32 u32DrmRentalUseCount;
    /// DivX DRM output protection.
    EN_MOVIE_OUTPUT_PROTECT_FLAG eOutputProtectFlg;
    /// DivX DRM Check Sum for store in EEPROM
    MS_U16 u16CheckSum;
    /// DivX DRM RentalSatus
    MS_U8 u8DrmRentalStatus;
    /// DivX DRM File Format
    MS_U8 u8DrmFileFormat;
    /// DivX DRM Authrization Satus
    MS_U8 u8DrmAuthorization;
    /// DivX DRM Reg Code
    MS_U8 u8RegistrationCode[MMSDK_DIVX_REG_CODE_LEN+1];
    /// DivX DRM Activation Code
    MS_U8 u8DeActivationCode[MMSDK_DIVX_DEACT_CODE_LEN+1];
    /// DivX DRM Key Data
    MS_U8 u8DRM_Data[MMSDK_DIVX_KEY_LEN];
    /// DivX DRM Key Generated or not
    MS_U8 bIsKeyGenerated;
    /// DivX DRM activated or not
    MS_U8 bIsActivated;
    /// DivX DRM deactivated or not
    MS_U8 bIsDeactivated;
    /// DivX DRM to clear last memory when deactivated.
    MS_U8 bClearLastMemory;
} ST_MOVIE_DIVXDRM_INFO;
#else
typedef enum
{
    E_MOVIE_PLAYER_STATE_INIT,
    E_MOVIE_PLAYER_STATE_DECODING,
    //E_MOVIE_PLAYER_STATE_FW_END,
    E_MOVIE_PLAYER_STATE_SUCCESS_EXIT,
    E_MOVIE_PLAYER_STATE_ERROR_EXIT,
    E_MOVIE_PLAYER_STATE_NONE,
}EN_MOVIE_PLAYER_STATE;
#endif

typedef enum
{
    E_MOVIE_PLAY_MODE_NORMAL_PLAY = 0,    // Normal Play Forward, it will do avsync control.
    E_MOVIE_PLAY_MODE_PAUSE,              // Pause Play
    E_MOVIE_PLAY_MODE_FAST_FORWARD,       // Fast Motion Forward.
    E_MOVIE_PLAY_MODE_SCAN_BACKWARD,      // Fast Play Backward

    E_MOVIE_PLAY_MODE_STOP,
    E_MOVIE_PLAY_MODE_INVALID,
    E_MOVIE_PLAY_MODE_NUM,
} EN_MOVIE_PLAY_MODE;


#ifdef NEW_VDPLAYER
INTERFACE MS_BOOL MApi_MoviePlayer_SetTtxOnOff(MS_BOOL bEnable);
INTERFACE MS_BOOL MApi_MoviePlayer_SetTtxTrackID(MS_U8 u8TrackId);
INTERFACE MS_U32 MApi_MoviePlayer_GetTtxTrackLanguage(MS_U8 u8SubtitleTrackId);
INTERFACE void MM_Scaler_EnableMADiForceMotion(MS_BOOL Enable);
INTERFACE MS_BOOL MApi_Divx_GetDivXInfo(const ST_MOVIE_PLAY_DIVX_OPERATE_PARAMETER *pstOperatePara, ST_MOVIE_PLAY_DIVX_INFO *pstDivxInfo);
INTERFACE MS_BOOL MApi_Divx_GetAutochapter(MS_U32 *pu32ID);
INTERFACE MS_BOOL MApi_Divx_GetChapter(MS_U32 *pu32ID);
INTERFACE MS_BOOL MApi_Divx_GetAutochapterTime(MS_U32 u32ID ,MS_U32 *pu32ID);
INTERFACE MS_BOOL MApi_Divx_GetChapterTime(MS_U32 u32ID ,MS_U32 *pu32ID);
INTERFACE MS_BOOL MApi_Divx_SetChapter(MS_U32 u32ID);
INTERFACE MS_BOOL MApi_Divx_SetAutochapter(MS_U32 u32ID);
INTERFACE MS_BOOL MApi_Divx_GetTitle(MS_U32 *pu32ID);
INTERFACE MS_BOOL MApi_Divx_SetTitle(MS_U32 u32ID);
INTERFACE MS_BOOL MApi_Divx_GetEdition(MS_U32 *pu32ID);
INTERFACE MS_BOOL MApi_Divx_SetEdition(MS_U32 u32ID);
INTERFACE MS_BOOL MApi_MoviePlayer_Delete(void);
INTERFACE MS_BOOL MApi_MoviePlayer_Play(void);
INTERFACE MS_BOOL MApi_MoviePlayer_Standby(void);
INTERFACE void MApi_MoviePlayer_FullStop(void);
INTERFACE EN_MM_VIDEO_CODEC MApi_MoviePlayer_GetMovieFileType(void);
INTERFACE EN_MM_AUDIO_CODEC MApi_MoviePlayer_GetMovieAudioType(void);
INTERFACE MS_U32 MApi_MoviePlayer_GetVideoInfo(EN_VDPLAYER_INFO enInfo);
INTERFACE MS_BOOL MApi_MoviePlayer_GotoMs(MS_U32 u32Gotoms);
INTERFACE MS_BOOL MApi_MoviePlayer_SetAudioTrackID(MS_U8 u8TrackId);
INTERFACE MS_BOOL MApi_MoviePlayer_GetAudioTrackID(MS_U8 *u8TrackId);
INTERFACE MS_U32 MApi_MoviePlayer_GetAudioTrackLanguage(MS_U8 u8AudioTrackId);
INTERFACE MS_BOOL MApi_MoviePlayer_SetSubtitleTrackID(MS_U8 u8TrackId);
INTERFACE MS_BOOL MApi_MoviePlayer_GetSubtitleTrackID(MS_U8 *u8TrackId);
INTERFACE MS_BOOL MApi_MoviePlayer_SetSubtitleOnOff(MS_BOOL bEnable);
INTERFACE MS_U32 MApi_MoviePlayer_GetSubtitleTrackLanguage(MS_U8 u8SubtitleTrackId);
INTERFACE MS_U32 MApi_MoviePlayer_GetCurrentInternalSubttileType(void);
INTERFACE MS_BOOL MApi_MoviePlayer_SetAspectRatio(EN_MOVIE_ASPECT_RATIO enRatio);
INTERFACE MS_BOOL MApi_MoviePlayer_SetDisplayRecangle(MS_BOOL bFullScreen, MS_BOOL bKeepRatio, MM_RECT *pRect, MS_BOOL bBlackScreen);
INTERFACE MS_BOOL MApi_MoviePlayer_SetPlayState(EN_MOVIE_PLAY_STATE enMoviePlayState);
INTERFACE MS_BOOL MApi_MoviePlayer_CheckPlayState(EN_MOVIE_PLAY_STATE enMoviePlayState);
INTERFACE MS_BOOL MApi_MoviePlayer_RemovePlayState(EN_MOVIE_PLAY_STATE enMoviePlayState);
INTERFACE MS_BOOL MApi_MoviePlayer_GetDrmInfo(ST_MOVIE_DIVXDRM_INFO * pstDrmInfo);
INTERFACE void MApi_MoviePlayer_LoadResumeInfo(ST_MOVIE_RESUME_INFO *pstMovieResumeInfo);
INTERFACE void MApi_MoviePlayer_SaveResumeInfo(ST_MOVIE_RESUME_INFO *pstMovieResumeInfo);
INTERFACE MS_BOOL MApi_MoviePlayer_SlowForward(EN_PLAY_SPEED enPlaySpeed);
INTERFACE MS_BOOL MApi_MoviePlayer_SlowBackward(EN_PLAY_SPEED enPlaySpeed);
#else
INTERFACE MS_BOOL MApi_MoviePlay_Delete(void);
INTERFACE MS_BOOL MApp_MoviePlay_Standby(void);
INTERFACE MS_BOOL MApp_MoviePlay_ProcessHandle(MS_U8 u8Cmd);
INTERFACE VDPLAYER_VIDEO_TYPE MApi_MoviePlayer_GetMovieFileType(void);
INTERFACE U32 MApi_MoviePlayer_GetVideoInfo(EN_VDPLAYER_INFO enInfo);
INTERFACE U32 MApi_MoviePlayer_GetMovieAudioType(void);
INTERFACE MS_BOOL MApi_MoviePlayer_GotoMs(U32 u32Gotoms);
INTERFACE MS_BOOL MApp_MoviePlay_GetRet(EN_RET* pePlayRet);
INTERFACE MS_BOOL MApp_MoviePlay_SetRet(EN_RET pePlayRet);
INTERFACE void MApi_VDPlayer_Release(void);
INTERFACE void MM_Scaler_EnableMADiForceMotion(MS_BOOL Enable);
#endif

INTERFACE MS_BOOL MApi_MoviePlayer_Init(void);
INTERFACE MS_BOOL MApi_MoviePlayer_Start(char *OpenFileName);
INTERFACE MS_BOOL MApi_MoviePlayer_FF(EN_PLAY_SPEED enPlaySpeed);
INTERFACE MS_BOOL MApi_MoviePlayer_FB(EN_PLAY_SPEED enPlaySpeed);
INTERFACE MS_BOOL MApi_MoviePlayer_Pause(void);
INTERFACE MS_BOOL MApi_MoviePlayer_Resume(void);
INTERFACE void MApi_MoviePlayer_Stop(void);
INTERFACE void MApi_MoviePlayer_StopAndStopDisplay(void);
INTERFACE TRICK_STATUS MApi_MoviePlayer_GetTrickMode(void);

INTERFACE MS_BOOL MApi_MoviePlayer_RepeatAB(MS_U16 *eRepeatAB);



#undef INTERFACE

#endif
