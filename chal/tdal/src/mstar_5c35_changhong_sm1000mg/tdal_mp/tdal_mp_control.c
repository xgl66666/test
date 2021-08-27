/*
 * tdal_mp_control.c
 *
 *  Created on: 5 Feb 2013
 *      Author: mijovic
 */

/********************************************************/
/*              Includes                                */
/********************************************************/
#include "tdal_mp.h"
#include "tdal_mp_p.h"
#include "crules.h"
#include "MsTypes.h"
#include "apiAUDIO.h"
#include "apiDAC.h"
#include "drvAUDIO.h"
#include "apiVDEC.h"
#include "apiVDEC_EX.h"
#include "apiDMX.h"
#include "apiHDMITx.h"
#include "drvTVEncoder.h"
#include "drvMVOP.h"

#include "tdal_dmx_module_priv.h"

#include "drvGPIO.h"
#include "apiAUDIO.h"

#include "msAPI_VE.h"
#include "drvXC_IOPort.h"
#include "apiXC.h"
#include "msAPI_XC.h"
#include "msAPI_MM.h"

#include "MsFS.h"
#include "stdio.h"

#include "tdal_common.h"
#include "tbox.h"

/********************************************************/
/*              Defines                                 */
/********************************************************/
#define MAX_SUPPORTED_RES_H 720
#define MAX_SUPPORTED_RES_V 576

/********************************************************/
/*              Macros                                  */
/********************************************************/
mTBOX_SET_MODULE(eTDAL_MP);

/********************************************************/
/*              Typedefs                                */
/********************************************************/

/********************************************************/
/*              Local Module Variables (MODULE)         */
/********************************************************/

/********************************************************/
/*              Global Variables (GLOBAL/IMPORT)        */
/********************************************************/

/********************************************************/
/*              Local File Variables (LOCAL)            */
/********************************************************/
LOCAL EN_MPLAYER_FILE_MODE eZmpMode = MPLAYER_FILE_MODE;

EN_MPLAYER_MEDIA_TYPE TDAL_MPm_eMediaType = E_MPLAYER_MEDIA_MOVIE;
MPLAYER_STREAM_IOCB TDAL_MPm_s_sStreamHandle;
MS_U8 TDAL_MPm_u8IsRepeat = FALSE;

LOCAL MS_U32 TDAL_MPi_u32FileOption = MPLAYER_MODE_INVALID;

const LOCAL EN_MPLAYER_VIDEO_CODEC_TYPE TDAL_MPi_SupportedVideoCodecs[] =
{
    MPLAYER_VIDEO_CODEC_MPEG2,
    MPLAYER_VIDEO_CODEC_MPEG1
};

const LOCAL EN_MPLAYER_AUDIO_CODEC_TYPE TDAL_MPi_SupportedAudioCodecs[] =
{
    MPLAYER_AUDIO_CODEC_MP2,
    MPLAYER_AUDIO_CODEC_MP3
};

/********************************************************/
/*              Local Functions Declarations (LOCAL)    */
/********************************************************/

/* fix from MSTAR for USB thread starvation in playback,
 * this fix is used to adjust miu group 1's member service number,
 * before and after playback */
void SetMIU_G1MaxServiceNum(MS_U16 value)
{
    *(volatile MS_U16*)(0xBF202400+(0x31<<2)) = value; // Set miu group 1 max member service number, default is 0x5
    printf("SetMIU_G1MaxServiceNum:%d\n", value);
}


LOCAL bool TDAL_MPi_GetMediaType(tTDAL_MP_Handle Handle, EN_MPLAYER_MEDIA_TYPE * mediaType)
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
        *mediaType = E_MPLAYER_MEDIA_MUSIC;
        mTBOX_RETURN(true);
    }

    mTBOX_TRACE((kTBOX_NIV_1, "[TDAL_MPi_GetMediaType] Media type is movie\n"));
    *mediaType = E_MPLAYER_MEDIA_MOVIE;
    mTBOX_RETURN(true);
}

#if 0
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
#endif

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

bool TDAL_MPi_IsAviContainer(tTDAL_MPm_Desc * desc)
{
    char headerBytes[11];
    int bytesRead;

    mTBOX_FCT_ENTER("TDAL_MPi_IsAviContainer");

    desc->pInputDataSeek((tTDAL_MP_Handle) desc, 0, eTDAL_MP_SEEK_MODE_SET);
    bytesRead = desc->pInputDataRead((tTDAL_MP_Handle) desc, headerBytes, sizeof(headerBytes));

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

    desc->pInputDataSeek((tTDAL_MP_Handle) desc, 0, eTDAL_MP_SEEK_MODE_SET);

    mTBOX_RETURN(true);

}


