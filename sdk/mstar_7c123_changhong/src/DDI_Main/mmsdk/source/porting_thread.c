#include "MsTypes.h"
#include "MsOS.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#ifdef MSOS_TYPE_LINUX
#include <pthread.h>
#endif
#ifdef MSOS_TYPE_ECOS
#include <list.h>
#endif
#include "HbCommon.h"

#include "porting_thread.h"
#include "porting_sysinfo.h"
#include "porting_os.h"

#ifdef AVP_ENABLE
#include <errno.h>
#include <sys/time.h>
#include "drvDTC.h"
#endif

#define PT_THREAD_ERR(fmt, arg...)   PT_SYS_PrintLog(E_MMSDK_DBG_LEVEL_ERR, "\033[1;31m######[%s]###### "fmt" \033[0m\n",__FUNCTION__,##arg);
#define PT_THREAD_DBG(fmt, arg...)   PT_SYS_PrintLog(E_MMSDK_DBG_LEVEL_DBG, "\033[1;33m######[%s]###### "fmt" \033[0m\n",__FUNCTION__,##arg);

#ifndef AVP_ENABLE

#ifndef FREE
#define FREE(_x_)                 if(NULL != (_x_)){ PT_MsOS_FreeMemory (_x_); (_x_) = NULL; }
#endif

#else

#ifndef UNUSED
#define UNUSED(x) (void)(x)
#endif

#ifndef PTH_RET_CHK
#define PTH_RET_CHK(_x_) _x_
#endif

#ifndef FREE
#define FREE(_x_)                 if(NULL != (_x_)){ free (_x_); (_x_) = NULL; }
#endif

#endif

#define TASK_STK_SIZE       8192
#define TASK_NAME_MAX_LEN   16
#define MUTEX_NAME_MAX_LEN  64

#if defined(MSOS_TYPE_ECOS)
typedef struct _PT_THREAD_INFO
{
    Task_Info stTaskInfo;
    struct list_head stList;
}PT_THREAD_INFO;

static LIST_HEAD(PTThreadInfoHead);

static TaskPriority _PT_Thread_ConvertTaskPriority(EN_MMSDK_TASK_PRIORITY eTaskPri)
{
    switch(eTaskPri)
    {
        case E_MMSDK_TASK_PRI_SYS:
            return E_TASK_PRI_SYS;
        case E_MMSDK_TASK_PRI_HIGHEST:
            return E_TASK_PRI_HIGHEST;
        case E_MMSDK_TASK_PRI_HIGH:
            return E_TASK_PRI_HIGH;
        case E_MMSDK_TASK_PRI_MEDIUM:
            return E_TASK_PRI_MEDIUM;
        case E_MMSDK_TASK_PRI_LOW:
            return E_TASK_PRI_LOW;
        case E_MMSDK_TASK_PRI_LOWEST:
            return E_TASK_PRI_LOWEST;
        default:
            return E_TASK_PRI_MEDIUM;

    }
}

#elif defined(MSOS_TYPE_LINUX)
    #ifndef ASSERT
    #include <assert.h>
    #define ASSERT(x)  assert(x)
    #endif
