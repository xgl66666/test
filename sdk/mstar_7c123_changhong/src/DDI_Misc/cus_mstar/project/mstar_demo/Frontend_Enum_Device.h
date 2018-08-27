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

//------------------------------MS_DVB_TYPE_SEL-------------------------------
//
#define BROADCAST_TYPE_NOT_SEETING  0
#define DVBC                        1
#define DVBT                        2
#define DVBT2                       3
#define ISDBT                       4
#define DVBS                        5
#define ATSC                        6
#define J83B                        7
#define INTERNAL_DVBT               8
#define INTERNAL_ISDBT              9
#define DTMB                        10
#define INTERNAL_DVBS               12
#define DVBS2                       11
#define INTERNAL_DVBT2              13
#define DSS                         15
#define ATV_PAL                     16
#define ATV_SECAM_L_PRIME           17
#define ATV_NTSC                    18

//For Hummingbird demod compatible
#define INTERNAL_DVBC               14

#define DVB_NULL                    255

//------------------------------------------------------------------------------------
//  FRONTEND_DEMOD_TYPE
//------------------------------------------------------------------------------------

//--------------- external demods start ---------------
#define DEMOD_MSB1236C              0
#define DEMOD_MSB1233C              1
#define DEMOD_MSB123X               2
#define DEMOD_MSB131X               3             //DVBS
#define DEMOD_MSB1400               4             //external ISDBT
#define DEMOD_MXL254                5
#define DEMOD_MSB124X               6
#define DEMOD_MSB1245               7
#define DEMOD_MXL542                8
#define DEMOD_MSR1742               9
#define DEMOD_MSB201X               10
#define DEMOD_MXL683                11
#define DEMOD_MSB3046               12
#define DEMOD_MSB3400               13
#define DEMOD_MXL214                DEMOD_MXL254
#define DEMOD_MSB101X               DEMOD_MSB201X
#define DEMOD_MXL582                DEMOD_MXL542



#define DEMOD_MSB1237               126             //Kappa external ATSC
#define DEMOD_AVL6211               134             //DVBS

#define DIBCOM_DIB8096_DEMOD        136             //ISDBT

#define DEMOD_MSB1238               137             //DTMB Demodulator

//others
#define DEMOD_ZARLINK_ZL10353       127             // DVB COFDM Demodulator
#define DEMOD_MSDVBC                128             // DVBC Demodulator
#define DEMOD_AFA_9013              129
#define DEMOD_MSB1228               130
#define DEMOD_MSB1210               131
#define DEMOD_MSB122x               132             //DVBT Demodulator
#define DEMOD_TC90517               133
#define DEMOD_ATBM884x              138            //DTMB
#define DEMOD_ATBM885x              139            //DTMB
#define DEMOD_MXL101SF              140
#define DEMOD_IT9173                141            //ISDBT

//For Hummingbird demod compatible
#define DEMOD_DIB8096GP             DIBCOM_DIB8096_DEMOD
#define DEMOD_DIB8096P              DIBCOM_DIB8096_DEMOD
#define DEMOD_MSB140X               DEMOD_MSB1400
#define DEMOD_MSB124X_DVBS2         DEMOD_MSB124X   //DVBS2+DVBT2
#define DEMOD_MSB124X_DVBT2         DEMOD_MSB124X
#define DEMOD_MSB1245_DVBS2         DEMOD_MSB1245

//--------------- external demods end ---------------


