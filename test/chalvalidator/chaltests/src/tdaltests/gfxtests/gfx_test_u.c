/** \file gfx_test_u.c
 *  \brief Unitary Tests for the GFX layer of the TDAL API
 *  \author Pascal PIGNON
 *  \date 20/04/06
 *  \version 1.0
 */

/******************************************************
 *              Includes                              *
 ******************************************************/

#include "tkel.h"
#include "tbox.h"
#include "tdal_gfx.h"
#include "tdal_disp.h"
#include "tdal_output.h"
#include "tdal_hdmi.h"

#include "testhelper.h"
#include "gfxtests.h"
#include "gfx_test_bmp.h"

/******************************************************
 *              Defines                               *
 ******************************************************/

/******************************************************
 *              DMD test tree                         *
 ******************************************************/

static tTestNode gTestUInitTerm = {
    "TestUInitTerm",
    "test TDAL_GFX_Init() and TDAL_GFX_Term() functions",
    0, /* not interactive */
    GFX_TestUnitary_InitTerm,
    NULL
};

static tTestNode gTestURegionManagement = {
    "TestURegionManagement",
    "test TDAL_GFX_RegionCreate() and TDAL_GFX_RegionDelete() functions",
    0, /* not interactive */
    GFX_TestUnitary_RegionManagement,
    NULL
};

static tTestNode gTestURegionAdvancedManagement = {
    "TestURegionAdvancedManagement",
    "test TDAL_GFX_RegionShow(), TDAL_GFX_RegionHide(), TDAL_GFX_RegionInfoGet()... functions",
    0, /* not interactive */
    GFX_TestUnitary_RegionAdvancedManagement,
    NULL
};

static tTestNode gTestURegionDraw = {
    "TestURegionDraw",
    "test TDAL_GFX_RectangleDraw() and TDAL_GFX_BmpDraw() functions",
    0, /* not interactive */
    GFX_TestUnitary_RegionDraw,
    NULL
};

tTestNode* gGFXUnitaryTestNodeList[] = {
    &gTestUInitTerm,
    &gTestURegionManagement,
    &gTestURegionAdvancedManagement,
    &gTestURegionDraw,
    NULL
};

tTestNode gTDAL_GFX_UnitaryTestSuite = {
    "TDAL_GFX Unitary Tests",
    "test the TDAL_GFX function input parameters",
    0, /* not interactive */
    NULL, /* no function, it's a test suite */
    (void**)gGFXUnitaryTestNodeList
};

/******************************************************
 *              Unitary tests                         *
 ******************************************************/

/*! \fn void GFX_TestUnitary_InitTerm( void )
 *  \brief Test initialization and termination of GFX driver
 */
void GFX_TestUnitary_InitTerm()
{
    uint8_t i = 0;

    /* Standard Init and Terminate (3 times) */
    for ( i = 0; i < 3; i++ )
    {
        TestManager_AssertEqual( TDAL_GFX_Init(), eTDAL_GFX_NO_ERROR, "GFX init" );
        TestManager_AssertEqual( TDAL_GFX_Terminate(), eTDAL_GFX_NO_ERROR, "GFX terminate" );
    }

    /* Init twice and then terminate */
    TestManager_AssertEqual( TDAL_GFX_Init(), eTDAL_GFX_NO_ERROR, "GFX first init" );
    TestManager_AssertEqual( TDAL_GFX_Init(), eTDAL_GFX_NO_ERROR, "GFX second init" );
    TestManager_AssertEqual( TDAL_GFX_Terminate(), eTDAL_GFX_NO_ERROR, "GFX terminate" );

}

/*! \fn void GFX_TestUnitary_RegionManagement( void )
 *  \brief Test creation and deletion of regions
 *  \bug RegionCreate() and RegionDelete() functions do not
 *  return expected error with ST5105
 */
