/*****************************************************************************
 *                      COPYRIGHT 2006 IWEDIA TECHNOLOGIES              *
 *****************************************************************************
 *
 * MODULE NAME: TDAL_DISP
 *
 * FILE NAME: URL:   $
 *          Rev:   $
 *          Date:  $
 *
 * Description : Display TDAL library implementation.
 *
 ****************************************************************************/

/****************************************************************************
 *   INCLUDES                                *
 ****************************************************************************/
#include <stdio.h>
#include <string.h>

/*   Generic   Headers   */
#include "crules.h"
#include "tbox.h"

#include "tdal_common.h"
#include "tdal_disp.h"
#include "tdal_disp_priv.h"

/****************************************************************************
   *   MACROS                                    *
   ****************************************************************************/
mTBOX_SET_MODULE(eTDAL_DISP);

/****************************************************************************
 *   DEFINES                                    *
 ****************************************************************************/
#ifdef HD_ENABLE
#define   kTDAL_DISP_BLENDER_0_WIDTH   1920
#define   kTDAL_DISP_BLENDER_0_HEIGHT  1080
#else
#define   kTDAL_DISP_BLENDER_0_WIDTH   720
#define   kTDAL_DISP_BLENDER_0_HEIGHT  576
#endif
#define   kTDAL_DISP_BLENDER_1_WIDTH   720
#define   kTDAL_DISP_BLENDER_1_HEIGHT  576

#define   kTDAL_DISP_LAYER_VIDEO_0_IDENTIFIER   0xFF000001
#define   kTDAL_DISP_LAYER_VIDEO_1_IDENTIFIER   0xFF000002
/****************************************************************************
 *   LOCAL   MODULE   VARIABLES   (MODULE/IMPORT)                   *
 ****************************************************************************/
LOCAL tTDAL_DISP_BlenderZOrderPositions   TDAL_DISP_InitZOrderListBlender0[]={
        {eTDAL_DISP_LAYER_VIDEO_ID_0, 0x1}, /*   0_00   */
        {eTDAL_DISP_LAYER_GFX_ID_0, 0x2},   /*   0_01   */
        {eTDAL_DISP_LAYER_GFX_ID_2, 0x4}  /*   0_02   */
};

LOCAL   tTDAL_DISP_LayerId   TDAL_DISP_InitZOrderCurrentListBlender0[]={
        eTDAL_DISP_LAYER_VIDEO_ID_0,
        eTDAL_DISP_LAYER_GFX_ID_0,
        eTDAL_DISP_LAYER_GFX_ID_2,
};

#ifdef NEVER /* Seems not used, should be removed later. */
LOCAL tTDAL_DISP_BlenderZOrderPositions   TDAL_DISP_InitZOrderListBlender1[]={
        {eTDAL_DISP_LAYER_VIDEO_ID_1, 0x1}, /*   0_00   */
        {eTDAL_DISP_LAYER_GFX_ID_0, 0x2},   /*   0_01   */
        {eTDAL_DISP_LAYER_GFX_ID_1, 0x4}   /*   0_01   */
};

LOCAL   tTDAL_DISP_LayerId   TDAL_DISP_InitZOrderCurrentListBlender1[]={
        eTDAL_DISP_LAYER_VIDEO_ID_1,
        eTDAL_DISP_LAYER_GFX_ID_0,
        eTDAL_DISP_LAYER_GFX_ID_1
};
#endif
/****************************************************************************
 *   LOCAL   FILE   VARIABLES   (LOCAL)                         *
 ****************************************************************************/
tTDAL_DISPi_BlenderDescriptorElements   TDAL_DISP_BlenderDescriptor[kTDAL_DISPi_NB_BLENDER];
tTDAL_DISPi_LayerDescriptorElements     TDAL_DISP_LayerDescriptor[kTDAL_DISPi_TOTAL_BLENDER_NB_LAYER];
tTDAL_DISPi_VideoLayerDescriptor        TDAL_DISP_VideoLayerDescriptor[kTDAL_DISPi_NB_LAYER_VIDEO];

bool                                    TDAL_DISP_AlreadyInitialized = FALSE;

/****************************************************************************
 *   FUNCTIONS   DEFINITIONS   (LOCAL/GLOBAL)                   *
 ****************************************************************************/
