/*   Generic   Headers   */
#include <stdio.h>
#include <string.h>

#include "crules.h"
#include "tbox.h"
#include "tdal_common.h"

#include "tdal_disp.h"
#include "tdal_gfx.h"
#include "mdrv_types.h"

#include "MsCommon.h"
#include "MsMemory.h"

#include "drvXC_IOPort.h"
#include "drvTVEncoder.h"
//Api
#include "apiGOP.h"
#include "apiXC.h"

#include "drvXC_IOPort.h"
#include "drvMVOP.h"
#include "apiVDEC.h"
#include "apiXC.h"
#include "apiGFX.h"
#include "drvTVEncoder.h"
#include "apiDAC.h"
#include "drvSEM.h"
#include "apiVDEC_EX.h"
//MApi
#include "xc/Panel.h"
#include "xc/msAPI_XC.h"
#include "xc/msAPI_VE.h"

#include "tdal_gfx_p.h"
#include "tdal_disp_module_priv.h"

mTBOX_SET_MODULE(eTDAL_GFX);

/****************************************************************************
 *  TYPEDEFS                                                         *
 ****************************************************************************/

/****************************************************************************
 *  LOCAL MODULE VARIABLES (MODULE)                                     *
 ****************************************************************************/
LOCAL MS_U32 u32BmpDRAMStrAddr;
MS_U32 u32BmpRGB565Addr;
MS_U32 u32BmpARGB4444Addr;
MS_U32 u32BmpARGB8888Addr;
MS_U32 u32BmpI8Addr;
bool isGFXInitialized = FALSE;
/********************************************************/
/* Functions Definitions (LOCAL/GLOBAL) */
/********************************************************/
GLOBAL void TDAL_OUTPUTm_VEInit(void);
GLOBAL void TDAL_DISPm_XCInit(void);
void TDAL_GFXm_Init(void);
uint8_t TDAL_DISPm_LayerGOPDesc(uint32_t layerHandle);
LOCAL void _XC_Sys_PQ_ReduceBW_ForOSD(MS_U8 PqWin, MS_BOOL bOSD_On);
LOCAL MS_U16 _sc_get_h_cap_start(void);
LOCAL MS_BOOL _sc_is_interlace(void);
LOCAL MS_U32 _OSD_SetFBFmt(MS_U16 pitch,MS_U32 addr , MS_U16 fmt );
MS_U32 OSD_RESOURCE_GetBitmapInfoGFX(MS_S16 handle, GFX_BitmapInfo* pinfo);
LOCAL GOP_InitInfo pGopInit;
LOCAL MS_VE_Output_Ctrl OutputCtrl;
LOCAL   bool   bTDAL_GFX_AlreadyInitialized = FALSE;


BmpInfo bmpinfo;

void _GE_InitGlobalVar(void)
{
#if ENABLE_MIU_1
    u32BmpDRAMStrAddr = SDRAMADDR_BMP + MIU_INTERVAL;
#else
    u32BmpDRAMStrAddr = SDRAM_ADR_BMP_ADR;
#endif
    memset (&bmpinfo, 0xFF, sizeof (bmpinfo));
}

void _GE_SetBmpAdr(void)
{
    u32BmpARGB8888Addr=u32BmpDRAMStrAddr;
    u32BmpRGB565Addr= GE_ADDR_ALIGNMENT(u32BmpARGB8888Addr+BMPARGB8888SIZE);
    u32BmpARGB4444Addr= GE_ADDR_ALIGNMENT(u32BmpRGB565Addr+BMPRGB565SIZE);
    u32BmpI8Addr= GE_ADDR_ALIGNMENT(u32BmpARGB4444Addr+BMPARGB4444SIZE);
}

