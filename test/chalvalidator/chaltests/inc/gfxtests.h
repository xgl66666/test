/** \file gfxtests.h
 *  \brief Declaration of functions for tests of the GFX layer
 *  \author Pascal PIGNON
 *  \date 14/04/06
 *  \version 1.0
 */

#ifndef _GFX_TESTS_H
#define _GFX_TESTS_H

/******************************************************
 *              Include                               *
 ******************************************************/

#include "tdal_gfx.h"

#include "TestManager.h"

/******************************************************
 *              Defines                               *
 ******************************************************/
#define GFX_TEST_MODE_576  1
#define GFX_TEST_MODE_720  2
#define GFX_TEST_MODE_1080 3

#define GFX_TEST_576_WIDTH   720
#define GFX_TEST_576_HEIGHT  576
#define GFX_TEST_720_WIDTH   1280
#define GFX_TEST_720_HEIGHT  720
#define GFX_TEST_1080_WIDTH  1920
#define GFX_TEST_1080_HEIGHT 1080

#ifndef GFX_TEST_DEFAULT_X
#define GFX_TEST_DEFAULT_X 0
#endif
#ifndef GFX_TEST_DEFAULT_Y
#define GFX_TEST_DEFAULT_Y 0
#endif
#ifndef GFX_TEST_DEFAULT_WIDTH
#define GFX_TEST_DEFAULT_WIDTH GFX_TEST_720_WIDTH
#endif
#ifndef GFX_TEST_DEFAULT_HEIGHT
#define GFX_TEST_DEFAULT_HEIGHT GFX_TEST_720_HEIGHT
#endif
#ifndef GFX_TEST_DEFAULT_COLOR_TYPE
#define GFX_TEST_DEFAULT_COLOR_TYPE eTDAL_GFX_COLOR_CLUT_ARGB8888
#endif

/******************************************************
 *              Function declaration                  *
 ******************************************************/

/* functions for unitary tests */
void GFX_TestUnitary_InitTerm( void );
void GFX_TestUnitary_RegionManagement( void );
void GFX_TestUnitary_RegionAdvancedManagement( void );
void GFX_TestUnitary_RegionDraw( void );

/* functions for scenario tests */
void GFX_TestScenario_InitTerm( void );
void GFX_TestScenario_MaxRegionCreation( void );
void GFX_TestScenario_RegionOverlap( void );
void GFX_TestScenario_RegionDraw( void );
void GFX_TestScenario_MultiRegionDraw( void );
void GFX_TestScenario_ColorSystemDraw( void );
void GFX_TestScenario_Transparency( void );
void GFX_TestScenario_Memcpy( void );
void GFX_TestScenario_LayerSize576( void );
void GFX_TestScenario_LayerSize720( void );
void GFX_TestScenario_LayerSize1080( void );
void GFX_TestScenario_Blit ( void );
void GFX_TestScenario_Blit1 ( void );

/* functions fo performance tests */
void GFX_TestPerformance_Memcpy8K( void );
void GFX_TestPerformance_Memcpy200K( void );
void GFX_TestPerformance_RectDraw( void );
void GFX_TestPerformance_BmpDraw( void );

/* help functions for DMD tests */
void TestHelper_GFX_FillPalette( tTDAL_GFX_Palette *pPalette );
tTDAL_GFX_Error TestHelper_GFX_BmpLoad( uint8_t *Bmp_ptr, tTDAL_GFX_Bitmap **bitmap, tTDAL_GFX_Palette **Palette );
tTDAL_GFX_Error TestHelper_GFX_BmpUnLoad( tTDAL_GFX_Bitmap **bitmap, tTDAL_GFX_Palette **Palette );

/******************************************************
 *              DMD test tree                         *
 ******************************************************/

extern tTestNode gTDAL_GFX_UnitaryTestSuite;
extern tTestNode gTDAL_GFX_ScenarioTestSuite;
extern tTestNode gTDAL_GFX_PerformanceTestSuite;
extern tTestNode* gTDAL_GFXTestNodeList[];

#endif /* _GFX_TESTS_H */