GLOBAL void TDAL_GFXm_Init();
/******************************************************************************
 * Function Name   : TDAL_DISP_APIRevisionGet
 * Description      : Get revision of the API (documentation) display driver
 * Side effects    :
 * Comment         :
 * Inputs          :
 * Outputs         :
 *****************************************************************************/
const   char   *TDAL_DISP_APIRevisionGet(void)
{
    static   const   char   API_Revision[] = "TDAL_DISP_v1.5";
    return((const   char   *)API_Revision);

}

/******************************************************************************
 * Function Name   : TDAL_DISP_PlatformRevisionGet
 * Description      : Get revision of the Platform (implementation) display driver
 * Side effects    :
 * Comment         :
 * Inputs          :
 * Outputs         :
 *****************************************************************************/
const   char   *TDAL_DISP_PlatformRevisionGet(void)
{
    static   const   char   PLTF_Revision[] = "BD_MST5017_SZ";
    return((const   char   *)PLTF_Revision);
}

/**========================================================================**
 * Function    : TDAL_DISP_Init
 *
 * Description  :
 *
 * Side effects :
 *
 * Comment      :
 *
 **========================================================================**/
tTDAL_DISP_Error   TDAL_DISP_Init(   void   )
{
    mTBOX_FCT_ENTER("TDAL_DISP_Init");

    if (TDAL_DISP_AlreadyInitialized == TRUE)
    {
        mTBOX_RETURN(eTDAL_DISP_ALREADY_INIT_ERROR);
    }
    /*   Set   Blenders   Capabilities   :   Blender   0   */
    TDAL_DISP_BlenderDescriptor[0].Id = eTDAL_DISP_BLENDER_ID_0;
    TDAL_DISP_BlenderDescriptor[0].stCapability.heightMax = kTDAL_DISP_BLENDER_0_HEIGHT;
    TDAL_DISP_BlenderDescriptor[0].stCapability.widthMax = kTDAL_DISP_BLENDER_0_WIDTH;
    TDAL_DISP_BlenderDescriptor[0].stCapability.bBackgroundColor = TRUE;
    TDAL_DISP_BlenderDescriptor[0].stCapability.backgroundColorPixelFormatMask = kTDAL_DISP_PIXEL_FORMAT_RGB888   |   kTDAL_DISP_PIXEL_FORMAT_YUV888_444;
    TDAL_DISP_BlenderDescriptor[0].stCapability.nbLayer = kTDAL_DISPi_BLENDER_ID_1_NB_LAYER;
    TDAL_DISP_BlenderDescriptor[0].Height = kTDAL_DISP_BLENDER_0_HEIGHT;
    TDAL_DISP_BlenderDescriptor[0].Width = kTDAL_DISP_BLENDER_0_WIDTH;
    
    TDAL_DISP_BlenderDescriptor[0].stBlenderOutputWindow.sLeft = 0;
    TDAL_DISP_BlenderDescriptor[0].stBlenderOutputWindow.sTop = 0;
    TDAL_DISP_BlenderDescriptor[0].stBlenderOutputWindow.uiRight = kTDAL_DISP_BLENDER_1_WIDTH   -   1;
    TDAL_DISP_BlenderDescriptor[0].stBlenderOutputWindow.uiBottom = kTDAL_DISP_BLENDER_1_HEIGHT   -   1;

    /*   Create   the   ZOrderList   for   Blender   0*/
    TDAL_DISP_BlenderDescriptor[0].pstZOrderPositionList = &TDAL_DISP_InitZOrderListBlender0[0];
    TDAL_DISP_BlenderDescriptor[0].pstZOrderCurrentList = &TDAL_DISP_InitZOrderCurrentListBlender0[0];
    
    
    TDAL_DISP_LayerDescriptor[0].Id = eTDAL_DISP_LAYER_VIDEO_ID_0;
    TDAL_DISP_LayerDescriptor[0].stCapability.NbRegionMax = 1;
    TDAL_DISP_LayerDescriptor[0].stCapability.IsMultiRegionOnSameLineCapable = FALSE;
    TDAL_DISP_LayerDescriptor[0].stCapability.IsInputWindowCapable = TRUE;
    TDAL_DISP_LayerDescriptor[0].stCapability.IsOutputWindowCapable = TRUE;
    TDAL_DISP_LayerDescriptor[0].stCapability.IsGlobalTransparencyCapable = FALSE;
    TDAL_DISP_LayerDescriptor[0].stCapability.IsAspectRatioWindowConvCapable = FALSE;
    TDAL_DISP_LayerDescriptor[0].IsEnabled = FALSE;
    TDAL_DISP_LayerDescriptor[0].bReadyToEnable = FALSE;
    TDAL_DISP_LayerDescriptor[0].pstVideoLayerDesc = &TDAL_DISP_VideoLayerDescriptor[1];
    TDAL_DISP_LayerDescriptor[0].stInputWindow.Left = 0;
    TDAL_DISP_LayerDescriptor[0].stInputWindow.Top = 0;
    TDAL_DISP_LayerDescriptor[0].stInputWindow.Right = kTDAL_DISP_BLENDER_0_WIDTH   -   1;
    TDAL_DISP_LayerDescriptor[0].stInputWindow.Bottom = kTDAL_DISP_BLENDER_0_HEIGHT   -   1;
    TDAL_DISP_LayerDescriptor[0].stRequestedOutputWindow.Left = 0;
    TDAL_DISP_LayerDescriptor[0].stRequestedOutputWindow.Top = 0;
    TDAL_DISP_LayerDescriptor[0].stRequestedOutputWindow.Right = kTDAL_DISP_BLENDER_0_WIDTH   -   1;
    TDAL_DISP_LayerDescriptor[0].stRequestedOutputWindow.Bottom = kTDAL_DISP_BLENDER_0_HEIGHT   -   1;
    TDAL_DISP_LayerDescriptor[1].Id = eTDAL_DISP_LAYER_GFX_ID_0;
    TDAL_DISP_LayerDescriptor[1].stCapability.NbRegionMax = (-1);
    TDAL_DISP_LayerDescriptor[1].stCapability.IsMultiRegionOnSameLineCapable = FALSE;
    TDAL_DISP_LayerDescriptor[1].stCapability.IsInputWindowCapable = FALSE;
    TDAL_DISP_LayerDescriptor[1].stCapability.IsOutputWindowCapable = FALSE;
    TDAL_DISP_LayerDescriptor[1].stCapability.IsGlobalTransparencyCapable = TRUE;
    TDAL_DISP_LayerDescriptor[1].stCapability.IsAspectRatioWindowConvCapable = FALSE;
    TDAL_DISP_LayerDescriptor[1].IsEnabled = FALSE;
    TDAL_DISP_LayerDescriptor[1].bReadyToEnable = FALSE;
    TDAL_DISP_LayerDescriptor[1].stInputWindow.Left = 0;
    TDAL_DISP_LayerDescriptor[1].stInputWindow.Top = 0;
    TDAL_DISP_LayerDescriptor[1].stInputWindow.Right = kTDAL_DISP_BLENDER_1_WIDTH   -   1;
    TDAL_DISP_LayerDescriptor[1].stInputWindow.Bottom = kTDAL_DISP_BLENDER_1_HEIGHT   -   1;
    TDAL_DISP_LayerDescriptor[1].stRequestedOutputWindow.Left = 0;
    TDAL_DISP_LayerDescriptor[1].stRequestedOutputWindow.Top = 0;
    TDAL_DISP_LayerDescriptor[1].stRequestedOutputWindow.Right = kTDAL_DISP_BLENDER_1_WIDTH   -   1;
    TDAL_DISP_LayerDescriptor[1].stRequestedOutputWindow.Bottom = kTDAL_DISP_BLENDER_1_HEIGHT   -   1;    

    TDAL_DISP_LayerDescriptor[2].Id = eTDAL_DISP_LAYER_GFX_ID_2;
    TDAL_DISP_LayerDescriptor[2].stCapability.NbRegionMax = (-1);
    TDAL_DISP_LayerDescriptor[2].stCapability.IsMultiRegionOnSameLineCapable = FALSE;
    TDAL_DISP_LayerDescriptor[2].stCapability.IsInputWindowCapable = FALSE;
    TDAL_DISP_LayerDescriptor[2].stCapability.IsOutputWindowCapable = FALSE;
    TDAL_DISP_LayerDescriptor[2].stCapability.IsGlobalTransparencyCapable = TRUE;
    TDAL_DISP_LayerDescriptor[2].stCapability.IsAspectRatioWindowConvCapable = FALSE;
    TDAL_DISP_LayerDescriptor[2].IsEnabled = FALSE;
    TDAL_DISP_LayerDescriptor[2].bReadyToEnable = FALSE;
    TDAL_DISP_LayerDescriptor[2].stInputWindow.Left = 0;
    TDAL_DISP_LayerDescriptor[2].stInputWindow.Top = 0;
    TDAL_DISP_LayerDescriptor[2].stInputWindow.Right = kTDAL_DISP_BLENDER_1_WIDTH   -   1;
    TDAL_DISP_LayerDescriptor[2].stInputWindow.Bottom = kTDAL_DISP_BLENDER_1_HEIGHT   -   1;
    TDAL_DISP_LayerDescriptor[2].stRequestedOutputWindow.Left = 0;
    TDAL_DISP_LayerDescriptor[2].stRequestedOutputWindow.Top = 0;
    TDAL_DISP_LayerDescriptor[2].stRequestedOutputWindow.Right = kTDAL_DISP_BLENDER_1_WIDTH   -   1;
    TDAL_DISP_LayerDescriptor[2].stRequestedOutputWindow.Bottom = kTDAL_DISP_BLENDER_1_HEIGHT   -   1;    
    
    
    TDAL_DISP_BlenderInit();
    TDAL_GFXm_Init();
    TDAL_DISP_LayerInit();

    TDAL_DISP_AlreadyInitialized = TRUE;    
    
    return eTDAL_DISP_NO_ERROR;
}

