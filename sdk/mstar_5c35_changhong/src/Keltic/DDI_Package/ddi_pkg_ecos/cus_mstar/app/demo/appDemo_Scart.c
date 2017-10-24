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
// Copyright (c) 2006-2010 MStar Semiconductor, Inc.
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

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @file   appDemo_Scart.c
/// @author MStar Semiconductor Inc.
/// @brief  Scart control Function Sample Demo
///////////////////////////////////////////////////////////////////////////////////////////////////



//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include "MsCommon.h"
#include "drvScart.h"
#include "appDemo_Scart.h"

//-------------------------------------------------------------------------------------------------
//                                MACROS
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Compiler Options
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/// @brief SCART control initialize function
/// @return TRUE: Success
/// @return FALSE: Failure
/// @sa
/// @note
/// Command: \b Scart_Init \n
//------------------------------------------------------------------------------
MS_BOOL appDemo_Scart_Init(void)
{
    return MDrv_SCART_Init();
}
//------------------------------------------------------------------------------
/// @brief Toggle SCART bypass control from VCR SCART to TV SCART
/// @param[in] bEnable
///            0 -> disable
///            1 -> enable
/// @return TRUE: Success
/// @return FALSE: Failure
/// @sa
/// @note
/// If current is bypass enable, after this function executed, it would be disabled.
/// Command: \b Scart_Bypass \n
//------------------------------------------------------------------------------
MS_BOOL appDemo_Scart_Bypass(MS_U32 *bEnable)
{
    return MDrv_SCART_SetBypassEnable((MS_BOOL)*bEnable);
}
//------------------------------------------------------------------------------
/// @brief set SCART Output Mode.
/// @param[in] eTargetState
///            0 -> DISPLAY_SCART_ALL_ENABLE 
///            1 -> DISPLAY_SCART_ALL_DISABLE 
///            2 -> DISPLAY_SCART_COMPOSITE_ONLY
/// @return TRUE: Success
/// @return FALSE: Failure
/// @sa
/// @note
/// Command: \b Scart_SetOutputMode 1 \n
//------------------------------------------------------------------------------
MS_BOOL appDemo_Scart_SetOutputMode(MS_U32 *eTargetState)
{
    return MDrv_SCART_SetOutputMode((EN_DISPLAY_SCART_OUT_STATE)*eTargetState);
}
//------------------------------------------------------------------------------
/// @brief set SCART out Aspect Ratio.
/// @param[in] eAs: Aspect Ratio.  
///            0 -> 1:1
///            1 -> 16:9
///            2 -> 4:3
/// @return TRUE: Success
/// @return FALSE: Failure
/// @sa
/// @note
/// Command: \b Scart_SetAspectRatio 1 \n
//------------------------------------------------------------------------------
MS_BOOL appDemo_Scart_SetAspectRatio(MS_U32 *eAs)
{
    return MDrv_SCART_SetAspectRatio((EN_ASPECT_RATIO_CODE)*eAs);
}
//------------------------------------------------------------------------------
/// @brief Get SCART interrupt flag and clear it
/// @return TRUE  - scart interrupt detected
/// @return FALSE - scart interrupt not detected
/// @sa
/// @note
/// Command: \b Scart_GetInterruptOut \n
//------------------------------------------------------------------------------
MS_BOOL appDemo_Scart_GetInterruptOut(void)
{
    MS_BOOL bLevel = FALSE;

    bLevel = MDrv_SCART_GetInterruptOutOnOff();

    printf("%s SCART IT_OUT = %d", __FUNCTION__, bLevel);

    return bLevel;
}

//------------------------------------------------------------------------------
/// @brief Set SCART interrupt enable or not
/// @param[in] bEnable: enable/disable.  
///            0 -> disable
///            1 -> enable
/// @return TRUE  - scart interrupt enable
/// @return FALSE - scart interrupt not enable
/// @sa
/// @note
/// Command: \b Scart_SetInterruptEnable 1 \n
//------------------------------------------------------------------------------
MS_BOOL appDemo_SCart_SetInterruptEnable(MS_U32 *bEnable)
{
    return MDrv_SCART_SetInterruptEnable((MS_BOOL)*bEnable);
}

//------------------------------------------------------------------------------
/// @brief Set SCART output audio level adjustment
/// @param[in] u32Adj - 0 - 31 (0 to -62db, -2db step)
/// @return TRUE  - success
/// @return FALSE - failure
/// @sa
/// @note
/// Command: \b Scart_SetAudioLevelAdjustment 1 \n
//------------------------------------------------------------------------------
MS_BOOL appDemo_SCart_SetAudioLevelAdjustment(MS_U32 *u32Adj)
{
    return MDrv_SCART_SetAudioLevelAdjustment((MS_U8)*u32Adj);
}
