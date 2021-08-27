/**********************************************************************
Copyright (c) 2006-2009 MStar Semiconductor, Inc.
All rights reserved.
Unless otherwise stipulated in writing, any and all information contained
herein regardless in any format shall remain the sole proprietary of
MStar Semiconductor Inc. and be kept in strict confidence
(MStar Confidential Information) by the recipient.
Any unauthorized act including without limitation unauthorized disclosure,
copying, use, reproduction, sale, distribution, modification, disassembling,
reverse engineering and compiling of the contents of MStar Confidential
Information is unlawful and strictly prohibited. MStar hereby reserves the
rights to any and all damages, losses, costs and expenses resulting therefrom.
 **********************************************************************/
#include <math.h>
#include "MsCommon.h"
#include "drvIIC.h"
#include "HbCommon.h"

#include "MsOS.h"
#include "Board.h"
#include "drvTuner.h"
#include "drvDemod.h"
#include "drvDemodNull.h"

#if IF_THIS_DEMOD_INUSE(DEMOD_MSB1400)
#include "drvDMD_common.h"
#include "drvDMD_ISDBT.h"       //dan add for integrate ISDBT utopia driver
#include "drvDMD_VD_MBX.h"


#define MSB140x_Delayms  MsOS_DelayTask
#define DEMOD_MSB140X_SLAVE_ID  0xD2
#define ISDBTDMD_MUTEX_TIMEOUT       (2000)

#define DEMOD_DEBUG 1
#if DEMOD_DEBUG
#define PRINTF(p)       printf p
#else
#define PRINTF(p)
#endif

// Division number of TS clock rate
// Formula: TS Clk Rate= 216 MHz/reg_isdbt_ts_clk_divnum
// Ex: ISDBT_TS_CLK_DIVNUM = 31 => TS Clk = 216/31= 6.967 MHz
#if TS_PARALLEL_OUTPUT
#define ISDBT_TS_SERIAL       0x00
#define ISDBT_TS_CLK_DIVNUM   31
#else
#define ISDBT_TS_SERIAL       0x01
#define ISDBT_TS_CLK_DIVNUM   4
#endif

#define ISDBT_TS_DATA_SWAP  0x00  // TS data inversion
#define ISDBT_TS_CLK_INV      0x01   // TS clock inverseion          //this define will depend on hw main board
#define ISDBT_IQ_SWAP   0x01          // IQ Swap                    //this define will depend on tuner
#define ISDBT_IFAGC_REF   0x38                                      //this define will depend on tuner

//#if IF_THIS_TUNER_INUSE(TUNER_RT820T) 
//#define TUNER_IFFREQ   4063
//#elif IF_THIS_TUNER_INUSE(TUNER_MXL136)  //IF=4M 
//#define TUNER_IFFREQ   4000
//#else
//#define TUNER_IFFREQ   4800
//#endif

#if IF_THIS_TUNER_INUSE(TUNER_MXL603)   //IF= 6M
#define TUNER_IFFREQ   5000
#include "MxL603_TunerApi.h"
#define MXL603_I2C_ADDR         0xC0
extern MS_BOOL MDrv_Tuner_Init(MS_U8 u8TunerIndex);
extern MS_U32 MDrv_Tuner_SetTuner(MS_U8 u8TunerIndex,MS_U32 dwFreq /*Khz*/, MS_U8 ucBw /*MHz*/);
#endif

static MS_BOOL bInited = FALSE;
static MS_S32 _s32MutexId = -1;

MS_BOOL MSB1400_I2C_Channel_Set(MS_U16 u16SlaveAddr, MS_U8 ch_num);
MS_BOOL MSB1400_I2C_Channel_Change(MS_U16 u16SlaveAddr, MS_U8 ch_num);
MS_U8 MSB1400_I2C_WriteByte(MS_U16 u16SlaveAddr, MS_U32 u32Addr, MS_U8 u8Data);

//##########################################################################################################
//       IIC interface
//##########################################################################################################

