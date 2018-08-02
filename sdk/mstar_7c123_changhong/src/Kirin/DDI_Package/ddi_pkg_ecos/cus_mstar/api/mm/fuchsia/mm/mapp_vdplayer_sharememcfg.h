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
////////////////////////////////////////////////////////////////////////////////

#ifndef MAPP_VDPLAYER_SHAREMENCFG_H
#define MAPP_VDPLAYER_SHAREMENCFG_H

#define SHAREMEM_VERSION_NUM 200054

#define E_SHAREMEM_RM_LBR_SIZE                              (6)
#define E_SHAREMEM_DRM_BUFF_ADDR_SIZE                       (48)
#define E_SHAREMEM_DRM_KEY_TABLE_SIZE                       (2048/4)
#define E_SHAREMEM_CUSTOMER_INFO_SIZE                       (48/4)
#define E_SHAREMEM_CUSTOMER_HASH_SIZE                       (16/4)
#define E_SHAREMEM_BUFFER_INFO_SIZE                         (8)
#define E_SHAREMEM_TRACK_SIZE                               (10)
#define E_SHAREMEM_MAGICKEY                                 (0x5A5AA5A5)

typedef enum
{
    E_SHAREMEM_VERSION_NUM ,
    E_SHAREMEM_DRM_BUFF_ADDR ,                  // HK -> Co-processor
    E_SHAREMEM_DATA_BUFF_ADDR       = E_SHAREMEM_DRM_BUFF_ADDR + E_SHAREMEM_DRM_BUFF_ADDR_SIZE,
    E_SHAREMEM_DATA_BUFF_LEN,                   // HK -> Co-processor
    E_SHAREMEM_BITSREAM_BUFF_ADDR,              // HK -> Co-processor
    E_SHAREMEM_BITSREAM_BUFF_LEN,               // HK -> Co-processor
    E_SHAREMEM_SUBTITLE_BITSREAM_BUFF_ADDR,     // HK -> Co-processor
    E_SHAREMEM_SUBTITLE_BITSREAM_BUFF_LEN,      // HK -> Co-processor
    E_SHAREMEM_SUBTITLE_ATTACHEDFILE_BUFF_ADDR, // HK -> Co-processor
    E_SHAREMEM_SUBTITLE_ATTACHEDFILE_BUFF_LEN,  // HK -> Co-processor
    E_SHAREMEM_SUBTITLE_ATTACHEDFILE_SIZE,      // Co-processor -> HK
    E_SHAREMEM_MVD3_FW_ADDR,                    // HK -> Co-processor
    E_SHAREMEM_MVD3_FW_LEN,                     // HK -> Co-processor
    E_SHAREMEM_MAD_BUFF_ADDR,                   // HK -> Co-processor
    E_SHAREMEM_MAD_BUFF_LEN,                    // HK -> Co-processor
    E_SHAREMEM_TSP_FW_ADDR,                     // HK -> Co-processor
    E_SHAREMEM_TSP_FW_LEN,                      // HK -> Co-processor
    E_SHAREMEM_MVD_BITSTREAM_ADR,               // HK -> Co-processor
    E_SHAREMEM_MVD_BITSTREAM_LEN,               // HK -> Co-processor
    E_SHAREMEM_MVD_FRAMEBUFFER_ADR,             // HK -> Co-processor
    E_SHAREMEM_MVD_FRAMEBUFFER_LEN,             // HK -> Co-processor

    E_SHAREMEM_MVD_FRAMEBUFFER_ADR_SD,          // HK -> Co-processor
    E_SHAREMEM_MVD_FRAMEBUFFER_LEN_SD,          // HK -> Co-processor
    E_SHAREMEM_MVD_BITSTREAM_ADR_SD,            // HK -> Co-processor
    E_SHAREMEM_MVD_BITSTREAM_LEN_SD,            // HK -> Co-processor
    E_SHAREMEM_MVD_FRAMEBUFFER_MODE,            // Decoder will set it to be FB or FBL for display setting , Co-processor -> HK
    E_SHAREMEM_MVD_FRAMEBUFFER_MODE_CHECKING,   // HK -> Co-processor, HK will set it for FB or FBL detection
    E_SHAREMEM_MVD_FBL_MODE_THRESHOLD,          // HK -> Co-processor, FBL mode threshold(the valuse is WxH)
    E_SHAREMEM_BITSREAM_BUFF_EXT_ADDR,          // HK -> Co-processor
    E_SHAREMEM_BITSREAM_BUFF_EXT_LEN,           // HK -> Co-processor
    E_SHAREMEM_VDEC_FW_ADDR,
    E_SHAREMEM_VDEC_FW_LEN,

    E_SHAREMEM_WIDTH,
    E_SHAREMEM_HEIGHT,
    E_SHAREMEM_DEMUX_WIDTH,
    E_SHAREMEM_DEMUX_HEIGHT,
    E_SHAREMEM_PAR_WIDTH,
    E_SHAREMEM_PAR_HEIGHT,
    E_SHAREMEM_VIDEO_CROP_RIGHT,
    E_SHAREMEM_VIDEO_CROP_LEFT,
    E_SHAREMEM_VIDEO_CROP_BOTTOM,
    E_SHAREMEM_VIDEO_CROP_TOP,
    E_SHAREMEM_INTERLACE,
    E_SHAREMEM_FRAME_RATE,
    E_SHAREMEM_VIDEO_RANGE,

    E_SHAREMEM_VIDEO_CODEC_ID,
    E_SHAREMEM_AUDIO_CODEC_ID,
    E_SHAREMEM_SUBTITLE_CODEC_ID,
    E_SHAREMEM_CURRENT_AUDIO_TRACK_ID,
    E_SHAREMEM_TOTAL_ADUIO_TRACK,
    E_SHAREMEM_CURRENT_SUBTITLE_TRACK_ID,
    E_SHAREMEM_CURRENT_SUBTITLE_TEXT,
    E_SHAREMEM_CURRENT_SUBTITLE_ISUNICODE,
    E_SHAREMEM_CURRENT_SUBTITLE_TAGCOUNT,
    E_SHAREMEM_TOTAL_SUBTITLE,
    E_SHAREMEM_TOTAL_TITLE,
    E_SHAREMEM_TOTAL_EDITION,
    E_SHAREMEM_TOTAL_CHAPTER,

    E_SHAREMEM_RM_LBR_NUM_CODECID,      // lbr, audio only
    E_SHAREMEM_RM_LBR_SAMPLE_CHANNEL,   // lbr, audio only
    E_SHAREMEM_RM_LBR_SAMPLE_RATE ,     // lbr, audio only
    E_SHAREMEM_RM_LBR_NUM_CHANNEL ,     // lbr, audio only
    E_SHAREMEM_RM_LBR_NUM_REGIONS   = E_SHAREMEM_RM_LBR_NUM_CHANNEL + E_SHAREMEM_RM_LBR_SIZE,   // lbr, audio only
    E_SHAREMEM_RM_LBR_NUM_CPLSTART  = E_SHAREMEM_RM_LBR_NUM_REGIONS + E_SHAREMEM_RM_LBR_SIZE,   // lbr, audio only
    E_SHAREMEM_RM_LBR_NUM_CPLQBITS  = E_SHAREMEM_RM_LBR_NUM_CPLSTART + E_SHAREMEM_RM_LBR_SIZE,  // lbr, audio only
    E_SHAREMEM_RM_LBR_FRAMESIZE     = E_SHAREMEM_RM_LBR_NUM_CPLQBITS + E_SHAREMEM_RM_LBR_SIZE,  // lbr, audio only
    E_SHAREMEM_TS_PROGRAM_NUM       = E_SHAREMEM_RM_LBR_FRAMESIZE + E_SHAREMEM_RM_LBR_SIZE,
    E_SHAREMEM_TS_CUR_PROGRAM_IDX ,

    E_SHAREMEM_WMA_VERSION,
    E_SHAREMEM_WMA_CHANNELS,
    E_SHAREMEM_WMA_SAMPLERATE,
    E_SHAREMEM_WMA_BYTERATE,
    E_SHAREMEM_WMA_BLOCKALIGN,
    E_SHAREMEM_WMA_ENCODEOPT,
    E_SHAREMEM_WMA_BIT_PER_SAMPLE,
    E_SHAREMEM_WMA_CHANNEL_MASK,
    E_SHAREMEM_WMA_DRC_PARAM_EXIST,
    E_SHAREMEM_WMA_DRC_PEEK_REF,
    E_SHAREMEM_WMA_DRC_PEEK_TARGET,
    E_SHAREMEM_WMA_DRC_AVE_REF,
    E_SHAREMEM_WMA_DRC_AVE_TARGET,

    E_SHAREMEM_DRM_RETURN_CODE,
    E_SHAREMEM_DRM_FILE_FORMAT,
    E_SHAREMEM_DRM_AUTHORIZATION_STATUS,
    E_SHAREMEM_DRM_RENTAL_EXPIRE_STATUS,
    E_SHAREMEM_DRM_RENTAL_FILE,
    E_SHAREMEM_DRM_RENTAL_USE_LIMIT,
    E_SHAREMEM_DRM_RENTAL_USE_COUNT,
    E_SHAREMEM_DRM_MODEL_ID,
    E_SHAREMEM_DRM_CHECK_STATUS,
    E_SHAREMEM_DRM_CONTEXT_ADDR,

    E_SHAREMEM_DECODE_STATUS    = E_SHAREMEM_DRM_CONTEXT_ADDR + E_SHAREMEM_DRM_KEY_TABLE_SIZE,
    E_SHAREMEM_ERROR_INFO,
    E_SHAREMEM_PREVIEW_MODE,                    // HK -> Co-processor
    E_SHAREMEM_PLAY_MODE,
    E_SHAREMEM_PLAY_SPEED,
    E_SHAREMEM_MAX_FAST_FORWARD_SPEED,
    E_SHAREMEM_MAX_FAST_BACKWARD_SPEED,
    E_SHAREMEM_BACKWARD_REPLAY_MODE,            // HK -> Co-processor
    E_SHAREMEM_RESUME_PLAY_SEEKPOS_LOW,         // HK -> Co-processor
    E_SHAREMEM_RESUME_PLAY_SEEKPOS_HIGH,        // HK -> Co-processor
    E_SHAREMEM_RESUME_PLAY_SEEKPTS,             // HK -> Co-processor
    E_SHAREMEM_RESUME_AUDIO_TRACK_ID,           // HK -> Co-processor
    E_SHAREMEM_RESUME_SUBTITLE_TRACK_ID,        // HK -> Co-processor
    E_SHAREMEM_RESUME_PROGRAM_IDX,              // HK -> Co-processor
    E_SHAREMEM_SYSTEM_CLOCK_REFERENCE,
    E_SHAREMEM_CURRENT_TIME,
    E_SHAREMEM_TOTAL_TIME,
    E_SHAREMEM_TOTAL_TIME_ISVALID,
    E_SHAREMEM_WDT_TIMER,
    E_SHAREMEM_FBL_FORCE_PROGRESS,              // HK -> Co-processor, Force progress when FBL mode.
    E_SHAREMEM_AUDIO_REQUEST_SIZE,
    E_SHAREMEM_CUSTOMER_INFO,                   // HK -> Co-processor
    E_SHAREMEM_CUSTOMER_HASH        = E_SHAREMEM_CUSTOMER_INFO + E_SHAREMEM_CUSTOMER_INFO_SIZE, // HK -> Co-processor
    E_SHAREMEM_NETWORKSTREAM_MODE   = E_SHAREMEM_CUSTOMER_HASH + E_SHAREMEM_CUSTOMER_HASH_SIZE, // HK -> Co-processor
    E_SHAREMEM_ENABLE_AUDIO_MOVE,
    E_SHAREMEM_FRAMEINFO_LUMA_ADDR,
    E_SHAREMEM_FRAMEINFO_CHROM_ADDR,
    E_SHAREMEM_FRAMEINFO_WIDTH,
    E_SHAREMEM_FRAMEINFO_HEIGHT,
    E_SHAREMEM_FRAMEINFO_PITCH,
    E_SHAREMEM_ENABLE_DBGINFO,                  // HK -> Co-processor
    E_SHAREMEM_ENABLE_DYNSCALING,               // HK <-> Co-processor, Co-p will set to disable if VDEC fw don't support DS for some specific codec
    E_SHAREMEM_DYNSCALING_WIDTH,                // HK -> Co-processor
    E_SHAREMEM_DYNSCALING_HEIGHT,               // HK -> Co-processor
    E_SHAREMEM_DYNSCALING_MIUSEL,
    E_SHAREMEM_DYNSCALING_ADDRESS,
    E_SHAREMEM_DYNSCALING_DEPTH,
    E_SHAREMEM_DISABLE_AUDIO,
    E_SHAREMEM_HK_AVI_SCAN,
    E_SHAREMEM_BUFFER_INFO,
    E_SHAREMEM_BLOCK_FILEIO_TIMEOUT = E_SHAREMEM_BUFFER_INFO + E_SHAREMEM_BUFFER_INFO_SIZE,     // HK -> Co-processor
    E_SHAREMEM_NBLOCK_READ_TIMEOUT,             // HK -> Co-processor
    E_SHAREMEM_NBLOCK_SEEK_TIMEOUT,             // HK -> Co-processor
    E_SHAREMEM_DECOCE_CTRL_TIMEOUT,             // HK -> Co-processor
    E_SHAREMEM_ESBUF_CTRL_TIMEOUT,              // HK -> Co-processor
    E_SHAREMEM_VIDEO_FRC_MODE,
    E_SHAREMEM_COLOUR_PRIMARIES,
    E_SHAREMEM_FRAME_COUNT,
    E_SHAREMEM_SETBALANCEBW,                    // HK -> Co-processor: ((u8QuarPixelTH << 24) | (u8DeBlockingTH << 16) | (u8UpperBound << 8) | u8SafeCoef)
    E_SHAREMEM_SUPPORT_DTS_DECODE,              // HK -> Co-processor
    E_SHAREMEM_JUMP_THRESHOLD_LOW_LO,           // HK -> Co-processor
    E_SHAREMEM_JUMP_THRESHOLD_LOW_HI,           // HK -> Co-processor
    E_SHAREMEM_JUMP_THRESHOLD_HIGH_LO,          // HK -> Co-processor
    E_SHAREMEM_JUMP_THRESHOLD_HIGH_HI,          // HK -> Co-processor
    E_SHAREMEM_DISABLE_AUDIO_ENDING_CHECK,
    E_SHAREMEM_WMA_PKT_SIZE,
    E_SHAREMEM_AUDIO_LANGUAGE,
    E_SHAREMEM_SUBTITLE_LANGUAGE    = E_SHAREMEM_AUDIO_LANGUAGE + E_SHAREMEM_TRACK_SIZE,
    E_SHAREMEM_MAX_H264_LEVEL       = E_SHAREMEM_SUBTITLE_LANGUAGE + E_SHAREMEM_TRACK_SIZE,
    E_SHAREMEM_CHROMA_FORMAT,
    E_SHAREMEM_FRC_OUTPUT_FORMAT,
    E_SHAREMEM_SUPPORTED_AUDIO,
    E_SHAREMEM_TRICK_PLAY_TRACK,
    E_SHAREMEM_FRAME_TABLE_ADDRESS,
    E_SHAREMEM_FRAME_TABLE_SIZE,
    E_SHAREMEM_SCALER_MAX_FRAMENUM,             // HK -> Co-processor
    E_SHAREMEM_SCALER_MAX_FIELDNUM,             // HK -> Co-processor
    E_SHAREMEM_PRELOAD_SIZE,                    // HK -> Co-processor
    E_SHAREMEM_VIDEO_REMAINDER,                 // Co-processor -> HK
    E_SHAREMEM_AUDIO_REMAINDER,                 // Co-processor -> HK
    E_SHAREMEM_DEMUX_REMAINDER,                 // Co-processor -> HK
    E_SHAREMEM_PCM_REMAINDER,                   // Co-processor -> HK
    E_SHAREMEM_ENABLE_TS_RADIO,                 // HK -> Co-processor
    E_SHAREMEM_ENABLE_TS_INTERNAL_SUBTITLE,     // HK -> Co-processor
    E_SHAREMEM_SET_VDEC_DBGLEVEL,
    E_SHAREMEM_SET_FRC_DROP_TYPE,
    E_SHAREMEM_DIVX_CURRENT_STATUS,             // Co-processor -> HK
    E_SHAREMEM_DIVX_TITLE_STRINGNAME_ADDR,                // HK -> Co-processor
    E_SHAREMEM_IS_3D,
    E_SHAREMEM_3D_LAYOUT,
    E_SHAREMEM_3D_CONTENT_TYPE,
    E_SHAREMEM_DEMUX_FRAME_RATE,
    E_SHAREMEM_NUM,
} EN_VDP_SHAREMEMORY;

