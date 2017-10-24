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
/// file    drvTuner_TDA18225.c
/// @brief  NXP TDA18225 DVBT/T2 Tuner Driver Interface
/// @author MStar Semiconductor Inc.
//////////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include "MsCommon.h"
#include "Board.h"
#include "drvTuner.h"
#include "drvTunerNull.h"
#include "drvIIC.h"
#include "apiDigiTuner.h"
#include "drvTuner.h"

//#include "drvTuner.h"

#if IF_THIS_TUNER_INUSE(TUNER_TDA18225)
#include "tmNxTypes.h"
#include "tmCompId.h"
#include "tmFrontEnd.h"
#include "tmbslFrontEndTypes.h"
#include "drvTuner_TDA18225.h"





//*--------------------------------------------------------------------------------------
//* Include Driver files
//*--------------------------------------------------------------------------------------
#include "tmbslTDA182I5a.h"

#define TDA182I5a_ADDR             0xC0

//UserWrittenI2CRead(0, 1, (00), 1, ()) //0xc7 18250b  0xd5 18250
//*--------------------------------------------------------------------------------------
//* Prototype of function to be provided by customer
//*--------------------------------------------------------------------------------------
tmErrorCode_t     UserWrittenI2CRead(tmUnitSelect_t tUnit,UInt32 AddrSize, UInt8* pAddr,UInt32 ReadLen, UInt8* pData);
tmErrorCode_t     UserWrittenI2CWrite (tmUnitSelect_t tUnit, UInt32 AddrSize, UInt8* pAddr,UInt32 WriteLen, UInt8* pData);
tmErrorCode_t     UserWrittenI2C1Read(tmUnitSelect_t tUnit,UInt32 AddrSize, UInt8* pAddr,UInt32 ReadLen, UInt8* pData);
tmErrorCode_t     UserWrittenI2C1Write (tmUnitSelect_t tUnit, UInt32 AddrSize, UInt8* pAddr,UInt32 WriteLen, UInt8* pData);
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

static TUNER_MS_INIT_PARAM TDA18225_InitParam[MAX_FRONTEND_NUM];
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
/*MS_U8 MDrv_Demod_Get_Packet_Error(MS_U16 *u16_data)
{
    return TRUE;
}*/

MS_BOOL MDrv_Tuner_TDA18225_SetTuner(MS_U8 u8TunerIndex,MS_U32 dwFreq /*Khz*/, MS_U8 u8Bw /*MHz*/)
{

    tmErrorCode_t err = TM_OK;
    TDA182I5aStandardMode_t standard_mode = TDA182I5a_StandardMode_Unknown;

    TUNER_DBG(("---> 18225 SetFreq: %u \n", dwFreq));
    if((*TDA18225_InitParam[u8TunerIndex].pCur_Broadcast_type == DVBT) || (*TDA18225_InitParam[u8TunerIndex].pCur_Broadcast_type == DVBT2))
    {
        switch(u8Bw)
        {
            case TUNER_BW_MODE_6MHZ:
                TUNER_DBG(("=== TDA18225_DVBT_6MHz\n"));
                standard_mode = TDA182I5a_DVBT_6MHz;
                break;
            case TUNER_BW_MODE_7MHZ:
                TUNER_DBG(("=== TDA18225_DVBT_7MHz\n"));
                standard_mode = TDA182I5a_DVBT_7MHz;
                break;
            case TUNER_BW_MODE_1_7MHZ:
                TUNER_DBG(("=== TDA18225_DVBT_1_7MHz\n"));
                standard_mode = TDA182I5a_DVBT_1_7MHz;
                break;     
           case TUNER_BW_MODE_8MHZ:
           default:
                TUNER_DBG(("=== TDA18225_DVBT_8MHz\n"));
                standard_mode = TDA182I5a_DVBT_8MHz;
                break;
       }
    }
    else if(*TDA18225_InitParam[u8TunerIndex].pCur_Broadcast_type == ISDBT)
    {
          standard_mode = TDA182I5a_ISDBT_6MHz;
    }
    else if(*TDA18225_InitParam[u8TunerIndex].pCur_Broadcast_type == ATSC)
    {
          standard_mode = TDA182I5a_ATSC_6MHz;
    }

    if(TM_OK != tmbslTDA182I5a_SetStandardMode(_get_Tuner_Instance_Index(u8TunerIndex), standard_mode))
        TUNER_ERR(("!!!!! standard_mode error\n"));
    err = tmbslTDA182I5a_SetRF(_get_Tuner_Instance_Index(u8TunerIndex), dwFreq*1000);

    if(err==TM_OK)
        return TRUE;
    else
        return FALSE;
}


