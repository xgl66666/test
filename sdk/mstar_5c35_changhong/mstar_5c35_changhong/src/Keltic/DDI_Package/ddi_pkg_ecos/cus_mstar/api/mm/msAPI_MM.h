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
#ifndef __MSRV_ZMPLAYER_H__
#define __MSRV_ZMPLAYER_H__


#include <stdio.h>
#include <string.h>
#include <sys/stat.h>


#ifdef __cplusplus
extern "C"
{
#endif

typedef enum
{
    MPLAYER_DBG_LEVEL_NONE = 0,    // disable all uart message.
    MPLAYER_DBG_LEVEL_ERR,    // Only output error message
    MPLAYER_DBG_LEVEL_INFO,    // output general message, and above.
    MPLAYER_DBG_LEVEL_DBG,    // output debug message, and above.
    MPLAYER_DBG_LEVEL_TRACE,    // output function tracing message, and above.
    MPLAYER_DBG_LEVEL_FW,    // output FW message.
} EN_MPLAYER_DBG_LEVEL;

typedef enum
{
    MPLAYER_UART_SWITCH_TO_HK,
    MPLAYER_UART_SWITCH_TO_COPROCESSOR,
} EN_MPLAYER_UART_SWITCH;

typedef enum
{
    MPLAYER_BUFFER_RTSP_AUDIO_ADDR,
    MPLAYER_BUFFER_RTSP_AUDIO_LEN,
} EN_MPLAYER_BUFFER_TYPE;

typedef enum
{
    E_ZOOM_1_DIV_8,  /*!< Zoom in, X(1/8). */
    E_ZOOM_1_DIV_4,  /*!< Zoom in, X(1/4). */
    E_ZOOM_1_DIV_2,  /*!< Zoom in, X(1/2). */
    E_ZOOM_1,        /*!< Normal, X1. */
    E_ZOOM_2,        /*!< Zoom out, X2. */
    E_ZOOM_4,        /*!< Zoom out, X4. */
    E_ZOOM_8,        /*!< Zoom out, X8. */

    E_ZOOM_MAX,
} E_ZOOM;

typedef enum
{
    E_ROTATION_0,
    E_ROTATION_90,
    E_ROTATION_180,
    E_ROTATION_270,

    E_ROTATION_MAX,
} E_ROTATION;

typedef enum
{
    E_DIRECTION_UP,
    E_DIRECTION_DOWN,
    E_DIRECTION_LEFT,
    E_DIRECTION_RIGHT,

    E_DIRECTION_MAX,
} E_DIRECTION;

typedef struct
{
    E_DIRECTION eDirection;
    U16 u16Offset;
} MPLAYER_MOVEVIEW_PARAM;


typedef enum
{
    REPEATAB_MODE_NONE,
    REPEATAB_MODE_A,
    REPEATAB_MODE_AB,
    REPEATAB_MODE_MAX,
} EN_MPLAYER_REPEATAB_STATUS;
typedef enum
{
    MPLAYER_EXIT_OK = 0, //playback ok, and exit to ap
    MPLAYER_EXIT_FORCE,  //custom stop playback, and exit to ap
    MPLAYER_EXIT_ERROR_FILE, //playback  error, and exit to ap

    MPLAYER_SUFFICIENT_DATA, //when playing, data enough to continue play, and codec wil resume
    MPLAYER_INSUFFICIENT_DATA, //when playing, run short of data, and codec wil pause
    MPLAYER_START_PLAY, //player init ok, and start playing

    MPLAYER_PHOTO_DECODE_DONE,     /*!< \brief The photo decode process has finished successfully. */
    MPLAYER_PHOTO_DECODE_FAILED,    /*!< \brief The photo decode process is failed. */
    MPLAYER_PHOTO_DECODING_NEXT,    /*!< \brief Next file is decoding and no decoded photo is ready for display. */
    MPLAYER_PHOTO_DISPLAYED,    /*!< \brief One decoded photo is now displayed. */

    MPLAYER_MOVIE_THUMBNAIL_DECODE_DONE,
    MPLAYER_MOVIE_THUMBNAIL_DECODE_FAIL,

    MPLAYER_PROGRAM_CHANGED, // program has been changed.
    MPLAYER_AUDIO_TRACK_CHANGED, // audio track has been changed.

    MPLAYER_FRAME_READY,
    MPLAYER_BLACKSCREEN_OFF,//george

    MPLAYER_SET_SUBTITLE_PALETTE,
    MPLAYER_SUBTITLE_INIT,
    MPLAYER_SUBQUEUE_PUSH,
    MPLAYER_SUBTITLE_ENABLE,
    MPLAYER_SET_SUBTITLE_TRACK,
    MPLAYER_TS_SUBTITLE_PROGRAM_CHANGED,
    MPLAYER_SUBTITLE_DISPLAY,
    MPLAYER_NOTIFY_NULL, //playback notify null
    MPLAYER_CHANGE_RESOLUTION,
} EN_MPLAYER_NOTIFY_TYPE;

typedef enum
{
    MPLAYER_COMMAND_NOTIFY, //chakra will notify AP EN_MPLAYER_NOTIFY_TYPE, when something happen in movie playing
    MPLAYER_COMMAND_SUBTITLE, //chakra will notify AP to draw/hide subtitle
    MPLAYER_COMMAND_MUSIC_LYRIC, //chakra will notify AP to draw music lyric
    MPLAYER_COMMAND_INVALID,
} EN_MPLAYER_COMMAND_TYPE;
typedef enum
{
    MPLAYER_FILE_MODE,      //for local file
    MPLAYER_RTSP_MODE,  //for rtsp stream
    MPLAYER_SEEKABLE_STREAM_MODE,   //for seekable stream
    MPLAYER_UNSEEKABLE_STREAM_MODE, //for unseekable stream
    MPLAYER_DIRECTMEM_MODE,	// for direct memory input
    MPLAYER_MODE_INVALID
} EN_MPLAYER_FILE_MODE;

typedef enum
{
    MPLAYER_SUBTITLE_SRT,
    MPLAYER_SUBTITLE_SSA,
    MPLAYER_SUBTITLE_ASS,
    MPLAYER_SUBTITLE_SMI,
    MPLAYER_SUBTITLE_IDX,
    MPLAYER_SUBTITLE_NUM,
    MPLAYER_SUBTITLE_INVALID = 0xFF,
} EN_MPLAYER_SUBTITLE_TYPE;

typedef enum
{
    PCM_FORMAT_PCM, //PCM
    PCM_FORMAT_M_ADPCM, //M ADPCM
    PCM_FORMAT_IMA_ADPCM, //8-bit ITU-T G.711
    PCM_FORMAT_NUM,
} EN_PCM_FORMAT;


typedef enum
{
    /// Codec ouput foramt is none.
    E_PHOTO_OUTPUT_NONE,
    /// Codec ouput foramt is YUV422.
    E_PHOTO_OUTPUT_YUV422,
    /// Codec ouput foramt is ARGB8888.
    E_PHOTO_OUTPUT_ARGB8888,
    E_PHOTO_OUTPUT_ARGB1555,
} EN_PHOTO_OUTPUT_FORMAT;


typedef struct
{
    EN_PCM_FORMAT eFormatTag;
    U16 u16Channels;
    unsigned int u32SamplesPerSec;
    U16 u16BitsPerSample;
    BOOLEAN bBigEndian;
} MPLAYER_PCM_PARAM;

typedef enum
{
    MPLAYER_SUBTYPE_UNKNOWN = 0,

    MPLAYER_SUBTYPE_MOVIE,

    MPLAYER_SUBTYPE_JPEG,
    MPLAYER_SUBTYPE_PNG,
    MPLAYER_SUBTYPE_GIF,
    MPLAYER_SUBTYPE_TIFF,
    MPLAYER_SUBTYPE_BMP,

    MPLAYER_SUBTYPE_MP3,
    MPLAYER_SUBTYPE_AAC,
    MPLAYER_SUBTYPE_PCM,
    MPLAYER_SUBTYPE_WAV,
    MPLAYER_SUBTYPE_WMA,

    MPLAYER_SUBTYPE_TEXT,
} EN_MPLAYER_SUB_TYPE;

typedef struct _MPLAYER_SUBTITLE
{
    char * SubtitleFileName;
    char * SubtitleSUBFileName;
    EN_MPLAYER_SUBTITLE_TYPE eSubtitleType;
} MPLAYER_SUBTITLE;

typedef struct _MPLAYER_DISP_WINDOW
{
    U32 u32DispX;
    U32 u32DispY;
    U32 u32DispW;
    U32 u32DispH;
}MPLAYER_DISP_WINDOW;

typedef enum
{
    E_MPLAYER_MEDIA_MIN,
    E_MPLAYER_MEDIA_PHOTO = E_MPLAYER_MEDIA_MIN,
    E_MPLAYER_MEDIA_MUSIC,
    E_MPLAYER_MEDIA_MOVIE,
    E_MPLAYER_MEDIA_NUM,
    E_MPLAYER_MEDIA_INVALID = E_MPLAYER_MEDIA_NUM,
} EN_MPLAYER_MEDIA_TYPE;  //for user selection

typedef struct _MPLAYER_MEDIA
{
    //[movie][music][photo] file name
    char * filename;
    //memory input address and size
    unsigned char *pu8DirectMemAddr;
    unsigned long long int u64DirectMemSize;

    //[movie][music][photo] file mode
    EN_MPLAYER_FILE_MODE eFileMode;

    //[moive][photo] display window, {0, 0, 0, 0} means full panel
    unsigned int u32DispX;
    unsigned int u32DispY;
    unsigned int u32DispW;
    unsigned int u32DispH;
    //[movie] Goto time when start play
    unsigned int u32GotoTimeMs;
    //[movie] external subtitle
    MPLAYER_SUBTITLE sSubtitle;
    //[movie] only when audio is a Separated stream
    char * MovieAudioStreamName;
    //[music] lyric file name
    char * MusicLyricfilename;
    //[photo/movie] thumbnail decode
    unsigned char bThumbnail;
} MPLAYER_MEDIA;

/*
typedef enum
{
    /// The trick mode type is normal speed.
    E_PLAYER_TRICKMODE_NORMAL = 0,
    /// The trick mode type is pause.
    E_PLAYER_TRICKMODE_PAUSE,
    /// The trick mode type is slow forward.
    E_PLAYER_TRICKMODE_SLOW_FORWARD,
    /// The trick mode type is fast forward.
    E_PLAYER_TRICKMODE_FAST_FORWARD,
    /// The trick mode type is fast backward.
    E_PLAYER_TRICKMODE_FAST_BACKWARD,
    /// The trick mode type is step by step (not support now).
    E_PLAYER_TRICKMODE_STEP,
    /// The trick mode type is none.
    E_PLAYER_TRICKMODE_NOT_SUPPORT
} EN_PLAYER_TRICKMODE;
*/
/*
typedef enum
{
    /// The trick mode speed is 1X.
    E_PLAYER_TRICKSPEED_1X = 1,
    /// The trick mode speed is 2X.
    E_PLAYER_TRICKSPEED_2X,
    /// The trick mode speed is 4X.
    E_PLAYER_TRICKSPEED_4X,
    /// The trick mode speed is 8X.
    E_PLAYER_TRICKSPEED_8X,
    /// The trick mode speed is 16X.
    E_PLAYER_TRICKSPEED_16X,
    /// The trick mode speed is 32X.
    E_PLAYER_TRICKSPEED_32X,
    /// The trick mode speed is none.
    E_PLAYER_TRICKSPEED_NOT_SUPPORT
} EN_PLAYER_TRICKSPEED;

typedef struct
{
    /// Trick mode type.
    EN_PLAYER_TRICKMODE eMode;
    /// Trick mode speed.
    EN_PLAYER_TRICKSPEED eSpeed;
} ST_TRICK_INFO;
*/

typedef enum
{
    MPLAYER_VIDEO_CODEC_ID_NONE             = 0,
    MPLAYER_VIDEO_CODEC_MPEG1           = 0x0040,
    MPLAYER_VIDEO_CODEC_MPEG2           = 0x0041,
    MPLAYER_VIDEO_CODEC_RM                = 0x0080,
    MPLAYER_VIDEO_CODEC_MPEG4               = 0x0100,
    MPLAYER_VIDEO_CODEC_H263               = 0x0101,
    MPLAYER_VIDEO_CODEC_H264                = 0x0200,
    MPLAYER_VIDEO_CODEC_MJPEG                = 0x0800,
    MPLAYER_VIDEO_CODEC_FLV                 = 0x1000,
    MPLAYER_VIDEO_CODEC_VC1                 = 0x2000,
    MPLAYER_VIDEO_CODEC_WMV3                = 0x2001,
} EN_MPLAYER_VIDEO_CODEC_TYPE;

typedef enum
{
    MPLAYER_AUDIO_CODEC_ID_NONE             = 0,
    MPLAYER_AUDIO_CODEC_MP2           = 0x0040,
    MPLAYER_AUDIO_CODEC_MP3           = 0x0041,
    MPLAYER_AUDIO_CODEC_RM             = 0x0080,
    MPLAYER_AUDIO_CODEC_AC3                 = 0x0100,
    MPLAYER_AUDIO_CODEC_AAC                 = 0x0200,
    MPLAYER_AUDIO_CODEC_PCM                 = 0x0400,
    MPLAYER_AUDIO_CODEC_ADPCM               = 0x0800,
    MPLAYER_AUDIO_CODEC_WMA1                 = 0x1000,
    MPLAYER_AUDIO_CODEC_WMA2                 = 0x1001,
    MPLAYER_AUDIO_CODEC_WMA3                 = 0x1002,
    MPLAYER_AUDIO_CODEC_DTS                 = 0x2000,
} EN_MPLAYER_AUDIO_CODEC_TYPE;


typedef enum
{
    MPLAYER_INFO_TOTAL_TIME = 0x0,
    MPLAYER_INFO_TOTAL_TIME_ISVALID,
    MPLAYER_INFO_CUR_TIME,
    MPLAYER_INFO_FRM_PER_SEC,
    MPLAYER_INFO_H_SIZE,
    MPLAYER_INFO_V_SIZE,
    MPLAYER_INFO_NB_AUDIO_TRACK,
    MPLAYER_INFO_AUDIO_TRACK_ID,
    MPLAYER_INFO_NB_SUBTITLE_TRACK, //Internal subtitle
    MPLAYER_INFO_SUBTITLE_TRACK_ID, //Internal subtitle
    MPLAYER_INFO_NB_PROGRAM,
    MPLAYER_INFO_PROGRAM_ID,
    MPLAYER_INFO_VIDEO_CODEC,   //EN_MPLAYER_VIDEO_CODEC_TYPE
    MPLAYER_INFO_AUDIO_CODEC,   //EN_MPLAYER_AUDIO_CODEC_TYPE
    MPLAYER_INFO_DURATION_PERCENT,
    MPLAYER_INFO_MOVIE_ERROR_CODE,
    MPLAYER_INFO_INDEX_FRAME_EXIST,
    MPLAYER_INFO_BUFFER_PERCENT,  //get buffer percent, 0%->pause, 100%->resume

    MPLAYER_INFO_MUSIC_LAYER_LEN = 0x20,                       /*!music info,for wma, Layer,COMMENT and GENRE info are not supported. */
    MPLAYER_INFO_MUSIC_BITRATE_LEN,
    MPLAYER_INFO_MUSIC_SAMPLINGRATE_LEN,
    MPLAYER_INFO_MUSIC_ALBUM_LEN,
    MPLAYER_INFO_MUSIC_TITLE_LEN,
    MPLAYER_INFO_MUSIC_ARTIST_LEN,
    MPLAYER_INFO_MUSIC_YEAR_LEN,
    MPLAYER_INFO_MUSIC_GENRE_LEN,
    MPLAYER_INFO_MUSIC_COMMENT_LEN,
    MPLAYER_INFO_MUSIC_CHECK_FF_FB_LEN,

    MPLAYER_INFO_MUSIC_LAYER,                       /*!music info,for wma, Layer,COMMENT and GENRE info are not supported. */
    MPLAYER_INFO_MUSIC_BITRATE,
    MPLAYER_INFO_MUSIC_SAMPLINGRATE,
    MPLAYER_INFO_MUSIC_ALBUM,
    MPLAYER_INFO_MUSIC_TITLE,
    MPLAYER_INFO_MUSIC_ARTIST,
    MPLAYER_INFO_MUSIC_YEAR,
    MPLAYER_INFO_MUSIC_GENRE,
    MPLAYER_INFO_MUSIC_COMMENT,
    MPLAYER_INFO_MUSIC_CHECK_FF_FB,
    MPLAYER_INFO_MUSIC_ERROR_CODE,

    MPLAYER_INFO_PHOTO_DISPLAYING_WIDTH = 0x40,
    MPLAYER_INFO_PHOTO_DISPLAYING_HEIGHT,
    MPLAYER_INFO_PHOTO_DISPLAYING_SIZE,
    MPLAYER_INFO_PHOTO_DECODED_WIDTH,
    MPLAYER_INFO_PHOTO_DECODED_HEIGHT,
    MPLAYER_INFO_PHOTO_DECODED_PITCH,
    MPLAYER_INFO_PHOTO_DECODE_PROGRESS,
    MPLAYER_INFO_PHOTO_DECODE_OUTBUF_ADDR,
    MPLAYER_INFO_PHOTO_ERROR_CODE,
    MPLAYER_INFO_PHOTO_CODEC_TYPE,
    MPLAYER_INFO_PHOTO_IMAGE_ORIGINAL_WIDTH,
    MPLAYER_INFO_PHOTO_IMAGE_ORIGINAL_HEIGHT,
    MPLAYER_INFO_PHOTO_DECODE_FORMAT,

    MPLAYER_INFO_MOVIE_THUMBNAIL_WIDTH = 0x60,
    MPLAYER_INFO_MOVIE_THUMBNAIL_HEIGHT,
    MPLAYER_INFO_MOVIE_THUMBNAIL_PITCH,
    MPLAYER_INFO_MOVIE_THUMBNAIL_OUTBUF_ADDR,
} EN_MPLAYER_INFO;

typedef enum
{
	MPLAYER_CONTAINER_TYPE_NONE    = 0x0,
	MPLAYER_CONTAINER_TYPE_AVI     = 0x1,
	MPLAYER_CONTAINER_TYPE_MP4     = 0x2,
	MPLAYER_CONTAINER_TYPE_MKV     = 0x4,
	MPLAYER_CONTAINER_TYPE_ASF     = 0x8,
	MPLAYER_CONTAINER_TYPE_RM      = 0x10,
	MPLAYER_CONTAINER_TYPE_TS      = 0x20,
	MPLAYER_CONTAINER_TYPE_MPG     = 0x40,
	MPLAYER_CONTAINER_TYPE_FLV     = 0x80,
	MPLAYER_CONTAINER_TYPE_ESDATA  = 0x100,
	MPLAYER_CONTAINER_TYPE_DSS     = 0x200,
	MPLAYER_CONTAINER_TYPE_SWF     = 0x400,

}EN_MPLAYER_CONTAINER_TYPE;


typedef enum
{
    MPLAYER_OPTION_WRITE_LIMIT = 0x0,    //max bytes to be fed per loop [ 0:total ring buffer size || others:specifc size]
    MPLAYER_OPTION_UP_BOUNDARY,     //ring buffer check range -> up boundary //unit: s default: 30
    MPLAYER_OPTION_DOWN_BOUNDARY,     //ring buffer check range -> down boundary //unit: s default: 10
    MPLAYER_OPTION_RING_BUFFER_HANDLE,    //the number of ring buffer stream handle //default: 1

    MPLAYER_OPTION_ENABLE_SUBTITLE,
    MPLAYER_OPTION_CHANGE_EXTERNAL_SUBTITLE,
    MPLAYER_OPTION_CHANGE_INTERNAL_SUBTITLE,
    MPLAYER_OPTION_CHANGE_PROGRAM,
    MPLAYER_OPTION_CHANGE_AUDIO,

    MPLAYER_OPTION_MOVIE_INFO,  //EN_MPLAYER_INFO
    MPLAYER_OPTION_TRICK_MODE,  //EN_MPLAYER_TRICK_MODE
    MPLAYER_OPTION_ASPECT_RATIO,    //EN_MPLAYER_ASPECT_RATIO_TYPE
    MPLAYER_OPTION_PICTURE_MODE,    //EN_MPLAYER_PICTURE_MODE
    MPLAYER_OPTION_DISPLAY_WINDOW,  //set display in playing

    MPLAYER_OPTION_READ_TIME_OUT,       //vdplayer read stram time out in ms; default:500
    MPLAYER_OPTION_INIT_TIME_OUT,       //vdplayer init time out in ms
    MPLAYER_OPTION_INIT_DATA_SIZE,       //data size before vdplayer init //default: 0x20000
    MPLAYER_OPTION_SET_DBG_LEVEL,       //Set debug level:EN_MPLAYER_DBG_LEVEL
    MPLAYER_OPTION_SET_STC_THRESHOLD,       //set rtsp stc & pts diffrence threshold
    MPLAYER_OPTION_GET_BUFFER_INFO,  //Get valid memory buffer in rtsp
    MPLAYER_OPTION_SET_KTV_MODE,    // KTV mode, moive(whitout audio) and music can play at same time
    MPLAYER_OPTION_SET_SEAMLESS_MODE,    // seamless mode, series of moive can player in seamless way
    MPLAYER_OPTION_PLAY_TIME_OUT,       //vdplayer MApp_VDPlayer_Play() time out in ms; default:5000
    MPLAYER_OPTION_RTSP_TS_MODE,       //if set true, ts movie also can ff/fb even if in unseekable stream mode //default: false
    MPLAYER_OPTION_FEED_LOOP_TIME,       //Set feed data thread sleep time, unit:ms //default: 1
    MPLAYER_OPTION_ENABLE_FAST_INIT,       //if set true, only read "INIT_DATA_SIZE" data before MApp_VDPlayer_Play() //default: false
    MPLAYER_OPTION_DECODE_TIME_OUT,       //codec decode timeout, unit:ms, "0" means disable decode timeout //default: 0
    MPLAYER_OPTION_SUBTITLE_MEM_LESS_MODE,       //Subtitle memory usage config: TRUE/FALSE //default FALSE
    MPLAYER_OPTION_SUBTITLE_DYN_ONSCRREN_MODE, //Subtitle on-screen buffer usage config: TRUE/FALSE //default FALSE
    MPLAYER_OPTION_MOVIE_SUBTITLE_TRACK_NAME,
    MPLAYER_OPTION_MOVIE_AUDIO_TRACK_NAME,
    MPLAYER_OPTION_MOVIE_AUDIO_TRACK_INFO,

    MPLAYER_OPTION_SET_MUSIC_TYPE = 0x30,
    MPLAYER_OPTION_GET_MUSIC_TYPE,
    MPLAYER_OPTION_ENABLE_MUSIC_LYRIC,
    MPLAYER_OPTION_DISABLE_MUSIC_LYRIC,
    MPLAYER_OPTION_IS_CURRENT_LYRIC_AVAIL,
    MPLAYER_OPTION_MUSIC_SHARE_MEM_ADDR,
    MPLAYER_OPTION_MUSIC_SHARE_MEM_SIZE,
    MPLAYER_OPTION_MUSIC_INFO,  //EN_MPLAYER_INFO
    MPLAYER_OPTION_MUSIC_SET_PCM_PARAM,

    MPLAYER_OPTION_ZOOM = 0x40,
    MPLAYER_OPTION_ROTATION,
    MPLAYER_OPTION_MOVE_VIEW,

    MPLAYER_OPTION_STOP_PHOTO_DISPLAY = 0x50,
    MPLAYER_OPTION_ENABLE_SLIDE_SHOW,
    MPLAYER_OPTION_DISABLE_SLIDE_SHOW,
    MPLAYER_OPTION_SET_SLIDE_SHOW_TIMEOUT,
    MPLAYER_OPTION_GET_PHOTO_FREE_SLOT,
    MPLAYER_OPTION_GET_PHOTO_INFO,
    MPLAYER_OPTION_PHOTO_ONLY_DECODE,

    MPLAYER_OPTION_CAPTURE_LOGO = 0x60,
    MPLAYER_OPTION_CAPTURE_LOGO_BYADDR,
    MPLAYER_OPTION_UART_SWITCH, //EN_MPLAYER_UART_SWITCH
    MPLAYER_OPTION_REPEATAB,
    MPLAYER_OPTION_FB_REPLAY,

    MPLAYER_OPTION_RTSP_TRACE = 0x70,    //Trace rtsp default: TRUE
    MPLAYER_OPTION_RTSP_FREEZE_MODE,    //Freeze before switch to next channel, default:TRUE
    MPLAYER_OPTION_RTSP_BUFFER_TIME,    //Buffered ms berfore play, unit:ms default:2500
    MPLAYER_OPTION_SET_DRM_KEY,
    MPLAYER_OPTION_SET_SUBTITLE_DELAY,                                       // set subtitle show delay steps, in unit 100ms
    MPLAYER_OPTION_SET_SUBTITLE_ADVANCE,                                  // set subtitle show advanced steps, in unit 100ms
    MPLAYER_OPTION_CANCEL_SUBTITLE_DELAY_ADVANCE,                // cancel subtitle delay/advanced steps, in unit 100ms

	MPLAYER_OPTION_SPECIFIC_TOTAL_TIME, // for ts stream, set specific total time, then codec needn't read file end
    MPLAYER_OPTION_BACKLIGHT_ON_OFF = 0x80,     //on/off back light
    MPLAYER_OPTION_MEMC_ON_OFF,     //on/off memc
    MPLAYER_OPTION_TRANSMIT_KEY,     //true: Transmit KEY_EXIT to AP when MM playing. Default: false

    MPLAYER_OPTION_GET_MOVIE_THUMBNAIL_INFO,
    MPLAYER_OPTION_SET_STARTTIME,
    MPLAYER_OPTION_ENABLE_FASTEN_TS_PLAYING,
    MPLAYER_OPTION_DISABLE_CONTAINER_TYPE,
    MPLAYER_OPTION_ENABLE_VDEC_FORCE_INTERLACE_MODE,

    MPLAYER_OPTIOND_INVALID,
} EN_MPLAYER_OPTION_TYPE;


typedef enum
{
    MPLAYER_TRICK_NORMAL,         /// Enter normal play mode
    MPLAYER_TRICK_PAUSE,          /// Pause
    MPLAYER_TRICK_FF_2X,          /// Fast forward 2x
    MPLAYER_TRICK_FF_4X,          /// Fast forward 4x
    MPLAYER_TRICK_FF_8X,          /// Fast forward 8x
    MPLAYER_TRICK_FF_16X,         /// Fast forward 16x
    MPLAYER_TRICK_FF_32X,
    MPLAYER_TRICK_FB_2X,          /// Fast backward 2x
    MPLAYER_TRICK_FB_4X,          /// Fast backward 4x
    MPLAYER_TRICK_FB_8X,          /// Fast backward 8x
    MPLAYER_TRICK_FB_16X,         /// Fast backward 16x
    MPLAYER_TRICK_FB_32X,
    MPLAYER_TRICK_SLOW,           /// Slow motion mode
    MPLAYER_TRICK_MODE_NUM,  /// Specify the number of movie play mode
    MPLAYER_TRICK_MODE_INVALID = MPLAYER_TRICK_MODE_NUM,
} EN_MPLAYER_TRICK_MODE;

typedef enum
{
    MPLAYER_ASPECT_RATIO_MIN,
    MPLAYER_ASPECT_RATIO_16X9 = MPLAYER_ASPECT_RATIO_MIN,
    MPLAYER_ASPECT_RATIO_JUSTSCAN,
    MPLAYER_ASPECT_RATIO_ORIGINAL,
    MPLAYER_ASPECT_RATIO_4X3,
    MPLAYER_ASPECT_RATIO_ORIGINAL_SCALE,
    MPLAYER_ASPECT_RATIO_ZOOM1,
    MPLAYER_ASPECT_RATIO_ZOOM2,
    MPLAYER_ASPECT_RATIO_PANORAMA,
    MPLAYER_ASPECT_RATIO_NUM,
} EN_MPLAYER_ASPECT_RATIO_TYPE;

typedef enum
{
    MPLAYER_SEAMLESS_NONE,
    MPLAYER_SEAMLESS_FREEZ,
    MPLAYER_SEAMLESS_SMOTH,     //only for mp4 and flv container
}EN_SEAMLESS_MODE;

typedef enum
{
    rtsp_mp4a,
    rtsp_mp3,
    rtsp_mpeg,
    rtsp_a52,
    rtsp_audio_none,
} rtsp_audio_type;

typedef enum
{
    rtsp_h264,
    rtsp_mp4v,
    rtsp_h263,
} rtsp_video_type;

typedef struct video_decoder_init_info
{
    rtsp_video_type video_type;             /**<must set, video codec type*/
    unsigned int video_width;            /**<video width*/
    unsigned int video_height;           /**<video height*/
    unsigned int frame_rate;             /**<frame rate*/
    unsigned char avsync_enable;
    unsigned char drop_error_frame;
} video_decoder_init_info;

typedef struct audio_decoder_init_info
{
    rtsp_audio_type audio_type;             /**<must set*/
    unsigned int sample_rate;            /**<audio sample rate*/
    unsigned int num_channels;           /**<channel number*/

    unsigned char avsync_enable;        /**<>**/
    unsigned char errcnt_enable;        /**<>**/
    unsigned char pes_enable;        /**<>**/
    unsigned char pes_head_length;     /**<>**/
} audio_decoder_init_info;

typedef enum
{
    normal_play,    /**<normal play*/
    slow_play,     /**<slow play*/
    fast_play,    /**<fast play*/
} rtsp_play_mode;

typedef struct video_disp_info //101005
{
   unsigned int dWidth;
   unsigned int dHeight;
   unsigned int dFrameRate;
   unsigned int dCropLeft;
   unsigned int dCropRight;
   unsigned int dCropTop;
   unsigned int dCropBottom;
   unsigned int dInterlace;
   unsigned int dDS_Info_BaseAddr;
   unsigned int dDS_MIU_Select;
   unsigned int dDS_Index_Depth;
   unsigned int dAspectWidth;
   unsigned int dAspectHeight;
   //use for callback setwindows
   unsigned int dX;
   unsigned int dY;
   unsigned int dW;
   unsigned int dH;
} video_disp_info; //george

typedef enum
{
    video_stream,    /**<video stream*/
    audio_stream,    /**<audio stream*/
} rtsp_payload_type;

typedef struct rtsp_block
{
    char* payload_buffer;                          /**<payload data*/
    unsigned int payload_len;                         /**<payload data length*/
    unsigned long long pts_timestamp;                           /**< pts time stamp unit: 1/90Khz*/
    unsigned long long dts_timestamp;                            /**< dts time stamp unit: 1/90Khz*/
} rtsp_block;

typedef struct AUDIO_INFO_S
{
    unsigned long    u32AudioSrc;
    unsigned long    u32AudioPath;
    unsigned long    u32AudioOutput;
} AUDIO_INFO;//george

typedef struct MM_TSP_INFO_S
{
    unsigned char    *pAddr;
    unsigned long    u32Size;
} MM_TSP_INFO;//george

typedef void *(*PFN_Stream_Open)(const char* const pathname,const char* const mode); //open stream, return stream handle
typedef int (*PFN_Stream_Close)(void* const stream); //close stream
typedef unsigned long long(*PFN_Stream_Length)(void* const stream);  //get stream length
typedef int (*PFN_Stream_Seek)(void* const stream, const unsigned long long offset, const int fromwhere); //seek to postition
typedef int (*PFN_Stream_Read)(void* const ptr, const int size, const int nitems, void* const stream); //read data from stream, return size of readed data

typedef void *(*PFN_Rtsp_Open)(const char * rtsp_url, video_decoder_init_info* video_dec_info, audio_decoder_init_info* audio_dec_info);
typedef int (*PFN_Rtsp_Close)(void * rhandle);
typedef int (*PFN_Rtsp_Play)(void * rhandle, unsigned int start_pos, rtsp_play_mode mode);
typedef int (*PFN_Rtsp_Pause)(void * rhandle);
typedef int (*PFN_Rtsp_Resume)(void * rhandle);
typedef int (*PFN_Rtsp_Get_Payload)(void * rhandle, rtsp_payload_type type, rtsp_block* block);

typedef struct _MPLAYER_STREAM_IOCB
{
    PFN_Stream_Open pfnStreamOpen;
    PFN_Stream_Close pfnStreamClose;
    PFN_Stream_Length pfnStreamLength;
    PFN_Stream_Seek pfnStreamSeek;
    PFN_Stream_Read pfnStreamRead;
    //for RTSP
    PFN_Rtsp_Open pfnRtspOpen;
    PFN_Rtsp_Close pfnRtspClose;
    PFN_Rtsp_Play pfnRtspPlay;
    PFN_Rtsp_Pause pfnRtspPause;
    PFN_Rtsp_Resume pfnRtspResume;
    PFN_Rtsp_Get_Payload pfnRtspGetPayload;
} MPLAYER_STREAM_IOCB;

typedef enum
{
    E_MMAP_ID_INVALID = 0,
    E_MMAP_ID_VDEC_CPU,
    E_MMAP_ID_VDEC_FRAMEBUFFER,
    E_MMAP_ID_VDEC_BITSTREAM,
    E_MMAP_ID_XC_MAIN_FB,
    E_MMAP_ID_PM51_VAR_MEM,
    E_MMAP_ID_GOP_REGDMA,
    E_MMAP_ID_DMX_SECBUF,
    E_MMAP_ID_MHEG5_CI_PLUS_BUFFER,
    E_MMAP_ID_VDEC_FRAMEBUFFER_SD,
    E_MMAP_ID_VDEC_BITSTREAM_SD,

    E_DFB_JPD_READ,
    E_DFB_JPD_WRITE,
    E_DFB_JPD_INTERNAL,
    E_MMAP_ID_VE,
    E_MMAP_ID_JPD_WRITE,
    E_MMAP_ID_JPD_READ,
    E_MMAP_ID_PHOTO_INTER,
    E_MMAP_ID_PHOTO_DISPLAY,
    E_MMAP_ID_PHOTO_MPO_INTER,
    E_MMAP_ID_DUMMY16,
    E_MMAP_ID_COPROCESSOR,

    E_MMAP_ID_PHOTO_SHAREMEM,     //RobertYang add. Reference PHOTO_SHARE_MEM_BUFFER_ADDR in Chakra2
    E_MMAP_ID_VDPLAYER_DATA,      //RobertYang add. Reference VDPLAYER_DATA_ADR in Chakra2
    E_MMAP_ID_VDPLAYER_BITSTREAM, //RobertYang add. Reference VDPLAYER_BITSTREAM_BUFF_ADR in Chakra2
    E_MMAP_ID_VDPLAYER_BITSTREAM_EXT, //WaHaHa add.
    E_MMAP_ID_DUMMY20,           //RobertYang add. Reference SUBTITLE_ADR in Chakra2
    E_MMAP_ID_MAD_DEC,            //RobertYang add. Reference MAD_DEC_BUFFER_ADR in Chakra2
    E_MMAP_ID_MAD_SE,             //WaHaHung add.   Reference MAD_SE_BUFFER_ADR in Obama
    E_MMAP_ID_TTX,                //WaHaHung add.   Reference TTXBUF_START_ADR in Obama
    E_MMAP_ID_TTX_VE,             //WaHaHung add.   Reference TTX_VE_BUFFER_START_ADR in Obama
    E_MMAP_ID_DUMMY21,           //WaHaHung add.   Reference TTX_FONT_START_ADR in Obama

    E_LX_MEM,
    E_LX_MEM2,
    E_EMAC_MEM,
    E_MST_GEVQ,
    E_MST_GOP_REGDMA,
    E_DFB_FRAMEBUFFER,
    //According to 1 MIU 128MB request, add following ID -- BELOW
    E_MMAP_ID_PVR_DOWNLOAD,
    E_MMAP_ID_PVR_UPLOAD,
    E_MMAP_ID_PVR_BUFFER,
    E_MMAP_ID_PVR_THUMBNAIL,

    E_MMAP_ID_PVR_BROWSER_BUFFER,
    E_MMAP_ID_COMB_3D_BUF,
    E_MMAP_ID_MM_DIVX_FONT_TBL,
    E_MMAP_ID_VDPLAYER_SUBTITLE_BITSTREAM_BUFF,
    E_MMAP_ID_MM_SUBTITLE_BUFFER,
    E_MMAP_ID_MHEG5_BUFFER,
    E_MMAP_ID_DUMMY17,
    E_MMAP_ID_DUMMY18,
    E_MMAP_ID_EVENTDB_SDRAM,
    E_MMAP_ID_EPGEXTDB_SDRAM,

    E_MMAP_ID_DOWNLOAD_BUFFER,
    E_MMAP_ID_DUMMY1,
    E_MMAP_ID_DUMMY2,
    E_MMAP_ID_DUMMY3,
    E_MMAP_ID_DUMMY4,
    E_MMAP_ID_DUMMY5,
    E_MMAP_ID_DUMMY6,
    E_MMAP_ID_DUMMY7,
    E_MMAP_ID_DUMMY8,
    E_MMAP_ID_VDPLAYER,

    E_MMAP_ID_PHOTO,
    E_MMAP_ID_DUMP_BUFFER,
    E_MMAP_ID_DUMP_BUFFER2,
    E_MMAP_ID_SCALER_DNR_SUB_BUF,
    E_MMAP_ID_FLUSH_BUFFER,
    E_MMAP_ID_MHEG5_JPEG_BUFFER,

    E_MMAP_ID_MAD_R2,
    E_MMAP_ID_MFE,
    E_MMAP_ID_DMX_VQUEUE,
    E_MMAP_ID_XC_MLOAD,
    E_MMAP_ID_CC,

    //According to 1 MIU 128MB request, add following ID -- ABOVE
    E_MMAP_ID_DIP,
    E_MMAP_ID_3D_BUF0,
    E_MMAP_ID_3D_BUF1,
    E_MMAP_ID_3D_COLOR,
    E_MMAP_ID_3D_CMD,
    E_MMAP_ID_3D_VTX,
    E_MMAP_ID_3D_DWIN,
    E_MMAP_ID_3D_MPOOL,
    E_MMAP_ID_3D_MPOOL_M1,
    E_MMAP_ID_CAPTURESCREEN,

    E_MMAP_ID_VDEC_SUB_FRAMEBUFFER,
    E_MMAP_ID_VDEC_SUB_BITSTREAM,
    E_MMAP_ID_VDEC_MVC_FRAMEBUFFER,
    E_MMAP_ID_VDEC_MVC_BITSTREAM,
    /**Secure Boot Requirement**/
    E_MMAP_ID_HW_AES_BUF,
    E_MMAP_ID_XC_2DTO3D_DD_BUF,
    E_MMAP_ID_XC_2DTO3D_DR_BUF,

    //FRC
    E_MMAP_ID_FRC_MEMC,
    E_MMAP_ID_FRC_OD,
    E_MMAP_ID_FRC_LD,
    E_MMAP_ID_FRC_ME1,
    E_MMAP_ID_FRC_ME2,
    E_MMAP_ID_FRC_2D3D_Render,
    E_MMAP_ID_FRC_2D3D_Render_Detection,
    E_MMAP_ID_FRC_Halo,
    E_MMAP_ID_FRC_R2,

    E_MMAP_ID_MAX
} EN_MMAP_ID;

typedef enum
{
    /// The trick mode speed is 0X.
    E_PLAYER_TRICKSPEED_0X = 0,
    /// The trick mode speed is 1X.
    E_PLAYER_TRICKSPEED_1X = 1,
    /// The trick mode speed is 2X.
    E_PLAYER_TRICKSPEED_2X,
    /// The trick mode speed is 4X.
    E_PLAYER_TRICKSPEED_4X,
    /// The trick mode speed is 8X.
    E_PLAYER_TRICKSPEED_8X,
    /// The trick mode speed is 16X.
    E_PLAYER_TRICKSPEED_16X,
    /// The trick mode speed is 32X.
    E_PLAYER_TRICKSPEED_32X,
    /// The trick mode speed is none.
    E_PLAYER_TRICKSPEED_NOT_SUPPORT
} EN_PLAYER_TRICKSPEED;

typedef struct MM_MEM_INFO_S
{
    unsigned long    u32Total_mem_size;
    unsigned long    u32Miu0_mem_size;
    unsigned long    u32Miu1_mem_size;
    unsigned long    u32Miu_boundary;
    unsigned long    u32Mmap_items;
} __attribute__((packed))MM_MEM_INFO;

typedef struct MM_MEMMAP_INFO_S
{
    unsigned long    u32ID;
    unsigned long    u32Addr;
    unsigned long    u32Size;
    unsigned char     u8Layer;
    unsigned long    u32Align;
    unsigned long    bIs_miu0;
} __attribute__((packed))MM_MEMMAP_INFO;


typedef struct
{
    char                                    FileExt[8];
    EN_MPLAYER_MEDIA_TYPE    				FileType;
} Z_File_Ext_Info;


//====================original from Mapi_cpcodec.h====================
typedef void* CPCODEC_MEDIA;

/// Define video intrnal subtitle type.
typedef enum
{
    /// Video internal subtitle type is none.
    E_CPCODEC_INTERNAL_SUBTITLE_TYPE_NONE = 0,
    /// Video internal subtitle type is TS subtitle.
    //E_CPCODEC_INTERNAL_SUBTITLE_TYPE_TS,
    /// Video internal subtitle type is TS BMP subtitle.
    E_CPCODEC_INTERNAL_SUBTITLE_TYPE_TS_BMP,
    /// Video internal subtitle type is TS PGS subtitle.
    E_CPCODEC_INTERNAL_SUBTITLE_TYPE_TS_PGS,
    /// Video internal subtitle type is VOB subtitle.
    E_CPCODEC_INTERNAL_SUBTITLE_TYPE_IMAGE_VOBSUB,
    /// Video internal subtitle type is BMP.
    E_CPCODEC_INTERNAL_SUBTITLE_TYPE_IMAGE_BMP,
    /// Video internal subtitle type is text.
    E_CPCODEC_INTERNAL_SUBTITLE_TYPE_TEXT,
    /// Video internal subtitle type is text.
    //E_CPCODEC_INTERNAL_SUBTITLE_TYPE_TEXT_UTF8,
    /// Video internal subtitle type is text.
    //E_CPCODEC_INTERNAL_SUBTITLE_TYPE_TEXT_UNICODE,
    /// Video internal subtitle type is TrueTypeFont.
    E_CPCODEC_INTERNAL_SUBTITLE_TYPE_TRUE_TYPE_FONT,
} EN_CPCODEC_INTERNAL_SUBTITLE_TYPE;


typedef struct
{
    /// Media object relative information.
    CPCODEC_MEDIA Media;
    /// Video subtitle type.
    EN_CPCODEC_INTERNAL_SUBTITLE_TYPE eType;
    /// Init subtitle decoder.
    unsigned char bInit;
    /// BMP subtitle width.
    U16 u16BmpWidth;
    /// BMP subtitle height.
    U16 u16BmpHeight;
    /// Check subtitle PTS
    U32 u32CurrentPTS;
    /// check subtitle show or hide
    unsigned char bShow;
} ST_CPCODEC_MSG_SUBTITLE_INIT_INFO;

/// Define the attached information for callback process. The callback message is E_CPCODEC_MSG_SUBQUEUE_PUSH.
typedef struct
{
    /// Media object relative information.
    CPCODEC_MEDIA Media;
    /// Video subtitle type.
    EN_CPCODEC_INTERNAL_SUBTITLE_TYPE eType;
    /// Subtitle data buffer start (virtual address).
    U32 u32QueueStar;
    /// Subtitle data buffer size.
    U32 u32QueueSize;
    /// Subtitle presentation time stamp start.
    U32 u32PTSStart;
    /// Subtitle presentation time stamp end.
    U32 u32PTSEnd;
} ST_CPCODEC_MSG_SUBQUEUE_PUSH_INFO;

/// Define the attached information for callback process. The callback message is E_CPCODEC_MSG_SET_SUBTITLE_TRACK.
typedef struct
{
    /// Media object relative information.
    CPCODEC_MEDIA Media;
    /// Video subtitle type.
    EN_CPCODEC_INTERNAL_SUBTITLE_TYPE eType;
    /// DVB composition_page_id.
    U16 u16CPageID;
    /// DVB ancillary_page_id.
    U16 u16APageID;
} ST_CPCODEC_MSG_SET_SUBTITLE_TRACK_INFO;

//====================original from Mapi_cpcodec.h====================

typedef void (*CmdCallback)(EN_MPLAYER_COMMAND_TYPE eCmd, unsigned int u32Param, unsigned int u32Info);//the callback of zmplayer

//-------------------------------------------------------------------------------------------------
/// It exists just for being compatible with obama platform(no implement)
///
/// @return                 \b void
//-------------------------------------------------------------------------------------------------
void MApi_MPlayer_Task(void);

//-------------------------------------------------------------------------------------------------
/// Finalize the MM when it quits
///
/// @return                 \b void
//-------------------------------------------------------------------------------------------------
void MApi_MPlayer_Finalize(void);

void MApi_MDMPPlayer_Finalize(void);

//-------------------------------------------------------------------------------------------------
/// Initialize the MM when it starts, should call it before any zmplayer function called
///
/// @return                 \b void
//-------------------------------------------------------------------------------------------------
//void MApi_MPlayer_Initialize(void);
BOOLEAN MApi_MPlayer_Initialize(AUDIO_INFO *pAudioInfo, MM_TSP_INFO *pTspInfo, void* pMMInfo, void* pMMapInfo);//george
void MApi_MDMPPlayer_Initialize(AUDIO_INFO *pAudioInfo, MM_TSP_INFO *pTspInfo, void* pMMInfo, void* pMMapInfo);

//-------------------------------------------------------------------------------------------------
/// Play a file
/// @param eMediaType           \b IN: media type(photo, music or movie).
/// @param pMplayerMedia           \b IN: the media infor should be input.
/// @param pStreamIOCB           \b IN: register callback functions MM will call.
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
BOOLEAN MApi_MPlayer_PlayFile(const EN_MPLAYER_MEDIA_TYPE eMediaType,  MPLAYER_MEDIA * const pMplayerMedia, MPLAYER_STREAM_IOCB* const pStreamIOCB);

//-------------------------------------------------------------------------------------------------
/// pause function when playing stream
/// @param eMediaType           \b IN: media type.
///
/// @return                 \b the current trick mode
//-------------------------------------------------------------------------------------------------
EN_MPLAYER_TRICK_MODE MApi_MPlayer_Pause(const EN_MPLAYER_MEDIA_TYPE eMediaType);

//-------------------------------------------------------------------------------------------------
/// resume function when playing stream
/// @param eMediaType           \b IN: media type.
///
/// @return                 \b the current trick mode
//-------------------------------------------------------------------------------------------------
EN_MPLAYER_TRICK_MODE MApi_MPlayer_Resume(const EN_MPLAYER_MEDIA_TYPE eMediaType);

//-------------------------------------------------------------------------------------------------
/// Fast forward function when playing stream
/// @param eMediaType           \b IN: media type.
///
/// @return                 \b the current trick mode
//-------------------------------------------------------------------------------------------------
EN_MPLAYER_TRICK_MODE MApi_MPlayer_FF(const EN_MPLAYER_MEDIA_TYPE eMediaType);
EN_MPLAYER_TRICK_MODE MApi_MPlayer_FF2(const EN_MPLAYER_MEDIA_TYPE eMediaType, EN_PLAYER_TRICKSPEED enPlaySpeed);

//-------------------------------------------------------------------------------------------------
/// Fast backward function when playing stream
/// @param eMediaType           \b IN: media type.
///
/// @return                 \b the current trick mode
//-------------------------------------------------------------------------------------------------
EN_MPLAYER_TRICK_MODE MApi_MPlayer_RW(const EN_MPLAYER_MEDIA_TYPE eMediaType);
EN_MPLAYER_TRICK_MODE MApi_MPlayer_RW2(const EN_MPLAYER_MEDIA_TYPE eMediaType, EN_PLAYER_TRICKSPEED enPlaySpeed);

//-------------------------------------------------------------------------------------------------
/// seek to some position when playing stream
/// @param eMediaType           \b IN: media type.
/// @param u32GotoTimeMs           \b IN: seek to the position.
///
/// @return                 \b the current trick mode
//-------------------------------------------------------------------------------------------------
EN_MPLAYER_TRICK_MODE MApi_MPlayer_Seek(const EN_MPLAYER_MEDIA_TYPE eMediaType, const unsigned int u32GotoTimeMs);

//-------------------------------------------------------------------------------------------------
/// stop playing stream
/// @param eMediaType           \b IN: media type.
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
BOOLEAN MApi_MPlayer_Stop(const EN_MPLAYER_MEDIA_TYPE eMediaType);

//-------------------------------------------------------------------------------------------------
/// set some parameter or status to the MM
/// @param eMediaType           \b IN: media type.
/// @param eOption           \b IN: what parameter or status will be set.
/// @param u32Param           \b IN: the correspongding value.
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
BOOLEAN MApi_MPlayer_SetOption(const EN_MPLAYER_MEDIA_TYPE eMediaType, const EN_MPLAYER_OPTION_TYPE eOption, const unsigned int u32Param);

//-------------------------------------------------------------------------------------------------
/// get some parameter or status of the MM
/// @param eMediaType           \b IN: media type.
/// @param eOption           \b IN: what parameter or status will be gotten.
/// @param u32SubOption           \b IN: sub option.
///
/// @return                 \b the corresponding value
//-------------------------------------------------------------------------------------------------
unsigned int MApi_MPlayer_GetOption(const EN_MPLAYER_MEDIA_TYPE eMediaType, const EN_MPLAYER_OPTION_TYPE eOption, const unsigned int u32SubOption);


//-------------------------------------------------------------------------------------------------
/// register the callback that notify users
/// @param eMediaType           \b IN: media type.
/// @param pCmdCbFunc           \b IN: user will get event in it .
///
/// @return                 \b void
//-------------------------------------------------------------------------------------------------
void MApi_MPlayer_RegisterCallBack(const EN_MPLAYER_MEDIA_TYPE eMediaType, CmdCallback pCmdCbFunc);

//-------------------------------------------------------------------------------------------------
///Reset the all buffer(both in zmplayer and codec)
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
BOOLEAN MApi_MPlayer_ResetAllBuf(const EN_MPLAYER_MEDIA_TYPE eMediaType);

//-------------------------------------------------------------------------------------------------
///Get Disp Info
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------

unsigned char MApi_MPlayer_Get_DispInfo(EN_MPLAYER_MEDIA_TYPE eMediaType, video_disp_info* pDispInfo);

//-------------------------------------------------------------------------------------------------
///Set Mem Pool Index
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------

unsigned char MApp_MPlayer_SetMemPool(S32 s32PoolId, unsigned char bIsCached);

//-------------------------------------------------------------------------------------------------
///Set Customer Info
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------

unsigned char MApp_MPlayer_SetCustomerInfo(unsigned char* pu8InfoKey, unsigned long u32InfokeySize, unsigned char* pu8HashKey, unsigned long u32HashKeySize);

#ifdef __cplusplus
}
#endif

#endif