/******************************************************************************
 * Function Name : TDAL_MP_Start
 *
 * Description   : This function starts the rendering of a stream media.
 *
 * Return        : It returns eTDAL_MP_NO_ERROR if succeed.
 *                 It returns eTDAL_MP_NOT_INIT_ERROR if a previous initialization
 *                 has not been done.
 *                 It returns eTDAL_MP_BAD_PARAMETER_ERROR if input parameters
 *                 are bad.
 *                 eTDAL_MP_NOT_DONE_ERROR otherwise.
 *
 * Comment       :
 *
 *****************************************************************************/
tTDAL_MP_Error TDAL_MP_Start(tTDAL_MP_Handle Handle, tTDAL_MP_StartParams *pstParams)
{
    MPLAYER_MEDIA sMplayerMedia;
    tTDAL_MPm_Desc *pstDesc;
    EN_MPLAYER_FILE_MODE pFileOption = MPLAYER_FILE_MODE; // currently supporting only File mode

    mTBOX_FCT_ENTER("TDAL_MP_Start");

    pstDesc = (tTDAL_MPm_Desc *)Handle;

    if(!is_TDAL_MP_Initialized())
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_MP_Start: not initialized\n"));
        mTBOX_RETURN(eTDAL_MP_NOT_INIT_ERROR);
    }

    if (pstDesc == NULL)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_MP_Start: Handle is NULL\n"));
        mTBOX_RETURN(eTDAL_MP_BAD_PARAMETER_ERROR);
    }

  /*  if (TDAL_MPi_IsMediaRunning())
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_MP_Start: No media is running\n"));
        mTBOX_RETURN(eTDAL_MP_NOT_DONE_ERROR);
    }*/

#if defined(SDEC_GLUE_JPEG_TDAL_MP_PARTITION)
    if (!TDAL_MPi_ReleasePool())
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Could not release memory pool\n"));
        mTBOX_RETURN(eTDAL_MP_NO_MEMORY_ERROR);
    }
