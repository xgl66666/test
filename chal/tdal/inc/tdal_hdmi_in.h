/******************************************************************************
 *            COPYRIGHT 2008 IWEDIA TECHNOLOGIES            *
 ******************************************************************************
 *
 * MODULE NAME: TDAL_HDMI_IN
 *
 * FILE NAME: $URL: http://10.50.1.219/svncomedia/sources/chal/core/trunk/tdal/inc/tdal_hdmi_in.h $
 *
 *      $Rev: 1620 $
 *      $Date: 2009-09-11 14:29:16 +0200 (ven., 11 sept. 2009) $
 *
 * PUBLIC
 *
 * DESCRIPTION : API Prototype for TDAL_HDMI_IN abstraction layer
 *
 *****************************************************************************/

#ifndef _TDAL_HDMI_IN_H_
#define _TDAL_HDMI_IN_H_


/*******************************************************/
/*       Includes              */
/*******************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#include "tdal_hdmi.h"

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
   eTDAL_HDMI_IN_AUDIO_SPEAKER_PLACEMENT_UNKNOWN = 0x000,
   eTDAL_HDMI_IN_AUDIO_SPEAKER_PLACEMENT_FL   = 0x001,/* Front Left */
   eTDAL_HDMI_IN_AUDIO_SPEAKER_PLACEMENT_FC   = 0x002,/* Front Center */
   eTDAL_HDMI_IN_AUDIO_SPEAKER_PLACEMENT_FR   = 0x004,/* Front Right */
   eTDAL_HDMI_IN_AUDIO_SPEAKER_PLACEMENT_FLC   = 0x008,/* Front Left Center */
   eTDAL_HDMI_IN_AUDIO_SPEAKER_PLACEMENT_FRC   = 0x010,/* Front Right Center */
   eTDAL_HDMI_IN_AUDIO_SPEAKER_PLACEMENT_RL   = 0x020,/* Rear Left */
   eTDAL_HDMI_IN_AUDIO_SPEAKER_PLACEMENT_RC   = 0x040,/* Rear Center */
   eTDAL_HDMI_IN_AUDIO_SPEAKER_PLACEMENT_RR   = 0x080,/* Rear Right */
   eTDAL_HDMI_IN_AUDIO_SPEAKER_PLACEMENT_RLC   = 0x100,/* Rear Left Center */
   eTDAL_HDMI_IN_AUDIO_SPEAKER_PLACEMENT_RRC   = 0x200,/* Rear Right Center */
   eTDAL_HDMI_IN_AUDIO_SPEAKER_PLACEMENT_LFE   = 0x400,/* Low Frequency Effect */
}tTDAL_HDMI_IN_AudioSpeakerPlacement;

typedef enum
{
   eTDAL_HDMI_IN_NO_ERROR,       /* No Error returned */
   eTDAL_HDMI_IN_NOT_INIT_ERROR,    /* TDAL_HDMI has not been initialised */
   eTDAL_HDMI_IN_ALREADY_INIT_ERROR,   /* TDAL_HDMI already initialised */
   eTDAL_HDMI_IN_BAD_PARAMETER_ERROR,   /* Bad parameter given to the function */
   eTDAL_HDMI_IN_DRIVER_ERROR,     /* Low level driver returns an error */
   eTDAL_HDMI_IN_NO_MEMORY_ERROR,    /* Not enough memory to perform action */
   eTDAL_HDMI_IN_NOT_AVAILABLE_ERROR   /* feature not available */
}tTDAL_HDMI_IN_Error;

typedef enum
{
   eTDAL_HDMI_IN_STATE_NO_SOURCE,
   eTDAL_HDMI_IN_STATE_SOURCE_CONNECTED,
   eTDAL_HDMI_IN_STATE_SOURCE_NOT_HDCP,
   eTDAL_HDMI_IN_STATE_AUTHENTICATION_IN_PROGRESS,
   eTDAL_HDMI_IN_STATE_AUTHENTICATION_SUCCEED,
   eTDAL_HDMI_IN_STATE_AUTHENTICATION_FAILED,
   eTDAL_HDMI_IN_STATE_NO_ENCRYPTION
}tTDAL_HDMI_IN_State;

