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
#ifndef __DRV_DEMOD_H__
#define __DRV_DEMOD_H__

#include "drvTuner.h"
#include "drvIIC.h"
#include "Frontend_Enum_Device.h"
#define MAX_TPS_LPF            10  //DVBS_STYLE

//============ debug setting ========================
#define DMD_EN_ERR 0x01
#define DMD_EN_DBG 0x02
#define DMD_EN_API 0x04
#define DMD_DEBUG_OPTIONS  0


#if (DMD_DEBUG_OPTIONS & DMD_EN_ERR)
#define DMD_ERR(x) printf("[DEMOD ERR]");printf x
#else
#define DMD_ERR(x)
#endif

#if (DMD_DEBUG_OPTIONS & DMD_EN_DBG)
#define DMD_DBG(x) printf("[DEMOD DBG]");printf x
#else
#define DMD_DBG(x)
#endif



#if (DMD_DEBUG_OPTIONS & DMD_EN_API)
#define DMD_ENTERAPI(x) printf x
#define DMD_ENTERAPISTR(devId) printf("+++ %s [%d] \n\r", __FUNCTION__, devId)
#define DMD_EXITAPI(x) printf x
#define DMD_EXITAPISTR(devId, retcode) printf("--- %s [%d] (retcode: %d)\n\r", __FUNCTION__, devId, retcode)
#else
#define DMD_ENTERAPI(x)
#define DMD_ENTERAPISTR(devId)
#define DMD_EXITAPI(x)
#define DMD_EXITAPISTR(devId, retcode)
#endif


#define DBG_MODE_AGC_OUT  0x03
#define DBG_MODE_AGC_ERR  0x05
#define DBG_MODE_AGC_MEAN 0x0a

#ifndef INTERN_DVBC_SUPPORT_DMD_INT
#define INTERN_DVBC_SUPPORT_DMD_INT 1
#endif

#ifndef INTERN_DVBT_SUPPORT_DMD_INT
#define INTERN_DVBT_SUPPORT_DMD_INT 1
#endif

#ifndef INTERN_DVBT2_SUPPORT_DMD_INT
#define INTERN_DVBT2_SUPPORT_DMD_INT 1
#endif

#ifndef INTERN_DVBS_SUPPORT_DMD_INT
#define INTERN_DVBS_SUPPORT_DMD_INT 1
#endif

#define DMD_CON_INFO_NOT_DEFINE -1

#define EXT_DMD_MSPI_PATH_MAX 3
#define EXT_DMD_MSPI_PATH_NONE 0xff

//===============================================
#if 0
/// Demodulator tune parameter
typedef struct
{
    // Channel setting
    MS_U32                          u32SymRate;                         ///< Symbol rate (Ksym/sec)
    // Channel characteristic
    DMD_QAMType                     eQamType;                           ///< QAM type
    DMD_IQType                      eIQSwap;                            ///< IQ type
    DMD_TapAssign                   eTapAssign;                         ///< Tap assign
    MS_U32                          u32FreqOffset;                      ///< Carrier frequency offset
    MS_U8                           u8TuneFreqOffset;                   ///< Request Tuner offset
    MS_U32                          u32TunerFreq;                       ///< Request Tuner offset
} DMD_Param;
#endif

typedef enum
{
    E_DEMOD_LOCK = 0,
    E_DEMOD_CHECKING,
    E_DEMOD_CHECKEND,
    E_DEMOD_UNLOCK,
    E_DEMOD_T_T2_UNLOCK,
    E_DEMOD_NULL,
} EN_LOCK_STATUS;

/// Define extension function types
typedef enum
{
    DEMOD_EXT_FUNC_OPEN,
    DEMOD_EXT_FUNC_CLOSE,
    DEMOD_EXT_FUNC_RESET,
    DEMOD_EXT_FUNC_TS_OUT,
    DEMOD_EXT_FUNC_POWER_ON_OFF,
    DEMOD_EXT_FUNC_SET_BW,
    DEMOD_EXT_FUNC_GET_BW,
    DEMOD_EXT_FUNC_SET_MODE,
    DEMOD_EXT_FUNC_CONFIG,
    DEMOD_EXT_FUNC_GET_ROLL_OFF,
    DEMOD_EXT_FUNC_GET_RF_OFFSET,
    DEMOD_EXT_FUNC_SET_TS_SERIAL,
    DEMOD_EXT_FUNC_FINALIZE,
    DEMOD_EXT_FUNC_CLEAR_STATUS,
#ifdef MS_DVBT2_INUSE
    DEMOD_EXT_FUNC_GET_PLPID_LIST,
    DEMOD_EXT_FUNC_CTRL_RESET_DJB_FLAG,
    DEMOD_EXT_FUNC_T2MI_RESTART,
    DEMOD_EXT_FUNC_INIT_PARAMETER,
    DEMOD_EXT_FUNC_T2_RESET,
    DEMOD_EXT_FUNC_T2_RESTART,
#endif
    DEMOD_EXT_FUNC_GET_T_T2_CELL_ID,
    DEMOD_EXT_FUNC_EXISTED,
    //DEMOD_EXT_FUNC_IIC_BYPASS_ON,
    //DEMOD_EXT_FUNC_IIC_BYPASS_OFF,
    DEMOD_EXT_FUNC_ATSC_GET_PRELOCK,           //pilot lock
    DEMOD_EXT_FUNC_ATSC_GET_FLAME_SYNC_LOCK,  //timing recovery lock
    DEMOD_EXT_FUNC_GET_PACKET_ERR,
    DEMOD_EXT_FUNC_GET_CFO,
    DEMOD_EXT_FUNC_GET_PACKGE_INFO, // MCP or External
    DEMOD_EXT_FUNC_SET_ISDBT_LAYER, // for ISDBT layer switch
    DEMOD_EXT_FUNC_SET_BUFFER_ADDR, //set demod buffer address
    //PID Filter
    DEMOD_EXT_FUNC_SET_PID_FILTER,
    DEMOD_EXT_FUNC_SET_DISEQC_TX,
    DEMOD_EXT_FUNC_SET_DISEQC_TX_22K_OFF,
    DEMOD_EXT_FUNC_SET_INTERRUPT_CALLBACK,
    DEMOD_EXT_FUNC_GET_IFAGC_OUT,
    DEMOD_EXT_FUNC_GET_BIT_RATE,
    DEMOD_EXT_FUNC_SET_RESET_PIN,
    DEMOD_EXT_FUNC_SET_CON_INFO
} DEMOD_EXT_FUNCTION_TYPE;


