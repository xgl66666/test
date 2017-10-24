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

////////////////////////////////////////////////////////////////////////////////
//
/// @file MSB122x.c
/// @brief MSB122x DVBT, VIF Controller Interface
/// @author MStar Semiconductor, Inc.
//
////////////////////////////////////////////////////////////////////////////////



#ifndef _MSB123x_C_
#define _MSB123x_C_
/*
#include "msAPI_Tuner.h"
#include "msAPI_Tuning.h"
#include "MSB122x.h"
#include "drvIIC.h"
#include "drvGPIO.h"
*/
#include <sys/common/MsTypes.h>
#include <sys/common/MsIRQ.h>
#include <sys/common/MsOS.h>
#include <msAPI_Tuner.h>
#include <device/MSB122x.h>

MS_U8 MSB122x_table[] = {
#include "msb122x_dvbt.dat"
};

#if 0
MS_U8 MSB122x_tableGet[] = {
#include "msb122x_dvbt.dat"
};
#endif


S_IFAGC_SSI IfagcSsi_HiRef[] =
{
    {-48,    0x52},
    {-49,    0x53},
    {-50,    0x55},
    {-51,    0x57},
    {-52,    0x59},
    {-53,    0x5B},
    {-54,    0x5E},
    {-55,    0x60},
    {-56,    0x62},
    {-57,    0x65},
    {-58,    0x67},
    {-59,    0x6A},
    {-60,    0x6C},
    {-61,    0x6F},
    {-62,    0x71},
    {-63,    0x74},
    {-64,    0x77},
    {-65,    0x7A},
    {-66,    0x7D},
    {-67,    0x80},
    {-68,    0x82},
    {-69,    0x84},
    {-70,    0x86},
    {-71,    0x88},
    {-72,    0x8A},
    {-73,    0x8C},
    {-74,    0x8E},
    {-75,    0x91},
    {-76,    0x93},
    {-77,    0x95},
    {-78,    0x98},
    {-79,    0x9B},
    {-80,    0x9D},
    {-81,    0xA0},
    {-82,    0xA2},
    {-83,    0xA5},
    {-84,    0xA7},
    {-85,    0xA9},
    {-86,    0xAC},
    {-87,    0xAE},
    {-88,    0xB0},
    {-89,    0xB3},
    {-90,    0xFF},
};


S_IFAGC_ERR IfagcErr_HiRef[] =
{
    {0,        0x04},
    {-1,       0x08},
    {-2,       0x0f},
    {-3,       0x14},
    {-4,       0x18},
    {-5,       0x1C},
    {-6,       0x20},
    {-7,       0x23},
    {-8,       0x27},
    {-9,       0x29},
    {-10,   0x2B},
    {-11,   0x2C},
    {-12,     0x2E},
    {-13,    0x30},
    {-14,   0x31},
    {-15,   0x32},
    {-16,    0x7F}, // stopper
};

S_IFAGC_SSI IfagcSsi_LoRef[] =
{
    {-48,    0x4C},
    {-49,    0x4D},
    {-50,    0x4E},
    {-51,    0x50},
    {-52,    0x52},
    {-53,    0x54},
    {-54,    0x57},
    {-55,    0x59},
    {-56,    0x5B},
    {-57,    0x5D},
    {-58,    0x60},
    {-59,    0x62},
    {-60,    0x64},
    {-61,    0x67},
    {-62,    0x69},
    {-63,    0x6C},
    {-64,    0x6E},
    {-65,    0x71},
    {-66,    0x74},
    {-67,    0x77},
    {-68,    0x7A},
    {-69,    0x7D},
    {-70,    0x7F},
    {-71,    0x81},
    {-72,    0x83},
    {-73,    0x85},
    {-74,    0x87},
    {-75,    0x89},
    {-76,    0x8B},
    {-77,    0x8E},
    {-78,    0x90},
    {-79,    0x93},
    {-80,    0x95},
    {-81,    0x97},
    {-82,    0x9A},
    {-83,    0x9D},
    {-84,    0x9F},
    {-85,    0xA1},
    {-86,    0xA4},
    {-87,    0xA6},
    {-88,    0xA8},
    {-89,    0xAB},
    {-90,    0xFF},
};

S_IFAGC_ERR IfagcErr_LoRef[] =
{
    {0,        0x04},
    {-1,       0x07},
    {-2,       0x0A},
    {-3,       0x0D},
    {-4,       0x0F},
    {-5,       0x11},
    {-6,       0x13},
    {-7,       0x15},
    {-8,       0x16},
    {-9,       0x17},
    {-10,   0x18},
    {-11,   0x19},
    {-12.5, 0x1A},
    {-14,   0x1B},
    {-15,   0x1C},
    {-16,    0x7F},
    // stopper
};

S_RFAGC_SSI RfagcSsi[] =
{
    {-19.40,    0xA9},
    {-20.40,    0xAA},
    {-21.40,    0xAB},
    {-22.40,    0xAC},
    {-23.40,    0xAD},
    {-24.40,    0xAE},
    {-25.20,    0xAF},
    {-26.60,    0xB0},
    {-28.00,    0xB1},
    {-29.20,    0xB2},
    {-31.50,    0xB3},
    {-32.75,    0xB4},
    {-34.00,    0xB5},
    {-35.25,    0xB6},
    {-36.75,    0xB7},
    {-38.50,    0xB9},
    {-40.00,    0xBB},
    {-42.00,    0xBC},
    {-43.50,    0xBE},
    {-44.80,    0xC0},
    {-46.20,    0xC2},
    {-47.40,    0xC6},
    {-48.00,    0xCA},
    {-48.40,    0xD1},
    {-49.00,    0xE1},
    {-99.00,    0xFF},
};

