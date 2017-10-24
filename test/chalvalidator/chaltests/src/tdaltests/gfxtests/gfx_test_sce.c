/** \file gfx_test_sce.c
 *  \brief Scenario tests for the GFX layer of the TDAL API
 *  \author Pascal PIGNON
 *  \date 20/04/06
 *  \version 1.0
 */

/******************************************************
 *              Includes                              *
 ******************************************************/

#include "string.h"

#include "tkel.h"
#include "tbox.h"
#include "tdal_avs.h"
#include "tdal_gfx.h"
#include "tdal_output.h"
#include "tdal_disp.h"
#include "tdal_hdmi.h"
#include "tdal_common.h"

#include "testhelper.h"
#include "gfxtests.h"
#include "gfx_test_bmp.h"

/******************************************************
 *              Defines                               *
 ******************************************************/
#ifndef min
#define min(a,b)      (((a) < (b)) ? (a) : (b))
#endif

extern void HDMICallback(tTDAL_HDMI_SourceId Id, tTDAL_HDMI_Event Event);

/******************************************************
 *              DMD test tree                         *
 ******************************************************/

static tTestNode gTestSceInitTerm = {
    "TestSceInitTerm",
    "test a scenario with TDAL_GFX_Init() and TDAL_GFX_Term() functions",
    0, /* not interactive */
    GFX_TestScenario_InitTerm,
    NULL
};

static tTestNode gTestSceMaxRegionCreation = {
    "TestSceMaxRegionCreation",
    "test to know maximum regions available",
    0, /* not interactive */
    GFX_TestScenario_MaxRegionCreation,
    NULL
};

static tTestNode gTestSceRegionOverlap = {
    "TestSceRegionOverlap",
    "test wether it is possible or not to support region overlapping",
    0, /* not interactive */
    GFX_TestScenario_RegionOverlap,
    NULL
};

static tTestNode gTestSceRegionDraw = {
    "TestSceRegionDraw",
    "test TDAL_GFX_RectangleDraw() and TDAL_GFX_BmpDraw() functions",
    0, /* not interactive */
    GFX_TestScenario_RegionDraw,
    NULL
};

static tTestNode gTestSceMultiRegionDraw = {
    "TestSceMultiRegionDraw",
    "Draw in 2 different regions",
    0,
    GFX_TestScenario_MultiRegionDraw,
    NULL
};

static tTestNode gTestSceColorSystemDraw = {
    "TestSceColorSystemDraw",
    "Mix color systems",
    0,
    GFX_TestScenario_ColorSystemDraw,
    NULL
};



static tTestNode gTestSceTransparency = {
    "TestSceTransparency",
    "Test transparency",
    0,
    GFX_TestScenario_Transparency,
    NULL
};

static tTestNode gTestSceMemcpy = {
    "TestSceMemcpy",
    "Test TDAL_GFX_MemcpyNonZero function",
    0,
    GFX_TestScenario_Memcpy,
    NULL
};

static tTestNode gTestSceLayerSize = {
    "TestSceLayerSize576Lines",
    "Test GFX layer size",
    0,
    GFX_TestScenario_LayerSize576,
    NULL
};

static tTestNode gTestSceLayerSize720 = {
    "TestSceLayerSize720Lines",
    "Test GFX layer size 1280x720",
    0,
    GFX_TestScenario_LayerSize720,
    NULL
};

static tTestNode gTestSceLayerSize1080 = {
    "TestSceLayerSize1080Lines",
    "Test GFX layer size 1920x1080",
    0,
    GFX_TestScenario_LayerSize1080,
    NULL
};

static tTestNode gTestSceBlit = {
        "TestSceBlit",
        "Test graphic blitting",
        0,
        GFX_TestScenario_Blit,
        NULL
};

#if 0 // gTestSceBlit1 not currently used
static tTestNode gTestSceBlit1 = {
    "TestSceBlit1",
    "Test graphic blitting",
    0,
    GFX_TestScenario_Blit1,
    NULL
};
#endif


tTestNode* gGFXScenarioTestNodeList[] = {
    &gTestSceInitTerm,
    &gTestSceMaxRegionCreation,
    &gTestSceRegionOverlap,
    &gTestSceRegionDraw,
    &gTestSceMultiRegionDraw,
    &gTestSceColorSystemDraw,
    &gTestSceTransparency,
    &gTestSceMemcpy,
    &gTestSceLayerSize,
    &gTestSceLayerSize720,
    &gTestSceLayerSize1080,
    &gTestSceBlit,
    NULL
};

tTestNode gTDAL_GFX_ScenarioTestSuite = {
    "TDAL_GFX Scenario Tests",
    "test the TDAL_GFX function input parameters",
    0, /* not interactive */
    NULL, /* no function, it's a test suite */
    (void**)gGFXScenarioTestNodeList
};

static void pGFX_TestScenario_LayerSize(uint8_t mode);

static uint32_t            gGFXLayerHandle[4] = {0, 0, 0, 0};
static tTDAL_DISP_LayerId  gGFXLayerTable[4] = {(-1), (-1), (-1), (-1)};
static uint8_t             gGFXNbLayer = 0;

uint8_t pGFX_TestScenario_LayerIdxGet(tTDAL_DISP_LayerId LayerId)
{
    uint8_t ucRet = (-1);
    uint8_t layerIdx;
    
    for(layerIdx = 0; layerIdx < gGFXNbLayer; layerIdx++)
    {
        if(gGFXLayerTable[layerIdx] == LayerId)
        {
            ucRet = layerIdx;
            break;
        }
    }

    return(ucRet);
}

uint32_t pGFX_TestScenario_LayerHandleGet(tTDAL_DISP_LayerId LayerId)
{
    uint32_t ucRet = (-1);
    uint8_t layerIdx;
    
    for(layerIdx = 0; layerIdx < gGFXNbLayer; layerIdx++)
    {
        if(gGFXLayerTable[layerIdx] == LayerId)
        {
            ucRet = gGFXLayerHandle[layerIdx];
            break;
        }
    }

    return(ucRet);
}

void pGFX_TestScenario_InitTest(uint8_t mode)
{
    tTDAL_OUTPUT_VideoHDDACParams stHDDACParams;
    tTDAL_OUTPUT_Capability       stOutputCap;
    tTDAL_DISP_LayerWindow        stInputWindow;
    tTDAL_DISP_LayerWindow        stOutputWindow;
    tTDAL_DISP_Capability         stDispCap;
    tTDAL_DISP_BlenderCapability  stBlenderCap;
    uint8_t                       blenderIdx;
    uint8_t                       layerIdx;
    uint8_t                       layerIdx1;
    tTDAL_DISP_BlenderId          blenderId[4] =
    {
        eTDAL_DISP_BLENDER_ID_0, /* Blender identifier 0 */
        eTDAL_DISP_BLENDER_ID_1, /* Blender identifier 1 */
        eTDAL_DISP_BLENDER_ID_2, /* Blender identifier 2 */
        eTDAL_DISP_BLENDER_ID_3  /* Blender identifier 3 */
    };
    tTDAL_DISP_BlenderZOrderPositions *pstTdalDispZOrderListPos;
    TKEL_err                           eTkelErr;
    bool                               bFound;
    uint32_t                           BlenderWidth;
    uint32_t                           BlenderHeight;

#if 0 //Currently not used. Use case commented out
    tTDAL_DISP_BlenderBackgroundColor pstColor;
#endif

    TestManager_AssertEqual( TDAL_GFX_Init(), eTDAL_GFX_NO_ERROR, "GFX init" );
    
#ifdef AVS_STB
    TestManager_AssertEqual( TDAL_AVS_Init(), eTDAL_AVS_NO_ERROR, "AVS init" );
    TestHelper_Configure_TDAL_AVS();
#endif
    TestManager_AssertEqual( TDAL_OUTPUT_Init(), eTDAL_OUTPUT_NO_ERROR, "output init" );
    
#ifdef TDAL_HDMI_USE
    TestManager_AssertEqual(TDAL_HDMI_Init(), eTDAL_HDMI_NO_ERROR, "hdmi init" );
#endif

    TestManager_AssertEqual( TDAL_DISP_Init(), eTDAL_DISP_NO_ERROR, "disp init" );


    memset(&stOutputCap, 0, sizeof(tTDAL_OUTPUT_Capability));
    TestManager_AssertEqual(TDAL_OUTPUT_CapabilityGet(&stOutputCap), eTDAL_OUTPUT_NO_ERROR, "output caps");
    
    if(stOutputCap.nbVideoHDDAC > 0)
    {
        stHDDACParams.ColorSpace     = eTDAL_OUTPUT_VIDEO_HD_DAC_ITU601;
        stHDDACParams.Format         = eTDAL_OUTPUT_VIDEO_HD_DAC_YCBCR;
        stHDDACParams.Standard       = eTDAL_OUTPUT_VIDEO_HD_STD_1920_1080_I_50;
        stHDDACParams.EmbeddedSyncOn = FALSE;
        TestManager_AssertEqual(TDAL_OUTPUT_VideoHDDACParamsSet(eTDAL_OUTPUT_VIDEO_HD_DAC_ID_0, &stHDDACParams), eTDAL_OUTPUT_NO_ERROR, "output hd dac params set");
        TestManager_AssertEqual(TDAL_OUTPUT_Enable(eTDAL_OUTPUT_VIDEO_HD_DAC_ID_0), eTDAL_OUTPUT_NO_ERROR, "output hd dac enable");
    }
    
#ifdef TDAL_HDMI_USE
    if(stOutputCap.nbHDMI > 0)
    {
        TestManager_AssertEqual(TDAL_HDMI_Start(eTDAL_HDMI_SOURCE_ID_0, HDMICallback, FALSE), eTDAL_HDMI_NO_ERROR, "hdmi start" );
    }
#endif

    if(stOutputCap.nbVideoDenc > 0)
    {
        TestManager_AssertEqual(TDAL_OUTPUT_Enable(eTDAL_OUTPUT_VIDEO_DENC_ID_0), eTDAL_OUTPUT_NO_ERROR, "output denc video enable" );
    }

    TestManager_AssertEqual(TDAL_DISP_CapabilityGet(&stDispCap),
                        eTDAL_DISP_NO_ERROR,
                        "disp caps");
    for(blenderIdx = 0; blenderIdx < stDispCap.nbBlender; blenderIdx++)
    {
        TestManager_AssertEqual(TDAL_DISP_BlenderDimensionsGet(blenderId[blenderIdx],
                                                               &BlenderWidth,
                                                               &BlenderHeight),
                                eTDAL_DISP_NO_ERROR,
                                "DISP get blender dimensions" );
        TestManager_AssertEqual(TDAL_DISP_BlenderCapabilityGet(blenderId[blenderIdx],
                                                               &stBlenderCap),
                                eTDAL_DISP_NO_ERROR,
                                "disp blender caps");
        /* allocate ZOrder possible positions structure */
        eTkelErr = TKEL_Malloc(stBlenderCap.nbLayer * sizeof(tTDAL_DISP_BlenderZOrderPositions),
                               (void*)&pstTdalDispZOrderListPos);
        mTBOX_ASSERT(eTkelErr == TKEL_NO_ERR);
    
        if(pstTdalDispZOrderListPos != NULL)
        {
            TestManager_AssertEqual(TDAL_DISP_BlenderZOrderPositionsGet(blenderId[blenderIdx],
                                                                        pstTdalDispZOrderListPos),
                                    eTDAL_DISP_NO_ERROR,
                                    "disp blender zorder positions get");
    
            for(layerIdx = 0; layerIdx < stBlenderCap.nbLayer; layerIdx++)
            {
                /* search for gfx layer Id presence */
                if((pstTdalDispZOrderListPos[layerIdx].LayerId == eTDAL_DISP_LAYER_GFX_ID_0) ||
                   (pstTdalDispZOrderListPos[layerIdx].LayerId == eTDAL_DISP_LAYER_GFX_ID_1) ||
                   pstTdalDispZOrderListPos[layerIdx].LayerId == eTDAL_DISP_LAYER_GFX_ID_2)
                {
                    /* check if layer not already found */
                    bFound = FALSE;
                    for(layerIdx1 = 0; layerIdx1 < gGFXNbLayer; layerIdx1++)
                    {
                        if(gGFXLayerTable[layerIdx1] == pstTdalDispZOrderListPos[layerIdx].LayerId)
                        {
                            bFound = TRUE;
                        }
                    }
                    
                    /* not already found, store it in the list & get handle */
                    if(bFound == FALSE)
                    {
                        gGFXLayerTable[gGFXNbLayer] = pstTdalDispZOrderListPos[layerIdx].LayerId;
                        switch(mode)
                        {
                        case GFX_TEST_MODE_720:
                            stInputWindow.Left   = 0;
                            stInputWindow.Top    = 0;
                            stInputWindow.Right  = 1280 - 1;
                            stInputWindow.Bottom = 720 - 1;
                            break;
                        case GFX_TEST_MODE_1080:
                            stInputWindow.Left   = 0;
                            stInputWindow.Top    = 0;
                            stInputWindow.Right  = 1920 - 1;
                            stInputWindow.Bottom = 1080 - 1;
                            break;
                        case GFX_TEST_MODE_576:
                        default:
                            stInputWindow.Left    = 0;
                            stInputWindow.Top     = 0;
                            stInputWindow.Right   = GFX_TEST_DEFAULT_WIDTH - 1;
                            stInputWindow.Bottom  = GFX_TEST_DEFAULT_HEIGHT - 1;
                            break;
                        }
                        
                        stOutputWindow.Left   = 0;
                        stOutputWindow.Top    = 0;
                        stOutputWindow.Right  = BlenderWidth - 1;
                        stOutputWindow.Bottom = BlenderHeight - 1;

                        TestManager_AssertEqual(TDAL_DISP_LayerIOWindowsSet(gGFXLayerTable[gGFXNbLayer],
                                                &stInputWindow,
                                                &stOutputWindow),
                                                eTDAL_DISP_NO_ERROR,
                                                "DISP GFX layer set image IOWindows" );

                        TestManager_AssertEqual(TDAL_DISP_LayerHandleGet(gGFXLayerTable[gGFXNbLayer],
                                                &gGFXLayerHandle[gGFXNbLayer]),
                                                eTDAL_DISP_NO_ERROR,
                                                "DISP layer handle get" );
                        gGFXNbLayer++;
                    }
                }
            }
    
            /* free ZOrder possible positions structure */
            eTkelErr = TKEL_Free((void*)pstTdalDispZOrderListPos);
            mTBOX_ASSERT(eTkelErr == TKEL_NO_ERR);
        }
    }

/*
    pstColor.PixelFormat = kTDAL_DISP_PIXEL_FORMAT_ARGB8888;
    pstColor.Color = 0xFFFF00FF;
    
    TDAL_DISP_BlenderBackgroundColorSet(eTDAL_DISP_BLENDER_ID_0,&pstColor);

*/
    return;
}