#endif

    TDAL_MPi_LockMutex();

    memset(&sMplayerMedia, 0, sizeof(MPLAYER_MEDIA));
    memset(&TDAL_MPm_s_sStreamHandle, 0, sizeof(MPLAYER_STREAM_IOCB));

    TDAL_MPi_u32FileOption = pFileOption;

    if(pFileOption != MPLAYER_FILE_MODE)
    {
        mTBOX_TRACE((kTBOX_NIV_1, "Warning: TDAL_MP_Start currently support MPLAYER_FILE_MODE\n"));
        TDAL_MPm_u8IsRepeat = FALSE; //currently only support MPLAYER_FILE_MODE
    }

    if(MApi_VDEC_IsAlive()==E_VDEC_OK)
    {
        MApi_VDEC_Rst();
        MApi_VDEC_Exit();
    }

    if(pFileOption == MPLAYER_FILE_MODE)
    {
        mTBOX_TRACE((kTBOX_NIV_1, "TDAL_MP_Start: MPLAYER_FILE_MODE\n"));

        pstDesc->bIsPlaying = true;

        if (!TDAL_MPi_GetMediaType(Handle, &TDAL_MPm_eMediaType))
        {
            mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Unsupported media type\n"));
            TDAL_MPi_UnlockMutex();
            mTBOX_RETURN(eTDAL_MP_NOT_DONE_ERROR);
        }

        if (TDAL_MPm_eMediaType == E_MPLAYER_MEDIA_MOVIE && !TDAL_MPi_IsAviContainer(pstDesc))
        {
            mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Movies are supported for AVI containers only\n"));
            TDAL_MPi_UnlockMutex();
            mTBOX_RETURN(eTDAL_MP_NOT_SUPPORTED_ERROR);
        }

        if (TDAL_MPm_eMediaType == E_MPLAYER_MEDIA_MUSIC && pstDesc->eStreamAudCodecType == eTDAL_MP_STREAM_CONTENT_PCM)
        {
            //=========================PCM========================
            MApi_MPlayer_SetOption(E_MPLAYER_MEDIA_MUSIC, MPLAYER_OPTION_SET_MUSIC_TYPE, MPLAYER_SUBTYPE_PCM);
            MPLAYER_PCM_PARAM* pPCM_Param;
            pPCM_Param = (MPLAYER_PCM_PARAM*)malloc(sizeof(MPLAYER_PCM_PARAM));
            pPCM_Param->eFormatTag = PCM_FORMAT_PCM;
            pPCM_Param->u16Channels = 2;
            pPCM_Param->u16BitsPerSample = 16;
            pPCM_Param->u32SamplesPerSec = 48000;
            pPCM_Param->bBigEndian = TRUE;
            MApi_MPlayer_SetOption(E_MPLAYER_MEDIA_MUSIC, MPLAYER_OPTION_MUSIC_SET_PCM_PARAM, (unsigned int)pPCM_Param);
        }

        // mm_print("\n[%d] %s :   -> %s %s\n", __LINE__, __FILE__, U8FileName,U8File);

        eZmpMode = MPLAYER_FILE_MODE;
        TDAL_MPm_s_sStreamHandle.pfnStreamOpen = MM_Simulate_Stream_Open_np;
        TDAL_MPm_s_sStreamHandle.pfnStreamClose = MM_Simulate_Stream_Close_np;
        TDAL_MPm_s_sStreamHandle.pfnStreamLength = MM_Simulate_Stream_Length_np;
        TDAL_MPm_s_sStreamHandle.pfnStreamSeek = MM_Simulate_Stream_Seek_np;
        TDAL_MPm_s_sStreamHandle.pfnStreamRead = MM_Simulate_Stream_Read_np;
        TDAL_MPm_s_sStreamHandle.pfnRtspOpen = NULL;
        TDAL_MPm_s_sStreamHandle.pfnRtspClose = NULL;
        TDAL_MPm_s_sStreamHandle.pfnRtspPlay = NULL;
        TDAL_MPm_s_sStreamHandle.pfnRtspPause = NULL;
        TDAL_MPm_s_sStreamHandle.pfnRtspResume = NULL;
        TDAL_MPm_s_sStreamHandle.pfnRtspGetPayload = NULL;

        //set display window, {0, 0, 0, 0} means full panel
        sMplayerMedia.eFileMode = MPLAYER_FILE_MODE;
        sMplayerMedia.u32DispX = 0;
        sMplayerMedia.u32DispY = 0;
        sMplayerMedia.u32DispW = 1920;
        sMplayerMedia.u32DispH = 1080;
        sMplayerMedia.u32GotoTimeMs = (unsigned int) pstParams->uiTime;
        sMplayerMedia.filename = (char *) Handle;
#if 0
        _MM_TxtSubtitleInit();
#endif
        if(TDAL_MPm_eMediaType == E_MPLAYER_MEDIA_PHOTO)
        {
            msAPI_VE_SetSourceType(E_MSAPI_VE_SOURCE_SCALER_OP2);
        }
        else
        {
            msAPI_VE_SetSourceType(E_MSAPI_VE_SOURCE_AUTO);
        }
        //msAPI_VE_SetMode();
        TDAL_DMXm_DisableFilters(true);

        SetMIU_G1MaxServiceNum(0x1);

        if (MApi_MPlayer_PlayFile(TDAL_MPm_eMediaType, &sMplayerMedia, &TDAL_MPm_s_sStreamHandle) == FALSE)
        {
            SetMIU_G1MaxServiceNum(0x5);

            mTBOX_TRACE((kTBOX_NIV_WARNING, "MApi_MPlayer_PlayFile returned error\n"));
            TDAL_MPi_UnlockMutex();
            mTBOX_RETURN(eTDAL_MP_NOT_DONE_ERROR);
        }

//        if (!TDAL_MPi_IsFileSupported())
//        {
//            TDAL_MPm_Stop();
//            MsOS_DelayTask(1000);
//            TDAL_MPi_UnlockMutex();
//            mTBOX_TRACE((kTBOX_NIV_1, "Unsupported file type\n"));
//            mTBOX_RETURN(eTDAL_MP_NOT_SUPPORTED_ERROR);
//        }
    }
    else if((pFileOption == MPLAYER_SEEKABLE_STREAM_MODE) || (pFileOption == MPLAYER_UNSEEKABLE_STREAM_MODE))
    {
        /* Not implemented, therefore not compiling */
#if 0
        mTBOX_TRACE((kTBOX_NIV_1, "TDAL_MP_Start: MPLAYER_SEEKABLE_STREAM_MODE""\n"));
        eZmpMode = MPLAYER_SEEKABLE_STREAM_MODE;
        if (!TDAL_MPi_GetMediaType(Handle, &TDAL_MPm_eMediaType))
        {
            mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Unsupported media type\n"));
            mTBOX_RETURN(eTDAL_MP_NOT_DONE_ERROR);
        }

        msAPI_VE_SetSourceType(E_MSAPI_VE_SOURCE_AUTO);
        msAPI_VE_SetMode();
#ifdef ENABLE_RTSP_PACKAGE

        mediainfo.media_length = 0;
        rtsp_set_heartbeatinterval(40);
        TDAL_MPm_s_sStreamHandle.pfnStreamOpen = MM_SeekAble_Stream_RTSP_Open;
        TDAL_MPm_s_sStreamHandle.pfnStreamClose = MM_SeekAble_Stream_RTSP_Close;
        TDAL_MPm_s_sStreamHandle.pfnStreamLength = MM_SeekAble_Stream_RTSP_Length;
        TDAL_MPm_s_sStreamHandle.pfnStreamSeek = MM_SeekAble_Stream_RTSP_Seek;
        TDAL_MPm_s_sStreamHandle.pfnStreamRead = MM_SeekAble_Stream_RTSP_Read;
        TDAL_MPm_s_sStreamHandle.pfnRtspOpen = NULL;
        TDAL_MPm_s_sStreamHandle.pfnRtspClose = NULL;
        TDAL_MPm_s_sStreamHandle.pfnRtspPlay = NULL;
        TDAL_MPm_s_sStreamHandle.pfnRtspPause = NULL;
        TDAL_MPm_s_sStreamHandle.pfnRtspResume = NULL;
        TDAL_MPm_s_sStreamHandle.pfnRtspGetPayload = NULL;
#else
        TDAL_MPm_s_sStreamHandle.pfnStreamOpen = NULL; // MM_Simulate_Stream_Open_np;
        TDAL_MPm_s_sStreamHandle.pfnStreamClose = NULL; // MM_Simulate_Stream_Close_np;
        TDAL_MPm_s_sStreamHandle.pfnStreamLength = NULL; // MM_Simulate_Stream_Length_np;
        TDAL_MPm_s_sStreamHandle.pfnStreamSeek = NULL; // MM_Simulate_Stream_Seek_np;
        TDAL_MPm_s_sStreamHandle.pfnStreamRead = NULL; // MM_Simulate_Stream_Read_np;
        TDAL_MPm_s_sStreamHandle.pfnRtspOpen = NULL;
        TDAL_MPm_s_sStreamHandle.pfnRtspClose = NULL;
        TDAL_MPm_s_sStreamHandle.pfnRtspPlay = NULL;
        TDAL_MPm_s_sStreamHandle.pfnRtspPause = NULL;
        TDAL_MPm_s_sStreamHandle.pfnRtspResume = NULL;
        TDAL_MPm_s_sStreamHandle.pfnRtspGetPayload = NULL;
#endif
        sMplayerMedia.eFileMode=MPLAYER_SEEKABLE_STREAM_MODE;
        sMplayerMedia.sSubtitle.eSubtitleType = MPLAYER_SUBTITLE_INVALID;
        sMplayerMedia.sSubtitle.SubtitleFileName = NULL;
        sMplayerMedia.sSubtitle.SubtitleSUBFileName = NULL;
        sMplayerMedia.filename = Handle;
        sMplayerMedia.u32DispX = 0;
        sMplayerMedia.u32DispY = 0;
        sMplayerMedia.u32DispW = 0;
        sMplayerMedia.u32DispH = 0;
        sMplayerMedia.u32GotoTimeMs = 0;
#ifdef ENABLE_RTSP_PACKAGE
        err = (rtsp_error_code)rtsp_open((char *)U8FileName,(void*) (&mediainfo), &g_rhandle);
        if(RTSP_NO_ERROR != err)
        {
            printf("rtsp_open error, errno = %d\n", err);
            err = (rtsp_error_code)rtsp_close(g_rhandle);
            if (RTSP_NO_ERROR != err)
            {
                printf("rtsp_close error, errno = %d\n", err);
            }
        }
        printf("mediainfo.media_length = %f\n", mediainfo.media_length);
        rtsplength = (int)mediainfo.media_length;

        //for rtsp_play
        err = (rtsp_error_code)rtsp_play(g_rhandle, (double)0.0, normal_play);
        if (RTSP_NO_ERROR != err)
        {
            printf("rtsp_play error, errno = %d\n", err);
            err = (rtsp_error_code)rtsp_close(g_rhandle);
            if (RTSP_NO_ERROR != err)
            {
                printf("rtsp_close error, errno = %d\n", err);
            }
        }
#endif
        if (MApi_MPlayer_PlayFile(TDAL_MPm_eMediaType, &sMplayerMedia, &TDAL_MPm_s_sStreamHandle) == FALSE)
        {
            mTBOX_TRACE((kTBOX_NIV_1, "MApi_MPlayer_PlayFile: Could not play stream\n"));
            mTBOX_RETURN(eTDAL_MP_NOT_DONE_ERROR);
        }
#endif
    }
    else if((pFileOption == MPLAYER_RTSP_MODE))
    {
        /* Not implemented, therefore not compiling */
#if 0
        eZmpMode = MPLAYER_RTSP_MODE;
        TDAL_MPm_eMediaType = E_MPLAYER_MEDIA_MOVIE;
        msAPI_VE_SetSourceType(E_MSAPI_VE_SOURCE_AUTO);
        msAPI_VE_SetMode();
#ifdef ENABLE_RTSP_PACKAGE
        TDAL_MPm_s_sStreamHandle.pfnStreamOpen = NULL;
        TDAL_MPm_s_sStreamHandle.pfnStreamClose = NULL;
        TDAL_MPm_s_sStreamHandle.pfnStreamLength = NULL;
        TDAL_MPm_s_sStreamHandle.pfnStreamSeek = NULL;
        TDAL_MPm_s_sStreamHandle.pfnStreamRead = NULL;
        TDAL_MPm_s_sStreamHandle.pfnRtspOpen = MApi_Rtsp_Open;
        TDAL_MPm_s_sStreamHandle.pfnRtspClose = MApi_Rtsp_Close;
        TDAL_MPm_s_sStreamHandle.pfnRtspPlay = MApi_Rtsp_Play;
        TDAL_MPm_s_sStreamHandle.pfnRtspPause = MApi_Rtsp_Pause;
        TDAL_MPm_s_sStreamHandle.pfnRtspResume = MApi_Rtsp_Pause;
        TDAL_MPm_s_sStreamHandle.pfnRtspGetPayload = MApi_Rtsp_Get_Payload;
#else
        TDAL_MPm_s_sStreamHandle.pfnStreamOpen = NULL;
        TDAL_MPm_s_sStreamHandle.pfnStreamClose = NULL;
        TDAL_MPm_s_sStreamHandle.pfnStreamLength = NULL;
        TDAL_MPm_s_sStreamHandle.pfnStreamSeek = NULL;
        TDAL_MPm_s_sStreamHandle.pfnStreamRead = NULL;
        TDAL_MPm_s_sStreamHandle.pfnRtspOpen = NULL; // MM_Simulate_Rtsp_Open;
        TDAL_MPm_s_sStreamHandle.pfnRtspClose = NULL; // MM_Simulate_Rtsp_Close;
        TDAL_MPm_s_sStreamHandle.pfnRtspPlay = NULL; // MM_Simulate_Rtsp_Play;
        TDAL_MPm_s_sStreamHandle.pfnRtspPause = NULL; // MM_Simulate_Rtsp_Pause;
        TDAL_MPm_s_sStreamHandle.pfnRtspResume = NULL; // MM_Simulate_Rtsp_Resume;
        TDAL_MPm_s_sStreamHandle.pfnRtspGetPayload = NULL; // MM_Simulate_Rtsp_Get_Payload;
#endif
        sMplayerMedia.eFileMode=MPLAYER_RTSP_MODE;
        sMplayerMedia.sSubtitle.eSubtitleType = MPLAYER_SUBTITLE_INVALID;
        sMplayerMedia.sSubtitle.SubtitleFileName = NULL;
        sMplayerMedia.sSubtitle.SubtitleSUBFileName = NULL;
        sMplayerMedia.filename = Handle;
        sMplayerMedia.u32DispX = 0;
        sMplayerMedia.u32DispY = 0;
        sMplayerMedia.u32DispW = 0;
        sMplayerMedia.u32DispH = 0;
        sMplayerMedia.u32GotoTimeMs = 0;
        if (MApi_MPlayer_PlayFile(TDAL_MPm_eMediaType, &sMplayerMedia, &TDAL_MPm_s_sStreamHandle) == FALSE)
        {

            return eTDAL_MP_NOT_DONE_ERROR;
        }
#endif
    }

    TDAL_MPi_UnlockMutex();

    mTBOX_RETURN(eTDAL_MP_NO_ERROR);
}

