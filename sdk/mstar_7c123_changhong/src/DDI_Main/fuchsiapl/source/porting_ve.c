#include <stdio.h>
#include "MsTypes.h"
#include "drvTVEncoder.h"
#include "porting_ve.h"

#ifdef MS_DEBUG
#define MPL_VE_TRACE(fmt, args...)      printf("[%s][%d]"fmt, __FUNCTION__, __LINE__, ##args)
#define MPL_VE_DBG(fmt, args...)        printf("[%s][%d]"fmt, __FUNCTION__, __LINE__, ##args)
#define MPL_VE_ERR(fmt, args...)        printf("[%s][%d]"fmt, __FUNCTION__, __LINE__, ##args)
#else
#define MPL_VE_TRACE(fmt, args...)      //printf("[%s][%d]"fmt, __FUNCTION__, __LINE__, ##args)
#define MPL_VE_DBG(fmt, args...)        //printf("[%s][%d]"fmt, __FUNCTION__, __LINE__, ##args)
#define MPL_VE_ERR(fmt, args...)        printf("[%s][%d]"fmt, __FUNCTION__, __LINE__, ##args)
#endif
MS_BOOL MPL_VE_GetTtxReadDoneStatus(void)
{
    MPL_VE_TRACE("Entered.\n");

    return MDrv_VE_GetTtxReadDoneStatus();
}

void MPL_VE_ClearTtxReadDoneStatus(void)
{
    MPL_VE_TRACE("Entered.\n");

    MDrv_VE_ClearTtxReadDoneStatus();
}

void MPL_VE_SetTtxBuffer(MS_U32 u32StartAddr, MS_U32 u32Size)
{
    MPL_VE_TRACE("Entered.\n");

    MPL_VE_DBG("StartAddr %ld Size %ld\n", u32StartAddr, u32Size);
    MDrv_VE_SetTtxBuffer(u32StartAddr, u32Size);
}

void MPL_VE_EnableTtx(MS_BOOL bEnable)
{
    MPL_VE_TRACE("Entered.\n");

    MDrv_VE_EnableTtx(bEnable);
}
