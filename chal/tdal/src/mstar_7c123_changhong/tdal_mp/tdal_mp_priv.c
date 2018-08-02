
#include "stdio.h"
#include <sys/stat.h>
#include <pthread.h>
#include <assert.h>

#include "crules.h"
#include "tdal_mp.h"
#include "tdal_mp_p.h"
#include "tdal_common.h"
#include "tbox.h"

#include "MsTypes.h"
#include "MsCommon.h"
#include "mmsdk_interface_def.h"
#include "mmsdk_interface.h"
#include "msapi_MM_Common.h"
#include "msapi_MM_Display.h"
#include "porting_sysinfo.h"
#include "MsFS.h"
#include "MM_Player_Def.h"
#include "MApi_MEMConfig_Interface.h"
#include "apiAUDIO.h"
mTBOX_SET_MODULE(eTDAL_MP);

/********************************************************/
/*              Defines                                 */
/********************************************************/
#define WAIT_INIT_OK_TIMEOUT 5000
/********************************************************/
/*              Typedefs                                */
/********************************************************/

/********************************************************/
/*              Local Module Variables (MODULE)         */
/********************************************************/

const Z_File_Ext_Info _gstZmmSupportExtTable[42] =
{
    //Auio support Suffixes name
    {"AAC",      E_MMSDK_MEDIA_TYPE_MUSIC},
    {"M4A",      E_MMSDK_MEDIA_TYPE_MUSIC},
    {"MP3",      E_MMSDK_MEDIA_TYPE_MUSIC},
    {"WMA",     E_MMSDK_MEDIA_TYPE_MUSIC},
    {"OGG",     E_MMSDK_MEDIA_TYPE_MUSIC},
    {"WAV",     E_MMSDK_MEDIA_TYPE_MUSIC},
    {"PCM",     E_MMSDK_MEDIA_TYPE_MUSIC},
    {"AC3",     E_MMSDK_MEDIA_TYPE_MUSIC},
    {"FLAC",     E_MMSDK_MEDIA_TYPE_MUSIC},

    //Video support Suffixes name
    {"MP4",     E_MMSDK_MEDIA_TYPE_MOVIE},
    {"MPG",     E_MMSDK_MEDIA_TYPE_MOVIE},
    {"AVI",      E_MMSDK_MEDIA_TYPE_MOVIE},
    {"DAT",     E_MMSDK_MEDIA_TYPE_MOVIE},
    {"VOB",     E_MMSDK_MEDIA_TYPE_MOVIE},
    {"MOV",     E_MMSDK_MEDIA_TYPE_MOVIE},
    {"MKV",     E_MMSDK_MEDIA_TYPE_MOVIE},
    {"DIV",      E_MMSDK_MEDIA_TYPE_MOVIE},
    {"DIVX",    E_MMSDK_MEDIA_TYPE_MOVIE},
    {"TS",        E_MMSDK_MEDIA_TYPE_MOVIE},
    {"MPEG",    E_MMSDK_MEDIA_TYPE_MOVIE},
    {"DAT",      E_MMSDK_MEDIA_TYPE_MOVIE},
    {"MPA",      E_MMSDK_MEDIA_TYPE_MOVIE},
    {"M2TS",    E_MMSDK_MEDIA_TYPE_MOVIE},
    {"M2T1",    E_MMSDK_MEDIA_TYPE_MOVIE},
    {"MPE",      E_MMSDK_MEDIA_TYPE_MOVIE},
    {"TRP",      E_MMSDK_MEDIA_TYPE_MOVIE},
    {"TP",        E_MMSDK_MEDIA_TYPE_MOVIE},
    {"3GP",     E_MMSDK_MEDIA_TYPE_MOVIE},
    {"RMVB",     E_MMSDK_MEDIA_TYPE_MOVIE},
    {"RM",     E_MMSDK_MEDIA_TYPE_MOVIE},
    {"FLV",     E_MMSDK_MEDIA_TYPE_MOVIE},
    {"ASF",     E_MMSDK_MEDIA_TYPE_MOVIE},
    {"WMV",     E_MMSDK_MEDIA_TYPE_MOVIE},
    {"WEBM",     E_MMSDK_MEDIA_TYPE_MOVIE},
    {"SWF",     E_MMSDK_MEDIA_TYPE_MOVIE},

    //Photo support Suffixes name
    {"BMP",     E_MMSDK_MEDIA_TYPE_PHOTO},
    {"PNG",     E_MMSDK_MEDIA_TYPE_PHOTO},
    {"JPG",      E_MMSDK_MEDIA_TYPE_PHOTO},
    {"JPEG",    E_MMSDK_MEDIA_TYPE_PHOTO},
    {"JPE",      E_MMSDK_MEDIA_TYPE_PHOTO},
    {"GIF",      E_MMSDK_MEDIA_TYPE_PHOTO},
    {"MPO",      E_MMSDK_MEDIA_TYPE_PHOTO},
};

/********************************************************/
/*              Global Variables (GLOBAL/IMPORT)        */
/********************************************************/
IMPORT TDAL_mutex_id _gs32MutexOpenStop;
tTDAL_MP_InputDataRead  _pInputDataRead = NULL;
tTDAL_MP_InputDataSeek  _pInputDataSeek = NULL;
tTDAL_MP_InputDataLength _pInputDataLength = NULL;
/*              Local File Variables (LOCAL)            */
/********************************************************/
LOCAL MS_BOOL m_bInitialize = FALSE;
LOCAL MS_BOOL _bIsReadyToPlay = FALSE;
LOCAL MEDIAITEM _gItem[MMSDK_MEDIA_TYPE_NUM] = {0, 0, 0};
LOCAL ST_MMSDK_CREATE_MEDIA_INFO _gstMedia;
LOCAL ST_RECT _gstRect[MMSDK_MEDIA_TYPE_NUM];
LOCAL MS_BOOL _gbExit[MMSDK_MEDIA_TYPE_NUM]  = {FALSE, FALSE, FALSE};
LOCAL EN_MPLAYER_FILE_MODE _geStrmMode[MMSDK_MEDIA_TYPE_NUM] = {MPLAYER_FILE_MODE, MPLAYER_FILE_MODE, MPLAYER_FILE_MODE};
LOCAL MS_U64 _gu64Size[MMSDK_MEDIA_TYPE_NUM] = {0, 0, 0};
LOCAL void *_gpStream[E_MPLAYER_MEDIA_STREAM_NUM] = {NULL, NULL, NULL, NULL, NULL, NULL};
LOCAL WAVE_HDR _gstWaveHdr;
LOCAL MS_U8 _gu8AudioPara[MAX_PARA_SIZE];
LOCAL MS_BOOL _gbUseNewAudioMode = TRUE;
LOCAL MS_BOOL _gbKTVMode = FALSE;
LOCAL MS_BOOL _gbBackwardRePlayMode = FALSE;
LOCAL MS_U32 _gu32TotalTime = 0;
LOCAL MS_U32 _gu32StartTime = 0;
LOCAL EN_SEAMLESS_MODE _geSeamlessMode = MPLAYER_SEAMLESS_NONE;
LOCAL MS_U8 _gu8IsRepeat = FALSE;
LOCAL MS_U8 _gu8Tmp_b = 0;
LOCAL MS_U8 _gu8Tmp_a = 0;
LOCAL MS_BOOL _gbPlay[MMSDK_MEDIA_TYPE_NUM] = {FALSE, FALSE, FALSE};
LOCAL Task_Info _gThreadEvtProcess[MMSDK_MEDIA_TYPE_NUM] =
{
    {-1, E_TASK_PRI_MEDIUM, NULL, TH_EVT_PROC_STACK_SIZE, "threadEvt1"},
    {-1, E_TASK_PRI_MEDIUM, NULL, TH_EVT_PROC_STACK_SIZE, "threadEvt2"},
    {-1, E_TASK_PRI_MEDIUM, NULL, TH_EVT_PROC_STACK_SIZE, "threadEvt3"},    
};
LOCAL MS_U64 _gu64CurPostn[MMSDK_MEDIA_TYPE_NUM] = {0, 0, 0};
LOCAL MS_S32 _gs32MediaEvtGrp = -1;
// for gif
#if (MM_PHOTO_PATH == 0x2)
LOCAL MS_U32 _gu32GifAddrOffset = 0;
#endif
LOCAL MS_BOOL _gbEnableMotionGif = FALSE;
LOCAL EN_MPLAYER_SUB_TYPE _geAudioType = MPLAYER_SUBTYPE_UNKNOWN;
LOCAL MS_BOOL _gbMusicBackwardRePlayMode = TRUE;  //for music
LOCAL EN_MMSDK_TRICK_MODE _geTrickMod[MMSDK_MEDIA_TYPE_NUM] = {E_MMSDK_TRICK_INVALID, E_MMSDK_TRICK_INVALID, E_MMSDK_TRICK_INVALID};
LOCAL MS_U32 _gu32SaveFilePos = 0;
LOCAL MS_U64 _gu64SubCurPostn = 0;
LOCAL MS_U64 _gu64AudioCurPostn = 0;
LOCAL MS_U64 _gu64Audio2CurPostn = 0;
LOCAL EN_MMSDK_ROTATE_ANGLE _geRotaAng = E_MMSDK_ROTATE_0;
LOCAL EN_MMSDK_VIEW_ZOOM_FACTOR _geZoomFctor = E_MMSDK_VIEW_ZOOM_1X;
LOCAL MS_U32 _gu32MetalLen = 0;
LOCAL MS_U8 *_gpu8Buffer = NULL;
LOCAL MS_U16 *_gpu16UnicdBuffer = NULL;
//for ring buffer info
IMPORT MS_U32 _gu32VdplayerReadTimerOut;
IMPORT MS_BOOL _gbFastInit;
IMPORT MS_U32 _gu32InitDataSize;
IMPORT MS_S32 _gs32MinBufferSec;
IMPORT MS_S32 _gs32MaxBufferSec;
LOCAL EN_MPLAYER_NOTIFY_TYPE _geVideoNotifyType = MPLAYER_NOTIFY_NULL;
LOCAL EN_MPLAYER_NOTIFY_TYPE _geMusicNotifyType = MPLAYER_NOTIFY_NULL;
LOCAL EN_MPLAYER_NOTIFY_TYPE _gePhotoNotifyType = MPLAYER_NOTIFY_NULL;

LOCAL MS_BOOL _bIsFileSupport = TRUE;
LOCAL MS_BOOL _bIsAudioSupport = TRUE;

LOCAL TDAL_mutex_id _gs32MutexOpenStop = NULL;
LOCAL TDAL_mutex_id _gs32Mutex = NULL;

//for debug info.....
IMPORT EN_MMSDK_DBG_LEVEL _geDebugLevel;
/********************************************************/
/*              Macros                                  */
/********************************************************/
#define MM_SET_PHOTO_NOTIFY_TYPE(PhotoNotiyType) \
            do{ \
                TDAL_LockMutex(_gs32Mutex); \
                _gePhotoNotifyType = PhotoNotiyType; \
                TDAL_UnlockMutex(_gs32Mutex); \
                MsOS_SetEvent(_gs32MediaEvtGrp, MM_EVENT_PHOTO_NOTIFY); \
            }while(0)
            
#define MM_SET_MUSIC_NOTIFY_TYPE(MusicNotiyType) \
                do{ \
                    TDAL_LockMutex(_gs32Mutex); \
                    _geMusicNotifyType = MusicNotiyType; \
                    TDAL_UnlockMutex(_gs32Mutex); \
                    MsOS_SetEvent(_gs32MediaEvtGrp, MM_EVENT_MUSIC_NOTIFY); \
                }while(0)
            
#define MM_SET_VIDEO_NOTIFY_TYPE(VideoNotiyType) \
                do{ \
                    TDAL_LockMutex(_gs32Mutex); \
                    _geVideoNotifyType = VideoNotiyType; \
                    TDAL_UnlockMutex(_gs32Mutex); \
                    MsOS_SetEvent(_gs32MediaEvtGrp, MM_EVENT_VIDEO_NOTIFY); \
                }while(0)

#define B_ENDIAN2L_ENDIAN16(ptr)                                \
                    _gu8Tmp_a = *((MS_U8*)(ptr)+1);          \
                    *((MS_U8*)(ptr)+1) = *(MS_U8*)(ptr);      \
                    *(MS_U8*)(ptr) = _gu8Tmp_a;              \
                
#define B_ENDIAN2L_ENDIAN32(ptr)                                \
                    _gu8Tmp_a = *((MS_U8*)(ptr));            \
                    _gu8Tmp_b = *((MS_U8*)(ptr)+1);          \
                    *((MS_U8*)(ptr)) =*((MS_U8*)(ptr)+3);     \
                    *((MS_U8*)(ptr)+1) =*((MS_U8*)(ptr)+2);   \
                    *((MS_U8*)(ptr)+2) = _gu8Tmp_b;          \
                    *((MS_U8*)(ptr)+3) = _gu8Tmp_a;          \

/********************************************************/
static void _TDAL_Photo_EvtProcess(MS_U32 argc, void *arg);
static void _TDAL_Music_EvtProcess(MS_U32 argc, void *arg);
static void _TDAL_Video_EvtProcess(MS_U32 argc, void *arg);
static void (*_gpEvtProcess[MMSDK_MEDIA_TYPE_NUM])(MS_U32, void*) = {_TDAL_Photo_EvtProcess, _TDAL_Music_EvtProcess, _TDAL_Video_EvtProcess};

#if defined(MSOS_TYPE_ECOS)
static void _MM_PollingThreadStatus(Task_Info stTaskInfo)
{
    TaskStatus eTaskStatus;
    while(1)
    {
        if(!MsOS_GetTaskStatus(&stTaskInfo,&eTaskStatus))
        {
            mTBOX_TRACE((kTBOX_NIV_WARNING, "Error!! MsOS_GetTaskStatus() fail!! \n"));
            break;
        }

        if(eTaskStatus == E_TASK_NOT_EXIST)
            break;
        else
        {
            mTBOX_TRACE((kTBOX_NIV_1, "\33[32m Wait for Task#%x %s termination \33[m \n",
                stTaskInfo.iId,stTaskInfo.szName));
            MsOS_DelayTask(1);
        }
    }
}
#endif
static tTDAL_MP_ContentType _MM_VdecConvert(EN_MMSDK_AUDIO_CODEC eVdec)
{
    switch (eVdec)
    {
        case E_MMSDK_VIDEO_CODEC_MPEG4:
            return eTDAL_MP_STREAM_CONTENT_MPEG4P2;
        case E_MMSDK_VIDEO_CODEC_H264:
            return eTDAL_MP_STREAM_CONTENT_H264;
        case E_MMSDK_VIDEO_CODEC_MPG:
            return eTDAL_MP_STREAM_CONTENT_MP1V;
        case E_MMSDK_VIDEO_CODEC_VC1:
            return eTDAL_MP_STREAM_CONTENT_VC1;            
        case E_MMSDK_VIDEO_CODEC_RM:
        case E_MMSDK_VIDEO_CODEC_TS:
        case E_MMSDK_VIDEO_CODEC_AVS:
        case E_MMSDK_VIDEO_CODEC_FLV:
        case E_MMSDK_VIDEO_CODEC_MVC:
        case E_MMSDK_VIDEO_CODEC_VP6:
        case E_MMSDK_VIDEO_CODEC_VP8:
        case E_MMSDK_VIDEO_CODEC_VP9:
        case E_MMSDK_VIDEO_CODEC_HEVC_DV:
        case E_MMSDK_VIDEO_CODEC_HEVC:
        case E_MMSDK_VIDEO_CODEC_MJPEG:
            return eTDAL_MP_STREAM_CONTENT_OTHER;
        case E_MMSDK_VIDEO_CODEC_UNKNOWN:
        default:
            mTBOX_TRACE((kTBOX_NIV_WARNING, "Not Supported Video Codec\n"));
            return eTDAL_MP_STREAM_CONTENT_INVALID;
    }    
}
static MS_BOOL _MM_CheckVdecSupport(EN_MMSDK_VIDEO_CODEC eVdec)
{
    MS_BOOL bRet = FALSE;
    
    switch (eVdec)
    {
        case E_MMSDK_VIDEO_CODEC_MPEG4:
        case E_MMSDK_VIDEO_CODEC_MJPEG:
        case E_MMSDK_VIDEO_CODEC_H264:
        case E_MMSDK_VIDEO_CODEC_RM:
        case E_MMSDK_VIDEO_CODEC_TS:
        case E_MMSDK_VIDEO_CODEC_MPG:
        case E_MMSDK_VIDEO_CODEC_VC1:
        case E_MMSDK_VIDEO_CODEC_AVS:
        case E_MMSDK_VIDEO_CODEC_FLV:
        case E_MMSDK_VIDEO_CODEC_MVC:
        case E_MMSDK_VIDEO_CODEC_VP6:
        case E_MMSDK_VIDEO_CODEC_VP8:
        case E_MMSDK_VIDEO_CODEC_VP9:
            bRet = TRUE;
            break;
        case E_MMSDK_VIDEO_CODEC_HEVC_DV:
        case E_MMSDK_VIDEO_CODEC_HEVC:
        case E_MMSDK_VIDEO_CODEC_UNKNOWN:
        default:
            mTBOX_TRACE((kTBOX_NIV_WARNING, "Not Supported Video Codec\n"));
            bRet = FALSE;
            break;
    }
    
    return bRet;
}
static tTDAL_MP_ContentType _MM_AdecConvert(EN_MMSDK_AUDIO_CODEC eAdec)
{
  
    //eTDAL_MP_STREAM_CONTENT_MP1A,    /* Audio : MPEG1 Layer I */
    //eTDAL_MP_STREAM_CONTENT_HEAAC,   /* Audio : Decoder LOAS/LATM-AAC */
    switch(eAdec)
    {
        case E_MMSDK_AUDIO_CODEC_MP3:
            return eTDAL_MP_STREAM_CONTENT_MP3A;
        case E_MMSDK_AUDIO_CODEC_MPEG:
            return eTDAL_MP_STREAM_CONTENT_MP2A;
        case E_MMSDK_AUDIO_CODEC_XPCM:
        case E_MMSDK_AUDIO_CODEC_PCM:
        case E_MMSDK_AUDIO_CODEC_PCM_MULAW:
        case E_MMSDK_AUDIO_CODEC_ADPCM:
            return eTDAL_MP_STREAM_CONTENT_PCM;          
        case E_MMSDK_AUDIO_CODEC_AAC:
            return eTDAL_MP_STREAM_CONTENT_AAC;
        case E_MMSDK_AUDIO_CODEC_AC3:
            return eTDAL_MP_STREAM_CONTENT_AC3;
        case E_MMSDK_AUDIO_CODEC_AC3P:
            return eTDAL_MP_STREAM_CONTENT_DDPLUS;
        case E_MMSDK_AUDIO_CODEC_DTS:
        case E_MMSDK_AUDIO_CODEC_DTSLBR:
        case E_MMSDK_AUDIO_CODEC_DTSHD:
        case E_MMSDK_AUDIO_CODEC_DTSHD_MA:
        case E_MMSDK_AUDIO_CODEC_DTSHD_HRA:
            return eTDAL_MP_STREAM_CONTENT_DTS;
        case E_MMSDK_AUDIO_CODEC_WMA:
        case E_MMSDK_AUDIO_CODEC_WMA1:
        case E_MMSDK_AUDIO_CODEC_WMA2:
        case E_MMSDK_AUDIO_CODEC_WMA3:
            return eTDAL_MP_STREAM_CONTENT_WMA;        
        case E_MMSDK_AUDIO_CODEC_AMR_NB:
        case E_MMSDK_AUDIO_CODEC_AMR_WB:
        case E_MMSDK_AUDIO_CODEC_FLAC:
        case E_MMSDK_AUDIO_CODEC_COOK:
        case E_MMSDK_AUDIO_CODEC_VORBIS:
        case E_MMSDK_AUDIO_CODEC_DRA:
            return eTDAL_MP_STREAM_CONTENT_OTHER;
        case E_MMSDK_AUDIO_CODEC_UNKNOWN:
        default:
            mTBOX_TRACE((kTBOX_NIV_WARNING, "Not Supported Audio Codec\n"));
            return eTDAL_MP_STREAM_CONTENT_INVALID;
    }
}
static MS_BOOL _MM_CheckAdecSupport(EN_MMSDK_AUDIO_CODEC eAdec)
{
    MS_BOOL bRet = FALSE;
    switch(eAdec)
    {
        case E_MMSDK_AUDIO_CODEC_AMR_NB:
        case E_MMSDK_AUDIO_CODEC_AMR_WB:
        case E_MMSDK_AUDIO_CODEC_FLAC:
        case E_MMSDK_AUDIO_CODEC_MP3:
        case E_MMSDK_AUDIO_CODEC_MPEG:
        case E_MMSDK_AUDIO_CODEC_COOK:
        case E_MMSDK_AUDIO_CODEC_VORBIS:
        case E_MMSDK_AUDIO_CODEC_DRA:
        case E_MMSDK_AUDIO_CODEC_XPCM:
        case E_MMSDK_AUDIO_CODEC_PCM:
        case E_MMSDK_AUDIO_CODEC_PCM_MULAW:
        case E_MMSDK_AUDIO_CODEC_ADPCM:
            bRet = TRUE;
            break;
        case E_MMSDK_AUDIO_CODEC_AAC:
        case E_MMSDK_AUDIO_CODEC_AC3:
        case E_MMSDK_AUDIO_CODEC_AC3P:
        case E_MMSDK_AUDIO_CODEC_DTS:
        case E_MMSDK_AUDIO_CODEC_DTSLBR:
        case E_MMSDK_AUDIO_CODEC_DTSHD:
        case E_MMSDK_AUDIO_CODEC_DTSHD_MA:
        case E_MMSDK_AUDIO_CODEC_DTSHD_HRA:
        case E_MMSDK_AUDIO_CODEC_WMA:
        case E_MMSDK_AUDIO_CODEC_WMA1:
        case E_MMSDK_AUDIO_CODEC_WMA2:
        case E_MMSDK_AUDIO_CODEC_WMA3:
        case E_MMSDK_AUDIO_CODEC_UNKNOWN:
        default:
            mTBOX_TRACE((kTBOX_NIV_WARNING, "Not Supported Audio Codec\n"));
            bRet = FALSE;
            break;
    }
    return bRet;
}

void TDAL_MPm_UsbRemoved(void)
{
    int i =  TDAL_MPi_GetRunningMediaDescriptor();
    if (i >= 0)
    {
        TDAL_MPm_Stop();
    }
}

static MS_BOOL TDAL_MM_FreeAudDec(void)
{
    //release audio decode system
    if(PT_SYS_SetAudDecID(AU_DEC_INVALID) == FALSE)
    {
        mTBOX_TRACE((kTBOX_NIV_WARNING, "clear audio decoder ID failed!\n"));
        return FALSE;
    }

    return TRUE;
}

static MS_U16 _TDAL_ConvBig5ToUnicode(const MS_U16 u16Big5Code)
{
    MS_U16 u16Unicode = INVALID_UNICODE;
    if ((u16Big5Code & 0x00FF) == 0x0000)
    {
        u16Unicode = u16Big5Code;
    }

    return u16Unicode;
}

static MS_U16 _TDAL_UTF8ToUCS2(const MS_U8 * const pu8Str, MS_U16* const pu16Str, const MS_U16 srcByteLen, const MS_U16 dstWideCharLen)
{
    //see reference from http://en.wikipedia.org/wiki/Utf-8
    MS_U8 u8Value;
    MS_U16 i, j;

    if ((pu8Str == 0) || (srcByteLen == 0) || (pu16Str == 0) || (dstWideCharLen <= 1))
    {
        return 0;
    }

    i = 0;
    j = 0;
    do
    {
        if ((i >= srcByteLen) ||(j >= (dstWideCharLen - 1)))
        {
            break;
        }
        u8Value = pu8Str[i];
        if (u8Value == 0)
        {
            break;
        }
        if ((u8Value & 0x80) == 0)
        {
            // 0zzzzzzz(00-7F) => 00000000 0zzzzzzz
            pu16Str[j] = (MS_U16)(u8Value & 0x7F);
        }
        else if(((u8Value & 0xE0) == 0xC0) && ((i + 1) < srcByteLen))
        {
            // 110yyyyy(C2-DF) 10zzzzzz(80-BF) => 00000yyy yyzzzzzz
            pu16Str[j] = (MS_U16)(((u8Value & 0x1F) << 6) | (pu8Str[i+1] & 0x3F));
            i += 1;
        }
        else if(((u8Value & 0xF0) == 0xE0) && ((i + 2) < srcByteLen))
        {
            // 1110xxxx(E0-EF) 10yyyyyy 10zzzzzz => xxxxyyyy yyzzzzzz
            pu16Str[j] = (MS_U16)(((u8Value & 0xF) << 12) | ((pu8Str[i+1] & 0x3F) << 6) | (pu8Str[i+2] & 0x3F));
            i += 2;
        }
        else if(((u8Value & 0xF8) == 0xF0) && ((i + 3) < srcByteLen))
        {
            MS_U32 u32Code = 0;
            /////11110XX 10XXXXXX ....
            u32Code = (MS_U16)(((u8Value & 0xF7) << 18) | ((pu8Str[i+1] & 0x3F) << 12) | ((pu8Str[i+2] & 0x3F) << 6) | (pu8Str[i+3] & 0x3F));
            u32Code -= 0x10000;
            pu16Str[j+1]  = (MS_U16)(0xd800 | (u32Code >> 10));
            pu16Str[j] = (MS_U16)(0xdc00 | (u32Code & 0x3ff));
            j += 1;
            i += 3;
        }
        else
        {
            // Unknown, skip
            j--;
        }

        i++;
        j++;
    }
    while(1);

    return j * 2;
}

