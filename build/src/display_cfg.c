/********************************************************************************
 *                       COPYRIGHT 2005 IWEDIA                                  *
 ********************************************************************************
 *
 * MODULE NAME: FLASH CFG
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/product/comedia/branches/idecode_hd/build/src/display_cfg.c $
 *            $Rev: 673 $
 *            $Date: 2011-10-28 15:10:43 +0200 (Fri, 28 Oct 2011) $
 *
 *
 *******************************************************************************/

/********************************************************/
/*                Includes                              */
/********************************************************/

#include "crules.h"
#include "string.h"

#include "pinfodbase.h"
#include "cm_setup.h"

#include "display_cfg.h"

tDISPLAY_CFG_List DISPLAY_CFG_TVFormatList[] =
{
#if defined(HD_ENABLE)
    {eSETUP_ASPECT_RATIO_16_9,          "16:9 Fit to Screen"},
    {eSETUP_ASPECT_RATIO_16_9_PILLAR_BOX,     "16:9 Pillar Box"},
    {eSETUP_ASPECT_RATIO_16_9_PANSCAN,  "16:9 Pan Scan"}
#else
    {eSETUP_ASPECT_RATIO_4_3,      "4:3 Fit to Screen"},
    {eSETUP_ASPECT_RATIO_4_3_LETTERBOX, "4:3 Letterbox"},
    {eSETUP_ASPECT_RATIO_4_3_PANSCAN,        "4:3 Pan Scan"},
    {eSETUP_ASPECT_RATIO_16_9,               "16:9 Fit to Screen"}
#endif
};

#if defined (AUXILIARY_VIDEO_DISPLAY_SD)
#define DISPLAY_CFG_NB_TVFORMAT_AUX	4
#else
#define DISPLAY_CFG_NB_TVFORMAT_AUX	0
#endif

tDISPLAY_CFG_List DISPLAY_CFG_TVFormatListAux[DISPLAY_CFG_NB_TVFORMAT_AUX] =
{
#if defined (AUXILIARY_VIDEO_DISPLAY_SD)
    {eSETUP_ASPECT_RATIO_4_3,      "4:3 Fit to Screen"},
    {eSETUP_ASPECT_RATIO_4_3_LETTERBOX, "4:3 Letterbox"},
	{eSETUP_ASPECT_RATIO_4_3_PANSCAN,        "4:3 Pan Scan"},
    {eSETUP_ASPECT_RATIO_16_9,          "16:9 Fit to Screen"}
#endif
};



#define DISPLAY_CFG_NB_TVOUTPUT	2

tDISPLAY_CFG_List DISPLAY_CFG_TVOutputList[DISPLAY_CFG_NB_TVOUTPUT] =
{
	{eSETUP_CVBS, "CVBS"},
	{eSETUP_RGB, "RGB"}
};


#define DISPLAY_CFG_NB_TVMODE 3

tDISPLAY_CFG_List DISPLAY_CFG_TVModeList[DISPLAY_CFG_NB_TVMODE] =
{
	{eSETUP_PAL, "PAL-B"},
	{eSETUP_PALM, "PAL-M"},
	{eSETUP_NTSC, "NTSC"}
};

#define DISPLAY_CFG_NB_VCROUTPUT 1

tDISPLAY_CFG_List DISPLAY_CFG_VCROutputList[DISPLAY_CFG_NB_VCROUTPUT] =
{
	{eSETUP_CVBS, "CVBS"}
};

#define DISPLAY_CFG_NB_RESOLUTION 4
uint8_t DISPLAY_CFG_TVHDDefaultIndex=0;

tDISPLAY_CFG_List DISPLAY_CFG_TVHDResolution[DISPLAY_CFG_NB_RESOLUTION] =
{
	{eSETUP_TVHD_RESOLUTION_AUTO, "Auto"},
	{eSETUP_TVHD_RESOLUTION_1080I, "1080i"},
	{eSETUP_TVHD_RESOLUTION_720P, "720p"},
	{eSETUP_TVHD_RESOLUTION_576P, "576p"}
/*,
	{eSETUP_TVHD_RESOLUTION_576I, "576i"},
	{eSETUP_TVHD_RESOLUTION_576P, "576p"}*/
};

#define DISPLAY_CFG_NB_VIDEOHDSTANDARD 27

