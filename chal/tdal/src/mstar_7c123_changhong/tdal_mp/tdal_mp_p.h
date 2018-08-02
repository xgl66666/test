/*
 * tdal_mp_p.h
 *
 *  Created on: 5 Feb 2013
 *      Author: mijovic
 */

#ifndef TDAL_MP_P_H_
#define TDAL_MP_P_H_

#include "crules.h"
#include "tdal_mp.h"
#include "MsTypes2.h"
//#include "MSrv_ZmplayerSubtitleDecoder.h"
#include "mmsdk_interface_def.h"
#include "MsTypes.h"

//#include "msAPI_MM.h"
#define MP_DBG 0
#define DEMO_FILE 1
#define MM_PHOTO_PATH                                  0x1 // 1 : MVOP  2 : GOP
#if MP_DBG
#define mp_printf(fmt, args...)  printf("[%s][%d]" fmt, __FUNCTION__, __LINE__, ## args)
#else
#define mp_printf(fmt, args...)  while(0);
#endif

#define IPANEL(dev, func) g_IPanel.func()
#define MAX_SUPPORTED_RES_H 720
#define MAX_SUPPORTED_RES_V 576

#define kTDAL_MPm_OPEN_SESSION_MAX	    10
#define kTDAL_MPm_MAGIC_NUMBER		    0xDEADBEEF

#define MMSDK_MEDIA_TYPE_MIN            (E_MMSDK_MEDIA_TYPE_PHOTO)
#define MMSDK_MEDIA_TYPE_NUM            (E_MMSDK_MEDIA_TYPE_MOVIE+1)
#define MAX_PARA_SIZE                   50
#define EXTENTION_NAME_LENGTH           10
#define FILE_NAME_LENGTH                1024
#if defined(MSOS_TYPE_ECOS)
#define TH_EVT_PROC_STACK_SIZE (8*(16<<10))
#elif defined(MSOS_TYPE_LINUX)
#define TH_EVT_PROC_STACK_SIZE 0 //use pthread default stack
#endif

#define MM_EVENT_VIDEO_NOTIFY 0x00000001
#define MM_EVENT_MUSIC_NOTIFY 0x00000002
#define MM_EVENT_PHOTO_NOTIFY 0x00000004
#define MM_EVENT_VIDEO_PROC_EXIT 0x00000008
#define MM_EVENT_MUSIC_PROC_EXIT 0x00000010
#define MM_EVENT_PHOTO_PROC_EXIT 0x00000020

#define ASSERT(x)  assert(x)
#define INVALID_UNICODE                 0xFFFF
#define INVALID_POOL_ID -1
#define SETU16(ptr, x, data) \
        (ptr)[(x)] = (data) & 0xFF; \
        (ptr)[(x)+1] = ((data) >> 8) & 0xFF;

#define SETU32(ptr, x, data) \
        (ptr)[(x)] = (data) & 0xFF; \
        (ptr)[(x)+1] = ((data) >> 8) & 0xFF; \
        (ptr)[(x)+2] = ((data) >> 16) & 0xFF; \
        (ptr)[(x)+3] = ((data) >> 24) & 0xFF;

#define MM_GET_OPTION(stParam, Option, SubOptioin, ParamSize, Param, Item) \
    do{ \
        stParam.u32Option = Option; \
        stParam.u32SubOption = SubOptioin; \
        stParam.u32Paramsize = ParamSize; \
        stParam.pParam = Param; \
        MMSDK_GetOptionByPara(Item, &stParam, 1); \
    }while(0)
    
#define MM_FILL_MEM_INFO(_buf, _addr, _len, _mtype) \
            do{ \
                _buf.u32VirtualAddr = MS_PA2KSEG1((MS_U32)MEM_ADR_BY_MIU(_addr, _mtype)); \
                _buf.u32Size = _len; \
                _buf.bIsMiu0 = (_mtype & MIU1) ? FALSE : TRUE; \
                _buf.u32MiuNo = (_buf.bIsMiu0 == TRUE) ? 0 : 1; \
                _buf.u32Addr = (_buf.bIsMiu0 == TRUE) ? _addr : (_addr | MIU_INTERVAL); \
            }while(0)

