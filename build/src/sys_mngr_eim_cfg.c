/******************************************************************************
 *                    COPYRIGHT 2006 IWEDIA TECHNOLOGIES                      *
 ******************************************************************************
 *
 * MODULE NAME: SYS MNGR EIM CFG
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/product/comedia/branches/idecode_hd/build/src/sys_mngr_eim_cfg.c $
 *            $Rev: 244 $
 *            $Date: 2011-08-18 18:33:39 +0200 (Thu, 18 Aug 2011) $
 *
 * PUBLIC
 *
 * DESCRIPTION: sys mngr eim configuration
 *
 *****************************************************************************/

/********************************************************/
/*                Includes                              */
/********************************************************/
#include "crules.h"
#include "tkel.h"
#include "country_cfg.h"
#include "sys_mngr_eim_cfg.h"
#include <time.h>

#if defined(EIT_FULL_CACHING_ON_AVMEM)
#include "stavmem.h"

    enum /* from pltf_hardcfg.h */
    {
        AVMEM_LMI_SYS,
        AVMEM_LMI_VID,
        AVMEM_SECURE,
        AVMEM_NB_DEVICES
    };
    extern STAVMEM_PartitionHandle_t SYS_AVMEMPartitionHandle[AVMEM_NB_DEVICES];

#endif

/********************************************************/
/*              Defines                                 */
/********************************************************/

#ifdef EIT_FULL_CACHING_ON
#ifndef SYS_MNGR_EIT_MEM_SIZE
#define SYS_MNGR_EIT_MEM_SIZE    1024*1024
#endif
#endif /* EIT_FULL_CACHING_ON */

/* Bitmap for set capabilities */
#define EIM_SETCAPAB_MEMORY                 0x01
#define EIM_SETCAPAB_TABREQ                 0x06
#define EIM_SETCAPAB_EIT_TABREQ             0x02
#define EIM_SETCAPAB_CONSEQ_TABREQ          0x04
#define EIM_SETCAPAB_OTHERTS                0x18
#define EIM_SETCAPAB_PF_OTHERTS             0x08
#define EIM_SETCAPAB_SC_OTHERTS             0x10
#define EIM_SETCAPAB_TIMEOUTS               0x20
#define EIM_SETCAPAB_DVB_SC_SPECIFIC        0x40
#define EIM_SETCAPAB_TABFILTER              0x80

/* Bitmap for implementation of DVB standard specifics */
#define EIM_DVB_EIT_PRESENT_FOLLOWING_FLAG  0x01
#define EIM_DVB_EIT_SCHEDULE_FLAG           0x02
#define EIM_DVB_EIT_LAST_SECTION_NUMBER     0x04
#define EIM_DVB_EIT_LAST_TABLE_ID           0x08
#define EIM_DVB_EIT_EMPTY_SEGMENT           0x10
#define EIM_DVB_EIT_LAST_TABLE_TIMEOUT      0x20

/********************************************************/
/*              Macros                                  */
/********************************************************/

/********************************************************/
/*              Local File Variables (LOCAL)            */
/********************************************************/

/********************************************************/
/*              Local Functions Declarations (LOCAL)    */
/*******************************************************/

/********************************************************/
/*              Functions Definitions (LOCAL/GLOBAL)    */
/********************************************************/
#ifdef EIT_FULL_CACHING_ON
#ifdef EIT_FULL_CACHING_ON_STATIC
        uint8_t pSYS_MNGR_EIT_memory[SYS_MNGR_EIT_MEM_SIZE];
        #pragma ST_section (pSYS_MNGR_EIT_memory, "eim_section")
#else
        uint8_t *pSYS_MNGR_EIT_memory = NULL;
#endif
#else
	uint8_t *pSYS_MNGR_EIT_memory = NULL;
#endif

