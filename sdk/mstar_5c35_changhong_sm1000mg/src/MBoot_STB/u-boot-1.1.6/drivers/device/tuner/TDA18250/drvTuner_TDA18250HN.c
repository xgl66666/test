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
/// file    drvTuner_TDA18250HN.c
/// @brief  PHILIPS CD1616 DVBC Tuner Driver Interface
/// @author MStar Semiconductor Inc.
//////////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include <sys/common/MsCommon.h>
//#include "Board.h"

#include <sys/drv/drvIIC.h>
#include <device/drvTuner.h>

#if (FRONTEND_TUNER_TYPE == TUNER_PHILIPS_TDA18250HN)

#include "inc/tmNxTypes.h"
#include "inc/tmCompId.h"
#include "inc/tmFrontEnd.h"
#include "inc/tmbslFrontEndTypes.h"

//*--------------------------------------------------------------------------------------
//* Include Driver files
//*--------------------------------------------------------------------------------------
#include "tmddTDA18250/inc/tmddTDA18250.h"
#include "tmbslTDA18250/inc/tmbslTDA18250.h"

#define TDA18250HN_ADDR             0xC0


//*--------------------------------------------------------------------------------------
//* Prototype of function to be provided by customer
//*--------------------------------------------------------------------------------------
tmErrorCode_t     UserWrittenI2CRead(tmUnitSelect_t tUnit,UInt32 AddrSize, UInt8* pAddr,UInt32 ReadLen, UInt8* pData);
tmErrorCode_t     UserWrittenI2CWrite (tmUnitSelect_t tUnit, UInt32 AddrSize, UInt8* pAddr,UInt32 WriteLen, UInt8* pData);
tmErrorCode_t     UserWrittenWait(tmUnitSelect_t tUnit, UInt32 tms);
tmErrorCode_t     UserWrittenPrint(UInt32 level, const char* format, ...);
tmErrorCode_t      UserWrittenMutexInit(ptmbslFrontEndMutexHandle *ppMutexHandle);
tmErrorCode_t      UserWrittenMutexDeInit( ptmbslFrontEndMutexHandle pMutex);
tmErrorCode_t      UserWrittenMutexAcquire(ptmbslFrontEndMutexHandle pMutex, UInt32 timeOut);
tmErrorCode_t      UserWrittenMutexRelease(ptmbslFrontEndMutexHandle pMutex);

//#define IIC_WRITE(u8SlaveId, pu8Addr, u8AddrSize, pu8Buf, u32BufSize)   MDrv_IIC_WriteBytes(u8SlaveId, u8AddrSize, pu8Addr, (MS_U16)u32BufSize, pu8Buf)
//#define IIC_READ(u8SlaveId, pu8Addr, u8AddrSize, pu8Buf, u32BufSize)   MDrv_IIC_ReadBytes(u8SlaveId, u8AddrSize, pu8Addr, (MS_U16)u32BufSize, pu8Buf)

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------




//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
MS_U8 dmdConfig[] =

{
    // SZ 5015 board ¨C TDA18250HN internal RFAGC
    // Rev.: [0x3f] 0x82, [0x20] 0x72, [0x29] 0xff, [0x2a] 0x7f, [0x30] 0xd3
    0x01, 0x08, 0x84, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x73, 0x00, 0xb3, 0x33, 0x03, 0x05, 0x52, 0x52, 0x73, 0x73, 0x73, 0x31, 0x52, 0x73, 0x73, 0x73,
    0x72, 0xc0, 0x24, 0x00, 0x80, 0xff, 0x7f, 0x00, 0x80, 0xff, 0x7f, 0x0e, 0x2b, 0x0b, 0x47, 0x40,
    0xd3, 0x9a, 0xde, 0x11, 0xa4, 0x60, 0x08, 0x08, 0x00, 0x10, 0x02, 0x0b, 0x03, 0x05, 0x03, 0x82,
    0x52, 0x52, 0x55,
};
/*
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


//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_Tuner_SetFreq(MS_U32 u32Freq)
{
    tmErrorCode_t err = TM_OK;
    tmbslFrontEndState_t PLLLockMaster = tmbslFrontEndStateUnknown;
    UInt32 uIFMaster = 0;

    printf("\n =harry=Set frequency %d KHz",u32Freq);
    err = tmbslTDA18250SetRf(0, u32Freq*1000);
    if(err != TM_OK)
        return err;

    /* Get TDA18250 Master PLL Lock status */
    err = tmbslTDA18250GetLockStatus(0, &PLLLockMaster);
    printf("=====================Lock status=%d\n",PLLLockMaster);
    if(err != TM_OK)
        return err;

    /* Get TDA18250 Master IF */
    err = tmbslTDA18250GetIF(0, &uIFMaster);
    printf("===================== uIFMaster=%d\n",uIFMaster);
    if(err==TM_OK)
        return TRUE;
    else
        return False;
}


