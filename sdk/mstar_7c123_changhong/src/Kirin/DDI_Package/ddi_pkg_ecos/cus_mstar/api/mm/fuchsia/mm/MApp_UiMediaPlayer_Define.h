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
// (¡§MStar Confidential Information¡¨) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
/// @file MApp_UiMediaPlayer_Define.h
/// @brief Definitions for media player kernel
/// @author MStar Semiconductor Inc.
///
/// Define the compiling time parameters which can be defined by users.
/// Users can re-define them to change the media player kernel configuration
///
////////////////////////////////////////////////////////////////////////////////

#ifndef MAPP_UI_MEDIAPLAYER_DEFINE_H
#define MAPP_UI_MEDIAPLAYER_DEFINE_H

// ZUI #include "MApp_UiMenuTbl.h"
//#include "Panel.h"
#include "MM_Player_Def.h"


#define ENABLE_MPLAYER_PHOTO            1
#define ENABLE_MPLAYER_MUSIC            1
#define ENABLE_MPLAYER_MOVIE            1
#define ENABLE_MPLAYER_TEXT             1
#if (!ENABLE_MPLAYER_PHOTO) && (!ENABLE_MPLAYER_MUSIC) && (!ENABLE_MPLAYER_MOVIE) && (!ENABLE_MPLAYER_TEXT)
#error "Have to enable at least one media type!"
#endif

#define VIDEO_CAPTURE_WIDTH         1024L
#define VIDEO_CAPTURE_HEIGHT        768L

//only for no os
#define ENABLE_FILE_INFO_LONG_FILENAME_SIZE_INCREASE             1//more 128 byte in longfilename size

//*****************************************************************************
//              Defines
//*****************************************************************************
#define STRING_CODE_GB                  1
#define STRING_CODE_ASCII               2


#define NUM_OF_DRIVE_PER_PAGE           7


#define ENABLE_SELECT_FILE_ACROSS_DIRECTORY     1

#define NEWUI_MEDIA_PLAYER_PREVIEW_BG_START_X   (0+20)//38+ PANEL_WIDTH/2+100  //brian test
#define NEWUI_MEDIA_PLAYER_PREVIEW_BG_START_Y   (0+40)//(UI_MEDIA_PLAYER_LIST_TYPE_START_Y+UI_MEDIA_PLAYER_LIST_TYPE_HEIGHT+15) //71 //152
#define NEWUI_MEDIA_PLAYER_PREVIEW_BG_WIDTH     (100-40)//394
#define NEWUI_MEDIA_PLAYER_PREVIEW_BG_HEIGHT    (100-80)//247
#define NEWUI_MEDIA_PLAYER_PREVIEW_X            NEWUI_MEDIA_PLAYER_PREVIEW_BG_START_X
#define NEWUI_MEDIA_PLAYER_PREVIEW_Y            NEWUI_MEDIA_PLAYER_PREVIEW_BG_START_Y
#define NEWUI_MEDIA_PLAYER_PREVIEW_WIDTH        NEWUI_MEDIA_PLAYER_PREVIEW_BG_WIDTH
#define NEWUI_MEDIA_PLAYER_PREVIEW_HEIGHT       NEWUI_MEDIA_PLAYER_PREVIEW_BG_HEIGHT

// Define preview parameters
#define PHOTO_PREVIEW_DELAY_TIME_MS             1500
#define MOVIE_PREVIEW_DELAY_TIME_MS             2500
#define MUSIC_PREVIEW_DELAY_TIME_MS             1500
#define TEXT_PREVIEW_DELAY_TIME_MS              1500

#define ZUI_DEFAULT_PANEL_WIDTH                 1366
#define ZUI_DEFAULT_PANEL_HEIGHT                 768

