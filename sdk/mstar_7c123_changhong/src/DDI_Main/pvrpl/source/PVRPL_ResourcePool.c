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
// ("MStar Confidential Information") by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   Resource_Pool_Manager.c
/// @author MStar Semiconductor Inc.
/// @brief  Get resource from Pool
///
/// @name how to get resource
/// @verbatim
///=================================================================================================
#include "PVRPL_ResourcePool.h"

// DDI
#include "demo_pvr.h"
#include "demo_dmx.h"
#include "demo_av.h"
#include "demo_zapping.h"
#include "demo_vdec.h"
#include "demo_xc.h"

#if(DEMO_AUDIO_TEST == 1)
#include "apiAUDIO.h"
#include "demo_audio.h"
#endif

// UTPA
#include "drvMVOP.h"
#include "drvDTC.h"

//msAPI
#if (DEMO_XC_DUALXC_TEST == 1)
#include "msAPI_XC_EX.h"
#else
#include "msAPI_XC.h"
#endif

#include "apiDAC.h"
#include "msAPI_VE.h"

// Porting Layer Video
static POOL_VDEC_CURR_INFO stVideoCurrIndo[PVR_PLAYBACK_THREAD_NUM];
static MSAPI_XC_DEVICE_ID g_stXC_DeviceId[2] = {{0, E_MSAPI_XC_DEVICE0}, {0, E_MSAPI_XC_DEVICE1}};

/// XC check device ID
#if (DEMO_XC_DUALXC_TEST == 1)
#define PVRPL_XC_CHECK_DEVICE_ID(u32Id)                                         \
    if ((E_MSAPI_XC_DEVICE_NUM)u32Id > E_MSAPI_XC_DEVICE1)                      \
    {                                                                           \
        printf("WANRING: Device ID %"DTC_MS_U32_d" exceeded \n", u32Id);        \
        return FALSE;                                                           \
    }
#else
#define PVRPL_XC_CHECK_DEVICE_ID(u32Id)                                         \
    if ((E_MSAPI_XC_DEVICE_NUM)u32Id > E_MSAPI_XC_DEVICE0)                      \
    {                                                                           \
        printf("WANRING: Device ID %"DTC_MS_U32_d" exceeded \n", u32Id);        \
        return FALSE;                                                           \
    }
#endif

#define E_DECODER_NUMBER (4)
static ST_AUDIO_DEC_INFO stDualDemoInfo[E_DECODER_NUMBER]={{.eDecID=AU_DEC_INVALID},{.eDecID=AU_DEC_INVALID},{.eDecID=AU_DEC_INVALID},{.eDecID=AU_DEC_INVALID}};

MS_BOOL _Pool_Audio_GetDecInfo(MS_U32 u32DevIdx, ST_AUDIO_DEC_INFO *pstInfo)
{
    if (MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Get_MultiPlayer_Capability) == 1)
    {
        if((u32DevIdx >= E_DECODER_NUMBER) || (pstInfo == NULL))
        {
            return FALSE;
        }
        memcpy((void *)pstInfo, (void *)&stDualDemoInfo[u32DevIdx], sizeof(ST_AUDIO_DEC_INFO));
    }

    return TRUE;
}

void Pool_SetMVOP(MS_U8 u8PathID,MS_BOOL bSet)
{
    if (u8PathID == 0)
    {
        MDrv_MVOP_Enable(DISABLE);
        printf("Display Main MVOP Disable\n");
    }
#if(DEMO_VDEC_MULTI_TEST == 1)
    else if (u8PathID == 1)
    {
        MDrv_MVOP_SubEnable(DISABLE);
        printf("Display Sub MVOP Disable\n");
    }
#endif
}

// ******************************************************************************
//Wrapper function for Porting Layer Using
// ******************************************************************************
MS_U32 Pool_GetMIUAddress(PVRPL_MIU enMiuID)
{
    if(enMiuID == E_MEM_MIU0)
    {
        return 0;
    }
    else
    {
        return (MS_U32)MIU_INTERVAL;
    }
}


