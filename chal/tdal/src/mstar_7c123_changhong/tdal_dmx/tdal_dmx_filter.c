/******************************************************************************
 *               COPYRIGHT 2004 IWEDIA TECHNOLOGIES               *
 ******************************************************************************
 *
 * MODULE NAME: TDAL_DMX
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/sources/chal/core/tdal/branches/idecode_hd/os21/tdal_dmx/tdal_dmx_filter.c $
 *        $Rev: 414 $
 *        $Date: 2011-09-23 12:34:21 +0200 (Fri, 23 Sep 2011) $
 *
 * DESCRIPTION : TDAL DMX implementation
 *
 *****************************************************************************/

/********************************************************
   *   Includes                        *
   ********************************************************/
#include <stdio.h>
#include <string.h>

#include "crules.h"
#include "tdal_common.h"
#include "tdal_common_priv.h"
#include "tdal_dmx.h"
#include "tdal_dmx_priv.h"

#include "MsMemory.h"
#include "apiDMX.h"

#include "tbox.h"

#if   (!defined(BOOTLOADER)) && defined(TTXT_OSD_ON) && defined(TTXT_VBI_ON)
#include   "tdal_vbi_module_priv.h"
#endif

/********************************************************
   *   Defines                        *
   ********************************************************/

#define DMX_DEMO_RESERVED_ADDRESS     SECTION_BUF_ADR

#define PES_BUFFER_NUM        8   // 8 * 0x10000 = 0xc0000 << modify this to fit stream requirement
#define PES_BUFFER_SIZE       0x18000 // 96K  << modify this to fit stream requirement

#define SECTION_BUFFER_NUM    64  // 64 * 0x1000 = 0x40000
#define SECTION_BUFFER_SIZE   0x1000



typedef MS_U8 PES_BUF[PES_BUFFER_SIZE];
//PES_BUF*      Pes_Buf_Addr =  (PES_BUF*)(DMX_DEMO_RESERVED_ADDRESS + 0x80000) ; // PES buffer size = 0x80000
MS_U8         Pes_Buf_AllocFlag[PES_BUFFER_NUM];            // allocation flag

typedef MS_U8 SEC_BUF[SECTION_BUFFER_SIZE];
//SEC_BUF*      Section_Buf_Addr = (SEC_BUF*)DMX_DEMO_RESERVED_ADDRESS;

/********************************************************
   *   Macros                        *
   ********************************************************/
mTBOX_SET_MODULE(eTDAL_DMX);

/********************************************************
   *   Local   File   Variables   (LOCAL)            *
   ********************************************************/

/********************************************************
   *   Module   variables   (MODULE)               *
   ********************************************************/

/********************************************************
   *   Functions   Definitions   (LOCAL/GLOBAL)        *
   ********************************************************/

LOCAL void TDAL_DMXi_ConvertMaskToMApiDmxFormat(uint8_t            Size,
        uint8_t   *        pFilter,
        uint8_t   *        pMask,
        uint8_t   *        pMode,
        uint8_t   *        mFilter,
        uint8_t   *        mPositiveMask,
        uint8_t   *        mNegativeMask);


LOCAL void TDAL_DMXi_ConvertMaskToMApiDmxFormat(uint8_t            Size,
        uint8_t   *        pFilter,
        uint8_t   *        pMask,
        uint8_t   *        pMode,
        uint8_t   *        mFilter,
        uint8_t   *        mPositiveMask,
        uint8_t   *        mNegativeMask)
{
#ifdef   __DEBUG_TRACE__
	unsigned char filterArr[256] = "", negativeMaskArr[256] = "", positiveMaskArr[256] = "", maskArr[256] = "", modeArr[256] = "";
	unsigned char digit[10];
#endif
	uint8_t i = 0;
	uint8_t mask, mode, filter;

	for (i = 0; i < Size; i++)
	{
		filter = pFilter[i];
		mask = pMask[i];
		mode = pMode[i];

#ifdef   __DEBUG_TRACE__
		sprintf(digit, "%02X ", filter);
		strcat(filterArr, digit);
		sprintf(digit, "%02X ", mask);
		strcat(maskArr, digit);
		sprintf(digit, "%02X ", mode);
		strcat(modeArr, digit);
#endif

		mFilter[i] = filter;
		mPositiveMask[i] = mask & mode;
		mNegativeMask[i] = mask & (~mode);
		mPositiveMask[i] = mPositiveMask[i] | mNegativeMask[i];


#ifdef   __DEBUG_TRACE__
		sprintf(digit, "%02X ", mPositiveMask[i]);
		strcat(positiveMaskArr, digit);
		sprintf(digit, "%02X ", mNegativeMask[i]);
		strcat(negativeMaskArr, digit);
#endif

	}

#ifdef   __DEBUG_TRACE__
	mTBOX_TRACE((kTBOX_NIV_1, "\n\n"
			"Filter:       %s\n"
			"Mask:         %s\n"
			"Mode:         %s\n"
			"PositiveMask: %s\n"
			"NegativeMask: %s\n"
			"\n\n",
			filterArr,
			maskArr,
			modeArr,
			positiveMaskArr,
			negativeMaskArr));

#endif
}

/**======================================================================**
 * Function Name : TDAL_DMX_Allocate_Filter
 *
 * Description   : this function allocates a filter on a channel. The output
 *             is the filter Id if succedded. Else the output is
 *             kTDAL_DMX_ILLEGAL_FILTER.
 *
 * Return       : kTDAL_DMX_NO_ERROR if all is correct
 *             kTDAL_DMX_ERROR_INVALID_CHANNEL_ID if bad Id
 *             kTDAL_DMX_ERROR_TOO_MANY_REQ if there is no more
 *                                filter available
 *             kTDAL_DMX_ERROR_MUTEX for mutex issue
 *             kTDAL_DMX_ERROR_UNABLE_TO_GRANT
 *
 * Comment       :
 *
 **======================================================================**/
