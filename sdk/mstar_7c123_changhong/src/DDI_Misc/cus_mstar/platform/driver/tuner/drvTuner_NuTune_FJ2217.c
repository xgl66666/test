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
/// file    drvTuner_NuTune_FJ2217.c
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
#include "tmNxTypes.h"
#include "tmCompId.h"
#include "tmFrontEnd.h"
#include "tmbslFrontEndTypes.h"
#include "tmbslNT220HN.h"
#include "tmsysFrontEndTypes.h"

#define DBG_FJ2217(format, args...) printf("%04d %s " format, __LINE__, __FUNCTION__, ##args)

#if IF_THIS_TUNER_INUSE(TUNER_NUTUNE_FJ2217)

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------

#define TUNERADDR 0xC0
#define UNITSEL 0x0

extern tmErrorCode_t UserWrittenI2CRead(tmUnitSelect_t tUnit,UInt32 AddrSize, UInt8* pAddr,UInt32 ReadLen, UInt8* pData);
extern tmErrorCode_t UserWrittenI2CWrite (tmUnitSelect_t tUnit, UInt32 AddrSize, UInt8* pAddr,UInt32 WriteLen, UInt8* pData);
extern tmErrorCode_t UserWrittenWait(tmUnitSelect_t tUnit, UInt32 tms);
extern tmErrorCode_t UserWrittenPrint(UInt32 level, const char* format, ...);
extern tmErrorCode_t UserWrittenMutexInit(ptmbslFrontEndMutexHandle *ppMutexHandle);
extern tmErrorCode_t UserWrittenMutexDeInit( ptmbslFrontEndMutexHandle pMutex);
extern tmErrorCode_t UserWrittenMutexAcquire(ptmbslFrontEndMutexHandle pMutex, UInt32 timeOut);
extern tmErrorCode_t UserWrittenMutexRelease(ptmbslFrontEndMutexHandle pMutex);


//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------

static MS_S32 stFrontEndSem = -1;

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
MS_U8 dmdConfig[] =
{
#if (MS_BOARD_TYPE_SEL == BD_MST098B_D01A_S) || (MS_BOARD_TYPE_SEL == BD_MST098C_D01A_S) || (MS_BOARD_TYPE_SEL == BD_MST098D_D01A_S) || (MS_BOARD_TYPE_SEL == BD_MST098E_D01A_S) || (MS_BOARD_TYPE_SEL == BD_MST098E_D01B_S) || (MS_BOARD_TYPE_SEL == BD_MST128A_D01A) || (MS_BOARD_TYPE_SEL == BD_MST098F_D01A)
      0x01, 0x90, 0x7E, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
      0x73, 0x00, 0xb3, 0x33, 0x03, 0x05, 0x52, 0x52, 0x73, 0x73, 0x73, 0x31, 0x52, 0x73, 0x73, 0x73,
      0x72, 0xc0, 0x24, 0x00, 0x80, 0xff, 0x7f, 0x00, 0x80, 0xff, 0x7f, 0x0e, 0x2b, 0x0b, 0x47, 0x40,
      0xd3, 0x9a, 0xde, 0x11, 0xa4, 0x60, 0x08, 0x08, 0x00, 0x10, 0x02, 0x0b, 0x03, 0x05, 0x03, 0x82,
      0x52, 0x52, 0x55,
#endif
};
//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------

static MS_BOOL IIC_WRITE(MS_U8 u8SlaveID, MS_U8* paddr,MS_U8 u8AddrNum, MS_U8* pu8data,  MS_U16 u16size)
{

    if ( FALSE == MDrv_IIC_WriteBytes(FRONTEND_TUNER_PORT,u8SlaveID, u8AddrNum, paddr, u16size, pu8data) )
    {
        //printf("MDrv_IIC_WriteBytes fail \n");
        return FALSE;
    }
    else
    {
        //printf("MDrv_IIC_WriteBytes success \n");
    }

    MsOS_DelayTask(1); // delay 1ms

    return TRUE;


}


static MS_BOOL IIC_READ(MS_U8 u8SlaveID, MS_U8* paddr, MS_U8 u8AddrNum, MS_U8* pu8data, MS_U16 u16size)
{

    if ( FALSE == MDrv_IIC_ReadBytes(FRONTEND_TUNER_PORT,u8SlaveID, u8AddrNum, paddr, u16size, pu8data))
    {
        //printf("MDrv_IIC_ReadBytes fail \n");

    }
    else
    {
        //printf("MDrv_IIC_ReadBytes success \n");
    }
    return TRUE;

}

