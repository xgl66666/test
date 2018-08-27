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

#include "device_demodulator_msb3400.h"

#if IF_THIS_DEMOD_INUSE(DEMOD_MSB3400)
#include "drvDMD_common.h"
#include "drvDMD_ISDBT.h"       //dan add for integrate ISDBT utopia driver
#include "drvDMD_VD_MBX.h"
#include "drvMSPI.h"
#include "drvSYS.h"


MS_BOOL MSB3400_Demod_I2C_ByPass(MS_U8 u8DemodIndex,MS_BOOL bOn);
#if MSB3400_INIT_BY_MSPI
static MS_BOOL ISDBT_TS_SERIAL = FALSE;
static MS_BOOL msb3400_MSPI_Init(MS_U8 u8DemodIndex);
static void msb3400_SPIPAD_TS0_En(MS_BOOL bOnOff);
static void msb3400_SPIPAD_TS1_En(MS_BOOL bOnOff);
static void msb3400_SPIPAD_TS2_En(MS_BOOL bOnOff);
static fpSPIPAD_En SPIPAD_EN[3]= {msb3400_SPIPAD_TS0_En,\
                                  msb3400_SPIPAD_TS1_En,\
                                  msb3400_SPIPAD_TS2_En};

static void msb3400_SPIPAD_TS0_En(MS_BOOL bOnOff)
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

static void msb3400_SPIPAD_TS1_En(MS_BOOL bOnOff)
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

static void msb3400_SPIPAD_TS2_En(MS_BOOL bOnOff)
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
//     General Function
//##########################################################################################################
static MS_U8      u8max_dev_num = 0;
MSB3400_Handle* pstMSB3400_Handle = NULL;
SLAVE_ID_USAGE* pstMSB3400_slave_ID_TBL = NULL;
static MS_BOOL* pDemodRest = NULL;

static SLAVE_ID_USAGE MSB3400_possible_slave_ID[3] =
{
    {0xD2, FALSE},
    {0xF2, FALSE},
    {0xFF, FALSE}
};

static MSB3400_Handle MSB3400_Init =
{
    FALSE, //bInited
    FALSE, //bOpen
    -1,    //s32_MSB3400_Mutex
    0,     //u32CurrFreq
    0xD2,  //u8SlaveID
    0,     //u32DmxInputPath;
    {NULL},//MSB3400_InitParam
    DEMOD_ISDBT_Layer_A, //eISDBT_Layer
};


static MS_U8 u8MSB3400_DevCnt = 0;

static MS_BOOL get_demod_handle(MS_U8 u8DemodIndex, MSB3400_Handle** ppHandle)
{
    if((pstMSB3400_Handle == NULL) || (u8DemodIndex > u8max_dev_num))
    {
        TUNER_ERR(("[MSB3400] Get Handle FAIL\n"));
        return FALSE;
    }
	
    *ppHandle = pstMSB3400_Handle + u8DemodIndex;
    return TRUE;
}

static MS_BOOL _mutex_lock(MS_U8 u8DemodIndex)
{
    MSB3400_Handle* pstHandle = NULL;
    if(!get_demod_handle(u8DemodIndex, &pstHandle))
        return FALSE;

    if(MsOS_ObtainMutex(pstHandle->s32_MSB3400_Mutex, ISDBTDMD_MUTEX_TIMEOUT) == FALSE)
        return FALSE;

    return TRUE;
}

static MS_BOOL _mutex_unlock(MS_U8 u8DemodIndex)
{    
    MSB3400_Handle* pstHandle = NULL;
    if(!get_demod_handle(u8DemodIndex, &pstHandle))
        return FALSE;
	
    return MsOS_ReleaseMutex(pstHandle->s32_MSB3400_Mutex);;
}

static MS_BOOL _IsInited(MS_U8 u8DemodIndex)
{
    MSB3400_Handle* pstHandle = NULL;
    if(!get_demod_handle(u8DemodIndex, &pstHandle))
        return FALSE;

    return pstHandle->bInited;
}


static MS_BOOL get_i2c_port(MS_U8 u8DemodIndex, MS_IIC_PORT* ePort)
{
    MSB3400_Handle* pstHandle = NULL;
	
    if(!get_demod_handle(u8DemodIndex, &pstHandle))
        return FALSE;
    else
    {
        *ePort = pstHandle->MSB3400_InitParam.stDMDCon.eI2C_PORT;
        return TRUE;
    }
}

