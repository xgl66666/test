/******************************************************************************
 *                  COPYRIGHT 2004 IWEDIA TECHNOLOGIES            *
 ******************************************************************************
 *
 * MODULE NAME: TDAL_PRIV
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/sources/chal/core/tdal/branches/idecode_hd/os21/tdal_priv_inc/tdal_mp_module_priv.h $
 *        $Rev: 673 $
 *        $Date: 2011-10-28 15:10:43 +0200 (Fri, 28 Oct 2011) $
 *
 * Description           : library primitives private tdal header.
 *
 *****************************************************************************/

#ifndef   _TDAL_MP_MODULE_PRIV_H_
#define   _TDAL_MP_MODULE_PRIV_H_

#ifdef   __cplusplus
extern   "C"   {
#endif

#include "tdal_mp.h"

/*--------------------------------------*/
/*      Public   types   definition      */
/*--------------------------------------*/


/*------------------------------------------*/
/*      Public   Functions   Declarations      */
/*------------------------------------------*/

IMPORT bool TDAL_MPi_GetDecoderStatus(void);
IMPORT void TDAL_MPm_UsbRemoved(void);
IMPORT uint32_t TDAL_MPm_GetPhotoPitch(void);
IMPORT uint16_t TDAL_MPm_GetPhotoFormat(void);

#ifdef   __cplusplus
}
#endif

#endif   /*_TDAL_MP_MODULE_PRIV_H_*/
