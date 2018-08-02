/*
 * tdal_mp_generic.c
 *
 *  Created on: 4 Feb 2013
 *      Author: mijovic
 */

/********************************************************/
/*              Includes                                */
/********************************************************/

#include <sys/stat.h>
#include <pthread.h>
#include <assert.h>

#include "crules.h"
#include "tdal_mp.h"
#include "tdal_mp_p.h"

#include "tdal_disp.h"

#include "tbox.h"
#include "tdal_common.h"
#include "tdal_common_priv.h"
#include "tdal_fs.h"
#include "tdal_fs_p.h"

#include "MsCommon.h"
#include "MsTypes2.h"
#include "MsTypes.h"

#include "mmsdk_interface_def.h"
#include "mmsdk_interface.h"
#include "msapi_MM_Common.h"
#include "msapi_MM_Display.h"

#include "porting_sysinfo.h"
#include "MsFS.h"
#include "MM_Player_Def.h"
#include "MApi_MEMConfig_Interface.h"

#include "apiHDMITx.h"
#include "drvXC_IOPort.h"
#include "apiXC.h"
#include "apiVDEC_EX.h"
#include "apiVDEC.h"
#include "drvMVOP.h"
#include "apiDMX.h"
#include "apiAUDIO.h"
#include "apiDAC.h"
#include "MsMemory.h"
#include "msAPI_XC.h"

#include "tdal_disp.h"
#include "tdal_disp_module_priv.h"
#include "tdal_av.h"
/********************************************************/
/*              Defines                                 */
/********************************************************/
#define MM_REMOVE_TXT_SUBTITLE 1
#define kTDAL_MP_REVISION "TDAL_MP_v0.0"
#define MM_COPROCESSOR_ADDR                     0x00000000  /* Alignment 0 */
#define MM_COPROCESSOR_LEN                      0x00200000 /* 2M */
#define MM_COPROCESSOR_TYPE                     (MIU0)
#define PROTECT_TDAL_MP
/********************************************************/
/*              Macros                                  */
/********************************************************/
mTBOX_SET_MODULE(eTDAL_MP);

/********************************************************/
/*              Typedefs                                */
/********************************************************/

typedef enum
{
    eTDAL_MP_CallbackMovie,
    eTDAL_MP_CallbackMusic,
    eTDAL_MP_CallbackPhoto,
} tTDAL_MPi_CallbackType;

typedef struct
{
    tTDAL_MPi_CallbackType callbackType;
    EN_MMSDK_CALLBACK_MSG eCmd;
    unsigned int  u32Param;
    unsigned int  u32Info;
    bool shouldExit;
} tTDAL_MPi_CallbackMsg;

/********************************************************/
/*              Local Module Variables (MODULE)         */
/********************************************************/
#define DRM_MODEL_ID                    0x3130
#define MMSDK_BUF_PHOTO_MEMORY_POOL_LEN 0x0000100000

#ifndef VDPLAYER_BS_EXT_AVAILABLE
#define MMSDK_BUF_VDPLAYER_BITSTREAM_AUDIO 0x0000400000
#define BITSTREAM_AUDIO_BUFFER_ALIGNMENT 0x1000
#endif

/********************************************************/
/*              Global Variables (GLOBAL/IMPORT)        */
/********************************************************/
IMPORT MS_S32 gs32CachedPoolID;
IMPORT MS_S32 gs32NonCachedPoolID;
IMPORT unsigned char Customer_info[49];
IMPORT MS_U8 Customer_hash[];

/********************************************************/
/*              Local File Variables (LOCAL)            */
/********************************************************/
LOCAL MS_BOOL b_MM_Initied = FALSE;
// wait porting LOCAL ST_CPCODEC_MSG_SUBTITLE_INIT_INFO _prestSubtitleInit;
LOCAL MS_BOOL b_InternalSubtitleInitialize = FALSE;

LOCAL MS_U8 u8TSPFirmwareForRTSP[] = {
    #include "../../../../ddi_pkg_ecos/bsp/lib/fw/fwTSP.dat"
};

LOCAL TDAL_queue_id TDAL_MPi_CallbackQueue = -1;
LOCAL TDAL_task_id TDAL_MPi_CallbackTaskId = NULL;
LOCAL uint8_t TDAL_MPi_CallbackTaskStack[4096];

tTDAL_MPm_Desc *TDAL_MPi_DescList[kTDAL_MPm_OPEN_SESSION_MAX];

LOCAL TDAL_mutex_id TDAL_MPi_Mutex = NULL;
LOCAL uint32_t TDAL_MPi_MutexThreadLocked = 0;
LOCAL tTDAL_MP_Capabilities _MPCapabilities = 0;
#ifndef VDPLAYER_BS_EXT_AVAILABLE
LOCAL void* _gpBSAudBufVA;
#endif




//for ring buffer info
MS_U32 _gu32VdplayerReadTimerOut = 0xFFFFFFFF;
MS_BOOL _gbFastInit = FALSE;
MS_U32 _gu32InitDataSize = 0x20000;
MS_S32 _gs32MinBufferSec = 10;
MS_S32 _gs32MaxBufferSec = 30;

EN_MMSDK_MEDIA_TYPE _geMediaType = E_MMSDK_MEDIA_TYPE_MOVIE;
static MS_U32 _gu32FileOption = MPLAYER_MODE_INVALID;

MS_U8 _gu8FileExtName[EXTENTION_NAME_LENGTH] = {};
MS_BOOL _gbFileSupported = TRUE;
//for debug info.....
EN_MMSDK_DBG_LEVEL _geDebugLevel  = E_MMSDK_DBG_LEVEL_NONE;

/********************************************************/
/*              Local Functions Declarations (LOCAL)    */
/********************************************************/
LOCAL MS_BOOL _TDAL_MPm_MM_MemInit();

#if defined(SDEC_GLUE_JPEG_TDAL_MP_PARTITION)

LOCAL MS_S32     TDAL_MPi_MemPart = -1;
LOCAL uint32_t   TDAL_MPi_AllocCount = 0;

IMPORT void Get_NonCachePoolID(MS_S32 *s32NonCachePoolID);
IMPORT void Get_CachePoolID(MS_S32 *s32CachePoolID);

static void _PT_SYS_ReadDB(ST_MMSDK_DIVXDRM_INFO *pstDRMInfo)
{
    //To read from NVM
}

static void _PT_SYS_WriteDB(ST_MMSDK_DIVXDRM_INFO *pstDRMInfo)
{
    //To write to NVM
}

tTDAL_MP_Error TDAL_MP_Malloc(uint32_t size, void ** p)
{
    tTDAL_MP_Error err;
    mTBOX_FCT_ENTER("TDAL_MP_Malloc");
#if 0 //wait porting


    if (size == 0 || p == NULL)
    {
        mTBOX_RETURN(eTDAL_MP_BAD_PARAMETER_ERROR);
    }

    TDAL_MPi_LockMutex();

    if (TDAL_MPi_MemPart == -1)
    {
        TDAL_MPi_MemPart = MsOS_CreateMemoryPool(VDPLAYER_DATA_LEN + VDPLAYER_SUB_BS_LEN + VDPLAYER_BS_LEN + VDPLAYER_BS_EXT_LEN,
                        0,
                        (void *) MsOS_PA2KSEG0((MS_U32) VDPLAYER_DATA_ADR),
                        E_MSOS_FIFO,
                        "TDAL MP pool");
        mTBOX_ASSERT(TDAL_MPi_MemPart != -1);
    }

    *p = MsOS_AllocateMemory((MS_U32) size, TDAL_MPi_MemPart);
    if (*p == NULL)
    {
        err = eTDAL_MP_NO_MEMORY_ERROR;
    }
    else
    {
        TDAL_MPi_AllocCount++;
        err = eTDAL_MP_NO_ERROR;
    }

    TDAL_MPi_UnlockMutex();
#endif
    mTBOX_RETURN(err);
}

