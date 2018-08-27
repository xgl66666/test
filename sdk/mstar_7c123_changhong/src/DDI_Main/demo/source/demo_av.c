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
#if (DEMO_AV_TEST == 1)
//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------
#include <string.h>
#include <ctype.h>
#include "MsMemory.h"

#include "apiDMX.h"
#include "apiVDEC_EX.h"
#include "apiHDMITx.h"
#include "apiAUDIO.h"
#include "apiXC.h"
#if (DEMO_XC_DUALXC_TEST == 1)
#include "apiXC_EX.h"
#endif
#include "drvMVOP.h"
#include "drvDTC.h"
#include "Panel.h"

#include "demo_audio_common.h"
#include "demo_dmx.h"
#include "demo_xc.h"
#include "demo_av.h"
#include "demo_vdec.h"
#include "demo_zapping.h"
#include "demo_utility.h"
#include "demo_main.h"
#include "demo_osd.h"

#if(DEMO_AUDIO_TEST == 1)
#include "demo_audio.h"
#endif


#if (DEMO_MM_TEST == 1) && (DEMO_MM_AVP_TEST == 1)
#include "mmsdk_interface_def.h"
#include "demo_mm.h"
#endif

//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------
/// AV check device ID
#if (DEMO_VDEC_MULTI_TEST == 1 || DEMO_VDEC_NDECODE_TEST == 1)
#define DEMO_AV_CHECK_DEVICE_ID(u8Id)                            \
    if (((EN_AV_Device)u8Id >= E_AV_DEVICE_MAX) || ((EN_AV_Device)u8Id == E_AV_DEVICE_INVALID))         \
    {                                                             \
        printf("[%s][%d] Wrong Device ID %d\n",__FUNCTION__,__LINE__, u8Id);      \
        return FALSE;                                             \
    }
#else
#define DEMO_AV_CHECK_DEVICE_ID(u8Id)                            \
    if (((EN_AV_Device)u8Id > 0) || ((EN_AV_Device)u8Id == E_AV_DEVICE_INVALID))        \
    {                                                             \
        printf("[%s][%d] Not support vdec multi Device ID %d\n",__FUNCTION__,__LINE__, u8Id);      \
        return FALSE;                                             \
    }
#endif

//-------------------------------------------------------------------------------------------------
// Global Definition
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
// Local Definition
//-------------------------------------------------------------------------------------------------
#define AV_DBG          0
#define WAITTIMEOUT     5000
#define INVALID_TASK_ID     -1
#define INVALID_PTS         -1

#if AV_DBG
#define db_print(fmt, args...)  printf("[%s][%d]" fmt, __FUNCTION__, __LINE__, ## args)
#else
#define db_print(fmt, args...)
#endif

//-------------------------------------------------------------------------------------------------
// Global Variables
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
// Local Variables
//-------------------------------------------------------------------------------------------------
static ST_AV_INFO stAVInfo[E_AV_DEVICE_MAX];
Task_Info AV_SYNC_Task = {-1, E_TASK_PRI_HIGH, NULL, 0x4000, "_AVSYNC_TASK"};

#if (DEMO_XC_DUALXC_TEST == 1)
static PNL_DeviceId stPNL_DeviceId = {0, 0};
#endif
//-------------------------------------------------------------------------------------------------
// Global function
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
// Local function
//-------------------------------------------------------------------------------------------------

static EN_AUDIO_CODEC_TYPE _Demo_AV_SI_Audio_CodecType_Mapping(EN_SI_AudioFormat eAudioFmt)
{
    switch ( eAudioFmt )
    {
        case E_SI_AUD_MPEG :
        case E_SI_AUD_MPEG_AD:
            return DEMO_AUDIO_CODEC_MPEG;

        case E_SI_AUD_AC3 :
        case E_SI_AUD_AC3_AD :
            return DEMO_AUDIO_CODEC_AC3;

        case E_SI_AUD_AC3P :
            return DEMO_AUDIO_CODEC_AC3P;

        case E_SI_AUD_MPEG4:
        case E_SI_AUD_AAC:
            return DEMO_AUDIO_CODEC_AAC;

        default:
            //DemoDmx_Print("Unsopprot Audio Format %ld!\n",eAudioFmt);
            return DEMO_AUDIO_CODEC_NONE ;
    }

}

static MS_BOOL _Demo_AV_TSP_MMFI_SetPid(EN_AV_Device *peDevice, MS_BOOL bType, MS_U32 *pu32Pid, MS_U32 *pu32Codec)
{
    ST_AV_Params *pstAvParams = NULL;

    if(bType) // Video
    {
    #if(DEMO_VDEC_NDECODE_TEST == 1)
        stAVInfo[*peDevice].stVideoParams.eFIFO_ID = *peDevice;
    #else
        if (*peDevice == E_AV_DEVICE_MAIN)
        {
            stAVInfo[*peDevice].stVideoParams.eFIFO_ID = E_DMX_FLT_TYPE_VID0;
        }
    #if(DEMO_VDEC_MULTI_TEST == 1)
        else if (*peDevice == E_AV_DEVICE_SUB)
        {
            stAVInfo[*peDevice].stVideoParams.eFIFO_ID = E_DMX_FLT_TYPE_VID1;
        }
    #endif
    #endif

        stAVInfo[*peDevice].stVideoParams.eVideoCodec = (*pu32Codec) & 0xFF;
        pstAvParams = &stAVInfo[*peDevice].stVideoParams;
    }
    else // Audio
    {
        if (MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Get_MultiPlayer_Capability) == 1)
        {
            stAVInfo[*peDevice].stAudioParams.eFIFO_ID = Demo_AV_GetAudioFIFO(peDevice);
        }
        else
        {
            stAVInfo[*peDevice].stAudioParams.eFIFO_ID = E_DMX_FLT_TYPE_AUD0;
        }

        stAVInfo[*peDevice].stAudioParams.eAudioCodec = (*pu32Codec) & 0xFF;
        pstAvParams = &stAVInfo[*peDevice].stAudioParams;
    }

    pstAvParams->u16PID = (*pu32Pid) & 0x1FFF;
    pstAvParams->stFilterType.eEngFormat = E_AV_FileIn_Eng_TS;

    if (pstAvParams->u8Filter != INVALID_FILTER_ID)
    {
        MApi_DMX_MMFI_Pid_Close(pstAvParams->u8Filter);
        pstAvParams->u8Filter = INVALID_FILTER_ID;
        db_print("MApi_DMX_MMFI_Pid_Close VideoFilter:%d\n", __FUNCTION__, __LINE__, pstAvParams->u8Filter);
    }

    //EN_DEMO_DMX_FLT_TYPE -> DMX_MMFI_FLTTYPE
    DMX_MMFI_FLTTYPE eMMFI_AVFltType = Demo_DMX_MMFI_FltTypeMapping(pstAvParams->eFIFO_ID);

    switch(pstAvParams->stFilterType.u8EngID)
    {
        case 0:
            eMMFI_AVFltType &= ~DMX_MMFI1_FLTTYPE_MASK;
            break;
        case 1:
            eMMFI_AVFltType |= DMX_MMFI1_FLTTYPE_MASK;
            break;
        default:
            printf("[%s][%d] Not support this MMFI engine number !!\n", __FUNCTION__, __LINE__);
            return FALSE;
    }

    if(DMX_FILTER_STATUS_OK != MApi_DMX_MMFI_Pid_Open(eMMFI_AVFltType, pstAvParams->u16PID, &pstAvParams->u8Filter))
    {
        printf("[%s][%d] Demo DMX MMFI VideoFlt Open fail \n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    return TRUE;
}


#if(DEMO_AUDIO_TEST == 1)
//------------------------------------------------------------------------------
/// @brief transform AV Audio AFIFO to TSP AFIFO
/// @param[in] AUDIO_AFIFO_SOURC Audio AFIFO Source
/// @return the TSP AFIFO
/// @sa
/// @note
/// Command: \b none \n
//------------------------------------------------------------------------------
static EN_DEMO_DMX_FLT_TYPE _Demo_AV_Audio_AFIFO_Map_TSP_AFIFO(AUDIO_AFIFO_SOURCE stAfifoSource)
{
    switch(stAfifoSource)
    {
        case E_AFIFO_0:
            return E_DMX_FLT_TYPE_AUD0;
        case E_AFIFO_1:
            return E_DMX_FLT_TYPE_AUD1;
        case E_AFIFO_2:
            return E_DMX_FLT_TYPE_AUD2;
#if (DEMO_AUDIO_MULTI_TEST==1)
        case E_AFIFO_3:
            return E_DMX_FLT_TYPE_AUD3;
        case E_AFIFO_4:
            return E_DMX_FLT_TYPE_AUD4;
        case E_AFIFO_5:
            return E_DMX_FLT_TYPE_AUD5;
#endif
        default:
            return E_DMX_FLT_TYPE_AUD0;
    }
}

//------------------------------------------------------------------------------
/// @brief get Audio fifo
/// @param[in] Main or Sub Device or Other Device
/// @return TRUE: Process success.
/// @return FALSE: Invalid input argument.
/// @sa
/// @note
/// Command: \b none \n
//------------------------------------------------------------------------------
EN_DEMO_DMX_FLT_TYPE Demo_AV_GetAudioFIFO(EN_AV_Device* peDevice)
{
    DEMO_AV_CHECK_DEVICE_ID(*peDevice)

    ST_AUDIO_DEC_INFO stAudioInfo={0};

    if ((*peDevice >= (MS_U32)E_AV_DEVICE_MAX) || (*peDevice == (MS_U32)E_AV_DEVICE_INVALID))
    {
        printf("Wrong Decoder \n");
        return E_DMX_FLT_TYPE_INVALID;
    }

    Demo_Audio_GetDecInfo(*peDevice, &stAudioInfo);

    if (MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Get_MultiPlayer_Capability) == 1)
    {
        if (stAudioInfo.eDecID == AU_DEC_INVALID)
        {
            return E_DMX_FLT_TYPE_INVALID;
        }
        else
        {
            return _Demo_AV_Audio_AFIFO_Map_TSP_AFIFO(stAudioInfo.stAudioDecStatus_t.eAfifoSource);
        }
    }
    else
    {
        return E_DMX_FLT_TYPE_AUD0;
    }

    return E_DMX_FLT_TYPE_INVALID;
}

//------------------------------------------------------------------------------
/// @brief get Audio AD fifo
/// @param[in] Main or Sub Device or Other Device
/// @return TRUE: Process success.
/// @return FALSE: Invalid input argument.
/// @sa
/// @note
/// Command: \b none \n
//------------------------------------------------------------------------------
EN_DEMO_DMX_FLT_TYPE Demo_AV_GetAudioADFIFO(EN_AV_Device* peDevice)
{
    DEMO_AV_CHECK_DEVICE_ID(*peDevice)

    ST_AUDIO_DEC_INFO stAudioInfo={0};

    if ((*peDevice >= (MS_U32)E_AV_DEVICE_MAX) || (*peDevice == (MS_U32)E_AV_DEVICE_INVALID))
    {
        printf("Wrong Decoder \n");
        return E_DMX_FLT_TYPE_INVALID;
    }

    if (Demo_Audio_GetDecInfo(*peDevice, &stAudioInfo) == FALSE)
    {
        printf("[%s][%d] Get Audio Decoder Info fail!!!\n", __FUNCTION__, __LINE__);
        return E_DMX_FLT_TYPE_INVALID;
    }

    if (MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Get_MultiPlayer_Capability) == 1)
    {
        if (stAudioInfo.eDecID == AU_DEC_ID1)
        {
            return E_DMX_FLT_TYPE_AUD1;
        }
        else if (stAudioInfo.eDecID == AU_DEC_ID3)
        {
            return E_DMX_FLT_TYPE_AUD3;
        }
        else
        {
            return E_DMX_FLT_TYPE_INVALID;
        }
    }
    else
    {
        return E_DMX_FLT_TYPE_AUD1;
    }

    return E_DMX_FLT_TYPE_INVALID;
}


//------------------------------------------------------------------------------
/// @brief check if main audio resource exist
/// @param[in] Main or Sub Device or Other Device
/// @return TRUE: Resource exist.
/// @return FALSE: Resource not exist.
/// @sa
/// @note
/// Command: \b none \n
//------------------------------------------------------------------------------
MS_BOOL Demo_AV_CheckMainAudioResource(EN_AV_Device* peDevice)
{
    DEMO_AV_CHECK_DEVICE_ID(*peDevice)

    ST_AUDIO_DEC_INFO stAudioInfo={0};

    if ((*peDevice >= (MS_U32)E_AV_DEVICE_MAX) || (*peDevice == (MS_U32)E_AV_DEVICE_INVALID))
    {
        printf("Wrong Decoder \n");
        return FALSE;
    }

    Demo_Audio_GetDecInfo(*peDevice, &stAudioInfo);

    if (MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Get_MultiPlayer_Capability) == 1)
    {
        if ((stAudioInfo.eDecID != AU_DEC_INVALID) && (stAudioInfo.stAudioDecStatus_t.eGroup == E_CONNECT_MAIN))
        {
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
    else
    {
        return TRUE;
    }

    return FALSE;
}

//------------------------------------------------------------------------------
/// @brief check if audio resource exist
/// @param[in] Main or Sub Device or Other Device
/// @return TRUE: Resource exist.
/// @return FALSE: Resource not exist.
/// @sa
/// @note
/// Command: \b none \n
//------------------------------------------------------------------------------
MS_BOOL Demo_AV_CheckAudioResource(EN_AV_Device* peDevice)
{
    DEMO_AV_CHECK_DEVICE_ID(*peDevice)

    ST_AUDIO_DEC_INFO stAudioInfo={0};

    if ((*peDevice >= (MS_U32)E_AV_DEVICE_MAX) || (*peDevice == (MS_U32)E_AV_DEVICE_INVALID))
    {
        printf("Wrong Decoder \n");
        return FALSE;
    }

    Demo_Audio_GetDecInfo(*peDevice, &stAudioInfo);

    if (MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Get_MultiPlayer_Capability) == 1)
    {
        if(stAudioInfo.eDecID == AU_DEC_INVALID)
        {
            return FALSE;
        }
        else
        {
            return TRUE;
        }
    }
    else
    {
        return TRUE;
    }

    return FALSE;
}

#endif

#if(DEMO_VDEC_NDECODE_TEST == 1)
EN_AV_Device Demo_AV_VDEC_Device_Mapping(EN_AV_Device eDevice)
{
    VDEC_EX_Stream eStreamType = E_VDEC_EX_MAIN_STREAM ;      // Catch Device ndecode or not
    EN_AV_Device eTempDevice = 0;

    eStreamType = eDevice >> 0x4;
    eTempDevice = eDevice & 0xF;

    if((eStreamType == E_VDEC_EX_MAIN_STREAM) && (eTempDevice == 1))
        eStreamType = E_VDEC_EX_SUB_STREAM;

    Demo_VDEC_SetMode((EN_VDEC_Device *)&eTempDevice,E_VDEC_SetCmd_StreamType,(void *)(&eStreamType));

    db_print("u8DeviceId[%d]eTempDevice[%d] \n",eStreamType,eTempDevice);

    return eTempDevice;
}
#endif

//------------------------------------------------------------------------------
/// @brief open timeout mechanism
/// @param[in] Main or Sub Device or Other Device
/// @return none
/// @sa
/// @note
/// Command: \b none \n
//------------------------------------------------------------------------------
static void _Demo_AV_TimeoutSet(EN_AV_Device eDevice,MS_BOOL bEnable)
{
    Demo_VDEC_SetMode((EN_VDEC_Device *)&eDevice,E_VDEC_SetCmd_TimeOutMech,(void *)(&bEnable));
}

//------------------------------------------------------------------------------
/// @brief SYNC state machine Task
/// @param[in] Main or Sub Device or Other Device
/// @return none
/// @sa
/// @note
/// Command: \b none \n
//------------------------------------------------------------------------------
static void _Demo_AV_SYNCTASK(MS_U32 u32Device)
{
    EN_AV_Device eDevice = 0;

    MS_U64 u64Stc = INVALID_PTS;

    MS_U64 u64Pts = INVALID_PTS;
    MS_U64 u64Vfpts = INVALID_PTS;
    MS_U64 u64Afpts = INVALID_PTS;

    //time difference
    MS_S64 s64Td = 0;

    MS_U8 u8Count[E_AV_DEVICE_MAX] = {0};

    MS_U8 u8Volume = 0;

    ST_AUDIO_DEC_INFO stAudioInfo = {0};
    MS_U32 u32AudioSTCSYNCstatus = 0;

    // coverity[no_escape]
    while(1)
    {
        for (eDevice = 0; eDevice < E_AV_DEVICE_MAX; eDevice++)
        {
            if(stAVInfo[eDevice].bEnableAVSYNC_Task != FALSE)
            {
                switch(stAVInfo[eDevice].eAVSYNC_Task_State)
                {

                    case E_AV_AVSYNC_STATE_RESET:

                            u64Stc = INVALID_PTS;
                            u64Pts = INVALID_PTS;
                            u64Vfpts = INVALID_PTS;
                            u64Afpts = INVALID_PTS;

                            s64Td = 0;
                            u8Count[eDevice] = 0;

                            u8Volume = 0;

                            stAVInfo[eDevice].eAVSYNC_Task_State = E_AV_AVSYNC_STATE_IDLE;

                            break;

                    case E_AV_AVSYNC_STATE_IDLE:

                            if(stAVInfo[eDevice].eAVSYNC_Mode == E_AV_SYNC_VIDEO_MASTER_MODE)
                            {
                                if(stAVInfo[eDevice].bInited == TRUE)
                                {
                                    stAVInfo[eDevice].eAVSYNC_Task_State = E_AV_AVSYNC_STATE_INIT;
                                }
                            }
                            else
                            {
                                stAVInfo[eDevice].eAVSYNC_Task_State = E_AV_AVSYNC_STATE_INIT;
                            }
                            break;

                    case E_AV_AVSYNC_STATE_INIT:

                            u64Stc = Demo_AV_InitSTC(&eDevice);

                            if(u64Stc == INVALID_PTS)
                            {
                                //db_print("E_AV_AVSYNC_STATE_INIT : u64Stc = %"DTC_MS_U64_d"\n", u64Stc);
                                break;
                            }

                            if(stAVInfo[eDevice].eAVSYNC_Mode == E_AV_SYNC_AUDIO_MASTER_MODE)
                            {
                                stAVInfo[eDevice].eAVSYNC_Task_State = E_AV_AVSYNC_STATE_WAIT;
                            }
                            else
                            {
                                stAVInfo[eDevice].eAVSYNC_Task_State = E_AV_AVSYNC_STATE_MONITOR;
                            }

                            break;

                    case E_AV_AVSYNC_STATE_WAIT:

                            //mute audio
                            Demo_Audio_SetAbsoluteVolume(&u8Volume);

                            Demo_Audio_GetDecInfo(eDevice, &stAudioInfo);

                            // ask Audio DSP to sync STC
                            Demo_Audio_SyncSTC(stAudioInfo.eDecID, TRUE);

                            // enable sync STC flag
                            Demo_Audio_GetSyncSTCStatus(stAudioInfo.eDecID, &u32AudioSTCSYNCstatus);

                            if(u32AudioSTCSYNCstatus == 0)
                            {
                                //db_print("\033[35m  Audio_infoType_syncSTC_in_mmTs break\033[0m\n");
                                break;
                            }

                            // disable sync STC flag
                            Demo_Audio_SyncSTC(stAudioInfo.eDecID, FALSE);

                            //unmute audio
                            u8Volume = DEFAULT_VOLUME;
                            Demo_Audio_SetAbsoluteVolume(&u8Volume);

                            stAVInfo[eDevice].eAVSYNC_Task_State = E_AV_AVSYNC_STATE_MONITOR;

                            break;
                    case E_AV_AVSYNC_STATE_MONITOR:

                            u64Stc = Demo_AV_GetSTC(&eDevice);

                            switch(stAVInfo[eDevice].eAVSYNC_Mode)
                            {
                                case E_AV_SYNC_VIDEO_MASTER_MODE:
                                    Demo_VDEC_GetVideoPTS((EN_VDEC_Device*)&eDevice,NULL,&u64Vfpts);
                                    u64Pts = u64Vfpts;
                                    break;
                                case E_AV_SYNC_AUDIO_MASTER_MODE:
                                    {
                                        MS_U8 u8DecStatus = 0;
                                        Demo_Audio_GetDecInfo(eDevice, &stAudioInfo);
                                        Demo_Audio_GetDecStatus(stAudioInfo.eDecID, &u8DecStatus);

                                        if (u8DecStatus)
                                        {
                                            Demo_Audio_GetCPTS(stAudioInfo.eDecID, &u64Afpts);
                                            u64Pts = u64Afpts / 90;
                                        }
                                    }
                                    break;
                                default:
                                    break;
                            }

                            if(u64Pts == INVALID_PTS || u64Stc == INVALID_PTS)
                            {
                                db_print("E_AV_AVSYNC_STATE_MONITOR : u64Pts = %"DTC_MS_U64_d"\n", u64Pts);
                                db_print("E_AV_AVSYNC_STATE_MONITOR : u64Stc = %"DTC_MS_U64_d"\n", u64Stc);
                                break;
                            }

                            s64Td = (MS_S64)(u64Stc - u64Pts);

                            if((s64Td > 33) || (s64Td < -33))
                            {
                                u8Count[eDevice]++;
                            }
                            else
                            {
                                u8Count[eDevice] = 0;
                            }

                            //if 2 continous Td are bigger or smaller than 33ms then STC update by VPTS (follow MM algorithm)
                            if(u8Count[eDevice] == 2)
                            {
                                //update PTS -> STC
                                Demo_AV_SetSTC(&eDevice,u64Pts);
                                u8Count[eDevice] = 0;
                            }

                            break;
                    default:
                            break;

                }
            }
        }

        MsOS_DelayTask(1);
    }

}

