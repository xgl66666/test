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
#include <frontend_config.h>
#if (FRONTEND_DEMOD_TYPE == DEMOD_AVL6211)

#include <sys/common/MsOS.h>
#include <sys/common/MsTypes.h>
#include <sys/drv/drvGPIO.h>
#include <sys/common/MsCommon.h>
#include <msAPI_Tuner.h>
#include <device/drvDemod.h>
#include <apiDTVSatTable.h>
#include "../../Dish/drvDish.h"
#include <apiDigiTuner.h>
static MS_BOOL bIsHiLOF = FALSE;
static MS_SAT_PARAM _stSATParam;
#define MAX_C_LOF_FREQ    6350   //4200+2150
#define SAT_MOVE_TIME           10 // s

static MS_BOOL _DigiTuner_Satellite_Set22K(MS_BOOL b22KOn)
{
     MS_BOOL bRet = TRUE;
     if(b22KOn)
     {
         bRet = MDrv_Dish_Set22k(EN_LNB22K_TYPE_ON);
     }
     else
     {
         bRet = MDrv_Dish_Set22k(EN_LNB22K_TYPE_OFF);
     }
     return bRet;
}

static MS_BOOL _DigiTuner_SetLNBPower(MS_EN_LNBPWR_ONOFF enLNBPower,MS_U8 u8Polarity)
{
    switch(enLNBPower)
    {
         case  EN_LNBPWR_13OR18V:
         {
              if(u8Polarity)
                    return MDrv_Dish_SetLNBPower(EN_LNBPWR_TYPE_13V);
              else
                    return MDrv_Dish_SetLNBPower(EN_LNBPWR_TYPE_18V);
             
         }

         case EN_LNBPWR_OFF:
         {
              return MDrv_Dish_SetLNBPower(EN_LNBPWR_TYPE_OFF);
         }

         case EN_LNBPWR_13V:
         {
              return MDrv_Dish_SetLNBPower(EN_LNBPWR_TYPE_13V);
         }
         case EN_LNBPWR_18V:
         {
              return MDrv_Dish_SetLNBPower(EN_LNBPWR_TYPE_18V);
         }
         default:break;
    }
    MsOS_DelayTask(100);
    return TRUE;
}

static MS_BOOL _DigiTuner_DiSEqC_SendCmd(MS_U8* pCmd,MS_U8 u8CmdSize)
{
	MS_BOOL bRet = TRUE;

	_DigiTuner_Satellite_Set22K(FALSE);
	bRet = MDrv_Dish_SendCmd(pCmd,u8CmdSize);
	if(_stSATParam.e22KOnOff == EN_22K_ON
	||(_stSATParam.e22KOnOff == EN_22K_AUTO && bIsHiLOF))
	{
	     _DigiTuner_Satellite_Set22K(TRUE);
	}
	return bRet;
}

static MS_BOOL _DiSEqC_SwitchPort(MS_U8 u8Port,MS_BOOL bUncommitted)
{
	MS_U8 u8Cmd[]={0xE0,0x10,0x38,0xF0};

	if(bUncommitted)
	{
	  u8Cmd[2] = 0x39;
	}
	u8Cmd[3] = 0xF0+u8Port;

	return  _DigiTuner_DiSEqC_SendCmd(u8Cmd,sizeof(u8Cmd));
}

static MS_BOOL _DiSEqC_GotoSatPosition(MS_U8 u8Pos)
{
        MS_U8 u8Cmd[]={0xE0,0x31,0x6B,u8Pos};
        u8Pos &= DISEQC12_USALS_POSITION_MASK;
        if(u8Pos == 0)
        {
              return FALSE;
        }
        _stSATParam.u8Position = u8Pos;
        return  _DigiTuner_DiSEqC_SendCmd(u8Cmd,sizeof(u8Cmd));
}



 U16 abs(MS_U16 num_in)
{
	int	num_out;
	if(num_in < 0)
		num_out =(0-num_in);
	else
		num_out = num_in;
	return num_out;

}

