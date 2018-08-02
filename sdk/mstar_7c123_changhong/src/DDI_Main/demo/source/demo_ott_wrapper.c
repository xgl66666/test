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
/*
 * Copyright (C) 2011 MStar Semiconductor, Inc.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 * 3.  Neither the name of MStar nor the names of
 *     its contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY MSTAR AND ITS CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL MSTAR OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#if ((DEMO_DRM_TEST == 1) || (DEMO_HBBTV_TEST == 1) || (DEMO_APM_TEST == 1) || (DEMO_QT_TEST == 1) || (DEMO_DFB_TEST == 1))

#include "MsCommon.h"
#include "mmsdk_interface_def.h"

#include "demo_xc.h"
#include "demo_dmx.h"
#include "demo_av.h"
#include "demo_vdec.h"
#include "demo_mm.h"
#include "demo_audio.h"

// demo_xc.h
MS_BOOL Demo_OTT_XC_Init(MS_U32 *pu32XCDevice)
{
    Demo_XC_Init(pu32XCDevice);
    return TRUE;
}

MS_BOOL Demo_OTT_VE_Init(void)
{
    Demo_VE_Init();
    return TRUE;
}

MS_BOOL Demo_OTT_XC_SetOutputTiming(MS_U32 *pu32XCDevice, MS_U32 *xcOutputTiming)
{
    Demo_XC_SetOutputTiming(pu32XCDevice, xcOutputTiming);
    return TRUE;
}

MS_BOOL Demo_OTT_VE_SetOutputTiming(MS_U32 *veOutputTiming)
{
    Demo_VE_SetOutputTiming(veOutputTiming);
    return TRUE;
}

MS_BOOL Demo_OTT_XC_SetDestWin(MS_U16 x, MS_U16 y, MS_U16 width, MS_U16 Height)
{
    MS_U32 u32XCDevice = 0;
    MS_U32 u32XCWindow = 0;

    Demo_XC_SetDestWin(&u32XCDevice, &u32XCWindow, &x, &y, &width, &Height);
    return TRUE;
}

MS_BOOL Demo_OTT_XC_PlayVideo(void)
{
    MS_U32 u32XCDevice = 0;
    MS_U32 xcWindow = E_AV_DEVICE_MAIN;
    MS_U32 xcInputSrc = E_DDI_XC_INPUT_SOURCE_DTV;

    Demo_XC_PlayVideo(&u32XCDevice, &xcWindow, &xcInputSrc);
    return TRUE;
}

MS_BOOL Demo_OTT_XC_PlayCusVideo(MS_U32 *pu32XCDevice, MS_U32 *pu32XCWindow, MS_U32 *pu32XCInputSrc)
{
    Demo_XC_PlayCusVideo(pu32XCDevice, pu32XCWindow, pu32XCInputSrc);
    return TRUE;
}

MS_BOOL Demo_OTT_VE_PlayVideo(void)
{
    Demo_VE_PlayVideo();
    return TRUE;
}

MS_BOOL Demo_OTT_SetDisplayPath(void)
{
    return TRUE;
}

// demo_dmx.h
MS_BOOL Demo_OTT_DMX_Init(void)
{
    Demo_DMX_Init();
    return TRUE;
}


// demo_av.h
MS_BOOL Demo_OTT_AV_ChangeAudioTrack(const MS_U32* pu32AudioPid, const MS_U32* peAudioCodecType)
{
    EN_AV_Device eDevice = E_AV_DEVICE_MAIN;

    Demo_AV_ChangeAudioTrack(&eDevice, pu32AudioPid, (const EN_AUDIO_CODEC_TYPE*) peAudioCodecType);
    return TRUE;
}

MS_BOOL Demo_OTT_AV_StopAV(void)
{
    EN_AV_Device eDevice = E_AV_DEVICE_MAIN;

    return Demo_AV_StopAV(&eDevice);
}

MS_BOOL Demo_OTT_AV_Tuner_Config(MS_U32* peDmxInput)
{
    EN_AV_Device eDevice = E_AV_DEVICE_MAIN;
    int clkInv = 0;
    int ExtSync = 1;
    int Paral = 1;
    EN_DEMO_DMX_FLOW Playback = E_DMX_FLOW_LIVE0;

    Demo_AV_Tuner_Config(&eDevice, &Playback,(EN_DEMO_DMX_FLOW_INPUT*)peDmxInput, &clkInv, &ExtSync, &Paral);
    return TRUE;
}

MS_BOOL Demo_OTT_AV_TSP_SetPid(MS_U32* pu32VideoPid, MS_U32* pu32AudioPid, MS_U32* pu32PCRPid, MS_U32* pu32VCodec, MS_U32* pu32ACodec)
{
    EN_AV_Device eDevice = E_AV_DEVICE_MAIN;

    return Demo_AV_TSP_SetPid(&eDevice, pu32VideoPid, pu32AudioPid, pu32PCRPid, pu32VCodec, pu32ACodec);
}

MS_BOOL Demo_OTT_AV_PlayAV(void)
{
    EN_AV_Device eDevice = E_AV_DEVICE_MAIN;

    return  Demo_AV_PlayAV(&eDevice);
}

// demo_vdec.h

MS_BOOL Demo_OTT_VDEC_CheckDecoderStatus(void)
{
    EN_VDEC_Device eVDevice = E_VDEC_DEVICE_MAIN;

    Demo_VDEC_CheckDecoderStatus(&eVDevice);

    return TRUE;
}

MS_BOOL Demo_OTT_VDEC_IsAVSyncDone(void)
{
    EN_VDEC_Device eVDevice = E_VDEC_DEVICE_MAIN;

    Demo_VDEC_IsAVSyncDone(eVDevice);

    return TRUE;
}
MS_BOOL Demo_OTT_VDEC_GetStreamID(VDEC_StreamId* pstStreamID)
{
    EN_VDEC_Device eVDevice = E_VDEC_DEVICE_MAIN;
    pstStreamID = (VDEC_StreamId*)Demo_VDEC_GetStreamID(eVDevice);
    return TRUE;
}

// demo_mm.h

MS_BOOL Demo_OTT_MM_INIT(void)
{
    return Demo_MM_Init();
}

MS_BOOL Demo_OTT_MM_DeINIT(void)
{
    return Demo_MM_DeInit();
}

MS_BOOL Demo_OTT_MM_AllocAudDec(void)
{
    MS_BOOL bRet = TRUE;
#if(DEMO_AUDIO_MULTI_TEST == 1)
    bRet = Demo_MM_AllocAudDec();
#endif
    return bRet;
}

MS_BOOL Demo_OTT_MM_FreeAudDec(void)
{
    MS_BOOL bRet = TRUE;
#if(DEMO_AUDIO_MULTI_TEST == 1)
    bRet = Demo_MM_FreeAudDec();
#endif
    return bRet;
}

//  demo_audio.h

MS_BOOL Demo_OTT_Audio_SetMute(MS_BOOL Enable)
{
    return Demo_Audio_SetMute(&Enable);
}

MS_BOOL Demo_OTT_Audio_Init(MS_U32 *Type)
{
    return Demo_Audio_Init(Type);
}

MS_BOOL Demo_OTT_Audio_SetAbsoluteVolume(MS_U8* vol)
{
    return Demo_Audio_SetAbsoluteVolume(vol);
}

#endif

