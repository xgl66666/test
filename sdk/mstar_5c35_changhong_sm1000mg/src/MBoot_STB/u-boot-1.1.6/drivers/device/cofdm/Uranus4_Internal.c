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



#ifndef _DIB8096_C_
#define _DIB8096_C_

#include <msAPI_Tuner.h>
#include <sys/drv/drvDMD.h>
#include <device/Uranus4_Internal.h>

extern MS_U32 MsOS_GetSystemTime (void);
static double log10(double flt_x);

#define DBG_U4_DMD(format, args...) printf("%04d %s " format, __LINE__, __FUNCTION__, ##args)
#define     DEMOD_DYNAMIC_SLAVE_ID_1          0x32

#define X4CFE_ENABLE_MODE       TRUE
#define PPD_ENABLE_MODE         FALSE
#define IQ_AUTO_ENABLE_MODE     TRUE
#define QAM_AUTO_ENABLE_MODE    FALSE
#define FHO_ENABLE_MODE         FALSE
#define DMD_TUNER_FREQ          FALSE


typedef enum _DVBC_QamType1
{
    E_DVBC_QAM16  = 0,                                                  ///< QAM 16
    E_DVBC_QAM32  = 1,                                                  ///< QAM 32
    E_DVBC_QAM64  = 2,                                                  ///< QAM 64
    E_DVBC_QAM128 = 3,                                                  ///< QAM 128
    E_DVBC_QAM256 = 4,                                                  ///< QAM 256

    E_DVBC_QAM_LASTNUM,
} __attribute__((packed)) DVBC_QAMType1;

//--------------------------
typedef enum _DVBC_IQType1
{
    E_DVBC_IQ_NORMAL = 0,                                               ///< IQ Normal
    E_DVBC_IQ_INVERT = 1,                                               ///< IQ Invert
} __attribute__((packed)) DVBC_IQType1;

//--------------------------
typedef enum _DVBC_TapAssign1
{
    E_DVBC_TAP_PRE  = 0,                                                ///< PreTap assign
    E_DVBC_TAP_POST = 1,                                                ///< PostTap assign
} __attribute__((packed)) DVBC_TapAssign1;

//EN_DEVICE_DEMOD_TYPE m_enCurrentDemodulator_Type = E_DEVICE_DEMOD_DVB_T;
BOOLEAN bIsSerialOut = FALSE, bIsDeviceBusy = FALSE, bIsPowerOnInitialized = FALSE;

static MS_U8 _u8dmdConfig[] =
{
    0x01, 0x22, 0x8d, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x73, 0x00, 0xb3, 0x33, 0x03, 0x05, 0x52, 0x52, 0x73, 0x73, 0x73, 0x31, 0x52, 0x73, 0x73, 0x73,
    0x72, 0xc0, 0x24, 0x00, 0x80, 0xff, 0x7f, 0x00, 0x80, 0xff, 0x7f, 0x0e, 0x2b, 0x0b, 0x47, 0x40,
    0xd3, 0x9a, 0xde, 0x11, 0xa4, 0x60, 0x08, 0x08, 0x00, 0x10, 0x02, 0x0b, 0x03, 0x05, 0x03, 0x82,
    0x52, 0x52, 0x55,
};//8D1D=36125, 8D47=36167

static double log10(double flt_x)
{
	MS_U32		u32_temp = 1;
	MS_U8		indx = 0;

	do {
		u32_temp = u32_temp << 1;
		if (flt_x < (double)u32_temp)
			break;
	}while (++indx < 32);

	// 10*log10(X) ~= 0.3*N, when X ~= 2^N
	return (double)0.3 * indx;
}

static BOOLEAN WriteReg(U16 u16Addr, U8 u8Data)
{
    U8 u8MsbData[6] = {0};
    u8MsbData[0] = 0;
    u8MsbData[1] = (u16Addr >> 8) & 0xff;
    u8MsbData[2] = u16Addr & 0xff;
    u8MsbData[3] = u8Data;
    return MDrv_IIC_WriteBytes(DEMOD_DYNAMIC_SLAVE_ID_1, 0, 0, 4, u8MsbData);
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
    DBG_U4_DMD("\n");

    return TRUE;
}


//##########################################################################################################
//##########################################################################################################
//########################################  Public:DTV Implementation ##################################
//##########################################################################################################
//##########################################################################################################

