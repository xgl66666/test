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

#if IF_THIS_TUNER_INUSE(TUNER_TDA18250A)||IF_THIS_TUNER_INUSE(TUNER_TDA18250B)

#include "tmNxTypes.h"
#include "tmCompId.h"
#include "tmFrontEnd.h"
#include "tmbslFrontEndTypes.h"


//*--------------------------------------------------------------------------------------
//* Include Driver files
//*--------------------------------------------------------------------------------------
#include "tmbslTDA18250A.h"
#include "tmbslTDA18250A_Advanced.h"
//#include "tmbslTDA18250A_Config.h"
//#include "tmbslTDA18250A_Local.h"

#define TDA18250A_ADDR             0xC0

#define usleep(x)                   MsOS_DelayTask(x/1000)


//*--------------------------------------------------------------------------------------
//* Prototype of function to be provided by customer
//*--------------------------------------------------------------------------------------
static tmErrorCode_t     UserWrittenI2CRead(tmUnitSelect_t tUnit,UInt32 AddrSize, UInt8* pAddr,UInt32 ReadLen, UInt8* pData);
static tmErrorCode_t     UserWrittenI2CWrite (tmUnitSelect_t tUnit, UInt32 AddrSize, UInt8* pAddr,UInt32 WriteLen, UInt8* pData);
static tmErrorCode_t     UserWrittenI2C1Read(tmUnitSelect_t tUnit,UInt32 AddrSize, UInt8* pAddr,UInt32 ReadLen, UInt8* pData);
static tmErrorCode_t     UserWrittenI2C1Write (tmUnitSelect_t tUnit, UInt32 AddrSize, UInt8* pAddr,UInt32 WriteLen, UInt8* pData);
static tmErrorCode_t     UserWrittenWait(tmUnitSelect_t tUnit, UInt32 tms);
static tmErrorCode_t     UserWrittenPrint(UInt32 level, const char* format, ...);
#if 0
static tmErrorCode_t     UserWrittenMutexInit(ptmbslFrontEndMutexHandle *ppMutexHandle);
static tmErrorCode_t     UserWrittenMutexDeInit( ptmbslFrontEndMutexHandle pMutex);
static tmErrorCode_t     UserWrittenMutexAcquire(ptmbslFrontEndMutexHandle pMutex, UInt32 timeOut);
static tmErrorCode_t     UserWrittenMutexRelease(ptmbslFrontEndMutexHandle pMutex);
#endif

