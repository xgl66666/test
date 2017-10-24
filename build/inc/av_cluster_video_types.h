/******************************************************************************
 *                         COPYRIGHT 2008 IWEDIA TECHNOLOGIES                         *
 ******************************************************************************
 *
 * MODULE NAME: AVCLUSTER_AUDIO_TYPES
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/product/comedia/branches/idecode_hd/build/inc/av_cluster_video_types.h $
 *            $Rev: 244 $
 *
 * PUBLIC
 *
 * DESCRIPTION
 *
 *
 *****************************************************************************/

#ifndef _AVCLUSTER_VIDEO_TYPES_H_
#define _AVCLUSTER_VIDEO_TYPES_H_


#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    eAVCLUSTER_VIDEO_14_9_FULL_FORMAT, /* Full Format 4:3 (Shoot&Protect 14:9 center) */
    eAVCLUSTER_VIDEO_14_9_LETTERBOX_CENTER,
    eAVCLUSTER_VIDEO_14_9_LETTERBOX_TOP,
    eAVCLUSTER_VIDEO_16_9_FULL_FORMAT,
    eAVCLUSTER_VIDEO_16_9_LETTERBOX_CENTER,
    eAVCLUSTER_VIDEO_16_9_LETTERBOX_TOP,
    eAVCLUSTER_VIDEO_4_3_FULL_FORMAT,
    eAVCLUSTER_VIDEO_HIGHER_THAN_16_9_LETTERBOX_CENTER
}
tAVCLUSTER_VIDEO_WSS;

typedef enum
{
    eAVCLUSTER_VIDEO_STD_NONE             = 0x00000000,
    eAVCLUSTER_VIDEO_STD_640_480_P_60     = 0x00000001, /* VESA */
    eAVCLUSTER_VIDEO_STD_720_480_P_5994   = 0x00000002, /* ATSC P59.94 */
    eAVCLUSTER_VIDEO_STD_720_480_P_60     = 0x00000004, /* ATSC P60 square */
    eAVCLUSTER_VIDEO_STD_720_480_I_5994   = 0x00000008, /* NTSC , PAL M */
    eAVCLUSTER_VIDEO_STD_720_480_I_60     = 0x00000010, /* NTSC I60 square */
    eAVCLUSTER_VIDEO_STD_720_483_P_5994   = 0x00000020, /* */
    eAVCLUSTER_VIDEO_STD_720_576_P_50     = 0x00000040, /* ITU-R BT 1358 */
    eAVCLUSTER_VIDEO_STD_720_576_I_50     = 0x00000080, /* PAL B,D,G,H,I,N,SECAM */
    eAVCLUSTER_VIDEO_STD_1280_720_P_2398  = 0x00000100, /* ATSC 720*1280 P24/1.001 */
    eAVCLUSTER_VIDEO_STD_1280_720_P_24    = 0x00000200, /* ATSC 720*1280 P24 */
    eAVCLUSTER_VIDEO_STD_1280_720_P_25    = 0x00000400, /* ATSC 720*1280 P25 */
    eAVCLUSTER_VIDEO_STD_1280_720_P_2997  = 0x00000800, /* ATSC 720*1280 P30/1.001 */
    eAVCLUSTER_VIDEO_STD_1280_720_P_30    = 0x00001000, /* ATSC 720*1280 P30 */
    eAVCLUSTER_VIDEO_STD_1280_720_P_50    = 0x00002000, /* ATSC 720*1280 P50 */
    eAVCLUSTER_VIDEO_STD_1280_720_P_5994  = 0x00004000, /* SMPTE296M #2 P60/1.001 */
    eAVCLUSTER_VIDEO_STD_1280_720_P_60    = 0x00008000, /* SMPTE296M #1 P60 */
    eAVCLUSTER_VIDEO_STD_1920_1080_P_2398 = 0x00010000, /* SMPTE274M #11 P24/1.001 */
    eAVCLUSTER_VIDEO_STD_1920_1080_P_24   = 0x00020000, /* SMPTE274M #10 P24 */
    eAVCLUSTER_VIDEO_STD_1920_1080_P_25   = 0x00040000, /* SMPTE274M #9 P25 */
    eAVCLUSTER_VIDEO_STD_1920_1080_P_2997 = 0x00080000, /* SMPTE274M #8 P30/1.001 */
    eAVCLUSTER_VIDEO_STD_1920_1080_P_30   = 0x00100000, /* SMPTE274M #7 P30 */
    eAVCLUSTER_VIDEO_STD_1920_1080_P_50   = 0x00200000, /* SMPTE274M #3 P50 */
    eAVCLUSTER_VIDEO_STD_1920_1080_P_5994 = 0x00400000, /* SMPTE274M #2 P60/1.001 */
    eAVCLUSTER_VIDEO_STD_1920_1080_P_60   = 0x00800000, /* SMPTE274M #1 P60 */
    eAVCLUSTER_VIDEO_STD_1920_1080_I_50   = 0x01000000, /* SMPTE295M #2 I50 */
    eAVCLUSTER_VIDEO_STD_1920_1080_I_5994 = 0x02000000, /* SMPTE274M #5 I60/1.001 */
    eAVCLUSTER_VIDEO_STD_1920_1080_I_60   = 0x04000000  /* SMPTE274M #4 I60 */
} tAVCLUSTER_VIDEO_Standard;

