/*****************************************************************************
 *                      COPYRIGHT 2006 IWEDIA TECHNOLOGIES              *
 *****************************************************************************
 *
 * MODULE NAME: TDAL_DISP
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/sources/chal/core/tdal/branches/idecode_hd/os21/tdal_disp/tdal_disp_layer.c $
 *          $Rev: 504 $
 *          $Date: 2011-10-12 09:09:33 +0200 (Wed, 12 Oct 2011) $
 *
 * Description : Display Layer TDAL library implementation.
 *
 ****************************************************************************/

/****************************************************************************
 *  INCLUDES                                                         *
 ****************************************************************************/
/* Generic Headers */
#include <stdio.h>
#include "string.h" /* needed for memcpy() */

#include "crules.h"
#include "tbox.h"

#include "tdal_common.h"
#include "tdal_disp.h"
#include "tdal_output.h"
#include "tdal_disp_priv.h"
#include "tdal_disp_module_priv.h"
#include "tdal_output_module_priv.h"
#include "tdal_mp_module_priv.h"
#include "mdrv_types.h"

#include "MsCommon.h"
#include "MsMemory.h"

#include "apiVDEC.h"
#include "tdal_av_module_priv.h"

#include "drvXC_IOPort.h"
#include "drvTVEncoder.h"
//Api
#include "apiGOP.h"
#include "apiXC.h"

#include "drvXC_IOPort.h"
#include "drvMVOP.h"
#include "apiVDEC.h"
#include "apiXC.h"
#include "drvTVEncoder.h"
#include "apiDAC.h"
#include "apiVDEC_EX.h"
//MApi
#include "xc/Panel.h"
#include "xc/msAPI_XC.h"
#include "xc/msAPI_VE.h"
#include "xc/msAPI_GEGOP.h"

//#define kTDAL_DISP_ACTIVE_WIDTH 624
//#define kTDAL_DISP_ACTIVE_HEIGHT 518
#define kTDAL_DISP_ACTIVE_WIDTH 720
#define kTDAL_DISP_ACTIVE_HEIGHT 576
#define kTDAL_DISP_ACTIVE_X_OFFSET ((720 - kTDAL_DISP_ACTIVE_WIDTH) / 2)
#define kTDAL_DISP_ACTIVE_Y_OFFSET ((576 - kTDAL_DISP_ACTIVE_HEIGHT) / 2)

#define MAX_WINDOW_WIDTH 720//2000
#define MAX_WINDOW_HEIGHT 576//1100

/****************************************************************************
 *  MACROS                                                            *
 ****************************************************************************/
mTBOX_SET_MODULE(eTDAL_DISP);
/****************************************************************************
 *  GLOBAL VARIABLES (GLOBAL/IMPORT)                                    *
 ****************************************************************************/
typedef struct
{
    MS_U32 u32BmpDramRblkStart;
    MS_U16 bmpWidth;
    MS_U16 bmpHeight;
}DrawBmpInfo;

/****************************************************************************
 *  LOCAL MODULE VARIABLES (MODULE)                                     *
 ****************************************************************************/
LOCAL TDAL_mutex_id TDAL_DISPi_LayerMutex = NULL;
/****************************************************************************
 *  LOCAL FILE VARIABLES (LOCAL)                                       *
 ****************************************************************************/

tTDAL_DISPi_Display              TDAL_DISP_MainDisplay;
void TDAL_DISPm_VE_CalculateAspectRatio(E_MSAPI_VE_ASPECT_RATIO_TYPE enVideoScreen,
                                       MS_VE_WINDOW_TYPE *pstCapWin,
                                       MS_VE_WINDOW_TYPE *pstDstWin);
LOCAL void TDAL_DISPi_GFXLayer(tTDAL_DISP_LayerId id);
GLOBAL void TDAL_GFXm_LayerDisable(tTDAL_DISP_LayerId Id);
GLOBAL void TDAL_GFXm_LayerEnable(tTDAL_DISP_LayerId Id);

bool TDAL_DISPm_VideoLayerEnabled(void);
/****************************************************************************
 *  TYPEDEFS                                                         *
 ****************************************************************************/
#if defined(AUXILIARY_VIDEO_DISPLAY_SD)
   #define LAYER_VIDEO_NB_DEVICES 2
#else
   #define LAYER_VIDEO_NB_DEVICES 1
#endif

enum
{
    LAYER_VIDEO1,
    LAYER_VIDEO2
};

enum
{
    LAYER_OSD1 = 0,
    LAYER_OSD2 = 1,
    LAYER_OSD3 = 2
};

void TDAL_DISP_LayerInit(void)
{
    mTBOX_FCT_ENTER("TDAL_DISP_LayerInit");
	mTBOX_TRACE(( kTBOX_NIV_WARNING, "[%s %d]enter: \n",__FUNCTION__,__LINE__));
    //E_GOP_API_Result gop_result;
    TDAL_DISPi_GFXLayer(eTDAL_DISP_LAYER_GFX_ID_0); // graphic layer for OSD
    TDAL_DISPi_GFXLayer(eTDAL_DISP_LAYER_GFX_ID_2); // graphic layer for subtitles and media viewer
    /* Z-order setting: OSD on the top
     * There are 2 layers i.e. 2 GOPs.
     * One for OSD, other for subtitles
     * and media player
     */
//    gop_result = MApi_GOP_GWIN_OutputLayerSwitch(LAYER_OSD2);
//    if (gop_result != GOP_API_SUCCESS)
//    {
//        mTBOX_TRACE((kTBOX_NIV_CRITICAL,"MApi_GOP_GWIN_OutputLayerSwitch: failure %d\n",gop_result));
//    }
    
    TDAL_CreateMutex(&TDAL_DISPi_LayerMutex);
    mTBOX_ASSERT(TDAL_DISPi_LayerMutex != NULL);
	mTBOX_TRACE(( kTBOX_NIV_WARNING, "[%s %d]success: \n",__FUNCTION__,__LINE__));

    mTBOX_RETURN;
}

void TDAL_DISP_LayerTerminate(void)
{
    TDAL_LockMutex(TDAL_DISPi_LayerMutex);
    TDAL_UnlockMutex(TDAL_DISPi_LayerMutex);
    
    TDAL_DeleteMutex(TDAL_DISPi_LayerMutex);
    TDAL_DISPi_LayerMutex = NULL;
}

/**========================================================================**
 * Function    : TDAL_DISP_LayerCapabilityGet
 *
 * Scope       :
 *
 * Description  :
 *
 * Side effects :
 *
 * Comment      :
 *
 **========================================================================**/
tTDAL_DISP_Error TDAL_DISP_LayerCapabilityGet(tTDAL_DISP_LayerId Id, tTDAL_DISP_LayerCapability *pstCapability)
{
    tTDAL_DISP_Error  error = eTDAL_DISP_NO_ERROR;
    int              Index=0;
    bool             IdFound=FALSE;

    mTBOX_FCT_ENTER("TDAL_DISP_LayerCapabilityGet");

    if (!TDAL_DISP_AlreadyInitialized)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_DISP_LayerCapabilityGet: DISP not initialise \n" ));
        mTBOX_RETURN(eTDAL_DISP_NOT_INIT_ERROR);
    }

    if( pstCapability == NULL)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_DISP_BlenderDimensionsGet: Pointer NULL :[pstCapability]\n"));
        mTBOX_RETURN(eTDAL_DISP_BAD_PARAMETER_ERROR);
    }

    /* Check the Id and fill-it */
    for(Index=0; Index<kTDAL_DISPi_TOTAL_BLENDER_NB_LAYER; Index++)
    {
        if(Id == TDAL_DISP_LayerDescriptor[Index].Id)
        {
            memcpy(pstCapability, &TDAL_DISP_LayerDescriptor[Index].stCapability, sizeof(tTDAL_DISP_LayerCapability) );
            IdFound = TRUE;
            break;
        }
    }

    if(!IdFound)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_DISP_LayerCapabilityGet: Unknown Id %d \n",  Id));
        mTBOX_RETURN(eTDAL_DISP_BAD_PARAMETER_ERROR);
    }

    mTBOX_RETURN(error);
}


/**========================================================================**
 * Function    : TDAL_DISP_LayerGlobalTransparencySet
 *
 * Scope       :
 *
 * Description  :
 *
 * Side effects :
 *
 * Comment      :
 *
 **========================================================================**/
