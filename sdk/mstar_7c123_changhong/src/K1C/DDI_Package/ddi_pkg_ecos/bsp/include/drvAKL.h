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
// Copyright (c) 2006-2009 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// ("MStar Confidential Information") by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   drvAKL.h
/// @brief  AKL Driver Interface
/// @author MStar Semiconductor,Inc.
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __DRV_AKL_H__
#define __DRV_AKL_H__

#include "MsTypes.h"
#include "MsCommon.h"

#include "UFO.h"

#ifdef UFO_AKL_CERT

#ifdef __cplusplus
extern "C"
{
#endif

//--------------------------------------------------------------------------------------------------
//  Define
//--------------------------------------------------------------------------------------------------
#define AKL_SET_KEY_TIMEOUT_VALUE    (20UL)

#define DRV_AKL_OK                   (0x00000000UL)
#define DRV_AKL_FAIL                 (0x00000001UL)
#define DRV_AKL_BAD_PARAM            (0x00000002UL)
#define DRV_AKL_TIMEOUT              (0x00000003UL)
//--------------------------------------------------------------------------------------------------
//  Driver Capability
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//  Local variable
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//  Type and Structure
//--------------------------------------------------------------------------------------------------
typedef MS_U32                         DRV_AKL_RET;

typedef enum
{
    E_AKL_DBG_NONE,
    E_AKL_DBG_ERR,
    E_AKL_DBG_WARN,
    E_AKL_DBG_INFO,
}AKL_DBGMSG_LEVEL;

typedef enum
{
    E_AKL_ENG_NONE,
    E_AKL_ENG_LSAS,
    E_AKL_ENG_ESA,
    E_AKL_ENG_LSAD,
} AKL_Eng_Type;

typedef enum
{
    E_AKL_KEY_CLEAR,
    E_AKL_KEY_EVEN,
    E_AKL_KEY_ODD,
} AKL_Key_Type;

typedef enum
{
    E_AKL_CERT_CMD_SET_INPUT,
    E_AKL_CERT_CMD_SET_OPCODE,
    E_AKL_CERT_CMD_GET_OPCODE,
    E_AKL_CERT_CMD_GET_STATUS,
    E_AKL_CERT_CMD_GET_OUPUT,
} AKL_CERT_Cmd;

MS_BOOL MDrv_AKL_Init(void);
MS_BOOL MDrv_AKL_Reset(void);
MS_BOOL MDrv_AKL_SetDbgLevel(AKL_DBGMSG_LEVEL eDbgLevel);
MS_BOOL MDrv_AKL_SetManualACK(void);
DRV_AKL_RET MDrv_AKL_SetDMAKey(MS_U32 u32KeyIndex);
DRV_AKL_RET MDrv_AKL_SetDMAParserKey(MS_U32 u32OddKeyIndex, MS_U32 u32EvenKeyIndex);
DRV_AKL_RET MDrv_AKL_SetDSCMBKey(MS_U32 u32EngId, MS_U32 u32Index, AKL_Eng_Type eEngType, AKL_Key_Type eKeyType);
DRV_AKL_RET MDrv_AKL_SetTSIOKey(MS_U32 u32ServiceId, AKL_Key_Type eKeyType);
DRV_AKL_RET MDrv_AKL_CERTCmd(AKL_CERT_Cmd eCmd, MS_U8 *pu8Data, MS_U32 u32DataSize);

MS_BOOL MDrv_AKL_SetManualACKMode(MS_BOOL bEnable);  //K series

#ifdef __cplusplus
}
#endif

#endif  //UFO_AKL_CERT

#endif