//------------------------------------------------------------------------------
/// @brief The sample code to eable XC window on/off
///           Please note that when x, y, width and height are 0s, original window size iskept.
/// @param[in] *pu32XCDevice the XC Device
/// @param[in] *pu32XCWindow the XC Window
/// @param[in] *pu32Enable Enable/Disable window
/// @param[in] *x Start Position of Y
/// @param[in] *y Start Position of Y
/// @param[in] *width Horizontal Size
/// @param[in] *height Vertical Size
/// @return TRUE: Get success.
/// @return FALSE: Get fail.
/// @sa
/// @note
/// Command: \b DigiTuner_GetIndex \n
//------------------------------------------------------------------------------
MS_BOOL PVRPL_XC_EnableWindow(MS_U32 *pu32XCDevice, MS_U32 *pu32XCWindow, MS_U32 *pu32Enable,
                                    MS_U32 *pu32X, MS_U32 *pu32Y, MS_U32 *pu32Width, MS_U32 *pu32Height)
{
    MSAPI_XC_WINDOW_TYPE stWindowType = {(MS_U16)*pu32X, (MS_U16)*pu32Y, (MS_U16)*pu32Width, (MS_U16)*pu32Height};

    PVRPL_XC_CHECK_DEVICE_ID(*pu32XCDevice)

    if ((*pu32X == 0) && (*pu32Y == 0) && (*pu32Width == 0) && (*pu32Height == 0))
    {
        msAPI_XC_EnableWindow_EX(&g_stXC_DeviceId[*pu32XCDevice], (E_MSAPI_XC_WINDOW)*pu32XCWindow, (MS_BOOL)*pu32Enable, NULL);
    }
    else
    {
        msAPI_XC_EnableWindow_EX(&g_stXC_DeviceId[*pu32XCDevice], (E_MSAPI_XC_WINDOW)*pu32XCWindow, (MS_BOOL)*pu32Enable, &stWindowType);
    }

    return TRUE;
}


//------------------------------------------------------------------------------
/// @brief Enable/disable video mute
/// @param[in] *pu32XCDevice the XC Device
/// @param[in] *pu32XCWindow the XC Window
/// @param[in] *pu32Enable 1=ENABLE, 0=DISABLE
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b XC_SetVideoMute 0 0 1\n
///          \b XC_SetVideoMute 0 0 0\n
//------------------------------------------------------------------------------
MS_BOOL PVRPL_XC_SetVideoMute(MS_U32 *pu32XCDevice, MS_U32 *pu32XCWindow, MS_U32 *pu32Enable)
{
    PVRPL_XC_CHECK_DEVICE_ID(*pu32XCDevice)

    if (*pu32XCDevice == E_MSAPI_XC_DEVICE1)
    {
        // If device is XC1 and its source is OP, do not mute XC1 to keep UI on SD output
        E_MSAPI_XC_INPUT_SRC eXCInputSrc = E_MSAPI_XC_INPUT_SOURCE_NONE;
        eXCInputSrc = msAPI_XC_GetConnect_EX(&g_stXC_DeviceId[*pu32XCDevice], *pu32XCWindow);
        if (eXCInputSrc == E_MSAPI_XC_INPUT_SOURCE_SCALER_OP)
        {
           return TRUE;
        }
    }
    msAPI_XC_SetVideoMute_EX(&g_stXC_DeviceId[*pu32XCDevice], *pu32Enable, *pu32XCWindow);
    return TRUE;
}


//------------------------------------------------------------------------------
/// @brief Enable/disable video mute
/// @param[in] *pu32Enable 1=ENABLE, 0=DISABLE
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b VE_SetVideoMute 0\n
///          \b VE_SetVideoMute 1\n
//------------------------------------------------------------------------------
MS_BOOL PVRPL_VE_SetVideoMute(MS_U32 *pu32Enable)
{
    if ( msAPI_VE_GetSourceType() == E_MSAPI_VE_SOURCE_SCALER_OP2)
    {
        return TRUE;
    }
    msAPI_VE_SetVideoMute((MS_BOOL)*pu32Enable);

    return TRUE;
}


