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

#ifndef _DEMO_KL_H_
#define _DEMO_KL_H_
#if (DEMO_DSCMB_ADVANCED_TEST == 1)
#include "drvDSCMB.h"
typedef enum
{
    E_DEMO_KL_TDES_MODE = E_DSCMB_KL_TDES ,
    E_DEMO_KL_AES_MODE = E_DSCMB_KL_AES,
} EN_DEMO_KL_Mode;


typedef enum
{
    E_DEMO_KL_SRC_ACPU = E_DSCMB_KL_SRC_ACPU,/// Key ladder source from ACPU
    E_DEMO_KL_SRC_SECRET_1 = E_DSCMB_KL_SRC_SECRET_1,/// Key ladder source from SECRET_1
    E_DEMO_KL_SRC_SECRET_2 = E_DSCMB_KL_SRC_SECRET_2,/// Key ladder source from SECRET_2
    E_DEMO_KL_SRC_SECRET_3 = E_DSCMB_KL_SRC_SECRET_3,/// Key ladder source from SECRET_3
    E_DEMO_KL_SRC_SECRET_4 = E_DSCMB_KL_SRC_SECRET_4,/// Key ladder source from SECRET_4
    E_DEMO_KL_SRC_SECRET_5 = E_DSCMB_KL_SRC_SECRET_5,/// Key ladder source from SECRET_5
    E_DEMO_KL_SRC_SECRET_6 = E_DSCMB_KL_SRC_SECRET_6,/// Key ladder source from SECRET_6
    E_DEMO_KL_SRC_SECRET_7 = E_DSCMB_KL_SRC_SECRET_7,/// Key ladder source from SECRET_7
    E_DEMO_KL_SRC_SECRET_8 = E_DSCMB_KL_SRC_SECRET_8,/// Key ladder source from SECRET_8
} EN_DEMO_KL_SRC;


typedef enum
{
    /// Key ladder destination to ACPU
    E_DEMO_KL_DST_ACPU = E_DSCMB_KL_DST_CPU_ACPU,
    /// Key ladder destination to Key Table ESA for DSCMB
    E_DEMO_KL_DST_KT_ESA = E_DSCMB_KL_DST_KT_ESA,
    /// Key ladder destination to AES Key Slot for DMA
    E_DEMO_KL_DST_DMA_AES = E_DSCMB_KL_DST_DMA_AES,
    //Key ladder destination to three key slot for DMA(CIPHER)
#if (DEMO_CIPHER_TEST == 1)
    E_DEMO_KL_DST_DMA_SK0 = E_DSCMB_KL_DST_DMA_SK0,
    E_DEMO_KL_DST_DMA_SK1 = E_DSCMB_KL_DST_DMA_SK1,
    E_DEMO_KL_DST_DMA_SK2 = E_DSCMB_KL_DST_DMA_SK2,
#endif
} EN_DEMO_KL_DST;




typedef enum
{
    /// Descrambler key type : clear
    E_DEMO_KL_KEY_CLEAR = E_DSCMB_KEY_CLEAR,
    /// Descrambler key type : odd
    E_DEMO_KL_KEY_ODD = E_DSCMB_KEY_ODD,
    /// Descrambler key type : even
    E_DEMO_KL_KEY_EVEN = E_DSCMB_KEY_EVEN,
} EN_DEMO_KL_Type;



typedef struct
{
    EN_DEMO_KL_Mode eKLMode;
    EN_DEMO_KL_SRC eSrc;
    EN_DEMO_KL_DST eDst;
    EN_DEMO_KL_Type eKeyType; // Don't care when dst is Crypto DMA
    MS_U32 u32DscmbId;// Don't care when dst is Crypto DMA
    MS_U32 u32EngID; // Don't care when dst is Crypto DMA
    MS_U32 u32Level;
    MS_U8 *pu8KeyACPU;
    MS_U8 *pu8KeyKLIn;
    MS_U32 u32CAVid;
} DEMOKLCONFIG;

MS_BOOL DEMO_KeyLadder_Setup(DEMOKLCONFIG stKLInfo);

#endif
#endif