//#define IIC_WRITE(u8SlaveId, pu8Addr, u8AddrSize, pu8Buf, u32BufSize)   MDrv_IIC_WriteBytes(u8SlaveId, u8AddrSize, pu8Addr, (MS_U16)u32BufSize, pu8Buf)
//#define IIC_READ(u8SlaveId, pu8Addr, u8AddrSize, pu8Buf, u32BufSize)   MDrv_IIC_ReadBytes(u8SlaveId, u8AddrSize, pu8Addr, (MS_U16)u32BufSize, pu8Buf)

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
/*
#if   (FRONTEND_TUNER_CTRL == PIU_IIC_0)
#define IIC_WRITE                   MDrv_IIC_Write
#define IIC_READ                    MDrv_IIC_Read
#elif (FRONTEND_TUNER_CTRL == PIU_IIC_1)
#define IIC_WRITE                   MDrv_IIC1_Write
#define IIC_READ                    MDrv_IIC1_Read
#elif (FRONTEND_TUNER_CTRL == PIU_IIC_NONE)
#define REG(addr)                   (*(volatile MS_U8*)(addr))
#else
#error "FRONTEND_TUNER_CTRL"
#endif
*/
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
/*MS_U8 dmdConfig[] =

{
    // SZ 5015 board TDA18250HN internal RFAGC
    // Rev.: [0x3f] 0x82, [0x20] 0x72, [0x29] 0xff, [0x2a] 0x7f, [0x30] 0xd3
    0x01, 0x08, 0x84, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x73, 0x00, 0xb3, 0x33, 0x03, 0x05, 0x52, 0x52, 0x73, 0x73, 0x73, 0x31, 0x52, 0x73, 0x73, 0x73,
    0x72, 0xc0, 0x24, 0x00, 0x80, 0xff, 0x7f, 0x00, 0x80, 0xff, 0x7f, 0x0e, 0x2b, 0x0b, 0x47, 0x40,
    0xd3, 0x9a, 0xde, 0x11, 0xa4, 0x60, 0x08, 0x08, 0x00, 0x10, 0x02, 0x0b, 0x03, 0x05, 0x03, 0x82,
    0x52, 0x52, 0x55,
};

MS_U8 dmdConfig[] =
{
      0x01, 0x22, 0x8d, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
      0x73, 0x00, 0xb3, 0x33, 0x03, 0x05, 0x52, 0x52, 0x73, 0x73, 0x73, 0x31, 0x52, 0x73, 0x73, 0x73,
      0x72, 0xc0, 0x24, 0x00, 0x80, 0xff, 0x7f, 0x00, 0x80, 0xff, 0x7f, 0x0e, 0x2b, 0x0b, 0x47, 0x40,
      0xd3, 0x9a, 0xde, 0x11, 0xa4, 0x60, 0x08, 0x08, 0x00, 0x10, 0x02, 0x0b, 0x03, 0x05, 0x03, 0x82,
      0x52, 0x52, 0x55,
};*/

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
static TUNER_MS_INIT_PARAM InitParam[MAX_FRONTEND_NUM];
static MS_U8 DrvInstance[MAX_FRONTEND_NUM] = {0xff};
static MS_U8 DrvInstance_acc = 0;

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
static MS_U8 _get_Tuner_Instance_Index(MS_U8 u8TunerIndex)
{
    return DrvInstance[u8TunerIndex];
}

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_Tuner_TDA18250A_SetFreq(MS_U8 u8TunerIndex, MS_U32 u32Freq)
{
    tmErrorCode_t err = TM_OK;
    tmbslFrontEndState_t PLLLockMaster = tmbslFrontEndStateUnknown;  
    UInt32 uIFMaster = 0;


    
    err = tmbslTDA18250A_SetRF(_get_Tuner_Instance_Index(u8TunerIndex),0, u32Freq*1000);
    if(err != TM_OK)
        return err;

    err = tmbslTDA18250A_GetPLLState(_get_Tuner_Instance_Index(u8TunerIndex),0, &PLLLockMaster);
    //TUNER_ERR(("===================== Lock status=%d, result[0x%X].\n", PLLLockMaster, err);
    if(err != TM_OK)
        return err;

    /* Get TDA18250A IF */
    err = tmbslTDA18250A_GetIF(_get_Tuner_Instance_Index(u8TunerIndex),0, &uIFMaster);
    //TUNER_ERR(("===================== uIFMaster=%d, result[0x%X].\n",(int)uIFMaster, err);


    if(err == TM_OK)
        return TRUE;
    else
        return FALSE;
}


MS_BOOL MDrv_Tuner_TDA18250A_SetTuner(MS_U8 u8TunerIndex, MS_U32 u32Freq, MS_U8 u8Bw)
{
    TDA18250AStandardMode_t standard_mode = TDA18250A_QAM_8MHz;
    tmErrorCode_t err;

    if((*InitParam[u8TunerIndex].pCur_Broadcast_type == DVBC) || (*InitParam[u8TunerIndex].pCur_Broadcast_type == J83B))
    {
        switch(u8Bw)
        {
            case TUNER_BW_MODE_6MHZ:
                TUNER_DBG(("=== TDA18250A_QAM_6MHz\n"));
                standard_mode = TDA18250A_QAM_6MHz;
                break;
            case TUNER_BW_MODE_7MHZ:
                TUNER_DBG(("=== TDA18250A_QAM_7MHz\n"));
                standard_mode = TDA18250A_QAM_7MHz;
                break;
           case TUNER_BW_MODE_8MHZ:
           default:
                TUNER_DBG(("=== TDA18250A_QAM_8MHz\n"));
                standard_mode = TDA18250A_QAM_8MHz;
                break;
       }
    }
    else if((*InitParam[u8TunerIndex].pCur_Broadcast_type == DVBT) || (*InitParam[u8TunerIndex].pCur_Broadcast_type == DVBT2))
    {
        switch(u8Bw)
        {
            case TUNER_BW_MODE_6MHZ:
                TUNER_DBG(("=== TDA18250A_DVBT_6MHz\n"));
                standard_mode = TDA18250A_DVBT_6MHz;
                break;
            case TUNER_BW_MODE_7MHZ:
                TUNER_DBG(("=== TDA18250A_DVBT_7MHz\n"));
                standard_mode = TDA18250A_DVBT_7MHz;
                break;
            case TUNER_BW_MODE_1_7MHZ:
                TUNER_DBG(("=== TDA18250A_DVBT_1_7MHz\n"));
                standard_mode = TDA18250A_DVBT_1_7MHz;
                break;     
           case TUNER_BW_MODE_8MHZ:
           default:
                TUNER_DBG(("=== TDA18250A_DVBT_8MHz\n"));
                standard_mode = TDA18250A_DVBT_8MHz;
                break;
       }
    }
    else if(*InitParam[u8TunerIndex].pCur_Broadcast_type == ISDBT)
    {
          standard_mode = TDA18250A_ISDBT_6MHz;
    }
    else if(*InitParam[u8TunerIndex].pCur_Broadcast_type == ATSC)
    {
          standard_mode = TDA18250A_ATSC_6MHz;
    }

    err = tmbslTDA18250A_SetStandardMode(_get_Tuner_Instance_Index(u8TunerIndex), 0, standard_mode);
    if(TM_OK != err)
        {
        TUNER_ERR(("!!!!! standard_mode error\n"));
        }
    
    return MDrv_Tuner_TDA18250A_SetFreq(u8TunerIndex,u32Freq);
}