S_SSI_PREF SsiPref[] =
{
    {_QPSK,  _CR1Y2, -21.0},
    {_QPSK,  _CR2Y3, -79.0},
    {_QPSK,  _CR3Y4, -78.0},
    {_QPSK,  _CR5Y6, -77.0},
    {_QPSK,  _CR7Y8, -76.0},
    {_16QAM, _CR1Y2, -75.0},
    {_16QAM, _CR2Y3, -73.0},
    {_16QAM, _CR3Y4, -72.0},
    {_16QAM, _CR5Y6, -71.0},
    {_16QAM, _CR7Y8, -70.0},
    {_64QAM, _CR1Y2, -69.0},
    {_64QAM, _CR2Y3, -68.0},
    {_64QAM, _CR3Y4, -66.0},
    {_64QAM, _CR5Y6, -65.0},
    {_64QAM, _CR7Y8, -64.0},
};


#define DBG_DOMOD_MSB(x)      x

static MS_U8 u8MsbData[6];
static MS_U32 u32ChkScanTimeStart = 0;
static BOOLEAN bPower_init_en = FALSE;

BOOLEAN FECLock;
EN_DEVICE_DEMOD_TYPE m_enCurrentDemodulator_Type = E_DEVICE_DEMOD_DVB_T;
S_CMDPKTREG gsCmdPacket;
U8 u8DeviceBusy = 0;
U8 gu8ChipRevId;
BOOLEAN gbSerialOut = FALSE;

BOOLEAN Active(BOOLEAN bEnable);
BOOLEAN LoadDSPCode(void);
static BOOLEAN ReadReg(U16 u16Addr, U8 *pu8Data);
static BOOLEAN WriteReg(U16 u16Addr, U8 u8Data);
void Reset(void);
void Driving_Control(BOOLEAN bEnable);


float Log10Approx(float flt_x)
{
	MS_U32		u32_temp = 1;
	MS_U8		indx = 0;

	do {
		u32_temp = u32_temp << 1;
		if (flt_x < (float)u32_temp)
			break;
	}while (++indx < 32);

	// 10*log10(X) ~= 0.3*N, when X ~= 2^N
	return (float)0.3 * indx;
}


//##########################################################################################################
//##########################################################################################################
//########################################  Public:DTV Implementation ##################################
//##########################################################################################################
//##########################################################################################################
/************************************************************************************************
Subject:    channel change config
Function:   MSB123x_Config
Parmeter:   BW: bandwidth
Return:     BOOLEAN :
Remark:
 *************************************************************************************************/
BOOLEAN DTV_Config(RF_CHANNEL_BANDWIDTH RfChBw, BOOLEAN bSerialTS, BOOLEAN bPalBG)
{
    MS_U8 bandwidth;
    MS_U8 status = TRUE;
    switch (RfChBw)
    {
        case E_RF_CH_BAND_6MHz:
            bandwidth = 1;
            break;
        case E_RF_CH_BAND_7MHz:
            bandwidth = 2;
            break;
        case E_RF_CH_BAND_8MHz:
        default:
            bandwidth = 3;
            break;
    }
    Reset();
    //// MSB1228 system init: DVB-T //////////
    gsCmdPacket.cmd_code = CMD_SYSTEM_INIT;
    gsCmdPacket.param[0] = E_SYS_DVBT;
    status &= MSB122x_Cmd_Packet_Send(&gsCmdPacket, 1);
    //// DVB-T configurate ///////////////////
    gsCmdPacket.cmd_code = CMD_DVBT_CONFIG;
    gsCmdPacket.param[p_opmode_rfagc_en] = 0;
    gsCmdPacket.param[p_opmode_humdet_en] = 0;
    gsCmdPacket.param[p_opmode_dcr_en] = 1;
    gsCmdPacket.param[p_opmode_iqb_en] = 0;
    gsCmdPacket.param[p_opmode_auto_iq_swap] = 1;
    gsCmdPacket.param[p_opmode_auto_fsa_left] = 1;
    gsCmdPacket.param[p_opmode_auto_rfmax] = 0;
    gsCmdPacket.param[p_opmode_mode_forced] = 0;
    gsCmdPacket.param[p_opmode_cp_forced] = 0;
    gsCmdPacket.param[pc_config_rssi] = 0;
    gsCmdPacket.param[pc_config_zif] = 0;
    gsCmdPacket.param[pc_config_fc_l] = 0x5a;
    gsCmdPacket.param[pc_config_fc_h] = 0x24;
    gsCmdPacket.param[pc_config_fs_l] = 0xa1;
    gsCmdPacket.param[pc_config_fs_h] = 0xb1;
    gsCmdPacket.param[pc_config_bw] = bandwidth;
    gsCmdPacket.param[pc_config_fsa_left] = 0;
    gsCmdPacket.param[pc_config_rfmax] = 1;
    gsCmdPacket.param[pc_config_lp_sel] = 0;
    gsCmdPacket.param[pc_config_cp] = 3;
    gsCmdPacket.param[pc_config_mode] = 1;
    gsCmdPacket.param[pc_config_iq_swap] = 0;
    gsCmdPacket.param[pc_config_atv_system] = bPalBG ? 0 : 1;
    gsCmdPacket.param[pc_config_serial_ts] =bSerialTS;
    #if (TS_CLK_INV == 1)
    gsCmdPacket.param[pc_config_ts_out_inv] = true;
    #else
     gsCmdPacket.param[pc_config_ts_out_inv]= false;
    #endif
    gsCmdPacket.param[pc_config_ts_data_swap] = 0;
    gsCmdPacket.param[pc_config_icfo_range] = 1; // HD-Forum frequency offset +3;
    // gsCmdPacket.param[pc_config_ts_serial_clk]   = TS_SERIAL_CLK_57p6M;
    status &= MSB122x_Cmd_Packet_Send(&gsCmdPacket, DVBT_PARAM_LEN);
    //// DVB-T AGC_REF_x, ripple_switch_th configure/////

    gsCmdPacket.cmd_code = CMD_TUNED_VALUE;
    // default value
    gsCmdPacket.param[agc_ref_small] = 168;
    gsCmdPacket.param[agc_ref_large] = 248;
    gsCmdPacket.param[agc_ref_aci] = 150;
    // 0x19A = 410
    gsCmdPacket.param[ripple_switch_th_l] = 0x9A;
    gsCmdPacket.param[ripple_switch_th_h] = 0x01;
    status &= MSB122x_Cmd_Packet_Send(&gsCmdPacket, TUNED_PARAM_MAX_NUM);
    return status;


}


