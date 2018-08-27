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
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#else
#include <string.h>
#include <math.h>
#endif
#include "MsCommon.h"
#include "drvIIC.h"
#include "HbCommon.h"

#include "MsOS.h"
#include "Board.h"
#include "drvTuner.h"
#include "drvDemod.h"
#include "drvDemodNull.h"
#include "drvGPIO.h"

#if IF_THIS_DEMOD_INUSE(DEMOD_MSB1400)
#include "drvDMD_common.h"
#include "drvDMD_ISDBT.h"       //dan add for integrate ISDBT utopia driver
#include "drvDMD_VD_MBX.h"
#if defined(SUPPORT_MSPI_LOAD_CODE) && (SUPPORT_MSPI_LOAD_CODE == 1)
#include "drvDMD_common.h"
#include "drvMSPI.h"
#include "drvSYS.h"
#ifndef USE_SPI_LOAD_TO_SDRAM
#define USE_SPI_LOAD_TO_SDRAM
#endif
#endif

#define MSB140x_Delayms  MsOS_DelayTask
#define DEMOD_MSB140X_SLAVE_ID  0xD2
#define ISDBTDMD_MUTEX_TIMEOUT       (2000)


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

static MS_BOOL bInited = FALSE;
static MS_S32 _s32MutexId = -1;
static DEMOD_ISDBT_Layer eISDBT_Layer = DEMOD_ISDBT_Layer_A;
static DEMOD_MS_INIT_PARAM MSB1400_INIT_PARAM[MAX_FRONTEND_NUM];

MS_BOOL MSB1400_I2C_Channel_Set(MS_U8 u8DemodIndex, MS_U16 u16SlaveAddr, MS_U8 ch_num);
MS_BOOL MSB1400_I2C_Channel_Change(MS_U8 u8DemodIndex, MS_U16 u16SlaveAddr, MS_U8 ch_num);
MS_U8 MSB1400_I2C_WriteByte(MS_U8 u8DemodIndex, MS_U16 u16SlaveAddr, MS_U32 u32Addr, MS_U8 u8Data);
MS_BOOL MSB1400_Demod_I2C_ByPass(MS_U8 u8DemodIndex,MS_BOOL bOn);

typedef MS_BOOL (*fpI2C_Write)(MS_U16 u16BusNumSlaveID, MS_U8 u8addrcount, MS_U8* pu8addr, MS_U16 u16size, MS_U8* pu8data);
typedef MS_BOOL (*fpI2C_Read)(MS_U16 u16BusNumSlaveID, MS_U8 u8AddrNum, MS_U8* paddr, MS_U16 u16size, MS_U8* pu8data);

#ifdef USE_SPI_LOAD_TO_SDRAM
static MS_U32 u32DmxInputPath = MSPI_PATH_NONE;
static void msb1400_SPIPAD_TS0_En(MS_BOOL bOnOff);
static void msb1400_SPIPAD_TS1_En(MS_BOOL bOnOff);
static void msb1400_SPIPAD_TS2_En(MS_BOOL bOnOff);
static fpSPIPAD_En SPIPAD_EN[3]= {msb1400_SPIPAD_TS0_En,\
                                  msb1400_SPIPAD_TS1_En,\
                                  msb1400_SPIPAD_TS2_En};

static void msb1400_SPIPAD_TS0_En(MS_BOOL bOnOff)
{
    //printf("msb1400_SPIPAD_TS0_En, bOnOff = %x\n", bOnOff);
    if(bOnOff)
    {
        MDrv_SYS_SetPadMux(E_TS0_PAD_SET, E_MSPI_PAD_ON);
    }
    else
    {
       if(ISDBT_TS_SERIAL)
           MDrv_SYS_SetPadMux(E_TS0_PAD_SET, E_SERIAL_IN);
       else
           MDrv_SYS_SetPadMux(E_TS0_PAD_SET, E_PARALLEL_IN);
    }

}

