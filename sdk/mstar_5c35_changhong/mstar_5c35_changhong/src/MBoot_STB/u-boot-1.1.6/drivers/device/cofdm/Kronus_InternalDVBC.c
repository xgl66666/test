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
#if (FRONTEND_DEMOD_TYPE == EMBEDDED_DVBC51_DEMOD)
#include <msAPI_Tuner.h>
#include <device/drvDemod.h>
#include "drvDemodK1DVBC.c"

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

extern MS_U32 MsOS_GetSystemTime (void);

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
    MS_U32                 u32Timeout;
    Demod_Mode dmdMode;

    #if (FRONTEND_TUNER_TYPE == TUNER_MXL603)    
	if (MDrv_Tuner_SetTuner(u32Frequency,eBandWidth) == FALSE)
	#else
	if (MDrv_Tuner_SetFreq(u32Frequency) == FALSE)
	#endif
    {
        printf("%s: Set frequency failed.\n", __FUNCTION__);
        //RETURN(FALSE);
    }


    memset(&dmdMode, 0x0, sizeof(Demod_Mode));
    memset(&dmdParam, 0x0, sizeof(DEMOD_MS_FE_CARRIER_PARAM));

    if (0)//(FE_TUNE_AUTO == eMode)
    {   // channel scan
        //HB_printf("Channel scan\n");
        printf(" ==== FE_TUNE_AUTO ==== \n");
        dmdMode.bX4CFE_en      = X4CFE_ENABLE_MODE; // X4CFE_ENABLE_MODE;
        dmdMode.bPPD_en        = PPD_ENABLE_MODE; // PPD_ENABLE_MODE;

        dmdMode.bIQAutoSwap_en = IQ_AUTO_ENABLE_MODE;
        dmdMode.bQAMScan_en    = QAM_AUTO_ENABLE_MODE;
        dmdMode.bFHO_en        = FHO_ENABLE_MODE;
    }
    else
    {   // channel switch
        //HB_printf("Channel switch\n");
        memset(&dmdMode, 0, sizeof(dmdMode));
        printf("non auto \n");
        dmdMode.bX4CFE_en      = X4CFE_ENABLE_MODE; // X4CFE_ENABLE_MODE;
        dmdMode.bPPD_en        = PPD_ENABLE_MODE; // PPD_ENABLE_MODE;
        dmdMode.bIQAutoSwap_en  = IQ_AUTO_ENABLE_MODE;
        dmdMode.bQAMScan_en     = QAM_AUTO_ENABLE_MODE;
        dmdMode.bFHO_en         = FHO_ENABLE_MODE;
        //DMD_TUNER_FREQ;

        // dmdMode.bIQAutoSwap_en = IQ_AUTO_ENABLE_MODE;
        // dmdMode.bQAMScan_en    = QAM_AUTO_ENABLE_MODE;
        // dmdMode.bFHO_en        = FHO_ENABLE_MODE;
    }
    dmdMode.fptTunerSet     = FALSE;
    //dmdMode.fptTunerSet = MDrv_Tuner_SetFreq;  chienfong
    // dmdMode.fptTunerSet = NULL;

    MDrv_Demod_SetMode(&dmdMode);
    //MDrv_Demod_Config(dmdConfig);
    dmdParam.CabParam.eConstellation = E_DVBC_QAM64;//pParam->CabParam.eConstellation;
    dmdParam.CabParam.u16SymbolRate = (long)6875; //pParam->CabParam.u16SymbolRate;
    dmdParam.CabParam.eIQMode = CAB_IQ_NORMAL;//pParam->CabParam.eIQMode;
    dmdParam.CabParam.u8TapAssign = E_DVBC_TAP_POST;//pParam->CabParam.u8TapAssign;
    dmdParam.CabParam.u32FreqOffset = 0;//pParam->CabParam.u32FreqOffset;
    dmdParam.CabParam.u8TuneFreqOffset = 0;//pParam->CabParam.u8TuneFreqOffset;
    dmdParam.u32Frequency  = u32Frequency;
    //#if  1// (FRONTEND_TUNER_TYPE != TUNER_PLAYCARD)

    u32Timeout = MsOS_GetSystemTime();
    while (MDrv_Demod_Restart(&dmdParam) != TRUE)
    {
        if ((MsOS_GetSystemTime()-u32Timeout) >= 5000)
        {
            printf("%s: demodulator restart fail.\n", __FUNCTION__);
            //RETURN(FALSE);
        }
    }

}

void DTV_Power_On_Initialization(void)
{

    printf("Internal DVB-C K1 PORTing 1 \n");
    /*
    if (MDrv_Tuner_Init() == FALSE)
    {
        printf("%s: Init driver failed.\n", __FUNCTION__);
        //return FALSE;
    }
    */
    if (MDrv_Demod_Init() == FALSE)
    {
        printf("%s: Init demodulator fail\n", __FUNCTION__);
        //return FALSE;
    }

}
#endif