//##########################################################################################################
//##########################################################################################################
//########################################  Public:DTV Implementation ##################################
//##########################################################################################################
//##########################################################################################################

BOOLEAN DTV_SetFrequency(U32 u32Frequency, RF_CHANNEL_BANDWIDTH eBandWidth)
{
    MS_BOOL b_pal_bg;
    MS_BOOL bStatus = FALSE;
    DBG_DOMOD_MSB(printf("%04d %s %d ,%d\n", __LINE__, __FUNCTION__, (int)u32Frequency, (int)eBandWidth));

    FECLock = FALSE;
    b_pal_bg = FALSE;

    bStatus = DTV_Config(eBandWidth, gbSerialOut, b_pal_bg);

    u32Frequency = u32Frequency;

    if (bStatus == FALSE)
    {
        printf("Demod_Config failed \n");
        return FALSE;
    }

    //printf("Demod_Config OK \n");
    bStatus = Active(TRUE);

    if (bStatus == FALSE)
    {
        printf("Demod Active failed \n");
        return FALSE;
    }
    printf("Demod Active OK \n");
    u32ChkScanTimeStart = MsOS_GetSystemTime();
    return TRUE;

}


U32 DTV_GetSNR(void)
{
    return 1;
}


U32 DTV_GetBER(void)
{
    return 0;
}




U16 DTV_GetSignalQuality(void)
{
    //return 80; //sttest
    float fber;
    float log_ber;
    MS_U8 u8Data;

    if(FECLock == FALSE)
        return 0;

    if (ReadReg(0x872C, &u8Data) == FALSE)
    {
        DBG_DOMOD_MSB(printf("DTV_GetSignalQuality ReadReg fail!!! \n"));
        return 0;
    }

    if (u8Data &0x02 /*BIT1*/)
    {
        MS_U8 status = true;
        MS_U8 reg, reg_frz;
        MS_U16 BitErrPeriod;
        MS_U32 BitErr;
        MS_U16 PktErr;

        /////////// Post-Viterbi BER /////////////
        // bank 7 0x32 [7] reg_bit_err_num_freeze
        status &= ReadReg(0x8732, &reg_frz);
        status &= WriteReg(0x8732, reg_frz | 0x80);

        // bank 7 0x30 [7:0] reg_bit_err_sblprd_7_0
        //             [15:8] reg_bit_err_sblprd_15_8
        status &= ReadReg(0x8731, &reg);
        BitErrPeriod = reg;
        status &= ReadReg(0x8730, &reg);
        BitErrPeriod = (BitErrPeriod << 8) | reg;

        // bank 7 0x3a [7:0] reg_bit_err_num_7_0
        //             [15:8] reg_bit_err_num_15_8
        // bank 7 0x3c [7:0] reg_bit_err_num_23_16
        //             [15:8] reg_bit_err_num_31_24
        status &= ReadReg(0x873D, &reg);
        BitErr = reg;
        status &= ReadReg(0x873C, &reg);
        BitErr = (BitErr << 8) | reg;
        status &= ReadReg(0x873B, &reg);
        BitErr = (BitErr << 8) | reg;
        status &= ReadReg(0x873A, &reg);
        BitErr = (BitErr << 8) | reg;

        // bank 7 0x3e [7:0] reg_uncrt_pkt_num_7_0
        //             [15:8] reg_uncrt_pkt_num_15_8
        status &= ReadReg(0x873F, &reg);
        PktErr = reg;
        status &= ReadReg(0x873E, &reg);
        PktErr = (PktErr << 8) | reg;

        // bank 7 0x32 [7] reg_bit_err_num_freeze
        status &= WriteReg(0x8732, reg_frz);
        if (BitErrPeriod == 0)
        {
            //protect 0
            BitErrPeriod = 1;
        }

        if (BitErr <= 0)
        {
            fber = 0.5 / ((float)BitErrPeriod *128 * 188 * 8);
        }
        else
        {
            fber = (float)BitErr / ((float)BitErrPeriod *128 * 188 * 8);
        }
        DBG_DOMOD_MSB(printf("MSB122x PostVitBER = %8.3e \n ", fber));
        DBG_DOMOD_MSB(printf("MSB122x PktErr = %d \n ", (int)PktErr));

        if (status == FALSE)
        {
            DBG_DOMOD_MSB(printf("DTV_GetSignalQuality GetPostViterbiBer Fail!\n"));
            return 0;
        }

        log_ber = ( - 1) *Log10Approx(1 / fber); // Log10Approx() provide 1~2^32 input range only
        //DBG_GET_SIGNAL(printf("Log(BER) = %f\n",log_ber));
        if (log_ber <= ( - 7.0))        // PostVit BER < 1e-7
        {
            //*quality = 100;
            return 100;
        }
        else if (log_ber < ( - 3.7))        // PostVit BER < 2e-4
        {
            //*quality = 60 + (((-3.7) - log_ber) / ((-3.7)-(-7.0)) * (100-60));
            return (60+((( - 3.7) - log_ber) / (( - 3.7) - ( - 7.0))*(100-60)));
        }
        else if (log_ber < ( - 2.7))        // PostVit BER < 2e-3
        {
            //*quality = 10 + (((-2.7) - log_ber) / ((-2.7)-(-3.7)) * (60-10));
            return (10+((( - 2.7) - log_ber) / (( - 2.7) - ( - 3.7))*(60-10)));
        }
        else
        {
            //*quality = 10;
            return 10;
        }
    }
    else
    {
        //*quality = 0;
        return 0;
    }

    return 0;
}