TKEL_err SYS_MNGR_EIM_CFG_Init(uint32_t *uiMemoryPartitionSize, uint8_t **partition)
{
#ifdef EIT_FULL_CACHING_ON
#ifdef EIT_FULL_CACHING_ON_STATIC
        printf("==========================================================\n");
        printf("CM_FULL CACHING _ sys_mngr_eim_cfg.c\n");
        printf("CM_FULL CACHING _ Size of STATIC memory defined for Events Information Caching = %ld bytes\n",
                                 SYS_MNGR_EIT_MEM_SIZE);
        printf("CM_FULL CACHING _ If not sufficient, Events Information Caching will suppress information from cache\n");
        printf("==========================================================\n");
        *uiMemoryPartitionSize = SYS_MNGR_EIT_MEM_SIZE;
        *partition = pSYS_MNGR_EIT_memory;
#elif defined(EIT_FULL_CACHING_ON_AVMEM)
    	ST_ErrorCode_t             StErr;
    	STAVMEM_AllocBlockParams_t stAvmemAllocBlockParams;
    	STAVMEM_BlockHandle_t      AvmemBlockHandle;
    	void     *address;

		stAvmemAllocBlockParams.PartitionHandle          = SYS_AVMEMPartitionHandle[AVMEM_LMI_SYS];
		stAvmemAllocBlockParams.Size                     = SYS_MNGR_EIT_MEM_SIZE + sizeof(STAVMEM_BlockHandle_t);
		stAvmemAllocBlockParams.Alignment                = sizeof(STAVMEM_BlockHandle_t);
		stAvmemAllocBlockParams.AllocMode                = STAVMEM_ALLOC_MODE_BOTTOM_TOP;
		stAvmemAllocBlockParams.NumberOfForbiddenRanges  = 0;
		stAvmemAllocBlockParams.ForbiddenRangeArray_p    = (STAVMEM_MemoryRange_t *)NULL;
		stAvmemAllocBlockParams.NumberOfForbiddenBorders = 0;
		stAvmemAllocBlockParams.ForbiddenBorderArray_p   = (void **)NULL;

		StErr = STAVMEM_AllocBlock(&stAvmemAllocBlockParams,
								   &AvmemBlockHandle);

		if(StErr != ST_NO_ERROR)
		{
            printf("SYS_MNGR_EIM_Init Unable to alloc EIT CACHE\n");
            return(TKEL_NOT_DONE);
		}

		StErr = STAVMEM_GetBlockAddressCached(AvmemBlockHandle, (void **) &pSYS_MNGR_EIT_memory);
		if(StErr != ST_NO_ERROR)
		{
            printf("SYS_MNGR_EIM_Init Unable to get cached EIT CACHE address\n");
            return(TKEL_NOT_DONE);
		}

        printf("SYS_MNGR_EIM_Init of %d\n", SYS_MNGR_EIT_MEM_SIZE);

        printf("==========================================================\n");
        printf("CM_FULL CACHING _ sys_mngr_eim_cfg.c\n");
        printf("CM_FULL CACHING _ Size of AVMEM memory defined (through STAVMEM_AllocBlock) for Events Information Caching = %u bytes\n",
                                 SYS_MNGR_EIT_MEM_SIZE);
        printf("CM_FULL CACHING _ If not sufficient, Events Information Caching will suppress information from cache\n");
        printf("==========================================================\n");

        printf("---SYS_MNGR_EIM_Malloc\n");

		memcpy(pSYS_MNGR_EIT_memory, &AvmemBlockHandle, sizeof(STAVMEM_BlockHandle_t));
		pSYS_MNGR_EIT_memory+=sizeof(STAVMEM_BlockHandle_t);

		memset(pSYS_MNGR_EIT_memory,
               0,
               SYS_MNGR_EIT_MEM_SIZE);

        *uiMemoryPartitionSize = SYS_MNGR_EIT_MEM_SIZE;
        *partition = pSYS_MNGR_EIT_memory;
#else
        printf("<<<SYS_MNGR_EIM_Init\n");
        if (pSYS_MNGR_EIT_memory == NULL)
        {
            printf("SYS_MNGR_EIM_Init of %d\n", SYS_MNGR_EIT_MEM_SIZE);
            /* Allocate it */
            if (TKEL_Malloc(SYS_MNGR_EIT_MEM_SIZE,(void **)&pSYS_MNGR_EIT_memory) != TKEL_NO_ERR)
            {
                printf("SYS_MNGR_EIM_Init Unable to alloc EIT CACHE\n");
                return(TKEL_NOT_DONE);
            }

            printf("==========================================================\n");
            printf("CM_FULL CACHING _ sys_mngr_eim_cfg.c\n");
            printf("CM_FULL CACHING _ Size of DYNAMIC memory defined (through TKEL_Malloc) for Events Information Caching = %u bytes\n",
                                     SYS_MNGR_EIT_MEM_SIZE);
            printf("CM_FULL CACHING _ If not sufficient, Events Information Caching will suppress information from cache\n");
            printf("==========================================================\n");
        }

        printf("---SYS_MNGR_EIM_Malloc\n");

        memset(pSYS_MNGR_EIT_memory,
               0,
               SYS_MNGR_EIT_MEM_SIZE);

        *uiMemoryPartitionSize = SYS_MNGR_EIT_MEM_SIZE;
        *partition = pSYS_MNGR_EIT_memory;
#endif
#endif
        return(TKEL_NO_ERR);
}