/******************************************************************************
 * Function Name : TDAL_MP_Stop
 *
 * Description   : This function stops the rendering of a media.
 *
 * Return        : It returns eTDAL_MP_NO_ERROR if succeed.
 *                 It returns eTDAL_MP_NOT_INIT_ERROR if a previous initialization
 *                 has not been done.
 *                 It returns eTDAL_MP_BAD_PARAMETER_ERROR if input parameters
 *                 are bad.
 *                 eTDAL_MP_NOT_DONE_ERROR otherwise.
 *
 * Comment       :
 *
 *****************************************************************************/
tTDAL_MP_Error TDAL_MP_Stop(tTDAL_MP_Handle Handle)
{
    tTDAL_MPm_Desc *pstDesc;

    mTBOX_FCT_ENTER("TDAL_MP_Stop");

    if (!is_TDAL_MP_Initialized())
	{
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_MP_Stop: not initialized\n"));
        mTBOX_RETURN(eTDAL_MP_NOT_INIT_ERROR);
	}

    if (Handle == (tTDAL_MP_Handle) NULL)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_MP_Stop: handle is NULL\n"));
        mTBOX_RETURN(eTDAL_MP_BAD_PARAMETER_ERROR);
    }

    pstDesc = (tTDAL_MPm_Desc *)Handle;

    TDAL_MPi_LockMutex();

    if(TDAL_MPm_eMediaType==E_MPLAYER_MEDIA_PHOTO)
    {
    	// TODO
        // appDemo_MM_ClearPhoto();
    }

    if(TDAL_MPm_Stop() != false)
    {
        TDAL_MPi_UnlockMutex();
        MsOS_DelayTask(1000);
        TDAL_MPi_LockMutex();

        SetMIU_G1MaxServiceNum(0x5);

        if (pstDesc->pEvtNotify)
        {
            pstDesc->pEvtNotify(Handle, eTDAL_MP_EVENT_STOPPED, pstDesc->pCtx);
        }

        mTBOX_TRACE((kTBOX_NIV_1, "TDAL_MP_Stop: Stream stopped\n"));
        TDAL_MPi_UnlockMutex();
        mTBOX_RETURN(eTDAL_MP_NO_ERROR);
    }

    SetMIU_G1MaxServiceNum(0x5);

    mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Could not stop MM\n"));
    TDAL_MPi_UnlockMutex();
    mTBOX_RETURN(eTDAL_MP_NOT_DONE_ERROR);
}

