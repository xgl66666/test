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


#ifndef _DEMO_AV_H_
#define _DEMO_AV_H_ 1

#define INVALID_FILEIN_TYPE 0xFF
#define INVALID_FILEIN_ID   0xFF
#define INVALID_FILTER_ID   0xFF
#define INVALID_PID_ID      0x1FFF

#include "MsCommon.h"
#include "demo_audio_common.h"
#include "demo_dmx.h"
#include "demo_vdec.h"

typedef enum
{
    E_AV_DEVICE_INVALID = -1,
    E_AV_DEVICE_MAIN = 0,   /// Main
    E_AV_DEVICE_SUB,        /// Sub
#if (DEMO_VDEC_NDECODE_TEST == 1)
    E_AV_DEVICE_FIRST = E_AV_DEVICE_MAIN,
    E_AV_DEVICE_SECOND,
    E_AV_DEVICE_THIRD,
    E_AV_DEVICE_FOURTH,
#endif
    E_AV_DEVICE_MAX,        /// Max Device
} EN_AV_Device;

typedef enum
{
    E_AV_AUDIO_SWITCH_NULL = -1,
    E_AV_AUDIO_SWITCH_TO_DTV = 0,
    E_AV_AUDIO_SWITCH_TO_MM,
    E_AV_AUDIO_SWITCH_MAX,
} EN_AV_AUDIO_SWITCH_TYPE;

typedef enum
{
    E_AV_SYNC_PCR_MODE = 0,
    E_AV_SYNC_AUDIO_MASTER_MODE,
    E_AV_SYNC_VIDEO_MASTER_MODE,
    E_AV_SYNC_MODE_MAX,
}EN_AV_AVSYNC_Type;

typedef enum
{
    E_AV_MVOP_MODE = 0,
    E_AV_DIP_MODE = 2,
    E_AV_SWDetile_MODE,
    E_AV_PureMCU_MODE,
    E_AV_ShowFrame_MODE_MAX,
}EN_AV_ShowFrame_Mode;

typedef enum
{
    E_AV_FileIn_Eng_MMFI = 0,
    E_AV_FileIn_Eng_FILE,
}EN_AV_FileIn_Eng_Type;

typedef enum
{
    E_AV_FileIn_Eng_TS = 0,
    E_AV_FileIn_Eng_PES,
    E_AV_FileIn_Eng_MAX
}EN_AV_FileIn_Eng_Format;

typedef enum
{
    E_AV_AVSYNC_STATE_RESET = 0,
    E_AV_AVSYNC_STATE_INIT,
    E_AV_AVSYNC_STATE_WAIT,
    E_AV_AVSYNC_STATE_MONITOR,
    E_AV_AVSYNC_STATE_IDLE,
    E_AV_AVSYNC_STATE_MAX,
}EN_AV_AVSYNC_TASk_STATE;

typedef enum
{
    E_AV_AVINIT_STATE_RESET = 0,
    E_AV_AVINIT_STATE_TUNER,
    E_AV_AVINIT_STATE_DMX,
    E_AV_AVINIT_STATE_AUDIO,
    E_AV_AVINIT_STATE_VIDEO,
    E_AV_AVINIT_STATE_MAX,
}EN_AV_AVINIT_STATE;

typedef enum
{
    //Get demo_AV Info
    E_AV_GetCmd_LiveInfo = 0x0,
    E_AV_GetCmd_VideoInfo,
    E_AV_GetCmd_AudioInfo,
    E_AV_GetCmd_AudioADInfo,
    E_AV_GetCmd_PCRInfo,
    E_AV_GetCmd_DMXFlowInfo,
    E_AV_GetCmd_WindowInfo,
    E_AV_GetCmd_WindowShowFrameMode,
    E_AV_GetCmd_MAX,

    //Set demo_AV Info
    E_AV_SetCmd_DMXFlow = 0x100,
    E_AV_SetCmd_DMXInput,
    E_AV_SetCmd_NotInitialize,
    E_AV_SetCmd_MAX,
} EN_AV_Cmd;