#define ERROR_CODE_ST   0xE001
typedef enum
{
    EN_VDP_ERRORCODE_NONE = 0,
    EN_VDP_ERRORCODE_GENERAL_ERROR              = ERROR_CODE_ST,
    EN_VDP_ERRORCODE_LOGICAL_ERROR,
    //------ RESOURCE
    EN_VDP_ERRORCODE_RESOURCE_AEON_CANNOTINIT   = ERROR_CODE_ST + 0x10,     // 0x11,
    EN_VDP_ERRORCODE_RESOURCE_MEMORY_NOTENOUGH,
    EN_VDP_ERRORCODE_RESOURCE_MEMORY_OVERFLOW,
    EN_VDP_ERRORCODE_RESOURCE_MEMORY_WRONGDATA,
    EN_VDP_ERRORCODE_RESOURCE_NO_FILESIZE,
    EN_VDP_ERRORCODE_ERR_FILE_OPEN,
    EN_VDP_ERRORCODE_ERR_FILE_READ,
    //------ CONTAINER
    EN_VDP_ERRORCODE_CONTAINER_UNKNOW           = ERROR_CODE_ST + 0x20,     // 0x21
    EN_VDP_ERRORCODE_CONTAINER_NOTSUPPORT,
    EN_VDP_ERRORCODE_CONTAINER_ERROR,
    //------ TIMEOUT
    EN_VDP_ERRORCODE_FILEREAD_TIMEOUT           = ERROR_CODE_ST + 0x30,     // 0x31,
    EN_VDP_ERRORCODE_DEMUX_TIMEOUT,
    EN_VDP_ERRORCODE_DEMUX_TIMEOUT_HOUSEKEEPING,
    //------ NOT ENOUGH INFO
    EN_VDP_ERRORCODE_NOT_ENOUGH_CONTENT_DURATION= ERROR_CODE_ST + 0x40,     // 0x41,
    EN_VDP_ERRORCODE_NOT_ENOUGH_CONTENT_FRAMERATE,
    EN_VDP_ERRORCODE_NOT_ENOUGH_CONTENT_BITRATE,
    EN_VDP_ERRORCODE_NOT_ENOUGH_CONTENT_WIDTH,
    EN_VDP_ERRORCODE_NOT_ENOUGH_CONTENT_HEIGHT,
    //------ NOT SUPPORT
    EN_VDP_ERRORCODE_NOT_SUPPORT_FILE           = ERROR_CODE_ST + 0x50,     // 0x51,
    EN_VDP_ERRORCODE_NOT_SUPPORT_SCREANSIZE,
    EN_VDP_ERRORCODE_NOT_SUPPORT_FRAMERATE,
    EN_VDP_ERRORCODE_NOT_SUPPORT_CODEC_VIDEO,
    EN_VDP_ERRORCODE_NOT_SUPPORT_MORE_VIDEO,
    EN_VDP_ERRORCODE_NOT_SUPPORT_NO_VIDEO,
    EN_VDP_ERRORCODE_NOT_SUPPORT_MORE_AUDIO,
    EN_VDP_ERRORCODE_NOT_SUPPORT_NO_SUPPORTED_AUDIO,
    EN_VDP_ERRORCODE_NOT_SUPPORT_DRM,
    // ------ NOT SUPPORT ---H264
    EN_VDP_ERRORCODE_NOT_SUPPORT_H264_BASE  = ERROR_CODE_ST + 0x60,     // 0x61
    EN_VDP_ERRORCODE_NOT_SUPPORT_H264_OUT_OF_MEMORY = EN_VDP_ERRORCODE_NOT_SUPPORT_H264_BASE,
    EN_VDP_ERRORCODE_NOT_SUPPORT_H264_RES,
    EN_VDP_ERRORCODE_NOT_SUPPORT_H264_MAX   = EN_VDP_ERRORCODE_NOT_SUPPORT_H264_RES,
    // ------ NOT SUPPORT ---MPEG4
    EN_VDP_ERRORCODE_NOT_SUPPORT_MPEG4_BASE = ERROR_CODE_ST + 0x70,     // 0x71
    EN_VDP_ERRORCODE_NOT_SUPPORT_MPEG4_VOL_SHAPE = EN_VDP_ERRORCODE_NOT_SUPPORT_MPEG4_BASE,
    EN_VDP_ERRORCODE_NOT_SUPPORT_MPEG4_VOL_USED_SPRITE,
    EN_VDP_ERRORCODE_NOT_SUPPORT_MPEG4_VOL_NOT_8_BIT,
    EN_VDP_ERRORCODE_NOT_SUPPORT_MPEG4_VOL_NERPRED_ENABLE,
    EN_VDP_ERRORCODE_NOT_SUPPORT_MPEG4_VOL_REDUCED_RES_ENABLE,
    EN_VDP_ERRORCODE_NOT_SUPPORT_MPEG4_VOL_SCALABILITY,
    EN_VDP_ERRORCODE_NOT_SUPPORT_MPEG4_VOL_H263_ERROR,
    EN_VDP_ERRORCODE_NOT_SUPPORT_MPEG4_RES,
    EN_VDP_ERRORCODE_NOT_SUPPORT_MPEG4_MAX  = EN_VDP_ERRORCODE_NOT_SUPPORT_MPEG4_RES,
    // ------ NOT SUPPORT ---RM
    EN_VDP_ERRORCODE_NOT_SUPPORT_RM_BASE    = ERROR_CODE_ST + 0x80,     // 0x81
    EN_VDP_ERRORCODE_NOT_SUPPORT_RM_MULTI_RATE  = EN_VDP_ERRORCODE_NOT_SUPPORT_RM_BASE,
    EN_VDP_ERRORCODE_NOT_SUPPORT_RM_RES,
    EN_VDP_ERRORCODE_NOT_SUPPORT_RM_MAX     = EN_VDP_ERRORCODE_NOT_SUPPORT_RM_RES,
    //------ DRM
    EN_VDP_ERRORCODE_DRM_ERR_HEADER             = ERROR_CODE_ST + 0x90,     // 0x91
    EN_VDP_ERRORCODE_DRM_ERR_NOT_AUTHORIZED,
    EN_VDP_ERRORCODE_DRM_ERR_NOT_REGISTERED,
    EN_VDP_ERRORCODE_DRM_ERR_RENTAL_EXPIRED,
    EN_VDP_ERRORCODE_DRM_ERR_VERSION_EXPIRED,
    // ----- Funtion error
    EN_VDP_ERRORCODE_FUNC_ERR_DEMUX_HEADER      = ERROR_CODE_ST + 0xA0,     // 0xA1
    EN_VDP_ERRORCODE_FUNC_ERR_CHANGE_AUDIO,
    EN_VDP_ERRORCODE_FUNC_ERR_DEMUX_PACKET,
    EN_VDP_ERRORCODE_FUNC_ERR_WAIT_DISP_READY,
    EN_VDP_ERRORCODE_FUNC_ERR_SCAN_INDEX,
    EN_VDP_ERRORCODE_FUNC_ERR_AUDIO_FAR_BEHIND_VIDEO,
    EN_VDP_ERRORCODE_FUNC_ERR_BM_INIT,
    EN_VDP_ERRORCODE_FUNC_ERR_DEMUXER_INIT,
    EN_VDP_ERRORCODE_FUNC_ERR_DECODE_INIT,
    EN_VDP_ERRORCODE_FUNC_ERR_MEMORY_INIT,
    EN_VDP_ERRORCODE_FUNC_ERR_DECODE_ERROR,
    EN_VDP_ERRORCODE_FUNC_ERR_DECODE_MONITOR_ERROR,
    // ----- HW error
    EN_VDP_ERRORCODE_HW_VIDEO_INIT_FAILED       = ERROR_CODE_ST + 0xB0,     // 0xB1
    EN_VDP_ERRORCODE_MAX,
}EN_VDP_ERRORCODE;

