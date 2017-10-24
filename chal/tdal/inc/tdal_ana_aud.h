/******************************************************************************
 *            COPYRIGHT 2008 IWEDIA TECHNOLOGIES            *
 ******************************************************************************
 *
 * MODULE NAME: TDAL_ANA_AUD
 *
 * FILE NAME: $URL: http://10.50.1.219/svncomedia/sources/chal/core/trunk/tdal/inc/tdal_ana_aud.h $
 *
 *      $Rev: 1522 $
 *      $Date: 2009-08-19 16:28:40 +0200 (mer., 19 août 2009) $
 *
 * PUBLIC
 *
 * DESCRIPTION : API Prototype for TDAL_ANA_AUD abstraction layer
 *
 *****************************************************************************/

#ifndef _TDAL_ANA_AUD_H_
#define _TDAL_ANA_AUD_H_


/*******************************************************/
/*       Includes              */
/*******************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************/
/*       Defines              */
/*******************************************************/

/*******************************************************/
/*       Macros               */
/*******************************************************/


/*******************************************************/
/*       Typedefs              */
/*******************************************************/
typedef enum
{
   eTDAL_ANA_AUD_NO_ERROR,      /* No Error returned */
   eTDAL_ANA_AUD_NOT_INIT_ERROR,   /* TDAL_ANA_AUD has not been initialised */
   eTDAL_ANA_AUD_ALREADY_INIT_ERROR,  /* TDAL_ANA_AUD already initialised */
   eTDAL_ANA_AUD_BAD_ARG_ERROR,    /* Bad parameter given to the function */
   eTDAL_ANA_AUD_DRIVER_ERROR,    /* Low level driver returns an error */
   eTDAL_ANA_AUD_NO_MEMORY_ERROR,   /* Not enough memory to perform action*/
   eTDAL_ANA_AUD_NOT_AVAILABLE_ERROR /* Feature not available*/
}tTDAL_ANA_AUD_Error;

typedef enum
{
   eTDAL_ANA_AUD_DECODER_1 = 0x00010000,
   eTDAL_ANA_AUD_DECODER_2 = 0x00020000,
   eTDAL_ANA_AUD_DECODER_3 = 0x00040000,
   eTDAL_ANA_AUD_DECODER_4 = 0x00080000
}tTDAL_ANA_AUD_Decoder;

typedef enum
{
   eTDAL_ANA_AUD_SOUND_STD_UNKNOWN  = 0xFFFFFFFF,
   eTDAL_ANA_AUD_SOUND_STD_AM_MONO  = 0x00000001,
   eTDAL_ANA_AUD_SOUND_STD_FM_MONO  = 0x00000002,
   eTDAL_ANA_AUD_SOUND_STD_2CS   = 0x00000004,
   eTDAL_ANA_AUD_SOUND_STD_AM_NICAM = 0x00000008,
   eTDAL_ANA_AUD_SOUND_STD_FM_NICAM = 0x00000010,
   eTDAL_ANA_AUD_SOUND_STD_BASEBAND = 0x00000020
} tTDAL_ANA_AUD_SoundStd;

typedef enum
{
   eTDAL_ANA_AUD_SOUND_PROP_UNKNOWN   = 0xFFFFFFFF,/* No additional property, the
                        audio signal is well defined
                        by the sound standard */
   eTDAL_ANA_AUD_SOUND_PROP_MONO   = 0x00000001,/* mono signal */
   eTDAL_ANA_AUD_SOUND_PROP_STEREO   = 0x00000002,/* stereo signal */
   eTDAL_ANA_AUD_SOUND_PROP_DUAL   = 0x00000004,/* 2nd language in the signal */
   eTDAL_ANA_AUD_SOUND_PROP_UNRELATED = 0x00000008, /* audio signal unrelated with
                        video signal */
   eTDAL_ANA_AUD_SOUND_PROP_DATA   = 0x00000010,/* signal contains data
                        information */
} tTDAL_ANA_AUD_SoundProp;

typedef enum
{
   eTDAL_ANA_AUD_TRACK_MONO,
   eTDAL_ANA_AUD_TRACK_STEREO,
   eTDAL_ANA_AUD_TRACK_LANG_A,
   eTDAL_ANA_AUD_TRACK_LANG_B,
   eTDAL_ANA_AUD_TRACK_LANG_A_PLUS_B /* respectively on right and left channel */
} tTDAL_ANA_AUD_Track;

