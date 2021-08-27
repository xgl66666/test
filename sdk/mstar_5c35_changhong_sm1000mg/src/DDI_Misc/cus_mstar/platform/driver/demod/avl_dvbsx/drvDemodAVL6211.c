//<MStar Software>
//******************************************************************************
// MStar Software
// Copyright (c) 2010 - 2012 MStar Semiconductor, Inc. All rights reserved.
// All software, firmware and related documentation herein ("MStar Software") are
// intellectual property of MStar Semiconductor, Inc. ("MStar") and protected by
// law, including, but not limited to, copyright law and international treaties.
// Any use, modification, reproduction, retransmission, or republication of all 
// or part of MStar Software is expressly prohibited, unless prior written 
// permission has been granted by MStar. 
//
// By accessing, browsing and/or using MStar Software, you acknowledge that you
// have read, understood, and agree, to be bound by below terms ("Terms") and to
// comply with all applicable laws and regulations:
//
// 1. MStar shall retain any and all right, ownership and interest to MStar
//    Software and any modification/derivatives thereof.
//    No right, ownership, or interest to MStar Software and any
//    modification/derivatives thereof is transferred to you under Terms.
//
// 2. You understand that MStar Software might include, incorporate or be
//    supplied together with third party`s software and the use of MStar
//    Software may require additional licenses from third parties.  
//    Therefore, you hereby agree it is your sole responsibility to separately
//    obtain any and all third party right and license necessary for your use of
//    such third party`s software. 
//
// 3. MStar Software and any modification/derivatives thereof shall be deemed as
//    MStar`s confidential information and you agree to keep MStar`s 
//    confidential information in strictest confidence and not disclose to any
//    third party.  
//
// 4. MStar Software is provided on an "AS IS" basis without warranties of any
//    kind. Any warranties are hereby expressly disclaimed by MStar, including
//    without limitation, any warranties of merchantability, non-infringement of
//    intellectual property rights, fitness for a particular purpose, error free
//    and in conformity with any international standard.  You agree to waive any
//    claim against MStar for any loss, damage, cost or expense that you may
//    incur related to your use of MStar Software.
//    In no event shall MStar be liable for any direct, indirect, incidental or
//    consequential damages, including without limitation, lost of profit or
//    revenues, lost or damage of data, and unauthorized system use.
//    You agree that this Section 4 shall still apply without being affected
//    even if MStar Software has been modified by MStar in accordance with your
//    request or instruction for your use, except otherwise agreed by both
//    parties in writing.
//
// 5. If requested, MStar may from time to time provide technical supports or
//    services in relation with MStar Software to you for your use of
//    MStar Software in conjunction with your or your customer`s product
//    ("Services").
//    You understand and agree that, except otherwise agreed by both parties in
//    writing, Services are provided on an "AS IS" basis and the warranty
//    disclaimer set forth in Section 4 above shall apply.  
//
// 6. Nothing contained herein shall be construed as by implication, estoppels
//    or otherwise:
//    (a) conferring any license or right to use MStar name, trademark, service
//        mark, symbol or any other identification;
//    (b) obligating MStar or any of its affiliates to furnish any person,
//        including without limitation, you and your customers, any assistance
//        of any kind whatsoever, or any information; or 
//    (c) conferring any license or right under any intellectual property right.
//
// 7. These terms shall be governed by and construed in accordance with the laws
//    of Taiwan, R.O.C., excluding its conflict of law rules.
//    Any and all dispute arising out hereof or related hereto shall be finally
//    settled by arbitration referred to the Chinese Arbitration Association,
//    Taipei in accordance with the ROC Arbitration Law and the Arbitration
//    Rules of the Association by three (3) arbitrators appointed in accordance
//    with the said Rules.
//    The place of arbitration shall be in Taipei, Taiwan and the language shall
//    be English.  
//    The arbitration award shall be final and binding to both parties.
//
//******************************************************************************
//<MStar Software>
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

#include "AVL_DVBSx_Board.h"
#include "drvDemod.h" //Temp for use
#include "AVL_DVBSx_Func.h"


#if IF_THIS_DEMOD_INUSE(DEMOD_AVL6211)

#ifdef MS_DEBUG
#define DMD_ERR(_print)	     			_print
#define DMD_MSG(_print)      			_print
#else
#define DMD_ERR(_print)	     			_print
#define DMD_MSG(_print)      			_print
#endif

extern void MApi_Demod_HWReset(void);

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
              //DMD_MSG(printf("\r\n u32TunerFreq = %d u32SymbolRate = %d",pParam->u32TunerFreq,pParam->u32SymbolRate));
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

