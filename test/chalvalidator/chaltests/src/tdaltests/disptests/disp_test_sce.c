/** \file disptest_sce.c
 *  \brief Scenario Tests for the display layer of the TDAL API
 *  \author WangYu
 *  \date 06/06/08
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

#include "tdal_hdmi.h"

#include "tdal_disp.h"
#include "tdal_output.h"

#include "tdal_ts.h"
#include "tdal_av.h"
#include "tdal_dmx.h"
#include "tdal_avs.h"
#include "tdal_dmd.h"

#include "avtests.h"
#include "gfxtests.h"
#ifdef PRODUCT_USE_CA
#include "tdal_desc.h"
#endif

/*! \include testhelper.h */

extern void HDMICallback(tTDAL_HDMI_SourceId Id, tTDAL_HDMI_Event Event);

/******************************************************
 *              Display test tree                         *
 ******************************************************/
void    TestSce_DispBlender_Capability( void );
void    TestSce_DispLayer_Capability( void );
void    TestSce_DispLayer_Disp_Management( void );
void    TestSce_DispLayer_Supported_Capability( void );



static tTestNode gTestScenario_DispBlender_Capability = {
	"TestSce_DispBlender_Capability",
	"Display blender capability",
	0,
	TestSce_DispBlender_Capability,
	NULL
};


static tTestNode gTestScenario_DispLayer_Capability = {
	"TestSce_DispLayer_Capability",
	"Display Layer_Capability",
	0,
	TestSce_DispLayer_Capability,
	NULL
};

static tTestNode gTestScenario_DispLayer_Management = {
	"TestSce_DispLayer_Disp_Management",
	"Layer Display Management",
	0,
	TestSce_DispLayer_Disp_Management,
	NULL
};

static tTestNode gTestScenario_DispLayer_Supported_Capability = {
	"TestSce_DispLayer_Supported_Capability",
	"Display Layer_Supported_Capability",
	0,
	TestSce_DispLayer_Supported_Capability,
	NULL
};



tTestNode* gDispScenarioTestNodeList[] = {
	&gTestScenario_DispBlender_Capability,
       &gTestScenario_DispLayer_Capability,
       &gTestScenario_DispLayer_Management,
	&gTestScenario_DispLayer_Supported_Capability,
	NULL
};

tTestNode gTDAL_Disp_ScenarioTestSuite = {
	"TDAL_Disp Scenario Tests",
	"test TDAL_Disp scenario",
	0, /* not interactive */
	NULL, /* no function, it's a test suite */
	(void**)gDispScenarioTestNodeList
};




/******************************************************
 *              Defines                               *
 ******************************************************/


/******************************************************
 *              Enums / Typedefs / Structures         *
 ******************************************************/


/******************************************************
 *              Local variables (LOCAL)               *
 ******************************************************/


/******************************************************
 *              Functions declarations                *
 ******************************************************/

#if 1 /* GS : the following test must be reviewed upon TDAL_DISP_BlenderZOrderListSet(-) calls.
       * For the moment just make a stub
	    */