#ifdef EIT_FULL_CACHING_ON
TKEL_err SYS_MNGR_EIM_Malloc(TKEL_msize memorySize,
                        void **buffer)
{
        memset(pSYS_MNGR_EIT_memory,
               0,
               SYS_MNGR_EIT_MEM_SIZE);

        *buffer = (void*)pSYS_MNGR_EIT_memory;

    return TKEL_NO_ERR;
}

TKEL_err SYS_MNGR_EIM_Free(void *buffer)
{
    return TKEL_NO_ERR;
}
#endif




/*===========================================================================
 *
 * SYS_MNGR_EIM_CFG_EitCachingActivated
 *
 * Parameters:
 *      None
 *
 * Description:
 *
 *
 * Returns:
 *
 *
 *===========================================================================*/
bool SYS_MNGR_EIM_CFG_EitCachingActivated(void)
{
#ifdef EIT_FULL_CACHING_ON
    return (true);
#else
    return (false);
#endif
}

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
void SYS_MNGR_EIM_CFG_GetInfo(tSYS_MNGR_EIM_CFG_info *pstInfo)
{
    tCOUNTRY_CFG_EIT_Requirements stEITRequirements;

    if(pstInfo != NULL)
    {
        COUNTRY_CFG_GetCountryEITRequirements(&stEITRequirements);

        pstInfo->ucMaxTableEitReq = 0xFF;       /* useless for DVB products */
        pstInfo->ucMaxConsecTableEitReq = 0xFF; /* useless for DVB products */

        /* The handling of EIT others depends on the country and the product */
#ifdef _SERV_NO_EIT_SERVICE_CACHING_
        pstInfo->bEimHandlePFOtherTS = FALSE;
#else
        pstInfo->bEimHandlePFOtherTS =
            (stEITRequirements.flags & kCOUNTRY_CFG_EIT_PRESENT_FOLLOWING_OTHER) ? TRUE : FALSE;
#endif
        pstInfo->bEimHandleSCOtherTS =
            (stEITRequirements.flags & kCOUNTRY_CFG_EIT_SCHEDULE_OTHER) ? TRUE : FALSE;

        /* The timeout values depend on the country */
        pstInfo->ucTimeoutPfActOrEitAtsc =
            stEITRequirements.EITpfActualRepetitionRate;
        pstInfo->ucTimeoutPfOthOrEttAtsc =
            stEITRequirements.EITpfOtherRepetitionRate;
        pstInfo->usTimeoutScAct =
            stEITRequirements.EITschActualRepetitionRate;
        pstInfo->usTimeoutScOth =
            stEITRequirements.EITschOtherRepetitionRate;

        /*
         * The number of consecutive unavailable segments to take into account
         * depends on the country.
         */
        pstInfo->ucDvbScMaxUnavailableBeforeIgnore =
            stEITRequirements.unavailableEITschSegmentsThreshold;

        /* The DVB specifics depend on the country */
        pstInfo->ucDVBSpecificsBitmask = 0;
        if (stEITRequirements.flags & kCOUNTRY_CFG_EIT_PRESENT_FOLLOWING_FLAG)
        {
            pstInfo->ucDVBSpecificsBitmask |= EIM_DVB_EIT_PRESENT_FOLLOWING_FLAG;
        }

        if (stEITRequirements.flags & kCOUNTRY_CFG_EIT_SCHEDULE_FLAG)
        {
            pstInfo->ucDVBSpecificsBitmask |= EIM_DVB_EIT_SCHEDULE_FLAG;
        }

        if (stEITRequirements.flags & kCOUNTRY_CFG_EIT_LAST_SECTION_NUMBER)
        {
            pstInfo->ucDVBSpecificsBitmask |= EIM_DVB_EIT_LAST_SECTION_NUMBER;
        }

        if (stEITRequirements.flags & kCOUNTRY_CFG_EIT_LAST_TABLE_ID)
        {
            pstInfo->ucDVBSpecificsBitmask |= EIM_DVB_EIT_LAST_TABLE_ID;
        }

        if (stEITRequirements.flags & kCOUNTRY_CFG_EIT_EMPTY_SEGMENT)
        {
            pstInfo->ucDVBSpecificsBitmask |= EIM_DVB_EIT_EMPTY_SEGMENT;
        }

        if (stEITRequirements.flags & kCOUNTRY_CFG_EIT_LAST_TABLE_TIMEOUT)
        {
            pstInfo->ucDVBSpecificsBitmask |= EIM_DVB_EIT_LAST_TABLE_TIMEOUT;
        }

        /*
         * The number of TABLE filters depends on the board and on TABLE
         * but it must not exceed 32.
         */
#if !defined(TABLE_EIT_DVB_SPEED_IMPROVED)
        pstInfo->ucMaxTableEitFilters = 10;
#else
        pstInfo->ucMaxTableEitFilters = 30;
#endif

        pstInfo->ucEimBitmask = EIM_SETCAPAB_OTHERTS;
        pstInfo->ucEimBitmask |= EIM_SETCAPAB_TIMEOUTS;
        pstInfo->ucEimBitmask |= EIM_SETCAPAB_TABREQ;
        pstInfo->ucEimBitmask |= EIM_SETCAPAB_MEMORY;
        pstInfo->ucEimBitmask |= EIM_SETCAPAB_TABFILTER;
        pstInfo->ucEimBitmask |= EIM_SETCAPAB_DVB_SC_SPECIFIC;

        /* The memory configuration depends on the board */
#if defined(MEMORYCONFIG_8MB)
        pstInfo->uiMemSize = 256 * 1024;
#elif defined(EIT_FULL_CACHING_ON)
        pstInfo->uiMemSize = SYS_MNGR_EIT_MEM_SIZE;
#else
        if (SYS_MNGR_EIM_CFG_EitCachingActivated() == true)
            pstInfo->uiMemSize = SYS_MNGR_EIT_MEM_SIZE;
        else
        	pstInfo->uiMemSize = SYS_MNGR_EIT_MEM_SIZE;
#endif
    }
/*
Configuration tips:
    Standard values are:
        pstInfo->ucMaxTableEitReq = 10;
        pstInfo->ucMaxConsecTableEitReq = 3;
        pstInfo->bEimHandlePFOtherTS = TRUE;
        pstInfo->bEimHandleSCOtherTS = FALSE;
        pstInfo->uiMemSize = 512 * 1024;
        pstInfo->ucEimBitmask = EIM_SETCAPAB_OTHERTS;
        pstInfo->ucEimBitmask |= EIM_SETCAPAB_TABREQ;
        pstInfo->ucEimBitmask |= EIM_SETCAPAB_MEMORY;
    Specific cases:
    - Full caching:
            pstInfo->ucMaxTableEitReq = 10;
            pstInfo->ucMaxConsecTableEitReq = 4;
            pstInfo->bEimHandlePFOtherTS = TRUE;
            pstInfo->bEimHandleSCOtherTS = FALSE;
        pstInfo->uiMemSize = (uint32_t)(SYS_MNGR_EIT_MEM_SIZE * 0.90);
            pstInfo->ucEimBitmask = EIM_SETCAPAB_OTHERTS;
            pstInfo->ucEimBitmask |= EIM_SETCAPAB_TABREQ;
            pstInfo->ucEimBitmask |= EIM_SETCAPAB_MEMORY;
    - No service caching for banner (flag
      _SERV_NO_EIT_SERVICE_CACHING_ activated):
            pstInfo->ucMaxTableEitReq = 10;
            pstInfo->ucMaxConsecTableEitReq = 3;
            pstInfo->bEimHandlePFOtherTS = FALSE;
            pstInfo->bEimHandleSCOtherTS = FALSE;
        pstInfo->uiMemSize = 512 * 1024;
            pstInfo->ucEimBitmask = EIM_SETCAPAB_OTHERTS;
            pstInfo->ucEimBitmask |= EIM_SETCAPAB_TABREQ;
            pstInfo->ucEimBitmask |= EIM_SETCAPAB_MEMORY;
    - Low demux performances (no service caching and low demux
      usage, ideally, service caching would have to be
      deactivated on both the banner and the EPG):
            pstInfo->ucMaxTableEitReq = 4;
            pstInfo->ucMaxConsecTableEitReq = 2;
        pstInfo->bEimHandlePFOtherTS = FALSE;
            pstInfo->bEimHandleSCOtherTS = FALSE;
            pstInfo->uiMemSize = 512 * 1024;
            pstInfo->ucEimBitmask = EIM_SETCAPAB_OTHERTS;
        pstInfo->ucEimBitmask |= EIM_SETCAPAB_TABREQ;
            pstInfo->ucEimBitmask |= EIM_SETCAPAB_MEMORY;
    - Low memory (allow 1k per event and 50-100 events per day ...
      here the _SEL_NB_NEXT_DAYS_CACHING_ flag would have to be 0
      or maybe 1 and the flag _SEL_NO_SERVICE_CACHING_ would have
      to be activated):
            pstInfo->ucMaxTableEitReq = 10;
        pstInfo->ucMaxConsecTableEitReq = 3;
            pstInfo->bEimHandlePFOtherTS = FALSE;
            pstInfo->bEimHandleSCOtherTS = FALSE;
            pstInfo->uiMemSize = 128 * 1024;
            pstInfo->ucEimBitmask = EIM_SETCAPAB_OTHERTS;
        pstInfo->ucEimBitmask |= EIM_SETCAPAB_TABREQ;
        pstInfo->ucEimBitmask |= EIM_SETCAPAB_MEMORY;
    - Non DVB compliant stream (longer broadcast period of
      the EIT tables, last section nb not reliable, etc.):
            pstInfo->ucMaxTableEitReq = 10;
            pstInfo->ucMaxConsecTableEitReq = 3;
            pstInfo->bEimHandlePFOtherTS = TRUE;
            pstInfo->bEimHandleSCOtherTS = FALSE;
            pstInfo->uiMemSize = 512 * 1024;
            pstInfo->ucTimeoutPfActOrEitAtsc = 12; (seconds)
            pstInfo->ucTimeoutPfOthOrEttAtsc = 12; (seconds)
            pstInfo->ucTimeoutScAct = 32; (seconds)
            pstInfo->ucTimeoutScOth = 32; (seconds)
            pstInfo->bUseLastSectionNb = FALSE;
            pstInfo->ucDvbScMaxUnavailableBeforeIgnore = 8 ... or more ;

            pstInfo->ucEimBitmask = EIM_SETCAPAB_OTHERTS;
            pstInfo->ucEimBitmask |= EIM_SETCAPAB_TABREQ;
        pstInfo->ucEimBitmask |= EIM_SETCAPAB_MEMORY;
            pstInfo->ucEimBitmask |= EIM_SETCAPAB_TIMEOUTS;
            pstInfo->ucEimBitmask |= EIM_SETCAPAB_DVB_SC_SPECIFIC;
*/
}