static MS_BOOL _MultiDiSEqc_SwitchPort(MS_SAT_PARAM *pSATParam,MS_U8 u8Polarity,MS_BOOL bIsHiLOF)
{
         MS_U8 u8SubPort = 0;

         _DigiTuner_Satellite_Set22K(FALSE);

         if(pSATParam->eToneburstType == EN_TONEBURST_1)
         {
                MDrv_Dish_SetTone(EN_TONEBURST_TYPE_1);
         }
         else if(pSATParam->eToneburstType == EN_TONEBURST_0)
         {
               MDrv_Dish_SetTone(EN_TONEBURST_TYPE_0);
         }
         MsOS_DelayTask(10);
         if(u8Polarity == 0)
              u8SubPort |= 0x02;
         if(bIsHiLOF)
              u8SubPort |= 0x01;
         switch(pSATParam->eSwt10Port)
         {
              case EN_SWT_PORT_1:
                    u8SubPort = u8SubPort;
                    break;
              case EN_SWT_PORT_2:
                    u8SubPort = 0x04+ u8SubPort;
                    break;
              case EN_SWT_PORT_3:
                    u8SubPort = 0x08+ u8SubPort;
                    break;
              case EN_SWT_PORT_4:
                    u8SubPort = 0x0C+ u8SubPort;
                    break;
              default:
                    break;
         }

         if(pSATParam->eSwt10Port >= EN_SWT_PORT_1 && pSATParam->eSwt10Port <= EN_SWT_PORT_4)
         {
              _DiSEqC_SwitchPort(u8SubPort,FALSE);
         }
         MsOS_DelayTask(50);
         if(pSATParam->eSwt11Port >= EN_SWT_PORT_1 && pSATParam->eSwt11Port <= EN_SWT_PORT_16)
         {
              _DiSEqC_SwitchPort((MS_U8)(pSATParam->eSwt11Port)-1,TRUE);
         }
         MsOS_DelayTask(50);
   
         _DiSEqC_GotoSatPosition((pSATParam->u8Position&DISEQC12_USALS_POSITION_MASK));

         MsOS_DelayTask(50);
         if(pSATParam->e22KOnOff == EN_22K_ON
           ||(pSATParam->e22KOnOff == EN_22K_AUTO && bIsHiLOF))
         {
               _DigiTuner_Satellite_Set22K(TRUE);
         }

         return TRUE;
}

static MS_BOOL  _DigiTuner_SwitchSatellite(MS_SAT_PARAM *pSATParam,MS_U8 u8Polarity,MS_BOOL bIsHiLOF)
{
	_DigiTuner_Satellite_Set22K(FALSE);

	switch(pSATParam->eDiseqcLevel)
	{
		case EN_DISEQC_LVL_OFF:
		{
		if(pSATParam->eToneburstType == EN_TONEBURST_1)
		{
			      MDrv_Dish_SetTone(EN_TONEBURST_TYPE_1);
			}
			else if(pSATParam->eToneburstType == EN_TONEBURST_0)
			{
			     MDrv_Dish_SetTone(EN_TONEBURST_TYPE_0);
			}
			break;
			}

			case EN_DISEQC_LVL_10:
			{
			MS_U8 u8SubPort = 0;
			if(pSATParam->eToneburstType == EN_TONEBURST_1)
			{
			      MDrv_Dish_SetTone(EN_TONEBURST_TYPE_1);
			}
			else if(pSATParam->eToneburstType == EN_TONEBURST_0)
			{
			      MDrv_Dish_SetTone(EN_TONEBURST_TYPE_0);
			}
			if(u8Polarity == 0)
			    u8SubPort |= 0x02;
			if(bIsHiLOF)
			    u8SubPort |= 0x01;
			switch(pSATParam->eSwt10Port)
			{
			    case EN_SWT_PORT_1:
			          u8SubPort = u8SubPort;
			          break;
			    case EN_SWT_PORT_2:
			          u8SubPort = 0x04+ u8SubPort;
			          break;
			    case EN_SWT_PORT_3:
			          u8SubPort = 0x08+ u8SubPort;
			          break;
			    case EN_SWT_PORT_4:
			          u8SubPort = 0x0C+ u8SubPort;
			          break;
			    default:
			          break;
			}
			if(pSATParam->eSwt10Port >= EN_SWT_PORT_1 && pSATParam->eSwt10Port <= EN_SWT_PORT_4)
			{
			    _DiSEqC_SwitchPort(u8SubPort,FALSE);
			}
			break;
			}
			case EN_DISEQC_LVL_11:
			{
			if(pSATParam->eSwt11Port >= EN_SWT_PORT_1 && pSATParam->eSwt11Port <= EN_SWT_PORT_16)
			{
			     _DiSEqC_SwitchPort((MS_U8)(pSATParam->eSwt11Port)-1,TRUE);
			}
			break;
			}

			case EN_DISEQC_LVL_12:
			case EN_DISEQC_LVL_13:
			{
			if((pSATParam->u8Position&DISEQC12_USALS_POSITION_MASK) != (pSATParam->u8Position&DISEQC12_USALS_POSITION_MASK))
			{
			_DiSEqC_GotoSatPosition(pSATParam->u8Position&DISEQC12_USALS_POSITION_MASK);
			}
			break;
			}

			case EN_DISEQC_LVL_MULTI:
			{
			_MultiDiSEqc_SwitchPort(pSATParam,u8Polarity,bIsHiLOF);
			}
			default:
			break;
	}
	if(pSATParam->e22KOnOff == EN_22K_ON
	 ||(pSATParam->e22KOnOff == EN_22K_AUTO && bIsHiLOF))
	{
	     _DigiTuner_Satellite_Set22K(TRUE);
	}
	return TRUE;
}