void    TestSce_DispBlender_Capability( void )
{
}
#else /* GS : the test must be reviewed */
void    TestSce_DispBlender_Capability( void )
{
	tTDAL_DISP_Capability DisCapatility;
	tTDAL_DISP_BlenderCapability BldCapatility[5];
	tTDAL_DISP_BlenderBackgroundColor pstColor;
	tTDAL_DISP_LayerId pstZorderLayerIDlist[5];
       tTDAL_DISP_BlenderZOrderPositions pstZorderPositionlist[10];
	bool checkedOK=TRUE;
	 tTDAL_AV_StreamType StreamType;
	tTDAL_OUTPUT_VideoHDDACParams stHDDACParams;
	tTDAL_OUTPUT_Capability       stOutputCap;
	


	uint32_t  Width, Height,pWidth,pHeight;
	uint8_t index,i,j, temp;

    TestHelper_Initialize_TDAL_DMD();
    TestManager_AssertEqual(TDAL_DMX_Init(),
							kTDAL_DMX_NO_ERROR,
							"dmx init");
#ifdef PRODUCT_USE_CA
    TestManager_AssertEqual(TDAL_DESC_Init(),
							eTDAL_DESC_ERROR,
							"desc init");
#endif

#ifdef AVS_STB
            TestManager_AssertEqual(TDAL_AVS_Init(),
                                    eTDAL_AVS_NO_ERROR,
                                    "avs init");
		        TestHelper_Configure_TDAL_AVS();
#endif



    TestManager_AssertEqual( TDAL_DISP_Init(), eTDAL_DISP_NO_ERROR, "Display init" );

    TestManager_AssertEqual(TDAL_OUTPUT_Init(),
							eTDAL_OUTPUT_NO_ERROR,
							"output init" );
#ifdef TDAL_HDMI_USE	
    TestManager_AssertEqual(TDAL_HDMI_Init(),
							eTDAL_HDMI_NO_ERROR,
							"hdmi init" );
#endif

    TestManager_AssertEqual(TDAL_OUTPUT_CapabilityGet(&stOutputCap),
							eTDAL_OUTPUT_NO_ERROR,
							"output caps");
	
	if(stOutputCap.nbVideoHDDAC > 0)
	{
		stHDDACParams.ColorSpace     = eTDAL_OUTPUT_VIDEO_HD_DAC_ITU601;
		stHDDACParams.Format         = eTDAL_OUTPUT_VIDEO_HD_DAC_YCBCR;
		stHDDACParams.Standard       = eTDAL_OUTPUT_VIDEO_HD_STD_1920_1080_I_50;
		stHDDACParams.EmbeddedSyncOn = FALSE;
		TestManager_AssertEqual(TDAL_OUTPUT_VideoHDDACParamsSet(eTDAL_OUTPUT_VIDEO_HD_DAC_ID_0,
																&stHDDACParams),
								eTDAL_OUTPUT_NO_ERROR,
								"output hd dac params set");
		TestManager_AssertEqual(TDAL_OUTPUT_Enable(eTDAL_OUTPUT_VIDEO_HD_DAC_ID_0),
								eTDAL_OUTPUT_NO_ERROR,
								"output hd dac enable");
	}

#ifdef TDAL_HDMI_USE
		if(stOutputCap.nbHDMI > 0)
		{
			TestManager_AssertEqual(TDAL_HDMI_Start(eTDAL_HDMI_SOURCE_ID_0,
													HDMICallback,
													FALSE),
									eTDAL_HDMI_NO_ERROR,
									"hdmi start" );
			
			TestManager_AssertEqual(TDAL_OUTPUT_Enable(eTDAL_OUTPUT_HDMI_ID_0),
							eTDAL_OUTPUT_NO_ERROR,
							"output HDMI video enable" );
	
		}
#endif
	
    TestManager_AssertEqual(TDAL_TSROUTE_Init(),
							eTDAL_TSROUTE_NO_ERROR,
							"tsroute init");
    TestManager_AssertEqual(TDAL_AV_Init(),
							eTDAL_AV_NO_ERROR,
							"av init");

    TestHelper_Configure_TDAL_TSROUTE(TEST_HELPER_TSROUTE_TUNER(gTDAL_AV_TestFeIdx),
                                      TEST_HELPER_TSROUTE_DEMUX(gTDAL_AV_TestDmxIdx));

    TestHelper_DMDTune(gTDAL_AV_TestFeIdx);
    TestHelper_Configure_TDAL_DMX(gTDAL_AV_TestDmxIdx);


       TestManager_AssertEqual( TDAL_DISP_CapabilityGet(&DisCapatility),
	   						   eTDAL_DISP_NO_ERROR,
	   						   "Display Get capabilities" );
       mTBOX_PRINT(( kTBOX_LF, "Display capabilities : %d blenders\n", DisCapatility.nbBlender));
	
#ifdef TDAL_HDMI_USE
	if(stOutputCap.nbHDMI > 0)
	{
		TestManager_AssertEqual(TDAL_HDMI_Start(eTDAL_HDMI_SOURCE_ID_0,
												HDMICallback,
												FALSE),
								eTDAL_HDMI_NO_ERROR,
								"hdmi start" );
	}
#endif

	if(stOutputCap.nbVideoDenc > 0)
	{
		TestManager_AssertEqual(TDAL_OUTPUT_Enable(eTDAL_OUTPUT_VIDEO_DENC_ID_0),
								eTDAL_OUTPUT_NO_ERROR,
								"output denc video enable" );
	}


    StreamType.videoType = eTDAL_AV_VIDEO_TYPE_MPEG2;

    TestManager_AssertEqual(TDAL_AV_InputStreamSet(eTDAL_AV_DECODER_VIDEO_1,
									  			TestHelper_DMXGetStreamHandle(eTDAL_AV_DECODER_VIDEO_1)),
						eTDAL_AV_NO_ERROR,
						"TDAL AV input stream set\n");

	
    TestManager_AssertEqual(TDAL_AV_Start(eTDAL_AV_DECODER_VIDEO_1,
										  StreamType),
							eTDAL_AV_NO_ERROR,
							"av start video");

    /*enable layer video*/	
    TestManager_AssertEqual(TDAL_DISP_LayerEnable(eTDAL_DISP_LAYER_VIDEO_ID_0),
							eTDAL_DISP_NO_ERROR,
							"disp video enable" );



	for(index=0;index< DisCapatility.nbBlender;index++)
	{
		TestManager_AssertEqual( TDAL_DISP_BlenderCapabilityGet((tTDAL_DISP_BlenderId)index,&BldCapatility[index]),
								   eTDAL_DISP_NO_ERROR,
								   "Display Blender Get capabilities" );		
      		 mTBOX_PRINT(( kTBOX_LF, "No. %d Display Blender capabilities : %d layers\n",index+1, BldCapatility[index].nbLayer));

		/* Blender background color set */
		if(BldCapatility[index].bBackgroundColor)
		{
			pstColor.PixelFormat=kTDAL_DISP_PIXEL_FORMAT_RGB888;
			pstColor.Color=(0xff<<24|0xff<<16|0xff<<8);/*white */
			TestManager_AssertEqual( TDAL_DISP_BlenderBackgroundColorSet((tTDAL_DISP_BlenderId)index,&pstColor),
								          eTDAL_DISP_NO_ERROR,
								          "Display Blender Set background color: white" );
			
			TestManager_AskForChecking( "background color is white? " );

			
			pstColor.Color=0;/*black */
			TestManager_AssertEqual( TDAL_DISP_BlenderBackgroundColorSet((tTDAL_DISP_BlenderId)index,&pstColor),
									  eTDAL_DISP_NO_ERROR,
									  "Display Blender Set background color: black" );

			TestManager_AskForChecking( "background color is black? " );

			pstColor.Color=(0xc8<<24|0x46<<16|0x24<<8);/*other */
			TestManager_AssertEqual( TDAL_DISP_BlenderBackgroundColorSet((tTDAL_DISP_BlenderId)index,&pstColor),
									   eTDAL_DISP_NO_ERROR,
									   "Display Blender Set background color: other" );	

			TestManager_AskForChecking( "background color change from black to other color?? " );
			
		}

		else
		{
			 mTBOX_PRINT(( kTBOX_LF, "Blender background color set not supported"));

		}

			/* ZOrder list test */
    		 TestManager_AssertEqual( TDAL_DISP_BlenderZOrderPositionsGet((tTDAL_DISP_BlenderId)index,pstZorderPositionlist),
    		 						    eTDAL_DISP_NO_ERROR,
    		 						    "Display Layer Get Zorder list position" );		

		for(i=0;i<BldCapatility[index].nbLayer;i++)
		{
      			 mTBOX_PRINT(( kTBOX_LF, "Display Blender Zorder list Get : [layerID: %d], [position: %d]\n",pstZorderPositionlist[i].LayerId,pstZorderPositionlist[i].ZOrderPositions));
			
		}


		for(i=0;i<BldCapatility[index].nbLayer-1;i++)
		{
			temp=pstZorderLayerIDlist[0];
			pstZorderLayerIDlist[0]=pstZorderLayerIDlist[i+1];
			pstZorderLayerIDlist[i+1]=temp;
			
	    		 TestManager_AssertEqual( TDAL_DISP_BlenderZOrderListSet((tTDAL_DISP_BlenderId)index,pstZorderLayerIDlist),
	 					    eTDAL_DISP_NO_ERROR,
	 					    "Display Layer Set Zorder list position" );		
	    		 TestManager_AssertEqual( TDAL_DISP_BlenderZOrderPositionsGet((tTDAL_DISP_BlenderId)index,pstZorderPositionlist),
				 					    eTDAL_DISP_NO_ERROR,
				 					    "Display Layer Get Zorder list position" );		

			for(j=0;j<BldCapatility[index].nbLayer;j++)
			{
				if(pstZorderLayerIDlist[j]!=pstZorderPositionlist[j].LayerId)
				{
					checkedOK=FALSE;
					break;
				}
			}
			
			 TestManager_AssertEqual(checkedOK,
									TRUE,
									"Blender ZOrder list set" );

		}
		

		
		/* Dimension get */
		 TestManager_AssertEqual( TDAL_DISP_BlenderDimensionsGet((tTDAL_DISP_BlenderId)index,&Width,&Height),
		 						    eTDAL_DISP_NO_ERROR,
		 						    "Display Blender Get dimension " );		
      		 mTBOX_PRINT(( kTBOX_LF, "Display Blender Dimension Get : [width: %d], [height: %d]\n",Width,Height));

		pWidth=200;
		pHeight=100;

		/* Dimension change */	
		 for(i=0;i<4;i++)
		 {
			 TestManager_AssertEqual( TDAL_DISP_BlenderDimensionsSet((tTDAL_DISP_BlenderId)index,pWidth,pHeight),
			 						    eTDAL_DISP_NO_ERROR,
			 						    "Display Blender Set dimension " );		
	      		 mTBOX_PRINT(( kTBOX_LF, "Display Blender dimension  Set : [width: %d], [height: %d]\n",pWidth,pHeight));
				
			 TestManager_AssertEqual( TDAL_DISP_BlenderDimensionsGet((tTDAL_DISP_BlenderId)index,&Width,&Height),
			 						    eTDAL_DISP_NO_ERROR,
			 						    "Display Blender Get dimension " );		
	      		 mTBOX_PRINT(( kTBOX_LF, "Display Blender dimension  Get : [width: %d], [height: %d]\n",Width,Height));

			if(pWidth==Width&&pHeight==Height)
			{
		      		 mTBOX_PRINT(( kTBOX_LF, "Display Blender dimension Set successfully!\n"));
			}

			else
			{
		      		 mTBOX_PRINT(( kTBOX_LF, "Display Blender dimension Set failed!\n"));
			}
			
			pWidth+=100;
			pHeight+=100;
			
		 }
	}

    TestManager_AssertEqual(TDAL_DISP_LayerDisable(eTDAL_DISP_LAYER_VIDEO_ID_0),
							eTDAL_DISP_NO_ERROR,
							" disp layer video disable" );	
	
    /* terminate tdal modules */
    TestManager_AssertEqual(TDAL_AV_Stop(eTDAL_AV_DECODER_VIDEO_1), eTDAL_AV_NO_ERROR,"av video audio");
    TestManager_AssertEqual(TDAL_AV_Terminate(), eTDAL_AV_NO_ERROR, "av terminate");
    TestHelper_Terminate_TDAL_DMD();
    TestHelper_Unconfigure_TDAL_DMX();
    TestHelper_Unconfigure_TDAL_TSROUTE(TEST_HELPER_TSROUTE_TUNER(gTDAL_AV_TestFeIdx),
                                        TEST_HELPER_TSROUTE_DEMUX(gTDAL_AV_TestDmxIdx));
    TestManager_AssertEqual(TDAL_TSROUTE_Term(),
							eTDAL_TSROUTE_NO_ERROR,
							"tsroute terminate");
#ifdef TDAL_HDMI_USE	
    TestManager_AssertEqual(TDAL_HDMI_Term(),
							eTDAL_HDMI_NO_ERROR,
							"hdmi terminate" );
#endif
    TestManager_AssertEqual(TDAL_OUTPUT_Term(),
							eTDAL_OUTPUT_NO_ERROR,
							"output terminate" );

    TestManager_AssertEqual(TDAL_DISP_Term(),
							eTDAL_DISP_NO_ERROR,
							"disp terminate" );

#ifdef AVS_STB
            TestManager_AssertEqual(TDAL_AVS_Term(),
                                    eTDAL_AVS_NO_ERROR,
                                    "avs terminate");
#endif


#ifdef PRODUCT_USE_CA
    TestManager_AssertEqual(TDAL_DESC_Terminate(),
							eTDAL_DESC_ERROR,
							"desc terminate");
#endif
    TestManager_AssertEqual(TDAL_DMX_Terminate(),
							kTDAL_DMX_NO_ERROR,
							"dmx terminate");	


}
#endif /* GS : the test must be reviewed */