static MS_S8 *_TDAL_itoa(MS_U32 num, MS_S8 *str, const MS_U32 radix)
{
    const MS_U8 index[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    MS_U32 unum;
    MS_U32 i = 0, j, k;

    unum = num;
    do
    {
        str[i++] = index[unum%radix];
        unum /= radix;
    }
    while(unum);

    str[i] = '\0';
    if ((i - 1) / 2 == 0)
        k = 1;
    else
        k = (i - 1) / 2;

    for (j = 0; j < k; j++)
    {
        num = str[j];
        str[j] = str[i-j-1];
        str[i-j-1] = num;
    }

    return str;
}

static MS_BOOL _TDAL_GetOption_CharEncConv(EN_PLAYER_STRING_FORMAT eStrFormt, MS_BOOL bEndianChange)
{
    MS_U32 u32Ret = 0;
    MS_U32 u32UnicdBytes = 0;

    if (eStrFormt == E_PLAYER_STRING_FORMAT_UTF8)
    {
        mTBOX_TRACE((kTBOX_NIV_1, "----------> info: %d ---UTF8-----\n", (MS_U8)_gu32MetalLen));
        u32UnicdBytes = _TDAL_UTF8ToUCS2(_gpu8Buffer, _gpu16UnicdBuffer, _gu32MetalLen, 0xffff);
        u32Ret = u32UnicdBytes;
    }
    else if (eStrFormt == E_PLAYER_STRING_FORMAT_ASCII)
    {
        MS_U32 i = 0;
        MS_U32 j = 0;
        MS_S16 s16GBCode = 0;
        MS_U16 u16Unicode = 0;
        mTBOX_TRACE((kTBOX_NIV_1, "----------> info: %d  bytes---ASCII-----\n", (int)_gu32MetalLen));
        for (j = 0, i = 0; i < _gu32MetalLen; i++, j++)
        {
            u32UnicdBytes++;
            s16GBCode = *((MS_S8 *)(_gpu8Buffer + i));
            if (s16GBCode > 0) //basic ASCII code.
            {
                u16Unicode = _TDAL_ConvBig5ToUnicode((MS_U16)s16GBCode);
                mTBOX_TRACE((kTBOX_NIV_1, "----English %d\n", s16GBCode));
            }
            else            //chinese char
            {
                MS_U16 u16EndianChange = *(MS_U16 *)(_gpu8Buffer + i);
                if(bEndianChange)
                    u16EndianChange = (u16EndianChange >> 8) + ((u16EndianChange & 0xff) << 8);

                u16Unicode = _TDAL_ConvBig5ToUnicode(u16EndianChange);
                mTBOX_TRACE((kTBOX_NIV_1, "----Chinese %d\n", u16EndianChange));
                mTBOX_TRACE((kTBOX_NIV_1, "----Chinese Unicode %x\n", u16Unicode));

                i += 1;
            }

            _gpu16UnicdBuffer[j] = u16Unicode;
        }

        u32Ret = u32UnicdBytes * 2;
    }
    else  //unicode
    {
        u32Ret = _gu32MetalLen;
        memcpy(_gpu16UnicdBuffer, _gpu8Buffer, _gu32MetalLen);
        mTBOX_TRACE((kTBOX_NIV_1, "-----> len is %d-----UNICODE\n", (int)_gu32MetalLen));
    }

    return u32Ret;
}

static MS_BOOL _TDAL_MM_GetSubtitleTrackInfo(MEDIAITEM u32Item, ST_SUBTITLE_TRACK_INFO * pTrackInfo)
{
    ST_MMSDK_PARAM stParam;
    MS_BOOL bRet = FALSE;
    MS_U32 u32SubtitleType = 0;
    memset(&stParam, 0, sizeof(ST_MMSDK_PARAM));
    stParam.u32Option = E_MMSDK_OPTION_MOVIE_INFO;
    stParam.u32SubOption = E_MMSDK_INFO_MOVIE_SUBTITLE_TYPE;
    stParam.pParam = (void*)&u32SubtitleType; //assign from UI(buffer adderss)
    stParam.u32Paramsize = sizeof(u32SubtitleType); //assign from UI
    bRet = MMSDK_GetOptionByPara(u32Item, &stParam,1);
    if (bRet == TRUE)
    {
        switch(u32SubtitleType)
        {
            case E_MMSDK_SUBTITLE_TYPE_TEXT_UTF8:
                pTrackInfo->eTextFormat = E_SUBTITLE_TEXT_UTF8;
            case E_MMSDK_SUBTITLE_TYPE_TEXT:
            case E_MMSDK_SUBTITLE_TYPE_TEXT_SRT:
            case E_MMSDK_SUBTITLE_TYPE_TEXT_ASS:
            case E_MMSDK_SUBTITLE_TYPE_TEXT_SSA:
            case E_MMSDK_SUBTITLE_TYPE_TEXT_3GPP:
            case E_MMSDK_SUBTITLE_TYPE_TEXT_USF:
                pTrackInfo->eSubtitleType = E_SUBTITLE_DATA_TYPE_TEXT;
                break;
            case E_MMSDK_SUBTITLE_TYPE_IMAGE:
            case E_MMSDK_SUBTITLE_TYPE_IMAGE_BMP:
            case E_MMSDK_SUBTITLE_TYPE_IMAGE_VOBSUB:
            case E_MMSDK_SUBTITLE_TYPE_IMAGE_TS_BMP:
            case E_MMSDK_SUBTITLE_TYPE_IMAGE_TS_PGS:
                pTrackInfo->eSubtitleType = E_SUBTITLE_DATA_TYPE_IMAGE;
                pTrackInfo->eImageFormat = E_SUBTITLE_IMAGE_ARGB8888;
                break;
            default:
                pTrackInfo->eSubtitleType = E_SUBTITLE_DATA_TYPE_TEXT;
                pTrackInfo->eTextFormat = E_SUBTITLE_TEXT_UTF8;
                break;
        }

        MMSDK_GetSubtitleTrackID(u32Item, &pTrackInfo->u8TrackID);
    }

    return bRet;
}


static MS_BOOL _TDAL_MM_SetCropDisplayRectangle(MEDIAITEM u32Item, ST_RECT_SIGNED_POSITION *stSignedCropRect, ST_RECT_SIGNED_POSITION *stSignedDispRect, const MS_BOOL bVideoMute, const MS_BOOL bKeepRatio)
{
    ST_MMSDK_CROP_DISPLAY_WINDOW stInfo;
    mp_printf("MM_SetCropDisplayRectangle.\n");

    memcpy(&(stInfo.stCropRect), (ST_MMSDK_RECT*)stSignedCropRect, sizeof(ST_MMSDK_RECT));
    memcpy(&(stInfo.stDispRect), (ST_MMSDK_RECT*)stSignedDispRect, sizeof(ST_MMSDK_RECT));
    stInfo.bkeepRatio = bKeepRatio;
    stInfo.bBlackScreen = bVideoMute;

    return MMSDK_SetCropDisplayRectangle(u32Item, &stInfo);
}

static MS_BOOL _TDAL_MM_SetDisplayRectangle(MEDIAITEM u32Item, ST_RECT_SIGNED_POSITION *stSignedDispRect, const MS_BOOL bVideoMute, const MS_BOOL bKeepRatio)
{
    ST_RECT_SIGNED_POSITION stSignedCropRect;
    mp_printf("MM_SetDisplayRectangle.\n");

    memset(&stSignedCropRect, 0, sizeof(ST_RECT_SIGNED_POSITION));

    return _TDAL_MM_SetCropDisplayRectangle(u32Item, &stSignedCropRect, stSignedDispRect, bVideoMute, bKeepRatio);
}

static EN_MMSDK_MEDIA_TYPE _TDAL_MapMediaStreamType2MediaType(EN_MPLAYER_MEDIA_STREAM_TYPE eStreamType)
{
    if (eStreamType == E_MPLAYER_MEDIA_STREAM_PHOTO)
    {
        return E_MMSDK_MEDIA_TYPE_PHOTO;
    }
    else if (eStreamType == E_MPLAYER_MEDIA_STREAM_MUSIC)
    {
        return E_MMSDK_MEDIA_TYPE_MUSIC;
    }
    else if (eStreamType == E_MPLAYER_MEDIA_STREAM_MOVIE)
    {
        return E_MMSDK_MEDIA_TYPE_MOVIE;
    }
    else
    {
        //mp_printf("UnKnown EN_MPLAYER_MEDIA_STREAM_TYPE eStreamType : %d\n",eStreamType);
        return E_MMSDK_MEDIA_TYPE_MOVIE;
    }
}

static int _MM_FileSeek(void* const stream, const MS_U64 offset, const int fromwhere)//seek to postition
{//implemented by AP
    return (MsFS_Fseek((FILE*)stream, offset, fromwhere));
}

static MS_U64 _MM_FileLength(void* const stream)  //get stream length
{//implemented by AP
    MS_U64 len = 0;
    if(stream)
    {
        _MM_FileSeek(stream, 0, SEEK_END);
        len = MsFS_Ftell((FILE*)stream);
        _MM_FileSeek(stream, 0, SEEK_SET);
    }

    return len;
}

static void* _MM_FileOpen(const char* const pathname,const char* const mode)//open stream, return stream handle
{//implemented by AP

    mTBOX_TRACE((kTBOX_NIV_1, "--Into --pathname %s --\n", pathname));
    FILE* fp = MsFS_Fopen(pathname, "rb");
    if (NULL==fp)
    {
        mTBOX_TRACE((kTBOX_NIV_WARNING, "ERROR!!! fopen fail, errno=%d\n", errno));
    }

    return fp;
}

static int _MM_FileClose(void* const stream)//close stream
{//implemented by AP
    return (MsFS_Fclose((FILE*)stream));
}

static int _TDAL_FileSeek(void* const stream, const MS_U64 offset, const int fromwhere)//seek to postition
{//implemented by AP
#if DEMO_FILE
    return (MsFS_Fseek((FILE*)stream, offset, fromwhere));
#else
    tTDAL_MP_SeekMode SeekMode = eTDAL_MP_SEEK_MODE_SET;
    MS_U32 _offset = 0;
    if (_pInputDataSeek == NULL)
    {
        mTBOX_TRACE((kTBOX_NIV_WARNING, "_pInputDataSeek is NULL\n"));
        return FALSE; 
    }
    switch (fromwhere)
    {
        case SEEK_SET:
            SeekMode = eTDAL_MP_SEEK_MODE_SET;
            break;        
        case SEEK_CUR:
            SeekMode = eTDAL_MP_SEEK_MODE_CUR;
            break;    
        case SEEK_END:
            SeekMode = eTDAL_MP_SEEK_MODE_END;
            break;
        default:
            mTBOX_TRACE((kTBOX_NIV_WARNING, "UnKnown Seek Mode %d \n",fromwhere));
            return FALSE;           
    }
    _offset = offset&0xFFFFFFFF;
    mp_printf("-----> Enter _TDAL_FileSeek, offset = %lu position = %d\n", _offset,SeekMode);
    return _pInputDataSeek(0,_offset,SeekMode);
#endif
}

static int _TDAL_FileRead(void* const ptr, const int size, const int nitems,  void* const stream)//read data from stream, return size of readed data
{//implemented by AP
#if DEMO_FILE
    return (MsFS_Fread(ptr, size, nitems, (FILE *)stream));
#else
    uint32_t _size = 0;
    if (_pInputDataRead == NULL)
    {
        mTBOX_TRACE((kTBOX_NIV_WARNING, " _pInputDataRead is NULL\n"));
        return FALSE;        
    }
    if (ptr == NULL)
    {
        mTBOX_TRACE((kTBOX_NIV_WARNING, "buffer ptr is NULL\n"));
        return FALSE;
    }
    mp_printf("-----> Enter _TDAL_FileRead, %d\n", size);
    _size = size*nitems;    
    return _pInputDataRead(0,(uint8_t *)ptr,_size);
#endif
}

static int _TDAL_Ringbuf_Seek(MS_U32 u32Hdl, MS_U64 u64SeekPos, int fromwhere)//seek to postition
{
    EN_MPLAYER_MEDIA_STREAM_TYPE eStreamType = (EN_MPLAYER_MEDIA_STREAM_TYPE)u32Hdl;
    EN_MMSDK_MEDIA_TYPE eMediaType = _TDAL_MapMediaStreamType2MediaType(eStreamType);
    MS_U32 u32Ret = 0; //be cautious , 0 indicates OK; 1 means failure!!!!!
    //_gbSeekRightNow[eMediaType] = TRUE;
    mp_printf("-----> Enter _TDAL_Ringbuf_Seek %d, %d\n",(int)eMediaType,(int)u64SeekPos);

    if(_geStrmMode[eMediaType] == MPLAYER_FILE_MODE)
    {
        int seekOff = -1;
        off_t offset;
        if(sizeof(off_t) > 4)
        {
            offset = u64SeekPos;
        }
        else
        {
            offset = u64SeekPos & 0xFFFFFFFF;
            //printf("Warning!!!!!!, sizeof(off_t) is not equal to sizeof(unsinged long long)\n");
        }

        void* pStream = NULL;
        pStream = _gpStream[eStreamType];

        seekOff =  _TDAL_FileSeek(pStream, offset, SEEK_SET);

        mp_printf("[%s][%d]SeekOff=%d\n",__FUNCTION__,__LINE__,seekOff);
        u32Ret = (seekOff >= 0) ? 0 : 1;
    }

    return u32Ret;
}

static EN_MPLAYER_MEDIA_STREAM_TYPE _TDAL_MapFileID2MediaStreamType(void* pFileHandle)
{
    MS_S32 i;
    for(i = MMSDK_MEDIA_TYPE_MIN; i < MMSDK_MEDIA_TYPE_NUM; i++)
    {
        if(pFileHandle == _gpStream[i])
        {
            //printf("_TDAL_MapFileID2MediaStreamType=%ld \n", i);
            return (EN_MPLAYER_MEDIA_STREAM_TYPE)i;
        }
    }
    if(pFileHandle == _gpStream[E_MPLAYER_MEDIA_STREAM_AUDIO])
    {
        //printf("_TDAL_MapFileID2MediaStreamType=%ld \n",(MS_U32)E_MPLAYER_MEDIA_STREAM_AUDIO);
        return E_MPLAYER_MEDIA_STREAM_AUDIO;
    }
    if(pFileHandle == _gpStream[E_MPLAYER_MEDIA_STREAM_AUDIO2])
    {
        //printf("_TDAL_MapFileID2MediaStreamType=%ld \n",(MS_U32)E_MPLAYER_MEDIA_STREAM_AUDIO2);
        return E_MPLAYER_MEDIA_STREAM_AUDIO2;
    }
    if(pFileHandle == _gpStream[E_MPLAYER_MEDIA_STREAM_SUBTITLE])
    {
        //printf("_TDAL_MapFileID2MediaStreamType=%ld \n",(MS_U32)E_MPLAYER_MEDIA_STREAM_SUBTITLE);
        return E_MPLAYER_MEDIA_STREAM_SUBTITLE;
    }
    //printf("_TDAL_MapFileID2MediaStreamType=%ld \n",(MS_U32)E_MPLAYER_MEDIA_STREAM_INVALID);
    return E_MPLAYER_MEDIA_STREAM_INVALID;
}

static int _TDAL_Ringbuf_Read(void *pBuffAddr, int size, MS_U32 u32Length , MS_BOOL bBlockRead, MS_U32 u32Hdl)
{
    EN_MPLAYER_MEDIA_STREAM_TYPE eStreamType = (EN_MPLAYER_MEDIA_STREAM_TYPE)u32Hdl;
    EN_MMSDK_MEDIA_TYPE eMediaType = _TDAL_MapMediaStreamType2MediaType(eStreamType);
    mp_printf("-----> Enter _TDAL_Ringbuf_Read, %d, %d\n", (int)eMediaType,(int)u32Length);
    //printf("u32Hdl=%ld \n", u32Hdl);
    //printf("bReadDataFromRingbuf [%d] \n", __LINE__);

    if (_geStrmMode[eMediaType] == MPLAYER_FILE_MODE)
    {
        void* pStream = NULL;
        pStream = _gpStream[eStreamType];
        
        int rdCnt = _TDAL_FileRead(pBuffAddr, sizeof(MS_U8), u32Length, pStream);

        mp_printf("[%s][%d]ReadCount=%d\n",__FUNCTION__,__LINE__,rdCnt);
        if(rdCnt <= 0)
        {
            return 0;
        }
        else
        {
            return rdCnt;
        }
    }

    return u32Length;
}

MS_BOOL TDAL_MPm_MPEventNotify(tTDAL_MP_Event eEvent)
{
    int descriptor = 0;
    tTDAL_MPm_Desc *pstDesc = NULL;

    descriptor = TDAL_MPi_GetRunningMediaDescriptor();

    if (descriptor == -1)
    {
        mTBOX_TRACE((kTBOX_NIV_WARNING, "Can't find running descriptor!\n"));
        return FALSE;
    }

    pstDesc = TDAL_MPi_DescList[descriptor];
    
    if (pstDesc->pEvtNotify != NULL)
    {
        mTBOX_TRACE((kTBOX_NIV_1, "Send tTDAL_MP_Event : 0x%.2x!\n", eEvent));
        pstDesc->pEvtNotify((tTDAL_MP_Handle)pstDesc, eEvent, pstDesc->pCtx);
    }
    else
    {
        mTBOX_TRACE((kTBOX_NIV_WARNING, "pEvtNotify is NULL!\n"));
        return FALSE;
    }

    return TRUE;
}

static MS_BOOL _TDAL_MPlayer_Stop(const EN_MMSDK_MEDIA_TYPE eMediaType)
{
    mTBOX_TRACE((kTBOX_NIV_1, "--------> enter _TDAL_MPlayer_Stop---------------\n "));
    BOOLEAN bRet = FALSE;
    MEDIAITEM u32ItemTmp = NULL;

    TDAL_LockMutex(_gs32MutexOpenStop);

    _geTrickMod[eMediaType] = E_MMSDK_TRICK_NORMAL;
    _gu64CurPostn[eMediaType] = 0;
    _gu64SubCurPostn = 0;

    _gbPlay[eMediaType] = FALSE;

    if(MMSDK_Stop(_gItem[eMediaType]) != TRUE)
    {
        mTBOX_TRACE((kTBOX_NIV_WARNING, "-----> MM_Stop fail \n"));
    }

    if(_gItem[eMediaType] != NULL)
    {
        u32ItemTmp = _gItem[eMediaType];
        _gItem[eMediaType] = NULL;
        MMSDK_DeleteMediaItem(u32ItemTmp);
    }

    if(eMediaType == E_MMSDK_MEDIA_TYPE_MUSIC)
    {
        _geAudioType = MPLAYER_SUBTYPE_UNKNOWN;
        _gu32SaveFilePos = 0;

#if(DEMO_AUDIO_MULTI_TEST == 1)
        TDAL_MM_FreeAudDec();
#endif
    }
    else if(eMediaType == E_MMSDK_MEDIA_TYPE_PHOTO)
    {
        _geRotaAng = E_MMSDK_ROTATE_0;
        _geZoomFctor = E_MMSDK_VIEW_ZOOM_1X;
    }
    else
    {
        _gu32TotalTime = 0;
        _gu32StartTime = 0;

#if(DEMO_AUDIO_MULTI_TEST == 1)
        TDAL_MM_FreeAudDec();
#endif
    }
    _gstMedia.pCallback = NULL;

    if(_gpStream[eMediaType] != NULL)
    {
        _MM_FileClose(_gpStream[eMediaType]);
        _gpStream[eMediaType] = NULL;
        if(eMediaType == E_MMSDK_MEDIA_TYPE_MOVIE && _gpStream[E_MPLAYER_MEDIA_STREAM_SUBTITLE])
        {
            _MM_FileClose(_gpStream[E_MPLAYER_MEDIA_STREAM_SUBTITLE]);
            _gpStream[E_MPLAYER_MEDIA_STREAM_SUBTITLE] = NULL;
        }
        if(eMediaType == E_MMSDK_MEDIA_TYPE_MOVIE && _gpStream[E_MPLAYER_MEDIA_STREAM_AUDIO])
        {
            _MM_FileClose(_gpStream[E_MPLAYER_MEDIA_STREAM_AUDIO]);
            _gpStream[E_MPLAYER_MEDIA_STREAM_AUDIO] = NULL;
        }
        if(eMediaType == E_MMSDK_MEDIA_TYPE_MOVIE && _gpStream[E_MPLAYER_MEDIA_STREAM_AUDIO2])
        {
            _MM_FileClose(_gpStream[E_MPLAYER_MEDIA_STREAM_AUDIO2]);
            _gpStream[E_MPLAYER_MEDIA_STREAM_AUDIO2] = NULL;
        }
    }

    MMSDK_SetBackwardRePlayMode(_gbBackwardRePlayMode);
    //_gbSeekRightNow[eMediaType] = FALSE;
    _geStrmMode[eMediaType] = MPLAYER_FILE_MODE;
    bRet = TRUE;
    TDAL_UnlockMutex(_gs32MutexOpenStop);
    mTBOX_TRACE((kTBOX_NIV_1, "--------> leave _TDAL_MPlayer_Stop---------------\n "));
    return bRet;
}

static void _TDAL_Photo_EvtProcess(MS_U32 argc, void *arg)
{
    EN_MPLAYER_NOTIFY_TYPE ePhotoNotifyTypeTmp = MPLAYER_NOTIFY_NULL;

    while (_gbExit[E_MMSDK_MEDIA_TYPE_PHOTO] != TRUE)
    {
        while((_gePhotoNotifyType == MPLAYER_NOTIFY_NULL) && (_gbExit[E_MMSDK_MEDIA_TYPE_PHOTO] != TRUE))
        {
            MS_U32  pu32RetrievedEventFlag;
            MsOS_WaitEvent(_gs32MediaEvtGrp, MM_EVENT_PHOTO_NOTIFY, &pu32RetrievedEventFlag, E_OR_CLEAR, MSOS_WAIT_FOREVER);
        }
        ePhotoNotifyTypeTmp = _gePhotoNotifyType;
        _gePhotoNotifyType = MPLAYER_NOTIFY_NULL;
        mTBOX_TRACE((kTBOX_NIV_1, "in Photo_EvtProcess thread\n"));
        if (ePhotoNotifyTypeTmp != MPLAYER_NOTIFY_NULL)
        {
            mTBOX_TRACE((kTBOX_NIV_1, "Receive signal=%d from MM_Callback_Photo .\n", (int)ePhotoNotifyTypeTmp));
            if (ePhotoNotifyTypeTmp ==  MPLAYER_START_PLAY)
            {
                TDAL_MPm_Player_SetOption(E_MMSDK_MEDIA_TYPE_PHOTO, MPLAYER_OPTION_STOP_PHOTO_DISPLAY, 0);

                TDAL_LockMutex(_gs32MutexOpenStop);
                _gbPlay[E_MMSDK_MEDIA_TYPE_PHOTO] = TRUE;
                if (_gstRect[E_MMSDK_MEDIA_TYPE_PHOTO].u16Height + _gstRect[E_MMSDK_MEDIA_TYPE_PHOTO].u16Width + _gstRect[E_MMSDK_MEDIA_TYPE_PHOTO].u16X + _gstRect[E_MMSDK_MEDIA_TYPE_PHOTO].u16Y != 0) //should set display win
                {
                    ST_RECT_SIGNED_POSITION sDispWindow;

                    sDispWindow.s32X = _gstRect[E_MMSDK_MEDIA_TYPE_PHOTO].u16X;
                    sDispWindow.s32Y = _gstRect[E_MMSDK_MEDIA_TYPE_PHOTO].u16Y;
                    sDispWindow.u16Width = _gstRect[E_MMSDK_MEDIA_TYPE_PHOTO].u16Width;
                    sDispWindow.u16Height = _gstRect[E_MMSDK_MEDIA_TYPE_PHOTO].u16Height;
                    mTBOX_TRACE((kTBOX_NIV_1, "--------DISP_WINDOW[%ld][%ld][%d][%d]\n", sDispWindow.s32X, sDispWindow.s32Y, sDispWindow.u16Width, sDispWindow.u16Height));
                    if (_TDAL_MM_SetDisplayRectangle(_gItem[E_MMSDK_MEDIA_TYPE_PHOTO], &sDispWindow, TRUE, TRUE))
                    {
                        _gbPlay[E_MMSDK_MEDIA_TYPE_PHOTO] = FALSE;
                    }
                }
                if (MMSDK_Play(_gItem[E_MMSDK_MEDIA_TYPE_PHOTO]) != TRUE)
                {
                    _gbPlay[E_MMSDK_MEDIA_TYPE_PHOTO] = FALSE;
                }
                TDAL_UnlockMutex(_gs32MutexOpenStop);
        }
            else if (ePhotoNotifyTypeTmp == MPLAYER_PHOTO_DECODE_DONE)
            {
                mTBOX_TRACE((kTBOX_NIV_1, "---------------->Decode Successfully!\n"));
                mTBOX_TRACE((kTBOX_NIV_1, "---------------------after  process MPLAYER_PHOTO_DISPLAYED .\n"));
            }
            else if (ePhotoNotifyTypeTmp == MPLAYER_PHOTO_DECODE_FAILED)
            {
                mTBOX_TRACE((kTBOX_NIV_WARNING, "---------------->Decode Error! \n"));
                mTBOX_TRACE((kTBOX_NIV_WARNING, "---------------------after  process MPLAYER_PHOTO_DECODE_FAILED .\n"));
            }
            else if (ePhotoNotifyTypeTmp == MPLAYER_EXIT_ERROR_FILE)
            {
                mTBOX_TRACE((kTBOX_NIV_WARNING, "---------------->Unsupported File! \n"));
                mTBOX_TRACE((kTBOX_NIV_WARNING, "---------------------after  process MPLAYER_PHOTO_ERROR_FILE .\n"));
            }

            ePhotoNotifyTypeTmp = MPLAYER_NOTIFY_NULL;
        }
    }
    MsOS_SetEvent(_gs32MediaEvtGrp, MM_EVENT_PHOTO_PROC_EXIT);
}

static void _TDAL_Music_EvtProcess(MS_U32 argc, void *arg)
{
    EN_MPLAYER_NOTIFY_TYPE eMusicNotifyTypeTmp = MPLAYER_NOTIFY_NULL;

    while(_gbExit[E_MMSDK_MEDIA_TYPE_MUSIC] != TRUE)
    {
        while(_geMusicNotifyType == MPLAYER_NOTIFY_NULL && _gbExit[E_MMSDK_MEDIA_TYPE_MUSIC] != TRUE)
        {
            MS_U32  pu32RetrievedEventFlag;
            //MsOS_WaitEvent(condMusicNotifyTypeValid, 1, &pu32RetrievedEventFlag, E_OR_CLEAR, MSOS_WAIT_FOREVER);
            MsOS_WaitEvent(_gs32MediaEvtGrp, MM_EVENT_MUSIC_NOTIFY, &pu32RetrievedEventFlag, E_OR_CLEAR, MSOS_WAIT_FOREVER);
        }
        eMusicNotifyTypeTmp = _geMusicNotifyType;
        _geMusicNotifyType = MPLAYER_NOTIFY_NULL;

        TDAL_LockMutex(_gs32MutexOpenStop);
        mTBOX_TRACE((kTBOX_NIV_1, "----- in Music_EvtProcess thread\n"));

        if((eMusicNotifyTypeTmp != MPLAYER_NOTIFY_NULL) && (_gItem[E_MMSDK_MEDIA_TYPE_MUSIC] != 0))
        {
            mTBOX_TRACE((kTBOX_NIV_1, "Receive signal=%d from MM_Callback_Music .\n", (int)eMusicNotifyTypeTmp));
            if(eMusicNotifyTypeTmp == MPLAYER_EXIT_OK)
            {
                mTBOX_TRACE((kTBOX_NIV_1, "Music exit ok!\n"));
                _TDAL_MPlayer_Stop(E_MMSDK_MEDIA_TYPE_MUSIC);
                TDAL_MPm_MPEventNotify(eTDAL_MP_EVENT_FINISHED);
            }
            else if(eMusicNotifyTypeTmp == MPLAYER_INSUFFICIENT_DATA)
            {
                if(MMSDK_SetTrickMode(_gItem[E_MMSDK_MEDIA_TYPE_MUSIC],  E_MMSDK_TRICK_PAUSE) == FALSE)
                {
                    mTBOX_TRACE((kTBOX_NIV_WARNING, "-----> MM_SetTrickMode fail 1\n"));
                }
            }
            else if(eMusicNotifyTypeTmp == MPLAYER_SUFFICIENT_DATA)
            {
                if(MMSDK_SetTrickMode(_gItem[E_MMSDK_MEDIA_TYPE_MUSIC],  E_MMSDK_TRICK_NORMAL) == FALSE)
                {
                    mTBOX_TRACE((kTBOX_NIV_WARNING, "-----> MM_SetTrickMode fail 2\n"));
                }
            }
            else if(eMusicNotifyTypeTmp == MPLAYER_START_PLAY)
            {
                _gbPlay[E_MMSDK_MEDIA_TYPE_MUSIC] = MMSDK_Play(_gItem[E_MMSDK_MEDIA_TYPE_MUSIC]);
            }
            eMusicNotifyTypeTmp = MPLAYER_NOTIFY_NULL;
        }
        TDAL_UnlockMutex(_gs32MutexOpenStop);

    }
    MsOS_SetEvent(_gs32MediaEvtGrp, MM_EVENT_MUSIC_PROC_EXIT);
}

static void _TDAL_Video_EvtProcess(MS_U32 argc, void *arg)
{
    EN_MPLAYER_NOTIFY_TYPE eVideoNotifyTypeTmp = MPLAYER_NOTIFY_NULL;
    while(_gbExit[E_MMSDK_MEDIA_TYPE_MOVIE] != TRUE)
    {
        while((_geVideoNotifyType == MPLAYER_NOTIFY_NULL) && (_gbExit[E_MMSDK_MEDIA_TYPE_MOVIE] != TRUE))
        {
            MS_U32  pu32RetrievedEventFlag;
            //MsOS_WaitEvent(condVideoNotifyTypeValid, 1, &pu32RetrievedEventFlag, E_OR_CLEAR, MSOS_WAIT_FOREVER);
            MsOS_WaitEvent(_gs32MediaEvtGrp, MM_EVENT_VIDEO_NOTIFY, &pu32RetrievedEventFlag, E_OR_CLEAR, MSOS_WAIT_FOREVER);
        }
        eVideoNotifyTypeTmp = _geVideoNotifyType;
        _geVideoNotifyType = MPLAYER_NOTIFY_NULL;

        TDAL_LockMutex(_gs32MutexOpenStop);
        mTBOX_TRACE((kTBOX_NIV_1, "----- in Video_EvtProcess thread\n"));
        if(eVideoNotifyTypeTmp != MPLAYER_NOTIFY_NULL)
        {
            if(eVideoNotifyTypeTmp == MPLAYER_FRAME_READY)
            {
               mTBOX_TRACE((kTBOX_NIV_1, "==================Movie frame ready!\n"));
            }
            else if(eVideoNotifyTypeTmp == MPLAYER_EXIT_OK)
            {
               mTBOX_TRACE((kTBOX_NIV_1, "Movie exit ok!\n"));
                _TDAL_MPlayer_Stop(E_MMSDK_MEDIA_TYPE_MOVIE);
                TDAL_MPm_MPEventNotify(eTDAL_MP_EVENT_FINISHED);
            }
            else if(eVideoNotifyTypeTmp == MPLAYER_EXIT_ERROR_FILE)
            {
                mTBOX_TRACE((kTBOX_NIV_WARNING, "Movie unsupport file!\n"));
                _TDAL_MPlayer_Stop(E_MMSDK_MEDIA_TYPE_MOVIE);
            }
            else if(eVideoNotifyTypeTmp == MPLAYER_BLACKSCREEN_OFF)
            {
            }
            else if(eVideoNotifyTypeTmp == MPLAYER_INSUFFICIENT_DATA)
            {
                if(MMSDK_SetTrickMode(_gItem[E_MMSDK_MEDIA_TYPE_MOVIE], E_MMSDK_TRICK_PAUSE) == FALSE)
                {
                    mTBOX_TRACE((kTBOX_NIV_WARNING, "-----> MM_SetTrickMode fail 1\n"));
                }
            }
            else if(eVideoNotifyTypeTmp == MPLAYER_SUFFICIENT_DATA)
            {
                if(MMSDK_SetTrickMode(_gItem[E_MMSDK_MEDIA_TYPE_MOVIE], E_MMSDK_TRICK_NORMAL) == FALSE)
                {
                    mTBOX_TRACE((kTBOX_NIV_WARNING, "-----> MM_SetTrickMode fail 2\n"));
                }
            }
            else if(eVideoNotifyTypeTmp == MPLAYER_START_PLAY)
            {
                if(_gstRect[E_MMSDK_MEDIA_TYPE_MOVIE].u16Height + _gstRect[E_MMSDK_MEDIA_TYPE_MOVIE].u16Width + _gstRect[E_MMSDK_MEDIA_TYPE_MOVIE].u16X + _gstRect[E_MMSDK_MEDIA_TYPE_MOVIE].u16Y != 0)  //should set display win
                {
                    ST_RECT_SIGNED_POSITION sDispWindow;

                    sDispWindow.s32X = _gstRect[E_MMSDK_MEDIA_TYPE_MOVIE].u16X;
                    sDispWindow.s32Y = _gstRect[E_MMSDK_MEDIA_TYPE_MOVIE].u16Y;
                    sDispWindow.u16Width = _gstRect[E_MMSDK_MEDIA_TYPE_MOVIE].u16Width;
                    sDispWindow.u16Height = _gstRect[E_MMSDK_MEDIA_TYPE_MOVIE].u16Height;
                    if(_TDAL_MM_SetDisplayRectangle(_gItem[E_MMSDK_MEDIA_TYPE_MOVIE], &sDispWindow, TRUE, TRUE))
                    {
                        _gbPlay[E_MMSDK_MEDIA_TYPE_MOVIE] = FALSE;
                    }
                }

                if(MMSDK_Play(_gItem[E_MMSDK_MEDIA_TYPE_MOVIE]) != TRUE)
                {
                    _gbPlay[E_MMSDK_MEDIA_TYPE_MOVIE] = FALSE;
                }
                else
                {
                    _gbPlay[E_MMSDK_MEDIA_TYPE_MOVIE] = TRUE;
                }
            }
            else if(eVideoNotifyTypeTmp == MPLAYER_PLAY_OK)
            {
                if(MMSDK_SetSubtitleOnOff(_gItem[E_MMSDK_MEDIA_TYPE_MOVIE], TRUE) == FALSE)
                {
                    mTBOX_TRACE((kTBOX_NIV_WARNING, " fail to set subtitle on.\n"));
                }
            }

            eVideoNotifyTypeTmp = MPLAYER_NOTIFY_NULL;
        }
        TDAL_UnlockMutex(_gs32MutexOpenStop);
    }

    MsOS_SetEvent(_gs32MediaEvtGrp, MM_EVENT_VIDEO_PROC_EXIT);
}

static MS_BOOL _TDAL_MM_Callback_Photo(const MMSDK_U32 u32Msg, void* pMsgInfo, void* pAckInfo)
{
    EN_MMSDK_CALLBACK_MSG eEvent = (EN_MMSDK_CALLBACK_MSG)((MS_U32)u32Msg);
    ST_MMSDK_CALLBACK_IO_INFO* pFileEvent = (ST_MMSDK_CALLBACK_IO_INFO*)pMsgInfo;
    ST_MMSDK_CALLBACK_IO_ACK* pstFileAck = (ST_MMSDK_CALLBACK_IO_ACK*)pAckInfo;
    
    MS_BOOL bRet = FALSE;
#if (MM_PHOTO_PATH == 0x2)    
    ST_MMSDK_CALLBACK_NOTIFY_INFO *pNotifyInfo = (ST_MMSDK_CALLBACK_NOTIFY_INFO*)pMsgInfo;
    extern tTDAL_GFX_Error TDAL_GFXm_Blit_GIF(void);
#endif
    mp_printf("eEvent is  %d\n", (int)eEvent);
    switch(eEvent)
    {
        /// The notify event is file read request.
        case E_MMSDK_CALLBACK_FILE_READ:
            mp_printf("Get E_MMSDK_CALLBACK_FILE_READ event. need %ld bytes data\n", pFileEvent->u32FileRequestSize);
            pstFileAck->u32ReadSize = _TDAL_Ringbuf_Read(pFileEvent->pu8FileBuff, sizeof(MS_U8),  pFileEvent->u32FileRequestSize, !_gbPlay[E_MMSDK_MEDIA_TYPE_PHOTO], (MS_U32)E_MMSDK_MEDIA_TYPE_PHOTO);
            _gu64CurPostn[E_MMSDK_MEDIA_TYPE_PHOTO] = pstFileAck->u32ReadSize;
            if (_gu64CurPostn[E_MMSDK_MEDIA_TYPE_PHOTO] >= _gu64Size[E_MMSDK_MEDIA_TYPE_PHOTO])
            {
                pstFileAck->bIsFileEnd = TRUE;
            }

            bRet = TRUE;
            break;

        /// The notify event is file seek request.
        case E_MMSDK_CALLBACK_FILE_SEEK:
            mp_printf("Get E_EVENT_FILE_SEEK event.\n");
            if(pFileEvent->u64FileSeekPos <= (MS_U64)_gu64Size[E_MMSDK_MEDIA_TYPE_PHOTO])
            {
                if(_TDAL_Ringbuf_Seek(E_MMSDK_MEDIA_TYPE_PHOTO, pFileEvent->u64FileSeekPos, SEEK_SET) == 0)
                {
                    _gu64CurPostn[E_MMSDK_MEDIA_TYPE_PHOTO] = pFileEvent->u64FileSeekPos;
                    bRet = TRUE;
                }
                else
                {
                    mp_printf("File seek fail.\n");
                }
            }
            break;

            /// The notify event is file tell request.
        case E_MMSDK_CALLBACK_FILE_TELL:
            mp_printf("Get E_MMSDK_CALLBACK_FILE_TELL event.\n");
            pstFileAck->u64FilePosition = _gu64CurPostn[E_MMSDK_MEDIA_TYPE_PHOTO];
            bRet = TRUE;
            break;

            /// The notify event is file init OK.
        case E_MMSDK_CALLBACK_INIT_OK:
            _bIsReadyToPlay = TRUE;
            break;

            /// The notify event is file init fail.
        case E_MMSDK_CALLBACK_INIT_FAIL:
            mp_printf("Get E_MMSDK_CALLBACK_INIT_FAIL event.\n");
            MM_SET_PHOTO_NOTIFY_TYPE(MPLAYER_EXIT_ERROR_FILE);
            break;

            /// The notify event is invalid file.
        case E_MMSDK_CALLBACK_INVALID_FILE:
            mp_printf("Get E_MMSDK_CALLBACK_INVALID_FILE event.\n");
            MM_SET_PHOTO_NOTIFY_TYPE(MPLAYER_EXIT_ERROR_FILE);
            break;

            /// The notify event is thumbnail decoding done.
        case E_MMSDK_CALLBACK_THUMBNAIL_DONE:
            mp_printf("Get E_MMSDK_CALLBACK_THUMBNAIL_DONE event.\n");
            break;

            /// The notify event is thumbnail decoding error.
        case E_MMSDK_CALLBACK_THUMBNAIL_ERROR:
            mp_printf("Get E_MMSDK_CALLBACK_THUMBNAIL_ERROR event.\n");
            break;

            /// The notify event is decoding error.
        case E_MMSDK_CALLBACK_DECODE_ERROR:
            mp_printf("Get E_MMSDK_CALLBACK_DECODE_ERROR event.\n");
            MM_SET_PHOTO_NOTIFY_TYPE(MPLAYER_PHOTO_DECODE_FAILED);
            break;

            /// The notify event is decoding done.
        case E_MMSDK_CALLBACK_DECODE_DONE:
            mp_printf("Get E_MMSDK_CALLBACK_DECODE_DONE event.\n");
            MM_SET_PHOTO_NOTIFY_TYPE(MPLAYER_PHOTO_DECODE_DONE);
            break;

        /// The notify event is video one frame decoding done.
        case E_MMSDK_CALLBACK_DECODE_ONE_FRAME_DONE:
            mp_printf("Get E_MMSDK_CALLBACK_DECODE_ONE_FRAME_DONE event.\n");
#if (MM_PHOTO_PATH == 0x2)
            _gu32GifAddrOffset = pNotifyInfo->u32Ret;
            if(TDAL_GFXm_Blit_GIF())
            {
                TDAL_GFXm_Blit_GIF();
            }
#endif          
            break;
        default:
            break;
    }

    return bRet;
}

static MS_BOOL _TDAL_MM_Callback_Music(const MMSDK_U32 u32Msg, void* pMsgInfo, void* pAckInfo)
{
    EN_MMSDK_CALLBACK_MSG eEvent = (EN_MMSDK_CALLBACK_MSG)((MS_U32)u32Msg);
    ST_MMSDK_CALLBACK_IO_INFO* pFileEvent = (ST_MMSDK_CALLBACK_IO_INFO*)pMsgInfo;
    ST_MMSDK_CALLBACK_IO_ACK* pstFileAck = (ST_MMSDK_CALLBACK_IO_ACK*)pAckInfo;
    MS_U8 *p_stWaveHdr = (MS_U8 *)&_gstWaveHdr;
    MS_BOOL bRet = FALSE;

    switch(eEvent)
    {
    /// The notify event is file read request.
    case E_MMSDK_CALLBACK_FILE_READ:
        memset(pstFileAck, 0, sizeof(ST_MMSDK_CALLBACK_IO_ACK));
        mp_printf("-------> E_MMSDK_CALLBACK_FILE_READ Needs %d\n", (int)pFileEvent->u32FileRequestSize);
        if(_geAudioType == MPLAYER_SUBTYPE_PCM && _gu32SaveFilePos <= sizeof(_gstWaveHdr)) // just PCM audio format
        {
            if((_gu32SaveFilePos < sizeof(_gstWaveHdr)) &&
                (pFileEvent->u32FileRequestSize + _gu32SaveFilePos < sizeof(_gstWaveHdr)))// within WAV header
            {
                memcpy(pFileEvent->pu8FileBuff, p_stWaveHdr + _gu32SaveFilePos, pFileEvent->u32FileRequestSize);
                pstFileAck->u32ReadSize = pFileEvent->u32FileRequestSize;
                _gu32SaveFilePos += pstFileAck->u32ReadSize;
            }
            else // within both WAV header
            {
                memcpy(pFileEvent->pu8FileBuff, p_stWaveHdr + _gu32SaveFilePos, sizeof(_gstWaveHdr) - _gu32SaveFilePos);
                pstFileAck->u32ReadSize = _TDAL_Ringbuf_Read(pFileEvent->pu8FileBuff + sizeof(_gstWaveHdr) - _gu32SaveFilePos, sizeof(MS_U8), pFileEvent->u32FileRequestSize - (sizeof(_gstWaveHdr) - _gu32SaveFilePos), !_gbPlay[E_MMSDK_MEDIA_TYPE_MUSIC], (MS_U32)E_MMSDK_MEDIA_TYPE_MUSIC);
                _gu64CurPostn[E_MMSDK_MEDIA_TYPE_MUSIC] += pstFileAck->u32ReadSize;
                if(_gu8AudioPara[9] == 0)
                {
                    MS_U16 i = 0;
                    if(*((U16*)(_gu8AudioPara + 3)) == 16)
                    {
                        for(i = 0; i < pstFileAck->u32ReadSize ; i += 2)
                        {
                            B_ENDIAN2L_ENDIAN16(pFileEvent->pu8FileBuff + sizeof(_gstWaveHdr) - _gu32SaveFilePos + i);//Name error, big endian, but in fact little endian
                        }
                    }
                    else if(*((U16*)(_gu8AudioPara + 3)) == 32)
                    {
                        for(i = 0; i < pstFileAck->u32ReadSize ; i += 4)
                        {
                            B_ENDIAN2L_ENDIAN16(pFileEvent->pu8FileBuff + sizeof(_gstWaveHdr) - _gu32SaveFilePos + i);
                        }
                    }
                }

                pstFileAck->u32ReadSize = pFileEvent->u32FileRequestSize;
                _gu32SaveFilePos += pFileEvent->u32FileRequestSize;
            }
        }
        else
        {
            pstFileAck->u32ReadSize = _TDAL_Ringbuf_Read(pFileEvent->pu8FileBuff, sizeof(MS_U8), pFileEvent->u32FileRequestSize, !_gbPlay[E_MMSDK_MEDIA_TYPE_MUSIC], (MS_U32)E_MMSDK_MEDIA_TYPE_MUSIC);
            _gu32SaveFilePos += pstFileAck->u32ReadSize;
            _gu64CurPostn[E_MMSDK_MEDIA_TYPE_MUSIC] +=  pstFileAck->u32ReadSize;
            if(_geAudioType == MPLAYER_SUBTYPE_PCM)
            {
                if(_gu8AudioPara[9] == 0)
                {
                    MS_U16 i = 0;
                    if(*((U16*)(_gu8AudioPara + 3)) == 16)
                    {
                        for(i = 0; i < pstFileAck->u32ReadSize ; i += 2)
                        {
                            B_ENDIAN2L_ENDIAN16(pFileEvent->pu8FileBuff + i); //Name error, big endian, but in fact little endian
                        }
                    }
                    else if(*((U16*)(_gu8AudioPara + 3)) == 32)
                    {
                        for(i = 0; i < pstFileAck->u32ReadSize ; i += 4)
                        {
                            B_ENDIAN2L_ENDIAN32(pFileEvent->pu8FileBuff + sizeof(_gstWaveHdr) - _gu32SaveFilePos + i);
                        }
                    }
                }

                if(_gu64CurPostn[E_MMSDK_MEDIA_TYPE_MUSIC] + 44 >= _gu64Size[E_MMSDK_MEDIA_TYPE_MUSIC])
                {
                    pstFileAck->bIsFileEnd = TRUE;
                }
            }

            else
            {
                if(_gu64CurPostn[E_MMSDK_MEDIA_TYPE_MUSIC] >= _gu64Size[E_MMSDK_MEDIA_TYPE_MUSIC])
                {
                    pstFileAck->bIsFileEnd = TRUE;
                }
            }
        }
        mp_printf("-------> E_MMSDK_CALLBACK_FILE_READ %d\n", (int)pstFileAck->u32ReadSize);
        bRet = TRUE;
        break;

        /// The notify event is file seek request.
    case E_MMSDK_CALLBACK_FILE_SEEK:
        memset(pstFileAck, 0, sizeof(ST_MMSDK_CALLBACK_IO_ACK));
        if(_geAudioType == MPLAYER_SUBTYPE_PCM)  // just PCM audio format
        {
            _gu32SaveFilePos = (MS_U32)pFileEvent->u64FileSeekPos;
            if(pFileEvent->u64FileSeekPos > sizeof(_gstWaveHdr)) // both within WAV header and file
            {
                if(_TDAL_Ringbuf_Seek(E_MPLAYER_MEDIA_STREAM_MUSIC, pFileEvent->u64FileSeekPos - sizeof(_gstWaveHdr), SEEK_SET) == 0)
                {
                    _gu64CurPostn[E_MMSDK_MEDIA_TYPE_MUSIC] = pFileEvent->u64FileSeekPos - sizeof(_gstWaveHdr);
                    bRet = TRUE;
                }
                else
                {
                    printf("File seek fail.\n");
                }
            }
            else  //just within WAV header
            {
                if(_TDAL_Ringbuf_Seek(E_MPLAYER_MEDIA_STREAM_MUSIC, 0, SEEK_SET)==0)
                {
                    _gu64CurPostn[E_MMSDK_MEDIA_TYPE_MUSIC] = 0;
                    bRet = TRUE;
                }
            }
        }
        else if(pFileEvent->u64FileSeekPos <= (MS_U64)_gu64Size[E_MMSDK_MEDIA_TYPE_MUSIC])
        {
            if(_TDAL_Ringbuf_Seek(E_MPLAYER_MEDIA_STREAM_MUSIC, pFileEvent->u64FileSeekPos, SEEK_SET) == 0)
            {
                _gu64CurPostn[E_MMSDK_MEDIA_TYPE_MUSIC] = pFileEvent->u64FileSeekPos;
                mp_printf("------->fseek :%lld successfully-----------\n",pFileEvent->u64FileSeekPos);
                bRet = TRUE;
            }
            else
            {
                mp_printf("File seek fail.\n");
            }
        }

        mp_printf("-------seeking %d bytes  needs time is %d ms\n", (int)pFileEvent->u64FileSeekPos,(int)_gu32TotalTime);
        break;

        /// The notify event is file tell request.
    case E_MMSDK_CALLBACK_FILE_TELL:
        memset(pstFileAck, 0, sizeof(ST_MMSDK_CALLBACK_IO_ACK));
        if(_geAudioType == MPLAYER_SUBTYPE_PCM)  // just PCM audio format
        {
            pstFileAck->u64FilePosition = (_gu32SaveFilePos > (MS_U32)_gu64CurPostn[E_MMSDK_MEDIA_TYPE_MUSIC])?(MS_U64)_gu32SaveFilePos : _gu64CurPostn[E_MMSDK_MEDIA_TYPE_MUSIC];
        }
        else
        {
            pstFileAck->u64FilePosition = _gu64CurPostn[E_MMSDK_MEDIA_TYPE_MUSIC];
        }
        bRet = TRUE;
        break;

        /// The notify event is file init OK.
    case E_MMSDK_CALLBACK_INIT_OK:
        _bIsReadyToPlay = TRUE;
        break;

        /// The notify event is file init fail.
    case E_MMSDK_CALLBACK_INIT_FAIL:
        MM_SET_MUSIC_NOTIFY_TYPE(MPLAYER_EXIT_ERROR_FILE);
        break;

        /// The notify event is invalid file.
    case E_MMSDK_CALLBACK_INVALID_FILE:
        mp_printf("Get E_MMSDK_CALLBACK_INVALID_FILE event.\n");
        MM_SET_MUSIC_NOTIFY_TYPE(MPLAYER_EXIT_ERROR_FILE);
        break;

        /// The notify event is decoding error.
    case E_MMSDK_CALLBACK_DECODE_ERROR:
        MM_SET_MUSIC_NOTIFY_TYPE(MPLAYER_EXIT_ERROR_FILE);
        break;

        /// The notify event is decoding done.
    case E_MMSDK_CALLBACK_DECODE_DONE:
        mp_printf("Get E_MMSDK_CALLBACK_DECODE_DONE event.\n");
        MM_SET_MUSIC_NOTIFY_TYPE(MPLAYER_EXIT_OK);
        break;

        /// The notify event is video first frame decoding done.
    case E_MMSDK_CALLBACK_DECODE_FIRST_FRAME_DONE:
        mp_printf("Get E_MMSDK_CALLBACK_DECODE_FIRST_FRAME_DONE event.\n");
        break;

        /// The notify event is media backward to file start and re-start the media playback.
    case E_MMSDK_CALLBACK_DECODE_BACKWARD_TO_FILE_START:
        if (_gbMusicBackwardRePlayMode)
            _geTrickMod[E_MMSDK_MEDIA_TYPE_MUSIC] = E_MMSDK_TRICK_NORMAL;
        else
            MM_SET_MUSIC_NOTIFY_TYPE(MPLAYER_EXIT_OK);
        break;
    case E_MMSDK_CALLBACK_AUDIO_UNSUPPORTED:
        mp_printf("Get E_MMSDK_CALLBACK_AUDIO_UNSUPPORTED event.\n");
        break;
    case E_MMSDK_CALLBACK_NOT_SUPPORT:
        mp_printf("Get E_MMSDK_CALLBACK_NOT_SUPPORT event.\n");
        break;
    default:
        break;
    }

    return bRet;
}

static MS_BOOL _TDAL_MM_Callback_Video(const MMSDK_U32 u32Msg, void* pMsgInfo, void* pAckInfo)
{
    EN_MMSDK_CALLBACK_MSG eEvent = (EN_MMSDK_CALLBACK_MSG)((MS_U32)u32Msg);
    ST_MMSDK_CALLBACK_IO_INFO* pFileEvent = (ST_MMSDK_CALLBACK_IO_INFO*)pMsgInfo;
    ST_MMSDK_CALLBACK_IO_ACK* pstFileAck = (ST_MMSDK_CALLBACK_IO_ACK*)pAckInfo;
    MS_BOOL bRet = FALSE;
    EN_MPLAYER_MEDIA_STREAM_TYPE eStreamType = E_MPLAYER_MEDIA_STREAM_INVALID;
    MS_U64* u64TempCurPostn = NULL;

    mp_printf("eEvent is  %d\n", (int)eEvent);
    switch(eEvent)
    {
        /// The notify event is file read request.
    case E_MMSDK_CALLBACK_FILE_READ:
        eStreamType = _TDAL_MapFileID2MediaStreamType(pFileEvent->pFileHandle);
        pstFileAck->u32ReadSize = _TDAL_Ringbuf_Read(pFileEvent->pu8FileBuff, sizeof(MS_U8), pFileEvent->u32FileRequestSize, !_gbPlay[E_MMSDK_MEDIA_TYPE_MOVIE], eStreamType);

        if(eStreamType == E_MPLAYER_MEDIA_STREAM_SUBTITLE)
            u64TempCurPostn = &_gu64SubCurPostn;
        else if (eStreamType == E_MPLAYER_MEDIA_STREAM_AUDIO)
            u64TempCurPostn = &_gu64AudioCurPostn;
        else if (eStreamType == E_MPLAYER_MEDIA_STREAM_AUDIO2)
            u64TempCurPostn = &_gu64Audio2CurPostn;
        else
            u64TempCurPostn = &_gu64CurPostn[(EN_MMSDK_MEDIA_TYPE)eStreamType];

        *u64TempCurPostn += pstFileAck->u32ReadSize;
        if((*u64TempCurPostn >= _gu64Size[E_MMSDK_MEDIA_TYPE_MOVIE]))
        {
            mp_printf("-------> E_MMSDK_CALLBACK_FILE_READ fileEnd,needs %lx \n", pFileEvent->u32FileRequestSize);
            pstFileAck->bIsFileEnd = TRUE;
        }

        mp_printf("-------> E_MMSDK_CALLBACK_FILE_READ %d\n", (int)pstFileAck->u32ReadSize);
        bRet = TRUE;
        break;

        /// The notify event is file seek request.
    case E_MMSDK_CALLBACK_FILE_SEEK:
        eStreamType = _TDAL_MapFileID2MediaStreamType(pFileEvent->pFileHandle);
        mp_printf("--- File size: 0x%llx, Seek position: 0x%llx.\n", _gu64Size[E_MMSDK_MEDIA_TYPE_MOVIE], pFileEvent->u64FileSeekPos);
        if(pFileEvent->u64FileSeekPos <= (MS_U64)_gu64Size[E_MMSDK_MEDIA_TYPE_MOVIE])
        {
            int SeekResult = 1;//be cautious , 0 indicates OK; 1 means failure!!!!!
            SeekResult = _TDAL_Ringbuf_Seek(eStreamType, pFileEvent->u64FileSeekPos, SEEK_SET);
            if(eStreamType == E_MPLAYER_MEDIA_STREAM_SUBTITLE)
                u64TempCurPostn = &_gu64SubCurPostn;
            else if (eStreamType == E_MPLAYER_MEDIA_STREAM_AUDIO)
                u64TempCurPostn = &_gu64AudioCurPostn;
            else if (eStreamType == E_MPLAYER_MEDIA_STREAM_AUDIO2)
                u64TempCurPostn = &_gu64Audio2CurPostn;
            else
                u64TempCurPostn = &_gu64CurPostn[(EN_MMSDK_MEDIA_TYPE)eStreamType];

            if(SeekResult == 0)
            {
                *u64TempCurPostn = pFileEvent->u64FileSeekPos;
                bRet = TRUE;
            }
            else
            {
                mp_printf("File seek fail.\n");
            }
        }
        break;

        /// The notify event is file tell request.
    case E_MMSDK_CALLBACK_FILE_TELL:
        mp_printf("Get E_MMSDK_CALLBACK_FILE_TELL event.\n");
        eStreamType = _TDAL_MapFileID2MediaStreamType(pFileEvent->pFileHandle);
        if(eStreamType == E_MPLAYER_MEDIA_STREAM_SUBTITLE)
            pstFileAck->u64FilePosition = _gu64SubCurPostn;
        else if(eStreamType == E_MPLAYER_MEDIA_STREAM_AUDIO)
            pstFileAck->u64FilePosition = _gu64AudioCurPostn;
        else if(eStreamType == E_MPLAYER_MEDIA_STREAM_AUDIO2)
            pstFileAck->u64FilePosition = _gu64Audio2CurPostn;
        else
            pstFileAck->u64FilePosition = _gu64CurPostn[(EN_MMSDK_MEDIA_TYPE)eStreamType];

        bRet = TRUE;
        break;

        /// The notify event is file init OK.
    case E_MMSDK_CALLBACK_INIT_OK:
        mp_printf("Movie Init ok!\n");
        _bIsReadyToPlay = TRUE;
        printf("=Test [%s][%d] time = %d\n",__FUNCTION__,__LINE__,MsOS_GetSystemTime());
        break;

        /// The notify event is file init fail.
    case E_MMSDK_CALLBACK_INIT_FAIL:
        MM_SET_VIDEO_NOTIFY_TYPE(MPLAYER_EXIT_ERROR_FILE);
        break;

        /// The notify event is invalid file.
    case E_MMSDK_CALLBACK_INVALID_FILE:
        mp_printf("Get E_MMSDK_CALLBACK_INVALID_FILE event.\n");
        MM_SET_VIDEO_NOTIFY_TYPE(MPLAYER_EXIT_ERROR_FILE);
        break;

        /// The notify event is thumbnail decoding done.
    case E_MMSDK_CALLBACK_THUMBNAIL_DONE:
        mp_printf("Get E_MMSDK_CALLBACK_THUMBNAIL_DONE event.\n");
        break;

        /// The notify event is thumbnail decoding error.
    case E_MMSDK_CALLBACK_THUMBNAIL_ERROR:
        mp_printf("Get E_MMSDK_CALLBACK_THUMBNAIL_ERROR event.\n");
        break;

        /// The notify event is decoding error.
    case E_MMSDK_CALLBACK_DECODE_ERROR:
        mp_printf("Get E_MMSDK_CALLBACK_DECODE_ERROR event.\n");
        MM_SET_VIDEO_NOTIFY_TYPE(MPLAYER_EXIT_ERROR_FILE);
        break;

        /// The notify event is decoding done.
    case E_MMSDK_CALLBACK_DECODE_DONE:
        mp_printf("Get E_MMSDK_CALLBACK_DECODE_DONE event.\n");
        MM_SET_VIDEO_NOTIFY_TYPE(MPLAYER_EXIT_OK);
        break;

        /// The notify event is video first frame decoding done.
    case E_MMSDK_CALLBACK_DECODE_FIRST_FRAME_DONE:
        mp_printf("Get E_MMSDK_CALLBACK_DECODE_FIRST_FRAME_DONE event.\n");
        break;

        /// The notify event is media backward to file start and re-start the media playback.
    case E_MMSDK_CALLBACK_DECODE_BACKWARD_TO_FILE_START:
        mp_printf("Get E_MMSDK_CALLBACK_DECODE_BACKWARD_TO_FILE_START event.\n");
        _geTrickMod[E_MMSDK_MEDIA_TYPE_MOVIE] = E_MMSDK_TRICK_NORMAL;
        break;

        ///  The notify event is program been changed.
    case E_MMSDK_CALLBACK_PROGRAM_CHANGED:
        mp_printf("Get E_MMSDK_CALLBACK_PROGRAM_CHANGED event.\n");
        break;

        ///  The notify event is audio track been changed.
    case E_MMSDK_CALLBACK_AUDIO_TRACK_CHANGED:
        mp_printf("Get E_MMSDK_CALLBACK_AUDIO_TRACK_CHANGED event.\n");
        break;

    case E_MMSDK_CALLBACK_PLAYING_OK:
        mp_printf("Get E_MMSDK_CALLBACK_PLAYING_OK event.\n");
        MM_SET_VIDEO_NOTIFY_TYPE(MPLAYER_PLAY_OK);
        break;
    case E_MMSDK_CALLBACK_AUDIO_UNSUPPORTED:
        mp_printf("Get E_MMSDK_CALLBACK_AUDIO_UNSUPPORTED event.\n");
        break;
    case E_MMSDK_CALLBACK_NOT_SUPPORT:
        mp_printf("Get E_MMSDK_CALLBACK_NOT_SUPPORT event.\n");
        break;
    default:
        break;
    }

    return bRet;
}

/// @param pClass          \b IN: Up-layer application class point.(for application to distinguish)
/// @param u32Msg         \b IN: Callback message type (EN_MMSDK_CALLBACK_MSG)
/// @param pMsgInfo       \b IN: Callback message info
/// @param pAckInfo       \b IN: Callback Ack info
MMSDK_BOOL _TDAL_MM_Callback(void* pClass, const MMSDK_U32 u32Msg, void* pMsgInfo, void* pAckInfo)
{
    MS_BOOL bRet = FALSE;
    if (pMsgInfo == NULL)
    {
        mTBOX_TRACE((kTBOX_NIV_WARNING, "No event attached data.\n"));
        goto FUN_END;
    }

    ST_MMSDK_CALLBACK_IO_INFO* pFileEvent = (ST_MMSDK_CALLBACK_IO_INFO*)pMsgInfo;
    if(pAckInfo)
        memset(pAckInfo, 0, sizeof(ST_MMSDK_CALLBACK_IO_ACK));

    MS_U32 i;
    for(i = MMSDK_MEDIA_TYPE_MIN; i < MMSDK_MEDIA_TYPE_NUM; i++)
        if(pFileEvent->Item == _gItem[i])
            switch((EN_MMSDK_MEDIA_TYPE)i)
            {
                case E_MMSDK_MEDIA_TYPE_PHOTO:
                    bRet = _TDAL_MM_Callback_Photo(u32Msg, pMsgInfo, pAckInfo);
                    break;
                case E_MMSDK_MEDIA_TYPE_MUSIC:
                    bRet = _TDAL_MM_Callback_Music(u32Msg, pMsgInfo, pAckInfo);
                    break;
                case E_MMSDK_MEDIA_TYPE_MOVIE:
                    bRet = _TDAL_MM_Callback_Video(u32Msg, pMsgInfo, pAckInfo);
                    break;
            }

FUN_END:
    return bRet;
}

static MS_BOOL _TDAL_GetOption_AllocateMemory(MS_U32 u32UnicdBufferSize)
{

    if (_gu32MetalLen == 0)
    {
        mTBOX_TRACE((kTBOX_NIV_1, "=========== _gu32MetalLen is 0 ===============\n"));
        return FALSE;
    }
    
    if (_gpu8Buffer == NULL)
    {
        _gpu8Buffer = (MS_U8 *)AllocNoncachedMem(sizeof(MS_U8) * _gu32MetalLen);
    }
    else
    {
        _gpu8Buffer = (MS_U8 *)ReAllocNoncachedMem(_gpu8Buffer, sizeof(MS_U8) * _gu32MetalLen);
    }
    
    if (_gpu8Buffer == NULL)
    {
        mTBOX_TRACE((kTBOX_NIV_WARNING, "WARNING WARNING _gpu8Buffer == NULL!!!!!!\n"));
        return FALSE;
    }

    if (_gpu16UnicdBuffer == NULL)
    {
        _gpu16UnicdBuffer = (MS_U16 *)AllocNoncachedMem(u32UnicdBufferSize); //malloc memory for unicode
    }
    else
    {
        _gpu16UnicdBuffer = (MS_U16*)ReAllocNoncachedMem(_gpu16UnicdBuffer, u32UnicdBufferSize);
    }
    
    if (_gpu16UnicdBuffer == NULL)
    {
        mTBOX_TRACE((kTBOX_NIV_WARNING, "WARNING WARNING _gpu16UnicdBuffer == NULL!!!!!!\n"));
        return FALSE;
    }

    return TRUE;
}

MS_BOOL TDAL_MPm_Finalize(void)
{
    MS_BOOL bRet = FALSE;
    mTBOX_TRACE((kTBOX_NIV_1, "_MM_Finalize.\n"));
    if (m_bInitialize == FALSE)
    {        
        mTBOX_TRACE((kTBOX_NIV_1, "Not initialized\n"));
        return TRUE;
    }

    //Finalize
    bRet = MMSDK_Finalize();
    m_bInitialize = FALSE;

    return bRet;
}

static MS_BOOL _TDAL_MM_Initialize(void)
{
    MS_BOOL bRet = FALSE;
    mTBOX_TRACE((kTBOX_NIV_1, "_TDAL_MM_Initialize.\n"));

    if (m_bInitialize == TRUE)
    {
        mTBOX_TRACE((kTBOX_NIV_1, "Already initialized\n"));
        bRet = TRUE;
        return bRet;
    }

    bRet = MMSDK_Initialize();

    MMSDK_DebugMSGLevel(E_MMSDK_DBG_LEVEL_TRACE);

    if (!bRet)
        MMSDK_Finalize();

    m_bInitialize = TRUE;
    mTBOX_TRACE((kTBOX_NIV_1,"_TDAL_MM_Initialize Done %d.\n",bRet));

    return bRet;
}

MS_BOOL TDAL_MPm_SetFileIsSupport(MS_BOOL bIsSupport)
{
    if (bIsSupport)
    {
        _bIsFileSupport = TRUE;
        _bIsAudioSupport = TRUE;
    }
    else
    {
        _bIsFileSupport = FALSE;
        _bIsAudioSupport = FALSE;        
    }
}

MS_BOOL TDAL_MPm_Player_GetSpeed(const EN_MMSDK_MEDIA_TYPE eMediaType, MS_S32 *s32Speed)
{
    switch(_geTrickMod[eMediaType] )
    {
        case E_MMSDK_TRICK_PAUSE:
            *s32Speed = 0;
            break;
        case E_MMSDK_TRICK_NORMAL:
            *s32Speed = 100;
            break;
        case E_MMSDK_TRICK_FF_2X:
            *s32Speed = 200;
            break;
        case E_MMSDK_TRICK_FF_4X:
            *s32Speed = 400;
            break;
        case E_MMSDK_TRICK_FF_8X:
            *s32Speed = 800;
            break;
        case E_MMSDK_TRICK_FF_16X:
            *s32Speed = 1600;
            break;
        case E_MMSDK_TRICK_FF_32X:
            *s32Speed = 3200;
            break;
        case E_MMSDK_TRICK_FB_2X:
            *s32Speed = -200;
            break;
        case E_MMSDK_TRICK_FB_4X:
            *s32Speed = -400;
            break;
        case E_MMSDK_TRICK_FB_8X:
            *s32Speed = -800;
            break;
        case E_MMSDK_TRICK_FB_16X:
            *s32Speed = -1600;
            break;
        case E_MMSDK_TRICK_FB_32X:
            *s32Speed = -3200;
            break;
        default:
            *s32Speed = 0;
            return FALSE;
            break;
    }
    return TRUE;
}
MS_BOOL TDAL_MPm_Player_SetSpeed(const EN_MMSDK_MEDIA_TYPE eMediaType, EN_MMSDK_TRICK_MODE eTrickMode)
{
    if(MMSDK_SetTrickMode(_gItem[eMediaType], eTrickMode) == FALSE)
    {
        return FALSE;
    }
    else
    {
        _geTrickMod[eMediaType] = eTrickMode;
    }
    return TRUE;
}

MS_BOOL TDAL_MPm_Player_GetPosition(const EN_MMSDK_MEDIA_TYPE eMediaType, MS_U32 *u32TimeMS)
{
    if (!MMSDK_GetPlayPosition(_gItem[eMediaType], u32TimeMS))
    {
        return FALSE;
    }
    return TRUE;
}

MS_BOOL TDAL_MPm_Player_SetPosition(const EN_MMSDK_MEDIA_TYPE eMediaType, MS_U32 u32TimeMS)
{
    if(!MMSDK_SetPlayPosition(_gItem[eMediaType], u32TimeMS))
    {
        return FALSE;
    }
    return TRUE;
}

EN_MMSDK_MEDIA_TYPE TDAL_MPm_Player_GetMediaType(void)
{
    return _gstMedia.enMediaType;
}

unsigned int TDAL_MPm_Player_GetOption(const EN_MMSDK_MEDIA_TYPE eMediaType, const EN_MPLAYER_OPTION_TYPE eOption, const unsigned int u32SubOption)
{
    MS_U32 u32Ret = 0;
    EN_PLAYER_STRING_FORMAT eStrFormt = E_PLAYER_STRING_FORMAT_UNICODE;
    MS_S8 numStr[256];
    MS_U8 u8TrackID = 0;
    ST_SUBTITLE_TRACK_INFO stSubTrackInfo;
    ST_MMSDK_PARAM stParam = {};
    MS_U32 u32UnicdBufferSize;

    TDAL_LockMutex(_gs32MutexOpenStop);
    switch(eOption)
    {
        case MPLAYER_OPTION_MOVIE_AUDIO_TRACK_NAME:
            MM_GET_OPTION(stParam, E_MMSDK_OPTION_MOVIE_INFO,
                E_MMSDK_INFO_MOVIE_AUDIO_TRACK_LANGUAGE, sizeof(MS_U32), (void*)&u32Ret, _gItem[eMediaType]);
            break;

        case MPLAYER_OPTION_MOVIE_AUDIO_TRACK_INFO:
            MM_GET_OPTION(stParam, E_MMSDK_OPTION_MOVIE_INFO,
                E_MMSDK_INFO_MOVIE_AUDIO_CODEC_TRACK_ID, sizeof(MS_U32), (void*)&u32Ret, _gItem[eMediaType]);
            break;

        case MPLAYER_OPTION_MOVIE_SUBTITLE_TRACK_NAME:
            MM_GET_OPTION(stParam, E_MMSDK_OPTION_MOVIE_INFO,
                E_MMSDK_INFO_MOVIE_SUBTITLE_TRACK_LANGUAGE, sizeof(MS_U32), (void*)&u32Ret, _gItem[eMediaType]);
            break;

        case MPLAYER_OPTION_MOVIE_INFO:
            switch(u32SubOption)
            {
                case MPLAYER_INFO_INDEX_FRAME_EXIST:
                    MM_GET_OPTION(stParam, E_MMSDK_OPTION_MOVIE_INFO,
                        E_MMSDK_INFO_MOVIE_INDEX_EXISTED, 4, (void*)&u32Ret, _gItem[eMediaType]);
                    break;

                case MPLAYER_INFO_TOTAL_TIME:
                    MM_GET_OPTION(stParam, E_MMSDK_OPTION_MOVIE_INFO,
                        E_MMSDK_INFO_TOTAL_TIME, 4, (void*)&u32Ret, _gItem[eMediaType]);
                    break;

                case MPLAYER_INFO_CUR_TIME:
                    MMSDK_GetPlayPosition(_gItem[eMediaType], &u32Ret);
                    break;

                case MPLAYER_INFO_NB_PROGRAM:
                    MM_GET_OPTION(stParam, E_MMSDK_OPTION_MOVIE_INFO,
                        E_MMSDK_INFO_MOVIE_NB_PROGRAM, 4, (void*)&u32Ret, _gItem[eMediaType]);
                    break;

                case MPLAYER_INFO_NB_AUDIO_TRACK:
                    MM_GET_OPTION(stParam, E_MMSDK_OPTION_MOVIE_INFO,
                        E_MMSDK_INFO_MOVIE_NB_AUDIO_TRACK, 4, (void*)&u32Ret, _gItem[eMediaType]);
                    break;

                case MPLAYER_INFO_NB_SUBTITLE_TRACK:
                    MM_GET_OPTION(stParam, E_MMSDK_OPTION_MOVIE_INFO,
                        E_MMSDK_INFO_MOVIE_NB_SUBTITLE_TRACK, 4, (void*)&u32Ret, _gItem[eMediaType]);
                    break;

                case MPLAYER_INFO_PROGRAM_ID:
                    MMSDK_GetProgramID(_gItem[eMediaType], &u8TrackID);
                    u32Ret = u8TrackID;
                    break;

                case MPLAYER_INFO_AUDIO_TRACK_ID:
                    MMSDK_GetAudioTrackID(_gItem[eMediaType], &u8TrackID);
                    u32Ret = u8TrackID;
                    break;

                case MPLAYER_INFO_SUBTITLE_TRACK_ID:
                    if (_TDAL_MM_GetSubtitleTrackInfo(_gItem[eMediaType], &stSubTrackInfo))
                        u32Ret = stSubTrackInfo.u8TrackID;
                    break;

                case MPLAYER_INFO_AUDIO_CODEC:
                    MM_GET_OPTION(stParam, E_MMSDK_OPTION_MOVIE_INFO,
                        E_MMSDK_INFO_MOVIE_AUDIO_CODEC, 4, (void*)&u32Ret, _gItem[eMediaType]);
                    break;

                case MPLAYER_INFO_VIDEO_CODEC:
                    MM_GET_OPTION(stParam, E_MMSDK_OPTION_MOVIE_INFO,
                        E_MMSDK_INFO_MOVIE_VIDEO_CODEC, 4, (void*)&u32Ret, _gItem[eMediaType]);
                    break;

                case MPLAYER_OPTION_GET_MUSIC_TYPE:
                    MM_GET_OPTION(stParam, E_MMSDK_OPTION_MOVIE_INFO,
                        E_MMSDK_INFO_MOVIE_AUDIO_CODEC, 4, (void*)&u32Ret, _gItem[eMediaType]);

                    if (_geAudioType == MPLAYER_SUBTYPE_PCM)
                        u32Ret = (MS_U32)MPLAYER_SUBTYPE_PCM;
                    else if(u32Ret == E_MMSDK_AUDIO_CODEC_MP3)
                        u32Ret = (MS_U32)MPLAYER_SUBTYPE_MP3;
                    else if(u32Ret == E_MMSDK_AUDIO_CODEC_AAC)
                        u32Ret = (MS_U32)MPLAYER_SUBTYPE_AAC;
                    else if((u32Ret == E_MMSDK_AUDIO_CODEC_PCM) || (u32Ret == E_MMSDK_AUDIO_CODEC_ADPCM))
                        u32Ret = (MS_U32)MPLAYER_SUBTYPE_WAV;
                    else if(u32Ret == E_MMSDK_AUDIO_CODEC_WMA)
                        u32Ret = (MS_U32)MPLAYER_SUBTYPE_WMA;
                    break;

                case MPLAYER_INFO_H_SIZE:
                    MM_GET_OPTION(stParam, E_MMSDK_OPTION_MOVIE_INFO,
                        E_MMSDK_INFO_HRES, 4, (void*)&u32Ret, _gItem[eMediaType]);
                    break;

                case MPLAYER_INFO_V_SIZE:
                    MM_GET_OPTION(stParam, E_MMSDK_OPTION_MOVIE_INFO,
                        E_MMSDK_INFO_VRES, 4, (void*)&u32Ret, _gItem[eMediaType]);
                    break;

                case MPLAYER_INFO_FRM_PER_SEC:
                    MM_GET_OPTION(stParam, E_MMSDK_OPTION_MOVIE_INFO,
                        E_MMSDK_INFO_MOVIE_FRAME_PER_SEC, 4, (void*)&u32Ret, _gItem[eMediaType]);
                    break;

                default:
                    mTBOX_TRACE((kTBOX_NIV_WARNING, "[ERROR] Unknown MOVIE INFO\n"));
                    break;
            }
            break;

        case MPLAYER_OPTION_TRICK_MODE:
            u32Ret = (MS_U32)_geTrickMod[eMediaType];
            break;

        case MPLAYER_OPTION_ROTATION:
            if (eMediaType == E_MMSDK_MEDIA_TYPE_PHOTO)
            {
                u32Ret = (MS_U32)_geRotaAng;
            }
            else
            {
                u32Ret = (MS_U32)0xFFFF;
            }
            break;

        case MPLAYER_OPTION_ZOOM:
            if (eMediaType == E_MMSDK_MEDIA_TYPE_PHOTO)
            {
                u32Ret = (MS_U32)_geZoomFctor;
            }
            else
            {
                u32Ret = (MS_U32)0xFFFF;
            }
            break;


        case MPLAYER_OPTION_GET_PHOTO_INFO:
            switch(u32SubOption)
            {
                case MPLAYER_INFO_PHOTO_DECODED_WIDTH:
                    MM_GET_OPTION(stParam, E_MMSDK_OPTION_PHOTO_INFO,
                        E_MMSDK_INFO_PHOTO_OUTPUT_WIDTH, 4, (void*)&u32Ret, _gItem[eMediaType]);
                    break;

                case MPLAYER_INFO_PHOTO_DECODED_HEIGHT:
                    MM_GET_OPTION(stParam, E_MMSDK_OPTION_PHOTO_INFO,
                        E_MMSDK_INFO_PHOTO_OUTPUT_HEIGHT, 4, (void*)&u32Ret, _gItem[eMediaType]);
                    break;

                case MPLAYER_INFO_PHOTO_DECODED_PITCH:
                    MM_GET_OPTION(stParam, E_MMSDK_OPTION_PHOTO_INFO,
                        E_MMSDK_INFO_PHOTO_OUTPUT_PITCH, 4, (void*)&u32Ret, _gItem[eMediaType]);
                    break;

                case MPLAYER_INFO_PHOTO_DECODE_PROGRESS:
                    MM_GET_OPTION(stParam, E_MMSDK_OPTION_PHOTO_INFO,
                        E_MMSDK_INFO_PHOTO_DECODE_PROGRESS, sizeof(MS_U32), (void*)&u32Ret, _gItem[eMediaType]);
                    break;

                case MPLAYER_INFO_PHOTO_IMAGE_ORIGINAL_WIDTH:
                    MM_GET_OPTION(stParam, E_MMSDK_OPTION_PHOTO_INFO,
                        E_MMSDK_INFO_HRES, 4, (void*)&u32Ret, _gItem[eMediaType]);
                    break;

                case MPLAYER_INFO_PHOTO_IMAGE_ORIGINAL_HEIGHT:
                    MM_GET_OPTION(stParam, E_MMSDK_OPTION_PHOTO_INFO,
                        E_MMSDK_INFO_VRES, 4, (void*)&u32Ret, _gItem[eMediaType]);
                    break;

                case MPLAYER_INFO_PHOTO_DISPLAYING_HEIGHT:
                    u32Ret = _gstRect[E_MMSDK_MEDIA_TYPE_PHOTO].u16Height;
                    break;

                case MPLAYER_INFO_PHOTO_DISPLAYING_WIDTH:
                    u32Ret = _gstRect[E_MMSDK_MEDIA_TYPE_PHOTO].u16Width;
                    break;

                case MPLAYER_INFO_PHOTO_DISPLAYING_SIZE:
                    u32Ret = _gstRect[E_MMSDK_MEDIA_TYPE_PHOTO].u16Width * _gstRect[E_MMSDK_MEDIA_TYPE_PHOTO].u16Height;
                    break;

                case MPLAYER_INFO_PHOTO_CODEC_TYPE:
                    MM_GET_OPTION(stParam, E_MMSDK_OPTION_PHOTO_INFO,
                        E_MMSDK_INFO_PHOTO_TYPE, 4, (void*)&u32Ret, _gItem[eMediaType]);
                    break;

                case MPLAYER_INFO_PHOTO_DECODE_FORMAT:
                    MM_GET_OPTION(stParam, E_MMSDK_OPTION_PHOTO_INFO,
                        E_MMSDK_INFO_PHOTO_OUTPUT_COLOR_FORMAT, 4, (void*)&u32Ret, _gItem[eMediaType]);
                    break;

                default:
                    mTBOX_TRACE((kTBOX_NIV_WARNING, "[ERROR] Unknown PHOTO INFO\n"));
                    break;
            }
            break;

        case MPLAYER_OPTION_MUSIC_INFO:
            switch(u32SubOption)
            {
                case MPLAYER_INFO_TOTAL_TIME:
                    MM_GET_OPTION(stParam, E_MMSDK_OPTION_MUSIC_INFO,
                        E_MMSDK_INFO_TOTAL_TIME, 4, (void*)&u32Ret, _gItem[eMediaType]);
                    break;

                case MPLAYER_INFO_CUR_TIME:
                    MMSDK_GetPlayPosition(_gItem[eMediaType], &u32Ret);
                    break;

                case MPLAYER_INFO_MUSIC_BITRATE_LEN:
                    MM_GET_OPTION(stParam, E_MMSDK_OPTION_MUSIC_INFO_LENGTH,
                        E_MMSDK_INFO_MUSIC_BITRATE, sizeof(MS_U32), (void*)&_gu32MetalLen, _gItem[eMediaType]);

                    if(!_TDAL_GetOption_AllocateMemory(2UL * 2 * _gu32MetalLen))
                        return 0;

                    if (_gpu8Buffer)
                    {
                        MM_GET_OPTION(stParam, E_MMSDK_OPTION_MUSIC_INFO,
                            E_MMSDK_INFO_MUSIC_BITRATE, _gu32MetalLen, (void*)_gpu8Buffer, _gItem[eMediaType]);
                    }

                    _TDAL_itoa(*((MS_U32*)_gpu8Buffer), numStr, 10);
                    u32Ret = strlen((const char *)numStr);
                    numStr[u32Ret] = 'K';
                    numStr[u32Ret+1] = '\0';
                    _TDAL_UTF8ToUCS2((MS_U8 *)numStr, _gpu16UnicdBuffer, (MS_U16)strlen((const char*)numStr), 0xffff);
                    u32Ret = strlen((const char*)numStr) * 2;
                    break;

            case MPLAYER_INFO_MUSIC_LAYER_LEN:
                u32Ret = 1;
                break;

            case MPLAYER_INFO_MUSIC_LAYER:
                if (_gpu16UnicdBuffer == NULL)
                {
                    _gpu16UnicdBuffer = (MS_U16*)AllocNoncachedMem(4 * 1); //malloc memory for unicode
                    ASSERT(_gpu16UnicdBuffer);
                }

                u32Ret = (MS_U32)_gpu16UnicdBuffer;
                break;

            case MPLAYER_INFO_MUSIC_GENRE_LEN:
                u32Ret = 1;
                break;

            case MPLAYER_INFO_MUSIC_GENRE:
                if (_gpu16UnicdBuffer == NULL)
                {
                    _gpu16UnicdBuffer = (MS_U16*)AllocNoncachedMem(4 * 1); //malloc memory for unicode
                    ASSERT(_gpu16UnicdBuffer);
                }

                u32Ret = (MS_U32)_gpu16UnicdBuffer;
                break;

            case MPLAYER_INFO_MUSIC_COMMENT_LEN:
                u32Ret = 1;
                break;

            case MPLAYER_INFO_MUSIC_COMMENT:
                if (_gpu16UnicdBuffer == NULL)
                {
                    _gpu16UnicdBuffer = (MS_U16*)AllocNoncachedMem(4 * 1); //malloc memory for unicode
                }
                u32Ret = (MS_U32)_gpu16UnicdBuffer;
                break;

            case MPLAYER_INFO_MUSIC_CHECK_FF_FB_LEN:
                u32Ret = 1;
                break;

            case MPLAYER_INFO_MUSIC_CHECK_FF_FB:
                if (_gpu16UnicdBuffer == NULL)
                {
                    _gpu16UnicdBuffer = (MS_U16*)AllocNoncachedMem(4 * 1); //malloc memory for unicode
                    ASSERT(_gpu16UnicdBuffer);
                }

                u32Ret = (MS_U32)_gpu16UnicdBuffer;
                break;

            case MPLAYER_INFO_MUSIC_SAMPLINGRATE:
                u32Ret = (MS_U32)_gpu16UnicdBuffer;
                break;

            case MPLAYER_INFO_MUSIC_SAMPLINGRATE_LEN:
                MM_GET_OPTION(stParam, E_MMSDK_OPTION_MUSIC_INFO_LENGTH,
                    E_MMSDK_INFO_MUSIC_SAMPLE_FREQ, sizeof(MS_U32), (void*)&_gu32MetalLen, _gItem[eMediaType]);

                if(!_TDAL_GetOption_AllocateMemory(4 * 4))
                    return 0;

                if (_gpu8Buffer)
                {
                    MM_GET_OPTION(stParam, E_MMSDK_OPTION_MUSIC_INFO,
                    E_MMSDK_INFO_MUSIC_SAMPLE_FREQ, _gu32MetalLen, (void*)_gpu8Buffer, _gItem[eMediaType]);
                }

                _TDAL_itoa(*((MS_U32 *)_gpu8Buffer), numStr, 10);
                _TDAL_UTF8ToUCS2((MS_U8 *)numStr, _gpu16UnicdBuffer, (MS_U16)strlen((const char *)numStr), 0xffff);
                u32Ret = strlen((const char*)numStr) * 2;
                break;

            case MPLAYER_INFO_MUSIC_YEAR_LEN:
                MM_GET_OPTION(stParam, E_MMSDK_OPTION_MUSIC_INFO_LENGTH,
                    E_MMSDK_INFO_MUSIC_YEAR, sizeof(MS_U32), (void*)&_gu32MetalLen, _gItem[eMediaType]);

                MM_GET_OPTION(stParam, E_MMSDK_OPTION_MUSIC_INFO,
                    E_MMSDK_INFO_MUSIC_YEAR_FORMAT, 4, (void*)&eStrFormt, _gItem[eMediaType]);

                if (eStrFormt == E_PLAYER_STRING_FORMAT_UTF8)
                    u32UnicdBufferSize = 2UL * 2 * _gu32MetalLen;
                else
                    u32UnicdBufferSize = 2 * _gu32MetalLen;

                if(!_TDAL_GetOption_AllocateMemory(u32UnicdBufferSize))
                    return 0;

                if (_gpu8Buffer)
                {
                    MM_GET_OPTION(stParam, E_MMSDK_OPTION_MUSIC_INFO,
                        E_MMSDK_INFO_MUSIC_YEAR, _gu32MetalLen, (void*)_gpu8Buffer, _gItem[eMediaType]);

                    u32Ret = _TDAL_GetOption_CharEncConv(eStrFormt, FALSE);
                }
                break;

            case MPLAYER_INFO_MUSIC_YEAR:
                u32Ret = (MS_U32)_gpu16UnicdBuffer;
                break;

            case MPLAYER_INFO_MUSIC_ARTIST_LEN:
                MM_GET_OPTION(stParam, E_MMSDK_OPTION_MUSIC_INFO_LENGTH,
                    E_MMSDK_INFO_MUSIC_ARTIST, sizeof(MS_U32), (void*)&_gu32MetalLen, _gItem[eMediaType]);

                MM_GET_OPTION(stParam, E_MMSDK_OPTION_MUSIC_INFO,
                    E_MMSDK_INFO_MUSIC_ARTIST_FORMAT, 4, (void*)&eStrFormt, _gItem[eMediaType]);

                if (eStrFormt == E_PLAYER_STRING_FORMAT_UTF8)
                    u32UnicdBufferSize = 2UL * 2 * _gu32MetalLen;
                else
                    u32UnicdBufferSize = 2 * _gu32MetalLen;

                if(!_TDAL_GetOption_AllocateMemory(u32UnicdBufferSize))
                    return 0;

                if (_gpu8Buffer)
                {
                    MM_GET_OPTION(stParam, E_MMSDK_OPTION_MUSIC_INFO,
                        E_MMSDK_INFO_MUSIC_ARTIST, _gu32MetalLen, (void*)_gpu8Buffer, _gItem[eMediaType]);

                    u32Ret = _TDAL_GetOption_CharEncConv(eStrFormt, TRUE);
                }
                break;

            case MPLAYER_INFO_MUSIC_ARTIST:
                u32Ret = (MS_U32)_gpu16UnicdBuffer;
                break;

            case MPLAYER_INFO_MUSIC_ALBUM_LEN:
                MM_GET_OPTION(stParam, E_MMSDK_OPTION_MUSIC_INFO_LENGTH,
                    E_MMSDK_INFO_MUSIC_ALBUM, sizeof(MS_U32), (void*)&_gu32MetalLen, _gItem[eMediaType]);

                MM_GET_OPTION(stParam, E_MMSDK_OPTION_MUSIC_INFO,
                    E_MMSDK_INFO_MUSIC_ALBUM_FORMAT, 4, (void*)&eStrFormt, _gItem[eMediaType]);

                if (eStrFormt == E_PLAYER_STRING_FORMAT_UTF8)
                    u32UnicdBufferSize = 2UL * 2 * _gu32MetalLen;
                else
                    u32UnicdBufferSize = 2 * _gu32MetalLen;

                if(!_TDAL_GetOption_AllocateMemory(u32UnicdBufferSize))
                    return 0;

                if (_gpu8Buffer)
                {
                    MM_GET_OPTION(stParam, E_MMSDK_OPTION_MUSIC_INFO,
                        E_MMSDK_INFO_MUSIC_ALBUM, _gu32MetalLen, (void*)_gpu8Buffer, _gItem[eMediaType]);

                    u32Ret = _TDAL_GetOption_CharEncConv(eStrFormt, TRUE);
                }
                break;

            case MPLAYER_INFO_MUSIC_ALBUM:
                u32Ret = (MS_U32)_gpu16UnicdBuffer;
                break;

            case MPLAYER_INFO_MUSIC_TITLE_LEN:
                MM_GET_OPTION(stParam, E_MMSDK_OPTION_MUSIC_INFO_LENGTH,
                    E_MMSDK_INFO_MUSIC_TITLE, sizeof(MS_U32), (void*)&_gu32MetalLen, _gItem[eMediaType]);

                MM_GET_OPTION(stParam, E_MMSDK_OPTION_MUSIC_INFO,
                    E_MMSDK_INFO_MUSIC_TITLE_FORMAT, 4, (void*)&eStrFormt, _gItem[eMediaType]);

                if (eStrFormt == E_PLAYER_STRING_FORMAT_UTF8)
                    u32UnicdBufferSize = 2UL * 2 * _gu32MetalLen;
                else
                    u32UnicdBufferSize = 2 * _gu32MetalLen;

                if(!_TDAL_GetOption_AllocateMemory(u32UnicdBufferSize))
                    return 0;

                if (_gpu8Buffer)
                {
                    MM_GET_OPTION(stParam, E_MMSDK_OPTION_MUSIC_INFO,
                        E_MMSDK_INFO_MUSIC_TITLE, _gu32MetalLen, (void*)_gpu8Buffer, _gItem[eMediaType]);

                    u32Ret = _TDAL_GetOption_CharEncConv(eStrFormt, TRUE);
                }
                break;

            case MPLAYER_INFO_MUSIC_TITLE:
                u32Ret = (MS_U32)_gpu16UnicdBuffer;
                break;
            }
            break;

    default:
        break;
    }

    TDAL_UnlockMutex(_gs32MutexOpenStop);
    return u32Ret;
}
MS_BOOL TDAL_MPm_Player_SetOption(const EN_MMSDK_MEDIA_TYPE eMediaType, const EN_MPLAYER_OPTION_TYPE eOption, const unsigned int u32Param)
{
    MS_U8 u8R = 0, u8G = 0, u8B = 0;
    MS_BOOL bRet = FALSE;
    MS_U32 u32TotalTrackCnt = 0;
    MS_BOOL bPlayerStatus = TRUE;
    MS_U32 u32TrackNum = u32Param;
    ST_RECT_SIGNED_POSITION sDispWindow;
    const MPLAYER_MOVEVIEW_PARAM *pMoveView_Param = (const MPLAYER_MOVEVIEW_PARAM *)u32Param;
    const MPLAYER_DISP_WINDOW *pDisplayWindow = (const MPLAYER_DISP_WINDOW *)u32Param;
    const MPLAYER_PCM_PARAM *pPCM_Param = (const MPLAYER_PCM_PARAM *)u32Param;
    ST_MMSDK_PARAM stParam = {.u32Option = E_MMSDK_OPTION_MOVIE_INFO, .u32Paramsize = sizeof(u32TotalTrackCnt), .pParam = (void*)&u32TotalTrackCnt};

    if (eMediaType >= MMSDK_MEDIA_TYPE_NUM)
    {
        return FALSE;
    }

    if (eOption == MPLAYER_OPTION_SPECIFIC_TOTAL_TIME) //avoid recursion lock(both playfile and set_option called)
    {
        _gu32TotalTime = (MS_U32)u32Param;
        mTBOX_TRACE((kTBOX_NIV_1, "-------> _gu32TotalTime(%ld)\n", _gu32TotalTime));
        return TRUE;
    }

    if (eOption == MPLAYER_OPTION_SET_STARTTIME) //avoid recursion lock(both playfile and set_option called)
    {
        _gu32StartTime = (MS_U32)u32Param;
        mTBOX_TRACE((kTBOX_NIV_1, "-------> _gu32StartTime(%ld)\n", _gu32StartTime));
        return TRUE;
    }

    if (eOption == MPLAYER_OPTION_ENABLE_NEW_AUDIO_MODE)
    {
        _gbUseNewAudioMode = (MS_BOOL)u32Param;
        mTBOX_TRACE((kTBOX_NIV_1, "-------> _gbUseNewAudioMode(%d)\n", _gbUseNewAudioMode));
        return TRUE;
    }

    TDAL_LockMutex(_gs32MutexOpenStop);
    switch(eOption)
    {
        case MPLAYER_OPTION_SUBTITLE_MEM_LESS_MODE://currently not supported
            break;
        case MPLAYER_OPTION_SUBTITLE_DYN_ONSCRREN_MODE://currently not supported
            break;

        case MPLAYER_OPTION_SET_SEAMLESS_MODE:
            _geSeamlessMode = (EN_SEAMLESS_MODE)u32Param;
            break;

        case MPLAYER_OPTION_UP_BOUNDARY:
            _gs32MaxBufferSec = (MS_S32)u32Param;
            break;

        case MPLAYER_OPTION_DOWN_BOUNDARY:
            _gs32MinBufferSec = (MS_S32)u32Param;
            break;

        case MPLAYER_OPTION_READ_TIME_OUT:
            _gu32VdplayerReadTimerOut = u32Param;
            break;

        case MPLAYER_OPTION_SET_DBG_LEVEL:
            _geDebugLevel = (EN_MMSDK_DBG_LEVEL)u32Param;
            bRet = MMSDK_DebugMSGLevel(_geDebugLevel);
            break;

        case MPLAYER_OPTION_INIT_DATA_SIZE:
            _gu32InitDataSize = u32Param;
            break;

        case MPLAYER_OPTION_ENABLE_FAST_INIT:
            _gbFastInit = (MS_BOOL)u32Param;
            break;

        case MPLAYER_OPTION_ENABLE_SLIDE_SHOW:
            if ((MS_BOOL)u32Param == TRUE)
                bRet = MMSDK_SetSlideShowEffect(E_MMSDK_SLIDE_SHOW_EFFECT_RANDOM);
            break;

        case MPLAYER_OPTION_DISABLE_SLIDE_SHOW:
            
            if ((MS_BOOL)u32Param == TRUE)
            {
                bRet = MMSDK_SetSlideShowEffect(E_MMSDK_SLIDE_SHOW_EFFECT_NORMAL);
            }
            break;

        case MPLAYER_OPTION_STOP_PHOTO_DISPLAY:
            bRet = MMSDK_ClearPhotoDisplay(u8R, u8G, u8B);
            break;

        case MPLAYER_OPTION_REPEATAB:
            bRet = MMSDK_SetRepeatAB(_gItem[eMediaType], (EN_MMSDK_REPEAT_AB)u32Param);
            break;

        case MPLAYER_OPTION_FB_REPLAY:
            if (eMediaType == E_MMSDK_MEDIA_TYPE_MUSIC)
            {
                _gbMusicBackwardRePlayMode = (MS_BOOL)u32Param;
            }
            else
            {
                _gbBackwardRePlayMode = (MS_BOOL)u32Param;
            }
            bRet = TRUE;
            break;

        case MPLAYER_OPTION_ZOOM:
            bRet = MMSDK_SetZoomFactor(_gItem[eMediaType], u32Param);
            if(bRet)
                _geZoomFctor = u32Param;
            break;

        case MPLAYER_OPTION_ROTATION:
            bRet = MMSDK_SetRotateAngle(_gItem[eMediaType], u32Param);
            if(bRet)
                _geRotaAng = u32Param;
            break;

        case MPLAYER_OPTION_MOVE_VIEW:
            bRet = MMSDK_MoveViewWindow(
                                _gItem[eMediaType],
                                (EN_MMSDK_VIEW_MOVE_DIRECTION)pMoveView_Param->eDirection,
                                pMoveView_Param->u16Offset);
            break;

        case MPLAYER_OPTION_ASPECT_RATIO:
            bRet = MMSDK_SetAspectRatio(E_MMSDK_MEDIA_TYPE_MOVIE, u32Param);
            break;

        case MPLAYER_OPTION_SET_KTV_MODE:
            if (eMediaType == E_MMSDK_MEDIA_TYPE_MOVIE)
            {
                _gbKTVMode = (MS_BOOL)u32Param;
                bRet = TRUE;
            }
            break;

        case MPLAYER_OPTION_CHANGE_PROGRAM:
            if (eMediaType == E_MMSDK_MEDIA_TYPE_MOVIE)
            {
                stParam.u32SubOption = E_MMSDK_INFO_MOVIE_NB_PROGRAM;
                bPlayerStatus = MMSDK_GetOptionByPara(_gItem[eMediaType], &stParam, 1);
                if ((bPlayerStatus == FALSE) || (u32TrackNum >= u32TotalTrackCnt))
                {
                    goto FUNC_END;
                }

                ST_MMSDK_TS_INFO stMMSDK_TSInfo = {};
                bRet = MMSDK_SetProgramID(_gItem[eMediaType], u32TrackNum, &stMMSDK_TSInfo);
            }
            break;

        case MPLAYER_OPTION_CHANGE_AUDIO:
            if (eMediaType == E_MMSDK_MEDIA_TYPE_MOVIE)
            {
                stParam.u32SubOption = E_MMSDK_INFO_MOVIE_NB_AUDIO_TRACK;
                bPlayerStatus = MMSDK_GetOptionByPara(_gItem[eMediaType], &stParam, 1);
                if ((bPlayerStatus == FALSE) || (u32TrackNum >= u32TotalTrackCnt))
                {
                    goto FUNC_END;
                }

                bRet = MMSDK_SetAudioTrackID(_gItem[eMediaType], u32TrackNum);
            }
            break;

        case MPLAYER_OPTION_CHANGE_INTERNAL_SUBTITLE:
            if (eMediaType == E_MMSDK_MEDIA_TYPE_MOVIE)
            {
                stParam.u32SubOption = E_MMSDK_INFO_MOVIE_NB_SUBTITLE_TRACK;
                bPlayerStatus = MMSDK_GetOptionByPara(_gItem[eMediaType], &stParam, 1);
                if ((bPlayerStatus == FALSE) || (u32TrackNum >= u32TotalTrackCnt))
                {
                    goto FUNC_END;
                }

                bRet = MMSDK_SetSubtitleTrackID(_gItem[eMediaType], u32TrackNum);
            }
            break;

        case MPLAYER_OPTION_SET_MUSIC_TYPE:
            if (eMediaType == E_MMSDK_MEDIA_TYPE_MUSIC)
            {
                _geAudioType = (EN_MPLAYER_SUB_TYPE)u32Param;
                bRet = TRUE;
            }
            break;

        case MPLAYER_OPTION_MUSIC_SET_PCM_PARAM:
            if (eMediaType == E_MMSDK_MEDIA_TYPE_MUSIC)
            {
                _gu8AudioPara[0] = (MS_U8)pPCM_Param->eFormatTag;
                SETU16(_gu8AudioPara, 1, pPCM_Param->u16Channels);
                SETU16(_gu8AudioPara, 3, pPCM_Param->u16BitsPerSample);
                SETU32(_gu8AudioPara, 5, pPCM_Param->u32SamplesPerSec);
                _gu8AudioPara[9] = (MS_U8)pPCM_Param->bBigEndian;
                bRet = TRUE;
            }
            break;

        case MPLAYER_OPTION_DISPLAY_WINDOW:
            sDispWindow.s32X = pDisplayWindow->u32DispX;
            sDispWindow.s32Y = pDisplayWindow->u32DispY;
            sDispWindow.u16Width = pDisplayWindow->u32DispW;
            sDispWindow.u16Height = pDisplayWindow->u32DispH;
            mTBOX_TRACE((kTBOX_NIV_1, "--------DISP_WINDOW[%ld][%ld][%d][%d]\n", sDispWindow.s32X, sDispWindow.s32Y, sDispWindow.u16Width, sDispWindow.u16Height));
            if (eMediaType == E_MMSDK_MEDIA_TYPE_MOVIE)
                bRet = _TDAL_MM_SetDisplayRectangle(_gItem[eMediaType], &sDispWindow, TRUE, TRUE);
            break;

        default:
            break;
    }

FUNC_END:
    TDAL_UnlockMutex(_gs32MutexOpenStop);
    return bRet;
}

tTDAL_MP_Error TDAL_MPm_PictureGetMetadata(tTDAL_MP_MetaDataPict *pstMetadata)
{    
    tTDAL_MP_Error result = eTDAL_MP_NO_ERROR;
    ST_MMSDK_PARAM stParam = {};
    MS_U32 u32Ret = 0;
    MS_U64 pixels = 0;
    mTBOX_FCT_ENTER("TDAL_MPm_PictureGetMetadata");
    
    if (pstMetadata == NULL)
    {
        result = eTDAL_MP_BAD_PARAMETER_ERROR;
    }
    
    if (!_bIsFileSupport)
    {
        result = eTDAL_MP_NOT_SUPPORTED_ERROR;
        mTBOX_RETURN(result);
    }
    
    u32Ret = 0;
    MM_GET_OPTION(stParam, E_MMSDK_OPTION_PHOTO_INFO,
                    E_MMSDK_INFO_PHOTO_TYPE, 4, (void*)&u32Ret, _gItem[_gstMedia.enMediaType]);
    
    if (u32Ret != E_MMSDK_PHOTO_UNKNOWN)
    {  
        pstMetadata->bIsSupported = TRUE;
    }
    
    u32Ret = 0;
    MM_GET_OPTION(stParam, E_MMSDK_OPTION_PHOTO_INFO,
                    E_MMSDK_INFO_PHOTO_JPEG_EXIF, 4, (void*)&u32Ret, _gItem[_gstMedia.enMediaType]);
    if (u32Ret)
    {  
        pstMetadata->bIsExifPresent = TRUE;
    }
    
    u32Ret = 0;
    MM_GET_OPTION(stParam, E_MMSDK_OPTION_PHOTO_INFO,
                    E_MMSDK_INFO_PHOTO_OUTPUT_WIDTH, 4, (void*)&u32Ret, _gItem[_gstMedia.enMediaType]);
    if (u32Ret)
    {  
        pstMetadata->uiWidth = u32Ret;
    }
    
    u32Ret = 0;
    MM_GET_OPTION(stParam, E_MMSDK_OPTION_PHOTO_INFO,
                    E_MMSDK_INFO_PHOTO_OUTPUT_HEIGHT, 4, (void*)&u32Ret, _gItem[_gstMedia.enMediaType]);
    if (u32Ret)
    {  
        pstMetadata->uiHeight = u32Ret;
    }
    
    pixels = ((MS_U64)pstMetadata->uiWidth)*((MS_U64)pstMetadata->uiHeight);
    u32Ret = (MS_U32)(_gu64Size[_gstMedia.enMediaType]*8/pixels);
    pstMetadata->usBitPerPixel = (MS_U16)u32Ret;
    
    mTBOX_RETURN(result);
}
tTDAL_MP_Error TDAL_MPm_AudioGetMetadata(tTDAL_MP_MetaDataAudio *pstMetadata)
{
    tTDAL_MP_Error result = eTDAL_MP_NO_ERROR;
    ST_MMSDK_PARAM stParam = {};
    MS_U32 u32Ret = 0;
    mTBOX_FCT_ENTER("TDAL_MPm_AudioGetMetadata");
    
    if (pstMetadata == NULL)
    {
        result = eTDAL_MP_BAD_PARAMETER_ERROR;
    }
    
    if (!_bIsFileSupport)
    {
        result = eTDAL_MP_NOT_SUPPORTED_ERROR;
    }
    
    pstMetadata->bIsID3Present = 0;

    if (result == eTDAL_MP_NO_ERROR)
    {    
        u32Ret = 0;

        MM_GET_OPTION(stParam, E_MMSDK_OPTION_MUSIC_INFO,
                        E_MMSDK_INFO_MUSIC_AUDIO_CODEC, 4, (void*)&u32Ret, _gItem[_gstMedia.enMediaType]);
        
        pstMetadata->bIsSupported = _MM_CheckAdecSupport((EN_MMSDK_AUDIO_CODEC)u32Ret);
        
        u32Ret = 0;
        MM_GET_OPTION(stParam, E_MMSDK_OPTION_MUSIC_INFO,
                        E_MMSDK_INFO_TOTAL_TIME, 4, (void*)&u32Ret, _gItem[_gstMedia.enMediaType]);
        pstMetadata->stDuration = u32Ret;

        u32Ret = 0;
        MM_GET_OPTION(stParam, E_MMSDK_OPTION_MUSIC_INFO,
                        E_MMSDK_INFO_MUSIC_BITRATE, 4, (void*)&u32Ret, _gItem[_gstMedia.enMediaType]);
        pstMetadata->uiBitrate = u32Ret;

        pstMetadata->uiFrameSize = 0;
        pstMetadata->uiFrequency = 0;

        u32Ret = 0;
        u32Ret = (MS_U32)MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_SampleRate);
        switch (u32Ret)
        {
            case AUDIO_FS_8KHZ :
                pstMetadata->uiSamplePerframe = 8000;
                break;
            case AUDIO_FS_11KHZ :
                pstMetadata->uiSamplePerframe = 11000;
                break;
            case AUDIO_FS_12KHZ :
                pstMetadata->uiSamplePerframe = 12000;
                break;
            case AUDIO_FS_16KHZ :
                pstMetadata->uiSamplePerframe = 16000;
                break;
            case AUDIO_FS_22KHZ :
                pstMetadata->uiSamplePerframe = 22000;
                break;
            case AUDIO_FS_24KHZ :
                pstMetadata->uiSamplePerframe = 24000;
                break;
            case AUDIO_FS_32KHZ :
                pstMetadata->uiSamplePerframe = 32000;
                break;
            case AUDIO_FS_44KHZ :
                pstMetadata->uiSamplePerframe = 44000;
                break;
            case AUDIO_FS_48KHZ :
                pstMetadata->uiSamplePerframe = 48000;
                break;
            case AUDIO_FS_64KHZ :
                pstMetadata->uiSamplePerframe = 64000;
                break;
            case AUDIO_FS_88KHZ :
                pstMetadata->uiSamplePerframe = 88000;
                break;
            case AUDIO_FS_96KHZ :
                pstMetadata->uiSamplePerframe = 96000;
                break;
            case AUDIO_FS_176KHZ :
                pstMetadata->uiSamplePerframe = 176000;
                break;
            case AUDIO_FS_192KHZ : 
                pstMetadata->uiSamplePerframe = 192000;
                break;
            case AUDIO_FS_NONE :
            case AUDIO_FS_UNSUPPORT :                
                pstMetadata->uiSamplePerframe = 0;
                break;
        }

        pstMetadata->uiAudioDataEnd = 0;//unknow requirement
        pstMetadata->uiAudioDataStart = 0;//unknow requirement
    }

    mTBOX_RETURN(result);
}
tTDAL_MP_Error TDAL_MPm_AVGetMetadata(tTDAL_MP_MetaDataAv *pstMetadata)
{
    tTDAL_MP_Error result = eTDAL_MP_NO_ERROR;
    ST_MMSDK_PARAM stParam = {};
    MS_U32 u32Ret = 0;
    mTBOX_FCT_ENTER("TDAL_MPm_AVGetMetadata");
    
    if (pstMetadata == NULL)
    {
        result = eTDAL_MP_BAD_PARAMETER_ERROR;
    }

    if (!_bIsFileSupport)
    {
        result = eTDAL_MP_NOT_SUPPORTED_ERROR;
    }
    
    if (result == eTDAL_MP_NO_ERROR)
    {
        u32Ret = 0;
        MM_GET_OPTION(stParam, E_MMSDK_OPTION_MOVIE_INFO,
                    E_MMSDK_INFO_TOTAL_TIME, 4, (void*)&u32Ret, _gItem[_gstMedia.enMediaType]);

        pstMetadata->stDuration = u32Ret/1000;

        u32Ret = 0;
        MM_GET_OPTION(stParam, E_MMSDK_OPTION_MOVIE_INFO,
                        E_MMSDK_INFO_MOVIE_NB_PROGRAM, 4, (void*)&u32Ret, _gItem[_gstMedia.enMediaType]);
        if (u32Ret == 0)
        {
            pstMetadata->bIsVideoPresent = FALSE;
        }
        else
        {
            pstMetadata->bIsVideoPresent = TRUE;
        }
        
        if (pstMetadata->bIsVideoPresent)
        {
            u32Ret = 0;
            MM_GET_OPTION(stParam, E_MMSDK_OPTION_MOVIE_INFO,
                            E_MMSDK_INFO_MOVIE_VIDEO_CODEC, 4, (void*)&u32Ret, _gItem[_gstMedia.enMediaType]);            
             
            pstMetadata->Video.bIsSupported = _MM_CheckVdecSupport((EN_MMSDK_VIDEO_CODEC)u32Ret);
            pstMetadata->Video.eType = _MM_VdecConvert((EN_MMSDK_AUDIO_CODEC)u32Ret);
        
            u32Ret = 0;
            MM_GET_OPTION(stParam, E_MMSDK_OPTION_MOVIE_INFO,
                            E_MMSDK_INFO_VRES, 4, (void*)&u32Ret, _gItem[_gstMedia.enMediaType]);
            pstMetadata->Video.uiWidth = u32Ret;

            u32Ret = 0;
            MM_GET_OPTION(stParam, E_MMSDK_OPTION_MOVIE_INFO,
                            E_MMSDK_INFO_HRES, 4, (void*)&u32Ret, _gItem[_gstMedia.enMediaType]);
            pstMetadata->Video.uiHeight = u32Ret;

            u32Ret = 0;
            MM_GET_OPTION(stParam, E_MMSDK_OPTION_MOVIE_INFO,
                            E_MMSDK_INFO_MOVIE_FRAME_PER_SEC, 4, (void*)&u32Ret, _gItem[_gstMedia.enMediaType]);
            pstMetadata->Video.iFramerate = u32Ret;

            VDEC_StreamId streamID;
            memset(&streamID,0,sizeof(VDEC_StreamId));
            MApi_VDEC_EX_GetControl(&streamID, E_VDEC_EX_USER_CMD_GET_MAIN_STREAM_ID, (MS_U32 *)&streamID);
            pstMetadata->Video.iBitrate = MApi_VDEC_EX_GetBitsRate(&streamID);//Not Support

        }
        else
        {
            pstMetadata->Video.bIsSupported = FALSE;
            pstMetadata->Video.eType = eTDAL_MP_STREAM_CONTENT_INVALID;
        }
        
        u32Ret = 0;
        MM_GET_OPTION(stParam, E_MMSDK_OPTION_MOVIE_INFO,
                        E_MMSDK_INFO_MOVIE_NB_AUDIO_TRACK, 4, (void*)&u32Ret, _gItem[_gstMedia.enMediaType]);
                
        if (u32Ret == 0)
        {
            pstMetadata->bIsAudioPresent = FALSE;
        }
        else
        {
            pstMetadata->bIsAudioPresent = TRUE;
        }

        if (pstMetadata->bIsAudioPresent)
        {       

            u32Ret = 0;
            MM_GET_OPTION(stParam, E_MMSDK_OPTION_MOVIE_INFO,
                            E_MMSDK_INFO_MOVIE_AUDIO_CODEC, 4, (void*)&u32Ret, _gItem[_gstMedia.enMediaType]);
            
            pstMetadata->Audio.eType = _MM_AdecConvert((EN_MMSDK_AUDIO_CODEC)u32Ret);
            pstMetadata->Audio.bIsSupported = _MM_CheckAdecSupport((EN_MMSDK_AUDIO_CODEC)u32Ret);
            
            u32Ret = 0;
            u32Ret = (MS_U32)MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_SampleRate);
            switch (u32Ret)
            {
                case AUDIO_FS_8KHZ :
                    pstMetadata->Audio.iSamplerate = 8000;
                    break;
                case AUDIO_FS_11KHZ :
                    pstMetadata->Audio.iSamplerate = 11000;
                    break;
                case AUDIO_FS_12KHZ :
                    pstMetadata->Audio.iSamplerate = 12000;
                    break;
                case AUDIO_FS_16KHZ :
                    pstMetadata->Audio.iSamplerate = 16000;
                    break;
                case AUDIO_FS_22KHZ :
                    pstMetadata->Audio.iSamplerate = 22000;
                    break;
                case AUDIO_FS_24KHZ :
                    pstMetadata->Audio.iSamplerate = 24000;
                    break;
                case AUDIO_FS_32KHZ :
                    pstMetadata->Audio.iSamplerate = 32000;
                    break;
                case AUDIO_FS_44KHZ :
                    pstMetadata->Audio.iSamplerate = 44000;
                    break;
                case AUDIO_FS_48KHZ :
                    pstMetadata->Audio.iSamplerate = 48000;
                    break;
                case AUDIO_FS_64KHZ :
                    pstMetadata->Audio.iSamplerate = 64000;
                    break;
                case AUDIO_FS_88KHZ :
                    pstMetadata->Audio.iSamplerate = 88000;
                    break;
                case AUDIO_FS_96KHZ :
                    pstMetadata->Audio.iSamplerate = 96000;
                    break;
                case AUDIO_FS_176KHZ :
                    pstMetadata->Audio.iSamplerate = 176000;
                    break;
                case AUDIO_FS_192KHZ : 
                    pstMetadata->Audio.iSamplerate = 192000;
                    break;
                case AUDIO_FS_NONE :
                case AUDIO_FS_UNSUPPORT :                
                    pstMetadata->Audio.iSamplerate = 0;
                    break;
            }

            u32Ret = 0;
            MM_GET_OPTION(stParam, E_MMSDK_OPTION_MOVIE_INFO,
                            E_MMSDK_INFO_MOVIE_NB_AUDIO_TRACK, 4, (void*)&u32Ret, _gItem[_gstMedia.enMediaType]);
            if (u32Ret)
                pstMetadata->Audio.iChannels = u32Ret;

            pstMetadata->Audio.iBitrate = MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_ADEC1_bitRate);
        }
        else
        {
            pstMetadata->Audio.eType = eTDAL_MP_STREAM_CONTENT_INVALID;
            pstMetadata->Audio.bIsSupported = FALSE;
        }
    }
    mTBOX_RETURN(result);
 }