//Co-enum between HK and AEON
typedef enum
{
    E_VDP_PLAY_MODE_NORMAL_PLAY = 0,    // Normal Play Forward, it will do avsync control.
    E_VDP_PLAY_MODE_PAUSE,              // Pause Play
    E_VDP_PLAY_MODE_SLOW_FORWARD,       // Slow Motion Forward without audio sound.
    E_VDP_PLAY_MODE_FAST_FORWARD,       // Fast Motion Forward without audio sound.
    E_VDP_PLAY_MODE_SCAN_BACKWARD,      // Scan Play Backward without audio sound.

    E_VDP_PLAY_MODE_SCAN_FORWARD,       // Scan Play Forward without audio sound.
    E_VDP_PLAY_MODE_STEP_FORWARD,       // Step Play Forward without audio sound.
    E_VDP_PLAY_MODE_STOP,
    E_VDP_PLAY_MODE_NUM,
} EN_VDP_PLAY_MODE;

typedef enum
{
    E_VDP_PLAY_SPEED_X1 = 1,
    E_VDP_PLAY_SPEED_X2,
    E_VDP_PLAY_SPEED_X4,
    E_VDP_PLAY_SPEED_X8,
    E_VDP_PLAY_SPEED_X16,
    E_VDP_PLAY_SPEED_X32,
    E_VDP_PLAY_SPEED_MAX
} EN_VDP_PLAY_SPEED;

