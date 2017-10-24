/******************************************************************************
 *                     COPYRIGHT 2004 IWEDIA TECHNOLOGIES                     *
 ******************************************************************************
 *
 * MODULE NAME: TKEL
 *
 * FILE NAME: $URL$
 *            $Rev$
 *            $Date$
 *
 * DESCRIPTION : Miscellaneous API of Thin Kernel Encapsulation Layer (T-KEL)
 *
 *****************************************************************************/


/********************************************************/
/*              Includes                                */
/********************************************************/
#include "crules.h"

#include "TestManager.h"

#include "tbox.h"
#include "tkel.h"

#include "tdal_common.h"
#include "testhelper.h"

#ifdef ENABLE_CORE_DUMP
	#include <stdio.h>
	#include <execinfo.h>
	#include <stdlib.h>
	#include <sys/prctl.h>
	#include <sys/time.h>
	#include <sys/resource.h>
	void TMP_CoreDump();
#endif

/*************************************************************************
 *  Defines                                                              *
 *************************************************************************/

/*************************************************************************
 *  LOCAL variables                                                      *
 *************************************************************************/
void InitTrace()
{
	//printf("\n\t InitTrace() \n\n");
	/* TDAL Traces */
	mTBOX_INIT_MOD_TRACE((eTDAL_UART,       kTBOX_TRC_ALL));
	mTBOX_INIT_MOD_TRACE((eTDAL_DESC,       kTBOX_TRC_ALL));
	mTBOX_INIT_MOD_TRACE((eTDAL_VBI,        kTBOX_TRC_ALL));
	mTBOX_INIT_MOD_TRACE((eTDAL_SMC,        kTBOX_TRC_ALL));
	mTBOX_INIT_MOD_TRACE((eTDAL_PTSM,       kTBOX_TRC_ALL));
	mTBOX_INIT_MOD_TRACE((eTDAL_GFX,        kTBOX_TRC_ALL));
	mTBOX_INIT_MOD_TRACE((eTDAL_COMMON,     kTBOX_TRC_ALL));
	//mTBOX_INIT_MOD_TRACE((eTDAL_DMX,        kTBOX_TRC_ALL));
	mTBOX_INIT_MOD_TRACE((eTDAL_DMX,        kTBOX_TRC_ALL));
	//mTBOX_INIT_MOD_TRACE((eTDAL_DMD,        kTBOX_TRC_ALL));
	mTBOX_INIT_MOD_TRACE((eTDAL_DMD,        kTBOX_TRC_ALL));
	//mTBOX_INIT_MOD_TRACE((eTDAL_AVS,        kTBOX_TRC_CRITICAL+kTBOX_TRC_WARNING));
	mTBOX_INIT_MOD_TRACE((eTDAL_AVS,        kTBOX_TRC_ALL));
	//mTBOX_INIT_MOD_TRACE((eTDAL_KBD,        kTBOX_TRC_CRITICAL+kTBOX_TRC_WARNING));
	mTBOX_INIT_MOD_TRACE((eTDAL_KBD,        kTBOX_TRC_ALL));
	mTBOX_INIT_MOD_TRACE((eTDAL_NVM,        kTBOX_TRC_ALL));
	mTBOX_INIT_MOD_TRACE((eTDAL_CRYPT,      kTBOX_TRC_ALL));
	mTBOX_INIT_MOD_TRACE((eTDAL_FLA,        kTBOX_TRC_ALL));
	mTBOX_INIT_MOD_TRACE((eTDAL_FP,         kTBOX_TRC_ALL));
	mTBOX_INIT_MOD_TRACE((eTDAL_AV,         kTBOX_TRC_CRITICAL));
	mTBOX_INIT_MOD_TRACE((eTDAL_DENC,       kTBOX_TRC_ALL));
	mTBOX_INIT_MOD_TRACE((eTDAL_TSOUT,      kTBOX_TRC_ALL));
	mTBOX_INIT_MOD_TRACE((eTDAL_TSIN,       kTBOX_TRC_ALL));
	mTBOX_INIT_MOD_TRACE((eTDAL_TS_ROUTE,   kTBOX_TRC_ALL));
	mTBOX_INIT_MOD_TRACE((eTDAL_TS_SOURCE,  kTBOX_TRC_ALL));
	mTBOX_INIT_MOD_TRACE((eTDAL_MP, 	 	kTBOX_TRC_ALL));
	mTBOX_INIT_MOD_TRACE((eTDAL_TS_SINK,    kTBOX_TRC_ALL));
	mTBOX_INIT_MOD_TRACE((eTDAL_ATA,        kTBOX_TRC_ALL));
    mTBOX_INIT_MOD_TRACE((eTDAL_HDMI,       kTBOX_TRC_ALL));
    mTBOX_INIT_MOD_TRACE((eTDAL_FS,         kTBOX_TRC_ALL));
	mTBOX_INIT_MOD_TRACE((eTDAL_OUTPUT,     kTBOX_TRC_ALL));
	mTBOX_INIT_MOD_TRACE((eTDAL_DISP,       kTBOX_TRC_ALL));
    mTBOX_INIT_MOD_TRACE((eTDAL_ANA_INPUT,  kTBOX_TRC_ALL));
    mTBOX_INIT_MOD_TRACE((eTDAL_VBI_SLICER, kTBOX_TRC_ALL));
    mTBOX_INIT_MOD_TRACE((eTDAL_ANA_AUD,    kTBOX_TRC_ALL));
    mTBOX_INIT_MOD_TRACE((eTDAL_ANA_VID,    kTBOX_TRC_ALL));
    mTBOX_INIT_MOD_TRACE((eTDAL_PANEL,      kTBOX_TRC_ALL/*kTBOX_TRC_CRITICAL+kTBOX_TRC_WARNING*/));
#if defined (PRODUCT_USE_HDMI_CEC)    
    mTBOX_INIT_MOD_TRACE((eTDAL_HDMI_CEC,   kTBOX_TRC_ALL/*kTBOX_TRC_CRITICAL+kTBOX_TRC_WARNING*/));
#endif

	/* TDAL Tests Traces */
    //mTBOX_INIT_MOD_TRACE((eTDAL_TEST_COMMON, kTBOX_TRC_CRITICAL|kTBOX_TRC_1));
	mTBOX_INIT_MOD_TRACE((eTDAL_TEST_COMMON, kTBOX_TRC_ALL));

	mTBOX_INIT_MOD_TRACE((eTDAL_TEST_DMD,    kTBOX_TRC_ALL));
    mTBOX_INIT_MOD_TRACE((eTDAL_TEST_DMX,    kTBOX_TRC_ALL));
    //mTBOX_INIT_MOD_TRACE((eTDAL_TEST_DMX,    kTBOX_TRC_ALL));
    mTBOX_INIT_MOD_TRACE((eTDAL_TEST_GFX,    kTBOX_TRC_ALL));
    mTBOX_INIT_MOD_TRACE((eTDAL_TEST_FLA,    kTBOX_TRC_ALL));
    mTBOX_INIT_MOD_TRACE((eTDAL_TEST_NVM,    kTBOX_TRC_ALL));
    mTBOX_INIT_MOD_TRACE((eTDAL_TEST_HDMI,   kTBOX_TRC_ALL));
    mTBOX_INIT_MOD_TRACE((eTDAL_TEST_KBD,    kTBOX_TRC_ALL));
    mTBOX_INIT_MOD_TRACE((eTDAL_TEST_DESC,   kTBOX_TRC_ALL));
    mTBOX_INIT_MOD_TRACE((eTDAL_TEST_ANA,    kTBOX_TRC_ALL));

    //temporary
    mTBOX_INIT_MOD_TRACE((eTKEL_COM,		 kTBOX_TRC_CRITICAL || kTBOX_TRC_WARNING));
    mTBOX_INIT_MOD_TRACE((eTKEL_SYNC,        kTBOX_TRC_CRITICAL || kTBOX_TRC_WARNING));
    mTBOX_INIT_MOD_TRACE((eTKEL_TIMER,       kTBOX_TRC_CRITICAL || kTBOX_TRC_WARNING));
    mTBOX_INIT_MOD_TRACE((eTKEL_TASK,        kTBOX_TRC_CRITICAL || kTBOX_TRC_WARNING));
    mTBOX_INIT_MOD_TRACE((eTKEL_SYNC,        kTBOX_TRC_CRITICAL || kTBOX_TRC_WARNING));
    mTBOX_INIT_MOD_TRACE((eTKEL_MISC,        kTBOX_TRC_CRITICAL || kTBOX_TRC_WARNING));


}