tTDAL_DMX_Error   TDAL_DMX_Allocate_Filter(   tTDAL_DMX_ChannelId   ChannelId   ,
                        tTDAL_DMX_FilterId   *pFilterId   )
{
	tTDAL_DMX_FilterId filterId;
	tTDAL_DMX_Error errorCode = kTDAL_DMX_NO_ERROR;
	int i;
	tTDAL_DMX_DemuxId demuxId;
	MS_BOOL AllocateHWFilter = TRUE;
	DMX_FILTER_STATUS MSErrorCode;
	DMX_FILTER_TYPE FilterTypeTmp;

	mTBOX_FCT_ENTER("TDAL_DMX_Allocate_Filter");

	/*------------------------------------*/
	/*   check   if   the   module   is   initialized   */
	/*------------------------------------*/
	if (!TDAL_DMXi_initialized)
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL,
						"Allocate_Filter:   TDAL_DMX   module   has   not   been   initialized\n"));
		mTBOX_RETURN(kTDAL_DMX_ERROR_NOT_INITIALIZED);
	}

	/*-------------------------------------*/
	/*   check   the   channel   Id        */
	/*-------------------------------------*/
	if ((ChannelId < 0 || ChannelId >= kTDAL_DMXi_MAX_NB_OF_CHANNELS)
			|| (TDAL_DMXi_pstChannel[ChannelId].bUsed == FALSE))
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL,"Allocate_Filter:   "
						"ChannelId   not   available   or   not   allocated\n"));

		mTBOX_RETURN(kTDAL_DMX_ERROR_INVALID_CHANNEL_ID);
	}

	/*   if   ((TDAL_DMXi_pstChannel[ChannelId].ChannelStream != eTDAL_DMX_SECTION_STREAM)   ||
	 (TDAL_DMXi_pstChannel[ChannelId].ChannelStream != eTDAL_DMX_SECTION_STREAM))
	 {
	 mTBOX_TRACE((kTBOX_NIV_CRITICAL,"Allocate_Filter :   "
	 "Filters   for   PES   streams   are   not   implemented\n"));
	 mTBOX_RETURN(kTDAL_DMX_ERROR_INVALID_CHANNEL_ID);
	 }*/

	mLockAccess(TDAL_DMXi_pSectionTableAccess);

	/*---------------------------------------------------------*/
	/*   check   for   PES   if   a   filter   already   exist   on   the   channel.   */
	/*   For   PES, handle   of   the   filter   is   equal   to   the   handle   of   */
	/*   of   the   stream.                        */
	/*---------------------------------------------------------*/
	if ((TDAL_DMXi_pstChannel[ChannelId].ChannelStream
			!= eTDAL_DMX_SECTION_STREAM)
			&& (TDAL_DMXi_pstChannel[ChannelId].ChannelStream
					!= eTDAL_DMX_TP_STREAM))
	{
		for (i = 0; i < kTDAL_DMXi_MAX_NB_OF_FILTERS; i++)
		{
			/*   two   tests   can   work   */
			if (TDAL_DMXi_pstFilter[i].AssociatedChannelId == ChannelId)
			{
#if   defined(TTXT_OSD_ON) && defined(TTXT_VBI_ON)
				/*   In   this   case, the   driver   have   already   allocated   a   filter   for   the   vbi   =>   nothing   to   allocate   for   the   OSD*/
				if( (TDAL_DMXi_pstChannel[ChannelId].bUsed) &&
						(TDAL_DMXi_pstChannel[ChannelId].ChannelStream == eTDAL_DMX_TELETEXT_STREAM) )
				{
					*pFilterId = TDAL_DMXi_pstFilter[i].FilterId;
					mUnLockAccess(TDAL_DMXi_pSectionTableAccess);
					mTBOX_RETURN(kTDAL_DMX_NO_ERROR);
				}
#endif
				mTBOX_TRACE((kTBOX_NIV_CRITICAL,"Allocate_Filter:   " "filter   already   exist   on   this   PES   channel\n"));
				mUnLockAccess(TDAL_DMXi_pSectionTableAccess);
				mTBOX_RETURN(kTDAL_DMX_ERROR_TOO_MANY_REQ);
			}
		}
	}

	/*------------------------------------------*/
	/*   Look   after   a   free   filter   Id        */
	/*------------------------------------------*/
	filterId = TDAL_DMXi_LookForFreeFilter();

	if (filterId == kTDAL_DMX_ILLEGAL_FILTER)
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Allocate_Filter:   "
						"Unable   to   found   a   free   filter\n"));

		errorCode = kTDAL_DMX_ERROR_TOO_MANY_REQ;
	}

	/*------------------------------------------*/
	/*   allocate   the   filter   in   pti        */
	/*------------------------------------------*/
	if (errorCode == kTDAL_DMX_NO_ERROR)
	{
		switch (TDAL_DMXi_pstChannel[ChannelId].ChannelStream)
		{
		case eTDAL_DMX_SECTION_STREAM:
		case eTDAL_DMX_TP_STREAM:

			break;
		case eTDAL_DMX_VIDEO_STREAM:
		case eTDAL_DMX_AUDIO_STREAM:
		case eTDAL_DMX_AUDIO_DESCRIPTION_STREAM:
		case eTDAL_DMX_PCR_STREAM:
			mTBOX_TRACE((kTBOX_NIV_1,"Allocate_Filter:   "
							"VID, AUD   or   PCR   channel :   No   HW   filter   allocated\n"));
			AllocateHWFilter = FALSE;
			break;

		case eTDAL_DMX_SUBTITLE_STREAM:
		case eTDAL_DMX_TELETEXT_STREAM:
			break;
		default:
			break;
		}

		TDAL_DMXi_pstFilter[filterId].AllocateHWFilter = AllocateHWFilter;

	}

	/*-------------------------------------------------------------*/
	/*   Configure   data   parameters   for   this   filter        */
	/*-------------------------------------------------------------*/
	if ((errorCode == kTDAL_DMX_NO_ERROR) && (AllocateHWFilter == TRUE))
	{
		/*   Configure   data   parameters   for   this   filter   */
		if (TDAL_DMXi_pstFilter[filterId].Filter == NULL)
		{
			TDAL_DMXi_pstFilter[filterId].Filter = TDAL_Malloc(
					kTDAL_DMX_DEPTH_SIZE);

			if (TDAL_DMXi_pstFilter[filterId].Filter == NULL)
			{
				mTBOX_TRACE((kTBOX_NIV_CRITICAL,"Allocate_Filter:   "
								"Unable   to   allocate   size   for   FilterBytes_p\n"));

				errorCode = kTDAL_DMX_ERROR_MUTEX;
			}
			else
			{
				mTBOX_TRACE((kTBOX_NIV_1,"Allocate_Filter:   "
								"FilterBytes_p = 0x%x\n",
								TDAL_DMXi_pstFilter[filterId].Filter));
			}
		}

		if (TDAL_DMXi_pstFilter[filterId].PositiveMask == NULL)
		{
			TDAL_DMXi_pstFilter[filterId].PositiveMask = TDAL_Malloc(
					kTDAL_DMX_DEPTH_SIZE);

			if (TDAL_DMXi_pstFilter[filterId].PositiveMask == NULL)
			{
				mTBOX_TRACE((kTBOX_NIV_CRITICAL,"Allocate_Filter:   "
								"Unable   to   allocate   size   for   PositiveMask\n"));

				errorCode = kTDAL_DMX_ERROR_MUTEX;
			}
			else
			{
				mTBOX_TRACE((kTBOX_NIV_1,"Allocate_Filter:   "
								"PositiveMask = 0x%x\n",
								TDAL_DMXi_pstFilter[filterId].PositiveMask));
			}
		}

		if (TDAL_DMXi_pstFilter[filterId].NegativeMask == NULL)
		{
			TDAL_DMXi_pstFilter[filterId].NegativeMask = TDAL_Malloc(
					kTDAL_DMX_DEPTH_SIZE);

			if (TDAL_DMXi_pstFilter[filterId].NegativeMask == NULL)
			{
				mTBOX_TRACE((kTBOX_NIV_CRITICAL,"Allocate_Filter:   "
								"Unable   to   allocate   size   for   NegativeMask\n"));

				errorCode = kTDAL_DMX_ERROR_MUTEX;
			}
			else
			{
				mTBOX_TRACE((kTBOX_NIV_1,"Allocate_Filter:   "
								"NegativeMask = 0x%x\n",
								TDAL_DMXi_pstFilter[filterId].NegativeMask));
			}
		}

		/*--------------------------*/
		/*   error   management      */
		/*--------------------------*/
		if (errorCode != kTDAL_DMX_NO_ERROR)
		{
			if (TDAL_DMXi_pstFilter[filterId].Filter != NULL)
			{
				TDAL_Free(TDAL_DMXi_pstFilter[filterId].Filter);
				TDAL_DMXi_pstFilter[filterId].Filter = NULL;
			}

			if (TDAL_DMXi_pstFilter[filterId].PositiveMask != NULL)
			{
				TDAL_Free(TDAL_DMXi_pstFilter[filterId].PositiveMask);
				TDAL_DMXi_pstFilter[filterId].PositiveMask = NULL;
			}

			if (TDAL_DMXi_pstFilter[filterId].NegativeMask != NULL)
			{
				TDAL_Free(TDAL_DMXi_pstFilter[filterId].NegativeMask);
				TDAL_DMXi_pstFilter[filterId].NegativeMask = NULL;
			}
		}
		else
		{
			/*------------------------------------------------------------------*/
			/*   filterData   is   the   structure   given   to   the   driver            */
			/*   with   this   code, any   modification   on   field   filter, mask   and   mode   */
			/*   followed   by   a   TDAL_DMXi_setFilter   will   change   the   behaviour   of   */
			/*   the   filter   concerned                        */
			/*------------------------------------------------------------------*/

//	      TDAL_DMXi_pstFilter[filterId].filterData.FilterBytes_p          = TDAL_DMXi_pstFilter[filterId].Filter;
//	      TDAL_DMXi_pstFilter[filterId].filterData.FilterMasks_p          = TDAL_DMXi_pstFilter[filterId].Mask;
//	      TDAL_DMXi_pstFilter[filterId].filterData.u.SectionFilter.ModePattern_p = TDAL_DMXi_pstFilter[filterId].Mode;
//	      TDAL_DMXi_pstFilter[filterId].filterData.u.SectionFilter.ModePattern_p = NULL; /*   not   used   in   stpti4   */
			memset(TDAL_DMXi_pstFilter[filterId].Filter, 0x00,
					kTDAL_DMX_DEPTH_SIZE);
			memset(TDAL_DMXi_pstFilter[filterId].PositiveMask, 0x00,
					kTDAL_DMX_DEPTH_SIZE);
			memset(TDAL_DMXi_pstFilter[filterId].NegativeMask, 0x00,
					kTDAL_DMX_DEPTH_SIZE);
		}
	}

	/*-----------------------------------------------*/
	/*   allocate   the   channel               */
	/*-----------------------------------------------*/
	if (errorCode == kTDAL_DMX_NO_ERROR)
	{
		switch (TDAL_DMXi_pstChannel[ChannelId].ChannelStream)
		{
		case eTDAL_DMX_SECTION_STREAM:
			FilterTypeTmp = DMX_FILTER_TYPE_SECTION;
			break;

		case eTDAL_DMX_VIDEO_STREAM:
			FilterTypeTmp = DMX_FILTER_TYPE_VIDEO;
			break;
		case eTDAL_DMX_AUDIO_STREAM:
			FilterTypeTmp = DMX_FILTER_TYPE_AUDIO;
			break;
		case eTDAL_DMX_AUDIO_DESCRIPTION_STREAM:
			FilterTypeTmp = DMX_FILTER_TYPE_AUDIO2;
			break;
		case eTDAL_DMX_TELETEXT_STREAM:
			FilterTypeTmp = DMX_FILTER_TYPE_TELETEXT;
			break;
		case eTDAL_DMX_SUBTITLE_STREAM:
			FilterTypeTmp = DMX_FILTER_TYPE_PES;
			break;
		case eTDAL_DMX_PCR_STREAM:
			FilterTypeTmp = DMX_FILTER_TYPE_PCR;
			break;
		case eTDAL_DMX_TP_STREAM:
			FilterTypeTmp = DMX_FILTER_TYPE_PACKET;
			break;
		default:
			mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Bad parameters for eChannelStream"));
			errorCode = kTDAL_DMX_ERROR_BAD_PARAMETERS;
			break;
		}

	}



	/*-----------------------------*/
	/*   update   the   filter   structure   */
	/*-----------------------------*/
	if (errorCode == kTDAL_DMX_NO_ERROR)
	{
		/*   Filter   allocation   success   */
		TDAL_DMXi_pstFilter[filterId].FilterId = filterId;
		TDAL_DMXi_pstFilter[filterId].bUsed = TRUE;
		TDAL_DMXi_pstFilter[filterId].bEnable = FALSE;
		TDAL_DMXi_pstFilter[filterId].bMultimediaDisabled = FALSE;
		TDAL_DMXi_pstFilter[filterId].AssociatedChannelId = ChannelId;
		TDAL_DMXi_pstFilter[filterId].bSet = 0;
		TDAL_DMXi_pstFilter[filterId].bStptiAssociated = FALSE;
		TDAL_DMXi_pstFilter[filterId].Tag = 0;
		TDAL_DMXi_pstFilter[filterId].efilterCRCMode = eTDAL_DMX_CRC_CHECK;
		TDAL_DMXi_pstFilter[filterId].FilterType = FilterTypeTmp;
		TDAL_DMXi_pstFilter[filterId].SectionBufferPtr = NULL;
		memset(&TDAL_DMXi_pstFilter[filterId].FilterInfo, 0x0,
				sizeof(TDAL_DMXi_pstFilter[filterId].FilterInfo));

		*pFilterId = filterId;
		mTBOX_TRACE((kTBOX_NIV_1,"Allocate_Filter:   SUCCEED   [ChId=%d-FId = %d]   \n",
						ChannelId,
						filterId));
	}

	/*-----------------------------------*/
	/*   Unlock   Protection   Table   Semaphore   */
	/*-----------------------------------*/
	mUnLockAccess(TDAL_DMXi_pSectionTableAccess);

	mTBOX_RETURN(errorCode);

}