typedef enum
{
   eTDAL_ANA_AUD_EVENT_SIGNAL_CHANGED = 0, /* tTDAL_ANA_AUD_Type* data */
   eTDAL_ANA_AUD_EVENT_SIGNAL_LOST,
   eTDAL_ANA_AUD_EVENT_SIGNAL_SWITCH    /* Bool   bBackupDone data */
} tTDAL_ANA_AUD_Event;

typedef void (*tTDAL_ANA_AUD_CallbackProc)
(
tTDAL_ANA_AUD_Decoder eDecoder,
tTDAL_ANA_AUD_Event   eEvent,
const void      *pEventData
);

typedef struct TDAL_ANA_AUD_Capability_s
{
   uint32_t uiDecoderSupported;
} tTDAL_ANA_AUD_Capability;

typedef struct TDAL_ANA_AUD_DecoderCapability_s
{
   uint32_t uiSupportedStdMask;
} tTDAL_ANA_AUD_DecoderCapability;

typedef struct TDAL_ANA_AUD_Type_s
{
   tTDAL_ANA_AUD_SoundStd eStd;
   uint32_t       uiPropMask;
} tTDAL_ANA_AUD_Type;

typedef struct TDAL_ANA_AUD_StartParam_s
{
   uint32_t uiStdMask;
} tTDAL_ANA_AUD_StartParams;


/*******************************************************/
/*       Variables Declarations (IMPORT)    */
/*******************************************************/

/*******************************************************/
/*       Functions Declarations        */
/*******************************************************/
tTDAL_ANA_AUD_Error TDAL_ANA_AUD_Init (void);
tTDAL_ANA_AUD_Error TDAL_ANA_AUD_Term (void);

tTDAL_ANA_AUD_Error TDAL_ANA_AUD_CapabilityGet(tTDAL_ANA_AUD_Capability       *pstCapability);

const char *    TDAL_ANA_AUD_APIRevisionGet(void);
const char *    TDAL_ANA_AUD_PlatformRevisionGet(void);

tTDAL_ANA_AUD_Error TDAL_ANA_AUD_DecoderCapabilityGet(tTDAL_ANA_AUD_Decoder      eDecoder,
                        tTDAL_ANA_AUD_DecoderCapability  *pstCapability);

tTDAL_ANA_AUD_Error TDAL_ANA_AUD_EventSubscribe(tTDAL_ANA_AUD_Decoder         eDecoder,
                     tTDAL_ANA_AUD_Event         eEvent,
                     tTDAL_ANA_AUD_CallbackProc       notifyFunction);
tTDAL_ANA_AUD_Error TDAL_ANA_AUD_EventUnsubscribe(tTDAL_ANA_AUD_Decoder        eDecoder,
                      tTDAL_ANA_AUD_Event         eEvent);

tTDAL_ANA_AUD_Error TDAL_ANA_AUD_InputStreamSet(tTDAL_ANA_AUD_Decoder         eDecoder,
                     uint32_t              streamHandle);

tTDAL_ANA_AUD_Error TDAL_ANA_AUD_Start(tTDAL_ANA_AUD_Decoder            eDecoder,
                  tTDAL_ANA_AUD_StartParams           *pstStartParams);
tTDAL_ANA_AUD_Error TDAL_ANA_AUD_Stop(tTDAL_ANA_AUD_Decoder             eDecoder);

tTDAL_ANA_AUD_Error TDAL_ANA_AUD_TypeGet(tTDAL_ANA_AUD_Decoder            eDecoder,
                  tTDAL_ANA_AUD_Type            *pstType);

tTDAL_ANA_AUD_Error TDAL_ANA_AUD_TrackSet(tTDAL_ANA_AUD_Decoder            eDecoder,
                  tTDAL_ANA_AUD_Track            eTrack);

tTDAL_ANA_AUD_Error TDAL_ANA_AUD_DestinationSet(tTDAL_ANA_AUD_Decoder         eDecoder,
                     uint32_t              SpeakerHandle);


#ifdef __cplusplus
}
#endif

#endif  /*_TDAL_ANA_AUD_H_*/