void _ScaledNotifyCb(tTDAL_DISP_GFXScale eScale)
{
    GFX_Result gfx_result  = GFX_SUCCESS;
    E_GOP_API_Result gop_result;
    GFX_DrawRect drawRect;
    GFX_BufferInfo          sourceBuffer, destinationBuffer;
    GOP_GwinFBAttr  gwinFB;
    MS_U8 gwinid = 0xFF;
    MS_U8 gfbid = 0xFF;
    MS_U8 i;
    MS_U16 u16TagId=0x0;
    MS_U16 u16QueueCnt = 1;
    GOP_GwinInfo stGWin;

    for(i=0; i< kTDAL_GFX_REGCOUNT; i++)
    {
        if (TDAL_GFX_RgnDesc[i].size.height == 720 && TDAL_GFX_RgnDesc[i].size.width == 1280)
        {
			gwinid = TDAL_GFX_RgnDesc[i].GeWinId;
            gfbid = TDAL_GFX_RgnDesc[i].frameBufferId;
            break;
        }
    }

    if (gwinid == 0xFF)
    {
        return;
    }

    MApi_GOP_GWIN_GetWinInfo(gwinid,&stGWin);

    if (eScale != eTDAL_DISP_GFXScale_720x576)
    {
        TDAL_GFX_Scaled_RgnDesc.used = false;
        stGWin.u16DispHPixelEnd = 1280;
        stGWin.u16DispVPixelEnd = 720;
        stGWin.u16RBlkHPixSize = 1280;
        stGWin.u16RBlkVPixSize = 720;
        stGWin.u16RBlkHRblkSize = (1280*4/GOP_WordUnit)<<4;

        
    }else
    {
        TDAL_GFX_Scaled_RgnDesc.used = true;
        stGWin.u16DispHPixelEnd = 720;
        stGWin.u16DispVPixelEnd = 576;
        stGWin.u16RBlkHPixSize = 720;
        stGWin.u16RBlkVPixSize = 576;  
        stGWin.u16RBlkHRblkSize = (720*4/GOP_WordUnit)<<4;
        
        memset(&sourceBuffer, 		0, 	sizeof(sourceBuffer));
        memset(&destinationBuffer, 	0, 	sizeof(destinationBuffer));

        //Src FB
        gop_result = MApi_GOP_GWIN_GetFBInfo(gfbid, &gwinFB);

        if (gop_result != GOP_API_SUCCESS)
        {
            mTBOX_TRACE((kTBOX_NIV_CRITICAL, "MApi_GOP_GWIN_GetFBInfo: failure %d\n", gop_result));
            return;
        } 

        sourceBuffer.u32Addr        = gwinFB.addr;
        sourceBuffer.u32ColorFmt    = gwinFB.fbFmt;
        sourceBuffer.u32Height      = gwinFB.height;
        sourceBuffer.u32Width       = gwinFB.width;
        sourceBuffer.u32Pitch       = gwinFB.pitch;

        //Dst FB
        gfbid = TDAL_GFX_Scaled_RgnDesc.frameBufferId;
        gop_result = MApi_GOP_GWIN_GetFBInfo(gfbid, &gwinFB);

        destinationBuffer.u32Addr       = gwinFB.addr;
        destinationBuffer.u32ColorFmt   = gwinFB.fbFmt;
        destinationBuffer.u32Height     = gwinFB.height;
        destinationBuffer.u32Width      = gwinFB.width;
        destinationBuffer.u32Pitch      = gwinFB.pitch;

        gfx_result = MApi_GFX_SetSrcBufferInfo(&sourceBuffer, 0);
        mTBOX_ASSERT(gfx_result == GFX_SUCCESS);
        gfx_result = MApi_GFX_SetDstBufferInfo(&destinationBuffer, 0);
        mTBOX_ASSERT(gfx_result == GFX_SUCCESS);

        drawRect.srcblk.x = 0;
        drawRect.srcblk.y = 0;
        drawRect.srcblk.height = sourceBuffer.u32Height;
        drawRect.srcblk.width = sourceBuffer.u32Width ;

        drawRect.dstblk.x = 0;
        drawRect.dstblk.y = 0;
        drawRect.dstblk.height = destinationBuffer.u32Height;
        drawRect.dstblk.width = destinationBuffer.u32Width ;
        
        gfx_result = MApi_GFX_BitBlt(&drawRect, GFXDRAW_FLAG_SCALE);
        
        MApi_GFX_FlushQueue();
    }

    MApi_GOP_GWIN_SetWinInfo(gwinid,&stGWin);
        
    if(MApi_GOP_Switch_GWIN_2_FB(gwinid, gfbid, u16TagId, &u16QueueCnt))
    {
        TDAL_GFX_Scaled_RgnDesc.GeWinId = gwinid;
        return;
    }
}