/**======================================================================**
 * Function Name : TDAL_DMX_Set_Filter
 *
 * Description   : This function sets a hardware section filter on channel with
 *             specific condition ( table_id, table_id extension,
 *             version number, section number .....)
 *
 * Return       : kTDAL_DMX_NO_ERROR if all is correct
 *             kTDAL_DMX_ERROR_INVALID_CHANNEL_ID if channel is invalid
 *                      or channel not used or bad PID on the channel
 *             kTDAL_DMX_ERROR_INVALID_FILTER_ID if filter is invalid
 *             kTDAL_DMX_ERROR_BAD_PARAMETERS if size is bad
 *             kTDAL_DMX_ERROR_MUTEX for mutex error
 *             kTDAL_DMX_ERROR_UNABLE_TO_GRANT
 *
 * Comment       :
 *
 **======================================================================**/
tTDAL_DMX_Error   TDAL_DMX_Set_Filter(   tTDAL_DMX_ChannelId      ChannelId,
                     tTDAL_DMX_FilterId      FilterId,
                     uint32_t        Filter_track,
                     uint8_t            Size,
                     uint8_t   *        pFilter,
                     uint8_t   *        pMask,
                     uint8_t   *        pMode,
                     tTDAL_DMX_FilterCRCMode   eCRCMode)
{
	   tTDAL_DMX_Error   errorCode = kTDAL_DMX_NO_ERROR;
	   int        i, j;
	   uint32_t      requiredSize = 0;
	   DMX_FILTER_STATUS MSErrorCode;

	   mTBOX_FCT_ENTER("TDAL_DMX_Set_Filter");

	   /*------------------------------------*/
	   /*   check   if   the   module   is   initialized   */
	   /*------------------------------------*/
	   if   (!TDAL_DMXi_initialized)
	   {
	      mTBOX_TRACE((kTBOX_NIV_CRITICAL,
	            "Set_Filter:   TDAL_DMX   module   has   not   been   initialized\n"));
	      mTBOX_RETURN(kTDAL_DMX_ERROR_NOT_INITIALIZED);
	   }

	   /*-----------------------------------*/
	   /*   Check   channel            */
	   /*-----------------------------------*/
	   if   ((ChannelId   <   0   ||   ChannelId >= kTDAL_DMXi_MAX_NB_OF_CHANNELS)   ||
	      (TDAL_DMXi_pstChannel[ChannelId].bUsed == FALSE)   ||
	      (TDAL_DMXi_pstChannel[ChannelId].ChannelPid == kTDAL_DMX_ILLEGAL_PID))
	   {
	      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"Set_Filter:   "
	            "ChannelId   not   available, or   channel   not   used   or   bad   PID\n"));

	      errorCode = kTDAL_DMX_ERROR_INVALID_CHANNEL_ID;
          mTBOX_RETURN(errorCode);
	   }

	   /*-----------------------------------*/
	   /*   Check   filter            */
	   /*-----------------------------------*/
	   if   ((FilterId   <   0   ||   FilterId >= kTDAL_DMXi_MAX_NB_OF_FILTERS   ||
	      TDAL_DMXi_pstFilter[FilterId].bUsed == FALSE)
	   && errorCode == kTDAL_DMX_NO_ERROR)
	   {
	      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"Set_Filter:   "
	            "Argument   FilterId   not   available\n"));

	      errorCode = kTDAL_DMX_ERROR_INVALID_FILTER_ID;
          mTBOX_RETURN(errorCode);
	   }

	   /*-----------------------------------*/
	   /*   check   argument   size        */
	   /*-----------------------------------*/
	   if   (   (Size   >   kTDAL_DMX_DEPTH_SIZE ||   pFilter == (uint8_t*)NULL   ||
	      pMask == (uint8_t*)NULL   ||   pMode == (uint8_t*)NULL)
	       && errorCode == kTDAL_DMX_NO_ERROR)
	   {
	      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"Set_Filter:   "
	            "bad   argument   size\n"));

	      errorCode = kTDAL_DMX_ERROR_BAD_PARAMETERS;
          mTBOX_RETURN(errorCode);
	   }
       
	   /*-----------------------------------*/
	   /*   Lock   Protection   mutex        */
	   /*-----------------------------------*/
	   mLockAccess(TDAL_DMXi_pSectionTableAccess);
	   /*------------------------------------------------*/
	   /*   Save   hardware   filter   arguments.        */
	   /*   For   bytes   >   size, the   mask   must   be   set   to   null   */
	   /*------------------------------------------------*/
	i = 0;
	j = 0;
	while (i < kTDAL_DMX_DEPTH_SIZE && errorCode == kTDAL_DMX_NO_ERROR)
	{
		if (j < Size)
		{
			if (i != 1 && i != 2)
			{
				TDAL_DMXi_pstFilter[FilterId].Filter[i] = pFilter[j];
				TDAL_DMXi_pstFilter[FilterId].PositiveMask[i] = pMask[j];
				TDAL_DMXi_pstFilter[FilterId].NegativeMask[i] = pMode[j];
                j++;
			}
			else /* First and second bytes are section length and they are skipped */
			{
				TDAL_DMXi_pstFilter[FilterId].PositiveMask[i] = 0x00;
				TDAL_DMXi_pstFilter[FilterId].NegativeMask[i] = 0x00;
			}
		}
		else
		{
			/*   no   check   the   other   bytes   */
			TDAL_DMXi_pstFilter[FilterId].PositiveMask[i] = 0x00;
			TDAL_DMXi_pstFilter[FilterId].NegativeMask[i] = 0x00;
		}
		i++;
	}
    
	   if(errorCode == kTDAL_DMX_NO_ERROR)
	   {

		   TDAL_DMXi_ConvertMaskToMApiDmxFormat(kTDAL_DMX_DEPTH_SIZE,
				   TDAL_DMXi_pstFilter[FilterId].Filter,
				   TDAL_DMXi_pstFilter[FilterId].PositiveMask,
				   TDAL_DMXi_pstFilter[FilterId].NegativeMask,
				   TDAL_DMXi_pstFilter[FilterId].Filter,
				   TDAL_DMXi_pstFilter[FilterId].PositiveMask,
				   TDAL_DMXi_pstFilter[FilterId].NegativeMask);
		   TDAL_DMXi_pstFilter[FilterId].efilterCRCMode = eCRCMode;
		   TDAL_DMXi_pstFilter[FilterId].Tag = Filter_track;
	   }

	   if(errorCode == kTDAL_DMX_NO_ERROR)
	   {
	      /*   EDO   */
	      /*   TDAL_DMXi_pstFilter[i].FilterId   =>   FilterId   */
	      errorCode = TDAL_DMXi_FilterActivate(   TDAL_DMXi_pstChannel[ChannelId].DemuxId,
	                        ChannelId,
	                        FilterId);

	      /*----------------------------------*/
	      /*   Update   filter   table            */
	      /*----------------------------------*/
	      if(errorCode == kTDAL_DMX_NO_ERROR)
	      {
	      mTBOX_TRACE((kTBOX_NIV_1,"Set_Filter:   Call   to   TDAL_DMX_Set_Filter(-) :   "
	            "SUCCEED   on   channel   %d\n",
	            ChannelId));
	      }

	   }
	   mUnLockAccess(TDAL_DMXi_pSectionTableAccess);

	   mTBOX_RETURN(errorCode);

}


/**======================================================================**
 * Function Name : TDAL_DMXi_FilterActivate
 *
 * Description   : set the filter parameters
 *
 * Comment       :
 *
 **======================================================================**/
