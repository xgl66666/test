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

#ifndef _DEMO_OSD_H_
#define _DEMO_OSD_H_

#if (DEMO_XC_DUALXC_TEST == 1)
#define IPANEL(dev, func) g_IPanelEx.func(dev)
#else
#define IPANEL(dev, func) g_IPanel.func()
#endif

typedef enum
{
    E_DDI_OSD_HD_DST_OP0          = 0,
    E_DDI_OSD_HD_DST_IP0          = 1,
    E_DDI_OSD_HD_DST_VOP          = 2,
    E_DDI_OSD_HD_DST_VOP_SUB      = 3,
    E_DDI_OSD_HD_DST_IP_SUB       = 4,
    E_DDI_OSD_HD_DST_GOPScaling   = 5,
}EN_DDI_OSD_HD_DST_TYPE;

typedef enum
{
    E_DDI_OSD_SD_DST_OP1          = 0,
    E_DDI_OSD_SD_DST_IP1          = 1,
    E_DDI_OSD_SD_DST_VE           = 2,
    E_DDI_OSD_SD_DST_MIXER2VE     = 3,
    E_DDI_OSD_SD_DST_OP2VE        = 4,
}EN_DDI_OSD_SD_DST_TYPE;

#if (DEMO_GOP_ZORDER_TEST == 1)
typedef enum
{
    E_DISPLAY_LAYER_0 = 0,
    E_DISPLAY_LAYER_1,
    E_DISPLAY_LAYER_2,
    E_DISPLAY_LAYER_3,
    E_DISPLAY_LAYER_4,
    E_DISPLAY_LAYER_5
}EN_DISPLAY_LAYER;

MS_BOOL Demo_ZOrder_Init(void);
MS_BOOL Demo_ZOrder_SetZOrder(
                                MS_U32* pu32VideoLayerOrder,
                                MS_U8* pu8Layer0Gop,
                                MS_U8* pu8Layer1Gop,
                                MS_U8* pu8Layer2Gop,
                                MS_U8* pu8Layer3Gop);
#endif
MS_BOOL Demo_OSD_Help(void);
MS_BOOL Demo_OSD_Init(MS_U32 *pu32HD_GOP, MS_U32 *pu32SD_GOP, MS_U32 *pu32HD_DST, MS_U32 *pu32SD_DST);
MS_BOOL Demo_OSD_PTGOP_Init(void);
MS_BOOL Demo_OSD_SetConfig(void);
MS_BOOL Demo_OSD_CreateWin(MS_U32 *pu32ColorType, MS_U32 *pu32GWinWidth0, MS_U32 *pu32GWinHeight0, MS_U32 *pu32GWinWidth1, MS_U32 *pu32GWinHeight1);
MS_BOOL Demo_OSD_DrawBmp(MS_U32 *pu32Index);
#if (DEMO_GOP_3DOSD_TEST == 1)
MS_BOOL Demo_OSD_Draw3DOSD(MS_U8 *pu8mode);
#endif
MS_BOOL Demo_OSD_HVScale(MS_BOOL *pbEnableScale, MS_U32 *pu32Hscale_rate, MS_U32 *pu32Vscale_rate);
MS_BOOL Demo_OSD_DWinCapture(MS_U32 *pu32CaptureOneFrame, MS_U32 *pu32GWinDsipCount, MS_U32 *pu32Source);
MS_BOOL Demo_OSD_DWinPingPong(MS_U32 *pu32BufferCount);
MS_BOOL Demo_OSD_DWinSkipFrame(MS_U32 *pu32FrameCount);
MS_BOOL Demo_OSD_EnableWin(MS_U32 *pu32Enable);
MS_BOOL Demo_OSD_FreeWin(void);
MS_BOOL Demo_OSD_SetGlobalAlpha(MS_U32 *pu32Auto, MS_U32 *pu32AlphaValue);
MS_BOOL Demo_OSD_EnablePerPixelAlpha(MS_U32 *pu32Auto, MS_U32 *pu32AlphaValue);
MS_BOOL Demo_OSD_SetFade(MS_U32 *pu32FadeType, MS_U32 *pu32Enable, MS_U32 *pu32Rate);

