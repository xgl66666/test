/*
 *           Copyright 2007-2009 Availink, Inc.
 *
 *  This software contains Availink proprietary information and
 *  its use and disclosure are restricted solely to the terms in
 *  the corresponding written license agreement. It shall not be
 *  disclosed to anyone other than valid licensees without
 *  written permission of Availink, Inc.
 *
 */
#include <common.h>
#include <frontend_config.h>
#include "BSP/IBSP.h"
#include "AVL_DVBSx/include/avl_dvbsx.h"
#include "AVL_DVBSx/include/IBase.h"
#include "AVL_DVBSx/include/IRx.h"
#include "AVL_DVBSx/include/ITuner.h"
#include "AVL_DVBSx/include/II2C.h"
#include "AVL_DVBSx/include/IDiseqc.h"
#include "AVL_DVBSx/include/IBlindScan.h"
#include "AVL_DVBSx_Func.h"
//#include "Board.h"
//#include "MsCommon.h"
//#include "HbCommon.h"
#if(FRONTEND_DEMOD_TYPE == DEMOD_AVL6211)
//#if (FRONTEND_TUNER_TYPE == TUNER_AV2011)
#include "../../tuner/AV2011/ExtAV2011.h"
//#endif
//#define NULL 0
#define TS_PARALLEL_OUTPUT          1
#define TS_CLK_INV 1

#define Diseqc_Tone_Frequency   22              // The Diseqc bus speed in the unit of kHz. Normally, it should be 22kHz.

#define bs_min_symbolrate		1			//The minimum symbol rate, 1M
#define bs_max_symbolrate		45			//The maximum symbol rate, 45M

#define TONE_COUNT              8

extern AVL_uchar ucPatchData [];			 //Defined in demod_patch.dat.cpp.

struct Signal_Level SignalLevel[47]=
{
	{8285,	-922},{10224,	-902},{12538,	-882},{14890,	-862},{17343,	-842},{19767,	-822},{22178,	-802},{24618,	-782},{27006,	-762},{29106,	-742},
	{30853,	-722},{32289,	-702},{33577,	-682},{34625,	-662},{35632,	-642},{36552,	-622},{37467,	-602},{38520,	-582},{39643,	-562},{40972,	-542},
	{42351,	-522},{43659,	-502},{44812,	-482},{45811,	-462},{46703,	-442},{47501,	-422},{48331,	-402},{49116,	-382},{49894,	-362},{50684,	-342},
	{51543,	-322},{52442,	-302},{53407,	-282},{54314,	-262},{55208,	-242},{56000,	-222},{56789,	-202},{57544,	-182},{58253,	-162},{58959,	-142},
	{59657,	-122},{60404,	-102},{61181,	-82},{62008,	-62},{63032,	-42},{65483,	-22},{65535,	-12}

};

static	struct AVL_DVBSx_Chip  _AVL_DVBSxChip;
static	struct AVL_Tuner _Tuner;
static	struct AVL_DVBSx_Chip * pAVLChip = &_AVL_DVBSxChip;
static	struct AVL_Tuner * pTuner =&_Tuner;
static      AVL_uint32  signal_symbolrate = 22425000;		//The symbol rate in a unit of Hz.
#define  tuner_freq   1101
static     AVL_uchar  _bTuneError = 0;

static struct AVL_DVBSx_BlindScanPara bsPara;
static struct AVL_DVBSx_BlindScanInfo bsInfo;
static  AVL_uint16 bs_start_freq = 950;			//The start rf frequency, 965M
static AVL_uint16 bs_stop_freq = 2150;		//The stop rf frequency, 2100M
static AVL_uint16 uiNextStartFreq_100kHz;
static AVL_uchar ucBlindWaitCount = 10;
//static AVL_uint16 uiMaxLPF;

static AVL_DVBSx_ErrorCode AVL_DVBSx_TuneChannel(AVL_uint16 uiFreq_MHz,AVL_uint32 uiSymbolRate_Hz);
//for Send out Tone
/*****************************************************************
*  AVL_DVBSx_DiSEqC_SetToneOut
*
*  set tone
*
*  Input:
*       ucTone     1 or 0
*       ucCount    the repeat 1(0) count
*
*****************************************************************/
AVL_DVBSx_ErrorCode AVL_DVBSx_DiSEqC_SetToneOut(AVL_uchar ucTone)
{
       AVL_DVBSx_ErrorCode r = AVL_DVBSx_EC_OK;
       struct AVL_DVBSx_Diseqc_TxStatus sTxStatus;

       r = AVL_DVBSx_IDiseqc_SendTone(ucTone, TONE_COUNT, pAVLChip);
	if(AVL_DVBSx_EC_OK != r)
	{
		printf("Send tone %d --- Fail!\n",ucTone);
	}
	else
	{
	    do
	    {
		    r =AVL_DVBSx_IDiseqc_GetTxStatus(&sTxStatus, pAVLChip);   //Get current status of the Diseqc transmitter data FIFO.
	    }
	    while(1 != sTxStatus.m_TxDone);			//Wait until operation finished.
	    if(AVL_DVBSx_EC_OK == r)
	    {
		    ;
	    }
	    else
	    {
		    printf("Output tone %d --- Fail!\n",ucTone);;
	    }
	}
       return (r);
}

