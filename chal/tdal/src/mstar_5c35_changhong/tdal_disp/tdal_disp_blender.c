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
 * Description : Display Blender TDAL library implementation
 *
 ****************************************************************************/

/****************************************************************************
   *   INCLUDES                                *
   ****************************************************************************/
/*   Generic   Headers   */
#include <stdio.h>
#include <string.h>


#include "crules.h"
#include "tbox.h"
#include "tdal_common.h"
#include "tdal_disp.h"
#include "tdal_disp_priv.h"

#include "MsCommon.h"
#include "MsMemory.h"
//Api
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
/****************************************************************************
   *   DEFINES                                    *
   ****************************************************************************/

/****************************************************************************
   *   MACROS                                    *
   ****************************************************************************/
mTBOX_SET_MODULE(eTDAL_DISP);
/****************************************************************************
   *   TYPEDEFS                                *
   ****************************************************************************/

/****************************************************************************
   *   GLOBAL   VARIABLES   (GLOBAL/IMPORT)                     *
   ****************************************************************************/
IMPORT MS_S32 gs32CachedPoolID;
/****************************************************************************
   *   LOCAL   MODULE   VARIABLES   (MODULE)                         *
   ****************************************************************************/
bool TDAL_DISPm_GetOutputWindow(tTDAL_DISP_BlenderId   Id, tTDAL_DISP_BlenderWindow   *pstOutputWindow);
void TDAL_DISPm_XCInit();
GLOBAL void TDAL_OUTPUTm_VEInit();
/****************************************************************************
   *   LOCAL   FILE   VARIABLES   (LOCAL)                         *
   ****************************************************************************/
LOCAL bool isXCInitialized = FALSE;

__INLINE uint8_t TDAL_DISP_YCrCb2R(uint8_t bY, uint8_t bCr, uint8_t bCb)
{
    int r;
    r = (298 * ((int)bY - 16) + 409 * ((int)bCr - 128)) / 256;

    if (r < 0)
        r = 0;
    else if (r > 255)
        r = 255;

    return (uint8_t)r;
}

__INLINE uint8_t TDAL_DISP_YCrCb2G(uint8_t bY, uint8_t bCr, uint8_t bCb)
{
    int g;
    g = (298 * ((int)bY - 16) - 212 * ((int)bCr - 128) - 100 * ((int)bCb - 128)) / 256;

    if (g < 0)
        g = 0;
    else if (g > 255)
        g = 255;

    return (uint8_t)g;
}

__INLINE uint8_t TDAL_DISP_YCrCb2B(uint8_t bY, uint8_t bCr, uint8_t bCb)
{
    int b;
    b = (298 * ((int)bY - 16) + 517 * ((int)bCb - 128)) / 256;

    if (b < 0)
        b = 0;
    else if (b > 255)
        b = 255;

    return (uint8_t)b;
}

/**========================================================================**
 * Function    : TDAL_DISP_BlenderCapabilityGet
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
tTDAL_DISP_Error   TDAL_DISP_BlenderCapabilityGet(tTDAL_DISP_BlenderId   Id, tTDAL_DISP_BlenderCapability   *pstCapability)
{
    tTDAL_DISP_Error   error = eTDAL_DISP_NO_ERROR;
    int        Index=0;
    bool        IdFound=FALSE;

    mTBOX_FCT_ENTER("TDAL_DISP_BlenderCapabilityGet");

    if   (!TDAL_DISP_AlreadyInitialized)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_DISP_BlenderCapabilityGet: DISP   not   initialise   \n"   ));
        mTBOX_RETURN(eTDAL_DISP_NOT_INIT_ERROR);
    }

    if(   pstCapability ==   NULL)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_DISP_BlenderDimensionsGet: Pointer   NULL   :[pstCapability]\n"));
        mTBOX_RETURN(eTDAL_DISP_BAD_PARAMETER_ERROR);
    }

    /*   Check   the   Id   and   fill-it   */
    for(Index=0; Index < kTDAL_DISPi_NB_BLENDER; Index++)
    {
        if(Id == TDAL_DISP_BlenderDescriptor[Index].Id)
        {
            memcpy(pstCapability, &TDAL_DISP_BlenderDescriptor[Index].stCapability, sizeof(tTDAL_DISP_BlenderCapability)   );
            IdFound = TRUE;
            break;
        }
    }

    if(!IdFound)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_DISP_BlenderCapabilityGet: Unknown   Id   \n"   ));
        mTBOX_RETURN(eTDAL_DISP_BAD_PARAMETER_ERROR);
    }

    mTBOX_RETURN(error);
}