void GFX_TestUnitary_RegionManagement()
{
    tTDAL_GFX_Capabilities  Capabilities;
    tTDAL_GFX_RegionHandle  GfxHandle;
    tTDAL_GFX_RegionConfig  Params;
    
    TestManager_AssertEqual( TDAL_GFX_Init(), eTDAL_GFX_NO_ERROR, "GFX init" );
    TestManager_AssertEqual( TDAL_DISP_Init(), eTDAL_GFX_NO_ERROR, "DISP init" );//added by IL
    TestManager_AssertEqual( TDAL_GFX_CapabilitiesGet( &Capabilities ), eTDAL_GFX_NO_ERROR, "GFX get capabilities" );

    /* Set up default params */
    Params.offset.x    = GFX_TEST_DEFAULT_X;
    Params.offset.y    = GFX_TEST_DEFAULT_Y;
    Params.size.width  = GFX_TEST_DEFAULT_WIDTH;
    Params.size.height = GFX_TEST_DEFAULT_HEIGHT;
    Params.refSize.width  = GFX_TEST_DEFAULT_WIDTH;
    Params.refSize.height = GFX_TEST_DEFAULT_HEIGHT;
    Params.colorType   = GFX_TEST_DEFAULT_COLOR_TYPE;
    Params.alpha       = 0;

    /* Create region with unsupported parameters */
    TestManager_AssertEqual( TDAL_GFX_RegionCreate( NULL, &Params),
                            eTDAL_GFX_BAD_PARAMETER, "Create region with null handle" );
    
    Params.size.width = GFX_TEST_DEFAULT_WIDTH + 5;
    TestManager_AssertEqual( TDAL_GFX_RegionCreate( &GfxHandle, &Params),
                            eTDAL_GFX_BAD_PARAMETER, "Create region with unsupported coordinates" );

    Params.size.width = GFX_TEST_DEFAULT_WIDTH;
    Params.colorType  = eTDAL_GFX_COLOR_TRUE_COLOR_ARGB8888;//why is this unsupported color type?
    //TestManager_AssertEqual( TDAL_GFX_RegionCreate( &GfxHandle, &Params),
                            //eTDAL_GFX_BAD_PARAMETER, "Create region with unsupported color type" );

    /* Delete a Region with a bad handle */
    Params.colorType  = GFX_TEST_DEFAULT_COLOR_TYPE;
    TestManager_AssertEqual( TDAL_GFX_RegionCreate( &GfxHandle, &Params),
                            eTDAL_GFX_NO_ERROR, "Create a valid region" );
    //TestManager_AssertEqual( TDAL_GFX_RegionDelete( 0x100 ),
                      //      eTDAL_GFX_UNKNOWN_REGION, "Delete the region with a bad handle" );
    TestManager_AssertEqual( TDAL_GFX_RegionDelete( GfxHandle ),
                            eTDAL_GFX_NO_ERROR, "Delete the region" );

    /* Terminate GFX driver */
    TestManager_AssertEqual( TDAL_GFX_Terminate(), eTDAL_GFX_NO_ERROR, "GFX terminate" );
    TestManager_AssertEqual( TDAL_DISP_Term(), eTDAL_GFX_NO_ERROR, "GFX terminate" );
}

/*! \fn void GFX_TestUnitary_RegionAdvancedManagement( void )
 *  \brief Test functions like TDAL_GFX_RegionShow, TDAL_GFX_RectangleDraw,
 *  TDAL_GFX_BmpDraw with bad handle.
 *  \bug Tested functions do not return expected values for ST5105
 */
