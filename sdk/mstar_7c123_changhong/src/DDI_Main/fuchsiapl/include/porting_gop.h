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

#ifndef _mw_porting_gop__
#define _mw_porting_gop__

#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus

#include "apiGOP.h"
//-------------------------------------------------------------------------------------------------
/// Switch current active GOP
/// @param u8GOP            \b IN : Number of GOP
/// @return GOP_API_SUCCESS - Succes
/// @return GOP_API_FAIL - Failure
//-------------------------------------------------------------------------------------------------
E_GOP_API_Result MPL_GOP_GWIN_SwitchGOP(MS_U8 u8GOP);

//-------------------------------------------------------------------------------------------------
/// Query GOP Capability
/// @param eCapType         \b IN: Capability type
/// @param pRet                 \b OUT: return value
/// @param ret_size            \b IN: input structure size
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
E_GOP_API_Result MPL_GOP_GetChipCaps(EN_GOP_CAPS eCapType, MS_U32* pRet, MS_U32 ret_size);

//-------------------------------------------------------------------------------------------------
/// Set GOP H-Start
/// @param u8GOP                \b IN: GOP number
/// @param u16PanelHStr     \b IN: Horizontal start
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
//-------------------------------------------------------------------------------------------------
E_GOP_API_Result MPL_GOP_SetGOPHStart(MS_U8 u8GOP,MS_U16 u16PanelHStr);

//-------------------------------------------------------------------------------------------------
/// Enable GOP transparent color
/// @param fmt              \b IN: Color format
/// @param bEnable      \b IN: Enable/Disable
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
E_GOP_API_Result MPL_GOP_GWIN_EnableTransClr(EN_GOP_TRANSCLR_FMT fmt, MS_BOOL bEnable);

//-------------------------------------------------------------------------------------------------
/// Create GWIN
/// @param winId            \b IN: GWIN id
/// @param bEnable        \b IN: Show or Hide
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
//-------------------------------------------------------------------------------------------------
E_GOP_API_Result MPL_GOP_GWIN_Create(MS_U16 width, MS_U16 height, MS_U16 fbFmt);

//-------------------------------------------------------------------------------------------------
/// Enable GWIN
/// @param width            \b IN: Width
/// @param heigh            \b IN: Heigh
/// @param fbFmt            \b IN: Frame buffer format
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
//-------------------------------------------------------------------------------------------------
E_GOP_API_Result MPL_GOP_GWIN_Enable(MS_U8 winId, MS_BOOL bEnable);

//-------------------------------------------------------------------------------------------------
/// Delete GWIN
/// @param winId            \b IN: GWIN id
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
//-------------------------------------------------------------------------------------------------
E_GOP_API_Result MPL_GOP_GWIN_Delete(MS_U8 winId);

//-------------------------------------------------------------------------------------------------
/// Reset GWIN
/// @param gId              \b IN: GWIN id
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
//-------------------------------------------------------------------------------------------------
E_GOP_API_Result MPL_GOP_GWIN_Release(MS_U8 gId);

//-------------------------------------------------------------------------------------------------
/// Destory GWIN
/// @param gId                 \b IN: GWIN id
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
//-------------------------------------------------------------------------------------------------
E_GOP_API_Result MPL_GOP_GWIN_Destroy(MS_U8 gId);

//-------------------------------------------------------------------------------------------------
/// Enable GOP progressive mode
/// @param bEnable          \b IN: Enable/Disable
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
E_GOP_API_Result MPL_GOP_GWIN_EnableProgressive(MS_BOOL bEnable);

//-------------------------------------------------------------------------------------------------
/// Set GOP force write mode
/// @param bEnable          \b IN: TRUE/FALSE
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
E_GOP_API_Result MPL_GOP_GWIN_SetForceWrite(MS_BOOL bEnable);

//-------------------------------------------------------------------------------------------------
/// Set GOP update register method by only once.
/// @param bWriteRegOnce        \b IN: TRUE/FALSE
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
E_GOP_API_Result MPL_GOP_GWIN_UpdateRegOnce(MS_BOOL bWriteRegOnce);

