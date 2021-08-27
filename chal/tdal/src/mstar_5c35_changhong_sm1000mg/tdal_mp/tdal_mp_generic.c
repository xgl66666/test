/*
 * tdal_mp_generic.c
 *
 *  Created on: 4 Feb 2013
 *      Author: mijovic
 */

/********************************************************/
/*              Includes                                */
/********************************************************/

#include "tdal_mp.h"

#include "tdal_disp.h"

#include "tbox.h"
#include "tdal_common.h"
#include "tdal_common_priv.h"
#include "MsCommon.h"
#include "MsTypes2.h"
#include "MsTypes.h"

#include "msAPI_MM.h"
#include "apiHDMITx.h"
#include "drvXC_IOPort.h"
#include "apiXC.h"
#include "drvMVOP.h"
#include "apiVDEC.h"
#include "apiVDEC_EX.h"
#include "msAPI_XC.h"
#include "tdal_mp_p.h"

#include "crules.h"

#include "drvAUDIO.h"
#include "drvMVOP.h"

#include "apiDMX.h"

#include "apiAUDIO.h"
#include "apiDAC.h"

#include "MsCommon.h"
#include "MsMemory.h"

#include "tdal_disp.h"
#include "tdal_disp_module_priv.h"

/********************************************************/
/*              Defines                                 */
/********************************************************/
#define MM_REMOVE_TXT_SUBTITLE 1
#define kTDAL_MP_REVISION "TDAL_MP_v0.0"
#define MM_COPROCESSOR_ADDR                     0x00000000  /* Alignment 0 */
#define MM_COPROCESSOR_LEN                      0x00200000 /* 2M */
#define MM_COPROCESSOR_TYPE                     (MIU0)
//#define PROTECT_TDAL_MP

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
    EN_MPLAYER_COMMAND_TYPE eCmd;
    unsigned int  u32Param;
    unsigned int  u32Info;
    bool shouldExit;
} tTDAL_MPi_CallbackMsg;

/********************************************************/
/*              Local Module Variables (MODULE)         */
/********************************************************/

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
LOCAL ST_CPCODEC_MSG_SUBTITLE_INIT_INFO _prestSubtitleInit;
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

/********************************************************/
/*              Local Functions Declarations (LOCAL)    */
/********************************************************/
LOCAL void _TDAL_MPm_Mplayer_Movie_CallBack(EN_MPLAYER_COMMAND_TYPE eCmd, unsigned int  u32Param, unsigned int  u32Info);
LOCAL void _TDAL_MPm_Mplayer_Photo_CallBack(EN_MPLAYER_COMMAND_TYPE eCmd, unsigned int  u32Param, unsigned int  u32Info);
LOCAL void _TDAL_MPm_Mplayer_Music_CallBack(EN_MPLAYER_COMMAND_TYPE eCmd, unsigned int  u32Param, unsigned int  u32Info);
LOCAL MS_BOOL _TDAL_MPm_MM_MemInit(MM_MEM_INFO *pMem_Info, MM_MEMMAP_INFO *pMemMap_Info);
LOCAL void _SubInitHandler(unsigned int  u32Info);
LOCAL void _SubQueuePushHandler(unsigned int  u32Info);
LOCAL void _SubtitleDisplayHandler(unsigned int  u32Info);
LOCAL void TDAL_MPi_CallbackTask(void* arg);

#if defined(SDEC_GLUE_JPEG_TDAL_MP_PARTITION)

LOCAL MS_S32     TDAL_MPi_MemPart = -1;
LOCAL uint32_t   TDAL_MPi_AllocCount = 0;

tTDAL_MP_Error TDAL_MP_Malloc(uint32_t size, void ** p)
{
    tTDAL_MP_Error err;

    mTBOX_FCT_ENTER("TDAL_MP_Malloc");

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

    mTBOX_RETURN(err);
}

tTDAL_MP_Error TDAL_MP_Free(void * p)
{
    tTDAL_MP_Error err;

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

    return err;
}

uint32_t TDAL_MP_GetAvailableSpace()
{
    uint32_t ret = 0;

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

    return ret;
}

