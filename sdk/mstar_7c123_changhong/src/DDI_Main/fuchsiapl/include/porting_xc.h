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

#ifndef __mw_porting_xc__
#define __mw_porting_xc__

#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus

#include "MsCommon.h"
#include "apiXC.h"
#include "apiPNL.h"
#include "apiPNL_EX.h"

//-------------------------------------------------------------------------------------------------
//  Enum Types
//-------------------------------------------------------------------------------------------------
/// Define PNL device number
typedef enum
{
    E_MPL_PNL_DEVICE0      = 0,
    E_MPL_PNL_DEVICE1,
    E_MPL_PNL_DEVICE_MAX
} E_MPL_PNL_DEVICE_NUM;

// Define Panel device ID
typedef struct
{
    MS_U32 u32Version;
    MS_U32 u32Id;
} MPL_PNL_DEVICE_ID;

///Define Get Panel Information
typedef enum
{
    /// Panel name
    E_MPL_PNL_NAME,
    /// DE H start
    E_MPL_PNL_HSTART,
    /// DE V start
    E_MPL_PNL_VSTART,
    /// DE H width
    E_MPL_PNL_WIDTH,
    /// DE V height
    E_MPL_PNL_HEIGHT,
    /// Htotal
    E_MPL_PNL_HTOTAL,
    /// Vtotal
    E_MPL_PNL_VTOTAL,
    /// H sync width
    E_MPL_PNL_HSYNC_WIDTH,
    /// H sync back porch
    E_MPL_PNL_HSYNC_BACKPORCH,
    /// V sync back porch
    E_MPL_PNL_VSYNC_BACKPORCH,
    /// Default V Freq
    E_MPL_PNL_VFREQ,
    /// 0: single, 1: dual mode
    E_MPL_PNL_LPLL_MODE,
    /// 0: LVDS, 1: RSDS
    E_MPL_PNL_LPLL_TYPE,
    /// E_PNL_ASPECT_RATIO
    E_MPL_PNL_ASPECT_RATIO,
    /// MinSET
    E_MPL_PNL_MIN_SET,
    /// MaxSET
    E_MPL_PNL_MAX_SET,
    /// Set Span-Spectrum-Control
    E_MPL_PNL_SET_SSC,
    /// Enable panel's output
    E_MPL_PNL_ENABLE,
    /// Printout panel dat
    E_MPL_PNL_DUMP,
    /// Dimming value control
    E_MPL_PNL_DIMMING_CONTROL,
    /// Query Power On Timing
    E_MPL_PNL_ON_TIMING,
    /// Query Power Off Timing
    E_MPL_PNL_OFF_TIMING,
    /// Turn on backlightr
    E_MPL_PNL_TURN_BACKLIGHT_ON,
    /// Output timing mode
    E_MPL_PNL_OUT_TIMING_MODE,
    /// Set Gamma value
    E_MPL_PNL_SET_GAMMA_VALUE,
    /// Get Gamma value
    E_MPL_PNL_GET_GAMMA_TABLE,
    /// number of buffer
    E_MPL_PNL_NUM,
}EN_MPL_PNL_INFO;

//-------------------------------------------------------------------------------------------------
/// Get current display window registers setting
/// @param  eWindow                          \b IN : Main or sub window
/// @param  pdspwin                          \b OUT: Pointer for ouput disp window register
//-------------------------------------------------------------------------------------------------
MS_BOOL MPL_XC_GetDispWinFromReg(MS_WINDOW_TYPE *pstDspwin, SCALER_WIN eWindow);

//-------------------------------------------------------------------------------------------------
/// Get signal sync status
/// @param  eWindow                          \b IN : Main or sub window
/// @return TRUE: sync loss
/// @return FALSE: sync active
//-------------------------------------------------------------------------------------------------
MS_BOOL MPL_XC_PCMonitor_SyncLoss(SCALER_WIN eWindow);

//-------------------------------------------------------------------------------------------------
/// Get signal sync status
/// @param  ePnlInfo                           \b IN : Panel info
/// @param  pData                               \b IN : Input data
/// @return Panel info
//-------------------------------------------------------------------------------------------------
MS_U32 MPL_PNL_GetPNLInfo(EN_MPL_PNL_INFO ePnlInfo, MS_U32 *pData);

#ifdef __cplusplus
}
#endif //__cplusplus
#endif //__mw_porting_xc__
