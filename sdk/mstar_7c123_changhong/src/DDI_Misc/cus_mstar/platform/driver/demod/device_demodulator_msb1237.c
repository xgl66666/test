#define _MSB1237_C_

/* ------------------------------------
    Header Files
   ------------------------------------ */
#include <stdio.h>
#include <string.h>
//#include "msAPI_Timer.h"
//#include "Mapp_IR.h"
//#include "Mapp_Key.h"
//#include "MApp_GlobalVar.h"

#include "MsCommon.h"
#include "MsOS.h"
#include "Board.h"
#include "drvDMD_common.h"
#include "drvDMD_VD_MBX.h"

#include "drvIIC.h"
#include "drvGPIO.h"
#include "drvHWI2C.h"
#include "drvTuner.h"
#include "drvDemod.h"
#include "HbCommon.h"
#include "drvDemodNull.h"
#include "device_demodulator_msb1237.h"


#if 0
#include <math.h>
#include "MsCommon.h"
#include "drvIIC.h"
#include "HbCommon.h"
#include "MsOS.h"
#include "Board.h"
#include "drvTuner.h"
#include "drvDemod.h"
#include "drvDMD_common.h"
#include "drvDMD_ISDBT.h"       //dan add for integrate ISDBT utopia driver
#include "drvDMD_VD_MBX.h"
#endif




#if IF_THIS_DEMOD_INUSE(DEMOD_MSB1237)//1237????
#include "drvDMD_ATSC.h" //drvDMD_ATSC.h
#include "device_demodulator_msb1237.h"//1237.h???


#define DBG_MSB(x)  x
#define MSB1237_DBINFO(y) y

#define MSTAR_MSB1237_I2C_ADDR 0x32//(((MS_U16)1 << 8) | 0xF2)//(((MS_U16) DEMOD_IIC_BUS << 8) | 0xF2)
#define MSB1237_ADDR_H         0x00
#define MSB1237_READ_REG       0x04
#define MSB1237_RAM_CONTROL    0x05

#define MSB1237_OUTER_STATE    0x80

#define MSB1237_VSB_TRAIN_SNR_LIMIT   0xBE//14.5dB
#define MSB1237_FEC_ENABLE     0x1F//

#define QAM_CHECK_LOCK_TIME_SLICE     50
#define QAM_WAIT_AGC_LOCK_TIME        250
#define QAM_WAIT_PRE_LOCK_TIME        700
#define QAM_WAIT_LOCK_TOTAL_TIME      2000
#define QAM_CHECK_AGC_LOCK_COUNTER    QAM_WAIT_AGC_LOCK_TIME/QAM_CHECK_LOCK_TIME_SLICE
#define QAM_CHECK_PRE_LOCK_COUNTER    QAM_WAIT_PRE_LOCK_TIME/QAM_CHECK_LOCK_TIME_SLICE
#define QAM_CHECK_LOCK_COUNTER        QAM_WAIT_LOCK_TOTAL_TIME/QAM_CHECK_LOCK_TIME_SLICE

#define CODE_TYPE_ORIGINAL  0
#define COFDMDMD_MUTEX_TIMEOUT       (2000)

MS_BOOL   MSB1237_IIC_Write(MS_U16 u16BusNumSlaveID, MS_U8 u8addrcount, MS_U8* pu8addr, MS_U16 u16size, MS_U8* pu8data);
MS_BOOL   MSB1237_IIC_Read(MS_U16 u16BusNumSlaveID, MS_U8 u8AddrNum, MS_U8* paddr, MS_U16 u16size, MS_U8* pu8data);



//#define TS_PARALLEL_OUTPUT 1
#if TS_PARALLEL_OUTPUT
#define ATSC_TS_SERIAL		  0x00
#else
#define ATSC_TS_SERIAL		  0x01
#endif

// Division number of TS clock rate
// Formula: TS Clk Rate= 216 MHz/reg_isdbt_ts_clk_divnum
// Ex: ISDBT_TS_CLK_DIVNUM = 31 => TS Clk = 216/31= 6.967 MHz
#define ATSC_TS_CLK_DIVNUM_SERIAL_MODE   0//4
#define ATSC_TS_CLK_DIVNUM_PARALLEL_MODE  26//31

