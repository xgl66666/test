/*
  Copyright (C) 2006-2009 NXP B.V., All Rights Reserved.
  This source code and any compilation or derivative thereof is the proprietary
  information of NXP B.V. and is confidential in nature. Under no circumstances
  is this software to be  exposed to or placed under an Open Source License of
  any type without the expressed written permission of NXP B.V.
 *
 * \file          tmddTDA18260Stream.c
 *
 *                1
 *
 * \date          %modify_time%
 *
 * \brief         Describe briefly the purpose of this file.
 *
 * REFERENCE DOCUMENTS :
 *                TDA18260STREAM_Driver_User_Guide.pdf
 *
 * Detailed description may be added here.
 *
 * \section info Change Information
 *
*/
#include "Board.h"
#if(FRONTEND_TUNER_TYPE == TUNER_TDA18260)

/*============================================================================*/
/* Standard include files:                                                    */
/*============================================================================*/
#include "tmNxTypes.h"
#include "tmCompId.h"
#include "tmFrontEnd.h"
#include "tmUnitParams.h"
#include "tmbslFrontEndTypes.h"

/*============================================================================*/
/* Project include files:                                                     */
/*============================================================================*/
#include "tmddTDA18260Stream.h"
#include "tmddTDA18260Stream_RegDef.h"
#include "tmddTDA18260Stream_Local.h"
#include "tmddTDA18260Stream_Instance.h"
#include <tmddTDA18260Stream_InstanceCustom.h>



/*============================================================================*/
/* Exported functions:                                                        */
/*============================================================================*/