void pGFX_TestScenario_TerminateTest(void)
{
    tTDAL_OUTPUT_Capability       stOutputCap;

    TestManager_AssertEqual( TDAL_GFX_Terminate(), eTDAL_GFX_NO_ERROR, "GFX terminate" );
#ifdef AVS_STB
    TestManager_AssertEqual( TDAL_AVS_Term(), eTDAL_AVS_NO_ERROR, "AVS term" );
#endif

    memset(&stOutputCap, 0, sizeof(tTDAL_OUTPUT_Capability));
    TestManager_AssertEqual(TDAL_OUTPUT_CapabilityGet(&stOutputCap), eTDAL_OUTPUT_NO_ERROR, "output caps");
    
    if(stOutputCap.nbVideoHDDAC > 0)
    {
        TestManager_AssertEqual(TDAL_OUTPUT_Disable(eTDAL_OUTPUT_VIDEO_HD_DAC_ID_0), eTDAL_OUTPUT_NO_ERROR, "output hd dac disable");
    }
    
#ifdef TDAL_HDMI_USE
    if(stOutputCap.nbHDMI > 0)
    {
        TestManager_AssertEqual(TDAL_HDMI_Stop(eTDAL_HDMI_SOURCE_ID_0), eTDAL_HDMI_NO_ERROR, "hdmi stop" );
    }
#endif

    if(stOutputCap.nbVideoDenc > 0)
    {
        TestManager_AssertEqual(TDAL_OUTPUT_Disable(eTDAL_OUTPUT_VIDEO_DENC_ID_0), eTDAL_OUTPUT_NO_ERROR, "output denc video disable" );
    }


#ifdef AVS_STB
    TestManager_AssertEqual( TDAL_AVS_Term(), eTDAL_AVS_NO_ERROR, "AVS term" );
#endif

    TestManager_AssertEqual( TDAL_OUTPUT_Term(), eTDAL_OUTPUT_NO_ERROR, "output term" );
    TestManager_AssertEqual( TDAL_DISP_Term(), eTDAL_DISP_NO_ERROR, "disp term" );
#ifdef TDAL_HDMI_USE
    TestManager_AssertEqual(TDAL_HDMI_Term(), eTDAL_HDMI_NO_ERROR, "hdmi term" );
#endif
    gGFXNbLayer       = 0;
    gGFXLayerTable[0] = (-1);
    gGFXLayerTable[1] = (-1);
    gGFXLayerTable[2] = (-1);
    gGFXLayerTable[3] = (-1);

    return;
}


/******************************************************
 *              Scenario tests                        *
 ******************************************************/

/*! \fn void GFX_TestScenario_InitTerm( void )
 *  \brief Test a scenario with initialization and
 *  termination of GFX driver
 */
void GFX_TestScenario_InitTerm()
{
    tTDAL_GFX_Capabilities  GfxCapabilities;

    TestManager_AssertEqual( TDAL_GFX_Init(), eTDAL_GFX_NO_ERROR, "GFX init" );
    TestManager_AssertEqual( TDAL_GFX_CapabilitiesGet( &GfxCapabilities ),
                            eTDAL_GFX_NO_ERROR, "Retrieve GFX capabilities" );
    mTBOX_PRINT(( kTBOX_LF, "GFX capabilities none for the moment\n"));
    TestManager_AssertEqual( TDAL_GFX_Terminate(), eTDAL_GFX_NO_ERROR, "GFX terminate" );
}

/*! \fn void GFX_TestScenario_MaxRegionCreation( void )
 *  \brief Test the maximum number of regions
 *  \bug Due to an unknown reason for the moment this test
 *  very often hangs or make following tests hang on
 *  RegionCreate() function.
 */
void GFX_TestScenario_MaxRegionCreation()
{
    tTDAL_GFX_Capabilities          Capabilities;
    tTDAL_GFX_RegionHandle  GfxHandle[32];
    tTDAL_GFX_RegionConfig  Params;
    int                     i = 0;
    uint8_t                 Layer;
    #define MAX_REGION_COUNT 2

    pGFX_TestScenario_InitTest(GFX_TEST_MODE_576);

    TestManager_AssertEqual( TDAL_GFX_CapabilitiesGet( &Capabilities ), eTDAL_GFX_NO_ERROR, "GFX get capabilities" );


    /* Set up default params */
    Params.offset.x    = GFX_TEST_DEFAULT_X;
    Params.offset.y    = GFX_TEST_DEFAULT_Y;
    Params.size.width  = GFX_TEST_DEFAULT_WIDTH;
    Params.size.height = GFX_TEST_DEFAULT_HEIGHT;
    Params.refSize.width  = GFX_TEST_DEFAULT_WIDTH;
    Params.refSize.height = GFX_TEST_DEFAULT_HEIGHT;
    Params.colorType   = GFX_TEST_DEFAULT_COLOR_TYPE;
    Params.alpha       = kTDAL_GFX_OPAQUE;

    Layer = pGFX_TestScenario_LayerIdxGet(eTDAL_DISP_LAYER_GFX_ID_0);

    /* Create max region */
    while ( (i< MAX_REGION_COUNT) && TDAL_GFX_RegionCreate( &GfxHandle[i], &Params) == eTDAL_GFX_NO_ERROR )
    {
        TestManager_AssertEqual(TDAL_GFX_RegionDestinationSet(GfxHandle[i],
                                                              gGFXLayerHandle[Layer]),
                                eTDAL_GFX_NO_ERROR,
                                "GFX region destination set" );
        i++;
    }
    mTBOX_PRINT(( kTBOX_LF, "Maximum of available regions: %d\n", i ));
    
    /* Terminate */
    while( i > 0 )
    {
        TestManager_AssertEqual( TDAL_GFX_RegionDelete( GfxHandle[--i] ),
                                eTDAL_GFX_NO_ERROR, "Delete Region" );
    }

    pGFX_TestScenario_TerminateTest();
}

/*! \fn void GFX_Scenario_RegionOverlap( void )
 *  \brief Test functions like TDAL_GFX_RegionShow, TDAL_GFX_RectangleDraw,
 *  TDAL_GFX_BmpDraw with bad handle.
 */
void GFX_TestScenario_RegionOverlap()
{
    tTDAL_GFX_Capabilities  Capabilities;
    tTDAL_GFX_RegionHandle  GfxHandle1, GfxHandle2;
    tTDAL_GFX_RegionConfig  Params;
    uint8_t                 Layer;
    
    pGFX_TestScenario_InitTest(GFX_TEST_MODE_576);
    
    Layer = pGFX_TestScenario_LayerIdxGet(eTDAL_DISP_LAYER_GFX_ID_0);

    TestManager_AssertEqual( TDAL_GFX_CapabilitiesGet( &Capabilities ), eTDAL_GFX_NO_ERROR, "GFX get capabilities" );
    
    /* Set up default params */
    Params.offset.x    = GFX_TEST_DEFAULT_X;
    Params.offset.y    = GFX_TEST_DEFAULT_Y;
    Params.size.width  = GFX_TEST_DEFAULT_WIDTH;
    Params.size.height = GFX_TEST_DEFAULT_HEIGHT;
    Params.refSize.width  = GFX_TEST_DEFAULT_WIDTH;
    Params.refSize.height = GFX_TEST_DEFAULT_HEIGHT;
    Params.colorType   = GFX_TEST_DEFAULT_COLOR_TYPE;
    Params.alpha       = kTDAL_GFX_OPAQUE;

    TestManager_AssertEqual( TDAL_GFX_RegionCreate( &GfxHandle1, &Params ),
                            eTDAL_GFX_NO_ERROR, "Create GFX region #1" );
    TestManager_AssertEqual(TDAL_GFX_RegionDestinationSet(GfxHandle1,
                                                          gGFXLayerHandle[Layer]),
                            eTDAL_GFX_NO_ERROR,
                            "GFX region destination set #1" );
    TestManager_AssertEqual( TDAL_GFX_RegionCreate( &GfxHandle2, &Params ),
                            eTDAL_GFX_NO_ERROR, "Create GFX region #2" );
    TestManager_AssertEqual(TDAL_GFX_RegionDestinationSet(GfxHandle2,
                                                          gGFXLayerHandle[Layer]),
                            eTDAL_GFX_NO_ERROR,
                            "GFX region destination set #2" );

    /* Show region */
    TestManager_AssertEqual( TDAL_GFX_RegionShow( GfxHandle1 ),
                            eTDAL_GFX_NO_ERROR, "Show region #1" );
    TestManager_LogMessage( "Show region #2" );
    if( TDAL_GFX_RegionShow( GfxHandle2 ) != eTDAL_GFX_NO_ERROR )
    {
        TestManager_LogMessage( "WARNING: Region overlapping is not supported" );
    }
    else
    {
        TestManager_LogMessage( "WARNING: Region overlapping is supported" );
    }

    /* Hide region */
    TestManager_AssertEqual( TDAL_GFX_RegionHide( GfxHandle1 ),
                            eTDAL_GFX_NO_ERROR, "Hide region #1" );
    TestManager_AssertEqual( TDAL_GFX_RegionHide( GfxHandle2 ),
                            eTDAL_GFX_NO_ERROR, "Hide region #2" );
    
    /* Terminate test */
    TestManager_AssertEqual( TDAL_GFX_RegionDelete( GfxHandle1 ),
                            eTDAL_GFX_NO_ERROR, "Delete Region #1" );
    TestManager_AssertEqual( TDAL_GFX_RegionDelete( GfxHandle2 ),
                            eTDAL_GFX_NO_ERROR, "Delete Region #2" );
    pGFX_TestScenario_TerminateTest();
}

/*! \fn void GFX_TestScenario_RegionDraw( void )
 *  \brief Draw 5 valid rectangles and bitmaps
 */
void GFX_TestScenario_RegionDraw()
{
    tTDAL_GFX_Capabilities          Capabilities;
    tTDAL_GFX_RegionHandle  GfxHandle;
    tTDAL_GFX_RegionConfig  Params;
    tTDAL_GFX_Bitmap        *Bitmap = NULL;
    tTDAL_GFX_Palette       *BMPPalette = NULL;
    tTDAL_GFX_Palette       Palette;
    tTDAL_GFX_Color         PaletteColorTab[256], PaletteColor;
    uint16_t                width = 100, height = 50, i = 0;
    tTDAL_GFX_Point         offset;
    tTDAL_GFX_Size          size;
    tTDAL_GFX_ColorType     colorType;
    uint8_t                 Layer;
    tTDAL_DISP_LayerId      layersToCheck[] = { eTDAL_DISP_LAYER_GFX_ID_0, eTDAL_DISP_LAYER_GFX_ID_1, eTDAL_DISP_LAYER_GFX_ID_2 };
    uint8_t                 numberOfLayersToCheck = 3;
    tTDAL_DISP_LayerId      layerIndex;

    Palette.nbColors = 256;
    Palette.pColor   = &PaletteColorTab[0];

    /* DENC & AVS init/config */

    //TestManager_AssertEqual(TDAL_DISP_Init(),
                          //  eTDAL_DISP_NO_ERROR,
                           // "disp init" );
    TestManager_AssertEqual(TDAL_OUTPUT_Init(),
                            eTDAL_OUTPUT_NO_ERROR,
                            "output init" );

    pGFX_TestScenario_InitTest(GFX_TEST_MODE_576);

    TestManager_AssertEqual( TDAL_GFX_CapabilitiesGet( &Capabilities ), eTDAL_GFX_NO_ERROR, "GFX get capabilities" );

    for(layerIndex=0;layerIndex<numberOfLayersToCheck;layerIndex++)
    {
        Layer = pGFX_TestScenario_LayerIdxGet(layersToCheck[layerIndex]);

        if(Layer == 255)//255 is uint8_t version of -1
        {
            continue;
        }

        /* Set up the palette */
        TestHelper_GFX_FillPalette( &Palette );

        /* Set up default params & create a GFX region */
        Params.offset.x    = GFX_TEST_DEFAULT_X;
        Params.offset.y    = GFX_TEST_DEFAULT_Y;
        Params.size.width  = GFX_TEST_DEFAULT_WIDTH;
        Params.size.height = GFX_TEST_DEFAULT_HEIGHT;
        Params.refSize.width  = GFX_TEST_DEFAULT_WIDTH;
        Params.refSize.height = GFX_TEST_DEFAULT_HEIGHT;
        Params.colorType   = GFX_TEST_DEFAULT_COLOR_TYPE;
        Params.alpha       = kTDAL_GFX_OPAQUE;

        mTBOX_PRINT((kTBOX_LF, "region: x=%d y=%d w=%d h=%d\n",
                     Params.offset.x,
                     Params.offset.y,
                     Params.size.width,
                     Params.size.height));

        TestManager_AssertEqual( TDAL_GFX_RegionCreate( &GfxHandle, &Params ),
                                eTDAL_GFX_NO_ERROR, "Create a GFX region" );
        TestManager_AssertEqual(TDAL_GFX_RegionDestinationSet(GfxHandle,
                                                              gGFXLayerHandle[Layer]),
                                eTDAL_GFX_NO_ERROR,
                                "GFX region destination set" );

        TestManager_AssertEqual( TDAL_GFX_RegionPaletteSet( GfxHandle, &Palette ),  eTDAL_GFX_NO_ERROR, "Set the palette" );
        TestManager_AssertEqual( TDAL_GFX_RegionPaletteSet( GfxHandle, &Palette ),  eTDAL_GFX_NO_ERROR, "Set the palette 2nd" );

        /* reset region */
        PaletteColor.paletteEntry = 0;
            TestManager_AssertEqual( TDAL_GFX_RectangleDraw( GfxHandle, Params.offset, Params.size, Params.colorType, PaletteColor ),
                                eTDAL_GFX_NO_ERROR, "Draw a rectangle" );

        /* Draw 5 valid rectangles */
        offset.x    = Params.offset.x + 40;
        offset.y    = Params.offset.y + 40;
        size.width  = width;
        size.height = height;
        colorType   = Palette.colorType;

        for( i = 0; i < 5; i++ )
                {
            PaletteColor.paletteEntry = i + 1;

            mTBOX_PRINT((kTBOX_LF, "%d ARBG=[%d-%d-%d-%d] x=%d y=%d w=%d h=%d\n",
                         (i+1),
                         Palette.pColor[i + 1].ARGB8888.alpha,
                         Palette.pColor[i + 1].ARGB8888.R,
                         Palette.pColor[i + 1].ARGB8888.G,
                         Palette.pColor[i + 1].ARGB8888.B,
                         offset.x,
                         offset.y,
                         size.width,
                         size.height));

            TestManager_AssertEqual( TDAL_GFX_RectangleDraw( GfxHandle, offset, size, colorType, PaletteColor ),
                                    eTDAL_GFX_NO_ERROR, "Draw a rectangle" );
            offset.x += width;
            offset.y += height;
            }
        TestManager_AssertEqual( TDAL_GFX_RegionShow( GfxHandle ),
                                eTDAL_GFX_NO_ERROR, "Show region" );
        TestManager_AskForChecking( "do you see rectangle" );
        TestManager_AssertEqual( TDAL_GFX_RegionHide( GfxHandle ),
                                eTDAL_GFX_NO_ERROR, "Hide region" );

        /* reset region */
        PaletteColor.paletteEntry = 0;
        TestManager_AssertEqual( TDAL_GFX_RectangleDraw( GfxHandle, Params.offset, Params.size, Params.colorType, PaletteColor ),
                                eTDAL_GFX_NO_ERROR, "Draw a rectangle" );

        /* Draw 5 valid bitmaps */
        offset.x = Params.offset.x + 40;
        offset.y = Params.offset.y + 40;
        TestManager_AssertEqual( TestHelper_GFX_BmpLoad( (uint8_t *)Bitmap_Linux, &Bitmap, &BMPPalette),
                                eTDAL_GFX_NO_ERROR, "Load the bitmap" );
        TestManager_AssertEqual( TDAL_GFX_RegionPaletteSet( GfxHandle, BMPPalette ), eTDAL_GFX_NO_ERROR, "Set the palette" );
        for( i = 0; i < 5; i++ )
                    {
            mTBOX_PRINT((kTBOX_LF, "%d x=%d y=%d w=%d h=%d\n",
                         (i+1),
                         offset.x,
                         offset.y,
                         Bitmap->size.width,
                         Bitmap->size.height));
            TestManager_AssertEqual( TDAL_GFX_BmpDraw( GfxHandle, offset, Bitmap ),
                                    eTDAL_GFX_NO_ERROR, "Draw a bitmap" );
            offset.x += 20;
            offset.y += 20;
        }
        TestManager_AssertEqual( TDAL_GFX_RegionShow( GfxHandle ),
                                eTDAL_GFX_NO_ERROR, "Show region" );
        TestManager_AskForChecking( "do you see bitmap" );
        TestManager_AssertEqual( TDAL_GFX_RegionHide( GfxHandle ),
                                eTDAL_GFX_NO_ERROR, "Hide region" );

        TestManager_AskForChecking( "no bitmap displayed ?" );

        TestManager_AssertEqual( TDAL_GFX_RegionShow( GfxHandle ),
                                eTDAL_GFX_NO_ERROR, "Show region" );
        TestManager_AskForChecking( "do you see bitmap" );

        TestManager_AssertEqual( TDAL_GFX_RegionDelete( GfxHandle ),
                                eTDAL_GFX_NO_ERROR, "Delete the GFX region" );
    }
    TestManager_AssertEqual( TDAL_DISP_LayerEnable(eTDAL_DISP_LAYER_CURSOR_ID_0 ),
                            eTDAL_DISP_BAD_PARAMETER_ERROR, "Layer enable" );

    TestManager_AssertEqual( TDAL_DISP_LayerDisable( eTDAL_DISP_LAYER_CURSOR_ID_0),
                            eTDAL_DISP_BAD_PARAMETER_ERROR, "Layer disable" );

    TestManager_AssertEqual( TDAL_DISP_LayerEnable(eTDAL_DISP_LAYER_CURSOR_ID_0 ),
                            eTDAL_DISP_BAD_PARAMETER_ERROR, "Layer enable" );

    pGFX_TestScenario_TerminateTest();
}

