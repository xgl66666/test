/******************************************************************************
 *            COPYRIGHT 2008 IWEDIA TECHNOLOGIES            *
 ******************************************************************************
 *
 * MODULE NAME: TDAL_SPEAKERS
 *
 * FILE NAME: $URL: http://10.50.1.219/svncomedia/sources/chal/core/trunk/tdal/inc/tdal_speakers.h $
 *
 *      $Rev: 1654 $
 *      $Date: 2009-10-02 11:33:57 +0200 (ven., 02 oct. 2009) $
 *
 * PUBLIC
 *
 * DESCRIPTION : API Prototype for TDAL_SPEAKERS abstraction layer
 *
 *****************************************************************************/

#ifndef _TDAL_SPEAKERS_H_
#define _TDAL_SPEAKERS_H_


/*******************************************************/
/*       Includes              */
/*******************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************/
/*       Defines              */
/*******************************************************/

#define kTDAL_SPEAKERS_RANGE_VALUE_MIN      0
#define kTDAL_SPEAKERS_RANGE_VALUE_MAX     100


/* DTV 543 used only 2 frequencies */
#define kTDAL_SPEAKERS_MAX_EQUALIZER_FREQUENCY   2  /* Default equalizer frequency : 100 Hz / 1kHz / 10 kHz / 50 kHz */

/*******************************************************/
/*       Macros               */
/*******************************************************/

/*******************************************************/
/*       Typedefs              */
/*******************************************************/


typedef enum
{
   eTDAL_SPEAKERS_NO_ERROR,      /* No Error returned */
   eTDAL_SPEAKERS_NOT_INIT_ERROR,   /* TDAL_SPEAKERS not initialised */
   eTDAL_SPEAKERS_ALREADY_INIT_ERROR,  /* TDAL_SPEAKERS already initialised */
   eTDAL_SPEAKERS_BAD_ARG_ERROR,    /* Bad parameter given to the function */
   eTDAL_SPEAKERS_DRIVER_ERROR,    /* Low level driver returns an error */
   eTDAL_SPEAKERS_NOT_AVAILABLE_ERROR  /* Feature not available*/
}tTDAL_SPEAKERS_Error;

typedef enum
{
  eTDAL_SPEAKERS_FPD_ID_0,  /* speaker flat panel display identifier 0     */
  eTDAL_SPEAKERS_HP_ID_0,   /* speaker headphone identifier 0         */
  eTDAL_SPEAKERS_AUX_ID_0   /* speaker auxiliary identifier 0         */
}tTDAL_SPEAKERS_SpeakerId;

/* specific NXP mode for wow */
typedef enum
{
   eTDAL_SPEAKER_PASS_THRU,
   eTDAL_SPEAKER_DOLBY_PROLOGIC2,
   eTDAL_SPEAKER_VIRTUAL_DOLBY_SURROUND_2,
   eTDAL_SPEAKER_VIRTUAL_DOLBY_DIGITAL_2,
   eTDAL_SPEAKER_VIRTUAL_DOLBY_SPEAKER,
   eTDAL_SPEAKER_TRUSURROUNDXT,
   eTDAL_SPEAKER_TRUSURROUNDHD,
   eTDAL_SPEAKER_SRS3DSOUND,
   eTDAL_SPEAKER_SRSWOW,
   eTDAL_SPEAKER_EXPANDEDSPATIALSTEREO,
   eTDAL_SPEAKER_EXPANDED_PSEUDOSTEREO,
   eTDAL_SPEAKER_TRUSURROUND
}tTDAL_SPEAKERS_WowEffect;


typedef struct TDAL_SPEAKERS_Capability_s
{
   uint8_t nbSpeakers;
} tTDAL_SPEAKERS_Capability;


typedef struct TDAL_SPEAKERS_SpeakerCapability_s
{
   uint8_t         ucEqualizerStep;

   uint8_t         ucTrebleStep;
   uint8_t         ucBassStep;
   uint8_t         ucBalanceStep;

   bool           bControlAutoVolumeSupported;

   bool           bWowSupported;
   uint8_t         ucTruBassStep;

} tTDAL_SPEAKERS_SpeakerCapability;