MS_BOOL   MDrv_MSB140X_IIC_Write(MS_U16 u16BusNumSlaveID, MS_U8 u8addrcount, MS_U8* pu8addr, MS_U16 u16size, MS_U8* pu8data)
{
    if (FALSE == MDrv_IIC_Write(u16BusNumSlaveID, pu8addr, u8addrcount, pu8data,  u16size))
    {
        printf("Enter MDrv_MSB140X_IIC_Write error\n");
    }

    return TRUE;
}

MS_BOOL   MDrv_MSB140X_IIC_Read(MS_U16 u16BusNumSlaveID, MS_U8 u8AddrNum, MS_U8* paddr, MS_U16 u16size, MS_U8* pu8data)
{
    if (FALSE == MDrv_IIC_Read(u16BusNumSlaveID, paddr, u8AddrNum, pu8data,  u16size))
    {
        printf("Enter MDrv_MSB140X_IIC_Read error\n");
    }

    return TRUE;
}

//##########################################################################################################
//        Public:Common Function Implementation
//##########################################################################################################
/* For FE auto test */
MS_BOOL MSB1400_Demod_WriteReg(MS_U16 u16Addr, MS_U8 u8Data)
{
    MS_BOOL bRet = TRUE;

    bRet = MDrv_DMD_ISDBT_SetReg(u16Addr, u8Data);

    return bRet;
}

/* For FE auto test */
MS_U8 MSB1400_Demod_ReadReg(MS_U16 u16Addr)
{
    MS_BOOL bRet = TRUE;
    MS_U8 u8Data;

    bRet = MDrv_DMD_ISDBT_GetReg(u16Addr, &u8Data);

    return u8Data;
}

