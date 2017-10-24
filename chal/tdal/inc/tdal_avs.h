/******************************************************************************
 *            COPYRIGHT 2004 IWEDIA TECHNOLOGIES        *
 ******************************************************************************
 *
 * MODULE NAME: TDAL AVS
 *
 * FILE NAME: $URL: http://10.50.1.219/svncomedia/sources/chal/core/trunk/tdal/inc/tdal_avs.h $
 *      $Rev: 66 $
 *      $Date: 2008-10-14 10:29:19 +0200 (mar., 14 oct. 2008) $
 *
 * PUBLIC
 *
 * DESCRIPTION - Public header for AVS driver
 *
 *****************************************************************************/

#ifndef _TDAL_AVS_H_
#define _TDAL_AVS_H_

/*******************************************************/
/*       Includes              */
/*******************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************/
/*       Defines              */
/*******************************************************/
#define kTDAL_AVS_MAX_VALUE_TV_AUDIO_LEVEL  14

/*******************************************************/
/*       Macros               */
/*******************************************************/


/*******************************************************/
/*       Typedefs              */
/*******************************************************/

/* Returned parameters */
typedef enum
{
   eTDAL_AVS_NO_ERROR,
   eTDAL_AVS_NOT_INIT,
   eTDAL_AVS_BAD_PARAMETER,
   eTDAL_AVS_NOT_IMPLEMENTED,
   eTDAL_AVS_NOT_DONE
}tTDAL_AVS_Error;

/* Types */
typedef enum
{
   eTDAL_AVS_AUDIO  = 0,
   eTDAL_AVS_VIDEO
}tTDAL_AVS_Object;

/* Devices */
typedef enum
{
   eTDAL_AVS_OUT_AUX,
   eTDAL_AVS_OUT_VCR,
   eTDAL_AVS_OUT_TV,
   eTDAL_AVS_OUT_RCA,
   eTDAL_AVS_OUT_SVIDEO
}tTDAL_AVS_OutputDevice;
typedef enum
{
   eTDAL_AVS_IN_AUX,
   eTDAL_AVS_IN_VCR,
   eTDAL_AVS_IN_TV,
   eTDAL_AVS_IN_ENCODER
}tTDAL_AVS_InputDevice;

/* Signal types */
typedef enum
{
   eTDAL_AVS_CVBS = 0,
   eTDAL_AVS_CHROMA   ,
   eTDAL_AVS_RGB,
   eTDAL_AVS_MONO,
   eTDAL_AVS_STEREO,
   eTDAL_AVS_RCA_GAIN_AS_TV,
   eTDAL_AVS_RCA_GAIN_CONSTANT
}tTDAL_AVS_SignalType;

/* Values */
typedef uint16_t tTDAL_AVS_Level;

typedef enum
{
   eTDAL_AVS_MUTE_OFF = 0,
   eTDAL_AVS_MUTE_ON
}tTDAL_AVS_MuteState;

typedef enum
{
   eTDAL_AVS_FB_FORCED_LOW_LEVEL = 0,
   eTDAL_AVS_FB_FORCED_HIGH_LEVEL,
   eTDAL_AVS_FB_FROM_ENCODER,
   eTDAL_AVS_FB_FROM_VCR
}tTDAL_AVS_FastBlankingLevel;

typedef enum
{
   eTDAL_AVS_SB_INPUT_MODE = 0,
   eTDAL_AVS_SB_STANDBY_MODE,
   eTDAL_AVS_SB_16_9_FORMAT,
   eTDAL_AVS_SB_4_3_FORMAT,
   eTDAL_AVS_SB_AS_SLB_VCR
}tTDAL_AVS_SlowBlankingLevel;

/*******************************************************/
/*       Variables Declarations (IMPORT)    */
/*******************************************************/


/*******************************************************/
/*       Functions Declarations        */
/*******************************************************/

tTDAL_AVS_Error TDAL_AVS_Init( void );

tTDAL_AVS_Error TDAL_AVS_Term( void );

const char *   TDAL_AVS_APIRevisionGet(void );
const char *   TDAL_AVS_PlatformRevisionGet(void );

tTDAL_AVS_Error TDAL_AVS_SelectInput(
   tTDAL_AVS_Object object,
   tTDAL_AVS_OutputDevice outputDevice,
   tTDAL_AVS_InputDevice inputDevice
);

tTDAL_AVS_Error TDAL_AVS_SelectSignalType(
   tTDAL_AVS_Object object,
   tTDAL_AVS_OutputDevice outputDevice,
   tTDAL_AVS_SignalType signalType
);

tTDAL_AVS_Error TDAL_AVS_SelectLevel(
   tTDAL_AVS_Object object,
   tTDAL_AVS_OutputDevice outputDevice,
   tTDAL_AVS_Level level
);

tTDAL_AVS_Error TDAL_AVS_Mute(
   tTDAL_AVS_Object object,
   tTDAL_AVS_OutputDevice outputDevice,
   tTDAL_AVS_MuteState muteState
);

tTDAL_AVS_Error TDAL_AVS_SelectSlowBlanking(
   tTDAL_AVS_OutputDevice outputDevice,
   tTDAL_AVS_SlowBlankingLevel SBLevel
);

tTDAL_AVS_Error TDAL_AVS_SelectFastBlanking(
   tTDAL_AVS_OutputDevice outputDevice,
   tTDAL_AVS_FastBlankingLevel FBLevel
);

tTDAL_AVS_Error TDAL_AVS_EnableAutomaticLoopthrough(
   bool enable
);

/******************************************************************
 * these functions belong to the old API so we remove them

tTDAL_AVS_Error TDAL_AVS_EnableIT( bool enable );

bool TDAL_AVS_GetITFlag( tTDAL_AVS_SlowBlankingLevel *SBlevel );

tTDAL_AVS_SlowBlankingLevel TDAL_AVS_GetSlowBlankingLevel(void);

tTDAL_AVS_Error TDAL_AVS_CVBS_Loopthrough(void);

tTDAL_AVS_Error TDAL_AVS_SVHS_Loopthrough(void);

tTDAL_AVS_Error TDAL_AVS_SelectEncoderLevel(
   tTDAL_AVS_DiscreteLevel level
);
*******************************************************************/

#ifdef __cplusplus
}
#endif


#endif /*_TDAL_AVS_H_*/
