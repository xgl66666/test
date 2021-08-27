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
/// @file MSB1228.c
/// @brief MSB1228 DVBT, VIF Controller Interface
/// @author MStar Semiconductor, Inc.
//
////////////////////////////////////////////////////////////////////////////////



#ifndef _MSB123x_C_
#define _MSB123x_C_

#include <sys/common/MsTypes.h>
#include <sys/common/MsIRQ.h>
#include <sys/common/MsOS.h>
#include <msAPI_Tuner.h>
#include <device/MSB123x.h>
//#include "drvIIC.h"



#define DBG_DOMOD_MSB(x)      x


static U8 u8MsbData[6];
static U32 u32ChkScanTimeStart = 0;
static BOOLEAN bPower_init_en = FALSE;

BOOLEAN FECLock;
EN_DEVICE_DEMOD_TYPE m_enCurrentDemodulator_Type = E_DEVICE_DEMOD_DVB_T2;
S_CMDPKTREG gsCmdPacket;
U8 u8DeviceBusy = 0;
U8 gu8ChipRevId;


BOOLEAN LoadDSPCode(void);
static BOOLEAN ReadReg(U16 u16Addr, U8 *pu8Data);
static BOOLEAN WriteReg(U16 u16Addr, U8 u8Data);
void Reset(void);
BOOLEAN WriteDspReg(U16 u16Addr, U8 u8Data);
BOOLEAN ReadDspReg(U16 u16Addr, U8* pData);
void Driving_Control(BOOLEAN bEnable);
BOOLEAN Active(BOOLEAN bEnable);

//##########################################################################################################
//##########################################################################################################
//########################################  Public:DTV Implementation ##################################
//##########################################################################################################
//##########################################################################################################
BOOLEAN DTV_SetFrequency(U32 u32Frequency, RF_CHANNEL_BANDWIDTH eBandWidth)
{
    DBG_DOMOD_MSB(printf("%04d %s %d ,%d\n", __LINE__, __FUNCTION__, (int)u32Frequency, (int)eBandWidth));

    //FRA_T2_BRINGUP
    FECLock = FALSE;
    u32ChkScanTimeStart = MsOS_GetSystemTime();

    Reset();

    MsOS_DelayTask(500);

    if(m_enCurrentDemodulator_Type==E_DEVICE_DEMOD_DVB_T2)
    {
        U8 bw = 4;
        switch (eBandWidth)
        {
            case E_RF_CH_BAND_6MHz:
                bw = 2;
                break;
            case E_RF_CH_BAND_7MHz:
                bw = 3;
                break;
            case E_RF_CH_BAND_8MHz:
                bw = 4;
                break;
            default:
                bw = 4;
                break;
        }
        WriteDspReg(0x40, bw);      // BW: 0->1.7M, 1->5M, 2->6M, 3->7M, 4->8M, 5->10M
        DTV_SetPlpGroupID(0,0);
        WriteReg(0x0C00+(0x0E)*2, 0x0001);  // FSM_EN
    }
    else if (m_enCurrentDemodulator_Type == E_DEVICE_DEMOD_DVB_T)
    {
        U8 bw = 3;
        switch (eBandWidth)
        {
            case E_RF_CH_BAND_6MHz:
            case E_RF_CH_BAND_7MHz:
            case E_RF_CH_BAND_8MHz:
                bw = eBandWidth;
                break;
            default:
                bw = E_RF_CH_BAND_8MHz;
                break;
        }
    	// Bandwidth: 0:5MHz, 1:6MHz, 2:7MHz, 3:8MHz
    	if( WriteDspReg(23 + 0x20, bw) == 0)//Driver update 2009/11/20
    	{
          DBG_DOMOD_MSB(printf("%04d %s Bandwidth setting NG\n", __LINE__, __FUNCTION__));
    	    return FALSE;
    	}
      Active(TRUE);
    }

    return TRUE;
}

#if 0
EN_FRONTEND_SIGNAL_CONDITION DTV_GetSNR(void)
{
    return E_FE_SIGNAL_NO;
}
#else
U32 DTV_GetSNR(void)
{
    return 1;
}
#endif

U32 DTV_GetBER(void)
{
    U8			status = TRUE;
    U8			reg, reg_frz;
    U16			BitErrPeriod;
    U32			BitErr;
    U16			PktErr;

    if(m_enCurrentDemodulator_Type==E_DEVICE_DEMOD_DVB_T2)
    {
        return 0;
    }
    else if(m_enCurrentDemodulator_Type==E_DEVICE_DEMOD_DVB_T)
    {
        /////////// Post-Viterbi BER /////////////

        // bank 7 0x32 [7] reg_bit_err_num_freeze
        status &= ReadReg(0x1132, &reg_frz);
        status &= WriteReg(0x1132, reg_frz|0x80);

        // bank 7 0x30 [7:0] reg_bit_err_sblprd_7_0
        //			   [15:8] reg_bit_err_sblprd_15_8
        status &= ReadReg(0x1131, &reg);
        BitErrPeriod = reg;

        status &= ReadReg(0x1130, &reg);
        BitErrPeriod = (BitErrPeriod << 8)|reg;

        // bank 7 0x3a [7:0] reg_bit_err_num_7_0
        //			   [15:8] reg_bit_err_num_15_8
        // bank 7 0x3c [7:0] reg_bit_err_num_23_16
        //			   [15:8] reg_bit_err_num_31_24

        status &= ReadReg(0x113D, &reg);
        BitErr = reg;

        status &= ReadReg(0x113C, &reg);
        BitErr = (BitErr << 8)|reg;

        status &= ReadReg(0x113B, &reg);
        BitErr = (BitErr << 8)|reg;

        status &= ReadReg(0x113A, &reg);
        BitErr = (BitErr << 8)|reg;

	#if 1
        // bank 7 0x3e [7:0] reg_uncrt_pkt_num_7_0
        //			   [15:8] reg_uncrt_pkt_num_15_8
        status &= ReadReg(0x113F, &reg);
        PktErr = reg;

        status &= ReadReg(0x113E, &reg);
        PktErr = (PktErr << 8)|reg;
	#endif

        // bank 7 0x32 [7] reg_bit_err_num_freeze
        status &= WriteReg(0x1132, reg_frz);

        if (BitErrPeriod == 0 )	//protect 0
            BitErrPeriod = 1;

    /*
		if (BitErr <=0 )
			*ber = 0.5 / ((float)BitErrPeriod*128*188*8);
		else
			*ber = (float)BitErr / ((float)BitErrPeriod*128*188*8);


	    printf("MSB PostVitBER = %8.3e \n ", *ber);
	    printf("MSB PktErr = %d \n ", (int)PktErr);
*/

        if(status)
        {
            printf(" @MSB1231_GetPostViterbiBer OK\n");
            return	TRUE;
        }
        else
        {
            printf(" @MSB1231_GetPostViterbiBer NG\n");
            return FALSE;
        }
	}
	return FALSE;
}

