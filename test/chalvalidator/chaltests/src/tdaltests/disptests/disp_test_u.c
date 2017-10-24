/** \file disptest_u.c
 *  \brief unitary Tests for the display layer of the TDAL API
 *  \author WangYu
 *  \date 17/06/08
 *  \version 1.0
 */



/******************************************************
 *              Includes                              *
 ******************************************************/
#include <string.h>
#include <stdio.h>

#include "tkel.h"
#include "tbox.h"
#include "tdal_disp.h"

#include "testhelper.h"
#include "TestManager.h"


#include "tdal_disp.h"
#include "tdal_output.h"

#include "tdal_ts.h"
#include "tdal_av.h"
#include "tdal_dmx.h"
#include "tdal_avs.h"
#include "tdal_dmd.h"
#include "avtests.h"
#ifdef PRODUCT_USE_CA
#include "tdal_desc.h"
#endif

/*! \include testhelper.h */


/******************************************************
 *              Display test tree                         *
 ******************************************************/
void    TestUnitary_DispInit( void );
void    TestUnitary_DisBlender( void );
void    TestUnitary_DispLayer( void );



static tTestNode gTestUnitary_DispInit = {
	"TestUnitary_DispInit ",
	"Display init",
	0,
	TestUnitary_DispInit,
	NULL
};


static tTestNode gTestUnitary_DispBlender = {
	"TestUnitary_DispBlender",
	"Display Blender unitary test",
	0,
	TestUnitary_DisBlender,
	NULL
};

static tTestNode gTestUnitary_DispLayer = {
	"TestUnitary_DispLayer",
	"Display Layer unitary test",
	0,
	TestUnitary_DispLayer,
	NULL
};


tTestNode* gDispUnitaryTestNodeList[] = {
	&gTestUnitary_DispInit,
       &gTestUnitary_DispBlender,
       &gTestUnitary_DispLayer,
	NULL
};

tTestNode gTDAL_Disp_UnitaryTestSuite = {
	"TDAL_Disp Unitary Tests",
	"test TDAL_Disp unitary",
	0, /* not interactive */
	NULL, /* no function, it's a test suite */
	(void**)gDispUnitaryTestNodeList
};


void    TestUnitary_DispInit()
{
	tTDAL_DISP_Capability pstCapability;
		
   TestManager_AssertEqual( TDAL_DISP_Init(), eTDAL_DISP_NO_ERROR, "Disp init" );

   TestManager_AssertEqual( TDAL_DISP_CapabilityGet(&pstCapability),
						   eTDAL_DISP_NO_ERROR,
						   "Display Get capabilities" );

   TestManager_AssertEqual( TDAL_DISP_Term(), eTDAL_DISP_NO_ERROR, "Disp term" );

   TestManager_AssertEqual( TDAL_DISP_Term(), eTDAL_DISP_NOT_INIT_ERROR, "Disp term not init error" );

   TestManager_AssertEqual( TDAL_DISP_CapabilityGet(&pstCapability),
						   eTDAL_DISP_NOT_INIT_ERROR,
						   "Display Get capabilities not init error" );


}