//-------------------------------------------------------------------------------------------------
/// Set GOP field inverse
/// @param enable               \b IN: TRUE/FALSE
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
E_GOP_API_Result MPL_GOP_GWIN_SetFieldInver(MS_BOOL enable);

//-------------------------------------------------------------------------------------------------
/// Configure the GOP destination
/// @param u8GOP            \b IN : Number of GOP
/// @param dsttype           \b IN : GOP destination
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_ENUM_NOT_SUPPORTED - GOP destination not support
//-------------------------------------------------------------------------------------------------
E_GOP_API_Result MPL_GOP_GWIN_SetGOPDst(MS_U8 u8GOP, EN_GOP_DST_TYPE dsttype);

//-------------------------------------------------------------------------------------------------
/// Set GWIN H-Stretch ratio
/// @param bEnable      \b IN: Enable/Disable
/// @param src              \b IN: Original size
/// @param dst              \b IN: Target size
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
E_GOP_API_Result MPL_GOP_GWIN_Set_Hscale(MS_BOOL bEnable, MS_U16 src, MS_U16 dst);

//-------------------------------------------------------------------------------------------------
/// Set GWIN V-Stretch ratio
/// @param bEnable      \b IN: Enable/Disable
/// @param src              \b IN: original size
/// @param dst              \b IN: target size
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
E_GOP_API_Result MPL_GOP_GWIN_Set_Vscale(MS_BOOL bEnable, MS_U16 src, MS_U16 dst);

//-------------------------------------------------------------------------------------------------
/// Set stretch window
/// @param u8GOP_num        \b IN: GOP number
/// @param eDstType             \b IN: GOP destination displayplane type
/// @param x                          \b IN: Horizontal start position
/// @param y                          \b IN: Vertical start position
/// @param width                   \b IN: Stretch window width
/// @param height                  \b IN: Stretch window height
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
//-------------------------------------------------------------------------------------------------
E_GOP_API_Result MPL_GOP_GWIN_Set_StretchWin(MS_U8 u8GOP_num, EN_GOP_DST_TYPE eDstType, MS_U16 x, MS_U16 y, MS_U16 width, MS_U16 height);

//-------------------------------------------------------------------------------------------------
/// Set GOP output color
/// @param type         \b IN: Color type
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
E_GOP_API_Result MPL_GOP_GWIN_OutputColor(EN_GOP_OUTPUT_COLOR type);

//-------------------------------------------------------------------------------------------------
/// Switch GE to render on a GWIN's frame buffer
/// @param winId            \b IN: GWIN id
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
//-------------------------------------------------------------------------------------------------
E_GOP_API_Result MPL_GOP_GWIN_Switch2Gwin(MS_U8 winId);

//-------------------------------------------------------------------------------------------------
/// Set GWIN info
/// @param u8win        \b IN: GWIN id
/// @param pinfo          \b IN: GWIN info
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
//-------------------------------------------------------------------------------------------------
E_GOP_API_Result MPL_GOP_GWIN_SetWinInfo(MS_U8 u8win, GOP_GwinInfo *pinfo);

//-------------------------------------------------------------------------------------------------
/// Get GWIN info
/// @param u8win        \b IN:  GWIN id
/// @param pinfo          \b OUT: Buffer to store GWIN info
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
//-------------------------------------------------------------------------------------------------
E_GOP_API_Result MPL_GOP_GWIN_GetWinInfo(MS_U8 u8win, GOP_GwinInfo *pinfo);

//-------------------------------------------------------------------------------------------------
/// Set GWIN alpha blending
/// @param u8win            \b IN: GWIN id
/// @param bEnable         \b IN: Enable/Disable
/// @param u8coef           \b IN: Alpha blending coefficient (0-7)
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
E_GOP_API_Result MPL_GOP_GWIN_SetBlending(MS_U8 u8win, MS_BOOL bEnable, MS_U8 u8coef);

//-------------------------------------------------------------------------------------------------
/// Set GWIN Blink
/// @param bEnable      \b IN: Enable/Disable
/// @param u8rate        \b IN: Blink frame rate
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
E_GOP_API_Result MPL_GOP_GWIN_SetBlink(MS_BOOL bEnable, MS_U8 u8rate);

