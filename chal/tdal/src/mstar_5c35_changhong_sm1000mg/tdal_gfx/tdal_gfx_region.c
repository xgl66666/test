#include <string.h>
#include <math.h>

#include "crules.h"
#include "tbox.h"
#include "MsCommon.h"
#include "MsMemory.h"

#include "drvXC_IOPort.h"
#include "drvTVEncoder.h"

#include "apiGOP.h"
#include "apiGFX.h"

#include "tdal_common.h"

#include "tdal_disp.h"
#include "tdal_gfx.h"
#include "tdal_gfx_p.h"

#define GFX_DEFAULT_WIDTH (720)
#define GFX_DEFAULT_HEIGHT (576)

#define MSTAR_INVALID_LAYER_HANDLE 0x0000FFFF

#define IMAGE_LAYER_FADE
/********************************************************/
/*        Global   Variables   (GLOBAL/IMPORT)      */
/********************************************************/

mTBOX_SET_MODULE(eTDAL_GFX);


GLOBAL uint8_t TDAL_DISPm_LayerGOPDesc(uint32_t layerHandle);
GLOBAL uint8_t TDAL_DISPm_LayerGWINDesc(uint32_t layerHandle);
LOCAL tTDAL_GFX_Palette   *pTDAL_GFXi_Pallete[kTDAL_GFX_REGCOUNT] = {0};
tTDAL_RegionDesc    TDAL_GFX_RgnDesc[kTDAL_GFX_REGCOUNT];
int transRgnIdx = -1;
void TDAL_GFXm_ClearRegionFromDisplay(tTDAL_GFX_RegionHandle regionHandle, bool clear, bool fadeOut);

uint16_t minYOffset[3] = {GFX_DEFAULT_HEIGHT, GFX_DEFAULT_HEIGHT, GFX_DEFAULT_HEIGHT};
uint16_t minXOffset[3] = {GFX_DEFAULT_WIDTH, GFX_DEFAULT_WIDTH, GFX_DEFAULT_WIDTH};
/********************************************************/
/*        Local   File   Variables   (LOCAL)      */
/********************************************************/

int32_t TDAL_GFXm_FindAvailableRegionDescription()
{
	int32_t i = 0;
	for ( i = 0; i < kTDAL_GFX_REGCOUNT; i++)
	{
		if (TDAL_GFX_RgnDesc[i].used == false)
		{
			return i;
		}
	}
	return -1;
}

/*===========================================================================
 *
 * TDAL_GFX_RegionCreate
 *
 * Parameters:
 *      None
 *
 * Description:
 *
 *
 * Returns:
 *      Revision
 *
 *===========================================================================*/
tTDAL_GFX_Error   TDAL_GFX_RegionCreate(tTDAL_GFX_RegionHandle   *pRegionHandle,
                     tTDAL_GFX_RegionConfig   *pRegionConfig)
{
    tTDAL_GFX_Error error = eTDAL_GFX_NO_ERROR;
    
    mTBOX_FCT_ENTER("TDAL_GFX_RegionCreate");
    
    uint8_t         frameBufferId = -1;
    uint16_t        fbFmt;
    int32_t         regDesc;

    if (pRegionHandle==NULL || pRegionConfig==NULL)
    {
        error = eTDAL_GFX_BAD_PARAMETER;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Bad parameter(s) regionHandle=0x%x regionConfig=0x%x\n", pRegionHandle, pRegionConfig));
        mTBOX_RETURN(error);
    }

    if ((pRegionConfig->size.width > GFX_DEFAULT_WIDTH) || (pRegionConfig->size.height > GFX_DEFAULT_HEIGHT))
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL,"RegionCreate:   width or height error\n"));
        mTBOX_RETURN(eTDAL_GFX_BAD_PARAMETER);
    }

    regDesc = TDAL_GFXm_FindAvailableRegionDescription();
    if (regDesc < 0)
    {
    	error = eTDAL_GFX_NO_MEMORY;
    	mTBOX_TRACE((kTBOX_NIV_CRITICAL, "All region descriptors used\n"));
    	mTBOX_RETURN(error);
    }

    switch (pRegionConfig->colorType)
    {
        //case eTDAL_GFX_COLOR_CLUT_AYCRCB8888:
        case eTDAL_GFX_COLOR_CLUT_ARGB8888:
            fbFmt = E_MS_FMT_I8;//E_MS_FMT_ARGB8888; 
            if (pRegionConfig->size.width % 8)
            {
            	pRegionConfig->size.width += 8 - pRegionConfig->size.width % 8;
            }
            break;
        //case eTDAL_GFX_COLOR_TRUE_COLOR_AYCRCB8888:
        case eTDAL_GFX_COLOR_TRUE_COLOR_ARGB8888:
            fbFmt = E_MS_FMT_ARGB8888;
            if (pRegionConfig->size.width % 2)
            {
            	pRegionConfig->size.width += 2 - pRegionConfig->size.width % 2;
            }
            break;
        default:
            mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Unhandled color type=0x%x\n", pRegionConfig->colorType));
            error = eTDAL_GFX_FEATURE_NOT_SUPPORTED;
            mTBOX_RETURN(error);
            break;
    }
    
    error = TDAL_GFXm_CreateFB(&frameBufferId, regDesc, &pRegionConfig->size, &pRegionConfig->offset, fbFmt);
    if (error == eTDAL_GFX_NO_ERROR)
    {
        *pRegionHandle = (tTDAL_GFX_RegionHandle)&TDAL_GFX_RgnDesc[regDesc];
        TDAL_GFX_RgnDesc[regDesc].alpha = pRegionConfig->alpha;
        TDAL_GFX_RgnDesc[regDesc].deletableWin = false;
    }
    
    mTBOX_RETURN(error);
}

/*===========================================================================
 *
 * TDAL_GFX_RegionDestinationSet
 *
 * Parameters:
 *      None
 *
 * Description:
 *
 *
 * Returns:
 *      Revision
 *
 *===========================================================================*/