void    TestUnitary_DisBlender( void )
{
	tTDAL_DISP_Capability DisCapatility;
	tTDAL_DISP_BlenderCapability BldCapatility;
	tTDAL_DISP_BlenderBackgroundColor pstColor;
	tTDAL_DISP_BlenderZOrderPositions pstZorderPositionlist[10];
	tTDAL_DISP_LayerId pstZorderLayerIDlist[]={eTDAL_DISP_LAYER_VIDEO_ID_0,eTDAL_DISP_LAYER_GFX_ID_0};
	uint32_t  Width, Height,pWidth,pHeight;




   TestManager_AssertEqual( TDAL_DISP_Init(), eTDAL_DISP_NO_ERROR, "Disp init" );

   TestManager_AssertEqual( TDAL_DISP_CapabilityGet(&DisCapatility),
						   eTDAL_DISP_NO_ERROR,
						   "Display Get capabilities" );

   TestManager_AssertEqual( TDAL_DISP_BlenderCapabilityGet((tTDAL_DISP_BlenderId)(DisCapatility.nbBlender+1),&BldCapatility),
							   eTDAL_DISP_BAD_PARAMETER_ERROR,
							   "Display Blender Get capabilities not available error" );		

   TestManager_AssertEqual( TDAL_DISP_BlenderCapabilityGet((tTDAL_DISP_BlenderId)(DisCapatility.nbBlender-1),&BldCapatility),
							   eTDAL_DISP_NO_ERROR,
							   "Display Blender Get capabilities " );		

   TestManager_AssertEqual( TDAL_DISP_BlenderDisable((tTDAL_DISP_BlenderId)(DisCapatility.nbBlender+1)),
							   eTDAL_DISP_BAD_PARAMETER_ERROR,
							   "Display Blender disable bad parameter error " );		

   TestManager_AssertEqual( TDAL_DISP_BlenderDisable((tTDAL_DISP_BlenderId)(DisCapatility.nbBlender-1)),
							   eTDAL_DISP_NO_ERROR,
							   "Display Blender disable " );		

   TestManager_AssertEqual( TDAL_DISP_BlenderEnable((tTDAL_DISP_BlenderId)(DisCapatility.nbBlender+1)),
							   eTDAL_DISP_BAD_PARAMETER_ERROR,
							   "Display Blender enable bad parameter error" );		

   TestManager_AssertEqual( TDAL_DISP_BlenderEnable((tTDAL_DISP_BlenderId)(DisCapatility.nbBlender-1)),
							   eTDAL_DISP_NO_ERROR,
							   "Display Blender enable " );		

   if(BldCapatility.bBackgroundColor)
   {
	pstColor.PixelFormat=0;
	TestManager_AssertEqual( TDAL_DISP_BlenderBackgroundColorSet((tTDAL_DISP_BlenderId)(DisCapatility.nbBlender-1),&pstColor),
						          eTDAL_DISP_BAD_PARAMETER_ERROR,
						          "Display Blender Set background color bad parameter error" );	
	
   }

   else
   {
	TestManager_AssertEqual( TDAL_DISP_BlenderBackgroundColorSet((tTDAL_DISP_BlenderId)(DisCapatility.nbBlender-1),&pstColor),
						          eTDAL_DISP_NOT_AVAILABLE_ERROR,
						          "Display Blender Set background color not available error" );	

   }

   TestManager_AssertEqual( TDAL_DISP_BlenderZOrderPositionsGet((tTDAL_DISP_BlenderId)(DisCapatility.nbBlender-1),NULL),
 						    eTDAL_DISP_BAD_PARAMETER_ERROR,
 						    "Display Layer Get Zorder list position bad parameter error" );		

   TestManager_AssertEqual( TDAL_DISP_BlenderZOrderPositionsGet((tTDAL_DISP_BlenderId)(DisCapatility.nbBlender-1),pstZorderPositionlist),
 						    eTDAL_DISP_NO_ERROR,
 						    "Display Layer Get Zorder list position" );		

       TestManager_AssertEqual( TDAL_DISP_BlenderZOrderListSet((tTDAL_DISP_BlenderId)(DisCapatility.nbBlender-1),NULL),
		 					    eTDAL_DISP_BAD_PARAMETER_ERROR,
		 					    "Display Layer Set Zorder list position bad parameter error" );	
		

	 TestManager_AssertEqual( TDAL_DISP_BlenderZOrderListSet((tTDAL_DISP_BlenderId)(DisCapatility.nbBlender-1),pstZorderLayerIDlist),
		 					    eTDAL_DISP_NO_ERROR,
		 					    "Display Layer Set Zorder list position" );

	 TestManager_AssertEqual( TDAL_DISP_BlenderDimensionsGet((tTDAL_DISP_BlenderId)(DisCapatility.nbBlender-1),&Width,&Height),
	 						    eTDAL_DISP_NO_ERROR,
	 						    "Display Blender Get dimension " );		
	
	pWidth=10000;
	pHeight=100;

	 TestManager_AssertEqual( TDAL_DISP_BlenderDimensionsSet((tTDAL_DISP_BlenderId)(DisCapatility.nbBlender-1),pWidth,pHeight),
	 						    eTDAL_DISP_BAD_PARAMETER_ERROR,
	 						    "Display Blender Set dimension bad parameter error " );		

	pWidth=500;
	pHeight=400;

	 TestManager_AssertEqual( TDAL_DISP_BlenderDimensionsSet((tTDAL_DISP_BlenderId)(DisCapatility.nbBlender-1),pWidth,pHeight),
	 						    eTDAL_DISP_NO_ERROR,
	 						    "Display Blender Set dimension" );		

   TestManager_AssertEqual( TDAL_DISP_Term(), eTDAL_DISP_NO_ERROR, "Disp term" );

}