tTDAL_DISP_Error TDAL_DISP_LayerGlobalTransparencySet(tTDAL_DISP_LayerId Id, uint8_t Transparency)
{
    tTDAL_DISP_Error       error = eTDAL_DISP_NO_ERROR;
    int                   Index=0;
    bool                  IdFound=FALSE;

    mTBOX_FCT_ENTER("TDAL_DISP_LayerGlobalTransparencySet");

    if (!TDAL_DISP_AlreadyInitialized)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_DISP_LayerGlobalTransparencySet: DISP not initialise \n" ));
        mTBOX_RETURN(eTDAL_DISP_NOT_INIT_ERROR);
    }

    /* Check the Id and fill-it */
    for(Index=0; Index<kTDAL_DISPi_TOTAL_BLENDER_NB_LAYER; Index++)
    {
        if(Id == TDAL_DISP_LayerDescriptor[Index].Id)
        {
            if(TDAL_DISP_LayerDescriptor[Index].stCapability.IsGlobalTransparencyCapable != TRUE)
            {
                mTBOX_TRACE((kTBOX_NIV_WARNING,"TDAL_DISP_LayerGlobalTransparencySet: ID[%d] not supported \n",TDAL_DISP_LayerDescriptor[Index].Id  ));
                mTBOX_RETURN(eTDAL_DISP_NOT_AVAILABLE_ERROR);
            }
            IdFound = TRUE;
            break;
        }
    }

    if(IdFound)
    {
        /*TBD*/
    }
    else
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_DISP_LayerGlobalTransparencySet: Unknown Id \n" ));
        mTBOX_RETURN(eTDAL_DISP_BAD_PARAMETER_ERROR);
    }

    mTBOX_RETURN(error);
}

/**========================================================================**
 * Function    : TDAL_DISP_LayerHandleGet
 *
 * Scope       :
 *
 * Description  :
 *
 * Side effects :
 *
 * Comment      :
 *
 **========================================================================**/
tTDAL_DISP_Error TDAL_DISP_LayerHandleGet(tTDAL_DISP_LayerId Id, void *pHandle)
{
    tTDAL_DISP_Error  error = eTDAL_DISP_NO_ERROR;
    int              Index=0;
    bool             IdFound=FALSE;
    uint32_t        layerHandle;

    mTBOX_FCT_ENTER("TDAL_DISP_LayerHandleGet");

    if (!TDAL_DISP_AlreadyInitialized)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_DISP_LayerHandleGet: DISP not initialise \n" ));
        mTBOX_RETURN(eTDAL_DISP_NOT_INIT_ERROR);
    }

    if( pHandle == NULL)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_DISP_LayerHandleGet: Pointer NULL :[pHandle]\n"));
        mTBOX_RETURN(eTDAL_DISP_BAD_PARAMETER_ERROR);
    }

    /* Check the Id and fill-it */
    for(Index=0; Index<kTDAL_DISPi_TOTAL_BLENDER_NB_LAYER; Index++)
    {
        if(Id == TDAL_DISP_LayerDescriptor[Index].Id)
        {
           switch(TDAL_DISP_LayerDescriptor[Index].Id)
           {
               case eTDAL_DISP_LAYER_VIDEO_ID_0 :
                   /*TBD*/
                   layerHandle = (uint32_t)&TDAL_DISP_LayerDescriptor[Index];
                   *(uint32_t*)pHandle = layerHandle;
                   break;
               case eTDAL_DISP_LAYER_GFX_ID_0:
               case eTDAL_DISP_LAYER_GFX_ID_2:
                   layerHandle = (uint32_t)&TDAL_DISP_LayerDescriptor[Index];
                   *(uint32_t*)pHandle = layerHandle;
                   break;
               default:
                   mTBOX_TRACE((kTBOX_NIV_WARNING,"TDAL_DISP_LayerHandleGet: Layer Identifier not supported :Id[%d]\n", TDAL_DISP_LayerDescriptor[Index].Id));
                   mTBOX_RETURN(eTDAL_DISP_NOT_AVAILABLE_ERROR);
                   break;
           }
           IdFound = TRUE;
           break;
        }
    }

    if(!IdFound)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_DISP_LayerHandleGet: Unknown Id \n" ));
        mTBOX_RETURN(eTDAL_DISP_BAD_PARAMETER_ERROR);
    }

    mTBOX_RETURN(error);
}

/**========================================================================**
 * Function    : TDAL_DISP_LayerIOWindowsAdjust
 *
 * Scope       :
 *
 * Description  :
 *
 * Side effects :
 *
 * Comment      :
 *
 **========================================================================**/
tTDAL_DISP_Error TDAL_DISP_LayerIOWindowsAdjust(tTDAL_DISP_LayerId Id, tTDAL_DISP_LayerWindow *pstInputWindow, tTDAL_DISP_LayerWindow *pstOutputWindow, bool *pbAdjusted)
{
    tTDAL_DISP_Error  error = eTDAL_DISP_NO_ERROR;
    int              Index=0;
    bool             IdFound=FALSE;

    mTBOX_FCT_ENTER("TDAL_DISP_LayerIOWindowsAdjust");

    if (!TDAL_DISP_AlreadyInitialized)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_DISP_LayerIOWindowsAdjust: DISP not initialise \n" ));
        mTBOX_RETURN(eTDAL_DISP_NOT_INIT_ERROR);
    }

    if( (pstInputWindow == NULL) || (pstOutputWindow == NULL) )
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_DISP_LayerIOWindowsAdjust: Pointer NULL :[pstInputWindow=0x%x], [pstOutputWindow=0x%x]\n", pstInputWindow, pstOutputWindow));
        mTBOX_RETURN(eTDAL_DISP_BAD_PARAMETER_ERROR);
    }

    /* Check the Id and fill-it */
    for(Index=0; Index<kTDAL_DISPi_TOTAL_BLENDER_NB_LAYER; Index++)
    {
        /*TBD*/
        if(Id == TDAL_DISP_LayerDescriptor[Index].Id)
        {
           switch(TDAL_DISP_LayerDescriptor[Index].Id)
           {
               case eTDAL_DISP_LAYER_GFX_ID_0:
               case eTDAL_DISP_LAYER_GFX_ID_1:
               case eTDAL_DISP_LAYER_GFX_ID_2:
               case eTDAL_DISP_LAYER_STILL_ID_0:
               case eTDAL_DISP_LAYER_STILL_ID_1:
               case eTDAL_DISP_LAYER_CURSOR_ID_0:
               case eTDAL_DISP_LAYER_CURSOR_ID_1:
               case eTDAL_DISP_LAYER_VIDEO_ID_2:
                   mTBOX_TRACE((kTBOX_NIV_WARNING,"TDAL_DISP_LayerIOWindowsAdjust: Layer Graphic not supported \n"));
                   mTBOX_RETURN(eTDAL_DISP_NOT_AVAILABLE_ERROR);
                   break;
               default:
                   break;
           }
           IdFound = TRUE;
           break;
        }
    }

    if(IdFound)
    {
        (*pbAdjusted) = FALSE;
    }
    else
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_DISP_LayerIOWindowsAdjust: Unknown Id \n" ));
        mTBOX_RETURN(eTDAL_DISP_BAD_PARAMETER_ERROR);
    }

    mTBOX_RETURN(error);
}

/**========================================================================**
 * Function    : TDAL_DISP_LayerIOWindowsAspectRatioParamsSet
 *
 * Scope       :
 *
 * Description  :
 *
 * Side effects :
 *
 * Comment      :
 *
 **========================================================================**/
tTDAL_DISP_Error TDAL_DISP_LayerIOWindowsAspectRatioParamsSet(tTDAL_DISP_LayerId Id, tTDAL_DISP_LayerWindow *pstInputWindow, tTDAL_DISP_AspectRatio InputAspectRatio, uint32_t InputAFD, tTDAL_DISP_AspectRatioConv AspectRatioConv)
{
    tTDAL_DISP_Error  error = eTDAL_DISP_NO_ERROR;

    mTBOX_FCT_ENTER("TDAL_DISP_LayerIOWindowsAspectRatioParamsSet");

 /*printf("TDAL_DISP_LayerIOWindowsAspectRatioParamsSet YP TODO  ?? %s %d\r\n", __FILE__, __LINE__);*/

    mTBOX_TRACE((kTBOX_NIV_WARNING,"TDAL_DISP_LayerIOWindowsAspectRatioParamsSet: function not implemented\n" ));
    error = eTDAL_DISP_NOT_AVAILABLE_ERROR;

    mTBOX_RETURN(error);
}

/**========================================================================**
 * Function    : TDAL_DISP_LayerIOWindowsGet
 *
 * Scope       :
 *
 * Description  :
 *
 * Side effects :
 *
 * Comment      :   Returns params only for MAIN layer - Not used for the moment by Comedia
 *
 **========================================================================**/
