//<MStar Software>
//******************************************************************************
// MStar Software
// Copyright (c) 2010 - 2012 MStar Semiconductor, Inc. All rights reserved.
// All software, firmware and related documentation herein ("MStar Software") are
// intellectual property of MStar Semiconductor, Inc. ("MStar") and protected by
// law, including, but not limited to, copyright law and international treaties.
// Any use, modification, reproduction, retransmission, or republication of all
// or part of MStar Software is expressly prohibited, unless prior written
// permission has been granted by MStar.
//
// By accessing, browsing and/or using MStar Software, you acknowledge that you
// have read, understood, and agree, to be bound by below terms ("Terms") and to
// comply with all applicable laws and regulations:
//
// 1. MStar shall retain any and all right, ownership and interest to MStar
//    Software and any modification/derivatives thereof.
//    No right, ownership, or interest to MStar Software and any
//    modification/derivatives thereof is transferred to you under Terms.
//
// 2. You understand that MStar Software might include, incorporate or be
//    supplied together with third party`s software and the use of MStar
//    Software may require additional licenses from third parties.
//    Therefore, you hereby agree it is your sole responsibility to separately
//    obtain any and all third party right and license necessary for your use of
//    such third party`s software.
//
// 3. MStar Software and any modification/derivatives thereof shall be deemed as
//    MStar`s confidential information and you agree to keep MStar`s
//    confidential information in strictest confidence and not disclose to any
//    third party.
//
// 4. MStar Software is provided on an "AS IS" basis without warranties of any
//    kind. Any warranties are hereby expressly disclaimed by MStar, including
//    without limitation, any warranties of merchantability, non-infringement of
//    intellectual property rights, fitness for a particular purpose, error free
//    and in conformity with any international standard.  You agree to waive any
//    claim against MStar for any loss, damage, cost or expense that you may
//    incur related to your use of MStar Software.
//    In no event shall MStar be liable for any direct, indirect, incidental or
//    consequential damages, including without limitation, lost of profit or
//    revenues, lost or damage of data, and unauthorized system use.
//    You agree that this Section 4 shall still apply without being affected
//    even if MStar Software has been modified by MStar in accordance with your
//    request or instruction for your use, except otherwise agreed by both
//    parties in writing.
//
// 5. If requested, MStar may from time to time provide technical supports or
//    services in relation with MStar Software to you for your use of
//    MStar Software in conjunction with your or your customer`s product
//    ("Services").
//    You understand and agree that, except otherwise agreed by both parties in
//    writing, Services are provided on an "AS IS" basis and the warranty
//    disclaimer set forth in Section 4 above shall apply.
//
// 6. Nothing contained herein shall be construed as by implication, estoppels
//    or otherwise:
//    (a) conferring any license or right to use MStar name, trademark, service
//        mark, symbol or any other identification;
//    (b) obligating MStar or any of its affiliates to furnish any person,
//        including without limitation, you and your customers, any assistance
//        of any kind whatsoever, or any information; or
//    (c) conferring any license or right under any intellectual property right.
//
// 7. These terms shall be governed by and construed in accordance with the laws
//    of Taiwan, R.O.C., excluding its conflict of law rules.
//    Any and all dispute arising out hereof or related hereto shall be finally
//    settled by arbitration referred to the Chinese Arbitration Association,
//    Taipei in accordance with the ROC Arbitration Law and the Arbitration
//    Rules of the Association by three (3) arbitrators appointed in accordance
//    with the said Rules.
//    The place of arbitration shall be in Taipei, Taiwan and the language shall
//    be English.
//    The arbitration award shall be final and binding to both parties.
//
//******************************************************************************
//<MStar Software>
////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2008-2009 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// ("MStar Confidential Information") by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   PVRPL_Video.h
/// @brief  CPL API
/// @author MStar Semiconductor,Inc.
////////////////////////////////////////////////////////////////////////////////////////////////////
#include <string.h>

#include "demo_vdec.h"

#if((DEMO_DMS_TEST == 1) && (DEMO_VDEC_NDECODE_TEST == 1))
#define PVR_VSYNC_MODULE_ENABLE 1
#else
#define PVR_VSYNC_MODULE_ENABLE 0
#endif

#if (PVR_VSYNC_MODULE_ENABLE == 1)
// For Demo_AV_SetShowFrameMode
#include "demo_av.h"
#endif

#include "PVRPL_Video.h"
#include "PVRPL_Sys.h"
#include "PVRPL_FileIn.h"
#include "PVRPL_ResourcePool.h"

#include "apiVDEC_EX.h"
#include "apiDMX.h"
#include "drvSYS.h"
//mvop setup header
#include "apiXC.h"
#include "drvPQ.h"
//---------------------
#include "apiDAC.h"
#include "drvTVEncoder.h"
#include "msAPI_VE.h"
#include "drvDTC.h"

#define PVR_VIDEO_XC_PATH 0 //0 : by tdal, 1 : by pvrpl

#define PVR_VIDEO_MAX_NUM 2

#define AVSYNC_TIMEOUT_INTERVAL     2000
#define PL_VIDEO_FRAMECOUNT_LOOP    300
#define CMDQ_TIMEOUT 1000
//es player define
#define ESPLAYER_ES_VIDEO_SAFETY_SIZE (0x20)
#define PVR_MVD_DRV_PROC_SIZE (0xA000)
//tsp related
#define _FILEIN_ENG_NUM(x) ((x)? DMX_FILEIN_TSIF1:DMX_FILEIN_TSIF0)
#define PVRPL_VIDEO_DBGMSG(_level,_f) {if(_u32PVRPLVIDEODbgLevel >= (_level)) (_f);}

#define PVRPL_VIDEO_GET_XC_WIN_BY_PATH(path_idx, xc_win)\
    if(path_idx == 0)\
        xc_win = E_MSAPI_XC_MAIN_WINDOW;\
    else if(path_idx == 1)\
        xc_win = E_MSAPI_XC_SUB_WINDOW;\

typedef enum
{
    PVRPL_VIDEO_CMD_SET, // set the video stream Id to POOL_VDEC_StreamInfo
    PVRPL_VIDEO_CMD_GET, // get the video stream Id from POOL_VDEC_StreamInfo
}PVRPL_VDEC_VStreamID_CMD;

static MS_U32  _u32PVRPLVIDEODbgLevel = E_PVRPL_VIDEO_DBG_ERR;

#if PVR_VIDEO_XC_PATH
static ST_VDEC_INFORMATION stVDECInfo[E_VDEC_DEVICE_MAX];
#endif
//static MS_BOOL bPCRClosed[PVR_VIDEO_MAX_NUM] = {FALSE,FALSE};
#if (DEMO_XC_DUALXC_TEST == 1)
PNL_DeviceId xcpl_stPNL_DeviceId = {0, 0};
#endif

static MS_U32 u32SyncDelay = 180;
static MS_U16 u16SyncTolerance = 66;

static MSAPI_XC_DEVICE_ID gstXC_DeviceId[PVR_VIDEO_MAX_NUM] = {{0, E_MSAPI_XC_DEVICE0}, {0, E_MSAPI_XC_DEVICE1}};

//@NOTE for change window
static MSAPI_XC_WINDOW_TYPE stDispWinInfo[PVR_VIDEO_MAX_NUM];

static MS_BOOL bChangeDone[PVR_VIDEO_MAX_NUM] = {FALSE,FALSE};
//@NOTE for Vdec call back function
static VDEC_StreamId stVDECStreamId[PVR_VIDEO_MAX_NUM] = {{0, 0},{0, 0}};
static PVRPL_VIDEO_STATUS _PVRPL_Video_SetDecodeType(VDEC_StreamId *pStreamId,EN_VIDEO_CPL_DECODE_TYPE eDecodeType);
static PVRPL_VIDEO_STATUS _PVRPL_Video_SetDecodeMode(VDEC_StreamId *pStreamId,MS_U8 u8PathIdx, MS_S32 s32Speed, EN_VIDEO_CPL_DECODE_TYPE eDecodeType,EN_VIDEO_CPL_DATA_TYPE enDataType, EN_PVRPL_AVSYNC_MODE eAVSyncMode);
static MS_BOOL _PVRPL_Video_SetNormalFowrardMode(VDEC_StreamId *pStreamId, MS_S32 s32Speed, EN_VIDEO_CPL_DECODE_TYPE eDecodeType, EN_PVRPL_AVSYNC_MODE eAVSyncMode);
static MS_BOOL _PVRPL_Video_SetScanMode(VDEC_StreamId *pStreamId, MS_S32 s32Speed, EN_VIDEO_CPL_DECODE_TYPE eDecodeType);
static MS_BOOL _PVRPL_Video_SetSmoothScanMode(VDEC_StreamId *pStreamId, MS_S32 s32Speed, EN_VIDEO_CPL_DECODE_TYPE eDecodeType);
static MS_BOOL _PVRPL_Video_SetMotionPatch(VDEC_StreamId *pStreamId,MS_U8 u8PathIdx,MS_BOOL bEnable);
static MS_BOOL _PVRPL_Video_PIDSwitch(VDEC_StreamId *pStreamId,MS_BOOL bEnable, PVRPL_PidType ePIDType, MS_U16 u16Pid,MS_U8 *u8PidFlt);

#if PVR_VIDEO_XC_PATH
static MS_BOOL _PVRPL_Video_GetDecoderStatus(VDEC_StreamId *pStreamId,MS_U8 u8PathIdx);
#endif
static MS_BOOL _PVRPL_Video_IsFFX2CapEnough(VDEC_StreamId *pStreamId);
#if (PVR_VSYNC_MODULE_ENABLE == 0)
static MVOP_InputSel _PVRPL_Video_GetMVOPInputSel(VDEC_StreamId* pStreamId);
static PVRPL_VIDEO_STATUS _PVRPL_Video_SetMVOP_Config(MS_U8 u8PathIdx, VDEC_EX_DispInfo *stDispinfo,MVOP_InputSel eMvopInputSel);
#endif
static PVRPL_VIDEO_STATUS _PVRPL_Video_ChangeWinSize(MSAPI_XC_WINDOW_TYPE* stXCDestWin,MS_U32 u32XCDevice, E_MSAPI_XC_WINDOW eXCWin, E_MSAPI_XC_INPUT_SRC eXCInputSrc);


VDEC_CB_PLAY TdalPlayCB = NULL;
DISP_CB_ENABLE TdalDispCB = NULL;

PVRPL_VIDEO_STATUS PVRPL_Video_RegisterPlayCB(VDEC_CB_PLAY PlayCB)
{
    if (PlayCB==NULL)
    {
        printf("[%s][%d]PVRPL_Video_RegisterPlayCB Fail!\n",__FUNCTION__,__LINE__);
        return PVRPL_VIDEO_STATUS_ERROR;
    }
    TdalPlayCB = PlayCB;
    return PVRPL_VIDEO_STATUS_OK;
}

PVRPL_VIDEO_STATUS PVRPL_Disp_RegisterEnableCB(DISP_CB_ENABLE DispEnableCB)
{
    if (DispEnableCB==NULL)
    {
        printf("PVRPL_Disp_RegisterEnableCB Fail!\n");
        return PVRPL_VIDEO_STATUS_ERROR;
    }
    TdalDispCB = DispEnableCB;
    return PVRPL_VIDEO_STATUS_OK;
}

#if PVR_VIDEO_XC_PATH
static void _POOL_VDEC_Callback(MS_U32 u32Event, void *param)
{
#if 0//(DEMO_XC_DIPMultiWin_TEST == 1)
    EN_VDEC_Device eDevice = 0;
    if(Demo_VDEC_GetMode(&eDevice,E_VDEC_GetCmd_GetDevice_ByStreamID,param) == FALSE)
        printf("\033[31m [%s][%d]Demo_VDEC_Callback get device fail by streamid\033[0m\n",__FUNCTION__,__LINE__);
#endif

    if (u32Event & E_VDEC_EX_EVENT_DISP_INFO_CHG)
    {
        printf("\n[0x%"DTC_MS_U32_x"] E_VDEC_EVENT_DISP_INFO_CHG !!!\n",((VDEC_StreamId*)param)->u32Id);

        MApi_VDEC_EX_SetBlueScreen(param, FALSE);

#if 0//(DEMO_XC_DIPMultiWin_TEST == 1)
        if(stVDECInfo[eDevice].stShowFrameInfo.eShowFrame_Mode == E_VDEC_DIP_MODE)
        {
            VDEC_EX_DispInfo stDispinfo;
            memset(&stDispinfo, 0, sizeof(VDEC_EX_DispInfo));
            if(MApi_VDEC_EX_GetDispInfo(param,&stDispinfo) == E_VDEC_EX_OK)
            {
                if(stDispinfo.u32FrameRate != stVDECDIPInfo[eDevice].u32FrameRate)
                {
                    printf("[%s][%d]FrameRate Change old[%"DTC_MS_U32_d"] NEW[%"DTC_MS_U32_d"]\n",__FUNCTION__,__LINE__,stVDECDIPInfo[eDevice].u32FrameRate,stDispinfo.u32FrameRate);
                    stVDECDIPInfo[eDevice].u32FrameRate = stDispinfo.u32FrameRate;

                    if(stVDECDIPInfo[eDevice].stDIPFrameSyncInfo.u16InputTolerance == 0)
                    {
                        stVDECDIPInfo[eDevice].stDIPFrameSyncInfo.u16Tolerance = (float)(1/(float)(stVDECDIPInfo[eDevice].u32FrameRate/1000)*1000); //ms
                        printf("[%s][%d]New Tolerance[%d]\n",__FUNCTION__,__LINE__,stVDECDIPInfo[eDevice].stDIPFrameSyncInfo.u16Tolerance);
                    }
                }
            }
        }
#endif
    }

    MS_U32 u32BufferSize = 0;

#if 0//(DEMO_VDEC_NDECODE_TEST == 1)
    VDEC_EX_BufferInfo parms;
    memset(&parms, 0 , sizeof(VDEC_EX_BufferInfo));

    if(Demo_VDEC_GetMode(&eDevice,E_VDEC_GetCmd_BUFFERTYPE_ALLOC_BS,&parms))
    {
        u32BufferSize  = parms.szSize;
    }
#else
    u32BufferSize = VDEC_BIT_STREAM_LEN;
#endif

    if (u32Event & E_VDEC_EX_EVENT_DISP_ONE)
    {
        MS_U32 u32VESlevel = 0;
        MS_U32 u32VWptr = 0;
        MS_U32 u32VRptr = 0;
        MS_U32 u32stc32 = 0;
        MS_U32 u32stc = 0;
        MS_U32 u32pcr32 = 0;
        MS_U32 u32pcr = 0;
        MS_U32 u32STCEng = 0;
        MS_U32 u32MvopBank = 0;
        MS_U8 u8Field = 0;
        MS_U8 u8Fdmask = 0;
        MS_U8 u8Streamidx = (MS_U8)(((VDEC_StreamId*)param)->u32Id >> 24);

        u32STCEng = stVDECInfo[u8Streamidx].u32STCEng;

        u32VWptr = MApi_VDEC_EX_GetESWritePtr(param);
        u32VRptr = MApi_VDEC_EX_GetESReadPtr(param);

        if (u32VWptr>=u32VRptr)
        {
            u32VESlevel = u32VWptr - u32VRptr;
        }
        else
        {
            u32VESlevel = u32BufferSize - ( u32VRptr - u32VWptr );
        }

        MApi_DMX_Stc_Eng_Get(u32STCEng,&u32stc32, &u32stc);
        MApi_DMX_Pcr_Eng_Get(u32STCEng,&u32pcr32, &u32pcr);

#if 0//(DEMO_XC_DIPMultiWin_TEST == 1)
        if(stVDECInfo[eDevice].stShowFrameInfo.eShowFrame_Mode == E_VDEC_MVOP_MODE)
#endif
        {
            if (u8Streamidx == 0 )
                u32MvopBank = 0x101400;
            else if (u8Streamidx == 1 )
                u32MvopBank = 0x103D00;

            MApi_XC_Write2ByteMask((u32MvopBank | 0x5E), 0x4000, 0xE000);//RIU debug register select = 2

            u8Field  = (MApi_XC_ReadByte((u32MvopBank | 0x5D)) & 0x10)>>4;
            u8Fdmask = (MApi_XC_ReadByte((u32MvopBank | 0x5D)) & 0x20)>>5;
        }

#if 0
        if(fpCallBackDBG)
        {
            fprintf(fpCallBackDBG,"[%d] systime: %"DTC_MS_U32_d",F: %"DTC_MS_U32_d",Dis: %"DTC_MS_U32_d",Err: %"DTC_MS_U32_d",Skip: %"DTC_MS_U32_d",Drop: %"DTC_MS_U32_d",PCR: %"DTC_MS_U32_d",STC: %"DTC_MS_U32_d",PTS: %"DTC_MS_U32_d",Delta: %"DTC_MS_U32_d",VES: 0x%"DTC_MS_U32_x",WPtr: 0x%"DTC_MS_U32_x",Rptr: 0x%"DTC_MS_U32_x"\n",
                u8Streamidx,MsOS_GetSystemTime(),MApi_VDEC_EX_GetFrameCnt(param),MApi_VDEC_EX_GetDispCnt(param),MApi_VDEC_EX_GetErrCnt(param),
                MApi_VDEC_EX_GetSkipCnt(param),MApi_VDEC_EX_GetDropCnt(param),u32pcr / 90,u32stc / 90,MApi_VDEC_EX_GetPTS(param),
                (MApi_VDEC_EX_GetPTS(param)- (u32stc / 90)),u32VESlevel,u32VWptr,u32VRptr);
            MsFS_Fflush(fpCallBackDBG);
        }
        else
        {
            printf("[%d] systime: %"DTC_MS_U32_d",F: %"DTC_MS_U32_d",Dis: %"DTC_MS_U32_d",Err: %"DTC_MS_U32_d",Skip: %"DTC_MS_U32_d",Drop: %"DTC_MS_U32_d",PCR: %"DTC_MS_U32_d",STC: %"DTC_MS_U32_d",PTS: %"DTC_MS_U32_d",[Field]%u ,[FDMASK]%u ,Delta: %"DTC_MS_U32_d",VES: 0x%"DTC_MS_U32_x",WPtr: 0x%"DTC_MS_U32_x",Rptr: 0x%"DTC_MS_U32_x"\n",
            u8Streamidx,MsOS_GetSystemTime(),MApi_VDEC_EX_GetFrameCnt(param),MApi_VDEC_EX_GetDispCnt(param),MApi_VDEC_EX_GetErrCnt(param),
            MApi_VDEC_EX_GetSkipCnt(param),MApi_VDEC_EX_GetDropCnt(param),u32pcr / 90,u32stc / 90,MApi_VDEC_EX_GetPTS(param),u8Field,u8Fdmask,
            (MApi_VDEC_EX_GetPTS(param)- (u32stc / 90)),u32VESlevel,u32VWptr,u32VRptr);
        }
#endif
     }
}
#endif
void _Pool_VDEC_InitSysConfigParam(EN_VDEC_Device eDevice,VDEC_EX_SysCfg *pstSysConfig)
{
    pstSysConfig->u32CodeBufAddr = MEM_ADR_BY_MIU(VDEC_AEON_ADR, VDEC_AEON_MEMORY_TYPE);
    pstSysConfig->u32CodeBufSize = VDEC_AEON_LEN;

#if(DEMO_VDEC_NDECODE_TEST == 1)
    pstSysConfig->u32FrameBufAddr       = 0;
    pstSysConfig->u32FrameBufSize       = 0;
    pstSysConfig->u32BitstreamBufAddr   = 0;
    pstSysConfig->u32BitstreamBufSize   = 0;
    pstSysConfig->u32DrvProcBufAddr     = 0;
#else
    if (eDevice == E_VDEC_DEVICE_MAIN)
    {
        pstSysConfig->u32FrameBufAddr = MEM_ADR_BY_MIU(VDEC_FRAME_BUF_ADR, VDEC_FRAME_BUF_MEMORY_TYPE);
        pstSysConfig->u32FrameBufSize = VDEC_FRAME_BUF_LEN;
        pstSysConfig->u32BitstreamBufAddr = MEM_ADR_BY_MIU(VDEC_BIT_STREAM_ADR, VDEC_BIT_STREAM_MEMORY_TYPE);
        pstSysConfig->u32BitstreamBufSize = VDEC_BIT_STREAM_LEN;
        pstSysConfig->u32DrvProcBufAddr = MEM_ADR_BY_MIU(VDEC_BIT_STREAM_ADR, VDEC_BIT_STREAM_MEMORY_TYPE);
    }
#if(DEMO_VDEC_MULTI_TEST == 1)
    else if (eDevice == E_VDEC_DEVICE_SUB)
    {
        pstSysConfig->u32FrameBufAddr = MEM_ADR_BY_MIU(VDEC_FRAME_BUF1_ADR, VDEC_FRAME_BUF1_MEMORY_TYPE);
        pstSysConfig->u32FrameBufSize = VDEC_FRAME_BUF1_LEN;
        pstSysConfig->u32BitstreamBufAddr = MEM_ADR_BY_MIU(VDEC_BIT_STREAM1_ADR, VDEC_BIT_STREAM1_MEMORY_TYPE);
        pstSysConfig->u32BitstreamBufSize = VDEC_BIT_STREAM1_LEN;
        pstSysConfig->u32DrvProcBufAddr = MEM_ADR_BY_MIU(VDEC_BIT_STREAM1_ADR, VDEC_BIT_STREAM1_MEMORY_TYPE);
    }
#endif
#endif

#if(DEMO_MVC_TEST == 1)
    if(stVDECInfo[eDevice].eVideoCodec == E_VDEC_DDI_CODEC_TYPE_MVC_MEMORY
        || stVDECInfo[eDevice].eVideoCodec == E_VDEC_DDI_CODEC_TYPE_MVC)
    {
        pstSysConfig->u32FrameBufAddr = MEM_ADR_BY_MIU(VDEC_MVC_FRAME_BUF_ADR, VDEC_MVC_FRAME_BUF_MEMORY_TYPE);
        pstSysConfig->u32FrameBufSize = VDEC_MVC_FRAME_BUF_LEN;
        pstSysConfig->u32BitstreamBufAddr = MEM_ADR_BY_MIU(VDEC_MVC_BIT_STREAM_ADR, VDEC_MVC_BIT_STREAM_MEMORY_TYPE);
        pstSysConfig->u32BitstreamBufSize = VDEC_MVC_BIT_STREAM_LEN;
        pstSysConfig->u32DrvProcBufAddr = MEM_ADR_BY_MIU(VDEC_MVC_BIT_STREAM_ADR, VDEC_MVC_BIT_STREAM_MEMORY_TYPE);
    }
#endif

#if(DEMO_CMA_TEST == 1)
    pstSysConfig->u32FrameBufAddr = 0;
#endif

    pstSysConfig->eDbgMsgLevel = E_VDEC_EX_DBG_LEVEL_NONE;
    printf("VDEC FW add = 0x%lx\n", pstSysConfig->u32CodeBufAddr);
    printf("VDEC FB add = 0x%lx\n", pstSysConfig->u32FrameBufAddr);
    printf("VDEC BS add = 0x%lx\n", pstSysConfig->u32BitstreamBufAddr);

}