#if 0 //FIXME - 2009.11.08 by chunlun.lai
    /****************************************************************************
    Subject:    Function providing approx. result of Log10(X)
    Function:   Log10Approx
    Parmeter:   Operand X in float
    Return:     Approx. value of Log10(X) in float
    Remark:      Ouput range from 0.0, 0.3 to 9.6 (input 1 to 2^32)
     *******:**********************************************************************/
    static float Log10Approx(float flt_x)
    {
        U32 u32_temp = 1;
        U8 indx = 0;
        do
        {
            u32_temp = u32_temp << 1;
            if (flt_x < (float)u32_temp)
                break;
        }
        while (++indx < 32);
        // 10*log10(X) ~= 0.3*N, when X ~= 2^N
        return (float)0.3 *indx;
    }
#endif
#if 0
    /****************************************************************************
    Subject:    To get the Post viterbi BER
    Function:   MSB123x_GetPostViterbiBer
    Parmeter:  Quility
    Return:       E_RESULT_SUCCESS
    E_RESULT_FAILURE =>Read I2C fail, MSB123x_VIT_STATUS_NG
    Remark:     For the Performance issue, here we just return the Post Value.(Not BER)
    We will not read the Period, and have the "/256/8"
     *****************************************************************************/
    static MAPI_BOOL GetPostViterbiBer(float *ber)
    {
        MAPI_U8 status = true;
        MAPI_U8 reg, reg_frz;
        MAPI_U16 BitErrPeriod;
        MAPI_U32 BitErr;
        MAPI_U16 PktErr;
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
        //protect 0
            BitErrPeriod = 1;
        if (BitErr <= 0)
            *ber = 0.5 / ((float)BitErrPeriod *128 * 188 * 8);
        else
            *ber = (float)BitErr / ((float)BitErrPeriod *128 * 188 * 8);
        DBG_DOMOD_MSB(printf("MSB123x PostVitBER = %8.3e \n ",  *ber));
        DBG_DOMOD_MSB(printf("MSB123x PktErr = %d \n ", (int)PktErr));
        return status;
    }
