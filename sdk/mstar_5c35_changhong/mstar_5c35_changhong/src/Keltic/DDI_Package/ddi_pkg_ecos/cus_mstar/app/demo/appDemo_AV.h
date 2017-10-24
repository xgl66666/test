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


#ifndef _APPDEMO_AV_H_
#define _APPDEMO_AV_H_

#include "MsCommon.h"

typedef enum
{
    //MPEG 1/2
    E_DDI_VDEC_CODEC_TYPE_MPEG2 = 0,
    //H264
    E_DDI_VDEC_CODEC_TYPE_H264,
    //AVS
    E_DDI_VDEC_CODEC_TYPE_AVS,
    //MPEG 1/2 for I-Frame
    E_DDI_VDEC_CODEC_TYPE_MPEG2_IFRAME,
    //H264 for I-Frame
    E_DDI_VDEC_CODEC_TYPE_H264_IFRAME,
}DDI_VDEC_CodecType;

typedef struct
{
    MS_U32 u32VideoPid;
    MS_U32 u32AudioPid;
    MS_U32 u32PCRPid;
    MS_U32 u32VCodec;
    MS_U32 u32ACodec;
    MS_BOOL bPlayLiveTV;

}codecPIDs,*pCodecPIDs;

codecPIDs liveProgramPIDs;
pCodecPIDs pliveProgramPIDs;


//Global Variables
extern MS_BOOL g_Demo_IsIframedecoding;

extern void DemoAV_Help_TV(void);


extern MS_BOOL DemoAV_Tuner_SetFreq(MS_U32*, MS_U32*);

extern MS_BOOL DemoAV_TSP_SetPid(MS_U32* VideoPid, MS_U32* AudioPid, MS_U32* PCRPid, MS_U32* pu32VCodec, MS_U32* pu32ACodec);
extern MS_BOOL DemoAV_TSP_SetVideoDmxFltRushPass(MS_U8 u8Enable);
extern MS_BOOL DemoAV_TSP_GetLiveTVPIDFlts(MS_U8* u8LiveVideoFltId, MS_U8* u8LiveAudioFltId, MS_U8* u8LivePCRFltId);
extern MS_BOOL DemoAV_MAD_SetVol(MS_U8*);
extern MS_BOOL DemoAV_MVD_GetVideoInfo(void);
extern MS_BOOL DemoAV_MVD_SetFRCMode(MS_U8*);
extern MS_BOOL DemoAV_MVD_DecIFrame(MS_U32* u32FrameBufAddr, MS_U32* u32StreamBufAddr, MS_U32* u32StreamBufEnd, MS_U32* pu32VCodec);

extern MS_BOOL DemoAV_PlayAV(void);
extern MS_BOOL DemoAV_PauseAV(void);
extern MS_BOOL DemoAV_StopAV(void);
extern MS_BOOL DemoAV_Set_AVSYNC_Mode(MS_U8* u8mode);
extern MS_BOOL DemoAV_RunAll(MS_U32*, MS_U32*, MS_U32*, MS_U32*);

extern void DemoAV_Disp_SetAS(MS_U8*);
extern void DemoAV_Disp_SetOutputTiming(MS_U8*, MS_U8*);
extern void DemoAV_Disp_SetSrcWin(MS_U16*, MS_U16*, MS_U16*, MS_U16*);
extern void DemoAV_Disp_SetDstWin(MS_U16*, MS_U16*, MS_U16*, MS_U16*);

extern void DemoAV_SetOutputType(MS_U8*);
extern void DemoAV_VE_Pattern(MS_BOOL*);

extern void DemoAV_Disp_FrameColor(MS_BOOL*);
extern void DemoAV_Disp_DIMode(MS_U8*);

extern void DemoAV_Show_Preview(MS_U16*, MS_U16*);
extern void DemoAV_Hide_Preview(void);
extern void DemoAV_Move_Preview(MS_U16*, MS_U16*);

extern void DemoAV_DEB_SetPicSize(MS_U16*, MS_U16*, MS_U16*, MS_U16*);
extern void DemoAV_ClipImage(MS_U16*, MS_U16*, MS_U16*, MS_U16*);

extern void DemoAV_ZoomToFullScreen(void);
extern MS_BOOL DemoAV_Tuner_Config(int* flow, int* pInSrc, int* pClkInv, int* pExtSync, int* pParal);

extern MS_BOOL DemoAV_TSP_SetPVRPid(MS_U32* VideoPid, MS_U32* AudioPid, MS_U32* PCRPid, MS_U32* pu32VCodec, MS_U32* pu32ACodec);
extern MS_BOOL DemoAV_TSP_ResetPVRPid(void);
extern MS_BOOL DemoAV_TSP_ResetDTVPid(void);
extern pCodecPIDs DemoAV_TSP_GetLiveTVPIDs(void);

#ifdef ENABLE_PVR_PACKAGE
extern MS_BOOL DemoAV_TSP_SetPVRAudioPid(MS_U32* AudioPid, MS_BOOL bEsPlayer);
extern MS_BOOL DemoAV_TSP_ClosePVRAudioPid(const MS_U32* AudioPid, MS_BOOL bEsPlayer);
extern MS_BOOL DemoAV_TSP_StopPVRFlt(MS_BOOL bStop);
extern MS_BOOL DualAV_StopPCRFLT(void);
extern MS_BOOL DualAV_StartPCRFLT(MS_U16 *PCRPid);
#endif /* ENABLE_PVR_PACKAGE */

#ifdef ENABLE_MM_PACKAGE
extern MS_BOOL DemoAV_TSP_StopDmxFlt(void);
#endif

#endif
