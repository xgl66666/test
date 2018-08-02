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
    /// Main
    E_AV_DEVICE_MAIN = 0,
    /// Sub
    E_AV_DEVICE_SUB,
    /// Max Device
    E_AV_DEVICE_MAX,
} EN_AV_Device;

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
    E_AV_SWDetile_MODE,
    E_AV_DIP_MODE,
    E_AV_ShowFrame_MODE_MAX,
}EN_AV_ShowFrame_Type;

typedef enum
{
    E_AV_FileIn_Eng_MMFI = 0,
    E_AV_FileIn_Eng_FILE,
}EN_AV_FileIn_Eng_Type;

typedef enum
{
    E_AV_INFO_DMX_FLOW,
    E_AV_INFO_INPUT,
    E_AV_INFO_INVALID,
}EN_AV_INFO;

typedef enum
{
    E_AV_AVSYNC_STATE_RESET = 0,
    E_AV_AVSYNC_STATE_INIT,
    E_AV_AVSYNC_STATE_WAIT,
    E_AV_AVSYNC_STATE_MONITOR,
    E_AV_AVSYNC_STATE_IDLE,

    E_AV_AVSYNC_STATE_MAX,
    
}EN_AV_AVSYNC_TASk_STATE;

typedef struct
{
    EN_AV_FileIn_Eng_Type eEngType;
    MS_U8 u8EngID;
}ST_AV_FileIn_Eng_Info;

typedef struct
{
    MS_U8   u8Filter;  
    MS_U8   u8PCREngID;
    MS_U16  u16PID;
    
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

}ST_AV_DMX_INFO;

typedef struct
{
    MS_BOOL bInited;
    ST_AV_Params    stVideoParams;
    ST_AV_Params    stAudioParams;
    ST_AV_Params    stAudioADParams;
    ST_AV_Params    stPCRParams;

    ST_AV_DMX_INFO stDMXflowParams;

    EN_AV_AVSYNC_Type eAVSYNC_Mode;
    EN_AV_ShowFrame_Type eShowFrame_Mode;

    MS_BOOL bEnableAVSYNC_Task;
    EN_AV_AVSYNC_TASk_STATE eAVSYNC_Task_State;
    
}ST_AV_INFO;

MS_U64 Demo_AV_InitSTC(EN_AV_Device eDevice);
MS_U64 Demo_AV_GetSTC(EN_AV_Device eDevice);
MS_BOOL Demo_AV_SetSTC(EN_AV_Device eDevice,MS_U64 u64Stc);
MS_BOOL Demo_AV_SetSYNC(EN_AV_Device eDevice);
MS_BOOL Demo_AV_GetAVInfo(EN_AV_Device eDevice,ST_AV_INFO* pstAVInfo);
MS_BOOL Demo_AV_SetAVInfo(EN_AV_Device eDevice,EN_AV_INFO eAVInfo,void* pAVInfo);
MS_BOOL Demo_AV_Tuner_Config(EN_AV_Device* peDevice,EN_DEMO_DMX_FLOW* pePlayback,EN_DEMO_DMX_FLOW_INPUT* peDmxInput,int* pClkInv,int* pExtSync,int* pParal);
MS_BOOL Demo_AV_SetAVSYNCMode(EN_AV_Device* peDevice, EN_AV_AVSYNC_Type* peMode);
MS_BOOL Demo_AV_SetShowFrameMode(EN_AV_Device* peDevice, EN_AV_ShowFrame_Type* peMode);
MS_BOOL Demo_AV_TSP_SetPid(EN_AV_Device* peDevice,MS_U32* pu32VideoPid, MS_U32* pu32AudioPid, MS_U32* pu32PCRPid, MS_U32* pu32VCodec, MS_U32* pu32ACodec);
MS_BOOL Demo_AV_TSP_FileIn_SetPid(EN_AV_Device *peDevice,char *pFileInEngType,MS_U8 *pu8EngID,char *pVideoPid,char *pAudioPid,MS_U32 *pu32VCodec,MS_U32 *pu32ACodec);
MS_BOOL Demo_AV_PlayAV(EN_AV_Device* peDevice);
MS_BOOL Demo_AV_PauseAV(EN_AV_Device *peDevice);
MS_BOOL Demo_AV_StopAV(EN_AV_Device* peDevice);
MS_BOOL Demo_AV_FileIn_StopAV(EN_AV_Device *peDevice,char *pFileInEngType,MS_U8 *pu8EngID);
MS_BOOL Demo_AV_StopAVFIFO(EN_AV_Device *peDevice);
MS_BOOL Demo_AV_PlayRadio(EN_AV_Device* peDevice);
MS_BOOL Demo_AV_StopRadio(EN_AV_Device* peDevice);
MS_BOOL Demo_AV_MuteVideo(EN_AV_Device *peDevice);
MS_BOOL Demo_AV_PlayFromMemory(EN_AV_Device* peDevice,int* pVCodec, int* pPlayLoop, char* pPath );
MS_BOOL Demo_AV_StopFromMemory(EN_AV_Device* peDevice);
MS_BOOL Demo_AV_ChangeAudioTrack(const EN_AV_Device* peDevice, const MS_U32* pu32AudioPid, const EN_AUDIO_CODEC_TYPE* peAudioCodecType);
MS_BOOL Demo_AV_DecodeIFrame(EN_AV_Device* peDevice,int* pCodec,char* pLogopath);
MS_BOOL Demo_AV_SetLanguage(EN_AV_Device* peDevice, MS_U32 *u32LangIndex);
MS_BOOL Demo_AV_LanguageList(EN_AV_Device* peDevice);

#if(DEMO_AUDIO_MULTI_TEST == 1)
MS_BOOL Demo_AV_SetAudioInfo(EN_AV_Device eDevice);
#endif

#if(DEMO_AUDIO_AD_TEST == 1)
MS_BOOL Demo_AV_PlayAD(EN_AV_Device* peDevice, MS_U16* u16AudioADPID);
MS_BOOL Demo_AV_StopAD(EN_AV_Device* peDevice);
#endif

#if(DEMO_MVC_TEST == 1)
MS_BOOL Demo_AV_MVC_PlayFromMemory(int* pPlayLoop, char* pPath1,char* pPath2);
MS_BOOL Demo_AV_MVC_StopFromMemory(void);
#endif

#endif
