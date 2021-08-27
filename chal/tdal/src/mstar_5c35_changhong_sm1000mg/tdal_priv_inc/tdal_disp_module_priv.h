/******************************************************************************
 *                  COPYRIGHT 2004 IWEDIA TECHNOLOGIES            *
 ******************************************************************************
 *
 * MODULE NAME: TDAL_PRIV
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/sources/chal/core/tdal/branches/idecode_hd/os21/tdal_priv_inc/tdal_disp_module_priv.h $
 *        $Rev: 414 $
 *        $Date: 2011-09-23 12:34:21 +0200 (Fri, 23 Sep 2011) $
 *
 * Description           : library primitives private tdal header.
 *
 *****************************************************************************/

#ifndef _TDAL_DISP_MODULE_PRIV_H_
#define _TDAL_DISP_MODULE_PRIV_H_

#ifdef   __cplusplus
extern   "C"   {
#endif


/*--------------------------------------*/
/*      Public   types   definition      */
/*--------------------------------------*/

typedef struct
{
    uint32_t x;
    uint32_t y;
    uint32_t width;
    uint32_t heigth;
} tTDAL_DISPm_Window;
/*------------------------------------------*/
/*      Public   Functions   Declarations      */
/*------------------------------------------*/

bool   TDAL_DISPm_OpenVideoViewports(bool bIsSlaveMode);
bool   TDAL_DISPm_CloseVideoViewports(bool bIsSlaveMode);

tTDAL_DISP_LayerId TDAL_DISPm_GetLayerId(uint32_t layerHandle);

uint32_t TDAL_DISPm_GetCurrentInputX(tTDAL_DISP_LayerId layerId);
uint32_t TDAL_DISPm_GetCurrentInputY(tTDAL_DISP_LayerId layerId);
uint32_t TDAL_DISPm_GetCurrentInputWidth(tTDAL_DISP_LayerId layerId);
uint32_t TDAL_DISPm_GetCurrentInputHeight(tTDAL_DISP_LayerId layerId);
uint32_t TDAL_DISPm_GetCurrentOutputWidth(tTDAL_DISP_LayerId layerId);
uint32_t TDAL_DISPm_GetCurrentOutputHeight(tTDAL_DISP_LayerId layerId);
uint32_t TDAL_DISPm_GetCurrentDisplayWidth(tTDAL_DISP_LayerId layerId);
uint32_t TDAL_DISPm_GetCurrentDisplayHeight(tTDAL_DISP_LayerId layerId);
uint32_t TDAL_DISPm_GetCurrentOutputWindowX(tTDAL_DISP_LayerId layerId);
uint32_t TDAL_DISPm_GetCurrentOutputWindowY(tTDAL_DISP_LayerId layerId);
uint32_t TDAL_DISPm_GetCurrentOutputWindowWidth(tTDAL_DISP_LayerId layerId);
uint32_t TDAL_DISPm_GetCurrentOutputWindowHeight(tTDAL_DISP_LayerId layerId);
bool TDAL_DISPm_GetOutputWindow(tTDAL_DISP_BlenderId Id, tTDAL_DISP_BlenderWindow *pstOutputWindow);
//
//bool   TDAL_DISPm_OpenGFXViewports(tTDAL_DISP_LayerId       layerId,
//                   STGXOBJ_Rectangle_t      inputRectangle,
//                   STGXOBJ_Rectangle_t      outputRectangle,
//                   STLAYER_ViewPortHandle_t *pVPHandle,
//                   STGXOBJ_Bitmap_t      **pBitmapDestination);
//bool   TDAL_DISPm_CloseGFXViewports(STLAYER_ViewPortHandle_t VPHandle);
//bool    TDAL_DISPm_EraseGFXViewports(tTDAL_DISP_LayerId layerId, STLAYER_ViewPortHandle_t VPHandle);
//void    TDAL_DISPm_WaitVSync(tTDAL_DISP_LayerId layerId);
//uint8_t  TDAL_DISPm_ConvertAlpha(tTDAL_DISP_LayerId layerId, uint8_t alpha);
//uint32_t TDAL_DISPm_GetVTGId(tTDAL_DISP_LayerId layerId);
//void    TDAL_DISPm_RestorePreviousScaling(void);
//bool    TDAL_DISPm_VidBlenderResize(tTDAL_DISP_BlenderId   Id);
//
//bool    TDAL_DISP_STVOUT_Term(uint8_t vout);
//bool    TDAL_DISP_STVOUT_Close(uint8_t vout);
//
//bool    TDAL_DISP_STVMIX_Init(uint8_t mixer);
//bool    TDAL_DISP_STVMIX_Term(uint8_t mixer);
//bool    TDAL_DISPm_IsGFXLayerEnable(tTDAL_DISP_LayerId Id);
//void    TDAL_DISPm_ReadyToEnableVideo(bool State);
//
//bool    TDAL_DISPm_STTTX_Init(uint8_t mixer, uint8_t vtg);

tTDAL_DISP_Error TDAL_DISPm_ReconfigureAndUnmuteVideoLayer(bool useMultimedia, tTDAL_DISPm_Window * sourceWindow, tTDAL_DISPm_Window * destWindow, bool resetVideoLayer);

#ifdef   __cplusplus
}
#endif

#endif   /*_TDAL_DMX_MODULE_PRIV_H_*/
