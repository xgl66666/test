////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2007 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (¡§MStar Confidential Information¡¨) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
///
/// file    drvTuner_TDA18250A.c
/// @author MStar Semiconductor Inc.
//////////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include "MsCommon.h"
#include "Board.h"

#include "drvIIC.h"
#include "drvTuner.h"
#include "drvTunerNull.h"

#if IF_THIS_TUNER_INUSE(TUNER_TDA18260)
#include "tmNxTypes.h"
#include "tmCompId.h"
#include "tmFrontEnd.h"
#include "tmbslFrontEndTypes.h"
#include "tmbslTDA18260.h"



//*--------------------------------------------------------------------------------------
//* Include Driver files
//*--------------------------------------------------------------------------------------

#define TDA18260_ADDR             0xC0


//*--------------------------------------------------------------------------------------
//* Prototype of function to be provided by customer
//*--------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------

#define IIC_WRITE                   MDrv_IIC_Write
#define IIC_READ                    MDrv_IIC_Read
#define IIC1_WRITE                  MDrv_IIC1_Write
#define IIC1_READ                   MDrv_IIC1_Read



//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
static MS_U8 DrvInstance[MAX_FRONTEND_NUM*2] = {0xff};
static MS_U8 DrvInstance_acc = 0;

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
static tmErrorCode_t     UserWrittenI2CRead(tmUnitSelect_t tUnit,UInt32 AddrSize, UInt8* pAddr,UInt32 ReadLen, UInt8* pData);
static tmErrorCode_t     UserWrittenI2CWrite (tmUnitSelect_t tUnit, UInt32 AddrSize, UInt8* pAddr,UInt32 WriteLen, UInt8* pData);
static tmErrorCode_t     UserWrittenI2C1Read(tmUnitSelect_t tUnit,UInt32 AddrSize, UInt8* pAddr,UInt32 ReadLen, UInt8* pData);
static tmErrorCode_t     UserWrittenI2C1Write (tmUnitSelect_t tUnit, UInt32 AddrSize, UInt8* pAddr,UInt32 WriteLen, UInt8* pData);
static tmErrorCode_t     UserWrittenWait(tmUnitSelect_t tUnit, UInt32 tms);
static tmErrorCode_t     UserWrittenPrint(UInt32 level, const char* format, ...);
static tmErrorCode_t      UserWrittenMutexInit(ptmbslFrontEndMutexHandle *ppMutexHandle);
static tmErrorCode_t      UserWrittenMutexDeInit( ptmbslFrontEndMutexHandle pMutex);
static tmErrorCode_t      UserWrittenMutexAcquire(ptmbslFrontEndMutexHandle pMutex, UInt32 timeOut);
static tmErrorCode_t      UserWrittenMutexRelease(ptmbslFrontEndMutexHandle pMutex);

static MS_U8 _get_Tuner_Instance_Index(MS_U8 u8TunerIndex)
{
    return DrvInstance[u8TunerIndex];
}

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_Tuner_TDA18260_SetFreq(MS_U8 u8TunerIndex, MS_U32 u32Freq)
{
    tmTDA18260_standardMode_t TDA18260StdMode = tmTDA18260_DIG_8MHz;
    tmErrorCode_t err = TM_OK;
    /* TDA18260 Master standard mode */
    err = tmbslTDA18260_SetStandardMode(_get_Tuner_Instance_Index(u8TunerIndex), TDA18260StdMode);
    if(err != TM_OK)
    {
        TUNER_ERR(("[%s][%d] Err %ld \n",__FUNCTION__,__LINE__, err));
        return FALSE;
    }

    /* TDA18260 Master RF frequency */
    err = tmbslTDA18260_SetRf(_get_Tuner_Instance_Index(u8TunerIndex), u32Freq*1000);
    if(err != TM_OK)
    {
        TUNER_ERR(("[%s][%d] Err %ld \n",__FUNCTION__,__LINE__, err));
        return FALSE;
    }

    return TRUE;
}


MS_BOOL MDrv_Tuner_TDA18260_SetTuner(MS_U8 u8TunerIndex, MS_U32 u32Freq, MS_U8 u8Bw)
{
    return MDrv_Tuner_TDA18260_SetFreq(u8TunerIndex,u32Freq);
}

