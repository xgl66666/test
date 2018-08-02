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


/////////////////////////////////////////////////////////////////////////////////////
/// file    apiVectorFont.c
/// @brief  Vector Font system API
/// @author MStar Semiconductor Inc.
///
/////////////////////////////////////////////////////////////////////////////////////
#include "HbCommon.h"

#include "apiMvf_types.h"
#include "apiMvf.h"
#include "apiVectorFont.h"
//#include "apiMVF_Font.h"

#include "MVF_en.h"
#include "MVF_tc.h"
#include "MVF_sc.h"
//#include "MVF_sc2.h"

#define VF_DEBUG
#ifdef VF_DEBUG
#define DEBUG_VF(x) (x)
#else
#define DEBUG_VF(x)
#endif


#if USE_MVF_PURE_ENGLISH_FONT
    static unsigned long u32MVF_Size = MS_MVF_EN_SIZE;
    static unsigned char *pMVF_Data = MS_MVF_EN_DATA;
#elif USE_MVF_TRADITIONAL_CHINESE_FONT
    static unsigned long u32MVF_Size = MS_MVF_TC_SIZE;
    static unsigned char *pMVF_Data = MS_MVF_TC_DATA;
#elif USE_MVF_SIMPLIFIED_CHINESE_FONT
//    static unsigned long u32MVF_Size = MS_MVF_SC_SIZE;
//    static unsigned char *pMVF_Data = MS_MVF_SC_DATA;
    static unsigned long u32MVF_Size = MS_MVF_SC2_SIZE;
    static unsigned char *pMVF_Data = MS_MVF_SC2_DATA;
#else
    static unsigned long u32MVF_Size = 1;
    static unsigned char *pMVF_Data = &u32MVF_SIZE;
#endif


extern MS_S32 gs32CachedPoolID;
extern MS_S32 gs32NonCachedPoolID;

//MVF_Face g_mvfFace;
#if 1//ATSC_CC_ENABLE //PATA test
MVF_Face g_cc_mvfFace; //kevinchen FOR CC
#endif
//------------------------------------------------------------------------------
/// @brief The function initialize the MStar Vector font system
/// @return the status of initial
//------------------------------------------------------------------------------
MS_BOOL MApi_VectorFont_Init(void)
{
    MVF_Error err;

    void * addr;
    MS_U32 u32PoolSize, u32FreeSize, u32LargeSize;

    MsOS_InfoMemoryPool (gs32CachedPoolID,&addr,&u32PoolSize,&u32FreeSize,&u32LargeSize);


    DEBUG_VF(HB_printf("MApi_VectorFont_Init total size=%lu, free size=%lu, large size=%lu\n", u32PoolSize, u32FreeSize, u32LargeSize));

   /* err = MVF_New_Face(pMVF_Data, pMVF_Data+u32MVF_Size, &g_mvfFace);
    if ( err )
    {
        DEBUG_VF(HB_printf("MApi_VectorFont_Init() -> MVF_New_Face error!\n"));
        return (FALSE);
    }*/
#if 1 //ATSC_CC_ENABLE //PATA test
printf("\033[1;32;40mERR!!!! %s %d\033[0m\n", __FUNCTION__, __LINE__);
    err = MVF_New_Face(pMVF_Data, pMVF_Data+u32MVF_Size, &g_cc_mvfFace);
    if ( err )
    {
        DEBUG_VF(HB_printf("MApi_VectorFont_Init() -> MVF_New_Face for CC error! err %d\n", err));
        return (FALSE);
    }
#endif
    return (TRUE);
}


//------------------------------------------------------------------------------
/// @brief The function de-initialize the MStar Vector font system
/// @return none
//------------------------------------------------------------------------------
void MApi_VectorFont_DeInit(void)
{
    MVF_Done_Face(&g_cc_mvfFace);
    MVF_Done_Render();

    void * addr;
    MS_U32 u32PoolSize, u32FreeSize, u32LargeSize;
    MsOS_InfoMemoryPool (gs32CachedPoolID,
                          &addr,
                          &u32PoolSize,
                          &u32FreeSize,
                          &u32LargeSize);
    DEBUG_VF(HB_printf("MApi_VectorFont_DeInit() total size=%lu, free size=%lu, large size=%lu\n", u32PoolSize, u32FreeSize, u32LargeSize));
}




