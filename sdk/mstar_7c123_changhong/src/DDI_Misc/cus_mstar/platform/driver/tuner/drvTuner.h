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

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   drvTuner.h
/// @brief  Tuner Common Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DRV_TUNER_H_
#define _DRV_TUNER_H_

#include "MsCommon.h"
#include "drvDemod.h"
#include "drvIIC.h"
#include "drvDish.h"
//-------------------------------------------------------------------------------------------------
//  Driver Capability
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
//============ debug setting ========================
#define TUNER_EN_ERR 0x01
#define TUNER_EN_DBG 0x02
#define TUNER_EN_API 0x04
#define TUNER_DEBUG_OPTIONS  0


#if (TUNER_DEBUG_OPTIONS & TUNER_EN_ERR)
#define TUNER_ERR(x) printf("[TUNER ERR]");printf x
#else
#define TUNER_ERR(x) 
#endif

#if (TUNER_DEBUG_OPTIONS & TUNER_EN_DBG)
#define TUNER_DBG(x) printf("[TUNER DBG]");printf x
#else
#define TUNER_DBG(x) 
#endif



#if (TUNER_DEBUG_OPTIONS & TUNER_EN_API)
#define TUNER_ENTERAPI(x) printf x
#define TUNER_ENTERAPISTR(devId) printf("+++ %s [%d] \n\r", __FUNCTION__, devId)
#define TUNER_EXITAPI(x) printf x
#define TUNER_EXITAPISTR(devId, retcode) printf("--- %s [%d] (retcode: %d)\n\r", __FUNCTION__, devId, retcode)
#else
#define TUNER_ENTERAPI(x)
#define TUNER_ENTERAPISTR(devId)
#define TUNER_EXITAPI(x)
#define TUNER_EXITAPISTR(devId, retcode)
#endif

//===============================================


//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
#define _GET_TUNER_ENTRY_NODE(var, Name) var##Name
#define GET_TUNER_ENTRY_NODE(Name) _GET_TUNER_ENTRY_NODE(tuner_entry_,Name) 

#define DDIBLD_ATTRIB_TUNER_SECTION(__sect__) __attribute__((section (__sect__)))

#define __string(_x) #_x
#define __xstring(_x) __string(_x)

#ifndef DDI_DRV_TUNER_TABLE_ENTRY
#define DDI_DRV_TUNER_TABLE_ENTRY( _name ) \
        DDIBLD_ATTRIB_TUNER_SECTION(".ecos.tuner." __xstring(_name) ".data")
#endif

/// Define extension function types
typedef enum
{
    TUNER_EXT_FUNC_STANDBY,
    TUNER_EXT_FUNC_WAKEUP,
    TUNER_EXT_FUNC_LOOP_THROUGH,
    TUNER_EXT_FUNC_POWER_ON_OFF,
    TUNER_EXT_FUNC_GET_POWER_LEVEL,
    TUNER_EXT_FUNC_DECIDE_LNB_LO,
    TUNER_EXT_FUNC_FINALIZE,
    TUNER_EXT_FUNC_BLIND_SCAN_MODE,
    TUNER_EXT_FUNC_GET_RFAGC_STATUS,
    TUNER_EXT_FUNC_SET_CABLE_INDEX,
    TUNER_EXT_FUNC_GET_SLAVE_ID,
    TUNER_EXT_FUNC_UNDER_EXT_DMD_TEST,
    //for ATV VIF
    TUNER_EXT_FUNC_GET_FREQ_STEP,
    TUNER_EXT_FUNC_GET_TUNER_TYPE,
    TUNER_EXT_FUNC_GET_VIF_NOTCH_SOSFILTER,
    TUNER_EXT_FUNC_GET_PEAKING_PARAMETER,
    TUNER_EXT_RUNC_SET_TUNER_IN_SCAN_MODE,
    TUNER_EXT_FUNC_GET_VIF_CR,
    TUNER_EXT_FUNC_GET_ATV_RF_BOUNDARY,
    TUNER_EXT_FUNC_RESET_RFAGC,
    TUNER_EXT_FUNC_SET_CON_INFO
} TUNER_EXT_FUNCTION_TYPE;

typedef enum
{
    TUNER_BW_MODE_6MHZ = 0,                                                   ///< 6 MHz
    TUNER_BW_MODE_7MHZ,                                                       ///< 7 MHz
    TUNER_BW_MODE_8MHZ,                                                       ///< 8 MHz
    TUNER_BW_MODE_1_7MHZ
} TUNER_EN_TER_BW_MODE;

/// the RF band
typedef enum
{
    /// VHF low
    E_RFBAND_VHF_LOW,
    /// VHF high
    E_RFBAND_VHF_HIGH,
    /// UHF
    E_RFBAND_UHF,
    /// invalid
    E_RFBAND_INVALID,
} TUNER_EN_RFBAND;

/// the scan frequency step
typedef enum
{
    /// 31.25 KHz
    E_FREQ_STEP_31_25KHz = 0x00,
    /// 50 KHz
    E_FREQ_STEP_50KHz    = 0x01,
    /// 62.5 KHz
    E_FREQ_STEP_62_5KHz  = 0x02,
    /// invalid
    E_FREQ_STEP_INVALD
} TUNER_EN_FREQ_STEP;