U16 DTV_GetSignalStrength(void)
{
    MS_U8 status = true;
    MS_U16 u16Strength;
    MS_U8 reg_tmp, reg_frz;
    MS_U8 rf_agc_val;
    MS_U8 if_agc_val;
    MS_U8 if_agc_err;
    MS_U8 i;
    MS_U8 ssi_tbl_len;
    MS_U8 err_tbl_len;
    MS_U8 tps_cnstl;
    MS_U8 tps_cr;
    float ch_power_db;
    float ch_power_rf=0;
    float ch_power_if=0;
    float ch_power_ref=0;
    float ch_power_rel;
    S_IFAGC_SSI        *ifagc_ssi;
    S_IFAGC_ERR        *ifagc_err;

    DBG_DOMOD_MSB(printf("Get DVB-T signal strength\n"));

    if(FECLock == TRUE)
    {
        status &= ReadReg(0x8512+1, &reg_tmp);
        DBG_DOMOD_MSB(printf("AGC_REF = %d\n", (int)reg_tmp));
        if (reg_tmp > 200)
        {
            ifagc_ssi = IfagcSsi_HiRef;
            ssi_tbl_len = sizeof(IfagcSsi_HiRef)/sizeof(S_IFAGC_SSI);
            ifagc_err = IfagcErr_HiRef;
            err_tbl_len = sizeof(IfagcErr_HiRef)/sizeof(S_IFAGC_ERR);
        }
        else
        {
            ifagc_ssi = IfagcSsi_LoRef;
            ssi_tbl_len = sizeof(IfagcSsi_LoRef)/sizeof(S_IFAGC_SSI);
            ifagc_err = IfagcErr_LoRef;
            err_tbl_len = sizeof(IfagcErr_LoRef)/sizeof(S_IFAGC_ERR);
        }


        ///////// RF/IF-AGC Gain Out /////////////
        #if 0 // Sidney
            //rf_agc_val = MDrv_ReadByte(0x3A1C);
            rf_agc_val=MDrv_SAR_Adc_GetValue(2);
        #else
            rf_agc_val=0x3F;
        #endif
        WriteReg(0x8522, 0x03); // select IF gain to read
        WriteReg(0x8504+1, 0x03); // hold values of 0x8524 and 0x8525


        status &= ReadReg(0x8524+1, &reg_tmp);

        WriteReg(0x8504+1, 0x00); // release values of 0x8524 and 0x8525


        if_agc_val = reg_tmp;

        DBG_DOMOD_MSB(printf("SSI_RFAGC (SAR-2) = 0x%x\n", rf_agc_val));
        DBG_DOMOD_MSB(printf("SSI_IFAGC_H = 0x%x\n", if_agc_val));

        for(i = 0; i < sizeof(RfagcSsi)/sizeof(S_RFAGC_SSI); i++)
        {
            if (rf_agc_val <= RfagcSsi[i].sar3_val)
            {
                ch_power_rf = RfagcSsi[i].power_db;
                break;
            }
        }

        for(i = 0; i < ssi_tbl_len; i++)
        {
            if (if_agc_val <= ifagc_ssi[i].agc_val)
            {
                ch_power_if = ifagc_ssi[i].power_db;
                break;
            }
        }

        DBG_DOMOD_MSB(printf("ch_power_rf = %f\n", ch_power_rf));
        DBG_DOMOD_MSB(printf("ch_power_if = %f\n", ch_power_if));

        ch_power_db = (ch_power_rf > ch_power_if)? ch_power_rf : ch_power_if;

        ///////// IF-AGC Error for Add. Attnuation /////////////

        // bank 5 0x04 [15] reg_tdp_lat
        status &= ReadReg(0x8504+1, &reg_frz);
        status &= WriteReg(0x8504+1, reg_frz|0x80);

        // bank 5 0x2c [9:0] reg_agc_error
        status &= ReadReg(0x852C+1, &reg_tmp);
        if_agc_err = reg_tmp & 0x03;
        status &= ReadReg(0x852C, &reg_tmp);
        if_agc_err = (if_agc_err << 6)|(reg_tmp >> 2);

        status &= WriteReg(0x8504+1, reg_frz&(~0x80));

        for(i = 0; i < err_tbl_len; i++)
        {
            if ((MS_S8)if_agc_err <= ifagc_err[i].agc_err)        // signed char comparison
            {
                ch_power_db += ifagc_err[i].attn_db;
                break;
            }
        }

        DBG_DOMOD_MSB(printf("if_agc_err = 0x%x\n", if_agc_err));
    }
    else
    {
        ch_power_db = -99.0;
    }

    DBG_DOMOD_MSB(printf(">>> SSI_CH_PWR(dB) = %f <<<\n", ch_power_db));

    ///////// Get Constellation and Code Rate to determine Ref. Power //////////
    ///////// (refer to Teracom min. spec 2.0 4.1.1.6) /////

    status &= ReadReg(0x8624, &reg_tmp);
    tps_cnstl = reg_tmp & 0x03;

    status &= ReadReg(0x8624+1, &reg_tmp);
    tps_cr = (reg_tmp & 0x70) >> 4;

    for(i = 0; i < sizeof(SsiPref)/sizeof(S_SSI_PREF); i++)
    {
        if ( (tps_cnstl == SsiPref[i].constel)
            && (tps_cr == SsiPref[i].code_rate) )
        {
            ch_power_ref = SsiPref[i].p_ref;
            break;
        }

    }


    ch_power_rel = ch_power_db - ch_power_ref;

    if (ch_power_rel < -15.0)
        u16Strength = 0;
    else if (ch_power_rel < 0.0)
        u16Strength = (MS_U16)((ch_power_rel + 15.0)*(10.0/15.0));
    else if (ch_power_rel < 20.0)
        u16Strength = (MS_U16)(ch_power_rel*4.0 + 10.0);
    else if (ch_power_rel < 35.0)
        u16Strength = (MS_U16)((ch_power_rel - 20.0)*(10.0/15.0) + 90.0);
    else
        u16Strength = 100;


    DBG_DOMOD_MSB(printf(">>> SSI = %d <<<\n", (int)u16Strength));

    if (status == true)
        return u16Strength;
    else
        return 0;

}