void   TDAL_DISP_BlenderInit(void)
{
    mTBOX_TRACE(( kTBOX_NIV_WARNING, "[%s %d]enter: \n",__FUNCTION__,__LINE__));
    // XC Init
    TDAL_DISPm_XCInit();
    
    //VE Init
    TDAL_OUTPUTm_VEInit();
    mTBOX_TRACE(( kTBOX_NIV_WARNING, "[%s %d]success: \n",__FUNCTION__,__LINE__));
}

void   TDAL_DISP_BlenderTerminate(void)
{
    
}

/**========================================================================**
 * Function    : TDAL_DISP_BlenderBackgroundColorSet
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
tTDAL_DISP_Error   TDAL_DISP_BlenderBackgroundColorSet(tTDAL_DISP_BlenderId   Id, tTDAL_DISP_BlenderBackgroundColor   *pstColor)
{
   tTDAL_DISP_Error      error = eTDAL_DISP_NO_ERROR;
   return(error);
}

/**========================================================================**
 * Function    : TDAL_DISP_BlenderDimensionsGet
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
tTDAL_DISP_Error   TDAL_DISP_BlenderDimensionsGet(tTDAL_DISP_BlenderId   Id, uint32_t   *pWidth, uint32_t   *pHeight)
{
    tTDAL_DISP_Error      error = eTDAL_DISP_NO_ERROR;
    int              Index=0;
    bool          IdFound=FALSE;

    mTBOX_FCT_ENTER("TDAL_DISP_BlenderDimensionsGet");

    if   (!TDAL_DISP_AlreadyInitialized)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_DISP_BlenderDimensionsGet: DISP   not   initialise   \n"   ));
        mTBOX_RETURN(eTDAL_DISP_NOT_INIT_ERROR);
    }

    if(pWidth==NULL || pHeight==NULL)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_DISP_BlenderDimensionsGet: Pointer   NULL   :[pWidth=0x%x], [pHeight=0x%x]\n", pWidth, pHeight   ));
        mTBOX_RETURN(eTDAL_DISP_BAD_PARAMETER_ERROR);
    }

//    /* Ugly workaround to manage HD Layers RE-Size to match a SD output */
//    if(TDAL_OUTPUTi_isOutputDISABLING_GFX_ID_1 == TRUE)
//    {
//        if (Id == eTDAL_DISP_BLENDER_ID_1)
//        {
//            Id = eTDAL_DISP_BLENDER_ID_0;
//        }
//    }

    /*   Check   the   Id   and   fill   the   size   */
    for(Index=0; Index<kTDAL_DISPi_NB_BLENDER; Index++)
    {
        if(Id == TDAL_DISP_BlenderDescriptor[Index].Id)
        {
            (*pWidth) = TDAL_DISP_BlenderDescriptor[Index].Width;
            (*pHeight) = TDAL_DISP_BlenderDescriptor[Index].Height;
            mTBOX_TRACE((kTBOX_NIV_1,"TDAL_DISP_BlenderDimensionsGet: [Width=%d], [Height=%d]\n", (*pWidth), (*pHeight)   ));

            IdFound = TRUE;
            break;
        }
    }

    if(!IdFound)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_DISP_BlenderDimensionsGet: Unknown   Id   \n"   ));
        mTBOX_RETURN(eTDAL_DISP_BAD_PARAMETER_ERROR);
    }

    mTBOX_RETURN(error);
}

