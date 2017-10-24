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

code U8 MST_HSPRule_IP_Index_Main[PQ_HSPRule_IP_NUM_Main]=
{
    PQ_IP_HSP_Y_Main,
    PQ_IP_HSP_C_Main,
    PQ_IP_SRAM2_Main,
    PQ_IP_C_SRAM2_Main,
};


code U8 MST_HSPRule_Array_Main[PQ_HSPRule_NUM_Main][PQ_HSPRule_IP_NUM_Main]=
{
    {//PreV_ScalingDown_Interlace, 0
        PQ_IP_HSP_Y_SRAM_2_4Tap_Main, PQ_IP_HSP_C_C_SRAM_2_Main, PQ_IP_SRAM2_InvSinc4Tc4p4Fc75Fstop124Apass0001Astop40_Main, PQ_IP_C_SRAM2_C121_Main, 
    },
    {//PreV_ScalingDown_Progressive, 1
        PQ_IP_HSP_Y_SRAM_2_4Tap_Main, PQ_IP_HSP_C_C_SRAM_2_Main, PQ_IP_SRAM2_InvSinc4Tc4p4Fc75Fstop124Apass0001Astop40_Main, PQ_IP_C_SRAM2_C121_Main, 
    },
    {//ScalingDown_00x_YUV, 2
        PQ_IP_HSP_Y_SRAM_2_4Tap_Main, PQ_IP_HSP_C_C_SRAM_2_Main, PQ_IP_SRAM2_InvSinc4Tc4p4Fc75Fstop124Apass0001Astop40_Main, PQ_IP_C_SRAM2_C121_Main, 
    },
    {//ScalingDown_00x_RGB, 3
        PQ_IP_HSP_Y_SRAM_2_4Tap_Main, PQ_IP_HSP_C_SRAM_2_4Tap_Main, PQ_IP_SRAM2_InvSinc4Tc4p4Fc75Fstop124Apass0001Astop40_Main, PQ_IP_C_SRAM2_C121_Main, 
    },
    {//ScalingDown_01x_YUV, 4
        PQ_IP_HSP_Y_SRAM_2_4Tap_Main, PQ_IP_HSP_C_C_SRAM_2_Main, PQ_IP_SRAM2_InvSinc4Tc4p4Fc75Fstop124Apass0001Astop40_Main, PQ_IP_C_SRAM2_C121_Main, 
    },
    {//ScalingDown_01x_RGB, 5
        PQ_IP_HSP_Y_SRAM_2_4Tap_Main, PQ_IP_HSP_C_SRAM_2_4Tap_Main, PQ_IP_SRAM2_InvSinc4Tc4p4Fc75Fstop124Apass0001Astop40_Main, PQ_IP_C_SRAM2_C121_Main, 
    },
    {//ScalingDown_02x_YUV, 6
        PQ_IP_HSP_Y_SRAM_2_4Tap_Main, PQ_IP_HSP_C_C_SRAM_2_Main, PQ_IP_SRAM2_InvSinc4Tc4p4Fc75Fstop124Apass0001Astop40_Main, PQ_IP_C_SRAM2_C121_Main, 
    },
    {//ScalingDown_02x_RGB, 7
        PQ_IP_HSP_Y_SRAM_2_4Tap_Main, PQ_IP_HSP_C_SRAM_2_4Tap_Main, PQ_IP_SRAM2_InvSinc4Tc4p4Fc75Fstop124Apass0001Astop40_Main, PQ_IP_C_SRAM2_C121_Main, 
    },
    {//ScalingDown_03x_YUV, 8
        PQ_IP_HSP_Y_SRAM_2_4Tap_Main, PQ_IP_HSP_C_C_SRAM_2_Main, PQ_IP_SRAM2_InvSinc4Tc4p4Fc75Fstop124Apass0001Astop40_Main, PQ_IP_C_SRAM2_C121_Main, 
    },
    {//ScalingDown_03x_RGB, 9
        PQ_IP_HSP_Y_SRAM_2_4Tap_Main, PQ_IP_HSP_C_SRAM_2_4Tap_Main, PQ_IP_SRAM2_InvSinc4Tc4p4Fc75Fstop124Apass0001Astop40_Main, PQ_IP_C_SRAM2_C121_Main, 
    },
    {//ScalingDown_04x_YUV, 10
        PQ_IP_HSP_Y_SRAM_2_4Tap_Main, PQ_IP_HSP_C_C_SRAM_2_Main, PQ_IP_SRAM2_InvSinc4Tc4p4Fc75Fstop124Apass0001Astop40_Main, PQ_IP_C_SRAM2_C121_Main, 
    },
    {//ScalingDown_04x_RGB, 11
        PQ_IP_HSP_Y_SRAM_2_4Tap_Main, PQ_IP_HSP_C_SRAM_2_4Tap_Main, PQ_IP_SRAM2_InvSinc4Tc4p4Fc75Fstop124Apass0001Astop40_Main, PQ_IP_C_SRAM2_C121_Main, 
    },
    {//ScalingDown_05x_YUV, 12
        PQ_IP_HSP_Y_SRAM_2_4Tap_Main, PQ_IP_HSP_C_C_SRAM_2_Main, PQ_IP_SRAM2_InvSinc4Tc4p4Fc75Fstop124Apass0001Astop40_Main, PQ_IP_C_SRAM2_C121_Main, 
    },
    {//ScalingDown_05x_RGB, 13
        PQ_IP_HSP_Y_SRAM_2_4Tap_Main, PQ_IP_HSP_C_SRAM_2_4Tap_Main, PQ_IP_SRAM2_InvSinc4Tc4p4Fc75Fstop124Apass0001Astop40_Main, PQ_IP_C_SRAM2_C121_Main, 
    },
    {//ScalingDown_06x_YUV, 14
        PQ_IP_HSP_Y_SRAM_2_4Tap_Main, PQ_IP_HSP_C_C_SRAM_2_Main, PQ_IP_SRAM2_InvSinc4Tc4p4Fc75Fstop124Apass0001Astop40_Main, PQ_IP_C_SRAM2_C121_Main, 
    },
    {//ScalingDown_06x_RGB, 15
        PQ_IP_HSP_Y_SRAM_2_4Tap_Main, PQ_IP_HSP_C_SRAM_2_4Tap_Main, PQ_IP_SRAM2_InvSinc4Tc4p4Fc75Fstop124Apass0001Astop40_Main, PQ_IP_C_SRAM2_C121_Main, 
    },
    {//ScalingDown_07x_YUV, 16
        PQ_IP_HSP_Y_SRAM_2_4Tap_Main, PQ_IP_HSP_C_C_SRAM_2_Main, PQ_IP_SRAM2_InvSinc4Tc4p4Fc75Fstop124Apass0001Astop40_Main, PQ_IP_C_SRAM2_C121_Main, 
    },
    {//ScalingDown_07x_RGB, 17
        PQ_IP_HSP_Y_SRAM_2_4Tap_Main, PQ_IP_HSP_C_SRAM_2_4Tap_Main, PQ_IP_SRAM2_InvSinc4Tc4p4Fc75Fstop124Apass0001Astop40_Main, PQ_IP_C_SRAM2_C121_Main, 
    },
    {//ScalingDown_08x_YUV, 18
        PQ_IP_HSP_Y_SRAM_2_4Tap_Main, PQ_IP_HSP_C_C_SRAM_2_Main, PQ_IP_SRAM2_InvSinc4Tc4p4Fc75Fstop124Apass0001Astop40_Main, PQ_IP_C_SRAM2_C121_Main, 
    },
    {//ScalingDown_08x_RGB, 19
        PQ_IP_HSP_Y_SRAM_2_4Tap_Main, PQ_IP_HSP_C_SRAM_2_4Tap_Main, PQ_IP_SRAM2_InvSinc4Tc4p4Fc75Fstop124Apass0001Astop40_Main, PQ_IP_C_SRAM2_C121_Main, 
    },
    {//ScalingDown_09x_YUV, 20
        PQ_IP_HSP_Y_SRAM_2_4Tap_Main, PQ_IP_HSP_C_C_SRAM_2_Main, PQ_IP_SRAM2_InvSinc4Tc4p4Fc75Fstop124Apass0001Astop40_Main, PQ_IP_C_SRAM2_C121_Main, 
    },
    {//ScalingDown_09x_RGB, 21
        PQ_IP_HSP_Y_SRAM_2_4Tap_Main, PQ_IP_HSP_C_SRAM_2_4Tap_Main, PQ_IP_SRAM2_InvSinc4Tc4p4Fc75Fstop124Apass0001Astop40_Main, PQ_IP_C_SRAM2_C121_Main, 
    },
    {//ScalingDown_10x_YUV, 22
        PQ_IP_HSP_Y_Bypass_Main, PQ_IP_HSP_C_Bypass_Main, PQ_IP_NULL, PQ_IP_NULL, 
    },
    {//ScalingDown_10x_RGB, 23
        PQ_IP_HSP_Y_Bypass_Main, PQ_IP_HSP_C_Bypass_Main, PQ_IP_NULL, PQ_IP_NULL, 
    },
};