BOOLEAN DTV_Serial_Control(BOOLEAN bEnable)
{
    MS_U8 status = true;
    gbSerialOut = bEnable;

    if (bEnable) //Serial mode for TS pad
    {
        //// MSB122x TS Control: Serial //////////
        gsCmdPacket.cmd_code = CMD_TS_CTRL;
        gsCmdPacket.param[0] = TS_SERIAL;

        #if (TS_CLK_INV == 1)
            gsCmdPacket.param[1] = 1; //TS_CLK_INVERSE;
        #else
            gsCmdPacket.param[1] = 0;
        #endif

        status &= MSB122x_Cmd_Packet_Send(&gsCmdPacket, 2);
    }
    else
    {
        //// MSB122x TS Control: Parallel //////////
        gsCmdPacket.cmd_code = CMD_TS_CTRL;
        gsCmdPacket.param[0] = TS_PARALLEL;

        #if (TS_CLK_INV == 1)
            gsCmdPacket.param[1] = 1; //TS_CLK_INVERSE;
        #else
            gsCmdPacket.param[1] = 0;
        #endif

        status &= MSB122x_Cmd_Packet_Send(&gsCmdPacket, 2);
    }

    //Driving_Control(MSB122x_DTV_DRIVING_LEVEL);
    Driving_Control(1);
    return status;

}

//##########################################################################################################
//##########################################################################################################
//########################################  Public:DTV-DVB-T Implementation ################################
//##########################################################################################################
//##########################################################################################################
BOOLEAN DTV_DVB_T_GetLockStatus(void)
{
    MS_U32 u32Timeout = 1500;
    MS_U8 u8Data;

    ReadReg(0x87E0, &u8Data);
    DBG_DOMOD_MSB(printf(">>>MSB122x: [%s] Lock Status = %d\n", __FUNCTION__, u8Data));

    if (u8Data == 0x0B)
    {
        FECLock = TRUE;
        //return E_DEMOD_LOCK;
        return TRUE;
    }
    else
    {
        FECLock = FALSE;
    }

    if (MsOS_GetSystemTime() - u32ChkScanTimeStart < u32Timeout)
    {
        //return E_DEMOD_CHECKING;
        return FALSE;
    }
    else
    {
        //return E_DEMOD_UNLOCK;
        return FALSE;
    }

}

//##########################################################################################################
//##########################################################################################################
//########################################  Private Function Implementation ################################
//##########################################################################################################
//##########################################################################################################