/**========================================================================**
 * Function    : TDAL_DISP_BlenderDimensionsSet
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
tTDAL_DISP_Error   TDAL_DISP_BlenderDimensionsSet(tTDAL_DISP_BlenderId   Id, uint32_t   Width, uint32_t   Height)
{
    tTDAL_DISP_Error      error = eTDAL_DISP_NO_ERROR;
    int              Index=0;
    bool          IdFound=FALSE;

    mTBOX_FCT_ENTER("TDAL_DISP_BlenderDimensionsSet");

    if   (!TDAL_DISP_AlreadyInitialized)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_DISP_BlenderDimensionsGet: DISP   not   initialise   \n"   ));
        mTBOX_RETURN(eTDAL_DISP_NOT_INIT_ERROR);
    }

//    /* Ugly workaround to manage HD Layers RE-Size to match a SD output */
//    if(TDAL_OUTPUTi_isOutputDISABLING_GFX_ID_1 == TRUE)
//    {
//        if (Id == eTDAL_DISP_BLENDER_ID_1)
//        {
//            Id = eTDAL_DISP_BLENDER_ID_0;
//        }
//        else
//        {
//            return eTDAL_DISP_NO_ERROR;
//        }
//    }

    /*   Check   the   Id   and   fill   the   size   */
    for(Index=0; Index<kTDAL_DISPi_NB_BLENDER; Index++)
    {
        if(Id == TDAL_DISP_BlenderDescriptor[Index].Id)
        {
            /*check   the   size*/
            if(   (Width   >   TDAL_DISP_BlenderDescriptor[Index].stCapability.widthMax)   ||   (Height   >   TDAL_DISP_BlenderDescriptor[Index].stCapability.heightMax)   )
            {
                mTBOX_TRACE((kTBOX_NIV_WARNING,"TDAL_DISP_BlenderDimensionsSet: With[%d]   or   Height[%d]   to   high   \n", Width, Height));
                mTBOX_TRACE((kTBOX_NIV_WARNING,"TDAL_DISP_BlenderDimensionsSet: WithMax[%d]   and   HeightMax[%d]   \n", TDAL_DISP_BlenderDescriptor[Index].stCapability.widthMax, TDAL_DISP_BlenderDescriptor[Index].stCapability.heightMax   ));

                mTBOX_RETURN(eTDAL_DISP_BAD_PARAMETER_ERROR);
            }
            else
            {
                TDAL_DISP_BlenderDescriptor[Index].Width = Width;
                TDAL_DISP_BlenderDescriptor[Index].Height = Height;

                mTBOX_TRACE((kTBOX_NIV_1,"TDAL_DISP_BlenderDimensionsSet: With[%d]   ; Height[%d]   \n", Width, Height));
            }
            IdFound = TRUE;
            break;
        }
    }

    if(!IdFound)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_DISP_BlenderDimensionsGet: Unknown   Id   \n"   ));
        mTBOX_RETURN(eTDAL_DISP_BAD_PARAMETER_ERROR);
    }

    mTBOX_RETURN(error);
}
tTDAL_DISP_BlenderWindow   stTDAL_DISP_RequestedBlenderOutputWindow = {0, 0, 0, 0};
/**************/
tTDAL_DISP_Error
TDAL_DISP_BlenderDimensionsOutputSet(
      tTDAL_DISP_BlenderId          Id,
      tTDAL_DISP_BlenderWindow      *pstOutputWindow)
{
    tTDAL_DISP_Error    error = eTDAL_DISP_NO_ERROR;
    int                 Index=0;

    mTBOX_FCT_ENTER("TDAL_DISP_BlenderDimensionsOutputSet");

    if   (!TDAL_DISP_AlreadyInitialized)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_DISP_BlenderDimensionsOutputSet: DISP   not   initialise   \n"   ));
        mTBOX_RETURN(eTDAL_DISP_NOT_INIT_ERROR);
    }

    if(Id == eTDAL_DISP_BLENDER_ID_0)
    {
        memcpy(&stTDAL_DISP_RequestedBlenderOutputWindow, pstOutputWindow, sizeof(tTDAL_DISP_BlenderWindow));
    }


    /*   Check   the   Id   and   fill   the   size   */
    for(Index=0; Index<kTDAL_DISPi_NB_BLENDER; Index++)
    {
        if(Id == TDAL_DISP_BlenderDescriptor[Index].Id)
        {
            TDAL_DISP_BlenderDescriptor[Index].stBlenderOutputWindow.sLeft = pstOutputWindow->sLeft;
            TDAL_DISP_BlenderDescriptor[Index].stBlenderOutputWindow.sTop = pstOutputWindow->sTop;
            TDAL_DISP_BlenderDescriptor[Index].stBlenderOutputWindow.uiRight = pstOutputWindow->uiRight;
            TDAL_DISP_BlenderDescriptor[Index].stBlenderOutputWindow.uiBottom = pstOutputWindow->uiBottom;

//            TDAL_GFXm_BlenderResize(Id);
//            TDAL_DISPm_VidBlenderResize(Id);
            break;
        }
    }

    if(Index   >   kTDAL_DISPi_NB_BLENDER)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_DISP_BlenderDimensionsGet: Unknown   Id   \n"   ));
        mTBOX_RETURN(eTDAL_DISP_BAD_PARAMETER_ERROR);
    }

    mTBOX_RETURN(error);
}

