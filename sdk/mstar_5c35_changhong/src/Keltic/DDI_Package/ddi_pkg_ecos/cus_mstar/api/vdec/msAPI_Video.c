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
#define OBA2 0
#define AUTO_ASPECT_RATIO 0
////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2009 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (¡§MStar Confidential Information¡¨) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
/// @file msAPI_Video.h
/// @brief API for MVD Video
/// @author MStar Semiconductor, Inc.
//
////////////////////////////////////////////////////////////////////////////////

#define MSAPI_VIDEO_C

/******************************************************************************/
/*                    Header Files                                            */
/* ****************************************************************************/
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#else
#include <stdio.h>
#include <string.h>
#endif
#include "MsCommon.h"
#include "MsTypes2.h"
#include "MsMemory.h"
#include "Board.h"
//#include "ver_dtv_comm.h"

//Drv
#include "drvXC_IOPort.h"
#include "drvMVOP.h"

//API
#include "apiXC.h"
#include "apiVDEC.h"
#include "apiXC.h"

//msAPI
#include "msAPI_Video.h"

/******************************************************************************/
/*                     Local                                                  */
/* ****************************************************************************/
static U8 u8SyncEnable                      = 1;

#if (ENABLE_DMP || ENABLE_PVR)
static VDEC_TrickDec eTrickDec;
static U8 u8DisplayDuration                 = 1;
#endif

static U8 u8PlayMode                        = MSAPI_VID_STOP;

static U32 _u32PrevWP;
static U32 _u32VdecBsBufLen                 = VDEC_AEON_LEN;

static code U16 _H264_ASPECT_RATIO[17][2]   = {{1,1},
                                               {1,1},
                                               {12,11},
                                               {10,11},
                                               {16,11},
                                               {40,33},
                                               {24,11},
                                               {20,11},
                                               {32,11},
                                               {80,33},
                                               {18,11},
                                               {15,11},
                                               {64,33},
                                               {160,99},
                                               {4,3},
                                               {3,2},
                                               {2,1}};

/******************************************************************************/
/*                   Functions                                                */
/******************************************************************************/

void _msAPI_VID_SetMemoryLayout(VDEC_InitParam *pInitParam)
{
#if (ENABLE_MIU_1 == ENABLE)
    pInitParam->SysConfig.u32CodeBufAddr = ((VDEC_AEON_MEMORY_TYPE & MIU1) ? (VDEC_AEON_ADR | MIU_INTERVAL) : (VDEC_AEON_ADR));
    pInitParam->SysConfig.u32CodeBufSize = VDEC_AEON_LEN;
    pInitParam->SysConfig.u32FrameBufAddr = VDEC_FRAME_BUF_ADR | 0x8000000;
    pInitParam->SysConfig.u32FrameBufSize = VDEC_FRAME_BUF_LEN;
    pInitParam->SysConfig.u32BitstreamBufAddr = VDEC_BIT_STREAM_ADR | 0x8000000;
    pInitParam->SysConfig.u32BitstreamBufSize = VDEC_BIT_STREAM_LEN;
#else
    pInitParam->SysConfig.u32CodeBufAddr = VDEC_AEON_ADR;
    pInitParam->SysConfig.u32CodeBufSize = VDEC_AEON_LEN;
    pInitParam->SysConfig.u32FrameBufAddr = VDEC_FRAME_BUF_ADR;
    pInitParam->SysConfig.u32FrameBufSize = VDEC_FRAME_BUF_LEN;
    pInitParam->SysConfig.u32BitstreamBufAddr = VDEC_BIT_STREAM_ADR;
    pInitParam->SysConfig.u32BitstreamBufSize = VDEC_BIT_STREAM_LEN;
#endif
}


U32 msAPI_VID_GetESWriteCnt(void)
{
    U32 cnt = 0;
    MS_U32 ptr = MApi_VDEC_GetESWritePtr();

    if (_u32PrevWP < ptr)
    {
        cnt = ptr - _u32PrevWP;
    }
    else
    {
        cnt = _u32VdecBsBufLen - (_u32PrevWP - ptr);
    }

    _u32PrevWP = ptr;

    return cnt;
}

