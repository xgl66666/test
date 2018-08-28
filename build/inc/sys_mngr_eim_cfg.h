/******************************************************************************
 *                    COPYRIGHT 2006 IWEDIA TECHNOLOGIES                      *
 ******************************************************************************
 *
 * MODULE NAME: SYS MNGR EIM CFG
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/product/comedia/branches/idecode_hd/build/inc/sys_mngr_eim_cfg.h $
 *            $Rev: 244 $
 *            $Date: 2011-08-18 18:33:39 +0200 (Thu, 18 Aug 2011) $
 *
 * PUBLIC
 *
 * DESCRIPTION: sys mngr eim configuration header
 *
 *****************************************************************************/

#ifndef _SYS_MNGR_EIM_CFG_H_
#define _SYS_MNGR_EIM_CFG_H_

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************/
/*              Defines                                */
/*******************************************************/

/*******************************************************/
/*               typedef                               */
/*******************************************************/
typedef struct
{
    uint8_t    ucMaxTableEitReq;
    uint8_t    ucMaxConsecTableEitReq;
    uint32_t   uiMemSize;
    bool       bEimHandlePFOtherTS;
    bool       bEimHandleSCOtherTS;
    uint8_t    ucEimBitmask; 
    uint8_t     ucTimeoutPfActOrEitAtsc;
    uint8_t     ucTimeoutPfOthOrEttAtsc;
    uint16_t    usTimeoutScAct;
    uint16_t    usTimeoutScOth;
    bool        bUseLastSectionNb;
    uint8_t     ucDvbScMaxUnavailableBeforeIgnore;
    uint8_t     ucDVBSpecificsBitmask;
    uint8_t     ucMaxTableEitFilters;
} tSYS_MNGR_EIM_CFG_info;


/********************************************************/
/*              Global File Variables                   */
/********************************************************/

/********************************************************/
/*              Local Functions Declarations (LOCAL)    */
/*******************************************************/

/********************************************************/
/*              Functions Definitions (LOCAL/GLOBAL)    */
/********************************************************/
TKEL_err SYS_MNGR_EIM_CFG_Init(uint32_t *uiMemoryPartitionSize, uint8_t **partition);


#ifdef EIT_FULL_CACHING_ON
TKEL_err SYS_MNGR_EIM_Malloc(TKEL_msize memorySize, void **buffer);
TKEL_err SYS_MNGR_EIM_Free(void *buffer);
#endif

/*===========================================================================
 *
 * SYS_MNGR_EIM_CFG_GetInfo
 *
 * Parameters:
 *      None
 *
 * Description:
 *      Sets a structure for the EIM capabilities.
 *
 * Returns:
 *      
 *
 *===========================================================================*/
void SYS_MNGR_EIM_CFG_GetInfo(tSYS_MNGR_EIM_CFG_info *pstInfo);
void SYS_MNGR_EIM_CFG_GetActiveCachingInfo(tSYS_MNGR_EIM_CFG_info *pstInfo);

bool SYS_MNGR_EIM_CFG_EitCachingActivated(void);

int SYS_MNGR_EIM_CFG_DoCacheRadioServices( void );

/*===========================================================================
 *
 * SYS_MNGR_EIM_CFG_GetEventBroadcastUpdateOffset
 *
 * Parameters:
 *      None
 *
 * Description:
 *      Returns the offset between the day switch at 00:00 UTC and the actual
 *      segment change in the event broadcast. The standard specifies that
 *      segment 0 contains events for the current day, but some broadcast may
 *      choose to delay the day switch for some reason, for instance to avoid
 *      issues when changing the time offset for daylight savings.
 *      For instance, the Ziggo network updates segment 0 at 02:00 UTC instead
 *      of 00:00 UTC.
 *
 * Returns:
 *      An offset indicating the number of seconds to apply to the current UTC
 *      time before deciding that the current day changed.
 *      For instance, the Ziggo network should be configured with -2*3600.
 *
 *===========================================================================*/
int SYS_MNGR_EIM_CFG_GetEventBroadcastUpdateOffset( void );

#ifdef __cplusplus
}
#endif

#endif /* _SYS_MNGR_EIM_CFG_H_ */
