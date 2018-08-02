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
// Copyright (c) 2008-2009 MStar Semiconductor, Inc.
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

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    drvTEMP.c
/// @brief  TEMP Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////
#define DRV_SCALER_ADAPTIVE_C

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
// Common Definition
#include "MsCommon.h"
#include "MsOS.h"

// Internal Definition
#include "color_reg.h"

#include "drvXC_IOPort.h"
//#include "drvGlobal.h"

#include "apiXC.h"
#include "apiXC_Dlc.h"
#include "msAPI_Scaler_Adaptive.h"

#ifndef UNUSED //to avoid compile warnings...
#define UNUSED(var) (void)((var) = (var))
#endif

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define DRVSCA_DBG(x)       //x
#define DBG_DEFEATHERING    0
#define DBG_DEFLICKERING    0
#define DBG_DEBOUNCING      0
#define DBG_DYNAMIC_SNR     0
#define DBG_DYNAMIC_DNR     0

// DeFeathering
#define MDP_CNT     2
#define ENABLE_MDP  0   //Motion decrase progressively enable

#define DEFETHERING_LV1_TH              30000
#define DEFETHERING_LV2_TH              5000
#define DEFETHERING_LV3_TH              2000
#define DEFETHERING_LV1_CNT             25
#define DEFETHERING_LV2_CNT             20
#define DEFETHERING_LV3_CNT             5

#define SST_STATIC_CORE_TH_LV1_VALUE    0x1C              //ryan update
#define SST_STATIC_CORE_TH_LV2_VALUE    0x0C              //ryan update
#define SST_STATIC_CORE_TH_LV3_VALUE    0x06              //ryan update
#define SST_STATIC_CORE_TH_LV4_VALUE    0x00              //ryan update

// DeFlickering
#define DEFLICKERING_TH                 52000
#define DEFLICKERING_CNT                150

// DeBouncing
#define DEBOUNCING_TH                   35000
#define DEBOUNCING_CNT                  10

// Dynamic SNR
#define DYNAMIC_SNR_TH                  2000
#define DYNAMIC_SNR_CNT                 30

// Dynamic DNR
//#define DYNAMIC_DNR_TH                  6000

#define DNR_TABLEY_0L_Zero_VALUE        0xDD
#define DNR_TABLEY_0H_Zero_VALUE        0xBD
#define DNR_TABLEY_1L_Zero_VALUE        0x79
#define DNR_TABLEY_1H_Zero_VALUE        0x35
#define DNR_TABLEY_2L_Zero_VALUE        0x11
#define DNR_TABLEY_2H_Zero_VALUE        0x00
#define DNR_TABLEY_3L_Zero_VALUE        0x00
#define DNR_TABLEY_3H_Zero_VALUE        0x00

#define DNR_TABLEY_0L_LV2_VALUE         0xCC
#define DNR_TABLEY_0H_LV2_VALUE         0xAC
#define DNR_TABLEY_1L_LV2_VALUE         0x68
#define DNR_TABLEY_1H_LV2_VALUE         0x24
#define DNR_TABLEY_2L_LV2_VALUE         0x00
#define DNR_TABLEY_2H_LV2_VALUE         0x00
#define DNR_TABLEY_3L_LV2_VALUE         0x00
#define DNR_TABLEY_3H_LV2_VALUE         0x00

#define DNR_TABLEY_0L_LV3_VALUE         0x67
#define DNR_TABLEY_0H_LV3_VALUE         0x45
#define DNR_TABLEY_1L_LV3_VALUE         0x33
#define DNR_TABLEY_1H_LV3_VALUE         0x22
#define DNR_TABLEY_2L_LV3_VALUE         0x11
#define DNR_TABLEY_2H_LV3_VALUE         0x00
#define DNR_TABLEY_3L_LV3_VALUE         0x00
#define DNR_TABLEY_3H_LV3_VALUE         0x00

// Dynamic Film 22
#define DYNAMIC_FILM22_TH               520000

// AUTO NR, HISDNR(Maria10)
#define noise_level_low_thrd_0              0x00
#define noise_level_low_thrd_1              0x18
#define noise_level_low_thrd_2              0x28
#define noise_level_low_thrd_3              0x38
#define noise_level_high_thrd_3             0x50  // 0x60 (T9)

#define noise_bin_step                          0x00
#define noise_bin_thrd                          0x4C

#define hismatch_level0                                 0x80
#define hismatch_level1                                 0x64
#define hismatch_level2                                 0x30
#define hismatch_level3                                 0x20

#define dnr_std_level0                                  0x0
#define dnr_std_level1                                  0x5
#define dnr_std_level2                                  0x8
#define dnr_std_level3                                  0x0
/*
#define dnr_md_high_thrd_level0         0x2
#define dnr_md_low_thrd_level0          0x0
#define dnr_md_high_thrd_level1         0x8
#define dnr_md_low_thrd_level1          0x6
#define dnr_md_high_thrd_level2         0x9
#define dnr_md_low_thrd_level2          0x8
#define dnr_md_high_thrd_level3         0x12
#define dnr_md_low_thrd_level3          0x10
*/
#define md_adjust_level0                0x2
#define md_adjust_level1                0xa
#define md_adjust_level2                0x6
#define md_adjust_level3                0xe

#define psnr_std_level0_thrd            0x13
#define psnr_std_level1_thrd            0x25
#define psnr_std_level2_thrd            0x20
#define psnr_std_level3_thrd            0x0

//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------


/******************************************************************************/
///Get control register for adaptive tuning function
///@return MS_U8: Control status
/******************************************************************************/
MS_U8 MDrv_SC_get_adaptive_ctrl(void)
{
    MS_U8 u8Ctrl;

    u8Ctrl = (MS_U8)MApi_XC_R2BYTEMSK(REG_SC_BK22_7C_L, 0xFF);

    return u8Ctrl;
}

/******************************************************************************/
///Get control register for adaptive tuning function
///@return MS_U8: Control status
/******************************************************************************/
MS_U8 MDrv_SC_get_adaptive_ctrl2(void)
{
    MS_U8 u8Ctrl;

    u8Ctrl = (MS_U8)(MApi_XC_R2BYTEMSK(REG_SC_BK22_7C_L, 0xFF00) >> 8);

    return u8Ctrl;
}

/******************************************************************************/
///Read motion value (F2 motion status)
///@return MS_U8: Motion value
/******************************************************************************/
MS_U32 MDrv_SC_read_motion_value1(void)
{

    MS_U32 u32MotionValue;
    MS_U32 u32RegMadi_1C, u32RegMadi_1B, u32RegMadi_1A;

    u32RegMadi_1C = (MS_U32)MApi_XC_R2BYTEMSK(REG_SC_BK22_0E_L, 0x3F);
    u32RegMadi_1B = (MS_U32)MApi_XC_R2BYTEMSK(REG_SC_BK22_0D_L, 0xFF00);
    u32RegMadi_1A = (MS_U32)MApi_XC_R2BYTEMSK(REG_SC_BK22_0D_L, 0x00FF);

    u32RegMadi_1C = u32RegMadi_1C * 0x10000UL;

    u32MotionValue = (u32RegMadi_1C + u32RegMadi_1B + u32RegMadi_1A)  ;

    DRVSCA_DBG(printf("MotionValue = 0x%lx\n", u32MotionValue));

    return u32MotionValue;
}

/******************************************************************************/
///Read motion value (Motion count status)
///@return MS_U8: Motion value
/******************************************************************************/
MS_U32 MDrv_SC_read_motion_value2(void)
{

    MS_U32 u32MotionValue;

    u32MotionValue = (MS_U32)MApi_XC_R2BYTEMSK(REG_SC_BK0A_1A_L, 0x7F);

    return u32MotionValue;
}

//-------------------------------------------------------------------------------------------------
/// Read motion value (Motion count status)
/// @return MS_U8: Motion value
//-------------------------------------------------------------------------------------------------
MS_U8 MApi_XC_GetCurrentMotionValue(void)
{

    MS_U8 u8MotionValue;

    u8MotionValue  = (MS_U8)((MApi_XC_R2BYTEMSK(REG_SC_BK22_0E_L, 0x3F) << 2) |
                             (MApi_XC_R2BYTEMSK(REG_SC_BK22_0D_L, 0xC000) >> 6));

    return u8MotionValue;
}

/******************************************************************************/
///Read VCnt value
///@return MS_U8: VCnt value
/******************************************************************************/
MS_U8 MDrv_SC_read_v_cnt(void)
{
    MS_U8 u8VCnt;

    u8VCnt = (MS_U8)(MApi_XC_R2BYTEMSK(REG_SC_BK22_0E_L, 0xC0) >> 6);
    return u8VCnt;
}

/******************************************************************************/
///Read SDHDD Detect Threshold value
///@return MS_U16: HDSDD_Det_Threshold value
/******************************************************************************/
MS_U16 MDrv_SC_read_HDSDD_Det_Threshold_value(void)
{

    MS_U16 u16Value;

    u16Value = (MS_U16)MApi_XC_R2BYTEMSK(REG_SC_BK22_7F_L, 0xFFFF);

    return u16Value;
}

/******************************************************************************/
///MCDi Driver
/******************************************************************************/
#define     ENABLE_XXX_V_MOVING     0x01
#define     ENABLE_XXX_SST          0x02
#define     ENABLE_XXX_EodiWeight   0x04


void MDrv_SC_mcdi_driver(MS_U32 u32MotionValue)
{
    MS_U32 u32Reg78, u32Reg79, u32Reg7A, u32Reg7B, u32Reg7C, u32Reg7D, u32Reg7E, filmFeatherCount;
    MS_U32 curCounter3, cntDiff;
    MS_U8 u8xxxCtrl;
    MS_BOOL verticalMovingU, horizontalMoving, slowMotion, featheringScene, featheringScene2;
    static MS_U8 u8SkipCnt;
    static MS_U8 featheringFrameCount;
    static MS_U8 s8DeBouncingCnt;
    static MS_U8 stillFrameCnt;

    static MS_U8 filmCnt = 0;
    static MS_U32 preCounter3 = 0;
    static MS_U8 eodiCnt = 0;

    MS_BOOL skipCond;
    MS_BOOL verticalMoving2;

    MS_BOOL filmDisableMCDiDFK;

    u8xxxCtrl = 0xFF; //MApi_XC_R2BYTE(REG_SC_BK2A_1F_L) >> 8;

    u32Reg78 = MApi_XC_R2BYTE(REG_SC_BK2A_78_L);
    u32Reg79 = MApi_XC_R2BYTE(REG_SC_BK2A_79_L);
    u32Reg7A = MApi_XC_R2BYTE(REG_SC_BK2A_7A_L);
    u32Reg7B = MApi_XC_R2BYTE(REG_SC_BK2A_7B_L);
    u32Reg7C = MApi_XC_R2BYTE(REG_SC_BK2A_7C_L);
    u32Reg7D = MApi_XC_R2BYTE(REG_SC_BK2A_7D_L);
    u32Reg7E = MApi_XC_R2BYTE(REG_SC_BK2A_7E_L);
    filmFeatherCount = MApi_XC_R2BYTE(REG_SC_BK0A_0B_L);

#if 0
    verticalMovingCharacterFavorMotion =
        u32Reg78 < 0x400 && u32Reg79 < 0x400 && u32Reg7A > 0x800 &&
        u32Reg7C < 0x100 && u32Reg7D > 0x200 && u32Reg7E > 0x200;
#endif

    //printf("MDrv_SC_mcdi_driver::verticalMovingCharacterFavorMotion:%d\n", verticalMovingCharacterFavorMotion);

    //if(!MirrorEnable)
    if(MApi_XC_GetMirrorModeType() == MIRROR_NORMAL)
    {
        //Non_Mirror
        verticalMovingU = (u32Reg7E) > 0x0100 && (u32Reg7E > 8 * u32Reg7C) && (u32Reg7E > 8 * u32Reg7D);
        verticalMoving2 = ((u32Reg7E > 0x200 && u32Reg7E > u32Reg7C) || (u32Reg7D > 0x200 && u32Reg7D > u32Reg7C));
        horizontalMoving = u32Reg79 > 0x200 && u32Reg7B < 0x40 && u32Reg79 > u32Reg78 && !verticalMoving2;
        slowMotion = u32Reg7A > 0x200 && u32Reg7B < 0x40 && !verticalMoving2;
        //featheringScene = (u32MotionValue > 0x0F) && (u32Reg7B > 0x800);
        featheringScene = u32Reg7B > 0x100 && u32Reg7B > u32Reg79 / 4 && u32Reg7B > u32Reg7A / 4;
        skipCond = (u32Reg7A > 8 * u32Reg79) && (u32Reg7B > 0x600);
        featheringScene2 = u32Reg7B > 0x200 && u32Reg7B > u32Reg79 / 8 && u32Reg7B > u32Reg7A / 8;
    }
    else
    {
        //Mirror
        verticalMovingU = (u32Reg7D) > 0x0100 && (u32Reg7D > 8 * u32Reg7C) && (u32Reg7D > 8 * u32Reg7E);
        verticalMoving2 = ((u32Reg7D > 0x200 && u32Reg7D > u32Reg7C) || (u32Reg7E > 0x200 && u32Reg7E > u32Reg7C));
        horizontalMoving = u32Reg79 > 0x200 && u32Reg7B < 0x40 && u32Reg79 > u32Reg78 && !verticalMoving2;
        slowMotion = u32Reg7A > 0x200 && u32Reg7B < 0x40 && !verticalMoving2;
        //featheringScene = (u32MotionValue > 0x0F) && (u32Reg7B > 0x800);
        featheringScene = u32Reg7B > 0x100 && u32Reg7B > u32Reg79 / 4 && u32Reg7B > u32Reg7A / 4;
        skipCond = (u32Reg7A > 8 * u32Reg79) && (u32Reg7B > 0x600);
        featheringScene2 = u32Reg7B > 0x200 && u32Reg7B > u32Reg79 / 8 && u32Reg7B > u32Reg7A / 8;
    }


    if(false == skipCond)
    {
        if(u8SkipCnt == 0)
            u8SkipCnt = 0;
        else
            u8SkipCnt--;
    }
    else
    {
        if(u8SkipCnt > 0x40)
            u8SkipCnt = 0x40;
        else
            u8SkipCnt++;
    }

    if(filmFeatherCount > 0x100)
    {
        if(featheringFrameCount < 0x80)
            featheringFrameCount ++;
    }
    else
    {
        featheringFrameCount = 0;
    }

    if(u32MotionValue == 0 && u32Reg7B < 0x20)
    {
        if(stillFrameCnt < 0x20)
            stillFrameCnt ++;
    }
    else
    {
        stillFrameCnt = 0;
    }

    ////////////////////////////////////////////////////////////////
    //  film content disable mcdi/dfk
    ////////////////////////////////////////////////////////////////

    curCounter3 = MApi_XC_R2BYTE(REG_SC_BK0A_0B_L);

    cntDiff = (preCounter3 > curCounter3) ? preCounter3 - curCounter3 : curCounter3 - preCounter3;

    if(cntDiff > 0x1500)
    {
        if(filmCnt != 10)
            filmCnt++;
    }
    else if(filmCnt != 0)
    {
        filmCnt--;
    }

    preCounter3 = curCounter3;
    //printf("filmCnt = [%x]\n", filmCnt);
    filmDisableMCDiDFK = (filmCnt > 5);

    ////////////////////////////////////////////////////////////////
    //  eodi counter
    ////////////////////////////////////////////////////////////////

    if(featheringScene2)
    {
        if(eodiCnt < 40)
            eodiCnt += 4;
    }
    else
    {
        if(eodiCnt > 0)
            eodiCnt--;
    }


    /////////////////////////////////////////////////////////////////
    //  SST (De-Feathering)
    /////////////////////////////////////////////////////////////////
    if(u8xxxCtrl & ENABLE_XXX_SST)
    {
        if(featheringScene || filmDisableMCDiDFK)       // moving & feather
        {
            MApi_XC_W2BYTEMSK(REG_SC_BK2A_29_L, (MS_U16)0x3100, 0xFF00); // increase SST weighting with checking feathering...
            MApi_XC_W2BYTEMSK(REG_SC_BK2A_2C_L, (MS_U16)0x0100, 0x0F00); // SST motion shift1
        }
        else if(u32Reg79 > 0x200 && u32Reg7B < 0x40)    // patch sony temple
        {
            MApi_XC_W2BYTEMSK(REG_SC_BK2A_29_L, (MS_U16)0x1100, 0xFF00); // default SST setting
            MApi_XC_W2BYTEMSK(REG_SC_BK2A_2C_L, (MS_U16)0x0200, 0x0F00); // SST motion shift1
        }
        else
        {
            MApi_XC_W2BYTEMSK(REG_SC_BK2A_29_L, (MS_U16)0x3100, 0xFF00); // default SST setting
            MApi_XC_W2BYTEMSK(REG_SC_BK2A_2C_L, (MS_U16)0x0100, 0x0F00); // SST motion shift1
        }
    }

    // disable DFK when verticalMovingU
    if(true == verticalMovingU || u8SkipCnt > 0x20 || filmDisableMCDiDFK)
    {
        MApi_XC_W2BYTEMSK(REG_SC_BK2A_28_L, (MS_U16)0x0000, 0xFFFF); // disable DFK
        MApi_XC_W2BYTEMSK(REG_SC_BK2A_2A_L, (MS_U16)0x0000, 0xF000); // disable DFK
    }
    else if(verticalMoving2)
    {
        MApi_XC_W2BYTEMSK(REG_SC_BK2A_28_L, (MS_U16)0xA8FF, 0xFFFF); // default DFK
        MApi_XC_W2BYTEMSK(REG_SC_BK2A_2A_L, (MS_U16)0x0000, 0xF000); // default DFK
    }
    else
    {
        MApi_XC_W2BYTEMSK(REG_SC_BK2A_28_L, (MS_U16)0xA8FF, 0xFFFF); // default DFK
        //MApi_XC_W2BYTEMSK(REG_SC_BK2A_2A_L, (MS_U16)0xF000, 0xF000); // default DFK  for china
        MApi_XC_W2BYTEMSK(REG_SC_BK2A_2A_L, (MS_U16)0x2000, 0xF000); // default DFK
    }

    // Favor MCDi-V when verticalMovingU
    if(true == verticalMovingU)
    {
        MApi_XC_W2BYTEMSK(REG_SC_BK2A_04_L, (MS_U16)0x0800, 0x0800); // Enable MCDi-V
        MApi_XC_W2BYTEMSK(REG_SC_BK2A_34_L, (MS_U16)0x0FF0, 0xFFFF); // favor MCDi Weight
        MApi_XC_W2BYTEMSK(REG_SC_BK2A_36_L, (MS_U16)0x2800, 0x3C00); // enable MCDi U for blending and history
        MApi_XC_W2BYTEMSK(REG_SC_BK2A_2E_L, (MS_U16)0x4000, 0xF000); // reduce motion history
    }
    else
    {
        MApi_XC_W2BYTEMSK(REG_SC_BK2A_04_L, (MS_U16)0x0000, 0x0800); // Disable MCDi-V
        MApi_XC_W2BYTEMSK(REG_SC_BK2A_34_L, (MS_U16)0x8888, 0xFFFF); // default MCDi Weight
        MApi_XC_W2BYTEMSK(REG_SC_BK2A_36_L, (MS_U16)0x0000, 0x3C00); // Use MCDi-H as default
        MApi_XC_W2BYTEMSK(REG_SC_BK2A_2E_L, (MS_U16)0x4000, 0xF000); // default MCDi motion history
    }

    // DFK EodiW1, 2
    if(s8DeBouncingCnt < 0x3 || filmDisableMCDiDFK)      //new add
    {
        MApi_XC_W2BYTEMSK(REG_SC_BK22_2C_L, (MS_U16)0x0080, 0x00F0); // reduce DFK EodiWeight
        MApi_XC_W2BYTEMSK(REG_SC_BK22_28_L, (MS_U16)0x4c30, 0xFF30); // disable EodiW for DFK
    }
    else if(true == verticalMovingU || verticalMoving2)
    {
        MApi_XC_W2BYTEMSK(REG_SC_BK22_2C_L, (MS_U16)0x0010, 0x00F0); // increase EodiW -> LG tower
        MApi_XC_W2BYTEMSK(REG_SC_BK22_28_L, (MS_U16)0x4c10, 0xFF30); // default
    }
    else if(false == verticalMovingU && slowMotion)
    {
        MApi_XC_W2BYTEMSK(REG_SC_BK22_2C_L, (MS_U16)0x0080, 0x00F0); // reduce DFK EodiWeight
        //MApi_XC_W2BYTEMSK(REG_SC_BK22_28_L, (MS_U16)0x2c30, 0xFF30); // disable EodiW for DFK
        MApi_XC_W2BYTEMSK(REG_SC_BK22_28_L, (MS_U16)0x0c30, 0xFF30); // disable EodiW for DFK
    }
    else if(MApi_XC_R2BYTEMSK(REG_SC_BK2A_1F_L, BIT(8)))  // RF/AV default, eodi weight strong
    {
        MApi_XC_W2BYTEMSK(REG_SC_BK22_2C_L, (MS_U16)0x0010, 0x00F0); // increase EodiW
        MApi_XC_W2BYTEMSK(REG_SC_BK22_28_L, (MS_U16)0x18F1, 0xFF30); // default
    }
    else // default
    {
        MApi_XC_W2BYTEMSK(REG_SC_BK22_2C_L, (MS_U16)0x0040, 0x00F0); // default DFK EodiWeight
        //MApi_XC_W2BYTEMSK(REG_SC_BK22_28_L, (MS_U16)0x4c10, 0xFF30); // default
        MApi_XC_W2BYTEMSK(REG_SC_BK22_28_L, (MS_U16)0x0c10, 0xFF30); // default
    }

    if(MApi_XC_R2BYTEMSK(REG_SC_BK2A_1F_L, BIT(8)))   // RF/AV default, eodi weight strong
        MApi_XC_W2BYTEMSK(REG_SC_BK22_2A_L, (MS_U16)0x4488, 0x00F0);
    else
        MApi_XC_W2BYTEMSK(REG_SC_BK22_2A_L, (MS_U16)0x0020, 0x00F0);

    // DFK check feathering gain
    if(stillFrameCnt > 0x8 || filmDisableMCDiDFK)    //new add
    {
        MApi_XC_W2BYTEMSK(REG_SC_BK2A_2A_L, (MS_U16)0x0100, 0x0F00);
    }
    else if(false == verticalMovingU && slowMotion)
    {
        MApi_XC_W2BYTEMSK(REG_SC_BK2A_2A_L, (MS_U16)0x0800, 0x0F00); // reduce DFK feathering weight
    }
    else if(u32Reg7B > 0x100)
    {
        MApi_XC_W2BYTEMSK(REG_SC_BK2A_2A_L, (MS_U16)0x0f00, 0x0F00); // feather -> increase
    }
    else
    {
        //MApi_XC_W2BYTEMSK(REG_SC_BK2A_2A_L, (MS_U16)0x0800, 0x0F00); // default   for china
        MApi_XC_W2BYTEMSK(REG_SC_BK2A_2A_L, (MS_U16)0x0f00, 0x0F00); // default
    }

    // EodiWeight for MCDi
    if(true == verticalMovingU)
    {
        //if ( u32Reg7A > 0x200 && u32Reg7A/4 > u32Reg79 ) // not-found >> horizontal : Church
        if(u32Reg7A > 0x100 && u32Reg7A / 2 > u32Reg79)  // not-found >> horizontal : Church
        {
            MApi_XC_W2BYTEMSK(REG_SC_BK22_2C_L, (MS_U16)0x0004, 0x000F); // reduce EodiWeight for MCDi
            MApi_XC_W2BYTEMSK(REG_SC_BK22_2A_L, (MS_U16)0x4800, 0xFF0F); // church
        }
        else // LG Tower
        {
            MApi_XC_W2BYTEMSK(REG_SC_BK22_2C_L, (MS_U16)0x0004, 0x000F); // increase EodiWeight for MCDi
            MApi_XC_W2BYTEMSK(REG_SC_BK22_2A_L, (MS_U16)0x4200, 0xFF0F); // LG Tower
        }
    }
    else if(false == verticalMovingU && horizontalMoving)
    {
        MApi_XC_W2BYTEMSK(REG_SC_BK22_2C_L, (MS_U16)0x0002, 0x000F); // reduce EodiWeight for MCDi
        MApi_XC_W2BYTEMSK(REG_SC_BK22_2A_L, (MS_U16)0x1800, 0xFF0F); // reduce EodiW for MCDi
    }
    else if(MApi_XC_R2BYTEMSK(REG_SC_BK2A_1F_L, BIT(8)))  // RF/AV default, eodi weight strong
    {
        MApi_XC_W2BYTEMSK(REG_SC_BK22_2C_L, (MS_U16)0x0002, 0x000F); // increase EodiWeight for MCDi
        MApi_XC_W2BYTEMSK(REG_SC_BK22_2A_L, (MS_U16)0x4408, 0xFF0F);
    }
    else if(eodiCnt > 25)
    {
        MApi_XC_W2BYTEMSK(REG_SC_BK22_2C_L, (MS_U16)0x0001, 0x000F); // default EodiWeight for MCDi
        MApi_XC_W2BYTEMSK(REG_SC_BK22_2A_L, (MS_U16)0x4400, 0xFF0F);
    }
    else if(eodiCnt < 10)
    {
        MApi_XC_W2BYTEMSK(REG_SC_BK22_2C_L, (MS_U16)0x0002, 0x000F); // default EodiWeight for MCDi
        MApi_XC_W2BYTEMSK(REG_SC_BK22_2A_L, (MS_U16)0x4800, 0xFF0F);
    }

    if(MApi_XC_R2BYTEMSK(REG_SC_BK2A_1F_L, BIT(8)))   // RF/AV default, eodi weight strong
        MApi_XC_W2BYTEMSK(REG_SC_BK22_29_L, (MS_U16)0xC830, 0xFFFF); // reduce EodiW on MCDi
    else
        MApi_XC_W2BYTEMSK(REG_SC_BK22_29_L, (MS_U16)0xC832, 0xFFFF); // reduce EodiW on MCDi

    // MCDi check Feathering weight
    if(true == verticalMovingU || filmDisableMCDiDFK)    //new add
    {
        MApi_XC_W2BYTEMSK(REG_SC_BK2A_2E_L, (MS_U16)0x0000, 0x00F0); // reduce MCDi feathering weight -> for church
    }
    else if(horizontalMoving)
    {
        MApi_XC_W2BYTEMSK(REG_SC_BK2A_2E_L, (MS_U16)0x0000, 0x00F0); // reduce MCDi feathering weight
    }
    else
    {
        MApi_XC_W2BYTEMSK(REG_SC_BK2A_2E_L, (MS_U16)0x0060, 0x00F0); // default MCDi Feathering Weight
    }
    // multi-burst skip
    if(u8SkipCnt > 0x20 || filmDisableMCDiDFK)
    {
        MApi_XC_W2BYTEMSK(REG_SC_BK2A_02_L, (MS_U16)0x0000, 0x0080); // disable mcdi
        //MApi_XC_W2BYTEMSK(REG_SC_BK2A_28_L, (MS_U16)0x0000, 0xFFFF); // disable DFK
        //MApi_XC_W2BYTEMSK(REG_SC_BK2A_2A_L, (MS_U16)0x0000, 0xF000); // disable DFK
    }
    else if(u8SkipCnt < 0x10)
    {
        MApi_XC_W2BYTEMSK(REG_SC_BK2A_02_L, (MS_U16)0x0080, 0x0080); // enable mcdi
        //MApi_XC_W2BYTEMSK(REG_SC_BK2A_28_L, (MS_U16)0xA8FF, 0xFFFF); // default DFK
        //MApi_XC_W2BYTEMSK(REG_SC_BK2A_2A_L, (MS_U16)0xF000, 0xF000); // default DFK
    }

    MApi_XC_W2BYTEMSK(REG_SC_BK2A_2B_L, (MS_U16)0xc000, 0xF000); //

    /////////////////////////////////////////////////////////////////
    //  De-bouncing
    /////////////////////////////////////////////////////////////////

    MS_U16 reg_debouncing_th, reg_debouncing_cnt;

    reg_debouncing_th = 0x0A;
    reg_debouncing_cnt = 0x03;

    if(u32MotionValue <= reg_debouncing_th)
    {
        s8DeBouncingCnt = 0;
    }
    else
    {
        if(s8DeBouncingCnt < 0xff)
            s8DeBouncingCnt++;
    }


    MS_BOOL film;
    film = ((MS_U16)MApi_XC_R2BYTEMSK(REG_SC_BK0A_21_L, 0x0008) == 0x0008) ||   // film22
           ((MS_U16)MApi_XC_R2BYTEMSK(REG_SC_BK0A_21_L, 0x0010) == 0x0010) ||   // film32
           ((MS_U16)MApi_XC_R2BYTEMSK(REG_SC_BK0A_21_L, 0x0040) == 0x0040) ;    // film any


    if((s8DeBouncingCnt >= reg_debouncing_cnt) || (true == film))
    {
        MApi_XC_W2BYTEMSK(REG_SC_BK2A_02_L, 0x000c, 0x000e);  // history weight = 6
    }
    else
    {
        MApi_XC_W2BYTEMSK(REG_SC_BK2A_02_L, 0x0004, 0x000e);  // history weight = 1
        //MApi_XC_W2BYTEMSK(REG_SC_BK2A_02_L, 0x000c, 0x000e);  // history weight = 1
    }

    /*
        MApi_XC_W2BYTEMSK(REG_SC_BK12_17_L, (MS_U16)0x02c0, 0xFFFF); // fetch no.
        MApi_XC_W2BYTEMSK(REG_SC_BK12_07_L, (MS_U16)0x2088, 0xFFFF); //
        MApi_XC_W2BYTEMSK(REG_SC_BK22_7A_L, (MS_U16)0x1000, 0xFFFF); //
        MApi_XC_W2BYTEMSK(REG_SC_BK05_12_L, (MS_U16)0x0000, 0x0001); // disable CCS
        MApi_XC_W2BYTEMSK(REG_SC_BK05_20_L, (MS_U16)0x0000, 0x0001); // disable DHD
    */
}