static void msb1400_SPIPAD_TS1_En(MS_BOOL bOnOff)
{
    if(bOnOff)
        MDrv_SYS_SetPadMux(E_TS1_PAD_SET, E_MSPI_PAD_ON);
    else
    {
       if(ISDBT_TS_SERIAL)
          MDrv_SYS_SetPadMux(E_TS1_PAD_SET, E_SERIAL_IN);
       else
          MDrv_SYS_SetPadMux(E_TS1_PAD_SET, E_PARALLEL_IN);
    }
}

static void msb1400_SPIPAD_TS2_En(MS_BOOL bOnOff)
{
    if(bOnOff)
        MDrv_SYS_SetPadMux(E_TS2_PAD_SET, E_MSPI_PAD_ON);
    else
    {
       if(ISDBT_TS_SERIAL)
          MDrv_SYS_SetPadMux(E_TS2_PAD_SET, E_SERIAL_IN);
       else
          MDrv_SYS_SetPadMux(E_TS2_PAD_SET, E_PARALLEL_IN);
    }
}

#endif

//##########################################################################################################
//       IIC interface
//##########################################################################################################

MS_BOOL   MDrv_MSB140X_IIC_Write(MS_U16 u16BusNumSlaveID, MS_U8 u8addrcount, MS_U8* pu8addr, MS_U16 u16size, MS_U8* pu8data)
{
    if (FALSE == MDrv_IIC_Write(u16BusNumSlaveID, pu8addr, u8addrcount, pu8data,  u16size))
    {
        DMD_ERR(("MDrv_MSB140X_IIC_Write error\n"));
    }

    return TRUE;
}

MS_BOOL   MDrv_MSB140X_IIC_Read(MS_U16 u16BusNumSlaveID, MS_U8 u8AddrNum, MS_U8* paddr, MS_U16 u16size, MS_U8* pu8data)
{
    if (FALSE == MDrv_IIC_Read(u16BusNumSlaveID, paddr, u8AddrNum, pu8data,  u16size))
    {
        DMD_ERR(("MDrv_MSB140X_IIC_Read error\n"));
    }

    return TRUE;
}

MS_BOOL   MDrv_MSB140X_IIC1_Write(MS_U16 u16BusNumSlaveID, MS_U8 u8addrcount, MS_U8* pu8addr, MS_U16 u16size, MS_U8* pu8data)
{
    if (FALSE == MDrv_IIC1_Write(u16BusNumSlaveID, pu8addr, u8addrcount, pu8data,  u16size))
    {
        DMD_ERR(("MDrv_MSB140X_IIC_Write error\n"));
    }

    return TRUE;
}