//*--------------------------------------------------------------------------------------
//* Function Name       : UserWrittenI2CRead
//* Object              :
//* Input Parameters    : 	tmUnitSelect_t tUnit
//* 						UInt32 AddrSize,
//* 						UInt8* pAddr,
//* 						UInt32 ReadLen,
//* 						UInt8* pData
//* Output Parameters   : tmErrorCode_t.
//*--------------------------------------------------------------------------------------
tmErrorCode_t UserWrittenI2CRead(tmUnitSelect_t tUnit,	UInt32 AddrSize, UInt8* pAddr,
UInt32 ReadLen, UInt8* pData)
{
   /* Variable declarations */
    tmErrorCode_t err = TM_OK;
    UInt8 U8TunerAddress = TUNERADDR; //for master tuner
    //printf("UserWrittenI2CRead AddrSize = 0x%x,pAddr = 0x%x,ReadLen = 0x%x,pData addr = 0x%x \n",AddrSize,*pAddr,ReadLen,(UInt32)pData);

    IIC_READ(U8TunerAddress,pAddr,AddrSize,pData,ReadLen);


   return err;
}

//*--------------------------------------------------------------------------------------
//* Function Name       : UserWrittenI2CWrite
//* Object              :
//* Input Parameters    : 	tmUnitSelect_t tUnit
//* 						UInt32 AddrSize,
//* 						UInt8* pAddr,
//* 						UInt32 WriteLen,
//* 						UInt8* pData
//* Output Parameters   : tmErrorCode_t.
//*--------------------------------------------------------------------------------------
tmErrorCode_t UserWrittenI2CWrite (tmUnitSelect_t tUnit, 	UInt32 AddrSize, UInt8* pAddr,
UInt32 WriteLen, UInt8* pData)
{
   // UInt32 i;
   /* Variable declarations */
    tmErrorCode_t err = TM_OK;
    UInt8 U8TunerAddress = TUNERADDR; //for master tuner

    //printf("===============================\n");
    //printf("UserWrittenI2CWrite AddrSize = 0x%x,pAddr = 0x%x,WriteLen = 0x%x,pData addr = 0x%x \n",AddrSize,*pAddr,WriteLen,(UInt32)pData);
    //for(i=0;i<WriteLen;i++)
    //{
    //    printf("pData[%d] = 0x%x \n",i,pData[i]);
    //}
    //printf("===============================\n");
    IIC_WRITE(U8TunerAddress,pAddr,AddrSize,pData,WriteLen);


   return err;
}

//*--------------------------------------------------------------------------------------
//* Function Name       : UserWrittenWait
//* Object              :
//* Input Parameters    : 	tmUnitSelect_t tUnit
//* 						UInt32 tms
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
//* Input Parameters    : 	UInt32 level, const char* format, ...
//*
//* Output Parameters   : tmErrorCode_t.
//*--------------------------------------------------------------------------------------
tmErrorCode_t UserWrittenPrint(UInt32 level, const char* format, ...)
{
   /* Variable declarations */
   tmErrorCode_t err = TM_OK;

    printf("======================");
   printf(format);
   printf("======================");

   return err;
}

//*--------------------------------------------------------------------------------------
//* Function Name       : UserWrittenMutexInit
//* Object              :
//* Input Parameters    : 	ptmbslFrontEndMutexHandle *ppMutexHandle
//* Output Parameters   : tmErrorCode_t.
//*--------------------------------------------------------------------------------------
tmErrorCode_t  UserWrittenMutexInit(ptmbslFrontEndMutexHandle *ppMutexHandle)
{
   /* Variable declarations */
    tmErrorCode_t err = TM_OK;
    stFrontEndSem = MsOS_CreateMutex (E_MSOS_FIFO, "MUTEX_FrontEnd", MSOS_PROCESS_PRIVATE);
    ppMutexHandle = (void *)stFrontEndSem;

   return err;
}


//*--------------------------------------------------------------------------------------
//* Function Name       : UserWrittenMutexDeInit
//* Object              :
//* Input Parameters    : 	 ptmbslFrontEndMutexHandle pMutex
//* Output Parameters   : tmErrorCode_t.
//*--------------------------------------------------------------------------------------
tmErrorCode_t  UserWrittenMutexDeInit( ptmbslFrontEndMutexHandle pMutex)
{
   /* Variable declarations */
   tmErrorCode_t err = TM_OK;
    MsOS_DeleteMutex(stFrontEndSem);


   return err;
}



