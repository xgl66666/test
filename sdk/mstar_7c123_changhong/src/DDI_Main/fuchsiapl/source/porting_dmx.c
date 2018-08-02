#include <stdio.h>
#include "MsTypes.h"
#include "apiDMX.h"
#include "porting_dmx.h"

#ifdef MS_DEBUG
#define MPL_DMX_TRACE(fmt, args...)     printf("[%s][%d]"fmt, __FUNCTION__, __LINE__, ##args)
#define MPL_DMX_DBG(fmt, args...)       printf("[%s][%d]"fmt, __FUNCTION__, __LINE__, ##args)
#define MPL_DMX_ERR(fmt, args...)       printf("[%s][%d]"fmt, __FUNCTION__, __LINE__, ##args)
#else
#define MPL_DMX_TRACE(fmt, args...)     //printf("[%s][%d]"fmt, __FUNCTION__, __LINE__, ##args)
#define MPL_DMX_DBG(fmt, args...)       //printf("[%s][%d]"fmt, __FUNCTION__, __LINE__, ##args)
#define MPL_DMX_ERR(fmt, args...)       printf("[%s][%d]"fmt, __FUNCTION__, __LINE__, ##args)
#endif

MS_BOOL MPL_DMX_Start(MS_U8 u8DmxId)
{
    MPL_DMX_TRACE("Entered.\n");

    if(DMX_FILTER_STATUS_OK == MApi_DMX_Start(u8DmxId))
    {
        return TRUE;
    }
    else
    {
        MPL_DMX_ERR("DMX Start Fail!\n");
        return FALSE;
    }
}

MS_BOOL MPL_DMX_Stop(MS_U8 u8DmxId)
{
    MPL_DMX_TRACE("Entered.\n");
    if(DMX_FILTER_STATUS_OK == MApi_DMX_Stop(u8DmxId))
    {
        return TRUE;
    }
    else
    {
        MPL_DMX_ERR("DMX Start Fail!\n");
        return FALSE;
    }
}

MS_BOOL MPL_DMX_Open(MPL_DMX_FILTER_TYPE DmxFltType, MS_U8* u8DmxId)
{
    MPL_DMX_TRACE("Entered.\n");
    if(DMX_FILTER_STATUS_OK == MApi_DMX_Open((DMX_FILTER_TYPE)DmxFltType, u8DmxId))
    {
        return TRUE;
    }
    else
    {
        MPL_DMX_ERR("DMX Open Fail!\n");
        return FALSE;
    }
}

MS_BOOL MPL_DMX_Close(MS_U8 u8DmxId)
{
    MPL_DMX_TRACE("Entered.\n");
    if(DMX_FILTER_STATUS_OK == MApi_DMX_Close(u8DmxId))
    {
        return TRUE;
    }
    else
    {
        MPL_DMX_ERR("DMX close Fail!\n");
        return FALSE;
    }

}

MS_BOOL MPL_DMX_IsStart(MS_U8 u8DmxId, MS_BOOL* pbEnable)
{
    MPL_DMX_TRACE("Entered.\n");
    if(DMX_FILTER_STATUS_OK == MApi_DMX_IsStart(u8DmxId, pbEnable))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }

}

MS_BOOL MPL_DMX_SectReadSet(MS_U8 u8DmxId, MS_PHYADDR Read)
{
    MPL_DMX_TRACE("Entered.\n");
    if(DMX_FILTER_STATUS_OK == MApi_DMX_SectReadSet(u8DmxId, Read))
    {
        return TRUE;
    }
    else
    {
        MPL_DMX_ERR("Reset DMX filer buffer Fail!\n");
        return FALSE;
    }
}

MS_BOOL MPL_DMX_Stc_Get(MS_U32* pu32Stc32, MS_U32* pu32Stc)
{
    MPL_DMX_TRACE("Entered.\n");
    if(DMX_FILTER_STATUS_OK == MApi_DMX_Stc_Get(pu32Stc32, pu32Stc))
    {
        return TRUE;
    }
    else
    {
        MPL_DMX_ERR("Get STC counter fail!\n");
        return FALSE;
    }
}

MS_BOOL MPL_DMX_SectPatternSet(MS_U8 u8DmxId, MS_U8* pPattern, MS_U8* pMask, MS_U8 *pu8NotMask, MS_U32 MatchSize)
{
    MPL_DMX_TRACE("Entered.\n");
    if(DMX_FILTER_STATUS_OK == MApi_DMX_SectPatternSet(u8DmxId, pPattern, pMask, pu8NotMask, MatchSize))
    {
        return TRUE;
    }
    else
    {
        MPL_DMX_ERR("Get STC counter fail!\n");
        return FALSE;
    }
}

MS_BOOL MPL_DMX_Pid(MS_U8 u8DmxId, MS_U16* pu16Pid, MS_BOOL bSet)
{
    MPL_DMX_TRACE("Entered.\n");
    if(DMX_FILTER_STATUS_OK == MApi_DMX_Pid(u8DmxId, pu16Pid, bSet))
    {
        return TRUE;
    }
    else
    {
        MPL_DMX_ERR("Demux ID [%d] set demux filter PID fail!\n", u8DmxId);
        return FALSE;
    }
}

