/** \file gfx_test_perf.c
 *  \brief Performance Tests for the GFX layer of the TDAL API
 *  \author Pascal PIGNON
 *  \date 24/04/06
 *  \version 1.0
 */

/******************************************************
 *              Includes                              *
 ******************************************************/

#include "tkel.h"
#include "tbox.h"
#include "tdal_gfx.h"
#include "tdal_avs.h"
#include "tdal_output.h"
#include "tdal_disp.h"

#include "testhelper.h"
#include "gfxtests.h"
#include "gfx_test_bmp.h"

/******************************************************
 *              Defines                               *
 ******************************************************/

/******************************************************
 *              DMD test tree                         *
 ******************************************************/

static tTestNode gTestPerfMemcpy8K = {
    "TestPerfMemcpy8K",
    "test performance of memcpy for 8K region",
    0, /* not interactive */
    GFX_TestPerformance_Memcpy8K,
    NULL
};

static tTestNode gTestPerfMemcpy200K = {
    "TestPerfMemcpy200K",
    "test performance of memcpy for 200K region",
    0, /* not interactive */
    GFX_TestPerformance_Memcpy200K,
    NULL
};

static tTestNode gTestPerfRectDraw = {
    "TestPerfRectDraw",
    "test performance of drawing a rectangle in full screen",
    0, /* not interactive */
    GFX_TestPerformance_RectDraw,
    NULL
};

static tTestNode gTestPerfBmpDraw = {
    "TestPerfBmpDraw",
    "test performance of drawing a bitmap in full screen",
    0, /* not interactive */
    GFX_TestPerformance_BmpDraw,
    NULL
};

tTestNode* gGFXPerformanceTestNodeList[] = {
    &gTestPerfMemcpy8K,
    &gTestPerfMemcpy200K,
    &gTestPerfRectDraw,
    &gTestPerfBmpDraw,
    NULL
};

tTestNode gTDAL_GFX_PerformanceTestSuite = {
    "TDAL_GFX Performance Tests",
    "test the TDAL_GFX performances",
    0, /* not interactive */
    NULL, /* no function, it's a test suite */
    (void**)gGFXPerformanceTestNodeList
};

extern void pGFX_TestScenario_InitTest(uint8_t mode);
extern void pGFX_TestScenario_TerminateTest(void);
extern uint32_t pGFX_TestScenario_LayerHandleGet(tTDAL_DISP_LayerId LayerId);

/******************************************************
 *              Unitary tests                         *
 ******************************************************/

/*! \fn void GFX_TestPerformance_Memcpy8K( void )
 *  \brief Performance test for memcpy function on 8K block
 */
uint8_t             Buffer1[200*1024], Buffer2[200*1024];
void GFX_TestPerformance_Memcpy8K()
{
    const uint16_t      Size = 8*1024;
//    uint8_t             Buffer1[8*1024], Buffer2[8*1024];
    int                 i = 0;
    tTestManagerChrono  chrono;
    long                elapsed_time = 0;
    tTDAL_GFX_Size      destSize;
    tTDAL_GFX_Size      srcSize;
    tTDAL_GFX_Point     srcOffset;
    tTDAL_GFX_ColorType colorType;
    tTDAL_GFX_Error     gfx_error;

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
    colorType       = eTDAL_GFX_COLOR_CLUT_ARGB8888;
    destSize.width  = Size;
    destSize.height = 1;
    srcSize.width   = Size;
    srcSize.height  = 1;
    srcOffset.x     = 0;
    srcOffset.y     = 0;
    
    /* Copy buffers and test the result */
    TestManager_StartChrono(&chrono);

    gfx_error = TDAL_GFX_Copy2DNonZero(colorType,
                           Buffer1,
                           destSize,
                           Buffer2,
                           srcOffset,
                           srcSize);
    TestManager_StopChrono(&chrono, &elapsed_time);
    if (gfx_error == eTDAL_GFX_NO_ERROR)
    {
        TestManager_AssertEqual( eTDAL_GFX_NO_ERROR,eTDAL_GFX_NO_ERROR, "TDAL_GFX_Copy2DNonZero OK");
    }
    else
    {
        TestManager_AssertEqual( eTDAL_GFX_UNKNOWN_ERROR,eTDAL_GFX_NO_ERROR, "TDAL_GFX_Copy2DNonZero KO");
    }
    mTBOX_PRINT((kTBOX_LF, "Time elapsed for MemcpyNonZero on 8Ko buffer (half zero buffer): %ld ms\n", elapsed_time));
    
    for( i = 0; i < Size; i++ )
    {
        Buffer1[i] = 0x01;
        Buffer2[i] = 0x00;
    }
    TestManager_StartChrono(&chrono);
    
    gfx_error = TDAL_GFX_Copy2DNonZero(colorType,
                           Buffer1,
                           destSize,
                           Buffer2,
                           srcOffset,
                           srcSize);
    TestManager_StopChrono(&chrono, &elapsed_time);
    if (gfx_error == eTDAL_GFX_NO_ERROR)
    {
        TestManager_AssertEqual( eTDAL_GFX_NO_ERROR,eTDAL_GFX_NO_ERROR, "TDAL_GFX_Copy2DNonZero OK");
    }
    else
    {
        TestManager_AssertEqual( eTDAL_GFX_UNKNOWN_ERROR,eTDAL_GFX_NO_ERROR, "TDAL_GFX_Copy2DNonZero KO");
    }
    mTBOX_PRINT((kTBOX_LF, "Time elapsed for MemcpyNonZero on 8Ko buffer (full zero buffer): %ld ms\n", elapsed_time));

    for( i = 0; i < Size; i++ )
    {
        Buffer1[i] = 0x01;
        Buffer2[i] = 0x02;
    }
    TestManager_StartChrono(&chrono);
    
    gfx_error = TDAL_GFX_Copy2DNonZero(colorType,
                           Buffer1,
                           destSize,
                           Buffer2,
                           srcOffset,
                           srcSize);
    TestManager_StopChrono(&chrono, &elapsed_time);
    if (gfx_error == eTDAL_GFX_NO_ERROR)
    {
        TestManager_AssertEqual( eTDAL_GFX_NO_ERROR,eTDAL_GFX_NO_ERROR, "TDAL_GFX_Copy2DNonZero OK");
    }
    else
    {
        TestManager_AssertEqual( eTDAL_GFX_UNKNOWN_ERROR,eTDAL_GFX_NO_ERROR, "TDAL_GFX_Copy2DNonZero KO");
    }
    mTBOX_PRINT((kTBOX_LF, "Time elapsed for MemcpyNonZero on 8Ko buffer (none zero buffer): %ld ms\n", elapsed_time));
}