//------------------------------------------------------------------------------
/// @brief create SYNC state machine Task
/// @param[in] Main or Sub Device or Other Device
/// @return none
/// @sa
/// @note
/// Command: \b none \n
//------------------------------------------------------------------------------
static MS_BOOL _Demo_AV_SYNCCreateTask(EN_AV_Device eDevice)
{
    MS_S32 s32MstarCachedPoolID = 0;

    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_CACHE,&s32MstarCachedPoolID);

    if(AV_SYNC_Task.iId == INVALID_TASK_ID)
    {
        AV_SYNC_Task.pStack = MsOS_AllocateMemory(AV_SYNC_Task.u32StackSize, s32MstarCachedPoolID);
        if(!AV_SYNC_Task.pStack)
        {
            printf("allocate stack fail\n");
            return FALSE;
        }
        AV_SYNC_Task.iId = MsOS_CreateTask((TaskEntry)_Demo_AV_SYNCTASK,
                                            eDevice,
                                            AV_SYNC_Task.ePriority,
                                            TRUE,
                                            AV_SYNC_Task.pStack,
                                            AV_SYNC_Task.u32StackSize,
                                            AV_SYNC_Task.szName);

        if(AV_SYNC_Task.iId == INVALID_TASK_ID )
        {
            printf("[%s][%d] Create AV SYNC Task Failed INVALID_TASK_ID \n",__FUNCTION__,__LINE__);
            return FALSE;
        }
        else
        {
            db_print("[%s][%d] s32_AVSYNCTaskID %"DTC_MS_U32_d" \n",__FUNCTION__,__LINE__,AV_SYNC_Task.iId);
            return TRUE;
        }
    }

    return TRUE;
}

MS_BOOL Demo_AV_SetLanguage(EN_AV_Device* peDevice, MS_U32 *u32LangIndex)
{
    DEMO_AV_CHECK_DEVICE_ID(*peDevice)

    MS_U32 i=0;
    MS_U32 u32PgNum=0;
    st_PG *pstPG = NULL;
    st_PG *pstcurrPG = NULL;
    MS_U32 u32VideoPid =stAVInfo[*peDevice].stVideoParams.u16PID;
    MS_U32 u32AudioPid =stAVInfo[*peDevice].stAudioParams.u16PID;
    MS_U32 u32PCRPid = stAVInfo[*peDevice].stPCRParams.u16PID;
    EN_DEMO_DMX_FLOW eDmxFlow = stAVInfo[*peDevice].stDMXflowParams.eFlow;

    if(stAVInfo[*peDevice].bInited == FALSE)
    {
        printf("[%s][%d] DTV %d not init\n", __FUNCTION__, __LINE__, *peDevice);
        return FALSE;
    }

    if(*u32LangIndex >= MAX_AUDIO_PG)
    {
        printf("[%s][%d] invalid language index = %"DTC_MS_U32_d"\n", __FUNCTION__, __LINE__, *u32LangIndex);
        return FALSE;
    }

    if(Demo_DMX_Scan(eDmxFlow, TRUE)==TRUE)
    {
        Demo_DMX_GetProgramInfo(&pstPG, &u32PgNum);
        for (i= 0; i< u32PgNum; i++)
        {
            if( ( (u32PCRPid == INVALID_PID_ID)||(u32PCRPid == (pstPG+i)->u32PidPcr) ) &&
                ( (u32VideoPid == INVALID_PID_ID)||(u32VideoPid == (pstPG+i)->u32PidVideo) ) &&
                ( (u32AudioPid == INVALID_PID_ID)||Demo_DMX_CheckAudioPgExist(pstPG+i, u32AudioPid)) )
            {
                pstcurrPG = pstPG+i;

                //get pid from u32LangIndex
                if(pstcurrPG->Audio_PG[*u32LangIndex].bISO_Lang_exist)
                {
                    stAVInfo[*peDevice].stAudioParams.u16PID = pstcurrPG->Audio_PG[*u32LangIndex].u32PidAudio;
                    stAVInfo[*peDevice].stAudioParams.eAudioCodec = _Demo_AV_SI_Audio_CodecType_Mapping(pstcurrPG->Audio_PG[*u32LangIndex].eAudioFmt);
                    db_print("[%s][%d] set PID = 0x%04"DTC_MS_U32_x", Lang = %s\n", __FUNCTION__, __LINE__, pstcurrPG->Audio_PG[*u32LangIndex].u32PidAudio, pstcurrPG->Audio_PG[*u32LangIndex].u8Lang);
                }
                else if(pstcurrPG->Audio_PG[*u32LangIndex].bSupplematary_Audio_exist)
                {
                    stAVInfo[*peDevice].stAudioParams.u16PID = pstcurrPG->Audio_PG[*u32LangIndex].u32PidAudio;
                    stAVInfo[*peDevice].stAudioParams.eAudioCodec = _Demo_AV_SI_Audio_CodecType_Mapping(pstcurrPG->Audio_PG[*u32LangIndex].eAudioFmt);
                    db_print("[%s][%d] set PID = 0x%04"DTC_MS_U32_x", Lang = %s\n", __FUNCTION__, __LINE__, pstcurrPG->Audio_PG[*u32LangIndex].u32PidAudio, pstcurrPG->Audio_PG[*u32LangIndex].u8Supplementary_Lang);
                }
                else
                {
                    printf("[%s][%d] invalid u32LangIndex = %"DTC_MS_U32_d"\n", __FUNCTION__, __LINE__, *u32LangIndex);
                    return FALSE;
                }
            }
        }
    }

    MS_U32 u32AudioPID = stAVInfo[*peDevice].stAudioParams.u16PID;

    Demo_AV_ChangeAudioTrack(peDevice, &u32AudioPID, &stAVInfo[*peDevice].stAudioParams.eAudioCodec);

    return TRUE;
}