MS_BOOL MPL_DMX_FlowStart(MS_U8 *u8DmxId, MS_U32 uPid, MPL_DMX_FLT_INFO *pFltInfo)
{
    DMX_Flt_info    FltInfo;

    MPL_DMX_TRACE("Entered.\n");

    if (DMX_FILTER_STATUS_ERROR == MApi_DMX_Open(pFltInfo->eFltType, u8DmxId))
    {
        MPL_DMX_ERR("Open DMX fail\n");
        return FALSE;
    }

    FltInfo.Info.SectInfo.SectBufAddr =  pFltInfo->u32SectBufAddr;
    FltInfo.Info.SectInfo.SectBufSize =  pFltInfo->u32SectBufSize;
    FltInfo.Info.SectInfo.SectMode    =  pFltInfo->eSectMode;
    FltInfo.Info.SectInfo.Event       =  pFltInfo->eEvent;
    FltInfo.Info.SectInfo.pNotify     =  pFltInfo ->pNotify;
    FltInfo.Info.SectInfo.pType2Notify = pFltInfo ->pNotify2;
    FltInfo.Info.SectInfo.Type2NotifyParam1 = pFltInfo ->u32NotifyParam;

    if (DMX_FILTER_STATUS_OK != MApi_DMX_Info(*u8DmxId, &FltInfo, (MS_U32*)pFltInfo->eFltType, TRUE))
    {
        goto FLT_CREATE_ERR;
    }

    if (pFltInfo->pMatch != NULL)
    {
        if (DMX_FILTER_STATUS_OK != MApi_DMX_SectPatternSet(*u8DmxId, pFltInfo->pMatch, pFltInfo->pMask,
                                                                        pFltInfo->pNMask, pFltInfo->u32MatchSize))
        {
            goto FLT_CREATE_ERR;
        }
    }

    if (DMX_FILTER_STATUS_OK != MApi_DMX_Pid(*u8DmxId, (MS_U16 *)&uPid, TRUE))
    {
        goto FLT_CREATE_ERR;
    }

    MApi_DMX_SectReset(*u8DmxId);

    if (DMX_FILTER_STATUS_OK != MApi_DMX_Start(*u8DmxId))
    {
        goto FLT_CREATE_ERR ;
    }

    return TRUE;

FLT_CREATE_ERR:

    MPL_DMX_ERR("Filter Create Error ! \n");
    MApi_DMX_Close(*u8DmxId);

    return FALSE;
}

MS_BOOL MPL_DMX_Info(MS_U8 u8DmxId, MPL_DMX_Sect_info* pDmxSectInfo, MPL_DMX_FILTER_TYPE* pDmxFltType, MS_BOOL bSet)
{
    MPL_DMX_TRACE("Entered.\n");

    DMX_Flt_info    FltInfo;
    FltInfo.Info.SectInfo.SectBufAddr =  pDmxSectInfo->SectBufAddr;
    FltInfo.Info.SectInfo.SectBufSize =  pDmxSectInfo->SectBufSize;
    FltInfo.Info.SectInfo.SectMode    =  pDmxSectInfo->SectMode;
    FltInfo.Info.SectInfo.Event       =  pDmxSectInfo->Event;
    FltInfo.Info.SectInfo.pNotify     =  pDmxSectInfo->pNotify;
    FltInfo.Info.SectInfo.pType2Notify = pDmxSectInfo->pNotify2;
    FltInfo.Info.SectInfo.Type2NotifyParam1 = pDmxSectInfo ->u32NotifyParam;

    if(DMX_FILTER_STATUS_OK == MApi_DMX_Info(u8DmxId, &FltInfo, pDmxFltType, bSet))
    {
        return TRUE;
    }
    else
    {
        MPL_DMX_ERR("Demux ID [%d] get filter information fail\n", u8DmxId);
        return FALSE;
    }
}

MS_BOOL MPL_DMX_CopyData(MS_U8 u8DmxId, MS_U8* pu8Buf, MS_U32 u32BufSize, MS_U32* pu32ActualSize, MPL_DMX_CheckCb pfCheckCB)
{
    MPL_DMX_TRACE("Entered.\n");
    MS_U32 pu32RmnSize = 0;
    if(DMX_FILTER_STATUS_OK == MApi_DMX_CopyData(u8DmxId, pu8Buf, u32BufSize, pu32ActualSize, &pu32RmnSize, (DMX_CheckCb)pfCheckCB))
    {
        return TRUE;
    }
    else
    {
        MPL_DMX_DBG("Demux ID [%d] fail to copy data to buffer\n", u8DmxId);
        return FALSE;
    }
}

MS_BOOL MPL_DMX_ReStart(MS_U8 u8DmxId)
{
    MPL_DMX_TRACE("Entered.\n");

    if(DMX_FILTER_STATUS_ERROR == MApi_DMX_Stop(u8DmxId))
    {
        MPL_DMX_ERR("Demux ID [%d] stop fail\n", u8DmxId);
        return FALSE;
    }
    if(DMX_FILTER_STATUS_ERROR == MApi_DMX_SectReset(u8DmxId))
    {
        MPL_DMX_ERR("Demux ID [%d] Reset filer buffer fail\n", u8DmxId);
        return FALSE;
    }
    if(DMX_FILTER_STATUS_ERROR == MApi_DMX_Start(u8DmxId))
    {
        MPL_DMX_ERR("Demux ID [%d] start fail\n", u8DmxId);
        return FALSE;
    }
    return TRUE;

}