typedef enum
{
    eAVCLUSTER_VIDEO_AR_CONVERSION_FULL_SCREEN        = 0x00001,
    eAVCLUSTER_VIDEO_AR_CONVERSION_16_9_LETTER_BOX    = 0x00002,/* LETTER_BOX */
    eAVCLUSTER_VIDEO_AR_CONVERSION_14_9_LETTER_BOX    = 0x00004,/* COMBINED */
    eAVCLUSTER_VIDEO_AR_CONVERSION_4_3_CENTER_CUT_OUT = 0x00008,/* PAN_SCAN */
    eAVCLUSTER_VIDEO_AR_CONVERSION_4_3_PILLAR_BOX     = 0x00010,
    eAVCLUSTER_VIDEO_AR_CONVERSION_ZOOM_4_3           = 0x00020,
    eAVCLUSTER_VIDEO_AR_CONVERSION_ZOOM_14_9          = 0x00040,
    eAVCLUSTER_VIDEO_AR_CONVERSION_SQUARE             = 0x00080,
	eAVCLUSTER_VIDEO_AR_CONVERSION_14_9_PILLAR_BOX    = 0x00100,
	eAVCLUSTER_VIDEO_AR_CONVERSION_WIDE_CENTER_CUT_OUT= 0x00200,
	eAVCLUSTER_VIDEO_AR_CONVERSION_WIDE_ZOOM		  = 0x00400,
	eAVCLUSTER_VIDEO_AR_CONVERSION_ZOOM_16_9          = 0x00800,
	eAVCLUSTER_VIDEO_AR_CONVERSION_STRETCH_16_9		  = 0x01000,
    eAVCLUSTER_VIDEO_AR_CONVERSION_WZ43IN169          = 0x02000,
    eAVCLUSTER_VIDEO_AR_CONVERSION_ST43IN169          = 0x04000,
    eAVCLUSTER_VIDEO_AR_CONVERSION_WZ43SP149IN169     = 0x08000,
    eAVCLUSTER_VIDEO_AR_CONVERSION_ST43SP149IN169     = 0x10000,
    eAVCLUSTER_VIDEO_AR_CONVERSION_ST149IN169         = 0x20000
}
tAVCLUSTER_VIDEO_ARConversion;

typedef enum
{
	eAVCLUSTER_VIDEO_OUTPUT_RESOLUTION_MODE_AUTO,
	eAVCLUSTER_VIDEO_OUTPUT_RESOLUTION_MODE_FORCED
}
tAVCLUSTER_VIDEO_OutputResolutionMode;

typedef enum
{
    eAVCLUSTER_OSD_MODE_NORMAL,
    eAVCLUSTER_OSD_MODE_4_3_SCENE,
    eAVCLUSTER_OSD_MODE_16_9_SCENE
}
tAVCLUSTER_SceneMode;

typedef enum
{
    eAVCLUSTER_AR_UNKNOWN,
    eAVCLUSTER_AR_4_3,
    eAVCLUSTER_AR_16_9_PAN_VECTOR,
    eAVCLUSTER_AR_16_9,
    eAVCLUSTER_AR_MORE_16_9
}
tAVCLUSTER_VIDEO_StreamAspectRatio;

