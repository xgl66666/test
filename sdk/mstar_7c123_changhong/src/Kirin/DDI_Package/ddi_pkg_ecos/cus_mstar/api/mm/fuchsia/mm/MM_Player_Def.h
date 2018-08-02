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
#ifndef __MM_PLAYER_DEF_H__
#define __MM_PLAYER_DEF_H__

#define DISABLE                                    0
#define ENABLE                                     1

//for Hummingbird 64M memory solution
//#define PLAYER_MEM_ALLOC_FROM_POOL
#ifdef PLAYER_MEM_ALLOC_FROM_POOL
#define VDPLAYER_MEM_ALLOC_FROM_POOL
#define PHOTO_MEM_ALLOC_FROM_POOL
#define MUSIC_MEM_ALLOC_FROM_POOL
#endif

#ifndef BUILD_LIB_FOR_DDI_USE
#define SUPPORT_ECOS_UNICODE_FS
#endif

#define OBA2    0
//#define MSOS_TYPE_LINUX
#define ENABLE_DMP                                 ENABLE
#define ENABLE_DIGITAL_MEDIA_API                   ENABLE
#define ENABLE_CARDREADER                          DISABLE
#define ENABLE_PTP                                 DISABLE
#define ENABLE_PVR                                 DISABLE
#define WATCH_DOG                                  DISABLE
#define ENABLE_WMA                                 ENABLE
#define ENABLE_SUBTITLE                            ENABLE //need check
#define ENABLE_LYRIC                               ENABLE
#define ENABLE_INETRNAL_SUBTITLE                   ENABLE
#define ENABLE_BABAO_PLAYER                        DISABLE
#define _VDPLAYER_DUMP_RAW_                        DISABLE
#define ENABLE_KTV                                 DISABLE
#define ENABLE_ZOOM                                ENABLE//ENABLE //need check
#define ENABLE_SLIDESHOW                           ENABLE//ENABLE //need check
#define Photo_Rotate                               ENABLE//ENABLE //need check
#define Photo_Thumbnail                            DISABLE//ENABLE //need check
#define ENABLE_PHOTONPP                            ENABLE//ENABLE //need check
#define ENABLE_PHOTO_3D                            ENABLE
#define BIN_ID_CODE_VDPLAYER                       0x0000  //only for compile
#ifdef  MSOS_TYPE_LINUX
#define _PA2VA(x) (MS_U32)MsOS_PA2KSEG1((x))
#else
#define _PA2VA(x) (MS_U32)MsOS_PA2KSEG1((x))
#endif
#define _VA2PA(x) (MS_U32)MsOS_VA2PA((x))

#define MPLAYER_TASK_DELAY_TIME                   5

//////////////////////Video & Subtitle ////////////////////////////////
#define DWIN_MOVIE_PREVIEW                              0
#define ENABLE_DIVX_PLUS                                0
#define ENABLE_DIVX_PLUS_TPT                            0
#define ENABLE_DIVX_TTF                                 0



/////////////////////////Music//////////////////////////////////////
#define ENABLE_LAST_MEMORY                               ENABLE
#define ENABLE_LAST_MEMORY_ENABLE_MUSIC                  0
#define ENABLE_MPLAYER_EXTRA_FLASH                       DISABLE

#define ENABLE_LYRICS3                                   1
#define ENABLE_LRC_LYRIC                                1

/////////////////////////////////////////////////////////////////////////
#define ENABLE_SUBTITLE_DMP                              1
#define ENABLE_MOVIE_PREVIEW                             1
#define ENABLE_WMDRMPD                                   0
#define ENABLE_WMV                                       0
#define ENABLE_DVD                                       0
#define ENABLE_GIF                                       0
#define ENABLE_TIFF                                      0

////////////////////////////////fs//////////////////////////////////////////
//#define FILE_SYSTEM_LONGLONG_API_ENABLE 0

#ifndef FILE_SYSTEM_LONGLONG_API_ENABLE
#define FILE_SYSTEM_LONGLONG_API_ENABLE 1
#endif

////////////////////////////////////////////////////////////////////////////

#define ENABLE_DTMB_CHINA_APP                             DISABLE
#define ENABLE_DVBC_PLUS_DTMB_CHINA_APP                   DISABLE
#define ENABLE_DVB_TAIWAN_APP                             DISABLE
#define ENABLE_SBTVD_BRAZIL_APP                           DISABLE
#define ENABLE_SBTVD_BRAZIL_CM_APP                        DISABLE
#define ENABLE_DVB_C_CHINA_APP                            DISABLE
#define ENABLE_ATV_CHINA_APP                              DISABLE

/////////////////////////////////////////////////////////////////////////////

