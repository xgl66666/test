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


#include <frontend_config.h>

#if (FRONTEND_DEMOD_TYPE == DEMOD_AVL6211)

#include "AVL_DVBSx_Board.h"
#include "../../../../include/device/drvDemod.h" //Temp for use
#include "AVL_DVBSx_Func.h"
#include "sys/common/MsOS.h"
//#include <sys/common/MsOS.h>
#include <sys/common/MsTypes.h>
#include <sys/drv/drvGPIO.h>
#include <sys/common/MsCommon.h>
#include <sys/drv/drvHWI2C.h>
#include "../../../../include/prana2/stdio.h"
#include <sys/common/MsOS.h>

#ifdef MS_DEBUG
#define DMD_ERR(_print)	     			_print
#define DMD_MSG(_print)      			_print
#else
#define DMD_ERR(_print)	     			_print
#define DMD_MSG(_print)      			_print
#endif
#define FE_INFO printf
//extern void MApi_Demod_HWReset(void);

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define COFDMDMD_MUTEX_TIMEOUT       (2000)

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
static MS_S32                       _s32TaskId 	= -1;
static MS_S32                       _s32MutexId;
static MS_BOOL 	 		bInited     = FALSE;
#define GEN_EXCEP   { while(1); }
#define MAX_TPS_LPF            10  //DVBS_STYLE
#define GPIO_FE_RST                     16
#define PIU_IIC_0                   0x0010
#define FRONTEND_TUNER_CTRL         PIU_IIC_0
#define FRONTEND_TUNER_PORT         E_HWI2C_PORT0_1
MS_U32 SysDelay(MS_U32 dwMs)
{
    if (bInited)
    {
        MsOS_DelayTask(dwMs);
    }
    else
    {
        MS_U32 volatile u32Dummy = 0;
        MS_U32 volatile u32Loop;
        u32Loop = 40000*dwMs;
        while (u32Loop-- > 0)
        {
            u32Dummy++;
        }
    }
    return (0);
}

void MApi_Demod_HWReset(void)
{
     mdrv_gpio_set_high(GPIO_FE_RST);
     SysDelay(100);
     mdrv_gpio_set_low(GPIO_FE_RST);
     SysDelay(200);
     mdrv_gpio_set_high(GPIO_FE_RST);
     SysDelay(100);
}
MS_BOOL MDrv_Demod_SetTsSerial(MS_BOOL bSerial)
{
    AVL_DVBSx_ErrorCode errRet;
    if (MsOS_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(printf("%s: Obtain mutex failed.\n", __FUNCTION__);)
        return FALSE;
    }
    else
    {
    }
    errRet = AVL_DVBSx_SetMpegMode(!bSerial,1);
    MsOS_ReleaseMutex(_s32MutexId);  //must release mutex before return
    if(errRet != AVL_DVBSx_EC_OK)
    {
          return FALSE;
    }
    return TRUE;
}

//Mstar Cofdm comman interface
MS_BOOL MDrv_Demod_Init(void)
{
    AVL_DVBSx_ErrorCode errRet;
    DMD_MSG(printf("[+]MDrv_CofdmDmd_Init\n"));
    bInited = FALSE;

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
    MApi_Demod_HWReset();
#if (FRONTEND_TUNER_CTRL == PIU_IIC_1)
    MDrv_HWI2C_SetReadModeP1(E_HWI2C_READ_MODE_DIRECT);
#else
    MDrv_HWI2C_SetReadMode(E_HWI2C_READ_MODE_DIRECT);
#endif
    errRet = AVL_DVBSx_Initialize();
    bInited = TRUE;
    DMD_MSG( printf("[-]MDrv_CofdmDmd_Init\n"));

    return TRUE;
}