MS_BOOL MDrv_Tuner_TDA18250A_PowerOnOff(MS_U8 u8TunerIndex, MS_BOOL bPowerOn)
{
    return TRUE;
}
MS_BOOL MDrv_Tuner_TDA18250B_Init(MS_U8 u8TunerIndex,TUNER_MS_INIT_PARAM* pParam)
{
    tmErrorCode_t               err = TM_OK;
    tmbslFrontEndDependency_t   sSrvTunerFunc;
    UInt32                      TunerUnit = 0;
    TDA18250AStandardMode_t     TDA18250AStdMode = TDA18250A_QAM_8MHz;
    //UInt32                      uRF = 0;
    //tmbslFrontEndState_t        PLLLock = tmbslFrontEndStateUnknown;
    HWI2C_PORT hwi2c_port;
    hwi2c_port = getI2CPort(u8TunerIndex);
    if (hwi2c_port < E_HWI2C_PORT_1)
    {
        sSrvTunerFunc.sIo.Write = UserWrittenI2CWrite;
        sSrvTunerFunc.sIo.Read = UserWrittenI2CRead;
    }
    else if (hwi2c_port < E_HWI2C_PORT_2)
    {
        sSrvTunerFunc.sIo.Write = UserWrittenI2C1Write;
        sSrvTunerFunc.sIo.Read = UserWrittenI2C1Read;
    }
    else
    {
        TUNER_ERR(("hwi2c_port number exceeds limitation\n"));
        return FALSE;
    }
    
    if(pParam->pCur_Broadcast_type == NULL)
        return FALSE;
    else
        InitParam[u8TunerIndex].pCur_Broadcast_type = pParam->pCur_Broadcast_type;

    DrvInstance[u8TunerIndex] = DrvInstance_acc;
     
    sSrvTunerFunc.sTime.Get             = Null;
    sSrvTunerFunc.sTime.Wait            = UserWrittenWait;
    sSrvTunerFunc.sDebug.Print          = UserWrittenPrint;
    sSrvTunerFunc.sMutex.Init           = Null;
    sSrvTunerFunc.sMutex.DeInit         = Null;
    sSrvTunerFunc.sMutex.Acquire        = Null;
    sSrvTunerFunc.sMutex.Release        = Null;
    sSrvTunerFunc.dwAdditionalDataSize  = 0;
    sSrvTunerFunc.pAdditionalData       = Null;

#ifdef DDI_MISC_INUSE    
  #ifndef TUNER_18250AB_XTAL_27MHZ
    tmbslTDA18250B_ResetConfig(_get_Tuner_Instance_Index(u8TunerIndex));
  #endif
#else
    // Reset the XTAL CLK to 18250B Only
    tmbslTDA18250B_ResetConfig(_get_Tuner_Instance_Index(u8TunerIndex));
#endif

    
    /* Open TDA18250A driver instance */
    err = tmbslTDA18250A_Open(_get_Tuner_Instance_Index(u8TunerIndex),TunerUnit, &sSrvTunerFunc);
    TUNER_DBG(("\n=== TDA18250B open, result[0x%lX]. \n", err));
    if(err == TM_OK)
    {
        /* TDA18250A Power On */
        //err = tmbslTDA18250A_SetPowerState(u8TunerIndex,TunerUnit, tmPowerOn);
        //TUNER_ERR(("\n=== TDA18250B SetPowerState, result[0x%lX]. \n", err);

        /* TDA18250A Hardware initialization */
        err = tmbslTDA18250A_HwInit(_get_Tuner_Instance_Index(u8TunerIndex),TunerUnit);
        TUNER_DBG(("\n=== TDA18250B HW init status, result[0x%lX]. \n", err));
        if(err != TM_OK)
        {
           usleep(20*1000);
           err = tmbslTDA18250A_HwInit(_get_Tuner_Instance_Index(u8TunerIndex),TunerUnit);
           TUNER_ERR(("\n=== TDA18250A HW init status, result[0x%lX]. \n", err));
           if(err != TM_OK)
           {
              usleep(20*1000);
              err = tmbslTDA18250A_HwInit(_get_Tuner_Instance_Index(u8TunerIndex),TunerUnit);
              TUNER_ERR(("\n=== TDA18250A HW init status, result[0x%lX]. \n", err));
            }
        }

        /* TDA18250A Power On */
        err = tmbslTDA18250A_SetPowerState(_get_Tuner_Instance_Index(u8TunerIndex),TunerUnit, tmPowerOn);
        TUNER_DBG(("\n=== TDA18250B Set2ndPowerState, result[0x%lX]. \n", err));

        if(err == TM_OK)
        {
          /* TDA18250A standard mode if changed */
          err = tmbslTDA18250A_SetStandardMode(_get_Tuner_Instance_Index(u8TunerIndex), TunerUnit, TDA18250AStdMode);
          TUNER_DBG(("\n=== TDA18250B init SetStandardMode status[0x%lX]. \n", err));
        }

    }
    else
    {
        TUNER_ERR(("\n=== TDA18250B open fail[%ld]. \n", err));
    }

    #if 0   //temp
    if(err == TM_OK)
    {
      /* Get TDA18250A PLL Lock status */
      err = tmbslTDA18250A_GetLockStatus(TunerUnit, &PLLLock);
    }
    #endif
    if(err == TM_OK)
    {
        DrvInstance_acc++;
        return TRUE;
    }
    else
    {
        TUNER_ERR(("\n=== TDA18250B init fail, status[%ld]. \n", err));
        return FALSE;
    }

}