void TDAL_MPm_GetMEDIAITEM(MEDIAITEM *item)
{
    if (item == NULL)
    {
        return;
    }
    memcpy(item,&_gItem[_gstMedia.enMediaType],sizeof(MEDIAITEM));
}
MS_BOOL TDAL_MPm_Player_PlayMediaFile(void)
{
    MS_BOOL bRet = TRUE;

    if (_gstMedia.enMediaType == E_MMSDK_MEDIA_TYPE_MUSIC)
    {
        MM_SET_MUSIC_NOTIFY_TYPE(MPLAYER_START_PLAY);
        mp_printf("Send MPLAYER_START_PLAY to Music_EvtProcess.\n");
    }
    else if (_gstMedia.enMediaType == E_MMSDK_MEDIA_TYPE_PHOTO)
    {
            MM_SET_PHOTO_NOTIFY_TYPE(MPLAYER_START_PLAY);
            mp_printf("Send MPLAYER_START_PLAY to Photo_EvtProcess.\n");
    }
    else if (_gstMedia.enMediaType == E_MMSDK_MEDIA_TYPE_MOVIE)
    {
        MM_SET_VIDEO_NOTIFY_TYPE(MPLAYER_START_PLAY);
        mp_printf("Send MPLAYER_START_PLAY to Video_EvtProcess.\n");
    }
    else
    {
        mp_printf("Unknown MediaType.\n");
        bRet = FALSE;
    }
    return bRet;
}
MS_BOOL TDAL_MPm_Player_CreateMediaFile(const EN_MMSDK_MEDIA_TYPE eMediaType, MPLAYER_MEDIA* const pMplayerMedia)
{
    mTBOX_TRACE((kTBOX_NIV_1, "------> Enter func MApi_MPlayer_PlayFile \n"));

    TDAL_LockMutex(_gs32MutexOpenStop);
    BOOLEAN bRet = FALSE;
    const MS_U32 handle = eMediaType;
    ST_MMSDK_PARAM astExtenInfoParam;
    MMSDK_U32 u32ExtenInfoParamNum = 0;
    MS_U32 u32wait_init_ok_timer = 0;
    memset(&_gstMedia, 0, sizeof(ST_MMSDK_CREATE_MEDIA_INFO));
    _gstMedia.u32Version = MMSDK_VERSION;

    _gbExit[eMediaType] = FALSE;
    _geStrmMode[handle] =  pMplayerMedia->eFileMode;

    _gstRect[eMediaType].u16Height = pMplayerMedia->u32DispH;
    _gstRect[eMediaType].u16Width = pMplayerMedia->u32DispW;
    _gstRect[eMediaType].u16X = pMplayerMedia->u32DispX;
    _gstRect[eMediaType].u16Y = pMplayerMedia->u32DispY;

    if(pMplayerMedia->filename != NULL)
    {
        _gpStream[eMediaType] = _MM_FileOpen((char* const )pMplayerMedia->filename, (char* const)"rb");
        if(_gpStream[eMediaType] != NULL)
            _gu64Size[eMediaType] = _MM_FileLength(_gpStream[eMediaType]);
        else
        {
            mTBOX_TRACE((kTBOX_NIV_WARNING, "open stream failed!\n"));
            goto FUNC_END;
        }
    }
    else
    {
        mTBOX_TRACE((kTBOX_NIV_WARNING, "MApi_MPlayer_PlayFile(): Filename is NULL ......\n"));
        goto FUNC_END;
    }

    /// Up-layer application class point of call back function.(for application to distinguish)
    _gstMedia.pClass = &_gstMedia;
    //Regiter callback.
    _gstMedia.pCallback = _TDAL_MM_Callback;

#if(DEMO_AUDIO_MULTI_TEST == 1)
    if(eMediaType == E_MMSDK_MEDIA_TYPE_MOVIE || eMediaType == E_MMSDK_MEDIA_TYPE_MUSIC)
    {
        if(Demo_MM_AllocAudDec() == FALSE)
        {
            goto FUNC_END;
        }
    }
#endif

    if(_geAudioType == MPLAYER_SUBTYPE_PCM && eMediaType == E_MMSDK_MEDIA_TYPE_MUSIC) // PCM audio format
    {
        _gstWaveHdr.fileID[0] = 'R';
        _gstWaveHdr.fileID[1] = 'I';
        _gstWaveHdr.fileID[2] = 'F';
        _gstWaveHdr.fileID[3] = 'F';
        _gstWaveHdr.fileleth = 0;
        _gstWaveHdr.wavTag[0] = 'W';
        _gstWaveHdr.wavTag[1] = 'A';
        _gstWaveHdr.wavTag[2] = 'V';
        _gstWaveHdr.wavTag[3] = 'E';

        _gstWaveHdr.FmtHdrID[0] = 'f';
        _gstWaveHdr.FmtHdrID[1] = 'm';
        _gstWaveHdr.FmtHdrID[2] = 't';
        _gstWaveHdr.FmtHdrID[3] = ' ';
        _gstWaveHdr.FmtHdrLeth = 0;

        _gstWaveHdr.DataHdrID[0] = 'd';
        _gstWaveHdr.DataHdrID[1] = 'a';
        _gstWaveHdr.DataHdrID[2] = 't';
        _gstWaveHdr.DataHdrID[3] = 'a';
        _gstWaveHdr.DataHdrLeth = 0;

        _gstWaveHdr.fileleth = (MS_U32)_gu64Size[eMediaType] + 36;
        _gstWaveHdr.FmtHdrLeth = 16;
        _gstWaveHdr.BitsPerSample = ((MS_U16)_gu8AudioPara[4] << 8) +_gu8AudioPara[3];
        _gstWaveHdr.Channels = ((MS_U16)_gu8AudioPara[2] << 8) +_gu8AudioPara[1];
        _gstWaveHdr.FormatTag = 0x0001;
        _gstWaveHdr.SamplesPerSec = ((MS_U16)_gu8AudioPara[6] << 8) +_gu8AudioPara[5];
        _gstWaveHdr.BlockAlign = _gstWaveHdr.Channels * _gstWaveHdr.BitsPerSample / 8;
        _gstWaveHdr.AvgBytesPerSec = _gstWaveHdr.BlockAlign * _gstWaveHdr.SamplesPerSec;
        _gstWaveHdr.DataHdrLeth = (MS_U32)_gu64Size[eMediaType];
        _gu64Size[eMediaType] = (MS_S64)_gu64Size[eMediaType] + 44;
        
        mTBOX_TRACE((kTBOX_NIV_1, "--------> PCM music format----------\n"));
        mTBOX_TRACE((kTBOX_NIV_1, "---BitsPerSample is %d----------\n", _gstWaveHdr.BitsPerSample));
        mTBOX_TRACE((kTBOX_NIV_1, "---Channels is %d----------\n", _gstWaveHdr.Channels));
        mTBOX_TRACE((kTBOX_NIV_1, "---SamplesPerSec is %d----------\n", (int)_gstWaveHdr.SamplesPerSec));
        mTBOX_TRACE((kTBOX_NIV_1, "---Endian is %d----------\n", _gu8AudioPara[9]));
        
    }
    
    mTBOX_TRACE((kTBOX_NIV_1, "------> _gu64Size[eMediaType] : %lld \n", _gu64Size[eMediaType]));

    _gstMedia.u32CtlFlag = 0;
    if(eMediaType == E_MMSDK_MEDIA_TYPE_MOVIE && pMplayerMedia->eFileMode == MPLAYER_FILE_MODE)
    {
#if DEMO_FILE
        _gstMedia.u32CtlFlag |= E_MMSDK_CTL_FLAG_ENABLE_AUDIO_HANDLER;
        _gpStream[E_MPLAYER_MEDIA_STREAM_SUBTITLE] = _MM_FileOpen((char* const )pMplayerMedia->filename, (char* const)"rb");
        _gpStream[E_MPLAYER_MEDIA_STREAM_AUDIO] = _MM_FileOpen((char* const )pMplayerMedia->filename, (char* const)"rb");
        _gpStream[E_MPLAYER_MEDIA_STREAM_AUDIO2] = _MM_FileOpen((char* const )pMplayerMedia->filename, (char* const)"rb");
#else
        //_gstMedia.u32CtlFlag |= E_MMSDK_CTL_FLAG_ENABLE_AUDIO_HANDLER;
        _gpStream[E_MPLAYER_MEDIA_STREAM_SUBTITLE] = NULL;//_MM_FileOpen((char* const )pMplayerMedia->filename, (char* const)"rb");
        _gpStream[E_MPLAYER_MEDIA_STREAM_AUDIO] = NULL;//_MM_FileOpen((char* const )pMplayerMedia->filename, (char* const)"rb");
        _gpStream[E_MPLAYER_MEDIA_STREAM_AUDIO2] = NULL;//_MM_FileOpen((char* const )pMplayerMedia->filename, (char* const)"rb");        
#endif
    }
    else
        _gstMedia.u32CtlFlag |= E_MMSDK_CTL_FLAG_SUBTITLE_DISABLE;
    _gbUseNewAudioMode = FALSE;
    if (_gbUseNewAudioMode)
    {
        _gstMedia.u32CtlFlag |= E_MMSDK_CTL_FLAG_ENABLE_NEW_AUDIO_MODE;
    }

    if(_gbKTVMode)
    {
        _gstMedia.u32CtlFlag |= E_MMSDK_CTL_FLAG_VIDEO_ONLY;
    }
    _gstMedia.u32CtlFlag |= E_MMSDK_CTL_FLAG_TRICKMODE_ON;
    _gstMedia.u64FileLength = (MS_U64)_gu64Size[eMediaType];
    _gstMedia.pFileHandle1 = _gpStream[eMediaType];

#if DEMO_FILE
    if(_gpStream[E_MPLAYER_MEDIA_STREAM_SUBTITLE])//subtile
        _gstMedia.pFileHandle2 = _gpStream[E_MPLAYER_MEDIA_STREAM_SUBTITLE];
    else
        _gstMedia.pFileHandle2 = _gpStream[eMediaType];
#else
        _gstMedia.pFileHandle2 = NULL;
#endif
    _gstMedia.pFileHandle3 = _gpStream[E_MPLAYER_MEDIA_STREAM_AUDIO];
    _gstMedia.pFileHandle4 = _gpStream[E_MPLAYER_MEDIA_STREAM_AUDIO2];

    _gstMedia.enMediaType = eMediaType;

    if(pMplayerMedia->eFileMode == MPLAYER_FILE_MODE)
    {
        _gstMedia.enMediaFileType = E_MMSDK_FILE_TYPE_MASS_STORAGE;    //For USB content
    }

    if(_gu64Size[eMediaType] == (MS_U64) - 1)
    {
        _geStrmMode[handle] = MPLAYER_UNSEEKABLE_STREAM_MODE;
        _gstMedia.enMediaFileType = E_MMSDK_FILE_TYPE_NETWORK_STREAM_WITHOUT_SEEK;
    }

    // Do not demo motion GIF, so setting E_MMSDK_CREATE_MODE_PREVIEW to decode only first frame
    _gstMedia.enMediaCreateMode = E_MMSDK_CREATE_MODE_NORMAL_PLAY;

    ST_MMSDK_TIMEOUT_VALUE stTimeOutInfo;
    stTimeOutInfo.u32FileIOTimeOut = 0;//0xFFFFFFFF;
    stTimeOutInfo.u32FileReadTimeOut = 0;//0xFFFFFFFF;
    stTimeOutInfo.u32FileSeekTimeOut = 0;//0xFFFFFFFF;
    stTimeOutInfo.u32DecodeCtrlTimeOut = 0;//0xFFFFFFFF;
    stTimeOutInfo.u32ESBuffCtrlTimeOut = 0;//0xFFFFFFFF;
    MMSDK_SetPlayerTimeOutValue(&stTimeOutInfo);
    MMSDK_SetBackwardRePlayMode(_gbBackwardRePlayMode);
    if(_gu32TotalTime != 0)
    {
        MMSDK_SetStreamTotalTime(_gu32TotalTime);
    }
    if(_gu32StartTime != 0)
    {
        MMSDK_SetStreamStartTime(_gu32StartTime);
        _gu32StartTime = 0;
    }
    
    printf("=Test [%s][%d] time = %d\n",__FUNCTION__,__LINE__,MsOS_GetSystemTime());
    if(MMSDK_CreateMediaItem(&_gstMedia, &astExtenInfoParam, u32ExtenInfoParamNum, &_gItem[eMediaType]) != TRUE)
    {
        mTBOX_TRACE((kTBOX_NIV_WARNING, "!!!!!MM_CreateMediaItem returns fail in playfile \n"));
        goto FUNC_END;
    }
    printf("=Test [%s][%d] time = %d\n",__FUNCTION__,__LINE__,MsOS_GetSystemTime());

    if(_gItem[eMediaType] == NULL)
    {
        mTBOX_TRACE((kTBOX_NIV_WARNING, "Create Meida item fail. u32Item is NULL in playfile\n"));
        goto FUNC_END;
    }


    if( _geSeamlessMode == MPLAYER_SEAMLESS_SMOTH )
    {
        if(eMediaType == E_MMSDK_MEDIA_TYPE_MOVIE)
        {
            MMSDK_SetSeamlessMode(E_MMSDK_SEAMLESS_SMOTH);
        }
    }
    else
    {
        MMSDK_SetSeamlessMode(E_MMSDK_SEAMLESS_NONE);
    }

    MMSDK_SetLoop(_gItem[eMediaType], _gu8IsRepeat);


    if(_gThreadEvtProcess[eMediaType].iId == -1)
    {
        if(TH_EVT_PROC_STACK_SIZE)
        {
            _gThreadEvtProcess[eMediaType].pStack = (void *)AllocNoncachedMem(_gThreadEvtProcess[eMediaType].u32StackSize);
            if(!_gThreadEvtProcess[eMediaType].pStack)
            {
                mTBOX_TRACE((kTBOX_NIV_WARNING, "MApi_MPlayer_PlayFile(): _gThreadEvtProcess[%d] stack malloc failed......\n",eMediaType));
                goto FUNC_END;
            }
        }
        

        _gThreadEvtProcess[eMediaType].iId = MsOS_CreateTask( _gpEvtProcess[eMediaType],
                                    (MS_U32)NULL,
                                    _gThreadEvtProcess[eMediaType].ePriority,
                                    TRUE,
                                    _gThreadEvtProcess[eMediaType].pStack,
                                    _gThreadEvtProcess[eMediaType].u32StackSize,
                                    _gThreadEvtProcess[eMediaType].szName);

        if(_gThreadEvtProcess[eMediaType].iId == -1)
        {
            mTBOX_TRACE((kTBOX_NIV_WARNING, "MApi_MPlayer_PlayFile(): create Thread _gpEvtProcess failed......\n"));
            _gstMedia.pCallback = NULL;
            goto FUNC_END;
        }
    }

    bRet = TRUE;

FUNC_END:
    if(bRet == FALSE && _gpStream[eMediaType])
    {
#if DEMO_FILE
        _MM_FileClose(_gpStream[eMediaType]);
        _MM_FileClose(_gpStream[E_MPLAYER_MEDIA_STREAM_SUBTITLE]);
        _MM_FileClose(_gpStream[E_MPLAYER_MEDIA_STREAM_AUDIO]);
        _MM_FileClose(_gpStream[E_MPLAYER_MEDIA_STREAM_AUDIO2]);
#else
#endif
        _gpStream[eMediaType] = NULL;
        _gpStream[E_MPLAYER_MEDIA_STREAM_SUBTITLE] = NULL;
        _gpStream[E_MPLAYER_MEDIA_STREAM_AUDIO] = NULL;
        _gpStream[E_MPLAYER_MEDIA_STREAM_AUDIO2] = NULL;
    }

    u32wait_init_ok_timer = MsOS_GetSystemTime();
    
    printf("=Test [%s][%d] time = %d\n",__FUNCTION__,__LINE__,MsOS_GetSystemTime());
    while (!_bIsReadyToPlay)
    {
        if (MsOS_GetSystemTime() - u32wait_init_ok_timer > WAIT_INIT_OK_TIMEOUT)
        {
            bRet = FALSE;
            mTBOX_TRACE((kTBOX_NIV_WARNING, "Wait MM INIT OK Timtout\n"));
            break;
        }
        MsOS_DelayTask(100);
    }
    printf("=Test [%s][%d] time = %d\n",__FUNCTION__,__LINE__,MsOS_GetSystemTime());
    mTBOX_TRACE((kTBOX_NIV_1, "------> Leave func MApi_MPlayer_PlayFile \n"));
    TDAL_UnlockMutex(_gs32MutexOpenStop);
    return bRet;
}
MS_BOOL TDAL_MPm_Close(void)
{
    MS_BOOL bRet = TRUE;
    int i;
    MEDIAITEM u32ItemTmp = NULL;
    
    mTBOX_FCT_ENTER("TDAL_MPm_Close");
    TDAL_LockMutex(_gs32MutexOpenStop);
    if(_gItem[_gstMedia.enMediaType] != NULL)
    {
        u32ItemTmp = _gItem[_gstMedia.enMediaType];
        _gItem[_gstMedia.enMediaType] = NULL;
        if (MMSDK_DeleteMediaItem(u32ItemTmp) != TRUE)
        {
            mTBOX_TRACE((kTBOX_NIV_WARNING, "-----> MMSDK_DeleteMediaItem fail \n"));
        }
    }

    if(_gstMedia.enMediaType == E_MMSDK_MEDIA_TYPE_MUSIC)
    {
        _geAudioType = MPLAYER_SUBTYPE_UNKNOWN;
        _gu32SaveFilePos = 0;

#if(DEMO_AUDIO_MULTI_TEST == 1)
        Demo_MM_FreeAudDec();
#endif
    }
    else if(_gstMedia.enMediaType == E_MMSDK_MEDIA_TYPE_PHOTO)
    {
        _geRotaAng = E_MMSDK_ROTATE_0;
        _geZoomFctor = E_MMSDK_VIEW_ZOOM_1X;
    }
    else
    {
        _gu32TotalTime = 0;
        _gu32StartTime = 0;

#if(DEMO_AUDIO_MULTI_TEST == 1)
        Demo_MM_FreeAudDec();
#endif
    }
    _gstMedia.pCallback = NULL;

    if(_gpStream[_gstMedia.enMediaType] != NULL)
    {
        _MM_FileClose(_gpStream[_gstMedia.enMediaType]);
        _gpStream[_gstMedia.enMediaType] = NULL;
        if(_gstMedia.enMediaType == E_MMSDK_MEDIA_TYPE_MOVIE && _gpStream[E_MPLAYER_MEDIA_STREAM_SUBTITLE])
        {
            _MM_FileClose(_gpStream[E_MPLAYER_MEDIA_STREAM_SUBTITLE]);
            _gpStream[E_MPLAYER_MEDIA_STREAM_SUBTITLE] = NULL;
        }
        if(_gstMedia.enMediaType == E_MMSDK_MEDIA_TYPE_MOVIE && _gpStream[E_MPLAYER_MEDIA_STREAM_AUDIO])
        {
            _MM_FileClose(_gpStream[E_MPLAYER_MEDIA_STREAM_AUDIO]);
            _gpStream[E_MPLAYER_MEDIA_STREAM_AUDIO] = NULL;
        }
        if(_gstMedia.enMediaType == E_MMSDK_MEDIA_TYPE_MOVIE && _gpStream[E_MPLAYER_MEDIA_STREAM_AUDIO2])
        {
            _MM_FileClose(_gpStream[E_MPLAYER_MEDIA_STREAM_AUDIO2]);
            _gpStream[E_MPLAYER_MEDIA_STREAM_AUDIO2] = NULL;
        }
    }

    MMSDK_SetBackwardRePlayMode(_gbBackwardRePlayMode);
    //_gbSeekRightNow[eMediaType] = FALSE;
    _geStrmMode[_gstMedia.enMediaType] = MPLAYER_FILE_MODE;

    i = TDAL_MPi_GetRunningMediaDescriptor();

    if (i >= 0)
    {
        mTBOX_TRACE((kTBOX_NIV_1, "Index %d Stopped\n",i));
        TDAL_MPi_DescList[i]->bIsPlaying = FALSE;
    }
    _bIsFileSupport = TRUE;
    _bIsAudioSupport = TRUE;    
    _bIsReadyToPlay = FALSE;
    _pInputDataRead = NULL;
    _pInputDataSeek = NULL;
    _pInputDataLength = NULL;
    TDAL_UnlockMutex(_gs32MutexOpenStop);
    mTBOX_RETURN(bRet);
}
MS_BOOL TDAL_MPm_Stop(void)
{
    bool returnValue = TRUE;
    
    mTBOX_FCT_ENTER("TDAL_MPm_Stop");
    
    TDAL_LockMutex(_gs32MutexOpenStop);

    if (_gbPlay[_gstMedia.enMediaType] == FALSE)
    {
        mTBOX_TRACE((kTBOX_NIV_1, "-----> Already Stop!\n"));
        TDAL_UnlockMutex(_gs32MutexOpenStop);
        mTBOX_RETURN(returnValue);
    }
 /*   if (!TDAL_MPi_IsMediaRunning())
    {
        mTBOX_TRACE((kTBOX_NIV_WARNING, "No media running, skipping stop\n"));
        mTBOX_RETURN(false);
    }*/
    
    _geTrickMod[_gstMedia.enMediaType] = E_MMSDK_TRICK_NORMAL;
    _gu64CurPostn[_gstMedia.enMediaType] = 0;
    _gu64SubCurPostn = 0;

    _gbPlay[_gstMedia.enMediaType] = FALSE;
    
    if(MMSDK_Stop(_gItem[_gstMedia.enMediaType]) != TRUE)
    {
        mTBOX_TRACE((kTBOX_NIV_WARNING, "-----> MMSDK_Stop fail \n"));
        returnValue = FALSE;
    }
    
    mTBOX_TRACE((kTBOX_NIV_1, "TDAL_MPm_Stop: Stream stopped\n"));

    TDAL_UnlockMutex(_gs32MutexOpenStop);
    mTBOX_RETURN(returnValue);
}