MS_BOOL MDrv_Tuner_TDA18260_PowerOnOff(MS_U8 u8TunerIndex, MS_BOOL bPowerOn)
{
    tmErrorCode_t err = TM_OK;

    /* TDA18260 Master Hardware power state */
    if (bPowerOn == TRUE)
    {
        err = tmbslTDA18260_SetPowerState(_get_Tuner_Instance_Index(u8TunerIndex),tmTDA18260_PowerOn);
    }
    else
    {
        err = tmbslTDA18260_SetPowerState(_get_Tuner_Instance_Index(u8TunerIndex),tmTDA18260_PowerOff);
    }

    if (err != TM_OK)
    {
        TUNER_ERR(("[%s][%d] \n",__FUNCTION__,__LINE__));
        return FALSE;
    }

    return TRUE;
}

MS_BOOL MDrv_Tuner_TDA18260_GetLock(MS_U8 u8TunerIndex)
{
    tmbslFrontEndState_t PLLLockState = tmbslFrontEndStateUnknown;
    tmErrorCode_t err = TM_OK;
    /* Get TDA18260 Slave PLL Lock status */
    err = tmbslTDA18260_GetLockStatus(_get_Tuner_Instance_Index(u8TunerIndex), &PLLLockState);
    if (err != TM_OK)
    {
        TUNER_ERR(("[%s][%d] \n",__FUNCTION__,__LINE__));
        return FALSE;
    }

    if (PLLLockState == tmbslFrontEndStateLocked)
    {
        TUNER_ERR(("[%s][%d] \n",__FUNCTION__,__LINE__));
        return TRUE;
    }

    return FALSE;
}


MS_BOOL MDrv_Tuner_TDA18260_Init(MS_U8 u8TunerIndex,TUNER_MS_INIT_PARAM* pParam)
{
    tmbslFrontEndDependency_t sSrvTunerFunc;
    tmTDA18260_Stream_t eStreamType;
    //tmbslFrontEndDependency_t  sSrvFunc;
    tmErrorCode_t err = TM_OK;
    HWI2C_PORT ei2cport = E_HWI2C_PORT_NOSUP;
    MS_U8 u8I2CPortIndex = 0;

    u8I2CPortIndex = u8TunerIndex/2;
    ei2cport = getI2CPort(u8I2CPortIndex);

    if (ei2cport >= E_HWI2C_PORT_1)
    {
        sSrvTunerFunc.sIo.Write = UserWrittenI2C1Write;
        sSrvTunerFunc.sIo.Read = UserWrittenI2C1Read;
    }
    else
    {
        sSrvTunerFunc.sIo.Write = UserWrittenI2CWrite;
        sSrvTunerFunc.sIo.Read = UserWrittenI2CRead;
    }
    sSrvTunerFunc.sTime.Get = Null;
    sSrvTunerFunc.sTime.Wait = UserWrittenWait;
    sSrvTunerFunc.sDebug.Print = UserWrittenPrint;
    sSrvTunerFunc.sMutex.Init = UserWrittenMutexInit;
    sSrvTunerFunc.sMutex.DeInit = UserWrittenMutexDeInit;
    sSrvTunerFunc.sMutex.Acquire = UserWrittenMutexAcquire;
    sSrvTunerFunc.sMutex.Release = UserWrittenMutexRelease;
    sSrvTunerFunc.dwAdditionalDataSize = 0;
    sSrvTunerFunc.pAdditionalData = Null;
    if (u8TunerIndex & 0x01)
    {
        eStreamType = tmTDA18260_Stream_B;
    }
    else
    {
        eStreamType = tmTDA18260_Stream_A;
    }
    sSrvTunerFunc.dwAdditionalDataSize =  sizeof( tmTDA18260_Stream_t); //// modification
    sSrvTunerFunc.pAdditionalData = &eStreamType; // modification
    //sSrvFunc.dwAdditionalDataSize = sizeof( tmTDA18260_Stream_t);
    //sSrvFunc.pAdditionalData = &eStreamType;
    /* TDA18260 Master Driver low layer setup */
    DrvInstance[u8TunerIndex] = DrvInstance_acc;
    err = tmbslTDA18260_Init(_get_Tuner_Instance_Index(u8TunerIndex), &sSrvTunerFunc);
    if(err != TM_OK)
    {
        TUNER_ERR(("[%s][%d] Err %ld \n",__FUNCTION__,__LINE__, err));
        return FALSE;
    }
    /* TDA18260 Master Hardware initialization */
    err = tmbslTDA18260_Reset(_get_Tuner_Instance_Index(u8TunerIndex));
    if (err != TM_OK)
    {
        TUNER_ERR(("[%s][%d] Err %ld \n",__FUNCTION__,__LINE__, err));
        return FALSE;
    }
    TUNER_DBG(("[%s][%d] Init Success \n",__FUNCTION__,__LINE__));
    DrvInstance_acc++;
    return TRUE;
}

