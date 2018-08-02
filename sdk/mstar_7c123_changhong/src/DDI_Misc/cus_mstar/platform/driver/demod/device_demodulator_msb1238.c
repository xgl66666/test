#define _MSB1238_C_

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


#if IF_THIS_DEMOD_INUSE(DEMOD_MSB1238)//1238????

#include "drvDMD_DTMB.h" //drvDMD_DTMB.h
#include "device_demodulator_msb1238.h"//1238.h???

#include "apiDigiTuner.h"


#define ERR_DEMOD_MSB(x)            //  x
#define DBG_DOMOD_MSB(x)            //   x
#define DBG_DEMOD_CHECK_LOCK(x)   // x
#define DBG_DEMOD_INFO(x)   // x
#define DBG_SSI(x)        //  x
#define DBG_SQI(x)        //  x
#define DBG_MSB(x)              x
#define COFDMDMD_MUTEX_TIMEOUT       (2000)

#define DTMB_CASHMERE_SLAVE_ID  0xD2  //0xF2

#if IF_THIS_TUNER_INUSE(TUNER_MXL603)
#include "MxL603_TunerApi.h"
#define MXL603_I2C_ADDR         0xC0
#endif

#if IF_THIS_TUNER_INUSE(TUNER_MXL603)
extern MS_BOOL MDrv_Tuner_Init(void);
extern MS_U32 MDrv_Tuner_SetTuner(MS_U8 u8TunerIndex,MS_U32 dwFreq /*Khz*/, MS_U8 ucBw /*MHz*/);

#define DTMB_TUNER_IF           5000 //4000//5000
#else
#define DTMB_TUNER_IF           3250
#endif
#define usleep(x)                   MsOS_DelayTask(x/1000)

#if TS_PARALLEL_OUTPUT
#define DTMB_TS_SERIAL        0x00
#else
#define DTMB_TS_SERIAL        0x01
#endif
// Division number of TS clock rate
// Formula: TS Clk Rate= 216 MHz/reg_dtmb_ts_clk_divnum
// Ex: DTMB_TS_CLK_DIVNUM = 31 => TS Clk = 216/31= 6.967 MHz
#define DTMB_TS_CLK_DIVNUM_SERIAL_MODE   2 //0 // 4
#define DTMB_TS_CLK_DIVNUM_PARALLEL_MODE  6//26//31

#if (DTMB_TS_SERIAL)
#define DTMB_TS_CLK_DIVNUM    DTMB_TS_CLK_DIVNUM_SERIAL_MODE
#else
#define DTMB_TS_CLK_DIVNUM    DTMB_TS_CLK_DIVNUM_PARALLEL_MODE
#endif

#define DTMB_TS_DATA_SWAP  0x00  // TS data inversion
#if TS_PARALLEL_OUTPUT
#define DTMB_TS_CLK_INV    0  //TS_CLK_INV  // TS clock inverseion //this define will depend on hw main board
#else
#define DTMB_TS_CLK_INV    0 //1
#endif
#define DTMB_IQ_SWAP   0x00          // IQ Swap  //this define will depend on tuner
#define DTMB_IFAGC_REF 0x500              //this define will depend on tuner
#define DTMB_TDI_START_ADDR (DMD_DTMB_ADR/16)   // System must allocate a 7MB memory space starting from this (128-bit)address
//static BOOL s_bI2C_FastMode = false;

//extern DEV_FRONTEND_TYPE frontendtab[MAX_FRONTEND_NUM]; // <= not good

//Local variables//
static MS_S32                       _s32TaskId  = -1;
static MS_S32                       _s32MutexId;
static MS_BOOL                      bInited     = FALSE;
static DEMOD_MS_INIT_PARAM          InitParam[MAX_FRONTEND_NUM];


MS_U32 Scan_Time_Start;

/***********************************************************************************
************************************************************************************/



