/******************************************************************************
 *                  COPYRIGHT 2004 IWEDIA TECHNOLOGIES            *
 ******************************************************************************
 *
 * MODULE NAME: TDAL_PRIV
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/sources/chal/core/tdal/branches/idecode_hd/os21/tdal_priv_inc/tdal_dmx_module_priv.h $
 *        $Rev: 414 $
 *        $Date: 2011-09-23 12:34:21 +0200 (Fri, 23 Sep 2011) $
 *
 * Description           : library primitives private tdal header.
 *
 *****************************************************************************/

#ifndef _TDAL_DMX_MODULE_PRIV_H_
#define _TDAL_DMX_MODULE_PRIV_H_

#ifdef __cplusplus
extern "C" {
#endif


/*--------------------------------------*/
/*      Public types definition      */
/*--------------------------------------*/


/*------------------------------------------*/
/*       Public Functions Declarations      */
/*------------------------------------------*/
bool TDAL_DMXm_SetDmxFlow(void);
void TDAL_DMXm_DisableFilters(bool disable);


#ifdef __cplusplus
}
#endif

#endif /*_TDAL_DMX_MODULE_PRIV_H_*/