typedef enum
{
    MPLAYER_FILE_MODE,      //for local file
    MPLAYER_SEEKABLE_STREAM_MODE,   //for seekable stream
    MPLAYER_UNSEEKABLE_STREAM_MODE, //for unseekable stream
    MPLAYER_MODE_INVALID,
} EN_MPLAYER_FILE_MODE;

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

typedef enum
{
    /// Media item preview mode is normal. For photo and video media.
    E_PREVIEW_NORMAL_MODE,
    /// Media item preview mode is pause mode. For video media only, the video will be paused on the time (u32VideoPauseTime).
    E_PREVIEW_VIDEO_PAUSE_MODE,
    /// Media item preview mode is none.
    E_PREVIEW_NUM
} EN_PREVIEW_MODE;

typedef struct
{
    /// Preview mode.
    EN_PREVIEW_MODE eMode;
    /// The time stamp for preview pause time (ms unit). Valid for preview mode is E_PREVIEW_VIDEO_PAUSE_MODE.
    MS_U32 u32VideoPauseTime;
} ST_PREVIEW_SETTING;

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
    PCM_FORMAT_PCM, //PCM
    PCM_FORMAT_M_ADPCM, //M ADPCM
    PCM_FORMAT_IMA_ADPCM, //8-bit ITU-T G.711
    PCM_FORMAT_NUM,
} EN_PCM_FORMAT;

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
    MPLAYER_SEAMLESS_NONE,
    MPLAYER_SEAMLESS_FREEZ,
    MPLAYER_SEAMLESS_SMOTH,     //only for mp4 and flv container
}EN_SEAMLESS_MODE;

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

typedef struct
{
    char                                    FileExt[8];
    EN_MMSDK_MEDIA_TYPE    				FileType;
} Z_File_Ext_Info;

typedef struct
{
    char fileID[4];
    MS_U32 fileleth;
    char wavTag[4];
    char FmtHdrID[4];
    MS_U32  FmtHdrLeth;
    MS_U16 FormatTag;
    MS_U16 Channels;
    MS_U32 SamplesPerSec;
    MS_U32 AvgBytesPerSec;
    MS_U16 BlockAlign;
    MS_U16 BitsPerSample;
    char DataHdrID[4];
    MS_U32  DataHdrLeth;
} WAVE_HDR;

typedef struct
{
	uint32_t            	uiMagicNumber;
	bool                	bOpened;
	tTDAL_MP_EvtNotify  	pEvtNotify;
	void               		*pCtx;
	tTDAL_MP_InputDataRead  pInputDataRead;
	tTDAL_MP_InputDataSeek  pInputDataSeek;
	tTDAL_MP_InputDataLength pInputDataLength;
	//STFFMPEG_Handle_t   	STFFMPEGHandle;
	bool                	bFileOpened;
	bool                    bIsPlaying;
	tTDAL_MP_Capabilities   stCaps;
	//tTDAL_MPm_STVIDCodec   *pstStvidCodec;
	//tTDAL_MPm_STAUDCodec   *pstStaudCodec;
	//uint32_t            	uiOutputMask; /* current output mask destination of the audio/video stream */
	tTDAL_MP_Wnd            stWnd;
    uint16_t            	usPictureWidth;  /* width returned by decoder  */
    uint16_t            	usPictureHeight; /* height returned by decoder */
	//uint32_t            	uiPictureDisplayEventCpt;
	int32_t					iSpeed;
	//int32_t             	iSTCDelay;
	//uint32_t            	uiClocksPerSecond;
	//uint32_t            	uiPreviousPTSInTicks;
	//STOS_Clock_t        	uiPreviousTimeInTicks;

	//uint8_t            		*pPrivDataBuf;  /* Pointer to private data buffer that may
	                                    // be needed by some AV codecs */
	//uint32_t            	uiPrivDataSize;/* Size of the private data buffer */

	/* Stream specific parameters:
	 * The video/audio codecs are given by the client
	 * and not probed in the media data file
	 */
	bool                bIsStreamed;
	tTDAL_MP_ContentType  eStreamVidCodecType; /* only relevant if bIsStreamed is TRUE */
	tTDAL_MP_ContentType  eStreamAudCodecType; /* only relevant if bIsStreamed is TRUE */
    //#if 1
    /* TEMP BUGFIX - THIS MODIFICATION SHOULD BE DONE LATER (BY GS) INSIDE  THE MODULE "stffmpeg.c" */
    //bool                bGoingToBeStopped;   /* means that AVP_Stop() have been requested : so we can abort a read() operation */
    //#endif

    //#if 1  /* fix interlock-bug */
    /* fix an interlock-bug when calling AVP_LengthGet when AVP_Start is called by another thread */
    /* the thread calling AVP_Start locks AVPm_ClientLock() and is blocked by MEDIA_RENDERER_AV's ClientLock in Read callback */
    /* the thread calling AVP_LengthGet locked MEDIA_RENDERER_AV's ClientLock and is blocked by AVPm_ClientLock */
    /* note: the AVP_Start can be very long due to the PlayProbe call */
    //bool                bGoingToBeStarted;
    //#endif /* fix interlock-bug */

	//void               *pSpecificDesc;
} tTDAL_MPm_Desc;
#if 0 //wait porting
IMPORT EN_MPLAYER_MEDIA_TYPE TDAL_MPm_eMediaType;
IMPORT MPLAYER_STREAM_IOCB TDAL_MPm_s_sStreamHandle;
#endif