tTDAL_MP_Error TDAL_MP_Free(void * p)
{
    tTDAL_MP_Error err;
#if 0 //wait porting

    if (p == 0)
    {
        return eTDAL_MP_BAD_PARAMETER_ERROR;
    }

    TDAL_MPi_LockMutex();

    err = MsOS_FreeMemory(p, TDAL_MPi_MemPart) != FALSE ? eTDAL_MP_NO_ERROR : eTDAL_MP_BAD_PARAMETER_ERROR;
    if (err == eTDAL_MP_NO_ERROR)
    {
        TDAL_MPi_AllocCount--;
    }

    TDAL_MPi_UnlockMutex();
#endif
    return err;
}

uint32_t TDAL_MP_GetAvailableSpace()
{
    uint32_t ret = 0;
#if 0 //wait porting

    TDAL_MPi_LockMutex();

    if (TDAL_MPi_MemPart != -1)
    {
        void * poolAdr;
        MS_U32 poolSize, freeSize, largestBlockSize;

        if (MsOS_InfoMemoryPool(TDAL_MPi_MemPart, &poolAdr, &poolSize, &freeSize, &largestBlockSize))
        {
            ret = largestBlockSize;
        }
    }

    TDAL_MPi_UnlockMutex();
#endif
    return ret;
}

bool TDAL_MPi_ReleasePool(void)
{
    bool retValue = true;
    mTBOX_FCT_ENTER("TDAL_MPi_ReleasePool");
#if 0 //wait porting
    MS_BOOL bRet;

    TDAL_MPi_LockMutex();

    if (TDAL_MPi_MemPart != -1)
    {
        if (TDAL_MPi_AllocCount != 0)
        {
            mTBOX_TRACE((kTBOX_NIV_CRITICAL, "There are still memory allocated in memory pool, alloc count = %d. Could not release\n", TDAL_MPi_AllocCount));
            retValue = false;
        }
        else
        {
            bRet = MsOS_DeleteMemoryPool(TDAL_MPi_MemPart);
            mTBOX_ASSERT(bRet == TRUE);
            TDAL_MPi_MemPart = -1;
            retValue = true;
        }
    }

    TDAL_MPi_UnlockMutex();
#endif
    mTBOX_RETURN(retValue);
}

#endif
/******************************************************************************
 * Function Name : TDAL_MP_Init
 *
 * Description   : This function initialises the TDAL_MP module.
 *
 * Return        : It returns eTDAL_MP_NO_ERROR if initialization succeed.
 *                 eTDAL_MP_NOT_DONE_ERROR otherwise.
 *
 * Comment       : This function must be called before any other functions
 *                 of the TDAL_MP API.
 *
 *****************************************************************************/

tTDAL_MP_Error TDAL_MP_Init(tTDAL_MP_InitParams *pstInitParams)
{
    tTDAL_MP_Error result = eTDAL_MP_NO_ERROR;
//    MS_U8 u8Volume = DEFAULT_VOLUME;
//    MS_BOOL bMute = FALSE;
    MS_S32 s32MstarCachedPoolID = INVALID_POOL_ID,s32MstarNonCachedPoolID=INVALID_POOL_ID;

    Get_CachePoolID(&s32MstarCachedPoolID);
    Get_NonCachePoolID(&s32MstarNonCachedPoolID);
    
    mTBOX_FCT_ENTER("TDAL_MP_Init");

    if (b_MM_Initied)
        return FALSE;

    TDAL_AV_Init();

    //Prepare sys related info before MMSDK_Initialize()
    if (PT_SYS_SetCusInfo(Customer_info, Customer_hash) == FALSE)
    {
        result = eTDAL_MP_NOT_DONE_ERROR;
        mTBOX_RETURN(result);        
    }
    PT_SYS_SetMemPoolID(s32MstarCachedPoolID, s32MstarNonCachedPoolID);
    PT_SYS_SetTspVQInfo(TSP_VQ_BUF_ADR, TSP_VQ_BUF_LEN);
    PT_SYS_SetDBReadWrite(_PT_SYS_ReadDB, _PT_SYS_WriteDB);
    PT_SYS_SetDRMID(DRM_MODEL_ID);//to revise? by teddy.chen
    _TDAL_MPm_MM_MemInit();
    //Create and initialize MM player.
    TDAL_MPm_Player_Initialize();

    //For image type subtitle
    msAPI_MM_Scaler_ForceOpen();

    MApi_AUDIO_SetSourceInfo(E_AUDIO_INFO_GAME_IN);
    PT_SYS_SetMMPhotoPath(MM_PHOTO_PATH);
    //audio de-mute and initial volume setting , move from mm mdw
//    bMute = FALSE;
//    Demo_Audio_SetMute(&bMute);

//    u8Volume = DEFAULT_VOLUME;
//    Demo_Audio_SetAbsoluteVolume(&u8Volume);
    _MPCapabilities = eTDAL_MP_CAPS_TRACK | eTDAL_MP_CAPS_SPEED | eTDAL_MP_CAPS_POS;
    TDAL_CreateMutex(&TDAL_MPi_Mutex);

    if (TDAL_MPi_Mutex == NULL)
    {
        result = eTDAL_MP_NOT_DONE_ERROR;        
        mTBOX_RETURN(result);
    }
    b_MM_Initied = TRUE;
    mTBOX_RETURN(result);
}

/******************************************************************************
 * Function Name : TDAL_MP_Terminate
 *
 * Description   : This function terminates the AVP module.
 *
 * Return        : It returns eTDAL_MP_NO_ERROR if succeed.
 *                 It returns eTDAL_MP_NOT_INIT_ERROR
 *                 if a previous initialization has not been done.
 *                 It returns eTDAL_MP_NOT_DONE_ERROR otherwise.
 *
 * Comment       : This function frees resources and terminates TDAL_MP
 *                 module. Any function of TDAL_MP API should not be
 *                 called after TDAL_MP_Terminate(-) call.
 *
 *****************************************************************************/