LOCAL void MediaVideoCallback(tTDAL_AV_Decoder decoder, tTDAL_AV_Event event, const void *pEventData)
{
    switch(event)
    {
        case eTDAL_AV_EVENT_VIDEO_PICTURE_PLAYED:
        case eTDAL_AV_EVENT_VIDEO_PICTURE_STOPPED:
            break;

        default:
            mTBOX_PRINT((kTBOX_LF, "VideoCallback: unknown event (%d)\n", event));
            break;
    }
    return;
}

LOCAL void VideoNeedDisplayCallback(tTDAL_AV_Decoder decoder, tTDAL_AV_Event event, const void *pEventData)
{
    tTDAL_DISP_Error tdispErr;

    /*      Enable Video Layer      */
    tdispErr = TDAL_DISP_LayerEnable(eTDAL_DISP_LAYER_VIDEO_ID_0);
    TestManager_AssertEqual(tdispErr, eTDAL_DISP_NO_ERROR, "disp layer video enable");
}

LOCAL void VideoUserDataCallback(tTDAL_AV_Decoder decoder, tTDAL_AV_Event event, const void *pEventData)
{
}

LOCAL void VideoAspectRatioMpegCallback(tTDAL_AV_Decoder decoder, tTDAL_AV_Event event, const void *pEventData)
{
}

LOCAL void VideoResolutionCallback(tTDAL_AV_Decoder decoder, tTDAL_AV_Event event, const void *pEventData)
{
}

LOCAL void ScrambledStatusCallback(tTDAL_AV_Decoder decoder, tTDAL_AV_Event event, const void *pEventData)
{
}

LOCAL void MediaAudioCallback(tTDAL_AV_Decoder decoder, tTDAL_AV_Event event, const void *pEventData)
{
    switch(event)
    {
        case eTDAL_AV_EVENT_AUDIO_SAMPLE_PLAYED:
        case eTDAL_AV_EVENT_AUDIO_SAMPLE_STOPPED:
            break;

        default:
            mTBOX_PRINT((kTBOX_LF, "AudioCallback: unknown event (%d)\n", event));
            break;
    }
    return;
}

LOCAL void VideoEventsRegister(void)
{
    tTDAL_AV_Error tavErr;

    /* register the video callback needed for the picture start */
    tavErr = TDAL_AV_EventSubscribe(eTDAL_AV_DECODER_VIDEO_1, eTDAL_AV_EVENT_VIDEO_PICTURE_PLAYED, MediaVideoCallback);
    TestManager_AssertEqual(tavErr, eTDAL_AV_NO_ERROR, "av register video picture played");

    /* register the video callback needed for the picture stop */
    tavErr = TDAL_AV_EventSubscribe(eTDAL_AV_DECODER_VIDEO_1, eTDAL_AV_EVENT_VIDEO_PICTURE_STOPPED, MediaVideoCallback);
    TestManager_AssertEqual(tavErr, eTDAL_AV_NO_ERROR, "av register video picture stopped");

    /* register the video callback needed for the display */
    tavErr = TDAL_AV_EventSubscribe(eTDAL_AV_DECODER_VIDEO_1, eTDAL_AV_EVENT_VIDEO_NEED_DISPLAY, VideoNeedDisplayCallback);
    TestManager_AssertEqual(tavErr, eTDAL_AV_NO_ERROR, "av register video need display");

    /* register the video user data */
    tavErr = TDAL_AV_EventSubscribe(eTDAL_AV_DECODER_VIDEO_1, eTDAL_AV_EVENT_VIDEO_USER_DATA, VideoUserDataCallback);
    TestManager_AssertEqual(tavErr, eTDAL_AV_NO_ERROR, "av register video user data");

    /* register the video aspect ratio change */
    tavErr = TDAL_AV_EventSubscribe(eTDAL_AV_DECODER_VIDEO_1, eTDAL_AV_EVENT_VIDEO_ASPECT_RATIO_CHANGE, VideoAspectRatioMpegCallback);
    TestManager_AssertEqual(tavErr, eTDAL_AV_NO_ERROR, "av register video ar change");

    /* register the video resolution change */
    tavErr = TDAL_AV_EventSubscribe(eTDAL_AV_DECODER_VIDEO_1, eTDAL_AV_EVENT_VIDEO_RESOLUTION_CHANGE, VideoResolutionCallback);
    TestManager_AssertEqual(tavErr, eTDAL_AV_NO_ERROR, "av register video ar change");

    /* register the video user data */
    tavErr = TDAL_AV_EventSubscribe(eTDAL_AV_DECODER_VIDEO_1, eTDAL_AV_EVENT_VIDEO_TRP_SCRAMBLED_CHANGE, ScrambledStatusCallback);
    TestManager_AssertEqual(tavErr, eTDAL_AV_NO_ERROR, "av register video scramble change");
}

LOCAL void AudioEventsRegister(void)
{
    tTDAL_AV_Error tavErr;

    /* register the audio callback needed for the sample start */
    tavErr = TDAL_AV_EventSubscribe(eTDAL_AV_DECODER_AUDIO_1, eTDAL_AV_EVENT_AUDIO_SAMPLE_PLAYED, MediaAudioCallback);
    TestManager_AssertEqual(tavErr, eTDAL_AV_NO_ERROR, "av register audio sample played");

                /* register the audio callback needed for the sample stop */
    tavErr = TDAL_AV_EventSubscribe(eTDAL_AV_DECODER_AUDIO_1, eTDAL_AV_EVENT_AUDIO_SAMPLE_STOPPED, MediaAudioCallback);
    TestManager_AssertEqual(tavErr, eTDAL_AV_NO_ERROR, "av register audio sample stopped");
}