tDISPLAY_CFG_List DISPLAY_CFG_VideoHDStandardList[DISPLAY_CFG_NB_VIDEOHDSTANDARD] =
{
	{eSETUP_VIDEO_HD_STD_640_480_P_60, "480p 640x480 60Hz"},
	{eSETUP_VIDEO_HD_STD_720_480_P_5994, "480p 720x480 59.94Hz"},
	{eSETUP_VIDEO_HD_STD_720_480_P_60, "480p 720x480 60Hz"},
	{eSETUP_VIDEO_HD_STD_720_480_I_5994, "480i 720x480 59.94Hz"},
	{eSETUP_VIDEO_HD_STD_720_480_I_60, "480i 720x480 60Hz"},
	{eSETUP_VIDEO_HD_STD_720_483_P_5994, "483p 720x483 59.94Hz"},
	{eSETUP_VIDEO_HD_STD_720_576_P_50, "576p 720x576 50Hz"},
	{eSETUP_VIDEO_HD_STD_720_576_I_50, "576i 720x576 50Hz"},
	{eSETUP_VIDEO_HD_STD_1280_720_P_2398, "720p 1280x720 23.98Hz"},
	{eSETUP_VIDEO_HD_STD_1280_720_P_24, "720p 1280x720 24Hz"},
	{eSETUP_VIDEO_HD_STD_1280_720_P_25, "720p 1280x720 25Hz"},
	{eSETUP_VIDEO_HD_STD_1280_720_P_2997, "720p 1280x720 29.97Hz"},
	{eSETUP_VIDEO_HD_STD_1280_720_P_30, "720p 1280x720 30Hz"},
	{eSETUP_VIDEO_HD_STD_1280_720_P_50, "720p 1280x720 50Hz"},
	{eSETUP_VIDEO_HD_STD_1280_720_P_5994, "720p 1280x720 29.94Hz"},
	{eSETUP_VIDEO_HD_STD_1280_720_P_60, "720p 1280x720 60Hz"},
	{eSETUP_VIDEO_HD_STD_1920_1080_P_2398, "1080p 1920x1080 23.98Hz"},
	{eSETUP_VIDEO_HD_STD_1920_1080_P_24, "1080p 1920x1080 24Hz"},
	{eSETUP_VIDEO_HD_STD_1920_1080_P_25, "1080p 1920x1080 25Hz"},
	{eSETUP_VIDEO_HD_STD_1920_1080_P_2997, "1080p 1920x1080 29.97Hz"},
	{eSETUP_VIDEO_HD_STD_1920_1080_P_30, "1080p 1920x1080 30Hz"},
	{eSETUP_VIDEO_HD_STD_1920_1080_P_50, "1080p 1920x1080 50Hz"},
	{eSETUP_VIDEO_HD_STD_1920_1080_P_5994, "1080p 1920x1080 59.94Hz"},
	{eSETUP_VIDEO_HD_STD_1920_1080_P_60, "1080p 1920x1080 60Hz"},
	{eSETUP_VIDEO_HD_STD_1920_1080_I_50, "1080i 1920x1080 50Hz"},
	{eSETUP_VIDEO_HD_STD_1920_1080_I_5994, "1080i 1920x1080 59.94Hz"},
	{eSETUP_VIDEO_HD_STD_1920_1080_I_60, "1080i 1920x1080 60Hz"}
};

#define DISPLAY_CFG_NB_RFMOD_BAND 2

tDISPLAY_CFG_List DISPLAY_CFG_RFModBandList[DISPLAY_CFG_NB_RFMOD_BAND] =
{
	{eSETUP_RFMODE_BAND_UHF, "UHF"},
	{eSETUP_RFMODE_BAND_VHF, "VHF"}
};

#define DISPLAY_CFG_NB_RFMOD_STANDARD 4

tDISPLAY_CFG_List DISPLAY_CFG_RFModStandardList[DISPLAY_CFG_NB_RFMOD_STANDARD] =
{
	{eSETUP_PAL, "PAL-B"},
	{eSETUP_SECAM, "SECAM"},
	{eSETUP_NTSC, "NTSC"},
	{eSETUP_PALN, "PAL-N"}
};

/*===========================================================================
 *
 * DISPLAY_CFG_GetTVFormatList
 *
 * Parameters:
 *      none
 *
 * Description:
 *      Return
 *
 * Returns:
 *      Return
 *
 *===========================================================================*/