// ******************************************************************************
// End of Wrapper function
// ******************************************************************************

//Porting Layer Record
DMX_PVR_ENG Pool_GetEng(MS_U8 u8PathID)
{

    switch(u8PathID)
    {
        case 0:
            return DMX_PVR_EGN0;
        case 1:
            return DMX_PVR_EGN1;
        case 2:
            return DMX_PVR_EGN2;
        case 3:
            return DMX_PVR_EGN3;
        case 4:
            return DMX_PVR_EGN4;
        case 5:
            return DMX_PVR_EGN5;
        default:
            return DMX_PVR_EGN0;
    }
    return TRUE;
}


//Get the Audio Source in SuperNova/HB/DDI
//TSIF source get
DMX_TSIF Pool_GetSource(DMX_FLOW_INPUT *peDmxInput)
{
    EN_DEMO_DMX_FLOW ePlayback = E_DMX_FLOW_INVALID;
    EN_DEMO_DMX_FLOW_INPUT DemoDmxInput = E_DMX_FLOW_INPUT_INVALID;

    Demo_PVR_GetRecFlowSet(&DemoDmxInput,&ePlayback);


    switch(DemoDmxInput)
    {
        case E_DMX_FLOW_INPUT_DEMOD0:
            *peDmxInput = DMX_FLOW_INPUT_DEMOD;
            break;
        case E_DMX_FLOW_INPUT_DEMOD1:
            *peDmxInput = DMX_FLOW_INPUT_DEMOD1;
            break;
        case E_DMX_FLOW_INPUT_EXT_INPUT0:
            *peDmxInput = DMX_FLOW_INPUT_EXT_INPUT0;
            break;
        case E_DMX_FLOW_INPUT_EXT_INPUT1:
            *peDmxInput = DMX_FLOW_INPUT_EXT_INPUT1;
            break;
        case E_DMX_FLOW_INPUT_EXT_INPUT2:
            *peDmxInput = DMX_FLOW_INPUT_EXT_INPUT2;
            break;
        case E_DMX_FLOW_INPUT_EXT_INPUT3:
            *peDmxInput = DMX_FLOW_INPUT_EXT_INPUT3;
            break;
        default:
            printf("Wrong Tuner\n");
            *peDmxInput = DMX_FLOW_INPUT_DEMOD;
            break;
    }

    switch(ePlayback)
    {
        case E_DMX_FLOW_LIVE0:
             return DMX_TSIF_LIVE0;
        case E_DMX_FLOW_LIVE1:
             return DMX_TSIF_LIVE1;
        case E_DMX_FLOW_LIVE2:
             return DMX_TSIF_LIVE2;
        case E_DMX_FLOW_LIVE3:
             return DMX_TSIF_LIVE3;
        default:
            printf("Wrong Tuner\n");
            return DMX_TSIF_LIVE0;
    }


}

DMX_PVR_ENG Pool_GetRecEng(void)
{
    EN_DEMO_DMX_PVR_ENG eDmxPvrEng = E_DMX_PVR_ENG_INVALID;

    Demo_PVR_GetRecEng(&eDmxPvrEng);

    switch(eDmxPvrEng)
    {
        case E_DMX_PVR_EGN0:
            return DMX_PVR_EGN0;
        case E_DMX_PVR_EGN1:
            return DMX_PVR_EGN1;
        case E_DMX_PVR_EGN2:
            return DMX_PVR_EGN2;
        case E_DMX_PVR_EGN3:
            return DMX_PVR_EGN3;
        default:
            printf("Wrong PVR Eng\n");
            return DMX_PVR_EGN0;
    }

}



