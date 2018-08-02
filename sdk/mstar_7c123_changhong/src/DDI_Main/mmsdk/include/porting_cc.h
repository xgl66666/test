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

#ifndef __mmsdk_porting_cc__
#define __mmsdk_porting_cc__
#include "mmsdk_interface_def.h"

#ifdef __cplusplus
extern "C"
{
#endif

/// @file
/// @defgroup porting_cc porting_cc functions
/// @{

/// define CC multiple-instance item
/// @since version 1.01
typedef void* PT_CCITEM;

/// Define CC callback type.
/// @since version 1.01
typedef enum
{
    /// none
    E_MMSDK_CC_CB_NONE,
    /// Callback message: Get current PTS for closed caption rendering.
    E_MMSDK_CC_CB_GET_CURRENT_PTS,
    /// Callback message: Get video size.
    E_MMSDK_CC_CB_GET_VIDEO_SIZE,
} EN_MMSDK_CC_CB;

/// Define the attached information for callback process. The callback message is E_CPCODEC_MSG_CC_INIT.
/// @since version 1.01
typedef struct
{
    /// Media object relative information.
    void* Media;
    /// Init closed caption decoder.
    MMSDK_BOOL bInit;
    /// check close caption show or hide
    EN_MMSDK_CC_CMD_TYPE eCmdType;
    /// The address of bit stream buf for CEA-608(EIA-608); original CC standard
    MMSDK_U32 u32CC608BitStreamAddr;
    /// The size of bit stream buf for CEA-608(EIA-608)
    MMSDK_U32 u32CC608BitStreamSize;
    /// The address of bit stream buf for CEA-708(EIA-708); support both SD and HD in DTV
    MMSDK_U32 u32CC708BitStreamAddr;
    /// The size of bit stream buf for CEA-708(EIA-708)
    MMSDK_U32 u32CC708BitStreamSize;
    /// Media format type if DSS or not
    MMSDK_BOOL bFormatTypeIsDSS;
} ST_MMSDK_CC_INIT_INFO;

/*
 /// Define the attached information for callback process. The callback message is E_CPCODEC_MSG_CC_ENABLE.
 typedef struct
 {
     /// Media object relative information.
     void* Media;
     /// Enable/Disable close caption show.
     MMSDK_BOOL bEnable;
} ST_MMSDK_CC_ENABLE_INFO;
*/

/// CC callback function
/// u32Msg plz use EN_MMSDK_CC_CB as callback message.
typedef MMSDK_BOOL (*pfnMmsdkCCCallback)(void* pClass, MMSDK_U32 u32Msg, void* pMsgInfo, void* pAckInfo);

//-------------------------------------------------------------------------------------------------
/// Set CC initialize
/// @since version 1.01
/// @param CCItem                 \b OUT: CC instance item
/// @param pstCCInfo              \b IN: CC initialize information
/// @param pClass                 \b IN: callback target class
/// @param pCallback              \b IN: callback function
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL PT_CCInit(PT_CCITEM* CCItem, const ST_MMSDK_CC_INIT_INFO *pstCCInfo, void* pClass, const pfnMmsdkCCCallback pCallback);

//------------------------------------------------------------------------------
/// Set CC deinit
/// @since version 1.01
/// @param CCItem                 \b IN: CC instance item
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//------------------------------------------------------------------------------
MMSDK_BOOL PT_CCDeInit(PT_CCITEM* CCItem);


//-------------------------------------------------------------------------------------------------
/// Display CC
/// @since version 1.01
/// @param CCItem                 \b IN: CC instance item
/// @param eCmdType               \b IN: set CC cmd type.
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL PT_CCDisplay(PT_CCITEM CCItem, const EN_MMSDK_CC_CMD_TYPE eCmdType);

//-------------------------------------------------------------------------------------------------
/// Inform CC Set Play Position ack
/// @since version 1.01
/// @param CCItem                 \b IN: CC instance item
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL PT_CCInformSetPositionAck(PT_CCITEM CCItem);

//-------------------------------------------------------------------------------------------------
/// Set CC trick mode
/// @since version 1.01
/// @param CCItem                 \b IN: CC instance item
/// @param eTrickMode             \b IN: trick mode
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL PT_CCSetTrickMode(PT_CCITEM CCItem, const EN_MMSDK_TRICK_MODE eTrickMode);

/// @} // end of porting_cc

#ifdef __cplusplus
}
#endif
#endif // __mmsdk_porting_cc__

///*** please do not remove change list tag ***///
///***$Change: 720163 $***///