typedef enum
{
    RETURN_IDLE_NO_ACTION                           = 0,
    RETURN_IDLE_GOTO_MENU,
    RETURN_IDLE_GOTO_CHANCHAN,
    RETURN_IDLE_GOTO_SOUNDTRACK,
    RETURN_IDLE_GOTO_VOLUMECONTROL,
    RETURN_IDLE_GOTO_STANDBY,
    RETURN_IDLE_GOTO_SCAN,

    RETURN_IDLE_GOTO_TT,
    RETURN_IDLE_GOTO_MMI,
    RETURN_IDLE_GOTO_A_IDLE,

    RETURN_A_IDLE_NO_ACTION,
    RETURN_A_IDLE_GOTO_A_IDLE,
    RETURN_A_IDLE_GOTO_MENU,
    RETURN_A_IDLE_GOTO_CHANCHAN,
    RETURN_A_IDLE_GOTO_STANDBY,
    RETURN_A_IDLE_GOTO_CLOSEDCAPTION,

    RETURN_MENU_NO_ACTION,
    RETURN_MENU_GOTO_STANDBY,
    RETURN_MENU_GOTO_SCAN,
    RETURN_MENU_END,
    RETURN_MENU_RETURN,
    RETURN_MENU_EXIT,
    RETURN_SUBMENU_NO_ACTION,
    RETURN_SUBMENU_END,
    RETURN_SUBMENU_EXIT,
    RETURN_SUBMENU_GOTO_STANDBY,
    RETURN_SUBMENU_GOTO_SCAN,
    RETURN_CHANNELCHANGE_NO_ACTION,
    RETURN_CHANNELCHANGE_ACTION_DONE,
    RETURN_CHANNELCHANGE_GOTO_VOLUMECONTROL,
    RETURN_CHANNELCHANGE_PROCESS_KEY,
    RETURN_CHANNELCHANGE_MENU_KEY,
    RETURN_CHANNELCHANGE_SOUNDTRACK_KEY,
    RETURN_CHANNELCHANGE_GOTO_UHF,
    RETURN_SCAN_NO_ACTION,
    RETURN_SCAN_EXIT_ACTION,
    RETURN_SCAN_ACTION_DONE,
    RETURN_STANDBY_NO_ACTION,
    RETURN_STANDBY_ENTER,
    RETURN_STANDBY_EXIT,
    RETURN_STANDBY_GOTO_CHANCHAN,
    RETURN_STANDBY_ACTION_DONE,
    RETURN_STANDBY_GOTO_UHF,
    RETURN_DIALOG_YES,
    RETURN_DIALOG_NO,

    RETURN_IDLE_GOTO_EPG,
    RETURN_EPGUI_NO_ACTION,
    RETURN_EPGUI_ACTION_DONE,
    RETURN_EPGUI_EXIT,
    RETURN_EPGUI_GOTO_STANDBY,

    RETURN_IDLE_GOTO_CLOSEDCAPTION,
    RETURN_CLOSEDCAPTION_NO_ACTION,
    RETURN_CLOSEDCAPTION_ACTION_DONE,

    RETURN_MMI_NO_ACTION,
    RETURN_MMI_EXIT,

    RETURN_GOTO_FACTORY_MENU,
    RETURN_FACTORY_NO_ACTION,
    RETURN_FACTORY_EXIT,
    RETURN_FACTORY_GOTO_STANDBY,
    RETURN_FACTORY_GOTO_HEATRUN,
#if 1 //def ENABLE_DMP
    EXIT_ANALOGINPUTS_TRAN_MEDIAPLAYER,
    EXIT_MPLAYER_NULL,
    EXIT_MPLAYER_EXIT,
    EXIT_MPLAYER_TRAN_STANDBY,
    EXIT_MPLAYER_TRAN_MENU,

    EXIT_PHOTO_DECODING,
    EXIT_PHOTO_DECODE_DONE,
    EXIT_PHOTO_DECODE_NO_THUMBNAIL,
    EXIT_PHOTO_DECODE_FAIL,
    EXIT_PHOTO_EXIT,
    EXIT_PHOTO_PREVIEW_EXIT,
    EXIT_PHOTO_PREVIEW_FAIL,
    EXIT_PHOTO_WAIT,

    EXIT_BMP_DECODING,
    EXIT_BMP_DECODE_DONE,
    EXIT_BMP_DECODE_FAIL,
    EXIT_BMP_EXIT,
    EXIT_BMP_PREVIEW_EXIT,
    EXIT_BMP_PREVIEW_GET_IMG_SIZE,
    EXIT_BMP_WAIT,
    EXIT_BMP_WAIT_PLAY_PREV,

    EXIT_PNG_DECODING,
    EXIT_PNG_DECODE_DONE,
    EXIT_PNG_DECODE_FAIL,
    EXIT_PNG_EXIT,
    EXIT_PNG_PREVIEW_EXIT,
    EXIT_PNG_PREVIEW_GET_IMG_SIZE,
    EXIT_PNG_WAIT,
    EXIT_PNG_WAIT_PLAY_PREV,

    EXIT_MP3_INIT,
    EXIT_MP3_DECODING,
    EXIT_MP3_EXIT,
    EXIT_MP3_TRAN_STANDBY,
    EXIT_MP3_WAIT,
    EXIT_MP3_WAIT_PLAY_PREV,

    EXIT_WMA_INIT,
    EXIT_WMA_DECODING,
    EXIT_WMA_EXIT,
    EXIT_WMA_TRAN_STANDBY,
    EXIT_WMA_WAIT,
    EXIT_WMA_WAIT_PLAY_PREV,

    EXIT_VDPLAYER_NULL,
    EXIT_VDPLAYER_PREDECODE_OK,
    EXIT_VDPLAYER_DECODING,
    EXIT_VDPLAYER_EXIT,
    EXIT_VDPLAYER_TRAN_STANDBY,
    EXIT_VDPLAYER_WAIT,
    EXIT_VDPLAYER_WAIT_PLAY_PREV,

    EXIT_MPEG2_NULL,
    EXIT_MPEG2_PREDECODE_OK,
    EXIT_MPEG2_EXIT,
    EXIT_MPEG2_WAIT,
    EXIT_MPEG2_INIT_DONE,
    EXIT_MPEG2_WAIT_PLAY_PREV,
    EXIT_MENU_EXIT_STORAGESELECT,

    EXIT_TS_NULL,
    EXIT_TS_PREDECODE_OK,
    EXIT_TS_EXIT,
    EXIT_TS_WAIT
#endif

} EN_RET;