//the preview window setting  is based on 1366x768 panel
#if ENABLE_DMP
#define PREIVEW_WND_DEFAULT_X                   458
#define PREIVEW_WND_DEFAULT_Y                   259
#define PREIVEW_WND_DEFAULT_WIDTH               451
#define PREIVEW_WND_DEFAULT_HEIGHT              251
#else
#define PREIVEW_WND_DEFAULT_X                   780
#define PREIVEW_WND_DEFAULT_Y                   150
#define PREIVEW_WND_DEFAULT_WIDTH               548
#define PREIVEW_WND_DEFAULT_HEIGHT              290
#endif

#define PHOTO_PREVIEW_X                         ((PREIVEW_WND_DEFAULT_X*(g_IPanel.Width()))/ZUI_DEFAULT_PANEL_WIDTH)
#define PHOTO_PREVIEW_Y                         ((PREIVEW_WND_DEFAULT_Y*(g_IPanel.Height()))/ZUI_DEFAULT_PANEL_HEIGHT)
#define PHOTO_PREVIEW_WIDTH                     ((PREIVEW_WND_DEFAULT_WIDTH*(g_IPanel.Width()))/ZUI_DEFAULT_PANEL_WIDTH)
#define PHOTO_PREVIEW_HEIGHT                    ((PREIVEW_WND_DEFAULT_HEIGHT*(g_IPanel.Height()))/ZUI_DEFAULT_PANEL_HEIGHT)

#define MOVIE_PREVIEW_X                         PHOTO_PREVIEW_X
#define MOVIE_PREVIEW_Y                         PHOTO_PREVIEW_Y
#define MOVIE_PREVIEW_WIDTH                     PHOTO_PREVIEW_WIDTH
#define MOVIE_PREVIEW_HEIGHT                    PHOTO_PREVIEW_HEIGHT
#define TEXT_PREVIEW_X                          PHOTO_PREVIEW_X
#define TEXT_PREVIEW_Y                          PHOTO_PREVIEW_Y
#define TEXT_PREVIEW_WIDTH                      PHOTO_PREVIEW_WIDTH
#define TEXT_PREVIEW_HEIGHT                     PHOTO_PREVIEW_HEIGHT

#define ENABLE_MOVIE_PREVIEW_ALL_AND_1ST_FRAME  1
#define ENABLE_MOVIE_PREVIEW_1ST_FRAME          0
#define ENABLE_MOVIE_PREVIEW_1ST_FRAME_POS_MS   2000
#define ENABLE_MOVIE_THUMBNAIL_1ST_FRAME_SHOW_TIMER 200

#define NUM_OF_SUPPORT_DEVICES                  2   // USB host + card reader
#define NUM_OF_MAX_USB_LUN                      4
#define NUM_OF_MAX_DRIVE_PER_LUN                8
#define NUM_OF_MAX_DRIVE                        16

#define DEFAULT_LYRIC_CODE                      STRING_CODE_GB
#define DEFAULT_SUBTITLE_CODE                   STRING_CODE_GB

// Define media file extension name
#define BMP_EXT_FILENAME        "BMP"
#define JPG_EXT_FILENAME        "JPG", "JPE"
#define MJPG_EXT_FILENAME       "AVI"
#define PNG_EXT_FILENAME        "PNG"
#define TIFF_EXT_FILENAME        "TIF"
#define GIF_EXT_FILENAME        "GIF"
#define MPEG_EXT_FILENAME       "MPG", "MPE", "VOB", "DAT", "BUK", "VRO"
#define TS_EXT_FILENAME         "TS\0", "TRP", "TS ", "TP ", "M2T"
#define RM_EXT_FILENAME         "RM\0", "RMV", "RM "
#define MPEG4_EXT_FILENAME      "AVI", "MP4", "DIV", "MOV", "MKV", "WSF", "ASF", "FLV", "WMV", "ASM", "3GP"
#define WMV_EXT_FILENAME        "WMV"
#define DVD_EXT_FILENAME        "IFO", "ISO"
#define IFO_EXT_FILENAME        "IFO"
#define MP3_EXT_FILENAME        "MP3"
#define WMA_EXT_FILENAME        "WMA"
#define AAC_EXT_FILENAME        "M4A", "AAC"
#define WAV_EXT_FILENAME        "WAV"
#define TXT_EXT_FILENAME        "TXT"
#define PLG_EXT_FILENAME        "BIN" //Get ext from BIN dynamically.
#define LOAS_EXT_FILENAME        "LOA", "ADT", "PKI", "FLA"    //LOAS, ADTS, PKI
#define OGG_EXT_FILENAME        "OGG"

