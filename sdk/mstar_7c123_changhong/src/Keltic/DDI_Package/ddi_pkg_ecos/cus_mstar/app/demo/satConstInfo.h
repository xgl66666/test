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

//////////////////////////////////////////////////////////////////////////////
///
/// @file   apiDTVFreqTable.h
/// @brief  DVBC Physical Channel Database API
/// @author MStar Semiconductor Inc.
/////////////////////////////////////////////////////////////////////////////
#if MS_DVBS_INUSE

#if !defined(__API_DTVSATCONSTINFO_H__)
#define __API_DTVSATCONSTINFO_H__

#define BAND_C    0
#define BAND_KU  1


typedef enum _SATELLITE
{
       KU_70_EUTELSAT_W3A,
       KU_100_EUTELSAT_W1,
       KU_130_HOTBIRD_6_7A_8,
       KU_160_EUTELSAT_W2,
       KU_192_ASTRA_1H_1KR_1L_1M,
       KU_235_ASTRA_1E_1G_3A,
       KU_260_BADR_2_3_4_EURO,
       KU_282_ASTRA_2A_2B_2D,
       KU_305_ARABSAT_2B,
       KU_360_EUTELSAT_SESAT,
       KU_390_HELLAS_SAT_2,
       KU_400_EXPRESS_AM1,
       KU_420_TURKSAT_2A_3A,
       KU_620_INTELSAT_920,
       C_686_INTELSAT_7_10,
       KU_703_NSS_703,
       KU_765_TELSTAR_10,
       C_765_TELSTAR_10,
       KU_785_THAICOM_2_5,
      C_785_THAICOM_2_5,
      KU_830_INSAT_3B_4A,
      KU_900_YAMAL_210,
      KU_950_NSS_6,
      C_1005_ASIASAT_2,
      KU_1005_ASIASAT_2,
      C_1055_ASIASAT_3S,
      KU_1055_ASIASAT_3S,
      C_1155_CHINASAT_6B,
      C_1250_SINOSAT_3,
      C_1340_APSTAR_6,
      KU_1340_APSTAR_6,
      KU_1380_TELSTAR_18,
      C_1660_INTELSAT_8,
      KU_1660_INTELSAT_8,
      C_1690_INTELSAT_2,
      KU_1690_INTELSAT_2,
      C_2330_HORIZONS_1,
      KU_2330_HORIZONS_1,
      KU_2370_GALAXY_10R,
      C_2370_GALAXY_10R,
      C_2390_GALAXY_23,
      KU_2630_GALAXY_25,
      C_2650_GALAXY_3C,
      KU_2650_GALAXY_3C,
      C_2690_GALAXY_11,
      KU_2690_GALAXY_11,
      KU_3020_INTELSAT_9,
      C_3020_INTELSAT_9,
      KU_3070_INTELSAT_707,
      KU_3300_HISPASAT_1C_1D,
      KU_3325_INTELSAT_907,
      KU_3380_NSS_7,
      KU_3450_TELSTAR_12,
      KU_3475_ATLANTIC_BIRD_1,
      KU_3490_EXPRESS_A3,
      KU_3520_ATLANTIC_BIRD_2,
      KU_3530_NILESAT_101_10,
      KU_3560_AMOS_2_3,
}TYPE_SATELLITE;


typedef struct
{
    TYPE_SATELLITE eSatellite;
    MS_U8 *pSatName;
    MS_U16 u16Band:2;//0:C  1:Ku 2:ATV c 3:ATV ku
    MS_U16 u16Angle:14;
}Antenna_SatInfo;

typedef struct
{
    TYPE_SATELLITE eSatellite;
    MS_U32  u32Frequency;
    MS_U16  u16SymbolRate;  //Bw Type
    MS_U8   u8Polarity;
}Antenna_TPInfo;


Antenna_SatInfo const AntennaSatList_BuiltIn[] =
{
    {KU_130_HOTBIRD_6_7A_8,               (MS_U8*)"HOTBIRD 6,7A,8",        BAND_KU,  130} , //      3   107
    {KU_192_ASTRA_1H_1KR_1L_1M,           (MS_U8*)"ASTRA 1H,1KR,1L,1M",    BAND_KU,  192} , //      5   85
    {KU_282_ASTRA_2A_2B_2D,               (MS_U8*)"ASTRA 2A,2B,2D",        BAND_KU,  282} , //      7   90
    {KU_235_ASTRA_1E_1G_3A,               (MS_U8*)"ASTRA 1E,1G,3A",        BAND_KU,  235} , //      7     90
    {KU_3530_NILESAT_101_10,              (MS_U8*)"NILESAT 101, 10",       BAND_KU,  3530} ,//          46   35
    {KU_70_EUTELSAT_W3A,                  (MS_U8*)"EUTELSAT W3A",          BAND_KU,  70} ,  //      1   18
    {KU_100_EUTELSAT_W1,                  (MS_U8*)"EUTELSAT W1",           BAND_KU,  100} ,//       2   6
    {KU_260_BADR_2_3_4_EURO,              (MS_U8*)"BADR-2,3,4/EURO",       BAND_KU,  260} ,//       6   30
    {KU_360_EUTELSAT_SESAT,               (MS_U8*)"EUTELSAT SESAT",        BAND_KU,  360} ,//       9   9
    {KU_160_EUTELSAT_W2,                  (MS_U8*)"EUTELSAT W2",           BAND_KU,  160} ,//       4   47
    {KU_400_EXPRESS_AM1,                  (MS_U8*)"EXPRESS AM1",           BAND_KU,  400} ,//   11  5
    {KU_420_TURKSAT_2A_3A,                (MS_U8*)"TURKSAT 2A, 3A",        BAND_KU,  420} ,//   12  170
    {KU_620_INTELSAT_920,                 (MS_U8*)"INTELSAT 902",          BAND_KU,  620} ,//   13  18
    {C_686_INTELSAT_7_10,                 (MS_U8*)"INTELSAT 7, 10",        BAND_C,   686} ,//   14  53

    {KU_305_ARABSAT_2B,                   (MS_U8*)"ARABSAT 2B",            BAND_KU,  305} ,//       8   4
    {KU_390_HELLAS_SAT_2,                 (MS_U8*)"HELLAS SAT 2",          BAND_KU,  390} ,//   10  21

    {KU_765_TELSTAR_10,                   (MS_U8*)"TELSTAR 10",            BAND_KU,  765} ,//   15  32
    {C_765_TELSTAR_10,                    (MS_U8*)"TELSTAR 10",            BAND_C,   765} ,//   15  32
    {KU_785_THAICOM_2_5,                  (MS_U8*)"THAICOM 2,5",           BAND_KU,  785} ,//   16  45
    {C_785_THAICOM_2_5,                   (MS_U8*)"THAICOM 2,5",           BAND_C,   785} ,//   16  45
    {KU_830_INSAT_3B_4A,                  (MS_U8*)"INSAT 3B,4A",           BAND_KU,  830} ,//   17  12
    {KU_900_YAMAL_210,                    (MS_U8*)"YAMAL 201",             BAND_KU,  900} ,//   18  3
    {KU_950_NSS_6,                        (MS_U8*)"NSS 6",                 BAND_KU,  950} ,//   19  23
    {C_1005_ASIASAT_2,                    (MS_U8*)"ASIASAT 2",             BAND_C,   1005} ,//  20  37
    {KU_1005_ASIASAT_2,                   (MS_U8*)"ASIASAT 2",             BAND_KU,  1005} ,//  20  37
    {C_1055_ASIASAT_3S,                   (MS_U8*)"ASIASAT 3S",            BAND_C,   1055} ,//  21  50
    {KU_1055_ASIASAT_3S,                  (MS_U8*)"ASIASAT 3S",            BAND_KU,  1055} ,//  21  50
    {C_1155_CHINASAT_6B,                  (MS_U8*)"CHINASAT 6B",           BAND_C,   1155} ,//  22  44
    {C_1250_SINOSAT_3,                    (MS_U8*)"SINOSAT-3",             BAND_C,   1250} ,//  23  14
    {C_1340_APSTAR_6,                     (MS_U8*)"APSTAR 6",              BAND_C,   1340} ,//  24  11
    {KU_1340_APSTAR_6,                    (MS_U8*)"APSTAR 6",              BAND_KU,  1340} ,//  24  11
    {C_1660_INTELSAT_8,                   (MS_U8*)"INTELSAT 8",            BAND_C,   1660} ,//  25  19
    {KU_1660_INTELSAT_8,                  (MS_U8*)"INTELSAT 8",            BAND_KU,  1660} ,//  25  19
    {C_1690_INTELSAT_2,                   (MS_U8*)"INTELSAT 2",            BAND_C,   1690} ,//  26  10
    {KU_1690_INTELSAT_2,                  (MS_U8*)"INTELSAT 2",            BAND_KU,  1690} ,//  26  10
    {C_2330_HORIZONS_1,                   (MS_U8*)"HORIZONS 1",            BAND_C,   2330} ,//  28  2
    {KU_2330_HORIZONS_1,                  (MS_U8*)"HORIZONS 1",            BAND_KU,  2330} ,//  28  2
    {KU_2370_GALAXY_10R,                  (MS_U8*)"GALAXY 10R",            BAND_KU,  2370} ,//  29  26
    {C_2370_GALAXY_10R,                   (MS_U8*)"GALAXY 10R",            BAND_C,   2370} ,//  29  26
    {C_2390_GALAXY_23,                    (MS_U8*)"GALAXY 23",             BAND_C,   2390} ,//  30  25
    {KU_2630_GALAXY_25,                   (MS_U8*)"GALAXY 25",             BAND_KU,  2630} ,//  32  19
    {C_2650_GALAXY_3C,                    (MS_U8*)"GALAXY 3C",             BAND_C,   2650} ,//  33  37
    {KU_2650_GALAXY_3C,                   (MS_U8*)"GALAXY 3C",             BAND_KU,  2650} ,//  33  37
    {C_2690_GALAXY_11,                    (MS_U8*)"GALAXY 11",             BAND_C,   2690} ,//  34  26
    {KU_2690_GALAXY_11,                   (MS_U8*)"GALAXY 11",             BAND_KU,  2690} ,//  34  26
    {KU_3020_INTELSAT_9,                  (MS_U8*)"INTELSAT 9",            BAND_KU,  3020} ,//  37  31
    {C_3020_INTELSAT_9,                   (MS_U8*)"INTELSAT 9",            BAND_C,   3020} ,//  37  31
    {KU_3070_INTELSAT_707,                (MS_U8*)"INTELSAT 707",          BAND_KU,  3070} ,//  38  2
    {KU_3300_HISPASAT_1C_1D,              (MS_U8*)"HISPASAT 1C, 1D",       BAND_KU,  3300} ,//  39  31
    {KU_3325_INTELSAT_907,                (MS_U8*)"INTELSAT 907",          BAND_KU,  3325} ,//  40  2
    {KU_3380_NSS_7,                       (MS_U8*)"NSS 7",                 BAND_KU,  3380} ,//  41  5
    {KU_3450_TELSTAR_12,                  (MS_U8*)"TELSTAR 12",            BAND_KU,  3450} ,//  42  11
    {KU_3475_ATLANTIC_BIRD_1,             (MS_U8*)"ATLANTIC BIRD 1",       BAND_KU,  3475} ,//  43  16
    {KU_3490_EXPRESS_A3,                  (MS_U8*)"EXPRESS A3",            BAND_KU,  3490} ,//  44  1
    {KU_3520_ATLANTIC_BIRD_2,             (MS_U8*)"ATLANTIC BIRD 2",       BAND_KU,  3520} ,//  45  9
    {KU_3560_AMOS_2_3,                    (MS_U8*)"AMOS 2,3",              BAND_KU,  3560} ,//  47  29
    {KU_703_NSS_703,                      (MS_U8*)"NSS 703",               BAND_KU,  703} ,//   19  23
    {KU_1380_TELSTAR_18,                  (MS_U8*)"TELSTAR 18",            BAND_KU,  1380},//
};