typedef enum
{
    E_TUNER_SIF_STANDARD_TYPE_BG                    = 0x00,
    E_TUNER_SIF_STANDARD_TYPE_BG_A2                 = 0x01,
    E_TUNER_SIF_STANDARD_TYPE_BG_NICAM              = 0x02,
    E_TUNER_SIF_STANDARD_TYPE_I                     = 0x03,
    E_TUNER_SIF_STANDARD_TYPE_DK                    = 0x04,
    E_TUNER_SIF_STANDARD_TYPE_DK1_A2                = 0x05,
    E_TUNER_SIF_STANDARD_TYPE_DK2_A2                = 0x06,
    E_TUNER_SIF_STANDARD_TYPE_DK3_A2                = 0x07,
    E_TUNER_SIF_STANDARD_TYPE_DK_NICAM              = 0x08,
    E_TUNER_SIF_STANDARD_TYPE_L                     = 0x09,
    E_TUNER_SIF_STANDARD_TYPE_M                     = 0x0A,
    E_TUNER_SIF_STANDARD_TYPE_M_BTSC                = 0x0B,
    E_TUNER_SIF_STANDARD_TYPE_M_A2                  = 0x0C,
    E_TUNER_SIF_STANDARD_TYPE_M_EIA_J               = 0x0D,
    E_TUNER_SIF_STANDARD_TYPE_NOTSTANDARD           = 0x0F
}TUNER_SIF_StandardType;

typedef enum
{
    E_TUNER_ATV_TUNE_DEFAULT_MODE = 0,
    E_TUNER_ATV_TUNE_SCAN_MODE,
}TUNER_ATV_Tune_Mode;

typedef struct
{
  MS_IIC_PORT eI2C_PORT;     ///<I2C port
  MS_U32 u32HW_ResetPin;    ///<HW reset pin
} TUNER_CON_CONFIG;

typedef struct
{
    MS_U32* pCur_Broadcast_type;
    MS_U8 u8SlaveID;
    MS_U8 u8SlaveID_EXT;
    MS_U8 u8Cable_Index;  //for tuner that is with more than one connectors
    struct drv_demodtab_entry*  pstDemodtab;
    struct drv_dishtab_entry*   pstDishtab;
    TUNER_CON_CONFIG stTUNCon;
} TUNER_MS_INIT_PARAM;



typedef struct
{
    MS_U32 u32RF_FREQ;
    MS_U16 u16LoLOF; // low LOF value,unit MHz
    MS_U16 u16HiLOF; // high LOF value,unit MHz
    MS_U8  u8_Polarity;
    MS_U8 u8bPorInvert;
    MS_BOOL* pbIsHiLOF;
} TUNER_MS_SAT_PARAM;

typedef struct
{
    MS_U32 u32Param1;
    MS_U32 u32Param2;
    void *pParam;
} TUNER_EXT_FUNCTION_PARAM;

typedef MS_BOOL     drv_tunerop_Init(MS_U8 u8TunerIndex, TUNER_MS_INIT_PARAM* pParam);
typedef MS_BOOL     drv_tunerop_SetFreq(MS_U8 u8TunerIndex, MS_U32 u32Freq);
typedef MS_BOOL     drv_tunerop_SetFreq_S2(MS_U8 u8TunerIndex, MS_U32 u32Freq, MS_U32 u32SymbolRate);
typedef MS_BOOL     drv_tunerop_GetLock(MS_U8 u8TunerIndex);
typedef MS_BOOL     drv_tunerop_SetTuner(MS_U8 u8TunerIndex, MS_U32 dwFreq /*Khz*/, MS_U8 ucBw /*MHz*/);
typedef MS_BOOL     drv_tunerop_SetATVTuner(MS_U8 u8TunerIndex, MS_U32 u32FreqKHz,TUNER_EN_RFBAND eBand, MS_U8 u8OtherMode );
typedef MS_BOOL     drv_tunerop_CheckExist(MS_U8 u8TunerIndex, MS_U32* pu32channel_cnt);
typedef MS_BOOL     drv_tunerop_GetTunerIF(MS_U8 u8TunerIndex, MS_U32* pu32IF_Freq);
typedef MS_BOOL     drv_tunerop_Extension_Function(MS_U8 u8TunerIndex, TUNER_EXT_FUNCTION_TYPE fuction_type, void *data);


typedef struct 
drv_tunertab_entry
{
    const char                     *name;          // demod system name
    MS_U32                         data;            // private data value
    drv_tunerop_Init               *Init;
    drv_tunerop_SetFreq            *SetFreq;
    drv_tunerop_SetFreq_S2         *SetFreqS2;
    drv_tunerop_GetLock            *GetLock;
    drv_tunerop_SetTuner           *SetTuner;
    drv_tunerop_CheckExist         *CheckExist;
    drv_tunerop_GetTunerIF            *GetTunerIF;
    drv_tunerop_Extension_Function *Extension_Function;
    //ATV
    drv_tunerop_SetATVTuner        *SetATVTuner;
} DRV_TUNER_TABLE_TYPE;

//-------------------------------------------------------------------------------------------------
//  Function Prototype
//-------------------------------------------------------------------------------------------------

#endif // _DRV_TUNER_H_

