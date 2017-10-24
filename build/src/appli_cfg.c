/********************************************************************************
 *                       COPYRIGHT 2005 IWEDIA                                  *
 ********************************************************************************
 *
 * MODULE NAME: APPLI CFG
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/product/comedia/branches/idecode_hd/build/src/appli_cfg.c $
 *            $Rev: 244 $
 *            $Date: 2011-08-18 18:33:39 +0200 (Thu, 18 Aug 2011) $
 *
 *
 *******************************************************************************/

/********************************************************/
/*                Includes                              */
/********************************************************/
#include <string.h>

#include "crules.h"
#include "appli_cfg.h"





/********************************************************/
/*              Defines                                 */
/********************************************************/

/********************************************************/
/*              Local Functions Declarations (LOCAL)    */
/*******************************************************/

/********************************************************/
/*              Functions Definitions (LOCAL/GLOBAL)    */
/********************************************************/

/*===========================================================================
 *
 * APPLI_CFG_GetAntiCloningStatus
 *
 * Parameters:
 *      none
 *
 * Description:
 *      This function is used for software anti-cloning purpose.
 *      A register is tested to guaranty that the software can only run on specific
 *      sale types.
 *
 * Returns:
 *      true if the anti-cloning check is disabled or if the anticloning check is OK
 *      false if the anti-cloning check failed
 *
 *===========================================================================*/
bool APPLI_CFG_GetAntiCloningStatus(void)
{
    bool isAllowed = TRUE;
#ifdef ANTICLONING_ENABLED
    /* Not Yet Implemented */
    isAllowed = FALSE;
#endif /* ANTICLONING_ENABLED */
    return isAllowed;
}


/*===========================================================================
 *
 * APPLI_CFG_SystemUpgradeScreen
 *
 * Parameters:
 *      none
 *
 * Description: do you want the 'system upgrade' screen ?
 *
 *
 * Returns:
 *      APPLI_CFG_SystemUpgradeScreen_t
 *
 *===========================================================================*/
APPLI_CFG_SystemUpgradeScreen_t APPLI_CFG_SystemUpgradeScreen(void)
{
    APPLI_CFG_SystemUpgradeScreen_t value;

    value = eAPPLI_CFG_SSU_NONE;

#if defined(TRIG_DVBSSU) || defined(TRIG_DTG)
    value |= eAPPLI_CFG_SSU_OADSCAN;
    value |= eAPPLI_CFG_SSU_OADTUNE;
#endif

#ifdef TRIG_USB
    value |= eAPPLI_CFG_SSU_USB;
#endif
    
    return value;
}

#ifdef PRODUCT_USE_USB
/*===========================================================================
 *
 * APPLI_CFG_MediaViewerScreen
 *
 * Parameters:
 *      none
 *
 * Description:
 *
 *
 * Returns:
 *      boolean
 *
 *===========================================================================*/
bool APPLI_CFG_MediaViewerScreen(void)
{
#ifdef PRODUCT_MEDIA_VIEWER
    return TRUE;
#else
    return FALSE;
#endif
}
#endif /* PRODUCT_USE_USB */

/*===========================================================================
 *
 * APPLI_CFG_IsVideoDisplayedWithMenu
 *
 * Parameters:
 *      none
 *
 * Description: do you want video with menu ?
 *
 *
 * Returns:
 *      boolean
 *
 *===========================================================================*/
bool APPLI_CFG_IsVideoDisplayedWithMenu(void)
{
#if defined(UI_OITV)
    return FALSE;
#else
	return TRUE;
#endif
}

/*===========================================================================
 *
 * APPLI_CFG_CAScreen
 *
 * Parameters:
 *      none
 *
 * Description: do you want the CA screen ?
 *
 *
 * Returns:
 *      boolean
 *
 *===========================================================================*/
bool APPLI_CFG_CAScreen(void)
{
#if defined(PRODUCT_USE_CA) || defined(PRODUCT_USE_CI)
    return TRUE;
#else
    return FALSE;
#endif
}

/*===========================================================================
 *
 * APPLI_CFG_ADScreen
 *
 * Parameters:
 *      none
 *
 * Description: do you want the Audio Description screen ?
 *      
 *
 * Returns:
 *      boolean
 *
 *===========================================================================*/