MS_BOOL TDAL_MPm_Player_Finalize(void)
{
    MS_BOOL error = TRUE;    
    MS_U32  pu32RetrievedEventFlag;    
    MS_S32 s32MstarNonCachedPoolID = INVALID_POOL_ID;


    _gbExit[E_MMSDK_MEDIA_TYPE_PHOTO] = TRUE;    
    _gbExit[E_MMSDK_MEDIA_TYPE_MUSIC] = TRUE;    
    _gbExit[E_MMSDK_MEDIA_TYPE_MOVIE] = TRUE;

    Get_NonCachePoolID(&s32MstarNonCachedPoolID);
        
    //Photo    
    if (_gThreadEvtProcess[E_MMSDK_MEDIA_TYPE_PHOTO].iId !=-1)  
    {        
        MM_SET_PHOTO_NOTIFY_TYPE(MPLAYER_NOTIFY_NULL);        
        MsOS_WaitEvent(_gs32MediaEvtGrp, MM_EVENT_PHOTO_PROC_EXIT, &pu32RetrievedEventFlag, E_OR_CLEAR, MSOS_WAIT_FOREVER);
#if defined(MSOS_TYPE_ECOS)        
        _MM_PollingThreadStatus(_gThreadEvtProcess[E_MMSDK_MEDIA_TYPE_PHOTO]);
#endif
    
        _gThreadEvtProcess[E_MMSDK_MEDIA_TYPE_PHOTO].iId = -1;        
        if (_gThreadEvtProcess[E_MMSDK_MEDIA_TYPE_PHOTO].pStack)
        {            
                MsOS_FreeMemory(_gThreadEvtProcess[E_MMSDK_MEDIA_TYPE_PHOTO].pStack, s32MstarNonCachedPoolID);
                 _gThreadEvtProcess[E_MMSDK_MEDIA_TYPE_PHOTO].pStack = NULL;      
        } 
    }        


    //Movie    
    if (_gThreadEvtProcess[E_MMSDK_MEDIA_TYPE_MOVIE].iId != -1)  
    {        
        MM_SET_VIDEO_NOTIFY_TYPE(MPLAYER_NOTIFY_NULL);
        MsOS_WaitEvent(_gs32MediaEvtGrp, MM_EVENT_VIDEO_PROC_EXIT, &pu32RetrievedEventFlag, E_OR_CLEAR, MSOS_WAIT_FOREVER);
#if defined(MSOS_TYPE_ECOS)
        _MM_PollingThreadStatus(_gThreadEvtProcess[E_MMSDK_MEDIA_TYPE_MOVIE]);
#endif       
        _gThreadEvtProcess[E_MMSDK_MEDIA_TYPE_MOVIE].iId = -1;
        if (_gThreadEvtProcess[E_MMSDK_MEDIA_TYPE_MOVIE].pStack)      
        {            
            MsOS_FreeMemory(_gThreadEvtProcess[E_MMSDK_MEDIA_TYPE_MOVIE].pStack, s32MstarNonCachedPoolID);
            _gThreadEvtProcess[E_MMSDK_MEDIA_TYPE_MOVIE].pStack = NULL;      
        } 
     }

     //Music    
     if (_gThreadEvtProcess[E_MMSDK_MEDIA_TYPE_MUSIC].iId != -1)  
    {        
        MM_SET_MUSIC_NOTIFY_TYPE(MPLAYER_NOTIFY_NULL);        
        MsOS_WaitEvent(_gs32MediaEvtGrp, MM_EVENT_MUSIC_PROC_EXIT, &pu32RetrievedEventFlag, E_OR_CLEAR, MSOS_WAIT_FOREVER);
#if defined(MSOS_TYPE_ECOS)
        _MM_PollingThreadStatus(_gThreadEvtProcess[E_MMSDK_MEDIA_TYPE_MUSIC]);
#endif
        _gThreadEvtProcess[E_MMSDK_MEDIA_TYPE_MUSIC].iId = -1;
        if (_gThreadEvtProcess[E_MMSDK_MEDIA_TYPE_MUSIC].pStack)
        {  
            MsOS_FreeMemory(_gThreadEvtProcess[E_MMSDK_MEDIA_TYPE_MUSIC].pStack, s32MstarNonCachedPoolID);  
            _gThreadEvtProcess[E_MMSDK_MEDIA_TYPE_MUSIC].pStack = NULL;            
        }
    }
     
    TDAL_LockMutex(_gs32MutexOpenStop);
    
    if (_gpu8Buffer)  
    {  
        MsOS_FreeMemory(_gpu8Buffer, s32MstarNonCachedPoolID);
        _gpu8Buffer = NULL;  
    }
    
    if (_gpu16UnicdBuffer)  
    {
        MsOS_FreeMemory(_gpu16UnicdBuffer, s32MstarNonCachedPoolID);
        _gpu16UnicdBuffer = NULL;
    }

    TDAL_UnlockMutex(_gs32MutexOpenStop);

    TDAL_DeleteMutex(_gs32Mutex);
    if (_gs32MediaEvtGrp != -1)  
    {
        MsOS_DeleteEventGroup(_gs32MediaEvtGrp);
        _gs32MediaEvtGrp = -1; 
    }

    TDAL_DeleteMutex(_gs32MutexOpenStop);
    return error;
}
MS_BOOL TDAL_MPm_Player_Initialize(void)
{
    MS_BOOL bRet = FALSE;
    const MS_U8* pMutexName1 = (MS_U8*)"_gs32MutexOpenStop";
    const MS_U8* pMutexName2 = (MS_U8*)"VarMutex";
    const MS_U8* sEvtGrpName = (MS_U8*)"MMEvnGrp";
    MS_U8 i = 0;

    _gbExit[E_MMSDK_MEDIA_TYPE_PHOTO] = FALSE;
    _gbExit[E_MMSDK_MEDIA_TYPE_MUSIC] = FALSE;
    _gbExit[E_MMSDK_MEDIA_TYPE_MOVIE] = FALSE;
    _geStrmMode[E_MMSDK_MEDIA_TYPE_PHOTO] = MPLAYER_FILE_MODE;
    _geStrmMode[E_MMSDK_MEDIA_TYPE_MUSIC] = MPLAYER_FILE_MODE;
    _geStrmMode[E_MMSDK_MEDIA_TYPE_MOVIE] = MPLAYER_FILE_MODE;

    _gu32VdplayerReadTimerOut = 500;
    _gbFastInit = FALSE;
    _gu32InitDataSize = 0x20000;

    _gs32MinBufferSec = 1;
    _gs32MaxBufferSec = 5;

    for(i = 0; i < E_MPLAYER_MEDIA_STREAM_NUM; i++)
    {
        _gpStream[i] = NULL;
    }

    TDAL_CreateMutex(&_gs32MutexOpenStop);
    if(_gs32MutexOpenStop == NULL)
    {
        goto FUNC_END;
    }

    TDAL_CreateMutex(&_gs32Mutex);
    if(_gs32Mutex == NULL)
    {
        goto FUNC_END;
    }

    _gs32MediaEvtGrp = MsOS_CreateEventGroup((char*)sEvtGrpName);
    if(_gs32MediaEvtGrp == -1)
    {
        goto FUNC_END;
    }

    if (!_TDAL_MM_Initialize())
    {
        mTBOX_TRACE((kTBOX_NIV_WARNING, "-----> MM_Initialize ERROR ERROR!!!!\n"));
    }

    for (i = 0; i < MMSDK_MEDIA_TYPE_NUM; i++)
    {
        //_gu8PreviewFlag[i] = 0;
        _geTrickMod[i] = E_MMSDK_TRICK_NORMAL;
    }

    _gbKTVMode = FALSE;
    _gbBackwardRePlayMode = FALSE;

    bRet = TRUE;
FUNC_END:
    return bRet;    
}