typedef enum
{
   eTDAL_HDMI_IN_EVENT_NEED_DISPLAY,
   eTDAL_HDMI_IN_EVENT_STATE_CHANGE,
   eTDAL_HDMI_IN_EVENT_INFOFRAME_AVI,
   eTDAL_HMDI_IN_EVENT_INFOFRAME_AUDIO,
   eTDAL_HDMI_IN_EVENT_INFOFRAME_SPD,
   eTDAL_HDMI_IN_EVENT_INFOFRAME_VS,
   eTDAL_HDMI_IN_EVENT_GAMUT_METADATA,
   eTDAL_HDMI_IN_EVENT_GENERAL_CONTROL,
   eTDAL_HDMI_IN_NB_EVENT
}tTDAL_HDMI_IN_Event;

typedef enum
{
   eTDAL_HDMI_IN_SINK_ID_0,       /* Sink identifier 0 */
   eTDAL_HDMI_IN_SINK_ID_1,       /* Sink identifier 1 */
   eTDAL_HDMI_IN_SINK_ID_2,       /* Sink identifier 2 */
   eTDAL_HDMI_IN_SINK_ID_3,       /* Sink identifier 3 */
   eTDAL_HDMI_IN_SINK_ID_4       /* Sink identifier 4 */
}tTDAL_HDMI_IN_SinkId;

typedef enum
{
   eTDAL_HDMI_IN_GAMUT_BOUNDARY_DESC_TYPE_VERTICES, /* gamut vertices boundary */
   eTDAL_HDMI_IN_GAMUT_BOUNDARY_DESC_TYPE_RANGES   /* gamut range boundary   */
} tTDAL_HDMI_IN_GamutBoundaryDescType;

typedef enum
{
   eTDAL_HDMI_IN_GAMUT_BOUNDARY_COLOR_PRECISION_8_BIT,
   eTDAL_HDMI_IN_GAMUT_BOUNDARY_COLOR_PRECISION_10_BIT,
   eTDAL_HDMI_IN_GAMUT_BOUNDARY_COLOR_PRECISION_12_BIT
} tTDAL_HDMI_IN_GamutBoundaryColorPrecision;

typedef enum
{
   eTDAL_HDMI_IN_GAMUT_BOUNDARY_COLOR_SPACE_ITU_R_BT601_RGB,
   eTDAL_HDMI_IN_GAMUT_BOUNDARY_COLOR_SPACE_ITU_R_BT709_RGB,
   eTDAL_HDMI_IN_GAMUT_BOUNDARY_COLOR_SPACE_XVYCC_601_RGB,   /* in IEC 61966-2-4 */
   eTDAL_HDMI_IN_GAMUT_BOUNDARY_COLOR_SPACE_XVYCC_709_RGB,   /* in IEC 61966-2-4 */
   eTDAL_HDMI_IN_GAMUT_BOUNDARY_COLOR_SPACE_XVYCC_601_YCBCR, /* in IEC 61966-2-4 */
   eTDAL_HDMI_IN_GAMUT_BOUNDARY_COLOR_SPACE_XVYCC_709_YCBCR  /* in IEC 61966-2-4 */
}tTDAL_HDMI_IN_GamutBoundaryColorSpace;

typedef enum
{
   eTDAL_HDMI_IN_COLOR_DEPTH_UNKNOWN = 0,
   eTDAL_HDMI_IN_COLOR_DEPTH_24BITS_PER_PIXEL,
   eTDAL_HDMI_IN_COLOR_DEPTH_30BITS_PER_PIXEL,
   eTDAL_HDMI_IN_COLOR_DEPTH_36BITS_PER_PIXEL,
   eTDAL_HDMI_IN_COLOR_DEPTH_48BITS_PER_PIXEL
} tTDAL_HDMI_IN_ColorDepthMode;

typedef enum
{
   eTDAL_HDMI_IN_PIXEL_PACKING_PHASE_1, /* Phase 1 (10P1, 12P1, 16P1) */
   eTDAL_HDMI_IN_PIXEL_PACKING_PHASE_2, /* Phase 2 (10P2, 12P2) */
   eTDAL_HDMI_IN_PIXEL_PACKING_PHASE_3, /* Phase 3 (10P3) */
   eTDAL_HDMI_IN_PIXEL_PACKING_PHASE_4  /* Phase 4 (10P4) */
} tTDAL_HDMI_IN_PixelPackingPhase;