typedef enum
{
    E_VDP_CODEC_ID_NONE             = 0,

    // Attribute for Video
    E_VDP_CODEC_MPG_VIDEO           = 0x0040,
    E_VDP_CODEC_RM_VIDEO            = 0x0080,
    E_VDP_CODEC_MPEG4               = 0x0100,
    E_VDP_CODEC_H264                = 0x0200,
    E_VDP_CODEC_TS                  = 0x0400,
    E_VDP_CODEC_JPEG                = 0x0800,
    E_VDP_CODEC_FLV                 = 0x1000,
    E_VDP_CODEC_VC1                 = 0x2000,
    E_VDP_CODEC_FOURCCEX            = 0x4000,

    // Attribute for Audio
    E_VDP_CODEC_MPG_AUDIO           = 0x0040,
    E_VDP_CODEC_RM_AUDIO            = 0x0080,
    E_VDP_CODEC_AC3                 = 0x0100,
    E_VDP_CODEC_AAC                 = 0x0200,
    E_VDP_CODEC_PCM                 = 0x0400,
    E_VDP_CODEC_ADPCM               = 0x0800,
    E_VDP_CODEC_WMA                 = 0x1000,
    E_VDP_CODEC_DTS                 = 0x2000,
    E_VDP_CODEC_FLAC                = 0x4000,
    E_VDP_CODEC_VORBIS              = 0x8000,
    E_VDP_CODEC_WAVEFORMATEX        = 0x10000,

    // Attribute for Subtitle
    E_VDP_CODEC_TEXT                = 0x0040,
    E_VDP_CODEC_BMP                 = 0x0080,
    //E_VDP_CODEC_HTML                = 0x0100,
    //E_VDP_CODEC_XML                 = 0x0200,

    E_VDP_CODEC_ATTRIBUTE_MASK      = 0x80FFFFC0,

    E_VDP_CODEC_ID_SUPPORT_VIDEO    = 0x1000000,

    E_VDP_CODEC_ID_MPEG1VIDEO       = E_VDP_CODEC_ID_SUPPORT_VIDEO + E_VDP_CODEC_MPG_VIDEO,
    E_VDP_CODEC_ID_MPEG2VIDEO,

    E_VDP_CODEC_ID_MPEG4            = E_VDP_CODEC_ID_SUPPORT_VIDEO + E_VDP_CODEC_MPEG4,
    E_VDP_CODEC_ID_H263,
    E_VDP_CODEC_ID_DIVX3,
    E_VDP_CODEC_ID_DIVX4,
    E_VDP_CODEC_ID_DIVX,

    E_VDP_CODEC_ID_H264             = E_VDP_CODEC_ID_SUPPORT_VIDEO + E_VDP_CODEC_H264,
    E_VDP_CODEC_ID_AVS,

    E_VDP_CODEC_ID_RV30             = E_VDP_CODEC_ID_SUPPORT_VIDEO + E_VDP_CODEC_RM_VIDEO,
    E_VDP_CODEC_ID_RV40,

    /* TS */
    E_VDP_CODEC_ID_MPEGTS           = E_VDP_CODEC_ID_SUPPORT_VIDEO + E_VDP_CODEC_TS + E_VDP_CODEC_MPG_VIDEO,
    E_VDP_CODEC_ID_MPEG4TS          = E_VDP_CODEC_ID_SUPPORT_VIDEO + E_VDP_CODEC_TS + E_VDP_CODEC_MPEG4,
    E_VDP_CODEC_ID_H264TS           = E_VDP_CODEC_ID_SUPPORT_VIDEO + E_VDP_CODEC_TS + E_VDP_CODEC_H264,
    E_VDP_CODEC_ID_AVSTS,
    E_VDP_CODEC_ID_VC1TS            = E_VDP_CODEC_ID_SUPPORT_VIDEO + E_VDP_CODEC_TS + E_VDP_CODEC_VC1,

    E_VDP_CODEC_ID_MJPEG            = E_VDP_CODEC_ID_SUPPORT_VIDEO + E_VDP_CODEC_JPEG,

    E_VDP_CODEC_ID_FLV1             = E_VDP_CODEC_ID_SUPPORT_VIDEO + E_VDP_CODEC_FLV,

    E_VDP_CODEC_ID_VC1              = E_VDP_CODEC_ID_SUPPORT_VIDEO + E_VDP_CODEC_VC1,
    E_VDP_CODEC_ID_WMV3,

    E_VDP_CODEC_ID_FOURCCEX         = E_VDP_CODEC_ID_SUPPORT_VIDEO + E_VDP_CODEC_FOURCCEX,

    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    E_VDP_CODEC_ID_SUPPORT_AUDIO    = 0x2000000,
    E_VDP_CODEC_ID_MP2              = E_VDP_CODEC_ID_SUPPORT_AUDIO + E_VDP_CODEC_MPG_AUDIO,
    E_VDP_CODEC_ID_MP3,

    /* RM */
    E_VDP_CODEC_ID_COOK             = E_VDP_CODEC_ID_SUPPORT_AUDIO + E_VDP_CODEC_RM_AUDIO,
    E_VDP_CODEC_ID_RAAC,
    E_VDP_CODEC_ID_RACP,

    E_VDP_CODEC_ID_AC3              = E_VDP_CODEC_ID_SUPPORT_AUDIO + E_VDP_CODEC_AC3,
    E_VDP_CODEC_ID_EAC3,

    E_VDP_CODEC_ID_AAC              = E_VDP_CODEC_ID_SUPPORT_AUDIO + E_VDP_CODEC_AAC,
    E_VDP_CODEC_ID_MPEG4AAC,

    /* various pcm "codecs" */
    E_VDP_CODEC_ID_PCM_S16LE        = E_VDP_CODEC_ID_SUPPORT_AUDIO + E_VDP_CODEC_PCM,
    E_VDP_CODEC_ID_PCM_S16BE,
    E_VDP_CODEC_ID_PCM_U16LE,
    E_VDP_CODEC_ID_PCM_U16BE,
    E_VDP_CODEC_ID_PCM_S8,
    E_VDP_CODEC_ID_PCM_U8,
    E_VDP_CODEC_ID_PCM_MULAW,
    E_VDP_CODEC_ID_PCM_ALAW,

    /* various adpcm codecs */
    E_VDP_CODEC_ID_ADPCM            = E_VDP_CODEC_ID_SUPPORT_AUDIO + E_VDP_CODEC_ADPCM,
    E_VDP_CODEC_ID_ADPCM_IMA_QT,
    E_VDP_CODEC_ID_ADPCM_IMA_WAV,
    E_VDP_CODEC_ID_ADPCM_IMA_DK3,
    E_VDP_CODEC_ID_ADPCM_IMA_DK4,
    E_VDP_CODEC_ID_ADPCM_MS,
    E_VDP_CODEC_ID_ADPCM_G726,

    E_VDP_CODEC_ID_WMAV1            = E_VDP_CODEC_ID_SUPPORT_AUDIO + E_VDP_CODEC_WMA,
    E_VDP_CODEC_ID_WMAV2,
    E_VDP_CODEC_ID_WMAV3,

    E_VDP_CODEC_ID_DTS              = E_VDP_CODEC_ID_SUPPORT_AUDIO + E_VDP_CODEC_DTS,
    E_VDP_CODEC_ID_DTS_HD,
    E_VDP_CODEC_ID_DTS_EXPRESS,

    E_VDP_CODEC_ID_FLAC             = E_VDP_CODEC_ID_SUPPORT_AUDIO + E_VDP_CODEC_FLAC,

    E_VDP_CODEC_ID_VORBIS           = E_VDP_CODEC_ID_SUPPORT_AUDIO + E_VDP_CODEC_VORBIS,

    E_VDP_CODEC_ID_WAVEFORMATEX     = E_VDP_CODEC_ID_SUPPORT_AUDIO + E_VDP_CODEC_WAVEFORMATEX,

    /* Subtitle */
    E_VDP_CODEC_ID_SUPPORT_SUBTITLE     = 0x4000000,
    E_VDP_CODEC_ID_SUBTITLE_TEXT_SRT    = E_VDP_CODEC_ID_SUPPORT_SUBTITLE + E_VDP_CODEC_TEXT,
    E_VDP_CODEC_ID_SUBTITLE_TEXT_UTF8,
    E_VDP_CODEC_ID_SUBTITLE_TEXT_ASS,
    E_VDP_CODEC_ID_SUBTITLE_TEXT_SSA,
    E_VDP_CODEC_ID_SUBTITLE_TEXT_3GPP,
    E_VDP_CODEC_ID_SUBTITLE_TEXT_USF,
    E_VDP_CODEC_ID_SUBTITLE_IMAGE_BMP   = E_VDP_CODEC_ID_SUPPORT_SUBTITLE + E_VDP_CODEC_BMP,
    E_VDP_CODEC_ID_SUBTITLE_IMAGE_VOBSUB,
    E_VDP_CODEC_ID_SUBTITLE_IMAGE_BMP_TS,
    E_VDP_CODEC_ID_SUBTITLE_IMAGE_PGS_TS,

    E_VDP_CODEC_ID_NON_SUPPORT      = 0x80000000,
    /* Video */
    E_VDP_CODEC_ID_MPEG2VIDEO_XVMC,
    E_VDP_CODEC_ID_H261,
    E_VDP_CODEC_ID_RV10,
    E_VDP_CODEC_ID_RV20,
    E_VDP_CODEC_ID_MJPEGB,
    E_VDP_CODEC_ID_LJPEG,
    E_VDP_CODEC_ID_RAWVIDEO,
    E_VDP_CODEC_ID_MSVIDEO1,
    E_VDP_CODEC_ID_MSMPEG4V1,
    E_VDP_CODEC_ID_MSMPEG4V2,
    E_VDP_CODEC_ID_MSMPEG4V3,
    E_VDP_CODEC_ID_WMV1,
    E_VDP_CODEC_ID_WMV2,
    E_VDP_CODEC_ID_H263I,
    E_VDP_CODEC_ID_H263P,
    E_VDP_CODEC_ID_SVQ1,
    E_VDP_CODEC_ID_SVQ3,
    E_VDP_CODEC_ID_DVVIDEO,
    E_VDP_CODEC_ID_SP5X,
    E_VDP_CODEC_ID_HUFFYUV,
    E_VDP_CODEC_ID_CYUV,
    E_VDP_CODEC_ID_INDEO3,
    E_VDP_CODEC_ID_VP3,
    E_VDP_CODEC_ID_THEORA,
    E_VDP_CODEC_ID_ASV1,
    E_VDP_CODEC_ID_ASV2,
    E_VDP_CODEC_ID_FFV1,
    E_VDP_CODEC_ID_4XM,
    E_VDP_CODEC_ID_XAN_WC3,
    E_VDP_CODEC_ID_XAN_WC4,
    E_VDP_CODEC_ID_VCR1,
    E_VDP_CODEC_ID_CLJR,
    E_VDP_CODEC_ID_MDEC,
    E_VDP_CODEC_ID_ROQ,
    E_VDP_CODEC_ID_INTERPLAY_VIDEO,
    E_VDP_CODEC_ID_RPZA,
    E_VDP_CODEC_ID_CINEPAK,
    E_VDP_CODEC_ID_WS_VQA,
    E_VDP_CODEC_ID_MSRLE,
    E_VDP_CODEC_ID_IDCIN,
    E_VDP_CODEC_ID_VMDVIDEO,
    E_VDP_CODEC_ID_SMC,
    E_VDP_CODEC_ID_FLIC,
    E_VDP_CODEC_ID_TRUEMOTION1,
    E_VDP_CODEC_ID_MSZH,
    E_VDP_CODEC_ID_ZLIB,
    E_VDP_CODEC_ID_QTRLE,
    E_VDP_CODEC_ID_8BPS,

    /* Audio */
    E_VDP_CODEC_ID_PCM_S32LE,
    E_VDP_CODEC_ID_PCM_S32BE,
    E_VDP_CODEC_ID_PCM_U32LE,
    E_VDP_CODEC_ID_PCM_U32BE,
    E_VDP_CODEC_ID_PCM_S24LE,
    E_VDP_CODEC_ID_PCM_S24BE,
    E_VDP_CODEC_ID_PCM_S24DAUD,
    E_VDP_CODEC_ID_PCM_U24LE,
    E_VDP_CODEC_ID_PCM_U24BE,

    E_VDP_CODEC_ID_ADPCM_IMA_SMJPEG,
    E_VDP_CODEC_ID_ADPCM_IMA_WS,
    E_VDP_CODEC_ID_ADPCM_4XM,
    E_VDP_CODEC_ID_ADPCM_XA,
    E_VDP_CODEC_ID_ADPCM_ADX,
    E_VDP_CODEC_ID_ADPCM_EA,
    E_VDP_CODEC_ID_ADPCM_CT,
    E_VDP_CODEC_ID_ADPCM_YAMAHA,
    E_VDP_CODEC_ID_ADPCM_SWF,
    E_VDP_CODEC_ID_ADPCM_SBPRO_2,
    E_VDP_CODEC_ID_ADPCM_SBPRO_3,
    E_VDP_CODEC_ID_ADPCM_SBPRO_4,

    /* AMR */
    E_VDP_CODEC_ID_AMR_NB,
    E_VDP_CODEC_ID_AMR_WB,

//    E_VDP_CODEC_ID_VORBIS,
    E_VDP_CODEC_ID_DVAUDIO,
    E_VDP_CODEC_ID_MACE3,
    E_VDP_CODEC_ID_MACE6,
    E_VDP_CODEC_ID_VMDAUDIO,
    E_VDP_CODEC_ID_ROQ_DPCM,
    E_VDP_CODEC_ID_INTERPLAY_DPCM,
    E_VDP_CODEC_ID_XAN_DPCM,
//    E_VDP_CODEC_ID_FLAC,
    E_VDP_CODEC_ID_SDDS,

    E_VDP_CODEC_ID_RA_144,
    E_VDP_CODEC_ID_RA_288,
    E_VDP_CODEC_ID_SIPR,
    E_VDP_CODEC_ID_ATRC,

    /* subtitle */
    E_VDP_CODEC_ID_SUBTITLE_TEXT_RT,
    E_VDP_CODEC_ID_SUBTITLE_TEXT_SMI,
    E_VDP_CODEC_ID_SUBTITLE_TEXT_KATE,

} EN_VDP_CODECID;

