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

#ifndef _DEMO_AUDIO_MULTI_H_
#define _DEMO_AUDIO_MULTI_H_

#include "demo_audio_common.h"

#if(DEMO_AUDIO_MULTI_TEST == 1)

typedef struct
{
    AUDIO_DEC_ID eDecID;
    AudioDecStatus_t stAudioDecStatus_t;
}ST_AUDIO_DEC_INFO;

MS_BOOL Demo_Audio_Help(void);
MS_BOOL Demo_Audio_Init(MS_U32 *Type);
MS_BOOL Demo_Audio_Out_Init(MS_U32 *pFileFmt);
MS_BOOL Demo_Audio_SetOutputMode(MS_U32* mode);
MS_BOOL Demo_Audio_SetAbsoluteVolume(MS_U8* vol);
MS_BOOL Demo_Audio_Stop_Demo(void);
MS_BOOL Demo_Audio_SetMute(MS_BOOL*);
MS_BOOL Demo_Audio_SetMonitor(void);
MS_BOOL Demo_Audio_Debug(MS_U8* u8Level, MS_U16* u16Interval);
MS_BOOL Demo_Audio_Play(MS_U32 u32DevIdx, EN_AUDIO_APP_TYPE eAppType, EN_AUDIO_CODEC_TYPE eCodecType, void *pvParam);
MS_BOOL Demo_Audio_Stop(MS_U32 u32DevIdx, EN_AUDIO_APP_TYPE eAppType);
MS_BOOL Demo_Audio_GetDecInfo(MS_U32 u32DevIdx, ST_AUDIO_DEC_INFO *pstInfo);

MS_BOOL Demo_Audio_Set_Output_Source_Config(MS_U8* u8HDMI, MS_U8* u8LineOut, MS_U8* u8SPDIF, MS_U8* u8SCART, MS_U8* u8I2S);
MS_BOOL Demo_Audio_SetMutePath(MS_U8* u8Path, MS_U8* u8Mute);
MS_BOOL Demo_Audio_DMA_Reader_Test(MS_U8* pFilename, MS_U8* u8SampleRate, MS_U8* u8RepeatTime);
MS_BOOL Demo_Audio_Set_DRC_Mode(EN_AUDIO_CODEC_TYPE* eCodecType, MS_U8* mode);
MS_BOOL Demo_Audio_Set_DRC_Scale(MS_U32* u32CutScale, MS_U32* u32BoostScale);
MS_BOOL Demo_Audio_Set_DownMix_Mode(EN_AUDIO_CODEC_TYPE* eCodecType, MS_U8* mode);

MS_BOOL Demo_Audio_DMA_Reader_Test(MS_U8* pFilename, MS_U8* u8SampleRate, MS_U8* u8RepeatTime);

#if(DEMO_AUDIO_SE_DV258_TEST == 1)
MS_BOOL Demo_Audio_Set_DV258(MS_U8* u8Enable);
#endif

#if(DEMO_AUDIO_FMTX_TEST == 1)
MS_BOOL Demo_Audio_FMTX_Start(MS_U8* u8Filter, MS_U16* u16Attenuation, MS_U16* u16OutScaling);
MS_BOOL Demo_Audio_FMTX_Stop(void);
#endif

#if(DEMO_AUDIO_BTSC_TEST == 1)
MS_BOOL Demo_Audio_BTSC_Start(MS_U16* u16Attenuation, MS_U16* u16OutScaling, MS_U16* u16_M_Gain, MS_U16* u16_D_Gain, MS_U16* u16_SAP_Gain);
MS_BOOL Demo_Audio_BTSC_Stop(void);
#endif

#if(DEMO_AUDIO_HDMI_TEST == 1)
MS_BOOL Demo_Audio_Set_HDMITx_Mode(MS_U8* u8Mode);
MS_BOOL Demo_Audio_DDP(MS_U8* u8Mode);
#endif

#if(DEMO_AUDIO_SPDIF_TEST == 1)
MS_BOOL Demo_Audio_Set_SPDIF_Mode(MS_U8* u8Mode);
#endif

#if(DEMO_AUDIO_SOUND_EFFECT_TEST == 1)
MS_BOOL Demo_Audio_SetPreScale(MS_U8* u8PreScale);
MS_BOOL Demo_Audio_SetBass_Param(MS_U8* u8Bass);
MS_BOOL Demo_Audio_SetTreble_Param(MS_U8* u8Treble);
MS_BOOL Demo_Audio_SetBassTreble_Enable(MS_BOOL* bEnable);
MS_BOOL Demo_Audio_SetAVC_Param(MS_U8* AvcMode, MS_U8* Level, MS_U8* AvcAT, MS_U8* AvcRT);
MS_BOOL Demo_Audio_SetAVC_Enable(MS_BOOL* bEnable);
MS_BOOL Demo_Audio_SetPEQ_Param(MS_U8* Band, MS_U8* Gain, MS_U16* Fc, MS_U8* QValue);
MS_BOOL Demo_Audio_SetPEQ_Enable(MS_BOOL* bEnable);
MS_BOOL Demo_Audio_SetGEQ_Param(MS_U8* u8band, MS_U8* u8level);
MS_BOOL Demo_Audio_SetGEQ_Enable(MS_BOOL* bEnable);
MS_BOOL Demo_Audio_SetHPF_Enable(MS_BOOL* bEnable);
#endif

#if(DEMO_AUDIO_SOUND_EFFECT_ADV_TEST == 1)
MS_BOOL Demo_Audio_Set_TSHD_Param(MS_U8* u8SpeakerSize, MS_U8* u8Bass, MS_U8* u8Definition, MS_U8* u8Surround, MS_U8* u8InputGain, MS_U8* u8OutputGain);
MS_BOOL Demo_Audio_Set_TSHD_Enable(MS_BOOL* bEnable);
#endif

#if (DEMO_AUDIO_AD_TEST == 1)
MS_BOOL Demo_Audio_SetADAbsoluteVolume(MS_U8* vol);
MS_BOOL Demo_Audio_ADSetMute(MS_BOOL* bEnable);
#endif

#endif

#endif