static  MS_BOOL  _DigiTuner_CalculateIF(MS_SAT_PARAM *pSATParam,MS_FE_CARRIER_PARAM *pParam,DEMOD_MS_FE_CARRIER_PARAM* pdmdParam)
{
      MS_S32 s32Freq;

      // calc Mid Freq & LNB control(22K on/off)
      if(pSATParam->u16LoLOF == pSATParam->u16HiLOF)
      {
            s32Freq  = pParam->u32Frequency;
            s32Freq -= pSATParam->u16LoLOF;
            if(s32Freq < 0) s32Freq = -s32Freq;
            pdmdParam->u32Frequency = s32Freq;
            bIsHiLOF = FALSE;
      }
      else // 2LOF
      {
            if(pSATParam->u16LoLOF <MAX_C_LOF_FREQ)  // c band
            {
                  s32Freq  = pParam->u32Frequency;
                  if(pSATParam->bPorInvert)
                  {
                        if(pParam->SatParam.u8Polarity == 1) //V ---> use low LOF
                        {
                                  bIsHiLOF = FALSE;
                                 pdmdParam->u32Frequency = abs(pSATParam->u16LoLOF - s32Freq);
                        }
                        else   //H --->use high LOF
                        {
                                 bIsHiLOF = TRUE;
                                 pdmdParam->u32Frequency =	abs(pSATParam->u16HiLOF - s32Freq) ;
                        }
                  }
                  else
                  {
                        if(pParam->SatParam.u8Polarity == 0) //H ---> use low LOF
                        {
                                  bIsHiLOF = FALSE;
                                 pdmdParam->u32Frequency = abs(pSATParam->u16LoLOF - s32Freq);
                        }
                        else   //V --->use high LOF
                        {
                                 bIsHiLOF = TRUE;
                                 pdmdParam->u32Frequency =	abs(pSATParam->u16HiLOF - s32Freq) ;
                        }
                  }
            }
            else //Ku band
            {
                  MS_U32 u32MidFreq;
                  MS_S32 s32Offset = 0;
                  s32Freq  = pParam->u32Frequency;
                  u32MidFreq =	abs(s32Freq - pSATParam->u16LoLOF) ;
                  s32Offset = pSATParam->u16LoLOF+2150 - pSATParam->u16HiLOF - 950;
                  if(s32Offset < 0)
                  {
                       s32Offset = 0;
                  }
                  else
                  {
                       s32Offset /= 2;
                  }
                  if( u32MidFreq <= (2150-s32Offset))
                  {
                        bIsHiLOF = FALSE;
                        pdmdParam->u32Frequency = u32MidFreq;
                  }
                  else
                  {
                        bIsHiLOF = TRUE;
                        pdmdParam->u32Frequency =	abs(s32Freq - pSATParam->u16HiLOF) ;
                  }
            }
      }
      if( pdmdParam->u32Frequency < 950 || pdmdParam->u32Frequency > 2150)
      {
            pdmdParam->u32Frequency = 945;
            bIsHiLOF = FALSE;
      }
      return TRUE;
}