#endif
MMSDK_BOOL PT_Thread_ThreadCreate(MMSDK_U32 *pu32ThreadId,
                            void *(*start_routine) (void *),
                            void* pTaskArg,
                            const EN_MMSDK_TASK_PRIORITY eTaskPriority,
                            void * pStackEntry,
                            const MMSDK_U32 u32StackSize,
                            const char *pTaskName)
{
    PT_THREAD_DBG("%s", "Entered.");
    if((!pu32ThreadId) || (!start_routine))
        return FALSE;

    // Thread name length must <= 16
    MS_U8 u8TaskName[TASK_NAME_MAX_LEN];
    snprintf((char*)u8TaskName, TASK_NAME_MAX_LEN, "%s", pTaskName);   // snprintf will add '\0' at the end of string automatically

#if defined(MSOS_TYPE_ECOS)
    MS_S32 s32ThreadId = 0;
    s32ThreadId = MsOS_CreateTask((TaskEntry)start_routine,
                                    (MS_U32)pTaskArg,
                                    _PT_Thread_ConvertTaskPriority(eTaskPriority),
                                    TRUE,
                                    pStackEntry,
                                    u32StackSize,
                                    (char*)u8TaskName);

    if(s32ThreadId < 0)
    {
        PT_THREAD_DBG("%s", "MsOS_CreateTask failed.");
        return FALSE;
    }
    else
    {
        PT_THREAD_DBG("Task Id is %d.\n", *pu32ThreadId);
        PT_THREAD_INFO *pPtThreadInfo = (PT_THREAD_INFO*)PT_MsOS_AllocateMemory(sizeof(PT_THREAD_INFO));
        if(pPtThreadInfo == NULL)
        {
            PT_THREAD_DBG("%s", "PT_MsOS_AllocateMemory failed.");
            return FALSE;
        }
        memset(pPtThreadInfo, 0x0, sizeof(PT_THREAD_INFO));
        pPtThreadInfo->stTaskInfo.iId = s32ThreadId;
        pPtThreadInfo->stTaskInfo.ePriority = _PT_Thread_ConvertTaskPriority(eTaskPriority);
        pPtThreadInfo->stTaskInfo.pStack = pStackEntry;
        pPtThreadInfo->stTaskInfo.u32StackSize = u32StackSize;
        strncpy(pPtThreadInfo->stTaskInfo.szName, (char*)u8TaskName, TASK_NAME_MAX_LEN-1);

        list_add(&pPtThreadInfo->stList, &PTThreadInfoHead);

        *pu32ThreadId = s32ThreadId;
        return TRUE;
    }
#elif defined(MSOS_TYPE_LINUX)
    int intPTHChk;
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    if (eTaskPriority == E_MMSDK_TASK_PRI_SUPER_SCHEDULE_RR)
    {
        int policy;
        struct sched_param param;
        if (pthread_getschedparam(pthread_self(), &policy, &param) == 0)
        {
            if (policy != SCHED_RR)
            {
                PT_THREAD_DBG("Replace Policy %d -> SCHED_RR\n", policy);
                policy = SCHED_RR;
                pthread_setschedparam(pthread_self(), policy, &param);
            }
            else
            {
                PT_THREAD_DBG("Schedule is Round Robin\n");
            }
        }
        else
        {
            PT_THREAD_ERR("Set schedule fail\n");
        }
        pthread_attr_getschedparam(&attr, &param);
        param.sched_priority = (int)eTaskPriority - 1;
        pthread_attr_setschedparam(&attr, &param);
    }

    pthread_attr_setstacksize(&attr, u32StackSize);
    intPTHChk = pthread_create((pthread_t*)pu32ThreadId, &attr, start_routine, pTaskArg);
    pthread_attr_destroy(&attr);
    if (intPTHChk ==0)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
#endif
}

void PT_Thread_ThreadExit (void* retval)
{
    PT_THREAD_DBG("%s", "Entered.");
#if defined(MSOS_TYPE_LINUX)
    pthread_exit(retval);
#endif
}

