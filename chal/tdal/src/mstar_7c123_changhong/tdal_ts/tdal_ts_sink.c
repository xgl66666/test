/******************************************************************************
 *                  COPYRIGHT 2006 IWEDIA TECHNOLOGIES             *
 ******************************************************************************
 *
 * MODULE NAME: TDAL_TS_SINK  -  TS_SINK driver
 *
 * FILE NAME:   TDAL_TS_SINK.C
 *   $URL: http://svnsrv/svn/iwedia-ext/sources/chal/core/tdal/branches/idecode_hd/os21/tdal_ts/tdal_ts_sink.c $
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

/************************************************************************
   *   Includes
   ***********************************************************************/
#include "string.h"


#include "crules.h"

#include "tbox.h"

#include "tdal_common.h"
#include "tdal_ts.h"
#include "tdal_av.h"




/************************************************************************
   *   Macros
   ***********************************************************************/


/************************************************************************
   *   Typedefs
   ***********************************************************************/


/************************************************************************
   *   Global   Variables   (GLOBAL/IMPORT)
   ***********************************************************************/
/************************************************************************
   *   Local   Module   Variables   (MODULE)
   ***********************************************************************/

/************************************************************************
   *   Local   File   Variables   (LOCAL)
   ***********************************************************************/
/************************************************************************
   *   Local   Functions   Declarations   (LOCAL)
   ***********************************************************************/
/*   see   tdal_ts_sink_p.h   for   internal   functions   */



/************************************************************************
   *   Exported   Functions   Definitions   (GLOBAL)
   ***********************************************************************/


/**----------------------------------------------------------------------
 * Function Name : TDAL_TSSINK_Init
 *
 * Description :   Initializes the TDAL_TSSINK driver
 *
 * Side effects :
 *
 * Comment :
 **--------------------------------------------------------------------**/
GLOBAL   tTDAL_TSSINK_Error   TDAL_TSSINK_Init(   void   )
{
   return   eTDAL_TSSINK_NO_ERROR;
}


/**----------------------------------------------------------------------
 * Function Name : TDAL_TSSINK_Term
 *
 * Description :   Terminates the TDAL_TSSINK driver
 *
 * Side effects :
 *
 * Comment :
 **--------------------------------------------------------------------**/
GLOBAL   tTDAL_TSSINK_Error   TDAL_TSSINK_Term(   void   )
{
   return   eTDAL_TSSINK_NO_ERROR;
}


/**----------------------------------------------------------------------
 * Function Name : TDAL_TSSINK_Open
 *
 * Description :   Initialize a new injection
 *
 * Side effects :  It allocates an tTDAL_TSSINK_Injection_s structure and some space for
 *             buffers using TDAL_Malloc.
 *             If there is not enough memory for all buffers, the
 *             function continues and modifies the field
 *             p_oprm.nb_buffers to make it contains the nb of
 *             buffers really allocated. In this case, the function
 *             return the value : TDAL_TSSINK_WARNING_MEMORY.
 *
 * Comment :
 **--------------------------------------------------------------------**/
GLOBAL   tTDAL_TSSINK_Error   TDAL_TSSINK_Open(
                 tTDAL_TSSINK_Handle   *      p_handle,
                 tTDAL_TSSINK_OpenParam   *   p_oprm
               )
{

    return eTDAL_TSSINK_NO_ERROR;
}


/**----------------------------------------------------------------------
 * Function Name :  TDAL_TSSINK_Close
 *
 * Description :   Closes an injection
 *
 * Side effects :   It modifies 'the_injection' structure and calls
 *            TDAL_TSSINK_Stop (if needed) and TDAL_Free.
 *            If an error occurs in these functions, it continues
 *            the execution and returns an error code.
 *
 * Comment :
 **--------------------------------------------------------------------**/
GLOBAL   tTDAL_TSSINK_Error   TDAL_TSSINK_Close(   tTDAL_TSSINK_Handle   handle   )   {
      return eTDAL_TSSINK_NO_ERROR;
}

/**----------------------------------------------------------------------
 * Function Name :  TDAL_TSSINK_SetPosition
 *
 * Description :   Set a position in the stream
 *
 * Side effects :
 *
 * Comment :
 **--------------------------------------------------------------------**/
GLOBAL   tTDAL_TSSINK_Error   TDAL_TSSINK_SetTimePosition(
                 tTDAL_TSSINK_Handle      handle,
                 bool            relative,
                 signed   int            timeInMs
               )
{
    return eTDAL_TSSINK_NO_ERROR;

}

/**----------------------------------------------------------------------
 * Function Name :  TDAL_TSSINK_GetPacketPosition
 *
 * Description :
 *
 * Side effects :
 *
 * Comment :
 **--------------------------------------------------------------------**/
