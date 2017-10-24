/******************************************************************************
 *                           COPYRIGHT 2005 IWEDIA                            *
 ******************************************************************************
 *
 * MODULE NAME: ACCESSCTRL CFG 
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/product/comedia/branches/idecode_hd/build/inc/appli_cfg.h $
 *            $Rev: 244 $
 *            $Date: 2011-08-18 18:33:39 +0200 (Thu, 18 Aug 2011) $
 *
 * PUBLIC
 *
 * DESCRIPTION
 *
 *****************************************************************************/

#ifndef _APPLI_CFG_H_
#define _APPLI_CFG_H_

#ifdef __cplusplus
extern "C" {
#endif


/*******************************************************/
/*               typedef                               */
/*******************************************************/
typedef enum
{
    eAPPLI_CFG_SSU_NONE  = 0x0000,
    eAPPLI_CFG_SSU_OADSCAN  = 0x0001,
    eAPPLI_CFG_SSU_OADTUNE  = 0x0002,
    eAPPLI_CFG_SSU_USB  = 0x0004,
    eAPPLI_CFG_SSU_ALL  = 0xFFFF /* All */
} APPLI_CFG_SystemUpgradeScreen_t;


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
bool APPLI_CFG_GetAntiCloningStatus(void);


APPLI_CFG_SystemUpgradeScreen_t APPLI_CFG_SystemUpgradeScreen(void);

#ifdef PRODUCT_USE_USB
bool APPLI_CFG_Does_AnUsbDeviceInsertion_InterruptAppli(void);
bool APPLI_CFG_MediaViewerScreen(void);
#endif
bool APPLI_CFG_CAScreen(void);
bool APPLI_CFG_ADScreen(void);
bool APPLI_CFG_RFScreen(void);
bool APPLI_CFG_ScrambledPopup(void);
bool APPLI_CFG_BannerScreen_SignalQualityAndLevel(void);
bool APPLI_CFG_IsVideoDisplayedWithMenu(void);
bool APPLI_CFG_UseSubtitlesToggleOnOff(void);
bool APPLI_CFG_DisplayMhegSubtitlePopup(void);
bool APPLI_CFG_CanInterruptFTIWithMenuKey(void);
bool APPLI_CFG_CanInterruptFTIWithMenuKeyWhenFailed(void);
bool APPLI_CFG_PVR_DisplayPopupWhenRecError(void);

bool APPLI_CFG_StartWhenMaturityRatingReceived(void);
bool APPLI_CFG_StartWhenMaturityRatingReceivedWithTimeout(void);
bool APPLI_CFG_MMIDisplayListInLabel(void);

bool APPLI_CFG_BannerScreen_Do_LeftRightKeys_Manage_Volume(void);

bool APPLI_CFG_GetUINativeResolution(uint16_t *pWidth, uint16_t *pHeight);

bool APPLI_CFG_IsServiceConsideredScrambled( int, bool, bool );

#ifdef __cplusplus
}
#endif

#endif /* _APPLI_CFG_H_ */
