/******************************************************************************
 *                           COPYRIGHT 2006 IWEDIA                            *
 ******************************************************************************
 *
 * MODULE NAME: CFG XPEL
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/product/comedia/branches/idecode_hd/build/inc/xpel_cfg.h $
 *            $Rev: 244 $
 *            $Date: 2011-08-18 18:33:39 +0200 (Thu, 18 Aug 2011) $
 *
 * PUBLIC
 *
 * DESCRIPTION
 *
 *****************************************************************************/

#ifndef _XPEL_CFG_H_
#define _XPEL_CFG_H_

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************/
/*               typedef                               */
/*******************************************************/

#define		xPEL_CFG_NB_MAX_RECORD		50
#define     xPEL_CFG_RECORD_TS			1
#define		xPEL_CFG_FREE_SPACE_WARNING 100 

/*This value defined the roles of record if the end user can record nb demux service or nb demux ts*/
/*===========================================================================
 *
 * xPEL_CFG_VCRIsAvailable
 *
 * Parameters:
 *
 * Description:
 *      return if VCR available
 *
 * Returns:
 *      nothing
 *
 *===========================================================================*/
bool xPEL_CFG_VCRIsAvailable(void);

/*===========================================================================
 *
 * xPEL_CFG_ReminderIsAvailable
 *
 * Parameters:
 *
 * Description:
 *      return if Reminder available
 *
 * Returns:
 *      nothing
 *
 *===========================================================================*/
bool xPEL_CFG_ReminderIsAvailable(void);

/*===========================================================================
 *
 * xPEL_CFG_InternalHDDIsAvailable
 *
 * Parameters:
 *
 * Description:
 *      return if Internal HDD available
 *
 * Returns:
 *      nothing
 *
 *===========================================================================*/
bool xPEL_CFG_InternalHDDIsAvailable(void);

/*===========================================================================
 *
 * xPEL_CFG_ExternalHDDIsAvailable
 *
 * Parameters:
 *
 * Description:
 *      return if External HDD available
 *
 * Returns:
 *      nothing
 *
 *===========================================================================*/
bool xPEL_CFG_ExternalHDDIsAvailable(void);

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
tXPEL_Target xPEL_CFG_DefaultTargetGet( void );


/*===========================================================================
 *
 * xPEL_CFG_GetMaxRecord
 *
 * Parameters:
 *
 * Description:
 *      return nb max record can scheduled 
 *
 * Returns:
 *      nothing
 *
 *===========================================================================*/
uint32_t 	xPEL_CFG_GetMaxRecord(void);	

bool			xPEL_CFG_InfiniteRecordIsStoppedInStandBy(void);

uint32_t 	xPEL_CFG_GetFreeWarning(void);

/*===========================================================================
 *
 * xPEL_CFG_GetDurationInMinOfInfiniteRecord
 *
 * Parameters:
 *
 * Description:
 *      return the duration of a "infinite" record
 *
 * Returns:
 *      duration of a "infinite" record
 *
 *===========================================================================*/
uint32_t	xPEL_CFG_GetDurationInMinOfInfiniteRecord( void );

/*===========================================================================
 *
 * xPEL_CFG_GetNbSecondBeforeReminderStart
 *
 * Parameters:
 *
 * Description:
 *      return the duration in second to display reminder popup before real start
 *      time of the reminder
 * Returns:
 *
 *===========================================================================*/
uint32_t xPEL_CFG_GetNbSecondBeforeReminderStart(void);

/*===========================================================================
 *
 * xPEL_CFG_GetNbSecondBeforeTimerStart
 *
 * Parameters:
 *
 * Description:
 *      return the duration in second to display timer popup before real start
 *      time of the timer
 * Returns:
 *
 *===========================================================================*/
uint32_t xPEL_CFG_GetNbSecondBeforeTimerStart(void);

#ifdef __cplusplus
}
#endif

#endif /* _XPEL_CFG_H_ */