LOCAL void VideoEventsUnregister(void)
{
    tTDAL_AV_Error tavErr;

    /* unregister the video callback needed for the picture start */
    tavErr = TDAL_AV_EventUnsubscribe(eTDAL_AV_DECODER_VIDEO_1, eTDAL_AV_EVENT_VIDEO_PICTURE_PLAYED);
    TestManager_AssertEqual(tavErr, eTDAL_AV_NO_ERROR, "av unregister video picture played");

    /* unregister the video callback needed for the picture stop */
    tavErr = TDAL_AV_EventUnsubscribe(eTDAL_AV_DECODER_VIDEO_1, eTDAL_AV_EVENT_VIDEO_PICTURE_STOPPED);
    TestManager_AssertEqual(tavErr, eTDAL_AV_NO_ERROR, "av unregister video picture stopped");

    /* unregister the video callback needed for the display */
    tavErr = TDAL_AV_EventUnsubscribe(eTDAL_AV_DECODER_VIDEO_1, eTDAL_AV_EVENT_VIDEO_NEED_DISPLAY);
    TestManager_AssertEqual(tavErr, eTDAL_AV_NO_ERROR, "av unregister video need display");

    /* unregister the video user data */
    tavErr = TDAL_AV_EventUnsubscribe(eTDAL_AV_DECODER_VIDEO_1, eTDAL_AV_EVENT_VIDEO_USER_DATA);
    TestManager_AssertEqual(tavErr, eTDAL_AV_NO_ERROR, "av unregister video user data");

    /* unregister the video aspect ratio change */
    tavErr = TDAL_AV_EventUnsubscribe(eTDAL_AV_DECODER_VIDEO_1, eTDAL_AV_EVENT_VIDEO_ASPECT_RATIO_CHANGE);
    TestManager_AssertEqual(tavErr, eTDAL_AV_NO_ERROR, "av unregister video ar change");

    /* unregister the video resolution change */
    tavErr = TDAL_AV_EventUnsubscribe(eTDAL_AV_DECODER_VIDEO_1, eTDAL_AV_EVENT_VIDEO_RESOLUTION_CHANGE);
    TestManager_AssertEqual(tavErr, eTDAL_AV_NO_ERROR, "av unregister video ar change");

    /* unregister the video user data */
    tavErr = TDAL_AV_EventUnsubscribe(eTDAL_AV_DECODER_VIDEO_1, eTDAL_AV_EVENT_VIDEO_TRP_SCRAMBLED_CHANGE);
    TestManager_AssertEqual(tavErr, eTDAL_AV_NO_ERROR, "av unregister video scramble change");
}

LOCAL void AudioEventsUnregister(void)
{
    tTDAL_AV_Error tavErr;

    /* unregister the audio callback needed for the sample start */
    tavErr = TDAL_AV_EventUnsubscribe(eTDAL_AV_DECODER_AUDIO_1, eTDAL_AV_EVENT_AUDIO_SAMPLE_PLAYED);
    TestManager_AssertEqual(tavErr, eTDAL_AV_NO_ERROR, "av unregister audio sample played");

    /* unregister the audio callback needed for the sample stop */
    tavErr = TDAL_AV_EventUnsubscribe(eTDAL_AV_DECODER_AUDIO_1, eTDAL_AV_EVENT_AUDIO_SAMPLE_STOPPED);
    TestManager_AssertEqual(tavErr, eTDAL_AV_NO_ERROR, "av unregister audio sample stopped");
}


void    TestSce_DispLayer_Capability( void )
{
	tTDAL_DISP_Capability               DisCapatility;
	tTDAL_DISP_BlenderCapability        BldCapatility;
	tTDAL_DISP_LayerCapability          LayerCapability[5];
    tTDAL_DISP_BlenderZOrderPositions   pstZOrderList[5];
	uint8_t                             index;

	TestManager_AssertEqual( TDAL_DISP_Init(), eTDAL_DISP_NO_ERROR, "Display init" );

    TestManager_AssertEqual( TDAL_DISP_CapabilityGet(&DisCapatility),
	   						   eTDAL_DISP_NO_ERROR,
	   						   "Display Get capabilities" );
    mTBOX_PRINT(( kTBOX_LF, "Display capabilities : %d blenders\n", DisCapatility.nbBlender));

	TestManager_AssertEqual( TDAL_DISP_BlenderCapabilityGet(eTDAL_DISP_BLENDER_ID_0,&BldCapatility),
							   eTDAL_DISP_NO_ERROR,
							   "Display Blender Get capabilities" );		
    mTBOX_PRINT(( kTBOX_LF, "Display Blender capabilities : %d layers\n", BldCapatility.nbLayer));

	TestManager_AssertEqual( TDAL_DISP_BlenderZOrderPositionsGet(eTDAL_DISP_BLENDER_ID_0,pstZOrderList),
	 						   eTDAL_DISP_NO_ERROR,
	 						   "Display blender Get Z order positions" );		


	for(index=0;index< BldCapatility.nbLayer;index++)
	{
		TestManager_AssertEqual( TDAL_DISP_LayerCapabilityGet(pstZOrderList[index].LayerId,&LayerCapability[index]),
								   eTDAL_DISP_NO_ERROR,
								   "Display Layer Get capabilities" );		
      	        mTBOX_PRINT(( kTBOX_LF, "No. %d Display Layer capabilities[1:YES/0:NO] : %d Regions, IsMultiRegionOnSameLineCapable: %d; \nIsInputWindowCapable: %d; IsOutputWindowCapable:%d; IsGlobalTransparencyCapable:%d; IsAspectRatioWindowConvCapable %d;\n",
								index+1, LayerCapability[index].NbRegionMax,LayerCapability[index].IsMultiRegionOnSameLineCapable,LayerCapability[index].IsInputWindowCapable
								,LayerCapability[index].IsOutputWindowCapable,LayerCapability[index].IsGlobalTransparencyCapable,LayerCapability[index].IsAspectRatioWindowConvCapable));
		
	}

	TestManager_AssertEqual( TDAL_DISP_Term(), eTDAL_DISP_NO_ERROR, "Display term" );

}