GLOBAL void ClientTestSetup()
{
	InitTrace();
#ifdef ENABLE_CORE_DUMP
	TMP_CoreDump();
#endif
	
	mTBOX_TRACE_ENABLED((TRUE));
	mTBOX_TRACE_SYNCHRONE((TRUE));

	mTBOX_TRACE_LOC_ENABLED((TRUE));
    mTBOX_TRACE_TIME_ENABLED((FALSE));
}


#ifdef ENABLE_CORE_DUMP
	void TMP_CoreDump()
	{
		int ret;
		struct rlimit rlim;
		//printf("Core Dump Enabled!!!\n");
		/* Get the Dumpable state */
		ret = prctl( PR_GET_DUMPABLE, 0, 0, 0, 0 );
		//printf( "PR_GET_DUMPABLE returned %d\n", ret );

		/* Set the Dumpable state to be enabled */
		ret = prctl( PR_SET_DUMPABLE, 1, 0, 0, 0 );
		//printf( "PR_SET_DUMPABLE returned %d\n", ret );

		/* Get the Dumpable state again, make sure it was set */
		ret = prctl( PR_GET_DUMPABLE, 0, 0, 0, 0 );
		//printf( "PR_GET_DUMPABLE returned %d\n", ret );

		/* Get the core dump limitation */
		ret = getrlimit(RLIMIT_CORE, &rlim);
		//printf( "RLIMIT_CORE returned %d (%d, %d)\n", ret, rlim.rlim_cur, rlim.rlim_max );

		/* Set the core dump limitation to be unlimited */
		rlim.rlim_cur = RLIM_INFINITY;
		rlim.rlim_max = RLIM_INFINITY;
		ret = setrlimit(RLIMIT_CORE, &rlim);
		//printf( "RLIMIT_CORE returned %d\n", ret );

		/* Get the core dump limitation again */
		ret = getrlimit(RLIMIT_CORE, &rlim);
		//printf( "RLIMIT_CORE returned %d (%d, %d)\n", ret, rlim.rlim_cur, rlim.rlim_max );
	}
#endif