tTDAL_GFX_Error   TDAL_GFX_RegionDestinationSet(tTDAL_GFX_RegionHandle   regionHandle,   uint32_t   layerHandle)
{
    tTDAL_GFX_Error error = eTDAL_GFX_NO_ERROR;
    int i;
    E_GOP_API_Result gop_result;
    mTBOX_FCT_ENTER("TDAL_GFX_RegionDestinationSet");
    
    for(i = 0; i < kTDAL_GFX_REGCOUNT; i++)
    {
        if (TDAL_GFX_RgnDesc[i].used && regionHandle==(tTDAL_GFX_RegionHandle)&TDAL_GFX_RgnDesc[i])
        {
            uint8_t gopId;
            uint8_t gwinId;
            TDAL_GFX_RgnDesc[i].layerHandle = layerHandle;
            gopId                           = TDAL_DISPm_LayerGOPDesc(layerHandle);
            gwinId                          = TDAL_DISPm_LayerGWINDesc(layerHandle);
            
            gop_result                      = MApi_GOP_GWIN_SwitchGOP(gopId);
            if (gop_result != GOP_API_SUCCESS)
            {
                printf("MApi_GOP_GWIN_SwitchGOP: failure %d\n", gop_result);
            }
            /* TB checked */
            /* Created frame buffer should be attached to the GWIN on given GOP
             * Created GWIN will serve as kind of enable/disable ability of display layer 
             */

            if (gopId == LAYER_OSD1)
            {
                GOP_GwinFBAttr DstFBInfo;
                if (MApi_GOP_GWIN_IsGwinExist(gwinId) && i == 0)
                {
                    TDAL_GFX_RgnDesc[i].GeWinId = gwinId;
                    TDAL_GFX_RgnDesc[i].frameBufferId = MApi_GOP_GWIN_GetFBfromGWIN(gwinId);
                }

                if (TDAL_GFX_RgnDesc[i].GeWinId == 0xff)
                {
              #if 1
                    MS_U8 fbId = MApi_GOP_GWIN_GetFreeFBID();
                    
                    if (MAX_GWIN_FB_SUPPORT <= fbId)
                    {
                        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "MApi_GOP_GWIN_GetFreeFBID: failure\n"));
                        return eTDAL_GFX_NO_MEMORY;
                    }
                    if (GWIN_OK != MApi_GOP_GWIN_CreateFB(fbId, 0, 0, TDAL_GFX_RgnDesc[i].size.width, TDAL_GFX_RgnDesc[i].size.height, TDAL_GFX_RgnDesc[i].fbFmt))
                    {
                        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "MApi_GOP_GWIN_GetFreeFBID: failure\n"));
                        return eTDAL_GFX_NO_MEMORY;
                    }
                    TDAL_GFX_RgnDesc[i].GeWinId = MApi_GOP_GWIN_CreateWin_Assign_FB(gopId, fbId, 0, 0);//MApi_GOP_GWIN_CreateWin(TDAL_GFX_RgnDesc[i].size.width, TDAL_GFX_RgnDesc[i].size.height, 3855);
                    if(TDAL_GFX_RgnDesc[i].GeWinId == GWIN_NO_AVAILABLE)
                    {
                        MApi_GOP_GWIN_DestroyFB(fbId);
                        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "MApi_GOP_GWIN_CreateWin_Assign_FB: failure\n"));
                        return eTDAL_GFX_NO_MEMORY;
                    }
	#else
		 TDAL_GFX_RgnDesc[i].GeWinId = MApi_GOP_GWIN_CreateWin(TDAL_GFX_RgnDesc[i].size.width, TDAL_GFX_RgnDesc[i].size.height, 3855);
	#endif
                    MApi_GOP_GWIN_SetWinDispPosition(TDAL_GFX_RgnDesc[i].GeWinId, TDAL_GFX_RgnDesc[i].offSet.x, TDAL_GFX_RgnDesc[i].offSet.y);

                    TDAL_GFX_RgnDesc[i].frameBufferId = MApi_GOP_GWIN_GetFBfromGWIN(TDAL_GFX_RgnDesc[i].GeWinId);
                    TDAL_GFX_RgnDesc[i].deletableWin = true;
                    MApi_GOP_GWIN_SwapOverlapWin(gopId, TDAL_GFX_RgnDesc[i].GeWinId);
                }

                gop_result = MApi_GOP_GWIN_GetFBInfo(TDAL_GFX_RgnDesc[i].frameBufferId, &DstFBInfo);
                if (gop_result != GOP_API_SUCCESS)
                {
                    mTBOX_TRACE((kTBOX_NIV_CRITICAL, "MApi_GOP_GWIN_GetFBInfo: failure %d\n", gop_result));
                }
                
                gop_result = MApi_GFX_ClearFrameBuffer(DstFBInfo.addr, DstFBInfo.size, 0);
                if (gop_result != GOP_API_SUCCESS)
                {
                     mTBOX_TRACE((kTBOX_NIV_CRITICAL, "MApi_GFX_ClearFrameBuffer: failure %d\n", gop_result));
                }
                
                if ((gop_result = MApi_GOP_GWIN_Switch2Gwin(TDAL_GFX_RgnDesc[i].GeWinId)) != GOP_API_SUCCESS)
                {
                    mTBOX_TRACE((kTBOX_NIV_CRITICAL, "MApi_GOP_GWIN_Switch2Gwin: failure %d\n", gop_result));
                }
            }
            else if (gopId == LAYER_OSD2)
            {
                static bool layer_osd2_initialized  = FALSE;
                gop_result                          = GOP_API_SUCCESS;
                if (MApi_GOP_GWIN_IsGwinExist(gwinId))
                {
                    TDAL_GFX_RgnDesc[i].GeWinId = gwinId;
                    TDAL_GFX_RgnDesc[i].frameBufferId = MApi_GOP_GWIN_GetFBfromGWIN(gwinId);
                }

                if (layer_osd2_initialized == FALSE)
                {
                    GOP_GwinFBAttr  DstFBInfo;                  
                    MApi_GOP_GWIN_GetFBInfo(TDAL_GFX_RgnDesc[i].frameBufferId, &DstFBInfo);
                    
                    MApi_GFX_ClearFrameBuffer(DstFBInfo.addr, DstFBInfo.size, 0);
                    layer_osd2_initialized = TRUE;
                    
                    if ((gop_result=MApi_GOP_GWIN_Switch2Gwin(TDAL_GFX_RgnDesc[i].GeWinId)) != GOP_API_SUCCESS)
                    {
                        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "MApi_GOP_GWIN_Switch2Gwin: failure %d\n", gop_result));
                    }                    
                }
                gop_result = GOP_API_SUCCESS;
            }
            else
                mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_GFX_RegionDestinationSet: incorect gopId=%d\n", gopId));      
           
            if ((gop_result=MApi_GOP_GWIN_SetBlending(TDAL_GFX_RgnDesc[i].GeWinId, /*TRUE*/TDAL_GFX_RgnDesc[i].fbFmt == E_MS_FMT_ARGB8888, TDAL_GFX_RgnDesc[i].alpha)) != GOP_API_SUCCESS)
            {
                mTBOX_TRACE((kTBOX_NIV_CRITICAL, "MApi_GOP_GWIN_SetBlending: failure %d\n", gop_result));
            }            
            break;
        }
    }
    
    if (i >= kTDAL_GFX_REGCOUNT)
    {
        error = eTDAL_GFX_UNKNOWN_REGION;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "There is mess with region handle bookkeeping!\n", regionHandle));
    }
    
    mTBOX_RETURN(error);
}

