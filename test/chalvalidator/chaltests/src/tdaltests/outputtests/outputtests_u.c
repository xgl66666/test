/** \file avtests_u.c
 *  \brief Scenario Tests for the OUTPUT API
 *  \date 11/07/06
 *  \version 1.0
 */

/********************************************************/
/*              Includes                                */
/********************************************************/
#include "crules.h"
#include <string.h>
#include <stdio.h>

#include "tkel.h"
#include "tbox.h"

#include "tdal_av.h"
#include "tdal_dmx.h"
/*#include "tdal_tsin.h"*/
#include "tdal_dmd.h"
#include "tdal_output.h"
#include "tdal_disp.h"

#include "tdal_hdmi.h"

#include "dmdtests.h"

#include "testhelper.h"
#include "TestManager.h"

#ifdef PRODUCT_USE_CA
#include "tdal_desc.h"
#endif

void TestCase_ExecuteUnitOutputInit(void);
void TestCase_ExecuteUnitOutputCapability(void);
void TestCase_ExecuteUnitOutputDENC(void);
void TestCase_ExecuteUnitOutputHD(void);
#if defined(TDAL_HDMI_USE)
void TestCase_ExecuteUnitOutputHDMI(void);
#endif
void TestCase_ExecuteUnitOutputAudioAna(void);
void TestCase_ExecuteUnitOutputAudioSPDIF(void);

static tTestNode gExecuteUnitOutputInit = {
    "ExecuteUnitInit",
    "test Init and terminate",
    0, /* not interactive */
    TestCase_ExecuteUnitOutputInit,
        NULL
};
static tTestNode gExecuteUnitOutputCapability = {
    "ExecuteUnitCapability",
    "test Capability Get",
    0, /* not interactive */
    TestCase_ExecuteUnitOutputCapability,
        NULL
};

static tTestNode gExecuteUnitOutputDENC = {
    "ExecuteUnitOutputDENC",
    "output Denc test",
    0, /* not interactive */
    TestCase_ExecuteUnitOutputDENC,
        NULL
};

static tTestNode gExecuteUnitOutputHD = {
    "ExecuteUnitOutputHD",
    "output HD test",
    0, /* not interactive */
    TestCase_ExecuteUnitOutputHD,
        NULL
};

#if defined(TDAL_HDMI_USE)
static tTestNode gExecuteUnitOutputHDMI = {
    "ExecuteUnitOutputHDMI",
    "output HDMI test",
    0, /* not interactive */
    TestCase_ExecuteUnitOutputHDMI,
        NULL
};
#endif

static tTestNode gExecuteUnitOutputAudioAna = {
    "ExecuteUnitOutputAudioAna",
    "output Audio Ana test",
    0, /* not interactive */
    TestCase_ExecuteUnitOutputAudioAna,
        NULL
};

static tTestNode gExecuteUnitOutputAudioSPDIF = {
    "ExecuteUnitOutputAudioSPDIF",
    "output Audio SPDIF test",
    0, /* not interactive */
    TestCase_ExecuteUnitOutputAudioSPDIF,
        NULL
};

/*! \var tTestNode* gTDAL_AVTestNodeList[]
 *  \brief Node for A/V tests
 *
 *  This node references available tests for
 *  TDAL_AV tests. For the moment only some
 *  scenario tests are available
 */
tTestNode* gTDAL_OUTPUTUnitaryTestNodeList[] = {
    &gExecuteUnitOutputInit,
    &gExecuteUnitOutputCapability,
    &gExecuteUnitOutputDENC,
    &gExecuteUnitOutputHD,
#if defined(TDAL_HDMI_USE)
    &gExecuteUnitOutputHDMI,
#endif
    &gExecuteUnitOutputAudioAna,
    &gExecuteUnitOutputAudioSPDIF,
    NULL
};