MMSDK_BOOL PT_Thread_ThreadJoin (const MMSDK_U32 u32ThreadId)
{
    PT_THREAD_DBG("%s", "Entered.");
    MS_BOOL bRet = TRUE;
#if defined(MSOS_TYPE_ECOS)
    TaskStatus eTaskStatus;
    PT_THREAD_INFO* pstTempInfo;
    PT_THREAD_INFO* pstPTTHreadInfo = NULL;
    struct list_head* stListHeadIterator;

    list_for_each(stListHeadIterator, &PTThreadInfoHead)
    {
        pstTempInfo = list_entry(stListHeadIterator, PT_THREAD_INFO, stList);
        if(pstTempInfo->stTaskInfo.iId == u32ThreadId)
        {
            pstPTTHreadInfo = pstTempInfo;
            break;
        }
    }

    list_del(stListHeadIterator);

    while(1)
    {
        if(!MsOS_GetTaskStatus(&pstPTTHreadInfo->stTaskInfo,&eTaskStatus))
        {
            PT_THREAD_ERR("Error!! MsOS_GetTaskStatus() fail!! \n");
            bRet = FALSE;
            break;
        }

        if(eTaskStatus == E_TASK_NOT_EXIST)
        {
            PT_MsOS_FreeMemory(pstPTTHreadInfo);
            pstPTTHreadInfo = NULL;
            bRet = TRUE;
            break;
        }
        else
        {
            PT_THREAD_DBG("[FUNC %s] [LINE %d] Wait for Task#%x %s termination \n", __FUNCTION__,__LINE__,u32ThreadId,pstPTTHreadInfo->stTaskInfo.szName);
            MsOS_DelayTask(1);
        }
    }
#elif defined(MSOS_TYPE_LINUX)
    int intPTHChk;
    intPTHChk = pthread_join(u32ThreadId, NULL);
    ASSERT(intPTHChk == 0);
#endif
    return bRet;
}

MMSDK_BOOL PT_Thread_ThreadSetName (const char * strName)
{
    /* Not such API in eCos. */
    PT_THREAD_DBG("%s", "Entered.");
    if(!strName)
        return FALSE;

    PT_THREAD_DBG("Thread name is %s.\n", strName);
    return TRUE;
}

MMSDK_BOOL PT_Thread_MutexInit(PT_MUTEXITEM* pMutex)
{
    //PT_THREAD_DBG("%s", "Entered.");
#ifndef AVP_ENABLE
    MS_S32 s32MutexId = 0;
    static MS_U16 u16MutexIndex = 0;
    char u8mutexName[MUTEX_NAME_MAX_LEN];

    if(!pMutex)
        return FALSE;

    *pMutex = (void*)PT_MsOS_AllocateMemory(sizeof(MS_S32));
    if (*pMutex == NULL)
        return FALSE;

    memset(&u8mutexName, 0x0, sizeof(u8mutexName));
    snprintf((char*)u8mutexName, MUTEX_NAME_MAX_LEN, "PT_THREAD_%d_MUTEX", u16MutexIndex);
    u16MutexIndex++;
    s32MutexId = MsOS_CreateMutex(E_MSOS_FIFO, u8mutexName, MSOS_PROCESS_SHARED);

    PT_THREAD_DBG("s32MutexId is %d.\n", s32MutexId);

    if(s32MutexId < 0)
        return FALSE;
    else
    {
        memcpy(*pMutex, &s32MutexId, sizeof(MS_S32));
        PT_THREAD_DBG("*pMutex is %p.\n", *pMutex);
        PT_THREAD_DBG("&s32MutexId is %p.\n", &s32MutexId);
        return TRUE;
    }
#else
    int intPTHChk;
    *pMutex = (void*)malloc(sizeof(pthread_mutex_t));
    intPTHChk = PTH_RET_CHK(pthread_mutex_init((pthread_mutex_t*)*pMutex, NULL));

    if (intPTHChk ==0)
    {
        return TRUE;
    }
    else
    {
        FREE(*pMutex);
        return FALSE;
    }
#endif
}

MMSDK_BOOL PT_Thread_MutexDestroy(PT_MUTEXITEM*pMutex)
{
    //PT_THREAD_DBG("%s", "Entered.");
#ifndef AVP_ENABLE
    MS_S32* s32Mutex = 0;
    if(!pMutex)
        return FALSE;

    s32Mutex = (MS_S32*)(*pMutex);
    if(!MsOS_DeleteMutex(*s32Mutex))
        return FALSE;

    FREE(*pMutex);

    return TRUE;
#else
    int intPTHChk = 0;
    if ((pMutex != NULL ) && (*pMutex != NULL))
    {
        intPTHChk = PTH_RET_CHK(pthread_mutex_destroy((pthread_mutex_t*)*pMutex));
    }
    FREE(*pMutex);
    if (intPTHChk ==0)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
#endif
}

