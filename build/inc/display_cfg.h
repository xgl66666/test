/******************************************************************************
 *                           COPYRIGHT 2005 IWEDIA                            *
 ******************************************************************************
 *
 * MODULE NAME: CFG DISPLAY
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/product/comedia/branches/idecode_hd/build/inc/display_cfg.h $
 *            $Rev: 244 $
 *            $Date: 2011-08-18 18:33:39 +0200 (Thu, 18 Aug 2011) $
 *
 * PUBLIC
 *
 * DESCRIPTION
 *
 *****************************************************************************/

#ifndef _DISPLAY_CFG_H_
#define _DISPLAY_CFG_H_

#ifdef __cplusplus
extern "C" {
#endif


/*******************************************************/
/*               typedef                               */
/*******************************************************/
typedef struct
{
	uint32_t 	value;
	char* 		string;
}tDISPLAY_CFG_List;

tDISPLAY_CFG_List * DISPLAY_CFG_GetTVFormatList(uint16_t *nbItems);
tDISPLAY_CFG_List * DISPLAY_CFG_GetTVFormatList_Aux(uint16_t *nbItems);
tDISPLAY_CFG_List * DISPLAY_CFG_GetTVOutputList(uint16_t *nbItems);
tDISPLAY_CFG_List * DISPLAY_CFG_GetTVModeList(uint16_t *nbItems);
tDISPLAY_CFG_List * DISPLAY_CFG_GetVCROutputList(uint16_t *nbItems);
tDISPLAY_CFG_List * DISPLAY_CFG_GetTVHDResolutionList(uint16_t *nbItems);
tDISPLAY_CFG_List * DISPLAY_CFG_GetVideoHDStandardList(uint16_t *nbItems);
void DISPLAY_CFG_SetTVHDefaultResolution(uint8_t Index);

cmSetup_VideoAspectRatio_t 		DISPLAY_CFG_GetDefaultTVFormat(void);
cmSetup_TvOutputSignal_t 		DISPLAY_CFG_GetDefaultTVOutput(void);
cmSetup_VideoEncodingMode_t 	DISPLAY_CFG_GetDefaultTVMode(void);
cmSetup_TvOutputSignal_t 		DISPLAY_CFG_GetDefaultVCROutput(void);
cmSetup_TVHDResolution_t		DISPLAY_CFG_GetDefaultTVHDResolution(void);
bool                            DISPLAY_CFG_GetDefaultTVHDStatus(void);

uint32_t	DISPLAY_CFG_GetDefaultVolume(void);

cmSetup_RFModBand_t 			DISPLAY_CFG_GetRFModDefaultBand(void);
uint32_t						DISPLAY_CFG_GetRFModDefaultChannel(void);
cmSetup_VideoEncodingMode_t		DISPLAY_CFG_GetRFModDefaultStandard(void);

tDISPLAY_CFG_List * DISPLAY_CFG_GetRFModBandList(uint16_t *nbItems);
tDISPLAY_CFG_List * DISPLAY_CFG_GetRFModStandardList(uint16_t *nbItems);

cmSetup_ScreenSaverTimeout_t	DISPLAY_CFG_GetDefaultScreenSaverTimeout(void);

#ifdef __cplusplus
}
#endif

#endif /* _DISPLAY_CFG_H_ */
