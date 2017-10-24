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
/// @file
/// @brief
/// @author MStar Semiconductor, Inc.
//
////////////////////////////////////////////////////////////////////////////////
#if (FRONTEND_DEMOD_TYPE == EMBEDDED_DVBT51_DEMOD)
#include <msAPI_Tuner.h>
#include <device/drvDemod.h>
#include "drvDemodK1DVBT.c"

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
MS_BOOL DTV_Lock(void)
{
    MS_BOOL block = FALSE;
    MS_BOOL ret = FALSE;

    ret = MDrv_Demod_GetLock(&block);
    if(ret == FALSE)
    {
        return FALSE;
    }
    return block;
}

MS_U16 DTV_GetSignalQuality(void)
{
    float fBER = 0.0;
    float flogBER = 0.0;
    MS_BOOL ret = FALSE;
    MS_U16 u16retval = 0;
    MS_BOOL bLock = FALSE;

    MDrv_Demod_GetLock(&bLock);
    if(bLock == FALSE)
    {
        printf("%s Lock Failed \n",__FUNCTION__);
        return 0;
    }

    ret = MDrv_Demod_GetBER(&fBER);
    if(ret == FALSE)
    {
        printf("Get BER Failed \n");
        return 0;
    }

    flogBER = (-1)*log10(1/fBER);	// Log10Approx() provide 1~2^32 input range only

    printf("Log(BER) = %f\n",flogBER);

    if ( flogBER <= (-7.0) )			// PostVit BER < 1e-7
    {
        u16retval = 100;
    }
    else  if ( flogBER < (-3.7) )		// PostVit BER < 2e-4
    {
        u16retval = 60 + (((-3.7) - flogBER) / ((-3.7)-(-7.0)) * (100-60));
    }
    else  if ( flogBER < (-2.7) )		// PostVit BER < 2e-3
    {
        u16retval = 10 + (((-2.7) - flogBER) / ((-2.7)-(-3.7)) * (60-10));
    }
    else
    {
        u16retval = 10;
    }
    return u16retval;

}

MS_U32 DTV_GetSNR(void)
{
    MS_U32 u32snr = 0;
    MS_BOOL ret = 0;

    ret = MDrv_Demod_GetSNR(&u32snr);
    if(ret == FALSE)
    {
        printf("Get SNR Failed \n");
    }
    return u32snr;
}
void DTV_SetFrequency(MS_U32 u32Frequency,MS_U8 eBandWidth)
{
    DEMOD_MS_FE_CARRIER_PARAM    dmdParam;

    dmdParam.u32Frequency  = u32Frequency;
    dmdParam.TerParam.eBandWidth  = eBandWidth - 1;//u16BandWidth / 1000;
    printf("FE %d  BW %d \n",dmdParam.u32Frequency,dmdParam.TerParam.eBandWidth );
    if (MDrv_Demod_Restart(&dmdParam) == FALSE)
    {
        printf("DVBT DEMOD RESTART \n");
    }
}

void DTV_Power_On_Initialization(void)
{

    printf("Internal DVB-T K1 PORTing 1 \n");
    if (MDrv_Tuner_Init() == FALSE)
    {
        printf("%s: Init driver failed.\n", __FUNCTION__);
        //return FALSE;
    }

    if (MDrv_Demod_Init() == FALSE)
    {
        printf("%s: Init demodulator fail\n", __FUNCTION__);
        //return FALSE;
    }

}
#endif