typedef enum
{
   eTDAL_HDMI_IN_SOURCE_TYPE_HDMI,   /* sound is found in HDMI */
   eTDAL_HDMI_IN_SOURCE_TYPE_DVI    /* no sound is found in HDMI */   
}tTDAL_HDMI_IN_SourceDeviceType;

typedef void (*tTDAL_HDMI_IN_CallbackFct) (tTDAL_HDMI_IN_SinkId   eId,
                   tTDAL_HDMI_IN_Event   eEvent,
                   const void       *pEventData);

typedef struct
{
   uint8_t ucNbSink;
   bool   bGamutMetadataCapable;
   bool   bGeneralControlPacketCapable;
}tTDAL_HDMI_IN_Capability;

typedef struct
{
   tTDAL_HDMI_IN_State      eState;
   tTDAL_HDMI_IN_SourceDeviceType  eSourceDeviceType;   
}tTDAL_HDMI_IN_SourceStatus;



typedef struct
{
   tTDAL_HDMI_AudioStreamType     eStreamType;
   uint8_t             ucChannelCount;
   tTDAL_HDMI_AudioSampleFreq     eSampleFreq;
   uint8_t             ucSampleSize;
   tTDAL_HDMI_IN_AudioSpeakerPlacement eSpeakerPlacementTab[8];
   uint8_t             ucLevelShiftValue;
   bool              bDownmixProhibited;
}tTDAL_HDMI_IN_InfoFrameAudio;

typedef struct
{
   uint32_t uiY;
   uint32_t uiCb;
   uint32_t uiCr;
}tTDAL_HDMI_IN_GamutBoundaryColorYCbCr;

typedef struct
{
   uint32_t uiR;
   uint32_t uiG;
   uint32_t uiB;
}tTDAL_HDMI_IN_GamutBoundaryColorRGB;

typedef union
{
   tTDAL_HDMI_IN_GamutBoundaryColorRGB   stRGB;
   tTDAL_HDMI_IN_GamutBoundaryColorYCbCr stYCbCr;
}tTDAL_HDMI_IN_GamutBoundaryColor;

typedef struct
{
   tTDAL_HDMI_IN_GamutBoundaryColorPrecision eColorPrecision;
   tTDAL_HDMI_IN_GamutBoundaryColorSpace   eColorSpace;
   tTDAL_HDMI_IN_GamutBoundaryColor     uBlack;
   tTDAL_HDMI_IN_GamutBoundaryColor     uRed;
   tTDAL_HDMI_IN_GamutBoundaryColor     uGreen;
   tTDAL_HDMI_IN_GamutBoundaryColor     uBlue;
}tTDAL_HDMI_IN_GamutBoundaryVertices;

typedef struct
{
   tTDAL_HDMI_IN_GamutBoundaryColorPrecision eColorPrecision;
   tTDAL_HDMI_IN_GamutBoundaryColorSpace   eColorSpace;
   tTDAL_HDMI_IN_GamutBoundaryColor     uRedMin;
   tTDAL_HDMI_IN_GamutBoundaryColor     uRedMax;
   tTDAL_HDMI_IN_GamutBoundaryColor     uGreenMin;
   tTDAL_HDMI_IN_GamutBoundaryColor     uGreenMax;
   tTDAL_HDMI_IN_GamutBoundaryColor     uBlueMin;
   tTDAL_HDMI_IN_GamutBoundaryColor     uBlueMax;
}tTDAL_HDMI_IN_GamutBoundaryRanges;

typedef struct
{
   tTDAL_HDMI_IN_GamutBoundaryDescType   eDescType; /* vertices/facets or ranges */
   union
   {
    tTDAL_HDMI_IN_GamutBoundaryVertices stVertices;
    tTDAL_HDMI_IN_GamutBoundaryRanges   stRanges;
   } uDesc;
}tTDAL_HDMI_IN_GamutBoundaryDesc;

typedef struct
{
   bool            bAudioVideoMute;
   tTDAL_HDMI_IN_ColorDepthMode   eMode;
   tTDAL_HDMI_IN_PixelPackingPhase ePixPacketPhase;
   bool            bDefaultPhase;
}tTDAL_HDMI_IN_GeneralControl;

/*******************************************************/
/*       Variables Declarations (IMPORT)    */
/*******************************************************/