/*===========================================================================
 *
 * TDAL_GFX_RegionConfigGet
 *
 * Parameters:
 *      None
 *
 * Description:
 *
 *
 * Returns:
 *      Revision
 *
 *===========================================================================*/
tTDAL_GFX_Error   TDAL_GFX_RegionConfigGet(tTDAL_GFX_RegionHandle   regionHandle,
        tTDAL_GFX_RegionConfig   *pRegionConfig)
{
    tTDAL_GFX_Error error = eTDAL_GFX_NO_ERROR;
    GOP_GwinFBAttr fbAttr;
    E_GOP_API_Result gop_result;
    int i;
    
    
    mTBOX_FCT_ENTER("TDAL_GFX_RegionConfigGet");
    for(i = 0; i < kTDAL_GFX_REGCOUNT; i++)
    {
         if (TDAL_GFX_RgnDesc[i].used && regionHandle==(tTDAL_GFX_RegionHandle)&TDAL_GFX_RgnDesc[i])
         {   
             MS_U8 fbId = TDAL_GFX_RgnDesc[i].frameBufferId;
             if (!MApi_GOP_GWIN_IsFBExist(fbId))
             {
                 mTBOX_TRACE((kTBOX_NIV_CRITICAL, "The region with handle(frame buffer Id) %d does not exist\n", regionHandle));
                 mTBOX_RETURN(error);
             }

             if ((gop_result=MApi_GOP_GWIN_GetFBInfo(fbId, &fbAttr)) != GOP_API_SUCCESS)
             {
                 error = eTDAL_GFX_DRIVER_ERROR;
                 mTBOX_TRACE((kTBOX_NIV_CRITICAL, "MApi_GOP_GWIN_GetFBInfo: error occurred %d\n", gop_result));
                 mTBOX_RETURN(error);
             }
             /* TBD */
             switch (fbAttr.fbFmt)
             {
                 case E_MS_FMT_I8:
                     pRegionConfig->colorType = eTDAL_GFX_COLOR_CLUT_ARGB8888;
                     break;
                 case E_MS_FMT_ARGB8888:
                     pRegionConfig->colorType = eTDAL_GFX_COLOR_TRUE_COLOR_ARGB8888;
                     break;
                 default:
                     mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Unknown color format %d\n", fbAttr.fbFmt));
                     break;
             }
             /* TB checked */
             pRegionConfig->offset.x     = fbAttr.s_x;
             pRegionConfig->offset.y     = fbAttr.s_y;
             pRegionConfig->size.width   = fbAttr.width;
             pRegionConfig->size.height  = fbAttr.height;
             pRegionConfig->alpha = TDAL_GFX_RgnDesc[i].alpha;
             
             /*pRegionConfig->refSize    = ?*/
             break;
         }
    }
    mTBOX_RETURN(error);
}

tTDAL_RegionDesc *TDAL_GFXm_RgnDescGet(tTDAL_GFX_RegionHandle regionHandle)
{
    tTDAL_GFX_Error error = eTDAL_GFX_NO_ERROR;
    int i;
    tTDAL_RegionDesc *rgnDesc = NULL;
    
    for(i = 0; i < kTDAL_GFX_REGCOUNT; i++)
    {
        if (TDAL_GFX_RgnDesc[i].used && regionHandle==(tTDAL_GFX_RegionHandle)&TDAL_GFX_RgnDesc[i])
        {
            rgnDesc = &TDAL_GFX_RgnDesc[i];
            break;
        }
    }
    
    if (i >= kTDAL_GFX_REGCOUNT)
    {
        error = eTDAL_GFX_UNKNOWN_REGION;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "There is mess with region handle bookkeeping!\n", regionHandle));
    }
    
    return rgnDesc;
}
/*===========================================================================
 *
 * TDAL_GFX_RegionDelete
 *
 * Parameters:
 *      None
 *
 * Description:
 *
 *
 * Returns:
 *      Revision
 *
 *===========================================================================*/
tTDAL_GFX_Error   TDAL_GFX_RegionDelete(tTDAL_GFX_RegionHandle   regionHandle)
{
    tTDAL_GFX_Error error = eTDAL_GFX_NO_ERROR;
    uint32_t        i, regDesc = -1;
    E_GOP_API_Result gop_result;    
    
    mTBOX_FCT_ENTER("TDAL_GFX_RegionDelete");

    for (i = 0; i < kTDAL_GFX_REGCOUNT; i++)
    {
    	if (TDAL_GFX_RgnDesc[i].used==true && regionHandle==(tTDAL_GFX_RegionHandle)&TDAL_GFX_RgnDesc[i] )
    	{
    		regDesc = i;
    		break;
    	}
    }

    if (regDesc == -1)
    {
        error = eTDAL_GFX_BAD_PARAMETER;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "The region with handle(frame buffer Id) %d does not exist\n", regionHandle));
    }

    if (MApi_GOP_GWIN_IsFBExist(TDAL_GFX_RgnDesc[regDesc].frameBufferId))
    {
        TDAL_GFXm_ClearRegionFromDisplay(regionHandle, true, false);
    }
    else
    {
        //error = eTDAL_GFX_BAD_PARAMETER;
        //mTBOX_ASSERT(FALSE);
        //mTBOX_TRACE((kTBOX_NIV_CRITICAL, "The region with handle(frame buffer Id) %d does not exist\n", regionHandle));
    }
    
    if (TDAL_GFX_RgnDesc[regDesc].deletableWin == true)
    {
        gop_result = MApi_GOP_GWIN_DestroyWin(TDAL_GFX_RgnDesc[regDesc].GeWinId);
        if (gop_result != GOP_API_SUCCESS)
        {
            error = eTDAL_GFX_DRIVER_ERROR;
            mTBOX_TRACE((kTBOX_NIV_CRITICAL, "MApi_GOP_GWIN_DestroyWin: failed to destroy GWIN\n"));
        }
        TDAL_GFX_RgnDesc[regDesc].GeWinId == GWIN_NO_AVAILABLE;
    }
    
    TDAL_GFX_RgnDesc[regDesc].used = false;
    pTDAL_GFXi_Pallete[i] = NULL;
    
    mTBOX_RETURN(error);
}

