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

#ifndef _DEMO_DSCMB_ADVANCED_H_
#define _DEMO_DSCMB_ADVANCED_H_
//--------------------------------------------------------------------------------------------------
// Structure define
//--------------------------------------------------------------------------------------------------
//EN_DEMO_DSCMB_TYPE, the tpye shall match with DSCMB_Type_Map[].name
typedef enum
{
    /// Descrambler algorithm : CSA
    EN_DEMO_DSCMB_TYPE_CSA = 0,    // CSA
    /// Descrambler algorithm : CSA Conformance
    EN_DEMO_DSCMB_TYPE_CSA_CONF,    // CSA CONF
    /// Descrambler algorithm : CSA3
    EN_DEMO_DSCMB_TYPE_CSA3,    // CSA3
    /// Descrambler algorithm : CI+ DES
    EN_DEMO_DSCMB_TYPE_DES,        // CI+ DES
    /// Descrambler algorithm : CI+ AES
    EN_DEMO_DSCMB_TYPE_AES,        // CI+ AES
    /// Descrambler algorithm : AES ECB
    EN_DEMO_DSCMB_TYPE_AES_ECB,        // AES ECB
    /// Descrambler algorithm : AES SCTE52
    EN_DEMO_DSCMB_TYPE_AES_SCTE52,        // Descrambler algorithm : DBook, AES+CBC, S8 new, not support for all chips
    /// Descrambler algorithm : TDES SCTE52
    EN_DEMO_DSCMB_TYPE_TDES_SCTE52,        //TDES SCTE52
    /// Descrambler algorithm : TDES ECB
    EN_DEMO_DSCMB_TYPE_TDES_ECB,        //TDES ECB
    /// Descrambler algorithm : Synamedia AES
    EN_DEMO_DSCMB_TYPE_SYN_AES,        // AES Leading CLR EBC
} EN_DEMO_DSCMB_TYPE;

typedef struct
{
    MS_BOOL bConnectStatus;
    MS_U8 *u8Type;
    MS_U8 *u8StrOddCW;
    MS_U8 *u8StrEvenCW;
    MS_U8 *u8StrOddIV;
    MS_U8 *u8StrEvenIV;
    MS_U32 u32CAVid;
} stDEMODSCMBINFO;

//-------------------------------------------------------------------------------------------------
// Demo Functions
//-------------------------------------------------------------------------------------------------
MS_BOOL Demo_DSCMB_InfoGet(MS_U32 u32DeviceId, stDEMODSCMBINFO *stInfo);

MS_U32  Demo_DSCMB_Create_Resource(void);
MS_BOOL Demo_DSCMB_Destroy_Resource(MS_U32 u32DscmbId);
MS_BOOL Demo_DSCMB_ConnectFltId(MS_U32 u32DscmbId, MS_U32 u32PidFlt);
MS_BOOL Demo_DSCMB_DisconnectFltId(MS_U32 u32DscmbId, MS_U32 u32PidFlt);
MS_BOOL Demo_DSCMB_SetFltType(MS_U32 u32DscmbId, MS_U8 *u8Type);
MS_BOOL Demo_DSCMB_SetCW(MS_U32 u32DscmbId, MS_U8 *u8StrOddCW, MS_U8 *u8StrEvenCW);
MS_BOOL Demo_DSCMB_SetIV(MS_U32 u32DscmbId, MS_U8 *u8StrOddIV, MS_U8 *u8StrEvenIV);

// DDI COMMAND FUNCTION
MS_BOOL Demo_DSCMB_SetCAVID(MS_U32 *CAVid);
MS_BOOL Demo_DSCMB_Help(void);
MS_BOOL Demo_DSCMB_ConnectPID(MS_U32 *u32DeviceId);
MS_BOOL Demo_DSCMB_DisconnectPID(MS_U32 *u32DeviceId);
MS_BOOL Demo_DSCMB_SetType(MS_U32 *u32DeviceId, MS_U8 *u8Type);
MS_BOOL Demo_DSCMB_CW(MS_U32 *u32DeviceId, MS_U8 *u8OddCW, MS_U8 *u8EvenCW);
MS_BOOL Demo_DSCMB_IV(MS_U32 *u32DeviceId, MS_U8 *u8OddIV, MS_U8 *u8EvenIV);
MS_BOOL Demo_DSCMB_KeyLadderMode(MS_U8 *u8Mode);
MS_BOOL Demo_DSCMB_KeyLadder(MS_U32 *u32DeviceId, MS_U8 *u8Source, MS_U8 *u8HexHostKey, MS_U8 *u8HexEncryptedOddCW, MS_U8 *u8HexEncryptedEvenCW);

#endif /*_DEMO_DSCMB_ADVANCED_H_*/


