/******************************************************************************
 *                  COPYRIGHT 2004 IWEDIA TECHNOLOGIES            *
 ******************************************************************************
 *
 * MODULE NAME: TDAL_PRIV
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/sources/chal/core/tdal/branches/idecode_hd/os21/tdal_priv_inc/tdal_av_module_priv.h $
 *        $Rev: 673 $
 *        $Date: 2011-10-28 15:10:43 +0200 (Fri, 28 Oct 2011) $
 *
 * Description           : library primitives private tdal header.
 *
 *****************************************************************************/

#ifndef   _TDAL_AV_MODULE_PRIV_H_
#define   _TDAL_AV_MODULE_PRIV_H_

#ifdef   __cplusplus
extern   "C"   {
#endif

#include "tdal_av.h"

/*--------------------------------------*/
/*      Public   types   definition      */
/*--------------------------------------*/


/*------------------------------------------*/
/*      Public   Functions   Declarations      */
/*------------------------------------------*/

extern bool TDAL_AVm_GetDecoderStatus(VDEC_DispInfo * pVidStatus);
extern tTDAL_AV_AudioType TDAL_AV_Audio_StreamType(void);
extern bool TDAL_AVm_SetAudioNeedUnmute(bool NeedUnmute);

#ifdef   __cplusplus
}
#endif

#endif   /*_TDAL_AV_MODULE_PRIV_H_*/