/// Define converlution code rate for DVB-T and DVB-S
typedef enum
{
    DEMOD_CONV_CODE_RATE_1_2=0,    ///< Code rate = 1/2
    DEMOD_CONV_CODE_RATE_1_3,    ///< Code rate = 1/3
    DEMOD_CONV_CODE_RATE_2_3,    ///< Code rate = 2/3
    DEMOD_CONV_CODE_RATE_1_4,    ///< Code rate = 1/4
    DEMOD_CONV_CODE_RATE_3_4,    ///< Code rate = 3/4
    DEMOD_CONV_CODE_RATE_2_5,    ///< Code rate = 2/5
    DEMOD_CONV_CODE_RATE_3_5,    ///< Code rate = 3/5
    DEMOD_CONV_CODE_RATE_4_5,    ///< Code rate = 4/5
    DEMOD_CONV_CODE_RATE_5_6,    ///< Code rate = 5/6
    DEMOD_CONV_CODE_RATE_7_8,    ///< Code rate = 7/8
    DEMOD_CONV_CODE_RATE_8_9,    ///< Code rate = 8/9
    DEMOD_CONV_CODE_RATE_9_10,   ///< Code rate = 9/10         
    //S2X
    DEMOD_DVBS2_CODE_RATE_2_9,
    DEMOD_DVBS2_CODE_RATE_13_45,
    DEMOD_DVBS2_CODE_RATE_9_20,
    DEMOD_DVBS2_CODE_RATE_90_180,
    DEMOD_DVBS2_CODE_RATE_96_180,
    DEMOD_DVBS2_CODE_RATE_11_20,
    DEMOD_DVBS2_CODE_RATE_100_180,
    DEMOD_DVBS2_CODE_RATE_104_180,
    DEMOD_DVBS2_CODE_RATE_26_45_L,
    DEMOD_DVBS2_CODE_RATE_18_30,
    DEMOD_DVBS2_CODE_RATE_28_45,    
    DEMOD_DVBS2_CODE_RATE_23_36,
    DEMOD_DVBS2_CODE_RATE_116_180,
    DEMOD_DVBS2_CODE_RATE_20_30,
    DEMOD_DVBS2_CODE_RATE_124_180,
    DEMOD_DVBS2_CODE_RATE_25_36,
    DEMOD_DVBS2_CODE_RATE_128_180,
    DEMOD_DVBS2_CODE_RATE_13_18,
    DEMOD_DVBS2_CODE_RATE_132_180,
    DEMOD_DVBS2_CODE_RATE_22_30,
    DEMOD_DVBS2_CODE_RATE_135_180,
    DEMOD_DVBS2_CODE_RATE_140_180,
    DEMOD_DVBS2_CODE_RATE_7_9,
    DEMOD_DVBS2_CODE_RATE_154_180,
    
    DEMOD_DVBS2_CODE_RATE_11_45,
    DEMOD_DVBS2_CODE_RATE_4_15,
    DEMOD_DVBS2_CODE_RATE_14_45,
    DEMOD_DVBS2_CODE_RATE_7_15,
    DEMOD_DVBS2_CODE_RATE_8_15,
    DEMOD_DVBS2_CODE_RATE_26_45_S,
    DEMOD_DVBS2_CODE_RATE_32_45     
} DEMOD_EN_CONV_CODE_RATE_TYPE;

/// Define terrestrial band width
typedef enum
{
    DEMOD_BW_MODE_6MHZ = 0,                                                   ///< 6 MHz
    DEMOD_BW_MODE_7MHZ,                                                       ///< 7 MHz
    DEMOD_BW_MODE_8MHZ,                                                       ///< 8 MHz
    DEMOD_BW_MODE_1_7MHZ
} DEMOD_EN_TER_BW_MODE;

/// Define terrestrial constellation type
typedef enum
{
    DEMOD_TER_QPSK,                                                           ///< QPSK type
    DEMOD_TER_QAM16,                                                          ///< QAM 16 type
    DEMOD_TER_QAM64,                                                           ///< QAM 64 type
    DEMOD_TER_QAM256                                                           ///< QAM 256 type
} DEMOD_EN_TER_CONSTEL_TYPE;

/// Define terrestrial hierarchy information
typedef enum
{
    DEMOD_TER_HIE_NONE,                                                       ///< Non-hierarchy
    DEMOD_TER_HIE_ALPHA_1,                                                    ///< Hierarchy alpha = 1
    DEMOD_TER_HIE_ALPHA_2,                                                    ///< Hierarchy alpha = 2
    DEMOD_TER_HIE_ALPHA_4                                                     ///< Hierarchy alpha = 4
} DEMOD_EN_TER_HIE_TYPE;

/// Define terrestrial guard interval
typedef enum
{
    DEMOD_TER_GI_1_32,                                                        ///< Guard interval value = 1/32
    DEMOD_TER_GI_1_16,                                                        ///< Guard interval value = 1/16
    DEMOD_TER_GI_1_8,                                                         ///< Guard interval value = 1/8
    DEMOD_TER_GI_1_4,                                                          ///< Guard interval value = 1/4
    DEMOD_TER_GI_1_128,                                                          ///< Guard interval value = 1/128
    DEMOD_TER_GI_19_128,                                                          ///< Guard interval value = 19/128
    DEMOD_TER_GI_19_256                                                          ///< Guard interval value = 19/256
} DEMOD_EN_TER_GI_TYPE;

/// Define terrestrial transmission mode
typedef enum
{
    DEMOD_TER_FFT_2K,                                                         /// 2k FFT mode
    DEMOD_TER_FFT_8K_GI_LONG,                                                 /// 8k with guard interval 1/32, 1/16, 1/8, 1/4
    DEMOD_TER_FFT_4K,                                                         /// 4k FFT mode
    DEMOD_TER_FFT_1K,                                                         /// 1k FFT mode
    DEMOD_TER_FFT_16K,                                                        /// 16k FFT mode
    DEMOD_TER_FFT_32K_GI_LONG,                                                /// 32k with guard interval 1/32, 1/16, 1/8
    DEMOD_TER_FFT_8K_GI_SHORT,                                                /// 8k with guard interval 1/128, 19/256, 19/128
    DEMOD_TER_FFT_32K_GI_SHORT                                                /// 32k with guard interval 1/128, 19/256, 19/128
} DEMOD_EN_TER_FFT_MODE;

