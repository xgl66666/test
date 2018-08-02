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

#include "PVRPL_ResourcePool.h"

#include "PVRPL_Video.h"
#include "PVRPL_Sys.h"
#include "PVRPL_FileIn.h"

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
#define PVR_AVSYNC_TIMEOUT_INTERVAL     3000
#define PL_VIDEO_FRAMECOUNT_LOOP    300
#define CMDQ_TIMEOUT 1000

//es player define
#define ESPLAYER_ES_VIDEO_SAFETY_SIZE (0x20)
#define PVR_MVD_DRV_PROC_SIZE (0xA000)

//tsp related
#define _FILEIN_ENG_NUM(x) ((x)? DMX_FILEIN_TSIF1:DMX_FILEIN_TSIF0)
#define PVRPL_VIDEO_DBGMSG(_level,_f) {if(_u32PVRPLVIDEODbgLevel >= (_level)) (_f);}
static MS_U32  _u32PVRPLVIDEODbgLevel = PVRPL_DBG_ERR;

//static MS_BOOL bPCRClosed[PVR_VIDEO_MAX_NUM] = {FALSE,FALSE};

#if (DEMO_XC_DUALXC_TEST == 1)
PNL_DeviceId xcpl_stPNL_DeviceId = {0, 0};
#endif

static MS_U32 u32SyncDelay = 180;
static MS_U16 u16SyncTolerance = 66;
static MSAPI_XC_DEVICE_ID gstXC_DeviceId[PVR_VIDEO_MAX_NUM] = {{0, E_MSAPI_XC_DEVICE0}, {0, E_MSAPI_XC_DEVICE1}};

//@NOTE for change window
static ST_PVRPL_VDEC_DISPLAYWININFO stDispWinInfo[PVR_VIDEO_MAX_NUM];
static MS_BOOL bChangeDone[PVR_VIDEO_MAX_NUM] = {FALSE,FALSE};

static MS_BOOL _PVRPL_Video_Convert2VStreamID(POOL_VDEC_StreamInfo *pStreamVideoID,VDEC_StreamId *StreamVideoID);
static PVRPL_VIDEO_STATUS _PVRPL_Video_SetDecodeType(VDEC_StreamId *pStreamId,EN_VIDEO_CPL_DECODE_TYPE eDecodeType);
static PVRPL_VIDEO_STATUS _PVRPL_Video_SetSpeed(VDEC_StreamId *pStreamId, MS_S16 speed);
static PVRPL_VIDEO_STATUS _PVRPL_Video_SetDecodeMode(VDEC_StreamId *pStreamId,MS_U8 u8PathIdx, MS_S16 s16Speed, EN_VIDEO_CPL_DECODE_TYPE eDecodeType,EN_VIDEO_CPL_DATA_TYPE enDataType);
static MS_BOOL _PVRPL_Video_SetNormalFowrardMode(VDEC_StreamId *pStreamId, MS_S16 speed, EN_VIDEO_CPL_DECODE_TYPE eDecodeType);
static MS_BOOL _PVRPL_Video_SetScanMode(VDEC_StreamId *pStreamId, MS_S16 speed, EN_VIDEO_CPL_DECODE_TYPE eDecodeType);
static MS_BOOL _PVRPL_Video_SetSmoothScanMode(VDEC_StreamId *pStreamId, MS_S16 speed, EN_VIDEO_CPL_DECODE_TYPE eDecodeType);
static MS_BOOL _PVRPL_Video_SetMotionPatch(VDEC_StreamId *pStreamId,MS_U8 u8PathIdx,MS_BOOL bEnable);
static MS_BOOL _PVRPL_Video_PIDSWITCH_FUNC(VDEC_StreamId *pStreamId,MS_BOOL bEnable, MS_U8 PIDType, MS_U16 u16Pid,MS_U8 *u8PidFlt);

static MVOP_InputSel _PVRPL_Video_GetMVOPInputSel(VDEC_StreamId* pStreamId);
#if PVR_VIDEO_XC_PATH
static MS_BOOL _PVRPL_Video_GetDecoderStatus(VDEC_StreamId *pStreamId,MS_U8 u8PathIdx);
#endif
static MS_BOOL _PVRPL_Convert2_msAPIVdecDispInfo(VDEC_EX_DispInfo *pstDispInfo, MSAPI_XC_VDEC_DispInfo *pstMSAPIDispInfo );

#if PVR_VIDEO_XC_PATH
static MS_BOOL _PVRPL_Video_XC_Mute(MS_U8 u8PathIdx);
static PVRPL_VIDEO_STATUS _PVRPL_Video_VE_PlayVideo(void);
#endif

static PVRPL_VIDEO_STATUS _PVRPL_Video_SetMVOP_Config(MS_U8 u8PathIdx, VDEC_EX_DispInfo *stDispinfo,MVOP_InputSel eMvopInputSel);
static PVRPL_VIDEO_STATUS _PVRPL_Video_ChangWinSize(MSAPI_XC_WINDOW_TYPE stXCDestWin,MS_U32 *pu32XCDevice, MS_U32 *pu32XCWindow, MS_U32 *pu32XCInputSrc);

static MS_BOOL _PVRPL_Video_GetMinPktNumFlushPatternGen(VDEC_StreamId *pStreamId, MS_U16 *u16PktNum);
static PVRPL_VIDEO_STATUS _PVRPL_Video_FileInData(DMX_FILEIN_PATH enPathIndex, MS_U32 u32PhyAdr, MS_U32 u32Size);
static PVRPL_VIDEO_STATUS _PVRPL_Video_FlushFileInEng(DMX_FILEIN_PATH enPathIndex);
static PVRPL_VIDEO_STATUS _PVRPL_Video_FileIn_ResetAVFIFO(MS_U8 u8PathIndex);
static PVRPL_VIDEO_STATUS _PVRPL_Video_EnableFileInPause(DMX_FILEIN_PATH enPathIndex,MS_BOOL bEnable);

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
MS_BOOL _PVRPL_Video_STC_Switch(VDEC_StreamId* pStreamId,MS_U8 u8PathIdx, MS_U32 u32STCSource)
{
    //  Indep Command
    VDEC_EX_DecModCfg stDecModCfg;

    memset(&stDecModCfg, 0, sizeof(VDEC_EX_DecModCfg));

    stDecModCfg.eDecMod = E_VDEC_EX_DEC_MODE_DUAL_INDIE;
    stDecModCfg.u8ArgSize = 1;


    /* Control for STC Usage by PCR Filter ID  information */
    if( u8PathIdx != u32STCSource )
    {
        printf("[%s][%d] %d eSTC_Source:%ld : Switch\n",__FUNCTION__,__LINE__,u8PathIdx,u32STCSource);
        stDecModCfg.u32Arg = E_VDEC_EX_DEC_PIP_SYNC_SWITCH;
    }
    else
    {
        printf("[%s][%d] %d eSTC_Source:%ld : INDIE\n",__FUNCTION__,__LINE__,u8PathIdx,u32STCSource);
        stDecModCfg.u32Arg = E_VDEC_EX_DEC_PIP_SYNC_INDIE;
    }

    if (MApi_VDEC_EX_SetControl(pStreamId,E_VDEC_EX_USER_CMD_SET_DECODE_MODE,(MS_U32)&stDecModCfg) != E_VDEC_EX_OK)
    {
        printf("[%s][%d]MApi_VDEC_EX_SetControl return fail!!! :%d\n",__FUNCTION__,__LINE__,MApi_VDEC_EX_SetControl(pStreamId,E_VDEC_EX_USER_CMD_SET_DECODE_MODE,(MS_U32)&stDecModCfg));
    }

    /* Control for Crop information */
    if (MApi_VDEC_EX_SetControl(pStreamId, E_VDEC_EX_USER_CMD_AVC_DISP_IGNORE_CROP,TRUE) != E_VDEC_EX_OK)
    {
        printf("[%s][%d]MApi_VDEC_EX_SetControl return fail!!! :%d\n",__FUNCTION__,__LINE__,MApi_VDEC_EX_SetControl(pStreamId, E_VDEC_EX_USER_CMD_AVC_DISP_IGNORE_CROP,TRUE));
    }

    return TRUE;

}

MS_BOOL _PVRPL_Video_Convert2VStreamID(POOL_VDEC_StreamInfo *pStreamVideoInfo,VDEC_StreamId *StreamVideoID)
{
    StreamVideoID->u32Id = pStreamVideoInfo->u32Id;
    StreamVideoID->u32Version = pStreamVideoInfo->u32Version;
    return TRUE;
}

