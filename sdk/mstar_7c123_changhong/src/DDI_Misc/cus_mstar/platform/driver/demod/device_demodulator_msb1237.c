#define _MSB1237_C_

/* ------------------------------------
    Header Files
   ------------------------------------ */
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#include <linux/fs.h>
#include <linux/vmalloc.h>
#include <linux/err.h>
#include <asm/uaccess.h>
#else
#include <stdio.h>
#include <string.h>
#endif
#include "MsCommon.h"
#include "MsOS.h"
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
#include "drvDMD_ATSC.h"


#if IF_THIS_DEMOD_INUSE(DEMOD_MSB1237)//1237????
#include "drvDMD_ATSC.h" //drvDMD_ATSC.h
#include "device_demodulator_msb1237.h"//1237.h???

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
MS_BOOL   MSB1237_IIC1_Write(MS_U16 u16BusNumSlaveID, MS_U8 u8addrcount, MS_U8* pu8addr, MS_U16 u16size, MS_U8* pu8data);
MS_BOOL   MSB1237_IIC1_Read(MS_U16 u16BusNumSlaveID, MS_U8 u8AddrNum, MS_U8* paddr, MS_U16 u16size, MS_U8* pu8data);


// Ex: ISDBT_TS_CLK_DIVNUM = 31 => TS Clk = 216/31= 6.967 MHz
#define ATSC_TS_CLK_DIVNUM_SERIAL_MODE   0//4
#define ATSC_TS_CLK_DIVNUM_PARALLEL_MODE  26//31

#define ATSC_TS_DATA_SWAP  0x00  // TS data inversion
#define ATSC_TS_CLK_INV      0x01  // TS clock inverseion          //this define will depend on hw main board


SLAVE_ID_USAGE* pstMSB1237_slave_ID_TBL = NULL;
static SLAVE_ID_USAGE MSB1237_possible_slave_ID[3] =
{
    {0xF2, FALSE},
    {0xD2, FALSE},
    {0xFF, FALSE}
};

//Local variables//
static MS_U8 u8DemodType = 0; // 0:VSB, 1:QAM256, 2:QAM64
static MS_S32* ps32MutexId = NULL;
static MS_BOOL* pbInited = NULL;
static DEMOD_MS_INIT_PARAM* pMSB1237_InitParam = NULL;
static MS_U8* pu8SlaveID = NULL;
static MS_U8 u8max_dev_num = 0;
static MS_U8 u8MSB1237_DevCnt = 0;

MS_U32 Scan_Time_Start;