tTDAL_DMX_Error   TDAL_DMXi_FilterActivate(   tTDAL_DMX_DemuxId   DemuxId,
                        tTDAL_DMX_ChannelId   ChannelId,
                        tTDAL_DMX_FilterId   FilterId)
{
	   DMX_FILTER_STATUS MSErrorCode;
	   tTDAL_DMX_Error   errorCode = kTDAL_DMX_NO_ERROR;
	   int   i;
	   MS_BOOL   FilterWasEnabled = FALSE;
	   MS_U8    *pu8BufAddr;

	   mTBOX_FCT_ENTER("TDAL_DMXi_FilterActivate");


	   /*   --------------------   */
	   /*   Check   Primary   Errors   */
	   /*   --------------------   */
	   if(DemuxId   <   0   ||   DemuxId >= kTDAL_DMX_NB_DEMUX)
	   {
	      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"FilterActivate:   "
	            "Argument   ChannelId   not   available\n"));

	      errorCode = kTDAL_DMX_ERROR_INVALID_DEMUX_ID;
	   }

	   if   (ChannelId   <   0   ||   ChannelId >= kTDAL_DMXi_MAX_NB_OF_CHANNELS)
	   {
	      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"FilterActivate:   "
	            "Argument   ChannelId   not   available\n"));
	      errorCode = kTDAL_DMX_ERROR_INVALID_CHANNEL_ID;
	   }

	   if   (errorCode == kTDAL_DMX_NO_ERROR)
	   {
	      if   (FilterId<   0   ||   FilterId >= kTDAL_DMXi_MAX_NB_OF_FILTERS)
	      {
	      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"FilterActivate:   "
	            "Argument   Filter   not   available\n"));

	      errorCode = kTDAL_DMX_ERROR_INVALID_FILTER_ID;
	      }
	   }

	   if   (errorCode == kTDAL_DMX_NO_ERROR)
	   {
	      if   (TDAL_DMXi_pstChannel[ChannelId].bUsed == FALSE)
	      {
	      mTBOX_TRACE((kTBOX_NIV_CRITICAL, "FilterActivate:   "
	            "Slot   not   allocated\n"));

	      errorCode = kTDAL_DMX_ERROR_INVALID_CHANNEL_ID;
	      }
	   }

	   if   (errorCode == kTDAL_DMX_NO_ERROR)
	   {
	      if   (TDAL_DMXi_pstFilter[FilterId].bUsed == FALSE)
	      {
	      mTBOX_TRACE((kTBOX_NIV_CRITICAL, "FilterActivate:   "
	            "Filter   not   allocated\n"));

	      errorCode = kTDAL_DMX_ERROR_INVALID_FILTER_ID;
	      }
	   }

	   if   (errorCode == kTDAL_DMX_NO_ERROR)
	   {
	      for   (i=0; i<kTDAL_DMXi_MAX_NB_OF_CHANNELS; i++)
	      {
	      if   ((TDAL_DMXi_pstChannel[i].bUsed == TRUE)   &&
	        (TDAL_DMXi_pstChannel[i].bWaitingForDeletion == FALSE)   &&
	        (i != ChannelId)   &&
	        (TDAL_DMXi_pstChannel[i].DemuxId == DemuxId)   &&
	        (TDAL_DMXi_pstChannel[i].ChannelStream != eTDAL_DMX_PCR_STREAM)   &&
	        (TDAL_DMXi_pstChannel[i].ChannelPid == TDAL_DMXi_pstChannel[ChannelId].ChannelPid))   /*   EDO   */
	      {
	        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "FilterActivate:   "
	                 "chID=%d   PID   already   used   on   other   slot   on   the   same   demux   [Slot :   %d]\n",
	            ChannelId,
	                 i));

	        errorCode = kTDAL_DMX_ERROR_PID_IN_USE;
	      }
	      }
	   }


	   /*---------------*/
	   /*   Save   argument   */
	   /*---------------*/

	   if   (errorCode == kTDAL_DMX_NO_ERROR)
	   {
	      /*--------------------------------------------*/
	      /*   STPTI   call   for   start   the   associated   filter   */
	      /*--------------------------------------------*/

	      /*   Clear   Pid   for   this   slot   _   Only   if   channel   not   already   enabled   */
//	      if   (TDAL_DMXi_pstChannel[ChannelId].bEnable == FALSE)
//	      {
//	      STErrorCode = STPTI_SlotClearPid(TDAL_DMXi_pstChannel[ChannelId].SlotHandle);
//
//	      if(STErrorCode != ST_NO_ERROR)
//	      {
//	        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "FilterActivate:   "
//	                 "ChId=%d-Fid=%d->STPTI_SlotClearPid(-)   return   %s\n",
//	            ChannelId,
//	            FilterId,
//	                 TBOX_GetDriverErrorText(STErrorCode)));
//
//	        errorCode = kTDAL_DMX_ERROR_UNABLE_TO_GRANT;
//	      }
//	      else
//	      {
//	        mTBOX_TRACE((kTBOX_NIV_1, "FilterActivate:   "
//	                 "ChId=%d-Fid=%d->STPTI_SlotClearPid(0x%x)   return   %s\n",
//	            ChannelId,
//	            FilterId,
//	            TDAL_DMXi_pstChannel[ChannelId].SlotHandle,
//	                 TBOX_GetDriverErrorText(STErrorCode)));
//	      }
//	      }
	   }

	   /*-------------------------------------------------------------*/
	   /*   Set   Pid   for   this   slot   _   Only   if   channel   not   already   enabled   */
	   /*-------------------------------------------------------------*/
	   if   (errorCode == kTDAL_DMX_NO_ERROR)
	   {
//	      if   (TDAL_DMXi_pstChannel[ChannelId].bEnable == FALSE)
//	      {
//	      STErrorCode = STPTI_SlotSetPid(TDAL_DMXi_pstChannel[ChannelId].SlotHandle   ,
//	                   (STPTI_Pid_t)   TDAL_DMXi_pstChannel[ChannelId].ChannelPid);
//
//	      if((STErrorCode != ST_NO_ERROR)   &&
//	      (STErrorCode != STPTI_ERROR_PID_ALREADY_COLLECTED))
//	      {
//	        mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_DMXi_FilterActivate :   "
//	                 "STPTI_SlotSetPid(-)   return   %s\n",
//	                 TBOX_GetDriverErrorText(STErrorCode)));
//	        errorCode = kTDAL_DMX_ERROR_UNABLE_TO_GRANT;
//	      }
//	      else
//	      {
//	      mTBOX_TRACE((kTBOX_NIV_1,"FilterActivate:   "
//	            "ChId=%d-Fid=%d->STPTI_SlotSetPid(s=0x%x,p=0x%x)   returns   %s\n",
//	            ChannelId,
//	            FilterId,
//	            TDAL_DMXi_pstChannel[ChannelId].SlotHandle,
//	            TDAL_DMXi_pstChannel[ChannelId].ChannelPid,
//	            TBOX_GetDriverErrorText(STErrorCode)));
//	      }
//	      }
	   }

	   if   (TDAL_DMXi_pstChannel[ChannelId].ChannelOutput == eTDAL_DMX_MEMORY)
	   {

	      /*   Check   if   the   filter   is   already   enabled   */
	      if(TDAL_DMXi_pstFilter[FilterId].bEnable == TRUE)
	      {
	      FilterWasEnabled = TRUE; /*   To   remember   to   renable   the   filter   afet   setting   */

	      TDAL_DMXi_Control_Filter(FilterId, eTDAL_DMX_CTRL_DISABLE);
	      }

	      /*---------------------------------*/
	      /*   Set   filter   data   and   filter   mask   */
	      /*---------------------------------*/

	      /*   Configure   filter   and   set   it   */
	      if   (errorCode == kTDAL_DMX_NO_ERROR)
	      {

	      /*---------------------------------*/
	      /*   Set   filter   configuration      */
	      /*---------------------------------*/

	      //STErrorCode = STPTI_FilterSet(TDAL_DMXi_pstFilter[FilterId].FilterHandle,
	       //          &TDAL_DMXi_pstFilter[FilterId].filterData);



	      /*-------------------------------------------------------------*/
	      /*   Link   filter   to   associated   slot                 */
	      /*-------------------------------------------------------------*/
	      if   (errorCode == kTDAL_DMX_NO_ERROR)
	      {
	      /*   EDO == => != */
//	      if   ((TDAL_DMXi_pstFilter[FilterId].AssociatedChannelId != kTDAL_DMX_ILLEGAL_CHANNEL)
//	       && (TDAL_DMXi_pstFilter[FilterId].bStptiAssociated == FALSE)   )
//	      {
//	        STErrorCode = STPTI_FilterAssociate(TDAL_DMXi_pstFilter[FilterId].FilterHandle,
//	                        TDAL_DMXi_pstChannel[ChannelId].SlotHandle);
//
//	        if(STErrorCode != ST_NO_ERROR)
//	        {
//	            mTBOX_TRACE((kTBOX_NIV_CRITICAL,"FilterActivate:   "
//	               "STPTI_FilterAssociate(-)   return   %s\n",
//	               TBOX_GetDriverErrorText(STErrorCode)));
//
//	            errorCode = kTDAL_DMX_ERROR_UNABLE_TO_GRANT;
//	        }
//	        else
//	        {
//	            mTBOX_TRACE((kTBOX_NIV_1,"FilterActivate:   "
//	               "STPTI_FilterAssociate(f=0x%x->0x%x)   return   %s\n",
//	               TDAL_DMXi_pstFilter[FilterId].FilterHandle,
//	               TDAL_DMXi_pstChannel[ChannelId].SlotHandle,
//	               TBOX_GetDriverErrorText(STErrorCode)));
//
//	            TDAL_DMXi_pstFilter[FilterId].bStptiAssociated = TRUE;
//	        }
//	      }
//	      }

	      }

	      if   (FilterWasEnabled == TRUE)
	      {
	    	  TDAL_DMXi_Control_Filter(FilterId, eTDAL_DMX_CTRL_ENABLE);
	      }
	   }

	#if   defined(__DEBUG_TRACE__) && defined(TDAL_DMX_DBG_TRC_PCK_CNT)
	   TDAL_DMXi_DebugPacketCount(ChannelId);
	#endif

	   /*-----------------------------------*/
	   /*   Unlock   Protection   Table   Semaphore   */
	   /*-----------------------------------*/
	   //mUnLockAccess(TDAL_DMXi_pSectionTableAccess);


	   }

	   mTBOX_RETURN(errorCode);
}