PVRPL_VIDEO_STATUS PVRPL_Video_Init(POOL_VDEC_StreamInfo *pStreamVideoInfo,MS_U32 u32VCodec,MS_U8 u8STCEng,void *pVideoID)
{
#if PVR_VIDEO_XC_PATH
#if (DEMO_PVR_V4_TEST == 1)
    //@NOTE as same as Demo_VDEC_Init
    VDEC_StreamId VDECStreamId;
    MS_U8 u8PathIdx = pStreamVideoInfo->u8PathIdx;
    _PVRPL_Video_Convert2VStreamID(pStreamVideoInfo,&VDECStreamId);

    VDEC_EX_Status vdecStatus;
    VDEC_EX_InitParam initParam;
    VDEC_EX_CodecType codecType;
    VDEC_EX_CodecType codecTypeCur;

    //Reset/Clear VDEC
    if(pStreamVideoInfo->bVInit == TRUE)
    {
        MApi_VDEC_EX_Rst(&VDECStreamId);
        MApi_VDEC_EX_Exit(&VDECStreamId);
    }

    EN_PL_VDEC_CodecType codec = (EN_PL_VDEC_CodecType)u32VCodec;
    // to do
    //need to get resource pool to get VDEC type

    switch (codec)
    {
        case EN_PL_VDEC_CODEC_TYPE_MPEG2:
        case EN_PL_VDEC_CODEC_TYPE_MPEG2_IFRAME:
        {
            PVRPL_VIDEO_DBGMSG(PVRPL_DBG_TRACE,printf("Video MPEG type\n"));
            codecType = E_VDEC_EX_CODEC_TYPE_MPEG2;
            break;
        }
        case EN_PL_VDEC_CODEC_TYPE_H264:
        case EN_PL_VDEC_CODEC_TYPE_H264_IFRAME:
        {
            PVRPL_VIDEO_DBGMSG(PVRPL_DBG_TRACE,printf("Video H264 type\n"));
            codecType = E_VDEC_EX_CODEC_TYPE_H264;
            break;
        }
        case EN_PL_VDEC_CODEC_TYPE_AVS:
        {
            PVRPL_VIDEO_DBGMSG(PVRPL_DBG_TRACE,printf("Video AVS type\n"));
            codecType = E_VDEC_EX_CODEC_TYPE_AVS;
            break;
        }
        case EN_PL_VDEC_CODEC_TYPE_HEVC:
        case EN_PL_VDEC_CODEC_TYPE_HEVC_IFRAME:
        {
            PVRPL_VIDEO_DBGMSG(PVRPL_DBG_TRACE,printf("Video HEVC type\n"));
            codecType = E_VDEC_EX_CODEC_TYPE_HEVC;
            break;
        }

        default:

        {

            PVRPL_VIDEO_DBGMSG(PVRPL_DBG_ERR,printf("Invalid Codec Type %ld\n",u32VCodec));

            return PVRPL_VIDEO_STATUS_ERROR;

        }
    }
    memset(&vdecStatus, 0, sizeof(VDEC_EX_Status));
    if(VDECStreamId.u32Id==0&&VDECStreamId.u32Version==0)
    {
        printf("[%s][%d]NULL Stream ID\n",__FUNCTION__, __LINE__);
    }
    else
    {
        codecTypeCur = MApi_VDEC_EX_GetActiveCodecType(&VDECStreamId);
        MApi_VDEC_EX_GetStatus(&VDECStreamId,&vdecStatus);
        if ((TRUE == vdecStatus.bInit) && (codecTypeCur == codecType))
        {
            printf("[%s][%d] Path %d => StreamID 0x%lx, Version 0x%lx\n", __FUNCTION__, __LINE__,u8PathIdx,VDECStreamId.u32Id,VDECStreamId.u32Version);
            MApi_DMX_AVFifo_Reset(Pool_GetVFifoType(u8PathIdx), FALSE);
            printf("[%s][%d]....Already Init\n",__FUNCTION__, __LINE__);
            return PVRPL_VIDEO_STATUS_OK;
        }
    }
    memset(&initParam, 0, sizeof(VDEC_EX_InitParam));
    initParam.bDisableDropErrFrame = TRUE;
    initParam.eCodecType = codecType;

    /* TODO START: H264.IFrame */
    //Temp Solution for I-Frame decoder on H264
    /*
    if(codec == EN_PL_VDEC_CODEC_TYPE_H264_IFRAME)
    {
        initParam.VideoInfo.eSrcMode = E_VDEC_EX_SRC_MODE_FILE;
    }
    else
    {
        initParam.VideoInfo.eSrcMode = E_VDEC_EX_SRC_MODE_DTV;
    }
    */
    //for PVR Playback
    //the type needs to be set this enum value
    initParam.VideoInfo.eSrcMode = E_VDEC_EX_SRC_MODE_TS_FILE;


    initParam.SysConfig.u32CodeBufAddr = POOL_VDEC_AEON_ADR;
    initParam.SysConfig.u32CodeBufSize = VDEC_AEON_LEN;
    if (u8PathIdx == 0)
    {
            initParam.SysConfig.u32FrameBufAddr = POOL_VDEC_FRAME_BUF_ADR;
            initParam.SysConfig.u32FrameBufSize = VDEC_FRAME_BUF_LEN;
            initParam.SysConfig.u32BitstreamBufAddr = POOL_VDEC_BIT_STREAM_ADR;
            initParam.SysConfig.u32BitstreamBufSize = VDEC_BIT_STREAM_LEN;
            initParam.SysConfig.u32DrvProcBufAddr = POOL_VDEC_BIT_STREAM_ADR;
    }
#if(DEMO_VDEC_MULTI_TEST == 1)
    else if (u8PathIdx == 1)
    {
        initParam.SysConfig.u32FrameBufAddr = POOL_VDEC_FRAME_BUF1_ADR;
        initParam.SysConfig.u32FrameBufSize = VDEC_FRAME_BUF1_LEN;
        initParam.SysConfig.u32BitstreamBufAddr = POOL_VDEC_BIT_STREAM1_ADR;
        initParam.SysConfig.u32BitstreamBufSize = VDEC_BIT_STREAM1_LEN;
        initParam.SysConfig.u32DrvProcBufAddr = POOL_VDEC_BIT_STREAM1_ADR;
    }
#endif

    initParam.SysConfig.eDbgMsgLevel = E_VDEC_EX_DBG_LEVEL_NONE;

    printf("VDEC FW add = 0x%lx\n", initParam.SysConfig.u32CodeBufAddr);
    printf("VDEC FB add = 0x%lx\n", initParam.SysConfig.u32FrameBufAddr);
    printf("VDEC BS add = 0x%lx\n", initParam.SysConfig.u32BitstreamBufAddr);

    if (E_VDEC_EX_OK != MApi_VDEC_EX_GetFreeStream(&VDECStreamId, sizeof(VDEC_StreamId), Pool_GetStreamType(u8PathIdx), (VDEC_EX_CodecType)initParam.eCodecType))
    {
        printf(" [%d]....GetFreeStream FAILd\n", __LINE__);
        return PVRPL_VIDEO_STATUS_ERROR;
    }
    else
    {
        pStreamVideoInfo->u32Version = VDECStreamId.u32Version;
        pStreamVideoInfo->u32Id      = VDECStreamId.u32Id;
        pStreamVideoInfo->bVInit     = TRUE;
        printf("PVRPL_Video_Init - stVDECStreamId:  0x%lx, Version 0x%lx \n",VDECStreamId.u32Id, VDECStreamId.u32Version);
    }


#if(DEMO_VDEC_ONE_PENDING_BUFFER_MODE_TEST == 1)
    MApi_VDEC_EX_PreSetControl(&VDECStreamId,E_VDEC_EX_USER_CMD_HVD_ONE_PENDING_BUFFER_MODE, TRUE);
#endif

    //Init/Re-Init VDEC
    if (E_VDEC_EX_OK != MApi_VDEC_EX_Init(&VDECStreamId,&initParam))
    {
        printf("*******************************************************\n");
        printf("MApi_VDEC_EX_Init  return fail!!!\n");
        printf("*******************************************************\n");
        return PVRPL_VIDEO_STATUS_ERROR;
    }

    //@NOTE as same as Demo_VDEC_Play
    MApi_VDEC_EX_AVSyncOn(&VDECStreamId,TRUE, u32SyncDelay, u16SyncTolerance);
    //Set Field Delay count befor VDEC play
    MApi_VDEC_EX_SetControl(&VDECStreamId,E_VDEC_EX_USER_CMD_FD_MASK_DELAY_COUNT, 16);
    MApi_VDEC_EX_SetControl(&VDECStreamId,E_VDEC_EX_USER_CMD_FAST_DISPLAY, ENABLE);
    MApi_VDEC_EX_SetControl(&VDECStreamId,E_VDEC_EX_USER_CMD_AVSYNC_REPEAT_TH, 0xff);

    //set the threshold of Freerun condition.If the difference of STC and PTS large than
    // threshold, FW will free run. The threshold value is 90KHZ base.
    if(MApi_VDEC_EX_SetAVSyncFreerunThreshold(&VDECStreamId,450000)!= E_VDEC_EX_OK)
    {
        printf("[%s][%d] MApi_VDEC_EX_SetAVSyncFreerunThreshold fail\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    _PVRPL_Video_STC_Switch(&VDECStreamId,u8PathIdx,u8STCEng);

    Pool_SetMVOP(u8PathIdx,DISABLE);

    if (MApi_VDEC_EX_Play(&VDECStreamId) != E_VDEC_EX_OK)
    {
        PVRPL_VIDEO_DBGMSG(PVRPL_DBG_ERR,printf("[%s][%d] MApi_VDEC_EX_Play fail\n", __FUNCTION__, __LINE__));
        return FALSE;
    }

    PVRPL_Video_SetReachSync(pStreamVideoInfo);
    printf("MAPI VDEC Play OK!!!!!!!!!!!! \n");
    MApi_DMX_AVFifo_Reset(Pool_GetVFifoType(u8PathIdx), FALSE);

#else
        VDEC_StreamId *pVdecStreamID = (VDEC_StreamId *)pVideoID;
        pStreamVideoInfo->u32Version = pVdecStreamID->u32Version;
        pStreamVideoInfo->u32Id      = pVdecStreamID->u32Id;
        pStreamVideoInfo->bVInit     = TRUE;
        //printf("[%s][%s][%d] Vdec version:%ld id:%ld\n",__FILE__,__FUNCTION__,__LINE__,pStreamVideoInfo->u32Version,pStreamVideoInfo->u32Id);
#endif
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


PVRPL_VIDEO_STATUS PVRPL_Video_GetInfo(POOL_VDEC_StreamInfo *pStreamVideoInfo, EN_VIDEO_CPL_INFO eVideoInfoCmd, MS_U32 *u32pmtr, void* structure)
{
    VDEC_StreamId VDECStreamId;
    _PVRPL_Video_Convert2VStreamID(pStreamVideoInfo,&VDECStreamId);

    switch(eVideoInfoCmd)
    {
        case EN_VIDEO_CPL_INFO_IS264:
            *u32pmtr = MApi_VDEC_EX_GetActiveCodecType(&VDECStreamId);
            break;
        case EN_VIDEO_CPL_INFO_ISSTEPDISPDONE:
            *u32pmtr = MApi_VDEC_EX_IsStepDispDone(&VDECStreamId);
            break;
        case EN_VIDEO_CPL_INFO_ISREACHSYNC:
            *u32pmtr = MApi_VDEC_EX_IsReachSync(&VDECStreamId);
            break;
        case EN_VIDEO_CPL_INFO_ISAVSYNCON:
            *u32pmtr = MApi_VDEC_EX_IsAVSyncOn(&VDECStreamId);
            break;
        case EN_VIDEO_CPL_INFO_GET_PTS_HIGH_BIT:
        {
            MS_U64 u64PTS = 0;
            MApi_VDEC_EX_GetControl(&VDECStreamId,E_VDEC_EX_USER_CMD_GET_U64PTS, (MS_U32 *)&u64PTS);
            *u32pmtr = ((u64PTS*90) >> 32);
        }
            break;
        case EN_VIDEO_CPL_INFO_GET_PTS:
            *u32pmtr = MApi_VDEC_EX_GetPTS(&VDECStreamId);
            break;
        case EN_VIDEO_CPL_INFO_GET_NEXTPTS:
            *u32pmtr = MApi_VDEC_EX_GetNextPTS(&VDECStreamId);
            break;
        case EN_VIDEO_CPL_INFO_GET_DISP_CNT:
            *u32pmtr = MApi_VDEC_EX_GetDispCnt(&VDECStreamId);
            break;
        case EN_VIDEO_CPL_INFO_GET_FRAME_CNT:
            *u32pmtr = MApi_VDEC_EX_GetFrameCnt(&VDECStreamId);
            break;
        case EN_VIDEO_CPL_INFO_GET_SKIP_CNT:
            *u32pmtr = MApi_VDEC_EX_GetSkipCnt(&VDECStreamId);
            break;
        case EN_VIDEO_CPL_INFO_DROP_CNT:
            *u32pmtr = MApi_VDEC_EX_GetDropCnt(&VDECStreamId);
            break;
        case EN_VIDEO_CPL_INFO_ERR_CNT:
            *u32pmtr = MApi_VDEC_EX_GetErrCnt(&VDECStreamId);
            break;
        case EN_VIDEO_CPL_INFO_ES_WP:
            *u32pmtr = MApi_VDEC_EX_GetESWritePtr(&VDECStreamId);
            break;
        case EN_VIDEO_CPL_INFO_ES_RP:
            *u32pmtr = MApi_VDEC_EX_GetESReadPtr(&VDECStreamId);
            break;
        case EN_VIDEO_CPL_INFO_WATERLEVEL:

            break;
        case EN_VIDEO_CPL_INFO_GET_PTSSTC_DELTA:
            *u32pmtr = MApi_VDEC_EX_GetVideoPtsStcDelta(&VDECStreamId);
            break;
        default:
            return PVRPL_VIDEO_STATUS_ERROR;
    }
    return PVRPL_VIDEO_STATUS_OK;

}

MS_U32 PVRPL_GetWaterLevel(POOL_VDEC_StreamInfo *pStreamVideoInfo,MS_U32 u32MmapSize)
{
    VDEC_StreamId VDECStreamId;
    _PVRPL_Video_Convert2VStreamID(pStreamVideoInfo,&VDECStreamId);
    MS_U32 u32WaterLevel = 0;
    MS_U32 u32Offset = 0;


    if(MApi_VDEC_EX_GetESReadPtr(&VDECStreamId)<MApi_VDEC_EX_GetESWritePtr(&VDECStreamId))
    {
        u32Offset = MApi_VDEC_EX_GetESWritePtr(&VDECStreamId)-MApi_VDEC_EX_GetESReadPtr(&VDECStreamId);
    }

    if((MApi_VDEC_EX_GetESWritePtr(&VDECStreamId)==MApi_VDEC_EX_GetESReadPtr(&VDECStreamId))||
        (u32Offset<ESPLAYER_ES_VIDEO_SAFETY_SIZE))
    {
        u32WaterLevel = PVR_MVD_DRV_PROC_SIZE;
    }
    else if(MApi_VDEC_EX_GetESWritePtr(&VDECStreamId)>MApi_VDEC_EX_GetESReadPtr(&VDECStreamId))
    {
        //    L------------R=======W-------------U
        u32WaterLevel = MApi_VDEC_EX_GetESWritePtr(&VDECStreamId)-MApi_VDEC_EX_GetESReadPtr(&VDECStreamId)+PVR_MVD_DRV_PROC_SIZE;
    }
    else
    {
        //    L=======W-------------R===========U
        u32WaterLevel = u32MmapSize-(MApi_VDEC_EX_GetESReadPtr(&VDECStreamId)-MApi_VDEC_EX_GetESWritePtr(&VDECStreamId));
    }

    //printf("\n((Read,Write))=(%06lX,%06lX)===> %08lX\n",MApi_VDEC_EX_GetESReadPtr(pStreamID),MApi_VDEC_EX_GetESWritePtr(pStreamID),u32WaterLevel);
    //info();
    return u32WaterLevel;
}


PVRPL_VIDEO_STATUS PVRPL_Video_Mute(POOL_VDEC_StreamInfo *pStreamVideoInfo, MS_U8 u8PathID, MS_BOOL bEnable)
{
    VDEC_StreamId VDECStreamId;
    _PVRPL_Video_Convert2VStreamID(pStreamVideoInfo,&VDECStreamId);


    if(pStreamVideoInfo->bVInit == FALSE)
    {
        MApi_VDEC_EX_SetFreezeDisp(Pool_GetVideoID(u8PathID), bEnable);
        return PVRPL_VIDEO_STATUS_OK;
    }

    MApi_VDEC_EX_SetFreezeDisp(&VDECStreamId, bEnable);
    return PVRPL_VIDEO_STATUS_OK;
}


PVRPL_VIDEO_STATUS PVRPL_Video_EnableCMD(POOL_VDEC_StreamInfo *pStreamVideoInfo, EN_VIDEO_CPL_CMD eVideoCmd, MS_U32 u32pmtr)
{
    MS_BOOL bEnable = (MS_BOOL)u32pmtr;
    VDEC_StreamId VDECStreamId;
    _PVRPL_Video_Convert2VStreamID(pStreamVideoInfo,&VDECStreamId);

    if(pStreamVideoInfo->bVInit == FALSE)
    {
        VDEC_StreamId* pVDECStreamId = Pool_GetVideoID(pStreamVideoInfo->u8PathIdx);
        VDECStreamId.u32Id = pVDECStreamId->u32Id;
        VDECStreamId.u32Version = pVDECStreamId->u32Version;
        printf("[%s][%d]VDECStreamId->u32Id(%lx) VDECStreamId->u32Version(%lx)\n",__FUNCTION__,__LINE__,VDECStreamId.u32Id,VDECStreamId.u32Version);
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
                    MApi_VDEC_EX_GetControl(&VDECStreamId, E_VDEC_EX_USER_CMD_GET_PVRSEAMLESS_INFO, (MS_U32*)&SeamlessInfo);
                    PVRPL_VIDEO_DBGMSG(PVRPL_DBG_NONE,printf("[%s][%d]Pause Frame PTS (%llx)\n",__FUNCTION__,__LINE__,SeamlessInfo.u64PTS));
                    //[Step3] Reset AV FIFO for Timeshift Seamless record start
                    MApi_DMX_AVFifo_Reset(Pool_GetVFifoType(pStreamVideoInfo->u8PathIdx), TRUE);
                    MApi_DMX_AVFifo_Reset(Pool_GetAFifoType(pStreamVideoInfo->u8PathIdx), TRUE);

                    //[Step4]Set CMD2:Reset VDEC ESBuffer
                    if(MApi_VDEC_EX_SetControl(&VDECStreamId, E_VDEC_EX_USER_CMD_PVR_TIMESHIFT_SEAMLESS_MODE, 2) != E_VDEC_EX_OK)
                    {
                        PVRPL_VIDEO_DBGMSG(PVRPL_DBG_ERR,printf("[%s][%d]Set VDEC Seamless CMD 2 errror!\n",__FUNCTION__,__LINE__));
                        MApi_DMX_AVFifo_Reset(Pool_GetVFifoType(pStreamVideoInfo->u8PathIdx), FALSE);
                        MApi_DMX_AVFifo_Reset(Pool_GetAFifoType(pStreamVideoInfo->u8PathIdx), FALSE);
                        return PVRPL_VIDEO_STATUS_ERROR;
                    }
                }
                else
                {
                    PVRPL_VIDEO_DBGMSG(PVRPL_DBG_ERR,printf("[%s][%d]Set VDEC Seamless CMD 1 errror!\n",__FUNCTION__,__LINE__));
                    MApi_DMX_AVFifo_Reset(Pool_GetVFifoType(pStreamVideoInfo->u8PathIdx), FALSE);
                    MApi_DMX_AVFifo_Reset(Pool_GetAFifoType(pStreamVideoInfo->u8PathIdx), FALSE);
                    return PVRPL_VIDEO_STATUS_ERROR;
                }
            }
            if(u32pmtr == 0)// Seamless error handle
            {
                if(MApi_VDEC_EX_SetControl(&VDECStreamId, E_VDEC_EX_USER_CMD_PVR_TIMESHIFT_SEAMLESS_MODE, 3) != E_VDEC_EX_OK)
                {
                    PVRPL_VIDEO_DBGMSG(PVRPL_DBG_ERR,printf("[%s][%d]Set VDEC Seamless CMD 3 errror!\n",__FUNCTION__,__LINE__));
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
PVRPL_VIDEO_STATUS PVRPL_Video_SetDecodeMode(POOL_VDEC_StreamInfo *pStreamVideoInfo, MS_S16 speed, EN_VIDEO_CPL_DECODE_TYPE enDecodeType,EN_VIDEO_CPL_DATA_TYPE enDataType)
{
    VDEC_StreamId VDECStreamId;
    _PVRPL_Video_Convert2VStreamID(pStreamVideoInfo,&VDECStreamId);

    pStreamVideoInfo->s16VSpeed = speed;

    return _PVRPL_Video_SetDecodeMode(&VDECStreamId, pStreamVideoInfo->u8PathIdx, pStreamVideoInfo->s16VSpeed, enDecodeType, enDataType);
}
PVRPL_VIDEO_STATUS _PVRPL_Video_SetDecodeMode(VDEC_StreamId *pStreamId,MS_U8 u8PathIdx, MS_S16 s16Speed, EN_VIDEO_CPL_DECODE_TYPE enDecodeType,EN_VIDEO_CPL_DATA_TYPE enDataType)
{

    Pool_SetVideoCurrCmdSetting(u8PathIdx, s16Speed ,enDecodeType, enDataType);

    PVRPL_VIDEO_DBGMSG(PVRPL_DBG_TRACE,printf("PVRPL_Video_SetSpeed - speed %d stVDECStreamId:  0x%"DTC_MS_U32_x", Version 0x%"DTC_MS_U32_x" \n",s16Speed,pStreamId->u32Id, pStreamId->u32Version));

    // set cmd by case
    if (s16Speed == 0)
    {
        MApi_VDEC_EX_Pause(pStreamId);
    }
    else if (s16Speed == 1)
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
    else if ((s16Speed > 1) && (s16Speed <= 2000))
    {
        _PVRPL_Video_SetNormalFowrardMode(pStreamId, s16Speed, enDecodeType);
    }
    else
    {
        if (EN_VIDEO_CPL_DATA_TYPE_ONLY_I == enDataType)
        {
            _PVRPL_Video_SetSmoothScanMode(pStreamId, s16Speed, enDecodeType);
        }
        else
        {
            _PVRPL_Video_SetScanMode(pStreamId, s16Speed, enDecodeType);
        }
    }
    return PVRPL_VIDEO_STATUS_OK;

}
PVRPL_VIDEO_STATUS _PVRPL_Video_SetSpeed(VDEC_StreamId *pStreamId, MS_S16 speed)
{
    VDEC_EX_SpeedType enSpeedType;
    VDEC_EX_DispSpeed enSpeed;

    if (speed == 1000)
    {
        enSpeedType = E_VDEC_EX_SPEED_DEFAULT;
        enSpeed = E_VDEC_EX_DISP_SPEED_1X;
    }
    else if ((speed > 0) && (speed < 1000))
    {
        enSpeedType = E_VDEC_EX_SPEED_SLOW;

        if (speed == 500)
        {
            enSpeed = E_VDEC_EX_DISP_SPEED_2X;
        }
        else if (speed == 250)
        {
            enSpeed = E_VDEC_EX_DISP_SPEED_4X;
        }
        else if (speed == 125)
        {
            enSpeed = E_VDEC_EX_DISP_SPEED_8X;
        }
        else if (speed == 64)
        {
            enSpeed = E_VDEC_EX_DISP_SPEED_16X;
        }
        else //if (s16VSpeed == 32)
        {
            enSpeed = E_VDEC_EX_DISP_SPEED_32X;
        }
    }
    else if ((speed > 1000) && (speed <= 2000))
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

    PVRPL_VIDEO_DBGMSG(PVRPL_DBG_TRACE,printf("[%s][%d] Path %d\n", __FUNCTION__, __LINE__,eDecodeType));
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
    VDEC_StreamId VDECStreamId;
    _PVRPL_Video_Convert2VStreamID(pStreamVideoInfo,&VDECStreamId);
    MApi_VDEC_EX_StepDisp(&VDECStreamId);
    return PVRPL_VIDEO_STATUS_OK;
}


PVRPL_VIDEO_STATUS PVRPL_Video_SetSync(POOL_VDEC_StreamInfo *pStreamVideoInfo, EN_PL_VDEC_TRICK_TYPE type, MS_U16 u16Pid,MS_U8 *u8PidFlt)
{
    VDEC_StreamId VDECStreamId;
    _PVRPL_Video_Convert2VStreamID(pStreamVideoInfo,&VDECStreamId);

printf("Boolean pStreamVideoInfo->bPCRClosed %d path %d\n",pStreamVideoInfo->bPCRClosed,pStreamVideoInfo->u8PathIdx);
    switch(type)
    {
        case EN_PL_VDEC_TRICK_TYPE_NORMAL:
            _PVRPL_Video_SetMotionPatch(&VDECStreamId,pStreamVideoInfo->u8PathIdx,FALSE);

            if(pStreamVideoInfo->bPCRClosed == TRUE)
            {
                _PVRPL_Video_PIDSWITCH_FUNC(&VDECStreamId,FALSE, PVRPL_PCR_PID, u16Pid, u8PidFlt);
                pStreamVideoInfo->bPCRClosed = FALSE;
            }
        break;
        case EN_PL_VDEC_TRICK_TYPE_FORWARD:
            _PVRPL_Video_SetMotionPatch(&VDECStreamId,pStreamVideoInfo->u8PathIdx,FALSE);

            if(pStreamVideoInfo->bPCRClosed == FALSE)
            {
                _PVRPL_Video_PIDSWITCH_FUNC(&VDECStreamId,TRUE, PVRPL_PCR_PID, u16Pid, u8PidFlt);
                pStreamVideoInfo->bPCRClosed = TRUE;
            }
        break;
        case EN_PL_VDEC_TRICK_TYPE_SCAN:
            _PVRPL_Video_SetMotionPatch(&VDECStreamId,pStreamVideoInfo->u8PathIdx,TRUE);

            if(pStreamVideoInfo->bPCRClosed == TRUE)
            {
                _PVRPL_Video_PIDSWITCH_FUNC(&VDECStreamId,FALSE, PVRPL_PCR_PID, u16Pid, u8PidFlt);
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
    VDEC_StreamId VDECStreamId;
    _PVRPL_Video_Convert2VStreamID(pStreamVideoInfo,&VDECStreamId);

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
    MApi_VDEC_EX_SetControl(&VDECStreamId, E_VDEC_EX_USER_CMD_AVSYNC_REPEAT_TH, u32THValue);

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
    VDEC_StreamId VDECStreamId;
    _PVRPL_Video_Convert2VStreamID(pStreamVideoInfo,&VDECStreamId);

    MS_U16 u16FrameTHreshold= 2;
    MS_U32 u32Loop=400;
       
    while(MApi_VDEC_EX_GetFrameCnt(&VDECStreamId)<u16FrameTHreshold)
    {
        u32Loop--;
        if(u32Loop<=0)
        {
           printf("TimeOut MApi_VDEC_EX_GetFrameCnt=0x%"DTC_MS_U32_x"\n",MApi_VDEC_EX_GetFrameCnt(&VDECStreamId));
           printf("VDECStreamId:  0x%"DTC_MS_U32_u", Version 0x%"DTC_MS_U32_u" \n",VDECStreamId.u32Id, VDECStreamId.u32Version);
           return PVRPL_VIDEO_STATUS_ERROR;
        }

        PVRPL_SleepMS(10);
    }

    return PVRPL_VIDEO_STATUS_OK;
}

PVRPL_VIDEO_STATUS PVRPL_Video_PlayXC(POOL_VDEC_StreamInfo *pStreamVideoInfo,MS_U8 u8PathID)
{
#if PVR_VIDEO_XC_PATH
#if (DEMO_PVR_V4_TEST == 1)
    MS_U32 u32XC_Device = 0;
    MS_U32 u32XC_Window = 0;
    VDEC_StreamId VDECStreamId;
    _PVRPL_Video_Convert2VStreamID(pStreamVideoInfo,&VDECStreamId);

   //Check Video Decoder Status
   if( TRUE != _PVRPL_Video_GetDecoderStatus(&VDECStreamId,pStreamVideoInfo->u8PathIdx))
   {
       PVRPL_VIDEO_DBGMSG(PVRPL_DBG_ERR,printf("[%s][%d]Video Decoder Status is Not Ready!!\n Please Check Out the Code Flow!!\n", __FUNCTION__, __LINE__));
       return PVRPL_VIDEO_STATUS_ERROR;
   }
   else
   {

        //@NOTE When decode is not ready, the MVOP driver can't be setted any change size and position , so we call set win function again in PVRPL_Video_PlayXC here.
        if(bChangeDone[pStreamVideoInfo->u8PathIdx] == FALSE)
        {
                //set display win
                MSAPI_XC_WINDOW_TYPE HD_DestWin;

                HD_DestWin.height                              =   stDispWinInfo[pStreamVideoInfo->u8PathIdx].u32Height;
                HD_DestWin.width                               =   stDispWinInfo[pStreamVideoInfo->u8PathIdx].u32Width;
                HD_DestWin.x                                   =   stDispWinInfo[pStreamVideoInfo->u8PathIdx].u32X_Pos;
                HD_DestWin.y                                   =   stDispWinInfo[pStreamVideoInfo->u8PathIdx].u32Y_Pos;
                bChangeDone[pStreamVideoInfo->u8PathIdx]       =   TRUE;

                MS_U32 u32XC_Device = 0;
                MS_U32 u32XC_Window = 0;
                MS_U32 u32XC_Inputsrc = 0;
                if (pStreamVideoInfo->u8PathIdx == 0)
                {
                    u32XC_Device = 0; //HD PATH
                    u32XC_Window = 0; // Main Window
                    u32XC_Inputsrc = EN_PVRPL_XC_INPUT_SOURCE_DTV;
                    _PVRPL_Video_ChangWinSize(HD_DestWin,&u32XC_Device,&u32XC_Window,&u32XC_Inputsrc);

                    //@NOTE Drvier behavior
                    //There is the other xc1 path to CVBS , so need to set up xc1.
                    #if (DEMO_XC_DUALXC_TEST == 1)
                    u32XC_Device = 1; //SD PATH
                    u32XC_Window = 0; // Main Window

                    #ifndef temp_Kano
                    u32XC_Inputsrc = EN_PVRPL_XC_INPUT_SOURCE_DTV;
                    #else
                    u32XC_Inputsrc = EN_PVRPL_XC_INPUT_SOURCE_SCALER0_OP;
                    #endif

                    PVRPL_VIDEO_DBGMSG(PVRPL_DBG_TRACE,printf("PVR Main for SD path(Main window) => Demo_XC_PlayVideo from source DTV\n"));
                    PVRPL_VIDEO_DBGMSG(PVRPL_DBG_TRACE,printf("[%s]Trace XC Function: msAPI_XC_SetWin_EX\n",__FUNCTION__));
                    _PVRPL_Video_ChangWinSize(HD_DestWin,&u32XC_Device,&u32XC_Window,&u32XC_Inputsrc);
                    #endif
                }
                else
                {
                    u32XC_Device = 0; //HD PATH
                    u32XC_Window = 1; // Sub Window
                    u32XC_Inputsrc = EN_PVRPL_XC_INPUT_SOURCE_DTV2;
                    _PVRPL_Video_ChangWinSize(HD_DestWin,&u32XC_Device,&u32XC_Window,&u32XC_Inputsrc);
                }
        }

        //@NOTE Disable block screen !!!
       if(pStreamVideoInfo->u8PathIdx == 0) //main path
       {

           u32XC_Device = 0; //HD PATH
           u32XC_Window = 0; // Main Window

           if(msAPI_XC_IsBlackVideoEnable_EX(&(gstXC_DeviceId[u32XC_Device]),E_MSAPI_XC_MAIN_WINDOW) == TRUE)
           {   //Disable Black Screen

               PVRPL_VIDEO_DBGMSG(PVRPL_DBG_TRACE,printf("[%s][%s][%d]    !!!\n",__FILE__,__FUNCTION__,__LINE__));
               PVRPL_VIDEO_DBGMSG(PVRPL_DBG_TRACE,printf("[%s]Trace XC Function: msAPI_XC_SetVideoMuteByMode_EX\n",__FUNCTION__));
               msAPI_XC_SetVideoMuteByMode_EX(&(gstXC_DeviceId[u32XC_Device]), DISABLE, (E_MSAPI_XC_WINDOW)u32XC_Window, E_MSAPI_XC_VIDEO_MUTE_MODE_AUTO_SYNC);
           }

       #if (DEMO_XC_DUALXC_TEST == 1)
               u32XC_Device = 1; //SD PATH
               u32XC_Window = 0; // Main Window

               if(msAPI_XC_IsBlackVideoEnable_EX(&(gstXC_DeviceId[u32XC_Device]),E_MSAPI_XC_MAIN_WINDOW) == TRUE)
               {   //Disable Black Screen

                   PVRPL_VIDEO_DBGMSG(PVRPL_DBG_TRACE,printf("[%s][%s][%d]    !!!\n",__FILE__,__FUNCTION__,__LINE__));
                   PVRPL_VIDEO_DBGMSG(PVRPL_DBG_TRACE,printf("[%s]Trace XC Function: msAPI_XC_SetVideoMuteByMode_EX\n",__FUNCTION__));
                   msAPI_XC_SetVideoMuteByMode_EX(&(gstXC_DeviceId[u32XC_Device]), DISABLE, (E_MSAPI_XC_WINDOW)u32XC_Window, E_MSAPI_XC_VIDEO_MUTE_MODE_AUTO_SYNC);
               }
       #endif
           _PVRPL_Video_VE_PlayVideo();
       }
       else //sub path
       {
           u32XC_Device = 0; //HD PATH
           u32XC_Window = 1; //sub Window
           if(msAPI_XC_IsBlackVideoEnable_EX(&(gstXC_DeviceId[u32XC_Device]), E_MSAPI_XC_SUB_WINDOW) == TRUE)
           {   //Disable Black Screen

               PVRPL_VIDEO_DBGMSG(PVRPL_DBG_TRACE,printf("[%s][%s][%d]    !!!\n",__FILE__,__FUNCTION__,__LINE__));
               PVRPL_VIDEO_DBGMSG(PVRPL_DBG_TRACE,printf("[%s]Trace XC Function: msAPI_XC_SetVideoMuteByMode_EX\n",__FUNCTION__));
               msAPI_XC_SetVideoMuteByMode_EX(&(gstXC_DeviceId[u32XC_Device]), DISABLE, (E_MSAPI_XC_WINDOW)u32XC_Window, E_MSAPI_XC_VIDEO_MUTE_MODE_AUTO_SYNC);
           }
       }
   }
#endif
#else
    #if 0 // dont have to wait sync status, it's already done by tdal_video   
    VDEC_StreamId VDECStreamId;
    _PVRPL_Video_Convert2VStreamID(pStreamVideoInfo,&VDECStreamId);
    if( TRUE != _PVRPL_Video_GetDecoderStatus(&VDECStreamId,pStreamVideoInfo->u8PathIdx))
    {
       PVRPL_VIDEO_DBGMSG(PVRPL_DBG_ERR,printf("[%s][%d]Video Decoder Status is Not Ready!!\n Please Check Out the Code Flow!!\n", __FUNCTION__, __LINE__));
       return PVRPL_VIDEO_STATUS_ERROR;
    }
    #endif
#endif
    return PVRPL_VIDEO_STATUS_OK;
}
#if PVR_VIDEO_XC_PATH
MS_BOOL _PVRPL_Video_XC_Mute(MS_U8 u8PathIdx)
{

#if (DEMO_PVR_V4_TEST == 1)
    MS_U32 u32XCWindow = 0;
    MS_U32 u32DisplayMute = ENABLE;
    if(u8PathIdx <= 1)
    {
        u32XCWindow = u8PathIdx;
    }
    else
    {
        PVRPL_VIDEO_DBGMSG(PVRPL_DBG_TRACE,printf("[%s][%d]Error AVPATH(%d)!\n", __FUNCTION__, __LINE__, u8PathIdx));
        return FALSE;
    }

    if(u8PathIdx == 1)
    {
        msAPI_XC_EnableWindow_EX(&gstXC_DeviceId[u8PathIdx], u32XCWindow, FALSE, NULL);
    }
    msAPI_XC_SetVideoMute_EX(&gstXC_DeviceId[u8PathIdx], u32DisplayMute, u32XCWindow);
    PVRPL_VIDEO_DBGMSG(PVRPL_DBG_TRACE,printf("[%s][%d]Demo_XC_SetVideoMute:%d %"DTC_MS_U32_d" %"DTC_MS_U32_d"\n",__FUNCTION__, __LINE__,u8PathIdx,u32XCWindow,u32DisplayMute));

#endif
    return TRUE;
}
#endif

PVRPL_VIDEO_STATUS PVRPL_Video_Exit(POOL_VDEC_StreamInfo *pStreamVideoInfo)
{
#if PVR_VIDEO_XC_PATH
#if (DEMO_PVR_V4_TEST == 1)
    VDEC_StreamId VDECStreamId;
    MS_U8 u8PathIdx = pStreamVideoInfo->u8PathIdx;
    _PVRPL_Video_Convert2VStreamID(pStreamVideoInfo,&VDECStreamId);

    MApi_DMX_AVFifo_Reset(Pool_GetVFifoType(u8PathIdx), TRUE);
    _PVRPL_Video_XC_Mute(u8PathIdx);

    //@NOTE MApi_VDEC_EX_Rst is not used on normal flow!!!
    //MApi_VDEC_EX_Rst(&VDECStreamId);
    MApi_VDEC_EX_Exit(&VDECStreamId);
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

MS_BOOL _PVRPL_Video_SetNormalFowrardMode(VDEC_StreamId *pStreamId, MS_S16 speed, EN_VIDEO_CPL_DECODE_TYPE enDecodeType)
{
    _PVRPL_Video_SetSpeed(pStreamId, speed);
    MApi_VDEC_EX_AVSyncOn(pStreamId,TRUE, u32SyncDelay, u16SyncTolerance);
    MApi_VDEC_EX_Play(pStreamId);
    _PVRPL_Video_SetDecodeType(pStreamId,enDecodeType);
    return TRUE;
}

MS_BOOL _PVRPL_Video_SetScanMode(VDEC_StreamId *pStreamId, MS_S16 speed, EN_VIDEO_CPL_DECODE_TYPE enDecodeType)
{
    _PVRPL_Video_SetSpeed(pStreamId, speed);
    // @Note Manits 0919691 Need to check whether set decode I
    _PVRPL_Video_SetDecodeType(pStreamId,enDecodeType);
    MApi_VDEC_EX_StepDisp(pStreamId);
    return TRUE;
}

MS_BOOL _PVRPL_Video_SetSmoothScanMode(VDEC_StreamId *pStreamId, MS_S16 speed, EN_VIDEO_CPL_DECODE_TYPE enDecodeType)
{
    _PVRPL_Video_SetSpeed(pStreamId, speed);
    MApi_VDEC_EX_AVSyncOn(pStreamId, FALSE,  u32SyncDelay, u16SyncTolerance);
    // @Note In Smooth Scan setting, need do vdec play cmd befor SetTrickMode
    // Because play-cmd will set the decode ALL on HVC codec.
    MApi_VDEC_EX_Play(pStreamId);
    _PVRPL_Video_SetDecodeType(pStreamId,enDecodeType);
    return TRUE;
}

MS_BOOL _PVRPL_Video_SetMotionPatch(VDEC_StreamId *pStreamId,MS_U8 u8PathIdx,MS_BOOL bEnable)
{

    MSAPI_XC_DEVICE_ID _stXC_DeviceId[2] = {{0, E_MSAPI_XC_DEVICE0}, {0, E_MSAPI_XC_DEVICE1}};
    E_MSAPI_XC_WINDOW eXCWindow;

    if(u8PathIdx == 0)
    {
        eXCWindow = E_MSAPI_XC_MAIN_WINDOW;
    }
    else
    {
        eXCWindow = E_MSAPI_XC_SUB_WINDOW;
    }

    PVRPL_VIDEO_DBGMSG(PVRPL_DBG_TRACE,printf("Force Motion(%d)!!!\n",bEnable));

    msAPI_XC_EnableMADiForceMotion_EX(&_stXC_DeviceId[0], eXCWindow, E_MSAPI_XC_MADI_MOTION_YC, bEnable);

    if (pStreamId == NULL)
    {
        PVRPL_VIDEO_DBGMSG(PVRPL_DBG_ERR,printf("[%s][%d]Error vdec streamid!!\n",__FUNCTION__, __LINE__));
        return FALSE;
    }

    if(MApi_VDEC_EX_SetControl(pStreamId, E_VDEC_EX_USER_CMD_DISP_ONE_FIELD, bEnable) != E_VDEC_EX_OK)
    {
        return FALSE;
    }

    PVRPL_VIDEO_DBGMSG(PVRPL_DBG_TRACE,printf("Force Motion(%d) Done!!!\n",bEnable));

    return TRUE;

}

MS_BOOL _PVRPL_Video_PIDSWITCH_FUNC(VDEC_StreamId *pStreamId,MS_BOOL bEnable, MS_U8 PIDType, MS_U16 u16Pid,MS_U8 *u8PidFlt)
{
    if(PIDType == PVRPL_PCR_PID)
    {
        if(bEnable == TRUE)
        {
            MApi_DMX_STC_UpdateCtrl( 0, DMX_STC_UPDATE_HK );
        }
        else
        {
            MApi_DMX_STC_UpdateCtrl( 0, DMX_STC_UPDATE_FW );
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

    MVOP_InputSel eMvopInputSel = MVOP_INPUT_UNKNOWN;

    VDEC_EX_DispInfo stDispinfo;

    if (u8PathIdx >= PVR_VIDEO_MAX_NUM)
    {
        PVRPL_VIDEO_DBGMSG(PVRPL_DBG_ERR,printf("[%s][%d] Not support vdec multi \n",__FUNCTION__,__LINE__));
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
            PVRPL_VIDEO_DBGMSG(PVRPL_DBG_ERR,printf("Timeout!! Video DispInfo is Not Ready!!\n"));
            PVRPL_VIDEO_DBGMSG(PVRPL_DBG_ERR,printf("DIFF TIME to check Disp Info = %"DTC_MS_U32_u" ms\n", u32EndTime - u32StartTime));
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
        PVRPL_VIDEO_DBGMSG(PVRPL_DBG_ERR,printf("MApi_VDEC_EX_GetDispInfo Failed !! \n"));
        return FALSE;
    }
    else
    {
        printf("MApi_VDEC_EX_GetDispInfo Success !!\n");

    }

    eMvopInputSel = _PVRPL_Video_GetMVOPInputSel(pStreamId);

    _PVRPL_Video_SetMVOP_Config(u8PathIdx,&stDispinfo,eMvopInputSel);

/*
    Pool_Convert2_msAPIVdecDispInfo(&stDispinfo, &stMSAPIDispinfo);

    msAPI_XC_SetVDECInfo_EX_ByPath(*peDevice, stMSAPIDispinfo);

    if(msAPI_XC_SetMVOPConfig_EX((E_MSAPI_XC_DECODER_PATH)*peDevice, eMvopInputSel) != TRUE)
    {
        PVRPL_VIDEO_DBGMSG(PVRPL_DBG_ERR,printf("msAPI_XC_SetMVOPConfig_EX Failed !! \n"));
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
            PVRPL_VIDEO_DBGMSG(PVRPL_DBG_ERR,printf("Timeout!! MApi_VDEC_EX_IsFrameRdy is Not Ready!!\n"));
            return FALSE;
        }
        PVRPL_SleepMS(u32VDEC_DelayTime);
    }
    u32EndTime = PVRPL_GetSysTime();

    printf("MApi_VDEC_EX_IsFrameRdy is Ready!!\n");
    printf("DIFF TIME to check VDEC Frame ready = %"DTC_MS_U32_u" ms\n",u32EndTime - u32StartTime);


    //////////////////////
    //check AV sync done  //
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
               printf("*****************PVRPL Sync Done %"DTC_MS_U32_d" ms\n", u32SyncTimeDiff);
               bSyncFlag = TRUE;
            }
            else if (u32SyncTimeDiff > PVR_AVSYNC_TIMEOUT_INTERVAL) // set timeout
            {
                printf("*****************PVRPL Sync Timeout %"DTC_MS_U32_d" ms\n", u32SyncTimeDiff);
                bSyncFlag = TRUE;
            }
            PVRPL_SleepMS(u32VDEC_DelayTime);
        }
    //}

    u32EndTime = PVRPL_GetSysTime();
    printf("DIFF TIME to wait AV-Sync = %"DTC_MS_U32_u" ms\n", u32EndTime - u32SyncStarTime);


    return TRUE;
}


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
        PVRPL_VIDEO_DBGMSG(PVRPL_DBG_TRACE,printf("MVOP_INPUT_MVD \n"));
    }
    else if((MApi_VDEC_EX_GetActiveCodecType(pStreamId) == E_VDEC_EX_CODEC_TYPE_H264) ||
        (MApi_VDEC_EX_GetActiveCodecType(pStreamId) == E_VDEC_EX_CODEC_TYPE_VP8) ||
        (MApi_VDEC_EX_GetActiveCodecType(pStreamId) == E_VDEC_EX_CODEC_TYPE_AVS))
    {
        eMvopInputSel = MVOP_INPUT_H264;
        PVRPL_VIDEO_DBGMSG(PVRPL_DBG_TRACE,printf("MVOP_INPUT_HVD \n"));
    }
    else if(MApi_VDEC_EX_GetActiveCodecType(pStreamId) == E_VDEC_EX_CODEC_TYPE_HEVC)
    {
        eMvopInputSel = MVOP_INPUT_EVD;
        PVRPL_VIDEO_DBGMSG(PVRPL_DBG_TRACE,printf("MVOP_INPUT_HEVC \n"));
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
#if PVR_VIDEO_XC_PATH//unmute display by tdal
PVRPL_VIDEO_STATUS _PVRPL_Video_VE_PlayVideo(void)
{
    msAPI_VE_SetVideoMute(ENABLE);
    msAPI_VE_SetMode();
    msAPI_VE_SetWin(NULL, NULL);
    msAPI_VE_SetVideoMute(DISABLE);

    return PVRPL_VIDEO_STATUS_OK;

}
#endif
PVRPL_VIDEO_STATUS _PVRPL_Video_ChangWinSize(MSAPI_XC_WINDOW_TYPE stXCDestWin,MS_U32 *pu32XCDevice, MS_U32 *pu32XCWindow, MS_U32 *pu32XCInputSrc)
{
    MS_U32 u32StartTime = 0, u32EndTime = 0;
    E_MSAPI_XC_INPUT_SRC eXCInputSrc = 0;
    return PVRPL_VIDEO_STATUS_OK;
    //Set DTV/DTV2/HDMI Rx path to Scaler
    switch(*pu32XCInputSrc)
    {
    case EN_PVRPL_XC_INPUT_SOURCE_DTV:
        eXCInputSrc = E_MSAPI_XC_INPUT_SOURCE_DTV;
        break;
    case EN_PVRPL_XC_INPUT_SOURCE_DTV2:
        eXCInputSrc = E_MSAPI_XC_INPUT_SOURCE_DTV2;
        break;
    case EN_PVRPL_XC_INPUT_SOURCE_HDMI:
        eXCInputSrc = E_MSAPI_XC_INPUT_SOURCE_HDMI;
        break;
    case EN_PVRPL_XC_INPUT_SOURCE_SCALER0_OP:
        eXCInputSrc = E_MSAPI_XC_INPUT_SOURCE_SCALER_OP;
        break;
    default:
        PVRPL_VIDEO_DBGMSG(PVRPL_DBG_TRACE,printf("Invalid input parameter eDDIXCInputSrc %"DTC_MS_U32_d"\n", *pu32XCInputSrc));
        break;
    }

    msAPI_XC_SetConnect_EX(&(gstXC_DeviceId[*pu32XCDevice]), eXCInputSrc, *pu32XCWindow);

#if (DEMO_XC_DUALXC_TEST == 1)
    xcpl_stPNL_DeviceId.u32Version = gstXC_DeviceId[*pu32XCDevice].u32Version;
    xcpl_stPNL_DeviceId.u32Id = gstXC_DeviceId[*pu32XCDevice].u32Id;
#endif
    //Below setting only for XC input source DTV/DTV2
    if( MSAPI_XC_IsSrcTypeDTV(eXCInputSrc) || MSAPI_XC_IsSrcTypeCapture(eXCInputSrc) )
    {
        u32StartTime = PVRPL_GetSysTime();
    }

    if(*pu32XCWindow == E_MSAPI_XC_MAIN_WINDOW)
    {
        //@NOTE defaul display win size
        if((stXCDestWin.height == 0)&&(stXCDestWin.width == 0 )&&(stXCDestWin.x == 0 )&&(stXCDestWin.y == 0 ))
        {
            msAPI_XC_SetWin_EX(&(gstXC_DeviceId[*pu32XCDevice]), NULL, NULL, NULL, *pu32XCWindow);
        }
        else
        {
            msAPI_XC_SetWin_EX(&(gstXC_DeviceId[*pu32XCDevice]), NULL, NULL, &stXCDestWin, *pu32XCWindow);
        }
    }
    else
    {
        //@NOTE NOT support change sub window now
        PVRPL_VIDEO_DBGMSG(PVRPL_DBG_TRACE,printf("Not Support Change Sub Window Size\n"));
        MSAPI_XC_WINDOW_TYPE stDispWin = {0, 0, (IPANEL(&xcpl_stPNL_DeviceId, Width)/3), (IPANEL(&xcpl_stPNL_DeviceId, Height)/3)};
        msAPI_XC_SetWin_EX(&(gstXC_DeviceId[*pu32XCDevice]), NULL, NULL, &stDispWin, *pu32XCWindow);
    }

    //Below setting only for XC input source DTV/DTV2
    if( MSAPI_XC_IsSrcTypeDTV(eXCInputSrc) || MSAPI_XC_IsSrcTypeCapture(eXCInputSrc) )
    {
        u32EndTime = PVRPL_GetSysTime();
        PVRPL_VIDEO_DBGMSG(PVRPL_DBG_TRACE,printf("[%s][%d] DIFF TIME after MVOP and AV-Sync = %"DTC_MS_U32_d" ms\n", __FUNCTION__, __LINE__, u32EndTime - u32StartTime));
        PVRPL_VIDEO_DBGMSG(PVRPL_DBG_TRACE,printf("########## XC Black Screen DISABLE!!\n"));
    }

    return PVRPL_VIDEO_STATUS_OK;

}

PVRPL_VIDEO_STATUS _PVRPL_Video_SetMVOP_Config(MS_U8 u8PathIdx, VDEC_EX_DispInfo *stDispinfo,MVOP_InputSel eMvopInputSel)
{

    MSAPI_XC_VDEC_DispInfo stMSAPIDispinfo;
    _PVRPL_Convert2_msAPIVdecDispInfo(stDispinfo, &stMSAPIDispinfo);

    msAPI_XC_SetVDECInfo_EX_ByPath(u8PathIdx, stMSAPIDispinfo);

    if(msAPI_XC_SetMVOPConfig_EX((E_MSAPI_XC_DECODER_PATH)u8PathIdx, eMvopInputSel) != TRUE)
    {
        PVRPL_VIDEO_DBGMSG(PVRPL_DBG_TRACE,printf("msAPI_XC_SetMVOPConfig_EX Failed !! \n"));
        return PVRPL_VIDEO_STATUS_ERROR;
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

    MS_U32 u32CurrTime=0;
    u32CurrTime = MsOS_GetSystemTime();

    while(MApi_DMX_Filein_Eng_IsIdle(enPathIndex) != TRUE)
    {
        // Delay 3 ms.
        // Reset AV FIFO with 384K * 3 takes 15-20 ms if bypass file-in timestamp.
        if (MsOS_GetSystemTime() - u32CurrTime > CMDQ_TIMEOUT)
        {
            //printf("PVR_TSP::WaitForEmptyCMDQ Fail! IsFileInIdle() time out!\n");
            PVRPL_VIDEO_DBGMSG(PVRPL_DBG_ERR,printf("[%s][%s][%d] <ERR> MApi_DMX_Filein_Eng_IsIdle() time out! duration:%"DTC_MS_U32_u"\n",  __FILE__, __FUNCTION__,__LINE__,(MsOS_GetSystemTime() - u32CurrTime)));

            printf("FUCHSIA CALL ============> MApi_DMX_Filein_Eng_IsIdle TimeOut!!!\n");
            return PVRPL_VIDEO_STATUS_ERROR;
        }
        MsOS_DelayTask(3);
     }

    PVRPL_VIDEO_DBGMSG(PVRPL_DBG_TRACE,printf("[%s][%s][%d]\n",__FILE__,__FUNCTION__,__LINE__));
    return PVRPL_VIDEO_STATUS_OK;
}

PVRPL_VIDEO_STATUS _PVRPL_Video_FileIn_ResetAVFIFO(MS_U8 u8PathIndex)
{
    if(MApi_DMX_AVFifo_Reset(Pool_GetAFifoType(u8PathIndex), TRUE) == DMX_FILTER_STATUS_ERROR)
    {
        PVRPL_VIDEO_DBGMSG(PVRPL_DBG_ERR,printf("[%s][%s][%d] <ERR>\n",__FILE__,__FUNCTION__,__LINE__));
        return E_PVRPL_FILEIN_FAIL;
    }

    if(MApi_DMX_AVFifo_Reset((DMX_FILTER_TYPE)(Pool_GetVFifoType(u8PathIndex)), TRUE) == DMX_FILTER_STATUS_ERROR)
    {
        PVRPL_VIDEO_DBGMSG(PVRPL_DBG_ERR,printf("[%s][%s][%d] <ERR>\n",__FILE__,__FUNCTION__,__LINE__));
        return E_PVRPL_FILEIN_FAIL;
    }

    _PVRPL_Video_FlushFileInEng(Pool_GetFileInSouce(u8PathIndex));

    if(MApi_DMX_AVFifo_Reset((DMX_FILTER_TYPE)(Pool_GetAFifoType(u8PathIndex)), FALSE) == DMX_FILTER_STATUS_ERROR)
    {
        PVRPL_VIDEO_DBGMSG(PVRPL_DBG_ERR,printf("[%s][%s][%d] <ERR>\n",__FILE__,__FUNCTION__,__LINE__));
        return E_PVRPL_FILEIN_FAIL;
    }

    if(MApi_DMX_AVFifo_Reset((DMX_FILTER_TYPE)(Pool_GetVFifoType(u8PathIndex)), FALSE) == DMX_FILTER_STATUS_ERROR)
    {
        PVRPL_VIDEO_DBGMSG(PVRPL_DBG_ERR,printf("[%s][%s][%d] <ERR>\n",__FILE__,__FUNCTION__,__LINE__));
        return E_PVRPL_FILEIN_FAIL;
    }


    PVRPL_VIDEO_DBGMSG(PVRPL_DBG_TRACE,printf("[%s][%s][%d]\n",__FILE__,__FUNCTION__,__LINE__));
    return E_PVRPL_FILEIN_SUCESS;
}

static MS_BOOL _PVRPL_Video_GetMinPktNumFlushPatternGen(VDEC_StreamId *pStreamId, MS_U16 *u16PktNum)
{

    VDEC_EX_CodecType eCodecTypeCur;
    MS_BOOL  bRet = TRUE;

    eCodecTypeCur = MApi_VDEC_EX_GetActiveCodecType(pStreamId);

    switch(eCodecTypeCur)
    {
        case E_VDEC_EX_CODEC_TYPE_H264:
        case E_VDEC_EX_CODEC_TYPE_AVS:
        case E_VDEC_EX_CODEC_TYPE_HEVC:
            *u16PktNum = 3;// include one PES header
            break;
        case E_VDEC_EX_CODEC_TYPE_MPEG2:
            *u16PktNum = 2;// include one PES header
           break;
        default:
            bRet = FALSE;
            break;
    }

    return bRet;
}
PVRPL_VIDEO_STATUS _PVRPL_Video_EnableFileInPause(DMX_FILEIN_PATH enPathIndex,MS_BOOL bEnable)
{
    if(bEnable == TRUE)
    {
        if(MApi_DMX_Filein_Eng_Pause(enPathIndex) == DMX_FILTER_STATUS_ERROR)
        {
            PVRPL_VIDEO_DBGMSG(PVRPL_DBG_ERR,printf("[%s][%s][%d] <ERR>\n",__FILE__,__FUNCTION__,__LINE__));
            return PVRPL_VIDEO_STATUS_ERROR;
        }
    }
    else
    {
        if(MApi_DMX_Filein_Eng_Resume(enPathIndex) == DMX_FILTER_STATUS_ERROR)
        {
            PVRPL_VIDEO_DBGMSG(PVRPL_DBG_ERR,printf("[%s][%s][%d] <ERR>\n",__FILE__,__FUNCTION__,__LINE__));
            return PVRPL_VIDEO_STATUS_ERROR;
        }
    }

    return PVRPL_VIDEO_STATUS_OK;
}
PVRPL_VIDEO_STATUS PVRPL_Video_FlushData(POOL_VDEC_StreamInfo *pStreamVideoInfo, MS_U16 u16VideoPID, MS_U16 u16PacketLen, MS_U32 u32VirAdr)
{
    // Var for video
    MS_U32 u32BufSize = 2048;
    MS_U8 u8Idx = 0;
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
    VDEC_StreamId VDECStreamId;
    POOL_VDEC_CURR_INFO *stVDECCurrInfo;
    // Var for filein
    DMX_FILEIN_PATH enFilePathIdx = Pool_GetFileInSouce(pStreamVideoInfo->u8PathIdx);
    MS_BOOL bLastFilePauseState = MApi_DMX_Filein_Eng_IsPause(enFilePathIdx);
    // Var for video
    _PVRPL_Video_Convert2VStreamID(pStreamVideoInfo,&VDECStreamId);
    stVDECCurrInfo = Pool_GetVideoCurrCmdSetting(pStreamVideoInfo->u8PathIdx);

    if ((u16VideoPID == 0x1FFF) || (u16VideoPID == 0))
    {
        return PVRPL_VIDEO_STATUS_OK;
    }
    memset(pu8Addr, 0xFF, u32BufSize);

    //1. VDEC pause, file in bypass and unpause
    MApi_VDEC_EX_Pause(&VDECStreamId);
    _PVRPL_Video_EnableFileInPause(enFilePathIdx, FALSE);


    //2. Gen video flush pattern

    // calculate the least time
    u32LeastLength = (MS_U32)MApi_VDEC_EX_GetPatternLeastLength(&VDECStreamId, stVdecType);
    if (u32LeastLength == (0xFFFFFFFFUL))   // U32_MAX
    {
        u32LeastLength = u16MostTimes * u32TsRawSize;
        u8SendSkipTimer = 0xFF;
    }
    else
    {
        u8SendSkipTimer = 5;
    }

    u16LeastTime = (u32LeastLength / u32TsRawSize) + ((u32LeastLength % u32TsRawSize > 0) ? 1 : 0);

    if (_PVRPL_Video_GetMinPktNumFlushPatternGen(&VDECStreamId, &u16LeastTimeByCodec)!= TRUE)
    {
        PVRPL_VIDEO_DBGMSG(PVRPL_DBG_ERR,printf("Error to check minimum video flush pattern packet number.\n"));
        PVRPL_VIDEO_DBGMSG(PVRPL_DBG_ERR,printf("Abort Video Flush!!!\n"));
        return FALSE;
    }

    if (u16LeastTime < u16LeastTimeByCodec)//send one video flush command at least
        u16LeastTime = u16LeastTimeByCodec;

    u16LeastTime++; // include 1 null pkt

    if (u16LeastTime > u16MostTimes)
    {
        PVRPL_VIDEO_DBGMSG(PVRPL_DBG_ERR,printf(" [TS] Warrings Make Skip PKT Warring. Buffer is not enough.\n"));
        u16LeastTime = u16MostTimes;
    }


    for (u8Idx = 0; u8Idx < u16MostTimes ; u8Idx++)
    {
        MS_U32 u32Remainder = u32TsRawSize;
        MS_U32 u32FileInTime = 0;

        if(MApi_DMX_Filein_Eng_GetFileInTimeStamp(enFilePathIdx, &u32FileInTime) == DMX_FILTER_STATUS_ERROR)
        {
            PVRPL_VIDEO_DBGMSG(PVRPL_DBG_ERR,printf("[%s][%s][%d] <ERR>\n",__FILE__,__FUNCTION__,__LINE__));
            return PVRPL_VIDEO_STATUS_ERROR;
        }

        // filein packet timestamp can't set to zero
        pu8Addr[0] = ((u32FileInTime & 0xff000000) >> 24);
        pu8Addr[1] = ((u32FileInTime & 0x00ff0000) >> 16);
        pu8Addr[2] = ((u32FileInTime & 0x0000ff00) >> 8);
        pu8Addr[3] = (u32FileInTime & 0x000000ff);

        if(u8Idx == 0)
        {
            // Add a null pkt
            // Reason: If data is not 13818 format, 1st pkt will be dropped for synchronizing on file-in case. (TSP HW behavior)
            pu8Addr[4] = 0x47;
            pu8Addr[5] = 0x1F;
            pu8Addr[6] = 0xFF;
            pu8Addr[7] = ((u8Idx & 0xf) | 0x10);
        }
        else if(u8Idx == 1)
        {
            // PES header for MVD parser
            pu8Addr[4] = 0x47;
            pu8Addr[5] = ((MS_U8)(u16VideoPID >> 8) | 0x40); //Vdec flow change (PUSI=1 if there is 1st PTS header in the pkt)
            pu8Addr[6] = (MS_U8)(u16VideoPID);
            pu8Addr[7] = ((u8Idx & 0xf) | 0x10);
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
        else if(u8Idx < u16LeastTime)
        {
            // Flush pattern
            pu8Addr[4] = 0x47;
            pu8Addr[5] = (MS_U8)(u16VideoPID >> 8);
            pu8Addr[6] = (MS_U8)(u16VideoPID);
            pu8Addr[7] = ((u8Idx & 0xf) | 0x10);

            MS_PHY PHYPhyAdr = (MS_PHY)MsOS_VA2PA((MS_U32)(pu8Addr + 8));
            MApi_VDEC_EX_GenPattern(&VDECStreamId, stVdecType, PHYPhyAdr, &u32Remainder);
        }
        else
        {
            // Dummy data
            pu8Addr[4] = 0x47;
            pu8Addr[5] = (MS_U8)(u16VideoPID >> 8);
            pu8Addr[6] = (MS_U8)(u16VideoPID);
            pu8Addr[7] = ((u8Idx & 0xf) | 0x10);
        }
        pu8Addr += u16PacketLen;
    }

    MApi_VDEC_EX_EnableLastFrameShow(&VDECStreamId, FALSE);
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
    while (MApi_VDEC_EX_Flush(&VDECStreamId, E_VDEC_EX_FREEZE_AT_CUR_PIC) != E_VDEC_EX_OK)
    {
        PVRPL_VIDEO_DBGMSG(PVRPL_DBG_ERR,printf("MApi_VDEC_EX_Flush Failed !! \n"));
        bRet = PVRPL_VIDEO_STATUS_OK;
        u32CurrTime1 = PVRPL_GetSysTime();
        while (MApi_DMX_Filein_Eng_IsIdle(pStreamVideoInfo->u8PathIdx) == FALSE)
        {
            if (PVRPL_GetSysTime() - u32CurrTime1 > 300)
            {
                PVRPL_VIDEO_DBGMSG(PVRPL_DBG_ERR,printf("1)Wait MS_DMX_Filein_IsIdle() time out!\n"));
                bRet = PVRPL_VIDEO_STATUS_ERROR;
                break;
            }
        }

        // add file in bloack
        //Push File-in block into TSP
        if(PVRPL_VIDEO_STATUS_OK == bRet)
        {
            u32CurrTime1 = PVRPL_GetSysTime();
            while (_PVRPL_Video_FileInData(pStreamVideoInfo->u8PathIdx,PHYPhyAddr,u32SkipPKTsSize) == PVRPL_VIDEO_STATUS_ERROR)
            {
                if (PVRPL_GetSysTime() - u32CurrTime1 > 300)
                {
                    PVRPL_VIDEO_DBGMSG(PVRPL_DBG_ERR,printf("2)Wait MS_DMX_Filein_Start() time out!\n"));
                    break;
                }
            }
        }
    }

    //4. Check whether finishing to flush frame buffer or not
    u32CurrTime1 = PVRPL_GetSysTime();
    while ((MApi_VDEC_EX_IsAllBufferEmpty(&VDECStreamId) != E_VDEC_EX_OK))
    {
        bRet = PVRPL_VIDEO_STATUS_OK;
        //to do
        //add TSP related function
        if (PVRPL_GetSysTime() - u32CurrTime1 > 1200)
        {
            PVRPL_VIDEO_DBGMSG(PVRPL_DBG_ERR,printf("3)Wait MApi_VDEC_IsAllBufferEmpty() time out!\n"));
            _PVRPL_Video_FileIn_ResetAVFIFO(pStreamVideoInfo->u8PathIdx);
            break;
        }

        if (u8SendSkipTimer)
        {
            if (u8SendSkipTimer <= 5)
                u8SendSkipTimer--;

            u32CurrTime2 = PVRPL_GetSysTime();
            while (MApi_DMX_Filein_Eng_IsIdle(pStreamVideoInfo->u8PathIdx) == FALSE)
            {
                if (PVRPL_GetSysTime() - u32CurrTime2 > 400)
                {
                    bRet = PVRPL_VIDEO_STATUS_ERROR;
                    PVRPL_VIDEO_DBGMSG(PVRPL_DBG_ERR,printf("4)Wait MS_DMX_Filein_IsIdle() time out!\n"));
                    break;
                }
            }

            u32CurrTime2 = PVRPL_GetSysTime();
            if(PVRPL_VIDEO_STATUS_OK == bRet)
            {
                while (_PVRPL_Video_FileInData(pStreamVideoInfo->u8PathIdx,PHYPhyAddr,u32SkipPKTsSize) == PVRPL_VIDEO_STATUS_ERROR)
                {
                    if (PVRPL_GetSysTime() - u32CurrTime2 > 300)
                    {
                        PVRPL_VIDEO_DBGMSG(PVRPL_DBG_ERR,printf("5)Wait MS_DMX_Filein_Start() time out!\n"));
                        break;
                    }
                }

                PVRPL_SleepMS(5);//prevent sending data too fast.
            }
        }

    }

    //5. Resume the video cmd and file in state
    _PVRPL_Video_SetDecodeMode(&VDECStreamId, pStreamVideoInfo->u8PathIdx, stVDECCurrInfo->s16Speed, stVDECCurrInfo->enDecodeType, stVDECCurrInfo->enDataType);
    _PVRPL_Video_EnableFileInPause(enFilePathIdx, bLastFilePauseState);
    PVRPL_VIDEO_DBGMSG(PVRPL_DBG_TRACE,printf("Flush Filein pause status: %x \n",bLastFilePauseState));

    return PVRPL_VIDEO_STATUS_OK;
}

PVRPL_VIDEO_STATUS PVRPL_Video_FileEnd(POOL_VDEC_StreamInfo *pStreamVideoInfo, MS_U16 u16VideoPID, MS_U16 u16PacketLen, MS_U32 u32VirAdr)
{
    MS_U32 u32BufSize = 2048;
    MS_U8 u8Idx = 0;
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

    VDEC_StreamId VDECStreamId;
    _PVRPL_Video_Convert2VStreamID(pStreamVideoInfo,&VDECStreamId);

    if ((u16VideoPID == 0x1FFF) || (u16VideoPID == 0))
    {
        return PVRPL_VIDEO_STATUS_OK;
    }

    memset(pu8Addr, 0xFF, u32BufSize);

    u32LeastLength = (MS_U32)MApi_VDEC_EX_GetPatternLeastLength(&VDECStreamId, stVdecType);
    if (u32LeastLength == (0xFFFFFFFFUL))   // U32_MAX
    {
        u32LeastLength = u16MostTimes * u32TsRawSize;
    }

    u16LeastTime = (u32LeastLength / u32TsRawSize) + ((u32LeastLength % u32TsRawSize > 0) ? 1 : 0);
    if (u16LeastTime <= 1)//send one video flush command at least
        u16LeastTime = 2;
    u16LeastTime++; // include null pkt

    if (u16LeastTime > u16MostTimes)
    {
        PVRPL_VIDEO_DBGMSG(PVRPL_DBG_ERR,printf(" [TS] Warrings Make Skip PKT Warring. Buffer is not enough.\n"));
        u16LeastTime = u16MostTimes;
    }

    for (u8Idx = 0; u8Idx < u16MostTimes ; u8Idx++)
    {
        MS_U32 u32Remainder = u32TsRawSize;

        DMX_FILEIN_PATH enFilePathIdx = Pool_GetFileInSouce(pStreamVideoInfo->u8PathIdx);
        MS_U32 u32FileInTime = 0;
        if(MApi_DMX_Filein_Eng_GetFileInTimeStamp(enFilePathIdx, &u32FileInTime) == DMX_FILTER_STATUS_ERROR)
        {
            PVRPL_VIDEO_DBGMSG(PVRPL_DBG_ERR,printf("[%s][%s][%d] <ERR>\n",__FILE__,__FUNCTION__,__LINE__));
            return PVRPL_VIDEO_STATUS_ERROR;
        }

        // filein packet timestamp can't set to zero
        pu8Addr[0] = ((u32FileInTime & 0xff000000) >> 24);
        pu8Addr[1] = ((u32FileInTime & 0x00ff0000) >> 16);
        pu8Addr[2] = ((u32FileInTime & 0x0000ff00) >> 8);
        pu8Addr[3] = (u32FileInTime & 0x000000ff);

        if(u8Idx == 0)
        {
            // Add 1 null pkt
            // Reason: If data is not 13818 format, 1st pkt will be dropped for synchronizing on file-in case. (TSP HW behavior)
            pu8Addr[4] = 0x47;
            pu8Addr[5] = 0x1F;
            pu8Addr[6] = 0xFF;
            pu8Addr[7] = ((u8Idx & 0xf) | 0x10);
        }
        else if(u8Idx == 1)
        {
            // PES header
            pu8Addr[4] = 0x47;
            pu8Addr[5] = ((MS_U8)(u16VideoPID >> 8) | 0x40); //Vdec flow change (PUSI=1 if there is 1st PTS header in the pkt)
            pu8Addr[6] = (MS_U8)(u16VideoPID);
            pu8Addr[7] = ((u8Idx & 0xf) | 0x10);
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
        else if (u8Idx < u16LeastTime)
        {
            //
            pu8Addr[4] = 0x47;
            pu8Addr[5] = (MS_U8)(u16VideoPID >> 8);
            pu8Addr[6] = (MS_U8)(u16VideoPID);
            pu8Addr[7] = ((u8Idx & 0xf) | 0x10);

            MS_PHY PHYPhyAdr = MsOS_VA2PA((MS_U32)(pu8Addr + 8));
            MApi_VDEC_EX_GenPattern(&VDECStreamId,stVdecType, PHYPhyAdr, &u32Remainder);
        }
        else
        {
            // Dummy data
            pu8Addr[4] = 0x47;
            pu8Addr[5] = (MS_U8)(u16VideoPID >> 8);
            pu8Addr[6] = (MS_U8)(u16VideoPID);
            pu8Addr[7] = ((u8Idx & 0xf) | 0x10);
        }

        pu8Addr += u16PacketLen;
    }

    MApi_VDEC_EX_EnableLastFrameShow(&VDECStreamId, FALSE);

    bRet = PVRPL_VIDEO_STATUS_OK;

    u32CurrTime1 = PVRPL_GetSysTime();
    //to do
    //add TSP related function
    while (MApi_DMX_Filein_Eng_IsIdle(pStreamVideoInfo->u8PathIdx) == FALSE)
    {
        if (PVRPL_GetSysTime() - u32CurrTime1 > 300)
        {
            PVRPL_VIDEO_DBGMSG(PVRPL_DBG_ERR,printf("1)Wait MS_DMX_Filein_IsIdle() time out!\n"));
            bRet = PVRPL_VIDEO_STATUS_ERROR;
            break;
        }
    }

    if(PVRPL_VIDEO_STATUS_OK == bRet)
    {
        u32CurrTime1 = PVRPL_GetSysTime();
        while (_PVRPL_Video_FileInData(pStreamVideoInfo->u8PathIdx,PHYPhyAddr,u32SkipPKTsSize) == PVRPL_VIDEO_STATUS_ERROR)
        {
            if (PVRPL_GetSysTime() - u32CurrTime1 > 300)
            {
                PVRPL_VIDEO_DBGMSG(PVRPL_DBG_ERR,printf("2)Wait MS_DMX_Filein_Start() time out!\n"));
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
                PVRPL_FILEIN_DBGMSG(PVRPL_DBG_ERR,printf("2)Wait MS_DMX_Filein_Start() time out!\n"));
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
    VDEC_StreamId VDECStreamId;
    _PVRPL_Video_Convert2VStreamID(pStreamVideoInfo,&VDECStreamId);

    //set display win
    MSAPI_XC_WINDOW_TYPE HD_DestWin;


    //This code flow is for playbackopen -> changewin -> playbackstart case
    if( TRUE != MApi_VDEC_EX_CheckDispInfoRdy(&VDECStreamId))
    {
        stDispWinInfo[pStreamVideoInfo->u8PathIdx].u32Height = stDisplayWinInfo.u32Height;
        stDispWinInfo[pStreamVideoInfo->u8PathIdx].u32Width  = stDisplayWinInfo.u32Width;
        stDispWinInfo[pStreamVideoInfo->u8PathIdx].u32X_Pos  = stDisplayWinInfo.u32X_Pos;
        stDispWinInfo[pStreamVideoInfo->u8PathIdx].u32Y_Pos  = stDisplayWinInfo.u32Y_Pos;
        stDispWinInfo[pStreamVideoInfo->u8PathIdx].bDef      = stDisplayWinInfo.bDef;
        bChangeDone[pStreamVideoInfo->u8PathIdx] = FALSE;
        PVRPL_VIDEO_DBGMSG(PVRPL_DBG_ERR,printf("Vdec Disp Info is not ready,so change window is called by PVRPL_Video_PlayXC !!!\n"));
        return PVRPL_VIDEO_STATUS_OK;
    }
    else
    {
        bChangeDone[pStreamVideoInfo->u8PathIdx] = TRUE;
    }

    HD_DestWin.height   =   stDisplayWinInfo.u32Height;
    HD_DestWin.width    =   stDisplayWinInfo.u32Width;
    HD_DestWin.x        =   stDisplayWinInfo.u32X_Pos;
    HD_DestWin.y        =   stDisplayWinInfo.u32Y_Pos;

    MS_U32 u32XC_Device = 0;
    MS_U32 u32XC_Window = 0;
    MS_U32 u32XC_Inputsrc = 0;
    if (pStreamVideoInfo->u8PathIdx == 0)
    {
        u32XC_Device = 0; //HD PATH
        u32XC_Window = 0; // Main Window
        u32XC_Inputsrc = EN_PVRPL_XC_INPUT_SOURCE_DTV;
        _PVRPL_Video_ChangWinSize(HD_DestWin,&u32XC_Device,&u32XC_Window,&u32XC_Inputsrc);

        //@NOTE Drvier behavior
        //There is the other xc1 path to CVBS , so need to set up xc1.
        #if (DEMO_XC_DUALXC_TEST == 1)
        u32XC_Device = 1; //SD PATH
        u32XC_Window = 0; // Main Window

        #ifndef temp_Kano
        u32XC_Inputsrc = EN_PVRPL_XC_INPUT_SOURCE_DTV;
        #else
        u32XC_Inputsrc = EN_PVRPL_XC_INPUT_SOURCE_SCALER0_OP;
        #endif

        PVRPL_VIDEO_DBGMSG(PVRPL_DBG_TRACE,printf("PVR Main for SD path(Main window) => Demo_XC_PlayVideo from source DTV\n"));
        PVRPL_VIDEO_DBGMSG(PVRPL_DBG_TRACE,printf("[%s]Trace XC Function: msAPI_XC_SetWin_EX\n",__FUNCTION__));
        _PVRPL_Video_ChangWinSize(HD_DestWin,&u32XC_Device,&u32XC_Window,&u32XC_Inputsrc);
        #endif
    }
    else
    {
        u32XC_Device = 0; //HD PATH
        u32XC_Window = 1; // Sub Window
        u32XC_Inputsrc = EN_PVRPL_XC_INPUT_SOURCE_DTV2;
        _PVRPL_Video_ChangWinSize(HD_DestWin,&u32XC_Device,&u32XC_Window,&u32XC_Inputsrc);
    }

    return PVRPL_VIDEO_STATUS_OK;
}