static BOOLEAN WriteReg(U16 u16Addr, U8 u8Data)
{
    u8MsbData[0] = DEMOD_ADDR_H;
    u8MsbData[1] = (u16Addr >> 8) &0xff;
    u8MsbData[2] = u16Addr &0xff;
    u8MsbData[3] = u8Data;


    return MDrv_IIC_WriteBytes(DEMOD_SLAVE_ID, 0, NULL, 4, u8MsbData);
}


/*@ </Operation ID=I2b28dd03m121c8cf959bmm6ff4> @*/
/*@ <Operation ID=I2b28dd03m121c8cf959bmm6fcf> @*/
static BOOLEAN ReadReg(U16 u16Addr, U8 *pu8Data)
{
     u8MsbData[0] = DEMOD_ADDR_H;
     u8MsbData[1] = (u16Addr >> 8) &0xff;
     u8MsbData[2] = u16Addr &0xff;
     //if (iptr->WriteBytes( 0, NULL, 3, u8MsbData) == MAPI_FALSE)
     if( MDrv_IIC_WriteBytes(DEMOD_SLAVE_ID, 0, NULL, 3, u8MsbData) == FALSE)
     {
        DBG_DOMOD_MSB(printf(">>>MSB122x:Read write fail\n"));
        return FALSE;
     }

     u8MsbData[0] = DEMOD_READ_REG;
     //return iptr->ReadBytes( 1, u8MsbData, 1, pu8Data);
     return MDrv_IIC_ReadBytes(DEMOD_SLAVE_ID, 1, u8MsbData, 1, pu8Data);

}



BOOLEAN Connect(EN_DEVICE_DEMOD_TYPE enDemodType)
{
    DBG_DOMOD_MSB(printf("%04d %s\n", __LINE__, __FUNCTION__));
    if (u8DeviceBusy == 1)
        return FALSE;
    m_enCurrentDemodulator_Type = enDemodType;
    u8DeviceBusy = 1;
    return TRUE;
}

/*@ </Operation ID=I2b28dd03m121c8cf959bmm722c> @*/
/*@ <Operation ID=I2b28dd03m121c8cf959bmm7207> @*/
BOOLEAN Disconnect(void)
{
    DBG_DOMOD_MSB(printf("%04d %s\n", __LINE__, __FUNCTION__));
    u8DeviceBusy = 0;
    return TRUE;
}

void Reset(void)
{

    WriteReg(0x8232, 0x01);
    WriteReg(0x8220, 0x01);
    MsOS_DelayTask(5);
    WriteReg(0x8220, 0x00);
    WriteReg(0x8232, 0x00);

}

/*@ </Operation ID=I2b28dd03m121c8cf959bmm71bd> @*/
BOOLEAN IIC_Bypass_Mode(BOOLEAN enable)
{
    U8 u8Retry=15;

    //DBG_DOMOD_MSB(printf("%04d %s set %x\n", __LINE__, __FUNCTION__, enable));

    while(u8Retry--)
    {
        if (enable)
            WriteReg(0x8010, 0x10);// IIC by-pass mode on
        else
            WriteReg(0x8010, 0x00);// IIC by-pass mode off

    }
    return TRUE;
}

/*@ <Operation ID=Im17018142m1221763cc7cmm46c6> @*/
BOOLEAN Power_On_Initialization(void)
{
    BOOLEAN bStatus = TRUE;

    FECLock = FALSE;
    DBG_DOMOD_MSB(printf(">>>MSB122x: Enter Power_On_Initialization()\n"));
    //// Firmware download //////////
    DBG_DOMOD_MSB(printf(">>>MSB122x: Load DSP...\n"));

    if (bPower_init_en == TRUE)//Stan FIXME.
        return TRUE;
    else
        bPower_init_en = TRUE;


    if (LoadDSPCode() == FALSE)
    {
        DBG_DOMOD_MSB(printf(">>>MSB122x:Fail\n"));
        return FALSE;
    }
    else
    {
        DBG_DOMOD_MSB(printf(">>>MSB122x:OK\n"));
    }

    //// MCU Reset //////////
    //DBG_DOMOD_MSB(printf(">>>MSB122x: Reset...\n"));
    //MSB122x_Reset();
    ReadReg(0x8000, &gu8ChipRevId);
    DBG_DOMOD_MSB(printf(">>>MSB122x:Gemini RevID:%x\n", gu8ChipRevId));

    #if 0 //init sar 2 for get strength // Sidney
    SAR_KpdRegCfg sarKpdRegCfgCh;
    MDrv_SAR_Kpd_Init();
    sarKpdRegCfgCh.u8SARChID = 2;
    sarKpdRegCfgCh.tSARChBnd.u8UpBnd = 0xFF;
    sarKpdRegCfgCh.tSARChBnd.u8LoBnd = 0;
    sarKpdRegCfgCh.u8KeyLevelNum = 0;
    //sarKpdRegCfgCh.u8KeyThreshold[0] = 0;
    //sarKpdRegCfgCh.u8KeyCode[0] = 0;
    if(MDrv_SAR_Kpd_SetChInfo(&sarKpdRegCfgCh)==E_SAR_KPD_FAIL)
    {
        printf("MDrv_SAR_Config: CH_3 fails\n");
        return FALSE;
    }
    #endif

    return bStatus;
}


