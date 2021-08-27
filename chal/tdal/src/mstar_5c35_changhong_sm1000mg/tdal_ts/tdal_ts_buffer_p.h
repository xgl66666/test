/******************************************************************************
 *                  COPYRIGHT 2004 IWEDIA TECHNOLOGIES             *
 ******************************************************************************
 *
 * MODULE NAME: TDAL_TSSOURCE and TDAL_TSSINK  -  Common types
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/sources/chal/core/tdal/branches/idecode_hd/os21/tdal_ts/tdal_ts_buffer_p.h $
 *        $Rev: 414 $
 *        $Date: 2011-09-23 12:34:21 +0200 (Fri, 23 Sep 2011) $
 *
 * PUBLIC
 *
 * DESCRIPTION - declaration of common types for TDAL_TSSOURCE and TDAL_TSSINK
 *
 *****************************************************************************/

#ifndef   _TDAL_TS_BUFFER_P_H_
#define   _TDAL_TS_BUFFER_P_H_

/*******************************************************************************
   *   Includes
   ******************************************************************************/

#ifdef   __cplusplus
extern   "C"   {
#endif


/******************************************************************************
   *   Defines
   ******************************************************************************/
#define   kTDAL_TSBUFFER_MAX_NB_CONSUMERS   5

#define   TDAL_TS_CIRCULAR_BUFFER_AVMEM
/*#define   TDAL_TS_CIRCULAR_BUFFER_TDAL*/


/******************************************************************************
   *   Macros
   ******************************************************************************/

/*******************************************************************************
   *   Structures
   ******************************************************************************/
struct   TDAL_TS_Consumer_s   {
   bool            used;
   uint8_t   *            consumer;
};

struct   TDAL_TS_CircularBuffer_s   {
   uint8_t   *            address;
   uint8_t   *            address_k;   /*   address   (accessible   in   kernel   mode)   (aligned   using   AVMEM)   */
   uint8_t   *            address_u;   /*   non   aligned   address   */
#if   defined(TDAL_TS_CIRCULAR_BUFFER_AVMEM)
   //STAVMEM_BlockHandle_t      avmem_buffer_handle;
#endif
   uint32_t            length;
   uint8_t   *            producer;
   TDAL_mutex_id        mutex_consumer;   /*   protect   access   to   the   'consumers'   array   */
   struct   TDAL_TS_Consumer_s   consumers[kTDAL_TSBUFFER_MAX_NB_CONSUMERS];
};


/******************************************************************************
   *   Typedefs
   ******************************************************************************/
typedef   struct   TDAL_TS_CircularBuffer_s   tTDAL_TSSOURCE_CircularBuffer;
typedef   struct   TDAL_TS_CircularBuffer_s   tTDAL_TSSINK_CircularBuffer;


#ifdef   __cplusplus
}
#endif

#endif   /*   _TDAL_TSBUFFER_P_H_   */