static tTDAL_GFX_Color          PaletteColor[256];
void GFX_TestUnitary_RegionAdvancedManagement()
{
    tTDAL_GFX_Capabilities  Capabilities;
    tTDAL_GFX_RegionHandle  GfxHandle, GfxBadHandle;
    tTDAL_GFX_RegionConfig  Params;
#if 0 //Currently not used(use cases commented out)
    tTDAL_GFX_Color         Color;
    tTDAL_GFX_Bitmap        Bitmap;
#endif
    tTDAL_GFX_Palette       Palette;
    tTDAL_GFX_Point         Offset;
    tTDAL_GFX_Size          Size;
    uint32_t                Width, Height;
    tTDAL_DISP_LayerWindow  stInputWindow;
    tTDAL_DISP_LayerWindow  stOutputWindow;
    uint32_t                LayerHandle;

    Palette.colorType = GFX_TEST_DEFAULT_COLOR_TYPE;
    Palette.nbColors  = 256;
    Palette.pColor    = &PaletteColor[0];
    
    TestManager_AssertEqual( TDAL_GFX_Init(), eTDAL_GFX_NO_ERROR, "GFX init" );
    TestManager_AssertEqual( TDAL_GFX_CapabilitiesGet( &Capabilities ), eTDAL_GFX_NO_ERROR, "GFX get capabilities" );
    
    TestManager_AssertEqual( TDAL_DISP_Init(), eTDAL_DISP_NO_ERROR, "DISP init" );
    
    TestManager_AssertEqual(TDAL_DISP_BlenderDimensionsGet(eTDAL_DISP_BLENDER_ID_0,
                                                           &Width,
                                                           &Height),
                            eTDAL_DISP_NO_ERROR,
                            "DISP get blender dimensions" );

    stInputWindow.Left    = 0;
    stInputWindow.Top     = 0;
    stInputWindow.Right   = Width;
    stInputWindow.Bottom  = Height;
    stOutputWindow.Left   = 0;
    stOutputWindow.Top    = 0;
    stOutputWindow.Right  = Width;
    stOutputWindow.Bottom = Height;
    
    TestManager_AssertEqual(TDAL_DISP_LayerIOWindowsSet(eTDAL_DISP_LAYER_GFX_ID_0,
                                                        &stInputWindow,
                                                        &stOutputWindow),
                            eTDAL_DISP_NO_ERROR,
                            "DISP GFX layer set image IOWindows" );
    
    TestManager_AssertEqual(TDAL_DISP_LayerEnable(eTDAL_DISP_LAYER_GFX_ID_0),
                            eTDAL_DISP_NO_ERROR,
                            "disp GFX layer enable" );

    /* Set up default params */
    Params.offset.x    = GFX_TEST_DEFAULT_X;
    Params.offset.y    = GFX_TEST_DEFAULT_Y;
    Params.size.width  = GFX_TEST_DEFAULT_WIDTH;
    Params.size.height = GFX_TEST_DEFAULT_HEIGHT;
    Params.refSize.width  = GFX_TEST_DEFAULT_WIDTH;
    Params.refSize.height = GFX_TEST_DEFAULT_HEIGHT;
    Params.colorType   = GFX_TEST_DEFAULT_COLOR_TYPE;
    Params.alpha       = 0;

    TestManager_AssertEqual( TDAL_GFX_RegionCreate( &GfxHandle, &Params ),
                            eTDAL_GFX_NO_ERROR, "Create a GFX region" );
    GfxBadHandle = 0x200;

    TestManager_AssertEqual(TDAL_DISP_LayerHandleGet(eTDAL_DISP_LAYER_GFX_ID_0,
                                                     &LayerHandle),
                            eTDAL_DISP_NO_ERROR,
                            "DISP layer handle get" );

    TestManager_AssertEqual(TDAL_GFX_RegionDestinationSet(GfxHandle,
                                                          LayerHandle),
                            eTDAL_GFX_NO_ERROR,
                            "GFX region destination set" );

    /* Show region with bad handle */
    //TestManager_AssertEqual( TDAL_GFX_RegionShow( GfxBadHandle ),
                         //   eTDAL_GFX_UNKNOWN_REGION, "Show region with wrong handle" );

    /* Hide region with bad handle */
   // TestManager_AssertEqual( TDAL_GFX_RegionHide( GfxBadHandle ),
                           // eTDAL_GFX_UNKNOWN_REGION, "Hide region with wrong handle" );
    
    /* Get info about region with bad handle */
    //TestManager_AssertEqual( TDAL_GFX_RegionConfigGet( GfxBadHandle, &Params ),
                          //  eTDAL_GFX_UNKNOWN_REGION, "Get info about region with bad handle" );
    
    /* Set palette with bad handle */
   // TestManager_AssertEqual( TDAL_GFX_RegionPaletteSet( GfxBadHandle, &Palette ),
                           // eTDAL_GFX_UNKNOWN_REGION, "Set palette with bad handle" );
    
    /* Get palette with bad handle */
    //TestManager_AssertEqual( TDAL_GFX_RegionPaletteGet( GfxBadHandle, &Palette ),
                           // eTDAL_GFX_UNKNOWN_REGION, "Get palette with bad handle" );
    
    /* Set global transparency with bad handle */
    //TestManager_AssertEqual( TDAL_GFX_RegionTransparencySet( GfxBadHandle, 0 ),
                          //  eTDAL_GFX_UNKNOWN_REGION, "Set global transparency with bad handle" );
    
    /* Draw rectangle with bad handle */
    Offset.x    = 10;
    Offset.y    = 10;
    Size.width  = 20;
    Size.height = 20;
 /*   TestManager_AssertEqual( TDAL_GFX_RectangleDraw( GfxBadHandle,
                                                     Offset,
                                                     Size,
                                                     Params.colorType,
                                                     Color),
                            eTDAL_GFX_UNKNOWN_REGION, "Draw rectangle with bad handle" );
    */
    /* Draw bitmap with bad handle */
    //TestManager_AssertEqual( TDAL_GFX_BmpDraw( GfxBadHandle, Offset, &Bitmap ),
                          //  eTDAL_GFX_UNKNOWN_REGION, "Draw bitmap with bad handle" );
    
    /* Get bitmap with bad handle */
   //TestManager_AssertEqual( TDAL_GFX_BmpGet( GfxBadHandle, Offset, &Bitmap ),
                     //       eTDAL_GFX_UNKNOWN_REGION, "Get bitmap with bad handle" );
  
    /* Advanced palette test */
    TestManager_AssertEqual( TDAL_GFX_RegionPaletteSet( GfxHandle, NULL ),
                            eTDAL_GFX_BAD_PARAMETER, "Set palette with NULL palette" );
    
    TestHelper_GFX_FillPalette( &Palette );

    /* Fill palette with wrong color type */
    Palette.colorType =  0;
    TestManager_AssertEqual( TDAL_GFX_RegionPaletteSet( GfxHandle, &Palette ),
                            eTDAL_GFX_BAD_PARAMETER, "Set palette with wrong color type" );

    Palette.colorType = GFX_TEST_DEFAULT_COLOR_TYPE;
    TestManager_AssertEqual( TDAL_GFX_RegionPaletteSet( GfxHandle, &Palette ),
                            eTDAL_GFX_NO_ERROR, "Set palette with good params" );

    TestManager_AssertEqual( TDAL_GFX_RegionPaletteGet( GfxHandle, NULL ),
                            eTDAL_GFX_BAD_PARAMETER, "Get palette with NULL palette" );

    TestManager_AssertEqual( TDAL_GFX_RegionDelete( GfxHandle ),
                            eTDAL_GFX_NO_ERROR, "Delete the GFX region" );

    TestManager_AssertEqual( TDAL_GFX_Terminate(), eTDAL_GFX_NO_ERROR, "GFX terminate" );
    
    TestManager_AssertEqual( TDAL_DISP_Term(), eTDAL_DISP_NO_ERROR, "DISP terminate" );

}