typedef enum
{
	eAVCLUSTER_VIDEO_NO_SOURCE,
    eAVCLUSTER_VIDEO_SOURCE_LIVE,
    eAVCLUSTER_VIDEO_SOURCE_SAMPLE
}
tAVCLUSTER_VIDEO_SourceType;

typedef enum
{
	eAVCLUSTER_OUTPUT_AR_NONE,
	eAVCLUSTER_OUTPUT_AR_4_3,
	eAVCLUSTER_OUTPUT_AR_16_9
}
tAVCLUSTER_VIDEO_OutputAR;

typedef enum
{
	eAVCLUSTER_VIDEO_DEFAULT_RULES,
	eAVCLUSTER_VIDEO_MHEG_RULES
}
tAVCLUSTER_VIDEO_Rules;

typedef struct
{
    int16_t sLeft;   /* Left position of the window rectangle (in pixels).*/
    int16_t sTop;    /* Top position of the window rectangle (in lines).*/
    int16_t sRight;  /* Right position of the window rectangle (in pixels).*/
    int16_t sBottom; /* Bottom position of the window rectangle (in lines).*/
}
tAVCLUSTER_VIDEO_Window;

typedef struct
{
    uint32_t uiWidth;  /* Width of the output rectangle (in pixels). */
    uint32_t uiHeight; /* Height of the output rectangle (in lines).*/
}
tAVCLUSTER_VIDEO_Resolution;

typedef struct
{
    uint32_t uiWidth;  /* Width of the output rectangle (in pixels). */
    uint32_t uiHeight; /* Height of the output rectangle (in lines).*/
}
tAVCLUSTER_OSD_Resolution;

typedef struct
{
	tAVCLUSTER_SceneMode				eSceneMode;
	tAVCLUSTER_OSD_Resolution			stOSDNativeResolution;
	tAVCLUSTER_VIDEO_Rules				eRules;
}
tAVCLUSTER_VIDEO_UserCommonOutputPreferences;

typedef struct
{
	uint8_t									ucOutputType;
	tAVCLUSTER_VIDEO_Window					stOutputWindow;/* referenced from OSD Resolution */
	uint8_t									ucFullScreen; /* if 1 = no size = max size */
	tAVCLUSTER_VIDEO_Window					stVisibleWindow; /* referenced from OSD Resolution */
	tAVCLUSTER_VIDEO_ARConversion			eARConversion;
	tAVCLUSTER_VIDEO_ARConversion			eForcedDecFC;
	tAVCLUSTER_VIDEO_OutputResolutionMode	eOutputResolutionMode;
	/* Only if eOutputResolutionMode is not AUTO */
	tAVCLUSTER_VIDEO_Standard				eOutputVideoStandard;
}
tAVCLUSTER_VIDEO_UserOutputPreferences;

typedef struct
{
	tAVCLUSTER_VIDEO_SourceType				eVideoSourceType;
	tAVCLUSTER_VIDEO_Resolution				stResolution;
	tAVCLUSTER_VIDEO_StreamAspectRatio		eStreamAspectRatio;
	uint8_t									eAFD;
    bool                                    bCopyProtection;
}
tAVCLUSTER_VIDEO_StreamContent;

typedef struct
{
	int8_t										cOutputId;
	uint8_t										ucVideoOutputType;
	tAVCLUSTER_VIDEO_Standard					eResolutionSupported;
	tAVCLUSTER_VIDEO_Standard					eSinkResolutionSupported;
    bool                                        bHDCPCapable;
    bool                                        bHDCPAuthenticated;
}
tAVCLUSTER_VIDEO_Output;

typedef struct
{
	int8_t									cOutputId;
	tAVCLUSTER_VIDEO_OutputAR				eOutputAspectRatio;
	tAVCLUSTER_VIDEO_WSS					eWSS;
	tAVCLUSTER_VIDEO_ARConversion			eDecFC;
}
tAVCLUSTER_VIDEO_Processing;

typedef struct
{
	int8_t									cOutputId;
	tAVCLUSTER_VIDEO_Standard				eOutputVideoStandard;
	tAVCLUSTER_VIDEO_Window					stInputWindow;
	tAVCLUSTER_VIDEO_Window					stOutputWindow;
}
tAVCLUSTER_VIDEO_Configuration;

#ifdef __cplusplus
}
#endif

#endif