static MS_BOOL MSB3400_Variables_alloc(void)
{
    MS_U8 i,j;
    MSB3400_Handle *pstHandle = NULL;
    SLAVE_ID_USAGE* pSlaveIDTBL = NULL;
    MS_U8 u8MaxI2CPort;

    u8MaxI2CPort = (MS_U8)((E_MS_IIC_SW_PORT_0/8) + (E_MS_IIC_PORT_NOSUP - E_MS_IIC_SW_PORT_0));

    if(NULL == pstMSB3400_Handle)
    {
        pstMSB3400_Handle = (MSB3400_Handle *)malloc(sizeof(MSB3400_Handle) * u8max_dev_num);
        if(NULL == pstMSB3400_Handle)
        {
            return FALSE;
        }
        else
        {
            for(i=0; i< u8max_dev_num; i++)
            {
                pstHandle = (pstMSB3400_Handle + i);
				memcpy(pstHandle, &MSB3400_Init, sizeof(MSB3400_Handle));
                pstHandle->MSB3400_InitParam.stDMDCon.u32TSVLDInterrupt = DMD_CON_INFO_NOT_DEFINE;
            }
        }
    }

    if(NULL == pDemodRest)
    {
        pDemodRest = (MS_BOOL*)malloc(sizeof(MS_BOOL) * u8max_dev_num);
        if(NULL == pDemodRest)
        {
            return FALSE;
        }
        else
        {
            for(i=0; i< u8max_dev_num; i++)
                *(pDemodRest + i) = TRUE;
        }
    }

    if(NULL == pstMSB3400_slave_ID_TBL)
    {
        pstMSB3400_slave_ID_TBL = (SLAVE_ID_USAGE *)malloc(sizeof(MSB3400_possible_slave_ID) * u8MaxI2CPort);
        if(NULL == pstMSB3400_slave_ID_TBL)
        {
            return FALSE;
        }
        else
        {
            for(i=0; i< u8MaxI2CPort; i++)
            {
                for(j=0; j< (sizeof(MSB3400_possible_slave_ID)/sizeof(SLAVE_ID_USAGE)); j++)
                {
                    pSlaveIDTBL = (pstMSB3400_slave_ID_TBL + i*sizeof(MSB3400_possible_slave_ID)/sizeof(SLAVE_ID_USAGE) + j);
                    memcpy(pSlaveIDTBL, &MSB3400_possible_slave_ID[j], sizeof(SLAVE_ID_USAGE));
                 }
            }

        }
    }
     
    return TRUE;

}

static void _variable_free(void** pp)
{
    if(NULL != *pp)
    {
        free(*pp);
        *pp = NULL;
    }
}

static MS_BOOL MSB3400_Variables_free(void)
{
    _variable_free((void*)&pstMSB3400_Handle);
    _variable_free((void*)&pstMSB3400_slave_ID_TBL);
    return TRUE;
}

static void MSB3400_hw_reset(MS_U8 u8DemodIndex)
{
    int rstPin;
    MSB3400_Handle* pstHandle = NULL;
	
    if(!get_demod_handle(u8DemodIndex, &pstHandle))
        return;
	
    rstPin = (int)(pstHandle->MSB3400_InitParam.stDMDCon.u32HW_ResetPin);
    mdrv_gpio_set_high(rstPin);
    MsOS_DelayTask(5);
    mdrv_gpio_set_low(rstPin);
    MsOS_DelayTask(10);
    mdrv_gpio_set_high(rstPin);
    MsOS_DelayTask(5);

    pstHandle->bInited = FALSE;
    pstHandle->bOpen = FALSE;
	*(pDemodRest + u8DemodIndex) = TRUE;

}


//##########################################################################################################
//       IIC interface
//##########################################################################################################

MS_BOOL   MDrv_MSB3400_IIC_Write(MS_U16 u16BusNumSlaveID, MS_U8 u8AddrNum, MS_U8* pu8addr, MS_U16 u16size, MS_U8* pu8data)
{
    MS_IIC_PORT ePort;
	MS_U16 u16SlaveID;
	MS_BOOL bret;

    ePort = ((u16BusNumSlaveID & 0xff00)>>8);
	u16SlaveID = u16BusNumSlaveID & 0xff;
	if(ePort > E_MS_IIC_PORT5_7) //support HWI2C only
    {
       return FALSE;
	}
	
    bret = MDrv_IIC_WriteBytes(ePort, u16SlaveID, u8AddrNum, pu8addr, u16size, pu8data);
	if(bret == FALSE)
	{
       DMD_ERR(("MDrv_MSB3400_IIC_Write fail, ePort = %d, slave ID = 0x%02x\n", (int)ePort, (int)u16SlaveID));
	}
	return bret;
}

MS_BOOL   MDrv_MSB3400_IIC_Read(MS_U16 u16BusNumSlaveID, MS_U8 u8AddrNum, MS_U8* pu8addr, MS_U16 u16size, MS_U8* pu8data)
{
    MS_IIC_PORT ePort;
	MS_U16 u16SlaveID;
	MS_BOOL bret;

    ePort = ((u16BusNumSlaveID & 0xff00)>>8);
	u16SlaveID = u16BusNumSlaveID & 0xff;
	if(ePort > E_MS_IIC_PORT5_7) //support HWI2C only
    {
       return FALSE;
	}

    bret = MDrv_IIC_ReadBytes(ePort, u16SlaveID, u8AddrNum, pu8addr, u16size, pu8data);
	if(bret == FALSE)
	{
       DMD_ERR(("MDrv_MSB3400_IIC_Read fail, ePort = %d, slave ID = 0x%02x\n", (int)ePort, (int)u16SlaveID));
	}
	return bret;

}

MS_BOOL MSB3400_WriteBytes(MS_U8 u8DemodIndex, MS_U8 u8AddrSize, MS_U8 *pu8Addr, MS_U16 u16Size, MS_U8 *pu8Data)
{
    MS_BOOL bRet = 0;
    MS_IIC_PORT ePort = E_MS_IIC_PORT_0;
	MSB3400_Handle* pstHandle = NULL;
	
    if(!get_demod_handle(u8DemodIndex, &pstHandle))
        return FALSE;
    
    get_i2c_port(u8DemodIndex, &ePort);
    bRet = MDrv_IIC_WriteBytes(ePort, (MS_U16)pstHandle->u8SlaveID, u8AddrSize, pu8Addr, u16Size, pu8Data);
    if (FALSE == bRet)
    {
        DMD_ERR(("[MSB3400] IIC write error, ePort = %x, ID = 0x%x\n", (int)ePort, pstHandle->u8SlaveID));
    }
    return bRet;
}