Antenna_TPInfo const AntennaTpList_BuiltIn[] =
{
// 0-------H
// 1-------V
// Freq,     SymbolRate,   H,           Index,   Def TP, angle, satellite name
// KU_130_HOTBIRD_6_7A_8¦W
{KU_130_HOTBIRD_6_7A_8,  10723,     29900,     0} ,//         24          0  130  HOTBIRD 6,7A,8      ---------0
{KU_130_HOTBIRD_6_7A_8,  10775,     28000,     0} ,//         25          0  130  HOTBIRD 6,7A,8
{KU_130_HOTBIRD_6_7A_8,  10830,      3333,     0} ,//         27          0  130  HOTBIRD 6,7A,8
{KU_130_HOTBIRD_6_7A_8,  10853,     27500,     0} ,//         28          0  130  HOTBIRD 6,7A,8
{KU_130_HOTBIRD_6_7A_8,  10892,     27500,     0} ,//         29          0  130  HOTBIRD 6,7A,8
{KU_130_HOTBIRD_6_7A_8,  10930,     27500,     0} ,//         30          0  130  HOTBIRD 6,7A,8
{KU_130_HOTBIRD_6_7A_8,  10971,     27500,     0} ,//         31          0  130  HOTBIRD 6,7A,8
{KU_130_HOTBIRD_6_7A_8,  11013,     27500,     0} ,//         33          0  130  HOTBIRD 6,7A,8
{KU_130_HOTBIRD_6_7A_8,  11054,     27500,     0} ,//         34          0  130  HOTBIRD 6,7A,8
{KU_130_HOTBIRD_6_7A_8,  11096,     28000,     0} ,//         35          0  130  HOTBIRD 6,7A,8
{KU_130_HOTBIRD_6_7A_8,  11137,     27500,     0} ,//         36          0  130  HOTBIRD 6,7A,8
{KU_130_HOTBIRD_6_7A_8,  11179,     27500,     0} ,//         37          0  130  HOTBIRD 6,7A,8
{KU_130_HOTBIRD_6_7A_8,  11219,     27500,     0} ,//         38          0  130  HOTBIRD 6,7A,8
{KU_130_HOTBIRD_6_7A_8,  11258,     27500,     0} ,//         39          0  130  HOTBIRD 6,7A,8
{KU_130_HOTBIRD_6_7A_8,  11296,     27500,     0} ,//         40          0  130  HOTBIRD 6,7A,8
{KU_130_HOTBIRD_6_7A_8,  11334,       27500,   0},
{KU_130_HOTBIRD_6_7A_8,  11411,     27500,     0} ,//         41          0  130  HOTBIRD 6,7A,8
{KU_130_HOTBIRD_6_7A_8,  11449,     27500,     0} ,//         42          0  130  HOTBIRD 6,7A,8
{KU_130_HOTBIRD_6_7A_8,  11488,     27500,     0} ,//         43          0  130  HOTBIRD 6,7A,8
{KU_130_HOTBIRD_6_7A_8,  11526,     27500,     0} ,//         44          0  130  HOTBIRD 6,7A,8
{KU_130_HOTBIRD_6_7A_8,  11566,     27500,     0} ,//         45          0  130  HOTBIRD 6,7A,8
{KU_130_HOTBIRD_6_7A_8,  11604,     27500,     0} ,//         46          0  130  HOTBIRD 6,7A,8
{KU_130_HOTBIRD_6_7A_8,  11642,     27500,     0} ,//         47          0  130  HOTBIRD 6,7A,8
{KU_130_HOTBIRD_6_7A_8,  11681,     27500,     0} ,//         50          0  130  HOTBIRD 6,7A,8
{KU_130_HOTBIRD_6_7A_8,  11747,     27500,     0} ,//         51          0  130  HOTBIRD 6,7A,8
{KU_130_HOTBIRD_6_7A_8,  11785,     27500,     0} ,//         52          0  130  HOTBIRD 6,7A,8
{KU_130_HOTBIRD_6_7A_8,  11823,     27500,     0} ,//         53          0  130  HOTBIRD 6,7A,8
{KU_130_HOTBIRD_6_7A_8,  11862,     27500,     0} ,//         54          0  130  HOTBIRD 6,7A,8
{KU_130_HOTBIRD_6_7A_8,  11900,     27500,     0} ,//         55          0  130  HOTBIRD 6,7A,8
{KU_130_HOTBIRD_6_7A_8,  11938,     27500,     0} ,//         56          0  130  HOTBIRD 6,7A,8
{KU_130_HOTBIRD_6_7A_8,  11977,     27500,     0} ,//         57          0  130  HOTBIRD 6,7A,8
{KU_130_HOTBIRD_6_7A_8,  12015,     27500,     0} ,//         58          0  130  HOTBIRD 6,7A,8
{KU_130_HOTBIRD_6_7A_8,  12054,     27500,     0} ,//         59          0  130  HOTBIRD 6,7A,8
{KU_130_HOTBIRD_6_7A_8,  12092,     27500,     0} ,//         60          0  130  HOTBIRD 6,7A,8
{KU_130_HOTBIRD_6_7A_8,  12130,     27500,     0} ,//         61          0  130  HOTBIRD 6,7A,8
{KU_130_HOTBIRD_6_7A_8,  12169,     27500,     0} ,//         62          0  130  HOTBIRD 6,7A,8
{KU_130_HOTBIRD_6_7A_8,  12207,     27500,     0} ,//         63          0  130  HOTBIRD 6,7A,8
{KU_130_HOTBIRD_6_7A_8,  12245,     27500,     0} ,//         65          0  130  HOTBIRD 6,7A,8
{KU_130_HOTBIRD_6_7A_8,  12284,     27500,     0} ,//         66          0  130  HOTBIRD 6,7A,8
{KU_130_HOTBIRD_6_7A_8,  12322,     27500,     0} ,//         67          0  130  HOTBIRD 6,7A,8
{KU_130_HOTBIRD_6_7A_8,  12360,     27500,     0} ,//         68          0  130  HOTBIRD 6,7A,8
{KU_130_HOTBIRD_6_7A_8,  12399,     27500,     0} ,//         69          0  130  HOTBIRD 6,7A,8
{KU_130_HOTBIRD_6_7A_8,  12437,     27500,     0} ,//         70          0  130  HOTBIRD 6,7A,8
{KU_130_HOTBIRD_6_7A_8,  12476,     27500,     0} ,//         71          0  130  HOTBIRD 6,7A,8
{KU_130_HOTBIRD_6_7A_8,  12539,     27500,     0} ,//         72          0  130  HOTBIRD 6,7A,8
{KU_130_HOTBIRD_6_7A_8,  12577,     27500,     0} ,//         73          0  130  HOTBIRD 6,7A,8
{KU_130_HOTBIRD_6_7A_8,  12616,     27500,     0} ,//         74          0  130  HOTBIRD 6,7A,8
{KU_130_HOTBIRD_6_7A_8,  12654,     27500,     0} ,//         75          0  130  HOTBIRD 6,7A,8
{KU_130_HOTBIRD_6_7A_8,  12692,     27500,     0} ,//         76          0  130  HOTBIRD 6,7A,8
{KU_130_HOTBIRD_6_7A_8,  12731,     27500,     0} ,//         77          0  130  HOTBIRD 6,7A,8
{KU_130_HOTBIRD_6_7A_8,  10719,     27500,     1} ,//         78          0  130  HOTBIRD 6,7A,8
{KU_130_HOTBIRD_6_7A_8,  10796,     27500,     1} ,//         80          0  130  HOTBIRD 6,7A,8
{KU_130_HOTBIRD_6_7A_8,  10834,     27500,     1} ,//         81          0  130  HOTBIRD 6,7A,8
{KU_130_HOTBIRD_6_7A_8,  10873,     27500,     1} ,//         82          0  130  HOTBIRD 6,7A,8
{KU_130_HOTBIRD_6_7A_8,  10911,       27500,   1},
{KU_130_HOTBIRD_6_7A_8,  10949,     27500,     1} ,//         84          0  130  HOTBIRD 6,7A,8
{KU_130_HOTBIRD_6_7A_8,  10992,     27500,     1} ,//         85          0  130  HOTBIRD 6,7A,8
{KU_130_HOTBIRD_6_7A_8,  11034,     27500,     1} ,//         86          0  130  HOTBIRD 6,7A,8
{KU_130_HOTBIRD_6_7A_8,  11075,     27500,     1} ,//         87          0  130  HOTBIRD 6,7A,8
{KU_130_HOTBIRD_6_7A_8,  11117,     27500,     1} ,//         88          0  130  HOTBIRD 6,7A,8
{KU_130_HOTBIRD_6_7A_8,  11158,     27500,     1} ,//         89          0  130  HOTBIRD 6,7A,8
{KU_130_HOTBIRD_6_7A_8,  11200,     27500,     1} ,//         90          0  130  HOTBIRD 6,7A,8
{KU_130_HOTBIRD_6_7A_8,  11240,     27500,     1} ,//         91          0  130  HOTBIRD 6,7A,8
{KU_130_HOTBIRD_6_7A_8,  11278,     27500,     1} ,//         92          0  130  HOTBIRD 6,7A,8
{KU_130_HOTBIRD_6_7A_8,  11355,     27500,     1} ,//         93          0  130  HOTBIRD 6,7A,8
{KU_130_HOTBIRD_6_7A_8,  11393,     27500,     1} ,//         94          0  130  HOTBIRD 6,7A,8
{KU_130_HOTBIRD_6_7A_8,  11432,       27500,   1},
{KU_130_HOTBIRD_6_7A_8,  11471,     27500,     1} ,//         95          0  130  HOTBIRD 6,7A,8
{KU_130_HOTBIRD_6_7A_8,  11508,     27500,     1} ,//         96          0  130  HOTBIRD 6,7A,8
{KU_130_HOTBIRD_6_7A_8,  11541,     22000,     1} ,//         98          0  130  HOTBIRD 6,7A,8
{KU_130_HOTBIRD_6_7A_8,  11585,     27500,     1} ,//         99          0  130  HOTBIRD 6,7A,8
{KU_130_HOTBIRD_6_7A_8,  11623,     27500,     1} ,//        100          0  130  HOTBIRD 6,7A,8
{KU_130_HOTBIRD_6_7A_8,  11662,     27500,     1} ,//        101          0  130  HOTBIRD 6,7A,8
{KU_130_HOTBIRD_6_7A_8,  11727,     27500,     1} ,//        102          0  130  HOTBIRD 6,7A,8
{KU_130_HOTBIRD_6_7A_8,  11766,     27500,     1} ,//        103          0  130  HOTBIRD 6,7A,8
{KU_130_HOTBIRD_6_7A_8,  11804,     27500,     1} ,//        104          0  130  HOTBIRD 6,7A,8
{KU_130_HOTBIRD_6_7A_8,  11843,     27500,     1} ,//        105          0  130  HOTBIRD 6,7A,8
{KU_130_HOTBIRD_6_7A_8,  11881,     27500,     1} ,//        106          0  130  HOTBIRD 6,7A,8
{KU_130_HOTBIRD_6_7A_8,  11919,     27500,     1} ,//        107          0  130  HOTBIRD 6,7A,8
{KU_130_HOTBIRD_6_7A_8,  11958,     27500,     1} ,//        108          0  130  HOTBIRD 6,7A,8
{KU_130_HOTBIRD_6_7A_8,  11996,     27500,     1} ,//        109          0  130  HOTBIRD 6,7A,8
{KU_130_HOTBIRD_6_7A_8,  12034,     27500,     1} ,//        110          0  130  HOTBIRD 6,7A,8
{KU_130_HOTBIRD_6_7A_8,  12073,     27500,     1} ,//        111          0  130  HOTBIRD 6,7A,8
{KU_130_HOTBIRD_6_7A_8,  12111,     27500,     1} ,//        112          0  130  HOTBIRD 6,7A,8
{KU_130_HOTBIRD_6_7A_8,  12149,     27500,     1} ,//        113          0  130  HOTBIRD 6,7A,8
{KU_130_HOTBIRD_6_7A_8,  12188,     27500,     1} ,//        114          0  130  HOTBIRD 6,7A,8
{KU_130_HOTBIRD_6_7A_8,  12226,     16300,     1} ,//        115          0  130  HOTBIRD 6,7A,8
{KU_130_HOTBIRD_6_7A_8,  12241,       3700,     1},
{KU_130_HOTBIRD_6_7A_8,  12265,     27500,     1} ,//        116          0  130  HOTBIRD 6,7A,8
{KU_130_HOTBIRD_6_7A_8,  12341,     27500,     1} ,//        118          0  130  HOTBIRD 6,7A,8
{KU_130_HOTBIRD_6_7A_8,  12380,     27500,     1} ,//        119          0  130  HOTBIRD 6,7A,8
{KU_130_HOTBIRD_6_7A_8,  12418,     27500,     1} ,//        120          0  130  HOTBIRD 6,7A,8
{KU_130_HOTBIRD_6_7A_8,  12466,       29900,   1},
{KU_130_HOTBIRD_6_7A_8,  12520,     27500,     1} ,//        123          0  130  HOTBIRD 6,7A,8
{KU_130_HOTBIRD_6_7A_8,  12558,     27500,     1} ,//        124          0  130  HOTBIRD 6,7A,8
{KU_130_HOTBIRD_6_7A_8,  12597,     27500,     1} ,//        126          0  130  HOTBIRD 6,7A,8
{KU_130_HOTBIRD_6_7A_8,  12635,     27500,     1} ,//        127          0  130  HOTBIRD 6,7A,8
{KU_130_HOTBIRD_6_7A_8,  12673,     27500,     1} ,//        128          0  130  HOTBIRD 6,7A,8
{KU_130_HOTBIRD_6_7A_8,  12713,     27500,     1} ,//        129          0  130  HOTBIRD 6,7A,8

//KU_192_ASTRA_1H_1KR_1L_1M
{KU_192_ASTRA_1H_1KR_1L_1M,  10744,     22000,     0} ,//        178
{KU_192_ASTRA_1H_1KR_1L_1M,  10773,     22000,     0} ,//        179
{KU_192_ASTRA_1H_1KR_1L_1M,  10832,     22000,     0} ,//        180
{KU_192_ASTRA_1H_1KR_1L_1M,  10862,     22000,     0} ,//        181
{KU_192_ASTRA_1H_1KR_1L_1M,  10921,     22000,     0} ,//        182
{KU_192_ASTRA_1H_1KR_1L_1M,  11023,     22000,     0} ,//        183
{KU_192_ASTRA_1H_1KR_1L_1M,  11171,     22000,     0} ,//        184
{KU_192_ASTRA_1H_1KR_1L_1M,  11303,     22000,     0} ,//        185
{KU_192_ASTRA_1H_1KR_1L_1M,  11362,     22000,     0} ,//        186
{KU_192_ASTRA_1H_1KR_1L_1M,  11464,     22000,     0} ,//
{KU_192_ASTRA_1H_1KR_1L_1M,  11671,     22000,     0} ,//        187
{KU_192_ASTRA_1H_1KR_1L_1M,  11720,     27500,     0} ,//        188
{KU_192_ASTRA_1H_1KR_1L_1M,  11758,     27500,     0} ,//        189
{KU_192_ASTRA_1H_1KR_1L_1M,  11798,     27500,     0} ,//        190
{KU_192_ASTRA_1H_1KR_1L_1M,  11836,     27500,     0} ,//        191
{KU_192_ASTRA_1H_1KR_1L_1M,  11876,     27500,     0} ,//        192
{KU_192_ASTRA_1H_1KR_1L_1M,  11914,     27500,     0} ,//        193
{KU_192_ASTRA_1H_1KR_1L_1M,  11954,     27500,     0} ,//        194
{KU_192_ASTRA_1H_1KR_1L_1M,  11992,     27500,     0} ,//        195
{KU_192_ASTRA_1H_1KR_1L_1M,  12032,     27500,     0} ,//        196
{KU_192_ASTRA_1H_1KR_1L_1M,  12070,     27500,     0} ,//        197
{KU_192_ASTRA_1H_1KR_1L_1M,  12110,     27500,     0} ,//        198
{KU_192_ASTRA_1H_1KR_1L_1M,  12148,     27500,     0} ,//        199
{KU_192_ASTRA_1H_1KR_1L_1M,  12188,     27500,     0} ,//        200
{KU_192_ASTRA_1H_1KR_1L_1M,  12226,     27500,     0} ,//        201
{KU_192_ASTRA_1H_1KR_1L_1M,  12266,     27500,     0} ,//        202
{KU_192_ASTRA_1H_1KR_1L_1M,  12304,     27500,     0} ,//        203
{KU_192_ASTRA_1H_1KR_1L_1M,  12344,     27500,     0} ,//        204
{KU_192_ASTRA_1H_1KR_1L_1M,  12382,     27500,     0} ,//        205
{KU_192_ASTRA_1H_1KR_1L_1M,  12422,     27500,     0} ,//        206
{KU_192_ASTRA_1H_1KR_1L_1M,  12460,     27500,     0} ,//        207
{KU_192_ASTRA_1H_1KR_1L_1M,  12515,     22000,     0} ,//        208
{KU_192_ASTRA_1H_1KR_1L_1M,  12545,     22000,     0} ,//        209
{KU_192_ASTRA_1H_1KR_1L_1M,  12574,     22000,     0} ,//        210
{KU_192_ASTRA_1H_1KR_1L_1M,  12604,     22000,     0} ,//        211
{KU_192_ASTRA_1H_1KR_1L_1M,  12633,     22000,     0} ,//        212
{KU_192_ASTRA_1H_1KR_1L_1M,  12663,     22000,     0} ,//        213
{KU_192_ASTRA_1H_1KR_1L_1M,  12692,     22000,     0} ,//        214
{KU_192_ASTRA_1H_1KR_1L_1M,  12722,     22000,     0} ,//        215
{KU_192_ASTRA_1H_1KR_1L_1M,  10788,     22000,     1} ,//        216
{KU_192_ASTRA_1H_1KR_1L_1M,  10818,     22000,     1} ,//        217
{KU_192_ASTRA_1H_1KR_1L_1M,  10847,     22000,     1} ,//        218
{KU_192_ASTRA_1H_1KR_1L_1M,  10876,     22000,     1} ,//        219
{KU_192_ASTRA_1H_1KR_1L_1M,  10979,     22000,     1} ,//        220
{KU_192_ASTRA_1H_1KR_1L_1M,  11038,     22000,     1} ,//        221
{KU_192_ASTRA_1H_1KR_1L_1M,  11097,     22000,     1} ,//        222
{KU_192_ASTRA_1H_1KR_1L_1M,  11156,     22000,     1} ,//        223
{KU_192_ASTRA_1H_1KR_1L_1M,  11318,     22000,     1} ,//        225
{KU_192_ASTRA_1H_1KR_1L_1M,  11436,     22000,     1} ,//        226
{KU_192_ASTRA_1H_1KR_1L_1M,  11479,     22000,     1} ,//        227
{KU_192_ASTRA_1H_1KR_1L_1M,  11509,     22000,     1} ,//        228
{KU_192_ASTRA_1H_1KR_1L_1M,  11538,     22000,     1} ,//        229
{KU_192_ASTRA_1H_1KR_1L_1M,  11568,     22000,     1} ,//        230
{KU_192_ASTRA_1H_1KR_1L_1M,  11597,     22000,     1} ,//        231
{KU_192_ASTRA_1H_1KR_1L_1M,  11686,     22000,     1} ,//        232
{KU_192_ASTRA_1H_1KR_1L_1M,  11739,     27500,     1} ,//        233
{KU_192_ASTRA_1H_1KR_1L_1M,  11778,     27500,     1} ,//        235
{KU_192_ASTRA_1H_1KR_1L_1M,  11817,     27500,     1} ,//        236
{KU_192_ASTRA_1H_1KR_1L_1M,  11856,     27500,     1} ,//        237
{KU_192_ASTRA_1H_1KR_1L_1M,  11895,     27500,     1} ,//        238
{KU_192_ASTRA_1H_1KR_1L_1M,  11934,     27500,     1} ,//        239
{KU_192_ASTRA_1H_1KR_1L_1M,  11973,     27500,     1} ,//        240
{KU_192_ASTRA_1H_1KR_1L_1M,  12012,     27500,     1} ,//        241
{KU_192_ASTRA_1H_1KR_1L_1M,  12051,     27500,     1} ,//        242
{KU_192_ASTRA_1H_1KR_1L_1M,  12090,     27500,     1} ,//        243
{KU_192_ASTRA_1H_1KR_1L_1M,  12129,     27500,     1} ,//        244
{KU_192_ASTRA_1H_1KR_1L_1M,  12168,     27500,     1} ,//        245
{KU_192_ASTRA_1H_1KR_1L_1M,  12207,     27500,     1} ,//        246
{KU_192_ASTRA_1H_1KR_1L_1M,  12246,     27500,     1} ,//        247
{KU_192_ASTRA_1H_1KR_1L_1M,  12285,     27500,     1} ,//        248
{KU_192_ASTRA_1H_1KR_1L_1M,  12324,     27500,     1} ,//        249
{KU_192_ASTRA_1H_1KR_1L_1M,  12363,     27500,     1} ,//        250
{KU_192_ASTRA_1H_1KR_1L_1M,  12402,     27500,     1} ,//        251
{KU_192_ASTRA_1H_1KR_1L_1M,  12441,     27500,     1} ,//        252
{KU_192_ASTRA_1H_1KR_1L_1M,  12480,     27500,     1} ,//        253
{KU_192_ASTRA_1H_1KR_1L_1M,  12522,     22000,     1} ,//        254
{KU_192_ASTRA_1H_1KR_1L_1M,  12552,     22000,     1} ,//        255
{KU_192_ASTRA_1H_1KR_1L_1M,  12581,     22000,     1} ,//        256
{KU_192_ASTRA_1H_1KR_1L_1M,  12610,     22000,     1} ,//        257
{KU_192_ASTRA_1H_1KR_1L_1M,  12640,     22000,     1} ,//        258
{KU_192_ASTRA_1H_1KR_1L_1M,  12670,     22000,     1} ,//        260
{KU_192_ASTRA_1H_1KR_1L_1M,  12699,     22000,     1} ,//        261
{KU_192_ASTRA_1H_1KR_1L_1M,  12728,     22000,     1} ,//    262

//KU_282_ASTRA_2A_2B_2D
{KU_282_ASTRA_2A_2B_2D,  10714,       22000,     0} ,//        293           ----198
{KU_282_ASTRA_2A_2B_2D,  10744,       22000,     0} ,//        294
{KU_282_ASTRA_2A_2B_2D,  10773,       22000,     0} ,//        295
{KU_282_ASTRA_2A_2B_2D,  10803,       22000,     0} ,//        296
{KU_282_ASTRA_2A_2B_2D,  10832,       22000,     0} ,//        297
{KU_282_ASTRA_2A_2B_2D,  10862,       22000,     0} ,//        298
{KU_282_ASTRA_2A_2B_2D,  10891,       22000,     0} ,//        299
{KU_282_ASTRA_2A_2B_2D,  10921,       22000,     0} ,//        300
{KU_282_ASTRA_2A_2B_2D,  11222,       27500,     0} ,//        303
{KU_282_ASTRA_2A_2B_2D,  11261,       27500,     0} ,//        304
{KU_282_ASTRA_2A_2B_2D,  11307,       27500,     0} ,//        305
{KU_282_ASTRA_2A_2B_2D,  11344,       27500,   0},
{KU_282_ASTRA_2A_2B_2D,  11389,       27500,     0} ,//        307
{KU_282_ASTRA_2A_2B_2D,  11428,       27500,     0} ,//        308
{KU_282_ASTRA_2A_2B_2D,  11469,       27500,     0} ,//        309
{KU_282_ASTRA_2A_2B_2D,  11508,       27500,     0} ,//        310
{KU_282_ASTRA_2A_2B_2D,  11546,       27500,     0} ,//        311
{KU_282_ASTRA_2A_2B_2D,  11585,       27500,     0} ,//        312
{KU_282_ASTRA_2A_2B_2D,  11623,       27500,     0} ,//        313
{KU_282_ASTRA_2A_2B_2D,  11662,       27500,     0} ,//        314
{KU_282_ASTRA_2A_2B_2D,  11720,       29500,     0} ,//        315
{KU_282_ASTRA_2A_2B_2D,  11758,       27500,   0},
{KU_282_ASTRA_2A_2B_2D,  11798,       29500,   0},
{KU_282_ASTRA_2A_2B_2D,  11836,       27500,     0} ,//        318
{KU_282_ASTRA_2A_2B_2D,  11876,       27500,     0} ,//        319
{KU_282_ASTRA_2A_2B_2D,  11914,       27500,   0},
{KU_282_ASTRA_2A_2B_2D,  11954,       27500,   0},
{KU_282_ASTRA_2A_2B_2D,  11992,       27500,   0},
{KU_282_ASTRA_2A_2B_2D,  12032,       27500,   0},
{KU_282_ASTRA_2A_2B_2D,  12070,       27500,   0},
{KU_282_ASTRA_2A_2B_2D,  12110,       27500,   0},
{KU_282_ASTRA_2A_2B_2D,  12148,       27500,   0},
{KU_282_ASTRA_2A_2B_2D,  12188,       27500,   0},
{KU_282_ASTRA_2A_2B_2D,  12226,       27500,   0},
{KU_282_ASTRA_2A_2B_2D,  12266,       27500,   0},
{KU_282_ASTRA_2A_2B_2D,  12304,       27500,   0},
{KU_282_ASTRA_2A_2B_2D,  12344,       29500,   0},
{KU_282_ASTRA_2A_2B_2D,  12382,       29500,   0},
{KU_282_ASTRA_2A_2B_2D,  12422,       27500,   0},
{KU_282_ASTRA_2A_2B_2D,  12460,       27500,   0},
{KU_282_ASTRA_2A_2B_2D,  12523,       27500,   0},
{KU_282_ASTRA_2A_2B_2D,  12545,       6111,    0},
{KU_282_ASTRA_2A_2B_2D,  12555,       5632,    0},
{KU_282_ASTRA_2A_2B_2D,  12572,       6111,    0},
{KU_282_ASTRA_2A_2B_2D,  12607,       27500,     0} ,//        337
{KU_282_ASTRA_2A_2B_2D,  12643,       27500,     0} ,//        338
{KU_282_ASTRA_2A_2B_2D,  12675,       2963,    0},
{KU_282_ASTRA_2A_2B_2D,  10729,       22000,     1} ,//        339
{KU_282_ASTRA_2A_2B_2D,  10758,       22000,   1},
{KU_282_ASTRA_2A_2B_2D,  10788,       22000,     1} ,//        341
{KU_282_ASTRA_2A_2B_2D,  10818,       22000,     1} ,//        342
{KU_282_ASTRA_2A_2B_2D,  10847,       22000,     1} ,//        343
{KU_282_ASTRA_2A_2B_2D,  10876,       22000,   1},
{KU_282_ASTRA_2A_2B_2D,  10906,       22000,     1} ,//        345
{KU_282_ASTRA_2A_2B_2D,  10936,       22000,   1},
{KU_282_ASTRA_2A_2B_2D,  11224,       27500,     1} ,//        347
{KU_282_ASTRA_2A_2B_2D,  11260,       27500,     1} ,//        348
{KU_282_ASTRA_2A_2B_2D,  11307,       27500,     1} ,//        349
{KU_282_ASTRA_2A_2B_2D,  11343,       27500,     1} ,//        350
{KU_282_ASTRA_2A_2B_2D,  11390,       27500,     1} ,//        351
{KU_282_ASTRA_2A_2B_2D,  11426,       27500,     1} ,//        352
{KU_282_ASTRA_2A_2B_2D,  11488,       27500,     1} ,//        353
{KU_282_ASTRA_2A_2B_2D,  11527,       27500,     1} ,//        354
{KU_282_ASTRA_2A_2B_2D,  11565,       27500,     1} ,//        355
{KU_282_ASTRA_2A_2B_2D,  11604,       27500,     1} ,//        356
{KU_282_ASTRA_2A_2B_2D,  11642,       27500,     1} ,//        357
{KU_282_ASTRA_2A_2B_2D,  11681,       27500,     1} ,//        358
{KU_282_ASTRA_2A_2B_2D,  11739,       27500,     1} ,//        359
{KU_282_ASTRA_2A_2B_2D,  11778,       27500,     1} ,//        360
{KU_282_ASTRA_2A_2B_2D,  11817,       27500,     1} ,//        361
{KU_282_ASTRA_2A_2B_2D,  11856,       29500,     1} ,//        362
{KU_282_ASTRA_2A_2B_2D,  11895,       27500,     1} ,//        363
{KU_282_ASTRA_2A_2B_2D,  11934,       27500,     1} ,//        364
{KU_282_ASTRA_2A_2B_2D,  11973,       27500,     1} ,//        365
{KU_282_ASTRA_2A_2B_2D,  12012,       29500,   1},
{KU_282_ASTRA_2A_2B_2D,  12051,       27500,     1} ,//        367
{KU_282_ASTRA_2A_2B_2D,  12090,       27500,     1} ,//        368
{KU_282_ASTRA_2A_2B_2D,  12129,       27500,     1} ,//        369
{KU_282_ASTRA_2A_2B_2D,  12168,       29500,   1},
{KU_282_ASTRA_2A_2B_2D,  12207,       27500,     1} ,//        371
{KU_282_ASTRA_2A_2B_2D,  12246,       29500,   1},
{KU_282_ASTRA_2A_2B_2D,  12285,       27500,     1} ,//        373
{KU_282_ASTRA_2A_2B_2D,  12324,       29500,     1} ,//        374
{KU_282_ASTRA_2A_2B_2D,  12363,       29500,     1} ,//        375
{KU_282_ASTRA_2A_2B_2D,  12402,       27500,     1} ,//        376
{KU_282_ASTRA_2A_2B_2D,  12441,       27500,     1} ,//        377
{KU_282_ASTRA_2A_2B_2D,  12480,       27500,     1} ,//        378
{KU_282_ASTRA_2A_2B_2D,  12523,       27500,     1} ,//        379
{KU_282_ASTRA_2A_2B_2D,  12552,       19531,   1},
{KU_282_ASTRA_2A_2B_2D,  12560,       27500,     1} ,//        380
{KU_282_ASTRA_2A_2B_2D,  12572,       2854,    1},
{KU_282_ASTRA_2A_2B_2D,  12599,       3250,    1},
{KU_282_ASTRA_2A_2B_2D,  12607,       27500,     1} ,//        381
{KU_282_ASTRA_2A_2B_2D,  12643,       27500,     1} ,//        382
{KU_282_ASTRA_2A_2B_2D,  12676,       6666,    1},
{KU_282_ASTRA_2A_2B_2D,  12685,       6666,   1},
{KU_282_ASTRA_2A_2B_2D,  12694,       6666,   1},
{KU_282_ASTRA_2A_2B_2D,  12703,       6666,   1},
{KU_282_ASTRA_2A_2B_2D,  12712,       6666,   1},
{KU_282_ASTRA_2A_2B_2D,  12721,       6111,   1},
{KU_282_ASTRA_2A_2B_2D,  12729,       2725,   1},
{KU_282_ASTRA_2A_2B_2D,  12730,       6666,   1},

//KU_235_ASTRA_1E_1G_3A
{KU_235_ASTRA_1E_1G_3A,  10803,     22000,     0} ,//         24               ----306
{KU_235_ASTRA_1E_1G_3A,  10862,     22000,     0} ,//         24
{KU_235_ASTRA_1E_1G_3A,  10891,     22000,     0} ,//         24
{KU_235_ASTRA_1E_1G_3A,  11720,     27500,     0} ,//         24
{KU_235_ASTRA_1E_1G_3A,  11758,     28200,     0} ,//         24
{KU_235_ASTRA_1E_1G_3A,  11836,     29900,     0} ,//         24
{KU_235_ASTRA_1E_1G_3A,  11954,     27500,     0} ,//         24
{KU_235_ASTRA_1E_1G_3A,  11739,     27500,     1} ,//         24
{KU_235_ASTRA_1E_1G_3A,  11798,     27500,     0} ,//         24
{KU_235_ASTRA_1E_1G_3A,  11856,     27500,     1} ,//         24
{KU_235_ASTRA_1E_1G_3A,  11876,     27500,     0} ,//         24
{KU_235_ASTRA_1E_1G_3A,  11914,     27500,     0} ,//         24
{KU_235_ASTRA_1E_1G_3A,  11992,     27500,     0} ,//         24
{KU_235_ASTRA_1E_1G_3A,  12070,     27500,     0} ,//         24
{KU_235_ASTRA_1E_1G_3A,  12110,     27500,     0} ,//         24
{KU_235_ASTRA_1E_1G_3A,  12148,     27500,     0} ,//         24
{KU_235_ASTRA_1E_1G_3A,  12168,     27500,     1} ,//         24
{KU_235_ASTRA_1E_1G_3A,  12226,     28200,     0} ,//         24
{KU_235_ASTRA_1E_1G_3A,  12304,     27500,     0} ,//         24
{KU_235_ASTRA_1E_1G_3A,  12344,     28200,     0} ,//         24
{KU_235_ASTRA_1E_1G_3A,  12382,     27500,     0} ,//         24
{KU_235_ASTRA_1E_1G_3A,  11455,     6666,      0} ,//         24
{KU_235_ASTRA_1E_1G_3A,  11463,     6666,      0} ,//         24
{KU_235_ASTRA_1E_1G_3A,  11473,     4200,      0} ,//         24
{KU_235_ASTRA_1E_1G_3A,  11475,     27500,     1} ,//         24
{KU_235_ASTRA_1E_1G_3A,  11515,     27500,     0} ,//         24
{KU_235_ASTRA_1E_1G_3A,  11515,     28500,     1} ,//         24
{KU_235_ASTRA_1E_1G_3A,  11555,     28500,     0} ,//         24
{KU_235_ASTRA_1E_1G_3A,  11555,     28500,     1} ,//         24
{KU_235_ASTRA_1E_1G_3A,  11595,     28500,     0} ,//         24
{KU_235_ASTRA_1E_1G_3A,  11595,     28500,     1} ,//         24
{KU_235_ASTRA_1E_1G_3A,  11635,     28500,     0} ,//         24
{KU_235_ASTRA_1E_1G_3A,  11635,     28500,     1} ,//         24
{KU_235_ASTRA_1E_1G_3A,  11662,     6666,      0} ,//         24
{KU_235_ASTRA_1E_1G_3A,  11675,     3333,      0} ,//         24
{KU_235_ASTRA_1E_1G_3A,  11675,     28500,     1} ,//         24
{KU_235_ASTRA_1E_1G_3A,  12525,     27500,     1} ,//         24
{KU_235_ASTRA_1E_1G_3A,  12565,     27500,     0} ,//         24
{KU_235_ASTRA_1E_1G_3A,  12565,     27500,     1} ,//         24
{KU_235_ASTRA_1E_1G_3A,  12605,     28500,     0} ,//         24
{KU_235_ASTRA_1E_1G_3A,  12605,     28500,     1} ,//         24
{KU_235_ASTRA_1E_1G_3A,  12630,     6000,      1} ,//         24
{KU_235_ASTRA_1E_1G_3A,  12631,     6666,      0} ,//         24
{KU_235_ASTRA_1E_1G_3A,  12636,     2277,      1} ,//         24
{KU_235_ASTRA_1E_1G_3A,  12637,     2894,      0} ,//         24
{KU_235_ASTRA_1E_1G_3A,  12650,     6111,      0} ,//         24
{KU_235_ASTRA_1E_1G_3A,  12653,     3000,      1} ,//         24
{KU_235_ASTRA_1E_1G_3A,  12659,     6111,      0} ,//         24
{KU_235_ASTRA_1E_1G_3A,  12671,     6666,      0} ,//         24
{KU_235_ASTRA_1E_1G_3A,  12680,     6666,      0} ,//         24
{KU_235_ASTRA_1E_1G_3A,  12689,     6666,      0} ,//         24
{KU_235_ASTRA_1E_1G_3A,  12689,     6666,      1} ,//         24
{KU_235_ASTRA_1E_1G_3A,  12696,     3255,      0} ,//         24
{KU_235_ASTRA_1E_1G_3A,  12698,     6666,      1} ,//         24
{KU_235_ASTRA_1E_1G_3A,  12725,     27500,     1} ,//         24           ----360

//KU_3530_NILESAT_101_10
{KU_3530_NILESAT_101_10,  10728,       38500,     0} ,//       1206            -----361
{KU_3530_NILESAT_101_10,  10775,       27500,     0} ,//       1207
{KU_3530_NILESAT_101_10,  10853,       27500,     0} ,//       1208
{KU_3530_NILESAT_101_10,  10930,       27500,     0} ,//       1209
{KU_3530_NILESAT_101_10,  11766,       27500,     0} ,//       1210
{KU_3530_NILESAT_101_10,  11804,       27500,     0} ,//       1211
{KU_3530_NILESAT_101_10,  11843,       27500,     0} ,//       1212
{KU_3530_NILESAT_101_10,  11881,       27500,     0} ,//       1213
{KU_3530_NILESAT_101_10,  11919,       27500,     0} ,//       1214
{KU_3530_NILESAT_101_10,  11957,       27500,     0} ,//       1215
{KU_3530_NILESAT_101_10,  11996,       27500,     0} ,//       1216
{KU_3530_NILESAT_101_10,  12034,       27500,     0} ,//       1217
{KU_3530_NILESAT_101_10,  12073,       27500,     0} ,//       1218
{KU_3530_NILESAT_101_10,  12149,       27500,     0} ,//       1219
{KU_3530_NILESAT_101_10,  12226,       27500,     0} ,//       1220
{KU_3530_NILESAT_101_10,  12302,       27500,     0} ,//       1221
{KU_3530_NILESAT_101_10,  12360,       27500,     0} ,//       1222
{KU_3530_NILESAT_101_10,  12399,       27500,     0} ,//       1223
{KU_3530_NILESAT_101_10,  10719,       27500,     1} ,//       1224
{KU_3530_NILESAT_101_10,  10757,       27500,     1} ,//       1225
{KU_3530_NILESAT_101_10,  10795,       27500,     1} ,//       1226
{KU_3530_NILESAT_101_10,  10834,       27500,     1} ,//       1227
{KU_3530_NILESAT_101_10,  11747,       27500,     1} ,//       1228
{KU_3530_NILESAT_101_10,  11785,       27500,     1} ,//       1229
{KU_3530_NILESAT_101_10,  11823,       27500,     1} ,//       1230
{KU_3530_NILESAT_101_10,  11862,       27500,     1} ,//       1231
{KU_3530_NILESAT_101_10,  11900,       27500,     1} ,//       1232
{KU_3530_NILESAT_101_10,  11938,       27500,     1} ,//       1233
{KU_3530_NILESAT_101_10,  11976,       27500,     1} ,//       1234
{KU_3530_NILESAT_101_10,  12015,       27500,     1} ,//       1235
{KU_3530_NILESAT_101_10,  12053,       27500,     1} ,//       1236
{KU_3530_NILESAT_101_10,  12130,       27500,     1} ,//       1237
{KU_3530_NILESAT_101_10,  12206,       27500,     1} ,//       1238
{KU_3530_NILESAT_101_10,  12284,       27500,     1} ,//       1239
{KU_3530_NILESAT_101_10,  12341,       27500,     1} ,//       1240            ----395

//KU_70_EUTELSAT_W3A
{KU_70_EUTELSAT_W3A,  10928,       30000,     0} ,//          0              ----396
{KU_70_EUTELSAT_W3A,  11345,       27500,     0} ,//          1
{KU_70_EUTELSAT_W3A,  11471,       30000,     0} ,//          2
{KU_70_EUTELSAT_W3A,  11513,       27500,     0} ,//          3
{KU_70_EUTELSAT_W3A,  11554,       27500,     0} ,//          4
{KU_70_EUTELSAT_W3A,  11595,       30000,     0} ,//          5
{KU_70_EUTELSAT_W3A,  11637,       30000,     0} ,//          6
{KU_70_EUTELSAT_W3A,  11679,       30000,     0} ,//          7
{KU_70_EUTELSAT_W3A,  10880,       17360,     1} ,//          8
{KU_70_EUTELSAT_W3A,  10880,       17360,     1} ,//          9
{KU_70_EUTELSAT_W3A,  11175,       27500,     1} ,//         10
{KU_70_EUTELSAT_W3A,  11283,       27500,     1} ,//         11
{KU_70_EUTELSAT_W3A,  11324,       27500,     1} ,//         12
{KU_70_EUTELSAT_W3A,  11452,       25066,     1} ,//         13
{KU_70_EUTELSAT_W3A,  11492,       30000,     1} ,//         14
{KU_70_EUTELSAT_W3A,  11534,       30000,     1} ,//         15
{KU_70_EUTELSAT_W3A,  11575,       30000,     1} ,//         16
{KU_70_EUTELSAT_W3A,  11617,       30000,     1} ,//         17            -----413

//KU_100_EUTELSAT_W1
{KU_100_EUTELSAT_W1,  11126,        4340,     0} ,//         18          -----414
{KU_100_EUTELSAT_W1,  11156,        3978,     0} ,//         19
{KU_100_EUTELSAT_W1,  11175,        2532,     0} ,//         20
{KU_100_EUTELSAT_W1,  12611,        9259,     0} ,//         21
{KU_100_EUTELSAT_W1,  11190,        5208,     1} ,//         22
{KU_100_EUTELSAT_W1,  12568,        3617,     1} ,//         23          -----419

//KU_260_BADR_2_3_4_EURO
{KU_260_BADR_2_3_4_EURO,  11727,       27500,     0} ,//        263             -------420
{KU_260_BADR_2_3_4_EURO,  11804,       27500,     0} ,//        264
{KU_260_BADR_2_3_4_EURO,  11843,       27500,     0} ,//        265
{KU_260_BADR_2_3_4_EURO,  11919,       27500,     0} ,//        266
{KU_260_BADR_2_3_4_EURO,  11957,       27500,     0} ,//        267
{KU_260_BADR_2_3_4_EURO,  11958,       27500,     0} ,//        268
{KU_260_BADR_2_3_4_EURO,  11996,       27500,     0} ,//        269
{KU_260_BADR_2_3_4_EURO,  12034,       27500,     0} ,//        270
{KU_260_BADR_2_3_4_EURO,  12072,       27500,     0} ,//        271
{KU_260_BADR_2_3_4_EURO,  12073,       27500,     0} ,//        272
{KU_260_BADR_2_3_4_EURO,  12111,       27500,     0} ,//        273
{KU_260_BADR_2_3_4_EURO,  12130,       27500,     0} ,//        274
{KU_260_BADR_2_3_4_EURO,  12149,       27500,     0} ,//        275
{KU_260_BADR_2_3_4_EURO,  12182,       16200,     0} ,//        276
{KU_260_BADR_2_3_4_EURO,  12226,       27500,     0} ,//        277
{KU_260_BADR_2_3_4_EURO,  12302,       27500,     0} ,//        278
{KU_260_BADR_2_3_4_EURO,  12523,       27500,     0} ,//        279
{KU_260_BADR_2_3_4_EURO,  11747,       27500,     1} ,//        280
{KU_260_BADR_2_3_4_EURO,  11785,       27500,     1} ,//        281
{KU_260_BADR_2_3_4_EURO,  11823,       27500,     1} ,//        282
{KU_260_BADR_2_3_4_EURO,  11862,       27500,     1} ,//        283
{KU_260_BADR_2_3_4_EURO,  11938,       27500,     1} ,//        284
{KU_260_BADR_2_3_4_EURO,  11976,       27500,     1} ,//        285
{KU_260_BADR_2_3_4_EURO,  12015,       27500,     1} ,//        286
{KU_260_BADR_2_3_4_EURO,  12053,       27500,     1} ,//        287
{KU_260_BADR_2_3_4_EURO,  12092,       27500,     1} ,//        288
{KU_260_BADR_2_3_4_EURO,  12130,       27500,     1} ,//        289
{KU_260_BADR_2_3_4_EURO,  12167,       27500,     1} ,//        290
{KU_260_BADR_2_3_4_EURO,  12206,       27500,     1} ,//        291
{KU_260_BADR_2_3_4_EURO,  12284,       27500,     1} ,//        292                ----449

//KU_360_EUTELSAT_SESAT
{KU_360_EUTELSAT_SESAT,  11142,       30000,     0} ,//        387                  ----450
{KU_360_EUTELSAT_SESAT,  11476,       29000,     0} ,//        388
{KU_360_EUTELSAT_SESAT,  11559,       29000,     0} ,//        389
{KU_360_EUTELSAT_SESAT,  12284,       26480,     0} ,//        390
{KU_360_EUTELSAT_SESAT,  12322,       26480,     0} ,//        391
{KU_360_EUTELSAT_SESAT,  12360,       26480,     0} ,//        392
{KU_360_EUTELSAT_SESAT,  12399,       26480,     0} ,//        393
{KU_360_EUTELSAT_SESAT,  12440,       23437,     0} ,//        394
{KU_360_EUTELSAT_SESAT,  12476,       26040,     0} ,//        395                -----458

//KU_160_EUTELSAT_W2
{KU_160_EUTELSAT_W2,  10976,       22500,     0} ,//        131                    -----459
{KU_160_EUTELSAT_W2,  11045,       10555,     0} ,//        132
{KU_160_EUTELSAT_W2,  11061,        5722,     0} ,//        133
{KU_160_EUTELSAT_W2,  11062,        5722,     0} ,//        134
{KU_160_EUTELSAT_W2,  11095,       27300,     0} ,//        135
{KU_160_EUTELSAT_W2,  11138,       27700,     0} ,//        136
{KU_160_EUTELSAT_W2,  11166,       11850,     0} ,//        137
{KU_160_EUTELSAT_W2,  11283,       27500,     0} ,//        138
{KU_160_EUTELSAT_W2,  11325,       27500,     0} ,//        139
{KU_160_EUTELSAT_W2,  11449,       27500,     0} ,//        140
{KU_160_EUTELSAT_W2,  12568,        3702,     0} ,//        141
{KU_160_EUTELSAT_W2,  12650,       15000,     0} ,//        142
{KU_160_EUTELSAT_W2,  12682,        2892,     0} ,//        143
{KU_160_EUTELSAT_W2,  12703,        2750,     0} ,//        144
{KU_160_EUTELSAT_W2,  12719,        9000,     0} ,//        145
{KU_160_EUTELSAT_W2,  12727,        3333,     0} ,//        146
{KU_160_EUTELSAT_W2,  10972,       27500,     1} ,//        147
{KU_160_EUTELSAT_W2,  11011,       27500,     1} ,//        148
{KU_160_EUTELSAT_W2,  11034,       27500,     1} ,//        149
{KU_160_EUTELSAT_W2,  11055,       27900,     1} ,//        150
{KU_160_EUTELSAT_W2,  11094,       27900,     1} ,//        151
{KU_160_EUTELSAT_W2,  11132,       14815,     1} ,//        152
{KU_160_EUTELSAT_W2,  11148,        8900,     1} ,//        153
{KU_160_EUTELSAT_W2,  11158,       27500,     1} ,//        154
{KU_160_EUTELSAT_W2,  11178,       27500,     1} ,//        155
{KU_160_EUTELSAT_W2,  11200,       27500,     1} ,//        156
{KU_160_EUTELSAT_W2,  11304,       30000,     1} ,//        157
{KU_160_EUTELSAT_W2,  11428,       30000,     1} ,//        158
{KU_160_EUTELSAT_W2,  11471,       29950,     1} ,//        159
{KU_160_EUTELSAT_W2,  11474,       29950,     1} ,//        160
{KU_160_EUTELSAT_W2,  11513,       29950,     1} ,//        161
{KU_160_EUTELSAT_W2,  11554,       30000,     1} ,//        162
{KU_160_EUTELSAT_W2,  11575,       30000,     1} ,//        163
{KU_160_EUTELSAT_W2,  11682,       14468,     1} ,//        164
{KU_160_EUTELSAT_W2,  12513,        2156,     1} ,//        165
{KU_160_EUTELSAT_W2,  12520,        2156,     1} ,//        166
{KU_160_EUTELSAT_W2,  12533,        5100,     1} ,//        167
{KU_160_EUTELSAT_W2,  12538,        4340,     1} ,//        168
{KU_160_EUTELSAT_W2,  12557,        2156,     1} ,//        169
{KU_160_EUTELSAT_W2,  12560,        2160,     1} ,//        170
{KU_160_EUTELSAT_W2,  12592,        2934,     1} ,//        171
{KU_160_EUTELSAT_W2,  12600,        6771,     1} ,//        172
{KU_160_EUTELSAT_W2,  12642,        3418,     1} ,//        173
{KU_160_EUTELSAT_W2,  12648,        5200,     1} ,//        174
{KU_160_EUTELSAT_W2,  12726,       30000,     1} ,//        175
{KU_160_EUTELSAT_W2,  12728,       26050,     1} ,//        176
{KU_160_EUTELSAT_W2,  12733,       26050,     1} ,//        177           -----505

//KU_400_EXPRESS_AM1
{KU_400_EXPRESS_AM1,  11097,        4000,     0} ,//        417  -----506
{KU_400_EXPRESS_AM1,  11679,        7596,     0} ,//        418
{KU_400_EXPRESS_AM1,  10967,       20000,     1} ,//        419
{KU_400_EXPRESS_AM1,  10995,       20000,     1} ,//        420
{KU_400_EXPRESS_AM1,  11159,        3333,     1} ,//        421 ------510

//KU_420_TURKSAT_2A_3A
{KU_420_TURKSAT_2A_3A,  10954 ,       2960,    0},
{KU_420_TURKSAT_2A_3A,  10958 ,       2960,    0},
{KU_420_TURKSAT_2A_3A,  10968 ,       11200,    0},
{KU_420_TURKSAT_2A_3A,  10982 ,       9600,    0},
{KU_420_TURKSAT_2A_3A,  11025 ,       4340,    0},
{KU_420_TURKSAT_2A_3A,  11027 ,       2965,    0},
{KU_420_TURKSAT_2A_3A,  11054 ,       30000,    0},
{KU_420_TURKSAT_2A_3A,  11096 ,       30000,    0},
{KU_420_TURKSAT_2A_3A,  11142 ,       2221,    0},
{KU_420_TURKSAT_2A_3A,  11146 ,       3330,    0},
{KU_420_TURKSAT_2A_3A,  11152 ,       2222,    0},
{KU_420_TURKSAT_2A_3A,  11156 ,       2960,    0},
{KU_420_TURKSAT_2A_3A,  11160 ,       2222,    0},
{KU_420_TURKSAT_2A_3A,  11165 ,       2200,    0},
{KU_420_TURKSAT_2A_3A,  11172 ,       4444,    0},
{KU_420_TURKSAT_2A_3A,  11177 ,       3600,    0},
{KU_420_TURKSAT_2A_3A,  11181 ,       2222,    0},
{KU_420_TURKSAT_2A_3A,  11187 ,       2080,    0},
{KU_420_TURKSAT_2A_3A,  11194 ,       7200,    0},
{KU_420_TURKSAT_2A_3A,  11540 ,       3600,    0},
{KU_420_TURKSAT_2A_3A,  11545 ,       4425,    0},
{KU_420_TURKSAT_2A_3A,  11562 ,       3333,    0},
{KU_420_TURKSAT_2A_3A,  11567 ,       3333,    0},
{KU_420_TURKSAT_2A_3A,  11572 ,       3333,    0},
{KU_420_TURKSAT_2A_3A,  11585 ,       2960,    0},
{KU_420_TURKSAT_2A_3A,  11624 ,       2960,    0},
{KU_420_TURKSAT_2A_3A,  11628 ,       6666,    0},
{KU_420_TURKSAT_2A_3A,  11651 ,       2222,    0},
{KU_420_TURKSAT_2A_3A,  11655 ,       3100,    0},
{KU_420_TURKSAT_2A_3A,  11659 ,       2222,    0},
{KU_420_TURKSAT_2A_3A,  11689 ,       2960,    0},
{KU_420_TURKSAT_2A_3A,  11746 ,       27500,    0},
{KU_420_TURKSAT_2A_3A,  11771 ,       2963,    0},
{KU_420_TURKSAT_2A_3A,  11777 ,       4800,    0},
{KU_420_TURKSAT_2A_3A,  11790 ,       16000,    0},
{KU_420_TURKSAT_2A_3A,  11862 ,       27500,    0},
{KU_420_TURKSAT_2A_3A,  11889 ,       2883,    0},
{KU_420_TURKSAT_2A_3A,  11906 ,       4800,    0},
{KU_420_TURKSAT_2A_3A,  11912 ,       3333,    0},
{KU_420_TURKSAT_2A_3A,  11934 ,       20000,    0},
{KU_420_TURKSAT_2A_3A,  11950 ,       7500,    0},
{KU_420_TURKSAT_2A_3A,  11964 ,       5925,    0},
{KU_420_TURKSAT_2A_3A,  11972 ,       7200,    0},
{KU_420_TURKSAT_2A_3A,  11981 ,       5200,    0},
{KU_420_TURKSAT_2A_3A,  11987 ,       2960,    0},
{KU_420_TURKSAT_2A_3A,  12015 ,       27500,    0},
{KU_420_TURKSAT_2A_3A,  12130 ,       27500,    0},
{KU_420_TURKSAT_2A_3A,  12519 ,       6000,    0},
{KU_420_TURKSAT_2A_3A,  12544 ,       24000,    0},
{KU_420_TURKSAT_2A_3A,  12592 ,       8888,    0},
{KU_420_TURKSAT_2A_3A,  12601 ,       4400,    0},
{KU_420_TURKSAT_2A_3A,  12607 ,       2950,    0},
{KU_420_TURKSAT_2A_3A,  12613 ,       4800,    0},
{KU_420_TURKSAT_2A_3A,  12617 ,       2800,    0},
{KU_420_TURKSAT_2A_3A,  12624 ,       2650,    0},
{KU_420_TURKSAT_2A_3A,  12627 ,       2650,    0},
{KU_420_TURKSAT_2A_3A,  12631 ,       2650,    0},
{KU_420_TURKSAT_2A_3A,  12634 ,       2650,    0},
{KU_420_TURKSAT_2A_3A,  12637 ,       2650,    0},
{KU_420_TURKSAT_2A_3A,  12643 ,       4800,    0},
{KU_420_TURKSAT_2A_3A,  12650 ,       4250,    0},
{KU_420_TURKSAT_2A_3A,  12656 ,       4444,    0},
{KU_420_TURKSAT_2A_3A,  12685 ,       30000,    0},
{KU_420_TURKSAT_2A_3A,  12729 ,       30000,    0},
{KU_420_TURKSAT_2A_3A,  10970 ,       30000,    1},
{KU_420_TURKSAT_2A_3A,  11012 ,       30000,    1},
{KU_420_TURKSAT_2A_3A,  11039 ,       3600,    1},
{KU_420_TURKSAT_2A_3A,  11049 ,       2400,    1},
{KU_420_TURKSAT_2A_3A,  11054 ,       2200,    1},
{KU_420_TURKSAT_2A_3A,  11064 ,       13000,    1},
{KU_420_TURKSAT_2A_3A,  11096 ,       30000,    1},
{KU_420_TURKSAT_2A_3A,  11156 ,       2222,    1},
{KU_420_TURKSAT_2A_3A,  11163 ,       4444,    1},
{KU_420_TURKSAT_2A_3A,  11170 ,       3600,    1},
{KU_420_TURKSAT_2A_3A,  11174 ,       2200,    1},
{KU_420_TURKSAT_2A_3A,  11178 ,       3600,    1},
{KU_420_TURKSAT_2A_3A,  11183 ,       2222,    1},
{KU_420_TURKSAT_2A_3A,  11196 ,       2222,    1},
{KU_420_TURKSAT_2A_3A,  11458 ,       3500,    1},
{KU_420_TURKSAT_2A_3A,  11464 ,       3500,    1},
{KU_420_TURKSAT_2A_3A,  11470 ,       3500,    1},
{KU_420_TURKSAT_2A_3A,  11475 ,       3500,    1},
{KU_420_TURKSAT_2A_3A,  11480 ,       4500,    1},
{KU_420_TURKSAT_2A_3A,  11487 ,       4500,    1},
{KU_420_TURKSAT_2A_3A,  11493 ,       2400,    1},
{KU_420_TURKSAT_2A_3A,  11496 ,       2200,    1},
{KU_420_TURKSAT_2A_3A,  11500 ,       2280,    1},
{KU_420_TURKSAT_2A_3A,  11519 ,       2960,    1},
{KU_420_TURKSAT_2A_3A,  11686 ,       2950,    1},
{KU_420_TURKSAT_2A_3A,  11712 ,       2963,    1},
{KU_420_TURKSAT_2A_3A,  11729 ,       15555,    1},
{KU_420_TURKSAT_2A_3A,  11742 ,       2965,    1},
{KU_420_TURKSAT_2A_3A,  11804 ,       24444,    1},
{KU_420_TURKSAT_2A_3A,  11830 ,       7200,    1},
{KU_420_TURKSAT_2A_3A,  11839 ,       4444,    1},
{KU_420_TURKSAT_2A_3A,  11845 ,       4444,    1},
{KU_420_TURKSAT_2A_3A,  11858 ,       3150,    1},
{KU_420_TURKSAT_2A_3A,  11870 ,       8888,    1},
{KU_420_TURKSAT_2A_3A,  11879 ,       2815,    1},
{KU_420_TURKSAT_2A_3A,  11885 ,       6000,    1},
{KU_420_TURKSAT_2A_3A,  11919 ,       24444,    1},
{KU_420_TURKSAT_2A_3A,  11948 ,       2960,    1},
{KU_420_TURKSAT_2A_3A,  11957 ,       2960,    1},
{KU_420_TURKSAT_2A_3A,  11962 ,       3333,    1},
{KU_420_TURKSAT_2A_3A,  11996 ,       27500,    1},
{KU_420_TURKSAT_2A_3A,  12130 ,       27500,    1},
{KU_420_TURKSAT_2A_3A,  12525 ,       30000,    1},
{KU_420_TURKSAT_2A_3A,  12561 ,       30000,    1},
{KU_420_TURKSAT_2A_3A,  12588 ,       3250,    1},
{KU_420_TURKSAT_2A_3A,  12595 ,       2500,    1},
{KU_420_TURKSAT_2A_3A,  12609 ,       3700,    1},
{KU_420_TURKSAT_2A_3A,  12615 ,       4800,    1},
{KU_420_TURKSAT_2A_3A,  12620 ,       2400,    1},
{KU_420_TURKSAT_2A_3A,  12624 ,       4000,    1},
{KU_420_TURKSAT_2A_3A,  12640 ,       2400,    1},
{KU_420_TURKSAT_2A_3A,  12644 ,       2960,    1},
{KU_420_TURKSAT_2A_3A,  12648 ,       2222,    1},
{KU_420_TURKSAT_2A_3A,  12652 ,       4444,    1},
{KU_420_TURKSAT_2A_3A,  12668 ,       2170,    1},
{KU_420_TURKSAT_2A_3A,  12672 ,       2278,    1},
{KU_420_TURKSAT_2A_3A,  12679 ,       8888,    1},
{KU_420_TURKSAT_2A_3A,  12686 ,       2222,    1},
{KU_420_TURKSAT_2A_3A,  12689 ,       2222,    1},
{KU_420_TURKSAT_2A_3A,  12694 ,       3333,    1},

//KU_620_INTELSAT_920
{KU_620_INTELSAT_920,  11105,        3328,     0} ,//        592   -----681
{KU_620_INTELSAT_920,  11108,        2170,     0} ,//        593
{KU_620_INTELSAT_920,  11148,        2710,     0} ,//        594
{KU_620_INTELSAT_920,  11151,        5787,     0} ,//        595
{KU_620_INTELSAT_920,  11152,        3333,     0} ,//        596
{KU_620_INTELSAT_920,  11175,        3333,     0} ,//        597
{KU_620_INTELSAT_920,  11186,        2046,     0} ,//        598
{KU_620_INTELSAT_920,  11189,        2221,     0} ,//        599
{KU_620_INTELSAT_920,  11190,        2480,     0} ,//        600
{KU_620_INTELSAT_920,  10973,       27500,     1} ,//        601
{KU_620_INTELSAT_920,  11043,        3600,     1} ,//        602
{KU_620_INTELSAT_920,  11068,        3610,     1} ,//        603
{KU_620_INTELSAT_920,  11073,        3600,     1} ,//        604
{KU_620_INTELSAT_920,  11078,        3610,     1} ,//        605
{KU_620_INTELSAT_920,  11083,        3600,     1} ,//        606
{KU_620_INTELSAT_920,  11088,        3610,     1} ,//        607
{KU_620_INTELSAT_920,  11093,        3600,     1} ,//        608
{KU_620_INTELSAT_920,  11103,       10000,     1} ,//        609    -------698

//C_686_INTELSAT_7_10
{C_686_INTELSAT_7_10,   3716,       20765,     0} ,//        610   -------699
{C_686_INTELSAT_7_10,   3745,       16175,     0} ,//        611
{C_686_INTELSAT_7_10,   3767,        6500,     0} ,//        612
{C_686_INTELSAT_7_10,   3774,        3300,     0} ,//        613
{C_686_INTELSAT_7_10,   3783,        3250,     0} ,//        614
{C_686_INTELSAT_7_10,   3811,        3313,     0} ,//        615
{C_686_INTELSAT_7_10,   3836,       19850,     0} ,//        616
{C_686_INTELSAT_7_10,   3864,       19850,     0} ,//        617
{C_686_INTELSAT_7_10,   3927,       26500,     0} ,//        618
{C_686_INTELSAT_7_10,   3974,       19500,     0} ,//        619
{C_686_INTELSAT_7_10,   4003,        2963,     0} ,//        620
{C_686_INTELSAT_7_10,   4034,       20500,     0} ,//        621
{C_686_INTELSAT_7_10,   4064,       19850,     0} ,//        622
{C_686_INTELSAT_7_10,   4087,        9375,     0} ,//        623
{C_686_INTELSAT_7_10,   4103,        5720,     0} ,//        624
{C_686_INTELSAT_7_10,   4117,        3333,     0} ,//        625
{C_686_INTELSAT_7_10,   4126,        3255,     0} ,//        626
{C_686_INTELSAT_7_10,   4131,        2572,     0} ,//        627
{C_686_INTELSAT_7_10,   4143,        3306,     0} ,//        628
{C_686_INTELSAT_7_10,   4150,        6600,     0} ,//        629
{C_686_INTELSAT_7_10,   4156,        2930,     0} ,//        630
{C_686_INTELSAT_7_10,   4160,        3618,     0} ,//        631
{C_686_INTELSAT_7_10,   4165,        3255,     0} ,//        632
{C_686_INTELSAT_7_10,   4184,       20000,     0} ,//        633
{C_686_INTELSAT_7_10,   3539,        3303,     1} ,//        634
{C_686_INTELSAT_7_10,   3662,       14074,     1} ,//        635
{C_686_INTELSAT_7_10,   3716,       20600,     1} ,//        636
{C_686_INTELSAT_7_10,   3743,       20600,     1} ,//        637
{C_686_INTELSAT_7_10,   3765,        2950,     1} ,//        638
{C_686_INTELSAT_7_10,   3777,        2940,     1} ,//        639
{C_686_INTELSAT_7_10,   3783,        2964,     1} ,//        640
{C_686_INTELSAT_7_10,   3787,        3617,     1} ,//        641
{C_686_INTELSAT_7_10,   3796,        7300,     1} ,//        642
{C_686_INTELSAT_7_10,   3808,       10340,     1} ,//        643
{C_686_INTELSAT_7_10,   3836,       20600,     1} ,//        644
{C_686_INTELSAT_7_10,   3863,       20600,     1} ,//        645
{C_686_INTELSAT_7_10,   3900,       12300,     1} ,//        646
{C_686_INTELSAT_7_10,   3913,        6510,     1} ,//        647
{C_686_INTELSAT_7_10,   3920,        3000,     1} ,//        648
{C_686_INTELSAT_7_10,   3924,        3003,     1} ,//        649
{C_686_INTELSAT_7_10,   3974,       19850,     1} ,//        650
{C_686_INTELSAT_7_10,   4011,        2200,     1} ,//        651
{C_686_INTELSAT_7_10,   4033,       19559,     1} ,//        652
{C_686_INTELSAT_7_10,   4054,        4400,     1} ,//        653
{C_686_INTELSAT_7_10,   4059,        3530,     1} ,//        654
{C_686_INTELSAT_7_10,   4064,        4610,     1} ,//        655
{C_686_INTELSAT_7_10,   4069,        3002,     1} ,//        656
{C_686_INTELSAT_7_10,   4075,        4003,     1} ,//        657
{C_686_INTELSAT_7_10,   4083,        3428,     1} ,//        658
{C_686_INTELSAT_7_10,   4124,       19850,     1} ,//        659
{C_686_INTELSAT_7_10,   4173,        3000,     1} ,//        660
{C_686_INTELSAT_7_10,   4178,        4000,     1} ,//        661
{C_686_INTELSAT_7_10,   4189,       12207,     1} ,//        662------751

//KU_305_ARABSAT_2B
{KU_305_ARABSAT_2B,  12582,        4220,     1} ,//        383------752
{KU_305_ARABSAT_2B,  12589,        4220,     1} ,//        384
{KU_305_ARABSAT_2B,  12661,       27500,     1} ,//        385
{KU_305_ARABSAT_2B,  12740,        5084,     1} ,//        386 ------755

//KU_390_HELLAS_SAT_2
{KU_390_HELLAS_SAT_2,  11512,       30000,     0} ,//        396   -----756
{KU_390_HELLAS_SAT_2,  12524,       30000,     0} ,//        397
{KU_390_HELLAS_SAT_2,  12559,       20000,     0} ,//        398
{KU_390_HELLAS_SAT_2,  12577,       10000,     0} ,//        399
{KU_390_HELLAS_SAT_2,  12606,       30000,     0} ,//        400
{KU_390_HELLAS_SAT_2,  12647,       30000,     0} ,//        401
{KU_390_HELLAS_SAT_2,  12688,       30000,     0} ,//        402
{KU_390_HELLAS_SAT_2,  12716,        7720,     0} ,//        403
{KU_390_HELLAS_SAT_2,  12717,        7720,     0} ,//        404
{KU_390_HELLAS_SAT_2,  10956,        4123,     1} ,//        405
{KU_390_HELLAS_SAT_2,  10969,        2893,     1} ,//        406
{KU_390_HELLAS_SAT_2,  11037,        5210,     1} ,//        407
{KU_390_HELLAS_SAT_2,  11099,        1478,     1} ,//        408
{KU_390_HELLAS_SAT_2,  11140,        1425,     1} ,//        409
{KU_390_HELLAS_SAT_2,  12524,       30000,     1} ,//        410
{KU_390_HELLAS_SAT_2,  12565,       30000,     1} ,//        411
{KU_390_HELLAS_SAT_2,  12647,       30000,     1} ,//        412
{KU_390_HELLAS_SAT_2,  12688,       30000,     1} ,//        413
{KU_390_HELLAS_SAT_2,  12715,        6654,     1} ,//        414
{KU_390_HELLAS_SAT_2,  12726,       30000,     1} ,//        415
{KU_390_HELLAS_SAT_2,  12729,       30000,     1} ,//        416  ------776

//KU_765_TELSTAR_10
{KU_765_TELSTAR_10,  12434,       10000,     0} ,//        663        -----777
{KU_765_TELSTAR_10,  12280,       22425,     1} ,//        664
{KU_765_TELSTAR_10,  12308,       22425,     1} ,//        665
{KU_765_TELSTAR_10,  12406,       30000,     1} ,//        666
{KU_765_TELSTAR_10,  12462,       22425,     1} ,//        667
{KU_765_TELSTAR_10,  12491,       22425,     1} ,//        668
{KU_765_TELSTAR_10,  12530,       30000,     1} ,//        669
{KU_765_TELSTAR_10,  12700,        5656,     1} ,//        670      ------784

//C_765_TELSTAR_10
{C_765_TELSTAR_10,   3656,        3215,     0} ,//        671            -----785
{C_765_TELSTAR_10,   3680,       27500,     0} ,//        672
{C_765_TELSTAR_10,   3720,       29270,     0} ,//        673
{C_765_TELSTAR_10,   3760,       28066,     0} ,//        674
{C_765_TELSTAR_10,   3787,        4600,     0} ,//        675
{C_765_TELSTAR_10,   3793,        4340,     0} ,//        676
{C_765_TELSTAR_10,   3840,       27500,     0} ,//        677
{C_765_TELSTAR_10,   3880,       28125,     0} ,//        678
{C_765_TELSTAR_10,   3921,       28343,     0} ,//        679
{C_765_TELSTAR_10,   4034,        4300,     0} ,//        680
{C_765_TELSTAR_10,   4049,        3600,     0} ,//        681
{C_765_TELSTAR_10,   4056,        3215,     0} ,//        682
{C_765_TELSTAR_10,   4071,       13021,     0} ,//        683
{C_765_TELSTAR_10,   4084,        2532,     0} ,//        684
{C_765_TELSTAR_10,   4089,        4300,     0} ,//        685
{C_765_TELSTAR_10,   4095,        4080,     0} ,//        686
{C_765_TELSTAR_10,   4106,        3125,     0} ,//        687
{C_765_TELSTAR_10,   4113,        2893,     0} ,//        688
{C_765_TELSTAR_10,   4146,        3600,     0} ,//        689
{C_765_TELSTAR_10,   4174,        7400,     0} ,//        690
{C_765_TELSTAR_10,   3646,        5065,     1} ,//        691
{C_765_TELSTAR_10,   3705,        6715,     1} ,//        692
{C_765_TELSTAR_10,   3780,       28062,     1} ,//        693
{C_765_TELSTAR_10,   4031,        6312,     1} ,//        694             -------808

//KU_785_THAICOM_2_5
{KU_785_THAICOM_2_5,  12271,       30000,     0} ,//        695            -------809
{KU_785_THAICOM_2_5,  12272,       30000,     0} ,//        696
{KU_785_THAICOM_2_5,  12313,       30000,     0} ,//        697
{KU_785_THAICOM_2_5,  12355,       30000,     0} ,//        698
{KU_785_THAICOM_2_5,  12438,       30000,     0} ,//        699
{KU_785_THAICOM_2_5,  12479,       30000,     0} ,//        700
{KU_785_THAICOM_2_5,  12521,       30000,     0} ,//        701
{KU_785_THAICOM_2_5,  12562,       25776,     0} ,//        702
{KU_785_THAICOM_2_5,  12604,       30000,     0} ,//        703
{KU_785_THAICOM_2_5,  12657,       30000,     0} ,//        704
{KU_785_THAICOM_2_5,  12720,       30000,     0} ,//        705
{KU_785_THAICOM_2_5,  12313,       30000,     1} ,//        706           --------820

//C_785_THAICOM_2_5
{C_785_THAICOM_2_5,   3431,       13333,     0} ,//        707           --------821
{C_785_THAICOM_2_5,   3449,       13330,     0} ,//        708
{C_785_THAICOM_2_5,   3505,        3368,     0} ,//        709
{C_785_THAICOM_2_5,   3551,       13333,     0} ,//        710
{C_785_THAICOM_2_5,   3563,        4444,     0} ,//        711
{C_785_THAICOM_2_5,   3575,        7495,     0} ,//        712
{C_785_THAICOM_2_5,   3600,       26670,     0} ,//        713
{C_785_THAICOM_2_5,   3640,       28062,     0} ,//        714
{C_785_THAICOM_2_5,   3694,        5000,     0} ,//        715
{C_785_THAICOM_2_5,   3421,        6666,     1} ,//        716
{C_785_THAICOM_2_5,   3432,        6664,     1} ,//        717
{C_785_THAICOM_2_5,   3449,        2222,     1} ,//        718
{C_785_THAICOM_2_5,   3454,        3333,     1} ,//        719
{C_785_THAICOM_2_5,   3490,        3333,     1} ,//        720
{C_785_THAICOM_2_5,   3502,        4445,     1} ,//        721
{C_785_THAICOM_2_5,   3522,        2915,     1} ,//        722
{C_785_THAICOM_2_5,   3544,       30000,     1} ,//        723
{C_785_THAICOM_2_5,   3585,       26667,     1} ,//        724
{C_785_THAICOM_2_5,   3609,        2500,     1} ,//        725
{C_785_THAICOM_2_5,   3617,       12500,     1} ,//        726
{C_785_THAICOM_2_5,   3711,       13330,     1} ,//        727
{C_785_THAICOM_2_5,   3760,       26660,     1} ,//        728
{C_785_THAICOM_2_5,   3814,        6660,     1} ,//        729
{C_785_THAICOM_2_5,   3840,       26667,     1} ,//        730
{C_785_THAICOM_2_5,   3873,        2963,     1} ,//        731
{C_785_THAICOM_2_5,   3877,        3333,     1} ,//        732
{C_785_THAICOM_2_5,   3920,       30000,     1} ,//        733
{C_785_THAICOM_2_5,   3960,       30000,     1} ,//        734
{C_785_THAICOM_2_5,   3991,       13330,     1} ,//        735
{C_785_THAICOM_2_5,   4005,        6665,     1} ,//        736
{C_785_THAICOM_2_5,   4107,        6665,     1} ,//        737
{C_785_THAICOM_2_5,   4136,        4443,     1} ,//        738
{C_785_THAICOM_2_5,   4169,        9600,     1} ,//        739             ------853

//KU_830_INSAT_3B_4A
{KU_830_INSAT_3B_4A,  10970,       27500,     0} ,//        740            ----854
{KU_830_INSAT_3B_4A,  11010,       27500,     0} ,//        741
{KU_830_INSAT_3B_4A,  11050,       27500,     0} ,//        742
{KU_830_INSAT_3B_4A,  11090,       27500,     0} ,//        743
{KU_830_INSAT_3B_4A,  11130,       27500,     0} ,//        744
{KU_830_INSAT_3B_4A,  11170,       27500,     0} ,//        745
{KU_830_INSAT_3B_4A,  11470,       27500,     0} ,//        746
{KU_830_INSAT_3B_4A,  11510,       27500,     0} ,//        747
{KU_830_INSAT_3B_4A,  11550,       27500,     0} ,//        748
{KU_830_INSAT_3B_4A,  11590,       27500,     0} ,//        749
{KU_830_INSAT_3B_4A,  11630,       27500,     0} ,//        750
{KU_830_INSAT_3B_4A,  11670,       27500,     0} ,//        751             ----865

//KU_900_YAMAL_210
{KU_900_YAMAL_210,  10995,        4285,     1} ,//        752          0  900  YAMAL 201    -----866
{KU_900_YAMAL_210,  11057,       26470,     1} ,//        753          0  900  YAMAL 201
{KU_900_YAMAL_210,  11092,       26470,     1} ,//        754          0  900  YAMAL 201   -----868

//KU_950_NSS_6
{KU_950_NSS_6,  11038,       40700,     0} ,//        755          0  950  INSAT 4B, NSS 6   ------869
{KU_950_NSS_6,  11091,       29500,     0} ,//        756          0  950  INSAT 4B, NSS 6
{KU_950_NSS_6,  11172,       27500,     0} ,//        757          0  950  INSAT 4B, NSS 6
{KU_950_NSS_6,  12595,       40700,     0} ,//        758          0  950  INSAT 4B, NSS 6
{KU_950_NSS_6,  12647,       27500,     0} ,//        759          0  950  INSAT 4B, NSS 6
{KU_950_NSS_6,  12688,       27500,     0} ,//        760          0  950  INSAT 4B, NSS 6
{KU_950_NSS_6,  10990,       27500,     1} ,//        761          0  950  INSAT 4B, NSS 6
{KU_950_NSS_6,  11030,       27500,     1} ,//        762          0  950  INSAT 4B, NSS 6
{KU_950_NSS_6,  11070,       27500,     1} ,//        763          0  950  INSAT 4B, NSS 6
{KU_950_NSS_6,  11110,       27500,     1} ,//        764          0  950  INSAT 4B, NSS 6
{KU_950_NSS_6,  11150,       27500,     1} ,//        765          0  950  INSAT 4B, NSS 6
{KU_950_NSS_6,  11185,       20000,     1} ,//        766          0  950  INSAT 4B, NSS 6
{KU_950_NSS_6,  11490,       27500,     1} ,//        767          0  950  INSAT 4B, NSS 6
{KU_950_NSS_6,  11530,       27500,     1} ,//        768          0  950  INSAT 4B, NSS 6
{KU_950_NSS_6,  11570,       27500,     1} ,//        769          0  950  INSAT 4B, NSS 6
{KU_950_NSS_6,  11610,       27500,     1} ,//        770          0  950  INSAT 4B, NSS 6
{KU_950_NSS_6,  11645,       20000,     1} ,//        771          0  950  INSAT 4B, NSS 6
{KU_950_NSS_6,  11682,        9500,     1} ,//        772          0  950  INSAT 4B, NSS 6
{KU_950_NSS_6,  12534,       40700,     1} ,//        773          0  950  INSAT 4B, NSS 6
{KU_950_NSS_6,  12595,       40700,     1} ,//        774          0  950  INSAT 4B, NSS 6
{KU_950_NSS_6,  12638,       13300,     1} ,//        775          0  950  INSAT 4B, NSS 6
{KU_950_NSS_6,  12688,       27500,     1} ,//        776          0  950  INSAT 4B, NSS 6
{KU_950_NSS_6,  12729,       27500,     1} ,//        777          0  950  INSAT 4B, NSS 6  ------891

//C_1005_ASIASAT_2
{C_1005_ASIASAT_2,   3637,        5400,     0} ,//        778          0 1005  ASIASAT 2   ------892
{C_1005_ASIASAT_2,   3650,        4000,     0} ,//        779          0 1005  ASIASAT 2
{C_1005_ASIASAT_2,   3705,        4167,     0} ,//        780          1 1005  ASIASAT 2
{C_1005_ASIASAT_2,   3760,       27500,     0} ,//        781          1 1005  ASIASAT 2
{C_1005_ASIASAT_2,   3786,        6000,     0} ,//        782          1 1005  ASIASAT 2
{C_1005_ASIASAT_2,   3800,        5632,     0} ,//        783          0 1005  ASIASAT 2
{C_1005_ASIASAT_2,   3905,        4000,     0} ,//        784          0 1005  ASIASAT 2
{C_1005_ASIASAT_2,   3960,       27500,     0} ,//        785          1 1005  ASIASAT 2
{C_1005_ASIASAT_2,   4000,       28125,     0} ,//        786          1 1005  ASIASAT 2
{C_1005_ASIASAT_2,   4117,       21700,     0} ,//        787          0 1005  ASIASAT 2
{C_1005_ASIASAT_2,   4160,        4680,     0} ,//        788          0 1005  ASIASAT 2
{C_1005_ASIASAT_2,   4167,        2965,     0} ,//        789          0 1005  ASIASAT 2
{C_1005_ASIASAT_2,   3660,       27500,     1} ,//        790          1 1005  ASIASAT 2
{C_1005_ASIASAT_2,   3713,        6110,     1} ,//        791          0 1005  ASIASAT 2
{C_1005_ASIASAT_2,   3766,        6528,     1} ,//        792          0 1005  ASIASAT 2
{C_1005_ASIASAT_2,   3778,        3333,     1} ,//        793          0 1005  ASIASAT 2
{C_1005_ASIASAT_2,   3791,        2444,     1} ,//        794          1 1005  ASIASAT 2
{C_1005_ASIASAT_2,   3820,       27500,     1} ,//        795          1 1005  ASIASAT 2
{C_1005_ASIASAT_2,   3860,       30000,     1} ,//        796          0 1005  ASIASAT 2
{C_1005_ASIASAT_2,   3905,       20926,     1} ,//        797          0 1005  ASIASAT 2
{C_1005_ASIASAT_2,   3940,       27500,     1} ,//        798          0 1005  ASIASAT 2
{C_1005_ASIASAT_2,   3940,       17500,     1} ,//        799          0 1005  ASIASAT 2
{C_1005_ASIASAT_2,   3980,       28100,     1} ,//        800          0 1005  ASIASAT 2
{C_1005_ASIASAT_2,   4062,        6660,     1} ,//        801          0 1005  ASIASAT 2
{C_1005_ASIASAT_2,   4148,       11852,     1} ,//        802          1 1005  ASIASAT 2  --------916

//KU_1005_ASIASAT_2
{KU_1005_ASIASAT_2,  12245,        6200,     0} ,//        803          0 1005  ASIASAT 2  --------917
{KU_1005_ASIASAT_2,  12278,        3111,     0} ,//        804          0 1005  ASIASAT 2
{KU_1005_ASIASAT_2,  12285,        3100,     0} ,//        805          0 1005  ASIASAT 2
{KU_1005_ASIASAT_2,  12331,        9000,     0} ,//        806          0 1005  ASIASAT 2
{KU_1005_ASIASAT_2,  12345,        5630,     0} ,//        807          0 1005  ASIASAT 2
{KU_1005_ASIASAT_2,  12390,        6111,     0} ,//        808          0 1005  ASIASAT 2
{KU_1005_ASIASAT_2,  12468,        7234,     0} ,//        809          0 1005  ASIASAT 2
{KU_1005_ASIASAT_2,  12222,        6000,     1} ,//        810          0 1005  ASIASAT 2
{KU_1005_ASIASAT_2,  12259,        7000,     1} ,//        811          0 1005  ASIASAT 2
{KU_1005_ASIASAT_2,  12310,        6000,     1} ,//        812          0 1005  ASIASAT 2
{KU_1005_ASIASAT_2,  12367,        2048,     1} ,//        813          0 1005  ASIASAT 2
{KU_1005_ASIASAT_2,  12445,        5200,     1} ,//        814          0 1005  ASIASAT 2   -------928

//C_1055_ASIASAT_3S
{C_1055_ASIASAT_3S,   3680,       26670,     0} ,//        815          1 1055  ASIASAT 3S  ------929
{C_1055_ASIASAT_3S,   3706,        6000,     0} ,//        816          1 1055  ASIASAT 3S
{C_1055_ASIASAT_3S,   3729,       13650,     0} ,//        817          1 1055  ASIASAT 3S
{C_1055_ASIASAT_3S,   3760,       26000,     0} ,//        818          1 1055  ASIASAT 3S
{C_1055_ASIASAT_3S,   3786,        5710,     0} ,//        819          1 1055  ASIASAT 3S
{C_1055_ASIASAT_3S,   3792,        3000,     0} ,//        820          1 1055  ASIASAT 3S
{C_1055_ASIASAT_3S,   3816,        2515,     0} ,//        821          0 1055  ASIASAT 3S
{C_1055_ASIASAT_3S,   3840,       26850,     0} ,//        822          0 1055  ASIASAT 3S
{C_1055_ASIASAT_3S,   3880,       27500,     0} ,//        823          1 1055  ASIASAT 3S
{C_1055_ASIASAT_3S,   3920,       26850,     0} ,//        824          0 1055  ASIASAT 3S
{C_1055_ASIASAT_3S,   3960,       27500,     0} ,//        825          0 1055  ASIASAT 3S
{C_1055_ASIASAT_3S,   4000,       26850,     0} ,//        826          1 1055  ASIASAT 3S
{C_1055_ASIASAT_3S,   4040,       20400,     0} ,//        827          1 1055  ASIASAT 3S
{C_1055_ASIASAT_3S,   4094,        5555,     0} ,//        828          1 1055  ASIASAT 3S
{C_1055_ASIASAT_3S,   4111,       13650,     0} ,//        829          1 1055  ASIASAT 3S
{C_1055_ASIASAT_3S,   4149,        2500,     0} ,//        830          0 1055  ASIASAT 3S
{C_1055_ASIASAT_3S,   4171,        5000,     0} ,//        831          0 1055  ASIASAT 3S
{C_1055_ASIASAT_3S,   3700,       27500,     1} ,//        832          0 1055  ASIASAT 3S
{C_1055_ASIASAT_3S,   3725,        4450,     1} ,//        833          1 1055  ASIASAT 3S
{C_1055_ASIASAT_3S,   3733,        6500,     1} ,//        834          0 1055  ASIASAT 3S
{C_1055_ASIASAT_3S,   3745,        2626,     1} ,//        835          1 1055  ASIASAT 3S
{C_1055_ASIASAT_3S,   3750,        2820,     1} ,//        836          1 1055  ASIASAT 3S
{C_1055_ASIASAT_3S,   3755,        4418,     1} ,//        837          1 1055  ASIASAT 3S
{C_1055_ASIASAT_3S,   3780,       28100,     1} ,//        838          1 1055  ASIASAT 3S
{C_1055_ASIASAT_3S,   3860,       27500,     1} ,//        839          0 1055  ASIASAT 3S
{C_1055_ASIASAT_3S,   3940,       28100,     1} ,//        840          0 1055  ASIASAT 3S
{C_1055_ASIASAT_3S,   3980,       28100,     1} ,//        841          0 1055  ASIASAT 3S
{C_1055_ASIASAT_3S,   4020,       27250,     1} ,//        842          1 1055  ASIASAT 3S
{C_1055_ASIASAT_3S,   4091,       13333,     1} ,//        843          1 1055  ASIASAT 3S
{C_1055_ASIASAT_3S,   4102,        2893,     1} ,//        844          1 1055  ASIASAT 3S
{C_1055_ASIASAT_3S,   4107,        2893,     1} ,//        845          1 1055  ASIASAT 3S
{C_1055_ASIASAT_3S,   4114,        7000,     1} ,//        846          1 1055  ASIASAT 3S
{C_1055_ASIASAT_3S,   4140,       27500,     1} ,//        847          1 1055  ASIASAT 3S
{C_1055_ASIASAT_3S,   4180,       26660,     1} ,//        848          1 1055  ASIASAT 3S -----------962

//KU_1055_ASIASAT_3S
{KU_1055_ASIASAT_3S,  12316,        5000,     0} ,//        849          0 1055  ASIASAT 3S -----------963
{KU_1055_ASIASAT_3S,  12351,        2893,     0} ,//        850          0 1055  ASIASAT 3S
{KU_1055_ASIASAT_3S,  12362,        3400,     0} ,//        851          0 1055  ASIASAT 3S
{KU_1055_ASIASAT_3S,  12392,        5000,     0} ,//        852          0 1055  ASIASAT 3S
{KU_1055_ASIASAT_3S,  12402,        3000,     0} ,//        853          0 1055  ASIASAT 3S
{KU_1055_ASIASAT_3S,  12419,        3000,     0} ,//        854          0 1055  ASIASAT 3S
{KU_1055_ASIASAT_3S,  12450,        7900,     0} ,//        855          0 1055  ASIASAT 3S
{KU_1055_ASIASAT_3S,  12472,       20000,     0} ,//        856          0 1055  ASIASAT 3S
{KU_1055_ASIASAT_3S,  12495,        2000,     0} ,//        857          0 1055  ASIASAT 3S
{KU_1055_ASIASAT_3S,  12502,        4341,     0} ,//        858          0 1055  ASIASAT 3S
{KU_1055_ASIASAT_3S,  12511,        4340,     0} ,//        859          0 1055  ASIASAT 3S
{KU_1055_ASIASAT_3S,  12534,       10000,     0} ,//        860          0 1055  ASIASAT 3S
{KU_1055_ASIASAT_3S,  12623,       14637,     0} ,//        861          0 1055  ASIASAT 3S
{KU_1055_ASIASAT_3S,  12688,        5640,     0} ,//        862          0 1055  ASIASAT 3S
{KU_1055_ASIASAT_3S,  12696,        5640,     0} ,//        863          0 1055  ASIASAT 3S
{KU_1055_ASIASAT_3S,  12705,        5640,     0} ,//        864          0 1055  ASIASAT 3S    ---978

//C_1155_CHINASAT_6B
{C_1155_CHINASAT_6B,   3706,        4420,     1} ,//        865          1 1155  CHINASAT 6B   ----979
{C_1155_CHINASAT_6B,   3730,       10720,     1} ,//        866          0 1155  CHINASAT 6B
{C_1155_CHINASAT_6B,   3750,       10490,     1} ,//        867          1 1155  CHINASAT 6B
{C_1155_CHINASAT_6B,   3771,        9375,     1} ,//        868          1 1155  CHINASAT 6B
{C_1155_CHINASAT_6B,   3786,        5440,     1} ,//        869          1 1155  CHINASAT 6B
{C_1155_CHINASAT_6B,   3796,        6930,     1} ,//        870          1 1155  CHINASAT 6B
{C_1155_CHINASAT_6B,   3808,        8800,     1} ,//        871          1 1155  CHINASAT 6B
{C_1155_CHINASAT_6B,   3840,       27500,     1} ,//        872          1 1155  CHINASAT 6B
{C_1155_CHINASAT_6B,   3880,       27500,     1} ,//        873          0 1155  CHINASAT 6B
{C_1155_CHINASAT_6B,   3950,       11400,     1} ,//        874          1 1155  CHINASAT 6B
{C_1155_CHINASAT_6B,   4000,       27500,     1} ,//        875          1 1155  CHINASAT 6B
{C_1155_CHINASAT_6B,   4040,       27500,     1} ,//        876          0 1155  CHINASAT 6B
{C_1155_CHINASAT_6B,   4080,       27500,     1} ,//        877          0 1155  CHINASAT 6B
{C_1155_CHINASAT_6B,   4111,       13300,     1} ,//        878          0 1155  CHINASAT 6B
{C_1155_CHINASAT_6B,   4129,       13300,     1} ,//        879          0 1155  CHINASAT 6B
{C_1155_CHINASAT_6B,   4147,        6150,     1} ,//        880          1 1155  CHINASAT 6B
{C_1155_CHINASAT_6B,   4158,        8680,     1} ,//        881          1 1155  CHINASAT 6B
{C_1155_CHINASAT_6B,   4171,        9200,     1} ,//        882          1 1155  CHINASAT 6B
{C_1155_CHINASAT_6B,   3600,       27500,     0} ,//        883          0 1155  CHINASAT 6B
{C_1155_CHINASAT_6B,   3640,       27500,     0} ,//        884          0 1155  CHINASAT 6B
{C_1155_CHINASAT_6B,   3680,       27500,     0} ,//        885          1 1155  CHINASAT 6B
{C_1155_CHINASAT_6B,   3740,       27500,     0} ,//        886          0 1155  CHINASAT 6B
{C_1155_CHINASAT_6B,   3780,       27500,     0} ,//        887          0 1155  CHINASAT 6B
{C_1155_CHINASAT_6B,   3807,        6000,     0} ,//        888          1 1155  CHINASAT 6B
{C_1155_CHINASAT_6B,   3825,        6780,     0} ,//        889          1 1155  CHINASAT 6B
{C_1155_CHINASAT_6B,   3834,        5400,     0} ,//        890          1 1155  CHINASAT 6B
{C_1155_CHINASAT_6B,   3846,        5950,     0} ,//        891          1 1155  CHINASAT 6B
{C_1155_CHINASAT_6B,   3854,        4420,     0} ,//        892          1 1155  CHINASAT 6B
{C_1155_CHINASAT_6B,   3861,        4800,     0} ,//        893          1 1155  CHINASAT 6B
{C_1155_CHINASAT_6B,   3871,        9080,     0} ,//        894          1 1155  CHINASAT 6B
{C_1155_CHINASAT_6B,   3885,        4340,     0} ,//        895          1 1155  CHINASAT 6B
{C_1155_CHINASAT_6B,   3892,        4420,     0} ,//        896          1 1155  CHINASAT 6B
{C_1155_CHINASAT_6B,   3900,        6670,     0} ,//        897          1 1155  CHINASAT 6B
{C_1155_CHINASAT_6B,   3910,        6400,     0} ,//        898          1 1155  CHINASAT 6B
{C_1155_CHINASAT_6B,   3929,        8840,     0} ,//        899          1 1155  CHINASAT 6B
{C_1155_CHINASAT_6B,   3940,        5950,     0} ,//        900          1 1155  CHINASAT 6B
{C_1155_CHINASAT_6B,   3951,        9520,     0} ,//        901          1 1155  CHINASAT 6B
{C_1155_CHINASAT_6B,   3980,       27500,     0} ,//        902          0 1155  CHINASAT 6B
{C_1155_CHINASAT_6B,   4020,       27500,     0} ,//        903          0 1155  CHINASAT 6B
{C_1155_CHINASAT_6B,   4060,       27500,     0} ,//        904          1 1155  CHINASAT 6B
{C_1155_CHINASAT_6B,   4100,       27500,     0} ,//        905          1 1155  CHINASAT 6B
{C_1155_CHINASAT_6B,   4140,       27500,     0} ,//        906          0 1155  CHINASAT 6B
{C_1155_CHINASAT_6B,   4175,        5990,     0} ,//        907          1 1155  CHINASAT 6B
{C_1155_CHINASAT_6B,   4192,        6000,     0} ,//        908          1 1155  CHINASAT 6B   -------1022

//C_1250_SINOSAT_3
{C_1250_SINOSAT_3,   3827,        5720,     0} ,//        909          1 1250  SINOSAT-3    -----1023
{C_1250_SINOSAT_3,   3845,       17778,     0} ,//        910          1 1250  SINOSAT-3
{C_1250_SINOSAT_3,   3885,        5640,     0} ,//        911          1 1250  SINOSAT-3
{C_1250_SINOSAT_3,   3893,        6880,     0} ,//        912          1 1250  SINOSAT-3
{C_1250_SINOSAT_3,   3909,        8934,     0} ,//        913          1 1250  SINOSAT-3
{C_1250_SINOSAT_3,   3923,        7250,     0} ,//        914          1 1250  SINOSAT-3
{C_1250_SINOSAT_3,   3933,        6590,     0} ,//        915          1 1250  SINOSAT-3
{C_1250_SINOSAT_3,   3989,        9078,     0} ,//        916          1 1250  SINOSAT-3
{C_1250_SINOSAT_3,   4000,        4420,     0} ,//        917          1 1250  SINOSAT-3
{C_1250_SINOSAT_3,   4006,        4420,     0} ,//        918          1 1250  SINOSAT-3
{C_1250_SINOSAT_3,   4040,       27500,     0} ,//        919          1 1250  SINOSAT-3
{C_1250_SINOSAT_3,   4080,       27500,     0} ,//        920          1 1250  SINOSAT-3
{C_1250_SINOSAT_3,   4120,       27500,     0} ,//        921          1 1250  SINOSAT-3
{C_1250_SINOSAT_3,   4160,       27500,     0} ,//        922          1 1250  SINOSAT-3   -----1036

//C_1340_APSTAR_6
{C_1340_APSTAR_6,   3766,        4285,     0} ,//        923          0 1340  APSTAR 6   -----1037
{C_1340_APSTAR_6,   3840,       27500,     0} ,//        924          0 1340  APSTAR 6
{C_1340_APSTAR_6,   3904,        3250,     0} ,//        925          0 1340  APSTAR 6   -----1039

//KU_1340_APSTAR_6
{KU_1340_APSTAR_6,  12275,       27500,     1} ,//        926          0 1340  APSTAR 6   -----1040
{KU_1340_APSTAR_6,  12302,        5990,     1} ,//        927          0 1340  APSTAR 6
{KU_1340_APSTAR_6,  12355,       27500,     1} ,//        928          0 1340  APSTAR 6
{KU_1340_APSTAR_6,  12395,       27500,     1} ,//        929          0 1340  APSTAR 6
{KU_1340_APSTAR_6,  12435,       27500,     1} ,//        930          0 1340  APSTAR 6
{KU_1340_APSTAR_6,  12515,       27500,     1} ,//        931          0 1340  APSTAR 6
{KU_1340_APSTAR_6,  12595,       27500,     1} ,//        932          0 1340  APSTAR 6
{KU_1340_APSTAR_6,  12675,       27500,     1} ,//        933          0 1340  APSTAR 6   -----1047

//C_1660_INTELSAT_8
{C_1660_INTELSAT_8,   3740,       27500,     0} ,//        934          0 1660  INTELSAT 8  ----1048
{C_1660_INTELSAT_8,   3808,        5632,     0} ,//        935          0 1660  INTELSAT 8
{C_1660_INTELSAT_8,   3815,        4400,     0} ,//        936          0 1660  INTELSAT 8
{C_1660_INTELSAT_8,   3880,       28700,     0} ,//        937          0 1660  INTELSAT 8
{C_1660_INTELSAT_8,   3790,        6507,     1} ,//        938          0 1660  INTELSAT 8
{C_1660_INTELSAT_8,   3808,        5632,     1} ,//        939          0 1660  INTELSAT 8
{C_1660_INTELSAT_8,   3815,        4400,     1} ,//        940          0 1660  INTELSAT 8
{C_1660_INTELSAT_8,   3833,       17500,     1} ,//        941          0 1660  INTELSAT 8
{C_1660_INTELSAT_8,   3880,       28700,     1} ,//        942          0 1660  INTELSAT 8
{C_1660_INTELSAT_8,   3949,        4613,     1} ,//        943          0 1660  INTELSAT 8
{C_1660_INTELSAT_8,   3960,        3428,     1} ,//        944          0 1660  INTELSAT 8 -------1058

//KU_1660_INTELSAT_8
{KU_1660_INTELSAT_8,  12286,       28800,     0} ,//        945          0 1660  INTELSAT 8 -------1059
{KU_1660_INTELSAT_8,  12326,       28800,     0} ,//        946          0 1660  INTELSAT 8
{KU_1660_INTELSAT_8,  12526,       28800,     0} ,//        947          0 1660  INTELSAT 8
{KU_1660_INTELSAT_8,  12575,       13845,     0} ,//        948          0 1660  INTELSAT 8
{KU_1660_INTELSAT_8,  12646,       28060,     0} ,//        949          0 1660  INTELSAT 8
{KU_1660_INTELSAT_8,  12686,       28125,     0} ,//        950          0 1660  INTELSAT 8
{KU_1660_INTELSAT_8,  12726,       28060,     0} ,//        951          0 1660  INTELSAT 8
{KU_1660_INTELSAT_8,  12502,        3074,     1} ,//        952          0 1660  INTELSAT 8  ----1066

//C_1690_INTELSAT_2
{C_1690_INTELSAT_2,   3901,       30800,     0} ,//        953          0 1690  INTELSAT 2  -----1067
{C_1690_INTELSAT_2,   4022,        5900,     0} ,//        954          0 1690  INTELSAT 2
{C_1690_INTELSAT_2,   3845,        4604,     1} ,//        955          0 1690  INTELSAT 2
{C_1690_INTELSAT_2,   3958,        6620,     1} ,//        956          0 1690  INTELSAT 2
{C_1690_INTELSAT_2,   3992,       26470,     1} ,//        957          0 1690  INTELSAT 2 -----1071

//KU_1690_INTELSAT_2
{KU_1690_INTELSAT_2,  12281,        3700,     0} ,//        958          0 1690  INTELSAT 2  -----1072
{KU_1690_INTELSAT_2,  12400,        3000,     0} ,//        959          0 1690  INTELSAT 2
{KU_1690_INTELSAT_2,  12281,       27500,     1} ,//        960          0 1690  INTELSAT 2
{KU_1690_INTELSAT_2,  12401,        4400,     1} ,//        961          0 1690  INTELSAT 2
{KU_1690_INTELSAT_2,  12604,        5429,     1} ,//        962          0 1690  INTELSAT 2   -----1076

//C_2330_HORIZONS_1
{C_2330_HORIZONS_1,   3800,       27690,     1} ,//        963          0 2330  HORIZONS 1  --1077

//KU_2330_HORIZONS_1
{KU_2330_HORIZONS_1,  11720,       26800,     0} ,//        964          0 2330  HORIZONS 1 ---1078

//KU_2370_GALAXY_10R
{KU_2370_GALAXY_10R,  11731,       13240,     0} ,//        965          0 2370  GALAXY 10R  -----1079
{KU_2370_GALAXY_10R,  11732,       13240,     0} ,//        966          0 2370  GALAXY 10R
{KU_2370_GALAXY_10R,  11805,        4580,     0} ,//        967          0 2370  GALAXY 10R
{KU_2370_GALAXY_10R,  11720,       27692,     1} ,//        968          0 2370  GALAXY 10R
{KU_2370_GALAXY_10R,  11798,       26657,     1} ,//        969          0 2370  GALAXY 10R
{KU_2370_GALAXY_10R,  11905,        4352,     1} ,//        970          0 2370  GALAXY 10R
{KU_2370_GALAXY_10R,  11965,        7234,     1} ,//        971          0 2370  GALAXY 10R
{KU_2370_GALAXY_10R,  12078,        3680,     1} ,//        972          0 2370  GALAXY 10R
{KU_2370_GALAXY_10R,  12104,        2222,     1} ,//        973          0 2370  GALAXY 10R
{KU_2370_GALAXY_10R,  12114,        4340,     1} ,//        974          0 2370  GALAXY 10R  ------1088

//C_2370_GALAXY_10R
{C_2370_GALAXY_10R,   3975,       19510,     0} ,//        975          0 2370  GALAXY 10R  ------1089
{C_2370_GALAXY_10R,   3992,        9765,     0} ,//        976          0 2370  GALAXY 10R
{C_2370_GALAXY_10R,   4100,       29270,     0} ,//        977          0 2370  GALAXY 10R
{C_2370_GALAXY_10R,   4140,       29270,     0} ,//        978          0 2370  GALAXY 10R
{C_2370_GALAXY_10R,   4180,       29270,     0} ,//        979          0 2370  GALAXY 10R
{C_2370_GALAXY_10R,   3795,       19510,     1} ,//        980          0 2370  GALAXY 10R
{C_2370_GALAXY_10R,   3812,        9760,     1} ,//        981          0 2370  GALAXY 10R
{C_2370_GALAXY_10R,   3837,       19510,     1} ,//        982          0 2370  GALAXY 10R
{C_2370_GALAXY_10R,   3840,       26470,     1} ,//        983          0 2370  GALAXY 10R
{C_2370_GALAXY_10R,   3880,       29270,     1} ,//        984          0 2370  GALAXY 10R
{C_2370_GALAXY_10R,   3920,       26470,     1} ,//        985          0 2370  GALAXY 10R
{C_2370_GALAXY_10R,   3940,       19510,     1} ,//        986          0 2370  GALAXY 10R
{C_2370_GALAXY_10R,   3960,       19510,     1} ,//        987          0 2370  GALAXY 10R
{C_2370_GALAXY_10R,   3970,       11162,     1} ,//        988          0 2370  GALAXY 10R
{C_2370_GALAXY_10R,   4120,       29270,     1} ,//        989          0 2370  GALAXY 10R
{C_2370_GALAXY_10R,   4160,       29270,     1} ,//        990          0 2370  GALAXY 10R  ----1104

//C_2390_GALAXY_23
{C_2390_GALAXY_23,   3707,        5632,     0} ,//        991          0 2390  GALAXY 23  ----1105
{C_2390_GALAXY_23,   3714,        4340,     0} ,//        992          0 2390  GALAXY 23
{C_2390_GALAXY_23,   3719,        3000,     0} ,//        993          0 2390  GALAXY 23
{C_2390_GALAXY_23,   3724,        3000,     0} ,//        994          0 2390  GALAXY 23
{C_2390_GALAXY_23,   3751,        8385,     0} ,//        995          0 2390  GALAXY 23
{C_2390_GALAXY_23,   3765,       19510,     0} ,//        996          0 2390  GALAXY 23
{C_2390_GALAXY_23,   3778,        1860,     0} ,//        997          0 2390  GALAXY 23
{C_2390_GALAXY_23,   3800,       19510,     0} ,//        998          0 2390  GALAXY 23
{C_2390_GALAXY_23,   3970,       14027,     0} ,//        999          0 2390  GALAXY 23
{C_2390_GALAXY_23,   4000,       19510,     0} ,//       1000          0 2390  GALAXY 23
{C_2390_GALAXY_23,   4160,       19510,     0} ,//       1001          0 2390  GALAXY 23
{C_2390_GALAXY_23,   3727,        6065,     1} ,//       1002          0 2390  GALAXY 23
{C_2390_GALAXY_23,   3732,       13235,     1} ,//       1003          0 2390  GALAXY 23
{C_2390_GALAXY_23,   3740,       19510,     1} ,//       1004          0 2390  GALAXY 23
{C_2390_GALAXY_23,   3780,       22500,     1} ,//       1005          0 2390  GALAXY 23
{C_2390_GALAXY_23,   3815,       19510,     1} ,//       1006          0 2390  GALAXY 23
{C_2390_GALAXY_23,   3832,       19510,     1} ,//       1007          0 2390  GALAXY 23
{C_2390_GALAXY_23,   3900,       27684,     1} ,//       1008          0 2390  GALAXY 23
{C_2390_GALAXY_23,   3940,       29270,     1} ,//       1009          0 2390  GALAXY 23
{C_2390_GALAXY_23,   3980,       27690,     1} ,//       1010          0 2390  GALAXY 23
{C_2390_GALAXY_23,   4060,       20000,     1} ,//       1011          0 2390  GALAXY 23
{C_2390_GALAXY_23,   4100,       29270,     1} ,//       1012          0 2390  GALAXY 23
{C_2390_GALAXY_23,   4140,       25325,     1} ,//       1013          0 2390  GALAXY 23
{C_2390_GALAXY_23,   4172,       14600,     1} ,//       1014          0 2390  GALAXY 23
{C_2390_GALAXY_23,   4186,        5184,     1} ,//       1015          0 2390  GALAXY 23   ----1129

//KU_2630_GALAXY_25
{KU_2630_GALAXY_25,  11874,       22000,     0} ,//       1016          0 2630  GALAXY 25  ----1130
{KU_2630_GALAXY_25,  11935,       20000,     0} ,//       1017          0 2630  GALAXY 25
{KU_2630_GALAXY_25,  11966,       22000,     0} ,//       1018          0 2630  GALAXY 25
{KU_2630_GALAXY_25,  11998,       20000,     0} ,//       1019          0 2630  GALAXY 25
{KU_2630_GALAXY_25,  12090,       20000,     0} ,//       1020          0 2630  GALAXY 25
{KU_2630_GALAXY_25,  12121,       20000,     0} ,//       1021          0 2630  GALAXY 25
{KU_2630_GALAXY_25,  12153,       20000,     0} ,//       1022          0 2630  GALAXY 25
{KU_2630_GALAXY_25,  11716,       22000,     1} ,//       1023          0 2630  GALAXY 25
{KU_2630_GALAXY_25,  11789,       28125,     1} ,//       1024          0 2630  GALAXY 25
{KU_2630_GALAXY_25,  11836,       20765,     1} ,//       1025          0 2630  GALAXY 25
{KU_2630_GALAXY_25,  11867,       22000,     1} ,//       1026          0 2630  GALAXY 25
{KU_2630_GALAXY_25,  11898,       22000,     1} ,//       1027          0 2630  GALAXY 25
{KU_2630_GALAXY_25,  11991,       22000,     1} ,//       1028          0 2630  GALAXY 25
{KU_2630_GALAXY_25,  12008,        3000,     1} ,//       1029          0 2630  GALAXY 25
{KU_2630_GALAXY_25,  12053,       22000,     1} ,//       1030          0 2630  GALAXY 25
{KU_2630_GALAXY_25,  12084,       22000,     1} ,//       1031          0 2630  GALAXY 25
{KU_2630_GALAXY_25,  12115,       22425,     1} ,//       1032          0 2630  GALAXY 25
{KU_2630_GALAXY_25,  12146,       22000,     1} ,//       1033          0 2630  GALAXY 25
{KU_2630_GALAXY_25,  12177,       23000,     1} ,//       1034          0 2630  GALAXY 25   -----1148

//C_2650_GALAXY_3C
{C_2650_GALAXY_3C,   3842,       11000,     0} ,//       1035          0 2650  GALAXY 3C  ----1149
{C_2650_GALAXY_3C,   3914,        3074,     0} ,//       1036          0 2650  GALAXY 3C
{C_2650_GALAXY_3C,   3923,        3074,     0} ,//       1037          0 2650  GALAXY 3C
{C_2650_GALAXY_3C,   3927,        3074,     0} ,//       1038          0 2650  GALAXY 3C
{C_2650_GALAXY_3C,   3937,        3074,     0} ,//       1039          0 2650  GALAXY 3C
{C_2650_GALAXY_3C,   3945,        3074,     0} ,//       1040          0 2650  GALAXY 3C
{C_2650_GALAXY_3C,   3953,        3074,     0} ,//       1041          0 2650  GALAXY 3C
{C_2650_GALAXY_3C,   3958,        3074,     0} ,//       1042          0 2650  GALAXY 3C
{C_2650_GALAXY_3C,   3968,        3074,     0} ,//       1043          0 2650  GALAXY 3C
{C_2650_GALAXY_3C,   3972,        3074,     0} ,//       1044          0 2650  GALAXY 3C
{C_2650_GALAXY_3C,   3977,        3074,     0} ,//       1045          0 2650  GALAXY 3C
{C_2650_GALAXY_3C,   4040,       29110,     0} ,//       1046          0 2650  GALAXY 3C
{C_2650_GALAXY_3C,   3730,        8825,     1} ,//       1047          0 2650  GALAXY 3C
{C_2650_GALAXY_3C,   3745,        2206,     1} ,//       1048          0 2650  GALAXY 3C
{C_2650_GALAXY_3C,   3754,        3978,     1} ,//       1049          0 2650  GALAXY 3C
{C_2650_GALAXY_3C,   3767,        6111,     1} ,//       1050          0 2650  GALAXY 3C
{C_2650_GALAXY_3C,   3885,        3074,     1} ,//       1051          0 2650  GALAXY 3C
{C_2650_GALAXY_3C,   3890,        3074,     1} ,//       1052          0 2650  GALAXY 3C
{C_2650_GALAXY_3C,   3908,        3074,     1} ,//       1053          0 2650  GALAXY 3C
{C_2650_GALAXY_3C,   3917,        3074,     1} ,//       1054          0 2650  GALAXY 3C
{C_2650_GALAXY_3C,   3925,        3074,     1} ,//       1055          0 2650  GALAXY 3C
{C_2650_GALAXY_3C,   3929,        3074,     1} ,//       1056          0 2650  GALAXY 3C
{C_2650_GALAXY_3C,   3934,        3074,     1} ,//       1057          0 2650  GALAXY 3C
{C_2650_GALAXY_3C,   3939,        3074,     1} ,//       1058          0 2650  GALAXY 3C
{C_2650_GALAXY_3C,   3943,        3074,     1} ,//       1059          0 2650  GALAXY 3C
{C_2650_GALAXY_3C,   3948,        3074,     1} ,//       1060          0 2650  GALAXY 3C
{C_2650_GALAXY_3C,   3956,        3074,     1} ,//       1061          0 2650  GALAXY 3C
{C_2650_GALAXY_3C,   3979,        3074,     1} ,//       1062          0 2650  GALAXY 3C
{C_2650_GALAXY_3C,   3983,        3074,     1} ,//       1063          0 2650  GALAXY 3C
{C_2650_GALAXY_3C,   3996,        3074,     1} ,//       1064          0 2650  GALAXY 3C
{C_2650_GALAXY_3C,   4100,       28640,     1} ,//       1065          0 2650  GALAXY 3C
{C_2650_GALAXY_3C,   4168,        6617,     1} ,//       1066          0 2650  GALAXY 3C
{C_2650_GALAXY_3C,   4176,        6617,     1} ,//       1067          0 2650  GALAXY 3C   ----1181

//KU_2650_GALAXY_3C
{KU_2650_GALAXY_3C,  11780,       20759,     0} ,//       1068          0 2650  GALAXY 3C   ----1182
{KU_2650_GALAXY_3C,  11810,       20000,     0} ,//       1069          0 2650  GALAXY 3C
{KU_2650_GALAXY_3C,  11907,        4296,     0} ,//       1070          0 2650  GALAXY 3C
{KU_2650_GALAXY_3C,  12050,       20000,     0} ,//       1071          0 2650  GALAXY 3C   ----1185

//C_2690_GALAXY_11
{C_2690_GALAXY_11,   3720,       26700,     0} ,//       1072          0 2690  GALAXY 11   ----1186
{C_2690_GALAXY_11,   3780,       30000,     0} ,//       1073          0 2690  GALAXY 11
{C_2690_GALAXY_11,   3800,       30000,     0} ,//       1074          0 2690  GALAXY 11
{C_2690_GALAXY_11,   3840,       29250,     0} ,//       1075          0 2690  GALAXY 11
{C_2690_GALAXY_11,   3920,       26000,     0} ,//       1076          0 2690  GALAXY 11
{C_2690_GALAXY_11,   4007,        2480,     0} ,//       1077          0 2690  GALAXY 11
{C_2690_GALAXY_11,   4013,        6620,     0} ,//       1078          0 2690  GALAXY 11
{C_2690_GALAXY_11,   4014,        6620,     0} ,//       1079          0 2690  GALAXY 11
{C_2690_GALAXY_11,   4040,       27800,     0} ,//       1080          0 2690  GALAXY 11
{C_2690_GALAXY_11,   4082,        3300,     0} ,//       1081          0 2690  GALAXY 11
{C_2690_GALAXY_11,   4089,        3300,     0} ,//       1082          0 2690  GALAXY 11
{C_2690_GALAXY_11,   4092,        2170,     0} ,//       1083          0 2690  GALAXY 11
{C_2690_GALAXY_11,   4120,       19510,     0} ,//       1084          0 2690  GALAXY 11
{C_2690_GALAXY_11,   4133,        6111,     0} ,//       1085          0 2690  GALAXY 11
{C_2690_GALAXY_11,   4135,        6112,     0} ,//       1086          0 2690  GALAXY 11
{C_2690_GALAXY_11,   3740,       27800,     1} ,//       1087          0 2690  GALAXY 11
{C_2690_GALAXY_11,   3780,       30000,     1} ,//       1088          0 2690  GALAXY 11
{C_2690_GALAXY_11,   3820,       30000,     1} ,//       1089          0 2690  GALAXY 11
{C_2690_GALAXY_11,   3860,       30000,     1} ,//       1090          0 2690  GALAXY 11
{C_2690_GALAXY_11,   4140,       27799,     1} ,//       1091          0 2690  GALAXY 11 ----1205

//KU_2690_GALAXY_11
{KU_2690_GALAXY_11,  11865,        2500,     1} ,//       1092          0 2690  GALAXY 11 ----1206
{KU_2690_GALAXY_11,  11872,        2500,     1} ,//       1093          0 2690  GALAXY 11
{KU_2690_GALAXY_11,  11924,        2500,     1} ,//       1094          0 2690  GALAXY 11
{KU_2690_GALAXY_11,  12060,       26700,     1} ,//       1095          0 2690  GALAXY 11
{KU_2690_GALAXY_11,  12135,       15000,     1} ,//       1096          0 2690  GALAXY 11
{KU_2690_GALAXY_11,  12152,        4883,     1} ,//       1097          0 2690  GALAXY 11  ----1211

//KU_3020_INTELSAT_9
{KU_3020_INTELSAT_9,  11477,       26470,     0} ,//       1098          0 3020  INTELSAT 9  ---1212
{KU_3020_INTELSAT_9,  11531,        6620,     0} ,//       1099          0 3020  INTELSAT 9
{KU_3020_INTELSAT_9,  11612,        3670,     0} ,//       1100          0 3020  INTELSAT 9 ----1215

//C_3020_INTELSAT_9
{C_3020_INTELSAT_9,   3720,       19510,     0} ,//       1101          0 3020  INTELSAT 9 ----1216
{C_3020_INTELSAT_9,   3760,       28500,     0} ,//       1102          0 3020  INTELSAT 9
{C_3020_INTELSAT_9,   3786,        4443,     0} ,//       1103          0 3020  INTELSAT 9
{C_3020_INTELSAT_9,   3800,       27680,     0} ,//       1104          0 3020  INTELSAT 9
{C_3020_INTELSAT_9,   3808,        3428,     0} ,//       1105          0 3020  INTELSAT 9
{C_3020_INTELSAT_9,   3815,        3950,     0} ,//       1106          0 3020  INTELSAT 9
{C_3020_INTELSAT_9,   3840,       27684,     0} ,//       1107          0 3020  INTELSAT 9
{C_3020_INTELSAT_9,   3880,       27684,     0} ,//       1108          0 3020  INTELSAT 9
{C_3020_INTELSAT_9,   4000,       27690,     0} ,//       1109          0 3020  INTELSAT 9
{C_3020_INTELSAT_9,   4080,       27684,     0} ,//       1110          0 3020  INTELSAT 9
{C_3020_INTELSAT_9,   4107,        6620,     0} ,//       1111          0 3020  INTELSAT 9
{C_3020_INTELSAT_9,   4123,        2222,     0} ,//       1112          0 3020  INTELSAT 9
{C_3020_INTELSAT_9,   4127,        3273,     0} ,//       1113          0 3020  INTELSAT 9
{C_3020_INTELSAT_9,   4137,        2941,     0} ,//       1114          0 3020  INTELSAT 9
{C_3020_INTELSAT_9,   4157,        6500,     0} ,//       1115          0 3020  INTELSAT 9
{C_3020_INTELSAT_9,   3720,       27700,     1} ,//       1116          0 3020  INTELSAT 9
{C_3020_INTELSAT_9,   3760,       27684,     1} ,//       1117          0 3020  INTELSAT 9
{C_3020_INTELSAT_9,   3800,       26470,     1} ,//       1118          0 3020  INTELSAT 9
{C_3020_INTELSAT_9,   3840,       27690,     1} ,//       1119          0 3020  INTELSAT 9
{C_3020_INTELSAT_9,   3880,       27684,     1} ,//       1120          0 3020  INTELSAT 9
{C_3020_INTELSAT_9,   3911,       13330,     1} ,//       1121          0 3020  INTELSAT 9
{C_3020_INTELSAT_9,   3924,        6620,     1} ,//       1122          0 3020  INTELSAT 9
{C_3020_INTELSAT_9,   3933,        7000,     1} ,//       1123          0 3020  INTELSAT 9
{C_3020_INTELSAT_9,   4080,       27690,     1} ,//       1124          0 3020  INTELSAT 9
{C_3020_INTELSAT_9,   4120,       27500,     1} ,//       1125          0 3020  INTELSAT 9
{C_3020_INTELSAT_9,   4143,        2205,     1} ,//       1126          0 3020  INTELSAT 9
{C_3020_INTELSAT_9,   4147,        2941,     1} ,//       1127          0 3020  INTELSAT 9
{C_3020_INTELSAT_9,   4160,        3000,     1} ,//       1128          0 3020  INTELSAT 9  -----1242

//KU_3070_INTELSAT_707
{KU_3070_INTELSAT_707,  11875,        2344,     1} ,//       1129          0 3070  INTELSAT 707 ----1243
{KU_3070_INTELSAT_707,  11891,       19510,     1} ,//       1130          0 3070  INTELSAT 707  ----1244

//KU_3300_HISPASAT_1C_1D
{KU_3300_HISPASAT_1C_1D,  11615,       27500,     0} ,//       1131          0 3300  HISPASAT 1C, 1D  ----1245
{KU_3300_HISPASAT_1C_1D,  11676,       16190,     0} ,//       1132          0 3300  HISPASAT 1C, 1D
{KU_3300_HISPASAT_1C_1D,  11731,       28126,     0} ,//       1133          0 3300  HISPASAT 1C, 1D
{KU_3300_HISPASAT_1C_1D,  11771,       27500,     0} ,//       1134          0 3300  HISPASAT 1C, 1D
{KU_3300_HISPASAT_1C_1D,  11811,       27500,     0} ,//       1135          0 3300  HISPASAT 1C, 1D
{KU_3300_HISPASAT_1C_1D,  11851,       27500,     0} ,//       1136          0 3300  HISPASAT 1C, 1D
{KU_3300_HISPASAT_1C_1D,  11891,       27500,     0} ,//       1137          0 3300  HISPASAT 1C, 1D
{KU_3300_HISPASAT_1C_1D,  11931,       27500,     0} ,//       1138          0 3300  HISPASAT 1C, 1D
{KU_3300_HISPASAT_1C_1D,  12012,       30000,     0} ,//       1139          0 3300  HISPASAT 1C, 1D
{KU_3300_HISPASAT_1C_1D,  12052,       30000,     0} ,//       1140          0 3300  HISPASAT 1C, 1D
{KU_3300_HISPASAT_1C_1D,  12092,       30000,     0} ,//       1141          0 3300  HISPASAT 1C, 1D
{KU_3300_HISPASAT_1C_1D,  11535,       24500,     1} ,//       1142          0 3300  HISPASAT 1C, 1D
{KU_3300_HISPASAT_1C_1D,  11577,       27500,     1} ,//       1143          0 3300  HISPASAT 1C, 1D
{KU_3300_HISPASAT_1C_1D,  11617,       27500,     1} ,//       1144          0 3300  HISPASAT 1C, 1D
{KU_3300_HISPASAT_1C_1D,  11682,        3800,     1} ,//       1145          0 3300  HISPASAT 1C, 1D
{KU_3300_HISPASAT_1C_1D,  11731,       27500,     1} ,//       1146          0 3300  HISPASAT 1C, 1D
{KU_3300_HISPASAT_1C_1D,  11771,       27500,     1} ,//       1147          0 3300  HISPASAT 1C, 1D
{KU_3300_HISPASAT_1C_1D,  11811,       27500,     1} ,//       1148          0 3300  HISPASAT 1C, 1D
{KU_3300_HISPASAT_1C_1D,  11851,       27500,     1} ,//       1149          0 3300  HISPASAT 1C, 1D
{KU_3300_HISPASAT_1C_1D,  11891,       30000,     1} ,//       1150          0 3300  HISPASAT 1C, 1D
{KU_3300_HISPASAT_1C_1D,  11931,       27500,     1} ,//       1151          0 3300  HISPASAT 1C, 1D
{KU_3300_HISPASAT_1C_1D,  11972,       27500,     1} ,//       1152          0 3300  HISPASAT 1C, 1D
{KU_3300_HISPASAT_1C_1D,  12012,       27500,     1} ,//       1153          0 3300  HISPASAT 1C, 1D
{KU_3300_HISPASAT_1C_1D,  12092,       27500,     1} ,//       1154          0 3300  HISPASAT 1C, 1D
{KU_3300_HISPASAT_1C_1D,  12149,       27500,     1} ,//       1155          0 3300  HISPASAT 1C, 1D
{KU_3300_HISPASAT_1C_1D,  12226,       27500,     1} ,//       1156          0 3300  HISPASAT 1C, 1D
{KU_3300_HISPASAT_1C_1D,  12303,       27500,     1} ,//       1157          0 3300  HISPASAT 1C, 1D
{KU_3300_HISPASAT_1C_1D,  12379,       27500,     1} ,//       1158          0 3300  HISPASAT 1C, 1D
{KU_3300_HISPASAT_1C_1D,  12456,       30000,     1} ,//       1159          0 3300  HISPASAT 1C, 1D
{KU_3300_HISPASAT_1C_1D,  12522,        7400,     1} ,//       1160          0 3300  HISPASAT 1C, 1D
{KU_3300_HISPASAT_1C_1D,  12547,       30000,     1} ,//       1161          0 3300  HISPASAT 1C, 1D -----1275

//KU_3325_INTELSAT_907
{KU_3325_INTELSAT_907,  11516,        5632,     1} ,//       1162          0 3325  INTELSAT 907  ---1276
{KU_3325_INTELSAT_907,  11665,       13020,     1} ,//       1163          0 3325  INTELSAT 907  ---1277

//KU_3380_NSS_7
{KU_3380_NSS_7,  11488,        2170,     0} ,//       1164          0 3380  NSS 7   -----1278
{KU_3380_NSS_7,  11563,        6111,     0} ,//       1165          0 3380  NSS 7
{KU_3380_NSS_7,  11605,        4000,     0} ,//       1166          0 3380  NSS 7
{KU_3380_NSS_7,  11620,        2000,     0} ,//       1167          0 3380  NSS 7
{KU_3380_NSS_7,  12735,       16640,     0} ,//       1168          0 3380  NSS 7 -----1282

//KU_3450_TELSTAR_12
{KU_3450_TELSTAR_12,  11062,       19280,     0} ,//       1169          0 3450  TELSTAR 12  ----1283
{KU_3450_TELSTAR_12,  11124,       18386,     0} ,//       1170          0 3450  TELSTAR 12
{KU_3450_TELSTAR_12,  11150,       18386,     0} ,//       1171          0 3450  TELSTAR 12
{KU_3450_TELSTAR_12,  12545,       11900,     0} ,//       1172          0 3450  TELSTAR 12
{KU_3450_TELSTAR_12,  12608,       19279,     0} ,//       1173          0 3450  TELSTAR 12
{KU_3450_TELSTAR_12,  12645,        3255,     0} ,//       1174          0 3450  TELSTAR 12
{KU_3450_TELSTAR_12,  11150,       19275,     1} ,//       1175          0 3450  TELSTAR 12
{KU_3450_TELSTAR_12,  11678,        2200,     1} ,//       1176          0 3450  TELSTAR 12
{KU_3450_TELSTAR_12,  12524,       26470,     1} ,//       1177          0 3450  TELSTAR 12
{KU_3450_TELSTAR_12,  12589,        4503,     1} ,//       1178          0 3450  TELSTAR 12
{KU_3450_TELSTAR_12,  12615,       11938,     1} ,//       1179          0 3450  TELSTAR 12 -----1293

//KU_3475_ATLANTIC_BIRD_1
{KU_3475_ATLANTIC_BIRD_1,  11100,        2000,     0} ,//       1180          0 3475  ATLANTIC BIRD 1  ----1294
{KU_3475_ATLANTIC_BIRD_1,  11132,        3254,     0} ,//       1181          0 3475  ATLANTIC BIRD 1
{KU_3475_ATLANTIC_BIRD_1,  11177,        7596,     0} ,//       1182          0 3475  ATLANTIC BIRD 1
{KU_3475_ATLANTIC_BIRD_1,  11331,        6000,     0} ,//       1183          0 3475  ATLANTIC BIRD 1
{KU_3475_ATLANTIC_BIRD_1,  11337,        1923,     0} ,//       1184          0 3475  ATLANTIC BIRD 1
{KU_3475_ATLANTIC_BIRD_1,  11340,        2279,     0} ,//       1185          0 3475  ATLANTIC BIRD 1
{KU_3475_ATLANTIC_BIRD_1,  11355,       11781,     0} ,//       1186          0 3475  ATLANTIC BIRD 1
{KU_3475_ATLANTIC_BIRD_1,  11387,       27500,     0} ,//       1187          0 3475  ATLANTIC BIRD 1
{KU_3475_ATLANTIC_BIRD_1,  12515,       17452,     0} ,//       1188          0 3475  ATLANTIC BIRD 1
{KU_3475_ATLANTIC_BIRD_1,  12546,       17454,     0} ,//       1189          0 3475  ATLANTIC BIRD 1
{KU_3475_ATLANTIC_BIRD_1,  12597,        1910,     0} ,//       1190          0 3475  ATLANTIC BIRD 1
{KU_3475_ATLANTIC_BIRD_1,  11185,       13657,     1} ,//       1191          0 3475  ATLANTIC BIRD 1
{KU_3475_ATLANTIC_BIRD_1,  12529,        1550,     1} ,//       1192          0 3475  ATLANTIC BIRD 1
{KU_3475_ATLANTIC_BIRD_1,  12578,        5860,     1} ,//       1193          0 3475  ATLANTIC BIRD 1
{KU_3475_ATLANTIC_BIRD_1,  12632,        2141,     1} ,//       1194          0 3475  ATLANTIC BIRD 1
{KU_3475_ATLANTIC_BIRD_1,  12637,        4285,     1} ,//       1195          0 3475  ATLANTIC BIRD 1  -----1309

//KU_3490_EXPRESS_A3
{KU_3490_EXPRESS_A3,  11516,        7780,     1} ,//       1196          0 3490  EXPRESS A3   ----1310

//KU_3520_ATLANTIC_BIRD_2
{KU_3520_ATLANTIC_BIRD_2,  10970,       27500,     0} ,//       1197          0 3520  ATLANTIC BIRD 2  ----1311
{KU_3520_ATLANTIC_BIRD_2,  11010,       27500,     0} ,//       1198          0 3520  ATLANTIC BIRD 2
{KU_3520_ATLANTIC_BIRD_2,  11387,       24740,     0} ,//       1199          0 3520  ATLANTIC BIRD 2
{KU_3520_ATLANTIC_BIRD_2,  11427,       27500,     0} ,//       1200          0 3520  ATLANTIC BIRD 2
{KU_3520_ATLANTIC_BIRD_2,  12539,        3038,     0} ,//       1201          0 3520  ATLANTIC BIRD 2
{KU_3520_ATLANTIC_BIRD_2,  10972,       27500,     1} ,//       1202          0 3520  ATLANTIC BIRD 2
{KU_3520_ATLANTIC_BIRD_2,  11011,       27500,     1} ,//       1203          0 3520  ATLANTIC BIRD 2
{KU_3520_ATLANTIC_BIRD_2,  12566,       27500,     1} ,//       1204          0 3520  ATLANTIC BIRD 2
{KU_3520_ATLANTIC_BIRD_2,  12649,       27500,     1} ,//       1205          0 3520  ATLANTIC BIRD 2 -----1319

//KU_3560_AMOS_2_3
{KU_3560_AMOS_2_3,  10722,       27500,     0} ,//       1241          0 3560  AMOS 2,3  ----1320
{KU_3560_AMOS_2_3,  10759,       30000,     0} ,//       1242          0 3560  AMOS 2,3
{KU_3560_AMOS_2_3,  10806,       27500,     0} ,//       1243          0 3560  AMOS 2,3
{KU_3560_AMOS_2_3,  10842,       27500,     0} ,//       1244          0 3560  AMOS 2,3
{KU_3560_AMOS_2_3,  10890,       27500,     0} ,//       1245          0 3560  AMOS 2,3
{KU_3560_AMOS_2_3,  10925,       27500,     0} ,//       1246          0 3560  AMOS 2,3
{KU_3560_AMOS_2_3,  11122,        1852,     0} ,//       1247          0 3560  AMOS 2,3
{KU_3560_AMOS_2_3,  11177,        4165,     0} ,//       1248          0 3560  AMOS 2,3
{KU_3560_AMOS_2_3,  11179,        6666,     0} ,//       1249          0 3560  AMOS 2,3
{KU_3560_AMOS_2_3,  11260,       27500,     0} ,//       1250          0 3560  AMOS 2,3
{KU_3560_AMOS_2_3,  11303,       19540,     0} ,//       1251          0 3560  AMOS 2,3
{KU_3560_AMOS_2_3,  11319,        2750,     0} ,//       1252          0 3560  AMOS 2,3
{KU_3560_AMOS_2_3,  11324,        3350,     0} ,//       1253          0 3560  AMOS 2,3
{KU_3560_AMOS_2_3,  11333,        3500,     0} ,//       1254          0 3560  AMOS 2,3
{KU_3560_AMOS_2_3,  11338,        2600,     0} ,//       1255          0 3560  AMOS 2,3
{KU_3560_AMOS_2_3,  11347,        2800,     0} ,//       1256          0 3560  AMOS 2,3
{KU_3560_AMOS_2_3,  11351,        3350,     0} ,//       1257          0 3560  AMOS 2,3
{KU_3560_AMOS_2_3,  11359,        3350,     0} ,//       1258          0 3560  AMOS 2,3
{KU_3560_AMOS_2_3,  11412,        7926,     0} ,//       1259          0 3560  AMOS 2,3
{KU_3560_AMOS_2_3,  11423,        2892,     0} ,//       1260          0 3560  AMOS 2,3
{KU_3560_AMOS_2_3,  11427,        5926,     0} ,//       1261          0 3560  AMOS 2,3
{KU_3560_AMOS_2_3,  11435,        2089,     0} ,//       1262          0 3560  AMOS 2,3
{KU_3560_AMOS_2_3,  11443,        4165,     0} ,//       1263          0 3560  AMOS 2,3
{KU_3560_AMOS_2_3,  11560,       13400,     0} ,//       1264          0 3560  AMOS 2,3
{KU_3560_AMOS_2_3,  11575,        8887,     0} ,//       1265          0 3560  AMOS 2,3
{KU_3560_AMOS_2_3,  11586,        7958,     0} ,//       1266          0 3560  AMOS 2,3
{KU_3560_AMOS_2_3,  11650,       21350,     0} ,//       1267          0 3560  AMOS 2,3
{KU_3560_AMOS_2_3,  11690,        7200,     0} ,//       1268          0 3560  AMOS 2,3
{KU_3560_AMOS_2_3,  11591,       20000,     1} ,//       1269          0 3560  AMOS 2,3 ----1348

//KU_703_NSS_703
{KU_703_NSS_703,  11138,       3617,     0} ,//       1270          0 703  NSS 703   -----1349
{KU_703_NSS_703,  11534,       5064,     0} ,//       1271          0 703  NSS 703
{KU_703_NSS_703,  11541,       5064,     0} ,//       1272          0 703  NSS 703
{KU_703_NSS_703,  12551,       9404,     0} ,//       1273          0 703  NSS 703  -----1352

//KU_1380_TELSTAR_18
{KU_1380_TELSTAR_18,  12316,       1499,   0},   //       TELSTAR 18 ----1353
{KU_1380_TELSTAR_18,  12321,       4999,   0},   //       TELSTAR 18
{KU_1380_TELSTAR_18,  12333,       7664,   0},   //       TELSTAR 18
{KU_1380_TELSTAR_18,  12368,       1341,   0},   //       TELSTAR 18
{KU_1380_TELSTAR_18,  12661,       4284,   0},  //       TELSTAR 18
{KU_1380_TELSTAR_18,  12302,       30000,   1},//       TELSTAR 18
{KU_1380_TELSTAR_18,  12354,       43000,   1},//       TELSTAR 18
{KU_1380_TELSTAR_18,  12401,       22425,   1},//       TELSTAR 18
{KU_1380_TELSTAR_18,  12430 ,       22425,    1},
{KU_1380_TELSTAR_18,  12463 ,       21600,    1},
{KU_1380_TELSTAR_18,  12538,       41250,   1},//       TELSTAR 18
{KU_1380_TELSTAR_18,  12598,       43000,   1},//       TELSTAR 18
{KU_1380_TELSTAR_18,  12646,       22425,   1},//       TELSTAR 18
{KU_1380_TELSTAR_18,  12667,       4284,   1}, //       TELSTAR 18
{KU_1380_TELSTAR_18,  12684,       3000,   1}, //       TELSTAR 18
{KU_1380_TELSTAR_18,  12720,       43000,   1},//       TELSTAR 18  -----1368
};

#endif //#if !defined(__API_DTVSATCONSTINFO_H__)
#endif//#if (MS_DVB_TYPE_SEL == DVBS)