typedef struct
{
    EN_AV_FileIn_Eng_Type eEngType;
    MS_U8 u8EngID;
    EN_AV_FileIn_Eng_Format eEngFormat;
}ST_AV_FileIn_Eng_Info;

typedef struct
{
    MS_U8       u8Filter;
    EN_PCR_ENG  ePCREngID;
    MS_U16      u16PID;

    EN_DEMO_DMX_FLT_TYPE eFIFO_ID;
    ST_AV_FileIn_Eng_Info stFilterType;

    //video part
    VDEC_StreamId stVDECSteamID;
    EN_VDEC_DDI_CodecType eVideoCodec;

    //Audio part
    EN_AUDIO_CODEC_TYPE   eAudioCodec;

}ST_AV_Params;

typedef struct
{
    EN_DEMO_DMX_FLOW eFlow;
    EN_DEMO_DMX_FLOW_INPUT eDmxInput;
    int   DmxClkInv;
    int   DmxExtSync;
    int   DmxParallal;
    // For merge stream. Store current source id for main/sub after Demo_AV_TSP_SetPid being called
    EN_DEMO_DMX_FLT_SOURCEID eDmxFltSourceID;
}ST_AV_DMX_INFO;

typedef struct
{
    MS_BOOL bValid;
    EN_AV_Device eDevice;
    EN_AV_ShowFrame_Mode eShowFrameMode;

    MS_U32 u32Window;
    MS_U16 u16X;
    MS_U16 u16Y;
    MS_U16 u16Width;
    MS_U16 u16Height;
    MS_U16 u16Layer;

    EN_VDEC_DDI_DISPLAY_PATH eShowFrame_MvopPath;
    ST_VDEC_WIN_INFO stVDECWinControl;

}ST_AV_ShowFrameInfo;

typedef struct
{
    MS_BOOL bInited;
    ST_AV_Params    stVideoParams;
    ST_AV_Params    stAudioParams;
    ST_AV_Params    stAudioADParams;
    ST_AV_Params    stPCRParams;

    ST_AV_DMX_INFO stDMXflowParams;

    EN_AV_AVSYNC_Type eAVSYNC_Mode;
    ST_AV_ShowFrameInfo stShowFrameInfo;

    MS_BOOL bEnableAVSYNC_Task;
    EN_AV_AVSYNC_TASk_STATE eAVSYNC_Task_State;

    MS_BOOL bInitializeOrNot; //if need not to do initialize, set the variable to TRUE;
    EN_AV_AVINIT_STATE eAVInitState;

}ST_AV_INFO;