void _PVRPL_Video_VStreamIDConverter(POOL_VDEC_StreamInfo *pStreamVideoInfo,VDEC_StreamId *StreamVideoID,PVRPL_VDEC_VStreamID_CMD enCMD)
{
    switch(enCMD)
    {
        case PVRPL_VIDEO_CMD_SET:
            pStreamVideoInfo->u32Id = StreamVideoID->u32Id;
            pStreamVideoInfo->u32Version = StreamVideoID->u32Version;
            break;
        case PVRPL_VIDEO_CMD_GET:
            StreamVideoID->u32Id = pStreamVideoInfo->u32Id;
            StreamVideoID->u32Version = pStreamVideoInfo->u32Version;
            break;
        default:
            PVRPL_VIDEO_DBGMSG(E_PVRPL_VIDEO_DBG_ERR,printf("Do not support this cmd(%"DTC_MS_U32_d")\n",(MS_U32)enCMD));
            break;
    }
}

void _PVRPL_Video_SC_Mute(MS_U32 u32XCDevice, E_MSAPI_XC_WINDOW eWindow, MS_BOOL bMute)
{
#if (PVR_VSYNC_MODULE_ENABLE == 1)
    if(u32XCDevice == 0)
    {
        if(bMute)
        {
            msAPI_XC_DIPMultiView_SetMute(eWindow,E_MSAPI_DIP_MULTIVIEW_MUTE_ON);
        }
        else
        {
            msAPI_XC_DIPMultiView_SetMute(eWindow,E_MSAPI_DIP_MULTIVIEW_MUTE_OFF);
        }
    }
    else
#endif
    {
        if(bMute)
        {
            if(msAPI_XC_IsBlackVideoEnable_EX(&(gstXC_DeviceId[u32XCDevice]),eWindow) == FALSE)
            {
                msAPI_XC_SetVideoMuteByMode_EX(&gstXC_DeviceId[u32XCDevice], ENABLE, eWindow, E_MSAPI_XC_VIDEO_MUTE_MODE_AUTO_SYNC);
                PVRPL_VIDEO_DBGMSG(E_PVRPL_VIDEO_DBG_TRACE,printf("[%s][%d]Demo_XC_SetVideoMute:%"DTC_MS_U32_d" %d\n",
                    __FUNCTION__, __LINE__, u32XCDevice, eWindow));
            }
        }
        else
        {
            if(msAPI_XC_IsBlackVideoEnable_EX(&(gstXC_DeviceId[u32XCDevice]),eWindow) == TRUE)
            {
                PVRPL_VIDEO_DBGMSG(E_PVRPL_VIDEO_DBG_TRACE,printf("[%s]Trace XC Function: msAPI_XC_SetVideoMuteByMode_EX\n",__FUNCTION__));
                msAPI_XC_SetVideoMuteByMode_EX(&(gstXC_DeviceId[u32XCDevice]), DISABLE, eWindow, E_MSAPI_XC_VIDEO_MUTE_MODE_AUTO_SYNC);
            }
        }
    }
}

MS_BOOL _PVRPL_Video_ChangeDisplayWindow(PVR_PATH u8PathIdx, MSAPI_XC_WINDOW_TYPE* pstXCDestWin)
{

#if (PVR_VSYNC_MODULE_ENABLE == 1)
    // DMS code flow
    VDEC_EX_DispInfo stDispinfo = {};
    MApi_VDEC_EX_GetDispInfo(&stVDECStreamId[u8PathIdx], &stDispinfo);

    MSAPI_XC_WINDOW_TYPE stCropWin = {};
    stCropWin.width = stDispinfo.u16HorSize - (stDispinfo.u16CropRight + stDispinfo.u16CropLeft);
    stCropWin.height = stDispinfo.u16VerSize - (stDispinfo.u16CropTop + stDispinfo.u16CropBottom);

    MSAPI_XC_WINDOW_TYPE stTempDestWin = *pstXCDestWin;

    MSAPI_XC_DEVICE_ID g_stDIP_DeviceId = {0, E_MSAPI_XC_DEVICE_DIP_0};
    if((pstXCDestWin->height == 0)&&(pstXCDestWin->width == 0 )&&(pstXCDestWin->x == 0 )&&(pstXCDestWin->y == 0 ))
    {
        pstXCDestWin->x = 0;
        pstXCDestWin->y = 0;
        if(u8PathIdx == 0)
        {
            // For Main path
            pstXCDestWin->width = IPANEL(&xcpl_stPNL_DeviceId, Width);
            pstXCDestWin->height = IPANEL(&xcpl_stPNL_DeviceId, Height);
        }
        else if(u8PathIdx == 1)
        {
            // If there is Sub path
            pstXCDestWin->width = IPANEL(&xcpl_stPNL_DeviceId, Width) / 3;
            pstXCDestWin->height = IPANEL(&xcpl_stPNL_DeviceId, Height) / 3;
        }
    }

    _PVRPL_Video_SC_Mute(0, (E_MSAPI_XC_WINDOW)u8PathIdx, TRUE);
    msAPI_XC_SetWin_EX(&g_stDIP_DeviceId, NULL, &stCropWin, pstXCDestWin, u8PathIdx);
    _PVRPL_Video_SC_Mute(0, (E_MSAPI_XC_WINDOW)u8PathIdx, FALSE);

    // Recover to the original DestWin, so the code flow for VE can be keep unchanged.
    *pstXCDestWin = stTempDestWin;
#else
    // MVOP/XC code flow
    if (u8PathIdx == 0)
    {
        _PVRPL_Video_SC_Mute(0, E_MSAPI_XC_MAIN_WINDOW, TRUE);
        _PVRPL_Video_ChangeWinSize(pstXCDestWin,0,E_MSAPI_XC_MAIN_WINDOW,E_MSAPI_XC_INPUT_SOURCE_DTV);
        _PVRPL_Video_SC_Mute(0, E_MSAPI_XC_MAIN_WINDOW, FALSE);
    }
    else if (u8PathIdx == 1)
    {
        _PVRPL_Video_SC_Mute(0, E_MSAPI_XC_SUB_WINDOW, TRUE);
        _PVRPL_Video_ChangeWinSize(pstXCDestWin,0,E_MSAPI_XC_SUB_WINDOW,E_MSAPI_XC_INPUT_SOURCE_DTV2);
        _PVRPL_Video_SC_Mute(0, E_MSAPI_XC_SUB_WINDOW, FALSE);
    }
#endif
    // VE code flow, no matter use DMS or MVOP/XC

    //@NOTE Drvier behavior
    //There is the other xc1 path to CVBS , so need to set up xc1.
#if (DEMO_XC_DUALXC_TEST == 1)
    #if (DEMO_XC_DUALXC_IDENTICAL_DISPLAY_TEST == 1)
    if(msAPI_XC_IsBlackVideoEnable_EX(&(gstXC_DeviceId[1]),E_MSAPI_XC_MAIN_WINDOW) == TRUE)
    {//Always keep SC1 main window un-mute
        PVRPL_VIDEO_DBGMSG(E_PVRPL_VIDEO_DBG_TRACE,printf("Main window of SD path (SC1) => Demo_XC_PlayVideo from source SCALER_OP\n"));
        PVRPL_VIDEO_DBGMSG(E_PVRPL_VIDEO_DBG_TRACE,printf("[%s]Trace XC Function: msAPI_XC_SetWin_EX\n",__FUNCTION__));
        _PVRPL_Video_ChangeWinSize(pstXCDestWin,1,E_MSAPI_XC_MAIN_WINDOW,E_MSAPI_XC_INPUT_SOURCE_SCALER_OP);
    }

    _PVRPL_Video_SC_Mute(1, E_MSAPI_XC_MAIN_WINDOW, FALSE);
    #else
    if (u8PathIdx == 1)
    {//SC1 main window follows SC0 main window. The look's the same.
        _PVRPL_Video_SC_Mute(1, E_MSAPI_XC_MAIN_WINDOW, TRUE);

        PVRPL_VIDEO_DBGMSG(E_PVRPL_VIDEO_DBG_TRACE,printf("Main window of SD path (SC1) => Demo_XC_PlayVideo from source DTV\n"));
        PVRPL_VIDEO_DBGMSG(E_PVRPL_VIDEO_DBG_TRACE,printf("[%s]Trace XC Function: msAPI_XC_SetWin_EX\n",__FUNCTION__));
        _PVRPL_Video_ChangeWinSize(pstXCDestWin,1,E_MSAPI_XC_MAIN_WINDOW,E_MSAPI_XC_INPUT_SOURCE_DTV);

        _PVRPL_Video_SC_Mute(1, E_MSAPI_XC_MAIN_WINDOW, FALSE);
    }
    #endif
#endif
    return TRUE;
}