/*! \fn void GFX_TestScenario_MultiRegionDraw( void )
 *  \brief Draw valid rectangle and bitmap in different regions
 *
 */
void GFX_TestScenario_MultiRegionDraw()
{
    tTDAL_GFX_Capabilities          Capabilities;
    tTDAL_GFX_RegionHandle  GfxHandle[2];
    tTDAL_GFX_RegionConfig  Params[2];
    tTDAL_GFX_Bitmap        *Bitmap = NULL;
    tTDAL_GFX_Palette       *BMPPalette = NULL;
    tTDAL_GFX_Palette       Palette;
    tTDAL_GFX_Color         PaletteColor[256], RectColor;
    uint16_t                x = 0, y = 0, width = 0, height = 0, i = 0;
    tTDAL_GFX_Point         offset;
    tTDAL_GFX_Size          size;
    tTDAL_GFX_ColorType     colorType;
    uint8_t                 Layer;
        
    Palette.nbColors = 256;
    Palette.pColor   = &PaletteColor[0];

    /* DENC & AVS init/config */
    pGFX_TestScenario_InitTest(GFX_TEST_MODE_576);
    
    Layer = pGFX_TestScenario_LayerIdxGet(eTDAL_DISP_LAYER_GFX_ID_0);

    TestManager_AssertEqual( TDAL_GFX_CapabilitiesGet( &Capabilities ), eTDAL_GFX_NO_ERROR, "GFX get capabilities" );
    
    /* Set up the palette */
    TestHelper_GFX_FillPalette( &Palette );
    
    /* Set up default params & create a GFX region */
    Params[0].offset.x    = GFX_TEST_DEFAULT_X;
    Params[0].offset.y    = GFX_TEST_DEFAULT_Y;
    Params[0].size.width  = GFX_TEST_DEFAULT_WIDTH/2 - 10; /* to be sure that no overlapping happen */
    Params[0].size.height = GFX_TEST_DEFAULT_HEIGHT/2 - 10;
    Params[0].refSize.width  = 720;
    Params[0].refSize.height = 576;
    Params[0].colorType   = Palette.colorType;
    Params[0].alpha       = kTDAL_GFX_OPAQUE;
    
    mTBOX_PRINT((kTBOX_LF, "1st region: x=%d y=%d w=%d h=%d\n",
                 Params[0].offset.x,
                 Params[0].offset.y,
                 Params[0].size.width,
                 Params[0].size.height));

    TestManager_AssertEqual( TDAL_GFX_RegionCreate( &GfxHandle[0], &Params[0] ),
                            eTDAL_GFX_NO_ERROR, "Create GFX region #1" );
    TestManager_AssertEqual(TDAL_GFX_RegionDestinationSet(GfxHandle[0],
                                                          gGFXLayerHandle[Layer]),
                            eTDAL_GFX_NO_ERROR,
                            "GFX region destination set" );

    Params[1].offset.x    = Params[0].size.width + 0 + 1;
    Params[1].offset.y    = Params[0].size.height + 0 + 1;
    Params[1].size.width  = GFX_TEST_DEFAULT_WIDTH/2;
    Params[1].size.height = GFX_TEST_DEFAULT_HEIGHT/2;
    Params[1].refSize.width  = 720;
    Params[1].refSize.height = 576;
    Params[1].colorType   = Palette.colorType;
    Params[1].alpha       = kTDAL_GFX_OPAQUE;
    
    mTBOX_PRINT((kTBOX_LF, "2nd region: x=%d y=%d w=%d h=%d\n",
                 Params[1].offset.x,
                 Params[1].offset.y,
                 Params[1].size.width,
                 Params[1].size.height));

    TestManager_AssertEqual( TDAL_GFX_RegionCreate( &GfxHandle[1], &Params[1] ),
                            eTDAL_GFX_NO_ERROR, "Create GFX region #2" );
    TestManager_AssertEqual(TDAL_GFX_RegionDestinationSet(GfxHandle[1],
                                                          gGFXLayerHandle[Layer]),
                            eTDAL_GFX_NO_ERROR,
                            "GFX region destination set" );
        
    mTBOX_PRINT((kTBOX_LF, "Pallete color type=%d nb=%d\n",
                 Palette.colorType,
                 Palette.nbColors));

    TestManager_AssertEqual( TDAL_GFX_RegionPaletteSet( GfxHandle[0], &Palette ),
                            eTDAL_GFX_NO_ERROR, "Set the palette for region #1" );
    TestManager_AssertEqual( TDAL_GFX_RegionPaletteSet( GfxHandle[1], &Palette ),
                            eTDAL_GFX_NO_ERROR, "Set the palette for region #2" );

    /* Show both regions */
    TestManager_AssertEqual( TDAL_GFX_RegionShow( GfxHandle[0] ), eTDAL_GFX_NO_ERROR, "Show region #1" );
    TestManager_AssertEqual( TDAL_GFX_RegionShow( GfxHandle[1] ), eTDAL_GFX_NO_ERROR, "Show region #2" );

    /* Draw rectangle for each region */
    for( i = 0; i < 2; i++ )
    {
        x = 10; /*Params[i].PositionX;*/
        y = 10; /*Params[i].PositionY;*/
        width  = Params[i].size.width - 10; /* region width size = 350 and not 360 */
        height = Params[i].size.height/2;

        RectColor.paletteEntry = i+1;
        
        mTBOX_PRINT((kTBOX_LF, "%d region: palette R=%d G=%d b=%d A=%d\n",
                     (i+1),
                     Palette.pColor[i+1].ARGB8888.R,
                     Palette.pColor[i+1].ARGB8888.G,
                     Palette.pColor[i+1].ARGB8888.B,
                     Palette.pColor[i+1].ARGB8888.alpha));

        offset.x    = x;
        offset.y    = y;
        size.width  = width;
        size.height = height;
        colorType   = Palette.colorType;
        
        mTBOX_PRINT((kTBOX_LF, "%d region: RectDraw x=%d y=%d w=%d h=%d\n",
                     (i+1),
                     offset.x,
                     offset.y,
                     size.width,
                     size.height));

        TestManager_AssertEqual( TDAL_GFX_RectangleDraw( GfxHandle[i], offset, size, colorType, RectColor ),
                                eTDAL_GFX_NO_ERROR, "Draw rectangle" );
    }
    TestManager_AskForChecking( "do you see 2 rectangles" );
    
    /* Draw rectangle for each region  with tranparent color */
    colorType              = Palette.colorType;
    RectColor.paletteEntry = 0;
    offset.x               = 0;
    offset.y               = 0;
    for( i = 0; i < 2; i++ )
    {
        mTBOX_PRINT((kTBOX_LF, "%d region: reset x=%d y=%d w=%d h=%d\n",
                     (i+1),
                     offset.x,
                     offset.y,
                     Params[i].size.width,
                     Params[i].size.height));

        TestManager_AssertEqual( TDAL_GFX_RectangleDraw( GfxHandle[i], offset, Params[i].size, colorType, RectColor ),
                                eTDAL_GFX_NO_ERROR, "Draw rectangle" );
    }

    /* Draw a bitmap for each region */
    TestManager_AssertEqual( TestHelper_GFX_BmpLoad( (uint8_t *)UnAirDeFamille, &Bitmap, &BMPPalette),
                            eTDAL_GFX_NO_ERROR, "Load the bitmap" );
    for( i = 0; i < 2; i++ )
    {
        x = 10; /*Params[i].PositionX;*/
        y = 10; /*Params[i].PositionY;*/
        offset.x = x;
        offset.y = y;
        
        /* fit in region configuration */
        #if 0
        Bitmap->size.width  = min(Bitmap->size.width,  (Params[i].size.width  - x));
        Bitmap->size.height = min(Bitmap->size.height, (Params[i].size.height - y));
        #endif
        
        mTBOX_PRINT((kTBOX_LF, "%d region: BmpDraw x=%d y=%d w=%d h=%d\n",
                     (i+1),
                     offset.x,
                     offset.y,
                     Bitmap->size.width,
                     Bitmap->size.height));
        
        TestManager_AssertEqual( TDAL_GFX_RegionPaletteSet( GfxHandle[i], BMPPalette ),
                                eTDAL_GFX_NO_ERROR, "Set the palette" );
        TestManager_AssertEqual( TDAL_GFX_BmpDraw( GfxHandle[i], offset, Bitmap ),
                                eTDAL_GFX_NO_ERROR, "Draw a bitmap" );
    }
    TestManager_AskForChecking( "do you see 2 bitmaps" );

    /* Terminate */
    TestManager_AssertEqual( TDAL_GFX_RegionHide( GfxHandle[0] ), eTDAL_GFX_NO_ERROR, "Hide region #1" );
    TestManager_AssertEqual( TDAL_GFX_RegionHide( GfxHandle[1] ), eTDAL_GFX_NO_ERROR, "Hide region #2" );
    TestManager_AssertEqual( TDAL_GFX_RegionDelete( GfxHandle[0] ), eTDAL_GFX_NO_ERROR, "Delete region #1" );
    TestManager_AssertEqual( TDAL_GFX_RegionDelete( GfxHandle[1] ), eTDAL_GFX_NO_ERROR, "Delete region #2" );
    pGFX_TestScenario_TerminateTest();
}

/*! \fn void GFX_TestScenario_MultiRegionDraw( void )
 *  \brief Mix color systems
 *
 */
void GFX_TestScenario_ColorSystemDraw(void)

