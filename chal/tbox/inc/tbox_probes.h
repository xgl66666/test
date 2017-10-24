/******************************************************************************
 *             COPYRIGHT 2009 IWEDIA TECHNOLOGIES             *
 ******************************************************************************
 *
 * MODULE NAME: TBOX
 *
 * FILE NAME: $URL: http://10.50.1.219/svn/sources/hal/tbox/branches/comediaV21/inc/tbox_probes.h $
 *       $Rev: 59073 $
 *       $Date: 2009-12-02 17:08:24 +0100 (mer., 02 déc. 2009) $
 *
 * DESCRIPTION : PROBES API
 */
/** @section TBOX_PROBE The Embedded Tester
 *
 *
 * @see The file inc/embt_main.h details this interface technically.
 *
 * @file
 * This is the Embedded Tester.
 *
 *
 * Here is an use case:
 * @code
 * @endcode
 *
 *****************************************************************************/

#ifndef _TBOX_PROBES_H
#define _TBOX_PROBES_H


/*******************************************************/
/*          Includes                   */
/*******************************************************/

#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif


/*******************************************************/
/*          Defines                   */
/*******************************************************/

/** The timestamp marking a probe that never fired. */
#define TBOX_PROBE_NEVER_FIRED 0x00000000


/*******************************************************/
/*          Macros                     */
/*******************************************************/

#ifdef TBOX_PROBES

#include "embt_main.h"

/* Helpers for location */
#define TBOX_PROBE_STRINGIFY(x) #x
#define TBOX_PROBE_AS_STRING(x) TBOX_PROBE_STRINGIFY(x)
#define TBOX_PROBE_LOCATION() __FILE__ ":" TBOX_PROBE_AS_STRING(__LINE__)

/** Declaring a module for the Embedded Tester.
 *
 * Use this macro at top-level to register a new module.
 *
 * @param name is a static string used as human-readable name for this module, which
 * will be used in the interface menu.
 *
 * @example Use case:
 * @code
 * TBOX_PROBE_MODULE( "IRDETO Core" );
 * @endcode
 *
 */
#define TBOX_PROBE_MODULE( name ) \
   static tEMBT_Module TBOX_PROBE_Module = { name, 0, 0 }

/** Declaring a scenario for the Embedded Tester.
 *
 * Use this macro at function-level to register a new scenario.
 *
 * @note Do not use this macro in a loop, it relies on declarations to make
 * each scenario unique.
 *
 * @param name is a static string used as human-readable name for this scenario, which
 * will be used in the interface menu.
 *
 * @example Use case:
 * @code
 * TBOX_PROBE_MODULE( IRDETO Core );
 * ...
 * void my_scenario_registrar( void )
 * {
 *    TBOX_PROBE_SCENARIO( "Testing this feature", my_feature_test_function );
 * }
 *
 * eTBOX_PROBE_Error my_feature_test_function( void )
 * {
 *    // Run the scenario steps and check probes
 *    ...
 *
 *    // Return the result
 *    return scenario_failed ? TBOX_PROBE_FAILURE : TBOX_PROBE_SUCCESS;
 * }
 * @endcode
 *
 */
#define TBOX_PROBE_SCENARIO( name, entry ) { \
   static tEMBT_Scenario TBOX_PROBE_Scenario = { name, entry, EMBT_NOT_TESTED, 0 }; \
   EMBT_RegisterScenario( &TBOX_PROBE_Module, &TBOX_PROBE_Scenario ); }

/** Firing a named probe.
 *
 * Use the probe name with TBOX_PROBE_CheckProbe() to check if it fired and when.
 *
 * @param name is a static string used as human-readable name for this probe.
 *
 */
#define TBOX_PROBE_FIRE_LOCATE_PROBE( name, file, line ) { \
   static char TBOX_PROBE_ProbeName[50];\
   static char TBOX_PROBE_ProbeLocation[50];\
   static tEMBT_Probe TBOX_PROBE_Probe = { TBOX_PROBE_ProbeName, TBOX_PROBE_ProbeLocation, 0, 0 }; \
   sprintf(TBOX_PROBE_ProbeLocation, "%s : %d", file, line); \
   sprintf(TBOX_PROBE_ProbeName, "%s", name); \
   EMBT_FireProbe( &TBOX_PROBE_Probe ); }