tTDAL_DISP_Error TDAL_DISP_LayerIOWindowsGet(tTDAL_DISP_LayerId Id, tTDAL_DISP_LayerWindow *pstInputWindow, tTDAL_DISP_LayerWindow *pstOutputWindow)
{
    tTDAL_DISP_Error  error = eTDAL_DISP_NO_ERROR;
    int              Index=0;
    bool             IdFound=FALSE;
    
    mTBOX_FCT_ENTER("TDAL_DISP_LayerIOWindowsGet");
    
    if (!TDAL_DISP_AlreadyInitialized)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_DISP_LayerIOWindowsGet: DISP not initialise \n" ));
        mTBOX_RETURN(eTDAL_DISP_NOT_INIT_ERROR);
    }
    
    if( (pstInputWindow == NULL) || (pstOutputWindow == NULL) )
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_DISP_LayerIOWindowsGet: Pointer NULL :[pstInputWindow=0x%x], [pstOutputWindow=0x%x]\n", pstInputWindow, pstOutputWindow));
        mTBOX_RETURN(eTDAL_DISP_BAD_PARAMETER_ERROR);
    }

    /* Check the Id and fill-it */
    for(Index=0; Index<kTDAL_DISPi_TOTAL_BLENDER_NB_LAYER; Index++)
    {
        if(Id == TDAL_DISP_LayerDescriptor[Index].Id)
        {
            switch(TDAL_DISP_LayerDescriptor[Index].Id)
            {
                case eTDAL_DISP_LAYER_STILL_ID_0:
                case eTDAL_DISP_LAYER_STILL_ID_1:
                case eTDAL_DISP_LAYER_CURSOR_ID_0:
                case eTDAL_DISP_LAYER_CURSOR_ID_1:
                case eTDAL_DISP_LAYER_VIDEO_ID_2:
                    mTBOX_TRACE((kTBOX_NIV_WARNING,"TDAL_DISP_LayerIOWindowsGet: Layer Graphic not supported \n"));
                    mTBOX_RETURN(eTDAL_DISP_NOT_AVAILABLE_ERROR);
                    break;
                default:
                    break;
            }
            IdFound = TRUE;
            break;
        }
    }

    if(IdFound)
    {
        if (Id == eTDAL_DISP_LAYER_GFX_ID_0)
        {
            pstInputWindow->Left    = TDAL_DISP_MainDisplay.inputWindow.Left;
            pstInputWindow->Top     = TDAL_DISP_MainDisplay.inputWindow.Top;
            pstInputWindow->Right   = TDAL_DISP_MainDisplay.inputWindow.Right;
            pstInputWindow->Bottom  = TDAL_DISP_MainDisplay.inputWindow.Bottom;
            
            pstOutputWindow->Left   = TDAL_DISP_MainDisplay.outputWindow.Left;
            pstOutputWindow->Top    = TDAL_DISP_MainDisplay.outputWindow.Top;
            pstOutputWindow->Right  = TDAL_DISP_MainDisplay.outputWindow.Right;
            pstOutputWindow->Bottom = TDAL_DISP_MainDisplay.outputWindow.Bottom;
        }
        else if (Id == eTDAL_DISP_LAYER_GFX_ID_2)
        {
            pstInputWindow->Left    = TDAL_DISP_MainDisplay.inputWindow2.Left;
            pstInputWindow->Top     = TDAL_DISP_MainDisplay.inputWindow2.Top;
            pstInputWindow->Right   = TDAL_DISP_MainDisplay.inputWindow2.Right;
            pstInputWindow->Bottom  = TDAL_DISP_MainDisplay.inputWindow2.Bottom;
    
            pstOutputWindow->Left   = TDAL_DISP_MainDisplay.outputWindow2.Left;
            pstOutputWindow->Top    = TDAL_DISP_MainDisplay.outputWindow2.Top;
            pstOutputWindow->Right  = TDAL_DISP_MainDisplay.outputWindow2.Right;
            pstOutputWindow->Bottom = TDAL_DISP_MainDisplay.outputWindow2.Bottom;
        }
        else
        {
            if ((Id == eTDAL_DISP_LAYER_VIDEO_ID_0) ||(Id == eTDAL_DISP_LAYER_VIDEO_ID_1))
            {
                memcpy(pstInputWindow, &TDAL_DISP_LayerDescriptor[Index].stInputWindow,  sizeof(tTDAL_DISP_LayerWindow) );
                memcpy(pstOutputWindow, &TDAL_DISP_LayerDescriptor[Index].stRequestedOutputWindow,  sizeof(tTDAL_DISP_LayerWindow) );
            }
        }
    }
    
    if(!IdFound)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_DISP_LayerIOWindowsGet: Unknown Id \n" ));
        mTBOX_RETURN(eTDAL_DISP_BAD_PARAMETER_ERROR);
    }

    mTBOX_RETURN(error);
}

/**========================================================================**
 * Function    : TDAL_DISP_LayerIOWindowsSet
 *
 * Scope       :
 *
 * Description  :
 *
 * Side effects :
 *
 * Comment      :
 *
 **========================================================================**/