void MDrv_SC_mcdi_driver_new(MS_U32 u32MotionValue, MS_U8 u8MotLvl)
{
    MS_U32 u32Reg78, u32Reg79, u32Reg7A, u32Reg7B, u32Reg7C, u32Reg7D, u32Reg7E, filmFeatherCount;
    MS_U32 curCounter3, cntDiff;
    MS_U8 u8xxxCtrl;
    MS_BOOL verticalMovingU, horizontalMoving, slowMotion, featheringScene, featheringScene2, movingScene;
    static MS_U8 u8SkipCnt;
    static MS_U8 featheringFrameCount;
    static MS_U8 s8DeBouncingCnt;
    //static MS_U8 stillFrameCnt;

    static MS_U8 filmCnt = 0;
    static MS_U32 preCounter3 = 0;
    static MS_U8 eodiCnt = 0;
    static MS_U8 verticalMovingUCnt = 0;

    MS_BOOL skipCond;
    MS_BOOL verticalMoving2;

    MS_BOOL filmDisableMCDiDFK;

    u8xxxCtrl = 0xFF; //MApi_XC_R2BYTE(REG_SC_BK2A_1F_L) >> 8;

    u32Reg78 = MApi_XC_R2BYTE(REG_SC_BK2A_78_L); // isMV0
    u32Reg79 = MApi_XC_R2BYTE(REG_SC_BK2A_79_L); // Horzontal Moving
    u32Reg7A = MApi_XC_R2BYTE(REG_SC_BK2A_7A_L); // MV not found
    u32Reg7B = MApi_XC_R2BYTE(REG_SC_BK2A_7B_L); // Feathering
    u32Reg7C = MApi_XC_R2BYTE(REG_SC_BK2A_7C_L);
    u32Reg7D = MApi_XC_R2BYTE(REG_SC_BK2A_7D_L);
    u32Reg7E = MApi_XC_R2BYTE(REG_SC_BK2A_7E_L);
    filmFeatherCount = MApi_XC_R2BYTE(REG_SC_BK0A_0B_L);

    //printf("motion level = %x\n", u8MotLvl );
//    if(!MirrorEnable)
    if(MApi_XC_GetMirrorModeType() == MIRROR_NORMAL)
    {
        //Non_Mirror
        verticalMovingU = ((u32Reg7E > 0x0100) && (u32Reg7E / 8  > u32Reg7C) && (u32Reg7E / 8  > u32Reg7D) && (u32Reg78 > u32Reg79 / 8)) ||
                          ((u32Reg7E > 0x0040) && (u32Reg7E / 32 > u32Reg7C) && (u32Reg7E / 32 > u32Reg7D) && (u32Reg78 > u32Reg79 / 8));
        movingScene = (u32Reg7E > 0x0080) && (u32Reg7E / 4 > u32Reg7C) && (u32Reg7E / 4  > u32Reg7D) && // V-moving-up &&
                      (u32Reg79 > 0x0200) && (u32Reg79 / 4 > u32Reg78);                                                    // H-moving
    }
    else
    {
        //Mirror
        verticalMovingU = ((u32Reg7D > 0x0100) && (u32Reg7D / 8  > u32Reg7C) && (u32Reg7D / 8  > u32Reg7E) && (u32Reg78 > u32Reg79 / 8)) ||
                          ((u32Reg7D > 0x0040) && (u32Reg7D / 32 > u32Reg7C) && (u32Reg7D / 32 > u32Reg7E) && (u32Reg78 > u32Reg79 / 8));
        movingScene = (u32Reg7D > 0x0080) && (u32Reg7D / 4 > u32Reg7C) && (u32Reg7D / 4  > u32Reg7E) && // V-moving-up &&
                      (u32Reg79 > 0x0200) && (u32Reg79 / 4 > u32Reg78);                                                    // H-moving
    }

    verticalMoving2 = ((u32Reg7E > 0x200 && u32Reg7E > u32Reg7C) || (u32Reg7D > 0x200 && u32Reg7D > u32Reg7C)) && (u32Reg78 > u32Reg79 / 8);

    /*verticalMovingU = ( (u32Reg7E > 0x0100) && (u32Reg7E/8  > u32Reg7C) && (u32Reg7E/8  > u32Reg7D) && (u32Reg78 > u32Reg79) ) ||
                             ( (u32Reg7E > 0x0040) && (u32Reg7E/32 > u32Reg7C) && (u32Reg7E/32 > u32Reg7D) && (u32Reg78 > u32Reg79) );
        verticalMoving2 = ( ( u32Reg7E > 0x200 && u32Reg7E > u32Reg7C ) || ( u32Reg7D > 0x200 && u32Reg7D > u32Reg7C ) ) && (u32Reg78 > u32Reg79);
    *///ian

    //horizontalMoving = u32Reg79 > 0x200 && u32Reg7B < 0x40 && u32Reg79 > u32Reg78 && !verticalMoving2;
    horizontalMoving = u32Reg79 > 0x200 && u32Reg79 / 16 > u32Reg78 && u32Reg79 > u32Reg7A / 2 &&
                       u32Reg7C > 0x100 && u32Reg7C / 2  > u32Reg7D && u32Reg7C / 2 > u32Reg7E ;

    /*    horizontalMoving = u32Reg79 > 0x200 && u32Reg79/16 > u32Reg78 && u32Reg79/4 > u32Reg7A &&
                                    u32Reg7C > 0x100 && u32Reg7C/4  > u32Reg7D && u32Reg7C/4 > u32Reg7E ;
    *///ian

    slowMotion = u32Reg7A > 0x200 && u32Reg7B < 0x40 && !verticalMoving2;
    //featheringScene = (u32MotionValue > 0x0F) && (u32Reg7B > 0x800);
    featheringScene = u32Reg7B > 0x100 && u32Reg7B > u32Reg79 / 4 && u32Reg7B > u32Reg7A / 4 && (u32MotionValue > 0x10);
    skipCond = (u32Reg7A > 8 * u32Reg79) && (u32Reg7B > 0x600);
    featheringScene2 = u32Reg7B > 0x200 && u32Reg7B > u32Reg79 / 8 && u32Reg7B > u32Reg7A / 8 && !horizontalMoving && (u32MotionValue > 0x10);


#if 0
    if(true == verticalMovingU)
    {
        MApi_XC_W2BYTEMSK(REG_SC_BK22_7A_L, (MS_U16)0x1000, 0xFFFF); // show verticalMovingU
    }
    else
    {
        MApi_XC_W2BYTEMSK(REG_SC_BK22_7A_L, (MS_U16)0x0000, 0xFFFF); // show verticalMovingU
    }
#endif

    if(verticalMovingU)
    {
        if(verticalMovingUCnt != 0xff)
        {
            verticalMovingUCnt ++;
        }
    }
    else
    {
        verticalMovingUCnt = 0;
    }

    if(false == skipCond)
    {
        if(u8SkipCnt == 0)
            u8SkipCnt = 0;
        else
            u8SkipCnt--;
    }
    else
    {
        if(u8SkipCnt > 0x40)
            u8SkipCnt = 0x40;
        else
            u8SkipCnt++;
    }

    if(filmFeatherCount > 0x100)
    {
        if(featheringFrameCount < 0x80)
            featheringFrameCount ++;
    }
    else
    {
        featheringFrameCount = 0;
    }
    /*
        if ( u32MotionValue == 0 && u32Reg7B < 0x20  )
        {
        stillFrameCnt = 0x20;
        }
        else
        {
            stillFrameCnt = 0;
        }
    */
    ////////////////////////////////////////////////////////////////
    //  film content disable mcdi/dfk
    ////////////////////////////////////////////////////////////////

    curCounter3 = MApi_XC_R2BYTE(REG_SC_BK0A_0B_L);

    cntDiff = (preCounter3 > curCounter3) ? preCounter3 - curCounter3 : curCounter3 - preCounter3;

    if(cntDiff > 0x1500)
    {
        if(filmCnt != 10)
            filmCnt++;
    }
    else if(filmCnt != 0)
    {
        filmCnt--;
    }

    preCounter3 = curCounter3;
    //printf("filmCnt = [%x]\n", filmCnt);
    filmDisableMCDiDFK = (filmCnt > 5);

    ////////////////////////////////////////////////////////////////
    //  eodi counter
    ////////////////////////////////////////////////////////////////

    if(featheringScene2)
    {
        if(eodiCnt < 40)
            eodiCnt += 4;
    }
    else
    {
        if(eodiCnt > 0)
            eodiCnt--;
    }


    /////////////////////////////////////////////////////////////////
    //  SST (De-Feathering)
    /////////////////////////////////////////////////////////////////
    if(u8xxxCtrl & ENABLE_XXX_SST)
    {
        if(movingScene || filmDisableMCDiDFK)
        {
            MApi_XC_W2BYTEMSK(REG_SC_BK2A_29_L, (MS_U16)0xF100, 0xFF00); // increase SST weighting with checking feathering...
            MApi_XC_W2BYTEMSK(REG_SC_BK2A_2C_L, (MS_U16)0x0000, 0x0F00); // SST motion shift1
        }
        /*
        else if ( (u32Reg79 > 0x200 && u32Reg7B < 0x40) || (stillFrameCnt>=10) ) // patch sony temple, but champion bubble !?
        {
            MApi_XC_W2BYTEMSK(REG_SC_BK2A_29_L, (MS_U16)0x1100, 0xFF00); // default SST setting
            MApi_XC_W2BYTEMSK(REG_SC_BK2A_2C_L, (MS_U16)0x0200, 0x0F00); // SST motion shift1
        }
        */
        else if(u32MotionValue < 0x10 && u32Reg7B > 0x1000)    // special patch for Toshiba... -> tend to still
        {
            MApi_XC_W2BYTEMSK(REG_SC_BK2A_29_L, (MS_U16)0x3100, 0xFF00); // increase SST weighting with checking feathering...
            MApi_XC_W2BYTEMSK(REG_SC_BK2A_2C_L, (MS_U16)0x0200, 0x0F00); // SST motion shift1
        }
        else if(featheringScene)        // moving & feather
        {
            MApi_XC_W2BYTEMSK(REG_SC_BK2A_29_L, (MS_U16)0xF100, 0xFF00); // increase SST weighting with checking feathering...
            MApi_XC_W2BYTEMSK(REG_SC_BK2A_2C_L, (MS_U16)0x0000, 0x0F00); // SST motion shift1
        }
        else if(u8MotLvl >= 3 &&  u32Reg7B > 0x40 &&  u32Reg7B > u32Reg7A / 16)  // for sony champion bubbles
        {
            MApi_XC_W2BYTEMSK(REG_SC_BK2A_29_L, (MS_U16)0xF100, 0xFF00); // default SST setting
            MApi_XC_W2BYTEMSK(REG_SC_BK2A_2C_L, (MS_U16)0x0000, 0x0F00); // SST motion shift1
        }
        else // default
        {
            MApi_XC_W2BYTEMSK(REG_SC_BK2A_29_L, (MS_U16)0x2100, 0xFF00); // default SST setting
            MApi_XC_W2BYTEMSK(REG_SC_BK2A_2C_L, (MS_U16)0x0100, 0x0F00); // SST motion shift1
        }
    }

    // disable DFK when verticalMovingU
    if(true == verticalMovingU || u8SkipCnt > 0x20 || filmDisableMCDiDFK)
    {
        MApi_XC_W2BYTEMSK(REG_SC_BK2A_28_L, (MS_U16)0x0000, 0xFFFF); // disable DFK
        MApi_XC_W2BYTEMSK(REG_SC_BK2A_2A_L, (MS_U16)0x0000, 0xF000); // disable DFK
    }
    else if(verticalMoving2)
    {
        MApi_XC_W2BYTEMSK(REG_SC_BK2A_28_L, (MS_U16)0xA8FF, 0xFFFF); // default DFK
        MApi_XC_W2BYTEMSK(REG_SC_BK2A_2A_L, (MS_U16)0x0000, 0xF000); // default DFK
    }
    else
    {
        MApi_XC_W2BYTEMSK(REG_SC_BK2A_28_L, (MS_U16)0xA8FF, 0xFFFF); // default DFK
        //MApi_XC_W2BYTEMSK(REG_SC_BK2A_2A_L, (MS_U16)0xF000, 0xF000); // default DFK  for china
        MApi_XC_W2BYTEMSK(REG_SC_BK2A_2A_L, (MS_U16)0x2000, 0xF000); // default DFK
    }

    if(verticalMovingUCnt > 0x20)    // for church
    {
        MApi_XC_W2BYTEMSK(REG_SC_BK2A_34_L, (MS_U16)0x0FF0, 0xFFFF); // favor MCDi Weight
        MApi_XC_W2BYTEMSK(REG_SC_BK2A_36_L, (MS_U16)0x2800, 0x3C0F); // enable MCDi U for blending and history
    }
    else
    {
        MApi_XC_W2BYTEMSK(REG_SC_BK2A_34_L, (MS_U16)0x8888, 0xFFFF); // default MCDi Weight
        MApi_XC_W2BYTEMSK(REG_SC_BK2A_36_L, (MS_U16)0x0000, 0x3C0F); // Use MCDi-H as default
    }

    // Favor MCDi-V when verticalMovingU
    if(1)    //true == verticalMovingU )
    {
        MApi_XC_W2BYTEMSK(REG_SC_BK2A_04_L, (MS_U16)0x0800, 0x0800); // Enable MCDi-V
        MApi_XC_W2BYTEMSK(REG_SC_BK2A_2E_L, (MS_U16)0x4000, 0xF000); // reduce motion history
    }
    else
    {
        MApi_XC_W2BYTEMSK(REG_SC_BK2A_04_L, (MS_U16)0x0800, 0x0800); // Disable MCDi-V
        MApi_XC_W2BYTEMSK(REG_SC_BK2A_2E_L, (MS_U16)0x4000, 0xF000); // default MCDi motion history
    }

    // DFK EodiW1, 2
    if(s8DeBouncingCnt < 0x3 || filmDisableMCDiDFK)      //new add
    {
        MApi_XC_W2BYTEMSK(REG_SC_BK22_2C_L, (MS_U16)0x0080, 0x00F0); // reduce DFK EodiWeight
        MApi_XC_W2BYTEMSK(REG_SC_BK22_28_L, (MS_U16)0x4c30, 0xFF30); // disable EodiW for DFK
    }
    else if(true == verticalMovingU || verticalMoving2)
    {
        MApi_XC_W2BYTEMSK(REG_SC_BK22_2C_L, (MS_U16)0x0010, 0x00F0); // increase EodiW -> LG tower
        MApi_XC_W2BYTEMSK(REG_SC_BK22_28_L, (MS_U16)0x4c10, 0xFF30); // default
    }
    else if(false == verticalMovingU && slowMotion)
    {
        MApi_XC_W2BYTEMSK(REG_SC_BK22_2C_L, (MS_U16)0x0080, 0x00F0); // reduce DFK EodiWeight
        //MApi_XC_W2BYTEMSK(REG_SC_BK22_28_L, (MS_U16)0x2c30, 0xFF30); // disable EodiW for DFK
        MApi_XC_W2BYTEMSK(REG_SC_BK22_28_L, (MS_U16)0x0c30, 0xFF30); // disable EodiW for DFK
    }
    else if(MApi_XC_R2BYTEMSK(REG_SC_BK2A_1F_L, BIT(8)))  // RF/AV default, eodi weight strong
    {
        MApi_XC_W2BYTEMSK(REG_SC_BK22_2C_L, (MS_U16)0x0010, 0x00F0); // increase EodiW
        MApi_XC_W2BYTEMSK(REG_SC_BK22_28_L, (MS_U16)0x18F1, 0xFF30); // default
    }
    else // default
    {
        MApi_XC_W2BYTEMSK(REG_SC_BK22_2C_L, (MS_U16)0x0040, 0x00F0); // default DFK EodiWeight
        //MApi_XC_W2BYTEMSK(REG_SC_BK22_28_L, (MS_U16)0x4c10, 0xFF30); // default
        MApi_XC_W2BYTEMSK(REG_SC_BK22_28_L, (MS_U16)0x0c10, 0xFF30); // default
    }

    if(MApi_XC_R2BYTEMSK(REG_SC_BK2A_1F_L, BIT(8)))   // RF/AV default, eodi weight strong
        MApi_XC_W2BYTEMSK(REG_SC_BK22_2A_L, (MS_U16)0x4488, 0x00F0);
    else
        MApi_XC_W2BYTEMSK(REG_SC_BK22_2A_L, (MS_U16)0x0020, 0x00F0);

    // DFK check feathering gain
    if(filmDisableMCDiDFK)     //new add
    {
        MApi_XC_W2BYTEMSK(REG_SC_BK2A_2A_L, (MS_U16)0x0f00, 0x0F00);
        MApi_XC_W2BYTEMSK(REG_SC_BK2A_2B_L, (MS_U16)0xf000, 0xF000); //
    }
    else if(u32MotionValue == 0)
    {
        MApi_XC_W2BYTEMSK(REG_SC_BK2A_2A_L, (MS_U16)0x0800, 0x0F00); // reduce DFK feathering weight
        MApi_XC_W2BYTEMSK(REG_SC_BK2A_2B_L, (MS_U16)0x1000, 0xF000); //
    }
    else if(false == verticalMovingU && slowMotion)
    {
        MApi_XC_W2BYTEMSK(REG_SC_BK2A_2A_L, (MS_U16)0x0800, 0x0F00); // reduce DFK feathering weight
        MApi_XC_W2BYTEMSK(REG_SC_BK2A_2B_L, (MS_U16)0x1000, 0xF000); //
    }
    else if(u32Reg7B > 0x100)
    {
        MApi_XC_W2BYTEMSK(REG_SC_BK2A_2A_L, (MS_U16)0x0f00, 0x0F00); // feather -> increase
        MApi_XC_W2BYTEMSK(REG_SC_BK2A_2B_L, (MS_U16)0xc000, 0xF000); //
    }
    else
    {
        //MApi_XC_W2BYTEMSK(REG_SC_BK2A_2A_L, (MS_U16)0x0800, 0x0F00); // default   for china
        MApi_XC_W2BYTEMSK(REG_SC_BK2A_2A_L, (MS_U16)0x0f00, 0x0F00); // default
        MApi_XC_W2BYTEMSK(REG_SC_BK2A_2B_L, (MS_U16)0x4000, 0xF000); //
    }

    // EodiWeight for MCDi
    if(true == verticalMovingU)
    {
        //if ( u32Reg7A > 0x200 && u32Reg7A/4 > u32Reg79 ) // not-found >> horizontal : Church
        if(u32Reg7A > 0x100 && u32Reg7A / 2 > u32Reg79)  // not-found >> horizontal : Church
        {
            MApi_XC_W2BYTEMSK(REG_SC_BK22_2C_L, (MS_U16)0x0004, 0x000F); // reduce EodiWeight for MCDi
            MApi_XC_W2BYTEMSK(REG_SC_BK22_2A_L, (MS_U16)0x4800, 0xFF0F); // church
        }
        else // LG Tower
        {
            MApi_XC_W2BYTEMSK(REG_SC_BK22_2C_L, (MS_U16)0x0004, 0x000F); // increase EodiWeight for MCDi
            MApi_XC_W2BYTEMSK(REG_SC_BK22_2A_L, (MS_U16)0x4200, 0xFF0F); // LG Tower
        }
    }
    else if(false == verticalMovingU && horizontalMoving)
    {
        MApi_XC_W2BYTEMSK(REG_SC_BK22_2C_L, (MS_U16)0x0002, 0x000F); // reduce EodiWeight for MCDi
        MApi_XC_W2BYTEMSK(REG_SC_BK22_2A_L, (MS_U16)0x1800, 0xFF0F); // reduce EodiW for MCDi
    }
    else if(MApi_XC_R2BYTEMSK(REG_SC_BK2A_1F_L, BIT(8)))  // RF/AV default, eodi weight strong
    {
        MApi_XC_W2BYTEMSK(REG_SC_BK22_2C_L, (MS_U16)0x0002, 0x000F); // increase EodiWeight for MCDi
        MApi_XC_W2BYTEMSK(REG_SC_BK22_2A_L, (MS_U16)0x4408, 0xFF0F);
    }
    else if(eodiCnt > 25)
    {
        MApi_XC_W2BYTEMSK(REG_SC_BK22_2C_L, (MS_U16)0x0001, 0x000F); // default EodiWeight for MCDi
        MApi_XC_W2BYTEMSK(REG_SC_BK22_2A_L, (MS_U16)0x4400, 0xFF0F);
    }
    else if(eodiCnt < 10)
    {
        MApi_XC_W2BYTEMSK(REG_SC_BK22_2C_L, (MS_U16)0x0002, 0x000F); // default EodiWeight for MCDi
        MApi_XC_W2BYTEMSK(REG_SC_BK22_2A_L, (MS_U16)0x4800, 0xFF0F);
    }

    if(MApi_XC_R2BYTEMSK(REG_SC_BK2A_1F_L, BIT(8)) && (!horizontalMoving)) // RF/AV default, eodi weight strong
        MApi_XC_W2BYTEMSK(REG_SC_BK22_29_L, (MS_U16)0xC830, 0xFFFF); // reduce EodiW on MCDi
    else if(!horizontalMoving)
        MApi_XC_W2BYTEMSK(REG_SC_BK22_29_L, (MS_U16)0xC832, 0xFFFF); // reduce EodiW on MCDi
    else
        MApi_XC_W2BYTEMSK(REG_SC_BK22_29_L, (MS_U16)0xC836, 0xFFFF);

    // MCDi check Feathering weight
    if(filmDisableMCDiDFK)
    {
        MApi_XC_W2BYTEMSK(REG_SC_BK2A_2E_L, (MS_U16)0x0060, 0x00F0); // reduce MCDi feathering weight -> for church
    }
    else if(true == verticalMovingU)     //new add
    {
        MApi_XC_W2BYTEMSK(REG_SC_BK2A_2E_L, (MS_U16)0x0000, 0x00F0); // reduce MCDi feathering weight -> for church
    }
    else if(horizontalMoving)
    {
        MApi_XC_W2BYTEMSK(REG_SC_BK2A_2E_L, (MS_U16)0x0000, 0x00F0); // reduce MCDi feathering weight
    }
    else
    {
        MApi_XC_W2BYTEMSK(REG_SC_BK2A_2E_L, (MS_U16)0x0060, 0x00F0); // default MCDi Feathering Weight
    }
    // multi-burst skip
    if(u8SkipCnt > 0x20 || filmDisableMCDiDFK)
    {
        MApi_XC_W2BYTEMSK(REG_SC_BK2A_02_L, (MS_U16)0x0000, 0x0080); // disable mcdi
        //MApi_XC_W2BYTEMSK(REG_SC_BK2A_28_L, (MS_U16)0x0000, 0xFFFF); // disable DFK
        //MApi_XC_W2BYTEMSK(REG_SC_BK2A_2A_L, (MS_U16)0x0000, 0xF000); // disable DFK
    }
    else if(u8SkipCnt < 0x10)
    {
        MApi_XC_W2BYTEMSK(REG_SC_BK2A_02_L, (MS_U16)0x0080, 0x0080); // enable mcdi
        //MApi_XC_W2BYTEMSK(REG_SC_BK2A_28_L, (MS_U16)0xA8FF, 0xFFFF); // default DFK
        //MApi_XC_W2BYTEMSK(REG_SC_BK2A_2A_L, (MS_U16)0xF000, 0xF000); // default DFK
    }

    //MApi_XC_W2BYTEMSK(REG_SC_BK2A_2B_L, (MS_U16)0xc000, 0xF000); //
    if(u32MotionValue == 0)    // still picture -> tend to still
    {
        MApi_XC_W2BYTEMSK(REG_SC_BK2A_2B_L, (MS_U16)0x0800, 0x0FFF); //
        MApi_XC_W2BYTEMSK(REG_SC_BK2A_2C_L, (MS_U16)0x0000, 0x00FF); //
    }
    else // default
    {
        MApi_XC_W2BYTEMSK(REG_SC_BK2A_2B_L, (MS_U16)0x0888, 0x0FFF); //
        MApi_XC_W2BYTEMSK(REG_SC_BK2A_2C_L, (MS_U16)0x0088, 0x00FF); //
    }

    /////////////////////////////////////////////////////////////////
    //  De-bouncing
    /////////////////////////////////////////////////////////////////
#if 0
    MS_U16 reg_debouncing_th, reg_debouncing_cnt;

    reg_debouncing_th = 0x0A;
    reg_debouncing_cnt = 0x03;

    if(u32MotionValue <= reg_debouncing_th)
    {
        s8DeBouncingCnt = 0;
    }
    else
    {
        if(s8DeBouncingCnt < 0xff)
            s8DeBouncingCnt++;
    }

    MS_BOOL film;
    film = ((MS_U16)MApi_XC_R2BYTEMSK(REG_SC_BK0A_21_L, 0x0008) == 0x0008) ||   // film22
           ((MS_U16)MApi_XC_R2BYTEMSK(REG_SC_BK0A_21_L, 0x0010) == 0x0010) ||   // film32
           ((MS_U16)MApi_XC_R2BYTEMSK(REG_SC_BK0A_21_L, 0x0040) == 0x0040) ;    // film any


    if((s8DeBouncingCnt >= reg_debouncing_cnt) || (true == film))
    {
        MApi_XC_W2BYTEMSK(REG_SC_BK2A_02_L, 0x000c, 0x000e);  // history weight = 6
    }
    else
    {
        MApi_XC_W2BYTEMSK(REG_SC_BK2A_02_L, 0x0004, 0x000e);  // history weight = 2
        //MApi_XC_W2BYTEMSK(REG_SC_BK2A_02_L, 0x000c, 0x000e);  // history weight = 1
    }
#endif

    MS_BOOL film;
    film = ((MS_U16)MApi_XC_R2BYTEMSK(REG_SC_BK0A_21_L, 0x0008) == 0x0008) ||   // film22
           ((MS_U16)MApi_XC_R2BYTEMSK(REG_SC_BK0A_21_L, 0x0010) == 0x0010) ||   // film32
           ((MS_U16)MApi_XC_R2BYTEMSK(REG_SC_BK0A_21_L, 0x0040) == 0x0040) ;    // film any


    if((u8MotLvl >= 1) || (true == film))
    {
        MApi_XC_W2BYTEMSK(REG_SC_BK2A_02_L, 0x000c, 0x000e);  // history weight = 6
    }
    else
    {
        MApi_XC_W2BYTEMSK(REG_SC_BK2A_02_L, 0x0004, 0x000e);  // history weight = 2
        //MApi_XC_W2BYTEMSK(REG_SC_BK2A_02_L, 0x000c, 0x000e);  // history weight = 1
    }
    /*
        MApi_XC_W2BYTEMSK(REG_SC_BK12_17_L, (MS_U16)0x02c0, 0xFFFF); // fetch no.
        MApi_XC_W2BYTEMSK(REG_SC_BK12_07_L, (MS_U16)0x2088, 0xFFFF); //
        MApi_XC_W2BYTEMSK(REG_SC_BK22_7A_L, (MS_U16)0x1000, 0xFFFF); //
        MApi_XC_W2BYTEMSK(REG_SC_BK05_12_L, (MS_U16)0x0000, 0x0001); // disable CCS
        MApi_XC_W2BYTEMSK(REG_SC_BK05_20_L, (MS_U16)0x0000, 0x0001); // disable DHD
    */
}