/*
MS_U32 MDrv_Tuner_Loop_Through_On(MS_U8 u8TunerIndex, MS_BOOL bOn)
{
    TUNER_ERR(("[%s][%d] \n",__FUNCTION__,__LINE__);
    return TRUE;
}
*/

/*
MS_U32 MDrv_Tuner_Stand_By(MS_U8 u8TunerIndex)
{
    TUNER_ERR(("[%s][%d] \n",__FUNCTION__,__LINE__);
    return TRUE;
}
*/

/*
MS_U32 MDrv_Tuner_Wake_Up(MS_U8 u8TunerIndex)
{
    TUNER_ERR(("[%s][%d] \n",__FUNCTION__,__LINE__);
    return TRUE;
}
*/
//*--------------------------------------------------------------------------------------
//* Function Name       : UserWrittenI2CRead
//* Object              :
//* Input Parameters    :   tmUnitSelect_t tUnit
//*                         UInt32 AddrSize,
//*                         UInt8* pAddr,
//*                         UInt32 ReadLen,
//*                         UInt8* pData
//* Output Parameters   : tmErrorCode_t.
//*--------------------------------------------------------------------------------------

tmErrorCode_t UserWrittenI2C1Read(tmUnitSelect_t tUnit, UInt32 AddrSize, UInt8* pAddr,
UInt32 ReadLen, UInt8* pData)
{
    tmErrorCode_t err = TM_OK;

    if(IIC1_READ(TDA18260_ADDR, pAddr, (MS_U8)AddrSize,  pData, (MS_U16)ReadLen)!=TRUE)
    {
        err = TDA18260_ERR_HW_FAILED;
        TUNER_ERR(("[%s][%d] \n",__FUNCTION__,__LINE__));
    }
    return err;
}

//*--------------------------------------------------------------------------------------
//* Function Name       : UserWrittenI2CWrite
//* Object              :
//* Input Parameters    :   tmUnitSelect_t tUnit
//*                         UInt32 AddrSize,
//*                         UInt8* pAddr,
//*                         UInt32 WriteLen,
//*                         UInt8* pData
//* Output Parameters   : tmErrorCode_t.
//*--------------------------------------------------------------------------------------

tmErrorCode_t UserWrittenI2C1Write (tmUnitSelect_t tUnit,   UInt32 AddrSize, UInt8* pAddr,
UInt32 WriteLen, UInt8* pData)
{
    tmErrorCode_t err = TM_OK;

    if(IIC1_WRITE(TDA18260_ADDR, pAddr, (MS_U8)AddrSize, pData, (MS_U16)WriteLen)!=TRUE)
    {
        err = TDA18260_ERR_HW_FAILED;
        TUNER_ERR(("[%s][%d] \n",__FUNCTION__,__LINE__));
    }

    return err;
}