/// Define terrestrial transmission mode
typedef enum
{
    DEMOD_TER_HP_SEL,                                                         ///< High priority level selection
    DEMOD_TER_LP_SEL                                                          ///< Low priority level selection
} DEMOD_EN_TER_LEVEL_SEL;

/// Define DVB-C modulation scheme
typedef enum
{
    DEMOD_CAB_QAM16,                                                          ///< QAM 16
    DEMOD_CAB_QAM32,                                                          ///< QAM 32
    DEMOD_CAB_QAM64,                                                          ///< QAM 64
    DEMOD_CAB_QAM128,                                                         ///< QAM 128
    DEMOD_CAB_QAM256,
    DEMOD_CAB_QAMAUTO

} DEMOD_EN_CAB_CONSTEL_TYPE;


/// Define DVB-S IQ tuning mode
typedef enum
{
    DEMOD_CAB_IQ_NORMAL,                                                      ///< Normal
    DEMOD_CAB_IQ_INVERT                                                       ///< Inverse
} DEMOD_EN_CAB_IQ_MODE;

/// Define DVB-S modulatiopn scheme
typedef enum
{
    DEMOD_SAT_DVBS2,                                                          ///< DVBS2
    DEMOD_SAT_DVBS                                                            ///< DVBS
} DEMOD_EN_SAT_MOD_TYPE;


/// Define DVB-S modulatiopn scheme
typedef enum
{
    DEMOD_SAT_QPSK,                                                           ///< QPSK
    DEMOD_SAT_8PSK,                                                           ///< 8PSK
    DEMOD_SAT_16APSK,                                                         ///< 16APSK
    DEMOD_SAT_32APSK,
    DEMOD_SAT_8APSK,
    DEMOD_SAT_8_8APSK,
    DEMOD_SAT_4_8_4_16APSK  
} DEMOD_EN_SAT_CONSTEL_TYPE;

/// Define DVB-S Roll-Off factor
typedef enum
{
    DEMOD_SAT_RO_35,                                                          ///< roll-off factor = 0.35
    DEMOD_SAT_RO_25,                                                          ///< roll-off factor = 0.25
    DEMOD_SAT_RO_20                                                           ///< roll-off factor = 0.20
} DEMOD_EN_SAT_ROLL_OFF_TYPE;

/// Define DVB-S IQ tuning mode
typedef enum
{
    DEMOD_SAT_IQ_NORMAL,                                                      ///< Normal
    DEMOD_SAT_IQ_INVERSE                                                      ///< Inverse
} DEMOD_EN_SAT_IQ_MODE;

/// Define Bit Error Rate range measure from signal
typedef enum
{
    DEMOD_BIT_ERR_RATIO_LOW ,                                                 ///< Low BER
    DEMOD_BIT_ERR_RATIO_MEDIUM ,                                              ///< Medium BER
    DEMOD_BIT_ERR_RATIO_HIGH                                                  ///< High BER
} DEMOD_EN_BIT_ERR_RATIO;

/// Define lock status of front end
typedef enum
{
    DEMOD_FE_UNLOCKED = 0,                                                    ///< Frontend is unlocked
    DEMOD_FE_LOCKED                                                           ///< Frontend is locked
} DEMOD_EN_FE_LOCK_STATUS;


/// Define tuning mode
/// NOTE: When this typedef is modified, the apiChScan should be rebuild.
typedef enum
{
    DEMOD_FE_TUNE_MANUAL,                                                     ///< Manual tuning to carrier
    DEMOD_FE_TUNE_AUTO,                                                       ///< Auto tuning to carrier
} DEMOD_EN_FE_TUNE_MODE;

/// Define output mode
/// NOTE: When this typedef is modified, the apiChScan should be rebuild.
typedef enum
{
    DEMOD_INTERFACE_SERIAL = 0,                                                   ///< Serial interface
    DEMOD_INTERFACE_PARALLEL                                                  ///< Parallel interface
} DEMOD_INTERFACE_MODE;

typedef enum
{
     E_DEMOD_TYPE_T =0,
     E_DEMOD_TYPE_T2,
     E_DEMOD_TYPE_S,
     E_DEMOD_TYPE_C
}EN_DEMOD_TYPE;


//#if ((MS_DVB_TYPE_SEL == ATSC))
typedef enum
{
	DEMOD_MODE_NTSC,
	DEMOD_MODE_256QAM,
	DEMOD_MODE_64QAM,
	DEMOD_MODE_8VSB,
	DEMOD_MODE_NUM
} DEMOD_EN_DEMOD_MODE;
//#endif

typedef enum
{
    DEMOD_CAB_BW_6M,
    DEMOD_CAB_BW_7M,
	DEMOD_CAB_BW_8M,
	DEMOD_CAB_BW_NUM
} DEMOD_CAB_BW;

typedef enum
{
    DEMOD_ISDBT_Layer_A = 0x00,
    DEMOD_ISDBT_Layer_B = 0x01,
    DEMOD_ISDBT_Layer_C = 0x02,
    DEMOD_ISDBT_Layer_INVALID,
} DEMOD_ISDBT_Layer;

typedef enum
{      
    DEMOD_ISDBT_FFT_2K = 0x00,  /// 2K     
    DEMOD_ISDBT_FFT_4K = 0x01,  /// 4k      
    DEMOD_ISDBT_FFT_8K = 0x02,  /// 8k     
    DEMOD_ISDBT_FFT_INVALID,    /// invalid indicator
} DEMOD_ISDBT_FFT_VAL;

typedef enum
{
    DEMOD_ISDBT_DQPSK   = 0,   /// DQPSK  
    DEMOD_ISDBT_QPSK    = 1,   /// QPSK
    DEMOD_ISDBT_16QAM   = 2,   /// 16QAM
    DEMOD_ISDBT_64QAM   = 3,   /// 64QAM
    DEMOD_ISDBT_QAM_INVALID,   /// invalid indicator
} DEMOD_ISDBT_CONSTEL_TYPE;