BOOLEAN Active(BOOLEAN bEnable)
{
    MS_U8 u8Status = TRUE;
    DBG_DOMOD_MSB(printf("%04d %s \n", __LINE__, __FUNCTION__));

    //// MSB122x Finite State Machine on/off //////////
    gsCmdPacket.cmd_code = CMD_FSM_CTRL;
    gsCmdPacket.param[0] = (MS_U8)bEnable;
    u8Status &= MSB122x_Cmd_Packet_Send(&gsCmdPacket, 1);
    return u8Status;
}


/*@ </Operation ID=Im17018142m1221763cc7cmm464e> @*/
EN_DEVICE_DEMOD_TYPE GetCurrentDemodulatorType(void)
{
    return m_enCurrentDemodulator_Type;
}


/************************************************************************************************
  Subject:    enable hw to lock channel
  Function:   MSB1228_Active
  Parmeter:   bEnable
  Return:     BOOLEAN
  Remark:
*************************************************************************************************/

BOOLEAN MSB122x_Active(BOOLEAN bEnable)
{
    return Active(bEnable);
}

/***********************************************************************************
  Subject:    Command Packet Interface
  Function:   MSB1228_Cmd_Packet_Send
  Parmeter:
  Return:     BOOLEAN
  Remark:
************************************************************************************/
BOOLEAN MSB122x_Cmd_Packet_Send(S_CMDPKTREG* pCmdPacket, U8 param_cnt)
{
    U8 status = TRUE, indx;
    //MS_U8             reg_val, timeout = 0;
    U16 timeout = 0;
    U8 reg_val = 0;
    // ==== Command Phase ===================
    DBG_DOMOD_MSB(printf(">>>%s: (cmd=0x%x)(0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,) \n", __FUNCTION__, pCmdPacket->cmd_code, pCmdPacket->param[0], pCmdPacket->param[1], pCmdPacket->param[2], pCmdPacket->param[3], pCmdPacket->param[4], pCmdPacket->param[5]));
    // wait _BIT_END clear
    do
    {
        status &= ReadReg(REG_CMD_CTRL, &reg_val);
         if((reg_val & _BIT_END) != _BIT_END)
            break;
        //MDrv_Timer_Delayms(5);
        MsOS_DelayTask(5);
        //usleep(5 *1000);
        if (timeout++ > 200)
        {
            DBG_DOMOD_MSB(printf(">>>MSB122x: MSB122x_Cmd_Packet_Send fail on 'wait _BIT_END clear' \n"));
            return false;
        }
    }while (1);
    printf("BIT_END OK \n");
    // set cmd_3:0 and _BIT_START
    status &= ReadReg(REG_CMD_CTRL, &reg_val);
    reg_val = (reg_val &0x0f) | (pCmdPacket->cmd_code << 4) | _BIT_START;
    status &= WriteReg(REG_CMD_CTRL, reg_val);

    // wait _BIT_START clear
    do
    {
        status &= ReadReg(REG_CMD_CTRL, &reg_val);
         if((reg_val & _BIT_START) != _BIT_START)
            break;
		MsOS_DelayTask(5);
        //usleep(5 *1000);
        if (timeout++ > 200)
        {
            DBG_DOMOD_MSB(printf(">>>MSB123x: MSB123x_Cmd_Packet_Send fail on 'wait _BIT_START clear' \n"));
            return false;
        }
    }while (1);
    printf("BIT_START Clear OK \n");
    // ==== Data Phase ======================
    status &= WriteReg(REG_CMD_ADDR, 0x00);
    for (indx = 0; indx < param_cnt; indx++)
    {
        status &= ReadReg(REG_CMD_ADDR, &reg_val);
        DBG_DOMOD_MSB(printf("demod_config: param[%d] = %x\n", reg_val, pCmdPacket->param[indx]));
        // set param[indx] and _BIT_DRQ
        status &= WriteReg(REG_CMD_DATA, pCmdPacket->param[indx]);
        status &= ReadReg(REG_DTA_CTRL, &reg_val);
        status &= WriteReg(REG_DTA_CTRL, reg_val | _BIT_DRQ);

        // wait _BIT_DRQ clear
        do
        {
            status &= ReadReg(REG_DTA_CTRL, &reg_val);
             if ((reg_val & _BIT_DRQ) != _BIT_DRQ)
                break;
			MsOS_DelayTask(5);
            //usleep(5 *1000);
            if (timeout++ > 200)
            {
                DBG_DOMOD_MSB(printf(">>>MSB123x: MSB123x_Cmd_Packet_Send fail on 'wait _BIT_DRQ clear' \n"));
                return false;
            }
        }while (1);
    }

    // ==== End Phase =======================
    // set _BIT_END to finish command
    status &= ReadReg(REG_CMD_CTRL, &reg_val);
    status &= WriteReg(REG_CMD_CTRL, reg_val | _BIT_END);

    return status;
}

/***********************************************************************************
  Subject:    Power on initialized function
  Function:   MSB1228_Power_On_Initialization
  Parmeter:
  Return:     BOOLEAN
  Remark:
************************************************************************************/
BOOLEAN MSB122x_Power_On_Initialization(BOOLEAN bIsDVBT)
{
    return Power_On_Initialization();
}