bool TDAL_MPi_ReleasePool(void)
{
    MS_BOOL bRet;
    bool retValue = true;

    mTBOX_FCT_ENTER("TDAL_MPi_ReleasePool");

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

	MS_BOOL B_Ret;
    MM_MEM_INFO MM_MemInfo;
    AUDIO_INFO MM_AudioInfo;
    MM_TSP_INFO MM_TSPInfo;
    void * ptmpMem = NULL;

    mTBOX_FCT_ENTER("TDAL_MP_Init");

    if(b_MM_Initied)
    {
    	mTBOX_TRACE((kTBOX_NIV_WARNING, "TDAL_MP_Init already initialized\n"));
        mTBOX_RETURN(eTDAL_MP_NOT_DONE_ERROR);
    }

    MM_MemInfo.u32Miu0_mem_size = MIU_DRAM_LEN0;
    MM_MemInfo.u32Miu1_mem_size = ENABLE_MIU_1;
    MM_MemInfo.u32Miu_boundary = MIU_INTERVAL;
    MM_MemInfo.u32Mmap_items = 60;
    MM_MemInfo.u32Total_mem_size = MM_MemInfo.u32Miu0_mem_size + MM_MemInfo.u32Miu1_mem_size;
    ptmpMem = MsOS_AllocateMemory(sizeof(MM_MEMMAP_INFO)* MM_MemInfo.u32Mmap_items,gs32NonCachedPoolID);
    if(!ptmpMem)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_MP_Init could not allocate memory for MM_MEMMAP_INFO\n"));
        mTBOX_RETURN(eTDAL_MP_NO_MEMORY_ERROR);
    }
    memset(ptmpMem, 0x00, sizeof(MM_MEMMAP_INFO)*MM_MemInfo.u32Mmap_items);

    _TDAL_MPm_MM_MemInit(&MM_MemInfo, (MM_MEMMAP_INFO *)ptmpMem);

    MM_AudioInfo.u32AudioSrc= AUDIO_SOURCE_DTV;
    MM_AudioInfo.u32AudioPath= AUDIO_PATH_MAIN_SPEAKER;
    MM_AudioInfo.u32AudioOutput = AUDIO_OUTPUT_MAIN_SPEAKER;

    MM_TSPInfo.pAddr = u8TSPFirmwareForRTSP;
    MM_TSPInfo.u32Size = sizeof(u8TSPFirmwareForRTSP);

    if(MApi_DMX_ChkAlive() != DMX_FILTER_STATUS_OK)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_MP_Init: DMX needs to be initialized before MM\n"));
        mTBOX_RETURN(eTDAL_MP_NOT_DONE_ERROR);
    }
    // TODO: DemoAV_TSP_StopDmxFlt();///stop live flt
#if MSTAR_QUEUE
    TDAL_MPi_CallbackQueue = MsOS_CreateQueue(NULL, //It is useless now, can pass NULL.
            0,   // queue size (byte unit) : now fixed as 10 * u32MessageSize
            E_MSG_FIXED_SIZE,   // E_MSG_VAR_SIZE has not been supported yet
            sizeof(tTDAL_MPi_CallbackMsg), // message size (byte unit) for E_MSG_FIXED_SIZE
            E_MSOS_FIFO,        // E_MSOS_FIFO suspended in FIFO order
            "TDAL_MP_Queue");      // queue name)
#else
    TDAL_CreateQueue(TDAL_QUEUE_MSG_MAX_NO, sizeof(tTDAL_MPi_CallbackMsg), &TDAL_MPi_CallbackQueue);