tTDAL_MP_Error TDAL_MP_PlayCtrlModeSet(tTDAL_MP_Handle Handle, uint32_t uiMode)
{
	tTDAL_MP_Error result = eTDAL_MP_NO_ERROR;

	return result;
}

tTDAL_MP_Error TDAL_MP_PosSet(tTDAL_MP_Handle Handle, uint32_t uiSec)
{
	tTDAL_MP_Error result = eTDAL_MP_NO_ERROR;

	return result;
}

tTDAL_MP_Error TDAL_MP_PosGet(tTDAL_MP_Handle Handle, uint32_t *puiSec)
{
	tTDAL_MP_Error result = eTDAL_MP_NO_ERROR;
	tTDAL_MPm_Desc * desc;
    MS_U32 u32Value = 0;
    int index;

    mTBOX_FCT_ENTER("TDAL_MP_PosGet");

    if (Handle == (tTDAL_MP_Handle) NULL || puiSec == NULL)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "[TDAL_MP_PosGet] Bad param\n"));
        mTBOX_RETURN(eTDAL_MP_BAD_PARAMETER_ERROR);
    }

    TDAL_MPi_LockMutex();

    desc = (tTDAL_MPm_Desc *) Handle;
    index = TDAL_MPi_GetRunningMediaDescriptor();

    if (index < 0 || desc != TDAL_MPi_DescList[index])
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "[TDAL_MP_PosGet] No running MM or given handle not playing\n"));
        TDAL_MPi_UnlockMutex();
        mTBOX_RETURN(eTDAL_MP_NOT_DONE_ERROR);
    }

    u32Value = MApi_MPlayer_GetOption(TDAL_MPm_eMediaType, MPLAYER_OPTION_MOVIE_INFO, MPLAYER_INFO_CUR_TIME);
    *puiSec = u32Value / 1000;

    TDAL_MPi_UnlockMutex();

	mTBOX_RETURN(result);
}