/*============================================================================*/
/* FUNCTION:    tmddTDA18260Stream_Init:                                      */
/*                                                                            */
/* DESCRIPTION: Initializes driver setup environment.                         */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmddTDA18260Stream_Init
(
    tmUnitSelect_t              tUnit,      /* I: Unit number      */
    tmbslFrontEndDependency_t*  psSrvFunc   /* I: setup parameters */
)
{
    ptmddTDA18260Stream_Object_t  pObj = Null;
    tmErrorCode_t                 err  = TM_OK;


    /* Test parameter(s) */
    if(psSrvFunc == Null)
    {
        err = ddTDA18260STREAM_ERR_BAD_PARAMETER;
    }

    if(err == TM_OK)
    {
        /* Get a driver instance */
        err = ddTDA18260Stream_GetInstance(tUnit, &pObj);
    }

    /* Check driver instance state */
    if(err == TM_OK || err == ddTDA18260STREAM_ERR_NOT_INITIALIZED)
    {
        if(pObj != Null && pObj->init == True)
        {
            err = ddTDA18260STREAM_ERR_ALREADY_SETUP;
        }
        else 
        {
            if(pObj == Null)
            {
                /* Try to allocate an instance of the driver */
                err = ddTDA18260Stream_AllocInstance(tUnit, &pObj);
                if(err != TM_OK || pObj == Null)
                {
                    err = ddTDA18260STREAM_ERR_NOT_INITIALIZED;        
                }
            }

            if(err == TM_OK)
            {
                /* Initialize the Object by default values */
                P_SIO = psSrvFunc->sIo;
                P_STIME = psSrvFunc->sTime;
                P_SDEBUG = psSrvFunc->sDebug;

                if(   psSrvFunc->sMutex.Init != Null
                   && psSrvFunc->sMutex.DeInit != Null
                   && psSrvFunc->sMutex.Acquire != Null
                   && psSrvFunc->sMutex.Release != Null)
                {
                    P_SMUTEX = psSrvFunc->sMutex;

                    err = P_SMUTEX.Init(&P_MUTEX);
                    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "Mutex_Init(0x%08X) failed.", tUnit));
                }
                
                if( psSrvFunc->dwAdditionalDataSize == sizeof( tmUnitSelect_t ) )
                {
                    pObj->tUnitW = *(tmUnitSelect_t*)(psSrvFunc->pAdditionalData);
                }
                pObj->init = True;
            }
        }
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18260Stream_DeInit:                                    */
/*                                                                            */
/* DESCRIPTION: Cleans driver setup environment.                              */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t 
tmddTDA18260Stream_DeInit
(
    tmUnitSelect_t  tUnit   /* I: Unit number */
)
{
    ptmddTDA18260Stream_Object_t  pObj = Null;
    tmErrorCode_t                 err = TM_OK;


    /* Get a driver instance */
    err = ddTDA18260Stream_GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18260Stream_GetInstance(0x%08X) failed.", tUnit));

    tmDBGPRINTEx(DEBUGLVL_BLAB, "tmddTDA18260Stream_DeInit(0x%08X)", tUnit);

    if(err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18260Stream_MutexAcquire(pObj, ddTDA18260STREAM_MUTEX_TIMEOUT);
    }

    if(err == TM_OK)
    {
        /* Release driver mutex */
        (void)ddTDA18260Stream_MutexRelease(pObj);

        if(P_SMUTEX.DeInit != Null)
        {
            if(P_MUTEX_VALID)
            {
                err = P_SMUTEX.DeInit(P_MUTEX);
            }

            P_SMUTEX.Init = Null;
            P_SMUTEX.DeInit = Null;
            P_SMUTEX.Acquire = Null;
            P_SMUTEX.Release = Null;

            P_MUTEX = Null;
        }
    }

    err = ddTDA18260Stream_DeAllocInstance(tUnit);

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18260Stream_GetSWVersion:                              */
/*                                                                            */
/* DESCRIPTION: Returns the version of this device.                           */
/*                                                                            */
/* RETURN:      TM_OK                                                         */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmddTDA18260Stream_GetSWVersion
(
    ptmSWVersion_t  pSWVersion  /* O: Receives SW Version */
)
{
    pSWVersion->compatibilityNr = TMDD_TDA18260STREAM_COMP_NUM;
    pSWVersion->majorVersionNr  = TMDD_TDA18260STREAM_MAJOR_VER;
    pSWVersion->minorVersionNr  = TMDD_TDA18260STREAM_MINOR_VER;

    return TM_OK;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18260Stream_GetSWSettingsVersion:                      */
/*                                                                            */
/* DESCRIPTION: Returns the version of this device settings.                  */
/*                                                                            */
/* RETURN:      TM_OK                                                         */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmddTDA18260Stream_GetSWSettingsVersion
(
    ptmSWSettingsVersion_t pSWSettingsVersion   /* O: Receives SW Settings Version */
)
{
    pSWSettingsVersion->customerNr      = TMDD_TDA18260STREAM_SETTINGS_CUSTOMER_NUM;
    pSWSettingsVersion->projectNr       = TMDD_TDA18260STREAM_SETTINGS_PROJECT_NUM;
    pSWSettingsVersion->majorVersionNr  = TMDD_TDA18260STREAM_SETTINGS_MAJOR_VER;
    pSWSettingsVersion->minorVersionNr  = TMDD_TDA18260STREAM_SETTINGS_MINOR_VER;

    return TM_OK;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18260Stream_Write                                      */
/*                                                                            */
/* DESCRIPTION: Sets the power state.                                         */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
tmddTDA18260Stream_Write
(
    tmUnitSelect_t  tUnit,          /* I: Unit number    */
    UInt8           uSubAddress,    /* I: sub address    */
    UInt8           uNbData,        /* I: nb of data     */
    UInt8*          pData           /* I/0: data pointer */
)
{
    ptmddTDA18260Stream_Object_t  pObj = Null;
    tmErrorCode_t                 err  = TM_OK;
    
    /* Get a driver instance */
    err = ddTDA18260Stream_GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18260Stream_GetInstance(0x%08X) failed.", tUnit));
    
    if(err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18260Stream_MutexAcquire(pObj, ddTDA18260STREAM_MUTEX_TIMEOUT);
    }
    
    if(err == TM_OK)
    {
        err = ddTDA18260Stream_Write(pObj, uSubAddress, uNbData, pData);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18260Stream_Write(0x%08X, 0x%02X, %d) failed.", tUnit, uSubAddress, uNbData));

        /* Release driver mutex */
        (void)ddTDA18260Stream_MutexRelease(pObj);
    }
    
    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18260Stream_Read                                       */
/*                                                                            */
/* DESCRIPTION: Sets the power state.                                         */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
tmddTDA18260Stream_Read
(
    tmUnitSelect_t  tUnit,          /* I: Unit number    */
    UInt8           uSubAddress,    /* I: sub address    */
    UInt8           uNbData,        /* I: nb of data     */
    UInt8*          pData           /* I/0: data pointer */
)
{
    ptmddTDA18260Stream_Object_t  pObj = Null;
    tmErrorCode_t                 err  = TM_OK;
    
    /* Get a driver instance */
    err = ddTDA18260Stream_GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18260Stream_GetInstance(0x%08X) failed.", tUnit));

    if(err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18260Stream_MutexAcquire(pObj, ddTDA18260STREAM_MUTEX_TIMEOUT);
    }
    
    if(err == TM_OK)
    {
        
        err = ddTDA18260Stream_Read(pObj, uSubAddress, uNbData, pData);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18260Stream_Read(0x%08X, 0x%02X, %d) failed.", tUnit, uSubAddress, uNbData));

        /* Release driver mutex */
        (void)ddTDA18260Stream_MutexRelease(pObj);
    }
    
    return err;
}


/*============================================================================*/
/* Internal functions:                                                        */
/*============================================================================*/

/*============================================================================*/
/* FUNCTION:    ddTDA18260Stream_Write:                                       */
/*                                                                            */
/* DESCRIPTION: Writes data in TDA18260.                                      */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t 
ddTDA18260Stream_Write
(
    ptmddTDA18260Stream_Object_t  pObj,           /* I: Driver object  */
    UInt8                         uSubAddress,    /* I: sub address    */
    UInt8                         uNbData,        /* I: nb of data     */
    UInt8*                        pData           /* I/0: data pointer */
)
{
    tmErrorCode_t   err = TM_OK;
    UInt8*          pRegMap = Null;
    UInt8           subAddress = 0x00;

    if(P_OBJ_VALID)
    {
        /* pRegMap initialization */
        if(pData == Null)
        {
#ifdef _TMDD_TDA18260_REGMAP_DEFINED
            pRegMap = (UInt8 *)&(pObj->RegMap) + uSubAddress * sizeof(pObj->RegMap.Reg_power_ctrl);
#else
            err = ddTDA18260STREAM_ERR_BAD_PARAMETER;
#endif
        }
        else
        {
            pRegMap = pData;
        }

        /* Set sub-address */
        subAddress = pObj->uBaseAddress + uSubAddress;

        if(P_SIO_WRITE_VALID && err == TM_OK)
        {
            /* Write data to TDA18260 */
            err = P_SIO.Write(pObj->tUnitW, TMDD_TDA18260STREAM_WRITE_SUB_ADDR_SZ, &subAddress, uNbData, pRegMap);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "IO_Write(0x%08X, %d, 0x%02X, %d, %c) failed.", pObj->tUnitW, TMDD_TDA18260STREAM_WRITE_SUB_ADDR_SZ, uSubAddress, uNbData, (pData==Null)?'I':'E'));
        }
        else
        {
            err = ddTDA18260STREAM_ERR_NOT_INITIALIZED;
        }
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    ddTDA18260Stream_Read:                                        */
/*                                                                            */
/* DESCRIPTION: Reads data in TDA18260.                                       */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t 
ddTDA18260Stream_Read
(
    ptmddTDA18260Stream_Object_t  pObj,           /* I: Driver object  */
    UInt8                         uSubAddress,    /* I: sub address    */
    UInt8                         uNbData,        /* I: nb of data     */
    UInt8*                        pData           /* I/0: data pointer */
)
{
    tmErrorCode_t   err = TM_OK;
    UInt8*          pRegMap = Null;
    UInt8           subAddress = 0x00;

    if(P_OBJ_VALID)
    {
        /* pRegMap initialization */
        if(pData == Null)
        {
#ifdef _TMDD_TDA18260_REGMAP_DEFINED
            pRegMap = (UInt8 *)&(pObj->RegMap) + uSubAddress * sizeof(pObj->RegMap.Reg_power_ctrl);
#else
            err = ddTDA18260STREAM_ERR_BAD_PARAMETER;
#endif
        }
        else
        {
            pRegMap = pData;
        }

        /* Set sub-address */
        subAddress = pObj->uBaseAddress + uSubAddress;

        if(P_SIO_READ_VALID && err == TM_OK)
        {
            /* Read data from TDA18260 */
            err = P_SIO.Read(pObj->tUnitW, TMDD_TDA18260STREAM_READ_SUB_ADDR_SZ, &subAddress, uNbData, pRegMap);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "IO_Read(0x%08X, %d, 0x%02X, %d, %c) failed.", pObj->tUnitW, TMDD_TDA18260STREAM_READ_SUB_ADDR_SZ, uSubAddress, uNbData, (pData==Null)?'I':'E'));
        }
        else
        {
            err = ddTDA18260STREAM_ERR_NOT_INITIALIZED;
        }
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    ddTDA18260Stream_Wait:                                        */
/*                                                                            */
/* DESCRIPTION: Waits for requested time.                                     */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t 
ddTDA18260Stream_Wait
(
    ptmddTDA18260Stream_Object_t  pObj,   /* I: Driver object    */
    UInt32                        Time    /* I: time to wait for */
)
{
    tmErrorCode_t   err = ddTDA18260STREAM_ERR_NOT_INITIALIZED;

    if(P_STIME_WAIT_VALID)
    {
        /* wait Time ms */
        err = P_STIME.Wait(pObj->tUnit, Time);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TIME_Wait(0x%08X, %d) failed.", pObj->tUnitW, Time));
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    ddTDA18260Stream_MutexAcquire:                                */
/*                                                                            */
/* DESCRIPTION: Acquires driver mutex.                                        */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
ddTDA18260Stream_MutexAcquire
(
    ptmddTDA18260Stream_Object_t  pObj,
    UInt32                        timeOut
)
{
    tmErrorCode_t   err = TM_OK;

    if( P_SMUTEX_ACQUIRE_VALID && P_MUTEX_VALID )
    {
        err = pObj->sMutex.Acquire(P_MUTEX, timeOut);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "Mutex_Acquire(0x%08X, %d) failed.", pObj->tUnitW, timeOut));
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    ddTDA18260Stream_MutexRelease:                                */
/*                                                                            */
/* DESCRIPTION: Releases driver mutex.                                        */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
ddTDA18260Stream_MutexRelease
(
    ptmddTDA18260Stream_Object_t  pObj    /* I: Driver object */
)
{
    tmErrorCode_t   err = TM_OK;

    if( P_SMUTEX_RELEASE_VALID && P_MUTEX_VALID )
    {
        err = pObj->sMutex.Release(P_MUTEX);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "Mutex_Release(0x%08X) failed.", pObj->tUnitW));
    }

    return err;
}
#endif

