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
// Copyright (c) 2006-2008 MStar Semiconductor, Inc.
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

#ifndef _MSB122x_H_
#define _MSB122x_H_


#define EXTSEL extern


//--------------------------------------------------------------------
#define     DEMOD_SLAVE_ID          0x32
#define     DEMOD_SLAVE_ID_2        0x72
#define     DEMOD_ADDR_H            0x00
#define     DEMOD_ADDR_L            0x01
#define     DEMOD_WRITE_REG         0x02
#define     DEMOD_WRITE_REG_EX      0x03
#define     DEMOD_READ_REG          0x04
#define     DEMOD_RAM_CONTROL       0x05

#if 0//(MS_BOARD_TYPE_SEL == BD_MST098E_D01A_S || MS_BOARD_TYPE_SEL == BD_MST098E_D01B_S)
#define PAD_RESET_DEMOD PAD_SAR1

#else
#undef PAD_RESET_DEMOD
#endif


/*@ <Definitions> @*/
    // ------------------------------------------------------------
    // Other Define
    // -----------------------------------------------------------



    #define REG_CMD_CTRL                0x809C
    #define REG_DTA_CTRL                0x809D
    #define REG_CMD_ADDR                0x809E
    #define REG_CMD_DATA                0x809F
    #define _REG_START                  REG_CMD_CTRL
    #define _REG_END                    REG_CMD_CTRL
    #define _REG_DRQ                    REG_DTA_CTRL
    #define _REG_FSM                    REG_CMD_CTRL
    #define _REG_ERR                    REG_DTA_CTRL
    #define _BIT_START                  0x02
    #define _BIT_END                    0x01
    #define _BIT_DRQ                    0x01
    #define _BIT_FSM                    0x08
    #define _BIT_ERR                    0x80
    #define MSVIF_0702_ENABLE           1


/*@ </Definitions> @*/

typedef enum
{
    COFDM_FEC_LOCK,
    COFDM_PSYNC_LOCK,
    COFDM_TPS_LOCK,
    COFDM_DCR_LOCK,
    COFDM_AGC_LOCK,
    COFDM_MODE_DET,

} COFDM_LOCK_STATUS;




typedef struct
{
    MS_U8  cmd_code;
    MS_U8  param[64];
} S_CMDPKTREG;

typedef enum
{
    E_SYS_UNKOWN = -1,
    E_SYS_DVBT,
    E_SYS_DVBC,
    E_SYS_ATSC,
    E_SYS_VIF,

    E_SYS_NUM
}E_SYSTEM;

typedef enum
{
    CMD_SYSTEM_INIT = 0,
    CMD_DAC_CALI,
    CMD_DVBT_CONFIG,
    CMD_DVBC_CONFIG,
    CMD_VIF_CTRL,
    CMD_FSM_CTRL,
    CMD_INDIR_RREG,
    CMD_INDIR_WREG,
    CMD_GET_INFO,
    CMD_TS_CTRL,
    CMD_TUNER_CFG,
    CMD_TUNER_GET,
    CMD_TUNED_VALUE,

    CMD_MAX_NUM
}E_CMD_CODE;

typedef enum
{
    // OP Mode Settings
    p_opmode_rfagc_en = 0,
    p_opmode_humdet_en,
    p_opmode_dcr_en,
    p_opmode_iqb_en,
    p_opmode_auto_iq_swap,
    p_opmode_auto_fsa_left,
    p_opmode_auto_rfmax,
    p_opmode_mode_forced,
    p_opmode_cp_forced,

    // Config Params
    pc_config_rssi,
    pc_config_zif,
    pc_config_fc_l,
    pc_config_fc_h,
    pc_config_fs_l,
    pc_config_fs_h,
    pc_config_bw,
    pc_config_fsa_left,
    pc_config_rfmax,
    pc_config_lp_sel,
    pc_config_cp,
    pc_config_mode,
    pc_config_iq_swap,
    pc_config_atv_system,
    pc_config_serial_ts,
    pc_config_ts_out_inv,
    pc_config_ts_data_swap,
    pc_config_icfo_range,
    // pc_config_ts_serial_clk,

    DVBT_PARAM_LEN,
} DVBT_Param;

typedef enum
{
    agc_ref_small = 0,
    agc_ref_large,
    agc_ref_aci,
    ripple_switch_th_l,
    ripple_switch_th_h,

    TUNED_PARAM_MAX_NUM
}E_TUNED_PARAM;

typedef enum
{
    TS_PARALLEL = 0,
    TS_SERIAL = 1,

    TS_MODE_MAX_NUM
}E_TS_MODE;


typedef struct
{
    MS_FLOAT    power_db;
    MS_U8       sar3_val;
}S_RFAGC_SSI;

typedef struct
{
    MS_FLOAT    power_db;
    MS_U8       agc_val;
}S_IFAGC_SSI;

typedef struct
{
    MS_FLOAT    attn_db;
    MS_S8       agc_err;
}S_IFAGC_ERR;

typedef struct
{
    MS_U8       constel;
    MS_U8       code_rate;
    MS_FLOAT    p_ref;
}S_SSI_PREF;


typedef enum
{
    _QPSK       = 0x0,
    _16QAM      = 0x1,
    _64QAM      = 0x2,
}E_CONSTEL;

typedef enum
{
    _CR1Y2      = 0x0,
    _CR2Y3      = 0x1,
    _CR3Y4      = 0x2,
    _CR5Y6      = 0x3,
    _CR7Y8      = 0x4,
}E_CODERATE;

typedef enum
{
    E_DEVICE_DEMOD_NULL,
    E_DEVICE_DEMOD_DVB_T,
    E_DEVICE_DEMOD_DVB_T2,
    E_DEVICE_DEMOD_DVB_C,
    E_DEVICE_DEMOD_ISDBT,
}E_DEVICE_DEMOD_TYPE;

//--------------------------------------------------------------------
EXTSEL MS_BOOL MSB122x_Active(MS_BOOL bEnable);
EXTSEL MS_BOOL MSB122x_Cmd_Packet_Send(S_CMDPKTREG* pCmdPacket, MS_U8 param_cnt);
EXTSEL MS_BOOL MSB122x_Power_On_Initialization (MS_BOOL bIsDVBT);
EXTSEL MS_BOOL DTV_Set_Demod_TYPE(E_DEVICE_DEMOD_TYPE enDemodType);
EXTSEL E_DEVICE_DEMOD_TYPE DTV_Get_Demod_TYPE(void);
EXTSEL MS_BOOL DTV_Config(MS_U8 RfChBw, MS_BOOL bSerialTS, MS_BOOL bPalBG);
EXTSEL MS_BOOL DTV_SetFrequency(MS_U32 u32Frequency, MS_U8 eBandWidth);
EXTSEL MS_U32 DTV_GetSNR(void);
EXTSEL MS_U32 DTV_GetBER(void);
EXTSEL MS_U16 DTV_GetSignalQuality(void);
EXTSEL MS_U16 DTV_GetSignalStrength(void);
EXTSEL MS_BOOL DTV_Serial_Control(MS_BOOL bEnable);
EXTSEL MS_BOOL DTV_DVB_T_GetLockStatus(void);
EXTSEL MS_BOOL Connect(E_DEVICE_DEMOD_TYPE enDemodType);
EXTSEL MS_BOOL Disconnect(void);
EXTSEL void Reset(void);
EXTSEL MS_BOOL IIC_Bypass_Mode(MS_BOOL enable);

//--------------------------------------------------------------------

#undef EXTSEL
#endif

