#include <stdio.h>
#include "MsTypes.h"
#include "porting_os.h"
#include "MsOS.h"
#include "mapp_vdplayer_sharememcfg.h"
#include "porting_sysinfo.h"

#ifdef AVP_ENABLE
#include <errno.h>
#include <semaphore.h>
#include <sys/time.h>
#include <string.h>
#include <stdlib.h>
#include "drvDTC.h"

#ifndef FREE
#define FREE(_x_)                 if(NULL != (_x_)){ free (_x_); (_x_) = NULL; }
#endif
#endif

#define FLOW(fmt, arg...)         //printf("\033[1;33m######[%s]######"fmt" \033[0m\n",__FUNCTION__,##arg)
#define ERR(fmt, arg...)         printf("\033[1;31m######[ERR][%s]######"fmt" \033[0m\n",__FUNCTION__,##arg)

MMSDK_BOOL MApp_MediaCodec_SetMemPool(MMSDK_S32 s32PoolId, MMSDK_BOOL bIsCached);
MMSDK_BOOL PT_MsOS_MemoryInit(MMSDK_S32 s32Reserved, MMSDK_BOOL bReserved)
{
    FLOW("");
#if defined(MSOS_TYPE_ECOS)
    MApp_MediaCodec_SetMemPool(PT_SYS_GetCachePoolID(), TRUE);
    MApp_MediaCodec_SetMemPool(PT_SYS_GetNonCachePoolID(), FALSE);
#endif
    return TRUE;
}

MMSDK_BOOL PT_MsOS_MemoryDeinit(void)
{
    FLOW("");
    return TRUE;
}

void * PT_MsOS_AllocateMemory (MMSDK_U32 u32Size)
{
    FLOW("");
#ifndef AVP_ENABLE
    return MsOS_AllocateMemory (u32Size, PT_SYS_GetCachePoolID());
#else
	return malloc(u32Size);
#endif
}

void * PT_MsOS_ReallocateMemory (void *pOrgAddress, MMSDK_U32 u32NewSize)
{
    FLOW("");
#ifndef AVP_ENABLE
    if(!pOrgAddress)
    {
        printf("Error!! pOrgAddress is NULL!! /n");
        return NULL;
    }

    return MsOS_ReallocateMemory(pOrgAddress, u32NewSize, PT_SYS_GetCachePoolID());
#else
	return realloc(pOrgAddress,u32NewSize);
#endif
}

void PT_MsOS_FreeMemory (void *pAddress)
{
    FLOW("");
#ifndef AVP_ENABLE
   if(!pAddress)
    {
        printf("Error!! pAddress is NULL!! /n");
        return;
    }

    MsOS_FreeMemory(pAddress, PT_SYS_GetCachePoolID());
#else
    if(pAddress != NULL)
    {
        free(pAddress);
    }
    return;
#endif
}

MMSDK_U32 PT_MsOS_PA2BA(MMSDK_U32 addr)
{
    FLOW("");
    return MsOS_PA2BA(addr);
}

MMSDK_U32 PT_MsOS_BA2PA(MMSDK_U32 addr)
{
    FLOW("");
    return MsOS_BA2PA(addr);
}

MMSDK_U64 PT_MsOS_GetSystemTime(void)
{
    FLOW("");
    MMSDK_U64 u64SystemTime = (MMSDK_U64)MsOS_GetSystemTime();
    return u64SystemTime;
}

void PT_MsOS_DelayTask(MMSDK_U32 u32Ms)
{
    FLOW("");
    MsOS_DelayTask(u32Ms);
    return;
}

#ifdef AVP_ENABLE

MMSDK_BOOL PT_MsOS_Sem_Init(PT_SEMAPHOREITEM* pSem, MMSDK_S32 s32SemVal)
{
    FLOW("");
    MMSDK_S32 s32ErrorCode = 0;

    *pSem = (void*)malloc(sizeof(sem_t));

    if (*pSem)
    {
        s32ErrorCode = sem_init((sem_t*)(*pSem), 0, s32SemVal);
        if (s32ErrorCode)
        {
            ERR("Initialize semaphore 0x%"DTC_MS_U32_x" error:%"DTC_MS_S32_d"", (MMSDK_U32)pSem, s32ErrorCode);
            FREE(*pSem);
            return FALSE;
        }
        else
        {
            return TRUE;
        }
    }
    return FALSE;
}

MMSDK_BOOL PT_MsOS_Sem_Destroy(PT_SEMAPHOREITEM pSem)
{
    FLOW("");
    MMSDK_S32 s32ErrorCode = 0;

    if (pSem)
    {
        s32ErrorCode = sem_destroy((sem_t*)(pSem));
        FREE(pSem);
        if (s32ErrorCode == 0)
        {
            return TRUE;
        }
        else
        {
            ERR("Destroy semaphore 0x%"DTC_MS_U32_x" error:%"DTC_MS_S32_d"", (MMSDK_U32)pSem, s32ErrorCode);
            return FALSE;
        }
    }
    return FALSE;
}