#endif
U16 DTV_GetSignalQuality(void)
{
    if(FECLock==FALSE)
    {
        return 0;
    }
    //FRA_T2_BRINGUP
    return 80;

#if 0
    //return 80; //sttest
    float fber;
    float log_ber;
    MAPI_U8 u8Data;
    if (ReadReg(0x872C, &u8Data) == FALSE)
    {
        DBG_DOMOD_MSB(printf("DTV_GetSignalQuality ReadReg fail!!! \n"));
        return 0;
    }
    if (u8Data &0x02 /*BIT1*/)
    {
        MAPI_U8 status = true;
        MAPI_U8 reg, reg_frz;
        MAPI_U16 BitErrPeriod;
        MAPI_U32 BitErr;
        MAPI_U16 PktErr;
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
        //protect 0
            BitErrPeriod = 1;
        if (BitErr <= 0)
            fber = 0.5 / ((float)BitErrPeriod *128 * 188 * 8);
        else
            fber = (float)BitErr / ((float)BitErrPeriod *128 * 188 * 8);
        DBG_DOMOD_MSB(printf("MSB123x PostVitBER = %8.3e \n ", fber));
        DBG_DOMOD_MSB(printf("MSB123x PktErr = %d \n ", (int)PktErr));
        if (status == FALSE)
        {
            DBG_DOMOD_MSB(printf("DTV_GetSignalQuality GetPostViterbiBer Fail!\n"));
            return 0;
        }
        log_ber = ( - 1) *Log10Approx(1 / fber); // Log10Approx() provide 1~2^32 input range only
        //DBG_GET_SIGNAL(printf("Log(BER) = %f\n",log_ber));
        if (log_ber <= ( - 7.0))
        // PostVit BER < 1e-7
        {
            //*quality = 100;
            return 100;
        }
        else if (log_ber < ( - 3.7))
        // PostVit BER < 2e-4
        {
            //*quality = 60 + (((-3.7) - log_ber) / ((-3.7)-(-7.0)) * (100-60));
            return (60+((( - 3.7) - log_ber) / (( - 3.7) - ( - 7.0))*(100-60)));
        }
        else if (log_ber < ( - 2.7))
        // PostVit BER < 2e-3
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
#endif
}

U16 DTV_GetSignalStrength(void)
{
    if(FECLock==FALSE)
    {
        return 0;
    }

    return 80;
}

BOOLEAN DTV_Serial_Control(BOOLEAN bEnable)
{
    if(m_enCurrentDemodulator_Type==E_DEVICE_DEMOD_DVB_T2)
    {
    //FRA_T2_BRINGUP
    }
    else if(m_enCurrentDemodulator_Type==E_DEVICE_DEMOD_DVB_T)
    {
        // set serial/parallel mode
        if( WriteDspReg(0x4B, (U8)bEnable)!= TRUE)//Driver update 2009/11/20
        {
            printf(" @MSB1231_Serial_Control seting NG\n");
            return FALSE;
        }
        // set clok inverse
        if( WriteDspReg(0x4D, (U8)bEnable)!= TRUE)//Driver update 2009/11/20
        {
            printf(" @MSB1231_Serial_Control seting NG\n");
            return FALSE;
        }
    }
    Driving_Control(1);
    return TRUE;

#if 0
    MAPI_U8 status = true;
    m_bSerialOut = bEnable;
    //printf(" @msb123x_ts... ");
    if (bEnable)
    //Serial mode for TS pad
    {
        //// MSB123x TS Control: Serial //////////
        gsCmdPacket.cmd_code = CMD_TS_CTRL;
        gsCmdPacket.param[0] = TS_SERIAL;
        //#if(MSB123x_TS_SERIAL_INVERSION == 0)
        gsCmdPacket.param[1] = 1; //TS_CLK_NO_INV;
        //#else
        //        gsCmdPacket.param[1] = 1;//TS_CLK_INVERSE;
        //#endif
        status &= Cmd_Packet_Send(&gsCmdPacket, 2);
    }
    else
    {
        //// MSB123x TS Control: Parallel //////////
        gsCmdPacket.cmd_code = CMD_TS_CTRL;
        gsCmdPacket.param[0] = TS_PARALLEL;
        //#if(MSB123x_TS_PARALLEL_INVERSION == 0)
        gsCmdPacket.param[1] = 1; //TS_CLK_NO_INV;
        //#else
        //        gsCmdPacket.param[1] = 1;//TS_CLK_INVERSE;
        //#endif
        status &= Cmd_Packet_Send(&gsCmdPacket, 2);
    }
    //(printf("---> Inversion(Bit5) = 0x%x \n",gsCmdPacket.param[1] ));
    //Driving_Control(MSB123x_DTV_DRIVING_LEVEL);
    Driving_Control(1);
    return status;
#endif
}

//##########################################################################################################
//##########################################################################################################
//########################################  Public:DTV-DVB-T Implementation ################################
//##########################################################################################################
//##########################################################################################################
BOOLEAN DTV_DVB_T_GetLockStatus(void)
{
    U32 u32Timeout;
    U32 u32LockTimeout=1000;
    U8 u8Data;
    U16 u16RegAddress;
    U8 u8LockState;
    U8 u8BitMask;
    BOOLEAN bCheckPass=FALSE;
    static U32 u32LockTimeStartDVBT = 0;
    static U32 u32LockTimeStartDVBT2 = 0;
    U32 u32NowTime=MsOS_GetSystemTime();;

    //static MAPI_U32 u32FecLockTime;
    //static MAPI_U8 m_u8Data;
    //printf("sttest get lock state \n");
    switch(m_enCurrentDemodulator_Type)
    {
         case E_DEVICE_DEMOD_DVB_T2:
            u32Timeout=4000;
            u16RegAddress=0x0900+(0x48)*2;
            u8LockState=0x09;
            ReadReg(u16RegAddress, &u8Data);
            DBG_DOMOD_MSB(printf(">>>MSB123x: [%s] Lock Status = %d\n", __FUNCTION__, u8Data));
            if(u8Data == u8LockState)
            {
                bCheckPass=TRUE;
            }
            else
            {
                u16RegAddress =  0x00f0; //P1 Lock History
                u8BitMask = BIT5;
                ReadDspReg(u16RegAddress, &u8Data);
                if((u8Data&u8BitMask) == 0)
                {
                    u32Timeout=2000;
                }
            }

            DBG_DOMOD_MSB(printf(">>>MSB123x: [%s] Reg Status = %d\n", __FUNCTION__, u8Data));
            if (bCheckPass)
            {
                u32LockTimeStartDVBT2=MsOS_GetSystemTime();

                FECLock = TRUE;
                return TRUE;
            }
            else if ((u32NowTime - u32ChkScanTimeStart < u32Timeout)
                ||(u32NowTime - u32LockTimeStartDVBT2 < u32LockTimeout))
            {
                FECLock = FALSE;
                return FALSE;
            }
            else
            {
                FECLock = FALSE;
                return FALSE;
            }
            break;
         case E_DEVICE_DEMOD_DVB_T:
            //u16RegAddress=0x11E0;
            //u8LockState=0x0B;
            u32Timeout=2000;
            u16RegAddress=0x11E0;
            u8LockState=0x0B;
            ReadReg(u16RegAddress, &u8Data);
            if(u8Data==u8LockState)
            {
                bCheckPass=TRUE;
            }
            else
            {
                u16RegAddress =  0x0990; //TPS his Lock,
                u8BitMask = BIT3;
                ReadReg(u16RegAddress, &u8Data);
                if((u8Data&u8BitMask) > 0)
                    u32Timeout=6000;
            }

            #if 0
            if(FECLock) //check fec
            {
                u32Timeout=6000;
                u16RegAddress=0x11E0;
                u8LockState=0x0B;
                ReadReg(u16RegAddress, &u8Data);
                if(u8Data==u8LockState)
                    bCheckPass=TRUE;
            }
           else
            {
                u32Timeout=2000;
                //u16RegAddress =  0x0f22; //TPS Lock,
                //u8BitMask = BIT1;
                u16RegAddress =  0x0990; //TPS his Lock,
                u8BitMask = BIT3;
                ReadReg(u16RegAddress, &u8Data);
                if((u8Data&u8BitMask) > 0)
                    bCheckPass=TRUE;
            }
            #endif
            DBG_DOMOD_MSB(printf(">>>MSB123x: [%s] Lock Status = %d\n", __FUNCTION__, u8Data));
            if (bCheckPass)
            {
                u32LockTimeStartDVBT=MsOS_GetSystemTime();
                FECLock = TRUE;
                return TRUE;
            }
            else if ((u32NowTime - u32ChkScanTimeStart < u32Timeout)
            ||(u32NowTime - u32LockTimeStartDVBT < u32LockTimeout))
            {
                FECLock = FALSE;
                return FALSE;
            }
            else
            {

                FECLock = FALSE;
                return FALSE;
            }
            break;
         default:
            return FALSE;
    }

}

//##########################################################################################################
//##########################################################################################################
//########################################  Private Function Implementation ################################
//##########################################################################################################
//##########################################################################################################
BOOLEAN I2C_CH_Reset(U8 ch_num)
{
    //U8         addr[4] = {0x00, 0x00, 0x00, 0x00};
    U8         data[5] = {0x53, 0x45, 0x52, 0x44, 0x42};

    //mapi_i2c *iptr = mapi_i2c::GetI2C_Dev(DEMOD_DYNAMIC_SLAVE_ID_1);

    // 8'hb2(SRID)->8,h53(PWD1)->8,h45(PWD2)->8,h52(PWD3)->8,h44(PWD4)->8,h42(PWD5)
    data[0] = 0x53;
    //iptr->WriteBytes(0, NULL, 5, data);
    MDrv_IIC_WriteBytes(DEMOD_SLAVE_ID, 0, 0, 5, data);

    // 8'hb2(SRID)->8,h71(CMD)	//TV.n_iic_
    data[0] = 0x71;
    //iptr->WriteBytes(0, NULL, 1, data);
    MDrv_IIC_WriteBytes(DEMOD_SLAVE_ID, 0, 0, 1, data);

    // 8'hb2(SRID)->8,h81(CMD)	//TV.n_iic_sel_b0
    data[0] = (ch_num & 0x01)? 0x81 : 0x80;
    //iptr->WriteBytes(0, NULL, 1, data);
    MDrv_IIC_WriteBytes(DEMOD_SLAVE_ID, 0, 0, 1, data);

    // 8'hb2(SRID)->8,h83(CMD)	//TV.n_iic_sel_b1
    data[0] = (ch_num & 0x02)? 0x83 : 0x82;
    //iptr->WriteBytes(0, NULL, 1, data);
    MDrv_IIC_WriteBytes(DEMOD_SLAVE_ID, 0, 0, 1, data);

    // 8'hb2(SRID)->8,h84(CMD)	//TV.n_iic_sel_b2
    data[0] = (ch_num & 0x04)? 0x85 : 0x84;
    //iptr->WriteBytes(0, NULL, 1, data);
    MDrv_IIC_WriteBytes(DEMOD_SLAVE_ID, 0, 0, 1, data);

    // 8'hb2(SRID)->8,h53(CMD)	//TV.n_iic_ad_byte_en2, 32bit read/write
    data[0] = 0x53;
    //iptr->WriteBytes(0, NULL, 1, data);
    MDrv_IIC_WriteBytes(DEMOD_SLAVE_ID, 0, 0, 1, data);

    // 8'hb2(SRID)->8,h7f(CMD)	//TV.n_iic_sel_use_cfg
    data[0] = 0x7f;
    //iptr->WriteBytes(0, NULL, 1, data);
    MDrv_IIC_WriteBytes(DEMOD_SLAVE_ID, 0, 0, 1, data);

/*
    // 8'hb2(SRID)->8,h35(CMD)	//TV.n_iic_use
    data[0] = 0x35;
    iptr->WriteBytes(0, NULL, 1, data);

    // 8'hb2(SRID)->8,h71(CMD)	//TV.n_iic_Re-shape
    data[0] = 0x71;
    iptr->WriteBytes(0, NULL, 1, data);
*/

    return TRUE;
}

static BOOLEAN WriteReg(U16 u16Addr, U8 u8Data)
{
    u8MsbData[0] = 0x10;
    u8MsbData[1] = 0x00;
    u8MsbData[2] = 0x00;
    u8MsbData[3] = (u16Addr >> 8) &0xff;
    u8MsbData[4] = u16Addr &0xff;
    u8MsbData[5] = u8Data;

    u8MsbData[0] = 0x35;
    //iptr->WriteBytes(0, NULL, 1, u8MsbData);
    MDrv_IIC_WriteBytes(DEMOD_SLAVE_ID, 0, 0, 1, u8MsbData);

    u8MsbData[0] = 0x10;
    //iptr->WriteBytes(0, NULL, 6, u8MsbData);
    MDrv_IIC_WriteBytes(DEMOD_SLAVE_ID, 0, 0, 6, u8MsbData);

    u8MsbData[0] = 0x34;
    //return iptr->WriteBytes(0, NULL, 1, u8MsbData);
    return MDrv_IIC_WriteBytes(DEMOD_SLAVE_ID, 0, 0, 1, u8MsbData);
}


/*@ </Operation ID=I2b28dd03m121c8cf959bmm6ff4> @*/
/*@ <Operation ID=I2b28dd03m121c8cf959bmm6fcf> @*/
static BOOLEAN ReadReg(U16 u16Addr, U8 *pu8Data)
{
    u8MsbData[0] = 0x10;
    u8MsbData[1] = 0x00;
    u8MsbData[2] = 0x00;
    u8MsbData[3] = (u16Addr >> 8) &0xff;
    u8MsbData[4] = u16Addr &0xff;

    u8MsbData[0] = 0x35;
    //iptr->WriteBytes(0, NULL, 1, u8MsbData);
    MDrv_IIC_WriteBytes(DEMOD_SLAVE_ID, 0, 0, 1, u8MsbData);

    u8MsbData[0] = 0x10;
    //iptr->WriteBytes(0, NULL, 5, u8MsbData);
    MDrv_IIC_WriteBytes(DEMOD_SLAVE_ID, 0, 0, 5, u8MsbData);
    //iptr->ReadBytes(0, NULL, 1, pu8Data);
    MDrv_IIC_ReadBytes(DEMOD_SLAVE_ID, 0, 0, 1, pu8Data);

    u8MsbData[0] = 0x34;
    //return iptr->WriteBytes(0, NULL, 1, u8MsbData);
    return MDrv_IIC_WriteBytes(DEMOD_SLAVE_ID, 0, 0, 1, u8MsbData);
}

#define REG_MB_CNTL     0x0C00
#define REG_MB_ADDR_L   0x0C02
#define REG_MB_ADDR_H   0x0C03
#define REG_MB_DATA     0x0C04

BOOLEAN WriteDspReg(U16 u16Addr, U8 u8Data)
{
    MS_U8     u8Status = true;
    MS_U8     u8cntl = 0x00;
    MS_U16    u16cntr = 0x00;

    u8Status &= WriteReg(REG_MB_DATA, u8Data);
    u8Status &= WriteReg(REG_MB_ADDR_H, (U8)(u16Addr >> 8));
    u8Status &= WriteReg(REG_MB_ADDR_L, (U8)(u16Addr));
    u8Status &= WriteReg(REG_MB_CNTL, 0x04);
    if(!u8Status)
    {
        DBG_DOMOD_MSB(printf("%04d %s REG_MB_CNTL = %d\n", __LINE__, __FUNCTION__, u8Status));
    }


    do {
        MsOS_DelayTaskUs(1);
        u8Status &= ReadReg(REG_MB_CNTL, &u8cntl);
        if (u16cntr++ > 0x7ff)
        {
            DBG_DOMOD_MSB(printf("%04d %s MSB123X_MB_WRITE_FAILURE\n", __LINE__, __FUNCTION__));

            return FALSE;
        }
    }while (u8cntl != 0xff);

    return u8Status;
}

BOOLEAN ReadDspReg(U16 u16Addr, U8* pData)
{
    U8     status = TRUE;
    U8     cntl = 0x00;
    U16    cntr = 0x00;

    status &= WriteReg(REG_MB_ADDR_H, (U8)(u16Addr >> 8));
    status &= WriteReg(REG_MB_ADDR_L, (U8)(u16Addr));
    status &= WriteReg(REG_MB_CNTL, 0x03);

    do {
        status &= ReadReg(REG_MB_CNTL, &cntl);
        if (cntr++ > 0x7ff)
        {
            printf("MSB123X_MB_READ_FAILURE\n");
            return FALSE;
        }
    }while (cntl != 0xff);

    status &= ReadReg(REG_MB_DATA, pData);

    return status;
}

BOOLEAN Connect(EN_DEVICE_DEMOD_TYPE enDemodType)
{
    DBG_DOMOD_MSB(printf("device_demodulator_msb123x: Connect\n"));
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
    DBG_DOMOD_MSB(printf("device_demodulator_msb123x: Disconnect\n"));
    u8DeviceBusy = 0;
    return TRUE;
}

void Reset(void)
{
    U8     u8Data = 0x00;
    U32    u32Retry = 0x00;

    DBG_DOMOD_MSB(printf("%04d %s\n", __LINE__, __FUNCTION__));

    WriteReg(0x0B00+(0x19)*2, 0x03);
    WriteReg(0x0C00, 0x00); //clear MB_CNTL

    if(m_enCurrentDemodulator_Type==E_DEVICE_DEMOD_DVB_T2)
    {
        WriteReg(0x0B00+(0x10)*2, 0x03);
    }

    MsOS_DelayTask(5);

    if(m_enCurrentDemodulator_Type == E_DEVICE_DEMOD_DVB_T)
    {
		    WriteReg(0x0B00+(0x10)*2, 0x00);
    }

    WriteReg(0x0B00+(0x19)*2, 0x00);

    if(m_enCurrentDemodulator_Type == E_DEVICE_DEMOD_DVB_T2)
    {
        ReadReg(0x0C00, &u8Data);       //check MB_CNTL ready
        while(u8Data != 0xff)
        {
            MsOS_DelayTask(50);
            ReadReg(0x0C00, &u8Data);       //check MB_CNTL ready
             if (u32Retry++ > 200)
            {
                DBG_DOMOD_MSB(printf(">>>MSB123x: Reset Fail!\n"));
                break;
            }
        }
    }

    DBG_DOMOD_MSB(printf("%04d %s Reset OK\n", __LINE__, __FUNCTION__));

    FECLock = FALSE;
    u32ChkScanTimeStart = MsOS_GetSystemTime();
}
/*@ </Operation ID=I2b28dd03m121c8cf959bmm71bd> @*/
BOOLEAN IIC_Bypass_Mode(BOOLEAN enable)
{
    U8 u8Retry=10;

    DBG_DOMOD_MSB(printf(">>>MSB123x: IIC_bypass() set %x\n", enable));

//FRA_T2_BRINGUP
    while(u8Retry--)
    {
        if (enable)
            WriteReg(0x0900+(0x08)*2, 0x10);// IIC by-pass mode on
        else
            WriteReg(0x0900+(0x08)*2, 0x00);// IIC by-pass mode off
    }
    return TRUE;
}

/*@ <Operation ID=Im17018142m1221763cc7cmm46c6> @*/
BOOLEAN Power_On_Initialization(void)
{
    BOOLEAN bStatus = TRUE;

    U8 u8RetryCount = 1;
    DBG_DOMOD_MSB(printf("%04d %s\n", __LINE__, __FUNCTION__));

    //// Firmware download //////////
    DBG_DOMOD_MSB(printf("%04d %s Load DSP...\n", __LINE__, __FUNCTION__));

    //usleep(100 *1000);
    if (bPower_init_en == TRUE)
    {
        return TRUE;
    }
    else
    {
        bPower_init_en = TRUE;
    }


    do
    {
        if(bStatus == FALSE)
        {
            u8RetryCount++;
            bStatus=TRUE;
        }

        if (I2C_CH_Reset(0) == FALSE)
        {
            DBG_DOMOD_MSB(printf("%04d %s CH Reset:Fail\n", __LINE__, __FUNCTION__));

            bStatus= FALSE;
            continue;
        }
        else
        {
            DBG_DOMOD_MSB(printf("%04d %s CH Reset:OK\n", __LINE__, __FUNCTION__));
        }



        if (I2C_CH_Reset(3) == FALSE)
        {

            DBG_DOMOD_MSB(printf("%04d %s MSB123x CH Reset:Fail\n", __LINE__, __FUNCTION__));
            bStatus= FALSE;
            continue;
        }
        else
        {

            DBG_DOMOD_MSB(printf("%04d %s MSB123x CH Reset OK\n", __LINE__, __FUNCTION__));
        }



        if (LoadDSPCode() == FALSE)
        {
            DBG_DOMOD_MSB(printf("%04d %s LoadDSPCode FAIL\n", __LINE__, __FUNCTION__));
            bStatus= FALSE;
            continue;
        }
        else
        {
            DBG_DOMOD_MSB(printf("%04d %s LoadDSPCode OK\n", __LINE__, __FUNCTION__));
        }

        ReadReg(0x0900+(0x01)*2, &gu8ChipRevId);
        DBG_DOMOD_MSB(printf(">>>MSB123x:Wimbledon RevID:%x\n", gu8ChipRevId));

        ReadReg(0x0900+(0x49)*2, &gu8ChipRevId);
        DBG_DOMOD_MSB(printf(">>>MSB123x:Wimbledon 0x49_L:%x\n", gu8ChipRevId));

        ReadReg(0x0900+(0x49)*2+1, &gu8ChipRevId);
        DBG_DOMOD_MSB(printf(">>>MSB123x:Wimbledon 0x49_H:%x\n", gu8ChipRevId));

        ReadReg(0x0900+(0x4A)*2, &gu8ChipRevId);
        DBG_DOMOD_MSB(printf(">>>MSB123x:Wimbledon 0x4A_L:%x\n", gu8ChipRevId));



    }while( (u8RetryCount < 7)&&(bStatus==FALSE));

    if(bStatus==FALSE)
    {
        DBG_DOMOD_MSB(printf("%04d %s FAIL\n", __LINE__, __FUNCTION__));
        return FALSE;
    }
    else
    {
        DBG_DOMOD_MSB(printf("%04d %s OK\n", __LINE__, __FUNCTION__));
    }


    return bStatus;
}

/*@ </Operation ID=Im17018142m1221763cc7cmm46c6> @*/
BOOLEAN Set_PowerOn(void)
{
    return TRUE;
}

BOOLEAN Set_PowerOff(void)
{
    return TRUE;
}

//operation
#define RFAGC_EN    	0x00
#define HUMDET_EN   	0x00
#define DCR_EN      	0x01
#define IIS_EN      	0x01
#define CCI_EN      	0x01
#define ACI_EN      	0x01
#define IQB_EN      	0x00
#define AUTO_IQ_SWAP	0x01
#define AUTO_RFMAX  	0x00
#define AUTO_ACI    	0x00
#define MODE_CP_FORCED  0x00
#define TPS_FORCED      0x00
#define AUTO_SCAN       0x00
#define RSV_0D          0x00
#define RSV_0E          0x00
#define RSV_0F   		0x00

//configure
#define RSSI            0x00
#define ZIF             0x00
#define FREQ            0x00

#if( ENABLE_MODULE_TUNER_NUTUNE_FJ2207 == 1 )

#define FC_L            0xEC // FS - IF_LOW = 48000 - 4500 = 43500
#define FC_H            0xA9
#define FS_L            0x80 // 48000
#define FS_H            0xBB
#define IQ_SWAP         0x00

#else
#define FC_L            0xE0
#define FC_H            0x2E
#define FS_L            0x80
#define FS_H            0xBB
#define IQ_SWAP         0x00

#endif

#define BW              0x03
#define MODE            0x01
#define CP              0x03
#define LP_SEL          0x00
#define CSTL            0x00
#define HIER            0x00
#define HPCR            0x00
#define LPCR            0x00
#define RFMAX           0x01
#define ATV_SYSTEM      0x01
#define ICFO_RANGE      0x01
#define RFAGC_REF       		0x64
#define IFAGC_REF_2K       		0x4B
#define IFAGC_REF_8K       		0x4B
#define IFAGC_REF_ACI      		0x4B
#define IFAGC_REF_IIS_2K   		0xA0
#define IFAGC_REF_IIS_8K   		0xA0
#define IFAGC_ACI_DET_TH_L 		0x9A
#define IFAGC_ACI_DET_TH_H 		0x01
#define SERIAL_TS          		0x00
#define TS_SERIAL_CLK      		0x00
#define TS_OUT_INV         		0x01
#define TS_DATA_SWAP       		0x00
#define SFO_2K_H           		0x00
#define SFO_2K_L           		0xC8
#define SFO_8K_H           		0x00
#define SFO_8K_L           		0xC8
#define CHECK_CHANNEL      		0x00
#define SNR_POS            		0x00
#define CCI_KP             		0x00
#define CCI_FSWEEP         		0x00

U8 MSB1231_DVBT_DSPREG_TABLE[] = // andy 2009-12-15 ¿ÀÈÄ 3:55:03 TW model
{
    RFAGC_EN,     HUMDET_EN,  	DCR_EN,     IIS_EN,         CCI_EN,      ACI_EN,
    IQB_EN,       AUTO_IQ_SWAP, AUTO_RFMAX, AUTO_ACI, 		MODE_CP_FORCED, TPS_FORCED,
    AUTO_SCAN,    RSV_0D,		RSV_0E,		RSV_0F,
    RSSI,		  ZIF,          FREQ,       FC_L,			FC_H,        FS_L,		FS_H,
    BW,           MODE,		    CP,       	LP_SEL,        	CSTL,        HIER,      HPCR,
    LPCR,         IQ_SWAP,      RFMAX,    	ATV_SYSTEM,		ICFO_RANGE,	 RFAGC_REF, RFAGC_REF,
    IFAGC_REF_2K, IFAGC_REF_8K, IFAGC_REF_ACI,  IFAGC_REF_IIS_2K,         IFAGC_ACI_DET_TH_L,
    IFAGC_ACI_DET_TH_H,       	SERIAL_TS,	TS_SERIAL_CLK,  TS_OUT_INV,  TS_DATA_SWAP,
    SFO_2K_H,	  SFO_2K_L,   	SFO_8K_H, 	SFO_8K_L,		CHECK_CHANNEL, 			SNR_POS,
    CCI_KP, 	  CCI_FSWEEP
};

BOOLEAN  DTV_DVBT_DSPReg_Init(void)
{
    U8    idx = 0;
    for (idx = 0; idx<sizeof( MSB1231_DVBT_DSPREG_TABLE); idx++)
    {
        if( WriteDspReg(idx + 0x20, MSB1231_DVBT_DSPREG_TABLE[idx])!=TRUE)
        {
            printf("dsp reg init NG\n");
            return FALSE;
        }
    }
    printf("dsp reg init ok\n");

    return TRUE;
}

//configure

// BW: 0->1.7M, 1->5M, 2->6M, 3->7M, 4->8M, 5->10M
#define T2_BW           0x04

// FC: FC = FS - IF = 48000 - 4500 = 0xA9EC     (4.50MHz IF)
// FC: FC = FS - IF = 48000 - 36000 = 0x2EE0    (36.0MHz IF)
#if( ENABLE_MODULE_TUNER_NUTUNE_FJ2207 == 1 )
#define T2_FC_L         0xEC
#define T2_FC_H         0xA9
#else
#define T2_FC_L         0xE0
#define T2_FC_H         0x2E
#endif

U8 MSB1231_DVBT2_DSPREG_TABLE[] =
{
    T2_BW,     T2_FC_L,  	T2_FC_H
};

BOOLEAN DTV_DVBT2_DSPReg_Init(void)
{
    U8    idx = 0;
    for (idx = 0; idx<sizeof( MSB1231_DVBT2_DSPREG_TABLE); idx++)
    {
        if( WriteDspReg(idx + 0x40, MSB1231_DVBT2_DSPREG_TABLE[idx])!=TRUE)
        {
            printf("T2 dsp reg init NG\n");
            return FALSE;
        }
    }
    printf("T2 dsp reg init ok\n");

    return TRUE;
}

BOOLEAN Active(BOOLEAN bEnable)
{
    MS_U8 u8Status = TRUE;
    DBG_DOMOD_MSB(printf("%04d %s \n", __LINE__, __FUNCTION__));

    if(m_enCurrentDemodulator_Type == E_DEVICE_DEMOD_DVB_T2)
    {
        return TRUE;
    }

    u8Status = WriteReg(0x0C00+(0x0E)*2, 0x0001);  // FSM_EN

	if(u8Status)
	{
        DBG_DOMOD_MSB(printf("%04d %s @MSB1231_active OK\n", __LINE__, __FUNCTION__));

		return	TRUE;
	}
	else
	{
        DBG_DOMOD_MSB(printf("%04d %s @MSB1231_active NG\n", __LINE__, __FUNCTION__));

		return FALSE;
	}

	return FALSE;
}


/***********************************************************************************
  Subject:    Write register
  Function:   MSB123x_WriteReg
  Parmeter:   u16Addr:   Register address
              u8Data:    Data to write
  Return:     BOOLEAN
  Remark:
************************************************************************************/
BOOLEAN MSB123x_WriteReg(U16 u16Addr, U8 u8Data)
{

    return WriteReg(u16Addr,u8Data);

}
/***********************************************************************************
  Subject:    read register
  Function:   MSB123x_ReadReg
  Parmeter:   u16Addr:   Register address
              pu8Data:   pointer for return data
  Return:     BOOLEAN
  Remark:
************************************************************************************/
BOOLEAN MSB123x_ReadReg(U16 u16Addr, U8 *pu8Data)
{

    return ReadReg(u16Addr, pu8Data);

}

/***********************************************************************************
  Subject:    Reset
  Function:   MSB1228_Reset
  Parmeter:
  Return:     BOOLEAN
  Remark:
************************************************************************************/
BOOLEAN MSB123x_Reset ( void )
{
    Reset();
    return TRUE;
}

/************************************************************************************************
  Subject:    enable hw to lock channel
  Function:   MSB1228_Active
  Parmeter:   bEnable
  Return:     BOOLEAN
  Remark:
*************************************************************************************************/

BOOLEAN MSB123x_Active(BOOLEAN bEnable)
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
BOOLEAN MSB123x_Cmd_Packet_Send(S_CMDPKTREG* pCmdPacket, U8 param_cnt)
{
    U8 status = TRUE, indx;
    //MAPI_U8             reg_val, timeout = 0;
    U16 timeout = 0;
    U8 reg_val = 0;
    // ==== Command Phase ===================
    DBG_DOMOD_MSB(printf(">>>MSB123x: (cmd=0x%x)(0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,) \n", pCmdPacket->cmd_code, pCmdPacket->param[0], pCmdPacket->param[1], pCmdPacket->param[2], pCmdPacket->param[3], pCmdPacket->param[4], pCmdPacket->param[5]));
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
            DBG_DOMOD_MSB(printf(">>>MSB123x: MSB123x_Cmd_Packet_Send fail on 'wait _BIT_END clear' \n"));
            return FALSE;
        }
    }while (1);

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
            return FALSE;
        }
    }while (1);

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
                return FALSE;
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
BOOLEAN MSB123x_Power_On_Initialization(BOOLEAN bIsDVBT)
{
    return Power_On_Initialization();
}

