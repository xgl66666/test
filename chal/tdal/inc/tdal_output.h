/******************************************************************************
 *            COPYRIGHT 2006 IWEDIA TECHNOLOGIES       *
 ******************************************************************************
 *
 * MODULE NAME: TDAL_OUTPUT
 *
 * FILE NAME: $URL: http://10.50.1.219/svncomedia/sources/chal/core/trunk/tdal/inc/tdal_output.h $
 *      $Rev: 4432 $
 *      $Date: 2010-12-14 09:55:14 +0100 (mar., 14 déc. 2010) $
 *
 * Description: Thin Driver Adaptation Layer (TDAL) of Output.
 *
 *****************************************************************************/

#ifndef _TDAL_OUTPUT_H_
#define _TDAL_OUTPUT_H_

#ifdef __cplusplus
extern "C" {
#endif

/*--------------------------------------*/
/*     Public types definition   */
/*--------------------------------------*/
typedef enum
{
   eTDAL_OUTPUT_NO_ERROR,      /* No Error returned */
   eTDAL_OUTPUT_NOT_INIT_ERROR,   /* TDAL_OUTPUT has not been initialised */
   eTDAL_OUTPUT_ALREADY_INIT_ERROR,  /* TDAL_OUTPUT already initialised */
   eTDAL_OUTPUT_BAD_PARAMETER_ERROR, /* Bad parameter given to the function */
   eTDAL_OUTPUT_DRIVER_ERROR,    /* Low level driver returns an error */
   eTDAL_OUTPUT_NO_MEMORY_ERROR,   /* Not enough memory to perform action */
   eTDAL_OUTPUT_NOT_AVAILABLE_ERROR  /* feature not available */
} tTDAL_OUTPUT_Error;

typedef enum
{
   eTDAL_OUTPUT_VIDEO_DENC_ID_0,
   eTDAL_OUTPUT_VIDEO_DENC_ID_1,
   eTDAL_OUTPUT_VIDEO_DENC_ID_2,
   eTDAL_OUTPUT_VIDEO_HD_DAC_ID_0,
   eTDAL_OUTPUT_VIDEO_HD_DAC_ID_1,
   eTDAL_OUTPUT_VIDEO_HD_DAC_ID_2,
   eTDAL_OUTPUT_HDMI_ID_0,
   eTDAL_OUTPUT_HDMI_ID_1,
   eTDAL_OUTPUT_HDMI_ID_2,
   eTDAL_OUTPUT_AUDIO_ANA_ID_0,
   eTDAL_OUTPUT_AUDIO_ANA_ID_1,
   eTDAL_OUTPUT_AUDIO_ANA_ID_2,
   eTDAL_OUTPUT_AUDIO_SPDIF_ID_0,
   eTDAL_OUTPUT_AUDIO_SPDIF_ID_1,
   eTDAL_OUTPUT_AUDIO_SPDIF_ID_2,
   eTDAL_OUTPUT_AUDIO_HEADPHONE,
   eTDAL_OUTPUT_RF_MOD_ID_0,
   eTDAL_OUTPUT_RF_MOD_ID_1,
   eTDAL_OUTPUT_RF_MOD_ID_2,
   eTDAL_OUTPUT_VIDEO_DIGITAL_ID_0,
   eTDAL_OUTPUT_VIDEO_DIGITAL_ID_1,
   eTDAL_OUTPUT_VIDEO_DIGITAL_ID_2
} tTDAL_OUTPUT_OutputId;

typedef enum
{
   eTDAL_OUTPUT_VIDEO_DENC_FORMAT_YC   = 0x01, /* video output YC/S-Video  */
   eTDAL_OUTPUT_VIDEO_DENC_FORMAT_CVBS = 0x02, /* video output CVBS - Composite*/
   eTDAL_OUTPUT_VIDEO_DENC_FORMAT_RGB  = 0x04, /* video output RGB */
   eTDAL_OUTPUT_VIDEO_DENC_FORMAT_YUV  = 0x08 /* video output YUV = YcrCb */
} tTDAL_OUTPUT_VideoDencFormat;


typedef enum
{
   eTDAL_OUTPUT_VIDEO_SD_STD_PAL_BG    = 0x01, /* PAL B, G*/
   eTDAL_OUTPUT_VIDEO_SD_STD_PAL_DK    = 0x02, /* PAL D, K */
   eTDAL_OUTPUT_VIDEO_SD_STD_PAL_I     = 0x04, /* PAL I */
   eTDAL_OUTPUT_VIDEO_SD_STD_PAL_M     = 0x08, /* PAL M (Brazil) */
   eTDAL_OUTPUT_VIDEO_SD_STD_PAL_N     = 0x10, /* PAL N (Argentina, Uruguay) */
   eTDAL_OUTPUT_VIDEO_SD_STD_SECAM     = 0x20, /* SECAM      */
   eTDAL_OUTPUT_VIDEO_SD_STD_NTSC_M    = 0x40  /* NTSC M      */
}tTDAL_OUTPUT_VideoSDStandard;

typedef enum
{
    eTDAL_OUTPUT_VIDEO_DENC_CGMS_A_COPY_FREELY,
    eTDAL_OUTPUT_VIDEO_DENC_CGMS_A_COPY_NO_MORE,
    eTDAL_OUTPUT_VIDEO_DENC_CGMS_A_COPY_ONCE,
    eTDAL_OUTPUT_VIDEO_DENC_GMS_A_COPY_NEVER,
} tTDAL_OUTPUT_VideoDencCGMSACopyProtection;

typedef enum
{
   eTDAL_OUTPUT_RF_MOD_BAND_UHF    = 0x01, /* UHF */
   eTDAL_OUTPUT_RF_MOD_BAND_VHF    = 0x02  /* VHF */
}tTDAL_OUTPUT_RFModBand;

typedef enum
{
   eTDAL_OUTPUT_VIDEO_HD_DAC_RGB   = 0x01, /* video output RGB   */
   eTDAL_OUTPUT_VIDEO_HD_DAC_YCBCR = 0x02 /* video output YCbCr  */
}tTDAL_OUTPUT_VideoHDDACFormat;

typedef enum
{
   eTDAL_OUTPUT_VIDEO_HDMI_RGB888   = 0x01, /* video output RGB888   */
   eTDAL_OUTPUT_VIDEO_HDHMI_YCBCR422   = 0x02, /* video output YCbCr422  */
   eTDAL_OUTPUT_VIDEO_HDHMI_YCBCR444   = 0x02 /* video output YCbCr444  */
}tTDAL_OUTPUT_VideoHDMIFormat;

typedef enum
{
  eTDAL_OUTPUT_VIDEO_DIGITAL_RGB888   = 0x01, /* video output RGB888   */
  eTDAL_OUTPUT_VIDEO_DIGITAL_YCBCR444 = 0x02 /* video output YCbCr444  */
}tTDAL_OUTPUT_VideoDigitalFormat;


typedef enum
{
   eTDAL_OUTPUT_VIDEO_HD_DAC_ITU601   = 0x01,
   eTDAL_OUTPUT_VIDEO_HD_DAC_ITU709   = 0x02,
   eTDAL_OUTPUT_VIDEO_HD_DAC_SMPTE240M = 0x04
}tTDAL_OUTPUT_VideoHDDACColorSpace;

typedef enum
{
   eTDAL_OUTPUT_VIDEO_HD_STD_NONE       = 0x00000000,
   eTDAL_OUTPUT_VIDEO_HD_STD_640_480_P_60   = 0x00000001,
   eTDAL_OUTPUT_VIDEO_HD_STD_720_480_P_5994   = 0x00000002,
   eTDAL_OUTPUT_VIDEO_HD_STD_720_480_P_60   = 0x00000004,
   eTDAL_OUTPUT_VIDEO_HD_STD_720_480_I_5994   = 0x00000008,
   eTDAL_OUTPUT_VIDEO_HD_STD_720_480_I_60   = 0x00000010,
   eTDAL_OUTPUT_VIDEO_HD_STD_720_483_P_5994   = 0x00000020,
   eTDAL_OUTPUT_VIDEO_HD_STD_720_576_P_50   = 0x00000040,
   eTDAL_OUTPUT_VIDEO_HD_STD_720_576_I_50   = 0x00000080,
   eTDAL_OUTPUT_VIDEO_HD_STD_1280_720_P_2398   = 0x00000100,
   eTDAL_OUTPUT_VIDEO_HD_STD_1280_720_P_24   = 0x00000200,
   eTDAL_OUTPUT_VIDEO_HD_STD_1280_720_P_25   = 0x00000400,
   eTDAL_OUTPUT_VIDEO_HD_STD_1280_720_P_2997   = 0x00000800,
   eTDAL_OUTPUT_VIDEO_HD_STD_1280_720_P_30   = 0x00001000,
   eTDAL_OUTPUT_VIDEO_HD_STD_1280_720_P_50   = 0x00002000,
   eTDAL_OUTPUT_VIDEO_HD_STD_1280_720_P_5994   = 0x00004000,
   eTDAL_OUTPUT_VIDEO_HD_STD_1280_720_P_60   = 0x00008000,
   eTDAL_OUTPUT_VIDEO_HD_STD_1920_1080_P_2398  = 0x00010000,
   eTDAL_OUTPUT_VIDEO_HD_STD_1920_1080_P_24   = 0x00020000,
   eTDAL_OUTPUT_VIDEO_HD_STD_1920_1080_P_25   = 0x00040000,
   eTDAL_OUTPUT_VIDEO_HD_STD_1920_1080_P_2997  = 0x00080000,
   eTDAL_OUTPUT_VIDEO_HD_STD_1920_1080_P_30   = 0x00100000,
   eTDAL_OUTPUT_VIDEO_HD_STD_1920_1080_P_50   = 0x00200000,
   eTDAL_OUTPUT_VIDEO_HD_STD_1920_1080_P_5994  = 0x00400000,
   eTDAL_OUTPUT_VIDEO_HD_STD_1920_1080_P_60   = 0x00800000,
   eTDAL_OUTPUT_VIDEO_HD_STD_1920_1080_I_50   = 0x01000000,
   eTDAL_OUTPUT_VIDEO_HD_STD_1920_1080_I_5994  = 0x02000000,
   eTDAL_OUTPUT_VIDEO_HD_STD_1920_1080_I_60   = 0x04000000
}tTDAL_OUTPUT_VideoHDStandard;

typedef enum
{
   eTDAL_OUTPUT_AUDIO_SPEAKER_TYPE_NONE  = 0x00000001, /* not present */
   eTDAL_OUTPUT_AUDIO_SPEAKER_TYPE_SMALL = 0x00000002, /* not capable of outputting low frequency */
   eTDAL_OUTPUT_AUDIO_SPEAKER_TYPE_LARGE = 0x00000004 /* capable of outputting low frequency */
} tTDAL_OUTPUT_AudioSpeakerType;

typedef enum
{
   eTDAL_OUTPUT_AUDIO_DIGITAL_MODE_UNCOMPRESSED      = 0x00000001, /* IEC60958: The output are PCM data.
                           Only the left and right channels are transmitted. */

/* The compressed bitstream is transmitted on
   the digital output and can be decoded
   by an external decoder */
   eTDAL_OUTPUT_AUDIO_DIGITAL_MODE_COMPRESSED_HE_AAC_MP2    = 0x00000002,
   eTDAL_OUTPUT_AUDIO_DIGITAL_MODE_COMPRESSED_HE_AAC_MP4_1   = 0x00000004,
   eTDAL_OUTPUT_AUDIO_DIGITAL_MODE_COMPRESSED_HE_AAC_MP4_2   = 0x00000008,
   eTDAL_OUTPUT_AUDIO_DIGITAL_MODE_COMPRESSED_AC3      = 0x00000010,
   eTDAL_OUTPUT_AUDIO_DIGITAL_MODE_COMPRESSED_EAC3       = 0x00000020,
   eTDAL_OUTPUT_AUDIO_DIGITAL_MODE_COMPRESSED_DTS       = 0x00000040
} tTDAL_OUTPUT_AudioDigitalMode;

typedef struct
{
   uint8_t nbVideoDenc;  /* Number of different video denc that can be controlled */
   uint8_t nbVideoHDDAC; /* Number of different video HD DACs that can be controlled */
   uint8_t nbHDMI;    /* Number of different HDMI that can be controlled */
   uint8_t nbAudioAna;   /* Number of different audio analogue that can be controlled */
   uint8_t nbAudioSPDIF; /* Number of different audio SPDIF that can be controlled */
   uint8_t nbRFMod;   /* Number of different RF modulator that can be controlled */
   uint8_t nbVideoDigital; /* Number of different video digital output that can be controlled */
}tTDAL_OUTPUT_Capability;

typedef struct
{
   uint32_t FormatSupportedMask;   /* bitfield of tTDAL_OUTPUT_VideoDencFormat supported   */
   uint32_t StandardSupportedMask; /* bitfield of tTDAL_OUTPUT_VideoSDStandard supported */
   bool   IsColorBarSupported;   /* TRUE if color bar test pattern supported */
   bool   IsBrigthnessSupported; /* TRUE if control of the brigthness supported */
   bool   IsContrastSupported;   /* TRUE if control of the contrast supported */
   bool   IsSaturationSupported; /* TRUE if control of the color saturation supported */
}tTDAL_OUTPUT_VideoDencCapability;

typedef struct
{
   uint8_t Brigthness; /* Brightness level 0:least brigthness - 255:most brightness */
   uint8_t Contrast;   /* Contrast level 0:least contrasting- 255:most contrasting */
   uint8_t Saturation; /* Saturation level 0:min amount of color saturation-255:max amount of color saturation */
}tTDAL_OUTPUT_VideoDencBCSParams;

typedef struct
{
   uint32_t FormatSupportedMask;   /* bitfield of tTDAL_OUTPUT_VideoHDDACFormat supported   */
   uint32_t ColorSpaceSupportedMask; /* bitfield of tTDAL_OUTPUT_VideoHDDACColorSpace supported   */
   uint32_t StandardSupportedMask;   /* bitfield of tTDAL_OUTPUT_VideoHDStandard supported */
}tTDAL_OUTPUT_VideoHDDACCapability;

typedef struct
{
   tTDAL_OUTPUT_VideoHDDACFormat   Format;
   tTDAL_OUTPUT_VideoHDDACColorSpace ColorSpace;
   tTDAL_OUTPUT_VideoHDStandard   Standard;
   bool              EmbeddedSyncOn; /* TRUE to insert embedded sync timings as in EIA770.3 */
}tTDAL_OUTPUT_VideoHDDACParams;

typedef struct
{
   uint32_t FormatSupportedMask;   /* bitfield of tTDAL_OUTPUT_VideoHDMIFormat supported   */
   uint32_t StandardSupportedMask;   /* bitfield of tTDAL_OUTPUT_VideoHDStandard supported */
}tTDAL_OUTPUT_HDMICapability;

typedef struct
{
   tTDAL_OUTPUT_VideoHDMIFormat  VideoFormat;
   tTDAL_OUTPUT_VideoHDStandard  VideoStandard;
   tTDAL_OUTPUT_AudioDigitalMode AudioMode;
   uint32_t         AudioDelay;
   uint8_t         AudioAdjust;
}tTDAL_OUTPUT_HDMIParams;

typedef struct
{
   uint32_t FormatSupportedMask;
   uint32_t StandardSupportedMask;
}tTDAL_OUTPUT_VideoDigitalCapability;

typedef struct
{
  tTDAL_OUTPUT_VideoDigitalFormat  VideoFormat;
  tTDAL_OUTPUT_VideoHDStandard   VideoStandard;
}tTDAL_OUTPUT_VideoDigitalParams;


typedef struct
{
   bool   IsFrontRLPresenceSupported;     /* TRUE if front right/left speakers presence is supported */
   bool   IsFrontCenterPresenceSupported;   /* TRUE if front center speakers presence is supported */
   bool   IsSurroundRLPresenceSupported;    /* TRUE if surround right/left speakers presence is supported */
   bool   IsLFEPresenceSupported;       /* TRUE if LFE (Low Frequency effect) speaker presence is supported  */
   bool   IsRearRLPresenceSupported;      /* TRUE if rear right/left speakers presence is supported  */
   bool   IsRearCenterPresenceSupported;    /* TRUE if rear center speakers presence is supported  */
   bool   IsFrontRLAttenuationSupported;    /* TRUE if front right/left speakers attenuation is supported */
   bool   IsFrontCenterAttenuationSupported;   /* TRUE if front center speakers attenuation is supported */
   bool   IsSourroundRLAttenuationSupported;   /* TRUE if surround right/left speakers attenuationis supported */
   bool   IsLFEAttenuationSupported;      /* TRUE if LFE (Low Frequency effect) speaker attenuationis supported  */
   bool   IsRearRLAttenuationSupported;    /* TRUE if rear right/left speakers attenuation is supported  */
   bool   IsRearCenterAttenuationSupported;   /* TRUE if rear center speakers attenuation is supported  */
   bool   IsFrontRLDelaySupported;      /* TRUE if front right/left speakers delay is supported */
   bool   IsFrontCenterDelaySupported;     /* TRUE if front center speakers is delay supported */
   bool   IsSurroundRLCenterDelaySupported;   /* TRUE if surround right/left speakers delay is supported */
   bool   IsLFEDelaySupported;        /* TRUE if LFE (Low Frequency effect) speaker delay is supported  */
   bool   IsRearRLDelaySupported;       /* TRUE if rear right/left speakers delay is supported  */
   bool   IsRearCenterDelaySupported;     /* TRUE if rear center speakers delay is supported  */
   uint32_t AttenuationMax;          /* Maximum steps of attenuation control (valid if attenuation is supported)*/
   uint32_t DelayMax;            /* Maximum speaker delay in ms (valid if daly control is supported) */
   bool   IsMixingSupported;        /* TRUE if two audio tracks mixing supported */
}tTDAL_OUTPUT_AudioAnaCapability;

typedef struct
{
   tTDAL_OUTPUT_AudioSpeakerType   FrontRL;
   tTDAL_OUTPUT_AudioSpeakerType   FrontCenter;
   tTDAL_OUTPUT_AudioSpeakerType   SurroundRL;
   tTDAL_OUTPUT_AudioSpeakerType   RearRL;
   tTDAL_OUTPUT_AudioSpeakerType   RearCenter;
   bool             IsLFEPresent;
   bool         bUseMixedAudio;   /* Request to output two audio tracks mixed */
   uint8_t         AudioAdjust;
} tTDAL_OUTPUT_AudioAnaSpeakerParams;

typedef struct
{
   uint32_t FrontRLAttenuation;     /* front right/left speakers attenuation */
   uint32_t FrontCenterAttenuation;   /* front center speakers attenuation */
   uint32_t SurroundRLAttenuation;    /* surround right/left speakers attenuation */
   uint32_t LFEAttenuation;       /* LFE (Low Frequency effect) speaker attenuation */
   uint32_t RearRLAttenuation;      /* rear right/left speakers attenuation  */
   uint32_t RearCenterAttenuation;    /* rear center speakers attenuation */
   uint32_t MixLevel;       /* Mix level between two mixed track if supported */
}tTDAL_OUTPUT_AudioAnaSpeakerAttenuation;

typedef struct
{
   uint32_t FrontRLDelay;    /* front right/left speakers delay */
   uint32_t FrontCenterDelay;   /* front center speakers delay */
   uint32_t SurroundRLDelay;   /* surround right/left speakers delay */
   uint32_t LFEDelay;      /* LFE (Low Frequency effect) speaker delay */
   uint32_t RearRLDelay;    /* rear right/left speakers delay  */
   uint32_t RearCenterDelay;   /* rear center speakers delay */
}tTDAL_OUTPUT_AudioAnaSpeakerDelay;

typedef struct
{
   uint32_t BandSupportedMask;     /* bitfield of tTDAL_OUTPUT_RFModBand supported   */
   uint32_t StandardSupportedMask;   /* bitfield of tTDAL_OUTPUT_VideoSDStandard supported */
   bool   IsTestGenerationSupported; /* TRUE if color bar test pattern supported */
   uint8_t  ChannelMin;       /* Minimum television channel that the RF modulator can be tuned on */
   uint8_t  ChannelMax;       /* Maximum television channel that the RF modulator can be tuned on */
}tTDAL_OUTPUT_RFModCapability;

typedef void (*tTDAL_OUTPUT_HeadphoneHotplugNotifyCbk)(tTDAL_OUTPUT_OutputId Id,
                  bool         bHotplug);

/*------------------------------------------*/
/*    Public Functions Declarations   */
/*------------------------------------------*/

/* Generic */
IMPORT tTDAL_OUTPUT_Error TDAL_OUTPUT_Init(void);
IMPORT tTDAL_OUTPUT_Error TDAL_OUTPUT_Term(void);
IMPORT tTDAL_OUTPUT_Error TDAL_OUTPUT_CapabilityGet(tTDAL_OUTPUT_Capability *pstCapability);
IMPORT tTDAL_OUTPUT_Error TDAL_OUTPUT_Enable(tTDAL_OUTPUT_OutputId Id);
IMPORT tTDAL_OUTPUT_Error TDAL_OUTPUT_Disable(tTDAL_OUTPUT_OutputId Id);
IMPORT const char    *TDAL_OUTPUT_APIRevisionGet(void);
IMPORT const char    *TDAL_OUTPUT_PlatformRevisionGet(void);

/* Video Denc */
IMPORT tTDAL_OUTPUT_Error TDAL_OUTPUT_VideoDencCapabilityGet(tTDAL_OUTPUT_OutputId Id, tTDAL_OUTPUT_VideoDencCapability *pstCapability);
IMPORT tTDAL_OUTPUT_Error TDAL_OUTPUT_VideoDencStandardSet(tTDAL_OUTPUT_OutputId Id, tTDAL_OUTPUT_VideoSDStandard Std);
IMPORT tTDAL_OUTPUT_Error TDAL_OUTPUT_VideoDencFormatSet(tTDAL_OUTPUT_OutputId Id, uint32_t FormatMask);
IMPORT tTDAL_OUTPUT_Error TDAL_OUTPUT_VideoDencColorBarControl(tTDAL_OUTPUT_OutputId Id, bool On);
IMPORT tTDAL_OUTPUT_Error TDAL_OUTPUT_VideoDencBCSParamsSet(tTDAL_OUTPUT_OutputId Id, tTDAL_OUTPUT_VideoDencBCSParams *pstBcs);
IMPORT tTDAL_OUTPUT_Error TDAL_OUTPUT_VideoDencGSMCACopyProtectionSet(tTDAL_OUTPUT_OutputId Id, tTDAL_OUTPUT_VideoDencCGMSACopyProtection cgmsaCopyProtection);
IMPORT tTDAL_OUTPUT_Error TDAL_OUTPUT_VideoDencMacrovisionCopyProtectionSet(tTDAL_OUTPUT_OutputId Id, bool On);

/* Video HD Dacs */
IMPORT tTDAL_OUTPUT_Error TDAL_OUTPUT_VideoHDDACCapabilityGet(tTDAL_OUTPUT_OutputId Id, tTDAL_OUTPUT_VideoHDDACCapability *pstCapability);
IMPORT tTDAL_OUTPUT_Error TDAL_OUTPUT_VideoHDDACParamsSet(tTDAL_OUTPUT_OutputId Id, tTDAL_OUTPUT_VideoHDDACParams *pstParams);

/* HDMI */
IMPORT tTDAL_OUTPUT_Error TDAL_OUTPUT_HDMICapabilityGet(tTDAL_OUTPUT_OutputId Id, tTDAL_OUTPUT_HDMICapability *pstCapability);
IMPORT tTDAL_OUTPUT_Error TDAL_OUTPUT_HDMIParamsSet(tTDAL_OUTPUT_OutputId Id, tTDAL_OUTPUT_HDMIParams *pstParams);

/* Audio analogue */
IMPORT tTDAL_OUTPUT_Error TDAL_OUTPUT_AudioAnaCapabilityGet(tTDAL_OUTPUT_OutputId Id, tTDAL_OUTPUT_AudioAnaCapability *pstCapability);
IMPORT tTDAL_OUTPUT_Error TDAL_OUTPUT_AudioAnaSpeakerParamsSet(tTDAL_OUTPUT_OutputId Id, tTDAL_OUTPUT_AudioAnaSpeakerParams *pstParams);
IMPORT tTDAL_OUTPUT_Error TDAL_OUTPUT_AudioAnaSpeakerAttenuationSet(tTDAL_OUTPUT_OutputId Id, tTDAL_OUTPUT_AudioAnaSpeakerAttenuation *pstAttenuation);
IMPORT tTDAL_OUTPUT_Error TDAL_OUTPUT_AudioAnaSpeakerAttenuationGet(tTDAL_OUTPUT_OutputId Id, tTDAL_OUTPUT_AudioAnaSpeakerAttenuation *pstAttenuation);
IMPORT tTDAL_OUTPUT_Error TDAL_OUTPUT_AudioAnaSpeakerDelaySet(tTDAL_OUTPUT_OutputId Id, tTDAL_OUTPUT_AudioAnaSpeakerDelay *pstDelay);
IMPORT tTDAL_OUTPUT_Error TDAL_OUTPUT_AudioAnaSpeakerAdjustSet(tTDAL_OUTPUT_OutputId Id, uint8_t AudioAdjust);

/* Audio SPDIF */
IMPORT tTDAL_OUTPUT_Error TDAL_OUTPUT_AudioSPDIFModeSet(tTDAL_OUTPUT_OutputId Id, tTDAL_OUTPUT_AudioDigitalMode Mode);
IMPORT tTDAL_OUTPUT_Error TDAL_OUTPUT_AudioSPDIFDelaySet(tTDAL_OUTPUT_OutputId Id, uint32_t AudioDelay);
IMPORT tTDAL_OUTPUT_Error TDAL_OUTPUT_AudioSPDIFAdjustSet(tTDAL_OUTPUT_OutputId Id, uint8_t AudioAdjust);


/* RF modulator */
IMPORT tTDAL_OUTPUT_Error TDAL_OUTPUT_RFModCapabilityGet(tTDAL_OUTPUT_OutputId Id, tTDAL_OUTPUT_RFModCapability *pstCapability);
IMPORT tTDAL_OUTPUT_Error TDAL_OUTPUT_RFModBandSet(tTDAL_OUTPUT_OutputId Id, tTDAL_OUTPUT_RFModBand Band);
IMPORT tTDAL_OUTPUT_Error TDAL_OUTPUT_RFModChannelSet(tTDAL_OUTPUT_OutputId Id, uint8_t Channel);
IMPORT tTDAL_OUTPUT_Error TDAL_OUTPUT_RFModStandardSet(tTDAL_OUTPUT_OutputId Id, tTDAL_OUTPUT_VideoSDStandard Std);
IMPORT tTDAL_OUTPUT_Error TDAL_OUTPUT_RFModTestGenControl(tTDAL_OUTPUT_OutputId Id, bool On);

/* Video digital output */
tTDAL_OUTPUT_Error    TDAL_OUTPUT_VideoDigitalCapabilityGet(tTDAL_OUTPUT_OutputId Id, tTDAL_OUTPUT_VideoDigitalCapability *pstCapability);
tTDAL_OUTPUT_Error    TDAL_OUTPUT_VideoDigitalParamsSet(tTDAL_OUTPUT_OutputId Id,tTDAL_OUTPUT_VideoDigitalParams *pstParams);


/* Headphone */
tTDAL_OUTPUT_Error TDAL_OUTPUT_HeadphoneHotplugDetectionSupported(tTDAL_OUTPUT_OutputId Id, bool *pbSupported);
tTDAL_OUTPUT_Error TDAL_OUTPUT_HeadphoneHotplugDetectionRegister(tTDAL_OUTPUT_OutputId Id, tTDAL_OUTPUT_HeadphoneHotplugNotifyCbk pCbk);
tTDAL_OUTPUT_Error TDAL_OUTPUT_HeadphoneHotplugDetectionStateGet(tTDAL_OUTPUT_OutputId Id, bool *pbState);

#ifdef __cplusplus
}
#endif

#endif /*_TDAL_HDMI_H_*/