typedef enum
{
    DEMOD_ISDBT_CODERATE_1_2 = 0,   /// 1/2
    DEMOD_ISDBT_CODERATE_2_3 = 1,   /// 2/3
    DEMOD_ISDBT_CODERATE_3_4 = 2,   /// 3/4
    DEMOD_ISDBT_CODERATE_5_6 = 3,   /// 5/6
    DEMOD_ISDBT_CODERATE_7_8 = 4,   /// 7/8
    DEMOD_ISDBT_CODERATE_INVALID,   /// invalid indicator
} DEMOD_ISDBT_CODE_RATE;

typedef enum
{
    DEMOD_ISDBT_GUARD_INTERVAL_1_4  = 0,   /// 1/4
    DEMOD_ISDBT_GUARD_INTERVAL_1_8  = 1,   /// 1/8
    DEMOD_ISDBT_GUARD_INTERVAL_1_16 = 2,   /// 1/16
    DEMOD_ISDBT_GUARD_INTERVAL_1_32 = 3,   /// 1/32
    DEMOD_ISDBT_GUARD_INTERVAL_INVALID,    /// invalid indicator
} DEMOD_ISDBT_GUARD_INTERVAL;

typedef enum
{ 
    // 2K mode
    DEMOD_ISDBT_2K_TDI_0 = 0,   /// Tdi = 0
    DEMOD_ISDBT_2K_TDI_4 = 1,   /// Tdi = 4
    DEMOD_ISDBT_2K_TDI_8 = 2,   /// Tdi = 8
    DEMOD_ISDBT_2K_TDI_16 = 3,  /// Tdi = 16
    // 4K mode
    DEMOD_ISDBT_4K_TDI_0 = 4,   /// Tdi = 0
    DEMOD_ISDBT_4K_TDI_2 = 5,   /// Tdi = 2
    DEMOD_ISDBT_4K_TDI_4 = 6,   /// Tdi = 4
    DEMOD_ISDBT_4K_TDI_8 = 7,   /// Tdi = 8
    // 8K mode   
    DEMOD_ISDBT_8K_TDI_0 = 8,   /// Tdi = 0
    DEMOD_ISDBT_8K_TDI_1 = 9,   /// Tdi = 1
    DEMOD_ISDBT_8K_TDI_2 = 10,  /// Tdi = 2
    DEMOD_ISDBT_8K_TDI_4 = 11,  /// Tdi = 4
    DEMOD_ISDBT_TDI_INVALID,    /// invalid indicator
}DEMOD_ISDBT_TIME_INTERLEAVING;


typedef enum
{
    DEMOD_ATSC_VSB,
    DEMOD_ATSC_64QAM,
    DEMOD_ATSC_256QAM,
    DEMOD_ATSC_16QAM,
    DEMOD_ATSC_32QAM,
    DEMOD_ATSC_128QAM,
    DEMOD_ATSC_MAX,
    DEMOD_ATSC_NULL = DEMOD_ATSC_MAX
} DEMOD_ATSC_CONSTEL_TYPE;

typedef enum
{
    DEMOD_DVBT2_PLP_TYPE_COMMON = 0x00,
    DEMOD_DVBT2_PLP_TYPE_1 = 0x01,
    DEMOD_DVBT2_PLP_TYPE_2 = 0x02,
    DEMOD_DVBT2_PLP_TYPE_INVALID,
} DEMOD_DVBT2_PLP_TYPE;

typedef enum
{
    DEMOD_EVT_UNKNOWN=0,
    DEMOD_EVT_LOCK,
    DEMOD_EVT_UNLOCK
} DEMOD_EVENT_TYPE;

//ATV VIF

///FREQ type define
typedef enum
{
    IF_FREQ_INVALID,
    IF_FREQ_B,              // 38.90 MHz
    IF_FREQ_G,              // 38.90 MHz
    IF_FREQ_I,              // 38.90 MHz
    IF_FREQ_DK,             // 38.90 MHz
    IF_FREQ_L,              // 38.90 MHz
    IF_FREQ_L_PRIME,        // 33.90 MHz
    IF_FREQ_MN,             // 45.75 MHz
    IF_FREQ_J,              // 58.75 MHz
    IF_FREQ_PAL_38,         // 38.00 MHz
    IF_FREQ_MAX_IF_FREQ,
    IF_DIGITAL_MODE
} DEMOD_IF_FREQ;

/// Audio SIF Standard Type
typedef enum
{
    ///< Audio standard BG
    E_DEMOD_AUDIOSTANDARD_BG_,
    ///< Audio standard BG A2
    E_DEMOD_AUDIOSTANDARD_BG_A2_,
    ///< Audio standard BG NICAM
    E_DEMOD_AUDIOSTANDARD_BG_NICAM_,
    ///< Audio standard I
    E_DEMOD_AUDIOSTANDARD_I_,
    ///< Audio standard DK
    E_DEMOD_AUDIOSTANDARD_DK_,
    ///< Audio standard DK1 A2
    E_DEMOD_AUDIOSTANDARD_DK1_A2_,
    ///< Audio standard DK2 A2
    E_DEMOD_AUDIOSTANDARD_DK2_A2_,
    ///< Audio standard DK3 A2
    E_DEMOD_AUDIOSTANDARD_DK3_A2_,
    ///< Audio standard DK NICAM
    E_DEMOD_AUDIOSTANDARD_DK_NICAM_,
    ///< Audio standard L
    E_DEMOD_AUDIOSTANDARD_L_,
    ///< Audio standard L
    E_DEMOD_AUDIOSTANDARD_LP_,
    ///< Audio standard M
    E_DEMOD_AUDIOSTANDARD_M_,
    ///< Audio standard M BTSC
    E_DEMOD_AUDIOSTANDARD_M_BTSC_,
    ///< Audio standard M A2
    E_DEMOD_AUDIOSTANDARD_M_A2_,
    ///< Audio standard M EIA J
    E_DEMOD_AUDIOSTANDARD_M_EIA_J_,
    ///< Not Audio standard
    E_DEMOD_AUDIOSTANDARD_NOTSTANDARD_
} DEMOD_AUDIOSTANDARD_TYPE;

