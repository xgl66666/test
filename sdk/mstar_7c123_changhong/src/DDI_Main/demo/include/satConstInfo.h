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
    MS_U16 u16Band:2;//0:C  1:Ku 2:ATV c 3:ATV ku
    MS_U16 u16Angle:14;
    MS_U8 *pSatName;
}Antenna_SatInfo;

#ifdef NON_EXTERN_SAT_INFO
Antenna_SatInfo const AntennaSatList_BuiltIn[] =
{
	{KU_130_HOTBIRD_6_7A_8,            BAND_KU,  130,(MS_U8*)"HOTBIRD 6,7A,8"} ,//		3	107
	{KU_192_ASTRA_1H_1KR_1L_1M,    BAND_KU,  192,(MS_U8*)"ASTRA 1H,1KR,1L,1M"} ,//		5	85
	{KU_282_ASTRA_2A_2B_2D,           BAND_KU,  282,(MS_U8*)"ASTRA 2A,2B,2D"} ,//		7	90
	{KU_235_ASTRA_1E_1G_3A,            BAND_KU,  235,(MS_U8*)"ASTRA 1E,1G,3A"} ,//		7	90
	{KU_3530_NILESAT_101_10,           BAND_KU,  3530,(MS_U8*)"NILESAT 101, 10"} ,//	46	35
	{KU_70_EUTELSAT_W3A,                BAND_KU,  70,(MS_U8*)"EUTELSAT W3A"} ,//		1	18
	{KU_100_EUTELSAT_W1,                BAND_KU,  100,(MS_U8*)"EUTELSAT W1"} ,//		2	6
	{KU_260_BADR_2_3_4_EURO,        BAND_KU,  260,(MS_U8*)"BADR-2,3,4/EURO"} ,//		6	30
	{KU_360_EUTELSAT_SESAT,           BAND_KU,  360,(MS_U8*)"EUTELSAT SESAT"} ,//		9	9
	{KU_160_EUTELSAT_W2,                BAND_KU,  160,(MS_U8*)"EUTELSAT W2"} ,//		4	47
	{KU_400_EXPRESS_AM1,                BAND_KU,  400,(MS_U8*)"EXPRESS AM1"} ,//	11	5
	{KU_420_TURKSAT_2A_3A,             BAND_KU,  420,(MS_U8*)"TURKSAT 2A, 3A"} ,//	12	170
	{KU_620_INTELSAT_920,                BAND_KU,  620,(MS_U8*)"INTELSAT 902"} ,//	13	18
	{C_686_INTELSAT_7_10,                BAND_C,  686,(MS_U8*)"INTELSAT 7, 10"} ,//	14	53

	{KU_305_ARABSAT_2B,                  BAND_KU,  305,(MS_U8*)"ARABSAT 2B"} ,//		8	4
	{KU_390_HELLAS_SAT_2,                 BAND_KU,  390,(MS_U8*)"HELLAS SAT 2"} ,//	10	21

	{KU_765_TELSTAR_10,                   BAND_KU,  765,(MS_U8*)"TELSTAR 10"} ,//	15	32
	{C_765_TELSTAR_10,                     BAND_C,  765,(MS_U8*)"TELSTAR 10"} ,//	15	32
	{KU_785_THAICOM_2_5,                 BAND_KU,  785,(MS_U8*)"THAICOM 2,5"} ,//	16	45
	{C_785_THAICOM_2_5,                  BAND_C,  785,(MS_U8*)"THAICOM 2,5"} ,//	16	45
	{KU_830_INSAT_3B_4A,                 BAND_KU,   830,(MS_U8*)"INSAT 3B,4A"} ,//	17	12
	{KU_900_YAMAL_210,                           BAND_KU,   900,(MS_U8*)"YAMAL 201"} ,//	18	3
	{KU_950_NSS_6,                                  BAND_KU,   950,(MS_U8*)"NSS 6"} ,//	19	23
	{C_1005_ASIASAT_2,                             BAND_C,  1005,(MS_U8*)"ASIASAT 2"} ,//	20	37
	{KU_1005_ASIASAT_2,                           BAND_KU,  1005,(MS_U8*)"ASIASAT 2"} ,//	20	37
	{C_1055_ASIASAT_3S,                           BAND_C,  1055,(MS_U8*)"ASIASAT 3S"} ,//	21	50
	{KU_1055_ASIASAT_3S,                         BAND_KU,  1055,(MS_U8*)"ASIASAT 3S"} ,//	21	50
	{C_1155_CHINASAT_6B,                        BAND_C,  1155,(MS_U8*)"CHINASAT 6B"} ,//	22	44
	{C_1250_SINOSAT_3,                             BAND_C,  1250,(MS_U8*)"SINOSAT-3"} ,//	23	14
	{C_1340_APSTAR_6,                              BAND_C,  1340,(MS_U8*)"APSTAR 6"} ,//	24	11
	{KU_1340_APSTAR_6,                            BAND_KU,  1340,(MS_U8*)"APSTAR 6"} ,//	24	11
	{C_1660_INTELSAT_8,                           BAND_C,  1660,(MS_U8*)"INTELSAT 8"} ,//	25	19
	{KU_1660_INTELSAT_8,                         BAND_KU,  1660,(MS_U8*)"INTELSAT 8"} ,//	25	19
	{C_1690_INTELSAT_2,                           BAND_C,  1690,(MS_U8*)"INTELSAT 2"} ,//	26	10
	{KU_1690_INTELSAT_2,                         BAND_KU,  1690,(MS_U8*)"INTELSAT 2"} ,//	26	10
	{C_2330_HORIZONS_1,                         BAND_C, 2330,(MS_U8*)"HORIZONS 1"} ,//	28	2
	{KU_2330_HORIZONS_1,                       BAND_KU,2330,(MS_U8*)"HORIZONS 1"} ,//	28	2
	{KU_2370_GALAXY_10R,                        BAND_KU, 2370,(MS_U8*)"GALAXY 10R"} ,//	29	26
	{C_2370_GALAXY_10R,                          BAND_C, 2370,(MS_U8*)"GALAXY 10R"} ,//	29	26
	{C_2390_GALAXY_23,                            BAND_C, 2390,(MS_U8*)"GALAXY 23"} ,//	30	25
	{KU_2630_GALAXY_25,                          BAND_KU,2630,(MS_U8*)"GALAXY 25"} ,//	32	19
	{C_2650_GALAXY_3C,                           BAND_C, 2650,(MS_U8*)"GALAXY 3C"} ,//	33	37
	{KU_2650_GALAXY_3C,                         BAND_KU,2650,(MS_U8*)"GALAXY 3C"} ,//	33	37
	{C_2690_GALAXY_11,                            BAND_C, 2690,(MS_U8*)"GALAXY 11"} ,//	34	26
	{KU_2690_GALAXY_11,                          BAND_KU,2690,(MS_U8*)"GALAXY 11"} ,//	34	26
	{KU_3020_INTELSAT_9,                         BAND_KU,3020,(MS_U8*)"INTELSAT 9"} ,//	37	31
	{C_3020_INTELSAT_9,                           BAND_C, 3020,(MS_U8*)"INTELSAT 9"} ,//	37	31
	{KU_3070_INTELSAT_707,                      BAND_KU, 3070,(MS_U8*)"INTELSAT 707"} ,//	38	2
	{KU_3300_HISPASAT_1C_1D,                BAND_KU, 3300,(MS_U8*)"HISPASAT 1C, 1D"} ,//	39	31
	{KU_3325_INTELSAT_907,                     BAND_KU, 3325,(MS_U8*)"INTELSAT 907"} ,//	40	2
	{KU_3380_NSS_7,                                BAND_KU, 3380,(MS_U8*)"NSS 7"} ,//	41	5
	{KU_3450_TELSTAR_12,                        BAND_KU, 3450,(MS_U8*)"TELSTAR 12"} ,//	42	11
	{KU_3475_ATLANTIC_BIRD_1,              BAND_KU, 3475,(MS_U8*)"ATLANTIC BIRD 1"} ,//	43	16
	{KU_3490_EXPRESS_A3,                       BAND_KU, 3490,(MS_U8*)"EXPRESS A3"} ,//	44	1
	{KU_3520_ATLANTIC_BIRD_2,              BAND_KU, 3520,(MS_U8*)"ATLANTIC BIRD 2"} ,//	45	9
	{KU_3560_AMOS_2_3,                          BAND_KU, 3560,(MS_U8*)"AMOS 2,3"} ,//	47	29
	{KU_703_NSS_703,                              BAND_KU,  703,(MS_U8*)"NSS 703"} ,//	19	23
	{KU_1380_TELSTAR_18,                       BAND_KU,  1380,(MS_U8*)"TELSTAR 18"},//
};
#else
extern Antenna_SatInfo const AntennaSatList_BuiltIn[];
#endif

#endif //#if !defined(__API_DTVSATCONSTINFO_H__)
#endif//#if (MS_DVB_TYPE_SEL == DVBS)