MS_BOOL MSB1238_IIC_Write(MS_U16 u16BusNumSlaveID, MS_U8 u8addrcount, MS_U8* pu8addr, MS_U16 u16size, MS_U8* pu8data);
MS_BOOL MSB1238_IIC_Read(MS_U16 u16BusNumSlaveID, MS_U8 u8AddrNum, MS_U8* paddr, MS_U16 u16size, MS_U8* pu8data);
MS_BOOL MSB1238_I2C_CH_Reset(MS_U8 ch_num,MS_BOOL enable);
MS_U8 MSB1238_I2C_WriteByte(MS_U16 u16SlaveAddr, MS_U32 u32Addr, MS_U8 u8Data);


// Global function//
MS_BOOL MSB1238_Demod_Init(MS_U8 u8DemodIndex,DEMOD_MS_INIT_PARAM* pParam)
{
    DMD_DTMB_InitData sDMD_DTMB_InitData;
    static MS_U8 u8DMD_DTMB_InitExt[]={1};

    //bInited = FALSE;
    printf("-----%s start-----\n", __FUNCTION__);

    if (_s32TaskId >= 0)
    {
        return TRUE;
    }

    _s32MutexId = MsOS_CreateMutex(E_MSOS_FIFO, "OfDmd_Mutex", MSOS_PROCESS_SHARED);

    if (_s32MutexId < 0)
    {
        DMD_ERR(("-----%s _s32MutexId < 0-----\n", __FUNCTION__));
        return FALSE;
    }

    MDrv_DMD_PreInit();

    // copy tuner config

    if(pParam->pstTunertab == NULL)
        return FALSE;
    else
        InitParam[u8DemodIndex].pstTunertab = pParam->pstTunertab;

    sDMD_DTMB_InitData.u16IF_KHZ = 5000;//u16DtvIFFreqKHz;
    sDMD_DTMB_InitData.bIQSwap = DTMB_IQ_SWAP;//u8IqSwap;

    //sDMD_DTMB_InitData.eSR_TYPE = SR_8M;
    sDMD_DTMB_InitData.u8IS_DUAL = FALSE;
    sDMD_DTMB_InitData.bIsUseSspiLoadCode = FALSE;

    sDMD_DTMB_InitData.u16DTMBAGCLockCheckTime = 50;
    sDMD_DTMB_InitData.u16DTMBPreLockCheckTime = DTMB_PreLock_timeout;
    sDMD_DTMB_InitData.u16DTMBPNMLockCheckTime = DTMB_PNMLock_timeout;
    sDMD_DTMB_InitData.u16DTMBFECLockCheckTime = DTMB_FEC_timeout;

    sDMD_DTMB_InitData.u16QAMAGCLockCheckTime = 50;
    sDMD_DTMB_InitData.u16QAMPreLockCheckTime = 1000;
    sDMD_DTMB_InitData.u16QAMMainLockCheckTime = 3000;


    sDMD_DTMB_InitData.u8DMD_DTMB_InitExt = u8DMD_DTMB_InitExt;
    // sanghyun
    //sDMD_DTMB_InitData.u16AgcReferenceValue = DTMB_IFAGC_REF; // Hugo enable for test

    //TS[begin]
    //Parallel mode
    sDMD_DTMB_InitData.u5TsConfigByte_DivNum = DTMB_TS_CLK_DIVNUM;
    sDMD_DTMB_InitData.u1TsConfigByte_ClockInv = DTMB_TS_CLK_INV;
    sDMD_DTMB_InitData.u1TsConfigByte_DataSwap = DTMB_TS_DATA_SWAP;
    sDMD_DTMB_InitData.u1TsConfigByte_SerialMode = DTMB_TS_SERIAL;
    //Serial mode
    //sDMD_ISDBT_InitData.u5TsConfigByte_DivNum = 4;
    //sDMD_ISDBT_InitData.u1TsConfigByte_ClockInv = 0;
    //sDMD_ISDBT_InitData.u1TsConfigByte_DataSwap = 0;
    //sDMD_ISDBT_InitData.u1TsConfigByte_SerialMode = 1;
    //TS[end]

    //I2C[begin]
    sDMD_DTMB_InitData.u8I2CSlaveAddr = DTMB_CASHMERE_SLAVE_ID;
    sDMD_DTMB_InitData.u8I2CSlaveBus = 0;
    //MAX
    sDMD_DTMB_InitData.bIsExtDemod = TRUE;
      //sDMD_DTMB_InitData.u16AgcReferenceValue = true;
    sDMD_DTMB_InitData.I2C_WriteBytes = MSB1238_IIC_Write;

    sDMD_DTMB_InitData.I2C_ReadBytes = MSB1238_IIC_Read;
    //I2C[end]

    //MDrv_EXTERNAL_DMD_DTMB_Link_Func();

    MDrv_DMD_DTMB_Initial_Hal_Interface();

    MDrv_DMD_DTMB_Init(&sDMD_DTMB_InitData, sizeof(sDMD_DTMB_InitData));

    MDrv_DMD_DTMB_SetConfig(DMD_DTMB_DEMOD_DTMB,TRUE);

    bInited = TRUE;

    return TRUE;


}

