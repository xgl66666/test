/*****************************************************************************
 *                         COPYRIGHT 2006 IWEDIA TECHNOLOGIES                *
 *****************************************************************************
 *
 * MODULE NAME: XPEL MODULE
 *
 * FILE NAME:   XPEL_CFG.C
 *   $URL: http://svnsrv/svn/iwedia-ext/product/comedia/branches/idecode_hd/build/src/xpel_cfg.c $
 *   $Rev: 244 $
 *   $Date: 2011-08-18 18:33:39 +0200 (Thu, 18 Aug 2011) $
 *
 * DESCRIPTION
 *
 *****************************************************************************/

/*****************************************************************************
 *  Includes                                                                 *
 *****************************************************************************/
/********************************************************/
#include <string.h>

#include "crules.h"

#include "pinfodbase.h"
#include "date_time.h"
#include "xpel.h"

#include "xpel_cfg.h"

/*****************************************************************************
 *  Defines                                                                  *
 *****************************************************************************/
/*****************************************************************************
 *  Macros                                                                   *
 *****************************************************************************/
/*****************************************************************************
 *  Structures                                                               *
 *****************************************************************************/
/*****************************************************************************
 *  Global Variables (GLOBAL/IMPORT)                                         *
 *****************************************************************************/
IMPORT	uint8_t 						SETUP_ReminderNotificationTiming;


/*****************************************************************************
 *  Local File Variables (LOCAL)                                             *
 *****************************************************************************/
/*****************************************************************************
 *  Global Functions Declarations (IMPORT)                                     *
 *****************************************************************************/
/*****************************************************************************
 *  Local Functions Declarations (LOCAL)                                     *
 *****************************************************************************/
/*****************************************************************************
 *  Functions Definitions (LOCAL/GLOBAL)                                     *
 *****************************************************************************/
bool xPEL_CFG_VCRIsAvailable(void)
{
    /* Set it to TRUE if you want, as in set-top box, VCR timers available. */
	return FALSE;
}

bool xPEL_CFG_ReminderIsAvailable(void)
{
	return TRUE;
}

/*===========================================================================
 *
 * xPEL_CFG_DefaultTargetGet
 *
 * Parameters:
 *
 * Description:
 *      defines the default target when a new xpel prog is going to be added.
 *      (returning NONE will force the user to set explicitly which target
 *       he prefers)
 *
 * Returns:
 *      the default target
 *
 *===========================================================================*/
tXPEL_Target xPEL_CFG_DefaultTargetGet( void )
{
    return eXPEL_TARGET_EXTERNAL_HDD;
}

#ifdef PACKAGE_PVR
bool xPEL_CFG_InternalHDDIsAvailable(void)
{
	return FALSE;
}

bool xPEL_CFG_ExternalHDDIsAvailable(void)
{
	return TRUE;
}
#endif /* PACKAGE_PVR */

uint32_t xPEL_CFG_GetMaxRecord(void)
{
	return xPEL_CFG_NB_MAX_RECORD;
}

bool xPEL_CFG_InfiniteRecordIsStoppedInStandBy(void)
{
#if defined(SYS_STANDBY_AV_OFF)
	return false;
#else
	return true;
#endif
}

uint32_t xPEL_CFG_GetFreeWarning(void)
{
	return xPEL_CFG_FREE_SPACE_WARNING;
}

uint32_t xPEL_CFG_GetDurationInMinOfInfiniteRecord(void)
{
	return 12*60;
}
uint32_t xPEL_CFG_GetNbSecondBeforeReminderStart(void)
{
	if(SETUP_ReminderNotificationTiming == 0) 
		return 30*1;
    else if(SETUP_ReminderNotificationTiming == 1) 
		return 60*1;
	else if(SETUP_ReminderNotificationTiming == 2) 
		return 90*1;
}
uint32_t xPEL_CFG_GetNbSecondBeforeTimerStart(void)
{
    return 30*1;
}