#define TBOX_PROBE_FIRE_PROBE(name) {\
   static tEMBT_Probe TBOX_PROBE_Probe = {name, TBOX_PROBE_LOCATION(), 0, 0};\
   EMBT_FireProbe( &TBOX_PROBE_Probe ); }

   
/** Ensuring the timestamp of a probe is set to NEVER_FIRED.
 */
#define TBOX_PROBE_CLEAR_PROBE( name ) \
   EMBT_CheckProbe( name, 0 );

/** Computing a delay in milliseconds between two ::tEMBT_Timestamp values.
 *
 * @note Clock overflow is managed by checking whether the result is over a big constant K, in
 * which case we consider the counter looped and the real delay is inverted. We expect probes not
 * to fire too soon compared to the tested delay. By doing this, we arbitrarily limit a valid
 * probe wait to K ticks, and a configuration of a probe wait to FullRange-K ticks in advance.
 *
 */
#define TBOX_PROBE_DELAY( later, sooner ) EMBT_Diff( later, sooner )

/** Checking if a probe fired in a specific interval.
 *
 * @param name is the name of the probe to check.
 * @param max_delay_ms is the maximum delay to wait for starting at from_timestamp.
 * @param from_timestamp is the time to consider as epoch start for this check.
 *
 * @example Use case:
 * @code
 * tEMBT_Timestamp test_start = TBOX_PROBE_NOW();
 *
 * // Run some action which should trigger probes
 * ...
 *
 * // Check each probe against its specified delay from the start of the test
 * TBOX_PROBE_ASSERT_PROBE_OR_FAIL( "Probe 1", 200, test_start );
 * TBOX_PROBE_ASSERT_PROBE_OR_FAIL( "Probe 2", 400, test_start );
 *
 * // All probes on time
 * return TBOX_PROBE_SUCCESS;
 * @endcode
 *
 * @note A probe firing before the value from_timestamp will trigger a failure. Only
 * probes firing after this timestamp will be considered.
 * @example For instance, this is not recommended:
 * @code
 * // Run some action which should trigger a probe
 * ...
 *
 * // Check if the probe fired - this fails if the probe fired before this line is executed
 * TBOX_PROBE_ASSERT_PROBE_OR_FAIL( "Probe 3", 1000, TBOX_PROBE_NOW() );
 * @endcode
 *
 * Use ::TBOX_PROBE_VERIFY_PROBE_OR_FAIL() instead.
 *
 * @note The first [INFO] debug output is here to check your probe coherently is waited for. If
 * you note an unexpectedly high value here, then your delay has already elapsed, and your
 * origin timestamp is misplaced. Either change your origin or use ::TBOX_PROBE_VERIFY_PROBE_OR_FAIL().
 *
 */