MS_BOOL MSB1238_Demod_Open(void)
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

MS_BOOL MSB1238_Demod_Close(void)
{
    return TRUE;
}

MS_BOOL MSB1238_Demod_Reset(void)
{
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    else
    {
        if(!MDrv_DMD_DTMB_SetReset())//(!MSB1238_SoftReset())
        {
            DBG_MSB(HB_printf("%s: Soft reset failed.\n", __FUNCTION__));
            return FALSE;
        }
    }

    HB_ReleaseMutex(_s32MutexId);
    return TRUE;

}

MS_BOOL MSB1238_Demod_TsOut(MS_BOOL bEnable)
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

MS_BOOL MSB1238_Demod_I2C_ByPass(MS_U8 u8DemodIndex, MS_BOOL bEnable)
{
    MS_BOOL bret = TRUE;
    if(bInited)
    {
        if(bEnable)
            return MDrv_DMD_DTMB_IIC_BYPASS_MODE(TRUE);
        else
            return MDrv_DMD_DTMB_IIC_BYPASS_MODE(FALSE);
    }
    else
    {
        bret &= MSB1238_I2C_CH_Reset(3,TRUE);
        bret &= MSB1238_I2C_WriteByte(DTMB_CASHMERE_SLAVE_ID,0x0951, 0x00);
        if(bEnable)
            bret &= MSB1238_I2C_WriteByte(DTMB_CASHMERE_SLAVE_ID,0x0910, 0x10);
        else
            bret &= MSB1238_I2C_WriteByte(DTMB_CASHMERE_SLAVE_ID,0x0910, 0x00);
        return bret;
    }
}

MS_BOOL MSB1238_Demod_PowerOnOff(MS_BOOL bPowerOn)
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

MS_BOOL MSB1238_Demod_SetBW(MS_U32 u32BW)
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

MS_BOOL MSB1238_Demod_GetBW(MS_U32 *pu32BW)
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

MS_BOOL MSB1238_Demod_GetLock(MS_U8 u8DemodIndex, EN_LOCK_STATUS *peLockStatus)
{
    DMD_DTMB_LOCK_STATUS eLockStatus = DMD_DTMB_UNLOCK;
    *peLockStatus =E_DEMOD_CHECKING;

    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    if(FALSE == bInited)
    {
        HB_printf("[%s]MSB1238 have not inited !!!\n",__FUNCTION__);
        HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }


    eLockStatus= MDrv_DMD_DTMB_GetLock(DMD_DTMB_GETLOCK);   //DMD_DTMB_LOCK

    switch (eLockStatus) // _UTOPIA
    {
    case DMD_DTMB_LOCK:
        *peLockStatus = E_DEMOD_LOCK;
        break;
    case DMD_DTMB_CHECKING:
        *peLockStatus = E_DEMOD_CHECKING;
        break;
    case DMD_DTMB_CHECKEND:
        *peLockStatus = E_DEMOD_CHECKEND;
        break;
    case DMD_DTMB_UNLOCK:
        *peLockStatus = E_DEMOD_UNLOCK;
        break;
    default:
        DBG_MSB(HB_printf("MDrv_DMD_DTMB_GetLock error\n"));
        break;
    }

    HB_ReleaseMutex(_s32MutexId);
    return TRUE;

}