#if ENABLE_DMP
#define NUM_OF_THUMBNAIL_FILES_PER_PAGE         12
#define UI_MEDIAPLAYER_THUMBNAIL_COLUMN_NUM     4
#define UI_MEDIAPLAYER_MAINITEM_FRM_LINE_WIDTH  2
#define UI_MEDIA_PLAYER_LIST_MARGIN_LEFT        259
#define UI_MEDIA_PLAYER_LIST_MARGIN_TOP         126
#define UI_MPLAYER_PHOTOITEM_WIDTH              98
#define UI_MPLAYER_PHOTOITEM_HEIGHT             106
#define UI_MPLAYER_PHOTOITEM_START_X(x)         (UI_MEDIA_PLAYER_LIST_MARGIN_LEFT+(x%UI_MEDIAPLAYER_THUMBNAIL_COLUMN_NUM)*(UI_MPLAYER_PHOTOITEM_WIDTH+UI_MEDIAPLAYER_MAINITEM_FRM_LINE_WIDTH*75))
#define UI_MPLAYER_PHOTOITEM_START_Y(y)         (UI_MEDIA_PLAYER_LIST_MARGIN_TOP+(y/UI_MEDIAPLAYER_THUMBNAIL_COLUMN_NUM)*(UI_MPLAYER_PHOTOITEM_HEIGHT+UI_MEDIAPLAYER_MAINITEM_FRM_LINE_WIDTH*40))

// Define thumbnail parameters
#define THUMBNAIL_WIDTH                     (UI_MPLAYER_PHOTOITEM_WIDTH)//(UI_MEDIA_PLAYER_LIST_ITEM_WIDTH/2)
#define THUMBNAIL_HEIGHT                    (UI_MPLAYER_PHOTOITEM_HEIGHT)//(UI_MEDIA_PLAYER_LIST_ITEM_HEIGHT*3)
#define THUMBNAIL_POSITION_ARRAY           {                                    \
            {UI_MPLAYER_PHOTOITEM_START_X(0), UI_MPLAYER_PHOTOITEM_START_Y(0)}, \
            {UI_MPLAYER_PHOTOITEM_START_X(1), UI_MPLAYER_PHOTOITEM_START_Y(1)}, \
            {UI_MPLAYER_PHOTOITEM_START_X(2), UI_MPLAYER_PHOTOITEM_START_Y(2)}, \
            {UI_MPLAYER_PHOTOITEM_START_X(3), UI_MPLAYER_PHOTOITEM_START_Y(3)}, \
            {UI_MPLAYER_PHOTOITEM_START_X(4), UI_MPLAYER_PHOTOITEM_START_Y(4)}, \
            {UI_MPLAYER_PHOTOITEM_START_X(5), UI_MPLAYER_PHOTOITEM_START_Y(5)}, \
            {UI_MPLAYER_PHOTOITEM_START_X(6), UI_MPLAYER_PHOTOITEM_START_Y(6)}, \
            {UI_MPLAYER_PHOTOITEM_START_X(7), UI_MPLAYER_PHOTOITEM_START_Y(7)}, \
            {UI_MPLAYER_PHOTOITEM_START_X(8), UI_MPLAYER_PHOTOITEM_START_Y(8)}, \
            {UI_MPLAYER_PHOTOITEM_START_X(9), UI_MPLAYER_PHOTOITEM_START_Y(9)}, \
            {UI_MPLAYER_PHOTOITEM_START_X(10), UI_MPLAYER_PHOTOITEM_START_Y(10)}, \
            {UI_MPLAYER_PHOTOITEM_START_X(11), UI_MPLAYER_PHOTOITEM_START_Y(11)}}