#if (ATSC_TS_SERIAL)
#define ATSC_TS_CLK_DIVNUM	  ATSC_TS_CLK_DIVNUM_SERIAL_MODE
#else
#define ATSC_TS_CLK_DIVNUM	  ATSC_TS_CLK_DIVNUM_PARALLEL_MODE
#endif

#define ATSC_TS_DATA_SWAP  0x00  // TS data inversion
#define ATSC_TS_CLK_INV      0x01  // TS clock inverseion          //this define will depend on hw main board
//#define ATSC_IQ_SWAP   0x01          // IQ Swap                    //this define will depend on tuner
//#define ATSC_IFAGC_REF   0x38                                      //this define will depend on tuner

#define KGC_MSB1237_DEMOD_IIC_SLAVE_ID 0xF2
#if defined(KGC_USE_ON_CHIP_MSB1237) && (KGC_USE_ON_CHIP_MSB1237 == 1)
#define DEMOD_MSB1237_SLAVE_ID  KGC_MSB1237_DEMOD_IIC_SLAVE_ID
#else
#define DEMOD_MSB1237_SLAVE_ID 0xD2
#endif
//static MS_BOOL bInited = FALSE;
//static MS_S32 _s32MutexId = -1;

#if 0
code MS_U8 MSB1237_table[] = {
 #include "MSB1237_Demod.dat"
};
#endif

//Local variables//
static MS_U8                        u8DemodType = 0; // 0:VSB, 1:QAM256, 2:QAM64
static MS_S32                       _s32TaskId  = -1;
static MS_S32                       _s32MutexId;
static MS_BOOL                      bInited     = FALSE;
static DEMOD_MS_INIT_PARAM          InitParam[MAX_FRONTEND_NUM];

MS_U32 Scan_Time_Start;