/**======================================================================**
 * Function Name : TDAL_DMX_Control_Filter
 *
 * Description   : this function is used to control a filter
 *              (enabling, stopping, resetting)
 *
 * Side effects  :
 *
 * Return       : kTDAL_DMX_NO_ERROR if all is correct
 *             kTDAL_DMX_ERROR_INVALID_FILTER_ID if no filter is invalid
 *             kTDAL_DMX_ERROR_BAD_PARAMETERS if bad argument
 *             kTDAL_DMX_ERROR_MUTEX for mutex issue
 *             kTDAL_DMX_ERROR_CANT_CONTROL_FREE_FILTER if filter is not used
 *             kTDAL_DMX_ERROR_UNABLE_TO_GRANT if not able to start or
 *                                  to stop the filter
 *
 * Comment       :
 *
 **======================================================================**/
tTDAL_DMX_Error   TDAL_DMXi_Control_Filter(   tTDAL_DMX_FilterId   FilterId,
                        tTDAL_DMX_Ctrl      Ctrl)
{
	tTDAL_DMX_Error errorCode = kTDAL_DMX_NO_ERROR;
	DMX_FILTER_STATUS MSErrorCode;

	mTBOX_FCT_ENTER("TDAL_DMXi_Control_Filter");

//	if (TDAL_DMXi_pstChannel[TDAL_DMXi_pstFilter[FilterId].AssociatedChannelId].ChannelStream
//			!= eTDAL_DMX_SECTION_STREAM)
//	{
//		mTBOX_TRACE((kTBOX_NIV_WARNING,"Control_Filter:   "
//						"Associated   channel   is   not   SECTION:   nothing   to   be   done\n"));
//
//		/*-------------------------------------*/
//		/*   Unlock   Protection   mutex        */
//		/*-------------------------------------*/
//		mTBOX_RETURN(kTDAL_DMX_NO_ERROR);
//	}
	/*-------------------------------------*/
	/*   Control   Filter            */
	/*-------------------------------------*/
	switch (Ctrl)
	{
	case eTDAL_DMX_CTRL_ENABLE:
		if (TDAL_DMXi_pstFilter[FilterId].bEnable == FALSE)
		{
			if (TDAL_DMXi_pstChannel[TDAL_DMXi_pstFilter[FilterId].AssociatedChannelId].bEnable
					== TRUE && TDAL_DMXi_pstFilter[FilterId].bMultimediaDisabled == FALSE)

			{
			    /*-------------------------------------------
			     *     openning channel
			     * ------------------------------------------
			     */

			    if (errorCode == kTDAL_DMX_NO_ERROR)
			    {
			        mTBOX_TRACE((kTBOX_NIV_1, "[TDAL_DMX_Allocate_Filter]    MApi_DMX_Open filter, filter type = %x\n", TDAL_DMXi_pstFilter[FilterId].FilterType));
			        MSErrorCode = MApi_DMX_Open(TDAL_DMXi_pstFilter[FilterId].FilterType | DMX_FILTER_SOURCE_TYPE_LIVE,
			                &TDAL_DMXi_pstFilter[FilterId].FilterHandle);

			        if (DMX_FILTER_STATUS_OK != MSErrorCode)
			        {
			            mTBOX_TRACE((kTBOX_NIV_CRITICAL, "MApi_DMX_Open returned error %d\n", MSErrorCode));
			            errorCode = kTDAL_DMX_ERROR_MUTEX;
			        }
			    }

				   if (errorCode == kTDAL_DMX_NO_ERROR &&
						   (TDAL_DMXi_pstFilter[FilterId].FilterType != DMX_FILTER_TYPE_VIDEO
								   && TDAL_DMXi_pstFilter[FilterId].FilterType != DMX_FILTER_TYPE_AUDIO
								   && TDAL_DMXi_pstFilter[FilterId].FilterType != DMX_FILTER_TYPE_AUDIO2
								   && TDAL_DMXi_pstFilter[FilterId].FilterType != DMX_FILTER_TYPE_PCR
						   ))
				   {

					   int bufferSize = TDAL_DMXi_pstChannel[TDAL_DMXi_pstFilter[FilterId].AssociatedChannelId].BufferSize;

					   if (TDAL_DMXi_pstFilter[FilterId].SectionBufferPtr != NULL)
					   {
						   //Transfer to cache va because the orignal memory was allocated as cache va.
						   TDAL_Free(MsOS_PA2KSEG0(MsOS_VA2PA(TDAL_DMXi_pstFilter[FilterId].SectionBufferPtr)));
						   TDAL_DMXi_pstFilter[FilterId].SectionBufferPtr = NULL;
					   }

		//			    // Here we use pre-defined physical address
		//			    // [Note] The section buffer MUST be continus in physical address space.(for DMA usage)
		//			   pu8BufAddr = (MS_U8*)Section_Buf_Addr[TDAL_DMXi_pstFilter[FilterId].FilterHandle];
		//
		//			   //MsOS_PA2KSEG1 : transfer physical address to virtual address.
		//			   memset((MS_U8*)MsOS_PA2KSEG1((MS_U32)pu8BufAddr), 0 , SECTION_BUFFER_SIZE);
		//
		//			   // Transfer Virtual Address to Phisical Hardware Address
		//			   // Section buffer is structed as ring buffer, keep 4 pointer of start,end ,read and write.
		//			   TDAL_DMXi_pstFilter[FilterId].FilterInfo.Info.SectInfo.SectBufAddr = (MS_U32)pu8BufAddr ;
		//			   // Set buffer size
		//			   TDAL_DMXi_pstFilter[FilterId].FilterInfo.Info.SectInfo.SectBufSize = SECTION_BUFFER_SIZE;

					   //To use non-cache va for data coherence.
					   TDAL_DMXi_pstFilter[FilterId].SectionBufferPtr = MsOS_PA2KSEG1(MsOS_VA2PA(TDAL_Malloc(bufferSize)));
					   memset(TDAL_DMXi_pstFilter[FilterId].SectionBufferPtr, 0x0, bufferSize);

					   TDAL_DMXi_pstFilter[FilterId].FilterInfo.Info.SectInfo.SectBufAddr = MsOS_VA2PA(TDAL_DMXi_pstFilter[FilterId].SectionBufferPtr);
					   TDAL_DMXi_pstFilter[FilterId].FilterInfo.Info.SectInfo.SectBufSize = bufferSize;

					   TDAL_DMXi_pstFilter[FilterId].FilterInfo.Info.SectInfo.Event       =     DMX_EVENT_DATA_READY | DMX_EVENT_CB_SELF_TYPE2 | DMX_EVENT_BUF_OVERFLOW;
					   TDAL_DMXi_pstFilter[FilterId].FilterInfo.Info.SectInfo.Type2NotifyParam1 = FilterId;
					   TDAL_DMXi_pstFilter[FilterId].FilterInfo.Info.SectInfo.pType2Notify     =    TDAL_DMXi_NotifySectCallback;

					   if(TDAL_DMXi_pstFilter[FilterId].FilterType  == DMX_FILTER_TYPE_TELETEXT ||
					      TDAL_DMXi_pstFilter[FilterId].FilterType  ==  DMX_FILTER_TYPE_PES)
					   {
						   TDAL_DMXi_pstFilter[FilterId].FilterInfo.Info.SectInfo.SectMode  =  DMX_SECT_MODE_CONTI;
					   }
					   else
					   {
						   TDAL_DMXi_pstFilter[FilterId].FilterInfo.Info.SectInfo.SectMode  =  DMX_SECT_MODE_CONTI | (TDAL_DMXi_pstFilter[FilterId].efilterCRCMode == eTDAL_DMX_CRC_CHECK ? DMX_SECT_MODE_CRCCHK : 0x0);
					   }

					   if (errorCode == kTDAL_DMX_NO_ERROR)
					   {
						   DMX_FILTER_TYPE filterType = TDAL_DMXi_pstFilter[FilterId].FilterType | DMX_FILTER_SOURCE_TYPE_LIVE;
						   mTBOX_TRACE((kTBOX_NIV_1, "[TDAL_DMX_Allocate_Filter]    MApi_DMX_Info filterId = %d, filter type = %x, buffer size %d\n", FilterId, filterType, bufferSize));
						   MSErrorCode = MApi_DMX_Info(TDAL_DMXi_pstFilter[FilterId].FilterHandle, &TDAL_DMXi_pstFilter[FilterId].FilterInfo, &filterType, TRUE);
						   if (MSErrorCode != DMX_FILTER_STATUS_OK)
						   {
							   mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Call to MApi_DMX_Info failed"));
							   errorCode = kTDAL_DMX_ERROR_MUTEX;
						   }
					   }
				   }

				if(errorCode == kTDAL_DMX_NO_ERROR)
		        {
		        	tTDAL_DMX_ChannelId channelId = TDAL_DMXi_pstFilter[FilterId].AssociatedChannelId;
		        	MS_U16 pid = TDAL_DMXi_pstChannel[channelId].ChannelPid;
		        	mTBOX_TRACE((kTBOX_NIV_1, "[TDAL_DMX_Control_Filter]    MApi_DMX_Pid filterId = %d, pid = %d\n", FilterId, pid));
		        	if   (pid   !=   0x1FFF)
		            {
		        		MSErrorCode = MApi_DMX_Pid(TDAL_DMXi_pstFilter[FilterId].FilterHandle, &pid, TRUE);
		            }
		            else
		            {
		            	MSErrorCode = MApi_DMX_Pid(TDAL_DMXi_pstFilter[FilterId].FilterHandle, &pid, TRUE);
		            }

		            if(MSErrorCode != DMX_FILTER_STATUS_OK)
		            {
						mTBOX_TRACE((kTBOX_NIV_CRITICAL,"Control_Channel:   "
								 "MApi_DMX_Pid(s=0x%x,p=0x%x)   return   %d\n",
								 TDAL_DMXi_pstFilter[FilterId].FilterHandle,
								 TDAL_DMXi_pstChannel[TDAL_DMXi_pstFilter[FilterId].AssociatedChannelId].ChannelPid,
								 MSErrorCode));
						errorCode = kTDAL_DMX_ERROR_UNABLE_TO_GRANT;
		            }
		            else
		            {
		            mTBOX_TRACE((kTBOX_NIV_1,"Control_Channel:   "
							 "MApi_DMX_Pid(s=0x%x,p=0x%x)   returns  success\n",
							 TDAL_DMXi_pstFilter[FilterId].FilterHandle,
							 TDAL_DMXi_pstChannel[TDAL_DMXi_pstFilter[FilterId].AssociatedChannelId].ChannelPid));
		            }
		        }

				   if (errorCode == kTDAL_DMX_NO_ERROR)
					{
						if (TDAL_DMXi_pstFilter[FilterId].AllocateHWFilter == TRUE)
						{

							mTBOX_TRACE((kTBOX_NIV_1, "[TDAL_DMX_Allocate_Filter]    MApi_DMX_SectPatternSet filterId = %d\n", FilterId));

							mTBOX_ASSERT(TDAL_DMXi_pstFilter[FilterId].Filter != NULL);
							mTBOX_ASSERT(TDAL_DMXi_pstFilter[FilterId].PositiveMask != NULL);
							mTBOX_ASSERT(TDAL_DMXi_pstFilter[FilterId].NegativeMask != NULL);

						    MApi_DMX_SectReset(TDAL_DMXi_pstFilter[FilterId].FilterHandle);
							MSErrorCode = MApi_DMX_SectPatternSet(
									TDAL_DMXi_pstFilter[FilterId].FilterHandle,
									TDAL_DMXi_pstFilter[FilterId].Filter,
									TDAL_DMXi_pstFilter[FilterId].PositiveMask,
									TDAL_DMXi_pstFilter[FilterId].NegativeMask,
									kTDAL_DMX_DEPTH_SIZE);

							if (MSErrorCode != DMX_FILTER_STATUS_OK)
							{
								mTBOX_TRACE((kTBOX_NIV_CRITICAL, "FilterActivate:   "
												"MApi_DMX_SectPatternSet(0x%x)   return   %d\n",
												TDAL_DMXi_pstFilter[FilterId].FilterHandle,
												MSErrorCode));

								errorCode = kTDAL_DMX_ERROR_UNABLE_TO_GRANT;
							}
							else
							{
								mTBOX_TRACE((kTBOX_NIV_1, "FilterActivate:   "
												"MApi_DMX_SectPatternSet(0x%x)   OK\n",
												MSErrorCode));
								/*
								 TDAL_DMXi_pstChannel[ChannelId].bEnable = TRUE;
								 TDAL_DMXi_pstFilter[FilterId].bEnable = TRUE;
								 */
								TDAL_DMXi_pstFilter[FilterId].bSet = TRUE;
							}

						}
					}


				if (errorCode == kTDAL_DMX_NO_ERROR)
				{
					mTBOX_TRACE((kTBOX_NIV_1, "[TDAL_DMX_Control_Filter]    MApi_DMX_Start filterId = %d\n", FilterId));
					MSErrorCode =
							MApi_DMX_Start(
									TDAL_DMXi_pstFilter[FilterId].FilterHandle);

					if (MSErrorCode != DMX_FILTER_STATUS_OK)
					{
						mTBOX_TRACE((kTBOX_NIV_CRITICAL,"Control_Filter:   "
										"ENABLE   FILTER   MApi_DMX_Start(-)"
										"   return  error  %d\n",
										MSErrorCode));

						errorCode = kTDAL_DMX_ERROR_UNABLE_TO_GRANT;
					}
					else
					{
						mTBOX_TRACE((kTBOX_NIV_1,"Control_Filter:   "
										"ENABLE   FILTER   MApi_DMX_Start(-)"
										"   return   success\n"));

						TDAL_DMXi_pstFilter[FilterId].bEnable = TRUE;
					}
				}

			}
			else
			{
				mTBOX_TRACE((kTBOX_NIV_1,"Control_Filter:   "
								"The   filter   is   marked   waiting   enable   because   the   channel   is   disabled\n"));
				TDAL_DMXi_pstFilter[FilterId].bWaitingEnable = TRUE;
			}
		}
		else
		{
			mTBOX_TRACE((kTBOX_NIV_1,"Control_Filter:   Filter   already   enable\n"));
		}
		break;

	case eTDAL_DMX_CTRL_DISABLE:
		if (TDAL_DMXi_pstFilter[FilterId].bEnable == TRUE)
		{
			if (TDAL_DMXi_pstChannel[TDAL_DMXi_pstFilter[FilterId].AssociatedChannelId].bEnable
					== TRUE)
			{
				MSErrorCode = MApi_DMX_Stop(
								TDAL_DMXi_pstFilter[FilterId].FilterHandle);
				MApi_DMX_SectReset(TDAL_DMXi_pstFilter[FilterId].FilterHandle);

				if (MSErrorCode != DMX_FILTER_STATUS_OK)
				{
					mTBOX_TRACE((kTBOX_NIV_CRITICAL,"Control_Filter:   "
									"ENABLE   FILTER   MApi_DMX_Stop(-)"
									"   return   %d\n",
									MSErrorCode));

					errorCode = kTDAL_DMX_ERROR_UNABLE_TO_GRANT;
				}
				else
				{
					mTBOX_TRACE((kTBOX_NIV_1,"Control_Filter:   "
									"ENABLE   FILTER   MApi_DMX_Stop(-)"
									"   return   success\n"));

					TDAL_DMXi_pstFilter[FilterId].bEnable = FALSE;
				}

	            MSErrorCode = MApi_DMX_Close(TDAL_DMXi_pstFilter[FilterId].FilterHandle);

	            if   (MSErrorCode != DMX_FILTER_STATUS_OK)
	            {
	                mTBOX_TRACE((kTBOX_NIV_CRITICAL,"FreeFilterAndAssociatedRessources:   "
	                         "MApi_DMX_Close(0x%x)   return   %s\n",
	                      TDAL_DMXi_pstFilter[FilterId].FilterHandle,
	                      MSErrorCode));

	                errorCode = kTDAL_DMX_ERROR_UNABLE_TO_GRANT;
	            }
	            else
	            {
	                mTBOX_TRACE((kTBOX_NIV_1,"FreeFilterAndAssociatedRessources:   "
	                         "MApi_DMX_Close(0x%x)   return   SUCCESS\n",
	                      TDAL_DMXi_pstFilter[FilterId].FilterHandle,
	                      MSErrorCode));
	            }

	            if (errorCode == kTDAL_DMX_NO_ERROR)
	            {


	                DMX_FILTER_TYPE filterType = TDAL_DMXi_pstFilter[FilterId].FilterType;

	                if (filterType == DMX_FILTER_TYPE_VIDEO || filterType == DMX_FILTER_TYPE_AUDIO || filterType == DMX_FILTER_TYPE_AUDIO2)
	                {
	                    MApi_DMX_AVFifo_Reset(filterType, FALSE);
	                }
	            }

			}
			else
			{
				mTBOX_TRACE((kTBOX_NIV_1,"Control_Filter:   "
								"The   filter   is   marked   waiting   disabled   but   the   channel   is   disabled\n"));

				TDAL_DMXi_pstFilter[FilterId].bWaitingEnable = FALSE;
			}
		}
		else
		{
			mTBOX_TRACE((kTBOX_NIV_1,"Control_Filter:   Filter   already   disabled\n"));
		}
		break;

	case eTDAL_DMX_CTRL_RESET:
		/*   do   nothing   */
		break;

	default:
		break;
	}

	if (errorCode == kTDAL_DMX_NO_ERROR)
	{
		mTBOX_TRACE((kTBOX_NIV_1,"Control_Filter:   SUCCEED   [FilterlId=%d]\n",FilterId));
	}

	mTBOX_RETURN(errorCode);
}