//******************************************************************************
/// issue Video command
/// @param u8VIDCommand \b IN
///   - # MSAPI_VID_PLAY play command
///   - # MSAPI_VID_PAUSE pause command
///   - # MSAPI_VID_STOP stop command
///   - # MSAPI_VID_RESET reset command
///   - # MSAPI_VID_FREERUN disable video-sync command
//******************************************************************************
#if OBA2
extern VDEC_Result MApi_VDEC_Exit(void);
#define MApi_VDEC_Stop MApi_VDEC_Exit
#endif

void msAPI_VID_Command(U8 u8VIDcommand)
{
    u8PlayMode = u8VIDcommand;

    VDEC_Result ret = E_VDEC_FAIL;


    switch (u8VIDcommand)
    {
        case MSAPI_VID_PLAY:
            ret = MApi_VDEC_Play();
            if( E_VDEC_OK != ret )
            {
               printf("msAPI_VID_Command(0x%x) not success! ret = %d\n", u8VIDcommand, ret);
            }
            // Must Call AVSyncOn to do AV syyc
            //ret = 0;//MApi_VDEC_AVSyncOn(TRUE,0,0);

            break;

        case MSAPI_VID_RESUME:
            ret = MApi_VDEC_Resume();
            u8SyncEnable = 1;

            break;

        case MSAPI_VID_PAUSE:
            ret = MApi_VDEC_Pause();

            break;

        case MSAPI_VID_STOP:
            ret = MApi_VDEC_Exit();

            if (E_VDEC_RET_NOT_INIT == ret) // already stopped, fine to return success
            {
                return;
            }

            break;

        case MSAPI_VID_RESET:
            ret = MApi_VDEC_Rst();

            break;

        case MSAPI_VID_FREERUN:
            ret = MApi_VDEC_AVSyncOn(FALSE, 0, 0);
            u8SyncEnable = 0;

            break;

#if ENABLE_DMP
        case MSAPI_VID_TRICK_PLAY:
            ret = MApi_VDEC_SetTrickMode(eTrickDec);
            u8SyncEnable = 0;

            break;

        case MSAPI_VID_STEP_PLAY:
            ret = MApi_VDEC_StepDisp();
            u8SyncEnable = 0;

            break;

        case MSAPI_VID_PLAY_EXTENSION:
            ret = MApi_VDEC_Play();
            u8SyncEnable = 1;

            break;
#endif  // #if ENABLE_DMP

        case MSAPI_VID_STEP_PLAY_EXTENSION:
            ret = MApi_VDEC_StepDisp();
            u8SyncEnable = 0;

            break;

        default:
            break;
    }

    if (E_VDEC_OK != ret)
    {
        printf("msAPI_VID_Command(0x%x) not success! ret = %d\n", u8VIDcommand, ret);
    }
}

#if (ENABLE_DMP || ENABLE_PVR)
//******************************************************************************
//Function name:    msAPI_VID_SetTrickModeInfo
//Passing parameter:    U8 u8DecodeType
//                      U8 u8FrameDuration
//Return parameter:     none
//Description:  set MVD command
//  [Doxygen]
/// API for set MVD command
/// @param u8MVDCommand \b IN MVD command, e.g., PLAY, PAUSE, STOP, CHANNEL_CHANGE, RESET
//******************************************************************************
void msAPI_VID_SetTrickModeInfo(U8 u8DecodeType, U8 u8FrameDuration)
{
    SHOW_VIDEO_INFO(("msAPI_VID_SetTrickModeInfo"));
    u8DisplayDuration = u8FrameDuration;

//    if (bAVCH264)
    if(msAPI_VID_GetCodecType() == E_VDEC_CODEC_TYPE_H264)
    {
        if (u8DecodeType == 0)
        {
            eTrickDec = E_VDEC_TRICK_DEC_ALL;
        }
        else if (u8DecodeType == 1)
        {
            eTrickDec = E_VDEC_TRICK_DEC_I;
        }
        else if (u8DecodeType == 2)
        {
            eTrickDec = E_VDEC_TRICK_DEC_IP;
        }
        else
        {
            printf("dec type %d is undefined\n", u8DecodeType);
        }
    }
    else
    {
        if (u8DecodeType == 0)
        {
            eTrickDec = E_VDEC_TRICK_DEC_ALL;
        }
        else if (u8DecodeType == 1)
        {
            eTrickDec = E_VDEC_TRICK_DEC_IP;
        }
        else if (u8DecodeType == 2)
        {
            eTrickDec = E_VDEC_TRICK_DEC_I;
        }
        else
        {
            printf("dec type %d is undefined\n", u8DecodeType);
        }
    }
}
#endif // #if ENABLE_DMP