///AFC type define
typedef enum
{
    ///AFC below MINUS 187p5KHz
    E_AFC_BELOW_MINUS_187p5KHz      = 0x07,
    ///AFC MINUS 162p5KHz
    E_AFC_MINUS_162p5KHz            = 0x06,
    ///AFC MINUS 137p5KHz
    E_AFC_MINUS_137p5KHz            = 0x05,
    ///AFC MINUS 112p5KHz
    E_AFC_MINUS_112p5KHz            = 0x04,
    ///AFC MINUS 87p5KHz
    E_AFC_MINUS_87p5KHz             = 0x03,
    ///AFC MINUS 62p5KHz
    E_AFC_MINUS_62p5KHz             = 0x02,
    ///AFC MINUS 37p5KHz
    E_AFC_MINUS_37p5KHz             = 0x01,
    ///AFC MINUS 12p5KHz
    E_AFC_MINUS_12p5KHz             = 0x00,
    ///AFC PLUS 12p5KHz
    E_AFC_PLUS_12p5KHz              = 0x0F,
    ///AFC PLUS 37p5KHz
    E_AFC_PLUS_37p5KHz              = 0x0E,
    ///AFC PLUS 62p5KHz
    E_AFC_PLUS_62p5KHz              = 0x0D,
    ///AFC PLUS 87p5KHz
    E_AFC_PLUS_87p5KHz              = 0x0C,
    ///AFC PLUS 112p5KHz
    E_AFC_PLUS_112p5KHz             = 0x0B,
    ///AFC PLUS 137p5KHz
    E_AFC_PLUS_137p5KHz             = 0x0A,
    ///AFC PLUS 162p5KHz
    E_AFC_PLUS_162p5KHz             = 0x09,
    ///AFC ABOVE PLUS 187p5KHz
    E_AFC_ABOVE_PLUS_187p5KHz       = 0x08,
    ///AFC out of AFCWIN
    E_AFC_OUT_OF_AFCWIN             = 0x10,
} DEMOD_VIF_AFC;

///VIF Sound system
typedef enum
{
    ///VIF Sound B
    E_VIF_SOUND_B_STEREO_A2,
    ///VIF Sound B
    E_VIF_SOUND_B_MONO_NICAM,
    ///VIF Sound GH
    E_VIF_SOUND_GH_STEREO_A2,
    ///VIF Sound GH
    E_VIF_SOUND_GH_MONO_NICAM,
    ///VIF Sound I
    E_VIF_SOUND_I,
    ///VIF Sound DK1
    E_VIF_SOUND_DK1_STEREO_A2,
    ///VIF Sound DK2
    E_VIF_SOUND_DK2_STEREO_A2,
    ///VIF Sound 3
    E_VIF_SOUND_DK3_STEREO_A2,
    ///VIF Sound 4
    E_VIF_SOUND_DK_MONO_NICAM,
    ///VIF Sound L
    E_VIF_SOUND_L,
    ///VIF Sound LL
    E_VIF_SOUND_LL,
    ///VIF Sound MN
    E_VIF_SOUND_MN,
    ///EN_VIF_SOUND_SYSTEM enum Numbers
    E_VIF_SOUND_NUMS
} DEMOD_VIF_SOUND_SYSTEM;

/// VIFUserFilter
typedef struct
{
    MS_U16 VifSos21FilterC0;
    MS_U16 VifSos21FilterC1;
    MS_U16 VifSos21FilterC2;
    MS_U16 VifSos21FilterC3;
    MS_U16 VifSos21FilterC4;
    MS_U16 VifSos22FilterC0;
    MS_U16 VifSos22FilterC1;
    MS_U16 VifSos22FilterC2;
    MS_U16 VifSos22FilterC3;
    MS_U16 VifSos22FilterC4;
    MS_U16 VifSos31FilterC0;
    MS_U16 VifSos31FilterC1;
    MS_U16 VifSos31FilterC2;
    MS_U16 VifSos31FilterC3;
    MS_U16 VifSos31FilterC4;
    MS_U16 VifSos32FilterC0;
    MS_U16 VifSos32FilterC1;
    MS_U16 VifSos32FilterC2;
    MS_U16 VifSos32FilterC3;
    MS_U16 VifSos32FilterC4;
    MS_U8 VifUserPeakingFilterSelect;
    MS_U8 VifUserYcDelayFilterSelect;
    MS_U8 VifUserGroupDelayFilterSelect;
}DEMOD_VIF_UserFilter;

typedef struct
{
    MS_U32 VifCrRate_B;
    MS_BOOL VifCrInvert_B;
    MS_U32 VifCrRate_GH;
    MS_BOOL VifCrInvert_GH;
    MS_U32 VifCrRate_DK;
    MS_BOOL VifCrInvert_DK;
    MS_U32 VifCrRate_I;
    MS_BOOL VifCrInvert_I;
    MS_U32 VifCrRate_L;
    MS_BOOL VifCrInvert_L;
    MS_U32 VifCrRate_LL;
    MS_BOOL VifCrInvert_LL;
    MS_U32 VifCrRate_MN;
    MS_BOOL VifCrInvert_MN;
}DEMOD_VIF_CARRIER_RECOVERY;

/// Define tuning paramter of DVB-T front-end
typedef struct
{
//#if ((MS_DVB_TYPE_SEL == DVBT2))
    MS_U8                 u8PlpID;
    MS_U8                 u8ScanType;
//#endif
    DEMOD_EN_TER_BW_MODE                  eBandWidth;                         ///< Band width
    DEMOD_EN_TER_CONSTEL_TYPE             eConstellation;                     ///< Constellation type
    DEMOD_EN_TER_HIE_TYPE                 eHierarchy;                         ///< Hierarchy
    DEMOD_EN_TER_GI_TYPE                  eGuardInterval;                     ///< Guard interval
    DEMOD_EN_TER_FFT_MODE                 eFFT_Mode;                          ///< Transmission mode
    DEMOD_EN_CONV_CODE_RATE_TYPE          eHPCodeRate;                        ///< HP code rate
    DEMOD_EN_CONV_CODE_RATE_TYPE          eLPCodeRate;                        ///< LP code rate
    DEMOD_EN_TER_LEVEL_SEL                eLevelSel;                          ///< Select HP or LP level
} DEMOD_MS_TER_CARRIER_PARAM;