#define TBOX_PROBE_ASSERT_PROBE_OR_FAIL( name, max_delay_ms, from_timestamp ) {{ \
   EMBT_Log( "[INFO] Asserting probe \"%s\" under %dms\n", name, EMBT_Diff( from_timestamp + max_delay_ms, EMBT_Clock() ) ); }{ \
   tEMBT_Timestamp const origin   = (from_timestamp); /* Prevent functions from being called after the probe check */ \
   tEMBT_Timestamp const timestamp = EMBT_CheckProbe( (name), (max_delay_ms) ); \
   if( TBOX_PROBE_NEVER_FIRED == timestamp ) { \
      EMBT_Log( "[FAIL] Probe \"%s\" did not fire under specified " #max_delay_ms "ms\n", name ); \
      EMBT_Log( "[WARN] Probe was waited for at " TBOX_PROBE_LOCATION() ")\n" ); \
      return EMBT_FAILURE; }\
   if( ((max_delay_ms) < EMBT_Diff( timestamp, origin )) ) { \
      EMBT_Log( "[FAIL] Probe \"%s\" fired after %dms, over specified " #max_delay_ms "ms\n", name, EMBT_Diff( timestamp, origin ) ); \
      EMBT_Log( "[WARN] Probe was waited for at " TBOX_PROBE_LOCATION() ")\n" ); \
      return EMBT_FAILURE; }}}


/** Checking if a probe fired in the pas.
 *
 * @param name is the name of the probe to check.
 * @param max_delay_ms is the maximum delay to wait for starting right now.
 *
 * @note If you need to verify a timestamp, use ::TBOX_PROBE_ASSERT_PROBE_OR_FAIL().
 *
 * @note This macro succeeds immediately if the probe already fired.
 *
 */
#define TBOX_PROBE_VERIFY_PROBE_OR_FAIL( name, max_delay_ms ) {{ \
   EMBT_Log( "[INFO] Expecting probe \"%s\" under %dms\n", name, max_delay_ms ); }{ \
   if( TBOX_PROBE_NEVER_FIRED == EMBT_CheckProbe( (name), (max_delay_ms) ) ) { \
      EMBT_Log( "[FAIL] Probe \"%s\" did not fire under specified " #max_delay_ms "ms\n", name );\
      EMBT_Log( "[WARN] Probe was waited for at " TBOX_PROBE_LOCATION() ")\n" ); \
      return EMBT_FAILURE; }}}

/** The timestamp representing the current time.
 *
 * Use macro TBOX_PROBE_DELAY() to obtain a delay in milliseconds between two ::tEMBT_Timestamp values.
 *
 * @example Use case:
 * @code
 * tEMBT_Timestamp now = TBOX_PROBE_NOW();
 *
 * // Run a test involving a probe
 *
 * tEMBT_Timestamp fired = TBOX_PROBE_CheckProbe( "A probe" );
 *
 * if( TBOX_PROBE_NEVER_FIRED == fired || 500 < TBOX_PROBE_DELAY( fired, now ) )
 *    return EMBT_FAILURE;
 * 
 * return TBOX_PROBE_SUCCESS;
 * @endcode
 *
 */
#define TBOX_PROBE_NOW() ( (tEMBT_Timestamp) EMBT_Clock() )

/** Logging and returning a failure from a scenario.
 */
#define TBOX_PROBE_FAIL( text ) { \
   EMBT_Log( "[FAIL] %.32s " text "\n", TBOX_PROBE_Module.name ); \
   EMBT_Log( "[WARN] Failure occured at " TBOX_PROBE_LOCATION() "\n" ); \
   return EMBT_FAILURE; }

/** Logging a text.
@@@ PIE elapsed macros DO NOT WORK in C ANSI !
*/
#define TBOX_PROBE_Log( format, value1 ) TBOX_Print( __LINE__, __FILE__, format, value1 )

#else

#define TBOX_PROBE_MODULE( name )
#define TBOX_PROBE_SCENARIO( name, entry )
#define TBOX_PROBE_FIRE_LOCATE_PROBE( name, file, line )
#define TBOX_PROBE_FIRE_PROBE(name)
#define TBOX_PROBE_CLEAR_PROBE( name )
#define TBOX_PROBE_DELAY( later, sooner )
#define TBOX_PROBE_ASSERT_PROBE_OR_FAIL( name, max_delay_ms, from_timestamp )
#define TBOX_PROBE_VERIFY_PROBE_OR_FAIL( name, max_delay_ms )
#define TBOX_PROBE_NOW() (0)
#define TBOX_PROBE_FAIL( text )
#define TBOX_PROBE_Log( format, value1 )

#endif

/*******************************************************/
/*          Typedefs                   */
/*******************************************************/

/*******************************************************/
/*          Variables Declarations (IMPORT)      */
/*******************************************************/

/*******************************************************/
/*          Functions Declarations            */
/*******************************************************/


#ifdef __cplusplus
}/* extern "C" */
#endif

#endif /* _TBOX_PROBES_H */
