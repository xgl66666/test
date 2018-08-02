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


#ifndef __MMSDK_INTERFACE_DEF_H__
#define __MMSDK_INTERFACE_DEF_H__


#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @file
 * @defgroup mmsdk_interface_def mmsdk_interface_def functions
 * @{
 */

/// The number of bits of MMSDK major version
#define MMSDK_VERSION_MAJOR_BITS 8
/// The major version of MMSDK: it should less than 0~255 (0x0~0xFF)
/// ABI incompatible
#define MMSDK_VERSION_MAJOR 1

/// The number of bits of MMSDK minor version
#define MMSDK_VERSION_MINOR_BITS 16
/// The minor version of MMSDK: it should less than 0~65535 (0x0~0xFFFF)
/// ABI compatible
#define MMSDK_VERSION_MINOR 72

/// The number of bits of MMSDK porting layer major version
#define MMSDK_PORTING_VERSION_MAJOR_BITS 8
/// The major version of MMSDK porting layer: it should less than 0~255 (0x0~0xFF)
/// ABI incompatible
#define MMSDK_PORTING_VERSION_MAJOR 1

/// The number of bits of MMSDK porting layer minor version
#define MMSDK_PORTING_VERSION_MINOR_BITS 16
/// The minor version of MMSDK porting layer: it should less than 0~65535 (0x0~0xFFFF)
/// ABI compatible
#define MMSDK_PORTING_VERSION_MINOR 2


#if !defined(TRUE) && !defined(FALSE)
/// definition for TRUE
#define TRUE                        1
/// definition for FALSE
#define FALSE                       0
#endif
/// data type unsigned char, data length 1 byte
typedef unsigned char      MMSDK_BOOL;
/// data type unsigned char, data length 1 byte
typedef unsigned char       MMSDK_U8;     // 1 byte
/// data type unsigned short, data length 2 byte
typedef unsigned short      MMSDK_U16;    // 2 bytes
/// data type unsigned int, data length 4 byte
typedef unsigned long       MMSDK_U32;    // 4 bytes
/// data type unsigned int, data length 8 byte
typedef unsigned long long  MMSDK_U64;    // 8 bytes
/// data type signed char, data length 1 byte
typedef signed char         MMSDK_S8;     // 1 byte
/// data type signed short, data length 2 byte
typedef signed short        MMSDK_S16;    // 2 bytes
/// data type signed int, data length 4 byte
typedef signed long         MMSDK_S32;    // 4 bytes
/// data type signed int, data length 8 byte
typedef long long           MMSDK_S64;    // 8 bytes
#ifdef NULL
#undef NULL
#endif
/// defination for NULL
#define NULL                        0

/// define the String APP Type max length
#define MMSDK_AP_TYPE_MAX_LEN     128

/// define the display output string max length
#define MMSDK_DISP_OUTPUT_PATH_MAX_LEN     (16)

/// define the scan trick mode enum offset
#define MMSDK_TRICK_MODE_SCAN_OFFSET    0x10000000

/// define the structure version of dolby vision information
#define MMSDK_DOLBY_VISION_VERSION (1)

/// define the structure version of HDR10 Config information
#define MMSDK_HDR10_CONFIG_VERSION (1)

/// define multiple-instance item
typedef void* MEDIAITEM;

/// defination the mmsdk version combined with major and minor version
#define MMSDK_VERSION \
(((MMSDK_U32)MMSDK_VERSION_MAJOR)<<(MMSDK_VERSION_MINOR_BITS) | \
 ((MMSDK_U32)MMSDK_VERSION_MINOR))

/// defination the mmsdk porting layer version combined with major and minor version
#define MMSDK_PORTING_VERSION \
(((MMSDK_U32)MMSDK_PORTING_VERSION_MAJOR)<<(MMSDK_PORTING_VERSION_MINOR_BITS) | \
 ((MMSDK_U32)MMSDK_PORTING_VERSION_MINOR))

/// Define MMSDK debug message level
typedef enum
{
    /// disable all uart message.
    E_MMSDK_DBG_LEVEL_NONE = 0,
    /// Only output error message
    E_MMSDK_DBG_LEVEL_ERR,
    /// output general message, and above.
    E_MMSDK_DBG_LEVEL_INFO,
    /// output debug message, and above.
    E_MMSDK_DBG_LEVEL_DBG,
    /// output function tracing message, and above.
    E_MMSDK_DBG_LEVEL_TRACE,
    /// output FW message. (example: decoder debug message)
    E_MMSDK_DBG_LEVEL_FW,
} EN_MMSDK_DBG_LEVEL;


/// Define media item file type.
typedef enum
{
    /// Media item file is from mass storage file mode. The media file can support file seek.
    E_MMSDK_FILE_TYPE_MASS_STORAGE,
    /// Media item file is from network. The media file can support file seek.
    E_MMSDK_FILE_TYPE_NETWORK_STREAM_WITH_SEEK,
    /// Media item file is from network. The media file can't support file seek.
    E_MMSDK_FILE_TYPE_NETWORK_STREAM_WITHOUT_SEEK,
    /// Medai item file is for ES data type.
    E_MMSDK_FILE_TYPE_ES,
} EN_MMSDK_FILE_TYPE;

/// Deifne media item type
typedef enum
{
    /// photo
    E_MMSDK_MEDIA_TYPE_PHOTO,
    /// music
    E_MMSDK_MEDIA_TYPE_MUSIC,
    /// movie
    E_MMSDK_MEDIA_TYPE_MOVIE,
}  EN_MMSDK_MEDIA_TYPE;


/// Define media item create type.
typedef enum
{
    /// Media item is created for normal playback.
    E_MMSDK_CREATE_MODE_NORMAL_PLAY,                //Create media item for normal playback
    /// Media item is created for pre-view.
    E_MMSDK_CREATE_MODE_PREVIEW,                    //Create media item for preview
    /// Media item is created for thumbnail.
    E_MMSDK_CREATE_MODE_THUMBNAIL,                  //Create media item for get thumbnail
} EN_MMSDK_CREATE_MODE;


/// Define media item create control flag
typedef enum
{
    /// Create media item with video only.
    E_MMSDK_CTL_FLAG_VIDEO_ONLY = 0x00000001,
    /// Create media item with tick mode.
    E_MMSDK_CTL_FLAG_TRICKMODE_ON = 0x00000002,
    /// Create media item with subtitle disable.
    E_MMSDK_CTL_FLAG_SUBTITLE_DISABLE = 0x00000004,
    /// Create media item with Replay mode.(Use for Divx DRM Rental)
    E_MMSDK_CTL_FLAG_REPLAY = 0x00000008,
    /// Create media item with display in decode order if it only contains I/P frame in stream.
    /// It is for some case that needs quick display as soon as possible.
    E_MMSDK_CTL_FLAG_DISPLAY_IN_DECODING_ORDER = 0x00000010,
    /// Create media item with reducing ts demux caching size.
    /// It is for some case that needs quick decode as soon as possible.
    /// For high bitrate stream, it not recomment to set this flag.
    E_MMSDK_CTL_FLAG_REDUCE_TS_DEMUX_CACHING_SIZE = 0x00000020,
    /// Create media item with drop error frame
    E_MMSDK_CTL_FLAG_DROP_ERROR_FRAME = 0x00000040,
    /// enable/disable audio sync stc
    E_MMSDK_CTL_FLAG_DISABLE_AUDIO_SYNC_STC = 0x00000080,
    ///enable audio handler (must support open multi file, in createmediaitem arguments (u32FileID/u32FileSecondID/...))
    E_MMSDK_CTL_FLAG_ENABLE_AUDIO_HANDLER = 0x00000100,
    ///enable decode full Photo if NoThumbnail
    E_MMSDK_CTL_FLAG_ENABLE_DECODE_FULL_PHOTO_IF_NOTHUMBNAIL = 0x00000200,
    ///enable close caption
    E_MMSDK_CTL_FLAG_ENABLE_CC = 0x00000400,
    ///enable new audio mode
    E_MMSDK_CTL_FLAG_ENABLE_NEW_AUDIO_MODE = 0x00000800,
    ///enable teletext
    E_MMSDK_CTL_FLAG_ENABLE_TTX = 0x00001000,
    ///enable vdec force interlace mode(means that not display black screen at changing progressive mode to interlace mode)
    E_MMSDK_CTL_FLAG_ENABLE_VDEC_FORCE_INTERLACE_MODE = 0x00002000,
    /// photo slide show base on previous photo
    E_MMSDK_CTL_FLAG_SLIDE_BASE_ON_PREVIOUS_PHOTO = 0x00004000,
    /// disable dynamic scaling
    E_MMSDK_CTL_FLAG_DISABLE_DYNAMIC_SCALING = 0x00008000,
} EN_MMSDK_CTL_FLAG;


/// Define demux reset control flag
typedef enum
{
    /// Bit0 = TRUE, disable AV sync
    E_MMSDK_DEMUX_FLAG_DISABLE_AV_SYNC = 0x00000001,
    /// Bit1 = TRUE, Only Decode I frame
    E_MMSDK_DEMUX_FLAG_DECODE_ONLY_I_FRAME = 0x00000002,
    /// Bit2 = TRUE, disable Audio
    E_MMSDK_DEMUX_FLAG_DISABLE_AUDIO = 0x00000004,
} EN_MMSDK_DEMUX_FLAG;

/// Define trick mode speed (Movie/Music)
typedef enum
{
    /// Pause
    E_MMSDK_TRICK_PAUSE = 0,
    /// Video trick mode is step by step.
    E_MMSDK_TRICKMODE_STEP = 1,
    /// Enter normal play mode
    E_MMSDK_TRICK_NORMAL = 1000,
    /// Fast forward 1.25x
    E_MMSDK_TRICK_FF_1_2_5X = 1250,
    /// Fast forward 1.5x
    E_MMSDK_TRICK_FF_1_5X = 1500,
    /// Fast forward 2x
    E_MMSDK_TRICK_FF_2X = 2000,
    /// Fast forward 4x
    E_MMSDK_TRICK_FF_4X = 4000,
    /// Fast forward 8x
    E_MMSDK_TRICK_FF_8X = 8000,
    /// Fast forward 16x
    E_MMSDK_TRICK_FF_16X = 16000,
    /// Fast forward 32x
    E_MMSDK_TRICK_FF_32X = 32000,
    /// Fast backward 2x
    E_MMSDK_TRICK_FB_2X = -2000,
    /// Fast backward 4x
    E_MMSDK_TRICK_FB_4X = -4000,
    /// Fast backward 8x
    E_MMSDK_TRICK_FB_8X = -8000,
    /// Fast backward 16x
    E_MMSDK_TRICK_FB_16X = -16000,
    /// Fast backward 32x
    E_MMSDK_TRICK_FB_32X = -32000,
    /// Slow forward motion mode 1/2
    E_MMSDK_TRICK_SF_2X = 500,
    /// Slow forward motion mode 1/4
    E_MMSDK_TRICK_SF_4X = 250,
    /// Slow forward motion mode 1/8
    E_MMSDK_TRICK_SF_8X = 125,
    /// Slow forward motion mode 1/16
    E_MMSDK_TRICK_SF_16X = 62, //1000/16
    /// Slow motion mode 1/32
    E_MMSDK_TRICK_SF_32X = 31, //1000/32
    /// Slow backward motion mode 1/2  (unsupport)
    E_MMSDK_TRICK_SB_2X = -500,
    /// Slow backward motion mode 1/4  (unsupport)
    E_MMSDK_TRICK_SB_4X = -250,
    /// Slow backward motion mode 1/8  (unsupport)
    E_MMSDK_TRICK_SB_8X = -125,
    /// Slow backward motion mode 1/16 (unsupport)
    E_MMSDK_TRICK_SB_16X = -62, //1000/16
    /// Slow backward motion mode 1/32 (unsupport)
    E_MMSDK_TRICK_SB_32X = -31, //1000/32
    /// Slow forward scan mode 1/2, 0x100001F4
    E_MMSDK_TRICK_SFS_2X = E_MMSDK_TRICK_SF_2X + MMSDK_TRICK_MODE_SCAN_OFFSET,
    /// Slow forward scan mode 1/4, 0x100000FA
    E_MMSDK_TRICK_SFS_4X = E_MMSDK_TRICK_SF_4X + MMSDK_TRICK_MODE_SCAN_OFFSET,
    /// Slow forward scan mode 1/8, 0x1000007D
    E_MMSDK_TRICK_SFS_8X = E_MMSDK_TRICK_SF_8X + MMSDK_TRICK_MODE_SCAN_OFFSET,
    /// Slow forward scan mode 1/16, 0x1000003E
    E_MMSDK_TRICK_SFS_16X = E_MMSDK_TRICK_SF_16X + MMSDK_TRICK_MODE_SCAN_OFFSET,
    /// Slow forward scan mode 1/32, 0x1000001F
    E_MMSDK_TRICK_SFS_32X = E_MMSDK_TRICK_SF_32X + MMSDK_TRICK_MODE_SCAN_OFFSET,
    /// Fast forward scan mode 2x, 0x100007D0
    E_MMSDK_TRICK_FFS_2X = E_MMSDK_TRICK_FF_2X + MMSDK_TRICK_MODE_SCAN_OFFSET,
    /// MIN (reserved for enum space)
    E_MMSDK_TRICK_MIN = -0xFFFFFFFF,
    /// MAX (reserved for enum space)
    E_MMSDK_TRICK_MAX = 0xFFFFFFFF,
    /// invalid
    E_MMSDK_TRICK_INVALID = E_MMSDK_TRICK_MAX
} EN_MMSDK_TRICK_MODE;

/// Define rotation angle.(Photo)
typedef enum
{
    /// rotation angle is 0 degree.
    E_MMSDK_ROTATE_0 = 0,
    /// rotation angle is 90 degree.
    E_MMSDK_ROTATE_90,
    /// rotation angle is 180 degree.
    E_MMSDK_ROTATE_180,
    /// rotation angle is 270 degree.
    E_MMSDK_ROTATE_270,
    /// rotation angle is none.
    E_MMSDK_ROTATE_NOT_SUPPORT
} EN_MMSDK_ROTATE_ANGLE;

/// Define view move direction
typedef enum
{
    /// The view move direction is up.
    E_MMSDK_VIEW_MOVE_UP = 0,
    /// The view move direction is down.
    E_MMSDK_VIEW_MOVE_DOWN,
    /// The view move direction is left.
    E_MMSDK_VIEW_MOVE_LEFT,
    /// The view move direction is right.
    E_MMSDK_VIEW_MOVE_RIGHT,
}EN_MMSDK_VIEW_MOVE_DIRECTION;

/// Define media item (photo and movie) zoom factor.
typedef enum
{
    /// The area is 1/8X, the side length is 3536/10000 X
    E_MMSDK_VIEW_ZOOM_1_DIV_8 = 3536,
    /// The area is 1/4X, the side length is 5000/10000 X
    E_MMSDK_VIEW_ZOOM_1_DIV_4 = 5000,
    /// The area is 1/2X, the side length is 7071/10000 X
    E_MMSDK_VIEW_ZOOM_1_DIV_2 = 7071,
    /// The area is 1X  , the side length is 1 X
    E_MMSDK_VIEW_ZOOM_1X = 10000,
    /// The area is 2X  , the side length is 14142/10000 X
    E_MMSDK_VIEW_ZOOM_2X = 14142,
    /// The area is 4X  , the side length is 2 X
    E_MMSDK_VIEW_ZOOM_4X = 20000,
    /// The area is 8X  , the side length is 28284/10000 X
    E_MMSDK_VIEW_ZOOM_8X = 28284,
    #if 0
    /// The zoom factor is 10/12X.(real factor is 10/12)
    E_MMSDK_VIEW_ZOOM_100_DIV_144 = 8333, //0.7X
    /// The zoom factor is 10/16X.(real factor is 10/16)
    E_MMSDK_VIEW_ZOOM_100_DIV_256 = 6250, //0.4X
    /// The zoom factor is 10/18X.(real factor is 10/18)
    E_MMSDK_VIEW_ZOOM_100_DIV_324 = 5555, //0.3X
    /// The zoom factor is 1.2X.(real factor is 12/10)
    E_MMSDK_VIEW_ZOOM_144_DIV_100 = 12000, //1.44X
    /// The zoom factor is 1.6X.(real factor is 16/10)
    E_MMSDK_VIEW_ZOOM_256_DIV_100 = 16000, //2.56X
    /// The zoom factor is 1.8X.(real factor is 18/10)
    E_MMSDK_VIEW_ZOOM_324_DIV_100 = 18000, //3.24X
    #endif
}EN_MMSDK_VIEW_ZOOM_FACTOR;


/// Define video repeat A point to B point setting. (Movie)
typedef enum
{
    /// Video repeat point is none. Back to normal playback.
    E_MMSDK_REPEAT_NONE = 0,
    /// Video repeat point is A point (start point).
    E_MMSDK_REPEAT_A,
    /// Video repeat point is B point (end point).
    E_MMSDK_REPEAT_B,
} EN_MMSDK_REPEAT_AB;

/// Define the information for the resume playback. (play the file from previous stop time).
typedef struct
{
    /// File position (To increase accuracy of TS and MPEG)
    MMSDK_U64 u64FilePos;
    /// Resume Play Time in millisecond
    MMSDK_U32 u32TimeInMs;
    /// Reserved
    MMSDK_U32 u32Reserved;
    /// title index (Movie DivX only)
    MMSDK_U8 u8TitleID;
    /// edition index (Movie DivX only)
    MMSDK_U8 u8EditionID;
    /// audio index (Movie only)
    MMSDK_U8 u8AudioID;
    /// subtitle index (Movie only)
    MMSDK_U8 u8SubtitleID;
    /// Program index (Movie TS file only)
    MMSDK_U8 u8ProgramID;
    /// Reserved
    MMSDK_U8 u8Reserved;
} ST_MMSDK_RESUME_INFO;


//================DISPLAY=====================

/// Define the information for view offset.
typedef struct
{
    /// Horizontal direction offset
    MMSDK_S32 s32XOffset;
    /// Vertical direction offset
    MMSDK_S32 s32YOffset;
} ST_MMSDK_VIEW_OFFSET;


/// Define Aspect ratio
typedef enum
{
    /// auto (auto-fit the display rect)
    E_MMSDK_ASPECT_RATIO_AUTO = 0,
    /// Strech to 4x3
    E_MMSDK_ASPECT_RATIO_4X3,
    /// Strech to 16x9
    E_MMSDK_ASPECT_RATIO_16X9,
    /// For STB: output for  4:3 TV
    /// pan & scan
    E_MMSDK_ASPECT_RATIO_4X3_PAN_SCAN,
    /// For STB: output for 16:9 TV
    /// pan & scan
    E_MMSDK_ASPECT_RATIO_16X9_PAN_SCAN,
    /// For STB: output for  4:3 TV
    /// letter box
    E_MMSDK_ASPECT_RATIO_4X3_LETTER_BOX,
    /// For STB: output for 16:9 TV
    /// pillar box
    E_MMSDK_ASPECT_RATIO_16X9_PILLARBOX,
}EN_MMSDK_ASPECT_RATIO;


/// Define photo slide show effect.
typedef enum
{
    /// Slide show effect is normal.
    E_MMSDK_SLIDE_SHOW_EFFECT_NORMAL,
    /// Slide show effect is random.
    E_MMSDK_SLIDE_SHOW_EFFECT_RANDOM,
    /// Slide show effect is block display (horizontal raster order scan, the first block in top and left corner).
    E_MMSDK_SLIDE_SHOW_EFFECT_HORZ_BLOCKS_TOPLEFT,
    /// Slide show effect is block display (horizontal raster order scan, the first block in top and right corner).
    E_MMSDK_SLIDE_SHOW_EFFECT_HORZ_BLOCKS_TOPRIGHT,
    /// Slide show effect is block display (horizontal raster order scan, the first block in bottom and left corner).
    E_MMSDK_SLIDE_SHOW_EFFECT_HORZ_BLOCKS_BOTTOMLEFT,
    /// Slide show effect is block display (horizontal raster order scan, the first block in bottom and right corner).
    E_MMSDK_SLIDE_SHOW_EFFECT_HORZ_BLOCKS_BOTTOMRIGHT,
    /// Slide show effect is block display (vertical raster order scan, the first block in top and left corner).
    E_MMSDK_SLIDE_SHOW_EFFECT_VERT_BLOCKS_TOPLEFT,
    /// Slide show effect is block display (vertical raster order scan, the first block in top and right corner).
    E_MMSDK_SLIDE_SHOW_EFFECT_VERT_BLOCKS_TOPRIGHT,
    /// Slide show effect is block display (vertical raster order scan, the first block in bottom and left corner).
    E_MMSDK_SLIDE_SHOW_EFFECT_VERT_BLOCKS_BOTTOMLEFT,
    /// Slide show effect is block display (vertical raster order scan, the first block in bottom and right corner).
    E_MMSDK_SLIDE_SHOW_EFFECT_VERT_BLOCKS_BOTTOMRIGHT,
    /// Slide show effect is block display (horizontal zigzag order scan, the first block in top and left corner).
    E_MMSDK_SLIDE_SHOW_EFFECT_HORZ_ZIGZAG_BLOCKS_TOPLEFT,
    /// Slide show effect is block display (horizontal zigzag order scan, the first block in top and right corner).
    E_MMSDK_SLIDE_SHOW_EFFECT_HORZ_ZIGZAG_BLOCKS_TOPRIGHT,
    /// Slide show effect is block display (horizontal zigzag order scan, the first block in bottom and left corner).
    E_MMSDK_SLIDE_SHOW_EFFECT_HORZ_ZIGZAG_BLOCKS_BOTTOMLEFT,
    /// Slide show effect is block display (horizontal zigzag order scan, the first block in bottom and right corner).
    E_MMSDK_SLIDE_SHOW_EFFECT_HORZ_ZIGZAG_BLOCKS_BOTTOMRIGHT,
    /// Slide show effect is block display (vertical zigzag order scan, the first block in top and left corner).
    E_MMSDK_SLIDE_SHOW_EFFECT_VERT_ZIGZAG_BLOCKS_TOPLEFT,
    /// Slide show effect is block display (vertical zigzag order scan, the first block in top and right corner).
    E_MMSDK_SLIDE_SHOW_EFFECT_VERT_ZIGZAG_BLOCKS_TOPRIGHT,
    /// Slide show effect is block display (vertical zigzag order scan, the first block in bottom and left corner).
    E_MMSDK_SLIDE_SHOW_EFFECT_VERT_ZIGZAG_BLOCKS_BOTTOMLEFT,
    /// Slide show effect is block display (vertical zigzag order scan, the first block in bottom and right corner).
    E_MMSDK_SLIDE_SHOW_EFFECT_VERT_ZIGZAG_BLOCKS_BOTTOMRIGHT,
    /// Slide show effect is random block display
    E_MMSDK_SLIDE_SHOW_EFFECT_BLOCKS_RANDOM,
    /// Slide show effect is block display (disgonal scan, the first block in top and left corner).
    E_MMSDK_SLIDE_SHOW_EFFECT_DIAGONAL_BLOCKS_TOPLEFT,
    /// Slide show effect is block display (disgonal scan, the first block in top and right corner).
    E_MMSDK_SLIDE_SHOW_EFFECT_DIAGONAL_BLOCKS_TOPRIGHT,
    /// Slide show effect is block display (disgonal scan, the first block in bottom and left corner).
    E_MMSDK_SLIDE_SHOW_EFFECT_DIAGONAL_BLOCKS_BOTTOMLEFT,
    /// Slide show effect is block display (disgonal scan, the first block in bottom and right corner).
    E_MMSDK_SLIDE_SHOW_EFFECT_DIAGONAL_BLOCKS_BOTTOMRIGHT,
    /// Slide show effect is bar wipe display (bar scan order is left to right).
    E_MMSDK_SLIDE_SHOW_EFFECT_BARWIPE_LEFT2RIGHT,
    /// Slide show effect is bar wipe display (bar scan order is right to left).
    E_MMSDK_SLIDE_SHOW_EFFECT_BARWIPE_RIGHT2LEFT,
    /// Slide show effect is bar wipe display (bar scan order is top to bottom).
    E_MMSDK_SLIDE_SHOW_EFFECT_BARWIPE_TOP2BOTTOM,
    /// Slide show effect is bar wipe display (bar scan order is bottom to top).
    E_MMSDK_SLIDE_SHOW_EFFECT_BARWIPE_BOTTOM2TOP,
    /// Slide show effect is box wipe display (box start from top and left corner).
    E_MMSDK_SLIDE_SHOW_EFFECT_BOXWIPE_TOPLEFT,
    /// Slide show effect is box wipe display (box start from top and right corner).
    E_MMSDK_SLIDE_SHOW_EFFECT_BOXWIPE_TOPRIGHT,
    /// Slide show effect is box wipe display (box start from bottom and left corner).
    E_MMSDK_SLIDE_SHOW_EFFECT_BOXWIPE_BOTTOMLEFT,
    /// Slide show effect is box wipe display (box start from bottom and right corner).
    E_MMSDK_SLIDE_SHOW_EFFECT_BOXWIPE_BOTTOMRIGHT,
    /// Slide show effect is barndoor wipe display (horizontal barndoor open).
    E_MMSDK_SLIDE_SHOW_EFFECT_BARNDOORWIPE_HORIZONTAL_OPEN,
    /// Slide show effect is barndoor wipe display (horizontal barndoor close).
    E_MMSDK_SLIDE_SHOW_EFFECT_BARNDOORWIPE_HORIZONTAL_CLOSE,
    /// Slide show effect is barndoor wipe display (vertical barndoor open).
    E_MMSDK_SLIDE_SHOW_EFFECT_BARNDOORWIPE_VERTICAL_OPEN,
    /// Slide show effect is barndoor wipe display (vertical barndoor close).
    E_MMSDK_SLIDE_SHOW_EFFECT_BARNDOORWIPE_VERTICAL_CLOSE,
    /// Slide show effect is box wipe display (box start from top center).
    E_MMSDK_SLIDE_SHOW_EFFECT_BOXWIPE_TOPCENTER,
    /// Slide show effect is box wipe display (box start from right center).
    E_MMSDK_SLIDE_SHOW_EFFECT_BOXWIPE_RIGHTCENTER,
    /// Slide show effect is box wipe display (box start from bottom center).
    E_MMSDK_SLIDE_SHOW_EFFECT_BOXWIPE_BOTTOMCENTER,
    /// Slide show effect is box wipe display (box start from left center).
    E_MMSDK_SLIDE_SHOW_EFFECT_BOXWIPE_LEFTCENTER,
    /// Slide show effect is iris wipe display (iris start from center).
    E_MMSDK_SLIDE_SHOW_EFFECT_IRISWIPE_CENTER,
    /// Slide show effect is iris wipe display (iris start from outer).
    E_MMSDK_SLIDE_SHOW_EFFECT_IRISWIPE_OUTER,
    /// Slide show effect is 4 horizontal bar wipe display (start for each bar top side).
    E_MMSDK_SLIDE_SHOW_EFFECT_4BARWIPE_HORIZONTAL_FROM_TOP,
    /// Slide show effect is 4 horizontal bar wipe display (start for each bar bottom side).
    E_MMSDK_SLIDE_SHOW_EFFECT_4BARWIPE_HORIZONTAL_FROM_BOTTOM,
    /// Slide show effect is 4 vertical bar wipe display (start for each bar left side).
    E_MMSDK_SLIDE_SHOW_EFFECT_4BARWIPE_VERTICAL_FROM_LEFT,
    /// Slide show effect is 4 vertical bar wipe display (start for each bar right side).
    E_MMSDK_SLIDE_SHOW_EFFECT_4BARWIPE_VERTICAL_FROM_RIGHT,
    /// Slide show effect is bar slide display (bar start from left to right).
    E_MMSDK_SLIDE_SHOW_EFFECT_BARSLIDE_LEFT2RIGHT,
    /// Slide show effect is bar slide display (bar start from right to left).
    E_MMSDK_SLIDE_SHOW_EFFECT_BARSLIDE_RIGHT2LEFT,
    /// Slide show effect is bar slide display (bar start from top to bottom).
    E_MMSDK_SLIDE_SHOW_EFFECT_BARSLIDE_TOP2BOTTOM,
    /// Slide show effect is bar slide display (bar start from bottom to top).
    E_MMSDK_SLIDE_SHOW_EFFECT_BARSLIDE_BOTTOM2TOP,
    /// Slide show effect is box slide display (box start from top and left corner).
    E_MMSDK_SLIDE_SHOW_EFFECT_BOXSLIDE_TOPLEFT,
    /// Slide show effect is box slide display (box start from top and right corner).
    E_MMSDK_SLIDE_SHOW_EFFECT_BOXSLIDE_TOPRIGHT,
    /// Slide show effect is box slide display (box start from bottom and left corner).
    E_MMSDK_SLIDE_SHOW_EFFECT_BOXSLIDE_BOTTOMLEFT,
    /// Slide show effect is box slide display (box start from bottom and right corner).
    E_MMSDK_SLIDE_SHOW_EFFECT_BOXSLIDE_BOTTOMRIGHT,
    /// Slide show effect is barndoor slide display (horizontal barndoor open).
    E_MMSDK_SLIDE_SHOW_EFFECT_BARNDOORSLIDE_HORIZONTAL_OPEN,
    /// Slide show effect is barndoor slide display (horizontal barndoor close).
    E_MMSDK_SLIDE_SHOW_EFFECT_BARNDOORSLIDE_HORIZONTAL_CLOSE,
    /// Slide show effect is barndoor slide display (vertical barndoor open).
    E_MMSDK_SLIDE_SHOW_EFFECT_BARNDOORSLIDE_VERTICAL_OPEN,
    /// Slide show effect is barndoor slide display (vertical barndoor close).
    E_MMSDK_SLIDE_SHOW_EFFECT_BARNDOORSLIDE_VERTICAL_CLOSE,
    /// Slide show effect is 4 horizontal bar slide display (start for each bar top side).
    E_MMSDK_SLIDE_SHOW_EFFECT_4BARSLIDE_HORIZONTAL_FROM_TOP,
    /// Slide show effect is 4 horizontal bar wipe display (start for each bar bottom side).
    E_MMSDK_SLIDE_SHOW_EFFECT_4BARSLIDE_HORIZONTAL_FROM_BOTTOM,
    /// Slide show effect is 4 vertical bar wipe display (start for each bar left side).
    E_MMSDK_SLIDE_SHOW_EFFECT_4BARSLIDE_VERTICAL_FROM_LEFT,
    /// Slide show effect is 4 vertical bar wipe display (start for each bar right side).
    E_MMSDK_SLIDE_SHOW_EFFECT_4BARSLIDE_VERTICAL_FROM_RIGHT,
    /// Slide show effect is fade in.
    E_MMSDK_SLIDE_SHOW_EFFECT_FADE_IN,
    /// Slide show effect is fade out.
    E_MMSDK_SLIDE_SHOW_EFFECT_FADE_OUT,
    /// Slide show effect is fade in and fade out at the same time.
    E_MMSDK_SLIDE_SHOW_EFFECT_FADE_IN_AND_OUT,
    // If add a new enum, please modify the calculation of E_MMSDK_SLIDE_SHOW_EFFECT_RANDOM in movie player
} EN_MMSDK_PHOTO_SLIDE_SHOW_EFFECT;


/// Define media app type
typedef enum
{
    /// Create media item for normal.
    E_MMSDK_AP_TYPE_NORMAL = 0,
    /// Create media item for netflix.
    E_MMSDK_AP_TYPE_NETFLIX,
    /// Create media item for dlna.
    E_MMSDK_AP_TYPE_DLNA,
    /// Create media item for hbbtv
    E_MMSDK_AP_TYPE_HBBTV,
    /// Create media item for webbrowser
    E_MMSDK_AP_TYPE_WEBBROWSER,
    /// Create media item for wmdrm10
    E_MMSDK_AP_TYPE_WMDRM10,
    /// Create media item for android usb
    E_MMSDK_AP_TYPE_ANDROID_USB,
    /// Create media item for android streaming
    E_MMSDK_AP_TYPE_ANDROID_STREAMING,
    /// Create media item for RVU
    E_MMSDK_AP_TYPE_RVU,
    /// Create media item for YOUTUBE (autoplay)
    E_MMSDK_AP_TYPE_YOUTUBE,
    /// Create media item for MSS
    E_MMSDK_AP_TYPE_MSS,
    /// Create media item for CSP
    E_MMSDK_AP_TYPE_CSP,
    /// Create media item for ADVERT Player
    E_MMSDK_AP_TYPE_ADVERT,
    /// Find ini setting by string
    E_MMSDK_AP_TYPE_STRING = 16
} EN_MMSDK_AP_TYPE;


/// Define signed position rectangle information.
typedef struct
{
    /// Horizontal start.
    MMSDK_S32 s32X;
    /// Vertical start.
    MMSDK_S32 s32Y;
    /// Width.
    MMSDK_U16 u16Width;
    /// Height.
    MMSDK_U16 u16Height;
}ST_MMSDK_RECT;

/// Define Crop and Display window setting for movie and photo
typedef struct
{
    /// Crop region of the source (video only) 0,0,0,0 will set to default, keep all source region
    ST_MMSDK_RECT stCropRect;
    /// Display region in the panel (video and photo) 0,0,0,0 will set to default, display region will set to full screen
    ST_MMSDK_RECT stDispRect;
    /// Keep ratio or stretch to display window
    MMSDK_BOOL bkeepRatio;
    /// Black Screen when setting window
    MMSDK_BOOL bBlackScreen;
}ST_MMSDK_CROP_DISPLAY_WINDOW;


//================DISPLAY=====================


/// Define audio information (for ES Player)
typedef struct
{
    /// Scale
    MMSDK_U32 u32Scale;               // AVI                      : MP2/ MP3
    /// Rate
    MMSDK_U32 u32Rate;                // AVI                      : MP3
    /// Length
    MMSDK_U32 u32Length;              // AVI                      : No use now.
    /// SampleSize
    MMSDK_U32 u32SampleSize;          // AVI/ RM                  : MP2/ MP3
    /// Channel
    MMSDK_U32 u32Channel;             // AVI/ MOV/ MKV/ RM/ MPG   : AAC/ PCM/ ADPCM
    /// SampleRate
    MMSDK_U32 u32SampleRate;          // AVI/ MOV/ MKV/ RM/ MPG   : AAC/ PCM/ ADPCM
    /// BitRate
    MMSDK_U32 u32BitRate;             // AVI/ RM/ MPG             : MP2
    /// BlockAlign
    MMSDK_U32 u32BlockAlign;          // AVI                      : ADPCM
    /// BitsPerSample
    MMSDK_U32 u32BitsPerSample;       // AVI/ MOV/ MKV/ MPG       : PCM/ ADPCM
    /// SamplesPerBlock
    MMSDK_U32 u32SamplesPerBlock;     // AVI/ MPG                 : ADPCM
    /// ObjectTypeID
    MMSDK_U32 u32ObjectTypeID;        // AVI/ MOV/ MKV            : AAC
    /// ChannelMask
    MMSDK_U32 u32ChannelMask;         // ASF/ WMV/ WMA            : WMA Pro
    /// EncodeOpt
    MMSDK_U16 u16EncodeOpt;           // ASF/ WMV/ WMA            : WMA
    /// FormatTag
    MMSDK_U16 u16FormatTag;           // MKV
    /// Version
    MMSDK_U8 u8Version;               // ASF/ WMV/ WMA            : WMA
    /// AdvanceEncodeOpt
    MMSDK_U16 u16AdvanceEncodeOpt;    // ASF/ WMV/ WMA            : WMA
} ST_MMSDK_ES_AUDIO_PARAM;

//TODO:
/// Define audio codec type
typedef enum
{
    /// Unkown
    E_MMSDK_AUDIO_CODEC_UNKNOWN = -1,
    /// AAC
    E_MMSDK_AUDIO_CODEC_AAC = 0x100,
    /// AC3
    E_MMSDK_AUDIO_CODEC_AC3 = 0x200,
    /// AC3 Plus
    E_MMSDK_AUDIO_CODEC_AC3P,
    /// AMR NB
    E_MMSDK_AUDIO_CODEC_AMR_NB = 0x300,
    /// AMR WB
    E_MMSDK_AUDIO_CODEC_AMR_WB = 0x310,
    /// DTS
    E_MMSDK_AUDIO_CODEC_DTS = 0x400,
    /// DTS Express (LBR)
    E_MMSDK_AUDIO_CODEC_DTSLBR,
    /// DTS HD (reserved.)
    E_MMSDK_AUDIO_CODEC_DTSHD,
    /// DTS HD Master Audio
    E_MMSDK_AUDIO_CODEC_DTSHD_MA,
    /// DTS HD High Resolution Audio (reserved.)
    E_MMSDK_AUDIO_CODEC_DTSHD_HRA,
    /// FLAC
    E_MMSDK_AUDIO_CODEC_FLAC = 0x500,
    /// MP3
    E_MMSDK_AUDIO_CODEC_MP3 = 0x600,
    /// MP2
    E_MMSDK_AUDIO_CODEC_MPEG,
    /// COOK
    E_MMSDK_AUDIO_CODEC_COOK = 0x700,
    /// COOK RA8LBR
    E_MMSDK_AUDIO_CODEC_RA8LBR = E_MMSDK_AUDIO_CODEC_COOK,
    /// Vorbis
    E_MMSDK_AUDIO_CODEC_VORBIS = 0x800,
    /// DRA
    E_MMSDK_AUDIO_CODEC_DRA = 0x900,
    /// WMA1 and WMA2
    E_MMSDK_AUDIO_CODEC_WMA = 0x1000,
    /// WMA1
    E_MMSDK_AUDIO_CODEC_WMA1,
    /// WMA2
    E_MMSDK_AUDIO_CODEC_WMA2,
    /// WMA3 (PRO)
    E_MMSDK_AUDIO_CODEC_WMA3,
    /// WMA3 (PRO)
    E_MMSDK_AUDIO_CODEC_WMA_PRO = E_MMSDK_AUDIO_CODEC_WMA3,
    /// Unknown PCM type...
    E_MMSDK_AUDIO_CODEC_XPCM = 0x1100,
    /// PCM
    E_MMSDK_AUDIO_CODEC_PCM,
    /// PCM MULAW
    E_MMSDK_AUDIO_CODEC_PCM_MULAW,
    /// ADCPM
    E_MMSDK_AUDIO_CODEC_ADPCM = 0x1200,
}EN_MMSDK_AUDIO_CODEC;


//TODO:
/// Define video codec type
typedef enum
{
    /// Video codec type is unknown.
    E_MMSDK_VIDEO_CODEC_UNKNOWN = -1,
    /// Video codec type is MPEG 4.
    E_MMSDK_VIDEO_CODEC_MPEG4,
    /// Video codec type is motion JPG.
    E_MMSDK_VIDEO_CODEC_MJPEG,
    /// Video codec type is H264.
    E_MMSDK_VIDEO_CODEC_H264,
    /// Video codec type is RealVideo.
    E_MMSDK_VIDEO_CODEC_RM,
    /// Video codec type is TS File.
    E_MMSDK_VIDEO_CODEC_TS,
    /// Video codec type is MPEG 1/2.
    E_MMSDK_VIDEO_CODEC_MPG,
    /// Video codec type is VC1.
    E_MMSDK_VIDEO_CODEC_VC1,
    /// Video codec type is Audio Video Standard.
    E_MMSDK_VIDEO_CODEC_AVS,
    /// Video codec type is FLV.
    E_MMSDK_VIDEO_CODEC_FLV,
    /// Video codec type is MVC.
    E_MMSDK_VIDEO_CODEC_MVC,
    /// Video codec type is VP6.
    E_MMSDK_VIDEO_CODEC_VP6,
    /// Video codec type is VP8.
    E_MMSDK_VIDEO_CODEC_VP8,
    /// Video codec type is HEVC.
    E_MMSDK_VIDEO_CODEC_HEVC,
    /// Video codec type is VP9.
    E_MMSDK_VIDEO_CODEC_VP9,
    /// Video codec type is HEVC Dolby Vision.
    E_MMSDK_VIDEO_CODEC_HEVC_DV,
}EN_MMSDK_VIDEO_CODEC;

//TODO:
/// Define subtitle type
typedef enum
{
    E_MMSDK_SUBTITLE_TYPE_NONE = 0,
    /// text base subtitle
    E_MMSDK_SUBTITLE_TYPE_TEXT = 0x00010000,
    /// SRT
    E_MMSDK_SUBTITLE_TYPE_TEXT_SRT,
    /// UTF8
    E_MMSDK_SUBTITLE_TYPE_TEXT_UTF8,
    /// ASS
    E_MMSDK_SUBTITLE_TYPE_TEXT_ASS,
    /// SSA
    E_MMSDK_SUBTITLE_TYPE_TEXT_SSA,
    /// 3GPP
    E_MMSDK_SUBTITLE_TYPE_TEXT_3GPP,
    /// USF
    E_MMSDK_SUBTITLE_TYPE_TEXT_USF,
    /// image base subtitle
    E_MMSDK_SUBTITLE_TYPE_IMAGE = 0x00020000,
    /// BMP
    E_MMSDK_SUBTITLE_TYPE_IMAGE_BMP,
    /// VOB SUB
    E_MMSDK_SUBTITLE_TYPE_IMAGE_VOBSUB,
    /// TS BMP
    E_MMSDK_SUBTITLE_TYPE_IMAGE_TS_BMP,
    /// TS PGS
    E_MMSDK_SUBTITLE_TYPE_IMAGE_TS_PGS,
}EN_MMSDK_SUBTITLE_TYPE;

/// Define the string format
typedef enum
{
    /// String data type is ASCII.
    E_MMSDK_STRING_FORMAT_ASCII,
    /// String data type is UTF-8.
    E_MMSDK_STRING_FORMAT_UTF8,
    /// String data type is Unicode.
    E_MMSDK_STRING_FORMAT_UNICODE
} EN_MMSDK_STRING_FORMAT;

/// Define MMSDK photo type.
typedef enum
{
    /// Photo image type is JPG baseline.
    E_MMSDK_PHOTO_JPEG_BASELINE,
    /// Photo image type is JPG progressive.
    E_MMSDK_PHOTO_JPEG_PROGRESSIVE,
    /// Photo image type is BMP.
    E_MMSDK_PHOTO_BMP,
    /// Photo image type is PNG non-interlace.
    E_MMSDK_PHOTO_PNG_NON_INTERLACED,
    /// Photo image type is PNG interlace.
    E_MMSDK_PHOTO_PNG_INTERLACED,
    /// Photo image type is GIF without animation
    E_MMSDK_PHOTO_GIF_NON_ANIMATION,
    /// Photo image type is GIF with animation
    E_MMSDK_PHOTO_GIF_ANIMATION,
    /// Photo image type is MPO
    E_MMSDK_PHOTO_MPO,
    /// Photo image type is none.
    E_MMSDK_PHOTO_UNKNOWN,
} EN_MMSDK_PHOTO_TYPE;

/// Define MMSDK color format.
typedef enum
{
    ///Photo format type is unknown.
    E_MMSDK_COLOR_FORMAT_UNKNOWN = -1,
    ///ARGB888
    E_MMSDK_COLOR_FORMAT_ARGB8888,
    ///ARG1555
    E_MMSDK_COLOR_FORMAT_ARGB1555,
    ///ARG4444
    E_MMSDK_COLOR_FORMAT_ARGB4444,
    ///YUV422
    E_MMSDK_COLOR_FORMAT_YUV422,
    ///YVYU
    E_MMSDK_COLOR_FORMAT_YVYU,
} EN_MMSDK_COLOR_FORMAT;

/// Define the information for thumbnail process.
typedef struct
{
    /// Thumbnail color format
    EN_MMSDK_COLOR_FORMAT enColorFmt;
    /// Buff Size for capture thumbnail buffer.
    MMSDK_U32 u32BuffSize;
    /// Thumbnail image width.
    MMSDK_U16 u16Width;
    /// Thumbnail image height.
    MMSDK_U16 u16Height;
    /// Background color for aplha. black (A,R,G,B) = (0,0,0,0)
    MMSDK_U8 u8A;
    /// Background color for red.  black (A,R,G,B) = (0,0,0,0)
    MMSDK_U8 u8R;
    /// Background color for green. black (A,R,G,B) = (0,0,0,0)
    MMSDK_U8 u8G;
    /// Background color for green. black (A,R,G,B) = (0,0,0,0)
    MMSDK_U8 u8B;
    /// Keep the original aspect ratio or not.
    MMSDK_BOOL bKeepAspectRatio;
} ST_MMSDK_THUMBNAIL_PARAM;

/// Defined ES audio info
typedef struct
{
    /// audio codec type
    EN_MMSDK_AUDIO_CODEC enCodecID;
    /// audio param
    ST_MMSDK_ES_AUDIO_PARAM stParam;
}ST_MMSDK_ES_AUDIO_INFO;

/// Defined ES video info
typedef struct
{
    /// video codec:
    EN_MMSDK_VIDEO_CODEC enVideoType;
    /// video width
    MMSDK_U32 u32VideoWidth;
    /// video height
    MMSDK_U32 u32VideoHeight;
    /// video rate
    MMSDK_U32 u32VideoRate;
    /// video disable AV sync
    MMSDK_BOOL bAvSyncDisable;
    /// video drop error_frame
    MMSDK_BOOL bDropErrorFrame;
    /// video PTS Mode or DTS Mode
    MMSDK_BOOL bPtsMode;
    /// Live/Game mode
    MMSDK_BOOL bLivePlayback;
    /// H264 insert Nalsize
    MMSDK_U8 u8NalLenSizeUnit;
    /// Enable seamless playback feature
    MMSDK_BOOL bEnableSeamless;
}ST_MMSDK_ES_VIDEO_INFO;

/// Define ES Player Data type
typedef enum
{
    /// audio
    E_MMSDK_ES_DATA_TYPE_AUDIO,
    /// video
    E_MMSDK_ES_DATA_TYPE_VIDEO,
    /// video and audio
    E_MMSDK_ES_DATA_TYPE_VIDEO_AUDIO,
    /// subtitle
    E_MMSDK_ES_DATA_TYPE_SUBTITLE,
    /// all of above
    E_MMSDK_ES_DATA_TYPE_ALL,
    /// Audio codec info ENABLE_SEAMLESS_ESPLAYER
    E_MMSDK_ES_DATA_INFO_AUDIO,
    /// Video codec info ENABLE_SEAMLESS_ESPLAYER
    E_MMSDK_ES_DATA_INFO_VIDEO,
    /// Subtitle codec info ENABLE_SEAMLESS_ESPLAYER
    E_MMSDK_ES_DATA_INFO_SUBTITLE,
    /// Vorbis Header Info
    E_MMSDK_ES_DATA_TYPE_VORBIS_HEADER,
}EN_MMSDK_ES_DATA_TYPE;

/// Defined ES INPUT DATA INFORMATION
typedef struct
{
    /// Data start address
    MMSDK_U32 u32StartAddr;
    /// Data u32Size u32Size
    MMSDK_U32 u32Size;
    /// Data u32TimeStamp
    MMSDK_U32 u32TimeStamp;
    /// Input Data Type
    EN_MMSDK_ES_DATA_TYPE enDataType;
}ST_MMSDK_ES_DATA_INFO;

/// Defined ES INPUT DATA INFORMATION for 64 bit based pts
typedef struct
{
    // The es data data information
    ST_MMSDK_ES_DATA_INFO* pstESDataInfo;
    // The 64bit base time stamp
    MMSDK_U64 u64TimeStamp;
    // Dolby Vision dual track index: track 1 base layer, track 2 enhance layer
    MMSDK_U32 u32DVDualTrackIndex;
    // The reserved parameter
    void *pParam;
}ST_MMSDK_ES_DATA_INFO64;

/// Defined player and media information
typedef enum
{
    /// TODO: is zoom avaliable (need to merge CL#555624)
    /// If scaler frame buffer size is not enough, zoom is not avaliable
    /// Movie : Is Zoom Valid; Param: MMSDK_BOOL
    E_MMSDK_INFO_MOVIE_IS_ZOOM_VALID = 0,
    /// Movie: set trick mode option; Param: MMSDK_U32
    E_MMSDK_INFO_TRICK_MODE_OPTION,
    /// Music: set time-based seek on/off; Param: MMSDK_BOOL
    E_MMSDK_INFO_TIME_BASED_SEEK_ON,
    /*==================== Comment ====================*/
    /// horizontal resolution (Photo/Movie); Param: MMSDK_U32
    E_MMSDK_INFO_HRES = 0x0800,
    /// vertical resolution (Photo/Movie); Param: MMSDK_U32
    E_MMSDK_INFO_VRES = 0x0801,
    /// current playing time (ms) (Music/Movie); Param: MMSDK_U32
    E_MMSDK_INFO_CURRENT_PLAYTIME = 0x0802,
    /// total time (s) (Music/Movie), total time invalid: 0xFFFFFFFF (U32 MAX); Param: MMSDK_U32
    E_MMSDK_INFO_TOTAL_TIME = 0x0803,
    /// total time is valid or not (Reserved)(Music/Movie); Param: MMSDK_BOOL
    E_MMSDK_INFO_TOTAL_TIME_ISVALID = 0x0804,
    /// TODO: channel mode: This routine is used to report decoder channel mode information.(Music/Movie); Param: MMSDK_
    E_MMSDK_INFO_CHANNEL_MODE = 0x0805,
    /// TODO; Param: MMSDK_
    E_MMSDK_INFO_CODEC_EXTRA_CMD = 0x0806,
    /// TODO; check the display(Photo/Movie) is over screen or not; Param: MMSDK_BOOL
    E_MMSDK_INFO_DISPLAY_IS_OVER_SCREEN = 0x0807,
    /// Set disable display(Photo/Movie) moved over screen; Param: MMSDK_BOOL
    E_MMSDK_INFO_DISABLE_DISPLAY_MOVE_OVER_SCREEN = 0x0808,
    /// Get player state; Param: EN_MMSDK_PLAYER_STATE
    E_MMSDK_INFO_PLAYER_STATE = 0x0809,
    /// Check all players are finalized; Param: MMSDK_BOOL
    E_MMSDK_INFO_PLAYER_IS_FINALIZED = 0x080A,
    /// Get VDEC codec capability; Param: ST_MMSDK_CODEC_CAP_INFO
    E_MMSDK_INFO_GET_CODEC_CAPABILITY = 0x080B,
    /// current playing time in 64bit base (ms) (Music/Movie); Param: MMSDK_U64
    E_MMSDK_INFO_CURRENT_PLAYTIME64 = 0x080C,
    /// create media item error code; Param: MMSDK_U32
    E_MMSDK_INFO_CREATE_ERROR_CODE = 0x080D,
    /// Pre load symbol for avp related libs for speedup playback; Param: None
    E_MMSDK_INFO_PRELOAD_SYMBOL = 0x080E,
    //========================== Reserved for Download module ============================/
    /// [Download module] set buffer manager size
    /// set info should be used before MMSDK_CreateMediaItem
    /// Param: MMSDK_U32
    E_MMSDK_INFO_DM_BUFFER_MANAGER_SIZE = 0x0820,
    /// [Download module] set high buffer level,
    /// set info should be used before MMSDK_CreateMediaItem
    /// when buffer rich high water level we will notify E_MMSDK_CALLBACK_DATA_BUFFERING_STOP
    /// Param: MMSDK_U32
    E_MMSDK_INFO_DM_BUFFER_MANAGER_HIGH_WATER_LEVEL = 0x0821,
    /// [Download module] set low buffer level,
    /// set info should be used before MMSDK_CreateMediaItem
    /// when buffer in low water level we will notify E_MMSDK_CALLBACK_DATA_BUFFERING_START
    /// Param: MMSDK_U32
    E_MMSDK_INFO_DM_BUFFER_MANAGER_LOW_WATER_LEVEL = 0x0822,
    /// [Download module] get current buffer manager water level
    /// get info should be used after MMSDK_CreateMediaItem
    /// Param: MMSDK_U32
    E_MMSDK_INFO_DM_BUFFER_MANAGER_CURRENT_WATER_LEVEL = 0x0823,
    /// Reserved Max (don't used this value)
    E_MMSDK_INFO_DM_RESERVED = 0x0840,
    //========================== Reserved for Download module End ========================/

    /*==================== Comment End ====================*/


    /*==================== Movie ====================*/
    /// movie error code; Param: MMSDK_U32
    E_MMSDK_INFO_MOVIE_ERROR_CODE = 0x1000,
    /// index existed; Param: MMSDK_BOOL
    E_MMSDK_INFO_MOVIE_INDEX_EXISTED,
    /// video frame per second. For 25 Fame/sec, the metadta is 25000.; Param: MMSDK_U32
    E_MMSDK_INFO_MOVIE_FRAME_PER_SEC,
    /// horizontal resolution; Param: MMSDK_U32
    E_MMSDK_INFO_MOVIE_DAR_WIDTH,
    /// vertical resolution; Param: MMSDK_U32
    E_MMSDK_INFO_MOVIE_DAR_HIGHT,
    /// TODO: current video track is 3D or not; Param: MMSDK_BOOL
    E_MMSDK_INFO_MOVIE_IS_3D,
    /// TODO: current video track 3D layout; Param: EN_MMSDK_3DLAYOUT
    E_MMSDK_INFO_MOVIE_3D_LAYOUT,
    /// TODO: Report all track 3D info, the size is ((Track num)*sizeof(MMSDK_U32))
    E_MMSDK_INFO_MOVIE_MULTI_VIDEO_3D_LAYOUT,
    /// current video codec; Param EN_MMSDK_VIDEO_CODEC
    E_MMSDK_INFO_MOVIE_VIDEO_CODEC,
    /// current audio codec; Param EN_MMSDK_AUDIO_CODEC
    E_MMSDK_INFO_MOVIE_AUDIO_CODEC,
    /// current subtitle type; Param EN_MMSDK_SUBTITLE_TYPE
    E_MMSDK_INFO_MOVIE_SUBTITLE_TYPE,
    /// TODO: video codec with track id ; Param EN_MMSDK_VIDEO_CODEC
    E_MMSDK_INFO_MOVIE_VIDEO_CODEC_TRACK_ID,
    /// audio codec with track id ; Param EN_MMSDK_AUDIO_CODEC
    E_MMSDK_INFO_MOVIE_AUDIO_CODEC_TRACK_ID,
    /// TODO: subtitle type with track id ; Param EN_MMSDK_SUBTITLE_TYPE
    E_MMSDK_INFO_MOVIE_SUBTITLE_TYPE_TRACK_ID,
    /// number of program(or video track); Param: MMSDK_U32
    E_MMSDK_INFO_MOVIE_NB_PROGRAM,
    /// number of audio track; Param: MMSDK_U32
    E_MMSDK_INFO_MOVIE_NB_AUDIO_TRACK,
    /// number of subtitle track; Param: MMSDK_U32
    E_MMSDK_INFO_MOVIE_NB_SUBTITLE_TRACK,
    /// current track id of program(or video track); Param: MMSDK_U8
    E_MMSDK_INFO_MOVIE_PROGRAM_ID,
    /// current audio track id; Param: MMSDK_U8
    E_MMSDK_INFO_MOVIE_AUDIO_TRACK_ID,
    /// current subtitle track id; Param: MMSDK_U32
    E_MMSDK_INFO_MOVIE_SUBTITLE_TRACK_ID,
    /// get audio track language code by index; Param: MMSDK_U32 (input: track Index, return track language code)
    /// exp: MMSDK_U8 iso639lang[4]; for(int i=0; i<4; i++){ iso639lang[i] = (u32LanguageCode >>(i*8))&0xFF};
    E_MMSDK_INFO_MOVIE_AUDIO_TRACK_LANGUAGE,
    /// get subtitle track language code by index; Param: MMSDK_U32 (input: track Index, return track language code)
    /// exp: MMSDK_U8 iso639Lang[4]; for(int i=0; i<4; i++){ iso639Lang[i] = (u32LanguageCode >>(i*8))&0xFF};
    E_MMSDK_INFO_MOVIE_SUBTITLE_TRACK_LANGUAGE,
    /// get the Presentation Time Stamp of Video stream; Param: MMSDK_U32
    E_MMSDK_INFO_MOVIE_PRESENTATION_TIME_STAMP,
    /// send Extra cmd to vdplayer. Application use ONLY
    E_MMSDK_INFO_MOVIE_SEND_EXTRA_COMMAND,
    /// get current audio channel configuration (number of channel of current audio track); Param: MMSDK_U32
    E_MMSDK_INFO_MOVIE_AUDIO_CHANNEL_CONFIGURATION,
    /// number of total teletext track; Param: MMSDK_U8
    E_MMSDK_INFO_MOVIE_NB_TTX_TRACK,
    /// get audio channel configuration with track id(number of channel of current audio track); Param: MMSDK_U32
    E_MMSDK_INFO_MOVIE_AUDIO_CHANNEL_CONFIGURATION_TRACK_ID,
    /// get video track id by index; Param: MMSDK_U32 (input: track Index, return track id)
    E_MMSDK_INFO_MOVIE_VIDEO_TRACKS_ID,
    /// get audio track id by index; Param: MMSDK_U32 (input: track Index, return track id)
    E_MMSDK_INFO_MOVIE_AUDIO_TRACKS_ID,
    /// get ES audio buffer active size; Param: MMSDK_U32
    E_MMSDK_INFO_MOVIE_AUDIO_BUFFER_ACTIVE,
    /// get ES video buffer active size; Param: MMSDK_U32
    E_MMSDK_INFO_MOVIE_VIDEO_BUFFER_ACTIVE,
    /// get video frame count; Param: MMSDK_U32
    E_MMSDK_INFO_MOVIE_FRAME_COUNT,
    /// get video error frame count; Param: MMSDK_U32
    E_MMSDK_INFO_MOVIE_ERROR_FRAME_COUNT,
    /// get video skip frame count; Param: MMSDK_U32
    E_MMSDK_INFO_MOVIE_SKIP_FRAME_COUNT,
    /// get video drop frame count; Param: MMSDK_U32
    E_MMSDK_INFO_MOVIE_DROP_FRAME_COUNT,
    /// Check current created media item are running with multi decode player or not ; Param: MMSDK_BOOL
    E_MMSDK_INFO_MOVIE_IS_CURRENT_MULTI_DECODE,
    /// get the 64 bit Presentation Time Stamp of Video stream; Param: MMSDK_U64
    E_MMSDK_INFO_MOVIE_PRESENTATION_TIME_STAMP64,
    /// get the system time clock; Param: MMSDK_U64
    E_MMSDK_INFO_MOVIE_SYSTEM_TIME_CLOCK64,
    /// get the Presentation Time Stamp of Audio stream; Param: MMSDK_U64
    E_MMSDK_INFO_MOVIE_AUDIO_PRESENTATION_TIME_STAMP,
    /// get the audio component id for ts file(with track id), track id for other container; input: track id; Param: MMSDK_U32
    E_MMSDK_INFO_MOVIE_AUDIO_COMPONENT_ID,
    /// get the video component id for ts file(with track id), track id for other container; input: track id; Param: MMSDK_U32
    E_MMSDK_INFO_MOVIE_VIDEO_COMPONENT_ID,
    /// Media item metadata is video codec id in detail. AP can classfy it by itself.; Param EN_MMSDK_VIDEO_CODEC
    E_MMSDK_INFO_MOVIE_VIDEO_CODEC_ID_IN_DETAIL,
    /// Media item metadata is audio codec id in detail. AP can classfy it by itself.; Param EN_MMSDK_AUDIO_CODEC
    E_MMSDK_INFO_MOVIE_AUDIO_CODEC_ID_IN_DETAIL,
    /// Media file format.; Param EN_MMSDK_FILE_FORMAT
    E_MMSDK_INFO_MOVIE_VIDEO_FILE_FORMAT,

    /*----------divx----------------*/
    /// divx: is Trick Play Track exist (for smooth play in trick mode).
    E_MMSDK_INFO_MOVIE_DIVX_TPT_MODE_EXIST = 0x1100,
    /// divx: number of title; Param: MMSDK_U32
    E_MMSDK_INFO_MOVIE_DIVX_NB_TITLE,
    /// divx: number of edition; Param: MMSDK_U32
    E_MMSDK_INFO_MOVIE_DIVX_NB_EDITION,
    /// divx: number of chapter; Param: MMSDK_U32
    E_MMSDK_INFO_MOVIE_DIVX_NB_CHAPTER,
    /// divx: number of auto-chapter; Param: MMSDK_U32
    E_MMSDK_INFO_MOVIE_DIVX_NB_AUTOCHAPTER,
    /*----------divx end------------*/
    /*==================== Movie End ====================*/

    /*==================== Music ====================*/
    ///Meida item metadata is music title; Param: EN_MMSDK_STRING_FORMAT
    E_MMSDK_INFO_MUSIC_TITLE = 0x2000,
    ///Media item title string format. Return EN_MMSDK_STRING_FORMAT
    E_MMSDK_INFO_MUSIC_TITLE_FORMAT,
    ///Meida item metadata is music album; Param: EN_MMSDK_STRING_FORMAT
    E_MMSDK_INFO_MUSIC_ALBUM,
    ///Media item album string format. Return EN_MMSDK_STRING_FORMAT
    E_MMSDK_INFO_MUSIC_ALBUM_FORMAT,
    ///Meida item metadata is music artist; Param: EN_MMSDK_STRING_FORMAT
    E_MMSDK_INFO_MUSIC_ARTIST,
    ///Media item artist string format. Return EN_MMSDK_STRING_FORMAT
    E_MMSDK_INFO_MUSIC_ARTIST_FORMAT,
    ///Meida item metadata is music year; Param: EN_MMSDK_STRING_FORMAT
    E_MMSDK_INFO_MUSIC_YEAR,
    /// Media item year string format. Return EN_MMSDK_STRING_FORMAT
    E_MMSDK_INFO_MUSIC_YEAR_FORMAT,
    ///Meida item metadata is music genre; Param: EN_MMSDK_STRING_FORMAT
    E_MMSDK_INFO_MUSIC_GENRE,
    ///Media item genre string format. Return EN_MMSDK_STRING_FORMAT
    E_MMSDK_INFO_MUSIC_GENRE_FORMAT,
    /// Has picture; Param: MMSDK_BOOL
    E_MMSDK_INFO_MUSIC_HAS_PICTURE,
    /// picture offset; Param: MMSDK_U32
    E_MMSDK_INFO_MUSIC_PICTURE_OFFSET,
    /// picture size; Param: MMSDK_U32
    E_MMSDK_INFO_MUSIC_PICTURE_SIZE,
    /// Is VBR (TRUE: VBR, FALSE: CBR); Param: MMSDK_BOOL
    E_MMSDK_INFO_MUSIC_VBR,
    ///music sampling rate; Param: MMSDK_U32
    E_MMSDK_INFO_MUSIC_SAMPLE_FREQ,
    ///music bit rate; Param: MMSDK_U32
    E_MMSDK_INFO_MUSIC_BITRATE,
    /// music codec; Param: EN_MMSDK_AUDIO_CODEC
    E_MMSDK_INFO_MUSIC_AUDIO_CODEC,
    /// Meida item metadata is whether support FF/FB or not; Param: MMSDK_BOOL
    E_MMSDK_INFO_MUSIC_SUPPORT_FFFB,
    ///Media item metadata is music comment; Param: EN_MMSDK_STRING_FORMAT
    E_MMSDK_INFO_MUSIC_COMMENT,
    ///Media item comment string format, Return EN_MMSDK_STRING_FORMAT
    E_MMSDK_INFO_MUSIC_COMMENT_FORMAT,
    /*==================== Music End ====================*/


    /*==================== Photo ====================*/
    /// photo error code; Param: MMSDK_U32
    E_MMSDK_INFO_PHOTO_ERROR_CODE = 0x3000,
    /// photo type; Param: EN_MMSDK_PHOTO_TYPE
    E_MMSDK_INFO_PHOTO_TYPE,
    /// Get photo decodec progress. 0~100; Param: MMSDK_U32
    E_MMSDK_INFO_PHOTO_DECODE_PROGRESS,
    /// has thumbnail; Param: MMSDK_BOOL
    E_MMSDK_INFO_PHOTO_HAS_THUMBNAIL,
    /// Is 3D Photo format; Param: MMSDK_BOOL
    E_MMSDK_INFO_PHOTO_IS_3D,
    /// has date time
    E_MMSDK_INFO_PHOTO_HAS_DATE,
    /// date year; Param: MMSDK_U32
    E_MMSDK_INFO_PHOTO_DATE_YEAR,
    /// date month; Param: MMSDK_U32
    E_MMSDK_INFO_PHOTO_DATE_MONTH,
    /// date day; Param: MMSDK_U32
    E_MMSDK_INFO_PHOTO_DATE_DAY,
    /// date hour; Param: MMSDK_U32
    E_MMSDK_INFO_PHOTO_DATE_HOUR,
    /// date minute; Param: MMSDK_U32
    E_MMSDK_INFO_PHOTO_DATE_MINUTE,
    /// date second; Param: MMSDK_U32
    E_MMSDK_INFO_PHOTO_DATE_SECOND,
    /// photo decoded width; DDI need; Param: MMSDK_U32
    E_MMSDK_INFO_PHOTO_OUTPUT_WIDTH,
    /// photo decoded height; DDI need; Param: MMSDK_U32
    E_MMSDK_INFO_PHOTO_OUTPUT_HEIGHT,
    /// photo decoded pitch (bytes per line); DDI need; Param: MMSDK_U32
    E_MMSDK_INFO_PHOTO_OUTPUT_PITCH,
    /// photo decoded color format; DDI need; Param: EN_MMSDK_COLOR_FORMAT
    E_MMSDK_INFO_PHOTO_OUTPUT_COLOR_FORMAT,
    /// photo decode max resolution; Param:  ST_MMSDK_DECODE_MAX_RESOLUTION
    E_MMSDK_INFO_PHOTO_DECODE_MAX_RESOLUTION,
    /// photo exif info; Param:  ST_STILLIMAGE_JPEG_EXIF
    E_MMSDK_INFO_PHOTO_JPEG_EXIF,
    /*==================== Photo End ====================*/
}EN_MMSDK_INFO;

///Define extension part of media info for create media item
typedef enum
{
    /// PCM Info music; Param:ST_MMSDK_PCM_INFO
    E_MMSDK_MEDIAINFO_EXT_MUSIC_PCM,
    /// Probe info of TS (This setting will help to decrease init time); Param: ST_MMSDK_TS_INFO
    E_MMSDK_MEDIAINFO_EXT_MOVIE_PROBE_INFO_TS,
    /// ES Player video info; Param:ST_MMSDK_ES_VIDEO_INFO
    E_MMSDK_MEDIAINFO_EXT_MOVIE_ESPLAYER_VIDEOINFO,
    /// ES Player audio info; Param:ST_MMSDK_ES_AUDIO_INFO
    E_MMSDK_MEDIAINFO_EXT_MOVIE_ESPLAYER_AUDIOINFO,
    /// for resume play; Param: ST_MMSDK_RESUME_INFO
    E_MMSDK_MEDIAINFO_EXT_MOVIE_SET_RESUMEPLAY,
    /// Movie item format type, for TS to speed up init time and DSS; Param: MMSDK_U32
    E_MMSDK_MEDIAINFO_EXT_MOVIE_FORMAT_TYPE,
    /// for set wm drm key; Param: ST_MMSDK_SET_WMDRM_INFO
    E_MMSDK_MEDIAINFO_EXT_MOVIE_SET_WMDRM_KEY,
    /// Set string app type; Param: U8 array[MMSDK_AP_TYPE_MAX_LEN]
    E_MMSDK_MEDIAINFO_EXT_AP_TYPE_STRING,
    /// Set string url; Param: pstrUrl, u32Paramsize = sizeof(pstrUrl)
    E_MMSDK_MEDIAINFO_EXT_URL_STRING,
    /// Set stream start time; Param: MMSDK_U32
    E_MMSDK_MEDIAINFO_EXT_STREAM_START_TIME,
    /// Set stream total time; Param: MMSDK_U32
    E_MMSDK_MEDIAINFO_EXT_STREAM_TOTAL_TIME,
    /// Set the display output path is XC or DIP; Param: U8 array[MMSDK_DISP_OUTPUT_PATH_MAX_LEN]
    /// | name  | display path | window index         | comment
    /// | xc    | xc           | x (without appoint)  | reserved
    /// | xc_01 | xc           | main window          | reserved
    /// | xc_02 | xc           | sub window           | reserved
    /// | dip   | dip          | unused               | valid
    E_MMSDK_MEDIAINFO_EXT_DISPLAY_OUTPUT_PATH,
    /// Set the n-decode capability of max resolution; Param: ST_MMSDK_CODEC_CAP_INFO
    E_MMSDK_MEDIAINFO_EXT_MULTI_DECODE_CAPABILITY,
    /// Set the dolby vision information in ES mode; Param: ST_MMSDK_DOLBY_VISION_INFO
    E_MMSDK_MEDIAINFO_EXT_SET_DOLBY_VISION_INFO,
    /// Set the HDR10 configure in ES mode;Param: ST_MMSDK_HDR10_CONFIG
    E_MMSDK_MEDIAINFO_EXT_SET_HDR10_CONFIG,
}EN_MMSDK_MEDIAINFO_EXT;

/// Define Option type
typedef enum
{
    // movie info: Sub-Option:EN_MMSDK_INFO
    E_MMSDK_OPTION_MOVIE_INFO,
    // movie metadata length info: Sub-Option:EN_MMSDK_INFO
    E_MMSDK_OPTION_MOVIE_INFO_LENGTH,
    // music info: Sub-Option:EN_MMSDK_INFO
    E_MMSDK_OPTION_MUSIC_INFO,
    // music metadata length info: Sub-Option:EN_MMSDK_INFO
    E_MMSDK_OPTION_MUSIC_INFO_LENGTH,
    // photo info: Sub-Option:EN_MMSDK_INFO
    E_MMSDK_OPTION_PHOTO_INFO,
    // photo metadata length info: Sub-Option:EN_MMSDK_INFO
    E_MMSDK_OPTION_PHOTO_INFO_LENGTH,
}EN_MMSDK_OPTION_TYPE;

/// Define PCM Info
typedef struct{
    ///Is PCM
    MMSDK_BOOL bIsPCM;
    ///PCM Channels
    MMSDK_U16 u16Channels;
    ///PCM SamplesPerSec
    MMSDK_U32 u32SamplesPerSec;
    ///PCM AvgBytesPerSec
    MMSDK_U32 u32AvgBytesPerSec;
    ///PCM BlockSize
    MMSDK_U16 u16BlockSize;
    ///PCM BitsPerSample
    MMSDK_U16 u16BitsPerSample;
} ST_MMSDK_PCM_INFO;


/// Define Callback Message Type Parameter structure: IO (read/seek/tell)
typedef struct
{
    /// Media Item
    MEDIAITEM Item;
    /// File identification information.
    void* pFileHandle;
    /// Buffer start for file read event.
    MMSDK_U8* pu8FileBuff;
    /// Read size for file read event.
    MMSDK_U32 u32FileRequestSize;
    /// File position for file seek event.
    MMSDK_U64 u64FileSeekPos;
    /// Blocking read: need to feed data as request size, except file end.
    /// Non blocking read: just feed data as you have, and feedback read size in ack info.
    MMSDK_BOOL bBlockingRead;
    /// Time seek flag
    MMSDK_BOOL bIsTimeSeekOn;
}ST_MMSDK_CALLBACK_IO_INFO;

/// Define Callback Message Type Parameter structure: IO ack info
typedef struct
{
    /// File read size.
    MMSDK_U32 u32ReadSize;
    /// Current file position.
    MMSDK_U64 u64FilePosition;
    /// File was read to end or not.
    MMSDK_BOOL bIsFileEnd;
}ST_MMSDK_CALLBACK_IO_ACK;


/// Define Callback Message Type Parameter structure: Notify
typedef struct
{
    /// Media item.
    MEDIAITEM Item;
    /// u32 return
    MMSDK_U32 u32Ret;
    /// bool return
    MMSDK_BOOL bRet;
}ST_MMSDK_CALLBACK_NOTIFY_INFO;

/// Define Callback Message Type Parameter structure: push subtitle info
typedef struct
{
    /// Media item.
    MEDIAITEM Item;
    /// Subtitle type
    EN_MMSDK_SUBTITLE_TYPE enType;
    /// Subtitle data buffer address
    MMSDK_U32 u32QueueStart;
    /// Subtitle data buffer size
    MMSDK_U32 u32QueueSize;
    /// PST start
    MMSDK_U32 u32PTSStart;
    /// PTS end
    MMSDK_U32 u32PTSEnd;
}ST_MMSDK_CALLBACK_SUBTITILE_INFO;

/// Define Callback Message Type Parameter structure: for IPC client to do mpool mapping
typedef struct
{
    /// Item: reserved for void*, not used now.
    MEDIAITEM Item;
    /// u32MapSize: MIU select for mpool mapping/unmapping
    MMSDK_U32 u32MiuNo;
    /// u32MapSize: address for mpool mapping/unmapping
    MMSDK_U32 u32PhyAddr;
    /// u32MapSize: size for mpool mapping/unmapping
    MMSDK_U32 u32MapSize;
    /// u32Reserved: reserved
    MMSDK_U32 u32Reserved;
    /// bIsMapping TRUE for Mapping, FALSE for UnMapping
    MMSDK_BOOL bIsMapping;
    /// bReserved1: reserved for aligned.
    MMSDK_BOOL bReserved1;
    /// bReserved2: reserved for aligned.
    MMSDK_BOOL bReserved2;
    /// bReserved3: reserved for aligned.
    MMSDK_BOOL bReserved3;
}ST_MMSDK_CALLBACK_MAP_INFO;

/// Define Callback Message Type
typedef enum
{
    /// Param: ST_MMSDK_CALLBACK_IO_INFO
    E_MMSDK_CALLBACK_FILE_READ = 0,
    /// Param: ST_MMSDK_CALLBACK_IO_INFO
    E_MMSDK_CALLBACK_FILE_SEEK = 1,
    /// Param: ST_MMSDK_CALLBACK_IO_INFO
    E_MMSDK_CALLBACK_FILE_TELL = 2,
    /// Param: ST_MMSDK_CALLBACK_NOTIFY_INFO
    E_MMSDK_CALLBACK_INIT_OK = 3,
    /// Param: ST_MMSDK_CALLBACK_NOTIFY_INFO
    E_MMSDK_CALLBACK_INIT_FAIL = 4,
    /// Param: ST_MMSDK_CALLBACK_NOTIFY_INFO
    E_MMSDK_CALLBACK_INVALID_FILE = 5,
    /// Param: ST_MMSDK_CALLBACK_NOTIFY_INFO
    E_MMSDK_CALLBACK_THUMBNAIL_DONE = 6,
    /// Param: ST_MMSDK_CALLBACK_NOTIFY_INFO
    E_MMSDK_CALLBACK_THUMBNAIL_ERROR = 7,
    /// Param: ST_MMSDK_CALLBACK_NOTIFY_INFO
    E_MMSDK_CALLBACK_DECODE_ERROR = 8,
    /// Param: ST_MMSDK_CALLBACK_NOTIFY_INFO
    E_MMSDK_CALLBACK_DECODE_DONE = 9,
    /// Param: ST_MMSDK_CALLBACK_NOTIFY_INFO
    E_MMSDK_CALLBACK_DECODE_FIRST_FRAME_DONE = 10,
    /// Param: ST_MMSDK_CALLBACK_NOTIFY_INFO
    E_MMSDK_CALLBACK_DECODE_BACKWARD_TO_FILE_START = 11,
    /// Param: ST_MMSDK_CALLBACK_SUBTITILE_INFO
    E_MMSDK_CALLBACK_INTERNAL_SUBTITLE_TYPE_TEXT = 12,
    /// Param: ST_MMSDK_CALLBACK_NOTIFY_INFO
    E_MMSDK_CALLBACK_PROGRAM_CHANGED = 13,
    /// Param: ST_MMSDK_CALLBACK_NOTIFY_INFO
    E_MMSDK_CALLBACK_AUDIO_TRACK_CHANGED = 14,
    /// Param: ST_MMSDK_CALLBACK_NOTIFY_INFO
    E_MMSDK_CALLBACK_DEMUX_HEADER_OK = 15,
    /// Param: ST_MMSDK_CALLBACK_NOTIFY_INFO
    E_MMSDK_CALLBACK_DEMUX_HEADER_FAIL = 16,
    /// Param: ST_MMSDK_CALLBACK_NOTIFY_INFO
    E_MMSDK_CALLBACK_AUDIO_UNSUPPORTED = 17,
    /// Param: ST_MMSDK_CALLBACK_NOTIFY_INFO
    E_MMSDK_CALLBACK_NOT_SUPPORT = 18,
    /// Param: ST_MMSDK_CALLBACK_NOTIFY_INFO
    E_MMSDK_CALLBACK_PLAYING_OK = 19,
    /// Param: ST_MMSDK_CALLBACK_NOTIFY_INFO
    E_MMSDK_CALLBACK_AUDIO_ONLY = 20,
    /// Param: ST_MMSDK_CALLBACK_NOTIFY_INFO
    E_MMSDK_CALLBACK_SETPLAYPOSITION_ACK = 21,
    /// Param: ST_MMSDK_CALLBACK_NOTIFY_INFO
    E_MMSDK_CALLBACK_SETTRICKMODE_ACK = 22,
    /// Param: ST_MMSDK_CALLBACK_NOTIFY_INFO
    E_MMSDK_CALLBACK_GS_DISPLAY_RDY = 23,
    /// Param: ST_MMSDK_CALLBACK_NOTIFY_INFO
    E_MMSDK_CALLBACK_ES_FLUSH_DATA_RDY = 24,
    /// Param: ST_MMSDK_CALLBACK_NOTIFY_INFO(TSB)
    E_MMSDK_CALLBACK_SETTITLEID_ACK = 25,
    /// Param: ST_MMSDK_CALLBACK_NOTIFY_INFO(TSB)
    E_MMSDK_CALLBACK_SETEDITIONID_ACK = 26,
    /// Param: ST_MMSDK_CALLBACK_NOTIFY_INFO(TSB)
    E_MMSDK_CALLBACK_SETCHAPTERID_ACK = 27,
    /// Param: ST_MMSDK_CALLBACK_NOTIFY_INFO(TSB)
    E_MMSDK_CALLBACK_SETAUTOCHAPTERID_ACK = 28,
    /// Param: ST_MMSDK_CALLBACK_NOTIFY_INFO
    E_MMSDK_CALLBACK_GS_DTCPINFO_FOUND = 29,
    /// Param: ST_MMSDK_CALLBACK_NOTIFY_INFO
    E_MMSDK_CALLBACK_DEMUXRESETADVANCE_ACK = 30,
    /// for IPC client to do mpool mapping (important for CMA), Param: ST_MMSDK_CALLBACK_MAP_INFO
    E_MMSDK_CALLBACK_MAP_MEMORY = 31,
    /// Param: ST_MMSDK_CALLBACK_NOTIFY_INFO(for STB MI render gif photo by upper layer)
    E_MMSDK_CALLBACK_DECODE_ONE_FRAME_DONE = 32,
    /// for download module info buffering, hint app low water level
    /// Param: ST_MMSDK_CALLBACK_NOTIFY_INFO
    E_MMSDK_CALLBACK_DATA_BUFFERING_START = 33,
    /// for download module info buffering, hint app high water level
    /// Param: ST_MMSDK_CALLBACK_NOTIFY_INFO
    E_MMSDK_CALLBACK_DATA_BUFFERING_STOP = 34,
}EN_MMSDK_CALLBACK_MSG;

/// Define TS info structure
typedef struct
{
    /// Video Codec ID
    EN_MMSDK_VIDEO_CODEC enVideoCodecType;
    /// Audio Codec ID
    EN_MMSDK_AUDIO_CODEC enAudioCodecType;
    /// Video PID
    MMSDK_U16 u16VideoPID;
    /// Audio PID
    MMSDK_U16 u16AudioPID;
    /// Transport packet length
    MMSDK_U8 u8TransportPacketLen;     // TS 188, 192, 204 bytes or DS length.
}ST_MMSDK_TS_INFO;

/// Define Callback Message Type Parameter structure: dtcp information
typedef struct
{
    /// Media item.
    MEDIAITEM Item;
    /// DTCP info: ((u16VideoDtcp & 0xFF) << 24) | (((u16VideoDtcp >> 8) & 0xFF) << 16) |
    ///            ((u16AudioDtcp & 0xFF) << 8) | ((u16AudioDtcp >> 8) & 0xFF)
    MMSDK_U32 u32Info;
} ST_MMSDK_DTCP_INFO;

/// Define Callback Message Type Parameter structure: Command
typedef struct
{
    /// Media item.
    MEDIAITEM Item;
    /// Command return info
    MMSDK_U32 u32Info;
} ST_MMSDK_CALLBACK_CMD_INFO;

/// define callback function
//-------------------------------------------------------------------------------------------------
/// Register Callback function
/// @param pClass          \b IN: Up-layer application class point.(for application to distinguish)
/// @param u32Msg         \b IN: Callback message type (EN_MMSDK_CALLBACK_MSG)
/// @param pMsgInfo       \b IN: Callback message info
/// @param pAckInfo       \b IN: Callback Ack info
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
typedef MMSDK_BOOL(*pfnMMSDKCallBack)(void* pClass, const MMSDK_U32 u32Msg, void* pMsgInfo, void* pAckInfo);


/// Define the information for create media item.
typedef struct
{
    /// Version of MMSDK. assigned this MMSDK_VERSION value
    MMSDK_U32 u32Version;
    /// Up-layer application class point of call back function.(for application to distinguish)
    void* pClass;
    /// Register call back function
    pfnMMSDKCallBack pCallback;
    /// file handle for IO callback, this argument must cannot be 0.
    void* pFileHandle1;
    /// The second file handle for IO callback. Use on video media subtitle data file read process. If the application can't support the seconad file index, please set this argument with 0.
    void* pFileHandle2;
    /// The third file handle for IO callback. Use on video media audio data file read process. If the application can't suppor the third file index, please set thie argument with 0.
    void* pFileHandle3;
    /// The fourth file handle for IO callback. Use on video media DIVX attachment file read process. If the application can't suppor the fourth file index, please set thie argument with 0.
    void* pFileHandle4;
    /// Media item file length. If the file size was unknow, please set with -1 (0xFFFFFFFFFFFFFFFF).
    MMSDK_U64 u64FileLength;
    /// Media item type.
    EN_MMSDK_MEDIA_TYPE enMediaType;
    /// Media item file type. (storage/ stream stream/ without seek)
    EN_MMSDK_FILE_TYPE enMediaFileType;
    /// Media item creating type. (normal play/ preview/ thumbnail)
    EN_MMSDK_CREATE_MODE enMediaCreateMode;
    /// app type for adapt customerization setting. (read ini setting)
    EN_MMSDK_AP_TYPE eAppType;
    /// Media item create flag, set with EN_MMSDK_CTL_FLAG definition
    MMSDK_U32 u32CtlFlag;
    /// Control flag for TEE encryption type, set with E_MMSDK_TEE_ENCRYPT_CTL_FLAG definition
    MMSDK_U32 u32TeeEncryptCtlFlag;
} ST_MMSDK_CREATE_MEDIA_INFO;


/// Define the info for general parameter setting
typedef struct
{
    /// Option
    MMSDK_U32 u32Option;
    /// Sub Option
    MMSDK_U32 u32SubOption;
    /// Parameter size
    MMSDK_U32 u32Paramsize;
    /// Reserved
    MMSDK_U32 u32Reserved;
    /// Parameter address
    void * pParam;
    /// Reserved
    void * pReserved;
}ST_MMSDK_PARAM;

/// Define the information for photo decode max resolution
typedef struct
{
    /// Photo codec type; EN_MMSDK_PHOTO_TYPE
    MMSDK_U32 u32CodecType;
    /// Max width of resolution
    MMSDK_U32 u32MaxWidth;
    /// Max height of resolution
    MMSDK_U32 u32MaxHeight;
}ST_MMSDK_DECODE_MAX_RESOLUTION;

/// Define the information for subtitle track
typedef struct
{
    /// Subtitle track index.
    MMSDK_U8 u8TrackID;
    /// Subtitle type.
    EN_MMSDK_SUBTITLE_TYPE enType;
}ST_MMSDK_SUBTITLE_TRACK_INFO;

/// Define the information for audio track.
typedef struct
{
    /// Audio track index.
    MMSDK_U8 u8TrackID;
    /// Audio codec type.
    EN_MMSDK_AUDIO_CODEC enType;
}ST_MMSDK_AUDIO_TRACK_INFO;


/// Define video buffer information
typedef enum
{
    /// Video data buffer data size which is demuxed and waiting for display.
    E_MMSDK_VIDEO_BUFFER_LEVEL,
    /// Video average bit-rate (for RM and FLV only).
    E_MMSDK_VIDEO_BUFFER_BITRATE,
    /// Video total time (ms unit).
    E_MMSDK_VIDEO_BUFFER_DURATION,
    /// @deprecated Video buffer control flag.
    E_MMSDK_VIDEO_BUFFER_FLAG,
    /// Video data buffer the residual play time of demuxed data(ms).
    E_MMSDK_VIDEO_BUFFER_LEVEL_TIME = 3,
    /// Video data size for TS file last PTS searching.
    E_MMSDK_VIDEO_BUFFER_LAST_PTS_LEN,
    /// RM Video index information offset (byte unit).
    E_MMSDK_VIDEO_BUFFER_INDEX_OFFSET,
    /// Video data bufer date fill percetage.
    E_MMSDK_VIDEO_BUFFER_PERCENT,
    ////Decoded frame count by decoder
    E_MMSDK_VIDEO_BUFFER_FRAME_COUNT,
} EN_MMSDK_VIDEO_BUFFER_INFO;

/// Define movie seamless playback mode
typedef enum
{
    /// No seamless mode
    E_MMSDK_SEAMLESS_NONE,
    /// Seamless FreeZ
    E_MMSDK_SEAMLESS_FREEZ,
    /// Seamless Smoth. only for mp4 and flv container
    E_MMSDK_SEAMLESS_SMOTH,
} EN_MMSDK_SEAMLESS_MODE;


/// Define the decoder time out control (unit: sec). Use for decoder error concealment check.
typedef struct
{
    /// File IO control time out value. 0 is for use decoder default value.
    MMSDK_U32 u32FileIOTimeOut;
    /// File read time out value. 0 is for use decoder default value.
    MMSDK_U32 u32FileReadTimeOut;
    /// File seek time out valuse. 0 is for use decoder default value.
    MMSDK_U32 u32FileSeekTimeOut;
    /// Decode time out value. 0 is for use decoder default value.
    MMSDK_U32 u32DecodeCtrlTimeOut;
    /// ES buffer control time out value. 0 is for use decoder default value.
    MMSDK_U32 u32ESBuffCtrlTimeOut;
} ST_MMSDK_TIMEOUT_VALUE;

/// DivX DRM Registration code length
#define MMSDK_DIVX_REG_CODE_LEN                   10
/// DivX DRM Deactivation code length
#define MMSDK_DIVX_DEACT_CODE_LEN                 8
/// DivX DRM Key length
#define MMSDK_DIVX_KEY_LEN                        48
/// DivX Plus metadata LawRating string size (divx plus 1.1.1 requirement)
#define MMSDK_DIVX_LAWRATING_SIZE        10
/// DivX Plus metadata name string size (divx plus 1.1.1 requirement)
#define MMSDK_DIVX_METADATA_NAME_SIZE             150

/// Define divx drm playback status
typedef struct
{
    /// Used limit
    MMSDK_U32 u32RentalLimit;
    /// Used count (remaining count= limit - used count)
    MMSDK_U32 u32RentalUsedCount;
    /// reserved 1
    MMSDK_U32 u32Reserved;
    /// reserved 2
    MMSDK_U32 u32Reserved2;
    /// TRUE: expired
    MMSDK_BOOL bIsRentalExpire;
    /// TRUE: drm rental file
    MMSDK_BOOL bIsRentalFile;
    /// TRUE: divx drm file
    MMSDK_BOOL bIsDrmFile;
    /// TRUE: Authorized
    MMSDK_BOOL bIsAuthorized;
} ST_MMSDK_DIVXDRM_STATUS;

/// Define drm registration info which should be saved
typedef struct
{
    /// DIVX DRM registration code
    MMSDK_U8 u8RegistrationCode[MMSDK_DIVX_REG_CODE_LEN+1];
    /// DIVX DRM deactivated code
    MMSDK_U8 u8DeActivationCode[MMSDK_DIVX_DEACT_CODE_LEN+1];
    /// DIVX DRM KEY
    MMSDK_U8 u8DRM_Data[MMSDK_DIVX_KEY_LEN];
    /// is registration code generated
    MMSDK_U8 bIsKeyGenerated;
    /// is set activated.
    MMSDK_U8 bIsActivated;
    /// is set deactivated.
    MMSDK_U8 bIsDeactivated;
} ST_MMSDK_DIVXDRM_USER_INFO;


/// DivX Operate and Get info (divx plus 1.1.1)
typedef enum
{
    /// none
    E_MMSDK_DIVX_NONE = 0,

    //Get info
    //MKV feature
    /// Title info with auto generate title name.
    /// if ((ST_MMSDK_DIVX_INFO.u32Reserved & BIT0) == BIT0) means has ST_MMSDK_DIVX_INFO.strName info
    /// else auto generate name info
    E_MMSDK_DIVX_GET_TITLE_INFO = 1,
    /// Edition info with auto generate edition name.
    /// if ((ST_MMSDK_DIVX_INFO.u32Reserved & BIT0) == BIT0) means has ST_MMSDK_DIVX_INFO.strName info
    /// else auto generate name info
    E_MMSDK_DIVX_GET_EDITION_INFO = 2,
    /// chapter info with auto generate chapter name.
    /// if ((ST_MMSDK_DIVX_INFO.u32Reserved & BIT0) == BIT0) means has ST_MMSDK_DIVX_INFO.strName info
    /// else auto generate name info
    E_MMSDK_DIVX_GET_CHAPTER_INFO= 3,
    /// auto-chapter info with auto generate auto-chapter name.
    /// if ((ST_MMSDK_DIVX_INFO.u32Reserved & BIT0) == BIT0) means has ST_MMSDK_DIVX_INFO.strName info
    /// else auto generate name info
    E_MMSDK_DIVX_GET_AUTOCHAPTER_INFO =4,

    /// get title info without auto generate title name.
    /// if ((ST_MMSDK_DIVX_INFO.u32Reserved & BIT0) == BIT0) means has ST_MMSDK_DIVX_INFO.strName info
    E_MMSDK_DIVX_GET_TITLE_INFO_NOAUTOGEN = 5,
    /// get edition info without auto generate editon name.
    /// if ((ST_MMSDK_DIVX_INFO.u32Reserved & BIT0) == BIT0) means has ST_MMSDK_DIVX_INFO.strName info
    E_MMSDK_DIVX_GET_EDITION_INFO_NOAUTOGEN = 6,
    /// get chapter info without auto generate chapter name.
    /// if ((ST_MMSDK_DIVX_INFO.u32Reserved & BIT0) == BIT0) means has ST_MMSDK_DIVX_INFO.strName info
    E_MMSDK_DIVX_GET_CHAPTER_INFO_NOAUTOGEN= 7,
    /// get auto-chapter info  without auto generate name.
    /// if ((ST_MMSDK_DIVX_INFO.u32Reserved & BIT0) == BIT0) means has ST_MMSDK_DIVX_INFO.strName info
    E_MMSDK_DIVX_GET_AUTOCHAPTER_INFO_NOAUTOGEN =8,

    /// get audio info with auto generate audio name.
    /// if ((ST_MMSDK_DIVX_INFO.u32Reserved & BIT0) == BIT0) means has ST_MMSDK_DIVX_INFO.strName info
    /// else auto generate name info
    E_MMSDK_DIVX_GET_AUDIO_INFO= 50,
    /// get audio info without auto generate audio name.
    /// if ((ST_MMSDK_DIVX_INFO.u32Reserved & BIT0) == BIT0) means has ST_MMSDK_DIVX_INFO.strName info
    E_MMSDK_DIVX_GET_AUDIO_INFO_NOAUTOGEN,

    /// get subtitle info with auto generate subtitle name.
    /// if ((ST_MMSDK_DIVX_INFO.u32Reserved & BIT0) == BIT0) means has ST_MMSDK_DIVX_INFO.strName info
    /// else auto generate name info
    E_MMSDK_DIVX_GET_SUBTITLE_INFO=60,
    /// get subtitle info without auto generate subtitle name.
    /// if ((ST_MMSDK_DIVX_INFO.u32Reserved & BIT0) == BIT0) means has ST_MMSDK_DIVX_INFO.strName info
    E_MMSDK_DIVX_GET_SUBTITLE_INFO_NOAUTOGEN,
} EN_MMSDK_DIVX_OPERATE;

/// Defined flag of DRM out project information
typedef enum
{
    /// none
    E_MMSDK_DRM_OP_NONE                   = 0x00000000,
    /// CGMS-A:copy Ffreely
    E_MMSDK_DRM_OP_CGMS_COPY_FREE         = 0x00000001,
    /// CGMS-A:copy no more
    E_MMSDK_DRM_OP_CGMS_COPY_NO_MORE      = 0x00000002,
    /// CGMS-A:copy once
    E_MMSDK_DRM_OP_CGMS_COPY_ONCE         = 0x00000004,
    /// CGMS-A:copy never
    E_MMSDK_DRM_OP_CGMS_COPY_NEVER        = 0x00000008,
    /// ACPTB(Macrovison):automatic gain control
    E_MMSDK_DRM_OP_ACP_AGC                = 0x00000010,
    /// ACPTB(Macrovison):2 line color burst
    E_MMSDK_DRM_OP_ACP_AGC_2LINE_COLOR    = 0x00000020,
    /// ACPTB(Macrovison):4 line color burst
    E_MMSDK_DRM_OP_ACP_AGC_4LINE_COLOR    = 0x00000040,
    /// DigitalProtection
    E_MMSDK_DRM_OP_HDCP                   = 0x00000080,
    /// Ict
    E_MMSDK_DRM_OP_ICT                    = 0x00000100,
    /// unknow
    E_MMSDK_DRM_OP_UNKNOW                 = 0x00000200,
} EN_MMSDK_OUTPUT_PROTECT_FLAG;

/// DivX Language (divx plus 1.1.1)
typedef enum
{
    /// undefine
    E_MMSDK_DIVX_UNDEFINE = 0,
    /// english
    E_MMSDK_DIVX_ENGLISH,
    /// chinese
    E_MMSDK_DIVX_CHINESE,
    /// korea
    E_MMSDK_DIVX_KOREA,
    /// japan
    E_MMSDK_DIVX_JAPAN,
    /// french
    E_MMSDK_DIVX_FRENCH,
    /// german
    E_MMSDK_DIVX_GERMANIC,
    /// spanish
    E_MMSDK_DIVX_SPANISH,
    /// italian
    E_MMSDK_DIVX_ITALIAN,
    /// greek
    E_MMSDK_DIVX_GREEK,
    /// portuguese
    E_MMSDK_DIVX_PORTUGUESE,
    /// russian
    E_MMSDK_DIVX_RUSSIAN,
    /// polish
    E_MMSDK_DIVX_POLISH,
    /// swedish
    E_MMSDK_DIVX_SWEDISH,
    /// norwegian
    E_MMSDK_DIVX_NORWEGIAN,
} EN_MMSDK_DIVX_LANG;

/// DivX DRM Info
typedef struct
{
    /// DivX DRM Renta lCode
    MMSDK_U32 u32DrmRentalCode;
    /// DivX DRM Rental File or not
    MMSDK_U32 u32DrmRentalFile;
    /// DivX DRM number of Rental Limit
    MMSDK_U32 u32DrmRentalLimit;
    /// DivX DRM number of Rental Use Count
    MMSDK_U32 u32DrmRentalUseCount;
    /// DivX DRM output protection.
    EN_MMSDK_OUTPUT_PROTECT_FLAG eOutputProtectFlg;
    /// DivX DRM Check Sum for store in EEPROM
    MMSDK_U16 u16CheckSum;
    /// DivX DRM RentalSatus
    MMSDK_U8 u8DrmRentalStatus;
    /// DivX DRM File Format
    MMSDK_U8 u8DrmFileFormat;
    /// DivX DRM Authrization Satus
    MMSDK_U8 u8DrmAuthorization;
    /// DivX DRM Reg Code
    MMSDK_U8 u8RegistrationCode[MMSDK_DIVX_REG_CODE_LEN+1];
    /// DivX DRM Activation Code
    MMSDK_U8 u8DeActivationCode[MMSDK_DIVX_DEACT_CODE_LEN+1];
    /// DivX DRM Key Data
    MMSDK_U8 u8DRM_Data[MMSDK_DIVX_KEY_LEN];
    /// DivX DRM Key Generated or not
    MMSDK_U8 bIsKeyGenerated;
    /// DivX DRM activated or not
    MMSDK_U8 bIsActivated;
    /// DivX DRM deactivated or not
    MMSDK_U8 bIsDeactivated;
    /// DivX DRM to clear last memory when deactivated.
    MMSDK_U8 bClearLastMemory;
} ST_MMSDK_DIVXDRM_INFO;

/// Divx MKV meta info (divx plus 1.1.1)
typedef struct
{
    /// Operate Type
    EN_MMSDK_DIVX_OPERATE eOperateType;
    /// OSD language
    EN_MMSDK_DIVX_LANG eOSDLanguage;
    /// operate index
    MMSDK_U32 u32Idx;
} ST_MMSDK_DIVX_OPERATE_PARAMETER;

/// Divx MKV meta info (divx plus 1.1.1)
typedef struct
{
    /// law rating
    char strLawRating[MMSDK_DIVX_LAWRATING_SIZE];
    /// name string
    char strName[MMSDK_DIVX_METADATA_NAME_SIZE];
    /// Total num
    MMSDK_U32 u32TotalNum;
    /// Current index
    MMSDK_U32 u32CurrentIdx;
    /// audio channel number
    MMSDK_U32 u32ChannelConfiguration;
    /// Reserved
    MMSDK_U32 u32Reserved;
} ST_MMSDK_DIVX_INFO;

/// Define the information for teletext.
typedef struct
{
    /// Teletext languge.
    MMSDK_U32 u32Language;
    /// Teletext track index.
    MMSDK_U8 u8TrackID;
    /// Teletext track count.
    MMSDK_U8 u8TrackCount;
    /// Teletext type.
    MMSDK_U8 u8Type;
    /// Teletext magazine number.
    MMSDK_U8 u8Magazine;
    /// Teletext page number.
    MMSDK_U8 u8Page;
} ST_MMSDK_TTX_TRACK_INFO;

/// Define media preview mode.
typedef enum
{
    /// Media preview mode is normal. For photo and video media.
    E_MMSDK_PREVIEW_NORMAL_MODE,
    /// Media preview mode is pause mode. For video media only, the video will be paused on the time (u32VideoPauseTime).
    E_MMSDK_PREVIEW_VIDEO_PAUSE_MODE,
    /// Media preview mode is none.
    E_MMSDK_PREVIEW_NUM
} EN_MMSDK_PREVIEW_MODE;

/// Define the information for preview process.
typedef struct
{
    /// Preview mode.
    EN_MMSDK_PREVIEW_MODE eMode;
    /// The time stamp for preview pause time (ms unit). Valid for preview mode is E_CPCODEC_PREVIEW_VIDEO_PAUSE_MODE.
    MMSDK_U32 u32VideoPauseTime;
} ST_MMSDK_PREVIEW_SETTING;


/// Define player state for getOptionByPara with E_MMSDK_METADATA_PLAYER_STATE
typedef enum
{
    /// Player state: default
    E_MMSDK_PLAYER_STATE_NULL               = 0,
    /// Player state: start init media item
    E_MMSDK_PLAYER_STATE_INIT               = 0x0100,
    /// Player state: start init media item done
    E_MMSDK_PLAYER_STATE_INIT_DONE          = 0x0110,
    /// Player state: start play, switch to this state after send play command
    E_MMSDK_PLAYER_STATE_PLAY_START         = 0x0200,
    /// Player state: playing
    E_MMSDK_PLAYER_STATE_PLAYING            = 0x0210,
    /// Player state: start stop procedure
    E_MMSDK_PLAYER_STATE_STOP_START         = 0x0800,
    /// Player state: start stopped
    E_MMSDK_PLAYER_STATE_STOPPED            = 0x0810,
} EN_MMSDK_PLAYER_STATE;

/// Define video closed caption cmd type
typedef enum
{
    /// Video closed caption off
    E_MMSDK_CC_OFF = 0,
    /// Video closed caption on
    E_MMSDK_CC_ON,
    /// Video closed caption off but do not update data base
    E_MMSDK_CC_OFF_DO_NOT_UPDATE_DATABASE
} EN_MMSDK_CC_CMD_TYPE;

/// Define the parameter for set wmdrm key
typedef struct
{
    /// key buffer address.
    MMSDK_U8 * pu8Key;
    /// key buffer size.
    MMSDK_U32 u32Size;
} ST_MMSDK_SET_WMDRM_INFO;

/// Define the control flag for DemuxResetAdvance
typedef enum
{
    /// demux reset none.
    E_MMSDK_DEMUX_RESET_NONE               = 0,
    /// demux reset disable av sync.
    E_MMSDK_DEMUX_RESET_DISABLE_AVSYNC    = 0x00000001,
    /// demux reset only decode I frame.
    E_MMSDK_DEMUX_RESET_DECODE_I           = 0x00000002,
    /// demux reset with audio disable.
    E_MMSDK_DEMUX_RESET_DISABLE_AUDIO      = 0x00000004,
    /// demux reset display one frame.
    E_MMSDK_DEMUX_RESET_DISPLAY_ONE_FRAME  = 0x00000008,
    /// demux reset file size.
    E_MMSDK_DEMUX_RESET_FILE_SIZE          = 0x00000010
} EN_MMSDK_DEMUX_RESET_CTRL_FLG;

/// Define return value.
typedef enum
{
    /// return value is OK.
    E_MMSDK_OK = 0,
    /// return value is fail.
    E_MMSDK_FAIL,
    /// return value is async.
    E_MMSDK_ASYNC
} E_MMSDK_RESULT;

/// Define control flag for TEE encryption type
typedef enum
{
    /// Not TEE
    E_MMSDK_TEE_ENCRYPT_CTL_FLAG_NONE     = 0x00000000,
    /// If TEE is, set this flag
    E_MMSDK_TEE_ENCRYPT_CTL_FLAG_ISTEE    = 0x00000001,
    /// encrypt type is VES
    E_MMSDK_TEE_ENCRYPT_CTL_FLAG_VES      = 0x00000002,
    /// encrypt type is AES
    E_MMSDK_TEE_ENCRYPT_CTL_FLAG_AES      = 0x00000004,
    /// encrypt type is VPES
    E_MMSDK_TEE_ENCRYPT_CTL_FLAG_VPES     = 0x00000008,
    /// encrypt type is APES
    E_MMSDK_TEE_ENCRYPT_CTL_FLAG_APES     = 0x00000010,
    /// encrypt type is All
    E_MMSDK_TEE_ENCRYPT_CTL_FLAG_ALL      = 0x00000020,
} E_MMSDK_TEE_ENCRYPT_CTL_FLAG;

///define 3D layout type
typedef enum
{
    /// not 3d
    E_MMSDK_3DLAYOUT_NOT_3D   =  0,
    /// side-by-side, right image first
    E_MMSDK_3DLAYOUT_SIDE_BY_SIDE_RF,
    /// side-by-side, left image first
    E_MMSDK_3DLAYOUT_SIDE_BY_SIDE_LF,
    /// top-bottom, right image top
    E_MMSDK_3DLAYOUT_TOP_BOTTOM_RT,
    /// top-bottom, left image top
    E_MMSDK_3DLAYOUT_TOP_BOTTOM_LT,
    /// separate left/right streams
    E_MMSDK_3DLAYOUT_DUAL_STREAM,
    /// more than two streams
    E_MMSDK_3DLAYOUT_MULTI_STREAM,
    /// mix with vertical odd line
    E_MMSDK_3DLAYOUT_VERTICAL_LINE_INTERLEAVED_TYPE_ODD_LINE,
    /// mix with vertical even line
    E_MMSDK_3DLAYOUT_VERTICAL_LINE_INTERLEAVED_TYPE_EVEN_LINE,
    /// mix with frame sequential odd frame
    E_MMSDK_3DLAYOUT_FRAME_SEQUENTIAL_TYPE_ODD_FRAME,
    /// mix with frame sequential even frame
    E_MMSDK_3DLAYOUT_FRAME_SEQUENTIAL_TYPE_EVEN_FRAME,
    /// LEFT_RIGHT_VIEW_SEQUENCE_TYPE_PRIMARY_VIEW
    E_MMSDK_3DLAYOUT_LEFT_RIGHT_VIEW_SEQUENCE_TYPE_PRIMARY_VIEW,
    /// LEFT_RIGHT_VIEW_SEQUENCE_TYPE_SECONDARY_VIEW
    E_MMSDK_3DLAYOUT_LEFT_RIGHT_VIEW_SEQUENCE_TYPE_SECONDARY_VIEW,
    /// mix with horizontal odd line
    E_MMSDK_3DLAYOUT_HORIZONTAL_LINE_INTERLEAVED_TYPE_ODD_LINE,
    /// mix with horizontal even line
    E_MMSDK_3DLAYOUT_HORIZONTAL_LINE_INTERLEAVED_TYPE_EVEN_LINE,
    /// FRAME_PACKING
    E_MMSDK_3DLAYOUT_FRAME_PACKING,
    /// CHECKERBOARD
    E_MMSDK_3DLAYOUT_CHECKERBOARD_INTERLEAVED_TYPE,
    /// alternative(2d)
    E_MMSDK_3DLAYOUT_FRAME_ALTERNATIVE,
} EN_MMSDK_3DLAYOUT;

/// Define VDEC codec for query capability
typedef enum
{
    ///unsupported codec type
    E_MMSDK_VDEC_CODEC_NONE = 0,
    ///MPEG 1/2
    E_MMSDK_VDEC_CODEC_MPEG2,
    ///H263 (short video header)
    E_MMSDK_VDEC_CODEC_H263,
    ///MPEG4 (default)
    E_MMSDK_VDEC_CODEC_MPEG4,
    ///MPEG4 (Divx311)
    E_MMSDK_VDEC_CODEC_DIVX311,
    ///MPEG4 (Divx412)
    E_MMSDK_VDEC_CODEC_DIVX412,
    ///FLV
    E_MMSDK_VDEC_CODEC_FLV,
    ///VC1 advanced profile (VC1)
    E_MMSDK_VDEC_CODEC_VC1_ADV,
    ///VC1 main profile (RCV)
    E_MMSDK_VDEC_CODEC_VC1_MAIN,
    ///Real Video version 8
    E_MMSDK_VDEC_CODEC_RV8,
    ///Real Video version 9 and 10
    E_MMSDK_VDEC_CODEC_RV9,
    ///H264
    E_MMSDK_VDEC_CODEC_H264,
    ///AVS
    E_MMSDK_VDEC_CODEC_AVS,
    ///MJPEG
    E_MMSDK_VDEC_CODEC_MJPEG,
    ///MVC
    E_MMSDK_VDEC_CODEC_MVC,
    ///VP8
    E_MMSDK_VDEC_CODEC_VP8,
    ///HEVC
    E_MMSDK_VDEC_CODEC_HEVC,
    ///VP9
    E_MMSDK_VDEC_CODEC_VP9,
} EN_MMSDK_VDEC_CODEC;

/// Define the vdec codec capability profile
typedef enum
{
    /// Codec Profile None
    E_MMSDK_CODEC_PROFILE_NONE,
    /// Codec Profile MP2 MAIN
    E_MMSDK_CODEC_PROFILE_MP2_MAIN,
    /// Codec Profile MP4 ASP
    E_MMSDK_CODEC_PROFILE_MP4_ASP,
    /// Codec Profile H263 BASELINE
    E_MMSDK_CODEC_PROFILE_H263_BASELINE,
    /// Codec Profile VC1 AP
    E_MMSDK_CODEC_PROFILE_VC1_AP,
    /// Codec Profile RCV MAIN
    E_MMSDK_CODEC_PROFILE_RCV_MAIN,
    /// Codec Profile VP9 0
    E_MMSDK_CODEC_PROFILE_VP9_0,
    /// Codec Profile VP9 2
    E_MMSDK_CODEC_PROFILE_VP9_2,
    /// Codec Profile H264 CBP
    E_MMSDK_CODEC_PROFILE_H264_CBP,
    /// Codec Profile H264 BP
    E_MMSDK_CODEC_PROFILE_H264_BP,
    /// Codec Profile H264 XP
    E_MMSDK_CODEC_PROFILE_H264_XP,
    /// Codec Profile H264 MP
    E_MMSDK_CODEC_PROFILE_H264_MP,
    /// Codec Profile H264 HIP
    E_MMSDK_CODEC_PROFILE_H264_HIP,
    /// Codec Profile H264 PHIP
    E_MMSDK_CODEC_PROFILE_H264_PHIP,
    /// Codec Profile H264 CHIP
    E_MMSDK_CODEC_PROFILE_H264_CHIP,
    /// Codec Profile H264 HT10P
    E_MMSDK_CODEC_PROFILE_H264_HI10P,
    /// Codec Profile H264 HI422P
    E_MMSDK_CODEC_PROFILE_H264_HI422P,
    /// Codec Profile H264 HI444PP
    E_MMSDK_CODEC_PROFILE_H264_HI444PP,
    /// Codec Profile H265 MAIN
    E_MMSDK_CODEC_PROFILE_H265_MAIN,
    /// Codec Profile H265 MAIN 10
    E_MMSDK_CODEC_PROFILE_H265_MAIN_10,
    /// Codec Profile H265 MAIN 12
    E_MMSDK_CODEC_PROFILE_H265_MAIN_12,
    /// Codec Profile H265 MAIN 422_10
    E_MMSDK_CODEC_PROFILE_H265_MAIN_422_10,
    /// Codec Profile H265 MAIN 422_12
    E_MMSDK_CODEC_PROFILE_H265_MAIN_422_12,
    /// Codec Profile H265 MAIN 444
    E_MMSDK_CODEC_PROFILE_H265_MAIN_444,
    /// Codec Profile H265 MAIN 444_10
    E_MMSDK_CODEC_PROFILE_H265_MAIN_444_10,
    /// Codec Profile H265 MAIN 444_12
    E_MMSDK_CODEC_PROFILE_H265_MAIN_444_12,
    /// Codec Profile Profile reserved size
    E_MMSDK_CODEC_PROFILE_RESERVED = 0xFFFF
} EN_MMSDK_CODEC_CAP_PROFILE_INFO;

/// Define the vdec codec capability level
typedef enum
{
    /// Codec Level None
    E_MMSDK_CODEC_LEVEL_NONE,
    /// Codec Level MP2 HIGH
    E_MMSDK_CODEC_LEVEL_MP2_HIGH,
    /// Codec Level MP4 L5
    E_MMSDK_CODEC_LEVEL_MP4_L5,
    /// Codec Level VC1 L3
    E_MMSDK_CODEC_LEVEL_VC1_L3,
    /// Codec Level REV HIGH
    E_MMSDK_CODEC_LEVEL_RCV_HIGH,
    /// Codec Level H264 1
    E_MMSDK_CODEC_LEVEL_H264_1,
    /// Codec Level H264 1B
    E_MMSDK_CODEC_LEVEL_H264_1B,
    /// Codec Level H264 1_1
    E_MMSDK_CODEC_LEVEL_H264_1_1,
    /// Codec Level H264 1_2
    E_MMSDK_CODEC_LEVEL_H264_1_2,
    /// Codec Level H264 1_3
    E_MMSDK_CODEC_LEVEL_H264_1_3,
    /// Codec Level H264 2
    E_MMSDK_CODEC_LEVEL_H264_2,
    /// Codec Level H264 2_1
    E_MMSDK_CODEC_LEVEL_H264_2_1,
    /// Codec Level H264 2_2
    E_MMSDK_CODEC_LEVEL_H264_2_2,
    /// Codec Level H264 3
    E_MMSDK_CODEC_LEVEL_H264_3,
    /// Codec Level H264 3_1
    E_MMSDK_CODEC_LEVEL_H264_3_1,
    /// Codec Level H264 3_2
    E_MMSDK_CODEC_LEVEL_H264_3_2,
    /// Codec Level H264 4
    E_MMSDK_CODEC_LEVEL_H264_4,
    /// Codec Level H264 4_1
    E_MMSDK_CODEC_LEVEL_H264_4_1,
    /// Codec Level H264 4_2
    E_MMSDK_CODEC_LEVEL_H264_4_2,
    /// Codec Level H264 5
    E_MMSDK_CODEC_LEVEL_H264_5,
    /// Codec Level H264 5_1
    E_MMSDK_CODEC_LEVEL_H264_5_1,
    /// Codec Level H264 5_2
    E_MMSDK_CODEC_LEVEL_H264_5_2,
    /// Codec Level H265 1
    E_MMSDK_CODEC_LEVEL_H265_1,
    /// Codec Level H265 2
    E_MMSDK_CODEC_LEVEL_H265_2,
    /// Codec Level H265 2_1
    E_MMSDK_CODEC_LEVEL_H265_2_1,
    /// Codec Level H265 3
    E_MMSDK_CODEC_LEVEL_H265_3,
    /// Codec Level H265 3_1
    E_MMSDK_CODEC_LEVEL_H265_3_1,
    /// Codec Level H265 4_MT
    E_MMSDK_CODEC_LEVEL_H265_4_MT,
    /// Codec Level H265 4_HT
    E_MMSDK_CODEC_LEVEL_H265_4_HT,
    /// Codec Level H265 4_1_MT
    E_MMSDK_CODEC_LEVEL_H265_4_1_MT,
    /// Codec Level H265 4_1_HT
    E_MMSDK_CODEC_LEVEL_H265_4_1_HT,
    /// Codec Level H265 5_MT
    E_MMSDK_CODEC_LEVEL_H265_5_MT,
    /// Codec Level H265 5_HT
    E_MMSDK_CODEC_LEVEL_H265_5_HT,
    /// Codec Level H265 5_1_MT
    E_MMSDK_CODEC_LEVEL_H265_5_1_MT,
    /// Codec Level H265 5_1_HT
    E_MMSDK_CODEC_LEVEL_H265_5_1_HT,
    /// Codec Level H265 5_2_MT
    E_MMSDK_CODEC_LEVEL_H265_5_2_MT,
    /// Codec Level H265 5_2_HT
    E_MMSDK_CODEC_LEVEL_H265_5_2_HT,
    /// Codec Level H265 6_MT
    E_MMSDK_CODEC_LEVEL_H265_6_MT,
    /// Codec Level H265 6_HT
    E_MMSDK_CODEC_LEVEL_H265_6_HT,
    /// Codec Level H265 6_1_MT
    E_MMSDK_CODEC_LEVEL_H265_6_1_MT,
    /// Codec Level H265 6_1_HT
    E_MMSDK_CODEC_LEVEL_H265_6_1_HT,
    /// Codec Level H265 6_2_MT
    E_MMSDK_CODEC_LEVEL_H265_6_2_MT,
    /// Codec Level H265 6_2_HT
    E_MMSDK_CODEC_LEVEL_H265_6_2_HT,
    /// Codec Level reserved size
    E_MMSDK_CODEC_LEVEL_RESERVED = 0xFFFF
} EN_MMSDK_CODEC_CAP_LEVEL_INFO;

/// Define the vdec codec capability version info
typedef enum
{
    /// Codec Version None
    E_MMSDK_CODEC_VERSION_NONE,
    /// Codec Version DIVX 311
    E_MMSDK_CODEC_VERSION_DIVX_311,
    /// Codec Version DIVX 4
    E_MMSDK_CODEC_VERSION_DIVX_4,
    /// Codec Version DIVX 5
    E_MMSDK_CODEC_VERSION_DIVX_5,
    /// Codec Version DIVX 6
    E_MMSDK_CODEC_VERSION_DIVX_6,
    /// Codec Version DIVX FLV 1
    E_MMSDK_CODEC_VERSION_FLV_1,
    /// Codec Version DIVX H263 1
    E_MMSDK_CODEC_VERSION_H263_1,
} EN_MMSDK_CODEC_CAP_VERSION_INFO;

/// Define the structure of vdec codec capability info
typedef struct
{
    /// Codec capability width
    MMSDK_U16 u16CodecCapWidth;
    /// Codec capability height
    MMSDK_U16 u16CodecCapHeight;
    /// Codec capability profile info
    EN_MMSDK_CODEC_CAP_PROFILE_INFO  enCodecCapProfile;
    /// Codec capability version info
    EN_MMSDK_CODEC_CAP_VERSION_INFO  enCodecCapVersion;
    /// Codec capability level info
    EN_MMSDK_CODEC_CAP_LEVEL_INFO  enCodecCapLevel;
    /// Codec capability frame rate
    MMSDK_U8  u8CodecCapFrameRate;
    /// The codec to query
    EN_MMSDK_VDEC_CODEC enVideoCodec;
    /// Codec capability reserved 1
    MMSDK_U32 u32Reserved1;
    /// Codec capability reserved 2
    MMSDK_U32 u32Reserved2;
} ST_MMSDK_CODEC_CAP_INFO;

/// Define the Pipe Type
typedef enum
{
    /// pipe type is master.
    E_MMSDK_PIPE_ENGINE_TYPE_MASTER,
    /// pipe type is vdec.
    E_MMSDK_PIPE_ENGINE_TYPE_VDEC,
    /// pipe type is xc.
    E_MMSDK_PIPE_ENGINE_TYPE_XC,
    /// pipe type is none. Add new type before it.
    E_MMSDK_PIPE_ENGINE_TYPE_NUM,
    /// MAX (reserved for enum space)
    E_MMSDK_PIPE_ENGINE_TYPE_MAX = E_MMSDK_PIPE_ENGINE_TYPE_NUM,
}EN_MMSDK_PIPE_ENGINE_TYPE;

/// Define Callback Message Type Parameter structure: update pipe info
typedef struct
{
    /// pipe id of supernova player
    MMSDK_U32 u32SvpPplID;
    /// pipe type
    EN_MMSDK_PIPE_ENGINE_TYPE ePipeType;
    /// pipe id enable or not
    MMSDK_BOOL bPipeEnable;
}ST_MMSDK_PIPE_INFO;

/// Define Callback Message Type Parameter structure for dolby vision: Notify
typedef struct
{
    /// Media item.
    MEDIAITEM Item;
    /// u32 return
    MMSDK_U32 u32Ret;
    /// bool return or media item is dolby vision stream;
    MMSDK_BOOL bRet;
    /// Define the information for create media item.
    ST_MMSDK_CREATE_MEDIA_INFO* pstMedia;
    /// Define the info for general parameter setting
    ST_MMSDK_PARAM* astExtenInfoParam;
    /// The number of parameters (default 0)
    MMSDK_U32 u32ExtenInfoParamNum;
} ST_MMSDK_CALLBACK_NOTIFY_DOLBY_VISION_STREAM;

/// Define Reorder Type of dolby vision
typedef enum
{
    /// Default value, metadata follow BL
    E_MMSDK_DV_META_REORDER_DEFAULT,
    /// single track (single pid is 1), metadata follow BL
    E_MMSDK_DV_META_REORDER_FOLLOW_BL,
    /// dual track (dual pid is 2), metadata follow EL
    E_MMSDK_DV_META_REORDER_FOLLOW_EL,
} EN_MMSDK_DV_METADATA_REORDER;

/// Define the structure of the dolby vision information in the es mode
typedef struct
{
    /// Structure version
    MMSDK_U32 u32Version;
    /// Structure version
    MMSDK_U32 u32Size;
    /// Provide the dolby vision profile to VDEC
    MMSDK_U32 u32DVProfile;
    /// Provide the dolby vision level to VDEC
    MMSDK_U32 u32DVLevel;
    /// Provide the meta data reorder to player
    EN_MMSDK_DV_METADATA_REORDER eDVMetaDataReorder;
} ST_MMSDK_DOLBY_VISION_INFO;

/// Define mmsdk error code offset
#define MMSDK_ERROR_CODE_ST   0xA001
#define MMSDK_ERROR_CODE_VDP_ST   0xE001

/// Define the mmsdk error code set
typedef enum
{
    /// MMSDK default error code(no error)
    EN_MMSDK_ERRORCODE_NONE = 0,
    /// MMSDK error code start
    EN_MMSDK_ERRORCODE_BASE = MMSDK_ERROR_CODE_ST,
    /// Create media item error with mmsdk is uninitialized
    EN_MMSDK_ERRORCODE_CREATE_UNINITIALIZED = MMSDK_ERROR_CODE_ST + 0x10,     // 0x11,
    /// Create media item error with the input create information error
    EN_MMSDK_ERRORCODE_CREATE_INPUT_PARAM_ERROR,
    /// Create media item error with memory allocate fail
    EN_MMSDK_ERRORCODE_CREATE_ALLOCATE_MEMORY_FAIL,
    /// Create media item error with the mmsdk lib version expired
    EN_MMSDK_ERRORCODE_CREATE_VERSION_EXPIRED,
    /// Create media item error with the unsupport input media type
    EN_MMSDK_ERRORCODE_CREATE_UNSUPPORT_MEDIA_TYPE,
    /// Create media item error with open ini setting file fail
    EN_MMSDK_ERRORCODE_CREATE_OPEN_INI_FAIL,
    /// Create media item error with PT_Audio_Initialize() return fail
    EN_MMSDK_ERRORCODE_CREATE_GET_AUDIO_RESOURCE_FAIL,
    /// Create media item error with PT_Display_Initialize() return fail
    EN_MMSDK_ERRORCODE_CREATE_GET_DISPLAY_RESOURCE_FAIL,
    /// Create media item error with the undefined error
    EN_MMSDK_ERRORCODE_CREATE_UNDEFINED_ERROR,  // 0x19;

    ///Vdplayer error with general error
    EN_MMSDK_ERRORCODE_VDP_GENERAL_ERROR              = MMSDK_ERROR_CODE_VDP_ST,
    ///Vdplayer error with logical error
    EN_MMSDK_ERRORCODE_VDP_LOGICAL_ERROR,
    ///------ RESOURCE
    ///Vdplayer resource error with AEON cannot init
    EN_MMSDK_ERRORCODE_VDP_RESOURCE_AEON_CANNOTINIT   = MMSDK_ERROR_CODE_VDP_ST + 0x10,     // 0x11,
    ///Vdplayer resource error with memory not enough
    EN_MMSDK_ERRORCODE_VDP_RESOURCE_MEMORY_NOTENOUGH,
    ///Vdplayer resource error with memory overflow
    EN_MMSDK_ERRORCODE_VDP_RESOURCE_MEMORY_OVERFLOW,
    ///Vdplayer resource error with wrong data of memory
    EN_MMSDK_ERRORCODE_VDP_RESOURCE_MEMORY_WRONGDATA,
    ///Vdplayer resource error with no file size
    EN_MMSDK_ERRORCODE_VDP_RESOURCE_NO_FILESIZE,
    ///Vdplayer resource error with file open error
    EN_MMSDK_ERRORCODE_VDP_ERR_FILE_OPEN,
    ///Vdplayer resource error with file read error
    EN_MMSDK_ERRORCODE_VDP_ERR_FILE_READ,
    ///------ CONTAINER
    ///Vdplayer container error with unknow container
    EN_MMSDK_ERRORCODE_VDP_CONTAINER_UNKNOW           = MMSDK_ERROR_CODE_VDP_ST + 0x20,     // 0x21
    ///Vdplayer container error with container not support
    EN_MMSDK_ERRORCODE_VDP_CONTAINER_NOTSUPPORT,
    ///Vdplayer container error with wrong container
    EN_MMSDK_ERRORCODE_VDP_CONTAINER_ERROR,
    ///------ TIMEOUT
    ///Vdplayer timeout error with file read timeout
    EN_MMSDK_ERRORCODE_VDP_FILEREAD_TIMEOUT           = MMSDK_ERROR_CODE_VDP_ST + 0x30,     // 0x31,
    ///Vdplayer timeout error with demux timeout
    EN_MMSDK_ERRORCODE_VDP_DEMUX_TIMEOUT,
    ///Vdplayer timeout error with housekeeping demux timeout
    EN_MMSDK_ERRORCODE_VDP_DEMUX_TIMEOUT_HOUSEKEEPING,
    ///------ NOT ENOUGH INFO
    ///Vdplayer not enough content error with duration
    EN_MMSDK_ERRORCODE_VDP_NOT_ENOUGH_CONTENT_DURATION= MMSDK_ERROR_CODE_VDP_ST + 0x40,     // 0x41,
    ///Vdplayer not enough content error with framerate
    EN_MMSDK_ERRORCODE_VDP_NOT_ENOUGH_CONTENT_FRAMERATE,
    ///Vdplayer not enough content error with bitrate
    EN_MMSDK_ERRORCODE_VDP_NOT_ENOUGH_CONTENT_BITRATE,
    ///Vdplayer not enough content error with width
    EN_MMSDK_ERRORCODE_VDP_NOT_ENOUGH_CONTENT_WIDTH,
    ///Vdplayer not enough content error with height
    EN_MMSDK_ERRORCODE_VDP_NOT_ENOUGH_CONTENT_HEIGHT,
    ///------ NOT SUPPORT
    ///Vdplayer not support error with file not support
    EN_MMSDK_ERRORCODE_VDP_NOT_SUPPORT_FILE           = MMSDK_ERROR_CODE_VDP_ST + 0x50,     // 0x51,
    ///Vdplayer not support error with screan size not support
    EN_MMSDK_ERRORCODE_VDP_NOT_SUPPORT_SCREANSIZE,
    ///Vdplayer not support error with frame rate not support
    EN_MMSDK_ERRORCODE_VDP_NOT_SUPPORT_FRAMERATE,
    ///Vdplayer not support error with not support video codec
    EN_MMSDK_ERRORCODE_VDP_NOT_SUPPORT_CODEC_VIDEO,
    ///Vdplayer not support error with not support more video
    EN_MMSDK_ERRORCODE_VDP_NOT_SUPPORT_MORE_VIDEO,
    ///Vdplayer not support error with not support no video
    EN_MMSDK_ERRORCODE_VDP_NOT_SUPPORT_NO_VIDEO,
    ///Vdplayer not support error with not support more audio
    EN_MMSDK_ERRORCODE_VDP_NOT_SUPPORT_MORE_AUDIO,
    ///Vdplayer not support error with not support no supported audio
    EN_MMSDK_ERRORCODE_VDP_NOT_SUPPORT_NO_SUPPORTED_AUDIO,
    ///Vdplayer not support error with not support DRM
    EN_MMSDK_ERRORCODE_VDP_NOT_SUPPORT_DRM,
    ///Vdplayer not support error with not support MM
    EN_MMSDK_ERRORCODE_VDP_NOT_MM,
    /// ------ NOT SUPPORT ---H264
    ///Vdplayer H264 not support error code start
    EN_MMSDK_ERRORCODE_VDP_NOT_SUPPORT_H264_BASE  = MMSDK_ERROR_CODE_VDP_ST + 0x60,     // 0x61
    ///Vdplayer H264 not support error with out of memory
    EN_MMSDK_ERRORCODE_VDP_NOT_SUPPORT_H264_OUT_OF_MEMORY = EN_MMSDK_ERRORCODE_VDP_NOT_SUPPORT_H264_BASE,
    ///Vdplayer H264 not support error code res
    EN_MMSDK_ERRORCODE_VDP_NOT_SUPPORT_H264_RES,
    ///Vdplayer H264 not support error code max
    EN_MMSDK_ERRORCODE_VDP_NOT_SUPPORT_H264_MAX   = EN_MMSDK_ERRORCODE_VDP_NOT_SUPPORT_H264_RES,
    /// ------ NOT SUPPORT ---MPEG4
    ///Vdplayer MPEG4 not support error code start
    EN_MMSDK_ERRORCODE_VDP_NOT_SUPPORT_MPEG4_BASE = MMSDK_ERROR_CODE_VDP_ST + 0x70,     // 0x71
    ///Vdplayer MPEG4 not support error with vol shape
    EN_MMSDK_ERRORCODE_VDP_NOT_SUPPORT_MPEG4_VOL_SHAPE = EN_MMSDK_ERRORCODE_VDP_NOT_SUPPORT_MPEG4_BASE,
    ///Vdplayer MPEG4 not support error with vol used sprite
    EN_MMSDK_ERRORCODE_VDP_NOT_SUPPORT_MPEG4_VOL_USED_SPRITE,
    ///Vdplayer MPEG4 not support error with vol not 8 bit
    EN_MMSDK_ERRORCODE_VDP_NOT_SUPPORT_MPEG4_VOL_NOT_8_BIT,
    ///Vdplayer MPEG4 not support error with vol nerpred enable
    EN_MMSDK_ERRORCODE_VDP_NOT_SUPPORT_MPEG4_VOL_NERPRED_ENABLE,
    ///Vdplayer MPEG4 not support error with vol reduced RES enable
    EN_MMSDK_ERRORCODE_VDP_NOT_SUPPORT_MPEG4_VOL_REDUCED_RES_ENABLE,
    ///Vdplayer MPEG4 not support error with vol scalability
    EN_MMSDK_ERRORCODE_VDP_NOT_SUPPORT_MPEG4_VOL_SCALABILITY,
    ///Vdplayer MPEG4 not support error with vol H263 error
    EN_MMSDK_ERRORCODE_VDP_NOT_SUPPORT_MPEG4_VOL_H263_ERROR,
    ///Vdplayer MPEG4 not support error code res
    EN_MMSDK_ERRORCODE_VDP_NOT_SUPPORT_MPEG4_RES,
    ///Vdplayer MPEG4 not support error code max
    EN_MMSDK_ERRORCODE_VDP_NOT_SUPPORT_MPEG4_MAX  = EN_MMSDK_ERRORCODE_VDP_NOT_SUPPORT_MPEG4_RES,
    /// ------ NOT SUPPORT ---RM
    ///Vdplayer RM not support error code start
    EN_MMSDK_ERRORCODE_VDP_NOT_SUPPORT_RM_BASE    = MMSDK_ERROR_CODE_VDP_ST + 0x80,     // 0x81
    ///Vdplayer RM not support error with multi rate
    EN_MMSDK_ERRORCODE_VDP_NOT_SUPPORT_RM_MULTI_RATE  = EN_MMSDK_ERRORCODE_VDP_NOT_SUPPORT_RM_BASE,
    ///Vdplayer RM not support error code res
    EN_MMSDK_ERRORCODE_VDP_NOT_SUPPORT_RM_RES,
    ///Vdplayer RM not support error code max
    EN_MMSDK_ERRORCODE_VDP_NOT_SUPPORT_RM_MAX     = EN_MMSDK_ERRORCODE_VDP_NOT_SUPPORT_RM_RES,
    ///------ DRM
    ///Vdplayer DRM error with header
    EN_MMSDK_ERRORCODE_VDP_DRM_ERR_HEADER             = MMSDK_ERROR_CODE_VDP_ST + 0x90,     // 0x91
    ///Vdplayer DRM error with not authorized
    EN_MMSDK_ERRORCODE_VDP_DRM_ERR_NOT_AUTHORIZED,
    ///Vdplayer DRM error with not registered
    EN_MMSDK_ERRORCODE_VDP_DRM_ERR_NOT_REGISTERED,
    ///Vdplayer DRM error with rental expired
    EN_MMSDK_ERRORCODE_VDP_DRM_ERR_RENTAL_EXPIRED,
    ///Vdplayer DRM error with version expired
    EN_MMSDK_ERRORCODE_VDP_DRM_ERR_VERSION_EXPIRED,
    /// ----- Funtion error
    ///Vdplayer function error with demux header
    EN_MMSDK_ERRORCODE_VDP_FUNC_ERR_DEMUX_HEADER      = MMSDK_ERROR_CODE_VDP_ST + 0xA0,     // 0xA1
    ///Vdplayer function error with change audio
    EN_MMSDK_ERRORCODE_VDP_FUNC_ERR_CHANGE_AUDIO,
    ///Vdplayer function error with demux packet
    EN_MMSDK_ERRORCODE_VDP_FUNC_ERR_DEMUX_PACKET,
    ///Vdplayer function error with wait disp ready
    EN_MMSDK_ERRORCODE_VDP_FUNC_ERR_WAIT_DISP_READY,
    ///Vdplayer function error with scan index
    EN_MMSDK_ERRORCODE_VDP_FUNC_ERR_SCAN_INDEX,
    ///Vdplayer function error with audio far behind video
    EN_MMSDK_ERRORCODE_VDP_FUNC_ERR_AUDIO_FAR_BEHIND_VIDEO,
    ///Vdplayer function error with BM init
    EN_MMSDK_ERRORCODE_VDP_FUNC_ERR_BM_INIT,
    ///Vdplayer function error with demuxer init
    EN_MMSDK_ERRORCODE_VDP_FUNC_ERR_DEMUXER_INIT,
    ///Vdplayer function error with decode init
    EN_MMSDK_ERRORCODE_VDP_FUNC_ERR_DECODE_INIT,
    ///Vdplayer function error with memory init
    EN_MMSDK_ERRORCODE_VDP_FUNC_ERR_MEMORY_INIT,
    ///Vdplayer function error with decode error
    EN_MMSDK_ERRORCODE_VDP_FUNC_ERR_DECODE_ERROR,
    ///Vdplayer function error with decode monitor error
    EN_MMSDK_ERRORCODE_VDP_FUNC_ERR_DECODE_MONITOR_ERROR,
    /// ----- HW error
    ///Vdplayer HW error with video init failed
    EN_MMSDK_ERRORCODE_VDP_HW_VIDEO_INIT_FAILED       = MMSDK_ERROR_CODE_VDP_ST + 0xB0,     // 0xB1
    ///Vdplayer HW error with video code max
    EN_MMSDK_ERRORCODE_VDP_MAX,
} EN_MMSDK_ERRORCODE;

/// Define X, Y coordinate for HDR10 config
#define MMSDK_HDR10_COORDINATE      (2)

/// Define R, G, B chromaticity for HDR10 config
#define MMSDK_HDR10_CHROMATICITY    (3)

/// Settings describing the colour format for HDR10 config.
/// Refer to https://www.matroska.org/technical/specs/index.html
typedef struct
{
    /// Structure version
    MMSDK_U32 u32Version;
    /// Structure size
    MMSDK_U32 u32Size;
    /// Maximum luminance. Shall be represented in candelas per square meter (cd/m2).
    MMSDK_U32  u32MaxLuminance;
    /// Minimum luminance. Shall be represented in candelas per square meter (cd/m2).
    MMSDK_U32  u32MinLuminance;
    /// (Red, Green, Blue) (X, Y) chromaticity coordinate as defined by CIE 1931.
    MMSDK_U16  u16Primaries[MMSDK_HDR10_CHROMATICITY][MMSDK_HDR10_COORDINATE];  /// [R,G,B][X,Y]
    /// White X, Y chromaticity coordinate as defined by CIE 1931.
    MMSDK_U16  u16WhitePoint[MMSDK_HDR10_COORDINATE];     /// [X,Y]
    /// Maximum brightness of a single pixel (Maximum Content Light Level) in candelas per square meter (cd/m2).
    MMSDK_U16  u16MaxCLL;
    /// Maximum brightness of a single full frame (Maximum FrameAverage Light Level) in candelas per square meter (cd/m2).
    MMSDK_U16  u16MaxFALL;
    /// The Matrix Coefficients of the video used to derive luma and chroma values from reg, green, and blue color primaries.
    /// For clarity, the value and meanings for MatrixCoefficients are adopted from Table 4 of ISO/IEC 230018:2013/DCOR1.
    /// (0:GBR, 1: BT709, 2: Unspecified, 3: Reserved, 4: FCC, 5: BT470BG, 6: SMPTE 170M, 7: SMPTE 240M, 8: YCOCG, 9: BT2020 Nonconstant Luminance, 10: BT2020 Constant Luminance)
    MMSDK_U8   u8MatrixCoefficients;
    /// Number of decoded bits per channel. A value of 0 indicates that the BitsPerChannel is unspecified .
    MMSDK_U8   u8BitsPerChannel;
    /// The amount of pixels to remove in the Cr and Cb channels for every pixel not removed horizontally.
    /// Example: For video with 4:2:0 chroma subsampling, the ChromaSubsamplingHorz should be set to 1.
    MMSDK_U8   u8ChromaSubsamplingHorz;
    /// The amount of pixels to remove in the Cr and Cb channels for every pixel not removed vertically.
    /// Example: For video with 4:2:0 chroma subsampling, the ChromaSubsamplingVert should be set to 1.
    MMSDK_U8   u8ChromaSubsamplingVert;
    /// The amount of pixels to remove in the Cb channel for every pixel not removed horizontally. This is additive with ChromaSubsamplingHorz.
    /// Example: For video with 4:2:1 chroma subsampling, the ChromaSubsamplingHorz should be set to 1 and CbSubsamplingHorz should be set to 1.
    MMSDK_U8   u8CbSubsamplingHorz;
    /// The amount of pixels to remove in the Cb channel for every pixel not removed vertically. This is additive with ChromaSubsamplingVert.
    MMSDK_U8   u8CbSubsamplingVert;
    /// How chroma is subsampled horizontally. (0:Unspecified, 1: Left Collocated, 2: Half)
    MMSDK_U8   u8ChromaSitingHorz;
    /// How chroma is subsampled vertically. (0:Unspecified, 1: Top Collocated, 2: Half)
    MMSDK_U8   u8ChromaSitingVert;
    /// Clipping of the color ranges. (0: Unspecified, 1:Broadcast Range, 2: Full range (no clipping), 3:Defined by MatrixCoefficients/ TransferCharacteristics )
    MMSDK_U8   u8ColorRange;
    /// The transfer characteristics of the video. For clarity, the value and meanings for TransferCharacteristics 115 are adopted from Table 3 of ISO/IEC 230018:2013/DCOR1. TransferCharacteristics 1618 are proposed values.
    /// (0: Reserved, 1: ITUR BT.709, 2:Unspecified, 3: Reserved, 4: Gamma 2.2 curve, 5: Gamma 2.8 curve, 6: SMPTE 170M, 7:SMPTE 240M, 8: Linear, 9: Log, 10: Log Sqrt, 11: IEC 6196624,
    /// 12: ITUR BT.1361 Extended Colour Gamut, 13: IEC 6196621, 14: ITUR BT.2020 10 bit, 15: ITUR BT.2020 12 bit, 16: SMPTE ST 2084, 17: SMPTE ST 4281 18:ARIB STDB67 (HLG))
    MMSDK_U8   u8TransferCharacteristics;
    /// The colour primaries of the video. For clarity, the value and meanings for Primaries are adopted from Table 2 of ISO/IEC 230018:2013/DCOR1.
    /// (0: Reserved, 1: ITUR BT.709, 2: Unspecified, 3: Reserved, 4: ITUR BT.470M, 5: ITUR BT.470BG, 6: SMPTE 170M, 7: SMPTE 240M, 8: FILM, 9: ITUR BT.2020, 10: SMPTE ST 4281, 22: JEDEC P22 phosphors)
    MMSDK_U8   u8ColourPrimaries;
} ST_MMSDK_HDR10_CONFIG;

/// Define the Download Module ffmpeg set
typedef struct
{
    /// Probe size for av_find_stream_info
    MMSDK_U32 u32ProbeSize;
    /// Duration size for av_find_stream_info
    MMSDK_U32 u32MaxAnalyzeDuration;
    /// Check Info is set
    MMSDK_BOOL bIsSetDownloadMoudleInfo;
} ST_MMSDK_DOWNLOAD_MODULE_INFO;

/// Define the playback status(now used for dolby vision)
typedef enum
{
    /// playback status is resume(pause to play)
    E_MMSDK_PLAYBACK_STATUS_RESUME,
    /// playback status is pause; reserved
    E_MMSDK_PLAYBACK_STATUS_PAUSE,
    /// playback status is stop; reserved
    E_MMSDK_PLAYBACK_STATUS_STOP,
} EN_MMSDK_PLAYBACK_STATUS;

/// Define video file format
typedef enum
{
    /// Unknown
    E_MMSDK_FILE_FORMAT_UNKNOWN = -1,
    /// AVI
    E_MMSDK_FILE_FORMAT_AVI = 0,
    /// MP4
    E_MMSDK_FILE_FORMAT_MP4,
    /// MKV
    E_MMSDK_FILE_FORMAT_MKV,
    /// ASF
    E_MMSDK_FILE_FORMAT_ASF,
    /// RM
    E_MMSDK_FILE_FORMAT_RM,
    /// TS
    E_MMSDK_FILE_FORMAT_TS,
    /// MPG
    E_MMSDK_FILE_FORMAT_MPG,
    /// FLV
    E_MMSDK_FILE_FORMAT_FLV,
    /// ES DATA
    E_MMSDK_FILE_FORMAT_ESDATA,
    /// DSS
    E_MMSDK_FILE_FORMAT_DSS,
    /// SWF
    E_MMSDK_FILE_FORMAT_SWF,
    /// OGM
    E_MMSDK_FILE_FORMAT_OGM,
} EN_MMSDK_FILE_FORMAT;

/** @} */ // end of mmsdk_interface_def

#ifdef __cplusplus
}
#endif
#endif

///*** please do not remove change list tag ***///
///***$Change: 1460642 $***///