//--------------- internal demods start---------------
#define DEMOD_MSINTERN_DVBT         232
#define DEMOD_MSINTERN_DVBC         233             // Mstar internal DVBC
#define DEMOD_MSINTERN_DVBT2        234
#define DEMOD_MSINTERN_ISDBT        235
//keres
#define DEMOD_MSKERES_ATSC          236
//kenya
#define DEMOD_MSKENYA_DVBC          237              //Kenya internal DVBC
//keltic
#define DEMOD_MSKELTIC_ATSC         238              //Keltic internal ATSC
//kappa
#define DEMOD_MSKAPPA_ISDBT         241              //Kappa internal ISDBT
#define DEMOD_MSATSC_C              242              //internal ATSC-Cable demod              // Kapa internal demod
#define DEMOD_MSKAPPA_DVBT          243              //Kappa internal DVBT
//kaiser
#define DEMOD_MSKAISER_DVBT         245              //Kaiser internal DVBT
#define DEMOD_MSKAISER_DVBC         246              //Kaiser internal DVBC
//kaiserin
#define DEMOD_MSKAISERIN_DVBT       247              //Kaiserin internal DVBT
//krnous
#define DEMOD_MSKRONUS_DVBT         249              //Kronus internal DVBT
//Kratos, Kayla, Kris, Keltic
#define DEMOD_MSINTERN_DVBS         251              //Mstar internal DVBS
//Kaiser, K6Lite
#define DEMOD_MSINTERN_DVBC_DUAL    252            // Mstar dual internal DVBC


//For Hummingbird demod compatible
#define DEMOD_MSDVBT_51             DEMOD_MSINTERN_DVBT
#define DEMOD_MSDVBC_51             DEMOD_MSINTERN_DVBC
#define DEMOD_MSKAISERIN_DVBC       DEMOD_MSINTERN_DVBC
#define DEMOD_MSKELTIC_DVBC         DEMOD_MSINTERN_DVBC
#define DEMOD_MSKAPPA_DVBC          DEMOD_MSINTERN_DVBC
#define DEMOD_MSKERES_DVBC          DEMOD_MSINTERN_DVBC
#define DEMOD_MSKRATOS_DVBC         DEMOD_MSINTERN_DVBC
#define DEMOD_MSKRIS_DVBS           DEMOD_MSINTERN_DVBS
//--------------- end internal demods ---------------

//not exist
#define DEMOD_NULL                  253
#define DEMOD_PLAYCARD              254              // DVB Fake Demodulator
#define DEMOD_UNKNWON               255

//------------------------------------------------------------------------------------
//FRONTEND_TUNER_TYPE
//------------------------------------------------------------------------------------
#define TUNER_TYPE                        0x100
#define TUNER_MXL603                        0
#define TUNER_TDA18250A                     1
#define TUNER_TDA18260                      2
#define TUNER_PHILIPS_TDA18250HN            3
#define TUNER_AV2011                        4
#define TUNER_AV2012                        5
#define TUNER_AV2028                        6
#define TUNER_AV2018                        7
#define TUNER_TDA18250B                     8
#define TUNER_R836                          9
#define TUNER_R820C                         10
#define TUNER_MXL203                        11
#define TUNER_MXL254                        12
#define TUNER_TDA18225                      13
#define TUNER_R848                          14
#define TUNER_RT710                         15
#define TUNER_AV2017                        16
#define TUNER_R858                          17
#define TUNER_R850                          18
#define TUNER_SI2141                        19
#define TUNER_RT828                         23
#define TUNER_R820T                         25
#define TUNER_MXL542                        26
#define TUNER_MSR1742                       27
#define TUNER_STV6111                       28
#define TUNER_MXL214                        TUNER_MXL254
#define TUNER_RT720                         TUNER_RT710
#define TUNER_MXL582                        TUNER_MXL542

#define TUNER_PHILIPS_TDA1316               128           // DVB, PAL RF Tuner
#define TUNER_THOMSON_FE6640                129           // DVB, PAL RF Tuner
#define TUNER_PHILIPS_FQD1216               130           // DVB, PAL RF Tuner
#define TUNER_PHILIPS_TDA6034               131           // DVB RF Tuner
#define TUNER_PHILIPS_CD1316                132           // DVB Tuner (external RFAGC)
#define TUNER_THOMSON_DCT70707              133               // DVB Tuner (internal RFAGC)
#define TUNER_PHILIPS_CD1616LF_GIGH_4       134               // DVB Tuner (internal RFAGC)
#define TUNER_PHILIPS_CD1616LF_GIH_4        135               // DVB Tuner (external RFAGC)
#define TUNER_MXL_MXL5007                   136
#define TUNER_LG_TDTM_G352                  137
#define TUNER_LG_TDTC_G311D                 138
#define TUNER_PLAYCARD                      139
#define TUNER_NXP_TD1611ALF                 140