bool APPLI_CFG_ADScreen(void)
{
    return FALSE;
}

/*===========================================================================
 *
 * APPLI_CFG_RFScreen
 *
 * Parameters:
 *      none
 *
 * Description: do you want the RF Mod screen ?
 *      
 *
 * Returns:
 *      boolean
 *
 *===========================================================================*/
bool APPLI_CFG_RFScreen(void)
{
    return FALSE;
}

/*===========================================================================
 *
 * APPLI_CFG_ScrambledPopup
 *
 * Parameters:
 *      none
 *
 * Description: Display a popup if the service is scrambled.
 *
 *
 * Returns:
 *      boolean
 *
 *===========================================================================*/
bool APPLI_CFG_ScrambledPopup(void)
{
#if defined(PRODUCT_USE_CA)
    return FALSE;
#else
    /* enable scrambled popup in CI case (used when module is extracted) */
    return TRUE;
#endif
}


/*===========================================================================
 *
 * APPLI_CFG_BannerScreen_SignalQualityAndLevel
 *
 * Parameters:
 *      none
 *
 * Description: do you want the signal quality and level in banner ?
 *
 *
 * Returns:
 *      boolean
 *
 *===========================================================================*/
bool APPLI_CFG_BannerScreen_SignalQualityAndLevel(void)
{
    return TRUE;
}


/*===========================================================================
 *
 * APPLI_CFG_UseSubtitlesToggleOnOff
 *
 * Parameters:
 *      none
 *
 * Description: do you want to use the "Subtitles Toggle On/Off" functionnality
 *				or the "Subtitles Language Choice Popup" one
 *				when the SUB key is pressed
 *
 * Returns:
 *      boolean
 *
 *===========================================================================*/
bool APPLI_CFG_UseSubtitlesToggleOnOff(void)
{
#ifdef UK_SPECIFIC
    return TRUE;
#else
    return FALSE;
#endif
}

/*===========================================================================
 *
 * APPLI_CFG_DisplayMhegSubtitlePopup
 *
 * Parameters:
 *      none
 *
 * Description: MHEG and subtitle can not be active at the same time. When
 *              subtitle is active and the user presses TEXT or RED to launch
 *              MHEG appli, we can either display a popup to warn the user
 *              or stop subtitle and resume MHEG.
 *
 * Returns:
 *      boolean
 *
 *===========================================================================*/
bool APPLI_CFG_DisplayMhegSubtitlePopup(void)
{
    return FALSE;
}

/*===========================================================================
 *
 * APPLI_CFG_CanInterruptFTIWithMenuKey
 *
 * Parameters:
 *      none
 *
 * Description: can the user interrupt the FTI by pressing Menu Key
 *
 *
 * Returns:
 *      boolean
 *
 *===========================================================================*/
bool APPLI_CFG_CanInterruptFTIWithMenuKey(void)
{
    return FALSE;
}

/*===========================================================================
 *
 * APPLI_CFG_CanInterruptFTIWithMenuKeyWhenFailed
 *
 * Parameters:
 *      none
 *
 * Description: can the user interrupt the FTI by pressing Menu Key if FTI failed
 *
 *
 * Returns:
 *      boolean
 *
 *===========================================================================*/
bool APPLI_CFG_CanInterruptFTIWithMenuKeyWhenFailed(void)
{
    return TRUE;
}

#ifdef PACKAGE_PVR
/*===========================================================================
 *
 * DISPLAY_CFG_PVR_DisplayPopupWhenRecError
 *
 * Parameters:
 *      none
 *
 * Description: Display a popup when a record can not be perform
 *      Return
 *
 * Returns:
 *      Return boolean
 *
 *===========================================================================*/
bool 	APPLI_CFG_PVR_DisplayPopupWhenRecError(void)
{
	return TRUE;
}
#endif /* PACKAGE_PVR */

/*===========================================================================
 *
 * APPLI_CFG_StartWhenMaturityRatingReceived
 *
 * Parameters:
 *      none
 *
 * Description: do you want to start service only when the maturity rating
 *              (from EITp is received). If no EITp is received after a timeout,
 *              the service is started anyway (cf. call to BAN_CheckMaturityRating
 *              in screen_5000.xml) if APPLI_CFG_StartWhenMaturityRatingReceivedWithTimeout is set to TRUE.
 *              This process is active only if the user has set a maturity rating.
 *
 *
 * Returns:
 *      boolean
 *
 *===========================================================================*/