void PT_Thread_MutexLock(PT_MUTEXITEM pMutex)
{
    //PT_THREAD_DBG("%s", "Entered.");
#ifndef AVP_ENABLE
    MS_S32* s32Mutex = 0;
    if(!pMutex)
    {
        PT_THREAD_ERR("Error!! pMutex is NULL!! /n");
        return;
    }

    s32Mutex = (MS_S32*)(pMutex);

    //PT_THREAD_DBG("*s32Mutex is %d.\n", *s32Mutex);

    if (pMutex)
        MsOS_ObtainMutex(*s32Mutex, MSOS_WAIT_FOREVER);
#else
    if (pMutex)
    {
        pthread_mutex_lock((pthread_mutex_t*)pMutex);
    }
#endif
}
void PT_Thread_MutexUnlock(PT_MUTEXITEM pMutex)
{
    //PT_THREAD_DBG("%s", "Entered.");
#ifndef AVP_ENABLE
    MS_S32* s32Mutex = 0;
    if(!pMutex)
    {
        PT_THREAD_ERR("Error!! pMutex is NULL!! /n");
        return;
    }

    s32Mutex = (MS_S32*)(pMutex);

    //PT_THREAD_DBG("*s32Mutex is %d.\n", *s32Mutex);

    if (pMutex)
        MsOS_ReleaseMutex(*s32Mutex);
#else
    if (pMutex)
    {
        pthread_mutex_unlock((pthread_mutex_t*)pMutex);
    }
#endif
}

#ifdef AVP_ENABLE

MMSDK_BOOL PT_Thread_Cond_Init(PT_CONDVARIABLEITEM* pCond, const void* pAttr, PT_MUTEXITEM *pMutex)
{
    PT_THREAD_DBG("");
    MMSDK_S32 s32ErrorCode = 0;
    // only use by ecos
    UNUSED(pMutex);

    *pCond = (void*)malloc(sizeof(pthread_cond_t));


    if (*pCond)
    {
        if (pAttr)
        {
            s32ErrorCode = pthread_cond_init((pthread_cond_t*)(*pCond), (pthread_condattr_t *)pAttr);
        }
        else
        {
            s32ErrorCode = pthread_cond_init((pthread_cond_t*)(*pCond), NULL);
        }

        if (s32ErrorCode)
        {
            PT_THREAD_ERR("Initialize condition variable 0x%"DTC_MS_U32_x" error:%"DTC_MS_S32_d"", (MMSDK_U32)pCond, s32ErrorCode);
            FREE(*pCond);
            return FALSE;
        }
        else
        {
            return TRUE;
        }
    }
    return FALSE;
}

MMSDK_BOOL PT_Thread_Cond_Destroy(PT_CONDVARIABLEITEM *pCond)
{
    PT_THREAD_DBG("");
    MMSDK_S32 s32ErrorCode = 0;

    if (*pCond)
    {
        s32ErrorCode = pthread_cond_destroy((pthread_cond_t*)(*pCond));
        FREE(*pCond);
        if (s32ErrorCode == 0)
        {
            return TRUE;
        }
        else
        {
            PT_THREAD_ERR("Destroy condition variable 0x%"DTC_MS_U32_x" error:%"DTC_MS_S32_d"", (MMSDK_U32)pCond, s32ErrorCode);
            return FALSE;
        }
    }
    return FALSE;
}

MMSDK_BOOL PT_Thread_Cond_Wait(PT_CONDVARIABLEITEM pCond, PT_MUTEXITEM pMutex)
{
    //PT_THREAD_DBG("");
    MMSDK_S32 s32ErrorCode = 0;

    if (pCond && pMutex)
    {
        if((s32ErrorCode = pthread_cond_wait((pthread_cond_t*)pCond, (pthread_mutex_t *)pMutex)))
        {
            PT_THREAD_ERR("Condition variable 0x%"DTC_MS_U32_x" mutex 0x%"DTC_MS_U32_x" wait error:%"DTC_MS_S32_d"", (MMSDK_U32)pCond, (MMSDK_U32)pMutex, s32ErrorCode);
            return FALSE;
        }
        else
        {
            return TRUE;
        }
    }
    return FALSE;
}