void    TestSce_DispLayer_Disp_Management( void )
{
    tTDAL_GFX_Capabilities  Capabilities;	
    tTDAL_DISP_LayerWindow  stInputWindow;
    tTDAL_DISP_LayerWindow  stOutputWindow;
    tTDAL_AV_VideoPictureParams stPictureParams;
	tTDAL_AV_StreamType StreamType;
    tTDAL_GFX_RegionHandle  GfxHandle;
	uint32_t                LayerHandle;
    tTDAL_GFX_RegionConfig  Params;
    tTDAL_GFX_Color         PaletteColor;
    tTDAL_GFX_Palette       Palette;
    tTDAL_GFX_Color         PaletteColorTab[256];
	tTDAL_OUTPUT_VideoHDDACParams stHDDACParams;
	tTDAL_OUTPUT_Capability stOutputCap;
	uint32_t                Width, Height;
	
    Palette.nbColors = 256;
    Palette.pColor   = &PaletteColorTab[0];

    stPictureParams.notCompressWidth            = 0;
    stPictureParams.notCompressHeight           = 0;
    stPictureParams.notCompressDataBufferPosX   = 0;
    stPictureParams.notCompressDataBufferPosY   = 0;
    stPictureParams.notCompressDataBufferWidth  = 0;
    stPictureParams.notCompressDataBufferHeight = 0;


    /* Initialize tdal modules */
    TestHelper_Initialize_TDAL_DMD();
    TestManager_AssertEqual(TDAL_DMX_Init(),kTDAL_DMX_NO_ERROR,"dmx init");
#ifdef PRODUCT_USE_CA
    TestManager_AssertEqual(TDAL_DESC_Init(),eTDAL_DESC_NO_ERROR,"desc init");
#endif


#ifdef AVS_STB
	TestManager_AssertEqual(TDAL_AVS_Init(),eTDAL_AVS_NO_ERROR,"avs init");
	TestHelper_Configure_TDAL_AVS();
#endif

    TestManager_AssertEqual(TDAL_DISP_Init(),eTDAL_DISP_NO_ERROR,"disp init" );
    TestManager_AssertEqual(TDAL_OUTPUT_Init(),eTDAL_OUTPUT_NO_ERROR,"output init");

#ifdef TDAL_HDMI_USE
    TestManager_AssertEqual(TDAL_HDMI_Init(),eTDAL_HDMI_NO_ERROR,"hdmi init" );
#endif

	TestManager_AssertEqual(TDAL_OUTPUT_CapabilityGet(&stOutputCap),eTDAL_OUTPUT_NO_ERROR,"output caps");
	
	if(stOutputCap.nbVideoHDDAC > 0)
	{
		stHDDACParams.ColorSpace     = eTDAL_OUTPUT_VIDEO_HD_DAC_ITU601;
		stHDDACParams.Format         = eTDAL_OUTPUT_VIDEO_HD_DAC_YCBCR;
		stHDDACParams.Standard       = eTDAL_OUTPUT_VIDEO_HD_STD_1920_1080_I_50;
		stHDDACParams.EmbeddedSyncOn = FALSE;
		TestManager_AssertEqual(TDAL_OUTPUT_VideoHDDACParamsSet(eTDAL_OUTPUT_VIDEO_HD_DAC_ID_0,&stHDDACParams),
								eTDAL_OUTPUT_NO_ERROR,
								"output hd dac params set");
		TestManager_AssertEqual(TDAL_OUTPUT_Enable(eTDAL_OUTPUT_VIDEO_HD_DAC_ID_0),eTDAL_OUTPUT_NO_ERROR,"output hd dac enable");
	}

#ifdef TDAL_HDMI_USE
		if(stOutputCap.nbHDMI > 0)
		{
			TestManager_AssertEqual(TDAL_HDMI_Start(eTDAL_HDMI_SOURCE_ID_0,HDMICallback,FALSE),
									eTDAL_HDMI_NO_ERROR,
									"hdmi start" );
			
			TestManager_AssertEqual(TDAL_OUTPUT_Enable(eTDAL_OUTPUT_HDMI_ID_0),
									eTDAL_OUTPUT_NO_ERROR,
									"output HDMI video enable" );
	
		}
#endif

    TestManager_AssertEqual(TDAL_TSROUTE_Init(),eTDAL_TSROUTE_NO_ERROR,"tsroute init");
    TestManager_AssertEqual(TDAL_AV_Init(),eTDAL_AV_NO_ERROR,"av init");

    TestHelper_Configure_TDAL_TSROUTE(TEST_HELPER_TSROUTE_TUNER(gTDAL_AV_TestFeIdx),TEST_HELPER_TSROUTE_DEMUX(gTDAL_AV_TestDmxIdx));

    TestManager_AssertEqual(TDAL_GFX_Init(),eTDAL_GFX_NO_ERROR,"gfx init" );

	TestManager_AssertEqual(TDAL_DISP_BlenderDimensionsGet(eTDAL_DISP_BLENDER_ID_0,&Width,&Height),
							eTDAL_DISP_NO_ERROR,
							"DISP get blender dimensions" );
    VideoEventsRegister();
    AudioEventsRegister();

    /* display video */
    TestHelper_DMDTune(gTDAL_AV_TestFeIdx);
    TestHelper_Configure_TDAL_DMX(gTDAL_AV_TestDmxIdx);

    StreamType.videoType = eTDAL_AV_VIDEO_TYPE_MPEG2;

    TestManager_AssertEqual(TDAL_AV_InputStreamSet(eTDAL_AV_DECODER_VIDEO_1,TestHelper_DMXGetStreamHandle(eTDAL_AV_DECODER_VIDEO_1)),
							eTDAL_AV_NO_ERROR,
							"TDAL AV input stream set\n");
	
    TestManager_AssertEqual(TDAL_AV_Start(eTDAL_AV_DECODER_VIDEO_1,StreamType),eTDAL_AV_NO_ERROR,"av start video");

	if(stOutputCap.nbVideoDenc > 0)
	{
		TestManager_AssertEqual(TDAL_OUTPUT_Enable(eTDAL_OUTPUT_VIDEO_DENC_ID_0),eTDAL_OUTPUT_NO_ERROR,"output denc video enable" );
	}

	/* SD Video input */
	stInputWindow.Left   = 0;
	stInputWindow.Top    = 0;
	stInputWindow.Right  = 720 - 1;
	stInputWindow.Bottom = 576 - 1;

	stOutputWindow.Left   = 0;
	stOutputWindow.Top    = 0;
	stOutputWindow.Right  = Width - 1;
	stOutputWindow.Bottom = Height - 1;

	/* set video layer windows size */
	TestManager_AssertEqual(TDAL_DISP_LayerIOWindowsSet(eTDAL_DISP_LAYER_VIDEO_ID_0,&stInputWindow,&stOutputWindow),
							eTDAL_DISP_NO_ERROR,
							"disp set 1st video IOWindows" );

    /*enable layer video*/	
    TestManager_AssertEqual(TDAL_DISP_LayerEnable(eTDAL_DISP_LAYER_VIDEO_ID_0),
							eTDAL_DISP_NO_ERROR,
							" disp layer video enable" );

    TestManager_AskForChecking( "1. Display layer video enable, do you see video playing" );

	
     /* disable layer video */
    TestManager_AssertEqual(TDAL_DISP_LayerDisable(eTDAL_DISP_LAYER_VIDEO_ID_0),eTDAL_DISP_NO_ERROR," disp layer video disable" );


    TestManager_AskForChecking( "2. Display layer video disable, do you see video stop" );


	
    /* display still picture */
    TestManager_AssertEqual(TDAL_DISP_LayerEnable(eTDAL_DISP_LAYER_GFX_ID_0),eTDAL_DISP_NO_ERROR,"disp GFX enable" );

    TestManager_AssertEqual(TDAL_GFX_CapabilitiesGet(&Capabilities),eTDAL_GFX_NO_ERROR,"GFX get capabilities" );


    /* Set up the palette */
    TestHelper_GFX_FillPalette(&Palette);
	
	/* SD input GFX layer */
	stInputWindow.Left    = 0;
    stInputWindow.Top     = 0;
    stInputWindow.Right   = 720 - 1;
    stInputWindow.Bottom  = 576 - 1;
	stOutputWindow.Left   = 0;
    stOutputWindow.Top    = 0;
    stOutputWindow.Right  = Width - 1;
    stOutputWindow.Bottom = Height - 1;

    TestManager_AssertEqual(TDAL_DISP_LayerIOWindowsSet(eTDAL_DISP_LAYER_GFX_ID_0,&stInputWindow,&stOutputWindow),
							eTDAL_DISP_NO_ERROR,
							"GFX set image IOWindows" );

    /* Set up default params & create a GFX region */
    Params.offset.x    = 200;
    Params.offset.y    = 200;
    Params.size.width  = 400;
    Params.size.height = 300;
    Params.refSize.width  = 720;
    Params.refSize.height = 576;
    Params.colorType   = Palette.colorType;
    //Params.alpha       = 0; //transparent
    Params.alpha     = 120;
    printf("\n""\033[1;31m""[FUN][%s()@%04d] Need To Check: alpha:0 is transparent. Can't see rectangle on panel ""\033[m""\n", __FUNCTION__, __LINE__);
    printf("\n""\033[1;31m""[FUN][%s()@%04d] Need To Check: alpha:0 is transparent. Can't see rectangle on panel ""\033[m""\n", __FUNCTION__, __LINE__);

    TestManager_AssertEqual(TDAL_GFX_RegionCreate(&GfxHandle,&Params),
							eTDAL_GFX_NO_ERROR,
							"Create a GFX region" );

    TestManager_AssertEqual(TDAL_DISP_LayerHandleGet(eTDAL_DISP_LAYER_GFX_ID_0, &LayerHandle),
							eTDAL_DISP_NO_ERROR,
							"TDAL_DISP_LayerHandleGet" );

    TestManager_AssertEqual(TDAL_GFX_RegionDestinationSet(GfxHandle,LayerHandle),
							eTDAL_GFX_NO_ERROR,
							"TDAL_GFX_RegionDestinationSet" );


    TestManager_AssertEqual(TDAL_GFX_RegionShow(GfxHandle),
							eTDAL_GFX_NO_ERROR,
							"Show region" );

    TestManager_AssertEqual(TDAL_GFX_RegionPaletteSet(GfxHandle,&Palette),
							eTDAL_GFX_NO_ERROR,
							"Set the palette" );

    /* Draw 5 valid rectangles */
    Params.offset.x    = 0;
    Params.offset.y    = 0;

    PaletteColor.paletteEntry = 8;
    TestManager_AssertEqual(TDAL_GFX_RectangleDraw(GfxHandle,Params.offset,Params.size,Params.colorType,PaletteColor),
							eTDAL_GFX_NO_ERROR,
							"Draw a rectangle" );

    TestManager_AskForChecking("3.do you see rectangle" );

	/* disable still picture display */
    TestManager_AssertEqual(TDAL_DISP_LayerDisable(eTDAL_DISP_LAYER_GFX_ID_0),eTDAL_DISP_NO_ERROR," disp layer GFX disable" );
	

    TestManager_AskForChecking( "4.Display layer GFX disable, do you see OSD disappear" );

	
	
	/* enable video playing */
    TestManager_AssertEqual(TDAL_DISP_LayerEnable(eTDAL_DISP_LAYER_VIDEO_ID_0),eTDAL_DISP_NO_ERROR," disp layer video enable" );

    TestManager_AssertEqual(TDAL_DISP_LayerEnable(eTDAL_DISP_LAYER_GFX_ID_0),eTDAL_DISP_NO_ERROR," disp layer GFX enable" );

     /* Get video layer windows size */
    TestManager_AssertEqual(TDAL_DISP_LayerIOWindowsGet(eTDAL_DISP_LAYER_VIDEO_ID_0,&stInputWindow,&stOutputWindow),
							eTDAL_DISP_NO_ERROR,
							"disp get IOWindows" );

    stInputWindow.Left   = 0;
    stInputWindow.Top    = 0;
    stInputWindow.Right  = 400;
    stInputWindow.Bottom = 534;

    stOutputWindow.Left   = 200;
    stOutputWindow.Top    = 10;
    stOutputWindow.Right  = stInputWindow.Right + stOutputWindow.Left;
    stOutputWindow.Bottom = stInputWindow.Bottom + stOutputWindow.Top;

	/* set video layer windows size */
    TestManager_AssertEqual(TDAL_DISP_LayerIOWindowsSet(eTDAL_DISP_LAYER_VIDEO_ID_0,&stInputWindow,&stOutputWindow),
							eTDAL_DISP_NO_ERROR,
							"disp set 1st video IOWindows" );

    TestManager_AskForChecking("5. do you see the video size has been changed? " );

    stOutputWindow.Left   = 0;
    stOutputWindow.Top    = 0;
    stOutputWindow.Right  = Width - 1;
    stOutputWindow.Bottom = Height - 1;

	/* set video layer windows size --full screen*/
    TestManager_AssertEqual(TDAL_DISP_LayerIOWindowsSet(eTDAL_DISP_LAYER_VIDEO_ID_0,&stInputWindow,&stOutputWindow),
							eTDAL_DISP_NO_ERROR,
							"disp set 2nd video IOWindows" );

    TestManager_AskForChecking("6. do you see the video full screen? " );



     /* Get picture layer windows size */
    TestManager_AssertEqual(TDAL_DISP_LayerIOWindowsGet(eTDAL_DISP_LAYER_GFX_ID_0,&stInputWindow,&stOutputWindow),
							eTDAL_DISP_NO_ERROR,
							"disp get IOWindows" );

    stInputWindow.Left   = 0;
    stInputWindow.Top    = 0;
    stInputWindow.Right  = 400;
    stInputWindow.Bottom = 534;

    stOutputWindow.Left   = 200;
    stOutputWindow.Top    = 10;
    stOutputWindow.Right  = stInputWindow.Right + stOutputWindow.Left;
    stOutputWindow.Bottom = stInputWindow.Bottom + stOutputWindow.Top;
   

	/* set picture layer windows size */
    TestManager_AssertEqual(TDAL_DISP_LayerIOWindowsSet(eTDAL_DISP_LAYER_GFX_ID_0,&stInputWindow,&stOutputWindow),
							eTDAL_DISP_NO_ERROR,
							"disp set 1st picture IOWindows" );

    TestManager_AskForChecking("7. do you see the RGB picture size has been changed? " );

    stInputWindow.Left    = 0;
    stInputWindow.Top     = 0;
    stInputWindow.Right   = 720 - 1;
    stInputWindow.Bottom  = 576 - 1;
    stOutputWindow.Left   = 0;
    stOutputWindow.Top    = 0;
    stOutputWindow.Right  = Width - 1;
    stOutputWindow.Bottom = Height - 1;

	/* set picture layer windows size --full screen*/
    TestManager_AssertEqual(TDAL_DISP_LayerIOWindowsSet(eTDAL_DISP_LAYER_GFX_ID_0,&stInputWindow,&stOutputWindow),
							eTDAL_DISP_NO_ERROR,
							"disp set 2nd picture IOWindows" );

    TestManager_AskForChecking("8. do you see the RGB picture full screen? " );

    TestManager_AssertEqual(TDAL_GFX_RegionDelete(GfxHandle),eTDAL_DISP_NO_ERROR," Delete the GFX region" );

    TestManager_AssertEqual(TDAL_DISP_LayerDisable(eTDAL_DISP_LAYER_VIDEO_ID_0),eTDAL_DISP_NO_ERROR," disp layer video disable" );

    TestManager_AssertEqual(TDAL_DISP_LayerDisable(eTDAL_DISP_LAYER_GFX_ID_0),eTDAL_DISP_NO_ERROR," disp layer video disable" );

	
    /* terminate tdal modules */
    TestManager_AssertEqual(TDAL_AV_Stop(eTDAL_AV_DECODER_VIDEO_1), eTDAL_AV_NO_ERROR,"av video stop");
    TestManager_AssertEqual(TDAL_AV_Terminate(), eTDAL_AV_NO_ERROR, "av terminate");
    TestHelper_Terminate_TDAL_DMD();
    TestHelper_Unconfigure_TDAL_DMX();
    TestHelper_Unconfigure_TDAL_TSROUTE(TEST_HELPER_TSROUTE_TUNER(gTDAL_AV_TestFeIdx),TEST_HELPER_TSROUTE_DEMUX(gTDAL_AV_TestDmxIdx));
    TestManager_AssertEqual(TDAL_TSROUTE_Term(),eTDAL_TSROUTE_NO_ERROR,"tsroute terminate");

    TestManager_AssertEqual(TDAL_GFX_Terminate(),eTDAL_GFX_NO_ERROR,"gfx terminate" );
#ifdef TDAL_HDMI_USE	
    TestManager_AssertEqual(TDAL_HDMI_Term(),eTDAL_HDMI_NO_ERROR,"hdmi terminate" );
#endif
    TestManager_AssertEqual(TDAL_OUTPUT_Term(),eTDAL_OUTPUT_NO_ERROR,"output terminate" );

    TestManager_AssertEqual(TDAL_DISP_Term(),eTDAL_DISP_NO_ERROR,"disp terminate" );

#ifdef AVS_STB
	TestManager_AssertEqual(TDAL_AVS_Term(),eTDAL_AVS_NO_ERROR,"avs terminate");
#endif


#ifdef PRODUCT_USE_CA
    TestManager_AssertEqual(TDAL_DESC_Terminate(),eTDAL_DESC_ERROR,"desc terminate");
#endif
	TestManager_AssertEqual(TDAL_DMX_Terminate(),kTDAL_DMX_NO_ERROR,"dmx terminate");

    VideoEventsUnregister();
    AudioEventsUnregister();


}