bool APPLI_CFG_StartWhenMaturityRatingReceived(void)
{
    return TRUE;
}

/*===========================================================================
 *
 * APPLI_CFG_StartWhenMaturityRatingReceived
 *
 * Parameters:
 *      none
 *
 * Description: If set to TRUE, if no EITp is received after a timeout,
 *              the service is started anyway (cf. call to BAN_CheckMaturityRating
 *              in screen_5000.xml).
 *              This process is active only if the user has set a maturity rating.
 *
 *
 * Returns:
 *      boolean
 *
 *===========================================================================*/
bool APPLI_CFG_StartWhenMaturityRatingReceivedWithTimeout(void)
{
    return TRUE;
}

#if defined(PRODUCT_USE_CA) || defined(PRODUCT_USE_CI)
/*===========================================================================
 *
 * APPLI_CFG_MMIDisplayListInLabel
 *
 * Parameters:
 *      none
 *
 * Description: Display CA MMI list in a label with scroolbar if true.
 *              Else display it in a list
 *
 *
 * Returns:
 *      boolean
 *
 *===========================================================================*/
bool APPLI_CFG_MMIDisplayListInLabel(void)
{
    return FALSE;
}
#endif

#ifdef PRODUCT_USE_USB
/*===========================================================================
 *
 * APPLI_CFG_Does_AnUsbDeviceInsertion_InterruptAppli
 *
 * Parameters:
 *      none
 *
 * Description: Display a popup when an usb device insertion is detected
 *      Return
 *
 * Returns:
 *      Return boolean
 *
 *===========================================================================*/
bool APPLI_CFG_Does_AnUsbDeviceInsertion_InterruptAppli(void)
{
	return TRUE;
}
#endif /* PRODUCT_USE_USB */


/*===========================================================================
 *
 * APPLI_CFG_BannerScreen_Do_LeftRightKeys_Manage_Volume
 *
 * Parameters:
 *      none
 *
 * Description:
 *
 * Returns:
 *      Return boolean
 *
 *===========================================================================*/
bool APPLI_CFG_BannerScreen_Do_LeftRightKeys_Manage_Volume(void)
{
    return FALSE;
}
/*===========================================================================
 *
 * APPLI_CFG_GetUINativeResolution
 *
 * Parameters:
 *      none
 *
 * Description:
 *
 * Returns:
 *      Return boolean
 *
 *===========================================================================*/
bool APPLI_CFG_GetUINativeResolution(uint16_t *pWidth, uint16_t *pHeight)
{
    bool result = FALSE;
	if ((pWidth != NULL) && (pHeight != NULL))
	{
		result = TRUE;
#ifdef UI_V4
#ifdef UI_SIZE_1080
		*pWidth = 1920;
		*pHeight = 1080;
    #elif defined UI_SIZE_720
        *pWidth = 1280;
        *pHeight = 720;
#else
		*pWidth = 720;
		*pHeight = 576;
#endif
#else
		*pWidth = 720;
		*pHeight = 576;
#endif
	}
	return result;
}

/*===========================================================================
 *
 * APPLI_CFG_IsServiceConsideredScrambled
 *
 * Parameters:
 *      type indicates the type of the service (0: TV, 1: Radio, 2: Other).
 *      video_scrambled indicates whether the PES stream is marked scrambled.
 *      audio_scrambled indicates whether the PES stream is marked scrambled.
 *
 * Description:
 *      This function determines if a service should be considered scrambled
 *      or free based on the scramble state of its video and audio feeds.
 *      The result has an impact on the CA pop-ups: those do appear on
 *      scrambled channels if required and never do on free-to-air services.
 *
 * Returns:
 *      A boolean indicating whether the service should be considered
 *      scrambled or not.
 *
 *===========================================================================*/

bool APPLI_CFG_IsServiceConsideredScrambled( int type, bool video_scrambled, bool audio_scrambled )
{
    switch( type )
    {
        case 0:
            return video_scrambled || audio_scrambled;

        case 1:
            return video_scrambled || audio_scrambled;

        default:
            return video_scrambled || audio_scrambled;
    }
}