/*! \fn void GFX_TestPerformance_Memcpy200K( void )
 *  \brief Performance test for memcpy function on 200K block
 */
void GFX_TestPerformance_Memcpy200K()
{
    const uint32_t      Size = 200*1024;
//    static uint8_t      Buffer1[200*1024], Buffer2[200*1024];
    unsigned int        i = 0;
    tTestManagerChrono  chrono;
    long                elapsed_time = 0;
    tTDAL_GFX_Size      destSize;
    tTDAL_GFX_Size      srcSize;
    tTDAL_GFX_Point     srcOffset;
    tTDAL_GFX_ColorType colorType;
    tTDAL_GFX_Error     gfx_error;
    
    colorType       = eTDAL_GFX_COLOR_CLUT_ARGB8888;
    destSize.width  = Size;
    destSize.height = 1;
    srcSize.width   = Size;
    srcSize.height  = 1;
    srcOffset.x     = 0;
    srcOffset.y     = 0;

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
    TestManager_StartChrono(&chrono);
    
    gfx_error = TDAL_GFX_Copy2DNonZero(colorType,
                           Buffer1,
                           destSize,
                           Buffer2,
                           srcOffset,
                           srcSize);
    TestManager_StopChrono(&chrono, &elapsed_time);
    if (gfx_error == eTDAL_GFX_NO_ERROR)
    {
        TestManager_AssertEqual( eTDAL_GFX_NO_ERROR,eTDAL_GFX_NO_ERROR, "TDAL_GFX_Copy2DNonZero OK");
    }
    else
    {
        TestManager_AssertEqual( eTDAL_GFX_UNKNOWN_ERROR,eTDAL_GFX_NO_ERROR, "TDAL_GFX_Copy2DNonZero KO");
    }
    mTBOX_PRINT((kTBOX_LF, "Time elapsed for MemcpyNonZero on 200Ko buffer (half zero buffer): %ld ms\n", elapsed_time));
    
    for( i = 0; i < Size; i++ )
    {
        Buffer1[i] = 0x01;
        Buffer2[i] = 0x00;
    }
    TestManager_StartChrono(&chrono);
    
    gfx_error = TDAL_GFX_Copy2DNonZero(colorType,
                           Buffer1,
                           destSize,
                           Buffer2,
                           srcOffset,
                           srcSize);
    TestManager_StopChrono(&chrono, &elapsed_time);
    if (gfx_error == eTDAL_GFX_NO_ERROR)
    {
        TestManager_AssertEqual( eTDAL_GFX_NO_ERROR,eTDAL_GFX_NO_ERROR, "TDAL_GFX_Copy2DNonZero OK");
    }
    else
    {
        TestManager_AssertEqual( eTDAL_GFX_UNKNOWN_ERROR,eTDAL_GFX_NO_ERROR, "TDAL_GFX_Copy2DNonZero KO");
    }
    mTBOX_PRINT((kTBOX_LF, "Time elapsed for MemcpyNonZero on 200Ko buffer (full zero buffer): %ld ms\n", elapsed_time));
    
    for( i = 0; i < Size; i++ )
    {
        Buffer1[i] = 0x01;
        Buffer2[i] = 0x02;
    }
    TestManager_StartChrono( &chrono );
    
    gfx_error = TDAL_GFX_Copy2DNonZero(colorType,
                           Buffer1,
                           destSize,
                           Buffer2,
                           srcOffset,
                           srcSize);
    TestManager_StopChrono( &chrono, &elapsed_time );
    if (gfx_error == eTDAL_GFX_NO_ERROR)
    {
        TestManager_AssertEqual( eTDAL_GFX_NO_ERROR,eTDAL_GFX_NO_ERROR, "TDAL_GFX_Copy2DNonZero OK");
    }
    else
    {
        TestManager_AssertEqual( eTDAL_GFX_UNKNOWN_ERROR,eTDAL_GFX_NO_ERROR, "TDAL_GFX_Copy2DNonZero KO");
    }
    mTBOX_PRINT((kTBOX_LF, "Time elapsed for MemcpyNonZero on 200Ko buffer (none zero buffer): %ld ms\n", elapsed_time));
}

