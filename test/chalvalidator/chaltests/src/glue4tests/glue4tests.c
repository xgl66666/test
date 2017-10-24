/*****************************************************************************
 * COPYRIGHT 2008 IWEDIA                                                     *
 *****************************************************************************
 *
 * MODULE NAME: Glue for CI+ stack
 *
 * FILE NAME: $URL:  $
 *            $Rev:  $
 *            $Date: $
 *
 * DESCRIPTION:
 *
 ****************************************************************************/

#include <time.h>

#include "glue4tests.h"
#include "tdal_fla.h"

#include "tkel.h"
#include "tbox.h"
#include "tdal_av.h"
#include "tdal_dmx.h"
#include "tdal_disp.h"
#include "tdal_output.h"
#include "tdal_hdmi.h"
#include "tdal_ts.h"

#include "tdal_avs.h"
#include "tdal_dmd.h"
#include "tdal_desc.h"

/*#include "tdal_common.h"*/
#include "TestHelper.h"
#include "TestManager.h"

#ifndef NO_CIPLUS_STACK
#include "dvbci_config.h"
#include "ci_util.h"
#include "ci_com.h"
#endif

IMPORT int gChannel;
void HDMICallback(tTDAL_HDMI_SourceId Id, tTDAL_HDMI_Event Event);
extern void TDAL_Init(void);

unsigned long GLUE4TESTS_CurrentTime = 0;

/*****************************************************************************
 *                              LOCALS DEFINES                               *
 *****************************************************************************/


/*****************************************************************************
 *                             EXTERNAL FUNCTION                             *
 *****************************************************************************/


 /*****************************************************************************
 *                             LOCALES VARIABLES                             *
 *****************************************************************************/
static int GLUE4TESTS_TestFeIdx  = 0;
static int GLUE4TESTS_TestDmxIdx = 0;
static tGLUE4TESTS_Channel GLUE4TESTS_Channel;
LOCAL void ClientTestSetup(void);

unsigned char PmtScrambCONAX2[] = { /* service CONAX2 */
    0x02, 0xb0 ,0x2f, 0x01, 0x92, 0xf5, 0x00, 0x00, 0xe1, 0xf6, 0xf0, 0x06, 0x09, 0x04, 0x0b, 0x00,
    0xf7, 0x71 ,0x02, 0xe1, 0xf6, 0xf0, 0x00, 0x03, 0xe2, 0x5a, 0xf0, 0x06, 0x0a, 0x04, 0x65, 0x6e,
    0x67, 0x00 ,0x06, 0xe2, 0xbe, 0xf0, 0x07, 0x56, 0x05, 0x65, 0x6e, 0x67, 0x09, 0x00, 0x1c, 0x57,
    0x24, 0xfb
};

unsigned char PmtScrambCONAX3[] = { /* service CONAX3 */
    0x02, 0xb0, 0x54, 0x01, 0x93, 0xf1, 0x00, 0x00, 0xe1, 0xf7, 0xf0, 0x00, 0x02, 0xe1, 0xf7, 0xf0,
    0x06, 0x09, 0x04, 0x0b, 0x00, 0xf7, 0x71, 0x03, 0xe2, 0x5b, 0xf0, 0x10, 0x0a, 0x08, 0x65, 0x6e,
    0x67, 0x00, 0x64, 0x61, 0x6e, 0x00, 0x09, 0x04, 0x0b, 0x00, 0xf7, 0x72, 0x03, 0xe2, 0x65, 0xf0,
    0x10, 0x0a, 0x08, 0x73, 0x77, 0x65, 0x00, 0x6e, 0x6f, 0x72, 0x00, 0x09, 0x04, 0x0b, 0x00, 0xf7,
    0x72, 0x06, 0xe2, 0xbf, 0xf0, 0x0d, 0x09, 0x04, 0x0b, 0x00, 0xf7, 0x73, 0x56, 0x05, 0x64, 0x61,
    0x6e, 0x09, 0x00, 0x41, 0x57, 0x64, 0x5f
};

unsigned char PmtScrambChannel2[] = { /* service CI+_CC_002 */
0x02,0xb0,0x27,0x00,0x05,0xc1,0x00,0x00,0xe0,0x31,0xf0,0x00,0x01,0xe0,0x31,0xf0
,0x08,0x09,0x06,0x10,0x00,0xe1,0x7c,0xff,0xfa,0x04,0xe0,0x34,0xf0,0x08,0x09,0x06
,0x10,0x00,0xe1,0x7d,0xff,0xfa,0x32,0x52,0x93,0x17
};


