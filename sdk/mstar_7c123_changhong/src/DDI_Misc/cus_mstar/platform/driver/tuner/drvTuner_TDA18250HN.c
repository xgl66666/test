//<MStar Software>
//******************************************************************************
// MStar Software
// Copyright (c) 2010 - 2012 MStar Semiconductor, Inc. All rights reserved.
// All software, firmware and related documentation herein ("MStar Software") are
// intellectual property of MStar Semiconductor, Inc. ("MStar") and protected by
// law, including, but not limited to, copyright law and international treaties.
// Any use, modification, reproduction, retransmission, or republication of all
// or part of MStar Software is expressly prohibited, unless prior written
// permission has been granted by MStar.
//
// By accessing, browsing and/or using MStar Software, you acknowledge that you
// have read, understood, and agree, to be bound by below terms ("Terms") and to
// comply with all applicable laws and regulations:
//
// 1. MStar shall retain any and all right, ownership and interest to MStar
//    Software and any modification/derivatives thereof.
//    No right, ownership, or interest to MStar Software and any
//    modification/derivatives thereof is transferred to you under Terms.
//
// 2. You understand that MStar Software might include, incorporate or be
//    supplied together with third party`s software and the use of MStar
//    Software may require additional licenses from third parties.
//    Therefore, you hereby agree it is your sole responsibility to separately
//    obtain any and all third party right and license necessary for your use of
//    such third party`s software.
//
// 3. MStar Software and any modification/derivatives thereof shall be deemed as
//    MStar`s confidential information and you agree to keep MStar`s
//    confidential information in strictest confidence and not disclose to any
//    third party.
//
// 4. MStar Software is provided on an "AS IS" basis without warranties of any
//    kind. Any warranties are hereby expressly disclaimed by MStar, including
//    without limitation, any warranties of merchantability, non-infringement of
//    intellectual property rights, fitness for a particular purpose, error free
//    and in conformity with any international standard.  You agree to waive any
//    claim against MStar for any loss, damage, cost or expense that you may
//    incur related to your use of MStar Software.
//    In no event shall MStar be liable for any direct, indirect, incidental or
//    consequential damages, including without limitation, lost of profit or
//    revenues, lost or damage of data, and unauthorized system use.
//    You agree that this Section 4 shall still apply without being affected
//    even if MStar Software has been modified by MStar in accordance with your
//    request or instruction for your use, except otherwise agreed by both
//    parties in writing.
//
// 5. If requested, MStar may from time to time provide technical supports or
//    services in relation with MStar Software to you for your use of
//    MStar Software in conjunction with your or your customer`s product
//    ("Services").
//    You understand and agree that, except otherwise agreed by both parties in
//    writing, Services are provided on an "AS IS" basis and the warranty
//    disclaimer set forth in Section 4 above shall apply.
//
// 6. Nothing contained herein shall be construed as by implication, estoppels
//    or otherwise:
//    (a) conferring any license or right to use MStar name, trademark, service
//        mark, symbol or any other identification;
//    (b) obligating MStar or any of its affiliates to furnish any person,
//        including without limitation, you and your customers, any assistance
//        of any kind whatsoever, or any information; or
//    (c) conferring any license or right under any intellectual property right.
//
// 7. These terms shall be governed by and construed in accordance with the laws
//    of Taiwan, R.O.C., excluding its conflict of law rules.
//    Any and all dispute arising out hereof or related hereto shall be finally
//    settled by arbitration referred to the Chinese Arbitration Association,
//    Taipei in accordance with the ROC Arbitration Law and the Arbitration
//    Rules of the Association by three (3) arbitrators appointed in accordance
//    with the said Rules.
//    The place of arbitration shall be in Taipei, Taiwan and the language shall
//    be English.
//    The arbitration award shall be final and binding to both parties.
//
//******************************************************************************
//<MStar Software>
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
#include "MsCommon.h"
#include "Board.h"

#include "drvIIC.h"
#include "drvTuner.h"
#include "drvTunerNull.h"

#if IF_THIS_TUNER_INUSE(TUNER_PHILIPS_TDA18250HN)

#include "tmNxTypes.h"
#include "tmCompId.h"
#include "tmFrontEnd.h"
#include "tmbslFrontEndTypes.h"

//*--------------------------------------------------------------------------------------
//* Include Driver files
//*--------------------------------------------------------------------------------------
#include "tmddTDA18250.h"
#include "tmbslTDA18250.h"

#define TDA18250HN_ADDR             0xC0

#define PWR_UNIT_DBUV               1
#if PWR_UNIT_DBUV
#define DBM2DBUV_FACTOR             64.99
#endif