/*! \fn void GFX_TestPerformance_RectDraw( void )
 *  \brief Performance test to evaluate how long it takes
 *  to draw a fullscreen rectangle.
 */
void GFX_TestPerformance_RectDraw()
{
    tTDAL_GFX_Capabilities  Capabilities;
    tTDAL_GFX_RegionHandle  GfxHandle;
    tTDAL_GFX_RegionConfig  Params;
    tTDAL_GFX_Palette       Palette;
    tTDAL_GFX_Color         PaletteColor[256], Color;
    tTestManagerChrono      chrono;
    long                    elapsed_time = 0;
    uint32_t                LayerHandle;
    
    Palette.nbColors = 256;
    Palette.pColor   = &PaletteColor[0];

    pGFX_TestScenario_InitTest(GFX_TEST_MODE_576);

    LayerHandle = pGFX_TestScenario_LayerHandleGet(eTDAL_DISP_LAYER_GFX_ID_0);

    TestManager_AssertEqual( TDAL_GFX_Init(), eTDAL_GFX_NO_ERROR, "GFX init" );
    TestManager_AssertEqual( TDAL_GFX_CapabilitiesGet( &Capabilities ), eTDAL_GFX_NO_ERROR, "GFX get capabilities" );
    
    /* Set up default params */
    Params.offset.x    = GFX_TEST_DEFAULT_X;
    Params.offset.y    = GFX_TEST_DEFAULT_Y;
    Params.size.width  = GFX_TEST_DEFAULT_WIDTH;
    Params.size.height = GFX_TEST_DEFAULT_HEIGHT;
    Params.refSize.width  = GFX_TEST_DEFAULT_WIDTH;
    Params.refSize.height = GFX_TEST_DEFAULT_HEIGHT;
    Params.colorType   = GFX_TEST_DEFAULT_COLOR_TYPE;
    //Params.alpha       = 0;
    Params.alpha       = kTDAL_GFX_OPAQUE;
    printf("\n""\033[1;31m""[FUN][%s()@%04d] Need To Check: alpha:0 is transparent. Can't see rectangle on panel ""\033[m""\n", __FUNCTION__, __LINE__);
    printf("\n""\033[1;31m""[FUN][%s()@%04d] Need To Check: alpha:0 is transparent. Can't see rectangle on panel ""\033[m""\n", __FUNCTION__, __LINE__);

    TestManager_AssertEqual( TDAL_GFX_RegionCreate( &GfxHandle, &Params ),
                            eTDAL_GFX_NO_ERROR, "Create a GFX region" );
    TestManager_AssertEqual(TDAL_GFX_RegionDestinationSet(GfxHandle,
                                                          LayerHandle),
                            eTDAL_GFX_NO_ERROR,
                            "GFX region destination set" );
    TestManager_AssertEqual( TDAL_GFX_RegionShow( GfxHandle ),
                            eTDAL_GFX_NO_ERROR, "Show region" );
    
    /* Set palette */
    TestHelper_GFX_FillPalette( &Palette );
    TestManager_AssertEqual( TDAL_GFX_RegionPaletteSet( GfxHandle, &Palette ),
                            eTDAL_GFX_NO_ERROR, "Set palette" );
    
    /* Draw rectangle */
    Color.paletteEntry = 30;
    /* Only valid for PAL/SECAM resolution */
    Params.offset.x    = GFX_TEST_DEFAULT_X;
    Params.offset.y    = GFX_TEST_DEFAULT_Y;
    Params.size.width  = GFX_TEST_DEFAULT_WIDTH;
    Params.size.height = GFX_TEST_DEFAULT_HEIGHT;
    Params.colorType   = GFX_TEST_DEFAULT_COLOR_TYPE;

    TestManager_StartChrono( &chrono );
    TestManager_AssertEqual( TDAL_GFX_RectangleDraw(GfxHandle, Params.offset, Params.size, Params.colorType, Color),
                            eTDAL_GFX_NO_ERROR, "Draw a rectangle" );
    TestManager_StopChrono( &chrono, &elapsed_time );
    TestManager_AskForChecking( "do you see a fullscreen rectangle" );
    
    mTBOX_PRINT((kTBOX_LF, "Time elapsed for drawing a fullscreen rectangle: %ld ms\n", elapsed_time));
    TestManager_AssertEqual( TDAL_GFX_RegionDelete( GfxHandle ), eTDAL_GFX_NO_ERROR, "Delete region" );
    TestManager_AssertEqual( TDAL_GFX_Terminate(), eTDAL_GFX_NO_ERROR, "GFX terminate" );

    pGFX_TestScenario_TerminateTest();

}