/// Define tuning paramter of DVB-C front-end
typedef struct
{
    DEMOD_EN_CAB_CONSTEL_TYPE             eConstellation;                     ///< Constellation type
    MS_U16                          u16SymbolRate;                      ///< Symbol rate (Ksym/sec)

    DEMOD_EN_CAB_IQ_MODE            eIQMode;                            ///< IQ Mode
    MS_U8                           u8TapAssign;                        ///< Tap assign
    MS_U32                          u32FreqOffset;                      ///< Carrier frequency offset
    MS_U8                           u8TuneFreqOffset;                       ///< Requeset tuner freq offset
#ifdef __KERNEL__
    MS_S64                          s64CFO;                                //Carrier frequency offset, kHz 
#else
    float                           fCFO;                                //Carrier frequency offset, kHz 
#endif
#ifdef SUPPORT_DVBC_DMD_BW_CHANGE
#if SUPPORT_DVBC_DMD_BW_CHANGE
    DEMOD_CAB_BW                    eBandWidth;
#endif
#endif
} DEMOD_MS_CAB_CARRIER_PARAM;

typedef struct
{
    DEMOD_ISDBT_CODE_RATE         eIsdbtCodeRate;
    DEMOD_ISDBT_GUARD_INTERVAL    eIsdbtGI;
    DEMOD_ISDBT_TIME_INTERLEAVING eIsdbtTDI;
    DEMOD_ISDBT_FFT_VAL           eIsdbtFFT;
    DEMOD_ISDBT_CONSTEL_TYPE      eIsdbtConstellation;
} DEMOD_MS_ISDBT_MODULATION_MODE;


typedef struct
{
    DEMOD_MS_ISDBT_MODULATION_MODE stLayerX_Param[3];
} DEMOD_MS_ISDBT_CARRIER_PARAM;

typedef struct
{
    DEMOD_ATSC_CONSTEL_TYPE eJ83BConstellation;
} DEMOD_MS_J83B_CARRIER_PARAM;

typedef struct
{
    float fSiCodeRate;
    MS_U16 u16SiInterLeaver;
    MS_U8 u8SiQamMode;//4QAM, 16QAM, 32QAM, 64QAM
    MS_U8 u8SiNR;//4QAM or 4QAM-NR
    MS_U8 u8SiCarrierMode; //0: multiple carrier, 1:single carrier
    MS_U16 u16PNM; //420, 595, 945
    MS_U8 u8PNC; //0: variable, 1: constant
} DEMOD_MS_DTMB_MODULATION_MODE;

typedef struct
{
    DEMOD_MS_DTMB_MODULATION_MODE stMOD;
} DEMOD_MS_DTMB_CARRIER_PARAM;


/// Define tuning paramter of DVB-S front-end
typedef struct
{
    DEMOD_EN_SAT_CONSTEL_TYPE             eConstellation;                     ///< Constellation type
    DEMOD_EN_SAT_ROLL_OFF_TYPE            eRollOff;                           ///< Roll-Off factor
    DEMOD_EN_SAT_IQ_MODE                  eIQ_Mode;                           ///< IQ mode
    DEMOD_EN_CONV_CODE_RATE_TYPE          eCodeRate;                          ///< Converlution code rate
    MS_U32                                u32SymbolRate;
    MS_BOOL                               bIsDVBS2;                           //DVBS or DVBS2
#if MS_DVBS_INUSE
    MS_U8                                 u8Polarity;                         // 0: Horizon; > 0(default 1): Vertical;
#endif
#ifdef __KERNEL__
    MS_S64                                s64CFO;
#else
    float                                 fCFO;                                //Carrier frequency offset, kHz
#endif
    MS_U8                                 u8VCM_IS_ID;
    MS_U8                                 u8VCM_MODE;                          //0:Disable, 1: Auto // Search ISID 2: set as u8VCM_ISID
} DEMOD_MS_SAT_CARRIER_PARAM;

typedef void (*fpDemodCB)(MS_U8 u8FEIndex,MS_U8 u8arg);


typedef struct
{
  MS_IIC_PORT eI2C_PORT;  ///<HW I2C port
  MS_U32 u32HW_ResetPin;    ///<HW reset pin
  MS_BOOL bTSClkInverse;    ///<Inverse TS clock or not
  MS_BOOL bTSIsParallel;    ///<TS is parallel mode or serial mode
  MS_BOOL bTS_3Wire_Mode;   ///<TS is serial 3-wire mode or 4-wire mode
  MS_U32 u32TSClkPhase;     ///<TS clk phase
  MS_U32 u32TSVLDInterrupt; ///<Interrupt of TS VLD
  MS_BOOL bSupportMSPILoad;  ///<Support MSPI load demod FW
  MS_BOOL bEnMSPIPathDet;   ///<Enable/Disable MSPI path detection
  MS_U32  u32TxRxSwitchPin;
  MS_BOOL bDiSeqcRXLowEn;
} DEMOD_CON_CONFIG;

typedef struct
{
    struct drv_tunertab_entry*             pstTunertab;
    MS_U8                                  u32DmxInputPath;
    fpDemodCB                              fpCB;
    MS_U8                                  u8FrontendIndex;
    DEMOD_CON_CONFIG                       stDMDCon;
} DEMOD_MS_INIT_PARAM;

/// Define carrier paramter of digital tuner
/// NOTE: When this typedef is modified, the apiChScan should be rebuild.
typedef struct
{
    MS_U32                          u32Frequency;                       ///< RF frequency
    union
    {
        DEMOD_MS_TER_CARRIER_PARAM        TerParam;                           ///< Paramters for DVB-T front-end
        DEMOD_MS_CAB_CARRIER_PARAM        CabParam;                           ///< Paramters for DVB-C front-end
        DEMOD_MS_SAT_CARRIER_PARAM        SatParam;                           ///< Paramters for DVB-S front-end
        DEMOD_MS_ISDBT_CARRIER_PARAM      ISDBTParam;                         ///< Paramters for ISDB-T front-end
        DEMOD_MS_J83B_CARRIER_PARAM       J83BParam;                          ///< Paramters for J83B front-end
        DEMOD_MS_DTMB_CARRIER_PARAM       DTMBParam;                          ///< Paramters for DTMB front-end
    };
} DEMOD_MS_FE_CARRIER_PARAM;

/// Define the quality report
typedef struct
{
    DEMOD_EN_FE_LOCK_STATUS               eLock;
    DEMOD_EN_BIT_ERR_RATIO                eBER;                               ///< Bit error rate
    MS_U8                           u8SNR;
    MS_U8                           u8Signal;
} DEMOD_MS_FE_CARRIER_STATUS;