void MSB123x_AFC_Routine(DWORD dwFrequency, RF_CHANNEL_BANDWIDTH eBandWidth)
{

}





BOOLEAN LoadDSPCode(void)
{
    U32        u32Timeout = 0;
    U8         u8DoneFlag = 0;
    U8         u8Data;
    switch(m_enCurrentDemodulator_Type)
    {
        case E_DEVICE_DEMOD_DVB_T2:
            u8Data=1;
            break;
        case E_DEVICE_DEMOD_DVB_T:
            u8Data=2;
            break;
        default:
            return FALSE;
    }
    // Disable MCU
    WriteReg(0x0b00+(0x19)*2, 0x03);

    // Run code on bootloader
    WriteReg(0x1000+(0x0c)*2, 0x02);

    // Assign f/w code type to load => 0: boot-loader 1: dvbt2, 2: dvbt, 3: dvbc
    WriteReg(0x0900+(0x4f)*2, u8Data);

    // Enable MCU
    WriteReg(0x0b00+(0x19)*2, 0x00);

    do
    {
        ReadReg(0x0900+(0x4f)*2, &u8DoneFlag);

        if (u32Timeout++ > 500)
            return FALSE;
		MsOS_DelayTask(1);
        //usleep(1*1000);

    } while(u8DoneFlag != 0xaa);
    // Disable MCU
    WriteReg(0x0b00+(0x19)*2, 0x03);

    // Run code on loaded firmware
    WriteReg(0x1000+(0x0c)*2, 0x05);

    do
    {
        ReadReg(0x1000+(0x0c)*2, &u8DoneFlag);

        if (u32Timeout++ > 500)
            return FALSE;
		MsOS_DelayTask(1);
        //usleep(1*1000);

    } while(u8DoneFlag != 0x05);
    // Enable MCU
    WriteReg(0x0b00+(0x19)*2, 0x00);
    switch(m_enCurrentDemodulator_Type)
    {
        case E_DEVICE_DEMOD_DVB_T2:
            DTV_DVBT2_DSPReg_Init();
            break;
        case E_DEVICE_DEMOD_DVB_T:
            DTV_DVBT_DSPReg_Init();
            break;
        default:
            return FALSE;
    }
    return TRUE;
}



