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
#ifndef _MSAPI_PVR_H_
#define _MSAPI_PVR_H_

#ifdef __cplusplus
extern "C" {
#endif

#define WARB 1
#define APIPVR_MAX_PROGRAM_NUM 5
#define APIPVR_MAX_PLAY_NUM 2
#define FILE_PATH_SIZE 256
#define APIPVR_MAX_SUBTITLEINFO_NUM    12
#define APIPVR_MAX_TTXINFO_NUM         12
#define APIPVR_MAX_AUDIOINFO_NUM       16
#define APIPVR_INVALID_HANDLER 0xFF
#define APIPVR_MAX_PID_NUM (16)

typedef struct
{
    /// save start time
    MS_U32 u32StartTime;
} APIPVR_INFO;


///define pa2va prototype
typedef void* (*APIPVR_MEMALLOC_FUNC)(MS_U32);
typedef MS_BOOL (*APIPVR_MEMFREE_FUNC)(void *);
typedef MS_U32 (*APIPVR_MEMPA2VA_FUNC)(MS_U32);
typedef void (*APIPVR_MEMFLUSH_FUNC)(MS_U32 ,MS_U32);
typedef MS_BOOL (*APIPVR_KLADDER_FUNC)(void);
typedef MS_BOOL (*APIPVR_UPDATEINFO_FUNC)(MS_U32);
typedef MS_BOOL (*APIPVR_DSCMB_FUNC)(MS_U8,MS_U32);
typedef MS_BOOL (*APIPVR_DSCMBCLOSE_FUNC)(MS_U8, MS_U32);
typedef MS_BOOL (*APIPVR_TRICKSET_FUNC)(MS_U32, void *);
typedef MS_BOOL (*APIPVR_DMAENCRYPT_FUNC)(MS_BOOL, MS_U32, MS_U32, MS_U32, MS_U32, void *);


//need to sync mapi_pvr.h pidType Enum value
typedef enum
{
    DEMOPVR_PCR_PID = 0,
    DEMOPVR_AUDIO_PID
}DemoPVR_PidType;

//need to sync mapi_pvr.h EN_PVR_TRICK_TYPE Enum value
typedef enum
{
   EN_DEMOPVR_TRICK_TYPE_FORCEMOTION=0,
   EN_DEMOPVR_TRICK_TYPE_PIDSWITCH
}EN_DEMOPVR_TRICK_TYPE;


typedef enum{
    APIPVR_LOG_LEVEL_ERROR=0,
    APIPVR_LOG_LEVEL_DEBUG=1,
    APIPVR_LOG_LEVEL_TRACE=2,
    APIPVR_LOG_LEVEL_MESSAGE=3
}APIPVR_LOG_LEVEL;


typedef enum
{
    EN_APIPVR_PLAYBACK_TYPE_BASICPLAYER=0,
    EN_APIPVR_PLAYBACK_TYPE_TSPLAYER,
    EN_APIPVR_PLAYBACK_TYPE_ESPLAYER,
    EN_APIPVR_PLAYBACK_TYPE_MAX
} APIPVR_PLAYBACK_TYPE;

typedef enum
{
    EN_APIPVR_RECORD_TYPE_SINGLE=0,
    EN_APIPVR_RECORD_TYPE_DUAL,
    EN_APIPVR_RECORD_TYPE_SWPVR,
    EN_APIPVR_RECORD_TYPE_MAX
} APIPVR_RECORD_TYPE;

typedef enum
{
    EN_APIPVR_SERVICETYPE_ATV              = 0x00,      ///< Service type ATV
    EN_APIPVR_SERVICETYPE_DTV              = 0x01,      ///< Service type DTV
    EN_APIPVR_SERVICETYPE_RADIO            = 0x02,      ///< Service type Radio
    EN_APIPVR_SERVICETYPE_DATA             = 0x03,        ///< Service type Data
    EN_APIPVR_SERVICETYPE_UNITED_TV        = 0x04,      ///< Service type United TV
    EN_APIPVR_SERVICETYPE_INVALID          = 0x05,      ///< Service type INVALID
} APIPVR_MEMBER_SERVICETYPE;

typedef enum
{
   EN_APIPVR_SINGLE_TUNER = 1,
   EN_APIPVR_DUAL_TUNER = 2,
   EN_APIPVR_TRIPLE_TUNER = 3,
   EN_APIPVR_FOUR_TUNER = 4,
} APIPVR_TUNER_NUM;

typedef enum
{
    APIPVR_FILE_LINEAR = 0,
    APIPVR_FILE_CIRCULAR = 1
} APIPVR_FILE_FORMAT;

typedef enum
{
    EN_APIPVR_ENCRYPTION_NONE    = 0,
    EN_APIPVR_ENCRYPTION_DEFAULT = 1,//default-defination  key
    EN_APIPVR_ENCRYPTION_CIPLUS  = 2,//not support for now
    EN_APIPVR_ENCRYPTION_USER    = 3,//user-defination key
    EN_APIPVR_ENCRYPTION_SMARTCARD  = 4,//Key-ladder generated key
    EN_APIPVR_ENCRYPTION_DYNAMICKEY = 5,//Dynamic key
    EN_APIPVR_ENCRYPTION_AESDMA_NUM,//AESDMA(Block) Encryption above
    //!!!Notice: Before release record clear stream lib or source code to customer, need follow formal release rule!!!//
    //!!!for details, please contact the HQ PVR module owner about the release flow!!!//
    EN_APIPVR_ENCRYPTION_CLEAR  = 0x100,//record to clear from scrambled, no encryption anymore
    EN_APIPVR_ENCRYPTION_MAX,
} APIPVR_ENCRYPTION_TYPE;

typedef enum
{
    EN_APIPVR_PLAYBACK_SPEED_32XFB    = -32000,   // -32 X
    EN_APIPVR_PLAYBACK_SPEED_16XFB    = -16000,   // -16 X
    EN_APIPVR_PLAYBACK_SPEED_8XFB     =  -8000,   // - 8 X
    EN_APIPVR_PLAYBACK_SPEED_4XFB     =  -4000,   // - 4 X
    EN_APIPVR_PLAYBACK_SPEED_2XFB     =  -2000,   // - 2 X
    EN_APIPVR_PLAYBACK_SPEED_1XFB     =  -1000,   // - 1 X
    EN_APIPVR_PLAYBACK_SPEED_0X       =      0,
    EN_APIPVR_PLAYBACK_SPEED_STEP_IN  =      1,   // 1 Frame
    EN_APIPVR_PLAYBACK_SPEED_FF_1_32X =     32,   // 1 / 32 X
    EN_APIPVR_PLAYBACK_SPEED_FF_1_16X =     64,   // 1 / 16 X
    EN_APIPVR_PLAYBACK_SPEED_FF_1_8X  =    125,   // 1 / 8  X
    EN_APIPVR_PLAYBACK_SPEED_FF_1_4X  =    250,   // 1 / 4  X
    EN_APIPVR_PLAYBACK_SPEED_FF_1_2X  =    500,   // 1 / 2  X
    EN_APIPVR_PLAYBACK_SPEED_1X       =   1000,   //     1  X
    EN_APIPVR_PLAYBACK_SPEED_2XFF     =   2000,   //     2  X
    EN_APIPVR_PLAYBACK_SPEED_4XFF     =   4000,   //     4  X
    EN_APIPVR_PLAYBACK_SPEED_8XFF     =   8000,   //     8  X
    EN_APIPVR_PLAYBACK_SPEED_16XFF    =  16000,   //    16  X
    EN_APIPVR_PLAYBACK_SPEED_32XFF    =  32000,   //    32  X
    EN_APIPVR_PLAYBACK_SPEED_INVALID  = 0xFFFF,
} APIPVR_PLAYBACK_SPEED;


enum{
    PVR_ID_DOWNLOAD=0,
    PVR_ID_UPLOAD,
    PVR_ID_BUFFER,
    PVR_ID_THUMBNAIL,
    PVR_ID_BROWSER_BUFFER,
    PVR_ID_SWBUFFER,
    PVR_ID_ES_VIDEO_BUFFER,
    PVR_ID_ES_AUDIO_BUFFER,
    PVR_ID_MN_VIDEO_BUFFER,
    PVR_ID_MN_AUDIO_BUFFER,
    PVR_ID_TSR_VIDEO_BUFFER,
    PVR_ID_TSR_AUDIO_BUFFER,
};

typedef enum
{
    EN_APIPVR_FILE_SYSTEM_TYPE_UNKNOWN,
    EN_APIPVR_FILE_SYSTEM_TYPE_JFFS2,
    EN_APIPVR_FILE_SYSTEM_TYPE_VFAT,
    EN_APIPVR_FILE_SYSTEM_TYPE_EXT2,
    EN_APIPVR_FILE_SYSTEM_TYPE_EXT3,
    EN_APIPVR_FILE_SYSTEM_TYPE_MSDOS,
    EN_APIPVR_FILE_SYSTEM_TYPE_NTFS,
    EN_APIPVR_FILE_SYSTEM_TYPE_NUM,
    EN_APIPVR_FILE_SYSTEM_TYPE_INVALID = EN_APIPVR_FILE_SYSTEM_TYPE_NUM,
} EN_APIPVR_FILE_SYSTEM_TYPE;



typedef enum
{
    EN_APIPVR_EVENT_ERROR_OVERRUN,
    EN_APIPVR_EVENT_ERROR_NO_DISK_SPACE,
    EN_APIPVR_EVENT_ERROR_SYNCBYTE_ERROR,
    EN_APIPVR_EVENT_ERROR_WRITE_FILE_FAIL,
    EN_APIPVR_EVENT_ERROR_READ_FILE_FAIL,
    EN_APIPVR_EVENT_NOTIFY_FILE_END,
    EN_APIPVR_EVENT_NOTIFY_FILE_END_RESUME,
    EN_APIPVR_EVENT_NOTIFY_FILE_BEGIN,
    EN_APIPVR_EVENT_NOTIFY_RECORDED_TIME,
    EN_APIPVR_EVENT_NOTIFY_RECORDED_SIZE,
    EN_APIPVR_EVENT_NOTIFY_RECORDED_NODATA,
    EN_APIPVR_EVENT_NOTIFY_PLAYBACK_TIME,
    EN_APIPVR_EVENT_NOTIFY_PLAYBACK_SIZE,
    EN_APIPVR_EVENT_NOTIFY_THUMBNAIL_AUTO_CAPTURE,
    EN_APIPVR_EVENT_NOTIFY_TRICK_MODE_DONE,
    EN_APIPVR_EVENT_NOTIFY_PLAYBACK_SUSPEND,
    EN_APIPVR_EVENT_ERROR_INVALID_ADDR,
    EN_APIPVR_PATH_EVENT_NOTIFY_TIMESHIFT_SEAMLEASS_TIMEOUT,    ///For Timeshift Seamless
    EN_APIPVR_EVENT_NOTIFY_PLAYBACK_DATABUFFERING_STATUS,//For data buffering status by playback in backgornd(time-shift) record case.
    EN_APIPVR_EVENT_NOTIFY_DESTROY_PVR,
} APIPVR_EVENT;

typedef enum
{
    EN_APIPVR_CODEC_TYPE_NONE = 0,      ///unsupported codec type
    EN_APIPVR_CODEC_TYPE_MPEG2,         ///MPEG 1/2
    EN_APIPVR_CODEC_TYPE_H264,          ///H264
    EN_APIPVR_CODEC_TYPE_AVS,           ///AVS
    EN_APIPVR_CODEC_TYPE_HEVC,          ///H.265
    EN_APIPVR_CODEC_TYPE_VC1,           ///VC1
    EN_APIPVR_CODEC_TYPE_MPEG2_IFRAME,  ///For MPEG 1/2 IFrame
    EN_APIPVR_CODEC_TYPE_H264_IFRAME    ///For H264 IFrame
} APIPVR_CODEC_TYPE;

typedef enum
{
    EN_APIPVR_AUD_DVB_NONE=0,
    EN_APIPVR_AUD_DVB_MPEG=1,
    EN_APIPVR_AUD_DVB_AC3=2,
    EN_APIPVR_AUD_DVB_AC3P=3,
    EN_APIPVR_AUD_DVB_AAC=4
}APIPVR_AUD_CODEC_TYPE;

typedef enum
{
    EN_APIPVR_AV_CODEC_VIDEO_NONE = 0,      ///unsupported video codec type
    EN_APIPVR_AV_CODEC_VIDEO_MPEG2,         ///MPEG 1/2
    EN_APIPVR_AV_CODEC_VIDEO_H264,          ///H264
    EN_APIPVR_AV_CODEC_VIDEO_AVS,           ///AVS
    EN_APIPVR_AV_CODEC_VIDEO_HEVC,          ///H.265
    EN_APIPVR_AV_CODEC_VIDEO_VC1,           ///VC1
    EN_APIPVR_AV_CODEC_VIDEO_MPEG2_IFRAME,  ///For MPEG 1/2 IFrame
    EN_APIPVR_AV_CODEC_VIDEO_H264_IFRAME,   ///For H264 IFrame
    EN_APIPVR_AV_CODEC_VIDEO_MAX,
    EN_APIPVR_AV_CODEC_AUD_NONE = 0x4000,   ///unsupported audio codec type
    EN_APIPVR_AV_CODEC_AUD_MPEG,            ///MPEG (audio)
    EN_APIPVR_AV_CODEC_AUD_AC3,             ///AC3 (audio)
    EN_APIPVR_AV_CODEC_AUD_AC3P,            ///AC3P (audio)
    EN_APIPVR_AV_CODEC_AUD_AAC,             ///AAC (audio)
    EN_APIPVR_AV_CODEC_AUD_MAX,
    EN_APIPVR_AV_CODEC_INVALID = 0xFFFFFFFFUL,
}EN_APIPVR_AV_CODEC_TYPE;

typedef enum
{
    EN_APIPVR_FILTER_TYPE_NONE = 0,
    EN_APIPVR_FILTER_TYPE_VIDEO,      ///< video filter type
    EN_APIPVR_FILTER_TYPE_AUDIO,      ///< audio filter type
    EN_APIPVR_FILTER_TYPE_AUDIO2,     ///< audio2 filter type
    EN_APIPVR_FILTER_TYPE_SECTION,    ///< section filter type
    EN_APIPVR_FILTER_TYPE_PES,        ///< PES filter type
    EN_APIPVR_FILTER_TYPE_PCR,        ///< PCR filter type
    EN_APIPVR_FILTER_TYPE_TELETEXT,   ///< Teletext filter type
    EN_APIPVR_FILTER_TYPE_MAX,
    EN_APIPVR_FILTER_TYPE_INVALID = 0xFFFFFFFFUL,
}EN_APIPVR_FILTER_TYPE;

typedef enum
{
    EN_APIPVR_INFO_NB_AUDIO_TRACK = 0,
    /// The number of Audio Track; Param: MS_U32
    EN_APIPVR_INFO_AUDIO_TRACK_INFO,
    /// Audio Track information; Param: PVRTrackInfo_t
} EN_APIPVR_INFO_TYPE;

typedef enum
{
    EN_APIPVR_ACTION_TYPE_SET,
    EN_APIPVR_ACTION_TYPE_GET,
    EN_APIPVR_ACTION_TYPE_INVALID
} EN_APIPVR_ACTION_TYPE;

typedef enum
{
    EN_APIPVR_PLAYBACK_DATABUFFERING_START,
    EN_APIPVR_PLAYBACK_DATABUFFERING_DONE,
    EN_APIPVR_PLAYBACK_DATABUFFERING_TIMEOUT,
} EN_APIPVR_PLAYBACK_DATABUFFERING_STATUS;

typedef enum
{
    EN_APIPVR_PLAYBACK_INFO_TSP_PCRENG_ID = 0,
    EN_APIPVR_PLAYBACK_INFO_INVALID
} EN_APIPVR_PLAYBACK_INFO_TYPE;

typedef struct PVREventInfo_s{
    APIPVR_EVENT pvrEvent;
    MS_U32 u32Data[2];
}PVREventInfo_t;

typedef struct PVR_TELETEXT_INFO_s
{
    MS_U8   u8ComponentTag;
    MS_U8   u16TTX_PidFilter;
    MS_U16  u16TTX_Pid;
    MS_U8   u8lanuage_code[3];
    MS_U8   u8TTXType; // EN_TTX_TYPE
    MS_U8   u8TTXMagNum;
    MS_U8   u8TTXPageNum;
} PVR_TELETEXT_INFO;

typedef struct PVR_DVB_SUBTITLE_INFO_s
{
    MS_U8   u8ComponentTag;
    MS_U8   u8Sub_PidFilter;
    MS_U16  u16Sub_Pid;
    MS_U8   u8lanuage_code[3];
    MS_U8   u8subtitling_type; // EN_TTX_TYPE
    MS_U16  u16composition_page_id;
    MS_U16  u16ancillary_page_id;
} PVR_DVB_SUBTITLE_INFO;

typedef struct PVR_AUDIO_INFO_s
{
    MS_U8  u8ISOLangInfo[3]; //:24
    MS_U8  u8ISOAudMode;     //: 2    ///< 0x00: Stereo, 0x01: Mono right, 0x02: Mono left
    MS_U8  u8ISOAudType;     //: 3    ///< 0x00:Undefined,
                                      ///  0x01:Clean effects,
                                      ///  0x02: Hearing impaired,
                                      ///  0x03: Visual impaired commentary,
                                      ///  0x04~0xFF: Reserved.
    MS_U8  u8ISOIsValid;     //: 1    ///< Valid or not
    MS_U16 u16AudPID;        //:13    ///< Audio PID
    MS_U8  u8AudPIDFilter;   //:13    ///< Audio PID Filter
    MS_U8  u8AudType;        //: 3    ///<  0x01: MPEG, 0x02: AC-3, 0x03: MPEG4_AUD
    //WORD Reserved          //: 1    ///< Reserved
}PVR_AUDIO_INFO;

//<
// For large metadata file
#define SI_MAX_EVENT_NAME_LEN       80

//program info: duration and name
typedef struct{
    MS_U16 u16ProgramName[SI_MAX_EVENT_NAME_LEN];
    MS_U32 u32Duration;
}Cross_Program;
//>

typedef struct PVR_TSPInfo_s
{
    MS_U16 u16DmxFlowInput;
    MS_BOOL bClkInv;
    MS_BOOL bExtSync;
    MS_BOOL bParallel;
}PVR_TSPInfo_t;


typedef struct PVRFilterInfo_s{
    MS_U16 u16PID;
    MS_U8  u8PIDFilter;
    MS_U32 u32DmxFltType;
}PVRFilterInfo_t;


typedef struct APIPVRDisplayWinInfo_s
{
    MS_U32  u32X_Pos;
    MS_U32  u32Y_Pos;
    MS_U32  u32Width;
    MS_U32  u32Height;
    MS_BOOL bDef;

}APIPVRDisplayWinInfo_t;


typedef struct PVRProgramInfo_s{
    char FileName[FILE_PATH_SIZE];
    APIPVR_MEMBER_SERVICETYPE enServiceType;
    MS_U32 u32StartTime;
    MS_U32 u32LCN;          /* Program Number */
    APIPVR_CODEC_TYPE enVCodec;
    MS_U32 u32ACodec;
    MS_U8 u8TTXNum;
    PVR_TELETEXT_INFO TXTInfo[APIPVR_MAX_TTXINFO_NUM];
    MS_U8 u8AudioInfoNum;
    PVR_AUDIO_INFO AudioInfo[APIPVR_MAX_AUDIOINFO_NUM];
    MS_U8 u8DVBSubtitleNum;
    PVR_DVB_SUBTITLE_INFO DVBSubtInfo[APIPVR_MAX_SUBTITLEINFO_NUM];
    MS_U8 u8EBUSubtitleNum;
    PVR_TELETEXT_INFO EBUSubtInfo[APIPVR_MAX_SUBTITLEINFO_NUM];
    MS_U32 u32Duration; /* sec */
    MS_U64 u64FileLength;
    MS_BOOL bIsScrambled;
    MS_BOOL bIsEncrypted;
    MS_U8 u8ProgIdx;
    MS_U32 u16AudioPid;
    APIPVR_ENCRYPTION_TYPE enEncryptionType;
    MS_U16 u16CCPid;
    MS_U8 u8Age;
    MS_U32 u32LastPlayPositionInSec;
    MS_BOOL bLocked;
    PVRFilterInfo_t Filters[APIPVR_MAX_PID_NUM];  /* Addr */
    MS_BOOL bLive;
}PVRProgramInfo_t;


typedef struct PVRDataInfo_s{
    MS_U16 u16VideoPID;
    APIPVR_CODEC_TYPE enCodecType;
    MS_U16 u16AudioPID;
    APIPVR_AUD_CODEC_TYPE enAdecType;
}PVRDataInfo_t;

typedef struct APIPVRTrackInfo_s{
    MS_U16 u16Pid;
    EN_APIPVR_FILTER_TYPE enFltType;
    EN_APIPVR_AV_CODEC_TYPE enAVCodecType;
    void *pReserved;
}APIPVRTrackInfo_t;

typedef struct APIPVR_PARAM_s{
    /// Option
    EN_APIPVR_INFO_TYPE enInfoType;
    /// Parameter size
    MS_U32 u32Paramsize;
    /// Reserved
    MS_U32 u32Reserved;
    /// Parameter address
    void *pParam;
    /// Reserved;
    void *pReserved;
}APIPVR_PARAM_t;

typedef struct PVRFuncInfo_s{
    APIPVR_MEMALLOC_FUNC ALLOC;
    APIPVR_MEMFREE_FUNC FREE;
    APIPVR_MEMPA2VA_FUNC PA2VA;
    APIPVR_MEMFLUSH_FUNC MemFlush;
    APIPVR_KLADDER_FUNC KLADDER;
    APIPVR_UPDATEINFO_FUNC InfoUpdate;
    APIPVR_DSCMB_FUNC DscmbConnect;
    APIPVR_DSCMBCLOSE_FUNC DscmbDisConnect;
    APIPVR_TRICKSET_FUNC TrickSetting;
    APIPVR_DMAENCRYPT_FUNC DMAEncryptDescrypt;
    APIPVR_MEMFLUSH_FUNC MemRead;
}PVRFuncInfo_t;

typedef union PVRPlaybackInfo_s
{
    MS_U8 u8PcrEngID; // PVR_TSP
}PVRPlaybackInfo_t;

///[[[For TimeshiftSeamless
typedef enum
{
    EN_APIPVR_RECORD_NORMAL=0, /// origin record mode , create record path and record to file
    EN_APIPVR_RECORD_MEMORY,  /// background record mode, create record path but just record to download buffer, not to file
    EN_APIPVR_RECORD_FILE,   /// seamless timeshift record mode , no need to create record path , just create file and meta in order to record to file
} APIPVR_RECORD_STATUS;

///

typedef enum
{
    EN_APIPVR_PVR_CFG_NONE,
    EN_APIPVR_CFG_AUTO_PB_PROGRAM_CHG,
} EN_APIPVR_PVR_CFG_CMD;

typedef struct
{
    MS_BOOL                 bSet;
    EN_APIPVR_PVR_CFG_CMD   eCfgCmd;

    MS_BOOL                 bAutoPlaybackProgramChg;
} PVR_CfgParam;



MS_BOOL MApi_PVR_Destroy(void);
MS_BOOL MApi_PVR_ResetMmap(MS_U32 u32PvrId);
MS_BOOL MApi_PVR_Mmap(MS_U32 u32PvrId,MS_U32 u32PhyAddr,MS_U32 u32Size);
MS_BOOL MApi_PVR_Init(char *pMountPath, EN_APIPVR_FILE_SYSTEM_TYPE enFSType,APIPVR_TUNER_NUM enTunerNum,PVR_TSPInfo_t pvrTspInfo);
MS_S32 MApi_PVR_GetMessageQueueID(void);
MS_BOOL MApi_PVR_SetMsgWaitMs(MS_U32 u32WaitMs);
MS_BOOL MApi_PVR_SetMetaData(PVRProgramInfo_t *pProgramInfo,MS_U8 u8RecordProgramIdx,MS_U32 u32AESDMAKey0,MS_U32 u32AESDMAKey1,MS_U32 u32AESDMAKey2,MS_U32 u32AESDMAKey3);
MS_BOOL MApi_PVR_RecordProgramChange(MS_U8 *pu8hRecord,MS_U8 u8ProgramIdx,PVRProgramInfo_t *pProgramInfo);
MS_BOOL MApi_PVR_TimeshiftRecordStart(MS_U8 *pu8hRecord,MS_U8 u8RecordProgramIdx);
MS_BOOL MApi_PVR_EX_TimeshiftRecordStart(MS_U8 *pu8hRecord, void *pAVStreamID, MS_U8 u8RecordProgramIdx, MS_U8 u8PlaybackProgramIdx);
MS_BOOL MApi_PVR_EX_TimeshiftRecordEngSet(MS_U8 *pu8hRecord, void *pAVStreamID, MS_U8 u8RecordProgramIdx, MS_U8 u8PlaybackProgramIdx);
MS_BOOL MApi_PVR_EX_TimeshiftRecordEngEnable(MS_U8 *pu8hRecord);

MS_BOOL MApi_PVR_TimeshiftRecordStop(MS_U8 *pu8hRecord,MS_U8 u8RecordProgramIdx);
MS_BOOL MApi_PVR_EX_TimeshiftRecordStop(MS_U8 *pu8hRecord,void *pAVStreamID,MS_U8 *pu8hPlayback,MS_U8 u8RecordProgramIdx,MS_U8 u8PlaybackProgramIdx);

MS_BOOL MApi_PVR_TimeshiftStop(MS_U8 *pu8hRecord,MS_U8 *pu8hPlayback,MS_U8 u8RecordProgramIdx);
MS_BOOL MApi_PVR_EX_TimeshiftStop(MS_U8 *pu8hRecord, void *pAVStreamID, MS_U8 *pu8hPlayback,MS_U8 u8RecordProgramIdx,MS_U8 u8PlaybackProgramIdx);

MS_BOOL MApi_PVR_TimeshiftPlaybackStart(MS_U8 *pu8hPlayback, char *fileName, MS_U32 u32PlaybackTimeInSec, MS_U32 u32PlaybackPTS);
MS_BOOL MApi_PVR_EX_TimeshiftPlaybackStart(MS_U8 u8CurProgramPath, MS_U8 *pu8hPlayback,MS_U8 u8PlaybackProgramIdx, void *pAVStreamID, char *fileName, MS_U32 u32PlaybackTimeInSec, MS_U32 u32PlaybackPTS);

MS_BOOL MApi_PVR_TimeshiftPlaybackStop(MS_U8 *pu8hPlayback,MS_U8 u8RecordProgramIdx);
MS_BOOL MApi_PVR_EX_TimeshiftPlaybackStop(MS_U8 *pu8hPlayback,MS_U8 u8RecordProgramIdx,MS_U8 u8PlaybackProgramIdx);

MS_BOOL MApi_PVR_RecordStart(MS_U8 *pu8hRecord,MS_U8 u8RecordProgramIdx,APIPVR_FILE_FORMAT pvrFileFormat, MS_U32 u32FreeDiskSpaceInMB,MS_BOOL bRecordAll);
MS_BOOL MApi_PVR_EX_RecordStart(MS_U8 *pu8hRecord,void *pAVStreamID,MS_U8 u8RecordProgramIdx,APIPVR_FILE_FORMAT pvrFileFormat, MS_U32 u32FreeDiskSpaceInMB,MS_BOOL bRecordAll);
MS_BOOL MApi_PVR_EX_RecordEngSet(MS_U8 *pu8hRecord, void *pAVStreamID, MS_U8 u8RecordProgramIdx,APIPVR_FILE_FORMAT pvrFileFormat, MS_U32 u32FreeDiskSpaceInMB,  MS_U32 u32PlanRecTimeInSec);
MS_BOOL MApi_PVR_EX_RecordEngEnable(MS_U8 *pu8hRecord,MS_BOOL bRecordAll);

MS_BOOL MApi_PVR_RecordPause(MS_U8 u8RecordProgramIdx);
MS_BOOL MApi_PVR_RecordResume (MS_U8 u8RecordProgramIdx);
MS_BOOL MApi_PVR_RecordStop(MS_U8 *pu8hRecord,MS_U8 u8RecordProgramIdx);
MS_BOOL MApi_PVR_PlaybackOpen(MS_U8 u8CurProgramPath, MS_U8 *pu8hPlayback, void *pAVStreamID, char *fileName);
MS_BOOL MApi_PVR_PlaybackStart(MS_U8 *pu8hPlayback, char *fileName, MS_U32 u32PlaybackTimeInSec, MS_U32 u32PlaybackPTS);
MS_BOOL MApi_PVR_EX_PlaybackStart(MS_U8 u8CurProgramPath, MS_U8 *pu8hPlayback, void *pAVStreamID, char *fileName, MS_U32 u32PlaybackTimeInSec, MS_U32 u32PlaybackPTS);
MS_BOOL MApi_PVR_PlaybackPause(MS_U8 u8hPlayback);
MS_BOOL MApi_PVR_PlaybackResume(MS_U8 u8hPlayback);
MS_BOOL MApi_PVR_PlaybackStop(MS_U8 *pu8hPlayback);
MS_BOOL MApi_PVR_PlaybackFastForward(MS_U8 u8hPlayback);
MS_BOOL MApi_PVR_PlaybackFastBackward(MS_U8 u8hPlayback);
MS_BOOL MApi_PVR_PlaybackSetSpeed(MS_U8 u8hPlayback,MS_S32 enSpeed);
MS_S32 MApi_PVR_PlaybackGetSpeed(MS_U8 u8hPlayback);
MS_BOOL MApi_PVR_PlaybackJumpToTime(MS_U8 u8hPlayback,MS_U32 u32JumpToTimeInSeconds);
MS_BOOL MApi_PVR_PlaybackSetFileEndAutoResume(MS_BOOL bPlaybackResumeWhenTrickToFileEnd);
MS_BOOL MApi_PVR_Configure(PVR_CfgParam *stPvrCfgParam);
MS_BOOL MApi_PVR_PlaybackProgramChangeOpen(MS_U8 u8hPlayback);
MS_BOOL MApi_PVR_PlaybackProgramChangeStart(MS_U8 u8hPlayback,PVRDataInfo_t dataInfo);
MS_BOOL MApi_PVR_SetTimeshiftFileSize(MS_U64 u64TimeShiftFileSizeInKB);
MS_BOOL MApi_PVR_SetTimeshiftFileDuration(MS_U32 u32PlanRecTimeInSec);
MS_BOOL MApi_PVR_IsRecording(MS_U8 u8hRecord);
MS_BOOL MApi_PVR_IsRecordPause(MS_U8 u8hRecord);

MS_BOOL MApi_PVR_IsPlaybacking(void);
MS_BOOL MApi_PVR_EX_IsPlaybacking(MS_U8 u8hPlayback);

MS_BOOL MApi_PVR_IsPlaybackPause(MS_U8 u8hPlayback);
MS_BOOL MApi_PVR_PlaybackStepIn(MS_U8 u8hPlayback);
MS_BOOL MApi_PVR_PlaybackJumpForward(MS_U8 u8hPlayback);
MS_BOOL MApi_PVR_PlaybackJumpBackward(MS_U8 u8hPlayback);
MS_U32 MApi_PVR_SpeedCheck(void);
MS_U32 MApi_PVR_GetPlaybackTimeInSec(MS_U8 u8hPlayback);


MS_BOOL MApi_PVR_PlaybackABLoop(char *fileName,MS_U32 u32ABLoopBeginTime, MS_U32 u32ABLoopEndTime);
MS_BOOL MApi_PVR_EX_PlaybackABLoop(MS_U8 u8hPlayback, char *fileName,MS_U32 u32ABLoopBeginTime, MS_U32 u32ABLoopEndTime);

MS_BOOL MApi_PVR_PlaybackABLoopReset(void);
MS_BOOL MApi_PVR_EX_PlaybackABLoopReset(MS_U8 u8hPlayback);

MS_BOOL MApi_PVR_PlaybackJumpToThumbnail(MS_U8 u8hPlayback,MS_U32 u32JumpToTimeInSeconds, MS_U32 u32capture_pts);
MS_BOOL MApi_PVR_CaptureVideo(MS_U32 u32PhysicalAddr, MS_U32 u32Width, MS_U32 u32Height,MS_U32 *capturepts);
MS_BOOL MApi_PVR_EX_CaptureVideo(void *pVStreamID, MS_U32 u32PhysicalAddr, MS_U32 u32Width, MS_U32 u32Height,MS_U32 *capturepts);

MS_U32 MApi_PVR_GetPlaybackTime(MS_U8 u8hPlayback);
MS_U32 MApi_PVR_GetRecordTime(MS_U8 u8hRecord,MS_U8 u8RecordProgramIdx);

MS_BOOL MApi_PVR_GetProgramInfo(char *fileName,PVRProgramInfo_t * programInfo);

const char * MApi_PVR_GetMouthPath(void);
MS_BOOL MApi_PVR_IsTimeShift(void);
MS_BOOL MApi_PVR_EX_IsTimeShift(MS_U8 u8PlaybackProgramIdx);

MS_BOOL MApi_PVR_SkipTableGetIndex(MS_U32 *pu32SkipBeginTime,MS_U32 *pu32Idx, MS_U32 *pu32SkipEndTime);
MS_BOOL MApi_PVR_EX_SkipTableGetIndex(MS_U8 u8hPlayback, MS_U32 *pu32SkipBeginTime,MS_U32 *pu32Idx, MS_U32 *pu32SkipEndTime);

MS_BOOL MApi_PVR_SkipTableAdd(char *fileName, MS_U32 u32SkipBeginTime, MS_U32 u32SkipEndTime);
MS_BOOL MApi_PVR_EX_SkipTableAdd(MS_U8 u8hPlayback, char *fileName, MS_U32 u32SkipBeginTime, MS_U32 u32SkipEndTime);

MS_BOOL MApi_PVR_SkipTableRemove(char *fileName,MS_U32 u32Index);
MS_BOOL MApi_PVR_EX_SkipTableRemove(MS_U8 u8hPlayback,char *fileName,MS_U32 u32Index);

MS_BOOL MApi_PVR_SetPlaybackRetentionLimit(char *fileName, MS_U32 *pu32RetentionLimitTime);
MS_BOOL MApi_PVR_EX_SetPlaybackRetentionLimit(MS_U8 u8hPlayback, char *fileName, MS_U32 *pu32RetentionLimitTime);

MS_BOOL MApi_PVR_SaveAge(char *filename,MS_U8 u8Age);
void MApi_PVR_SaveCrossProgramInfo(char *fileName,MS_U32 u32ProDuration,MS_U16 *u16pProName); //For getting next program info
void MApi_PVR_GetCrossProgramInfo(char *fileName,Cross_Program *PVRCrossProgram,MS_U8 *u8CrossProgram_Index);


void MApi_PVR_NotifyTrickModeDoneMsg(void); // For resolution change, trigger once

MS_BOOL MApi_PVR_SetPlaybackType(APIPVR_PLAYBACK_TYPE enType);
MS_BOOL MApi_PVR_SetSyncInfo(MS_U32 u32SyncDelay, MS_U16 u16SyncTolerance);

MS_BOOL MApi_PVR_FreezeScreenInTimeshiftRecord(EN_APIPVR_ACTION_TYPE enAction,MS_BOOL *bFreeze);
MS_BOOL MApi_PVR_EX_FreezeScreenInTimeshiftRecord(MS_U8 u8PlaybackProgramIdx, EN_APIPVR_ACTION_TYPE enAction,MS_BOOL *bFreeze);

MS_BOOL MApi_PVR_RemoveFile(char *fileName);
MS_BOOL MApi_PVR_SetDebugLevel(APIPVR_LOG_LEVEL enLogLevel);


MS_BOOL MApi_PVR_SetFuncInfo(PVRFuncInfo_t funcInfo);

MS_BOOL MApi_PVR_LockProgram(char *fileName,MS_BOOL bLock);
MS_BOOL MApi_PVR_RenameFileName(char *fileName,char *newFileName);
MS_BOOL MApi_PVR_SetRecordType(APIPVR_RECORD_TYPE enType);

MS_BOOL MApi_PVR_UpdateLastPlaybackTime(EN_APIPVR_ACTION_TYPE enAction,char *fileName,MS_U32 *pu32LastPlaybackTime);
MS_BOOL MApi_PVR_GetRecordStartTime(MS_U8 u8hRecord,MS_U32* u32RecordStartTime);
MS_BOOL MApi_PVR_SetPlaybackPathNum(MS_U8 PlaybackPathNum);
MS_BOOL MApi_PVR_SetPlaybackInfo(MS_U8 u8PlaybackPath,EN_APIPVR_PLAYBACK_INFO_TYPE eInfoType,PVRPlaybackInfo_t *pstPlaybackInfo);
///[[ For Timeshift Seamless
MS_BOOL MApi_PVR_EX_BackRecordEngSet(MS_U8 *pu8hRecord, void *pAVStreamID, MS_U8 u8RecordProgramIdx,APIPVR_FILE_FORMAT pvrFileFormat, MS_U32 u32FreeDiskSpaceInMB,  MS_U32 u32PlanRecTimeInSec);
MS_BOOL MApi_PVR_SetBackRecordMetaData(PVRProgramInfo_t *pProgramInfo,MS_U8 u8RecordProgramIdx,MS_U32 u32AESDMAKey0,MS_U32 u32AESDMAKey1,MS_U32 u32AESDMAKey2,MS_U32 u32AESDMAKey3);
MS_BOOL MApi_PVR_BackRecordStop(MS_U8 *pu8hRecord,MS_U8 u8RecordProgramIdx);

// @NOTE DynamicKey
MS_BOOL MApi_PVR_EX_SetEncryptKeyIdx(MS_U8 u8hRecord, MS_U32 u32KeyIdx);
MS_BOOL MApi_PVR_EncryptFileCheck(char *fileName);

MS_BOOL MApi_PVR_PlaybackGetTrackInfoByPara(MS_U8 u8hPlayback, APIPVR_PARAM_t *astParam, const MS_U32 u32ParamNum);
MS_BOOL MApi_PVR_PlaybackTrackChange(MS_U8 u8hPlayback, APIPVRTrackInfo_t *astTrackInfo, MS_U32 u32TrackNum);
MS_BOOL MApi_PVR_DispPlayWindowChange(MS_U8 u8hPlayback, APIPVRDisplayWinInfo_t stAPIDisplayWinInfo);


#ifdef __cplusplus
}
#endif

#endif /* _MSAPI_PVR_H_ */