//*--------------------------------------------------------------------------------------
//* Prototype of function to be provided by customer
//*--------------------------------------------------------------------------------------
static tmErrorCode_t   UserWrittenI2CRead(tmUnitSelect_t tUnit,UInt32 AddrSize, UInt8* pAddr,UInt32 ReadLen, UInt8* pData);
static tmErrorCode_t   UserWrittenI2CWrite (tmUnitSelect_t tUnit, UInt32 AddrSize, UInt8* pAddr,UInt32 WriteLen, UInt8* pData);
static tmErrorCode_t   UserWrittenI2C1Read(tmUnitSelect_t tUnit,UInt32 AddrSize, UInt8* pAddr,UInt32 ReadLen, UInt8* pData);
static tmErrorCode_t   UserWrittenI2C1Write (tmUnitSelect_t tUnit, UInt32 AddrSize, UInt8* pAddr,UInt32 WriteLen, UInt8* pData);
static tmErrorCode_t   UserWrittenWait(tmUnitSelect_t tUnit, UInt32 tms);
#if 0
static tmErrorCode_t   UserWrittenPrint(UInt32 level, const char* format, ...);
static tmErrorCode_t   UserWrittenMutexInit(ptmbslFrontEndMutexHandle *ppMutexHandle);
static tmErrorCode_t   UserWrittenMutexDeInit( ptmbslFrontEndMutexHandle pMutex);
static tmErrorCode_t   UserWrittenMutexAcquire(ptmbslFrontEndMutexHandle pMutex, UInt32 timeOut);
static tmErrorCode_t   UserWrittenMutexRelease(ptmbslFrontEndMutexHandle pMutex);
#endif

//#define IIC_WRITE(u8SlaveId, pu8Addr, u8AddrSize, pu8Buf, u32BufSize)   MDrv_IIC_WriteBytes(u8SlaveId, u8AddrSize, pu8Addr, (MS_U16)u32BufSize, pu8Buf)
//#define IIC_READ(u8SlaveId, pu8Addr, u8AddrSize, pu8Buf, u32BufSize)   MDrv_IIC_ReadBytes(u8SlaveId, u8AddrSize, pu8Addr, (MS_U16)u32BufSize, pu8Buf)

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


//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
static MS_U8 TDA118250HN_SetFreq(MS_U32 u32Frf, MS_U8 *u8Data) __attribute__ ((unused));



static MS_U8 TDA118250HN_SetFreq(MS_U32 u32Frf, MS_U8 *u8Data)
{

    return 4;
}


//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_Tuner_PHILIPS_TDA18250HN_SetFreq(MS_U8 u8TunerIndex, MS_U32 u32Freq)
{
    tmErrorCode_t err = TM_OK;
    tmbslFrontEndState_t PLLLockMaster = tmbslFrontEndStateUnknown;
    UInt32 uIFMaster = 0;

    err = tmbslTDA18250SetRf(0, u32Freq*1000);
    if(err != TM_OK)
        return err;

    /* Get TDA18250 Master PLL Lock status */
    err = tmbslTDA18250GetLockStatus(0, &PLLLockMaster);
    TUNER_DBG(("=====================Lock status=%d\n",PLLLockMaster));
    if(err != TM_OK)
        return err;

    /* Get TDA18250 Master IF */
    err = tmbslTDA18250GetIF(0, &uIFMaster);
    TUNER_DBG(("===================== uIFMaster=0x%lx\n",uIFMaster));
    if(err==TM_OK)
        return TRUE;
    else
        return False;
}

MS_BOOL MDrv_Tuner_PHILIPS_TDA18250HN_SetTuner(MS_U8 u8TunerIndex, MS_U32 u32Freq, MS_U8 u8Bw)
{
    return MDrv_Tuner_PHILIPS_TDA18250HN_SetFreq(u8TunerIndex,u32Freq);
}


MS_BOOL MDrv_Tuner_PHILIPS_TDA18250HN_PowerOnOff(MS_U8 u8TunerIndex, MS_BOOL bPowerOn)
{
    return TRUE;
}

