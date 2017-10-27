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
/*
  Copyright (C) 2006-2009 NXP B.V., All Rights Reserved.
  This source code and any compilation or derivative thereof is the proprietary
  information of NXP B.V. and is confidential in nature. Under no circumstances
  is this software to be  exposed to or placed under an Open Source License of
  any type without the expressed written permission of NXP B.V.
 *
 * \file          tmbslTDA18250InstanceCustom.h
 *
 *                1
 *
 * \date          %modify_time%
 *
 * \brief         Describe briefly the purpose of this file.
 *
 * REFERENCE DOCUMENTS :
 *                TDA18250_Driver_User_Guide.pdf
 *
 * Detailed description may be added here.
 *
 * \section info Change Information
 *
*/

#ifndef _TMBSL_TDA18250_INSTANCE_CUSTOM_H
#define _TMBSL_TDA18250_INSTANCE_CUSTOM_H


#ifdef __cplusplus
extern "C"
{
#endif


/* Driver settings version definition */
#define TMBSL_TDA18250_SETTINGS_CUSTOMER_NUM    3  /* SW Settings Customer Number */
#define TMBSL_TDA18250_SETTINGS_PROJECT_NUM     0  /* SW Settings Project Number  */
#define TMBSL_TDA18250_SETTINGS_MAJOR_VER       0  /* SW Settings Major Version   */
#define TMBSL_TDA18250_SETTINGS_MINOR_VER       19 /* SW Settings Minor Version   */

/*======================================================================================================*/
/*======================================================================================================*/
/* Custom Driver: RF Plan                                                                               */
/*======================================================================================================*/
/*======================================================================================================*/
#define TMBSL_TDA18250_RF_MIN  42000000
#define TMBSL_TDA18250_RF_MAX 1004000000

typedef enum _tmTDA18250BandIndex_t
{
    tmTDA18250_IND_DIG_Band_1 = 0,
    tmTDA18250_IND_DIG_Band_2 = 1,
    tmTDA18250_IND_DIG_Band_3 = 2,
    tmTDA18250_IND_DIG_Band_4 = 3,
    tmTDA18250_IND_DIG_Band_5 = 4,
    tmTDA18250_IND_DIG_Band_6 = 5,
    tmTDA18250_IND_DIG_Band_7 = 6,
    tmTDA18250_IND_Band_Max = 7
} tmTDA18250BandIndex_t, *ptmTDA18250BandIndex_t;


#define tmTDA18250_RF_MAX_DIG_Band_1     67000000
#define tmTDA18250_RF_MAX_DIG_Band_2     93000000
#define tmTDA18250_RF_MAX_DIG_Band_3    134000000
#define tmTDA18250_RF_MAX_DIG_Band_4    184000000
#define tmTDA18250_RF_MAX_DIG_Band_5    249000000
#define tmTDA18250_RF_MAX_DIG_Band_6    335000000
#define tmTDA18250_RF_MAX_DIG_Band_7    1004000000


/*======================================================================================================*/
/*======================================================================================================*/
/* Custom Driver: Get Power Level                                                                       */
/*======================================================================================================*/
/*======================================================================================================*/
/*  { X,     X   }  */
/*  { 0,     1   }  */
/*  {               */
#define TMBSL_TDA18250_INSTANCE_CUSTOM_COMMON_Lvl_RSSI \
    {-175, 25}


#define tmTDA18250_RfFilter_Index_Max tmTDA18250_IND_Band_Max
/*  {    X   ,   X   ,   X   ,               R             }      */
/*  {    0   ,   1   ,   2   ,               f             },     */
/*  {        ,       ,       ,               M             },     */
/*  {        ,       ,       ,               a             },     */
/*  {        ,       ,       ,               x             },     */
/*  {        ,       ,       ,                             },     */
/*  {        ,       ,       ,                             },     */
#define TMBSL_TDA18250_INSTANCE_CUSTOM_COMMON_Lvl_RfFilter         \
{                                                                  \
    {40886077, 333791, -30333, tmTDA18250_RF_MAX_DIG_Band_1},      \
    {41197501, 255563, -17010, tmTDA18250_RF_MAX_DIG_Band_2},      \
    {44759473, 122952,  -5641, tmTDA18250_RF_MAX_DIG_Band_3},      \
    {42648888, 125336,  -4214, tmTDA18250_RF_MAX_DIG_Band_4},      \
    {49875524,  28761,   -899, tmTDA18250_RF_MAX_DIG_Band_5},      \
    {52715155,   8792,   -429, tmTDA18250_RF_MAX_DIG_Band_6},      \
    {52669542,  -3572,    -13, tmTDA18250_RF_MAX_DIG_Band_7}       \
}

#define tmTDA18250_Lvl_AGC2b_Index_Max 13

/*  {    X  ,  X  ,  X ,  u  },   */
/*  {    0  ,  1  ,  2 ,  A  },   */
/*  {       ,     ,    ,  G  },   */
/*  {       ,     ,    ,  C  },   */
/*  {       ,     ,    ,  2  },   */
/*  {       ,     ,    ,  b  },   */
/*  {       ,     ,    ,     },   */
#define TMBSL_TDA18250_INSTANCE_CUSTOM_COMMON_Lvl_AGC2b         \
{                                                               \
    {-262945, 2139,  87,   10},                                 \
    {-264228, 2051,  77,   21},                                 \
    {-267917, 2097,  71,   32},                                 \
    {-270737, 2086,  66,   43},                                 \
    {-272344, 2065,  63,   54},                                 \
    {-275541, 2077,  59,   65},                                 \
    {-277713, 2064,  55,   76},                                 \
    {-280763, 2065,  52,   87},                                 \
    {-282455, 2053,  46,   98},                                 \
    {-283799, 2038,  39,  109},                                 \
    {-284735, 2029,  29,  120},                                 \
    {-283234, 2005,  17,  131},                                 \
    {-281728, 1984,   0,  142}                                  \
}



#define tmTDA18250_Lvl_AGC1_Index_Max 10

/* {    X     ,   X   ,  X ,                  u                 },      */
/* {    0     ,   1   ,  2 ,                  A                 },      */
/* {          ,       ,    ,                  G                 },      */
/* {          ,       ,    ,                  C                 },      */
/* {          ,       ,    ,                  1                 },      */
/* {          ,       ,    ,                                    },      */
#define TMBSL_TDA18250_INSTANCE_CUSTOM_COMMON_Lvl_AGC1_MasterMulti       \
{  /* AGC1 Master with Slave(s) */                                       \
   {         0,      0,   0, tmddTDA18250_AGC1_Gain_Fixed_Minus2dB   },       \
   {         0,      0,   0, tmddTDA18250_AGC1_Gain_Fixed_2dB   },       \
   {         0,      0,   0, tmddTDA18250_AGC1_Gain_Fixed_4_875dB },       \
   {         0,      0,   0, tmddTDA18250_AGC1_Gain_Fixed_6_75dB   },       \
   {         0,      0,   0, tmddTDA18250_AGC1_Gain_Fixed_8_625dB },       \
   {         0,      0,   0, tmddTDA18250_AGC1_Gain_Fixed_10_5dB   },       \
   {         0,      0,   0, tmddTDA18250_AGC1_Gain_Fixed_12_375dB},       \
   {         0,      0,   0, tmddTDA18250_AGC1_Gain_Fixed_14_25dB  },       \
   {         0,      0,   0, tmddTDA18250_AGC1_Gain_Fixed_16_125dB},       \
   {         0,      0,   0, tmddTDA18250_AGC1_Gain_Fixed_18dB  }        \
}


/* {     X    ,    X  ,  X  ,                  u                 },      */
/* {     0    ,    1  ,  2  ,                  A                 },      */
/* {          ,       ,     ,                  G                 },      */
/* {          ,       ,     ,                  C                 },      */
/* {          ,       ,     ,                  1                 },      */
/* {          ,       ,     ,                                    },      */
#define TMBSL_TDA18250_INSTANCE_CUSTOM_COMMON_Lvl_AGC1                    \
{                                                                         \
   /* AGC1 Single Master or Slave */                                      \
   {-189937310,   9940,   36, tmddTDA18250_AGC1_Gain_Fixed_Minus2dB   },       \
   {-154857430,   6133,   68, tmddTDA18250_AGC1_Gain_Fixed_2dB   },       \
   {-138125330,  15613,  -18, tmddTDA18250_AGC1_Gain_Fixed_4_875dB },       \
   {-118264210,  15070,  -19, tmddTDA18250_AGC1_Gain_Fixed_6_75dB   },       \
   { -98653587,  14566,  -21, tmddTDA18250_AGC1_Gain_Fixed_8_625dB },       \
   { -79125105,  13781,  -19, tmddTDA18250_AGC1_Gain_Fixed_10_5dB   },       \
   { -59511242,  12038,  -18, tmddTDA18250_AGC1_Gain_Fixed_12_375dB},       \
   { -40098525,   9927,  -14, tmddTDA18250_AGC1_Gain_Fixed_14_25dB  },       \
   { -20360764,   6353,  -17, tmddTDA18250_AGC1_Gain_Fixed_16_125dB},       \
   {         0,      0,    0, tmddTDA18250_AGC1_Gain_Fixed_18dB  }        \
}


/* {X, X, X, S}, */
/* {0, 1, 2, U}, */
/* { ,  ,  , M}, */
/* { ,  ,  ,  }, */
#define TMBSL_TDA18250_INSTANCE_CUSTOM_COMMON_Lvl_XDIV10                  \
{                                                                         \
   /* Division by 10 coefficients  */                                     \
   {4, 4, 5, 0},   /* RF Filter    */                                     \
   {2, 0, 3, 0},   /* AGC2         */                                     \
   {0, 0, 1, 5},   /* AGC1         */                                     \
   {0, 0, 1, 5},   /* AGC1Multi    */                                     \
   {0, 0, 0, 5}    /* Offset Slave */                                     \
}


/*  { X,     X   }  */
/*  { 0,     1   }  */
/*  {               */
#define TMBSL_TDA18250_INSTANCE_CUSTOM_COMMON_Lvl_Offset \
    {90000000, -14584}


/*======================================================================================================*/
/*======================================================================================================*/
/* Custom Driver: Group Delay                                                                           */
/*======================================================================================================*/
/*======================================================================================================*/
#define tmTDA18250_Gd_RCcal_Index_Max 32

#define TMBSL_TDA18250_INSTANCE_CUSTOM_COMMON_Gd_RCcal \
{                                        \
    { 0, tmTDA18250_GD_SIGNE_PLUS,  0 }, \
    { 1, tmTDA18250_GD_SIGNE_PLUS,  0 }, \
    { 2, tmTDA18250_GD_SIGNE_PLUS,  0 }, \
    { 3, tmTDA18250_GD_SIGNE_PLUS,  0 }, \
    { 4, tmTDA18250_GD_SIGNE_PLUS,  0 }, \
    { 5, tmTDA18250_GD_SIGNE_PLUS,  0 }, \
    { 6, tmTDA18250_GD_SIGNE_PLUS,  0 }, \
    { 7, tmTDA18250_GD_SIGNE_PLUS,  0 }, \
    { 8, tmTDA18250_GD_SIGNE_PLUS,  0 }, \
    { 9, tmTDA18250_GD_SIGNE_PLUS,  0 }, \
    {10, tmTDA18250_GD_SIGNE_MINUS, 1 }, \
    {11, tmTDA18250_GD_SIGNE_MINUS, 1 }, \
    {12, tmTDA18250_GD_SIGNE_MINUS, 1 }, \
    {13, tmTDA18250_GD_SIGNE_MINUS, 1 }, \
    {14, tmTDA18250_GD_SIGNE_MINUS, 1 }, \
    {15, tmTDA18250_GD_SIGNE_MINUS, 1 }, \
    {16, tmTDA18250_GD_SIGNE_PLUS,  0 }, \
    {17, tmTDA18250_GD_SIGNE_MINUS, 1 }, \
    {18, tmTDA18250_GD_SIGNE_MINUS, 1 }, \
    {19, tmTDA18250_GD_SIGNE_PLUS,  0 }, \
    {20, tmTDA18250_GD_SIGNE_PLUS,  0 }, \
    {21, tmTDA18250_GD_SIGNE_PLUS,  0 }, \
    {22, tmTDA18250_GD_SIGNE_PLUS,  0 }, \
    {23, tmTDA18250_GD_SIGNE_PLUS,  0 }, \
    {24, tmTDA18250_GD_SIGNE_PLUS,  0 }, \
    {25, tmTDA18250_GD_SIGNE_PLUS,  0 }, \
    {26, tmTDA18250_GD_SIGNE_PLUS,  0 }, \
    {27, tmTDA18250_GD_SIGNE_PLUS,  0 }, \
    {28, tmTDA18250_GD_SIGNE_PLUS,  0 }, \
    {29, tmTDA18250_GD_SIGNE_PLUS,  0 }, \
    {30, tmTDA18250_GD_SIGNE_PLUS,  0 }, \
    {31, tmTDA18250_GD_SIGNE_PLUS,  0 }  \
}

/*======================================================================================================*/
/*======================================================================================================*/
/* Custom Driver Instance Parameters: AGC_TOP                                                           */
/*======================================================================================================*/
/*======================================================================================================*/
#define TMBSL_TDA18250_INSTANCE_CUSTOM_COMMON_AGC_TOP                                                    \
    {   /* AGC1_Gud */                                                                                   \
        0x06,                                               /* AGC1_Gud_Up    */                         \
        0x03                                                /* AGC1_Gud_Down  */                         \
    },                                                                                                   \
    {   /* AGC1_Aup */                                                                                   \
        0x02,                                               /* AGC1_Aup_Minus2dB */                      \
        0x03,                                               /* AGC1_Aup_2dB      */                      \
        0x04,                                               /* AGC1_Aup_4_875dB  */                      \
        0x04,                                               /* AGC1_Aup_6_75dB   */                      \
        0x05,                                               /* AGC1_Aup_8_625dB  */                      \
        0x04,                                               /* AGC1_Aup_10_5dB   */                      \
        0x05,                                               /* AGC1_Aup_12_375dB */                      \
        0x05,                                               /* AGC1_Aup_14_25dB  */                      \
        0x05                                                /* AGC1_Aup_16_125dB */                      \
    },                                                                                                   \
    {   /* AGC1_Adown */                                                                                 \
        0x01,                                               /* AGC1_Ado_2dB      */                      \
        0x02,                                               /* AGC1_Ado_4_875dB  */                      \
        0x03,                                               /* AGC1_Ado_6_75dB   */                      \
        0x04,                                               /* AGC1_Ado_8_625dB  */                      \
        0x06,                                               /* AGC1_Ado_10_5dB   */                      \
        0x05,                                               /* AGC1_Ado_12_375dB */                      \
        0x05,                                               /* AGC1_Ado_14_25dB  */                      \
        0x05,                                               /* AGC1_Ado_16_125dB */                      \
        0x05                                                /* AGC1_Ado_18dB     */                      \
    },                                                                                                   \
    {   /* AGC2_GdownDetector[][] */                                                                     \
        {   /* BP Filter 0 */                                                                            \
            0x08,                                           /* AGC2_Detector2 */                         \
            0x08,                                           /* AGC2_Detector3 */                         \
            0x0B                                            /* AGC2_Detector4 */                         \
        },                                                                                               \
        {   /* BP Filter 1 */                                                                            \
            0x08,                                           /* AGC2_Detector2 */                         \
            0x08,                                           /* AGC2_Detector3 */                         \
            0x0B                                            /* AGC2_Detector4 */                         \
        },                                                                                               \
        {   /* BP Filter 2 */                                                                            \
            0x08,                                           /* AGC2_Detector2 */                         \
            0x08,                                           /* AGC2_Detector3 */                         \
            0x0B                                            /* AGC2_Detector4 */                         \
        },                                                                                               \
        {   /* BP Filter 3 */                                                                            \
            0x08,                                           /* AGC2_Detector2 */                         \
            0x08,                                           /* AGC2_Detector3 */                         \
            0x0B                                            /* AGC2_Detector4 */                         \
        },                                                                                               \
        {   /* BP Filter 4 */                                                                            \
            0x08,                                           /* AGC2_Detector2 */                         \
            0x08,                                           /* AGC2_Detector3 */                         \
            0x0B                                            /* AGC2_Detector4 */                         \
        },                                                                                               \
        {   /* BP Filter 5 */                                                                            \
            0x08,                                           /* AGC2_Detector2 */                         \
            0x08,                                           /* AGC2_Detector3 */                         \
            0x0B                                            /* AGC2_Detector4 */                         \
        },                                                                                               \
        {   /* BP Filter 6 */                                                                            \
            0x08,                                           /* AGC2_Detector2 */                         \
            0x08,                                           /* AGC2_Detector3 */                         \
            0x0B                                            /* AGC2_Detector4 */                         \
        },                                                                                               \
        {   /* BP Filter 7 */                                                                            \
            0x08,                                           /* AGC2_Detector2 */                         \
            0x08,                                           /* AGC2_Detector3 */                         \
            0x0B                                            /* AGC2_Detector4 */                         \
        }                                                                                                \
    }


/*======================================================================================================*/
/*======================================================================================================*/
/* Custom Driver Instance Parameters: STD_ARRAY                                                         */
/*======================================================================================================*/
/*======================================================================================================*/
#define TMBSL_TDA18250_INSTANCE_CUSTOM_COMMON_STD_DIGIAL_6MHz                                            \
        {                                                   /* Digital 6MHz   */                         \
            tmTDA18250DigitalChannel,                       /* ChannelType    */                         \
            4000000,                                        /* IF             */                         \
            tmddTDA18250_RFcal_modeOFF_OPEN,                /* RFcal_modeOFF  */                         \
            tmTDA18250_LPF_6MHz,                            /* LPF            */                         \
            tmddTDA18250_PD_NotActivated,                   /* PD_hpf         */                         \
            tmddTDA18250_HPF_Freq_1MHz,                   /* IF_hpf         */                         \
            0,                                              /* notch_select   */                         \
            tmddTDA18250_LPF_Offset_3,                      /* RC_LPFC_Offset */                         \
            tmddTDA18250_LPF_Sign_Minus                      /* RC_LPFC_Sign   */                         \
        }

#define TMBSL_TDA18250_INSTANCE_CUSTOM_COMMON_STD_DIGIAL_7MHz                                            \
        {                                                   /* Digital 7MHz   */                         \
            tmTDA18250DigitalChannel,                       /* ChannelType    */                         \
            4000000,                                        /* IF             */                         \
            tmddTDA18250_RFcal_modeOFF_OPEN,                /* RFcal_modeOFF  */                         \
            tmTDA18250_LPF_7MHz,                            /* LPF            */                         \
            tmddTDA18250_PD_NotActivated,                   /* PD_hpf         */                         \
            tmddTDA18250_HPF_Freq_400KHz,                   /* IF_hpf         */                         \
            0,                                              /* notch_select   */                         \
            tmddTDA18250_LPF_Offset_0,                      /* RC_LPFC_Offset */                         \
            tmddTDA18250_LPF_Sign_Plus                      /* RC_LPFC_Sign   */                         \
        }

#define TMBSL_TDA18250_INSTANCE_CUSTOM_COMMON_STD_DIGIAL_8MHz                                            \
        {                                                   /* Digital 8MHz   */                         \
            tmTDA18250DigitalChannel,                       /* ChannelType    */                         \
            4250000,                                        /* IF             */                         \
            tmddTDA18250_RFcal_modeOFF_OPEN,                /* RFcal_modeOFF  */                         \
            tmTDA18250_LPF_9MHz,                            /* LPF            */                         \
            tmddTDA18250_PD_NotActivated,                   /* PD_hpf         */                         \
            tmddTDA18250_HPF_Freq_1MHz,                     /* IF_hpf         */                         \
            0,                                              /* notch_select   */                         \
            tmddTDA18250_LPF_Offset_0,                      /* RC_LPFC_Offset */                         \
            tmddTDA18250_LPF_Sign_Plus                      /* RC_LPFC_Sign   */                         \
        }

#define TMBSL_TDA18250_INSTANCE_CUSTOM_COMMON_STD_DIGIAL_9MHz                                            \
        {                                                   /* Digital 9MHz   */                         \
            tmTDA18250DigitalChannel,                       /* ChannelType    */                         \
            5000000,                                        /* IF             */                         \
            tmddTDA18250_RFcal_modeOFF_OPEN,                /* RFcal_modeOFF  */                         \
            tmTDA18250_LPF_9MHz,                            /* LPF            */                         \
            tmddTDA18250_PD_NotActivated,                   /* PD_hpf         */                         \
            tmddTDA18250_HPF_Freq_400KHz,                   /* IF_hpf         */                         \
            0,                                              /* notch_select   */                         \
            tmddTDA18250_LPF_Offset_0,                      /* RC_LPFC_Offset */                         \
            tmddTDA18250_LPF_Sign_Plus                      /* RC_LPFC_Sign   */                         \
        }
#define TMBSL_TDA18250_INSTANCE_CUSTOM_COMMON_STD_DIGIAL_xMHz                                            \
        {                                                   /* Digital xMHz   */                         \
            tmTDA18250DigitalChannel,                       /* ChannelType    */                         \
            3750000,                                        /* IF             */                         \
            tmddTDA18250_RFcal_modeOFF_OPEN,                /* RFcal_modeOFF  */                         \
            tmTDA18250_LPF_6MHz,                            /* LPF            */                         \
            tmddTDA18250_PD_NotActivated,                   /* PD_hpf         */                         \
            tmddTDA18250_HPF_Freq_1_5MHz,                   /* IF_hpf         */                         \
            0,                                              /* notch_select   */                         \
            tmddTDA18250_LPF_Offset_0,                      /* RC_LPFC_Offset */                         \
            tmddTDA18250_LPF_Sign_Plus                      /* RC_LPFC_Sign   */                         \
        }

#define TMBSL_TDA18250_INSTANCE_CUSTOM_COMMON_STD_DIGIAL_6MHz_256QAM                                     \
        {                                                   /* Digital 6MHz   */                         \
            tmTDA18250DigitalChannel,                       /* ChannelType    */                         \
            4000000,                                        /* IF             */                         \
            tmddTDA18250_RFcal_modeOFF_OPEN,                /* RFcal_modeOFF  */                         \
            tmTDA18250_LPF_6MHz,                            /* LPF            */                         \
            tmddTDA18250_PD_NotActivated,                   /* PD_hpf         */                         \
            tmddTDA18250_HPF_Freq_1MHz,                   /* IF_hpf         */                         \
            0,                                              /* notch_select   */                         \
            tmddTDA18250_LPF_Offset_3,                      /* RC_LPFC_Offset */                         \
            tmddTDA18250_LPF_Sign_Minus                      /* RC_LPFC_Sign   */                         \
        }

#define TMBSL_TDA18250_INSTANCE_CUSTOM_COMMON_STD_DIGIAL_7MHz_256QAM                                     \
        {                                                   /* Digital 7MHz   */                         \
            tmTDA18250DigitalChannel,                       /* ChannelType    */                         \
            4000000,                                        /* IF             */                         \
            tmddTDA18250_RFcal_modeOFF_OPEN,                /* RFcal_modeOFF  */                         \
            tmTDA18250_LPF_7MHz,                            /* LPF            */                         \
            tmddTDA18250_PD_NotActivated,                   /* PD_hpf         */                         \
            tmddTDA18250_HPF_Freq_400KHz,                   /* IF_hpf         */                         \
            0,                                              /* notch_select   */                         \
            tmddTDA18250_LPF_Offset_0,                      /* RC_LPFC_Offset */                         \
            tmddTDA18250_LPF_Sign_Plus                      /* RC_LPFC_Sign   */                         \
        }

#define TMBSL_TDA18250_INSTANCE_CUSTOM_COMMON_STD_DIGIAL_8MHz_256QAM                                     \
        {                                                   /* Digital 8MHz   */                         \
            tmTDA18250DigitalChannel,                       /* ChannelType    */                         \
            4250000,                                        /* IF             */                         \
            tmddTDA18250_RFcal_modeOFF_OPEN,                /* RFcal_modeOFF  */                         \
            tmTDA18250_LPF_9MHz,                            /* LPF            */                         \
            tmddTDA18250_PD_NotActivated,                   /* PD_hpf         */                         \
            tmddTDA18250_HPF_Freq_1MHz,                     /* IF_hpf         */                         \
            0,                                              /* notch_select   */                         \
            tmddTDA18250_LPF_Offset_0,                      /* RC_LPFC_Offset */                         \
            tmddTDA18250_LPF_Sign_Minus                     /* RC_LPFC_Sign   */                         \
        }

#define TMBSL_TDA18250_INSTANCE_CUSTOM_COMMON_STD_DIGIAL_9MHz_256QAM                                     \
        {                                                   /* Digital 9MHz   */                         \
            tmTDA18250DigitalChannel,                       /* ChannelType    */                         \
            5000000,                                        /* IF             */                         \
            tmddTDA18250_RFcal_modeOFF_OPEN,                /* RFcal_modeOFF  */                         \
            tmTDA18250_LPF_9MHz,                            /* LPF            */                         \
            tmddTDA18250_PD_NotActivated,                   /* PD_hpf         */                         \
            tmddTDA18250_HPF_Freq_400KHz,                   /* IF_hpf         */                         \
            0,                                              /* notch_select   */                         \
            tmddTDA18250_LPF_Offset_1,                      /* RC_LPFC_Offset */                         \
            tmddTDA18250_LPF_Sign_Plus                      /* RC_LPFC_Sign   */                         \
        }

#define TMBSL_TDA18250_INSTANCE_CUSTOM_COMMON_STD_DIGIAL_6MHz_64QAM                                      \
        {                                                   /* Digital 6MHz   */                         \
            tmTDA18250DigitalChannel,                       /* ChannelType    */                         \
            4000000,                                        /* IF             */                         \
            tmddTDA18250_RFcal_modeOFF_OPEN,                /* RFcal_modeOFF  */                         \
            tmTDA18250_LPF_6MHz,                            /* LPF            */                         \
            tmddTDA18250_PD_NotActivated,                   /* PD_hpf         */                         \
            tmddTDA18250_HPF_Freq_1MHz,                   /* IF_hpf         */                         \
            0,                                              /* notch_select   */                         \
            tmddTDA18250_LPF_Offset_3,                      /* RC_LPFC_Offset */                         \
            tmddTDA18250_LPF_Sign_Minus                      /* RC_LPFC_Sign   */                         \
        }

#define TMBSL_TDA18250_INSTANCE_CUSTOM_COMMON_STD_DIGIAL_7MHz_64QAM                                      \
        {                                                   /* Digital 7MHz   */                         \
            tmTDA18250DigitalChannel,                       /* ChannelType    */                         \
            4000000,                                        /* IF             */                         \
            tmddTDA18250_RFcal_modeOFF_OPEN,                /* RFcal_modeOFF  */                         \
            tmTDA18250_LPF_7MHz,                            /* LPF            */                         \
            tmddTDA18250_PD_NotActivated,                   /* PD_hpf         */                         \
            tmddTDA18250_HPF_Freq_400KHz,                   /* IF_hpf         */                         \
            0,                                              /* notch_select   */                         \
            tmddTDA18250_LPF_Offset_0,                      /* RC_LPFC_Offset */                         \
            tmddTDA18250_LPF_Sign_Plus                      /* RC_LPFC_Sign   */                         \
        }

#define TMBSL_TDA18250_INSTANCE_CUSTOM_COMMON_STD_DIGIAL_8MHz_64QAM                                      \
        {                                                   /* Digital 8MHz   */                         \
            tmTDA18250DigitalChannel,                       /* ChannelType    */                         \
            4250000,                                        /* IF             */                         \
            tmddTDA18250_RFcal_modeOFF_OPEN,                /* RFcal_modeOFF  */                         \
            tmTDA18250_LPF_9MHz,                            /* LPF            */                         \
            tmddTDA18250_PD_NotActivated,                   /* PD_hpf         */                         \
            tmddTDA18250_HPF_Freq_1MHz,                     /* IF_hpf         */                         \
            0,                                              /* notch_select   */                         \
            tmddTDA18250_LPF_Offset_2,                      /* RC_LPFC_Offset */                         \
            tmddTDA18250_LPF_Sign_Plus                      /* RC_LPFC_Sign   */                         \
        }

#define TMBSL_TDA18250_INSTANCE_CUSTOM_COMMON_STD_DIGIAL_9MHz_64QAM                                      \
        {                                                   /* Digital 9MHz   */                         \
            tmTDA18250DigitalChannel,                       /* ChannelType    */                         \
            5000000,                                        /* IF             */                         \
            tmddTDA18250_RFcal_modeOFF_OPEN,                /* RFcal_modeOFF  */                         \
            tmTDA18250_LPF_9MHz,                            /* LPF            */                         \
            tmddTDA18250_PD_NotActivated,                   /* PD_hpf         */                         \
            tmddTDA18250_HPF_Freq_400KHz,                   /* IF_hpf         */                         \
            0,                                              /* notch_select   */                         \
            tmddTDA18250_LPF_Offset_3,                      /* RC_LPFC_Offset */                         \
            tmddTDA18250_LPF_Sign_Minus                     /* RC_LPFC_Sign   */                         \
        }

#define TMBSL_TDA18250_INSTANCE_CUSTOM_COMMON_STD_ARRAY                                                  \
    {   /* stdArray */                                                                                   \
        TMBSL_TDA18250_INSTANCE_CUSTOM_COMMON_STD_DIGIAL_6MHz,                                           \
        TMBSL_TDA18250_INSTANCE_CUSTOM_COMMON_STD_DIGIAL_7MHz,                                           \
        TMBSL_TDA18250_INSTANCE_CUSTOM_COMMON_STD_DIGIAL_8MHz,                                           \
        TMBSL_TDA18250_INSTANCE_CUSTOM_COMMON_STD_DIGIAL_9MHz,                                           \
        TMBSL_TDA18250_INSTANCE_CUSTOM_COMMON_STD_DIGIAL_xMHz,                                           \
        TMBSL_TDA18250_INSTANCE_CUSTOM_COMMON_STD_DIGIAL_6MHz_256QAM,                                    \
        TMBSL_TDA18250_INSTANCE_CUSTOM_COMMON_STD_DIGIAL_7MHz_256QAM,                                    \
        TMBSL_TDA18250_INSTANCE_CUSTOM_COMMON_STD_DIGIAL_8MHz_256QAM,                                    \
        TMBSL_TDA18250_INSTANCE_CUSTOM_COMMON_STD_DIGIAL_9MHz_256QAM,                                    \
        TMBSL_TDA18250_INSTANCE_CUSTOM_COMMON_STD_DIGIAL_6MHz_64QAM,                                     \
        TMBSL_TDA18250_INSTANCE_CUSTOM_COMMON_STD_DIGIAL_7MHz_64QAM,                                     \
        TMBSL_TDA18250_INSTANCE_CUSTOM_COMMON_STD_DIGIAL_8MHz_64QAM,                                     \
        TMBSL_TDA18250_INSTANCE_CUSTOM_COMMON_STD_DIGIAL_9MHz_64QAM,                                     \
    }

/*======================================================================================================*/
/*======================================================================================================*/
/* Custom Driver Instance Parameters: BAND_PARAM                                                        */
/*======================================================================================================*/
/*======================================================================================================*/
/*                                                           {{    F                     F},                L             ,             C           ,         S             ,           S          , I, C, C, C, B, B, C, C, D,             p            , A, B, S, S, P}*/
/*                                                           {{    i                     i},                o             ,             h           ,         w             ,           w          , n, a, a, a, P, P, a, a, T,             d            , G, P, t, t, S}*/
/*                                                           {{    l                     l},                M             ,             a           ,         i             ,           i          , d, p, p, p, _, _, p, p, A,             _            , C, _, o, o, M}*/
/*                                                           {{    t                     t},                a             ,             n           ,         t             ,           t          , _, _, _, _, I, I, _, _, _,             A            , 1, F, b, b, _}*/
/*                                                           {{    e                     e},                x             ,             n           ,         c             ,           c          , D, H, H, H, n, n, L, L, n,             G            , b, i, _, _, S}*/
/*                                                           {{    r                     r},                -             ,             e           ,         h             ,           h          , i, P, P, P, d, d, 6, 1, o,             C            , i, l, G, D, t}*/
/*                                                           {{                           },                I             ,             l           ,         T             ,           S          , v, _, _, _, 1, 2, n, 0, t,             1            , s, t, a, C, o}*/
/*                                                           {{    T                     n},                F             ,                         ,         y             ,           t          , 2, L, M, R,  ,  ,  , n, M,             b            , _, e, i,  , b}*/
/*                                                           {{    y                     u},                              ,             T           ,         p             ,           a          ,  , e, i, i,  ,  ,  ,  , O,             i            , g, r, n,  ,  }*/
/*                                                           {{    p                     m},                (             ,             y           ,         e             ,           t          ,  , f, d, g,  ,  ,  ,  , C,             s            , a,  ,  ,  ,  }*/
/*                                                           {{    e                     b},                H             ,             p           ,                       ,           e          ,  , t,  , h,  ,  ,  ,  , A,                          , i,  ,  ,  ,  }*/
/*                                                           {{                          e},                z             ,             e           ,                       ,                      ,  ,  ,  , t,  ,  ,  ,  ,  ,                          , n,  ,  ,  ,  }*/
/*                                                           {{                          r),                )             ,                         ,                       ,                      ,  ,  ,  ,  ,  ,  ,  ,  ,  ,                          ,  ,  ,  ,  ,  )*/

#define TMBSL_TDA18250_INSTANCE_CUSTOM_COMMON_BP_DIG_Band_1  {{tmTDA18250_FILTER_BYPASS, 0}, tmTDA18250_RF_MAX_DIG_Band_1 , tmTDA18250DigitalChannel, tmTDA18250_DTA_bypass , tmddTDA18250_SW_CLOSE, 0, 0, 0, 0, 0, 0, 0, 0, 0, tmddTDA18250_PD_Activated, 1, 0, 0, 1, 1}
#define TMBSL_TDA18250_INSTANCE_CUSTOM_COMMON_BP_DIG_Band_2  {{tmTDA18250_FILTER_BYPASS, 0}, tmTDA18250_RF_MAX_DIG_Band_2 , tmTDA18250DigitalChannel, tmTDA18250_DTA_bypass , tmddTDA18250_SW_CLOSE, 0, 0, 0, 0, 0, 0, 0, 0, 0, tmddTDA18250_PD_Activated, 1, 1, 0, 1, 1}
#define TMBSL_TDA18250_INSTANCE_CUSTOM_COMMON_BP_DIG_Band_3  {{tmTDA18250_FILTER_BYPASS, 0}, tmTDA18250_RF_MAX_DIG_Band_3 , tmTDA18250DigitalChannel, tmTDA18250_DTA_bypass , tmddTDA18250_SW_CLOSE, 0, 0, 0, 0, 0, 0, 0, 0, 0, tmddTDA18250_PD_Activated, 1, 2, 0, 1, 1}
#define TMBSL_TDA18250_INSTANCE_CUSTOM_COMMON_BP_DIG_Band_4  {{tmTDA18250_FILTER_BYPASS, 0}, tmTDA18250_RF_MAX_DIG_Band_4 , tmTDA18250DigitalChannel, tmTDA18250_DTA_bypass , tmddTDA18250_SW_CLOSE, 0, 0, 0, 0, 0, 0, 0, 0, 0, tmddTDA18250_PD_Activated, 1, 3, 0, 1, 1}
#define TMBSL_TDA18250_INSTANCE_CUSTOM_COMMON_BP_DIG_Band_5  {{tmTDA18250_FILTER_BYPASS, 0}, tmTDA18250_RF_MAX_DIG_Band_5 , tmTDA18250DigitalChannel, tmTDA18250_DTA_bypass , tmddTDA18250_SW_CLOSE, 0, 0, 3, 0, 0, 0, 3, 3, 0, tmddTDA18250_PD_Activated, 1, 4, 0, 1, 1}
#define TMBSL_TDA18250_INSTANCE_CUSTOM_COMMON_BP_DIG_Band_6  {{tmTDA18250_FILTER_BYPASS, 0}, tmTDA18250_RF_MAX_DIG_Band_6 , tmTDA18250DigitalChannel, tmTDA18250_DTA_bypass , tmddTDA18250_SW_CLOSE, 0, 0, 3, 0, 1, 0, 3, 3, 0, tmddTDA18250_PD_Activated, 1, 5, 0, 1, 1}
#define TMBSL_TDA18250_INSTANCE_CUSTOM_COMMON_BP_DIG_Band_7  {{tmTDA18250_FILTER_BYPASS, 0}, tmTDA18250_RF_MAX_DIG_Band_7 , tmTDA18250DigitalChannel, tmTDA18250_DTA_bypass , tmddTDA18250_SW_CLOSE, 1, 3, 3, 0, 1, 2, 3, 1, 0, tmddTDA18250_PD_Activated, 1, 6, 0, 1, 1}


#define TMBSL_TDA18250_INSTANCE_CUSTOM_COMMON_BAND_PARAM                                                 \
    tmTDA18250_IND_DIG_Band_6,                              /* BandIndex      */                         \
    {   /* BandParam */                                                                                  \
        TMBSL_TDA18250_INSTANCE_CUSTOM_COMMON_BP_DIG_Band_1,                                             \
        TMBSL_TDA18250_INSTANCE_CUSTOM_COMMON_BP_DIG_Band_2,                                             \
        TMBSL_TDA18250_INSTANCE_CUSTOM_COMMON_BP_DIG_Band_3,                                             \
        TMBSL_TDA18250_INSTANCE_CUSTOM_COMMON_BP_DIG_Band_4,                                             \
        TMBSL_TDA18250_INSTANCE_CUSTOM_COMMON_BP_DIG_Band_5,                                             \
        TMBSL_TDA18250_INSTANCE_CUSTOM_COMMON_BP_DIG_Band_6,                                             \
        TMBSL_TDA18250_INSTANCE_CUSTOM_COMMON_BP_DIG_Band_7,                                             \
    }


/*======================================================================================================*/
/*======================================================================================================*/
/* Custom Driver Instance Parameters: RF_AGC_TOP_PARAM                                                  */
/*======================================================================================================*/
/*======================================================================================================*/
#define TMBSL_TDA18250_INSTANCE_CUSTOM_COMMON_RF_AGC_TOP_PARAM                                           \
    {                                                                                                    \
        /* RF_AGC_Top */                                                                                 \
        {145250000,             3 },                                                                     \
        {301250000,             2 },                                                                     \
        {337250000,             1 },                                                                     \
        {373250000,             2 },                                                                     \
        {547250000,             1 },                                                                     \
        {TMBSL_TDA18250_RF_MAX, 1 }                                                                      \
    }
#define TDA18250_MAX_UNITS (4)
#define TDA18250_MASTER_UNIT (0)

/*======================================================================================================*/
/*======================================================================================================*/
/* Custom Driver Instance Parameters: (Path 0)                                                          */
/*======================================================================================================*/
/*======================================================================================================*/
#define TMBSL_TDA18250_INSTANCE_CUSTOM_0                                                                 \
    tmddTDA18250_DeviceTypeMasterMulti,                     /* Device Type    */                         \
    tmTDA18250_PowerStandby,                                /* Power state    */                         \
    tmTDA18250_DIG_7MHz,                                    /* Standard mode  */                         \
    True,                                                   /* Clock_mode_auto*/                         \
    tmddTDA18250_AGC1_Gain_Free,                            /* AGC1 mode      */                         \
    tmddTDA18250_TCST_8,                                    /* Switch_TimeCst */                         \
    tmddTDA18250_TCST_32,                                   /* AGC1_TimeCst   */                         \
    tmddTDA18250_TCST_32,                                   /* LT_TimeCst     */                         \
    tmddTDA18250_TCST_8,                                    /* MTO_TimeCst    */                         \
    tmddTDA18250_IF_Level_1V,                               /* uIF_Level      */                         \
    TMBSL_TDA18250_INSTANCE_CUSTOM_COMMON_AGC_TOP,                                                       \
    TMBSL_TDA18250_INSTANCE_CUSTOM_COMMON_STD_ARRAY,                                                     \
    TMBSL_TDA18250_INSTANCE_CUSTOM_COMMON_BAND_PARAM,                                                    \
    TMBSL_TDA18250_INSTANCE_CUSTOM_COMMON_RF_AGC_TOP_PARAM


/*======================================================================================================*/
/*======================================================================================================*/
/* Custom Driver Instance Parameters: (Path 1)                                                          */
/*======================================================================================================*/
/*======================================================================================================*/
#define TMBSL_TDA18250_INSTANCE_CUSTOM_1                                                                 \
    tmTDA18250_DeviceTypeSlaveWithXTout,                    /* Device Type    */                         \
    tmTDA18250_PowerStandby,                                /* Power state    */                         \
    tmTDA18250_DIG_7MHz,                                    /* Standard mode  */                         \
    True,                                                   /* Clock_mode_auto*/                         \
    tmddTDA18250_AGC1_Gain_Fixed_6_75dB,                    /* AGC1 mode      */                         \
    tmddTDA18250_TCST_0,                                    /* Switch_TimeCst */                         \
    tmddTDA18250_TCST_2,                                    /* AGC1_TimeCst   */                         \
    tmddTDA18250_TCST_2,                                    /* LT_TimeCst     */                         \
    tmddTDA18250_TCST_0,                                    /* MTO_TimeCst    */                         \
    tmddTDA18250_IF_Level_1V,                               /* uIF_Level      */                         \
    TMBSL_TDA18250_INSTANCE_CUSTOM_COMMON_AGC_TOP,                                                       \
    TMBSL_TDA18250_INSTANCE_CUSTOM_COMMON_STD_ARRAY,                                                     \
    TMBSL_TDA18250_INSTANCE_CUSTOM_COMMON_BAND_PARAM,                                                    \
    TMBSL_TDA18250_INSTANCE_CUSTOM_COMMON_RF_AGC_TOP_PARAM


/*======================================================================================================*/
/*======================================================================================================*/
/* Custom Driver Instance Parameters: (Path 2)                                                          */
/*======================================================================================================*/
/*======================================================================================================*/
#define TMBSL_TDA18250_INSTANCE_CUSTOM_2                                                                 \
    tmTDA18250_DeviceTypeSlave,                             /* Device Type    */                         \
    tmTDA18250_PowerStandby,                                /* Power state    */                         \
    tmTDA18250_DIG_7MHz,                                    /* Standard mode  */                         \
    True,                                                   /* Clock_mode_auto*/                         \
    tmddTDA18250_AGC1_Gain_Free,                            /* AGC1 mode      */                         \
    tmddTDA18250_TCST_0,                                    /* Switch_TimeCst */                         \
    tmddTDA18250_TCST_2,                                    /* AGC1_TimeCst   */                         \
    tmddTDA18250_TCST_2,                                    /* LT_TimeCst     */                         \
    tmddTDA18250_TCST_0,                                    /* MTO_TimeCst    */                         \
    tmddTDA18250_IF_Level_1V,                               /* uIF_Level      */                         \
    TMBSL_TDA18250_INSTANCE_CUSTOM_COMMON_AGC_TOP,                                                       \
    TMBSL_TDA18250_INSTANCE_CUSTOM_COMMON_STD_ARRAY,                                                     \
    TMBSL_TDA18250_INSTANCE_CUSTOM_COMMON_BAND_PARAM,                                                    \
    TMBSL_TDA18250_INSTANCE_CUSTOM_COMMON_RF_AGC_TOP_PARAM


/*======================================================================================================*/
/*======================================================================================================*/
/* Custom Driver Instance Parameters: (Path 3)                                                          */
/*======================================================================================================*/
/*======================================================================================================*/
#define TMBSL_TDA18250_INSTANCE_CUSTOM_3                                                                 \
    tmTDA18250_DeviceTypeSlave,                             /* Device Type    */                         \
    tmTDA18250_PowerStandby,                                /* Power state    */                         \
    tmTDA18250_DIG_7MHz,                                    /* Standard mode  */                         \
    True,                                                   /* Clock_mode_auto*/                         \
    tmddTDA18250_AGC1_Gain_Free,                            /* AGC1 mode      */                         \
    tmddTDA18250_TCST_0,                                    /* Switch_TimeCst */                         \
    tmddTDA18250_TCST_2,                                    /* AGC1_TimeCst   */                         \
    tmddTDA18250_TCST_2,                                    /* LT_TimeCst     */                         \
    tmddTDA18250_TCST_0,                                    /* MTO_TimeCst    */                         \
    tmddTDA18250_IF_Level_1V,                               /* uIF_Level      */                         \
    TMBSL_TDA18250_INSTANCE_CUSTOM_COMMON_AGC_TOP,                                                       \
    TMBSL_TDA18250_INSTANCE_CUSTOM_COMMON_STD_ARRAY,                                                     \
    TMBSL_TDA18250_INSTANCE_CUSTOM_COMMON_BAND_PARAM,                                                    \
    TMBSL_TDA18250_INSTANCE_CUSTOM_COMMON_RF_AGC_TOP_PARAM


#ifdef __cplusplus
}
#endif

#endif /* _TMBSL_TDA18250_INSTANCE_CUSTOM_H */