/***********************************************************************************
************************************************************************************/
static MS_BOOL _Demod_Get_Lock(MS_U8 u8DemodIndex, DMD_ATSC_GETLOCK_TYPE eType, EN_LOCK_STATUS *peLockStatus)
{
    DMD_ATSC_LOCK_STATUS eLockStatus;
    static MS_U8 flag = 0;
    MS_U16 locktime;

    if (MsOS_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    if(FALSE == bInited)
    {
        printf("[%s]MSB1237 have not inited !!!\n",__FUNCTION__);
        HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }


    eLockStatus= MDrv_DMD_ATSC_GetLock(eType);
    if (eLockStatus == E_DEMOD_LOCK)
    {
        if (!flag)
        {
             flag = 1;
             MDrv_DMD_ATSC_Get_Lock_Time(&locktime);
             DBG_MSB(printf("****ATSC lock time**** %d\r\n",locktime));
         }
     }
     else
           flag = 0;

      switch (eLockStatus)
      {
       case DMD_ATSC_LOCK:
            *peLockStatus = E_DEMOD_LOCK;
            break;
        case DMD_ATSC_CHECKING:
            *peLockStatus = E_DEMOD_CHECKING;
            break;
        case DMD_ATSC_CHECKEND:
            *peLockStatus = E_DEMOD_CHECKEND;
            break;
        case DMD_ATSC_UNLOCK:
            *peLockStatus = E_DEMOD_UNLOCK;
            break;
        default:
            break;
      }
    HB_ReleaseMutex(_s32MutexId);
    return TRUE;
}

// Global function//
MS_BOOL MSB1237_Demod_Init(MS_U8 u8DemodIndex,DEMOD_MS_INIT_PARAM* pParam)
{
    DMD_ATSC_InitData sDMD_ATSC_InitData;
    static MS_U8 u8DMD_ATSC_InitExt[]={1};
    MS_BOOL ret = TRUE;

    //bInited = FALSE;
    DBG_MSB(printf("-----%s start----- 111 \n", __FUNCTION__));

    if (_s32TaskId >= 0)
    {
        return TRUE;
    }

    _s32MutexId = MsOS_CreateMutex(E_MSOS_FIFO, "OfDmd_Mutex", MSOS_PROCESS_SHARED);

    if (_s32MutexId < 0)
    {
        DMD_ERR(("-----%s _s32MutexId < 0----- \n", __FUNCTION__));
        return FALSE;
    }

    MDrv_DMD_PreInit();


    // copy tuner config
    if(pParam->pstTunertab == NULL)
        return FALSE;
    else
        InitParam[u8DemodIndex].pstTunertab = pParam->pstTunertab;

    if(InitParam[u8DemodIndex].pstTunertab->data == TUNER_R836)
        sDMD_ATSC_InitData.bIQSwap = 1;//u8IqSwap;
    else
        sDMD_ATSC_InitData.bIQSwap = 0;

    sDMD_ATSC_InitData.u16IF_KHZ = 5000;//u16DtvIFFreqKHz;
    sDMD_ATSC_InitData.u16VSBAGCLockCheckTime = 50;
    sDMD_ATSC_InitData.u16VSBPreLockCheckTime = 300;
    sDMD_ATSC_InitData.u16VSBFSyncLockCheckTime = 1200;
    sDMD_ATSC_InitData.u16VSBFECLockCheckTime = 5000;

    sDMD_ATSC_InitData.u16QAMAGCLockCheckTime = 50;
    sDMD_ATSC_InitData.u16QAMPreLockCheckTime = 1000;
    sDMD_ATSC_InitData.u16QAMMainLockCheckTime = 3000;


    sDMD_ATSC_InitData.u8DMD_ATSC_InitExt = u8DMD_ATSC_InitExt;

    //Parallel mode
    sDMD_ATSC_InitData.u5TsConfigByte_DivNum = ATSC_TS_CLK_DIVNUM;
    sDMD_ATSC_InitData.u1TsConfigByte_ClockInv = ATSC_TS_CLK_INV;
    sDMD_ATSC_InitData.u1TsConfigByte_DataSwap = ATSC_TS_DATA_SWAP;
    sDMD_ATSC_InitData.u1TsConfigByte_SerialMode = ATSC_TS_SERIAL;

    //I2C[begin]
    sDMD_ATSC_InitData.u8I2CSlaveAddr = DEMOD_MSB1237_SLAVE_ID;
    sDMD_ATSC_InitData.u8I2CSlaveBus = 0;
    sDMD_ATSC_InitData.bIsExtDemod = TRUE;
    sDMD_ATSC_InitData.I2C_WriteBytes = MSB1237_IIC_Write;
    sDMD_ATSC_InitData.I2C_ReadBytes = MSB1237_IIC_Read;
    sDMD_ATSC_InitData.bIsQPad =  FALSE; //FALSE is Q PAD. TRUE is I PAD.
    sDMD_ATSC_InitData.bIsUseSspiLoadCode = FALSE;

    ret &= MDrv_DMD_ATSC_Initial_Hal_Interface();

    ret &= MDrv_DMD_ATSC_Init(&sDMD_ATSC_InitData, sizeof(sDMD_ATSC_InitData));

    if(ret)
    {
       bInited = TRUE;
    }
    return ret;
}

MS_BOOL MSB1237_Demod_Open(MS_U8 u8DemodIndex)
{
    if (MsOS_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    else
    {
    }

    HB_ReleaseMutex(_s32MutexId);
    return TRUE;

}

MS_BOOL MSB1237_Demod_Close(MS_U8 u8DemodIndex)
{
    return TRUE;
}

MS_BOOL MSB1237_Demod_Reset(MS_U8 u8DemodIndex)
{
    if (MsOS_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    else
    {
        if(!MDrv_DMD_ATSC_SetReset())//(!MSB1237_SoftReset())
        {
            DBG_MSB(printf("%s: Soft reset failed.\n", __FUNCTION__));
            return FALSE;
        }
    }

    HB_ReleaseMutex(_s32MutexId);
    return TRUE;

}

MS_BOOL MSB1237_Demod_TsOut(MS_U8 u8DemodIndex, MS_BOOL bEnable)
{
    if (MsOS_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    else
    {
    }

    HB_ReleaseMutex(_s32MutexId);
    return TRUE;
}

MS_BOOL MSB1237_Demod_PowerOnOff(MS_U8 u8DemodIndex, MS_BOOL bPowerOn)
{
    if (MsOS_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    else
    {
    }

    HB_ReleaseMutex(_s32MutexId);
    return TRUE;
}

MS_BOOL MSB1237_Demod_SetBW(MS_U8 u8DemodIndex, MS_U32 u32BW)
{
    if (MsOS_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    else
    {
    }

    HB_ReleaseMutex(_s32MutexId);
    return TRUE;

}

MS_BOOL MSB1237_Demod_GetBW(MS_U8 u8DemodIndex, MS_U32 *pu32BW)
{
    if (MsOS_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    else
    {
    }

    HB_ReleaseMutex(_s32MutexId);
    return TRUE;

}

MS_BOOL MSB1237_Demod_GetLock(MS_U8 u8DemodIndex, EN_LOCK_STATUS *peLockStatus)
{
    return _Demod_Get_Lock(u8DemodIndex, DMD_ATSC_GETLOCK, peLockStatus);
}

MS_BOOL MSB1237_Demod_GetPreLockStatus(MS_U8 u8DemodIndex, EN_LOCK_STATUS *peLockStatus)
{
    return _Demod_Get_Lock(u8DemodIndex, DMD_ATSC_GETLOCK_VSB_PRELOCK, peLockStatus);
}

MS_BOOL MSB1237_Demod_GetFlameSyncLockStatus(MS_U8 u8DemodIndex, EN_LOCK_STATUS *peLockStatus)
{
    return _Demod_Get_Lock(u8DemodIndex, DMD_ATSC_GETLOCK_VSB_FSYNCLOCK, peLockStatus);
}

MS_BOOL MSB1237_Demod_GetSignalQuality(MS_U8 u8DemodIndex, MS_U16 *pu16quality)
{
    if (MsOS_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    else
    {
        if( MDrv_DMD_ATSC_GetLock(DMD_ATSC_GETLOCK)== DMD_ATSC_LOCK)
        {

            *pu16quality = (MS_U16)MDrv_DMD_ATSC_GetSNRPercentage();
        }

        else
        {
            *pu16quality = 0;
        }
    }

    HB_ReleaseMutex(_s32MutexId);
    return TRUE ;

}

MS_BOOL MSB1237_Demod_GetSNR(MS_U8 u8DemodIndex, float *pfSNR)
{

    if (MsOS_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    else
    {
        if( MDrv_DMD_ATSC_GetLock(DMD_ATSC_GETLOCK)== DMD_ATSC_LOCK)
        {

            *pfSNR = (float)MDrv_DMD_ATSC_GetSNRPercentage() * 0.4;
        }

        else
        {
            *pfSNR = 0;
        }
    }

    HB_ReleaseMutex(_s32MutexId);
    return TRUE ;

}

MS_BOOL MSB1237_Demod_GetBER(MS_U8 u8DemodIndex, float *pfBER)
{
    MS_BOOL  bret;

    if (MsOS_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    else
    {
        *pfBER = -1;
    }

    bret = MDrv_DMD_ATSC_GetPostViterbiBer(pfBER);
    HB_ReleaseMutex(_s32MutexId);
    return bret;
}

MS_BOOL MSB1237_Demod_GetPWR(MS_U8 u8DemodIndex, MS_S32 *ps32Signal)
{
#define DEFAULT_PWR_MIN (-97)
#define DEFAULT_PWR_MAX (-5)

    MS_U32  dwError=0;
    MS_U8 u8snr;
 // MS_S32  wSignal;

    if (MsOS_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    else
    {
        //if (TRUE == MSB1237_GetLock())
        if( MDrv_DMD_ATSC_GetLock(DMD_ATSC_GETLOCK)== DMD_ATSC_LOCK)
        {
           //Signal = lgdt3305_SignalStatus(DEMOD_TYPE);
           //       wSignal = lgdt3305_IfAcc();
          //rintf("PWR = 0x%x \n",wSignal);

         u8snr = MDrv_DMD_ATSC_GetSNRPercentage();
         *ps32Signal = (MS_U32) u8snr;
            //*ps32Signal =  MSB1237_ReadSNRPercentage();
        }

        else
        {
            *ps32Signal = DEFAULT_PWR_MIN;
        }
    }
        //*ps32Signal = DEFAULT_PWR_MAX;

    HB_ReleaseMutex(_s32MutexId);
    return (dwError ) ? FALSE : TRUE;
   // return TRUE;
}

MS_BOOL MSB1237_Demod_Config(MS_U8 u8DemodIndex, MS_U8 *pRegParam)
{
    if (MsOS_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    else
    {

    }
    HB_ReleaseMutex(_s32MutexId);
    return TRUE;

}

MS_BOOL MSB1237_Demod_GetParam(MS_U8 u8DemodIndex, DEMOD_MS_FE_CARRIER_PARAM* pParam)
{
    if (MsOS_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    else
    {
    }
    HB_ReleaseMutex(_s32MutexId);
    return TRUE;

}

MS_BOOL MSB1237_Demod_I2C_ByPass(MS_U8 u8DemodIndex, MS_BOOL bEnable)
{
    MS_BOOL bret = TRUE;
    if(bInited)
    {
        if(bEnable)
            return MDrv_DMD_ATSC_IIC_BYPASS_MODE(TRUE);
        else
            return MDrv_DMD_ATSC_IIC_BYPASS_MODE(FALSE);
    }
    else
    {
         bret &= MSB1237_I2C_CH_Reset(3,TRUE);
         if(bEnable)
             bret &= MSB1237_I2C_WriteByte(DEMOD_MSB1237_SLAVE_ID,0x0910, 0x10);
         else
             bret &= MSB1237_I2C_WriteByte(DEMOD_MSB1237_SLAVE_ID,0x0910, 0x00);
         return bret;
    }
}


MS_BOOL MSB1237_Demod_Restart(MS_U8 u8DemodIndex, DEMOD_MS_FE_CARRIER_PARAM* pParam,MS_U32 u32BroadCastType)
{
    MS_BOOL bret = FALSE;

    if (MsOS_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(printf("MDrv_COFDMDMD_Restart: Obtain mutex failed.\n"));
        return FALSE;
    }
    else
    {
        //MSB1237_Demod_I2C_ByPass(0, TRUE);
        //NoError = MDrv_Tuner_SetTuner(0, pParam->u32Frequency, pParam->TerParam.eBandWidth);
          MSB1237_Demod_I2C_ByPass(0, FALSE);
        //MsOS_DelayTask(250); //necessary for NMI120 get RSSI
        if(!MDrv_DMD_ATSC_SetReset())//(!MSB1237_SoftReset())
        {
          printf("-----%s FAIL-----\n", __FUNCTION__);
          HB_ReleaseMutex(_s32MutexId);
          return FALSE;
        }
        MsOS_DelayTask(100); // ori = 10

        //printf("    MDrv_DMD_ATSC_SetReset OK ! \n");
        //printf("\npParam->TerParam.eDemodMode %d\n",pParam->TerParam.eDemodMode);
        //use demod type of current channel in database
        //if(pParam->TerParam.eDemodMode == DEMOD_MODE_8VSB)
        //    u8DemodType = 0;
        //else if(pParam->TerParam.eDemodMode == DEMOD_MODE_256QAM)
        //    u8DemodType = 1;
        //else
       //     u8DemodType = 2;


       if(u32BroadCastType == J83B)
           MSB1237_Demod_1237_SetMode(1);
       else //ATSC
           MSB1237_Demod_1237_SetMode(0);


        switch(u8DemodType)
        {
           case 0:
               bret=MDrv_DMD_ATSC_SetVsbMode();//MSB1237_VsbMode();
               break;
           case 1:
               bret=MDrv_DMD_ATSC_Set256QamMode();//MSB1237_256QamMode();
               break;
           case 2:
               bret=MDrv_DMD_ATSC_Set64QamMode();//MSB1237_64QamMode();
               break;
           default:
               DBG_MSB(printf("Unknown Demod type\n"));
               break;
        }
    }
    HB_ReleaseMutex(_s32MutexId);
    //return (NoError) ? TRUE : FALSE;
    return bret;
}

MS_BOOL MSB1237_Demod_Restart_AT(DEMOD_MS_FE_CARRIER_PARAM* pParam)
{
    MS_U32  dwError = 0;
    MS_BOOL bret;
    switch(pParam->TerParam.eHierarchy)
    {
    case 9:  //64QAM
        u8DemodType = 2;
        break;
    case 10: //256QAM
        u8DemodType = 1;
        break;
    case 6:  //VSB
    default:
        u8DemodType = 0;
        break;
    };


    if (MsOS_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(printf("MDrv_COFDMDMD_Restart: Obtain mutex failed.\n"));
        return FALSE;
    }
    else
    {
	  // dwError = MDrv_Tuner_SetTuner(0, pParam->u32Frequency, pParam->TerParam.eBandWidth);
      // MsOS_DelayTask(250); //necessary for NMI120 get RSSI
       if(!MDrv_DMD_ATSC_SetReset())//(!MSB1237_SoftReset())
       {
          DBG_MSB(printf("-----%s FAIL-----\n", __FUNCTION__));
          HB_ReleaseMutex(_s32MutexId);
          return FALSE;
       }
       MsOS_DelayTask(10);

       switch(u8DemodType)
       {
           case 0:
               bret=MDrv_DMD_ATSC_SetVsbMode();//MSB1237_VsbMode();
               break;
           case 1:
               bret=MDrv_DMD_ATSC_Set256QamMode();//MSB1237_256QamMode();
               break;
           case 2:
               bret=MDrv_DMD_ATSC_Set64QamMode();//MSB1237_64QamMode();
               break;
           default:
               DBG_MSB(printf("Unknown Demod type\n"));
               break;
       }

    }
    HB_ReleaseMutex(_s32MutexId);
    return (dwError) ? FALSE : TRUE;
}

MS_BOOL MSB1237_Demod_SetMode(MS_U8 u8DemodIndex, Demod_Mode* pMode)
{
    if (MsOS_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    else
    {
    }
    HB_ReleaseMutex(_s32MutexId);
    return TRUE;

}

MS_BOOL MSB1237_Demod_SetOutoutPath(MS_U8 u8DemodIndex, DEMOD_INTERFACE_MODE path)
{
    //Dummy function
    return TRUE;
}


MS_BOOL   MSB1237_IIC_Write(MS_U16 u16BusNumSlaveID, MS_U8 u8addrcount, MS_U8* pu8addr, MS_U16 u16size, MS_U8* pu8data)
{
    MS_BOOL bResult = TRUE;
    //printf("Enter MDrv_MSB140X_IIC_Write\n");

    bResult = MDrv_IIC_Write(u16BusNumSlaveID, pu8addr, u8addrcount, pu8data,  u16size);

    return bResult;
}

MS_BOOL   MSB1237_IIC_Read(MS_U16 u16BusNumSlaveID, MS_U8 u8AddrNum, MS_U8* paddr, MS_U16 u16size, MS_U8* pu8data)
{
    MS_BOOL bResult = TRUE;
    //printf("Enter MDrv_MSB140X_IIC_Read\n");

    bResult = MDrv_IIC_Read(u16BusNumSlaveID, paddr, u8AddrNum, pu8data,  u16size);

    return bResult;
}




DEMOD_INTERFACE_MODE MSB1237_Demod_GetOutoutPath(MS_U8 u8DemodIndex)
{
    //Dummy function
    return DEMOD_INTERFACE_PARALLEL;
}


MS_U16 MSB1237_Demod_ReadReg(MS_U16 RegAddr)
{
    MS_U8 RegData = 0;
    //MSB1237_ReadReg(RegAddr, &RegData);
    MDrv_DMD_ATSC_GetReg(RegAddr, &RegData);
    return (MS_U16) RegData;
}

MS_BOOL MSB1237_Demod_WriteReg(MS_U16 RegAddr, MS_U16 RegData)
{
	  MS_U8 u8RegData = 0;

	  u8RegData= (MS_U8) RegData;
    return MDrv_DMD_ATSC_SetReg(RegAddr, u8RegData);
    //return MSB1237_WriteReg(RegAddr, RegData);
}

void MSB1237_Demod_1237_SetMode(MS_U8 u8demod_type)
{
    u8DemodType = u8demod_type;
    printf("\n\n===== Demod Type = %d =====\n\n", u8DemodType);
}

MS_U8 MSB1237_Demod_1237_GetMode(void)
{
    if(u8DemodType == 0)
        return DEMOD_MODE_8VSB;
    else if(u8DemodType == 1)
        return DEMOD_MODE_256QAM;
    else if(u8DemodType == 2)
        return DEMOD_MODE_64QAM;


    return DEMOD_MODE_8VSB;
    //return u8DemodType;
}
//===================================================================================
MS_U8 MSB1237_I2C_ReadByte(MS_U16 u16SlaveAddr, MS_U32 u32Addr, MS_U8 *pu8Data)
{

    MS_BOOL bRet=TRUE;
    MS_U8 u8MsbData[6] = {0};

    u8MsbData[0] = 0x10;
    u8MsbData[1] = 0x00;
    u8MsbData[2] = 0x00;
    u8MsbData[3] = (u32Addr >> 8) &0xff;
    u8MsbData[4] = u32Addr &0xff;

    u8MsbData[0] = 0x35;
    bRet &= MSB1237_IIC_Write(u16SlaveAddr, 0, 0, 1, u8MsbData);

    u8MsbData[0] = 0x10;
    bRet &= MSB1237_IIC_Write(u16SlaveAddr, 0, 0, 5, u8MsbData);
    bRet &= MSB1237_IIC_Read(u16SlaveAddr, 0, 0, 1, pu8Data);

    u8MsbData[0] = 0x34;
    bRet &= MSB1237_IIC_Write(u16SlaveAddr, 0, 0, 1, u8MsbData);

    return bRet;
}

MS_U8 MSB1237_I2C_WriteByte(MS_U16 u16SlaveAddr, MS_U32 u32Addr, MS_U8 u8Data)
{
    MS_BOOL bRet=TRUE;
    MS_U8 u8MsbData[6] = {0};

    u8MsbData[0] = 0x10;
    u8MsbData[1] = 0x00;
    u8MsbData[2] = 0x00;
    u8MsbData[3] = (u32Addr >> 8) &0xff;
    u8MsbData[4] = u32Addr &0xff;
    u8MsbData[5] = u8Data;

    u8MsbData[0] = 0x35;
    bRet &= MSB1237_IIC_Write(u16SlaveAddr, 0, 0, 1, u8MsbData);
    u8MsbData[0] = 0x10;
    bRet &= MSB1237_IIC_Write(u16SlaveAddr, 0, 0, 6, u8MsbData);
    u8MsbData[0] = 0x34;
    bRet &= MSB1237_IIC_Write(u16SlaveAddr, 0, 0, 1, u8MsbData);

    return bRet;
}


MS_BOOL MSB1237_I2C_CH_Reset(MS_U8 ch_num,MS_BOOL enable)
{
    MS_BOOL bRet  =TRUE;
    MS_U8         data[5] = {0x53, 0x45, 0x52, 0x44, 0x42};

    if(enable)        //in 7853,always execute this
    {
        data[0] = 0x53;
        bRet &=MSB1237_IIC_Write(DEMOD_MSB1237_SLAVE_ID, 0, 0, 5, data);
    }
    data[0] = 0x71;
    bRet &=MSB1237_IIC_Write(DEMOD_MSB1237_SLAVE_ID, 0, 0, 1, data);
    data[0] = (ch_num & 0x01)? 0x81 : 0x80;
    bRet &=MSB1237_IIC_Write(DEMOD_MSB1237_SLAVE_ID, 0, 0, 1, data);
    data[0] = (ch_num & 0x02)? 0x83 : 0x82;
    bRet &=MSB1237_IIC_Write(DEMOD_MSB1237_SLAVE_ID, 0, 0, 1, data);
    data[0] = (ch_num & 0x04)? 0x85 : 0x84;
    bRet &=MSB1237_IIC_Write(DEMOD_MSB1237_SLAVE_ID, 0, 0, 1, data);
    data[0] = 0x53;
    bRet &=MSB1237_IIC_Write(DEMOD_MSB1237_SLAVE_ID, 0, 0, 1, data);
    data[0] = 0x7f;
    bRet &=MSB1237_IIC_Write(DEMOD_MSB1237_SLAVE_ID, 0, 0, 1, data);

    return bRet;

}


#define MSB1237_CHIP_ID 0x77
MS_BOOL MSB1237_Check_Exist(MS_U8 u8DemodIndex, MS_U8* pu8SlaveID)
{
    MS_U8 u8_tmp = 0;


    if(!MSB1237_I2C_CH_Reset(3,TRUE))
    {
        printf("[MSB1237] I2C_CH_Reset fail \n");
        //sreturn FALSE;
    }

    if (!MSB1237_I2C_ReadByte(DEMOD_MSB1237_SLAVE_ID,0x0900, &u8_tmp))
    {
        printf("[MSB1237] Read  Chip ID fail \n");
    }

    printf("[MSB1237] read id :%x \n",u8_tmp );

    *pu8SlaveID = DEMOD_MSB1237_SLAVE_ID;
    return (u8_tmp == MSB1237_CHIP_ID);
}


//=======================================================================================
MS_BOOL MSB1237_Extension_Function(MS_U8 u8DemodIndex, DEMOD_EXT_FUNCTION_TYPE fuction_type, void *data)
{
    MS_BOOL bret = TRUE;
    switch(fuction_type)
    {
        case DEMOD_EXT_FUNC_OPEN:
            bret &= MSB1237_Demod_Open(u8DemodIndex);
            break;
        case DEMOD_EXT_FUNC_ATSC_GET_PRELOCK:
            bret &= MSB1237_Demod_GetPreLockStatus(u8DemodIndex,(EN_LOCK_STATUS*)(data));
            break;
        case DEMOD_EXT_FUNC_ATSC_GET_FLAME_SYNC_LOCK:
            bret &= MSB1237_Demod_GetFlameSyncLockStatus(u8DemodIndex,(EN_LOCK_STATUS*)(data));
            break;
        case DEMOD_EXT_FUNC_FINALIZE:
            if (_s32MutexId >= 0)
            {
               bret &= MsOS_DeleteMutex(_s32MutexId);
               _s32MutexId = -1;
            }
            bInited = FALSE;
            bret &= MDrv_DMD_ATSC_Exit();
            break;
        default:
            printf("Request extension function (%x) does not exist\n",fuction_type);
            break;
    }
    return bret;
}

DRV_DEMOD_TABLE_TYPE GET_DEMOD_ENTRY_NODE(DEMOD_MSB1237) DDI_DRV_TABLE_ENTRY(demodtab) =
{
     .name                         = "DEMOD_MSB1237",
     .data                         = DEMOD_MSB1237,
     .init                         = MSB1237_Demod_Init,
     .GetLock                      = MSB1237_Demod_GetLock,
     .GetSNR                       = MSB1237_Demod_GetSNR,
     .GetBER                       = MSB1237_Demod_GetBER,
     .GetPWR                       = MSB1237_Demod_GetPWR,
     .GetQuality                   = MSB1237_Demod_GetSignalQuality,
     .GetParam                     = MSB1237_Demod_GetParam,
     .Restart                      = MSB1237_Demod_Restart,
     .CheckExist                   = MSB1237_Check_Exist,
     .I2CByPass                    = MSB1237_Demod_I2C_ByPass,
     .I2CByPassPreSetting          = NULL,
     .Extension_Function           = MSB1237_Extension_Function,
     .Extension_FunctionPreSetting = NULL,
     .Get_Packet_Error             = MDrv_Demod_null_Get_Packet_Error,     
#if MS_DVBT2_INUSE
     .SetCurrentDemodType          = MDrv_Demod_null_SetCurrentDemodType,
     .GetCurrentDemodType          = MDrv_Demod_null_GetCurrentDemodType,
     .GetPlpBitMap                 = MDrv_Demod_null_GetPlpBitMap,
     .GetPlpGroupID                = MDrv_Demod_null_GetPlpGroupID,
     .SetPlpGroupID                = MDrv_Demod_null_SetPlpGroupID,
     .GetNextPLPID                 = MDrv_Demod_null_GetNextPLPID,
     .GetPLPType                   = MDrv_Demod_null_GetPLPType,
#endif
#if MS_DVBS_INUSE
     .BlindScanStart               = MDrv_Demod_null_BlindScan_Start,
     .BlindScanNextFreq            = MDrv_Demod_null_BlindScan_NextFreq,
     .BlindScanWaitCurFreqFinished = MDrv_Demod_null_BlindScan_WaitCurFreqFinished,
     .BlindScanCancel              = MDrv_Demod_null_BlindScan_Cancel,
     .BlindScanEnd                 = MDrv_Demod_null_BlindScan_End,
     .BlindScanGetChannel          = MDrv_Demod_null_BlindScan_GetChannel,
     .BlindScanGetCurrentFreq      = MDrv_Demod_null_BlindScan_GetCurrentFreq,
     .DiSEqCSetTone                = MDrv_Demod_null_DiSEqC_SetTone,
     .DiSEqCSetLNBOut              = MDrv_Demod_null_DiSEqC_SetLNBOut,
     .DiSEqCGetLNBOut              = MDrv_Demod_null_DiSEqC_GetLNBOut,
     .DiSEqCSet22kOnOff            = MDrv_Demod_null_DiSEqC_Set22kOnOff,
     .DiSEqCGet22kOnOff            = MDrv_Demod_null_DiSEqC_Get22kOnOff,
     .DiSEqC_SendCmd               = MDrv_Demod_null_DiSEqC_SendCmd
#endif
};

/***********************************************************************************
************************************************************************************/
#undef _MSB1237_C_

#endif