tTestNode gTDAL_OUTPUT_UnitaryTestSuite = {
    "TDAL_OUTPUT Unitary Tests",
    "test the TDAL_DISP function input parameters",
    0, /* not interactive */
    NULL, /* no function, it's a test suite */
    (void**)gTDAL_OUTPUTUnitaryTestNodeList
};

extern void HDMICallback(tTDAL_HDMI_SourceId Id, tTDAL_HDMI_Event Event);

/*! \fn void TestCase_ExecuteUnitInit( void )
 *  \brief This function performs unit tests of init and close
 */
void TestCase_ExecuteUnitOutputInit()
{

    TestManager_AssertEqual(TDAL_DISP_Init(),
							eTDAL_DISP_NO_ERROR,
							"disp init" );


   TestManager_AssertEqual( TDAL_OUTPUT_Init(), eTDAL_OUTPUT_NO_ERROR, "output init" );

   TestManager_AssertEqual( TDAL_OUTPUT_Term(), eTDAL_OUTPUT_NO_ERROR, "output term" );

     TestManager_AssertEqual(TDAL_DISP_Term(),
							eTDAL_DISP_NO_ERROR,
							"disp terminate" );

}

/*! \fn void TestCase_ExecuteUnitCapability( void )
 *  \brief This function performs unit tests of Capability Get
 */
void TestCase_ExecuteUnitOutputCapability()
{
   tTDAL_OUTPUT_Capability Capability;

    TestManager_AssertEqual(TDAL_DISP_Init(),
							eTDAL_DISP_NO_ERROR,
							"disp init" );

   TestManager_AssertEqual( TDAL_OUTPUT_Init(), eTDAL_OUTPUT_NO_ERROR, "output init" );

   TestManager_AssertEqual( TDAL_OUTPUT_CapabilityGet(&Capability), eTDAL_OUTPUT_NO_ERROR, "output capabilities get" );

   TestManager_AssertEqual( TDAL_OUTPUT_Term(), eTDAL_OUTPUT_NO_ERROR, "output term" );


     TestManager_AssertEqual(TDAL_DISP_Term(),
							eTDAL_DISP_NO_ERROR,
							"disp terminate" );

}

