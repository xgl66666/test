#include "MsOS.h"
#include "MsTypes.h"
#include "porting.h"
#include "porting_sysinfo.h"
#include "mmsdk_interface_def.h"
#include <stdio.h>
#include <string.h>
#include "drvDTC.h"

#define FLOW(fmt, arg...)         //printf("\033[1;33m######[%s]######"fmt" \033[0m\n",__FUNCTION__,##arg)

PT_SYS_MmapInfo *stMmapInfo = NULL;

MMSDK_BOOL PT_MMAP_Initialize(void)
{
    FLOW("%s", "Entered.");
    return TRUE;
}

MMSDK_BOOL PT_MMAP_SetBuffer(const EN_MMSDK_BUF_ID eBufferID, const ST_MMSDK_BUF_INFO* pstMemInfo)
{
    FLOW("%s", "Entered.");
    if(!pstMemInfo)
        return FALSE;

    return TRUE;
}

MMSDK_BOOL PT_MMAP_GetBuffer(const EN_MMSDK_BUF_ID eBufferID, ST_MMSDK_BUF_INFO* pstMemInfo)
{
    FLOW("%s", "Entered.");
    if (!pstMemInfo)
    {
        FLOW("Error!! pstMemInfo=%p\n", pstMemInfo);
        return FALSE;
    }

    if(PT_SYS_GetMmapInfo(pstMemInfo, eBufferID, 0) != 0)
        return TRUE;
    else
        return FALSE;
}

MMSDK_U32 PT_MMAP_GetBufferInfo(const EN_MMSDK_BUF_ID eBufferID, const EN_MMSDK_BUF_INFO eInfo)
{
    MMSDK_U32 u32Ret = 0;
    FLOW("%s", "Entered.");

    u32Ret = PT_SYS_GetMmapInfo(NULL, eBufferID, eInfo);

    FLOW("eBufferID=%d, eInfo=%d, u32Ret=0x%"DTC_MS_U32_x"\n", eBufferID, eInfo, u32Ret);
    return u32Ret;
}

MMSDK_BOOL  PT_MMAP_GetMminfo(ST_MMSDK_MMAP_INFO* pInfo)
{
    FLOW("%s", "Entered.");
    if(!pInfo)
        return FALSE;

    PT_SYS_GetMIUInfo(pInfo);

    return  TRUE;
}

MMSDK_BOOL PT_MMAP_Finalize(void)
{
    FLOW("%s", "Entered.");
    return TRUE;
}