MS_BOOL MDrv_Tuner_TDA18250A_Init(MS_U8 u8TunerIndex,TUNER_MS_INIT_PARAM* pParam)
{
    tmErrorCode_t               err = TM_OK;
    tmbslFrontEndDependency_t   sSrvTunerFunc;
    UInt32                      TunerUnit = 0;
    TDA18250AStandardMode_t     TDA18250AStdMode = TDA18250A_QAM_8MHz;
    //UInt32                      uRF = 0;
    //tmbslFrontEndState_t        PLLLock = tmbslFrontEndStateUnknown;
    HWI2C_PORT hwi2c_port;
    hwi2c_port = getI2CPort(u8TunerIndex);
    if (hwi2c_port < E_HWI2C_PORT_1)
    {
        sSrvTunerFunc.sIo.Write = UserWrittenI2CWrite;
        sSrvTunerFunc.sIo.Read = UserWrittenI2CRead;
    }
    else if (hwi2c_port < E_HWI2C_PORT_2)
    {
        sSrvTunerFunc.sIo.Write = UserWrittenI2C1Write;
        sSrvTunerFunc.sIo.Read = UserWrittenI2C1Read;
    }
    else
    {
        TUNER_ERR(("hwi2c_port number exceeds limitation\n"));
        return FALSE;
    }

    if(pParam->pCur_Broadcast_type == NULL)
        return FALSE;
    else
        InitParam[u8TunerIndex].pCur_Broadcast_type = pParam->pCur_Broadcast_type;


    DrvInstance[u8TunerIndex] = DrvInstance_acc;
    
    sSrvTunerFunc.sTime.Get             = Null;
    sSrvTunerFunc.sTime.Wait            = UserWrittenWait;
    sSrvTunerFunc.sDebug.Print          = UserWrittenPrint;
    sSrvTunerFunc.sMutex.Init           = Null;
    sSrvTunerFunc.sMutex.DeInit         = Null;
    sSrvTunerFunc.sMutex.Acquire        = Null;
    sSrvTunerFunc.sMutex.Release        = Null;
    sSrvTunerFunc.dwAdditionalDataSize  = 0;
    sSrvTunerFunc.pAdditionalData       = Null;

#ifdef DDI_MISC_INUSE    
  #ifndef TUNER_18250AB_XTAL_27MHZ 
    // default 27M
    tmbslTDA18250B_ResetConfig(_get_Tuner_Instance_Index(u8TunerIndex));
  #endif
#endif
    /* Open TDA18250A driver instance */
    err = tmbslTDA18250A_Open(_get_Tuner_Instance_Index(u8TunerIndex),TunerUnit, &sSrvTunerFunc);
    TUNER_DBG(("\n=== TDA18250A open, result[0x%lX]. \n", err));
    if(err == TM_OK)
    {
        /* TDA18250A Power On */
        //err = tmbslTDA18250A_SetPowerState(u8TunerIndex,TunerUnit, tmPowerOn);
        //TUNER_ERR(("\n=== TDA18250A SetPowerState, result[0x%lX]. \n", err);

        /* TDA18250A Hardware initialization */
        err = tmbslTDA18250A_HwInit(_get_Tuner_Instance_Index(u8TunerIndex),TunerUnit);
        TUNER_DBG(("\n=== TDA18250A HW init status, result[0x%lX]. \n", err));
        if(err != TM_OK)
        {
           usleep(20*1000);
           err = tmbslTDA18250A_HwInit(_get_Tuner_Instance_Index(u8TunerIndex),TunerUnit);
           TUNER_ERR(("\n=== TDA18250A HW init status, result[0x%lX]. \n", err));
           if(err != TM_OK)
           {
              usleep(20*1000);
              err = tmbslTDA18250A_HwInit(_get_Tuner_Instance_Index(u8TunerIndex),TunerUnit);
              TUNER_ERR(("\n=== TDA18250A HW init status, result[0x%lX]. \n", err));
            }
        }

        /* TDA18250A Power On */
        err = tmbslTDA18250A_SetPowerState(_get_Tuner_Instance_Index(u8TunerIndex),TunerUnit, tmPowerOn);
        TUNER_DBG(("\n=== TDA18250A Set2ndPowerState, result[0x%lX]. \n", err));

        if(err == TM_OK)
        {
          /* TDA18250A standard mode if changed */
          err = tmbslTDA18250A_SetStandardMode(_get_Tuner_Instance_Index(u8TunerIndex), TunerUnit, TDA18250AStdMode);
          TUNER_ERR(("\n=== TDA18250A init SetStandardMode status[0x%lX]. \n", err));
        }

    }
    else
    {
        TUNER_ERR(("\n=== TDA18250A open fail[%ld]. \n", err));
    }

    #if 0   //temp
    if(err == TM_OK)
    {
      /* Get TDA18250A PLL Lock status */
      err = tmbslTDA18250A_GetLockStatus(TunerUnit, &PLLLock);
    }
    #endif
    if(err == TM_OK)
    {
        DrvInstance_acc++;
        return TRUE;
    }
    else
    {
        TUNER_ERR(("\n=== TDA18250A init fail, status[%ld]. \n", err));
        return FALSE;
    }

}