void TestCase_ExecuteUnitOutputDENC(void)
{
	tTDAL_OUTPUT_Capability pstCapability;
	tTDAL_OUTPUT_VideoDencCapability DencCapability;
	tTDAL_OUTPUT_VideoDencBCSParams Bcs;

    TestManager_AssertEqual(TDAL_DISP_Init(),
							eTDAL_DISP_NO_ERROR,
							"disp init" );

   TestManager_AssertEqual( TDAL_OUTPUT_Init(), eTDAL_OUTPUT_NO_ERROR, "output init" );

   TestManager_AssertEqual( TDAL_OUTPUT_CapabilityGet(&pstCapability),
   						   eTDAL_OUTPUT_NO_ERROR,
   						   "Output Get capabilities" );

	if(pstCapability.nbVideoDenc!=0)
	{
		     TestManager_AssertEqual( TDAL_OUTPUT_VideoDencCapabilityGet((tTDAL_OUTPUT_OutputId)(pstCapability.nbVideoDenc-1),&DencCapability),
									   eTDAL_OUTPUT_NO_ERROR,
									   "Output videoDENC Get capabilities" );



		    TestManager_AssertEqual( TDAL_OUTPUT_VideoDencColorBarControl(eTDAL_OUTPUT_VIDEO_DENC_ID_0,1),
			   						 eTDAL_OUTPUT_NO_ERROR,
			   					       "Output videodenc color bar enable" );

		    if(DencCapability.FormatSupportedMask&eTDAL_OUTPUT_VIDEO_DENC_FORMAT_YC)
		    {
			    TestManager_AssertEqual(TDAL_OUTPUT_VideoDencFormatSet
										(eTDAL_OUTPUT_VIDEO_DENC_ID_0,eTDAL_OUTPUT_VIDEO_DENC_FORMAT_YC),
										eTDAL_OUTPUT_NO_ERROR,
										"output videodenc format set" );

		    }

		     else
		    {
			    TestManager_AssertEqual(TDAL_OUTPUT_VideoDencFormatSet
										(eTDAL_OUTPUT_VIDEO_DENC_ID_0,eTDAL_OUTPUT_VIDEO_DENC_FORMAT_YC),
										eTDAL_OUTPUT_NOT_AVAILABLE_ERROR,
										"output videodenc format set: not available error" );

		     }

		    if(DencCapability.StandardSupportedMask&eTDAL_OUTPUT_VIDEO_SD_STD_PAL_BG)
		    {
			    TestManager_AssertEqual(TDAL_OUTPUT_VideoDencStandardSet
										(eTDAL_OUTPUT_VIDEO_DENC_ID_0,eTDAL_OUTPUT_VIDEO_SD_STD_PAL_BG),
										eTDAL_OUTPUT_NO_ERROR,
										"output videodenc standard set" );

		    }


		     else
		    {
			    TestManager_AssertEqual(TDAL_OUTPUT_VideoDencStandardSet
										(eTDAL_OUTPUT_VIDEO_DENC_ID_0,eTDAL_OUTPUT_VIDEO_SD_STD_PAL_BG),
										eTDAL_OUTPUT_NOT_AVAILABLE_ERROR,
										"output videodenc standard set not available error" );

		     }

		    Bcs.Brigthness=255;
		    Bcs.Contrast=100;
		    Bcs.Saturation=100;

		    if(DencCapability.IsBrigthnessSupported)
		    {
			    TestManager_AssertEqual(TDAL_OUTPUT_VideoDencBCSParamsSet
										(eTDAL_OUTPUT_VIDEO_DENC_ID_0,&Bcs),
										eTDAL_OUTPUT_NO_ERROR,
										"output videodenc BCS set" );
		    }

	}

	else
	{
	     mTBOX_PRINT(( kTBOX_LF, "DENC output not supported\n"));

	}

   TestManager_AssertEqual( TDAL_OUTPUT_Term(), eTDAL_OUTPUT_NO_ERROR, "output term" );

     TestManager_AssertEqual(TDAL_DISP_Term(),
							eTDAL_DISP_NO_ERROR,
							"disp terminate" );

}

void TestCase_ExecuteUnitOutputHD(void)
{
	tTDAL_OUTPUT_Capability pstCapability;
	tTDAL_OUTPUT_VideoHDDACCapability HDCapability;
	tTDAL_OUTPUT_VideoHDDACParams HDParams;

		HDParams.ColorSpace     = eTDAL_OUTPUT_VIDEO_HD_DAC_ITU601;
		HDParams.Format         = eTDAL_OUTPUT_VIDEO_HD_DAC_YCBCR;
		HDParams.Standard       = eTDAL_OUTPUT_VIDEO_HD_STD_1920_1080_I_50;
		HDParams.EmbeddedSyncOn = FALSE;

    TestManager_AssertEqual(TDAL_DISP_Init(),
							eTDAL_DISP_NO_ERROR,
							"disp init" );

	TestManager_AssertEqual( TDAL_OUTPUT_Init(), eTDAL_OUTPUT_NO_ERROR, "Output init" );
       TestManager_AssertEqual( TDAL_OUTPUT_CapabilityGet(&pstCapability),
	   						   eTDAL_OUTPUT_NO_ERROR,
	   						   "Output Get capabilities" );

	if(pstCapability.nbVideoHDDAC!=0)
	{

	       TestManager_AssertEqual( TDAL_OUTPUT_VideoHDDACCapabilityGet(eTDAL_OUTPUT_VIDEO_HD_DAC_ID_0,&HDCapability),
								   eTDAL_OUTPUT_NO_ERROR,
								   "Output videoHD Get capabilities" );

	       TestManager_AssertEqual(TDAL_OUTPUT_VideoHDDACParamsSet
								(eTDAL_OUTPUT_VIDEO_HD_DAC_ID_0,&HDParams),
								eTDAL_OUTPUT_NO_ERROR,
								"output videoHD params set " );
	}

	else
	{
	     mTBOX_PRINT(( kTBOX_LF, "HDDAC output not supported\n"));

	}

   TestManager_AssertEqual( TDAL_OUTPUT_Term(), eTDAL_OUTPUT_NO_ERROR, "output term" );

     TestManager_AssertEqual(TDAL_DISP_Term(),
							eTDAL_DISP_NO_ERROR,
							"disp terminate" );

}