tDISPLAY_CFG_List * DISPLAY_CFG_GetTVFormatList(uint16_t *nbItems)
{
	*nbItems = sizeof(DISPLAY_CFG_TVFormatList)/sizeof(tDISPLAY_CFG_List);
	return DISPLAY_CFG_TVFormatList;
}

/*===========================================================================
 *
 * DISPLAY_CFG_GetTVFormatList_Aux
 *
 * Parameters:
 *      none
 *
 * Description:
 *      Return
 *
 * Returns:
 *      Return
 *
 *===========================================================================*/
tDISPLAY_CFG_List * DISPLAY_CFG_GetTVFormatList_Aux(uint16_t *nbItems)
{
	*nbItems = DISPLAY_CFG_NB_TVFORMAT_AUX;
	return DISPLAY_CFG_TVFormatListAux;
}

/*===========================================================================
 *
 * DISPLAY_CFG_GetTVOutputList
 *
 * Parameters:
 *      none
 *
 * Description:
 *      Return
 *
 * Returns:
 *      Return
 *
 *===========================================================================*/
tDISPLAY_CFG_List * DISPLAY_CFG_GetTVOutputList(uint16_t *nbItems)
{
	*nbItems = DISPLAY_CFG_NB_TVOUTPUT;
	return DISPLAY_CFG_TVOutputList;
}

/*===========================================================================
 *
 * DISPLAY_CFG_GetTVModeList
 *
 * Parameters:
 *      none
 *
 * Description:
 *      Return
 *
 * Returns:
 *      Return
 *
 *===========================================================================*/
tDISPLAY_CFG_List * DISPLAY_CFG_GetTVModeList(uint16_t *nbItems)
{
	*nbItems = DISPLAY_CFG_NB_TVMODE;
	return DISPLAY_CFG_TVModeList;
}

/*===========================================================================
 *
 * DISPLAY_CFG_GetVCROutputList
 *
 * Parameters:
 *      none
 *
 * Description:
 *      Return
 *
 * Returns:
 *      Return
 *
 *===========================================================================*/
tDISPLAY_CFG_List * DISPLAY_CFG_GetVCROutputList(uint16_t *nbItems)
{
	*nbItems = DISPLAY_CFG_NB_VCROUTPUT;
	return DISPLAY_CFG_VCROutputList;
}

/*===========================================================================
 *
 * DISPLAY_CFG_GetTVHDResolutionList
 *
 * Parameters:
 *      none
 *
 * Description:
 *      Return
 *
 * Returns:
 *      Return
 *
 *===========================================================================*/
tDISPLAY_CFG_List * DISPLAY_CFG_GetTVHDResolutionList(uint16_t *nbItems)
{
	*nbItems = DISPLAY_CFG_NB_RESOLUTION;
	return DISPLAY_CFG_TVHDResolution;
}

/*===========================================================================
 *
 * DISPLAY_CFG_GetVideoHDStandardList
 *
 * Parameters:
 *      none
 *
 * Description:
 *      Return
 *
 * Returns:
 *      Return
 *
 *===========================================================================*/
tDISPLAY_CFG_List * DISPLAY_CFG_GetVideoHDStandardList(uint16_t *nbItems)
{
	*nbItems = DISPLAY_CFG_NB_VIDEOHDSTANDARD;
	return DISPLAY_CFG_VideoHDStandardList;
}
/*===========================================================================
 *
 * DISPLAY_CFG_GetRFModBandList
 *
 * Parameters:
 *      none
 *
 * Description:
 *      Return
 *
 * Returns:
 *      Return
 *
 *===========================================================================*/
tDISPLAY_CFG_List * DISPLAY_CFG_GetRFModBandList(uint16_t *nbItems)
{
	*nbItems = DISPLAY_CFG_NB_RFMOD_BAND;
	return DISPLAY_CFG_RFModBandList;
}

/*===========================================================================
 *
 * DISPLAY_CFG_GetRFModStandardList
 *
 * Parameters:
 *      none
 *
 * Description:
 *      Return
 *
 * Returns:
 *      Return
 *
 *===========================================================================*/
tDISPLAY_CFG_List * DISPLAY_CFG_GetRFModStandardList(uint16_t *nbItems)
{
	*nbItems = DISPLAY_CFG_NB_RFMOD_STANDARD;
	return DISPLAY_CFG_RFModStandardList;
}

/*===========================================================================
 *
 * DISPLAY_CFG_GetDefaultTVFormat
 *
 * Parameters:
 *      none
 *
 * Description:
 *      Return
 *
 * Returns:
 *      Return
 *
 *===========================================================================*/