tTDAL_MP_Error TDAL_MP_SpeedSet(tTDAL_MP_Handle Handle, int32_t uiSpeed)
{
    int32_t index;
	tTDAL_MPm_Desc * desc;
	tTDAL_MP_Error result = eTDAL_MP_NO_ERROR;
	EN_MPLAYER_TRICK_MODE   nextTrickMode;
    EN_MPLAYER_TRICK_MODE   currentTrickMode;
    int32_t difference;

	mTBOX_FCT_ENTER("TDAL_MP_SpeedSet");

	if (Handle == (tTDAL_MP_Handle) NULL)
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL, "[TDAL_MP_SpeedSet] Handle not valid or speed not valid\n"));
		mTBOX_RETURN(eTDAL_MP_BAD_PARAMETER_ERROR);
	}

	TDAL_MPi_LockMutex();

	desc = (tTDAL_MPm_Desc *) Handle;
	index = TDAL_MPi_GetRunningMediaDescriptor();

	if (index < 0 || desc != TDAL_MPi_DescList[index])
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL, "[TDAL_MP_SpeedSet] No running MM or given handle not playing\n"));
		TDAL_MPi_UnlockMutex();
		mTBOX_RETURN(eTDAL_MP_NOT_DONE_ERROR);
	}

/*	if (!TDAL_MPi_IsMediaRunning())
	{
	    mTBOX_TRACE((kTBOX_NIV_WARNING, "No media is running, skipping pause\n"));
	    mTBOX_RETURN(eTDAL_MP_NOT_DONE_ERROR);
	}*/

	switch(uiSpeed)
	{
	case 0:
	    nextTrickMode = MPLAYER_TRICK_PAUSE;
	    difference = 0;
	    break;
	case 100:
	    nextTrickMode = MPLAYER_TRICK_NORMAL;
	    difference = 1;
	    break;
	case 200:
        nextTrickMode = MPLAYER_TRICK_FF_2X;
        difference = 2;
        break;
	case 400:
	    nextTrickMode = MPLAYER_TRICK_FF_4X;
	    difference = 3;
	    break;
	case 800:
	    nextTrickMode = MPLAYER_TRICK_FF_8X;
	    difference = 4;
	    break;
	case 1600:
	    nextTrickMode = MPLAYER_TRICK_FF_16X;
	    difference = 5;
	    break;
	case -100:
	case -200:
	    nextTrickMode = MPLAYER_TRICK_FB_2X;
	    difference = -1;
	    break;
	case -400:
	    nextTrickMode = MPLAYER_TRICK_FB_4X;
	    difference = -2;
	    break;
	case -800:
	    nextTrickMode = MPLAYER_TRICK_FB_8X;
	    difference = -3;
	    break;
	case -1600:
	    nextTrickMode = MPLAYER_TRICK_FB_16X;
        difference = -4;
	    break;
	default:
	    result = eTDAL_MP_NOT_SUPPORTED_ERROR;
	    mTBOX_TRACE((kTBOX_NIV_WARNING, "[TDAL_MP_SpeedSet] Unsupported trick mode %d. Skipping\n", uiSpeed));
	    break;
	}

	if (result == eTDAL_MP_NO_ERROR)
	{

	    currentTrickMode = MApi_MPlayer_GetOption(TDAL_MPm_eMediaType, MPLAYER_OPTION_TRICK_MODE, 0);

	    if (currentTrickMode == nextTrickMode)
	    {
	        /* Speed is already set, exit here */
	        mTBOX_RETURN(eTDAL_MP_NO_ERROR);
	    }

        switch(uiSpeed)
        {
        case 0:
            mTBOX_TRACE((kTBOX_NIV_1, "[TDAL_MP_SpeedSet] PAUSE \n"));
            currentTrickMode = MApi_MPlayer_Pause(TDAL_MPm_eMediaType);
            if (currentTrickMode != MPLAYER_TRICK_PAUSE)
            {
                mTBOX_TRACE((kTBOX_NIV_WARNING, "[TDAL_MP_SpeedSet] Could not pause stream\n"));
                result = eTDAL_MP_NOT_DONE_ERROR;
            }
            break;
        case 100:
            mTBOX_TRACE((kTBOX_NIV_1, "[TDAL_MP_SpeedSet] RESUME \n"));
            currentTrickMode = MApi_MPlayer_Resume(TDAL_MPm_eMediaType);
            if (currentTrickMode != MPLAYER_TRICK_NORMAL)
            {
                mTBOX_TRACE((kTBOX_NIV_WARNING, "[TDAL_MP_SpeedSet] Could not resume stream\n"));
                result = eTDAL_MP_NOT_DONE_ERROR;
            }
            break;

        default:
        {
            int32_t changedTrickMode = currentTrickMode;

            /* calculating difference between current and wanted trick mode */

            difference -=  (currentTrickMode == MPLAYER_TRICK_PAUSE ? 0 :
                            currentTrickMode == MPLAYER_TRICK_NORMAL ? 1 :
                            currentTrickMode == MPLAYER_TRICK_FF_2X ? 2 :
                            currentTrickMode == MPLAYER_TRICK_FF_4X ? 3 :
                            currentTrickMode == MPLAYER_TRICK_FF_8X ? 4 :
                            currentTrickMode == MPLAYER_TRICK_FF_16X ? 5 :
                            currentTrickMode == MPLAYER_TRICK_FB_2X ? -1 :
                            currentTrickMode == MPLAYER_TRICK_FB_4X ? -2 :
                            currentTrickMode == MPLAYER_TRICK_FB_8X ? -3 :
                            currentTrickMode == MPLAYER_TRICK_FB_16X ? -4 : 0);

            difference += (currentTrickMode == MPLAYER_TRICK_NORMAL && nextTrickMode == MPLAYER_TRICK_FB_2X  ? -1 :
                           currentTrickMode == MPLAYER_TRICK_FB_2X  && nextTrickMode == MPLAYER_TRICK_NORMAL ?  1 : 0);

          	while ((difference != 0) && (currentTrickMode != nextTrickMode))
        	{
        		if (difference < 0)
        		{
        		    mTBOX_TRACE((kTBOX_NIV_1, "[TDAL_MP_SpeedSet] BACKWARD\n"));
        		    changedTrickMode = MApi_MPlayer_RW(TDAL_MPm_eMediaType);
        			difference ++;
        		}
        		else
        		{
        		    mTBOX_TRACE((kTBOX_NIV_1, "[TDAL_MP_SpeedSet] FASTFORWARD\n"));
        		    changedTrickMode = MApi_MPlayer_FF(TDAL_MPm_eMediaType);
        			difference --;
        		}

        		if (changedTrickMode == currentTrickMode)
        		{
        		    /* speed change failed, don't proceed further */
        		    break;
        		}

        		currentTrickMode = changedTrickMode;
        	}

          	if (currentTrickMode != nextTrickMode)
          	{
          	    mTBOX_TRACE((kTBOX_NIV_WARNING, "[TDAL_MP_SpeedSet] Failed to set speed\n"));
          	    result = eTDAL_MP_NOT_DONE_ERROR;
          	}

            break;
        }
        }

        mTBOX_TRACE((kTBOX_NIV_1, "[TDAL_MP_SpeedSet] CURRENT TRICKMODE: %s\n",
                                    currentTrickMode == MPLAYER_TRICK_PAUSE ? "PAUSE" :
                                    currentTrickMode == MPLAYER_TRICK_NORMAL ? "NORMAL" :
                                    currentTrickMode == MPLAYER_TRICK_FF_2X ? "FF_2X" :
                                    currentTrickMode == MPLAYER_TRICK_FF_4X ? "FF_4X" :
                                    currentTrickMode == MPLAYER_TRICK_FF_8X ? "FF_8X" :
                                    currentTrickMode == MPLAYER_TRICK_FF_16X ? "FF_16X" :
                                    currentTrickMode == MPLAYER_TRICK_FB_2X ? "FB_2X" :
                                    currentTrickMode == MPLAYER_TRICK_FB_4X ? "FB_4X" :
                                    currentTrickMode == MPLAYER_TRICK_FB_8X ? "FB_8X" :
                                    currentTrickMode == MPLAYER_TRICK_FB_16X ? "FB_16X" : "OTHER" ));
	}

	TDAL_MPi_UnlockMutex();
	mTBOX_RETURN(result);
}

