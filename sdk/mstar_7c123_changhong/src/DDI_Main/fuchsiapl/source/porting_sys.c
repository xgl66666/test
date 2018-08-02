#include <stdio.h>
#include "MsTypes.h"
#include "MsOS.h"
#include "porting_sys.h"
#include "asmCPU.h"

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#ifdef MS_DEBUG
#define MPL_SYS_TRACE(fmt, args...)     printf("[%s][%d]"fmt, __FUNCTION__, __LINE__, ##args)
#define MPL_SYS_DBG(fmt, args...)       printf("[%s][%d]"fmt, __FUNCTION__, __LINE__, ##args)
#define MPL_SYS_ERR(fmt, args...)       printf("[%s][%d]"fmt, __FUNCTION__, __LINE__, ##args)
#else
#define MPL_SYS_TRACE(fmt, args...)     //printf("[%s][%d]"fmt, __FUNCTION__, __LINE__, ##args)
#define MPL_SYS_DBG(fmt, args...)       //printf("[%s][%d]"fmt, __FUNCTION__, __LINE__, ##args)
#define MPL_SYS_ERR(fmt, args...)       printf("[%s][%d]"fmt, __FUNCTION__, __LINE__, ##args)
#endif

static MS_S32 gs32CachedPoolID = -1;
static MS_S32 gs32NonCachedPoolID = -1;

MS_S32 MPL_SYS_CreateTask(TaskEntry pTaskEntry,
                        MS_U32 u32TaskEntryData,
                        TaskPriority eTaskPriority,
                        MS_BOOL bAutoStart,
                        void *pStackEntry,
                        MS_U32 u32StackSize,
                        char *pTaskNam)
{
    MS_S32 TaskId = -1;

    MPL_SYS_TRACE("Entered.\n");

    if (pTaskNam == NULL)
    {
        MPL_SYS_ERR("Invalid task name\n");
        return TaskId;
    }

#ifdef MSOS_TYPE_ECOS
    if(pStackEntry == NULL || u32StackSize == 0)
    {
        MPL_SYS_ERR("Invalid parameter\n");
        return TaskId;
    }
#endif

    TaskId = MsOS_CreateTask(pTaskEntry, u32TaskEntryData, eTaskPriority, bAutoStart, pStackEntry, u32StackSize, pTaskNam);
    return TaskId;
}

MS_BOOL MPL_SYS_DeleteTask(MS_S32 s32TaskId)
{
    MPL_SYS_TRACE("Entered.\n");

    if (s32TaskId < 0)
    {
        MPL_SYS_ERR("Invalid parameter\n");
        return FALSE;
    }

    return MsOS_DeleteTask(s32TaskId);
}

void MPL_SYS_DelayTask(MS_U32 u32Ms)
{
    MPL_SYS_TRACE("Entered.\n");

    MsOS_DelayTask(u32Ms);
}

MS_S32 MPL_SYS_CreateEvent(char *pName)
{
    MS_S32 EventId = -1;

    MPL_SYS_TRACE("Entered.\n");

    if (pName == NULL)
    {
        MPL_SYS_ERR("Invalid parameter\n");
        return EventId;
    }

    EventId = MsOS_CreateEventGroup(pName);
    return EventId;
}

MS_BOOL MPL_SYS_DeleteEvent(MS_S32 s32EventGroupId)
{
    MPL_SYS_TRACE("Entered.\n");

    if (s32EventGroupId < 0)
    {
        MPL_SYS_ERR("Invalid parameter\n");
        return FALSE;
    }

    return MsOS_DeleteEventGroup(s32EventGroupId);
}

MS_BOOL MPL_SYS_SetEvent(MS_S32 s32EventGroupId, MS_U32 u32EventFlag)
{
    MPL_SYS_TRACE("Entered.\n");

    if (s32EventGroupId < 0)
    {
        MPL_SYS_ERR("Invalid parameter\n");
        return FALSE;
    }

    return MsOS_SetEvent(s32EventGroupId, u32EventFlag);
}

MS_BOOL MPL_SYS_ClearEvent(MS_S32 s32EventGroupId, MS_U32 u32EventFlag)
{
    MPL_SYS_TRACE("Entered.\n");

    if (s32EventGroupId < 0)
    {
        MPL_SYS_ERR("Invalid parameter\n");
        return FALSE;
    }

    return MsOS_ClearEvent(s32EventGroupId, u32EventFlag);
}

MS_BOOL MPL_SYS_WaitEvent(MS_S32 s32EventGroupId,
                        MS_U32 u32WaitEventFlag,
                        MS_U32 *pu32RetrievedEventFlag,
                        EventWaitMode eWaitMode,
                        MS_U32 u32WaitMs)
{
    MPL_SYS_TRACE("Entered.\n");

    if ((s32EventGroupId < 0) || (pu32RetrievedEventFlag == NULL))
    {
        MPL_SYS_ERR("Invalid parameter\n");
        return FALSE;
    }

    return MsOS_WaitEvent(s32EventGroupId, u32WaitEventFlag, pu32RetrievedEventFlag, eWaitMode, u32WaitMs);
}

