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

#ifndef _APPDEMO_GE_H_
#define _APPDEMO_GE_H_

MS_BOOL GE_Help_Part0(void);
MS_BOOL GE_Help_Part1(void);
MS_BOOL GE_Initialize(MS_U32 *index);
MS_BOOL GE_SetConfig(void);
MS_BOOL GE_SetColorFmt(MS_U32 *GWinIndex,MS_U32 *pColorFormat);
MS_BOOL GE_CreateWin(MS_U32 *GWinWidth0, MS_U32 *GWinHeight0, MS_U32 *GWinWidth1, MS_U32 *GWinHeight1);
MS_BOOL GE_FillBlock(MS_U32 *index, MS_U32 *RectX, MS_U32 *RectY, MS_U32 *RectWidth, MS_U32 *RectHeight);
MS_BOOL GE_DrawLine(MS_U32 *index, MS_U32 *X1, MS_U32 *Y1, MS_U32 *X2, MS_U32 *Y2);
MS_BOOL GE_DrawBmp(MS_U32 *Index, MS_U32 *X, MS_U32 *Y, MS_U32 *width, MS_U32 *height);
MS_BOOL GE_DrawBmpRotate(MS_U32 *X, MS_U32 *Y, MS_U32 *RotateIndex, MS_U32 *MirrorX, MS_U32 *MirrorY);
MS_BOOL GE_DrawBmpScale(MS_U32 *X, MS_U32 *Y, MS_U32 *ScaleX, MS_U32 *ScaleY);
MS_BOOL GE_ConvertColor(void);
MS_BOOL GE_ScreenCopyRop(MS_U32 *RopIndex, MS_U32 *SrcColorIndex, MS_U32 *DstColorIndex);
MS_BOOL GE_BitBltSample(MS_U32 *SrcX, MS_U32 *SrcY, MS_U32 *DstX, MS_U32 *DstY, MS_U32 *Width, MS_U32 *Height);
MS_BOOL GE_TextOut(MS_U32 *FontIndex, MS_U32 *ColorIndex, MS_U32 *FlagIndex, MS_U32 *X, MS_U32 *Y);
MS_BOOL appDemo_MM_DrawPhoto(void);
MS_BOOL appDemo_MM_Photo_SetGwinFmt(MS_U16 * pfmt);
MS_BOOL appDemo_MM_ClearPhoto(void);

#endif