//*--------------------------------------------------------------------------------------
//* Function Name       : UserWrittenMutexAcquire
//* Object              :
//* Input Parameters    : 	ptmbslFrontEndMutexHandle pMutex, UInt32 timeOut
//* Output Parameters   : tmErrorCode_t.
//*--------------------------------------------------------------------------------------
tmErrorCode_t  UserWrittenMutexAcquire(ptmbslFrontEndMutexHandle pMutex, UInt32 timeOut)
{
   /* Variable declarations */
   tmErrorCode_t err = TM_OK;
   MsOS_ObtainMutex(stFrontEndSem, (MS_U32)timeOut);


   return err;
}

//*--------------------------------------------------------------------------------------
//* Function Name       : UserWrittenMutexRelease
//* Object              :
//* Input Parameters    : 	ptmbslFrontEndMutexHandle pMutex
//* Output Parameters   : tmErrorCode_t.
//*--------------------------------------------------------------------------------------
tmErrorCode_t  UserWrittenMutexRelease(ptmbslFrontEndMutexHandle pMutex)
{
   /* Variable declarations */
   tmErrorCode_t err = TM_OK;
     MsOS_ReleaseMutex(stFrontEndSem);



   return err;
}


//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

MS_BOOL MDrv_Tuner_SetFreq(MS_U32 u32Freq)
{
    tmErrorCode_t err = TM_OK;
    tmNT220xStandardMode_t stdModeMaster = tmNT220x_QAM_6MHz;
    UInt32 uRFMaster = u32Freq * 1000;
    tmbslFrontEndState_t PLLLockMaster = tmbslFrontEndStateUnknown;
    UInt32 uIFMaster = 0;


    /* NT220X Master standard mode */
    printf("u32Freq = 0x%x , uRFMaster = 0x%x\n",u32Freq,uRFMaster);
    err = tmbslNT220xSetStandardMode(0, stdModeMaster);
    if(err != TM_OK)
    {
        printf("tmbslNT220xSetStandardMode fail err= 0x%x\n",err);
        return err;
    }

    /* NT220X Master RF frequency */
    err = tmbslNT220xSetRf(0, uRFMaster);
    if(err != TM_OK)
    {
        printf("tmbslNT220xSetRf fail err= 0x%x\n",err);
        return err;
    }

    /* Get NT220X Master PLL Lock status */
    err = tmbslNT220xGetLockStatus(0, &PLLLockMaster);
    if(err != TM_OK)
    {
       printf("tmbslNT220xGetLockStatus fail err= 0x%x\n",err);
       return err;
    }
    printf("PLLLockMaster = 0x%x\n",PLLLockMaster);

    /* Get NT220X Master IF */
    err = tmbslNT220xGetIF(0, &uIFMaster);
    if(err != TM_OK)
    {
        printf("tmbslNT220xGetIF fail err= 0x%x\n",err);
        return err;
    }
    printf("uIFMaster = 0x%x\n",uIFMaster);

    return TRUE;
}


MS_BOOL MDrv_Tuner_PowerOnOff(MS_BOOL bPowerOn)
{
    //[TBD]
    return TRUE;
}


MS_BOOL MDrv_Tuner_Init()
{
    /* Variable declarations */
    tmErrorCode_t err = TM_OK;
    tmbslFrontEndDependency_t sSrvTunerFunc;

    sSrvTunerFunc.sIo.Write             = UserWrittenI2CWrite;
    sSrvTunerFunc.sIo.Read              = UserWrittenI2CRead;
    sSrvTunerFunc.sTime.Get             = Null;
    sSrvTunerFunc.sTime.Wait            = UserWrittenWait;
    sSrvTunerFunc.sDebug.Print          = UserWrittenPrint;
    sSrvTunerFunc.sMutex.Init           = UserWrittenMutexInit;
    sSrvTunerFunc.sMutex.DeInit         = UserWrittenMutexDeInit;
    sSrvTunerFunc.sMutex.Acquire        = UserWrittenMutexAcquire;
    sSrvTunerFunc.sMutex.Release        = UserWrittenMutexRelease;
    sSrvTunerFunc.dwAdditionalDataSize  = 0;
    sSrvTunerFunc.pAdditionalData       = Null;

    printf("\n\n[MDrv_Tuner_Init], entry\n\n");

   /* NT220X Master Driver low layer setup */
   err = tmbslNT220xInit(0, &sSrvTunerFunc);
   if(err != TM_OK)
   {
        printf("tmbslNT220xInit fail err= 0x%x\n",err);
        return err;
   }

    /* NT220X Master Hardware initialization */
   err = tmbslNT220xReset(0);
   if(err != TM_OK)
   {
        printf("tmbslNT220xReset fail err= 0x%x\n",err);
       return err;
   }


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

#endif // TUNER_PHILIPS_CD1616LF_GIH_4

