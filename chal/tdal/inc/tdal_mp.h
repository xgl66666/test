/******************************************************************************
 *            COPYRIGHT 2004 IWEDIA TECHNOLOGIES        *
 ******************************************************************************
 *
 * MODULE NAME: TDAL MEDIA PLAYER
 *
 * FILE NAME: $URL: http://10.50.1.219/svncomedia/sources/chal/core/trunk/tdal/inc/tdal_mp.h $
 *      $Rev: 66 $
 *      $Date: 2008-10-14 10:29:19 +0200 (mar., 14 oct. 2008) $
 *
 * PUBLIC
 *
 * DESCRIPTION - Header of the TDAL Media Player
 *
 *****************************************************************************/

#ifndef _TDAL_MP_H_
#define _TDAL_MP_H_

/*******************************************************/
/*              Includes                               */
/*******************************************************/
#include "crules.h"

#ifdef __cplusplus
extern "C" {
#endif


/*******************************************************/
/*              Defines                                */
/*******************************************************/

/*******************************************************/
/*              Macros                                 */
/*******************************************************/


/*******************************************************/
/*              Typedefs                               */
/*******************************************************/
/* MEDiA_RENDERER TBOX ID management - Possible values from 0x080B0000-0x080BFFFF */

typedef uint32_t tTDAL_MP_METADATA_Time;

typedef enum
{
    /* TBOX ID management - Possible values from 0x045F0000-0x045FFFFF */
    eTDAL_MP_TRACE_GENERIC_ID =  0x045F0000,
    eTDAL_MP_TRACE_CONTROL_ID =  0x045F0100,
    eTDAL_MP_TRACE_STATUS_ID  =  0x045F0200,
    eTDAL_MP_TRACE_TRACKS_ID  =  0x045F0300,
    eTDAL_MP_TRACE_WINDOW_ID  =  0x045F0400
} tTDAL_MP_TraceId;


typedef enum {
    eTDAL_MP_NO_ERROR,
    eTDAL_MP_NOT_INIT_ERROR,
    eTDAL_MP_BAD_PARAMETER_ERROR,
    eTDAL_MP_NOT_SUPPORTED_ERROR,
    eTDAL_MP_NO_MEMORY_ERROR,
    eTDAL_MP_NB_HANDLE_MAX_ERROR, /* max session handle reached */
    eTDAL_MP_NOT_DONE_ERROR
}tTDAL_MP_Error;

typedef enum {
    eTDAL_MP_CAPS_BASIC = 0x00000000, /* always basic ops (Play, Stop) */
    eTDAL_MP_CAPS_POS   = 0x00000001, /* supports PosSet */
    eTDAL_MP_CAPS_SCALE = 0x00000002, /* can scale the video */
    eTDAL_MP_CAPS_SPEED = 0x00000004, /* supports changing playback speed */
    eTDAL_MP_CAPS_TRACK = 0x00000008 /* supports changing of tracks */
} tTDAL_MP_Capabilities;

typedef enum
{
    eTDAL_MP_STREAM_TYPE_UNKNOWN = -1,
    eTDAL_MP_STREAM_TYPE_VIDEO,
    eTDAL_MP_STREAM_TYPE_AUDIO,
    eTDAL_MP_STREAM_TYPE_DATA,
    eTDAL_MP_STREAM_TYPE_SUBTITLE
} tTDAL_MP_StreamType;

typedef enum
{
    eTDAL_MP_STREAM_CONTENT_INVALID, /* None  : Invalid type */
    eTDAL_MP_STREAM_CONTENT_MP1V,    /* Video : MPEG1 */
    eTDAL_MP_STREAM_CONTENT_MP2V,    /* Video : MPEG2 */
    eTDAL_MP_STREAM_CONTENT_VC1,     /* Video : VC-1 Decode SimpleMain/Advanced profiles */
    eTDAL_MP_STREAM_CONTENT_H264,    /* Video : H264 */
    eTDAL_MP_STREAM_CONTENT_MPEG4P2, /* Video : MPEG4 Part II */
    eTDAL_MP_STREAM_CONTENT_VMW, /* Video : MPEG4 Part II */
    eTDAL_MP_STREAM_CONTENT_MP1A,    /* Audio : MPEG1 Layer I */
    eTDAL_MP_STREAM_CONTENT_MP2A,    /* Audio : MPEG1 Layer II */
    eTDAL_MP_STREAM_CONTENT_MP3A,    /* Audio : MPEG1 Layer III */
    eTDAL_MP_STREAM_CONTENT_AC3,     /* Audio : AC3 */
    eTDAL_MP_STREAM_CONTENT_WMA,     /* Audio : AC3 */
    eTDAL_MP_STREAM_CONTENT_AAC,     /* Audio : Decode ADTS - AAC */
    eTDAL_MP_STREAM_CONTENT_HEAAC,   /* Audio : Decoder LOAS/LATM-AAC */
    eTDAL_MP_STREAM_CONTENT_DDPLUS,  /* Audio : Windows Media Audio */
    eTDAL_MP_STREAM_CONTENT_DTS,     /* Audio : Digital Theater Systems */
    eTDAL_MP_STREAM_CONTENT_PCM,     /* Audio : Pulse-code modulation */
    eTDAL_MP_STREAM_CONTENT_TTXT,    /* Teletext : Teletext */
    eTDAL_MP_STREAM_CONTENT_SUBT,    /* Subtitle : Subtitle */
    eTDAL_MP_STREAM_CONTENT_OTHER    /* Misc : Non identified */
} tTDAL_MP_ContentType;

typedef enum
{
    eTDAL_MP_STATUS_UNKNOWN  = 0x00000000, /* unknown status */
    eTDAL_MP_STATUS_PLAY     = 0x00000001, /* playing */
    eTDAL_MP_STATUS_STOP     = 0x00000002, /* stopped */
    eTDAL_MP_STATUS_FINISHED = 0x00000003  /* playback is finished */
} tTDAL_MP_Status;

typedef enum
{
    eTDAL_MP_PLAY_CTRL_MODE_NORMAL  = 0x00000000, /* normal playback */
    eTDAL_MP_PLAY_CTRL_MODE_REWIND  = 0x00000001, /* reverse playback */
    eTDAL_MP_PLAY_CTRL_MODE_LOOPING = 0x00000002  /* automatically restart
                                                 playback when
                                                 end-of-stream is
                                                 reached (gapless). */
} tTDAL_MP_PlayCtrlMode;

typedef enum
{
    eTDAL_MP_SUBTITLES_NORMAL = 0,
    eTDAL_MP_SUBTITLES_HH
} tTDAL_MP_SubtitleType;

typedef enum
{
    eTDAL_MP_EVENT_PLAYED,      /* play control done */
    eTDAL_MP_EVENT_STOPPED,     /* stop control done */
    eTDAL_MP_EVENT_FINISHED,    /* rendering of media finished */
    eTDAL_MP_EVENT_REWINDED,    /* rewinding of media finished */
    eTDAL_MP_EVENT_LOOP,        /* looping in media done */
    eTDAL_MP_EVENT_NEW_FRAME,   /* a new frame has been presented */
    eTDAL_MP_EVENT_UNDERFLOW,   /* Input buffer has reached low level threshold */
    eTDAL_MP_EVENT_NO_MEMORY,   /* Not enough memory for rendering process */
    eTDAL_MP_EVENT_STREAM_ERROR /* Bad incoming data stream has been detected */
} tTDAL_MP_Event;

typedef enum
{
    eTDAL_MP_SEEK_MODE_SET = 0, /* Beginning of file */
    eTDAL_MP_SEEK_MODE_CUR,     /* Current position of the stream pointer */
    eTDAL_MP_SEEK_MODE_END      /* End of file */
}tTDAL_MP_SeekMode;

typedef enum
{
    eTDAL_MP_OUTPUT_MAIN = 0x00000001, /* Main display destination */
    eTDAL_MP_OUTPUT_AUX  = 0x00000002  /* Auxiliary display destination (e.g. VCR or 2nd TV) */
} tTDAL_MP_Output;

typedef uint32_t tTDAL_MP_Handle;

typedef uint32_t (*tTDAL_MP_InputDataRead)(tTDAL_MP_Handle Handle,
                                       uint8_t *pucBuffer,
                                       uint32_t ulSize);
typedef uint32_t (*tTDAL_MP_InputDataSeek)(tTDAL_MP_Handle Handle,
                                       int32_t llOffset,
                                       tTDAL_MP_SeekMode eMode);
typedef uint32_t (*tTDAL_MP_InputDataLength)(tTDAL_MP_Handle Handle);

typedef void (*tTDAL_MP_EvtNotify)(tTDAL_MP_Handle Handle,
                               tTDAL_MP_Event eEvt,
                               const void *pCtx);


typedef struct
{
    uint32_t                         uiWidth;
    uint32_t                         uiHeight;
    uint16_t                         usBitPerPixel;
    bool                             bIsExifPresent;
    bool                             bIsSupported;
} tTDAL_MP_MetaDataPict;

typedef struct
{
    tTDAL_MP_METADATA_Time           stDuration;
    uint32_t                         uiBitrate;
    uint32_t                         uiFrequency;
    bool                             bIsID3Present;
    uint32_t                         uiFrameSize;
    uint32_t                         uiSamplePerframe;  
    uint32_t                         uiAudioDataStart;
    uint32_t                         uiAudioDataEnd;
    bool                             bIsSupported;
} tTDAL_MP_MetaDataAudio;

typedef struct
{
    tTDAL_MP_METADATA_Time       stDuration;
    bool                         bIsVideoPresent;
    bool                         bIsAudioPresent;
    struct
    {
        tTDAL_MP_ContentType  eType;
        uint32_t                    uiWidth;
        uint32_t                    uiHeight;
        int                         iFramerate;
        int                         iBitrate;
        bool                        bIsSupported;
    } Video;
    struct
    {
        tTDAL_MP_ContentType eType;
        int                        iSamplerate;
        int                        iChannels;
        int                        iBitrate;
        bool                       bIsSupported;
    } Audio;
} tTDAL_MP_MetaDataAv;


typedef struct
{
    uint32_t uiDummy;
} tTDAL_MP_InitParams;

typedef struct
{
    uint32_t dummy;

    tTDAL_MP_InputDataRead InputDataRead; /* callback function called when the
                                         consuming process of AV Player needs
                                         to read some data */
    tTDAL_MP_InputDataSeek InputDataSeek; /* callback function called when the
                                         consuming process of AV Player needs
                                         to seek in input data data stream */
    tTDAL_MP_InputDataLength InputDataLength; /* callback function called when the
                                         consuming process of AV Player needs
                                         to determine length of input data data stream */
    tTDAL_MP_EvtNotify     EvtNotify;     /* callback function called to notify
                                         events */
    void              *pCtx;          /* Pointer to subscriber�s specific data
                                         context. TDAL_MP does nothing with this
                                         data except pass it to the callback
                                         functions */
    tTDAL_MP_Output        eDefaultOutput;/* Default output destination of
                                         the audio/video stream. */
    uint8_t           *pPrivDataBuf;  /* Pointer to private data buffer that may
                                         be needed by some AV codecs. 
                                         In the case of Picture this pointer should be used for decoded picture*/
    uint32_t           uiPrivDataSize;/* Size of the private data buffer */

    char*              fileExtension; /* Extension of media file, needed for probing media file*/

    /* Stream media info */
    bool               bIsStreamed;   /* TRUE if it is a streamed media */
    tTDAL_MP_ContentType   eStreamVidType;/* Only relevant if bIsStreamed is TRUE */
    tTDAL_MP_ContentType   eStreamAudType;/* Only relevant if bIsStreamed is TRUE */
} tTDAL_MP_OpenParams;

typedef struct
{
    uint32_t uiTime; /* Start time position in the stream (in millisecond) */
    int32_t iSpeed;  /* Start speed. Values below 100 reduce playback speed
                        while values over 100 increase it. Specifying a value
                        of 0 has the effect of putting the playback in pause
                        mode without stopping the provider */
} tTDAL_MP_StartParams;

typedef struct
{
    int16_t usLeftSide;    /* in pixels from the left */
    int16_t usTopSide;     /* in lines from the top   */
    uint16_t usRightSide;  /* in pixels from the left */
    uint16_t usBottomSide; /* in lines from the top   */
} tTDAL_MP_WndPosition;

typedef struct
{
    tTDAL_MP_WndPosition stOutWnd;  /* "virtual" output window */
    tTDAL_MP_WndPosition stDispWnd; /* "real" display window   */
} tTDAL_MP_Wnd;

typedef struct _tTDAL_MP_ISO639LangCode
{
    char cTrigram[3]; /* That is a string of 3 letters. Such a mnemonic is use
                         to define languages according to ISO standard.
                         see ISO 639 part 2. */
} tTDAL_MP_ISO639LangCode;

typedef struct
{
    tTDAL_MP_ContentType    eType;  /* Type of audio                         */
    tTDAL_MP_ISO639LangCode stLang; /* ISO language of the audio track.      */
    uint16_t            usIdx;  /* Index of the audio track in the list. */
} tTDAL_MP_AudioTrack;

typedef struct
{
    tTDAL_MP_SubtitleType   eType;               /* Type of subtitle.                */
    tTDAL_MP_ISO639LangCode stLang;              /* ISO language of the track        */
    uint16_t            usCompositionPageID; /* Composition page ID of the track */
    uint16_t            usAncillaryPageID;   /* Ancillary page ID of the track   */
    uint16_t            usIdx;               /* Index of the track in the list   */
} tTDAL_MP_SubtitleTrack;

typedef struct
{
    tTDAL_MP_ContentType    eType;  /* Type of video                         */
    uint16_t            usIdx;  /* Index of the video track in the list. */
} tTDAL_MP_VideoTrack;

typedef struct
{
    uint16_t usTrackCount; /* Number of tracks in the list */
    void    *pTrackList;   /* Pointer to the track list that must be cast in the
                              track video, audio or subtitle type structure */
} tTDAL_MP_TrackList;

/*******************************************************/
/*              Variables Declarations (IMPORT)        */
/*******************************************************/

/*******************************************************/
/*              Functions Declarations                 */
/*******************************************************/
/*******************************************************/
/*  Generic Part */
/*******************************************************/
IMPORT tTDAL_MP_Error TDAL_MP_Init(tTDAL_MP_InitParams *pstInitParams);
IMPORT tTDAL_MP_Error TDAL_MP_Terminate (void);
IMPORT char      *TDAL_MP_RevisionGet(void);
IMPORT tTDAL_MP_Error TDAL_MP_Open(tTDAL_MP_OpenParams *pstParams, tTDAL_MP_Handle *pHandle);
IMPORT tTDAL_MP_Error TDAL_MP_Close(tTDAL_MP_Handle Handle);
IMPORT tTDAL_MP_Error TDAL_MP_CapabilityGet(tTDAL_MP_Handle Handle, tTDAL_MP_Capabilities *pstCaps);
IMPORT tTDAL_MP_Error TDAL_MP_PictureGetMetadata(tTDAL_MP_Handle Handle, tTDAL_MP_MetaDataPict *pstMetadata);
IMPORT tTDAL_MP_Error TDAL_MP_AudioGetMetadata(tTDAL_MP_Handle Handle, tTDAL_MP_MetaDataAudio *pstMetadata);
IMPORT tTDAL_MP_Error TDAL_MP_AVGetMetadata(tTDAL_MP_Handle Handle, tTDAL_MP_MetaDataAv *pstMetadata);
IMPORT tTDAL_MP_Error TDAL_MP_OutputSet(tTDAL_MP_Handle Handle, uint32_t uiOutputMask);

/*******************************************************/
/*  Control Part                                       */
/*******************************************************/
IMPORT tTDAL_MP_Error TDAL_MP_Start(tTDAL_MP_Handle Handle, tTDAL_MP_StartParams *pstParams);
IMPORT tTDAL_MP_Error TDAL_MP_Stop(tTDAL_MP_Handle Handle);
IMPORT tTDAL_MP_Error TDAL_MP_PlayCtrlModeSet(tTDAL_MP_Handle Handle, uint32_t uiMode);
IMPORT tTDAL_MP_Error TDAL_MP_PosSet(tTDAL_MP_Handle Handle, uint32_t uiSec);
IMPORT tTDAL_MP_Error TDAL_MP_PosGet(tTDAL_MP_Handle Handle, uint32_t *puiSec);
IMPORT tTDAL_MP_Error TDAL_MP_SpeedSet(tTDAL_MP_Handle Handle, int32_t uiSpeed);
IMPORT tTDAL_MP_Error TDAL_MP_SpeedGet(tTDAL_MP_Handle Handle, int32_t *puiSpeed);
/*******************************************************/
/*  Status Part                                        */
/*******************************************************/
IMPORT tTDAL_MP_Error TDAL_MP_StatusGet(tTDAL_MP_Handle Handle, tTDAL_MP_Status *pstStatus);
IMPORT tTDAL_MP_Error TDAL_MP_LengthGet(tTDAL_MP_Handle Handle, uint32_t *puiSec);
/*******************************************************/
/*  Windowing Part                                     */
/*******************************************************/
IMPORT tTDAL_MP_Error TDAL_MP_ScaleWndSet(tTDAL_MP_Handle Handle, tTDAL_MP_Output eOutput, tTDAL_MP_Wnd *pstWnd);
IMPORT tTDAL_MP_Error TDAL_MP_ScaleWndGet(tTDAL_MP_Handle Handle, tTDAL_MP_Output eOutput, tTDAL_MP_Wnd *pstWnd);
IMPORT tTDAL_MP_Error TDAL_MP_ResolutionGet(tTDAL_MP_Handle Handle,
                                    uint16_t *pusWidth,
                                    uint16_t *pusHeight);
/*******************************************************/
/*  Track selection Part                               */
/*******************************************************/
IMPORT tTDAL_MP_Error TDAL_MP_TracksGet(tTDAL_MP_Handle      Handle,
                                tTDAL_MP_StreamType  eType,
                                tTDAL_MP_TrackList **ppstList,
                                uint16_t        *pusSelectedIdx);
IMPORT tTDAL_MP_Error TDAL_MP_TracksRelease(tTDAL_MP_Handle     Handle,
                                    tTDAL_MP_TrackList *pstList);
IMPORT tTDAL_MP_Error TDAL_MP_TrackSelect(tTDAL_MP_Handle Handle,
                                  tTDAL_MP_StreamType eType,
                                  uint16_t usSelectedIdx);
#if defined(SDEC_GLUE_JPEG_TDAL_MP_PARTITION)
/*******************************************************/
/*  Alloc and free functions
 *
 */
/*******************************************************/
IMPORT tTDAL_MP_Error TDAL_MP_Malloc(uint32_t size, void ** p);
IMPORT tTDAL_MP_Error TDAL_MP_Free(void * p);
#endif


#ifdef __cplusplus
}
#endif

#endif /*_MEDIA_RENDERER_H_*/