#if 0 // useless

bool TDAL_MPi_IsPhotoContainer(tTDAL_MPm_Desc * desc)
{

    mTBOX_FCT_ENTER("TDAL_MPi_IsPhotoContainer");

#if MP_PHOTO_DEMO

#else
    char headerBytes[11];
    int bytesRead;

    _pInputDataSeek((tTDAL_MP_Handle) desc, 0, eTDAL_MP_SEEK_MODE_SET);
    bytesRead = _pInputDataRead((tTDAL_MP_Handle) desc, headerBytes, sizeof(headerBytes));

    if (bytesRead != sizeof(headerBytes))
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Could not read header bytes\n"));
        mTBOX_RETURN(false);
    }

    if (strncmp(headerBytes + 6, "JFIF", 4) != 0)
    {
        mTBOX_TRACE((kTBOX_NIV_WARNING, "Could not find JFIF in JPG header\n"));
        mTBOX_RETURN(false);
    }

    _pInputDataSeek((tTDAL_MP_Handle) desc, 0, eTDAL_MP_SEEK_MODE_SET);
#endif

    mTBOX_RETURN(true);

}

bool TDAL_MPi_IsAviContainer(tTDAL_MPm_Desc * desc)
{
    char headerBytes[11];
    int bytesRead;

    mTBOX_FCT_ENTER("TDAL_MPi_IsAviContainer");

    _pInputDataSeek((tTDAL_MP_Handle) desc, 0, eTDAL_MP_SEEK_MODE_SET);
    bytesRead = _pInputDataRead((tTDAL_MP_Handle) desc, headerBytes, sizeof(headerBytes));

    if (bytesRead != sizeof(headerBytes))
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Could not read header bytes\n"));
        mTBOX_RETURN(false);
    }

    if (strncmp(headerBytes, "RIFF", 4) != 0)
    {
        mTBOX_TRACE((kTBOX_NIV_WARNING, "Could not find RIFF in AVI header\n"));
        mTBOX_RETURN(false);
    }

    if (strncmp(headerBytes + 8, "AVI", 3) != 0)
    {
        mTBOX_TRACE((kTBOX_NIV_WARNING, "Could not find AVI in AVI header\n"));
        mTBOX_RETURN(false);
    }

    _pInputDataSeek((tTDAL_MP_Handle) desc, 0, eTDAL_MP_SEEK_MODE_SET);

    mTBOX_RETURN(true);

}