IMPORT const Z_File_Ext_Info _gstZmmSupportExtTable[42];

IMPORT tTDAL_MP_InputDataRead  _pInputDataRead;
IMPORT tTDAL_MP_InputDataSeek  _pInputDataSeek;
IMPORT tTDAL_MP_InputDataLength _pInputDataLength;
IMPORT MS_U8 TDAL_MPm_u8IsRepeat;
IMPORT tTDAL_MPm_Desc *TDAL_MPi_DescList[kTDAL_MPm_OPEN_SESSION_MAX];
IMPORT MS_U32 _gu32GifAddrOffset;
IMPORT MS_BOOL _gbEnableMotionGif;
//LOCAL MS_U32 _u32FileOption = MPLAYER_MODE_INVALID;


IMPORT void*  AllocNoncachedMem(int size);
IMPORT void   FreeNoncachedMem(void* pBuf);
IMPORT void *ReAllocNoncachedMem(void* pBuf,int size);
IMPORT MS_BOOL          is_TDAL_MP_Initialized(void);
IMPORT int TDAL_MPi_GetRunningMediaDescriptor(void);
IMPORT MS_BOOL TDAL_MPi_CheckMediaDescriptor(tTDAL_MPm_Desc * desc);
IMPORT MS_BOOL TDAL_MPm_Close(void);
IMPORT MS_BOOL TDAL_MPm_Stop(void);
IMPORT MS_BOOL TDAL_MPm_SetFileIsSupport(MS_BOOL bIsSupport);
IMPORT tTDAL_MP_Error TDAL_MPm_MPlayer_Finalize(void);
IMPORT tTDAL_MP_Error TDAL_MPm_AVGetMetadata(tTDAL_MP_MetaDataAv *pstMetadata);
IMPORT tTDAL_MP_Error TDAL_MPm_AudioGetMetadata(tTDAL_MP_MetaDataAudio *pstMetadata);
IMPORT tTDAL_MP_Error TDAL_MPm_PictureGetMetadata(tTDAL_MP_MetaDataPict *pstMetadata);
IMPORT void TDAL_MPi_LockMutex(void);
IMPORT void TDAL_MPi_UnlockMutex(void);
IMPORT bool TDAL_MPi_LockMutexIfDifferentThread(void);
IMPORT void TDAL_MPm_GetMEDIAITEM(MEDIAITEM *item);
IMPORT MS_BOOL TDAL_MPm_Player_GetSpeed(const EN_MMSDK_MEDIA_TYPE eMediaType, MS_S32 *s32Speed);
IMPORT MS_BOOL TDAL_MPm_Player_SetSpeed(const EN_MMSDK_MEDIA_TYPE eMediaType, EN_MMSDK_TRICK_MODE eTrickMode);
IMPORT MS_BOOL TDAL_MPm_Player_GetPosition(const EN_MMSDK_MEDIA_TYPE eMediaType, MS_U32 *u32TimeMS);
IMPORT MS_BOOL TDAL_MPm_Player_SetPosition(const EN_MMSDK_MEDIA_TYPE eMediaType, MS_U32 u32TimeMS);
IMPORT EN_MMSDK_MEDIA_TYPE TDAL_MPm_Player_GetMediaType(void);
IMPORT unsigned int TDAL_MPm_Player_GetOption(const EN_MMSDK_MEDIA_TYPE eMediaType, const EN_MPLAYER_OPTION_TYPE eOption, const unsigned int u32SubOption);
IMPORT MS_BOOL TDAL_MPm_Player_SetOption(const EN_MMSDK_MEDIA_TYPE eMediaType, const EN_MPLAYER_OPTION_TYPE eOption, const unsigned int u32Param);
IMPORT tTDAL_MP_Error TDAL_MPm_PictureGetMetadata(tTDAL_MP_MetaDataPict *pstMetadata);
IMPORT tTDAL_MP_Error TDAL_MPm_AudioGetMetadata(tTDAL_MP_MetaDataAudio *pstMetadata);
IMPORT tTDAL_MP_Error TDAL_MPm_AVGetMetadata(tTDAL_MP_MetaDataAv *pstMetadata);
IMPORT MS_BOOL TDAL_MPm_Player_PlayMediaFile(void);
IMPORT MS_BOOL TDAL_MPm_Player_CreateMediaFile(const EN_MMSDK_MEDIA_TYPE eMediaType, MPLAYER_MEDIA* const pMplayerMedia);
IMPORT MS_BOOL TDAL_MPm_Finalize(void);
IMPORT MS_BOOL TDAL_MPm_Player_Finalize(void);
IMPORT MS_BOOL TDAL_MPm_Player_Initialize(void);
#if defined(SDEC_GLUE_JPEG_TDAL_MP_PARTITION)
IMPORT bool TDAL_MPi_ReleasePool(void);
#endif

