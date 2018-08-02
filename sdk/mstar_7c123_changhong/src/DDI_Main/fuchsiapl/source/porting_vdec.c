#include <stdio.h>
#include "MsTypes.h"
#include "apiVDEC_EX.h"
#include "porting_vdec.h"

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#ifdef MS_DEBUG
#define MPL_VDEC_TRACE(fmt, args...)     printf("[%s][%d]"fmt, __FUNCTION__, __LINE__, ##args)
#define MPL_VDEC_DBG(fmt, args...)       printf("[%s][%d]"fmt, __FUNCTION__, __LINE__, ##args)
#define MPL_VDEC_ERR(fmt, args...)       printf("[%s][%d]"fmt, __FUNCTION__, __LINE__, ##args)
#else
#define MPL_VDEC_TRACE(fmt, args...)     //printf("[%s][%d]"fmt, __FUNCTION__, __LINE__, ##args)
#define MPL_VDEC_DBG(fmt, args...)       //printf("[%s][%d]"fmt, __FUNCTION__, __LINE__, ##args)
#define MPL_VDEC_ERR(fmt, args...)       printf("[%s][%d]"fmt, __FUNCTION__, __LINE__, ##args)
#endif
#define MPL_U32_MAX                      0xffffffffUL

VDEC_EX_Result MPL_VDEC_SetFreezeDisp(VDEC_StreamId *pStreamId, MS_BOOL bEnable)
{
    MPL_VDEC_TRACE("Entered.\n");

    if (pStreamId == NULL)
    {
        MPL_VDEC_ERR("Invalid parameter\n");
        return E_VDEC_EX_RET_INVALID_PARAM;
    }

    return MApi_VDEC_EX_SetFreezeDisp(pStreamId, bEnable);
}

MS_U32 MPL_VDEC_GetESWritePtr(VDEC_StreamId *pStreamId)
{
    MS_U32 u32Ptr = 0;

    MPL_VDEC_TRACE("Entered.\n");

    if (pStreamId == NULL)
    {
        MPL_VDEC_ERR("Invalid parameter\n");
        return u32Ptr;
    }

    u32Ptr = MApi_VDEC_EX_GetESWritePtr(pStreamId);
    return u32Ptr;
}

MS_U32 MPL_VDEC_GetESReadPtr(VDEC_StreamId *pStreamId)
{
    MS_U32 u32Ptr = 0;

    MPL_VDEC_TRACE("Entered.\n");

    if (pStreamId == NULL)
    {
        MPL_VDEC_ERR("Invalid parameter\n");
        return u32Ptr;
    }

    u32Ptr = MApi_VDEC_EX_GetESReadPtr(pStreamId);
    return u32Ptr;
}

VDEC_EX_Result MPL_VDEC_GetControl(VDEC_StreamId *pStreamId, VDEC_EX_User_Cmd cmd_id, MS_U32 *param)
{
    MPL_VDEC_TRACE("Entered.\n");

    if ((pStreamId == NULL) || (param == NULL))
    {
        MPL_VDEC_ERR("Invalid parameter\n");
        return E_VDEC_EX_RET_INVALID_PARAM;
    }

    return MApi_VDEC_EX_GetControl(pStreamId, cmd_id, param);
}

VDEC_EX_Result MPL_VDEC_GetDispInfo(VDEC_StreamId *pStreamId, VDEC_EX_User_Cmd cmd_id, VDEC_EX_DispInfo *pDispinfo)
{
    VDEC_StreamId stVDECStreamId = {0};
    VDEC_EX_Result eRet;

    MPL_VDEC_TRACE("Entered.\n");

    if (pDispinfo == NULL)
    {
        MPL_VDEC_ERR("Invalid parameter\n");
        return E_VDEC_EX_RET_INVALID_PARAM;
    }

    if(pStreamId == NULL)
    {
        if((cmd_id != E_VDEC_EX_USER_CMD_GET_MAIN_STREAM_ID) && (cmd_id != E_VDEC_EX_USER_CMD_GET_SUB_STREAM_ID))
        {
            MPL_VDEC_ERR("Invalid parameter\n");
            return E_VDEC_EX_RET_INVALID_PARAM;
        }

        if((eRet = MApi_VDEC_EX_GetControl(&stVDECStreamId, cmd_id, (MS_U32*)&stVDECStreamId)) != E_VDEC_EX_OK)
        {
            MPL_VDEC_ERR("Fail to get stream ID\n");
            return eRet;
        }

        pStreamId = &stVDECStreamId;
    }

    return MApi_VDEC_EX_GetDispInfo(pStreamId, pDispinfo);
}

VDEC_EX_Result MPL_VDEC_CC_Init(VDEC_StreamId *pStreamId, MS_U32 *pData)
{
    MPL_VDEC_TRACE("Entered.\n");

    if (pStreamId == NULL)
    {
        MPL_VDEC_ERR("Invalid parameter\n");
        return E_VDEC_EX_RET_INVALID_PARAM;
    }

    return MApi_VDEC_EX_CC_Init(pStreamId, pData);
}