tTDAL_MP_Error TDAL_MP_Terminate (void)
{
    tTDAL_MP_Error result = eTDAL_MP_NO_ERROR;
    MS_S32 s32MstarCachedPoolID = INVALID_POOL_ID,s32MstarNonCachedPoolID=INVALID_POOL_ID;
    int i = 0;
    mTBOX_FCT_ENTER("TDAL_MP_Terminate");

    if(b_MM_Initied == FALSE)
    {
        mTBOX_TRACE((kTBOX_NIV_WARNING, "TDAL_MP_Terminate: MM not initialized\n"));
        mTBOX_RETURN(eTDAL_MP_NOT_INIT_ERROR);
    }
    
    i = TDAL_MPi_GetRunningMediaDescriptor();

    if (i != -1)
    {
        TDAL_MP_Stop((tTDAL_MP_Handle)TDAL_MPi_DescList[i]);
        TDAL_MP_Close((tTDAL_MP_Handle)TDAL_MPi_DescList[i]);
    }
    
    Get_CachePoolID(&s32MstarCachedPoolID);
    Get_NonCachePoolID(&s32MstarNonCachedPoolID);

    if (!TDAL_MPm_Player_Finalize())
    {
        result = eTDAL_MP_NOT_DONE_ERROR;
    }    
    
 #ifndef VDPLAYER_BS_EXT_AVAILABLE
    if(_gpBSAudBufVA)
    {
        MsOS_FreeMemory(_gpBSAudBufVA, s32MstarNonCachedPoolID);
        _gpBSAudBufVA = NULL;
    }
#endif

    if (result != eTDAL_MP_NO_ERROR)
    {
        mTBOX_TRACE((kTBOX_NIV_WARNING, "TDAL_MPm_MPlayer_Finalize not done!\n"));
        mTBOX_RETURN(result);
    }
    
    if (!TDAL_MPm_Finalize())
    {
        result = eTDAL_MP_NOT_DONE_ERROR;
        mTBOX_TRACE((kTBOX_NIV_WARNING, "-----> MM_Finalize ERROR ERROR!!!!\n"));
    }
    
    if (result != eTDAL_MP_NO_ERROR)
    {
        mTBOX_TRACE((kTBOX_NIV_WARNING, "_MM_Finalize not done!\n"));
        mTBOX_RETURN(result);
    }
    
    b_MM_Initied = FALSE;       
#if 0 // wait porting


    if(b_MM_Initied == FALSE)
    {
    	mTBOX_TRACE((kTBOX_NIV_WARNING, "TDAL_MP_Terminate: MM not initialized\n"));
        mTBOX_RETURN(eTDAL_MP_NOT_INIT_ERROR);
    }

    MApi_MPlayer_Finalize();
    MApi_MPlayer_RegisterCallBack(E_MPLAYER_MEDIA_MOVIE, NULL);
    MApi_MPlayer_RegisterCallBack(E_MPLAYER_MEDIA_PHOTO, NULL);
    MApi_MPlayer_RegisterCallBack(E_MPLAYER_MEDIA_MUSIC, NULL);

    if(TDAL_MPm_eMediaType==E_MPLAYER_MEDIA_MOVIE)
    {
        // user would swith to live after watching ts file
        // DemoAV_Tuner_Config(&flow, &inSrc, &clkInv, &extSync, &parallel);
    	//TDAL_DMX_Init();
        mTBOX_TRACE((kTBOX_NIV_1, "MPlayer movie requires DMX reinialization. Check this!!!"));
    }

    if (TDAL_MPi_CallbackQueue != -1)
    {
        tTDAL_MPi_CallbackMsg msg;
        MS_BOOL bRet;

        msg.shouldExit = true;
#if MSTAR_QUEUE
        bRet = MsOS_SendToQueue(TDAL_MPi_CallbackQueue, (MS_U8 *) &msg, sizeof(tTDAL_MPi_CallbackMsg), MSOS_WAIT_FOREVER);
#else
        bRet = TDAL_Enqueue(TDAL_MPi_CallbackQueue, &msg);
#endif
        mTBOX_ASSERT(bRet == TRUE);
    }

    if (TDAL_MPi_CallbackTaskId != NULL)
    {
        /* This waits until thread finished and deletes it */
        TDAL_DeleteTask(TDAL_MPi_CallbackTaskId);
        TDAL_MPi_CallbackTaskId = NULL;
    }

    if (TDAL_MPi_CallbackQueue != -1)
    {
#if MSTAR_QUEUE
        MsOS_DeleteQueue(TDAL_MPi_CallbackQueue);
#else
        TDAL_DeleteQueue(TDAL_MPi_CallbackQueue);
#endif
        TDAL_MPi_CallbackQueue = -1;
    }

    if (TDAL_MPi_Mutex != NULL)
    {
        TDAL_DeleteMutex(TDAL_MPi_Mutex);
        TDAL_MPi_Mutex = NULL;
    }

    b_MM_Initied = FALSE;
    
#endif

	mTBOX_RETURN(result);
}