MS_BOOL MDrv_Demod_Open(void)
{
    if (MsOS_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(printf("%s: Obtain mutex failed.\n", __FUNCTION__);)
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
    if (MsOS_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(printf("%s: Obtain mutex failed.\n", __FUNCTION__);)
        return FALSE;
    }

    MsOS_ReleaseMutex(_s32MutexId);
    return TRUE;
}


MS_BOOL MDrv_Demod_Reset(void)
{
    if (MsOS_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(printf("%s: Obtain mutex failed.\n", __FUNCTION__);)
        return FALSE;
    }
    else
    {
    }
    // Redo Init??

    MsOS_ReleaseMutex(_s32MutexId);
    return TRUE;
}

MS_BOOL MDrv_Demod_Restart(DEMOD_MS_FE_CARRIER_PARAM *pParam)
{
    if (MsOS_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(printf("MDrv_COFDMDMD_Restart: Obtain mutex failed.\n");)
        return FALSE;
    }
    else
    {
        // Lock channel
        printf("\r\n u32TunerFreq = %d u32SymbolRate = %d XharryX",pParam->u32Frequency,pParam->SatParam.u32SymbolRate);
	 	AVL_DVBSx_SetFrequency(pParam->u32Frequency,pParam->SatParam.u32SymbolRate);
    }

    MsOS_ReleaseMutex(_s32MutexId);
    return TRUE;

}

MS_BOOL MDrv_Demod_SetMode(Demod_Mode *pMode)
{
    if (MsOS_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(printf("%s: Obtain mutex failed.\n", __FUNCTION__);)
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
        DMD_ERR(printf("%s: Obtain mutex failed.\n", __FUNCTION__);)
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
        DMD_ERR(printf("%s: Obtain mutex failed.\n", __FUNCTION__);)
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
        DMD_ERR(printf("%s: Obtain mutex failed.\n", __FUNCTION__);)
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
        DMD_ERR(printf("%s: Obtain mutex failed.\n", __FUNCTION__);)
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
        DMD_ERR(printf("%s: Obtain mutex failed.\n", __FUNCTION__);)
        return FALSE;
    }
    // get lock
    if( AVL_DVBSx_GetLockStatus() == AVL_DVBSx_EC_OK)
    {
          *pbLock = TRUE;
    }
    else
    {
          *pbLock = FALSE;
    }

    MsOS_ReleaseMutex(_s32MutexId);
    return TRUE;
}

MS_BOOL MDrv_Demod_TPSGetLock(MS_BOOL *pbLock)
{
    if (MsOS_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(printf("%s: Obtain mutex failed.\n", __FUNCTION__);)
        return FALSE;
    }
    // get lock
    if( AVL_DVBSx_GetLockStatus() == AVL_DVBSx_EC_OK)
    {
          *pbLock = TRUE;
    }
    else
    {
          *pbLock = FALSE;
    }

    MsOS_ReleaseMutex(_s32MutexId);
    return TRUE ;
}


MS_BOOL MDrv_Demod_MPEGGetLock(MS_BOOL *pbLock)
{
    if (MsOS_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(printf("%s: Obtain mutex failed.\n", __FUNCTION__);)
        return FALSE;
    }
    if( AVL_DVBSx_GetLockStatus() == AVL_DVBSx_EC_OK)
    {
          *pbLock = TRUE;
    }
    else
    {
          *pbLock = FALSE;
    }

    MsOS_ReleaseMutex(_s32MutexId);
    return TRUE;
}

MS_BOOL MDrv_Demod_GetSNR(MS_U32 *pu32SNR)
{
    if (MsOS_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(printf("%s: Obtain mutex failed.\n", __FUNCTION__);)
        return FALSE;
    }
    if( AVL_DVBSx_GetLockStatus() == AVL_DVBSx_EC_OK)
    {
          if(AVL_DVBSx_GetSNR((int*)pu32SNR) != AVL_DVBSx_EC_OK)
          {
                 *pu32SNR = 0;
          }
    }
    else
    {
         *pu32SNR = 0;
    }
    MsOS_ReleaseMutex(_s32MutexId);
    return TRUE;
}

MS_BOOL MDrv_Demod_GetBER(float *pfBER)
{
    if (MsOS_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(printf("%s: Obtain mutex failed.\n", __FUNCTION__);)
        return FALSE;
    }
    if( AVL_DVBSx_GetLockStatus() == AVL_DVBSx_EC_OK)
    {
          if(AVL_DVBSx_GetBER(pfBER) != AVL_DVBSx_EC_OK)
          {
                 *pfBER = 0;
          }
    }
    MsOS_ReleaseMutex(_s32MutexId);
    return TRUE;

}

MS_BOOL MDrv_Demod_GetPWR(MS_S32 *ps32Signal)
{
    if (MsOS_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(printf("%s: Obtain mutex failed.\n", __FUNCTION__);)
        return FALSE;
    }
    if( AVL_DVBSx_GetLockStatus() == AVL_DVBSx_EC_OK)
    {
          MS_S16 s16Signallevel;
          AVL_DVBSx_GetSinalLevel(&s16Signallevel);
          *ps32Signal = s16Signallevel;
    }
    else
    {
           *ps32Signal = -97;
    }
    MsOS_ReleaseMutex(_s32MutexId);
    return TRUE;

}

MS_BOOL MDrv_Demod_GetRollOff(MS_U8 *pRollOff)
{
    struct AVL_DVBSx_SignalInfo SignalInfo;

    if (MsOS_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(printf("%s: Obtain mutex failed.\n", __FUNCTION__);)
        return FALSE;
    }
    if( AVL_DVBSx_GetSignalInfo(&SignalInfo) == AVL_DVBSx_EC_OK)
    {
          if(SignalInfo.m_rolloff == AVL_DVBSx_RO_35)
          {
                *pRollOff = 0;
          }
          else if(SignalInfo.m_rolloff == AVL_DVBSx_RO_35)
          {
                *pRollOff = 1;
          }
          else
          {
                *pRollOff = 2;
          }
    }

    MsOS_ReleaseMutex(_s32MutexId);
    return TRUE;
}

MS_BOOL MDrv_Demod_GetRFOffset(MS_S16 *ps16RFOff)
{
    MS_BOOL bRet = FALSE;
    if (MsOS_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(printf("%s: Obtain mutex failed.\n", __FUNCTION__);)
        return FALSE;
    }
    *ps16RFOff = 0;
    if( AVL_DVBSx_GetLockStatus() == AVL_DVBSx_EC_OK)
    {
            if( AVL_DVBSx_GetRFOffset(ps16RFOff) != AVL_DVBSx_EC_OK)
            {
                 *ps16RFOff = 0;
            }
            bRet = TRUE;
    }

    MsOS_ReleaseMutex(_s32MutexId);
    return bRet;

}

MS_BOOL MDrv_Demod_GetParam(DEMOD_MS_FE_CARRIER_PARAM *pParam)
{
    if (MsOS_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(printf("%s: Obtain mutex failed.\n", __FUNCTION__);)
        return FALSE;
    }
    else
    {
    }
    MsOS_ReleaseMutex(_s32MutexId);
    return TRUE;
}

MS_BOOL MDrv_Demod_Config(MS_U8 *pRegParam)
{
    if (MsOS_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(printf("%s: Obtain mutex failed.\n", __FUNCTION__);)
        return FALSE;
    }
    else
    {
    }
    MsOS_ReleaseMutex(_s32MutexId);
    return TRUE;
}

//-------------------------------------------------------------------------------------------------
//blind scan
//-----------------------------------------------------------------------------------------------
MS_BOOL MDrv_Demod_BlindScan_Start(MS_U16 u16StartFreq,MS_U16 u16EndFreq)
{
    MS_BOOL bRet = TRUE;
    if (MsOS_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(printf("%s: Obtain mutex failed.\n", __FUNCTION__);)
        return FALSE;
    }
    else
    {
         if(AVL_DVBSx_Blind_start(u16StartFreq,u16EndFreq) != AVL_DVBSx_EC_OK)
         {
               bRet =  FALSE;
         }
    }
    MsOS_ReleaseMutex(_s32MutexId);
    return bRet;
}

//[out]:MS_BOOL  == FALSE------>scan end
MS_BOOL MDrv_Demod_BlindScan_NextFreq(MS_BOOL* bBlindScanEnd)
{
    MS_BOOL bRet = TRUE;
    if (MsOS_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(printf("%s: Obtain mutex failed.\n", __FUNCTION__);)
        return FALSE;
    }
    else
    {
         MS_U8 u8End;
         if(AVL_DVBSx_Blind_NextFreqBlindScan(&u8End) != AVL_DVBSx_EC_OK)
         {
               bRet = FALSE;
         }
         if(u8End > 0)
         {
               *bBlindScanEnd = TRUE;
         }
         else
         {
               *bBlindScanEnd = FALSE;
         }
    }
    MsOS_ReleaseMutex(_s32MutexId);
    return bRet;
}

//MDrv_CofdmDmd_BlindScan_WaitFinished
//wait blind scan current freq finished,not ended blind scan
MS_BOOL MDrv_Demod_BlindScan_WaitCurFreqFinished(MS_U8* u8Progress,MS_U8 *u8FindNum)
{
    MS_BOOL bRet = TRUE;
    if (MsOS_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(printf("%s: Obtain mutex failed.\n", __FUNCTION__);)
        return FALSE;
    }
    else
    {
         if(AVL_DVBSx_Blind_WaitCurFreqFinished((MS_U8*)u8Progress,u8FindNum) != AVL_DVBSx_EC_OK)
         {
               bRet = FALSE;
         }
    }
    MsOS_ReleaseMutex(_s32MutexId);
    return bRet;
}

MS_BOOL MDrv_Demod_BlindScan_Cancel(void)
{
    MS_BOOL bRet = TRUE;
    if (MsOS_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(printf("%s: Obtain mutex failed.\n", __FUNCTION__);)
        return FALSE;
    }
    else
    {
         if(AVL_DVBSx_Blind_Cancel() != AVL_DVBSx_EC_OK)
         {
               bRet = FALSE;
         }
    }
    MsOS_ReleaseMutex(_s32MutexId);
    return bRet;
}

MS_BOOL MDrv_Demod_BlindScan_End(void)
{
    MS_BOOL bRet = TRUE;
    if (MsOS_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(printf("%s: Obtain mutex failed.\n", __FUNCTION__);)
        return FALSE;
    }
    else
    {
         if(AVL_DVBSx_Blind_End() != AVL_DVBSx_EC_OK)
         {
               bRet = FALSE;
         }
    }
    MsOS_ReleaseMutex(_s32MutexId);
    return bRet;
}

MS_BOOL MDrv_Demod_BlindScan_GetChannel(MS_U16 u16ReadStart,MS_U16* u16TPNum,DEMOD_MS_FE_CARRIER_PARAM *pTable)
{
    MS_BOOL bRet = TRUE;
    if (MsOS_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(printf("%s: Obtain mutex failed.\n", __FUNCTION__);)
        return FALSE;
    }
    else
    {
        struct AVL_DVBSx_Channel channels[MAX_TPS_LPF];
         MS_U16 u16ChNum = MAX_TPS_LPF,idx=0;

         if(AVL_DVBSx_Blind_GetChannel(u16ReadStart,&u16ChNum,channels) != AVL_DVBSx_EC_OK)
         {
               bRet =  FALSE;
               *u16TPNum = 0;
         }
         else
         {
             *u16TPNum = u16ChNum;
             for(idx = 0;idx < u16ChNum;idx++)
             {
                  pTable[idx].u32Frequency = channels[idx].m_uiFrequency_kHz /1000;
                  pTable[idx].SatParam.u32SymbolRate = channels[idx].m_uiSymbolRate_Hz /1000;
             }
         }
    }
    MsOS_ReleaseMutex(_s32MutexId);
    return bRet;
}

MS_BOOL MDrv_Demod_BlindScan_GetCurrentFreq(MS_U32 *u32CurrentFeq)
{
    MS_BOOL bRet = TRUE;
    if (MsOS_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(printf("%s: Obtain mutex failed.\n", __FUNCTION__);)
        return FALSE;
    }
    else
    {
         *u32CurrentFeq = AVL_DVBSx_Blind_GetCurrentFreq();
    }
    MsOS_ReleaseMutex(_s32MutexId);
    return bRet;
}
//-------------------------------------------------------------------------------------------------
//  drvDiSEqC Function
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_DiSEqC_SetTone(MS_BOOL bTone1)
{
       AVL_DVBSx_ErrorCode r = AVL_DVBSx_EC_OK;
       MS_BOOL bRet = TRUE;

       if (MsOS_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
       {
           DMD_ERR(printf("%s: Obtain mutex failed.\n", __FUNCTION__);)
           return FALSE;
       }

       r = AVL_DVBSx_DiSEqC_SetToneOut((MS_U8)bTone1);
       MsOS_ReleaseMutex(_s32MutexId);
	if(AVL_DVBSx_EC_OK != r)
	{
		bRet = FALSE;
	}
       if(bTone1)
       {
            printf("\n Set Tone1\n");
       }
       else
       {
            printf("\n Set Tone0\n");
       }
	return bRet;
}

MS_BOOL MDrv_DiSEqC_SetLNBOut(MS_BOOL bLow)
{
       AVL_DVBSx_ErrorCode r = AVL_DVBSx_EC_OK;
       MS_BOOL bRet = TRUE;

       if (MsOS_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
       {
           DMD_ERR(printf("%s: Obtain mutex failed.\n", __FUNCTION__);)
           return FALSE;
       }

       r = AVL_DVBSx_DiSEqC_SetLNBOut((MS_U8)bLow);
       MsOS_ReleaseMutex(_s32MutexId);
	if(AVL_DVBSx_EC_OK != r)
	{
		bRet = FALSE;
	}
    #if 0
       if(bLow)
       {
            printf("\n 13V\n");
       }
       else
       {
            printf("\n 18V\n");
       }
    #endif
	return bRet;
}

MS_BOOL MDrv_DiSEqC_GetLNBOut(MS_BOOL* bLNBOutLow)
{
       AVL_DVBSx_ErrorCode r = AVL_DVBSx_EC_OK;
       MS_U8 u8LNBOut =0;
       MS_BOOL bRet = TRUE;

       if (MsOS_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
       {
           DMD_ERR(printf("%s: Obtain mutex failed.\n", __FUNCTION__);)
           return FALSE;
       }

       r = AVL_DVBSx_DiSEqC_GetLNBOut(&u8LNBOut);
       if(u8LNBOut)
       {
             *bLNBOutLow = TRUE;
       }
       else
       {
            *bLNBOutLow = FALSE;
       }
       MsOS_ReleaseMutex(_s32MutexId);
	if(AVL_DVBSx_EC_OK != r)
	{
		bRet = FALSE;
	}
	return bRet;
}

MS_BOOL MDrv_DiSEqC_Set22kOnOff(MS_BOOL b22kOn)
{
       AVL_DVBSx_ErrorCode r = AVL_DVBSx_EC_OK;
       MS_U8 u822kStatus =0;
       MS_BOOL bRet = TRUE;

       if (MsOS_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
       {
           DMD_ERR(printf("%s: Obtain mutex failed.\n", __FUNCTION__);)
           return FALSE;
       }

       r = AVL_DVBSx_DiSEqC_Get22kOnOff(&u822kStatus);
       if(r != AVL_DVBSx_EC_OK)
       {
            u822kStatus = !b22kOn;
       }

       if(u822kStatus != b22kOn)
       {
           r = AVL_DVBSx_DiSEqC_Set22kOnOff((MS_U8)b22kOn);
           if(AVL_DVBSx_EC_OK != r)
           {
               bRet = FALSE;
           }
       }
       MsOS_ReleaseMutex(_s32MutexId);
       #if 0
       if(b22kOn)
       {
            printf("\n 22K On\n");
       }
       else
       {
            printf("\n 22K Off\n");
       }
       #endif
	return bRet;
}

MS_BOOL MDrv_DiSEqC_Get22kOnOff(MS_BOOL* b22kOn)
{
       AVL_DVBSx_ErrorCode r = AVL_DVBSx_EC_OK;
       MS_U8 u822kStatus =0;
       MS_BOOL bRet = TRUE;

       if (MsOS_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
       {
           DMD_ERR(printf("%s: Obtain mutex failed.\n", __FUNCTION__);)
           return FALSE;
       }

       r = AVL_DVBSx_DiSEqC_Get22kOnOff(&u822kStatus);
       if(u822kStatus)
       {
             *b22kOn = TRUE;
       }
       else
       {
            *b22kOn = FALSE;
       }
       MsOS_ReleaseMutex(_s32MutexId);
	if(AVL_DVBSx_EC_OK != r)
	{
		bRet = FALSE;
	}
	return bRet;
}

MS_BOOL MDrv_DiSEqC_SendCmd(MS_U8* pCmd,MS_U8 u8CmdSize)
{
       AVL_DVBSx_ErrorCode r = AVL_DVBSx_EC_OK;
       MS_BOOL bRet = TRUE;

       if (MsOS_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
       {
           DMD_ERR(printf("%s: Obtain mutex failed.\n", __FUNCTION__);)
           return FALSE;
       }

       r = AVL_DVBSx_DiSEqC_SetCmd(pCmd,u8CmdSize);

	if(AVL_DVBSx_EC_OK != r)
	{
		bRet = FALSE;
	}
       MsOS_ReleaseMutex(_s32MutexId);
       printf("\nCmdData[2] = 0x%X",pCmd[2]);
       printf("\nCmdData[3] = 0x%X",pCmd[3]);
	return bRet;
}

#endif