static void * MM_Simulate_Stream_Open_np(const char * pathname, const char * mode)
{
//
//    FILE* fp = fopen("/mount1/poison.mp3", "r");
//    if (NULL==fp)
//    {
//        printf("fopen fail\n");
//    }
//    printf("FILE* fp %d, path = %s\n", (int)fp, pathname == NULL ? "NULL": pathname);
//    return (void *)fp;
    int i;
    tTDAL_MP_Handle handle;
    bool locked;

    mTBOX_FCT_ENTER("MM_Simulate_Stream_Open_np");

    locked = TDAL_MPi_LockMutexIfDifferentThread();

    i = TDAL_MPi_GetRunningMediaDescriptor();

    if (i < 0)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "[MM_Simulate_Stream_Open_np] No running stream\n"));
        handle = (tTDAL_MP_Handle) NULL;
    }
    else
    {
        handle = (tTDAL_MP_Handle) TDAL_MPi_DescList[i];
    }

    if (locked) { TDAL_MPi_UnlockMutex(); }

    if (handle != (tTDAL_MP_Handle) NULL)
    {
        TDAL_MPi_DescList[i]->pInputDataSeek(handle, 0, eTDAL_MP_SEEK_MODE_SET);
        mTBOX_RETURN(TDAL_MPi_DescList[i]);
    }

    mTBOX_RETURN((void *) handle);
}