#ifdef NEW_VDPLAYER
typedef enum
{
    /// Video codec type is unknown.
    E_MM_VIDEO_CODEC_UNKNOWN = -1,
    /// Video codec type is MPEG 4.
    E_MM_VIDEO_CODEC_MPEG4,
    /// Video codec type is motion JPG.
    E_MM_VIDEO_CODEC_MJPEG,
    /// Video codec type is H264.
    E_MM_VIDEO_CODEC_H264,
    /// Video codec type is RealVideo.
    E_MM_VIDEO_CODEC_RM,
    /// Video codec type is TS File.
    E_MM_VIDEO_CODEC_TS,
    /// Video codec type is MPEG 1/2.
    E_MM_VIDEO_CODEC_MPG,
    /// Video codec type is VC1.
    E_MM_VIDEO_CODEC_VC1,
    /// Video codec type is Audio Video Standard.
    E_MM_VIDEO_CODEC_AVS,
    /// Video codec type is FLV.
    E_MM_VIDEO_CODEC_FLV,
    /// Video codec type is MVC.
    E_MM_VIDEO_CODEC_MVC,
    /// Video codec type is VP6.
    E_MM_VIDEO_CODEC_VP6,
    /// Video codec type is VP8.
    E_MM_VIDEO_CODEC_VP8,
    /// Video codec type is HEVC.
    E_MM_VIDEO_CODEC_HEVC,
    /// Video codec type is VP9.
    E_MM_VIDEO_CODEC_VP9,
    /// Video codec number
    E_MM_VIDEO_CODEC_NUM,
}EN_MM_VIDEO_CODEC;

typedef enum
{
    /// Unkown
    E_MM_AUDIO_CODEC_UNKNOWN = -1,
    /// AAC
    E_MM_AUDIO_CODEC_AAC,
    /// AC3
    E_MM_AUDIO_CODEC_AC3,
    /// AC3 Plus
    E_MM_AUDIO_CODEC_AC3P,
    /// AMR NB
    E_MM_AUDIO_CODEC_AMR_NB,
    /// AMR WB
    E_MM_AUDIO_CODEC_AMR_WB,
    /// DTS
    E_MM_AUDIO_CODEC_DTS,
    /// FLAC
    E_MM_AUDIO_CODEC_FLAC,
    /// MP3
    E_MM_AUDIO_CODEC_MP3,
    /// MP2
    E_MM_AUDIO_CODEC_MPEG,
    /// COOK
    E_MM_AUDIO_CODEC_COOK,
    /// COOK RA8LBR
    E_MM_AUDIO_CODEC_RA8LBR,
    /// Vorbis
    E_MM_AUDIO_CODEC_VORBIS,
    /// WMA1 and WMA2
    E_MM_AUDIO_CODEC_WMA,
    /// WMA1
    E_MM_AUDIO_CODEC_WMA1,
    /// WMA2
    E_MM_AUDIO_CODEC_WMA2,
    /// WMA3 (PRO)
    E_MM_AUDIO_CODEC_WMA3,
    /// WMA3 (PRO)
    E_MM_AUDIO_CODEC_WMA_PRO,
    /// Unknown PCM type...
    E_MM_AUDIO_CODEC_XPCM,
    /// PCM
    E_MM_AUDIO_CODEC_PCM,
    /// ADCPM
    E_MM_AUDIO_CODEC_ADPCM,
    /// audio codec number
    E_MM_AUDIO_CODEC_NUM,
}EN_MM_AUDIO_CODEC;
#endif
#endif
