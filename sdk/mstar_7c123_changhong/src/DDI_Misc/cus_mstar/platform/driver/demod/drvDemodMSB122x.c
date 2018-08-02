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
//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------
#include "Board.h"

#if IF_THIS_DEMOD_INUSE(DEMOD_MSB122x)


#include "MsCommon.h"
#include "HbCommon.h"
//#include "drvDMD.h"
#include "drvIIC.h"
#include "drvTuner.h"
#include "drvDemod.h"
#include "MSB122x.h"

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

#define CHG_COFDMDMD_EVENT(_event)      MsOS_ClearEvent(_s32EventId, 0xFFFFFFFF);   \
                                        MsOS_SetEvent(_s32EventId, _event);


//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Global Variables
//------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Local Variables
//-------------------------------------------------------------------------------------------------
//static void*                                      _pTaskStack;
static MS_S32                       _s32TaskId  = -1;
static MS_S32                       _s32MutexId;
//static MS_S32                                  _s32EventId;
//static COFDMDMD_TunState               _eTunState;
//static MS_BOOL                                _bTuned;
static MS_BOOL                      bInited     = FALSE;
//static MS_U32                      u32CurrentTime = 0;
static MS_BOOL                      _bIsSerialTsOutput = !TS_PARALLEL_OUTPUT;

static MS_U8                        _u8Bandwith = DEMOD_BW_MODE_8MHZ;

//static MS_U32                                   u32SetFreq;
//static MS_U8                                     u8BandW;

//-------------------------------------------------------------------------------------------------
// Local Functions
//-------------------------------------------------------------------------------------------------
MS_U8   u8DemodAddr = DEMOD_SLAVE_ID;

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