MS_U32 MDrv_Tuner_Loop_Through_On(MS_U8 u8TunerIndex, MS_BOOL bOn)
{

    return TRUE;
}


MS_U32 MDrv_Tuner_Stand_By(MS_U8 u8TunerIndex)
{

    return TRUE;
}


MS_U32 MDrv_Tuner_Wake_Up(MS_U8 u8TunerIndex)
{

    return TRUE;
}
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
   /* Variable declarations */
    tmErrorCode_t err = TM_OK;

    if(IIC1_READ(TDA18250A_ADDR, pAddr, (MS_U8)AddrSize,  pData, (MS_U16)ReadLen)!=TRUE)
    {
        err = TDA18250A_ERR_HW_FAILED;
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
   /* Variable declarations */
    tmErrorCode_t err = TM_OK;

    if(IIC1_WRITE(TDA18250A_ADDR, pAddr, (MS_U8)AddrSize, pData, (MS_U16)WriteLen)!=TRUE)
    {
        err = TDA18250A_ERR_HW_FAILED;
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

    if(IIC_READ(TDA18250A_ADDR, pAddr, (MS_U8)AddrSize, pData, (MS_U16)ReadLen)!= TRUE)
        err = TDA18250A_ERR_HW_FAILED;

    //TUNER_ERR(("\n=== TDA18250A UserWrittenI2CRead, result[0x%X]. \n", err);
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

    if(IIC_WRITE(TDA18250A_ADDR, pAddr, (MS_U8)AddrSize, pData, (MS_U16)WriteLen)!= TRUE)
    {
        err = TDA18250A_ERR_HW_FAILED;
    }

    //TUNER_ERR(("\n=== TDA18250A UserWrittenI2CWrite, result[0x%X]. \n", err);
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
//tmErrorCode_t  UserWrittenMutexInit(ptmbslFrontEndMutexHandle *ppMutexHandle)
//{
    /* Variable declarations */
  //  tmErrorCode_t err = TM_OK;

    /* Customer code here */
    /* ...*/

    /* ...*/
    /* End of Customer code here */

    //return err;
//}


//*--------------------------------------------------------------------------------------
//* Function Name       : UserWrittenMutexDeInit
//* Object              :
//* Input Parameters    :      ptmbslFrontEndMutexHandle pMutex
//* Output Parameters   : tmErrorCode_t.
//*--------------------------------------------------------------------------------------
//tmErrorCode_t  UserWrittenMutexDeInit( ptmbslFrontEndMutexHandle pMutex)
//{
    /* Variable declarations */
    //tmErrorCode_t err = TM_OK;

    /* Customer code here */
    /* ...*/

    /* ...*/
    /* End of Customer code here */

    //return err;
//}



//*--------------------------------------------------------------------------------------
//* Function Name       : UserWrittenMutexAcquire
//* Object              :
//* Input Parameters    :     ptmbslFrontEndMutexHandle pMutex, UInt32 timeOut
//* Output Parameters   : tmErrorCode_t.
//*--------------------------------------------------------------------------------------
//tmErrorCode_t  UserWrittenMutexAcquire(ptmbslFrontEndMutexHandle pMutex, UInt32 timeOut)
//{
    /* Variable declarations */
    //tmErrorCode_t err = TM_OK;

    /* Customer code here */
    /* ...*/

    /* ...*/
    /* End of Customer code here */

    //return err;
//}

//*--------------------------------------------------------------------------------------
//* Function Name       : UserWrittenMutexRelease
//* Object              :
//* Input Parameters    :     ptmbslFrontEndMutexHandle pMutex
//* Output Parameters   : tmErrorCode_t.
//*--------------------------------------------------------------------------------------
//tmErrorCode_t  UserWrittenMutexRelease(ptmbslFrontEndMutexHandle pMutex)
//{
    /* Variable declarations */
    //tmErrorCode_t err = TM_OK;

    /* Customer code here */
    /* ...*/

    /* ...*/
    /* End of Customer code here */

    //return err;
//}

#define TDA18250A_CHIP_ID_1 0x4a
#define TDA18250A_CHIP_ID_0 0xc7
#define TDA18250A_REVISION_0 0x20
#define TDA18250B_REVISION_0 0x21


MS_BOOL TDA18250A_CheckExist(MS_U8 u8TunerIndex, MS_U32* pu32channel_cnt)
{
    MS_U8 Addr = 0x00;
    MS_U8 Data[3] = {0};

    HWI2C_PORT hwi2c_port;
    hwi2c_port = getI2CPort(u8TunerIndex);
    
    if (hwi2c_port < E_HWI2C_PORT_1)
    {
        IIC_READ(TDA18250A_ADDR, &Addr,1, &Data[0],1);
    }
    else if (hwi2c_port < E_HWI2C_PORT_2)
    {
        IIC1_READ(TDA18250A_ADDR, &Addr,1, &Data[0],1);
    }
    else
    {
        TUNER_ERR(("hwi2c_port number exceeds limitation\n"));
        return FALSE;
    }
    Addr = 0x01;
    if (hwi2c_port < E_HWI2C_PORT_1)
    {
        IIC_READ(TDA18250A_ADDR, &Addr,1, &Data[1],1);
    }
    else if (hwi2c_port < E_HWI2C_PORT_2)
    {
        IIC1_READ(TDA18250A_ADDR, &Addr,1, &Data[1],1);
    }
    else
    {
        TUNER_ERR(("hwi2c_port number exceeds limitation\n"));
        return FALSE;
    }
    Addr = 0x02;
    if (hwi2c_port < E_HWI2C_PORT_1)
    {
        IIC_READ(TDA18250A_ADDR, &Addr,1, &Data[2],1);
    }
    else if (hwi2c_port < E_HWI2C_PORT_2)
    {
        IIC1_READ(TDA18250A_ADDR, &Addr,1, &Data[2],1);
    }
    else
    {
        TUNER_ERR(("hwi2c_port number exceeds limitation\n"));
        return FALSE;
    }

    

    TUNER_DBG(("[18250a] read id =0x%x 0x%x , 0x%x\n",Data[0],Data[1],Data[2]));

    if((Data[0] == TDA18250A_CHIP_ID_0) && (Data[1] == TDA18250A_CHIP_ID_1) && (NULL != pu32channel_cnt))
    {
            *(pu32channel_cnt) += 1; 
    }
    return (Data[0] == TDA18250A_CHIP_ID_0) && (Data[1] == TDA18250A_CHIP_ID_1) ;
}

MS_BOOL TDA18250B_CheckExist(MS_U8 u8TunerIndex, MS_U32* pu32channel_cnt)
{
    MS_U8 Addr = 0x00;
    MS_U8 Data[3] = {0};

    HWI2C_PORT hwi2c_port;
    hwi2c_port = getI2CPort(u8TunerIndex);
    
    if (hwi2c_port < E_HWI2C_PORT_1)
    {
        IIC_READ(TDA18250A_ADDR, &Addr,1, &Data[0],1);
    }
    else if (hwi2c_port < E_HWI2C_PORT_2)
    {
        IIC1_READ(TDA18250A_ADDR, &Addr,1, &Data[0],1);
    }
    else
    {
        TUNER_ERR(("hwi2c_port number exceeds limitation\n"));
        return FALSE;
    }
    Addr = 0x01;
    if (hwi2c_port < E_HWI2C_PORT_1)
    {
        IIC_READ(TDA18250A_ADDR, &Addr,1, &Data[1],1);
    }
    else if (hwi2c_port < E_HWI2C_PORT_2)
    {
        IIC1_READ(TDA18250A_ADDR, &Addr,1, &Data[1],1);
    }
    else
    {
        TUNER_ERR(("hwi2c_port number exceeds limitation\n"));
        return FALSE;
    }
    Addr = 0x02;
    if (hwi2c_port < E_HWI2C_PORT_1)
    {
        IIC_READ(TDA18250A_ADDR, &Addr,1, &Data[2],1);
    }
    else if (hwi2c_port < E_HWI2C_PORT_2)
    {
        IIC1_READ(TDA18250A_ADDR, &Addr,1, &Data[2],1);
    }
    else
    {
        TUNER_ERR(("hwi2c_port number exceeds limitation\n"));
        return FALSE;
    }

    

    TUNER_DBG(("[18250b] read id =0x%x 0x%x , 0x%x\n",Data[0],Data[1],Data[2]));
    if((Data[0] == TDA18250A_CHIP_ID_0) && (Data[1] == TDA18250A_CHIP_ID_1) && (NULL != pu32channel_cnt))
    {
            *(pu32channel_cnt) += 1; 
    }

    return (Data[0] == TDA18250A_CHIP_ID_0) && (Data[1] == TDA18250A_CHIP_ID_1) ;

}


MS_BOOL MDRV_Tuner_TDA18250A_GetTunerIF(MS_U8 u8TunerIndex, MS_U32* pu32IF_Freq)
{
  tmErrorCode_t err = TM_OK;

  err = tmbslTDA18250A_GetIF(_get_Tuner_Instance_Index(u8TunerIndex), 0, (UInt32*)pu32IF_Freq);
  if(TM_OK == err)
  {
    *pu32IF_Freq /= 1000;
    return TRUE;
  }
  else
    return FALSE;
}

MS_BOOL MDRV_Tuner_TDA18250B_GetTunerIF(MS_U8 u8TunerIndex, MS_U32* pu32IF_Freq)
{
  tmErrorCode_t err = TM_OK;

  err = tmbslTDA18250A_GetIF(_get_Tuner_Instance_Index(u8TunerIndex), 0, (UInt32*)pu32IF_Freq);
  if(TM_OK == err)
  {
    *pu32IF_Freq /= 1000;
    return TRUE;
  }
  else
    return FALSE;
}





//*--------------------------------------------------------------------------------------
//*--------------------------------------------------------------------------------------

//* Function Name       : MDrv_Tuner_GetPowerLevel
//*Input: tuner index,power var
//*return;
//*TUNER PWL             dbm
//*11055	               110
//*10066        	        100
//*9066	                        90
//*8097	                        80
//*7109	                        70
//*6135	                        60
//*5101	                        50
//*4112	                        40
//*83087	                 30
//*--------------------------------------------------------------------------------------
//*--------------------------------------------------------------------------------------
MS_BOOL TDA18250A_GetPowerLevel(MS_U8 u8TunerIndex,float *Level)
{
    if(Level == NULL)
    {
        TUNER_ERR(("NULL Pointer %s %d\n ",__FUNCTION__,__LINE__));
        return FALSE;
    }
    Int32 orlLevel = 0;
    MS_FLOAT x = 0;
    
    tmbslTDA18250A_GetPowerLevel(_get_Tuner_Instance_Index(u8TunerIndex),0,&orlLevel);
    
    TUNER_DBG(("[%s][%d]original level:%ld\t",__FUNCTION__,__LINE__,orlLevel ));
    
    x = (MS_FLOAT)orlLevel;
    
    
    *Level = 0.0101 * x -1.3813;//DATA FROM HW INSTRUMENT SFU

    *Level -= 108.7506; //transfer unit dBuV to dBmW

    TUNER_DBG(("Power level:%f\n",*Level));

    return TRUE;
}

MS_BOOL TDA18250A_Extension_Function(MS_U8 u8TunerIndex, TUNER_EXT_FUNCTION_TYPE fuction_type, void *data)
{
    switch(fuction_type)
    {
        case TUNER_EXT_FUNC_GET_POWER_LEVEL:
            if(NULL == data)
            {
                TUNER_DBG(("[%s][%d]param [*data] null!\n",__FUNCTION__,__LINE__));
                return FALSE;
            }
            TDA18250A_GetPowerLevel(u8TunerIndex,(float *)data);
            break;
        case TUNER_EXT_FUNC_POWER_ON_OFF:
            if(FALSE == *(MS_BOOL *)data)   //power off
            {
                tmbslTDA18250A_SetPowerState(_get_Tuner_Instance_Index(u8TunerIndex), 0, tmPowerOff);
            }
            else
            {
                 tmbslTDA18250A_SetPowerState(_get_Tuner_Instance_Index(u8TunerIndex), 0, tmPowerOn);
            }
            break;
        default:
            TUNER_DBG(("Request extension function (%x) does not exist\n",fuction_type));
            return TRUE;
    }
    return TRUE;

}


#if IF_THIS_TUNER_INUSE(TUNER_TDA18250A)
DRV_TUNER_TABLE_TYPE GET_TUNER_ENTRY_NODE(TUNER_TDA18250A) DDI_DRV_TUNER_TABLE_ENTRY(tunertab) =
{
    .name               = "TUNER_TDA18250A",          // demod system name
    .data               = TUNER_TDA18250A,            // private data value
    .Init               = MDrv_Tuner_TDA18250A_Init,
    .SetFreq            = MDrv_Tuner_TDA18250A_SetFreq,
    .SetFreqS2          = MDrv_Tuner_Null_SetFreq_S2,
    .GetLock            = MDrv_Tuner_Null_GetLock,
    .SetTuner           = MDrv_Tuner_TDA18250A_SetTuner,
    .CheckExist         = TDA18250A_CheckExist,
    .GetTunerIF         = MDRV_Tuner_TDA18250A_GetTunerIF,
    .Extension_Function = TDA18250A_Extension_Function
};
#endif

#if IF_THIS_TUNER_INUSE(TUNER_TDA18250B)
DRV_TUNER_TABLE_TYPE GET_TUNER_ENTRY_NODE(TUNER_TDA18250B) DDI_DRV_TUNER_TABLE_ENTRY(tunertab) =
{
    .name               = "TUNER_TDA18250B",          // demod system name
    .data               = TUNER_TDA18250B,            // private data value
    .Init               = MDrv_Tuner_TDA18250B_Init,
    .SetFreq            = MDrv_Tuner_TDA18250A_SetFreq,
    .SetFreqS2          = MDrv_Tuner_Null_SetFreq_S2,
    .GetLock            = MDrv_Tuner_Null_GetLock,
    .SetTuner           = MDrv_Tuner_TDA18250A_SetTuner,
    .CheckExist         = TDA18250B_CheckExist,
    .GetTunerIF         = MDRV_Tuner_TDA18250B_GetTunerIF,
    .Extension_Function = TDA18250A_Extension_Function
};
#endif

#endif // TUNER_PHILIPS_TDA18250A