/*===========================================================================
 *
 * TDAL_GFX_RegionShow
 *
 * Parameters:
 *      None
 *
 * Description:
 *
 *
 * Returns:
 *      Revision
 *
 *===========================================================================*/
tTDAL_GFX_Error TDAL_GFX_RegionShow(tTDAL_GFX_RegionHandle   regionHandle)
{
    tTDAL_GFX_Error     error = eTDAL_GFX_NO_ERROR;
    int i;
    GWINID              GeWinId = 0xff;
    E_GOP_API_Result    gop_result = GOP_API_SUCCESS;
    mTBOX_FCT_ENTER("TDAL_GFX_RegionShow");
    
    for(i = 0; i < kTDAL_GFX_REGCOUNT; i++)
    {
         if (TDAL_GFX_RgnDesc[i].used && regionHandle==(tTDAL_GFX_RegionHandle)&TDAL_GFX_RgnDesc[i])
         {
             GeWinId = TDAL_GFX_RgnDesc[i].GeWinId;
             break;
         }
    }
    
    if (i >= kTDAL_GFX_REGCOUNT)
    {
        error = eTDAL_GFX_UNKNOWN_REGION;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "There is mess with region handle bookkeeping!\n", regionHandle));
        return error;
    }

    if (!TDAL_GFX_RgnDesc[i].visible)
    {
        TDAL_GFX_RgnDesc[i].visible = true;
        if (TDAL_DISPm_IsGFXLayerEnable(TDAL_GFX_RgnDesc[i].layerHandle))
        {
            gop_result = MApi_GOP_GWIN_Enable(GeWinId, TRUE);
            if (gop_result != GOP_API_SUCCESS)
            {
                error = eTDAL_GFX_DRIVER_ERROR;
                mTBOX_TRACE((kTBOX_NIV_CRITICAL, "MApi_GOP_GWIN_Enable: failure %\n", gop_result));
            }
    #ifdef IMAGE_LAYER_FADE
            {
                uint8_t gopId;
                gopId = TDAL_DISPm_LayerGOPDesc(TDAL_GFX_RgnDesc[i].layerHandle);
                if (gopId == LAYER_OSD2)
                {
                    MApi_GOP_GWIN_SetFadeInOut(GeWinId, E_GOP_GWIN_FADE_IN, TRUE, 128);
                }
            }
    #endif
        }
    }
    
    mTBOX_RETURN(error);
}

/*===========================================================================
 *
 * TDAL_GFX_RegionHide
 *
 * Parameters:
 *      None
 *
 * Description:
 *
 *
 * Returns:
 *      Revision
 *
 *===========================================================================*/
tTDAL_GFX_Error TDAL_GFX_RegionHide(tTDAL_GFX_RegionHandle   regionHandle)
{
    tTDAL_GFX_Error error = eTDAL_GFX_NO_ERROR;
    int i;
    GWINID GeWinId;
    E_GOP_API_Result gop_result = GOP_API_SUCCESS;
    mTBOX_FCT_ENTER("TDAL_GFX_RegionHide");
    
    for(i = 0; i < kTDAL_GFX_REGCOUNT; i++)
    {
         if (TDAL_GFX_RgnDesc[i].used && regionHandle == (tTDAL_GFX_RegionHandle)&TDAL_GFX_RgnDesc[i])
         {
             GeWinId = TDAL_GFX_RgnDesc[i].GeWinId;
             break;
         }
    }
    
    if (i >= kTDAL_GFX_REGCOUNT)
    {
        error = eTDAL_GFX_UNKNOWN_REGION;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "There is mess with region handle bookkeeping!\n", regionHandle));
        return error;
    }

    if (TDAL_GFX_RgnDesc[i].visible)
    {
        TDAL_GFX_RgnDesc[i].visible = false;
        if (TDAL_DISPm_IsGFXLayerEnable(TDAL_GFX_RgnDesc[i].layerHandle))
        {
        #if 1
            gop_result = MApi_GOP_GWIN_Enable(GeWinId, FALSE);
            if (gop_result != GOP_API_SUCCESS)
            {
                 error = eTDAL_GFX_DRIVER_ERROR;
                 mTBOX_TRACE((kTBOX_NIV_CRITICAL, "MApi_GOP_GWIN_Enable: failure %\n", gop_result));
            }
        #else
            if (TDAL_GFX_RgnDesc[i].layerHandle != MSTAR_INVALID_LAYER_HANDLE)
            {
                uint8_t gopId;
                gopId = TDAL_DISPm_LayerGOPDesc(TDAL_GFX_RgnDesc[i].layerHandle);
    
                if (gopId==LAYER_OSD1 || gopId==LAYER_OSD2)
                {
    #ifdef IMAGE_LAYER_FADE
                    TDAL_GFXm_ClearRegionFromDisplay(regionHandle, false, gopId==LAYER_OSD2);
    #else
                    TDAL_GFXm_ClearRegionFromDisplay(regionHandle, false, false);
    #endif
                }
                else
                    mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_GFX_RegionHide failure\n"));
            }
    
            if (gop_result != GOP_API_SUCCESS)
            {
                error = eTDAL_GFX_DRIVER_ERROR;
                mTBOX_TRACE((kTBOX_NIV_CRITICAL, "MApi_GOP_GWIN_Enable: failure %\n", gop_result));
            }
        #endif
        }
    }
    
    mTBOX_RETURN(error);
}