//-------------------------------
// E_VS_MBX_CLASS_VDPLAYER
//-------------------------------
// HK --> MM Command Index
#define MB_VDPLAYER_INIT                            0x01
// #define MB_VDPLAYER_MEM_INIT                        0x02
// #define MB_VDPLAYER_STATE_INIT                      0x03
#define MB_VDPLAYER_RESET                           0x04
// #define MB_VDPLAYER_SET_PREVIEW_MODE                0x05
// #define MB_VDPLAYER_SET_BUFF_SIZE                   0x06

#define MB_VDPLAYER_PLAY                            0x10
#define MB_VDPLAYER_STOP                            0x11
#define MB_VDPLAYER_PAUSE                           0x12
#define MB_VDPLAYER_RESUME                          0x13
// #define MB_VDPLAYER_FF                              0x14
// #define MB_VDPLAYER_REW                             0x15
#define MB_VDPLAYER_SET_TRICK_MODE                  0x16
#define MB_VDPLAYER_GET_TRICK_MODE                  0x17
#define MB_VDPLAYER_REPEAT_AB                       0x18
#define MB_VDPLAYER_STEP                            0x19
#define MB_VDPLAYER_DMX_RESET                       0x1A

#define MB_VDPLAYER_SET_SUBTITLE_TRACKID            0x21
// #define MB_VDPLAYER_GET_SUBTITLE_STATE              0x22
// #define MB_VDPLAYER_GET_VDPLAYER_INFO               0x23
#define MB_VDPLAYER_SET_PLAYER_POSITION             0x26