//Porting Layer FileIn
DMX_FILEIN_PATH Pool_GetFileInSouce(MS_U8 u8PathID)
{
    if(u8PathID == 0)
    {
        return DMX_FILEIN_TSIF0;
    }
    else
    {
        return DMX_FILEIN_TSIF1;
    }
}


DMX_FLOW Pool_GetPVRDMXFlow(MS_U8 u8PathID)
{
    if(u8PathID == 0)
    {
        return DMX_FLOW_PLAYBACK;
    }
    else
    {
        return DMX_FLOW_PLAYBACK1;
    }
}


//Get the Audio Source in SuperNova/HB/DDI
//Audio source mapping with play index
DMX_FILTER_TYPE Pool_GetAFifoType(MS_U8 u8PathID)
{
    if (MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Get_MultiPlayer_Capability) == 1)
    {
        ST_AUDIO_DEC_INFO stAudioDecInfo;
        _Pool_Audio_GetDecInfo(u8PathID,&stAudioDecInfo);

        switch(stAudioDecInfo.stAudioDecStatus_t.eAfifoSource)
        {
            case E_AFIFO_0:
                 return DMX_FILTER_TYPE_AUDIO;
            case E_AFIFO_1:
                 return DMX_FILTER_TYPE_AUDIO2;
            case E_AFIFO_2:
                 return DMX_FILTER_TYPE_AUDIO3;
            default:
                 printf("[%s][%s][%d][ERROR]Get Audio type fail!!!\n",__FILE__,__FUNCTION__,__LINE__);
                 return DMX_FILTER_TYPE_AUDIO;
        }
    }
    else
    {
        return DMX_FILTER_TYPE_AUDIO;
    }
}

//Depends on different Playback path get the different video fifo type
DMX_FILTER_TYPE Pool_GetVFifoType(MS_U8 u8PathID)
{
    if(u8PathID == 0)
    {
        return DMX_FILTER_TYPE_VIDEO;
    }
    else
    {
        return DMX_FILTER_TYPE_VIDEO3D;
    }
}

void Pool_SetVideoCurrCmdSetting(MS_U8 u8PathID, MS_S32 s32Speed, EN_VIDEO_CPL_DECODE_TYPE enDecodeType, EN_VIDEO_CPL_DATA_TYPE enDataType)
{
    stVideoCurrIndo[u8PathID].u8PathID = u8PathID;
    stVideoCurrIndo[u8PathID].s32Speed = s32Speed;
    stVideoCurrIndo[u8PathID].enDecodeType = enDecodeType;
    stVideoCurrIndo[u8PathID].enDataType = enDataType;
}

POOL_VDEC_CURR_INFO* Pool_GetVideoCurrCmdSetting(MS_U8 u8PathID)
{
    return &stVideoCurrIndo[u8PathID];
}
VDEC_StreamId* Pool_GetVideoID(MS_U8 u8PathID)
{
    void *vStreamID = NULL;
    if(u8PathID == 0)
    {
        EN_VDEC_Device eDevice= E_VDEC_DEVICE_MAIN;
        vStreamID = Demo_VDEC_GetStreamID(&eDevice);
    }
    else if(u8PathID == 1)
    {
        EN_VDEC_Device eDevice= E_VDEC_DEVICE_SUB;
        vStreamID = Demo_VDEC_GetStreamID(&eDevice);
    }
    else
    {
        printf("[%s][%d]Wrong AV PATH(%d)\n",__FUNCTION__, __LINE__,u8PathID);
    }

    return (VDEC_StreamId*)vStreamID;
}

VDEC_EX_Stream Pool_GetStreamType(MS_U8 u8PathID)
{

    if (u8PathID == 0)
    {
        return E_VDEC_EX_MAIN_STREAM;
    }
#if(DEMO_VDEC_MULTI_TEST == 1)
    else if (u8PathID == 1)
    {
        return E_VDEC_EX_SUB_STREAM;
    }
#endif
    printf("[%s][%d]Wrong AV PATH(%d)\n",__FUNCTION__, __LINE__,u8PathID);
    return E_VDEC_EX_MAIN_STREAM;
}

