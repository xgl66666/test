/******************************************************************************
 *                    COPYRIGHT 2006 IWEDIA TECHNOLOGIES                      *
 ******************************************************************************
 *
 * MODULE NAME: TELETEXT CFG
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/product/comedia/branches/idecode_hd/build/src/teletext_cfg.c $
 *            $Rev: 244 $
 *            $Date: 2011-08-18 18:33:39 +0200 (Thu, 18 Aug 2011) $
 *
 * PUBLIC
 *
 * DESCRIPTION: teletext configuration
 *
 *****************************************************************************/

/********************************************************/
/*                Includes                              */
/********************************************************/
#include "crules.h"
#include "tkel.h"
#include "teletext_cfg.h"
#include "tdal_gfx.h"
#include "comedia_keys.h"

/********************************************************/
/*              Defines                                 */
/********************************************************/

/********************************************************/
/*              Macros                                  */
/********************************************************/

/********************************************************/
/*              Typedefs                                */
/********************************************************/

/********************************************************/
/*              Global Variables (GLOBAL/IMPORT)        */
/********************************************************/

/* teletext system key list */
const tSYS_KeyEvtCode TTXT_CFG_AllKeyCodesArray[]=
{
    eSYS_KEY_DIGIT0,
    eSYS_KEY_DIGIT1,
    eSYS_KEY_DIGIT2,
    eSYS_KEY_DIGIT3,
    eSYS_KEY_DIGIT4,
    eSYS_KEY_DIGIT5,
    eSYS_KEY_DIGIT6,
    eSYS_KEY_DIGIT7,
    eSYS_KEY_DIGIT8,
    eSYS_KEY_DIGIT9,
    eSYS_UP_ARROW_KEY,
    eSYS_DOWN_ARROW_KEY,
    eSYS_SELECT_KEY,
    eSYS_RED_KEY,
    eSYS_GREEN_KEY,
    eSYS_YELLOW_KEY,
    eSYS_BLUE_KEY,
    eSYS_PROGRAM_NEXT_KEY,
    eSYS_PROGRAM_PREV_KEY,
    eSYS_MENU_KEY,
    eSYS_EXIT_KEY,
    eSYS_FAVORITE_KEY,
    eSYS_TELETEXT_MIX_KEY,
    eSYS_SUBTITLE_KEY,
    eSYS_AUDIO_KEY,
    eSYS_INFO_KEY,
    eSYS_TVFORMAT_KEY,
    eSYS_EPG_KEY
};

const uint16_t TTXT_CFG_AllKeyCount =
                    (sizeof(TTXT_CFG_AllKeyCodesArray)/sizeof(tSYS_KeyEvtCode));

/********************************************************/
/*              Local File Variables (LOCAL)            */
/********************************************************/
const char TTXTOSD_CFG_FontFileName[] = {"ttxtfont.tzp"};



/********************************************************/
/*              Local Functions Declarations (LOCAL)    */
/*******************************************************/

/********************************************************/
/*              Functions Definitions (LOCAL/GLOBAL)    */
/********************************************************/