char      *TDAL_MP_RevisionGet(void) {

    mTBOX_FCT_ENTER("TDAL_MP_RevisionGet");
    

	return kTDAL_MP_REVISION;
}
tTDAL_MP_Error TDAL_MP_Open(tTDAL_MP_OpenParams *pstParams, tTDAL_MP_Handle *pHandle) {

    tTDAL_MP_Error result = eTDAL_MP_NO_ERROR;
    tTDAL_MPm_Desc *pstDesc = NULL;
    int i;
    uint8_t ucDescIdx = 0;

    MPLAYER_MEDIA sMplayerMeida;
    EN_MPLAYER_FILE_MODE pFileOption = MPLAYER_FILE_MODE;
    uint8_t _u8FileName[FILE_NAME_LENGTH] = {};
    mTBOX_FCT_ENTER("TDAL_MP_Open");

    if((pstParams == (tTDAL_MP_OpenParams *)NULL))
    {
    	mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_MP_Open: Bad input parameters\n"));
    	result = eTDAL_MP_BAD_PARAMETER_ERROR;
    }

    TDAL_MPi_LockMutex();

    if(result == eTDAL_MP_NO_ERROR)
    {
        /* search for a free empty desc in the list */
        for(i = 0; i < kTDAL_MPm_OPEN_SESSION_MAX; i++)
        {
            if(TDAL_MPi_DescList[i] == (tTDAL_MPm_Desc *)NULL)
            {
                ucDescIdx = i;
                break;
            }
        }

        if(ucDescIdx == kTDAL_MPm_OPEN_SESSION_MAX)
        {
            mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_MP_Open: Max session reached\n"));
            result = eTDAL_MP_NB_HANDLE_MAX_ERROR;
        }
    }

    if(result == eTDAL_MP_NO_ERROR)
    {
        /* allocate a media descriptor */
        pstDesc = TDAL_Malloc(sizeof(tTDAL_MPm_Desc));

        if(pstDesc == NULL)
        {
            mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_MP_Open: Media desc alloc failed\n"));
            result = eTDAL_MP_NO_MEMORY_ERROR;
        }
    }

    if(result == eTDAL_MP_NO_ERROR)
    {
        memset(pstDesc,         0, sizeof(tTDAL_MPm_Desc));
        pstDesc->uiMagicNumber  = kTDAL_MPm_MAGIC_NUMBER;
        pstDesc->pEvtNotify     = pstParams->EvtNotify;
        if (pstDesc->pEvtNotify == NULL)
        {
            mTBOX_TRACE((kTBOX_NIV_WARNING, "pstDesc->pEvtNotify is NULL\n")); 
        }
        pstDesc->pInputDataRead = pstParams->InputDataRead;
        pstDesc->pInputDataSeek = pstParams->InputDataSeek;
        pstDesc->pInputDataLength = pstParams->InputDataLength;
        pstDesc->pCtx           = pstParams->pCtx;
        //pstDesc->uiOutputMask   = pstParams->eDefaultOutput;
        pstDesc->iSpeed = 0xFFFF;
        //pstDesc->pPrivDataBuf   = pstParams->pPrivDataBuf;
        //pstDesc->uiPrivDataSize = pstParams->uiPrivDataSize;
        pstDesc->bIsStreamed    = pstParams->bIsStreamed;
        pstDesc->eStreamAudCodecType = pstParams->eStreamAudType;
        pstDesc->eStreamVidCodecType = pstParams->eStreamVidType;
        pstDesc->bIsPlaying     = false;
        //#if 1
        /* TEMP BUGFIX - THIS MODIFICATION SHOULD BE DONE LATER (BY GS) INSIDE  THE MODULE "stffmpeg.c" */
        //pstDesc->bGoingToBeStopped = FALSE;
        //#endif
        //#if 1  /* fix interlock-bug */
        //pstDesc->bGoingToBeStarted = FALSE;
        //#endif /* fix interlock-bug */

        /*if(pstParams->bIsStreamed == TRUE)
        {
        	mTBOX_TRACE((kTBOX_NIV_1, "Open:"
        				 "Stream video type (%s)\n",
        				 AVPm_ContentTypeToString(pstParams->eStreamVidType)));

        	switch(pstParams->eStreamVidType)
        	{
        	case eAVP_STREAM_CONTENT_H264:
        		pstDesc->eStreamVidCodecType = eAVPm_VID_CODEC_TYPE_H264;
        		break;
        	case eAVP_STREAM_CONTENT_MP2V:
        		pstDesc->eStreamVidCodecType = eAVPm_VID_CODEC_TYPE_MPEG;
        		break;
        	default:
        		pstDesc->eStreamVidCodecType = eAVPm_VID_CODEC_TYPE_NONE;
        		mTBOX_TRACE((kTBOX_NIV_WARNING, "Open:"
        					 "Stream video type (%s) unkown\n",
        					 AVPm_ContentTypeToString(pstParams->eStreamVidType)));
        		break;
        	}

        	mTBOX_TRACE((kTBOX_NIV_1, "Open:"
        				 "Stream audio type (%s)\n",
        				 AVPm_ContentTypeToString(pstParams->eStreamAudType)));

        	switch(pstParams->eStreamAudType)
        	{
        	case eAVP_STREAM_CONTENT_MP2A:
        		pstDesc->eStreamAudCodecType = eAVPm_AUD_CODEC_TYPE_MPEG2;
        		break;
        	case eAVP_STREAM_CONTENT_MP3A:
        		pstDesc->eStreamAudCodecType = eAVPm_AUD_CODEC_TYPE_MPEG3;
        		break;
        	case eAVP_STREAM_CONTENT_AAC:
        		pstDesc->eStreamAudCodecType = eAVPm_AUD_CODEC_TYPE_AAC;
        		break;
        	default:
        		pstDesc->eStreamAudCodecType = eAVPm_AUD_CODEC_TYPE_NONE;
        		mTBOX_TRACE((kTBOX_NIV_WARNING, "Open:"
        					 "Stream audio type (%s) unkown\n",
        					 AVPm_ContentTypeToString(pstParams->eStreamAudType)));
        		break;
        	}
        }
        else
        {
        	pstDesc->eStreamVidCodecType = eAVPm_VID_CODEC_TYPE_NONE;
        	pstDesc->eStreamAudCodecType = eAVPm_AUD_CODEC_TYPE_NONE;
        }*/

        /* store the desc in the list here
         * because needed in file open/read/seek callbacks
         */
        TDAL_MPi_DescList[ucDescIdx] = pstDesc;

        /* minimum basic caps for the moment */
        pstDesc->stCaps = eTDAL_MP_CAPS_BASIC;
    }

    if(result == eTDAL_MP_NO_ERROR)
    {
        pstDesc->bOpened = TRUE;
        /* descriptor pointer as handle */
        *pHandle = (tTDAL_MP_Handle)pstDesc;
    }
    else
    {
    	/* free all resources in case of error */
    	if(pstDesc != NULL)
    	{
            TDAL_Free(pstDesc);

            if((result != eTDAL_MP_NB_HANDLE_MAX_ERROR) &&
            (result != eTDAL_MP_BAD_PARAMETER_ERROR))
            {
                /* reset desc list */
                TDAL_MPi_DescList[ucDescIdx] = NULL;
            }
    	}
    }
    
    if (!TDAL_MPi_ReleasePool())
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Could not release memory pool\n"));
        mTBOX_RETURN(eTDAL_MP_NO_MEMORY_ERROR);
    }

    mTDAL_FS_MP_GetFileName(_u8FileName);
    memset(&sMplayerMeida, 0, sizeof(MPLAYER_MEDIA));
    
    MApi_AUDIO_SetSourceInfo(E_AUDIO_INFO_GAME_IN);
    MApi_AUDIO_SPDIF_SetMode(MSAPI_AUD_SPDIF_PCM);
    
    if(pFileOption == MPLAYER_FILE_MODE)
    {

        _pInputDataRead = pstDesc->pInputDataRead;
        _pInputDataSeek = pstDesc->pInputDataSeek;
        _pInputDataLength = pstDesc->pInputDataLength;
    
        //Check file type supported or not
        memset(_gu8FileExtName, 0, sizeof(_gu8FileExtName));
        if (pstParams->fileExtension != NULL)
        {
            memcpy(_gu8FileExtName, pstParams->fileExtension, 4*sizeof(char));
        }
        else
        {
            mTBOX_TRACE((kTBOX_NIV_1,"pstParams->fileExtension is NULL, parse from full filename\n"));
            MS_U8 *_ExtNamePtr = NULL;
            MS_BOOL _Ret = FALSE;
            _ExtNamePtr = _u8FileName + strlen((char *)_u8FileName) - 1;
            while(_ExtNamePtr > _u8FileName)
            {
                if(_ExtNamePtr[0] == '.')
                {
                    _ExtNamePtr++;
                    _Ret = TRUE;
                    break;
                }
                else if(_ExtNamePtr[0] == '/')
                {   // no extension name.
                    _Ret = FALSE;
                }
                _ExtNamePtr--;
            }
            if (_Ret)
            {
                strcpy(_gu8FileExtName, _ExtNamePtr);
            }
        }
        _gu8FileExtName[strlen(_gu8FileExtName)]=0;

        if (strlen(_gu8FileExtName) > 0)
        {
            MS_U32 i = 0;
            
            while(_gu8FileExtName[i])
            {
                //prevent the extension becomes upper case, and may cause the file can't be opened
                _gu8FileExtName[i] = toupper(_gu8FileExtName[i]);
                i++;
            }
            
            for (i = 0; i < sizeof(_gstZmmSupportExtTable) / sizeof(Z_File_Ext_Info); i++)
            {
                if (strcmp(&_gstZmmSupportExtTable[i].FileExt[0], (char *)_gu8FileExtName) == 0)
                {
                    _geMediaType = _gstZmmSupportExtTable[i].FileType;
                    break;
                }
            }
            mTBOX_TRACE((kTBOX_NIV_1, "Index=%d FileExtentionName = %s _geMediaType=%d\n", i, _gu8FileExtName, _geMediaType));
            if(i == sizeof(_gstZmmSupportExtTable) / sizeof(Z_File_Ext_Info))
            {
                mTBOX_TRACE((kTBOX_NIV_1, "extension name is not support yet i=%d _gstZmmSupportExtTable=%d Z_File_Ext_Info=%d\n", i, sizeof(_gstZmmSupportExtTable),sizeof(Z_File_Ext_Info)));
                _gbFileSupported = FALSE;
            }
        }
        else
        {
            _gbFileSupported = TRUE;
        }
        TDAL_MPm_SetFileIsSupport(_gbFileSupported);

        if (_geMediaType == E_MMSDK_MEDIA_TYPE_MUSIC && pstDesc->eStreamAudCodecType == eTDAL_MP_STREAM_CONTENT_PCM)
        {
            //=========================PCM========================
            TDAL_MPm_Player_SetOption(E_MMSDK_MEDIA_TYPE_MUSIC, MPLAYER_OPTION_SET_MUSIC_TYPE, MPLAYER_SUBTYPE_PCM);            
            MPLAYER_PCM_PARAM* pPCM_Param;
            pPCM_Param = (MPLAYER_PCM_PARAM*)AllocNoncachedMem(sizeof(MPLAYER_PCM_PARAM));
            pPCM_Param->eFormatTag = PCM_FORMAT_PCM;
            pPCM_Param->u16Channels = 2;
            pPCM_Param->u16BitsPerSample = 16;
            pPCM_Param->u32SamplesPerSec = 44100;
            pPCM_Param->bBigEndian = TRUE;
            TDAL_MPm_Player_SetOption(E_MMSDK_MEDIA_TYPE_MUSIC, MPLAYER_OPTION_MUSIC_SET_PCM_PARAM, (unsigned int)pPCM_Param);
            FreeNoncachedMem(pPCM_Param);            
        }

        //Force disable SLIDE SHOW for MVOP/GOP show photo behavior are consistent
        TDAL_MPm_Player_SetOption(E_MMSDK_MEDIA_TYPE_PHOTO, MPLAYER_OPTION_DISABLE_SLIDE_SHOW, TRUE);
        
        //set display window, {0, 0, 0, 0} means full panel
        sMplayerMeida.eFileMode = MPLAYER_FILE_MODE;
        sMplayerMeida.u32DispX = 0;
        sMplayerMeida.u32DispY = 0;
        sMplayerMeida.u32DispW = IPANEL(&gDevId ,Width);
        sMplayerMeida.u32DispH = IPANEL(&gDevId ,Height);
        sMplayerMeida.u32GotoTimeMs = 0;
#if DEMO_FILE
        sMplayerMeida.filename = (char*)_u8FileName;//utf8fn;
#else
        sMplayerMeida.filename = NULL;
#endif

        TDAL_MPm_Player_SetOption(E_MMSDK_MEDIA_TYPE_MOVIE,MPLAYER_OPTION_SET_DBG_LEVEL,E_MMSDK_DBG_LEVEL_NONE);

        if (!TDAL_MPm_Player_CreateMediaFile(_geMediaType, &sMplayerMeida))
        {
            result = eTDAL_MP_NOT_DONE_ERROR;
        }
        
    }
    else if ((pFileOption == MPLAYER_SEEKABLE_STREAM_MODE) || (pFileOption == MPLAYER_UNSEEKABLE_STREAM_MODE))
    {
    }
    if (result == eTDAL_MP_NO_ERROR)
    {
        pstDesc->bIsPlaying = TRUE;
    }
    
    TDAL_MPi_UnlockMutex();
    mTBOX_RETURN(result);
}