/// Define the carrier information
typedef struct
{
    DEMOD_MS_FE_CARRIER_PARAM             Param;                              ///< Carrier parameter
    DEMOD_MS_FE_CARRIER_STATUS            Status;                             ///< Quality report
} DEMOD_MS_FE_CARRIER_INFO;

typedef struct
{
    // Demodulator option
    MS_BOOL                         bX4CFE_en;                          ///< Carrier frequency estimation
    MS_BOOL                         bPPD_en;                            ///< Tap assign estimation
    MS_BOOL                         bIQAutoSwap_en;                     ///< IQ mode auto swap
    MS_BOOL                         bQAMScan_en;                        ///< QAM type auto scan
    MS_BOOL                         bFHO_en;                            ///< FHO
    MS_BOOL                         (*fptTunerSet)(MS_U32);             ///< Tuner set freq function pointer
// } DMD_Mode;
} Demod_Mode;

typedef struct
{
    MS_U8            u8Tuner;
    MS_U32           u32IFFreq; //kHz
} DEMOD_MS_FE_IF;

typedef struct
{
  MS_U16      u16PIDValue;
  MS_U16      u16PIDRemapValue;
  MS_BOOL     bPIDDrop;
} DEMOD_PID_FILTER;

typedef struct
{
    MS_U8 u8SlaveID;
    MS_BOOL bInUse;
}SLAVE_ID_USAGE;

typedef struct
{
    MS_U8 u8Nbyte;
    MS_U8 u8Msg[32];
}DISEQC_RX_MSG;

typedef struct
{
    MS_U32 u32Param1;
    MS_U32 u32Param2;
    void *pParam;
} DEMOD_EXT_FUNCTION_PARAM;


#define _GET_DEMOD_ENTRY_NODE(var, Name) var##Name
#define GET_DEMOD_ENTRY_NODE(Name) _GET_DEMOD_ENTRY_NODE(demod_entry_,Name)

#define DDIBLD_ATTRIB_SECTION(__sect__) __attribute__((section (__sect__)))

#define __string(_x) #_x
#define __xstring(_x) __string(_x)

#ifndef DDI_DRV_TABLE_ENTRY
#define DDI_DRV_TABLE_ENTRY( _name ) \
        DDIBLD_ATTRIB_SECTION(".ecos.demod." __xstring(_name) ".data")
#endif
//for MSPI load FW
typedef void (*fpSPIPAD_En)(MS_BOOL bOnOff);

typedef MS_BOOL     drv_demodop_init(MS_U8 u8DemodIndex, DEMOD_MS_INIT_PARAM* pParam);
typedef MS_BOOL     drv_demodop_Reset(MS_U8 u8DemodIndex);
typedef MS_BOOL     drv_demodop_GetLock(MS_U8 u8DemodIndex,EN_LOCK_STATUS *pbLock);
#ifdef __KERNEL__
typedef MS_BOOL     drv_demodop_GetSNR(MS_U8 u8DemodIndex, MS_S32 *ps32SNR);
typedef MS_BOOL     drv_demodop_GetBER(MS_U8 u8DemodIndex, MS_S32 *ps32BER);
#else
typedef MS_BOOL     drv_demodop_GetSNR(MS_U8 u8DemodIndex,float *pfSNR);
typedef MS_BOOL     drv_demodop_GetBER(MS_U8 u8DemodIndex,float *pfBER);
#endif
typedef MS_BOOL     drv_demodop_GetPWR(MS_U8 u8DemodIndex,MS_S32 *ps32Signal);
typedef MS_BOOL     drv_demodop_GetSSI(MS_U8 u8DemodIndex,MS_U16 *pu16SSI);
typedef MS_BOOL     drv_demodop_GetSignalQuality(MS_U8 u8DemodIndex,MS_U16 *pu16quality);
typedef MS_BOOL     drv_demodop_GetParam(MS_U8 u8DemodIndex,DEMOD_MS_FE_CARRIER_PARAM* pParam); // MS_BOOL MDrv_Demod_GetParam(DMD_Param *pParam);
typedef MS_BOOL     drv_demodop_Restart(MS_U8 u8DemodIndex,DEMOD_MS_FE_CARRIER_PARAM* pParam,MS_U32 u32BroadCastType); // MS_BOOL MDrv_Demod_Restart(DMD_Param *pParam);
typedef MS_BOOL     drv_demodop_I2C_ByPass(MS_U8 u8DemodIndex,MS_BOOL bOn);
typedef MS_BOOL     drv_demodop_CheckExist(MS_U8 u8DemodIndex, MS_U8* pu8SlaveID);
typedef MS_BOOL     drv_demodop_Extension_Function(MS_U8 u8DemodIndex, DEMOD_EXT_FUNCTION_TYPE fuction_type, void *data);

#ifdef FE_AUTO_TEST
typedef MS_BOOL     drv_demodop_ReadReg(MS_U8 u8DemodIndex, MS_U16 RegAddr, MS_U8 *pu8Data);
typedef MS_BOOL     drv_demodop_WriteReg(MS_U8 u8DemodIndex, MS_U16 RegAddr, MS_U16 RegData);
#endif

typedef MS_BOOL     drv_demodop_Get_Packet_Error(MS_U8 u8DemodIndex, MS_U16 *u16_data);

#if MS_DVBT2_INUSE
typedef MS_BOOL     drv_demodop_SetCurrentDemodType(MS_U8 u8DemodIndex, MS_U8 type);
typedef MS_U8       drv_demodop_GetCurrentDemodType(MS_U8 u8DemodIndex);
typedef MS_BOOL     drv_demodop_GetPlpBitMap(MS_U8 u8DemodIndex, MS_U8* u8PlpBitMap);
typedef MS_BOOL     drv_demodop_GetPlpGroupID(MS_U8 u8DemodIndex, MS_U8 u8PlpID, MS_U8* u8GroupID);
typedef MS_BOOL     drv_demodop_SetPlpGroupID(MS_U8 u8DemodIndex, MS_U8 u8PlpID, MS_U8 u8GroupID);
typedef MS_BOOL     drv_demod_SetScanTypeStatus(MS_U8 u8DemodIndex, MS_U8 status);
typedef MS_U8       drv_demod_GetScanTypeStatus(MS_U8 u8DemodIndex);
typedef MS_BOOL     drv_demod_GetNextPLPID(MS_U8 u8DemodIndex, MS_U8 Index, MS_U8* pu8PLPID);
typedef MS_BOOL     drv_demod_GetPLPType(MS_U8 u8DemodIndex, DEMOD_DVBT2_PLP_TYPE* ePLPTYPE);
#endif