/**========================================================================**
 * Function    : TDAL_DISP_Term
 *
 * Description  :
 *
 * Side effects :
 *
 * Comment      :
 *
 **========================================================================**/
tTDAL_DISP_Error   TDAL_DISP_Term(   void   )
{
    tTDAL_DISP_Error   error = eTDAL_DISP_NO_ERROR;
    int          Index=0;

    mTBOX_FCT_ENTER("TDAL_DISP_Term");

    if   (!TDAL_DISP_AlreadyInitialized)
    {
       mTBOX_TRACE((kTBOX_NIV_WARNING,"TDAL_DISP_Term:   DISP   not   initialise   \n"   ));
       mTBOX_RETURN(eTDAL_DISP_NOT_INIT_ERROR);
    }

    /*   Dï¿½allocates   mutex   for   each   descriptor   */
    for(Index=0 ;Index<kTDAL_DISPi_NB_BLENDER; Index++)
    {
//       STOS_SemaphoreDelete(NULL,TDAL_DISP_BlenderDescriptor[Index].Lock);
      /*   set   globales   variables   to   0   */
       memset(&TDAL_DISP_BlenderDescriptor[Index], 0, sizeof(tTDAL_DISPi_BlenderDescriptorElements)   );
    }

    for(Index=0 ;Index<kTDAL_DISPi_TOTAL_BLENDER_NB_LAYER; Index++)
    {
//       STOS_SemaphoreDelete(NULL,TDAL_DISP_LayerDescriptor[Index].Lock);
       /*   set   globales   variables   to   0   */
       memset(&TDAL_DISP_LayerDescriptor[Index], 0, sizeof(tTDAL_DISPi_LayerDescriptorElements)   );
    }

    TDAL_DISP_LayerTerminate();
    TDAL_DISP_BlenderTerminate();

    TDAL_DISP_AlreadyInitialized = FALSE;

    mTBOX_RETURN(error);
}


/**========================================================================**
 * Function    : TDAL_DISP_CapabilityGet
 *
 * Description  :
 *
 * Side effects :
 *
 * Comment      :
 *
 **========================================================================**/
tTDAL_DISP_Error   TDAL_DISP_CapabilityGet(tTDAL_DISP_Capability   *pstCapability)
{
    tTDAL_DISP_Error   error = eTDAL_DISP_NO_ERROR;

    mTBOX_FCT_ENTER("TDAL_DISP_CapabilityGet");

    if   (!TDAL_DISP_AlreadyInitialized)
    {
       mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_DISP_CapabilityGet:   DISP   not   initialise   \n"   ));
       mTBOX_RETURN(eTDAL_DISP_NOT_INIT_ERROR);
    }

    if(pstCapability   ==   NULL)
    {
       mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_DISP_CapabilityGet:   Pointer   NULL   :   pstCapability   \n"   ));
       mTBOX_RETURN(eTDAL_DISP_BAD_PARAMETER_ERROR);
    }

    pstCapability->nbBlender = kTDAL_DISPi_NB_BLENDER;
    
    mTBOX_RETURN(error);
}