/*
POOL_PCR_ENG Pool_GetPCREng(MS_U8 u8PathID)
{
    EN_PCR_ENG eRet = E_POOL_PCR_ENG0;

    if(u8PathID == 0)
    {
        eRet = (POOL_PCR_ENG)Demo_AV_Set_STC(E_AV_DEVICE_MAIN);

    }
    else
    {
        eRet = (POOL_PCR_ENG)Demo_AV_Set_STC(E_AV_DEVICE_SUB);
    }
    return eRet;
}
*/
//Porting Layer Audio
AUDIO_DEC_ID Pool_GetAudioID(MS_U8 u8PathID)
{
    if (MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Get_MultiPlayer_Capability) == 1)
    {
        ST_AUDIO_DEC_INFO stAudioDecInfo = {};
        _Pool_Audio_GetDecInfo(u8PathID,&stAudioDecInfo);
        return stAudioDecInfo.eDecID;
    }

    return AU_DEC_ID1;
}

//XC Setting
MS_BOOL POOL_XC_Mute(MS_U8 u8PathID)
{
    MS_U32 u32XCDevice = 0;
    MS_U32 u32XCWindow = 0;
    MS_BOOL bWindowEnable = FALSE;
    MS_U32 u32DisplayMute = ENABLE;
    MS_U32 u32X = 0;
    MS_U32 u32Y = 0;
    MS_U32 u32Width = 0;
    MS_U32 u32Height = 0;
    EN_ZAPPING_TYPE eZappingType = E_ZAPPING_NORMAL;

    if(u8PathID == 0)
    {
        u32XCWindow = 0;
    }
    else
    {
        u32XCWindow = 1;

        PVRPL_XC_EnableWindow(&u32XCDevice, &u32XCWindow, (MS_U32*)&bWindowEnable,
                             &u32X, &u32Y, &u32Width, &u32Height);

        printf("[%s][%d]PVRPL_XC_EnableWindow:%"DTC_MS_U32_d" %"DTC_MS_U32_d" %d\n",__FUNCTION__, __LINE__,u32XCDevice,u32XCWindow,FALSE);
    }

    EN_AV_Device eDevice = u8PathID;
    Demo_Zapping_GetZappingType(&eDevice,&eZappingType);
#if (DEMO_XC_SEAMLESS_ZAPPING_TEST == 1)
    if (((eZappingType == E_ZAPPING_XC_SEAMLESS) && (Demo_XC_SeamlessZapping_IsFreeze(&u32XCDevice, &u32XCWindow) == FALSE)) ||
        (eZappingType == E_ZAPPING_NORMAL))
#endif
    {

        PVRPL_XC_SetVideoMute(&u32XCDevice, &u32XCWindow, &u32DisplayMute); // Mute HDMI Main/Sub Window
        printf("[%s][%d]PVRPL_XC_SetVideoMute:%"DTC_MS_U32_d" %"DTC_MS_U32_d" %"DTC_MS_U32_d"\n",__FUNCTION__, __LINE__,u32XCDevice,u32XCWindow,u32DisplayMute);

        if(u8PathID == 0)
        {
            u32XCDevice = 1;
            PVRPL_XC_SetVideoMute(&u32XCDevice, &u32XCWindow, &u32DisplayMute); // Mute CVBS Main Window
            printf("[%s][%d]PVRPL_XC_SetVideoMute:%"DTC_MS_U32_d" %"DTC_MS_U32_d" %"DTC_MS_U32_d"\n",__FUNCTION__, __LINE__,u32XCDevice,u32XCWindow,u32DisplayMute);
            PVRPL_VE_SetVideoMute(&u32DisplayMute);                        // Mute CVBS
            printf("[%s][%d]PVRPL_VE_SetVideoMute:%"DTC_MS_U32_d"\n",__FUNCTION__, __LINE__,u32DisplayMute);
        }
    }
    return TRUE;
}