#define MB_VDPLAYER_ACK_HK_TO_COPROCESSOR           0x30
// #define MB_VDPLAYER_HEAERPARSER_GETINFO             0x31

#define MB_VDPLAYER_SET_AUDIO_TRACK_ID              0x43
#define MB_VDPLAYER_SPUQUEUE_GET_DRAM_ADDR          0x44
#define MB_VDPLAYER_SPUQUEUE_PUSH                   0x45
// #define MB_VDPLAYER_MPEG_SUBTITLE_MAIN              0x46
#define MB_VDPLAYER_MPEG_SET_SUBTITLE_SHOW          0x47
#define MB_VDPLAYER_MPEG_SUBTITLE_INIT_DEINIT       0x48 // Init: 0 deinit:1
#define MB_VDPLAYER_MPEG_SUBTITLE_RESET             0x49
#define MB_VDPLAYER_MPEG_SUBTITLE_SETHEADERINFO     0x4A
#define MB_VDPLAYER_MPEG_SUBTITLE_GETQUEUEINFO      0x4B
#define MB_VDPLAYER_MPEG_SUBTITLE_FILEPROCESS       0x4C
#define MB_VDPLAYER_SUBTITLE_TIMESHIFT              0x4D

// DRM
#define MB_VDPLAYER_GET_DRM_REG_CODE                0x50
#define MB_VDPLAYER_CHECK_DRM_AUTHORIZATION         0x51
#define MB_VDPLAYER_QUERY_DRM_RENTAL_STATUS         0x52
#define MB_VDPLAYER_GET_DRM_REG_CODE_2              0x53
#define MB_VDPLAYER_GET_DRM_DEACTIVATION_CODE       0x54