static char * getNotifyTypeAsText(EN_MPLAYER_NOTIFY_TYPE t)
{
#if 0 //wait porting
    switch(t)
    {
    case MPLAYER_EXIT_OK: //playback ok, and exit to ap
        return "MPLAYER_EXIT_OK";
    case MPLAYER_EXIT_FORCE:  //custom stop playback, and exit to ap
        return "MPLAYER_EXIT_FORCE";
    case MPLAYER_EXIT_ERROR_FILE: //playback  error, and exit to ap
        return "MPLAYER_EXIT_ERROR_FILE";
    case MPLAYER_SUFFICIENT_DATA: //when playing, data enough to continue play, and codec wil resume
        return "MPLAYER_SUFFICIENT_DATA";
    case MPLAYER_INSUFFICIENT_DATA: //when playing, run short of data, and codec wil pause
        return "MPLAYER_INSUFFICIENT_DATA";
    case MPLAYER_START_PLAY: //player init ok, and start playing
        return "MPLAYER_START_PLAY";
    case MPLAYER_PHOTO_DECODE_DONE:     /*!< \brief The photo decode process has finished successfully. */
        return "MPLAYER_PHOTO_DECODE_DONE";
    case MPLAYER_PHOTO_DECODE_FAILED:    /*!< \brief The photo decode process is failed. */
        return "MPLAYER_PHOTO_DECODE_FAILED";
    case MPLAYER_PHOTO_DECODING_NEXT:    /*!< \brief Next file is decoding and no decoded photo is ready for display. */
        return "MPLAYER_PHOTO_DECODING_NEXT";
    case MPLAYER_PHOTO_DISPLAYED:    /*!< \brief One decoded photo is now displayed. */
        return "MPLAYER_PHOTO_DISPLAYED";
    case MPLAYER_MOVIE_THUMBNAIL_DECODE_DONE:
        return "MPLAYER_MOVIE_THUMBNAIL_DECODE_DONE";
    case MPLAYER_MOVIE_THUMBNAIL_DECODE_FAIL:
        return "MPLAYER_MOVIE_THUMBNAIL_DECODE_FAIL";
    case MPLAYER_PROGRAM_CHANGED: // program has been changed.
        return "MPLAYER_PROGRAM_CHANGED";
    case MPLAYER_AUDIO_TRACK_CHANGED: // audio track has been changed.
        return "MPLAYER_AUDIO_TRACK_CHANGED";
    case MPLAYER_FRAME_READY:
        return "MPLAYER_FRAME_READY";
    case MPLAYER_BLACKSCREEN_OFF://george
        return "MPLAYER_BLACKSCREEN_OFF";
    case MPLAYER_SET_SUBTITLE_PALETTE:
        return "MPLAYER_SET_SUBTITLE_PALETTE";
    case MPLAYER_SUBTITLE_INIT:
        return "MPLAYER_SUBTITLE_INIT";
    case MPLAYER_SUBQUEUE_PUSH:
        return "MPLAYER_SUBQUEUE_PUSH";
    case MPLAYER_SUBTITLE_ENABLE:
        return "MPLAYER_SUBTITLE_ENABLE";
    case MPLAYER_SET_SUBTITLE_TRACK:
        return "MPLAYER_SET_SUBTITLE_TRACK";
    case MPLAYER_TS_SUBTITLE_PROGRAM_CHANGED:
        return "MPLAYER_TS_SUBTITLE_PROGRAM_CHANGED";
    case MPLAYER_SUBTITLE_DISPLAY:
        return "MPLAYER_SUBTITLE_DISPLAY";
    case MPLAYER_NOTIFY_NULL:
        return "MPLAYER_NOTIFY_NULL";
    default:
        return "MPLAYER_NOTIFY_UNKNOWN";
    }
#endif
    return "HAHATEST";
}