BOOLEAN DTV_SetFrequency(U32 u32Frequency, RF_CHANNEL_BANDWIDTH eBandWidth)
{
	DMD_Param dmdParamBox;
    MS_U32 u32Timeout;

    dmdParamBox.u32SymRate 		= (long)6875; 			// SampleRate 6875 3600-7000
	dmdParamBox.eQamType 		= E_DVBC_QAM64;			// QAM64=2
    dmdParamBox.eIQSwap 		= E_DVBC_IQ_NORMAL;		// 0-> not swap,
    dmdParamBox.eTapAssign 		= E_DVBC_TAP_POST;		// tap_assign: 0->(64+32), 1->(32+64)
    dmdParamBox.u32FreqOffset  	= 0;
    dmdParamBox.u8TuneFreqOffset= 0;
    dmdParamBox.u32TunerFreq 	= u32Frequency;				// TuneFreqOffset: 0:0x, 1:+1x, 2:-1x delta

    u32Timeout = MsOS_GetSystemTime();
    while (MDrv_Dmd_Restart(&dmdParamBox) != TRUE)
    {
        if ((MsOS_GetSystemTime() - u32Timeout) >= 5000)
        {
            DBG_U4_DMD("demodulator restart fail.\n");
            return FALSE;
        }
    }

    return TRUE;

}


MS_U32 DTV_GetSNR(void)
{

    MS_U32 u32SNR = 0;
    if(MDrv_Dmd_GetSNR(&u32SNR))
    {
        return u32SNR;
    }
    else
    {
        DBG_U4_DMD("MDrv_Dmd_GetSNR = 0");
        return 0;
    }
}


U32 DTV_GetBER(void)
{
    DBG_U4_DMD("\n");
    U32 u32BER = 0;

    if(MDrv_Dmd_GetBER((float*)&u32BER))
    {
        return u32BER;
    }
    else
    {
        DBG_U4_DMD("MDrv_Dmd_GetBER = 0\n");
        return 0;
    }
}




MS_U16 DTV_GetSignalQuality(void)
{

    MS_U16 u16Signal = 0;
    float 		fBER;
    float 		fLogBER;
    MS_BOOL bLock = FALSE;

    MDrv_Dmd_GetLock(&bLock);
    if (bLock)
    {
        if (MDrv_Dmd_GetBER(&fBER) == FALSE)
        {
            DBG_U4_DMD("MDrv_Dmd_GetBER = 0\n");
            return 0;
        }

        fLogBER = (-1)*log10(1/fBER);	// Log10Approx() provide 1~2^32 input range only

        DBG_U4_DMD("Log(BER) = %f\n",fLogBER);

        if ( fLogBER <= (-7.0) )			// PostVit BER < 1e-7
        {
            u16Signal = 100;
        }
        else  if ( fLogBER < (-3.7) )		// PostVit BER < 2e-4
        {
            u16Signal = 60 + (((-3.7) - fLogBER) / ((-3.7)-(-7.0)) * (100-60));
        }
        else  if ( fLogBER < (-2.7) )		// PostVit BER < 2e-3
        {
            u16Signal = 10 + (((-2.7) - fLogBER) / ((-2.7)-(-3.7)) * (60-10));
        }
        else
        {
            u16Signal = 10;
        }

    }
    else
    {
        u16Signal = 0;
    }

    DBG_U4_DMD("Signal Quility = %d\n", (int)u16Signal);

    return u16Signal;
}


MS_U16 DTV_GetSignalStrength(void)
{
#define DEFAULT_PWR_MIN (-97)
#define DEFAULT_PWR_MAX (-5)
    MS_S32 s32PWR = 0;
    MS_U16 u16Precent = 0;
    if(MDrv_Dmd_GetPWR(&s32PWR))
    {
        DBG_U4_DMD("s32PWR = %d\n", s32PWR);

        if(s32PWR < DEFAULT_PWR_MIN || s32PWR > DEFAULT_PWR_MAX)
        {
            u16Precent = 0;
        }
        else
        {
            u16Precent = ((s32PWR - DEFAULT_PWR_MIN ) * 100) / (MS_S16)(DEFAULT_PWR_MAX - DEFAULT_PWR_MIN);

            DBG_U4_DMD("u16Percent = %d\n", u16Precent);

            if( u16Precent > 100 )
            {
                u16Precent = 100;
            }
        }

        return (MS_U16)u16Precent;
    }
    else
    {
        DBG_U4_DMD("MDrv_Dmd_GetPWR = 0\n");
        return 0;
    }

}