#define MB_VDPLAYER_TS_SET_PROGRAM                  0x60
#define MB_VDPLAYER_SET_TITLE_ID                    0x61
#define MB_VDPLAYER_SET_EDITION_ID                  0x62
#define MB_VDPLAYER_SET_CHAPTER_ID                  0x63
#define MB_VDPLAYER_SET_AUTOGEN_CHAPTER_ID          0x64
#define MB_VDPLAYER_AVI_SCAN_INDEX                  0x65
#define MB_VDPLAYER_DIVX_PLUS_RESET                 0x66
#define MB_VDPLAYER_DEMUX_HEADER                    0x67

#define MB_VDPLAYER_SETDYNSCALINGPARAMS             0x70
#define MB_VDPLAYER_HVD_FEATURE                     0x71
#define MB_VDPLAYER_CMD_HK2VDEC                     0x72

// MM -> HK Command Index
#define MB_VDPLAYER_FILE_READ                       0x80
#define MB_VDPLAYER_FILE_SEEK                       0x81
#define MB_VDPLAYER_FILE_OPEN                       0x82
#define MB_VDPLAYER_FILE_CLOSE                      0x83
#define MB_VDPLAYER_VIDEO_INIT                      0x84
#define MB_VDPLAYER_AUDIO_INIT                      0x85
#define MB_VDPLAYER_SETUP_DISPLAY                   0x86
#define MB_VDPLAYER_CLOSE_DISPLAY                   0x87
#define MB_VDPLAYER_AUDIO_START                     0x88
#define MB_VDPLAYER_FILE_READ_NONBLOCKING           0x89
#define MB_VDPLAYER_FILE_DUMP_RAW                   0x8A
// #define MB_VDPLAYER_GET_DISPLAY_INFO             0x8B
#define MB_VDPLAYER_SET_DISPLAY_INFO                0x8C
#define MB_VDPLAYER_FILE_SEEK_NONBLOCKING           0x8D
#define MB_VDPLAYER_AUDIO_UNSUPPORT                 0x8E
#define MB_VDPLAYER_Set_MVOP_OneField               0x8F
#define MB_VDPLAYER_SET_REPLAYBACK                  0x90
#define MB_VDPLAYER_LOAD_VDECBIN                    0x91

#define MB_VDPLAYER_ACK_COPROCESSOR_TO_HK           0xA0
// #define MB_VDPLAYER_STATE_RET                       0xA1
#define MB_VDPLAYER_STATE_RET_WAIT                  0xA2
#define MB_VDPLAYER_STATE_RET_EXIT                  0xA3
#define MB_VDPLAYER_FILE_READ_NONBLOCKING_DONE      0xA4
#define MB_VDPLAYER_FILE_SEEK_NONBLOCKING_DONE      0xA5
#define MB_VDPLAYER_FORCE_FILE_END                  0xA6

// DVD MM Function
#define MB_VDPLAYER_DVD_INIT                        0xB0
#define MB_VDPLAYER_DVD_PREDECODE                   0xB1
#define MB_VDPLAYER_DVD_SEEKPLAY                    0xB2
#define MB_VDPLAYER_DVD_WAIT                        0xB3
#define MB_VDPLAYER_DVD_DISCONTINUITY               0xB4
#define MB_VDPLAYER_DVD_WAITDONE                    0xB5
#define MB_VDPLAYER_DVD_SET_TRICK_MODE              0xB6
#define MB_VDPLAYER_DVD_PLAY                        0xB7
#define MB_VDPLAYER_DVD_SCANSYNC                    0xB8
#define MB_VDPLAYER_DVD_SCANDONE                    0xB9
#define MB_VDPLAYER_DVD_SCANEXIT                    0xBA

#define MB_VDPLAYER_TEST                            0xE0
#define MB_VDPLAYER_ACK_TEST                        0xE1
#define MB_VDPLAYER_DBGINFO                         0xE2

typedef enum
{
    E_VDP_VIDEO_UNKNOW = -1,
    E_VDP_VIDEO_MPEG4,
    E_VDP_VIDEO_MJPEG,
    E_VDP_VIDEO_H264,
    E_VDP_VIDEO_RM,
    E_VDP_VIDEO_TS,
    E_VDP_VIDEO_MPG,
    E_VDP_VIDEO_VC1,
    E_VDP_VIDEO_AVS,
    E_VDP_VIDEO_FLV,
} EN_VDP_VIDEO_TYPE;

typedef enum
{
    E_SUBTITLE_INIT_COMMAND_INIT    = 0,
    E_SUBTITLE_INIT_COMMAND_DEINIT,
    E_SUBTITLE_INIT_COMMAND_INIT_FINISH,
} EN_SUBTITLE_INIT_COMMAND;