tTDAL_DISP_Error
TDAL_DISPm_ForceBlenderMainDimensionOutputSet(void)
{
    return eTDAL_DISP_NO_ERROR; 
}

tTDAL_DISP_Error
TDAL_DISPm_RestoreBlenderMainDimensionOutputSet(void)
{
    return eTDAL_DISP_NO_ERROR; 
}


/**========================================================================**
 * Function    : TDAL_DISP_BlenderDisable
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
tTDAL_DISP_Error   TDAL_DISP_BlenderDisable(tTDAL_DISP_BlenderId   Id)
{
    tTDAL_DISP_Error   error = eTDAL_DISP_NO_ERROR;
    int        Index=0;
    bool        IdFound=FALSE;

    mTBOX_FCT_ENTER("TDAL_DISP_BlenderDisable");

    if   (!TDAL_DISP_AlreadyInitialized)
    {
       mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_DISP_BlenderDisable: DISP   not   initialise   \n"   ));
       mTBOX_RETURN(eTDAL_DISP_NOT_INIT_ERROR);
    }

    /*   Check   the   Id   */
    for(Index=0; Index<kTDAL_DISPi_NB_BLENDER; Index++)
    {
       if(Id == TDAL_DISP_BlenderDescriptor[Index].Id)
       {
         TDAL_DISP_BlenderDescriptor[Index].IsEnabled = FALSE;
         IdFound = TRUE;
         break;
       }
    }

    if(!IdFound)
    {
       mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_DISP_BlenderDisable: Unknown   Id[%d]   \n", Id   ));
       mTBOX_RETURN(eTDAL_DISP_BAD_PARAMETER_ERROR);
    }

    mTBOX_RETURN(error);
}

/**========================================================================**
 * Function    : TDAL_DISP_BlenderEnable
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
tTDAL_DISP_Error   TDAL_DISP_BlenderEnable(tTDAL_DISP_BlenderId   Id)
{
    tTDAL_DISP_Error   error = eTDAL_DISP_NO_ERROR;
    int        Index=0;
    bool        IdFound=FALSE;

    mTBOX_FCT_ENTER("TDAL_DISP_BlenderEnable");

    if   (!TDAL_DISP_AlreadyInitialized)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_DISP_BlenderEnable: DISP   not   initialise   \n"   ));
        mTBOX_RETURN(eTDAL_DISP_NOT_INIT_ERROR);
    }

    /*   Check   the   Id   */
    for(Index=0; Index<kTDAL_DISPi_NB_BLENDER; Index++)
    {
        if(Id == TDAL_DISP_BlenderDescriptor[Index].Id)
        {
            TDAL_DISP_BlenderDescriptor[Index].IsEnabled = TRUE;
            IdFound = TRUE;
            break;
        }
    }

    if(!IdFound)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_DISP_BlenderEnable: Unknown   Id[%d]   \n", Id   ));
        mTBOX_RETURN(eTDAL_DISP_BAD_PARAMETER_ERROR);
    }

    mTBOX_RETURN(error);
}

