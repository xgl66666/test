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

//////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   drvSC_v2.h
/// @brief  SC Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DRV_SC_V2_H_
#define _DRV_SC_V2_H_

#include "drvSC.h"

#ifdef __cplusplus
extern "C"
{
#endif

//-------------------------------------------------------------------------------------------------
// Defines
//-------------------------------------------------------------------------------------------------
// below for utopia20 structure

typedef enum
{
    E_MDRV_CMD_SC_Init,
    E_MDRV_CMD_SC_Exit,
    E_MDRV_CMD_SC_Open,
    E_MDRV_CMD_SC_Activate,
    E_MDRV_CMD_SC_Deactivate,
    E_MDRV_CMD_SC_Close,
    E_MDRV_CMD_SC_Reset,
    E_MDRV_CMD_SC_ClearState,
    E_MDRV_CMD_SC_GetATR,
    E_MDRV_CMD_SC_Conig,
    E_MDRV_CMD_SC_Send,
    E_MDRV_CMD_SC_Recv,
    E_MDRV_CMD_SC_GetCaps,
    E_MDRV_CMD_SC_SetPPS,
    E_MDRV_CMD_SC_PPS,
    E_MDRV_CMD_SC_GETSTATUS,
    E_MDRV_CMD_SC_SETDBGLEVEL,
    E_MDRV_CMD_SC_RAWEXCHANGE,
    E_MDRV_CMD_SC_CARDVOLTAGE_CONFIG,
    E_MDRV_CMD_SC_KDRVKERNELMODEPOLL,
    E_MDRV_CMD_SC_RESET_ATR,
    E_MDRV_CMD_SC_T0_SENDRECV,
    E_MDRV_CMD_SC_T1_SENDRECV,
    E_MDRV_CMD_SC_T14_SENDRECV,
    E_MDRV_CMD_SC_GETINFO,
    E_MDRV_CMD_SC_POWEROFF,
    E_MDRV_CMD_SC_SETGUARDTIME,
    E_MDRV_CMD_SC_SETPOWERSTATE
} eScIoctlOpt;

//-------------------------------------------------------------------------------------------------
// Type and Structure Declaration
//-------------------------------------------------------------------------------------------------
// below are for utopia20 cmd and parameters structure

typedef MS_U8 SC_GENERIC_PARAM;

typedef struct DLL_PACKED _SC_OPEN_PARAM
{
    MS_U8 u8SCID;
    MS_U8 u8Protocol;
    SC_Param* pParam;
    P_SC_Callback pfSmartNotify;
}SC_OPEN_PARAM, *PSC_OPEN_PARAM;

typedef struct DLL_PACKED _SC_RESET_PARAM
{
    MS_U8 u8SCID;
    SC_Param* pParam;
}SC_RESET_PARAM, *PSC_RESET_PARAM;

typedef struct DLL_PACKED _SC_GETATR_PARAM
{
    MS_U8 u8SCID;
    MS_U32 u32TimeOut;
    MS_U8 *pu8Atr;
    MS_U16 *pu16AtrLen;
    MS_U8 *pu8His;
    MS_U16 *pu16HisLen;
}SC_GETATR_PARAM, *PSC_GETATR_PARAM;

typedef struct DLL_PACKED _SC_CONFIG_PARAM
{
    MS_U8 u8SCID;
    SC_Param* pParam;
}SC_CONFIG_PARAM, *PSC_CONFIG_PARAM;

typedef struct DLL_PACKED _SC_SEND_PARAM
{
    MS_U8 u8SCID;
    MS_U8 *pu8SendData;
    MS_U16 u16SendDataLen;
    MS_U32 u32TimeoutMs;
}SC_SEND_PARAM, *PSC_SEND_PARAM;

typedef struct DLL_PACKED _SC_RECV_PARAM
{
    MS_U8 u8SCID;
    MS_U8 *pu8ReadData;
    MS_U16 *pu16ReadDataLen;
    MS_U32 u32TimeoutMs;
}SC_RECV_PARAM, *PSC_RECV_PARAM;

typedef struct DLL_PACKED _SC_GETCAPS_PARAM
{
    SC_Caps *pstCaps;
}SC_GETCAPS_PARAM, *PSC_GETCAPS_PARAM;

typedef struct DLL_PACKED _SC_SETPPS_PARAM
{
    MS_U8 u8SCID;
    MS_U8 u8SCProtocol;
    MS_U8 u8Di;
    MS_U8 u8Fi;
}SC_SETPPS_PARAM, *PSC_SETPPS_PARAM;

typedef struct DLL_PACKED _SC_GETSTATUS_PARAM
{
    MS_U8 u8SCID;
    SC_Status *pstStatus;
}SC_GETSTATUS_PARAM, *PSC_GETSTATUS_PARAM;

typedef SC_DbgLv SC_SETDBGLEVEL_PARAM;

typedef struct DLL_PACKED _SC_RAWEXCHANGE_PARAM
{
    MS_U8 u8SCID;
    MS_U8 *pu8SendData;
    MS_U16 *u16SendDataLen;
    MS_U8 *pu8ReadData;
    MS_U16 *u16ReadDataLen;
}SC_RAWEXCHANGE_PARAM, *PSC_RAWEXCHANGE_PARAM;

typedef struct DLL_PACKED _SC_CARDVOLTAGE_CONFIG_PARAM
{
    MS_U8 u8SCID;
    SC_VoltageCtrl eVoltage;
}SC_CARDVOLTAGE_CONFIG_PARAM, *PSC_CARDVOLTAGE_CONFIG_PARAM;

typedef struct DLL_PACKED _SC_KDRVKERNELMODEPOLL_PARAM
{
    MS_U8 u8SCID;
    MS_U32 *pu32Event;
    MS_U32 u32TimeoutMs;
}SC_KDRVKERNELMODEPOLL_PARAM, *PSC_KDRVKERNELMODEPOLL_PARAM;

typedef struct DLL_PACKED _SC_RESET_ATR_PARAM
{
    MS_U8 u8SCID;
    SC_Param* pParam;
    MS_U8 *pu8Atr;
    MS_U16 *pu16AtrLen;
    MS_U8 *pu8His;
    MS_U16 *pu16HisLen;
}SC_RESET_ATR_PARAM, *PSC_RESET_ATR_PARAM;

typedef struct DLL_PACKED _SC_T0_SENDRECV_PARAM
{
    MS_U8 u8SCID;
    MS_U8 *pu8SendData;
    MS_U16 u16SendLen;
    MS_U8 *pu8RecvData;
    MS_U16 *pu16RecvLen;
}SC_T0_SENDRECV_PARAM, *PSC_T0_SENDRECV_PARAM;

typedef struct DLL_PACKED _SC_T1_SENDRECV_PARAM
{
    MS_U8 u8SCID;
    MS_U8 *pu8SendData;
    MS_U16 *u16SendDataLen;
    MS_U8 *pu8ReadData;
    MS_U16 *u16ReadDataLen;
}SC_T1_SENDRECV_PARAM, *PSC_T1_SENDRECV_PARAM;

typedef struct DLL_PACKED _SC_T14_SENDRECV_PARAM
{
    MS_U8 u8SCID;
    MS_U8 *pu8SendData;
    MS_U16 u16SendLen;
    MS_U8 *pu8RecvData;
    MS_U16 *pu16RecvLen;
}SC_T14_SENDRECV_PARAM, *PSC_T14_SENDRECV_PARAM;

typedef struct DLL_PACKED _SC_GETINFO_PARAM
{
    MS_U8 u8SCID;
    SC_Info *pstInfo;
}SC_GETINFO_PARAM, *PSC_GETINFO_PARAM;

typedef struct DLL_PACKED _SC_SETGUARDTIME_PARAM
{
    MS_U8 u8SCID;
    MS_U8 u8GuardTime;
}SC_SETGUARDTIME_PARAM, *PSC_SETGUARDTIME_PARAM;

typedef EN_POWER_MODE SC_SETPOWERSTATE_PARAM;

#ifdef __cplusplus
}
#endif

#endif // _DRV_SC_V2_H_