/*===========================================================================
 *
 * TDAL_GFX_Init
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
tTDAL_GFX_Error   TDAL_GFX_Init(void)
{
	mTBOX_FCT_ENTER("TDAL_GFX_Init");
    
    tTDAL_DISP_Error   error = eTDAL_DISP_NO_ERROR;  

    if   (bTDAL_GFX_AlreadyInitialized)
    {
        mTBOX_RETURN(error);
    }

    //XC Init
    TDAL_DISPm_XCInit();

    //VE Init
    TDAL_OUTPUTm_VEInit(); 

    //GFX Init
    TDAL_GFXm_Init();
    
//    _GE_InitGlobalVar();
//    _GE_SetBmpAdr();
//    MApi_GFX_RegisterGetBMPCB(OSD_RESOURCE_GetBitmapInfoGFX);

    TDAL_GFXm_InitBitmapDescriptors();

    TDAL_GFXm_InitScaledDescriptors();

    TDAL_DISPm_SetNotifyEvent(_ScaledNotifyCb);

    bTDAL_GFX_AlreadyInitialized = TRUE;

    mTBOX_RETURN(error);
}

/*===========================================================================
 *
 * TDAL_GFX_Terminate
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
tTDAL_GFX_Error   TDAL_GFX_Terminate(void)
{
    mTBOX_FCT_ENTER("TDAL_GFX_Terminate");

    if (TDAL_GFX_Scaled_RgnDesc.frameBufferId != 0xFF)
    {
        MApi_GOP_GWIN_Destroy32FB(TDAL_GFX_Scaled_RgnDesc.frameBufferId);
        TDAL_GFX_Scaled_RgnDesc.GeWinId = 0xFF;
        TDAL_GFX_Scaled_RgnDesc.frameBufferId = 0xFF;
        TDAL_GFX_Scaled_RgnDesc.used = false;
    }
    bTDAL_GFX_AlreadyInitialized = FALSE;

    mTBOX_RETURN(eTDAL_GFX_NO_ERROR);

}

/*===========================================================================
 *
 * TDAL_GFX_CapabilitiesGet
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
tTDAL_GFX_Error   TDAL_GFX_CapabilitiesGet(tTDAL_GFX_Capabilities*   pCapabilities)
{
    mTBOX_FCT_ENTER("TDAL_GFX_CapabilitiesGet");
	mTBOX_RETURN(eTDAL_GFX_NO_ERROR);
}

/*===========================================================================
 *
 * TDAL_GFX_APIRevisionGet
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
const   char   *TDAL_GFX_APIRevisionGet(void)
{
   return   "";
}
/*===========================================================================
 *
 * TDAL_GFX_PlatformRevisionGet
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
const   char   *TDAL_GFX_PlatformRevisionGet(void)
{
   return   "";
}

LOCAL void _XC_Sys_PQ_ReduceBW_ForOSD(MS_U8 PqWin, MS_BOOL bOSD_On)
{
    PqWin=bOSD_On=0;
}

LOCAL MS_U16 _sc_get_h_cap_start(void)
{
    // this function should support "multiple process"
    MS_WINDOW_TYPE stCapWin;

    // this function only used when OSD input from IP, so if no input, OSD should be attached to OP
    MApi_XC_GetCaptureWindow(&stCapWin, MAIN_WINDOW);       // read from register directly

    return stCapWin.x;
}

LOCAL MS_BOOL _sc_is_interlace(void)
{
    return 0;
}

LOCAL MS_U32 _OSD_SetFBFmt(MS_U16 pitch,MS_U32 addr , MS_U16 fmt )
{

    GFX_BufferInfo dstbuf;

    printf("OSD set format\n");
    printf("format = 0x%x\n",fmt);
    dstbuf.u32ColorFmt = (GFX_Buffer_Format)(fmt&0xff);
    dstbuf.u32Addr = addr;
    dstbuf.u32Pitch = pitch;
    printf("SetFBFmt: %x, %lx, %lx\n",dstbuf.u32ColorFmt,dstbuf.u32Addr,dstbuf.u32Pitch  );
    return 0;
}


void TDAL_GFXm_Init()
{
    GFX_Config tGFXcfg;
    E_GOP_API_Result gop_result;

    if (isGFXInitialized)
        return;
    MDrv_SEM_Init();

    tGFXcfg.bIsCompt = TRUE;
    tGFXcfg.bIsHK = TRUE;
    MApi_GFX_Init(&tGFXcfg);
    //Initial GOP
    MApi_GOP_RegisterFBFmtCB(( MS_U32(*)(MS_U16 pitch,MS_U32 addr , MS_U16 fmt ))_OSD_SetFBFmt);
    MApi_GOP_RegisterXCIsInterlaceCB(_sc_is_interlace);
    MApi_GOP_RegisterXCGetCapHStartCB(_sc_get_h_cap_start);
    MApi_GOP_RegisterXCReduceBWForOSDCB(_XC_Sys_PQ_ReduceBW_ForOSD);

    pGopInit.u16PanelWidth = g_IPanel.Width();
    pGopInit.u16PanelHeight = g_IPanel.Height();
    pGopInit.u16PanelHStr = g_IPanel.HStart();

#if ENABLE_MIU_1
    pGopInit.u32GOPRBAdr = GOP_GWIN_RB_ADR + MIU_INTERVAL;
#else
    pGopInit.u32GOPRBAdr = GOP_GWIN_RB_ADR;
#endif
    pGopInit.u32GOPRBLen = GOP_GWIN_RB_LEN;
#if ENABLE_MIU_1
    pGopInit.u32GOPRegdmaAdr = GOP_REG_DMA_BASE_ADR + MIU_INTERVAL;
#else
    pGopInit.u32GOPRegdmaAdr = GOP_REG_DMA_BASE_ADR;
#endif
    pGopInit.u32GOPRegdmaLen = GOP_REG_DMA_BASE_LEN;
    pGopInit.bEnableVsyncIntFlip = FALSE;
    MApi_GOP_Init(&pGopInit);
    printf("GOP_GWIN_RB_ADR : %x, GOP_REG_DMA_BASE_ADR: %x\n", GOP_GWIN_RB_ADR, GOP_REG_DMA_BASE_ADR);
    printf("driver GOP init ok\n");

    /*---------------GOP0: OSD---------------*/
    gop_result = MApi_GOP_GWIN_SwitchGOP(LAYER_OSD1);//use GOP 0
    mTBOX_ASSERT(gop_result == GOP_API_SUCCESS);
    gop_result = MApi_GOP_GWIN_EnableTransClr(GOPTRANSCLR_FMT0, FALSE);
    mTBOX_ASSERT(gop_result == GOP_API_SUCCESS);
    gop_result = MApi_GOP_GWIN_OutputColor(GOPOUT_YUV);
    mTBOX_ASSERT(gop_result == GOP_API_SUCCESS);
    gop_result = MApi_GOP_GWIN_SetGOPDst(LAYER_OSD1, E_GOP_DST_OP0);
    mTBOX_ASSERT(gop_result == GOP_API_SUCCESS);

    /*---------------GOP1: Subtitles+Media---------------*/
    gop_result = MApi_GOP_GWIN_SwitchGOP(LAYER_OSD2);//use GOP 1
    mTBOX_ASSERT(gop_result == GOP_API_SUCCESS);
    gop_result = MApi_GOP_GWIN_EnableTransClr(GOPTRANSCLR_FMT0, FALSE);
    mTBOX_ASSERT(gop_result == GOP_API_SUCCESS);
    gop_result = MApi_GOP_GWIN_OutputColor(GOPOUT_YUV);
    mTBOX_ASSERT(gop_result == GOP_API_SUCCESS);
    gop_result = MApi_GOP_GWIN_SetGOPDst(LAYER_OSD2, E_GOP_DST_OP0);
    mTBOX_ASSERT(gop_result == GOP_API_SUCCESS);
    
    // for CVBS output, use VE
    // To choose video to VE mux
    MApi_XC_SetOSD2VEMode(E_VOP_SEL_OSD_BLEND2); // set OP can through VE out-put to CVBS

    // set output control
    OutputCtrl.bEnable = TRUE;
    OutputCtrl.OutputType = MS_VE_OUT_TVENCODER;
    MDrv_VE_SetOutputCtrl(&OutputCtrl);

    // disable blackScreen
    MDrv_VE_SetBlackScreen(FALSE);

    // Init GOPD
    MApi_GOP_DWIN_Init();
    isGFXInitialized = TRUE;
}