MS_BOOL MSB1238_Demod_GetSignalQuality(MS_U8 u8DemodIndex, MS_U16 *pu16quality)
{
   // MS_U8 u8snr=0;

    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    else
    {
        //if (TRUE == MSB1238_GetLock())
        if( MDrv_DMD_DTMB_GetLock(DMD_DTMB_GETLOCK)== DMD_DTMB_LOCK)
        {
           *pu16quality  =   (MS_U16)MDrv_DMD_DTMB_GetSignalQuality();
            //u8snr = MDrv_DMD_DTMB_GetSNRPercentage();
            //*pu32SNR= (MS_U32) u8snr;//*0.4;
            //*pu32SNR = MSB1238_ReadSNRdB();
        }

        else
        {
            *pu16quality = 0;
        }
    }

    HB_ReleaseMutex(_s32MutexId);
    return TRUE ;

}

MS_BOOL MSB1238_Demod_GetBER(MS_U8 u8DemodIndex, float *pfBER)
{
    MS_U32  dwError = 0;

    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    else
    {
        *pfBER = -1;
    }

    HB_ReleaseMutex(_s32MutexId);
    return (dwError ) ? FALSE : TRUE;
}

MS_BOOL MSB1238_Demod_GetPWR(MS_U8 u8DemodIndex, MS_S32 *ps32Signal)
{
#define DEFAULT_PWR_MIN (-97)
#define DEFAULT_PWR_MAX (-5)

    MS_U32  dwError=0;
    MS_U8 u8snr;
 // MS_S32  wSignal;

    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    else
    {
        //if (TRUE == MSB1238_Demod_GetLock(u8DemodIndex, DMD_DTMB_LOCK))
        if( MDrv_DMD_DTMB_GetLock(DMD_DTMB_GETLOCK)== DMD_DTMB_LOCK)
        {
           //Signal = lgdt3305_SignalStatus(DEMOD_TYPE);
           //       wSignal = lgdt3305_IfAcc();
          //rintf("PWR = 0x%x \n",wSignal);

         //u8snr = MDrv_DMD_DTMB_GetSNRPercentage();
         u8snr = MDrv_DMD_DTMB_GetSignalQuality();
         *ps32Signal = (MS_U32) u8snr;
            //*ps32Signal =  MSB1238_ReadSNRPercentage();
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

MS_BOOL MSB1238_Demod_Config(MS_U8 *pRegParam)
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

MS_BOOL MSB1238_Demod_GetParam(MS_U8 u8DemodIndex, DEMOD_MS_FE_CARRIER_PARAM* pParam)
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


MS_BOOL MSB1238_Demod_Restart(MS_U8 u8DemodIndex, DEMOD_MS_FE_CARRIER_PARAM* pParam,MS_U32 u32BroadCastType)
{
    //MS_U32  NoError = 0;
    MS_BOOL bret;

    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(HB_printf("MDrv_COFDMDMD_Restart: Obtain mutex failed.\n"));
        return FALSE;
    }
    else
    {
        //MDrv_Demod_I2C_ByPass(TRUE);
        // bret = MSB1238_Demod_I2C_ByPass(u8DemodIndex, TRUE);
        //NoError = MDrv_Tuner_SetTuner(0, pParam->u32Frequency, pParam->TerParam.eBandWidth);
        //MDrv_Demod_I2C_ByPass(FALSE);

        MSB1238_Demod_I2C_ByPass(u8DemodIndex, FALSE);

        //MsOS_DelayTask(250); //necessary for NMI120 get RSSI
        if(!MDrv_DMD_DTMB_SetReset())//(!MSB1238_SoftReset())
        {
          HB_printf("-----%s FAIL-----\n", __FUNCTION__);
          return FALSE;
        }
        MsOS_DelayTask(100); // ori = 10
        //bret = MDrv_DMD_DTMB_SetDtmbMode();
        bret = MDrv_DMD_DTMB_SetConfig(DMD_DTMB_DEMOD_DTMB,TRUE);
        if (bret == TRUE)
          HB_printf("MDrv_DMD_DTMB_SetConfig OK ! \n");

    }

    HB_ReleaseMutex(_s32MutexId);
    //return (NoError) ? TRUE : FALSE;
    return bret;

}

MS_BOOL MSB1238_Demod_SetMode(Demod_Mode* pMode)
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

MS_BOOL MSB1238_Demod_SetOutoutPath(DEMOD_INTERFACE_MODE path)
{
    //Dummy function
    return TRUE;
}


MS_BOOL MSB1238_IIC_Write(MS_U16 u16BusNumSlaveID, MS_U8 u8addrcount, MS_U8* pu8addr, MS_U16 u16size, MS_U8* pu8data)
{
    MS_BOOL bResult = TRUE;
    //printf("Enter MSB1238_IIC_Write\n");
    //printf("u16BusNumSlaveID=%x\n",u16BusNumSlaveID);
    bResult = MDrv_IIC_Write(u16BusNumSlaveID, pu8addr, u8addrcount, pu8data,  u16size);
    //printf("\n===talent_%s=%d=slaveid=%d\n",__func__,bResult,u16BusNumSlaveID);
    if(!bResult)
    {
        printf("1238C IIC Write Erro..\n");
    }
    return bResult;
}

MS_BOOL MSB1238_IIC_Read(MS_U16 u16BusNumSlaveID, MS_U8 u8AddrNum, MS_U8* paddr, MS_U16 u16size, MS_U8* pu8data)
{
    MS_BOOL bResult = TRUE;
    //printf("Enter MDrv_MSB140X_IIC_Read\n");

    bResult = MDrv_IIC_Read(u16BusNumSlaveID, paddr, u8AddrNum, pu8data,  u16size);
    if(!bResult)
    {
        printf("1238C IIC Read Erro..\n");
    }
    return bResult;
}



DEMOD_INTERFACE_MODE MSB1238_Demod_GetOutoutPath(void)
{
    //Dummy function
    return DEMOD_INTERFACE_PARALLEL;
}

MS_U16 MSB1238_Demod_ReadReg(MS_U16 RegAddr)
{
    MS_U8 RegData = 0;
    //MSB1238_ReadReg(RegAddr, &RegData);
    MDrv_DMD_DTMB_GetReg(RegAddr,&RegData);
    return (MS_U16) RegData;
}

MS_BOOL MSB1238_Demod_WriteReg(MS_U16 RegAddr, MS_U16 RegData)
{
      MS_U8 u8RegData = 0;

      u8RegData= (MS_U8) RegData;
    return MDrv_DMD_DTMB_SetReg(RegAddr, u8RegData);
    //return MSB1238_WriteReg(RegAddr, RegData);
}

//===================================================================================
MS_U8 MSB1238_I2C_ReadByte(MS_U16 u16SlaveAddr, MS_U32 u32Addr, MS_U8 *pu8Data)
{

    MS_BOOL bRet=TRUE;
    MS_U8 u8MsbData[6] = {0};

    u8MsbData[0] = 0x10;
    u8MsbData[1] = 0x00;
    u8MsbData[2] = 0x00;
    u8MsbData[3] = (u32Addr >> 8) &0xff;
    u8MsbData[4] = u32Addr &0xff;

    u8MsbData[0] = 0x35;
    bRet &= MSB1238_IIC_Write(u16SlaveAddr, 0, 0, 1, u8MsbData);

    u8MsbData[0] = 0x10;
    bRet &= MSB1238_IIC_Write(u16SlaveAddr, 0, 0, 5, u8MsbData);
    bRet &= MSB1238_IIC_Read(u16SlaveAddr, 0, 0, 1, pu8Data);

    u8MsbData[0] = 0x34;
    bRet &= MSB1238_IIC_Write(u16SlaveAddr, 0, 0, 1, u8MsbData);

    return bRet;
}

MS_U8 MSB1238_I2C_WriteByte(MS_U16 u16SlaveAddr, MS_U32 u32Addr, MS_U8 u8Data)
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
    bRet &= MSB1238_IIC_Write(u16SlaveAddr, 0, 0, 1, u8MsbData);
    u8MsbData[0] = 0x10;
    bRet &= MSB1238_IIC_Write(u16SlaveAddr, 0, 0, 6, u8MsbData);
    u8MsbData[0] = 0x34;
    bRet &= MSB1238_IIC_Write(u16SlaveAddr, 0, 0, 1, u8MsbData);

    return bRet;
}

MS_BOOL MSB1238_I2C_CH_Reset(MS_U8 ch_num,MS_BOOL enable)
{
    MS_BOOL bRet  =TRUE;
    MS_U8         data[5] = {0x53, 0x45, 0x52, 0x44, 0x42};

    if(enable)        //in 7853,always execute this
    {
        data[0] = 0x53;
        bRet &=MSB1238_IIC_Write(DTMB_CASHMERE_SLAVE_ID, 0, 0, 5, data);
    }
    data[0] = 0x71;
    bRet &=MSB1238_IIC_Write(DTMB_CASHMERE_SLAVE_ID, 0, 0, 1, data);
    data[0] = (ch_num & 0x01)? 0x81 : 0x80;
    bRet &=MSB1238_IIC_Write(DTMB_CASHMERE_SLAVE_ID, 0, 0, 1, data);
    data[0] = (ch_num & 0x02)? 0x83 : 0x82;
    bRet &=MSB1238_IIC_Write(DTMB_CASHMERE_SLAVE_ID, 0, 0, 1, data);
    data[0] = (ch_num & 0x04)? 0x85 : 0x84;
    bRet &=MSB1238_IIC_Write(DTMB_CASHMERE_SLAVE_ID, 0, 0, 1, data);
    data[0] = 0x53;
    bRet &=MSB1238_IIC_Write(DTMB_CASHMERE_SLAVE_ID, 0, 0, 1, data);
    data[0] = 0x7f;
    bRet &=MSB1238_IIC_Write(DTMB_CASHMERE_SLAVE_ID, 0, 0, 1, data);

    return bRet;

}


#define MSB1238_CHIP_ID 0x63
MS_BOOL MSB1238_Check_Exist(MS_U8 u8DemodIndex, MS_U8* pu8SlaveID)
{
    MS_U8 u8_tmp = 0;


    if(!MSB1238_I2C_CH_Reset(3,TRUE))
    {
        printf("[MSB1238] I2C_CH_Reset fail \n");
        //sreturn FALSE;
    }

    if (!MSB1238_I2C_ReadByte(DTMB_CASHMERE_SLAVE_ID,0x0900, &u8_tmp))
    {
        printf("[MSB1238] Read  Chip ID fail \n");
    }

    printf("[MSB1238] read id :%x \n",u8_tmp );

    *pu8SlaveID = DTMB_CASHMERE_SLAVE_ID;
    return (u8_tmp == MSB1238_CHIP_ID);
}


//=======================================================================================
MS_BOOL MSB1238_Extension_Function(MS_U8 u8DemodIndex, DEMOD_EXT_FUNCTION_TYPE fuction_type, void *data)
{
    MS_BOOL bret = TRUE;
    switch(fuction_type)
    {
            case DEMOD_EXT_FUNC_FINALIZE:
            if (_s32MutexId >= 0)
            {
               bret &= MsOS_DeleteMutex(_s32MutexId);
               _s32MutexId = -1;
            }
            bInited = FALSE;
            bret &= MDrv_DMD_DTMB_Exit();
            break;
        default:
            printf("Request extension function (%x) does not exist\n",fuction_type);
            break;
    }
    return bret;
}

DRV_DEMOD_TABLE_TYPE GET_DEMOD_ENTRY_NODE(DEMOD_MSB1238) DDI_DRV_TABLE_ENTRY(demodtab) =
{
     .name                         = "DEMOD_MSB1238",
     .data                         = DEMOD_MSB1238,
     .init                         = MSB1238_Demod_Init,
     .GetLock                      = MSB1238_Demod_GetLock,
     .GetSNR                       = MDrv_Demod_null_GetSNR,
     .GetBER                       = MSB1238_Demod_GetBER,
     .GetPWR                       = MSB1238_Demod_GetPWR,
     .GetQuality                   = MSB1238_Demod_GetSignalQuality,
     .GetParam                     = MSB1238_Demod_GetParam,
     .Restart                      = MSB1238_Demod_Restart,
     .CheckExist                   = MSB1238_Check_Exist,
     .I2CByPass                    = MSB1238_Demod_I2C_ByPass,
     .I2CByPassPreSetting          = NULL,
     .Extension_Function           = MSB1238_Extension_Function,
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
#undef _MSB1238_C_

#endif