tTDAL_DISP_Error TDAL_DISP_LayerIOWindowsSet(tTDAL_DISP_LayerId Id, tTDAL_DISP_LayerWindow *pstInputWindow, tTDAL_DISP_LayerWindow *pstOutputWindow)
{
    tTDAL_DISP_Error        error = eTDAL_DISP_NO_ERROR;
    int                     Index=0;
    bool            IdFound;
    
    tTDAL_DISP_BlenderWindow    stBlenderOutputWindow;
    U32                         DisplayOutputWidth = 0;
    U32                         DisplayOutputHeight = 0;
    
    mTBOX_FCT_ENTER("TDAL_DISP_LayerIOWindowsSet");

    if (!TDAL_DISP_AlreadyInitialized)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_DISP_LayerIOWindowsSet: DISP not initialise \n" ));
        mTBOX_RETURN(eTDAL_DISP_NOT_INIT_ERROR);
    }

    if( (pstInputWindow == NULL) || (pstOutputWindow == NULL) )
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_DISP_LayerIOWindowsSet: Pointer NULL :[pstInputWindow=0x%x], [pstOutputWindow=0x%x]\n", pstInputWindow, pstOutputWindow));
        mTBOX_RETURN(eTDAL_DISP_BAD_PARAMETER_ERROR);
    }

    /* Avoid any comedia call that could disturb resolution change */
    if (TDAL_GlobalResolutionChangeOnGoing == TRUE)
    {
        mTBOX_RETURN(eTDAL_DISP_NO_ERROR);
    }

    TDAL_LockMutex(TDAL_DISPi_LayerMutex);

    /* Check the Id and fill-it */
    for(Index=0; Index<kTDAL_DISPi_TOTAL_BLENDER_NB_LAYER; Index++)
    {
        if(Id == TDAL_DISP_LayerDescriptor[Index].Id)
        {
            switch(TDAL_DISP_LayerDescriptor[Index].Id)
            {
                case eTDAL_DISP_LAYER_GFX_ID_2:
                case eTDAL_DISP_LAYER_STILL_ID_0:
                case eTDAL_DISP_LAYER_STILL_ID_1:
                case eTDAL_DISP_LAYER_CURSOR_ID_0:
                case eTDAL_DISP_LAYER_CURSOR_ID_1:
                case eTDAL_DISP_LAYER_VIDEO_ID_2:
                    TDAL_UnlockMutex(TDAL_DISPi_LayerMutex);
                    mTBOX_TRACE((kTBOX_NIV_WARNING,"TDAL_DISP_LayerIOWindowsSet: Layer Graphic not supported \n"));
                    mTBOX_RETURN(eTDAL_DISP_NOT_AVAILABLE_ERROR);
                    break;
                default:
                    break;
            }
            IdFound = TRUE;
            break;
        }
    }

    if(IdFound)
    {
        if (Id == eTDAL_DISP_LAYER_GFX_ID_0)
        {
            mTBOX_TRACE((kTBOX_NIV_2,"LayerIOWindowsSet:GFX_ID_0 bitmapHdle@%x\n",
                    TDAL_DISP_MainDisplay.memBitmapHandle));

            if (TDAL_DISP_MainDisplay.memBitmapHandle != 0)
            {
                mTBOX_TRACE((kTBOX_NIV_2,"LayerIOWindowsSet:GFX_ID_0   curr(in[%d %d %d %d]out(%d %d %d %d])\n",
                        TDAL_DISP_MainDisplay.inputWindow.Left,
                        TDAL_DISP_MainDisplay.inputWindow.Top,
                        TDAL_DISP_MainDisplay.inputWindow.Right,
                        TDAL_DISP_MainDisplay.inputWindow.Bottom,
                        TDAL_DISP_MainDisplay.outputWindow.Left,
                        TDAL_DISP_MainDisplay.outputWindow.Top,
                        TDAL_DISP_MainDisplay.outputWindow.Right,
                        TDAL_DISP_MainDisplay.outputWindow.Bottom));
                mTBOX_TRACE((kTBOX_NIV_2,"LayerIOWindowsSet:GFX_ID_0    new(in[%d %d %d %d]out(%d %d %d %d])\n",
                        pstInputWindow->Left,
                        pstInputWindow->Top,
                        pstInputWindow->Right,
                        pstInputWindow->Bottom,
                        pstOutputWindow->Left,
                        pstOutputWindow->Top,
                        pstOutputWindow->Right,
                        pstOutputWindow->Bottom));

                if ((TDAL_DISP_MainDisplay.inputWindow.Left != pstInputWindow->Left) ||
                        (TDAL_DISP_MainDisplay.inputWindow.Top != pstInputWindow->Top) ||
                        (TDAL_DISP_MainDisplay.inputWindow.Right != pstInputWindow->Right) ||
                        (TDAL_DISP_MainDisplay.inputWindow.Bottom != pstInputWindow->Bottom) ||
                        (TDAL_DISP_MainDisplay.outputWindow.Left != pstOutputWindow->Left) ||
                        (TDAL_DISP_MainDisplay.outputWindow.Top != pstOutputWindow->Top) ||
                        (TDAL_DISP_MainDisplay.outputWindow.Right != pstOutputWindow->Right) ||
                        (TDAL_DISP_MainDisplay.outputWindow.Bottom != pstOutputWindow->Bottom) ||
                        /* Check also if layer exist has been resetted */
                        (TDAL_OUTPUTm_isOutputResolutionChangedForGFX_ID_0() == TRUE)
                )
                {
                    TDAL_OUTPUTm_resetOutputResolutionChangedForGFX_ID_0();
//                    TDAL_GFXm_ResolutionCloseViewPort(eTDAL_DISP_LAYER_GFX_ID_0);
//                    TDAL_DISPm_FreeGFXBuffer(&TDAL_DISP_MainDisplay, eTDAL_DISP_LAYER_GFX_ID_0);
   
                    TDAL_DISP_MainDisplay.inputWindow.Left    = pstInputWindow->Left;
                    TDAL_DISP_MainDisplay.inputWindow.Top      = pstInputWindow->Top;
                    TDAL_DISP_MainDisplay.inputWindow.Right   = pstInputWindow->Right;
                    TDAL_DISP_MainDisplay.inputWindow.Bottom = pstInputWindow->Bottom;

                    TDAL_DISP_MainDisplay.outputWindow.Left    = pstOutputWindow->Left;
                    TDAL_DISP_MainDisplay.outputWindow.Top      = pstOutputWindow->Top;
                    TDAL_DISP_MainDisplay.outputWindow.Right   = pstOutputWindow->Right;
                    TDAL_DISP_MainDisplay.outputWindow.Bottom   = pstOutputWindow->Bottom;

//                    TDAL_DISPm_AllocateGFXBuffer(&TDAL_DISP_MainDisplay, VTG1, eTDAL_DISP_LAYER_GFX_ID_0);
//                    TDAL_GFXm_ResolutionOpenViewPort(eTDAL_DISP_LAYER_GFX_ID_0); /* in order to refresh OSD*/
                }
            }

            if (TDAL_DISP_MainDisplay.memBitmapHandle == 0)
            {
                TDAL_DISP_MainDisplay.inputWindow.Left    = pstInputWindow->Left;
                TDAL_DISP_MainDisplay.inputWindow.Top      = pstInputWindow->Top;
                TDAL_DISP_MainDisplay.inputWindow.Right   = pstInputWindow->Right;
                TDAL_DISP_MainDisplay.inputWindow.Bottom = pstInputWindow->Bottom;

                TDAL_DISP_MainDisplay.outputWindow.Left    = pstOutputWindow->Left;
                TDAL_DISP_MainDisplay.outputWindow.Top      = pstOutputWindow->Top;
                TDAL_DISP_MainDisplay.outputWindow.Right   = pstOutputWindow->Right;
                TDAL_DISP_MainDisplay.outputWindow.Bottom   = pstOutputWindow->Bottom;

//                TDAL_DISPm_AllocateGFXBuffer(&TDAL_DISP_MainDisplay, VTG1, eTDAL_DISP_LAYER_GFX_ID_0);
            }

            mTBOX_TRACE((kTBOX_NIV_2,"LayerIOWindowsSet:GFX_ID_0 updated(in[%d %d %d %d]out(%d %d %d %d])\n",
                    TDAL_DISP_MainDisplay.inputWindow.Left,
                    TDAL_DISP_MainDisplay.inputWindow.Top,
                    TDAL_DISP_MainDisplay.inputWindow.Right,
                    TDAL_DISP_MainDisplay.inputWindow.Bottom,
                    TDAL_DISP_MainDisplay.outputWindow.Left,
                    TDAL_DISP_MainDisplay.outputWindow.Top,
                    TDAL_DISP_MainDisplay.outputWindow.Right,
                    TDAL_DISP_MainDisplay.outputWindow.Bottom));
        }
        else if (Id == eTDAL_DISP_LAYER_GFX_ID_2)
        {
            mTBOX_TRACE((kTBOX_NIV_2,"LayerIOWindowsSet:GFX_ID_1 bitmapHdle@%x\n",
                    TDAL_DISP_MainDisplay.memBitmapHandle));

            if (TDAL_DISP_MainDisplay.memBitmapHandle != 0)
            {
                mTBOX_TRACE((kTBOX_NIV_2,"LayerIOWindowsSet:GFX_ID_1   curr(in[%d %d %d %d]out(%d %d %d %d])\n",
                        TDAL_DISP_MainDisplay.inputWindow.Left,
                        TDAL_DISP_MainDisplay.inputWindow.Top,
                        TDAL_DISP_MainDisplay.inputWindow.Right,
                        TDAL_DISP_MainDisplay.inputWindow.Bottom,
                        TDAL_DISP_MainDisplay.outputWindow.Left,
                        TDAL_DISP_MainDisplay.outputWindow.Top,
                        TDAL_DISP_MainDisplay.outputWindow.Right,
                        TDAL_DISP_MainDisplay.outputWindow.Bottom));
                mTBOX_TRACE((kTBOX_NIV_2,"LayerIOWindowsSet:GFX_ID_1    new(in[%d %d %d %d]out(%d %d %d %d])\n",
                        pstInputWindow->Left,
                        pstInputWindow->Top,
                        pstInputWindow->Right,
                        pstInputWindow->Bottom,
                        pstOutputWindow->Left,
                        pstOutputWindow->Top,
                        pstOutputWindow->Right,
                        pstOutputWindow->Bottom));

                if ((TDAL_DISP_MainDisplay.inputWindow.Left != pstInputWindow->Left) ||
                        (TDAL_DISP_MainDisplay.inputWindow.Top != pstInputWindow->Top) ||
                        (TDAL_DISP_MainDisplay.inputWindow.Right != pstInputWindow->Right) ||
                        (TDAL_DISP_MainDisplay.inputWindow.Bottom != pstInputWindow->Bottom) ||
                        (TDAL_DISP_MainDisplay.outputWindow.Left != pstOutputWindow->Left) ||
                        (TDAL_DISP_MainDisplay.outputWindow.Top != pstOutputWindow->Top) ||
                        (TDAL_DISP_MainDisplay.outputWindow.Right != pstOutputWindow->Right) ||
                        (TDAL_DISP_MainDisplay.outputWindow.Bottom != pstOutputWindow->Bottom) ||
                        /* Check also if layer exist has been resetted */
                        (TDAL_OUTPUTm_isOutputResolutionChangedForGFX_ID_1() == TRUE))
                {
                    TDAL_OUTPUTm_resetOutputResolutionChangedForGFX_ID_1();
//                    TDAL_GFXm_ResolutionCloseViewPort(eTDAL_DISP_LAYER_GFX_ID_1);
//                    TDAL_DISPm_FreeGFXBuffer(&TDAL_DISP_MainDisplay, eTDAL_DISP_LAYER_GFX_ID_1);

                    TDAL_DISP_MainDisplay.inputWindow.Left   = pstInputWindow->Left;
                    TDAL_DISP_MainDisplay.inputWindow.Top    = pstInputWindow->Top;
                    TDAL_DISP_MainDisplay.inputWindow.Right   = pstInputWindow->Right;
                    TDAL_DISP_MainDisplay.inputWindow.Bottom = pstInputWindow->Bottom;

                    TDAL_DISP_MainDisplay.outputWindow.Left   = pstOutputWindow->Left;
                    TDAL_DISP_MainDisplay.outputWindow.Top    = pstOutputWindow->Top;
                    TDAL_DISP_MainDisplay.outputWindow.Right   = pstOutputWindow->Right;
                    TDAL_DISP_MainDisplay.outputWindow.Bottom = pstOutputWindow->Bottom;

//                    TDAL_DISPm_AllocateGFXBuffer(&TDAL_DISP_MainDisplay, VTG2, eTDAL_DISP_LAYER_GFX_ID_1);
//                    TDAL_GFXm_ResolutionOpenViewPort(eTDAL_DISP_LAYER_GFX_ID_1); /* in order to refresh OSD*/
                }
            }

            if (TDAL_DISP_MainDisplay.memBitmapHandle == 0)
            {
                TDAL_DISP_MainDisplay.inputWindow.Left   = pstInputWindow->Left;
                TDAL_DISP_MainDisplay.inputWindow.Top    = pstInputWindow->Top;
                TDAL_DISP_MainDisplay.inputWindow.Right   = pstInputWindow->Right;
                TDAL_DISP_MainDisplay.inputWindow.Bottom = pstInputWindow->Bottom;

                TDAL_DISP_MainDisplay.outputWindow.Left   = pstOutputWindow->Left;
                TDAL_DISP_MainDisplay.outputWindow.Top    = pstOutputWindow->Top;
                TDAL_DISP_MainDisplay.outputWindow.Right   = pstOutputWindow->Right;
                TDAL_DISP_MainDisplay.outputWindow.Bottom = pstOutputWindow->Bottom;

//                TDAL_DISPm_AllocateGFXBuffer(&TDAL_DISP_MainDisplay, VTG2, eTDAL_DISP_LAYER_GFX_ID_1);
            }
            mTBOX_TRACE((kTBOX_NIV_2,"LayerIOWindowsSet:GFX_ID_1 updated(in[%d %d %d %d]out(%d %d %d %d])\n",
                    TDAL_DISP_MainDisplay.inputWindow.Left,
                    TDAL_DISP_MainDisplay.inputWindow.Top,
                    TDAL_DISP_MainDisplay.inputWindow.Right,
                    TDAL_DISP_MainDisplay.inputWindow.Bottom,
                    TDAL_DISP_MainDisplay.outputWindow.Left,
                    TDAL_DISP_MainDisplay.outputWindow.Top,
                    TDAL_DISP_MainDisplay.outputWindow.Right,
                    TDAL_DISP_MainDisplay.outputWindow.Bottom));
        }
        else if(Id == eTDAL_DISP_LAYER_VIDEO_ID_0)
        {
            memcpy(&TDAL_DISP_LayerDescriptor[Index].stInputWindow, pstInputWindow, sizeof(tTDAL_DISP_LayerWindow));
            memcpy(&TDAL_DISP_LayerDescriptor[Index].stRequestedOutputWindow, pstOutputWindow, sizeof(tTDAL_DISP_LayerWindow));

            if(TDAL_DISPm_VideoLayerEnabled())
            {
                tTDAL_DISPm_Window srcWindow, dstWindow;

                srcWindow.x = pstInputWindow->Left;
            	srcWindow.y = pstInputWindow->Top;
            	srcWindow.width = pstInputWindow->Right - pstInputWindow->Left + 1;
            	srcWindow.heigth = pstInputWindow->Bottom - pstInputWindow->Top + 1;

            	dstWindow.x = pstOutputWindow->Left;
            	dstWindow.y = pstOutputWindow->Top;
            	dstWindow.width = pstOutputWindow->Right - pstOutputWindow->Left + 1;
            	dstWindow.heigth = pstOutputWindow->Bottom - pstOutputWindow->Top + 1;

                   if (TDAL_AVm_IsDecoderRunning()) TDAL_DISPm_ReconfigureAndUnmuteVideoLayer(false, &srcWindow, &dstWindow, FALSE);
            }
        }
        else
        {
        	mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Unsupported layer %d\n", Id));
        }
    }
    else
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_DISP_LayerIOWindowsSet: Unknown Id \n" ));
        error = eTDAL_DISP_BAD_PARAMETER_ERROR;
    }

    TDAL_UnlockMutex(TDAL_DISPi_LayerMutex);

    mTBOX_RETURN(error);
}