{
    tTDAL_GFX_Capabilities          Capabilities;
    tTDAL_GFX_RegionHandle  GfxHandle[4];
    tTDAL_GFX_RegionConfig  Params;
    tTDAL_GFX_Bitmap        Bitmap;
    tTDAL_GFX_Palette       Palette;
    tTDAL_GFX_Color         PaletteColorTab[256], Color;
    tTDAL_GFX_Point         offset;
    tTDAL_GFX_Size          size;
    tTDAL_GFX_ColorType     colorType;
    uint8_t                 indexRegion;
    uint8_t*                BitmapBuff=NULL;
    uint8_t                 Layer;
    

    /* DENC & AVS init/config */
    pGFX_TestScenario_InitTest(GFX_TEST_MODE_576);
    
    Layer = pGFX_TestScenario_LayerIdxGet(eTDAL_DISP_LAYER_GFX_ID_0);
    
    TestManager_AssertEqual( TDAL_GFX_CapabilitiesGet( &Capabilities ), eTDAL_GFX_NO_ERROR, "GFX get capabilities" );
    
    /* Set up the palette */
    Palette.nbColors = 256;
    Palette.pColor   = &PaletteColorTab[0];
    TestHelper_GFX_FillPalette( &Palette );
    
    mTBOX_PRINT((kTBOX_LF, " Create 2 regions in different color system :\n"
                            "     ----------         \n"
                            "     |        |         \n"
                            "     |   A    |         \n"
                            "     |        |         \n"
                            "     -------------------\n"
                            "              |        |\n"
                            "              |   B    |\n"
                            "              |        |\n"
                            "              ----------\n"
                            "\n"
                            "A : CLUT_ARGB888\n"
                            "B : TRUECOLOR_ARGB888\n\n"));
                            
    mTBOX_PRINT((kTBOX_LF, " For each region, draw 4 Bitmap/Rectangle in different color system :\n"
                                "     -------------------\n"
                                "     |        |        |\n"
                                "     |   A'   |   B'   |\n"
                                "     |        |        |\n"
                                "     -------------------\n"
                                "     |        |        |\n"
                                "     |   C'   |   D'   |\n"
                                "     |        |        |\n"
                                "     -------------------\n"
                                "\n"
                                "A' : RECTANGLE CLUT_ARGB888\n"
                                "B' : RECTANGLE TRUECOLOR_ARGB888\n"
                                "C' : BITMAP CLUT_ARGB888\n"
                                "D' : BITMAP TRUECOLOR_ARGB888\n\n"));
                                
    
    mTBOX_PRINT((kTBOX_LF, "NOTE THAT     A-B' , A-D' , B-A' , B-C'     ARE OPTIONAL\n\n"));
    
    for ( indexRegion=0 ; indexRegion<2 ; indexRegion++)
    {
        switch(indexRegion)
        {
            case 0 :
            {
                /*--- REGION A : CLUT_ARGB888 ---*/
                Params.offset.x    = 100;
                Params.offset.y    = 100;
                Params.size.width  = 200;
                Params.size.height = 200;
                Params.refSize.width  = 720;
                Params.refSize.height = 576;
                Params.colorType   = eTDAL_GFX_COLOR_CLUT_ARGB8888;
                Params.alpha       = kTDAL_GFX_OPAQUE;
                mTBOX_PRINT((kTBOX_LF, "REGION A : CLUT_ARGB888 : x=%d y=%d w=%d h=%d\n",    Params.offset.x,
                                                                                             Params.offset.y,
                                                                                             Params.size.width,
                                                                                             Params.size.height));
                TestManager_AssertEqual( TDAL_GFX_RegionCreate( &(GfxHandle[indexRegion]), &Params ),eTDAL_GFX_NO_ERROR, "Create the region" );
                TestManager_AssertEqual(TDAL_GFX_RegionDestinationSet(GfxHandle[indexRegion],gGFXLayerHandle[Layer]),eTDAL_GFX_NO_ERROR, "GFX region destination set" );
                TestManager_AssertEqual( TDAL_GFX_RegionPaletteSet( GfxHandle[indexRegion], &Palette ), eTDAL_GFX_NO_ERROR, "Set the palette" );
                TestManager_AssertEqual( TDAL_GFX_RegionShow( GfxHandle[indexRegion] ),eTDAL_GFX_NO_ERROR, "Show the region" );
                break;
            }
            case 1 :
            {

                /*--- REGION D : TRUECOLOR_ARGB888 ---*/
                Params.offset.x    = 300;
                Params.offset.y    = 300;
                Params.size.width  = 200;
                Params.size.height = 200;
                Params.refSize.width  = 720;
                Params.refSize.height = 576;
                Params.colorType   = eTDAL_GFX_COLOR_TRUE_COLOR_ARGB8888;
                Params.alpha       = kTDAL_GFX_OPAQUE;
                mTBOX_PRINT((kTBOX_LF, "REGION D : TRUECOLOR_ARGB888 : x=%d y=%d w=%d h=%d\n",    Params.offset.x,
                                                                                             Params.offset.y,
                                                                                             Params.size.width,
                                                                                             Params.size.height));
                TestManager_AssertEqual( TDAL_GFX_RegionCreate( &(GfxHandle[indexRegion]), &Params ),eTDAL_GFX_NO_ERROR, "Create the region" );
                TestManager_AssertEqual(TDAL_GFX_RegionDestinationSet(GfxHandle[indexRegion],gGFXLayerHandle[Layer]),eTDAL_GFX_NO_ERROR, "GFX region destination set" );
                TestManager_AssertEqual( TDAL_GFX_RegionPaletteSet( GfxHandle[indexRegion], &Palette ), eTDAL_GFX_NO_ERROR, "Set the palette" );
				TestManager_AssertEqual( TDAL_GFX_RegionShow( GfxHandle[indexRegion] ),eTDAL_GFX_NO_ERROR, "Show the region" );
                break;
            }
        }

        
        /*------------- A' ---------------*/
        offset.x                    = 0;
        offset.y                    = 0;
        size.width                  = 100;
        size.height                 = 100;
        colorType                   = eTDAL_GFX_COLOR_CLUT_ARGB8888;
        Color.paletteEntry          = 1;
        TestManager_AssertEqual( TDAL_GFX_RectangleDraw( GfxHandle[indexRegion], offset, size, colorType, Color ),eTDAL_GFX_NO_ERROR, "Draw A'" );


        /*------------- B' ---------------*/
        offset.x                    = 100;
        offset.y                    = 0;
        size.width                  = 100;
        size.height                 = 100;
        colorType                   = eTDAL_GFX_COLOR_TRUE_COLOR_ARGB8888;
        Color.ARGB8888.alpha        = kTDAL_GFX_OPAQUE;
        Color.ARGB8888.R            = 255;
        Color.ARGB8888.G            = 255;
        Color.ARGB8888.B            = 0;
        TestManager_AssertEqual( TDAL_GFX_RectangleDraw( GfxHandle[indexRegion], offset, size, colorType, Color ),eTDAL_GFX_NO_ERROR, "Draw B'" );

        /*------------- C' ---------------*/
        TKEL_Malloc(100*100*1, (void **)&BitmapBuff);
        if(BitmapBuff != NULL)
        {
            uint8_t i;
            for (i=0; i<100; i++)
            {
                memset( &(BitmapBuff[i*100]), (i+1) , 100 );
            }
            
            offset.x                    = 0;
            offset.y                    = 100;
            Bitmap.colorType            = eTDAL_GFX_COLOR_CLUT_ARGB8888;
            Bitmap.size.width           = 100;
            Bitmap.size.height          = 100;
            Bitmap.pData                = BitmapBuff;
            
            TestManager_AssertEqual( TDAL_GFX_BmpDraw( GfxHandle[indexRegion], offset, &Bitmap ),eTDAL_GFX_NO_ERROR, "Draw C'" );

            TKEL_Free(BitmapBuff);
            BitmapBuff=NULL;
        }


        /*------------- D' ---------------*/
        TKEL_Malloc(100*100*4, (void **)&BitmapBuff);
        if(BitmapBuff != NULL)
        {
            uint8_t i,j;
            for (i=0; i<100; i++)
            {
                for (j=0; j<100; j++)
                {
                    BitmapBuff[((i*100 + j)*4) + 0 ] = (2*j);       /*B*/
                    BitmapBuff[((i*100 + j)*4) + 1 ] = (220-(2*i));     /*G*/
                    BitmapBuff[((i*100 + j)*4) + 2 ] = (2*i);       /*R*/
                    BitmapBuff[((i*100 + j)*4) + 3 ] = 255;     /*A*/
                }
            }
            
            offset.x                    = 100;
            offset.y                    = 100;
            Bitmap.colorType            = eTDAL_GFX_COLOR_TRUE_COLOR_ARGB8888;
            Bitmap.size.width           = 100;
            Bitmap.size.height          = 100;
            Bitmap.pData                = BitmapBuff;
            
            TestManager_AssertEqual( TDAL_GFX_BmpDraw( GfxHandle[indexRegion], offset, &Bitmap ),eTDAL_GFX_NO_ERROR, "Draw D'" );

            TKEL_Free(BitmapBuff);
            BitmapBuff=NULL;
        }


    }
    
    TestManager_AskForChecking( "do you see every mandatory drawings" );
   
    TestManager_AssertEqual( TDAL_GFX_RegionHide( GfxHandle[0] ),eTDAL_GFX_NO_ERROR, "Hide region A" );

    TestManager_AskForChecking( "do you see that region A is hidden" );

    TestManager_AssertEqual( TDAL_GFX_RegionHide( GfxHandle[1] ),eTDAL_GFX_NO_ERROR, "Hide region B" );

    TestManager_AskForChecking( "do you see that region B is hidden" );
    
    TestManager_AssertEqual( TDAL_GFX_RegionDelete( GfxHandle[0] ),eTDAL_GFX_NO_ERROR, "Delete the GFX region" );
    TestManager_AssertEqual( TDAL_GFX_RegionDelete( GfxHandle[1] ),eTDAL_GFX_NO_ERROR, "Delete the GFX region" );
    
    pGFX_TestScenario_TerminateTest();
}

/*! \fn void GFX_TestScenario_Transparency( void )
 *  \brief Test the global transparency over a
 *  fullscreen palette.
 */
void GFX_TestScenario_Transparency()
{
    tTDAL_GFX_Capabilities          Capabilities;
    tTDAL_GFX_RegionHandle  GfxHandle;
    tTDAL_GFX_RegionConfig  Params;
    tTDAL_GFX_Palette       Palette;
    tTDAL_GFX_Color         PaletteColor[256], RectColor;
    uint16_t                width = 45, height = 36, i = 0;
#if 0
	uint16_t uXCounter = 0, uYCounter = 0;
#endif
    tTDAL_GFX_Point         offset;
    tTDAL_GFX_Size          size;
    tTDAL_GFX_ColorType     colorType;
    uint8_t                 Layer,color;
    
    Palette.nbColors = 256;
    Palette.pColor   = &PaletteColor[0];

    /* DENC & AVS init/config */
    pGFX_TestScenario_InitTest(GFX_TEST_MODE_576);  
    
    Layer = pGFX_TestScenario_LayerIdxGet(eTDAL_DISP_LAYER_GFX_ID_0);
    
    TestManager_AssertEqual( TDAL_GFX_CapabilitiesGet( &Capabilities ), eTDAL_GFX_NO_ERROR, "GFX get capabilities" );
    
    /* Set up the palette */
    TestHelper_GFX_FillPalette( &Palette );
    
    /* Set up default params & create a GFX region */
    Params.offset.x    = GFX_TEST_DEFAULT_X;
    Params.offset.y    = GFX_TEST_DEFAULT_Y;
    Params.size.width  = GFX_TEST_DEFAULT_WIDTH;
    Params.size.height = GFX_TEST_DEFAULT_HEIGHT;
    Params.refSize.width  = GFX_TEST_DEFAULT_WIDTH;
    Params.refSize.height = GFX_TEST_DEFAULT_HEIGHT;
    Params.colorType   = Palette.colorType;
    Params.alpha       = kTDAL_GFX_OPAQUE;

    TestManager_AssertEqual( TDAL_GFX_RegionCreate( &GfxHandle, &Params ),
                            eTDAL_GFX_NO_ERROR, "Create a GFX region" );
    TestManager_AssertEqual(TDAL_GFX_RegionDestinationSet(GfxHandle,
                                                          gGFXLayerHandle[Layer]),
                            eTDAL_GFX_NO_ERROR,
                            "GFX region destination set" );
    
    TestManager_AssertEqual( TDAL_GFX_RegionPaletteSet( GfxHandle, &Palette ), eTDAL_GFX_NO_ERROR, "Set the palette" );

    /* Draw a rectangle */
    RectColor.paletteEntry = 0;
    color = 0;
    colorType              = Palette.colorType;
    size.width             = width;
    size.height            = height;
#if 0
    for (uYCounter = 0; uYCounter < 16; uYCounter++)
    {
        for (uXCounter = 0; uXCounter < 16; uXCounter++)
        {
            offset.x = (uXCounter*45);
            offset.y = (uYCounter*36);

            TDAL_GFX_RectangleDraw ( GfxHandle, offset, size, colorType, RectColor );
            color++;
            RectColor.paletteEntry = color;
            
        }
    }
#else
    RectColor.paletteEntry = 0;
    offset.x = 0;
    offset.y = 0;
    size.width             = 100;
    size.height            = 100;
    TDAL_GFX_RectangleDraw ( GfxHandle, offset, size, colorType, RectColor );
    
    RectColor.paletteEntry = 1;
    offset.x = 100;
    TDAL_GFX_RectangleDraw ( GfxHandle, offset, size, colorType, RectColor );
    
    RectColor.paletteEntry = 2;
    offset.x = 200;
    TDAL_GFX_RectangleDraw ( GfxHandle, offset, size, colorType, RectColor );
    
    RectColor.paletteEntry = 3;
    offset.x = 300;
    TDAL_GFX_RectangleDraw ( GfxHandle, offset, size, colorType, RectColor );
#endif
    TestManager_AssertEqual( TDAL_GFX_RegionShow( GfxHandle ),
                            eTDAL_GFX_NO_ERROR, "Show region" );

    TestManager_AssertEqual( TDAL_GFX_RegionTransparencySet( GfxHandle, 0xFF),
                            eTDAL_GFX_NO_ERROR, "set transparency opaque" );
    TestManager_AskForChecking( "Is rectangle visible" );
    /* Make transparency change */
    for( i = 1; i < 9; i++ )
    {
        mTBOX_PRINT((kTBOX_LF, "Transparency = %d\n", (32 * i)));

        TestManager_AssertEqual( TDAL_GFX_RegionTransparencySet( GfxHandle, 32 * i ),
                                eTDAL_GFX_NO_ERROR, "change transparency" );
        //TestManager_AskForChecking( "Have you seen a transparency change" );
       
        TDAL_DISP_LayerGlobalTransparencySet(eTDAL_DISP_LAYER_GFX_ID_0, (32 * i) );
       
      
        
    }
    TestManager_AssertEqual( TDAL_GFX_RegionTransparencySet( GfxHandle, 255 ),
                            eTDAL_GFX_NO_ERROR, "set transparency opaque" );
    TestManager_AskForChecking( "Is rectangle visible" );

    /* Terminate */
    TestManager_AssertEqual( TDAL_GFX_RegionHide( GfxHandle ), eTDAL_GFX_NO_ERROR, "Hide region" );
    TestManager_AssertEqual( TDAL_GFX_RegionDelete( GfxHandle ), eTDAL_GFX_NO_ERROR, "Delete region" );
    pGFX_TestScenario_TerminateTest();
}

/*! \fn void  GFX_TestScenarioMemcpy( void )
 *  \brief Test TDAL_GFX_MemcpyNonZero function
 *
 *  This tests creates a first buffer filled with 0x01 pattern.
 *  It creates a second buffer filled with couple of 0x00 and 0x02.
 *  It uses MemcpyNonZero with first buffer as dest and second one
 *  as source. After that destination, so the first buffer, is filled
 *  with couple of 0x01 and 0x02.
 */
void  GFX_TestScenario_Memcpy()
{
    const uint16_t  Size = 256;
    uint8_t         Buffer1[256], Buffer2[256];
    int             i = 0;
    tTDAL_GFX_Size      destSize;
    tTDAL_GFX_Point     srcOffset;
    tTDAL_GFX_Size      srcSize;
    tTDAL_GFX_ColorType colorType;

    destSize.width  = Size;
    destSize.height = 1;
    srcSize         = destSize;
    srcOffset.x     = 0;
    srcOffset.y     = 0;
    colorType       = eTDAL_GFX_COLOR_CLUT_ARGB8888;
    
    bool        copy_ok = true;

    /* Fill Buffer1 and Buffer2 */
    for( i = 0; i < Size; i++ )
    {
        Buffer1[i] = 0x01;
        if( i % 2 == 0 )
        {
            Buffer2[i] = 0x00;
        }
        else
        {
            Buffer2[i] = 0x02;
        }
    }
    /* Copy buffers and test the result */
    TDAL_GFX_Copy2DNonZero(colorType,
                           Buffer1,
                           destSize,
                           Buffer2,
                           srcOffset,
                           srcSize);
    for( i = 0; i < Size; i++ )
    {
        if( i % 2 != 0 )
        {
            if( Buffer1[i] != 0x02 )
            {
                TestManager_LogErrorMessage( "MemcpyNonZero did not return the expected buffer" );
                copy_ok = false;
            }
        }
    }
    if (copy_ok == true)
    {
        TestManager_AssertEqual( eTDAL_GFX_NO_ERROR, eTDAL_GFX_NO_ERROR, "MemcpyNonZero OK" );
    }
    else
    {
        TestManager_AssertEqual( eTDAL_GFX_UNKNOWN_ERROR, eTDAL_GFX_NO_ERROR, "MemcpyNonZero KO" );
    }
}

void GFX_TestScenario_LayerSize576(void)
{
    pGFX_TestScenario_LayerSize(GFX_TEST_MODE_576);
}

void GFX_TestScenario_LayerSize720(void)
{
	if(GFX_TEST_720_HEIGHT <= GFX_TEST_DEFAULT_HEIGHT)
	{
		pGFX_TestScenario_LayerSize(GFX_TEST_MODE_720);
	}
}

void GFX_TestScenario_LayerSize1080(void)
{
	if(GFX_TEST_1080_HEIGHT <= GFX_TEST_DEFAULT_HEIGHT)
	{
		pGFX_TestScenario_LayerSize(GFX_TEST_MODE_1080);
	}
}


