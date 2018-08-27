/******************************************************************************
 *                    COPYRIGHT 2004 IWEDIA TECHNOLOGIES                      *
 ******************************************************************************
 *
 * MODULE NAME: TRIGGER
 *
 * FILE NAME: $URL: http://10.50.1.219/svncomedia/idecode/sources/packages/trigger/core/trunk/inc/ssu_enhanced_types.h $
 *            $Rev: 6180 $
 *            $Date: 2011-06-01 15:26:55 +0200 (mer., 01 juin 2011) $
 *
 * PUBLIC
 *
 * DESCRIPTION: ssu enhanced types header
 *
 *****************************************************************************/

#ifndef _SSU_ENH_TYPES_H_
#define _SSU_ENH_TYPES_H_

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************/
/*              Includes                               */
/*******************************************************/

/*******************************************************/
/*              Defines                                */
/*******************************************************/
#define MAX_CHAR_SZ 256

/*******************************************************/
/*              Macros                                 */
/*******************************************************/

/*******************************************************/
/*              Typedefs                               */
/*******************************************************/

/* SSU Enhanced Profile */
/* -------------------- */
typedef enum
{
	eTRIG_ENHANCED_NO_ERROR = 0,
	eTRIG_ENHANCED_ERROR,
	eTRIG_ENHANCED_NOT_FOUND,
	eTRIG_ENHANCED_UP_TO_DATE
} tTRIG_ENHANCED_Error;

/* Update Descriptor info */	
typedef struct
{
    uint8_t		ucUpdateFlag;
    uint8_t		ucUpdateMethod;
    uint8_t		ucUpdatePriority;
	uint8_t		ucPrivateDataByteLength;
	uint8_t		acPrivateDataBytes[MAX_CHAR_SZ];
} tTRIG_ENHANCED_UpdateDescInfo;

/* Scheduling Descriptor info */
typedef struct
{
    uint16_t    Year;
    uint8_t		Month;
    uint8_t		Day;
} tTRIG_ENHANCED_Date;

typedef struct
{
    uint8_t	Hour;		/* 0..23 */
    uint8_t	Minute;
    uint8_t	Second;
} tTRIG_ENHANCED_Time;

typedef struct
{
    tTRIG_ENHANCED_Date	stStartDate;
    tTRIG_ENHANCED_Time	stStartTime;
    tTRIG_ENHANCED_Date	stEndDate;
    tTRIG_ENHANCED_Time	stEndTime;
    uint8_t			    ucFinalAvailability;
    uint8_t			    ucPeriodicityFlag;
    uint8_t			    ucPeriodUnit;
    uint8_t			    ucDurationUnit;
    uint8_t			    ucEstimatedCycleTimeUnit;
    uint8_t			    ucPeriod ;
    uint8_t			    ucDuration;
    uint8_t			    ucEstimatedCycleTime;
	uint8_t			    ucPrivateDataByteLength;
	uint8_t			    acPrivateDataBytes[MAX_CHAR_SZ];
} tTRIG_ENHANCED_SchedulingDescInfo;

/* Message Descriptor info */
typedef struct
{
	uint8_t		ucDescriptorNumber;
	uint8_t		ucLastDescriptorNumber;
	uint32_t	uiISO639LanguageCode;
	uint8_t		ucTextCharLength;
	uint8_t		acTextChar[MAX_CHAR_SZ];
} tTRIG_ENHANCED_MessageDescInfo;

/* SSU Event Name Descriptor info */
typedef struct
{
	uint32_t	uiISO639LanguageCode;
	/* Name char data */
	uint8_t		ucNameCharLength;
	uint8_t		acNameChar[MAX_CHAR_SZ];
	/* Text char data */
	uint8_t		ucTextCharLength;
	uint8_t		acTextChar[MAX_CHAR_SZ];
} tTRIG_ENHANCED_SSUEventNameDescInfo;

/* Status */
typedef struct
{
	bool		bScheduling_descriptor_found; /* true if valid scheduling descriptor found in UNT */
	bool		bUpdate_descriptor_found;     /* true if valid update descriptor found in UNT */
	uint8_t		ucSchedulingTableSize;        /* input  : max number of scheduling to return    */
	                                          /* output : current number of scheduling returned */
} tTRIG_ENHANCED_Desc_status;

/* Start delay (negative if already started) and remaining duration   */ 
/* for most relevant scheduling descriptor's window of SSU broadcast */
typedef struct
{
	int32_t		iCurrentWindowsStartLaps;           /* delay compared to current time (in seconds)*/
	int32_t		iCurrentWindowsRemainingDuration;   /* remaining duration before window end time  */
} tTRIG_ENHANCED_CurrentSchedulingWindow;

/* Return type to client */
typedef struct
{
	tTRIG_ENHANCED_Desc_status			    stStatus;
	tTRIG_ENHANCED_UpdateDescInfo		    stUpdateDescInfo;
	tTRIG_ENHANCED_CurrentSchedulingWindow  stCurrentSchedulingWindow;
	tTRIG_ENHANCED_SchedulingDescInfo	    *pstSchedulingDescInfoList;
} tTRIG_ENHANCED_Info;

/* Callback definition */
typedef void (* fTRIG_ENHANCED_MessageCallback) (tTRIG_ENHANCED_MessageDescInfo *pMessageDescInfo);
typedef void (* fTRIG_ENHANCED_SSUEventNameCallback) (tTRIG_ENHANCED_SSUEventNameDescInfo *pSSUEventNameDescInfo);

typedef struct
{
	fTRIG_ENHANCED_MessageCallback		fMessageCallBack;
	fTRIG_ENHANCED_SSUEventNameCallback	fSSUEventNameCallBack;
} tTRIG_ENHANCED_CallBacks;

/*******************************************************/
/*              Variables Declarations (IMPORT)        */
/*******************************************************/

/*******************************************************/
/*              Functions Declarations                 */
/*******************************************************/


#ifdef __cplusplus
}
#endif

#endif /*_SSU_ENH_TYPES_H_*/
