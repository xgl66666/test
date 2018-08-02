/******************************************************************************
 *                  COPYRIGHT 2004 IWEDIA TECHNOLOGIES            *
 ******************************************************************************
 *
 * MODULE NAME: TDAL_COMMON_PRIV
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/sources/chal/core/tdal/branches/idecode_hd/os21/tdal_priv_inc/tdal_common_priv.h $
 *        $Rev: 414 $
 *        $Date: 2011-09-23 12:34:21 +0200 (Fri, 23 Sep 2011) $
 *
 * Description           : library primitives private tdal header.
 *
 *****************************************************************************/

#ifndef _TDAL_COMMON_MODULE_PRIV_H_
#define _TDAL_COMMON_MODULE_PRIV_H_

#ifdef __cplusplus
extern "C" {
#endif

/*--------------------------------------*/
/*      Public types definition      */
/*--------------------------------------*/

typedef uint32_t    TDAL_queue_id;   /* TDAL queue ID */
typedef int16_t     TDAL_qdepth;     /* TDAL queue depth */
typedef uint32_t    TDAL_qwidth;   /* TDAL queue width */

//bool TDAL_CreateQueue(TDAL_qdepth maxNumber, TDAL_qwidth msgSize, TDAL_queue_id *qid);
//bool TDAL_Enqueue(TDAL_queue_id qid, void *msg);
//bool TDAL_Dequeue(TDAL_queue_id qid, void *msg_buf);
//bool TDAL_DeleteQueue(TDAL_queue_id qid);

typedef struct
{
    uint32_t param1;
    uint32_t param2;
    uint32_t param3;
    void *   pointer;
} tTDALm_WorkerThreadsDescriptor;

typedef void (* tTDALm_WorkerThreadsFunc) (tTDALm_WorkerThreadsDescriptor *);

/*------------------------------------------*/
/*       Public Functions Declarations      */
/*------------------------------------------*/

bool TDALm_WorkerThreadsInitalize(void);
bool TDALm_WorkerThreadsDeinitalize(void);
bool TDALm_WorkerThreadsSendRequest(tTDALm_WorkerThreadsFunc funcToCall, tTDALm_WorkerThreadsDescriptor * descriptor);

#define TDAL_QUEUE_MSG_MAX_NO 10

#ifdef __cplusplus
}
#endif

#endif /*_TDAL_COMMON_MODULE_PRIV_H_*/