bool SYS_KDB_CFG_IsMappedOnPhysicalKey(tSYS_KeyEvtCode KeyCode);

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
bool TTXT_CFG_IsTransparencyToggleEnabled(void)
{
	bool result = FALSE;

	if(SYS_KDB_CFG_IsMappedOnPhysicalKey(eSYS_TELETEXT_MIX_KEY) == TRUE)
	{
		 /* the transparency toggle feature has its own key */
		result = TRUE;
	}

	return(result);
}

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
void TTXT_CFG_GetFontTTXTFileName(char **Name)
{
	if(Name != NULL)
	{
		*Name = (char *)TTXTOSD_CFG_FontFileName;
	}
	return;
}

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
tTTXT_Key TTXT_CFG_GetTTXTKeyCode(tSYS_KeyEvtCode keyCode)
{
	tTTXT_Key TTXT_KEY = eTTXT_MAX_KEY;

	switch(keyCode)
	{
		case eSYS_KEY_DIGIT0:
			TTXT_KEY = eTTXT_ZERO;
			break;
		case eSYS_KEY_DIGIT1:
			TTXT_KEY = eTTXT_ONE;
			break;
		case eSYS_KEY_DIGIT2:
			TTXT_KEY = eTTXT_TWO;
			break;
		case eSYS_KEY_DIGIT3:
			TTXT_KEY = eTTXT_THREE;
			break;
		case eSYS_KEY_DIGIT4:
			TTXT_KEY = eTTXT_FOUR;
			break;
		case eSYS_KEY_DIGIT5:
			TTXT_KEY = eTTXT_FIVE;
			break;
		case eSYS_KEY_DIGIT6:
			TTXT_KEY = eTTXT_SIX;
			break;
		case eSYS_KEY_DIGIT7:
			TTXT_KEY = eTTXT_SEVEN;
			break;
		case eSYS_KEY_DIGIT8:
			TTXT_KEY = eTTXT_EIGHT;
			break;
		case eSYS_KEY_DIGIT9:
			TTXT_KEY = eTTXT_NINE;
			break;
		case eSYS_UP_ARROW_KEY:
			TTXT_KEY = eTTXT_ARROW_UP;
			break;
		case eSYS_DOWN_ARROW_KEY:
			TTXT_KEY = eTTXT_ARROW_DOWN;
			break;
		case eSYS_SELECT_KEY:
			TTXT_KEY = eTTXT_OK;
			break;
		case eSYS_BLUE_KEY:
			TTXT_KEY = eTTXT_BLUE;
			break;
		case eSYS_YELLOW_KEY:
			TTXT_KEY = eTTXT_YELLOW;
			break;
		case eSYS_GREEN_KEY:
			TTXT_KEY = eTTXT_GREEN;
			break;
		case eSYS_RED_KEY:
			TTXT_KEY = eTTXT_RED;
			break;
		case eSYS_PROGRAM_NEXT_KEY:
			TTXT_KEY = eTTXT_ZOOM;
			break;
		case eSYS_PROGRAM_PREV_KEY :
			TTXT_KEY = eTTXT_SUBPAGE_PAUSE;
			break;
		case eSYS_MENU_KEY:
			TTXT_KEY = eTTXT_CONCEALREVEAL;
			break;
		case eSYS_EXIT_KEY:
			TTXT_KEY = eTTXT_SUBPAGE_DIRECTACCESS;
			break;
		case eSYS_FAVORITE_KEY:
			TTXT_KEY = eTTXT_TINDEX;
			break;
		default:
			break;
	}


	return TTXT_KEY;
}

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
uint32_t TTXT_CFG_GetMemorySize(void)
{
    return (300*1024);
}


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
uint32_t TTXT_CFG_GetPalette(void* pColorArray)
{
	int 				i;
	tTDAL_GFX_Color*	palette = (tTDAL_GFX_Color*)pColorArray;

	if(palette != NULL)
	{
		/* Initialize palette */
		for(i = 0 ; i < 256 ; i++)
		{
			palette[i].ARGB8888.R 	= 0x00;
			palette[i].ARGB8888.G 	= 0x00;
			palette[i].ARGB8888.B 	= 0x00;
			palette[i].ARGB8888.alpha = 0x00;
		}

		/* black */
		palette[1].ARGB8888.R = 0x00;
		palette[1].ARGB8888.G = 0x00;
		palette[1].ARGB8888.B = 0x00;
		palette[1].ARGB8888.alpha = 0xFF;

		/* red */
		palette[2].ARGB8888.R = 0xFF;
		palette[2].ARGB8888.G = 0x00;
		palette[2].ARGB8888.B = 0x00;
		palette[2].ARGB8888.alpha = 0xFF;

		/* blue */
		palette[3].ARGB8888.R = 0x00;
		palette[3].ARGB8888.G = 0x00;
		palette[3].ARGB8888.B = 0xFF;
		palette[3].ARGB8888.alpha = 0xFF;

		/* green */
		palette[4].ARGB8888.R = 0x00;
		palette[4].ARGB8888.G = 0xFF;
		palette[4].ARGB8888.B = 0x00;
		palette[4].ARGB8888.alpha = 0xFF;

		/* yellow */
		palette[5].ARGB8888.R = 0xFF;
		palette[5].ARGB8888.G = 0xFF;
		palette[5].ARGB8888.B = 0x00;
		palette[5].ARGB8888.alpha = 0xFF;

		/* cyan */
		palette[6].ARGB8888.R = 0x00;
		palette[6].ARGB8888.G = 0xFF;
		palette[6].ARGB8888.B = 0xFF;
		palette[6].ARGB8888.alpha = 0xFF;

		/* magenta */
		palette[7].ARGB8888.R = 0xFF;
		palette[7].ARGB8888.G = 0x00;
		palette[7].ARGB8888.B = 0xFF;
		palette[7].ARGB8888.alpha = 0xFF;


		palette[8].ARGB8888.R = 0xFF;
		palette[8].ARGB8888.G = 0xFF;
		palette[8].ARGB8888.B = 0xFF;
		palette[8].ARGB8888.alpha = 0xFF;
	}
	return TRUE;
}

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
bool TTXT_CFG_IsExternalRegionUsed(void)
{
	return FALSE;
}



