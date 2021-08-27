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
// (¡§MStar Confidential Information¡¨) by the recipient.
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
#include <string.h>
#include "MsCommon.h"
#include "MsMemory.h"
#include "apiDMX.h"
#include "apiVDEC.h"
#include "apiVDEC_EX.h"
#include "apiAUDIO.h"
#include "drvAUDIO.h"
#include "drvMVOP.h"
#include "drvGPIO.h"
//#include "ver_dtv_comm.h"

#include "appDemo_AV.h"
#include "appDemo_Video.h"
#include "appDemo_Audio.h"

#ifdef ENABLE_MM_PACKAGE
#include "appDemo_DMX.h"
#include "appDemo_MM.h"
#endif

#define DEMOAV_ENABLE_OSD       0

#define WAIT_TIMEOUT 5000

//-------------------------------------------------------------------------------------------------
// Global Variables
//-------------------------------------------------------------------------------------------------
MS_BOOL g_Demo_IsIframedecoding = FALSE;
extern MS_S32 gs32CachedPoolID;

//--------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Debug Macros
//------------------------------------------------------------------------------
#if ENABLE_DEBUG
#define db_print(fmt, args...)  printf("[%s][%d]" fmt, __FUNCTION__, __LINE__, ## args)
#else
#define db_print(fmt, args...)  while(0);
#endif

//--------------------------------------------------------------------------------------------------
// Macros
//--------------------------------------------------------------------------------------------------
#define INVALID_FILTER_ID                       0xFF
static MS_U32                   VDES_AVSYNCTIME      = 450000;
static MS_U8 u8PCRFltId = INVALID_FILTER_ID;
static MS_U8 u8VideoFltId = INVALID_FILTER_ID;
static MS_U8 u8AudioFltId = INVALID_FILTER_ID;
static MS_U32 _u32VCodec = 0;
static MS_U32 _u32ACodec = 0;
static MS_BOOL _bAudioInit = FALSE;
static MS_BOOL _bIsFileIn = FALSE;
static MS_U8 u8PVRPCRFltId = INVALID_FILTER_ID;
static MS_U8 u8PVRVideoFltId = INVALID_FILTER_ID;
static MS_U8 u8PVRAudioFltId = INVALID_FILTER_ID;
static Task_Info _audio_AudioMaster_Monitor_Task = {-1, E_TASK_PRI_HIGH, NULL, 4096, "Audio Monitor"};
static MS_U8 _u8AVSync_Mode = 0;    //0:av sync mode, 1:audio master mode
static codecPIDs nullProgramPIDs = {0,0,0,FALSE};

static MS_U8 u8Logo[] =
{
#include "bindata/BootLogo.dat"     //MPEG2
//#include "bindata/concert.dat"    //H264
};

// static MS_U8 u8PCRFltId = INVALID_FILTER_ID;

void DemoAV_Disp_SetSrcWin(MS_U16 *XStart, MS_U16 *YStart, MS_U16 *HSize, MS_U16 *VSize);
void DemoAV_Disp_SetDstWin(MS_U16 *XStart, MS_U16 *YStart, MS_U16 *HSize, MS_U16 *VSize);
MS_BOOL DemoAV_Tuner_Config(int* flow, int* pInSrc, int* pClkInv, int* pExtSync, int* pParal);

static void _DemoAV_ADEC_Play(En_DVB_decSystemType AudioCodecType);
static void _DemoAV_ADEC_Stop(void);
static En_DVB_decSystemType _DemoAV_ADEC_Fmt(MS_U32 u32ACodec);

typedef enum
{
    EN_ASPECT_RATIO_IGNORE = 0,
    EN_ASPECT_RATIO_169_ON_43_LB,
    EN_ASPECT_RATIO_169_ON_43_PS,
    EN_ASPECT_RATIO_43_ON_169_LB,
    EN_ASPECT_RATIO_43_ON_169_PS,
    EN_ASPECT_RATIO_NUM
} EN_ASPECT_RATIO;

//--------------------------------------------------------------------------------------------------
// Global Variables
//--------------------------------------------------------------------------------------------------



//--------------------------------------------------------------------------------------------------
// Local Variables
//--------------------------------------------------------------------------------------------------
// static MS_U8*    pu8PreviewBuf= NULL;
// static MS_BOOL   bPlayCard = FALSE;
// static EN_ASPECT_RATIO eAspectRatio = EN_ASPECT_RATIO_IGNORE;
// static MS_U16    u16SrcWinXOffset = 0;
// static MS_U16    u16SrcWinYOffset = 0;
// static MS_U16    u16SrcWinWidth = 720;
// static MS_U16    u16SrcWinHeight = 576;

//------------------------------------------------------------------------------
/// Free GC buffer and resource occupied by preview window.
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/// Allocate GC buffer and resource for preview window.
/// @param hsize \b IN: Horizontal size of preview window
/// @param vsize \b IN: Vertical size of preview window
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// Enable showing preview window
/// @param hsize \b IN: Horizontal size of preview window
/// @param vsize \b IN: Vertical size of preview window
//------------------------------------------------------------------------------


#ifdef ENABLE_PVR_PACKAGE
MS_BOOL DemoAV_TSP_SetPVRAudioPid(MS_U32* AudioPid, MS_BOOL bEsPlayer)
{
    //printf("[%s][%d]u8AudioFltId=%d\n", __FUNCTION__, __LINE__,u8AudioFltId);
    //printf("[%s][%d]AudioPid=%ld\n", __FUNCTION__, __LINE__,*AudioPid);
    //printf("_u32ACodec=%ld\n",_u32ACodec);
    if(bEsPlayer)
    {
        return TRUE;
    }

    if (u8PVRAudioFltId != INVALID_FILTER_ID)
    {
        return TRUE;
    }
    if (DMX_FILTER_STATUS_OK != MApi_DMX_Open(DMX_FILTER_TYPE_AUDIO | DMX_FILTER_SOURCE_TYPE_FILE, &u8PVRAudioFltId))
    {
        goto DemoAV_TSP_SetPVRAudioPid_fail;
    }

    if (DMX_FILTER_STATUS_OK != MApi_DMX_Pid(u8PVRAudioFltId, (MS_U16*)AudioPid, TRUE))
    {
        goto DemoAV_TSP_SetPVRAudioPid_fail;
    }

    if (DMX_FILTER_STATUS_OK != MApi_DMX_Start(u8PVRAudioFltId))
    {
        goto DemoAV_TSP_SetPVRAudioPid_fail;
    }

    return TRUE;
DemoAV_TSP_SetPVRAudioPid_fail:
    printf("[%s][%d] fail\n", __FUNCTION__, __LINE__);

    if (u8PVRAudioFltId != INVALID_FILTER_ID)
    {
        MApi_DMX_Stop(u8PVRAudioFltId);
        MApi_DMX_Close(u8PVRAudioFltId);
        u8PVRAudioFltId = INVALID_FILTER_ID;
    }
    return FALSE;

}