#define MF_Gain            30
MS_U32 MDrv_SC_T12_UC_SmallMoving(void)
{
    MS_U32 before_MC_feather;
    MS_U32 nonMV0_Comfirm, MV_Contradict;
    MS_U32 MovingFeather, MV_Confirm;
    static MS_U32 PrevMV_Confirm = 0;

    before_MC_feather = MApi_XC_R2BYTE(REG_SC_BK2A_77_L) ;
    nonMV0_Comfirm = MApi_XC_R2BYTE(REG_SC_BK2A_79_L);
    MV_Contradict = MApi_XC_R2BYTE(REG_SC_BK2A_7A_L);


    MV_Confirm = (nonMV0_Comfirm + MV_Contradict) >> 1;

    MV_Confirm = (PrevMV_Confirm * 3 + MV_Confirm) >> 2;
    PrevMV_Confirm = MV_Confirm;

    if(MV_Confirm > (before_MC_feather * MF_Gain))
        MovingFeather = MV_Confirm - ((before_MC_feather * MF_Gain)) ;
    else
        MovingFeather = 0;

    return MovingFeather;  // "Big": in seach range "Small": out of seach range
}

#define OOSRF_Gain            4
#define OOSRF_Power         2
MS_U32 MDrv_SC_T12_UC_InSearchRange_Feather(void)
{
    MS_U32 before_MC_feather, after_MC_feather;
    MS_U32 MC_OOSRF;
    static MS_U32 Prev_before_MC_feather = 0;
    static MS_U32 Prev_after_MC_feather = 0;

    before_MC_feather = MApi_XC_R2BYTE(REG_SC_BK2A_77_L) ;
    after_MC_feather = MApi_XC_R2BYTE(REG_SC_BK2A_7B_L);

    before_MC_feather = (Prev_before_MC_feather * 3 + before_MC_feather) >> 2;
    Prev_before_MC_feather = before_MC_feather;

    after_MC_feather = (Prev_after_MC_feather * 3 + after_MC_feather) >> 2;
    Prev_after_MC_feather = after_MC_feather;

    if(before_MC_feather > (after_MC_feather * OOSRF_Gain) >> OOSRF_Power)
        MC_OOSRF = before_MC_feather - ((after_MC_feather * OOSRF_Gain) >> OOSRF_Power) ;
    else
        MC_OOSRF = 0;

    return MC_OOSRF;  // "Big": in seach range "Small": out of seach range
}


#define OOSRMC_Gain         20
MS_U32 MDrv_SC_T12_UC_InSearchRange_MvComfirm(void)
{
    MS_U32 MV0_Comfirm,  nonMV0_Comfirm, MV_Contradict;
    static MS_U32 Prev_MV0_Comfirm = 0;
    static MS_U32 Prev_nonMV0_Comfirm = 0;
    static MS_U32 Prev_MV_Contradict = 0;
    MS_U32 MV_Comfirm, MC_OOSRMC;

    MV0_Comfirm = MApi_XC_R2BYTE(REG_SC_BK2A_78_L);
    nonMV0_Comfirm = MApi_XC_R2BYTE(REG_SC_BK2A_79_L);
    MV_Contradict = MApi_XC_R2BYTE(REG_SC_BK2A_7A_L);

    MV0_Comfirm = (Prev_MV0_Comfirm * 3 + MV0_Comfirm) >> 2;
    Prev_MV0_Comfirm = MV0_Comfirm;

    nonMV0_Comfirm = (Prev_nonMV0_Comfirm * 3 + nonMV0_Comfirm) >> 2;
    Prev_nonMV0_Comfirm = nonMV0_Comfirm;

    MV_Contradict = (Prev_MV_Contradict * 3 + MV_Contradict) >> 2;
    Prev_MV_Contradict = MV_Contradict;

    MV_Comfirm = MV0_Comfirm + nonMV0_Comfirm;

    if(MV_Comfirm > MV_Contradict * OOSRMC_Gain)
        MC_OOSRMC = MV_Comfirm - MV_Contradict * OOSRMC_Gain;
    else
        MC_OOSRMC = 0;

    return MC_OOSRMC;  // "Big": in seach range "Small": out of seach range

}

#define ISR_NONMV0_Gain         15
MS_U32 MDrv_SC_T12_UC_InSearchRange_nonMv0Comfirm(void)
{
    MS_U32 MV0_Comfirm,  nonMV0_Comfirm, MV_Contradict;
    static MS_U32 Prev_MV0_Comfirm = 0;
    static MS_U32 Prev_nonMV0_Comfirm = 0;
    static MS_U32 Prev_MV_Contradict = 0;
    MS_U32 MV_Comfirm, Comfirm_Value;

    MV0_Comfirm = MApi_XC_R2BYTE(REG_SC_BK2A_78_L);
    nonMV0_Comfirm = MApi_XC_R2BYTE(REG_SC_BK2A_79_L);
    MV_Contradict = MApi_XC_R2BYTE(REG_SC_BK2A_7A_L);

    MV0_Comfirm = (Prev_MV0_Comfirm * 3 + MV0_Comfirm) >> 2;
    Prev_MV0_Comfirm = MV0_Comfirm;

    nonMV0_Comfirm = (Prev_nonMV0_Comfirm * 3 + nonMV0_Comfirm) >> 2;
    Prev_nonMV0_Comfirm = nonMV0_Comfirm;

    MV_Contradict = (Prev_MV_Contradict * 3 + MV_Contradict) >> 2;
    Prev_MV_Contradict = MV_Contradict;

    MV_Comfirm = /*MV0_Comfim +*/nonMV0_Comfirm;

    if(MV_Comfirm > MV_Contradict * ISR_NONMV0_Gain)
        Comfirm_Value = MV_Comfirm - MV_Contradict * ISR_NONMV0_Gain;
    else
        Comfirm_Value = 0;

    return Comfirm_Value;  // "Big": in seach range "Small": out of seach range

}

void MDrv_SC_T12_DIPF_TemporalGainControl(MS_U32 OOSRMC, MS_U32 OOSRF, MS_U32 ISR_NONMV0)
{
    static MS_U16 TemporalGainControl = 0x0F;

    //printf("====OOSRMC=%x===\n",OOSRMC);
    //printf("====OOSRF=%x===\n",OOSRF);
    UNUSED(OOSRMC);
    UNUSED(OOSRF);

    if(ISR_NONMV0 > 1000)  //&& OOSRF > 50)
    {
        if(TemporalGainControl > 0x00)
            TemporalGainControl--;
        else
            TemporalGainControl = 0;
    }
    else
    {
        if(TemporalGainControl < 0x0F)
            TemporalGainControl++;
        else
            TemporalGainControl = 0x0F;
    }
    MApi_XC_W2BYTEMSK(REG_SC_BK22_14_L, (MS_U16)TemporalGainControl, 0x000F);

}

#define ComplexOffset_MaxValue 0x06
#define ComplexOffset_MinValue 0x00
void MDrv_SC_T12_UCDi_ComplexOffset(MS_U32 OOSRMC, MS_U32 OOSRF, MS_U32 ISR_NONMV0)
{
    static MS_U16 ComplexOffset = ComplexOffset_MaxValue;

    //printf("====OOSRMC=%x===\n",OOSRMC);
    //printf("====OOSRF=%x===\n",OOSRF);
    UNUSED(OOSRMC);
    UNUSED(OOSRF);

    if(ISR_NONMV0 > 1000)  //&& OOSRF > 50)
    {
        if(ComplexOffset > ComplexOffset_MinValue)
            ComplexOffset--;
        else
            ComplexOffset = ComplexOffset_MinValue;
    }
    else
    {
        if(ComplexOffset < ComplexOffset_MaxValue)
            ComplexOffset++;
        else
            ComplexOffset = ComplexOffset_MaxValue;
    }
    MApi_XC_W2BYTEMSK(REG_SC_BK2A_2E_L, (MS_U16)ComplexOffset << 4, 0x00F0);

}

#define OP1_HisWeight_MaxValue 0x0C
#define OP1_HisWeight_MinValue 0x04
void MDrv_SC_T12_UCDi_OP1_HisWeight(MS_U8 u8MotLvl)
{
    MS_BOOL film;

    film = ((MS_U16)MApi_XC_R2BYTEMSK(REG_SC_BK0A_21_L, 0x0008) == 0x0008) ||   // film22
           ((MS_U16)MApi_XC_R2BYTEMSK(REG_SC_BK0A_21_L, 0x0010) == 0x0010) ||   // film32
           ((MS_U16)MApi_XC_R2BYTEMSK(REG_SC_BK0A_21_L, 0x0040) == 0x0040) ;    // film any

    if((u8MotLvl >= 1) || (true == film))
    {
        MApi_XC_W2BYTEMSK(REG_SC_BK2A_02_L, OP1_HisWeight_MaxValue, 0x000e);  // history weight = 6
    }
    else
    {
        MApi_XC_W2BYTEMSK(REG_SC_BK2A_02_L, OP1_HisWeight_MinValue, 0x000e);  // history weight = 2
    }
}

#define A5_HIS_MOTION_TH 0x0A
void MDrv_SC_A5_UCDi_OP1_HisWeight(const MS_U32 u32MotionValue)
{
    //MS_BOOL film;
    static MS_U32 reg_his_weight = OP1_HisWeight_MinValue;

    //MS_U8 motion_level;
    static MS_U32 u32MotionValue_prev = 0;

    if( u32MotionValue_prev < u32MotionValue )
        u32MotionValue_prev = ((u32MotionValue_prev*3 )+ u32MotionValue ) >> 2;
    else
        u32MotionValue_prev = u32MotionValue;

    //printf("===u32MotionValue_prev==%x=\n",u32MotionValue_prev);
    //printf("===u16_unmatch_3==%x=\n",u16_unmatch_3);

    if( u32MotionValue_prev < A5_HIS_MOTION_TH )
    {
        if( reg_his_weight > OP1_HisWeight_MinValue )
            reg_his_weight--;
        else
            reg_his_weight = OP1_HisWeight_MinValue;
    }
    else
    {
        if( reg_his_weight < OP1_HisWeight_MaxValue )
            reg_his_weight++;
        else
            reg_his_weight = OP1_HisWeight_MaxValue;
    }


    MApi_XC_W2BYTEMSK(REG_SC_BK2A_02_L, reg_his_weight, 0x000E);

}

#define FeatherValueGain_MaxValue 0x02
#define FeatherValueGain_MinValue 0x00
void MDrv_SC_T12_UCDi_FeatherValueGain(MS_U32 OOSRMC, MS_U32 OOSRF, MS_U32 ISR_NONMV0)
{
    static MS_U16 FeatherValue = FeatherValueGain_MaxValue;

    // printf("====OOSRMC=%x===\n",OOSRMC);
    //  printf("====OOSRF=%x===\n",OOSRF);
    UNUSED(OOSRMC);
    UNUSED(OOSRF);

    if(ISR_NONMV0 > 1000)  //&& OOSRF > 50)
    {
        if(FeatherValue > FeatherValueGain_MinValue)
            FeatherValue--;
        else
            FeatherValue = FeatherValueGain_MinValue;
    }
    else
    {
        if(FeatherValue < FeatherValueGain_MaxValue)
            FeatherValue++;
        else
            FeatherValue = FeatherValueGain_MaxValue;
    }
    MApi_XC_W2BYTEMSK(REG_SC_BK2A_2D_L, (MS_U16)FeatherValue << 12, 0xF000);

}

#define W2SadGain_MaxValue 0x0C
#define W2SadGain_MinValue 0x08
void MDrv_SC_T12_UCDi_W2SadGain(MS_U32 OOSRMC, MS_U32 OOSRF, MS_U32 ISR_NONMV0)
{
    static MS_U16 W2SadGain = FeatherValueGain_MaxValue;

    // printf("====OOSRMC=%x===\n",OOSRMC);
    //  printf("====OOSRF=%x===\n",OOSRF);
    UNUSED(OOSRMC);
    UNUSED(OOSRF);

    if(ISR_NONMV0 > 1000)  //&& OOSRF > 50)
    {
        if(W2SadGain < W2SadGain_MaxValue)
            W2SadGain++;
        else
            W2SadGain = W2SadGain_MaxValue;
    }
    else
    {
        if(W2SadGain > W2SadGain_MinValue)
            W2SadGain--;
        else
            W2SadGain = W2SadGain_MinValue;
    }
    MApi_XC_W2BYTEMSK(REG_SC_BK2A_2D_L, (MS_U16)W2SadGain << 8, 0x0F00);

}

#define VerticalMovingUdRatio_MaxValue 0x0F
#define VerticalMovingUdRatio_MinValue 0x07
void MDrv_SC_T12_UCDi_VerticalMovingUdRatio(MS_U32 OOSRMC, MS_U32 OOSRF, MS_U32 ISR_NONMV0)
{
    static MS_U16 VerticalMovingUdRatioValue = VerticalMovingUdRatio_MaxValue;

    // printf("====OOSRMC=%x===\n",OOSRMC);
    //  printf("====OOSRF=%x===\n",OOSRF);
    UNUSED(OOSRMC);
    UNUSED(ISR_NONMV0);

    //if( ISR_NONMV0 > 1000)
    if( OOSRF > 0x300 )
    {
        if(VerticalMovingUdRatioValue > VerticalMovingUdRatio_MinValue)
            VerticalMovingUdRatioValue--;
        else
            VerticalMovingUdRatioValue = VerticalMovingUdRatio_MinValue;
    }
    else
    {
        if(VerticalMovingUdRatioValue < VerticalMovingUdRatio_MaxValue)
            VerticalMovingUdRatioValue++;
        else
            VerticalMovingUdRatioValue = VerticalMovingUdRatio_MaxValue;
    }
    MApi_XC_W2BYTEMSK(REG_SC_BK2A_28_L, (MS_U16)VerticalMovingUdRatioValue, 0x000F);

}

#define KFC_8bitMotionGain_MaxValue 0x08
#define KFC_8bitMotionGain_MinValue 0x04 //0 have feather issue.
void MDrv_SC_T12_KFC_8bitMotionGain(MS_U32 SmallMoving)
{
    static MS_U16 KFC_8bitMotionGainValue = KFC_8bitMotionGain_MaxValue;

    //printf("====SmallMoving=%x===\n",SmallMoving);
    //MApi_XC_W2BYTEMSK(REG_SC_BK20_1D_L, 0x0008, 0x003F);

    if(SmallMoving > 500)
    {
        //MApi_XC_W2BYTEMSK(REG_SC_BK22_7A_L, 0x1000, 0x1000);
        if(KFC_8bitMotionGainValue > KFC_8bitMotionGain_MinValue)
            KFC_8bitMotionGainValue--;
        else
            KFC_8bitMotionGainValue = KFC_8bitMotionGain_MinValue;
    }
    else
    {
        //MApi_XC_W2BYTEMSK(REG_SC_BK22_7A_L, 0x0000, 0x1000);
        if(KFC_8bitMotionGainValue < KFC_8bitMotionGain_MaxValue)
            KFC_8bitMotionGainValue++;
        else
            KFC_8bitMotionGainValue = KFC_8bitMotionGain_MaxValue;
    }
    MApi_XC_W2BYTEMSK(REG_SC_BK2A_3B_L, (MS_U16)KFC_8bitMotionGainValue << 4, 0x00F0);

}

#define UCDi_EODiWGain_MaxValue 0x0F
#define UCDi_EODiWGain_MinValue 0x00
void MDrv_SC_T12_UCDi_EODiWGain(MS_U32 OOSRMC, MS_U32 OOSRF, MS_U32 ISR_NONMV0)
{
    static MS_U16 EODiWGainValue = UCDi_EODiWGain_MaxValue;

    // printf("====OOSRMC=%x===\n",OOSRMC);
    //  printf("====OOSRF=%x===\n",OOSRF);
    UNUSED(OOSRMC);

    if(ISR_NONMV0 > 1000 && OOSRF > 50)
    {
        if(EODiWGainValue > UCDi_EODiWGain_MinValue)
            EODiWGainValue--;
        else
            EODiWGainValue = UCDi_EODiWGain_MinValue;
    }
    else
    {
        if(EODiWGainValue < UCDi_EODiWGain_MaxValue)
            EODiWGainValue++;
        else
            EODiWGainValue = UCDi_EODiWGain_MaxValue;
    }
    MApi_XC_W2BYTEMSK(REG_SC_BK22_2A_L, (MS_U16)EODiWGainValue << 12, 0xF000);

}