//*************************************************************************
/// get MVD current play status
/// @return
///   - # MSAPI_VID_PLAY, MSAPI_VID_PAUSE, MSAPI_VID_STOP, MSAPI_VID_RESET, MSAPI_VID_FREERUN
//*************************************************************************
U8 msAPI_VID_GetPlayMode(void)
{
    return u8PlayMode;
}

//*************************************************************************
/// get if MVD video sync is enable
/// @return
///   - # TRUE video-sync is enable
///   - # FALSE otherwise
//*************************************************************************
U8 msAPI_VID_IsSyncEnable()
{
    return u8SyncEnable;
}

//******************************************************************************
/// get video information from MPEG2 video header
/// @param pstVidStatus \b OUT information about progressive/interlace, resolution, framerate, aspect ratio
/// @return
///   - # TRUE video header has no problem
///   - # FALSE otherwise
//******************************************************************************
BOOLEAN msAPI_VID_GetVidInfo(VDEC_DispInfo *pstVidStatus)
{
    VDEC_DispInfo   info;
    VDEC_Result     ret;

//    SHOW_VIDEO_INFO(("msAPI_VID_GetVidInfo"));

    if(msAPI_VID_GetCodecType() == E_VDEC_CODEC_TYPE_MPEG2)
    {
        if (E_VDEC_FAIL == MApi_VDEC_IsWithValidStream()) /*--- check valid mpeg ---*/
        {
           return FALSE;
        }
    }

    memset(&info, 0, sizeof(VDEC_DispInfo));
    ret = MApi_VDEC_GetDispInfo(&info);

    if (E_VDEC_OK != ret)
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

        if(msAPI_VID_GetCodecType() == E_VDEC_CODEC_TYPE_MPEG2)
        {
            pstVidStatus->u8AFD         = info.u8AFD;
            pstVidStatus->u16Pitch      = info.u16Pitch;
            pstVidStatus->u16PTSInterval = info.u16PTSInterval;
            pstVidStatus->u8MPEG1       = info.u8MPEG1;
            pstVidStatus->u8PlayMode    = info.u8PlayMode;
            pstVidStatus->u8FrcMode     = info.u8FrcMode;

            if (pstVidStatus->u32FrameRate == 0
#if AUTO_ASPECT_RATIO
                ||
                pstVidStatus->u8AspectRate == FORBIDDEN ||
                pstVidStatus->u8AspectRate >= ASP_MAXNUM
#endif
                ||
                pstVidStatus->u16HorSize == 0 ||
                pstVidStatus->u16HorSize > 2000 ||
                pstVidStatus->u16VerSize == 0 ||
                pstVidStatus->u16VerSize > 1100)
            {
                return FALSE;
            }
//            pstVidStatus->u8AFD = msAPI_VID_GetActiveFormat();
            pstVidStatus->u8AFD = MApi_VDEC_GetActiveFormat();
            return TRUE;
        }
    }
    return TRUE;
}

VDEC_CodecType g_eCodecType;
void msAPI_VID_SetMappingAVCParameter(VDEC_CodecType eVDEC_CodecType)
{
    SHOW_VIDEO_INFO(("msAPI_VID_SetMappingAVCParameter eVDEC_CodecType=%d\n",eVDEC_CodecType));
    g_eCodecType = eVDEC_CodecType;

    msAPI_VID_SetCodecType(eVDEC_CodecType);
}

VDEC_CodecType geVDEC_CodecType;
void msAPI_VID_SetCodecType(VDEC_CodecType eVDEC_CodecType)
{
    geVDEC_CodecType = eVDEC_CodecType;
}

VDEC_CodecType msAPI_VID_GetCodecType(void)
{
    return geVDEC_CodecType;
}


