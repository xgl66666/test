
/******************************************************************************
 *            COPYRIGHT 2008 IWEDIA TECHNOLOGIES            *
 ******************************************************************************
 *
 * MODULE NAME: TDAL_ANA_VID
 *
 * FILE NAME: $URL: http://10.50.1.219/svncomedia/sources/chal/core/trunk/tdal/inc/tdal_ana_vid.h $
 *
 *      $Rev: 1778 $
 *      $Date: 2009-10-21 14:05:06 +0200 (mer., 21 oct. 2009) $
 *
 * PUBLIC
 *
 * DESCRIPTION : API Prototype for TDAL_ANA_VID abstraction layer
 *
 *****************************************************************************/

#ifndef _TDAL_ANA_VID_H_
#define _TDAL_ANA_VID_H_


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
   eTDAL_ANA_VID_NO_ERROR,      /* No Error returned */
   eTDAL_ANA_VID_NOT_INIT_ERROR,   /* TDAL_ANA_VID has not been initialized */
   eTDAL_ANA_VID_ALREADY_INIT_ERROR,  /* TDAL_ANA_VID already initialized */
   eTDAL_ANA_VID_BAD_ARG_ERROR,    /* Bad parameter given to the function */
   eTDAL_ANA_VID_DRIVER_ERROR,    /* Low level driver returns an error */
   eTDAL_ANA_VID_NO_MEMORY_ERROR,   /* Not enough memory to perform action*/
   eTDAL_ANA_VID_NOT_AVAILABLE_ERROR, /* Feature not available*/
}tTDAL_ANA_VID_Error;

typedef enum
{
   eTDAL_ANA_VID_DECODER_1 = 0x00000001,
   eTDAL_ANA_VID_DECODER_2 = 0x00000002,
   eTDAL_ANA_VID_DECODER_3 = 0x00000004,
   eTDAL_ANA_VID_DECODER_4 = 0x00000008,
}tTDAL_ANA_VID_Decoder;

typedef enum
{
   eTDAL_ANA_VID_STD_UNKNOWN  = 0xFFFFFFFF,
   eTDAL_ANA_VID_STD_PAL_BGHI = 0x00000001,
   eTDAL_ANA_VID_STD_PAL_N   = 0x00000002,
   eTDAL_ANA_VID_STD_PAL_M   = 0x00000004,
   eTDAL_ANA_VID_STD_SECAM   = 0x00000008,
   eTDAL_ANA_VID_STD_NTSC   = 0x00000010,
   eTDAL_ANA_VID_STD_PC    = 0x00000020
} tTDAL_ANA_VID_Std;

typedef enum
{
   eTDAL_ANA_VID_CODING_UNKNOWN = 0xFFFFFFFF,/* */
   eTDAL_ANA_VID_CODING_CVBS   = 0x00000001,/* */
   eTDAL_ANA_VID_CODING_YC   = 0x00000002,/* */
   eTDAL_ANA_VID_CODING_YPRPB   = 0x00000004,/* */
   eTDAL_ANA_VID_CODING_RGB_S   = 0x00000008, /* RGB + composite synch signal */
   eTDAL_ANA_VID_CODING_RGB_SOG = 0x00000010,/* RGB Sync On Green */
   eTDAL_ANA_VID_CODING_RGB_HV  = 0x00000020,/* RGB + Horiz/Vert synch signals */
} tTDAL_ANA_VID_Coding;

typedef enum
{
   eTDAL_ANA_VID_SCAN_TYPE_UNKNOWN   = 0xFFFFFFFF,/* */
   eTDAL_ANA_VID_SCAN_TYPE_INTERLACED  = 0x00000001,/* frame consists of two fields */
   eTDAL_ANA_VID_SCAN_TYPE_PROGRESSIVE = 0x00000002,/* frame consists of one field*/
} tTDAL_ANA_VID_ScanType;

typedef enum
{
   eTDAL_ANA_VID_EVENT_TYPE_CHANGE = 0,
   eTDAL_ANA_VID_EVENT_TIMING_CHANGE,
   eTDAL_ANA_VID_EVENT_SIGNAL_LOST,
   eTDAL_ANA_VID_EVENT_NEED_DISPLAY
} tTDAL_ANA_VID_Event;

typedef void (*tTDAL_ANA_VID_CallbackProc)
(
   tTDAL_ANA_VID_Decoder eDecoder,
   tTDAL_ANA_VID_Event   eEvent,
   const void      *pEventData
);


typedef struct TDAL_ANA_VID_Capability_s
{
   uint32_t uiDecoderSupported;
   bool   bPcInputAutomaticAdaptationModeSupported;
} tTDAL_ANA_VID_Capability;

typedef struct TDAL_ANA_VID_DecoderCapability_s
{
   uint32_t uiSupportedStdMask;
   uint32_t uiSupportedCodingMask;
} tTDAL_ANA_VID_DecoderCapability;