MS_BOOL Demo_AV_LanguageList(EN_AV_Device* peDevice)
{
    DEMO_AV_CHECK_DEVICE_ID(*peDevice)

    MS_U32 i=0, j=0;
    MS_U32 u32PgNum=0;
    st_PG *pstPG = NULL;
    st_PG *pstcurrPG = NULL;
    MS_U32 u32VideoPid =stAVInfo[*peDevice].stVideoParams.u16PID;
    MS_U32 u32AudioPid =stAVInfo[*peDevice].stAudioParams.u16PID;
    MS_U32 u32PCRPid = stAVInfo[*peDevice].stPCRParams.u16PID;
    EN_DEMO_DMX_FLOW eDmxFlow = stAVInfo[*peDevice].stDMXflowParams.eFlow;

    if(stAVInfo[*peDevice].bInited == FALSE)
    {
        printf("[%s][%d] DTV %d not init\n", __FUNCTION__, __LINE__, *peDevice);
        return FALSE;
    }

    if(Demo_DMX_Scan(eDmxFlow, TRUE)==TRUE)
    {
        Demo_DMX_GetProgramInfo(&pstPG, &u32PgNum);

        for (i= 0; i< u32PgNum; i++)
        {
            if( ( (u32PCRPid == INVALID_PID_ID)||(u32PCRPid == (pstPG+i)->u32PidPcr) ) &&
                ( (u32VideoPid == INVALID_PID_ID)||(u32VideoPid == (pstPG+i)->u32PidVideo) ) &&
                ( (u32AudioPid == INVALID_PID_ID)||Demo_DMX_CheckAudioPgExist(pstPG+i, u32AudioPid)) )
            {
                printf("[%s][%d] start =============================================================\n", __FUNCTION__, __LINE__);
                pstcurrPG = pstPG+i;
                for (j=0;j<MAX_AUDIO_PG;j++)
                {
                    if ((pstcurrPG->Audio_PG[j].bISO_Lang_exist) || (pstcurrPG->Audio_PG[j].bSupplematary_Audio_exist))
                    {
                        printf("[%s][%d]    index = %"DTC_MS_U32_d", pid = 0x%04"DTC_MS_U32_d"", __FUNCTION__, __LINE__, j, pstcurrPG->Audio_PG[j].u32PidAudio);

                        //print language
                        if(strcmp((char*)pstcurrPG->Audio_PG[j].u8Lang, "") != 0)
                        {
                            printf(", Lang = %s", pstcurrPG->Audio_PG[j].u8Lang);
                        }
                        else if(strcmp((char*)pstcurrPG->Audio_PG[j].u8Supplementary_Lang, "") != 0)
                        {
                            printf(", Lang = %s", pstcurrPG->Audio_PG[j].u8Supplementary_Lang);
                        }
                        else
                        {
                            printf(", Lang = N/A");
                        }

                        //print AD
                        if ((pstcurrPG->Audio_PG[j].u8Lang_audio_type == 0x03) || (pstcurrPG->Audio_PG[j].u8Supplementary_classification == 0x01))
                        {
                            printf(", AD = y");
                        }
                        else
                        {
                            printf(", AD = n");
                        }

                        //print if current language
                        if(u32AudioPid == pstcurrPG->Audio_PG[j].u32PidAudio)
                        {
                            printf(" (current language)\n");
                        }
                        else
                        {
                            printf(" \n");
                        }

                    }
                }
                printf("[%s][%d] end ===============================================================\n", __FUNCTION__, __LINE__);
            }
        }
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Initialize demo av global struct
/// @param[in] Main or Sub Device or Other Device
/// @return none
/// @sa
/// @note
/// Command: \b none \n
//------------------------------------------------------------------------------
MS_BOOL Demo_AV_Initialize(EN_AV_Device* peDevice)
{
    DEMO_AV_CHECK_DEVICE_ID(*peDevice)

    if(stAVInfo[*peDevice].bInitializeOrNot == FALSE)
    {
        if(stAVInfo[*peDevice].bInited == FALSE)
        {
            stAVInfo[*peDevice].bInited = FALSE;

            stAVInfo[*peDevice].stVideoParams.u8Filter = INVALID_FILTER_ID;
            stAVInfo[*peDevice].stVideoParams.ePCREngID = E_PCR_ENG_INVALID;
            stAVInfo[*peDevice].stVideoParams.u16PID = INVALID_PID_ID;
            stAVInfo[*peDevice].stVideoParams.eFIFO_ID = E_DMX_FLT_TYPE_INVALID;
            stAVInfo[*peDevice].stVideoParams.stFilterType.eEngType = INVALID_FILEIN_TYPE;
            stAVInfo[*peDevice].stVideoParams.stFilterType.u8EngID = INVALID_FILEIN_ID;
            stAVInfo[*peDevice].stVideoParams.stFilterType.eEngFormat = E_AV_FileIn_Eng_TS;
            memset(&stAVInfo[*peDevice].stVideoParams.stVDECSteamID,0, sizeof(VDEC_StreamId));
            stAVInfo[*peDevice].stVideoParams.eVideoCodec = E_VDEC_DDI_CODEC_TYPE_MPEG2;
            stAVInfo[*peDevice].stVideoParams.eAudioCodec = DEMO_AUDIO_CODEC_MPEG;

            memcpy(&stAVInfo[*peDevice].stAudioParams, &stAVInfo[*peDevice].stVideoParams, sizeof(ST_AV_Params));
            memcpy(&stAVInfo[*peDevice].stAudioADParams, &stAVInfo[*peDevice].stVideoParams, sizeof(ST_AV_Params));
            memcpy(&stAVInfo[*peDevice].stPCRParams, &stAVInfo[*peDevice].stVideoParams, sizeof(ST_AV_Params));
            memset(&stAVInfo[*peDevice].stDMXflowParams, 0, sizeof(ST_AV_DMX_INFO));

            stAVInfo[*peDevice].eAVSYNC_Mode = E_AV_SYNC_PCR_MODE;
            memset(&stAVInfo[*peDevice].stShowFrameInfo, 0, sizeof(ST_AV_ShowFrameInfo));

            stAVInfo[*peDevice].bEnableAVSYNC_Task = FALSE;
            stAVInfo[*peDevice].eAVSYNC_Task_State = E_AV_AVSYNC_STATE_RESET;

            stAVInfo[*peDevice].bInitializeOrNot = FALSE;
            stAVInfo[*peDevice].eAVInitState = E_AV_AVINIT_STATE_RESET;
        }
    }
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief SYNC mode to Init STC
/// @param[in] Main or Sub Device or Other Device
/// @return none
/// @sa
/// @note
/// Command: \b none \n
//------------------------------------------------------------------------------
MS_U64 Demo_AV_InitSTC(EN_AV_Device* peDevice)
{
    DEMO_AV_CHECK_DEVICE_ID(*peDevice)

    MS_U64 u64InitStc = INVALID_PTS;

    switch(stAVInfo[*peDevice].eAVSYNC_Mode)
    {
        case E_AV_SYNC_VIDEO_MASTER_MODE:
                Demo_VDEC_GetVideoPTS((EN_VDEC_Device*)peDevice,NULL,&u64InitStc);
            break;
        case E_AV_SYNC_AUDIO_MASTER_MODE:
                {
                    ST_AUDIO_DEC_INFO stAudioInfo = {0};
                    MS_U8 u8DecStatus = 0;
                    Demo_Audio_GetDecInfo(*peDevice, &stAudioInfo);
                    Demo_Audio_GetDecStatus(stAudioInfo.eDecID, &u8DecStatus);
                    if (u8DecStatus)
                    {
                        Demo_Audio_GetCPTS(stAudioInfo.eDecID, &u64InitStc);
                        u64InitStc = u64InitStc / 90;
                    }
                }
                break;
        default:
            break;
    }

    if(u64InitStc == INVALID_PTS || u64InitStc == 0)
    {
        //db_print("return u64InitStc = %"DTC_MS_U64_d"\n", u64InitStc);
        return INVALID_PTS;
    }

    db_print("STC : u64InitStc = %"DTC_MS_U64_d"\n", u64InitStc);

    if(Demo_AV_SetSTC(peDevice,u64InitStc) == FALSE)
    {
        printf("Demo_AV_SetSTC fail \n");
        return INVALID_PTS;
    }

    return u64InitStc;
}

//------------------------------------------------------------------------------
/// @brief SYNC mode to Set STC
/// @param[in] Main or Sub Device or Other Device
/// @param[in] u64Stc (unit:ms)
/// @return none
/// @sa
/// @note
/// Command: \b none \n
//------------------------------------------------------------------------------
MS_BOOL Demo_AV_SetSTC(EN_AV_Device* peDevice,MS_U64 u64Stc)
{
    DEMO_AV_CHECK_DEVICE_ID(*peDevice)

    MS_U32 u32StcH =0;
    MS_U32 u32Stc = 0;

    u64Stc = u64Stc*90;

    u64Stc &= 0x1FFFFFFFFLL;

    u32StcH = (MS_U32)((u64Stc >> 32) & 0xFFFFFFFF);
    u32Stc = (MS_U32)(u64Stc & 0xFFFFFFFF);

    if(MApi_DMX_Stc_Eng_Set((MS_U8)stAVInfo[*peDevice].stPCRParams.ePCREngID, u32StcH, u32Stc)!= DMX_FILTER_STATUS_OK)
    {
        printf("[%s][%d] MApi_DMX_Stc_Set Fail\n",__FUNCTION__,__LINE__);
        return FALSE;
    }


    db_print("eDevice : %d : PTS -> STC, u64Stc=%"DTC_MS_U64_d"\n",*peDevice, u64Stc/90);
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief SYNC mode to Get STC
/// @param[in] Main or Sub Device or Other Device
/// @return u64Stc (unit:ms)
/// @sa
/// @note
/// Command: \b none \n
//------------------------------------------------------------------------------
MS_U64 Demo_AV_GetSTC(EN_AV_Device* peDevice)
{
    DEMO_AV_CHECK_DEVICE_ID(*peDevice)

    MS_U32 u32StcH = 0;
    MS_U32 u32Stc = 0;
    MS_U64 u64Stc = 0;

    if(MApi_DMX_Stc_Eng_Get((MS_U8)stAVInfo[*peDevice].stPCRParams.ePCREngID, &u32StcH, &u32Stc) != DMX_FILTER_STATUS_OK)
    {
        printf("[%s][%d] MApi_DMX_Stc_Get Fail\n",__FUNCTION__,__LINE__);
        return INVALID_PTS;
    }

    u64Stc = ((MS_U64)u32StcH << 32) | u32Stc;
    u64Stc &= 0x1FFFFFFFFLL;

    u64Stc = u64Stc/90;

    return u64Stc;
}

//------------------------------------------------------------------------------
/// @brief open the SYNC mode
/// @param[in] Main or Sub Device or Other Device
/// @return none
/// @sa
/// @note
/// Command: \b none \n
//------------------------------------------------------------------------------
MS_BOOL Demo_AV_SetSYNC(EN_AV_Device* peDevice)
{
    DEMO_AV_CHECK_DEVICE_ID(*peDevice)

    stAVInfo[*peDevice].eAVSYNC_Task_State = E_AV_AVSYNC_STATE_RESET;
    stAVInfo[*peDevice].bEnableAVSYNC_Task = TRUE;

    if(AV_SYNC_Task.iId == INVALID_TASK_ID)
    {
        if(!_Demo_AV_SYNCCreateTask(*peDevice))
        {
            printf("[%s][%d]create SYNC task fail!!!!!!!!!!! OH NO \n",__FUNCTION__,__LINE__);
            return FALSE;
        }
    }
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief if other program wants AV info that can use this to get info
/// @param[in] Main or Sub Device or Other Device
/// @param[in] EN_AV_Cmd which you want get
/// @param[out] pAVInfo
///     pointer to output buffer
/// @return TRUE: Process success.
/// @return FALSE: Invalid input argument.
/// @sa
/// @note
/// Command: \b none \n
//------------------------------------------------------------------------------
MS_BOOL Demo_AV_GetAVInfo(EN_AV_Device* peDevice,EN_AV_Cmd eAVCmd,void* pAVInfo)
{
    DEMO_AV_CHECK_DEVICE_ID(*peDevice)

    switch(eAVCmd)
    {
        case E_AV_GetCmd_LiveInfo:
            memcpy((ST_AV_INFO*)pAVInfo, &stAVInfo[*peDevice], sizeof(ST_AV_INFO));
            return TRUE;
        case E_AV_GetCmd_VideoInfo:
            memcpy((ST_AV_Params*)pAVInfo, &stAVInfo[*peDevice].stVideoParams, sizeof(ST_AV_Params));
            return TRUE;
        case E_AV_GetCmd_AudioInfo:
            memcpy((ST_AV_Params*)pAVInfo, &stAVInfo[*peDevice].stAudioParams, sizeof(ST_AV_Params));
            return TRUE;
        case E_AV_GetCmd_AudioADInfo:
            memcpy((ST_AV_Params*)pAVInfo, &stAVInfo[*peDevice].stAudioADParams, sizeof(ST_AV_Params));
            return TRUE;
        case E_AV_GetCmd_PCRInfo:
            memcpy((ST_AV_Params*)pAVInfo, &stAVInfo[*peDevice].stPCRParams, sizeof(ST_AV_Params));
            return TRUE;
        case E_AV_GetCmd_DMXFlowInfo:
            memcpy((ST_AV_DMX_INFO*)pAVInfo, &stAVInfo[*peDevice].stDMXflowParams, sizeof(ST_AV_DMX_INFO));
            return TRUE;
        case E_AV_GetCmd_WindowInfo:
            {
                int i ;
                for(i = 0;i < E_AV_DEVICE_MAX;i++)
                {
                    if(stAVInfo[i].stShowFrameInfo.u32Window == (MS_U32)*peDevice && stAVInfo[i].stShowFrameInfo.bValid)
                    {
                        if(stAVInfo[i].stShowFrameInfo.u16Width != 0)
                        {
                            memcpy((ST_AV_ShowFrameInfo*)pAVInfo, &stAVInfo[i].stShowFrameInfo, sizeof(ST_AV_ShowFrameInfo));
                            return TRUE;
                        }
                        else
                        {
                            ST_AV_ShowFrameInfo stTempShowFrameInfo = {0};
                            memcpy(&stTempShowFrameInfo, &stAVInfo[i].stShowFrameInfo, sizeof(ST_AV_ShowFrameInfo));

                            stTempShowFrameInfo.u16Width  = IPANEL(&stPNL_DeviceId, Width);
                            stTempShowFrameInfo.u16Height = IPANEL(&stPNL_DeviceId, Height);
                            memcpy((ST_AV_ShowFrameInfo*)pAVInfo, &stTempShowFrameInfo, sizeof(ST_AV_ShowFrameInfo));

                            printf("\033[35m [%s][%d] E_AV_GetCmd_WindowInfo default window size: u16Width[%d] u16Height[%d]\033[0m\n",__FUNCTION__,__LINE__,stTempShowFrameInfo.u16Width,stTempShowFrameInfo.u16Height);
                            return TRUE;
                        }
                    }
                }

                memset((ST_AV_ShowFrameInfo*)pAVInfo, 0, sizeof(ST_AV_ShowFrameInfo));
                return FALSE;
            }
        case E_AV_GetCmd_WindowShowFrameMode:
            {
                int i ;
                for(i = 0;i < E_AV_DEVICE_MAX;i++)
                {
                    if(stAVInfo[i].stShowFrameInfo.u32Window == (MS_U32)*peDevice && stAVInfo[i].stShowFrameInfo.bValid)
                    {
                        memcpy((EN_AV_ShowFrame_Mode*)pAVInfo, &stAVInfo[i].stShowFrameInfo.eShowFrameMode, sizeof(EN_AV_ShowFrame_Mode));
                        return TRUE;
                    }
                }
                memset((EN_AV_ShowFrame_Mode*)pAVInfo, 0, sizeof(EN_AV_ShowFrame_Mode));
                return FALSE;
            }
        case E_AV_GetCmd_MAX:
        default:
            printf("[%s][%d] wrong eAVCmd:%d!\n", __FUNCTION__, __LINE__,eAVCmd);
            return FALSE;
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief if other program wants AV info that can use this to set AV info
/// @param[in] Main or Sub Device or Other Device
/// @param[in] eAVCmd enum
/// @param[in] Value of AVInfo element
/// @return TRUE: Process success.
/// @return FALSE: Invalid input argument.
/// @sa
/// @note
/// Command: \b none \n
//------------------------------------------------------------------------------
MS_BOOL Demo_AV_SetAVInfo(EN_AV_Device* peDevice,EN_AV_Cmd eAVCmd,void* pAVInfo)
{

    DEMO_AV_CHECK_DEVICE_ID(*peDevice)

    switch(eAVCmd)
    {
        case E_AV_SetCmd_DMXFlow:
            stAVInfo[*peDevice].stDMXflowParams.eFlow = *((EN_DEMO_DMX_FLOW *)pAVInfo);
            return TRUE;
        case E_AV_SetCmd_DMXInput:
            stAVInfo[*peDevice].stDMXflowParams.eDmxInput= *((EN_DEMO_DMX_FLOW_INPUT *)pAVInfo);
            return TRUE;
        case E_AV_SetCmd_NotInitialize:
            stAVInfo[*peDevice].bInitializeOrNot= *((MS_BOOL *)pAVInfo);
            return TRUE;
        case E_AV_SetCmd_MAX:
        default:
            printf("[%s][%d] wrong eAVCmd:%d!\n", __FUNCTION__, __LINE__,eAVCmd);
            return FALSE;
    }
}


//------------------------------------------------------------------------------
/// @brief The sample code to set the av-sync mode
/// @param[in] Main or Sub Device or Other Device
/// @param[in] u8mode 0x0: av-sync mode
///                             0x1: audio master mode
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b DTV_SetAVSyncMode \n
//------------------------------------------------------------------------------
MS_BOOL Demo_AV_SetAVSYNCMode(EN_AV_Device* peDevice, EN_AV_AVSYNC_Type* peMode)
{
    DEMO_AV_CHECK_DEVICE_ID(*peDevice)

    stAVInfo[*peDevice].eAVSYNC_Mode = *peMode;

    printf("================Demo_AV_SetAVSYNCMode================\n");
    printf("0x0 : PCR_MODE\n");
    printf("0x1 : AUDIO_MASTER_MODE\n");
    printf("0x2 : VIDEO_MASTER_MODE\n");
    printf("==============End Demo_AV_SetAVSYNCMode==============\n");


    db_print("Demo_AV_SetAVSYNCMode :%d\n",stAVInfo[*peDevice].eAVSYNC_Mode);

    if(stAVInfo[*peDevice].eAVSYNC_Mode == E_AV_SYNC_VIDEO_MASTER_MODE ||
        stAVInfo[*peDevice].eAVSYNC_Mode == E_AV_SYNC_AUDIO_MASTER_MODE)
    {
        Demo_AV_SetSYNC(peDevice);
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to set the Decode mode & window
/// @param[in] Main or Sub Device or Other Device
/// @param[in] u8mode 0x0: Mvop1 mode
///                   0x1: Mvop2 mode
///                   0x2: DIP
///                   0x3: SW-detile + GOP
/// @param[in] Show Window
/// @param[in] X
/// @param[in] Y
/// @param[in] Width
/// @param[in] Height
/// @param[in] Layer
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b DTV_SetShowFrameMode \n
//------------------------------------------------------------------------------
MS_BOOL Demo_AV_SetShowFrameMode(EN_AV_Device* peDevice, MS_U32* pu32Mode, MS_U32* pu32Window, MS_U16* pu16X, MS_U16* pu16Y, MS_U16* pu16Width, MS_U16* pu16Height, MS_U16* pu16Layer)
{
    DEMO_AV_CHECK_DEVICE_ID(*peDevice)

    MS_U32 u32Window = *pu32Window;
    EN_VDEC_DDI_DISPLAY_PATH eDisplayPath = E_VDEC_DDI_DISPLAY_PATH_NONE;

    if(stAVInfo[*peDevice].bInited == TRUE
        || stAVInfo[*peDevice].stShowFrameInfo.bValid == TRUE)
    {
        goto ChangePosition;
    }

    printf("================Demo_AV_SetShowFrameMode [Device][ShowFrame_Type][Window][X][Y][Width][Height][Layer]================\n");
    printf("ShowFrame_Type 0x0 : MVOP1_MODE\n");
    printf("ShowFrame_Type 0x1 : MVOP2_MODE\n");
    printf("ShowFrame_Type 0x2 : DIP_MODE\n");
    printf("ShowFrame_Type 0x3 : SWDetile_MODE\n");
    printf("ShowFrame_Type 0x4 : PureMCU MODE\n");
    printf("Layer number choose your window layer \n");
    printf("==============End Demo_AV_SetShowFrameMode==============\n");

    if(*pu32Mode == 0x0 || *pu32Mode == 0x1)
    {
        stAVInfo[*peDevice].stShowFrameInfo.eShowFrameMode = E_AV_MVOP_MODE;
        if(*pu32Mode == 0x0)
            eDisplayPath = E_VDEC_DDI_DISPLAY_PATH_MVOP_MAIN;
        else if(*pu32Mode == 0x1)
            eDisplayPath = E_VDEC_DDI_DISPLAY_PATH_MVOP_SUB;
    }
    else
    {
        stAVInfo[*peDevice].stShowFrameInfo.eShowFrameMode = *pu32Mode;
        eDisplayPath = E_VDEC_DDI_DISPLAY_PATH_NONE;
    }

#if (DEMO_XC_DIPMultiWin_TEST == 1)
    if(stAVInfo[*peDevice].stShowFrameInfo.eShowFrameMode == E_AV_PureMCU_MODE
        || stAVInfo[*peDevice].stShowFrameInfo.eShowFrameMode == E_AV_DIP_MODE)
    {
        printf("\033[35m [%s][%d] msAPI_XC_DIPMultiView_Init init \033[0m\n",__FUNCTION__,__LINE__);
        msAPI_XC_DIPMultiView_Init(2, E_MSAPI_XC_DIP_FMT_YUV422);

        if(stAVInfo[*peDevice].stShowFrameInfo.eShowFrameMode == E_AV_PureMCU_MODE)
        {
            if(msAPI_XC_DIPMultiView_CreateWindow(NULL, &u32Window) == E_MSAPI_XC_OK)
                printf("\033[35m [%s][%d] Really Window : [%d]!!!!!!\033[0m\n",__FUNCTION__,__LINE__,u32Window);
            else
                u32Window = *pu32Window;
        }
    }
#endif

    stAVInfo[*peDevice].stShowFrameInfo.eShowFrame_MvopPath = eDisplayPath;

    stAVInfo[*peDevice].stShowFrameInfo.stVDECWinControl.u8EnableSetDigitalSignal = 0;
    stAVInfo[*peDevice].stShowFrameInfo.stVDECWinControl.u8EnableClearDigitalSignal = 0;
    stAVInfo[*peDevice].stShowFrameInfo.stVDECWinControl.win_id = stAVInfo[*peDevice].stShowFrameInfo.u32Window = u32Window;

ChangePosition:

    stAVInfo[*peDevice].stShowFrameInfo.bValid = TRUE;
    stAVInfo[*peDevice].stShowFrameInfo.eDevice = *peDevice;
    stAVInfo[*peDevice].stShowFrameInfo.u16X = *pu16X;
    stAVInfo[*peDevice].stShowFrameInfo.u16Y = *pu16Y;
    stAVInfo[*peDevice].stShowFrameInfo.u16Width = *pu16Width;
    stAVInfo[*peDevice].stShowFrameInfo.u16Height = *pu16Height;
    stAVInfo[*peDevice].stShowFrameInfo.u16Layer = *pu16Layer;

    Demo_VDEC_SetMode((EN_VDEC_Device *)peDevice,E_VDEC_SetCmd_ShowFrameInfo,(void *)(&stAVInfo[*peDevice].stShowFrameInfo));

    db_print("Demo_AV_SetShowFrame : %d\n",stAVInfo[*peDevice].stShowFrameInfo.eShowFrameMode);
    db_print("Demo_AV_SetShowFrameWindow : %d\n",stAVInfo[*peDevice].stShowFrameInfo.u32Window);
    //db_print("Demo_AV_SetShowMVOPPath : %d\n",eDisplayPath);

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to change window position
/// @param[in] Show window
/// @param[in] X
/// @param[in] Y
/// @param[in] Width
/// @param[in] Height
/// @param[in] Layer
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b DTV_ChangeDispWindow \n
//------------------------------------------------------------------------------
MS_BOOL Demo_AV_ChangeDispWindow(MS_U32* pu32Window, MS_U16* pu16X, MS_U16* pu16Y, MS_U16* pu16Width, MS_U16* pu16Height, MS_U16* pu16Layer)
{
    printf("================Demo_AV_ChangeDispWindow [Window][X][Y][Width][Height][Layer]================\n");

    MSAPI_XC_WINDOW_TYPE stDestWin = {0};
    ST_AV_ShowFrameInfo stAVShowFrameInfo = {0};

    Demo_AV_GetAVInfo((EN_AV_Device*)pu32Window,E_AV_GetCmd_WindowInfo,&stAVShowFrameInfo);
    EN_AV_Device eDevice = stAVShowFrameInfo.eDevice;

    stAVInfo[eDevice].stShowFrameInfo.eDevice = eDevice;
    stAVInfo[eDevice].stShowFrameInfo.u32Window = *pu32Window;
    stAVInfo[eDevice].stShowFrameInfo.u16X = *pu16X;
    stAVInfo[eDevice].stShowFrameInfo.u16Y = *pu16Y;
    stAVInfo[eDevice].stShowFrameInfo.u16Width = *pu16Width;
    stAVInfo[eDevice].stShowFrameInfo.u16Height = *pu16Height;
    stAVInfo[eDevice].stShowFrameInfo.u16Layer = *pu16Layer;

    Demo_VDEC_SetMode((EN_VDEC_Device *)&eDevice,E_VDEC_SetCmd_ShowFrameInfo,(void *)(&stAVInfo[eDevice].stShowFrameInfo));

    db_print("Demo_AV_SetShowFrame : %d\n",stAVInfo[eDevice].stShowFrameInfo.eShowFrameMode);
    db_print("Demo_AV_SetShowFrameWindow : %d bValid:%d\n",stAVInfo[eDevice].stShowFrameInfo.u32Window,stAVInfo[eDevice].stShowFrameInfo.bValid);

    //do setwin

    stDestWin.x = stAVInfo[eDevice].stShowFrameInfo.u16X;
    stDestWin.y = stAVInfo[eDevice].stShowFrameInfo.u16Y;
    stDestWin.width = stAVInfo[eDevice].stShowFrameInfo.u16Width;
    stDestWin.height = stAVInfo[eDevice].stShowFrameInfo.u16Height;


    if(*pu16Width == 0 || *pu16Height == 0)
    {
        stDestWin.width  = IPANEL(&stPNL_DeviceId, Width);
        stDestWin.height = IPANEL(&stPNL_DeviceId, Height);
    }

    if(stAVInfo[eDevice].stShowFrameInfo.eShowFrameMode == E_AV_DIP_MODE
        || stAVInfo[eDevice].stShowFrameInfo.eShowFrameMode == E_AV_PureMCU_MODE)
    {
        db_print("DIP or PureMCU_MODE!!!!!!!!!!!\n");

        VDEC_EX_DispInfo stDispinfo;
        memset(&stDispinfo, 0, sizeof(VDEC_EX_DispInfo));
        Demo_VDEC_GetDispInfo((EN_VDEC_Device *)&eDevice,&stDispinfo);

        MSAPI_XC_WINDOW_TYPE stCropWin = {0};

        stCropWin.x = 0;
        stCropWin.y = 0;
        stCropWin.width  = stDispinfo.u16HorSize - (stDispinfo.u16CropRight + stDispinfo.u16CropLeft);
        stCropWin.height = stDispinfo.u16VerSize - (stDispinfo.u16CropTop + stDispinfo.u16CropBottom);

        MS_BOOL bDisplayMute = TRUE;
        Demo_AV_MuteVideo(&eDevice,&bDisplayMute);

        MSAPI_XC_DEVICE_ID g_stDIP_DeviceId = {0, E_MSAPI_XC_DEVICE_DIP_0};
        msAPI_XC_SetWin_EX(&g_stDIP_DeviceId, NULL, &stCropWin, &stDestWin, stAVInfo[eDevice].stShowFrameInfo.u32Window);

        bDisplayMute = FALSE;
        Demo_AV_MuteVideo(&eDevice,&bDisplayMute);

    }
    else if(stAVInfo[eDevice].stShowFrameInfo.eShowFrameMode == E_AV_MVOP_MODE)
    {
        db_print("SC !!!!!!!!!!!\n");
        MS_BOOL bDisplayMute = TRUE;
        Demo_AV_MuteVideo(&eDevice,&bDisplayMute);

        MSAPI_XC_DEVICE_ID g_stXC_DeviceId = {0, E_MSAPI_XC_DEVICE0};
        msAPI_XC_SetWin_EX(&g_stXC_DeviceId, NULL, NULL, &stDestWin, stAVInfo[eDevice].stShowFrameInfo.u32Window);

        bDisplayMute = FALSE;
        Demo_AV_MuteVideo(&eDevice,&bDisplayMute);
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Set Demux flow
/// @param[in] Main or Sub Device
/// @param[in] flow DMX flow
///        0: E_DMX_FLOW_LIVE0
///        1: E_DMX_FLOW_LIVE1
///        2: E_DMX_FLOW_LIVE2
///        3: E_DMX_FLOW_LIVE3
/// @param[in] pInsrc DMX flow input
///        0: E_DMX_FLOW_INPUT_DEMOD0
///        1: E_DMX_FLOW_INPUT_DEMOD1
///        2: E_DMX_FLOW_INPUT_MEM
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
/// Command: \b DTV_Tuner_Config 0 0 0 0 0 0 \n
//------------------------------------------------------------------------------
MS_BOOL Demo_AV_Tuner_Config(
    EN_AV_Device* peDevice,
    EN_DEMO_DMX_FLOW* pePlayback,
    EN_DEMO_DMX_FLOW_INPUT* peDmxInput,
    int* pClkInv,
    int* pExtSync,
    int* pParal)
{
    DEMO_AV_CHECK_DEVICE_ID(*peDevice)

    Demo_AV_Initialize(peDevice);
    Demo_VDEC_SetMode((EN_VDEC_Device *)peDevice,E_VDEC_SetCmd_Initialize,(void *)TRUE);

    MS_BOOL bClkInv;
    MS_BOOL bExtSync;
    MS_BOOL bParallel;

    if ((pClkInv == NULL) || (pExtSync == NULL) || (pParal == NULL)|| (pePlayback == NULL)||(peDmxInput == NULL))
    {
        printf("Can not enter space\n");
        return FALSE;
    }

    printf("========================================================\nplease enter(0/1):\n1. Device:%d\n2. Playpack:%d\n3. Tuner_Idx:%d\n4. Clk inv:%d\n5. ExtSync:%d\n6. Parallel:%d\n========================================================\n",
    *peDevice,*pePlayback,*peDmxInput,*pClkInv,*pExtSync,*pParal);

    bClkInv = (0 == *pClkInv)? FALSE: TRUE;
    bExtSync = (0 == *pExtSync)? FALSE: TRUE;
    bParallel = (0 == *pParal)? FALSE: TRUE;

    stAVInfo[*peDevice].stDMXflowParams.eFlow = *pePlayback;
    stAVInfo[*peDevice].stDMXflowParams.eDmxInput = *peDmxInput;

    stAVInfo[*peDevice].stDMXflowParams.DmxClkInv = *pClkInv;
    stAVInfo[*peDevice].stDMXflowParams.DmxExtSync = *pExtSync;
    stAVInfo[*peDevice].stDMXflowParams.DmxParallal = *pParal;


    if ( TRUE != Demo_DMX_FlowSet(stAVInfo[*peDevice].stDMXflowParams.eFlow, *peDmxInput, bClkInv, bExtSync, bParallel))
    {
        printf("Demo_DMX_FlowSet fail\n");
        return FALSE;
    }

    stAVInfo[*peDevice].eAVInitState = E_AV_AVINIT_STATE_TUNER;

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Set Audio pid to device decoder path
/// @param[in] Main or Sub Device
/// @param[in] AudioPid Audio PID
/// @param[in] pu32ACodec Audio Codec Type
/// @return TRUE: success.
/// @return FALSE: Process fail or Invalid input argument.
/// @sa
/// @note
/// Command: \b DTV_TSP_SetAudioPid 0 102 0 \n
//------------------------------------------------------------------------------
MS_BOOL Demo_AV_TSP_SetAudioPid(EN_AV_Device* peDevice,MS_U32* pu32AudioPid, MS_U32* pu32ACodec)
{
    DEMO_AV_CHECK_DEVICE_ID(*peDevice)

    EN_DEMO_DMX_FLOW eDMX_FLOW = stAVInfo[*peDevice].stDMXflowParams.eFlow;

    db_print("%d\n",*peDevice);
    db_print("%"DTC_MS_U32_d"\n",*pu32AudioPid);
    db_print("%"DTC_MS_U32_d"\n",*pu32ACodec);

    stAVInfo[*peDevice].stAudioParams.u16PID = (*pu32AudioPid) & 0x1FFF;
    stAVInfo[*peDevice].stAudioParams.eAudioCodec = (*pu32ACodec) & 0xFF;
    stAVInfo[*peDevice].stAudioParams.stFilterType.eEngFormat = E_AV_FileIn_Eng_TS;

    if (MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Get_MultiPlayer_Capability) == 1)
    {
        stAVInfo[*peDevice].stAudioParams.eFIFO_ID = Demo_AV_GetAudioFIFO(peDevice);
    }
    else
    {
        stAVInfo[*peDevice].stAudioParams.eFIFO_ID = E_DMX_FLT_TYPE_AUD0;
    }

    if (Demo_AV_CheckAudioResource(peDevice) == TRUE)
    {
        //Audio FIFO reset
        if (TRUE != Demo_DMX_AVFifo_Reset(stAVInfo[*peDevice].stAudioParams.eFIFO_ID,TRUE))
        {
            printf("Demo DMX AudioFIFO Reset fail\n");
            goto Demo_AV_SetPid_fail;
        }

        //stop audio pidflt
        if (stAVInfo[*peDevice].stAudioParams.u8Filter != INVALID_FILTER_ID)
        {
            MApi_DMX_Stop(stAVInfo[*peDevice].stAudioParams.u8Filter);
            MApi_DMX_Close(stAVInfo[*peDevice].stAudioParams.u8Filter);
            stAVInfo[*peDevice].stAudioParams.u8Filter = INVALID_FILTER_ID;
            db_print("MApi_DMX_Stop AudioFilter:%d\n",stAVInfo[*peDevice].stAudioParams.u8Filter);
        }

        //open audio pidflt
        if (TRUE != Demo_DMX_FltOpen( eDMX_FLOW, stAVInfo[*peDevice].stAudioParams.eFIFO_ID, &stAVInfo[*peDevice].stAudioParams.u8Filter, stAVInfo[*peDevice].stAudioParams.u16PID))
        {
            printf("Demo DMX AudioFlt Open fail \n");
            goto Demo_AV_SetPid_fail;
        }

        //start audio pidflt
        if (DMX_FILTER_STATUS_OK != MApi_DMX_Start(stAVInfo[*peDevice].stAudioParams.u8Filter))
        {
            printf("MApi DMX AudioFlt start fail \n");
            goto Demo_AV_SetPid_fail;
        }
    }
    else
    {
        printf("No Audio resource, Device[%d] no need set pid flow\n", *peDevice);
    }

    // For Merge Stream. If set pid success, store current source id in AVInfo.
    stAVInfo[*peDevice].stDMXflowParams.eDmxFltSourceID = Demo_DMX_GetSrcID(stAVInfo[*peDevice].stDMXflowParams.eFlow);

    db_print("Device filters are ready\n");

    return TRUE;

Demo_AV_SetPid_fail:

    printf("Demo_AV_SetAudioPid_fail \n");

    if (stAVInfo[*peDevice].stAudioParams.u8Filter != INVALID_FILTER_ID)
    {
        MApi_DMX_Stop(stAVInfo[*peDevice].stAudioParams.u8Filter);
        MApi_DMX_Close(stAVInfo[*peDevice].stAudioParams.u8Filter);
        stAVInfo[*peDevice].stAudioParams.u8Filter = INVALID_FILTER_ID;
    }

    return FALSE;
}

//------------------------------------------------------------------------------
/// @brief Set Video pid to device decoder path
/// @param[in] Main or Sub Device
/// @param[in] VideoPid Video PID
/// @param[in] PCRPid PCR PID
/// @param[in] pu32VCodec Video Codec (0: MPEG 1: H.264 2:AVS 3:HEVC)
/// @return TRUE: success.
/// @return FALSE: Process fail or Invalid input argument.
/// @sa
/// @note
/// Command: \b DTV_TSP_SetVideoPid 0 101 0 \n
//------------------------------------------------------------------------------
MS_BOOL Demo_AV_TSP_SetVideoPid(EN_AV_Device* peDevice,MS_U32* pu32VideoPid, MS_U32* pu32VCodec)
{
    DEMO_AV_CHECK_DEVICE_ID(*peDevice)

    EN_DEMO_DMX_FLOW eDMX_FLOW = stAVInfo[*peDevice].stDMXflowParams.eFlow;

    db_print("%d\n",*peDevice);
    db_print("%"DTC_MS_U32_d"\n",*pu32VideoPid);
    db_print("%"DTC_MS_U32_d"\n",*pu32VCodec);

#if(DEMO_VDEC_NDECODE_TEST == 1)
    stAVInfo[*peDevice].stVideoParams.eFIFO_ID = *peDevice;
#else
    if (*peDevice == E_AV_DEVICE_MAIN)
    {
        stAVInfo[*peDevice].stVideoParams.eFIFO_ID = E_DMX_FLT_TYPE_VID0;
    }
#if(DEMO_VDEC_MULTI_TEST == 1)
    else if (*peDevice == E_AV_DEVICE_SUB)
    {
        stAVInfo[*peDevice].stVideoParams.eFIFO_ID = E_DMX_FLT_TYPE_VID1;
    }
#endif
#endif

    stAVInfo[*peDevice].stVideoParams.u16PID = (*pu32VideoPid) & 0x1FFF;
    stAVInfo[*peDevice].stVideoParams.eVideoCodec = (*pu32VCodec) & 0xFF;
    stAVInfo[*peDevice].stVideoParams.stFilterType.eEngFormat = E_AV_FileIn_Eng_TS;

    //start video FIFO Reset
    if (TRUE != Demo_DMX_AVFifo_Reset(stAVInfo[*peDevice].stVideoParams.eFIFO_ID,TRUE))
    {
        printf("Demo DMX VideoFIFO Reset fail\n");
        goto Demo_AV_SetPid_fail;
    }

    //stop video pidflt
    if (stAVInfo[*peDevice].stVideoParams.u8Filter != INVALID_FILTER_ID)
    {
        MApi_DMX_Stop(stAVInfo[*peDevice].stVideoParams.u8Filter);
        MApi_DMX_Close(stAVInfo[*peDevice].stVideoParams.u8Filter);
        stAVInfo[*peDevice].stVideoParams.u8Filter = INVALID_FILTER_ID;
        db_print("MApi_DMX_Stop VideoFilter:%d\n",stAVInfo[*peDevice].stVideoParams.u8Filter);
    }

    //open video pidflt
    if (TRUE != Demo_DMX_FltOpen( eDMX_FLOW, stAVInfo[*peDevice].stVideoParams.eFIFO_ID, &stAVInfo[*peDevice].stVideoParams.u8Filter, stAVInfo[*peDevice].stVideoParams.u16PID))
    {
        printf("Demo DMX VideoFlt Open fail \n");
        goto Demo_AV_SetPid_fail;
    }

    //start video pidflt
    if (DMX_FILTER_STATUS_OK != MApi_DMX_Start(stAVInfo[*peDevice].stVideoParams.u8Filter))
    {
        printf("MApi DMX VideoFlt start fail \n");
        goto Demo_AV_SetPid_fail;
    }

    // For Merge Stream. If set pid success, store current source id in AVInfo.
    stAVInfo[*peDevice].stDMXflowParams.eDmxFltSourceID = Demo_DMX_GetSrcID(stAVInfo[*peDevice].stDMXflowParams.eFlow);

    db_print("Device filters are ready\n");

    return TRUE;

Demo_AV_SetPid_fail:

    printf("Demo_AV_SetVideoPid_fail \n");

    if (stAVInfo[*peDevice].stVideoParams.u8Filter != INVALID_FILTER_ID)
    {
        MApi_DMX_Stop(stAVInfo[*peDevice].stVideoParams.u8Filter);
        MApi_DMX_Close(stAVInfo[*peDevice].stVideoParams.u8Filter);
        stAVInfo[*peDevice].stVideoParams.u8Filter = INVALID_FILTER_ID;
    }

    return FALSE;
}

//------------------------------------------------------------------------------
/// @brief Set PCR pid to device decoder path
/// @param[in] Main or Sub Device
/// @param[in] PCRPid PCR PID
/// @return TRUE: success.
/// @return FALSE: Process fail or Invalid input argument.
/// @sa
/// @note
/// Command: \b DTV_TSP_SetPCRPid 0 101 \n
//------------------------------------------------------------------------------
MS_BOOL Demo_AV_TSP_SetPCRPid(EN_AV_Device* peDevice, MS_U32* pu32PCRPid)
{
    DEMO_AV_CHECK_DEVICE_ID(*peDevice)

    EN_DEMO_DMX_FLOW eDMX_FLOW = stAVInfo[*peDevice].stDMXflowParams.eFlow;

    db_print("%d\n",*peDevice);
    db_print("%"DTC_MS_U32_d"\n",*pu32PCRPid);

    stAVInfo[*peDevice].stPCRParams.u16PID = (*pu32PCRPid) & 0x1FFF;

    //stop PCR pidflt
    if (stAVInfo[*peDevice].stPCRParams.ePCREngID != E_PCR_ENG_INVALID)
    {
        Demo_DMX_PCR_FltStop(stAVInfo[*peDevice].stPCRParams.ePCREngID);
        Demo_DMX_PCR_FltClose(stAVInfo[*peDevice].stPCRParams.ePCREngID);
        stAVInfo[*peDevice].stPCRParams.ePCREngID = E_PCR_ENG_INVALID;
        db_print("MApi_DMX_Stop PCREngID:%d\n",stAVInfo[*peDevice].stPCRParams.ePCREngID);
    }

    //open PCR pidflt
    if (TRUE != Demo_DMX_PCR_FltOpen(eDMX_FLOW, stAVInfo[*peDevice].stPCRParams.u16PID, &stAVInfo[*peDevice].stPCRParams.ePCREngID))
    {
        printf("Demo DMX PCRFlt Open fail \n");
        goto Demo_AV_SetPid_fail;
    }

    //start PCR pidflt
    if (stAVInfo[*peDevice].eAVSYNC_Mode == E_AV_SYNC_PCR_MODE)
    {
        if (TRUE != Demo_DMX_PCR_FltStart(stAVInfo[*peDevice].stPCRParams.ePCREngID))
        {
            printf("MApi DMX PCRFlt start fail \n");
            goto Demo_AV_SetPid_fail;
        }
    }

    // For Merge Stream. If set pid success, store current source id in AVInfo.
    stAVInfo[*peDevice].stDMXflowParams.eDmxFltSourceID = Demo_DMX_GetSrcID(stAVInfo[*peDevice].stDMXflowParams.eFlow);

    db_print("Device filters are ready\n");

    return TRUE;

Demo_AV_SetPid_fail:

    printf("Demo_AV_SetPCRPid_fail \n");

    if (stAVInfo[*peDevice].stPCRParams.ePCREngID != E_PCR_ENG_INVALID)
    {
        Demo_DMX_PCR_FltStop(stAVInfo[*peDevice].stPCRParams.ePCREngID);
        Demo_DMX_PCR_FltClose(stAVInfo[*peDevice].stPCRParams.ePCREngID);
        stAVInfo[*peDevice].stPCRParams.ePCREngID = E_PCR_ENG_INVALID;
    }

    return FALSE;
}

//------------------------------------------------------------------------------
/// @brief Set Video / Audio / PCR pid to device decoder path
/// @param[in] Main or Sub Device
/// @param[in] VideoPid Video PID
/// @param[in] AudioPid Audio PID
/// @param[in] PCRPid PCR PID
/// @param[in] pu32VCodec Video Codec (0: MPEG 1: H.264 2:AVS 3:HEVC)
/// @param[in] pu32ACodec Audio Codec Type
/// @return TRUE: success.
/// @return FALSE: Process fail or Invalid input argument.
/// @sa
/// @note
/// Command: \b DTV_TSP_SetPid 0 101 102 101 0 1 \n
//------------------------------------------------------------------------------
MS_BOOL Demo_AV_TSP_SetPid(EN_AV_Device* peDevice,MS_U32* pu32VideoPid, MS_U32* pu32AudioPid, MS_U32* pu32PCRPid, MS_U32* pu32VCodec, MS_U32* pu32ACodec)
{
    DEMO_AV_CHECK_DEVICE_ID(*peDevice)

    db_print("%d\n",*peDevice);
    db_print("%"DTC_MS_U32_d"\n",*pu32VideoPid);
    db_print("%"DTC_MS_U32_d"\n",*pu32AudioPid);
    db_print("%"DTC_MS_U32_d"\n",*pu32PCRPid);
    db_print("%"DTC_MS_U32_d"\n",*pu32VCodec);
    db_print("%"DTC_MS_U32_d"\n",*pu32ACodec);

    //set audio pid
    if (TRUE != Demo_AV_TSP_SetAudioPid(peDevice, pu32AudioPid, pu32ACodec))
    {
        return FALSE;
    }

    //set video pid
    if (TRUE != Demo_AV_TSP_SetVideoPid(peDevice, pu32VideoPid, pu32VCodec))
    {
        return FALSE;
    }

    //set PCR pid
    if (TRUE != Demo_AV_TSP_SetPCRPid(peDevice, pu32PCRPid))
    {
        return FALSE;
    }

    // For Merge Stream. If set pid success, store current source id in AVInfo.
    stAVInfo[*peDevice].stDMXflowParams.eDmxFltSourceID = Demo_DMX_GetSrcID(stAVInfo[*peDevice].stDMXflowParams.eFlow);

    stAVInfo[*peDevice].eAVInitState = E_AV_AVINIT_STATE_DMX;

    db_print("Device filters are ready\n");

    return TRUE;

}

//------------------------------------------------------------------------------
/// @brief Set Video / Audio pid to device decoder path (file-in)
/// @param[in] Main or Sub Device
/// @param[in] File-in engine type ("FILE" or "MMFI")
/// @param[in] File-in engine ID (MMFI: 0~1 , FILE: 0~3)
/// @param[in] VideoPid Video PID
/// @param[in] AudioPid Audio PID
/// @param[in] pu32VCodec Video Codec (0: MPEG 1: H.264 2:AVS 3:HEVC)
/// @param[in] pu32ACodec Audio Codec Type
/// @return TRUE: success.
/// @return FALSE: Process fail or Invalid input argument.
/// @sa
/// @note
/// Command: \b DTV_TSP_FileIn_SetPid 0 FILE 0  2011 2012  0 1\n
/// Command: \b DTV_TSP_FileIn_SetPid 1 MMFI 0  VOID 0x100 0 5\n
//------------------------------------------------------------------------------
MS_BOOL Demo_AV_TSP_FileIn_SetPid(EN_AV_Device *peDevice,char *pFileInEngType,MS_U8 *pu8EngID,char *pVideoPid,char *pAudioPid,char *pVCodec,char *pACodec)
{
    DEMO_AV_CHECK_DEVICE_ID(*peDevice)

    //Demo_AV_Initialize(peDevice);
    Demo_VDEC_SetMode((EN_VDEC_Device *)peDevice,E_VDEC_SetCmd_Initialize,(void *)TRUE);

    EN_AV_AVSYNC_Type eAVsyncType = E_AV_SYNC_AUDIO_MASTER_MODE;
    EN_AV_FileIn_Eng_Type eAvFileinType = E_AV_FileIn_Eng_FILE;
    MS_U32  u32VideoPid = INVALID_PID_ID, u32AudioPid = INVALID_PID_ID;
    MS_U32  u32VideoCodec = 0, u32AudioCodec = DEMO_AUDIO_CODEC_INVALID;

    db_print("%d\n",*peDevice);
    db_print("%s\n",pFileInEngType);
    db_print("%d\n",*pu8EngID);
    db_print("%s\n",pVideoPid);
    db_print("%s\n",pAudioPid);
    db_print("%s\n",pVCodec);
    db_print("%s\n",pACodec);

#if(DEMO_VDEC_NDECODE_TEST == 1)
    stAVInfo[*peDevice].stVideoParams.eFIFO_ID = *peDevice;
#else
    if (*peDevice == E_AV_DEVICE_MAIN)
    {
        stAVInfo[*peDevice].stVideoParams.eFIFO_ID = E_DMX_FLT_TYPE_VID0;
    }
#if(DEMO_VDEC_MULTI_TEST == 1)
    else if (*peDevice == E_AV_DEVICE_SUB)
    {
        stAVInfo[*peDevice].stVideoParams.eFIFO_ID = E_DMX_FLT_TYPE_VID1;
    }
#endif
#endif

    if (MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Get_MultiPlayer_Capability) == 1)
    {
        stAVInfo[*peDevice].stAudioParams.eFIFO_ID = Demo_AV_GetAudioFIFO(peDevice);
    }
    else
    {
        stAVInfo[*peDevice].stAudioParams.eFIFO_ID = E_DMX_FLT_TYPE_AUD0;
    }

    //allocate stc eng
    if (DMX_FILTER_STATUS_OK != MApi_DMX_ResAllocate(DMX_RES_STC_ENG,&stAVInfo[*peDevice].stPCRParams.ePCREngID))
    {
        printf("Demo DMX STC Eng allocate fail!\n");
        goto Demo_AV_TSP_FileIn_SetPid_fail;
    }
    else
    {
        printf("Demo DMX STC Eng allocate success!\n");
    }

    if(!Demo_Util_strcmp(pFileInEngType,"FILE"))
    {
        if(*pu8EngID >= 4)
        {
            printf("[%s][%d] Not support this file-in engine num !!\n", __FUNCTION__, __LINE__);
            goto Demo_AV_TSP_FileIn_SetPid_fail;
        }
        else
        {
            eAvFileinType = E_AV_FileIn_Eng_FILE;
            stAVInfo[*peDevice].stDMXflowParams.eFlow = E_DMX_FLOW_FILE0 + *pu8EngID;
        }
    }
    else if(!Demo_Util_strcmp(pFileInEngType,"MMFI"))
    {
        if(*pu8EngID >= 2)
        {
            printf("[%s][%d] Not support this mmfi-in engine num !!\n", __FUNCTION__, __LINE__);
            goto Demo_AV_TSP_FileIn_SetPid_fail;
        }

        eAvFileinType = E_AV_FileIn_Eng_MMFI;
    }
    else
    {
        printf("[%s][%d] Engine type is not valid !!\n", __FUNCTION__, __LINE__);
        goto Demo_AV_TSP_FileIn_SetPid_fail;
    }

    if(isalpha(pVCodec[0]))
    {
        if(!Demo_Util_strcmp(pVCodec,"VOID"))
        {
            db_print("not update video codec type !!\n");
        }
        else
        {
            printf("video codec type is not valid !!\n");
            goto Demo_AV_TSP_FileIn_SetPid_fail;
        }
    }
    else
    {
        if((pVCodec[0]=='0') && (pVCodec[1]=='X'|| pVCodec[1]=='x')) //hex
        {
            u32VideoCodec = strtoul(pVCodec,NULL,16) & 0xFF;
        }
        else
        {
            u32VideoCodec = strtoul(pVCodec,NULL,10) & 0xFF;
        }

        stAVInfo[*peDevice].stVideoParams.eVideoCodec = u32VideoCodec;

        printf("[%s][%d] u32VideoCodec = %"DTC_MS_U32_u"\n", __FUNCTION__, __LINE__, u32VideoCodec);

        stAVInfo[*peDevice].stVideoParams.stFilterType.eEngType = eAvFileinType;
        stAVInfo[*peDevice].stVideoParams.stFilterType.u8EngID = *pu8EngID;
    }

    if(isalpha(pVideoPid[0]))
    {
        if(!Demo_Util_strcmp(pVideoPid,"VOID"))
        {
            db_print("not update video Pid !!\n");
        }
        else
        {
            printf("video Pid is not valid !!\n");
            goto Demo_AV_TSP_FileIn_SetPid_fail;
        }
    }
    else
    {
        if((pVideoPid[0]=='0') && (pVideoPid[1]=='X'|| pVideoPid[1]=='x')) //hex
        {
            u32VideoPid = strtoul(pVideoPid,NULL,16) & 0x1FFF;
        }
        else
        {
            u32VideoPid = strtoul(pVideoPid,NULL,10) & 0x1FFF;
        }

        if(eAvFileinType == E_AV_FileIn_Eng_MMFI)
        {
            if(_Demo_AV_TSP_MMFI_SetPid(peDevice, TRUE, &u32VideoPid, &u32VideoCodec) != TRUE)
            {
                printf("[%s][%d] _Demo_AV_TSP_MMFI_SetPid() fail !!\n", __FUNCTION__, __LINE__);
                goto Demo_AV_TSP_FileIn_SetPid_fail;
            }
        }
        else if(eAvFileinType == E_AV_FileIn_Eng_FILE)
        {
            if(Demo_AV_TSP_SetVideoPid(peDevice, &u32VideoPid, &u32VideoCodec) != TRUE)
            {
                printf("[%s][%d] Demo_AV_TSP_SetVideoPid() fail !!\n", __FUNCTION__, __LINE__);
                goto Demo_AV_TSP_FileIn_SetPid_fail;
            }
        }
    }

    if(isalpha(pACodec[0]))
    {
        if(!Demo_Util_strcmp(pACodec,"VOID"))
        {
            db_print("not update audio codec type !!\n");
        }
        else
        {
            printf("audio codec type is not valid !!\n");
            goto Demo_AV_TSP_FileIn_SetPid_fail;
        }
    }
    else
    {
        if((pACodec[0]=='0') && (pACodec[1]=='X'|| pACodec[1]=='x')) //hex
        {
            u32AudioCodec = strtoul(pACodec,NULL,16) & 0xFF;
        }
        else
        {
            u32AudioCodec = strtoul(pACodec,NULL,10) & 0xFF;
        }

        stAVInfo[*peDevice].stAudioParams.eAudioCodec = u32AudioCodec;

        printf("[%s][%d] u32AudioCodec = %"DTC_MS_U32_u"\n", __FUNCTION__, __LINE__, u32AudioCodec);

        stAVInfo[*peDevice].stAudioParams.stFilterType.eEngType = eAvFileinType;
        stAVInfo[*peDevice].stAudioParams.stFilterType.u8EngID = *pu8EngID;

    }

    if(isalpha(pAudioPid[0]))
    {
        if(!Demo_Util_strcmp(pAudioPid,"VOID"))
        {
            db_print("not update audio Pid !!\n");
        }
        else
        {
            printf("audio Pid is not valid !!\n");
            goto Demo_AV_TSP_FileIn_SetPid_fail;
        }
    }
    else
    {
        if((pAudioPid[0]=='0') && (pAudioPid[1]=='X'|| pAudioPid[1]=='x')) //hex
        {
            u32AudioPid = strtoul(pAudioPid,NULL,16) & 0x1FFF;
        }
        else
        {
            u32AudioPid = strtoul(pAudioPid,NULL,10) & 0x1FFF;
        }

        //check if audio allocated the resource. If not, then skip.
        if (Demo_AV_CheckAudioResource(peDevice) == TRUE)
        {
            if(eAvFileinType == E_AV_FileIn_Eng_MMFI)
            {
                if(_Demo_AV_TSP_MMFI_SetPid(peDevice, FALSE, &u32AudioPid, &u32AudioCodec) != TRUE)
                {
                    printf("[%s][%d] _Demo_AV_TSP_MMFI_SetPid() fail !!\n", __FUNCTION__, __LINE__);
                    goto Demo_AV_TSP_FileIn_SetPid_fail;
                }
            }
            else if(eAvFileinType == E_AV_FileIn_Eng_FILE)
            {
                if(Demo_AV_TSP_SetAudioPid(peDevice, &u32AudioPid, &u32AudioCodec) != TRUE)
                {
                    printf("[%s][%d] Demo_AV_TSP_SetAudioPid() fail !!\n", __FUNCTION__, __LINE__);
                    goto Demo_AV_TSP_FileIn_SetPid_fail;
                }
            }
        }
        else
        {
            printf("No Audio resource, Device[%d] no need set pid flow\n", *peDevice);
        }
    }

    if((u32VideoPid == INVALID_PID_ID) && (u32AudioPid == INVALID_PID_ID)) // PES mode
    {
        if(u32AudioCodec != DEMO_AUDIO_CODEC_INVALID) //@TODO: add invalid entry of EN_VDEC_DDI_CodecType @ demo_vdec.h
            stAVInfo[*peDevice].stAudioParams.stFilterType.eEngFormat = E_AV_FileIn_Eng_PES;
        else
            stAVInfo[*peDevice].stVideoParams.stFilterType.eEngFormat = E_AV_FileIn_Eng_PES;
    }

    printf("[%s][%d] Video format = %x\n", __FUNCTION__, __LINE__, stAVInfo[*peDevice].stVideoParams.stFilterType.eEngFormat);
    printf("[%s][%d] Audio format = %x\n", __FUNCTION__, __LINE__, stAVInfo[*peDevice].stAudioParams.stFilterType.eEngFormat);

    /**** [Need to Modify] ****/ // @Vincent
    Demo_AV_SetAVSYNCMode(peDevice,&eAVsyncType);//AUDIO master mode
    /**************************/

    db_print("Device filters are ready\n");
    return TRUE;

Demo_AV_TSP_FileIn_SetPid_fail:
    printf("Demo_AV_TSP_FileIn_SetPid_fail \n");
    return FALSE;
}

//------------------------------------------------------------------------------
/// @brief  Play decoder
/// @param[in] Main or Sub Device or Other Device
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b DTV_PlayAV 0 \n
///                \b DTV_PlayAV 1 \n
//------------------------------------------------------------------------------
MS_BOOL Demo_AV_PlayAV(EN_AV_Device* peDevice)
{

#if(DEMO_VDEC_NDECODE_TEST == 1)
    *peDevice = Demo_AV_VDEC_Device_Mapping(*peDevice);
#endif

    DEMO_AV_CHECK_DEVICE_ID(*peDevice)

    MS_U8 u8Volume = DEFAULT_VOLUME;
    EN_VDEC_DDI_SrcMode eVDEC_SrcMode = E_VDEC_DDI_SRC_MODE_DTV;

    db_print("========[%s]========\n",__FUNCTION__);
    db_print("peDevice : %d ,Stream ID Before Init  0x%"DTC_MS_U32_x"\n",*peDevice,stAVInfo[*peDevice].stVideoParams.stVDECSteamID.u32Id);
    db_print("Demo_AV_Set_STC : %d  \n",stAVInfo[*peDevice].stPCRParams.ePCREngID);

    MS_BOOL bDisplayMute = TRUE;
    Demo_AV_MuteVideo(peDevice,&bDisplayMute);

    if (Demo_AV_CheckAudioResource(peDevice) == TRUE)
    {
        if (stAVInfo[*peDevice].eAVSYNC_Mode == E_AV_SYNC_PCR_MODE
            || stAVInfo[*peDevice].eAVSYNC_Mode == E_AV_SYNC_VIDEO_MASTER_MODE)
        {

            db_print("E_AV_SYNC_VIDEO_MASTER_MODE or E_AV_SYNC_PCR_MODE  \n");
            if (Demo_Audio_Play(*peDevice,AUDIO_APP_DTV,stAVInfo[*peDevice].stAudioParams.eAudioCodec,&stAVInfo[*peDevice].stPCRParams.ePCREngID) == FALSE) // default mepg2
            {
                printf("Demo_Audio_Play fail\n");
                return FALSE;
            }
            else
            {
                db_print("Demo_Audio_Play ok\n");
            }

            //stop audio FIFO Reset
            if (TRUE != Demo_DMX_AVFifo_Reset(stAVInfo[*peDevice].stAudioParams.eFIFO_ID,FALSE))
            {
                printf("Demo DMX AudioFIFO reset fail\n");
                return FALSE;
            }

            //set audio volume(default)
            Demo_Audio_SetAbsoluteVolume(&u8Volume);

        }
        else if (stAVInfo[*peDevice].eAVSYNC_Mode == E_AV_SYNC_AUDIO_MASTER_MODE)
        {
            //Audio Master mode
            db_print("E_AV_SYNC_AUDIO_MASTER_MODE \n");

            EN_AUDIO_APP_TYPE eAudioAppType = (stAVInfo[*peDevice].stAudioParams.stFilterType.eEngFormat == E_AV_FileIn_Eng_PES)? AUDIO_APP_PES_PLAY : AUDIO_APP_DTV_AM;

            printf("[%s][%d] eAudioAppType = %x\n", __FUNCTION__, __LINE__, eAudioAppType);

            if (Demo_Audio_Play(*peDevice,eAudioAppType,stAVInfo[*peDevice].stAudioParams.eAudioCodec,&stAVInfo[*peDevice].stPCRParams.ePCREngID) == FALSE) // default mepg2
            {
                printf("Demo_Audio_Play fail\n");
                return FALSE;
            }
            else
            {
                db_print("Demo_Audio_Play ok\n");
            }

            //stop audio FIFO Reset
            if (TRUE != Demo_DMX_AVFifo_Reset(stAVInfo[*peDevice].stAudioParams.eFIFO_ID,FALSE))
            {
                printf("Demo DMX AudioFIFO reset fail\n");
                return FALSE;
            }

            //set audio volume(default)
            Demo_Audio_SetAbsoluteVolume(&u8Volume);
        }
    }
    else
    {
        printf("No Audio resource, Device[%d] no play av\n", *peDevice);
    }

    stAVInfo[*peDevice].eAVInitState = E_AV_AVINIT_STATE_AUDIO;

    switch(stAVInfo[*peDevice].stVideoParams.stFilterType.eEngType)
    {
        case E_AV_FileIn_Eng_MMFI:
            //eVDEC_SrcMode = E_DDI_VDEC_SRC_MODE_FILE;
            eVDEC_SrcMode = E_VDEC_DDI_SRC_MODE_TS_FILE;
            break;
        case E_AV_FileIn_Eng_FILE:
            eVDEC_SrcMode = E_VDEC_DDI_SRC_MODE_TS_FILE;
            break;
        default:
            eVDEC_SrcMode = E_VDEC_DDI_SRC_MODE_DTV;
            break;
    }

    if (TRUE != Demo_VDEC_Init((EN_VDEC_Device*)peDevice,&(stAVInfo[*peDevice].stVideoParams.stVDECSteamID),stAVInfo[*peDevice].stVideoParams.eVideoCodec,eVDEC_SrcMode,stAVInfo[*peDevice].stPCRParams.ePCREngID)) // default mepg2
    {
        printf("Demo_VDEC_Init fail\n");
        return FALSE;
    }
    else
    {
        db_print("peDevice : %d ,Stream ID After Init  0x%"DTC_MS_U32_x"\n",*peDevice,stAVInfo[*peDevice].stVideoParams.stVDECSteamID.u32Id);
    }

    if(stAVInfo[*peDevice].stVideoParams.stFilterType.eEngFormat == E_AV_FileIn_Eng_PES)
    {
        MS_BOOL bEnable = FALSE;
        Demo_VDEC_SetMode((EN_VDEC_Device *)peDevice,E_VDEC_SetCmd_PUSIControl,(void *)(&bEnable));
        printf("[%s][%d] disable Vdec PUSI control \n", __FUNCTION__, __LINE__);
    }
    else
    {
        MS_BOOL bEnable = TRUE;
        Demo_VDEC_SetMode((EN_VDEC_Device *)peDevice,E_VDEC_SetCmd_PUSIControl,(void *)(&bEnable));
    }

    if (TRUE != Demo_VDEC_Play((EN_VDEC_Device*)peDevice,&(stAVInfo[*peDevice].stVideoParams.stVDECSteamID),stAVInfo[*peDevice].eAVSYNC_Mode))
    {
        printf("Demo_VDEC_Play fail\n");
        return FALSE;
    }
    else
    {
        db_print("Demo_VDEC_Play OK\n");
    }

    if (TRUE != Demo_DMX_AVFifo_Reset(stAVInfo[*peDevice].stVideoParams.eFIFO_ID,FALSE))
    {
        printf("Demo DMX VideoFIFO reset fail\n");
        return FALSE;
    }

    //Check Video Decoder Status
    if( TRUE != Demo_VDEC_CheckDecoderStatus((EN_VDEC_Device*)peDevice))
    {
        printf("Video Decoder Status is Not Ready!!\n Please Check Out the Code Flow!!\n");
        return FALSE;
    }

    if(stAVInfo[*peDevice].stShowFrameInfo.eShowFrameMode == E_AV_MVOP_MODE
        || stAVInfo[*peDevice].stShowFrameInfo.eShowFrameMode == E_AV_PureMCU_MODE)
        Demo_VDEC_IsAVSyncDone((EN_VDEC_Device*)peDevice);

    //open timeout machenism => the data is enough to decode
    // if this function calls in the begining of vdec playback, the mosaic might happen
    _Demo_AV_TimeoutSet(*peDevice,TRUE);

    stAVInfo[*peDevice].eAVInitState = E_AV_AVINIT_STATE_VIDEO;

    stAVInfo[*peDevice].bInited = TRUE;

    return TRUE;

}

//------------------------------------------------------------------------------
/// @brief  Pause decoder
/// @param[in] Main or Sub Device or Other Device
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b DTV_PauseAV 0 \n
/// Command: \b DTV_PauseAV 1 \n
//------------------------------------------------------------------------------

MS_BOOL Demo_AV_PauseAV(EN_AV_Device *peDevice)
{
    DEMO_AV_CHECK_DEVICE_ID(*peDevice)

    //AVFIFO Reset
    if (TRUE != Demo_DMX_AVFifo_Reset(stAVInfo[*peDevice].stVideoParams.eFIFO_ID,TRUE))
    {
        printf("Demo DMX VideoFIFO reset fail\n");
    }

    if (TRUE != Demo_DMX_AVFifo_Reset(stAVInfo[*peDevice].stAudioParams.eFIFO_ID,TRUE))
    {
        printf("Demo DMX AudioFIFO reset fail\n");
    }

    if (MApi_VDEC_EX_Pause(&(stAVInfo[*peDevice].stVideoParams.stVDECSteamID)) != E_VDEC_EX_OK)
    {
        printf("MApi_VDEC_EX_Pause fail\n");
        return FALSE;
    }

    if (stAVInfo[*peDevice].eAVSYNC_Mode == E_AV_SYNC_PCR_MODE
        || stAVInfo[*peDevice].eAVSYNC_Mode == E_AV_SYNC_VIDEO_MASTER_MODE)
    {
        if (Demo_Audio_Stop(*peDevice, AUDIO_APP_DTV) != TRUE)
        {
            printf("Demo_Audio_Stop fail\n");
            return FALSE;
        }
    }
    else if (stAVInfo[*peDevice].eAVSYNC_Mode == E_AV_SYNC_AUDIO_MASTER_MODE)
    {
        if (Demo_Audio_Stop(*peDevice, AUDIO_APP_DTV_AM) != TRUE)
        {
            printf("Demo_Audio_Stop fail\n");
            return FALSE;
        }
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Stop DTV audio
/// @param[in] peDevice Main or Sub Device or Other Device
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b none \n
//------------------------------------------------------------------------------
MS_BOOL Demo_AV_DTV_StopAudio(EN_AV_Device* peDevice)
{
    DEMO_AV_CHECK_DEVICE_ID(*peDevice)

    if (Demo_AV_CheckAudioResource(peDevice) == TRUE)
    {

        EN_AUDIO_APP_TYPE eAppType = AUDIO_APP_DTV;

        //audio type
        if (stAVInfo[*peDevice].eAVSYNC_Mode == E_AV_SYNC_PCR_MODE
            || stAVInfo[*peDevice].eAVSYNC_Mode == E_AV_SYNC_VIDEO_MASTER_MODE)
        {
            eAppType = AUDIO_APP_DTV;
        }
        else if (stAVInfo[*peDevice].eAVSYNC_Mode == E_AV_SYNC_AUDIO_MASTER_MODE)
        {
            eAppType = AUDIO_APP_DTV_AM;
        }
        else
        {
            eAppType = AUDIO_APP_DTV;
        }

        //start audio fifo reset
        if (TRUE != Demo_DMX_AVFifo_Reset(stAVInfo[*peDevice].stAudioParams.eFIFO_ID,TRUE))
        {
            printf("Demo DMX AudioFIFO reset fail\n");
        }

        //stop audio pidflt
        if (stAVInfo[*peDevice].stAudioParams.u8Filter != INVALID_FILTER_ID)
        {
            MApi_DMX_Stop(stAVInfo[*peDevice].stAudioParams.u8Filter);
            MApi_DMX_Close(stAVInfo[*peDevice].stAudioParams.u8Filter);
            stAVInfo[*peDevice].stAudioParams.u8Filter = INVALID_FILTER_ID;
        }

        if (Demo_Audio_Stop(*peDevice, eAppType) != TRUE)
        {
            printf("Demo_Audio_Stop fail\n");
            return FALSE;
        }

    }
    else
    {
        printf("No Audio resource, Device[%d] no need stop\n", *peDevice);
    }
    return TRUE;

}

//------------------------------------------------------------------------------
/// @brief Release all DTV audio resource
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b none \n
//------------------------------------------------------------------------------
MS_BOOL Demo_AV_DTV_ReleaseAudio_All(void)
{

    EN_AV_Device eDevice = E_AV_DEVICE_INVALID;

    for (eDevice=0; eDevice<E_AV_DEVICE_MAX; eDevice++)
    {
        if (Demo_AV_CheckAudioResource(&eDevice) == TRUE)
        {
            Demo_Audio_Release((MS_U32*)(&eDevice));
        }
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Stop all DTV audio
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b none \n
//------------------------------------------------------------------------------
MS_BOOL Demo_AV_DTV_StopAudio_All(void)
{

    EN_AV_Device eDevice = E_AV_DEVICE_INVALID;

    for (eDevice=0; eDevice<E_AV_DEVICE_MAX; eDevice++)
    {
        if (Demo_AV_CheckAudioResource(&eDevice) == TRUE)
        {
            Demo_AV_DTV_StopAudio(&eDevice);
        }
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief  Stop decoder
/// @param[in] Main or Sub Device or Other Device
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b DTV_StopAV 0 \n
/// Command: \b DTV_StopAV 1 \n
//------------------------------------------------------------------------------

MS_BOOL Demo_AV_StopAV(EN_AV_Device *peDevice)
{
    DEMO_AV_CHECK_DEVICE_ID(*peDevice)

    if(stAVInfo[*peDevice].bInited == TRUE)
    {
        MS_BOOL bDisplayMute = TRUE;
        Demo_AV_MuteVideo(peDevice,&bDisplayMute);
    }

    if(stAVInfo[*peDevice].eAVInitState >= E_AV_AVINIT_STATE_AUDIO)
    {
        if (Demo_AV_CheckAudioResource(peDevice) == TRUE)
        {
            if (Demo_AV_DTV_StopAudio(peDevice) != TRUE)
            {
                printf("Demo_Audio_Stop fail\n");
                return FALSE;
            }
        }
        else
        {
            printf("No Audio resource, Device[%d] no need stop\n", *peDevice);
        }
    }

    if(stAVInfo[*peDevice].eAVInitState >= E_AV_AVINIT_STATE_DMX)
    {
        //start video fifo reset
        if (TRUE != Demo_DMX_AVFifo_Reset(stAVInfo[*peDevice].stVideoParams.eFIFO_ID,TRUE))
        {
            printf("Demo DMX VideoFIFO reset fail\n");
        }

        //stop video pidflt
        if (stAVInfo[*peDevice].stVideoParams.u8Filter != INVALID_FILTER_ID)
        {
            MApi_DMX_Stop(stAVInfo[*peDevice].stVideoParams.u8Filter);
            MApi_DMX_Close(stAVInfo[*peDevice].stVideoParams.u8Filter);
            stAVInfo[*peDevice].stVideoParams.u8Filter = INVALID_FILTER_ID;
        }

        //stop PCR pidflt
        if (stAVInfo[*peDevice].stPCRParams.ePCREngID != E_PCR_ENG_INVALID)
        {
            Demo_DMX_PCR_FltStop(stAVInfo[*peDevice].stPCRParams.ePCREngID);
            Demo_DMX_PCR_FltClose(stAVInfo[*peDevice].stPCRParams.ePCREngID);
            stAVInfo[*peDevice].stPCRParams.ePCREngID = E_PCR_ENG_INVALID;
        }
    }

    //stop video
    Demo_VDEC_Stop((EN_VDEC_Device*) peDevice,&(stAVInfo[*peDevice].stVideoParams.stVDECSteamID));

    //reset variables
    stAVInfo[*peDevice].bInited = FALSE;
    stAVInfo[*peDevice].eAVSYNC_Task_State = E_AV_AVSYNC_STATE_RESET;
    stAVInfo[*peDevice].eAVSYNC_Mode = E_AV_SYNC_PCR_MODE;
    stAVInfo[*peDevice].eAVInitState = E_AV_AVINIT_STATE_RESET;

    Demo_AV_Initialize(peDevice);

    db_print("Demo_AV_StopAV\n");

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief  Stop decoder (file-in)
/// @param[in] Main or Sub Device or Other Device
/// @param[in] File-in engine type ("FILE" or "MMFI")
/// @param[in] File-in engine ID (MMFI: 0~1 , FILE: 0~3)
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b DTV_FileIn_StopAV 0 FILE 0 \n
/// Command: \b DTV_FileIn_StopAV 1 MMFI 1 \n
//------------------------------------------------------------------------------
MS_BOOL Demo_AV_FileIn_StopAV(EN_AV_Device *peDevice,char *pFileInEngType,MS_U8 *u8EngID)
{
    DEMO_AV_CHECK_DEVICE_ID(*peDevice)

    EN_AV_FileIn_Eng_Type eEngType = 0;

    if(!Demo_Util_strcmp(pFileInEngType,"MMFI"))
    {
        eEngType = E_AV_FileIn_Eng_MMFI;
    }
    else if(!Demo_Util_strcmp(pFileInEngType,"FILE"))
    {
        eEngType = E_AV_FileIn_Eng_FILE;
    }
    else
    {
        printf("Not support this engine type !!\n");
        return FALSE;
    }

    //STC Eng
    if (stAVInfo[*peDevice].stPCRParams.ePCREngID != E_PCR_ENG_INVALID)
    {
        if(DMX_FILTER_STATUS_OK != MApi_DMX_ResFree(DMX_RES_STC_ENG, &stAVInfo[*peDevice].stPCRParams.ePCREngID ))
        {
            printf("DMX free stc eng fail!!!\n");
        }
        else
        {
            printf("DMX free stc eng success!!!\n");
        }
        stAVInfo[*peDevice].stPCRParams.ePCREngID = E_PCR_ENG_INVALID;
    }

    stAVInfo[*peDevice].bEnableAVSYNC_Task = FALSE; //disable avsync task for stop av
    //Video
    if((eEngType == stAVInfo[*peDevice].stVideoParams.stFilterType.eEngType) && (*u8EngID == stAVInfo[*peDevice].stVideoParams.stFilterType.u8EngID))
    {
        MS_BOOL bDisplayMute = TRUE;
        Demo_AV_MuteVideo(peDevice,&bDisplayMute);

        //reset VFIFO
        if (TRUE != Demo_DMX_AVFifo_Reset(stAVInfo[*peDevice].stVideoParams.eFIFO_ID,TRUE))
        {
            printf("Demo DMX VideoFIFO reset fail\n");
        }

        if(stAVInfo[*peDevice].stVideoParams.u8Filter != INVALID_FILTER_ID)
        {
            switch(eEngType)
            {
                case E_AV_FileIn_Eng_MMFI:
                    MApi_DMX_MMFI_Pid_Close(stAVInfo[*peDevice].stVideoParams.u8Filter);
                    break;
                case E_AV_FileIn_Eng_FILE:
                    MApi_DMX_Stop(stAVInfo[*peDevice].stVideoParams.u8Filter);
                    MApi_DMX_Close(stAVInfo[*peDevice].stVideoParams.u8Filter);
                    break;
            }

            stAVInfo[*peDevice].stVideoParams.u8Filter = INVALID_FILTER_ID;
        }

        Demo_VDEC_Stop((EN_VDEC_Device*)peDevice,&(stAVInfo[*peDevice].stVideoParams.stVDECSteamID));

        stAVInfo[*peDevice].stVideoParams.u16PID = INVALID_PID_ID;
        stAVInfo[*peDevice].stVideoParams.stFilterType.eEngType = INVALID_FILEIN_TYPE;
        stAVInfo[*peDevice].stVideoParams.stFilterType.u8EngID = INVALID_FILEIN_ID;
    }

    //Audio
    if((eEngType == stAVInfo[*peDevice].stAudioParams.stFilterType.eEngType) && (*u8EngID == stAVInfo[*peDevice].stAudioParams.stFilterType.u8EngID))
    {
        //reset AFIFO
        if (TRUE != Demo_DMX_AVFifo_Reset(stAVInfo[*peDevice].stAudioParams.eFIFO_ID,TRUE))
        {
            printf("Demo DMX AudioFIFO reset fail\n");
        }

        if(stAVInfo[*peDevice].stAudioParams.u8Filter != INVALID_FILTER_ID)
        {
            switch(eEngType)
            {
                case E_AV_FileIn_Eng_MMFI:
                    MApi_DMX_MMFI_Pid_Close(stAVInfo[*peDevice].stAudioParams.u8Filter);
                    break;
                case E_AV_FileIn_Eng_FILE:
                    MApi_DMX_Stop(stAVInfo[*peDevice].stAudioParams.u8Filter);
                    MApi_DMX_Close(stAVInfo[*peDevice].stAudioParams.u8Filter);
                    break;
                default:
                    printf("[%s][%d] Impossible !!\n",__FUNCTION__, __LINE__);
                    break;
            }

            stAVInfo[*peDevice].stAudioParams.u8Filter = INVALID_FILTER_ID;
        }

        //Audio
        if (stAVInfo[*peDevice].eAVSYNC_Mode == E_AV_SYNC_PCR_MODE
            || stAVInfo[*peDevice].eAVSYNC_Mode == E_AV_SYNC_VIDEO_MASTER_MODE)
        {
            Demo_Audio_Stop(*peDevice, AUDIO_APP_DTV);
        }
        else if (stAVInfo[*peDevice].eAVSYNC_Mode == E_AV_SYNC_AUDIO_MASTER_MODE)
        {
            if (Demo_Audio_Stop(*peDevice, AUDIO_APP_DTV_AM) != TRUE)
            {
                printf("Demo_Audio_Stop fail\n");
                return FALSE;
            }
        }

        stAVInfo[*peDevice].stAudioParams.u16PID = INVALID_PID_ID;
        stAVInfo[*peDevice].stAudioParams.stFilterType.eEngType = INVALID_FILEIN_TYPE;
        stAVInfo[*peDevice].stAudioParams.stFilterType.u8EngID = INVALID_FILEIN_ID;
    }

    if((stAVInfo[*peDevice].stVideoParams.u8Filter == INVALID_FILTER_ID) && (stAVInfo[*peDevice].stAudioParams.u8Filter == INVALID_FILTER_ID))
    {
        db_print("bInited = FALSE \n");
        stAVInfo[*peDevice].bInited = FALSE;
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief  Stop AV FIFO
/// @param[in] Main or Sub Device or Other Device
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b NA \n
//------------------------------------------------------------------------------

MS_BOOL Demo_AV_StopAVFIFO(EN_AV_Device *peDevice)
{
    DEMO_AV_CHECK_DEVICE_ID(*peDevice)

    //AVFIFO Reset
    if (TRUE != Demo_DMX_AVFifo_Reset(stAVInfo[*peDevice].stVideoParams.eFIFO_ID,TRUE))
    {
        printf("Demo DMX VideoFIFO reset fail\n");
    }
    if (TRUE != Demo_DMX_AVFifo_Reset(stAVInfo[*peDevice].stAudioParams.eFIFO_ID,TRUE))
    {
        printf("Demo DMX AudioFIFO reset fail\n");
    }

    if (stAVInfo[*peDevice].stVideoParams.u8Filter != INVALID_FILTER_ID)
    {
        MApi_DMX_Stop(stAVInfo[*peDevice].stVideoParams.u8Filter);
        MApi_DMX_Close(stAVInfo[*peDevice].stVideoParams.u8Filter);
        stAVInfo[*peDevice].stVideoParams.u8Filter = INVALID_FILTER_ID;
    }
    if (stAVInfo[*peDevice].stAudioParams.u8Filter != INVALID_FILTER_ID)
    {
        MApi_DMX_Stop(stAVInfo[*peDevice].stAudioParams.u8Filter);
        MApi_DMX_Close(stAVInfo[*peDevice].stAudioParams.u8Filter);
        stAVInfo[*peDevice].stAudioParams.u8Filter = INVALID_FILTER_ID;
    }
    if (stAVInfo[*peDevice].stPCRParams.ePCREngID != E_PCR_ENG_INVALID)
    {
        Demo_DMX_PCR_FltStop(stAVInfo[*peDevice].stPCRParams.ePCREngID);
        Demo_DMX_PCR_FltClose(stAVInfo[*peDevice].stPCRParams.ePCREngID);
        stAVInfo[*peDevice].stPCRParams.ePCREngID = E_PCR_ENG_INVALID;
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief  Play Radio
/// @param[in] Main or Sub Device or Other Device
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b DTV_PlayRadio 0 \n
///                \b DTV_PlayRadio 1 \n
//------------------------------------------------------------------------------
MS_BOOL Demo_AV_PlayRadio(EN_AV_Device* peDevice)
{
    DEMO_AV_CHECK_DEVICE_ID(*peDevice)

    MS_U8 u8Volume = DEFAULT_VOLUME;

    EN_PCR_ENG eSetSTC = stAVInfo[*peDevice].stPCRParams.ePCREngID;

    db_print("========[%s]========\n",__FUNCTION__);
    db_print("Demo_AV_Set_STC : %d  \n",eSetSTC);

    if (stAVInfo[*peDevice].eAVSYNC_Mode == E_AV_SYNC_PCR_MODE)
    {
        if (Demo_Audio_Play(*peDevice,AUDIO_APP_DTV,stAVInfo[*peDevice].stAudioParams.eAudioCodec,&eSetSTC) == FALSE) // default mepg2
        {
            printf("Demo_Audio_Play fail\n");
            return FALSE;
        }
        else
        {
            db_print("Demo_Audio_Play ok\n");
        }

        //AVFIFO Reset
        if (TRUE != Demo_DMX_AVFifo_Reset(stAVInfo[*peDevice].stAudioParams.eFIFO_ID,FALSE))
        {
            printf("Demo DMX AudioFIFO reset fail\n");
            return FALSE;
        }

        //add audio unmute
        Demo_Audio_SetAbsoluteVolume(&u8Volume);

    }
    else if (stAVInfo[*peDevice].eAVSYNC_Mode == E_AV_SYNC_AUDIO_MASTER_MODE)
    {
        //Audio Master mode

        if (Demo_Audio_Play(*peDevice,AUDIO_APP_DTV_AM,stAVInfo[*peDevice].stAudioParams.eAudioCodec,&eSetSTC) == FALSE) // default mepg2
        {
            printf("Demo_Audio_Play fail\n");
            return FALSE;
        }
        else
        {
            db_print("Demo_Audio_Play ok\n");
        }

        if (TRUE != Demo_DMX_AVFifo_Reset(stAVInfo[*peDevice].stAudioParams.eFIFO_ID,FALSE))
        {
            printf("Demo DMX AudioFIFO reset fail\n");
            return FALSE;
        }

    }

    return TRUE;

}

//------------------------------------------------------------------------------
/// @brief  Stop Radio
/// @param[in] Main or Sub Device or Other Device
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b DTV_StopRadio 0 \n
/// Command: \b DTV_StopRadio 1 \n
//------------------------------------------------------------------------------

MS_BOOL Demo_AV_StopRadio(EN_AV_Device *peDevice)
{
    DEMO_AV_CHECK_DEVICE_ID(*peDevice)

    //stop PCR
    if (stAVInfo[*peDevice].stPCRParams.ePCREngID != E_PCR_ENG_INVALID)
    {
        Demo_DMX_PCR_FltStop(stAVInfo[*peDevice].stPCRParams.ePCREngID);
        Demo_DMX_PCR_FltClose(stAVInfo[*peDevice].stPCRParams.ePCREngID);
        stAVInfo[*peDevice].stPCRParams.ePCREngID = E_PCR_ENG_INVALID;
    }

    //stop audio
    if (Demo_AV_CheckAudioResource(peDevice) == TRUE)
    {
        Demo_AV_DTV_StopAudio(peDevice);
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief  switch audio
/// @param[in] Main or Sub Device or Other Device
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b DTV_Audio_Switch \n
//------------------------------------------------------------------------------
MS_BOOL Demo_AV_Audio_Switch(EN_AV_Device* peDevice, EN_AV_AUDIO_SWITCH_TYPE* peSwitchType)
{
    DEMO_AV_CHECK_DEVICE_ID(*peDevice)

    EN_AUDIO_APP_TYPE eAPP_Type = AUDIO_APP_DTV;
    AUDIO_OUTPORT_SOURCE_TYPE eOutSrcType = E_CONNECT_MAIN;
    EN_AV_Device eDevice = E_AV_DEVICE_INVALID, eDevice_main = E_AV_DEVICE_INVALID;
    ST_AUDIO_DEC_INFO stAudioInfo={0};

    //DTV stop all audio
    Demo_AV_DTV_StopAudio_All();

    //check if main resource allocated. If yes, than keep it. (speed up DTV to DTV)
    if(*peSwitchType == E_AV_AUDIO_SWITCH_TO_DTV)
    {
        //search and keep available main resource, and release other resources

        for (eDevice=0; eDevice<E_AV_DEVICE_MAX; eDevice++)
        {
            Demo_Audio_GetDecInfo(eDevice, &stAudioInfo);

            if (Demo_AV_CheckMainAudioResource(&eDevice) == TRUE)
            {
                eDevice_main = eDevice;
            }
            else
            {
                Demo_Audio_Release((MS_U32*)(&eDevice));
            }
        }

        //copy to new device
        if(eDevice_main != E_AV_DEVICE_INVALID)
        {
            Demo_Audio_SwitchResource((MS_U32*)peDevice, (MS_U32*)(&eDevice_main));
        }

    }
    else
    {
        //DTV release all audio resource
        Demo_AV_DTV_ReleaseAudio_All();
    }

#if (DEMO_MM_TEST == 1) && (DEMO_MM_AVP_TEST == 1)
    //MM stop and release all audio
    Demo_MM_MuteMediaItem_All();
#endif

    //play audio by application(DTV, MM)
    switch (*peSwitchType)
    {
        case E_AV_AUDIO_SWITCH_TO_DTV:
            {
                //App type
                if (stAVInfo[*peDevice].eAVSYNC_Mode == E_AV_SYNC_PCR_MODE
                || stAVInfo[*peDevice].eAVSYNC_Mode == E_AV_SYNC_VIDEO_MASTER_MODE)
                {
                    eAPP_Type = AUDIO_APP_DTV;
                }
                else if (stAVInfo[*peDevice].eAVSYNC_Mode == E_AV_SYNC_AUDIO_MASTER_MODE)
                {
                    eAPP_Type = AUDIO_APP_DTV_AM;
                }
                else
                {
                    eAPP_Type = AUDIO_APP_DTV;
                }

                //check if no audio resource
                if(Demo_AV_CheckAudioResource(peDevice) != TRUE)
                {
                    //DTV allocate audio resource
                    if (Demo_Audio_Open((MS_U32*)peDevice, &eOutSrcType, &eAPP_Type) == TRUE)
                    {
                        db_print("Device[%d] audio allocate done\n", *peDevice);
                    }
                    else
                    {
                        printf("Device[%d] audio allocate fail\n", *peDevice);
                        return FALSE;
                    }
                }

                MS_U32 u32AudioPID = (MS_U32)stAVInfo[*peDevice].stAudioParams.u16PID;
                MS_U32 u32AudioCodec = (MS_U32)stAVInfo[*peDevice].stAudioParams.eAudioCodec;
                //DTV set audio pid
                if (Demo_AV_TSP_SetAudioPid(peDevice, &u32AudioPID, &u32AudioCodec) == FALSE)
                {
                    printf("Device[%d] Demo AV TSP SetAudioPid fail\n", *peDevice);
                    return FALSE;
                }

                //DTV play audio
                if (Demo_AV_PlayRadio(peDevice) == FALSE)
                {
                    printf("Device[%d] Demo AV PlayRadio fail\n", *peDevice);
                    return FALSE;
                }

            }
            break;
        case E_AV_AUDIO_SWITCH_TO_MM:
            {

#if (DEMO_MM_TEST == 1) && (DEMO_MM_AVP_TEST == 1)
                //MM unmute and play audio
                MS_U32 u32unmute = 0;
                MS_U32 u32DeviceID = (MS_U32)(*peDevice);
                Demo_MM_MuteMediaItem(&u32DeviceID, &u32unmute);
#endif
            }
            break;
        default:
            break;
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief  mute video
/// @param[in] Main or Sub Device or Other Device
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b none \n
//------------------------------------------------------------------------------

MS_BOOL Demo_AV_MuteVideo(EN_AV_Device *peDevice,MS_BOOL *pbDisplayMute)
{
    DEMO_AV_CHECK_DEVICE_ID(*peDevice)

    MS_U32 u32XCDevice = 0;
    MS_U32 u32XCWindow = *peDevice;
    MS_U32 u32DisplayMute = *pbDisplayMute;
    EN_ZAPPING_TYPE eZappingType = E_ZAPPING_NORMAL;

    Demo_Zapping_GetZappingType(peDevice,&eZappingType);

    switch(eZappingType)
    {
        case E_ZAPPING_XC_SEAMLESS:
    #if (DEMO_XC_SEAMLESS_ZAPPING_TEST == 1)
            if(Demo_XC_SeamlessZapping_IsFreeze(&u32XCDevice, &u32XCWindow) == FALSE)
            {
                goto VideoMute;
            }
    #endif
            break;

        case E_ZAPPING_GOP_SEAMLESS:
        case E_ZAPPING_FQ:
            break;

        case E_ZAPPING_DIP_SEAMLESS:
            {
                MS_BOOL bStopType = TRUE;
                Demo_VDEC_SetMode((EN_VDEC_Device *)peDevice,E_VDEC_SetCmd_DIPStopType,(void *)(&bStopType));
                if(stAVInfo[*peDevice].stShowFrameInfo.eShowFrameMode == E_AV_PureMCU_MODE)
                {
                    stAVInfo[*peDevice].stShowFrameInfo.stVDECWinControl.u8EnableSetDigitalSignal   = 2 ;   //2:  force disable
                    stAVInfo[*peDevice].stShowFrameInfo.stVDECWinControl.u8EnableClearDigitalSignal = 2 ;
                }
            }
            break;

        case E_ZAPPING_NORMAL:
        default:
    #if (DEMO_XC_SEAMLESS_ZAPPING_TEST == 1)
        VideoMute:
    #endif
            if(stAVInfo[*peDevice].stShowFrameInfo.eShowFrameMode == E_AV_DIP_MODE
                || stAVInfo[*peDevice].stShowFrameInfo.eShowFrameMode == E_AV_PureMCU_MODE)
                Demo_XC_SetVideoMute(&u32XCDevice, &stAVInfo[*peDevice].stShowFrameInfo.u32Window, &u32DisplayMute);
            else
                Demo_XC_SetVideoMute(&u32XCDevice, &u32XCWindow, &u32DisplayMute); // Mute HDMI  :XC0

            if(*peDevice == E_AV_DEVICE_MAIN)
            {
    #if (DEMO_XC_DUALXC_TEST == 1)
                u32XCDevice = 1;                                          //XC1
                Demo_XC_SetVideoMute(&u32XCDevice, &u32XCWindow, &u32DisplayMute); // Mute CVBS
    #endif
                Demo_VE_SetVideoMute(&u32DisplayMute);                             // Mute CVBS

                db_print("Demo_VE_SetVideoMute:%"DTC_MS_U32_d"\n",u32DisplayMute);
            }
            break;
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief to Demo_AV_PlayFromMemory \n
/// - For Video Decoder:
///     - check display information is ready
///     - check frame count is increasing
///     - check AV sync done
/// - For MVOP and SCALER:
///     - get video information and set output information of MVOP
///     - the output of MVOP is the input of SCALER
/// @param[in]  0 : for Main 1: for Sub
/// @param[in]  0: MPEG2  1:AVC  2:AVS 3:HEVC 4:MPEG4  5:VC1 6:VP9
/// @param[in]  play loop number
/// @param[in]  Reserved for dynamic scale demo
/// @param[in]  input source
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command:
//    \b DTV_PlayFromMemory 0 1 0  /applications/avc.bin     \n
//    \b DTV_PlayFromMemory 0 0 0  /applications/mpeg2.bin  \n
//------------------------------------------------------------------------------
MS_BOOL Demo_AV_PlayFromMemory(EN_AV_Device* peDevice,int* pVCodec, int* pPlayLoop, char* pPath)
{

#if(DEMO_VDEC_NDECODE_TEST == 1)
    *peDevice = Demo_AV_VDEC_Device_Mapping(*peDevice);
#endif

    DEMO_AV_CHECK_DEVICE_ID(*peDevice)

    MS_U32 u32DstAddr = 0;
    MS_U32 u32DstAddr2 = 0;
    MS_U32 u32ActualBitstreamSize = 0;
    MS_U32 u32ActualBitstreamSize2 = 0;
    MS_U32 u32DstSize = 0;

    db_print("=====[%s]=====\n",__FUNCTION__);

    switch(*pVCodec)
    {
        case E_VDEC_DDI_CODEC_TYPE_H264:
            stAVInfo[*peDevice].stVideoParams.eVideoCodec = E_VDEC_DDI_CODEC_TYPE_H264_MEMORY;
            db_print(" VideoCodec : H264 \n");
            break;
        case E_VDEC_DDI_CODEC_TYPE_AVS:
            stAVInfo[*peDevice].stVideoParams.eVideoCodec = E_VDEC_DDI_CODEC_TYPE_AVS_MEMORY;
            db_print(" VideoCodec : AVS \n");
            break;
        case E_VDEC_DDI_CODEC_TYPE_HEVC:
            stAVInfo[*peDevice].stVideoParams.eVideoCodec = E_VDEC_DDI_CODEC_TYPE_HEVC_MEMORY;
            db_print(" VideoCodec : HEVC \n");
            break;
        case 4:
            stAVInfo[*peDevice].stVideoParams.eVideoCodec = E_VDEC_DDI_CODEC_TYPE_MPEG4_MEMORY;
            db_print(" VideoCodec : MPEG4 \n");
            break;
        case 5:
            stAVInfo[*peDevice].stVideoParams.eVideoCodec = E_VDEC_DDI_CODEC_TYPE_VC1_ADV_MEMORY;
            db_print(" VideoCodec : VC1 \n");
            break;
        case 6:
            stAVInfo[*peDevice].stVideoParams.eVideoCodec = E_VDEC_DDI_CODEC_TYPE_VP9_MEMORY;
            db_print(" VideoCodec : VP9 \n");
            break;
        case E_VDEC_DDI_CODEC_TYPE_MPEG2:
        default:
            stAVInfo[*peDevice].stVideoParams.eVideoCodec = E_VDEC_DDI_CODEC_TYPE_MPEG2_MEMORY;
            db_print(" VideoCodec : MPEG2 \n");
            break;
    }

    db_print(" Stream ID Before Init  0x%"DTC_MS_U32_x"\n",stAVInfo[*peDevice].stVideoParams.stVDECSteamID.u32Id);

    if (Demo_VDEC_Init((EN_VDEC_Device*)peDevice,&(stAVInfo[*peDevice].stVideoParams.stVDECSteamID),stAVInfo[*peDevice].stVideoParams.eVideoCodec,E_VDEC_DDI_SRC_MODE_FILE,0) == FALSE) // default mepg2
    {
        printf("Demo_VDEC_Init fail\n");
        return FALSE;
    }
    else
    {
        db_print(" Stream ID After Init 0x%"DTC_MS_U32_x"\n",stAVInfo[*peDevice].stVideoParams.stVDECSteamID.u32Id);
    }

#if(DEMO_VDEC_NDECODE_TEST == 1)
    VDEC_EX_BufferInfo parms;
    memset(&parms, 0 , sizeof(VDEC_EX_BufferInfo));

    if(Demo_VDEC_GetMode((EN_VDEC_Device *)peDevice,E_VDEC_GetCmd_BUFFERTYPE_ALLOC_BS,&parms))
    {
        u32DstAddr = parms.phyAddr;
        u32DstSize = parms.szSize;
    }
#else
    if (*peDevice == E_AV_DEVICE_MAIN)
    {
        u32DstAddr = MEM_ADR_BY_MIU(VDEC_BIT_STREAM_ADR, VDEC_BIT_STREAM_MEMORY_TYPE);
        u32DstSize = VDEC_BIT_STREAM_LEN;
    }
#if(DEMO_VDEC_MULTI_TEST == 1)
    else if (*peDevice == E_AV_DEVICE_SUB)
    {
        u32DstAddr = MEM_ADR_BY_MIU(VDEC_BIT_STREAM1_ADR, VDEC_BIT_STREAM1_MEMORY_TYPE);
        u32DstSize = VDEC_BIT_STREAM1_LEN;
    }
#endif
#endif

    /* Load Stream to ES Buffer */

    u32DstAddr = u32DstAddr+ DRV_PROC_SIZE;
    db_print("B bitstream 0x%08"DTC_MS_U32_x"\n",u32DstAddr);
    u32ActualBitstreamSize = Demo_VDEC_UtlLoadStream(u32DstAddr,u32DstSize,stAVInfo[*peDevice].stVideoParams.eVideoCodec ,pPath);
    db_print("u32ActualBitstreamSize %"DTC_MS_U32_d" \n",u32ActualBitstreamSize);


    if (u32ActualBitstreamSize == 0)
    {
        printf("File Size 0 \n");
        return FALSE;
    }

    if (TRUE != Demo_VDEC_FilePlay((EN_VDEC_Device*)peDevice,&stAVInfo[*peDevice].stVideoParams.stVDECSteamID,u32DstAddr,u32ActualBitstreamSize,*pPlayLoop,u32DstAddr2,u32ActualBitstreamSize2,stAVInfo[*peDevice].stShowFrameInfo.eShowFrameMode))
    {
        printf("Demo_VDEC_FilePlay fail\n");
        return FALSE;
    }

    stAVInfo[*peDevice].bInited = TRUE;

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief to Demo_AV_StopFromMemory \n
/// - For Video Decoder:
///     - check display information is ready
///     - check frame count is increasing
///     - check AV sync done
/// - For MVOP and SCALER:
///     - get video information and set output information of MVOP
///     - the output of MVOP is the input of SCALER
/// @param[in]  0 : for Main 1: for Sub
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command:  \b DTV_StopFromMemory 0  \n
//------------------------------------------------------------------------------

MS_BOOL Demo_AV_StopFromMemory(EN_AV_Device* peDevice)
{
    DEMO_AV_CHECK_DEVICE_ID(*peDevice)

    MS_BOOL bDisplayMute = TRUE;
    Demo_AV_MuteVideo(peDevice,&bDisplayMute);

    if (TRUE != Demo_VDEC_FileStop((EN_VDEC_Device*) peDevice))
    {
        printf("Demo_VDEC_FileStop fail\n");
        return FALSE;
    }

    //reset variables
    stAVInfo[*peDevice].bInited = FALSE;
    stAVInfo[*peDevice].eAVSYNC_Task_State = E_AV_AVSYNC_STATE_RESET;
    stAVInfo[*peDevice].eAVSYNC_Mode = E_AV_SYNC_PCR_MODE;

    Demo_AV_Initialize(peDevice);

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief to Demo_AV_DecodeIFrame \n
/// - For NO MVOP SCALER:
///     - VDEC decode I Frame
///     - get video information and do SW Detile
///     - Use GOP to show
/// @param[in]  Main or Sub Device
/// @param[in] 0:MPEG2 1.H.264 2.AVS 3.HEVC
/// @Param[in] file path
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command:
///  DTV_DecIFrame 0 1 /applications/ifram.bin
///  DTV_DecIFrame 1 1 /applications/ifram.bin
///  DTV_DecIFrame 1 0 /applications/ifram2.bin
///  DTV_DecIFrame 0 0 /applications/ifram2.bin
//------------------------------------------------------------------------------
MS_BOOL Demo_AV_DecodeIFrame(EN_AV_Device* peDevice,int* pCodec,char* pLogopath)
{

#if(DEMO_VDEC_NDECODE_TEST == 1)
    *peDevice = Demo_AV_VDEC_Device_Mapping(*peDevice);
#endif

    DEMO_AV_CHECK_DEVICE_ID(*peDevice)

    MS_U8 u8GOPNum;
    MS_U32 u32ColorType;
    MS_U32 u32GWinWidth;
    MS_U32 u32GWinHeight;

    VDEC_EX_FrameInfo stFrameInfo;

    memset(&stFrameInfo, 0, sizeof(VDEC_EX_FrameInfo));

    if(Demo_VDEC_DecIFrame((EN_VDEC_Device*)peDevice,pCodec,pLogopath) != TRUE)
    {
        printf("Demo_VDEC_DecIFrame FAIL!!!\n");
        return FALSE;
    }

    if(stAVInfo[*peDevice].stShowFrameInfo.eShowFrameMode == E_AV_SWDetile_MODE)
    {
        if(Demo_VDEC_GetDecFrameInfo((EN_VDEC_Device*)peDevice,&stFrameInfo) != TRUE)
        {
            printf("Demo_VDEC_GetDecFrameInfo FAIL!!!\n");
            return FALSE;
        }

        db_print("Demo_VDEC_GetDecFrameInfo !!\n");
        db_print("LumaAddr [0x%"DTC_MS_PHY_x"], ChromaAddr [0x%"DTC_MS_PHY_x"]\n",stFrameInfo.u32LumaAddr,stFrameInfo.u32ChromaAddr);
        db_print("H [%d], V [%d], Pitch [%d]\n",stFrameInfo.u16Width,stFrameInfo.u16Height,stFrameInfo.u16Pitch);

        u8GOPNum = 0;
        u32ColorType = 4;
        u32GWinWidth = stFrameInfo.u16Width;
        u32GWinHeight = stFrameInfo.u16Height;

        Demo_OSD_CreateWin(&u32ColorType, &u32GWinWidth, &u32GWinHeight, &u32GWinWidth, &u32GWinHeight);

        if(Demo_VDEC_SW_DeTile((EN_VDEC_Device*)peDevice, &u8GOPNum,&stFrameInfo) != TRUE)
        {
            printf("Demo_VDEC_SW_DeTile FAIL!!!\n");
            return FALSE;
        }
    }

    stAVInfo[*peDevice].bInited = TRUE;

    return TRUE;
}

#if(DEMO_AUDIO_AD_1PID_TEST == 1)
//------------------------------------------------------------------------------
/// @brief  Play 1PID AD
/// @param[in] Main or Sub Device
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b DTV_PlayAD_1PID 0\n
//------------------------------------------------------------------------------
MS_BOOL Demo_AV_PlayAD_1PID(EN_AV_Device* peDevice)
{
    DEMO_AV_CHECK_DEVICE_ID(*peDevice)

    // coverity[var_deref_model]
    if (TRUE != Demo_Audio_Play(*peDevice, AUDIO_APP_DTV_AD_1PID, stAVInfo[*peDevice].stAudioParams.eAudioCodec, NULL))
    {
        printf("Demo_Audio_Play fail\n");
        return FALSE;
    }
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief  Stop 1PID AD
/// @param[in] Main or Sub Device
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b DTV_StopAD_1PID 0 \n
//------------------------------------------------------------------------------
MS_BOOL Demo_AV_StopAD_1PID(EN_AV_Device* peDevice)
{
    DEMO_AV_CHECK_DEVICE_ID(*peDevice)

    if (TRUE != Demo_Audio_Stop(*peDevice, AUDIO_APP_DTV_AD_1PID))
    {
        printf("Demo_Audio_Stop fail\n");
        return FALSE;
    }
    return TRUE;
}
#endif

#if(DEMO_AUDIO_AD_TEST == 1)
//------------------------------------------------------------------------------
/// @brief  Play AD
/// @param[in] Main or Sub Device
/// @param[in] u16AudioADPID Audio AD PID
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b DTV_PlayAD 0 102\n
//------------------------------------------------------------------------------
MS_BOOL Demo_AV_PlayAD(EN_AV_Device* peDevice, MS_U16* u16AudioADPID)
{
    DEMO_AV_CHECK_DEVICE_ID(*peDevice)

    MS_BOOL bADMute = FALSE;
    MS_U8 u8Volume = DEFAULT_VOLUME;
    EN_DEMO_DMX_FLOW eDMX_FLOW = stAVInfo[*peDevice].stDMXflowParams.eFlow;

    stAVInfo[*peDevice].stAudioADParams.u16PID = *u16AudioADPID;
    stAVInfo[*peDevice].stAudioADParams.eFIFO_ID = Demo_AV_GetAudioADFIFO(peDevice);

    //AVFIFO Reset
    if (TRUE != Demo_DMX_AVFifo_Reset(stAVInfo[*peDevice].stAudioADParams.eFIFO_ID,TRUE))
    {
        printf("Demo DMX AudioADFIFO Reset fail\n");
        goto Demo_AV_PlayAD_SetPid_fail;
    }

    if (stAVInfo[*peDevice].stAudioADParams.u8Filter != INVALID_FILTER_ID)
    {
        MApi_DMX_Stop(stAVInfo[*peDevice].stAudioADParams.u8Filter);
        MApi_DMX_Close(stAVInfo[*peDevice].stAudioADParams.u8Filter);
        stAVInfo[*peDevice].stAudioADParams.u8Filter = INVALID_FILTER_ID;
        db_print("MApi_DMX_Stop AudioADFilters:%d\n",stAVInfo[*peDevice].stAudioADParams.u8Filter);
    }

    //DMX Flt Open and SetPid
    if (TRUE != Demo_DMX_FltOpen( eDMX_FLOW, stAVInfo[*peDevice].stAudioADParams.eFIFO_ID, &stAVInfo[*peDevice].stAudioADParams.u8Filter, stAVInfo[*peDevice].stAudioADParams.u16PID))
    {
        printf("Demo DMX AudioADFilter Open fail\n");
        goto Demo_AV_PlayAD_SetPid_fail;
    }

    //DMX Flt Start
    if (DMX_FILTER_STATUS_OK != MApi_DMX_Start(stAVInfo[*peDevice].stAudioADParams.u8Filter))
    {
        printf("MApi DMX AudioADFilter Start fail\n");
        goto Demo_AV_PlayAD_SetPid_fail;
    }

    // coverity[var_deref_model]
    if (Demo_Audio_Play(*peDevice, AUDIO_APP_DTV_AD, stAVInfo[*peDevice].stAudioParams.eAudioCodec, NULL) == FALSE)
    {
        printf("Demo Audio Play fail\n");
        goto Demo_AV_PlayAD_SetPid_fail;
    }

    //open audio AD FIFO
    if (TRUE != Demo_DMX_AVFifo_Reset(stAVInfo[*peDevice].stAudioADParams.eFIFO_ID,FALSE))
    {
        printf("Demo DMX AudioADFIFO Reset fail\n");
    }

    MApi_AUDIO_SetADOutputMode(AD_OUT_BOTH);
    Demo_Audio_SetADAbsoluteVolume(&u8Volume);
    Demo_Audio_ADSetMute(&bADMute);

    return TRUE;

Demo_AV_PlayAD_SetPid_fail:

    printf("Demo_AV_PlayAD_SetPid_fail\n");

    if (stAVInfo[*peDevice].stAudioADParams.u8Filter != INVALID_FILTER_ID)
    {
        MApi_DMX_Stop(stAVInfo[*peDevice].stAudioADParams.u8Filter);
        MApi_DMX_Close(stAVInfo[*peDevice].stAudioADParams.u8Filter);
        stAVInfo[*peDevice].stAudioADParams.u8Filter = INVALID_FILTER_ID;
    }

    return FALSE;
}

//------------------------------------------------------------------------------
/// @brief  Stop AD
/// @param[in] Main or Sub Device
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b DTV_StopAD 0 \n
//------------------------------------------------------------------------------
MS_BOOL Demo_AV_StopAD(EN_AV_Device* peDevice)
{
    DEMO_AV_CHECK_DEVICE_ID(*peDevice)

    //Stop audio AD FIFO
    if (TRUE != Demo_DMX_AVFifo_Reset(stAVInfo[*peDevice].stAudioADParams.eFIFO_ID,TRUE))
    {
        printf("Demo DMX AudioADFIFO Reset fail\n");
    }

    if (stAVInfo[*peDevice].stAudioADParams.u8Filter != INVALID_FILTER_ID)
    {
        MApi_DMX_Stop(stAVInfo[*peDevice].stAudioADParams.u8Filter);
        MApi_DMX_Close(stAVInfo[*peDevice].stAudioADParams.u8Filter);
        stAVInfo[*peDevice].stAudioADParams.u8Filter = INVALID_FILTER_ID;
    }

    if (TRUE != Demo_Audio_Stop(*peDevice, AUDIO_APP_DTV_AD))
    {
        printf("Demo_Audio_Stop fail\n");
        return FALSE;
    }

    return TRUE;
}
#endif

#if(DEMO_MVC_TEST == 1)
//------------------------------------------------------------------------------
/// @brief to Demo_AV_MVC_PlayFromMemory \n
/// @param[in] play loop number
/// @param[in] file path
/// @param[in] file path
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b DTV_MVC_PlayFromMemory \n
//  DTV_MVC_PlayFromMemory 0 /applications/mvc.bin
//  DTV_MVC_PlayFromMemory 8 /applications/mvc_0.bin  /applications/mvc_1.bin
//------------------------------------------------------------------------------
MS_BOOL Demo_AV_MVC_PlayFromMemory(int* pPlayLoop, char* pPath1,char* pPath2)
{
    MS_U32 u32ActualBitstreamSize = 0;
    MS_U32 u32ActualBitstreamSize2 = 0;
    MS_U32 u32DstAddr = 0;
    MS_U32 u32DstAddr2 = 0;
    EN_AV_Device eDevice = 0;

    memset(&stAVInfo[eDevice],  0 , sizeof(ST_AV_INFO));

    stAVInfo[eDevice].stVideoParams.eVideoCodec = E_VDEC_DDI_CODEC_TYPE_MVC_MEMORY;

    if (*pPath2 != NULL)
    {
        Demo_VDEC_Init((EN_VDEC_Device*)&eDevice,&stAVInfo[eDevice].stVideoParams.stVDECSteamID,stAVInfo[eDevice].stVideoParams.eVideoCodec,E_VDEC_DDI_SRC_MODE_FILE_DUAL_ES,0);
    }
    else
    {
        Demo_VDEC_Init((EN_VDEC_Device*)&eDevice,&stAVInfo[eDevice].stVideoParams.stVDECSteamID,stAVInfo[eDevice].stVideoParams.eVideoCodec,E_VDEC_DDI_SRC_MODE_FILE,0);
    }

    u32DstAddr = MEM_ADR_BY_MIU(VDEC_MVC_BIT_STREAM_ADR, VDEC_MVC_BIT_STREAM_MEMORY_TYPE);
    u32DstAddr = u32DstAddr+ DRV_PROC_SIZE;
    u32ActualBitstreamSize = Demo_VDEC_UtlLoadStream(u32DstAddr,VDEC_MVC_BIT_STREAM_LEN,E_VDEC_DDI_CODEC_TYPE_MVC_MEMORY,pPath1);
    db_print("A bitstream 0x%08"DTC_MS_U32_x"\n",u32DstAddr);
    db_print("u32ActualBitstreamSize %"DTC_MS_U32_d" \n",u32ActualBitstreamSize);

    if (*pPath2 != NULL)
    {
        db_print("=======Load 2nd Binary for Sub=======\n");
        u32DstAddr2 = MEM_ADR_BY_MIU(VDEC_MVC_BIT_STREAM_ADR + PLAY_SIZE_LIMITITATION, VDEC_MVC_BIT_STREAM_MEMORY_TYPE);
        u32ActualBitstreamSize2 = Demo_VDEC_UtlLoadStream(u32DstAddr2,VDEC_MVC_BIT_STREAM_LEN,E_VDEC_DDI_CODEC_TYPE_MVC_MEMORY,pPath2);

        if (u32ActualBitstreamSize2 == 0)
        {
            printf("File2 Size 0\n");
            return FALSE;
        }
    }

    if (u32ActualBitstreamSize == 0)
    {
        printf("File1 Size 0\n");
        return FALSE;
    }

    u32DstAddr = u32DstAddr - MEM_ADR_BY_MIU(VDEC_MVC_BIT_STREAM_ADR, VDEC_MVC_BIT_STREAM_MEMORY_TYPE);
    u32DstAddr2 = u32DstAddr2 - MEM_ADR_BY_MIU(VDEC_MVC_BIT_STREAM_ADR, VDEC_MVC_BIT_STREAM_MEMORY_TYPE);

    if (TRUE != Demo_VDEC_FilePlay((EN_VDEC_Device*)eDevice,&stAVInfo[eDevice].stVideoParams.stVDECSteamID,u32DstAddr,u32ActualBitstreamSize,*pPlayLoop,u32DstAddr2,u32ActualBitstreamSize2,stAVInfo[eDevice].stShowFrameInfo.eShowFrameMode))
    {
        printf("Demo_VDEC_FilePlay fail\n");
        return FALSE;
    }

   stAVInfo[eDevice].bInited = TRUE;

   return TRUE;
}

//------------------------------------------------------------------------------
/// @brief to Demo_AV_MVC_StopFromMemory \n
/// - For Video Decoder:
///     - check display information is ready
///     - check frame count is increasing
///     - check AV sync done
/// - For MVOP and SCALER:
///     - get video information and set output information of MVOP
///     - the output of MVOP is the input of SCALER
/// @param[in]  0 : for Main 1: for Sub
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command:  \b DTV_MVC_StopFromMemory  \n
//------------------------------------------------------------------------------

MS_BOOL Demo_AV_MVC_StopFromMemory(void)
{
    EN_AV_Device eDevice = 0;

    MS_BOOL bDisplayMute = TRUE;
    Demo_AV_MuteVideo(&eDevice,&bDisplayMute);

    if (TRUE != Demo_VDEC_FileStop((EN_VDEC_Device*)&eDevice))
    {
        printf("Demo_VDEC_FileStop fail\n");
        return FALSE;
    }
    stAVInfo[eDevice].bInited = FALSE;

    return TRUE;
}
#endif

//------------------------------------------------------------------------------
/// @brief to Demo_AV_ExChangeWindow \n
/// - change window
/// @param[in]  windowA
/// @param[in]  windowB
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command:  \b DTV_ExChangeWindow windowA windowB  \n
//------------------------------------------------------------------------------
MS_BOOL Demo_AV_ExChangeWindow(MS_U32* pu32WindowA, MS_U32* pu32WindowB)
{
    printf("Demo_AV_ExChangeWindow windowA[%"DTC_MS_U32_d"] windowB[%"DTC_MS_U32_d"]\n",*pu32WindowA,* pu32WindowB);

    ST_AV_ShowFrameInfo stAVShowFrameInfoA = {0},stAVShowFrameInfoB = {0};
    ST_VDEC_WIN_INFO stVDECWinInfoA = {0}, stVDECWinInfoB = {0};

    Demo_AV_GetAVInfo((EN_AV_Device*)pu32WindowA,E_AV_GetCmd_WindowInfo,&stAVShowFrameInfoA);
    EN_AV_Device eDeviceA = stAVShowFrameInfoA.eDevice;

    Demo_AV_GetAVInfo((EN_AV_Device*)pu32WindowB,E_AV_GetCmd_WindowInfo,&stAVShowFrameInfoB);
    EN_AV_Device eDeviceB = stAVShowFrameInfoB.eDevice;

    // Device A use window B
    stAVInfo[eDeviceA].stShowFrameInfo.u32Window = *pu32WindowB;

    stAVInfo[eDeviceA].stShowFrameInfo.stVDECWinControl.win_id  = stVDECWinInfoA.win_id = *pu32WindowB;
    stAVInfo[eDeviceA].stShowFrameInfo.stVDECWinControl.u8EnableSetDigitalSignal   = stVDECWinInfoA.u8EnableSetDigitalSignal   = 2;
    stAVInfo[eDeviceA].stShowFrameInfo.stVDECWinControl.u8EnableClearDigitalSignal = stVDECWinInfoA.u8EnableClearDigitalSignal = 2;

    Demo_AV_SetShowFrameMode(&eDeviceA, (MS_U32 *)&stAVShowFrameInfoB.eShowFrameMode, pu32WindowB, &stAVShowFrameInfoB.u16X, &stAVShowFrameInfoB.u16Y, &stAVShowFrameInfoB.u16Width, &stAVShowFrameInfoB.u16Height, &stAVShowFrameInfoB.u16Layer);

    // Device B use window A
    stAVInfo[eDeviceB].stShowFrameInfo.u32Window = *pu32WindowA;

    stAVInfo[eDeviceB].stShowFrameInfo.stVDECWinControl.win_id  = stVDECWinInfoB.win_id = *pu32WindowA;
    stAVInfo[eDeviceB].stShowFrameInfo.stVDECWinControl.u8EnableSetDigitalSignal   = stVDECWinInfoB.u8EnableSetDigitalSignal   = 2;
    stAVInfo[eDeviceB].stShowFrameInfo.stVDECWinControl.u8EnableClearDigitalSignal = stVDECWinInfoB.u8EnableClearDigitalSignal = 2;

    Demo_AV_SetShowFrameMode(&eDeviceB, (MS_U32 *)&stAVShowFrameInfoA.eShowFrameMode, pu32WindowA, &stAVShowFrameInfoA.u16X, &stAVShowFrameInfoA.u16Y, &stAVShowFrameInfoA.u16Width, &stAVShowFrameInfoA.u16Height, &stAVShowFrameInfoA.u16Layer);

    Demo_VDEC_SetMode((EN_VDEC_Device *)&eDeviceA,E_VDEC_SetCmd_SetVDECWinInfo,(void *)(&stVDECWinInfoA));
    Demo_VDEC_SetMode((EN_VDEC_Device *)&eDeviceB,E_VDEC_SetCmd_SetVDECWinInfo,(void *)(&stVDECWinInfoB));

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief to Demo_AV_ChangeAudioTrack \n
/// - For HbbTV:
///     - Change DTV language
/// @param[in]  0 : for Main 1: for Sub
/// @param[in]  1 : audio pid
/// @param[in]  2 : audio codec type. Ex. AC3 or AAC ....etc
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command:  \b Demo_AV_ChangeAudioTrack  \n
//------------------------------------------------------------------------------
MS_BOOL Demo_AV_ChangeAudioTrack(const EN_AV_Device* peDevice, const MS_U32* pu32AudioPid, const EN_AUDIO_CODEC_TYPE* peAudioCodecType)
{
    DEMO_AV_CHECK_DEVICE_ID(*peDevice)

    EN_DEMO_DMX_FLOW eDMX_FLOW = stAVInfo[*peDevice].stDMXflowParams.eFlow;
    EN_PCR_ENG eSetSTC = stAVInfo[*peDevice].stPCRParams.ePCREngID;
    MS_U8 u8Volume = DEFAULT_VOLUME;

    db_print("[%s][%d] device %d\n", __FUNCTION__, __LINE__,(int)*peDevice);
    db_print("[%s][%d] audio pid %d\n", __FUNCTION__, __LINE__, (int)(*pu32AudioPid));

    if (MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Get_MultiPlayer_Capability) == 1)
    {
        stAVInfo[*peDevice].stAudioParams.eFIFO_ID = Demo_AV_GetAudioFIFO((EN_AV_Device*)peDevice);
    }
    else
    {
    stAVInfo[*peDevice].stAudioParams.eFIFO_ID = E_DMX_FLT_TYPE_AUD0;
    }

    stAVInfo[*peDevice].stAudioParams.u16PID = (*pu32AudioPid) & 0x1FFF;
    stAVInfo[*peDevice].stAudioParams.eAudioCodec = *peAudioCodecType;


    if (TRUE != Demo_DMX_AVFifo_Reset(stAVInfo[*peDevice].stAudioParams.eFIFO_ID,TRUE))
    {
        printf("[%s][%d]Demo DMX AudioFIFO Reset fail\n", __FUNCTION__, __LINE__);
        goto Demo_AV_ChangeAudioTrack_fail;
    }

    if (stAVInfo[*peDevice].stAudioParams.u8Filter != INVALID_FILTER_ID)
    {
        MApi_DMX_Stop(stAVInfo[*peDevice].stAudioParams.u8Filter);
        MApi_DMX_Close(stAVInfo[*peDevice].stAudioParams.u8Filter);
        stAVInfo[*peDevice].stAudioParams.u8Filter = INVALID_FILTER_ID;
        db_print("[%s][%d] MApi_DMX_Stop AudioFilter:%d\n", __FUNCTION__, __LINE__,stAVInfo[*peDevice].stAudioParams.u8Filter);
    }

    if (stAVInfo[*peDevice].eAVSYNC_Mode == E_AV_SYNC_PCR_MODE
        || stAVInfo[*peDevice].eAVSYNC_Mode == E_AV_SYNC_VIDEO_MASTER_MODE)
    {
        Demo_Audio_Stop(*peDevice, AUDIO_APP_DTV);
    }
    else if (stAVInfo[*peDevice].eAVSYNC_Mode == E_AV_SYNC_AUDIO_MASTER_MODE)
    {
        if (Demo_Audio_Stop(*peDevice, AUDIO_APP_DTV_AM) != TRUE)
        {
            printf("[%s][%d]Demo_Audio_Stop fail\n", __FUNCTION__, __LINE__);
            return FALSE;
        }
    }

    if (TRUE != Demo_DMX_FltOpen( eDMX_FLOW, stAVInfo[*peDevice].stAudioParams.eFIFO_ID, &stAVInfo[*peDevice].stAudioParams.u8Filter, stAVInfo[*peDevice].stAudioParams.u16PID))
    {
        printf("[%s][%d]Demo DMX AudioFlt Open fail \n", __FUNCTION__, __LINE__);
        goto Demo_AV_ChangeAudioTrack_fail;
    }


    if (DMX_FILTER_STATUS_OK != MApi_DMX_Start(stAVInfo[*peDevice].stAudioParams.u8Filter))
    {
        printf("[%s][%d]MApi DMX AudioFlt start fail \n", __FUNCTION__, __LINE__);
        goto Demo_AV_ChangeAudioTrack_fail;
    }


    db_print("[%s][%d] Device filters are ready\n", __FUNCTION__, __LINE__);


    if (stAVInfo[*peDevice].eAVSYNC_Mode == E_AV_SYNC_PCR_MODE
        || stAVInfo[*peDevice].eAVSYNC_Mode == E_AV_SYNC_VIDEO_MASTER_MODE)
    {
        if (Demo_Audio_Play(*peDevice,AUDIO_APP_DTV,stAVInfo[*peDevice].stAudioParams.eAudioCodec,&eSetSTC) == FALSE) // default mepg2
        {
            printf("[%s][%d] Demo_Audio_Play fail\n", __FUNCTION__, __LINE__);
            return FALSE;
        }
        else
        {
            db_print("[%s][%d] Demo_Audio_Play ok\n", __FUNCTION__, __LINE__);
        }

        //AVFIFO Reset
        if (TRUE != Demo_DMX_AVFifo_Reset(stAVInfo[*peDevice].stAudioParams.eFIFO_ID,FALSE))
        {
            printf("[%s][%d]Demo DMX AudioFIFO reset fail\n", __FUNCTION__, __LINE__);
            return FALSE;
        }

        //add audio unmute
        Demo_Audio_SetAbsoluteVolume(&u8Volume);

    }
    else if (stAVInfo[*peDevice].eAVSYNC_Mode == E_AV_SYNC_AUDIO_MASTER_MODE)
    {
        //Audio Master mode

        if (Demo_Audio_Play(*peDevice,AUDIO_APP_DTV_AM,stAVInfo[*peDevice].stAudioParams.eAudioCodec,&eSetSTC) == FALSE) // default mepg2
        {
            printf("Demo_Audio_Play fail\n");
            return FALSE;
        }
        else
        {
            db_print("Demo_Audio_Play ok\n");
        }

        if (TRUE != Demo_DMX_AVFifo_Reset(stAVInfo[*peDevice].stAudioParams.eFIFO_ID,FALSE))
        {
            printf("Demo DMX AudioFIFO reset fail\n");
            return FALSE;
        }

    }


    return TRUE;

Demo_AV_ChangeAudioTrack_fail:

    printf("Demo_AV_SetPid_fail \n");

    if (stAVInfo[*peDevice].stAudioParams.u8Filter != INVALID_FILTER_ID)
    {
        MApi_DMX_Stop(stAVInfo[*peDevice].stAudioParams.u8Filter);
        MApi_DMX_Close(stAVInfo[*peDevice].stAudioParams.u8Filter);
        stAVInfo[*peDevice].stAudioParams.u8Filter = INVALID_FILTER_ID;
    }

    return FALSE;
}

#endif