/***********************************************************************************
************************************************************************************/
static MS_BOOL MSB1237_Variables_alloc(void)
{
    int i,j;
    SLAVE_ID_USAGE* pSlaveIDTBL = NULL;
    MS_U8 u8MaxI2CPort;

    u8MaxI2CPort = (MS_U8)((E_MS_IIC_SW_PORT_0/8) + (E_MS_IIC_PORT_NOSUP - E_MS_IIC_SW_PORT_0));

    if(NULL == pMSB1237_InitParam)
    {
        pMSB1237_InitParam = (DEMOD_MS_INIT_PARAM *)malloc(sizeof(DEMOD_MS_INIT_PARAM) * u8max_dev_num);
        if(NULL == pMSB1237_InitParam)
        {
            return FALSE;
        }
        else
        {
            memset(pMSB1237_InitParam, 0, sizeof(DEMOD_MS_INIT_PARAM) * u8max_dev_num);
        }
    }

    if(NULL == pbInited)
    {
        pbInited = (MS_BOOL *)malloc(sizeof(MS_BOOL) * u8max_dev_num);
        if(NULL == pbInited)
        {
            return FALSE;
        }
        else
        {
            memset(pbInited, 0, sizeof(MS_BOOL) * u8max_dev_num);
        }
    }

    if(NULL == pu8SlaveID)
    {
        pu8SlaveID = (MS_U8 *)malloc(sizeof(MS_U8) * u8max_dev_num);
        if(NULL == pu8SlaveID)
        {
            return FALSE;
        }
        else
        {
            memset(pu8SlaveID, 0, sizeof(MS_U8) * u8max_dev_num);
        }
    }

    if(NULL == ps32MutexId)
    {
        ps32MutexId = (MS_S32 *)malloc(sizeof(MS_S32) * u8max_dev_num);
        if(NULL == ps32MutexId)
        {
            return FALSE;
        }
        else
        {
            memset(ps32MutexId, -1, sizeof(MS_S32) * u8max_dev_num);
        }
    }

    if(NULL == pstMSB1237_slave_ID_TBL)
    {
        pstMSB1237_slave_ID_TBL = (SLAVE_ID_USAGE *)malloc(sizeof(MSB1237_possible_slave_ID) * u8MaxI2CPort);
        if(NULL == pstMSB1237_slave_ID_TBL)
        {
            return FALSE;
        }
        else
        {
            for(i=0; i< u8MaxI2CPort; i++)
            {
                for(j=0; j< (sizeof(MSB1237_possible_slave_ID)/sizeof(SLAVE_ID_USAGE)); j++)
                {
                    pSlaveIDTBL = (pstMSB1237_slave_ID_TBL + i*sizeof(MSB1237_possible_slave_ID)/sizeof(SLAVE_ID_USAGE) + j);
                    memcpy(pSlaveIDTBL, &MSB1237_possible_slave_ID[j], sizeof(SLAVE_ID_USAGE));
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

static MS_BOOL MSB1237_Variables_free(void)
{
    _variable_free((void*)&pMSB1237_InitParam);
    _variable_free((void*)&pbInited);
    _variable_free((void*)&pu8SlaveID);
    _variable_free((void*)&ps32MutexId);
    _variable_free((void*)&pstMSB1237_slave_ID_TBL);
    return TRUE;
}

static MS_BOOL get_init_param(MS_U8 u8DemodIndex, DEMOD_MS_INIT_PARAM** ppstInitParam)
{
    if(pMSB1237_InitParam == NULL)
        return FALSE;

    *ppstInitParam = pMSB1237_InitParam + u8DemodIndex;
    return TRUE;
}

static MS_BOOL mutex_lock(MS_U8 u8DemodIndex)
{
    MS_S32* ps32Mutex = NULL;

    if(ps32MutexId == NULL)
        return FALSE;

    ps32Mutex = ps32MutexId+u8DemodIndex;

    return MsOS_ObtainMutex(*ps32Mutex, COFDMDMD_MUTEX_TIMEOUT);
}

static MS_BOOL mutex_unlock(MS_U8 u8DemodIndex)
{
    MS_S32* ps32Mutex = NULL;

    if(ps32MutexId == NULL)
        return FALSE;

    ps32Mutex = ps32MutexId+u8DemodIndex;

    return MsOS_ReleaseMutex(*ps32Mutex);
}

static MS_BOOL get_i2c_port(MS_U8 u8DemodIndex, MS_IIC_PORT* pePort)
{
    DEMOD_MS_INIT_PARAM* pstInitParam;

    if(!get_init_param(u8DemodIndex, &pstInitParam))
        return FALSE;

    *pePort = pstInitParam->stDMDCon.eI2C_PORT;

    return TRUE;
}

static MS_U8 get_i2c_slaveID(MS_U8 u8DemodIndex)
{

    if(pu8SlaveID == NULL)
        return 0;

    return *(pu8SlaveID + u8DemodIndex);
}

static MS_BOOL set_i2c_slaveID(MS_U8 u8DemodIndex, MS_U8 u8SlaveID)
{

    if(pu8SlaveID == NULL)
        return FALSE;

    *(pu8SlaveID + u8DemodIndex) = u8SlaveID;
    return TRUE;
}

static MS_BOOL bIsInit(MS_U8 u8DemodIndex)
{
    MS_BOOL* pbIsInit = NULL;

    if(pbInited == NULL)
        return FALSE;

    pbIsInit = pbInited+u8DemodIndex;

    return *pbIsInit;
}

static MS_BOOL _Demod_Get_Lock(MS_U8 u8DemodIndex, DMD_ATSC_GETLOCK_TYPE eType, EN_LOCK_STATUS *peLockStatus)
{
    DMD_ATSC_LOCK_STATUS eLockStatus;
    static MS_U8 flag = 0;

    if (mutex_lock(u8DemodIndex) == FALSE)
    {
        DMD_DBG(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    if(FALSE == bIsInit(u8DemodIndex))
    {
        DMD_DBG(("[%s]MSB1237 have not inited !!!\n",__FUNCTION__));
        mutex_unlock(u8DemodIndex);
        return FALSE;
    }

    eLockStatus= MDrv_DMD_ATSC_GetLock(eType);
    if (eLockStatus == (DMD_ATSC_LOCK_STATUS)E_DEMOD_LOCK)
    {
        if (!flag)
        {
             flag = 1;
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
    mutex_unlock(u8DemodIndex);
    return TRUE;
}

// Global function//
MS_BOOL MSB1237_Demod_Init(MS_U8 u8DemodIndex,DEMOD_MS_INIT_PARAM* pParam)
{
    DMD_ATSC_InitData sDMD_ATSC_InitData;
    static MS_U8 u8DMD_ATSC_InitExt[]={1};
    MS_BOOL ret = TRUE;
    MS_IIC_PORT i2c_port;
    DEMOD_MS_INIT_PARAM* pstInitParam = NULL;
    MS_BOOL* pbInitFlag = NULL;
    MS_S32* ps32Mutex = NULL;

    //bInited = FALSE;
    DMD_DBG(("-----%s start----- 111 \n", __FUNCTION__));

    if(bIsInit(u8DemodIndex))
    {
        DMD_DBG(("[MSB1237]Have been inited\n"));
        return TRUE;
    }

    if(!MSB1237_Variables_alloc())
        return FALSE;

    pbInitFlag = pbInited + u8DemodIndex;
    ps32Mutex = ps32MutexId + u8DemodIndex;
    get_init_param(u8DemodIndex, &pstInitParam);

    *ps32Mutex = MsOS_CreateMutex(E_MSOS_FIFO, "OfDmd_Mutex", MSOS_PROCESS_SHARED);

    if (*ps32Mutex < 0)
    {
        DMD_ERR(("-----%s _s32MutexId < 0----- \n", __FUNCTION__));
        return FALSE;
    }

    MDrv_DMD_PreInit();

    memcpy(pstInitParam, pParam,sizeof(DEMOD_MS_INIT_PARAM));
    // copy tuner config
    if(pstInitParam->pstTunertab == NULL)
        return FALSE;

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
    if(pstInitParam->stDMDCon.bTSIsParallel)
    {
        sDMD_ATSC_InitData.u5TsConfigByte_DivNum = ATSC_TS_CLK_DIVNUM_PARALLEL_MODE;
    }
    else
    {
        sDMD_ATSC_InitData.u5TsConfigByte_DivNum = ATSC_TS_CLK_DIVNUM_SERIAL_MODE;
    }

    sDMD_ATSC_InitData.u1TsConfigByte_ClockInv = ATSC_TS_CLK_INV;
    sDMD_ATSC_InitData.u1TsConfigByte_DataSwap = ATSC_TS_DATA_SWAP;
    sDMD_ATSC_InitData.u1TsConfigByte_SerialMode = !pstInitParam->stDMDCon.bTSIsParallel;
    if(pstInitParam->pstTunertab->data == TUNER_R836)
        sDMD_ATSC_InitData.bIQSwap = 1;
    else
        sDMD_ATSC_InitData.bIQSwap = 0;

    //I2C[begin]
    get_i2c_port(u8DemodIndex, &i2c_port);
    sDMD_ATSC_InitData.u8I2CSlaveAddr = get_i2c_slaveID(u8DemodIndex);
    sDMD_ATSC_InitData.u8I2CSlaveBus = 0;
    sDMD_ATSC_InitData.bIsExtDemod = TRUE;
    sDMD_ATSC_InitData.bIsQPad =  FALSE; //FALSE is Q PAD. TRUE is I PAD.
    sDMD_ATSC_InitData.bIsUseSspiLoadCode = FALSE;
    if (i2c_port < E_MS_IIC_PORT_1)
    {
        sDMD_ATSC_InitData.I2C_WriteBytes = MSB1237_IIC_Write;
        sDMD_ATSC_InitData.I2C_ReadBytes = MSB1237_IIC_Read;
    }
    else if (i2c_port < E_MS_IIC_PORT_2)
    {
        sDMD_ATSC_InitData.I2C_WriteBytes = MSB1237_IIC1_Write;
        sDMD_ATSC_InitData.I2C_ReadBytes = MSB1237_IIC1_Read;
    }
    else
    {
        DMD_ERR(("hwi2c_port number exceeds limitation\n"));
        return FALSE;
    }


    ret &= MDrv_DMD_ATSC_Initial_Hal_Interface();

    ret &= MDrv_DMD_ATSC_Init(&sDMD_ATSC_InitData, sizeof(sDMD_ATSC_InitData));

    if(pstInitParam->pstTunertab->data == TUNER_R836)
        ret &= MDrv_DMD_ATSC_MD_DoIQSwap(0, TRUE);
    else
        ret &= MDrv_DMD_ATSC_MD_DoIQSwap(0, FALSE);

    if(ret)
    {
       *pbInitFlag = TRUE;
    }
    return ret;
}

MS_BOOL MSB1237_Demod_Open(MS_U8 u8DemodIndex)
{
#if 0
    if (mutex_lock(u8DemodIndex) == FALSE)
    {
        DMD_DBG(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    else
    {
    }

    mutex_unlock(u8DemodIndex);
    #endif
    return TRUE;

}

MS_BOOL MSB1237_Demod_Close(MS_U8 u8DemodIndex)
{
    return TRUE;
}

MS_BOOL MSB1237_Demod_Reset(MS_U8 u8DemodIndex)
{
    if (mutex_lock(u8DemodIndex) == FALSE)
    {
        DMD_DBG(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    else
    {
        if(!MDrv_DMD_ATSC_SetReset())//(!MSB1237_SoftReset())
        {
            DMD_DBG(("%s: Soft reset failed.\n", __FUNCTION__));
            return FALSE;
        }
    }

    mutex_unlock(u8DemodIndex);
    return TRUE;

}

MS_BOOL MSB1237_Demod_TsOut(MS_U8 u8DemodIndex, MS_BOOL bEnable)
{
    if (mutex_lock(u8DemodIndex) == FALSE)
    {
        DMD_DBG(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    else
    {
    }

    mutex_unlock(u8DemodIndex);
    return TRUE;
}

MS_BOOL MSB1237_Demod_PowerOnOff(MS_U8 u8DemodIndex, MS_BOOL bPowerOn)
{
    if (mutex_lock(u8DemodIndex) == FALSE)
    {
        DMD_DBG(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    else
    {
    }

    mutex_unlock(u8DemodIndex);
    return TRUE;
}

MS_BOOL MSB1237_Demod_SetBW(MS_U8 u8DemodIndex, MS_U32 u32BW)
{
    if (mutex_lock(u8DemodIndex) == FALSE)
    {
        DMD_DBG(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    else
    {
    }

    mutex_unlock(u8DemodIndex);
    return TRUE;

}

MS_BOOL MSB1237_Demod_GetBW(MS_U8 u8DemodIndex, MS_U32 *pu32BW)
{
    if (mutex_lock(u8DemodIndex) == FALSE)
    {
        DMD_DBG(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    else
    {
    }

    mutex_unlock(u8DemodIndex);
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
    if (mutex_lock(u8DemodIndex) == FALSE)
    {
        DMD_DBG(("%s: Obtain mutex failed.\n", __FUNCTION__));
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

    mutex_unlock(u8DemodIndex);
    return TRUE ;

}

MS_BOOL MSB1237_Demod_GetSNR(MS_U8 u8DemodIndex, float *pfSNR)
{

    if (mutex_lock(u8DemodIndex) == FALSE)
    {
        DMD_DBG(("%s: Obtain mutex failed.\n", __FUNCTION__));
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

    mutex_unlock(u8DemodIndex);
    return TRUE ;

}

MS_BOOL MSB1237_Demod_GetBER(MS_U8 u8DemodIndex, float *pfBER)
{
    MS_BOOL  bret;

    if (mutex_lock(u8DemodIndex) == FALSE)
    {
        DMD_DBG(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    else
    {
        *pfBER = -1;
    }

    bret = MDrv_DMD_ATSC_GetPostViterbiBer(pfBER);
    mutex_unlock(u8DemodIndex);
    return bret;
}

MS_BOOL MSB1237_Demod_GetPWR(MS_U8 u8DemodIndex, MS_S32 *ps32Signal)
{
    DEMOD_MS_INIT_PARAM* pstInitParam = NULL;

    if(!get_init_param(u8DemodIndex, &pstInitParam))
        return FALSE;

    if (mutex_lock(u8DemodIndex) == FALSE)
    {
        DMD_DBG(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    else
    {
        MSB1237_Demod_I2C_ByPass(u8DemodIndex, TRUE);
        if (TRUE != pstInitParam->pstTunertab->Extension_Function(u8DemodIndex,TUNER_EXT_FUNC_GET_POWER_LEVEL, ps32Signal))
        {
            MSB1237_Demod_I2C_ByPass(u8DemodIndex, FALSE);
            return FALSE;
        }
        MSB1237_Demod_I2C_ByPass(u8DemodIndex, FALSE);
    }

    mutex_unlock(u8DemodIndex);
    return TRUE;
}

MS_BOOL MSB1237_Demod_GetSSI(MS_U8 u8DemodIndex, MS_U16 *pu16SSI)
{
    MS_BOOL bRet = TRUE;

    if (mutex_lock(u8DemodIndex) == FALSE)
    {
        DMD_DBG(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    else
    {
         bRet = MDrv_DMD_ATSC_GetSignalStrength(pu16SSI);
    }

    mutex_unlock(u8DemodIndex);
    return bRet;
}

MS_BOOL MSB1237_Demod_Config(MS_U8 u8DemodIndex, MS_U8 *pRegParam)
{
    if (mutex_lock(u8DemodIndex) == FALSE)
    {
        DMD_DBG(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    else
    {

    }
    mutex_unlock(u8DemodIndex);
    return TRUE;

}

MS_BOOL MSB1237_Demod_GetParam(MS_U8 u8DemodIndex, DEMOD_MS_FE_CARRIER_PARAM* pParam)
{
    if (mutex_lock(u8DemodIndex) == FALSE)
    {
        DMD_DBG(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    else
    {
    }
    mutex_unlock(u8DemodIndex);
    return TRUE;

}

MS_BOOL MSB1237_Demod_I2C_ByPass(MS_U8 u8DemodIndex, MS_BOOL bEnable)
{
    MS_BOOL bret = TRUE;
    if(bIsInit(u8DemodIndex))
    {
        if(bEnable)
            return MDrv_DMD_ATSC_IIC_BYPASS_MODE(TRUE);
        else
            return MDrv_DMD_ATSC_IIC_BYPASS_MODE(FALSE);
    }
    else
    {
         bret &= MSB1237_I2C_CH_Reset(u8DemodIndex, 3,TRUE);
         if(bEnable)
             bret &= MSB1237_I2C_WriteByte(u8DemodIndex, get_i2c_slaveID(u8DemodIndex),0x0910, 0x10);
         else
             bret &= MSB1237_I2C_WriteByte(u8DemodIndex, get_i2c_slaveID(u8DemodIndex),0x0910, 0x00);
         return bret;
    }
}


MS_BOOL MSB1237_Demod_Restart(MS_U8 u8DemodIndex, DEMOD_MS_FE_CARRIER_PARAM* pParam,MS_U32 u32BroadCastType)
{
    MS_BOOL bret = FALSE;

    if (mutex_lock(u8DemodIndex) == FALSE)
    {
        DMD_DBG(("MDrv_COFDMDMD_Restart: Obtain mutex failed.\n"));
        return FALSE;
    }
    else
    {
        //MSB1237_Demod_I2C_ByPass(0, TRUE);
        //NoError = MDrv_Tuner_SetTuner(0, pParam->u32Frequency, pParam->TerParam.eBandWidth);
        //MSB1237_Demod_I2C_ByPass(0, FALSE);
        //MsOS_DelayTask(250); //necessary for NMI120 get RSSI
        if(!MDrv_DMD_ATSC_SetReset())//(!MSB1237_SoftReset())
        {
          DMD_ERR(("-----%s FAIL-----\n", __FUNCTION__));
          mutex_unlock(u8DemodIndex);
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
               bret = MDrv_DMD_ATSC_MD_SetConfig(0, DMD_ATSC_DEMOD_ATSC_VSB, TRUE);//MSB1237_VsbMode();
               break;
           case 1:
               bret = MDrv_DMD_ATSC_MD_SetConfig(0, DMD_ATSC_DEMOD_ATSC_256QAM, TRUE);//MSB1237_256QamMode();
               break;
           case 2:
               bret = MDrv_DMD_ATSC_MD_SetConfig(0, DMD_ATSC_DEMOD_ATSC_64QAM, TRUE);//MSB1237_64QamMode();
               break;
           default:
               DMD_DBG(("Unknown Demod type\n"));
               break;
        }
    }
    mutex_unlock(u8DemodIndex);
    //return (NoError) ? TRUE : FALSE;
    return bret;
}

#if 0
MS_BOOL MSB1237_Demod_Restart_AT(DEMOD_MS_FE_CARRIER_PARAM* pParam)
{
    MS_U32  dwError = 0;

    switch(pParam->TerParam.eHierarchy)
    {
#if(0)
    case 9:  //64QAM
        u8DemodType = 2;
        break;
    case 10: //256QAM
        u8DemodType = 1;
        break;
    case 6:  //VSB
#endif
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
               MDrv_DMD_ATSC_SetVsbMode();//MSB1237_VsbMode();
               break;
           case 1:
               MDrv_DMD_ATSC_Set256QamMode();//MSB1237_256QamMode();
               break;
           case 2:
               MDrv_DMD_ATSC_Set64QamMode();//MSB1237_64QamMode();
               break;
           default:
               DBG_MSB(printf("Unknown Demod type\n"));
               break;
       }

    }
    HB_ReleaseMutex(_s32MutexId);
    return (dwError) ? FALSE : TRUE;
}
#endif

MS_BOOL MSB1237_Demod_SetMode(MS_U8 u8DemodIndex, Demod_Mode* pMode)
{
    if(mutex_lock(u8DemodIndex) == FALSE)
    {
        DMD_DBG(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    else
    {
    }
    mutex_unlock(u8DemodIndex);
    return TRUE;

}

MS_BOOL MSB1237_Demod_SetOutoutPath(MS_U8 u8DemodIndex, DEMOD_INTERFACE_MODE path)
{
    //Dummy function
    return TRUE;
}


MS_BOOL MSB1237_IIC_Write(MS_U16 u16BusNumSlaveID, MS_U8 u8addrcount, MS_U8* pu8addr, MS_U16 u16size, MS_U8* pu8data)
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

MS_BOOL MSB1237_IIC1_Write(MS_U16 u16BusNumSlaveID, MS_U8 u8addrcount, MS_U8* pu8addr, MS_U16 u16size, MS_U8* pu8data)
{
    MS_BOOL bResult = TRUE;
    //printf("Enter MDrv_MSB140X_IIC_Write\n");

    bResult = MDrv_IIC1_Write(u16BusNumSlaveID, pu8addr, u8addrcount, pu8data,  u16size);
    return bResult;
}

MS_BOOL   MSB1237_IIC1_Read(MS_U16 u16BusNumSlaveID, MS_U8 u8AddrNum, MS_U8* paddr, MS_U16 u16size, MS_U8* pu8data)
{
    MS_BOOL bResult = TRUE;
    //printf("Enter MDrv_MSB140X_IIC_Read\n");

    bResult = MDrv_IIC1_Read(u16BusNumSlaveID, paddr, u8AddrNum, pu8data,  u16size);
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
MS_U8 MSB1237_I2C_ReadByte(MS_U8 u8DemodIndex, MS_U16 u16SlaveAddr, MS_U32 u32Addr, MS_U8 *pu8Data)
{

    MS_BOOL bRet=TRUE;
    MS_U8 u8MsbData[6] = {0};
    MS_IIC_PORT ePort;

    get_i2c_port(u8DemodIndex, &ePort);

    u8MsbData[0] = 0x10;
    u8MsbData[1] = 0x00;
    u8MsbData[2] = 0x00;
    u8MsbData[3] = (u32Addr >> 8) &0xff;
    u8MsbData[4] = u32Addr &0xff;

    u8MsbData[0] = 0x35;
    bRet &= MDrv_IIC_WriteBytes(ePort,u16SlaveAddr, 0, 0, 1, u8MsbData);

    u8MsbData[0] = 0x10;
    bRet &= MDrv_IIC_WriteBytes(ePort,u16SlaveAddr, 0, 0, 5, u8MsbData);
    bRet &= MDrv_IIC_ReadBytes(ePort,u16SlaveAddr, 0, 0, 1, pu8Data);

    u8MsbData[0] = 0x34;
    bRet &= MDrv_IIC_WriteBytes(ePort,u16SlaveAddr, 0, 0, 1, u8MsbData);

    return bRet;
}

MS_U8 MSB1237_I2C_WriteByte(MS_U8 u8DemodIndex, MS_U16 u16SlaveAddr, MS_U32 u32Addr, MS_U8 u8Data)
{
    MS_BOOL bRet=TRUE;
    MS_U8 u8MsbData[6] = {0};
    MS_IIC_PORT ePort;

    get_i2c_port(u8DemodIndex, &ePort);
    u8MsbData[0] = 0x10;
    u8MsbData[1] = 0x00;
    u8MsbData[2] = 0x00;
    u8MsbData[3] = (u32Addr >> 8) &0xff;
    u8MsbData[4] = u32Addr &0xff;
    u8MsbData[5] = u8Data;

    u8MsbData[0] = 0x35;
    bRet &= MDrv_IIC_WriteBytes(ePort,u16SlaveAddr, 0, 0, 1, u8MsbData);
    u8MsbData[0] = 0x10;
    bRet &= MDrv_IIC_WriteBytes(ePort,u16SlaveAddr, 0, 0, 6, u8MsbData);
    u8MsbData[0] = 0x34;
    bRet &= MDrv_IIC_WriteBytes(ePort,u16SlaveAddr, 0, 0, 1, u8MsbData);

    return bRet;
}


MS_BOOL MSB1237_I2C_CH_Reset(MS_U8 u8DemodIndex,MS_U8 ch_num,MS_BOOL enable)
{
    MS_BOOL bRet  =TRUE;
    MS_U8         data[5] = {0x53, 0x45, 0x52, 0x44, 0x42};
    MS_IIC_PORT ePort;

    get_i2c_port(u8DemodIndex, &ePort);

    if(enable)        //in 7853,always execute this
    {
        data[0] = 0x53;
        bRet &=MDrv_IIC_WriteBytes(ePort,get_i2c_slaveID(u8DemodIndex), 0, NULL, 5, data);
    }
    data[0] = 0x71;
    bRet &=MDrv_IIC_WriteBytes(ePort,get_i2c_slaveID(u8DemodIndex), 0, 0, 1, data);
    data[0] = (ch_num & 0x01)? 0x81 : 0x80;
    bRet &=MDrv_IIC_WriteBytes(ePort,get_i2c_slaveID(u8DemodIndex), 0, 0, 1, data);
    data[0] = (ch_num & 0x02)? 0x83 : 0x82;
    bRet &=MDrv_IIC_WriteBytes(ePort,get_i2c_slaveID(u8DemodIndex), 0, 0, 1, data);
    data[0] = (ch_num & 0x04)? 0x85 : 0x84;
    bRet &=MDrv_IIC_WriteBytes(ePort,get_i2c_slaveID(u8DemodIndex), 0, 0, 1, data);
    data[0] = 0x53;
    bRet &=MDrv_IIC_WriteBytes(ePort,get_i2c_slaveID(u8DemodIndex), 0, 0, 1, data);
    data[0] = 0x7f;
    bRet &=MDrv_IIC_WriteBytes(ePort,get_i2c_slaveID(u8DemodIndex), 0, 0, 1, data);

    return bRet;

}


#define MSB1237_CHIP_ID 0x77
MS_BOOL MSB1237_Check_Exist(MS_U8 u8DemodIndex, MS_U8* pu8SlaveID)
{
    MS_U8 u8_tmp = 0;
    MS_U8 i, u8I2C_Port = 0;
    SLAVE_ID_USAGE *pMSB1237_ID_TBL;
    MS_IIC_PORT ePort = 0;

    if(!MSB1237_Variables_alloc())
    {
        MSB1237_Variables_free();
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
        pMSB1237_ID_TBL = pstMSB1237_slave_ID_TBL + u8I2C_Port*sizeof(MSB1237_possible_slave_ID)/sizeof(SLAVE_ID_USAGE) + i;
        DMD_DBG(("### %x\n",i));
        if(pMSB1237_ID_TBL->u8SlaveID == 0xFF)
        {
            DMD_DBG(("[MSB1237] All Slave ID have tried but not detect\n"));
            break;
        }

        if(pMSB1237_ID_TBL->bInUse)
        {
            DMD_DBG(("[MSB1237] Slave ID 0x%x have been used\n", pMSB1237_ID_TBL->u8SlaveID));
            i++;
            continue;
        }
        else
        {
            set_i2c_slaveID(u8DemodIndex,pMSB1237_ID_TBL->u8SlaveID);
            DMD_DBG(("[MSB1237] Try slave ID 0x%x\n",pMSB1237_ID_TBL->u8SlaveID));
        }

        if(!MSB1237_I2C_CH_Reset(u8DemodIndex, 3,TRUE))
        {
            DMD_DBG(("[MSB1237] I2C_CH_Reset fail \n"));
        }

        if (!MSB1237_I2C_ReadByte(u8DemodIndex, get_i2c_slaveID(u8DemodIndex),0x0900, &u8_tmp))
        {
            DMD_DBG(("[MSB1237] Read  Chip ID fail \n"));
        }
        else
        {
            DMD_DBG(("[MSB1237] read id :%x \n",u8_tmp ));
            if(u8_tmp == MSB1237_CHIP_ID)
            {
                pMSB1237_ID_TBL->bInUse = TRUE;
                break;
            }
        }

        i++;
    }while((pMSB1237_ID_TBL->u8SlaveID) != 0xFF);


    if(u8_tmp != MSB1237_CHIP_ID)
    {
        if(!u8MSB1237_DevCnt)
            MSB1237_Variables_free();

        return FALSE;
    }
    else
    {
        return TRUE;
    }

}

//=======================================================================================
MS_BOOL MSB1237_Extension_Function(MS_U8 u8DemodIndex, DEMOD_EXT_FUNCTION_TYPE fuction_type, void *data)
{
    MS_BOOL bret = TRUE;
    DEMOD_EXT_FUNCTION_PARAM* pstData;
    DEMOD_CON_CONFIG* pstCon;
    DEMOD_MS_INIT_PARAM* pstInitParam;
    MS_S32* ps32Mutex= NULL;

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
            if((ps32MutexId == NULL) || (pbInited == NULL))
                return FALSE;

            ps32Mutex = (ps32MutexId+u8DemodIndex);

            if (*ps32Mutex >= 0)
            {
               bret &= MsOS_DeleteMutex(*ps32Mutex);
               *ps32Mutex = -1;
            }
            *(pbInited +  u8DemodIndex) = FALSE;
            bret &= MDrv_DMD_ATSC_Exit();
            break;

        case DEMOD_EXT_FUNC_SET_CON_INFO:
            pstData = (DEMOD_EXT_FUNCTION_PARAM*)data;
            u8max_dev_num = pstData->u32Param1;
            pstCon = (DEMOD_CON_CONFIG*)(pstData->pParam);
            if(!MSB1237_Variables_alloc())
            {
                MSB1237_Variables_free();
                return FALSE;
            }
            else
            {
                if(!get_init_param(u8DemodIndex, &pstInitParam))
                    return FALSE;

                memcpy(&pstInitParam->stDMDCon, pstCon,sizeof(DEMOD_CON_CONFIG));
            }

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
     .SupportINT                   = FALSE,
     .init                         = MSB1237_Demod_Init,
     .GetLock                      = MSB1237_Demod_GetLock,
     .GetSNR                       = MSB1237_Demod_GetSNR,
     .GetBER                       = MSB1237_Demod_GetBER,
     .GetPWR                       = MSB1237_Demod_GetPWR,
     .GetSSI                       = MSB1237_Demod_GetSSI,
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
     .DiSEqC_SendCmd               = MDrv_Demod_null_DiSEqC_SendCmd,
     .DiSEqC_GetReply              = MDrv_Demod_null_DiSEqC_GetReply,
     .GetISIDInfo                  = MDrv_Demod_null_GetVCM_ISID_INFO,
     .SetISID                      = MDrv_Demod_null_SetVCM_ISID
#endif
};

/***********************************************************************************
************************************************************************************/
#undef _MSB1237_C_

#endif
