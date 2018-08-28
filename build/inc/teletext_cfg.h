/******************************************************************************
 *                    COPYRIGHT 2006 IWEDIA TECHNOLOGIES                      *
 ******************************************************************************
 *
 * MODULE NAME: TELETEXT CFG
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/product/comedia/branches/idecode_hd/build/inc/teletext_cfg.h $
 *            $Rev: 244 $
 *            $Date: 2011-08-18 18:33:39 +0200 (Thu, 18 Aug 2011) $
 *
 * PUBLIC
 *
 * DESCRIPTION: teletext configuration header
 *
 *****************************************************************************/

#ifndef _TELETEXT_CFG_H_
#define _TELETEXT_CFG_H_

/*******************************************************/
/*              Includes                               */
/*******************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#include "comedia_keys.h"

/*******************************************************/
/*              Defines                                */
/*******************************************************/


/*******************************************************/
/*              Macros                                 */
/*******************************************************/


/*******************************************************/
/*              Typedefs                               */
/*******************************************************/

typedef enum
{
    eTTXT_ZERO = 0x00,	/* number key stroke */
    eTTXT_ONE,
    eTTXT_TWO,
    eTTXT_THREE,
    eTTXT_FOUR,
    eTTXT_FIVE,
    eTTXT_SIX,
    eTTXT_SEVEN,
    eTTXT_EIGHT,
    eTTXT_NINE,

    eTTXT_PAGE_DOWN,	/* to get previous page  */
    eTTXT_PAGE_UP,		/* to get next page      */
    eTTXT_RED,			/* special red    button */
    eTTXT_GREEN,		        /* special green  button */
    eTTXT_YELLOW,		/* special yellow button */
    eTTXT_BLUE,			/* special blue   button */
	eTTXT_ARROW_UP,		/* go to next     page number on screen */
	eTTXT_ARROW_DOWN,	/* go to previous page number on screen */
	eTTXT_OK,			/* select page selected on screen */

	eTTXT_ZOOM,
	eTTXT_CONCEALREVEAL,
	eTTXT_SUBPAGE_PAUSE,
	eTTXT_SUBPAGE_DIRECTACCESS,
	eTTXT_TINDEX,
	eTTXT_CANCEL,

	eTTXT_MAX_KEY
} tTTXT_Key;

/*******************************************************/
/*              Variables Declarations (IMPORT)        */
/*******************************************************/

/* teletext system key list */
IMPORT const tSYS_KeyEvtCode TTXT_CFG_AllKeyCodesArray[];

IMPORT const uint16_t TTXT_CFG_AllKeyCount;

/*******************************************************/
/*              Functions Declarations                 */
/*******************************************************/


/*===========================================================================
 *
 * TTXT_CFG_IsTransparencyToggleEnabled
 *
 * Parameters:
 *      None
 *
 * Description:
 *      return TRUE if the transparency toggle feature is enabled
 *      (e.g. there is a RCU key dedicated to the transparency toggle).
 *
 * Returns:
 *
 *
 *===========================================================================*/
bool TTXT_CFG_IsTransparencyToggleEnabled(void);


/*===========================================================================
 *
 * TTXT_CFG_GetFontTTXTFileName
 *
 * Parameters:
 *      None
 *
 * Description:
 *      return color index for a specific tTTXT_Color value.
 *
 * Returns:
 *
 *
 *===========================================================================*/
void TTXT_CFG_GetFontTTXTFileName(char **Name);


/*===========================================================================
 *
 * TTXT_CFG_GetTTXTKeyCode
 *
 * Parameters:
 *      SYS Key code
 *
 * Description:
 *      return teletext feature associated to system key code.
 *
 * Returns:
 *
 *
 *===========================================================================*/
tTTXT_Key TTXT_CFG_GetTTXTKeyCode(tSYS_KeyEvtCode keyCode);


/*===========================================================================
 *
 * TTXT_CFG_GetMemorySize
 *
 * Parameters:
 *
 * Description:
 *
 * Returns:
 *
 *
 *===========================================================================*/
uint32_t TTXT_CFG_GetMemorySize(void);

/*===========================================================================
 *
 * TTXT_CFG_GetPalette
 *
 * Parameters:
 *
 * Description:
 *
 * Returns:
 *
 *
 *===========================================================================*/
uint32_t TTXT_CFG_GetPalette(void* pColorArray);

/*===========================================================================
 *
 * TTXT_CFG_IsExternalRegionUsed
 *
 * Parameters:
 *
 * Description:
 *
 * Returns:
 *
 *
 *===========================================================================*/
bool TTXT_CFG_IsExternalRegionUsed(void);



#ifdef __cplusplus
}
#endif

#endif /* _TELETEXT_CFG_H_ */