/*! \fn void GFX_TestPerformance_BmpDraw( void )
 *  \brief Performance test to evaluate how long it takes
 *  to draw a fullscreen bitmap.
 */
void GFX_TestPerformance_BmpDraw()
{
    tTDAL_GFX_Capabilities  Capabilities;
    tTDAL_GFX_RegionHandle  GfxHandle;
    tTDAL_GFX_RegionConfig  Params;
    tTDAL_GFX_Bitmap        *Bitmap = NULL;
    tTDAL_GFX_Palette       *BMPPalette = NULL;
    tTDAL_GFX_Palette       Palette;
    tTDAL_GFX_Color         PaletteColor[256];
    tTestManagerChrono      chrono;
    long                    elapsed_time = 0;
    tTDAL_GFX_Point         offset;
    uint32_t                 LayerHandle;
    
    Palette.nbColors  = 256;
    Palette.pColor    = &PaletteColor[0];

    pGFX_TestScenario_InitTest(GFX_TEST_MODE_576);

    LayerHandle = pGFX_TestScenario_LayerHandleGet(eTDAL_DISP_LAYER_GFX_ID_0);

    TestHelper_Configure_TDAL_AVS();
    
    TestManager_AssertEqual( TDAL_GFX_Init(), eTDAL_GFX_NO_ERROR, "GFX init" );
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
    Params.colorType   = GFX_TEST_DEFAULT_COLOR_TYPE;
    //Params.alpha       = 0;
    Params.alpha       = kTDAL_GFX_OPAQUE;
    printf("\n""\033[1;31m""[FUN][%s()@%04d] Need To Check: alpha:0 is transparent. Can't see bmp on panel ""\033[m""\n", __FUNCTION__, __LINE__);
    printf("\n""\033[1;31m""[FUN][%s()@%04d] Need To Check: alpha:0 is transparent. Can't see bmp on panel ""\033[m""\n", __FUNCTION__, __LINE__);
    
    TestManager_AssertEqual( TDAL_GFX_RegionCreate( &GfxHandle, &Params ),
                            eTDAL_GFX_NO_ERROR, "Create a GFX region" );
    TestManager_AssertEqual(TDAL_GFX_RegionDestinationSet(GfxHandle,
                                                          LayerHandle),
                            eTDAL_GFX_NO_ERROR,
                            "GFX region destination set" );

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

    offset.x = 0;
    offset.y = 0;

    TestManager_StartChrono( &chrono );
    TestManager_AssertEqual( TDAL_GFX_BmpDraw( GfxHandle, offset, Bitmap ),
                            eTDAL_GFX_NO_ERROR, "Draw a bitmap outside the region" );
    TestManager_StopChrono( &chrono, &elapsed_time );
    mTBOX_PRINT((kTBOX_LF, "Time elapsed for drawing a fullscreen bitmap: %ld ms\n", elapsed_time));
    TestManager_AskForChecking( "do you see a windows bitmap : not full screen" );
    
    /* Terminate */
    TestManager_AssertEqual( TDAL_GFX_RegionDelete( GfxHandle ),
                            eTDAL_GFX_NO_ERROR, "Delete the GFX region" );
    TestManager_AssertEqual( TDAL_GFX_Terminate(), eTDAL_GFX_NO_ERROR, "GFX terminate" );
    
    pGFX_TestScenario_TerminateTest();
}
