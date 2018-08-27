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

#ifndef _DEMO_AUDIO_H_
#define _DEMO_AUDIO_H_

#include "demo_audio_common.h"

#if(DEMO_AUDIO_TEST == 1)

typedef struct
{
    AUDIO_DEC_ID eDecID;
    AudioDecStatus_t stAudioDecStatus_t;
}ST_AUDIO_DEC_INFO;

#if (DEMO_AUDIO_ENCODER_TEST == 1)
typedef struct
{
    MS_S32 s32DeviceId;     //encoder device id
    MS_U8 u8Filename[80];
    MS_U32 u32RequireDumpSize;
    Task_Info stTask_Info;
}ST_AUDIO_ENC_DUMP_FILE_TASK;
#endif

#if (DEMO_AUDIO_FRAME_DECODE_TEST == 1)
typedef struct
{
    MS_S32 s32DeviceId;     //encoder device id
    MS_BOOL bOutFileAppend;
    EN_AUDIO_TRANSCODE_PATH enTranscodePath;
    EN_AUDIO_TRANSCODE_FORMAT enTranscodeFormat;
    EN_AUDIO_MONITOR_TASK_STATUS enTaskStatus;
    MS_U8 u8Filename[80];
    Task_Info stTask_Info;
}ST_AUDIO_TRANSCODE_TASK;
#endif

#if (((DEMO_AUDIO_DMA_READER_TEST == 1) || (DEMO_AUDIO_PCM_CAPTURE_TEST == 1)) && (DEMO_AUDIO_R2_MEM_ARCHI_TEST == 1))
typedef struct
{
    MS_S32 s32DeviceId;
    MS_U8 u8Filename[80];
    MS_BOOL bOutFileAppend;
    AUDIO_PCM_INFO_t tPcmInfo;
    MS_U32 u32RepeatLoop;
    EN_AUDIO_MONITOR_TASK_STATUS enTaskStatus;
    EN_AUDIO_PCM_CAPTURE_OUTPUT_PATH enOutputPath;
    AUDIO_CAPTURE_SOURCE_TYPE enCaptureSource;
    Task_Info stTask_Info;
}ST_AUDIO_PCM_IO_TASK;
#endif

typedef struct
{
    EN_AUDIO_MONITOR_TASK_STATUS enTaskStatus;
    Task_Info stTask_Info;
}ST_AUDIO_TASK;

typedef struct
{
    AUDIO_DEC_ID eDecID;
    MS_U32 u32MonitorType1;
    MS_U32 u32CodecType;
    MS_U32 u32PlayCmd;
    MS_U32 u32HDMIPCMMode;
    MS_U32 u32SPDIFPCMMode;
    MS_U32 u32Volume;
    ST_AUDIO_TASK stAudioTask_Info;
}ST_AUDIO_MONITOR_TEST_TASK;