static int MM_Simulate_Stream_Close_np(void * stream)
{
//    return fclose(stream);
    mTBOX_FCT_ENTER("MM_Simulate_Stream_Close_np");
    /* Do nothing */
    mTBOX_RETURN(0);
}

static unsigned long long MM_Simulate_Stream_Length_np(void * stream)
{
	int i;
	tTDAL_MP_Handle handle;
	unsigned long long returnValue = 0;
	bool locked;

    mTBOX_FCT_ENTER("MM_Simulate_Stream_Length_np");

    locked = TDAL_MPi_LockMutexIfDifferentThread();

    i = TDAL_MPi_GetRunningMediaDescriptor();
    if (i < 0)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "[MM_Simulate_Stream_Length_np] No running stream\n"));
        handle = (tTDAL_MP_Handle)  NULL;
    }
    else
    {
        handle = (tTDAL_MP_Handle) TDAL_MPi_DescList[i];
    }

    if (stream != (void *) handle)
    {
        handle = (tTDAL_MP_Handle) NULL;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "[MM_Simulate_Stream_Length_np] Started stream != running stream"));
        if (locked) { TDAL_MPi_UnlockMutex(); }
        mTBOX_RETURN(-1);
    }

    if (locked) { TDAL_MPi_UnlockMutex(); }

    if (TDAL_MPi_DescList[i]->pInputDataLength != NULL && handle != (tTDAL_MP_Handle)  NULL)
    {
    	returnValue = TDAL_MPi_DescList[i]->pInputDataLength(handle);
    }
    else
    {
    	mTBOX_TRACE((kTBOX_NIV_WARNING, "[MM_Simulate_Stream_Length_np] No length callback present, returning default length"));
    	returnValue = 0xFFFFFFFF;
    }

    mTBOX_RETURN(returnValue);
}

static int MM_Simulate_Stream_Seek_np(void * stream, MS_U64 offset, int fromwhere)
{
//    printf("MM_Simulate_Stream_Seek\n");
//    int ret;
//    ret = fseek((FILE*)stream, offset, fromwhere);
//    return ret;
    int i;
    int returnValue;
    tTDAL_MP_Handle handle;
    bool locked;

    mTBOX_FCT_ENTER("MM_Simulate_Stream_Seek_np");

    locked = TDAL_MPi_LockMutexIfDifferentThread();

    i = TDAL_MPi_GetRunningMediaDescriptor();

    if (i < 0)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "No running stream\n"));
        if (locked)  { TDAL_MPi_UnlockMutex(); }
        mTBOX_RETURN(-1);
    }

    if (stream != TDAL_MPi_DescList[i])
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Started stream != running stream"));
        if (locked) { TDAL_MPi_UnlockMutex(); }
        mTBOX_RETURN(-1);
    }

    handle = (tTDAL_MP_Handle) TDAL_MPi_DescList[i];

    if (locked)  { TDAL_MPi_UnlockMutex(); }

    if (TDAL_MPi_DescList[i]->pInputDataSeek != NULL)
    {
        returnValue = (TDAL_MPi_DescList[i]->pInputDataSeek(handle, offset, fromwhere));
    }
    else
    {
        returnValue = -1;
    }

    mTBOX_RETURN(returnValue);
}

#define MAX_READ_SIZE (20*1024)