MS_BOOL MApi_DigiTuner_Tune2RfCh(MS_SAT_PARAM *pSATParam,MS_FE_CARRIER_PARAM *pParam)
{
    DEMOD_MS_FE_CARRIER_PARAM dmdParam;//DEMOD_MS_FE_CARRIER_PARAM
	MS_FE_CARRIER_PARAM	_tunerParam;
	
    bIsHiLOF = FALSE;
    _DigiTuner_CalculateIF(pSATParam,pParam,&dmdParam);
    _DigiTuner_SetLNBPower(pSATParam->eLNBPwrOnOff,pParam->SatParam.u8Polarity);
    _DigiTuner_SwitchSatellite(pSATParam,pParam->SatParam.u8Polarity,bIsHiLOF);
    _tunerParam = *pParam;
    

    dmdParam.SatParam.u32SymbolRate = pParam->SatParam.u32SymbolRate;
    dmdParam.SatParam.u32SymbolRate *= 1000;
    printf("\n==%s==%d==%ld==%ld==\n",__FUNCTION__,__LINE__,dmdParam.u32Frequency,dmdParam.SatParam.u32SymbolRate);
    if (MDrv_Demod_Restart(&dmdParam) == FALSE)
    {
        printf("%s: COfdm DMD restart failed.\n", __FUNCTION__);
        return FALSE;
    }
	return TRUE;
}

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
MS_BOOL  MApi_FE_CheckLock(void)
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

MS_U16 MApi_FE_GetSignalQuality(void)
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

MS_U32 MApi_FE_GetSNR(void)
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

static MS_SAT_PARAM s_SATParam;
static MS_FE_CARRIER_PARAM	s_tunerParam;
MS_BOOL MApi_FE_SetSATParam(MS_SAT_PARAM *pSATParam)
{
	memcpy(&s_SATParam, pSATParam, sizeof(MS_SAT_PARAM));
	return TRUE;
}

MS_BOOL MApi_FE_SetFECarrierParam(MS_FE_CARRIER_PARAM *pParam)
{
	memcpy(&s_tunerParam, pParam, sizeof(MS_FE_CARRIER_PARAM));
	return TRUE;
}

void MApi_FE_SetFrequency(MS_U32 u32Frequency,MS_U8 eBandWidth)
{
    DEMOD_MS_FE_CARRIER_PARAM    dmdParam;

    bIsHiLOF = FALSE;
    s_tunerParam.u32Frequency = u32Frequency;
    _DigiTuner_CalculateIF(&s_SATParam, &s_tunerParam,&dmdParam);
    _DigiTuner_SetLNBPower(s_SATParam.eLNBPwrOnOff,s_tunerParam.SatParam.u8Polarity);
    _DigiTuner_SwitchSatellite(&s_SATParam, s_tunerParam.SatParam.u8Polarity,bIsHiLOF);

	//dmdParam.u32Frequency = u32Frequency;
    dmdParam.SatParam.u32SymbolRate = s_tunerParam.SatParam.u32SymbolRate;
    dmdParam.SatParam.u32SymbolRate *= 1000;
    printf("\n==%s==%d==%ld==%ld==\n",__FUNCTION__,__LINE__,dmdParam.u32Frequency,dmdParam.SatParam.u32SymbolRate);
    if (MDrv_Demod_Restart(&dmdParam) == FALSE)
    {
        printf("%s: COfdm DMD restart failed.\n", __FUNCTION__);
        //return FALSE;
    }
	//return TRUE;

}

void MApi_FE_Power_On_Initialization(void)
{

    printf("ENTRY, [%s] at <%s> \n", __FUNCTION__, __FILE__);

    if (MDrv_Demod_Init() == FALSE)
    {
        printf("%s: Init demodulator fail\n", __FUNCTION__);
    }

}
#endif