tTDAL_DMX_Error   TDAL_DMX_Control_Filter(   tTDAL_DMX_FilterId   FilterId,
                        tTDAL_DMX_Ctrl      Ctrl)
{
	tTDAL_DMX_Error errorCode = kTDAL_DMX_NO_ERROR;
	DMX_FILTER_STATUS MSErrorCode;

	mTBOX_FCT_ENTER("TDAL_DMX_Control_Filter");

	/*------------------------------------*/
	/*   check   if   the   module   is   initialized   */
	/*------------------------------------*/
	if (!TDAL_DMXi_initialized)
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL,
						"Control_Filter:   TDAL_DMX   module   has   not   been   initialized\n"));
		mTBOX_RETURN(kTDAL_DMX_ERROR_NOT_INITIALIZED);
	}

	/*-------------------------------*/
	/*   check   filter   Id        */
	/*-------------------------------*/
	if (FilterId < 0 || FilterId >= kTDAL_DMXi_MAX_NB_OF_FILTERS)
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL,"Control_Filter:   "
						"FilterId   not   valid   or   filter   not   used\n"));

		mTBOX_RETURN(kTDAL_DMX_ERROR_INVALID_FILTER_ID);
	}

	/*-------------------------------*/
	/*   Check   filter   state      */
	/*-------------------------------*/
	if (TDAL_DMXi_pstFilter[FilterId].bUsed == FALSE)
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL,"Control_Filter:   "
						"Filter   not   used\n"));

		mTBOX_RETURN(kTDAL_DMX_ERROR_CANT_CONTROL_FREE_FILTER);
	}

	/*-------------------------------*/
	/*   Check   argument        */
	/*-------------------------------*/
	if ((Ctrl != eTDAL_DMX_CTRL_RESET) && (Ctrl != eTDAL_DMX_CTRL_ENABLE)
			&& (Ctrl != eTDAL_DMX_CTRL_DISABLE))
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL,"Control_Filter:   "
						"Argument   Ctrl   is   an   unknown   value   [%d]\n",
						Ctrl));

		mTBOX_RETURN(kTDAL_DMX_ERROR_BAD_PARAMETERS);
	}

	/*-------------------------------------*/
	/*   Lock   Protection   mutex        */
	/*-------------------------------------*/
	mLockAccess(TDAL_DMXi_pSectionTableAccess);

	errorCode = TDAL_DMXi_Control_Filter(FilterId, Ctrl);

	/*-------------------------------------*/
	/*   Unlock   Protection   mutex        */
	/*-------------------------------------*/
	mUnLockAccess(TDAL_DMXi_pSectionTableAccess);

	mTBOX_RETURN(errorCode);
}