/*===========================================================================
 *
 * TDAL_GFX_RegionPaletteSet
 *
 * Parameters:
 *      None
 *
 * Description:
 *
 *
 * Returns:
 *      Revision
 *
 *===========================================================================*/
tTDAL_GFX_Error   TDAL_GFX_RegionPaletteSet   (tTDAL_GFX_RegionHandle   regionHandle,
        tTDAL_GFX_Palette   *pPalette)
{
    tTDAL_GFX_Error     error = eTDAL_GFX_NO_ERROR;
    GFX_Result          gfx_result;
    int                 regDesc = -1;
    int                 i;
    mTBOX_FCT_ENTER("TDAL_GFX_RegionPaletteSet");
    
    if (!pPalette)
    {
        error = eTDAL_GFX_BAD_PARAMETER;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Error: Palette is empty!\n"));
        mTBOX_RETURN(error);
    }

    if (pPalette->colorType != eTDAL_GFX_COLOR_CLUT_ARGB8888)
    {
        return eTDAL_GFX_BAD_PARAMETER;
    }

    for (i = 0; i < kTDAL_GFX_REGCOUNT; i++)
    {
        if (TDAL_GFX_RgnDesc[i].used == true && (regionHandle==(tTDAL_GFX_RegionHandle)&TDAL_GFX_RgnDesc[i]))
        {
            regDesc = i;
            break;
        }
    }
    
    pTDAL_GFXi_Pallete[regDesc] = pPalette;    
    for (i=0; i<pTDAL_GFXi_Pallete[regDesc]->nbColors; i++)
    {
        
        if (pTDAL_GFXi_Pallete[regDesc]->colorType == eTDAL_GFX_COLOR_CLUT_AYCRCB8888)
        {
            _gePalette[i].RGB.u8R = TDAL_GFX_ycrcb2R(pTDAL_GFXi_Pallete[regDesc]->pColor[i].AYCrCb8888.Y,
                                                     pTDAL_GFXi_Pallete[regDesc]->pColor[i].AYCrCb8888.Cr,
                                                     pTDAL_GFXi_Pallete[regDesc]->pColor[i].AYCrCb8888.Cb);  
            _gePalette[i].RGB.u8G = TDAL_GFX_ycrcb2G(pTDAL_GFXi_Pallete[regDesc]->pColor[i].AYCrCb8888.Y, 
                                                     pTDAL_GFXi_Pallete[regDesc]->pColor[i].AYCrCb8888.Cr,
                                                     pTDAL_GFXi_Pallete[regDesc]->pColor[i].AYCrCb8888.Cb); 
            _gePalette[i].RGB.u8B = TDAL_GFX_ycrcb2B(pTDAL_GFXi_Pallete[regDesc]->pColor[i].AYCrCb8888.Y, 
                                                     pTDAL_GFXi_Pallete[regDesc]->pColor[i].AYCrCb8888.Cr,
                                                     pTDAL_GFXi_Pallete[regDesc]->pColor[i].AYCrCb8888.Cb);
            _gePalette[i].RGB.u8A = pTDAL_GFXi_Pallete[regDesc]->pColor[i].AYCrCb8888.alpha;
        }
        else
        {
            _gePalette[i].RGB.u8R = pTDAL_GFXi_Pallete[regDesc]->pColor[i].ARGB8888.R;
            _gePalette[i].RGB.u8G = pTDAL_GFXi_Pallete[regDesc]->pColor[i].ARGB8888.G;
            _gePalette[i].RGB.u8B = pTDAL_GFXi_Pallete[regDesc]->pColor[i].ARGB8888.B;
            _gePalette[i].RGB.u8A = pTDAL_GFXi_Pallete[regDesc]->pColor[i].ARGB8888.alpha;
        }
    }
    //Set GE palette table when bitblt from I8 to ARGB
#if 1
    if (TDAL_GFX_RgnDesc[regDesc].fbFmt == E_MS_FMT_I8)
    {
        MApi_GOP_GWIN_SwitchGOP(TDAL_DISPm_LayerGOPDesc(TDAL_GFX_RgnDesc[regDesc].layerHandle));
        MApi_GOP_GWIN_SetPaletteOpt((GOP_PaletteEntry *)_gePalette, 0, GOP_PALETTE_ENTRY_NUM - 1, E_GOP_PAL_ARGB8888);
    }
#endif
    gfx_result = MApi_GFX_SetPaletteOpt(_gePalette, 0, GOP_PALETTE_ENTRY_NUM - 1);   
    if (gfx_result != GFX_SUCCESS)
    {
        error = eTDAL_GFX_DRIVER_ERROR;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "MApi_GFX_SetPaletteOpt: error occurred %d!\n", gfx_result));
        mTBOX_RETURN(error);        
    }

     mTBOX_RETURN(error);
}