BOOLEAN LoadDSPCode(void)
{

    //    MAPI_U32 i;
    MS_U32 u32TempCounter = 0;
    DBG_DOMOD_MSB(printf(">>>MSB122x: Enter LoadDSPCode()\n"));
    if (WriteReg(0x8232, 0x01) == FALSE)
    {
        // MCU SW reset
        return FALSE;
    }

    if (WriteReg(0x8230, 0x03) == FALSE)
    {
        // Enable read SRAM from I2CS
        return FALSE;
    }

    if (WriteReg(0x8210, 0x55) == FALSE)
        return FALSE;

    if (WriteReg(0x8211, 0xAA) == FALSE)
        return FALSE;

    u8MsbData[0] = DEMOD_RAM_CONTROL;
    u8MsbData[1] = 0x01;

    //mapi_i2c *iptr = mapi_i2c::GetI2C_Dev(DEMOD_DYNAMIC_SLAVE_ID_1);
    //if (iptr->WriteBytes( 0, NULL, 2, u8MsbData) == FALSE)
    if (MDrv_IIC_WriteBytes(DEMOD_SLAVE_ID, 0, NULL, 2, u8MsbData) == FALSE)
    {
        // Enable SRAM write
        return FALSE;
    }


    //iptr->SetSpeed(E_HWI2C_HIGH); // Sidney
    MDrv_IIC_SelectCLK(E_HWI2C_HIGH);
    // load table
    u32TempCounter = sizeof(MSB122x_table);
    DBG_DOMOD_MSB(printf(">>>MSB122x: Enter LoadDSPCode() u32TempCounter =0x%lx \n",u32TempCounter));
    //iptr->WriteBytes( 0, NULL, u32TempCounter, &MSB122x_table[0]);
    MDrv_IIC_WriteBytes(DEMOD_SLAVE_ID, 0, NULL, u32TempCounter, &MSB122x_table[0]);

    //iptr->SetScpeed(E_HWI2C_SLOW); // Sidney
    MDrv_IIC_SelectCLK(E_HWI2C_SLOW);

    u8MsbData[0] = DEMOD_RAM_CONTROL;
    u8MsbData[1] = 0x00;    //@FIXME , need to define I2C bus/slaveid reference

    //if (iptr->WriteBytes( 0, NULL, 2, u8MsbData) == FALSE)
    if (MDrv_IIC_WriteBytes(DEMOD_SLAVE_ID, 0, NULL, 2, u8MsbData) == FALSE)
    {
        // Disable Ram write
        return FALSE;
    }

    if (WriteReg(0x8230, 0x00) == FALSE)
    {
        // Disable download from I2CS
        return FALSE;
    }

    #if 0
        MS_U16 checknum = sizeof(MSB122x_table) - 3;
        MS_U8 *getValue = MSB122x_tableGet;
        MS_U32 i;
        for(i =0;i<sizeof(MSB122x_table);i++)
        {
            getValue[i] = 0xff;
        }

        if (getValue != NULL)
        {
            MS_BOOL bRet = TRUE;
            int i = 0;
            MsOS_DelayTask(100);
            if (WriteReg(0x8230, 0x02) == FALSE)// Enable read SRAM from I2CS
                bRet = FALSE;
            u8MsbData[0] = DEMOD_ADDR_H;
            u8MsbData[1] = 0;
            u8MsbData[2] = 0;
            //if (iptr->WriteBytes(0, 0, 3, u8MsbData) == FALSE)
            if (MDrv_IIC_WriteBytes(DEMOD_SLAVE_ID, 0, NULL, 3, u8MsbData) == FALSE)
            {
                bRet = FALSE;
            }

            MsOS_DelayTask(100);
            u8MsbData[0] = DEMOD_READ_REG;
            //while(1)
            {
                //iptr->SetSpeed(E_HWI2C_HIGH);
                //iptr->ReadBytes( 1, u8MsbData, checknum, getValue);
                MDrv_IIC_ReadBytes(DEMOD_SLAVE_ID, 1, u8MsbData, checknum, getValue);
                //usleep(2000 *1000);
                //iptr->SetSpeed(E_HWI2C_SLOW);
            }

            for (i = 0; i < checknum; i++)
            {
                if (MSB122x_table[i + 3] != getValue[i])
                {
                    printf("load msb dsp error : %d,%x,%x \n", i, MSB122x_table[i + 3], getValue[i]);

                    bRet = FALSE;
                }
            }
            printf("check over %d \n\n", checknum);
            if (WriteReg(0x8230, 0x0) == FALSE) // Disable read SRAM from I2CS
                bRet = FALSE;


            if (bRet == FALSE)
                return FALSE;
        }
        else
        {
            printf("sttest no mem to verify dsp \n\n");
        }
    #endif

    if (WriteReg(0x8232, 0x00) == FALSE) // MCU SW reset
        return FALSE;


    return TRUE;
}



BOOLEAN DTV_Set_Demod_TYPE(EN_DEVICE_DEMOD_TYPE enDemodType)
{
    m_enCurrentDemodulator_Type = enDemodType;
    return TRUE;
}


EN_DEVICE_DEMOD_TYPE DTV_Get_Demod_TYPE(void)
{
    return m_enCurrentDemodulator_Type;
}


void Driving_Control(BOOLEAN bEnable)
{
    MS_U8 u8Temp;
    ReadReg(0x8056, &u8Temp);
    if (bEnable)
    {
        u8Temp = u8Temp | 0x02;
    }
    else
    {
        u8Temp = u8Temp &(~0x02);
    }
    WriteReg(0x8056, u8Temp);

}

#endif