/**========================================================================**
 * Function    : TDAL_DISP_LayerDisable
 *
 * Scope       :
 *
 * Description  :
 *
 * Side effects :
 *
 * Comment      :
 *
 **========================================================================**/
tTDAL_DISP_Error TDAL_DISP_LayerDisable(tTDAL_DISP_LayerId Id)
{
    tTDAL_DISP_Error       error = eTDAL_DISP_NO_ERROR;
    int                   Index=0;
    bool                  IdFound=FALSE;

    mTBOX_FCT_ENTER("TDAL_DISP_LayerDisable");

    if (!TDAL_DISP_AlreadyInitialized)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_DISP_LayerDisable: DISP not initialise \n" ));
        mTBOX_RETURN(eTDAL_DISP_NOT_INIT_ERROR);
    }

    TDAL_LockMutex(TDAL_DISPi_LayerMutex);

    /* Check the Id and fill-it */
    for(Index=0; Index<kTDAL_DISPi_TOTAL_BLENDER_NB_LAYER; Index++)
    {
        if(Id == TDAL_DISP_LayerDescriptor[Index].Id)
        {
           IdFound = TRUE;
           break;
        }
    }

    if(IdFound)
    {
		/*check if enable is true*/
		if(TDAL_DISP_LayerDescriptor[Index].IsEnabled)
		{
			switch(Id)
			{
				case eTDAL_DISP_LAYER_GFX_ID_0:
				case eTDAL_DISP_LAYER_GFX_ID_2:
					TDAL_GFXm_LayerDisable(Id/*TDAL_DISP_LayerDescriptor[Index].GFXLayerDesc*/);
					break;
				case eTDAL_DISP_LAYER_VIDEO_ID_0:
					/*TBD*/ 
					//MApi_XC_EnableWindow(FALSE, MAIN_WINDOW);

					//msAPI_VE_SetVideoMute(TRUE);
					msAPI_XC_SetVideoMute(TRUE, MAIN_WINDOW);
					/* TBD: so these are the functions from lower layer, 
					* MApi_XC_EnableWindow(MS_BOOL bEnable, SCALER_WIN eWindow),
					* MApi_XC_GenerateBlackVideo(MS_BOOL bEnable, SCALER_WIN eWindow)
					* should we use those, then
					*/
					//MApi_XC_EnableWindow(FALSE, MAIN_WINDOW);
					break;
				default:
					mTBOX_TRACE((kTBOX_NIV_WARNING,"TDAL_DISP_LayerDisable: Layer ID[%d] not supported \n", Id));
					TDAL_UnlockMutex(TDAL_DISPi_LayerMutex);
					mTBOX_RETURN(eTDAL_DISP_NOT_AVAILABLE_ERROR);
					break;
			}

			TDAL_DISP_LayerDescriptor[Index].IsEnabled = FALSE;
		} /* if enable*/
    }
    else
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_DISP_LayerDisable: Unknown Id \n" ));
        error = eTDAL_DISP_BAD_PARAMETER_ERROR;
    }

    TDAL_UnlockMutex(TDAL_DISPi_LayerMutex);
    mTBOX_RETURN(error);
}

bool TDAL_DISPm_VideoLayerEnabled(void)
{
    int Index;
    bool IdFound = FALSE;
    bool enabled = false;

    mTBOX_FCT_ENTER("TDAL_DISPm_VideoLayerEnabled");

	/* Check the Id and fill-it */
    for(Index=0; Index<kTDAL_DISPi_TOTAL_BLENDER_NB_LAYER; Index++)
    {
        if(eTDAL_DISP_LAYER_VIDEO_ID_0 == TDAL_DISP_LayerDescriptor[Index].Id)
        {
           IdFound = TRUE;
           break;
        }
    }

    mTBOX_ASSERTm(IdFound == TRUE, "eTDAL_DISP_LAYER_VIDEO_ID_0 must exist");
    if (IdFound)
    {
    	enabled = TDAL_DISP_LayerDescriptor[Index].IsEnabled;
    }

    mTBOX_TRACE((kTBOX_NIV_1, "[TDAL_DISPm_VideoLayerEnabled] layer enabled = %s\n", enabled ? "TRUE" : "FALSE"));

    mTBOX_RETURN(enabled);
}


