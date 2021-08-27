//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------
#include <frontend_config.h>

#if (FRONTEND_DEMOD_TYPE == DEMOD_ATBM885x)
#include "../../../../include/prana2/stdio.h"
#include <sys/common/MsCommon.h>
#include <sys/drv/drvHWI2C.h>
#include <sys/drv/drvGPIO.h>
#include "../../../../include/device/drvDemod.h" //Temp for use

#include <sys/common/MsOS.h>
#include "device/drvTuner_MXL5007T.h"
#include <linux/string.h>


#include "atbm885x.h"

#ifdef MS_DEBUG
#define DBG_MSB(x)  x
#else
#define DBG_MSB(x)
#endif

//-------------------------------------------------------------------------------------------------
// Local Defines
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Local Defines
//-------------------------------------------------------------------------------------------------
//#define DVBC_MSEC_LOOP              ( CPU_CLOCK_FREQ/1000/4 )
//#define DVBC_DELAY(_loop)           { volatile int i; for (i=0; i<(_loop)*DVBC_MSEC_LOOP; i++); }
#define COFDMDMD_STACK_SIZE          (2048)
#define COFDMDMD_MUTEX_TIMEOUT       (2000)
#define COFDMDMD_AUTO_TUN_MAX        (2 * 5)
#define COFDMDMD_AUTO_TUN_RETRY_MAX  (2)

#define CHG_COFDMDMD_EVENT(_event)    	MsOS_ClearEvent(_s32EventId, 0xFFFFFFFF);   \
                                        MsOS_SetEvent(_s32EventId, _event);
//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
typedef enum _COFDMDMD_Event
{
    E_COFDMDMD_EVENT_RUN     = 0x0000001,
} __attribute__((packed)) CCOFDMDMD_Event;



//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
//static void*                        _pTaskStack;
static MS_S32                       _s32TaskId 	= -1;
static MS_S32                       _s32MutexId;
//static MS_S32                       _s32EventId;
//static COFDMDMD_TunState             _eTunState;
//static MS_BOOL                      _bTuned;
static MS_BOOL 	 					bInited     = FALSE;
//static MS_U32						u32CurrentTime = 0;

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
//MS_U8   u8DemodAddr = DEMOD_SLAVE_ID;

MS_U32 SysDelay(MS_U32 dwMs)
{
    if (bInited)
    {
        MsOS_DelayTask(dwMs);
    }
    return (0);
}


//-------------------------------------------------------------------------------------------------
// Global Functions
//-------------------------------------------------------------------------------------------------
#if 0
static void _Demod_HwReset(void)
{
    extern void MApi_Demod_HWReset(MS_BOOL bReset);
    DBG_MSB(printf("---> Hw Rest for DEMOD \n"));
    //MApi_Demod_HWReset(TRUE);
    SysDelay(20);
    //MApi_Demod_HWReset(FALSE);
    SysDelay(100);
}
#endif


static MS_BOOL _Demod_Init(void)
{
    MS_U32      dwError = 0;

    DBG_MSB(printf("-----%s start-----\n", __FUNCTION__));
    DemodHardwareReset();
    //ATBMInit();
    ATBMInit_DTMB_Mode();
   // if(!ATBMInit())
    {
   //     DMD_MSG(printf("ZL10353_Initialise failed"));
    }
    ATBMChipID();

    DBG_MSB(printf("-----%s done-----\n", __FUNCTION__));
    return (dwError ) ? FALSE : TRUE;
}

/*
static void _COFDMDMD_Task(void)
{
    MS_U32   u32Events;

    while (1)
    {
        //[TODO] Does waitevent do OS scheduling?
        MsOS_WaitEvent(_s32EventId, E_COFDMDMD_EVENT_RUN, &u32Events, E_OR, MSOS_WAIT_FOREVER);

        if (MsOS_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
        {
            DMD_ERR(printf("_COFDMDMD_Task: Obtain mutex failed.\n");)
        }
        else
        {
            // Read event in mutex
            MsOS_WaitEvent(_s32EventId, E_COFDMDMD_EVENT_RUN, &u32Events, E_OR_CLEAR, 0);

            if (u32Events & E_COFDMDMD_EVENT_RUN)
            {
                //_COFDMDMD_AutoTun_State();
            }

            MsOS_ReleaseMutex(_s32MutexId);
        }

        MsOS_DelayTask(5);

    }
}
*/
//Mstar Cofdm comman interface
MS_BOOL MDrv_Demod_Init(void)
{
    bInited = FALSE;
    DBG_MSB(printf("-----%s start-----\n", __FUNCTION__));

    if (_s32TaskId >= 0)
    {
        return TRUE;
    }

    _s32MutexId = MsOS_CreateMutex(E_MSOS_FIFO, "OfDmd_Mutex",MSOS_PROCESS_SHARED);

    if (_s32MutexId < 0)
    {
        GEN_EXCEP;
        return FALSE;
    }

    //Connect(DTV_Get_Demod_TYPE());
    _Demod_Init();
    printf("/////////////////////////////////////////////////\n\n\n\n");
    printf("%s\n",__FUNCTION__);
    printf("/////////////////////////////////////////////////\n");
    bInited = TRUE;
    DBG_MSB(printf("-----%s done-----\n", __FUNCTION__));
    return TRUE;
}