/*******************************************************/
/*       Functions Declarations        */
/*******************************************************/

tTDAL_HDMI_IN_Error TDAL_HDMI_IN_Init (void);
tTDAL_HDMI_IN_Error TDAL_HDMI_IN_Term (void);

tTDAL_HDMI_IN_Error TDAL_HDMI_IN_CapabilityGet(tTDAL_HDMI_IN_Capability      *pstCapability);

const char *    TDAL_HDMI_IN_APIRevisionGet(void);
const char *    TDAL_HDMI_IN_PlatformRevisionGet(void);

tTDAL_HDMI_IN_Error TDAL_HDMI_IN_EventSubscribe(tTDAL_HDMI_IN_SinkId       Id,
                     tTDAL_HDMI_IN_Event        eEvent,
                     tTDAL_HDMI_IN_CallbackFct      notifyFunction);
tTDAL_HDMI_IN_Error TDAL_HDMI_IN_EventUnsubscribe(tTDAL_HDMI_IN_SinkId       Id,
                      tTDAL_HDMI_IN_Event       eEvent);

tTDAL_HDMI_IN_Error TDAL_HDMI_IN_Start(tTDAL_HDMI_IN_SinkId            eId);
tTDAL_HDMI_IN_Error TDAL_HDMI_IN_Stop(tTDAL_HDMI_IN_SinkId            eId);

tTDAL_HDMI_IN_Error TDAL_HDMI_IN_StatusGet(tTDAL_HDMI_IN_SinkId         eId,
                   tTDAL_HDMI_IN_SourceStatus       *pstStatus);
tTDAL_HDMI_IN_Error TDAL_HDMI_IN_VideoDestinationSet(tTDAL_HDMI_IN_SinkId     eId,
                  uint32_t          layerHandle);
tTDAL_HDMI_IN_Error TDAL_HDMI_IN_AudioStreamHandleGet(tTDAL_HDMI_IN_SinkId     eId,
                  uint32_t         *pStreamHandle);

tTDAL_HDMI_IN_Error TDAL_HDMI_IN_EDIDRawBufferSet(tTDAL_HDMI_EDIDRawBuffer     *pstEDIDBuffer);

tTDAL_HDMI_IN_Error TDAL_HDMI_IN_InfoFrameSPDGet(tTDAL_HDMI_IN_SinkId       eId,
                     tTDAL_HDMI_InfoFrameSPD      *pstSPD);
tTDAL_HDMI_IN_Error TDAL_HDMI_IN_InfoFrameVSGet(tTDAL_HDMI_IN_SinkId       eId,
                     tTDAL_HDMI_InfoFrameVS      *pstVS);
tTDAL_HDMI_IN_Error TDAL_HDMI_IN_InfoFrameAVIGet(tTDAL_HDMI_IN_SinkId       eId,
                     tTDAL_HDMI_InfoFrameAVI      *pstAVI);
tTDAL_HDMI_IN_Error TDAL_HDMI_IN_InfoFrameAudioGet(tTDAL_HDMI_IN_SinkId      eId,
                       tTDAL_HDMI_IN_InfoFrameAudio   *pstAudio);

tTDAL_HDMI_IN_Error TDAL_HDMI_IN_GamutMetadataGet(tTDAL_HDMI_IN_SinkId      eId,
                      tTDAL_HDMI_IN_GamutBoundaryDesc  *pstDesc);
tTDAL_HDMI_IN_Error TDAL_HDMI_IN_GeneralControlGet(tTDAL_HDMI_IN_SinkId      eId,
                       tTDAL_HDMI_IN_GeneralControl   *pstControl);

tTDAL_HDMI_IN_Error TDAL_HDMI_IN_HDCPEnable(tTDAL_HDMI_IN_SinkId         eId);
tTDAL_HDMI_IN_Error TDAL_HDMI_IN_HDCPDisable(tTDAL_HDMI_IN_SinkId         eId);
tTDAL_HDMI_IN_Error TDAL_HDMI_IN_HDCPParamsSet(tTDAL_HDMI_IN_SinkId        eId,
                     tTDAL_HDMI_HDCPParams       *pstParams);


#ifdef __cplusplus
}
#endif

#endif  /*_TDAL_HDMI_IN_H_*/



