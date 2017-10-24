/******************************************************************************
 *                  COPYRIGHT 2006 IWEDIA TECHNOLOGIES             *
 ******************************************************************************
 *
 * MODULE NAME: TDAL_TS_ROUTE  -  TS_ROUTE Management driver
 *
 * FILE NAME:   TDAL_TS_ROUTE.C
 *   $URL: http://svnsrv/svn/iwedia-ext/sources/chal/core/tdal/branches/idecode_hd/os21/tdal_ts/tdal_ts_route.c $
 *   $Rev: 414 $
 *   $Date: 2011-09-23 12:34:21 +0200 (Fri, 23 Sep 2011) $
 *
 * PUBLIC
 *
 * DESCRIPTION - This driver is divided in 3 parts :
 *            - TDAL_TS_ROUTE  : Connect a TS source and a demux
 *            - TDAL_TS_SINK   : Inject a TS to a demux or to an external sink
 *            - TDAL_TS_SOURCE : Extract a TS from a demux or an external source
 *
 *****************************************************************************/

/******************** INFOS ABOUT TRACES LEVEL ********************
 ** NIV_2 :                                     **
 ** NIV_4 :                                     **
 ******************************************************************/



/************************************************************************
 *   Includes
 ***********************************************************************/
#include <string.h>

#include "crules.h"

#include "tdal_common.h"
#include "tdal_ts.h"
#include "tdal_ts_route_p.h"

#include "tbox.h"


/************************************************************************
 *   Defines
 ***********************************************************************/


/************************************************************************
 *   Macros
 ***********************************************************************/


/************************************************************************
 *   Typedefs
 ***********************************************************************/


/************************************************************************
 *   Global Variables (GLOBAL/IMPORT)
 ***********************************************************************/

/************************************************************************
 *   Local Module Variables (MODULE)
 ***********************************************************************/


/************************************************************************
 *   Local File Variables (LOCAL)
 ***********************************************************************/
mTBOX_SET_MODULE(eTDAL_TS_ROUTE);

LOCAL   struct AvailableSource  src_connection[kTDAL_TSROUTEi_NB_SOURCES];
LOCAL   struct AvailableDemux   dst_connection[kTDAL_TSROUTEi_NB_DEMUX];

LOCAL   tTDAL_TS_Object      src_available[kTDAL_TSROUTEi_NB_SOURCES];
LOCAL   tTDAL_TS_Object      dst_available[kTDAL_TSROUTEi_NB_DEMUX];

LOCAL   bool              tdal_tsroute_is_initialised = false;


/************************************************************************
 *   Local Functions Declarations (LOCAL)
 ***********************************************************************/
/* see tdal_ts_route_p.h for internal functions */



/************************************************************************
 *   Exported Functions Definitions  (GLOBAL)
 ***********************************************************************/


/**----------------------------------------------------------------------
 * Function Name : TDAL_TSROUTE_Init
 *
 * Description :   Initializes the TDAL_TSROUTE driver
 *
 * Side effects :
 *
 * Comment :
 **--------------------------------------------------------------------**/
tTDAL_TSROUTE_Error    TDAL_TSROUTE_Init( void )
{
	uint8_t i;

	/* ARV : Currently, the source:demux avaibility is done manually ...
	 it should be dynamic */
	src_available[0] =
			mTDAL_TS_GetObjectId( eTDAL_TS_CLASS_SOURCE , eTDAL_TS_TYPE_TUNER , 0 );
#if defined(USE_TDAL_TS_SOURCE_SINK)
#if (NB_TUNERS==2)
	src_available[1] = mTDAL_TS_GetObjectId( eTDAL_TS_CLASS_SOURCE , eTDAL_TS_TYPE_TUNER , 1 );
	src_available[2] = mTDAL_TS_GetObjectId( eTDAL_TS_CLASS_SOURCE , eTDAL_TS_TYPE_MEMORY , 0 );
#else
	src_available[1] = mTDAL_TS_GetObjectId( eTDAL_TS_CLASS_SOURCE , eTDAL_TS_TYPE_MEMORY , 0 );
#endif
#endif
	for (i = 0; i < kTDAL_TSROUTEi_NB_SOURCES; i++)
	{
		src_connection[i].source = src_available[i];
		src_connection[i].connected_demux = kTDAL_TS_NO_OBJECT;
		mTBOX_TRACE((kTBOX_NIV_1, "TDAL_TSROUTE_Init: Source[%d]=0x%x\n", i, src_connection[i].source ));
	}
	for (i = 0; i < kTDAL_TSROUTEi_NB_DEMUX; i++)
	{
		dst_available[i] =
				mTDAL_TS_GetObjectId( eTDAL_TS_CLASS_DESTINATION , eTDAL_TS_TYPE_DEMUX , i );
		dst_connection[i].demux = dst_available[i];
		dst_connection[i].connected_source = kTDAL_TS_NO_OBJECT;
		mTBOX_TRACE((kTBOX_NIV_1, "TDAL_TSROUTE_Init:  Demux[%d]=0x%x\n", i, dst_connection[i].demux ));
	}
	tdal_tsroute_is_initialised = true;
	return eTDAL_TSROUTE_NO_ERROR;
}