void    TestUnitary_DispLayer( void )
{
    tTDAL_DISP_LayerCapability LayerCapability;
    tTDAL_DISP_LayerWindow      stInputWindow,pstInputWindow;
    tTDAL_DISP_LayerWindow      stOutputWindow;
    bool                        adjuested;
	
    stInputWindow.Left   = 0;
    stInputWindow.Top    = 0;
    stInputWindow.Right  = 400;
    stInputWindow.Bottom = 534;

    stOutputWindow.Left   = 200;
    stOutputWindow.Top    = 10;
    stOutputWindow.Right  = stInputWindow.Right + stOutputWindow.Left;
    stOutputWindow.Bottom = stInputWindow.Bottom + stOutputWindow.Top;
	

   TestManager_AssertEqual( TDAL_DISP_Init(), eTDAL_DISP_NO_ERROR, "Disp init" );
	

    TestManager_AssertEqual( TDAL_DISP_LayerCapabilityGet(eTDAL_DISP_LAYER_VIDEO_ID_0,&LayerCapability),
    							eTDAL_DISP_NO_ERROR,
    							"Display Layer Get capabilities" );		

    if(LayerCapability.IsGlobalTransparencyCapable)
    {
	     TestManager_AssertEqual( TDAL_DISP_LayerGlobalTransparencySet(
		 						eTDAL_DISP_LAYER_VIDEO_ID_0,0),
		 						eTDAL_DISP_NO_ERROR,
		 						"Display layer GlobalTransparency set to 0" );	

	     TestManager_AssertEqual( TDAL_DISP_LayerGlobalTransparencySet(
		 						eTDAL_DISP_LAYER_VIDEO_ID_2,0),
		 						eTDAL_DISP_BAD_PARAMETER_ERROR,
		 						"Display layer GlobalTransparency set bad parameter error" );	

    }

    else
    {
	     TestManager_AssertEqual( TDAL_DISP_LayerGlobalTransparencySet(
		 						eTDAL_DISP_LAYER_VIDEO_ID_0,0),
		 						eTDAL_DISP_NOT_AVAILABLE_ERROR,
		 						"Display layer GlobalTransparency set not available error" );	
    }

    pstInputWindow.Left   = 0;
    pstInputWindow.Top    = 0;
    pstInputWindow.Right  = 9000;
    pstInputWindow.Bottom = 700;

	/* Set AspectRatio*/
    if(LayerCapability.IsAspectRatioWindowConvCapable)
    {
	     TestManager_AssertEqual( TDAL_DISP_LayerIOWindowsAspectRatioParamsSet(
		 						eTDAL_DISP_LAYER_VIDEO_ID_0,&stInputWindow,eTDAL_DISP_ASPECT_RATIO_4_3,3,eTDAL_DISP_ASPECT_RATIO_CONV_14_9_LETTER_BOX),
		 						eTDAL_DISP_NO_ERROR,
		 						"Display layer AspectRatio set to 14_9_LETTER_BOX" );	
		
	     TestManager_AssertEqual( TDAL_DISP_LayerIOWindowsAspectRatioParamsSet(
		 						eTDAL_DISP_LAYER_VIDEO_ID_0,&pstInputWindow,eTDAL_DISP_ASPECT_RATIO_4_3,3,eTDAL_DISP_ASPECT_RATIO_CONV_14_9_LETTER_BOX),
		 						eTDAL_DISP_BAD_PARAMETER_ERROR,
		 						"Display layer AspectRatio set bad parameter error" );	

    }

    else
    {
	     TestManager_AssertEqual( TDAL_DISP_LayerIOWindowsAspectRatioParamsSet(
		 						eTDAL_DISP_LAYER_VIDEO_ID_0,&stInputWindow,eTDAL_DISP_ASPECT_RATIO_4_3,3,eTDAL_DISP_ASPECT_RATIO_CONV_14_9_LETTER_BOX),
		 						eTDAL_DISP_NOT_AVAILABLE_ERROR,
		 						"Display layer AspectRatio set not available error" );	
    }

	/* Set InputWindow*/
    if(LayerCapability.IsInputWindowCapable)
    {
	     TestManager_AssertEqual( TDAL_DISP_LayerIOWindowsAdjust(
		 						eTDAL_DISP_LAYER_VIDEO_ID_0,&stInputWindow,&stOutputWindow,&adjuested),
		 						eTDAL_DISP_NO_ERROR,
		 						"Display layer IO windows adjust" );	

    }

    else
    {
	     TestManager_AssertEqual( TDAL_DISP_LayerIOWindowsAdjust(
		 						eTDAL_DISP_LAYER_VIDEO_ID_0,&stInputWindow,&stOutputWindow,&adjuested),
		 						eTDAL_DISP_NOT_AVAILABLE_ERROR,
		 						"Display layer IO windows adjust not available error" );	
    }
	
   TestManager_AssertEqual( TDAL_DISP_Term(), eTDAL_DISP_NO_ERROR, "Disp term" );

}