LOCAL bool TDAL_DISPi_CheckWindow(tTDAL_DISPm_Window * window)
{
    if ((window->x > MAX_WINDOW_WIDTH) ||
        (window->width > MAX_WINDOW_WIDTH) ||
        ((window->x + window->width) > MAX_WINDOW_WIDTH) ||
        (window->y > MAX_WINDOW_HEIGHT) ||
        (window->heigth > MAX_WINDOW_HEIGHT) ||
        ((window->y + window->heigth) > MAX_WINDOW_HEIGHT)
    )
    {
        return false;
    }

    if (window->width == 0 || window->heigth == 0)
    {
        return false;
    }

    return true;
}

LOCAL MS_WINDOW_TYPE TDAL_DISPi_GetMStarWindow(tTDAL_DISPm_Window * window)
{
    MS_WINDOW_TYPE r;

    r.x = window->x;
    r.y = window->y;
    r.height = window->heigth;
    r.width = window->width;

    return r;
}

tTDAL_DISP_Error TDAL_DISPm_ReconfigureAndUnmuteVideoLayer(bool useMultimedia, tTDAL_DISPm_Window * sourceWindow, tTDAL_DISPm_Window * destWindow, bool resetVideoLayer)
{
    int Index;
    bool IdFound = FALSE;

    mTBOX_FCT_ENTER("TDAL_DISPm_ReconfigureAndUnmuteVideoLayer");

	/* Check the Id and fill-it */
    for(Index=0; Index<kTDAL_DISPi_TOTAL_BLENDER_NB_LAYER; Index++)
    {
        if(eTDAL_DISP_LAYER_VIDEO_ID_0 == TDAL_DISP_LayerDescriptor[Index].Id)
        {
           IdFound = TRUE;
           break;
        }
    }

    mTBOX_ASSERTm(IdFound == TRUE, "eTDAL_DISP_LAYER_VIDEO_ID_0 must exist\n");

    if (IdFound)
    {
		MS_WINDOW_TYPE ms_SrcWin;
		MS_WINDOW_TYPE ms_DstWin;
		MS_WINDOW_TYPE * pMs_SrcWin = NULL;
		MS_WINDOW_TYPE * pMs_DstWin = NULL;

		if (sourceWindow) printf("=========sourceWindow %d %d %d %d\n", sourceWindow->x, sourceWindow->y, sourceWindow->width, sourceWindow->heigth);
		else printf("=========sourceWindow NULL\n");
		if (destWindow) printf("=========destWindow %d %d %d %d\n", destWindow->x, destWindow->y, destWindow->width, destWindow->heigth);
		else printf("=========destWindow NULL\n");
		
		if (sourceWindow != NULL)
		{
		    if (TDAL_DISPi_CheckWindow(sourceWindow))
		    {
		        ms_SrcWin = TDAL_DISPi_GetMStarWindow(sourceWindow);
		        pMs_SrcWin = &ms_SrcWin;
		    }
		}

		if (destWindow != NULL)
		{
		    if (TDAL_DISPi_CheckWindow(destWindow))
		    {
		        ms_DstWin = TDAL_DISPi_GetMStarWindow(destWindow);
		        pMs_DstWin = &ms_DstWin;
		    }
		}

        //Enable Blue Screen to avoid garbage video
		if(resetVideoLayer == TRUE && !msAPI_GEGOP_IsStillVideoEnable())
		{
		    //msAPI_VE_SetVideoMute(ENABLE);
		    msAPI_XC_SetVideoMute(ENABLE, MAIN_WINDOW);
		}

		msAPI_XC_EnableCCIRInput(0, DISABLE);

#ifdef USE_TDAL_MP
	    if (useMultimedia)
	    {
	        TDAL_MPi_GetDecoderStatus();
	    }
	    else
#endif
	    {
	        if (TDAL_AVm_GetDecoderStatus(NULL) == false)
	        {
	            mTBOX_TRACE((kTBOX_NIV_1, "Decoder not running\n"));
	        }
	    }

	    msAPI_XC_DTV_SetMode();

		if (pMs_SrcWin) printf("=========pMs_SrcWin %d %d %d %d\n", pMs_SrcWin->x, pMs_SrcWin->y, pMs_SrcWin->width, pMs_SrcWin->height);
		else printf("=========pMs_SrcWin NULL\n");
		if (pMs_DstWin) printf("=========pMs_DstWin %d %d %d %d\n", pMs_DstWin->x, pMs_DstWin->y, pMs_DstWin->width, pMs_DstWin->height);
		else printf("=========pMs_DstWin NULL\n");

		if ((pMs_SrcWin) && (pMs_SrcWin->width == MAX_WINDOW_WIDTH) && (pMs_SrcWin->height == MAX_WINDOW_HEIGHT))
		msAPI_XC_SetWin(NULL, NULL, pMs_DstWin, MAIN_WINDOW);
		else
		msAPI_XC_SetWin(pMs_SrcWin, NULL, pMs_DstWin, MAIN_WINDOW);

	    //Wait at least 4 V-syncs(VE setting time + wait 2 VSync) for SCALER to fill frame buffer
	    MApi_XC_WaitOutputVSync(2, 100, MAIN_WINDOW);

	    if (resetVideoLayer == TRUE)
	    {
		    msAPI_XC_SetVideoMute(FALSE, MAIN_WINDOW);
	    }

		mTBOX_TRACE((kTBOX_NIV_1, "TDAL_DISPm_ReconfigureAndUnmuteVideoLayer: video layer enabled\n"));

		mTBOX_RETURN(eTDAL_DISP_NO_ERROR);
    }
    else
    {
    	mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Could not find video layer descriptor\n"));
    	mTBOX_RETURN(eTDAL_DISP_DRIVER_ERROR);
    }
}

void TDAL_DISPm_ReconfigureVideoLayer(void)
{
    int Index;
    bool IdFound = FALSE;

    mTBOX_FCT_ENTER("TDAL_DISPm_ReconfigureVideoLayer");

    for(Index=0; Index<kTDAL_DISPi_TOTAL_BLENDER_NB_LAYER; Index++)
    {
        if(eTDAL_DISP_LAYER_VIDEO_ID_0 == TDAL_DISP_LayerDescriptor[Index].Id)
        {
           IdFound = TRUE;
           break;
        }
    }

    if (IdFound)
    {
        if (TDAL_DISP_LayerDescriptor[Index].IsEnabled)
        {
            tTDAL_DISPm_Window srcWindow, dstWindow;
            
            srcWindow.x = TDAL_DISP_LayerDescriptor[Index].stInputWindow.Left;
            srcWindow.y = TDAL_DISP_LayerDescriptor[Index].stInputWindow.Top;
            srcWindow.width = TDAL_DISP_LayerDescriptor[Index].stInputWindow.Right - TDAL_DISP_LayerDescriptor[Index].stInputWindow.Left + 1;
            srcWindow.heigth = TDAL_DISP_LayerDescriptor[Index].stInputWindow.Bottom - TDAL_DISP_LayerDescriptor[Index].stInputWindow.Top + 1;
            
            dstWindow.x = TDAL_DISP_LayerDescriptor[Index].stRequestedOutputWindow.Left;
            dstWindow.y = TDAL_DISP_LayerDescriptor[Index].stRequestedOutputWindow.Top;
            dstWindow.width = TDAL_DISP_LayerDescriptor[Index].stRequestedOutputWindow.Right - TDAL_DISP_LayerDescriptor[Index].stRequestedOutputWindow.Left + 1;
            dstWindow.heigth = TDAL_DISP_LayerDescriptor[Index].stRequestedOutputWindow.Bottom - TDAL_DISP_LayerDescriptor[Index].stRequestedOutputWindow.Top + 1;
            TDAL_DISPm_ReconfigureAndUnmuteVideoLayer(false, &srcWindow, &dstWindow, true);
        }
    }
}

/**========================================================================**
 * Function    : TDAL_DISP_LayerEnable
 *
 * Scope       :
 *
 * Description  :
 *
 * Side effects :
 *
 * Comment      :
 *
 **========================================================================**/