/************************************************************************************************
Subject:    Set demodulator type
Function:   SetCurrentDemodulatorType
Parmeter:   enDemodType
Return:     BOOLEAN :
 *************************************************************************************************/
BOOLEAN SetCurrentDemodulatorType(EN_DEVICE_DEMOD_TYPE enDemodType)
{
    DBG_DOMOD_MSB(printf("device_demodulator_msb123x: SetCurrentDemodulatorType %d\n", (int)enDemodType));
    u32ChkScanTimeStart = MsOS_GetSystemTime();
    switch(enDemodType)
    {
        case E_DEVICE_DEMOD_DVB_T2:
            m_enCurrentDemodulator_Type = enDemodType;
            break;
        case E_DEVICE_DEMOD_DVB_T:
            m_enCurrentDemodulator_Type = enDemodType;
            break;
        default:
            return FALSE;
    }

    if (LoadDSPCode() == FALSE)
    {
        printf(">>>>MSB123x:Fail\n");
        return FALSE;
    }
    else
    {
        printf(">>>MSB123x:OK\n");
    }

    return TRUE;
}


/************************************************************************************************
Subject:    Get PLP-ID Bit Map
Function:   DTV_GetPlpBitMap
Parmeter:   u32PlpBitMap
Return:     BOOLEAN :
Remark:     32 byte of data to show PLP-ID existense
            |00000000|00000000|...|00000000|00011111|11111111|
             byte31   byte30   ... byte2    byte1    byte0   => 256 bit in total
 *************************************************************************************************/