//*--------------------------------------------------------------------------------------
//* Function Name       : UserWrittenI2CRead
//* Object              :
//* Input Parameters    :     tmUnitSelect_t tUnit
//*                         UInt32 AddrSize,
//*                         UInt8* pAddr,
//*                         UInt32 ReadLen,
//*                         UInt8* pData
//* Output Parameters   : tmErrorCode_t.
//*--------------------------------------------------------------------------------------
tmErrorCode_t UserWrittenI2CRead(tmUnitSelect_t tUnit,    UInt32 AddrSize, UInt8* pAddr,
UInt32 ReadLen, UInt8* pData)
{
   /* Variable declarations */
    tmErrorCode_t err = TM_OK;

    if(IIC_READ(TDA18260_ADDR, pAddr, (MS_U8)AddrSize, pData, (MS_U16)ReadLen)!= TRUE)
    {
        err = TDA18260_ERR_HW_FAILED;
        TUNER_ERR(("[%s][%d] \n",__FUNCTION__,__LINE__));
    }

    return err;
}
//*--------------------------------------------------------------------------------------
//* Function Name       : UserWrittenI2CWrite
//* Object              :
//* Input Parameters    :     tmUnitSelect_t tUnit
//*                         UInt32 AddrSize,
//*                         UInt8* pAddr,
//*                         UInt32 WriteLen,
//*                         UInt8* pData
//* Output Parameters   : tmErrorCode_t.
//*--------------------------------------------------------------------------------------
tmErrorCode_t UserWrittenI2CWrite (tmUnitSelect_t tUnit,     UInt32 AddrSize, UInt8* pAddr,
UInt32 WriteLen, UInt8* pData)
{
   /* Variable declarations */
    tmErrorCode_t err = TM_OK;

    if(IIC_WRITE(TDA18260_ADDR, pAddr, (MS_U8)AddrSize, pData, (MS_U16)WriteLen)!= TRUE)
    {
        err = TDA18260_ERR_HW_FAILED;
        TUNER_ERR(("[%s][%d] \n",__FUNCTION__,__LINE__));
    }

    return err;
}

//*--------------------------------------------------------------------------------------
//* Function Name       : UserWrittenWait
//* Object              :
//* Input Parameters    :     tmUnitSelect_t tUnit
//*                         UInt32 tms
//* Output Parameters   : tmErrorCode_t.
//*--------------------------------------------------------------------------------------
tmErrorCode_t UserWrittenWait(tmUnitSelect_t tUnit, UInt32 tms)
{
    /* Variable declarations */
    tmErrorCode_t err = TM_OK;
    MsOS_DelayTask(tms);
    return err;
}

//*--------------------------------------------------------------------------------------
//* Function Name       : UserWrittenPrint
//* Object              :
//* Input Parameters    :     UInt32 level, const char* format, ...
//*
//* Output Parameters   : tmErrorCode_t.
//*--------------------------------------------------------------------------------------
tmErrorCode_t             UserWrittenPrint(UInt32 level, const char* format, ...)
{
    /* Variable declarations */
    tmErrorCode_t err = TM_OK;

    /* Customer code here */
    /* ...*/

    /* ...*/
    /* End of Customer code here */

    return err;
}

//*--------------------------------------------------------------------------------------
//* Function Name       : UserWrittenMutexInit
//* Object              :
//* Input Parameters    :     ptmbslFrontEndMutexHandle *ppMutexHandle
//* Output Parameters   : tmErrorCode_t.
//*--------------------------------------------------------------------------------------
tmErrorCode_t  UserWrittenMutexInit(ptmbslFrontEndMutexHandle *ppMutexHandle)
{
    /* Variable declarations */
    tmErrorCode_t err = TM_OK;

    /* Customer code here */
    /* ...*/

    /* ...*/
    /* End of Customer code here */

    return err;
}


//*--------------------------------------------------------------------------------------
//* Function Name       : UserWrittenMutexDeInit
//* Object              :
//* Input Parameters    :      ptmbslFrontEndMutexHandle pMutex
//* Output Parameters   : tmErrorCode_t.
//*--------------------------------------------------------------------------------------
tmErrorCode_t  UserWrittenMutexDeInit( ptmbslFrontEndMutexHandle pMutex)
{
    /* Variable declarations */
    tmErrorCode_t err = TM_OK;

    /* Customer code here */
    /* ...*/

    /* ...*/
    /* End of Customer code here */

    return err;
}