cmSetup_VideoAspectRatio_t 	DISPLAY_CFG_GetDefaultTVFormat(void)
{
	return eSETUP_ASPECT_RATIO_16_9;
}
/*===========================================================================
 *
 * DISPLAY_CFG_GetDefaultTVOutput
 *
 * Parameters:
 *      none
 *
 * Description:
 *      Return
 *
 * Returns:
 *      Return
 *
 *===========================================================================*/
cmSetup_TvOutputSignal_t 		DISPLAY_CFG_GetDefaultTVOutput(void)
{
	return eSETUP_CVBS;
}
/*===========================================================================
 *
 * DISPLAY_CFG_GetDefaultTVMode
 *
 * Parameters:
 *      none
 *
 * Description:
 *      Return
 *
 * Returns:
 *      Return
 *
 *===========================================================================*/
cmSetup_VideoEncodingMode_t 	DISPLAY_CFG_GetDefaultTVMode(void)
{
	return eSETUP_PAL;
}
/*===========================================================================
 *
 * DISPLAY_CFG_GetDefaultVCROutput
 *
 * Parameters:
 *      none
 *
 * Description:
 *      Return
 *
 * Returns:
 *      Return
 *
 *===========================================================================*/
cmSetup_TvOutputSignal_t		DISPLAY_CFG_GetDefaultVCROutput(void)
{
	return eSETUP_CVBS;
}

/*===========================================================================
 *
 * DISPLAY_CFG_SetTVHDefaultResolution
 *
 * Parameters:
 *      none
 *
 * Description:
 *      Return
 *
 * Returns:
 *      Return
 *
 *===========================================================================*/
void DISPLAY_CFG_SetTVHDefaultResolution(uint8_t Index)
{
    DISPLAY_CFG_TVHDDefaultIndex = Index;
	return;
}

/*===========================================================================
 *
 * DISPLAY_CFG_GetDefaultTVHDResolution
 *
 * Parameters:
 *      none
 *
 * Description:
 *      Return
 *
 * Returns:
 *      Return
 *
 *===========================================================================*/
cmSetup_TVHDResolution_t		DISPLAY_CFG_GetDefaultTVHDResolution(void)
{
	return DISPLAY_CFG_TVHDResolution[DISPLAY_CFG_TVHDDefaultIndex].value;
}

/*===========================================================================
 *
 * DISPLAY_CFG_GetDefaultTVHDStatus
 *
 * Parameters:
 *      none
 *
 * Description:
 *      Return
 *
 * Returns:
 *      Return
 *
 *===========================================================================*/
bool                            DISPLAY_CFG_GetDefaultTVHDStatus(void)
{
    /* For SAF_DTT, default is Slave Mode -> No HD output connected by default */
    /* BUT for test and development purpose, keep this status to TRUE in order to not be disturbed */
    /* RefDesign contain a HW issue that cause HDMI detection problem */
    /* This issue is fixed on MTC manufacturer board */
    return TRUE; /*FALSE;*/
}

/*===========================================================================
 *
 * DISPLAY_CFG_GetDefaultVolume
 *
 * Parameters:
 *      none
 *
 * Description:
 *      Return
 *
 * Returns:
 *      Return
 *
 *===========================================================================*/
uint32_t	DISPLAY_CFG_GetDefaultVolume(void)
{
	return kSETUP_VOLUME_MAX - 2;
}

cmSetup_RFModBand_t 			DISPLAY_CFG_GetRFModDefaultBand(void)
{
	return eSETUP_RFMODE_BAND_UHF;
}

uint32_t						DISPLAY_CFG_GetRFModDefaultChannel(void)
{
	return 63;
}

cmSetup_VideoEncodingMode_t		DISPLAY_CFG_GetRFModDefaultStandard(void)
{
	return eSETUP_PAL;
}

/*===========================================================================
 *
 * DISPLAY_CFG_GetDefaultScreenSaverTimeout
 *
 * Parameters:
 *      none
 *
 * Description: 	Init default screen saver timeout value to 5 min
 *      Return
 *
 * Returns:
 *      Return
 *
 *===========================================================================*/
cmSetup_ScreenSaverTimeout_t	DISPLAY_CFG_GetDefaultScreenSaverTimeout(void)
{
	return eSETUP_SCREENSAVER_OFF;
}