/**========================================================================**
 * Function    : TDAL_DISP_BlenderZOrderPositionsGet
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
tTDAL_DISP_Error   TDAL_DISP_BlenderZOrderPositionsGet(tTDAL_DISP_BlenderId   Id, tTDAL_DISP_BlenderZOrderPositions   *pstZOrderList)
{
    tTDAL_DISP_Error   error = eTDAL_DISP_NO_ERROR;
    int        Index=0;
    bool        IdFound=FALSE;

    mTBOX_FCT_ENTER("TDAL_DISP_BlenderZOrderPositionsGet");

    if (!TDAL_DISP_AlreadyInitialized)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_DISP_BlenderZOrderPositionsGet: DISP   not   initialise   \n"   ));
        mTBOX_RETURN(eTDAL_DISP_NOT_INIT_ERROR);
    }

    if (pstZOrderList == NULL)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_DISP_BlenderZOrderPositionsGet: Pointer   NULL   :[pstZOrderList]\n"));
        mTBOX_RETURN(eTDAL_DISP_BAD_PARAMETER_ERROR);
    }

    /*   Check   the   Id   */
    for(Index=0; Index<kTDAL_DISPi_NB_BLENDER; Index++)
    {
        if(Id == TDAL_DISP_BlenderDescriptor[Index].Id)
        {
            if(TDAL_DISP_BlenderDescriptor[Index].pstZOrderPositionList != NULL)
            {
                memcpy(pstZOrderList,TDAL_DISP_BlenderDescriptor[Index].pstZOrderPositionList,TDAL_DISP_BlenderDescriptor[Index].stCapability.nbLayer * sizeof(tTDAL_DISP_BlenderZOrderPositions));
            }
            else
            {
                mTBOX_TRACE((kTBOX_NIV_WARNING,"TDAL_DISP_BlenderZOrderPositionsGet: Current   pstZOrderPositionList   NULL   \n"));
            }
            IdFound = TRUE;
            break;
        }
    }

    if(!IdFound)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_DISP_BlenderEnable: Unknown   Id[%d]   \n", Id   ));
        mTBOX_RETURN(eTDAL_DISP_BAD_PARAMETER_ERROR);
    }

    mTBOX_RETURN(error);
}

/**========================================================================**
 * Function    : TDAL_DISP_BlenderZOrderListSet
 *
 * Scope       :
 *
 * Description  :
 *
 * Side effects :
 *
 * Comment      : NOT IMPLEMENTED
 *
 **========================================================================**/
tTDAL_DISP_Error   TDAL_DISP_BlenderZOrderListSet(tTDAL_DISP_BlenderId   Id, tTDAL_DISP_LayerId   *pstZOrderList)
{
    mTBOX_FCT_ENTER("TDAL_DISP_BlenderZOrderListSet");

    if   (!TDAL_DISP_AlreadyInitialized)
    {
       mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_DISP_BlenderZOrderListSet: DISP   not   initialise   \n"   ));
       mTBOX_RETURN(eTDAL_DISP_NOT_INIT_ERROR);
    }

    if(pstZOrderList == NULL)
    {
       mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_DISP_BlenderZOrderListSet: Pointer   NULL   :[pstZOrderList]\n"));
       mTBOX_RETURN(eTDAL_DISP_BAD_PARAMETER_ERROR);
    }

    /*   not   implemented   */
    mTBOX_RETURN(eTDAL_DISP_NO_ERROR);
}

