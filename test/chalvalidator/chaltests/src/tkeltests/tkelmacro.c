/** \file tkelmacro.c
 *  \brief Test TKEL macro
 *  \author Olivier DOLE
 *  \date 16/03/06
 *  \version 1.0
 */

/********************************************************/
/*              Includes                                */
/********************************************************/
#include <stdio.h>
#include <signal.h>
#include <time.h>

#include "tkel.h"
#include "TestManager.h"

void TestCase_diff_tick2ms( void );
void TestCase_ExecuteTestms2tick2ms( void );

static tTestNode gTestMacro_diff_tick2ms = {
	"TestMacro_diff_tick2ms",
	"test the macro tick2ms()",
	0, /* not interactive */
	TestCase_diff_tick2ms,
	NULL
};

static tTestNode gTestMacro_ms2tick2ms = {
	"TestMacro_ms2tick2ms",
	"test conversion from ms to tick, then tick to ms, and compare",
	0, /* not interactive */
	TestCase_ExecuteTestms2tick2ms,
	NULL
};

tTestNode* gTKELMacroTestNodeList[] = {
	&gTestMacro_diff_tick2ms,
	&gTestMacro_ms2tick2ms,
	NULL
};

tTestNode gTKELMacroTestSuite = {
	"TestMacro",
	"test the macros",
	0, /* not interactive */
	NULL, /* no function, it's a test suite */
	(void**)gTKELMacroTestNodeList
};

/*! \fn void TestCase_diff_tick2ms( void )
 *  \brief Test the macro tick2ms()
 *
 *  This test needs to really work that macro ms2tick is OK.
 *  As a matter of fact this test uses the macro ms2tick in a
 *  TKEL_DelayTask() call.
 *
 \verbatim
 Retrieve the current tick number <BR>
 Wait for a period of 200ms (use of the macro ms2tick) <BR>
 Retrieve the current tick number <BR>
 Do the difference between tick number and check that 200ms or more is spent <BR>
 Do this algorithm 100 times
 \endverbatim
 *
 *  \bug there was a bug in tkelos20.h header (from trunk) \n
 * The implementation was: \n
 *   #define tick2ms(ticks)          (TKEL_uSecondsPerTick*ticks/1000) \n
 * It shoud be fixed as: \n
 *   #define tick2ms(ticks)
 *		(((ticks) / TKEL_ticksPerMillisecond) +
 *		 ((ticks) % TKEL_ticksPerMillisecond) / TKEL_ticksPerMillisecond ) \n
 * With a division with a tickpermillisecond, more precise than multiplication \n
 * with a secondpertick (rounding imprecision). \n
 * And with the parenthesis around the parameter.
 */
void TestCase_diff_tick2ms()
{
	tTestManagerChrono chrono;
	#define time_to_wait 200
	int i = 0;

	for(i = 0; i < 100; i++)
	{
		TestManager_AssertEqual(TKEL_GetTick(&chrono.mStartTime), TKEL_NO_ERR, "retrieve #ticks");
		TestManager_AssertEqual(TKEL_DelayTask(ms2tick(time_to_wait)), TKEL_NO_ERR, "waiting 200 ms");
		TestManager_AssertEqual(TKEL_GetTick(&chrono.mStopTime), TKEL_NO_ERR, "retrieve #ticks");

		TestManager_AssertTrue( chrono.mStopTime > chrono.mStartTime, "stop time greater than start time" );
		TestManager_AssertTrue(
			tick2ms(chrono.mStopTime - chrono.mStartTime) >= time_to_wait, "Spent more than 200ms to wait");
		TestManager_AssertTrue(
			(tick2ms(chrono.mStopTime) - tick2ms(chrono.mStartTime)) >= time_to_wait, "Spent more than 200ms to wait");
/*		TestManager_AssertGreater(
			tick2ms(chrono.mStopTime) - tick2ms(chrono.mStartTime), time_to_wait, "Spent more than 200ms to wait"); */
	}
}

/*! \fn void TestCase_ExecuteTestms2tick2ms( void )
 *  \brief execute ms2tick, then tick2ms, and compare
 *  \bug there was a minor bug in tkelos20.h header (from trunk) \n
 * The implementation was: \n
 *  #define ms2tick(milliseconds)	(TKEL_tck) ( ((TKEL_tck)milliseconds) * TKEL_ticksPerSecond / 1000 ) \n
 * \n
 * To ensure that ms2tick(tick2ms(a)) == a, it should be fixed as: \n
 *  #define ms2tick(milliseconds) (TKEL_tck)((milliseconds) * TKEL_ticksPerMillisecond ) \n
 */
void TestCase_ExecuteTestms2tick2ms()
{
	unsigned long aMs = 0;
	int aStep = 1;

    if (TKEL_ticksPerSecond < 1000)
    {
        /*
         * there is less than 1000 ticks per seconds:
         * steps lower than (1000 / nb ticks per seconds) do not make sense.
         */
        aStep = 1000 / TKEL_ticksPerSecond;
    }

	for( aMs = 0; aMs < 200000; aMs += aStep )
	{
		TKEL_tck aTick;
		unsigned long aResMs;
		unsigned long aResMsPlus300;
		unsigned long aResMsMinus300;

		aTick = ms2tick(aMs);
		aResMs = tick2ms(aTick);

		TestManager_AssertEqual( aMs, aResMs, "ms is equal to ms2tick2ms" );

		aResMsPlus300 = tick2ms(aTick + ((TKEL_ticksPerSecond * 3) / 10));
		TestManager_AssertEqual( aResMsPlus300, aResMs+300, "check positive tick shift of 300ms" );

		if( aMs > 300 )
		{
			aResMsMinus300 = tick2ms(aTick - ((TKEL_ticksPerSecond * 3) / 10));
			TestManager_AssertEqual( aResMsMinus300, aResMs-300, "check negative tick shift of 300ms" );
		}

		if( aMs == 1000 )
		{
			/* after 1000, goes by steps of 500 */
            if (aStep < 500)
            {
                aStep = 500;
            }
		}
	}
}
