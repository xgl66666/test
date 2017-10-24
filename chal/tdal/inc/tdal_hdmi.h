/******************************************************************************
 *            COPYRIGHT 2006 IWEDIA TECHNOLOGIES       *
 ******************************************************************************
 *
 * MODULE NAME: TDAL_HDMI
 *
 * FILE NAME: $URL: http://10.50.1.219/svncomedia/sources/chal/core/trunk/tdal/inc/tdal_hdmi.h $
 *      $Rev: 894 $
 *      $Date: 2009-02-20 10:06:02 +0100 (ven., 20 févr. 2009) $
 *
 * Description: Thin Driver Adaptation Layer (TDAL) of
 *       High Definition Multimedia Interface (HDMI).
 *
 *****************************************************************************/

#ifndef _TDAL_HDMI_H_
#define _TDAL_HDMI_H_

#ifdef __cplusplus
extern "C" {
#endif

/*--------------------------------------*/
/*     Public types definition   */
/*--------------------------------------*/
#define kTDAL_HDMI_NB_VIDEO_FORMAT_MAX  (10)
#define kTDAL_HDMI_NB_AUDIO_FORMAT_MAX  (10)
#ifdef USE_HDCP
#define kOFFSET_FOR_KSV 				(0)
#define kOFFSET_FOR_DEVICE_KEYS 		(8)
#endif

typedef enum
{
   eTDAL_HDMI_NO_ERROR,      /* No Error returned */
   eTDAL_HDMI_NOT_INIT_ERROR,   /* TDAL_HDMI has not been initialised */
   eTDAL_HDMI_ALREADY_INIT_ERROR,  /* TDAL_HDMI already initialised */
   eTDAL_HDMI_BAD_PARAMETER_ERROR, /* Bad parameter given to the function */
   eTDAL_HDMI_DRIVER_ERROR,    /* Low level driver returns an error */
   eTDAL_HDMI_NO_MEMORY_ERROR,   /* Not enough memory to perform action */
   eTDAL_HDMI_NOT_AVAILABLE_ERROR  /* feature not available */
} tTDAL_HDMI_Error;

typedef enum
{
   eTDAL_HDMI_EVENT_NO_RECEIVER,
   eTDAL_HDMI_EVENT_RECEIVER_CONNECTED,
   eTDAL_HDMI_EVENT_RECEIVER_NOT_HDCP,
   eTDAL_HDMI_EVENT_AUTHENTICATION_IN_PROGRESS,
   eTDAL_HDMI_EVENT_AUTHENTICATION_SUCCEED,
   eTDAL_HDMI_EVENT_AUTHENTICATION_FAILED,
   eTDAL_HDMI_EVENT_NO_ENCRYPTION
} tTDAL_HDMI_Event;

typedef enum
{
   eTDAL_HDMI_AUDIO_SAMPLE_FREQ_UNKNOWN   = 0x00, /* Unknown   */
   eTDAL_HDMI_AUDIO_SAMPLE_FREQ_32_KHZ   = 0x01, /* 32 kHz   */
   eTDAL_HDMI_AUDIO_SAMPLE_FREQ_44_1_KHZ  = 0x02, /* 44.1 kHz  */
   eTDAL_HDMI_AUDIO_SAMPLE_FREQ_48_KHZ   = 0x04, /* 48 kHz   */
   eTDAL_HDMI_AUDIO_SAMPLE_FREQ_88_2_KHZ  = 0x08, /* 88.2 kHz  */
   eTDAL_HDMI_AUDIO_SAMPLE_FREQ_96_KHZ   = 0x10, /* 96 kHz   */
   eTDAL_HDMI_AUDIO_SAMPLE_FREQ_176_4_KHZ = 0x20, /* 176.4 kHz */
   eTDAL_HDMI_AUDIO_SAMPLE_FREQ_192_KHZ   = 0x40 /* 192 kHz   */
} tTDAL_HDMI_AudioSampleFreq;

typedef enum
{
   eTDAL_HDMI_AUDIO_STREAM_TYPE_UNKNOWN = 0x000, /* Unknown */
   eTDAL_HDMI_AUDIO_STREAM_TYPE_LPCM   = 0x001, /* Linear PCM */
   eTDAL_HDMI_AUDIO_STREAM_TYPE_AC3   = 0x002, /* AC-3 */
   eTDAL_HDMI_AUDIO_STREAM_TYPE_MPEG1   = 0x004, /* MPEG1 (layer 1 and 2) */
   eTDAL_HDMI_AUDIO_STREAM_TYPE_MP3   = 0x008, /* MPEG1 (layer 3) */
   eTDAL_HDMI_AUDIO_STREAM_TYPE_MPEG2   = 0x010, /* MPEG2 (multichannel) */
   eTDAL_HDMI_AUDIO_STREAM_TYPE_AAC   = 0x020, /* Advanced Audio Coding */
   eTDAL_HDMI_AUDIO_STREAM_TYPE_DTS   = 0x040, /* Digital Theater System */
   eTDAL_HDMI_AUDIO_STREAM_TYPE_ATRAC   = 0x080, /* Adaptive TRansform Acoustic Coding */
   eTDAL_HDMI_AUDIO_STREAM_TYPE_OBA   = 0x100, /* One Bit Audio */
   eTDAL_HDMI_AUDIO_STREAM_TYPE_DBPLUS  = 0x200, /* Dolby Digital Plus */
   eTDAL_HDMI_AUDIO_STREAM_TYPE_DTS_HD  = 0x400, /* Digital Theater System HD */
   eTDAL_HDMI_AUDIO_STREAM_TYPE_MLP   = 0x800  /* Meridian Lossless Packing */
} tTDAL_HDMI_AudioStreamType;

typedef enum
{
   eTDAL_HDMI_BLOCK_EXT_TYPE_CEA,   /* CEA extension */
   eTDAL_HDMI_BLOCK_EXT_TYPE_MONITOR,  /* Monitor extension */
   eTDAL_HDMI_BLOCK_EXT_TYPE_BLOCK_MAP /* Block map extension */
} tTDAL_HDMI_EDIDExtensionType;

typedef enum
{
   eTDAL_HDMI_EDID_DISPLAY_TYPE_UNKNOWN,
   eTDAL_HDMI_EDID_DISPLAY_TYPE_MONOCHROME,
   eTDAL_HDMI_EDID_DISPLAY_TYPE_RGB,
   eTDAL_HDMI_EDID_DISPLAY_TYPE_NON_RGB
} tTDAL_HDMI_EDIDDisplayType;

typedef enum
{
   eTDAL_HDMI_EDID_NO_STEREO,
   eTDAL_HDMI_EDID_STEREO_RIGHT_IMAGE,      /* Field Sequentiel stereo, right image when stereo sync=1 */
   eTDAL_HDMI_EDID_STEREO_LEFT_IMAGE,      /* Field Sequentiel stereo, left image when stereo sync =1 */
   eTDAL_HDMI_EDID_STEREO_2WAY_INTERLEAVED_RIGHT, /* 2-way interleaved stereo, right image on even lines */
   eTDAL_HDMI_EDID_STEREO_2WAY_INTERLEAVED_LEFT,  /* 2-way interleaved stereo, left image on even lines */
   eTDAL_HDMI_EDID_STEREO_4WAY_INTERLEAVED,    /* 4-way interleaved stereo */
   eTDAL_HDMI_EDID_STEREO_SBS_INTERLEAVED     /* Side-by-side interleaved stereo */
} tTDAL_HDMI_EDIDDecodeStereo;

typedef enum
{
   eTDAL_HDMI_EDID_SYNC_ANALOG_COMPOSITE,   /* Analog composite */
   eTDAL_HDMI_EDID_SYNC_BIP_ANALOG_COMPOSITE, /* Bipolar Analog Composite */
   eTDAL_HDMI_EDID_SYNC_DIGITAL_COMPOSITE,   /* Digital Composite */
   eTDAL_HDMI_EDID_SYNC_DIGITAL_SEPARATE   /* Digital Separate */
} tTDAL_HDMI_EDIDSyncSignalType;

typedef enum
{
   eTDAL_HDMI_ESTABLISHED_TIMING_UNKNOWN   = 0x0000, /* Unknown */
   /* TIMING I */
   eTDAL_HDMI_ESTABLISHED_TIMING_720_400_70   = 0x0001, /* 720x400-70Hz IBM VGA */
   eTDAL_HDMI_ESTABLISHED_TIMING_720_400_88   = 0x0002, /* 720x400-80Hz IBM XGA2 */
   eTDAL_HDMI_ESTABLISHED_TIMING_640_480_60   = 0x0004, /* 640x480-60Hz IBM VGA */
   eTDAL_HDMI_ESTABLISHED_TIMING_640_480_67   = 0x0008, /* 640x480-67Hz Apple,MacII */
   eTDAL_HDMI_ESTABLISHED_TIMING_640_480_72   = 0x0010, /* 640x480-72Hz VESA */
   eTDAL_HDMI_ESTABLISHED_TIMING_640_480_75   = 0x0020, /* 640x480-75Hz VESA */
   eTDAL_HDMI_ESTABLISHED_TIMING_800_600_56   = 0x0040, /* 800x600-56Hz VESA */
   eTDAL_HDMI_ESTABLISHED_TIMING_800_600_60   = 0x0080, /* 800x600-60Hz VESA */
   /* TIMING II */
   eTDAL_HDMI_ESTABLISHED_TIMING_800_600_72   = 0x0100, /* 800x600-72Hz VESA */
   eTDAL_HDMI_ESTABLISHED_TIMING_800_600_75   = 0x0200, /* 800x600-75Hz VESA */
   eTDAL_HDMI_ESTABLISHED_TIMING_832_624_75   = 0x0400, /* 832x624-75Hz Apple,MacII */
   eTDAL_HDMI_ESTABLISHED_TIMING_1024_768_87  = 0x0800, /* 1024x768-87Hz Apple,MacII */
   eTDAL_HDMI_ESTABLISHED_TIMING_1024_768_60  = 0x1000, /* 1024x768-60Hz VESA */
   eTDAL_HDMI_ESTABLISHED_TIMING_1024_768_70  = 0x2000, /* 1024x768-70Hz VESA */
   eTDAL_HDMI_ESTABLISHED_TIMING_1024_768_75  = 0x4000, /* 1024x768-75Hz VESA */
   eTDAL_HDMI_ESTABLISHED_TIMING_1280_1024_75 = 0x8000  /* 1280x1024-75Hz VESA */
} tTDAL_HDMI_EstablishedTiming;

typedef enum
{
   eTDAL_HDMI_SINK_DEVICE_TYPE_UNKNOWN, /* Unknown sink device type */
   eTDAL_HDMI_SINK_DEVICE_TYPE_DVI,   /* DVI sink device type */
   eTDAL_HDMI_SINK_DEVICE_TYPE_HDMI,   /* HDMI sink device type */
   eTDAL_HDMI_SINK_DEVICE_TYPE_REPEATER /* HDMI repeater sink device type */
} tTDAL_HDMI_SinkDeviceType;

typedef enum
{
   eTDAL_HDMI_SOURCE_ID_0, /* Source identifier 0 */
   eTDAL_HDMI_SOURCE_ID_1, /* Source identifier 1 */
   eTDAL_HDMI_SOURCE_ID_2 /* Source identifier 2 */
} tTDAL_HDMI_SourceId;

typedef enum
{
   eTDAL_HDMI_VIDEO_ASPECT_RATIO_UNKNOWN, /* Unknown aspect ratio */
   eTDAL_HDMI_VIDEO_ASPECT_RATIO_16_TO_9, /* 16/9 aspect ratio */
   eTDAL_HDMI_VIDEO_ASPECT_RATIO_4_TO_3  /* 4/3 aspect ratio */
} tTDAL_HDMI_VideoAspectRatio;

typedef enum
{
   eTDAL_HDMI_VIDEO_RES_UNKNOWN, /*       Unknown */
   eTDAL_HDMI_VIDEO_RES_480I,   /*   720x480 interlaced  */
   eTDAL_HDMI_VIDEO_RES_480P,   /*   720x480 progressive */
   eTDAL_HDMI_VIDEO_RES_576I,   /*   720x576 interlaced  */
   eTDAL_HDMI_VIDEO_RES_576P,   /*   720x480 progressive */
   eTDAL_HDMI_VIDEO_RES_720P,   /*  1280x720 progressive */
   eTDAL_HDMI_VIDEO_RES_1080P,   /* 1920x1080 progressive */
   eTDAL_HDMI_VIDEO_RES_1080I,   /* 1920x1080 interlaced  */
   eTDAL_HDMI_VIDEO_RES_1035I,   /* 1920x1035 progressive */
   eTDAL_HDMI_VIDEO_RES_1035P,   /* 1920x1035 interlaced  */
   eTDAL_HDMI_VIDEO_RES_1152I   /* 1280x1152 progressive */
} tTDAL_HDMI_VideoResolution;

typedef enum
{
   eTDAL_HDMI_SRC_DEVICE_INFO_UNKNOWN,
   eTDAL_HDMI_SRC_DEVICE_INFO_DIG_STB,
   eTDAL_HDMI_SRC_DEVICE_INFO_DVD,
   eTDAL_HDMI_SRC_DEVICE_INFO_DVHS,
   eTDAL_HDMI_SRC_DEVICE_INFO_HDD_VIDEO,
   eTDAL_HDMI_SRC_DEVICE_INFO_DVC,
   eTDAL_HDMI_SRC_DEVICE_INFO_DSC,
   eTDAL_HDMI_SRC_DEVICE_INFO_VIDEO_CD,
   eTDAL_HDMI_SRC_DEVICE_INFO_GAME,
   eTDAL_HDMI_SRC_DEVICE_INFO_PC_GENERAL,
   eTDAL_HDMI_SRC_DEVICE_INFO_BLU_RAY_DISC,
   eTDAL_HDMI_SRC_DEVICE_INFO_SACD
} tTDAL_HDMI_SrcDeviceInfoSPD;

typedef void (*tTDAL_HDMI_CallbackFct)(tTDAL_HDMI_SourceId Id, tTDAL_HDMI_Event Event);

typedef struct
{
   tTDAL_HDMI_AudioStreamType eStreamType;   /* Audio stream type supported by the sink device */
   uint32_t         uiSampleFreqMask; /* Sampling frequency mask.
                      It is a bitmask of tTDAL_HDMI_AudioSampleFreq values */
   uint32_t         uiMaxChannel;   /* Maximum number of audio channels */
} tTDAL_HDMI_AudioFormat;

typedef struct
{
   uint8_t NbSource;      /* Number of different source that can be controlled */
   bool   EDIDParsingCapable; /* Indicates if parsing of E-DID data buffer is implemented in TDAL_HDMI */
}tTDAL_HDMI_Capability;

typedef struct
{
   char   cIDManufacturerName[3]; /* 3 characters manufacturer name.
                  It corresponds to an EISA ID (which was coded in compressed ASCII code) */
   uint32_t uiIDProductCode;     /* Product code sets by the manufacturer */
   uint32_t uiIDSerialNumber;    /* Optional product serial number. It can also be stored in separate descriptor */
   uint32_t uiWeekOfManufacture;   /* Week of manufacture (1 to 53). Zero means no week designated.
                  It can also be 0xFF, if manufacturer chooses to declare Model Year only */
   uint32_t uiYearOfManufacture;   /* Year of manufacture or Model Year only. 1990 to be added to get the year value.
                 (example: 1990 + 12 = 2002) */
} tTDAL_HDMI_EDIDVendorProductIdent;

typedef struct
{
   bool   bIsDigitalSignal;    /* TRUE if sink device supports digital signal. FALSE Otherwise */
   bool   bIsSetupExpected;    /* Only relevant if IsDigitalSignal is FALSE.
                  It indicates if blank-to-black setup is available */
   bool   bIsSepSyncSupported;   /* Only relevant if IsDigitalSignal is FALSE
                  It indicates if the seperate sync is supported */
   bool   bIsCompSyncSupported;   /* Only relevant if IsDigitalSignal is FALSE.
                  It indicates if the composite sync is supported*/
   bool   bIsSyncOnGreenSupported; /* Only relevant if IsDigitalSignal is FALSE
                  It indicates if sync on green video is supported */
   bool   bIsPulseRequired;    /* Only relevant if IsDigitalSignal is FALSE.
                  It indicates if assertion of the Vsync is supported */
   bool   bIsVesaDFPCompatible;   /* Only relevant if IsDigitalSignal is TRUE.
                  It indicates if the signal compatible with VESA DFP (Digital Flat Panel)
                  1.X TMDS CRGB */
   uint32_t uiSignalLevelMax;     /* Only relevant if IsDigitalSignal is FALSE.
                  Standard signal level maximum value (multiplied by 1000) */
   uint32_t uiSignalLevelMin;     /* Only relevant if IsDigitalSignal is FALSE.
                  Standard signal level minimum value (multiplied by 1000) */
} tTDAL_HDMI_EDIDVideoInputParams;

typedef struct
{
   bool           bIsStandby;       /* TRUE if the sink device standby is supported */
   bool           bIsSuspend;       /* TRUE if the sink device suspend is supported */
   bool           bIsActiveOff;      /* TRUE if the sink device active off is supported */
   bool           bIsRGBDefaultColorSpace; /* TRUE if RGB is the default colour space */
   bool           bIsPreferredTimingMode;  /* TRUE if preferred timing mode is indicated in 1st detailed timing block */
   bool           bIsDefaultGTFSupported;  /* TRUE if GTF (Generalized Timing Formula) is supported */
   tTDAL_HDMI_EDIDDisplayType eDisplayType;      /* display type*/
} tTDAL_HDMI_EDIDFeatureSupported;

typedef struct
{
   tTDAL_HDMI_EDIDVideoInputParams stVideoInput;    /* Identify the output configuration required by the sink device */
   uint32_t           uiMaxHorizImageSize; /* Indicates the sink device horizontal screen size (in cm).
                         It can be zero if size is not known.
                         It is used to calculate the aspect ratio in conjunction
                         with MaxVertImageSize */
   uint32_t           uiMaxVertImageSize;  /* Indicates the sink device vertical screen size (in cm).
                         It can be zero if size is not known.
                         It is used to calculate the aspect ratio in conjunction
                         with MaxHorizImageSize */
   uint32_t           uiDisplayGamma;   /* It indicates the display transfer characteristics (Gamma).
                         Usually used with PC CRT monitor application */
   tTDAL_HDMI_EDIDFeatureSupported stFeatureSupport;   /* Identify various display or sink device parameters (power management, ) */
} tTDAL_HDMI_EDIDDisplayParams;

typedef struct
{
   uint32_t uiRed_x;   /* Red X value   */
   uint32_t uiRed_y;   /* Red Y value   */
   uint32_t uiGreen_x; /* Green X value */
   uint32_t uiGreen_y; /* Green Y value */
   uint32_t uiBlue_x;  /* Blue X value  */
   uint32_t uiBlue_y;  /* Blue Y value  */
   uint32_t uiWhite_x; /* White X value */
   uint32_t uiWhite_y; /* White Y value */
} tTDAL_HDMI_EDIDColorParams;

typedef struct
{
   tTDAL_HDMI_EstablishedTiming eTiming1;       /* Established timing 1 */
   tTDAL_HDMI_EstablishedTiming eTiming2;       /* Established timing 2 */
   bool            bIsManufacturerTiming; /* It indicates if the established timings were adjusted
                         and verified at the factory */
} tTDAL_HDMI_EDIDEstablishedTiming;

typedef struct
{
   bool   bIsTimingUsed; /* TRUE means that this standard timing mode is used. FALSE it is not used */
   uint32_t uiHorizActive;  /* Horizontal active pixels */
   uint32_t uiVertActive;   /* Vertical active lines */
   uint32_t uiRefreshRate;  /* Refresh rate */
} tTDAL_HDMI_EDIDStandardTiming;

typedef struct
{
   uint32_t          uiPixelClock;     /* Pixel clock */
   uint32_t          uiHorizActivePixel;   /* Horizontal active pixel */
   uint32_t          uiHorizBlankingPixel;  /* Horizontal blanking pixel */
   uint32_t          uiVertActiveLine;   /* Vertical active pixel */
   uint32_t          uiVertBlankingLine;   /* Horizontal blanking pixel */
   uint32_t          uiHorizSyncOffset;   /* Horizontal synchronisation offset */
   uint32_t          uiHorizSyncWidth;   /* Horizontal synchronisation width */
   uint32_t          uiVertSyncOffset;   /* Vertical synchronisation offset */
   uint32_t          uiVertSyncWidth;    /* Vertical synchronisation width */
   uint32_t          uiHorizImageSize;   /* Horizontal image size */
   uint32_t          uiVertImageSize;    /* Vertical image size */
   uint32_t          uiHorizBorder;     /* Horizontal border */
   uint32_t          uiVertBorder;     /* Vertical border */
   bool            bIsInterlaced;    /* TRUE if interlaced mode. FALSE otherwise */
   bool            bIsHorizPolaPositive; /* TRUE if horizontal polarity is positive. FALSE otherwise */
   bool            bIsVertPolaPositive;  /* TRUE if vertical polarity is positive. FALSE otherwise*/
   tTDAL_HDMI_EDIDDecodeStereo   eDecodeStereo;    /* Stereo decode mode */
   tTDAL_HDMI_EDIDSyncSignalType eSyncSignal;     /* Synchronisation signal type */
} tTDAL_HDMI_EDIDTimingDescription;

typedef struct
{
   uint32_t uiC; /* 0=< C =<127   */
   uint32_t uiM; /* 0=< M =<65535 */
   uint32_t uiK; /* 0=< K =<255   */
   uint32_t uiJ; /* 0=< J =<127   */
} tTDAL_HDMI_EDIDGTFParams;

typedef struct
{
   uint32_t        uiMinVertRate;     /* Minimum vertical rate (in Hz) */
   uint32_t        uiMaxVertRate;     /* Maximum vertical rate (in Hz) */
   uint32_t        uiMinHorizRate;     /* Minimum horizontal rate (in KHz) */
   uint32_t        uiMaxHorizRate;     /* Maximum horizontal rate (in KHz) */
   uint32_t        uiMaxPixelClock;    /* Maximum supported pixel Clock (in MHz) */
   bool         bIsSecondGTFSupported;  /* TRUE if secondary GTF is supported */
   uint32_t        uiStartFrequency;    /* Start Frequency for secondary curve (in kHz) */
   tTDAL_HDMI_EDIDGTFParams stGTFParams;      /* Standard Generalized Timing Formula parameters */
} tTDAL_HDMI_EDIDMonitorRangeLimitDesc;

typedef struct
{
   uint8_t* pucData;  /* Pointer to descriptor data */
   uint8_t  ucLength; /* Length of descriptor data */
} tTDAL_HDMI_EDIDMonitorDataDesc;

typedef struct
{
   uint32_t uiWhite_x[2];   /* White X value for both 1 and 2 colour point */
   uint32_t uiWhite_y[2];   /* White Y value for both 1 and 2 colour point */
   uint32_t uiWhiteGamma[2]; /* White gamma value for both 1 and 2 colour point */
} tTDAL_HDMI_EDIDMonitorColorPointDesc;

typedef struct
{
   tTDAL_HDMI_EDIDStandardTiming StdTiming[6]; /* Array of 6 standard timing identifications */
} tTDAL_HDMI_EDIDMonitorTimingIdDesc;

typedef struct
{
   uint8_t        uiDescTag;   /* Descriptor tag:
                          0xFF: Monitor serial Number-stored as ASCII.
                          0xFE: ASCII String stored as ASCII.
                          0xFD: Monitor Range limits, binary coded.
                          0xFC: Monitor name, stored as ASCII.
                          0xFB: Additional colour point data.
                          0xFA: Additional standard timing identification.
                         */
   union
   {
    tTDAL_HDMI_EDIDMonitorDataDesc    stData;    /* Data description used to store ASCII formatted descriptors */
    tTDAL_HDMI_EDIDMonitorRangeLimitDesc stRangeLimit; /* Monitor range limit descriptor */
    tTDAL_HDMI_EDIDMonitorColorPointDesc stColorPoint; /* Monitor colour point descriptor */
    tTDAL_HDMI_EDIDMonitorTimingIdDesc   stTimingId;   /* Monitor timing identification descriptor */
   } uDesc;
} tTDAL_HDMI_EDIDMonitorDesc;

typedef struct
{
   uint8_t               ucHeader[8];      /* The header provides an eight byte fixed data pattern
                           that is used to easily identify the data as E-DID.
                           Header data must match the pattern defined in the E-DID
                           standard (00h, FFh, FFh, FFh, FFh, FFh, FFh, 00h) */
   tTDAL_HDMI_EDIDVendorProductIdent    stVendorProductIdent;   /* Vendor/Product identification */
   uint8_t               ucVersion;       /* E-DID version (in binary) */
   uint8_t               ucRevision;       /* E-DID revision (in binary) */
   tTDAL_HDMI_EDIDDisplayParams      stDisplayParams;     /* Basic display parameters and features */
   tTDAL_HDMI_EDIDColorParams       stColorParams;      /* Colour characteristics */
   tTDAL_HDMI_EDIDEstablishedTiming    stEstablishedTimings;   /* Established timings   */
   tTDAL_HDMI_EDIDStandardTiming      stStdTiming[8];     /* Array of standard timing identifications */
   tTDAL_HDMI_EDIDTimingDescription    stDetailedTimingDesc;   /* Array of detailed timing descriptions */
   tTDAL_HDMI_EDIDMonitorRangeLimitDesc   stRangeLimit;
   tTDAL_HDMI_EDIDMonitorDataDesc     stMonitorName;
   bool                bDetailedTimingDesc2ndFill;
   tTDAL_HDMI_EDIDTimingDescription    stDetailedTimingDesc2nd;
   bool                bMonitorDescFill;
   tTDAL_HDMI_EDIDMonitorDesc       stMonitorDesc;
   uint8_t               ucExtensionFlag;   /* Extension flag which is the number of 128 byte E-DID extension blocks */
} tTDAL_HDMI_EDIDDataBasic;

typedef struct
{
   uint8_t            ucNbTimingDesc;  /* Number of detailed timing description in E-DID CEA extension
                        version 1 filled in TimingDesc array field */
   tTDAL_HDMI_EDIDTimingDescription *pstTimingDesc; /* Array of detailed timing descriptions */
}tTDAL_HDMI_EDIDCEAExtensionVer1;

typedef struct sTDAL_HDMI_EDIDCEA_SVD
{
   bool            bIsNative;    /* TRUE if video format indicated by VideoIdentCode
                        field is a native format of the display */
   uint32_t           uiVideoIdentCode; /* Video identification code as indicated in Table3 of CEA-681-C */
   struct sTDAL_HDMI_EDIDCEA_SVD *pstNext;     /* Pointer to next E-DID CEA short video descriptor.
                        NULL if last element of the list */
} tTDAL_HDMI_EDIDCEA_SVD;

typedef struct sTDAL_HDMI_EDIDCEA_SAD
{
   tTDAL_HDMI_AudioFormat     stAudioFormat; /* audio format description */
   struct sTDAL_HDMI_EDIDCEA_SAD *pstNext;    /* Pointer to next E-DID CEA short audio descriptor.
                       NULL if last element of the list */
} tTDAL_HDMI_EDIDCEA_SAD;

typedef struct
{
   bool bIsFrontRLSupported;    /* TRUE if front right/left speakers is supported by sink device */
   bool bIsFrontCenterSupported;   /* TRUE if front center speakers is supported by sink device */
   bool bIsFrontRLCenterSupported; /* TRUE if front right/left center speakers is supported by sink device */
   bool bIsLFESupported;      /* TRUE if LFE (Low Frequency effect) speaker is supported by sink device */
   bool bIsRearRLSupported;    /* TRUE if rear right/left speakers is supported by sink device */
   bool bIsRearCenterSupported;   /* TRUE if rear center speakers is supported by sink device */
   bool bIsRearRLCenterSupported;  /* TRUE if rear right/left center speakers is supported by sink device */
} tTDAL_HDMI_EDIDCEA_SADB;

typedef struct
{
   bool   bLatencyPresent;
   bool   bInterlacedLatencyPresent;
   uint32_t uiVideoLatency;
   uint32_t uiAudioLatency;
   uint32_t uiInterlacedVideoLatency;
   uint32_t uiInterlacedAudioLatency;
   bool   bDeepColor30BitsPerPixelSupported;
   bool   bDeepColor36BitsPerPixelSupported;
   bool   bDeepColor48BitsPerPixelSupported;
   bool   bDeepColorYCbCr444Supported;
}tTDAL_HDMI_EDIDCEA_VSDBExtension;

typedef struct
{
   uint8_t       ucIEEERegistrationId[3]; /* 24 bit IEEE registration identifier (LSB 1st) */
   uint8_t       ucCECPhysicalAddress[4];
   tTDAL_HDMI_EDIDCEA_VSDBExtension   *pstExtension;
} tTDAL_HDMI_EDIDCEA_VSDB;


typedef struct
{
   bool            bIsUnderscanSink;    /* TRUE if sink device supports underscan */
   bool            bIsBasicAudioSupported; /* TRUE if sink device supports basic audio */
   bool            bIsYCbCr444Supported;   /* TRUE if sink device supports YCbCr444 in addition to RGB */
   bool            bIsYCbCr422Supported;   /* TRUE if sink device supports YCbCr422 in addition to RGB */
   uint32_t          uiNbNativeFormats;   /* Total number of native formats */
   uint8_t             ucNbTimingDesc;  /* Number of detailed timing description in E-DID CEA extension
                        version 3 filled in TimingDesc array field */
   tTDAL_HDMI_EDIDTimingDescription   *pstTimingDesc; /* Array of detailed timing descriptions */
   tTDAL_HDMI_EDIDCEA_SVD    *pstSVD;    /* Pointer to the list of Short Video Descriptors (SVD).
                        NULL if no SVD */
   tTDAL_HDMI_EDIDCEA_SAD    *pstSAD;    /* Pointer to the list of Short Audio Descriptors (SAD).
                        NULL if no SAD */
   tTDAL_HDMI_EDIDCEA_SADB    stSADB;     /* Speaker Allocation Data Block (SADB) */
   tTDAL_HDMI_EDIDCEA_VSDB    stVSDB;     /* Vendor Specific Data block (VSDB) */
} tTDAL_HDMI_EDIDCEAExtensionVer3;

typedef struct
{
   uint32_t uiRevisionNumber;         /* Revision number which should be 0x01 or 0x03.
                     0x02 revision is deprecated and should not be used in DTV monitors */
   union
   {
    tTDAL_HDMI_EDIDCEAExtensionVer1 stVer1; /* Specific descriptors according to CEA extension version 1 */
    tTDAL_HDMI_EDIDCEAExtensionVer3 stVer3; /* Specific descriptors according to CEA extension version 3 */
   } uVersionSpecific;
} tTDAL_HDMI_EDIDCEAExtension;

typedef struct
{
   uint32_t uiTag;       /* Tag value which should be 0xF0 */
   uint32_t uiExtensionTag[126]; /* Array of tags of the different block extension */
   uint32_t uiChecksum;     /* One byte checksum (addition of all bytes) */
} tTDAL_HDMI_EDIDBlockMap;

typedef struct sTDAL_HDMI_EDIDDataExtension
{
   tTDAL_HDMI_EDIDExtensionType     eType;   /* Type of the extension (CEA extension, monitor, block map) */
   union
   {
    tTDAL_HDMI_EDIDCEAExtension   stCEAExt;   /* Additional CEA extension block */
    tTDAL_HDMI_EDIDMonitorDesc    stMonitor;  /* Monitor description block */
    tTDAL_HDMI_EDIDBlockMap     stBlockMap; /* Block Map */
   } uBlock;
   struct sTDAL_HDMI_EDIDDataExtension *pstNext;   /* Pointer to next extension block. NULL if last block */
}tTDAL_HDMI_EDIDDataExtension;


typedef struct
{
   tTDAL_HDMI_EDIDDataBasic   stEDIDBasic;      /* E-DID basic data */
   bool            bIsEDIDExtensionUsed; /* TRUE if E-DID block extension is filled with data.
                         FALSE otherwise */
   tTDAL_HDMI_EDIDDataExtension *pstEDIDExtension;    /* E-DID extension block data */
} tTDAL_HDMI_EDIDData;

typedef struct
{
   uint8_t* pucBuffer; /* Pointer to E-DID raw buffer */
   uint32_t uiLength;  /* Length of E-DID raw buffer */
} tTDAL_HDMI_EDIDRawBuffer;

typedef struct
{
   tTDAL_HDMI_VideoResolution Resolution; /* Video resolution of the sink device screen display */
   uint32_t         FrameFreq;  /* Frame frequency (in milliHz) */
   uint32_t         PixelFreq;  /* Pixel frequency (in KHz) */
} tTDAL_HDMI_VideoFormat;


typedef struct
{
   tTDAL_HDMI_VideoAspectRatio  AspectRatio;             /* Aspect ratio of the sink device screen
                                 display */
   uint8_t          NbFormats;              /* Number of video formats of the sink
                                 device */
   tTDAL_HDMI_VideoFormat    Formats[kTDAL_HDMI_NB_VIDEO_FORMAT_MAX]; /* Array that lists the NbFormats video
                                 formats */
   tTDAL_HDMI_EstablishedTiming EstablishedTiming;           /* Computer display timing recognized by VESA.
                                 It is used only if NbFormats is 0 */
} tTDAL_HDMI_SinkVideoInfo;

typedef struct
{
   uint8_t       NbFormats;              /* Number of audio formats of the sink device */
   tTDAL_HDMI_AudioFormat Formats[kTDAL_HDMI_NB_AUDIO_FORMAT_MAX]; /* Array that lists the NbFormats audio formats */
} tTDAL_HDMI_SinkAudioInfo;


typedef struct
{
   tTDAL_HDMI_Event     EventState;   /* Current state of the internal state machine */
   tTDAL_HDMI_SinkDeviceType SinkDeviceType; /* Device type (HDMI or DVI) */
   tTDAL_HDMI_SinkVideoInfo  SinkVideoInfo;  /* Video information of the sink device (aspect ratio, format supported, ...) */
   tTDAL_HDMI_SinkAudioInfo  SinkAudioInfo;  /* Audio information of the sink device (audio stream type, sampling frequencies, ...) */
} tTDAL_HDMI_SinkStatus;

typedef enum
{
   eTDAL_HDMI_BAR_INFO_NOT_VALID,   /* Bar Data not valid */
   eTDAL_HDMI_BAR_INFO_V,       /* Vertical bar data valid */
   eTDAL_HDMI_BAR_INFO_H,       /* Horizental bar data valid */
   eTDAL_HDMI_BAR_INFO_VH      /* Horizental and Vertical bar data valid */
}tTDAL_HDMI_BarInfo;

typedef enum
{
   eTDAL_HDMI_PICTURE_NON_UNIFORM_SCALING,   /* No Known, non-uniform picture scaling  */
   eTDAL_HDMI_PICTURE_SCALING_H,       /* Picture has been scaled horizentally */
   eTDAL_HDMI_PICTURE_SCALING_V,       /* Picture has been scaled Vertically */
   eTDAL_HDMI_PICTURE_SCALING_HV       /* Picture has been scaled Horizentally and Vertically   */
 }tTDAL_HDMI_PictureScaling;

typedef struct
{
   bool         bhasActiveFormatInformation;
   tTDAL_HDMI_BarInfo     ebarInfo;
   tTDAL_HDMI_VideoAspectRatio   evideoAspectRatio;
   uint32_t       uiactiveAspectRatio;
   tTDAL_HDMI_PictureScaling   epictureScaling;
   uint32_t         uiPixelRepetition;
   uint32_t         uiVideoFormatIDCode;
   uint32_t         uiLineNEndofTopLower;
   uint32_t         uiLineNEndofTopUpper;
   uint32_t         uiLineNStartofBotLower;
   uint32_t         uiLineNStartofBotUpper;
   uint32_t         uiPixelNEndofLeftLower;
   uint32_t         uiPixelNEndofLeftUpper;
   uint32_t         uiPixelNStartofRightLower;
   uint32_t         uiPixelNStartofRightUpper;
}tTDAL_HDMI_InfoFrameAVI;

typedef struct
{
   uint32_t         uiVersion;       /* Version of the SPD InfoFrame.
                          Only 0x01 is defined in CEA-861-C */
   char           cVendorName[8];     /* Array of the vendor name characters (7 bit ASCII code) */
   char           cProductDescription[16]; /* Array of the product description characters (7 bit ASCII code) */
   tTDAL_HDMI_SrcDeviceInfoSPD eSrcDeviceInfo;     /* Source device information */
} tTDAL_HDMI_InfoFrameSPD;

typedef struct
{
   uint32_t uiVersion;       /* Version of the VS InfoFrame. Only 0x01 is defined in CEA-861-C */
   uint8_t  ucIEEERegistrationId[3]; /* 24 bit IEEE registration ID (LSB 1st) also called "company_id" or OUI */
   uint32_t uiSpecificPayloadLength; /* Length (in bytes) of the vendor specific payload */
   uint8_t *pucSpecificPayload;   /* Pointer to vendor specific payload data */
} tTDAL_HDMI_InfoFrameVS;

typedef struct
{
   uint32_t uiIV[2];     /* Array containing initial vector 56-bit
              (index 0 is LSB 32-bit and index 1 is MSB 24-bit) */
   uint32_t uiKSV[2];     /* Array containing key selection vector 56-bit
              (index 0 is LSB 32-bit and index 1 is MSB 24-bit) */
   uint32_t uiDeviceKeys[80]; /* Array containing source device keys in 40*56-bit
              (array index [j] is LSB 32-bit and array index [j+1] is MSB 24-bit) */
} tTDAL_HDMI_HDCPParams;

/*------------------------------------------*/
/*    Public Functions Declarations   */
/*------------------------------------------*/

IMPORT tTDAL_HDMI_Error TDAL_HDMI_Init(void);
IMPORT tTDAL_HDMI_Error TDAL_HDMI_Term(void);
IMPORT tTDAL_HDMI_Error TDAL_HDMI_CapabilityGet(tTDAL_HDMI_Capability *pstCapability);
IMPORT const char *   TDAL_HDMI_APIRevisionGet(void );
IMPORT const char *   TDAL_HDMI_PlatformRevisionGet(void );
IMPORT tTDAL_HDMI_Error TDAL_HDMI_Start(tTDAL_HDMI_SourceId Id, tTDAL_HDMI_CallbackFct NotifyFct, bool AVIDisplayFormatForced);
IMPORT tTDAL_HDMI_Error TDAL_HDMI_Stop(tTDAL_HDMI_SourceId Id);
IMPORT tTDAL_HDMI_Error TDAL_HDMI_StatusGet(tTDAL_HDMI_SourceId Id, tTDAL_HDMI_SinkStatus *pstStatus);
IMPORT tTDAL_HDMI_Error TDAL_HDMI_EDIDDataGet(tTDAL_HDMI_SourceId Id, tTDAL_HDMI_EDIDData *pstEDIDData);
IMPORT tTDAL_HDMI_Error TDAL_HDMI_EDIDDataFree(tTDAL_HDMI_SourceId Id, tTDAL_HDMI_EDIDData *pstEDIDData);
IMPORT tTDAL_HDMI_Error TDAL_HDMI_EDIDRawBufferGet(tTDAL_HDMI_SourceId Id, tTDAL_HDMI_EDIDRawBuffer *pstEDIDBuffer);
IMPORT tTDAL_HDMI_Error TDAL_HDMI_InfoFrameAVISet(tTDAL_HDMI_SourceId Id, tTDAL_HDMI_InfoFrameAVI *pstAVI);
IMPORT tTDAL_HDMI_Error TDAL_HDMI_InfoFrameSPDSet(tTDAL_HDMI_SourceId Id, tTDAL_HDMI_InfoFrameSPD *pstSPD);
IMPORT tTDAL_HDMI_Error TDAL_HDMI_InfoFrameVSSet(tTDAL_HDMI_SourceId Id, tTDAL_HDMI_InfoFrameVS *pstVS);
IMPORT tTDAL_HDMI_Error TDAL_HDMI_HDCPEnable(tTDAL_HDMI_SourceId Id);
IMPORT tTDAL_HDMI_Error TDAL_HDMI_HDCPDisable(tTDAL_HDMI_SourceId Id);
IMPORT tTDAL_HDMI_Error TDAL_HDMI_HDCPParamSet(tTDAL_HDMI_SourceId Id, tTDAL_HDMI_HDCPParams *pParams);

#ifdef __cplusplus
}
#endif

#endif /*_TDAL_HDMI_H_*/