struct TestManager_Channel gChannelList[] = {
{
    "test stream Conax CD1 ts1.trp",
    "service CONAX1 (clear)",
    482000000,                  /* uint32_t  TunFrequency */
    8000000,                    /* uint32_t  SymbolRateBw */
    eTDAL_DMD_MOD_AUTO,         /* tTDAL_DMD_Modulation         Modulation */
    eTDAL_DMD_SPECTRUM_AUTO,    /* tTDAL_DMD_Spectrum           SpecInv */
    {
        eTDAL_DMD_MODE_8K,
        eTDAL_DMD_GUARD_1_32,
        eTDAL_DMD_HIER_AUTO,
        eTDAL_DMD_PRIORITY_STREAM_HIGH,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_OFFSET_AUTO,
        0
    },
    0,
    501,
    601,
    501,
	eTDAL_AV_VIDEO_TYPE_MPEG2,
	eTDAL_AV_AUDIO_TYPE_MPEG,0,0
},
{
    "test stream Conax CD1 ts1.trp",
    "service CONAX2 (scrambled)",
    482000000,                  /* uint32_t  TunFrequency */
    8000000,                    /* uint32_t  SymbolRateBw */
    eTDAL_DMD_MOD_AUTO,         /* tTDAL_DMD_Modulation         Modulation */
    eTDAL_DMD_SPECTRUM_AUTO,    /* tTDAL_DMD_Spectrum           SpecInv */
    {
        eTDAL_DMD_MODE_8K,
        eTDAL_DMD_GUARD_1_32,
        eTDAL_DMD_HIER_AUTO,
        eTDAL_DMD_PRIORITY_STREAM_HIGH,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_OFFSET_AUTO,
        0
    },
    0,
    502,
    602,
    502,
	eTDAL_AV_VIDEO_TYPE_MPEG2,
	eTDAL_AV_AUDIO_TYPE_MPEG,0,0
},
{
    "test stream Conax CD1 ts1.trp",
    "service CONAX3 (scrambled)",
    482000000,                  /* uint32_t  TunFrequency */
    8000000,                    /* uint32_t  SymbolRateBw */
    eTDAL_DMD_MOD_AUTO,         /* tTDAL_DMD_Modulation         Modulation */
    eTDAL_DMD_SPECTRUM_AUTO,    /* tTDAL_DMD_Spectrum           SpecInv */
    {
        eTDAL_DMD_MODE_8K,
        eTDAL_DMD_GUARD_1_32,
        eTDAL_DMD_HIER_AUTO,
        eTDAL_DMD_PRIORITY_STREAM_HIGH,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_OFFSET_AUTO,
        0
    },
    0,
    503,
    603,
    503,
	eTDAL_AV_VIDEO_TYPE_MPEG2,
	eTDAL_AV_AUDIO_TYPE_MPEG,0,0
},
{ NULL, NULL, 0, 0, 0, 0, {0, 0, 0, 0, 0, 0, 0, 0}, 0, 0, 0, 0,
	0,0,0,0
},
{
    "CI+_Cert_Test_Stream_Ter.trp",
    "FTA SVC 1 free_ci_mode 0 match_brand 0",
    506000000,                  /* uint32_t  TunFrequency */
    8000000,                    /* uint32_t  SymbolRateBw */
    eTDAL_DMD_MOD_AUTO,         /* tTDAL_DMD_Modulation         Modulation */
    eTDAL_DMD_SPECTRUM_AUTO,    /* tTDAL_DMD_Spectrum           SpecInv */
    {
        eTDAL_DMD_MODE_AUTO,
        eTDAL_DMD_GUARD_AUTO,
        eTDAL_DMD_HIER_AUTO,
        eTDAL_DMD_PRIORITY_STREAM_HIGH,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_OFFSET_AUTO,
        0
    },
    0,
    34,
    33,
    34,
	eTDAL_AV_VIDEO_TYPE_MPEG2,
	eTDAL_AV_AUDIO_TYPE_MPEG,0,0
},
{
    "CI+_Cert_Test_Stream_Ter.trp",
    "FTA SVC 2 free_ci_mode 0 match_brand 0",
    506000000,                  /* uint32_t  TunFrequency */
    8000000,                    /* uint32_t  SymbolRateBw */
    eTDAL_DMD_MOD_AUTO,         /* tTDAL_DMD_Modulation         Modulation */
    eTDAL_DMD_SPECTRUM_AUTO,    /* tTDAL_DMD_Spectrum           SpecInv */
    {
        eTDAL_DMD_MODE_AUTO,
        eTDAL_DMD_GUARD_AUTO,
        eTDAL_DMD_HIER_AUTO,
        eTDAL_DMD_PRIORITY_STREAM_HIGH,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_OFFSET_AUTO,
        0
    },
    0,
    36,
    37,
    36,
	eTDAL_AV_VIDEO_TYPE_MPEG2,
	eTDAL_AV_AUDIO_TYPE_MPEG,0,0
},
{
    "CI+_Cert_Test_Stream_Ter.trp",
    "CA pg SVC free_ci_mode 1 match_brand 0",
    506000000,                  /* uint32_t  TunFrequency */
    8000000,                    /* uint32_t  SymbolRateBw */
    eTDAL_DMD_MOD_AUTO,         /* tTDAL_DMD_Modulation         Modulation */
    eTDAL_DMD_SPECTRUM_AUTO,    /* tTDAL_DMD_Spectrum           SpecInv */
    {
        eTDAL_DMD_MODE_AUTO,
        eTDAL_DMD_GUARD_AUTO,
        eTDAL_DMD_HIER_AUTO,
        eTDAL_DMD_PRIORITY_STREAM_HIGH,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_OFFSET_AUTO,
        0
    },
    0,
    40,
    39,
    40,
	eTDAL_AV_VIDEO_TYPE_MPEG2,
	eTDAL_AV_AUDIO_TYPE_MPEG,0,0
},
{
    "CI+_Cert_Test_Stream_Ter.trp",
    "CA es SVC free_ci_mode 1 match_brand 0",
    506000000,                  /* uint32_t  TunFrequency */
    8000000,                    /* uint32_t  SymbolRateBw */
    eTDAL_DMD_MOD_AUTO,         /* tTDAL_DMD_Modulation         Modulation */
    eTDAL_DMD_SPECTRUM_AUTO,    /* tTDAL_DMD_Spectrum           SpecInv */
    {
        eTDAL_DMD_MODE_AUTO,
        eTDAL_DMD_GUARD_AUTO,
        eTDAL_DMD_HIER_AUTO,
        eTDAL_DMD_PRIORITY_STREAM_HIGH,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_OFFSET_AUTO,
        0
    },
    0,
    42,
    43,
    42,
	eTDAL_AV_VIDEO_TYPE_MPEG2,
	eTDAL_AV_AUDIO_TYPE_MPEG,0,0
},
{
    "CI+_Cert_Test_Stream_Ter.trp",
    "FTA SVC free_ci_mode 1 match_brand 1",
    506000000,                  /* uint32_t  TunFrequency */
    8000000,                    /* uint32_t  SymbolRateBw */
    eTDAL_DMD_MOD_AUTO,         /* tTDAL_DMD_Modulation         Modulation */
    eTDAL_DMD_SPECTRUM_AUTO,    /* tTDAL_DMD_Spectrum           SpecInv */
    {
        eTDAL_DMD_MODE_AUTO,
        eTDAL_DMD_GUARD_AUTO,
        eTDAL_DMD_HIER_AUTO,
        eTDAL_DMD_PRIORITY_STREAM_HIGH,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_OFFSET_AUTO,
        0
    },
    0,
    45,
    46,
    45,
	eTDAL_AV_VIDEO_TYPE_MPEG2,
	eTDAL_AV_AUDIO_TYPE_MPEG,0,0
},
{
    "CI+_Cert_Test_Stream_Ter.trp",
    "CA pg SVC free_ci_mode 1 match_brand 1 (CA PVR)",
    506000000,                  /* uint32_t  TunFrequency */
    8000000,                    /* uint32_t  SymbolRateBw */
    eTDAL_DMD_MOD_AUTO,         /* tTDAL_DMD_Modulation         Modulation */
    eTDAL_DMD_SPECTRUM_AUTO,    /* tTDAL_DMD_Spectrum           SpecInv */
    {
        eTDAL_DMD_MODE_AUTO,
        eTDAL_DMD_GUARD_AUTO,
        eTDAL_DMD_HIER_AUTO,
        eTDAL_DMD_PRIORITY_STREAM_HIGH,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_OFFSET_AUTO,
        0
    },
    0,
    49,
    48,
    49,
	eTDAL_AV_VIDEO_TYPE_MPEG2,
	eTDAL_AV_AUDIO_TYPE_MPEG,0,0
},
{
    "CI+_Cert_Test_Stream_Ter.trp",
    "CA es SVC free_ci_mode 1 match_brand 1 (CA PVR)",
    506000000,                  /* uint32_t  TunFrequency */
    8000000,                    /* uint32_t  SymbolRateBw */
    eTDAL_DMD_MOD_AUTO,         /* tTDAL_DMD_Modulation         Modulation */
    eTDAL_DMD_SPECTRUM_AUTO,    /* tTDAL_DMD_Spectrum           SpecInv */
    {
        eTDAL_DMD_MODE_AUTO,
        eTDAL_DMD_GUARD_AUTO,
        eTDAL_DMD_HIER_AUTO,
        eTDAL_DMD_PRIORITY_STREAM_HIGH,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_OFFSET_AUTO,
        0
    },
    0,
    51,
    52,
    51,
	eTDAL_AV_VIDEO_TYPE_MPEG2,
	eTDAL_AV_AUDIO_TYPE_MPEG,0,0
},
{
    "Stream_CIplus_1_CLSCRv2.0.ts",
    "STREAM_CI+_CLSCR_001",
    506000000,                  /* uint32_t  TunFrequency */
    8000000,                    /* uint32_t  SymbolRateBw */
    eTDAL_DMD_MOD_AUTO,         /* tTDAL_DMD_Modulation         Modulation */
    eTDAL_DMD_SPECTRUM_AUTO,    /* tTDAL_DMD_Spectrum           SpecInv */
    {
        eTDAL_DMD_MODE_AUTO,
        eTDAL_DMD_GUARD_AUTO,
        eTDAL_DMD_HIER_AUTO,
        eTDAL_DMD_PRIORITY_STREAM_HIGH,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_OFFSET_AUTO,
        0
    },
    0,
    0x21,
    0x24,
    0x21,
	eTDAL_AV_VIDEO_TYPE_MPEG2,
	eTDAL_AV_AUDIO_TYPE_MPEG,0,0
},
{
    "Stream_CIplus_1_CLSCRv2.0.ts",
    "STREAM_CI+_CLSCR_002",
    506000000,                  /* uint32_t  TunFrequency */
    8000000,                    /* uint32_t  SymbolRateBw */
    eTDAL_DMD_MOD_AUTO,         /* tTDAL_DMD_Modulation         Modulation */
    eTDAL_DMD_SPECTRUM_AUTO,    /* tTDAL_DMD_Spectrum           SpecInv */
    {
        eTDAL_DMD_MODE_AUTO,
        eTDAL_DMD_GUARD_AUTO,
        eTDAL_DMD_HIER_AUTO,
        eTDAL_DMD_PRIORITY_STREAM_HIGH,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_OFFSET_AUTO,
        0
    },
    0,
    0x31,
    0x34,
    0x31,
	eTDAL_AV_VIDEO_TYPE_MPEG2,
	eTDAL_AV_AUDIO_TYPE_MPEG,0,0
},
{
    "Stream_CIplus_1_CLSCRv2.0.ts",
    "STREAM_CI+_CLSCR_003",
    506000000,                  /* uint32_t  TunFrequency */
    8000000,                    /* uint32_t  SymbolRateBw */
    eTDAL_DMD_MOD_AUTO,         /* tTDAL_DMD_Modulation         Modulation */
    eTDAL_DMD_SPECTRUM_AUTO,    /* tTDAL_DMD_Spectrum           SpecInv */
    {
        eTDAL_DMD_MODE_AUTO,
        eTDAL_DMD_GUARD_AUTO,
        eTDAL_DMD_HIER_AUTO,
        eTDAL_DMD_PRIORITY_STREAM_HIGH,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_OFFSET_AUTO,
        0
    },
    0,
    0x41,
    0x44,
    0x41,
	eTDAL_AV_VIDEO_TYPE_MPEG2,
	eTDAL_AV_AUDIO_TYPE_MPEG,0,0
},
{
    "Stream_CIplus_2_CCv2.0.ts",
    "STREAM_CI+_CC_001",
    506000000,                  /* uint32_t  TunFrequency */
    8000000,                    /* uint32_t  SymbolRateBw */
    eTDAL_DMD_MOD_AUTO,         /* tTDAL_DMD_Modulation         Modulation */
    eTDAL_DMD_SPECTRUM_AUTO,    /* tTDAL_DMD_Spectrum           SpecInv */
    {
        eTDAL_DMD_MODE_AUTO,
        eTDAL_DMD_GUARD_AUTO,
        eTDAL_DMD_HIER_AUTO,
        eTDAL_DMD_PRIORITY_STREAM_HIGH,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_OFFSET_AUTO,
        0
    },
    0,
    0x21,
    0x24,
    0x21,
	eTDAL_AV_VIDEO_TYPE_MPEG2,
	eTDAL_AV_AUDIO_TYPE_MPEG,0,0
},
{
    "Stream_CIplus_2_CCv2.0.ts",
    "STREAM_CI+_CC_002",
    506000000,                  /* uint32_t  TunFrequency */
    8000000,                    /* uint32_t  SymbolRateBw */
    eTDAL_DMD_MOD_AUTO,         /* tTDAL_DMD_Modulation         Modulation */
    eTDAL_DMD_SPECTRUM_AUTO,    /* tTDAL_DMD_Spectrum           SpecInv */
    {
        eTDAL_DMD_MODE_AUTO,
        eTDAL_DMD_GUARD_AUTO,
        eTDAL_DMD_HIER_AUTO,
        eTDAL_DMD_PRIORITY_STREAM_HIGH,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_OFFSET_AUTO,
        0
    },
    0,
    0x31,
    0x34,
    0x31,
	eTDAL_AV_VIDEO_TYPE_MPEG2,
	eTDAL_AV_AUDIO_TYPE_MPEG,0,0
},
{
    "Stream_CIplus_2_CCv2.0.ts",
    "STREAM_CI+_CC_003",
    506000000,                  /* uint32_t  TunFrequency */
    8000000,                    /* uint32_t  SymbolRateBw */
    eTDAL_DMD_MOD_AUTO,         /* tTDAL_DMD_Modulation         Modulation */
    eTDAL_DMD_SPECTRUM_AUTO,    /* tTDAL_DMD_Spectrum           SpecInv */
    {
        eTDAL_DMD_MODE_AUTO,
        eTDAL_DMD_GUARD_AUTO,
        eTDAL_DMD_HIER_AUTO,
        eTDAL_DMD_PRIORITY_STREAM_HIGH,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_OFFSET_AUTO,
        0
    },
    0,
    0x41,
    0x44,
    0x41,
	eTDAL_AV_VIDEO_TYPE_MPEG2,
	eTDAL_AV_AUDIO_TYPE_MPEG,0,0
},
{
    "Stream_CIplus_3_DOTAv2.0.ts",
    "STREAM_CI+_DOTA_001",
    506000000,                  /* uint32_t  TunFrequency */
    8000000,                    /* uint32_t  SymbolRateBw */
    eTDAL_DMD_MOD_AUTO,         /* tTDAL_DMD_Modulation         Modulation */
    eTDAL_DMD_SPECTRUM_AUTO,    /* tTDAL_DMD_Spectrum           SpecInv */
    {
        eTDAL_DMD_MODE_AUTO,
        eTDAL_DMD_GUARD_AUTO,
        eTDAL_DMD_HIER_AUTO,
        eTDAL_DMD_PRIORITY_STREAM_HIGH,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_OFFSET_AUTO,
        0
    },
    0,
    0x21,
    0x24,
    0x21,
	eTDAL_AV_VIDEO_TYPE_MPEG2,
	eTDAL_AV_AUDIO_TYPE_MPEG,0,0
},
{
    "Stream_CIplus_3_DOTAv2.0.ts",
    "STREAM_CI+_DOTA_002",
    506000000,                  /* uint32_t  TunFrequency */
    8000000,                    /* uint32_t  SymbolRateBw */
    eTDAL_DMD_MOD_AUTO,         /* tTDAL_DMD_Modulation         Modulation */
    eTDAL_DMD_SPECTRUM_AUTO,    /* tTDAL_DMD_Spectrum           SpecInv */
    {
        eTDAL_DMD_MODE_AUTO,
        eTDAL_DMD_GUARD_AUTO,
        eTDAL_DMD_HIER_AUTO,
        eTDAL_DMD_PRIORITY_STREAM_HIGH,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_OFFSET_AUTO,
        0
    },
    0,
    0x31,
    0x34,
    0x31,
	eTDAL_AV_VIDEO_TYPE_MPEG2,
	eTDAL_AV_AUDIO_TYPE_MPEG,0,0
},
{
    "Stream_CIplus_3_DOTAv2.0.ts",
    "STREAM_CI+_DOTA_003",
    506000000,                  /* uint32_t  TunFrequency */
    8000000,                    /* uint32_t  SymbolRateBw */
    eTDAL_DMD_MOD_AUTO,         /* tTDAL_DMD_Modulation         Modulation */
    eTDAL_DMD_SPECTRUM_AUTO,    /* tTDAL_DMD_Spectrum           SpecInv */
    {
        eTDAL_DMD_MODE_AUTO,
        eTDAL_DMD_GUARD_AUTO,
        eTDAL_DMD_HIER_AUTO,
        eTDAL_DMD_PRIORITY_STREAM_HIGH,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_OFFSET_AUTO,
        0
    },
    0,
    0x41,
    0x44,
    0x41,
	eTDAL_AV_VIDEO_TYPE_MPEG2,
	eTDAL_AV_AUDIO_TYPE_MPEG,0,0
},
{
    "SWE RF27 522Mhz Silver 10mn Playback OK.mpg",
    "Kanal Lokal Skane",
    506000000,                  /* uint32_t  TunFrequency */
    8000000,                    /* uint32_t  SymbolRateBw */
    eTDAL_DMD_MOD_AUTO,         /* tTDAL_DMD_Modulation         Modulation */
    eTDAL_DMD_SPECTRUM_AUTO,    /* tTDAL_DMD_Spectrum           SpecInv */
    {
        eTDAL_DMD_MODE_AUTO,
        eTDAL_DMD_GUARD_AUTO,
        eTDAL_DMD_HIER_AUTO,
        eTDAL_DMD_PRIORITY_STREAM_HIGH,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_OFFSET_AUTO,
        0
    },
    0,
    0x1fd5,
    0x1fd4,
    0x1fd5,
	eTDAL_AV_VIDEO_TYPE_MPEG2,
	eTDAL_AV_AUDIO_TYPE_MPEG,0,0
},
{
    "SWE RF27 522Mhz Silver 10mn Playback OK.mpg",
    "TV7",
    506000000,                  /* uint32_t  TunFrequency */
    8000000,                    /* uint32_t  SymbolRateBw */
    eTDAL_DMD_MOD_AUTO,         /* tTDAL_DMD_Modulation         Modulation */
    eTDAL_DMD_SPECTRUM_AUTO,    /* tTDAL_DMD_Spectrum           SpecInv */
    {
        eTDAL_DMD_MODE_AUTO,
        eTDAL_DMD_GUARD_AUTO,
        eTDAL_DMD_HIER_AUTO,
        eTDAL_DMD_PRIORITY_STREAM_HIGH,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_OFFSET_AUTO,
        0
    },
    0,
    0x333,
    0x332,
    0x333,
	eTDAL_AV_VIDEO_TYPE_MPEG2,
	eTDAL_AV_AUDIO_TYPE_MPEG,0,0
},
{
    "SWE RF27 522Mhz Silver 10mn Playback OK.mpg",
    "BBC World",
    506000000,                  /* uint32_t  TunFrequency */
    8000000,                    /* uint32_t  SymbolRateBw */
    eTDAL_DMD_MOD_AUTO,         /* tTDAL_DMD_Modulation         Modulation */
    eTDAL_DMD_SPECTRUM_AUTO,    /* tTDAL_DMD_Spectrum           SpecInv */
    {
        eTDAL_DMD_MODE_AUTO,
        eTDAL_DMD_GUARD_AUTO,
        eTDAL_DMD_HIER_AUTO,
        eTDAL_DMD_PRIORITY_STREAM_HIGH,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_OFFSET_AUTO,
        0
    },
    0,
    0x49b,
    0x49a,
    0x49b,
	eTDAL_AV_VIDEO_TYPE_MPEG2,
	eTDAL_AV_AUDIO_TYPE_MPEG,0,0
},
{
    "SWE RF27 522Mhz Silver 10mn Playback OK.mpg",
    "Discovery T&L",
    506000000,                  /* uint32_t  TunFrequency */
    8000000,                    /* uint32_t  SymbolRateBw */
    eTDAL_DMD_MOD_AUTO,         /* tTDAL_DMD_Modulation         Modulation */
    eTDAL_DMD_SPECTRUM_AUTO,    /* tTDAL_DMD_Spectrum           SpecInv */
    {
        eTDAL_DMD_MODE_AUTO,
        eTDAL_DMD_GUARD_AUTO,
        eTDAL_DMD_HIER_AUTO,
        eTDAL_DMD_PRIORITY_STREAM_HIGH,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_OFFSET_AUTO,
        0
    },
    0,
    0x469,
    0x468,
    0x469,
	eTDAL_AV_VIDEO_TYPE_MPEG2,
	eTDAL_AV_AUDIO_TYPE_MPEG,0,0
},
{
    "SWE RF27 522Mhz Silver 10mn Playback OK.mpg",
    "TMC",
    506000000,                  /* uint32_t  TunFrequency */
    8000000,                    /* uint32_t  SymbolRateBw */
    eTDAL_DMD_MOD_AUTO,         /* tTDAL_DMD_Modulation         Modulation */
    eTDAL_DMD_SPECTRUM_AUTO,    /* tTDAL_DMD_Spectrum           SpecInv */
    {
        eTDAL_DMD_MODE_AUTO,
        eTDAL_DMD_GUARD_AUTO,
        eTDAL_DMD_HIER_AUTO,
        eTDAL_DMD_PRIORITY_STREAM_HIGH,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_OFFSET_AUTO,
        0
    },
    0,
    0x4b9,
    0x4b8,
    0x4b9,
	eTDAL_AV_VIDEO_TYPE_MPEG2,
	eTDAL_AV_AUDIO_TYPE_MPEG,0,0
},
{
    "SWE RF27 522Mhz Silver 10mn Playback OK.mpg",
    "BBC Prime",
    506000000,                  /* uint32_t  TunFrequency */
    8000000,                    /* uint32_t  SymbolRateBw */
    eTDAL_DMD_MOD_AUTO,         /* tTDAL_DMD_Modulation         Modulation */
    eTDAL_DMD_SPECTRUM_AUTO,    /* tTDAL_DMD_Spectrum           SpecInv */
    {
        eTDAL_DMD_MODE_AUTO,
        eTDAL_DMD_GUARD_AUTO,
        eTDAL_DMD_HIER_AUTO,
        eTDAL_DMD_PRIORITY_STREAM_HIGH,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_OFFSET_AUTO,
        0
    },
    0,
    0x33d,
    0x33c,
    0x33d,
	eTDAL_AV_VIDEO_TYPE_MPEG2,
	eTDAL_AV_AUDIO_TYPE_MPEG,0,0
},
{
    "SWE RF27 522Mhz Silver 10mn Playback OK.mpg",
    "Axess TV",
    506000000,                  /* uint32_t  TunFrequency */
    8000000,                    /* uint32_t  SymbolRateBw */
    eTDAL_DMD_MOD_AUTO,         /* tTDAL_DMD_Modulation         Modulation */
    eTDAL_DMD_SPECTRUM_AUTO,    /* tTDAL_DMD_Spectrum           SpecInv */
    {
        eTDAL_DMD_MODE_AUTO,
        eTDAL_DMD_GUARD_AUTO,
        eTDAL_DMD_HIER_AUTO,
        eTDAL_DMD_PRIORITY_STREAM_HIGH,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_OFFSET_AUTO,
        0
    },
    0,
    0x329,
    0x328,
    0x329,
	eTDAL_AV_VIDEO_TYPE_MPEG2,
	eTDAL_AV_AUDIO_TYPE_MPEG,0,0
},

{
    "FIN RF53 730Mhz Eurosport 10mn.mpg",
    "MTV3 Fakta",
    506000000,                  /* uint32_t  TunFrequency */
    8000000,                    /* uint32_t  SymbolRateBw */
    eTDAL_DMD_MOD_AUTO,         /* tTDAL_DMD_Modulation         Modulation */
    eTDAL_DMD_SPECTRUM_AUTO,    /* tTDAL_DMD_Spectrum           SpecInv */
    {
        eTDAL_DMD_MODE_AUTO,
        eTDAL_DMD_GUARD_AUTO,
        eTDAL_DMD_HIER_AUTO,
        eTDAL_DMD_PRIORITY_STREAM_HIGH,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_OFFSET_AUTO,
        0
    },
    0,
    0x200,
    0x28a,
    0x200,
	eTDAL_AV_VIDEO_TYPE_MPEG2,
	eTDAL_AV_AUDIO_TYPE_MPEG,0,0
},
{
    "FIN RF53 730Mhz Eurosport 10mn.mpg",
    "Discovery",
    506000000,                  /* uint32_t  TunFrequency */
    8000000,                    /* uint32_t  SymbolRateBw */
    eTDAL_DMD_MOD_AUTO,         /* tTDAL_DMD_Modulation         Modulation */
    eTDAL_DMD_SPECTRUM_AUTO,    /* tTDAL_DMD_Spectrum           SpecInv */
    {
        eTDAL_DMD_MODE_AUTO,
        eTDAL_DMD_GUARD_AUTO,
        eTDAL_DMD_HIER_AUTO,
        eTDAL_DMD_PRIORITY_STREAM_HIGH,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_OFFSET_AUTO,
        0
    },
    0,
    0x201,
    0x294,
    0x201,
	eTDAL_AV_VIDEO_TYPE_MPEG2,
	eTDAL_AV_AUDIO_TYPE_MPEG,0,0
},
{
    "FIN RF53 730Mhz Eurosport 10mn.mpg",
    "Eurosport",
    506000000,                  /* uint32_t  TunFrequency */
    8000000,                    /* uint32_t  SymbolRateBw */
    eTDAL_DMD_MOD_AUTO,         /* tTDAL_DMD_Modulation         Modulation */
    eTDAL_DMD_SPECTRUM_AUTO,    /* tTDAL_DMD_Spectrum           SpecInv */
    {
        eTDAL_DMD_MODE_AUTO,
        eTDAL_DMD_GUARD_AUTO,
        eTDAL_DMD_HIER_AUTO,
        eTDAL_DMD_PRIORITY_STREAM_HIGH,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_OFFSET_AUTO,
        0
    },
    0,
    0x202,
    0x29e,
    0x202,
	eTDAL_AV_VIDEO_TYPE_MPEG2,
	eTDAL_AV_AUDIO_TYPE_MPEG,0,0
},
{
    "FIN RF53 730Mhz Eurosport 10mn.mpg",
    "MTV Music Television",
    506000000,                  /* uint32_t  TunFrequency */
    8000000,                    /* uint32_t  SymbolRateBw */
    eTDAL_DMD_MOD_AUTO,         /* tTDAL_DMD_Modulation         Modulation */
    eTDAL_DMD_SPECTRUM_AUTO,    /* tTDAL_DMD_Spectrum           SpecInv */
    {
        eTDAL_DMD_MODE_AUTO,
        eTDAL_DMD_GUARD_AUTO,
        eTDAL_DMD_HIER_AUTO,
        eTDAL_DMD_PRIORITY_STREAM_HIGH,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_OFFSET_AUTO,
        0
    },
    0,
    0x203,
    0x2a8,
    0x203,
	eTDAL_AV_VIDEO_TYPE_MPEG2,
	eTDAL_AV_AUDIO_TYPE_MPEG,0,0
},
{
    "FIN RF53 730Mhz Eurosport 10mn.mpg",
    "SVT Europa",
    506000000,                  /* uint32_t  TunFrequency */
    8000000,                    /* uint32_t  SymbolRateBw */
    eTDAL_DMD_MOD_AUTO,         /* tTDAL_DMD_Modulation         Modulation */
    eTDAL_DMD_SPECTRUM_AUTO,    /* tTDAL_DMD_Spectrum           SpecInv */
    {
        eTDAL_DMD_MODE_AUTO,
        eTDAL_DMD_GUARD_AUTO,
        eTDAL_DMD_HIER_AUTO,
        eTDAL_DMD_PRIORITY_STREAM_HIGH,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_OFFSET_AUTO,
        0
    },
    0,
    0x205,
    0x2bc,
    0x205,
	eTDAL_AV_VIDEO_TYPE_MPEG2,
	eTDAL_AV_AUDIO_TYPE_MPEG,0,0
},
{
    "FIN RF53 730Mhz Eurosport 10mn.mpg",
    "KinoTV",
    506000000,                  /* uint32_t  TunFrequency */
    8000000,                    /* uint32_t  SymbolRateBw */
    eTDAL_DMD_MOD_AUTO,         /* tTDAL_DMD_Modulation         Modulation */
    eTDAL_DMD_SPECTRUM_AUTO,    /* tTDAL_DMD_Spectrum           SpecInv */
    {
        eTDAL_DMD_MODE_AUTO,
        eTDAL_DMD_GUARD_AUTO,
        eTDAL_DMD_HIER_AUTO,
        eTDAL_DMD_PRIORITY_STREAM_HIGH,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_OFFSET_AUTO,
        0
    },
    0,
    0x206,
    0x2c6,
    0x206,
	eTDAL_AV_VIDEO_TYPE_MPEG2,
	eTDAL_AV_AUDIO_TYPE_MPEG,0,0
},
{
    "FIN RF53 730Mhz Eurosport 10mn.mpg",
    "Nickelodeon",
    506000000,                  /* uint32_t  TunFrequency */
    8000000,                    /* uint32_t  SymbolRateBw */
    eTDAL_DMD_MOD_AUTO,         /* tTDAL_DMD_Modulation         Modulation */
    eTDAL_DMD_SPECTRUM_AUTO,    /* tTDAL_DMD_Spectrum           SpecInv */
    {
        eTDAL_DMD_MODE_AUTO,
        eTDAL_DMD_GUARD_AUTO,
        eTDAL_DMD_HIER_AUTO,
        eTDAL_DMD_PRIORITY_STREAM_HIGH,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_OFFSET_AUTO,
        0
    },
    0,
    0x204,
    0x2b2,
    0x204,
	eTDAL_AV_VIDEO_TYPE_MPEG2,
	eTDAL_AV_AUDIO_TYPE_MPEG,0,0
},
{
    "FIN RF53 730Mhz Eurosport 10mn.mpg",
    "PlusTV",
    506000000,                  /* uint32_t  TunFrequency */
    8000000,                    /* uint32_t  SymbolRateBw */
    eTDAL_DMD_MOD_AUTO,         /* tTDAL_DMD_Modulation         Modulation */
    eTDAL_DMD_SPECTRUM_AUTO,    /* tTDAL_DMD_Spectrum           SpecInv */
    {
        eTDAL_DMD_MODE_AUTO,
        eTDAL_DMD_GUARD_AUTO,
        eTDAL_DMD_HIER_AUTO,
        eTDAL_DMD_PRIORITY_STREAM_HIGH,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_OFFSET_AUTO,
        0
    },
    0,
    0x207,
    0x32a,
    0x207,
	eTDAL_AV_VIDEO_TYPE_MPEG2,
	eTDAL_AV_AUDIO_TYPE_MPEG,0,0
},

{
    "FIN RF46 674Mhz C+ 10mn.mpg",
    "Urheilukanava",
    506000000,                  /* uint32_t  TunFrequency */
    8000000,                    /* uint32_t  SymbolRateBw */
    eTDAL_DMD_MOD_AUTO,         /* tTDAL_DMD_Modulation         Modulation */
    eTDAL_DMD_SPECTRUM_AUTO,    /* tTDAL_DMD_Spectrum           SpecInv */
    {
        eTDAL_DMD_MODE_AUTO,
        eTDAL_DMD_GUARD_AUTO,
        eTDAL_DMD_HIER_AUTO,
        eTDAL_DMD_PRIORITY_STREAM_HIGH,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_OFFSET_AUTO,
        0
    },
    0,
    0x1a1,
    0x2a1,
    0x1a1,
	eTDAL_AV_VIDEO_TYPE_MPEG2,
	eTDAL_AV_AUDIO_TYPE_MPEG,0,0
},
{
    "FIN RF46 674Mhz C+ 10mn.mpg",
    "The Voice",
    506000000,                  /* uint32_t  TunFrequency */
    8000000,                    /* uint32_t  SymbolRateBw */
    eTDAL_DMD_MOD_AUTO,         /* tTDAL_DMD_Modulation         Modulation */
    eTDAL_DMD_SPECTRUM_AUTO,    /* tTDAL_DMD_Spectrum           SpecInv */
    {
        eTDAL_DMD_MODE_AUTO,
        eTDAL_DMD_GUARD_AUTO,
        eTDAL_DMD_HIER_AUTO,
        eTDAL_DMD_PRIORITY_STREAM_HIGH,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_OFFSET_AUTO,
        0
    },
    0,
    0x201,
    0x294,
    0x201,
	eTDAL_AV_VIDEO_TYPE_MPEG2,
	eTDAL_AV_AUDIO_TYPE_MPEG,0,0
},
{
    "FIN RF46 674Mhz C+ 10mn.mpg",
    "URHEILU+KANAVA",
    506000000,                  /* uint32_t  TunFrequency */
    8000000,                    /* uint32_t  SymbolRateBw */
    eTDAL_DMD_MOD_AUTO,         /* tTDAL_DMD_Modulation         Modulation */
    eTDAL_DMD_SPECTRUM_AUTO,    /* tTDAL_DMD_Spectrum           SpecInv */
    {
        eTDAL_DMD_MODE_AUTO,
        eTDAL_DMD_GUARD_AUTO,
        eTDAL_DMD_HIER_AUTO,
        eTDAL_DMD_PRIORITY_STREAM_HIGH,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_OFFSET_AUTO,
        0
    },
    0,
    0x1a2,
    0x2a3,
    0x1a2,
	eTDAL_AV_VIDEO_TYPE_MPEG2,
	eTDAL_AV_AUDIO_TYPE_MPEG,0,0
},
{
    "FIN RF46 674Mhz C+ 10mn.mpg",
    "Disney Channel",
    506000000,                  /* uint32_t  TunFrequency */
    8000000,                    /* uint32_t  SymbolRateBw */
    eTDAL_DMD_MOD_AUTO,         /* tTDAL_DMD_Modulation         Modulation */
    eTDAL_DMD_SPECTRUM_AUTO,    /* tTDAL_DMD_Spectrum           SpecInv */
    {
        eTDAL_DMD_MODE_AUTO,
        eTDAL_DMD_GUARD_AUTO,
        eTDAL_DMD_HIER_AUTO,
        eTDAL_DMD_PRIORITY_STREAM_HIGH,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_OFFSET_AUTO,
        0
    },
    0,
    0x211,
    0x2da,
    0x211,
	eTDAL_AV_VIDEO_TYPE_MPEG2,
	eTDAL_AV_AUDIO_TYPE_MPEG,0,0
},
{
    "FIN RF46 674Mhz C+ 10mn.mpg",
    "CANAL+ FIRST",
    506000000,                  /* uint32_t  TunFrequency */
    8000000,                    /* uint32_t  SymbolRateBw */
    eTDAL_DMD_MOD_AUTO,         /* tTDAL_DMD_Modulation         Modulation */
    eTDAL_DMD_SPECTRUM_AUTO,    /* tTDAL_DMD_Spectrum           SpecInv */
    {
        eTDAL_DMD_MODE_AUTO,
        eTDAL_DMD_GUARD_AUTO,
        eTDAL_DMD_HIER_AUTO,
        eTDAL_DMD_PRIORITY_STREAM_HIGH,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_OFFSET_AUTO,
        0
    },
    0,
    0x202,
    0x29e,
    0x202,
	eTDAL_AV_VIDEO_TYPE_MPEG2,
	eTDAL_AV_AUDIO_TYPE_MPEG,0,0
},
{
    "FIN RF46 674Mhz C+ 10mn.mpg",
    "CANAL+ HITS",
    506000000,                  /* uint32_t  TunFrequency */
    8000000,                    /* uint32_t  SymbolRateBw */
    eTDAL_DMD_MOD_AUTO,         /* tTDAL_DMD_Modulation         Modulation */
    eTDAL_DMD_SPECTRUM_AUTO,    /* tTDAL_DMD_Spectrum           SpecInv */
    {
        eTDAL_DMD_MODE_AUTO,
        eTDAL_DMD_GUARD_AUTO,
        eTDAL_DMD_HIER_AUTO,
        eTDAL_DMD_PRIORITY_STREAM_HIGH,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_OFFSET_AUTO,
        0
    },
    0,
    0x203,
    0x2a8,
    0x203,
	eTDAL_AV_VIDEO_TYPE_MPEG2,
	eTDAL_AV_AUDIO_TYPE_MPEG,0,0
},
{
    "FIN RF46 674Mhz C+ 10mn.mpg",
    "CANAL+ SPORT 1",
    506000000,                  /* uint32_t  TunFrequency */
    8000000,                    /* uint32_t  SymbolRateBw */
    eTDAL_DMD_MOD_AUTO,         /* tTDAL_DMD_Modulation         Modulation */
    eTDAL_DMD_SPECTRUM_AUTO,    /* tTDAL_DMD_Spectrum           SpecInv */
    {
        eTDAL_DMD_MODE_AUTO,
        eTDAL_DMD_GUARD_AUTO,
        eTDAL_DMD_HIER_AUTO,
        eTDAL_DMD_PRIORITY_STREAM_HIGH,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_OFFSET_AUTO,
        0
    },
    0,
    0x205,
    0x2bc,
    0x205,
	eTDAL_AV_VIDEO_TYPE_MPEG2,
	eTDAL_AV_AUDIO_TYPE_MPEG,0,0
},
{
    "FIN RF46 674Mhz C+ 10mn.mpg",
    "CANAL+ SPORT 2",
    506000000,                  /* uint32_t  TunFrequency */
    8000000,                    /* uint32_t  SymbolRateBw */
    eTDAL_DMD_MOD_AUTO,         /* tTDAL_DMD_Modulation         Modulation */
    eTDAL_DMD_SPECTRUM_AUTO,    /* tTDAL_DMD_Spectrum           SpecInv */
    {
        eTDAL_DMD_MODE_AUTO,
        eTDAL_DMD_GUARD_AUTO,
        eTDAL_DMD_HIER_AUTO,
        eTDAL_DMD_PRIORITY_STREAM_HIGH,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_OFFSET_AUTO,
        0
    },
    0,
    0x207,
    0x2d0,
    0x207,
	eTDAL_AV_VIDEO_TYPE_MPEG2,
	eTDAL_AV_AUDIO_TYPE_MPEG,0,0
},
{
    "FIN RF46 674Mhz C+ 10mn.mpg",
    "Canal69",
    506000000,                  /* uint32_t  TunFrequency */
    8000000,                    /* uint32_t  SymbolRateBw */
    eTDAL_DMD_MOD_AUTO,         /* tTDAL_DMD_Modulation         Modulation */
    eTDAL_DMD_SPECTRUM_AUTO,    /* tTDAL_DMD_Spectrum           SpecInv */
    {
        eTDAL_DMD_MODE_AUTO,
        eTDAL_DMD_GUARD_AUTO,
        eTDAL_DMD_HIER_AUTO,
        eTDAL_DMD_PRIORITY_STREAM_HIGH,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_OFFSET_AUTO,
        0
    },
    0,
    0x213,
    0x2e5,
    0x213,
	eTDAL_AV_VIDEO_TYPE_MPEG2,
	eTDAL_AV_AUDIO_TYPE_MPEG,0,0
},
{
    "FIN RF46 674Mhz C+ 10mn.mpg",
    "Iskelm_TV Harju...",
    506000000,                  /* uint32_t  TunFrequency */
    8000000,                    /* uint32_t  SymbolRateBw */
    eTDAL_DMD_MOD_AUTO,         /* tTDAL_DMD_Modulation         Modulation */
    eTDAL_DMD_SPECTRUM_AUTO,    /* tTDAL_DMD_Spectrum           SpecInv */
    {
        eTDAL_DMD_MODE_AUTO,
        eTDAL_DMD_GUARD_AUTO,
        eTDAL_DMD_HIER_AUTO,
        eTDAL_DMD_PRIORITY_STREAM_HIGH,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_OFFSET_AUTO,
        0
    },
    0,
    0x200,
    0x28a,
    0x200,
	eTDAL_AV_VIDEO_TYPE_MPEG2,
	eTDAL_AV_AUDIO_TYPE_MPEG,0,0
},
{
    "FIN RF46 674Mhz C+ 10mn.mpg",
    "ISKELM",
    506000000,                  /* uint32_t  TunFrequency */
    8000000,                    /* uint32_t  SymbolRateBw */
    eTDAL_DMD_MOD_AUTO,         /* tTDAL_DMD_Modulation         Modulation */
    eTDAL_DMD_SPECTRUM_AUTO,    /* tTDAL_DMD_Spectrum           SpecInv */
    {
        eTDAL_DMD_MODE_AUTO,
        eTDAL_DMD_GUARD_AUTO,
        eTDAL_DMD_HIER_AUTO,
        eTDAL_DMD_PRIORITY_STREAM_HIGH,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_OFFSET_AUTO,
        0
    },
    0,
    0x208,
    0x2db,
    0x208,
	eTDAL_AV_VIDEO_TYPE_MPEG2,
	eTDAL_AV_AUDIO_TYPE_MPEG,0,0
},
{
    "FIN RF46 674Mhz C+ 10mn.mpg",
    "klubi.tv",
    506000000,                  /* uint32_t  TunFrequency */
    8000000,                    /* uint32_t  SymbolRateBw */
    eTDAL_DMD_MOD_AUTO,         /* tTDAL_DMD_Modulation         Modulation */
    eTDAL_DMD_SPECTRUM_AUTO,    /* tTDAL_DMD_Spectrum           SpecInv */
    {
        eTDAL_DMD_MODE_AUTO,
        eTDAL_DMD_GUARD_AUTO,
        eTDAL_DMD_HIER_AUTO,
        eTDAL_DMD_PRIORITY_STREAM_HIGH,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_OFFSET_AUTO,
        0
    },
    0,
    0x212,
    0x2e4,
    0x212,
	eTDAL_AV_VIDEO_TYPE_MPEG2,
	eTDAL_AV_AUDIO_TYPE_MPEG,0,0
},
{
    "FIN RF46 674Mhz C+ 10mn.mpg",
    "DIGIVIIHDE",
    506000000,                  /* uint32_t  TunFrequency */
    8000000,                    /* uint32_t  SymbolRateBw */
    eTDAL_DMD_MOD_AUTO,         /* tTDAL_DMD_Modulation         Modulation */
    eTDAL_DMD_SPECTRUM_AUTO,    /* tTDAL_DMD_Spectrum           SpecInv */
    {
        eTDAL_DMD_MODE_AUTO,
        eTDAL_DMD_GUARD_AUTO,
        eTDAL_DMD_HIER_AUTO,
        eTDAL_DMD_PRIORITY_STREAM_HIGH,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_OFFSET_AUTO,
        0
    },
    0,
    0x206,
    0x2d1,
    0x206,
	eTDAL_AV_VIDEO_TYPE_MPEG2,
	eTDAL_AV_AUDIO_TYPE_MPEG,0,0
},

{
    "Live_Portsmouth_080430_1420_setanta_all/MUX546_Five.trp",
    "Five",
    506000000,                  /* uint32_t  TunFrequency */
    8000000,                    /* uint32_t  SymbolRateBw */
    eTDAL_DMD_MOD_AUTO,         /* tTDAL_DMD_Modulation         Modulation */
    eTDAL_DMD_SPECTRUM_AUTO,    /* tTDAL_DMD_Spectrum           SpecInv */
    {
        eTDAL_DMD_MODE_AUTO,
        eTDAL_DMD_GUARD_AUTO,
        eTDAL_DMD_HIER_AUTO,
        eTDAL_DMD_PRIORITY_STREAM_HIGH,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_OFFSET_AUTO,
        0
    },
    0,
    0x1781,
    0x1782,
    0x1781,
	eTDAL_AV_VIDEO_TYPE_MPEG2,
	eTDAL_AV_AUDIO_TYPE_MPEG,0,0
},
{
    "Live_Portsmouth_080430_1420_setanta_all/MUX546_Five.trp",
    "FIVER",
    506000000,                  /* uint32_t  TunFrequency */
    8000000,                    /* uint32_t  SymbolRateBw */
    eTDAL_DMD_MOD_AUTO,         /* tTDAL_DMD_Modulation         Modulation */
    eTDAL_DMD_SPECTRUM_AUTO,    /* tTDAL_DMD_Spectrum           SpecInv */
    {
        eTDAL_DMD_MODE_AUTO,
        eTDAL_DMD_GUARD_AUTO,
        eTDAL_DMD_HIER_AUTO,
        eTDAL_DMD_PRIORITY_STREAM_HIGH,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_OFFSET_AUTO,
        0
    },
    0,
    0x1a11,
    0x1a12,
    0x1a11,
	eTDAL_AV_VIDEO_TYPE_MPEG2,
	eTDAL_AV_AUDIO_TYPE_MPEG,0,0
},
{
    "Live_Portsmouth_080430_1420_setanta_all/MUX546_Five.trp",
    "Five US",
    506000000,                  /* uint32_t  TunFrequency */
    8000000,                    /* uint32_t  SymbolRateBw */
    eTDAL_DMD_MOD_AUTO,         /* tTDAL_DMD_Modulation         Modulation */
    eTDAL_DMD_SPECTRUM_AUTO,    /* tTDAL_DMD_Spectrum           SpecInv */
    {
        eTDAL_DMD_MODE_AUTO,
        eTDAL_DMD_GUARD_AUTO,
        eTDAL_DMD_HIER_AUTO,
        eTDAL_DMD_PRIORITY_STREAM_HIGH,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_OFFSET_AUTO,
        0
    },
    0,
    0x1a21,
    0x1a22,
    0x1a21,
	eTDAL_AV_VIDEO_TYPE_MPEG2,
	eTDAL_AV_AUDIO_TYPE_MPEG,0,0
},
{
    "Live_Portsmouth_080430_1420_setanta_all/MUX546_Five.trp",
    "QVC",
    506000000,                  /* uint32_t  TunFrequency */
    8000000,                    /* uint32_t  SymbolRateBw */
    eTDAL_DMD_MOD_AUTO,         /* tTDAL_DMD_Modulation         Modulation */
    eTDAL_DMD_SPECTRUM_AUTO,    /* tTDAL_DMD_Spectrum           SpecInv */
    {
        eTDAL_DMD_MODE_AUTO,
        eTDAL_DMD_GUARD_AUTO,
        eTDAL_DMD_HIER_AUTO,
        eTDAL_DMD_PRIORITY_STREAM_HIGH,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_OFFSET_AUTO,
        0
    },
    0,
    0x17a1,
    0x17a2,
    0x17a1,
	eTDAL_AV_VIDEO_TYPE_MPEG2,
	eTDAL_AV_AUDIO_TYPE_MPEG,0,0
},
{
    "Live_Portsmouth_080430_1420_setanta_all/MUX546_Five.trp",
    "UKTV STYLE",
    506000000,                  /* uint32_t  TunFrequency */
    8000000,                    /* uint32_t  SymbolRateBw */
    eTDAL_DMD_MOD_AUTO,         /* tTDAL_DMD_Modulation         Modulation */
    eTDAL_DMD_SPECTRUM_AUTO,    /* tTDAL_DMD_Spectrum           SpecInv */
    {
        eTDAL_DMD_MODE_AUTO,
        eTDAL_DMD_GUARD_AUTO,
        eTDAL_DMD_HIER_AUTO,
        eTDAL_DMD_PRIORITY_STREAM_HIGH,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_OFFSET_AUTO,
        0
    },
    0,
    0x1911,
    0x1912,
    0x1911,
	eTDAL_AV_VIDEO_TYPE_MPEG2,
	eTDAL_AV_AUDIO_TYPE_MPEG,0,0
},
{
    "Live_Portsmouth_080430_1420_setanta_all/MUX546_Five.trp",
    "SETANTA SPORTS",
    506000000,                  /* uint32_t  TunFrequency */
    8000000,                    /* uint32_t  SymbolRateBw */
    eTDAL_DMD_MOD_AUTO,         /* tTDAL_DMD_Modulation         Modulation */
    eTDAL_DMD_SPECTRUM_AUTO,    /* tTDAL_DMD_Spectrum           SpecInv */
    {
        eTDAL_DMD_MODE_AUTO,
        eTDAL_DMD_GUARD_AUTO,
        eTDAL_DMD_HIER_AUTO,
        eTDAL_DMD_PRIORITY_STREAM_HIGH,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_OFFSET_AUTO,
        0
    },
    0,
    0x1a91,
    0x1a92,
    0x1a91,
	eTDAL_AV_VIDEO_TYPE_MPEG2,
	eTDAL_AV_AUDIO_TYPE_MPEG,0,0
},
{
    "Live_Portsmouth_080430_1420_setanta_all/MUX546_Five.trp",
    "SETANTA SPORTS (bis)",
    506000000,                  /* uint32_t  TunFrequency */
    8000000,                    /* uint32_t  SymbolRateBw */
    eTDAL_DMD_MOD_AUTO,         /* tTDAL_DMD_Modulation         Modulation */
    eTDAL_DMD_SPECTRUM_AUTO,    /* tTDAL_DMD_Spectrum           SpecInv */
    {
        eTDAL_DMD_MODE_AUTO,
        eTDAL_DMD_GUARD_AUTO,
        eTDAL_DMD_HIER_AUTO,
        eTDAL_DMD_PRIORITY_STREAM_HIGH,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_OFFSET_AUTO,
        0
    },
    0,
    0x1a91,
    0x1a92,
    0x1a91,
	eTDAL_AV_VIDEO_TYPE_MPEG2,
	eTDAL_AV_AUDIO_TYPE_MPEG,0,0
},

};