tTDAL_MP_Error TDAL_MP_Close(tTDAL_MP_Handle Handle) {
	tTDAL_MP_Error result = eTDAL_MP_NO_ERROR;
	mTBOX_FCT_ENTER("TDAL_MP_Close");

	tTDAL_MPm_Desc *pstDesc;
	int i;


	pstDesc = (tTDAL_MPm_Desc *) Handle;

	if(pstDesc == (tTDAL_MPm_Desc *)NULL)
	{
	    mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_MP_Close: bad handle\n"));
	    result = eTDAL_MP_BAD_PARAMETER_ERROR;
	}

	TDAL_MPi_LockMutex();

	i = TDAL_MPi_GetRunningMediaDescriptor();
        
	if (i >= 0)
	{
            if (TDAL_MPi_DescList[i] == pstDesc)
            {
                mTBOX_TRACE((kTBOX_NIV_1, "Index %d is still running\n",i));
                bool r = TDAL_MPm_Close();

                if (r)
                {
                    TDAL_MPi_UnlockMutex();
                    MsOS_DelayTask(100);
                    TDAL_MPi_LockMutex();
                    if (TDAL_MPi_DescList[i]->pEvtNotify)
                    {
                        TDAL_MPi_DescList[i]->pEvtNotify(Handle, eTDAL_MP_EVENT_STOPPED, TDAL_MPi_DescList[i]->pCtx);
                    }
                }
                else
                {
                    result = eTDAL_MP_NOT_DONE_ERROR;
                }
            }
	}

	/* free all resources */
    if(result == eTDAL_MP_NO_ERROR)
    {
		/* search for a desc in the list */
		for(i = 0; i < kTDAL_MPm_OPEN_SESSION_MAX; i++)
		{
			if(TDAL_MPi_DescList[i] == (tTDAL_MPm_Desc *)pstDesc)
			{
				TDAL_MPi_DescList[i] = (tTDAL_MPm_Desc *)NULL;
			}
		}

		pstDesc->bOpened = FALSE;

		TDAL_Free(pstDesc);
    }

    TDAL_MPi_UnlockMutex();

	mTBOX_RETURN(result);
}

tTDAL_MP_Error TDAL_MP_CapabilityGet(tTDAL_MP_Handle Handle, tTDAL_MP_Capabilities *pstCaps) {
	tTDAL_MP_Error result = eTDAL_MP_NO_ERROR;
	mTBOX_FCT_ENTER("TDAL_MP_CapabilityGet");
       *pstCaps = _MPCapabilities;
	mTBOX_RETURN(result);
}

tTDAL_MP_Error TDAL_MP_OutputSet(tTDAL_MP_Handle Handle, uint32_t uiOutputMask) {
	tTDAL_MP_Error result = eTDAL_MP_NO_ERROR;
	mTBOX_FCT_ENTER("TDAL_MP_OutputSet");
#if 0 //wait porting


	printf("TDAL_MP_OutputSet NOT IMPLEMENTED\n");
#endif
	mTBOX_RETURN(result);
}
tTDAL_MP_Error TDAL_MP_PictureGetMetadata(tTDAL_MP_Handle Handle, tTDAL_MP_MetaDataPict *pstMetadata)
{
    tTDAL_MP_Error result = eTDAL_MP_NO_ERROR;
    mTBOX_FCT_ENTER("TDAL_MP_PictureGetMetadata");

    tTDAL_MPm_Desc *pstDesc;
    int i;
    
    pstDesc = (tTDAL_MPm_Desc *) Handle;

    if(pstDesc == (tTDAL_MPm_Desc *)NULL)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_MP_PictureGetMetadata: bad handle\n"));
        result = eTDAL_MP_BAD_PARAMETER_ERROR;
    }

    TDAL_MPi_LockMutex();

    i = TDAL_MPi_GetRunningMediaDescriptor();

    if (i < 0 || pstDesc != TDAL_MPi_DescList[i])  
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "[TDAL_MP_PictureGetMetadata] No running MM or given handle not playing\n"));
        TDAL_MPi_UnlockMutex();
        mTBOX_RETURN(eTDAL_MP_NOT_DONE_ERROR); 
    }
    
    memset(pstMetadata,0,sizeof(tTDAL_MP_MetaDataPict));
    if (result == eTDAL_MP_NO_ERROR)
    {
        result = TDAL_MPm_PictureGetMetadata(pstMetadata);
    }
    
    TDAL_MPi_UnlockMutex();
    mTBOX_RETURN(result);
}
tTDAL_MP_Error TDAL_MP_AudioGetMetadata(tTDAL_MP_Handle Handle, tTDAL_MP_MetaDataAudio *pstMetadata)
{
    tTDAL_MP_Error result = eTDAL_MP_NO_ERROR;
    mTBOX_FCT_ENTER("TDAL_MP_AudioGetMetadata");

    tTDAL_MPm_Desc *pstDesc;
    int i;
    
    pstDesc = (tTDAL_MPm_Desc *) Handle;

    if(pstDesc == (tTDAL_MPm_Desc *)NULL)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_MP_AudioGetMetadata: bad handle\n"));
        result = eTDAL_MP_BAD_PARAMETER_ERROR;
    }

    TDAL_MPi_LockMutex();

    i = TDAL_MPi_GetRunningMediaDescriptor();

    if (i < 0 || pstDesc != TDAL_MPi_DescList[i])  
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "[TDAL_MP_AudioGetMetadata] No running MM or given handle not playing\n"));
        TDAL_MPi_UnlockMutex();
        mTBOX_RETURN(eTDAL_MP_NOT_DONE_ERROR); 
    }
    
    memset(pstMetadata,0,sizeof(tTDAL_MP_MetaDataAudio));
    if (result == eTDAL_MP_NO_ERROR)
    {
        result = TDAL_MPm_AudioGetMetadata(pstMetadata);
    }
    mTBOX_TRACE((kTBOX_NIV_1, "Audio Metadata Info\n"));
    mTBOX_TRACE((kTBOX_NIV_1, "stDuration=%lu\n",pstMetadata->stDuration));
    mTBOX_TRACE((kTBOX_NIV_1, "bIsSupported=%d\n",pstMetadata->bIsSupported));
    mTBOX_TRACE((kTBOX_NIV_1, "bIsID3Present=%d\n",pstMetadata->bIsID3Present));
    mTBOX_TRACE((kTBOX_NIV_1, "uiAudioDataStart = %lu\n",pstMetadata->uiAudioDataStart));
    mTBOX_TRACE((kTBOX_NIV_1, "uiAudioDataEnd = %lu\n",pstMetadata->uiAudioDataEnd));
    mTBOX_TRACE((kTBOX_NIV_1, "uiBitrate = %lu\n",pstMetadata->uiBitrate));
    mTBOX_TRACE((kTBOX_NIV_1, "uiFrameSize = %lu\n",pstMetadata->uiFrameSize));
    mTBOX_TRACE((kTBOX_NIV_1, "uiFrequency = %lu\n",pstMetadata->uiFrequency));
    mTBOX_TRACE((kTBOX_NIV_1, "uiSamplePerframe = %lu\n",pstMetadata->uiSamplePerframe));    
    TDAL_MPi_UnlockMutex();
    mTBOX_RETURN(result);
}
tTDAL_MP_Error TDAL_MP_AVGetMetadata(tTDAL_MP_Handle Handle, tTDAL_MP_MetaDataAv *pstMetadata)
{
    tTDAL_MP_Error result = eTDAL_MP_NO_ERROR;
    mTBOX_FCT_ENTER("TDAL_MP_AVGetMetadata");

    tTDAL_MPm_Desc *pstDesc;
    int i;
    
    pstDesc = (tTDAL_MPm_Desc *) Handle;

    if(pstDesc == (tTDAL_MPm_Desc *)NULL)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_MP_AVGetMetadata: bad handle\n"));
        result = eTDAL_MP_BAD_PARAMETER_ERROR;
    }

    TDAL_MPi_LockMutex();

    i = TDAL_MPi_GetRunningMediaDescriptor();

    if (i < 0 || pstDesc != TDAL_MPi_DescList[i])  
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "[TDAL_MP_AVGetMetadata] No running MM or given handle not playing\n"));
        TDAL_MPi_UnlockMutex();
        mTBOX_RETURN(eTDAL_MP_NOT_DONE_ERROR); 
    }
    
    memset(pstMetadata,0,sizeof(tTDAL_MP_MetaDataAv));
    if (result == eTDAL_MP_NO_ERROR)
    {
        result = TDAL_MPm_AVGetMetadata(pstMetadata);
    }
    mTBOX_TRACE((kTBOX_NIV_1, "AV Metadata Info\n"));
    mTBOX_TRACE((kTBOX_NIV_1, "stDuration=%lu\n",pstMetadata->stDuration));
    mTBOX_TRACE((kTBOX_NIV_1, "bIsVideoPresent=%lu\n",pstMetadata->bIsVideoPresent));
    mTBOX_TRACE((kTBOX_NIV_1, "bIsAudioPresent=%lu\n",pstMetadata->bIsAudioPresent));
    
    mTBOX_TRACE((kTBOX_NIV_1, "VContentType = %d\n",pstMetadata->Video.eType));
    mTBOX_TRACE((kTBOX_NIV_1, "Width = %lu\n",pstMetadata->Video.uiWidth));
    mTBOX_TRACE((kTBOX_NIV_1, "Height = %lu\n",pstMetadata->Video.uiHeight));
    mTBOX_TRACE((kTBOX_NIV_1, "iFramerate = %lu\n",pstMetadata->Video.iFramerate));
    mTBOX_TRACE((kTBOX_NIV_1, "iBitrate = %lu\n",pstMetadata->Video.iBitrate));
    mTBOX_TRACE((kTBOX_NIV_1, "bIsSupported = %lu\n",pstMetadata->Video.bIsSupported));
    
    mTBOX_TRACE((kTBOX_NIV_1, "AContentType = %d\n",pstMetadata->Audio.eType));
    mTBOX_TRACE((kTBOX_NIV_1, "iSamplerate = %d\n",pstMetadata->Audio.iSamplerate));
    mTBOX_TRACE((kTBOX_NIV_1, "iChannels = %d\n",pstMetadata->Audio.iChannels));
    mTBOX_TRACE((kTBOX_NIV_1, "iBitrate = %d\n",pstMetadata->Audio.iBitrate));
    mTBOX_TRACE((kTBOX_NIV_1, "bIsSupported = %d\n",pstMetadata->Audio.bIsSupported));
    TDAL_MPi_UnlockMutex();
    mTBOX_RETURN(result);
}