GLOBAL   tTDAL_TSSINK_Error   TDAL_TSSINK_GetPacketPosition(
                 tTDAL_TSSINK_Handle   handle,
                 uint32_t   *      p_packet_position
               )
{
   return eTDAL_TSSINK_NO_ERROR;

}

/**----------------------------------------------------------------------
 * Function Name :  TDAL_TSSINK_GetStatus
 *
 * Description :   Get status information about an injection
 *
 * Side effects :
 *
 * Comment :
 **--------------------------------------------------------------------**/
GLOBAL   tTDAL_TSSINK_Error   TDAL_TSSINK_GetStatus(
                 tTDAL_TSSINK_Handle      handle,
                 tTDAL_TSSINK_Status   *      p_status
               )
{
    return eTDAL_TSSINK_NO_ERROR;

}



/**----------------------------------------------------------------------
 * Function Name :  TDAL_TSSINK_Internal_SetPosition
 *
 * Description :   Set a position in the stream
 *
 * Side effects :
 *
 * Comment :
 **--------------------------------------------------------------------**/
/*   TODEL :   TO   BE   REMOVED   */
GLOBAL      tTDAL_TSSINK_Error   TDAL_TSSINK_Internal_SetPosition(
                 tTDAL_TSSINK_Handle      handle,
                 bool            relative,
                 signed   int            timeInMs)
{

    return eTDAL_TSSINK_NO_ERROR;
}


/**----------------------------------------------------------------------
 * Function Name :  TDAL_TSSINK_Start
 *
 * Description :   Start injecting data
 *
 * Side effects :
 *
 * Comment :
 **--------------------------------------------------------------------**/
GLOBAL   tTDAL_TSSINK_Error   TDAL_TSSINK_Start(
                 tTDAL_TSSINK_Handle      handle   ,
                 tTDAL_TSSINK_StartParam   *   p_sprm
               )
{
   return eTDAL_TSSINK_NO_ERROR;
}


/**----------------------------------------------------------------------
 * Function Name :  TDAL_TSSINK_Start
 *
 * Description :   Start the sink
 *
 * Side effects :
 *
 * Comment :
 **--------------------------------------------------------------------**/
GLOBAL   tTDAL_TSSINK_Error   TDAL_TSSINK_StartFrom(
                 tTDAL_TSSINK_Handle      handle   ,
                 tTDAL_TSSINK_Handle      predecessor   ,
                 tTDAL_TSSINK_StartParam   *   p_sprm
               )
{

    return eTDAL_TSSINK_NO_ERROR;
}



/**----------------------------------------------------------------------
 * Function Name :  TDAL_TSSINK_Stop
 *
 * Description :   Stop the injection of data
 *
 * Side effects :   It stops the injecting thread. It calls
 *            TKEL_WaitSemaphore, TKEL_DeleteSemaphore. If an
 *            error occurs, it tries to continue and returns an
 *            error code.
 *
 * Comment :
 **--------------------------------------------------------------------**/
GLOBAL   tTDAL_TSSINK_Error   TDAL_TSSINK_Stop(   tTDAL_TSSINK_Handle   handle   )
{
   return eTDAL_TSSINK_NO_ERROR;
}

/**----------------------------------------------------------------------
 * Function Name :  TDAL_TSSINK_SetConsumerCallback
 *
 * Description :
 *
 * Side effects :
 *
 * Comment :
 **--------------------------------------------------------------------**/
GLOBAL   tTDAL_TSSINK_Error   TDAL_TSSINK_SetConsumerCallback(
                 tTDAL_TSSINK_Handle            handle,
                 tTDAL_TSSINK_ConsumerCallbackFct   consumer_callback,
                 void   *                 client_tag
)   {
   tTDAL_TSSINK_Error            my_err = eTDAL_TSSINK_NO_ERROR;
   return (   my_err   );

}


/**----------------------------------------------------------------------
 * Function Name :  TDAL_TSSINK_NotifyConsumption
 *
 * Description :
 *
 * Side effects :
 *
 * Comment :
 **--------------------------------------------------------------------**/
GLOBAL   tTDAL_TSSINK_Error   TDAL_TSSINK_NotifyConsumption(
                 tTDAL_TSSINK_Handle      handle,
                 uint32_t            consumed_packets,
                 uint32_t            notification_level
)   
{
   tTDAL_TSSINK_Error            my_err = eTDAL_TSSINK_NO_ERROR;
   return (   my_err   );

}


/**----------------------------------------------------------------------
 * Function Name :  TDAL_TSSINK_SetSource
 *
 * Description :
 *
 * Side effects :
 *
 * Comment :
 **--------------------------------------------------------------------**/
GLOBAL   tTDAL_TSSINK_Error   TDAL_TSSINK_SetTSSource(
                 tTDAL_TSSINK_Handle      handle,
                 tTDAL_TSSOURCE_Handle      tssource_handle
)   {
   tTDAL_TSSINK_Error            my_err = eTDAL_TSSINK_NO_ERROR;
   return (   my_err   );
}