/*****************************************************************************
 *                       LOCALES FUNCTIONS PROTOTYPES                        *
 *****************************************************************************/

/*****************************************************************************
 *                             PUBLIC FUNCTIONS                              *
 *****************************************************************************/
static bool isGLUEinit = FALSE;
static bool isTDALinit = FALSE;


void GLUE4TESTS_Active_CI_Traces(void)
{
#ifndef NO_CIPLUS_STACK
	CI_Desc_lst[ID_PHYSICAL].niv_trace =  PHYSICAL_Trace_Level = NIV_ERR|NIV_WAR;
	CI_Desc_lst[ID_LINK].niv_trace = LINK_Trace_Level = NIV_ERR|NIV_WAR;
	CI_Desc_lst[ID_TRANSPORT].niv_trace = TRANSPORT_Trace_Level = NIV_ERR|NIV_WAR;
	CI_Desc_lst[ID_SESSION].niv_trace = SESSION_Trace_Level = NIV_ERR|NIV_WAR;
	CI_Desc_lst[ID_RESOURCE].niv_trace = RESOURCE_Trace_Level = NIV_ERR|NIV_WAR;
	CI_Desc_lst[ID_RESS_AI].niv_trace = AI_Trace_Level = NIV_ERR|NIV_WAR;
	CI_Desc_lst[ID_RESS_CAS].niv_trace = CA_Trace_Level = NIV_ERR|NIV_WAR;
	CI_Desc_lst[ID_RESS_MMI].niv_trace = MMI_Trace_Level = NIV_ERR|NIV_WAR;
	CI_Desc_lst[ID_RESS_AMMI].niv_trace = AMMI_Trace_Level = NIV_ERR|NIV_WAR;
	CI_Desc_lst[ID_RESEXT_CC].niv_trace = CC_Trace_Level = NIV_ERR|NIV_WAR;
	CI_Desc_lst[ID_RESS_HLC].niv_trace = HLC_Trace_Level = NIV_ERR|NIV_WAR;
	CI_Desc_lst[ID_RESS_UPGR].niv_trace = UPGR_Trace_Level = NIV_ERR|NIV_WAR;
	CI_Desc_lst[ID_RESS_SAS].niv_trace = SAS_Trace_Level = NIV_ERR|NIV_WAR;
	CI_Desc_lst[ID_RESS_LSC].niv_trace = LSC_Trace_Level = NIV_ERR|NIV_WAR;
	CI_Desc_lst[ID_RESS_DT].niv_trace = DT_Trace_Level = NIV_ERR|NIV_WAR;
	CI_Desc_lst[ID_RESS_HC].niv_trace = HC_Trace_Level = NIV_ERR|NIV_WAR;
	CI_Desc_lst[ID_RESS_AUTH].niv_trace = AUTH_Trace_Level = NIV_ERR|NIV_WAR;
#endif

	return;
}