int TDAL_MPi_GetRunningMediaDescriptor(void)
{
    int i;

    for (i = 0; i < kTDAL_MPm_OPEN_SESSION_MAX; i++)
    {
        if (TDAL_MPi_DescList[i] != NULL)
        {
            if (TDAL_MPi_DescList[i]->bIsPlaying)
            {
                return i;
            }
        }
    }
    return -1;
}
MS_BOOL TDAL_MPi_CheckMediaDescriptor(tTDAL_MPm_Desc * desc)
{
    int i;

    for (i = 0; i < kTDAL_MPm_OPEN_SESSION_MAX; i++)
    {
        if (TDAL_MPi_DescList[i] != NULL)
        {
            if (TDAL_MPi_DescList[i] == desc)
            {
                return TRUE;
            }
        }
    }
    return FALSE;
}
LOCAL MS_BOOL _TDAL_MPm_MM_MemInit()
{
        PT_SYS_MmapInfo stMmapInfo={};
        ST_MMSDK_MMAP_INFO stMIUInfo={};
    
        //DRAM info
        stMIUInfo.u32Miu0MemSize  =  MIU_DRAM_LEN0;
#if (ENABLE_MIU_1 == 1)
        stMIUInfo.u32Miu1MemSize  =  MIU_DRAM_LEN1;
#endif
        stMIUInfo.u32MiuBoundary  =  MIU_INTERVAL;
        stMIUInfo.u32NMmapItems   =  sizeof(stMmapInfo)/sizeof(ST_MMSDK_BUF_INFO);
        stMIUInfo.u32TotalMemSize =  MIU_DRAM_LEN;
    
        //E_MMSDK_BUF_PHOTO_DISPLAY (need to check)
        MM_FILL_MEM_INFO(stMmapInfo.stPhotoDispBufAddr, PHOTO_DISPLAY_ADR, PHOTO_DISPLAY_LEN, PHOTO_DISPLAY_MEMORY_TYPE);
    
        //E_MMSDK_BUF_PHOTO_DISPLAY_MPO
        //currenty doesn't support 3D display .mpo file, no need for this buffer
        //MM_FILL_MEM_INFO(stMmapInfo.stPhotoMPODispBufAddr, PHOTO_MPO_DISPLAY_ADR, PHOTO_MPO_DISPLAY_LEN, PHOTO_MPO_DISPLAY_MEMORY_TYPE);
    
        //E_MMSDK_BUF_PHOTO_INTERNAL
        MM_FILL_MEM_INFO(stMmapInfo.stJPDInterBufAddr, JPD_INTER_BUF_ADR, JPD_INTER_BUF_LEN, JPD_INTER_BUF_MEMORY_TYPE);
    
        //E_MMSDK_BUF_PHOTO_MEMORY_POOL
        MM_FILL_MEM_INFO(stMmapInfo.stPhotoMemPoolAddr, JPD_INTER_BUF_ADR, MMSDK_BUF_PHOTO_MEMORY_POOL_LEN, JPD_INTER_BUF_MEMORY_TYPE);
    
        //E_MMSDK_BUF_PHOTO_OUTPUT
        //MM_FILL_MEM_INFO(stMmapInfo.stJPDOutBufAddr, JPD_OUT_ADR, JPD_OUT_LEN, JPD_OUT_MEMORY_TYPE);
        MM_FILL_MEM_INFO( stMmapInfo.stJPDOutBufAddr, VDEC_FRAME_BUF_ADR, VDEC_FRAME_BUF_LEN, VDEC_FRAME_BUF_MEMORY_TYPE);
        
        //E_MMSDK_BUF_PHOTO_PROGRESSIVE
        MM_FILL_MEM_INFO(stMmapInfo.stPhotoProgAddr, JPD_INTER_BUF_ADR, JPD_INTER_BUF_LEN, JPD_INTER_BUF_MEMORY_TYPE);
    
        //E_MMSDK_BUF_PHOTO_READ
        MM_FILL_MEM_INFO(stMmapInfo.stJPDReadBufAddr, JPD_READ_BUF_ADR, JPD_READ_BUF_LEN, JPD_READ_BUF_MEMORY_TYPE);
    
        //E_MMSDK_BUF_PHOTO_SHAREMEMORY
        MM_FILL_MEM_INFO(stMmapInfo.stPhotoShareMemAddr, PHOTO_SHARE_MEM_ADR, PHOTO_SHARE_MEM_LEN, PHOTO_SHARE_MEM_MEMORY_TYPE);
    
        //E_MMSDK_BUF_VDPLAYER_AEON_DATA
        MM_FILL_MEM_INFO(stMmapInfo.stVdplayerDataAddr, VDPLAYER_DATA_ADR, VDPLAYER_DATA_LEN, VDPLAYER_DATA_MEMORY_TYPE);
    
        //E_MMSDK_BUF_VDPLAYER_BITSTREAM_AUDIO
#ifdef VDPLAYER_BS_EXT_AVAILABLE
        MM_FILL_MEM_INFO(stMmapInfo.stVdplayerBSExtAddr, VDPLAYER_BS_EXT_ADR, VDPLAYER_BS_EXT_LEN, VDPLAYER_BS_EXT_MEMORY_TYPE);
#else
        MS_S32 s32MstarNonCachedPoolID=INVALID_POOL_ID;
        Get_NonCachePoolID(&s32MstarNonCachedPoolID);
    
        _gpBSAudBufVA = (void *)AllocNoncachedMem(MMSDK_BUF_VDPLAYER_BITSTREAM_AUDIO + BITSTREAM_AUDIO_BUFFER_ALIGNMENT);
        if(!_gpBSAudBufVA)
            mTBOX_TRACE((kTBOX_NIV_WARNING,"ERROR!!! No enough free mem for E_MMSDK_BUF_VDPLAYER_BITSTREAM_AUDIO!\n"));
    
        MS_PHY PhyBSAudBufPA = MsOS_VA2PA((MS_VIRT)MEMALIGN(BITSTREAM_AUDIO_BUFFER_ALIGNMENT, (MS_VIRT)_gpBSAudBufVA));
    
        MS_U32 u32BSAudioMemType = 0;
        if(PhyBSAudBufPA & MIU_INTERVAL)
            u32BSAudioMemType |= MIU1;
        else
            u32BSAudioMemType |= MIU0;
    
        MM_FILL_MEM_INFO(stMmapInfo.stVdplayerBSExtAddr, PhyBSAudBufPA, MMSDK_BUF_VDPLAYER_BITSTREAM_AUDIO, u32BSAudioMemType);
#endif
    
        //E_MMSDK_BUF_VDPLAYER_BITSTREAM_SUBTITLE
        MM_FILL_MEM_INFO(stMmapInfo.stVdplayerSubBSAddr, VDPLAYER_SUB_BS_ADR, VDPLAYER_SUB_BS_LEN, VDPLAYER_SUB_BS_MEMORY_TYPE);
    
        //E_MMSDK_BUF_VDPLAYER_BITSTREAM_VIDEO
        MM_FILL_MEM_INFO(stMmapInfo.stVdplayerBSAddr, VDPLAYER_BS_ADR, VDPLAYER_BS_LEN, VDPLAYER_BS_MEMORY_TYPE);
    
        //E_MMSDK_BUF_VDPLAYER_SHAREMEMORY: /* needed size >= 4K*/
        MM_FILL_MEM_INFO(stMmapInfo.stVdplayerShmMemAddr, VDPLAYER_SHARE_MEM_ADR, VDPLAYER_SHARE_MEM_LEN, VDPLAYER_SHARE_MEM_MEMORY_TYPE);
    
        //E_MMSDK_BUF_SUBTITLE_PUSH
        MM_FILL_MEM_INFO(stMmapInfo.stVdplayerSubAddr, VDPLAYER_SUB_ADR, VDPLAYER_SUB_LEN, VDPLAYER_SUB_MEMORY_TYPE);
    
        //E_MMSDK_BUF_DEMUX_SEC
        MM_FILL_MEM_INFO(stMmapInfo.stTspFWAddr, TSP_FW_BUF_ADR, TSP_FW_BUF_LEN, TSP_FW_BUF_MEMORY_TYPE);
    
        //E_MMSDK_BUF_MAD_DECODE

        MM_FILL_MEM_INFO(stMmapInfo.stMADDecBufAddr, MAD_ADV_BUF_ADR, MAD_ADV_BUF_LEN, MAD_ADV_BUF_MEMORY_TYPE);
    
        //E_MMSDK_BUF_VDEC_CPU
        MM_FILL_MEM_INFO(stMmapInfo.stVdecAeonAddr, VDEC_AEON_ADR, VDEC_AEON_LEN, VDEC_AEON_MEMORY_TYPE);
    
        //E_MMSDK_BUF_VDEC_FRAMEBUFFER
        MM_FILL_MEM_INFO(stMmapInfo.stVdecFrameBufAddr, VDEC_FRAME_BUF_ADR, VDEC_FRAME_BUF_LEN, VDEC_FRAME_BUF_MEMORY_TYPE);

        //E_MMSDK_BUF_VDEC_FRAMEBUFFER_SD
        MM_FILL_MEM_INFO(stMmapInfo.stVdecFrameBufferSDAddr, VDEC_FRAME_BUF_ADR, VDEC_FRAME_BUF_LEN, VDEC_FRAME_BUF_MEMORY_TYPE);
    
        //E_MMSDK_BUF_VDEC_STREAMBUFFER
        MM_FILL_MEM_INFO(stMmapInfo.stVdecBitStreamAddr, VDEC_BIT_STREAM_ADR, VDEC_BIT_STREAM_LEN, VDEC_BIT_STREAM_MEMORY_TYPE);
    
        //E_MMSDK_BUF_XC_MAIN_FB
        MM_FILL_MEM_INFO(stMmapInfo.stXCMainFrameBufAddr, SC0_MAIN_FB_ADR, SC0_MAIN_FB_LEN, SC0_MAIN_FB_MEMORY_TYPE);
    
        //E_MMSDK_BUF_PHY_ADDR_FOR_COPY_DATA (need to check)
        MM_FILL_MEM_INFO(stMmapInfo.stPHYAddrForCopyDataAddr, PHY_COPY_DATA_ADR, PHY_COPY_DATA_LEN, PHY_COPY_DATA_MEMORY_TYPE);
    
        PT_SYS_SetMmapInfo(&stMmapInfo, &stMIUInfo);
    
        return TRUE;

}

// true if TDAL_MP is initialized
MS_BOOL is_TDAL_MP_Initialized(void)
{
	return b_MM_Initied;
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

void TDAL_MPi_LockMutex()
{
#ifdef PROTECT_TDAL_MP
    TDAL_LockMutex(TDAL_MPi_Mutex);
    //TDAL_MPi_MutexThreadLocked = TDAL_GetRunningThreadSystemId();
#endif
}

void TDAL_MPi_UnlockMutex()
{
#ifdef PROTECT_TDAL_MP
    //TDAL_MPi_MutexThreadLocked = NULL;
    TDAL_UnlockMutex(TDAL_MPi_Mutex);
#endif
}

bool TDAL_MPi_LockMutexIfDifferentThread()
{
#if 0
    if (TDAL_MPi_MutexThreadLocked != TDAL_GetRunningThreadSystemId())
    {
        TDAL_MPi_LockMutex();
        return true;
    }
#endif
    return false;
}
