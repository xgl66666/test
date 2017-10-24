
/******************************************************************************
 *            COPYRIGHT 2008 IWEDIA TECHNOLOGIES            *
 ******************************************************************************
 *
 * MODULE NAME: TDAL_PANEL
 *
 * FILE NAME: $URL: http://10.50.1.219/svncomedia/sources/chal/core/trunk/tdal/inc/tdal_panel.h $ 
 * 
 *      $Rev: 739 $
 *      $Date: 2009-01-27 11:13:12 +0100 (mar., 27 janv. 2009) $
 *
 * PUBLIC
 *
 * DESCRIPTION : API Prototype for TDAL_PANEL abstraction layer 
 *
 *****************************************************************************/

#ifndef _TDAL_PANEL_H_
#define _TDAL_PANEL_H_

/*******************************************************/
/*       Includes              */
/*******************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************/
/*       Defines              */
/*******************************************************/
#define kTDAL_PANEL_RANGE_VALUE_MIN      0
#define kTDAL_PANEL_RANGE_VALUE_MAX     100

/*******************************************************/
/*       Macros               */
/*******************************************************/

/*******************************************************/
/*       Typedefs              */
/*******************************************************/
typedef enum 
{
   eTDAL_PANEL_NO_ERROR,      /* No Error returned */
   eTDAL_PANEL_NOT_INIT_ERROR,   /* TDAL_PANEL not initialised */
   eTDAL_PANEL_ALREADY_INIT_ERROR,  /* TDAL_PANEL already initialised */
   eTDAL_PANEL_BAD_ARG_ERROR,    /* Bad parameter given to the function */
   eTDAL_PANEL_DRIVER_ERROR,    /* Low level driver returns an error */  
   eTDAL_PANEL_NOT_AVAILABLE_ERROR  /* Feature not available*/
}tTDAL_PANEL_Error;

typedef struct TDAL_PANEL_Capability_s 
{
   uint8_t ucControlBacklightSupported;       /* 0: Not supported / Step otherwise */

   uint8_t ucBrightnessSupported;
   uint8_t ucContrastSupported;
   uint8_t ucSaturationSupported;
   uint8_t ucSharpnessSupported;
   uint8_t ucColorTemperatureSupported;
   uint8_t ucColorTemperatureByWhitePointSupported;

   uint8_t ucControlAnalogNoiseReductionSupported;
   uint8_t ucControlMpegNoiseReductionSupported;   

   uint8_t ucBlackColorCorrectionSupported;
   uint8_t ucWhiteColorCorrectionSupported;

   uint8_t ucGammaCorrectionSupported;

   uint8_t ucPhaseClockPositionSupported;
} tTDAL_PANEL_Capability;


/*******************************************************/
/*       Variables Declarations (IMPORT)    */
/*******************************************************/

/*******************************************************/
/*       Functions Declarations        */
/*******************************************************/
tTDAL_PANEL_Error    TDAL_PANEL_Init(void);
tTDAL_PANEL_Error    TDAL_PANEL_Term(void);

tTDAL_PANEL_Error    TDAL_PANEL_CapabilityGet(tTDAL_PANEL_Capability *pstCapability);

const char *      TDAL_VBI_PANEL_APIRevisionGet(void);
const char *      TDAL_PANEL_PlatformRevisionGet(void);

tTDAL_PANEL_Error    TDAL_PANEL_GetControlBacklight(bool    *pbControl,
                        uint8_t   *pucBackLightLevel);
tTDAL_PANEL_Error    TDAL_PANEL_SetControlBacklight(bool    bControl,
                        uint8_t   ucBacklightLevelToApply);

tTDAL_PANEL_Error    TDAL_PANEL_GetBrightness(uint8_t     *pucBrightness);
tTDAL_PANEL_Error    TDAL_PANEL_SetBrightness(uint8_t      ucBrightness);

tTDAL_PANEL_Error    TDAL_PANEL_GetContrast(uint8_t      *pucContrast);
tTDAL_PANEL_Error    TDAL_PANEL_SetContrast(uint8_t      ucContrast);

tTDAL_PANEL_Error    TDAL_PANEL_GetSaturation(uint8_t      *pucSaturation);
tTDAL_PANEL_Error    TDAL_PANEL_SetSaturation(uint8_t      ucSaturation);

tTDAL_PANEL_Error    TDAL_PANEL_GetSharpness(uint8_t *pucSharpness);
tTDAL_PANEL_Error    TDAL_PANEL_SetSharpness(uint8_t ucSharpness);

tTDAL_PANEL_Error    TDAL_PANEL_GetColorTemperature(uint8_t   *pucColorTemperature);
tTDAL_PANEL_Error    TDAL_PANEL_SetColorTemperature(uint8_t   ucColorTemperature);

tTDAL_PANEL_Error    TDAL_PANEL_GetColorTemperatureByWhitePoint(uint8_t   *pucRedWhitePoint,
                              uint8_t   *pucBlueWhitePoint,
                              uint8_t   *pucGreenWhitePoint);
tTDAL_PANEL_Error    TDAL_PANEL_SetColorTemperatureByWhitePoint(uint8_t   ucRedWhitePoint,
                              uint8_t   ucBlueWhitePoint,
                              uint8_t   ucGreenWhitePoint);

tTDAL_PANEL_Error    TDAL_PANEL_GetAnalogNoiseReduction(bool   *pbControl,
                          uint8_t *pucNoiseReduction);
tTDAL_PANEL_Error    TDAL_PANEL_SetAnalogNoiseReduction(bool   bControl,
                          uint8_t ucNoiseReduction);

tTDAL_PANEL_Error    TDAL_PANEL_GetMpegNoiseReduction(bool   *pbControl,
                         uint8_t   *pucNoiseReduction);
tTDAL_PANEL_Error    TDAL_PANEL_SetMpegNoiseReduction(bool    bControl,
                         uint8_t   ucNoiseReduction);

tTDAL_PANEL_Error    TDAL_PANEL_GetBlackColorCorrection(uint8_t *pucBlackColorCorrection);
tTDAL_PANEL_Error    TDAL_PANEL_SetBlackColorCorrection(uint8_t ucBlackColorCorrection);

tTDAL_PANEL_Error    TDAL_PANEL_GetWhiteColorCorrection(uint8_t *pucWhiteColorCorrection);
tTDAL_PANEL_Error    TDAL_PANEL_SetWhiteColorCorrection(uint8_t  ucWhiteColorCorrection);

tTDAL_PANEL_Error    TDAL_PANEL_GetGammaCorrection(uint8_t   *pucGammaCorrection);
tTDAL_PANEL_Error    TDAL_PANEL_SetGammaCorrection(uint8_t    ucGammaCorrection);


tTDAL_PANEL_Error    TDAL_PANEL_GetPhaseClockPosition(bool   *pbManualControl,
                         uint8_t   *pucPhase,
                         uint8_t   *pucClock,
                         uint8_t   *pucPositionHorizontal,
                         uint8_t   *pucPositionVertical);
tTDAL_PANEL_Error    TDAL_PANEL_SetPhaseClockPosition(bool    bManualControl,
                         uint8_t   ucPhase,
                         uint8_t   ucClock,
                         uint8_t   ucPositionHorizontal,
                         uint8_t   ucPositionVertical);

#ifdef __cplusplus
}
#endif

#endif  /*_TDAL_PANEL_H_*/