static void pGFX_TestScenario_LayerSize(uint8_t mode)
{
    tTDAL_GFX_Capabilities  Capabilities;
    tTDAL_GFX_RegionHandle  GfxHandle, GfxHandle_1;
    tTDAL_GFX_RegionConfig  Params;
    tTDAL_GFX_Bitmap        *Bitmap = NULL;
       tTDAL_GFX_Palette        *BMPPalette = NULL;
       tTDAL_GFX_Palette        Palette;
    tTDAL_GFX_Color         PaletteColor[256];
    tTestManagerChrono      chrono;
    long                    elapsed_time = 0;
    tTDAL_GFX_Point         offset;
    tTDAL_DISP_LayerWindow  stInputWindow,  pstInputWindow;
    tTDAL_DISP_LayerWindow  stOutputWindow, pstOutputWindow[4];
    uint8_t                 Layer;
    
    Palette.nbColors  = 256;
    Palette.pColor    = &PaletteColor[0];

    pGFX_TestScenario_InitTest(mode);
    
    Layer = pGFX_TestScenario_LayerIdxGet(eTDAL_DISP_LAYER_GFX_ID_0);

    TestManager_AssertEqual( TDAL_GFX_CapabilitiesGet( &Capabilities ), eTDAL_GFX_NO_ERROR, "GFX get capabilities" );
    
    /* Set up the palette */
    TestHelper_GFX_FillPalette( &Palette );
    
    /* Set up default params & create a GFX region */
    switch(mode)
    {
    case GFX_TEST_MODE_720:
        //printf("\n GFX_TEST_MODE_720 \n");
        Params.offset.x    = GFX_TEST_DEFAULT_X;
        Params.offset.y    = GFX_TEST_DEFAULT_Y;
        Params.size.width  = GFX_TEST_720_WIDTH;
        Params.size.height = GFX_TEST_720_HEIGHT;
        Params.refSize.width  = GFX_TEST_720_WIDTH;
        Params.refSize.height = GFX_TEST_720_HEIGHT;
        pstInputWindow.Left   = 0;
        pstInputWindow.Top    = 0;
        pstInputWindow.Right  = 1280 - 1;
        pstInputWindow.Bottom = 720 - 1;
        break;
    case GFX_TEST_MODE_1080:
        //printf("\n GFX_TEST_MODE_1080 \n");
        Params.offset.x    = GFX_TEST_DEFAULT_X;
        Params.offset.y    = GFX_TEST_DEFAULT_Y;
        Params.size.width  = GFX_TEST_1080_WIDTH;
        Params.size.height = GFX_TEST_1080_HEIGHT;
        Params.refSize.width  = GFX_TEST_1080_WIDTH;
        Params.refSize.height = GFX_TEST_1080_HEIGHT;
        pstInputWindow.Left   = 0;
        pstInputWindow.Top    = 0;
        pstInputWindow.Right  = 1920 - 1;
        pstInputWindow.Bottom = 1080 - 1;
        break;
    case GFX_TEST_MODE_576:
    default:
        //printf("\n GFX_TEST_MODE_576 \n");
        Params.offset.x    = GFX_TEST_DEFAULT_X;
        Params.offset.y    = GFX_TEST_DEFAULT_Y;
        Params.size.width  = GFX_TEST_DEFAULT_WIDTH;
        Params.size.height = GFX_TEST_DEFAULT_HEIGHT;
        Params.refSize.width  = GFX_TEST_DEFAULT_WIDTH;
        Params.refSize.height = GFX_TEST_DEFAULT_HEIGHT;
        pstInputWindow.Left   = 0;
        pstInputWindow.Top    = 0;
        pstInputWindow.Right  = 720 - 1;
        pstInputWindow.Bottom = 576 - 1;
        break;
    }
    Params.colorType   = GFX_TEST_DEFAULT_COLOR_TYPE;
    Params.alpha       = kTDAL_GFX_OPAQUE;

    /* DISP_LAYER_GFX_ID_0 */
       TestManager_AssertEqual(TDAL_DISP_LayerEnable(eTDAL_DISP_LAYER_GFX_ID_0),
                            eTDAL_DISP_NO_ERROR,
                            " disp layer GFX 0 enable" );
    
    TestManager_AssertEqual( TDAL_GFX_RegionCreate( &GfxHandle, &Params ),
                            eTDAL_GFX_NO_ERROR, "Create a GFX region" );

       TestManager_AssertEqual(TDAL_GFX_RegionDestinationSet(GfxHandle,
                                gGFXLayerHandle[Layer]),
                            eTDAL_GFX_NO_ERROR,
                            "TDAL_GFX_RegionDestinationSet" );
    

    TestManager_AssertEqual( TDAL_GFX_RegionPaletteSet( GfxHandle, &Palette ),
                             eTDAL_GFX_NO_ERROR, "Set the palette" );


    /*! Draw the default bitmap
     *  in next revision, should add a fullscreen bitmap
     *  but issue for the moment to generate such a bitmap
     */
    TestManager_AssertEqual( TestHelper_GFX_BmpLoad( (uint8_t *)Bitmap_Linux, &Bitmap, &BMPPalette),
                            eTDAL_GFX_NO_ERROR, "Load bitmap" );
    TestManager_AssertEqual( TDAL_GFX_RegionPaletteSet( GfxHandle, BMPPalette ), eTDAL_GFX_NO_ERROR, "Set the palette" );
    TestManager_AssertEqual( TDAL_GFX_RegionShow( GfxHandle ), eTDAL_GFX_NO_ERROR, "Show region" );

    offset.x = 50;
    offset.y = 50;

    TestManager_StartChrono( &chrono );
    TestManager_AssertEqual( TDAL_GFX_BmpDraw( GfxHandle, offset, Bitmap ),
                            eTDAL_GFX_NO_ERROR, "Draw a bitmap in the region" );
    TestManager_StopChrono( &chrono, &elapsed_time );
    mTBOX_PRINT((kTBOX_LF, "Time elapsed for drawing a fullscreen bitmap: %ld ms\n", elapsed_time));
    TestManager_AskForChecking( "do you see a windows bitmap :on layer 0" );

    TestManager_AssertEqual(TDAL_DISP_LayerIOWindowsGet(eTDAL_DISP_LAYER_GFX_ID_0,
                                                        &stInputWindow,
                                                        &stOutputWindow),
                            eTDAL_DISP_NO_ERROR,
                            "GFX get IOWindows" );
    
    mTBOX_PRINT((kTBOX_LF, "Layer IO Get In[%d %d %d %d] Out[%d %d %d %d]\n",
                 stInputWindow.Left,
                 stInputWindow.Top,
                 stInputWindow.Right,
                 stInputWindow.Bottom,
                 stOutputWindow.Left,
                 stOutputWindow.Top,
                 stOutputWindow.Right,
                 stOutputWindow.Bottom));


    pstOutputWindow[Layer].Left   = stOutputWindow.Left;
    pstOutputWindow[Layer].Top    = stOutputWindow.Top;
    pstOutputWindow[Layer].Right  = stOutputWindow.Right;
    pstOutputWindow[Layer].Bottom = stOutputWindow.Bottom;
    stInputWindow.Left            = pstInputWindow.Left;
    stInputWindow.Top             = pstInputWindow.Top;
    stInputWindow.Right           = pstInputWindow.Right;
    stInputWindow.Bottom          = pstInputWindow.Bottom;
    
    TestManager_AssertEqual(TDAL_DISP_LayerIOWindowsSet(eTDAL_DISP_LAYER_GFX_ID_0,
                                                        &stInputWindow,
                                                        &stOutputWindow),
                            eTDAL_DISP_NO_ERROR,
                            "GFX first set BMP image IOWindows" );

    TestManager_AskForChecking( "the bmp picture should be no changing" );

    stOutputWindow.Right  /= 2;
    stOutputWindow.Bottom /= 2;

    TestManager_AssertEqual( TDAL_GFX_RegionHide( GfxHandle ), eTDAL_GFX_NO_ERROR, "Hide region" );


    TestManager_AssertEqual(TDAL_DISP_LayerIOWindowsSet(eTDAL_DISP_LAYER_GFX_ID_0,
                                                        &stInputWindow,
                                                        &stOutputWindow),
                            eTDAL_DISP_NO_ERROR,
                            "GFX first set BMP image IOWindows" );


    TestManager_AssertEqual( TDAL_GFX_BmpDraw( GfxHandle, offset, Bitmap ),
                            eTDAL_GFX_NO_ERROR, "Draw a bitmap in the region" );
                            
    TestManager_AssertEqual( TDAL_GFX_RegionShow( GfxHandle ), eTDAL_GFX_NO_ERROR, "Show region" );

    TestManager_AskForChecking( "the bmp picture should be 1/4 of last size" );

    stOutputWindow.Right  *= 2; 
    stOutputWindow.Bottom *= 2;

    TestManager_AssertEqual( TDAL_GFX_RegionHide( GfxHandle ), eTDAL_GFX_NO_ERROR, "Hide region" );

    TestManager_AssertEqual(TDAL_DISP_LayerIOWindowsSet(eTDAL_DISP_LAYER_GFX_ID_0,
                                                        &stInputWindow,
                                                        &stOutputWindow),
                            eTDAL_DISP_NO_ERROR,
                            "GFX first set BMP image IOWindows" );

    TestManager_AssertEqual( TDAL_GFX_BmpDraw( GfxHandle, offset, Bitmap ),
                            eTDAL_GFX_NO_ERROR, "Draw a bitmap in the region" );

    TestManager_AssertEqual( TDAL_GFX_RegionShow( GfxHandle ), eTDAL_GFX_NO_ERROR, "Show region" );

    TestManager_AskForChecking( "the bmp picture should be upscaled 4 times than last size" );

       TestManager_AssertEqual(TDAL_DISP_LayerDisable(eTDAL_DISP_LAYER_GFX_ID_0),
                            eTDAL_DISP_NO_ERROR,
                            " disp GFX layer 0 disable" );
    
    /* DISP_LAYER_GFX_ID_1 */
    if((Layer = pGFX_TestScenario_LayerIdxGet(eTDAL_DISP_LAYER_GFX_ID_1)) != (uint8_t)(-1))
    {
        TestManager_AssertEqual( TDAL_GFX_RegionCreate( &GfxHandle_1, &Params ),
                                 eTDAL_GFX_NO_ERROR, "Create a second GFX region" );
        TestManager_AssertEqual(TDAL_GFX_RegionDestinationSet(GfxHandle_1,
                                                              gGFXLayerHandle[Layer]),
                            eTDAL_GFX_NO_ERROR,
                            "TDAL_GFX_RegionDestinationSet" );
        
        TestManager_AssertEqual( TDAL_GFX_RegionPaletteSet( GfxHandle_1, &Palette ),
                                 eTDAL_GFX_NO_ERROR, "Set the palette" );
        
        TestManager_AssertEqual(TDAL_DISP_LayerEnable(eTDAL_DISP_LAYER_GFX_ID_1),
                                eTDAL_DISP_NO_ERROR,
                                " disp GFX layer 1 enable" );
        /*! Draw the default bitmap
        *  in next revision, should add a fullscreen bitmap
        *  but issue for the moment to generate such a bitmap
        */
        TestManager_AssertEqual( TDAL_GFX_RegionPaletteSet( GfxHandle_1, BMPPalette ), eTDAL_GFX_NO_ERROR, "Set the palette" );
        TestManager_AssertEqual( TDAL_GFX_RegionShow( GfxHandle_1 ), eTDAL_GFX_NO_ERROR, "Show region" );

        
        offset.x = 50;
        offset.y = 50;
        
        TestManager_StartChrono( &chrono );
        TestManager_AssertEqual( TDAL_GFX_BmpDraw( GfxHandle_1, offset, Bitmap ),
                                 eTDAL_GFX_NO_ERROR, "Draw a bitmap in the region 1" );
        TestManager_StopChrono( &chrono, &elapsed_time );
        mTBOX_PRINT((kTBOX_LF, "Time elapsed for drawing a fullscreen bitmap: %ld ms\n", elapsed_time));
        TestManager_AskForChecking( "do you see a windows bitmap :on layer 1" );

        TestManager_AssertEqual(TDAL_DISP_LayerIOWindowsGet(eTDAL_DISP_LAYER_GFX_ID_1,
                                                            &stInputWindow,
                                                            &stOutputWindow),
                                eTDAL_DISP_NO_ERROR,
                                "GFX get IOWindows" );
        
        /* DISP_LAYER_GFX_ID_1 save */
        pstOutputWindow[Layer].Left   = stOutputWindow.Left;
        pstOutputWindow[Layer].Top    = stOutputWindow.Top;
        pstOutputWindow[Layer].Right  = stOutputWindow.Right;
        pstOutputWindow[Layer].Bottom = stOutputWindow.Bottom;
        
        TestManager_AssertEqual(TDAL_DISP_LayerIOWindowsSet(eTDAL_DISP_LAYER_GFX_ID_1,
                                                            &stInputWindow,
                                                            &stOutputWindow),
                                eTDAL_DISP_NO_ERROR,
                                "GFX first set BMP image IOWindows" );

        TestManager_AskForChecking( "GFX layer 1:the bmp picture should be no changing" );

        TestManager_AssertEqual( TDAL_GFX_RegionHide( GfxHandle_1 ), eTDAL_GFX_NO_ERROR, "Hide Region" );


        stOutputWindow.Right  /= 2;
        stOutputWindow.Bottom /= 2;


        TestManager_AssertEqual(TDAL_DISP_LayerIOWindowsSet(eTDAL_DISP_LAYER_GFX_ID_1,
                                                            &stInputWindow,
                                                            &stOutputWindow),
                                eTDAL_DISP_NO_ERROR,
                                "GFX first set BMP image IOWindows" );

    TestManager_AssertEqual( TDAL_GFX_BmpDraw( GfxHandle_1, offset, Bitmap ),
                            eTDAL_GFX_NO_ERROR, "Draw a bitmap in the region" );

    TestManager_AssertEqual( TDAL_GFX_RegionShow( GfxHandle_1 ), eTDAL_GFX_NO_ERROR, "Show region" );
        
    TestManager_AskForChecking( "GFX layer 1: the bmp picture should be 1/4 of last size" );

    TestManager_AssertEqual( TDAL_GFX_RegionHide( GfxHandle_1 ), eTDAL_GFX_NO_ERROR, "Hide region" );
        
    stOutputWindow.Right  *= 2; 
        stOutputWindow.Bottom *= 2;
        
        TestManager_AssertEqual(TDAL_DISP_LayerIOWindowsSet(eTDAL_DISP_LAYER_GFX_ID_1,
                                                            &stInputWindow,
                                                            &stOutputWindow),
                                eTDAL_DISP_NO_ERROR,
                                "GFX first set BMP image IOWindows" );

    TestManager_AssertEqual( TDAL_GFX_BmpDraw( GfxHandle_1, offset, Bitmap ),
                            eTDAL_GFX_NO_ERROR, "Draw a bitmap in the region" );

    TestManager_AssertEqual( TDAL_GFX_RegionShow( GfxHandle_1 ), eTDAL_GFX_NO_ERROR, "Show region" );
        
    TestManager_AskForChecking( "GFX layer 1: the bmp picture should be upscaled 4 times than last size" );

        /* disp two layers at the same time */
       TestManager_AssertEqual(TDAL_DISP_LayerEnable(eTDAL_DISP_LAYER_GFX_ID_0),
                               eTDAL_DISP_NO_ERROR,
                               " disp GFX layer 0 enable" );
       TestManager_AskForChecking( "two layers are all visable?" );
    
       /* restore GFX_ID_0 output */
       Layer = pGFX_TestScenario_LayerIdxGet(eTDAL_DISP_LAYER_GFX_ID_0);
       stOutputWindow.Left   = pstOutputWindow[Layer].Left;
       stOutputWindow.Top    = pstOutputWindow[Layer].Top;
       stOutputWindow.Right  = pstOutputWindow[Layer].Right;
       stOutputWindow.Bottom = pstOutputWindow[Layer].Bottom;

    TestManager_AssertEqual( TDAL_GFX_RegionHide( GfxHandle ), eTDAL_GFX_NO_ERROR, "Show Hide" );

       TestManager_AssertEqual(TDAL_DISP_LayerIOWindowsSet(eTDAL_DISP_LAYER_GFX_ID_0,
                                                           &stInputWindow,
                                                           &stOutputWindow),
                               eTDAL_DISP_NO_ERROR,
                               "GFX first set BMP image IOWindows" );
    
    TestManager_AssertEqual( TDAL_GFX_BmpDraw( GfxHandle, offset, Bitmap ),
                            eTDAL_GFX_NO_ERROR, "Draw a bitmap in the region" );

    TestManager_AssertEqual( TDAL_GFX_RegionShow( GfxHandle ), eTDAL_GFX_NO_ERROR, "Show region" );

       /* restore GFX_ID_1 output */
       Layer = pGFX_TestScenario_LayerIdxGet(eTDAL_DISP_LAYER_GFX_ID_1);
       stOutputWindow.Left   = pstOutputWindow[Layer].Left;
       stOutputWindow.Top    = pstOutputWindow[Layer].Top;
       stOutputWindow.Right  = pstOutputWindow[Layer].Right;
       stOutputWindow.Bottom = pstOutputWindow[Layer].Bottom;

    TestManager_AssertEqual( TDAL_GFX_RegionHide( GfxHandle_1 ), eTDAL_GFX_NO_ERROR, "Show Hide" );    
    
       TestManager_AssertEqual(TDAL_DISP_LayerIOWindowsSet(eTDAL_DISP_LAYER_GFX_ID_1,
                                                           &stInputWindow,
                                                           &stOutputWindow),
                               eTDAL_DISP_NO_ERROR,
                               "GFX first set BMP image IOWindows" );

    TestManager_AssertEqual( TDAL_GFX_BmpDraw( GfxHandle_1, offset, Bitmap ),
                            eTDAL_GFX_NO_ERROR, "Draw a bitmap in the region" );

    TestManager_AssertEqual( TDAL_GFX_RegionShow( GfxHandle_1 ), eTDAL_GFX_NO_ERROR, "Show region" );


TestManager_AskForChecking( "the images of two layers return to the original size?" );

TestManager_AssertEqual( TDAL_GFX_RegionHide( GfxHandle ), eTDAL_GFX_NO_ERROR, "Show Hide" );
TestManager_AssertEqual( TDAL_GFX_RegionHide( GfxHandle_1 ), eTDAL_GFX_NO_ERROR, "Show Hide" ); 

       TestManager_AssertEqual(TDAL_DISP_LayerIOWindowsGet(eTDAL_DISP_LAYER_GFX_ID_0,
                                                           &stInputWindow,
                                                           &stOutputWindow),
                               eTDAL_DISP_NO_ERROR,
                               "GFX get IOWindows" );
       stOutputWindow.Right  /= 2;
       stOutputWindow.Bottom /= 2;

       TestManager_AssertEqual(TDAL_DISP_LayerIOWindowsSet(eTDAL_DISP_LAYER_GFX_ID_0,
                                                           &stInputWindow,
                                                           &stOutputWindow),
                               eTDAL_DISP_NO_ERROR,
                               "GFX first set BMP image IOWindows" );

    TestManager_AssertEqual( TDAL_GFX_BmpDraw( GfxHandle, offset, Bitmap ),
                            eTDAL_GFX_NO_ERROR, "Draw a bitmap in the region" );

    TestManager_AssertEqual( TDAL_GFX_RegionShow( GfxHandle ), eTDAL_GFX_NO_ERROR, "Show region" );

    
       TestManager_AssertEqual(TDAL_DISP_LayerIOWindowsGet(eTDAL_DISP_LAYER_GFX_ID_1,
                                                           &stInputWindow,
                                                           &stOutputWindow),
                               eTDAL_DISP_NO_ERROR,
                               "GFX get IOWindows" );
       stOutputWindow.Right  /= 2;
       stOutputWindow.Bottom /= 2;
    
       TestManager_AssertEqual(TDAL_DISP_LayerIOWindowsSet(eTDAL_DISP_LAYER_GFX_ID_1,
                                                           &stInputWindow,
                                                           &stOutputWindow),
                               eTDAL_DISP_NO_ERROR,
                               "GFX first set BMP image IOWindows" );

    TestManager_AssertEqual( TDAL_GFX_BmpDraw( GfxHandle_1, offset, Bitmap ),
                            eTDAL_GFX_NO_ERROR, "Draw a bitmap in the region" );

    TestManager_AssertEqual( TDAL_GFX_RegionShow( GfxHandle_1 ), eTDAL_GFX_NO_ERROR, "Show region" );


       TestManager_AskForChecking( "the images of two layers downscale 1/4 of original size?" );

    TestManager_AssertEqual( TDAL_GFX_RegionHide( GfxHandle ), eTDAL_GFX_NO_ERROR, "Show Hide" );
    TestManager_AssertEqual( TDAL_GFX_RegionHide( GfxHandle_1 ), eTDAL_GFX_NO_ERROR, "Show Hide" ); 

       TestManager_AssertEqual(TDAL_DISP_LayerIOWindowsGet(eTDAL_DISP_LAYER_GFX_ID_0,
                                                           &stInputWindow,
                                                           &stOutputWindow),
                               eTDAL_DISP_NO_ERROR,
                               "GFX get IOWindows" );
      stOutputWindow.Right  *= 2; 
        stOutputWindow.Bottom *= 2; 
    
       TestManager_AssertEqual(TDAL_DISP_LayerIOWindowsSet(eTDAL_DISP_LAYER_GFX_ID_0,
                                                           &stInputWindow,
                                                           &stOutputWindow),
                               eTDAL_DISP_NO_ERROR,
                               "GFX first set BMP image IOWindows" );
                               
    TestManager_AssertEqual( TDAL_GFX_BmpDraw( GfxHandle, offset, Bitmap ),
                            eTDAL_GFX_NO_ERROR, "Draw a bitmap in the region" );

    TestManager_AssertEqual( TDAL_GFX_RegionShow( GfxHandle ), eTDAL_GFX_NO_ERROR, "Show region" );

       TestManager_AssertEqual(TDAL_DISP_LayerIOWindowsGet(eTDAL_DISP_LAYER_GFX_ID_1,
                                                           &stInputWindow,
                                                           &stOutputWindow),
                               eTDAL_DISP_NO_ERROR,
                               "GFX get IOWindows" );
       stOutputWindow.Right  *= 2; 
        stOutputWindow.Bottom *= 2; 
    
       TestManager_AssertEqual(TDAL_DISP_LayerIOWindowsSet(eTDAL_DISP_LAYER_GFX_ID_1,
                                                           &stInputWindow,
                                                           &stOutputWindow),
                               eTDAL_DISP_NO_ERROR,
                               "GFX first set BMP image IOWindows" );


    TestManager_AssertEqual( TDAL_GFX_BmpDraw( GfxHandle_1, offset, Bitmap ),
                            eTDAL_GFX_NO_ERROR, "Draw a bitmap in the region" );

    TestManager_AssertEqual( TDAL_GFX_RegionShow( GfxHandle_1 ), eTDAL_GFX_NO_ERROR, "Show region" );


       TestManager_AskForChecking( "the images of two layers upscale 4 times of original size?" );
    }

    TestManager_AssertEqual(TDAL_DISP_LayerDisable(eTDAL_DISP_LAYER_GFX_ID_0),
                            eTDAL_DISP_NO_ERROR,
                            " disp GFX layer 0 disable" );

    TestManager_AssertEqual( TDAL_GFX_RegionDelete( GfxHandle ),
                            eTDAL_GFX_NO_ERROR, "Delete the GFX region" );
    
    if((Layer = pGFX_TestScenario_LayerIdxGet(eTDAL_DISP_LAYER_GFX_ID_1)) != (uint8_t)(-1))
    {
        TestManager_AssertEqual(TDAL_DISP_LayerDisable(eTDAL_DISP_LAYER_GFX_ID_1),
                                eTDAL_DISP_NO_ERROR,
                                " disp GFX layer 1 disable" );
        TestManager_AssertEqual( TDAL_GFX_RegionDelete( GfxHandle_1 ),
                                eTDAL_GFX_NO_ERROR, "Delete the GFX region 1" );
    }
    
    pGFX_TestScenario_TerminateTest();
    
}

