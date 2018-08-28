/********************************************************************************
 *                       COPYRIGHT 2005 IWEDIA                                  *
 ********************************************************************************
 *
 * MODULE NAME: DATE TIME CFG
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/product/comedia/branches/idecode_hd/build/src/datetime_cfg.c $
 *            $Rev: 244 $
 *            $Date: 2011-08-18 18:33:39 +0200 (Thu, 18 Aug 2011) $
 *
 *
 *******************************************************************************/

/********************************************************/
/*                Includes                              */
/********************************************************/
#include "crules.h"
#include "time.h"
#include "date_time.h"
#include "datetime_cfg.h"





/********************************************************/
/*              Defines                                 */
/********************************************************/
/**
 * @brief
 * Next time offset of TOT default behavior */
LOCAL tDATETIME_CFG_NextTimeOffsetMode nextTimeOffsetMode = {
#ifdef IMCL
		FALSE, /* next time offset not to be take into account */
		0
#else
		TRUE,
		86400 /* 1 Day in second */
#endif
};
/********************************************************/
/*              Local Functions Declarations (LOCAL)    */
/*******************************************************/

/********************************************************/
/*              Functions Definitions (LOCAL/GLOBAL)    */
/********************************************************/

/*===========================================================================
 *
 * DATETIME_CFG_GetAvailableHourMode
 *
 * Parameters:
 *      index
 *
 * Description:
 *      Return the available hour mode
 *
 * Returns:
 *      
 *
 *===========================================================================*/
void DATETIME_CFG_GetAvailableHourMode(bool  *AvailableHourModeList)
{
	/* FALSE : Not Available 	*/
	/* TRUE  : Available		*/
	AvailableHourModeList[eDATETIME_CFG_MODE_SEMI_AUTOMATIC]	= FALSE;
	AvailableHourModeList[eDATETIME_CFG_MODE_MANUAL]			= TRUE;
	AvailableHourModeList[eDATETIME_CFG_MODE_FULL_AUTOMATIC]	= TRUE;
	
	return;
}

/*===========================================================================
 *
 * DATETIME_CFG_GetDefaultHourMode
 *
 * Parameters:
 *      none
 *
 * Description:
 *      Return the default hour mode
 *
 * Returns:
 *      
 *
 *===========================================================================*/
tDATETIME_CFG_HourMode DATETIME_CFG_GetDefaultHourMode(void)
{
    return eDATETIME_CFG_MODE_FULL_AUTOMATIC;
}

/*===========================================================================
 *
 * DATETIME_CFG_GetMinimumDateToSet
 *
 * Parameters: 
 *      year: (YYYY)
 *      month: must be 1 (january) or more
 *      day: must be 1 or more
 *
 * Description:
 *      Indicates the minimum date
 *
 * Returns:
 *      
 *
 *===========================================================================*/
void DATETIME_CFG_GetMinimumDateToSet(uint16_t *year,uint8_t *month,uint8_t *day)
{
	*year = 2006; /* must be 1999 or more */
	*month = 1; /* must be 1 (january) or more */
	*day = 1; /* must be 1 or more */
	return;
}

/** Determinating if TimeOffsetChange is valid or not.
 *
 * @param[in] localDate is a ::tDAT_Datee indicating current local Date.
 * @param[in] nextDateOffset is a ::tDAT_Date indicating Date when nextOffset must be apply.
 * @param[in] localTime is a ::tDAT_Time indicating current local Time.
 * @param[in] nextTimeOffset is a ::tDAT_Time indicating Time when nextOffset must be apply.
 *
 * @return TRUE if next offset must be taken into account.
 * @return FALSE otherwise.
 *
 * @note returned value depend of specific customer customization
 */
bool DATETIME_CFG_IsTimeOffsetChangeValid(tDAT_Date localDate,tDAT_Time localTime, tDAT_Date nextDateOffset, tDAT_Time nextTimeOffset)
{

	time_t localDateTimeInSecond;
	time_t nextDateTimeOffsetInSecond;

	if(!nextTimeOffsetMode.isManaged)
	{
		/* nextDateTimeOffset is not managed */
		return FALSE;
	}
	else
	{
		localDateTimeInSecond = DAT_ConvertDateInSecond(localDate,localTime);
		nextDateTimeOffsetInSecond = DAT_ConvertDateInSecond(nextDateOffset,nextTimeOffset);

		if(nextDateTimeOffsetInSecond > localDateTimeInSecond)
		{
			/* nextDateTimeOffset is in the future: not apply */
			return FALSE;
		}
		else if( (nextDateTimeOffsetInSecond + nextTimeOffsetMode.offsetValidityDurationInSecond)< localDateTimeInSecond)
		{
			/* nextDateTimeOffset is too far in the past: not apply */
			return FALSE;
		}
		else
		{
			return TRUE;
		}
	}
}