PVRPL_VIDEO_STATUS PVRPL_Video_Init(POOL_VDEC_StreamInfo *pStreamVideoInfo,MS_U32 u32VCodec,MS_U8 u8STCEng,void *pVideoID)
{
#if PVR_VIDEO_XC_PATH
    //EN_VDEC_Device eVDECDevice;
    //EN_VDEC_DDI_CodecType eVDECCodeType;
    PVR_PATH u8PathIdx = GET_PVR_PATH_IDX(pStreamVideoInfo->u8PathIdx);

    VDEC_EX_Status stVDECStatus;
    //VDEC_EX_Stream eVDECStream = 0;
    VDEC_EX_CodecType eCodecType = 0;
    VDEC_EX_InitParam stInitParam;

#if (PVR_VSYNC_MODULE_ENABLE == 1)
    MS_U16 u16X = 0;
    MS_U16 u16Y = 0;
    MS_U16 u16Width = 0;
    MS_U16 u16Height = 0;
    if(u8PathIdx == 0)
    {
        // Main path
        u16Width = IPANEL(&xcpl_stPNL_DeviceId, Width);
        u16Height = IPANEL(&xcpl_stPNL_DeviceId, Height);
    }
    else if(u8PathIdx == 1)
    {
        // If there is Sub path
        u16Width = IPANEL(&xcpl_stPNL_DeviceId, Width) / 3;
        u16Height = IPANEL(&xcpl_stPNL_DeviceId, Height) / 3;
    }

    // Always use PureMCU mode in PVR
    EN_AV_ShowFrame_Mode eMode = E_AV_PureMCU_MODE;
    EN_AV_Device eDevice = (u8PathIdx == 0) ? E_AV_DEVICE_MAIN : E_AV_DEVICE_SUB;
    Demo_AV_SetShowFrameMode(&eDevice, &eMode, (MS_U32 *)&u8PathIdx, &u16X, &u16Y, &u16Width, &u16Height, (MS_U16 *)&u8PathIdx);

    // In Demo_AV_PlayAV, set E_VDEC_EX_N_STREAM
    VDEC_EX_Stream eStreamType = E_VDEC_EX_N_STREAM;
    Demo_VDEC_SetMode((EN_VDEC_Device *)&eDevice, E_VDEC_SetCmd_StreamType, (void *)&eStreamType);
#endif

    // If seamless, don't have to init Vdec again.
    if(pStreamVideoInfo->bIsSeamless)
    {
        PVRPL_VIDEO_DBGMSG(E_PVRPL_VIDEO_DBG_TRACE,printf("!!!!!!!!!!!!! seamless should be here\n"));
        VDEC_StreamId* pVdecStreamId  = Pool_GetVideoID(u8PathIdx);
        if(pVdecStreamId == NULL)
        {
            PVRPL_VIDEO_DBGMSG(E_PVRPL_VIDEO_DBG_ERR,printf("[%s][%d]Get pVDECStreamId failed!\n",__FUNCTION__,__LINE__));
            return PVRPL_VIDEO_STATUS_ERROR;
        }
        pStreamVideoInfo->bVInit     = TRUE;
        _PVRPL_Video_VStreamIDConverter(pStreamVideoInfo,pVdecStreamId,PVRPL_VIDEO_CMD_SET);
        _PVRPL_Video_VStreamIDConverter(pStreamVideoInfo,&stVDECStreamId[u8PathIdx],PVRPL_VIDEO_CMD_GET);
        PVRPL_VIDEO_DBGMSG(E_PVRPL_VIDEO_DBG_TRACE,printf("PVRPL_Video_Init seamless(%p/%p) -Path:%"DTC_MS_U32_d" stVDECStreamId:  0x%"DTC_MS_U32_x", Version 0x%"DTC_MS_U32_x"\n",&stVDECStreamId[u8PathIdx],Pool_GetVideoID(u8PathIdx),(MS_U32)u8PathIdx,stVDECStreamId[u8PathIdx].u32Id, stVDECStreamId[u8PathIdx].u32Version));

        MApi_DMX_AVFifo_Reset(Pool_GetVFifoType(u8PathIdx), FALSE);

        //@NOTE for MVD 2xFF, PVR default: AV sync mode
        MApi_VDEC_EX_SetControl(&stVDECStreamId[u8PathIdx],E_VDEC_EX_USER_CMD_SET_TRICKPLAY_2X_MODE, E_VDEC_EX_TRICKPLAY2X_AVSYNC);
        return PVRPL_VIDEO_STATUS_OK;
    }
/*
    switch(u8PathIdx)
    {
        case 0:
            eVDECDevice = E_VDEC_DEVICE_MAIN;
            break;
        case 1:
            eVDECDevice = E_VDEC_DEVICE_SUB;
            break;
        default:
            PVRPL_VIDEO_DBGMSG(E_PVRPL_VIDEO_DBG_ERR,printf("Wrong VDEC Device:%d\n",pStreamVideoInfo->u8PathIdx));
            return PVRPL_VIDEO_STATUS_ERROR;
    }
*/
    switch (u32VCodec)
    {
        case EN_PL_VDEC_CODEC_TYPE_MPEG2:
        case EN_PL_VDEC_CODEC_TYPE_MPEG2_IFRAME:
        {
            PVRPL_VIDEO_DBGMSG(E_PVRPL_VIDEO_DBG_TRACE,printf("Video MPEG type\n"));
            eCodecType = E_VDEC_EX_CODEC_TYPE_H264;
            break;
        }
        case EN_PL_VDEC_CODEC_TYPE_H264:
        case EN_PL_VDEC_CODEC_TYPE_H264_IFRAME:
        {
            PVRPL_VIDEO_DBGMSG(E_PVRPL_VIDEO_DBG_TRACE,printf("Video H264 type\n"));
            eCodecType = E_VDEC_EX_CODEC_TYPE_H264;
            break;
        }
        case EN_PL_VDEC_CODEC_TYPE_AVS:
        {
            PVRPL_VIDEO_DBGMSG(E_PVRPL_VIDEO_DBG_TRACE,printf("Video AVS type\n"));
            eCodecType = E_VDEC_EX_CODEC_TYPE_AVS;
            break;
        }
        case EN_PL_VDEC_CODEC_TYPE_HEVC:
        case EN_PL_VDEC_CODEC_TYPE_HEVC_IFRAME:
        {
            PVRPL_VIDEO_DBGMSG(E_PVRPL_VIDEO_DBG_TRACE,printf("Video HEVC type\n"));
            eCodecType = E_VDEC_EX_CODEC_TYPE_HEVC;
            break;
        }
        default:
        {
            PVRPL_VIDEO_DBGMSG(E_PVRPL_VIDEO_DBG_ERR,printf("Invalid Codec Type %"DTC_MS_U32_d"\n",u32VCodec));
            return PVRPL_VIDEO_STATUS_ERROR;
        }
    }

#if 0
    if (FALSE == Demo_VDEC_Init(&eVDECDevice,&stVDECStreamId[u8PathIdx],eVDECCodeType,E_VDEC_EX_SRC_MODE_TS_FILE,u8STCEng))
    {
        PVRPL_VIDEO_DBGMSG(E_PVRPL_VIDEO_DBG_ERR,printf(" [%d]....GetFreeStream FAILd\n", __LINE__));
        return PVRPL_VIDEO_STATUS_ERROR;
    }
    else
    {
        pStreamVideoInfo->bVInit     = TRUE;
        _PVRPL_Video_VStreamIDConverter(pStreamVideoInfo,&stVDECStreamId[u8PathIdx],PVRPL_VIDEO_CMD_SET);
        PVRPL_VIDEO_DBGMSG(E_PVRPL_VIDEO_DBG_TRACE,printf("PVRPL_Video_Init(%p) - stVDECStreamId:  0x%"DTC_MS_U32_x", Version 0x%"DTC_MS_U32_x" \n",&stVDECStreamId[u8PathIdx],stVDECStreamId[u8PathIdx].u32Id, stVDECStreamId[u8PathIdx].u32Version));
    }
#else //@NOTE as same as Demo_VDEC_Init

//    stVDECInfo[eVDECDevice].eVideoCodec = eVDECCodeType;
//    stVDECInfo[eVDECDevice].u32STCEng = u8STCEng;
//    stVDECInfo[eVDECDevice].eVideoSrcMode = E_VDEC_EX_SRC_MODE_TS_FILE;

    memset(&stVDECStatus, 0, sizeof(VDEC_EX_Status));
    MApi_VDEC_EX_GetStatus(&stVDECStreamId[u8PathIdx],&stVDECStatus);
    printf("eCodecTypeCur : %d \n",MApi_VDEC_EX_GetActiveCodecType(&stVDECStreamId[u8PathIdx]));


    memset(&stInitParam, 0, sizeof(VDEC_EX_InitParam));
    stInitParam.bDisableDropErrFrame = TRUE;
    stInitParam.eCodecType = eCodecType;// = _Demo_VDEC_MappingCodec(stVDECInfo[eVDECDevice].eVideoCodec);
    stInitParam.VideoInfo.eSrcMode = E_VDEC_EX_SRC_MODE_TS_FILE;//stVDECInfo[eVDECDevice].eVideoSrcMode;


    //_Demo_VDEC_InitSysConfigParam(*peDevice,&stInitParam.SysConfig);
    //_Pool_VDEC_InitSysConfigParam(eVDECDevice,&stInitParam.SysConfig);
        stInitParam.SysConfig.u32CodeBufAddr = MEM_ADR_BY_MIU(VDEC_AEON_ADR, VDEC_AEON_MEMORY_TYPE);
        stInitParam.SysConfig.u32CodeBufSize = VDEC_AEON_LEN;

#if(DEMO_VDEC_NDECODE_TEST == 1)
        stInitParam.SysConfig.u32FrameBufAddr       = 0;
        stInitParam.SysConfig.u32FrameBufSize       = 0;
        stInitParam.SysConfig.u32BitstreamBufAddr   = 0;
        stInitParam.SysConfig.u32BitstreamBufSize   = 0;
        stInitParam.SysConfig.u32DrvProcBufAddr     = 0;
#else
        //if (eDevice == E_VDEC_DEVICE_MAIN)
        {
            stInitParam.SysConfig.u32FrameBufAddr = MEM_ADR_BY_MIU(VDEC_FRAME_BUF_ADR, VDEC_FRAME_BUF_MEMORY_TYPE);
            stInitParam.SysConfig.u32FrameBufSize = VDEC_FRAME_BUF_LEN;
            stInitParam.SysConfig.u32BitstreamBufAddr = MEM_ADR_BY_MIU(VDEC_BIT_STREAM_ADR, VDEC_BIT_STREAM_MEMORY_TYPE);
            stInitParam.SysConfig.u32BitstreamBufSize = VDEC_BIT_STREAM_LEN;
            stInitParam.SysConfig.u32DrvProcBufAddr = MEM_ADR_BY_MIU(VDEC_BIT_STREAM_ADR, VDEC_BIT_STREAM_MEMORY_TYPE);
        }
#if 0//(DEMO_VDEC_MULTI_TEST == 1)
        else if (eDevice == E_VDEC_DEVICE_SUB)
        {
            stInitParam.SysConfig.u32FrameBufAddr = MEM_ADR_BY_MIU(VDEC_FRAME_BUF1_ADR, VDEC_FRAME_BUF1_MEMORY_TYPE);
            stInitParam.SysConfig.u32FrameBufSize = VDEC_FRAME_BUF1_LEN;
            stInitParam.SysConfig.u32BitstreamBufAddr = MEM_ADR_BY_MIU(VDEC_BIT_STREAM1_ADR, VDEC_BIT_STREAM1_MEMORY_TYPE);
            stInitParam.SysConfig.u32BitstreamBufSize = VDEC_BIT_STREAM1_LEN;
            stInitParam.SysConfig.u32DrvProcBufAddr = MEM_ADR_BY_MIU(VDEC_BIT_STREAM1_ADR, VDEC_BIT_STREAM1_MEMORY_TYPE);
        }
#endif
#endif

#if(DEMO_MVC_TEST == 1)
        if(stVDECInfo[eDevice].eVideoCodec == E_VDEC_DDI_CODEC_TYPE_MVC_MEMORY
            || stVDECInfo[eDevice].eVideoCodec == E_VDEC_DDI_CODEC_TYPE_MVC)
        {
            stInitParam.SysConfig.u32FrameBufAddr = MEM_ADR_BY_MIU(VDEC_MVC_FRAME_BUF_ADR, VDEC_MVC_FRAME_BUF_MEMORY_TYPE);
            stInitParam.SysConfig.u32FrameBufSize = VDEC_MVC_FRAME_BUF_LEN;
            stInitParam.SysConfig.u32BitstreamBufAddr = MEM_ADR_BY_MIU(VDEC_MVC_BIT_STREAM_ADR, VDEC_MVC_BIT_STREAM_MEMORY_TYPE);
            stInitParam.SysConfig.u32BitstreamBufSize = VDEC_MVC_BIT_STREAM_LEN;
            stInitParam.SysConfig.u32DrvProcBufAddr = MEM_ADR_BY_MIU(VDEC_MVC_BIT_STREAM_ADR, VDEC_MVC_BIT_STREAM_MEMORY_TYPE);
        }
#endif

#if(DEMO_CMA_TEST == 1)
        stInitParam.SysConfig.u32FrameBufAddr = 0;
#endif

        stInitParam.SysConfig.eDbgMsgLevel = E_VDEC_EX_DBG_LEVEL_NONE;
        printf("VDEC FW add = 0x%lx\n", stInitParam.SysConfig.u32CodeBufAddr);
        printf("VDEC FB add = 0x%lx\n", stInitParam.SysConfig.u32FrameBufAddr);
        printf("VDEC BS add = 0x%lx\n", stInitParam.SysConfig.u32BitstreamBufAddr);

    if (MApi_VDEC_EX_GetFreeStream((VDEC_StreamId *)&stVDECStreamId[u8PathIdx], sizeof(VDEC_StreamId), Pool_GetStreamType(u8PathIdx), (VDEC_EX_CodecType)stInitParam.eCodecType) != E_VDEC_EX_OK)
    {
        printf("[%s][%d]....GetFreeStream Failed Stream ID:  0x%"DTC_MS_U32_x" \n",__FUNCTION__,__LINE__, stVDECStreamId[u8PathIdx].u32Id);
        return FALSE;
    }
    else
    {
        printf("[%s][%d] Stream ID:  0x%"DTC_MS_U32_x", Version 0x%"DTC_MS_U32_x" \n",__FUNCTION__,__LINE__,stVDECStreamId[u8PathIdx].u32Id, stVDECStreamId[u8PathIdx].u32Version);
    }

    //@NOTE as same as Demo_VDEC_PreSetControl start
    //VDEC will CAL FRAMERATE when frame rate not in SPS
    MApi_VDEC_EX_PreSetControl(&stVDECStreamId[u8PathIdx],E_VDEC_EX_USER_CMD_CAL_FRAMERATE, TRUE);
    //_Demo_VDEC_PreSetControl_NdecodeSetting(peDevice,pstVDECStreamId);
    //if(stVDECInfo[eVDECDevice].u32CmdPreSetControl & 0x1)
    //    MApi_VDEC_EX_PreSetControl(&stVDECStreamId[u8PathIdx],E_VDEC_EX_USER_CMD_FORCE_8BIT_DEC_MODE, TRUE);
    //@NOTE as same as Demo_VDEC_PreSetControl end

    //MS_U32 u32StartTime= MsOS_GetSystemTime();

    //Init/Re-Init VDEC
    if (MApi_VDEC_EX_Init(&stVDECStreamId[u8PathIdx],&stInitParam) != E_VDEC_EX_OK)
    {
        printf("*******************************************************\n");
        printf("MApi_VDEC_Init  return fail!!!\n");
        printf("*******************************************************\n");
        return FALSE;
    }
    else
    {
        pStreamVideoInfo->bVInit     = TRUE;
        _PVRPL_Video_VStreamIDConverter(pStreamVideoInfo,&stVDECStreamId[u8PathIdx],PVRPL_VIDEO_CMD_SET);
    }

    //stVDECInfo[eVDECDevice].stVDECSteamID = stVDECStreamId[u8PathIdx];
#endif

    //@NOTE for MVD 2xFF, PVR default: AV sync mode
    MApi_VDEC_EX_SetControl(&stVDECStreamId[u8PathIdx],E_VDEC_EX_USER_CMD_SET_TRICKPLAY_2X_MODE, E_VDEC_EX_TRICKPLAY2X_AVSYNC);

#if 0
    if(Demo_VDEC_Play(&eVDECDevice,&stVDECStreamId[u8PathIdx],E_VDEC_SYNC_PCR_MODE) == FALSE)
    {
        PVRPL_VIDEO_DBGMSG(E_PVRPL_VIDEO_DBG_ERR,printf("Demo_VDEC_Play failed\n"));
    }
#else //@NOTE as same as Demo_VDEC_Play
    //@NOTE  reset AV sync
    MApi_VDEC_EX_AVSyncOn(&stVDECStreamId[u8PathIdx],TRUE, u32SyncDelay, u16SyncTolerance);

    //set the threshold of Freerun condition.If the difference of STC and PTS large than
    // threshold, FW will free run. The threshold value is 90KHZ base.
    if(MApi_VDEC_EX_SetAVSyncFreerunThreshold(&stVDECStreamId[u8PathIdx],450000)!= E_VDEC_EX_OK)
    {
        printf("[%s][%d] MApi_VDEC_EX_SetAVSyncFreerunThreshold fail\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    //if (eVDECDevice == E_VDEC_DEVICE_MAIN)
    {
        MDrv_MVOP_Enable(DISABLE);
        printf("Display Main MVOP Disable\n");
    }
#if 0//(DEMO_VDEC_MULTI_TEST == 1)
    else if (eVDECDevice == E_VDEC_DEVICE_SUB)
    {
        MDrv_MVOP_SubEnable(DISABLE);
        printf("Display Sub MVOP Disable\n");
    }
#endif
    MApi_VDEC_EX_SetEvent(&stVDECStreamId[u8PathIdx], E_VDEC_EX_EVENT_DISP_INFO_CHG , _POOL_VDEC_Callback, &stVDECStreamId[u8PathIdx]);

    //Set Field Delay count befor VDEC play
    MApi_VDEC_EX_SetControl(&stVDECStreamId[u8PathIdx],E_VDEC_EX_USER_CMD_FD_MASK_DELAY_COUNT, 16);
    MApi_VDEC_EX_SetControl(&stVDECStreamId[u8PathIdx],E_VDEC_EX_USER_CMD_FAST_DISPLAY, ENABLE);
    MApi_VDEC_EX_SetControl(&stVDECStreamId[u8PathIdx],E_VDEC_EX_USER_CMD_AVSYNC_REPEAT_TH, 0xff);

    //set for FRC
    MApi_VDEC_EX_SetControl(&stVDECStreamId[u8PathIdx], E_VDEC_EX_USER_CMD_FRC_ONLY_SHOW_TOP_FIELD, TRUE);

    /* Control for Crop information */
    MApi_VDEC_EX_SetControl(&stVDECStreamId[u8PathIdx], E_VDEC_EX_USER_CMD_AVC_DISP_IGNORE_CROP,TRUE);

    if (MApi_VDEC_EX_Play(&stVDECStreamId[u8PathIdx]) != E_VDEC_EX_OK)
    {
        printf("[%s][%d] MApi_VDEC_EX_Play Main fail\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    //stVDECInfo[eVDECDevice].bInited = TRUE;
#endif

    PVRPL_Video_SetReachSync(pStreamVideoInfo);
    PVRPL_VIDEO_DBGMSG(E_PVRPL_VIDEO_DBG_TRACE,printf("MAPI VDEC Play OK!!!!!!!!!!!! \n"));
    MApi_DMX_AVFifo_Reset(Pool_GetVFifoType(u8PathIdx), FALSE);

    PVRPL_VIDEO_DBGMSG(E_PVRPL_VIDEO_DBG_TRACE,printf("!!!!!!!!!!!!! non-seamless should be here\n"));

#else
    VDEC_StreamId *VDECStreamId=NULL;

    if (TdalPlayCB != NULL)
    {

        EN_PL_VDEC_CodecType codec = (EN_PL_VDEC_CodecType)u32VCodec;
        // to do
        //need to get resource pool to get VDEC type
        switch (codec)
        {
            case EN_PL_VDEC_CODEC_TYPE_MPEG2:
            case EN_PL_VDEC_CODEC_TYPE_MPEG2_IFRAME:
            {
                PVRPL_VIDEO_DBGMSG(PVRPL_DBG_TRACE,printf("Video MPEG type\n"));
                u32VCodec = E_VDEC_EX_CODEC_TYPE_MPEG2;
                break;
            }
            case EN_PL_VDEC_CODEC_TYPE_H264:
            case EN_PL_VDEC_CODEC_TYPE_H264_IFRAME:
            {
                PVRPL_VIDEO_DBGMSG(PVRPL_DBG_TRACE,printf("Video H264 type\n"));
                u32VCodec = E_VDEC_EX_CODEC_TYPE_H264;
                break;
            }
            case EN_PL_VDEC_CODEC_TYPE_AVS:
            {
                PVRPL_VIDEO_DBGMSG(PVRPL_DBG_TRACE,printf("Video AVS type\n"));
                u32VCodec = E_VDEC_EX_CODEC_TYPE_AVS;
                break;
            }
            case EN_PL_VDEC_CODEC_TYPE_HEVC:
            case EN_PL_VDEC_CODEC_TYPE_HEVC_IFRAME:
            {
                PVRPL_VIDEO_DBGMSG(PVRPL_DBG_TRACE,printf("Video HEVC type\n"));
                u32VCodec = E_VDEC_EX_CODEC_TYPE_HEVC;
                break;
            }
            default:
            {
                PVRPL_VIDEO_DBGMSG(PVRPL_DBG_ERR,printf("Invalid Codec Type %ld\n",u32VCodec));
                return PVRPL_VIDEO_STATUS_ERROR;
            }
        }
        VDECStreamId = (VDEC_StreamId *)TdalPlayCB(u32VCodec,TRUE);
        pStreamVideoInfo->u32Version = VDECStreamId->u32Version;
        pStreamVideoInfo->u32Id = VDECStreamId->u32Id;
    }
    else
    {
        return PVRPL_VIDEO_STATUS_ERROR;
    }

#endif
    return PVRPL_VIDEO_STATUS_OK;
}

static MS_BOOL _PVRPL_Video_IsFFX2CapEnough(VDEC_StreamId *pStreamId)
{
    MS_U32 u32HWCapability = 0, u32Capability = 0;
    VDEC_EX_DispInfo stDispinfo = {};
    // Get display Info.
    if(MApi_VDEC_EX_GetDispInfo(pStreamId,&stDispinfo) != E_VDEC_EX_OK)
    {
         PVRPL_VIDEO_DBGMSG(E_PVRPL_VIDEO_DBG_ERR,printf("[%s][%d] MApi_VDEC_EX_GetDispInfo errror!\n",__FUNCTION__,__LINE__));
         //@NOTE Fixed me! This is the temporary solution.
         // It should return fail when getting display information fail.
         // However, the screen will hang in the case of the program change with 2FF if this function is returned false.
         // For the program change flow with 2FF, getting display information will be fail due to that no data is fed to video decoder.
         // The flow should be refined.
         // return FALSE;
    }

    // Get the HW capability
    VDEC_EX_CODEC_CAP_INFO stVdecCapInfo;
    memset(&stVdecCapInfo, 0, sizeof(VDEC_EX_CODEC_CAP_INFO));
    stVdecCapInfo.u32CodecType = MApi_VDEC_EX_GetActiveCodecType(pStreamId);

    if(MApi_VDEC_EX_GetControl(pStreamId,E_VDEC_EX_USER_CMD_GET_CODEC_CAP,(MS_U32 *)&stVdecCapInfo) != E_VDEC_EX_OK)
    {
        PVRPL_VIDEO_DBGMSG(E_PVRPL_VIDEO_DBG_ERR,printf("[%s][%d] E_VDEC_EX_USER_CMD_GET_CODEC_CAP errror!\n",__FUNCTION__,__LINE__));
        return FALSE;
    }
    u32HWCapability = stVdecCapInfo.u8CodecCapFrameRate * stVdecCapInfo.u16CodecCapWidth * stVdecCapInfo.u16CodecCapHeight;

    // Caculate the capability
    u32Capability = (stDispinfo.u32FrameRate/1000*2)* stDispinfo.u16HorSize * stDispinfo.u16VerSize;

    PVRPL_VIDEO_DBGMSG(E_PVRPL_VIDEO_DBG_TRACE,printf("Codec %"DTC_MS_U32_u"/%"DTC_MS_U32_u", HW Capability= %"DTC_MS_U32_u"*%"DTC_MS_U32_u"*%"DTC_MS_U32_u" = %"DTC_MS_U32_u"\n",
                                              stVdecCapInfo.u32CodecType,(MS_U32)MApi_VDEC_EX_GetActiveCodecType(pStreamId), (MS_U32)stVdecCapInfo.u8CodecCapFrameRate, (MS_U32)stVdecCapInfo.u16CodecCapWidth, (MS_U32)stVdecCapInfo.u16CodecCapHeight, u32HWCapability));
    PVRPL_VIDEO_DBGMSG(E_PVRPL_VIDEO_DBG_TRACE,printf("SW Capability= %"DTC_MS_U32_u"*%"DTC_MS_U32_u"*%"DTC_MS_U32_u" = %"DTC_MS_U32_u"\n",
                                              (MS_U32)stDispinfo.u32FrameRate, (MS_U32)stDispinfo.u16HorSize, (MS_U32)stDispinfo.u16VerSize, u32Capability));
    if(u32Capability > u32HWCapability)
    {
        PVRPL_VIDEO_DBGMSG(E_PVRPL_VIDEO_DBG_ERR,printf("Capability > HW Capability: %"DTC_MS_U32_u" > %"DTC_MS_U32_u"\n",u32Capability,u32HWCapability));
        return FALSE;
    }
    else
    {
        PVRPL_VIDEO_DBGMSG(E_PVRPL_VIDEO_DBG_TRACE,printf("Capability <= HW Capability: %"DTC_MS_U32_u" <= %"DTC_MS_U32_u"\n",u32Capability,u32HWCapability));
        return TRUE;
    }
}

PVRPL_VIDEO_STATUS PVRPL_Video_GetInfo(POOL_VDEC_StreamInfo *pStreamVideoInfo, EN_VIDEO_CPL_INFO eVideoInfoCmd, MS_U32 *u32pmtr, void* structure)
{
    PVR_PATH u8PathIdx = GET_PVR_PATH_IDX(pStreamVideoInfo->u8PathIdx);

    switch(eVideoInfoCmd)
    {
        case EN_VIDEO_CPL_INFO_IS264:
            *u32pmtr = MApi_VDEC_EX_GetActiveCodecType(&stVDECStreamId[u8PathIdx]);
            break;
        case EN_VIDEO_CPL_INFO_ISSTEPDISPDONE:
            *u32pmtr = MApi_VDEC_EX_IsStepDispDone(&stVDECStreamId[u8PathIdx]);
            break;
        case EN_VIDEO_CPL_INFO_ISREACHSYNC:
            *u32pmtr = MApi_VDEC_EX_IsReachSync(&stVDECStreamId[u8PathIdx]);
            break;
        case EN_VIDEO_CPL_INFO_ISAVSYNCON:
            *u32pmtr = MApi_VDEC_EX_IsAVSyncOn(&stVDECStreamId[u8PathIdx]);
            break;
        case EN_VIDEO_CPL_INFO_GET_PTS_HIGH_BIT:
        {
            MS_U64 u64PTS = 0;
            if(MApi_VDEC_EX_GetControl(&stVDECStreamId[u8PathIdx],E_VDEC_EX_USER_CMD_GET_U64PTS, (MS_U32 *)&u64PTS) != E_VDEC_EX_OK)
            {
                 *u32pmtr = 0;
                 PVRPL_VIDEO_DBGMSG(E_PVRPL_VIDEO_DBG_ERR,printf("[%s][%d] E_VDEC_EX_USER_CMD_GET_U64PTS errror!\n",__FUNCTION__,__LINE__));
                 return PVRPL_VIDEO_STATUS_ERROR;
            }
            *u32pmtr = ((u64PTS*90) >> 32);
        }
            break;
        case EN_VIDEO_CPL_INFO_GET_PTS:
            *u32pmtr = MApi_VDEC_EX_GetPTS(&stVDECStreamId[u8PathIdx]);
            break;
        case EN_VIDEO_CPL_INFO_GET_NEXTPTS:
            *u32pmtr = MApi_VDEC_EX_GetNextPTS(&stVDECStreamId[u8PathIdx]);
            break;
        case EN_VIDEO_CPL_INFO_GET_DISP_CNT:
            *u32pmtr = MApi_VDEC_EX_GetDispCnt(&stVDECStreamId[u8PathIdx]);
            break;
        case EN_VIDEO_CPL_INFO_GET_FRAME_CNT:
            *u32pmtr = MApi_VDEC_EX_GetFrameCnt(&stVDECStreamId[u8PathIdx]);
            break;
        case EN_VIDEO_CPL_INFO_GET_SKIP_CNT:
            *u32pmtr = MApi_VDEC_EX_GetSkipCnt(&stVDECStreamId[u8PathIdx]);
            break;
        case EN_VIDEO_CPL_INFO_DROP_CNT:
            *u32pmtr = MApi_VDEC_EX_GetDropCnt(&stVDECStreamId[u8PathIdx]);
            break;
        case EN_VIDEO_CPL_INFO_ERR_CNT:
            *u32pmtr = MApi_VDEC_EX_GetErrCnt(&stVDECStreamId[u8PathIdx]);
            break;
        case EN_VIDEO_CPL_INFO_ES_WP:
            *u32pmtr = MApi_VDEC_EX_GetESWritePtr(&stVDECStreamId[u8PathIdx]);
            break;
        case EN_VIDEO_CPL_INFO_ES_RP:
            *u32pmtr = MApi_VDEC_EX_GetESReadPtr(&stVDECStreamId[u8PathIdx]);
            break;
        case EN_VIDEO_CPL_INFO_WATERLEVEL:
        {
            MS_U32 u32VWptr = MApi_VDEC_EX_GetESWritePtr(&stVDECStreamId[u8PathIdx]);
            MS_U32 u32VRptr = MApi_VDEC_EX_GetESReadPtr(&stVDECStreamId[u8PathIdx]);

            if(u32VWptr >= u32VRptr)
            {
                *u32pmtr = u32VWptr - u32VRptr;
            }
            else
            {
                *u32pmtr = VDEC_BIT_STREAM_LEN - (u32VRptr - u32VWptr);
            }

            if(*u32pmtr > VDEC_BIT_STREAM_LEN)
            {
                *u32pmtr = 0;
                PVRPL_VIDEO_DBGMSG(E_PVRPL_VIDEO_DBG_ERR,printf("Get Vdec waterlevel error!\n"));
                return PVRPL_VIDEO_STATUS_ERROR;
            }
        }
            break;
        case EN_VIDEO_CPL_INFO_GET_PTSSTC_DELTA:
            *u32pmtr = MApi_VDEC_EX_GetVideoPtsStcDelta(&stVDECStreamId[u8PathIdx]);
            break;
        case EN_VIDEO_CPL_INFO_GET_FRAME_RATE:
        {
            VDEC_EX_DispInfo stDispinfo = {};
            if(MApi_VDEC_EX_GetDispInfo(&stVDECStreamId[u8PathIdx],&stDispinfo) != E_VDEC_EX_OK)
            {
                 *u32pmtr = 0;
                 PVRPL_VIDEO_DBGMSG(E_PVRPL_VIDEO_DBG_ERR,printf("[%s][%d] MApi_VDEC_EX_GetDispInfo errror!\n",__FUNCTION__,__LINE__));
                 return PVRPL_VIDEO_STATUS_ERROR;
            }
            *u32pmtr = stDispinfo.u32FrameRate;
        }
            break;
        case EN_VIDEO_CPL_INFO_GET_TOLERANCE:
            *u32pmtr = (MS_U32)u16SyncTolerance;
            break;
        case EN_VIDEO_CPL_INFO_IS_FFX2_CAP_ENOUGH:
        {
            if(_PVRPL_Video_IsFFX2CapEnough(&stVDECStreamId[u8PathIdx]) == TRUE)
                *u32pmtr = 1;
            else
                *u32pmtr = 0;
        }
            break;
        case EN_VIDEO_CPL_INFO_GET_MIN_TSP_DATA_SIZE:
        {
            if(E_VDEC_EX_OK != MApi_VDEC_EX_GetControl(&stVDECStreamId[u8PathIdx],E_VDEC_EX_USER_CMD_GET_MIN_TSP_DATA_SIZE, u32pmtr) || *u32pmtr > VDEC_BIT_STREAM_LEN)
            {
                *u32pmtr = 0;
                PVRPL_VIDEO_DBGMSG(E_PVRPL_VIDEO_DBG_ERR,printf("Vdec user command fail: get min TSP data size\n"));
                return PVRPL_VIDEO_STATUS_ERROR;
            }
        }
            break;
        default:
            return PVRPL_VIDEO_STATUS_ERROR;
    }

    return PVRPL_VIDEO_STATUS_OK;
}

MS_U32 PVRPL_GetWaterLevel(POOL_VDEC_StreamInfo *pStreamVideoInfo,MS_U32 u32MmapSize)
{
    PVR_PATH u8PathIdx = GET_PVR_PATH_IDX(pStreamVideoInfo->u8PathIdx);

    MS_U32 u32WaterLevel = 0;
    MS_U32 u32Offset = 0;

    if(MApi_VDEC_EX_GetESReadPtr(&stVDECStreamId[u8PathIdx])<MApi_VDEC_EX_GetESWritePtr(&stVDECStreamId[u8PathIdx]))
    {
        u32Offset = MApi_VDEC_EX_GetESWritePtr(&stVDECStreamId[u8PathIdx])-MApi_VDEC_EX_GetESReadPtr(&stVDECStreamId[u8PathIdx]);
    }

    if((MApi_VDEC_EX_GetESWritePtr(&stVDECStreamId[u8PathIdx])==MApi_VDEC_EX_GetESReadPtr(&stVDECStreamId[u8PathIdx]))||
        (u32Offset<ESPLAYER_ES_VIDEO_SAFETY_SIZE))
    {
        u32WaterLevel = PVR_MVD_DRV_PROC_SIZE;
    }
    else if(MApi_VDEC_EX_GetESWritePtr(&stVDECStreamId[u8PathIdx])>MApi_VDEC_EX_GetESReadPtr(&stVDECStreamId[u8PathIdx]))
    {
        //    L------------R=======W-------------U
        u32WaterLevel = MApi_VDEC_EX_GetESWritePtr(&stVDECStreamId[u8PathIdx])-MApi_VDEC_EX_GetESReadPtr(&stVDECStreamId[u8PathIdx])+PVR_MVD_DRV_PROC_SIZE;
    }
    else
    {
        //    L=======W-------------R===========U
        u32WaterLevel = u32MmapSize-(MApi_VDEC_EX_GetESReadPtr(&stVDECStreamId[u8PathIdx])-MApi_VDEC_EX_GetESWritePtr(&stVDECStreamId[u8PathIdx]));
    }

    //printf("\n((Read,Write))=(%06lX,%06lX)===> %08lX\n",MApi_VDEC_EX_GetESReadPtr(&stVDECStreamId[u8PathIdx]),MApi_VDEC_EX_GetESWritePtr(&stVDECStreamId[u8PathIdx]),u32WaterLevel);
    //info();
    return u32WaterLevel;
}

PVRPL_VIDEO_STATUS PVRPL_Video_Mute(POOL_VDEC_StreamInfo *pStreamVideoInfo, MS_U8 u8PathID, MS_BOOL bEnable)
{
    PVR_PATH u8PathIdx = GET_PVR_PATH_IDX(pStreamVideoInfo->u8PathIdx);
    VDEC_StreamId VDECStreamId;

    if(pStreamVideoInfo->bVInit == FALSE)
    {
        VDEC_StreamId *pVDECStreamId = Pool_GetVideoID(u8PathID);
        if(pVDECStreamId == NULL)
        {
            PVRPL_VIDEO_DBGMSG(E_PVRPL_VIDEO_DBG_ERR,printf("[%s][%d]Get pVDECStreamId failed (%u)!\n",__FUNCTION__,__LINE__, u8PathID));
            return PVRPL_VIDEO_STATUS_ERROR;
        }
        VDECStreamId.u32Id = pVDECStreamId->u32Id;
        VDECStreamId.u32Version = pVDECStreamId->u32Version;
    }
    else
    {
        VDECStreamId.u32Id = stVDECStreamId[u8PathIdx].u32Id;
        VDECStreamId.u32Version = stVDECStreamId[u8PathIdx].u32Version;
    }

    MApi_VDEC_EX_SetFreezeDisp(&VDECStreamId, bEnable);
    return PVRPL_VIDEO_STATUS_OK;
}

PVRPL_VIDEO_STATUS PVRPL_Video_EnableCMD(POOL_VDEC_StreamInfo *pStreamVideoInfo, EN_VIDEO_CPL_CMD eVideoCmd, MS_U32 u32pmtr)
{
    MS_BOOL bEnable = (MS_BOOL)u32pmtr;
    PVR_PATH u8PathIdx = GET_PVR_PATH_IDX(pStreamVideoInfo->u8PathIdx);
    VDEC_StreamId VDECStreamId;

    if(pStreamVideoInfo->bVInit == FALSE)
    {
        VDEC_StreamId* pVDECStreamId = Pool_GetVideoID(u8PathIdx);
        if(pVDECStreamId == NULL)
        {
            PVRPL_VIDEO_DBGMSG(E_PVRPL_VIDEO_DBG_ERR,printf("[%s][%d]Get pVDECStreamId failed (%u)!\n",__FUNCTION__,__LINE__, u8PathIdx));
            return PVRPL_VIDEO_STATUS_ERROR;
        }
        VDECStreamId.u32Id = pVDECStreamId->u32Id;
        VDECStreamId.u32Version = pVDECStreamId->u32Version;
        PVRPL_VIDEO_DBGMSG(E_PVRPL_VIDEO_DBG_TRACE,printf("[%s][%d](%p)(%"DTC_MS_U32_d") VDECStreamId->u32Id(%"DTC_MS_U32_x") VDECStreamId->u32Version(%"DTC_MS_U32_x")\n",
            __FUNCTION__,__LINE__,pVDECStreamId,(MS_U32)u8PathIdx,pVDECStreamId->u32Id,pVDECStreamId->u32Version));
    }
    else
    {
        VDECStreamId.u32Id = stVDECStreamId[u8PathIdx].u32Id;
        VDECStreamId.u32Version = stVDECStreamId[u8PathIdx].u32Version;
    }

    switch(eVideoCmd)
    {
        case EN_VIDEO_CPL_CMD_LASTFRAMESHOW:
            MApi_VDEC_EX_EnableLastFrameShow(&VDECStreamId, bEnable);
            break;
        case EN_VIDEO_CPL_CMD_ESBUFFMALLOC:
            MApi_VDEC_EX_EnableESBuffMalloc(&VDECStreamId, bEnable);
            break;
        case EN_VIDEO_CPL_CMD_SEETTOPTS:
            MApi_VDEC_EX_SeekToPTS(&VDECStreamId, u32pmtr);
            break;
#if (DEMO_PVR_SUPPORT_TIMESHIFTSEAMLESS_TEST == 1)

        case EN_VDEC_CPL_CMD_TIMESHIFTSEAMLESS:
            if(u32pmtr == 1)// Seamless Start
            {

                //[Step1] Enable FDMASK
                MApi_XC_set_FD_Mask(TRUE); // Just for K3

                //[Step2] Set CMD1:Pause frame

                if(MApi_VDEC_EX_SetControl(&VDECStreamId, E_VDEC_EX_USER_CMD_PVR_TIMESHIFT_SEAMLESS_MODE, 1) == E_VDEC_EX_OK)
                {
                    VDEC_EX_PVR_Seamless_Info SeamlessInfo;
                    if(MApi_VDEC_EX_GetControl(&VDECStreamId, E_VDEC_EX_USER_CMD_GET_PVRSEAMLESS_INFO, (MS_U32*)&SeamlessInfo) != E_VDEC_EX_OK)
                    {
                        PVRPL_VIDEO_DBGMSG(E_PVRPL_VIDEO_DBG_ERR,printf("[%s][%d] E_VDEC_EX_USER_CMD_GET_PVRSEAMLESS_INFO errror!\n",__FUNCTION__,__LINE__));
                    }
                    PVRPL_VIDEO_DBGMSG(E_PVRPL_VIDEO_DBG_MUST,printf("[%s][%d]Pause Frame PTS (0x%"DTC_MS_U64_x")\n",__FUNCTION__,__LINE__,SeamlessInfo.u64PTS));
                    //[Step3] Reset AV FIFO for Timeshift Seamless record start
                    MApi_DMX_AVFifo_Reset(Pool_GetVFifoType(u8PathIdx), TRUE);
                    MApi_DMX_AVFifo_Reset(Pool_GetAFifoType(u8PathIdx), TRUE);

                    //[Step4]Set CMD2:Reset VDEC ESBuffer
                    if(MApi_VDEC_EX_SetControl(&VDECStreamId, E_VDEC_EX_USER_CMD_PVR_TIMESHIFT_SEAMLESS_MODE, 2) != E_VDEC_EX_OK)
                    {
                        PVRPL_VIDEO_DBGMSG(E_PVRPL_VIDEO_DBG_ERR,printf("[%s][%d]Set VDEC Seamless CMD 2 errror!\n",__FUNCTION__,__LINE__));
                        MApi_DMX_AVFifo_Reset(Pool_GetVFifoType(u8PathIdx), FALSE);
                        MApi_DMX_AVFifo_Reset(Pool_GetAFifoType(u8PathIdx), FALSE);
                        return PVRPL_VIDEO_STATUS_ERROR;
                    }
                }
                else
                {
                    PVRPL_VIDEO_DBGMSG(E_PVRPL_VIDEO_DBG_ERR,printf("[%s][%d]Set VDEC Seamless CMD 1 errror!\n",__FUNCTION__,__LINE__));
                    MApi_DMX_AVFifo_Reset(Pool_GetVFifoType(u8PathIdx), FALSE);
                    MApi_DMX_AVFifo_Reset(Pool_GetAFifoType(u8PathIdx), FALSE);
                    return PVRPL_VIDEO_STATUS_ERROR;
                }
            }
            if(u32pmtr == 0)// Seamless error handle
            {
                if(MApi_VDEC_EX_SetControl(&VDECStreamId, E_VDEC_EX_USER_CMD_PVR_TIMESHIFT_SEAMLESS_MODE, 3) != E_VDEC_EX_OK)
                {
                    PVRPL_VIDEO_DBGMSG(E_PVRPL_VIDEO_DBG_ERR,printf("[%s][%d]Set VDEC Seamless CMD 3 errror!\n",__FUNCTION__,__LINE__));
                    return PVRPL_VIDEO_STATUS_ERROR;
                }
            }
            break;
#endif
        default:
            return PVRPL_VIDEO_STATUS_ERROR;
    }
    return PVRPL_VIDEO_STATUS_OK;
}
PVRPL_VIDEO_STATUS PVRPL_Video_SetDecodeMode(POOL_VDEC_StreamInfo *pStreamVideoInfo, MS_S32 s32Speed, EN_VIDEO_CPL_DECODE_TYPE enDecodeType,EN_VIDEO_CPL_DATA_TYPE enDataType)
{
    PVR_PATH u8PathIdx = GET_PVR_PATH_IDX(pStreamVideoInfo->u8PathIdx);

    pStreamVideoInfo->s32VSpeed = s32Speed;

    return _PVRPL_Video_SetDecodeMode(&stVDECStreamId[u8PathIdx], u8PathIdx, pStreamVideoInfo->s32VSpeed, enDecodeType, enDataType, pStreamVideoInfo->eAVSyncMode);
}
PVRPL_VIDEO_STATUS _PVRPL_Video_SetDecodeMode(VDEC_StreamId *pStreamId,MS_U8 u8PathIdx, MS_S32 s32Speed, EN_VIDEO_CPL_DECODE_TYPE enDecodeType,EN_VIDEO_CPL_DATA_TYPE enDataType, EN_PVRPL_AVSYNC_MODE eAVSyncMode)
{
    Pool_SetVideoCurrCmdSetting(u8PathIdx, s32Speed ,enDecodeType, enDataType);
    PVRPL_VIDEO_DBGMSG(E_PVRPL_VIDEO_DBG_TRACE,printf("PVRPL_Video_SetSpeed - speed %"DTC_MS_S32_d" stVDECStreamId:  0x%"DTC_MS_U32_x", Version 0x%"DTC_MS_U32_x" \n",s32Speed,pStreamId->u32Id, pStreamId->u32Version));
    // set cmd by case
    if (s32Speed == 0)
    {
        MApi_VDEC_EX_Pause(pStreamId);
    }
    else if (s32Speed == 1)
    {
        MApi_VDEC_EX_AVSyncOn(pStreamId, FALSE,  u32SyncDelay, u16SyncTolerance);
        MApi_VDEC_EX_Pause(pStreamId);
        _PVRPL_Video_SetDecodeType(pStreamId, enDecodeType);
        _PVRPL_Video_SetMotionPatch(pStreamId, u8PathIdx, TRUE);
        MApi_VDEC_EX_StepDisp(pStreamId);
    }
    // it could be three different modes
    // 1) 1 < speed < 1000 : slowforward
    // 2) speed = 1000 : normal play
    // 3) 1000 < speed <= 2000 : fastforward
    // 4) speed = 2000, and vdec 2FF capability is enough: fastforward
    else if (((s32Speed > 1) && (s32Speed < 2000)) || ((s32Speed == 2000) && (_PVRPL_Video_IsFFX2CapEnough(pStreamId) == TRUE)))
    {
        _PVRPL_Video_SetNormalFowrardMode(pStreamId, s32Speed, enDecodeType, eAVSyncMode);
    }
    // it could be two situations
    // 1) speed = 2000, and vdec 2FF capability is not enough: scan or smooth scan mode
    // 2) speed > 4000 and speed < 0: scan or smooth scan mode
    else
    {
        if (EN_VIDEO_CPL_DATA_TYPE_ONLY_I == enDataType)
        {
            _PVRPL_Video_SetSmoothScanMode(pStreamId, s32Speed, enDecodeType);
        }
        else
        {
            _PVRPL_Video_SetScanMode(pStreamId, s32Speed, enDecodeType);
        }
    }

    return PVRPL_VIDEO_STATUS_OK;
}
PVRPL_VIDEO_STATUS _PVRPL_Video_SetSpeed(VDEC_StreamId *pStreamId, MS_S32 s32Speed)
{
    VDEC_EX_SpeedType enSpeedType;
    VDEC_EX_DispSpeed enSpeed;
    if (s32Speed == 1000)
    {
        enSpeedType = E_VDEC_EX_SPEED_DEFAULT;
        enSpeed = E_VDEC_EX_DISP_SPEED_1X;
    }
    else if ((s32Speed > 0) && (s32Speed < 1000))
    {
        enSpeedType = E_VDEC_EX_SPEED_SLOW;

        if (s32Speed == 500)
        {
            enSpeed = E_VDEC_EX_DISP_SPEED_2X;
        }
        else if (s32Speed == 250)
        {
            enSpeed = E_VDEC_EX_DISP_SPEED_4X;
        }
        else if (s32Speed == 125)
        {
            enSpeed = E_VDEC_EX_DISP_SPEED_8X;
        }
        else if (s32Speed == 64)
        {
            enSpeed = E_VDEC_EX_DISP_SPEED_16X;
        }
        else //if (s32VSpeed == 32)
        {
            enSpeed = E_VDEC_EX_DISP_SPEED_32X;
        }
    }
    else if ((s32Speed > 1000) && (s32Speed <= 2000))
    {
        enSpeedType = E_VDEC_EX_SPEED_FAST;
        enSpeed = E_VDEC_EX_DISP_SPEED_2X;
    }
//#if ENABLE_PLAYER_DECODER_CABILITY
//        else if (*s16OldSpeed == 4000)
//        {
//            _PVRPL_Video_SetNormalMode(u8VideoHandle);
//            enSpeedType = E_VDEC_EX_SPEED_FAST;
//            enSpeed = E_VDEC_EX_DISP_SPEED_4X;
//        }
//#endif /*ENABLE_PLAYER_DECODER_CABILITY*/
    else
    {
        enSpeedType = E_VDEC_EX_SPEED_DEFAULT;
        enSpeed = E_VDEC_EX_DISP_SPEED_1X;
    }
    MApi_VDEC_EX_SetSpeed(pStreamId, enSpeedType, enSpeed);

    return PVRPL_VIDEO_STATUS_OK;
}

PVRPL_VIDEO_STATUS _PVRPL_Video_SetDecodeType(VDEC_StreamId *pStreamId,EN_VIDEO_CPL_DECODE_TYPE eDecodeType)
{
    PVRPL_VIDEO_DBGMSG(E_PVRPL_VIDEO_DBG_TRACE,printf("[%s][%d] Path %d\n", __FUNCTION__, __LINE__,eDecodeType));
    switch(eDecodeType)
    {
        case EN_VIDEO_CPL_DECODE_TYPE_DECODE_I:
            MApi_VDEC_EX_SetTrickMode(pStreamId,E_VDEC_EX_TRICK_DEC_I);
            break;
        case EN_VIDEO_CPL_DECODE_TYPE_DECODE_ALL:
            MApi_VDEC_EX_SetTrickMode(pStreamId,E_VDEC_EX_TRICK_DEC_ALL);
            break;
        default:
            return PVRPL_VIDEO_STATUS_ERROR;

    }
    return PVRPL_VIDEO_STATUS_OK;
}

PVRPL_VIDEO_STATUS PVRPL_Video_StepDisp(POOL_VDEC_StreamInfo *pStreamVideoInfo)
{
    PVR_PATH u8PathIdx = GET_PVR_PATH_IDX(pStreamVideoInfo->u8PathIdx);

    MApi_VDEC_EX_StepDisp(&stVDECStreamId[u8PathIdx]);
    return PVRPL_VIDEO_STATUS_OK;
}

PVRPL_VIDEO_STATUS PVRPL_Video_SetSync(POOL_VDEC_StreamInfo *pStreamVideoInfo, EN_PL_VDEC_TRICK_TYPE type, MS_U16 u16Pid,MS_U8 *u8PidFlt)
{
    PVR_PATH u8PathIdx = GET_PVR_PATH_IDX(pStreamVideoInfo->u8PathIdx);

    printf("Boolean pStreamVideoInfo->bPCRClosed %d path %d\n",pStreamVideoInfo->bPCRClosed,pStreamVideoInfo->u8PathIdx);
    switch(type)
    {
        case EN_PL_VDEC_TRICK_TYPE_NORMAL:
            _PVRPL_Video_SetMotionPatch(&stVDECStreamId[u8PathIdx],u8PathIdx,FALSE);

            if(pStreamVideoInfo->bPCRClosed == TRUE)
            {
                _PVRPL_Video_PIDSwitch(&stVDECStreamId[u8PathIdx],FALSE, PVRPL_PCR_PID, u16Pid, u8PidFlt);
                pStreamVideoInfo->bPCRClosed = FALSE;
            }
        break;
        case EN_PL_VDEC_TRICK_TYPE_FORWARD:
            _PVRPL_Video_SetMotionPatch(&stVDECStreamId[u8PathIdx],u8PathIdx,TRUE);
            if(pStreamVideoInfo->bPCRClosed == FALSE)
            {
                _PVRPL_Video_PIDSwitch(&stVDECStreamId[u8PathIdx],TRUE, PVRPL_PCR_PID, u16Pid, u8PidFlt);
                pStreamVideoInfo->bPCRClosed = TRUE;
            }
        break;
        case EN_PL_VDEC_TRICK_TYPE_SCAN:
            _PVRPL_Video_SetMotionPatch(&stVDECStreamId[u8PathIdx],u8PathIdx,TRUE);

            if(pStreamVideoInfo->bPCRClosed == TRUE)
            {
                _PVRPL_Video_PIDSwitch(&stVDECStreamId[u8PathIdx],FALSE, PVRPL_PCR_PID, u16Pid, u8PidFlt);
                pStreamVideoInfo->bPCRClosed = FALSE;
            }
        break;
        default:
            return PVRPL_VIDEO_STATUS_ERROR;
    }
    return PVRPL_VIDEO_STATUS_OK;
}

/*********************************************************************/
/* PVRPL_Video_SetReachSync:                                                                                 */
/* ReachSync to let vdec to  SetAVSyncRepeatTH                                                        */
/* if sync repath rate is                                                                                            */
/* u32THValue: 0xff: play video after TSP STC is caught up                                            */
/* u32THValue: 0x01: play video with slow motion to catch up TSP STC                            */
/*                                                                                                                         */
/* It will appears Mosaic if run time calls this API in Dual Normal playback                           */
/* As discussion, only call this API once in initial setup                                                   */
/* @return    VIDEO_MODULE_STATUS \b OUT:VIDEO_MODULE_STATUS_OK for success.     */
/*********************************************************************/
PVRPL_VIDEO_STATUS PVRPL_Video_SetReachSync(POOL_VDEC_StreamInfo *pStreamVideoInfo)
{
    PVR_PATH u8PathIdx = GET_PVR_PATH_IDX(pStreamVideoInfo->u8PathIdx);

    MS_U32 u32THValue = 0xff;
    //u32THValue sync:0x01    not sync:0xff
    // static MS_BOOL bsync=false;
    /*
    if (MApi_VDEC_EX_IsReachSync(&VDECStreamId))
    {
        u32THValue = 0x01;
        bsync = TRUE;
    }
    else
    {
        u32THValue = 0xff;
        bsync = FALSE;
    }
    */

    MApi_VDEC_EX_SetControl(&stVDECStreamId[u8PathIdx], E_VDEC_EX_USER_CMD_AVSYNC_REPEAT_TH, u32THValue);

    return PVRPL_VIDEO_STATUS_OK;
}

PVRPL_VIDEO_STATUS PVRPL_Video_SetSyncOnParam(POOL_VDEC_StreamInfo *pStreamVideoInfo, MS_U32 u32MVDSyncDelay, MS_U16 u16MVDSyncTolerance)
{
    u32SyncDelay = u32MVDSyncDelay;
    u16SyncTolerance = u16MVDSyncTolerance;
    return PVRPL_VIDEO_STATUS_OK;
}

PVRPL_VIDEO_STATUS PVRPL_Video_CheckSync(POOL_VDEC_StreamInfo *pStreamVideoInfo)
{
    // @NOTE
    // It seems that the flow is redundant.
    // The frame buffer of video decoder stores the decoded frames and reference frames.
    // The frame will be displayed when the reference frames satisfy the display conditions.
    // Therefore, the frame may not be displayed even thought it buffers 2 decoded frames.
    return PVRPL_VIDEO_STATUS_OK;
}

PVRPL_VIDEO_STATUS PVRPL_Video_PlayXC(POOL_VDEC_StreamInfo *pStreamVideoInfo,MS_U8 u8PathID)
{
#if PVR_VIDEO_XC_PATH
    PVR_PATH u8PathIdx = GET_PVR_PATH_IDX(pStreamVideoInfo->u8PathIdx);

    //Check Video Decoder Status
    if( TRUE != _PVRPL_Video_GetDecoderStatus(&stVDECStreamId[u8PathIdx],u8PathIdx))
    {
        PVRPL_VIDEO_DBGMSG(E_PVRPL_VIDEO_DBG_ERR,printf("[%s][%d]Video Decoder Status is Not Ready!!\n Please Check Out the Code Flow!!\n", __FUNCTION__, __LINE__));
        return PVRPL_VIDEO_STATUS_ERROR;
    }

    //@NOTE When decode is not ready, the MVOP driver can't be setted any change size and position , so we call set win function again in PVRPL_Video_PlayXC here.
    if(bChangeDone[u8PathIdx] == FALSE)
    {
        //set display win
        MSAPI_XC_WINDOW_TYPE stHD_DestWin =
        {
            stDispWinInfo[u8PathIdx].x,
            stDispWinInfo[u8PathIdx].y,
            stDispWinInfo[u8PathIdx].width,
            stDispWinInfo[u8PathIdx].height
        };

        bChangeDone[u8PathIdx] = TRUE;

        _PVRPL_Video_ChangeDisplayWindow(u8PathIdx, &stHD_DestWin);
    }

    if(MDrv_VE_IsBlackScreenEnabled() == TRUE)
    {//Always keep VE un-mute
        msAPI_VE_SetMode();
        msAPI_VE_SetWin(NULL, NULL);
        msAPI_VE_SetVideoMute(DISABLE);
    }
#endif
    return PVRPL_VIDEO_STATUS_OK;
}

PVRPL_VIDEO_STATUS PVRPL_Video_Exit(POOL_VDEC_StreamInfo *pStreamVideoInfo)
{
#if PVR_VIDEO_XC_PATH
    PVR_PATH u8PathIdx = GET_PVR_PATH_IDX(pStreamVideoInfo->u8PathIdx);

    MApi_DMX_AVFifo_Reset(Pool_GetVFifoType(u8PathIdx), TRUE);

    if(u8PathIdx == 0)
        _PVRPL_Video_SC_Mute(0, E_MSAPI_XC_MAIN_WINDOW, TRUE);
    else if(u8PathIdx == 1)
    {
        _PVRPL_Video_SC_Mute(0, E_MSAPI_XC_SUB_WINDOW, TRUE);
#if (DEMO_XC_DUALXC_TEST == 1)
    #if (DEMO_XC_DUALXC_IDENTICAL_DISPLAY_TEST == 1)
    #else
        _PVRPL_Video_SC_Mute(1, E_MSAPI_XC_MAIN_WINDOW, FALSE);
    #endif
#endif
    }

    MApi_VDEC_EX_Exit(&stVDECStreamId[u8PathIdx]);
    memset(&stVDECStreamId[u8PathIdx], 0, sizeof(VDEC_StreamId));

#if (PVR_VSYNC_MODULE_ENABLE == 1)
    if(u8PathIdx == 0)
    {
        msAPI_XC_DIPMultiView_Finalize(E_MSAPI_XC_MAIN_WINDOW);
    }
    else if(u8PathIdx == 1)
    {
        msAPI_XC_DIPMultiView_Finalize(E_MSAPI_XC_SUB_WINDOW);
    }
    // It is for setting stVDECInfo[eDevice].bInited to FALSE,
    // else the next playback would be in a wrong config.
    EN_VDEC_Device eDevice = (u8PathIdx == 0) ? E_VDEC_DEVICE_MAIN : E_VDEC_DEVICE_SUB;
    Demo_VDEC_SetMode(&eDevice, E_VDEC_SetCmd_Initialize, (void *)TRUE);

    ST_AV_INFO stAVInfo = {};
    EN_AV_Device eAVDevice = (u8PathIdx == 0) ? E_AV_DEVICE_MAIN : E_AV_DEVICE_SUB;

    if (Demo_AV_GetAVInfo(&eAVDevice,E_AV_GetCmd_LiveInfo,&stAVInfo)  == FALSE)
    {
        printf("[%s][%d] Error!!!, Get Live info by Demo_AV_GetAVInfo failed\n",__FUNCTION__,__LINE__);
        return FALSE;
    }
    // For two cases, one is seamless timeshifting and program change case,
    // and the other one is seamless timeshift playback stop.
    if(stAVInfo.bInited == TRUE)
    {
        // It should clear the stVDECInfo[eDevice].bInited,
        // otherwise it will not create multiview window.
        Demo_AV_StopAV(&eAVDevice);
    }
    else // For timeshifting and program change case
    {
        // It should reset stAVInfo[*peDevice].stShowFrameInfo.bValid to FALSE,
        // otherwise it will not create multiview window.
        Demo_AV_Initialize(&eAVDevice);
    }
#endif
#else
        if (TdalDispCB != NULL)
        {
            TdalDispCB(FALSE);
        }
        if (TdalPlayCB != NULL)
        {
            TdalPlayCB((MS_U32)0, FALSE);
        }
        else
        {
            return PVRPL_VIDEO_STATUS_ERROR;
        }
#endif

    return PVRPL_VIDEO_STATUS_OK;
}

MS_BOOL _PVRPL_Video_SetNormalFowrardMode(VDEC_StreamId *pStreamId, MS_S32 s32Speed, EN_VIDEO_CPL_DECODE_TYPE enDecodeType, EN_PVRPL_AVSYNC_MODE eAVSyncMode)
{
    _PVRPL_Video_SetSpeed(pStreamId, s32Speed);
    if(eAVSyncMode == E_PVRPL_AV_SYNC_FREE_RUN)
    {
        MApi_VDEC_EX_AVSyncOn(pStreamId,FALSE, u32SyncDelay, u16SyncTolerance);
    }
    else
    {
        MApi_VDEC_EX_AVSyncOn(pStreamId,TRUE, u32SyncDelay, u16SyncTolerance);
    }
    MApi_VDEC_EX_Play(pStreamId);
    _PVRPL_Video_SetDecodeType(pStreamId,enDecodeType);
    return TRUE;
}

MS_BOOL _PVRPL_Video_SetScanMode(VDEC_StreamId *pStreamId, MS_S32 s32Speed, EN_VIDEO_CPL_DECODE_TYPE enDecodeType)
{
    _PVRPL_Video_SetSpeed(pStreamId, s32Speed);
    // @Note Manits 0919691 Need to check whether set decode I
    _PVRPL_Video_SetDecodeType(pStreamId,enDecodeType);
    MApi_VDEC_EX_StepDisp(pStreamId);
    return TRUE;
}

MS_BOOL _PVRPL_Video_SetSmoothScanMode(VDEC_StreamId *pStreamId, MS_S32 s32Speed, EN_VIDEO_CPL_DECODE_TYPE enDecodeType)
{
    _PVRPL_Video_SetSpeed(pStreamId, s32Speed);
    MApi_VDEC_EX_AVSyncOn(pStreamId, FALSE,  u32SyncDelay, u16SyncTolerance);
    // @Note In Smooth Scan setting, need do vdec play cmd befor SetTrickMode
    // Because play-cmd will set the decode ALL on HVC codec.
    MApi_VDEC_EX_Play(pStreamId);
    _PVRPL_Video_SetDecodeType(pStreamId,enDecodeType);
    return TRUE;
}

MS_BOOL _PVRPL_Video_SetMotionPatch(VDEC_StreamId *pStreamId,MS_U8 u8PathIdx,MS_BOOL bEnable)
{
#if (PVR_VSYNC_MODULE_ENABLE == 0)
    MSAPI_XC_DEVICE_ID _stXC_DeviceId[2] = {{0, E_MSAPI_XC_DEVICE0}, {0, E_MSAPI_XC_DEVICE1}};
    E_MSAPI_XC_WINDOW eXCWin = E_MSAPI_XC_MAX_WINDOW;
    PVRPL_VIDEO_GET_XC_WIN_BY_PATH(u8PathIdx,eXCWin);

    PVRPL_VIDEO_DBGMSG(E_PVRPL_VIDEO_DBG_TRACE,printf("Force Motion(%d)!!!\n",bEnable));
    msAPI_XC_EnableMADiForceMotion_EX(&_stXC_DeviceId[0], eXCWin, E_MSAPI_XC_MADI_MOTION_YC, bEnable);
#endif

    if (pStreamId == NULL)
    {
        PVRPL_VIDEO_DBGMSG(E_PVRPL_VIDEO_DBG_ERR,printf("[%s][%d]Error vdec streamid!!\n",__FUNCTION__, __LINE__));
        return FALSE;
    }

    if(MApi_VDEC_EX_SetControl(pStreamId, E_VDEC_EX_USER_CMD_DISP_ONE_FIELD, bEnable) != E_VDEC_EX_OK)
    {
        return FALSE;
    }

    PVRPL_VIDEO_DBGMSG(E_PVRPL_VIDEO_DBG_TRACE,printf("Force Motion(%d) Done!!!\n",bEnable));
    return TRUE;
}

MS_BOOL _PVRPL_Video_PIDSwitch(VDEC_StreamId *pStreamId,MS_BOOL bEnable, PVRPL_PidType ePIDType, MS_U16 u16Pid,MS_U8 *u8PidFlt)
{
    if(ePIDType == PVRPL_PCR_PID)
    {
        MS_U32 u32PCREng = 0;
        MS_U8 u8PCREng = 0;
        if (DMX_FILTER_STATUS_OK != MApi_DMX_Pcr_Get_MapSTC(*u8PidFlt, &u32PCREng))
        {
            printf("Mapping PCR filter %x to STC engine %"DTC_MS_U32_x" Fail !\n", *u8PidFlt, u32PCREng);
        }
        else
        {
            printf("pvr test stc eng = %"DTC_MS_U32_x"\n",u32PCREng);
        }
        u8PCREng = u32PCREng & 0xFF;

        if(bEnable == TRUE)
        {
            PVRPL_VIDEO_DBGMSG(E_PVRPL_VIDEO_DBG_TRACE,printf("[%s][%d]PCR Pid filter ID :%d Stop(HK)\n", __FUNCTION__, __LINE__,u8PCREng));
            MApi_DMX_STC_UpdateCtrl( u8PCREng , DMX_STC_UPDATE_HK );
        }
        else
        {
            PVRPL_VIDEO_DBGMSG(E_PVRPL_VIDEO_DBG_TRACE,printf("[%s][%d]PCR Pid filter ID :%d  Start(FW)\n", __FUNCTION__, __LINE__,u8PCREng));
            MApi_DMX_STC_UpdateCtrl( u8PCREng , DMX_STC_UPDATE_FW );
        }
        return TRUE;
    }
    return FALSE;
}

//------------------------------------------------------------------------------
/// @brief to Get Video Decoder Status and Set Output Config \n
/// - For Video Decoder:
///     - check display information is ready
///     - check frame count is increasing
///     - check AV sync done
/// - For MVOP and SCALER:
///     - get video information and set output information of MVOP
///     - the output of MVOP is the input of SCALER
/// @param[in]  Main or Sub Device
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b DTV_GetDecoderStatus 0 \n
///          \b DTV_GetDecoderStatus 1 \n
//------------------------------------------------------------------------------
MS_BOOL _PVRPL_Video_GetDecoderStatus(VDEC_StreamId *pStreamId,MS_U8 u8PathIdx)
{
    MS_BOOL bSyncFlag = FALSE;
    MS_U32 u32StartTime = 0, u32EndTime = 0;
    MS_U32 u32SyncStarTime, u32SyncTimeDiff;
    MS_U32 u32Loop = PL_VIDEO_FRAMECOUNT_LOOP;
    MS_U32 u32VDEC_DelayTime = 10;

    VDEC_EX_DispInfo stDispinfo;

    if (u8PathIdx >= PVR_VIDEO_MAX_NUM)
    {
        PVRPL_VIDEO_DBGMSG(E_PVRPL_VIDEO_DBG_ERR,printf("[%s][%d] Not support vdec multi \n",__FUNCTION__,__LINE__));
        return FALSE;
    }
    ////////////////////////////////////////////////////////////
    //Check if information for display setting is ready or not                  //
    //And disable VDEC blue screen                                                  //
    ////////////////////////////////////////////////////////////

    u32StartTime = PVRPL_GetSysTime();

    while(MApi_VDEC_EX_CheckDispInfoRdy(pStreamId) != E_VDEC_EX_OK)
    {
        u32Loop--;
        if(u32Loop==0)
        {
            u32EndTime = PVRPL_GetSysTime();
            PVRPL_VIDEO_DBGMSG(E_PVRPL_VIDEO_DBG_ERR,printf("Timeout!! Video DispInfo is Not Ready!!\n"));
            PVRPL_VIDEO_DBGMSG(E_PVRPL_VIDEO_DBG_ERR,printf("DIFF TIME to check Disp Info = %"DTC_MS_U32_u" ms\n", u32EndTime - u32StartTime));
            return FALSE;
        }
        PVRPL_SleepMS(u32VDEC_DelayTime);
    }

    u32EndTime = PVRPL_GetSysTime();
    printf("MApi_VDEC_EX_CheckDispInfoRdy is Ready!!\n");
    printf("DIFF TIME to check Disp Info = %"DTC_MS_U32_u" ms\n", u32EndTime - u32StartTime);

    MApi_VDEC_EX_SetBlueScreen(pStreamId, FALSE );
    /////////////////////////
    //get video information//
    /////////////////////////
    if(MApi_VDEC_EX_GetDispInfo(pStreamId,&stDispinfo) != E_VDEC_EX_OK)
    {
        PVRPL_VIDEO_DBGMSG(E_PVRPL_VIDEO_DBG_ERR,printf("MApi_VDEC_EX_GetDispInfo Failed !! \n"));
        return FALSE;
    }
    else
    {
        printf("MApi_VDEC_EX_GetDispInfo Success !!\n");

    }

#if (PVR_VSYNC_MODULE_ENABLE == 0)
    MVOP_InputSel eMvopInputSel = MVOP_INPUT_UNKNOWN;
    eMvopInputSel = _PVRPL_Video_GetMVOPInputSel(pStreamId);
    _PVRPL_Video_SetMVOP_Config(u8PathIdx,&stDispinfo,eMvopInputSel);
#else
    // Always use PureMCU mode in PVR
    // It will close DS when stoping DTV. Therefore, it should open the DS when playing file.
    EN_VDEC_ShowFrame_Mode eMode = E_VDEC_PureMCU_MODE;
    EN_VDEC_Device eDevice = (u8PathIdx == 0) ? E_VDEC_DEVICE_MAIN : E_VDEC_DEVICE_SUB;
    Demo_VDEC_SetDispConfig(&eDevice,&eMode);
#endif

/*
    Pool_Convert2_msAPIVdecDispInfo(&stDispinfo, &stMSAPIDispinfo);

    msAPI_XC_SetVDECInfo_EX_ByPath(*peDevice, stMSAPIDispinfo);

    if(msAPI_XC_SetMVOPConfig_EX((E_MSAPI_XC_DECODER_PATH)*peDevice, eMvopInputSel) != TRUE)
    {
        PVRPL_VIDEO_DBGMSG(E_PVRPL_VIDEO_DBG_ERR,printf("msAPI_XC_SetMVOPConfig_EX Failed !! \n"));
        return FALSE;
    }
*/
    /////////////////////
    //  check frame Rdy   //
    /////////////////////
    u32Loop = PL_VIDEO_FRAMECOUNT_LOOP;
    u32StartTime = PVRPL_GetSysTime();
    while(MApi_VDEC_EX_IsFrameRdy(pStreamId) != E_VDEC_EX_OK )
    {
        u32Loop--;
        if(u32Loop==0)
        {
            PVRPL_VIDEO_DBGMSG(E_PVRPL_VIDEO_DBG_ERR,printf("Timeout!! MApi_VDEC_EX_IsFrameRdy is Not Ready!!\n"));
            return FALSE;
        }
        PVRPL_SleepMS(u32VDEC_DelayTime);
    }

    u32EndTime = PVRPL_GetSysTime();
    printf("MApi_VDEC_EX_IsFrameRdy is Ready!!\n");
    printf("DIFF TIME to check VDEC Frame ready = %"DTC_MS_U32_u" ms\n",u32EndTime - u32StartTime);
    //////////////////////
    //check AV sync done//
    //////////////////////
    u32SyncStarTime = PVRPL_GetSysTime();
    //if(bVDEC_WaitAVSync == TRUE)
    //{
        while(bSyncFlag != TRUE)
        {
            u32SyncTimeDiff = PVRPL_GetSysTime()- u32SyncStarTime;
            printf("SyncTimeDiff = %"DTC_MS_U32_u" \n",u32SyncTimeDiff);
            if(MApi_VDEC_EX_IsAVSyncOn(pStreamId)&&
               MApi_VDEC_EX_IsReachSync(pStreamId))
            {
               printf("*****************Sync Done %"DTC_MS_U32_d" ms\n", u32SyncTimeDiff);
               bSyncFlag = TRUE;
            }
            else if (u32SyncTimeDiff > AVSYNC_TIMEOUT_INTERVAL) // set timeout
            {
                printf("*****************Sync Timeout %"DTC_MS_U32_d" ms\n", u32SyncTimeDiff);
                bSyncFlag = TRUE;
            }
            PVRPL_SleepMS(u32VDEC_DelayTime);
        }
    //}

    u32EndTime = PVRPL_GetSysTime();
    printf("DIFF TIME to wait AV-Sync = %"DTC_MS_U32_u" ms\n", u32EndTime - u32SyncStarTime);

    return TRUE;
}

#if (PVR_VSYNC_MODULE_ENABLE == 0)
//------------------------------------------------------------------------------
/// @brief get MVOP input select
/// @param[in] Main or Sub Device
/// @param[in] VDEC StreamId
/// @return tMvopInputSel.
/// @return FALSE: Process fail or Invalid input argument.
/// @sa
/// @note
/// Command: \b none \n
//------------------------------------------------------------------------------
MVOP_InputSel _PVRPL_Video_GetMVOPInputSel(VDEC_StreamId* pStreamId)
{
    MVOP_InputSel eMvopInputSel = MVOP_INPUT_UNKNOWN;
/*
    if ((stVDECInfo[eDevice].eVideoCodec == E_DDI_VDEC_CODEC_TYPE_MPEG2_IFRAME)||
        (stVDECInfo[eDevice].eVideoCodec == E_DDI_VDEC_CODEC_TYPE_H264_IFRAME)||
        (stVDECInfo[eDevice].eVideoCodec == E_DDI_VDEC_CODEC_TYPE_AVS_IFRAME))
    {
        eMvopInputSel = MVOP_INPUT_DRAM;
        printf("MVOP_INPUT_MEMORY \n");
    }
*/
    if ((MApi_VDEC_EX_GetActiveCodecType(pStreamId) == E_VDEC_EX_CODEC_TYPE_MPEG2) ||
        (MApi_VDEC_EX_GetActiveCodecType(pStreamId) == E_VDEC_EX_CODEC_TYPE_VC1_ADV) ||
        (MApi_VDEC_EX_GetActiveCodecType(pStreamId) == E_VDEC_EX_CODEC_TYPE_MPEG4) ||
        (MApi_VDEC_EX_GetActiveCodecType(pStreamId) == E_VDEC_EX_CODEC_TYPE_DIVX412) ||
        (MApi_VDEC_EX_GetActiveCodecType(pStreamId) == E_VDEC_EX_CODEC_TYPE_H263))
    {
        eMvopInputSel = MVOP_INPUT_MVD;
        PVRPL_VIDEO_DBGMSG(E_PVRPL_VIDEO_DBG_TRACE,printf("MVOP_INPUT_MVD \n"));
    }
    else if((MApi_VDEC_EX_GetActiveCodecType(pStreamId) == E_VDEC_EX_CODEC_TYPE_H264) ||
        (MApi_VDEC_EX_GetActiveCodecType(pStreamId) == E_VDEC_EX_CODEC_TYPE_VP8) ||
        (MApi_VDEC_EX_GetActiveCodecType(pStreamId) == E_VDEC_EX_CODEC_TYPE_AVS))
    {
        eMvopInputSel = MVOP_INPUT_H264;
        PVRPL_VIDEO_DBGMSG(E_PVRPL_VIDEO_DBG_TRACE,printf("MVOP_INPUT_HVD \n"));
    }
    else if(MApi_VDEC_EX_GetActiveCodecType(pStreamId) == E_VDEC_EX_CODEC_TYPE_HEVC)
    {
        eMvopInputSel = MVOP_INPUT_EVD;
        PVRPL_VIDEO_DBGMSG(E_PVRPL_VIDEO_DBG_TRACE,printf("MVOP_INPUT_HEVC \n"));
    }

    return eMvopInputSel;
}

//-----------------------------------------------------------------------------
/// @brief: Convert vdec diplay info to MSAPI display info
/// @param[in] pstDispInfo vdec display info
/// @param[out] pstMSAPIDispInfo XC MSAPI display info
//-----------------------------------------------------------------------------
MS_BOOL _PVRPL_Convert2_msAPIVdecDispInfo(VDEC_EX_DispInfo *pstDispInfo, MSAPI_XC_VDEC_DispInfo *pstMSAPIDispInfo )
{
    pstMSAPIDispInfo->u16HorSize          = pstDispInfo->u16HorSize;
    pstMSAPIDispInfo->u16VerSize          = pstDispInfo->u16VerSize;
    pstMSAPIDispInfo->u32FrameRate        = pstDispInfo->u32FrameRate;
    pstMSAPIDispInfo->u8Interlace         = pstDispInfo->u8Interlace;
    pstMSAPIDispInfo->u8AFD               = pstDispInfo->u8AFD;
    pstMSAPIDispInfo->u16SarWidth         = pstDispInfo->u16SarWidth;
    pstMSAPIDispInfo->u16SarHeight        = pstDispInfo->u16SarHeight;
    pstMSAPIDispInfo->u16CropRight        = pstDispInfo->u16CropRight;
    pstMSAPIDispInfo->u16CropLeft         = pstDispInfo->u16CropLeft;
    pstMSAPIDispInfo->u16CropBottom       = pstDispInfo->u16CropBottom;
    pstMSAPIDispInfo->u16CropTop          = pstDispInfo->u16CropTop;
    pstMSAPIDispInfo->u16Pitch            = pstDispInfo->u16Pitch;
    pstMSAPIDispInfo->u16PTSInterval      = pstDispInfo->u16PTSInterval;
    pstMSAPIDispInfo->u8MPEG1             = pstDispInfo->u8MPEG1;
    pstMSAPIDispInfo->u8PlayMode          = pstDispInfo->u8PlayMode;
    pstMSAPIDispInfo->u8FrcMode           = pstDispInfo->u8FrcMode;
    pstMSAPIDispInfo->u8AspectRate        = pstDispInfo->u8AspectRate;
    pstMSAPIDispInfo->bWithChroma         = pstDispInfo->bWithChroma;
    pstMSAPIDispInfo->bColorInXVYCC       = pstDispInfo->bColorInXVYCC;
    pstMSAPIDispInfo->u32DynScalingAddr   = pstDispInfo->u32DynScalingAddr;
    pstMSAPIDispInfo->u32DynScalingSize   = pstDispInfo->u32DynScalingSize;
    pstMSAPIDispInfo->u8DynScalingDepth   = pstDispInfo->u8DynScalingDepth;
    pstMSAPIDispInfo->bEnableMIUSel       = pstDispInfo->bEnableMIUSel;
    pstMSAPIDispInfo->u32AspectWidth      = pstDispInfo->u32AspectWidth;
    pstMSAPIDispInfo->u32AspectHeight     = pstDispInfo->u32AspectHeight;

    return TRUE;
}

PVRPL_VIDEO_STATUS _PVRPL_Video_SetMVOP_Config(MS_U8 u8PathIdx, VDEC_EX_DispInfo *stDispinfo,MVOP_InputSel eMvopInputSel)
{
    MSAPI_XC_VDEC_DispInfo stMSAPIDispinfo;
    _PVRPL_Convert2_msAPIVdecDispInfo(stDispinfo, &stMSAPIDispinfo);

    msAPI_XC_SetVDECInfo_EX_ByPath(u8PathIdx, stMSAPIDispinfo);

    if(msAPI_XC_SetMVOPConfig_EX((E_MSAPI_XC_DECODER_PATH)u8PathIdx, eMvopInputSel) != TRUE)
    {
        PVRPL_VIDEO_DBGMSG(E_PVRPL_VIDEO_DBG_TRACE,printf("msAPI_XC_SetMVOPConfig_EX Failed !! \n"));
        return PVRPL_VIDEO_STATUS_ERROR;
    }
    return PVRPL_VIDEO_STATUS_OK;
}
#endif

PVRPL_VIDEO_STATUS _PVRPL_Video_ChangeWinSize(MSAPI_XC_WINDOW_TYPE* stXCDestWin,MS_U32 u32XCDevice, E_MSAPI_XC_WINDOW eXCWin, E_MSAPI_XC_INPUT_SRC eXCInputSrc)
{


    //Set DTV/DTV2/HDMI Rx path to Scaler
    msAPI_XC_SetConnect_EX(&(gstXC_DeviceId[u32XCDevice]), eXCInputSrc, eXCWin);

#if (DEMO_XC_DUALXC_TEST == 1)
    memcpy(&xcpl_stPNL_DeviceId,&gstXC_DeviceId,sizeof(PNL_DeviceId));
#endif

    if(eXCWin == E_MSAPI_XC_MAIN_WINDOW)
    {
        //@NOTE defaul display win size
        if((stXCDestWin->height == 0)&&(stXCDestWin->width == 0 )&&(stXCDestWin->x == 0 )&&(stXCDestWin->y == 0 ))
        {
            msAPI_XC_SetWin_EX(&(gstXC_DeviceId[u32XCDevice]), NULL, NULL, NULL, eXCWin);
        }
        else
        {
            msAPI_XC_SetWin_EX(&(gstXC_DeviceId[u32XCDevice]), NULL, NULL, stXCDestWin, eXCWin);
        }
    }
#if (DEMO_XC_DUALXC_TEST == 1)
    else
    {
        //@NOTE NOT support change sub window now
        PVRPL_VIDEO_DBGMSG(E_PVRPL_VIDEO_DBG_TRACE,printf("Not Support Change Sub Window Size\n"));
        MSAPI_XC_WINDOW_TYPE stDispWin = {0, 0, (IPANEL(&xcpl_stPNL_DeviceId, Width)/3), (IPANEL(&xcpl_stPNL_DeviceId, Height)/3)};
        msAPI_XC_SetWin_EX(&(gstXC_DeviceId[u32XCDevice]), NULL, NULL, &stDispWin, eXCWin);
    }
#endif
    //Below setting only for XC input source DTV/DTV2
    if( MSAPI_XC_IsSrcTypeDTV(eXCInputSrc) || MSAPI_XC_IsSrcTypeCapture(eXCInputSrc) )
    {
        PVRPL_VIDEO_DBGMSG(E_PVRPL_VIDEO_DBG_TRACE,printf("########## XC Black Screen DISABLE!!\n"));
    }

    return PVRPL_VIDEO_STATUS_OK;
}

PVRPL_VIDEO_STATUS _PVRPL_Video_FileInData(DMX_FILEIN_PATH enPathIndex, MS_U32 u32PhyAdr, MS_U32 u32Size)
{
    if (DMX_FILTER_STATUS_OK == MApi_DMX_Filein_Eng_Start(enPathIndex,DMX_PES_NO_BYPASS_FIFO, u32PhyAdr, u32Size))
        return PVRPL_VIDEO_STATUS_OK;
    else
        return PVRPL_VIDEO_STATUS_ERROR;
}
PVRPL_VIDEO_STATUS _PVRPL_Video_FlushFileInEng(DMX_FILEIN_PATH enPathIndex)
{
    //printf("FUCHSIA CALL ============> FLUSH Start !!!\n");
    MS_U32 u32CurrTime = MsOS_GetSystemTime();
    while(MApi_DMX_Filein_Eng_IsIdle(enPathIndex) != TRUE)
    {
        // Delay 3 ms.
        // Reset AV FIFO with 384K * 3 takes 15-20 ms if bypass file-in timestamp.
        if (MsOS_GetSystemTime() - u32CurrTime > CMDQ_TIMEOUT)
        {
            //printf("PVR_TSP::WaitForEmptyCMDQ Fail! IsFileInIdle() time out!\n");
            PVRPL_VIDEO_DBGMSG(E_PVRPL_VIDEO_DBG_ERR,printf("[%s][%s][%d] <ERR> MApi_DMX_Filein_Eng_IsIdle() time out! duration:%"DTC_MS_U32_u"\n",  __FILE__, __FUNCTION__,__LINE__,(MsOS_GetSystemTime() - u32CurrTime)));

            printf("FUCHSIA CALL ============> MApi_DMX_Filein_Eng_IsIdle TimeOut!!!\n");
            return PVRPL_VIDEO_STATUS_ERROR;
        }
        MsOS_DelayTask(3);
     }

    PVRPL_VIDEO_DBGMSG(E_PVRPL_VIDEO_DBG_TRACE,printf("[%s][%s][%d]\n",__FILE__,__FUNCTION__,__LINE__));
    return PVRPL_VIDEO_STATUS_OK;
}

PVRPL_VIDEO_STATUS _PVRPL_Video_FileIn_ResetAVFIFO(MS_U8 u8PathIndex)
{
    if(MApi_DMX_AVFifo_Reset(Pool_GetAFifoType(u8PathIndex), TRUE) == DMX_FILTER_STATUS_ERROR)
    {
        PVRPL_VIDEO_DBGMSG(E_PVRPL_VIDEO_DBG_ERR,printf("[%s][%s][%d] <ERR>\n",__FILE__,__FUNCTION__,__LINE__));
        return PVRPL_VIDEO_STATUS_ERROR;
    }

    if(MApi_DMX_AVFifo_Reset((DMX_FILTER_TYPE)(Pool_GetVFifoType(u8PathIndex)), TRUE) == DMX_FILTER_STATUS_ERROR)
    {
        PVRPL_VIDEO_DBGMSG(E_PVRPL_VIDEO_DBG_ERR,printf("[%s][%s][%d] <ERR>\n",__FILE__,__FUNCTION__,__LINE__));
        return PVRPL_VIDEO_STATUS_ERROR;
    }

    _PVRPL_Video_FlushFileInEng(Pool_GetFileInSouce(u8PathIndex));

    if(MApi_DMX_AVFifo_Reset((DMX_FILTER_TYPE)(Pool_GetAFifoType(u8PathIndex)), FALSE) == DMX_FILTER_STATUS_ERROR)
    {
        PVRPL_VIDEO_DBGMSG(E_PVRPL_VIDEO_DBG_ERR,printf("[%s][%s][%d] <ERR>\n",__FILE__,__FUNCTION__,__LINE__));
        return PVRPL_VIDEO_STATUS_ERROR;
    }

    if(MApi_DMX_AVFifo_Reset((DMX_FILTER_TYPE)(Pool_GetVFifoType(u8PathIndex)), FALSE) == DMX_FILTER_STATUS_ERROR)
    {
        PVRPL_VIDEO_DBGMSG(E_PVRPL_VIDEO_DBG_ERR,printf("[%s][%s][%d] <ERR>\n",__FILE__,__FUNCTION__,__LINE__));
        return PVRPL_VIDEO_STATUS_ERROR;
    }

    PVRPL_VIDEO_DBGMSG(E_PVRPL_VIDEO_DBG_TRACE,printf("[%s][%s][%d]\n",__FILE__,__FUNCTION__,__LINE__));
    return PVRPL_VIDEO_STATUS_OK;
}

static MS_BOOL _PVRPL_Video_GetMinPktNumFlushPatternGen(VDEC_StreamId *pStreamId, MS_U16 *u16PktNum)
{
    MS_U32 u32Num = 0;

    if(MApi_VDEC_EX_GetControl(pStreamId, E_VDEC_EX_USER_CMD_GET_FLUSH_PATTEN_ENTRY_NUM, &u32Num) != E_VDEC_EX_OK || u32Num == 0)
    {
        return FALSE;
    }

    *u16PktNum = u32Num + 1;/// include one PES header

    return TRUE;
}

PVRPL_VIDEO_STATUS _PVRPL_Video_EnableFileInPause(DMX_FILEIN_PATH enPathIndex,MS_BOOL bEnable)
{
    if(bEnable == TRUE)
    {
        if(MApi_DMX_Filein_Eng_Pause(enPathIndex) == DMX_FILTER_STATUS_ERROR)
        {
            PVRPL_VIDEO_DBGMSG(E_PVRPL_VIDEO_DBG_ERR,printf("[%s][%s][%d] <ERR>\n",__FILE__,__FUNCTION__,__LINE__));
            return PVRPL_VIDEO_STATUS_ERROR;
        }
    }
    else
    {
        if(MApi_DMX_Filein_Eng_Resume(enPathIndex) == DMX_FILTER_STATUS_ERROR)
        {
            PVRPL_VIDEO_DBGMSG(E_PVRPL_VIDEO_DBG_ERR,printf("[%s][%s][%d] <ERR>\n",__FILE__,__FUNCTION__,__LINE__));
            return PVRPL_VIDEO_STATUS_ERROR;
        }
    }

    return PVRPL_VIDEO_STATUS_OK;
}
PVRPL_VIDEO_STATUS PVRPL_Video_FlushData(POOL_VDEC_StreamInfo *pStreamVideoInfo, MS_U16 u16VideoPID, MS_U16 u16PacketLen, MS_U32 u32VirAdr)
{
    // Var for video
    MS_U32 u32BufSize = 2048;
    MS_U16 u16Idx = 0;
    MS_U8 u8SendSkipTimer = 0;
    MS_PHY PHYPhyAddr = (MS_PHY)MsOS_VA2PA(u32VirAdr);
    MS_U8* pu8Addr = (MS_U8 *)u32VirAdr;
    MS_U16 u16MostTimes = u32BufSize / u16PacketLen;
    MS_U16 u16LeastTime = 0;
    MS_U16 u16LeastTimeByCodec = 0;
    MS_U32 u32LeastLength = 0;
    MS_U32 u32TsRawSize = 184;
    MS_U32 u32SkipPKTsSize = u16MostTimes * u16PacketLen;
    MS_U32 u32CurrTime1 = 0;
    MS_U32 u32CurrTime2 = 0;
    VDEC_EX_PatternType stVdecType = E_VDEC_EX_PATTERN_FLUSH;
    MS_BOOL bRet = PVRPL_VIDEO_STATUS_ERROR;
    POOL_VDEC_CURR_INFO *stVDECCurrInfo;
    PVR_PATH u8PathIdx = GET_PVR_PATH_IDX(pStreamVideoInfo->u8PathIdx);

    // Var for filein
    DMX_FILEIN_PATH enFilePathIdx = Pool_GetFileInSouce(u8PathIdx);
    MS_BOOL bLastFilePauseState = MApi_DMX_Filein_Eng_IsPause(enFilePathIdx);
    // Var for video
    stVDECCurrInfo = Pool_GetVideoCurrCmdSetting(u8PathIdx);

    if ((u16VideoPID == 0x1FFF) || (u16VideoPID == 0))
    {
        return PVRPL_VIDEO_STATUS_OK;
    }
    memset(pu8Addr, 0xFF, u32BufSize);

    //1. VDEC pause, file in bypass and unpause
    MApi_VDEC_EX_Pause(&stVDECStreamId[u8PathIdx]);
    _PVRPL_Video_EnableFileInPause(enFilePathIdx, FALSE);


    //2. Gen video flush pattern

    // calculate the least time
    u32LeastLength = (MS_U32)MApi_VDEC_EX_GetPatternLeastLength(&stVDECStreamId[u8PathIdx], stVdecType);
    // Only MVD would be in this case
    if (u32LeastLength == (0xFFFFFFFFUL))   // U32_MAX
    {
        // There would be a preceding Null packet at the head of Flush data,
        // so the Flush data packet count is (u16MostTimes - 1)
        u32LeastLength = (u16MostTimes - 1) * u32TsRawSize;
        u8SendSkipTimer = 0xFF;
    }
    else
    {
        u8SendSkipTimer = 5;
    }

    u16LeastTime = (u32LeastLength / u32TsRawSize) + ((u32LeastLength % u32TsRawSize > 0) ? 1 : 0);

    if (_PVRPL_Video_GetMinPktNumFlushPatternGen(&stVDECStreamId[u8PathIdx], &u16LeastTimeByCodec)!= TRUE)
    {
        PVRPL_VIDEO_DBGMSG(E_PVRPL_VIDEO_DBG_ERR,printf("Error to check minimum video flush pattern packet number.\n"));
        PVRPL_VIDEO_DBGMSG(E_PVRPL_VIDEO_DBG_ERR,printf("Abort Video Flush!!!\n"));
        return FALSE;
    }

    if (u16LeastTime < u16LeastTimeByCodec)//send one video flush command at least
    {
        u16LeastTime = u16LeastTimeByCodec;
    }

    u16LeastTime++; // include 1 null pkt

    if (u16LeastTime > u16MostTimes)
    {
        PVRPL_VIDEO_DBGMSG(E_PVRPL_VIDEO_DBG_ERR,printf(" [TS] Make Skip PKT Warning: Buffer is not enough.\n"));
        u16LeastTime = u16MostTimes;
    }


    for (u16Idx = 0; u16Idx < u16MostTimes ; u16Idx++)
    {
        MS_U32 u32Remainder = u32TsRawSize;
        MS_U32 u32FileInTime = 0;

        if(MApi_DMX_Filein_Eng_GetFileInTimeStamp(enFilePathIdx, &u32FileInTime) == DMX_FILTER_STATUS_ERROR)
        {
            PVRPL_VIDEO_DBGMSG(E_PVRPL_VIDEO_DBG_ERR,printf("[%s][%s][%d] <ERR>\n",__FILE__,__FUNCTION__,__LINE__));
            return PVRPL_VIDEO_STATUS_ERROR;
        }

        // filein packet timestamp can't set to zero
        pu8Addr[0] = ((u32FileInTime & 0xff000000) >> 24);
        pu8Addr[1] = ((u32FileInTime & 0x00ff0000) >> 16);
        pu8Addr[2] = ((u32FileInTime & 0x0000ff00) >> 8);
        pu8Addr[3] = (u32FileInTime & 0x000000ff);

        if(u16Idx == 0)
        {
            // Add a null pkt
            // Reason: If data is not 13818 format, 1st pkt will be dropped for synchronizing on file-in case. (TSP HW behavior)
            pu8Addr[4] = 0x47;
            pu8Addr[5] = 0x1F;
            pu8Addr[6] = 0xFF;
            pu8Addr[7] = ((u16Idx & 0xf) | 0x10);
        }
        else if(u16Idx == 1)
        {
            // PES header for MVD parser
            pu8Addr[4] = 0x47;
            pu8Addr[5] = ((MS_U8)(u16VideoPID >> 8) | 0x40); //Vdec flow change (PUSI=1 if there is 1st PTS header in the pkt)
            pu8Addr[6] = (MS_U8)(u16VideoPID);
            pu8Addr[7] = ((u16Idx & 0xf) | 0x10);
            pu8Addr[8] = 0x0;
            pu8Addr[9] = 0x0;
            pu8Addr[10] = 0x01;
            pu8Addr[11] = 0xE0;
            pu8Addr[12] = 0x0;
            pu8Addr[13] = 0x0;
            pu8Addr[14] = 0x80;
            pu8Addr[15] = 0x0;
            pu8Addr[16] = 0x0;
        }
        else if(u16Idx < u16LeastTime)
        {
            // Flush pattern
            pu8Addr[4] = 0x47;
            pu8Addr[5] = (MS_U8)(u16VideoPID >> 8);
            pu8Addr[6] = (MS_U8)(u16VideoPID);
            pu8Addr[7] = ((u16Idx & 0xf) | 0x10);

            MS_PHY PHYPhyAdr = (MS_PHY)MsOS_VA2PA((MS_U32)(pu8Addr + 8));
            MApi_VDEC_EX_GenPattern(&stVDECStreamId[u8PathIdx], stVdecType, PHYPhyAdr, &u32Remainder);
        }
        else
        {
            // Dummy data
            pu8Addr[4] = 0x47;
            pu8Addr[5] = (MS_U8)(u16VideoPID >> 8);
            pu8Addr[6] = (MS_U8)(u16VideoPID);
            pu8Addr[7] = ((u16Idx & 0xf) | 0x10);
        }
        pu8Addr += u16PacketLen;
    }

    MApi_VDEC_EX_EnableLastFrameShow(&stVDECStreamId[u8PathIdx], FALSE);
//For debug: gen patterm
/*
    pu8Addr = (MS_U8 *)u32VirAdr;
    printf("VDEC Pattern:%x , %x, %x\n",u16LeastTime, u16MostTimes, u32SkipPKTsSize);
    MS_U8 i = 0, j=0;
    for(i=0;i< u16MostTimes;i++)
    {
        for(j=0;j<16;j++)
        {
            printf("%x ",*(pu8Addr+j+i*192));
        }
        printf("\n");
        j = 0;
    }
*/
    //3. File in flush pattern and send video flush cmd
    MS_U32 u32Trial = 1;
    while (MApi_VDEC_EX_Flush(&stVDECStreamId[u8PathIdx], E_VDEC_EX_FREEZE_AT_CUR_PIC) != E_VDEC_EX_OK)
    {
        // The Flush Video Data flow:
        // In HVD case should be (1) send flush command. (2) feed flush pattern.
        // In MVD case should be (1) feed flush pattern. (2) send flush command.
        // So the current PVRPL flush data code flow would fail at the first time (because flush command is sent before flush pattern feed)
        // Prevent false alarm error message in MVD case.
        if(u32Trial++ > 1)
        {
            PVRPL_VIDEO_DBGMSG(E_PVRPL_VIDEO_DBG_ERR,printf("MApi_VDEC_EX_Flush Failed !! Trial %"DTC_MS_U32_u"\n", u32Trial));
        }
        bRet = PVRPL_VIDEO_STATUS_OK;
        u32CurrTime1 = PVRPL_GetSysTime();
        while (MApi_DMX_Filein_Eng_IsIdle(u8PathIdx) == FALSE)
        {
            if (PVRPL_GetSysTime() - u32CurrTime1 > 300)
            {
                PVRPL_VIDEO_DBGMSG(E_PVRPL_VIDEO_DBG_ERR,printf("1)Wait MS_DMX_Filein_IsIdle() time out!\n"));
                bRet = PVRPL_VIDEO_STATUS_ERROR;
                break;
            }
        }

        // add file in bloack
        //Push File-in block into TSP
        if(PVRPL_VIDEO_STATUS_OK == bRet)
        {
            u32CurrTime1 = PVRPL_GetSysTime();
            while (_PVRPL_Video_FileInData(u8PathIdx,PHYPhyAddr,u32SkipPKTsSize) == PVRPL_VIDEO_STATUS_ERROR)
            {
                if (PVRPL_GetSysTime() - u32CurrTime1 > 300)
                {
                    PVRPL_VIDEO_DBGMSG(E_PVRPL_VIDEO_DBG_ERR,printf("2)Wait MS_DMX_Filein_Start() time out!\n"));
                    break;
                }
            }
        }
    }

    //4. Check whether finishing to flush frame buffer or not
    u32CurrTime1 = PVRPL_GetSysTime();
    while ((MApi_VDEC_EX_IsAllBufferEmpty(&stVDECStreamId[u8PathIdx]) != E_VDEC_EX_OK))
    {
        bRet = PVRPL_VIDEO_STATUS_OK;
        //to do
        //add TSP related function
        if (PVRPL_GetSysTime() - u32CurrTime1 > 1200)
        {
            PVRPL_VIDEO_DBGMSG(E_PVRPL_VIDEO_DBG_ERR,printf("3)Wait MApi_VDEC_IsAllBufferEmpty() time out!\n"));
            _PVRPL_Video_FileIn_ResetAVFIFO(u8PathIdx);
            break;
        }

        if (u8SendSkipTimer)
        {
            if (u8SendSkipTimer <= 5)
                u8SendSkipTimer--;

            u32CurrTime2 = PVRPL_GetSysTime();
            while (MApi_DMX_Filein_Eng_IsIdle(u8PathIdx) == FALSE)
            {
                if (PVRPL_GetSysTime() - u32CurrTime2 > 400)
                {
                    bRet = PVRPL_VIDEO_STATUS_ERROR;
                    PVRPL_VIDEO_DBGMSG(E_PVRPL_VIDEO_DBG_ERR,printf("4)Wait MS_DMX_Filein_IsIdle() time out!\n"));
                    break;
                }
            }

            u32CurrTime2 = PVRPL_GetSysTime();
            if(PVRPL_VIDEO_STATUS_OK == bRet)
            {
                while (_PVRPL_Video_FileInData(u8PathIdx,PHYPhyAddr,u32SkipPKTsSize) == PVRPL_VIDEO_STATUS_ERROR)
                {
                    if (PVRPL_GetSysTime() - u32CurrTime2 > 300)
                    {
                        PVRPL_VIDEO_DBGMSG(E_PVRPL_VIDEO_DBG_ERR,printf("5)Wait MS_DMX_Filein_Start() time out!\n"));
                        break;
                    }
                }

                PVRPL_SleepMS(5);//prevent sending data too fast.
            }
        }

    }

    //5. Resume the video cmd and file in state
    _PVRPL_Video_SetDecodeMode(&stVDECStreamId[u8PathIdx], u8PathIdx, stVDECCurrInfo->s32Speed, stVDECCurrInfo->enDecodeType, stVDECCurrInfo->enDataType,pStreamVideoInfo->eAVSyncMode);
    _PVRPL_Video_EnableFileInPause(enFilePathIdx, bLastFilePauseState);
    PVRPL_VIDEO_DBGMSG(E_PVRPL_VIDEO_DBG_TRACE,printf("Flush Filein pause status: %x \n",bLastFilePauseState));

    return PVRPL_VIDEO_STATUS_OK;
}

PVRPL_VIDEO_STATUS PVRPL_Video_FileEnd(POOL_VDEC_StreamInfo *pStreamVideoInfo, MS_U16 u16VideoPID, MS_U16 u16PacketLen, MS_U32 u32VirAdr)
{
    MS_U32 u32BufSize = 2048;
    MS_U16 u16Idx = 0;
    MS_PHY PHYPhyAddr = (MS_PHY)MsOS_VA2PA(u32VirAdr);
    MS_U8* pu8Addr = (MS_U8 *)u32VirAdr;
    MS_U16 u16MostTimes = u32BufSize / u16PacketLen;
    MS_U16 u16LeastTime = 0;
    MS_U32 u32LeastLength = 0;
    MS_U32 u32TsRawSize = 184;
    MS_U32 u32SkipPKTsSize = u16MostTimes * u16PacketLen;
    MS_U32 u32CurrTime1 = 0;

    VDEC_EX_PatternType stVdecType = E_VDEC_EX_PATTERN_FILEEND;
    MS_BOOL bRet = PVRPL_VIDEO_STATUS_ERROR;

    PVR_PATH u8PathIdx = GET_PVR_PATH_IDX(pStreamVideoInfo->u8PathIdx);

    if ((u16VideoPID == 0x1FFF) || (u16VideoPID == 0))
    {
        return PVRPL_VIDEO_STATUS_OK;
    }

    memset(pu8Addr, 0xFF, u32BufSize);

    u32LeastLength = (MS_U32)MApi_VDEC_EX_GetPatternLeastLength(&stVDECStreamId[u8PathIdx], stVdecType);
    // Only MVD would be in this case
    if (u32LeastLength == (0xFFFFFFFFUL))   // U32_MAX
    {
        // There would be a preceding Null packet at the head of Flush data,
        // so the Flush data packet count is (u16MostTimes - 1)
        u32LeastLength = (u16MostTimes - 1) * u32TsRawSize;
    }

    u16LeastTime = (u32LeastLength / u32TsRawSize) + ((u32LeastLength % u32TsRawSize > 0) ? 1 : 0);
    if (u16LeastTime <= 1)//send one video flush command at least
        u16LeastTime = 2;
    u16LeastTime++; // include null pkt

    if (u16LeastTime > u16MostTimes)
    {
        PVRPL_VIDEO_DBGMSG(E_PVRPL_VIDEO_DBG_ERR,printf(" [TS] Warrings Make Skip PKT Warring. Buffer is not enough.\n"));
        u16LeastTime = u16MostTimes;
    }

    for (u16Idx = 0; u16Idx < u16MostTimes ; u16Idx++)
    {
        MS_U32 u32Remainder = u32TsRawSize;

        DMX_FILEIN_PATH enFilePathIdx = Pool_GetFileInSouce(u8PathIdx);
        MS_U32 u32FileInTime = 0;
        if(MApi_DMX_Filein_Eng_GetFileInTimeStamp(enFilePathIdx, &u32FileInTime) == DMX_FILTER_STATUS_ERROR)
        {
            PVRPL_VIDEO_DBGMSG(E_PVRPL_VIDEO_DBG_ERR,printf("[%s][%s][%d] <ERR>\n",__FILE__,__FUNCTION__,__LINE__));
            return PVRPL_VIDEO_STATUS_ERROR;
        }

        // filein packet timestamp can't set to zero
        pu8Addr[0] = ((u32FileInTime & 0xff000000) >> 24);
        pu8Addr[1] = ((u32FileInTime & 0x00ff0000) >> 16);
        pu8Addr[2] = ((u32FileInTime & 0x0000ff00) >> 8);
        pu8Addr[3] = (u32FileInTime & 0x000000ff);

        if(u16Idx == 0)
        {
            // Add 1 null pkt
            // Reason: If data is not 13818 format, 1st pkt will be dropped for synchronizing on file-in case. (TSP HW behavior)
            pu8Addr[4] = 0x47;
            pu8Addr[5] = 0x1F;
            pu8Addr[6] = 0xFF;
            pu8Addr[7] = ((u16Idx & 0xf) | 0x10);
        }
        else if(u16Idx == 1)
        {
            // PES header
            pu8Addr[4] = 0x47;
            pu8Addr[5] = ((MS_U8)(u16VideoPID >> 8) | 0x40); //Vdec flow change (PUSI=1 if there is 1st PTS header in the pkt)
            pu8Addr[6] = (MS_U8)(u16VideoPID);
            pu8Addr[7] = ((u16Idx & 0xf) | 0x10);
            pu8Addr[8] = 0x0;
            pu8Addr[9] = 0x0;
            pu8Addr[10] = 0x01;
            pu8Addr[11] = 0xE0;
            pu8Addr[12] = 0x0;
            pu8Addr[13] = 0x0;
            pu8Addr[14] = 0x80;
            pu8Addr[15] = 0x0;
            pu8Addr[16] = 0x0;
        }
        else if (u16Idx < u16LeastTime)
        {
            //
            pu8Addr[4] = 0x47;
            pu8Addr[5] = (MS_U8)(u16VideoPID >> 8);
            pu8Addr[6] = (MS_U8)(u16VideoPID);
            pu8Addr[7] = ((u16Idx & 0xf) | 0x10);

            MS_PHY PHYPhyAdr = MsOS_VA2PA((MS_U32)(pu8Addr + 8));
            MApi_VDEC_EX_GenPattern(&stVDECStreamId[u8PathIdx],stVdecType, PHYPhyAdr, &u32Remainder);
        }
        else
        {
            // Dummy data
            pu8Addr[4] = 0x47;
            pu8Addr[5] = (MS_U8)(u16VideoPID >> 8);
            pu8Addr[6] = (MS_U8)(u16VideoPID);
            pu8Addr[7] = ((u16Idx & 0xf) | 0x10);
        }

        pu8Addr += u16PacketLen;
    }

    MApi_VDEC_EX_EnableLastFrameShow(&stVDECStreamId[u8PathIdx], FALSE);

    bRet = PVRPL_VIDEO_STATUS_OK;

    u32CurrTime1 = PVRPL_GetSysTime();
    //to do
    //add TSP related function
    while (MApi_DMX_Filein_Eng_IsIdle(u8PathIdx) == FALSE)
    {
        if (PVRPL_GetSysTime() - u32CurrTime1 > 300)
        {
            PVRPL_VIDEO_DBGMSG(E_PVRPL_VIDEO_DBG_ERR,printf("1)Wait MS_DMX_Filein_IsIdle() time out!\n"));
            bRet = PVRPL_VIDEO_STATUS_ERROR;
            break;
        }
    }

    if(PVRPL_VIDEO_STATUS_OK == bRet)
    {
        u32CurrTime1 = PVRPL_GetSysTime();
        while (_PVRPL_Video_FileInData(u8PathIdx,PHYPhyAddr,u32SkipPKTsSize) == PVRPL_VIDEO_STATUS_ERROR)
        {
            if (PVRPL_GetSysTime() - u32CurrTime1 > 300)
            {
                PVRPL_VIDEO_DBGMSG(E_PVRPL_VIDEO_DBG_ERR,printf("2)Wait MS_DMX_Filein_Start() time out!\n"));
                break;
            }
        }
// @NOTE Mantis 0919691 why to do the flow twice?
#if 0
        PVRPL_FileIn_FeedFileInBlock(sCurFileInInfo,u32PhyAdr,u32SkipPKTsSize);
        while (PVRPL_FileIn_ConsumeFileInBlock(sCurFileInInfo) == E_PVRPL_FILEIN_FAIL)
        {
            if (PVRPL_GetSysTime() - u32CurrTime1 > 300)
            {
                PVRPL_FILEIN_DBGMSG(E_PVRPL_VIDEO_DBG_ERR,printf("2)Wait MS_DMX_Filein_Start() time out!\n"));
                break;
            }
        }
#endif
    }

    return PVRPL_VIDEO_STATUS_OK;
}

PVRPL_VIDEO_STATUS PVRPL_Video_ChangeDisplayWindow(POOL_VDEC_StreamInfo *pStreamVideoInfo,ST_PVRPL_VDEC_DISPLAYWININFO stDisplayWinInfo)
{
    return PVRPL_VIDEO_STATUS_ERROR;

    PVR_PATH u8PathIdx = GET_PVR_PATH_IDX(pStreamVideoInfo->u8PathIdx);

    //This code flow is for playbackopen -> changewin -> playbackstart case
    if( TRUE != MApi_VDEC_EX_CheckDispInfoRdy(&stVDECStreamId[u8PathIdx]))
    {
        stDispWinInfo[u8PathIdx].x = stDisplayWinInfo.u32X_Pos;
        stDispWinInfo[u8PathIdx].y = stDisplayWinInfo.u32Y_Pos;
        stDispWinInfo[u8PathIdx].width = stDisplayWinInfo.u32Width;
        stDispWinInfo[u8PathIdx].height = stDisplayWinInfo.u32Height;

        bChangeDone[u8PathIdx] = FALSE;
        PVRPL_VIDEO_DBGMSG(E_PVRPL_VIDEO_DBG_ERR,printf("Vdec Disp Info is not ready,so change window is called by PVRPL_Video_PlayXC !!!\n"));
        return PVRPL_VIDEO_STATUS_OK;
    }
    else
        bChangeDone[u8PathIdx] = TRUE;

    //set display win
    MSAPI_XC_WINDOW_TYPE stHD_DestWin =
    {stDisplayWinInfo.u32X_Pos, stDisplayWinInfo.u32Y_Pos, stDisplayWinInfo.u32Width, stDisplayWinInfo.u32Height};

    _PVRPL_Video_ChangeDisplayWindow(u8PathIdx, &stHD_DestWin);

    return PVRPL_VIDEO_STATUS_OK;
}