void GLUE4TESTS_Init(void)
{
   	struct tm refTime;
    int hour, min, sec;
    int month, day, year;
    char sMonth[4];
    char scMonth[12][4] = {"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};

	/* current time is compil time + tick (see AVtests_cbDT_TimeRequest) */
	sscanf(__TIME__,"%u:%u:%u", &hour, &min, &sec); 	/* FORMAT -> 16:34:13 */
	refTime.tm_sec = sec;	
	refTime.tm_min = min;	
	refTime.tm_hour = hour;
	
	sscanf(__DATE__,"%s %u %u", sMonth, &day, &year); 	/* FORMAT -> Jan 13 2009 */
    for (month = 0; month < 12; month++) if (!strcmp(sMonth,scMonth[month])) break;
	refTime.tm_mday = day;
	refTime.tm_mon = month;         /* number of months since january */
	refTime.tm_year = year-1900;    /* number of years since 1900 */
	GLUE4TESTS_CurrentTime = mktime(&refTime);

    /* init only once */
    if (isTDALinit == FALSE)
    {
        /* for traces */
        ClientTestSetup();

        /* for low level driver */
        TDAL_Init();

        isTDALinit = TRUE;
    }

    if (isGLUEinit == FALSE)
    {
        /* for nvm */
        TDAL_FLA_Init();
        TDAL_FLA_Open();

        /* for desc */
        TDAL_DMX_Init();
        TDAL_DESC_Init();

        isGLUEinit = TRUE;
    }

	GLUE4TESTS_Active_CI_Traces();

}

void GLUE4TESTS_Terminate(void)
{
    if (isGLUEinit == TRUE)
    {
        TDAL_DESC_Terminate();
        TDAL_DMX_Terminate();
        TDAL_FLA_Close();
        TDAL_FLA_Term();
        isGLUEinit = FALSE;
    }
}


void GLUE4TESTS_StartAV(tGLUE4TESTS_Channel ChannelId)
{
    tTDAL_AV_StreamType StreamType;
    tTDAL_AV_VideoPictureParams stPictureParams;
	tTDAL_OUTPUT_VideoHDDACParams stHDDACParams;
	tTDAL_OUTPUT_Capability       stOutputCap;
    tTDAL_OUTPUT_AudioAnaSpeakerAttenuation stAudioAttenuation;
    tTDAL_OUTPUT_AudioAnaCapability stAudioCapability;

    stPictureParams.notCompressWidth            = 0;
    stPictureParams.notCompressHeight           = 0;
    stPictureParams.notCompressDataBufferPosX   = 0;
    stPictureParams.notCompressDataBufferPosY   = 0;
    stPictureParams.notCompressDataBufferWidth  = 0;
    stPictureParams.notCompressDataBufferHeight = 0;

    tTDAL_DISP_LayerWindow InputWind = {0, 0, 720, 576};
    tTDAL_DISP_LayerWindow OutputWind = {0, 0, 1920, 1080};

    /* select channel */
    GLUE4TESTS_Channel = ChannelId;
    if (ChannelId == NOT_SCRAMBLED_CHANNEL)
        gChannel = 0;
    else if (ChannelId == SCRAMBLED_CHANNEL_CIV1_1)
        gChannel = 1;
    else if (ChannelId == SCRAMBLED_CHANNEL_CIV1_2)
        gChannel = 2;
    else if (ChannelId == SCRAMBLED_CHANNEL_CIPLUS)
        gChannel = 3;
    else if (ChannelId == GLUE4TEST_CIplus_Cert_Test_Stream_Terpid32)
        gChannel = 4;
    else if (ChannelId == GLUE4TEST_CIplus_Cert_Test_Stream_Terpid35)
        gChannel = 5;
    else if (ChannelId == GLUE4TEST_CIplus_Cert_Test_Stream_Terpid38)
        gChannel = 6;
    else if (ChannelId == GLUE4TEST_CIplus_Cert_Test_Stream_Terpid41)
        gChannel = 7;
    else if (ChannelId == GLUE4TEST_CIplus_Cert_Test_Stream_Terpid44)
        gChannel = 8;
    else if (ChannelId == GLUE4TEST_CIplus_Cert_Test_Stream_Terpid47)
        gChannel = 9;
    else if (ChannelId == GLUE4TEST_CIplus_Cert_Test_Stream_Terpid50)
        gChannel = 10;
    else if (ChannelId == GLUE4TEST_Stream_CIplus_1_CLSCRv2_0pid24)
        gChannel = 11;
    else if (ChannelId == GLUE4TEST_Stream_CIplus_1_CLSCRv2_0pid32)
        gChannel = 12;
    else if (ChannelId == GLUE4TEST_Stream_CIplus_1_CLSCRv2_0pid40)
        gChannel = 13;
    else if (ChannelId == GLUE4TEST_Stream_CIplus_2_CCv2_0pid24)
        gChannel = 14;
    else if (ChannelId == GLUE4TEST_Stream_CIplus_2_CCv2_0pid32)
        gChannel = 15;
    else if (ChannelId == GLUE4TEST_Stream_CIplus_2_CCv2_0pid40)
        gChannel = 16;
    else if (ChannelId == GLUE4TEST_Stream_CIplus_3_DOTAv2_0pid24)
        gChannel = 17;
    else if (ChannelId == GLUE4TEST_Stream_CIplus_3_DOTAv2_0pid32)
        gChannel = 18;
    else if (ChannelId == GLUE4TEST_Stream_CIplus_3_DOTAv2_0pid40)
        gChannel = 19;

    else if (ChannelId == GLUE4TEST_Stream_VIACCESS_SWE_RF27_service1)
        gChannel = 20;
    else if (ChannelId == GLUE4TEST_Stream_VIACCESS_SWE_RF27_service2)
        gChannel = 21;
    else if (ChannelId == GLUE4TEST_Stream_VIACCESS_SWE_RF27_service3)
        gChannel = 22;
    else if (ChannelId == GLUE4TEST_Stream_VIACCESS_SWE_RF27_service4)
        gChannel = 23;
    else if (ChannelId == GLUE4TEST_Stream_VIACCESS_SWE_RF27_service5)
        gChannel = 24;
    else if (ChannelId == GLUE4TEST_Stream_VIACCESS_SWE_RF27_service1)
        gChannel = 25;
    else if (ChannelId == GLUE4TEST_Stream_VIACCESS_SWE_RF27_service6)
        gChannel = 26;

    else if (ChannelId == GLUE4TEST_Stream_CONAX_FIN_RF53_service1)
        gChannel = 27;
    else if (ChannelId == GLUE4TEST_Stream_CONAX_FIN_RF53_service2)
        gChannel = 28;
    else if (ChannelId == GLUE4TEST_Stream_CONAX_FIN_RF53_service3)
        gChannel = 29;
    else if (ChannelId == GLUE4TEST_Stream_CONAX_FIN_RF53_service4)
        gChannel = 30;
    else if (ChannelId == GLUE4TEST_Stream_CONAX_FIN_RF53_service5)
        gChannel = 31;
    else if (ChannelId == GLUE4TEST_Stream_CONAX_FIN_RF53_service6)
        gChannel = 32;
    else if (ChannelId == GLUE4TEST_Stream_CONAX_FIN_RF53_service7)
        gChannel = 33;
    else if (ChannelId == GLUE4TEST_Stream_CONAX_FIN_RF53_service8)
        gChannel = 34;

    else if (ChannelId == GLUE4TEST_Stream_CONAX_FIN_RF46_service1)
        gChannel = 35;
    else if (ChannelId == GLUE4TEST_Stream_CONAX_FIN_RF46_service2)
        gChannel = 36;
    else if (ChannelId == GLUE4TEST_Stream_CONAX_FIN_RF46_service3)
        gChannel = 37;
    else if (ChannelId == GLUE4TEST_Stream_CONAX_FIN_RF46_service4)
        gChannel = 38;
    else if (ChannelId == GLUE4TEST_Stream_CONAX_FIN_RF46_service5)
        gChannel = 39;
    else if (ChannelId == GLUE4TEST_Stream_CONAX_FIN_RF46_service6)
        gChannel = 40;
    else if (ChannelId == GLUE4TEST_Stream_CONAX_FIN_RF46_service7)
        gChannel = 41;
    else if (ChannelId == GLUE4TEST_Stream_CONAX_FIN_RF46_service8)
        gChannel = 42;
    else if (ChannelId == GLUE4TEST_Stream_CONAX_FIN_RF46_service9)
        gChannel = 43;
    else if (ChannelId == GLUE4TEST_Stream_CONAX_FIN_RF46_service10)
        gChannel = 44;
    else if (ChannelId == GLUE4TEST_Stream_CONAX_FIN_RF46_service11)
        gChannel = 45;
    else if (ChannelId == GLUE4TEST_Stream_CONAX_FIN_RF46_service12)
        gChannel = 46;
    else if (ChannelId == GLUE4TEST_Stream_CONAX_FIN_RF46_service13)
        gChannel = 47;
    else if (ChannelId == GLUE4TEST_Stream_Setanta_Prog_12866_Five)
        gChannel = 48;
    else if (ChannelId == GLUE4TEST_Stream_Setanta_Prog_12928_FIVER)
        gChannel = 49;
    else if (ChannelId == GLUE4TEST_Stream_Setanta_Prog_12992_Five_US)
        gChannel = 50;
    else if (ChannelId == GLUE4TEST_Stream_Setanta_Prog_13120_QVC)
        gChannel = 51;
    else if (ChannelId == GLUE4TEST_Stream_Setanta_Prog_14976_UKTV_STYLE)
        gChannel = 52;
    else if (ChannelId == GLUE4TEST_Stream_Setanta_Prog_16096_SETANTA_SPORTS)
        gChannel = 53;
    else if (ChannelId == GLUE4TEST_Stream_Setanta_Prog_20000_SETANTA_SPORTS)
        gChannel = 54;

    else
        return;

    /* Initialize tdal modules */
    TestHelper_Initialize_TDAL_DMD();

#ifdef AVS_STB
    TestManager_AssertEqual(TDAL_AVS_Init(),
							eTDAL_AVS_NO_ERROR,
							"avs init");
#endif

    TestManager_AssertEqual(TDAL_DISP_Init(),
							eTDAL_DISP_NO_ERROR,
							"disp init" );
    TestManager_AssertEqual(TDAL_OUTPUT_Init(),
							eTDAL_OUTPUT_NO_ERROR,
							"output init" );
    TestManager_AssertEqual(TDAL_HDMI_Init(),
							eTDAL_HDMI_NO_ERROR,
							"hdmi init" );

	memset(&stOutputCap, 0, sizeof(tTDAL_OUTPUT_Capability));
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
	}
#endif

	if(stOutputCap.nbVideoDenc > 0)
	{
		TestManager_AssertEqual(TDAL_OUTPUT_VideoDencStandardSet(eTDAL_OUTPUT_VIDEO_DENC_ID_0,
																 eTDAL_OUTPUT_VIDEO_SD_STD_PAL_BG),
								eTDAL_OUTPUT_NO_ERROR,
								"output denc STD set");

		TestManager_AssertEqual(TDAL_OUTPUT_Enable(eTDAL_OUTPUT_VIDEO_DENC_ID_0),
								eTDAL_OUTPUT_NO_ERROR,
								"output denc video enable" );
	}

    TestManager_AssertEqual(TDAL_TSROUTE_Init(),
							eTDAL_TSROUTE_NO_ERROR,
							"tsroute init");
    TestManager_AssertEqual(TDAL_AV_Init(),
							eTDAL_AV_NO_ERROR,
							"av init");
    TestHelper_Configure_TDAL_AVS();

    TestHelper_Configure_TDAL_TSROUTE(TEST_HELPER_TSROUTE_TUNER(GLUE4TESTS_TestFeIdx),
                                      TEST_HELPER_TSROUTE_DEMUX(GLUE4TESTS_TestDmxIdx));

    TestHelper_DMDTune(GLUE4TESTS_TestFeIdx);
    TestHelper_Configure_TDAL_DMX(GLUE4TESTS_TestDmxIdx);

    TestManager_AssertEqual(TDAL_AV_InputStreamSet(eTDAL_AV_DECODER_VIDEO_1,
										  			TestHelper_DMXGetStreamHandle(eTDAL_AV_DECODER_VIDEO_1)),
							eTDAL_AV_NO_ERROR,
							"TDAL AV input stream set\n");

    StreamType.videoType = gChannelList[gChannel].videoType;
    TestManager_AssertEqual(TDAL_AV_Start(eTDAL_AV_DECODER_VIDEO_1,
										  StreamType),
							eTDAL_AV_NO_ERROR,
							"1st av start video");

    TestManager_AssertEqual(TDAL_DISP_LayerEnable(eTDAL_DISP_LAYER_VIDEO_ID_0),
							eTDAL_DISP_NO_ERROR,
							"1st disp layer video enable" );

    TestManager_AssertEqual(TDAL_DISP_LayerIOWindowsSet(eTDAL_DISP_LAYER_VIDEO_ID_0,
                                                        &InputWind, &OutputWind),
                            							eTDAL_DISP_NO_ERROR,
                            							"Resize layer video" );

		TestManager_AssertEqual(TDAL_OUTPUT_AudioAnaCapabilityGet(eTDAL_OUTPUT_AUDIO_ANA_ID_0,
																  &stAudioCapability),
								eTDAL_OUTPUT_NO_ERROR,
								"output audio capability get");
		stAudioAttenuation.FrontRLAttenuation     = stAudioCapability.AttenuationMax;
		stAudioAttenuation.FrontCenterAttenuation = stAudioCapability.AttenuationMax;
		stAudioAttenuation.SurroundRLAttenuation  = stAudioCapability.AttenuationMax;
		stAudioAttenuation.LFEAttenuation         = stAudioCapability.AttenuationMax;
		stAudioAttenuation.RearRLAttenuation      = stAudioCapability.AttenuationMax;
		stAudioAttenuation.RearCenterAttenuation  = stAudioCapability.AttenuationMax;
		TestManager_AssertEqual(TDAL_OUTPUT_AudioAnaSpeakerAttenuationSet(eTDAL_OUTPUT_AUDIO_ANA_ID_0,
																		  &stAudioAttenuation),
								eTDAL_OUTPUT_NO_ERROR,
								"output audio volume set");

		TestManager_AssertEqual(TDAL_AV_InputStreamSet(eTDAL_AV_DECODER_AUDIO_1,
										  			TestHelper_DMXGetStreamHandle(eTDAL_AV_DECODER_AUDIO_1)),
							eTDAL_AV_NO_ERROR,
							"TDAL AV input stream set\n");

        StreamType.audioType = gChannelList[gChannel].audioType;
        TestManager_AssertEqual(TDAL_AV_Start(eTDAL_AV_DECODER_AUDIO_1,
											  StreamType),
								eTDAL_AV_NO_ERROR,
								"1st av start audio");
        TestManager_AssertEqual(TDAL_OUTPUT_Enable(eTDAL_OUTPUT_AUDIO_ANA_ID_0),
								eTDAL_OUTPUT_NO_ERROR,
								"1st output audio enable" );
}