MS_BOOL MDrv_Tuner_PowerOnOff(MS_BOOL bPowerOn)
{
    return TRUE;
}

MS_BOOL MDrv_Tuner_Init(void)
{
    tmErrorCode_t err = TM_OK;
    tmbslFrontEndDependency_t sSrvTunerFunc;

    tmTDA18250StandardMode_t stdModeMaster = tmTDA18250_DIG_9MHz;
    UInt32 uRFMaster = 562000000;
    tmbslFrontEndState_t PLLLockMaster = tmbslFrontEndStateUnknown;
    UInt32 uIFMaster = 0;


    /* Low layer struct set-up to link with user written functions */
    sSrvTunerFunc.sIo.Write             = UserWrittenI2CWrite;
    sSrvTunerFunc.sIo.Read              = UserWrittenI2CRead;
    sSrvTunerFunc.sTime.Get             = Null;
    sSrvTunerFunc.sTime.Wait            = UserWrittenWait;
    sSrvTunerFunc.sDebug.Print          = Null;//UserWrittenPrint;
    sSrvTunerFunc.sMutex.Init           = Null;//UserWrittenMutexInit;
    sSrvTunerFunc.sMutex.DeInit         = Null;//UserWrittenMutexDeInit;
    sSrvTunerFunc.sMutex.Acquire        = Null;//UserWrittenMutexAcquire;
    sSrvTunerFunc.sMutex.Release        = Null;//UserWrittenMutexRelease;
    sSrvTunerFunc.dwAdditionalDataSize  = 0;
    sSrvTunerFunc.pAdditionalData       = Null;

    /* TDA18250 Master Driver low layer setup */
    err = tmbslTDA18250Init(0, &sSrvTunerFunc);
    if(err != TM_OK)
       return err;


    /* TDA18250 Master Hardware initialization */
    err = tmbslTDA18250Reset(0);
    if(err != TM_OK)
       return err;

    /* TDA18250 Master Hardware power state */
    err = tmbslTDA18250SetPowerState(0, tmTDA18250_PowerNormalMode);


    /* TDA18250 Master standard mode */
    err = tmbslTDA18250SetStandardMode(0, stdModeMaster);
    if(err != TM_OK)
       return err;

    /* TDA18250 Master RF frequency */
    err = tmbslTDA18250SetRf(0, uRFMaster);
    if(err != TM_OK)
       return err;

    /* Get TDA18250 Master PLL Lock status */
    err = tmbslTDA18250GetLockStatus(0, &PLLLockMaster);
    printf("=====================Lock status=%d\n",PLLLockMaster);
    if(err != TM_OK)
       return err;

    /* Get TDA18250 Master IF */
    err = tmbslTDA18250GetIF(0, &uIFMaster);
    printf("===================== uIFMaster=%d\n",uIFMaster);
    if(err != TM_OK)
       return err;

    UInt8 puBytes[69]={0};
    err = tmddTDA18250Read(0,0,69,puBytes);
    if(err != TM_OK)
       return err;

    printf("--------------------------------------------------------------------\n");
    int i=0;
    for(i=0;i<69;i++)
    {
        printf("0x%x " ,puBytes[i]);
    }
    printf("\n--------------------------------------------------------------------\n");
    /* DeInitialize TDA18250 Master Driver */
    // err = tmbslTDA18250DeInit(0);

    if(err==TM_OK)
        return TRUE;
    else
        return False;
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
    if(MDrv_IIC_ReadBytesPort0(TDA18250HN_ADDR, (MS_U16)AddrSize, pAddr, (MS_U16)ReadLen, pData)!=TRUE)
        err = TDA18250_ERR_HW_FAILED;
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
    if(MDrv_IIC_WriteBytesPort0(TDA18250HN_ADDR, (MS_U16)AddrSize, pAddr, (MS_U16)WriteLen, pData)!=TRUE)
    {
        err = TDA18250_ERR_HW_FAILED;
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



#endif // TUNER_PHILIPS_TDA18250HN

