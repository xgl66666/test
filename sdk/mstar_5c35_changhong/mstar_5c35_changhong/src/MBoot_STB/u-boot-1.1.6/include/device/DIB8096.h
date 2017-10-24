////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2008 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (��MStar Confidential Information��) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _MSB123x_H_
#define _MSB123x_H_


#define EXTSEL extern

#define DIB_RETURN_SUCCESS                   0
#define DIB_RETURN_ERROR                    -1
#define DIB_RETURN_NOT_SUPPORTED            -2

//--------------------------------------------------------------------

#if (MS_BOARD_TYPE_SEL == BD_CUS75_MBQ213A)
#define PAD_RESET_DEMOD PAD_SAR0

#elif (MS_BOARD_TYPE_SEL == BD_MST098E_D01A_S || MS_BOARD_TYPE_SEL == BD_MST098E_D01B_S)
#define PAD_RESET_DEMOD PAD_SAR1

#else
#undef PAD_RESET_DEMOD
#endif


/*@ <Definitions> @*/
    // ------------------------------------------------------------
    // Other Define
    // -----------------------------------------------------------


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
	U8		cmd_code;
	U8		param[64];
} S_CMDPKTREG;

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
	float	power_db;
	U8		sar3_val;
}S_RFAGC_SSI;

typedef struct
{
	float	power_db;
	U8		agc_val;
}S_IFAGC_SSI;

typedef struct
{
	float	attn_db;
	S8		agc_err;
}S_IFAGC_ERR;

typedef struct
{
	U8		constel;
	U8		code_rate;
	float	p_ref;
}S_SSI_PREF;


typedef enum
{
	_QPSK		= 0x0,
	_16QAM		= 0x1,
	_64QAM		= 0x2,
}E_CONSTEL;

typedef enum
{
	_CR1Y2		= 0x0,
	_CR2Y3		= 0x1,
	_CR3Y4		= 0x2,
	_CR5Y6		= 0x3,
	_CR7Y8		= 0x4,
}E_CODERATE;

//--------------------------------------------------------------------
EXTSEL BOOLEAN MSB122x_Active(BOOLEAN bEnable);
EXTSEL BOOLEAN MSB122x_Cmd_Packet_Send(S_CMDPKTREG* pCmdPacket, U8 param_cnt);
EXTSEL BOOLEAN MSB122x_Power_On_Initialization (BOOLEAN bIsDVBT);
#if 0
EXTSEL BOOLEAN DTV_Set_Demod_TYPE(MS_U32 enDemodType);
EXTSEL MS_U32 DTV_Get_Demod_TYPE(void);
#endif
//--------------------------------------------------------------------

#undef EXTSEL
#endif

