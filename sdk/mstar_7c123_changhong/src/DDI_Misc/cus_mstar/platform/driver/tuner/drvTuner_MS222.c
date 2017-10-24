//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (�MStar Confidential Information�) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
///
/// file    drvTuner_MS222.c
/// @author MStar Semiconductor Inc.
//////////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include "MsCommon.h"
#include "Board.h"

#include "drvIIC.h"
#include "drvTuner.h"

#if IF_THIS_TUNER_INUSE(TUNER_MS222)

// NXP Architecture includes
#include "tmCompId.h"
#include "tmNxTypes.h"
#include "tmFrontEnd.h"
#include "tmbslFrontEndCfgItem.h"
#include "tmbslFrontEndTypes.h"

// Components includes
#include "tmbslMS222.h"
#include "tmbslMS222_Advanced.h"
#include "drvTunerNull.h"
#include "apiDigiTuner.h"

#define MS222_ADDR             0xC0

//*--------------------------------------------------------------------------------------
//* Prototype of function to be provided by customer
//*--------------------------------------------------------------------------------------
static tmErrorCode_t     UserWrittenI2CRead(tmUnitSelect_t tUnit,UInt32 AddrSize, UInt8* pAddr,UInt32 ReadLen, UInt8* pData);
static tmErrorCode_t     UserWrittenI2CWrite (tmUnitSelect_t tUnit, UInt32 AddrSize, UInt8* pAddr,UInt32 WriteLen, UInt8* pData);
static tmErrorCode_t     UserWrittenI2C1Read(tmUnitSelect_t tUnit,UInt32 AddrSize, UInt8* pAddr,UInt32 ReadLen, UInt8* pData);
static tmErrorCode_t     UserWrittenI2C1Write (tmUnitSelect_t tUnit, UInt32 AddrSize, UInt8* pAddr,UInt32 WriteLen, UInt8* pData);

static tmErrorCode_t     UserWrittenWait(tmUnitSelect_t tUnit, UInt32 tms);
static tmErrorCode_t     UserWrittenPrint(UInt32 level, const char* format, ...);

#define IIC_WRITE                   MDrv_IIC_Write
#define IIC_READ                    MDrv_IIC_Read
#define IIC1_WRITE                  MDrv_IIC1_Write
#define IIC1_READ                   MDrv_IIC1_Read

//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

#define TunerUnit0 0
#define TunerUnit1 1


// This function opens the drivers and initialiases the components for one path
MS_BOOL MDrv_Tuner_MS222_Init(MS_U8 u8TunerIndex)
{
    tmUnitSelect_t tunerUnit;

    tmbslFrontEndDependency_t  sSrvFunc;
    MS222_AdditionnalData_t sMS222AdditionnalData;
    tmErrorCode_t err = TM_ERR_NOT_INITIALIZED;
    HWI2C_PORT hwi2c_port;

    hwi2c_port = getI2CPort(u8TunerIndex);

    if (hwi2c_port < E_HWI2C_PORT_1)
    {
        sSrvFunc.sIo.Write = UserWrittenI2CWrite;
        sSrvFunc.sIo.Read = UserWrittenI2CRead;
    }
    else if (hwi2c_port < E_HWI2C_PORT_2)
    {
        sSrvFunc.sIo.Write = UserWrittenI2C1Write;
        sSrvFunc.sIo.Read = UserWrittenI2C1Read;
    }
    else
    {
        printf("hwi2c_port number exceeds limitation\n");
        return FALSE;
    }

    tunerUnit = u8TunerIndex;

    sSrvFunc.sTime.Wait = UserWrittenWait;
    sSrvFunc.sDebug.Print = UserWrittenPrint;
    sSrvFunc.sMutex.Init = Null;
    sSrvFunc.sMutex.DeInit = Null;
    sSrvFunc.sMutex.Acquire = Null;
    sSrvFunc.sMutex.Release = Null;

    /* Tuner init */
    sMS222AdditionnalData.tOtherUnit = TunerUnit1;

    sSrvFunc.dwAdditionalDataSize = sizeof(sMS222AdditionnalData);
    sSrvFunc.pAdditionalData = &sMS222AdditionnalData;
    err = tmbslMS222_Open(tunerUnit, &sSrvFunc);
    printf("\n=== MS222 open, result[0x%lX]. \n", err);

    if (err == TM_OK)
    {
        err = tmbslMS222_HwInit(tunerUnit);
        printf("\n=== MS222 HwInit, result[0x%lX]. \n", err);
    }
    if (err == TM_OK)
    {
        err = tmbslMS222_SetPowerState(tunerUnit, tmPowerOn);
        printf("\n=== MS222 SetPowerState, result[0x%lX]. \n", err);
    }

    if(err == TM_OK)
    {
        return TRUE;
    }
    else
    {
        printf("\n=== MS222 init fail, status[%ld]. \n", err);
        return FALSE;
    }

}

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