MS_S32 MPL_SYS_CreateMutex(MsOSAttribute eAttribute, char *pMutexName, MS_U32 u32Flag)
{
    MS_S32 MutexId = -1;

    MPL_SYS_TRACE("Entered.\n");

    if (pMutexName == NULL)
    {
        MPL_SYS_ERR("Invalid parameter\n");
        return MutexId;
    }

    MutexId = MsOS_CreateMutex(eAttribute, pMutexName, u32Flag);
    return MutexId;
}

MS_BOOL MPL_SYS_DeleteMutex(MS_S32 s32MutexId)
{
    MPL_SYS_TRACE("Entered.\n");

    if (s32MutexId < 0)
    {
        MPL_SYS_ERR("Invalid parameter\n");
        return FALSE;
    }

    return MsOS_DeleteMutex(s32MutexId);
}

MS_BOOL MPL_SYS_LockMutex(MS_S32 s32MutexId, MS_U32 u32WaitMs)
{
    MPL_SYS_TRACE("Entered.\n");

    if (s32MutexId < 0)
    {
        MPL_SYS_ERR("Invalid parameter\n");
        return FALSE;
    }

    return MsOS_ObtainMutex(s32MutexId, u32WaitMs);
}

MS_BOOL MPL_SYS_UnlockMutex(MS_S32 s32MutexId)
{
    MPL_SYS_TRACE("Entered.\n");

    if (s32MutexId < 0)
    {
        MPL_SYS_ERR("Invalid parameter\n");
        return FALSE;
    }

    return MsOS_ReleaseMutex(s32MutexId);
}

MS_U32 MPL_SYS_GetSystemTime(void)
{
    MPL_SYS_TRACE("Entered.\n");

    return MsOS_GetSystemTime();
}

MS_U8 MPL_SYS_CPU_GetTrailOne(MS_U32 u32Flags)
{
    MPL_SYS_TRACE("Entered.\n");

    return MAsm_CPU_GetTrailOne(u32Flags);
}

MS_BOOL MPL_SYS_SetMemPoolID(MS_S32 s32CachePoolID, MS_S32 s32NonCachePoolID)
{
    MPL_SYS_TRACE("Entered.\n");

    if ((s32CachePoolID < 0) || (s32NonCachePoolID < 0))
    {
        MPL_SYS_ERR("Invalid parameter : Set Memory pool ID\n");
        return FALSE;
    }

    gs32CachedPoolID = s32CachePoolID;
    gs32NonCachedPoolID = s32NonCachePoolID;

    return TRUE;
}

void *MPL_SYS_AllocateMemory (MS_U32 u32Size, eMPL_SYS_MEM_TYPE eMemType)
{
    MPL_SYS_TRACE("Entered.\n");

    if ((gs32CachedPoolID < 0) || (gs32NonCachedPoolID < 0))
    {
        MPL_SYS_ERR("Invalid parameter : init cache pool\n");
        return NULL;
    }

    switch(eMemType)
    {
        case E_MEM_TYPE_CACHED:
            return MsOS_AllocateMemory(u32Size, gs32CachedPoolID);

        case E_MEM_TYPE_NONCACHED:
            return MsOS_AllocateMemory(u32Size, gs32NonCachedPoolID);

        default:
            return NULL;
    }
}

MS_BOOL MPL_SYS_FreeMemory (void *pAddress, eMPL_SYS_MEM_TYPE eMemType)
{
    MPL_SYS_TRACE("Entered.\n");

    if ((gs32CachedPoolID < 0) || (gs32NonCachedPoolID < 0))
    {
        MPL_SYS_ERR("Invalid parameter : init cache pool\n");
        return NULL;
    }

    switch(eMemType)
    {
        case E_MEM_TYPE_CACHED:
            return MsOS_FreeMemory(pAddress, gs32CachedPoolID);

        case E_MEM_TYPE_NONCACHED:
            return MsOS_FreeMemory(pAddress, gs32NonCachedPoolID);

        default:
            return FALSE;
    }
}

MS_BOOL MPL_SYS_FlushMemory(MS_U32 u32Start , MS_U32 u32Size)
{
    MS_BOOL bRet = TRUE;

    MPL_SYS_TRACE("Entered.\n");

    if (MsOS_Dcache_Flush(u32Start, u32Size))
    {
        MsOS_FlushMemory();
    }
    else
    {
        bRet = FALSE;
    }

    return bRet;
}

MS_U32 MPL_SYS_VA2PA(MS_U32 addr)
{
    MPL_SYS_TRACE("Entered.\n");

    return MsOS_VA2PA(addr);
}

MS_U32 MPL_SYS_PA2VA(MS_U32 addr, eMPL_SYS_MEM_TYPE eMemType)
{
    MPL_SYS_TRACE("Entered.\n");

    switch(eMemType)
    {
        case E_MEM_TYPE_CACHED:
            return MsOS_PA2KSEG0(addr);

        case E_MEM_TYPE_NONCACHED:
            return MsOS_PA2KSEG1(addr);

        default:
            return FALSE;
    }
}

MS_BOOL MPL_SYS_GetTaskStatus (Task_Info* pTaskInfo, TaskStatus *peTaskState)
{
    return MsOS_GetTaskStatus(pTaskInfo, peTaskState);
}