typedef struct TDAL_SPEAKERS_WOW_s
{
   tTDAL_SPEAKERS_WowEffect   ucMode;
} tTDAL_SPEAKERS_WowStructure;

/*******************************************************/
/*       Variables Declarations (IMPORT)    */
/*******************************************************/

/*******************************************************/
/*       Functions Declarations        */
/*******************************************************/

tTDAL_SPEAKERS_Error    TDAL_SPEAKERS_Init(void);
tTDAL_SPEAKERS_Error    TDAL_SPEAKERS_Term (void);

tTDAL_SPEAKERS_Error    TDAL_SPEAKERS_CapabilityGet(tTDAL_SPEAKERS_Capability *pstCapability);
tTDAL_SPEAKERS_Error    TDAL_SPEAKERS_SpeakerCapabilityGet(tTDAL_SPEAKERS_SpeakerId eId,
                     tTDAL_SPEAKERS_SpeakerCapability *pstSpeakerCapability);

const char *       TDAL_SPEAKERS_APIRevisionGet(void);
const char *       TDAL_SPEAKERS_PlatformRevisionGet(void);

tTDAL_SPEAKERS_Error    TDAL_SPEAKERS_HandleGet(tTDAL_SPEAKERS_SpeakerId eId,
                  void         *pHandle);


tTDAL_SPEAKERS_Error    TDAL_SPEAKERS_GetEqualizer(tTDAL_SPEAKERS_SpeakerId eId,
                  uint8_t uiNbElt,      
                  uint8_t *pucEqualizer);
tTDAL_SPEAKERS_Error    TDAL_SPEAKERS_SetEqualizer(tTDAL_SPEAKERS_SpeakerId eId,
                  uint8_t pucEqualizer[kTDAL_SPEAKERS_MAX_EQUALIZER_FREQUENCY]);


tTDAL_SPEAKERS_Error    TDAL_SPEAKERS_GetTreble(tTDAL_SPEAKERS_SpeakerId eId,
                  uint8_t *pucTreble);
tTDAL_SPEAKERS_Error    TDAL_SPEAKERS_SetTreble(tTDAL_SPEAKERS_SpeakerId eId,
                  uint8_t ucTreble);


tTDAL_SPEAKERS_Error    TDAL_SPEAKERS_GetBass(tTDAL_SPEAKERS_SpeakerId eId,
                 uint8_t *pucBass);
tTDAL_SPEAKERS_Error    TDAL_SPEAKERS_SetBass(tTDAL_SPEAKERS_SpeakerId eId,
                 uint8_t ucBass);


tTDAL_SPEAKERS_Error    TDAL_SPEAKERS_GetBalance(tTDAL_SPEAKERS_SpeakerId eId,
                  uint8_t *pucBalance);
tTDAL_SPEAKERS_Error    TDAL_SPEAKERS_SetBalance(tTDAL_SPEAKERS_SpeakerId eId,
                  uint8_t ucBalance);


tTDAL_SPEAKERS_Error    TDAL_SPEAKERS_GetControlAutoVolume(tTDAL_SPEAKERS_SpeakerId eId,
                     bool *pbControl);
tTDAL_SPEAKERS_Error    TDAL_SPEAKERS_SetControlAutoVolume(tTDAL_SPEAKERS_SpeakerId eId,
                     bool bControl);

tTDAL_SPEAKERS_Error    TDAL_SPEAKERS_GetWOWSettings(tTDAL_SPEAKERS_SpeakerId eId,
                   tTDAL_SPEAKERS_WowStructure *pstWowStructure);
tTDAL_SPEAKERS_Error    TDAL_SPEAKERS_SetWOWSettings(tTDAL_SPEAKERS_SpeakerId eId,
                   tTDAL_SPEAKERS_WowStructure stWowStructure);

#ifdef __cplusplus
}
#endif

#endif  /*_TDAL_SPEAKERS_H_*/