/**----------------------------------------------------------------------
 * Function Name : TDAL_TSROUTE_Term
 *
 * Description :   Terminates the TDAL_TSROUTE driver
 *
 * Side effects :
 *
 * Comment :
 **--------------------------------------------------------------------**/
tTDAL_TSROUTE_Error    TDAL_TSROUTE_Term( void )
{
	mTBOX_FCT_ENTER("TDAL_TSROUTE_Term()");
	mTDAL_TSROUTE_CheckInit()
	tdal_tsroute_is_initialised = false;
	mTBOX_RETURN(eTDAL_TSROUTE_NO_ERROR);
}

/**----------------------------------------------------------------------
 * Function Name :  TDAL_TSROUTE_GetCapabilities
 *
 * Description :   Get hardware capabilities
 *
 * Side effects :   ...
 *
 * Comment :
 **--------------------------------------------------------------------**/
tTDAL_TSROUTE_Error    TDAL_TSROUTE_GetCapabilities(
                   tTDAL_TSROUTE_Capabilities *   p_capabilities
                 )
{
	mTBOX_FCT_ENTER("TDAL_TSROUTE_GetCapabilities()");

	mTDAL_TSROUTE_CheckInit()

	if (p_capabilities == NULL)
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_TSROUTE_GetCapabilities: 'p_capabilities' is NULL\n"));
		mTBOX_RETURN(eTDAL_TSROUTE_ERROR_BAD_ARG);
	}

	p_capabilities->nb_sources = kTDAL_TSROUTEi_NB_SOURCES;
	p_capabilities->sources_array = src_available;
	p_capabilities->nb_destinations = kTDAL_TSROUTEi_NB_DEMUX;
	p_capabilities->destinations_array = dst_available;

	mTBOX_RETURN(eTDAL_TSROUTE_NO_ERROR);
}


/**----------------------------------------------------------------------
 * Function Name :  TDAL_TSROUTE_Connect
 *
 * Description :   Make a route (=connection) between two objects
 *
 * Side effects :   ...
 *
 * Comment :
 **--------------------------------------------------------------------**/
tTDAL_TSROUTE_Error    TDAL_TSROUTE_Connect(
                   tTDAL_TS_Object             source ,
                   tTDAL_TS_Object              destination
                 )
{
   return eTDAL_TSROUTE_NO_ERROR;
}



/**----------------------------------------------------------------------
 * Function Name :  TDAL_TSROUTE_Disconnect
 *
 * Description :   Remove a route (=disconnection) between two objects
 *
 * Side effects :   ...
 *
 * Comment :
 **--------------------------------------------------------------------**/
tTDAL_TSROUTE_Error    TDAL_TSROUTE_Disconnect(
                   tTDAL_TS_Object              source ,
                   tTDAL_TS_Object              destination
                 )
{
   return eTDAL_TSROUTE_NO_ERROR;}


/**----------------------------------------------------------------------
 * Function Name :  TDAL_TSROUTE_GetStatus
 *
 * Description :   Get the connection status of an object
 *
 * Side effects :   ...
 *
 * Comment :
 **--------------------------------------------------------------------**/
tTDAL_TSROUTE_Error    TDAL_TSROUTE_GetStatus(
                   tTDAL_TS_Object             object ,
                   tTDAL_TSROUTE_Status *       p_status
                 )
{
   return eTDAL_TSROUTE_NO_ERROR;
}



/**----------------------------------------------------------------------
 * Function Name :  TDAL_TSROUTEm_SetParam
 *
 * Description :
 *
 * Side effects :
 *
 * Comment :
 *
 **--------------------------------------------------------------------**/
bool    TDAL_TSROUTEm_SetParam( void )
{
   return ( TRUE );
}