bool TDAL_DISPm_GetOutputWindow(tTDAL_DISP_BlenderId   Id, tTDAL_DISP_BlenderWindow   *pstOutputWindow)
{
   tTDAL_DISP_Error   error = eTDAL_DISP_NO_ERROR;
   int        Index=0;
   bool        IdFound=FALSE;

   mTBOX_FCT_ENTER("TDAL_DISPm_GetOutputWindow");

   if   (!TDAL_DISP_AlreadyInitialized)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_DISP_BlenderEnable: DISP   not   initialise   \n"   ));
      mTBOX_RETURN(eTDAL_DISP_NOT_INIT_ERROR);
   }

   if   (pstOutputWindow != NULL)
   {
      /*   Check   the   Id   */
      for(Index=0; Index<kTDAL_DISPi_NB_BLENDER; Index++)
      {
        if(Id == TDAL_DISP_BlenderDescriptor[Index].Id)
        {
        memcpy(pstOutputWindow, &(TDAL_DISP_BlenderDescriptor[Index].stBlenderOutputWindow), sizeof(tTDAL_DISP_BlenderWindow));
        IdFound = TRUE;
        break;
        }
      }
   
      if(!IdFound)
      {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_DISP_BlenderEnable: Unknown   Id[%d]   \n", Id   ));
        mTBOX_RETURN(eTDAL_DISP_BAD_PARAMETER_ERROR);
      }
   }
   else
   {
      mTBOX_RETURN(eTDAL_DISP_BAD_PARAMETER_ERROR);
   }

   mTBOX_RETURN(error);
}

static int  _DISP_DAC_Init(MS_BOOL HalfOutput)
{
    //Set SD DAC Cfg
    MApi_DAC_SetOutputSource(E_DAC_TO_VE, FALSE);
    MApi_DAC_SetOutputSwapSel((E_DAC_SWAP_SEL)E_DAC_R_G_B, FALSE);
    MApi_DAC_SetOutputLevel((E_DAC_MAX_OUTPUT_LEVEL)E_DAC_OUTPUT_LEVEL_HIGH, FALSE);
    MApi_DAC_OnOffSD((E_DAC_SD_ENABLE_CTRL)E_DAC_SD_XYC);
    MApi_DAC_SetIHalfOutput( HalfOutput , FALSE);
    return 0;
}
void TDAL_DISPm_XCInit()
{
    if (isXCInitialized)
        return;
    
    MSAPI_XC_INITDATA msAPI_XC_InitData;

    /* Parameter Initialization */
    memset(&msAPI_XC_InitData, 0, sizeof(MSAPI_XC_INITDATA));

    msAPI_XC_InitData.u32XCMemAddress       = SC0_MAIN_FB_ADR;
    msAPI_XC_InitData.u32XCMemSize          = SC0_MAIN_FB_LEN;

    msAPI_XC_InitData.eTimingType           = E_MSAPI_XC_RES_720x576I_50Hz;
    
    msAPI_XC_InitData.s32CachedPoolID       = gs32CachedPoolID;

    msAPI_XC_InitData.bBootLogoEnable       = FALSE;

    /* For XC Menu Load, this function to switch XC mode after V-Sync */
    msAPI_XC_InitData.bMenuLoadEnable       = TRUE;
    msAPI_XC_InitData.u32XCMenuLoadAddress  = SC0_MENULOAD_BUF_ADR;
    msAPI_XC_InitData.u32XCMenuLoadSize     = SC0_MENULOAD_BUF_LEN;
    msAPI_XC_InitData.bDisableDacSD         = TRUE;

    //For Bandwidth Table selection and to MIU configuration
    msAPI_XC_InitData.eBWTBLType = E_MSAPI_XC_BW_TBL_MIU_128;
    
//For HDMITx Configuration
//msAPI_XC_InitData.u16HdmitxHpdPin = HDMITX_HPD_PM_GPIO;
//msAPI_XC_InitData.eHdmitxOutputMode = E_MSAPI_XC_HDMITX_OUTPUT_HDMI;
//msAPI_XC_InitData.eHdmitxCDType = E_MSAPI_XC_HDMITX_CD_8BITS;
//memcpy(&msAPI_XC_InitData.stHdmitxBoardInfo, &stHDMITxInfo, sizeof(MSAPI_XC_HDMITX_BOARD_INFO));

    msAPI_XC_Init(msAPI_XC_InitData);

    //Set DTV path to Scaler
    msAPI_XC_SetConnect(INPUT_SOURCE_DTV);

 //init DAC 
 	_DISP_DAC_Init(TRUE);
 	printf("[gwq]:Set DAC init parameters!HalfOutput is true.\n");
}