//*--------------------------------------------------------------------------------------
//* Function Name       : UserWrittenMutexAcquire
//* Object              :
//* Input Parameters    :     ptmbslFrontEndMutexHandle pMutex, UInt32 timeOut
//* Output Parameters   : tmErrorCode_t.
//*--------------------------------------------------------------------------------------
tmErrorCode_t  UserWrittenMutexAcquire(ptmbslFrontEndMutexHandle pMutex, UInt32 timeOut)
{
    /* Variable declarations */
    tmErrorCode_t err = TM_OK;

    /* Customer code here */
    /* ...*/

    /* ...*/
    /* End of Customer code here */

    return err;
}

//*--------------------------------------------------------------------------------------
//* Function Name       : UserWrittenMutexRelease
//* Object              :
//* Input Parameters    :     ptmbslFrontEndMutexHandle pMutex
//* Output Parameters   : tmErrorCode_t.
//*--------------------------------------------------------------------------------------
tmErrorCode_t  UserWrittenMutexRelease(ptmbslFrontEndMutexHandle pMutex)
{
    /* Variable declarations */
    tmErrorCode_t err = TM_OK;

    /* Customer code here */
    /* ...*/

    /* ...*/
    /* End of Customer code here */

    return err;
}

#define TDA18260hn_CHIP_ID_1 0x54
#define TDA18260hn_CHIP_ID_0 0xc7
MS_BOOL TDA18260HN_CheckExist(MS_U8 u8TunerIndex, MS_U32* pu32channel_cnt)
{
    MS_U8 pAddr[2] = {0x00,0x01};
	MS_U8 pData[2] ;

    switch(u8TunerIndex)
    {
        case 1:
            IIC1_READ(TDA18260_ADDR, pAddr, 2, pData,2);
            break;
        case 0:
        default:
            IIC_READ(TDA18260_ADDR, pAddr, 2, pData,2);
            break;
    }
    TUNER_DBG(("[18260hn] read id =0x%x 0x%x\n",pData[0],pData[1]));
    if((pData[0] == TDA18260hn_CHIP_ID_0) && (pData[1] == TDA18260hn_CHIP_ID_1) && (NULL != pu32channel_cnt))
        *(pu32channel_cnt) += 2;
    
    return (pData[0] == TDA18260hn_CHIP_ID_0) && (pData[1] == TDA18260hn_CHIP_ID_1) ;
}

MS_BOOL TUNER_TDA18260_Extension_Function(MS_U8 u8TunerIndex, TUNER_EXT_FUNCTION_TYPE fuction_type, void *data)
{
    switch(fuction_type)
    {
        case TUNER_EXT_FUNC_POWER_ON_OFF:
            return MDrv_Tuner_TDA18260_PowerOnOff(u8TunerIndex, *(MS_BOOL *)data);
            break;
        default:
            TUNER_DBG(("Request extension function (%x) does not exist\n",fuction_type));
            return TRUE;
    } 

}

MS_BOOL MDRV_Tuner_TDA18260_GetTunerIF(MS_U8 u8TunerIndex, MS_U32* pu32IF_Freq)
{
  tmErrorCode_t err = TM_OK;

  err = tmbslTDA18260_GetIF(_get_Tuner_Instance_Index(u8TunerIndex), (UInt32*)pu32IF_Freq);
  if(TM_OK == err)
  {
    *pu32IF_Freq /= 1000;
    return TRUE;
  }
  else
    return FALSE;
}

DRV_TUNER_TABLE_TYPE GET_TUNER_ENTRY_NODE(TUNER_TDA18260) DDI_DRV_TUNER_TABLE_ENTRY(tunertab) =      
{                                                                       
    .name               = "TUNER_TDA18260",          // demod system name
    .data               = TUNER_TDA18260,            // private data value
    .Init               = MDrv_Tuner_TDA18260_Init,
    .SetFreq            = MDrv_Tuner_TDA18260_SetFreq,
    .SetFreqS2          = MDrv_Tuner_Null_SetFreq_S2,
    .GetLock            = MDrv_Tuner_TDA18260_GetLock,
    .SetTuner           = MDrv_Tuner_TDA18260_SetTuner,
    .CheckExist         = TDA18260HN_CheckExist,
    .GetTunerIF         = MDRV_Tuner_TDA18260_GetTunerIF,
    .Extension_Function = TUNER_TDA18260_Extension_Function
};

#endif // TUNER_PHILIPS_TDA18250A