MS_U64 Demo_AV_InitSTC(EN_AV_Device* peDevice);
MS_U64 Demo_AV_GetSTC(EN_AV_Device* peDevice);
MS_BOOL Demo_AV_Initialize(EN_AV_Device* peDevice);
MS_BOOL Demo_AV_SetSTC(EN_AV_Device* peDevice,MS_U64 u64Stc);
MS_BOOL Demo_AV_SetSYNC(EN_AV_Device* peDevice);
MS_BOOL Demo_AV_GetAVInfo(EN_AV_Device* peDevice,EN_AV_Cmd eAVCmd,void* pAVInfo);
MS_BOOL Demo_AV_SetAVInfo(EN_AV_Device* peDevice,EN_AV_Cmd eAVCmd,void* pAVInfo);
MS_BOOL Demo_AV_MuteVideo(EN_AV_Device *peDevice,MS_BOOL *pbDisplayMute);
MS_BOOL Demo_AV_Tuner_Config(EN_AV_Device* peDevice,EN_DEMO_DMX_FLOW* pePlayback,EN_DEMO_DMX_FLOW_INPUT* peDmxInput,int* pClkInv,int* pExtSync,int* pParal);
MS_BOOL Demo_AV_SetAVSYNCMode(EN_AV_Device* peDevice, EN_AV_AVSYNC_Type* peMode);
MS_BOOL Demo_AV_SetShowFrameMode(EN_AV_Device* peDevice, MS_U32* pu32Mode, MS_U32* pu32Window, MS_U16* pu16X, MS_U16* pu16Y, MS_U16* pu16Width, MS_U16* pu16Height, MS_U16* pu16Layer);
MS_BOOL Demo_AV_ChangeDispWindow(MS_U32* pu32Window, MS_U16* pu16X, MS_U16* pu16Y, MS_U16* pu16Width, MS_U16* pu16Height, MS_U16* pu16Layer);
MS_BOOL Demo_AV_TSP_SetPid(EN_AV_Device* peDevice,MS_U32* pu32VideoPid, MS_U32* pu32AudioPid, MS_U32* pu32PCRPid, MS_U32* pu32VCodec, MS_U32* pu32ACodec);
MS_BOOL Demo_AV_TSP_FileIn_SetPid(EN_AV_Device *peDevice,char *pFileInEngType,MS_U8 *pu8EngID,char *pVideoPid,char *pAudioPid,char *pVCodec,char *pACodec);
MS_BOOL Demo_AV_PlayAV(EN_AV_Device* peDevice);
MS_BOOL Demo_AV_PauseAV(EN_AV_Device *peDevice);
MS_BOOL Demo_AV_StopAV(EN_AV_Device* peDevice);
MS_BOOL Demo_AV_FileIn_StopAV(EN_AV_Device *peDevice,char *pFileInEngType,MS_U8 *pu8EngID);
MS_BOOL Demo_AV_StopAVFIFO(EN_AV_Device *peDevice);
MS_BOOL Demo_AV_PlayRadio(EN_AV_Device* peDevice);
MS_BOOL Demo_AV_StopRadio(EN_AV_Device* peDevice);
MS_BOOL Demo_AV_PlayFromMemory(EN_AV_Device* peDevice,int* pVCodec, int* pPlayLoop, char* pPath );
MS_BOOL Demo_AV_StopFromMemory(EN_AV_Device* peDevice);
MS_BOOL Demo_AV_ChangeAudioTrack(const EN_AV_Device* peDevice, const MS_U32* pu32AudioPid, const EN_AUDIO_CODEC_TYPE* peAudioCodecType);
MS_BOOL Demo_AV_DecodeIFrame(EN_AV_Device* peDevice,int* pCodec,char* pLogopath);
MS_BOOL Demo_AV_SetLanguage(EN_AV_Device* peDevice, MS_U32 *u32LangIndex);
MS_BOOL Demo_AV_LanguageList(EN_AV_Device* peDevice);
EN_DEMO_DMX_FLT_TYPE Demo_AV_GetAudioFIFO(EN_AV_Device* peDevice);
EN_DEMO_DMX_FLT_TYPE Demo_AV_GetAudioADFIFO(EN_AV_Device* peDevice);
MS_BOOL Demo_AV_Audio_Switch(EN_AV_Device* peDevice, EN_AV_AUDIO_SWITCH_TYPE* peSwitchType);
MS_BOOL Demo_AV_CheckAudioResource(EN_AV_Device* peDevice);
MS_BOOL Demo_AV_CheckMainAudioResource(EN_AV_Device* peDevice);
MS_BOOL Demo_AV_ExChangeWindow(MS_U32* pu32WindowA, MS_U32* pu32WindowB);

#if(DEMO_VDEC_NDECODE_TEST == 1)
EN_AV_Device Demo_AV_VDEC_Device_Mapping(EN_AV_Device eDevice);
#endif

#if(DEMO_AUDIO_AD_TEST == 1)
MS_BOOL Demo_AV_PlayAD(EN_AV_Device* peDevice, MS_U16* u16AudioADPID);
MS_BOOL Demo_AV_StopAD(EN_AV_Device* peDevice);
#endif

#if(DEMO_AUDIO_AD_1PID_TEST == 1)
MS_BOOL Demo_AV_PlayAD_1PID(EN_AV_Device* peDevice);
MS_BOOL Demo_AV_StopAD_1PID(EN_AV_Device* peDevice);
#endif

#if(DEMO_MVC_TEST == 1)
MS_BOOL Demo_AV_MVC_PlayFromMemory(int* pPlayLoop, char* pPath1,char* pPath2);
MS_BOOL Demo_AV_MVC_StopFromMemory(void);
#endif

#endif