MS_BOOL DemoAV_TSP_ClosePVRAudioPid(const MS_U32* AudioPid, MS_BOOL bEsPlayer)
{

    if(bEsPlayer)
    {
        return TRUE;
    }
    if (u8PVRAudioFltId != INVALID_FILTER_ID)
    {
        MApi_DMX_Stop(u8PVRAudioFltId);
        MApi_DMX_Close(u8PVRAudioFltId);
        u8PVRAudioFltId = INVALID_FILTER_ID;
    }
    return TRUE;

}

MS_BOOL DemoAV_TSP_StopPVRFlt(MS_BOOL bStop)
{
    if(bStop)
    {
            if (u8PCRFltId != INVALID_FILTER_ID)
            {
                MApi_DMX_Stop(u8PCRFltId);
            }

            if (u8VideoFltId != INVALID_FILTER_ID)
            {
                MApi_DMX_AVFifo_Reset(DMX_FILTER_TYPE_VIDEO,TRUE);
                MApi_DMX_Stop(u8VideoFltId);
            }

            if (u8AudioFltId != INVALID_FILTER_ID)
            {
                MApi_DMX_AVFifo_Reset(DMX_FILTER_TYPE_AUDIO,TRUE);
                MApi_DMX_Stop(u8AudioFltId);
            }

            return TRUE;

    }
    else
    {
            if (u8PCRFltId != INVALID_FILTER_ID)
            {
                MApi_DMX_Start(u8PCRFltId);
            }

            if (u8VideoFltId != INVALID_FILTER_ID)
            {
                MApi_DMX_Start(u8VideoFltId);
            }

            if (u8AudioFltId != INVALID_FILTER_ID)
            {
                MApi_DMX_Start(u8AudioFltId);
            }
            return TRUE;
    }

    return FALSE;
}