MS_BOOL Demo_Audio_Help(void);
MS_BOOL Demo_Audio_Init(MS_U32 *Type);
MS_BOOL Demo_Audio_SwitchResource(MS_U32 *u32DevIdx1, MS_U32 *u32DevIdx2);
MS_BOOL Demo_Audio_Open(MS_U32 *u32DevIdx, AUDIO_OUTPORT_SOURCE_TYPE *eOutSrcType, EN_AUDIO_APP_TYPE *eAppType);
MS_BOOL Demo_Audio_Release(MS_U32 *u32DevIdx);
MS_BOOL Demo_Audio_Out_Init(MS_U32 *pFileFmt);
MS_BOOL Demo_Audio_SetDecOutputMode(MS_U32* u32DevIdx, MS_U32* mode);
MS_BOOL Demo_Audio_SetOutputMode(MS_U32* mode);
MS_BOOL Demo_Audio_SetAbsoluteVolume(MS_U8* vol);
MS_BOOL Demo_Audio_Stop_Demo(void);
MS_BOOL Demo_Audio_SetMute(MS_BOOL*);
MS_BOOL Demo_Audio_SetDecMute(MS_U32* u32DevIdx, MS_BOOL* bEnable);
MS_BOOL Demo_Audio_SetMonitor(void);
MS_BOOL Demo_Audio_Debug(MS_U8* u8Level, MS_U16* u16Interval);
MS_BOOL Demo_Audio_Play(MS_U32 u32DevIdx, EN_AUDIO_APP_TYPE eAppType, EN_AUDIO_CODEC_TYPE eCodecType, void *pvParam);
MS_BOOL Demo_Audio_Stop(MS_U32 u32DevIdx, EN_AUDIO_APP_TYPE eAppType);
MS_BOOL Demo_Audio_GetDecInfo(MS_U32 u32DevIdx, ST_AUDIO_DEC_INFO *pstInfo);
MS_BOOL Demo_Audio_Set_Output_Source_Config(MS_U8* u8HDMI, MS_U8* u8LineOut, MS_U8* u8SPDIF, MS_U8* u8SCART, MS_U8* u8I2S);
MS_BOOL Demo_Audio_SetMutePath(MS_U8* u8Path, MS_U8* u8Mute);
MS_BOOL Demo_Audio_Set_DRC_Mode(EN_AUDIO_CODEC_TYPE* eCodecType, MS_U8* mode);
MS_BOOL Demo_Audio_Set_DRC_Scale(MS_U32* u32CutScale, MS_U32* u32BoostScale);
MS_BOOL Demo_Audio_Set_DownMix_Mode(EN_AUDIO_CODEC_TYPE* eCodecType, MS_U8* mode);
MS_BOOL Demo_Audio_Set_DRC_Mode_EX(AUDIO_DEC_ID* DecId, MS_VIRT* mode);
MS_BOOL Demo_Audio_Set_DRC_Scale_EX(AUDIO_DEC_ID* DecId, MS_U32* u32CutScale, MS_U32* u32BoostScale);
MS_BOOL Demo_Audio_Set_DownMix_Mode_EX(AUDIO_DEC_ID* DecId, MS_VIRT* mode);
MS_BOOL Demo_Audio_Set_TB11(MS_U8* mode);
MS_BOOL Demo_Audio_SyncSTC(AUDIO_DEC_ID DecId, MS_U32 u32Param );
MS_BOOL Demo_Audio_GetSyncSTCStatus(AUDIO_DEC_ID DecId, MS_U32* u32Param );
MS_BOOL Demo_Audio_GetDecStatus(AUDIO_DEC_ID DecId, MS_U8* u8Param );
MS_BOOL Demo_Audio_GetCPTS(AUDIO_DEC_ID DecId, MS_U64* u64Param );
MS_BOOL Demo_Audio_Set_Dynamic_Allocate_Resource(MS_U32* u32Mode);
MS_BOOL Demo_Audio_Dynamic_Allocate_Resource(void);
MS_BOOL Demo_Audio_Codec_Delay(EN_AUDIO_CODEC_TYPE* eCodecType, MS_U32* u32Delayms);
MS_BOOL Demo_Audio_Codec_Delay_EX(AUDIO_DEC_ID* DecId, EN_AUDIO_CODEC_TYPE* eCodecType, MS_U32* u32Delayms);
MS_BOOL Demo_Audio_Delay(MS_U8* u8Delayms);
MS_BOOL Demo_Audio_Set_DTSHD(MS_U8* u8Mode);
MS_BOOL Demo_Audio_Debug_WriteReg(MS_U32* pu32Bank, MS_U32* pu32Offset, MS_U32* pu32RegValue);
MS_BOOL Demo_Audio_Exit(void);
MS_BOOL Demo_Audio_IR_Debug(MS_U32* u32Enable);
MS_BOOL Demo_Audio_Debug_R2(MS_U32* u32R2_CMD);
MS_BOOL Demo_Audio_PUSI(MS_U32* u32DevIdx, MS_U32* u32enable);


#if (DEMO_AUDIO_FRAME_DECODE_TEST == 1)
MS_BOOL Demo_Audio_Transcode_Start(MS_U32 *u32DevIdx, EN_AUDIO_TRANSCODE_PATH* enOutputPath, EN_AUDIO_TRANSCODE_FORMAT* enOutputFormat, MS_U32* u32OutputFilename, MS_BOOL* bOutFileAppend);
MS_BOOL Demo_Audio_Transcode_Stop(MS_U32 *u32DevIdx);
MS_BOOL Demo_Audio_Transcode_Pause(MS_U32 *u32DevIdx);
#endif

#if (DEMO_AUDIO_DMA_READER_TEST == 1)
MS_BOOL Demo_Audio_DMA_Reader_Test(MS_U8* pFilename, MS_U8* u8SampleRate, MS_U8* u8RepeatTime);
#endif

#if ((DEMO_AUDIO_DMA_READER_TEST == 1) && (DEMO_AUDIO_R2_MEM_ARCHI_TEST == 1))
MS_BOOL Demo_Audio_DMA_Reader_EX_Test(MS_U32* u32DevIdx, MS_U8* pFilename, MS_U32* u32ChannelNum, MS_U32* u32SampleRate, MS_U32* u32BitWidth, MS_U32* u32RepeatLoop);
#endif