/*===========================================================================
 *
 * SYS_MNGR_EIM_CFG_GetActiveCachingInfo
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
void SYS_MNGR_EIM_CFG_GetActiveCachingInfo(tSYS_MNGR_EIM_CFG_info *pstInfo)
{
#ifdef EIT_FULL_CACHING_ON
    tCOUNTRY_CFG_EIT_Requirements stEITRequirements;

    if(pstInfo != NULL)
    {
        COUNTRY_CFG_GetCountryEITRequirements(&stEITRequirements);

        /* timeout values depend on the country */
        pstInfo->ucTimeoutPfActOrEitAtsc =
            stEITRequirements.EITpfActualRepetitionRate + 1;
        pstInfo->ucTimeoutPfOthOrEttAtsc =
            stEITRequirements.EITpfOtherRepetitionRate + 1;
        pstInfo->usTimeoutScAct =
            stEITRequirements.EITschActualRepetitionRate + 1;
        pstInfo->usTimeoutScOth =
            stEITRequirements.EITschOtherRepetitionRate + 1;
        pstInfo->ucEimBitmask |= EIM_SETCAPAB_TIMEOUTS;
    }
#endif /* EIT_FULL_CACHING_ON */
    return;
}

/*===========================================================================
 *
 * SYS_MNGR_EIM_CFG_DoCacheRadioServices
 *
 * Parameters:
 *      None
 *
 * Description:
 *      Indicates whether events for radio services should be cached.
 *      Including radio services in the cache while no event is broadcast
 *      for these events increases the cache processing time by 45*2 seconds
 *      per cached day, i.e. 10'30" for a 7-day cache.
 *
 * Returns:
 *      0 if radio services should not be included in the cache.
 *      1 if radio services should be included in the cache.
 *
 *===========================================================================*/
int SYS_MNGR_EIM_CFG_DoCacheRadioServices( void )
{
    return 0;
}

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
int SYS_MNGR_EIM_CFG_GetEventBroadcastUpdateOffset()
{
    return -2*3600;
}