tTDAL_MP_Error TDAL_MP_SpeedGet(tTDAL_MP_Handle Handle, int32_t *puiSpeed)
{
	tTDAL_MP_Error result = eTDAL_MP_NO_ERROR;
	EN_MPLAYER_TRICK_MODE trickMode;
	tTDAL_MPm_Desc * desc;
	int index;

	mTBOX_FCT_ENTER(TDAL_MP_SpeedGet);

    if (Handle == (tTDAL_MP_Handle) NULL)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "[TDAL_MP_SpeedGet] Handle not valid or speed not valid\n"));
        mTBOX_RETURN(eTDAL_MP_BAD_PARAMETER_ERROR);
    }

    if (puiSpeed == NULL)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "[TDAL_MP_SpeedGet] puiSpeed NULL\n"));
        mTBOX_RETURN(eTDAL_MP_BAD_PARAMETER_ERROR);
    }

    TDAL_MPi_LockMutex();

    desc = (tTDAL_MPm_Desc *) Handle;
    index = TDAL_MPi_GetRunningMediaDescriptor();

    if (index < 0 || desc != TDAL_MPi_DescList[index])
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "[TDAL_MP_SpeedSet] No running MM or given handle not playing\n"));
        TDAL_MPi_UnlockMutex();
        mTBOX_RETURN(eTDAL_MP_NOT_DONE_ERROR);
    }

    trickMode = MApi_MPlayer_GetOption(TDAL_MPm_eMediaType, MPLAYER_OPTION_TRICK_MODE, 0);

	switch(trickMode)
	{
    case MPLAYER_TRICK_PAUSE:
        *puiSpeed = 0;
        break;
    case MPLAYER_TRICK_NORMAL:
        *puiSpeed = 100;
        break;
    case MPLAYER_TRICK_FF_2X:
        *puiSpeed = 200;
        break;
    case MPLAYER_TRICK_FF_4X:
        *puiSpeed = 400;
        break;
    case MPLAYER_TRICK_FF_8X:
        *puiSpeed = 800;
        break;
    case MPLAYER_TRICK_FF_16X:
        *puiSpeed = 1600;
        break;
    case MPLAYER_TRICK_FB_2X:
        *puiSpeed = -200;
        break;
    case MPLAYER_TRICK_FB_4X:
        *puiSpeed = -400;
        break;
    case MPLAYER_TRICK_FB_8X:
        *puiSpeed = -800;
        break;
    case MPLAYER_TRICK_FB_16X:
        *puiSpeed = -1600;
        break;
    default:
        *puiSpeed = 0;
        result = eTDAL_MP_NOT_DONE_ERROR;
        break;
	}

	TDAL_MPi_UnlockMutex();

	mTBOX_RETURN(result);
}