//-------------------------------------------------------------------------------------------------
/// Set GWIN property
/// @param WinProperty          \b IN: GWIN property info
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
//-------------------------------------------------------------------------------------------------
E_GOP_API_Result MPL_GOP_GWIN_SetWinProperty(GOP_GwinDispProperty WinProperty);

//-------------------------------------------------------------------------------------------------
/// Check specify GWIN is enabled or not
/// @param  winId       \b IN: GWIN id
/// @return TRUE: Enable
/// @return FALSE: Disable
//-------------------------------------------------------------------------------------------------
MS_BOOL MPL_GOP_GWIN_IsGWINEnabled(MS_U8 winId);

//-------------------------------------------------------------------------------------------------
/// Check if GWIN is enabled
/// @return TRUE: Enable
/// @return FALSE: Disable
//-------------------------------------------------------------------------------------------------
MS_BOOL MPL_GOP_GWIN_IsEnabled(void);

//-------------------------------------------------------------------------------------------------
/// Check if specify GWIN is created
/// @param gId \b       IN: GWIN id
/// @return TRUE: Enable
/// @return FALSE: Disable
//-------------------------------------------------------------------------------------------------
MS_U8 MPL_GOP_GWIN_IsGwinExist(MS_U8 gId);

//-------------------------------------------------------------------------------------------------
/// Create frame buffer
/// @param fbId         \b IN: Frame buffer id
/// @param dispX       \b IN: Display x
/// @param dispY       \b IN: Display y
/// @param width       \b IN: Width
/// @param height      \b IN: Height
/// @param fbFmt       \b IN: Frame buffer format
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_CRT_GWIN_NOAVAIL - Failure
//-------------------------------------------------------------------------------------------------
MS_U8 MPL_GOP_GWIN_CreateFB(MS_U8 fbId, MS_U16 dispX, MS_U16 dispY, MS_U16 width, MS_U16 height, MS_U16 fbFmt);

//-------------------------------------------------------------------------------------------------
/// Create frame buffer from given frame buffer id and dram address.
/// @param fbId                 \b IN: Frame buffer id
/// @param dispX               \b IN: Frame buffer starting x
/// @param dispY               \b IN: Frame buffer starting y
/// @param width              \b IN: Frame buffer width
/// @param height             \b IN: Frame buffer height
/// @param fbFmt              \b IN: Frame buffer color format
/// @param u32FbAddr      \b IN: Frame buffer starting address
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
//-------------------------------------------------------------------------------------------------
MS_U8 MPL_GOP_GWIN_CreateFBbyStaticAddr(MS_U8 fbId, MS_U16 dispX, MS_U16 dispY, MS_U16 width, MS_U16 height,  MS_U16 fbFmt, MS_U32 u32FbAddr);

//-------------------------------------------------------------------------------------------------
/// Create GWIN and assign frame buffer to this GWIN
/// @param u8GOP        \b IN: GOP number
/// @param FBId            \b IN: Frame buffer ID
/// @param dispX          \b IN: Frame buffer starting x position
/// @param dispY          \b IN: Frame buffer starting y position
/// @return GWIN id, 0xff for fail
//-------------------------------------------------------------------------------------------------
MS_U8 MPL_GOP_GWIN_CreateWin_Assign_FB(MS_U8 u8GOP, MS_U8 FBId, MS_U16 dispX, MS_U16 dispY);

//-------------------------------------------------------------------------------------------------
/// Set frame buffer info
/// @param fbId          \b IN: Frame buffer id
/// @param fbAttr       \b IN: Frame buffer attribute
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
//-------------------------------------------------------------------------------------------------
E_GOP_API_Result MPL_GOP_GWIN_SetFBInfo(MS_U8 fbId, GOP_GwinFBAttr *fbAttr);

//-------------------------------------------------------------------------------------------------
/// Get frame buffer information
/// @param fbId                 \b IN: Frame buffer id
/// @param fbAttr               \b OUT: Frame buffer attribute
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
//-------------------------------------------------------------------------------------------------
E_GOP_API_Result MPL_GOP_GWIN_GetFBInfo(MS_U8 fbId, GOP_GwinFBAttr *fbAttr);

