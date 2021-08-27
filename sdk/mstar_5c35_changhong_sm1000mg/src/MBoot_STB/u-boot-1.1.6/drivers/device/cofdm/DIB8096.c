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
#include <sys/drv/drvIIC.h>
#include <sys/drv/drvGPIO.h>
#include <device/DIB8096.h>

double pow(double a, double b);
double log10(double flt_x);
double fabs (double x);

#include "DIB8096/dibcom_i2c_app.c"
#include "DIB8096/dibcom809x_api_2_layer_simple.c"
#include "DIB8096/dibcom8090.c"


#define DBG_DIB8096(format, args...) //printf("%04d %s " format, __LINE__, __FUNCTION__, ##args)

EN_DEVICE_DEMOD_TYPE m_enCurrentDemodulator_Type = E_DEVICE_DEMOD_DVB_T;
BOOLEAN bIsSerialOut = FALSE, bIsDeviceBusy = FALSE, bIsPowerOnInitialized = FALSE;

double fabs (double x)
{
    if(x<0)
    {
        return -x;
    }
    else
    {
        return x;
    }

}

double pow(double a, double b)
{
    int tmp = (*(1 + (int *)&a));
    int tmp2 = (int)(b * (tmp - 1072632447) + 1072632447);
    double p = 0.0;
    *(1 + (int * )&p) = tmp2;
    return p;
}


double log10(double flt_x)
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
    DBG_DIB8096("\n");
        if(dib_init() == DIB_RETURN_SUCCESS)
        {
        }

    return TRUE;
}


//##########################################################################################################
//##########################################################################################################
//########################################  Public:DTV Implementation ##################################
//##########################################################################################################
//##########################################################################################################

BOOLEAN DTV_SetFrequency(U32 u32Frequency, RF_CHANNEL_BANDWIDTH eBandWidth)
{
    DBG_DIB8096("(%d ,%d)\n", (int)u32Frequency, (int)eBandWidth);

    MS_BOOL bStatus=TRUE;

    bStatus = DTV_Config(eBandWidth, bIsSerialOut, FALSE);

    dib_tune(u32Frequency,6);

    return bStatus;
}


MS_U32 DTV_GetSNR(void)
{
    MS_U16  u8Lock = dib_lockstatus(), u16Signal = dib_signal_quality();

    DBG_DIB8096("%d %d\n", u8Lock, u16Signal);

    if( u8Lock != 0 && u16Signal > 0)
    {
        return 80;
    }

    return 0;
}


U32 DTV_GetBER(void)
{
    DBG_DIB8096("\n");

    return 0;
}




MS_U16 DTV_GetSignalQuality(void)
{
    MS_U16  u8Lock = dib_lockstatus(), u16Signal = dib_signal_quality();

    DBG_DIB8096("%d %d\n", u8Lock, u16Signal);

    if ( u8Lock )
    {
//        u16Signal = dib_signal_quality();
    }
    else
    {
        u16Signal = 0;
    }

    return u16Signal;
}


MS_U16 DTV_GetSignalStrength(void)
{
    MS_U8 u8Return = dib_signal_strength();
    DBG_DIB8096("%d\n", u8Return);

    return u8Return;

}

BOOLEAN DTV_Serial_Control(BOOLEAN bEnable)
{
    DBG_DIB8096("\n");
    return TRUE;

}

//##########################################################################################################
//##########################################################################################################
//########################################  Public:DTV-DVB-T Implementation ################################
//##########################################################################################################
//##########################################################################################################
BOOLEAN DTV_GetLockStatus(void)
{
    MS_U8 u8Return = dib_lockstatus();
    DBG_DIB8096("%d\n", u8Return);

    return u8Return;
}

//##########################################################################################################
//##########################################################################################################
//########################################  Private Function Implementation ################################
//##########################################################################################################
//##########################################################################################################








BOOLEAN Connect(EN_DEVICE_DEMOD_TYPE enDemodType)
{
    DBG_DIB8096("\n");
    if (bIsDeviceBusy == TRUE)
        return FALSE;
    m_enCurrentDemodulator_Type = enDemodType;
    bIsDeviceBusy = TRUE;
    return TRUE;
}

/*@ </Operation ID=I2b28dd03m121c8cf959bmm722c> @*/
/*@ <Operation ID=I2b28dd03m121c8cf959bmm7207> @*/
BOOLEAN Disconnect(void)
{
    DBG_DIB8096("\n");
    bIsDeviceBusy = FALSE;
    return TRUE;
}

void Reset(void)
{
    DBG_DIB8096("%d\n");
    dib0090_reset();
}

/*@ </Operation ID=I2b28dd03m121c8cf959bmm71bd> @*/
BOOLEAN IIC_Bypass_Mode(BOOLEAN enable)
{

    DBG_DIB8096("\n");

    return TRUE;
}

/*@ <Operation ID=Im17018142m1221763cc7cmm46c6> @*/
BOOLEAN Power_On_Initialization(void)
{
    BOOLEAN bStatus = TRUE;
    MS_U8 u8RetryCount = 0;
    DBG_DIB8096("\n");
    if (bIsPowerOnInitialized == TRUE)
    {
        return TRUE;
    }
    else
    {
        bIsPowerOnInitialized = TRUE;
    }


    do
    {
        if(bStatus == FALSE)
        {
            u8RetryCount++;
            bStatus=TRUE;
        }
#ifdef PAD_RESET_DEMOD
        DBG_DIB8096("Reset Demodulator after program firmware\n");
        mdrv_gpio_set_low(PAD_RESET_DEMOD);
        DBG_DIB8096("mdrv_gpio_set_low(%d)\n", PAD_RESET_DEMOD);

        MsOS_DelayTask(1000);
        mdrv_gpio_set_high(PAD_RESET_DEMOD);
        DBG_DIB8096("mdrv_gpio_set_high(%d)\n", PAD_RESET_DEMOD);

        MDrv_IIC_Init();

#endif
        bStatus = TRUE;

    }while((u8RetryCount < 7) && (bStatus == FALSE));

    if(bStatus)
    {
        DBG_DIB8096(" SUCCESS\n");
    }
    else
    {
        DBG_DIB8096(" FAIL\n");
    }

    return bStatus;
}


BOOLEAN Active(BOOLEAN bEnable)
{
    MS_U8 u8Status = TRUE;
    DBG_DIB8096("\n");
    return u8Status;
}


/*@ </Operation ID=Im17018142m1221763cc7cmm464e> @*/
EN_DEVICE_DEMOD_TYPE GetCurrentDemodulatorType(void)
{
    DBG_DIB8096("\n");
    return m_enCurrentDemodulator_Type;
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
    DBG_DIB8096("\n");
    return m_enCurrentDemodulator_Type;
}
#endif



#endif

