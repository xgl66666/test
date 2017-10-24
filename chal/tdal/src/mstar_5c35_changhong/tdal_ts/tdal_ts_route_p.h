/******************************************************************************
 *                  COPYRIGHT 2006 IWEDIA TECHNOLOGIES             *
 ******************************************************************************
 *
 * MODULE NAME: TDAL_TS_ROUTE  -  TS_ROUTE Management driver
 *
 * FILE NAME:   TDAL_TS_ROUTE_P.H
 *   $URL: http://svnsrv/svn/iwedia-ext/sources/chal/core/tdal/branches/idecode_hd/os21/tdal_ts/tdal_ts_route_p.h $
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

#ifndef   _TDAL_TS_ROUTE_P_H_
#define   _TDAL_TS_ROUTE_P_H_

/*******************************************************************************
   *   Includes
   ******************************************************************************/
#ifndef   _TDAL_TS_H_
#error   "tdal_ts.h   must   be   included   before   tdal_ts_route_p.h"
#endif

#ifdef   __cplusplus
extern   "C"   {
#endif


/*******************************************************************************
   *   Defines
   ******************************************************************************/
#if   defined(USE_TDAL_TS_SOURCE_SINK)
#if   (NB_TUNERS==2)
#define   kTDAL_TSROUTEi_NB_SOURCES   3
#define   kTDAL_TSROUTEi_NB_DEMUX   3
#else
#define   kTDAL_TSROUTEi_NB_SOURCES   2
#define   kTDAL_TSROUTEi_NB_DEMUX   2
#endif
#else
#define   kTDAL_TSROUTEi_NB_SOURCES   1
#define   kTDAL_TSROUTEi_NB_DEMUX   1
#endif
/*   Result   returned   by   the   function   p_TDAL_TSROUTE_CheckObjectIndex()   */
#define   kTDAL_TS_OBJECT_INVALID_INDEX      0xFF


/*******************************************************************************
   *   Macros
   ******************************************************************************/
#define   mTDAL_TSROUTE_CheckInit()                     \
      if   (!tdal_tsroute_is_initialised)               \
      {                              \
        mTBOX_TRACE((kTBOX_NIV_CRITICAL,               \
               "TDAL_TSROUTE   is   not   initialized   !\n"));   \
        mTBOX_RETURN(   eTDAL_TSROUTE_ERROR_NOT_INIT   );      \
      }


/*******************************************************************************
   *   Typedefs
   ******************************************************************************/


/*******************************************************************************
   *   Structures
   ******************************************************************************/
struct   AvailableSource   {
   tTDAL_TS_Object      source   ;
   tTDAL_TS_Object      connected_demux   ;
};
struct   AvailableDemux   {
   tTDAL_TS_Object      demux   ;
   tTDAL_TS_Object      connected_source   ;
};


/*******************************************************************************
   *   Variables   Declarations   (IMPORT)
   ******************************************************************************/


/*******************************************************************************
   *   Functions   Declarations   (MODULE)
   ******************************************************************************/
      /*   Main   loop   of   the   injecting   thread   */
//uint8_t        p_TDAL_TSROUTE_CheckObjectIndex(   tTDAL_TS_Object   object   );
//bool        p_TDAL_TSROUTE_ConfigureTSMERGEObject(   tTDAL_TS_Object      tdal_ts_object   ,
//                              STMERGE_ObjectConfig_t   *   p_stmerge_object   );
//STMERGE_ObjectId_t   p_TDAL_TSROUTE_GetTSMERGESource(   tTDAL_TS_Object   object   );
//STMERGE_ObjectId_t   p_TDAL_TSROUTE_GetTSMERGEDemux(   tTDAL_TS_Object   object   );
//ST_DeviceName_t   *   p_TDAL_TSROUTE_GetPTIDeviceName(   tTDAL_TS_Object   object   );
//STPTI_StreamID_t   p_TDAL_TSROUTE_GetPTIStreamID(   tTDAL_TS_Object   object   );
#ifdef   __cplusplus
}
#endif

#endif   /*   _TDAL_TSIN_P_H_   */