MMSDK_BOOL PT_MsOS_Sem_Post(PT_SEMAPHOREITEM pSem)
{
    FLOW("");
    MMSDK_S32 s32ErrorCode = 0;

    if (pSem)
    {
        if ((s32ErrorCode = sem_post((sem_t*)pSem)))
        {
            ERR("Semaphore 0x%"DTC_MS_U32_x" post error:%"DTC_MS_S32_d"", (MMSDK_U32)pSem, s32ErrorCode);
            return FALSE;
        }
        else
        {
            return TRUE;
        }
    }
    return FALSE;
}

MMSDK_BOOL PT_MsOS_Sem_Wait(PT_SEMAPHOREITEM pSem)
{
    FLOW("");
    MMSDK_S32 s32ErrorCode = 0;

    if (pSem)
    {
        if ((s32ErrorCode = sem_wait((sem_t*)pSem)))
        {
            ERR("Semaphore 0x%"DTC_MS_U32_x" wait error:%"DTC_MS_S32_d"", (MMSDK_U32)pSem, s32ErrorCode);
            return FALSE;
        }
        else
        {
            return TRUE;
        }
    }
    return FALSE;
}

MMSDK_BOOL PT_MsOS_Sem_TryWait(PT_SEMAPHOREITEM pSem, MMSDK_S32 *s32Errno)
{
    FLOW("");
    MMSDK_S32 s32ErrorCode = 0;

    if (pSem)
    {
        if ((s32ErrorCode = sem_trywait((sem_t*)pSem)))
        {
            ERR("Semaphore 0x%"DTC_MS_U32_x" try wait error:%"DTC_MS_S32_d"", (MMSDK_U32)pSem, s32ErrorCode);
            *s32Errno = (MMSDK_S32)errno;
            return FALSE;
        }
        else
        {
            return TRUE;
        }
    }
    return FALSE;
}

/* FIXME: The sem_timedwait isn't support CLOCK_MONOTONIC,
 *     use relavite time with real-time clock instead now.
 * The NTP maybe make the sem_timedwait abnormal.
 */
MMSDK_BOOL PT_MsOS_Sem_TimedWait(PT_SEMAPHOREITEM pSem, MMSDK_U32 u32RelatvieWaitTimeMs)
{
    FLOW("");
    MMSDK_S32 s32ErrorCode = 0;

    struct timespec stTime;
    memset(&stTime, 0, sizeof(struct timespec));
    struct timeval now;
    memset(&now, 0, sizeof(struct timeval));

    if (pSem)
    {
        gettimeofday(&now, NULL);
        FLOW("pRelatvieWaitTimeMs = %"DTC_MS_U32_d"\n", u32RelatvieWaitTimeMs);
        stTime.tv_sec = now.tv_sec;
        stTime.tv_nsec = now.tv_usec * 1000;
        FLOW("Now time = %"DTC_MS_S64_d".%.9ld\n", (MMSDK_S64)(stTime.tv_sec), stTime.tv_nsec);

        // Convert u32RelatvieWaitTimeMs to abs time
        stTime.tv_sec += u32RelatvieWaitTimeMs / 1000;
        stTime.tv_nsec += (u32RelatvieWaitTimeMs % 1000) * 1000000;
        if (stTime.tv_nsec >= 1000000000)
        {
            stTime.tv_nsec -= 1000000000;
            stTime.tv_sec  += 1;
        }
        FLOW("Absolute wait time = %"DTC_MS_S64_d".%.3ld\n", (MMSDK_S64)(stTime.tv_sec), stTime.tv_nsec);
        s32ErrorCode = sem_timedwait((sem_t*)pSem, &stTime);

        if (s32ErrorCode == 0)
        {
            return TRUE;
        }
        else if (s32ErrorCode == ETIMEDOUT)
        {
            FLOW("Wait until timeout\n");
            return FALSE;
        }
        else
        {
            ERR("Semaphore 0x%"DTC_MS_U32_x" abs time %"DTC_MS_S64_d".%.3ld wait error:%"DTC_MS_S32_d"", (MMSDK_U32)pSem, (MMSDK_S64)(stTime.tv_sec), stTime.tv_nsec, s32ErrorCode);
            return FALSE;
        }
    }
    return FALSE;
}

MMSDK_BOOL PT_MsOS_Sem_GetValue(PT_SEMAPHOREITEM pSem, MMSDK_S32 *ps32SemVal)
{
    FLOW("");
    MMSDK_S32 s32ErrorCode = 0;
    int intSemVal = 0;

    if (pSem)
    {
        if ((s32ErrorCode = sem_getvalue((sem_t*)pSem, &intSemVal)))
        {
            ERR("Semaphore 0x%"DTC_MS_U32_x" get value error, code: %"DTC_MS_S32_d"", (MMSDK_U32)pSem, s32ErrorCode);
            return FALSE;
        }
        else
        {
            *ps32SemVal = intSemVal;
            return TRUE;
        }
    }
    return FALSE;
}

#endif