MS_BOOL MDrv_Tuner_TDA18225_PowerOnOff(MS_U8 u8TunerIndex,MS_BOOL bPowerOn)
{
    return TRUE;
}

MS_BOOL MDrv_Tuner_TDA18225_Init(MS_U8 u8TunerIndex, TUNER_MS_INIT_PARAM* pParam)
{
    tmErrorCode_t err = TM_OK;
    tmbslFrontEndDependency_t sSrvTunerFunc;
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
        TDA18225_InitParam[u8TunerIndex].pCur_Broadcast_type = pParam->pCur_Broadcast_type;

    DrvInstance[u8TunerIndex] = DrvInstance_acc;
   
    /* Low layer struct set-up to link with user written functions */
    //sSrvTunerFunc.sIo.Write             = UserWrittenI2CWrite;
    //sSrvTunerFunc.sIo.Read              = UserWrittenI2CRead;
    sSrvTunerFunc.sTime.Get             = Null;
    sSrvTunerFunc.sTime.Wait            = UserWrittenWait;
    sSrvTunerFunc.sDebug.Print          = Null;//UserWrittenPrint;
    sSrvTunerFunc.sMutex.Init           = Null;//UserWrittenMutexInit;
    sSrvTunerFunc.sMutex.DeInit         = Null;//UserWrittenMutexDeInit;
    sSrvTunerFunc.sMutex.Acquire        = Null;//UserWrittenMutexAcquire;
    sSrvTunerFunc.sMutex.Release        = Null;//UserWrittenMutexRelease;
    sSrvTunerFunc.dwAdditionalDataSize  = 0;
    sSrvTunerFunc.pAdditionalData       = Null;

    /* Tuner init */
    err = tmbslTDA182I5a_Open(_get_Tuner_Instance_Index(u8TunerIndex), &sSrvTunerFunc);

    if (err == TM_OK)
    {
        err = tmbslTDA182I5a_SetPowerState(_get_Tuner_Instance_Index(u8TunerIndex), tmPowerOn);
    }

    if (err == TM_OK)
    {
        err = tmbslTDA182I5a_HwInit(_get_Tuner_Instance_Index(u8TunerIndex));
    }

    if(err==TM_OK)
    {
        DrvInstance_acc++;
        return TRUE;
    }
    else
        return FALSE;
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
tmErrorCode_t UserWrittenI2CRead(tmUnitSelect_t tUnit,    UInt32 AddrSize, UInt8* pAddr,UInt32 ReadLen, UInt8* pData)
{
   /* Variable declarations */
    tmErrorCode_t err = TM_OK;
    if(MDrv_IIC_ReadBytesPort0(TDA182I5a_ADDR, (MS_U16)AddrSize, pAddr, (MS_U16)ReadLen, pData)!=TRUE)
    {
        err = TDA182I5a_ERR_HW_FAILED;
        TUNER_ERR(("---> UserWrittenI2CRead fail! \n"));
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
tmErrorCode_t UserWrittenI2CWrite (tmUnitSelect_t tUnit,     UInt32 AddrSize, UInt8* pAddr,UInt32 WriteLen, UInt8* pData)
{
   /* Variable declarations */
    tmErrorCode_t err = TM_OK;
    if(MDrv_IIC_WriteBytesPort0(TDA182I5a_ADDR, (MS_U16)AddrSize, pAddr, (MS_U16)WriteLen, pData)!=TRUE)
    {
        err = TDA182I5a_ERR_HW_FAILED;
        TUNER_ERR(("---> UserWrittenI2CWrite fail! \n"));
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
tmErrorCode_t UserWrittenI2C1Read(tmUnitSelect_t tUnit,    UInt32 AddrSize, UInt8* pAddr,UInt32 ReadLen, UInt8* pData)
{
   /* Variable declarations */
    tmErrorCode_t err = TM_OK;
    if(MDrv_IIC_ReadBytesPort1(TDA182I5a_ADDR, (MS_U16)AddrSize, pAddr, (MS_U16)ReadLen, pData)!=TRUE)
    {
        err = TDA182I5a_ERR_HW_FAILED;
        TUNER_ERR(("---> UserWrittenI2C1Read fail! \n"));
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
tmErrorCode_t UserWrittenI2C1Write (tmUnitSelect_t tUnit,     UInt32 AddrSize, UInt8* pAddr,UInt32 WriteLen, UInt8* pData)
{
   /* Variable declarations */
    tmErrorCode_t err = TM_OK;
    if(MDrv_IIC_WriteBytesPort1(TDA182I5a_ADDR, (MS_U16)AddrSize, pAddr, (MS_U16)WriteLen, pData)!=TRUE)
    {
        err = TDA182I5a_ERR_HW_FAILED;
        TUNER_ERR(("---> UserWrittenI2C1Write fail! \n"));
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
#if 0
//*--------------------------------------------------------------------------------------
//* Function Name       : UserWrittenPrint
//* Object              :
//* Input Parameters    :     UInt32 level, const char* format, ...
//*
//* Output Parameters   : tmErrorCode_t.
//*--------------------------------------------------------------------------------------
tmErrorCode_t  UserWrittenPrint(UInt32 level, const char* format, ...)
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
#endif
#define TDA18225_CHIP_ID_1 0x27
#define TDA18225_CHIP_ID_0 0xc7
MS_BOOL TDA18225_CheckExist(MS_U8 u8TunerIndex, MS_U32* pu32channel_cnt)
{
    UInt8 Addr=0x00;
    UInt8 Data[3] = {0};;
    HWI2C_PORT hwi2c_port;

    hwi2c_port = getI2CPort(u8TunerIndex);
    
    if (hwi2c_port < E_HWI2C_PORT_1)
    {
        UserWrittenI2CRead(0, 1, &Addr, 1, &Data[0]);
    }
    else if (hwi2c_port < E_HWI2C_PORT_2)
    {
        UserWrittenI2C1Read(0, 1, &Addr, 1, &Data[0]);
    }
    else
    {
        TUNER_ERR(("hwi2c_port number exceeds limitation\n"));
        return FALSE;
    }
    Addr = 0x01;
    if (hwi2c_port < E_HWI2C_PORT_1)
    {
        UserWrittenI2CRead(0, 1, &Addr,1, &Data[1]);
    }
    else if (hwi2c_port < E_HWI2C_PORT_2)
    {
        UserWrittenI2C1Read(0, 1, &Addr, 1, &Data[1]);
    }
    else
    {
        TUNER_ERR(("hwi2c_port number exceeds limitation\n"));
        return FALSE;
    }
    Addr = 0x02;
    if (hwi2c_port < E_HWI2C_PORT_1)
    {
        UserWrittenI2CRead(0, 1, &Addr,1, &Data[2]);
    }
    else if (hwi2c_port < E_HWI2C_PORT_2)
    {
        UserWrittenI2C1Read(0, 1, &Addr,1, &Data[2]);
    }
    else
    {
        TUNER_ERR(("hwi2c_port number exceeds limitation\n"));
        return FALSE;
    }
    TUNER_DBG(("[18225] read id =0x%x 0x%x , 0x%x\n",Data[0],Data[1],Data[2]));

    if((Data[0] == TDA18225_CHIP_ID_0) && (Data[1] == TDA18225_CHIP_ID_1) && (NULL != pu32channel_cnt))
    {
            *(pu32channel_cnt) += 1; 
    }
    return (Data[0] == TDA18225_CHIP_ID_0) && (Data[1] == TDA18225_CHIP_ID_1) ;

}

MS_BOOL TUNER_TDA18225_Extension_Function(MS_U8 u8TunerIndex, TUNER_EXT_FUNCTION_TYPE fuction_type, void *data)
{
    switch(fuction_type)
    {
        default:
            TUNER_DBG(("Request extension function (%x) does not exist\n",fuction_type));
            return TRUE;
    }

}

MS_BOOL MDRV_Tuner_TDA18225_GetTunerIF(MS_U8 u8TunerIndex, MS_U32* pu32IF_Freq)
{
  tmErrorCode_t err = TM_OK;

  err = tmbslTDA182I5a_GetIF(_get_Tuner_Instance_Index(u8TunerIndex), (UInt32*)pu32IF_Freq);
  if(TM_OK == err)
  {
    *pu32IF_Freq /= 1000;
    return TRUE;
  }
  else
    return FALSE;
}


DRV_TUNER_TABLE_TYPE GET_TUNER_ENTRY_NODE(TUNER_TDA18225) DDI_DRV_TUNER_TABLE_ENTRY(tunertab) =
{
    .name               = "TUNER_TDA18225",          // demod system name
    .data               = TUNER_TDA18225,            // private data value
    .Init               = MDrv_Tuner_TDA18225_Init,
    .SetFreq            = MDrv_Tuner_Null_SetFreq,
    .SetFreqS2          = MDrv_Tuner_Null_SetFreq_S2,
    .GetLock            = MDrv_Tuner_Null_GetLock,
    .SetTuner           = MDrv_Tuner_TDA18225_SetTuner,
    .CheckExist         = TDA18225_CheckExist,
    .GetTunerIF         = MDRV_Tuner_TDA18225_GetTunerIF,
    .Extension_Function = TUNER_TDA18225_Extension_Function
};

#endif // TUNER_TDA18225