MS_BOOL   MDrv_MSB140X_IIC1_Read(MS_U16 u16BusNumSlaveID, MS_U8 u8AddrNum, MS_U8* paddr, MS_U16 u16size, MS_U8* pu8data)
{
    if (FALSE == MDrv_IIC1_Read(u16BusNumSlaveID, paddr, u8AddrNum, pu8data,  u16size))
    {
        DMD_ERR(("MDrv_MSB140X_IIC_Read error\n"));
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
    //MS_BOOL bRet = TRUE;
    MS_U8 u8Data;

    MDrv_DMD_ISDBT_GetReg(u16Addr, &u8Data);

    return u8Data;
}

MS_BOOL MSB1400_Demod_Open(void)
{
    if (MsOS_ObtainMutex(_s32MutexId, ISDBTDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    if (FALSE == bInited)
    {
        DMD_ERR(("[%s]MSB140X have not inited !!!\n", __FUNCTION__));
        MsOS_ReleaseMutex(_s32MutexId);
        return FALSE;
    }

    MDrv_DMD_PreInit();
    MsOS_ReleaseMutex(_s32MutexId);
    return TRUE;
}

MS_BOOL MSB1400_Demod_PowerOnOff(MS_BOOL bPowerOn)
{
    if (bPowerOn)
    {
        mdrv_gpio_set_high(GPIO_FE_RST);
    }
    else
    {
        mdrv_gpio_set_low(GPIO_FE_RST);
    }

    return TRUE;
}

MS_BOOL MSB1400_Demod_Close(void)
{
    if (MsOS_ObtainMutex(_s32MutexId, ISDBTDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    if (FALSE == bInited)
    {
        DMD_ERR(("[%s]MSB140X have not inited !!!\n", __FUNCTION__));
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
        DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    if (FALSE == bInited)
    {
        DMD_ERR(("[%s]MSB140X have not inited !!!\n", __FUNCTION__));
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
        DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    if (FALSE == bInited)
    {
        DMD_ERR(("[%s]MSB140X have not inited !!!\n", __FUNCTION__));
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
        DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    if (FALSE == bInited)
    {
        DMD_ERR(("[%s]MSB140X have not inited !!!\n", __FUNCTION__));
        MsOS_ReleaseMutex(_s32MutexId);
        return FALSE;
    }
    if (DMD_ISDBT_LOCK == MDrv_DMD_ISDBT_GetLock(DMD_ISDBT_GETLOCK))
    {
        bRet = MDrv_DMD_ISDBT_GetPostViterbiBer((EN_ISDBT_Layer)eISDBT_Layer, pfBER);
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
    MS_BOOL bret = TRUE;
    if (MsOS_ObtainMutex(_s32MutexId, ISDBTDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    if (FALSE == bInited)
    {
        DMD_ERR(("[%s]MSB140X have not inited !!!\n", __FUNCTION__));
        MsOS_ReleaseMutex(_s32MutexId);
        return FALSE;
    }

    if (DMD_ISDBT_LOCK == MDrv_DMD_ISDBT_GetLock(DMD_ISDBT_GETLOCK))
    {
        MSB1400_Demod_I2C_ByPass(u8DemodIndex, TRUE);
        bret = MSB1400_INIT_PARAM[u8DemodIndex].pstTunertab->Extension_Function(u8DemodIndex, TUNER_EXT_FUNC_GET_POWER_LEVEL, ps32Signal);
        MSB1400_Demod_I2C_ByPass(u8DemodIndex, FALSE);
    }

    MsOS_ReleaseMutex(_s32MutexId);
    return bret;
}

MS_BOOL MSB1400_Demod_GetSSI(MS_U8 u8DemodIndex,MS_U16 *pu16SSI)
{
    MS_U16  u16SignalStrengthValue;

    if (MsOS_ObtainMutex(_s32MutexId, ISDBTDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    if (FALSE == bInited)
    {
        DMD_ERR(("[%s]MSB140X have not inited !!!\n", __FUNCTION__));
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
    *pu16SSI = u16SignalStrengthValue;

    MsOS_ReleaseMutex(_s32MutexId);
    return TRUE;
}


MS_BOOL MSB1400_Demod_GetLock(MS_U8 u8DemodIndex,EN_LOCK_STATUS *peLockStatus)
{
    DMD_ISDBT_LOCK_STATUS eLockStatus;

    *peLockStatus = E_DEMOD_CHECKING;
    if (MsOS_ObtainMutex(_s32MutexId, ISDBTDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    if (FALSE == bInited)
    {
        DMD_ERR(("[%s]MSB140X have not inited !!!\n", __FUNCTION__));
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
        DMD_ERR(("ISDBTT_GetLockStatus error\n"));
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
        DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    if (FALSE == bInited)
    {
        DMD_ERR(("[%s]MSB140X have not inited !!!\n", __FUNCTION__));
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
        bret &= MSB1400_I2C_Channel_Set(u8DemodIndex, DEMOD_MSB140X_SLAVE_ID,0);
        bret &= MSB1400_I2C_Channel_Change(u8DemodIndex, DEMOD_MSB140X_SLAVE_ID,3);
        if(bOn)
            bret &= MSB1400_I2C_WriteByte(u8DemodIndex, DEMOD_MSB140X_SLAVE_ID,0x0910, 0x10);
        else
            bret &= MSB1400_I2C_WriteByte(u8DemodIndex, DEMOD_MSB140X_SLAVE_ID,0x0910, 0x00);
        return bret;
    }

    if (MsOS_ObtainMutex(_s32MutexId, ISDBTDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    bret &= MDrv_DMD_ISDBT_IIC_BYPASS_MODE(bOn);
    MsOS_ReleaseMutex(_s32MutexId);
    return bret;
}

MS_BOOL MSB1400_Demod_Init(MS_U8 u8DemodIndex,DEMOD_MS_INIT_PARAM* pParam)
{
    MS_BOOL bRet = TRUE;
    MS_U32 u32TunerIF = 0;
    DMD_ISDBT_InitData sDMD_ISDBT_InitData;
    static MS_U8 u8DMD_ISDBT_InitExt[] = {1}; // RFAGC tristate control default value, 1:trisate 0:non-tristate,never modify unless you know the meaning
    MS_IIC_PORT ePort = getI2CPort(u8DemodIndex);

    DMD_DBG(("\nMSB1400_Demod_Init\n"));
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

    /* get IF */
    if(TRUE == pParam->pstTunertab->GetTunerIF(u8DemodIndex, &u32TunerIF))
        sDMD_ISDBT_InitData.u16IF_KHZ = (MS_U16)u32TunerIF;
    else
        sDMD_ISDBT_InitData.u16IF_KHZ = 5000;

    MSB1400_INIT_PARAM[u8DemodIndex].pstTunertab = pParam->pstTunertab;

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

    if(ePort < E_MS_IIC_PORT_1)
    {
        sDMD_ISDBT_InitData.I2C_WriteBytes = MDrv_MSB140X_IIC_Write;
        sDMD_ISDBT_InitData.I2C_ReadBytes = MDrv_MSB140X_IIC_Read;
    }
    else if(ePort < E_MS_IIC_PORT_2)
    {
        sDMD_ISDBT_InitData.I2C_WriteBytes = MDrv_MSB140X_IIC1_Write;
        sDMD_ISDBT_InitData.I2C_ReadBytes = MDrv_MSB140X_IIC1_Read;
    }
    else
    {
        bInited = FALSE;
        DMD_ERR(("MSB1400 Init I2C select error\n"));
        MsOS_DeleteMutex(_s32MutexId);
        return FALSE;
    }
    //I2C[end]

#ifdef USE_SPI_LOAD_TO_SDRAM
    if((u32DmxInputPath != MSPI_PATH_NONE) && (u32DmxInputPath < 3))
    {
       SPIPAD_EN[u32DmxInputPath](TRUE);
       sDMD_ISDBT_InitData.bIsUseSspiLoadCode = TRUE;
       sDMD_ISDBT_InitData.bIsSspiUseTsPin = TRUE;
    }
    else
#else
    {
       sDMD_ISDBT_InitData.bIsUseSspiLoadCode = FALSE;
       sDMD_ISDBT_InitData.bIsSspiUseTsPin = FALSE;
    }
#endif
    sDMD_ISDBT_InitData.bIsExtDemod = 1;

    bRet &= MDrv_DMD_ISDBT_Initial_Hal_Interface();
    bRet &= MDrv_DMD_ISDBT_Init(&sDMD_ISDBT_InitData, sizeof(sDMD_ISDBT_InitData));

#ifdef USE_SPI_LOAD_TO_SDRAM
    if((u32DmxInputPath != MSPI_PATH_NONE) && (u32DmxInputPath < 3))
    {
       SPIPAD_EN[u32DmxInputPath](FALSE);
    }
#endif

    if (!bRet)
    {
        bInited = FALSE;
        DMD_ERR(("MSB1400 Init error\n"));
        MsOS_DeleteMutex(_s32MutexId);
        return FALSE;
    }
    //show 1400 demod fw version
    MS_U16 fw_ver;
    MDrv_DMD_ISDBT_GetFwVer(&fw_ver);
    DMD_DBG(("\nISDBT FW Version:%d\n",fw_ver));

    bInited = TRUE;
    return bRet;
}


DEMOD_INTERFACE_MODE MSB1400_Demod_GetOutoutPath(void)
{
    //Dummy function
    if (MsOS_ObtainMutex(_s32MutexId, ISDBTDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    MsOS_ReleaseMutex(_s32MutexId);
    return DEMOD_INTERFACE_PARALLEL;
}

//===================================================================================
MS_U8 MSB1400_I2C_ReadByte(MS_U8 u8DemodIndex, MS_U16 u16SlaveAddr, MS_U32 u32Addr, MS_U8 *pu8Data)
{

    MS_BOOL bRet=TRUE;
    MS_U8 u8MsbData[6] = {0};
    fpI2C_Read fpRead;
    fpI2C_Write fpWrite;
    MS_IIC_PORT ePort = getI2CPort(u8DemodIndex);

    if(ePort < E_MS_IIC_PORT_1)
    {
        fpWrite = MDrv_MSB140X_IIC_Write;
        fpRead = MDrv_MSB140X_IIC_Read;
    }
    else if(ePort < E_MS_IIC_PORT_2)
    {
        fpWrite = MDrv_MSB140X_IIC1_Write;
        fpRead = MDrv_MSB140X_IIC1_Read;
    }
    else
    {
        return FALSE;
    }
    u8MsbData[0] = 0x10;
    u8MsbData[1] = 0x00;
    u8MsbData[2] = 0x00;
    u8MsbData[3] = (u32Addr >> 8) &0xff;
    u8MsbData[4] = u32Addr &0xff;

    u8MsbData[0] = 0x35;
    bRet &= (*fpWrite)(u16SlaveAddr, 0, 0, 1, u8MsbData);

    u8MsbData[0] = 0x10;
    bRet &= (*fpWrite)(u16SlaveAddr, 0, 0, 5, u8MsbData);
    bRet &= (*fpRead)(u16SlaveAddr, 0, 0, 1, pu8Data);

    u8MsbData[0] = 0x34;
    bRet &= (*fpWrite)(u16SlaveAddr, 0, 0, 1, u8MsbData);

    return bRet;
}

MS_U8 MSB1400_I2C_WriteByte(MS_U8 u8DemodIndex, MS_U16 u16SlaveAddr, MS_U32 u32Addr, MS_U8 u8Data)
{
    MS_BOOL bRet=TRUE;
    MS_U8 u8MsbData[6] = {0};
    fpI2C_Write fpWrite;
    MS_IIC_PORT ePort = getI2CPort(u8DemodIndex);

    if(ePort < E_MS_IIC_PORT_1)
    {
        fpWrite = MDrv_MSB140X_IIC_Write;
    }
    else if(ePort < E_MS_IIC_PORT_2)
    {
        fpWrite = MDrv_MSB140X_IIC1_Write;
    }
    else
    {
        return FALSE;
    }

    u8MsbData[0] = 0x10;
    u8MsbData[1] = 0x00;
    u8MsbData[2] = 0x00;
    u8MsbData[3] = (u32Addr >> 8) &0xff;
    u8MsbData[4] = u32Addr &0xff;
    u8MsbData[5] = u8Data;

    u8MsbData[0] = 0x35;
    bRet &= (*fpWrite)(u16SlaveAddr, 0, 0, 1, u8MsbData);
    u8MsbData[0] = 0x10;
    bRet &= (*fpWrite)(u16SlaveAddr, 0, 0, 6, u8MsbData);
    u8MsbData[0] = 0x34;
    bRet &= (*fpWrite)(u16SlaveAddr, 0, 0, 1, u8MsbData);

    return bRet;
}


MS_BOOL MSB1400_I2C_Channel_Change(MS_U8 u8DemodIndex, MS_U16 u16SlaveAddr, MS_U8 ch_num)
{
    MS_BOOL bRet=TRUE;
    MS_U8 Data[5] = {0x53, 0x45, 0x52, 0x44, 0x42};
    fpI2C_Write fpWrite;
    MS_IIC_PORT ePort = getI2CPort(u8DemodIndex);

    if(ePort < E_MS_IIC_PORT_1)
    {
        fpWrite = MDrv_MSB140X_IIC_Write;
    }
    else if(ePort < E_MS_IIC_PORT_2)
    {
        fpWrite = MDrv_MSB140X_IIC1_Write;
    }
    else
    {
        return FALSE;
    }

    Data[0] = (ch_num & 0x01)? 0x81 : 0x80;
    bRet&= (*fpWrite)(u16SlaveAddr, 0, 0, 1, Data);
    Data[0] = (ch_num & 0x02)? 0x83 : 0x82;
    bRet&= (*fpWrite)(u16SlaveAddr, 0, 0, 1, Data);
    Data[0] = (ch_num & 0x04)? 0x85 : 0x84;
    bRet&= (*fpWrite)(u16SlaveAddr, 0, 0, 1, Data);

    return bRet;
}

MS_BOOL MSB1400_I2C_Channel_Set(MS_U8 u8DemodIndex, MS_U16 u16SlaveAddr, MS_U8 ch_num)
{
    MS_BOOL bRet=TRUE;
    MS_U8 Data[5] = {0x53, 0x45, 0x52, 0x44, 0x42};
    fpI2C_Write fpWrite;
    MS_IIC_PORT ePort = getI2CPort(u8DemodIndex);

    if(ePort < E_MS_IIC_PORT_1)
    {
        fpWrite = MDrv_MSB140X_IIC_Write;
    }
    else if(ePort < E_MS_IIC_PORT_2)
    {
        fpWrite = MDrv_MSB140X_IIC1_Write;
    }
    else
    {
        return FALSE;
    }

    //Exit
    Data[0] = 0x34;
    bRet &= (*fpWrite)(u16SlaveAddr, 0, NULL, 1, Data); //MDrv_msb131x_IIC_Write(DEMOD_MSB131X_SLAVE_ID, 0, 0, Data, 1);
    Data[0]=(ch_num & 0x01)? 0x36 : 0x45;
    bRet &= (*fpWrite)(u16SlaveAddr, 0, NULL, 1, Data); //MDrv_msb131x_IIC_Write(DEMOD_MSB131X_SLAVE_ID, 0, 0, Data, 1);
    //Init
    Data[0] = 0x53;
    bRet &= (*fpWrite)(u16SlaveAddr, 0, NULL, 5, Data); //MDrv_msb131x_IIC_Write(DEMOD_MSB131X_SLAVE_ID, 0, 0, Data, 5);
    Data[0]=(ch_num & 0x04)? 0x80 : 0x81;
    bRet &= (*fpWrite)(u16SlaveAddr, 0, NULL, 1, Data); //MDrv_msb131x_IIC_Write(DEMOD_MSB131X_SLAVE_ID, 0, 0, Data, 1);
    if ((ch_num==4)||(ch_num==5)||(ch_num==1))
        Data[0]=0x82;
    else
        Data[0] = 0x83;
    bRet &= (*fpWrite)(u16SlaveAddr, 0, NULL, 1, Data); //MDrv_msb131x_IIC_Write(DEMOD_MSB131X_SLAVE_ID, 0, 0, Data, 1);

    if ((ch_num==4)||(ch_num==5))
        Data[0]=0x85;
    else
        Data[0] = 0x84;

    bRet &= (*fpWrite)(u16SlaveAddr, 0, NULL, 1, Data); //MDrv_msb131x_IIC_Write(DEMOD_MSB131X_SLAVE_ID, 0, 0, Data, 1);
    Data[0]=(ch_num & 0x01)? 0x51 : 0x53;
    bRet &= (*fpWrite)(u16SlaveAddr, 0, NULL, 1, Data); //MDrv_msb131x_IIC_Write(DEMOD_MSB131X_SLAVE_ID, 0, 0, Data, 1);
    Data[0]=(ch_num & 0x01)? 0x37 : 0x7F;
    bRet &= (*fpWrite)(u16SlaveAddr, 0, NULL, 1, Data); //MDrv_msb131x_IIC_Write(DEMOD_MSB131X_SLAVE_ID, 0, 0, Data, 1);
    Data[0] = 0x35;
    bRet &= (*fpWrite)(u16SlaveAddr, 0, NULL, 1, Data); //MDrv_msb131x_IIC_Write(DEMOD_MSB131X_SLAVE_ID, 0, 0, Data, 1);
    Data[0] = 0x71;
    bRet &= (*fpWrite)(u16SlaveAddr, 0, NULL, 1, Data); //MDrv_msb131x_IIC_Write(DEMOD_MSB131X_SLAVE_ID, 0, 0, Data, 1);

    return bRet;

}

#define MSB1400_CHIP_ID 0x45
MS_BOOL MSB1400_Check_Exist(MS_U8 u8DemodIndex, MS_U8* pu8SlaveID)
{
    MS_U8 u8_tmp = 0;

    if(!MSB1400_I2C_Channel_Set(u8DemodIndex, DEMOD_MSB140X_SLAVE_ID,0))
    {
        DMD_ERR(("[MSB1400] I2C_CH_set fail \n"));
        //sreturn FALSE;
    }

    if(!MSB1400_I2C_Channel_Change(u8DemodIndex, DEMOD_MSB140X_SLAVE_ID,3))
    {
        DMD_ERR(("[MSB1400] I2C_CH_Reset fail \n"));
        //sreturn FALSE;
    }

    if (!MSB1400_I2C_ReadByte(u8DemodIndex, DEMOD_MSB140X_SLAVE_ID,0x0900, &u8_tmp))
    {
        DMD_ERR(("[MSB1400] Read  Chip ID fail \n"));
    }

    DMD_ERR(("[MSB1400] read id :%x \n",u8_tmp ));
    if ( u8_tmp == MSB1400_CHIP_ID )
    {
        *pu8SlaveID = DEMOD_MSB140X_SLAVE_ID;

        #ifdef USE_SPI_LOAD_TO_SDRAM
        #if defined(MSPI_PATH_DETECT) && (MSPI_PATH_DETECT == 1)
        MS_U8 i;
        //check TS path
        MDrv_DMD_SSPI_Init(0);
        MDrv_MasterSPI_CsPadConfig(0, 0xff);
        MDrv_MasterSPI_MaxClkConfig(0, 15);

        MSB1400_I2C_WriteByte(u8DemodIndex, DEMOD_MSB140X_SLAVE_ID, 0x0900+(0x28)*2, 0x00);
        MSB1400_I2C_WriteByte(u8DemodIndex, DEMOD_MSB140X_SLAVE_ID, 0x0900+(0x28)*2 + 1, 0x00);
        MSB1400_I2C_WriteByte(u8DemodIndex, DEMOD_MSB140X_SLAVE_ID, 0x0900+(0x2d)*2, 0xff);
        MSB1400_I2C_WriteByte(u8DemodIndex, DEMOD_MSB140X_SLAVE_ID, 0x0900+(0x2d)*2 + 1, 0x00);

        // ------enable to use TS_PAD as SSPI_PAD
        // [0:0] reg_en_sspi_pad
        // [1:1] reg_ts_sspi_en, 1: use TS_PAD as SSPI_PAD
        MSB1400_I2C_WriteByte(u8DemodIndex, DEMOD_MSB140X_SLAVE_ID, 0x0900+(0x3b)*2, 0x02);
        MSB1400_I2C_WriteByte(u8DemodIndex, DEMOD_MSB140X_SLAVE_ID, 0x0900+(0x3b)*2 + 1, 0x00);

        // ------- MSPI protocol setting
        // [8] cpha
        // [9] cpol
        MSB1400_I2C_ReadByte(u8DemodIndex, DEMOD_MSB140X_SLAVE_ID,0x0900+(0x3a)*2+1, &u8_tmp);
        u8_tmp &= 0xFC;
        MSB1400_I2C_WriteByte(u8DemodIndex, DEMOD_MSB140X_SLAVE_ID, 0x0900+(0x3a)*2 + 1, u8_tmp);

        // ------- MSPI driving setting
        MSB1400_I2C_WriteByte(u8DemodIndex, DEMOD_MSB140X_SLAVE_ID, 0x0900+(0x2c)*2, 0xff);
        MSB1400_I2C_WriteByte(u8DemodIndex, DEMOD_MSB140X_SLAVE_ID, 0x0900+(0x2c)*2 + 1, 0x07);


        for(i=0; i<MSPI_PATH_MAX; i++)
        {
            SPIPAD_EN[i](TRUE);

            MDrv_DMD_SSPI_RIU_Read8(0x900, &u8_tmp);
            MDrv_DMD_SSPI_RIU_Read8(0x900, &u8_tmp);

            if(u8_tmp == MSB1400_CHIP_ID)
            {
                u32DmxInputPath = (MS_U32)i;
                SPIPAD_EN[i](FALSE);
                DMD_DBG(("Get MSB1400 chip ID by MSPI on TS%d\n", (int)u32DmxInputPath));
                break;
            }
            else
            {
                DMD_DBG(("Cannot get MSB1400 chip ID by MSPI on TS%x\n", i));
                if( i == (MSPI_PATH_MAX - 1))
                    u32DmxInputPath = MSPI_PATH_NONE;
            }

            SPIPAD_EN[i](FALSE);
        }

        // ------disable to use TS_PAD as SSPI_PAD after load code
        // [0:0] reg_en_sspi_pad
        // [1:1] reg_ts_sspi_en, 1: use TS_PAD as SSPI_PAD
        MSB1400_I2C_WriteByte(u8DemodIndex, DEMOD_MSB140X_SLAVE_ID, 0x0900+(0x3b)*2, 0x01);
        MSB1400_I2C_WriteByte(u8DemodIndex, DEMOD_MSB140X_SLAVE_ID, 0x0900+(0x3b)*2 + 1, 0x00);
        #endif //MSPI_PATH_DETECT
        #endif
    }

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
    case DEMOD_EXT_FUNC_FINALIZE:
        if (bInited)
        {
            bret &=MDrv_DMD_ISDBT_MD_SetPowerState(0,E_POWER_SUSPEND);
            bret &= MSB1400_Demod_Close();
        }
        bInited = FALSE;
        break;
    case DEMOD_EXT_FUNC_POWER_ON_OFF:
        bret &= MSB1400_Demod_PowerOnOff(*(MS_U8 *)data);
        break;
    case DEMOD_EXT_FUNC_SET_ISDBT_LAYER:
        eISDBT_Layer = *(DEMOD_ISDBT_Layer*)data;
        break;
    default:
        DMD_DBG(("Request extension function (%x) does not exist\n",fuction_type));
    }
    return bret;
}


DRV_DEMOD_TABLE_TYPE GET_DEMOD_ENTRY_NODE(DEMOD_MSB1400) DDI_DRV_TABLE_ENTRY(demodtab) =
{
    .name                         = "DEMOD_MSB1400",
    .data                         = DEMOD_MSB1400,
    .SupportINT                   = FALSE,
    .init                         = MSB1400_Demod_Init,
    .GetLock                      = MSB1400_Demod_GetLock,
    .GetSNR                       = MSB1400_Demod_GetSNR,
    .GetBER                       = MSB1400_Demod_GetBER,
    .GetPWR                       = MSB1400_Demod_GetPWR,
    .GetSSI                       = MSB1400_Demod_GetSSI,
    .GetQuality                   = MSB1400_Demod_GetSignalQuality,
    .GetParam                     = MDrv_Demod_null_GetParam,
    .Restart                      = MSB1400_Demod_Restart,
    .I2CByPass                    = MSB1400_Demod_I2C_ByPass,
    .I2CByPassPreSetting          = NULL,
    .CheckExist                   = MSB1400_Check_Exist,
    .Extension_Function           = MSB1400_Extension_Function,
    .Extension_FunctionPreSetting = NULL,
    .Get_Packet_Error              = MDrv_Demod_null_Get_Packet_Error,
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
    .DiSEqC_SendCmd               = MDrv_Demod_null_DiSEqC_SendCmd,
    .DiSEqC_GetReply              = MDrv_Demod_null_DiSEqC_GetReply,
    .GetISIDInfo                  = MDrv_Demod_null_GetVCM_ISID_INFO,
    .SetISID                      = MDrv_Demod_null_SetVCM_ISID
#endif
};

#endif

