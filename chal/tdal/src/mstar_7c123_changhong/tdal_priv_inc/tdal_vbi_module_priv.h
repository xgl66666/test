/******************************************************************************
 *                  COPYRIGHT 2004 IWEDIA TECHNOLOGIES            *
 ******************************************************************************
 *
 * MODULE NAME: TDAL_PRIV
 *
 * FILE NAME: $URL: 
 *        $Rev: 
 *        $Date: 
 *
 * Description           : library primitives private tdal header.
 *
 *****************************************************************************/

#ifndef   _TDAL_VBI_MODULE_PRIV_H_
#define   _TDAL_VBI_MODULE_PRIV_H_

#ifdef   __cplusplus
extern   "C"   {
#endif
   
   
/*--------------------------------------*/
/*      Public   types   definition      */
/*--------------------------------------*/

   
/*------------------------------------------*/
/*      Public   Functions   Declarations      */
/*------------------------------------------*/
#if   defined(TTXT_OSD_ON)   &&   defined(TTXT_VBI_ON)

void   TDAL_VBI_PESArrivalCallBack(signed   char      ChannelId,   /*   Unused*/
                   signed   char      FilterId,      /*   Unused*/
                   uint32_t        Filter_track,   /*   Unused*/
                   uint32_t        Length,      /*   length   of   the   buffer   */
                   uint8_t        *Buffer);      /*   Data   */
#endif   

#ifdef   __cplusplus
}
#endif

#endif   /*_TDAL_VBI_MODULE_PRIV_H_*/