MS_BOOL Demo_OSD_FillBlock(MS_U32 *pu32Target, MS_U32 *pu32index, MS_U32 *pu32RectX, MS_U32 *pu32RectY, MS_U32 *pu32RectWidth, MS_U32 *pu32RectHeight);
MS_BOOL Demo_OSD_DrawLine(MS_U32 *pu32Target, MS_U32 *pu32index, MS_U32 *pu32X1, MS_U32 *pu32Y1, MS_U32 *pu32X2, MS_U32 *pu32Y2);
MS_BOOL Demo_OSD_DrawBmpXY(MS_U32 *pu32Target, MS_U32 *pu32Index, MS_U32 *pu32X, MS_U32 *pu32Y, MS_U32 *pu32width, MS_U32 *pu32height);
MS_BOOL Demo_OSD_DrawBmpRotate(MS_U32 *pu32Target, MS_U32 *pu32X, MS_U32 *pu32Y, MS_U32 *pu32RotateIndex, MS_U32 *pu32MirrorX, MS_U32 *pu32MirrorY);
MS_BOOL Demo_OSD_DrawBmpScale(MS_U32 *pu32Target, MS_U32 *pu32X, MS_U32 *pu32Y, MS_U32 *pu32ScaleX, MS_U32 *pu32ScaleY);
MS_BOOL Demo_OSD_ConvertColor(void);
MS_BOOL Demo_OSD_ScreenCopyRop(MS_U32 *pu32Target, MS_U32 *pu32RopIndex, MS_U32 *pu32SrcColorIndex, MS_U32 *pu32DstColorIndex);
MS_BOOL Demo_OSD_BitBltSample(MS_U32 *pu32Target, MS_U32 *pu32SrcX, MS_U32 *pu32SrcY, MS_U32 *pu32DstX, MS_U32 *pu32DstY, MS_U32 *pu32Width, MS_U32 *pu32Height);
MS_BOOL Demo_OSD_TextOut(MS_U32 *pu32Target, MS_U32 *pu32FontIndex, MS_U32 *pu32ColorIndex, MS_U32 *pu32FlagIndex, MS_U32 *pu32X, MS_U32 *pu32Y);
MS_BOOL Demo_OSD_SetGWinFmt(MS_U16 *pu16fmt);
#if (DEMO_GE_TWOSOURCEBITBLT_TEST == 1)
MS_BOOL Demo_OSD_TwoSourceBitblt(void);
#endif
MS_BOOL Demo_OSD_DrawJpeg(void);
MS_BOOL Demo_OSD_DrawGPD(MS_BOOL bGif);

#if (DEMO_ZUI_TEST == 1)
MS_BOOL Demo_UI_GEGOPInit(MS_U32 *pu32HD_GOP, MS_U32 *pu32SD_GOP, MS_U32 *pu32HD_DST, MS_U32 *pu32SD_DST);
MS_U8 Demo_OSD_GetHDGOPNum(void);
MS_U8 Demo_OSD_GetSDGOPNum(void);
MS_U8 Demo_OSD_GetHDGOPDst(void);
MS_U8 Demo_OSD_GetSDGOPDst(void);
#endif

#if (DEMO_MM_TEST == 1)
MS_BOOL Demo_MM_DrawPhoto(void);
MS_BOOL Demo_MM_Photo_SetGwinFmt(MS_U16 *pu16fmt);
MS_BOOL Demo_MM_ClearPhoto(void);
MS_BOOL Demo_MM_StartDrawPhoto(void);
#endif

#if (DEMO_DTE_TEST == 1)
MS_BOOL Demo_OSD_Draw3D(const MS_U32* PhyAddr, const MS_U32 *width, const MS_U32 *height);
#endif

#if (DEMO_GOP_GOPSC_TEST == 1)
//GOP Scaling
MS_BOOL Demo_OSD_GOPSC_Init(MS_U32 *pu32Source, MS_U32 *pu32cmdFlag);
MS_BOOL Demo_OSD_GOPSC_HVScalingProcess(MS_U32 *pu32Enable, MS_U32 *pu32SrcW, MS_U32 *pu32SrcH, MS_U32 *pu32DestW, MS_U32 *pu32DestH, MS_U32 *pu32FrameConut);
#endif

#endif