/*******************************************************************************
 * TDAL_MP SubRender                                                           *
 *******************************************************************************/
#if 0 //wait porting
MS_BOOL MM_SubRender_Create(void *arg);
MS_BOOL MM_SubRender_GetVideoSizeFromVdec(SBT_Rect *pVideoSize, void *arg);
void MM_SubRender_Open(void *arg);
void MM_SubRender_Close(void *arg);
MS_BOOL MM_SubRender_CreateWindow(U8 u8WinID, SBT_Rect *pRect, MS_BOOL bDisplayAble, void *arg);
MS_BOOL MM_SubRender_DestroyWindow(U8 u8WinID, void *arg);
void MM_SubRender_SetPalette(U8 u8WinID, RGBPaletteColor *pPaletteTbl, U16 u16PaletteSize, U8 u8Index, void *arg);
void MM_SubRender_Bitblt(U8 u8SrcWinID, U8 u8DstWinID, SBT_Rect *pSrcRect, SBT_Rect *pDstRect, void *arg);
void MM_SubRender_ClearWindowByIndex(U8 u8WinID, SBT_Rect *pRect, U8 u8ColorIndex, void *arg);
void MM_SubRender_ClearWindowByColor(U8 u8WinID, SBT_Rect *pRect, RGBPaletteColor *pColor, void *arg);
MS_BOOL MM_SubRender_GetWindowInfo(U8 u8WinID, GWinInfo *pInfo, void *arg);
MS_BOOL MM_SubRender_Lock(U8 u8WinID, EN_GRAPHIC_LOCK_TYPE enLockType, U8 **pu8Buffer, void *arg);
void MM_SubRender_UnLock(U8 u8WinID, void *arg);
MS_BOOL MM_SubRender_RenderFont(U8 *pu8TextBuff, U16 u16TextLength, U8 u8TransPaletteIdx, U8 *pu8DisplayBuffer, U16 u16DisplayWidth, U16 u16DisplayHeight, U16 u16DisplayPitch);
void MM_SubRender_Show(void *arg);
void MM_SubRender_Unshow(void *arg);
#endif
#endif /* TDAL_MP_P_H_ */