MS_BOOL MSB3400_ReadBytes(MS_U8 u8DemodIndex, MS_U8 u8AddrSize, MS_U8 *pu8Addr, MS_U16 u16Size, MS_U8 *pu8Data)
{
    MS_BOOL bRet = 0;
    MS_IIC_PORT ePort = E_MS_IIC_PORT_0;
    MSB3400_Handle* pstHandle = NULL;
	
    if(!get_demod_handle(u8DemodIndex, &pstHandle))
        return FALSE;
    
    get_i2c_port(u8DemodIndex, &ePort);
    bRet = MDrv_IIC_ReadBytes(ePort, (MS_U16)pstHandle->u8SlaveID, u8AddrSize, pu8Addr, u16Size, pu8Data);

    if (FALSE == bRet)
    {
        DMD_ERR(("[MSB3046] IIC read error\n"));
    }
    return bRet;
}



//##########################################################################################################
//        Public:Common Function Implementation
//##########################################################################################################
/* For FE auto test */
MS_BOOL MSB3400_Demod_WriteReg(MS_U16 u16Addr, MS_U8 u8Data)
{
    MS_BOOL bRet = TRUE;

    return bRet;
}

/* For FE auto test */
MS_U8 MSB3400_Demod_ReadReg(MS_U16 u16Addr)
{
    MS_U8 u8Data = 0;

    return u8Data;
}

MS_BOOL MSB3400_Demod_Open(void)
{
    
    return TRUE;
}

MS_BOOL MSB3400_Demod_PowerOnOff(MS_BOOL bPowerOn)
{

    return TRUE;
}

MS_BOOL MSB3400_Demod_Close(void)
{

    return TRUE;
}