tTDAL_DISP_Error TDAL_DISP_LayerEnable(tTDAL_DISP_LayerId Id)
{
    tTDAL_DISP_Error  error = eTDAL_DISP_NO_ERROR;
    int                   Index=0;
    bool                  IdFound=FALSE;

    mTBOX_FCT_ENTER("TDAL_DISP_LayerEnable");

    mTBOX_TRACE((kTBOX_NIV_1,"TDAL_DISP_LayerEnable: Layer[0x%x]\n",Id ));


    if (!TDAL_DISP_AlreadyInitialized)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_DISP_LayerEnable: DISP not initialise \n" ));
        mTBOX_RETURN(eTDAL_DISP_NOT_INIT_ERROR);
    }

    TDAL_LockMutex(TDAL_DISPi_LayerMutex);

    /* Check the Id and fill-it */
    for(Index=0; Index<kTDAL_DISPi_TOTAL_BLENDER_NB_LAYER; Index++)
    {
        if(Id == TDAL_DISP_LayerDescriptor[Index].Id)
        {
           IdFound = TRUE;
           break;
        }
    }

    if(IdFound)
    {
        /*check if enable is true*/
        if(!TDAL_DISP_LayerDescriptor[Index].IsEnabled)
        {
           switch(Id)
           {
               case eTDAL_DISP_LAYER_GFX_ID_0:
               case eTDAL_DISP_LAYER_GFX_ID_2:
                   TDAL_GFXm_LayerEnable(Id/*TDAL_DISP_LayerDescriptor[Index].GFXLayerDesc*/);
                   break;
               case eTDAL_DISP_LAYER_VIDEO_ID_0:
               {

                   tTDAL_DISPm_Window srcWindow, dstWindow;

                   srcWindow.x = TDAL_DISP_LayerDescriptor[Index].stInputWindow.Left;
                   srcWindow.y = TDAL_DISP_LayerDescriptor[Index].stInputWindow.Top;
                   srcWindow.width = TDAL_DISP_LayerDescriptor[Index].stInputWindow.Right - TDAL_DISP_LayerDescriptor[Index].stInputWindow.Left + 1;
                   srcWindow.heigth = TDAL_DISP_LayerDescriptor[Index].stInputWindow.Bottom - TDAL_DISP_LayerDescriptor[Index].stInputWindow.Top + 1;

                   dstWindow.x = TDAL_DISP_LayerDescriptor[Index].stRequestedOutputWindow.Left;
                   dstWindow.y = TDAL_DISP_LayerDescriptor[Index].stRequestedOutputWindow.Top;
                   dstWindow.width = TDAL_DISP_LayerDescriptor[Index].stRequestedOutputWindow.Right - TDAL_DISP_LayerDescriptor[Index].stRequestedOutputWindow.Left + 1;
                   dstWindow.heigth = TDAL_DISP_LayerDescriptor[Index].stRequestedOutputWindow.Bottom - TDAL_DISP_LayerDescriptor[Index].stRequestedOutputWindow.Top + 1;

                   if (TDAL_AVm_IsDecoderRunning())
                   {
                       error = TDAL_DISPm_ReconfigureAndUnmuteVideoLayer(false, &srcWindow, &dstWindow, TRUE);
                   }
               }
               break;
               default:
                   mTBOX_TRACE((kTBOX_NIV_WARNING,"TDAL_DISP_LayerEnable: Layer ID[%d] not supported \n", Id));
                   TDAL_UnlockMutex(TDAL_DISPi_LayerMutex);
                   mTBOX_RETURN(eTDAL_DISP_NOT_AVAILABLE_ERROR);
                   break;
           }

           TDAL_DISP_LayerDescriptor[Index].IsEnabled = TRUE;
        } /* if enable*/
    }
    else
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_DISP_LayerEnable: Unknown Id %d\n", Id ));
        error = eTDAL_DISP_BAD_PARAMETER_ERROR;
    }

    TDAL_UnlockMutex(TDAL_DISPi_LayerMutex);

    mTBOX_RETURN(error);
}
/**========================================================================**
 * Function    : TDAL_DISPm_OpenVideoViewports
 *
 * Scope       : private to module TDAL_AV
 *
 * Description  :
 *
 * Side effects :
 *
 * Comment      :
 *
 **========================================================================**/
bool   TDAL_DISPm_OpenVideoViewports( bool bIsSlaveMode )
{
    tTDAL_DISP_Error  error = eTDAL_DISP_NO_ERROR;
    return (error);

}

/**========================================================================**
 * Function    : TDAL_DISPm_CloseVideoViewports
 *
 * Scope       : private to module TDAL_AV
 *
 * Description  :
 *
 * Side effects :
 *
 * Comment      :
 *
 **========================================================================**/
bool   TDAL_DISPm_CloseVideoViewports(bool bIsSlaveMode)
{
    tTDAL_DISP_Error  error = eTDAL_DISP_NO_ERROR;
    return (error);

}

uint32_t TDAL_DISPm_GetCurrentDisplayWidth(tTDAL_DISP_LayerId layerId)
{
    return 0;
}

uint32_t TDAL_DISPm_GetCurrentDisplayHeight(tTDAL_DISP_LayerId layerId)
{
    return 0;
}

uint32_t TDAL_DISPm_GetCurrentInputX(tTDAL_DISP_LayerId layerId)
{
    return 0;
}

uint32_t TDAL_DISPm_GetCurrentInputY(tTDAL_DISP_LayerId layerId)
{
    return 0;

}

uint32_t TDAL_DISPm_GetCurrentInputWidth(tTDAL_DISP_LayerId layerId)
{
    return 0;
}

uint32_t TDAL_DISPm_GetCurrentInputHeight(tTDAL_DISP_LayerId layerId)
{
    return 0;

}

uint32_t TDAL_DISPm_GetCurrentOutputWindowX(tTDAL_DISP_LayerId layerId)
{
    return 0;
}

uint32_t TDAL_DISPm_GetCurrentOutputWindowY(tTDAL_DISP_LayerId layerId)
{
    return 0;
}

uint32_t TDAL_DISPm_GetCurrentOutputWindowWidth(tTDAL_DISP_LayerId layerId)
{
    return 0;
}

uint32_t TDAL_DISPm_GetCurrentOutputWindowHeight(tTDAL_DISP_LayerId layerId)
{
    return 0;
}

uint32_t TDAL_DISPm_GetCurrentOutputWidth(tTDAL_DISP_LayerId layerId)
{
    return 0;
}

uint32_t TDAL_DISPm_GetCurrentOutputHeight(tTDAL_DISP_LayerId layerId)
{
    return 0;

}

uint32_t TDAL_DISPm_GetVTGId(tTDAL_DISP_LayerId layerId)
{
    return 0;
}

uint8_t TDAL_DISPm_ConvertAlpha(tTDAL_DISP_LayerId layerId, uint8_t alpha)
{
    if(alpha == 255)
    {
        return 128;
    }
    else
    {
        return (alpha / 2);
    }
}


void TDAL_DISPm_WaitVSync(tTDAL_DISP_LayerId layerId)
{
}

bool TDAL_DISPm_IsGFXLayerEnable(uint32_t layerHandle/*tTDAL_DISP_LayerId Id*/)
{
	return ((tTDAL_DISPi_LayerDescriptorElements*)layerHandle)->IsEnabled;
}


void TDAL_DISPm_RestorePreviousScaling(void)
{
}

bool TDAL_DISPm_VidBlenderResize(tTDAL_DISP_BlenderId   Id)
{
    return TRUE;
}

void TDAL_DISPm_ReadyToEnableVideo(bool State)
{
}

