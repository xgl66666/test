/******************************************************************************
 *                     COPYRIGHT 2006 IWEDIA TECHNOLOGIES                     *
 ******************************************************************************
 *
 * MODULE NAME: TKEL
 *
 * FILE NAME: $URL$
 *            $Rev$
 *            $Date$
 *
 * DESCRIPTION : Define how is organized root node of the test suite
 *
 *****************************************************************************/


/********************************************************/
/*              Includes                                */
/********************************************************/
#include "crules.h"

#include "TestManager.h"

#include "tkeltests.h"
#include "avtests.h"
#include "flatests.h"
#include "nvmtests.h"
#include "kbdtests.h"
#include "tstests.h"
#include "disptests.h"
#if defined(USE_TDAL_FS)
#include "fstests.h"
#endif
#if defined(USE_TDAL_MP)
#include "mptests.h"
#endif
#if defined(USE_TDAL_PVR)
#include "pvrtests.h"
#endif
#ifdef TDAL_HDMI_USE
#include "hdmitests.h"
#endif
#include "outputtests.h"
#if defined(PRODUCT_USE_CI)
#include "citests.h"
#endif
#if defined(PRODUCT_USE_CIPLUS)
#include "ciplustests.h"
#include "desctests.h"
#endif
#include "analogtests.h"
#if defined(PRODUCT_USE_HDMI_CEC)
#include "hdmicectests.h"
#endif
#if defined(USE_TDAL_SSU)
#include "ssutests.h"
#endif
/* TDAL */
tTestNode* gTDALTestNodeList[] = {
	&gTDAL_DMD_TestSuite,
	&gTDAL_DMX_TestSuite,
	&gTDAL_GFX_TestSuite,
	&gTDAL_AV_TestSuite,
	&gTDAL_FLA_TestSuite,
	&gTDAL_NVM_TestSuite,
	&gTDAL_KBD_TestSuite,
#if defined(PACKAGE_PVR) && !defined(PVR_CORE_IMPLEMENTATION_TDAL_PVR)
    #ifdef USE_TDAL_PVR
	    &gTDAL_PVR_TestSuite,
    #else
	    &gTDAL_TS_TestSuite,
    #endif
#endif
#if defined(USE_TDAL_FS)
	&gTDAL_FS_TestSuite,
#endif
#if defined(USE_TDAL_MP)
	&gTDAL_MP_TestSuite,
#endif
  #ifdef TDAL_HDMI_USE
    &gTDAL_HDMI_TestSuite,
  #endif
	&gTDAL_OUTPUT_TestSuite,
	&gTDAL_Disp_TestSuite,
#if defined(PRODUCT_USE_CI)
	&gTDAL_CI_TestSuite,
#endif
#if defined(PRODUCT_USE_CIPLUS)
	&gTDAL_CIPLUS_TestSuite,
	&gTDAL_DESC_TestSuite,
#endif
#if defined(ANALOG)
    &gTDAL_ANALOG_TestSuite,
#endif
#if defined(PRODUCT_USE_HDMI_CEC)
    &gTDAL_HDMI_CEC_TestSuite,
#endif
#if defined(USE_TDAL_SSU)
  &gTDAL_SSU_TestSuite,
#endif
	NULL
};

tTestNode gTDALTestSuite = {
    "TestTDAL",
    "test the tdal modules",
    0, /* not interactive */
    NULL, /* no function, it's a test suite */
		(void**)gTDALTestNodeList
};

/* ROOT */
tTestNode* gRootTestNodeList[] = {
    &gTKELTestSuite,
	&gTDALTestSuite,
	NULL
};

tTestNode gTestRoot = {
    "root",
    "the test root",
    0, /* not interactive */
    NULL,
    (void**)gRootTestNodeList
};