#else
#define NUM_OF_THUMBNAIL_FILES_PER_PAGE         24
#define UI_MEDIAPLAYER_THUMBNAIL_COLUMN_NUM     6
#define UI_MEDIAPLAYER_MAINITEM_FRM_LINE_WIDTH  2
#define UI_MEDIA_PLAYER_LIST_MARGIN_LEFT        48
#define UI_MEDIA_PLAYER_LIST_MARGIN_TOP         20
#define UI_MPLAYER_PHOTOITEM_WIDTH              194
#define UI_MPLAYER_PHOTOITEM_HEIGHT             144
#define UI_MPLAYER_PHOTOITEM_START_X(x)         (UI_MEDIA_PLAYER_LIST_MARGIN_LEFT+(x%UI_MEDIAPLAYER_THUMBNAIL_COLUMN_NUM)*(UI_MPLAYER_PHOTOITEM_WIDTH+UI_MEDIAPLAYER_MAINITEM_FRM_LINE_WIDTH*3+20))
#define UI_MPLAYER_PHOTOITEM_START_Y(y)         (UI_MEDIA_PLAYER_LIST_MARGIN_TOP+(y/UI_MEDIAPLAYER_THUMBNAIL_COLUMN_NUM)*(UI_MPLAYER_PHOTOITEM_HEIGHT+UI_MEDIAPLAYER_MAINITEM_FRM_LINE_WIDTH*3+40))

// Define thumbnail parameters
#define THUMBNAIL_WIDTH                     (UI_MPLAYER_PHOTOITEM_WIDTH)//(UI_MEDIA_PLAYER_LIST_ITEM_WIDTH/2)
#define THUMBNAIL_HEIGHT                    (UI_MPLAYER_PHOTOITEM_HEIGHT)//(UI_MEDIA_PLAYER_LIST_ITEM_HEIGHT*3)
#define THUMBNAIL_POSITION_ARRAY           {                                    \
            {UI_MPLAYER_PHOTOITEM_START_X(0), UI_MPLAYER_PHOTOITEM_START_Y(0)}, \
            {UI_MPLAYER_PHOTOITEM_START_X(1), UI_MPLAYER_PHOTOITEM_START_Y(1)}, \
            {UI_MPLAYER_PHOTOITEM_START_X(2), UI_MPLAYER_PHOTOITEM_START_Y(2)}, \
            {UI_MPLAYER_PHOTOITEM_START_X(3), UI_MPLAYER_PHOTOITEM_START_Y(3)}, \
            {UI_MPLAYER_PHOTOITEM_START_X(4), UI_MPLAYER_PHOTOITEM_START_Y(4)}, \
            {UI_MPLAYER_PHOTOITEM_START_X(5), UI_MPLAYER_PHOTOITEM_START_Y(5)}, \
            {UI_MPLAYER_PHOTOITEM_START_X(6), UI_MPLAYER_PHOTOITEM_START_Y(6)}, \
            {UI_MPLAYER_PHOTOITEM_START_X(7), UI_MPLAYER_PHOTOITEM_START_Y(7)}, \
            {UI_MPLAYER_PHOTOITEM_START_X(8), UI_MPLAYER_PHOTOITEM_START_Y(8)}, \
            {UI_MPLAYER_PHOTOITEM_START_X(9), UI_MPLAYER_PHOTOITEM_START_Y(9)}, \
            {UI_MPLAYER_PHOTOITEM_START_X(10), UI_MPLAYER_PHOTOITEM_START_Y(10)}, \
            {UI_MPLAYER_PHOTOITEM_START_X(11), UI_MPLAYER_PHOTOITEM_START_Y(11)}, \
            {UI_MPLAYER_PHOTOITEM_START_X(12), UI_MPLAYER_PHOTOITEM_START_Y(12)}, \
            {UI_MPLAYER_PHOTOITEM_START_X(13), UI_MPLAYER_PHOTOITEM_START_Y(13)}, \
            {UI_MPLAYER_PHOTOITEM_START_X(14), UI_MPLAYER_PHOTOITEM_START_Y(14)}, \
            {UI_MPLAYER_PHOTOITEM_START_X(15), UI_MPLAYER_PHOTOITEM_START_Y(15)}, \
            {UI_MPLAYER_PHOTOITEM_START_X(16), UI_MPLAYER_PHOTOITEM_START_Y(16)}, \
            {UI_MPLAYER_PHOTOITEM_START_X(17), UI_MPLAYER_PHOTOITEM_START_Y(17)}, \
            {UI_MPLAYER_PHOTOITEM_START_X(18), UI_MPLAYER_PHOTOITEM_START_Y(18)}, \
            {UI_MPLAYER_PHOTOITEM_START_X(19), UI_MPLAYER_PHOTOITEM_START_Y(19)}, \
            {UI_MPLAYER_PHOTOITEM_START_X(20), UI_MPLAYER_PHOTOITEM_START_Y(20)}, \
            {UI_MPLAYER_PHOTOITEM_START_X(21), UI_MPLAYER_PHOTOITEM_START_Y(21)}, \
            {UI_MPLAYER_PHOTOITEM_START_X(22), UI_MPLAYER_PHOTOITEM_START_Y(22)}, \
            {UI_MPLAYER_PHOTOITEM_START_X(23), UI_MPLAYER_PHOTOITEM_START_Y(23)}}