MS_BOOL DualAV_StopPCRFLT(void)
{
    printf("[%s][%d]\n", __FUNCTION__, __LINE__);
    if (DMX_FILTER_STATUS_OK != MApi_DMX_Stop(u8PVRPCRFltId))
    {
        printf("[%s][%d]DualAV Stop PCR Pid fail\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    if (DMX_FILTER_STATUS_OK != MApi_DMX_Close(u8PVRPCRFltId))
    {
        printf("[%s][%d]DualAV Close PCR Pid fail\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    return TRUE;
}


MS_BOOL DualAV_StartPCRFLT(MS_U16 *PCRPid)
{

    printf("[%s][%d]\n", __FUNCTION__, __LINE__);
    if (DMX_FILTER_STATUS_OK != MApi_DMX_Open(DMX_FILTER_TYPE_PCR | DMX_FILTER_SOURCE_TYPE_FILE, &u8PVRPCRFltId))
    {
        printf("[%s][%d]DualAV Start PCR Pid fail\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    if (DMX_FILTER_STATUS_OK != MApi_DMX_Pid(u8PVRPCRFltId, (MS_U16*)PCRPid, TRUE))
    {
        printf("[%s][%d]DualAV Start PCR Pid fail\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    if (DMX_FILTER_STATUS_OK != MApi_DMX_Start(u8PVRPCRFltId))
    {
        printf("[%s][%d]DualAV Start PCR Pid fail\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    return TRUE;
}

#endif  /* ENABLE_PVR_PACKAGE */

MS_BOOL DemoAV_TSP_ResetPVRPid(void)
{
    if (u8PVRPCRFltId != INVALID_FILTER_ID)
    {
        MApi_DMX_Stop(u8PVRPCRFltId);
        MApi_DMX_Close(u8PVRPCRFltId);
        u8PVRPCRFltId=INVALID_FILTER_ID;
    }

    if (u8PVRVideoFltId != INVALID_FILTER_ID)
    {
        MApi_DMX_AVFifo_Reset(DMX_FILTER_TYPE_VIDEO,TRUE);
        MApi_DMX_Stop(u8PVRVideoFltId);
        MApi_DMX_Close(u8PVRVideoFltId);
        u8PVRVideoFltId=INVALID_FILTER_ID;
    }

    if (u8PVRAudioFltId != INVALID_FILTER_ID)
    {
        MApi_DMX_AVFifo_Reset(DMX_FILTER_TYPE_AUDIO,TRUE);
        MApi_DMX_Stop(u8PVRAudioFltId);
        MApi_DMX_Close(u8PVRAudioFltId);
        u8PVRAudioFltId=INVALID_FILTER_ID;
    }
    return TRUE;
}


MS_BOOL DemoAV_TSP_ResetDTVPid(void)
{
    if (u8PCRFltId != INVALID_FILTER_ID)
    {
        MApi_DMX_Stop(u8PCRFltId);
        MApi_DMX_Close(u8PCRFltId);
        u8PCRFltId = INVALID_FILTER_ID;
    }

    if (u8VideoFltId != INVALID_FILTER_ID)
    {
        MApi_DMX_AVFifo_Reset(DMX_FILTER_TYPE_VIDEO,TRUE);
        MApi_DMX_Stop(u8VideoFltId);
        MApi_DMX_Close(u8VideoFltId);
        u8VideoFltId = INVALID_FILTER_ID;
    }

    if (u8AudioFltId != INVALID_FILTER_ID)
    {
        MApi_DMX_AVFifo_Reset(DMX_FILTER_TYPE_AUDIO,TRUE);
        MApi_DMX_Stop(u8AudioFltId);
        MApi_DMX_Close(u8AudioFltId);
        u8AudioFltId = INVALID_FILTER_ID;
    }
    return TRUE;
}
//------------------------------------------------------------------------------
/// @brief The sample code to set video and audio pid to playback
/// @param[in] VideoPid Video Pid
/// @param[in] AudioPid Audio Pid
/// @param[in] PCRPid PCR Pid
/// @param[in] pu32VCodec Video codec type.
///        0: MPEG2
///        1: H264
/// @param[in] pu32ACodec Audio Codec
///        0x01: MPEG
///        0x02: AC3
///        0x03: AC3P
///        0x04: AC3P
///        others: NONE
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b TV_TSP_SetPid_File 1249 1248 1249 0 1 \n
//------------------------------------------------------------------------------

MS_BOOL DemoAV_TSP_SetPVRPid(MS_U32* VideoPid, MS_U32* AudioPid, MS_U32* PCRPid, MS_U32* pu32VCodec, MS_U32* pu32ACodec)
{
    VDEC_StreamId *stVDECStreamId = NULL;
    stVDECStreamId = appDemo_Video_GetStreamID();


#ifdef ENABLE_CI_PACKAGE
    MS_BOOL st;
#endif

    _bIsFileIn = TRUE;
#ifdef ENABLE_MM_PACKAGE
    if(appDemo_MM_IsInit())
    {
        printf("Fail due to not MM_INIT\n");
        return FALSE;
    }
#endif

    if (u8PVRPCRFltId != INVALID_FILTER_ID)
    {
        MApi_DMX_Stop(u8PVRPCRFltId);
        MApi_DMX_Close(u8PVRPCRFltId);
        u8PVRPCRFltId = INVALID_FILTER_ID;
    }

    if (u8PVRVideoFltId != INVALID_FILTER_ID)
    {
        MApi_DMX_AVFifo_Reset(DMX_FILTER_TYPE_VIDEO,TRUE);
        MApi_DMX_Stop(u8PVRVideoFltId);
        MApi_DMX_Close(u8PVRVideoFltId);
        u8PVRVideoFltId = INVALID_FILTER_ID;
    }

    if (u8PVRAudioFltId != INVALID_FILTER_ID)
    {
        MApi_DMX_AVFifo_Reset(DMX_FILTER_TYPE_AUDIO,TRUE);
        MApi_DMX_Stop(u8PVRAudioFltId);
        MApi_DMX_Close(u8PVRAudioFltId);
        u8PVRAudioFltId = INVALID_FILTER_ID;
    }
#ifdef ENABLE_PVR_PACKAGE
    if(MDrv_AUDIO_GET_INIT_FLAG())
    {
        MApi_AUDIO_SetCommand(MSAPI_AUD_DVB_DECCMD_STOP);
    }
    MApi_VDEC_EX_Exit(stVDECStreamId);
#endif

    if (DMX_FILTER_STATUS_OK != MApi_DMX_Open(DMX_FILTER_TYPE_PCR | DMX_FILTER_SOURCE_TYPE_FILE, &u8PVRPCRFltId))
    {
        goto DemoAV_TSP_SetPid_fail;
    }
    if (DMX_FILTER_STATUS_OK != MApi_DMX_Pid(u8PVRPCRFltId, (MS_U16*)PCRPid, TRUE))
    {
        goto DemoAV_TSP_SetPid_fail;
    }
    if (DMX_FILTER_STATUS_OK != MApi_DMX_Start(u8PVRPCRFltId))
    {
        goto DemoAV_TSP_SetPid_fail;
    }


    if (DMX_FILTER_STATUS_OK != MApi_DMX_Open(DMX_FILTER_TYPE_VIDEO | DMX_FILTER_SOURCE_TYPE_FILE, &u8PVRVideoFltId))
    {
        goto DemoAV_TSP_SetPid_fail;
    }
    if (DMX_FILTER_STATUS_OK != MApi_DMX_Pid(u8PVRVideoFltId, (MS_U16*)VideoPid, TRUE))
    {
        goto DemoAV_TSP_SetPid_fail;
    }
    if (DMX_FILTER_STATUS_OK != MApi_DMX_Start(u8PVRVideoFltId))
    {
        goto DemoAV_TSP_SetPid_fail;
    }



    if (DMX_FILTER_STATUS_OK != MApi_DMX_Open(DMX_FILTER_TYPE_AUDIO | DMX_FILTER_SOURCE_TYPE_FILE, &u8PVRAudioFltId))
    {
        goto DemoAV_TSP_SetPid_fail;
    }

    if (DMX_FILTER_STATUS_OK != MApi_DMX_Pid(u8PVRAudioFltId, (MS_U16*)AudioPid, TRUE))
    {
        goto DemoAV_TSP_SetPid_fail;
    }

    if (DMX_FILTER_STATUS_OK != MApi_DMX_Start(u8PVRAudioFltId))
    {
        goto DemoAV_TSP_SetPid_fail;
    }
#ifdef ENABLE_CI_PACKAGE

    st = msAPP_CI_setCCFID(u8PVRVideoFltId, TRUE);
    printf("[CI+] Set FID: %d,%d.\r\n", st, u8PVRVideoFltId);
    st = msAPP_CI_setCCFID(u8PVRAudioFltId, TRUE);
    printf("[CI+] Set FID: %d,%d.\r\n", st, u8PVRAudioFltId);
    st = msAPP_CI_setCCFID(u8PVRPCRFltId, TRUE);
    printf("[CI+] Set FID: %d,%d.\r\n", st, u8PVRPCRFltId);
#endif

    _u32VCodec = *pu32VCodec;
    _u32ACodec = *pu32ACodec;
    return TRUE;
DemoAV_TSP_SetPid_fail:
    printf("[%s][%d] fail\n", __FUNCTION__, __LINE__);
    if (u8PVRVideoFltId != INVALID_FILTER_ID)
    {
        MApi_DMX_Stop(u8PVRVideoFltId);
        MApi_DMX_Close(u8PVRVideoFltId);
        u8PVRVideoFltId = INVALID_FILTER_ID;
    }
    if (u8PVRAudioFltId != INVALID_FILTER_ID)
    {
        MApi_DMX_Stop(u8PVRAudioFltId);
        MApi_DMX_Close(u8PVRAudioFltId);
        u8PVRAudioFltId = INVALID_FILTER_ID;
    }
    return FALSE;
}




MS_BOOL DemoAV_TSP_StopDmxFlt(void)
{
    if (u8PCRFltId != INVALID_FILTER_ID)
    {
        MApi_DMX_Stop(u8PCRFltId);
        MApi_DMX_Close(u8PCRFltId);
        u8PCRFltId = INVALID_FILTER_ID;
    }

    if (u8VideoFltId != INVALID_FILTER_ID)
    {
        MApi_DMX_Stop(u8VideoFltId);
        MApi_DMX_Close(u8VideoFltId);
        u8VideoFltId = INVALID_FILTER_ID;
    }

    if (u8AudioFltId != INVALID_FILTER_ID)
    {
        MApi_DMX_Stop(u8AudioFltId);
        MApi_DMX_Close(u8AudioFltId);
        u8AudioFltId = INVALID_FILTER_ID;
    }
    MApi_DMX_AVFifo_Reset(DMX_FILTER_TYPE_VIDEO, FALSE);
    MApi_DMX_AVFifo_Reset(DMX_FILTER_TYPE_AUDIO, FALSE);
    MApi_DMX_AVFifo_Reset(DMX_FILTER_TYPE_AUDIO2, FALSE);

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to get LIVE TV Tuner Index
/// @return Tuner Index
/// @sa
/// @note
/// Command: \b DigiTuner_GetIndex \n
//------------------------------------------------------------------------------
pCodecPIDs DemoAV_TSP_GetLiveTVPIDs(void)
{
    if(pliveProgramPIDs == NULL)
    {
        return &nullProgramPIDs;
    }

    return  pliveProgramPIDs;
}


//------------------------------------------------------------------------------
/// @brief The sample code to set video and audio pid to TSP
/// @param[in] VideoPid Video Pid
/// @param[in] AudioPid Audio Pid
/// @param[in] PCRPid PCR Pid
/// @param[in] pu32VCodec Video codec type.
///        0: MPEG2
///        1: H264
///        2: AVS
/// @param[in] pu32ACodec Audio Codec
///        0x01: MPEG
///        0x02: AC3
///        0x03: AC3P
///        0x04: AC3P
///        others: NONE
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b TV_TSP_SetPid 1249 1248 1249 0 1 \n
//------------------------------------------------------------------------------
MS_BOOL DemoAV_TSP_SetPid(MS_U32* VideoPid, MS_U32* AudioPid, MS_U32* PCRPid, MS_U32* pu32VCodec, MS_U32* pu32ACodec)
{
    VDEC_StreamId *stVDECStreamId = NULL;
    stVDECStreamId = appDemo_Video_GetStreamID();

    _bIsFileIn = FALSE;

#ifdef ENABLE_MM_PACKAGE
    if(appDemo_MM_IsInit())
    {
        printf("Fail due to not MM_INIT\n");
        return FALSE;
    }
#endif
    if (u8PCRFltId != INVALID_FILTER_ID)
    {
        MApi_DMX_Stop(u8PCRFltId);
        MApi_DMX_Close(u8PCRFltId);
        u8PCRFltId = INVALID_FILTER_ID;
    }

    if (u8VideoFltId != INVALID_FILTER_ID)
    {
        MApi_DMX_AVFifo_Reset(DMX_FILTER_TYPE_VIDEO,TRUE);
        MApi_DMX_Stop(u8VideoFltId);
        MApi_DMX_Close(u8VideoFltId);
        u8VideoFltId = INVALID_FILTER_ID;
    }

    if (u8AudioFltId != INVALID_FILTER_ID)
    {
        MApi_DMX_AVFifo_Reset(DMX_FILTER_TYPE_AUDIO,TRUE);
        MApi_DMX_Stop(u8AudioFltId);
        MApi_DMX_Close(u8AudioFltId);
        u8AudioFltId = INVALID_FILTER_ID;
    }

    //get live TV program info
    liveProgramPIDs.u32AudioPid = *AudioPid;
    liveProgramPIDs.u32VideoPid = *VideoPid;
    liveProgramPIDs.u32PCRPid = *PCRPid;
    liveProgramPIDs.u32VCodec = *pu32VCodec;
    liveProgramPIDs.u32ACodec = *pu32ACodec;
    liveProgramPIDs.bPlayLiveTV = TRUE;
    pliveProgramPIDs = &liveProgramPIDs;


#ifdef ENABLE_PVR_PACKAGE
    if(MDrv_AUDIO_GET_INIT_FLAG())
    {
        MApi_AUDIO_SetCommand(MSAPI_AUD_DVB_DECCMD_STOP);
    }
    MApi_VDEC_EX_Exit(stVDECStreamId);
#endif

    if(_u8AVSync_Mode != 1)
    {
        if (DMX_FILTER_STATUS_OK != MApi_DMX_Open(DMX_FILTER_TYPE_PCR | DMX_FILTER_SOURCE_TYPE_LIVE , &u8PCRFltId))
        {
            goto DemoAV_TSP_SetPid_fail;
        }
        if (DMX_FILTER_STATUS_OK != MApi_DMX_Pid(u8PCRFltId, (MS_U16*)PCRPid, TRUE))
        {
            goto DemoAV_TSP_SetPid_fail;
        }
        if (DMX_FILTER_STATUS_OK != MApi_DMX_Start(u8PCRFltId))
        {
            goto DemoAV_TSP_SetPid_fail;
        }
    }

    if (DMX_FILTER_STATUS_OK != MApi_DMX_Open(DMX_FILTER_TYPE_VIDEO | DMX_FILTER_SOURCE_TYPE_LIVE , &u8VideoFltId))
    {
        goto DemoAV_TSP_SetPid_fail;
    }
    if (DMX_FILTER_STATUS_OK != MApi_DMX_Pid(u8VideoFltId, (MS_U16*)VideoPid, TRUE))
    {
        goto DemoAV_TSP_SetPid_fail;
    }
    if (DMX_FILTER_STATUS_OK != MApi_DMX_Start(u8VideoFltId))
    {
        goto DemoAV_TSP_SetPid_fail;
    }

    if (DMX_FILTER_STATUS_OK != MApi_DMX_Open(DMX_FILTER_TYPE_AUDIO | DMX_FILTER_SOURCE_TYPE_LIVE , &u8AudioFltId))
    {
        goto DemoAV_TSP_SetPid_fail;
    }
    if (DMX_FILTER_STATUS_OK != MApi_DMX_Pid(u8AudioFltId, (MS_U16*)AudioPid, TRUE))
    {
        goto DemoAV_TSP_SetPid_fail;
    }
    if (DMX_FILTER_STATUS_OK != MApi_DMX_Start(u8AudioFltId))
    {
        goto DemoAV_TSP_SetPid_fail;
    }

    _u32VCodec = *pu32VCodec;
    _u32ACodec = *pu32ACodec;
    return TRUE;
DemoAV_TSP_SetPid_fail:
    printf("[%s][%d] fail\n", __FUNCTION__, __LINE__);
    if (u8VideoFltId != INVALID_FILTER_ID)
    {
        MApi_DMX_Stop(u8VideoFltId);
        MApi_DMX_Close(u8VideoFltId);
        u8VideoFltId = INVALID_FILTER_ID;
    }
    if (u8AudioFltId != INVALID_FILTER_ID)
    {
        MApi_DMX_Stop(u8AudioFltId);
        MApi_DMX_Close(u8AudioFltId);
        u8AudioFltId = INVALID_FILTER_ID;
    }
    return FALSE;
}

MS_BOOL DemoAV_TSP_SetVideoDmxFltRushPass(MS_U8 u8Enable)
{
    if (u8VideoFltId != INVALID_FILTER_ID)
        MApi_DMX_FQ_SetFltRushPass(u8VideoFltId, u8Enable);
    return TRUE;
}

MS_BOOL DemoAV_TSP_GetLiveTVPIDFlts(MS_U8* u8LiveVideoFltId, MS_U8* u8LiveAudioFltId, MS_U8* u8LivePCRFltId)
{
    if((u8VideoFltId!=INVALID_FILTER_ID) && (u8AudioFltId!=INVALID_FILTER_ID)&& (u8PCRFltId!=INVALID_FILTER_ID))
    {
        *u8LiveVideoFltId = u8VideoFltId;
        *u8LiveAudioFltId = u8AudioFltId;
        *u8LivePCRFltId = u8PCRFltId;
        return TRUE;
    }
    else
    {
        return FALSE;
    }

}


//------------------------------------------------------------------------------
/// @brief Set Tuner configuration
/// @param[in] flow DMX flow
///        0: DMX_FLOW_PLAYBACK
///        1: DMX_FLOW_PVR
///        2: DMX_FLOW_PVR1
/// @param[in] pInsrc DMX flow input
///        0: DMX_FLOW_INPUT_DEMOD
///        1: DMX_FLOW_INPUT_MEM
///        2: DMX_FLOW_INPUT_EXT_INPUT0
///        3: DMX_FLOW_INPUT_EXT_INPUT1
///        4: DMX_FLOW_INPUT_EXT_INPUT2
/// @param[in] pClkInv TSin options: clock phase inversion
///        0: phases of these clocks are non-inverted
///        1: clock phase inversion
/// @param[in] pExtSync TSin options: sync by external signal
///        0: sync by internal signal
///        1: sync by external signal
/// @param[in] pParal TSin is parallel interface or serial interface
///        0: serial interface
///        1: parallel interface
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b  \n
//------------------------------------------------------------------------------
MS_BOOL DemoAV_Tuner_Config(int* flow, int* pInSrc, int* pClkInv, int* pExtSync, int* pParal)
{
    DMX_FLOW dmxFlow = DMX_FLOW_NUM;
    DMX_FLOW_INPUT inSrc = DMX_FLOW_INPUT_DISABLE;
    MS_BOOL bClkInv, bExtSync, bParallel;

    switch (*flow)
    {
    case 0:
        dmxFlow = DMX_FLOW_PLAYBACK;
        break;
    case 1:
        dmxFlow = DMX_FLOW_PVR;
        break;
    case 2:
        dmxFlow = DMX_FLOW_PVR1;
        break;
    default:
        printf("[%s][%d] unsupport flow %d\n", __FUNCTION__, __LINE__, *flow);
        break;
    }
    switch (*pInSrc)
    {
    case 0:
        inSrc = DMX_FLOW_INPUT_DEMOD;
        break;
    case 1:
        inSrc = DMX_FLOW_INPUT_MEM;
        break;
    case 2:
        inSrc = DMX_FLOW_INPUT_EXT_INPUT0;
        break;
    case 3:
        inSrc = DMX_FLOW_INPUT_EXT_INPUT1;
        break;
#ifdef TS2_IF_SUPPORT
    case 4:
        inSrc = DMX_FLOW_INPUT_EXT_INPUT2;
        break;
#endif
    default:
        printf("[%s][%d] unsupport input source  %d\n", __FUNCTION__, __LINE__, *pInSrc);
        break;
    }
    bClkInv = (0 == *pClkInv)? FALSE: TRUE;
    bExtSync = (0 == *pExtSync)? FALSE: TRUE;
    bParallel = (0 == *pParal)? FALSE: TRUE;
    if (DMX_FILTER_STATUS_OK != MApi_DMX_FlowSet(dmxFlow, inSrc, bClkInv, bExtSync, bParallel))
    {
        printf("[%s][%d] MApi_DMX_FlowSet fail\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to get video resolution
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b Command:TV_MVD_GetVideoInfo \n
//------------------------------------------------------------------------------
MS_BOOL DemoAV_MVD_GetVideoInfo(void)
{
    VDEC_EX_DispInfo PicData;
    VDEC_StreamId *stVDECStreamId = NULL;
    stVDECStreamId = appDemo_Video_GetStreamID();
    // MS_U32 u32FrmCnt = 0;

    // u32FrmCnt = MApi_VDEC_GetFrameCnt();
    if ((MApi_VDEC_EX_GetDispInfo(stVDECStreamId,&PicData) != E_VDEC_EX_OK) || (PicData.u16HorSize==0 || PicData.u16VerSize==0))
    {
        printf("[%s][%d] Get video information failed\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    // printf("[%s][%d] Frame count = %d\n", __FUNCTION__, __LINE__, (int)u32FrmCnt);
    printf("[%s][%d] (width, height, frame_rate, interlace)= (%8u %8u %8lu %8u)\n",
        __FUNCTION__, __LINE__,
        PicData.u16HorSize,
        PicData.u16VerSize,
        PicData.u32FrameRate,
        PicData.u8Interlace);

    return TRUE;
}



static MS_BOOL _DemoAV_VDEC_Init(int codec)
{
    VDEC_EX_Status vdecStatus;
    VDEC_EX_InitParam initParam;
    VDEC_EX_CodecType codecType;
    VDEC_EX_CodecType codecTypeCur;
    VDEC_StreamId *stVDECStreamId = NULL;
    stVDECStreamId = appDemo_Video_GetStreamID();


    //Reset/Clear VDEC
    {
        MApi_VDEC_EX_Exit(stVDECStreamId);
        g_Demo_IsIframedecoding = FALSE;
    }

    switch (codec)
    {
        case E_DDI_VDEC_CODEC_TYPE_MPEG2:
        case E_DDI_VDEC_CODEC_TYPE_MPEG2_IFRAME:
        {
            codecType = E_VDEC_EX_CODEC_TYPE_MPEG2;
            break;
        }
        case E_DDI_VDEC_CODEC_TYPE_H264:
        case E_DDI_VDEC_CODEC_TYPE_H264_IFRAME:
        {
            codecType = E_VDEC_EX_CODEC_TYPE_H264;
            break;
        }
        case E_DDI_VDEC_CODEC_TYPE_AVS:
        {
            codecType = E_VDEC_EX_CODEC_TYPE_AVS;
            break;
        }
        default:
        {
            printf("Invalid Codec Type %d\n",codec);
            return FALSE;
        }
    }

    memset(&vdecStatus, 0, sizeof(VDEC_EX_Status));
    codecTypeCur = MApi_VDEC_EX_GetActiveCodecType(stVDECStreamId);
    MApi_VDEC_EX_GetStatus(stVDECStreamId,&vdecStatus);
    if ((TRUE == vdecStatus.bInit) && (codecTypeCur == codecType))
    {
        return TRUE;
    }

    memset(&initParam, 0, sizeof(VDEC_EX_InitParam));
    initParam.bDisableDropErrFrame = TRUE;
    initParam.eCodecType = codecType;
    /* TODO START: H264.IFrame */
    //Temp Solution for I-Frame decoder on H264
    if(codec == E_DDI_VDEC_CODEC_TYPE_H264_IFRAME)
    {
        initParam.VideoInfo.eSrcMode = E_VDEC_EX_SRC_MODE_FILE;
    }
    else
    {
        initParam.VideoInfo.eSrcMode = E_VDEC_EX_SRC_MODE_DTV;
    }

    if(_bIsFileIn)
    {
        initParam.VideoInfo.eSrcMode = E_VDEC_EX_SRC_MODE_TS_FILE;
    }
    /* TODO END: H264.IFrame */
    initParam.SysConfig.u32CodeBufAddr = VDEC_AEON_ADR ;
    initParam.SysConfig.u32CodeBufSize = VDEC_AEON_LEN;
    initParam.SysConfig.u32FrameBufAddr = VDEC_FRAME_BUF_ADR ;
    initParam.SysConfig.u32FrameBufSize = VDEC_FRAME_BUF_LEN;
    initParam.SysConfig.u32BitstreamBufAddr = VDEC_BIT_STREAM_ADR ;
    initParam.SysConfig.u32BitstreamBufSize = VDEC_BIT_STREAM_LEN;
    initParam.SysConfig.u32DrvProcBufAddr = VDEC_BIT_STREAM_ADR ;
    initParam.SysConfig.eDbgMsgLevel = E_VDEC_EX_DBG_LEVEL_NONE;

    printf("VDEC FW add = 0x%lx\n", initParam.SysConfig.u32CodeBufAddr);
    printf("VDEC FB add = 0x%lx\n", initParam.SysConfig.u32FrameBufAddr);
    printf("VDEC BS add = 0x%lx\n", initParam.SysConfig.u32BitstreamBufAddr);

    if (E_VDEC_EX_OK != MApi_VDEC_EX_GetFreeStream(stVDECStreamId, sizeof(VDEC_StreamId), E_VDEC_EX_MAIN_STREAM, (VDEC_EX_CodecType)initParam.eCodecType))
    {
        printf(" [%d]....GetFreeStream Failed\n", __LINE__);
        return FALSE;
    }
    else
    {
        printf("_DemoAV_VDEC_Init - stVDECStreamId:  0x%lx, Version 0x%lx \n",stVDECStreamId->u32Id, stVDECStreamId->u32Version);
    }

    //Init/Re-Init VDEC
    if (E_VDEC_EX_OK != MApi_VDEC_EX_Init(stVDECStreamId,&initParam))
    {
        printf("*******************************************************\n");
        printf("MApi_VDEC_EX_Init  return fail!!!\n");
        printf("*******************************************************\n");
        return FALSE;
    }

    //Move MVOP Setting to XC Demo code
    //MDrv_MVOP_Init();
    //MDrv_MVOP_Enable(TRUE);

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to decode ONE I-frame only
/// @param[in] u32FrameBufAddr  to get destination buffer address
/// @param[in] u32StreamBufAddr to get source elementary stream buffer start address
/// @param[in] u32StreamBufEnd to get source elementary stream buffer end address
/// @param[in] pu32VCodec Video codec type. 0 is for MPEG2. 1 is for H264
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b Command:TV_MVD_DecIFrame \n
//------------------------------------------------------------------------------
MS_BOOL DemoAV_MVD_DecIFrame(MS_U32* u32FrameBufAddr, MS_U32* u32StreamBufAddr, MS_U32* u32StreamBufEnd, MS_U32* pu32VCodec)
{
    MS_U8* pVirAddr = NULL;
    VDEC_StreamId *stVDECStreamId = NULL;
    stVDECStreamId = appDemo_Video_GetStreamID();


    VDEC_EX_Result VR = 0;
    if(*pu32VCodec == E_DDI_VDEC_CODEC_TYPE_MPEG2)
    {
        *pu32VCodec = E_DDI_VDEC_CODEC_TYPE_MPEG2_IFRAME;
    }
    else
    {
        *pu32VCodec = E_DDI_VDEC_CODEC_TYPE_H264_IFRAME;
    }

    if (TRUE != _DemoAV_VDEC_Init(*pu32VCodec)) // default mepg2
    {
        printf("[%s][%d] _DemoAV_VDEC_Init fail\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    if ( (u32FrameBufAddr == NULL) || (u32StreamBufAddr == NULL) || (u32StreamBufEnd == NULL) )
    {
        printf("[%s][%d] invalid parameter.\n", __FUNCTION__, __LINE__);
        return FALSE;
    }


    *u32FrameBufAddr = VDEC_FRAME_BUF_ADR;
    *u32StreamBufAddr = (MS_U32)VDEC_BIT_STREAM_ADR;
    *u32StreamBufEnd = (MS_U32)VDEC_BIT_STREAM_ADR + sizeof(u8Logo);

    pVirAddr = (MS_U8*)MsOS_PA2KSEG1(*u32StreamBufAddr);
    memcpy(pVirAddr, u8Logo, sizeof(u8Logo));
    MsOS_FlushMemory();
    MsOS_ReadMemory();

    VR = MApi_VDEC_EX_MHEG_DecodeIFrame(stVDECStreamId,*u32FrameBufAddr, *u32StreamBufAddr, *u32StreamBufEnd);
    if (VR!= E_VDEC_EX_OK)
    {
        db_print("Decode I-frame failed! %d\n", VR);
        return FALSE;
    }
    else
    {
        db_print("*******************************************************\n");
        db_print("MApi_VDEC_EX_MHEG_DecodeIFrame good\n");
        db_print("*******************************************************\n");

        g_Demo_IsIframedecoding = TRUE;
    }

    printf("I-frame size = ");
    DemoAV_MVD_GetVideoInfo();
    return TRUE;
}

///////////////////////// Audio part
static MS_BOOL _DemoAV_ADEC_Init(void)
{
    AUDIO_INIT_INFO SystemInfo;
    AUDIO_OUT_INFO OutputInfo;
    AUDIO_PATH_INFO PathInfo;
    if (TRUE == _bAudioInit)
    {
        return TRUE;
    }
    SystemInfo.tv_system=TV_PAL;
    SystemInfo.au_system_sel=AUDIO_SYSTEM_A2;
    SystemInfo.dtv_system=0;
    MApi_AUDIO_SetSystemInfo(&SystemInfo);

    OutputInfo.SpeakerOut=AUDIO_OUTPUT_MAIN_SPEAKER;
    OutputInfo.HpOut=AUDIO_OUTPUT_HP;
    OutputInfo.ScartOut=AUDIO_OUTPUT_SIFOUT;
    OutputInfo.MonitorOut=AUDIO_OUTPUT_LINEOUT;
    MApi_AUDIO_SetOutputInfo(&OutputInfo);

    PathInfo.SpeakerOut = AUDIO_PATH_MAIN;
    PathInfo.HpOut = 0;
    PathInfo.MonitorOut = AUDIO_PATH_1;
    PathInfo.SpdifOut = AUDIO_PATH_2;
    PathInfo.ScartOut = AUDIO_PATH_1;
    MApi_AUDIO_SetPathInfo(&PathInfo);

    MDrv_AUDIO_SetDspBaseAddr(DSP_DEC, 0, MAD_DEC_BUF_ADR);
    MApi_AUDIO_Initialize();
    MApi_AUDIO_SetSourceInfo(E_AUDIO_INFO_DTV_IN);
    //MApi_AUDIO_EnableTone(TRUE);
    MApi_AUDIO_SetAC3PInfo(Audio_AC3P_infoType_DrcMode , 1, 0);

    _bAudioInit = TRUE;
    return TRUE;
}


static void _DemoAV_AudioMaster_Monitor_task(MS_U32 argc, void *argv)
{
    //previous STC and APTS
    MS_U32 u32Pre_STC_L = 0;
    MS_U32 u32Pre_APTS = 0;

    //STC and APTS
    MS_U32 u32Cur_STC_L = 0;    //low 32bit
    MS_U32 u32Cur_STC_H = 0;    //high 32bit
    MS_U32 u32Cur_APTS = 0;

    //count
    MS_U8 u8Count = 0;

    //time difference
    MS_S32 s32Td = 0;

    while(1)
    {
        //if not audio master mode then skip this loop
        if(_u8AVSync_Mode != 1)
        {
            MsOS_DelayTask(1);
            continue;
        }

        MApi_DMX_Stc_Eng_Get(0, &u32Cur_STC_H, &u32Cur_STC_L);
        u32Cur_APTS = (MS_U32)(MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_ADEC1_33bit_PTS));

        if((u32Pre_STC_L==u32Cur_STC_L)&&(u32Pre_APTS==u32Cur_APTS))
        {
            MsOS_DelayTask(1);
            continue;
        }
        else
        {
            u32Pre_STC_L = u32Cur_STC_L;
            u32Pre_APTS = u32Cur_APTS;
        }

        s32Td = ((MS_S32)(u32Cur_STC_L-u32Cur_APTS)/90);

        if((s32Td > 33) || (s32Td < -33))
        {
            u8Count++;
        }
        else
        {
            u8Count = 0;
        }

        //if 2 continous Td are bigger or smaller than 33ms then STC update by APTS (follow MM algorithm)
        if(u8Count == 2)
        {
            //update APTS -> STC
            MApi_DMX_Stc_Set(0L, u32Cur_APTS);
            printf("[%s][%d] APTS -> STC, u32Cur_APTS=%lx\n", __FUNCTION__, __LINE__, u32Cur_APTS);
            u8Count = 0;
        }

        MsOS_DelayTask(1);
    }
}

static MS_BOOL _DemoAV_AudioMaster_SetMonitor(void)
{
    if(_audio_AudioMaster_Monitor_Task.iId == -1)
    {
        _audio_AudioMaster_Monitor_Task.pStack = MsOS_AllocateMemory(_audio_AudioMaster_Monitor_Task.u32StackSize, gs32CachedPoolID);
        if(!_audio_AudioMaster_Monitor_Task.pStack)
        {
            printf("allocate stack fail\n");
            GEN_EXCEP;
        }
        _audio_AudioMaster_Monitor_Task.iId = MsOS_CreateTask( _DemoAV_AudioMaster_Monitor_task,
                                            NULL,
                                            _audio_AudioMaster_Monitor_Task.ePriority,
                                            TRUE,
                                            _audio_AudioMaster_Monitor_Task.pStack,
                                            _audio_AudioMaster_Monitor_Task.u32StackSize,
                                            _audio_AudioMaster_Monitor_Task.szName);

        if(_audio_AudioMaster_Monitor_Task.iId < 0)
        {
            printf("create audio freerun monitor task fail \n");
            GEN_EXCEP;
        }
        else
        {
            return TRUE;
        }
    }

    return FALSE;
}


static void _DemoAV_ADEC_Play_AudioMaster(En_DVB_decSystemType AudioCodecType)
{
    MS_U8 u8Volume = DEFAULT_VOLUME;
    MS_BOOL bMute = FALSE;
    if (MSAPI_AUD_DVB_NONE == AudioCodecType)
    {
        return;
    }
    _DemoAV_ADEC_Init();

    //Set MAD
    //MDrv_AUDIO_SetNormalPath(AUDIO_PATH_MAIN_SPEAKER, AUDIO_SOURCE_DTV, AUDIO_OUTPUT_MAIN_SPEAKER);
    MApi_AUDIO_SetSystem(AudioCodecType);
    MApi_AUDIO_SetCommand(MSAPI_AUD_DVB_DECCMD_PLAYFILETSP);
    MApi_AUDIO_SetCommand(MSAPI_AUD_DVB_DECCMD_FREE_RUN);

    bMute = FALSE;
    appDemo_Audio_SetMute(&bMute);

    u8Volume = DEFAULT_VOLUME;
    appDemo_Audio_SetAbsoluteVolume(&u8Volume);

}


//------------------------------------------------------------------------------
/// @brief The sample code to enable audio decoder and video decoder
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b TV_PlayAV \n
//------------------------------------------------------------------------------
MS_BOOL DemoAV_PlayAV(void)
{
    VDEC_EX_Result enRst = E_VDEC_EX_FAIL;
    VDEC_StreamId *stVDECStreamId = NULL;
    MS_U32 u32APTS = 0, u32TmpAPTS = 0;
    MS_U32 u32Timeout_StartTime = 0;
    MS_U8 u8AVSync_Delay = 0;

    stVDECStreamId = appDemo_Video_GetStreamID();

    printf("==DemoAV_PlayAV==\n");
    //MDrv_DEB_SetPictureSize(IMG_SIZE_H, IMG_SIZE_V); //apply DemoAV_ClipImage() later to set correct image size for deblocking module


    if (TRUE != _DemoAV_VDEC_Init(_u32VCodec)) // default mepg2
    {
        printf("[%s][%d] _DemoAV_VDEC_Init fail\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    if(_u8AVSync_Mode == 1)
    {
        //audio master with using TS file play command, and no decoder delay in this mode
        u8AVSync_Delay = 0;
    }
    else
    {
        //av-sync mode
        u8AVSync_Delay = 180;
    }

    //change sync offset to 180ms, this value will influence lip-sync
    //If you want to change this value, please inform video and audio team at the same time
    enRst = MApi_VDEC_EX_AVSyncOn(stVDECStreamId,TRUE, u8AVSync_Delay, 0);
    if(enRst != E_VDEC_EX_OK)
    {
        printf("[%s][%d] MApi_VDEC_EX_AVSyncOn fail\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    //Set Field Delay count befor VDEC play
    MApi_VDEC_EX_SetControl(stVDECStreamId,E_VDEC_EX_USER_CMD_FD_MASK_DELAY_COUNT, 16);
    MApi_VDEC_EX_SetControl(stVDECStreamId,E_VDEC_EX_USER_CMD_FAST_DISPLAY, ENABLE);
    MApi_VDEC_EX_SetControl(stVDECStreamId,E_VDEC_EX_USER_CMD_AVSYNC_REPEAT_TH, 0xff);

    if(MApi_VDEC_EX_SetAVSyncFreerunThreshold(stVDECStreamId,VDES_AVSYNCTIME)!= E_VDEC_EX_OK)//VDES_AVSYNCTIME
    {
        printf("[%s][%d] MApi_VDEC_EX_SetAVSyncFreerunThreshold fail\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    MDrv_MVOP_Enable(DISABLE);

    if(_u8AVSync_Mode == 1)
    {
        // Audio master mode

        _DemoAV_ADEC_Play_AudioMaster(_DemoAV_ADEC_Fmt(_u32ACodec));

        MApi_DMX_AVFifo_Reset(DMX_FILTER_TYPE_AUDIO, FALSE);

        //try to get the first APTS
        u32APTS = (MS_U32)((MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_ADEC1_33bit_PTS)) / 90);
        u32TmpAPTS = u32APTS;
        while(u32TmpAPTS == u32APTS)
        {
            u32TmpAPTS = (MS_U32)((MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_ADEC1_33bit_PTS)) / 90);
        }

        u32APTS = u32TmpAPTS;


        // set 1st APTS to STC
        MApi_DMX_Stc_Set(0L, (u32APTS*90));
        printf("u32APTS = %lu\n", u32APTS);

        //mute audio
        MApi_AUDIO_SetCommAudioInfo(Audio_Comm_infoType_ADEC1_mute, 1, 0);

        // ask Audio DSP to sync STC
        MApi_AUDIO_SetCommAudioInfo(Audio_Comm_infoType_syncSTC_in_mmTs, 1, 0);

        // Video play command
        if (MApi_VDEC_EX_Play(stVDECStreamId) != E_VDEC_OK)
        {
            printf("[%s][%d] MApi_VDEC_Play fail\n", __FUNCTION__, __LINE__);
            return FALSE;
        }
        MApi_DMX_AVFifo_Reset(DMX_FILTER_TYPE_VIDEO, FALSE);

        //wait to start sync STC
        u32Timeout_StartTime = MsOS_GetSystemTime();
        while(MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_syncSTC_in_mmTs) == 0)
        {
            //printf("[%s][%d] wait to start sync STC!!!\n", __FUNCTION__, __LINE__);

            if (MsOS_GetSystemTime()-u32Timeout_StartTime > WAIT_TIMEOUT)
            {
                //timeout
                return FALSE;
            }
        }

        // disable sync STC flag
        MApi_AUDIO_SetCommAudioInfo(Audio_Comm_infoType_syncSTC_in_mmTs, 0, 0);

        //unmute audio
        MApi_AUDIO_SetCommAudioInfo(Audio_Comm_infoType_ADEC1_mute, 0, 0);

        //create a new task to monitor STC and APTS difference
        if(_DemoAV_AudioMaster_SetMonitor())
        {
            printf("[%s][%d] create audio master monitor!\n", __FUNCTION__, __LINE__);
        }
        else
        {
            printf("[%s][%d] audio master monitor already exist!\n", __FUNCTION__, __LINE__);
        }
    }
    else
    {
        // AV-sync mode

        // MDrv_MAD_Dvb_setDecCmd(MAD_PLAY);
        if (MApi_VDEC_EX_Play(stVDECStreamId) != E_VDEC_EX_OK)
        {
            printf("[%s][%d] MApi_VDEC_EX_Play fail\n", __FUNCTION__, __LINE__);
            return FALSE;
        }
        MApi_DMX_AVFifo_Reset(DMX_FILTER_TYPE_VIDEO, FALSE);
        _DemoAV_ADEC_Play(_DemoAV_ADEC_Fmt(_u32ACodec));
        MApi_DMX_AVFifo_Reset(DMX_FILTER_TYPE_AUDIO, FALSE);
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to stop audio decoder and halt video decoder
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b TV_PauseAV \n
//------------------------------------------------------------------------------
MS_BOOL DemoAV_PauseAV(void)
{
    VDEC_StreamId *stVDECStreamId = NULL;
    stVDECStreamId = appDemo_Video_GetStreamID();

    // @FIXME: forget audio part at this stage
    // MDrv_MAD_Dvb_setDecCmd(MAD_STOP);
    MApi_DMX_AVFifo_Reset(DMX_FILTER_TYPE_VIDEO, TRUE);
    if (MApi_VDEC_EX_Pause(stVDECStreamId) != E_VDEC_EX_OK)
    {
        return FALSE;
    }
    MApi_DMX_AVFifo_Reset(DMX_FILTER_TYPE_AUDIO, TRUE);
    _DemoAV_ADEC_Stop();
    return TRUE;

}

//------------------------------------------------------------------------------
/// @brief The sample code to stop both audio and video decoder
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b TV_StopAV \n
//------------------------------------------------------------------------------
MS_BOOL DemoAV_StopAV(void)
{
    if (_audio_AudioMaster_Monitor_Task.iId != -1)
    {
        MsOS_DeleteTask(_audio_AudioMaster_Monitor_Task.iId);
        printf("delete audio freerun task!\n");
        _audio_AudioMaster_Monitor_Task.iId = -1;
    }
    else
    {
        printf("delete audio freerun task fail!\n");
    }
    return DemoAV_PauseAV();
}

static void _DemoAV_ADEC_Play(En_DVB_decSystemType AudioCodecType)
{
    MS_U8 u8Volume = DEFAULT_VOLUME;
    MS_BOOL bMute = FALSE;

    if (MSAPI_AUD_DVB_NONE == AudioCodecType)
    {
        return;
    }
    _DemoAV_ADEC_Init();

    //Set MAD
    //MDrv_AUDIO_SetNormalPath(AUDIO_PATH_MAIN_SPEAKER, AUDIO_SOURCE_DTV, AUDIO_OUTPUT_MAIN_SPEAKER);
    MApi_AUDIO_SetSystem(AudioCodecType);
    MApi_AUDIO_SetCommand((En_DVB_decCmdType)MSAPI_AUD_PLAY);
    MApi_AUDIO_SetCommand(MSAPI_AUD_DVB_DECCMD_AVSYNC);

    bMute = FALSE;
    appDemo_Audio_SetMute(&bMute);

    u8Volume = DEFAULT_VOLUME;
    appDemo_Audio_SetAbsoluteVolume(&u8Volume);

}

static void _DemoAV_ADEC_Stop(void)
{
    MS_BOOL bMute = FALSE;

    bMute = TRUE;
    appDemo_Audio_SetMute(&bMute);

    MApi_AUDIO_SetCommand( (En_DVB_decCmdType)MSAPI_AUD_STOP );
}

//------------------------------------------------------------------------------
/// @brief The sample code to set the av-sync mode
/// @param[in] u8mode 0x0: av-sync mode
///                   0x1: audio master mode
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b TV_Set_AVSync_Mode \n
//------------------------------------------------------------------------------
MS_BOOL DemoAV_Set_AVSYNC_Mode(MS_U8* u8mode)
{
    _u8AVSync_Mode = *u8mode;
    return TRUE;
}

static En_DVB_decSystemType _DemoAV_ADEC_Fmt(MS_U32 u32ACodec)
{
    switch (u32ACodec)
    {
    case 0x01:
        return MSAPI_AUD_DVB_MPEG;
    case 0x02:
        return MSAPI_AUD_DVB_AC3;
    case 0x03:
        return MSAPI_AUD_DVB_AC3P;
    case 0x04:
        return MSAPI_AUD_DVB_AAC;
    default:
        return MSAPI_AUD_DVB_NONE;
    }
    return MSAPI_AUD_DVB_NONE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to set volume
/// @param[in] Vol Volume, range {0...100}
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b Command:TV_MAD_SetVol \n
//------------------------------------------------------------------------------
MS_BOOL DemoAV_MAD_SetVol(MS_U8 *Vol)
{
    printf("[%s][%d] Set Volume to %d\n", __FUNCTION__, __LINE__, *Vol);

    // if (*Vol>100) *Vol = 100;
    // MDrv_MAD_ProcessSetVolume(AUDIO_PATH_0, *Vol);
    appDemo_Audio_SetAbsoluteVolume(Vol);

    return TRUE;
}


//------------------------------------------------------------------------------
/// @brief Print help message
/// @return None
/// @sa
/// Command: \b Command:HelpTV \n
//------------------------------------------------------------------------------
void DemoAV_Help_TV(void)
{
    printf ("************************* TV function List *************************\n");
    printf ("TV_TSP_SetPid [VideoPid] [AudioPid] [PCRPid] [AudDec] [VidDec]\n");
    printf ("-----VideoPid : Video Pid\n");
    printf ("-----AudioPid : Audio Pid\n");
    printf ("-----PCRPid : PCR Pid\n");
    printf ("-----AudDec : Audio Dec Type\n");
    printf ("-----VidDec : Video Dec Type\n");
    printf ("TV_TSP_SetPid_File [VideoPid] [AudioPid] [PCRPid] [AudDec] [VidDec]\n");
    printf ("** TS file mode only **\n");
    printf ("-----VideoPid : Video Pid\n");
    printf ("-----AudioPid : Audio Pid\n");
    printf ("-----PCRPid : PCR Pid\n");
    printf ("-----AudDec : Audio Dec Type\n");
    printf ("-----VidDec : Video Dec Type\n");
    printf ("TV_MVD_GetVideoInfo\n");
    printf ("TV_MVD_DecIFrame [destBufAddr] [srcBufStartAddr] [srcBufEndAddr] [codec]\n");
    printf ("TV_PlayAV\n");
    printf ("TV_PauseAV\n");
    printf ("TV_StopAV\n");
    printf ("TV_MAD_SetVol [Vol]\n");
    printf ("-----Vol: Volume, range {0...100}\n");
    printf ("************************* End of TV function List*************************\n");

}