#if defined(TDAL_HDMI_USE)
void TestCase_ExecuteUnitOutputHDMI(void)
{
	tTDAL_OUTPUT_Capability pstCapability;
	tTDAL_OUTPUT_HDMICapability HDMICapability;

	tTDAL_OUTPUT_HDMIParams     stHdmiParams;

	stHdmiParams.AudioMode     = eTDAL_OUTPUT_AUDIO_DIGITAL_MODE_UNCOMPRESSED;
	stHdmiParams.VideoFormat   = eTDAL_OUTPUT_VIDEO_HDMI_RGB888;
	stHdmiParams.VideoStandard = eTDAL_OUTPUT_VIDEO_HD_STD_1920_1080_I_50;


    /*      TDAL AV      */
    TestManager_AssertEqual(TDAL_AV_Init(), eTDAL_AV_NO_ERROR, "av init");

    /*      TDAL DISP      */
    TestManager_AssertEqual(TDAL_DISP_Init(), eTDAL_DISP_NO_ERROR, "disp init");

    /*      TDAL OUTPUT      */
    TestManager_AssertEqual(TDAL_OUTPUT_Init(), eTDAL_OUTPUT_NO_ERROR, "output init");


    TestManager_AssertEqual(TDAL_HDMI_Init(),
							eTDAL_HDMI_NO_ERROR,
							"hdmi init" );

       TestManager_AssertEqual( TDAL_OUTPUT_CapabilityGet(&pstCapability),
	   						   eTDAL_OUTPUT_NO_ERROR,
	   						   "Output Get capabilities" );


	if(pstCapability.nbHDMI!=0)
	{
        TestManager_AssertEqual(TDAL_HDMI_Start(eTDAL_HDMI_SOURCE_ID_0, HDMICallback, FALSE),
								eTDAL_HDMI_NO_ERROR,
								"hdmi start" );

		TestManager_AssertEqual(TDAL_OUTPUT_Enable(eTDAL_OUTPUT_HDMI_ID_0),
                                eTDAL_OUTPUT_NO_ERROR,
                                "output HDMI video enable" );

        TKEL_DelayTask(ms2tick(3000));

	     TestManager_AssertEqual( TDAL_OUTPUT_HDMICapabilityGet(eTDAL_OUTPUT_HDMI_ID_0,&HDMICapability),
								   eTDAL_OUTPUT_NO_ERROR,
								   "Output videoHDMI Get capabilities" );

	      TestManager_AssertEqual(TDAL_OUTPUT_HDMIParamsSet
								(eTDAL_OUTPUT_HDMI_ID_0,&stHdmiParams),
								eTDAL_OUTPUT_NO_ERROR,
								"output videoHDMI params set " );
	}

	else
	{
	     mTBOX_PRINT(( kTBOX_LF, "HDMI output not supported\n"));

	}

    TestManager_AssertEqual(TDAL_HDMI_Stop(eTDAL_HDMI_SOURCE_ID_0),
							eTDAL_HDMI_NO_ERROR,
							"hdmi stop" );

    TestManager_AssertEqual(TDAL_AV_Terminate(), eTDAL_AV_NO_ERROR, "av terminate");

   TestManager_AssertEqual( TDAL_OUTPUT_Term(), eTDAL_OUTPUT_NO_ERROR, "output term" );

   TestManager_AssertEqual( TDAL_HDMI_Term(), eTDAL_OUTPUT_NO_ERROR, "hdmi term" );

     TestManager_AssertEqual(TDAL_DISP_Term(),
							eTDAL_DISP_NO_ERROR,
							"disp terminate" );

}
#endif