//for DiSEqC
AVL_DVBSx_ErrorCode AVL_DVBSx_DiSEqC_SetLNBOut(AVL_uchar ucLow)
{
       AVL_DVBSx_ErrorCode r = AVL_DVBSx_EC_OK;
       AVL_uchar uiOut;

       r = AVL_DVBSx_IDiseqc_SetLNBOut(ucLow, pAVLChip);
	if(AVL_DVBSx_EC_OK != r)
	{
		printf("Set LNB output  --- Fail !\n");
	}
	else
	{
 	    r = AVL_DVBSx_IDiseqc_GetLNBOut(&uiOut, pAVLChip);   //Get the current LNB output pin level.
           if((AVL_DVBSx_EC_OK == r) && (ucLow == uiOut))
	    {
		     ;
	    }
	    else
	    {
		     printf("LNB output not change --- Fail !\n");
                   r = AVL_DVBSx_EC_GeneralFail;
	    }
	}
       return (r);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_DiSEqC_GetLNBOut(AVL_puchar pucLNBOut)
{
       AVL_DVBSx_ErrorCode r = AVL_DVBSx_EC_OK;
       AVL_uchar uiOut = 0;

       r = AVL_DVBSx_IDiseqc_GetLNBOut(&uiOut, pAVLChip);   //Get the current LNB output pin level.
       * pucLNBOut = uiOut;

       return (r);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_DiSEqC_Set22kOnOff(AVL_uchar uc22kOn)
{
       AVL_DVBSx_ErrorCode r = AVL_DVBSx_EC_OK;

       if(uc22kOn)
       {
             r = AVL_DVBSx_IDiseqc_StartContinuous(pAVLChip);
		//	 printf("\n==harry==%d==%s==%d==\n",r,__FUNCTION__,__LINE__);
       }
       else
       {
             r = AVL_DVBSx_IDiseqc_StopContinuous(pAVLChip);
		//	 printf("\n==harry==%d==%s==%d==\n",r,__FUNCTION__,__LINE__);
       }
	if(AVL_DVBSx_EC_OK == r)
	{
		;
	}
	else
	{
		printf("Set 22k On/Off --- Fail!\n");
	}
       return (r);
}

AVL_DVBSx_ErrorCode  AVL_DVBSx_DiSEqC_Get22kOnOff(AVL_puchar puc22kOn)
{
       AVL_DVBSx_ErrorCode r = AVL_DVBSx_EC_OK;
       AVL_uchar ui22kon = 0;

       r = AVL_DVBSx_IDiseqc_GetContinuous(&ui22kon, pAVLChip);   //Get the current LNB output pin level.
       * puc22kOn = ui22kon;

       return AVL_DVBSx_EC_OK;
}

AVL_DVBSx_ErrorCode AVL_DVBSx_DiSEqC_SetCmd(AVL_puchar pucCmd,AVL_uchar ucCmdSize)
{
       AVL_DVBSx_ErrorCode r = AVL_DVBSx_EC_OK;
       struct AVL_DVBSx_Diseqc_TxStatus sTxStatus;

       r = AVL_DVBSx_IDiseqc_SendModulationData(pucCmd, ucCmdSize, pAVLChip);
	if(AVL_DVBSx_EC_OK != r)
	{
		printf("Output  modulation bytes --- Fail!\n");
	}
	else
	{
	    do
	    {
		    r = AVL_DVBSx_IDiseqc_GetTxStatus(&sTxStatus, pAVLChip);        //Get current status of the Diseqc transmitter data FIFO.
	    }
	    while(1 != sTxStatus.m_TxDone);			//Wait until operation finished.
	    if(AVL_DVBSx_EC_OK == r)
	    {
		    ;
	    }
	    else
	    {
		    printf("Output modulation bytes --- Fail!\n");
	    }
	}
       return (r);
}

//for channel
#if 0
AVL_DVBSx_ErrorCode Init_AVL_DVBSx_ChipObject(struct AVL_DVBSx_Chip * pAVL_DVBSx_ChipObject, AVL_uint16 uiSlaveAddress)
{
	AVL_DVBSx_ErrorCode r;
	pAVL_DVBSx_ChipObject->m_SlaveAddr = uiSlaveAddress;
	pAVL_DVBSx_ChipObject->m_StdBuffIndex = 0;
	r = AVL_DVBSx_IBSP_InitSemaphore(&(pAVL_DVBSx_ChipObject->m_semRx));
	r |= AVL_DVBSx_IBSP_InitSemaphore(&(pAVL_DVBSx_ChipObject->m_semI2CRepeater));
	r |= AVL_DVBSx_IBSP_InitSemaphore(&(pAVL_DVBSx_ChipObject->m_semI2CRepeater_r));
	r |= AVL_DVBSx_IBSP_InitSemaphore(&(pAVL_DVBSx_ChipObject->m_semDiseqc));
	r |= AVL_DVBSx_II2C_Initialize(); // there is a internal protection to assure it will be initialized only once.
	return (r);
}
#endif
AVL_DVBSx_ErrorCode CPU_Halt(struct AVL_DVBSx_Chip * pAVLChip)
{
     AVL_DVBSx_ErrorCode r;
     AVL_uint16 i= 0;

     r = AVL_DVBSx_IBase_SendRxOP(OP_RX_HALT, pAVLChip );

     if(AVL_DVBSx_EC_OK == r)
     {
          while(i++<20)
          {
              r = AVL_DVBSx_IBase_GetRxOPStatus(pAVLChip);
              if(AVL_DVBSx_EC_OK == r)
              {
                   break;
              }
              else
              {
                   AVL_DVBSx_IBSP_Delay(10);
              }
          }
     }
	 return (r);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_SetMpegMode(AVL_uchar bParallelTS,AVL_uchar bLockChannel)
{
       AVL_DVBSx_ErrorCode r=AVL_DVBSx_EC_OK;
       struct AVL_DVBSx_MpegInfo sMpegMode;

       //Setup MPEG mode parameters.
       sMpegMode.m_MpegFormat = AVL_DVBSx_MPF_TSP;
       if(bParallelTS)
       {
            sMpegMode.m_MpegMode = AVL_DVBSx_MPM_Parallel;
       }
       else
       {
	      sMpegMode.m_MpegMode = AVL_DVBSx_MPM_Serial;
       }

    #if TS_CLK_INV
	sMpegMode.m_MpegClockPolarity = AVL_DVBSx_MPCP_Rising;
    #else
       sMpegMode.m_MpegClockPolarity = AVL_DVBSx_MPCP_Falling;
    #endif

	//This function should be called after demod initialized to set MPEG mode.
	r = AVL_DVBSx_IRx_SetMpegMode(&sMpegMode,pAVLChip );
	if(AVL_DVBSx_EC_OK != r)
	{
		printf("Set MPEG output mode failed !\n");
              return (r);
	}
       if(bParallelTS == 0)
       {
             r = AVL_DVBSx_IRx_SetMpegSerialPin(pAVLChip,AVL_DVBSx_MPSP_DATA0);
             r = AVL_DVBSx_IRx_SetMpegBitOrder(pAVLChip,AVL_DVBSx_MPM_Serial,AVL_DVBSx_MPBO_Normal);
       }
       else
       {
             r = AVL_DVBSx_IRx_SetMpegBitOrder(pAVLChip,AVL_DVBSx_MPM_Parallel,AVL_DVBSx_MPBO_Normal);
             //r = AVL_DVBSx_IRx_EnableMpegPersistentClockMode(800,pAVLChip);
       }
       // Enable the MPEG output (this function call has no effect for the AVL_DVBSxLG and AVL_DVBSxLGa devices)
       r = AVL_DVBSx_IRx_DriveMpegOutput(pAVLChip);

       if(bLockChannel)
       {
            struct AVL_DVBSx_Channel Channel;

            Channel.m_uiSymbolRate_Hz = signal_symbolrate;
            Channel.m_Flags = (CI_FLAG_IQ_NO_SWAPPED) << CI_FLAG_IQ_BIT;	//Normal IQ
            Channel.m_Flags |= (CI_FLAG_IQ_AUTO_BIT_AUTO) << CI_FLAG_IQ_AUTO_BIT;	//Enable automatic IQ swap detection
            Channel.m_Flags |= (CI_FLAG_DVBS2_UNDEF) << CI_FLAG_DVBS2_BIT;			//Enable automatic standard detection

            //This function should be called after tuner locked to lock the channel.
            r = AVL_DVBSx_IRx_LockChannel(&Channel, pAVLChip);
            if (AVL_DVBSx_EC_OK != r)
            {
                printf("Lock channel failed !\n");
             }
       }

       return (r);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_Initialize(void)
{
      struct AVL_DVBSx_Diseqc_Para sDiseqcPara;
	AVL_DVBSx_ErrorCode r=AVL_DVBSx_EC_OK;

       printf("\n ------ AVL_DVBSx_Initialize -------");
       pTuner->m_pLockFunc = NULL;
       pTuner->m_pGetLockStatusFunc = NULL;
       // This function should be called after bsp initialized to initialize the chip object.
       r = Init_AVL_DVBSx_ChipObject(pAVLChip, AVL_DVBSx_SA_0 <<1);
	if( AVL_DVBSx_EC_OK !=r )
	{
		printf("Chip Object Initialization failed !\n");
		return (r);
	}

       //This function should be called after chip object initialized to initialize the IBase,using reference clock as 10M. Make sure you pickup the right pll_conf since it may be modified in BSP.
	r = AVL_DVBSx_IBase_Initialize((const struct AVL_DVBSx_PllConf * )(pll_conf+2), ucPatchData, pAVLChip);
	if( AVL_DVBSx_EC_OK !=r )
	{
		printf("IBase Initialization failed !\n");
		return (r);
	}
	AVL_DVBSx_IBSP_Delay(100);	  //Wait 100 ms to assure that the AVL_DVBSx chip boots up.This function should be implemented by customer.

	//This function should be called to verify the AVL_DVBSx chip has completed its initialization procedure.
       r = AVL_DVBSx_IBase_GetStatus(pAVLChip);
	if( AVL_DVBSx_EC_OK != r )
	{
		printf("Booted failed !\n");
		return (r);
	}
       printf("Booted !\n");

    //This function should be called after IBase initialized to initialize the demod.
	r = AVL_DVBSx_IRx_Initialize(pAVLChip);
	if(AVL_DVBSx_EC_OK != r)
	{
		printf("Demod Initialization failed !\n");
		return (r);
	}
	//This function should be called after demod initialized to set RF AGC polar.
	//User does not need to setup this for Sharp tuner since it is the default value. But for other tuners, user may need to do it here.
#if 0//(FRONTEND_TUNER_TYPE == TUNER_TDA20152)
    r = AVL_DVBSx_IRx_SetRFAGCPola(AVL_DVBSx_RA_Normal, pAVLChip);
#else
    r = AVL_DVBSx_IRx_SetRFAGCPola(AVL_DVBSx_RA_Invert, pAVLChip);
#endif
	if(AVL_DVBSx_EC_OK != r)
    {
        printf("Set RF AGC Polar failed !\n");
        return (r);
    }

    //Setup MPEG mode parameters.
    //This function should be called after demod initialized to set MPEG mode.
    r = AVL_DVBSx_SetMpegMode(TS_PARALLEL_OUTPUT,0);
    if(AVL_DVBSx_EC_OK != r)
    {
        printf("Set MPEG output mode failed !\n");
        return (r);
    }
    //modify rx_config.aagc_ref
    r |= AVL_DVBSx_II2C_Write16(pAVLChip, rc_aagc_ref_addr, 48);
    r |= AVL_DVBSx_IRx_DriveRFAGC(pAVLChip);
    if(AVL_DVBSx_EC_OK != r)
    {
        printf("Driver RF AGC failed !\n");
        return (r);
    }
    //Setup tuner parameters for tuner initialization.
    pTuner->m_uiSlaveAddress = tuner_slave_address;
    pTuner->m_uiI2CBusClock_kHz = tuner_I2Cbus_clock;
    pTuner->m_pParameters = 0;
    pTuner->m_pAVLChip = pAVLChip;
#if 0//(FRONTEND_TUNER_TYPE == TUNER_SHARP_BS2S7HZ6306)
    pTuner->m_pInitializeFunc = &ExtSharpBS2S7HZ6306_Initialize;
    pTuner->m_pLockFunc = &ExtSharpBS2S7HZ6306_Lock;
    pTuner->m_pGetLockStatusFunc = &ExtSharpBS2S7HZ6306_GetLockStatus;
#endif
#if (FRONTEND_TUNER_TYPE == TUNER_AV2011)
    pTuner->m_pInitializeFunc = &AVL_DVBSx_ExtAV2011_Initialize;
    printf("\n===%s===%d====\n",__FUNCTION__,__LINE__);

    pTuner->m_pLockFunc = &AVL_DVBSx_ExtAV2011_Lock;
    pTuner->m_pGetLockStatusFunc = &AVL_DVBSx_ExtAV2011_GetLockStatus;
#endif
#if 0//(FRONTEND_TUNER_TYPE == TUNER_SHARP_BS2S7VZ7801)
   pTuner->m_pInitializeFunc = &ExtSharpBS2S7VZ7801_Initialize;
    pTuner->m_pLockFunc = &ExtSharpBS2S7VZ7801_Lock;
    pTuner->m_pGetLockStatusFunc = &ExtSharpBS2S7VZ7801_GetLockStatus;
#endif
#if 0//(FRONTEND_TUNER_TYPE == TUNER_TDA20152)
		pTuner->m_pInitializeFunc = &ExtTDA20152_Initialize;
		pTuner->m_pLockFunc = &ExtTDA20152_Lock;
		pTuner->m_pGetLockStatusFunc = &ExtTDA20152_GetLockStatus;
#endif

	//This function should be called after IBase initialized to initialize the tuner.
	r = pTuner->m_pInitializeFunc(pTuner);
	if(AVL_DVBSx_EC_OK != r)
	{
		printf("Tuner Initialization failed !\n");
		return (r);
	}
    printf("\n===%s===%d====\n",__FUNCTION__,__LINE__);

	//Setup Diseqc parameters for Diseqc initialization.
	sDiseqcPara.m_RxTimeout = AVL_DVBSx_DRT_150ms;
	sDiseqcPara.m_RxWaveForm = AVL_DVBSx_DWM_Normal;
	sDiseqcPara.m_ToneFrequency_kHz = Diseqc_Tone_Frequency;
	sDiseqcPara.m_TXGap = AVL_DVBSx_DTXG_15ms;
	sDiseqcPara.m_TxWaveForm = AVL_DVBSx_DWM_Normal;

	//The Diseqc should be initialized if AVL_DVBSx need to supply power to LNB. This function should be called after IBase initialized to initialize the Diseqc.
	r = AVL_DVBSx_IDiseqc_Initialize(&sDiseqcPara, pAVLChip);
    printf("\n===%s===%d====\n",__FUNCTION__,__LINE__);
	if(AVL_DVBSx_EC_OK != r)
	{
		printf("Diseqc Initialization failed !\n");
	}
	return (r);
}

static AVL_DVBSx_ErrorCode AVL_DVBSx_TuneChannel(AVL_uint16 uiFreq_MHz,AVL_uint32 uiSymbolRate_Hz)
{
       AVL_DVBSx_ErrorCode r=AVL_DVBSx_EC_OK;
       AVL_uchar u8CheckTimes = 10;
       struct AVL_DVBSx_Channel Channel;

      //Change the value defined by macro and go back here when we want to lock a new channel.
	pTuner->m_uiFrequency_100kHz = uiFreq_MHz*10;

      if(AVL_DVBSx_ITuner_CalculateLPF(uiSymbolRate_Hz/10000,pTuner) != AVL_DVBSx_EC_OK)
      {
	     pTuner->m_uiLPF_100kHz = tuner_LPF;
      }

#if (FRONTEND_TUNER_TYPE == TUNER_AV2011)
        pTuner->m_uiLPF_100kHz = uiSymbolRate_Hz/100000;
#endif
#if 0//(FRONTEND_TUNER_TYPE == TUNER_SHARP_BS2S7HZ6306)
        pTuner->m_uiLPF_100kHz = uiSymbolRate_Hz*75/10000000+40;
        if(pTuner->m_uiLPF_100kHz > 340)    pTuner->m_uiLPF_100kHz = 340;
#endif
#if 0//(FRONTEND_TUNER_TYPE == TUNER_SHARP_BS2S7VZ7801)
	pTuner->m_uiLPF_100kHz = uiSymbolRate_Hz*75/10000000+40;
        if(pTuner->m_uiLPF_100kHz > 340)    pTuner->m_uiLPF_100kHz = 340;
#endif

       signal_symbolrate = uiSymbolRate_Hz;

#if 0
       r=CPU_Halt(pAVLChip);
	if(AVL_DVBSx_EC_OK != r)
	{
		printf("CPU halt failed !\n");
		return (r);
	}
#endif

       //This function should be called after initialization to lock the tuner.
      if(pTuner->m_pLockFunc == NULL)
      {
           return AVL_DVBSx_EC_GeneralFail;
      }
	r = pTuner->m_pLockFunc(pTuner);
       if (AVL_DVBSx_EC_OK != r)
	{
 		printf("Set Tuner failed !\n");
		return (r);
	}

       while(u8CheckTimes --)
       {
            //This function should be called to check the lock status of the tuner.
            r = pTuner->m_pGetLockStatusFunc(pTuner);
            if (AVL_DVBSx_EC_OK == r)
            {
                 break;
            }
            AVL_DVBSx_IBSP_Delay(10);		//Wait a while for tuner to lock in certain frequency.
       }
       if( r != AVL_DVBSx_EC_OK)
       {
            //printf("Tuner test failed !\n");
            return (r);
       }

       Channel.m_uiSymbolRate_Hz = signal_symbolrate;
	Channel.m_Flags = (CI_FLAG_IQ_NO_SWAPPED) << CI_FLAG_IQ_BIT;	//Normal IQ
	Channel.m_Flags |= (CI_FLAG_IQ_AUTO_BIT_AUTO) << CI_FLAG_IQ_AUTO_BIT;	//Enable automatic IQ swap detection
	Channel.m_Flags |= (CI_FLAG_DVBS2_UNDEF) << CI_FLAG_DVBS2_BIT;			//Enable automatic standard detection

	//This function should be called after tuner locked to lock the channel.
	r = AVL_DVBSx_IRx_LockChannel(&Channel, pAVLChip);
	if (AVL_DVBSx_EC_OK != r)
	{
		printf("Lock channel failed !\n");
		return (r);
	}
    return (r);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_SetFrequency(AVL_uint16 uiFreq_MHz,AVL_uint32 uiSymbolRate_Hz)
{
       AVL_DVBSx_ErrorCode r=AVL_DVBSx_EC_OK;

       _bTuneError = 0;
       if(AVL_DVBSx_TuneChannel(uiFreq_MHz,uiSymbolRate_Hz) != AVL_DVBSx_EC_OK)
       {
               _bTuneError = 1;
       }
       else
       {
             AVL_DVBSx_IBSP_Delay(50);
       }

	  // Before we call function AVL_DVBSx_IDVBSxRx_GetPER,function A1108_IABSRx_ResetErrorStat should be called .
	  // Do NOT call AVL_DVBSx_IDVBSxRx_ResetErrorStat before AVL1108 chip lock a signal. It may lock the AVL_DVBSx.
	  r=AVL_DVBSx_IRx_ResetErrorStat(pAVLChip);
	  if (AVL_DVBSx_EC_OK != r)
	  {
		printf("Reset error status failed !\n");
		return (r);
       }

       return (r);
}


AVL_DVBSx_ErrorCode AVL_DVBSx_GetLockStatus(void)
{
       AVL_DVBSx_ErrorCode r=AVL_DVBSx_EC_OK;
       AVL_uint16 uiLockStatus=0;

		r = AVL_DVBSx_IRx_GetLockStatus(&uiLockStatus, pAVLChip);
	    if ((AVL_DVBSx_EC_OK != r)||(1 != uiLockStatus))
	    {
		   return (AVL_DVBSx_EC_GeneralFail);
	    }
printf("\nAVL_DVBSx_GetLockStatus = LOCKED\n");
	return (AVL_DVBSx_EC_OK);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_GetPER(float *pfPER)
{
     AVL_DVBSx_ErrorCode r=AVL_DVBSx_EC_OK;
     AVL_uint32 uiPER;

	//This function can be called to read back the current PER calculation result after function AVL_DVBSx_IDVBSxRx_ResetErrorStat called.
	r=AVL_DVBSx_IRx_GetPER(&uiPER, pAVLChip);
    if (AVL_DVBSx_EC_OK != r)
    {
       uiPER = 0;
    	printf("Get PER --- Fail !\n");
    }
    *pfPER = (float)(uiPER*1.0e-9);
    return (r);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_GetBER(float *pfBER)
{
     AVL_DVBSx_ErrorCode r=AVL_DVBSx_EC_OK;
     AVL_uint32 uiBER;

    //This function can be called to read back the current BER calculation result after function AVL_DVBSx_IDVBSxRx_ResetErrorStat called.
	r=AVL_DVBSx_IRx_GetDVBSBER(&uiBER, pAVLChip);
    if (AVL_DVBSx_EC_OK != r)
    {
       uiBER = 0;
    	printf("Get BER --- Fail !\n");
    }
    *pfBER = (float)(uiBER*1.0e-9);
    return (r);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_GetSNR(AVL_int32 *pfSNR)
{
     AVL_DVBSx_ErrorCode r=AVL_DVBSx_EC_OK;
     AVL_uint32 uiSNR;

    if(_bTuneError)
    {
         *pfSNR = 0;
         return AVL_DVBSx_EC_GeneralFail;
    }
    //This function can be called to read back the current SNR estimate after the channel locked and some waiting time.
	r=AVL_DVBSx_IRx_GetSNR(&uiSNR, pAVLChip);
    if (AVL_DVBSx_EC_OK != r)
    {
		printf("Get SNR --- Fail !\n");
        uiSNR = 0;
    }
    *pfSNR = (uiSNR/75);
    return (r);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_GetRFOffset(AVL_int16 *piRFOff)
{
       AVL_DVBSx_ErrorCode r=AVL_DVBSx_EC_OK;
       AVL_int16 iRFOff;

       //This function can be called to get the RF frequency offset after the channel locked.
	   r=AVL_DVBSx_IRx_GetRFOffset(&iRFOff, pAVLChip);
	   if (AVL_DVBSx_EC_OK != r)
	   {
		    printf("Get RFOffset --- Fail !\n");
            iRFOff = 0;
	   }
       *piRFOff = iRFOff/10;
        return (r);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_GetSinalLevel(AVL_int16 *piRFSignalLevel)
{
    AVL_DVBSx_ErrorCode r=AVL_DVBSx_EC_OK;
    AVL_uint16 uiRFSignalLevel;
    AVL_int16 uiRFSignalDBM=0;
    AVL_int16 i;

    //This function can be called to get the RF signal level after the channel locked.
    r=AVL_DVBSx_IRx_GetSignalLevel(&uiRFSignalLevel, pAVLChip);
	if (AVL_DVBSx_EC_OK != r)
	{
		printf("Get SignalLevel --- Fail !\n");
        uiRFSignalDBM = 0;
    }
	else
	{
		for(i=0; i<47; i++)
		{
			if(uiRFSignalLevel<= SignalLevel[i].SignalLevel)
			{
				//Calculate the corresponding DBM value.
                            if((0==i)&&(uiRFSignalLevel< SignalLevel[i].SignalLevel))
				{
					printf("RFSignalLevel is too weak !");
				}
				else
				{
					uiRFSignalDBM=SignalLevel[i].SignalDBM;
				}
				break;
			}
		}
	}
       *piRFSignalLevel = (float)(uiRFSignalDBM/10.0);

        return (r);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_GetSignalInfo(struct AVL_DVBSx_SignalInfo *pSignalInfo)
{
       AVL_DVBSx_ErrorCode r=AVL_DVBSx_EC_OK;
       struct AVL_DVBSx_SignalInfo SignalInfo;

       //This function can be called to get the current locked signal information after the channel locked.
	   r=AVL_DVBSx_IRx_GetSignalInfo(&SignalInfo, pAVLChip);
       *pSignalInfo = SignalInfo;

       return (r);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_Blind_start(AVL_int16 u16StartFreq,AVL_int16 u16EndFreq)
{
          AVL_DVBSx_ErrorCode r=AVL_DVBSx_EC_OK;

          bs_start_freq = u16StartFreq;
          bs_stop_freq = u16EndFreq;
          r = AVL_DVBSx_IBase_SetFunctionalMode(pAVLChip,AVL_DVBSx_FunctMode_BlindScan);
          if(AVL_DVBSx_EC_OK != r)
          {
                return (r);
          }
          //Setup blind scan parameters and start to perform blind scan.
          bsPara.m_uiMaxSymRate_kHz = 1000*bs_max_symbolrate;
          bsPara.m_uiMinSymRate_kHz = 1000*bs_min_symbolrate;
	   //r = AVL_DVBSx_II2C_Read16(pAVLChip, rc_tuner_max_LPF_100kHz_addr, &uiMaxLPF);     //Get the right LPF for blind scan to regulate the freq_step.

          uiNextStartFreq_100kHz = 10*bs_start_freq;

          // The Sharp tuner does not invert the spectrum.  For other tuners, this may have to be set to AVL_DVBSx_BS_INVERT.
          r = AVL_DVBSx_IBlindScan_SetSpectrumInversion(pAVLChip, AVL_DVBSx_BS_NORMAL);
          if(AVL_DVBSx_EC_OK != r)
          {
                printf("Set blindscan spectrum inversion failed !\n");
                return (r);
          }
#if 0
          //Halt CPU to improve the tuner locking speed. It's the optional operation.
           r=CPU_Halt(pAVLChip);
           if(AVL_DVBSx_EC_OK != r)
           {
                printf("CPU halt failed !\n");
                return (r);
           }
#endif
           r = AVL_DVBSx_IBlindScan_Reset(pAVLChip);
           return(r);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_Blind_NextFreqBlindScan(AVL_uchar* u8BlindScanEnd)
{
       AVL_DVBSx_ErrorCode r=AVL_DVBSx_EC_OK;
       unsigned char u8CheckTimes = 10;

       *u8BlindScanEnd = 0;
       bsPara.m_uiStartFreq_100kHz = uiNextStartFreq_100kHz;    // Give the start scan frequency.
     	if(bsPara.m_uiStartFreq_100kHz >= bs_stop_freq*10)
     	{
     	    *u8BlindScanEnd = 1;
     		return AVL_DVBSx_EC_OK;			//Do not need to do scan
     	}
     	//Sharp tuner has a maximum 34 MHz LPF, but only 32MHz is flat.
     	//The scan frequency range is 640(32MHz*2*10),unit is 100KHz.
	bsPara.m_uiStopFreq_100kHz = bsPara.m_uiStartFreq_100kHz + 320*2;

     	if(bsPara.m_uiStopFreq_100kHz > bs_stop_freq*10)
     	{
     		bsPara.m_uiStopFreq_100kHz = bs_stop_freq*10;
            uiNextStartFreq_100kHz = bs_stop_freq*10;
     	}

     	//Setup the tuner before scan
     	pTuner->m_uiFrequency_100kHz = (bsPara.m_uiStartFreq_100kHz + bsPara.m_uiStopFreq_100kHz)/2;  //Calculate the center frequency.
#if (FRONTEND_TUNER_TYPE == TUNER_AV2011)
      pTuner->m_uiLPF_100kHz = 440;
#endif
#if 0//(FRONTEND_TUNER_TYPE == TUNER_SHARP_BS2S7HZ6306)
      pTuner->m_uiLPF_100kHz = 340;
#endif
#if 0//(FRONTEND_TUNER_TYPE == TUNER_SHARP_BS2S7VZ7801)
	pTuner->m_uiLPF_100kHz = 340;
#endif

     	r = pTuner->m_pLockFunc(pTuner);   //Lock the tuner.
     	if(AVL_DVBSx_EC_OK != r)
      {
          printf("Tuner lock failed !\n");
          return (r);
      }

      while(u8CheckTimes --)
      {
           //This function should be called to check the lock status of the tuner.
           r = pTuner->m_pGetLockStatusFunc(pTuner);
           if (AVL_DVBSx_EC_OK == r)
           {
                break;
           }
           AVL_DVBSx_IBSP_Delay(10);       //Wait a while for tuner to lock in certain frequency.
      }
          //This function should be called to check the lock status of the tuner.
   	r = pTuner->m_pGetLockStatusFunc(pTuner);
   	if (AVL_DVBSx_EC_OK != r)
   	{
   	      //printf("Tuner test failed !\n");
       	return (r);
   	}

     	//This function should be called after tuner lock to perform a blind scan.
       r = AVL_DVBSx_IBlindScan_Scan(&bsPara, 340, pAVLChip);
       if(AVL_DVBSx_EC_OK != r)
       {
   	       printf("Blind Scan failed !\n");
   		return (r);
       }
       ucBlindWaitCount = 10;

       return (r);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_Blind_WaitCurFreqFinished(AVL_uchar* u8Progress,AVL_uchar* ucFindNum)
{
      AVL_DVBSx_ErrorCode r=AVL_DVBSx_EC_OK;

      *u8Progress = 0;
      *ucFindNum = 0;
      //Monitor the blind scan progress.
      AVL_DVBSx_IBSP_Delay(100);
      r = AVL_DVBSx_IBlindScan_GetScanStatus(&bsInfo, pAVLChip);  //Query the internal blind scan procedure information.
      if(AVL_DVBSx_EC_OK != r || bsInfo.m_uiResultCode >0)
      {
  	       printf("Get scan status failed !\n");
              uiNextStartFreq_100kHz = bs_stop_freq*10;   //to end blind scan
              *u8Progress = 100;
      }
      else if(100 == bsInfo.m_uiProgress)
      {
             *ucFindNum = bsInfo.m_uiChannelCount;
             if(uiNextStartFreq_100kHz < bs_stop_freq*10)
             {
                   uiNextStartFreq_100kHz = bsInfo.m_uiNextStartFreq_100kHz;
             }
             *u8Progress = 100;
      }
      else
      {
           *u8Progress = (bsInfo.m_uiProgress >100)?100:(AVL_uchar)bsInfo.m_uiProgress;
      }

      return (r);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_Blind_GetChannel(AVL_uint16 u16Start,AVL_uint16 *chNum,struct AVL_DVBSx_Channel *channels)
{
    AVL_DVBSx_ErrorCode r=AVL_DVBSx_EC_OK;

      //The blind scan is finished now, read out the channel information
     r = AVL_DVBSx_IBlindScan_ReadChannelInfo(u16Start, chNum, channels, pAVLChip);
     if(AVL_DVBSx_EC_OK != r)
     {
           *chNum = 0;
     }

     return(r);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_Blind_Cancel(void)
{
     AVL_DVBSx_ErrorCode r=AVL_DVBSx_EC_OK;

     r =  AVL_DVBSx_IBlindScan_Cancel(pAVLChip);
     return (r);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_Blind_End(void)
{
     AVL_DVBSx_ErrorCode r=AVL_DVBSx_EC_OK;

     uiNextStartFreq_100kHz = 10*bs_start_freq;
     r = AVL_DVBSx_IBase_SetFunctionalMode(pAVLChip,AVL_DVBSx_FunctMode_Demod);
     return (r);
}

AVL_uint32 AVL_DVBSx_Blind_GetCurrentFreq(void)
{
      return uiNextStartFreq_100kHz /10;
}
#endif
