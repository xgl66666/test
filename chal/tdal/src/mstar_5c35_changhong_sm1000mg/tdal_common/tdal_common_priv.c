/*****************************************************************************
 *                  COPYRIGHT 2004 IWEDIA TECHNOLOGIES            *
 *****************************************************************************
 *
 * MODULE NAME: TDAL_COMMON_PRIV
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/sources/chal/core/tdal/branches/idecode_hd/os21/tdal_COMMON_PRIV/tdal_COMMON_PRIV.c $
 *        $Rev: 414 $
 *        $Date: 2011-09-23 12:34:21 +0200 (Fri, 23 Sep 2011) $
 *
 * Description : Front Panel driver TDAL library implementation.
 *
 ****************************************************************************/

/****************************************************************************
 *  INCLUDES                                                                *
 ****************************************************************************/
/*   Generic   Headers   */

#include "MsCommon.h"
#include "MsOS.h"

#include "crules.h"
#include "tbox.h"
#include "tdal_common.h"

#include "tdal_common_priv.h"

/****************************************************************************
 *  DEFINES                                                                 *
 ****************************************************************************/
/****************************************************************************
 *  MACROS                                                                  *
 ****************************************************************************/

/****************************************************************************
 *  TYPEDEFS                                                                *
 ****************************************************************************/
mTBOX_SET_MODULE(eTDAL_COMMON);
/****************************************************************************
 *  GLOBAL VARIABLES (GLOBAL/IMPORT)                                        *
 ****************************************************************************/

/****************************************************************************
 *  LOCAL MODULE VARIABLES (MODULE/IMPORT)                                  *
 ****************************************************************************/

/****************************************************************************
 *  TRACE CONFIGURATION                                                     *
 ****************************************************************************/

/****************************************************************************
 *  LOCAL FILE VARIABLES (LOCAL)                                            *
 ****************************************************************************/

LOCAL TDAL_queue_id TDALi_WorkerThreadQueue = -1;
LOCAL TDAL_task_id TDALi_WorkerThreadTask = NULL;
LOCAL MS_U8  TDAL_WorkerThreadTask[(16*1024)];
LOCAL bool TDAL_WorkerThreadFinished;
/****************************************************************************
 *  FUNCTIONS DEFINITIONS (LOCAL/GLOBAL)                                    *
 ****************************************************************************/

typedef struct
{
    tTDALm_WorkerThreadsDescriptor originalDescriptor;
    tTDALm_WorkerThreadsFunc  funcToCall;
    bool shouldExit;
}tTDALi_WorkerThreadDescriptor;

typedef struct
{

} tTDAL_;

/**========================================================================**
 * Function    : TDALi_WorkerThread
 *
 * Description  :
 *
 * Side effects :
 *
 * Comment      :
 *
 **========================================================================**/
LOCAL void TDALi_WorkerThread(void * arg)
{
    tTDALi_WorkerThreadDescriptor descr;
    MS_U32 actualSize;
    MS_BOOL bRet;

    mTBOX_FCT_ENTER("TDALi_WorkerThread");

    while (1)
    {
#if MSTAR_QUEUE
        bRet = MsOS_RecvFromQueue(TDALi_WorkerThreadQueue, (MS_U8 *) &descr, sizeof(tTDALi_WorkerThreadDescriptor), &actualSize, MSOS_WAIT_FOREVER);
#else
        bRet = TDAL_Dequeue(TDALi_WorkerThreadQueue, &descr);
#endif
        //mTBOX_ASSERT(sizeof(tTDALi_WorkerThreadDescriptor) == actualSize);

        if (bRet)
        {
            if(descr.shouldExit)
            {
                mTBOX_TRACE((kTBOX_NIV_1, "Exit worker thread\n"));
                break;
            }

            descr.funcToCall(&descr.originalDescriptor);
        }
    }

    TDAL_WorkerThreadFinished = true;

    mTBOX_RETURN;
}


bool TDALm_WorkerThreadsInitalize(void)
{
    mTBOX_FCT_ENTER("TDALp_WorkerThreadsInitalize");
#if MSTAR_QUEUE
    TDALi_WorkerThreadQueue = MsOS_CreateQueue(NULL,
            NULL,
            E_MSG_FIXED_SIZE,
            sizeof(tTDALi_WorkerThreadDescriptor),
            E_MSOS_FIFO,
            "Tdal Worker Thread Queue");
#else
    TDAL_CreateQueue(TDAL_QUEUE_MSG_MAX_NO, sizeof(tTDALi_WorkerThreadDescriptor), &TDALi_WorkerThreadQueue);
#endif
    if (TDALi_WorkerThreadQueue < 0)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "[TDALp_WorkerThreadsInitalize] MsOS_CreateQueue FAIL\n"));
        mTBOX_RETURN(false);
    }

    TDALi_WorkerThreadTask = TDAL_CreateTask(
            eTDAL_PRIORITY_NORMAL,
            "Tdal worker Task",
            TDAL_WorkerThreadTask,
            sizeof(TDAL_WorkerThreadTask),
            TDALi_WorkerThread,
            NULL);

    if (TDALi_WorkerThreadTask == NULL)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "[TDALp_WorkerThreadsInitalize] Creating task failed\n"));
        mTBOX_RETURN(false);
    }

    mTBOX_RETURN(true);

}

bool TDALm_WorkerThreadsDeinitalize(void)
{
    tTDALi_WorkerThreadDescriptor newDesc;

    newDesc.shouldExit = true;

#if MSTAR_QUEUE
    MsOS_SendToQueue(TDALi_WorkerThreadQueue, (MS_U8 *) &newDesc, sizeof(tTDALi_WorkerThreadDescriptor), MSOS_WAIT_FOREVER);
#else
    TDAL_Enqueue(TDALi_WorkerThreadQueue, &newDesc);
#endif
    TDAL_DeleteTask(TDALi_WorkerThreadTask);
    TDALi_WorkerThreadTask = NULL;

#if MSTAR_QUEUE
    MsOS_DeleteQueue(TDALi_WorkerThreadQueue);
#else
    TDAL_DeleteQueue(TDALi_WorkerThreadQueue);
#endif
    TDALi_WorkerThreadQueue = -1;

    return true;
}

bool TDALm_WorkerThreadsSendRequest(tTDALm_WorkerThreadsFunc funcToCall, tTDALm_WorkerThreadsDescriptor * descriptor)
{
    tTDALi_WorkerThreadDescriptor newDesc;
    MS_BOOL bRet;

    mTBOX_FCT_ENTER("TDALm_WorkerThreadsSendRequest");

    if (descriptor == NULL)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Descriptor is NULL\n"));
        mTBOX_RETURN(false);
    }

    if (funcToCall == NULL)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Func to call is NULL\n"));
        mTBOX_RETURN(false);
    }

    newDesc.originalDescriptor = *descriptor;
    newDesc.funcToCall = funcToCall;
    newDesc.shouldExit = false;

#if MSTAR_QUEUE
    bRet = MsOS_SendToQueue(TDALi_WorkerThreadQueue, (MS_U8 *) &newDesc, sizeof(tTDALi_WorkerThreadDescriptor), 0);
#else
    bRet = TDAL_Enqueue(TDALi_WorkerThreadQueue, &newDesc);
#endif
    if (bRet == FALSE)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Could not add to queue\n"));
        mTBOX_RETURN(false);
    }

    mTBOX_RETURN(true);
}

