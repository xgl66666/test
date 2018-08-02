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
#ifndef _DEMO_MM_MMSDK_H_
#define _DEMO_MM_MMSDK_H_

/// The Version of MM IF
#define MM_IF_VERSION 0x55AA5A5A00010001ULL

/// Define the string data type in meta data.
typedef enum
{
    /// Metadata string data type is ASCII.
    E_PLAYER_STRING_FORMAT_ASCII,
    /// Metadata string data type is UTF-8.
    E_PLAYER_STRING_FORMAT_UTF8,
    /// Metadata string data type is Unicode.
    E_PLAYER_STRING_FORMAT_UNICODE
} EN_PLAYER_STRING_FORMAT;

/// Define meida item metadata type.
typedef enum
{
    /// Media item title string format.
    E_PLAYER_METADATA_TITLE_STRING_FORMAT,
    /// Media item album string format.
    E_PLAYER_METADATA_ALBUM_STRING_FORMAT,
    /// Media item artist string format.
    E_PLAYER_METADATA_ARTIST_STRING_FORMAT,
    /// Media item year string format.
    E_PLAYER_METADATA_YEAR_STRING_FORMAT,
    /// Meida item metadata is music title.
    E_PLAYER_METADATA_TITLE,                //MP3_INFO_TITLE
    /// Meida item metadata is music album.
    E_PLAYER_METADATA_ALBUM,                //MP3_INFO_ALBUM
    /// Meida item metadata is music artist.
    E_PLAYER_METADATA_ARTIST,                //MP3_INFO_ARTIST
    /// Meida item metadata is music sampling rate.
    E_PLAYER_METADATA_SAMPLE_FREQ,            //MP3_INFO_SAMPLINGRATE
    /// Meida item metadata is music bit rate.
    E_PLAYER_METADATA_BITRATE,                //MP3_INFO_BITRATE
    /// Meida item metadata is music publication year.
    E_PLAYER_METADATA_YEAR,                    //MP3_INFO_YEAR
    /// Meida item metadata is media total play time.
    E_PLAYER_METADATA_PLAYTIME,                //E_VDPLAYER_INFO_TOTAL_TIME
    /// Meida item metadata is media horizontal resolution.
    E_PLAYER_METADATA_HRES,                    //E_PHOTO_INFO_IMAGE_WIDTH, E_VDPLAYER_INFO_H_SIZE
    /// Meida item metadata is media vertical resolution.
    E_PLAYER_METADATA_VRES,                    //E_PHOTO_INFO_IMAGE_HEIGHT, E_VDPLAYER_INFO_V_SIZE
    /// Meida item metadata is photo type (reference EN_PHOTO_IMAGE).
    E_PLAYER_METADATA_IMAGE_TYPE,            //E_PHOTO_INFO_IMAGE_TYPE
    /// Meida item metadata is photo that has thumbnail information.
    E_PLAYER_METADATA_IMAGE_HAS_THUMBNAIL,  //E_PHOTO_INFO_HAS_THUMBNAIL
    /// Meida item metadata is video frame per second. For 25 Fame/sec, the metadta is 25000.
    E_PLAYER_METADATA_FRAME_PER_SEC,        //E_VDPLAYER_INFO_FRM_PER_SEC
    /// Meida item metadata is video pixel aspect ratio width.
    E_PLAYER_METADATA_PAR_WIDTH,            //E_VDPLAYER_INFO_PAR_H_SIZE
    /// Meida item metadata is video pixel aspect ratio height.
    E_PLAYER_METADATA_PAR_HEIGHT,            //E_VDPLAYER_INFO_PAR_V_SIZE
    /// Meida item metadata is video codec type.
    E_PLAYER_METADATA_VIDEO_CODEC,
    /// Meida item metadata is audio codec type.
    E_PLAYER_METADATA_AUDIO_CODEC,
    /// Meida item metadata is video has index information (supprt trick mode process).
    E_PLAYER_METADATA_INDEX_EXISTED,        //E_VDPLAYER_INFO_MAX_FB_SPEED
    //E_PLAYER_METADATA_TOTAL_PAGE_NUM,        //For document only.
    /// Meida item metadata is total subtitle track number.
    E_PLAYER_METADATA_NB_SUBTITLE_TRACK,     //E_VDPLAYER_INFO_NB_SUBTITLE_TRACK
    /// Meida item metadata is total audio track number.
    E_PLAYER_METADATA_NB_AUDIO_TRACK,        //E_VDPLAYER_INFO_NB_AUDIO_TRACK
    /// Meida item metadata is program number in transport stream media.
    E_PLAYER_METADATA_NB_PROGRAM,            //E_VDPLAYER_INFO_NB_PROGRAM
#ifdef K3_MMSDK_PORTING
    /// Meida item metadata is DIVX title.
    E_PLAYER_METADATA_DIVX_TITLE,
    /// Meida item metadata is edition number.
    E_PLAYER_METADATA_EDITION,               //E_PLAYER_METADATA_EDITION
    /// Meida item metadata is chapter number.
    E_PLAYER_METADATA_CHAPTER,               //E_PLAYER_METADATA_CHAPTER
    /// Meida item metadata is auto-chapter number.
    E_PLAYER_METADATA_AUTOCHAPTER,           //E_PLAYER_METADATA_AUTOCHAPTER
    /// Meida item metadata is TPT MODE EXIST.
    E_PLAYER_METADATA_TPT_MODE_EXIST,        //E_PLAYER_METADATA_TPT_MODE_EXIST
    /// Meida item metadata is audio channel configuration.
    E_PLAYER_METADATA_AUDIO_CHANNEL_CONFIGURATION,
    /// Meida item metadata is law rating of title or edition
    E_PLAYER_METADATA_LAW_RATING,
    /// Meida item metadata is title string addr.
    E_PLAYER_METADATA_TITLE_STRING_ADDR,
    /// Meida item metadata is chapter string addr.
    E_PLAYER_METADATA_CHAPTER_STRING_ADDR,
    /// Meida item metadata is audio track name addr.
    E_PLAYER_METADATA_AUDIO_TRACK_NAME_ADDR,
    /// Meida item metadata is subtitle track name addr.
    E_PLAYER_METADATA_SUBTITLE_TRACK_NAME_ADDR,
#endif
    /// Meida item metadata is 3D or not
    E_PLAYER_METADATA_IS_3D,
    /// Meida item metadata is video 3D layout
    E_PLAYER_METADATA_3D_LAYOUT,
    /// Meida item metadata is subtitle type
    E_PLAYER_METADATA_SUBTITLE,
    /// Meida item metadata is Audio buffer remainder
    E_PLAYER_METADATA_AUDIO_BUFFER_REMAINDER,
    /// Meida item metadata is video buffer remainder
    E_PLAYER_METADATA_VIDEO_BUFFER_REMAINDER,
    /// Report all track 3D info, the size is ((Track num)*sizeof(U32))
    E_PLAYER_METADATA_MULTI_VIDEO_3D_LAYOUT,
    /// Report audio channel mode.
    E_PLAYER_METADATA_AUDIO_CHANNEL_MODE,
    /// Meida item metadata is Audio is Varible bitrate or not
    E_PLAYER_METADATA_AUDIO_IS_VBR,
    E_PLAYER_METADATA_PHOTO_OUTPUT_WIDTH,
    E_PLAYER_METADATA_PHOTO_OUTPUT_HEIGHT,
    E_PLAYER_METADATA_PHOTO_OUTPUT_PITCH,
    /// Media information is codec output photo has date time or not
    E_PLAYER_METADATA_IMAGE_HAS_DATE_TIME,
    /// Media information is codec output photo date time year
    E_PLAYER_METADATA_IMAGE_DATE_TIME_YEAR,
    /// Media information is codec output photo date time month
    E_PLAYER_METADATA_IMAGE_DATE_TIME_MONTH,
    /// Media information is codec output photo date time day
    E_PLAYER_METADATA_IMAGE_DATE_TIME_DAY,
    /// Media information is codec output photo date time hour
    E_PLAYER_METADATA_IMAGE_DATE_TIME_HOUR,
    /// Media information is codec output photo date time minute
    E_PLAYER_METADATA_IMAGE_DATE_TIME_MINUTE,
    /// Media information is codec output photo date time second
    E_PLAYER_METADATA_IMAGE_DATE_TIME_SECOND,
    /// Set Play Position is success or not
    E_PLAYER_METADATA_SET_PLAY_POSITION_ACK,
    /// Get audio language
    E_PLAYER_METADATA_GET_AUDIO_LANGUAGE_INDEX,
    /// Media item genre string format.
    E_PLAYER_METADATA_GENRE_STRING_FORMAT,
    /// Meida item metadata is music genre.
    E_PLAYER_METADATA_GENRE,
    /// Meida item metadata is whether it has picture
    E_PLAYER_METADATA_MUSIC_HAS_PICTURE,
    /// Meida item metadata is picture offset
    E_PLAYER_METADATA_MUSIC_PICTURE_OFFSET,
    /// Meida item metadata is picture size
    E_PLAYER_METADATA_MUSIC_PICTURE_SIZE,
    /// Meida item metadata is video codec id in detail. AP can classfy it by itself.
    E_PLAYER_METADATA_VIDEO_CODEC_ID_IN_DETAIL,
    /// Meida item metadata is audio codec id in detail. AP can classfy it by itself.
    E_PLAYER_METADATA_AUDIO_CODEC_ID_IN_DETAIL,
    /// Media item metadata is total time in container
    E_PLAYER_METADATA_CONTAINER_TOTAL_PLAYTIME,
    /// Meida item metadata is none.
    E_PLAYER_METADATA_PHOTO_OUTPUT_FORMAT,

    E_PLAYER_METADATA_NOT_SUPPORT
} EN_PLAYER_METADATA;