#define UCDi_HistoryRatio_MaxValue 0x07
#define UCDi_HistoryRatio_MinValue 0x04
void MDrv_SC_T12_UCDi_HistoryRatio(MS_U32 OOSRMC, MS_U32 OOSRF, MS_U32 ISR_NONMV0)
{
    static MS_U16 HistoryRatioValue = UCDi_HistoryRatio_MaxValue;

    // printf("====OOSRMC=%x===\n",OOSRMC);
    //  printf("====OOSRF=%x===\n",OOSRF);
    // MApi_XC_W2BYTEMSK(REG_SC_BK20_1D_L, 0x0008, 0x00FF);
    UNUSED(OOSRMC);

    if(ISR_NONMV0 > 1000 && OOSRF > 50)
    {
        //MApi_XC_W2BYTEMSK(REG_SC_BK22_7A_L, 0x1000, 0x1000);
        if(HistoryRatioValue > UCDi_HistoryRatio_MinValue)
            HistoryRatioValue--;
        else
            HistoryRatioValue = UCDi_HistoryRatio_MinValue;
    }
    else
    {
        //MApi_XC_W2BYTEMSK(REG_SC_BK22_7A_L, 0x0000, 0x1000);
        if(HistoryRatioValue < UCDi_HistoryRatio_MaxValue)
            HistoryRatioValue++;
        else
            HistoryRatioValue = UCDi_HistoryRatio_MaxValue;
    }
    MApi_XC_W2BYTEMSK(REG_SC_BK2A_2E_L, (MS_U16)HistoryRatioValue << 12, 0xF000);

}

#define KFC_EODIW_GAIN_MAX 4
#define KFC_EODIW_GAIN_MIN 0
void MDrv_SC_A5_KFC_EODiW(const MS_U32 IsVerticalMoving)
{
    MS_U32 Statistics_cplx_pixel;
    //MS_U32 meVerticalMoving;
    static MS_U32 Statistics_cplx_pixel_iir = KFC_EODIW_GAIN_MIN;

    static MS_U32 reg_kfc_eodiw_gain = KFC_EODIW_GAIN_MAX ;

    Statistics_cplx_pixel = ((Statistics_cplx_pixel_iir*3 )+ MApi_XC_R2BYTE(REG_SC_BK2A_7F_L) ) >> 2;
    Statistics_cplx_pixel_iir = Statistics_cplx_pixel;

    if( (IsVerticalMoving < 0x150) && (Statistics_cplx_pixel_iir > 0x1000) )
    {
        if( reg_kfc_eodiw_gain > KFC_EODIW_GAIN_MIN )
            reg_kfc_eodiw_gain--;
        else
            reg_kfc_eodiw_gain  = KFC_EODIW_GAIN_MIN;
        //reg_kfc_eodiw_gain = MAX(KFC_EODIW_GAIN_MIN, reg_kfc_eodiw_gain - 1);
    }
    else
    {
        if( reg_kfc_eodiw_gain < KFC_EODIW_GAIN_MAX )
            reg_kfc_eodiw_gain++;
        else
            reg_kfc_eodiw_gain = KFC_EODIW_GAIN_MAX;
        //reg_kfc_eodiw_gain = MIN(KFC_EODIW_GAIN_MAX, reg_kfc_eodiw_gain + 1);
    }

     MApi_XC_W2BYTEMSK(REG_SC_BK22_28_L, (MS_U16)reg_kfc_eodiw_gain << 12, 0xF000);
}

#define LG107_FEATHER_GAIN_MAX 2
#define LG107_FEATHER_GAIN_MIN 0
#define LG107_CPLX_OFFSET_MAX 6
#define LG107_CPLX_OFFSET_MIN 0
#define LG107_CPLX_GAIN_MAX 15
#define LG107_CPLX_GAIN_MIN 4
void MDrv_SC_A5_LG107(const MS_U32 IsVerticalMoving, const MS_U32 IsHorizontalMoving, const MS_U32 total_feather, const MS_U32 mvComfirm, const MS_U32 colorSum,const MS_U32 u32MotionValue, const MS_U32 uComplex )
{
  static MS_U32 reg_mcdi_feather_gain = LG107_FEATHER_GAIN_MAX ;
  static MS_U32 reg_mcdi_cplx_offset = LG107_CPLX_OFFSET_MAX ;
  static MS_U32 reg_mcdi_cplx_gain = LG107_CPLX_GAIN_MIN ;

  //printf("====IsVerticalMoving=%x===\n",IsVerticalMoving);
  //printf("====IsHorizontalMoving=%x===\n",IsHorizontalMoving);
  //printf("====total_feather=%x===\n",total_feather);
  //printf("====mvComfirm=%x===\n",mvComfirm);
  //printf("====colorSum=%x===\n",colorSum);
 // printf("====u32MotionValue=%x===\n",u32MotionValue);

  if( (IsVerticalMoving < 20) && (IsHorizontalMoving < 200) && (total_feather < 100) && (mvComfirm > 200) && (colorSum > 200) && (u32MotionValue > 0x4000) && (uComplex < 0x700))
    {

        if( reg_mcdi_feather_gain > LG107_FEATHER_GAIN_MIN )
            reg_mcdi_feather_gain--;
        else
            reg_mcdi_feather_gain = LG107_FEATHER_GAIN_MIN;

        if( reg_mcdi_cplx_offset > LG107_CPLX_OFFSET_MIN )
            reg_mcdi_cplx_offset--;
        else
            reg_mcdi_cplx_offset = LG107_CPLX_OFFSET_MIN;

        if( reg_mcdi_cplx_gain < LG107_CPLX_GAIN_MAX )
            reg_mcdi_cplx_gain++;
        else
            reg_mcdi_cplx_gain = LG107_CPLX_GAIN_MAX;
    }
  else
    {
        if( reg_mcdi_feather_gain < LG107_FEATHER_GAIN_MAX )
            reg_mcdi_feather_gain++;
        else
            reg_mcdi_feather_gain = LG107_FEATHER_GAIN_MAX;

        if( reg_mcdi_cplx_offset < LG107_CPLX_OFFSET_MAX )
            reg_mcdi_cplx_offset++;
        else
            reg_mcdi_cplx_offset = LG107_CPLX_OFFSET_MAX;

        if( reg_mcdi_cplx_gain > LG107_CPLX_GAIN_MIN )
            reg_mcdi_cplx_gain--;
        else
            reg_mcdi_cplx_gain = LG107_CPLX_GAIN_MIN;
    }

  MApi_XC_W2BYTEMSK(REG_SC_BK2A_2D_L, (MS_U16)reg_mcdi_feather_gain<<12, 0xF000); // 2 to 0
  MApi_XC_W2BYTEMSK(REG_SC_BK2A_2E_L, (MS_U16)reg_mcdi_cplx_offset<<4, 0x00F0);// 6 to 0
  MApi_XC_W2BYTEMSK(REG_SC_BK2A_2E_L, (MS_U16)reg_mcdi_cplx_gain<<8, 0x0F00);// 4 to F

}

MS_U32 MDrv_SC_A5_ComplexSum(void)
{
   static MS_U32 IsComplex_iir = 0xFFFF;
   MS_U32 IsComplex;

   IsComplex = MApi_XC_R2BYTE(REG_SC_BK2A_7F_L);

   IsComplex = (IsComplex_iir*3 + IsComplex ) >> 2;
   IsComplex_iir = IsComplex;

   return IsComplex_iir;
}

MS_U32 MDrv_SC_A5_KFC_IsVerticalMoving(void)
{
    MS_U32 u32_mvc, u32_mvu, u32_mvd;
    MS_U32 me_vertical;
    static MS_U32 IsVerticalMoving_iir = 0;

    u32_mvc     = MApi_XC_R2BYTE(REG_SC_BK2A_7C_L);
    u32_mvu     = MApi_XC_R2BYTE(REG_SC_BK2A_7D_L);
    u32_mvd     = MApi_XC_R2BYTE(REG_SC_BK2A_7E_L);


    if ((u32_mvu > u32_mvc) && (u32_mvu > u32_mvd) /*&& (u32_mvu > 0x200*/)
    {
        if( u32_mvu > (u32_mvc+u32_mvd)/2 )
            me_vertical = (u32_mvu - (u32_mvc+u32_mvd)/2);
        else
            me_vertical = 0;

        //me_vertical = MAX(0, u32_mvu - (u32_mvc+u32_mvd)/2);
    }
    else if((u32_mvd > u32_mvc) && (u32_mvd > u32_mvu) /*&& (u32_mvd > 0x200)*/)
    {
        if( u32_mvd > (u32_mvc+u32_mvu)/2 )
            me_vertical = (u32_mvd - (u32_mvc+u32_mvu)/2);
        else
            me_vertical = 0;

        //me_vertical = MAX(0, u32_mvd - (u32_mvc+u32_mvu)/2);
    }
    else
    {
        me_vertical = 0;
    }


    if( me_vertical > 255 )
        me_vertical = 255;

    me_vertical = (IsVerticalMoving_iir*3 + me_vertical ) >> 2;
    IsVerticalMoving_iir = me_vertical;

    return me_vertical;
}

MS_U32 MDrv_SC_A5_IsHorizontalMoving(void)
{
    MS_U32 u32_mvc, u32_mvu, u32_mvd;
    MS_U32 me_horizontal;
    static MS_U32 IsHorizontalMoving_iir = 0;

    u32_mvc     = MApi_XC_R2BYTE(REG_SC_BK2A_7C_L);
    u32_mvu     = MApi_XC_R2BYTE(REG_SC_BK2A_7D_L);
    u32_mvd     = MApi_XC_R2BYTE(REG_SC_BK2A_7E_L);


    if ((u32_mvc > u32_mvu) && (u32_mvc > u32_mvd) /*&& (u32_mvu > 0x200*/)
    {
        if( u32_mvc > (u32_mvu+u32_mvd)/2 )
            me_horizontal = (u32_mvc - (u32_mvu+u32_mvd)/2);
        else
            me_horizontal = 0;
    }
    else
    {
        me_horizontal = 0;
    }


    if( me_horizontal > 255 )
        me_horizontal = 255;

    me_horizontal = (IsHorizontalMoving_iir*3 + me_horizontal ) >> 2;
    IsHorizontalMoving_iir = me_horizontal;

    return me_horizontal;
}


MS_U32 MDrv_SC_A5_ColorPixelSum(void)
{
   MS_U32 colorSum;
   static MS_U32 colorSum_iir = 0;

   colorSum      = MApi_XC_R2BYTE(REG_SC_BK1A_6E_L);

   colorSum  = colorSum / 8;

   if( colorSum > 255 )
    colorSum = 255;

   colorSum = ((colorSum_iir*3 )+ colorSum ) >> 2;
   colorSum_iir = colorSum;

   return colorSum_iir;
}


MS_U32 MDrv_SC_A5_TotalFeather(void)
{
   MS_U32 mv0_feather, nonMv0_feather;
   MS_U32 total_feather;
   static MS_U32 total_feather_iir = 255;

   mv0_feather      = MApi_XC_R2BYTE(REG_SC_BK2A_77_L);
   nonMv0_feather   = MApi_XC_R2BYTE(REG_SC_BK2A_7B_L);

   total_feather = ( mv0_feather + nonMv0_feather ) / 4;

   if( total_feather > 255 )
    total_feather = 255;

   total_feather = ((total_feather_iir*3 )+ total_feather ) >> 2;
   total_feather_iir = total_feather;

   return total_feather_iir;
}

#define TOTAL_MV_TIME 10
MS_U32 MDrv_SC_A5_TotalMvComfirm(void)
{
  MS_U32 mv0, nonMv0, notFound;
  MS_U32 total_mv, mvComfirm;
  static MS_U32 mvComfirm_iir = 0;

  mv0      = MApi_XC_R2BYTE(REG_SC_BK2A_78_L);
  nonMv0   = MApi_XC_R2BYTE(REG_SC_BK2A_79_L);
  notFound   = MApi_XC_R2BYTE(REG_SC_BK2A_7A_L);

  total_mv = mv0 + nonMv0;

  if( total_mv > notFound*TOTAL_MV_TIME )
     mvComfirm =   total_mv - notFound*TOTAL_MV_TIME;
  else
     mvComfirm = 0;

  if( mvComfirm > 255 )
    mvComfirm = 255;

   mvComfirm = ((mvComfirm_iir*3 )+ mvComfirm ) >> 2;
   mvComfirm_iir = mvComfirm;

   return mvComfirm_iir;
}
/******************************************************************************/
///MCNR Driver
/******************************************************************************/

void MDrv_SC_mcnr_driver(void)
{
    MS_U32 u32_is_mv0, u32_non_mv0, u32_mvNotFound, u32_mvc, u32_mvu, u32_mvd;
    MS_BOOL me_not_confirm , me_vertical/*, scene_change*/;
    static MS_U8 u8notConfirmCnt;
    //static MS_U32 last_u32_mvNotFound;
    //static MS_U8 u8SceneChangeFrame, u8LastNRStrength;


    u32_is_mv0  = MApi_XC_R2BYTE(REG_SC_BK2A_78_L);
    u32_non_mv0 = MApi_XC_R2BYTE(REG_SC_BK2A_79_L);
    u32_mvNotFound = MApi_XC_R2BYTE(REG_SC_BK2A_7A_L);
    u32_mvc     = MApi_XC_R2BYTE(REG_SC_BK2A_7C_L);
    u32_mvu     = MApi_XC_R2BYTE(REG_SC_BK2A_7D_L);
    u32_mvd     = MApi_XC_R2BYTE(REG_SC_BK2A_7E_L);


    /////////////////////////////////////////////////////////////////
    //  NR Patch 1
    /////////////////////////////////////////////////////////////////

    me_not_confirm = (u32_is_mv0 < 0x0200) && (u32_non_mv0 < 0x0200) ;
    me_vertical    = ((u32_mvu > u32_mvc) && (u32_mvu > u32_mvd) && (u32_mvu > 0x200)) ||
                     ((u32_mvd > u32_mvc) && (u32_mvd > u32_mvu) && (u32_mvd > 0x200));
    /*
        // Check Scene Change, reduce NR
        scene_change = u32_mvNotFound > 0x200 && ( u32_mvNotFound/4 > last_u32_mvNotFound );
        if ( scene_change )
        {
            u8SceneChangeFrame = 0x20;
        }
        else if ( u8SceneChangeFrame > 0 )
        {
            u8SceneChangeFrame --;
        }

        if ( u8SceneChangeFrame < 10 )
        {
            if ( u8LastNRStrength < 24 )
                u8LastNRStrength ++;
        }
        else
        {
            if ( u8LastNRStrength > 0 )
                u8LastNRStrength --;
        }
        //u8LastNRStrength range 0 ~ 24
        MApi_XC_W2BYTEMSK(REG_SC_BK2A_18_L, (MS_U16)(u8LastNRStrength<<6), 0x0F00);
     */

    if(false == me_not_confirm)
    {
        if(u8notConfirmCnt > 0)
        {
            u8notConfirmCnt--;
        }
    }
    else
    {
        if(u8notConfirmCnt <= 0x40)
        {
            u8notConfirmCnt++;
        }
    }


    if(1)
    {
        if((u8notConfirmCnt > 0x30) || me_vertical)    // low nr strength
        {
            MApi_XC_W2BYTEMSK(REG_SC_BK2A_08_L, (MS_U16)0x7777, 0xFFFF);
            MApi_XC_W2BYTEMSK(REG_SC_BK2A_09_L, (MS_U16)0x7777, 0xFFFF);
            MApi_XC_W2BYTEMSK(REG_SC_BK2A_0A_L, (MS_U16)0x7777, 0xFFFF);
            MApi_XC_W2BYTEMSK(REG_SC_BK2A_0B_L, (MS_U16)0x7777, 0xFFFF);
            MApi_XC_W2BYTEMSK(REG_SC_BK2A_0C_L, (MS_U16)0x6655, 0xFFFF);
            MApi_XC_W2BYTEMSK(REG_SC_BK2A_0D_L, (MS_U16)0x4433, 0xFFFF);
            MApi_XC_W2BYTEMSK(REG_SC_BK2A_0E_L, (MS_U16)0x3322, 0xFFFF);
            MApi_XC_W2BYTEMSK(REG_SC_BK2A_0F_L, (MS_U16)0x1100, 0xFFFF);
            MApi_XC_W2BYTEMSK(REG_SC_BK2A_1B_L, (MS_U16)0x0064, 0x00FF); // moving picture, increase random motion
        }
        else if(u8notConfirmCnt < 0x10)
        {

            MApi_XC_W2BYTEMSK(REG_SC_BK2A_08_L, (MS_U16)0xdddd, 0xFFFF);
            MApi_XC_W2BYTEMSK(REG_SC_BK2A_09_L, (MS_U16)0xdddd, 0xFFFF);
            MApi_XC_W2BYTEMSK(REG_SC_BK2A_0A_L, (MS_U16)0xdddd, 0xFFFF);
            MApi_XC_W2BYTEMSK(REG_SC_BK2A_0B_L, (MS_U16)0xdddd, 0xFFFF);
            MApi_XC_W2BYTEMSK(REG_SC_BK2A_0C_L, (MS_U16)0xdddd, 0xFFFF);
            MApi_XC_W2BYTEMSK(REG_SC_BK2A_0D_L, (MS_U16)0xBA98, 0xFFFF);
            MApi_XC_W2BYTEMSK(REG_SC_BK2A_0E_L, (MS_U16)0x7654, 0xFFFF);
            MApi_XC_W2BYTEMSK(REG_SC_BK2A_0F_L, (MS_U16)0x3210, 0xFFFF);
#if 0
            MApi_XC_W2BYTEMSK(REG_SC_BK2A_08_L, (MS_U16)0xdddd, 0xFFFF);
            MApi_XC_W2BYTEMSK(REG_SC_BK2A_09_L, (MS_U16)0xdddd, 0xFFFF);
            MApi_XC_W2BYTEMSK(REG_SC_BK2A_0A_L, (MS_U16)0xdddd, 0xFFFF);
            MApi_XC_W2BYTEMSK(REG_SC_BK2A_0B_L, (MS_U16)0xdddd, 0xFFFF);
            MApi_XC_W2BYTEMSK(REG_SC_BK2A_0C_L, (MS_U16)0xdddd, 0xFFFF);
            MApi_XC_W2BYTEMSK(REG_SC_BK2A_0D_L, (MS_U16)0xdddd, 0xFFFF);
            MApi_XC_W2BYTEMSK(REG_SC_BK2A_0E_L, (MS_U16)0xdddd, 0xFFFF);
            MApi_XC_W2BYTEMSK(REG_SC_BK2A_0F_L, (MS_U16)0xddd0, 0xFFFF);
#endif
        }

        if(u32_is_mv0 > 0x200 && u32_is_mv0 / 2 > u32_non_mv0 && u32_is_mv0 / 2 > u32_mvNotFound)
        {
            MApi_XC_W2BYTEMSK(REG_SC_BK2A_1B_L, (MS_U16)0x006c, 0x00FF); // still picture, reduce random motion
        }
        else if(u32_non_mv0 > u32_is_mv0)
        {
            MApi_XC_W2BYTEMSK(REG_SC_BK2A_1B_L, (MS_U16)0x0060, 0x00FF); // moving picture, increase random motion
        }
        else
        {
            MApi_XC_W2BYTEMSK(REG_SC_BK2A_1B_L, (MS_U16)0x0064, 0x00FF); // default random motion
        }
    }

//last_u32_mvNotFound = u32_mvNotFound;
}

/******************************************************************************/
///DeFeathering
///@param u32MotionValue \     IN: Motion value
/******************************************************************************/
void MDrv_SC_de_feathering(MS_U32 u32MotionValue)
{
    static MS_U32 u32DeFeatherCntLv1 = 0;
    static MS_U32 u32DeFeatherCntLv2 = 0;
    static MS_U32 u32DeFeatherCntLv3 = 0;

    MS_U8 u8SST_Static_Core_TH = 0;

    MS_U32 reg_defethering_lv1_cnt = 0;
    MS_U32 reg_defethering_lv2_cnt = 0;
    MS_U32 reg_mdp_cnt = 0;

    reg_defethering_lv1_cnt = 0x0A;
    reg_defethering_lv2_cnt = 0x08;
    reg_mdp_cnt = 0x01;

    // motion level count
    if(u32MotionValue >= DEFETHERING_LV1_TH)
    {
        if(u32DeFeatherCntLv1 < reg_defethering_lv1_cnt)
            u32DeFeatherCntLv1++;
    }
    else
    {
        if(u32DeFeatherCntLv1 >= reg_mdp_cnt)
            u32DeFeatherCntLv1 = u32DeFeatherCntLv1 - reg_mdp_cnt;
        //if(reg_mdp_cnt == 0xFF)
        //u32DeFeatherCntLv1 = 0;
    }

    if(u32MotionValue >= DEFETHERING_LV2_TH)
    {
        if(u32DeFeatherCntLv2 < reg_defethering_lv2_cnt)
            u32DeFeatherCntLv2++;
    }
    else
    {
        if(u32DeFeatherCntLv2 >= reg_mdp_cnt)
            u32DeFeatherCntLv2 = u32DeFeatherCntLv2 - reg_mdp_cnt;
        //if(reg_mdp_cnt == 0xFF)
        //u32DeFeatherCntLv2 = 0;
    }

    if(u32MotionValue >= DEFETHERING_LV3_TH)
    {
        if(u32DeFeatherCntLv3 < DEFETHERING_LV3_CNT)
            u32DeFeatherCntLv3++;
    }
    else
    {
        if(u32DeFeatherCntLv3 >= reg_mdp_cnt)
            u32DeFeatherCntLv3 = u32DeFeatherCntLv3 - reg_mdp_cnt;
        //if(reg_mdp_cnt == 0xFF)
        //u32DeFeatherCntLv3 = 0;
    }

    //DeFeathering begin
    if(u32DeFeatherCntLv1 >= reg_defethering_lv1_cnt)
    {
        u8SST_Static_Core_TH    = SST_STATIC_CORE_TH_LV1_VALUE;
    }
    else if(u32DeFeatherCntLv2 >= reg_defethering_lv2_cnt)
    {
        u8SST_Static_Core_TH    = SST_STATIC_CORE_TH_LV2_VALUE;
    }
    else if(u32DeFeatherCntLv3 >= DEFETHERING_LV3_CNT)
    {
        u8SST_Static_Core_TH    = SST_STATIC_CORE_TH_LV3_VALUE;
    }
    else
    {
        u8SST_Static_Core_TH    = SST_STATIC_CORE_TH_LV4_VALUE;
    }

    MApi_XC_W2BYTEMSK(REG_SC_BK22_1A_L, (MS_U16)u8SST_Static_Core_TH, 0xFF);
}


int MDrv_SC_motion_level(MS_U32 u32MotionValue)
{
    static MS_U32 u32DeFeatherCntLv1 = 0;
    static MS_U32 u32DeFeatherCntLv2 = 0;
    static MS_U32 u32DeFeatherCntLv3 = 0;

    MS_U8 u8MotLvl;

    MS_U32 reg_defethering_lv1_cnt;
    MS_U32 reg_defethering_lv2_cnt;
    MS_U32 reg_mdp_cnt;

    reg_defethering_lv1_cnt = 0x0A;
    reg_defethering_lv2_cnt = 0x08;
    reg_mdp_cnt = 0x01;

    // motion level count
    if(u32MotionValue >= DEFETHERING_LV1_TH)
    {
        if(u32DeFeatherCntLv1 < reg_defethering_lv1_cnt)
            u32DeFeatherCntLv1++;
    }
    else
    {
        if(u32DeFeatherCntLv1 >= reg_mdp_cnt)
            u32DeFeatherCntLv1 = u32DeFeatherCntLv1 - reg_mdp_cnt;
        //if(reg_mdp_cnt == 0xFF)
        //u32DeFeatherCntLv1 = 0;
    }

    if(u32MotionValue >= DEFETHERING_LV2_TH)
    {
        if(u32DeFeatherCntLv2 < reg_defethering_lv2_cnt)
            u32DeFeatherCntLv2++;
    }
    else
    {
        if(u32DeFeatherCntLv2 >= reg_mdp_cnt)
            u32DeFeatherCntLv2 = u32DeFeatherCntLv2 - reg_mdp_cnt;
        //if(reg_mdp_cnt == 0xFF)
        //u32DeFeatherCntLv2 = 0;
    }

    if(u32MotionValue >= DEFETHERING_LV3_TH)
    {
        if(u32DeFeatherCntLv3 < DEFETHERING_LV3_CNT)
            u32DeFeatherCntLv3++;
    }
    else
    {
        if(u32DeFeatherCntLv3 >= reg_mdp_cnt)
            u32DeFeatherCntLv3 = u32DeFeatherCntLv3 - reg_mdp_cnt;
        //if(reg_mdp_cnt == 0xFF)
        //u32DeFeatherCntLv3 = 0;
    }

    //level begin
    if(u32DeFeatherCntLv1 >= reg_defethering_lv1_cnt)
    {
        u8MotLvl    = 3;
    }
    else if(u32DeFeatherCntLv2 >= reg_defethering_lv2_cnt)
    {
        u8MotLvl    = 2;
    }
    else if(u32DeFeatherCntLv3 >= DEFETHERING_LV3_CNT)
    {
        u8MotLvl    = 1;
    }
    else
    {
        u8MotLvl    = 0;
    }

    return u8MotLvl;

}