#if 0
tTDAL_GFX_Error   TDAL_GFXm_BlitPaletteSet   (tTDAL_GFX_RegionHandle   regionHandle,
        tTDAL_GFX_Palette   *pPalette)
{
    tTDAL_GFX_Error     error = eTDAL_GFX_NO_ERROR;
    GFX_Result          gfx_result;
    int i;
    mTBOX_FCT_ENTER("TDAL_GFXm_RegionPaletteSet");
    
    if (!pPalette)
    {
        error = eTDAL_GFX_BAD_PARAMETER;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Error: Palette is empty!\n"));
        mTBOX_RETURN(error);
    }

    if (pPalette->colorType != eTDAL_GFX_COLOR_CLUT_ARGB8888)
    {
        return eTDAL_GFX_BAD_PARAMETER;
    }

    for (i=0; i<pPalette->nbColors; i++)
    {
        if (pPalette->colorType == eTDAL_GFX_COLOR_CLUT_AYCRCB8888)
        {
            _gePalette[i].RGB.u8R = TDAL_GFX_ycrcb2R(pPalette->pColor[i].AYCrCb8888.Y,
                                                     pPalette->pColor[i].AYCrCb8888.Cr,
                                                     pPalette->pColor[i].AYCrCb8888.Cb);  
            _gePalette[i].RGB.u8G = TDAL_GFX_ycrcb2G(pPalette->pColor[i].AYCrCb8888.Y, 
                                                     pPalette->pColor[i].AYCrCb8888.Cr,
                                                     pPalette->pColor[i].AYCrCb8888.Cb); 
            _gePalette[i].RGB.u8B = TDAL_GFX_ycrcb2B(pPalette->pColor[i].AYCrCb8888.Y, 
                                                     pPalette->pColor[i].AYCrCb8888.Cr,
                                                     pPalette->pColor[i].AYCrCb8888.Cb);
            _gePalette[i].RGB.u8A = pPalette->pColor[i].AYCrCb8888.alpha;
        }
        else
        {
            _gePalette[i].RGB.u8R = pPalette->pColor[i].ARGB8888.R;
            _gePalette[i].RGB.u8G = pPalette->pColor[i].ARGB8888.G;
            _gePalette[i].RGB.u8B = pPalette->pColor[i].ARGB8888.B;
            _gePalette[i].RGB.u8A = pPalette->pColor[i].ARGB8888.alpha;
        }
    }
    //Set GE palette table when bitblt from I8 to ARGB
    gfx_result = MApi_GFX_SetPaletteOpt(_gePalette, 0, GOP_PALETTE_ENTRY_NUM - 1);   
    if (gfx_result != GFX_SUCCESS)
    {
        error = eTDAL_GFX_DRIVER_ERROR;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "MApi_GFX_SetPaletteOpt: error occurred %d!\n", gfx_result));
        mTBOX_RETURN(error);        
    }
     mTBOX_RETURN(error);
}
#endif

/*===========================================================================
 *
 * TDAL_GFX_RegionPaletteGet
 *
 * Parameters:
 *      None
 *
 * Description:
 *
 *
 * Returns:
 *      Revision
 *
 *===========================================================================*/
tTDAL_GFX_Error   TDAL_GFX_RegionPaletteGet   (tTDAL_GFX_RegionHandle   regionHandle,
        tTDAL_GFX_Palette   *pPalette)
{
    int i;

    if (pPalette == NULL)
    {
        return eTDAL_GFX_BAD_PARAMETER;
    }

    for (i = 0; i < kTDAL_GFX_REGCOUNT; i++)
    {
        if (TDAL_GFX_RgnDesc[i].used == true && (regionHandle==(tTDAL_GFX_RegionHandle)&TDAL_GFX_RgnDesc[i]))
        {
            break;
        }
    }
    
    if (pTDAL_GFXi_Pallete[i])
    {
        pPalette->nbColors = pTDAL_GFXi_Pallete[i]->nbColors;
        pPalette->colorType = pTDAL_GFXi_Pallete[i]->colorType;
        for (i=0; i<pTDAL_GFXi_Pallete[i]->nbColors; i++)
        {
            pPalette->pColor[i] = pTDAL_GFXi_Pallete[i]->pColor[i];
        }
    }
    else
    {
        pPalette->nbColors = 0;
    }
    
    return eTDAL_GFX_NO_ERROR;
}

tTDAL_GFX_Palette *TDAL_GFXm_RegionPaletteGet(tTDAL_GFX_RegionHandle regionHandle)
{
    int i;
    
    for (i = 0; i < kTDAL_GFX_REGCOUNT; i++)
    {
        if (TDAL_GFX_RgnDesc[i].used == true && (regionHandle==(tTDAL_GFX_RegionHandle)&TDAL_GFX_RgnDesc[i]))
        {
            break;
        }
    }
    
    if (i < kTDAL_GFX_REGCOUNT)
    {
        return pTDAL_GFXi_Pallete[i];
    }

    return NULL;
}

/*===========================================================================
 *
 * TDAL_GFX_RegionTransparencySet
 *
 * Parameters:
 *      None
 *
 * Description:
 *
 *
 * Returns:
 *      Revision
 *
 *===========================================================================*/
tTDAL_GFX_Error   TDAL_GFX_RegionTransparencySet   (tTDAL_GFX_RegionHandle   regionHandle,
        uint8_t   alpha)
{
    tTDAL_GFX_Error     error = eTDAL_GFX_NO_ERROR;
    E_GOP_API_Result    gop_result = GOP_API_SUCCESS;
    GWINID              GeWinId = 0xff;
    int                 i;
    
    mTBOX_FCT_ENTER("TDAL_GFX_RegionHide");
    
    for(i = 0; i < kTDAL_GFX_REGCOUNT; i++)
    {
        if (TDAL_GFX_RgnDesc[i].used && regionHandle==(tTDAL_GFX_RegionHandle)&TDAL_GFX_RgnDesc[i])
        {
            GeWinId = TDAL_GFX_RgnDesc[i].GeWinId;
            transRgnIdx = i;
            break;
        }
    }
    
    if (i >= kTDAL_GFX_REGCOUNT)
    {
        error = eTDAL_GFX_UNKNOWN_REGION;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "There is mess with region handle bookkeeping!\n", regionHandle));
    }

    gop_result = MApi_GOP_GWIN_SetBlending(GeWinId, TDAL_GFX_RgnDesc[i].fbFmt == E_MS_FMT_ARGB8888, alpha);
    if (gop_result != GOP_API_SUCCESS)
    {
        error = eTDAL_GFX_DRIVER_ERROR;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "MApi_GOP_GWIN_SetBlending: failure %d\n", gop_result));
    }
    mTBOX_RETURN(error);
}