typedef enum
{
    MPLAYER_AUDIO_CODEC_ID_NONE         = 0,
    MPLAYER_AUDIO_CODEC_MP2             = 0x0040,
    MPLAYER_AUDIO_CODEC_MP3             = 0x0041,
    MPLAYER_AUDIO_CODEC_RM              = 0x0080,
    MPLAYER_AUDIO_CODEC_AC3             = 0x0100,
    MPLAYER_AUDIO_CODEC_AAC             = 0x0200,
    MPLAYER_AUDIO_CODEC_PCM             = 0x0400,
    MPLAYER_AUDIO_CODEC_ADPCM           = 0x0800,
    MPLAYER_AUDIO_CODEC_WMA1            = 0x1000,
    MPLAYER_AUDIO_CODEC_WMA2            = 0x1001,
    MPLAYER_AUDIO_CODEC_WMA3            = 0x1002,
    MPLAYER_AUDIO_CODEC_DTS             = 0x2000,
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
    MPLAYER_INFO_CONTAINER_TOTAL_TIME,

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

/// Define media item preview mode.
typedef enum
{
    /// Media item preview mode is normal. For photo and video media.
    E_PREVIEW_NORMAL_MODE,
    /// Media item preview mode is pause mode. For video media only, the video will be paused on the time (u32VideoPauseTime).
    E_PREVIEW_VIDEO_PAUSE_MODE,
    /// Media item preview mode is none.
    E_PREVIEW_NUM
} EN_PREVIEW_MODE;

/// Define subtitle type.
typedef enum
{
    /// Subtitle type is image.
    E_SUBTITLE_DATA_TYPE_IMAGE,
    /// Subtitle type is text.
    E_SUBTITLE_DATA_TYPE_TEXT,
    /// Subtitle type is none.
    E_SUBTITLE_DATA_TYPE_NUM
} EN_SUBTITLE_DATA_TYPE;

/// Define subtitle image format
typedef enum
{
    /// Subtitle image format is ARGB8888.
    E_SUBTITLE_IMAGE_ARGB8888,
    /// Subtitle image format is ARGB1555.
    E_SUBTITLE_IMAGE_ARGB1555,
    /// Subtitle image format is ARGB4444.
    E_SUBTITLE_IMAGE_ARGB4444,
    /// Subtitle image format is none.
    E_SUBTITLE_IMAGE_NUM
} EN_SUBTITLE_IMAGE_FORMAT;

/// Define subtitle image format
typedef enum
{
    /// Subtitle text format is UTF8.
    E_SUBTITLE_TEXT_UTF8,
    /// Subtitle text format is UNICODE.
    E_SUBTITLE_TEXT_UNICODE,
    /// Subtitle text format is none.
    E_SUBTITLE_TEXT_NUM
} EN_SUBTITLE_TEXT_FORMAT;

/// Define video codec type
typedef enum
{
    /// Video codec type is unknow.
    E_PLAYER_VIDEO_CODEC_UNKNOW = -1,
    /// Video codec type is MPEG 4.
    E_PLAYER_VIDEO_CODEC_MPEG4,
    /// Video codec type is motion JPG.
    E_PLAYER_VIDEO_CODEC_MJPEG,
    /// Video codec type is H264.
    E_PLAYER_VIDEO_CODEC_H264,
    /// Video codec type is RealVideo.
    E_PLAYER_VIDEO_CODEC_RM,
    E_PLAYER_VIDEO_CODEC_TS,
    /// Video codec type is MPEG 1/2.
    E_PLAYER_VIDEO_CODEC_MPG,
    /// Video codec type is VC1.
    E_PLAYER_VIDEO_CODEC_VC1,
    /// Video codec type is Audio Video Standard.
    E_PLAYER_VIDEO_CODEC_AVS,
    /// Video codec type is FLV.
    E_PLAYER_VIDEO_CODEC_FLV,
    /// Video codec type is MVC.
    E_PLAYER_VIDEO_CODEC_MVC,
    /// Video codec type is VP6.
    E_PLAYER_VIDEO_CODEC_VP6,
    /// Video codec type is VP8.
    E_PLAYER_VIDEO_CODEC_VP8,
} EN_PLAYER_VIDEO_CODEC;

/// Define audio codec tyep
typedef enum
{
    /// Audio codec type is none.
    E_PLAYER_AUDIO_CODEC_UNKNOW = -1,
    /// Audio codec type is WMA.
    E_PLAYER_AUDIO_CODEC_WMA,
    /// Audio codec type is DTS.
    E_PLAYER_AUDIO_CODEC_DTS,
    /// Audio codec type is MP3.
    E_PLAYER_AUDIO_CODEC_MP3,
    /// Audio codec type is MPEG.
    E_PLAYER_AUDIO_CODEC_MPEG,
    /// Audio codec type is AC3.
    E_PLAYER_AUDIO_CODEC_AC3,
    /// Audio codec type is AC3 plus.
    E_PLAYER_AUDIO_CODEC_AC3_PLUS,
    /// Audio codec type is AAC.
    E_PLAYER_AUDIO_CODEC_AAC,
    /// Audio codec type is PCM.
    E_PLAYER_AUDIO_CODEC_PCM,
    /// Audio codec type is ADPCM
    E_PLAYER_AUDIO_CODEC_ADPCM,
    /// Audio codec type is RAAC.
    E_PLAYER_AUDIO_CODEC_RAAC,
    /// Audio codec type is COOK.
    E_PLAYER_AUDIO_CODEC_COOK,
    /// Audio codec type is FLAC.
    E_PLAYER_AUDIO_CODEC_FLAC,
    /// Audio codec type is VORBIS.
    E_PLAYER_AUDIO_CODEC_VORBIS,
    /// Audio codec type is AMR NB.
    E_PLAYER_AUDIO_CODEC_AMR_NB,
    /// Audio codec type is AMR WB.
    E_PLAYER_AUDIO_CODEC_AMR_WB,
    /// Audio codec type is HEAAC.
    E_PLAYER_AUDIO_CODEC_HEAAC,
    /// Audio codec type is PCM MULAW.
    E_PLAYER_AUDIO_CODEC_PCM_MULAW,
    /// Audio codec type is PCM ALAW.
    E_PLAYER_AUDIO_CODEC_PCM_ALAW,
    /// Audio codec type is WMA2.
    E_PLAYER_AUDIO_CODEC_WMA2,
    /// Audio codec type is WMA3.
    E_PLAYER_AUDIO_CODEC_WMA3,
    /// Audio codec type is WAVEFORMATEX.
    E_PLAYER_AUDIO_CODEC_WAVEFORMATEX,
    /// Audio codec type is DRA.
    E_PLAYER_AUDIO_CODEC_DRA,
} EN_PLAYER_AUDIO_CODEC;

typedef enum
{
    MPLAYER_TRICK_NORMAL,         /// Enter normal play mode
    MPLAYER_TRICK_PAUSE,          /// Pause
    MPLAYER_TRICK_FF_2X,          /// Fast forward 2x
    MPLAYER_TRICK_FF_4X,          /// Fast forward 4x
    MPLAYER_TRICK_FF_8X,          /// Fast forward 8x
    MPLAYER_TRICK_FF_16X,         /// Fast forward 16x
    MPLAYER_TRICK_FB_2X,          /// Fast backward 2x
    MPLAYER_TRICK_FB_4X,          /// Fast backward 4x
    MPLAYER_TRICK_FB_8X,          /// Fast backward 8x
    MPLAYER_TRICK_FB_16X,         /// Fast backward 16x
    MPLAYER_TRICK_SLOW,           /// Slow motion mode
    MPLAYER_TRICK_MODE_NUM,  /// Specify the number of movie play mode
    MPLAYER_TRICK_MODE_INVALID = MPLAYER_TRICK_MODE_NUM,
} EN_MPLAYER_TRICK_MODE;

/// Define rectangle information.
typedef struct
{
    /// Horizontal start.
    MS_U16 u16X;
    /// Vertical start.
    MS_U16 u16Y;
    /// Width.
    MS_U16 u16Width;
    /// Height.
    MS_U16 u16Height;
} ST_RECT;

/// Define signed position rectangle information.
typedef struct
{
    /// Horizontal start.
    MS_S32 s32X;
    /// Vertical start.
    MS_S32 s32Y;
    /// Width.
    MS_U16 u16Width;
    /// Height.
    MS_U16 u16Height;
} ST_RECT_SIGNED_POSITION;

/// Define the information for preview process.
typedef struct
{
    /// Preview mode.
    EN_PREVIEW_MODE eMode;
    /// The time stamp for preview pause time (ms unit). Valid for preview mode is E_PREVIEW_VIDEO_PAUSE_MODE.
    MS_U32 u32VideoPauseTime;
} ST_PREVIEW_SETTING;

/// Define the information for subtitle.
typedef struct
{
    /// Subtitle track index.
    MS_U8 u8TrackID;
    /// Subtitle type.
    EN_SUBTITLE_DATA_TYPE eSubtitleType;
    /// Image subtitle format.
    EN_SUBTITLE_IMAGE_FORMAT eImageFormat;
    /// text subtitle format.
    EN_SUBTITLE_TEXT_FORMAT eTextFormat;
} ST_SUBTITLE_TRACK_INFO;

typedef enum
{
    MPLAYER_OPTION_UP_BOUNDARY = 0x0,     //ring buffer check range -> up boundary //unit: s default: 30
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
    MPLAYER_OPTION_SET_DBG_LEVEL,       //Set debug level:EN_MMSDK_DBG_LEVEL
    MPLAYER_OPTION_SET_KTV_MODE,    // KTV mode, moive(whitout audio) and music can play at same time
    MPLAYER_OPTION_SET_SEAMLESS_MODE,    // seamless mode, series of moive can player in seamless way
    MPLAYER_OPTION_PLAY_TIME_OUT,       //vdplayer MApp_VDPlayer_Play() time out in ms; default:5000
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

    MPLAYER_OPTION_ENABLE_NEW_AUDIO_MODE,

    MPLAYER_OPTIOND_INVALID,
} EN_MPLAYER_OPTION_TYPE;

typedef enum
{
    MPLAYER_SEAMLESS_NONE,
    MPLAYER_SEAMLESS_FREEZ,
    MPLAYER_SEAMLESS_SMOTH,     //only for mp4 and flv container
}EN_SEAMLESS_MODE;

typedef enum
{
    E_DIRECTION_UP,
    E_DIRECTION_DOWN,
    E_DIRECTION_LEFT,
    E_DIRECTION_RIGHT,

    E_DIRECTION_MAX,
} E_DIRECTION;

typedef enum
{
    MPLAYER_FILE_MODE,      //for local file
    MPLAYER_SEEKABLE_STREAM_MODE,   //for seekable stream
    MPLAYER_UNSEEKABLE_STREAM_MODE, //for unseekable stream
    MPLAYER_MODE_INVALID,
} EN_MPLAYER_FILE_MODE;

typedef enum
{
    PCM_FORMAT_PCM, //PCM
    PCM_FORMAT_M_ADPCM, //M ADPCM
    PCM_FORMAT_IMA_ADPCM, //8-bit ITU-T G.711
    PCM_FORMAT_NUM,
} EN_PCM_FORMAT;

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

typedef struct
{
    E_DIRECTION eDirection;
    MS_U16 u16Offset;
} MPLAYER_MOVEVIEW_PARAM;

typedef struct _MPLAYER_DISP_WINDOW
{
    MS_U32 u32DispX;
    MS_U32 u32DispY;
    MS_U32 u32DispW;
    MS_U32 u32DispH;
} MPLAYER_DISP_WINDOW;

typedef struct
{
    EN_PCM_FORMAT eFormatTag;
    MS_U16 u16Channels;
    unsigned int u32SamplesPerSec;
    MS_U16 u16BitsPerSample;
    MS_BOOL bBigEndian;
} MPLAYER_PCM_PARAM;

typedef void *(*PFN_Stream_Open)(const char * pathname, const char * mode); //open stream, return stream handle
typedef int (*PFN_Stream_Close)(void * stream); //close stream
typedef unsigned long long(*PFN_Stream_Length)(void * stream);  //get stream length
typedef int (*PFN_Stream_Seek)(void * stream, unsigned long long offset, int fromwhere); //seek to postition
typedef int (*PFN_Stream_Read)(void *ptr, int size, int nitems, void *stream); //read data from stream, return size of readed data

typedef struct _MPLAYER_STREAM_IOCB
{
    PFN_Stream_Open pfnStreamOpen;
    PFN_Stream_Close pfnStreamClose;
    PFN_Stream_Length pfnStreamLength;
    PFN_Stream_Seek pfnStreamSeek;
    PFN_Stream_Read pfnStreamRead;
} MPLAYER_STREAM_IOCB;

typedef struct
{
    char                                    FileExt[8];
    EN_MMSDK_MEDIA_TYPE    				FileType;
} Z_File_Ext_Info;

//for E_MPLAYER_MEDIA_MOVIE to open E_MPLAYER_MEDIA_STREAM_SUBTITLE
typedef enum
{
    E_MPLAYER_MEDIA_STREAM_MIN =  E_MMSDK_MEDIA_TYPE_PHOTO,
    E_MPLAYER_MEDIA_STREAM_PHOTO = E_MMSDK_MEDIA_TYPE_PHOTO,
    E_MPLAYER_MEDIA_STREAM_MUSIC = E_MMSDK_MEDIA_TYPE_MUSIC,
    E_MPLAYER_MEDIA_STREAM_MOVIE = E_MMSDK_MEDIA_TYPE_MOVIE,
    E_MPLAYER_MEDIA_STREAM_SUBTITLE,
    E_MPLAYER_MEDIA_STREAM_AUDIO,
    E_MPLAYER_MEDIA_STREAM_AUDIO2,
    E_MPLAYER_MEDIA_STREAM_NUM,
    E_MPLAYER_MEDIA_STREAM_INVALID = E_MPLAYER_MEDIA_STREAM_NUM,
} EN_MPLAYER_MEDIA_STREAM_TYPE;

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

typedef struct _MPLAYER_SUBTITLE
{
    char * SubtitleFileName;
    char * SubtitleSUBFileName;
    EN_MPLAYER_SUBTITLE_TYPE eSubtitleType;
} MPLAYER_SUBTITLE;

typedef struct _MPLAYER_MEDIA
{
    //[movie][music][photo] file name
    char * filename;
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
    MS_BOOL bThumbnail;

    unsigned char bInitOnly;
    unsigned char bPreviewEnable;
    ST_PREVIEW_SETTING stPreviewSetting;

} MPLAYER_MEDIA;

typedef enum
{
    MPLAYER_EXIT_OK = 0, //playback ok, and exit to ap
    MPLAYER_EXIT_FORCE,  //custom stop playback, and exit to ap
    MPLAYER_EXIT_ERROR_FILE, //playback  error, and exit to ap

    MPLAYER_SUFFICIENT_DATA, //when playing, data enough to continue play, and codec wil resume
    MPLAYER_INSUFFICIENT_DATA, //when playing, run short of data, and codec wil pause
    MPLAYER_START_PLAY, //player init ok, and start playing
    MPLAYER_PLAY_OK,

    MPLAYER_PHOTO_DECODE_DONE,     /*!< \brief The photo decode process has finished successfully. */
    MPLAYER_PHOTO_DECODE_FAILED,    /*!< \brief The photo decode process is failed. */
    MPLAYER_PHOTO_DECODING_NEXT,    /*!< \brief Next file is decoding and no decoded photo is ready for display. */
    MPLAYER_PHOTO_DISPLAYED,    /*!< \brief One decoded photo is now displayed. */

    MPLAYER_MOVIE_THUMBNAIL_DECODE_DONE,
    MPLAYER_MOVIE_THUMBNAIL_DECODE_FAIL,

    MPLAYER_PROGRAM_CHANGED, // program has been changed.
    MPLAYER_AUDIO_TRACK_CHANGED, // audio track has been changed.

    MPLAYER_FRAME_READY,
    MPLAYER_BLACKSCREEN_OFF,
    MPLAYER_NOTIFY_NULL, //playback notify null
} EN_MPLAYER_NOTIFY_TYPE;

#if (DEMO_APM_TEST == 1)
typedef struct MM_TSP_INFO_S
{
    unsigned char    *pAddr;
    unsigned long    u32Size;
} MM_TSP_INFO;//george
#endif

MS_BOOL Demo_MM_Init(void);
MS_BOOL Demo_MM_Seek(MS_U32 *pGotoTimeMs);
MS_BOOL Demo_MM_DeInit(void);
MS_BOOL Demo_MM_GetOption(MS_U32 *pOption, MS_U32 *pMMInfo);
MS_BOOL Demo_MM_Play(MS_U32 *pFileOption, MS_U8* U8FileName);
MS_BOOL Demo_MM_FF(void);
MS_BOOL Demo_MM_RW(void);
MS_BOOL Demo_MM_Pause(void);
MS_BOOL Demo_MM_Resume(void);
MS_BOOL Demo_MM_Stop(void);
MS_BOOL Demo_MM_Stop_MediaType(MS_U32 * pMediaType);
MS_BOOL Demo_MM_Repeat(MS_U32 *pu32On);
MS_U32 Demo_MM_GetOutAdr(void);
MS_BOOL Demo_MM_SetSubtitleTrack(MS_U32 *u32TrackId);
MS_BOOL Demo_MM_SetAudioTrack(MS_U32 *u32TrackId);
MS_BOOL Demo_MM_SetProgramTrack(MS_U32 *u32ProgramId);
MS_BOOL Demo_MM_RW_Replay(MS_U32 *pu32On);
MS_BOOL Demo_MM_Get_DrawPhotoInfo(MS_U16* u16width, MS_U16* u16height, MS_U16* u16pitch, MS_U16* u16format);
MS_BOOL Demo_MM_Get_DrawPhotoInfo(MS_U16* u16width, MS_U16* u16height, MS_U16* u16pitch, MS_U16* u16format);
MS_BOOL Demo_MM_Set_DrawPhotoPath(MS_U32* u32Path);
MS_BOOL Demo_MM_AllocAudDec(void);
MS_BOOL Demo_MM_FreeAudDec(void);
MS_BOOL Demo_MM_SetDebugLevel(MS_U32* u32DebugLevel);

#if (DEMO_APM_TEST == 1)
MS_BOOL Demo_MM_IPCINIT(void);
MS_BOOL Demo_MM_IPCDeInit(void);
MS_BOOL Demo_MM_AUDIO(void);
#endif

#endif