typedef struct TDAL_ANA_VID_Type_s
{
   tTDAL_ANA_VID_Coding   eCoding;
   tTDAL_ANA_VID_Std   eStd;
} tTDAL_ANA_VID_Type;

typedef struct
{
   uint32_t       uiFrameRate;
   tTDAL_ANA_VID_ScanType eScanType;
   uint32_t       uiActiveAreaWidth;
   uint32_t       uiActiveAreaHeight;
   uint32_t       uiActiveAreaXStart;
   uint32_t       uiActiveAreaYStart;
} tTDAL_ANA_VID_Timing;

typedef struct TDAL_ANA_VID_StartParam_s
{
   uint32_t uiStdMask;
   uint32_t uiCodingMask;
} tTDAL_ANA_VID_StartParams;

typedef struct
{
   uint32_t uiActiveAreaWidth;
   uint32_t uiActiveAreaHeight;
   uint32_t uiActiveAreaXStart;
   uint32_t uiActiveAreaYStart;

   uint16_t usPixelClock;     /* in kHz */
   uint16_t usPhase;      /* in degree */
} tTDAL_ANA_VID_PC_FormatInformation;

typedef void (*tTDAL_ANA_VID_PC_CallbackProc)
(
   tTDAL_ANA_VID_Decoder         eDecoder,
   const tTDAL_ANA_VID_PC_FormatInformation *pstPCFormatInfo
);

/*******************************************************/
/*       Variables Declarations (IMPORT)    */
/*******************************************************/

/*******************************************************/
/*       Functions Declarations        */
/*******************************************************/

tTDAL_ANA_VID_Error TDAL_ANA_VID_Init (void);
tTDAL_ANA_VID_Error TDAL_ANA_VID_Term (void);

tTDAL_ANA_VID_Error TDAL_ANA_VID_CapabilityGet(tTDAL_ANA_VID_Capability         *pstCapability);

const char *    TDAL_ANA_VID_APIRevisionGet(void);
const char *    TDAL_ANA_VID_PlatformRevisionGet(void);

tTDAL_ANA_VID_Error TDAL_ANA_VID_DecoderCapabilityGet(tTDAL_ANA_VID_Decoder        eDecoder,
                        tTDAL_ANA_VID_DecoderCapability   *pstCapability;);

tTDAL_ANA_VID_Error TDAL_ANA_VID_EventSubscribe(tTDAL_ANA_VID_Decoder           eDecoder,
                     tTDAL_ANA_VID_Event            eEvent,
                     tTDAL_ANA_VID_CallbackProc         pFctNotifyFunction);
tTDAL_ANA_VID_Error TDAL_ANA_VID_EventUnsubscribe(tTDAL_ANA_VID_Decoder         eDecoder,
                      tTDAL_ANA_VID_Event           eEvent);

tTDAL_ANA_VID_Error TDAL_ANA_VID_InputStreamSet(tTDAL_ANA_VID_Decoder           eDecoder,
                     uint32_t                streamHandle);

tTDAL_ANA_VID_Error TDAL_ANA_VID_DestinationSet(tTDAL_ANA_VID_Decoder           eDecoder,
                     uint32_t                layerHandle);

tTDAL_ANA_VID_Error TDAL_ANA_VID_Start(tTDAL_ANA_VID_Decoder              eDecoder,
                  tTDAL_ANA_VID_StartParams            *pstStartParams);
tTDAL_ANA_VID_Error TDAL_ANA_VID_Stop(tTDAL_ANA_VID_Decoder               eDecoder);

tTDAL_ANA_VID_Error TDAL_ANA_VID_TypeGet(tTDAL_ANA_VID_Decoder              eDecoder,
                  tTDAL_ANA_VID_Type               *pstType);

tTDAL_ANA_VID_Error TDAL_ANA_VID_TimingGet(tTDAL_ANA_VID_Decoder            eDecoder,
                   tTDAL_ANA_VID_Timing            *pstTiming);

/* PC input specific functions */
tTDAL_ANA_VID_Error TDAL_ANA_VID_EnablePcInputAutomaticMode(tTDAL_ANA_VID_Decoder      eDecoder,
                    bool            bEnableAutomaticMode,
                    tTDAL_ANA_VID_PC_CallbackProc   pFctNotifyFunction);

tTDAL_ANA_VID_Error TDAL_ANA_VID_SetPcFormatInfo(tTDAL_ANA_VID_Decoder          eDecoder,
                tTDAL_ANA_VID_PC_FormatInformation    *pstPcFormat);

tTDAL_ANA_VID_Error TDAL_ANA_VID_GetPcFormatInfo(tTDAL_ANA_VID_Decoder          eDecoder,
                tTDAL_ANA_VID_PC_FormatInformation    *pstPcFormat);


#ifdef __cplusplus
}
#endif

#endif  /*_TDAL_ANA_VID_H_*/