MS_BOOL MDrv_Tuner_PHILIPS_TDA18250HN_Init(MS_U8 u8TunerIndex,TUNER_MS_INIT_PARAM* pParam)
{
    tmErrorCode_t err = TM_OK;
    tmbslFrontEndDependency_t sSrvTunerFunc;

    tmTDA18250StandardMode_t stdModeMaster = tmTDA18250_DIG_9MHz;
    UInt32 uRFMaster = 562000000;
    tmbslFrontEndState_t PLLLockMaster = tmbslFrontEndStateUnknown;
    UInt32 uIFMaster = 0;

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
    /* Low layer struct set-up to link with user written functions */
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
    TUNER_DBG(("=====================Lock status=%d\n",PLLLockMaster));
    if(err != TM_OK)
       return err;

    /* Get TDA18250 Master IF */
    err = tmbslTDA18250GetIF(0, &uIFMaster);
    TUNER_DBG(("===================== uIFMaster=0x%lx\n",uIFMaster));
    if(err != TM_OK)
       return err;

    UInt8 puBytes[69]={0};
    err = tmddTDA18250Read(0,0,69,puBytes);
    if(err != TM_OK)
       return err;

    TUNER_DBG(("--------------------------------------------------------------------\n"));
    int i=0;
    for(i=0;i<69;i++)
    {
        TUNER_DBG(("0x%x " ,puBytes[i]));
    }
    TUNER_DBG(("\n--------------------------------------------------------------------\n"));
    /* DeInitialize TDA18250 Master Driver */
    // err = tmbslTDA18250DeInit(0);

    if(err==TM_OK)
        return TRUE;
    else
        return FALSE;
}

MS_U32 MDrv_Tuner_PHILIPS_TDA18250HN_Loop_Through_On(MS_U8 u8TunerIndex, MS_BOOL bOn)
{

    return TRUE;
}


MS_U32 MDrv_Tuner_PHILIPS_TDA18250HN_Stand_By(MS_U8 u8TunerIndex)
{

    return TRUE;
}


MS_U32 MDrv_Tuner_PHILIPS_TDA18250HN_Wake_Up(MS_U8 u8TunerIndex)
{

    return TRUE;
}