void TDAL_GFXm_LayerEnable(uint8_t GOP_Id)
{
    uint8_t gopId;    
    int i;
    
    for(i=0; i< kTDAL_GFX_REGCOUNT; i++)
    {
        if (TDAL_GFX_RgnDesc[i].used)
        {
			gopId = TDAL_DISPm_LayerGOPDesc(TDAL_GFX_RgnDesc[i].layerHandle);
			if (gopId == GOP_Id)
			{
			    mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_GFXm_LayerDisable: GOPID=%d GWINID=%d\n", gopId, TDAL_GFX_RgnDesc[i].GeWinId));
				MApi_GOP_GWIN_Enable(TDAL_GFX_RgnDesc[i].GeWinId, TRUE);
			}
        }
    }
}

void TDAL_GFXm_LayerDisable(uint8_t GOP_Id)
{
    uint8_t gopId = 0;    
    int i;
    
    for(i=0; i< kTDAL_GFX_REGCOUNT; i++)
    {
    	if (TDAL_GFX_RgnDesc[i].used)
    	{
			gopId = TDAL_DISPm_LayerGOPDesc(TDAL_GFX_RgnDesc[i].layerHandle);
			if (gopId == GOP_Id)
			{
			    mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_GFXm_LayerDisable: GOPID=%d GWINID=%d\n", gopId, TDAL_GFX_RgnDesc[i].GeWinId));
				MApi_GOP_GWIN_Enable(TDAL_GFX_RgnDesc[i].GeWinId, FALSE);
			}
    	}
    }
}