#define TUNER_NUTUNE_FJ2217                 141
#define TUNER_NUTUNE_FQD1136                142
#define TUNER_MXL201RF                      143
#define TUNER_NXP_TDA18219                  144

#define TUNER_MXL608	                    145

#define TUNER_MS221                         146
#define TUNER_MS222                         147

//DVBS Tuner
#define TUNER_SHARP_BS2S7HZ6306             148

#define TUNER_TBWU18120IN                   149
#define TUNER_SHARP_BS2S7VZ7801             150
#define TUNER_TDA20152                      151
#define TUNER_RDA5815M                      153
#define TUNER_AV2035                        154

//ISDBT
#define TUNER_DIBCOM_DIB8096                152

//Bifrsot TV Tuner
#define TUNER_MXL661                        155

#define TUNER_MXL683                        156
#define TUNER_RDA5817                       157
#define TUNER_RDA5880U                      158
#define TUNER_ATBM2040                      159
//NULL Tuner
#define TUNER_NULL                          255

//For Hummingbird demod compatible
#define TUNER_TDA18250AB                    TUNER_TDA18250A

//---------------------------  DISH_TYPE-------------------
#define DISH_NONE                            0
#define DISH_DEMOD                           1
#define DISH_A8293                           2
#define DISH_A8296                           3
#define DISH_TPS65233                        4
#define DISH_A8304                           5
#define DISH_A8297                           6
#define DISH_A8302                           7


//For Hummingbird demod compatible
#define TUNER_DIBCOM                         TUNER_DIBCOM_DIB8096

#define DISH_NULL                           255


//------------------------------------------------------------------------------------
// FRONTEND_IF_DEMODE_TYPE
//------------------------------------------------------------------------------------
#define PHILIPS_TDA9886                 0   // DVB Domodulator for Analog AV
#define PHILIPS_TDA9881                 1
#define RENESAS_R2A10406NP              2
#define MSTAR_VIF                       3
#define MSTAR_VIF_MSB1210               4    // MSB1210 DVB-T + VIF Demodulator
#define TOSHIBA_TC90512XBG_IF           5
#define MSTAR_INTERN_VIF                6
#define MSTAR_VIF_MSB1228               7

//------------------------------------------------------------------------------------
// DMX_DEMOD_INPUT TYPE
//------------------------------------------------------------------------------------
#define DMX_INPUT_DEMOD             0              ///< DMX input from internal demod 0
#define DMX_INPUT_DEMOD1            1              ///< DMX input from internal demod 1
#define DMX_INPUT_MEM               2              ///< DMX input from memory
#define DMX_INPUT_EXT_INPUT0        3              ///< DMX input from external input 0
#define DMX_INPUT_EXT_INPUT1        4              ///< DMX input from external input 1
#define DMX_INPUT_EXT_INPUT2        5              ///< DMX input from external input 2
#define DMX_INPUT_EXT_INPUT3        6              ///< DMX input from external input 3
#define DMX_INPUT_EXT_INPUT0_3WIRE        7              ///< DMX input from 3wire external input 0
#define DMX_INPUT_EXT_INPUT1_3WIRE        8              ///< DMX input from 3wire external input 1
#define DMX_INPUT_EXT_INPUT2_3WIRE        9              ///< DMX input from 3wire external input 2
#define DMX_INPUT_EXT_INPUT3_3WIRE        10             ///< DMX input from 3wire external input 3




//------------------------------ DTV Standar Type -----------------------------------
#ifndef MS_DVB_TYPE_SEL1
#define MS_DVB_TYPE_SEL1 DVB_NULL
#endif
#ifndef MS_DVB_TYPE_SEL2
#define MS_DVB_TYPE_SEL2 DVB_NULL
#endif
#ifndef MS_DVB_TYPE_SEL3
#define MS_DVB_TYPE_SEL3 DVB_NULL
#endif

