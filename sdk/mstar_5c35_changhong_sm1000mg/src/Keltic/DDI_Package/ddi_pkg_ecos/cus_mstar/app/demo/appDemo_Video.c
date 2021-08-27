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
// Copyright (c) 2006-2007 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (!¡±MStar Confidential Information!¡L) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////



//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------
//Common
#include "MsCommon.h"
#include "MsTypes2.h"
#include "MsMemory.h"
//#include "ver_dtv_comm.h"
#include <string.h>
#include <time.h>
#include "MsOS.h"

//Api
#include "apiVDEC.h"
#include "apiVDEC_EX.h"
#include "asmCPU.h"
#include "drvXC_IOPort.h"
#include "apiXC.h"
#include "apiDAC.h"
#include "apiHDMITx.h"
#include "apiXC_Ace.h"

//Drv
#include "drvMVOP.h"
#include "drvTVEncoder.h"

//MApi
#include "msAPI_XC.h"
#include "msAPI_VE.h"
#include "appDemo_Video.h"
#include "appDemo_XC.h"
#include "appDemo_AV.h"

#ifdef ENABLE_MM_PACKAGE
#include "msAPI_MM.h"
extern MS_BOOL b_MM_Initied;
#endif
//------------------------------------------------------------------------------
// Macros
//------------------------------------------------------------------------------
#define VIDEO_FRAMECOUNT_LOOP           300
#define VIDEO_FRAMECOUNT_THRESHHOLD     3
#define AVSYNC_TIMEOUT_INTERVAL         5000
#define DAC_DETECTTASK_STACK_SIZE     0x2000

//------------------------------------------------------------------------------
// Debug Macros
//------------------------------------------------------------------------------
#if ENABLE_DEBUG
#define db_print(fmt, args...)  printf("[%s][%d]" fmt, __FUNCTION__, __LINE__, ## args)
#else
#define db_print(fmt, args...)  while(0);
#endif


//-------------------------------------------------------------------------------------------------
// Local Variables
//-------------------------------------------------------------------------------------------------
static MS_BOOL _bVideo_WaitAVSync = TRUE;

static MS_BOOL _bVideo_DACSD_IN = TRUE;
static void*   _pDetectTaskStack = NULL;
static MS_S32  s32DetectTaskID = -1;
extern MS_S32 gs32CachedPoolID;

static VDEC_StreamId gtVDECStreamId = {0x0, 0x1111};