static void p_GFX_Draw_Bitmap_On_Screen(tTDAL_GFX_RegionHandle regionHandle, tTDAL_GFX_BitmapHandle bitmapHandle, int srcW, int srcH, int dstW, int dstH)
{
    tTDAL_GFX_BlitContext   blitSrc, blitDst;

    blitSrc.clipOffset.x = 0;
    blitSrc.clipOffset.y = 0;
    blitSrc.colorType = eTDAL_GFX_COLOR_TRUE_COLOR_ARGB8888;
    blitSrc.clipSize.height = srcH;
    blitSrc.clipSize.width = srcW;
    blitSrc.dstOffset.x = 0;
    blitSrc.dstOffset.y = 0;
    blitSrc.offset.x = 0;
    blitSrc.offset.y = 0;
    blitSrc.pBuffer = (void *)bitmapHandle;
    blitSrc.pPalette = NULL;
    blitSrc.size.height = srcH;
    blitSrc.size.width = srcW;
    blitSrc.type = eTDAL_GFX_TYPE_BITMAP;

    blitDst.clipOffset.x = 0;
    blitDst.clipOffset.y = 0;
    blitDst.clipSize.height = srcH;
    blitDst.clipSize.width = srcW;
    blitDst.colorType = eTDAL_GFX_COLOR_TRUE_COLOR_ARGB8888;
    blitDst.dstOffset.x = 0;
    blitDst.dstOffset.y = 0;
    blitDst.offset.x = 0;
    blitDst.offset.y = 0;
    blitDst.pBuffer = (void *)regionHandle;
    blitDst.pPalette = NULL;
    blitDst.size.height = srcH;
    blitDst.size.width = srcW;
    blitDst.type = eTDAL_GFX_TYPE_DISPLAY;

    TestManager_AssertEqual( TDAL_GFX_Blit (eTDAL_GFX_BLIT_METHOD_SCALE, blitSrc, blitDst), eTDAL_GFX_NO_ERROR, "draw full screen bmp" );
}