/****************************************************************************
**function:
**
**
****************************************************************************/
MS_BOOL MSB3400_Demod_GetSignalQuality(MS_U8 u8DemodIndex,MS_U16 *pu16quality)
{
    MS_BOOL bRet = TRUE;

    if (_mutex_lock(u8DemodIndex) == FALSE)
    {
        DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

	if (!_IsInited(u8DemodIndex))
    {
        DMD_ERR(("[%s]MSB140X have not inited !!!\n", __FUNCTION__));
        _mutex_unlock(u8DemodIndex);
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

	_mutex_unlock(u8DemodIndex);
    return bRet;
}

/****************************************************************************
**function:
**
**
****************************************************************************/
MS_BOOL MSB3400_Demod_GetSNR(MS_U8 u8DemodIndex,float *pfSNR)
{
    MS_BOOL bRet = TRUE;

    if (_mutex_lock(u8DemodIndex) == FALSE)
    {
        DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

	if (!_IsInited(u8DemodIndex))
    {
        DMD_ERR(("[%s]MSB140X have not inited !!!\n", __FUNCTION__));
        _mutex_unlock(u8DemodIndex);
        return FALSE;
    }


    *pfSNR = 0;
    if (DMD_ISDBT_LOCK == MDrv_DMD_ISDBT_GetLock(DMD_ISDBT_GETLOCK))
    {
        bRet = MDrv_DMD_ISDBT_GetSNR(pfSNR);
    }

	_mutex_unlock(u8DemodIndex);

    return bRet;
}


/****************************************************************************
**function:
**
**
****************************************************************************/
MS_BOOL MSB3400_Demod_GetBER(MS_U8 u8DemodIndex,float *pfBER)
{
    MS_BOOL bRet = TRUE;
    MSB3400_Handle* pstHandle = NULL;
	
    if(!get_demod_handle(u8DemodIndex, &pstHandle))
        return FALSE;

    if (_mutex_lock(u8DemodIndex) == FALSE)
    {
        DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

	if (!_IsInited(u8DemodIndex))
    {
        DMD_ERR(("[%s]MSB140X have not inited !!!\n", __FUNCTION__));
        _mutex_unlock(u8DemodIndex);
        return FALSE;
    }

    if (DMD_ISDBT_LOCK == MDrv_DMD_ISDBT_GetLock(DMD_ISDBT_GETLOCK))
    {
        bRet = MDrv_DMD_ISDBT_GetPostViterbiBer((EN_ISDBT_Layer)pstHandle->eISDBT_Layer, pfBER);
    }
    else
    {
        *pfBER = 0.0;
    }

	_mutex_unlock(u8DemodIndex);

    return bRet;
}

MS_BOOL MSB3400_Demod_GetPWR(MS_U8 u8DemodIndex,MS_S32 *ps32Signal)
{
    MS_BOOL bret = TRUE;
	MSB3400_Handle* pstHandle = NULL;
	
    if(!get_demod_handle(u8DemodIndex, &pstHandle))
        return FALSE;
	
    if (_mutex_lock(u8DemodIndex) == FALSE)
    {
        DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

	if (!_IsInited(u8DemodIndex))
    {
        DMD_ERR(("[%s]MSB140X have not inited !!!\n", __FUNCTION__));
        _mutex_unlock(u8DemodIndex);
        return FALSE;
    }


    if (DMD_ISDBT_LOCK == MDrv_DMD_ISDBT_GetLock(DMD_ISDBT_GETLOCK))
    {
        MSB3400_Demod_I2C_ByPass(u8DemodIndex, TRUE);
        bret = pstHandle->MSB3400_InitParam.pstTunertab->Extension_Function(u8DemodIndex, TUNER_EXT_FUNC_GET_POWER_LEVEL, ps32Signal);
        MSB3400_Demod_I2C_ByPass(u8DemodIndex, FALSE);
    }

	_mutex_unlock(u8DemodIndex);

    return bret;
}

MS_BOOL MSB3400_Demod_GetSSI(MS_U8 u8DemodIndex,MS_U16 *pu16SSI)
{
    MS_U16  u16SignalStrengthValue;

    if (_mutex_lock(u8DemodIndex) == FALSE)
    {
        DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

	if (!_IsInited(u8DemodIndex))
    {
        DMD_ERR(("[%s]MSB140X have not inited !!!\n", __FUNCTION__));
        _mutex_unlock(u8DemodIndex);
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

	_mutex_unlock(u8DemodIndex);

    return TRUE;
}

MS_BOOL MSB3400_Demod_Demod_GetParam(MS_U8 u8DemodIndex,DEMOD_MS_FE_CARRIER_PARAM* pParam)
{
    MS_BOOL bRtn = FALSE;
    MS_U8 i;
    sISDBT_MODULATION_MODE stMOD;

    
    for(i=0;i<3;i++)
    {
        bRtn = MDrv_DMD_ISDBT_GetModulationMode((EN_ISDBT_Layer)i, &stMOD);
        if(bRtn)
        {
            memcpy(&pParam->ISDBTParam.stLayerX_Param[i], &stMOD, sizeof(sISDBT_MODULATION_MODE));
        }
        else
        {
            return bRtn;
        }
    }
    return bRtn;
}

MS_BOOL MSB3400_Demod_GetLock(MS_U8 u8DemodIndex,EN_LOCK_STATUS *peLockStatus)
{
    DMD_ISDBT_LOCK_STATUS eLockStatus;

    *peLockStatus = E_DEMOD_CHECKING;
    if (_mutex_lock(u8DemodIndex) == FALSE)
    {
        DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    if (!_IsInited(u8DemodIndex))
    {
        DMD_ERR(("[%s]MSB140X have not inited !!!\n", __FUNCTION__));
        _mutex_unlock(u8DemodIndex);
        return FALSE;
    }


    eLockStatus = MDrv_DMD_ISDBT_GetLock(DMD_ISDBT_GETLOCK);

    DMD_DBG(("eLockStatus: %d\n", (int)eLockStatus));
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
	
	_mutex_unlock(u8DemodIndex);

    return TRUE;
}


MS_BOOL MSB3400_Demod_Restart(MS_U8 u8DemodIndex,DEMOD_MS_FE_CARRIER_PARAM *pParam,MS_U32 u32BroadCastType)
{
    MS_BOOL  bRet = TRUE;

    if (_mutex_lock(u8DemodIndex) == FALSE)
    {
        DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    if (!_IsInited(u8DemodIndex))
    {
        DMD_ERR(("[%s]MSB3400 have not inited !!!\n", __FUNCTION__));
        _mutex_unlock(u8DemodIndex);
        return FALSE;
    }

    bRet &= MDrv_DMD_ISDBT_SetConfig(TRUE);

	_mutex_unlock(u8DemodIndex);

    return bRet;
}

MS_BOOL MSB3400_Demod_I2C_ByPass(MS_U8 u8DemodIndex,MS_BOOL bOn)
{
    MS_BOOL bret=TRUE;
	//MS_U8 u8data=0; 

    if (!_IsInited(u8DemodIndex))
    {
        bret &= MSB3400_I2C_CH_Reset(u8DemodIndex,3);
        if(bOn)
        {
            bret &= MSB3400_SetReg(u8DemodIndex, 0x0951, 0x00);
            bret &= MSB3400_SetReg(u8DemodIndex, 0x0910, 0x10);
        }
        else
        {
            bret &= MSB3400_SetReg(u8DemodIndex, 0x0910, 0x00);
			bret &= MSB3400_SetReg(u8DemodIndex, 0x0951, 0x01);
        }
        return bret;
    }

    if (_mutex_lock(u8DemodIndex) == FALSE)
    {
        DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    bret &= MDrv_DMD_ISDBT_IIC_BYPASS_MODE(bOn);

	_mutex_unlock(u8DemodIndex);
	
    return bret;
}

MS_BOOL MSB3400_Demod_Init(MS_U8 u8DemodIndex,DEMOD_MS_INIT_PARAM* pParam)
{
    MS_BOOL bRet = TRUE;
    //MS_U32 u32TunerIF = 0;
	DMD_ISDBT_InitData sDMD_ISDBT_InitData;
    static MS_U8 u8DMD_ISDBT_InitExt[] = {1}; // RFAGC tristate control default value, 1:trisate 0:non-tristate,never modify unless you know the meaning
    MSB3400_Handle* pstHandle = NULL;
	
    if(!get_demod_handle(u8DemodIndex, &pstHandle))
        return FALSE;

    DMD_DBG(("\nMSB1400_Demod_Init\n"));
    if (pstHandle->s32_MSB3400_Mutex< 0)
    {
        pstHandle->s32_MSB3400_Mutex = MsOS_CreateMutex(E_MSOS_FIFO, "Isdbt_Mutex", MSOS_PROCESS_SHARED);

        if (pstHandle->s32_MSB3400_Mutex < 0)
        {
            return FALSE;
        }
    }

    bRet &= MDrv_DMD_PreInit();

	memcpy(&pstHandle->MSB3400_InitParam, pParam,sizeof(DEMOD_MS_INIT_PARAM));
#if MSB3400_INIT_BY_MSPI
	if(pstHandle->MSB3400_InitParam.stDMDCon.bSupportMSPILoad)
    {
       if(pstHandle->MSB3400_InitParam.stDMDCon.bEnMSPIPathDet)
       {
           DMD_DBG(("MSPI path is set by detected result\n"));
       }
       else
           pstHandle->u32DmxInputPath = pParam->u32DmxInputPath;
    }
#else
    pstHandle->u32DmxInputPath = pParam->u32DmxInputPath;
#endif

    sDMD_ISDBT_InitData.u16IF_KHZ = 5000;
    sDMD_ISDBT_InitData.bIQSwap = MSB3400_TS_DATA_SWAP;          // IQ Swap  //this define will depend on tuner;
    sDMD_ISDBT_InitData.u16ISDBTFECLockCheckTime = 2000;//ISDBT_FEC_timeout;
    sDMD_ISDBT_InitData.u16ISDBTIcfoChExistCheckTime = 300;//ISDBT_ChExist_timeout;
    sDMD_ISDBT_InitData.u8DMD_ISDBT_InitExt = u8DMD_ISDBT_InitExt;
    sDMD_ISDBT_InitData.u16AgcReferenceValue = MSB3400_IFAGC_REF; //this define will depend on tuner

    //TS[begin]
    if(pstHandle->MSB3400_InitParam.stDMDCon.bTSIsParallel)
    {
		sDMD_ISDBT_InitData.u5TsConfigByte_DivNum = MSB3400_TS_CLK_DIVNUM_PARALLEL_MODE;
		sDMD_ISDBT_InitData.u1TsConfigByte_SerialMode = 0;
    }
	else
    {
        sDMD_ISDBT_InitData.u5TsConfigByte_DivNum = MSB3400_TS_CLK_DIVNUM_SERIAL_MODE;
		sDMD_ISDBT_InitData.u1TsConfigByte_SerialMode = 1;
    }
    sDMD_ISDBT_InitData.u1TsConfigByte_ClockInv = MSB3400_TS_CLK_INV;
    sDMD_ISDBT_InitData.u1TsConfigByte_DataSwap = MSB3400_TS_DATA_SWAP;
    //TS[end]

    //I2C[begin]
    sDMD_ISDBT_InitData.u8I2CSlaveAddr = (pstHandle->u8SlaveID);
    sDMD_ISDBT_InitData.u8I2CSlaveBus = (MS_U8)pstHandle->MSB3400_InitParam.stDMDCon.eI2C_PORT;
    sDMD_ISDBT_InitData.bIsExtDemod = TRUE;

    sDMD_ISDBT_InitData.I2C_WriteBytes = MDrv_MSB3400_IIC_Write;
    sDMD_ISDBT_InitData.I2C_ReadBytes = MDrv_MSB3400_IIC_Read;


    //I2C[end]

	#if MSB3400_INIT_BY_MSPI
	ISDBT_TS_SERIAL = !(pstHandle->MSB3400_InitParam.stDMDCon.bTSIsParallel);
    if(pstHandle->MSB3400_InitParam.stDMDCon.bSupportMSPILoad)
    {
       if(pstHandle->MSB3400_InitParam.stDMDCon.bEnMSPIPathDet == TRUE)
       {
          if((EXT_DMD_MSPI_PATH_NONE != pstHandle->u32DmxInputPath) && (pstHandle->u32DmxInputPath < 3))
          {
              msb3400_MSPI_Init(u8DemodIndex); 
			  SPIPAD_EN[pstHandle->u32DmxInputPath](TRUE);
              sDMD_ISDBT_InitData.bIsUseSspiLoadCode = TRUE;
              sDMD_ISDBT_InitData.bIsSspiUseTsPin = TRUE;
          }
		  else
		  {
			  sDMD_ISDBT_InitData.bIsUseSspiLoadCode = FALSE;
			  sDMD_ISDBT_InitData.bIsSspiUseTsPin = FALSE;
		  }
       }
       else if(DMX_INPUT_EXT_INPUT0 == pstHandle->u32DmxInputPath)//manual set
       {
           msb3400_MSPI_Init(u8DemodIndex); 
		   SPIPAD_EN[0](TRUE);
		   sDMD_ISDBT_InitData.bIsUseSspiLoadCode = TRUE;
		   sDMD_ISDBT_InitData.bIsSspiUseTsPin = TRUE;
       }
       else if(DMX_INPUT_EXT_INPUT1 == pstHandle->u32DmxInputPath)
       {
       	   msb3400_MSPI_Init(u8DemodIndex); 
		   SPIPAD_EN[1](TRUE);
		   sDMD_ISDBT_InitData.bIsUseSspiLoadCode = TRUE;
  	       sDMD_ISDBT_InitData.bIsSspiUseTsPin = TRUE;
       }
       else
       {
		   sDMD_ISDBT_InitData.bIsUseSspiLoadCode = FALSE;
		   sDMD_ISDBT_InitData.bIsSspiUseTsPin = FALSE;
       }
    }
    else
   #endif		
    {
		sDMD_ISDBT_InitData.bIsUseSspiLoadCode = FALSE;
		sDMD_ISDBT_InitData.bIsSspiUseTsPin = FALSE;
    }

    sDMD_ISDBT_InitData.bIsExtDemod = 1;

    bRet &= MDrv_DMD_ISDBT_Initial_Hal_Interface();
    bRet &= MDrv_DMD_ISDBT_Init(&sDMD_ISDBT_InitData, sizeof(sDMD_ISDBT_InitData));

#if MSB3400_INIT_BY_MSPI
    if(pstHandle->MSB3400_InitParam.stDMDCon.bSupportMSPILoad)
    {
       if(pstHandle->MSB3400_InitParam.stDMDCon.bEnMSPIPathDet == TRUE)
       {
    	  if((EXT_DMD_MSPI_PATH_NONE != pstHandle->u32DmxInputPath) && (pstHandle->u32DmxInputPath < 3))
    	  {
    		  SPIPAD_EN[pstHandle->u32DmxInputPath](FALSE);
    	  }
       }
       else if(DMX_INPUT_EXT_INPUT0 == pstHandle->u32DmxInputPath)//manual set
       {
    	   SPIPAD_EN[0](FALSE);
       }
       else if(DMX_INPUT_EXT_INPUT1 == pstHandle->u32DmxInputPath)
       {
    	   SPIPAD_EN[1](FALSE);
       }
    }
#endif

    if (!bRet)
    {
        pstHandle->bInited = FALSE;
        DMD_ERR(("MSB3400 Init error\n"));
        MsOS_DeleteMutex(pstHandle->s32_MSB3400_Mutex);
        return FALSE;
    }

    pstHandle->bInited = TRUE;
    return bRet;
}

//===================================================================================
MS_BOOL MSB3400_GetReg(MS_U8 u8DemodIndex, MS_U16 u16Addr, MS_U8 *pu8Data)
{
    MS_BOOL bRet=TRUE;
    MS_U8 u8MsbData[6];

    u8MsbData[0] = 0x10;
    u8MsbData[1] = 0x00;
    u8MsbData[2] = 0x00;
    u8MsbData[3] = (u16Addr >> 8) &0xff;
    u8MsbData[4] = u16Addr & 0xff;

    u8MsbData[0] = 0x35;
    bRet &= MSB3400_WriteBytes(u8DemodIndex, 0, NULL, 1, u8MsbData);

    u8MsbData[0] = 0x10;
    bRet &= MSB3400_WriteBytes(u8DemodIndex, 0, NULL, 5, u8MsbData);
    bRet &= MSB3400_ReadBytes(u8DemodIndex, 0, NULL, 1, pu8Data);

    u8MsbData[0] = 0x34;
    bRet &= MSB3400_WriteBytes(u8DemodIndex, 0, NULL, 1, u8MsbData);

    return bRet;
}

MS_BOOL MSB3400_SetReg(MS_U8 u8DemodIndex, MS_U16 u16Addr, MS_U8 u8Data)
{
    MS_BOOL bRet=TRUE;
    MS_U8 u8MsbData[6];

    u8MsbData[0] = 0x10;
    u8MsbData[1] = 0x00;
    u8MsbData[2] = 0x00;
    u8MsbData[3] = (u16Addr >> 8) &0xff;
    u8MsbData[4] = u16Addr &0xff;
    u8MsbData[5] = u8Data;

    u8MsbData[0] = 0x35;
    bRet &= MSB3400_WriteBytes(u8DemodIndex, 0, NULL, 1, u8MsbData);

    u8MsbData[0] = 0x10;
    bRet &= MSB3400_WriteBytes(u8DemodIndex, 0, NULL, 6, u8MsbData);

    u8MsbData[0] = 0x34;
    bRet &= MSB3400_WriteBytes(u8DemodIndex, 0, NULL, 1, u8MsbData);
    return bRet;
}


MS_BOOL MSB3400_I2C_CH_Reset(MS_U8 u8DemodIndex, MS_U8 ch_num)
{
    MS_BOOL bRet = TRUE;
    MS_U8     data[5] = {0x53, 0x45, 0x52, 0x44, 0x42};

    // ch_num, 0: for MIU access, 3: for RIU access
    DMD_DBG(("[MSB3400][beg] I2C_CH_Reset, CH=0x%x\n", ch_num));

    if(pDemodRest == NULL)
        return FALSE;

    if (*(pDemodRest + u8DemodIndex))
    {
        // 8'hb2(SRID)->8,h53(PWD1)->8,h45(PWD2)->8,h52(PWD3)->8,h44(PWD4)->8,h42(PWD5)
        data[0] = 0x53;
        // Don't check Ack because this passward only ack one time for the first time.
        if(MSB3400_WriteBytes(u8DemodIndex, 0, NULL, 5, data))
            *(pDemodRest + u8DemodIndex) = FALSE;
        else
            return FALSE;
    }

    // 8'hb2(SRID)->8,h71(CMD)  //TV.n_iic_
    data[0] = 0x71;
    bRet &= MSB3400_WriteBytes(u8DemodIndex, 0, NULL, 1, data);

    // 8'hb2(SRID)->8,h81(CMD)  //TV.n_iic_sel_b0
    data[0] = ((ch_num & 0x01) != 0) ? 0x81 : 0x80;
    bRet &= MSB3400_WriteBytes(u8DemodIndex, 0, NULL, 1, data);

    // 8'hb2(SRID)->8,h83(CMD)  //TV.n_iic_sel_b1
    data[0] = ((ch_num & 0x02) != 0) ? 0x83 : 0x82;
    bRet &= MSB3400_WriteBytes(u8DemodIndex, 0, NULL, 1, data);

    // 8'hb2(SRID)->8,h84(CMD)  //TV.n_iic_sel_b2
    data[0] = ((ch_num & 0x04) != 0) ? 0x85 : 0x84;
    bRet &= MSB3400_WriteBytes(u8DemodIndex, 0, NULL, 1, data);

    // 8'hb2(SRID)->8,h53(CMD)  //TV.n_iic_ad_byte_en2, 32bit read/write
    data[0] = 0x53;
    bRet &= MSB3400_WriteBytes(u8DemodIndex, 0, NULL, 1, data);

    // 8'hb2(SRID)->8,h7f(CMD)  //TV.n_iic_sel_use_cfg
    data[0] = 0x7f;
    bRet &= MSB3400_WriteBytes(u8DemodIndex, 0, NULL, 1, data);

    DMD_DBG(("[MSB3400][end] I2C_CH_Reset, CH=0x%x\n", ch_num));
    return bRet;
}


#define MSB3400_CHIP_ID 0xD2
static MS_BOOL msb3400_I2C_CH3_reset(MS_U8 u8DemodIndex)
{
    //Reset twice to check if reset pin is correct
    MSB3400_hw_reset(u8DemodIndex);
    if(!MSB3400_I2C_CH_Reset(u8DemodIndex,3))
    {
        DMD_ERR(("[MSB124X] I2C_CH_Reset fail \n"));
        return FALSE;
    }
    else
    {
        MSB3400_hw_reset(u8DemodIndex);
        if(!MSB3400_I2C_CH_Reset(u8DemodIndex,3))
        {
            DMD_ERR(("[MSB124X] I2C_CH_Reset fail \n"));
            return FALSE;
        }
   }
   return TRUE;
}

#if MSB3400_INIT_BY_MSPI
static MS_BOOL msb3400_MSPI_Init(MS_U8 u8DemodIndex)
{
    MS_U8 u8_tmp;
	MS_BOOL bret = TRUE;

    bret &= MDrv_DMD_SSPI_Init(0);
    bret &= MDrv_MasterSPI_CsPadConfig(0, 0xff);
    bret &= MDrv_MasterSPI_MaxClkConfig(0, 27);
    bret &= MSB3400_I2C_CH_Reset(u8DemodIndex,3);	
	bret &= MSB3400_SetReg(u8DemodIndex, 0x0900+(0x28)*2, 0x00);
	bret &= MSB3400_SetReg(u8DemodIndex, 0x0900+(0x28)*2 + 1, 0x00);
	bret &= MSB3400_SetReg(u8DemodIndex, 0x0900+(0x2d)*2, 0xff);
	bret &= MSB3400_SetReg(u8DemodIndex, 0x0900+(0x2d)*2 + 1, 0x00);
			
	// ------enable to use TS_PAD as SSPI_PAD
	// [0:0] reg_en_sspi_pad
	// [1:1] reg_ts_sspi_en, 1: use TS_PAD as SSPI_PAD
	bret &= MSB3400_SetReg(u8DemodIndex, 0x0900+(0x3b)*2, 0x02);
	bret &= MSB3400_SetReg(u8DemodIndex, 0x0900+(0x3b)*2 + 1, 0x00);
			
	// ------- MSPI protocol setting
	// [8] cpha
	// [9] cpol
	bret &= MSB3400_GetReg(u8DemodIndex, 0x0900+(0x3a)*2+1, &u8_tmp);
	u8_tmp &= 0xFC;//0xFC
	bret &= MSB3400_SetReg(u8DemodIndex, 0x0900+(0x3a)*2+1, u8_tmp);
			
	// ------- MSPI driving setting
	bret &= MSB3400_SetReg(u8DemodIndex, 0x0900+(0x2c)*2, 0xff);
	bret &= MSB3400_SetReg(u8DemodIndex, 0x0900+(0x2c)*2 + 1, 0x07);

	return bret;

}
#endif

MS_BOOL MSB3400_Check_Exist(MS_U8 u8DemodIndex, MS_U8* pu8SlaveID)
{
    MS_U8 u8_tmp = 0;
    MS_U8 i, u8I2C_Port = 0;
    SLAVE_ID_USAGE *pMSB3400_ID_TBL;
    MS_IIC_PORT ePort = 0;
	MSB3400_Handle* pstHandle = NULL;

    if(!MSB3400_Variables_alloc())
    {
        MSB3400_Variables_free();
        return FALSE;
    }
    else
    {
       if(!get_demod_handle(u8DemodIndex, &pstHandle))
	       return FALSE;
    }

	get_i2c_port(u8DemodIndex, &ePort);
    if((int)ePort < (int)E_MS_IIC_SW_PORT_0)
    {
       u8I2C_Port = (MS_U8)ePort/8;
    }
    else if((int)ePort < (int)E_MS_IIC_PORT_NOSUP)//sw i2c
    {
       u8I2C_Port = E_MS_IIC_SW_PORT_0/8 + (ePort - E_MS_IIC_SW_PORT_0);
    }



    i=0;
    do
    {
        pMSB3400_ID_TBL = pstMSB3400_slave_ID_TBL + u8I2C_Port*sizeof(MSB3400_possible_slave_ID)/sizeof(SLAVE_ID_USAGE) + i;
        DMD_DBG(("### %x\n",i));
        if(pMSB3400_ID_TBL->u8SlaveID == 0xFF)
        {
            DMD_DBG(("[MSB3400] All Slave ID have tried but not detect\n"));
            break;
        }

        if(pMSB3400_ID_TBL->bInUse)
        {
            DMD_DBG(("[MSB3400] Slave ID 0x%x have been used\n", pMSB3400_ID_TBL->u8SlaveID));
            i++;
            continue;
        }
        else
        {
            pstHandle->u8SlaveID = pMSB3400_ID_TBL->u8SlaveID;
            DMD_DBG(("[MSB3400] Try slave ID 0x%x\n",pMSB3400_ID_TBL->u8SlaveID));
        }

        if(msb3400_I2C_CH3_reset(u8DemodIndex))
        {
            DMD_DBG(("[MSB3400] I2C slave id :%x \n",pstHandle->u8SlaveID ));
            if(MSB3400_GetReg(u8DemodIndex,0x0900,&u8_tmp))
            {
                DMD_DBG(("[MSB3400] read id :%x \n",u8_tmp ));
                if(u8_tmp == MSB3400_CHIP_ID)
                {
                    pMSB3400_ID_TBL->bInUse = TRUE;
                    break;
                }
            }
        }

        i++;
    }while((pMSB3400_ID_TBL->u8SlaveID) != 0xFF);


    if ( u8_tmp == MSB3400_CHIP_ID )
    {
        *pu8SlaveID = pstHandle->u8SlaveID;
		 u8MSB3400_DevCnt++;
#if MSB3400_INIT_BY_MSPI		
        if(pstHandle->MSB3400_InitParam.stDMDCon.bSupportMSPILoad)
        {
            if(pstHandle->MSB3400_InitParam.stDMDCon.bEnMSPIPathDet)
            {
                MS_U8 i;
				msb3400_MSPI_Init(u8DemodIndex); 
                for(i=0; i<EXT_DMD_MSPI_PATH_MAX; i++)
                {
                    SPIPAD_EN[i](TRUE);
                
                    MDrv_DMD_SSPI_RIU_Read8(0x900, &u8_tmp);
                    MDrv_DMD_SSPI_RIU_Read8(0x900, &u8_tmp);
                
                    if(u8_tmp == MSB3400_CHIP_ID)
                    {
                        pstHandle->u32DmxInputPath = (MS_U32)i;
                        SPIPAD_EN[i](FALSE);
                        DMD_DBG(("Get MSB3400 chip ID by MSPI on TS%d\n", (int)pstHandle->u32DmxInputPath));
                        break;
                    }
                    else
                    {
                        DMD_DBG(("Cannot get MSB3400 chip ID by MSPI on TS%x\n", i));
                        if( i == (EXT_DMD_MSPI_PATH_MAX - 1))
                            pstHandle->u32DmxInputPath = EXT_DMD_MSPI_PATH_NONE;
                    }
                
                    SPIPAD_EN[i](FALSE);
                }
                
                // ------disable to use TS_PAD as SSPI_PAD after load code
                // [0:0] reg_en_sspi_pad
                // [1:1] reg_ts_sspi_en, 1: use TS_PAD as SSPI_PAD
                MSB3400_SetReg(u8DemodIndex, 0x0900+(0x3b)*2, 0x01);
                MSB3400_SetReg(u8DemodIndex, 0x0900+(0x3b)*2 + 1, 0x00);
            }
        }
#endif //MSB3400_INIT_BY_MSPI
		    return TRUE;
    }
    else
    {		 
	      if(!u8MSB3400_DevCnt)
		        MSB3400_Variables_free();
		    return FALSE;
	  }
}


MS_BOOL MSB3400_Extension_Function(MS_U8 u8DemodIndex, DEMOD_EXT_FUNCTION_TYPE fuction_type, void *data)
{
    MS_BOOL bret = TRUE;
    MSB3400_Handle* pstHandle = NULL;
	DEMOD_EXT_FUNCTION_PARAM* pstData;
    DEMOD_CON_CONFIG* pstCon;
	
    switch(fuction_type)
    {

    case DEMOD_EXT_FUNC_FINALIZE:
		if(!get_demod_handle(u8DemodIndex, &pstHandle))
            return FALSE;
		
        if (pstHandle->bInited)
        {
            bret &= MDrv_DMD_ISDBT_MD_SetPowerState(0,E_POWER_SUSPEND);
        }
        pstHandle->bInited = FALSE;
		MSB3400_hw_reset(u8DemodIndex);//for I2C re-write password issue
        break;
    case DEMOD_EXT_FUNC_POWER_ON_OFF:
        bret &= MSB3400_Demod_PowerOnOff(*(MS_U8 *)data);
        break;
    case DEMOD_EXT_FUNC_SET_ISDBT_LAYER:
		if(!get_demod_handle(u8DemodIndex, &pstHandle))
            return FALSE;
		
        pstHandle->eISDBT_Layer = *(DEMOD_ISDBT_Layer*)data;
        break;
		
    case DEMOD_EXT_FUNC_SET_CON_INFO:
        pstData = (DEMOD_EXT_FUNCTION_PARAM*)data;
        u8max_dev_num = pstData->u32Param1;
        pstCon = (DEMOD_CON_CONFIG*)(pstData->pParam);
        if(!MSB3400_Variables_alloc())
        {
            MSB3400_Variables_free();
            return FALSE;
        }
        else
        {
            if(!get_demod_handle(u8DemodIndex, &pstHandle))
	            return FALSE;

            memcpy(&pstHandle->MSB3400_InitParam.stDMDCon, pstCon,sizeof(DEMOD_CON_CONFIG));
			#if MSB3400_INIT_BY_MSPI
			ISDBT_TS_SERIAL = !pstHandle->MSB3400_InitParam.stDMDCon.bTSIsParallel;//Will be used in MSPI path detection only
			#endif
        }
        break;		
    default:
        DMD_DBG(("Request extension function (%x) does not exist\n",fuction_type));
    }
    return bret;
}


DRV_DEMOD_TABLE_TYPE GET_DEMOD_ENTRY_NODE(DEMOD_MSB3400) DDI_DRV_TABLE_ENTRY(demodtab) =
{
    .name                         = "DEMOD_MSB3400",
    .data                         = DEMOD_MSB3400,
    .SupportINT                   = FALSE,
    .init                         = MSB3400_Demod_Init,
    .GetLock                      = MSB3400_Demod_GetLock,
    .GetSNR                       = MSB3400_Demod_GetSNR,
    .GetBER                       = MSB3400_Demod_GetBER,
    .GetPWR                       = MSB3400_Demod_GetPWR,
    .GetSSI                       = MSB3400_Demod_GetSSI,
    .GetQuality                   = MSB3400_Demod_GetSignalQuality,
    .GetParam                     = MSB3400_Demod_Demod_GetParam,
    .Restart                      = MSB3400_Demod_Restart,
    .I2CByPass                    = MSB3400_Demod_I2C_ByPass,
    .I2CByPassPreSetting          = NULL,
    .CheckExist                   = MSB3400_Check_Exist,
    .Extension_Function           = MSB3400_Extension_Function,
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