MS_BOOL MDrv_Demod_Open(void)
{
    if (MsOS_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    else
    {
    }

    MsOS_ReleaseMutex(_s32MutexId);
    return TRUE;

}

MS_BOOL MDrv_Demod_Close(void)
{
    return TRUE;
}

MS_BOOL MDrv_Demod_Reset(void)
{
    if (MsOS_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    else
    {
    }

    MsOS_ReleaseMutex(_s32MutexId);
    return TRUE;
}

MS_BOOL MDrv_Demod_TsOut(MS_BOOL bEnable)
{
    if (MsOS_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    else
    {
    }

    MsOS_ReleaseMutex(_s32MutexId);
    return TRUE;
}

MS_BOOL MDrv_Demod_PowerOnOff(MS_BOOL bPowerOn)
{
    if (MsOS_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    else
    {
    }

    MsOS_ReleaseMutex(_s32MutexId);
    return TRUE;
}

MS_BOOL MDrv_Demod_SetBW(MS_U32 u32BW)
{
    if (MsOS_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    else
    {
    }

    MsOS_ReleaseMutex(_s32MutexId);
    return TRUE;
}

MS_BOOL MDrv_Demod_GetBW(MS_U32 *pu32BW)
{
    if (MsOS_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    else
    {
    }

    MsOS_ReleaseMutex(_s32MutexId);
    return TRUE;

}

MS_BOOL MDrv_Demod_GetLock(MS_BOOL *pbLock)
{
    if (MsOS_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    else
    {

		if(ATBMLockedFlag()==1)
		{
			*pbLock = TRUE;
		}
		else
		{
			*pbLock = FALSE;
		}

#if 0
		MS_U8 SignalJitterNum=7;
		while(SignalJitterNum)
		{
			if(ATBMLockedFlag()==1)
			{
				*pbLock = TRUE;
				break;
			}
			else
			{
				*pbLock = FALSE;
			}
			SignalJitterNum--;
			MsOS_DelayTask(20);
	 	}
#endif

    }

    MsOS_ReleaseMutex(_s32MutexId);
    return TRUE;
}

MS_BOOL MDrv_Demod_GetSNR(MS_U32 *pu32SNR)
{
    if (MsOS_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    else
    {
        if (ATBMLockedFlag()==1)
        {
            *pu32SNR = (MS_U32)ATBMSignalQuality();
        }
        else
        {
            *pu32SNR = 0;
        }
    }

    MsOS_ReleaseMutex(_s32MutexId);
    return TRUE ;
}

MS_BOOL MDrv_Demod_GetBER(float *pfBER)
{
    MS_U32              dwError = 0;

    if (MsOS_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    else
    {
    }

    MsOS_ReleaseMutex(_s32MutexId);
    return (dwError ) ? FALSE : TRUE;
}

MS_BOOL MDrv_Demod_GetPWR(MS_S32 *ps32Signal)
{
    #define DEFAULT_PWR_MIN (-97)
    #define DEFAULT_PWR_MAX (-5)

    MS_U16   wSignal = 0;

    if (MsOS_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    else
    {
        if (ATBMLockedFlag()==1)
        {
            wSignal = ATBMSignalStrength();
			//0~1023, the bigger value means the signal is more weak
			if(wSignal <352)
            {
				*ps32Signal = 80;
			}
			else if(wSignal <492)
			{
				*ps32Signal = (u_int16)((532 - wSignal)/2);
            }
            else
            {
				*ps32Signal = 0;
            }
        }
        else
        {
            *ps32Signal = 0;
        }
    }
	printf("wSignal %d\n", wSignal);
    MsOS_ReleaseMutex(_s32MutexId);
    return  TRUE;
}

MS_BOOL MDrv_Demod_Config(MS_U8 *pRegParam)
{
    if (MsOS_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    else
    {

    }
    MsOS_ReleaseMutex(_s32MutexId);
    return TRUE;

}
MS_BOOL MDrv_Demod_GetParam(DEMOD_MS_FE_CARRIER_PARAM* pParam)
{
    if (MsOS_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    else
    {
    }
    MsOS_ReleaseMutex(_s32MutexId);
    return TRUE;
}

MS_BOOL MDrv_Demod_Restart(DEMOD_MS_FE_CARRIER_PARAM* pParam)
{
    MS_U32  			dwError = 0;

    if (MsOS_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(printf("MDrv_COFDMDMD_Restart: Obtain mutex failed.\n"));
        return FALSE;
    }
    else
    {
        DBG_MSB(printf("\r\n u32TunerFreq = %ld u8BandWidth = %d \n",pParam->u32Frequency,pParam->TerParam.eBandWidth));
        ATBM_I2CByPassOn();
        MsOS_DelayTask(100);
        if(MDrv_Tuner_Init()==FALSE) 
           printf("=====================TUNER  FAIL=================\n"); //Bruce 2011-8-15 12:8
        dwError = MDrv_Tuner_SetTuner( pParam->u32Frequency, pParam->TerParam.eBandWidth);
       //if(dwError == FALSE)
          //printf("set tuner fail\n");
        MsOS_DelayTask(200);
        ATBM_I2CByPassOff();

       // ATBMReset(CRYSTAL);//???or OSCILLATOR
       //  MsOS_DelayTask(200);
	    //ZL10353_FSM_Restart();
	   // ZL10353_BW_Programming(pParam->u8BandWidth);
    }

    MsOS_ReleaseMutex(_s32MutexId);
    //  return (dwError ) ? FALSE : TRUE;
    return TRUE;

}

MS_BOOL MDrv_Demod_SetMode(Demod_Mode* pMode)
{
    if (MsOS_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    else
    {
    }
    MsOS_ReleaseMutex(_s32MutexId);
    return TRUE;

}

MS_BOOL MDrv_Demod_I2C_ByPass(MS_BOOL bOn)
{
	//Alex
	printf("Demod by pass\n");
    if(bOn)
	{
        ATBM_I2CByPassOn();
	}
	else
	{
	    ATBM_I2CByPassOff();
	}
    return TRUE;
}

MS_BOOL MDrv_Demod_SetTsSerial(MS_BOOL bSerial) //bSerial 1: Ts Serial ouput 0: Ts Parallel output /*MSD5021:ts output is parrel in CI, but is serial in non ci*/
{
    if (MsOS_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(printf("MDrv_COFDMDMD_Restart: Obtain mutex failed.\n"));
        return FALSE;
    }
    else
    {
    }

    MsOS_ReleaseMutex(_s32MutexId);
    return TRUE;
}


MS_BOOL MDrv_Demod_SetOutoutPath(DEMOD_INTERFACE_MODE path)
{
    //Dummy function
    return TRUE;
}

DEMOD_INTERFACE_MODE MDrv_Demod_GetOutoutPath(void)
{
    //Dummy function
    return DEMOD_INTERFACE_PARALLEL;
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

MS_U16  MApi_FE_GetSignalQuality(void)
{
    MS_U32 u32SNR;

    if (MDrv_Demod_GetSNR(&u32SNR) != TRUE)
    {
        u32SNR = 0;
    }
    return (MS_U16)u32SNR; 
}

MS_U32  MApi_FE_GetSNR(void)
{
    MS_U32 u32snr = 0;
    MS_BOOL ret = 0;

    ret = MDrv_Demod_GetSNR(&u32snr);
    if(ret == FALSE)
    {
        //printf("Get SNR Failed \n");
    }
    return u32snr;
}



void MApi_FE_Power_On_Initialization(void)
{
    //printf("ENTRY, [%s] at <%s> \n", __FUNCTION__, __FILE__);

    if (MDrv_Demod_Init() == FALSE)
    {
        //printf("%s: Init demodulator fail\n", __FUNCTION__);
    }

}

static DEMOD_MS_FE_CARRIER_PARAM	s_tunerParam;
MS_BOOL MApi_FE_SetFECarrierParam(DEMOD_MS_FE_CARRIER_PARAM *pParam)
{
    memcpy(&s_tunerParam, pParam, sizeof(DEMOD_MS_FE_CARRIER_PARAM));
    return TRUE;
}

void MApi_FE_SetFrequency(MS_U32 u32Frequency,MS_U8 eBandWidth)
{
    DEMOD_MS_FE_CARRIER_PARAM DemoParam;

    DemoParam.u32Frequency  = u32Frequency;
    DemoParam.TerParam.eBandWidth  = eBandWidth;//u16BandWidth / 1000;
    
//    s_tunerParam.u32Frequency  = u32Frequency;
//    s_tunerParam.TerParam.eBandWidth  = eBandWidth;//u16BandWidth / 1000;

//    if (MDrv_Demod_Restart(&s_tunerParam) == FALSE)
    if (MDrv_Demod_Restart(&DemoParam) == FALSE)
    {
        
    }

}

#endif


