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

MS_BOOL Demo_Audio_Help(void);
MS_BOOL Demo_Audio_Init(MS_U32 *Type);
MS_BOOL Demo_Audio_Out_Init(MS_U32 *pFileFmt);
MS_BOOL Demo_Audio_SetOutputMode(MS_U32* mode);
MS_BOOL Demo_Audio_SetAbsoluteVolume(MS_U8*);
MS_BOOL Demo_Audio_Stop_Demo(void);
MS_BOOL Demo_Audio_SetMute(MS_BOOL*);
MS_BOOL Demo_Audio_SetMonitor(void);
MS_BOOL Demo_Audio_Debug(MS_U8* u8Level, MS_U16* u16Interval);
MS_BOOL Demo_Audio_Play(MS_U32 u32DevIdx, EN_AUDIO_APP_TYPE eAppType, EN_AUDIO_CODEC_TYPE eCodecType, void *pvParam);
MS_BOOL Demo_Audio_Stop(MS_U32 u32DevIdx, EN_AUDIO_APP_TYPE eAppType);
MS_BOOL Demo_Audio_DMA_Reader_Test(MS_U8* pFilename, MS_U8* u8SampleRate, MS_U8* u8RepeatTime);
MS_BOOL Demo_Audio_Set_DRC_Mode(EN_AUDIO_CODEC_TYPE* eCodecType, MS_U8* mode);
MS_BOOL Demo_Audio_Set_DRC_Scale(MS_U32* u32CutScale, MS_U32* u32BoostScale);
MS_BOOL Demo_Audio_Set_DownMix_Mode(EN_AUDIO_CODEC_TYPE* eCodecType, MS_U8* mode);
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
#endif

#if(DEMO_AUDIO_SOUND_EFFECT_ADV_TEST == 1)
#endif

#if (DEMO_AUDIO_AD_TEST == 1)
MS_BOOL Demo_Audio_SetADAbsoluteVolume(MS_U8* vol);
MS_BOOL Demo_Audio_ADSetMute(MS_BOOL* bEnable);
#endif

#endif

#endif