void GFX_TestScenario_Blit(void)
{

    tTDAL_GFX_Capabilities          Capabilities;
    tTDAL_GFX_RegionHandle  GfxHandle;
    tTDAL_GFX_RegionConfig  Params;
    tTDAL_GFX_Bitmap        bx;
    tTDAL_GFX_BitmapHandle  bmpHandle, helpBmpHandle;
    tTDAL_GFX_Color         Color;
    tTDAL_GFX_BlitContext   blitSrc, blitDst;
    uint8_t                 Layer;

    pGFX_TestScenario_InitTest(GFX_TEST_MODE_576);


    Layer = pGFX_TestScenario_LayerIdxGet(eTDAL_DISP_LAYER_GFX_ID_0);

    TestManager_AssertEqual( TDAL_DISP_LayerEnable( eTDAL_DISP_LAYER_GFX_ID_0 ),
            eTDAL_DISP_NO_ERROR, "Layer enable" );

    TestManager_AssertEqual( TDAL_GFX_CapabilitiesGet( &Capabilities ), eTDAL_GFX_NO_ERROR, "GFX get capabilities" );


    /* Set up default params & create a GFX region */
    Params.offset.x    = GFX_TEST_DEFAULT_X;
    Params.offset.y    = GFX_TEST_DEFAULT_Y;
    Params.size.width  = GFX_TEST_DEFAULT_WIDTH;
    Params.size.height = GFX_TEST_DEFAULT_HEIGHT;
    Params.refSize.width  = GFX_TEST_DEFAULT_WIDTH;
    Params.refSize.height = GFX_TEST_DEFAULT_HEIGHT;
    Params.colorType   = eTDAL_GFX_COLOR_TRUE_COLOR_ARGB8888;
    Params.alpha       = kTDAL_GFX_OPAQUE;

    mTBOX_PRINT((kTBOX_LF, "region: x=%d y=%d w=%d h=%d\n",
                 Params.offset.x,
                 Params.offset.y,
                 Params.size.width,
                 Params.size.height));

    TestManager_AssertEqual( TDAL_GFX_RegionCreate( &GfxHandle, &Params ),
                            eTDAL_GFX_NO_ERROR, "Create a GFX region" );
    TestManager_AssertEqual(TDAL_GFX_RegionDestinationSet(GfxHandle,
                                                          gGFXLayerHandle[Layer]),
                            eTDAL_GFX_NO_ERROR,
                            "GFX region destination set" );

    TestManager_AssertEqual( TDAL_GFX_RegionShow( GfxHandle ),
                                eTDAL_GFX_NO_ERROR, "Show region" );

    /* Create ARGB bitmap */
    bx.colorType = eTDAL_GFX_COLOR_TRUE_COLOR_ARGB8888;
    bx.pData     = (void *)bmpTest;
    bx.size.height  = GFX_TEST_DEFAULT_HEIGHT;
    bx.size.width   = GFX_TEST_DEFAULT_WIDTH;

    TestManager_AssertEqual( TDAL_GFX_BitmapCreate(&bmpHandle, &bx), eTDAL_GFX_NO_ERROR, "Create bitmap");


    /* Color bitmap in red */

    Color.ARGB8888.R = 255;
    Color.ARGB8888.G = 0;
    Color.ARGB8888.B = 0;
    Color.ARGB8888.alpha = kTDAL_GFX_OPAQUE;

    blitSrc.type = eTDAL_GFX_TYPE_COLOR;
    blitSrc.colorType = eTDAL_GFX_COLOR_TRUE_COLOR_ARGB8888;
    blitSrc.pBuffer = &Color;

    blitDst.clipOffset.x = 0;
    blitDst.clipOffset.y = 0;
    blitDst.clipSize.height = GFX_TEST_DEFAULT_HEIGHT;
    blitDst.clipSize.width = GFX_TEST_DEFAULT_WIDTH;
    blitDst.colorType = eTDAL_GFX_COLOR_TRUE_COLOR_ARGB8888;
    blitDst.dstOffset.x = 0;
    blitDst.dstOffset.y = 0;
    blitDst.offset.x = 0;
    blitDst.offset.y = 0;
    blitDst.pBuffer = (void *)bmpHandle;
    blitDst.pPalette = NULL;
    blitDst.size.height = GFX_TEST_DEFAULT_HEIGHT;
    blitDst.size.width = GFX_TEST_DEFAULT_WIDTH;
    blitDst.type = eTDAL_GFX_TYPE_BITMAP;

    TestManager_AssertEqual( TDAL_GFX_Blit(eTDAL_GFX_BLIT_METHOD_COPY, blitSrc, blitDst), eTDAL_GFX_NO_ERROR, "Color bmp red" );

    p_GFX_Draw_Bitmap_On_Screen(GfxHandle,
            bmpHandle,
            GFX_TEST_DEFAULT_WIDTH,
            GFX_TEST_DEFAULT_HEIGHT,
            GFX_TEST_DEFAULT_WIDTH,
            GFX_TEST_DEFAULT_HEIGHT);

    TestManager_AskForChecking( "do you screen colored in red" );

    /* Draw memory on bitmap on */

    blitSrc.clipOffset.x = 0;
    blitSrc.clipOffset.y = 0;
    blitSrc.colorType = eTDAL_GFX_COLOR_TRUE_COLOR_ARGB8888;
    blitSrc.clipSize.height = bmpTestHeight;
    blitSrc.clipSize.width = bmpTestWidth;
    blitSrc.dstOffset.x = 0;
    blitSrc.dstOffset.y = 0;
    blitSrc.offset.x = 0;
    blitSrc.offset.y = 0;
    blitSrc.pBuffer = (void *)bmpTest;
    blitSrc.pPalette = NULL;
    blitSrc.size.height = bmpTestHeight;
    blitSrc.size.width = bmpTestWidth;
    blitSrc.type = eTDAL_GFX_TYPE_MEMORY;

    blitDst.clipOffset.x = 0;
    blitDst.clipOffset.y = 0;
    blitDst.clipSize.height = GFX_TEST_DEFAULT_HEIGHT;
    blitDst.clipSize.width = GFX_TEST_DEFAULT_WIDTH;
    blitDst.colorType = eTDAL_GFX_COLOR_TRUE_COLOR_ARGB8888;
    blitDst.dstOffset.x = 0;
    blitDst.dstOffset.y = 0;
    blitDst.offset.x = 0;
    blitDst.offset.y = 0;
    blitDst.pBuffer = (void*)bmpHandle;
    blitDst.pPalette = NULL;
    blitDst.size.height = GFX_TEST_DEFAULT_HEIGHT;
    blitDst.size.width = GFX_TEST_DEFAULT_WIDTH;
    blitDst.type = eTDAL_GFX_TYPE_BITMAP;

    TestManager_AssertEqual( TDAL_GFX_Blit (eTDAL_GFX_BLIT_METHOD_SCALE, blitSrc, blitDst), eTDAL_GFX_NO_ERROR, "draw full screen bmp in memory" );

    p_GFX_Draw_Bitmap_On_Screen(GfxHandle,
            bmpHandle,
            GFX_TEST_DEFAULT_WIDTH,
            GFX_TEST_DEFAULT_HEIGHT,
            GFX_TEST_DEFAULT_WIDTH,
            GFX_TEST_DEFAULT_HEIGHT);

    TestManager_AskForChecking( "do you see comedia screen with red background" );

    /* Color main bmp green */

    Color.ARGB8888.R = 0;
    Color.ARGB8888.G = 255;
    Color.ARGB8888.B = 0;
    Color.ARGB8888.alpha = kTDAL_GFX_OPAQUE;

    blitSrc.type = eTDAL_GFX_TYPE_COLOR;
    blitSrc.colorType = eTDAL_GFX_COLOR_TRUE_COLOR_ARGB8888;
    blitSrc.pBuffer = &Color;

    blitDst.clipOffset.x = 0;
    blitDst.clipOffset.y = 0;
    blitDst.clipSize.height = GFX_TEST_DEFAULT_HEIGHT;
    blitDst.clipSize.width = GFX_TEST_DEFAULT_WIDTH;
    blitDst.colorType = eTDAL_GFX_COLOR_TRUE_COLOR_ARGB8888;
    blitDst.dstOffset.x = 0;
    blitDst.dstOffset.y = 0;
    blitDst.offset.x = 0;
    blitDst.offset.y = 0;
    blitDst.pBuffer = (void *)bmpHandle;
    blitDst.pPalette = NULL;
    blitDst.size.height = GFX_TEST_DEFAULT_HEIGHT;
    blitDst.size.width = GFX_TEST_DEFAULT_WIDTH;
    blitDst.type = eTDAL_GFX_TYPE_BITMAP;

    TestManager_AssertEqual( TDAL_GFX_Blit(eTDAL_GFX_BLIT_METHOD_COPY, blitSrc, blitDst), eTDAL_GFX_NO_ERROR, "Color bmp green" );

    /* Draw four comedia screens using different types of blitting */

    blitSrc.clipOffset.x = 0;
    blitSrc.clipOffset.y = 0;
    blitSrc.colorType = eTDAL_GFX_COLOR_TRUE_COLOR_ARGB8888;
    blitSrc.clipSize.height = bmpTestHeight;
    blitSrc.clipSize.width = bmpTestWidth;
    blitSrc.dstOffset.x = 0;
    blitSrc.dstOffset.y = 0;
    blitSrc.offset.x = 0;
    blitSrc.offset.y = 0;
    blitSrc.pBuffer = (void *)bmpTest;
    blitSrc.pPalette = NULL;
    blitSrc.size.height = bmpTestHeight;
    blitSrc.size.width = bmpTestWidth;
    blitSrc.type = eTDAL_GFX_TYPE_MEMORY;

    blitDst.clipOffset.x = 0;
    blitDst.clipOffset.y = 0;
    blitDst.offset = blitDst.clipOffset;
    blitDst.clipSize.width = GFX_TEST_DEFAULT_WIDTH / 2;
    blitDst.clipSize.height = GFX_TEST_DEFAULT_HEIGHT / 2;
    blitDst.size = blitDst.clipSize;

    TestManager_AssertEqual( TDAL_GFX_Blit(eTDAL_GFX_BLIT_METHOD_SCALE, blitSrc, blitDst), eTDAL_GFX_NO_ERROR, "Comedia left upper screen" );

    /* Create ARGB bitmap */
    bx.colorType = eTDAL_GFX_COLOR_TRUE_COLOR_ARGB8888;
    bx.pData     = NULL;
    bx.size.height  = GFX_TEST_DEFAULT_HEIGHT / 2;
    bx.size.width   = GFX_TEST_DEFAULT_WIDTH / 2;

    TestManager_AssertEqual( TDAL_GFX_BitmapCreate(&helpBmpHandle, &bx), eTDAL_GFX_NO_ERROR, "Create bitmap");

    blitDst.clipOffset.x = 0;
    blitDst.clipOffset.y = 0;
    blitDst.offset = blitDst.clipOffset;
    blitDst.clipSize.width = GFX_TEST_DEFAULT_WIDTH / 2;
    blitDst.clipSize.height = GFX_TEST_DEFAULT_HEIGHT / 2;
    blitDst.size = blitDst.clipSize;
    blitDst.type = eTDAL_GFX_TYPE_BITMAP;
    blitDst.pBuffer = (void *)helpBmpHandle;

    TestManager_AssertEqual( TDAL_GFX_Blit(eTDAL_GFX_BLIT_METHOD_SCALE, blitSrc, blitDst), eTDAL_GFX_NO_ERROR, "Comedia right lower screen" );

    blitSrc.clipOffset.x = 0;
    blitSrc.clipOffset.y = 0;
    blitSrc.offset = blitSrc.clipOffset;
    blitSrc.clipSize.width = GFX_TEST_DEFAULT_WIDTH / 2;
    blitSrc.clipSize.height = GFX_TEST_DEFAULT_HEIGHT / 2;
    blitSrc.size = blitSrc.clipSize;
    blitSrc.type = eTDAL_GFX_TYPE_BITMAP;
    blitSrc.pBuffer = (void *)helpBmpHandle;

    blitDst.clipOffset.x = GFX_TEST_DEFAULT_WIDTH / 2;
    blitDst.clipOffset.y = GFX_TEST_DEFAULT_HEIGHT / 2;
    blitDst.offset = blitDst.clipOffset;
    blitDst.clipSize.width = GFX_TEST_DEFAULT_WIDTH / 2;
    blitDst.clipSize.height = GFX_TEST_DEFAULT_HEIGHT / 2;
    blitDst.size = blitDst.clipSize;
    blitDst.type = eTDAL_GFX_TYPE_BITMAP;
    blitDst.pBuffer = (void *)bmpHandle;

    TestManager_AssertEqual( TDAL_GFX_Blit(eTDAL_GFX_BLIT_METHOD_SCALE, blitSrc, blitDst), eTDAL_GFX_NO_ERROR, "Comedia right lower screen" );

    p_GFX_Draw_Bitmap_On_Screen(GfxHandle,
            bmpHandle,
            GFX_TEST_DEFAULT_WIDTH,
            GFX_TEST_DEFAULT_HEIGHT,
            GFX_TEST_DEFAULT_WIDTH,
            GFX_TEST_DEFAULT_HEIGHT);

    TestManager_AskForChecking( "do you see two comedia screens with green background in left upper and right lower corner" );

    blitSrc.clipOffset.x = 0;
    blitSrc.clipOffset.y = 0;
    blitSrc.colorType = eTDAL_GFX_COLOR_TRUE_COLOR_ARGB8888;
    blitSrc.clipSize.height = bmpTestHeight;
    blitSrc.clipSize.width = bmpTestWidth;
    blitSrc.dstOffset.x = 0;
    blitSrc.dstOffset.y = 0;
    blitSrc.offset.x = 0;
    blitSrc.offset.y = 0;
    blitSrc.pBuffer = (void *)bmpTest;
    blitSrc.pPalette = NULL;
    blitSrc.size.height = bmpTestHeight;
    blitSrc.size.width = bmpTestWidth;
    blitSrc.type = eTDAL_GFX_TYPE_MEMORY;


    blitDst.clipOffset.x = 0;
    blitDst.clipOffset.y = 0;
    blitDst.clipSize.height = GFX_TEST_DEFAULT_HEIGHT;
    blitDst.clipSize.width = GFX_TEST_DEFAULT_WIDTH;
    blitDst.colorType = eTDAL_GFX_COLOR_TRUE_COLOR_ARGB8888;
    blitDst.dstOffset.x = 0;
    blitDst.dstOffset.y = 0;
    blitDst.offset.x = 0;
    blitDst.offset.y = 0;
    blitDst.pBuffer = (void *)GfxHandle;
    blitDst.pPalette = NULL;
    blitDst.size.height = GFX_TEST_DEFAULT_HEIGHT;
    blitDst.size.width = GFX_TEST_DEFAULT_WIDTH;
    blitDst.type = eTDAL_GFX_TYPE_DISPLAY;

    TestManager_AssertEqual( TDAL_GFX_Blit (eTDAL_GFX_BLIT_METHOD_SCALE, blitSrc, blitDst), eTDAL_GFX_NO_ERROR, "draw full screen bmp" );

    TestManager_AskForChecking( "do you see full screen comedia with black background" );

    TestManager_AssertEqual( TDAL_GFX_BitmapDelete(helpBmpHandle), eTDAL_GFX_NO_ERROR, "Delete bitmap");
    TestManager_AssertEqual( TDAL_GFX_BitmapDelete(bmpHandle), eTDAL_GFX_NO_ERROR, "Delete bitmap");

    TestManager_AssertEqual( TDAL_GFX_RegionHide( GfxHandle ),
                            eTDAL_GFX_NO_ERROR, "Show region" );

    TestManager_AskForChecking( "do you see blank screen" );

    TestManager_AssertEqual( TDAL_DISP_LayerDisable( eTDAL_DISP_LAYER_GFX_ID_0),
                            eTDAL_GFX_NO_ERROR, "Layer disable" );

    /* Terminate */
    TestManager_AssertEqual( TDAL_GFX_RegionDelete( GfxHandle ),
                            eTDAL_GFX_NO_ERROR, "Delete the GFX region" );
    pGFX_TestScenario_TerminateTest();

}