BOOLEAN DTV_Serial_Control(BOOLEAN bEnable)
{
    DBG_U4_DMD("\n");
    return TRUE;

}

//##########################################################################################################
//##########################################################################################################
//########################################  Public:DTV-DVB-T Implementation ################################
//##########################################################################################################
//##########################################################################################################
BOOLEAN DTV_GetLockStatus(void)
{
    MS_BOOL bLock = FALSE;

    if(MDrv_Dmd_GetLock(&bLock))
    {
        return bLock;
    }
    else
    {
        DBG_U4_DMD("MDrv_Dmd_GetLock = 0\n");
        return FALSE;
    }
}

//##########################################################################################################
//##########################################################################################################
//########################################  Private Function Implementation ################################
//##########################################################################################################
//##########################################################################################################








BOOLEAN Connect(EN_DEVICE_DEMOD_TYPE enDemodType)
{
    DBG_U4_DMD("\n");
    if (bIsDeviceBusy == TRUE)
        return FALSE;
//    m_enCurrentDemodulator_Type = enDemodType;
    bIsDeviceBusy = TRUE;
    return TRUE;
}

/*@ </Operation ID=I2b28dd03m121c8cf959bmm722c> @*/
/*@ <Operation ID=I2b28dd03m121c8cf959bmm7207> @*/
BOOLEAN Disconnect(void)
{
    DBG_U4_DMD("\n");
    bIsDeviceBusy = FALSE;
    return TRUE;
}

void Reset(void)
{
    DBG_U4_DMD("%d\n");

}

/*@ </Operation ID=I2b28dd03m121c8cf959bmm71bd> @*/
BOOLEAN IIC_Bypass_Mode(BOOLEAN enable)
{

    DBG_U4_DMD("\n");
    if (enable)
    {
        WriteReg(0x8010, 0x10);        // IIC by-pass mode on
    }
    else
    {
        WriteReg(0x8010, 0x00);        // IIC by-pass mode off
    }
    return TRUE;
}

/*@ <Operation ID=Im17018142m1221763cc7cmm46c6> @*/
BOOLEAN Power_On_Initialization(void)
{
    BOOLEAN bStatus = TRUE;
    DMD_Mode ModeBox;

    DBG_U4_DMD("\n");
    if (bIsPowerOnInitialized == TRUE)
    {
        return TRUE;
    }
    else
    {
        bIsPowerOnInitialized = TRUE;
    }

    bStatus &= MDrv_Dmd_Init(); //ok

    bStatus &= MDrv_Dmd_Open(); //Demod Open=True=1 ok

    ModeBox.bX4CFE_en              = X4CFE_ENABLE_MODE;
    ModeBox.bPPD_en            = PPD_ENABLE_MODE;
    ModeBox.bIQAutoSwap_en    = IQ_AUTO_ENABLE_MODE;
    ModeBox.bQAMScan_en        = QAM_AUTO_ENABLE_MODE;
    ModeBox.bFHO_en            = FHO_ENABLE_MODE;
    ModeBox.fptTunerSet        = DMD_TUNER_FREQ;

    bStatus &= MDrv_Dmd_SetMode(&ModeBox);
    bStatus &= MDrv_Dmd_Config(_u8dmdConfig); //Tuner Setup Array.


    if(bStatus)
    {
        DBG_U4_DMD(" SUCCESS\n");
    }
    else
    {
        DBG_U4_DMD(" FAIL\n");
    }

    return bStatus;
}


BOOLEAN Active(BOOLEAN bEnable)
{
    MS_U8 u8Status = TRUE;
    DBG_U4_DMD("\n");
    return u8Status;
}


/*@ </Operation ID=Im17018142m1221763cc7cmm464e> @*/
EN_DEVICE_DEMOD_TYPE GetCurrentDemodulatorType(void)
{
    DBG_U4_DMD("\n");
    return 0;
}


/***********************************************************************************
  Subject:    Command Packet Interface
  Function:   MSB1228_Cmd_Packet_Send
  Parmeter:
  Return:     BOOLEAN
  Remark:
************************************************************************************/


#if 0
BOOLEAN DTV_Set_Demod_TYPE(EN_DEVICE_DEMOD_TYPE enDemodType)
{
    m_enCurrentDemodulator_Type = enDemodType;
    return TRUE;
}


EN_DEVICE_DEMOD_TYPE DTV_Get_Demod_TYPE(void)
{
    DBG_U4_DMD("\n");
    return m_enCurrentDemodulator_Type;
}
#endif



#endif