void GLUE4TESTS_StopAV(void)
{
    tTDAL_AV_StreamType StreamType;

    /* add a zapping */
    TestManager_AssertEqual(TDAL_DISP_LayerDisable(eTDAL_DISP_LAYER_VIDEO_ID_0),
							eTDAL_DISP_NO_ERROR,
							"1st disp layer video disable" );
    TestManager_AssertEqual(TDAL_AV_Stop(eTDAL_AV_DECODER_VIDEO_1),
							eTDAL_AV_NO_ERROR,
							"1st av stop video");
    TestManager_AssertEqual(TKEL_DelayTask(ms2tick(500)),
							TKEL_NO_ERR,
							"1st wait for 500ms");

    StreamType.audioType = eTDAL_AV_AUDIO_TYPE_MPEG;
    TestManager_AssertEqual(TDAL_AV_Stop(eTDAL_AV_DECODER_AUDIO_1),
							eTDAL_AV_NO_ERROR,
							"1st av stop audio");
    TestManager_AssertEqual(TDAL_OUTPUT_Disable(eTDAL_OUTPUT_AUDIO_ANA_ID_0),
							eTDAL_OUTPUT_NO_ERROR,
							"1st output audio disable" );

    TestManager_AssertEqual(TDAL_OUTPUT_Disable(eTDAL_OUTPUT_AUDIO_ANA_ID_0),
							eTDAL_OUTPUT_NO_ERROR,
							"2nd output audio disable" );

    TestManager_AssertEqual(TDAL_DISP_LayerDisable(eTDAL_DISP_LAYER_VIDEO_ID_0),
							eTDAL_DISP_NO_ERROR,
							"7th disp layer disable" );
    TestManager_AssertEqual(TDAL_AV_Stop(eTDAL_AV_DECODER_AUDIO_1),
							eTDAL_AV_NO_ERROR,
							"av stop audio");

    TestManager_AssertEqual(TDAL_AV_Terminate(),
							eTDAL_AV_NO_ERROR,
							"av terminate");

    /* terminate tdal modules */
    TestHelper_Terminate_TDAL_DMD();
    TestHelper_Unconfigure_TDAL_DMX();
    TestHelper_Unconfigure_TDAL_TSROUTE(TEST_HELPER_TSROUTE_TUNER(GLUE4TESTS_TestFeIdx),
                                        TEST_HELPER_TSROUTE_DEMUX(GLUE4TESTS_TestDmxIdx));
    TestManager_AssertEqual(TDAL_TSROUTE_Term(),
							eTDAL_TSROUTE_NO_ERROR,
							"tsroute terminate");

    TestManager_AssertEqual(TDAL_DISP_Term(),
							eTDAL_DISP_NO_ERROR,
							"disp terminate" );
    TestManager_AssertEqual(TDAL_OUTPUT_Term(),
							eTDAL_OUTPUT_NO_ERROR,
							"output terminate" );
    TestManager_AssertEqual(TDAL_HDMI_Term(),
							eTDAL_HDMI_NO_ERROR,
							"hdmi terminate" );
#ifdef AVS_STB
    TestManager_AssertEqual(TDAL_AVS_Term(),
							eTDAL_AVS_NO_ERROR,
							"avs terminate");
#endif
}