//#if((MS_DVB_TYPE_SEL == DVBS) || (MS_DVB_TYPE_SEL1 == DVBS) || (MS_DVB_TYPE_SEL2 == DVBS) || (MS_DVB_TYPE_SEL3 == DVBS))

#ifndef MS_DVBS_INUSE
#ifdef __KERNEL__
#define MS_DVBS_INUSE 0
#else
#define MS_DVBS_INUSE 1
#endif
#endif

#ifndef MS_DVBC_INUSE
#define MS_DVBC_INUSE 1
#endif

#ifndef MS_DVBT2_INUSE
#define MS_DVBT2_INUSE 1
#endif

#ifndef MS_J83B_INUSE
#define MS_J83B_INUSE 1
#endif


#ifndef MS_DTMB_INUSE
#define MS_DTMB_INUSE 1
#endif

#ifndef MS_ATV_INUSE
#define MS_ATV_INUSE 1
#endif


#ifndef FRONEND_LOCAL_DETECT_LIST
#include "Frontend_Detect_List.h"
#else
#include "Local_Frontend_Detect_List.h"
#endif

//------------------------------ Demod Usage ----------------------------------
#if TUNER_14TH_SCAN_DEVICE != TUNER_NULL
#define TUNER_SCAN_NUMBER 15
#elif TUNER_13TH_SCAN_DEVICE != TUNER_NULL
#define TUNER_SCAN_NUMBER 14
#elif TUNER_12TH_SCAN_DEVICE != TUNER_NULL
#define TUNER_SCAN_NUMBER 13
#elif TUNER_11TH_SCAN_DEVICE != TUNER_NULL
#define TUNER_SCAN_NUMBER 12
#elif TUNER_10TH_SCAN_DEVICE != TUNER_NULL
#define TUNER_SCAN_NUMBER 11
#elif TUNER_09TH_SCAN_DEVICE != TUNER_NULL
#define TUNER_SCAN_NUMBER 10
#elif TUNER_08TH_SCAN_DEVICE != TUNER_NULL
#define TUNER_SCAN_NUMBER  9
#elif TUNER_07TH_SCAN_DEVICE != TUNER_NULL
#define TUNER_SCAN_NUMBER  8
#elif TUNER_06TH_SCAN_DEVICE != TUNER_NULL
#define TUNER_SCAN_NUMBER  7
#elif TUNER_05TH_SCAN_DEVICE != TUNER_NULL
#define TUNER_SCAN_NUMBER  6
#elif TUNER_04TH_SCAN_DEVICE != TUNER_NULL
#define TUNER_SCAN_NUMBER  5
#elif TUNER_03RD_SCAN_DEVICE != TUNER_NULL
#define TUNER_SCAN_NUMBER  4
#elif TUNER_02ND_SCAN_DEVICE != TUNER_NULL
#define TUNER_SCAN_NUMBER  3
#elif TUNER_01ST_SCAN_DEVICE != TUNER_NULL
#define TUNER_SCAN_NUMBER  2
#elif TUNER_00TH_SCAN_DEVICE != TUNER_NULL
#define TUNER_SCAN_NUMBER  1
#else
#define TUNER_SCAN_NUMBER  0
#endif

#ifndef FRONTEND_DEMOD_TYPE1
#define FRONTEND_DEMOD_TYPE1 DEMOD_NULL
#endif
#ifndef FRONTEND_DEMOD_TYPE2
#define FRONTEND_DEMOD_TYPE2 DEMOD_NULL
#endif
#ifndef FRONTEND_DEMOD_TYPE3
#define FRONTEND_DEMOD_TYPE3 DEMOD_NULL
#endif