#endif

#define THUMBNAIL_PRIORITY_HIGHER_PREVIEW   1
#define THUMBNAIL_SHOW_DIRECTORY            1
#define SUPPORT_EMBEDDED_THUMBNAIL          1
#define SUPPORT_SHRINK_THUMBNAIL            0

#define SUPPORT_PHOTO_BACKGROUND_MUSIC      0
#define DEFAULT_PHOTO_SLIDESHOW_DELAY_TIME  5

#define SUPPORT_BACKGROUND_MUSIC      1
// BGM: BackGround Music
#undef ENABLE_BGM
#define ENABLE_BGM \
    (ENABLE_MPLAYER_MUSIC && SUPPORT_BACKGROUND_MUSIC)

#define FREEZE_BROWSING_CURSOR_WHEN_BGM_PLAYING      0
#define DISABLE_FILE_BROWSE_WHEN_BGM_PLAYING \
    (ENABLE_BGM && FREEZE_BROWSING_CURSOR_WHEN_BGM_PLAYING)

#define MOVE_BROWSING_CURSOR_TO_CURRENTLY_PLAYING_MUSIC      0
#define FOCUSING_CURRENTLY_PLAYING_MUSIC \
    (MOVE_BROWSING_CURSOR_TO_CURRENTLY_PLAYING_MUSIC) \
        && ((!ENABLE_BGM) || (DISABLE_FILE_BROWSE_WHEN_BGM_PLAYING))

#define MOVIE_REPEAT_PREVIEW                0
#define MUSIC_REPEAT_PREVIEW                0
#define MUSIC_RANDOM_PLAY                   0
#define VIDEO_CAPTURE_USE_GE_BUFFER         1

// co_buffer with GE_FRAMEBUFFER
#if (VIDEO_CAPTURE_USE_GE_BUFFER)
#define VIDEO_CAPTURE_BUFFER_ADR            ((GE_FRAMEBUFFER_MEMORY_TYPE & MIU1) ? (GE_FRAMEBUFFER_ADR | MIU_INTERVAL) : (GE_FRAMEBUFFER_ADR))
#define VIDEO_CAPTURE_BUFFER_LEN            GE_FRAMEBUFFER_LEN
#else
#define VIDEO_CAPTURE_BUFFER_ADR            0
#define VIDEO_CAPTURE_BUFFER_LEN            0
#endif

#ifdef ENABLE_DMBT_MM
#define HIGH_SPEED_BROWSER                  1
#else
#define HIGH_SPEED_BROWSER                  1
#endif
#define HIDE_HIDDEN_FILE                    1