BOOLEAN DTV_GetPlpBitMap(U8* u8PlpBitMap)
{
    BOOLEAN   status = TRUE;
    U8     u8Data = 0;
    U8     indx = 0;

    status &= ReadDspReg(0x0120, &u8Data);     // check L1 ready
    if (u8Data != 0x30)
    {
        return FALSE;
    }

    while(indx < 32)
    {

        status &= ReadDspReg(0x0100+indx, &u8Data);
        u8PlpBitMap[indx] = u8Data;
        indx++;
    }

    return status;
}

/************************************************************************************************
Subject:    Get GROUP_ID upon PLP_ID for MPLP mode
Function:   DTV_GetPlpGroupID
Parmeter:   u8PlpID, u8GroupID
Return:     BOOLEAN :
Remark:
 *************************************************************************************************/
BOOLEAN DTV_GetPlpGroupID(U8 u8PlpID, U8* u8GroupID)
{
    BOOLEAN   status = TRUE;
    U8 u8Data;
    status &= ReadDspReg(0x0120, &u8Data);         // check L1 ready
    if (u8Data != 0x30)
    {
        return FALSE;
    }

    ReadReg(0x0900+(0x48)*2, &u8Data);

    if (u8Data != 0x09)
    {
        return FALSE;
    }
    // assign PLP-ID value
    status &= WriteReg(0x2700+(0x78)*2, u8PlpID);
    status &= WriteReg(0x2700+(0x01)*2 + 1, 0x01);  // MEM_EN
	MsOS_DelayTask(1);
    //usleep(10);
    status &= ReadReg(0x2700+(0x79)*2, u8GroupID);
    status &= WriteReg(0x2700+(0x01)*2 + 1, 0x00);  // ~MEM_EN

    return status;
}