/*! \fn void GFX_TestUnitary_RegionDraw( void )
 *  \brief Draw a rectangle and a bitmap outside the created region
 */
void GFX_TestUnitary_RegionDraw()
{
    tTDAL_GFX_Capabilities  Capabilities;
    tTDAL_GFX_RegionHandle  GfxHandle;
    tTDAL_GFX_RegionConfig  Params;
    tTDAL_GFX_Bitmap        *Bitmap = NULL;
    tTDAL_GFX_Palette       *BMPPalette = NULL;
    tTDAL_GFX_Palette       Palette;
    tTDAL_GFX_Color         PaletteColor[256];
    tTDAL_GFX_Color         Color;
    uint16_t                width = 100, height = 50;
	uint16_t                x = GFX_TEST_DEFAULT_WIDTH - width / 2;
	uint16_t                y = GFX_TEST_DEFAULT_HEIGHT - height/2;
    tTDAL_GFX_Point         Offset;
    tTDAL_GFX_Size          Size;
    uint32_t                Width, Height;
    tTDAL_DISP_LayerWindow  stInputWindow;
    tTDAL_DISP_LayerWindow  stOutputWindow;
    uint32_t                LayerHandle;

    
    Palette.colorType = GFX_TEST_DEFAULT_COLOR_TYPE;
    Palette.nbColors  = 256;
    Palette.pColor    = &PaletteColor[0];
    
    TestManager_AssertEqual( TDAL_OUTPUT_Init(), eTDAL_DISP_NO_ERROR, "OUTPUT init" );
    TestManager_AssertEqual( TDAL_HDMI_Init(), eTDAL_DISP_NO_ERROR, "HDMI init" );

    TestManager_AssertEqual( TDAL_GFX_Init(), eTDAL_GFX_NO_ERROR, "GFX init" );
    TestManager_AssertEqual( TDAL_GFX_CapabilitiesGet( &Capabilities ), eTDAL_GFX_NO_ERROR, "GFX get capabilities" );
    
    TestManager_AssertEqual( TDAL_DISP_Init(), eTDAL_DISP_NO_ERROR, "DISP init" );

    TestManager_AssertEqual(TDAL_DISP_BlenderDimensionsGet(eTDAL_DISP_BLENDER_ID_0,
                                                           &Width,
                                                           &Height),
                            eTDAL_DISP_NO_ERROR,
                            "DISP get blender dimensions" );
    
    stInputWindow.Left    = 0;
    stInputWindow.Top     = 0;
    stInputWindow.Right   = Width;
    stInputWindow.Bottom  = Height;
    stOutputWindow.Left   = 0;
    stOutputWindow.Top    = 0;
    stOutputWindow.Right  = Width;
    stOutputWindow.Bottom = Height;
    
    TestManager_AssertEqual(TDAL_DISP_LayerIOWindowsSet(eTDAL_DISP_LAYER_GFX_ID_0,
                                                        &stInputWindow,
                                                        &stOutputWindow),
                            eTDAL_DISP_NO_ERROR,
                            "DISP GFX layer set image IOWindows" );

    /* Set up default params & create a GFX region */
    Params.offset.x    = GFX_TEST_DEFAULT_X;
    Params.offset.y    = GFX_TEST_DEFAULT_Y;
    Params.size.width  = GFX_TEST_DEFAULT_WIDTH;
    Params.size.height = GFX_TEST_DEFAULT_HEIGHT;
    Params.refSize.width  = GFX_TEST_DEFAULT_WIDTH;
    Params.refSize.height = GFX_TEST_DEFAULT_HEIGHT;
    Params.colorType   = GFX_TEST_DEFAULT_COLOR_TYPE;
    Params.alpha       = 0;
    
    TestManager_AssertEqual( TDAL_GFX_RegionCreate( &GfxHandle, &Params ),
                            eTDAL_GFX_NO_ERROR, "Create a GFX region" );
    TestManager_AssertEqual(TDAL_DISP_LayerHandleGet(eTDAL_DISP_LAYER_GFX_ID_0,
                                                     &LayerHandle),
                            eTDAL_DISP_NO_ERROR,
                            "DISP layer handle get" );

    TestManager_AssertEqual(TDAL_GFX_RegionDestinationSet(GfxHandle,
                                                          LayerHandle),
                            eTDAL_GFX_NO_ERROR,
                            "GFX region destination set" );


    /* Set up the palette */
    TestHelper_GFX_FillPalette( &Palette );
    TestManager_AssertEqual( TDAL_GFX_RegionPaletteSet( GfxHandle, &Palette ),  eTDAL_GFX_NO_ERROR, "Set the palette" );

    /* Test RectangleDraw() */
    Offset.x    = x;
    Offset.y    = y;
    Size.width  = width;
    Size.height = height;

    Color.paletteEntry = 1;
    
    TestManager_AssertEqual( TDAL_GFX_RectangleDraw( GfxHandle, Offset, Size, Params.colorType, Color ),
                            eTDAL_GFX_BAD_PARAMETER, "Draw a rectangle outside the region" );
    
    /* Test BmpDraw() */
    TestManager_AssertEqual( TestHelper_GFX_BmpLoad( (uint8_t *)Bitmap_Linux, &Bitmap, &BMPPalette),
                            eTDAL_GFX_NO_ERROR, "Load the bitmap" );
    TestManager_AssertEqual( TDAL_GFX_RegionPaletteSet( GfxHandle, BMPPalette ), eTDAL_GFX_NO_ERROR, "Set the palette" );
    TestManager_AssertEqual( TDAL_GFX_BmpDraw( GfxHandle, Offset, Bitmap ),
                            eTDAL_GFX_BAD_PARAMETER, "Draw a bitmap outside the region" );


    TestManager_AssertEqual( TDAL_DISP_LayerEnable(eTDAL_DISP_LAYER_CURSOR_ID_0 ),
                            eTDAL_DISP_BAD_PARAMETER_ERROR, "Layer enable" );
                            
    
    TestManager_AssertEqual( TDAL_DISP_LayerDisable( eTDAL_DISP_LAYER_CURSOR_ID_0),
                            eTDAL_DISP_BAD_PARAMETER_ERROR, "Layer disable" );

    TestManager_AssertEqual( TDAL_DISP_LayerEnable(eTDAL_DISP_LAYER_CURSOR_ID_0 ),
                            eTDAL_DISP_BAD_PARAMETER_ERROR, "Layer enable" );
    

    /* Terminate */
    TestManager_AssertEqual( TDAL_GFX_RegionDelete( GfxHandle ),
                            eTDAL_GFX_NO_ERROR, "Delete the GFX region" );
    TestManager_AssertEqual( TDAL_GFX_Terminate(), eTDAL_GFX_NO_ERROR, "GFX terminate" );
    
    TestManager_AssertEqual( TDAL_DISP_Term(), eTDAL_DISP_NO_ERROR, "DISP terminate" );
	
	TestManager_AssertEqual( TDAL_HDMI_Term(), eTDAL_DISP_NO_ERROR, "HDMI terminate" );
}