typedef enum
{
    E_VDP_PREVIEW_NONE              = 0,
    E_VDP_PREVIEW_AUDIO             = 0x1,
    E_VDP_PREVIEW_VIDEO             = 0x2,
    E_VDP_PREVIEW_INFO_DURATION     = 0x4,
    E_VDP_PREVIEW_INFO_RESOLUTION   = 0x8,
    E_VDP_PREVIEW_INFO_INDEXTABLE   = 0x10,
    E_VDP_PREVIEW_THUMBNAIL         = 0x20,
    E_VDP_PREVIEW_SUBTITLE          = 0x40,
} EN_VDP_PREVIDE_MODE;

typedef enum
{
    E_VDP_AUDIO_REQUIRE_DEFAULT     = 0,
    E_VDP_AUDIO_REQUIRE_SIZE_1KB,
    E_VDP_AUDIO_REQUIRE_SIZE_2KB,
    E_VDP_AUDIO_REQUIRE_SIZE_4KB,
    E_VDP_AUDIO_REQUIRE_SIZE_8KB,
    E_VDP_AUDIO_REQUIRE_SIZE_16KB,
    E_VDP_AUDIO_REQUIRE_SIZE_32KB,
} EN_VDP_AUDIO_REQUIRE_SIZE;

typedef enum
{
    E_VDP_HVD_DISABLE_DEBLOCKING    = 0,
    E_VDP_HVD_DISABLE_QUARTERPIXEL,
} EN_VDP_HVD_FEATURE;


typedef enum
{
    E_H264_LEVEL_ALL    = 0,
    E_H264_LEVEL_10     = 10,
    E_H264_LEVEL_11     = 11,
    E_H264_LEVEL_12     = 12,
    E_H264_LEVEL_13     = 13,
    E_H264_LEVEL_20     = 20,
    E_H264_LEVEL_21     = 21,
    E_H264_LEVEL_22     = 22,
    E_H264_LEVEL_30     = 30,
    E_H264_LEVEL_31     = 31,
    E_H264_LEVEL_32     = 32,
    E_H264_LEVEL_40     = 40,
    E_H264_LEVEL_41     = 41,
    E_H264_LEVEL_42     = 42,
    E_H264_LEVEL_50     = 50,
    E_H264_LEVEL_51     = 51,
} EN_VDP_H264_LEVEL;

typedef enum
{
    E_FRC_INTERLACE     = 0x1000,       // Internal used, Flag Only
    E_FRC_DISABLE       = 0,
    E_FRC_FORMAT_24P    = 24,
    E_FRC_FORMAT_25P    = 25,
    E_FRC_FORMAT_30P    = 30,
    E_FRC_FORMAT_50I    = 50 + E_FRC_INTERLACE,
    E_FRC_FORMAT_50P    = 50,
    E_FRC_FORMAT_60I    = 60 + E_FRC_INTERLACE,
    E_FRC_FORMAT_60P    = 60,
} EN_VDP_FRC_OUTPUT_FORMAT;

typedef enum
{
    E_VDP_AUDIO_NO_SUPPORT_MPG     = E_VDP_CODEC_MPG_AUDIO,
    E_VDP_AUDIO_NO_SUPPORT_RM     = E_VDP_CODEC_RM_AUDIO,
    E_VDP_AUDIO_NO_SUPPORT_AC3     = E_VDP_CODEC_AC3,
    E_VDP_AUDIO_NO_SUPPORT_EAC3     = 0x0001,
    E_VDP_AUDIO_NO_SUPPORT_AAC     = E_VDP_CODEC_AAC,
    E_VDP_AUDIO_NO_SUPPORT_PCM     = E_VDP_CODEC_PCM,
    E_VDP_AUDIO_NO_SUPPORT_ADPCM     = E_VDP_CODEC_ADPCM,
    E_VDP_AUDIO_NO_SUPPORT_WMA     = E_VDP_CODEC_WMA,
    E_VDP_AUDIO_NO_SUPPORT_DTS     = E_VDP_CODEC_DTS,
    E_VDP_AUDIO_NO_SUPPORT_FLAC     = E_VDP_CODEC_FLAC,
    E_VDP_AUDIO_NO_SUPPORT_VORBIS     = E_VDP_CODEC_VORBIS,

    // set '0' as supported, '1' as not supported
    // for sharememory default '0' to be "support all"
    E_VDP_AUDIO_SUPPORT_NONE     = 0xFFFFFFFF,
    E_VDP_AUDIO_SUPPORT_ALL     = 0x00000000,
} EN_VDP_AUDIO_SUPPORT;


typedef enum
{
    E_VDP_NOT_3D   =  0,
    E_VDP_SIDE_BY_SIDE_RF,        // side-by-side, right image first
    E_VDP_SIDE_BY_SIDE_LF,        // side-by-side, left image first
    E_VDP_OVER_UNDER_RT,          // over/under, right image top
    E_VDP_OVER_UNDER_LT,          // over/under, left image top
    E_VDP_DUAL_STREAM,            // separate left/right streams
    E_VDP_MULTI_STREAM,           // more than two streams
    E_VDP_VERTICAL_LINE_INTERLEAVED_TYPE_ODD_LINE,
    E_VDP_VERTICAL_LINE_INTERLEAVED_TYPE_EVEN_LINE,
    E_VDP_FRAME_SEQUENTIAL_TYPE_ODD_FRAME,
    E_VDP_FRAME_SEQUENTIAL_TYPE_EVEN_FRAME,
    E_VDP_LEFT_RIGHT_VIEW_SEQUENCE_TYPE_PRIMARY_VIEW,
    E_VDP_LEFT_RIGHT_VIEW_SEQUENCE_TYPE_SECONDARY_VIEW,
    E_VDP_TOP_BOTTOM_TYPE_TOP_FIRST,
    E_VDP_TOP_BOTTOM_TYPE_BOTTOM_FIRST,
} EN_VDP_3D_LAYOUT;

typedef enum
{
    E_VDP_STEREOSCOPIC    = 1,
    E_VDP_STEREO_MONOSCOPIC_MIXED,
} EN_VDP_3D_CONTENT_TYPE;



typedef enum
{
    E_BUFFER_INFO_LEVEL = 0,
    E_BUFFER_INFO_BITRATE = 1,
    E_BUFFER_INFO_DURATION = 2,
    E_BUFFER_INFO_FLAG = 3,
    E_BUFFER_INFO_LAST_PTS_LEN = 4,
    E_BUFFER_INFO_INDEX_OFFSET = 5,
    E_BUFFER_INFO_SUBTITLE = 6,
    E_BUFFER_INFO_PERCENT = E_SHAREMEM_BUFFER_INFO_SIZE-1,
} EN_BUFFER_INFO_INDEX;

#endif //#ifndef MAPP_VDPLAYER_SHAREMENCFG_H