/******************************************************************************/
///DeFlickering
///@param u32MotionValue \     IN: Motion value
/******************************************************************************/
void MDrv_SC_de_flickering(MS_U32 u32MotionValue)
{
    static MS_S32 s32DeFlickerCnt = 0;

    MS_U32 reg_m_feat_smooth_hle_th = 0;
    MS_U32 reg_m_feat_smooth_shrink = 0;

    reg_m_feat_smooth_hle_th = (MS_U32)MApi_XC_R2BYTEMSK(REG_SC_BK22_1E_L, 0xF000); // Feat Smooth HLE TH
    reg_m_feat_smooth_shrink = (MS_U32)MApi_XC_R2BYTEMSK(REG_SC_BK22_1E_L, 0x8F);

    if(u32MotionValue >= DEFLICKERING_TH)
    {
        if(s32DeFlickerCnt < 65535)
            s32DeFlickerCnt++;
    }
    else
    {
        s32DeFlickerCnt = 0;
    }

    if(s32DeFlickerCnt >= DEFLICKERING_CNT)
    {
        reg_m_feat_smooth_hle_th += 0x0300;
        reg_m_feat_smooth_shrink += 0x10;
    }
    else
    {
        reg_m_feat_smooth_hle_th += 0x0700;
        reg_m_feat_smooth_shrink += 0x30;
    }

    MApi_XC_W2BYTEMSK(REG_SC_BK22_1E_L, reg_m_feat_smooth_shrink, 0xFF);
    MApi_XC_W2BYTEMSK(REG_SC_BK22_1E_L, reg_m_feat_smooth_hle_th, 0xFF00);

}


/******************************************************************************/
///DeBouncing
///@param u32MotionValue \     IN: Motion value
/******************************************************************************/
#define DEBOUNCING_GAIN 1 //0
void MDrv_SC_de_bouncing(MS_U32 u32MotionValue)
{
    static MS_S32 s32DeBouncingCnt = 0;
    MS_U32 reg_his_wt_f2 = 0;
    MS_U32 reg_debouncing_th = 0;
    int reg_debouncing_cnt = 0;

    reg_debouncing_th = 0x0A;
    reg_debouncing_cnt = 0x03;

    reg_his_wt_f2 = (MS_U32)MApi_XC_R2BYTEMSK(REG_SC_BK22_0A_L, 0xF8); // history ratio weighting

    if(u32MotionValue <= reg_debouncing_th * DEBOUNCING_GAIN)
    {
        s32DeBouncingCnt = 0;
    }
    else
    {
        if(s32DeBouncingCnt < 65535)
            s32DeBouncingCnt++;
    }

    if(s32DeBouncingCnt >= reg_debouncing_cnt)
    {
        reg_his_wt_f2 += 0x07; // history = 7 moving
    }
    else
    {
        reg_his_wt_f2 += 0x02; // history = 2 still
    }
    MApi_XC_W2BYTEMSK(REG_SC_BK22_0A_L, reg_his_wt_f2, 0xFF);
}

/******************************************************************************/
///DePreSNR
///@param u32MotionValue \     IN: Motion value
/******************************************************************************/
void MDrv_SC_de_pre_snr(MS_U32 u32MotionValue)
{
    static MS_S32 s32DePreSNRCnt = 0;

    MS_U8  reg_preSnr_en = 0;
    MS_U32 reg_preSnr_th;
    int reg_preSnr_cnt;

    reg_preSnr_th = 0x06;
    reg_preSnr_cnt = 0x01;

    if(u32MotionValue <= reg_preSnr_th * DEBOUNCING_GAIN)
    {
        s32DePreSNRCnt = 0;
    }
    else
    {
        if(s32DePreSNRCnt < 65535)
            s32DePreSNRCnt++;
    }

    if(s32DePreSNRCnt >= reg_preSnr_cnt)
    {
        reg_preSnr_en = 0x01; // enable pre-snr
    }
    else
    {
        reg_preSnr_en = 0x00; // disable pre-snr
    }
    MApi_XC_W2BYTEMSK(REG_SC_BK06_22_L, (MS_U16)reg_preSnr_en, BIT(0));
}

/******************************************************************************/
///Dynamic SNR
///@param u32MotionValue \     IN: Motion value
/******************************************************************************/
void MDrv_SC_dynamic_snr(MS_U32 u32MotionValue)
{
    static MS_S32 s32DynamicSnrCnt = 0;
#if (DBG_DYNAMIC_SNR)
    static MS_S32 s32CurrentLevel = -1;
    static MS_S32 s32LastLevel = -1;
#endif

    MS_U8  u8SNR_Bypass;
    MS_U32 reg_snr_cnt;

    reg_snr_cnt = (MS_U32)MApi_XC_R2BYTEMSK(REG_SC_BK0B_77_L, 0xFF);

    if(u32MotionValue <= DYNAMIC_SNR_TH)
    {
        s32DynamicSnrCnt = 0;
    }
    else
    {
        if(s32DynamicSnrCnt < 65535)
            s32DynamicSnrCnt++;
    }

    if(s32DynamicSnrCnt >= (MS_S32)reg_snr_cnt)
    {
        u8SNR_Bypass = 0; // SNR enable

#if (DBG_DYNAMIC_SNR)
        s32CurrentLevel = 1;
#endif
    }
    else
    {
        u8SNR_Bypass = 1; // SNR disable

#if (DBG_DYNAMIC_SNR)
        s32CurrentLevel = 2;
#endif
    }
    MApi_XC_W2BYTEMSK(REG_SC_BK0C_70_L, (MS_U16)u8SNR_Bypass, 0xFF);

#if (DBG_DYNAMIC_SNR)
    if(s32CurrentLevel != s32LastLevel)
    {
        if(s32CurrentLevel == 1)
            printf("SNR = Enable[%x]\n", s32CurrentLevel);
        else
            printf("SNR = Disable[%x]\n", s32CurrentLevel);

        s32LastLevel = s32CurrentLevel;
    }
#endif
}

/******************************************************************************/
///FantasticDNR
///@param u32MotionValue \     IN: Motion value
/******************************************************************************/
void MDrv_SC_fantastic_dnr(void)
{
    static MS_S32 s32Cnt  = 0;
    MS_U8 u8DNR_TblY_0L, u8DNR_TblY_0H, u8DNR_TblY_1L, u8DNR_TblY_1H;
    MS_U8 u8DNR_TblY_2L, u8DNR_TblY_2H, u8DNR_TblY_3L, u8DNR_TblY_3H;

    s32Cnt++;

    if(s32Cnt % 2 == 0)
    {
        u8DNR_TblY_0L = 0xDE;
        u8DNR_TblY_0H = 0xDD;
        u8DNR_TblY_1L = 0x79;
        u8DNR_TblY_1H = 0x67;
        u8DNR_TblY_2L = 0x56;
        u8DNR_TblY_2H = 0x45;
        u8DNR_TblY_3L = 0x11;
        u8DNR_TblY_3H = 0x00;
    }
    else
    {
        u8DNR_TblY_0L = 0xCD;
        u8DNR_TblY_0H = 0x39;
        u8DNR_TblY_1L = 0x34;
        u8DNR_TblY_1H = 0x11;
        u8DNR_TblY_2L = 0x00;
        u8DNR_TblY_2H = 0x00;
        u8DNR_TblY_3L = 0x00;
        u8DNR_TblY_3H = 0x00;
    }
    MApi_XC_W2BYTE(REG_SC_BK06_40_L, ((MS_U16)u8DNR_TblY_0L | (((MS_U16) u8DNR_TblY_0H) << 8)));
    MApi_XC_W2BYTE(REG_SC_BK06_41_L, ((MS_U16)u8DNR_TblY_1L | (((MS_U16) u8DNR_TblY_1H) << 8)));
    MApi_XC_W2BYTE(REG_SC_BK06_42_L, ((MS_U16)u8DNR_TblY_2L | (((MS_U16) u8DNR_TblY_2H) << 8)));
    MApi_XC_W2BYTE(REG_SC_BK06_43_L, ((MS_U16)u8DNR_TblY_3L | (((MS_U16) u8DNR_TblY_3H) << 8)));

}

/******************************************************************************/
///Dynamic DNR
///@param u32MotionValue \     IN: Motion value
/******************************************************************************/
#define DYNAMIC_DNR_TH                  1000
void MDrv_SC_dynamic_dnr(MS_U32 u32MotionValue)
{
    static MS_S32 s32ZeroCnt  = 0;
    static MS_S32 s32StillCnt = 0;
    MS_U8 u8DNR_TblY_0L, u8DNR_TblY_0H, u8DNR_TblY_1L, u8DNR_TblY_1H;
    MS_U8 u8DNR_TblY_2L, u8DNR_TblY_2H, u8DNR_TblY_3L, u8DNR_TblY_3H;

    if(u32MotionValue <= DYNAMIC_DNR_TH)
    {
        if(s32StillCnt < 65535)
            s32StillCnt++;
    }
    else
    {
        s32StillCnt = 0;
    }

    if(u32MotionValue == 0)
    {
        if(s32ZeroCnt < 65535)
            s32ZeroCnt++;
    }
    else
    {
        s32ZeroCnt = 0;
    }

    if(s32ZeroCnt >= 18)
    {
        u8DNR_TblY_0L = 0xDE;
        u8DNR_TblY_0H = 0xDD;
        u8DNR_TblY_1L = 0x79;
        u8DNR_TblY_1H = 0x67;
        u8DNR_TblY_2L = 0x56;
        u8DNR_TblY_2H = 0x45;
        u8DNR_TblY_3L = 0x11;
        u8DNR_TblY_3H = 0x00;
    }
    else if(s32StillCnt >= 8)  //still
    {
        u8DNR_TblY_0L = 0xCE;
        u8DNR_TblY_0H = 0xAC;
        u8DNR_TblY_1L = 0x78;
        u8DNR_TblY_1H = 0x34;
        u8DNR_TblY_2L = 0x12;
        u8DNR_TblY_2H = 0x00;
        u8DNR_TblY_3L = 0x00;
        u8DNR_TblY_3H = 0x00;

    }
    else //moving
    {
        u8DNR_TblY_0L = 0xCD;
        u8DNR_TblY_0H = 0x39;
        u8DNR_TblY_1L = 0x34;
        u8DNR_TblY_1H = 0x11;
        u8DNR_TblY_2L = 0x00;
        u8DNR_TblY_2H = 0x00;
        u8DNR_TblY_3L = 0x00;
        u8DNR_TblY_3H = 0x00;
    }
    MApi_XC_W2BYTE(REG_SC_BK06_40_L, ((MS_U16)u8DNR_TblY_0L | (((MS_U16) u8DNR_TblY_0H) << 8)));
    MApi_XC_W2BYTE(REG_SC_BK06_41_L, ((MS_U16)u8DNR_TblY_1L | (((MS_U16) u8DNR_TblY_1H) << 8)));
    MApi_XC_W2BYTE(REG_SC_BK06_42_L, ((MS_U16)u8DNR_TblY_2L | (((MS_U16) u8DNR_TblY_2H) << 8)));
    MApi_XC_W2BYTE(REG_SC_BK06_43_L, ((MS_U16)u8DNR_TblY_3L | (((MS_U16) u8DNR_TblY_3H) << 8)));

}

void MDrv_SC_dynamic_film_mode(void)
{
    MS_U8 u8Value;
    MS_U16 u16_unmatch_3;
    static MS_U8 u8PrevLuma, u8_unmatch_3_Cnt;
    XC_ApiStatus stXCStatus;

    if(MApi_XC_GetStatus(&stXCStatus, MAIN_WINDOW) == FALSE)
    {
        DRVSCA_DBG(printf("MApi_XC_GetStatus failed because of InitData wrong, please update header file and compile again\n"));
    }

    if(stXCStatus.bInterlace)
    {
        if(MApi_XC_R2BYTEMSK(REG_SC_BK0A_21_L, BIT(3)))//only active at Film22
        {
            MApi_XC_W2BYTEMSK(REG_SC_BK0A_02_L, ((MS_U16)0x06) << 8, 0x1F00);
            MApi_XC_W2BYTEMSK(REG_SC_BK0A_05_L, ((MS_U16)0x02) << 8, 0xFF00);
        }
        else if(MApi_XC_R2BYTEMSK(REG_SC_BK0A_21_L, BIT(4)))          // only active at Film32
        {
            MApi_XC_W2BYTEMSK(REG_SC_BK0A_02_L, ((MS_U16)0x06) << 8, 0x1F00);
        }
        else
        {
            MApi_XC_W2BYTEMSK(REG_SC_BK0A_02_L, ((MS_U16)0x1F) << 8, 0x1F00);
            MApi_XC_W2BYTEMSK(REG_SC_BK0A_05_L, ((MS_U16)0x10) << 8, 0xFF00);
        }

///--> new for T3
        //T3 add new feature for film mode to enhance the film lock or not
        MApi_XC_FilmMode_P();
///--> end

///--> new add function for skip film mode in still pattern
        if(!MApi_XC_R2BYTEMSK(REG_SC_BK12_03_L, BIT(1)))     // 2R mode can't enter this loop
        {
            u16_unmatch_3 = MApi_XC_R2BYTE(REG_SC_BK0A_0B_L);

            if((u16_unmatch_3 > 0x100))
                u8_unmatch_3_Cnt = 0;
            else if(u8_unmatch_3_Cnt == 0xFF)
                u8_unmatch_3_Cnt = 0xFF;
            else
                u8_unmatch_3_Cnt = u8_unmatch_3_Cnt + 1;

            if(u8_unmatch_3_Cnt > 0x05)
            {
                MApi_XC_W2BYTEMSK(REG_SC_BK0A_1E_L, 0x0002, 0x0002);
                MApi_XC_W2BYTEMSK(REG_SC_BK0A_21_L, 0x0006, 0x0006);
            }
            else
            {
                MApi_XC_W2BYTEMSK(REG_SC_BK0A_1E_L, 0x0000, 0x0002);
                MApi_XC_W2BYTEMSK(REG_SC_BK0A_21_L, 0x0000, 0x0006);
            }
        }
///--> end

        u8Value = (MS_U32)((MS_U32)u8PrevLuma * 3 + MApi_XC_DLC_GetAverageValue()) >> 2;
        u8PrevLuma = u8Value;
        MS_U32 u32MotionValue = MDrv_SC_read_motion_value1();
        if(u32MotionValue < 1200)
        {
            if(u8Value < 0x48)
            {
                MApi_XC_W2BYTE(REG_SC_BK0A_0C_L, 0x00);
            }
            else if((u8Value<0x88)&(u8Value >= 0x48))
            {
                MApi_XC_W2BYTE(REG_SC_BK0A_0C_L, 0x0011);
            }
            else
            {
                MApi_XC_W2BYTE(REG_SC_BK0A_0C_L, 0x1111);
            }
        }
        else
        {
            if(u8Value < 0x48)
            {
                MApi_XC_W2BYTE(REG_SC_BK0A_0C_L, 0x2222);
            }
            else if((u8Value<0x88)&(u8Value >= 0x48))
            {
                MApi_XC_W2BYTE(REG_SC_BK0A_0C_L, 0x5555);
            }
            else
            {
                MApi_XC_W2BYTE(REG_SC_BK0A_0C_L, 0xAAAA);
            }
        }
#if 0
        // for Fulke 4 vertical line pattern

        if(ucValue < 0x18)

        {

            MApi_XC_W2BYTEMSK(REG_SC_BK0A_02_L, 0x1000, 0x1F00);

            MApi_XC_W2BYTEMSK(REG_SC_BK0A_0C_L, 0x0400, 0xFFFF);

        }
#endif
    }
}

void MDrv_SC_film_driver(void)
{
    XC_ApiStatus stXCStatus;
    static MS_U8 u8FilmCnt,u8_unmatch_3_Cnt;

    MS_U32 u32Reg7C, u32Reg7D, u32Reg7E, u16_unmatch_3;
    MS_BOOL verticalMovingU;

    u32Reg7C = MApi_XC_R2BYTE(REG_SC_BK2A_7C_L);
    u32Reg7D = MApi_XC_R2BYTE(REG_SC_BK2A_7D_L);
    u32Reg7E = MApi_XC_R2BYTE(REG_SC_BK2A_7E_L);

    verticalMovingU = (u32Reg7E) > 0x0100 && (u32Reg7E > 8 * u32Reg7C) && (u32Reg7E > 8 * u32Reg7D);


    if(MApi_XC_GetStatus(&stXCStatus, MAIN_WINDOW) == FALSE)
    {
        DRVSCA_DBG(printf("MApi_XC_GetStatus failed because of InitData wrong, please update header file and compile again\n"));
    }

    if(stXCStatus.bInterlace)
    {
        if(MApi_XC_R2BYTEMSK(REG_SC_BK0A_21_L, BIT(3)))//only active at Film22
        {
            MApi_XC_W2BYTEMSK(REG_SC_BK0A_02_L, ((MS_U16)0x01) << 8, 0x1F00);
            MApi_XC_W2BYTEMSK(REG_SC_BK0A_05_L, ((MS_U16)0x02) << 8, 0xFF00);
        }
        else if(MApi_XC_R2BYTEMSK(REG_SC_BK0A_21_L, BIT(4)))          // only active at Film32
        {
            MApi_XC_W2BYTEMSK(REG_SC_BK0A_02_L, ((MS_U16)0x06) << 8, 0x1F00);
            MApi_XC_W2BYTEMSK(REG_SC_BK0A_05_L, ((MS_U16)0x10) << 8, 0xFF00);
        }
        else
        {
            MApi_XC_W2BYTEMSK(REG_SC_BK0A_02_L, ((MS_U16)0x1F) << 8, 0x1F00);
            MApi_XC_W2BYTEMSK(REG_SC_BK0A_05_L, ((MS_U16)0x10) << 8, 0xFF00);
        }

///--> new for T3
        //T3 add new feature for film mode to enhance the film lock or not
        MApi_XC_FilmMode_P();
///--> end

///--> new add function for skip film mode in still pattern
        if(!MApi_XC_R2BYTEMSK(REG_SC_BK12_03_L, BIT(1)))     // 2R mode can't enter this loop
        {
            u16_unmatch_3 = MApi_XC_R2BYTE(REG_SC_BK0A_0B_L);

            if((u16_unmatch_3 > 0x100))
                u8_unmatch_3_Cnt = 0;
            else if(u8_unmatch_3_Cnt == 0xFF)
                u8_unmatch_3_Cnt = 0xFF;
            else
                u8_unmatch_3_Cnt = u8_unmatch_3_Cnt + 1;

            if(u8_unmatch_3_Cnt > 0x05)
            {
                MApi_XC_W2BYTEMSK(REG_SC_BK0A_1E_L, 0x0002, 0x0002);
                MApi_XC_W2BYTEMSK(REG_SC_BK0A_21_L, 0x0006, 0x0006);
            }
            else
            {
                MApi_XC_W2BYTEMSK(REG_SC_BK0A_1E_L, 0x0000, 0x0002);
                MApi_XC_W2BYTEMSK(REG_SC_BK0A_21_L, 0x0000, 0x0006);
            }
        }
///--> end

///--> Jump out Film for LG tower

        if(!verticalMovingU)
            u8FilmCnt = 0;
        else if(u8FilmCnt == 0xFF)
            u8FilmCnt = 0xFF;
        else
            u8FilmCnt = u8FilmCnt + 1;

        if(u8FilmCnt > 0x05)
        {
        MApi_XC_W2BYTEMSK(REG_SC_BK0A_1E_L, 0x0002, 0x0002);
        MApi_XC_W2BYTEMSK(REG_SC_BK0A_21_L, 0x0006, 0x0006);
        }
        else
        {
        MApi_XC_W2BYTEMSK(REG_SC_BK0A_1E_L, 0x0000, 0x0002);
        MApi_XC_W2BYTEMSK(REG_SC_BK0A_21_L, 0x0000, 0x0006);
        }

///--> end

#if 0
        if(true == verticalMovingU)
        {
            MApi_XC_W2BYTE(REG_SC_BK0A_02_L, 0x2122);
            MApi_XC_W2BYTE(REG_SC_BK0A_0C_L, 0x2222);
        }
        else
            MApi_XC_W2BYTEMSK(REG_SC_BK0A_02_L, 0x0000, 0x2000);
#endif

        // for debug usage
        //MApi_XC_W2BYTE(REG_SC_BK22_7A_L, 0x1000);
        //MApi_XC_W2BYTE(REG_SC_BK20_1D_L, 0x0000);

    } // end interlace loop
}

void MDrv_SC_Janus_DHD_driver(void)
{
    // dhd shining line patch
    static MS_U32 u32videoCnt = 0;

    if(MApi_XC_R2BYTEMSK(REG_SC_BK0A_21_L, BIT(3)))  // film22
        u32videoCnt = 0;
    else if(MApi_XC_R2BYTEMSK(REG_SC_BK0A_21_L, BIT(4))) // film32
        u32videoCnt = 0;
    else if(MApi_XC_R2BYTEMSK(REG_SC_BK0A_21_L, BIT(6)))
        u32videoCnt = 0;
    else if(u32videoCnt >= 10000)
        u32videoCnt = 10000;
    else
        u32videoCnt++;

    if(u32videoCnt >= 60)   // 2 sec
    {
        // turn on dhd, 6R
        MApi_XC_W2BYTEMSK(REG_SC_BK12_05_L, 0x8000, 0x8000);    // 6R
        MApi_XC_W2BYTEMSK(REG_SC_BK05_20_L, 0x0001, 0x0001);    // dhd on
    }
    else
    {
        // turn off dhd, 4R
        MApi_XC_W2BYTEMSK(REG_SC_BK12_05_L, 0x0000, 0x8000);    // 4R
        MApi_XC_W2BYTEMSK(REG_SC_BK05_20_L, 0x0000, 0x0001);    // dhd off
    }

}

#define HD_ACT_TH 10
#define HDSDD_SPEED 1
#define HDSDD_DIV 0
#define HD_SPEED 2
#define SD_SPEED 8