int GLUE4TESTS_GetCurrentPMT(unsigned char * pPmt, unsigned short *pPmtSize)
{
    switch(GLUE4TESTS_Channel)
    {
        case SCRAMBLED_CHANNEL_CIV1_1:
            memcpy(pPmt, PmtScrambCONAX2, sizeof(PmtScrambCONAX2));
            *pPmtSize = sizeof(PmtScrambCONAX2);
            break;
        case SCRAMBLED_CHANNEL_CIV1_2:
            memcpy(pPmt, PmtScrambCONAX3, sizeof(PmtScrambCONAX3));
            *pPmtSize = sizeof(PmtScrambCONAX3);
            break;
        case SCRAMBLED_CHANNEL_CIPLUS:
            memcpy(pPmt, PmtScrambChannel2, sizeof(PmtScrambChannel2));
            *pPmtSize = sizeof(PmtScrambChannel2);
            break;
        default:
            return false;
    }

    return true;
}
/*****************************************************************************
 * End of File:   glue4tests.c                                         *
 *****************************************************************************/
typedef enum
{
    eMHEG               = 0x10080000,
    eMHEG_ASN1_PARSER   = 0x10080001,
    eMHEG_CLASS_HANDLER = 0x10080002,
    eMHEG_MNGR          = 0x10080003,
	eMHEG_DISPLAY       = 0x10080004,
	eMHEG_OSD           = 0x10080005,
	eMHEG_STR           = 0x10080006,
	eMHEG_TSCHED        = 0x10080007,
	eMHEG_OBJ			= 0x10080008,
	eMHEG_RP            = 0x10080009,
	eMHEG_FILEMNGR      = 0x1008000A,
	eMHEG_IC            = 0x1008000B,
	eMHEG_AV            = 0x1008000C
} tMHEG_TracesID;