void GFX_TestScenario_Blit1(void)
{
    tTDAL_GFX_Capabilities          Capabilities;
    tTDAL_GFX_RegionHandle  GfxHandle, GfxHandle1;
    tTDAL_GFX_RegionConfig  Params;
    tTDAL_GFX_Bitmap        bx;
    tTDAL_GFX_BitmapHandle  bmpHandle;
    tTDAL_GFX_Color         Color;
    tTDAL_GFX_BlitContext   blitSrc, blitDst;
    uint8_t                 Layer;
    uint8_t*                BitmapBuff=NULL;
    tTDAL_GFX_Palette       Palette;
    tTDAL_GFX_Color         PaletteColorTab[256];


    /* Set up the palette */
    Palette.nbColors = 256;
    Palette.pColor   = &PaletteColorTab[0];
    TestHelper_GFX_FillPalette( &Palette );



    pGFX_TestScenario_InitTest(GFX_TEST_MODE_576);


    Layer = pGFX_TestScenario_LayerIdxGet(eTDAL_DISP_LAYER_GFX_ID_0);

    TestManager_AssertEqual( TDAL_DISP_LayerEnable( eTDAL_DISP_LAYER_GFX_ID_0 ),
            eTDAL_DISP_NO_ERROR, "Layer enable" );

    TestManager_AssertEqual( TDAL_GFX_CapabilitiesGet( &Capabilities ), eTDAL_GFX_NO_ERROR, "GFX get capabilities" );


    /* Set up default params & create a GFX region */
    Params.offset.x    = GFX_TEST_DEFAULT_X;
    Params.offset.y    = GFX_TEST_DEFAULT_Y;
    Params.size.width  = GFX_TEST_DEFAULT_WIDTH;
    Params.size.height = GFX_TEST_DEFAULT_HEIGHT/2;
    Params.refSize.width  = GFX_TEST_DEFAULT_WIDTH;
    Params.refSize.height = GFX_TEST_DEFAULT_HEIGHT/2;
    Params.colorType   = eTDAL_GFX_COLOR_TRUE_COLOR_ARGB8888;
    Params.alpha       = kTDAL_GFX_OPAQUE;

    mTBOX_PRINT((kTBOX_LF, "region: x=%d y=%d w=%d h=%d\n",
                 Params.offset.x,
                 Params.offset.y,
                 Params.size.width,
                 Params.size.height));

    TestManager_AssertEqual( TDAL_GFX_RegionCreate( &GfxHandle, &Params ),
                            eTDAL_GFX_NO_ERROR, "Create a GFX region" );
    TestManager_AssertEqual(TDAL_GFX_RegionDestinationSet(GfxHandle,
                                                          gGFXLayerHandle[Layer]),
                            eTDAL_GFX_NO_ERROR,
                            "GFX region destination set" );

    TestManager_AssertEqual( TDAL_GFX_RegionShow( GfxHandle ),
                                eTDAL_GFX_NO_ERROR, "Show region" );

    Params.offset.x    = GFX_TEST_DEFAULT_X;
    Params.offset.y    = GFX_TEST_DEFAULT_Y + GFX_TEST_DEFAULT_HEIGHT/2;
    Params.size.width  = GFX_TEST_DEFAULT_WIDTH;
    Params.size.height = GFX_TEST_DEFAULT_HEIGHT;
    Params.refSize.width  = GFX_TEST_DEFAULT_WIDTH;
    Params.refSize.height = GFX_TEST_DEFAULT_HEIGHT;
    Params.colorType   = eTDAL_GFX_COLOR_CLUT_ARGB8888;
    Params.alpha       = kTDAL_GFX_OPAQUE;
    mTBOX_PRINT((kTBOX_LF, "region: x=%d y=%d w=%d h=%d\n",
                   Params.offset.x,
                   Params.offset.y,
                   Params.size.width,
                   Params.size.height));
    TestManager_AssertEqual( TDAL_GFX_RegionCreate( &GfxHandle1, &Params ),
                                eTDAL_GFX_NO_ERROR, "Create a GFX region" );
    TestManager_AssertEqual(TDAL_GFX_RegionDestinationSet(GfxHandle1,
                                                              gGFXLayerHandle[Layer]),
                                eTDAL_GFX_NO_ERROR,
                                "GFX region destination set" );

    TestManager_AssertEqual( TDAL_GFX_RegionShow( GfxHandle1 ),
                                    eTDAL_GFX_NO_ERROR, "Show region" );


    /* Create ARGB bitmap */
    bx.colorType = eTDAL_GFX_COLOR_TRUE_COLOR_ARGB8888;
    bx.pData     = (void *)bmpTest;
    bx.size.height  = GFX_TEST_DEFAULT_HEIGHT;
    bx.size.width   = GFX_TEST_DEFAULT_WIDTH;



    TestManager_AssertEqual( TDAL_GFX_BitmapCreate(&bmpHandle, &bx), eTDAL_GFX_NO_ERROR, "Create bitmap");


    /* Color bitmap in red */

    Color.ARGB8888.R = 255;
    Color.ARGB8888.G = 0;
    Color.ARGB8888.B = 0;
    Color.ARGB8888.alpha = kTDAL_GFX_OPAQUE;

    blitSrc.type = eTDAL_GFX_TYPE_COLOR;
    blitSrc.colorType = eTDAL_GFX_COLOR_TRUE_COLOR_ARGB8888;
    blitSrc.pBuffer = &Color;

    blitDst.clipOffset.x = 0;
    blitDst.clipOffset.y = 0;
    blitDst.clipSize.height = GFX_TEST_DEFAULT_HEIGHT/2;
    blitDst.clipSize.width = GFX_TEST_DEFAULT_WIDTH;
    blitDst.colorType = eTDAL_GFX_COLOR_TRUE_COLOR_ARGB8888;
    blitDst.dstOffset.x = 0;
    blitDst.dstOffset.y = 0;
    blitDst.offset.x = 0;
    blitDst.offset.y = 0;
    blitDst.pBuffer = (void *)bmpHandle;
    blitDst.pPalette = NULL;
    blitDst.size.height = GFX_TEST_DEFAULT_HEIGHT/2;
    blitDst.size.width = GFX_TEST_DEFAULT_WIDTH;
    blitDst.type = eTDAL_GFX_TYPE_BITMAP;

    TestManager_AssertEqual( TDAL_GFX_Blit(eTDAL_GFX_BLIT_METHOD_COPY, blitSrc, blitDst), eTDAL_GFX_NO_ERROR, "Color bmp red" );
    p_GFX_Draw_Bitmap_On_Screen(GfxHandle,
            bmpHandle,
            GFX_TEST_DEFAULT_WIDTH,
            GFX_TEST_DEFAULT_HEIGHT,
            GFX_TEST_DEFAULT_WIDTH,
            GFX_TEST_DEFAULT_HEIGHT);

    /*Color bitmap in green*/

       Color.ARGB8888.R = 0;
       Color.ARGB8888.G = 255;
       Color.ARGB8888.B = 0;
       Color.ARGB8888.alpha = kTDAL_GFX_OPAQUE;

       blitSrc.type = eTDAL_GFX_TYPE_COLOR;
       blitSrc.colorType = eTDAL_GFX_COLOR_TRUE_COLOR_ARGB8888;
       blitSrc.pBuffer = &Color;

       blitDst.clipOffset.x = 0;
       blitDst.clipOffset.y = 0;
       blitDst.clipSize.height = GFX_TEST_DEFAULT_HEIGHT;
       blitDst.clipSize.width = GFX_TEST_DEFAULT_WIDTH;
       blitDst.colorType = eTDAL_GFX_COLOR_CLUT_ARGB8888;
       blitDst.dstOffset.x = 0;
       blitDst.dstOffset.y = 0;
       blitDst.offset.x = 0;
       blitDst.offset.y = GFX_TEST_DEFAULT_HEIGHT/2;
       blitDst.pBuffer = (void *)bmpHandle;
       blitDst.pPalette = NULL;
       blitDst.size.height = GFX_TEST_DEFAULT_HEIGHT;
       blitDst.size.width = GFX_TEST_DEFAULT_WIDTH;
       blitDst.type = eTDAL_GFX_TYPE_BITMAP;

       TestManager_AssertEqual( TDAL_GFX_Blit(eTDAL_GFX_BLIT_METHOD_COPY, blitSrc, blitDst), eTDAL_GFX_NO_ERROR, "Color bmp green" );
       p_GFX_Draw_Bitmap_On_Screen(GfxHandle1,
               bmpHandle,
               GFX_TEST_DEFAULT_WIDTH,
               GFX_TEST_DEFAULT_HEIGHT,
               GFX_TEST_DEFAULT_WIDTH,
               GFX_TEST_DEFAULT_HEIGHT);

    TestManager_AskForChecking( "do you see screen colored in red and green" );

    blitSrc.clipOffset.x = 0;
    blitSrc.clipOffset.y = 0;
    blitSrc.colorType = eTDAL_GFX_COLOR_TRUE_COLOR_ARGB8888;
    blitSrc.clipSize.height = bmpTestHeight;
    blitSrc.clipSize.width = bmpTestWidth;
    blitSrc.dstOffset.x = 0;
    blitSrc.dstOffset.y = 0;
    blitSrc.offset.x = 0;
    blitSrc.offset.y = 0;
    blitSrc.pBuffer = (void *)bmpTest;
    blitSrc.pPalette = NULL;
    blitSrc.size.height = bmpTestHeight;
    blitSrc.size.width = bmpTestWidth;
    blitSrc.type = eTDAL_GFX_TYPE_MEMORY;

    blitDst.clipOffset.x = 0;
    blitDst.clipOffset.y = 0;
    blitDst.clipSize.height = GFX_TEST_DEFAULT_HEIGHT;
    blitDst.clipSize.width = GFX_TEST_DEFAULT_WIDTH;
    blitDst.colorType = eTDAL_GFX_COLOR_TRUE_COLOR_ARGB8888;
    blitDst.dstOffset.x = 0;
    blitDst.dstOffset.y = 0;
    blitDst.offset.x = 0;
    blitDst.offset.y = 0;
    blitDst.pBuffer = (void *)bmpHandle;
    blitDst.pPalette = NULL;
    blitDst.size.height = GFX_TEST_DEFAULT_HEIGHT/2;
    blitDst.size.width = GFX_TEST_DEFAULT_WIDTH/2;
    blitDst.type = eTDAL_GFX_TYPE_BITMAP;

    TestManager_AssertEqual( TDAL_GFX_Blit (eTDAL_GFX_BLIT_METHOD_SCALE, blitSrc, blitDst), eTDAL_GFX_NO_ERROR, "draw scaled bmp in memory" );

    p_GFX_Draw_Bitmap_On_Screen(GfxHandle,
                bmpHandle,
                GFX_TEST_DEFAULT_WIDTH,
                GFX_TEST_DEFAULT_HEIGHT,
                GFX_TEST_DEFAULT_WIDTH,
                GFX_TEST_DEFAULT_HEIGHT);

    TestManager_AskForChecking( "do you see comedia screen with red background in left upper corner" );



    TKEL_Malloc(100*100*1, (void **)&BitmapBuff);
    if(BitmapBuff != NULL)
    {
        uint8_t i;
        for (i=0; i<100; i++)
        {
            memset( &(BitmapBuff[i*100]), (i+1) , 100 );
            Palette.pColor[i].ARGB8888.alpha=20+i*2;
        }
    }


    blitSrc.clipOffset.x = 0;
    blitSrc.clipOffset.y = 0;
    blitSrc.colorType = eTDAL_GFX_COLOR_CLUT_ARGB8888;
    blitSrc.clipSize.height = 100;
    blitSrc.clipSize.width = 100;
    blitSrc.dstOffset.x = 0;
    blitSrc.dstOffset.y = 0;
    blitSrc.offset.x = 0;
    blitSrc.offset.y = 0;
    blitSrc.pBuffer = BitmapBuff;
    blitSrc.pPalette = &Palette;
    blitSrc.size.height = 100;
    blitSrc.size.width = 100;
    blitSrc.type = eTDAL_GFX_TYPE_MEMORY;

    blitDst.clipOffset.x = 0;
    blitDst.clipOffset.y = 0;
    blitDst.clipSize.height = GFX_TEST_DEFAULT_HEIGHT;
    blitDst.clipSize.width = GFX_TEST_DEFAULT_WIDTH;
    blitDst.colorType = eTDAL_GFX_COLOR_TRUE_COLOR_ARGB8888;
    blitDst.dstOffset.x = 0;
    blitDst.dstOffset.y = 0;
    blitDst.offset.x = GFX_TEST_DEFAULT_WIDTH/2;
    blitDst.offset.y = 0;
    blitDst.pBuffer = (void *)bmpHandle;
    blitDst.pPalette = NULL;
    blitDst.size.height = GFX_TEST_DEFAULT_HEIGHT/2;
    blitDst.size.width = GFX_TEST_DEFAULT_WIDTH/2;
    blitDst.type = eTDAL_GFX_TYPE_BITMAP;

    TestManager_AssertEqual( TDAL_GFX_Blit (eTDAL_GFX_BLIT_METHOD_SCALE, blitSrc, blitDst), eTDAL_GFX_NO_ERROR, "draw scaled bmp in memory" );

    p_GFX_Draw_Bitmap_On_Screen(GfxHandle,
                   bmpHandle,
                   GFX_TEST_DEFAULT_WIDTH,
                   GFX_TEST_DEFAULT_HEIGHT,
                   GFX_TEST_DEFAULT_WIDTH,
                   GFX_TEST_DEFAULT_HEIGHT);

    TestManager_AskForChecking( "do you see comedia screen with red background in left upper corner and bitmap in right upper corner" );

    blitSrc.clipOffset.x = 0;
    blitSrc.clipOffset.y = 0;
    blitSrc.colorType = eTDAL_GFX_COLOR_TRUE_COLOR_ARGB8888;
    blitSrc.clipSize.height = bmpTestHeight;
    blitSrc.clipSize.width = bmpTestWidth;
    blitSrc.dstOffset.x = 0;
    blitSrc.dstOffset.y = 0;
    blitSrc.offset.x = 0;
    blitSrc.offset.y = 0;
    blitSrc.pBuffer = (void *)bmpTest;
    blitSrc.pPalette = NULL;
    blitSrc.size.height = bmpTestHeight;
    blitSrc.size.width = bmpTestWidth;
    blitSrc.type = eTDAL_GFX_TYPE_MEMORY;

    blitDst.clipOffset.x = 0;
    blitDst.clipOffset.y = 0;
    blitDst.clipSize.height = GFX_TEST_DEFAULT_HEIGHT;
    blitDst.clipSize.width = GFX_TEST_DEFAULT_WIDTH;
    blitDst.colorType = eTDAL_GFX_COLOR_TRUE_COLOR_ARGB8888;
    blitDst.dstOffset.x = 0;
    blitDst.dstOffset.y = 0;
    blitDst.offset.x = GFX_TEST_DEFAULT_WIDTH/2;
    blitDst.offset.y = GFX_TEST_DEFAULT_HEIGHT/2;
    blitDst.pBuffer = (void *)bmpHandle;
    blitDst.pPalette = NULL;
    blitDst.size.height = GFX_TEST_DEFAULT_HEIGHT/2;
    blitDst.size.width = GFX_TEST_DEFAULT_WIDTH/2;
    blitDst.type = eTDAL_GFX_TYPE_BITMAP;

        TestManager_AssertEqual( TDAL_GFX_Blit (eTDAL_GFX_BLIT_METHOD_SCALE, blitSrc, blitDst), eTDAL_GFX_NO_ERROR, "draw scaled bmp in memory" );

        p_GFX_Draw_Bitmap_On_Screen(GfxHandle1,
                    bmpHandle,
                    GFX_TEST_DEFAULT_WIDTH,
                    GFX_TEST_DEFAULT_HEIGHT,
                    GFX_TEST_DEFAULT_WIDTH,
                    GFX_TEST_DEFAULT_HEIGHT);

    TestManager_AskForChecking( "do you see comedia screen with red and green background in left upper and right lower corner and bitmap in right upper corner" );

    blitSrc.clipOffset.x = 0;
    blitSrc.clipOffset.y = 0;
    blitSrc.colorType = eTDAL_GFX_COLOR_CLUT_ARGB8888;
    blitSrc.clipSize.height = 100;
    blitSrc.clipSize.width = 100;
    blitSrc.dstOffset.x = 0;
    blitSrc.dstOffset.y = 0;
    blitSrc.offset.x = 0;
    blitSrc.offset.y = 0;
    blitSrc.pBuffer = BitmapBuff;
    blitSrc.pPalette = &Palette;
    blitSrc.size.height = 100;
    blitSrc.size.width = 100;
    blitSrc.type = eTDAL_GFX_TYPE_MEMORY;

    blitDst.clipOffset.x = 0;
    blitDst.clipOffset.y = 0;
    blitDst.clipSize.height = GFX_TEST_DEFAULT_HEIGHT;
    blitDst.clipSize.width = GFX_TEST_DEFAULT_WIDTH;
    blitDst.colorType = eTDAL_GFX_COLOR_TRUE_COLOR_ARGB8888;
    blitDst.dstOffset.x = 0;
    blitDst.dstOffset.y = 0;
    blitDst.offset.x = 0;
    blitDst.offset.y = GFX_TEST_DEFAULT_HEIGHT/2;
    blitDst.pBuffer = (void *)bmpHandle;
    blitDst.pPalette = NULL;
    blitDst.size.height = GFX_TEST_DEFAULT_HEIGHT/2;
    blitDst.size.width = GFX_TEST_DEFAULT_WIDTH/2;
    blitDst.type = eTDAL_GFX_TYPE_BITMAP;

    TestManager_AssertEqual( TDAL_GFX_Blit (eTDAL_GFX_BLIT_METHOD_SCALE, blitSrc, blitDst), eTDAL_GFX_NO_ERROR, "draw scaled bmp in memory" );

    p_GFX_Draw_Bitmap_On_Screen(GfxHandle1,
                          bmpHandle,
                          GFX_TEST_DEFAULT_WIDTH,
                          GFX_TEST_DEFAULT_HEIGHT,
                          GFX_TEST_DEFAULT_WIDTH,
                          GFX_TEST_DEFAULT_HEIGHT);

    TestManager_AskForChecking( "do you see comedia screen with red and green background in left upper and right lower corner and bitmap in right upper and left lower corner" );


    TestManager_AssertEqual( TDAL_GFX_RegionHide( GfxHandle ),
                                eTDAL_GFX_NO_ERROR, "Hide region" );

    TestManager_AssertEqual( TDAL_GFX_RegionHide( GfxHandle1 ),
                                    eTDAL_GFX_NO_ERROR, "Hide region" );

    TestManager_AskForChecking( "do you see blank screen" );

    TestManager_AssertEqual( TDAL_DISP_LayerDisable( eTDAL_DISP_LAYER_GFX_ID_0),
                                eTDAL_GFX_NO_ERROR, "Layer disable" );


    TestManager_AssertEqual( TDAL_GFX_RegionDelete( GfxHandle ),
                            eTDAL_GFX_NO_ERROR, "Delete the GFX region" );

    TestManager_AssertEqual( TDAL_GFX_RegionDelete( GfxHandle1 ),
                            eTDAL_GFX_NO_ERROR, "Delete the GFX1 region" );
    TKEL_Free(BitmapBuff);
    BitmapBuff=NULL;
    pGFX_TestScenario_TerminateTest();

}