MMSDK_BOOL PT_Thread_Cond_TimeWait(PT_CONDVARIABLEITEM pCond, PT_MUTEXITEM pMutex, const MMSDK_U32 u32RelatvieWaitTimeMs)
{
    //PT_THREAD_DBG("");
    MMSDK_S32 s32ErrorCode = 0;

    struct timespec stTime;
    memset(&stTime, 0, sizeof(struct timespec));
    struct timeval now;
    memset(&now, 0, sizeof(struct timeval));

    if (pCond && pMutex)
    {
        gettimeofday(&now, NULL);
        //PT_THREAD_DBG("pRelatvieWaitTimeMs = %"DTC_MS_U32_d"\n", u32RelatvieWaitTimeMs);
        stTime.tv_sec = now.tv_sec;
        stTime.tv_nsec = now.tv_usec * 1000;
        //PT_THREAD_DBG("Now time = %"DTC_MS_S64_d".%.9ld\n", (MMSDK_S64)(stTime.tv_sec), stTime.tv_nsec);

        // Convert u32RelatvieWaitTimeMs to abs time
        stTime.tv_sec += u32RelatvieWaitTimeMs / 1000;
        stTime.tv_nsec += (u32RelatvieWaitTimeMs % 1000) * 1000000;
        if (stTime.tv_nsec >= 1000000000)
        {
            stTime.tv_nsec -= 1000000000;
            stTime.tv_sec  += 1;
        }
        //PT_THREAD_DBG("Absolute wait time = %"DTC_MS_S32_d".%.3ld\n", (MMSDK_S64)(stTime.tv_sec), stTime.tv_nsec);
        s32ErrorCode = pthread_cond_timedwait((pthread_cond_t*)pCond, (pthread_mutex_t*)pMutex, &stTime);

        if (s32ErrorCode == 0)
        {
            return TRUE;
        }
        else if (s32ErrorCode == ETIMEDOUT)
        {
            //PT_THREAD_ERR("Wait until timeout\n");
            return FALSE;
        }
        else
        {
            PT_THREAD_ERR("Condition variable 0x%"DTC_MS_U32_x" mutex 0x%"DTC_MS_U32_x" abs time %"DTC_MS_S64_d".%.3ld wait error:%"DTC_MS_S32_d"",
                (MMSDK_U32)pCond, (MMSDK_U32)pMutex, (MMSDK_S64)(stTime.tv_sec), stTime.tv_nsec, s32ErrorCode);
            return FALSE;
        }
    }
    return FALSE;
}

MMSDK_BOOL PT_Thread_Cond_Signal(PT_CONDVARIABLEITEM pCond)
{
    //PT_THREAD_DBG("");
    MMSDK_S32 s32ErrorCode = 0;

    if (pCond)
    {
        if ((s32ErrorCode = pthread_cond_signal((pthread_cond_t*)pCond)))
        {
            PT_THREAD_ERR("Condition variable 0x%"DTC_MS_U32_x" signal error:%"DTC_MS_S32_d"", (MMSDK_U32)pCond, s32ErrorCode);
            return FALSE;
        }
        else
        {
            return TRUE;
        }
    }
    return FALSE;
}

MMSDK_BOOL PT_Thread_Cond_Broadcast(PT_CONDVARIABLEITEM pCond)
{
    PT_THREAD_DBG("");
    MMSDK_S32 s32ErrorCode = 0;

    if (pCond)
    {
        if ((s32ErrorCode = pthread_cond_broadcast((pthread_cond_t*)pCond)))
        {
            PT_THREAD_ERR("Condition variable 0x%"DTC_MS_U32_x" broadcast error:%"DTC_MS_S32_d"", (MMSDK_U32)pCond, s32ErrorCode);
            return FALSE;
        }
        else
        {
            return TRUE;
        }
    }
    return FALSE;
}

#endif