#if MS_DVBS_INUSE
typedef MS_BOOL     drv_demodop_BlindScan_Start(MS_U8 u8DemodIndex,MS_U16 u16StartFreq,MS_U16 u16EndFreq);
typedef MS_BOOL     drv_demodop_BlindScan_NextFreq(MS_U8 u8DemodIndex,MS_BOOL* bBlindScanEnd);
typedef MS_BOOL     drv_demodop_BlindScan_WaitCurFreqFinished(MS_U8 u8DemodIndex,MS_U8* u8Progress,MS_U8 *u8FindNum);
typedef MS_BOOL     drv_demodop_BlindScan_Cancel(MS_U8 u8DemodIndex);
typedef MS_BOOL     drv_demodop_BlindScan_End(MS_U8 u8DemodIndex);
typedef MS_BOOL     drv_demodop_BlindScan_GetChannel(MS_U8 u8DemodIndex, MS_U16 u16ReadStart,MS_U16* u16TPNum,DEMOD_MS_FE_CARRIER_PARAM *pTable);
typedef MS_BOOL     drv_demodop_BlindScan_GetCurrentFreq(MS_U8 u8DemodIndex, MS_U32 *u32CurrentFeq);
typedef MS_BOOL     drv_demodop_DiSEqC_SetTone(MS_U8 u8DemodIndex, MS_BOOL bTone1);
typedef MS_BOOL     drv_demodop_DiSEqC_SetLNBOut(MS_U8 u8DemodIndex, MS_BOOL bLow);
typedef MS_BOOL     drv_demodop_DiSEqC_GetLNBOut(MS_U8 u8DemodIndex, MS_BOOL* bLNBOutLow);
typedef MS_BOOL     drv_demodop_DiSEqC_Set22kOnOff(MS_U8 u8DemodIndex, MS_BOOL b22kOn);
typedef MS_BOOL     drv_demodop_DiSEqC_Get22kOnOff(MS_U8 u8DemodIndex, MS_BOOL* b22kOn);
typedef MS_BOOL     drv_demodop_DiSEqC_SendCmd(MS_U8 u8DemodIndex, MS_U8* pCmd,MS_U8 u8CmdSize);
typedef MS_BOOL     drv_demodop_DiSEqC_GetReply(MS_U8 u8DemodIndex, DISEQC_RX_MSG* pstRxMSG);
typedef MS_BOOL     drv_demodop_Get_VCM_ISID_INFO(MS_U8 u8DemodIndex, MS_U8* pu8CurIS_ID, MS_U8* pu8Table);
typedef MS_BOOL     drv_demodop_Set_VCM_ISID(MS_U8 u8DemodIndex, MS_U8 u8IS_ID);
#endif

typedef struct drv_demodtab_entry
{
    const char              *name;          // demod system name
    MS_U32                  data;            // private data value
    MS_BOOL                 SupportINT;
    drv_demodop_init        *init;
    drv_demodop_GetLock     *GetLock;
    drv_demodop_GetSNR      *GetSNR;
    drv_demodop_GetBER      *GetBER;
    drv_demodop_GetPWR      *GetPWR;
    drv_demodop_GetSSI      *GetSSI;
    drv_demodop_GetSignalQuality *GetQuality;
    drv_demodop_GetParam    *GetParam;
    drv_demodop_Restart     *Restart;
    drv_demodop_I2C_ByPass  *I2CByPass;
    drv_demodop_I2C_ByPass  *I2CByPassPreSetting;
    drv_demodop_Extension_Function  *Extension_Function;
    drv_demodop_Extension_Function  *Extension_FunctionPreSetting;
    drv_demodop_CheckExist  *CheckExist;
    #if MS_DVBT2_INUSE
    drv_demodop_SetCurrentDemodType *SetCurrentDemodType;
    drv_demodop_GetCurrentDemodType *GetCurrentDemodType;
    drv_demodop_GetPlpBitMap *GetPlpBitMap;
    drv_demodop_GetPlpGroupID *GetPlpGroupID;
    drv_demodop_SetPlpGroupID *SetPlpGroupID;
    drv_demod_SetScanTypeStatus *SetScanTypeStatus;
    drv_demod_GetScanTypeStatus *GetScanTypeStatus;
    drv_demod_GetNextPLPID *GetNextPLPID;
    drv_demod_GetPLPType *GetPLPType;
    #endif
    #if MS_DVBS_INUSE
    drv_demodop_BlindScan_Start *BlindScanStart;
    drv_demodop_BlindScan_NextFreq  *BlindScanNextFreq;
    drv_demodop_BlindScan_WaitCurFreqFinished   *BlindScanWaitCurFreqFinished;
    drv_demodop_BlindScan_Cancel    *BlindScanCancel;
    drv_demodop_BlindScan_End   *BlindScanEnd;
    drv_demodop_BlindScan_GetChannel    *BlindScanGetChannel;
    drv_demodop_BlindScan_GetCurrentFreq    *BlindScanGetCurrentFreq;
    drv_demodop_DiSEqC_SetTone  *DiSEqCSetTone;
    drv_demodop_DiSEqC_SetLNBOut    *DiSEqCSetLNBOut;
    drv_demodop_DiSEqC_GetLNBOut    *DiSEqCGetLNBOut;
    drv_demodop_DiSEqC_Set22kOnOff  *DiSEqCSet22kOnOff;
    drv_demodop_DiSEqC_Get22kOnOff  *DiSEqCGet22kOnOff;
    drv_demodop_DiSEqC_SendCmd  *DiSEqC_SendCmd;
    drv_demodop_DiSEqC_GetReply *DiSEqC_GetReply;
    drv_demodop_Get_VCM_ISID_INFO *GetISIDInfo;
    drv_demodop_Set_VCM_ISID *SetISID;
    #endif
    drv_demodop_Get_Packet_Error *Get_Packet_Error;
    #ifdef FE_AUTO_TEST
    drv_demodop_ReadReg  *ReadReg;
    drv_demodop_WriteReg *WriteReg;
    #endif
} DRV_DEMOD_TABLE_TYPE;




#endif // #define __DRV_DEMOD_H__