/************************************************************************************************
Subject:    Select PLP_ID, GROUP_ID for MPLP mode
Function:   DTV_SetPlpGroupID
Parmeter:   u8PlpID, u8GroupID
Return:     BOOLEAN :
Remark:
 *************************************************************************************************/
BOOLEAN DTV_SetPlpGroupID(U8 u8PlpID, U8 u8GroupID)
{
    printf("sttest DTV_SetPlpGroupID %d,%d \n",u8PlpID,u8GroupID);
    BOOLEAN   status = TRUE;
    FECLock = FALSE;
    u32ChkScanTimeStart = MsOS_GetSystemTime();


    // assign Group-ID and PLP-ID value (must be written in order)
    status &= WriteDspReg(0x0122, u8GroupID);
    status &= WriteDspReg(0x0121, u8PlpID);

    return status;
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
    ReadReg(0x0958, &u8Temp);
    if (bEnable)
    {
        u8Temp = 0xFF;
    }
    else
    {
        u8Temp = 0x00;
    }
    WriteReg(0x0958, u8Temp);

    ReadReg(0x0959, &u8Temp);
    if (bEnable)
    {
        u8Temp = u8Temp | 0x0F;
    }
    else
    {
        u8Temp = u8Temp &(~0x0F);
    }
    WriteReg(0x0959, u8Temp);
}


U16 DTV_GetCellID(void)
{
    return 0;
}

#endif