void MDrv_SC_SDHD_DETECT_driver(void)
{
    MS_U8 u8Value;
    MS_U16 u16MinPixel, u16MaxPixel, MaxMinPixel;
        MS_U32 HDcnt_Act, HDcnt_NonAct;
        MS_U32 SDcnt_Act, SDcnt_NonAct;
    static MS_U16 HD_ConfirmCnt = 0xFF; // 0x7FFF == 0
    static MS_U8 u8PrevLuma = 0;
    static MS_U16 u16PrevMinPixel = 0;
    static MS_U16 u16PrevMaxPixel = 0;
    MS_U32 HD_ConfirmCnt_Reg;
    MS_U16 HDSDD_Det_threshold, HDSDD_Det_offset;

    // To avoid compiling warning
    UNUSED(HDcnt_NonAct);
    UNUSED(SDcnt_NonAct);

    HDcnt_Act = (MApi_XC_R2BYTE(REG_SC_BK02_75_L) << 16) + MApi_XC_R2BYTE(REG_SC_BK02_74_L);
    SDcnt_Act = (MApi_XC_R2BYTE(REG_SC_BK02_77_L) << 16) + MApi_XC_R2BYTE(REG_SC_BK02_76_L);
        HDcnt_NonAct =  (MApi_XC_R2BYTE(REG_SC_BK02_79_L) << 16) + MApi_XC_R2BYTE(REG_SC_BK02_78_L);
        SDcnt_NonAct =  (MApi_XC_R2BYTE(REG_SC_BK02_7B_L) << 16) + MApi_XC_R2BYTE(REG_SC_BK02_7A_L);
    HDSDD_Det_offset = (MS_U16)MApi_XC_R2BYTEMSK(REG_SC_BK22_7F_L, 0xFFFF);

    //printf("====HDcnt_Act===%x\n",HDcnt_Act);
    //printf("====SDcnt_Act===%x\n",SDcnt_Act);
    //printf("====HDcnt_NonAct===%x\n",HDcnt_NonAct);
    //printf("====SDcnt_NonAct===%x\n",SDcnt_NonAct);

    u8Value = (MS_U32)((MS_U32)u8PrevLuma * 3 + MApi_XC_DLC_GetAverageValue()) >> 2;
    u8PrevLuma = u8Value;

    u16MaxPixel = MApi_XC_R2BYTEMSK(REG_SC_BK1A_0B_L, 0x00FF);
    u16MaxPixel = (u16PrevMaxPixel * 3 + u16MaxPixel) >> 2;
    u16PrevMaxPixel = u16MaxPixel;

    u16MinPixel = MApi_XC_R2BYTEMSK(REG_SC_BK1A_0B_L, 0xFF00) >> 8;
    u16MinPixel = (u16PrevMinPixel * 3 + u16MinPixel) >> 2;
    u16PrevMinPixel = u16MinPixel;

    MaxMinPixel = u16MaxPixel - u16MinPixel;
    //printf("===u8MinPixel=%x==\n",u16MinPixel);
    //printf("===MaxMinPixel=%x==\n",MaxMinPixel);


    if(u8Value >= 0xFF)
        u8Value = 0xFF;
    //printf("===AvgLuma=%x==\n",u8Value);

    HDSDD_Det_threshold = HDSDD_Det_offset + ((MaxMinPixel * 6) >> 2) + ((u8Value * 5) >> 2);
    //printf("===HDSDD_Det_threshold=%x==\n",HDSDD_Det_threshold);

#if 0
    {
        if(u8Value > 0x80)
            Cur_HDSDD_Det_threshold = u16HDSDD_Det_Threshold;
        else if(u8Value > 0x40)
            Cur_HDSDD_Det_threshold = ((u16HDSDD_Det_Threshold * 3) >> 2);
        else if(u8Value > 0x20)
            Cur_HDSDD_Det_threshold = (u16HDSDD_Det_Threshold >> 1);
        else
            Cur_HDSDD_Det_threshold = (u16HDSDD_Det_Threshold >> 2);
    }
    else
        Cur_HDSDD_Det_threshold = u16HDSDD_Det_Threshold;
#endif

    if(HDSDD_Det_threshold >= 0xFFFF)
    {
        HDSDD_Det_threshold = 0xFFFF;
    }
    MApi_XC_W2BYTEMSK(REG_SC_BK02_71_L, (MS_U16)HDSDD_Det_threshold, 0xFFFF);
    MApi_XC_W2BYTEMSK(REG_SC_BK02_72_L, (MS_U16)(HDSDD_Det_threshold >> 1), 0xFFFF);

    HDcnt_Act = (MApi_XC_R2BYTE(REG_SC_BK02_75_L) << 16) + MApi_XC_R2BYTE(REG_SC_BK02_74_L);
    SDcnt_Act = (MApi_XC_R2BYTE(REG_SC_BK02_77_L) << 16) + MApi_XC_R2BYTE(REG_SC_BK02_76_L);
        HDcnt_NonAct =  (MApi_XC_R2BYTE(REG_SC_BK02_79_L) << 16) + MApi_XC_R2BYTE(REG_SC_BK02_78_L);
        SDcnt_NonAct =  (MApi_XC_R2BYTE(REG_SC_BK02_7B_L) << 16) + MApi_XC_R2BYTE(REG_SC_BK02_7A_L);

    if(HDcnt_Act > HD_ACT_TH && SDcnt_Act != 0)
    {
        if(HD_ConfirmCnt < HD_SPEED)
            HD_ConfirmCnt = 0;
        else
            HD_ConfirmCnt = HD_ConfirmCnt - HD_SPEED;
    }
    else if(SDcnt_Act != 0)  //&& HDcnt_NonAct !=0) // && (HDcnt_NonAct < 0x90000))
    {
        if(HD_ConfirmCnt > (0xFF - SD_SPEED))
            HD_ConfirmCnt = 0xFF;
        else
            HD_ConfirmCnt = HD_ConfirmCnt + SD_SPEED;;

    }
    else
    {
        if(HD_ConfirmCnt < 1)
            HD_ConfirmCnt = 0;
        else
            HD_ConfirmCnt = HD_ConfirmCnt - 1;
    }

    if(HD_ConfirmCnt > 0x80)
        HD_ConfirmCnt_Reg = HD_ConfirmCnt - 0x80;
    else
        HD_ConfirmCnt_Reg = 0;

    HD_ConfirmCnt_Reg = (HD_ConfirmCnt_Reg * HDSDD_SPEED) >> HDSDD_DIV;

    if(HD_ConfirmCnt_Reg > 0x3F)
        HD_ConfirmCnt_Reg = 0x3F;

    //printf("====HD_ConfirmCnt_Reg===%x\n",HD_ConfirmCnt_Reg);

    MApi_XC_W2BYTEMSK(REG_SC_BK23_54_L, (MS_U16)HD_ConfirmCnt_Reg, 0x003F);

}

void MDrv_SC_NEW_SDHD_DETECT_driver(void)
{
        MS_U8 u8Value,FEATHER_CNT_TH;
        MS_U16 u16MinPixel,u16MaxPixel, MaxMinPixel;
        MS_U32 HDcnt_Act, HDcnt_NonAct,FeatherCnt;
        MS_U32 SDcnt_Act, SDcnt_NonAct;
        static MS_U32 PreFeatherCnt = 0;
        static MS_U16 HD_ConfirmCnt = 0xFF; // 0x7FFF == 0
        static MS_U8 u8PrevLuma = 0;
        static MS_U16 u16PrevMinPixel = 0;
        static MS_U16 u16PrevMaxPixel = 0;
        MS_U32 HD_ConfirmCnt_Reg;
        MS_U16 HDSDD_Det_threshold,HDSDD_Det_offset;

        // To avoid compiling warning
        UNUSED(HDcnt_NonAct);
        UNUSED(SDcnt_NonAct);

        HDcnt_Act =  (MApi_XC_R2BYTE(REG_SC_BK02_75_L) << 16) + MApi_XC_R2BYTE(REG_SC_BK02_74_L);
        SDcnt_Act =  (MApi_XC_R2BYTE(REG_SC_BK02_77_L) << 16) + MApi_XC_R2BYTE(REG_SC_BK02_76_L);
        HDcnt_NonAct =  (MApi_XC_R2BYTE(REG_SC_BK02_79_L) << 16) + MApi_XC_R2BYTE(REG_SC_BK02_78_L);
        SDcnt_NonAct =  (MApi_XC_R2BYTE(REG_SC_BK02_7B_L) << 16) + MApi_XC_R2BYTE(REG_SC_BK02_7A_L);
        HDSDD_Det_offset = (MS_U16)MApi_XC_R2BYTEMSK(REG_SC_BK22_7F_L, 0xFFFF);

        FeatherCnt = MApi_XC_R2BYTE(REG_SC_BK23_56_L);       //New add for A1 chip
        FEATHER_CNT_TH = MApi_XC_R2BYTEMSK(REG_SC_BK22_7D_L, 0xFF00)>>8;

        //printf("====HDcnt_Act===%x\n",HDcnt_Act);
        //printf("====SDcnt_Act===%x\n",SDcnt_Act);
        //printf("====HDcnt_NonAct===%x\n",HDcnt_NonAct);
        //printf("====SDcnt_NonAct===%x\n",SDcnt_NonAct);

        FeatherCnt = ( PreFeatherCnt*3 + FeatherCnt ) >> 2;
        PreFeatherCnt = FeatherCnt;

        u8Value=(MS_U32)((MS_U32)u8PrevLuma*3 + MApi_XC_DLC_GetAverageValue())>>2;
        u8PrevLuma = u8Value;

        u16MaxPixel = MApi_XC_R2BYTEMSK(REG_SC_BK1A_0B_L, 0x00FF);
        u16MaxPixel = (u16PrevMaxPixel*3 + u16MaxPixel ) >> 2;
        u16PrevMaxPixel = u16MaxPixel;

        u16MinPixel = MApi_XC_R2BYTEMSK(REG_SC_BK1A_0B_L, 0xFF00)>>8;
        u16MinPixel = (u16PrevMinPixel*3 + u16MinPixel ) >> 2;
        u16PrevMinPixel = u16MinPixel;

        MaxMinPixel = u16MaxPixel - u16MinPixel;
        //printf("===u8MinPixel=%x==\n",u16MinPixel);
        //printf("===MaxMinPixel=%x==\n",MaxMinPixel);


        if(u8Value>=0xFF)
            u8Value = 0xFF;
        //printf("===AvgLuma=%x==\n",u8Value);

        HDSDD_Det_threshold = HDSDD_Det_offset +((MaxMinPixel*6)>>2) + ((u8Value*5)>>2);
        //printf("===HDSDD_Det_threshold=%x==\n",HDSDD_Det_threshold);

        #if 0
        {
        if( u8Value > 0x80)
                Cur_HDSDD_Det_threshold = u16HDSDD_Det_Threshold;
        else if( u8Value > 0x40)
                Cur_HDSDD_Det_threshold = ((u16HDSDD_Det_Threshold*3)>>2);
            else if( u8Value > 0x20)
                Cur_HDSDD_Det_threshold = (u16HDSDD_Det_Threshold>>1);
        else
                Cur_HDSDD_Det_threshold = (u16HDSDD_Det_Threshold>>2);
        }
        else
            Cur_HDSDD_Det_threshold = u16HDSDD_Det_Threshold;
         #endif

        if(HDSDD_Det_threshold >=0xFFFF)
        {
                       HDSDD_Det_threshold=0xFFFF;
        }
        MApi_XC_W2BYTEMSK(REG_SC_BK02_71_L, (MS_U16)HDSDD_Det_threshold, 0xFFFF);
        MApi_XC_W2BYTEMSK(REG_SC_BK02_72_L, (MS_U16)(HDSDD_Det_threshold>>1), 0xFFFF);

        HDcnt_Act =  (MApi_XC_R2BYTE(REG_SC_BK02_75_L) << 16) + MApi_XC_R2BYTE(REG_SC_BK02_74_L);
        SDcnt_Act =  (MApi_XC_R2BYTE(REG_SC_BK02_77_L) << 16) + MApi_XC_R2BYTE(REG_SC_BK02_76_L);
        HDcnt_NonAct =  (MApi_XC_R2BYTE(REG_SC_BK02_79_L) << 16) + MApi_XC_R2BYTE(REG_SC_BK02_78_L);
        SDcnt_NonAct =  (MApi_XC_R2BYTE(REG_SC_BK02_7B_L) << 16) + MApi_XC_R2BYTE(REG_SC_BK02_7A_L);


        if( (HDcnt_Act > HD_ACT_TH) && (SDcnt_Act != 0) )
        {
                if(HD_ConfirmCnt < HD_SPEED)
                        HD_ConfirmCnt = 0;
                else
                        HD_ConfirmCnt = HD_ConfirmCnt - HD_SPEED;
        }
        else if( (SDcnt_Act != 0) && (FeatherCnt > FEATHER_CNT_TH) )//&& HDcnt_NonAct !=0) // && (HDcnt_NonAct < 0x90000))
        {
                if(HD_ConfirmCnt > (0xFF - SD_SPEED))
                        HD_ConfirmCnt = 0xFF;
                else
                        HD_ConfirmCnt = HD_ConfirmCnt + SD_SPEED;;

        }
        else
         {
             if(HD_ConfirmCnt < 1)
                        HD_ConfirmCnt = 0;
                else
                        HD_ConfirmCnt = HD_ConfirmCnt - 1;
         }

        if( HD_ConfirmCnt > 0x80 )
                HD_ConfirmCnt_Reg = HD_ConfirmCnt - 0x80;
        else
                HD_ConfirmCnt_Reg = 0;

        HD_ConfirmCnt_Reg = (HD_ConfirmCnt_Reg*HDSDD_SPEED) >> HDSDD_DIV;

        if( HD_ConfirmCnt_Reg > 0x3F )
            HD_ConfirmCnt_Reg = 0x3F;

        //Debug mode for SDHDD detection function
         #if 0
                if( HD_ConfirmCnt_Reg == 0x3F )
                    MApi_XC_W2BYTEMSK(REG_SC_BK22_7A_L, 0x1000, 0x1000);
                else
                    MApi_XC_W2BYTEMSK(REG_SC_BK22_7A_L, 0x0000, 0x1000);

         #endif
        //printf("====HD_ConfirmCnt_Reg===%x\n",HD_ConfirmCnt_Reg);

        MApi_XC_W2BYTEMSK(REG_SC_BK23_54_L, (MS_U16)HD_ConfirmCnt_Reg, 0x003F);

}

static MS_U32 md_adjust_level_iir   = 0;
static MS_U32 psnr_std_level_thrd_iir  = 0;
static MS_U32 dnr_std_thrd_iir = 0;
static MS_U32 md_lut_iir[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} ;
static MS_U32 luma_sum_pre = 0;