#endif
    if (TDAL_MPi_CallbackQueue == -1)
    {
        mTBOX_TRACE((kTBOX_NIV_1, "[TDAL_MP_Init]: Could not create TDAL_MP queue\n"));
        result = eTDAL_MP_NOT_DONE_ERROR;
    }

    if (result == eTDAL_MP_NO_ERROR)
    {
        MApp_MPlayer_SetMemPool(gs32CachedPoolID, 1);
        MApp_MPlayer_SetMemPool(gs32NonCachedPoolID, 0);
        MApp_MPlayer_SetCustomerInfo(Customer_info,49,Customer_hash,16);
        B_Ret=MApi_MPlayer_Initialize(&MM_AudioInfo,&MM_TSPInfo,(void*)&MM_MemInfo, ptmpMem);
        if(B_Ret)
        {
            MApi_MPlayer_RegisterCallBack(E_MPLAYER_MEDIA_MOVIE, _TDAL_MPm_Mplayer_Movie_CallBack);
            MApi_MPlayer_RegisterCallBack(E_MPLAYER_MEDIA_PHOTO, _TDAL_MPm_Mplayer_Photo_CallBack);
            MApi_MPlayer_RegisterCallBack(E_MPLAYER_MEDIA_MUSIC, _TDAL_MPm_Mplayer_Music_CallBack);
        }
        else
        {
            mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_MP_Init: Could not initialize MM\n"));
            result = eTDAL_MP_NOT_DONE_ERROR;
        }
    }

    if (result == eTDAL_MP_NO_ERROR)
    {
        TDAL_CreateMutex(&TDAL_MPi_Mutex);
        mTBOX_ASSERT(TDAL_MPi_Mutex != NULL);
    }

    if (result == eTDAL_MP_NO_ERROR)
    {
        TDAL_MPi_CallbackTaskId = TDAL_CreateTask(
                eTDAL_PRIORITY_NORMAL,
                "TDAL MP Task",
                TDAL_MPi_CallbackTaskStack,
                sizeof(TDAL_MPi_CallbackTaskStack),
                TDAL_MPi_CallbackTask,
                NULL);

        if (TDAL_MPi_CallbackTaskId == NULL)
        {
            mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_MP_Init: Could not initialize MP callback task\n"));
            result = eTDAL_MP_NOT_DONE_ERROR;
        }

    }

    if (MsOS_FreeMemory(ptmpMem, gs32NonCachedPoolID))
    {
        ptmpMem = NULL;
    }
    else
    {
        mTBOX_ASSERTm(FALSE, "free(ptmpMem) should always sucedd\n");
    }

    if (result == eTDAL_MP_NO_ERROR)
    {
        b_MM_Initied = TRUE;
    }
    else
    {
        b_MM_Initied = FALSE;

        MApi_MPlayer_Finalize();
        MApi_MPlayer_RegisterCallBack(E_MPLAYER_MEDIA_MOVIE, NULL);
        MApi_MPlayer_RegisterCallBack(E_MPLAYER_MEDIA_PHOTO, NULL);
        MApi_MPlayer_RegisterCallBack(E_MPLAYER_MEDIA_MUSIC, NULL);

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
    }

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

	mTBOX_FCT_ENTER("TDAL_MP_Terminate");

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

    TDAL_MPi_UnlockMutex();

	mTBOX_RETURN(result);
}

static char * getNotifyTypeAsText(EN_MPLAYER_NOTIFY_TYPE t)
{
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
}