#if (DEMO_XC_HDMIRX_TEST == 1)
MS_BOOL Demo_Audio_EDID_Output(MS_U32* u32DevIdx, MS_VIRT* pmode);
MS_BOOL Demo_Audio_EDID_Output_EX(AUDIO_DEC_ID* enDecId, MS_VIRT* pmode);
#endif

#if(DEMO_AUDIO_AD_FADER_CONTROL_TEST == 1)
MS_BOOL Demo_Audio_Set_AD_Fader(MS_U8* u8mode);
#endif

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
MS_BOOL Demo_Audio_DDP(MS_VIRT* pmode);
MS_BOOL Demo_Audio_Show_HDMITx_EDID(void);
MS_BOOL Demo_Audio_HDMITx_EDID_Check_Support_Codec(En_DVB_decSystemType enCurCodec);
MS_BOOL Demo_Audio_Check_Support_HDMITx_Mode(MS_U8 u8Mode);
MS_BOOL Demo_Audio_Check_Support_DDPBYPASS_Mode(MS_VIRT mode);
MS_BOOL Demo_Audio_HDMI_Delay(MS_U32* u32Delayms);
#endif

#if(DEMO_AUDIO_SPDIF_TEST == 1)
MS_BOOL Demo_Audio_Set_SPDIF_Mode(MS_U8* u8Mode);
MS_BOOL Demo_Audio_Check_Support_SPDIF_Mode(MS_U8 u8Mode);
MS_BOOL Demo_Audio_SPDIF_Delay(MS_U32* u32Delayms);
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
MS_BOOL Demo_Audio_SetBalance_Enable(MS_U32* u32Enable);
MS_BOOL Demo_Audio_SetBalance_Param(MS_U32* u32Balance);
#endif

#if(DEMO_AUDIO_SOUND_EFFECT_ADV_TEST == 1)
MS_BOOL Demo_Audio_Set_TSHD_Param(MS_U8* u8SpeakerSize, MS_U8* u8Bass, MS_U8* u8Definition, MS_U8* u8Surround, MS_U8* u8InputGain, MS_U8* u8OutputGain);
MS_BOOL Demo_Audio_Set_TSHD_Enable(MS_BOOL* bEnable);
#endif

#if (DEMO_AUDIO_AD_TEST == 1)
MS_BOOL Demo_Audio_SetADAbsoluteVolume(MS_U8* vol);
MS_BOOL Demo_Audio_ADSetMute(MS_BOOL* bEnable);
#endif

#if defined (DEMO_AUDIO_SUPPORT_MS12_B_TEST) && (DEMO_AUDIO_SUPPORT_MS12_B_TEST == 1)
MS_BOOL Demo_Audio_SetDAP_Enable(MS_BOOL* bEnable);
MS_BOOL Demo_Audio_SetDAP_Param(char* filename);
#endif

#if (DEMO_AUDIO_ENCODER_TEST == 1)
MS_BOOL Demo_Audio_Enc_Open(MS_U32 *u32EncIdx, char* encode_type);
MS_BOOL Demo_Audio_Enc_Close(MS_U32 *u32EncIdx);
MS_BOOL Demo_Audio_Enc_Dump(MS_U32 *u32EncIdx, char* outputfilename, MS_U32* u32RequireDumpSize);
MS_BOOL Demo_Audio_Enc_Flush(MS_U32 *u32EncIdx);
MS_BOOL Demo_Audio_Enc_Ioctl(MS_U32 *u32EncIdx, char* pCmd, MS_U32 *u32Data);
#endif

#if (DEMO_AUDIO_PCM_CAPTURE_TEST == 1)
MS_BOOL Demo_Audio_PCM_Capture_Start(MS_U32 *u32DevIdx, AUDIO_CAPTURE_SOURCE_TYPE* enCaptureSource, MS_U32* u32channel, MS_U32* u32SampleRate, EN_AUDIO_PCM_CAPTURE_OUTPUT_PATH* enOutputPath, MS_U32* u32OutputFilename, MS_BOOL* bOutFileAppend);
MS_BOOL Demo_Audio_PCM_Capture_Stop(MS_U32 *u32DevIdx);
#endif

MS_BOOL Demo_Audio_Test_Monitor_Start(AUDIO_DEC_ID* eDecID, MS_U32* u32MonitorType, MS_U32* u32CodecType, MS_U32* u32PlayCmd, MS_U32* u32HDMIPCMMode, MS_U32* u32SPDIFPCMMode, MS_U32* u32Volume);
MS_BOOL Demo_Audio_Test_Monitor_Stop(void);

#endif

#endif
