/******************************************************************************
 *                           COPYRIGHT 2005 IWEDIA                            *
 ******************************************************************************
 *
 * MODULE NAME: DATETIME CFG 
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/product/comedia/branches/idecode_hd/build/inc/datetime_cfg.h $
 *            $Rev: 244 $
 *            $Date: 2011-08-18 18:33:39 +0200 (Thu, 18 Aug 2011) $
 *
 * PUBLIC
 *
 * DESCRIPTION
 *
 *****************************************************************************/

#ifndef _DATETIME_CFG_H_
#define _DATETIME_CFG_H_

#ifdef __cplusplus
extern "C" {
#endif


/********************************************************/
/*														*/
/********************************************************/
#define kDATETIME_CFG_NumberOfHourMode	3

typedef enum
{
    eDATETIME_CFG_MODE_SEMI_AUTOMATIC=0,
    eDATETIME_CFG_MODE_MANUAL,
    eDATETIME_CFG_MODE_FULL_AUTOMATIC
}tDATETIME_CFG_HourMode;

/**
 * @brief
 * Next time offset of TOT behavior */
typedef struct
{
	bool isManaged; /**<  is next time offset managed */
	time_t offsetValidityDurationInSecond; /**<  validity duration of next time offset : difference between time_of_change and current time */

}tDATETIME_CFG_NextTimeOffsetMode;
/********************************************************/
/*														*/
/********************************************************/

void DATETIME_CFG_GetAvailableHourMode(bool  *AvailableHourModeList);
tDATETIME_CFG_HourMode DATETIME_CFG_GetDefaultHourMode(void);
void DATETIME_CFG_GetMinimumDateToSet(uint16_t *year,uint8_t *month,uint8_t *day  );
bool DATETIME_CFG_IsTimeOffsetChangeValid(tDAT_Date localDate,tDAT_Time localTime, tDAT_Date nextDateOffset, tDAT_Time nextTimeOffset);
	
#ifdef __cplusplus
}
#endif

#endif /* _DATETIME_CFG_H_ */