MS_U32 MDrv_Tuner_PHILIPS_TDA18250HN_GetPowerLevel(MS_U8 u8TunerIndex, float *fPowerLevel)
{
    MS_S32 s32Level = 0;

    // get channel power, unit: dBm
    tmbslTDA18250GetPowerLevel(0, (Int32 *)&s32Level);
    if(s32Level < 0)
    {
        *fPowerLevel = ((float)s32Level)/100;
    #ifdef PWR_UNIT_DBUV
        *fPowerLevel = *fPowerLevel + DBM2DBUV_FACTOR;  // dBm -> dBuV
    #endif
    }

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

    if(IIC1_READ(TDA18250HN_ADDR, pAddr, (MS_U8)AddrSize,  pData, (MS_U16)ReadLen)!=TRUE)
    {
        err = TDA18250_ERR_HW_FAILED;
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

    if(IIC1_WRITE(TDA18250HN_ADDR, pAddr, (MS_U8)AddrSize, pData, (MS_U16)WriteLen)!=TRUE)
    {
        err = TDA18250_ERR_HW_FAILED;
		TUNER_ERR(("[%s][%d] \n",__FUNCTION__,__LINE__));
    }

    return err;
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
tmErrorCode_t UserWrittenI2CRead(tmUnitSelect_t tUnit,  UInt32 AddrSize, UInt8* pAddr,
UInt32 ReadLen, UInt8* pData)
{
   /* Variable declarations */
    tmErrorCode_t err = TM_OK;

    if(IIC_READ(TDA18250HN_ADDR, pAddr, (MS_U8)AddrSize,  pData, (MS_U16)ReadLen)!=TRUE)
    {
        err = TDA18250_ERR_HW_FAILED;
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
tmErrorCode_t UserWrittenI2CWrite (tmUnitSelect_t tUnit,    UInt32 AddrSize, UInt8* pAddr,
UInt32 WriteLen, UInt8* pData)
{
   /* Variable declarations */
    tmErrorCode_t err = TM_OK;

    if(IIC_WRITE(TDA18250HN_ADDR, pAddr, (MS_U8)AddrSize, pData, (MS_U16)WriteLen)!=TRUE)
    {
        err = TDA18250_ERR_HW_FAILED;
    }

    return err;
}

//*--------------------------------------------------------------------------------------
//* Function Name       : UserWrittenWait
//* Object              :
//* Input Parameters    :   tmUnitSelect_t tUnit
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
//* Input Parameters    :   UInt32 level, const char* format, ...
//*
//* Output Parameters   : tmErrorCode_t.
//*--------------------------------------------------------------------------------------
//tmErrorCode_t           UserWrittenPrint(UInt32 level, const char* format, ...)
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
//* Function Name       : UserWrittenMutexInit
//* Object              :
//* Input Parameters    :   ptmbslFrontEndMutexHandle *ppMutexHandle
//* Output Parameters   : tmErrorCode_t.
//*--------------------------------------------------------------------------------------
//tmErrorCode_t  UserWrittenMutexInit(ptmbslFrontEndMutexHandle *ppMutexHandle)
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
//* Function Name       : UserWrittenMutexDeInit
//* Object              :
//* Input Parameters    :    ptmbslFrontEndMutexHandle pMutex
//* Output Parameters   : tmErrorCode_t.
//*--------------------------------------------------------------------------------------
//tmErrorCode_t  UserWrittenMutexDeInit( ptmbslFrontEndMutexHandle pMutex)
//{
   /* Variable declarations */
  // tmErrorCode_t err = TM_OK;

/* Customer code here */
/* ...*/

/* ...*/
/* End of Customer code here */

   //return err;
//}



//*--------------------------------------------------------------------------------------
//* Function Name       : UserWrittenMutexAcquire
//* Object              :
//* Input Parameters    :   ptmbslFrontEndMutexHandle pMutex, UInt32 timeOut
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

  // return err;
//}

//*--------------------------------------------------------------------------------------
//* Function Name       : UserWrittenMutexRelease
//* Object              :
//* Input Parameters    :   ptmbslFrontEndMutexHandle pMutex
//* Output Parameters   : tmErrorCode_t.
//*--------------------------------------------------------------------------------------
//tmErrorCode_t  UserWrittenMutexRelease(ptmbslFrontEndMutexHandle pMutex)
//{
   /* Variable declarations */
  // tmErrorCode_t err = TM_OK;

/* Customer code here */
/* ...*/

/* ...*/
/* End of Customer code here */

   //return err;
//}

#define TDA18250HN_CHIP_ID 0xd5
MS_BOOL TDA18250HN_CheckExist(MS_U8 u8TunerIndex, MS_U32* pu32channel_cnt)
{
	MS_U8 pAddr = 0x00;
	MS_U8 pData = 0x00;

    HWI2C_PORT hwi2c_port;
    hwi2c_port = getI2CPort(u8TunerIndex);
    
    if (hwi2c_port < E_HWI2C_PORT_1)
    {
        IIC_READ(TDA18250HN_ADDR, &pAddr,1, &pData,1);
    }
    else if (hwi2c_port < E_HWI2C_PORT_2)
    {
        IIC1_READ(TDA18250HN_ADDR, &pAddr,1, &pData,1);
    }
    else
    {
        TUNER_ERR(("hwi2c_port number exceeds limitation\n"));
        return FALSE;
    }

    TUNER_DBG(("[18250hn] read id =0x%x\n",pData));

    if((pData == TDA18250HN_CHIP_ID) && (NULL != pu32channel_cnt))
        *(pu32channel_cnt) += 1;    
        

    return (pData == TDA18250HN_CHIP_ID);
}

MS_BOOL PHILIPS_TDA18250HN_Extension_Function(MS_U8 u8TunerIndex, TUNER_EXT_FUNCTION_TYPE fuction_type, void *data)
{
    switch(fuction_type)
    {
        case TUNER_EXT_FUNC_GET_POWER_LEVEL:
            MDrv_Tuner_PHILIPS_TDA18250HN_GetPowerLevel(u8TunerIndex, data);
            break;
        default:
            TUNER_DBG(("Request extension function (%x) does not exist\n",fuction_type));
            break;
    }
    return TRUE;
}

MS_BOOL MDRV_Tuner_TDA18250HN_GetTunerIF(MS_U8 u8TunerIndex, MS_U32* pu32IF_Freq)
{
 tmErrorCode_t err = TM_OK;

  err = tmbslTDA18250GetIF(0, (UInt32*)pu32IF_Freq);
  if(TM_OK == err)
  {
    *pu32IF_Freq /= 1000;
    return TRUE;
  }
  else
    return FALSE;
}
DRV_TUNER_TABLE_TYPE GET_TUNER_ENTRY_NODE(TUNER_PHILIPS_TDA18250HN) DDI_DRV_TUNER_TABLE_ENTRY(tunertab) =
{
    .name               = "TUNER_PHILIPS_TDA18250HN",          // demod system name
    .data               = TUNER_PHILIPS_TDA18250HN,            // private data value
    .Init               = MDrv_Tuner_PHILIPS_TDA18250HN_Init,
    .SetFreq            = MDrv_Tuner_PHILIPS_TDA18250HN_SetFreq,
    .SetFreqS2          = MDrv_Tuner_Null_SetFreq_S2,
    .GetLock            = MDrv_Tuner_Null_GetLock,
    .SetTuner           = MDrv_Tuner_PHILIPS_TDA18250HN_SetTuner,
    .CheckExist         = TDA18250HN_CheckExist,
    .GetTunerIF         = MDRV_Tuner_TDA18250HN_GetTunerIF,
    .Extension_Function = PHILIPS_TDA18250HN_Extension_Function
};

#endif // TUNER_PHILIPS_TDA18250HN

