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
#ifndef _DEMO_CH_SCAN_H_
#define _DEMO_CH_SCAN_H_

#if(DEMO_CH_SCAN_TEST == 1)

//==========Modified from MApp_GlobalSettingSt.h start==========
typedef enum
{
    SCAN_TYPE_AUTO,
    SCAN_TYPE_MANUAL,
    SCAN_TYPE_BLIND,
    SCAN_TYPE_NIT,
    SCAN_TYPE_NUM
} EN_SCAN_TYPE;
//==========Modified from MApp_GlobalSettingSt.h end==========

//==========Modified from apiRfChannel.h start==========
#define FREQ_TAL_SIZE   7
typedef struct  _FreqTblType
{
       MS_U8 u8CHNo;   //channel no.
       MS_U32 u32Freq;   //frequency
       MS_S8 u8CHNoOffset;  //No. offset ----->for displaying 0:= No.  +X:No.-X and add "A"   -X:No.-X
}FreqTblType;

typedef enum _EN_COUNTRY_TYPE
{
    COUNTRY_TYPE_EUROPE,
    COUNTRY_TYPE_GERMANY,
    COUNTRY_TYPE_TAIWAN,
    COUNTRY_TYPE_ITALY,
    COUNTRY_TYPE_FRANCE,
    COUNTRY_TYPE_CHINA,
    COUNTRY_TYPE_AUSTRALIA,
    COUNTRY_TYPE_BRAZIL,
    COUNTRY_TYPE_CHILE,
    COUNTRY_TYPE_HONGKONG,
    COUNTRY_TYPE_ARGENTINA,
    COUNTRY_TYPE_PERU,
    COUNTRY_TYPE_UK,
    COUNTRY_TYPE_RUSSIAN,
    COUNTRY_TYPE_INDIA,
    COUNTRY_TYPE_CUBA,
    COUNTRY_TYPE_OTHER,
    COUNTRY_TYPE_NUM,

} EN_COUNTRY_TYPE;
//==========Modified from apiRfChannel.h end==========

//==========Modified from apiChScan.h start==========
typedef enum
{
     BLINDSCAN_READY,
     BLINDSCAN_H_LoLOF,
     BLINDSCAN_V_LoLOF,
     BLINDSCAN_H_HiLOF,
     BLINDSCAN_V_HiLOF,
     BLINDSCAN_GETTP_FINISH,

     BLINDSCAN_STATUS_NUM
}BLINDSCAN_STATUS;

/// Channel scan mode
typedef enum _ChScan_Mode
{
    E_CHSCAN_MODE_FREQ_TABLE,                                           ///< Scanning by discrete frequency table
    E_CHSCAN_MODE_CHANNEL_NO,                                           ///< Scanning by increment channel number
    E_CHSCAN_MODE_TPSIDX_TABLE,                                           ///< Scanning by increment channel number
} __attribute__((packed)) ChScan_Mode;

/// Channel scan reporting events definition
typedef enum _ChScan_Event
{
    //E_CHSCAN_EVENT_START = 0x0001
    E_CHSCAN_EVENT_SCAN  = 0x0002,                                      ///< Scan start
    E_CHSCAN_EVENT_FOUND = 0x0004,                                      ///< Channel found and pause
    E_CHSCAN_EVENT_FAIL  = 0x0008,                                      ///< Scan fail and stop
    E_CHSCAN_EVENT_END   = 0x0010,                                      ///< Scan end
} __attribute__((packed)) ChScan_Event;

typedef enum _TV_SystemType
{
    E_TV_DVBS,
    E_TV_DVBC,
    E_TV_DVBT,
} __attribute__((packed)) TV_SystemType;

/// DVB scan setting data structure
typedef struct
{
    TV_SystemType                   eTvSysType;                         ///<  Specific TV system
    ChScan_Mode                     bScanMode;                          ///<  Scan mode by frequency table or apiRfChannel
    MS_U8                   enScanType;                        //
    MS_U8               *pu8SatIDList;
    EN_COUNTRY_TYPE                 eCountry;                           ///<  Country code @ref apiRfChannel.h
    EN_FE_TUNE_MODE                 eAutoTune;                          ///<  TRUE: Auto tune, FALSE: Manual tune
    MS_BOOL                         bIsNitSearch;
    MS_U32*                     pu32FreqTable;                      ///<  Frequency table start address. (@ref E_CHSCAN_MODE_FREQ_TABLE)
    MS_U32                      u32StartFreq;                       ///<  Start frequency to scan. Unit is in KHZ. Valid in continuous mode.
    MS_FE_CARRIER_PARAM*        pstTPIdxTable;                  ///
    MS_U32                      u32FreqCount;                       ///<  Discrete frequency count. (@ref E_CHSCAN_MODE_FREQ_TABLE)
    MS_U32                      u32LastFreq;                        ///<  Last frequency to scan. Unit is KHZ. Valid in continuous mode.
    MS_U32                      u32TPSCount;
    MS_FE_CARRIER_PARAM             stTPSInfo;                            ///<  Modulation parameter.
} ChScan_Param;
//==========Modified from apiChScan.h end==========
typedef enum
{
    E_TASK_STATE_BLIND_START,
    E_TASK_STATE_BLIND_TURNMOTOR,
    E_TASK_STATE_BLIND_NEXTFREQ,
    E_TASK_STATE_BLIND_WAITFINISH,
    E_TASK_STATE_BLIND_GETTPLIST,
    E_TASK_STATE_BLIND_END,
    E_TASK_STATE_START,
    E_TASK_STATE_TURNMOTOR,
    E_TASK_STATE_TUNE2RF,
    E_TASK_STATE_WAITLOCK,
    E_TASK_STATE_SETTPS,
    E_TASK_STATE_IDLE,
} TaskState;

typedef enum
{
    E_SCAN_TYPE_SCAN_TP_ALL_THEN_PG_ALL,
    E_SCAN_TYPE_SCAN_EACH_TP_AND_ITS_PG
} BLIND_SCAN_TYPE;

typedef TaskState (*TaskStateFunc)(void);

MS_BOOL Demo_ChScan_Auto(MS_U32* puCountry, MS_U32* pu32Arg1, MS_U32* pu32Arg2);
#endif
#endif
