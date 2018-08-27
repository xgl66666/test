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


#ifndef _DEMO_DOLBYHDR_PREIDK_H_
#define _DEMO_DOLBYHDR_PREIDK_H_ 1

typedef enum
{
    E_DOLBYHDR_PREIDK_REG_DBG_HDR_SEL_FE,
    E_DOLBYHDR_PREIDK_REG_DBG_HDR_SEL_BE,
    E_DOLBYHDR_PREIDK_REG_DBG_HDR_SEL_OSD,
    E_DOLBYHDR_PREIDK_REG_DBG_HDR_SEL_NUM,
} EN_DOLBYHDR_PREIDK_REG_DBG_HDR_SEL;

typedef enum
{
    E_DOLBYHDR_PREIDK_REG_DBG_SEL_B0101_IN = 0,
    E_DOLBYHDR_PREIDK_REG_DBG_SEL_B0101_OUT,
    E_DOLBYHDR_PREIDK_REG_DBG_SEL_B0106_OUT,
    E_DOLBYHDR_PREIDK_REG_DBG_SEL_B02_OUT,
    E_DOLBYHDR_PREIDK_REG_DBG_SEL_B0401_IN = 0,
    E_DOLBYHDR_PREIDK_REG_DBG_SEL_B0406_IN = 1,
    E_DOLBYHDR_PREIDK_REG_DBG_SEL_B0406_OUT = 2,
    E_DOLBYHDR_PREIDK_REG_DBG_SEL_Scramble_OUT = 3,
    E_DOLBYHDR_PREIDK_REG_DBG_SEL_NUM = 4,
} EN_DOLBYHDR_PREIDK_REG_DBG_SEL;

typedef enum
{
    E_DOLBYHDR_PREIDK_REG_DBG_ODD_PIXEL,
    E_DOLBYHDR_PREIDK_REG_DBG_EVEN_PIXEL,
    E_DOLBYHDR_PREIDK_REG_DBG_EVEN_NUM,
} EN_DOLBYHDR_PREIDK_REG_DBG_EVEN;


MS_BOOL Demo_DolbyHDR_PreIDK_SaveDram2Bin(MS_U8* pPath, MS_U32* u32Addr, MS_U32* u32Size);
MS_BOOL Demo_DolbyHDR_PreIDK_LoadFrame(MS_U8* pPathBL, MS_U8* pPathEL, unsigned int* uIdx, MS_BOOL* b10Bit);
MS_BOOL Demo_DolbyHDR_PreIDK_LoadPicture(MS_U8* pPath);
MS_BOOL Demo_DolbyHDR_PreIDK_AutoVerify(MS_U8* pPath);
MS_BOOL Demo_DolbyHDR_PreIDK_VerifyFrame(MS_U8* pPath, unsigned int* uIdx);
MS_BOOL Demo_DolbyHDR_PreIDK_RunRiuScripts(MS_U8* pPath);
MS_BOOL Demo_DolbyHDR_PreIDK_ADC_Dump(MS_U8* pPath, EN_DOLBYHDR_PREIDK_REG_DBG_HDR_SEL *hdrSel, EN_DOLBYHDR_PREIDK_REG_DBG_SEL *dbgSel, EN_DOLBYHDR_PREIDK_REG_DBG_EVEN *evenSel);
MS_BOOL Demo_DolbyHDR_PreIDK_Capture2File(MS_U8* pPath, EN_DOLBYHDR_PREIDK_REG_DBG_HDR_SEL *hdrSel, EN_DOLBYHDR_PREIDK_REG_DBG_SEL *dbgSel, MS_U8* u8OutDepth);
MS_BOOL Demo_DolbyHDR_PreIDK_DumpMM(MS_U8* pPath, MS_U32* pResolution, MS_U8* pOutType, MS_U8* pVsvdb, MS_U8* pPriority, MS_BOOL* pbGop, MS_BOOL* pbLow, MS_BOOL* pbDVMapHDR10);
MS_BOOL Demo_DolbyHDR_PreIDK_OSDBlending(MS_U8* pPath);
MS_BOOL Demo_DolbyHDR_PreIDK_IDKMode(MS_U8 *u8Mode);
MS_BOOL Demo_DolbyHDR_PreIDK_Set_OutputType(MS_U8* pOutType);
MS_BOOL Demo_DolbyHDR_PreIDK_Set_VSVDB(MS_U8* pVsvdb);
MS_BOOL Demo_DolbyHDR_PreIDK_Set_GraphicPriority(MS_U8* pPriority);
MS_BOOL Demo_DolbyHDR_PreIDK_Set_LowLatencyMode(MS_BOOL* pbLow);
MS_BOOL Demo_DolbyHDR_PreIDK_Set_DolbyMappingHDR10(MS_BOOL* pbDVMapHDR10);

#endif