VDEC_EX_Result MPL_VDEC_CC_DisableParsing(VDEC_StreamId *pStreamId, VDEC_EX_CCFormat eFmt)
{
    MPL_VDEC_TRACE("Entered.\n");

    if (pStreamId == NULL)
    {
        MPL_VDEC_ERR("Invalid parameter\n");
        return E_VDEC_EX_RET_INVALID_PARAM;
    }

    return MApi_VDEC_EX_CC_DisableParsing(pStreamId, eFmt);
}

VDEC_EX_Result MPL_VDEC_CC_SetCfg(VDEC_StreamId *pStreamId, VDEC_EX_CCFormat eFmt, VDEC_EX_CCType eType, MS_U32 *pData)
{
    MPL_VDEC_TRACE("Entered.\n");

    if (pStreamId == NULL)
    {
        MPL_VDEC_ERR("Invalid parameter\n");
        return E_VDEC_EX_RET_INVALID_PARAM;
    }

    return MApi_VDEC_EX_CC_SetCfg(pStreamId, eFmt, eType, pData);
}

MS_PHYADDR MPL_VDEC_CC_GetBuffAddr(VDEC_StreamId *pStreamId, VDEC_EX_CCFormat eFmt, MPL_VDEC_BUFF_CMD cmd)
{
    MS_PHYADDR u32PhyAddr = MPL_U32_MAX;

    MPL_VDEC_TRACE("Entered.\n");

    if (pStreamId == NULL)
    {
        MPL_VDEC_ERR("Invalid parameter\n");
        return u32PhyAddr;
    }

    switch(cmd)
    {
        case E_CMD_GET_CC_WRITE_ADDR:
            u32PhyAddr = MApi_VDEC_EX_CC_GetWriteAdd(pStreamId, eFmt);
            break;

        case E_CMD_GET_CC_READ_ADDR:
            u32PhyAddr = MApi_VDEC_EX_CC_GetReadAdd(pStreamId, eFmt);
            break;

        default:
            MPL_VDEC_ERR("Invalid parameter\n");
    }

    return u32PhyAddr;
}

VDEC_EX_Result MPL_VDEC_CC_SetBuffAddr(VDEC_StreamId *pStreamId, VDEC_EX_CCFormat eFmt, MPL_VDEC_BUFF_CMD cmd, MS_U32 *pData)
{
    VDEC_EX_Result retVal = E_VDEC_EX_FAIL;

    MPL_VDEC_TRACE("Entered.\n");

    if (pStreamId == NULL)
    {
        MPL_VDEC_ERR("Invalid parameter\n");
        return E_VDEC_EX_RET_INVALID_PARAM;
    }

    switch(cmd)
    {
        case E_CMD_UPDATE_CC_WRITE_ADDR:
            retVal = MApi_VDEC_EX_CC_UpdateWriteAdd(pStreamId, eFmt, pData);
            break;

        case E_CMD_UPDATE_CC_READ_ADDR:
            retVal = MApi_VDEC_EX_CC_UpdateReadAdd(pStreamId, eFmt, pData);
            break;

        case E_CMD_SET_CC_START_ADDR:
            retVal = MApi_VDEC_EX_CC_SetBuffStartAdd(pStreamId, eFmt, pData);
            break;

        default:
            MPL_VDEC_ERR("Invalid parameter\n");
            retVal = E_VDEC_EX_RET_INVALID_PARAM;
    }

    return retVal;
}

MS_BOOL MPL_VDEC_GetStatus(VDEC_StreamId *pStreamId, VDEC_EX_Status *pStatus)
{
    MPL_VDEC_TRACE("Entered.\n");

    if ((pStreamId == NULL) || (pStatus == NULL))
    {
        MPL_VDEC_ERR("Invalid parameter\n");
        return FALSE;
    }

    return MApi_VDEC_EX_GetStatus(pStreamId, pStatus);
}

VDEC_EX_Result MPL_VDEC_CC_GetIsBuffOverflow(VDEC_StreamId *pStreamId, VDEC_EX_CCFormat eFmt)
{
    MPL_VDEC_TRACE("Entered.\n");

    if (pStreamId == NULL)
    {
        MPL_VDEC_ERR("Invalid parameter\n");
        return E_VDEC_EX_RET_INVALID_PARAM;
    }

    return MApi_VDEC_EX_CC_GetIsBuffOverflow(pStreamId, eFmt);
}

VDEC_EX_Result MPL_VDEC_GetCCInfo(VDEC_StreamId *pStreamId, void *pInfo, MS_U32 u32Size)
{
    MPL_VDEC_TRACE("Entered.\n");

    if ((pStreamId == NULL) || (pInfo == NULL))
    {
        MPL_VDEC_ERR("Invalid parameter\n");
        return E_VDEC_EX_RET_INVALID_PARAM;
    }

    return MApi_VDEC_EX_GetCCInfo(pStreamId, pInfo, u32Size);
}