typedef enum
{
    /* TBOX ID management - Possible values from 0x04660000 to 0x0466FFFF */
    eTRD_API = 0x04660000
} tTRD_TracesID;
typedef enum
{
    /* TBOX ID management - Possible values from 0x04650000 to 0x0465FFFF */
    eFNT_API = 0x04650000
} tFNT_TracesID;


LOCAL void ClientTestSetup(void)
{
	mTBOX_INIT_MOD_TRACE((eMHEG, kTBOX_TRC_CRITICAL+kTBOX_TRC_WARNING));
	mTBOX_INIT_MOD_TRACE((eMHEG_MNGR,  kTBOX_TRC_CRITICAL+kTBOX_TRC_WARNING));
	mTBOX_INIT_MOD_TRACE((eMHEG_CLASS_HANDLER,	kTBOX_TRC_CRITICAL+kTBOX_TRC_WARNING));

	mTBOX_INIT_MOD_TRACE((eMHEG_DISPLAY,	kTBOX_TRC_CRITICAL+kTBOX_TRC_WARNING));
	mTBOX_INIT_MOD_TRACE((eMHEG_OSD,	kTBOX_TRC_CRITICAL+kTBOX_TRC_WARNING));
	mTBOX_INIT_MOD_TRACE((eMHEG_STR,	kTBOX_TRC_CRITICAL+kTBOX_TRC_WARNING));

	mTBOX_INIT_MOD_TRACE((eFNT_API,	kTBOX_TRC_CRITICAL+kTBOX_TRC_WARNING));
	mTBOX_INIT_MOD_TRACE((eTRD_API,	kTBOX_TRC_CRITICAL+kTBOX_TRC_WARNING));

	mTBOX_TRACE_ENABLED((TRUE));
    mTBOX_TRACE_SYNCHRONE((TRUE));
}