//-------------------------------------------------------------------------------------------------
/// Get free frame buffer id
/// @return frame buffer id. If return oxFF, it represents no free frame buffer id for use.
//-------------------------------------------------------------------------------------------------
MS_U8 MPL_GOP_GWIN_GetFreeFBID(void);

//-------------------------------------------------------------------------------------------------
/// Delete a frame buffer
/// @param fbId             \b IN: Frame buffer id
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
//-------------------------------------------------------------------------------------------------
E_GOP_API_Result MPL_GOP_GWIN_DeleteFB(MS_U8 fbId);

//-------------------------------------------------------------------------------------------------
/// Destroy fram buffer
/// @param fbId             \b IN: Frame buffer id
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_CRT_GWIN_NOAVAIL - Failure
//-------------------------------------------------------------------------------------------------
MS_U8 MPL_GOP_GWIN_DestroyFB(MS_U8 fbId);

//-------------------------------------------------------------------------------------------------
/// Check if a frame buffer is allocated
/// @param fbId             \b IN: Frame buffer id
/// @return TRUE: Allocated
/// @return FALSE: Invalid
//-------------------------------------------------------------------------------------------------
MS_U8 MPL_GOP_GWIN_IsFBExist(MS_U8 fbId);

//-------------------------------------------------------------------------------------------------
/// Set GE to render on the frame buffer
/// @param fbId             \b IN: Frame buffer id
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
//-------------------------------------------------------------------------------------------------
E_GOP_API_Result MPL_GOP_GWIN_Switch2FB(MS_U8 fbId);

//-------------------------------------------------------------------------------------------------
/// Change a GWIN's frame buffer
/// @param fbId             \b IN: Frame buffer id
/// @param gwinId         \b IN: GWIN id
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
//-------------------------------------------------------------------------------------------------
E_GOP_API_Result MPL_GOP_GWIN_MapFB2Win(MS_U8 fbId, MS_U8 gwinId);

//-------------------------------------------------------------------------------------------------
/// Get frame bufferid from the gwin id
/// @param gwinId       \b IN gwin id
/// @return return frame buffer id. If return 0xFF, it represents this function fail
//-------------------------------------------------------------------------------------------------
MS_U8 MPL_GOP_GWIN_GetFBfromGWIN(MS_U8 gwinId);

//-------------------------------------------------------------------------------------------------
/// Switch frame buffer pool
/// @param u8FB         \b IN 0: Frame buffer idx
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
E_GOP_API_Result MPL_GOP_FB_SEL(MS_U8 u8FB);

//-------------------------------------------------------------------------------------------------
/// Read GOP palette values
/// @param idx      \b IN: Pallete idx
/// @param clr       \b OUT: Color
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
E_GOP_API_Result MPL_GOP_GWIN_ReadPalette(MS_U8 idx, MS_U32 *color);

//-------------------------------------------------------------------------------------------------
/// Set palette optional values
/// @param *pPalArray           \b IN: Pointer to pallete list gonna set into pallete table
/// @param u32PalStart         \b IN: Start of pallete table to be overwrite
/// @param u32PalEnd           \b IN: End of pallete table to be overwrite
/// @param ePalType             \b IN: Pallete format
//-------------------------------------------------------------------------------------------------
E_GOP_API_Result MPL_GOP_GWIN_SetPaletteOpt(GOP_PaletteEntry *pPalArray, MS_U16 u32PalStart, MS_U16 u32PalEnd, EN_GOP_PALETTE_TYPE ePalType);

//-------------------------------------------------------------------------------------------------
/// Get video status from scaler.
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
//-------------------------------------------------------------------------------------------------
E_GOP_API_Result MPL_GOP_RegisterXCIsInterlaceCB(MS_BOOL (*fpGOP_CB)(void));

//-------------------------------------------------------------------------------------------------
/// Get frame buffer address by ID.
/// @param *pAddr  \b IN: Pointer to frame buffer
//-------------------------------------------------------------------------------------------------
E_GOP_API_Result MPL_GOP_GWIN_GetFBAddr(MS_U8 fbID, MS_U32 *pAddr);

#ifdef __cplusplus
}
#endif //__cplusplus
#endif //_mw_porting_gop__
