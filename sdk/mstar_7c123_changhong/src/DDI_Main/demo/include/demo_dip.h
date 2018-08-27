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

#ifndef _DEMO_DIP_H_
#define _DEMO_DIP_H_

typedef enum
{
    E_DDI_DIP_SOURCE_SC0_IP_MAIN         = 0,
    E_DDI_DIP_SOURCE_SC0_IP_SUB          = 1,
    E_DDI_DIP_SOURCE_SC0_OP_CAPTURE      = 2,
    E_DDI_DIP_SOURCE_DRAM                = 3,
    E_DDI_DIP_SOURCE_SC1_OP_CAPTURE      = 4,
}EN_DDI_DIP_SOURCE_TYPE;

typedef enum
{
    E_DDI_DIP_DISPLAY_NONE             = 0,
    E_DDI_DIP_DISPLAY_FROM_GOP         = 1,
    E_DDI_DIP_DISPLAY_FROM_GEGOP       = 2,
}EN_DDI_DIP_DISPLAY_TYPE;

typedef enum
{
    /// CrYCb domain.
    E_DDI_DIP_DATA_FMT_YUV422 = 0,
    /// CrYCb domain.
    E_DDI_DIP_DATA_FMT_RGB565,
    /// RGB domain (CSC bypass).
    E_DDI_DIP_DATA_FMT_ARGB8888,
    /// YUV420
    E_DDI_DIP_DATA_FMT_YUV420,
    E_DDI_DIP_DATA_FMT_MAX
} EN_DDI_DIP_DATA_FMT;

typedef struct
{
    MS_U16 u16Width;
    MS_U16 u16Height;
    MS_U16 u16Pitch;
    MS_U32 u32YBufAddr;
    MS_U32 u32CBufAddr;
    EN_DDI_DIP_DATA_FMT enDataFmt;
}ST_DEMO_DIPR_SOURCE_INFO;

MS_BOOL Demo_DIP_Init(void);
MS_BOOL Demo_DIP_Exit(void);
MS_BOOL Demo_DIP_InputSourceSelect(MS_U32 *pu32DIPSrc);
MS_BOOL Demo_DIP_OutputFormatSelect(MS_U32 *pu32Format);
MS_BOOL Demo_DIP_ARGBAlphaSet(MS_U32 *pu32Alpha);
MS_BOOL Demo_DIP_ShowResultType(MS_U32 *pu32DIPDispType, MS_U32 *pu32GOP);
MS_BOOL Demo_DIP_CaptureOneFrame(MS_U32 *u32Width, MS_U32 *u32Height);
MS_BOOL Demo_DIP_CaptureContinually(MS_U32 *u32Width, MS_U32 *u32Height);
MS_BOOL Demo_DIP_GOPPingpong(MS_U32 *u32Width, MS_U32 *u32Height);
MS_BOOL Demo_DIP_MFE(MS_U32 *u32Width, MS_U32 *u32Height);
MS_BOOL Demo_DIP_DIPRFromVDEC(MS_U32 *pu32Device,MS_U32 *u32Width, MS_U32 *u32Height);
MS_BOOL Demo_DIP_DIPR(MS_U32 *pu32Device,MS_BOOL benable,MS_BOOL bDIPSetInit,MS_U32 u32Width, MS_U32 u32Height, ST_DEMO_DIPR_SOURCE_INFO *p_Demo_DIPRSrcInfo);

#endif