tTDAL_MP_Error TDAL_MP_Close(tTDAL_MP_Handle Handle) {
	tTDAL_MP_Error result = eTDAL_MP_NO_ERROR;
	tTDAL_MPm_Desc *pstDesc;
	int i;

	mTBOX_FCT_ENTER("TDAL_MP_Close");

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
            bool r = TDAL_MPm_Stop();

            if (r)
            {
                TDAL_MPi_UnlockMutex();
                MsOS_DelayTask(1000);
                TDAL_MPi_LockMutex();
                TDAL_MPi_DescList[i]->pEvtNotify(Handle, eTDAL_MP_EVENT_STOPPED, TDAL_MPi_DescList[i]->pCtx);
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

	printf("TDAL_MP_CapabilityGet NOT IMPLEMENTED\n");

	mTBOX_RETURN(result);
}

tTDAL_MP_Error TDAL_MP_OutputSet(tTDAL_MP_Handle Handle, uint32_t uiOutputMask) {
	tTDAL_MP_Error result = eTDAL_MP_NO_ERROR;

	mTBOX_FCT_ENTER("TDAL_MP_OutputSet");

	printf("TDAL_MP_OutputSet NOT IMPLEMENTED\n");

	mTBOX_RETURN(result);
}

LOCAL void _TDAL_MPm_Mplayer_Movie_CallBack(EN_MPLAYER_COMMAND_TYPE eCmd, unsigned int  u32Param, unsigned int  u32Info)
{
    tTDAL_MPi_CallbackMsg msg;
    MS_BOOL bRet;

    mTBOX_FCT_ENTER("_TDAL_MPm_Mplayer_Movie_CallBack");

    mTBOX_TRACE((kTBOX_NIV_1, "_TDAL_MPm_Mplayer_Movie_CallBack: cmd = %d, param = %s, info = %d\n", eCmd, getNotifyTypeAsText(u32Param), u32Info));

    if (TDAL_MPi_CallbackQueue != -1)
    {
        msg.shouldExit = false;
        msg.eCmd = eCmd;
        msg.callbackType = eTDAL_MP_CallbackMovie;
        msg.u32Info = u32Info;
        msg.u32Param = u32Param;

#if MSTAR_QUEUE
        bRet = MsOS_SendToQueue(TDAL_MPi_CallbackQueue, (MS_U8 *) &msg, sizeof(msg), 0);
#else
        bRet = TDAL_Enqueue(TDAL_MPi_CallbackQueue, (MS_U8 *) &msg);
#endif
        if (bRet == FALSE)
        {
            mTBOX_TRACE((kTBOX_NIV_CRITICAL, "[_TDAL_MPm_Mplayer_Movie_CallBack] Could not send event to queue\n"));
        }
    }
}

LOCAL void _SubInitHandler(unsigned int  u32Info)
{
    mTBOX_FCT_ENTER("_SubInitHandler()\n");
    ST_CPCODEC_MSG_SUBTITLE_INIT_INFO *stSubtitle = (ST_CPCODEC_MSG_SUBTITLE_INIT_INFO *)u32Info;

    if(stSubtitle->bInit && !b_InternalSubtitleInitialize)
    {
        VDEC_DispInfo   info = {0};
        MApi_VDEC_GetDispInfo(&info);
        if((info.u16HorSize== 0) || (info.u16VerSize == 0))
        {
            printf("Post to Check\n");
            return;
        }
        // to prevent from UI doesn't close subtitle correctly
        MApi_MPlayer_Close();
        MApi_MPlayer_Disconnect();


        SUBTITLE_IRenderTarget m_RenderTarge =
        {
            MM_SubRender_Create,
            MM_SubRender_GetVideoSizeFromVdec,
            MM_SubRender_Open,
            MM_SubRender_Close,
            MM_SubRender_CreateWindow,
            MM_SubRender_DestroyWindow,
            MM_SubRender_SetPalette,
            MM_SubRender_Bitblt,
            MM_SubRender_ClearWindowByIndex,
            MM_SubRender_ClearWindowByColor,
            MM_SubRender_GetWindowInfo,
            MM_SubRender_Lock,
            MM_SubRender_UnLock,
            MM_SubRender_RenderFont,
            MM_SubRender_Show,
            MM_SubRender_Unshow
        };
        switch(stSubtitle->eType)
        {
            case E_CPCODEC_INTERNAL_SUBTITLE_TYPE_TEXT:
                printf("Subtitle Type: TEXT\n");
                MApi_MPlayer_Set_Subtitle_Type(E_SUBTITLE_TYPE_TTF);
                break;
            case E_CPCODEC_INTERNAL_SUBTITLE_TYPE_TS_PGS:
                printf("Subtitle Type: ts-pgs\n");
                MApi_MPlayer_Set_Subtitle_Type(E_SUBTITLE_TYPE_TS);
                break;
            case E_CPCODEC_INTERNAL_SUBTITLE_TYPE_TS_BMP:
                printf("Subtitle Type: ts-bmp(dvb-subtitle)\n");
                MApi_MPlayer_Set_Subtitle_Type(E_SUBTITLE_TYPE_TS);
                break;
            case E_CPCODEC_INTERNAL_SUBTITLE_TYPE_IMAGE_BMP:
                printf("Subtitle Type: image-bmp\n");
                MApi_MPlayer_Set_Subtitle_Type(E_SUBTITLE_TYPE_BMP);
                break;
            case E_CPCODEC_INTERNAL_SUBTITLE_TYPE_IMAGE_VOBSUB:
                printf("Subtitle Type: vob-sub\n");
                MApi_MPlayer_Set_Subtitle_Type(E_SUBTITLE_TYPE_VOBSUB);
                break;
            default:
                printf("Subtitle Type: unknown\n");
                MApi_MPlayer_Set_Subtitle_Type(E_SUBTITLE_TYPE_UNKNOWN);
                break;
        }


        switch(stSubtitle->eType)
        {
            case E_CPCODEC_INTERNAL_SUBTITLE_TYPE_IMAGE_BMP:
            case E_CPCODEC_INTERNAL_SUBTITLE_TYPE_IMAGE_VOBSUB:
                //use default dimension
                //MApi_MPlayer_Set_Subtitle_Dimension(stSubtitle->u16BmpWidth, stSubtitle->u16BmpHeight);
                MApi_MPlayer_Connect(&m_RenderTarge);
                MApi_MPlayer_Open();
                break;
            case E_CPCODEC_INTERNAL_SUBTITLE_TYPE_TS_PGS:
            case E_CPCODEC_INTERNAL_SUBTITLE_TYPE_TEXT:
            case E_CPCODEC_INTERNAL_SUBTITLE_TYPE_TS_BMP:
                MApi_MPlayer_Connect(&m_RenderTarge);
                MApi_MPlayer_Open();
                break;
            default:
                break;
        }

        b_InternalSubtitleInitialize = TRUE;
    }
    //De-INIT
    else if(!stSubtitle->bInit && b_InternalSubtitleInitialize)
    {
        MApi_MPlayer_Close();
        MApi_MPlayer_Disconnect();
        b_InternalSubtitleInitialize = FALSE;
    }
    else {
        // mm_print("Subtitle init or de-init fail!!\n");
    }
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

LOCAL void _TDAL_MPm_Mplayer_Photo_CallBack(EN_MPLAYER_COMMAND_TYPE eCmd, unsigned int  u32Param, unsigned int  u32Info)
{
    tTDAL_MPi_CallbackMsg msg;
    MS_BOOL bRet;

    mTBOX_FCT_ENTER("_TDAL_MPm_Mplayer_Photo_CallBack");

    if (TDAL_MPi_CallbackQueue != -1)
    {
        msg.shouldExit = false;
        msg.eCmd = eCmd;
        msg.callbackType = eTDAL_MP_CallbackPhoto;
        msg.u32Info = u32Info;
        msg.u32Param = u32Param;

#if MSTAR_QUEUE
        bRet = MsOS_SendToQueue(TDAL_MPi_CallbackQueue, (MS_U8 *) &msg, sizeof(msg), 0);
#else        
        bRet = TDAL_Enqueue(TDAL_MPi_CallbackQueue, &msg);
#endif
        if (bRet == FALSE)
        {
            mTBOX_TRACE((kTBOX_NIV_CRITICAL, "[_TDAL_MPm_Mplayer_Photo_CallBack] Could not send event to queue\n"));
        }
    }

    mTBOX_RETURN;
}

LOCAL void _TDAL_MPm_Mplayer_Music_CallBack(EN_MPLAYER_COMMAND_TYPE eCmd, unsigned int  u32Param, unsigned int  u32Info)
{
    tTDAL_MPi_CallbackMsg msg;
    MS_BOOL bRet;

    mTBOX_FCT_ENTER("_TDAL_MPm_Mplayer_Music_CallBack");

    if (TDAL_MPi_CallbackQueue != -1)
    {
        msg.shouldExit = false;
        msg.eCmd = eCmd;
        msg.callbackType = eTDAL_MP_CallbackMusic;
        msg.u32Info = u32Info;
        msg.u32Param = u32Param;

#if MSTAR_QUEUE
        bRet = MsOS_SendToQueue(TDAL_MPi_CallbackQueue, (MS_U8 *) &msg, sizeof(msg), 0);
#else
        bRet = TDAL_Enqueue(TDAL_MPi_CallbackQueue, &msg);
#endif
        if (bRet == FALSE)
        {
            mTBOX_TRACE((kTBOX_NIV_CRITICAL, "[_TDAL_MPm_Mplayer_Music_CallBack] Could not send event to queue\n"));
        }
    }

    mTBOX_RETURN;
}

LOCAL MS_BOOL _TDAL_MPm_MM_MemInit(MM_MEM_INFO *pMem_Info, MM_MEMMAP_INFO *pMemMap_Info)
{
    MS_U32 i;
    if(pMemMap_Info == NULL)
        return FALSE;

    for(i = 0; i < pMem_Info->u32Mmap_items; i++)
    {
        if(i == E_MMAP_ID_VDEC_CPU)
        {
            (pMemMap_Info+i)->u32ID = E_MMAP_ID_VDEC_CPU;
            (pMemMap_Info+i)->u32Addr = VDEC_AEON_ADR;
            (pMemMap_Info+i)->u32Size = VDEC_AEON_LEN;
            (pMemMap_Info+i)->u8Layer = 0;
            (pMemMap_Info+i)->u32Align = 0x00000010;
            (pMemMap_Info+i)->bIs_miu0 = (VDEC_AEON_MEMORY_TYPE & 0x01) ? 0:1;

        }
        else if(i == E_MMAP_ID_VDEC_FRAMEBUFFER)
        {
            (pMemMap_Info+i)->u32ID = E_MMAP_ID_VDEC_FRAMEBUFFER;
            (pMemMap_Info+i)->u32Addr = VDEC_FRAME_BUF_ADR;
            (pMemMap_Info+i)->u32Size = VDEC_FRAME_BUF_LEN;
            (pMemMap_Info+i)->u8Layer = 0;
            (pMemMap_Info+i)->u32Align = 0x00000200;
            (pMemMap_Info+i)->bIs_miu0 = (VDEC_FRAME_BUF_MEMORY_TYPE & 0x01) ? 0:1;

        }
        else if(i == E_MMAP_ID_VDEC_BITSTREAM)
        {
            (pMemMap_Info+i)->u32ID = E_MMAP_ID_VDEC_BITSTREAM;
            (pMemMap_Info+i)->u32Addr = VDEC_BIT_STREAM_ADR;
            (pMemMap_Info+i)->u32Size = VDEC_BIT_STREAM_LEN;
            (pMemMap_Info+i)->u8Layer = 0;
            (pMemMap_Info+i)->u32Align = 0x00000008;
            (pMemMap_Info+i)->bIs_miu0 = (VDEC_BIT_STREAM_MEMORY_TYPE & 0x01) ? 0:1;

        }
        else if(i == E_MMAP_ID_XC_MAIN_FB)
        {
            (pMemMap_Info+i)->u32ID = E_MMAP_ID_XC_MAIN_FB;
            (pMemMap_Info+i)->u32Addr = SC0_MAIN_FB_ADR;
            (pMemMap_Info+i)->u32Size = SC0_MAIN_FB_LEN;
            (pMemMap_Info+i)->u8Layer = 0;
            (pMemMap_Info+i)->u32Align = 0x00000010;
            (pMemMap_Info+i)->bIs_miu0 = (SC0_MAIN_FB_MEMORY_TYPE & 0x01) ? 0:1;;

        }
        else if(i == E_MMAP_ID_VE)
        {
            (pMemMap_Info+i)->u32ID = E_MMAP_ID_VE;
            (pMemMap_Info+i)->u32Addr = VE_FRAME_BUF_ADR;
            (pMemMap_Info+i)->u32Size = VE_FRAME_BUF_LEN;
            (pMemMap_Info+i)->u8Layer = 0;
            (pMemMap_Info+i)->u32Align = 0x00000008;
            (pMemMap_Info+i)->bIs_miu0 = 1;

        }
        else if(i == E_MMAP_ID_MAD_DEC)
        {
            (pMemMap_Info+i)->u32ID = E_MMAP_ID_MAD_DEC;
            (pMemMap_Info+i)->u32Addr = MAD_DEC_BUF_ADR;
            (pMemMap_Info+i)->u32Size = MAD_DEC_BUF_LEN;
            (pMemMap_Info+i)->u8Layer = 0;
            (pMemMap_Info+i)->u32Align = 0x00001000;
            (pMemMap_Info+i)->bIs_miu0 = 1;

        }
        else if(i == E_MMAP_ID_MAD_SE)
        {
            (pMemMap_Info+i)->u32ID = E_MMAP_ID_MAD_SE;
            (pMemMap_Info+i)->u32Addr = MAD_SE_BUF_ADR;
            (pMemMap_Info+i)->u32Size = MAD_SE_BUF_LEN;
            (pMemMap_Info+i)->u8Layer = 0;
            (pMemMap_Info+i)->u32Align = 0x00001000;
            (pMemMap_Info+i)->bIs_miu0 = 1;

        }
        else if(i == E_MMAP_ID_COPROCESSOR)
        {
            (pMemMap_Info+i)->u32ID = E_MMAP_ID_COPROCESSOR;
            (pMemMap_Info+i)->u32Addr = MM_COPROCESSOR_ADDR;
            (pMemMap_Info+i)->u32Size = MM_COPROCESSOR_LEN;
            (pMemMap_Info+i)->u8Layer = 0;
            (pMemMap_Info+i)->u32Align = 0x00000000;
            (pMemMap_Info+i)->bIs_miu0 = (MM_COPROCESSOR_TYPE & 0x01) ? 0:1;

        }
        else if(i == E_MMAP_ID_VDPLAYER_DATA)
        {
            (pMemMap_Info+i)->u32ID = E_MMAP_ID_VDPLAYER_DATA;
            (pMemMap_Info+i)->u32Addr = VDPLAYER_DATA_ADR;
            (pMemMap_Info+i)->u32Size = VDPLAYER_DATA_LEN;
            (pMemMap_Info+i)->u8Layer = 0;
            (pMemMap_Info+i)->u32Align = 0x00000000;
            (pMemMap_Info+i)->bIs_miu0 = (VDPLAYER_DATA_MEMORY_TYPE & 0x01) ? 0:1;

        }
        else if(i == E_MMAP_ID_VDPLAYER_BITSTREAM)
        {
            (pMemMap_Info+i)->u32ID = E_MMAP_ID_VDPLAYER_BITSTREAM;
            (pMemMap_Info+i)->u32Addr = VDPLAYER_BS_ADR;
            (pMemMap_Info+i)->u32Size = VDPLAYER_BS_LEN;
            (pMemMap_Info+i)->u8Layer = 0;
            (pMemMap_Info+i)->u32Align = 0x00000000;
            (pMemMap_Info+i)->bIs_miu0 = (VDPLAYER_DATA_MEMORY_TYPE & 0x01) ? 0:1;

        }
        else if(i == E_MMAP_ID_VDPLAYER_BITSTREAM_EXT)
        {
            (pMemMap_Info+i)->u32ID = E_MMAP_ID_VDPLAYER_BITSTREAM_EXT;
            (pMemMap_Info+i)->u32Addr = VDPLAYER_BS_EXT_ADR;
            (pMemMap_Info+i)->u32Size = VDPLAYER_BS_EXT_LEN;
            (pMemMap_Info+i)->u8Layer = 0;
            (pMemMap_Info+i)->u32Align = 0x00000000;
            (pMemMap_Info+i)->bIs_miu0 = (VDPLAYER_DATA_MEMORY_TYPE & 0x01) ? 0:1;

        }
        else if(i == E_MMAP_ID_VDPLAYER_BITSTREAM)
        {
            (pMemMap_Info+i)->u32ID = E_MMAP_ID_VDPLAYER_BITSTREAM;
            (pMemMap_Info+i)->u32Addr = VDPLAYER_BS_ADR;
            (pMemMap_Info+i)->u32Size = VDPLAYER_BS_LEN;
            (pMemMap_Info+i)->u8Layer = 0;
            (pMemMap_Info+i)->u32Align = 0x00000000;
            (pMemMap_Info+i)->bIs_miu0 = (VDPLAYER_BS_MEMORY_TYPE & 0x01) ? 0:1;

        }
        else if(i == E_MMAP_ID_VDPLAYER_BITSTREAM_EXT)
        {
            (pMemMap_Info+i)->u32ID = E_MMAP_ID_VDPLAYER_BITSTREAM_EXT;
            (pMemMap_Info+i)->u32Addr = VDPLAYER_BS_EXT_ADR;
            (pMemMap_Info+i)->u32Size = VDPLAYER_BS_EXT_LEN;
            (pMemMap_Info+i)->u8Layer = 0;
            (pMemMap_Info+i)->u32Align = 0x00000000;
            (pMemMap_Info+i)->bIs_miu0 = (VDPLAYER_BS_EXT_MEMORY_TYPE & 0x01) ? 0:1;

        }
        else if(i == E_MMAP_ID_VDPLAYER_SUBTITLE_BITSTREAM_BUFF)
        {
            (pMemMap_Info+i)->u32ID = E_MMAP_ID_VDPLAYER_SUBTITLE_BITSTREAM_BUFF;
            (pMemMap_Info+i)->u32Addr = VDPLAYER_SUB_BS_ADR;
            (pMemMap_Info+i)->u32Size = VDPLAYER_SUB_BS_LEN;
            (pMemMap_Info+i)->u8Layer = 0;
            (pMemMap_Info+i)->u32Align = 0x00000000;
            (pMemMap_Info+i)->bIs_miu0 = (VDPLAYER_SUB_BS_MEMORY_TYPE & 0x01) ? 0:1;

        }
        else if(i == E_MMAP_ID_MM_SUBTITLE_BUFFER)
        {
            (pMemMap_Info+i)->u32ID = E_MMAP_ID_MM_SUBTITLE_BUFFER;
            (pMemMap_Info+i)->u32Addr = VDPLAYER_SUB_ADR;
            (pMemMap_Info+i)->u32Size = VDPLAYER_SUB_LEN;
            (pMemMap_Info+i)->u8Layer = 0;
            (pMemMap_Info+i)->u32Align = 0x00000000;
            (pMemMap_Info+i)->bIs_miu0 = (VDPLAYER_SUB_MEMORY_TYPE & 0x01) ? 0:1;

        }
        else if(i == E_MMAP_ID_DMX_SECBUF)
        {
            (pMemMap_Info+i)->u32ID = E_MMAP_ID_DMX_SECBUF;
            (pMemMap_Info+i)->u32Addr = VDPLAYER_DMX_SEC_ADR;
            (pMemMap_Info+i)->u32Size = VDPLAYER_DMX_SEC_LEN;
            (pMemMap_Info+i)->u8Layer = 0;
            (pMemMap_Info+i)->u32Align = 0x00010000;
            (pMemMap_Info+i)->bIs_miu0 = (VDPLAYER_DMX_SEC_MEMORY_TYPE & 0x01) ? 0:1;

        }
        else if(i == E_MMAP_ID_PHOTO_INTER)
        {
            (pMemMap_Info+i)->u32ID = E_MMAP_ID_PHOTO_INTER;
            (pMemMap_Info+i)->u32Addr = JPD_INTER_BUF_ADR;
            (pMemMap_Info+i)->u32Size = JPD_INTER_BUF_LEN;
            (pMemMap_Info+i)->u8Layer = 0;
            (pMemMap_Info+i)->u32Align = 0x00001000;
            (pMemMap_Info+i)->bIs_miu0 = (JPD_INTER_BUF_MEMORY_TYPE & 0x01) ? 0:1;
        }
        else if(i == E_MMAP_ID_JPD_READ)
        {
            (pMemMap_Info+i)->u32ID = E_MMAP_ID_JPD_READ;
            (pMemMap_Info+i)->u32Addr = JPD_READ_BUF_ADR;
            (pMemMap_Info+i)->u32Size = JPD_READ_BUF_LEN;
            (pMemMap_Info+i)->u8Layer = 0;
            (pMemMap_Info+i)->u32Align = 0x00001000;
            (pMemMap_Info+i)->bIs_miu0 = (JPD_READ_BUF_MEMORY_TYPE & 0x01) ? 0:1;
        }
        else if(i == E_MMAP_ID_JPD_WRITE)
        {
            (pMemMap_Info+i)->u32ID = E_MMAP_ID_JPD_WRITE;
            (pMemMap_Info+i)->u32Addr = JPD_OUT_ADR;
            (pMemMap_Info+i)->u32Size = JPD_OUT_LEN;
            (pMemMap_Info+i)->u8Layer = 0;
            (pMemMap_Info+i)->u32Align = 0x00001000;
            (pMemMap_Info+i)->bIs_miu0 = (JPD_OUT_MEMORY_TYPE & 0x01) ? 0:1;
        }
        else if(i == E_MMAP_ID_PHOTO_SHAREMEM)
        {
            (pMemMap_Info+i)->u32ID = E_MMAP_ID_PHOTO_SHAREMEM;
            (pMemMap_Info+i)->u32Addr = PHOTO_SHARE_MEM_ADR;
            (pMemMap_Info+i)->u32Size =PHOTO_SHARE_MEM_LEN;
            (pMemMap_Info+i)->u8Layer = 0;
            (pMemMap_Info+i)->u32Align = 0x00001000;
            (pMemMap_Info+i)->bIs_miu0 = (PHOTO_SHARE_MEM_MEMORY_TYPE & 0x01) ? 0:1;
        }
        else if(i == E_MMAP_ID_DMX_SECBUF)
        {
            (pMemMap_Info+i)->u32ID = E_MMAP_ID_DMX_SECBUF;
            (pMemMap_Info+i)->u32Addr = TSP_FW_BUF_ADR;
            (pMemMap_Info+i)->u32Size =TSP_FW_BUF_LEN;
            (pMemMap_Info+i)->u8Layer = 0;
            (pMemMap_Info+i)->u32Align = 0x00000000;
            (pMemMap_Info+i)->bIs_miu0 = (TSP_FW_BUF_MEMORY_TYPE & 0x01) ? 0:1;
        }
        else if(i == E_MMAP_ID_DUMMY2)
        {
            (pMemMap_Info+i)->u32ID = E_MMAP_ID_DUMMY2;
            (pMemMap_Info+i)->u32Addr = TSP_VQ_BUF_ADR;
            (pMemMap_Info+i)->u32Size =TSP_VQ_BUF_LEN;
            (pMemMap_Info+i)->u8Layer = 0;
            (pMemMap_Info+i)->u32Align = 0x00000000;
            (pMemMap_Info+i)->bIs_miu0 = (TSP_VQ_BUF_MEMORY_TYPE & 0x01) ? 0:1;
        }
    }
    return TRUE;
}

// true if TDAL_MP is initialized
MS_BOOL is_TDAL_MP_Initialized(void)
{
	return b_MM_Initied;
}

LOCAL void _SubQueuePushHandler(unsigned int  u32Info)
{
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
}

LOCAL void _SubtitleDisplayHandler(unsigned int  u32Info)
{
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
}

LOCAL bool TDAL_MPi_VideoSetOutput(VDEC_DispInfo *pstVidStatus)
{
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
    tTDAL_MPi_CallbackMsg msg;
    MS_U32 actualSize;
    MS_BOOL bRet;
    tTDAL_MP_EvtNotify notifyFct = NULL;
    tTDAL_MP_Event event;
    int index;

    mTBOX_FCT_ENTER("TDAL_MPi_CallbackTask");

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


}

void TDAL_MPi_LockMutex()
{
#ifdef PROTECT_TDAL_MP
    TDAL_LockMutex(TDAL_MPi_Mutex);
    TDAL_MPi_MutexThreadLocked = TDAL_GetRunningThreadSystemId();
#endif
}

void TDAL_MPi_UnlockMutex()
{
#ifdef PROTECT_TDAL_MP
    TDAL_MPi_MutexThreadLocked = NULL;
    TDAL_UnlockMutex(TDAL_MPi_Mutex);
#endif
}

bool TDAL_MPi_LockMutexIfDifferentThread()
{
#ifdef PROTECT_TDAL_MP
    if (TDAL_MPi_MutexThreadLocked != TDAL_GetRunningThreadSystemId())
    {
        TDAL_MPi_LockMutex();
        return true;
    }
#endif
    return false;
}