void MDrv_SC_HISNR_Maria10_driver(MS_U16 u16Width, MS_U16 u16Height)
{
    MS_BOOL histogram_ack_en = 0;
    MS_U16 noise_histogram[10];            //noise level detection
    MS_U16 noise_histogram_scaled[10];
    MS_U16 noise_histogram_hm[8];          // hismatching
    MS_U16 noise_histogram_hm_scaled[8];
    MS_U8 rounding_bit = 0;
    MS_U8 rounding_array[16];



    MS_U16 u16Height_div;

    XC_ApiStatus stXCStatus;

    if(MApi_XC_GetStatus(&stXCStatus, MAIN_WINDOW) == FALSE)
    {
        printf("MApi_XC_GetStatus failed because of InitData wrong, please update header file and compile again\n");
    }

    if(stXCStatus.bInterlace)
        u16Height_div = u16Height / 2 ;
    else
        u16Height_div = u16Height ;


    MS_U32 scaled_ratio = 4.0 * (u16Width * u16Height_div / 172800); // 720*240 = 172800

    if(scaled_ratio < 4)
        scaled_ratio = 4;

    MS_U32 scaled_ratio1 = 1 * (u16Width * u16Height_div / 172800); // 720*240 = 172800

    if(scaled_ratio1 < 1)
        scaled_ratio1 = 1;

    MS_U16 bin;

    MS_U16 md_lut_default[16] = {0xf, 0xd, 0x9, 0x6, 4, 3, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0} ;
    MS_U32 md_lut_level00[16] = {15, 14, 13, 12, 11, 10, 9, 8, 6, 4, 2, 0, 0, 0, 0, 0} ;

    MS_U32 md_lut_level1[16] = {9, 11, 13, 15, 15, 13, 11, 9, 7, 5, 3, 1, 0, 0, 0, 0} ;
    MS_U32 md_lut_level2[16] = {7, 9, 11, 13, 15, 14, 12, 10, 8, 6, 4, 0, 0, 0, 0, 0} ;
//  MS_U32 md_lut_level3[16] = {0,1,3,5,7,9,11,13,15,15,12,10,8,6,3,0} ;


    md_lut_default[0] =             MApi_XC_R2BYTEMSK(REG_SC_BK06_40_L, 0x000F);
    md_lut_default[1] =     MApi_XC_R2BYTEMSK(REG_SC_BK06_40_L, 0x00F0) >> 4;
    md_lut_default[2] =     MApi_XC_R2BYTEMSK(REG_SC_BK06_40_L, 0x0F00) >> 8;
    md_lut_default[3] =     MApi_XC_R2BYTEMSK(REG_SC_BK06_40_L, 0xF000) >> 12;
    md_lut_default[4] =     MApi_XC_R2BYTEMSK(REG_SC_BK06_41_L, 0x000F);
    md_lut_default[5] =     MApi_XC_R2BYTEMSK(REG_SC_BK06_41_L, 0x00F0) >> 4;
    md_lut_default[6] =     MApi_XC_R2BYTEMSK(REG_SC_BK06_41_L, 0x0F00) >> 8;
    md_lut_default[7] =     MApi_XC_R2BYTEMSK(REG_SC_BK06_41_L, 0xF000) >> 12;
    md_lut_default[8] =     MApi_XC_R2BYTEMSK(REG_SC_BK06_42_L, 0x000F);
    md_lut_default[9] =     MApi_XC_R2BYTEMSK(REG_SC_BK06_42_L, 0x00F0) >> 4;
    md_lut_default[10] =     MApi_XC_R2BYTEMSK(REG_SC_BK06_42_L, 0x0F00) >> 8;
    md_lut_default[11] =     MApi_XC_R2BYTEMSK(REG_SC_BK06_42_L, 0xF000) >> 12;
    md_lut_default[12] =     MApi_XC_R2BYTEMSK(REG_SC_BK06_43_L, 0x000F);
    md_lut_default[13] =     MApi_XC_R2BYTEMSK(REG_SC_BK06_43_L, 0x00F0) >> 4;
    md_lut_default[14] =     MApi_XC_R2BYTEMSK(REG_SC_BK06_43_L, 0x0F00) >> 8;
    md_lut_default[15] =     MApi_XC_R2BYTEMSK(REG_SC_BK06_43_L, 0xF000) >> 12;

    MS_U16 hismatch_level_thrd = 0;
    MS_U16 dnr_std_thrd = 0;
    MS_U16 md_adjust_level = 0;
    MS_U16 psnr_std_level_thrd = 0;

    MS_U32 md_lut_new[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} ;
    MS_U32 hismatch_level_thrd_iir = 0 ;



    // get image size 480i,1080i
    MS_U16 V_start_height;

    if(u16Width <= 1000)
        V_start_height = u16Height_div / 240;
    else
        V_start_height = 0x10;




    MS_U16 V_end_height = (u16Height_div - V_start_height * 8) / 8;
    MS_U16 V_active_height = (V_end_height - V_start_height) * 8;

    MApi_XC_W2BYTEMSK(REG_SC_BK0E_1A_L, V_start_height, 0x00FF);
    MApi_XC_W2BYTEMSK(REG_SC_BK0E_1A_L, V_end_height << 8, 0xFF00);


    MS_U16 H_start_width, H_end_width;

    {
        H_start_width = 0;
        H_end_width   = u16Width / 8;
    }


    MApi_XC_W2BYTEMSK(REG_SC_BK0E_19_L, H_start_width, 0x00FF);
    MApi_XC_W2BYTEMSK(REG_SC_BK0E_19_L, H_end_width << 8, 0xFF00);

    MS_U16 H_active_width = (H_end_width - H_start_width) * 8;

    MS_U32 pixel_number = (H_active_width * V_active_height) ; // directly take

    //-------------------------------------------------------
    while(!histogram_ack_en)
        histogram_ack_en = MApi_XC_R2BYTEMSK(REG_SC_BK0E_01_L, BIT(2));

    if(histogram_ack_en)
    {
        MApi_XC_W2BYTEMSK(REG_SC_BK0E_01_L, 1, BIT(2));

        noise_histogram[0] = (MS_U16)MApi_XC_R2BYTE(REG_SC_BK0E_05_L) / 2;
        noise_histogram[1] = (MS_U16)MApi_XC_R2BYTE(REG_SC_BK0E_06_L) / 2;
        noise_histogram[2] = (MS_U16)MApi_XC_R2BYTE(REG_SC_BK0E_07_L) / 2;
        noise_histogram[3] = (MS_U16)MApi_XC_R2BYTE(REG_SC_BK0E_08_L) / 2;
        noise_histogram[4] = (MS_U16)MApi_XC_R2BYTE(REG_SC_BK0E_09_L) / 2;
        noise_histogram[5] = (MS_U16)MApi_XC_R2BYTE(REG_SC_BK0E_0A_L) / 2;
        noise_histogram[6] = (MS_U16)MApi_XC_R2BYTE(REG_SC_BK0E_0B_L) / 2;
        noise_histogram[7] = (MS_U16)MApi_XC_R2BYTE(REG_SC_BK0E_0C_L) / 2;
        noise_histogram[8] = (MS_U16)MApi_XC_R2BYTE(REG_SC_BK0E_0D_L) / 2;
        noise_histogram[9] = (MS_U16)MApi_XC_R2BYTE(REG_SC_BK0E_0E_L) / 2;

        noise_histogram_hm[0] = (MS_U16)MApi_XC_R2BYTE(REG_SC_BK0E_1C_L) / 2;
        noise_histogram_hm[1] = (MS_U16)MApi_XC_R2BYTE(REG_SC_BK0E_1D_L) / 2;
        noise_histogram_hm[2] = (MS_U16)MApi_XC_R2BYTE(REG_SC_BK0E_1F_L) / 2;
        noise_histogram_hm[3] = (MS_U16)MApi_XC_R2BYTE(REG_SC_BK0E_20_L) / 2;
        noise_histogram_hm[4] = (MS_U16)MApi_XC_R2BYTE(REG_SC_BK0E_21_L) / 2;
        noise_histogram_hm[5] = (MS_U16)MApi_XC_R2BYTE(REG_SC_BK0E_22_L) / 2;
        noise_histogram_hm[6] = (MS_U16)MApi_XC_R2BYTE(REG_SC_BK0E_23_L) / 2;
        noise_histogram_hm[7] = (MS_U16)MApi_XC_R2BYTE(REG_SC_BK0E_24_L) / 2;

        for(bin = 0; bin < 10; bin++)
        {
            if(bin < 8)
            {
                noise_histogram_scaled[bin]    = noise_histogram[bin] / scaled_ratio1;
                noise_histogram_hm_scaled[bin] = noise_histogram_hm[bin] / scaled_ratio;
            }
            else
                noise_histogram_scaled[bin] =    noise_histogram[bin] / scaled_ratio1;

        }

        MS_U16  LargerThanHigh, Off_from_low; // cond_diagonal, BitsValue_3,adjust_alpha;

        /*
        MS_U32 weighted_sum = noise_histogram_hm_scaled[0]*64*scaled_ratio;   // 6bits
        MS_U32 weighted_sumother=0;


        for(bin=5; bin<8; bin++)
            weighted_sumother += noise_histogram_hm_scaled[bin];
        */


        MS_U16 luma_sum = (MS_U16)MApi_XC_R2BYTE(REG_SC_BK0E_1B_L);



        //-------

        MS_U32 luma_avg_pre  = (luma_sum_pre * 16384) / pixel_number; // 16bits 2^14
        MS_U32 luma_avg      = (luma_sum * 16384) / pixel_number;
        MS_U16 luma_avg_diff = (MS_U16) abs(luma_avg_pre - luma_avg);

        luma_sum_pre = luma_sum;

        MS_U32 iir_strength = 2;

        // remove pause_ack
        if(luma_avg_diff >= 20)
            iir_strength = 8;



        MApi_XC_W2BYTEMSK(REG_SC_BK0E_0F_L, noise_histogram_hm_scaled[0], 0x1FF);
        MApi_XC_W2BYTEMSK(REG_SC_BK0E_10_L, noise_histogram_hm_scaled[1], 0x1FF);
        MApi_XC_W2BYTEMSK(REG_SC_BK0E_11_L, noise_histogram_hm_scaled[2], 0x1FF);
        MApi_XC_W2BYTEMSK(REG_SC_BK0E_12_L, noise_histogram_hm_scaled[3], 0x1FF);
        MApi_XC_W2BYTEMSK(REG_SC_BK0E_13_L, noise_histogram_hm_scaled[4], 0x1FF);
        MApi_XC_W2BYTEMSK(REG_SC_BK0E_14_L, noise_histogram_hm_scaled[5], 0x1FF);
        MApi_XC_W2BYTEMSK(REG_SC_BK0E_15_L, noise_histogram_hm_scaled[6], 0x1FF);
        MApi_XC_W2BYTEMSK(REG_SC_BK0E_16_L, noise_histogram_hm_scaled[7], 0x1FF);




        MS_U32 histogram_sum_final = noise_histogram_scaled[0] + noise_histogram_scaled[1] + noise_histogram_scaled[2]
                                     + noise_histogram_scaled[3] + noise_histogram_scaled[4] + noise_histogram_scaled[5]
                                     + noise_histogram_scaled[6] + noise_histogram_scaled[7];



        MS_U16 max_value_final = 0;
        MS_U16 max_bin_final = 0;
        MS_U16 incre_scale = 7;  // old is 7
        for(bin = 8; bin > 0; bin--)
        {

            if(noise_histogram_scaled[bin] + noise_histogram_scaled[bin-1] >= max_value_final)
            {
                if(noise_histogram_scaled[bin-1] >= noise_histogram_scaled[bin])
                    max_bin_final = bin + incre_scale - 1 ;
                else
                    max_bin_final = bin + incre_scale;
                max_value_final = noise_histogram_scaled[bin] + noise_histogram_scaled[bin-1];

            }
            incre_scale--;
        }


        if(noise_histogram_scaled[0] >= (histogram_sum_final * 3 / 4))
            max_bin_final = 0;


        MS_S32  weighted_sum_final_dec = (noise_histogram_scaled[9] - 200) / 4.0;
        MS_U32 weighted_sum_final_dec_value = 0;

        if((weighted_sum_final_dec & 0x8000) == 32768)
        {
            weighted_sum_final_dec_value = 0;
        }
        else
        {
            weighted_sum_final_dec_value = weighted_sum_final_dec & 0xFFFF;
        }



        //   float  weighted_sum_final_dec= ( float( ( noise_histogram_scaled[9] - 200.0 )  /4.0 )>=0 )? float( ( noise_histogram_scaled[9] - 200.0 )  /4.0 ): 0 ;   // old is 400  /8.0
        MS_U16 BitsValue_4;
        MS_U16 decrease_alpha_final;
        if(histogram_sum_final > 100)
        {
            //cond_diagonal = 1;
            decrease_alpha_final = 0xF;
        }
        else
        {
            Off_from_low =  100 -  histogram_sum_final;


            //cond_diagonal = 0;
            switch(noise_bin_step)
            {
                case 0:
                    BitsValue_4 = Off_from_low & 0xF;
                    LargerThanHigh = ((Off_from_low >> 4) > 0) ? 1 : 0;
                    break;
                case 1:
                    BitsValue_4 = (Off_from_low >> 1) & 0xF;
                    LargerThanHigh = ((Off_from_low >> 5) > 0) ? 1 : 0;
                    break;
                case 2:
                    BitsValue_4 = (Off_from_low >> 2) & 0xF;
                    LargerThanHigh = ((Off_from_low >> 6) > 0) ? 1 : 0;
                    break;
                case 3:
                    BitsValue_4 = (Off_from_low >> 3) & 0x3F;
                    LargerThanHigh = ((Off_from_low >> 7) > 0) ? 1 : 0;
                    break;

                default:
                    printf(" this is wrong case !\n");
                    exit(1) ;
                    break;
            } // end switch

            if(LargerThanHigh == 1)
                decrease_alpha_final = 0;
            else
                decrease_alpha_final =  ~BitsValue_4;
            decrease_alpha_final = decrease_alpha_final & 0xF;


        } // end of diag switch

        MS_U32 queo_result = max_bin_final * 6; // 6 : withour NR

        MS_S32 result_final = ((queo_result * (decrease_alpha_final + 1)) >> 4) - weighted_sum_final_dec_value ;


        MS_BOOL result_final_sign  ;
        MS_U32 result_final_value = 0;

        if((result_final & 0x8000) == 32768)
        {
            result_final_sign = 1;
            result_final_value = (~(result_final & 0xFFFF)) + 1;
        }
        else
        {
            result_final_value = result_final & 0xFFFF;
            result_final_sign = 0;
        }




        if(max_bin_final != 0)
        {
            if(result_final_sign == 1 || (result_final_sign == 0 &&  result_final_value <= 5))
                result_final_value = 5;
        }
        else
        {
            if(result_final_sign == 1)
                result_final_value = 0;
        }

        /*

        printf("=====max_bin_final== %x\n",max_bin_final);
              printf("=====queo_result== %x\n",queo_result);
              printf("=====weighted_sum_final_dec_value== %x\n",weighted_sum_final_dec_value);
        printf("=====result_final== %x\n",result_final);
        printf("=====result_final_value== %x\n",result_final_value);

              */




        MS_U16 calculated_result_final_nr = (MS_U16)(result_final_value);

        //calculated_result_final_nr = (calculated_result_final_nr >= 75) ? 75 : (calculated_result_final_nr <= 0 ) ?  0 :  calculated_result_final_nr ;
        if(calculated_result_final_nr >= 75)
        {
            calculated_result_final_nr = 75;
        }
        else if(calculated_result_final_nr <= 0)
        {
            calculated_result_final_nr = 0;
        }

        MS_U16 noise_level_int;

        // calculated_result_final_nr >= 0 && calculated_result_final_nr <= 75
        if(calculated_result_final_nr <= 2)     //34 is ok , wh
        {
            noise_level_int = 0;
        }
        // calculated_result_final_nr > 2 && calculated_result_final_nr <= 75
        else if(calculated_result_final_nr <= 10)
        {
            noise_level_int = 1;
        }
        // calculated_result_final_nr > 10 && calculated_result_final_nr <= 75
        else if(calculated_result_final_nr <= 18)
        {
            noise_level_int = 2;
        }
        // calculated_result_final_nr > 18 && calculated_result_final_nr <= 75
        else if(calculated_result_final_nr <= 23)  //24
        {
            noise_level_int = 3;
        }
        // calculated_result_final_nr > 23 && calculated_result_final_nr <= 75
        else if( calculated_result_final_nr <= 33)
        {
            noise_level_int = 4;
        }
        // calculated_result_final_nr > 33 && calculated_result_final_nr <= 75
        else if(calculated_result_final_nr <= 41)
        {
            noise_level_int = 5;
        }
        // calculated_result_final_nr > 41 && calculated_result_final_nr <= 75
        else if(calculated_result_final_nr <= 49)
        {
            noise_level_int = 6;
        }
        // calculated_result_final_nr > 49 && calculated_result_final_nr <= 75
        else
        {
            noise_level_int = 7;
        }

        //MApi_XC_W2BYTEMSK(REG_SC_BK18_60_L, ((noise_level_int)&0x0007)<<8 , 0xFF00);


        //printf("=====noise_level_int== %x\n",noise_level_int);

        MS_U16 noise_strength = MApi_XC_R2BYTEMSK(REG_SC_BK18_60_L, 0x7F00) >> 8;

#if 0
        if(noise_strength == 0)  // 1st
        {

            if(noise_level_int == 0)
            {
                hismatch_level_thrd = hismatch_level0;
                dnr_std_thrd = 5;

                for(bin = 0; bin < 16; bin++)
                    md_lut_new[bin] = md_lut_level0[bin];;

                md_adjust_level = md_adjust_level0;
                psnr_std_level_thrd = psnr_std_level0_thrd;

            }
            else if(noise_level_int == 1)
            {
                hismatch_level_thrd = hismatch_level0;
                dnr_std_thrd = dnr_std_level0;

                for(bin = 0; bin < 16; bin++)
                    md_lut_new[bin] = md_lut_level0[bin];
                md_adjust_level = md_adjust_level0;
                psnr_std_level_thrd = psnr_std_level0_thrd;
            }
            else if(noise_level_int == 2)
            {
                hismatch_level_thrd = hismatch_level0;
                dnr_std_thrd = dnr_std_level0;

                for(bin = 0; bin < 16; bin++)
                    md_lut_new[bin] = md_lut_level0[bin];
                md_adjust_level = 2;
                psnr_std_level_thrd = psnr_std_level0_thrd;

            }
            else if(noise_level_int == 3)
            {
                hismatch_level_thrd = hismatch_level1;
                dnr_std_thrd = dnr_std_level1;

                for(bin = 0; bin < 16; bin++)
                    md_lut_new[bin] = md_lut_level1[bin];
                md_adjust_level = 2;
                psnr_std_level_thrd = psnr_std_level1_thrd;


            }
            else if(noise_level_int == 4)
            {
                hismatch_level_thrd = hismatch_level2;
                dnr_std_thrd = dnr_std_level2;

                md_adjust_level = 6;
                for(bin = 0; bin < 16; bin++)
                    md_lut_new[bin] = md_lut_level2[bin];
                psnr_std_level_thrd = psnr_std_level2_thrd;
            }
            else if(noise_level_int == 5)
            {
                hismatch_level_thrd = hismatch_level2;
                dnr_std_thrd = dnr_std_level2;

                md_adjust_level = 8;

                for(bin = 0; bin < 16; bin++)
                    md_lut_new[bin] = md_lut_level2[bin];
                psnr_std_level_thrd = psnr_std_level2_thrd;


            }
            else if(noise_level_int == 6 || noise_level_int == 7)
            {
                hismatch_level_thrd = hismatch_level3  ;   //hismatch_level3
                dnr_std_thrd = dnr_std_level3;

                for(bin = 0; bin < 16; bin++)
                    md_lut_new[bin] = md_lut_level2[bin];

                md_adjust_level = 10;
                psnr_std_level_thrd = psnr_std_level3_thrd;


            }

        }
        else if(noise_strength == 1)
        {

            if(noise_level_int == 0)
            {

                hismatch_level_thrd = hismatch_level0;
                dnr_std_thrd = 3;

                for(bin = 0; bin < 16; bin++)
                    md_lut_new[bin] = md_lut_level0[bin];;

                md_adjust_level = 2;
                psnr_std_level_thrd = psnr_std_level0_thrd;

            }
            else if(noise_level_int == 1)
            {
                hismatch_level_thrd = hismatch_level0;
                dnr_std_thrd = dnr_std_level0;

                for(bin = 0; bin < 16; bin++)
                    md_lut_new[bin] = md_lut_level0[bin];
                md_adjust_level = 3;
                psnr_std_level_thrd = psnr_std_level0_thrd;
            }
            else if(noise_level_int == 2)
            {
                hismatch_level_thrd = hismatch_level1;
                dnr_std_thrd = dnr_std_level1;

                for(bin = 0; bin < 16; bin++)
                    md_lut_new[bin] = md_lut_level1[bin];
                md_adjust_level = 4;
                psnr_std_level_thrd = psnr_std_level1_thrd;

            }
            else if(noise_level_int == 3)
            {
                hismatch_level_thrd = hismatch_level1;
                dnr_std_thrd = dnr_std_level1;

                for(bin = 0; bin < 16; bin++)
                    md_lut_new[bin] = md_lut_level1[bin];
                md_adjust_level = 5;
                psnr_std_level_thrd = psnr_std_level1_thrd;


            }
            else if(noise_level_int == 4)
            {
                hismatch_level_thrd = hismatch_level2;
                dnr_std_thrd = dnr_std_level2;

                md_adjust_level = 6;
                for(bin = 0; bin < 16; bin++)
                    md_lut_new[bin] = md_lut_level2[bin];
                psnr_std_level_thrd = psnr_std_level2_thrd;


            }
            else if(noise_level_int == 5)
            {
                hismatch_level_thrd = hismatch_level2;
                dnr_std_thrd = dnr_std_level2;

                md_adjust_level = 8;

                for(bin = 0; bin < 16; bin++)
                    md_lut_new[bin] = md_lut_level2[bin];
                psnr_std_level_thrd = psnr_std_level2_thrd;


            }
            else if(noise_level_int == 6 || noise_level_int == 7)
            {
                hismatch_level_thrd = hismatch_level3  ;   //hismatch_level3
                dnr_std_thrd = dnr_std_level3;

                for(bin = 0; bin < 16; bin++)
                    md_lut_new[bin] = md_lut_level2[bin];

                md_adjust_level = 10;
                psnr_std_level_thrd = psnr_std_level3_thrd;


            }

        }
        else if(noise_strength == 2)
        {

            if(noise_level_int == 0)
            {

                hismatch_level_thrd = hismatch_level0;
                dnr_std_thrd = 2 ;

                for(bin = 0; bin < 16; bin++)
                    md_lut_new[bin] = md_lut_level0[bin];;
                md_adjust_level = md_adjust_level0 + 1;

                psnr_std_level_thrd = psnr_std_level0_thrd;


            }
            else if(noise_level_int == 1)
            {
                hismatch_level_thrd = hismatch_level0;
                dnr_std_thrd = dnr_std_level0;

                for(bin = 0; bin < 16; bin++)
                    md_lut_new[bin] = md_lut_level0[bin];
                md_adjust_level = md_adjust_level0 + 1;

                psnr_std_level_thrd = psnr_std_level0_thrd;
            }
            else if(noise_level_int == 2)
            {
                hismatch_level_thrd = hismatch_level0;
                dnr_std_thrd = dnr_std_level0;

                for(bin = 0; bin < 16; bin++)
                    md_lut_new[bin] = md_lut_level0[bin];
                md_adjust_level = 2 + 1;

                psnr_std_level_thrd = psnr_std_level0_thrd;

            }
            else if(noise_level_int == 3)
            {
                hismatch_level_thrd = hismatch_level1;
                dnr_std_thrd = dnr_std_level1;

                for(bin = 0; bin < 16; bin++)
                    md_lut_new[bin] = md_lut_level1[bin];
                md_adjust_level = 2 + 2;

                psnr_std_level_thrd = psnr_std_level1_thrd;


            }
            else if(noise_level_int == 4)
            {
                hismatch_level_thrd = hismatch_level2;
                dnr_std_thrd = dnr_std_level2;

                md_adjust_level = 6 + 2;

                for(bin = 0; bin < 16; bin++)
                    md_lut_new[bin] = md_lut_level2[bin];
                psnr_std_level_thrd = psnr_std_level2_thrd;

            }
            else if(noise_level_int == 5)
            {
                hismatch_level_thrd = hismatch_level2;
                dnr_std_thrd = dnr_std_level2;

                md_adjust_level = 8 + 2; //  //6  FPGA

                for(bin = 0; bin < 16; bin++)
                    md_lut_new[bin] = md_lut_level2[bin];
                psnr_std_level_thrd = psnr_std_level2_thrd;

            }
            else if(noise_level_int == 6 || noise_level_int == 7)
            {
                hismatch_level_thrd = hismatch_level3  ;   //hismatch_level3
                dnr_std_thrd = dnr_std_level3;

                for(bin = 0; bin < 16; bin++)
                    md_lut_new[bin] = md_lut_level2[bin];
                md_adjust_level = 12;    //8 FPGA
                // md_adjust_level =14;     //8 FPGA
                psnr_std_level_thrd = psnr_std_level3_thrd;


            }

        }
#endif

        //else if( noise_strength == 3 )  // sd_new low
        if(noise_strength == 0)    // sd_new low
        {
            if(noise_level_int == 0 || noise_level_int == 1 || noise_level_int == 2)
            {

                dnr_std_thrd = 3 + 2; //

                for(bin = 0; bin < 16; bin++)
                    md_lut_new[bin] = md_lut_level00[bin];
                md_adjust_level = 2 - 1;

                psnr_std_level_thrd = 0x13;


            }

            else if(noise_level_int == 3)
            {

                dnr_std_thrd = 5;

                for(bin = 0; bin < 16; bin++)
                    md_lut_new[bin] = md_lut_level1[bin];
                md_adjust_level = 2 - 1;
                psnr_std_level_thrd = 0x20;


            }

            else if(noise_level_int == 4)
            {

                dnr_std_thrd = 8;

                md_adjust_level = 7 - 1;
                for(bin = 0; bin < 16; bin++)
                    md_lut_new[bin] = md_lut_level2[bin];
                psnr_std_level_thrd = 0x10;


            }
            else if(noise_level_int == 5)
            {

                dnr_std_thrd = 8;

                md_adjust_level = 8 - 1;
                for(bin = 0; bin < 16; bin++)
                    md_lut_new[bin] = md_lut_level2[bin];
                psnr_std_level_thrd = 0x10;


            }
            else if(noise_level_int == 6 || noise_level_int == 7)
            {

                dnr_std_thrd = 0;

                for(bin = 0; bin < 16; bin++)
                    md_lut_new[bin] = md_lut_level2[bin];
                md_adjust_level = 10 - 1;
                psnr_std_level_thrd = 0x10;


            }



        }
        else if(noise_strength == 1)    // sd_new med ***
        {
            if(noise_level_int == 0 || noise_level_int == 1 || noise_level_int == 2)
            {

                dnr_std_thrd = 3; //

                for(bin = 0; bin < 16; bin++)
                    md_lut_new[bin] = md_lut_level00[bin];
                md_adjust_level = 2;

                psnr_std_level_thrd = 0x13;


            }

            else if(noise_level_int == 3)
            {

                dnr_std_thrd = 5;

                for(bin = 0; bin < 16; bin++)
                    md_lut_new[bin] = md_lut_level1[bin];
                md_adjust_level = 2;
                psnr_std_level_thrd = 0x20;


            }

            else if(noise_level_int == 4)
            {

                dnr_std_thrd = 8;

                md_adjust_level = 7;
                for(bin = 0; bin < 16; bin++)
                    md_lut_new[bin] = md_lut_level2[bin];
                psnr_std_level_thrd = 0x10;


            }
            else if(noise_level_int == 5)
            {

                dnr_std_thrd = 8;

                md_adjust_level = 8;
                for(bin = 0; bin < 16; bin++)
                    md_lut_new[bin] = md_lut_level2[bin];
                psnr_std_level_thrd = 0x10;


            }
            else if(noise_level_int == 6 || noise_level_int == 7)
            {

                dnr_std_thrd = 0;

                for(bin = 0; bin < 16; bin++)
                    md_lut_new[bin] = md_lut_level2[bin];
                md_adjust_level = 10;
                psnr_std_level_thrd = 0x10;


            }




        }
        else if(noise_strength == 2)     // sd_new high ***
        {
            if(noise_level_int == 0 || noise_level_int == 1 || noise_level_int == 2)
            {

                dnr_std_thrd = 0; //

                for(bin = 0; bin < 16; bin++)
                    md_lut_new[bin] = md_lut_level00[bin];
                md_adjust_level = 2 + 1;

                psnr_std_level_thrd = 0x13;


            }

            else if(noise_level_int == 3)
            {

                dnr_std_thrd = 5;

                for(bin = 0; bin < 16; bin++)
                    md_lut_new[bin] = md_lut_level1[bin];
                md_adjust_level = 2 + 1;
                psnr_std_level_thrd = 0x20;


            }

            else if(noise_level_int == 4)
            {

                dnr_std_thrd = 8;

                md_adjust_level = 7 + 1;
                for(bin = 0; bin < 16; bin++)
                    md_lut_new[bin] = md_lut_level2[bin];
                psnr_std_level_thrd = 0x10;


            }
            else if(noise_level_int == 5)
            {

                dnr_std_thrd = 8;

                md_adjust_level = 8 + 1;
                for(bin = 0; bin < 16; bin++)
                    md_lut_new[bin] = md_lut_level2[bin];
                psnr_std_level_thrd = 0x10;


            }
            else if(noise_level_int == 6 || noise_level_int == 7)
            {

                dnr_std_thrd = 0;

                for(bin = 0; bin < 16; bin++)
                    md_lut_new[bin] = md_lut_level2[bin];
                md_adjust_level = 10 + 1;
                psnr_std_level_thrd = 0x10;


            }




        }
        else if(noise_strength == 3)
        {
            if(noise_level_int == 0 || noise_level_int == 1 || noise_level_int == 2)
            {

                dnr_std_thrd = 0; //

                for(bin = 0; bin < 16; bin++)
                    md_lut_new[bin] = md_lut_level00[bin];
                md_adjust_level = 2 + 2;

                psnr_std_level_thrd = 0x13;


            }

            else if(noise_level_int == 3)
            {

                dnr_std_thrd = 5;

                for(bin = 0; bin < 16; bin++)
                    md_lut_new[bin] = md_lut_level1[bin];
                md_adjust_level = 2 + 2;
                psnr_std_level_thrd = 0x20;


            }

            else if(noise_level_int == 4)
            {

                dnr_std_thrd = 8;

                md_adjust_level = 7 + 2;
                for(bin = 0; bin < 16; bin++)
                    md_lut_new[bin] = md_lut_level2[bin];
                psnr_std_level_thrd = 0x10;


            }
            else if(noise_level_int == 5)
            {

                dnr_std_thrd = 8;

                md_adjust_level = 8 + 2;
                for(bin = 0; bin < 16; bin++)
                    md_lut_new[bin] = md_lut_level2[bin];
                psnr_std_level_thrd = 0x10;


            }
            else if(noise_level_int == 6 || noise_level_int == 7)
            {

                dnr_std_thrd = 0;

                for(bin = 0; bin < 16; bin++)
                    md_lut_new[bin] = md_lut_level2[bin];
                md_adjust_level = 10 + 2;
                psnr_std_level_thrd = 0x10;


            }




        }
        else if(noise_strength == 4)
        {
            if(noise_level_int == 0 || noise_level_int == 1 || noise_level_int == 2)
            {

                dnr_std_thrd = 0; //

                for(bin = 0; bin < 16; bin++)
                    md_lut_new[bin] = md_lut_level00[bin];
                md_adjust_level = 2 + 3;

                psnr_std_level_thrd = 0x13;


            }

            else if(noise_level_int == 3)
            {

                dnr_std_thrd = 5;

                for(bin = 0; bin < 16; bin++)
                    md_lut_new[bin] = md_lut_level1[bin];
                md_adjust_level = 2 + 3;
                psnr_std_level_thrd = 0x20;


            }

            else if(noise_level_int == 4)
            {

                dnr_std_thrd = 8;

                md_adjust_level = 7 + 3;
                for(bin = 0; bin < 16; bin++)
                    md_lut_new[bin] = md_lut_level2[bin];
                psnr_std_level_thrd = 0x10;


            }
            else if(noise_level_int == 5)
            {

                dnr_std_thrd = 8;

                md_adjust_level = 8 + 3;
                for(bin = 0; bin < 16; bin++)
                    md_lut_new[bin] = md_lut_level2[bin];
                psnr_std_level_thrd = 0x10;


            }
            else if(noise_level_int == 6 || noise_level_int == 7)
            {

                dnr_std_thrd = 0;

                for(bin = 0; bin < 16; bin++)
                    md_lut_new[bin] = md_lut_level2[bin];
                md_adjust_level = 10 + 3;
                psnr_std_level_thrd = 0x10;


            }




        }
        else if(noise_strength == 5)    //
        {
            if(noise_level_int == 0)
            {

                dnr_std_thrd = 0 + 5; //

                for(bin = 0; bin < 16; bin++)
                    md_lut_new[bin] = md_lut_level00[bin];
                md_adjust_level = 1 - 1;
                psnr_std_level_thrd = 0x30;

            }
            else if(noise_level_int == 1)
            {

                dnr_std_thrd = 0;

                for(bin = 0; bin < 16; bin++)
                    md_lut_new[bin] = md_lut_level00[bin];
                md_adjust_level = 4 - 2;
                psnr_std_level_thrd = 0x30;


            }
            else if(noise_level_int == 2)
            {

                dnr_std_thrd = 5;

                for(bin = 0; bin < 16; bin++)
                    md_lut_new[bin] = md_lut_level1[bin];
                md_adjust_level = 7 - 2;
                psnr_std_level_thrd = 0x30;


            }
            else
            {

                dnr_std_thrd = 5;

                for(bin = 0; bin < 16; bin++)
                    md_lut_new[bin] = md_lut_level1[bin];
                md_adjust_level = 14 - 2;
                psnr_std_level_thrd = 0x25;

            }



        }

        else if(noise_strength == 6)    // hd_new low
        {
            if(noise_level_int == 0)
            {

                dnr_std_thrd = 0 + 2; //

                for(bin = 0; bin < 16; bin++)
                    md_lut_new[bin] = md_lut_level00[bin];
                md_adjust_level = 1 - 1;
                psnr_std_level_thrd = 0x30;

            }
            else if(noise_level_int == 1)
            {

                dnr_std_thrd = 0;

                for(bin = 0; bin < 16; bin++)
                    md_lut_new[bin] = md_lut_level00[bin];
                md_adjust_level = 4 - 1;
                psnr_std_level_thrd = 0x30;


            }
            else if(noise_level_int == 2)
            {

                dnr_std_thrd = 5;

                for(bin = 0; bin < 16; bin++)
                    md_lut_new[bin] = md_lut_level1[bin];
                md_adjust_level = 7 - 1;
                psnr_std_level_thrd = 0x30;


            }
            else
            {

                dnr_std_thrd = 5;

                for(bin = 0; bin < 16; bin++)
                    md_lut_new[bin] = md_lut_level1[bin];
                md_adjust_level = 14 - 1;
                psnr_std_level_thrd = 0x25;

            }



        }
        else if(noise_strength == 7)    // hd_new med, ***
        {
            if(noise_level_int == 0)
            {

                dnr_std_thrd = 0;

                for(bin = 0; bin < 16; bin++)
                    md_lut_new[bin] = md_lut_level00[bin];
                md_adjust_level = 1;
                psnr_std_level_thrd = 0x30;

            }
            else if(noise_level_int == 1)
            {

                dnr_std_thrd = 0;

                for(bin = 0; bin < 16; bin++)
                    md_lut_new[bin] = md_lut_level00[bin];
                md_adjust_level = 4;
                psnr_std_level_thrd = 0x30;


            }
            else if(noise_level_int == 2)
            {

                dnr_std_thrd = 5;

                for(bin = 0; bin < 16; bin++)
                    md_lut_new[bin] = md_lut_level1[bin];
                md_adjust_level = 7;
                psnr_std_level_thrd = 0x30;


            }
            else
            {

                dnr_std_thrd = 5;

                for(bin = 0; bin < 16; bin++)
                    md_lut_new[bin] = md_lut_level1[bin];
                md_adjust_level = 14;
                psnr_std_level_thrd = 0x25;

            }



        }
        else if(noise_strength == 8)    // hd high
        {

            if(noise_level_int == 0)
            {

                dnr_std_thrd = 0;

                for(bin = 0; bin < 16; bin++)
                    md_lut_new[bin] = md_lut_level00[bin];
                md_adjust_level = 1 + 1;
                psnr_std_level_thrd = 0x30;

            }
            else if(noise_level_int == 1)
            {

                dnr_std_thrd = 0;

                for(bin = 0; bin < 16; bin++)
                    md_lut_new[bin] = md_lut_level00[bin];
                md_adjust_level = 4 + 1;
                psnr_std_level_thrd = 0x30;


            }
            else if(noise_level_int == 2)
            {

                dnr_std_thrd = 5;

                for(bin = 0; bin < 16; bin++)
                    md_lut_new[bin] = md_lut_level1[bin];
                md_adjust_level = 7 + 1;
                psnr_std_level_thrd = 0x30;


            }
            else
            {

                dnr_std_thrd = 5;

                for(bin = 0; bin < 16; bin++)
                    md_lut_new[bin] = md_lut_level1[bin];
                md_adjust_level = 14 + 1;
                psnr_std_level_thrd = 0x25;

            }





        }

        else if(noise_strength == 9)
        {

            if(noise_level_int == 0)
            {

                dnr_std_thrd = 0;

                for(bin = 0; bin < 16; bin++)
                    md_lut_new[bin] = md_lut_level00[bin];
                md_adjust_level = 1 + 2;
                psnr_std_level_thrd = 0x30;

            }
            else if(noise_level_int == 1)
            {

                dnr_std_thrd = 0;

                for(bin = 0; bin < 16; bin++)
                    md_lut_new[bin] = md_lut_level00[bin];
                md_adjust_level = 4 + 2;
                psnr_std_level_thrd = 0x30;


            }
            else if(noise_level_int == 2)
            {

                dnr_std_thrd = 5;

                for(bin = 0; bin < 16; bin++)
                    md_lut_new[bin] = md_lut_level1[bin];
                md_adjust_level = 7 + 2;
                psnr_std_level_thrd = 0x30;


            }
            else
            {

                dnr_std_thrd = 5;

                for(bin = 0; bin < 16; bin++)
                    md_lut_new[bin] = md_lut_level1[bin];
                md_adjust_level = 14 + 1;
                psnr_std_level_thrd = 0x25;

            }





        }

        //MApi_XC_W2BYTEMSK(REG_SC_BK0E_30_L, md_pre_gain, 0x003F);
        //MApi_XC_W2BYTEMSK(REG_SC_BK0E_30_L, (md_pre_shift<<8), 0x0700);

        MS_U16 hisdnr_en;
        hisdnr_en = MApi_XC_R2BYTEMSK(REG_SC_BK0E_01_L, BIT(4));




        hismatch_level_thrd_iir = (iir_strength * hismatch_level_thrd + (8 - iir_strength) * hismatch_level_thrd_iir) / 8 ;
        rounding_bit = (iir_strength * hismatch_level_thrd + (8 - iir_strength) * hismatch_level_thrd_iir) & 0x04;



        MApi_XC_W2BYTEMSK(REG_SC_BK0E_03_L, (0) << 8 , 0xFF00);




        dnr_std_thrd_iir = (iir_strength * dnr_std_thrd + (8 - iir_strength) * dnr_std_thrd_iir) / 8 ;
        rounding_bit = (iir_strength * dnr_std_thrd + (8 - iir_strength) * dnr_std_thrd_iir) & 0x04;

        //if(pause_ack == 2)
        //  dnr_std_thrd_iir= 5 ;

        MApi_XC_W2BYTEMSK(REG_SC_BK0E_04_L, ((MS_U16)(dnr_std_thrd_iir + rounding_bit / 4)) << 4 , 0x00F0);






        md_adjust_level_iir = (iir_strength * md_adjust_level + (8 - iir_strength) * md_adjust_level_iir) / 8.0  ;
        rounding_bit = (iir_strength * md_adjust_level + (8 - iir_strength) * md_adjust_level_iir) & 0x04;



        MApi_XC_W2BYTEMSK(REG_SC_BK0E_02_L, ((MS_U16)(md_adjust_level_iir + rounding_bit / 4)) << 4 , 0x00F0);

        for(bin = 0; bin < 16; bin++)
            rounding_array[bin] = 0;


        for(bin = 0; bin < 16; bin++)
            md_lut_iir[bin] = (iir_strength * md_lut_new[bin] + (8 - iir_strength) * md_lut_iir[bin]) / 8.0  ;
        for(bin = 0; bin < 16; bin++)
            rounding_array[bin] = (iir_strength * md_lut_new[bin] + (8 - iir_strength) * md_lut_iir[bin]) & 0x4 ;


        if(hisdnr_en == 0)
        {
            MApi_XC_W2BYTEMSK(REG_SC_BK06_40_L, ((MS_U16)(md_lut_default[0]))    , 0x000F);
            MApi_XC_W2BYTEMSK(REG_SC_BK06_40_L, ((MS_U16)(md_lut_default[1])) << 4 , 0x00F0);
            MApi_XC_W2BYTEMSK(REG_SC_BK06_40_L, ((MS_U16)(md_lut_default[2])) << 8 , 0x0F00);
            MApi_XC_W2BYTEMSK(REG_SC_BK06_40_L, ((MS_U16)(md_lut_default[3])) << 12, 0xF000);
            MApi_XC_W2BYTEMSK(REG_SC_BK06_41_L, ((MS_U16)(md_lut_default[4]))    , 0x000F);
            MApi_XC_W2BYTEMSK(REG_SC_BK06_41_L, ((MS_U16)(md_lut_default[5])) << 4 , 0x00F0);
            MApi_XC_W2BYTEMSK(REG_SC_BK06_41_L, ((MS_U16)(md_lut_default[6])) << 8 , 0x0F00);
            MApi_XC_W2BYTEMSK(REG_SC_BK06_41_L, ((MS_U16)(md_lut_default[7])) << 12, 0xF000);
            MApi_XC_W2BYTEMSK(REG_SC_BK06_42_L, ((MS_U16)(md_lut_default[8]))    , 0x000F);
            MApi_XC_W2BYTEMSK(REG_SC_BK06_42_L, ((MS_U16)(md_lut_default[9])) << 4 , 0x00F0);
            MApi_XC_W2BYTEMSK(REG_SC_BK06_42_L, ((MS_U16)(md_lut_default[10])) << 8 , 0x0F00);
            MApi_XC_W2BYTEMSK(REG_SC_BK06_42_L, ((MS_U16)(md_lut_default[11])) << 12, 0xF000);
            MApi_XC_W2BYTEMSK(REG_SC_BK06_43_L, ((MS_U16)(md_lut_default[12]))    , 0x000F);
            MApi_XC_W2BYTEMSK(REG_SC_BK06_43_L, ((MS_U16)(md_lut_default[13])) << 4 , 0x00F0);
            MApi_XC_W2BYTEMSK(REG_SC_BK06_43_L, ((MS_U16)(md_lut_default[14])) << 8 , 0x0F00);
            MApi_XC_W2BYTEMSK(REG_SC_BK06_43_L, ((MS_U16)(md_lut_default[15])) << 12, 0xF000);
        }
        else
        {
            MApi_XC_W2BYTEMSK(REG_SC_BK06_40_L, ((MS_U16)(md_lut_iir[0] + rounding_array[0] / 4))    , 0x000F);
            MApi_XC_W2BYTEMSK(REG_SC_BK06_40_L, ((MS_U16)(md_lut_iir[1] + rounding_array[1] / 4)) << 4 , 0x00F0);
            MApi_XC_W2BYTEMSK(REG_SC_BK06_40_L, ((MS_U16)(md_lut_iir[2] + rounding_array[2] / 4)) << 8 , 0x0F00);
            MApi_XC_W2BYTEMSK(REG_SC_BK06_40_L, ((MS_U16)(md_lut_iir[3] + rounding_array[3] / 4)) << 12, 0xF000);
            MApi_XC_W2BYTEMSK(REG_SC_BK06_41_L, ((MS_U16)(md_lut_iir[4] + rounding_array[4] / 4))    , 0x000F);
            MApi_XC_W2BYTEMSK(REG_SC_BK06_41_L, ((MS_U16)(md_lut_iir[5] + rounding_array[5] / 4)) << 4 , 0x00F0);
            MApi_XC_W2BYTEMSK(REG_SC_BK06_41_L, ((MS_U16)(md_lut_iir[6] + rounding_array[6] / 4)) << 8 , 0x0F00);
            MApi_XC_W2BYTEMSK(REG_SC_BK06_41_L, ((MS_U16)(md_lut_iir[7] + rounding_array[7] / 4)) << 12, 0xF000);
            MApi_XC_W2BYTEMSK(REG_SC_BK06_42_L, ((MS_U16)(md_lut_iir[8] + rounding_array[8] / 4))    , 0x000F);
            MApi_XC_W2BYTEMSK(REG_SC_BK06_42_L, ((MS_U16)(md_lut_iir[9] + rounding_array[9] / 4)) << 4 , 0x00F0);
            MApi_XC_W2BYTEMSK(REG_SC_BK06_42_L, ((MS_U16)(md_lut_iir[10] + rounding_array[10] / 4)) << 8 , 0x0F00);
            MApi_XC_W2BYTEMSK(REG_SC_BK06_42_L, ((MS_U16)(md_lut_iir[11] + rounding_array[11] / 4)) << 12, 0xF000);
            MApi_XC_W2BYTEMSK(REG_SC_BK06_43_L, ((MS_U16)(md_lut_iir[12] + rounding_array[12] / 4))    , 0x000F);
            MApi_XC_W2BYTEMSK(REG_SC_BK06_43_L, ((MS_U16)(md_lut_iir[13] + rounding_array[13] / 4)) << 4 , 0x00F0);
            MApi_XC_W2BYTEMSK(REG_SC_BK06_43_L, ((MS_U16)(md_lut_iir[14] + rounding_array[14] / 4)) << 8 , 0x0F00);
            MApi_XC_W2BYTEMSK(REG_SC_BK06_43_L, ((MS_U16)(md_lut_iir[15] + rounding_array[15] / 4)) << 12, 0xF000);


        }




        psnr_std_level_thrd_iir = (iir_strength * psnr_std_level_thrd + (8 - iir_strength) * psnr_std_level_thrd_iir) / 8.0 ;
        rounding_bit = (iir_strength * psnr_std_level_thrd + (8 - iir_strength) * psnr_std_level_thrd_iir) & 0x4;

        MApi_XC_W2BYTEMSK(REG_SC_BK0E_18_L, ((MS_U16)(psnr_std_level_thrd_iir + rounding_bit / 4)) << 8 , 0xFF00);

    }

}