/**======================================================================**
 * Function Name : TDAL_DMX_Free_Filter
 *
 * Description   : this function is used to free a filter from a channel
 *
 * Side effects  :
 *
 * Return       : kTDAL_DMX_NO_ERROR if all is correct
 *             kTDAL_DMX_ERROR_INVALID_FILTER_ID if filter Id is invalid
 *             kTDAL_DMX_ERROR_MUTEX for mutex issue
 *             kTDAL_DMX_ERROR_CANT_CONTROL_FREE_FILTER if filter is not used
 *             kTDAL_DMX_ERROR_UNABLE_TO_GRANT if not able to stop or to
 *                                  deallocate the filter
 *
 * Comment       :
 *
 **======================================================================**/
tTDAL_DMX_Error   TDAL_DMX_Free_Filter(   tTDAL_DMX_FilterId   FilterId   )
{
	   tTDAL_DMX_Error   errorCode = kTDAL_DMX_NO_ERROR;

	   mTBOX_FCT_ENTER("TDAL_DMX_Free_Filter");

	   /*------------------------------------*/
	   /*   check   if   the   module   is   initialized   */
	   /*------------------------------------*/
	   if   (!TDAL_DMXi_initialized)
	   {
	      mTBOX_TRACE((kTBOX_NIV_CRITICAL,
	            "Free_Filter:   TDAL_DMX   module   has   not   been   initialized\n"));
	      mTBOX_RETURN(kTDAL_DMX_ERROR_NOT_INITIALIZED);
	   }

	   /*------------------------*/
	   /*   Check   filter   Id      */
	   /*------------------------*/
	   if   (FilterId   <   0   ||   FilterId >= kTDAL_DMXi_MAX_NB_OF_FILTERS)
	   {
	      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"Free_Filter:   "
	        "FilterId   not   valid\n"));

	      errorCode = kTDAL_DMX_ERROR_INVALID_FILTER_ID;
	   }

	   /*------------------------*/
	   /*   check   filter   state      */
	   /*------------------------*/
	   if   (TDAL_DMXi_pstFilter[FilterId].bUsed == FALSE)
	   {
	      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"Free_Filter:   "
	            "filter   (%d)   not   used\n",
	        FilterId));

	      errorCode = kTDAL_DMX_ERROR_CANT_CONTROL_FREE_FILTER;
	   }
	#if   defined(TTXT_OSD_ON) && defined(TTXT_VBI_ON)
	   /*   Never   free   the   filter   if   the   VBI   is   on   */
	   if(   (TDAL_DMXi_pstChannel[TDAL_DMXi_pstFilter[FilterId].AssociatedChannelId].ChannelStream == eTDAL_DMX_TELETEXT_STREAM)   &&
	      (TDAL_DMXi_pstFilter[FilterId].bUsed)   )
	   {
	      mTBOX_RETURN(kTDAL_DMX_NO_ERROR);
	   }
	#endif
	   /*-------------------------------------*/
	   /*   Lock   Protection   mutex        */
	   /*-------------------------------------*/
	   mLockAccess(TDAL_DMXi_pSectionTableAccess);

	   /*-----------------------------------------*/
	   /*   If   the   filter   is   still   running, stop   it   */
	   /*-----------------------------------------*/

	#if   1   /*   Why   test   bEnable=TRUE?   What   happens   if   not   enable   -->   FREE   NOT   complete   */
	   if   (errorCode == kTDAL_DMX_NO_ERROR)
	#else
	   if   (errorCode == kTDAL_DMX_NO_ERROR   &&
	      TDAL_DMXi_pstFilter[FilterId].bEnable == TRUE)
	#endif
	   {
	      errorCode = TDAL_DMXi_FreeFilterAndAssociatedRessources(FilterId);
	   }

	   if   (errorCode == kTDAL_DMX_NO_ERROR)
	   {
	      TDAL_DMXi_pstFilter[FilterId].AssociatedChannelId = kTDAL_DMX_ILLEGAL_CHANNEL;
	      TDAL_DMXi_pstFilter[FilterId].bUsed       = FALSE;
	      TDAL_DMXi_pstFilter[FilterId].bSet       = FALSE;

	      if (TDAL_DMXi_pstFilter[FilterId].SectionBufferPtr != NULL)
	      {
	            //Transfer to cache va because the orignal memory was allocated as cache va.
	            TDAL_Free(MsOS_PA2KSEG0(MsOS_VA2PA(TDAL_DMXi_pstFilter[FilterId].SectionBufferPtr)));
	      }

	      mTBOX_TRACE((kTBOX_NIV_1,"Free_Filter:   SUCCEED   [FilterlId=%d]\n",FilterId));
	   }

	   mUnLockAccess(TDAL_DMXi_pSectionTableAccess);

	   mTBOX_RETURN(errorCode);
}

/**======================================================================**
 * Function Name : TDAL_DMX_Free_Buffer
 *
 * Description   : this function is used to free a buffer
 *
 * Side effects  :
 *
 * Return       : kTDAL_DMX_NO_ERROR if all is correct
 *             kTDAL_DMX_ERROR_BAD_PARAMETERS if buffer is null
 *
 * Comment       :
 *
 **======================================================================**/
tTDAL_DMX_Error   TDAL_DMX_Free_Buffer(   void   *   buffer   )
{
	   mTBOX_FCT_ENTER("TDAL_DMX_Free_Buffer");

	   /*------------------------------------*/
	   /*   check   if   the   module   is   initialized   */
	   /*------------------------------------*/
	   if   (!TDAL_DMXi_initialized)
	   {
	      mTBOX_TRACE((kTBOX_NIV_CRITICAL,
	            "Free_Buffer:   TDAL_DMX   module   has   not   been   initialized\n"));
	      mTBOX_RETURN(kTDAL_DMX_ERROR_NOT_INITIALIZED);
	   }

	   if   (buffer == NULL)
	   {
	      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"Free_Buffer:   buffer   null   \n"));
	      mTBOX_RETURN(kTDAL_DMX_ERROR_BAD_PARAMETERS);
	   }
	   else
	   {
	      TDAL_DMXi_Section_Free(buffer);
	   }

	   mTBOX_RETURN(kTDAL_DMX_NO_ERROR);
}


/**======================================================================**
 * Function Name : TDAL_DMX_Set_FilterTid
 * Function Name : TDAL_DMX_Set_FilterWithMask
 * Function Name : TDAL_DMX_Set_FilterWithVersionInNotMatchMode
 *
 * Description   : those functions belong to an old API and we didn't have
 *             the time to implement them.
 *
 **======================================================================**/
tTDAL_DMX_ChannelId   TDAL_DMX_Set_FilterTid(   tTDAL_DMX_ChannelId   ChannelId,
                        tTDAL_DMX_FilterId   FilterId,
                        uint16_t   Filter_track,
                        uint16_t   ReqPid,
                        uint8_t   ReqTableId,
                        int32_t   ReqTableIdExtension,
                        int16_t   ReqSectionNo,
                        int8_t   ReqVersionNo,
                        int8_t   ReqCurNextIndicator   )
{
   return   (kTDAL_DMX_ILLEGAL_CHANNEL);
}


tTDAL_DMX_ChannelId   TDAL_DMX_Set_FilterWithMask(   tTDAL_DMX_ChannelId   ChannelId,
                            tTDAL_DMX_FilterId   FilterId,
                            uint16_t   ReqPid,
                            uint8_t   ReqTableId,
                            uint8_t   ReqTableIdMask,
                            int32_t   ReqTableIdExtension,
                            int16_t   ReqSectionNo,
                            int8_t   ReqVersionNo,
                            int8_t   ReqCurNextIndicator   )
{
   return   (kTDAL_DMX_ILLEGAL_CHANNEL);
}