////////////////////////////////////////////////////////////////////////////////
/// @brief
///         Initial Video codec
/// @par [In] eCodecType:input codec type
///         [In] bEnableDropErrFrame: Does enable drop error frame
/// @ret TRUE:success
///         FALSE: fail
////////////////////////////////////////////////////////////////////////////////
//void msAPI_VID_Init(BOOLEAN bEnableDropErrFrame)
//BOOLEAN MAPI_VID_Init(VDEC_CodecType eCodecType, BOOLEAN bEnableDropErrFrame)
BOOLEAN msAPI_VID_Init(BOOLEAN bEnableDropErrFrame)
{
    VDEC_InitParam initParam;
    VDEC_Result ret;

    SHOW_VIDEO_INFO(("msAPI_VID_Init bEnableDropErrFrame=%d codec=%d\n",bEnableDropErrFrame,msAPI_VID_GetCodecType()));

//    if((eCodecType != E_VDEC_CODEC_TYPE_MPEG2)&&(eCodecType != E_VDEC_CODEC_TYPE_H264))
//        SHOW_VIDEO_ERR(("MAPI_VID_Init not suuport this codec type(%d)",eCodecType))

    VDEC_Status vdecStatus;
    memset(&vdecStatus, 0, sizeof(VDEC_Status));
    MS_BOOL bRet = MApi_VDEC_GetStatus(&vdecStatus);
    if(bRet == TRUE && vdecStatus.bInit == TRUE) // msAPI_VID_Init run once only
        return TRUE;

    memset(&initParam, 0, sizeof(VDEC_InitParam));
    initParam.bDisableDropErrFrame = (!bEnableDropErrFrame);
    initParam.eCodecType = msAPI_VID_GetCodecType();
    initParam.VideoInfo.eSrcMode = E_VDEC_SRC_MODE_DTV;

#if (ENABLE_MIU_1 == ENABLE)
    printf("==== ENABLE_MIU_1 is ENABLE ====\n");
    initParam.SysConfig.u32CodeBufAddr = ((VDEC_AEON_MEMORY_TYPE & MIU1) ? (VDEC_AEON_ADR + MIU_INTERVAL) : (VDEC_AEON_ADR));
    initParam.SysConfig.u32CodeBufSize = VDEC_AEON_LEN;
    initParam.SysConfig.u32FrameBufAddr = ((VDEC_AEON_MEMORY_TYPE & MIU1) ? (VDEC_FRAME_BUF_ADR + MIU_INTERVAL) : (VDEC_FRAME_BUF_ADR));
    initParam.SysConfig.u32FrameBufSize = VDEC_FRAME_BUF_LEN;
    initParam.SysConfig.u32BitstreamBufAddr = ((VDEC_AEON_MEMORY_TYPE & MIU1) ? (VDEC_BIT_STREAM_ADR + MIU_INTERVAL) : (VDEC_BIT_STREAM_ADR));
    initParam.SysConfig.u32BitstreamBufSize = VDEC_BIT_STREAM_LEN;
#else
    initParam.SysConfig.u32CodeBufAddr = VDEC_AEON_ADR;
    initParam.SysConfig.u32CodeBufSize = VDEC_AEON_LEN;
    initParam.SysConfig.u32FrameBufAddr = VDEC_FRAME_BUF_ADR;
    initParam.SysConfig.u32FrameBufSize = VDEC_FRAME_BUF_LEN;
    initParam.SysConfig.u32BitstreamBufAddr = VDEC_BIT_STREAM_ADR;
    initParam.SysConfig.u32BitstreamBufSize = VDEC_BIT_STREAM_LEN;
#endif

#if 0
    printf("VDEC FW add = 0x%lx\n", initParam.SysConfig.u32CodeBufAddr);
    printf("VDEC FB add = 0x%lx\n", initParam.SysConfig.u32FrameBufAddr);
    printf("VDEC BS add = 0x%lx\n", initParam.SysConfig.u32BitstreamBufAddr);
#endif

    ret = MApi_VDEC_Init(&initParam);

    MDrv_MVOP_Init();
    MDrv_MVOP_Enable( TRUE );
//  MDrv_MVOP_SetClk( ENABLE );
    msAPI_VID_SetCodecType(msAPI_VID_GetCodecType());

    if (E_VDEC_OK != ret)
    {
        printf("msAPI_VID_Init: fail!\n");
        return FALSE;
    }
    else
    {
        return TRUE;
    }

}