static MS_U16 g_u16BK0A_24 = 0;

static MS_U16 g_u16BK0C_10 = 0;
static MS_U16 g_u16BK0C_39 = 0;
static MS_U16 g_u16BK0C_3A = 0;
static MS_U16 g_u16BK0C_40 = 0;
static MS_U16 g_u16BK0C_41 = 0;
static MS_U16 g_u16BK0C_42 = 0;

static MS_U16 g_u16BK26_10 = 0;
static MS_U16 g_u16BK26_11 = 0;
static MS_U16 g_u16BK26_12 = 0;
static MS_U16 g_u16BK26_13 = 0;
static MS_U16 g_u16BK26_18 = 0;



/******************************************************************************/
///Film mode control for Any Candence
///@param bEnable \     IN: Enable/Disable control
/******************************************************************************/
void MApi_XC_FilmMode_AnyCandence_Enable(MS_BOOL bEnable)
{
    MApi_XC_W2BYTEMSK(REG_SC_BK0A_24_L, (bEnable ? (BIT(15)) : 0), BIT(15));
}

/******************************************************************************/
///De-Blocking Control
///@param bEnable \     IN: Enable/Disable control
/******************************************************************************/
void MApi_XC_DBK_Control(MS_BOOL bEnable, XC_CTRL_LEVEL eLevel)
{
    MApi_XC_W2BYTEMSK(REG_SC_BK0C_10_L, bEnable, BIT(0));
    if(bEnable)
    {
        switch(eLevel)
        {
            default:
            case Level_Low:
                MApi_XC_W2BYTEMSK(REG_SC_BK0C_10_L, (0x1F) << 8, 0xFF00);
                MApi_XC_W2BYTE(REG_SC_BK0C_39_L, 0x0709);
                MApi_XC_W2BYTEMSK(REG_SC_BK0C_3A_L, 0x06, 0x00FF);
                MApi_XC_W2BYTE(REG_SC_BK0C_40_L, 0x0563);
                MApi_XC_W2BYTE(REG_SC_BK0C_41_L, 0x0442);
                MApi_XC_W2BYTE(REG_SC_BK0C_42_L, 0x0412);
                break;
            case Level_MID:
                MApi_XC_W2BYTEMSK(REG_SC_BK0C_10_L, (0x40) << 8, 0xFF00);
                MApi_XC_W2BYTE(REG_SC_BK0C_39_L, 0x0709);
                MApi_XC_W2BYTEMSK(REG_SC_BK0C_3A_L, 0x05, 0x00FF);
                MApi_XC_W2BYTE(REG_SC_BK0C_40_L, 0x0767);
                MApi_XC_W2BYTE(REG_SC_BK0C_41_L, 0x0742);
                MApi_XC_W2BYTE(REG_SC_BK0C_42_L, 0x0912);
                break;
            case Level_High:
                MApi_XC_W2BYTEMSK(REG_SC_BK0C_10_L, (0x80) << 8, 0xFF00);
                MApi_XC_W2BYTE(REG_SC_BK0C_39_L, 0x0709);
                MApi_XC_W2BYTEMSK(REG_SC_BK0C_3A_L, 0x05, 0x00FF);
                MApi_XC_W2BYTE(REG_SC_BK0C_40_L, 0x0767);
                MApi_XC_W2BYTE(REG_SC_BK0C_41_L, 0x0742);
                MApi_XC_W2BYTE(REG_SC_BK0C_42_L, 0x0902);
                break;
        }
    }
}

/******************************************************************************/
///De-Mosquito Control
///@param bEnable \     IN: Enable/Disable control
/******************************************************************************/
void MApi_XC_DMS_Control(MS_BOOL bEnable, XC_CTRL_LEVEL eLevel)
{
    MApi_XC_W2BYTEMSK(REG_SC_BK26_10_L, bEnable, BIT(0));
    if(bEnable)
    {
        switch(eLevel)
        {
            default:
            case Level_Low:
                MApi_XC_W2BYTEMSK(REG_SC_BK26_10_L, 0x1000, 0xFF00);
                MApi_XC_W2BYTE(REG_SC_BK26_11_L, 0x0110);
                MApi_XC_W2BYTE(REG_SC_BK26_12_L, 0x0000);
                MApi_XC_W2BYTE(REG_SC_BK26_13_L, 0x0006);
                MApi_XC_W2BYTE(REG_SC_BK26_18_L, 0x0020);
                break;
            case Level_MID:
                break;
            case Level_High:
                MApi_XC_W2BYTEMSK(REG_SC_BK26_10_L, 0x1500, 0xFF00);
                MApi_XC_W2BYTE(REG_SC_BK26_11_L, 0x0110);
                MApi_XC_W2BYTE(REG_SC_BK26_12_L, 0x0000);
                MApi_XC_W2BYTE(REG_SC_BK26_13_L, 0x0110);
                MApi_XC_W2BYTE(REG_SC_BK26_18_L, 0x0000);
                break;
        }
    }

}

/******************************************************************************/
///Store the Current value
/******************************************************************************/
void MApi_XC_SotreCurrentValue(void)
{
    //for AnyCandence Film Mode
    g_u16BK0A_24 = MApi_XC_R2BYTEMSK(REG_SC_BK0A_24_L, BIT(15));

    //for DBK
    g_u16BK0C_10 = MApi_XC_R2BYTE(REG_SC_BK0C_10_L);
    g_u16BK0C_39 = MApi_XC_R2BYTE(REG_SC_BK0C_39_L);
    g_u16BK0C_3A = MApi_XC_R2BYTE(REG_SC_BK0C_3A_L);
    g_u16BK0C_40 = MApi_XC_R2BYTE(REG_SC_BK0C_40_L);
    g_u16BK0C_41 = MApi_XC_R2BYTE(REG_SC_BK0C_41_L);
    g_u16BK0C_42 = MApi_XC_R2BYTE(REG_SC_BK0C_42_L);

    //for DMS
    g_u16BK26_10 = MApi_XC_R2BYTE(REG_SC_BK26_10_L);
    g_u16BK26_11 = MApi_XC_R2BYTE(REG_SC_BK26_11_L);
    g_u16BK26_12 = MApi_XC_R2BYTE(REG_SC_BK26_12_L);
    g_u16BK26_13 = MApi_XC_R2BYTE(REG_SC_BK26_13_L);
    g_u16BK26_18 = MApi_XC_R2BYTE(REG_SC_BK26_18_L);
}

/******************************************************************************/
///Store the Current value
/******************************************************************************/
void MApi_XC_ReSotreToDefault(void)
{
    //for AnyCandence Film Mode
    MApi_XC_W2BYTEMSK(REG_SC_BK0A_24_L, g_u16BK0A_24, BIT(15));

    //for DBK
    MApi_XC_W2BYTE(REG_SC_BK0C_10_L, g_u16BK0C_10);
    MApi_XC_W2BYTE(REG_SC_BK0C_39_L, g_u16BK0C_39);
    MApi_XC_W2BYTE(REG_SC_BK0C_3A_L, g_u16BK0C_3A);
    MApi_XC_W2BYTE(REG_SC_BK0C_40_L, g_u16BK0C_40);
    MApi_XC_W2BYTE(REG_SC_BK0C_41_L, g_u16BK0C_41);
    MApi_XC_W2BYTE(REG_SC_BK0C_42_L, g_u16BK0C_42);

    //for DMS
    MApi_XC_W2BYTE(REG_SC_BK26_10_L, g_u16BK26_10);
    MApi_XC_W2BYTE(REG_SC_BK26_11_L, g_u16BK26_11);
    MApi_XC_W2BYTE(REG_SC_BK26_12_L, g_u16BK26_12);
    MApi_XC_W2BYTE(REG_SC_BK26_13_L, g_u16BK26_13);
    MApi_XC_W2BYTE(REG_SC_BK26_18_L, g_u16BK26_18);
}


#define ENABLE_SCALER_DEFEATHERING           0x01
#define ENABLE_SCALER_DEFLICKERING           0x02
#define ENABLE_SCALER_DEBOUNCING             0x04
#define ENABLE_SCALER_DYNAMIC_SNR            0x08
#define ENABLE_SCALER_DYNAMIC_DNR            0x10
#define ENABLE_SCALER_DYNAMIC_FILM22         0x20
#define ENABLE_SCALER_DEPRESNR               0x40
#define ENABLE_SCALER_FANTASTICDNR           0x80

#define ENABLE_SCALER_MCDI_DRIVER            0x01
#define ENABLE_SCALER_MCNR_DRIVER            0x02
#define ENABLE_SCALER_FILM_DRIVER            0x04
#define ENABLE_SCALER_JANUS_DHD_DRIVER       0x08
#define ENABLE_SCALER_HISDNR_DRIVER          0x10
#define ENABLE_SCALER_SDHD_DETECT_DRIVER          0x20
#define ENABLE_SCALER_T12_UCDI_DRIVER            0x40
#define ENABLE_SCALER_NEW_SDHD_DETECT_DRIVER 0x80

//-------------------------------------------------------------------------------------------------
/// this function does adaptive tuning periodic
//-------------------------------------------------------------------------------------------------
void MApi_XC_AdaptiveTuning(void)
{
    static MS_U8 u8PreviousVcnt = 0;
    MS_U8 u8CurrentVcnt;
    MS_U8  u8Ctrl,ColorSum;
    MS_U8  u8Ctrl2;
    MS_U32 u32MotionValue,u32MotionValue2,OOSRMC,OOSRF,ISR_NONMV0,SmallMoving,VerticalMoving,HorizontalMoving,TotalFeather,TotalMvComfirm;
    MS_U32 uComplex;
    MS_U8 u8MotLvl;
    XC_ApiStatus stXCStatus;

    //
    // Check VCnt
    //
    u8CurrentVcnt = MDrv_SC_read_v_cnt();
    if(u8PreviousVcnt == u8CurrentVcnt)
    {
        return;
    }
    else
    {
        u8PreviousVcnt = u8CurrentVcnt;
    }

    //
    // Get adaptive function control
    //
    u8Ctrl  = MDrv_SC_get_adaptive_ctrl();
    u8Ctrl2 = MDrv_SC_get_adaptive_ctrl2();

    //
    // Get motion value
    //
    u32MotionValue = MDrv_SC_read_motion_value1();
    u32MotionValue2 = MDrv_SC_read_motion_value2();

    //
    // Get UCDi status
    //
    OOSRMC = MDrv_SC_T12_UC_InSearchRange_MvComfirm();
    OOSRF = MDrv_SC_T12_UC_InSearchRange_Feather();
    ISR_NONMV0 = MDrv_SC_T12_UC_InSearchRange_nonMv0Comfirm();
    SmallMoving = MDrv_SC_T12_UC_SmallMoving();
    VerticalMoving = MDrv_SC_A5_KFC_IsVerticalMoving();

    HorizontalMoving = MDrv_SC_A5_IsHorizontalMoving();
    TotalFeather = MDrv_SC_A5_TotalFeather();
    TotalMvComfirm = MDrv_SC_A5_TotalMvComfirm();
    ColorSum = MDrv_SC_A5_ColorPixelSum();
    uComplex = MDrv_SC_A5_ComplexSum();

    //printf("===OOSRMC==%x=\n",OOSRMC);
    //printf("===OOSRF==%x=\n",OOSRF);
    //printf("===ISR_NONMV0==%x=\n",ISR_NONMV0);
    //
    // Adaptive functions
    //

    if(!(u8Ctrl2 & ENABLE_SCALER_T12_UCDI_DRIVER))
    {
        if(u8Ctrl2 & ENABLE_SCALER_MCDI_DRIVER)
        {
            MDrv_SC_mcdi_driver(u32MotionValue2);
        }
        else
        {
            u8MotLvl = MDrv_SC_motion_level(u32MotionValue);
            MDrv_SC_mcdi_driver_new(u32MotionValue2, u8MotLvl);
        }
    }

    //
    // T12 UCDi function
    //
    if(u8Ctrl2 & ENABLE_SCALER_T12_UCDI_DRIVER)
    {
        //u8MotLvl = MDrv_SC_motion_level(u32MotionValue);
        //MDrv_SC_T12_UCDi_OP1_HisWeight(u8MotLvl);
        MDrv_SC_T12_UCDi_FeatherValueGain(OOSRMC, OOSRF, ISR_NONMV0);
        //MDrv_SC_T12_DIPF_TemporalGainControl(OOSRMC, OOSRF, ISR_NONMV0);
        MDrv_SC_T12_UCDi_VerticalMovingUdRatio(OOSRMC, OOSRF, ISR_NONMV0);
        MDrv_SC_T12_UCDi_ComplexOffset(OOSRMC, OOSRF, ISR_NONMV0 );
        MDrv_SC_T12_UCDi_W2SadGain(OOSRMC, OOSRF, ISR_NONMV0);
        MDrv_SC_T12_KFC_8bitMotionGain(SmallMoving);
        MDrv_SC_T12_UCDi_EODiWGain(OOSRMC, OOSRF, ISR_NONMV0);
        MDrv_SC_T12_UCDi_HistoryRatio(OOSRMC, OOSRF, ISR_NONMV0);
        MDrv_SC_A5_UCDi_OP1_HisWeight(u32MotionValue2);
        MDrv_SC_A5_KFC_EODiW(VerticalMoving);
        MDrv_SC_A5_LG107(VerticalMoving, HorizontalMoving, TotalFeather, TotalMvComfirm,ColorSum,u32MotionValue, uComplex);
    }

    if(u8Ctrl2 & ENABLE_SCALER_MCNR_DRIVER)
    {
        MDrv_SC_mcnr_driver();
    }

    if(u8Ctrl2 & ENABLE_SCALER_SDHD_DETECT_DRIVER)
    {
        MDrv_SC_SDHD_DETECT_driver();
    }

    if (u8Ctrl2 & ENABLE_SCALER_NEW_SDHD_DETECT_DRIVER)
    {
        MDrv_SC_NEW_SDHD_DETECT_driver();
    }

    if(u8Ctrl2 & ENABLE_SCALER_FILM_DRIVER)
    {
        MDrv_SC_film_driver();
    }
    else
    {
        MDrv_SC_dynamic_film_mode();    // for T2 and before
    }

    if(u8Ctrl2 & ENABLE_SCALER_JANUS_DHD_DRIVER)
    {
        MDrv_SC_Janus_DHD_driver();
    }

    if(u8Ctrl2 & ENABLE_SCALER_HISDNR_DRIVER)
    {
        MS_BOOL _tmp = FALSE;

        _tmp = MApi_XC_GetStatus(&stXCStatus, MAIN_WINDOW);
        if(_tmp == TRUE)
        {
            MDrv_SC_HISNR_Maria10_driver(stXCStatus.stCropWin.width, stXCStatus.stCropWin.height);
        }
    }

    if(u8Ctrl & ENABLE_SCALER_DEFEATHERING)
    {
        MDrv_SC_de_feathering(u32MotionValue);
    }

    if(u8Ctrl & ENABLE_SCALER_DEFLICKERING)
    {
        MDrv_SC_de_flickering(u32MotionValue);
    }

    if(u8Ctrl & ENABLE_SCALER_DEBOUNCING)
    {
        MDrv_SC_de_bouncing(u32MotionValue2);
    }

    if(u8Ctrl & ENABLE_SCALER_DYNAMIC_SNR)
    {
        MDrv_SC_dynamic_snr(u32MotionValue);
    }

    if(u8Ctrl & ENABLE_SCALER_DYNAMIC_DNR)
    {
        MDrv_SC_dynamic_dnr(u32MotionValue);
    }

    if(u8Ctrl & ENABLE_SCALER_DEPRESNR)
    {
        MDrv_SC_de_pre_snr(u32MotionValue2);
    }

    if(u8Ctrl & ENABLE_SCALER_FANTASTICDNR)
    {
        MDrv_SC_fantastic_dnr();
    }


}


#undef DRV_SCALER_ADAPTIVE_C