MS_BOOL MPL_DMX_SectReset(MS_U8 u8DmxId)
{
    MPL_DMX_TRACE("Entered.\n");
    if(DMX_FILTER_STATUS_OK== MApi_DMX_SectReset(u8DmxId))
    {
        return TRUE;
    }
    else
    {
        MPL_DMX_ERR("Demux ID [%d] Reset filer buffer fail\n", u8DmxId);
        return FALSE;
    }
}

MS_BOOL MPL_DMX_SectReadGet(MS_U8 u8DmxId, MS_PHYADDR* pRead)
{
    MPL_DMX_TRACE("Entered.\n");

    if (pRead == NULL)
    {
        MPL_DMX_ERR("Invalid parameter\n");
        return FALSE;
    }

    if (DMX_FILTER_STATUS_OK == MApi_DMX_SectReadGet(u8DmxId,pRead))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

MS_BOOL MPL_DMX_SectWriteGet(MS_U8 u8DmxId, MS_PHYADDR* pWrite)
{
    MPL_DMX_TRACE("Entered.\n");

    if (pWrite == NULL)
    {
        MPL_DMX_ERR("Invalid parameter\n");
        return FALSE;
    }

    if (DMX_FILTER_STATUS_OK == MApi_DMX_SectWriteGet(u8DmxId,pWrite))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

MS_BOOL MPL_DMX_SectStartGet(MS_U8 u8DmxId, MS_PHYADDR* pStart)
{
    MPL_DMX_TRACE("Entered.\n");

    if (pStart == NULL)
    {
        MPL_DMX_ERR("Invalid parameter\n");
        return FALSE;
    }

    if (DMX_FILTER_STATUS_OK == MApi_DMX_SectStartGet(u8DmxId,pStart))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

MS_BOOL MPL_DMX_SectEndGet(MS_U8 u8DmxId, MS_PHYADDR* pEnd)
{
    MPL_DMX_TRACE("Entered.\n");

    if (pEnd == NULL)
    {
        MPL_DMX_ERR("Invalid parameter\n");
        return FALSE;
    }

    if (DMX_FILTER_STATUS_OK == MApi_DMX_SectEndGet(u8DmxId,pEnd))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

MS_BOOL MPL_DMX_DataLenGet(MS_U8 u8DmxId, MS_U32 *u32data_length)
{
    MS_PHYADDR pRead, pWrite, pStart, pEnd;

    MPL_DMX_TRACE("Entered.\n");
    if(u32data_length == NULL)
    {
        return FALSE;
    }

    if(DMX_FILTER_STATUS_ERROR == MApi_DMX_SectReadGet(u8DmxId, &pRead))
    {
        MPL_DMX_ERR("Demux ID [%d] get read pointer of demux filer buffer fail\n", u8DmxId);
        return FALSE;
    }
    if(DMX_FILTER_STATUS_ERROR == MApi_DMX_SectWriteGet(u8DmxId, &pWrite))
    {
        MPL_DMX_ERR("Demux ID [%d] get write pointer of demux filer buffer fail\n", u8DmxId);
        return FALSE;
    }
    if(DMX_FILTER_STATUS_ERROR == MApi_DMX_SectStartGet(u8DmxId, &pStart))
    {
        MPL_DMX_ERR("Demux ID [%d] get start pointer of demux filer buffer fail\n", u8DmxId);
        return FALSE;
    }
    if(DMX_FILTER_STATUS_ERROR == MApi_DMX_SectEndGet(u8DmxId, &pEnd))
    {
        MPL_DMX_ERR("Demux ID [%d] get end pointer of demux filer buffer fail\n", u8DmxId);
        return FALSE;
    }

    *u32data_length = (pRead < pWrite) ? (pWrite - pRead) : ((pEnd - pRead) + (pWrite - pStart));
    return TRUE;
}

MS_BOOL MPL_DMX_Get_FlowInput_Status(DMX_FLOW DmxFlow, DMX_FLOW_INPUT *pDmxFlowInput, MS_BOOL *pbClkInv, MS_BOOL *pbExtSync, MS_BOOL *pbParallel)
{
    MS_BOOL bRet = TRUE;

    MPL_DMX_TRACE("Entered.\n");

    if ((pDmxFlowInput == NULL) || (pbClkInv == NULL) || (pbExtSync == NULL) || (pbParallel == NULL))
    {
        MPL_DMX_ERR("Invalid parameter\n");
        bRet = FALSE;
        return bRet;
    }

    if (DMX_FILTER_STATUS_ERROR == MApi_DMX_Get_FlowInput_Status(DmxFlow, pDmxFlowInput, pbClkInv, pbExtSync, pbParallel))
    {
        bRet = FALSE;
    }

    return bRet;
}