void TDAL_GFXm_ClearRegionFromDisplay(tTDAL_GFX_RegionHandle regionHandle, bool clear, bool fadeOut)
{
    int i;
    E_GOP_API_Result gop_result = GOP_API_SUCCESS;

    for(i = 0; i < kTDAL_GFX_REGCOUNT; i++)
    {

        if (    TDAL_GFX_RgnDesc[i].layerHandle!=MSTAR_INVALID_LAYER_HANDLE && 
                TDAL_GFX_RgnDesc[i].used &&
                regionHandle==(tTDAL_GFX_RegionHandle)&TDAL_GFX_RgnDesc[i])
        {
            GFX_RgbColor        color_s = {0};
            GFX_RectFillInfo    gfxFillBlock;
            GFX_BufferInfo      destinationBuffer;
            GOP_GwinFBAttr      DstFBAttr;
            uint32_t x, y, width, height;
           
            gop_result = MApi_GOP_GWIN_Switch2Gwin(TDAL_GFX_RgnDesc[i].GeWinId);
            if (gop_result != GOP_API_SUCCESS)
            {
                mTBOX_TRACE((kTBOX_NIV_CRITICAL, "MApi_GOP_GWIN_GetFBInfo: failure %d\n", gop_result));
            } 
            
            gop_result = MApi_GOP_GWIN_GetFBInfo(TDAL_GFX_RgnDesc[i].frameBufferId, &DstFBAttr);
            if (gop_result != GOP_API_SUCCESS)
            {
                mTBOX_TRACE((kTBOX_NIV_CRITICAL, "MApi_GOP_GWIN_GetFBInfo: failure %d\n", gop_result));
            } 
            destinationBuffer.u32Addr       = DstFBAttr.addr;
            destinationBuffer.u32ColorFmt   = DstFBAttr.fbFmt;
            destinationBuffer.u32Height     = DstFBAttr.height;
            destinationBuffer.u32Width      = DstFBAttr.width;
            destinationBuffer.u32Pitch      = DstFBAttr.pitch;
            gop_result = MApi_GFX_SetDstBufferInfo(&destinationBuffer, 0);
			if (gop_result != GFX_SUCCESS)
            {
                mTBOX_TRACE((kTBOX_NIV_CRITICAL, "MApi_GFX_SetDstBufferInfo: failure %d\n", gop_result));
            } 
			else
			{
				mTBOX_TRACE((kTBOX_NIV_CRITICAL, "[%s %d]addr=0x%x,pitch=0x%x,height=%d,width=%d,colorfmt=%d\n",__FUNCTION__,__LINE__,destinationBuffer.u32Addr,destinationBuffer.u32Pitch,destinationBuffer.u32Height,destinationBuffer.u32Width,destinationBuffer.u32ColorFmt));
			}

            x = TDAL_GFX_RgnDesc[i].offSet.x;
            y = TDAL_GFX_RgnDesc[i].offSet.y;
            width = TDAL_GFX_RgnDesc[i].size.width;
            height = TDAL_GFX_RgnDesc[i].size.height;

            gfxFillBlock.fmt                = destinationBuffer.u32ColorFmt;
            gfxFillBlock.flag               = GFXRECT_FLAG_COLOR_CONSTANT;            
            gfxFillBlock.dstBlock.x         = x;
            gfxFillBlock.dstBlock.y         = y;
            gfxFillBlock.dstBlock.width     = width;
            gfxFillBlock.dstBlock.height    = height;
            gfxFillBlock.colorRange.color_s = color_s;
            gfxFillBlock.colorRange.color_e = color_s;

            if (clear)
            {
                MApi_GFX_RectFill(&gfxFillBlock);
            }

            if (fadeOut)
            {
                MApi_GOP_GWIN_SetFadeInOut(TDAL_GFX_RgnDesc[i].GeWinId, E_GOP_GWIN_FADE_OUT, TRUE, 128);
            }
            else
            {
                MApi_GOP_GWIN_Enable(TDAL_GFX_RgnDesc[i].GeWinId, FALSE);
            }

            break;
        }
    }
}
__INLINE uint8_t TDAL_GFX_rgb2Y(uint8_t bRed, uint8_t bGreen, uint8_t bBlue)
{
   int   y;
   y = ((66 * (int)bRed) + (129 * (int)bGreen) + (25 * (int)bBlue))/256 + 16;

   if (y < 16)
      y = 16;
   else if (y > 255)
      y = 255;

   return (uint8_t)y;
}

__INLINE uint8_t TDAL_GFX_rgb2Cr(uint8_t bRed, uint8_t bGreen, uint8_t bBlue)
{
   int cr;
   cr = ((112 * (int)bRed) + (-94 * (int)bGreen) + (-18 * (int)bBlue))/256 + 128;
   if (cr < 0)
      cr = 0;
   else if (cr > 255)
      cr = 255;

   return (uint8_t)cr;
}

__INLINE uint8_t TDAL_GFX_rgb2Cb(uint8_t bRed, uint8_t bGreen, uint8_t bBlue)
{
   int cb;
   cb = ((-38 * (int)bRed) + (-74 * (int)bGreen) + (112 * (int)bBlue))/256 + 128;
   if   (cb < 0)
      cb = 0;
   else if (cb > 255)
      cb = 255;

   return (uint8_t)cb;
}


__INLINE uint8_t TDAL_GFX_ycrcb2R(uint8_t bY, uint8_t bCr, uint8_t bCb)
{
   int r;
   r = (298 * ((int)bY - 16) + 409 * ((int)bCr - 128) ) / 256;

   if (r < 0)
      r = 0;
   else if (r > 255)
      r = 255;

   return (uint8_t)r;
}

__INLINE uint8_t TDAL_GFX_ycrcb2G(uint8_t bY, uint8_t bCr, uint8_t bCb)
{
   int g;
   g = (298 * ((int)bY - 16) - 212 * ((int)bCr - 128) - 100 * ((int)bCb - 128) ) / 256;

   if (g < 0)
      g = 0;
   else if (g > 255)
      g = 255;

   return (uint8_t)g;
}

__INLINE uint8_t TDAL_GFX_ycrcb2B(uint8_t bY, uint8_t bCr, uint8_t bCb)
{
   int b;
   b = (298 * ((int)bY - 16) + 517 * ((int)bCb - 128)) / 256;

   if (b < 0)
      b = 0;
   else if (b > 255)
      b = 255;

   return (uint8_t)b;
}
/*
tTDAL_GFX_Error   TDAL_GFXm_BitmapSrcAllocate(tTDAL_GFX_ColorType   colorType,
      uint16_t   width,
      uint16_t   height,
      U32   *bitmapHandle,
      STGXOBJ_Bitmap_t      *bitmapSrc)
{
    return eTDAL_GFX_NO_ERROR;
}

tTDAL_GFX_Error   TDAL_GFXm_BitmapSrcFree(U32   *pMemBitmapSrcHandle)
{
return eTDAL_GFX_NO_ERROR;
}
*/