#define IF_THIS_DEMOD_INUSE(x) ( ((DEMOD_00TH_SCAN_DEVICE == x)&&(DEMOD_SCAN_NUMBER > 0) )||\
                                 ((DEMOD_01ST_SCAN_DEVICE == x)&&(DEMOD_SCAN_NUMBER > 1) ) ||\
                                 ((DEMOD_02ND_SCAN_DEVICE == x)&&(DEMOD_SCAN_NUMBER > 2) ) ||\
                                 ((DEMOD_03RD_SCAN_DEVICE == x)&&(DEMOD_SCAN_NUMBER > 3) ) ||\
                                 ((DEMOD_04TH_SCAN_DEVICE == x)&&(DEMOD_SCAN_NUMBER > 4) ) ||\
                                 ((DEMOD_05TH_SCAN_DEVICE == x)&&(DEMOD_SCAN_NUMBER > 5) ) ||\
                                 ((DEMOD_06TH_SCAN_DEVICE == x)&&(DEMOD_SCAN_NUMBER > 6) ) ||\
                                 ((DEMOD_07TH_SCAN_DEVICE == x)&&(DEMOD_SCAN_NUMBER > 7) ) ||\
                                 ((DEMOD_08TH_SCAN_DEVICE == x)&&(DEMOD_SCAN_NUMBER > 8) ) ||\
                                 ((DEMOD_09TH_SCAN_DEVICE == x)&&(DEMOD_SCAN_NUMBER > 9) ) ||\
                                 ((DEMOD_10TH_SCAN_DEVICE == x)&&(DEMOD_SCAN_NUMBER > 10) ) ||\
                                 ((DEMOD_11TH_SCAN_DEVICE == x)&&(DEMOD_SCAN_NUMBER > 11) ) ||\
                                 ((DEMOD_12TH_SCAN_DEVICE == x)&&(DEMOD_SCAN_NUMBER > 12) ) ||\
                                 ((DEMOD_13TH_SCAN_DEVICE == x)&&(DEMOD_SCAN_NUMBER > 13) ) ||\
                                 ((DEMOD_14TH_SCAN_DEVICE == x)&&(DEMOD_SCAN_NUMBER > 14) ) )

//------------------------------ Tuner Usage ----------------------------------
#if DEMOD_14TH_SCAN_DEVICE != DEMOD_NULL
#define DEMOD_SCAN_NUMBER 15
#elif DEMOD_13TH_SCAN_DEVICE != DEMOD_NULL
#define DEMOD_SCAN_NUMBER 14
#elif DEMOD_12TH_SCAN_DEVICE != DEMOD_NULL
#define DEMOD_SCAN_NUMBER 13
#elif DEMOD_11TH_SCAN_DEVICE != DEMOD_NULL
#define DEMOD_SCAN_NUMBER 12
#elif DEMOD_10TH_SCAN_DEVICE != DEMOD_NULL
#define DEMOD_SCAN_NUMBER 11
#elif DEMOD_09TH_SCAN_DEVICE != DEMOD_NULL
#define DEMOD_SCAN_NUMBER 10
#elif DEMOD_08TH_SCAN_DEVICE != DEMOD_NULL
#define DEMOD_SCAN_NUMBER  9
#elif DEMOD_07TH_SCAN_DEVICE != DEMOD_NULL
#define DEMOD_SCAN_NUMBER  8
#elif DEMOD_06TH_SCAN_DEVICE != DEMOD_NULL
#define DEMOD_SCAN_NUMBER  7
#elif DEMOD_05TH_SCAN_DEVICE != DEMOD_NULL
#define DEMOD_SCAN_NUMBER  6
#elif DEMOD_04TH_SCAN_DEVICE != DEMOD_NULL
#define DEMOD_SCAN_NUMBER  5
#elif DEMOD_03RD_SCAN_DEVICE != DEMOD_NULL
#define DEMOD_SCAN_NUMBER  4
#elif DEMOD_02ND_SCAN_DEVICE != DEMOD_NULL
#define DEMOD_SCAN_NUMBER  3
#elif DEMOD_01ST_SCAN_DEVICE != DEMOD_NULL
#define DEMOD_SCAN_NUMBER  2
#elif DEMOD_00TH_SCAN_DEVICE != DEMOD_NULL
#define DEMOD_SCAN_NUMBER  1
#else
#define DEMOD_SCAN_NUMBER  0
#endif


