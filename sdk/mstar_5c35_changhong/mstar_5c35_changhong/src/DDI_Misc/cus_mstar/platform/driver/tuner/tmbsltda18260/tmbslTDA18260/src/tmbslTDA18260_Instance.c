/*
  Copyright (C) 2006-2009 NXP B.V., All Rights Reserved.
  This source code and any compilation or derivative thereof is the proprietary
  information of NXP B.V. and is confidential in nature. Under no circumstances
  is this software to be  exposed to or placed under an Open Source License of
  any type without the expressed written permission of NXP B.V.
 *
 * \file          tmbslTDA18260_Instance.c
 *
 *                0
 *
 * \date          %modify_time%
 *
 * \brief         Describe briefly the purpose of this file.
 *
 * REFERENCE DOCUMENTS :
 *                TDA18260_Driver_User_Guide.pdf
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
#include "tmddTDA18260Shared.h"
#include "tmddTDA18260Stream.h"

#include "tmbslTDA18260.h"
#include <tmbslTDA18260_InstanceCustom.h>
#include "tmbslTDA18260_Local.h"
#include "tmbslTDA18260_Instance.h"


/*============================================================================*/
/* Global data:                                                               */
/*============================================================================*/

/* Driver instance */
tmbslTDA18260_Object_t gTDA18260_Instance[] = 
{
    {
        (tmUnitSelect_t)(-1),               /* Unit not set */
        (tmUnitSelect_t)(-1),               /* UnitW not set */
        Null,                               /* pMutex */
        False,                              /* init (instance initialization default) */
        {                                   /* sRWFunc */
            Null,
            Null
        },
        {                                   /* sTime */
            Null,
            Null
        },
        {                                   /* sDebug */
            Null
        },
        {                                   /* sMutex */
            Null,
            Null,
            Null,
            Null
        },
        tmTDA18260_SW_CLOSE,                /* lpfSwitchState    */                         \
        tmTDA18260_SW_CLOSE,                /* hpfSwitchState    */                         \
        tmTDA18260_SW_CLOSE,                /* bypassSwitchState */                         \
        0,                                  /* rf                */                         \
        0,                                  /* freqCut           */                         \
        False,                              /* resetCalledPreviously */                     \
        tmTDA18260_PowerTransitionMode_Smooth,   /* ePowerTransitionMode */                             \
        (tmUnitSelect_t)(-1),               /* tUnitOtherStream */ \
        tmTDA18260_StreamInvalid,           /* StreamType; */ \
        TMBSL_TDA18260_INSTANCE_CUSTOM_0,   /* Instance Customizable Settings */ \
    },
    {
        (tmUnitSelect_t)(-1),               /* Unit not set */
        (tmUnitSelect_t)(-1),               /* UnitW not set */
        Null,                               /* pMutex */
        False,                              /* init (instance initialization default) */
        {                                   /* sRWFunc */
            Null,
            Null
        },
        {                                   /* sTime */
            Null,
            Null
        },
        {                                   /* sDebug */
            Null
        },
        {                                   /* sMutex */
            Null,
            Null,
            Null,
            Null
        },
        tmTDA18260_SW_CLOSE,                /* lpfSwitchState    */                         \
        tmTDA18260_SW_CLOSE,                /* hpfSwitchState    */                         \
        tmTDA18260_SW_CLOSE,                /* bypassSwitchState */                         \
        0,                                  /* rf                */                         \
        0,                                  /* freqCut           */                         \
        False,                              /* resetCalledPreviously */                     \
        tmTDA18260_PowerTransitionMode_Smooth,   /* ePowerTransitionMode */                             \
        (tmUnitSelect_t)(-1),               /* tUnitOtherStream */ \
        tmTDA18260_StreamInvalid,           /* StreamType; */ \
        TMBSL_TDA18260_INSTANCE_CUSTOM_1,   /* Instance Customizable Settings */ \
    },
    {
        (tmUnitSelect_t)(-1),               /* Unit not set */
        (tmUnitSelect_t)(-1),               /* UnitW not set */
        Null,                               /* pMutex */
        False,                              /* init (instance initialization default) */
        {                                   /* sRWFunc */
            Null,
            Null
        },
        {                                   /* sTime */
            Null,
            Null
        },
        {                                   /* sDebug */
            Null
        },
        {                                   /* sMutex */
            Null,
            Null,
            Null,
            Null
        },
        tmTDA18260_SW_CLOSE,                /* lpfSwitchState    */                         \
        tmTDA18260_SW_CLOSE,                /* hpfSwitchState    */                         \
        tmTDA18260_SW_CLOSE,                /* bypassSwitchState */                         \
        0,                                  /* rf                */                         \
        0,                                  /* freqCut           */                         \
        False,                              /* resetCalledPreviously */                     \
        tmTDA18260_PowerTransitionMode_Smooth,   /* ePowerTransitionMode */                             \
        (tmUnitSelect_t)(-1),               /* tUnitOtherStream */ \
        tmTDA18260_StreamInvalid,           /* StreamType; */ \
        TMBSL_TDA18260_INSTANCE_CUSTOM_2,   /* Instance Customizable Settings */ \
    },
    {
        (tmUnitSelect_t)(-1),               /* Unit not set */
        (tmUnitSelect_t)(-1),               /* UnitW not set */
        Null,                               /* pMutex */
        False,                              /* init (instance initialization default) */
        {                                   /* sRWFunc */
            Null,
            Null
        },
        {                                   /* sTime */
            Null,
            Null
        },
        {                                   /* sDebug */
            Null
        },
        {                                   /* sMutex */
            Null,
            Null,
            Null,
            Null
        },
        tmTDA18260_SW_CLOSE,                /* lpfSwitchState    */                         \
        tmTDA18260_SW_CLOSE,                /* hpfSwitchState    */                         \
        tmTDA18260_SW_CLOSE,                /* bypassSwitchState */                         \
        0,                                  /* rf                */                         \
        0,                                  /* freqCut           */                         \
        False,                              /* resetCalledPreviously */                     \
        tmTDA18260_PowerTransitionMode_Smooth,   /* ePowerTransitionMode */                             \
        (tmUnitSelect_t)(-1),               /* tUnitOtherStream */ \
        tmTDA18260_StreamInvalid,           /* StreamType; */ \
        TMBSL_TDA18260_INSTANCE_CUSTOM_3,   /* Instance Customizable Settings */ \
    },
    {
        (tmUnitSelect_t)(-1),               /* Unit not set */
        (tmUnitSelect_t)(-1),               /* UnitW not set */
        Null,                               /* pMutex */
        False,                              /* init (instance initialization default) */
        {                                   /* sRWFunc */
            Null,
            Null
        },
        {                                   /* sTime */
            Null,
            Null
        },
        {                                   /* sDebug */
            Null
        },
        {                                   /* sMutex */
            Null,
            Null,
            Null,
            Null
        },
        tmTDA18260_SW_CLOSE,                /* lpfSwitchState    */                         \
        tmTDA18260_SW_CLOSE,                /* hpfSwitchState    */                         \
        tmTDA18260_SW_CLOSE,                /* bypassSwitchState */                         \
        0,                                  /* rf                */                         \
        0,                                  /* freqCut           */                         \
        False,                              /* resetCalledPreviously */                     \
        tmTDA18260_PowerTransitionMode_Smooth,   /* ePowerTransitionMode */                             \
        (tmUnitSelect_t)(-1),               /* tUnitOtherStream */ \
        tmTDA18260_StreamInvalid,           /* StreamType; */ \
        TMBSL_TDA18260_INSTANCE_CUSTOM_4,   /* Instance Customizable Settings */ \
    },
    {
        (tmUnitSelect_t)(-1),               /* Unit not set */
        (tmUnitSelect_t)(-1),               /* UnitW not set */
        Null,                               /* pMutex */
        False,                              /* init (instance initialization default) */
        {                                   /* sRWFunc */
            Null,
            Null
        },
        {                                   /* sTime */
            Null,
            Null
        },
        {                                   /* sDebug */
            Null
        },
        {                                   /* sMutex */
            Null,
            Null,
            Null,
            Null
        },
        tmTDA18260_SW_CLOSE,                /* lpfSwitchState    */                         \
        tmTDA18260_SW_CLOSE,                /* hpfSwitchState    */                         \
        tmTDA18260_SW_CLOSE,                /* bypassSwitchState */                         \
        0,                                  /* rf                */                         \
        0,                                  /* freqCut           */                         \
        False,                              /* resetCalledPreviously */                     \
        tmTDA18260_PowerTransitionMode_Smooth,   /* ePowerTransitionMode */                             \
        (tmUnitSelect_t)(-1),               /* tUnitOtherStream */ \
        tmTDA18260_StreamInvalid,           /* StreamType; */ \
        TMBSL_TDA18260_INSTANCE_CUSTOM_5,   /* Instance Customizable Settings */ \
    }
};