static int MM_Simulate_Stream_Read_np(void *ptr, int size, int nitems, void *stream)
{

//   MS_S32 retSize = 0;
//
//   printf("Read file, bytes = %d, \n");
//
//   retSize = fread(ptr, size, nitems, (FILE *)stream);
//   return retSize;

    int i;
    int returnValue;
    uint32_t bytesRead;
    uint32_t bytesToRead = size * nitems;
    tTDAL_MP_Handle handle;
    bool locked;

    mTBOX_FCT_ENTER("MM_Simulate_Stream_Read_np");

    locked = TDAL_MPi_LockMutexIfDifferentThread();

    i = TDAL_MPi_GetRunningMediaDescriptor();

    if (i < 0)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "[MM_Simulate_Stream_Read_np] No running stream\n"));
        if (locked)  { TDAL_MPi_UnlockMutex(); }
        return -1;
    }

    if (stream != TDAL_MPi_DescList[i])
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "[MM_Simulate_Stream_Read_np] Started stream != running stream"));
        if (locked)  { TDAL_MPi_UnlockMutex(); }
        return -1;
    }

    handle = (tTDAL_MP_Handle)  TDAL_MPi_DescList[i];

    if (locked) { TDAL_MPi_UnlockMutex(); }

    if (TDAL_MPi_DescList[i]->pInputDataRead != NULL)
    {
        if (bytesToRead > MAX_READ_SIZE) { bytesToRead = MAX_READ_SIZE; }
        bytesRead = TDAL_MPi_DescList[i]->pInputDataRead(handle, ptr, bytesToRead);

        if (bytesRead > bytesToRead)
        {
            returnValue = -1;
        }
        else
        {
            mTBOX_TRACE((kTBOX_NIV_1, "[MM_Simulate_Stream_Read_np] Read data Ok, bytes read = %d\n", bytesRead));
            returnValue = (int) bytesRead;
        }
    }
    else
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Input data read function is NULL\n"));
        returnValue = -1;
    }

    mTBOX_RETURN(returnValue);
}

LOCAL bool TDAL_MPi_GetMediaType(tTDAL_MP_Handle Handle, EN_MMSDK_MEDIA_TYPE * mediaType)
{
    tTDAL_MPm_Desc *pstDesc;

    mTBOX_FCT_ENTER("TDAL_MPi_GetMediaType");

    mTBOX_ASSERT(Handle != ((tTDAL_MP_Handle) NULL));
    mTBOX_ASSERT(mediaType != NULL);

    pstDesc = (tTDAL_MPm_Desc *)Handle;
    mTBOX_TRACE((kTBOX_NIV_1, "TDAL_MPi_GetMediaType: Audio type = %d\n, Video type = %d\n", pstDesc->eStreamAudCodecType, pstDesc->eStreamVidCodecType));

    if (pstDesc->eStreamAudCodecType == eTDAL_MP_STREAM_CONTENT_INVALID && pstDesc->eStreamVidCodecType == eTDAL_MP_STREAM_CONTENT_INVALID)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_MPi_GetMediaType: invalid media type\n"));
        mTBOX_RETURN(false);
    }

    if (pstDesc->eStreamVidCodecType == eTDAL_MP_STREAM_CONTENT_INVALID)
    {
        mTBOX_TRACE((kTBOX_NIV_1, "[TDAL_MPi_GetMediaType] Media type is music\n"));
        *mediaType = E_MMSDK_MEDIA_TYPE_MUSIC;
        mTBOX_RETURN(true);
    }

    if (pstDesc->eStreamVidCodecType == eTDAL_MP_STREAM_CONTENT_JPG)
    {
        mTBOX_TRACE((kTBOX_NIV_1, "[TDAL_MPi_GetMediaType] Media type is photo\n"));
        *mediaType = E_MMSDK_MEDIA_TYPE_PHOTO;
        mTBOX_RETURN(true);
    }

    mTBOX_TRACE((kTBOX_NIV_1, "[TDAL_MPi_GetMediaType] Media type is movie\n"));
    *mediaType = E_MMSDK_MEDIA_TYPE_MOVIE;
    mTBOX_RETURN(true);
}

const LOCAL EN_MPLAYER_AUDIO_CODEC_TYPE TDAL_MPi_SupportedAudioCodecs[] =
{
    MPLAYER_AUDIO_CODEC_MP2,
    MPLAYER_AUDIO_CODEC_MP3
};

const LOCAL EN_MPLAYER_VIDEO_CODEC_TYPE TDAL_MPi_SupportedVideoCodecs[] =
{
    MPLAYER_VIDEO_CODEC_MPEG2,
    MPLAYER_VIDEO_CODEC_MPEG1
};

LOCAL bool TDAL_MPi_IsFileSupported(void)
{
    int i, j;
    unsigned int audioCodec, videoCodec;
    unsigned int hSize, vSize;
    bool videoCodecSupported = false;
    bool audioCodecSupported = false;

    mTBOX_FCT_ENTER("TDAL_MPi_IsFileSupported");

    if (TDAL_MPm_eMediaType != E_MPLAYER_MEDIA_MOVIE)
    {
        mTBOX_RETURN(true);
    }

    for (i = 0; i < 10; i++)
    {
        videoCodec = MApi_MPlayer_GetOption(TDAL_MPm_eMediaType, MPLAYER_OPTION_MOVIE_INFO, MPLAYER_INFO_VIDEO_CODEC);
        audioCodec = MApi_MPlayer_GetOption(TDAL_MPm_eMediaType, MPLAYER_OPTION_MOVIE_INFO, MPLAYER_INFO_AUDIO_CODEC);
        hSize = MApi_MPlayer_GetOption(TDAL_MPm_eMediaType, MPLAYER_OPTION_MOVIE_INFO, MPLAYER_INFO_H_SIZE);
        vSize = MApi_MPlayer_GetOption(TDAL_MPm_eMediaType, MPLAYER_OPTION_MOVIE_INFO, MPLAYER_INFO_V_SIZE);

        printf("Video codec = %d, audio codec = %d, hSize = %d, vSize = %d\n", videoCodec, audioCodec, hSize, vSize);

        if (hSize != 0 && vSize != 0)
        {
            for (j = 0; j < sizeof(TDAL_MPi_SupportedVideoCodecs); j++)
            {
                if (TDAL_MPi_SupportedVideoCodecs[j] == videoCodec)
                {
                    videoCodecSupported = true;
                }
            }

            if (!videoCodecSupported)
            {
                mTBOX_TRACE((kTBOX_NIV_1, "Unsupported video codec %d\n", videoCodec));
                mTBOX_RETURN(false);
            }

            for (j = 0; j < sizeof(TDAL_MPi_SupportedAudioCodecs); j++)
            {
                if (TDAL_MPi_SupportedAudioCodecs[j] == audioCodec)
                {
                    audioCodecSupported = true;
                }
            }

            if (!audioCodecSupported)
            {
                mTBOX_TRACE((kTBOX_NIV_1, "Unsupported audio codec %d\n", audioCodec));
                mTBOX_RETURN(false);
            }

            if (hSize > MAX_SUPPORTED_RES_H || vSize > MAX_SUPPORTED_RES_V)
            {
                mTBOX_TRACE((kTBOX_NIV_1, "Unsupported resolution %dx%d\n", hSize, vSize));
                mTBOX_RETURN(false);
            }

            mTBOX_RETURN(true);
        }

        MsOS_DelayTask(100);
    }

    mTBOX_TRACE((kTBOX_NIV_WARNING, "Could not obtain information about the running file\n"));
    mTBOX_RETURN(false);
}

/* fix from MSTAR for USB thread starvation in playback,
 * this fix is used to adjust miu group 1's member service number,
 * before and after playback */
void SetMIU_G1MaxServiceNum(MS_U16 value)
{
    *(volatile MS_U16*)(0xBF202400+(0x31<<2)) = value; // Set miu group 1 max member service number, default is 0x5
    printf("SetMIU_G1MaxServiceNum:%d\n", value);
}

LOCAL void _SubQueuePushHandler(unsigned int  u32Info)
{
#if 0 //wait porting
    ST_CPCODEC_MSG_SUBQUEUE_PUSH_INFO *stQueueInfo = (ST_CPCODEC_MSG_SUBQUEUE_PUSH_INFO *)u32Info;
    switch(stQueueInfo->eType)
    {
        case E_CPCODEC_INTERNAL_SUBTITLE_TYPE_TS_PGS:
            MApi_MPlayer_Push_SpuQueue((U8 *)stQueueInfo->u32QueueStar, (U16)stQueueInfo->u32QueueSize, (stQueueInfo->u32PTSStart), E_SUBTITLE_TYPE_TS, 0);
            break;
        case E_CPCODEC_INTERNAL_SUBTITLE_TYPE_TS_BMP:
            MApi_MPlayer_Push_SpuQueue((U8 *)stQueueInfo->u32QueueStar, (U16)stQueueInfo->u32QueueSize, (stQueueInfo->u32PTSStart), E_SUBTITLE_TYPE_TS, 0);
            break;
        case E_CPCODEC_INTERNAL_SUBTITLE_TYPE_TEXT:
            MApi_MPlayer_Push_SpuQueue((U8 *)stQueueInfo->u32QueueStar, (U16)stQueueInfo->u32QueueSize, ( stQueueInfo->u32PTSStart), E_SUBTITLE_TYPE_TTF, (stQueueInfo->u32PTSEnd - stQueueInfo->u32PTSStart));
            break;
        case E_CPCODEC_INTERNAL_SUBTITLE_TYPE_IMAGE_BMP :
            MApi_MPlayer_Push_SpuQueue((U8 *)stQueueInfo->u32QueueStar, (U16)stQueueInfo->u32QueueSize, (stQueueInfo->u32PTSStart * 90), E_SUBTITLE_TYPE_BMP, 0);
            break;
        case E_CPCODEC_INTERNAL_SUBTITLE_TYPE_IMAGE_VOBSUB:
            MApi_MPlayer_Push_SpuQueue((U8 *)stQueueInfo->u32QueueStar, (U16)stQueueInfo->u32QueueSize, (stQueueInfo->u32PTSStart * 90), E_SUBTITLE_TYPE_VOBSUB, 0);
            break;
#if MM_REMOVE_TXT_SUBTITLE == 0
        case E_CPCODEC_INTERNAL_SUBTITLE_TYPE_TEXT:
            if(_bExitSubTask)
                break;

            MsOS_ObtainMutex(_s32QMutex, MSOS_WAIT_FOREVER);
            if((s32QBack + 1) % Q_SIZE == s32QFront)
            {
                printf("@@@Queue is full!!!@@@\n");
                break;//drop data
            }
            m_QElement[s32QBack].pu8Buf = (MS_U8*)MsOS_AllocateMemory(stQueueInfo->u32QueueSize, gs32NonCachedPoolID);
            memcpy(m_QElement[s32QBack].pu8Buf, (void*)stQueueInfo->u32QueueStar, stQueueInfo->u32QueueSize * sizeof(MS_U8));
            m_QElement[s32QBack].u16Len = stQueueInfo->u32QueueSize;
            m_QElement[s32QBack].u32PTS = stQueueInfo->u32PTSStart;
            m_QElement[s32QBack].u32Duration = stQueueInfo->u32PTSStart - stQueueInfo->u32PTSEnd;

            s32QBack++;
            if(s32QBack >= Q_SIZE)
                s32QBack = 0;
            MsOS_ReleaseMutex(_s32QMutex);
            break;
#endif
        default:
            break;
    }
#endif    
}

LOCAL void _SubtitleDisplayHandler(unsigned int  u32Info)
{
#if 0 //wait porting
    ST_CPCODEC_MSG_SUBTITLE_INIT_INFO *stSubtitle = (ST_CPCODEC_MSG_SUBTITLE_INIT_INFO *)u32Info;
    switch(stSubtitle->eType)
    {
        case E_CPCODEC_INTERNAL_SUBTITLE_TYPE_TS_BMP:
        case E_CPCODEC_INTERNAL_SUBTITLE_TYPE_IMAGE_VOBSUB:
            if(stSubtitle->bShow)
            {
                MApi_MPlayer_Open();
            }
            else
            {
                MApi_MPlayer_Close();
            }
         break;
    default:
        break;
    }

#endif    
}

LOCAL bool TDAL_MPi_VideoSetOutput(VDEC_DispInfo *pstVidStatus)
{
#if 0 //wait porting
    MVOP_VidStat stMvopVidSt;
    MVOP_InputSel tMvopInputSel;
    video_disp_info MM_DispInfo;
    MS_U32 u32CodecType;
    VDEC_DispInfo stVidStatus;

    //MS504x set MVOP as MIU1 Group1
    MDrv_MVOP_MiuSwitch(1);

    MDrv_MVOP_Init();
    MDrv_MVOP_Enable(FALSE); //wait setting done
    u32CodecType = MApi_MPlayer_GetOption(E_MPLAYER_MEDIA_MOVIE, MPLAYER_OPTION_MOVIE_INFO, (unsigned int)MPLAYER_INFO_VIDEO_CODEC);
    //u32CodecType = msAPI_MM_GetOption((int)MM_OPTION_MOVIE_INFO, (int)MM_INFO_VIDEO_CODEC);
    if((u32CodecType == MPLAYER_VIDEO_CODEC_MPEG4) ||
        (u32CodecType == MPLAYER_VIDEO_CODEC_MPEG2) ||
        (u32CodecType == MPLAYER_VIDEO_CODEC_VC1) ||
        (u32CodecType == MPLAYER_VIDEO_CODEC_FLV))
    {
        tMvopInputSel = MVOP_INPUT_MVD;
    }
    else if((u32CodecType == MPLAYER_VIDEO_CODEC_H264) ||
        (u32CodecType == MPLAYER_VIDEO_CODEC_RM))
    {
        tMvopInputSel = MVOP_INPUT_H264;
    }
    else if(u32CodecType == MPLAYER_VIDEO_CODEC_MJPEG)
    {
        tMvopInputSel = MVOP_INPUT_JPD;
    }
    else
    {
        printf("Unsupported coded type\n");
    }
    MDrv_MVOP_SetInputCfg(tMvopInputSel, NULL);

    //msAPI_MM_GetDispInfo(&MM_DispInfo);
    MApi_MPlayer_Get_DispInfo(E_MPLAYER_MEDIA_MOVIE, (video_disp_info*)&MM_DispInfo);

    memset(&stVidStatus, 0, sizeof(VDEC_DispInfo));

    //Fill Video information for msAPI_XC lib
    stVidStatus.u16CropLeft = MM_DispInfo.dCropLeft;
    stVidStatus.u16CropRight = MM_DispInfo.dCropRight;
    stVidStatus.u16CropTop = MM_DispInfo.dCropTop;
    stVidStatus.u16CropBottom = MM_DispInfo.dCropBottom;
    stVidStatus.u16SarWidth   = 0;
    stVidStatus.u16SarHeight = 0;
    stVidStatus.u16HorSize    = MM_DispInfo.dWidth;
    stVidStatus.u16VerSize    = MM_DispInfo.dHeight;
    stVidStatus.u32FrameRate  = MM_DispInfo.dFrameRate;
    stVidStatus.u8Interlace   = MM_DispInfo.dInterlace;

    msAPI_XC_SetVDECInfo(stVidStatus);

    mTBOX_TRACE((kTBOX_NIV_1, "MM_DispInfo dWidth %d, dHeight %d, dFrameRate %d dInterlace %d\n",
         MM_DispInfo.dWidth, MM_DispInfo.dHeight, MM_DispInfo.dFrameRate, MM_DispInfo.dInterlace));
    mTBOX_TRACE((kTBOX_NIV_1, "MM_DispInfo dCropLeft %d, dCropRight %d, dCropTop %d, dCropBottom %d\n",
         MM_DispInfo.dCropLeft, MM_DispInfo.dCropRight, MM_DispInfo.dCropTop, MM_DispInfo.dCropBottom));
    memset(&stMvopVidSt, 0, sizeof(MVOP_VidStat));
    stMvopVidSt.u16HorSize = (MS_U16)MM_DispInfo.dWidth;
    stMvopVidSt.u16VerSize = MM_DispInfo.dHeight;
    stMvopVidSt.u16FrameRate = MM_DispInfo.dFrameRate;
    stMvopVidSt.u8AspectRate = 2;//pstVidStatus->u8AspectRate;
    stMvopVidSt.u8Interlace = MM_DispInfo.dInterlace;//0;//pstVidStatus->u8Interlace;

    MDrv_MVOP_SetOutputCfg(&stMvopVidSt, FALSE);
    MDrv_MVOP_SetStartPos(0,0);

    MDrv_MVOP_Enable(TRUE); //ENABLE MVOP, Video pass to Scaler

    mTBOX_TRACE((kTBOX_NIV_1, "->u32FrameRate = %u\n", stMvopVidSt.u16FrameRate));
    mTBOX_TRACE((kTBOX_NIV_1, "->u8Interlace = %x\n", stMvopVidSt.u8Interlace));
    mTBOX_TRACE((kTBOX_NIV_1, "->u16HorSize = %u\n", stMvopVidSt.u16HorSize));
    mTBOX_TRACE((kTBOX_NIV_1, "->u16VerSize = %u\n", stMvopVidSt.u16VerSize));
    //db_print("->u16Crop R/L = %u, %u\n", stMvopVidSt.u16CropRight, stMvopVidSt.u16CropLeft);
    //db_print("->u16Crop B/T = %u, %u\n", stMvopVidSt.u16CropBottom, stMvopVidSt.u16CropTop);
    mTBOX_TRACE((kTBOX_NIV_1, "->u8AspectRate = %u\n", stMvopVidSt.u8AspectRate));
    //db_print("->u16SarWidth = %u\n", stMvopVidSt.u16SarWidth);
    //db_print("->u16SarHeight = %u\n", stMvopVidSt.u16SarHeight);
#endif
    return TRUE;
}

bool TDAL_MPi_GetDecoderStatus(void)
{
    VDEC_DispInfo stVidStatus;
    bool locked;

    mTBOX_FCT_ENTER("TDAL_MPi_GetDecoderStatus");

    locked = TDAL_MPi_LockMutexIfDifferentThread();

#if 0 //george 110822
    MS_U32 u32Loop = VIDEO_FRAMECOUNT_LOOP;

    while(msAPI_MM_GetOption((int)MM_OPTION_MOVIE_INFO, (int)MM_INFO_INDEX_FIRST_FRAME) != TRUE)
    {
        u32Loop--;
        if(u32Loop==0)
        {
            mTBOX_TRACE((kTBOX_NIV_WARNING, " Video DispInfo is Not Ready!! \n"));
            if (locked) { TDAL_MPi_UnlockMutex(); }
            return FALSE;
        }
        MsOS_DelayTask(10);
    }
#endif

    if(TRUE != TDAL_MPi_VideoSetOutput(&stVidStatus))
    {
        mTBOX_TRACE((kTBOX_NIV_WARNING, " Video Set Output Config Failed!! \n"));
        if (locked) { TDAL_MPi_UnlockMutex(); }
        mTBOX_RETURN(FALSE);
    }

    if (locked) { TDAL_MPi_UnlockMutex(); }

    mTBOX_RETURN(TRUE);
}

LOCAL void TDAL_MPi_CallbackTask(void* arg)
{
    mTBOX_FCT_ENTER("TDAL_MPi_CallbackTask");
#if 0 //wait porting
    tTDAL_MPi_CallbackMsg msg;
    MS_U32 actualSize;
    MS_BOOL bRet;
    tTDAL_MP_EvtNotify notifyFct = NULL;
    tTDAL_MP_Event event;
    int index;


    while(1)
    {
#if MSTAR_QUEUE
        bRet = MsOS_RecvFromQueue(TDAL_MPi_CallbackQueue, (MS_U8 *) &msg, sizeof(msg), &actualSize, MSOS_WAIT_FOREVER);
#else
        bRet = TDAL_Dequeue(TDAL_MPi_CallbackQueue, (MS_U8 *) &msg);
#endif
        if (bRet == TRUE && (actualSize == sizeof(msg)))
        {
            if (msg.shouldExit)
            {
                mTBOX_TRACE((kTBOX_NIV_1, "Stopping TDAL_MP task\n"));
                break;
            }

            TDAL_MPi_LockMutex();

            index = TDAL_MPi_GetRunningMediaDescriptor();

            if (index < 0)
            {
                mTBOX_TRACE((kTBOX_NIV_WARNING, "No running media, skipping event, eventType = %d, cmd = %d, param = %d\n", msg.callbackType, msg.eCmd, msg.u32Param));
                continue;
            }

            notifyFct = TDAL_MPi_DescList[index]->pEvtNotify;

            switch(msg.callbackType)
            {
            case eTDAL_MP_CallbackMovie:
                switch (msg.eCmd)
                    {
                        case MPLAYER_COMMAND_NOTIFY:
                            switch(msg.u32Param)
                            {
                                case MPLAYER_FRAME_READY:
                                    //_SubInitHandler((MS_U32)&_prestSubtitleInit);
                                    //appDemo_XC_PlayVideo_UsingDefaultSetting();
                                    break;
                                case MPLAYER_EXIT_OK:
                #if MM_REMOVE_TXT_SUBTITLE == 0
                                    _MM_TxtSubtitleFinalize();
                #endif
                                    TDAL_MPm_Stop();
                                    if (notifyFct != NULL)
                                    {
                                        notifyFct((tTDAL_MP_Handle) TDAL_MPi_DescList[index], eTDAL_MP_EVENT_FINISHED, TDAL_MPi_DescList[index]->pCtx);
                                    }
                                    //if(u8IsRepeat)
                                        // appDemo_MM_Play(&_u32FileOption, _pu8FileName);
                                    break;
                                case MPLAYER_START_PLAY:
                                    printf("Movie init OK\n");
                                    break;

                                case MPLAYER_EXIT_ERROR_FILE:
                                    printf("Movie unsupport file!\n");
                #if MM_REMOVE_TXT_SUBTITLE == 0
                                    _MM_TxtSubtitleFinalize();
                #endif
                                    TDAL_MPm_Stop();
                                    if (notifyFct != NULL)
                                    {
                                        notifyFct((tTDAL_MP_Handle) TDAL_MPi_DescList[index], eTDAL_MP_EVENT_STREAM_ERROR, TDAL_MPi_DescList[index]->pCtx);
                                    }
                                    break;

                                case MPLAYER_SUFFICIENT_DATA:
                                    break;
                                case MPLAYER_BLACKSCREEN_OFF:
                                    TDAL_DISPm_ReconfigureAndUnmuteVideoLayer(true, NULL, NULL, TRUE);
                                    break;
                            }
                            break;

                        case MPLAYER_COMMAND_SUBTITLE:
                            switch(msg.u32Param)
                            {
                                case MPLAYER_SET_SUBTITLE_PALETTE:
                                    break;
                                case MPLAYER_SUBTITLE_INIT:
                                    memcpy(&_prestSubtitleInit, (void*)msg.u32Info, sizeof(ST_CPCODEC_MSG_SUBTITLE_INIT_INFO));
                                    //_SubInitHandler((MS_U32)&_prestSubtitleInit);
                                    break;
                                case MPLAYER_SUBQUEUE_PUSH:
                                    _SubQueuePushHandler(msg.u32Info);
                                    break;
                                case MPLAYER_SUBTITLE_ENABLE:
                                    break;
                                case MPLAYER_SET_SUBTITLE_TRACK:
                                    break;
                                case MPLAYER_TS_SUBTITLE_PROGRAM_CHANGED:
                                    break;
                                case MPLAYER_SUBTITLE_DISPLAY:
                                    //_SubtitleDisplayHandler(msg.u32Info);
                                    break;
                            }
                            break;
                        default:
                            mTBOX_ASSERTm(FALSE, "Unsupported MM event\n");
                            break;
                    }
                break;
            case eTDAL_MP_CallbackMusic:
                switch (msg.eCmd)
                {
                    case MPLAYER_COMMAND_NOTIFY:
                        switch(msg.u32Param)
                        {
                            case MPLAYER_START_PLAY:
                                printf("Music Init ok!\n");
                                break;
                            case MPLAYER_EXIT_ERROR_FILE:
                                TDAL_MPm_Stop();
                                if (notifyFct != NULL)
                                {
                                    notifyFct((tTDAL_MP_Handle) TDAL_MPi_DescList[index], eTDAL_MP_EVENT_STREAM_ERROR, TDAL_MPi_DescList[index]->pCtx);
                                }
                                printf("Music play fail!\n");
                                break;
                            case MPLAYER_EXIT_OK:
                                TDAL_MPm_Stop();
                                if (notifyFct != NULL)
                                {
                                    notifyFct((tTDAL_MP_Handle) TDAL_MPi_DescList[index], eTDAL_MP_EVENT_FINISHED, TDAL_MPi_DescList[index]->pCtx);
                                }
                                printf("Music exit ok!\n");
                                break;
                            default:
                                printf("Unsupported music event %d\n", msg.u32Param);
                                break;
                        }
                        break;

                    default:
                        break;
                }

                break;
            case eTDAL_MP_CallbackPhoto:
                switch (msg.eCmd)
                {
                    case MPLAYER_COMMAND_NOTIFY:
                        switch(msg.u32Param)
                        {
                            case MPLAYER_PHOTO_DECODE_DONE:
                                printf("---------------->Decode Successfully!\n");
                                break;
                            case MPLAYER_PHOTO_DECODE_FAILED:
                                printf("---------------->Decode Error! \n");
                                break;
                            case MPLAYER_EXIT_ERROR_FILE:
                                printf("---------------->Unsupported File! \n");
                                break;

                            default:
                                break;
                        }
                        break;

                    default:
                        break;
                }
                break;
            default:
                mTBOX_ASSERT(false);
                break;
            }

            TDAL_MPi_UnlockMutex();

        }
        else
        {
            mTBOX_TRACE((kTBOX_NIV_CRITICAL, "[TDAL_MPi_CallbackTask] Message not received or message size not right, ret = %d, size = %d, actualSize = %d", bRet, sizeof(msg), actualSize));
        }
    }
#endif

}

#endif