void TestCase_ExecuteUnitOutputAudioAna(void)
{
	tTDAL_OUTPUT_Capability pstCapability;
	tTDAL_OUTPUT_AudioAnaCapability AudioAnaCapability;
	tTDAL_OUTPUT_AudioAnaSpeakerAttenuation pstAttenuation;

	pstAttenuation.FrontCenterAttenuation=0;
	pstAttenuation.FrontRLAttenuation=0;
	pstAttenuation.LFEAttenuation=0;
	pstAttenuation.MixLevel=0;
	pstAttenuation.RearCenterAttenuation=0;
	pstAttenuation.RearRLAttenuation=0;
	pstAttenuation.SurroundRLAttenuation=0;

    TestManager_AssertEqual(TDAL_DISP_Init(),
							eTDAL_DISP_NO_ERROR,
							"disp init" );

	TestManager_AssertEqual( TDAL_OUTPUT_Init(), eTDAL_OUTPUT_NO_ERROR, "Output init" );
       TestManager_AssertEqual( TDAL_OUTPUT_CapabilityGet(&pstCapability),
	   						   eTDAL_OUTPUT_NO_ERROR,
	   						   "Output Get capabilities" );

	if(pstCapability.nbAudioAna!=0)
	{
	     TestManager_AssertEqual( TDAL_OUTPUT_AudioAnaCapabilityGet(eTDAL_OUTPUT_AUDIO_ANA_ID_0,&AudioAnaCapability),
								   eTDAL_OUTPUT_NO_ERROR,
								   "Output AudioAna Get capabilities" );

	     TestManager_AssertEqual(TDAL_OUTPUT_AudioAnaSpeakerAttenuationSet
								(eTDAL_OUTPUT_AUDIO_ANA_ID_0,&pstAttenuation),
								eTDAL_OUTPUT_NO_ERROR,
								"output Audioana attenuation set" );


	}

	else
	{
	     mTBOX_PRINT(( kTBOX_LF, "Audioana output not supported\n"));

	}

   TestManager_AssertEqual( TDAL_OUTPUT_Term(), eTDAL_OUTPUT_NO_ERROR, "output term" );

     TestManager_AssertEqual(TDAL_DISP_Term(),
							eTDAL_DISP_NO_ERROR,
							"disp terminate" );

}

void TestCase_ExecuteUnitOutputAudioSPDIF(void)
{
	tTDAL_OUTPUT_Capability pstCapability;

    TestManager_AssertEqual(TDAL_DISP_Init(),
							eTDAL_DISP_NO_ERROR,
							"disp init" );


	TestManager_AssertEqual( TDAL_OUTPUT_Init(), eTDAL_OUTPUT_NO_ERROR, "Output init" );
       TestManager_AssertEqual( TDAL_OUTPUT_CapabilityGet(&pstCapability),
	   						   eTDAL_OUTPUT_NO_ERROR,
	   						   "Output Get capabilities" );

	if(pstCapability.nbAudioSPDIF!=0)
	{
   	           TestManager_AssertEqual(TDAL_OUTPUT_AudioSPDIFModeSet
								(eTDAL_OUTPUT_AUDIO_SPDIF_ID_0,eTDAL_OUTPUT_AUDIO_DIGITAL_MODE_COMPRESSED_AC3),
								eTDAL_OUTPUT_NO_ERROR,
								"output audio SPDIF audio mode  set" );
	}

	else
	{
	     mTBOX_PRINT(( kTBOX_LF, "SPDIF output not supported\n"));

	}

   TestManager_AssertEqual( TDAL_OUTPUT_Term(), eTDAL_OUTPUT_NO_ERROR, "output term" );

     TestManager_AssertEqual(TDAL_DISP_Term(),
							eTDAL_DISP_NO_ERROR,
							"disp terminate" );

}