tTDAL_DMX_ChannelId   TDAL_DMX_Set_FilterWithVersionInNotMatchMode(   tTDAL_DMX_ChannelId   ChannelId,
                                    tTDAL_DMX_FilterId   FilterId,
                                    uint16_t   ReqPid,
                                    uint8_t   ReqTableId,
                                    int32_t   ReqTableIdExtension,
                                    int16_t   ReqSectionNo,
                                    bool   NotMatchVersionMode,
                                    int8_t   ReqVersionNo,
                                    int8_t   ReqCurNextIndicator   )
{
   return   (kTDAL_DMX_ILLEGAL_CHANNEL);
}





/**======================================================================**
 * Function Name : TDAL_DMXi_LookForFreeFilter
 *
 * Description   : this function return a free filter
 *
 *      Return    FilterId if found a free filter
 *           kTDAL_DMX_ILLEGAL_FILTER otherwise
 *
 * Comment       :
 *
 **======================================================================**/
tTDAL_DMX_FilterId   TDAL_DMXi_LookForFreeFilter(   void   )
{
   int   i;
   tTDAL_DMX_FilterId   filterIdFound = kTDAL_DMX_ILLEGAL_FILTER;

   for   (i=0; i<kTDAL_DMXi_MAX_NB_OF_FILTERS; i++)
   {
	  if   (TDAL_DMXi_pstFilter[i].bUsed == FALSE)
	  {
		  filterIdFound = i;
		  break;
	  }
   }

   return   (filterIdFound);
}



/**======================================================================**
 * Function Name : TDAL_DMXi_FreeFilterAndAssociatedRessources
 *
 * Description   : filterId : id of the filter to free
 *
 * Comment       : For a VIDEO,AUDIO or PCR stream, the behaviour of this
              function is undetermined !
 *
 **======================================================================**/
tTDAL_DMX_Error   TDAL_DMXi_FreeFilterAndAssociatedRessources(
               tTDAL_DMX_FilterId   filterId
)
{

	   DMX_FILTER_STATUS dmxStatus;
	   tTDAL_DMX_Error   errorCode = kTDAL_DMX_NO_ERROR;

	   mTBOX_FCT_ENTER("TDAL_DMXi_FreeFilterAndAssociatedRessources");

	   /*--------------------------*/
	   /*   Disable   Filter   if   enable   */
	   /*--------------------------*/
	   if   (TDAL_DMXi_pstFilter[filterId].bEnable == TRUE)
	   {
	      if   (TDAL_DMXi_pstChannel[TDAL_DMXi_pstFilter[filterId].AssociatedChannelId].bEnable == TRUE)
	      {
			  /*   Disable   the   filter   */

			  dmxStatus = MApi_DMX_Stop(TDAL_DMXi_pstFilter[filterId].FilterHandle);

			  if(dmxStatus != DMX_FILTER_STATUS_OK)
			  {
				mTBOX_TRACE((kTBOX_NIV_CRITICAL,"FreeFilterAndAssociatedRessources:   " "MApi_DMX_Stop(-)   return   %d\n",dmxStatus));
				errorCode = kTDAL_DMX_ERROR_UNABLE_TO_GRANT;
			}
			else
			{
				mTBOX_TRACE((kTBOX_NIV_1,"FreeFilterAndAssociatedRessources:   " "MApi_DMX_Stop(-)   return   SUCCESS\n",dmxStatus));
			  }
	      }

	      if(errorCode == kTDAL_DMX_NO_ERROR)
	        {
	            /*   Deallocate   filter   */
	           dmxStatus = MApi_DMX_Close(TDAL_DMXi_pstFilter[filterId].FilterHandle);

	           if   (dmxStatus != DMX_FILTER_STATUS_OK)
	           {
	               mTBOX_TRACE((kTBOX_NIV_CRITICAL,"FreeFilterAndAssociatedRessources:   "
	                        "MApi_DMX_Close(0x%x)   return   %s\n",
	                     TDAL_DMXi_pstFilter[filterId].FilterHandle,
	                     dmxStatus));

	               errorCode = kTDAL_DMX_ERROR_UNABLE_TO_GRANT;
	           }
	           else
	           {
	               mTBOX_TRACE((kTBOX_NIV_1,"FreeFilterAndAssociatedRessources:   "
	                        "MApi_DMX_Close(0x%x)   return   SUCCESS\n",
	                     TDAL_DMXi_pstFilter[filterId].FilterHandle,
	                     dmxStatus));
	           }
	        }
	   }


	   /*---------------------*/
	   /*   Free   OS   resources   */
	   /*---------------------*/

	if ((errorCode == kTDAL_DMX_NO_ERROR) && (TDAL_DMXi_pstFilter[filterId].AllocateHWFilter == TRUE))
	   {
		   if(TDAL_DMXi_pstFilter[filterId].Filter != NULL)
		   {
			  TDAL_Free(TDAL_DMXi_pstFilter[filterId].Filter);
			  mTBOX_TRACE((kTBOX_NIV_1,"FreeFilterAndAssociatedRessources:   "
				"Free   Filter = 0x%x\n",
				TDAL_DMXi_pstFilter[filterId].Filter));
			  TDAL_DMXi_pstFilter[filterId].Filter = NULL;
		   }
		
		   if(TDAL_DMXi_pstFilter[filterId].PositiveMask != NULL)
		   {
			  TDAL_Free(TDAL_DMXi_pstFilter[filterId].PositiveMask);
			  mTBOX_TRACE((kTBOX_NIV_1,"FreeFilterAndAssociatedRessources:   "
				"Free   PositiveMask = 0x%x\n",
				TDAL_DMXi_pstFilter[filterId].PositiveMask));
			  TDAL_DMXi_pstFilter[filterId].PositiveMask = NULL;
		   }

		   if(TDAL_DMXi_pstFilter[filterId].NegativeMask != NULL)
		   {
			  TDAL_Free(TDAL_DMXi_pstFilter[filterId].NegativeMask);
			  mTBOX_TRACE((kTBOX_NIV_1,"FreeFilterAndAssociatedRessources:   "
				"Free   NegativeMask = 0x%x\n",
				TDAL_DMXi_pstFilter[filterId].NegativeMask));
			  TDAL_DMXi_pstFilter[filterId].NegativeMask = NULL;
		   }
	   }

	   /*---------------------*/
	   /*   RAZ   filter   variable   */
	   /*---------------------*/
	   if(errorCode == kTDAL_DMX_NO_ERROR)
	   {
	      TDAL_DMXi_pstFilter[filterId].FilterId   = filterId;
	      TDAL_DMXi_pstFilter[filterId].bUsed       = FALSE;
	      TDAL_DMXi_pstFilter[filterId].bEnable   = FALSE;
	      TDAL_DMXi_pstFilter[filterId].AssociatedChannelId = kTDAL_DMX_ILLEGAL_CHANNEL;
	      TDAL_DMXi_pstFilter[filterId].bSet       = FALSE;
	      TDAL_DMXi_pstFilter[filterId].Tag       = 0;
	      TDAL_DMXi_pstFilter[filterId].FilterHandle = 0;
	      TDAL_DMXi_pstFilter[filterId].bWaitingEnable = FALSE;

	      mTBOX_TRACE((kTBOX_NIV_1,"FreeFilterAndAssociatedRessources:   SUCCEED\n"));
	   }

	   mTBOX_RETURN(errorCode);

}

tTDAL_DMX_Error   TDAL_DMXi_GetPidFilter(tTDAL_DMX_ChannelStream streamType, uint16_t *pid, uint16_t *filt)
 {
        mTBOX_FCT_ENTER("TDAL_DMXi_GetPidFilter");
        uint8_t i = 0;
        if(pid == NULL || filt == NULL)
        {
            mTBOX_RETURN(kTDAL_DMX_ERROR_BAD_PARAMETERS);
        }
        for   (i=0; i < kTDAL_DMXi_MAX_NB_OF_FILTERS; i++)
        {
            if(TDAL_DMXi_pstChannel[TDAL_DMXi_pstFilter[i].AssociatedChannelId].ChannelStream == streamType)
            {
                *pid = TDAL_DMXi_pstChannel[TDAL_DMXi_pstFilter[i].AssociatedChannelId].ChannelPid;
                *filt = TDAL_DMXi_pstFilter[i].FilterHandle;
                mTBOX_RETURN(kTDAL_DMX_NO_ERROR);
            }
        }
        mTBOX_RETURN(kTDAL_DMX_ERROR_BAD_PARAMETERS);
    }

tTDAL_DMX_Error TDAL_DMXi_GetFilterByPid(uint16_t pid, uint16_t *filt)
{
    mTBOX_FCT_ENTER("TDAL_DMXi_GetFilterByPid");
    uint8_t i = 0;
    if (pid == NULL || filt == NULL)
    {
        mTBOX_RETURN(kTDAL_DMX_ERROR_BAD_PARAMETERS);
    }
    for (i = 0; i < kTDAL_DMXi_MAX_NB_OF_FILTERS; i++)
    {
        if (TDAL_DMXi_pstChannel[TDAL_DMXi_pstFilter[i].AssociatedChannelId].ChannelPid == pid)
        {
            *filt = TDAL_DMXi_pstFilter[i].FilterHandle;
            mTBOX_RETURN(kTDAL_DMX_NO_ERROR);
        }
    }
    mTBOX_RETURN(kTDAL_DMX_ERROR_BAD_PARAMETERS);
}