MS_BOOL MSB1400_Demod_Open(void)
{
    if (MsOS_ObtainMutex(_s32MutexId, ISDBTDMD_MUTEX_TIMEOUT) == FALSE)
    {
        PRINTF(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    if (FALSE == bInited)
    {
        PRINTF(("[%s]MSB140X have not inited !!!\n", __FUNCTION__));
        MsOS_ReleaseMutex(_s32MutexId);
        return FALSE;
    }

    MDrv_DMD_PreInit();
    MsOS_ReleaseMutex(_s32MutexId);
    return TRUE;
}

MS_BOOL MSB1400_Demod_PowerOnOff(MS_BOOL bPowerOn)
{
    return TRUE;
}

MS_BOOL MSB1400_Demod_Close(void)
{
    if (MsOS_ObtainMutex(_s32MutexId, ISDBTDMD_MUTEX_TIMEOUT) == FALSE)
    {
        PRINTF(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    if (FALSE == bInited)
    {
        PRINTF(("[%s]MSB140X have not inited !!!\n", __FUNCTION__));
        MsOS_ReleaseMutex(_s32MutexId);
        return FALSE;
    }

    MsOS_ReleaseMutex(_s32MutexId);
    return TRUE;
}

/****************************************************************************
**function:
**
**
****************************************************************************/
MS_BOOL MSB1400_Demod_GetSignalQuality(MS_U8 u8DemodIndex,MS_U16 *pu16quality)
{
    MS_BOOL bRet = TRUE;

    if (MsOS_ObtainMutex(_s32MutexId, ISDBTDMD_MUTEX_TIMEOUT) == FALSE)
    {
        PRINTF(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    if (FALSE == bInited)
    {
        PRINTF(("[%s]MSB140X have not inited !!!\n", __FUNCTION__));
        MsOS_ReleaseMutex(_s32MutexId);
        return FALSE;
    }

    *pu16quality = 0;
    if (DMD_ISDBT_LOCK == MDrv_DMD_ISDBT_GetLock(DMD_ISDBT_GETLOCK))
    {
        *pu16quality  = MDrv_DMD_ISDBT_GetSignalQuality();
    }
    
    if (*pu16quality > 100)
    {
        *pu16quality = 100;
    }
    //printf("MDrv_DMD_ISDBT_GetSignalQuality = %ld\n", *pu32SNR);
    MsOS_ReleaseMutex(_s32MutexId);
    return bRet;
}

/****************************************************************************
**function:
**
**
****************************************************************************/
MS_BOOL MSB1400_Demod_GetSNR(MS_U8 u8DemodIndex,float *pfSNR)
{
    MS_BOOL bRet = TRUE;

    if (MsOS_ObtainMutex(_s32MutexId, ISDBTDMD_MUTEX_TIMEOUT) == FALSE)
    {
        PRINTF(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    if (FALSE == bInited)
    {
        PRINTF(("[%s]MSB140X have not inited !!!\n", __FUNCTION__));
        MsOS_ReleaseMutex(_s32MutexId);
        return FALSE;
    }

    *pfSNR = 0;
    if (DMD_ISDBT_LOCK == MDrv_DMD_ISDBT_GetLock(DMD_ISDBT_GETLOCK))
    {
        bRet = MDrv_DMD_ISDBT_GetSNR(pfSNR);
    }

    //printf("MDrv_DMD_ISDBT_GetSignalQuality = %ld\n", *pu32SNR);
    MsOS_ReleaseMutex(_s32MutexId);
    return bRet;
}


/****************************************************************************
**function:
**
**
****************************************************************************/
MS_BOOL MSB1400_Demod_GetBER(MS_U8 u8DemodIndex,float *pfBER)
{
    MS_BOOL bRet = TRUE;

    if (MsOS_ObtainMutex(_s32MutexId, ISDBTDMD_MUTEX_TIMEOUT) == FALSE)
    {
        PRINTF(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    if (FALSE == bInited)
    {
        PRINTF(("[%s]MSB140X have not inited !!!\n", __FUNCTION__));
        MsOS_ReleaseMutex(_s32MutexId);
        return FALSE;
    }
    if (DMD_ISDBT_LOCK == MDrv_DMD_ISDBT_GetLock(DMD_ISDBT_GETLOCK))
    {
        bRet = MDrv_DMD_ISDBT_GetPostViterbiBer(E_ISDBT_Layer_A, pfBER);
    }
    else
    {
        *pfBER = 0.0;
    }

    //printf("dan Enter MDrv_Demod_GetBER = %f\n", *pfBER);
    MsOS_ReleaseMutex(_s32MutexId);
    return bRet;
}

MS_BOOL MSB1400_Demod_GetPWR(MS_U8 u8DemodIndex,MS_S32 *ps32Signal)
{
    MS_U16  u16SignalStrengthValue;

    if (MsOS_ObtainMutex(_s32MutexId, ISDBTDMD_MUTEX_TIMEOUT) == FALSE)
    {
        PRINTF(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    if (FALSE == bInited)
    {
        PRINTF(("[%s]MSB140X have not inited !!!\n", __FUNCTION__));
        MsOS_ReleaseMutex(_s32MutexId);
        return FALSE;
    }

    u16SignalStrengthValue = 0;
    if (DMD_ISDBT_LOCK == MDrv_DMD_ISDBT_GetLock(DMD_ISDBT_GETLOCK))
    {
        MDrv_DMD_ISDBT_GetSignalStrength(&u16SignalStrengthValue);
    }

    if (u16SignalStrengthValue > 100)
        u16SignalStrengthValue = 100;
    else if (u16SignalStrengthValue <= 0)
        u16SignalStrengthValue = 0;
    *ps32Signal = (MS_S32)u16SignalStrengthValue;

    MsOS_ReleaseMutex(_s32MutexId);
    return TRUE;
}

MS_BOOL MSB1400_Demod_GetLock(MS_U8 u8DemodIndex,EN_LOCK_STATUS *peLockStatus)
{
    DMD_ISDBT_LOCK_STATUS eLockStatus;

    *peLockStatus = E_DEMOD_CHECKING;
    if (MsOS_ObtainMutex(_s32MutexId, ISDBTDMD_MUTEX_TIMEOUT) == FALSE)
    {
        PRINTF(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    if (FALSE == bInited)
    {
        PRINTF(("[%s]MSB140X have not inited !!!\n", __FUNCTION__));
        MsOS_ReleaseMutex(_s32MutexId);
        return FALSE;
    }

    eLockStatus = MDrv_DMD_ISDBT_GetLock(DMD_ISDBT_GETLOCK);
    
    switch (eLockStatus) // _UTOPIA
    {
        case DMD_ISDBT_LOCK:
            *peLockStatus = E_DEMOD_LOCK;
            break;
        case DMD_ISDBT_CHECKING:
            *peLockStatus = E_DEMOD_CHECKING;
            break;
        case DMD_ISDBT_CHECKEND:
            *peLockStatus = E_DEMOD_CHECKEND;
            break;
        case DMD_ISDBT_UNLOCK:
            *peLockStatus = E_DEMOD_UNLOCK;
            break;
        default:
            PRINTF(("ISDBTT_GetLockStatus error\n"));
            break;
    }
    MsOS_ReleaseMutex(_s32MutexId);
    return TRUE;
}


MS_BOOL MSB1400_Demod_Restart(MS_U8 u8DemodIndex,DEMOD_MS_FE_CARRIER_PARAM *pParam,MS_U32 u32BroadCastType)
{
    MS_BOOL  bRet = TRUE;

    if (MsOS_ObtainMutex(_s32MutexId, ISDBTDMD_MUTEX_TIMEOUT) == FALSE)
    {
        PRINTF(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    if (FALSE == bInited)
    {
        PRINTF(("[%s]MSB140X have not inited !!!\n", __FUNCTION__));
        MsOS_ReleaseMutex(_s32MutexId);
        return FALSE;
    }

    //set freq and bw to tuner
    //bRet &= MDrv_DMD_ISDBT_IIC_BYPASS_MODE(TRUE);
    //bRet &= MDrv_Tuner_SetTuner(0,pParam->u32Frequency, 0);//0:6Mhz
    //bRet &= MDrv_DMD_ISDBT_IIC_BYPASS_MODE(FALSE);
    bRet &= MDrv_DMD_ISDBT_SetConfig(TRUE);
    
    MsOS_ReleaseMutex(_s32MutexId);
    return bRet;
}

MS_BOOL MSB1400_Demod_I2C_ByPass(MS_U8 u8DemodIndex,MS_BOOL bOn)
{
    MS_BOOL bret=TRUE;
    printf("MSB1400_Demod_I2C_ByPass\n");
    if (FALSE == bInited)
    {
        bret &= MSB1400_I2C_Channel_Set(DEMOD_MSB140X_SLAVE_ID,0);
        bret &= MSB1400_I2C_Channel_Change(DEMOD_MSB140X_SLAVE_ID,3);
        if(bOn)
           bret &= MSB1400_I2C_WriteByte(DEMOD_MSB140X_SLAVE_ID,0x0910, 0x10);
        else
           bret &= MSB1400_I2C_WriteByte(DEMOD_MSB140X_SLAVE_ID,0x0910, 0x00);
        return bret;
    }
    
    if (MsOS_ObtainMutex(_s32MutexId, ISDBTDMD_MUTEX_TIMEOUT) == FALSE)
    {
        PRINTF(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    bret &= MDrv_DMD_ISDBT_IIC_BYPASS_MODE(bOn);
    MsOS_ReleaseMutex(_s32MutexId);
    return bret;
}

MS_BOOL MSB1400_Demod_Init(MS_U8 u8DemodIndex,DEMOD_MS_INIT_PARAM* pParam)
{
    MS_BOOL bRet = TRUE;
    DMD_ISDBT_InitData sDMD_ISDBT_InitData;
    static MS_U8 u8DMD_ISDBT_InitExt[] = {1}; // RFAGC tristate control default value, 1:trisate 0:non-tristate,never modify unless you know the meaning

    printf("\nMSB1400_Demod_Init\n");
    if (_s32MutexId < 0)
    {
        _s32MutexId = MsOS_CreateMutex(E_MSOS_FIFO, "Isdbt_Mutex", MSOS_PROCESS_SHARED);

        if (_s32MutexId < 0)
        {
            return FALSE;
        }

    }

    //extern void MApi_Demod_HWReset(MS_U8 drv_frontend_index);
    //MApi_Demod_HWReset(u8DemodIndex);

    bRet &= MDrv_DMD_PreInit();
    // copy tuner config
    //mapi_interface::Get_mapi_pcb()->GetIsdbTuner(0)->ExtendCommand(mapi_tuner_datatype::E_TUNER_SUBCMD_GET_IQ_SWAP, E_TUNER_DTV_ISDB_MODE, NULL, &u8IqSwap);
    //mapi_interface::Get_mapi_pcb()->GetIsdbTuner(0)->ExtendCommand(mapi_tuner_datatype::E_TUNER_SUBCMD_GET_DTV_IF_FREQ, E_TUNER_DTV_ISDB_MODE, NULL, &u16DtvIFFreqKHz);
    sDMD_ISDBT_InitData.u16IF_KHZ = TUNER_IFFREQ;
    sDMD_ISDBT_InitData.bIQSwap = ISDBT_IQ_SWAP;
    sDMD_ISDBT_InitData.u16ISDBTFECLockCheckTime = ISDBT_FEC_timeout;
    sDMD_ISDBT_InitData.u16ISDBTIcfoChExistCheckTime = ISDBT_ChExist_timeout;
    sDMD_ISDBT_InitData.u8DMD_ISDBT_InitExt = u8DMD_ISDBT_InitExt;
    sDMD_ISDBT_InitData.u16AgcReferenceValue = ISDBT_IFAGC_REF;

    //TS[begin]
    sDMD_ISDBT_InitData.u5TsConfigByte_DivNum = ISDBT_TS_CLK_DIVNUM;
    sDMD_ISDBT_InitData.u1TsConfigByte_ClockInv = ISDBT_TS_CLK_INV;
    sDMD_ISDBT_InitData.u1TsConfigByte_DataSwap = ISDBT_TS_DATA_SWAP;
    sDMD_ISDBT_InitData.u1TsConfigByte_SerialMode = ISDBT_TS_SERIAL;
    //TS[end]

    //I2C[begin]
    sDMD_ISDBT_InitData.u8I2CSlaveAddr = DEMOD_MSB140X_SLAVE_ID;
    sDMD_ISDBT_InitData.u8I2CSlaveBus = 0;
    sDMD_ISDBT_InitData.bIsExtDemod = TRUE;

    sDMD_ISDBT_InitData.I2C_WriteBytes = MDrv_MSB140X_IIC_Write;
    sDMD_ISDBT_InitData.I2C_ReadBytes = MDrv_MSB140X_IIC_Read;
    //I2C[end]

    bRet &= MDrv_DMD_ISDBT_Initial_Hal_Interface();
    bRet &= MDrv_DMD_ISDBT_Init(&sDMD_ISDBT_InitData, sizeof(sDMD_ISDBT_InitData));
    if (!bRet)
    {
        bInited = FALSE;
        printf("MSB1400 Init error\n");
        MsOS_DeleteMutex(_s32MutexId);
        return FALSE;
    }
    //show 1400 demod fw version
    MS_U16 fw_ver;
    MDrv_DMD_ISDBT_GetFwVer(&fw_ver);
    printf("\nISDBT FW Version:%d\n",fw_ver);

    bInited = TRUE;
    return bRet;
}


DEMOD_INTERFACE_MODE MSB1400_Demod_GetOutoutPath(void)
{
    //Dummy function
    if (MsOS_ObtainMutex(_s32MutexId, ISDBTDMD_MUTEX_TIMEOUT) == FALSE)
    {
        PRINTF(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    MsOS_ReleaseMutex(_s32MutexId);
    return DEMOD_INTERFACE_PARALLEL;
}

//===================================================================================
MS_U8 MSB1400_I2C_ReadByte(MS_U16 u16SlaveAddr, MS_U32 u32Addr, MS_U8 *pu8Data)
{

    MS_BOOL bRet=TRUE;
    MS_U8 u8MsbData[6] = {0};
    
    u8MsbData[0] = 0x10;
    u8MsbData[1] = 0x00;
    u8MsbData[2] = 0x00;
    u8MsbData[3] = (u32Addr >> 8) &0xff;
    u8MsbData[4] = u32Addr &0xff;

    u8MsbData[0] = 0x35;
    bRet &= MDrv_MSB140X_IIC_Write(u16SlaveAddr, 0, 0, 1, u8MsbData);

    u8MsbData[0] = 0x10;
    bRet &= MDrv_MSB140X_IIC_Write(u16SlaveAddr, 0, 0, 5, u8MsbData);
    bRet &= MDrv_MSB140X_IIC_Read(u16SlaveAddr, 0, 0, 1, pu8Data);

    u8MsbData[0] = 0x34;
    bRet &= MDrv_MSB140X_IIC_Write(u16SlaveAddr, 0, 0, 1, u8MsbData);

    return bRet;
}

MS_U8 MSB1400_I2C_WriteByte(MS_U16 u16SlaveAddr, MS_U32 u32Addr, MS_U8 u8Data)
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
    bRet &= MDrv_MSB140X_IIC_Write(u16SlaveAddr, 0, 0, 1, u8MsbData);
    u8MsbData[0] = 0x10;
    bRet &= MDrv_MSB140X_IIC_Write(u16SlaveAddr, 0, 0, 6, u8MsbData);
    u8MsbData[0] = 0x34;
    bRet &= MDrv_MSB140X_IIC_Write(u16SlaveAddr, 0, 0, 1, u8MsbData);

    return bRet;
}


MS_BOOL MSB1400_I2C_Channel_Change(MS_U16 u16SlaveAddr, MS_U8 ch_num)
{
     MS_BOOL bRet=TRUE;
     MS_U8 Data[5] = {0x53, 0x45, 0x52, 0x44, 0x42};

     Data[0] = (ch_num & 0x01)? 0x81 : 0x80;
     bRet&= MDrv_MSB140X_IIC_Write(u16SlaveAddr, 0, 0, 1, Data);
     Data[0] = (ch_num & 0x02)? 0x83 : 0x82;
     bRet&= MDrv_MSB140X_IIC_Write(u16SlaveAddr, 0, 0, 1, Data);
     Data[0] = (ch_num & 0x04)? 0x85 : 0x84;
     bRet&= MDrv_MSB140X_IIC_Write(u16SlaveAddr, 0, 0, 1, Data);

     return bRet;
}

MS_BOOL MSB1400_I2C_Channel_Set(MS_U16 u16SlaveAddr, MS_U8 ch_num)
{
    MS_BOOL bRet=TRUE;
    MS_U8 Data[5] = {0x53, 0x45, 0x52, 0x44, 0x42};
    //Exit
    Data[0] = 0x34;
    bRet &= MDrv_MSB140X_IIC_Write(u16SlaveAddr, 0, NULL, 1, Data); //MDrv_msb131x_IIC_Write(DEMOD_MSB131X_SLAVE_ID, 0, 0, Data, 1);
    Data[0]=(ch_num & 0x01)? 0x36 : 0x45;
    bRet &= MDrv_MSB140X_IIC_Write(u16SlaveAddr, 0, NULL, 1, Data); //MDrv_msb131x_IIC_Write(DEMOD_MSB131X_SLAVE_ID, 0, 0, Data, 1);
    //Init
    Data[0] = 0x53;
    bRet &= MDrv_MSB140X_IIC_Write(u16SlaveAddr, 0, NULL, 5, Data); //MDrv_msb131x_IIC_Write(DEMOD_MSB131X_SLAVE_ID, 0, 0, Data, 5);
    Data[0]=(ch_num & 0x04)? 0x80 : 0x81;
    bRet &= MDrv_MSB140X_IIC_Write(u16SlaveAddr, 0, NULL, 1, Data); //MDrv_msb131x_IIC_Write(DEMOD_MSB131X_SLAVE_ID, 0, 0, Data, 1);
    if ((ch_num==4)||(ch_num==5)||(ch_num==1))
        Data[0]=0x82;
    else
        Data[0] = 0x83;
     bRet &= MDrv_MSB140X_IIC_Write(u16SlaveAddr, 0, NULL, 1, Data); //MDrv_msb131x_IIC_Write(DEMOD_MSB131X_SLAVE_ID, 0, 0, Data, 1);

    if ((ch_num==4)||(ch_num==5))
        Data[0]=0x85;
    else
        Data[0] = 0x84;

     bRet &= MDrv_MSB140X_IIC_Write(u16SlaveAddr, 0, NULL, 1, Data); //MDrv_msb131x_IIC_Write(DEMOD_MSB131X_SLAVE_ID, 0, 0, Data, 1);
     Data[0]=(ch_num & 0x01)? 0x51 : 0x53;
     bRet &= MDrv_MSB140X_IIC_Write(u16SlaveAddr, 0, NULL, 1, Data); //MDrv_msb131x_IIC_Write(DEMOD_MSB131X_SLAVE_ID, 0, 0, Data, 1);
     Data[0]=(ch_num & 0x01)? 0x37 : 0x7F;
     bRet &= MDrv_MSB140X_IIC_Write(u16SlaveAddr, 0, NULL, 1, Data); //MDrv_msb131x_IIC_Write(DEMOD_MSB131X_SLAVE_ID, 0, 0, Data, 1);
     Data[0] = 0x35;
     bRet &= MDrv_MSB140X_IIC_Write(u16SlaveAddr, 0, NULL, 1, Data); //MDrv_msb131x_IIC_Write(DEMOD_MSB131X_SLAVE_ID, 0, 0, Data, 1);
     Data[0] = 0x71;
     bRet &= MDrv_MSB140X_IIC_Write(u16SlaveAddr, 0, NULL, 1, Data); //MDrv_msb131x_IIC_Write(DEMOD_MSB131X_SLAVE_ID, 0, 0, Data, 1);

     return bRet;

}

#define MSB1400_CHIP_ID 0x45
MS_BOOL MSB1400_Check_Exist(MS_U8 u8DemodIndex)
{
    MS_U8 u8_tmp = 0;
    
    if(!MSB1400_I2C_Channel_Set(DEMOD_MSB140X_SLAVE_ID,0))
    {
        printf("[MSB1400] I2C_CH_set fail \n");
        //sreturn FALSE;
    }

    if(!MSB1400_I2C_Channel_Change(DEMOD_MSB140X_SLAVE_ID,3))
    {
        printf("[MSB1400] I2C_CH_Reset fail \n");
        //sreturn FALSE;
    }

    if (!MSB1400_I2C_ReadByte(DEMOD_MSB140X_SLAVE_ID,0x0900, &u8_tmp))
    {
        printf("[MSB1400] Read  Chip ID fail \n");
    }
    
    printf("[MSB1400] read id :%x \n",u8_tmp );
    
    return (u8_tmp == MSB1400_CHIP_ID);
}


MS_BOOL MSB1400_Extension_Function(MS_U8 u8DemodIndex, DEMOD_EXT_FUNCTION_TYPE fuction_type, void *data)
{
    MS_BOOL bret = TRUE;
    switch(fuction_type)
    {
        case DEMOD_EXT_FUNC_OPEN:
            bret &= MSB1400_Demod_Open();
            break;
        case DEMOD_EXT_FUNC_CLOSE:
            bret &= MSB1400_Demod_Close();
            break;
        default:
            printf("Request extension function (%x) does not exist\n",fuction_type);
    }
    return bret;
}


DRV_DEMOD_TABLE_TYPE GET_DEMOD_ENTRY_NODE(DEMOD_MSB1400) DDI_DRV_TABLE_ENTRY(demodtab) =
{
     .name                         = "DEMOD_MSB1400",
     .data                         = DEMOD_MSB1400,
     .init                         = MSB1400_Demod_Init,
     .GetLock                      = MSB1400_Demod_GetLock,
     .GetSNR                       = MSB1400_Demod_GetSNR,
     .GetBER                       = MSB1400_Demod_GetBER,
     .GetPWR                       = MSB1400_Demod_GetPWR,
     .GetQuality                   = MSB1400_Demod_GetSignalQuality,
     .GetParam                     = MDrv_Demod_null_GetParam,
     .Restart                      = MSB1400_Demod_Restart,
     .I2CByPass                    = MSB1400_Demod_I2C_ByPass,
     .I2CByPassPreSetting          = NULL,
     .CheckExist                   = MSB1400_Check_Exist,
     .Extension_Function           = MSB1400_Extension_Function,
     .Extension_FunctionPreSetting = NULL,
#if MS_DVBT2_INUSE
     .SetCurrentDemodType          = MDrv_Demod_null_SetCurrentDemodType,
     .GetCurrentDemodType          = MDrv_Demod_null_GetCurrentDemodType,
     .GetPlpBitMap                 = MDrv_Demod_null_GetPlpBitMap,
     .GetPlpGroupID                = MDrv_Demod_null_GetPlpGroupID,
     .SetPlpGroupID                = MDrv_Demod_null_SetPlpGroupID,

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

#endif