//******************************************************************************
/// set MVOP output timing according to MVD/h.264 video frame information
/// @param pstVOPTiming \b IN VOP timing
/// @param pstVidStatus \b IN Video Status
//******************************************************************************
BOOLEAN msAPI_VID_SetOutput(VDEC_DispInfo *pstVidStatus)
{
    MVOP_VidStat stMvopVidSt;
    MVOP_InputSel tMvopInputSel;

    if ((msAPI_VID_GetCodecType() == E_VDEC_CODEC_TYPE_MPEG2) ||
        (msAPI_VID_GetCodecType() == E_VDEC_CODEC_TYPE_VC1_ADV))
    {
        tMvopInputSel = MVOP_INPUT_MVD;
    }
    else if(msAPI_VID_GetCodecType() == E_VDEC_CODEC_TYPE_H264)
    {
        tMvopInputSel = MVOP_INPUT_H264;
    }
    else if(msAPI_VID_GetCodecType() == E_VDEC_CODEC_TYPE_AVS)
    {
        tMvopInputSel = MVOP_INPUT_H264;
    }
    else
    {
        SHOW_VIDEO_ERR(("not support this codec type=%d",msAPI_VID_GetCodecType()));
        return FAIL;
    }
    MDrv_MVOP_SetInputCfg(tMvopInputSel, NULL);

    memset(&stMvopVidSt, 0, sizeof(MVOP_VidStat));
    stMvopVidSt.u16HorSize = pstVidStatus->u16HorSize;
    stMvopVidSt.u16VerSize = pstVidStatus->u16VerSize;
    stMvopVidSt.u16FrameRate = pstVidStatus->u32FrameRate;
    stMvopVidSt.u8AspectRate = pstVidStatus->u8AspectRate;
    stMvopVidSt.u8Interlace = pstVidStatus->u8Interlace;

#if STB_ENABLE
    //STB 504x do not support Hduplicate */
    MDrv_MVOP_SetOutputCfg(&stMvopVidSt, FALSE);
#else
    MDrv_MVOP_SetOutputCfg(&stMvopVidSt, TRUE);
#endif

#if 1
        (printf("->u32FrameRate = %lu\n", pstVidStatus->u32FrameRate));
        (printf("->u8Interlace = %x\n", pstVidStatus->u8Interlace));
        (printf("->u16HorSize = %u\n", pstVidStatus->u16HorSize));
        (printf("->u16VerSize = %u\n", pstVidStatus->u16VerSize));
        (printf("->u16Crop R/L = %u, %u\n", pstVidStatus->u16CropRight, pstVidStatus->u16CropLeft));
        (printf("->u16Crop B/T = %u, %u\n", pstVidStatus->u16CropBottom, pstVidStatus->u16CropTop));
        (printf("->u8AspectRate = %u\n", pstVidStatus->u8AspectRate));
        (printf("->u16SarWidth = %u\n", pstVidStatus->u16SarWidth));
        (printf("->u16SarHeight = %u\n", pstVidStatus->u16SarHeight));
#endif

    return PASS;
}

//******************************************************************************
/// AVCH264 is aspect ratio wide
/// @param pstVidStatus \b in information about pic
/// @param pstVidStatus \b out is wide
/// @return
///   - # TRUE success
///   - # FALSE error
//******************************************************************************
BOOLEAN msAPI_VID_AVCH264IsAspectRatioWide(VDEC_DispInfo *pstVidStatus,
                                           BOOLEAN *bResult)
{
    U32 u32AspRatioH, u32AspRatioW;

    if (pstVidStatus->u8AspectRate == 0xFF)  //extended_sar
    {
        u32AspRatioW = (U32) pstVidStatus->u16SarWidth;
        u32AspRatioH = (U32) pstVidStatus->u16SarHeight;
    }
    else if (pstVidStatus->u8AspectRate < 17)
    {
        u32AspRatioW = (U32)
                       _H264_ASPECT_RATIO[pstVidStatus->u8AspectRate][0];
        u32AspRatioH = (U32)
                       _H264_ASPECT_RATIO[pstVidStatus->u8AspectRate][1];
    }
    else
    {
        *bResult = FALSE;
        return FALSE;
    }
    if (pstVidStatus->u16HorSize * u32AspRatioW * 2 >
        pstVidStatus->u16VerSize * u32AspRatioH * 3)
    {
        *bResult = TRUE;
    }
    else
    {
        *bResult = FALSE;
    }
    return TRUE;
}

#undef MSAPI_VIDEO_C