static MS_U32 u32GetDecStatusDelay = 10;
//-------------------------------------------------------------------------------------------------
// Local Functions
//-------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
/// @brief get video information from MPEG2 video header
/// @param[in] *pstVidStatus \b OUT information about progressive/interlace, resolution, framerate, aspect ratio
/// @return
///   - # TRUE video header has no problem
///   - # FALSE otherwise
//----------------------------------------------------------------------------------
static MS_BOOL _appDemo_Video_GetVidInfo(VDEC_EX_DispInfo *pstVidStatus)
{
    VDEC_EX_DispInfo   info;
    VDEC_EX_Result     ret;


#ifdef ENABLE_MM_PACKAGE
    if((b_MM_Initied == TRUE) )//&& (msAPI_MM_GetFileType() != MM_ES_MODE)
    {
        video_disp_info MM_DispInfo;
        memset(&MM_DispInfo, 0x00, sizeof(video_disp_info));

        //if(msAPI_MM_GetDispInfo(&MM_DispInfo) == TRUE)
        if(MApi_MPlayer_Get_DispInfo(E_MPLAYER_MEDIA_MOVIE, (video_disp_info*)&MM_DispInfo) == TRUE)
        {
            pstVidStatus->u16CropLeft = MM_DispInfo.dCropLeft;
            pstVidStatus->u16CropRight = MM_DispInfo.dCropRight;
            pstVidStatus->u16CropTop = MM_DispInfo.dCropTop;
            pstVidStatus->u16CropBottom = MM_DispInfo.dCropBottom;
            pstVidStatus->u16SarWidth   = 0;
            pstVidStatus->u16SarHeight = 0;
            pstVidStatus->u16HorSize    = MM_DispInfo.dWidth;
            pstVidStatus->u16VerSize    = MM_DispInfo.dHeight;
            pstVidStatus->u32FrameRate  = MM_DispInfo.dFrameRate;
            pstVidStatus->u8Interlace   = MM_DispInfo.dInterlace;
        }
        else
            return FALSE;
    }
    else
#endif
    {
    #if 0
    if(MApi_VDEC_GetActiveCodecType() == E_VDEC_CODEC_TYPE_MPEG2)
    {
        if (E_VDEC_FAIL == MApi_VDEC_IsWithValidStream()) /*--- check valid mpeg ---*/
        {
            db_print("Stream is not Valid!! \n");
            return FALSE;
        }
    }
    #endif

    memset(&info, 0, sizeof(VDEC_EX_DispInfo));
    ret = MApi_VDEC_EX_GetDispInfo(&gtVDECStreamId,&info);

    if (E_VDEC_EX_OK != ret)
    {
        return FALSE;
    }
    else
    {
        pstVidStatus->u16HorSize    = info.u16HorSize;
        pstVidStatus->u16VerSize    = info.u16VerSize;
        pstVidStatus->u32FrameRate  = info.u32FrameRate;
        pstVidStatus->u8AspectRate  = info.u8AspectRate;
        pstVidStatus->u8Interlace   = info.u8Interlace;
        pstVidStatus->u16SarWidth   = info.u16SarWidth;
        pstVidStatus->u16SarHeight  = info.u16SarHeight;
        pstVidStatus->u16CropRight  = info.u16CropRight;
        pstVidStatus->u16CropLeft   = info.u16CropLeft;
        pstVidStatus->u16CropBottom = info.u16CropBottom;
        pstVidStatus->u16CropTop    = info.u16CropTop;
        pstVidStatus->u32AspectWidth = info.u32AspectWidth;
        pstVidStatus->u32AspectHeight    = info.u32AspectHeight;

        db_print("HorSize = %u \n", pstVidStatus->u16HorSize);
        db_print("VerSize = %u \n", pstVidStatus->u16VerSize);
        db_print("FrameRate = %lu \n", pstVidStatus->u32FrameRate);
        db_print("AspectRate = %u \n", pstVidStatus->u8AspectRate);
        db_print("Interlace = %u \n", pstVidStatus->u8Interlace);
        db_print("SarWidth = %u \n", pstVidStatus->u16SarWidth);
        db_print("SarHeight = %u \n", pstVidStatus->u16SarHeight);
        db_print("AspectWidth = %lu \n", pstVidStatus->u32AspectWidth);
        db_print("AspectHeight = %lu \n", pstVidStatus->u32AspectHeight);

        if(MApi_VDEC_EX_GetActiveCodecType(&gtVDECStreamId) == E_VDEC_EX_CODEC_TYPE_MPEG2)
        {
            pstVidStatus->u8AFD         = info.u8AFD;
            pstVidStatus->u16Pitch      = info.u16Pitch;
            pstVidStatus->u16PTSInterval = info.u16PTSInterval;
            pstVidStatus->u8MPEG1       = info.u8MPEG1;
            pstVidStatus->u8PlayMode    = info.u8PlayMode;
            pstVidStatus->u8FrcMode     = info.u8FrcMode;

            if (pstVidStatus->u32FrameRate == 0||
                pstVidStatus->u16HorSize == 0 ||
                pstVidStatus->u16HorSize > 2000 ||
                pstVidStatus->u16VerSize == 0 ||
                pstVidStatus->u16VerSize > 1100)
            {
                db_print("Video Info is not Valid!! \n");
                return FALSE;
            }
            pstVidStatus->u8AFD = MApi_VDEC_EX_GetActiveFormat(&gtVDECStreamId);
            return TRUE;
        }
    }
    }
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief function to get time
//------------------------------------------------------------------------------
static MS_U32 _appDemo_GetTime(void)
{
    #if defined (MSOS_TYPE_UCOS) || defined (MSOS_TYPE_ECOS)
        return MsOS_GetSystemTime();
    #else
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        return ts.tv_sec* 1000+ ts.tv_nsec/1000000;
    #endif
}

//------------------------------------------------------------------------------
/// @brief function to check AV sync is reach
//------------------------------------------------------------------------------
static MS_BOOL _appDemo_IsSyncDone(void)
{
    VDEC_EX_Result enRst = E_VDEC_EX_FAIL;

    if(!MApi_VDEC_EX_IsAVSyncOn(&gtVDECStreamId))
    {
        db_print("MApi_VDEC_IsAVSyncOn Not ON\n");
        return TRUE;
    }

    enRst = MApi_VDEC_EX_IsReachSync(&gtVDECStreamId);
    if(enRst == E_VDEC_EX_OK)
    {
        db_print("MApi_VDEC_EX_IsReachSync OK!\n");
        return TRUE;
    }
    else
    {
        db_print("MApi_VDEC_EX_IsReachSync Not OK, return %u\n", enRst);
        //MApi_VDEC_SetDbgLevel(E_VDEC_DBG_LEVEL_DBG);
        //MApi_VDEC_DbgDumpStatus();
        return FALSE;
    }
}

static MVOP_InputSel _appDemo_GetMVOPInputSel(void)
{
    MVOP_InputSel tMvopInputSel = MVOP_INPUT_UNKNOWN;
    if(g_Demo_IsIframedecoding == TRUE)
    {
        tMvopInputSel = MVOP_INPUT_DRAM;
    }
    else if ((MApi_VDEC_EX_GetActiveCodecType(&gtVDECStreamId) == E_VDEC_EX_CODEC_TYPE_MPEG2) ||
        (MApi_VDEC_EX_GetActiveCodecType(&gtVDECStreamId) == E_VDEC_EX_CODEC_TYPE_VC1_ADV) ||
        (MApi_VDEC_EX_GetActiveCodecType(&gtVDECStreamId) == E_VDEC_EX_CODEC_TYPE_MPEG4))
    {
        tMvopInputSel = MVOP_INPUT_MVD;
        db_print("MVOP_INPUT_MVD \n");
    }
    else if(MApi_VDEC_EX_GetActiveCodecType(&gtVDECStreamId) == E_VDEC_EX_CODEC_TYPE_H264)
    {
        tMvopInputSel = MVOP_INPUT_H264;
        db_print("E_VDEC_EX_CODEC_TYPE_H264 \n");
    }
    else if(MApi_VDEC_EX_GetActiveCodecType(&gtVDECStreamId) == E_VDEC_EX_CODEC_TYPE_AVS)
    {
        tMvopInputSel = MVOP_INPUT_H264;
        db_print("E_VDEC_EX_CODEC_TYPE_AVS \n");
    }

    return tMvopInputSel;
}

//------------------------------------------------------------------------------
/// @brief function to get Diff time
//------------------------------------------------------------------------------
static MS_U32 _appDemo_TimeDiffFromNow(MS_U32 u32Time)
{
    MS_U32 u32Now;

    #if defined (MSOS_TYPE_UCOS) || defined (MSOS_TYPE_ECOS)
        u32Now = MsOS_GetSystemTime();
    #else
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        u32Now = ts.tv_sec * 1000+ ts.tv_nsec/1000000;
    #endif

    if(u32Now >= u32Time)
    {
        return u32Now - u32Time;
    }
    else
    {
        return 0xFFFFFFFF - u32Time + u32Now;
    }
}

//------------------------------------------------------------------------------
/// @brief function to tune on/off DAC voltage
//------------------------------------------------------------------------------
static void _SetDACFullVolt(MS_BOOL bFullOut, MS_BOOL bHDDAC)
{
    MApi_DAC_SetIHalfOutput(!bFullOut, bHDDAC);
    MApi_DAC_SetQuartOutput(!bFullOut, bHDDAC);
}

//------------------------------------------------------------------------------
/// @brief function to detectDAC state
//------------------------------------------------------------------------------
void _appDemo_Video_DACDetect(void)
{
    MS_BOOL bIsPlugIn;

    while(1)
    {
        if(_bVideo_DACSD_IN)  //Detect Plug Out
        {
            _SetDACFullVolt(TRUE, TRUE);
            _SetDACFullVolt(TRUE, FALSE);
            MApi_DAC_SetOutputLevel(E_DAC_OUTPUT_LEVEL_LOW, TRUE); //detect need 1.0V voltage
            MApi_DAC_HotPlugDetect(E_DAC_DETECT_SD, E_DAC_DETECT_PLUGOUT, &bIsPlugIn);

            if(!bIsPlugIn)
            {
                _SetDACFullVolt(FALSE, TRUE);
                _SetDACFullVolt(FALSE, FALSE);
            }
        }
        else //Detect Plug In
        {
            _SetDACFullVolt(FALSE, TRUE);
            _SetDACFullVolt(FALSE, FALSE);
            MApi_DAC_SetOutputLevel(E_DAC_OUTPUT_LEVEL_HIGH, TRUE); //detect need 1.0V voltage
            MApi_DAC_HotPlugDetect(E_DAC_DETECT_SD, E_DAC_DETECT_PLUGIN, &bIsPlugIn);

            if(bIsPlugIn)
            {
                _SetDACFullVolt(TRUE, TRUE);
                _SetDACFullVolt(TRUE, FALSE);
            }
        }

        _bVideo_DACSD_IN = bIsPlugIn;
        db_print("_bVideo_DACSD_IN %d!\n",_bVideo_DACSD_IN);
        MsOS_DelayTask(10);
    }
}

//-------------------------------------------------------------------------------------------------
// Demo Functions
//-------------------------------------------------------------------------------------------------

#ifdef ENABLE_MM_PACKAGE
#include "msAPI_MM.h"

//------------------------------------------------------------------------------
/// @brief set MVOP output timing according to MVD/h.264 video frame information
/// @param[in] *pstVidStatus \b input video information
//------------------------------------------------------------------------------
MS_BOOL _appDemo_MM_Video_SetOutput(VDEC_EX_DispInfo *pstVidStatus)
{
    MVOP_VidStat stMvopVidSt;
    MVOP_InputSel tMvopInputSel = MVOP_INPUT_UNKNOWN;
    video_disp_info MM_DispInfo;
    MS_U32 u32CodecType;
    VDEC_EX_DispInfo stVidStatus;

    //MS504x set MVOP as MIU1 Group1
    MDrv_MVOP_MiuSwitch(1);

    MDrv_MVOP_Init();
    MDrv_MVOP_Enable(FALSE); //wait setting done
    u32CodecType = MApi_MPlayer_GetOption(E_MPLAYER_MEDIA_MOVIE, MPLAYER_OPTION_MOVIE_INFO, (unsigned int)MPLAYER_INFO_VIDEO_CODEC);
    //u32CodecType = msAPI_MM_GetOption((int)MM_OPTION_MOVIE_INFO, (int)MM_INFO_VIDEO_CODEC);
    if((u32CodecType == MPLAYER_VIDEO_CODEC_MPEG4) ||
        (u32CodecType == MPLAYER_VIDEO_CODEC_MPEG2) ||
        (u32CodecType == MPLAYER_VIDEO_CODEC_VC1) ||
        (u32CodecType == MPLAYER_VIDEO_CODEC_FLV))
        tMvopInputSel = MVOP_INPUT_MVD;
    else if((u32CodecType == MPLAYER_VIDEO_CODEC_H264) ||
        (u32CodecType == MPLAYER_VIDEO_CODEC_RM))
        tMvopInputSel = MVOP_INPUT_H264;
    else if(u32CodecType == MPLAYER_VIDEO_CODEC_MJPEG)
        tMvopInputSel = MVOP_INPUT_JPD;
    MDrv_MVOP_SetInputCfg(tMvopInputSel, NULL);

    //msAPI_MM_GetDispInfo(&MM_DispInfo);
    MApi_MPlayer_Get_DispInfo(E_MPLAYER_MEDIA_MOVIE, (video_disp_info*)&MM_DispInfo);

    memset(&stVidStatus, 0, sizeof(VDEC_EX_DispInfo));

    //Fill Video information for msAPI_XC lib
    stVidStatus.u16CropLeft = MM_DispInfo.dCropLeft;
    stVidStatus.u16CropRight = MM_DispInfo.dCropRight;
    stVidStatus.u16CropTop = MM_DispInfo.dCropTop;
    stVidStatus.u16CropBottom = MM_DispInfo.dCropBottom;
    stVidStatus.u16SarWidth   = 0;
    stVidStatus.u16SarHeight = 0;
    stVidStatus.u16HorSize    = MM_DispInfo.dWidth;
    stVidStatus.u16VerSize    = MM_DispInfo.dHeight;
    stVidStatus.u32FrameRate  = MM_DispInfo.dFrameRate;
    stVidStatus.u8Interlace   = MM_DispInfo.dInterlace;

    msAPI_XC_SetVDECInfo_EX(&gtVDECStreamId,&stVidStatus);

    db_print("MM_DispInfo dWidth %d, dHeight %d, dFrameRate %d dInterlace %d\n",
         MM_DispInfo.dWidth, MM_DispInfo.dHeight, MM_DispInfo.dFrameRate, MM_DispInfo.dInterlace);
    db_print("MM_DispInfo dCropLeft %d, dCropRight %d, dCropTop %d, dCropBottom %d\n",
         MM_DispInfo.dCropLeft, MM_DispInfo.dCropRight, MM_DispInfo.dCropTop, MM_DispInfo.dCropBottom);
    memset(&stMvopVidSt, 0, sizeof(MVOP_VidStat));
    stMvopVidSt.u16HorSize = (MS_U16)MM_DispInfo.dWidth;
    stMvopVidSt.u16VerSize = MM_DispInfo.dHeight;
    stMvopVidSt.u16FrameRate = MM_DispInfo.dFrameRate;
    stMvopVidSt.u8AspectRate = 2;//pstVidStatus->u8AspectRate;
    stMvopVidSt.u8Interlace = MM_DispInfo.dInterlace;//0;//pstVidStatus->u8Interlace;

    MDrv_MVOP_SetOutputCfg(&stMvopVidSt, FALSE);
    MDrv_MVOP_SetStartPos(0,0);

    MDrv_MVOP_Enable(TRUE); //ENABLE MVOP, Video pass to Scaler

    db_print("->u32FrameRate = %u\n", stMvopVidSt.u16FrameRate);
    db_print("->u8Interlace = %x\n", stMvopVidSt.u8Interlace);
    db_print("->u16HorSize = %u\n", stMvopVidSt.u16HorSize);
    db_print("->u16VerSize = %u\n", stMvopVidSt.u16VerSize);
    //db_print("->u16Crop R/L = %u, %u\n", stMvopVidSt.u16CropRight, stMvopVidSt.u16CropLeft);
    //db_print("->u16Crop B/T = %u, %u\n", stMvopVidSt.u16CropBottom, stMvopVidSt.u16CropTop);
    db_print("->u8AspectRate = %u\n", stMvopVidSt.u8AspectRate);
    //db_print("->u16SarWidth = %u\n", stMvopVidSt.u16SarWidth);
    //db_print("->u16SarHeight = %u\n", stMvopVidSt.u16SarHeight);

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief to Get Multi-Media Video Decoder Status and Set Output Config
/// Usage: Video_GetDecoderStatus
/// @sa
/// @note
/// Sample Command: \n
///          Video_GetDecoderStatus
//------------------------------------------------------------------------------
MS_BOOL appDemo_MM_Video_GetDecoderStatus(void)
{
    VDEC_EX_DispInfo stVidStatus;

#if 0 //george 110822
    MS_U32 u32Loop = VIDEO_FRAMECOUNT_LOOP;

    while(msAPI_MM_GetOption((int)MM_OPTION_MOVIE_INFO, (int)MM_INFO_INDEX_FIRST_FRAME) != TRUE)
    {
        u32Loop--;
        if(u32Loop==0)
        {
            db_print(" Video DispInfo is Not Ready!! \n");
            return FALSE;
        }
        MsOS_DelayTask(10);
    }
#endif

    if(TRUE != _appDemo_MM_Video_SetOutput(&stVidStatus))
    {
        db_print(" Video Set Output Config Failed!! \n");
        return FALSE;
    }

    return TRUE;
}
#endif

//------------------------------------------------------------------------------
/// @brief to Get Video Decoder Status and Set Output Config \n
/// - For Video Decoder:
///     - check display information is ready
///     - check frame count is increasing
///     - check AV sync done
/// - For MVOP and SCALER:
///     - get video information and set output information of MVOP
///     - the output of MVOP is the input of SCALER
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b Video_GetDecoderStatus \n
//------------------------------------------------------------------------------
extern MS_U32 gu32Time;
MS_BOOL appDemo_Video_GetDecoderStatus(void)
{
    VDEC_EX_DispInfo stVidStatus;
    MS_U32 u32StartTime = 0, u32EndTime = 0;
    static MS_U32 u32SyncStarTime, u32SyncTimeDiff;
    MS_U32 u32Loop = VIDEO_FRAMECOUNT_LOOP;
    MS_BOOL bSyncFlag = FALSE;
    MVOP_InputSel tMvopInputSel = MVOP_INPUT_UNKNOWN;

    ////////////////////////////////////////////////////////////
    //Check if information for display setting is ready or not//
    //And disable VDEC blue screen                            //
    ////////////////////////////////////////////////////////////
    u32StartTime = _appDemo_GetTime();

    while(E_VDEC_EX_OK != MApi_VDEC_EX_CheckDispInfoRdy(&gtVDECStreamId))
    {
        u32Loop--;
        if(u32Loop==0)
        {
            u32EndTime = _appDemo_GetTime();
            db_print(" Timeout!! Video DispInfo is Not Ready!!\n");
            db_print("[%s][%d] DIFF TIME to check Disp Info = %lu ms\n", __FUNCTION__, __LINE__, u32EndTime - u32StartTime);
            return FALSE;
        }
        MsOS_DelayTask(u32GetDecStatusDelay);
    }
    u32EndTime = _appDemo_GetTime();
    db_print("[%s][%d] DIFF TIME to check Disp Info = %lu ms\n", __FUNCTION__, __LINE__, u32EndTime - u32StartTime);

    MApi_VDEC_EX_SetBlueScreen( &gtVDECStreamId,FALSE );

    /////////////////////
    //check frame count//
    /////////////////////
    u32StartTime = _appDemo_GetTime();

    u32Loop = VIDEO_FRAMECOUNT_LOOP;

    //check frame ready
/*
    VDEC_Result ret = E_VDEC_FAIL;
    do
    {
        u32Loop--;
        if(u32Loop==0)
        {
            db_print("get frame count timeout!! \n");
            return FALSE;
        }

        MsOS_DelayTask(10);
        ret = MApi_VDEC_IsFrameRdy();
        //db_print("Frame Ready Ret = %u \n", ret);
    }while(ret != E_VDEC_OK);
*/
    /////////////////////////
    //get video information//
    /////////////////////////
    if(TRUE != _appDemo_Video_GetVidInfo(&stVidStatus))
    {
        db_print(" Video Get Info Failed!! \n");
        return FALSE;
    }

    /////////////////////////////////////
    //Set VDEC information and Set MVOP//
    ////////////////////////////////////
    tMvopInputSel = _appDemo_GetMVOPInputSel();

    msAPI_XC_SetVDECInfo_EX(&gtVDECStreamId,&stVidStatus);

    if(TRUE != msAPI_XC_SetMVOPConfig(tMvopInputSel))
    {
        db_print(" Video Set Output Config Failed!! \n");
        return FALSE;
    }

    u32EndTime = _appDemo_GetTime();
    db_print("[%s][%d] DIFF TIME to Set Mvop = %lu ms\n", __FUNCTION__, __LINE__, u32EndTime - u32StartTime);

    //////////////////////
    //check AV sync done//
    //////////////////////
    u32SyncStarTime = _appDemo_GetTime();
    if(_bVideo_WaitAVSync == TRUE)
    {
        while(bSyncFlag != TRUE)
        {
            u32SyncTimeDiff = _appDemo_TimeDiffFromNow(u32SyncStarTime);
            db_print("SyncTimeDiff = %lu \n", u32SyncTimeDiff);
            if(_appDemo_IsSyncDone())
            {
               db_print("*****************Sync Done %ld ms\n", u32SyncTimeDiff);
               bSyncFlag = TRUE;
            }
            else if (u32SyncTimeDiff > AVSYNC_TIMEOUT_INTERVAL) // set timeout
            {
                db_print("*****************Sync Timeout %ld ms\n", u32SyncTimeDiff);
                bSyncFlag = TRUE;
            }
            MsOS_DelayTask(u32GetDecStatusDelay);
        }
    }

    u32EndTime = _appDemo_GetTime();
    db_print("[%s][%d] DIFF TIME to wait AV-Sync = %lu ms\n", __FUNCTION__, __LINE__, u32EndTime - u32SyncStarTime);

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Set Flag to wait AV Sync or not \n
/// @param[in] *pEnable 1=ENABLE, 0=DISABLE
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b Video_WaitAVSync 0 \n
///          \b Video_WaitAVSync 1 \n
//------------------------------------------------------------------------------
MS_BOOL appDemo_Video_WaitAVSync(MS_U32 *pEnable)
{
    _bVideo_WaitAVSync = *pEnable;

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Set Video as Blue Screen/Black Screen \n
/// @param[in] *pEnable 1=ENABLE, 0=DISABLE
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b Video_Mute 0 \n
///          \b Video_Mute 1 \n
//------------------------------------------------------------------------------
MS_BOOL appDemo_Video_Mute(MS_U32 *pEnable)
{
    //Enable Blue Screen to avoid garbage video
    msAPI_VE_SetVideoMute(*pEnable);
    msAPI_XC_SetVideoMute(*pEnable, MAIN_WINDOW);
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Set DACDetect task \n
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b Video_DACDetect  \n
///          \b Video_DACDetect  \n
//------------------------------------------------------------------------------
MS_BOOL appDemo_Video_DACDetect(void)
{
    if(s32DetectTaskID == -1)
    {
        _pDetectTaskStack = MsOS_AllocateMemory(DAC_DETECTTASK_STACK_SIZE, gs32CachedPoolID);

        if(_pDetectTaskStack == NULL)
        {
            db_print("Task stack create fail!\n");
            return FALSE;
        }

        s32DetectTaskID = MsOS_CreateTask((TaskEntry)_appDemo_Video_DACDetect,
                                (MS_U32)NULL,
                                E_TASK_PRI_HIGH,
                                TRUE,
                                _pDetectTaskStack,
                                DAC_DETECTTASK_STACK_SIZE,
                                "DAC_Detect_Task");

        if(s32DetectTaskID == -1)
        {
            db_print("Task create fail!\n");
            return FALSE;
        }

        db_print("DetectTask Create %d!\n",s32DetectTaskID);
        return TRUE;
    }
    else
    {
        db_print("Task Alread Create!\n");
        return FALSE;
    }
}

//------------------------------------------------------------------------------
/// @brief to enable/disable selected video output destination:
/// - Video_SetOutputDest [Dest] [bEnable] \n
/// [Dest]:
///     -    OUTPUT_DESTINATION_SD_CVBS      = 0,
///     -    OUTPUT_DESTINATION_SD_SVIDEO    = 1,
///     -    OUTPUT_DESTINATION_SD_SCART_RGB = 2,
///     -    OUTPUT_DESTINATION_SD_YPBPR     = 3,
///     -    OUTPUT_DESTINATION_HD_COMPONENT = 4,
///     -    OUTPUT_DESTINATION_HD_HDMITX    = 5,
/// @param[in] *pu16OutputDest Output Destination Selection of E_OUTPUT_DESTINTATION
/// @param[in] *pEnable 1=ENABLE, 0=DISABLE
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b Video_SetOutputDest 3 0 \n
///          \b Video_SetOutputDest 3 1 \n
//------------------------------------------------------------------------------
MS_BOOL appDemo_Video_SetOutputDest(MS_U16 *pu16OutputDest, MS_U16 *pEnable)
{
    switch(*pu16OutputDest)
    {
        case OUTPUT_DESTINATION_SD_CVBS:
        case OUTPUT_DESTINATION_SD_SVIDEO:
        case OUTPUT_DESTINATION_SD_SCART_RGB:
        case OUTPUT_DESTINATION_SD_YPBPR:
            appDemo_VE_SetOutputDest(pu16OutputDest, pEnable);
            break;
        case OUTPUT_DESTINATION_HD_COMPONENT:
        case OUTPUT_DESTINATION_HD_HDMITX:
        default:
            appDemo_XC_SetOutputDest(pu16OutputDest, pEnable);
            break;
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Set Scaler Crop Window Size(for cutting function) \n
/// Usage: Video_HD_SetCropWin [ScalerWindow] [XStart] [YStart] [HSize] [VSize]:
///     - ScalerWindow: Scaler Window Selection:
///         - MAIN_WINDOW   = 0
///         - SUB_WINDOW    = 1
/// @param[in] *pu32ScalerWindow Scaler Window Selection
/// @param[in] *x Start Position of X
/// @param[in] *y Start Position of Y
/// @param[in] *width Horizontal Size
/// @param[in] *height Vertical Size
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b Video_HD_SetCropWin 0 0 0 360 240 \n
///          \b Video_HD_SetCropWin 0 0 0 720 480 \n
//------------------------------------------------------------------------------
MS_BOOL appDemo_Video_HD_SetCropWin(MS_U32 *pu32ScalerWindow, MS_U16 *x, MS_U16 *y, MS_U16 *width, MS_U16 *height)
{
    g_Demo_DispInfo.HD_CropWin[*pu32ScalerWindow].x = *x;
    g_Demo_DispInfo.HD_CropWin[*pu32ScalerWindow].y = *y;
    g_Demo_DispInfo.HD_CropWin[*pu32ScalerWindow].width= *width;
    g_Demo_DispInfo.HD_CropWin[*pu32ScalerWindow].height = *height;
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Set Scaler Destination Window(Display Window) Size \n
/// Usage: Video_HD_SetDestWin [ScalerWindow] [XStart] [YStart] [HSize] [VSize]:
///     - ScalerWindow: Scaler Window Selection:
///         - MAIN_WINDOW   = 0
///         - SUB_WINDOW    = 1
/// @param[in] *pu32ScalerWindow Scaler Window Selection
/// @param[in] *x Start Position of Y
/// @param[in] *y Start Position of Y
/// @param[in] *width Horizontal Size
/// @param[in] *height Vertical Size
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b Video_HD_SetDestWin 0 0 0 1280 720 \n
///          \b Video_HD_SetDestWin 0 360 240 920 480 \n
//------------------------------------------------------------------------------
MS_BOOL appDemo_Video_HD_SetDestWin(MS_U32 *pu32ScalerWindow, MS_U16 *x, MS_U16 *y, MS_U16 *width, MS_U16 *height)
{
    g_Demo_DispInfo.HD_DestWin[*pu32ScalerWindow].x = *x;
    g_Demo_DispInfo.HD_DestWin[*pu32ScalerWindow].y = *y;
    g_Demo_DispInfo.HD_DestWin[*pu32ScalerWindow].width= *width;
    g_Demo_DispInfo.HD_DestWin[*pu32ScalerWindow].height = *height;

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Set Scaler Aspect Ratio Type \n
/// Usage: Video_HD_SetAS [eAS]:
///    - E_ASPECT_RATIO_NORMAL           = 0,
///    - E_ASPECT_RATIO_4_3              = 1,
///    - E_ASPECT_RATIO_16_9             = 2,
///    - E_ASPECT_RATIO_CUT_4_3          = 3,
///    - E_ASPECT_RATIO_CUT_16_9         = 4,
///    - E_ASPECT_RATIO_FULL             = 5,
/// @param[in] *pu32HD_AS aspect ratio type of E_ASPECT_RATIO_TYPE
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b Video_HD_SetAS 0 \n
///          \b Video_HD_SetAS 2 \n
//------------------------------------------------------------------------------
MS_BOOL appDemo_Video_HD_SetAS(MS_U32 *pu32HD_AS)
{
    g_Demo_DispInfo.HD_AS = *pu32HD_AS;
    msAPI_XC_SetAspectRatioType(g_Demo_DispInfo.HD_AS);
    return TRUE;
}


//------------------------------------------------------------------------------
/// @brief Set Scaler Aspect Ratio Type \n
/// Usage: Video_HD_SetAsInfo [eWAS] [eHAS]:
/// @param[in] *pu32width aspect ratio width
/// @param[in] *pu32height aspect ratio height
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b Video_HD_SetCusAsInfo 5 7 \n
///          \b Video_HD_SetCusAsInfo 6 5 \n
//------------------------------------------------------------------------------

MS_BOOL appDemo_Video_HD_SetCusASInfo(MS_U32 *pu32AspectRatio, MS_U32 *pu32width, MS_U32 *pu32height)
{
    if((*pu32AspectRatio)>=E_AR_INVALID)
    {
        printf("Invalid Aspect Ratio Type %lu\n", *pu32AspectRatio);
        return FALSE;
    }

	g_eAspectRatio = (E_AR_TYPE)*pu32AspectRatio;

    if((g_eAspectRatio != E_AR_4x3_Combind) && (g_eAspectRatio != E_AR_16x9_Combind))
    {
        if ((*pu32height == 0) || (*pu32width == 0))
        {
            printf("Invalid Input %lu:%lu\n", *pu32width, *pu32height);
            return FALSE;
        }

        g_u16CusHRatio = *pu32width;
        g_u16CusVRatio = *pu32height;
    }

    return TRUE;
}




//------------------------------------------------------------------------------
/// @brief Set VE Capture Window(Source Window) Size(for cutting function) \n
/// Usage: Video_SD_SetSrcWin [XStart] [YStart] [HSize] [VSize] \n
/// \b \n
/// @param[in] *x Start Position of X
/// @param[in] *y Start Position of Y
/// @param[in] *width Horizontal Size
/// @param[in] *height Vertical Size
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b Video_SD_SetSrcWin 0 0 360 240 \n
///          \b Video_SD_SetSrcWin 0 0 720 480 \n
//------------------------------------------------------------------------------
MS_BOOL appDemo_Video_SD_SetSrcWin(MS_U16 *x, MS_U16 *y, MS_U16 *width, MS_U16 *height)
{
    g_Demo_DispInfo.SD_SrcWin.x = *x;
    g_Demo_DispInfo.SD_SrcWin.y = *y;
    g_Demo_DispInfo.SD_SrcWin.width= *width;
    g_Demo_DispInfo.SD_SrcWin.height= *height;

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Set VE Destination Window(Display Window) Size \n
/// Usage: Video_SD_SetDestWin [XStart] [YStart] [HSize] [VSize] \n
/// @param[in] *x Start Position of X
/// @param[in] *y Start Position of Y
/// @param[in] *width Horizontal Size
/// @param[in] *height Vertical Size
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b Video_SD_SetDestWin 0 0 720 480 \n
///          \b Video_SD_SetDestWin 360 240 360 240 \n
//------------------------------------------------------------------------------
MS_BOOL appDemo_Video_SD_SetDestWin(MS_U16 *x, MS_U16 *y, MS_U16 *width, MS_U16 *height)
{
    g_Demo_DispInfo.SD_DestWin.x = *x;
    g_Demo_DispInfo.SD_DestWin.y = *y;
    g_Demo_DispInfo.SD_DestWin.width= *width;
    g_Demo_DispInfo.SD_DestWin.height= *height;

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Set VE Aspect Ratio Type \n
/// Usage: Video_SD_SetAS [eAS]:
///    - E_ASPECT_RATIO_NORMAL           = 0,
///    - E_ASPECT_RATIO_4_3              = 1,
///    - E_ASPECT_RATIO_16_9             = 2,
///    - E_ASPECT_RATIO_CUT_4_3          = 3,
///    - E_ASPECT_RATIO_CUT_16_9         = 4,
///    - E_ASPECT_RATIO_FULL             = 5,
/// @param[in] *pu32SD_AS aspect ratio type of E_ASPECT_RATIO_TYPE
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b Video_SD_SetAS 1 \n
///          \b Video_SD_SetAS 5 \n
//------------------------------------------------------------------------------
MS_BOOL appDemo_Video_SD_SetAS(MS_U32 *pu32SD_AS)
{
    g_Demo_DispInfo.SD_AS = *pu32SD_AS;

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Set VE Capture Mode \n
/// Usage: Video_SD_SetCaptureMode [Enable]:
///    - Default Mode(DE Mode)          = 0, (Capture HD Video which is in DE region)
///    - Capture Mode                   = 1, (Capture HD video which is in display region)
/// @param[in] *pEnable 1=ENABLE, 0=DISABLE
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b Video_SD_SetCaptureMode 0 \n
///          \b Video_SD_SetCaptureMode 1 \n
//------------------------------------------------------------------------------
MS_BOOL appDemo_Video_SD_SetCaptureMode(MS_U32 *pEnable)
{
    g_Demo_VE_CaptureModeEn = *pEnable;
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Play Video Using Customer Setting(crop size, window size, aspect ratio...):
///         -   HD: DTV -> XC -> HDMITx/Component
///         -   SD: DTV -> XC -> VE Source -> VE Encoder
/// @param[in] *pu32ScalerWindow window type of SACLER(set to MAIN_WINDOW as Default)
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b Video_PlayVideo_UsingCusSetting 0 \n
//------------------------------------------------------------------------------
MS_BOOL appDemo_Video_PlayVideo_UsingCusSetting(MS_U32 *pu32ScalerWindow)
{
    return appDemo_XC_PlayVideo_UsingCusSetting(pu32ScalerWindow);
}


//------------------------------------------------------------------------------
/// @brief Set Video Brightness
/// @param[in] *pu32ScalerWindow SCALER display window selection
/// @param[in] *pu8Value brightness value(0~255)
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b Video_SetBrightness 0 50 \n
//------------------------------------------------------------------------------
MS_BOOL appDemo_Video_SetBrightness(MS_U32 *pu32ScalerWindow, MS_U8 *pu8Value)
{
    appDemo_XC_SetBrightness(pu32ScalerWindow, pu8Value);
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Set Video Sharpness
/// @param[in] *pu32ScalerWindow SCALER display window selection
/// @param[in] *pu8Value Sharpness value(0~63)
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b Video_SetSharpness 0 50 \n
//------------------------------------------------------------------------------
MS_BOOL appDemo_Video_SetSharpness(MS_U32 *pu32ScalerWindow, MS_U8 *pu8Value)
{
    appDemo_XC_SetSharpness(pu32ScalerWindow, pu8Value);
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Set Flag for Scaler RFBL Mode \n
/// to save Bandwidth and enable field skip for 60->50 Frame Rate Convert in some MSTAR chip \n
/// Usage: Video_HD_SetRFBLMode [Enable]:
///    - Default Mode(Frame Buffer Mode)            = 0, (fully access MIU)
///    - RFBL Mode(Request Frame Buffer Less Mode)  = 1, (some frames/fields store in framebuffer, some not)
///    to save Bandwidth and enable field skip for 60->50 Frame Rate Convert in some MSTAR chip
/// @param[in] *pEnable 1=ENABLE, 0=DISABLE
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b Video_HD_SetRFBLMode 0 \n
///          \b Video_HD_SetRFBLMode 1 \n
//------------------------------------------------------------------------------
MS_BOOL appDemo_Video_HD_SetRFBLMode(MS_U32 *pEnable)
{
    msAPI_XC_EnableRFBLMode((MS_BOOL)*pEnable);
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief enable CCIR input path \n
/// @param[in] *pEnable 1=ENABLE, 0=DISABLE
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b Video_CCIRIn \n
//------------------------------------------------------------------------------
MS_BOOL appDemo_Video_CCIRIn(void)
{
    appDemo_XC_Init();
    appDemo_VE_Init();

    MS_U32 eCCIRType = (MS_U32)E_CCIR656_INPUT_720X480_60I;
    appDemo_XC_CCIRIn(&eCCIRType);

    return TRUE;
}

MS_BOOL appDemo_Video_PreSetControl(void)
{
    MApi_VDEC_EX_PreSetControl(&gtVDECStreamId,E_VDEC_EX_USER_CMD_HVD_ONE_PENDING_BUFFER_MODE, 1);
    return TRUE;
}


VDEC_StreamId * appDemo_Video_GetStreamID()
{
    VDEC_StreamId stStreamId = {0};
    MApi_VDEC_EX_GetControl(&stStreamId, E_VDEC_EX_USER_CMD_GET_MAIN_STREAM_ID, (MS_U32*)&gtVDECStreamId);

    return &gtVDECStreamId;
}

MS_BOOL appDemo_Video_SetDecStatusDelay(MS_U32 u3Ms)
{
    u32GetDecStatusDelay = u3Ms;
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief list description and sample comand of video demo code
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b helpvideo \n
//------------------------------------------------------------------------------
MS_BOOL appDemo_Video_Help(void)
{
    printf ("************************* Video function List *************************\n");
    printf ("1.Video_GetDecoderStatus \n");
    printf ("2.Video_SetOutputDest [Dest] [bEnable] \n");
    printf ("-----Dest : Output Destination Selection[0-4]\n");
    printf ("----------: OUTPUT_DESTINATION_SD_CVBS      = 0,\n");
    printf ("----------: OUTPUT_DESTINATION_SD_SVIDEO    = 1,\n");
    printf ("----------: OUTPUT_DESTINATION_SD_SCART_RGB = 2,\n");
    printf ("----------: OUTPUT_DESTINATION_SD_YPBPR     = 3,\n");
    printf ("----------: OUTPUT_DESTINATION_HD_COMPONENT = 4,\n");
    printf ("----------: OUTPUT_DESTINATION_HD_HDMITX    = 5,\n");
    printf ("-----bEnable : Enable = 1, Disable =0\n");
    printf ("====>example : Video_SetOutputDest 4 0\n");
    printf ("====>example : Video_SetOutputDest 4 1\n");
    printf ("3.Video_HD_SetCropWin [ScalerWindow] [XStart] [YStart] [HSize] [VSize] \n");
    printf ("====>example : Video_HD_SetCropWin 0 0 0 704 480\n");
    printf ("4.Video_HD_SetDestWin [ScalerWindow] [XStart] [YStart] [HSize] [VSize] \n");
    printf ("====>example : Video_HD_SetDestWin 0 0 0 1280 720\n");
    printf ("5.Video_HD_SetAS [eAS] \n");
    printf ("-----eAS  : Enum of Aspect Ratio Type\n");
    printf ("----------: E_ASPECT_RATIO_NORMAL           = 0,\n");
    printf ("----------: E_ASPECT_RATIO_4_3              = 1,\n");
    printf ("----------: E_ASPECT_RATIO_16_9             = 2,\n");
    printf ("----------: E_ASPECT_RATIO_CUT_4_3          = 3,\n");
    printf ("----------: E_ASPECT_RATIO_CUT_16_9         = 4,\n");
    printf ("----------: E_ASPECT_RATIO_FULL             = 5,\n");
    printf ("----------: E_ASPECT_RATIO_CUSTOM1          = 10,\n");
    printf ("====>example : Video_HD_SetAS 2\n");
    printf ("6.Video_SD_SetSrcWin [XStart] [YStart] [HSize] [VSize] \n");
    printf ("====>example : Video_SD_SetSrcWin 0 0 1280 720 \n");
    printf ("7.Video_SD_SetDestWin [XStart] [YStart] [HSize] [VSize] \n");
    printf ("====>example : Video_SD_SetDestWin 0 0 720 576\n");
    printf ("8.Video_SD_SetAS [eAS] \n");
    printf ("-----eAS  : Enum of Aspect Ratio Type\n");
    printf ("----------: E_ASPECT_RATIO_NORMAL           = 0,\n");
    printf ("----------: E_ASPECT_RATIO_4_3              = 1,\n");
    printf ("----------: E_ASPECT_RATIO_16_9             = 2,\n");
    printf ("----------: E_ASPECT_RATIO_CUT_4_3          = 3,\n");
    printf ("----------: E_ASPECT_RATIO_CUT_16_9         = 4,\n");
    printf ("----------: E_ASPECT_RATIO_FULL             = 5,\n");
    printf ("====>example : Video_SD_SetAS 5 (Set to 5 for Full Screen)\n");
    printf ("9.Video_PlayVideo_UsingCusSetting [ScalerWindow] \n");
    printf ("----@brief: Play Video Using Customer Setting(with crop size, window size, aspect ratio...)\n");
    printf ("--------HD: DTV -> XC -> HDMITx/Component\n");
    printf ("--------SD: DTV -> XC -> VE Source -> VE Encoder\n");
    printf ("====>example : Video_PlayVideo_UsingCusSetting 0\n");
    printf ("10.Video_SD_SetCaptureMode [Enable] \n");
    printf ("----@brief: Set VE Capture Mode \n");
    printf ("--------Default Mode(DE Mode) = 0, (Capture HD Video which is in DE region)\n");
    printf ("--------Capture Mode          = 1, (Capture HD video which is in display region)\n");
    printf ("====>example : Video_SD_SetCaptureMode 0\n");
    printf ("====>example : Video_SD_SetCaptureMode 1\n");
    printf ("11.Video_DTV_SetMode \n");
    printf ("----@brief: DTV Set Mode(Set VDEC FRC, MVOP and Scaler FBL Mode) \n");
    printf ("====>example : Video_DTV_SetMode \n");
    printf ("12.Video_HD_SetRFBLMode \n");
    printf ("----@brief: Set Flag for Scaler RFBL Mode \n");
    printf ("====>example : Video_HD_SetRFBLMode 0 \n");
    printf ("====>example : Video_HD_SetRFBLMode 1 \n");
    printf ("13.Video_WaitAVSync \n");
    printf ("----@brief: Set Flag for AV-Sync \n");
    printf ("====>example : Video_WaitAVSync 0 \n");
    printf ("====>example : Video_WaitAVSync 1 \n");
    printf ("14.Video_CCIRIn \n");
    printf ("----@brief: CCIR Input Demo \n");
    printf ("====>example : Video_CCIRIn \n");
    printf ("15.Video_HD_SetCusAsInfo \n");
    printf ("----@brief: Set Customer Aspect Ratio Info\n");
    printf ("====>example : Video_HD_SetCusAsInfo 7 5 \n");
    return TRUE;
}

