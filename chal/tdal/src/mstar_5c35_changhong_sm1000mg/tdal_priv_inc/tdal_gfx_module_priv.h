/******************************************************************************
 *                  COPYRIGHT 2004 IWEDIA TECHNOLOGIES            *
 ******************************************************************************
 *
 * MODULE NAME: TDAL_PRIV
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/sources/chal/core/tdal/branches/idecode_hd/os21/tdal_priv_inc/tdal_gfx_module_priv.h $
 *        $Rev: 414 $
 *        $Date: 2011-09-23 12:34:21 +0200 (Fri, 23 Sep 2011) $
 *
 * Description           : library primitives private tdal header.
 *
 *****************************************************************************/

#ifndef   _TDAL_GFX_MODULE_PRIV_H_
#define   _TDAL_GFX_MODULE_PRIV_H_

#include   "stblit.h"

#ifdef   __cplusplus
extern   "C"   {
#endif

/*--------------------------------------*/
/*      Public   types   definition      */
/*--------------------------------------*/

/*------------------------------------------*/
/*      Public   Functions   Declarations      */
/*------------------------------------------*/

bool   TDAL_GFXm_RegionHideAll(   tTDAL_DISP_LayerId   Id   );
bool   TDAL_GFXm_RegionShowAll(   tTDAL_DISP_LayerId   Id   );

bool   TDAL_GFXm_ResolutionOpenViewPort(   tTDAL_DISP_LayerId   Id   );
bool   TDAL_GFXm_ResolutionCloseViewPort(   tTDAL_DISP_LayerId   Id   );

STBLIT_Handle_t   TDAL_GFXmGetBlitHandle(void);
bool   TDAL_GFXm_BlenderResize(tTDAL_DISP_BlenderId   Id);
bool   TDAL_GFXm_FillBlock2D_Blit(   uint32_t   Xpos,   uint32_t   Ypos,   uint32_t   Wpos,   uint32_t   Hpos,
                   STGXOBJ_Bitmap_t   *pBitmapDest,   STGXOBJ_Color_t   *pColor,   uint32_t   vtg);

#if   defined(ST_USED_STBUFFER)
ST_ErrorCode_t   TDAL_GFXm_STBufferAllocate(STGXOBJ_BitmapAllocParams_t   bitmapAllocParams,
                        STGXOBJ_Bitmap_t   *Bitmap,
                        U32   *bitmapHandle);
#endif

#ifdef   __cplusplus
}
#endif

#endif   /*_TDAL_GFX_MODULE_PRIV_H_*/