MS_U32 MDrv_Tuner_MS222_SetTuner(MS_U8 u8TunerIndex, MS_U32 dwFreq /*Khz*/, MS_U8 ucBw /*MHz*/)
{
    tmUnitSelect_t tunerUnit;
    tmErrorCode_t err;

    tunerUnit = u8TunerIndex;

    tmbslMS222_SetXtal(0, MS222_XtalFreq_24000000);

    // Set Tuner
    err = tmbslMS222_SetStandardMode(tunerUnit, MS222_DVBT_8MHz);
    if (err == TM_OK)
    {
        err = tmbslMS222_SetRF(tunerUnit, dwFreq*1000);
    }

    if(err == TM_OK)
        return TRUE;
    else
        return FALSE;

}

#if 0
MS_BOOL MDrv_Tuner_PowerOnOff(MS_BOOL bPowerOn)
{
    return TRUE;
}

MS_U32 MDrv_Tuner_Loop_Through_On(MS_BOOL bOn)
{

    return TRUE;
}


MS_U32 MDrv_Tuner_Stand_By(void)
{

    return TRUE;
}


MS_U32 MDrv_Tuner_Wake_Up(void)
{

    return TRUE;
}
#endif

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

    if(IIC_READ(MS222_ADDR, pAddr, (MS_U8)AddrSize, pData, (MS_U16)ReadLen)!= TRUE)
        err = MS222_ERR_HW_FAILED;

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

    if(IIC_WRITE(MS222_ADDR, pAddr, (MS_U8)AddrSize, pData, (MS_U16)WriteLen)!= TRUE)
    {
        err = MS222_ERR_HW_FAILED;
    }

    return err;
}

//*--------------------------------------------------------------------------------------
//* Function Name       : UserWrittenI2C1Read
//* Object              :
//* Input Parameters    :     tmUnitSelect_t tUnit
//*                         UInt32 AddrSize,
//*                         UInt8* pAddr,
//*                         UInt32 ReadLen,
//*                         UInt8* pData
//* Output Parameters   : tmErrorCode_t.
//*--------------------------------------------------------------------------------------
tmErrorCode_t UserWrittenI2C1Read(tmUnitSelect_t tUnit,    UInt32 AddrSize, UInt8* pAddr,
UInt32 ReadLen, UInt8* pData)
{
   /* Variable declarations */
    tmErrorCode_t err = TM_OK;

    if(IIC1_READ(MS222_ADDR, pAddr, (MS_U8)AddrSize, pData, (MS_U16)ReadLen)!= TRUE)
        err = MS222_ERR_HW_FAILED;

    return err;
}
//*--------------------------------------------------------------------------------------
//* Function Name       : UserWrittenI2C1Write
//* Object              :
//* Input Parameters    :     tmUnitSelect_t tUnit
//*                         UInt32 AddrSize,
//*                         UInt8* pAddr,
//*                         UInt32 WriteLen,
//*                         UInt8* pData
//* Output Parameters   : tmErrorCode_t.
//*--------------------------------------------------------------------------------------
tmErrorCode_t UserWrittenI2C1Write (tmUnitSelect_t tUnit,     UInt32 AddrSize, UInt8* pAddr,
UInt32 WriteLen, UInt8* pData)
{
   /* Variable declarations */
    tmErrorCode_t err = TM_OK;

    if(IIC1_WRITE(MS222_ADDR, pAddr, (MS_U8)AddrSize, pData, (MS_U16)WriteLen)!= TRUE)
    {
        err = MS222_ERR_HW_FAILED;
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

MS_BOOL TUNER_MS222_Extension_Function(MS_U8 u8TunerIndex, TUNER_EXT_FUNCTION_TYPE fuction_type, void *data)
{
    switch(fuction_type)
    {
        default:
            printf("Request extension function (%x) does not exist\n",fuction_type);
            return TRUE;
    }

}

DRV_TUNER_TABLE_TYPE GET_TUNER_ENTRY_NODE(TUNER_MS222) DDI_DRV_TUNER_TABLE_ENTRY(tunertab) =
{
    .name               = "TUNER_MS222",          // demod system name
    .data               = TUNER_MS222,            // private data value
    .Init               = MDrv_Tuner_MS222_Init,
    .SetFreq            = MDrv_Tuner_Null_SetFreq,
    .SetFreqS2          = MDrv_Tuner_Null_SetFreq_S2,
    .GetLock            = MDrv_Tuner_Null_GetLock,
    .SetTuner           = MDrv_Tuner_MS222_SetTuner,
    .Extension_Function = TUNER_MS222_Extension_Function
};


#endif // TUNER_MS222