MS_BOOL MDrv_Demod_Init(void)
{

    bInited = FALSE;
    DBG_MSB(HB_printf("-----%s start-----\n", __FUNCTION__));

    if (_s32TaskId >= 0)
    {
        return TRUE;
    }

    _s32MutexId = MsOS_CreateMutex(E_MSOS_FIFO, "OfDmd_Mutex", MSOS_PROCESS_SHARED);

    if (_s32MutexId < 0)
    {
        DMD_ERR(("%s: Create mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    Connect(DTV_Get_Demod_TYPE());
    MSB122x_Power_On_Initialization(TRUE);

    bInited = TRUE;
    DBG_MSB(HB_printf("-----%s done-----\n", __FUNCTION__));
    return TRUE;

}

MS_BOOL MDrv_Demod_Open(void)
{
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    else
    {
    }

    HB_ReleaseMutex(_s32MutexId);
    return TRUE;

}

MS_BOOL MDrv_Demod_Close(void)
{
    return TRUE;
}

MS_BOOL MDrv_Demod_Reset(void)
{
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    else
    {
        #if(FRONTEND_DEMOD_TYPE == DEMOD_MSB122x)
        Reset();
        #endif
    }

    HB_ReleaseMutex(_s32MutexId);
    return TRUE;

}

MS_BOOL MDrv_Demod_TsOut(MS_BOOL bEnable)
{
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    else
    {
    }

    HB_ReleaseMutex(_s32MutexId);
    return TRUE;
}

MS_BOOL MDrv_Demod_PowerOnOff(MS_BOOL bPowerOn)
{
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    else
    {
    }

    HB_ReleaseMutex(_s32MutexId);
    return TRUE;
}

MS_BOOL MDrv_Demod_SetBW(MS_U32 u32BW)
{
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    else
    {
    }

    HB_ReleaseMutex(_s32MutexId);
    return TRUE;

}

MS_BOOL MDrv_Demod_GetBW(MS_U32 *pu32BW)
{
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    else
    {
    }

    HB_ReleaseMutex(_s32MutexId);
    return TRUE;

}

MS_BOOL MDrv_Demod_GetLock(MS_BOOL *pbLock)
{
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    else
    {
        *pbLock = DTV_DVB_T_GetLockStatus();
    }

    HB_ReleaseMutex(_s32MutexId);
    return TRUE;

}

MS_BOOL MDrv_Demod_GetSNR(MS_U32 *pu32SNR)
{
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    else
    {
        if (TRUE == DTV_DVB_T_GetLockStatus() )
        {
            *pu32SNR = (MS_U32)DTV_GetSignalQuality();
        }

        else
        {
            *pu32SNR = 0;
        }
    }

    HB_ReleaseMutex(_s32MutexId);
    return TRUE ;

}

MS_BOOL MDrv_Demod_GetBER(float *pfBER)
{
    MS_U32  dwError = 0;

    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    else
    {
        if (TRUE == DTV_DVB_T_GetLockStatus() )
        {

            dwError = DTV_GetBER();

            if (dwError )
            {
                *pfBER = -1;
            }
        }
        else
        {
            *pfBER = -1;
        }
    }

    HB_ReleaseMutex(_s32MutexId);
    return (dwError ) ? FALSE : TRUE;
}

MS_BOOL MDrv_Demod_GetPWR(MS_S32 *ps32Signal)
{
#define DEFAULT_PWR_MIN (-97)
#define DEFAULT_PWR_MAX (-5)

    MS_U32  dwError=0;
    MS_U16   wSignal;

    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    else
    {
         if (TRUE == DTV_DVB_T_GetLockStatus() )
        {
            wSignal = DTV_GetSignalStrength();
            HB_printf("[%s][%d] DTV_GetSignalStrength = %d \n", __FUNCTION__, __LINE__, wSignal);
            if (wSignal == 0)
            {
                *ps32Signal = DEFAULT_PWR_MIN;
            }
            else
            {
                *ps32Signal = wSignal;
            }
        }
        else
        {
            *ps32Signal = DEFAULT_PWR_MIN;
        }
    }

    HB_ReleaseMutex(_s32MutexId);
    return (dwError ) ? FALSE : TRUE;

}

MS_BOOL MDrv_Demod_Config(MS_U8 *pRegParam)
{
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    else
    {

    }
    HB_ReleaseMutex(_s32MutexId);
    return TRUE;

}

MS_BOOL MDrv_Demod_GetParam(DEMOD_MS_FE_CARRIER_PARAM* pParam)
{
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    else
    {
    }
    HB_ReleaseMutex(_s32MutexId);
    return TRUE;

}

MS_BOOL MDrv_Demod_Restart(DEMOD_MS_FE_CARRIER_PARAM* pParam)
{
    MS_U32  dwError = 0;

    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(HB_printf("MDrv_COFDMDMD_Restart: Obtain mutex failed.\n"));
        return FALSE;
    }
    else
    {
        DBG_MSB(HB_printf("\r\n u32TunerFreq = %ld u8BandWidth = %d \n",pParam->u32Frequency,pParam->TerParam.eBandWidth));
        MSB122x_Active(DISABLE);
        dwError = MDrv_Tuner_SetTuner( pParam->u32Frequency/1000.0, pParam->TerParam.eBandWidth);

        #if(FRONTEND_DEMOD_TYPE == DEMOD_MSB122x)
        SysDelay(10);
        DTV_SetFrequency(pParam->u32Frequency/1000.0, pParam->TerParam.eBandWidth);
        _u8Bandwith = pParam->TerParam.eBandWidth;
        //DTV_Config (pParam->TerParam.eBandWidth+6, _bIsSerialTsOutput, TRUE);
        //DTV_Serial_Control(_bIsSerialTsOutput);

        MSB122x_Active(ENABLE);
        #endif
    }
    HB_ReleaseMutex(_s32MutexId);
    return (dwError) ? FALSE : TRUE;
}

MS_BOOL MDrv_Demod_SetMode(Demod_Mode* pMode)
{
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    else
    {
    }
    HB_ReleaseMutex(_s32MutexId);
    return TRUE;

}

MS_BOOL MDrv_Demod_I2C_ByPass(MS_U8 u8DemodIndex,MS_BOOL bOn)
{
     #if(FRONTEND_DEMOD_TYPE == DEMOD_MSB122x)
     return IIC_Bypass_Mode( bOn);
     #endif
}
MS_BOOL MDrv_Demod_SetTsSerial(MS_BOOL bSerial) //bSerial 1: Ts Serial ouput 0: Ts Parallel output /*MSD5021:ts output is parrel in CI, but is serial in non ci*/
{
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(HB_printf("MDrv_COFDMDMD_Restart: Obtain mutex failed.\n"));
        return FALSE;
    }
    else
    {
        _bIsSerialTsOutput = bSerial;
        DTV_Config (_u8Bandwith, _bIsSerialTsOutput, FALSE);
        DTV_Serial_Control(_bIsSerialTsOutput);
        MSB122x_Active(ENABLE);
    }

    HB_ReleaseMutex(_s32MutexId);
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




#endif // (FRONTEND_DEMOD_TYPE == DEMOD_MSB122x)