#ifndef FRONTEND_TUNER_TYPE1
#define FRONTEND_TUNER_TYPE1 TUNER_NULL
#endif
#ifndef FRONTEND_TUNER_TYPE2
#define FRONTEND_TUNER_TYPE2 TUNER_NULL
#endif
#ifndef FRONTEND_TUNER_TYPE3
#define FRONTEND_TUNER_TYPE3 TUNER_NULL
#endif

//#define IF_THIS_TUNER_INUSE(x) 1
#define IF_THIS_TUNER_INUSE(x) (((TUNER_00TH_SCAN_DEVICE == x) && (TUNER_SCAN_NUMBER > 0)) ||\
                                ((TUNER_01ST_SCAN_DEVICE == x) && (TUNER_SCAN_NUMBER > 1)) ||\
                                ((TUNER_02ND_SCAN_DEVICE == x) && (TUNER_SCAN_NUMBER > 2)) ||\
                                ((TUNER_03RD_SCAN_DEVICE == x) && (TUNER_SCAN_NUMBER > 3)) ||\
                                ((TUNER_04TH_SCAN_DEVICE == x) && (TUNER_SCAN_NUMBER > 4)) ||\
                                ((TUNER_05TH_SCAN_DEVICE == x) && (TUNER_SCAN_NUMBER > 5)) ||\
                                ((TUNER_06TH_SCAN_DEVICE == x) && (TUNER_SCAN_NUMBER > 6)) ||\
                                ((TUNER_07TH_SCAN_DEVICE == x) && (TUNER_SCAN_NUMBER > 7)) ||\
                                ((TUNER_08TH_SCAN_DEVICE == x) && (TUNER_SCAN_NUMBER > 8)) ||\
                                ((TUNER_09TH_SCAN_DEVICE == x) && (TUNER_SCAN_NUMBER > 9)) ||\
                                ((TUNER_10TH_SCAN_DEVICE == x) && (TUNER_SCAN_NUMBER > 10)) ||\
                                ((TUNER_11TH_SCAN_DEVICE == x) && (TUNER_SCAN_NUMBER > 11)) ||\
                                ((TUNER_12TH_SCAN_DEVICE == x) && (TUNER_SCAN_NUMBER > 12)) ||\
                                ((TUNER_13TH_SCAN_DEVICE == x) && (TUNER_SCAN_NUMBER > 13)) ||\
                                ((TUNER_14TH_SCAN_DEVICE == x) && (TUNER_SCAN_NUMBER > 14)) )

#define IS_INTERN_DEMOD_PICKED IF_THIS_DEMOD_INUSE(DEMOD_MSINTERN_DVBS)||\
                               IF_THIS_DEMOD_INUSE(DEMOD_MSINTERN_DVBC)||\
                               IF_THIS_DEMOD_INUSE(DEMOD_MSINTERN_DVBT)||\
                               IF_THIS_DEMOD_INUSE(DEMOD_MSINTERN_DVBT2)||\
                               IF_THIS_DEMOD_INUSE(DEMOD_MSINTERN_ISDBT)||\
                               IF_THIS_DEMOD_INUSE(DEMOD_MSKERES_ATSC)||\
                               IF_THIS_DEMOD_INUSE(DEMOD_MSINTERN_DVBC_DUAL)||\
                               IF_THIS_DEMOD_INUSE(DEMOD_MSKELTIC_ATSC)||\
                               IF_THIS_DEMOD_INUSE(DEMOD_MSKENYA_DVBC)||\
                               IF_THIS_DEMOD_INUSE(DEMOD_MSATSC_C)
                               
#if IS_INTERN_DEMOD_PICKED
#define IS_THIS_DEMOD_PICKED(x) IF_THIS_DEMOD_INUSE(x)
#else
#define IS_THIS_DEMOD_PICKED(x) 1
#endif

                      