/*============================================================================*/
/* Internal functions:                                                        */
/*============================================================================*/

/*============================================================================*/
/* FUNCTION:    TDA18260_AllocInstance:                                       */
/*                                                                            */
/* DESCRIPTION: Allocates an instance.                                        */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
TDA18260_AllocInstance
(
    tmUnitSelect_t              tUnit,      /* I: Unit number */
    pptmbslTDA18260_Object_t    ppDrvObject /* I: Device Object */
)
{ 
    tmErrorCode_t           err = TDA18260_ERR_BAD_UNIT_NUMBER;
    ptmbslTDA18260_Object_t pObj = Null;
    UInt32                  uLoopCounter = 0;    

    /* Find a free instance */
    for (uLoopCounter = 0; uLoopCounter<TDA18260_MAX_UNITS; uLoopCounter++)
    {
        pObj = &gTDA18260_Instance[uLoopCounter];
        if (pObj->init == False)
        {
            pObj->tUnit = tUnit;
            pObj->tUnitW = tUnit;

            *ppDrvObject = pObj;
            err = TM_OK;
            break;
        }
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    TDA18260_DeAllocInstance:                                     */
/*                                                                            */
/* DESCRIPTION: Deallocates an instance.                                      */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
TDA18260_DeAllocInstance
(
    tmUnitSelect_t tUnit   /* I: Unit number */
)
{     
    tmErrorCode_t           err  = TDA18260_ERR_BAD_UNIT_NUMBER;
    ptmbslTDA18260_Object_t pObj = Null;

    /* check input parameters */
    err = TDA18260_GetInstance(tUnit, &pObj);

    /* check driver state */
    if (err == TM_OK)
    {
        if (pObj == Null || pObj->init == False)
        {
            err = TDA18260_ERR_NOT_INITIALIZED;
        }
    }

    if ((err == TM_OK) && (pObj != Null)) 
    {
        if(pObj->init)
        {
            pObj->init--;
        }
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    TDA18260_GetInstance:                                         */
/*                                                                            */
/* DESCRIPTION: Gets an instance.                                             */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
TDA18260_GetInstance
(
    tmUnitSelect_t              tUnit,       /* I: Unit number   */
    pptmbslTDA18260_Object_t    ppDrvObject  /* I: Device Object */
)
{     
    tmErrorCode_t           err = TDA18260_ERR_NOT_INITIALIZED;
    ptmbslTDA18260_Object_t pObj = Null;
    UInt32                  uLoopCounter = 0;    

    /* get instance */
    for (uLoopCounter = 0; uLoopCounter<TDA18260_MAX_UNITS; uLoopCounter++)
    {
        pObj = &gTDA18260_Instance[uLoopCounter];
        if ( pObj->init && ( (pObj->tUnit == GET_INDEX_TYPE_TUNIT(tUnit)) || (pObj->tUnitOtherStream == GET_INDEX_TYPE_TUNIT(tUnit)) ) )
        {
            pObj->tUnitW = tUnit;

            *ppDrvObject = pObj;
            err = TM_OK;
            break;
        }
    }

    return err;
}
#endif