bool TDAL_MPm_Stop(void)
{
    bool returnValue = false;

    mTBOX_FCT_ENTER("TDAL_MPm_Stop");

 /*   if (!TDAL_MPi_IsMediaRunning())
    {
        mTBOX_TRACE((kTBOX_NIV_WARNING, "No media running, skipping stop\n"));
        mTBOX_RETURN(false);
    }*/

    if(MApi_MPlayer_Stop(TDAL_MPm_eMediaType) == TRUE)
    {
        int i;

        TDAL_DMXm_DisableFilters(false);

        //msAPI_VE_SetVideoMute(ENABLE);
        msAPI_XC_SetVideoMute(ENABLE, MAIN_WINDOW);

        if (TDAL_MPm_eMediaType == E_MPLAYER_MEDIA_MOVIE)
        {
            TDAL_DMXm_SetDmxFlow();
        }

        i = TDAL_MPi_GetRunningMediaDescriptor();

        if (i >= 0)
        {
            TDAL_MPi_DescList[i]->bIsPlaying = false;
        }

        mTBOX_TRACE((kTBOX_NIV_1, "TDAL_MPm_Stop: Stream stopped\n"));
        returnValue = true;
    }
    else
    {
        mTBOX_TRACE((kTBOX_NIV_WARNING, "Could not stop file\n"));
    }

    mTBOX_RETURN(returnValue);
}

void TDAL_MPm_UsbRemoved(void)
{
    int i =  TDAL_MPi_GetRunningMediaDescriptor();
    if (i >= 0)
    {
        TDAL_MPm_Stop();
    }
}