LOCAL void TDAL_DISPi_GFXLayer(tTDAL_DISP_LayerId id)
{
    E_GOP_API_Result        gop_result = GOP_API_SUCCESS;
    GOP_MIXER_TIMINGTYPE    gopMixerTimingType;
    GOP_MixerTiming         gopMixerTiming;
    VE_DrvStatus            DrvStatus;
    MS_VE_DST_DispInfo      DrvVEDispInfo;
    MS_U16                  GOPSC_width;
    MS_BOOL                 VE_PAL;
    int                     Index;
    bool                    IdFound = FALSE;
    
    for(Index=0; Index<kTDAL_DISPi_TOTAL_BLENDER_NB_LAYER; Index++)
    {
        if(id == TDAL_DISP_LayerDescriptor[Index].Id)
        {
            switch(TDAL_DISP_LayerDescriptor[Index].Id)
            {
                case eTDAL_DISP_LAYER_STILL_ID_0:
                case eTDAL_DISP_LAYER_STILL_ID_1:
                case eTDAL_DISP_LAYER_CURSOR_ID_0:
                case eTDAL_DISP_LAYER_CURSOR_ID_1:
                case eTDAL_DISP_LAYER_VIDEO_ID_2:
                case eTDAL_DISP_LAYER_VIDEO_ID_0:
                    mTBOX_TRACE((kTBOX_NIV_WARNING,"TDAL_DISP_LayerIOWindowsGet: Layer Graphic not supported \n"));
                    return;
                    break;
                default:
                    break;
            }
            IdFound = TRUE;
            break;
        }
    }
    
    if (IdFound == TRUE)
    {
        MS_U8 gopId;
        MS_U8 winId;
        if (eTDAL_DISP_LAYER_GFX_ID_0 == id)
        {
            /* GOP1(graphical output processor) have 4 GWINs(graphical window) maximum
             * with 4-7 IDs. 
             */  
            gopId = LAYER_OSD1;

        }
        else if (eTDAL_DISP_LAYER_GFX_ID_2 == id)
        {
            /* GOP0(graphical output processor) have 4 GWINs(graphical window) maximum
             * with 0-3 IDs. 
             */            
            gopId = LAYER_OSD2;

        }
        else
            mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_DISPi_GFXLayer: failure, wrong layer ID %d\n", id));
        
        TDAL_DISP_LayerDescriptor[Index].GFXLayerDesc = gopId;
        gop_result = MApi_GOP_GWIN_SwitchGOP(gopId);
        if (gop_result != GOP_API_SUCCESS)
        {
            printf("MApi_GOP_GWIN_SwitchGOP: failure %d\n", gop_result);
        }

        gop_result = MApi_GOP_GWIN_Set_STRETCHWIN(gopId, E_GOP_DST_OP0, kTDAL_DISP_ACTIVE_X_OFFSET, kTDAL_DISP_ACTIVE_Y_OFFSET, 720, 576);
        if (gop_result != GOP_API_SUCCESS)
        {
            printf("MApi_GOP_GWIN_Set_STRETCHWIN: failure %d\n", gop_result);
        }
        printf("#####\n\n\nMApi_GOP_SetGOPHStart HStart=%d\n\n\n", g_IPanel.HStart());
        
        gop_result = MApi_GOP_SetGOPHStart(gopId,g_IPanel.HStart());
        if (gop_result != GOP_API_SUCCESS)
        {
            printf("MApi_GOP_SetGOPHStart: failure %d\n", gop_result);
        }
        gop_result = MApi_GOP_GWIN_EnableProgressive(TRUE); //FIXME
        if (gop_result != GOP_API_SUCCESS)
        {
            printf("MApi_GOP_GWIN_EnableProgressive: failure %d\n", gop_result);
        }
        
        //winId = MApi_GOP_GWIN_CreateWin(720, 576, 3855);
        //TDAL_DISP_LayerDescriptor[Index].gwinID = winId;
    } 
}

void TDAL_DISPm_VE_CalculateAspectRatio(E_MSAPI_VE_ASPECT_RATIO_TYPE enVideoScreen,
                                       MS_VE_WINDOW_TYPE *pstCapWin,
                                       MS_VE_WINDOW_TYPE *pstDstWin)
{
    MS_U16 u16Temp;
    VE_DrvStatus veStatus;

    MDrv_VE_GetStatus(&veStatus);

    switch( enVideoScreen )
    {
        case E_MSAPI_VE_ASPECT_RATIO_16_9:
        {
            mTBOX_TRACE((kTBOX_NIV_5,"Aspect Ratio Type: E_MSAPI_VE_ASPECT_RATIO_16_9\n"));
            u16Temp = (U32)pstDstWin->height * 16 / 9;
            if (u16Temp <= pstDstWin->width) // H:V >= 16:9
            {
                pstDstWin->x += (pstDstWin->width - u16Temp) / 2;
                pstDstWin->width = u16Temp;
            }
            else // H:V <= 16:9
            {
                u16Temp = (U32)pstDstWin->width * 9 / 16;
                pstDstWin->y += (pstDstWin->height - u16Temp) / 2;
                pstDstWin->height = u16Temp;
            }
        }
        break;
        case E_MSAPI_VE_ASPECT_RATIO_4_3:
        {
            mTBOX_TRACE((kTBOX_NIV_5,"Aspect Ratio Type: E_MSAPI_VE_ASPECT_RATIO_4_3\n"));
            u16Temp = (U32)pstDstWin->height * 4 / 3;
            if (u16Temp <= pstDstWin->width) // H:V >= 4:3
            {
                pstDstWin->x += (pstDstWin->width - u16Temp) / 2;
                pstDstWin->width = u16Temp;
            }
            else // H:V <= 4:3
            {
                u16Temp = (U32)pstDstWin->width * 3 / 4;
                pstDstWin->y += (pstDstWin->height - u16Temp) / 2;
                pstDstWin->height = u16Temp;
            }
        }
        break;

        case E_MSAPI_VE_ASPECT_RATIO_FULL:
        default:
        {
            mTBOX_TRACE((kTBOX_NIV_5,"Aspect Ratio Type: E_MSAPI_VE_ASPECT_RATIO_FULL\n"));
            pstDstWin->x = 0;
            pstDstWin->y = 0;
            if(veStatus.VideoSystem <= MS_VE_NTSC_J)
            {
                pstDstWin->width= 720;
                pstDstWin->height = 480;
            }
            else
            {
                pstDstWin->width= 720;
                pstDstWin->height = 576;
            }
        }
        break;

        case E_MSAPI_VE_ASPECT_RATIO_NORMAL:
            mTBOX_TRACE((kTBOX_NIV_5,"Aspect Ratio Type: E_MSAPI_VE_ASPECT_RATIO_NORMAL\n"));
            break;
        case E_MSAPI_VE_ASPECT_RATIO_CUT_4_3: // cut off content to reserve aspect ratio
            mTBOX_TRACE((kTBOX_NIV_5,"Aspect Ratio Type: E_MSAPI_VE_ASPECT_RATIO_CUT_4_3\n"));
            //capture window
            if((pstDstWin->width != 0) || (pstDstWin->height != 0))
            {
                u16Temp = (U32)pstCapWin->height * pstDstWin->width / pstDstWin->height;
                if (u16Temp <= pstCapWin->width) // H:V >= 4:3
                {
                    pstCapWin->x += (pstCapWin->width - u16Temp) / 2;
                    pstCapWin->width = u16Temp;
                }
                else // H:V <= 4:3
                {
                    u16Temp = (U32)pstCapWin->width * pstDstWin->height / pstDstWin->width;
                    pstCapWin->y += (pstCapWin->height - u16Temp) / 2;
                    pstCapWin->height = u16Temp;
                }
            }
            break;
        case E_MSAPI_VE_ASPECT_RATIO_CUT_16_9:
            mTBOX_TRACE((kTBOX_NIV_5,"Aspect Ratio Type: E_MSAPI_VE_ASPECT_RATIO_CUT_16_9\n"));
            //capture window
            {
                u16Temp = (U32)pstCapWin->height * 16 / 9;
                if (u16Temp <= pstCapWin->width) // H:V >= 16:9
                {
                    pstCapWin->x += (pstCapWin->width - u16Temp) / 2;
                    pstCapWin->width = u16Temp;
                }
                else // H:V <= 16:9
                {
                    u16Temp = (U32)pstCapWin->width * 9 / 16;
                    pstCapWin->y += (pstCapWin->height - u16Temp) / 2;
                    pstCapWin->height = u16Temp;
                }
            }
            //display window
            {
                u16Temp = (U32)pstDstWin->height * 16 / 9;
                if (u16Temp <= pstDstWin->width) // H:V >= 16:9
                {
                    pstDstWin->x += (pstDstWin->width - u16Temp) / 2;
                    pstDstWin->width = u16Temp;
                }
                else // H:V <= 16:9
                {
                    u16Temp = (U32)pstDstWin->width * 9 / 16;
                    pstDstWin->y += (pstDstWin->height - u16Temp) / 2;
                    pstDstWin->height = u16Temp;
                }
            }
            break;

    }

}

uint8_t TDAL_DISPm_LayerGOPDesc(uint32_t layerHandle)
{
    return ((tTDAL_DISPi_LayerDescriptorElements*)layerHandle)->GFXLayerDesc;
}

uint8_t TDAL_DISPm_LayerGWINDesc(uint32_t layerHandle)
{
    return ((tTDAL_DISPi_LayerDescriptorElements*)layerHandle)->gwinID;
}

uint8_t TDAL_DISPm_LayerId(uint32_t layerHandle)
{
    return ((tTDAL_DISPi_LayerDescriptorElements*)layerHandle)->Id;
}