#define USER_MP3_FLASH_START_ADDR           (0x00000)
#define USER_LOGO_FLASH_START_ADDR          (0x80000)
#define USER_LOGO_LENGTH_PER_LOGO           (0x80000) // 512K
#define USER_LOGO_MAX_NUMBER                (7) // USER_LOGO_MAX_NUMBER * USER_LOGO_LENGTH_PER_LOGO = flash size for user logo

#define ENABLE_DRM                          0

#define ENABLE_DECODE_FULL_PHOTO_IF_NOTHUMB 1

#define PHOTO_SLIDE_SHOW_EFFECT                 (1)
#define SHOW_PARTIALLY_DECODING_PHOTO           (PHOTO_SLIDE_SHOW_EFFECT && 1)
#define SHOW_PARTIALLY_DECODING_THUMBNAIL       (1)
#define ENABLE_PHOTO_SOFTWARE_DECODER_THUMBNAIL (0)

#if (PHOTO_SLIDE_SHOW_EFFECT)
    #define PHOTO_TRANS_EFFECT_DELAY_MS                     1000

    #define ENABLE_PHOTO_TRANS_EFFECT_BLOCKS                1
    #define ENABLE_PHOTO_TRANS_EFFECT_DIAGONAL_BLOCKS       1
    #define ENABLE_PHOTO_TRANS_EFFECT_WIPE                  1
    #define ENABLE_PHOTO_TRANS_EFFECT_SLIDE                 1
    #define ENABLE_PHOTO_TRANS_EFFECT_ZOOMIN                1
#ifdef NEW_VDPLAYER
    #define ENABLE_PHOTO_TRANS_EFFECT_FADE                  0
#endif
    #if (ENABLE_PHOTO_TRANS_EFFECT_BLOCKS)
        #define PHOTO_TRANS_EFFECT_BLOCK_HORZ_NUM           16
        #define PHOTO_TRANS_EFFECT_BLOCK_VERT_NUM           9
    #endif  // #if (ENABLE_PHOTO_TRANS_EFFECT_BLOCKS)

    #if (ENABLE_PHOTO_TRANS_EFFECT_DIAGONAL_BLOCKS)
        #define PHOTO_TRANS_EFFECT_DIAGONAL_BLOCK_NUM       16
    #endif  // #if (ENABLE_PHOTO_TRANS_EFFECT_DIAGONAL_BLOCKS)

    #if (ENABLE_PHOTO_TRANS_EFFECT_WIPE)
        #define PHOTO_TRANS_EFFECT_BOXWIPE_STEPS            64
        #define PHOTO_TRANS_EFFECT_BARWIPE_LINE_NUM         8
        #define PHOTO_TRANS_EFFECT_BARNDOORWIPE_LINE_NUM    8
        #define PHOTO_TRANS_EFFECT_IRISWIPE_STEPS           64
        #define PHOTO_TRANS_EFFECT_4BARWIPE_LINE_NUM        4
    #endif  // #if (ENABLE_PHOTO_TRANS_EFFECT_WIPE)

    #if (ENABLE_PHOTO_TRANS_EFFECT_SLIDE)
        #define PHOTO_TRANS_EFFECT_SLIDE_STEPS              4
        #define PHOTO_TRANS_EFFECT_SLIDE_UNIT_DELAY         200
    #endif  // #if (ENABLE_PHOTO_TRANS_EFFECT_SLIDE)

    #if (ENABLE_PHOTO_TRANS_EFFECT_ZOOMIN)
        #define PHOTO_TRANS_EFFECT_ZOOMIN_STEPS             10
    #endif  // #if (ENABLE_PHOTO_TRANS_EFFECT_ZOOMIN)

#endif  // #if (PHOTO_SLIDE_SHOW_EFFECT)

#define ALLOW_CAPTURE_SCREEN_WHEN_SLIDE_SHOW                0

#define MEDIA_PLAYER_HIGHLIGHT_CHANGE


#define ENABLE_MOVIE_INFO_CURRENT_TIME_REFINE   0
#define ENABLE_MOVIE_INFO_WINDOW_DETAILED       1
#define ENABLE_MPLAYER_RECORD_DIR_ENTRY         1

#endif