void    TestSce_DispLayer_Supported_Capability( void )
{
    tTDAL_DISP_LayerWindow      stInputWindow,pstInputWindow;
    tTDAL_DISP_LayerWindow      stOutputWindow,pstOutputWindow;
    tTDAL_DISP_LayerCapability  LayerCapability;
    bool                        adjuested;
    tTDAL_AV_StreamType         StreamType;
	tTDAL_OUTPUT_VideoHDDACParams stHDDACParams;
	tTDAL_OUTPUT_Capability       stOutputCap;
	
    stInputWindow.Left   = 0;
    stInputWindow.Top    = 0;
    stInputWindow.Right  = 400;
    stInputWindow.Bottom = 534;

    stOutputWindow.Left   = 200;
    stOutputWindow.Top    = 10;
    stOutputWindow.Right  = stInputWindow.Right + stOutputWindow.Left;
    stOutputWindow.Bottom = stInputWindow.Bottom + stOutputWindow.Top;

    pstInputWindow=stInputWindow;
    pstOutputWindow=stOutputWindow;


    TestHelper_Initialize_TDAL_DMD();
    TestManager_AssertEqual(TDAL_DMX_Init(),kTDAL_DMX_NO_ERROR,"dmx init");
#ifdef PRODUCT_USE_CA
    TestManager_AssertEqual(TDAL_DESC_Init(),eTDAL_DESC_ERROR,"desc init");
#endif


#ifdef AVS_STB
	TestManager_AssertEqual(TDAL_AVS_Init(),eTDAL_AVS_NO_ERROR,"avs init");
	TestHelper_Configure_TDAL_AVS();
#endif


    TestManager_AssertEqual(TDAL_DISP_Init(),eTDAL_DISP_NO_ERROR,"disp init" );

    TestManager_AssertEqual(TDAL_OUTPUT_Init(),eTDAL_OUTPUT_NO_ERROR,"output init" );
#ifdef TDAL_HDMI_USE
    TestManager_AssertEqual(TDAL_HDMI_Init(),eTDAL_HDMI_NO_ERROR,"hdmi init" );
#endif

    TestManager_AssertEqual(TDAL_OUTPUT_CapabilityGet(&stOutputCap),eTDAL_OUTPUT_NO_ERROR,"output caps");
	
	if(stOutputCap.nbVideoHDDAC > 0)
	{
		stHDDACParams.ColorSpace     = eTDAL_OUTPUT_VIDEO_HD_DAC_ITU601;
		stHDDACParams.Format         = eTDAL_OUTPUT_VIDEO_HD_DAC_YCBCR;
		stHDDACParams.Standard       = eTDAL_OUTPUT_VIDEO_HD_STD_1920_1080_I_50;
		stHDDACParams.EmbeddedSyncOn = FALSE;
		TestManager_AssertEqual(TDAL_OUTPUT_VideoHDDACParamsSet(eTDAL_OUTPUT_VIDEO_HD_DAC_ID_0,&stHDDACParams),
								eTDAL_OUTPUT_NO_ERROR,
								"output hd dac params set");
		TestManager_AssertEqual(TDAL_OUTPUT_Enable(eTDAL_OUTPUT_VIDEO_HD_DAC_ID_0),
								eTDAL_OUTPUT_NO_ERROR,
								"output hd dac enable");
	}

#ifdef TDAL_HDMI_USE
	if(stOutputCap.nbHDMI > 0)
	{
		TestManager_AssertEqual(TDAL_HDMI_Start(eTDAL_HDMI_SOURCE_ID_0,HDMICallback,FALSE),eTDAL_HDMI_NO_ERROR,"hdmi start" );
		
		TestManager_AssertEqual(TDAL_OUTPUT_Enable(eTDAL_OUTPUT_HDMI_ID_0),eTDAL_OUTPUT_NO_ERROR,"output HDMI video enable" );

	}
#endif

    TestManager_AssertEqual(TDAL_TSROUTE_Init(),eTDAL_TSROUTE_NO_ERROR,"tsroute init");
    TestManager_AssertEqual(TDAL_AV_Init(),eTDAL_AV_NO_ERROR,"av init");

    TestHelper_Configure_TDAL_TSROUTE(TEST_HELPER_TSROUTE_TUNER(gTDAL_AV_TestFeIdx),TEST_HELPER_TSROUTE_DEMUX(gTDAL_AV_TestDmxIdx));

    TestHelper_DMDTune(gTDAL_AV_TestFeIdx);
    TestHelper_Configure_TDAL_DMX(gTDAL_AV_TestDmxIdx);

	/*Layer capability get*/
    TestManager_AssertEqual(TDAL_DISP_LayerCapabilityGet(eTDAL_DISP_LAYER_VIDEO_ID_0,&LayerCapability),
							eTDAL_DISP_NO_ERROR,
							"Display Layer Get capabilities" );		
    mTBOX_PRINT(( kTBOX_LF, "Display Layer capabilities[1:YES/0:NO] : %d Regions, IsMultiRegionOnSameLineCapable: %d; \nIsInputWindowCapable: %d; IsOutputWindowCapable:%d;\n IsGlobalTransparencyCapable:%d; IsAspectRatioWindowConvCapable %d;\n",
				 LayerCapability.NbRegionMax,LayerCapability.IsMultiRegionOnSameLineCapable,LayerCapability.IsInputWindowCapable
				,LayerCapability.IsOutputWindowCapable,LayerCapability.IsGlobalTransparencyCapable,LayerCapability.IsAspectRatioWindowConvCapable));

    StreamType.videoType = eTDAL_AV_VIDEO_TYPE_MPEG2;
    VideoEventsRegister();
    AudioEventsRegister();

    TestManager_AssertEqual(TDAL_AV_InputStreamSet(eTDAL_AV_DECODER_VIDEO_1,TestHelper_DMXGetStreamHandle(eTDAL_AV_DECODER_VIDEO_1)),
							eTDAL_AV_NO_ERROR,
							"TDAL AV input stream set\n");
	
    TestManager_AssertEqual(TDAL_AV_Start(eTDAL_AV_DECODER_VIDEO_1,StreamType),
							eTDAL_AV_NO_ERROR,
							"av start video");

	if(stOutputCap.nbVideoDenc > 0)
	{
		TestManager_AssertEqual(TDAL_OUTPUT_Enable(eTDAL_OUTPUT_VIDEO_DENC_ID_0),
								eTDAL_OUTPUT_NO_ERROR,
								"output denc video enable" );
	}

    /*enable layer video*/	
    TestManager_AssertEqual(TDAL_DISP_LayerEnable(eTDAL_DISP_LAYER_VIDEO_ID_0),
							eTDAL_DISP_NO_ERROR,
							" disp layer video enable" );
	
	/* Set GlobalTransparency*/
    if(LayerCapability.IsGlobalTransparencyCapable)
    {
	     TestManager_AssertEqual( TDAL_DISP_LayerGlobalTransparencySet(
		 						eTDAL_DISP_LAYER_VIDEO_ID_0,0),
		 						eTDAL_DISP_NO_ERROR,
		 						"Display layer GlobalTransparency set to 0" );	
		 						
	     TestManager_AskForChecking("GlobalTransparencySet to 0 " );


	     TestManager_AssertEqual( TDAL_DISP_LayerGlobalTransparencySet(
		 						eTDAL_DISP_LAYER_VIDEO_ID_0,130),
		 						eTDAL_DISP_NO_ERROR,
		 						"Display layer GlobalTransparency set to 130" );	
		 						
	     TestManager_AskForChecking("GlobalTransparencySet to 130 " );
		 				
		 						
	     TestManager_AssertEqual( TDAL_DISP_LayerGlobalTransparencySet(
		 						eTDAL_DISP_LAYER_VIDEO_ID_0,255),
		 						eTDAL_DISP_NO_ERROR,
		 						"Display layer GlobalTransparency set to 255" );	

	     TestManager_AskForChecking("GlobalTransparencySet to 255 " );

    }

    else
    {
	     mTBOX_PRINT(( kTBOX_LF, "Layer GlobalTransparencyCapable set not supported\n"));
    }

	/* Set AspectRatio*/
    if(LayerCapability.IsAspectRatioWindowConvCapable)
    {
	     TestManager_AssertEqual( TDAL_DISP_LayerIOWindowsAspectRatioParamsSet(
		 						eTDAL_DISP_LAYER_VIDEO_ID_0,&stInputWindow,eTDAL_DISP_ASPECT_RATIO_4_3,3,eTDAL_DISP_ASPECT_RATIO_CONV_14_9_LETTER_BOX),
		 						eTDAL_DISP_NO_ERROR,
		 						"Display layer AspectRatio set to 14_9_LETTER_BOX" );	

	     TestManager_AskForChecking("IOWindowsAspectRatioParamsSet to 14_9_LETTER_BOX " );

	     TestManager_AssertEqual( TDAL_DISP_LayerIOWindowsAspectRatioParamsSet(
		 						eTDAL_DISP_LAYER_VIDEO_ID_0,&stInputWindow,eTDAL_DISP_ASPECT_RATIO_4_3,4,eTDAL_DISP_ASPECT_RATIO_CONV_16_9_LETTER_BOX),
		 						eTDAL_DISP_NO_ERROR,
		 						"Display layer AspectRatio set to 16_9_LETTER_BOX" );	

	     TestManager_AskForChecking("IOWindowsAspectRatioParamsSet to 14_9_LETTER_BOX " );

	     TestManager_AssertEqual( TDAL_DISP_LayerIOWindowsAspectRatioParamsSet(
		 						eTDAL_DISP_LAYER_VIDEO_ID_0,&stInputWindow,eTDAL_DISP_ASPECT_RATIO_4_3,9,eTDAL_DISP_ASPECT_RATIO_CONV_4_3_CENTER_CUT_OUT),
		 						eTDAL_DISP_NO_ERROR,
		 						"Display layer AspectRatio set to 4_3_CENTER_CUT_OUT" );	

	     TestManager_AskForChecking("IOWindowsAspectRatioParamsSet to 4_3_CENTER_CUT_OUT " );

    }

    else
    {
	     mTBOX_PRINT(( kTBOX_LF, "Layer AspectRatio set not supported\n"));
    }

	/* Set InputWindow*/
    if(LayerCapability.IsInputWindowCapable)
    {
	     TestManager_AssertEqual(TDAL_DISP_LayerIOWindowsAdjust(eTDAL_DISP_LAYER_VIDEO_ID_0,&stInputWindow,&stOutputWindow,&adjuested),
								eTDAL_DISP_NO_ERROR,
								"Display layer IO windows adjust" );	
	    if(adjuested)
    	{
			if(stInputWindow.Bottom==pstInputWindow.Bottom
				&& stInputWindow.Left==pstInputWindow.Left
				&& stInputWindow.Right==pstInputWindow.Right
				&& stInputWindow.Top==pstInputWindow.Top
				&& stOutputWindow.Bottom==pstOutputWindow.Bottom
				&& stOutputWindow.Left==pstOutputWindow.Left
				&& stOutputWindow.Right==pstOutputWindow.Right
				&& stOutputWindow.Top==pstOutputWindow.Top)
			{
				mTBOX_PRINT(( kTBOX_LF, "Layer IO window set failed, not take into account"));

			}
		}

		else
		{
			mTBOX_PRINT(( kTBOX_LF, "Layer IO window not adjust\n"));
		}
    }

    else
    {
		mTBOX_PRINT(( kTBOX_LF, "Display layer IO windows adjust not supported"));
    }


	TestManager_AskForChecking(" do you see video on screen ? " );
    TestManager_AssertEqual(TDAL_DISP_LayerDisable(eTDAL_DISP_LAYER_VIDEO_ID_0),eTDAL_DISP_NO_ERROR," disp layer video disable" );
	
    /* terminate tdal modules */
    TestManager_AssertEqual(TDAL_AV_Stop(eTDAL_AV_DECODER_VIDEO_1), eTDAL_AV_NO_ERROR,"av video audio");
    TestManager_AssertEqual(TDAL_AV_Terminate(), eTDAL_AV_NO_ERROR, "av terminate");
    TestHelper_Terminate_TDAL_DMD();
    TestHelper_Unconfigure_TDAL_DMX();
    TestHelper_Unconfigure_TDAL_TSROUTE(TEST_HELPER_TSROUTE_TUNER(gTDAL_AV_TestFeIdx),TEST_HELPER_TSROUTE_DEMUX(gTDAL_AV_TestDmxIdx));
    TestManager_AssertEqual(TDAL_TSROUTE_Term(),eTDAL_TSROUTE_NO_ERROR,"tsroute terminate");
#ifdef TDAL_HDMI_USE	
    TestManager_AssertEqual(TDAL_HDMI_Term(),eTDAL_HDMI_NO_ERROR,"hdmi terminate" );
#endif
    TestManager_AssertEqual(TDAL_OUTPUT_Term(),eTDAL_OUTPUT_NO_ERROR,"output terminate" );
    TestManager_AssertEqual(TDAL_DISP_Term(),eTDAL_DISP_NO_ERROR,"disp terminate" );

#ifdef AVS_STB
	TestManager_AssertEqual(TDAL_AVS_Term(),eTDAL_AVS_NO_ERROR,"avs terminate");
#endif


#ifdef PRODUCT_USE_CA
    TestManager_AssertEqual(TDAL_DESC_Terminate(),eTDAL_DESC_ERROR,"desc terminate");
#endif
    TestManager_AssertEqual(TDAL_DMX_Terminate(),kTDAL_DMX_NO_ERROR,"dmx terminate");

    VideoEventsUnregister();
    AudioEventsUnregister();

}
