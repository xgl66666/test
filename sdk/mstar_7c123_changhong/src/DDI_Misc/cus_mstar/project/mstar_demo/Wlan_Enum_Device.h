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

//------------------------------------------------------------------------------------
//  WLAN_DEVICE_TYPE
//------------------------------------------------------------------------------------

//--------------- external demods start ---------------
#define WLAN_MT7601U               0

//not exist
#define WLAN_NULL                  254
#define WLAN_UNKNWON               255


#ifndef WLAN_LOCAL_SUPPORT_LIST
#include "Wlan_Support_List.h"
#else
#include "Local_Wlan_Support_List.h"
#endif

//------------------------------ Demod Usage ----------------------------------
#if WLAN_14TH_SUPPORT_DEVICE != WLAN_NULL
#define WLAN_SUPPORT_NUMBER 15
#elif WLAN_13TH_SUPPORT_DEVICE != WLAN_NULL
#define WLAN_SUPPORT_NUMBER 14
#elif WLAN_12TH_SUPPORT_DEVICE != WLAN_NULL
#define WLAN_SUPPORT_NUMBER 13
#elif WLAN_11TH_SUPPORT_DEVICE != WLAN_NULL
#define WLAN_SUPPORT_NUMBER 12
#elif WLAN_10TH_SUPPORT_DEVICE != WLAN_NULL
#define WLAN_SUPPORT_NUMBER 11
#elif WLAN_09TH_SUPPORT_DEVICE != WLAN_NULL
#define WLAN_SUPPORT_NUMBER 10
#elif WLAN_08TH_SUPPORT_DEVICE != WLAN_NULL
#define WLAN_SUPPORT_NUMBER  9
#elif WLAN_07TH_SUPPORT_DEVICE != WLAN_NULL
#define WLAN_SUPPORT_NUMBER  8
#elif WLAN_06TH_SUPPORT_DEVICE != WLAN_NULL
#define WLAN_SUPPORT_NUMBER  7
#elif WLAN_05TH_SUPPORT_DEVICE != WLAN_NULL
#define WLAN_SUPPORT_NUMBER  6
#elif WLAN_04TH_SUPPORT_DEVICE != WLAN_NULL
#define WLAN_SUPPORT_NUMBER  5
#elif WLAN_03RD_SUPPORT_DEVICE != WLAN_NULL
#define WLAN_SUPPORT_NUMBER  4
#elif WLAN_02ND_SUPPORT_DEVICE != WLAN_NULL
#define WLAN_SUPPORT_NUMBER  3
#elif WLAN_01ST_SUPPORT_DEVICE != WLAN_NULL
#define WLAN_SUPPORT_NUMBER  2
#elif WLAN_00TH_SUPPORT_DEVICE != WLAN_NULL
#define WLAN_SUPPORT_NUMBER  1
#else
#define WLAN_SUPPORT_NUMBER  0
#endif


//#define IF_THIS_WLAN_INUSE(x) 1
#define IF_THIS_WLAN_INUSE(x) (((WLAN_00TH_SUPPORT_DEVICE == x) && (WLAN_SUPPORT_NUMBER > 0)) ||\
                                ((WLAN_01ST_SUPPORT_DEVICE == x) && (WLAN_SUPPORT_NUMBER > 1)) ||\
                                ((WLAN_02ND_SUPPORT_DEVICE == x) && (WLAN_SUPPORT_NUMBER > 2)) ||\
                                ((WLAN_03RD_SUPPORT_DEVICE == x) && (WLAN_SUPPORT_NUMBER > 3)) ||\
                                ((WLAN_04TH_SUPPORT_DEVICE == x) && (WLAN_SUPPORT_NUMBER > 4)) ||\
                                ((WLAN_05TH_SUPPORT_DEVICE == x) && (WLAN_SUPPORT_NUMBER > 5)) ||\
                                ((WLAN_06TH_SUPPORT_DEVICE == x) && (WLAN_SUPPORT_NUMBER > 6)) ||\
                                ((WLAN_07TH_SUPPORT_DEVICE == x) && (WLAN_SUPPORT_NUMBER > 7)) ||\
                                ((WLAN_08TH_SUPPORT_DEVICE == x) && (WLAN_SUPPORT_NUMBER > 8)) ||\
                                ((WLAN_09TH_SUPPORT_DEVICE == x) && (WLAN_SUPPORT_NUMBER > 9)) ||\
                                ((WLAN_10TH_SUPPORT_DEVICE == x) && (WLAN_SUPPORT_NUMBER > 10)) ||\
                                ((WLAN_11TH_SUPPORT_DEVICE == x) && (WLAN_SUPPORT_NUMBER > 11)) ||\
                                ((WLAN_12TH_SUPPORT_DEVICE == x) && (WLAN_SUPPORT_NUMBER > 12)) ||\
                                ((WLAN_13TH_SUPPORT_DEVICE == x) && (WLAN_SUPPORT_NUMBER > 13)) ||\
                                ((WLAN_14TH_SUPPORT_DEVICE == x) && (WLAN_SUPPORT_NUMBER > 14)) )