void   TDAL_GFXm_ConvertSrcAreaToDstActiveArea(
      int32_t   srcWidth,
      int32_t   srcHeight,
      int32_t   dstWidth,
      int32_t   dstHeight,
      int32_t   xPos,   int32_t   yPos,   int32_t   width,   int32_t   height,
      int32_t   *pXPos,   int32_t   *pYPos,   int32_t   *pWidth,   int32_t   *pHeight
   )
{
    
}


#ifdef   INTERACTIVE_ENGINE_ON
tTDAL_GFX_Error   TDAL_GFXm_RegionDeleteAll(void)
{    uint16_t   uRegionCounter = 0;
    
    return (eTDAL_GFX_NO_ERROR);
}


bool   TDAL_GFXm_ConvertSrcAreaToDstActiveArea_Optimized(
      int32_t   srcWidth,   int32_t   srcHeight,
      int32_t   dstWidth,   int32_t   dstHeight,
      int32_t   *pArea_src_xPos,   int32_t   *pArea_src_yPos,   int32_t   *pArea_src_width,   int32_t   *pArea_src_height,
      int32_t   *pArea_dest_xPos,   int32_t   *pArea_dest_yPos,   int32_t   *pArea_dest_width,   int32_t   *pArea_dest_height
      )
{
   return   false;
}
#endif   /*   INTERACTIVE_ENGINE_ON   */

/******************************************************************************
 * Function Name   : TDAL_GFXm_RegionHideAll
 * Description      : function used by display to hide the region
 * Side effects    :
 * Comment       :
 * Inputs        :
 * Outputs       :
 *****************************************************************************/
bool   TDAL_GFXm_RegionHideAll(tTDAL_DISP_LayerId Id)
{
    int i;
    //uint8_t gopId;
    for(i = 0; i < kTDAL_GFX_REGCOUNT; i++)
    {
         if (TDAL_GFX_RgnDesc[i].used)
         {
			//gopId = TDAL_DISPm_LayerGOPDesc(TDAL_GFX_RgnDesc[i].layerHandle);
			//if (gopId == Id)
			if (TDAL_DISPm_LayerId(TDAL_GFX_RgnDesc[i].layerHandle) == Id)
			{
				if (TDAL_GFX_RgnDesc[i].visible)
				{
					MApi_GOP_GWIN_Enable( TDAL_GFX_RgnDesc[i].GeWinId, FALSE);
				}
			}
         }
    }
    
    return eTDAL_GFX_NO_ERROR;
}
/******************************************************************************
 * Function Name   : TDAL_GFXm_RegionShowAll
 * Description      : function used by display to show the region
 * Side effects    :
 * Comment       :
 * Inputs        :
 * Outputs       :
 *****************************************************************************/
bool   TDAL_GFXm_RegionShowAll(tTDAL_DISP_LayerId Id)
{
    int i;
    //uint8_t gopId;
    for(i = 0; i < kTDAL_GFX_REGCOUNT; i++)
    {
         if (TDAL_GFX_RgnDesc[i].used)
         {
			//gopId = TDAL_DISPm_LayerGOPDesc(TDAL_GFX_RgnDesc[i].layerHandle);
			//if (gopId == Id)
			if (TDAL_DISPm_LayerId(TDAL_GFX_RgnDesc[i].layerHandle) == Id)
			{
				if (TDAL_GFX_RgnDesc[i].visible)
				{
					MApi_GOP_GWIN_Enable( TDAL_GFX_RgnDesc[i].GeWinId, TRUE);
				}
			}
         }
    }    
    return eTDAL_GFX_NO_ERROR;
}

/*===========================================================================
 *
 * TDAL_GFXm_RegionDeleteAll
 *
 * Parameters:
 *      None
 *
 * Description:
 *
 *
 * Returns:
 *      Revision
 *
 *===========================================================================*/
void   TDAL_GFXm_RegionRefresh(tTDAL_DISP_LayerId   Id)
{
}


/******************************************************************************
 * Function Name   : TDAL_GFXm_ResolutionOpenViewPort
 * Description      :
 * Side effects      :
 * Comment      :
 * Inputs      :
 * Outputs      :
 *****************************************************************************/
bool   TDAL_GFXm_ResolutionOpenViewPort(   tTDAL_DISP_LayerId   Id   )
{
    return false;
}

/******************************************************************************
 * Function Name   : TDAL_GFXm_ResolutionCloseViewPort
 * Description      :
 * Side effects      :
 * Comment      :
 * Inputs      :
 * Outputs      :
 *****************************************************************************/
bool   TDAL_GFXm_ResolutionCloseViewPort(   tTDAL_DISP_LayerId   Id   )
{
return false;
}


bool   TDAL_GFXm_isRegionExist(tTDAL_GFX_RegionHandle   regionHandle)
{
    return   false;
}



bool   TDAL_GFXm_BlenderResize(tTDAL_DISP_BlenderId   Id)
{
    return   false;
}


tTDAL_GFX_Color *TDAL_GFXm_GetCurPallete(uint8_t index)
{
    if (pTDAL_GFXi_Pallete[index] == NULL)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Error: pallete is empty\n"));
        return NULL;
    }
    else
    {
        return &pTDAL_GFXi_Pallete[index]->pColor[index];
    }
}

tTDAL_GFX_Error TDAL_GFXm_CreateFB(uint8_t *frameBufferId, uint32_t regionDescriptor, tTDAL_GFX_Size *size, tTDAL_GFX_Point *offset, uint16_t fbFmt)
{
    mTBOX_FCT_ENTER("TDAL_GFXm_CreateFB");

    TDAL_GFX_RgnDesc[regionDescriptor].alpha            = 0;
    TDAL_GFX_RgnDesc[regionDescriptor].used             = true;
    TDAL_GFX_RgnDesc[regionDescriptor].visible = false;
    TDAL_GFX_RgnDesc[regionDescriptor].offSet           = *offset;
    TDAL_GFX_RgnDesc[regionDescriptor].size             = *size;
    TDAL_GFX_RgnDesc[regionDescriptor].GeWinId          = -1;
    TDAL_GFX_RgnDesc[regionDescriptor].layerHandle      = MSTAR_INVALID_LAYER_HANDLE;
    TDAL_GFX_RgnDesc[regionDescriptor].fbFmt = fbFmt;
    
    mTBOX_RETURN(eTDAL_GFX_NO_ERROR);
}
