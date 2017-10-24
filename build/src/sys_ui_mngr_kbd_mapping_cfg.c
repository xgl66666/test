/******************************************************************************
 *                         COPYRIGHT 2005 IWEDIA TECHNOLOGIES                         *
 ******************************************************************************
 *
 * MODULE NAME: SYS
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/product/comedia/branches/idecode_hd/build/src/sys_ui_mngr_kbd_mapping_cfg.c $
 *            $Rev: 244 $
 *            $Date: 2011-08-18 18:33:39 +0200 (Thu, 18 Aug 2011) $
 *
 * DESCRIPTION
 *
 *****************************************************************************/

/********************************************************/
/*              Includes                                */
/********************************************************/
#include "sys_ui_mngr_kbd_mapping_cfg.h"
#include "sys_ui_mngr_kbd_mapping_cfg_p.h"

/********************************************************/
/*              Functions Definitions (LOCAL/GLOBAL)    */
/********************************************************/
typedef uint32_t tSYS_KeyEvtCodeMapping;   /* type -> tSYS_KBD_KeyCode */

tSYS_KeyEvtCode SYS_AllKeyEvtList[] =
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
    eSYS_MUTE_KEY,
    eSYS_POWER_KEY,
    eSYS_STANDBY_KEY,
    eSYS_RED_KEY,
    eSYS_GREEN_KEY,
    eSYS_YELLOW_KEY,
    eSYS_BLUE_KEY,
    eSYS_PAGE_PREV_KEY,
    eSYS_FAVORITE_KEY,
    eSYS_SUBTITLE_KEY,
    eSYS_AUDIO_KEY,
    eSYS_PAGE_NEXT_KEY,
    eSYS_EPG_KEY,
    eSYS_INFO_KEY,
    eSYS_TIMER_KEY,
    eSYS_MENU_KEY,
    eSYS_UP_ARROW_KEY,
    eSYS_EXIT_KEY,
    eSYS_LEFT_ARROW_KEY,
    eSYS_SELECT_KEY,
    eSYS_RIGHT_ARROW_KEY,
    eSYS_VOLUME_NEXT_KEY,
    eSYS_DOWN_ARROW_KEY,
    eSYS_PROGRAM_NEXT_KEY,
    eSYS_VOLUME_PREV_KEY,
    eSYS_PROGRAM_PREV_KEY,
    eSYS_TV_STB_KEY,
    eSYS_TV_RADIO_KEY,
    eSYS_PRF_PREV_KEY,
    eSYS_PRF_NEXT_KEY,
    eSYS_TELETEXT_KEY,
    eSYS_TELETEXT_MIX_KEY,
    eSYS_PAUSE_KEY,
    eSYS_SWAP_KEY,
    eSYS_LIST_KEY,
    eSYS_TVFORMAT_KEY,
    eSYS_OTHERAPP_KEY,
    eSYS_PIP_PAP_KEY,
    eSYS_PIP_KEY,
    eSYS_PAP_KEY,
    eSYS_RECORD_KEY,
    eSYS_STOP_KEY,
    eSYS_PLAY_KEY,
    eSYS_BACKWARD_KEY,
    eSYS_FORWARD_KEY,
    eSYS_AD_KEY,
    eSYS_BACK_KEY,
    eSYS_SKIP_BACKWARD_KEY,
    eSYS_SKIP_FORWARD_KEY,
    eSYS_HOTKEY1,
    eSYS_HOTKEY2,
    eSYS_HOTKEY3,
    eSYS_MEDIA_LIBRARY_LEY,
    eSYS_PVR_LIBRARY_KEY,
    eSYS_MAIL_KEY,
    eSYS_FACTORY_DEFAULT_KEY
};

const uint32_t SYS_AllKeyEvtListCount = sizeof(SYS_AllKeyEvtList) / sizeof(SYS_AllKeyEvtList[0]);


LOCAL tSYS_KeyEvtCodeMapping	SYS_KeyEvtCodeMapping[SYS_CODE_NUMBER] =
{
	kSYS_KEY_DIGIT0,         /* eSYS_KEY_DIGIT0       */
	kSYS_KEY_DIGIT1,         /* eSYS_KEY_DIGIT1       */
	kSYS_KEY_DIGIT2,         /* eSYS_KEY_DIGIT2       */
	kSYS_KEY_DIGIT3,         /* eSYS_KEY_DIGIT3       */
	kSYS_KEY_DIGIT4,         /* eSYS_KEY_DIGIT4       */
	kSYS_KEY_DIGIT5,         /* eSYS_KEY_DIGIT5       */
	kSYS_KEY_DIGIT6,         /* eSYS_KEY_DIGIT6       */
	kSYS_KEY_DIGIT7,         /* eSYS_KEY_DIGIT7       */
	kSYS_KEY_DIGIT8,         /* eSYS_KEY_DIGIT8       */
	kSYS_KEY_DIGIT9,         /* eSYS_KEY_DIGIT9       */
	kSYS_MUTE_KEY,           /* eSYS_MUTE_KEY         */
	kSYS_POWER_KEY,          /* eSYS_POWER_KEY        */
	kSYS_STANDBY_KEY,        /* eSYS_STANDBY_KEY      */
	kSYS_RED_KEY,            /* eSYS_RED_KEY          */
	kSYS_GREEN_KEY,          /* eSYS_GREEN_KEY        */
	kSYS_YELLOW_KEY,         /* eSYS_YELLOW_KEY       */
	kSYS_BLUE_KEY,           /* eSYS_BLUE_KEY         */
	kSYS_PAGE_PREV_KEY,      /* eSYS_PAGE_PREV_KEY    */
	kSYS_FAVORITE_KEY,       /* eSYS_FAVORITE_KEY     */
	kSYS_SUBTITLE_KEY,       /* eSYS_SUBTITLE_KEY     */
	kSYS_AUDIO_KEY,          /* eSYS_AUDIO_KEY        */
	kSYS_PAGE_NEXT_KEY,      /* eSYS_PAGE_NEXT_KEY    */
	kSYS_EPG_KEY,            /* eSYS_EPG_KEY          */
	kSYS_INFO_KEY,           /* eSYS_INFO_KEY         */
	kSYS_TIMER_KEY,          /* eSYS_TIMER_KEY        */
	kSYS_MENU_KEY,           /* eSYS_MENU_KEY         */
	kSYS_UP_ARROW_KEY,       /* eSYS_UP_ARROW_KEY     */
	kSYS_EXIT_KEY,           /* eSYS_EXIT_KEY         */
	kSYS_LEFT_ARROW_KEY,     /* eSYS_LEFT_ARROW_KEY   */
	kSYS_SELECT_KEY,         /* eSYS_SELECT_KEY       */
	kSYS_RIGHT_ARROW_KEY,    /* eSYS_RIGHT_ARROW_KEY  */
	kSYS_VOLUME_NEXT_KEY,    /* eSYS_VOLUME_NEXT_KEY  */
	kSYS_DOWN_ARROW_KEY,     /* eSYS_DOWN_ARROW_KEY   */
	kSYS_PROGRAM_NEXT_KEY,   /* eSYS_PROGRAM_NEXT_KEY */
	kSYS_VOLUME_PREV_KEY,    /* eSYS_VOLUME_PREV_KEY  */
	kSYS_PROGRAM_PREV_KEY,   /* eSYS_PROGRAM_PREV_KEY */
	kSYS_TV_STB_KEY,         /* eSYS_TV_STB_KEY       */
	kSYS_TV_RADIO_KEY,       /* eSYS_TV_RADIO_KEY     */
	kSYS_PRF_PREV_KEY,       /* eSYS_PRF_PREV_KEY     */
	kSYS_PRF_NEXT_KEY,       /* eSYS_PRF_NEXT_KEY     */
	kSYS_TELETEXT_KEY,       /* eSYS_TELETEXT_KEY     */
	kSYS_TELETEXT_MIX_KEY,   /* eSYS_TELETEXT_MIX_KEY */
	kSYS_PAUSE_KEY,          /* eSYS_PAUSE_KEY        */
	kSYS_SWAP_KEY,           /* eSYS_SWAP_KEY         */
	kSYS_LIST_KEY,           /* eSYS_LIST_KEY         */
	kSYS_TVFORMAT_KEY,       /* eSYS_TVFORMAT_KEY     */
    NO_PHYSICAL_CODE,        /* eSYS_OTHERAPP_KEY     */
	kSYS_PIP_PAP_KEY,        /* eSYS_PIP_PAP_KEY      */
	kSYS_PIP_KEY,            /* eSYS_PIP_KEY          */
	kSYS_PAP_KEY,            /* eSYS_PAP_KEY          */
	kSYS_RECORD_KEY,		 /* eSYS_RECORD_KEY		  */
	kSYS_STOP_KEY,			 /* eSYS_STOP_KEY		  */
	kSYS_PLAY_KEY,			 /* eSYS_PLAY_KEY		  */
	kSYS_BACKWARD_KEY,		 /* eSYS_BACKWARD_KEY     */
	kSYS_FORWARD_KEY, 		 /* eSYS_FORWARD_KEY	  */
	NO_PHYSICAL_CODE,        /* eSYS_AD_KEY           */
    kSYS_BACK_KEY,           /* eSYS_BACK_KEY         */
    kSYS_SKIP_BACKWARD_KEY,  /* eSYS_SKIP_BACKWARD_KEY*/
    kSYS_SKIP_FORWARD_KEY,   /* eSYS_SKIP_FORWARD_KEY */
    kSYS_HOTKEY1,            /* eSYS_HOTKEY1           */
    kSYS_HOTKEY2,            /* eSYS_HOTKEY2           */
    kSYS_HOTKEY3,            /* eSYS_HOTKEY3           */
    kSYS_MEDIA_LIBRARY_KEY,  /* eSYS_MEDIA_LIBRARY_LEY */
    kSYS_PVR_LIBRARY_KEY,    /* eSYS_PVR_LIBRARY_KEY   */
    kSYS_MAIL_KEY,           /* eSYS_MAIL_KEY          */
};


/*===========================================================================
 * SYS_KBD_GetKeyMappingNumber
 *
 *
 * Parameters:
 *
 * Description:
 *      return
 *
 * Returns:
 *
 *
 *===========================================================================*/
 uint8_t SYS_KBD_GetKeyMappingNumber(void)
 {
	uint8_t nb_key = 0;
	uint8_t index;

	for (index = 0; index<SYS_CODE_NUMBER;index++)
	{
		if (SYS_KeyEvtCodeMapping[index] != NO_PHYSICAL_CODE)
		{
			nb_key++;
		}
	}
	return nb_key;
 }

/*===========================================================================
 * SYS_KBD_GetKeyMapping
 *
 *
 * Parameters:
 *
 * Description:
 *      return
 *
 * Returns:
 *
 *
 *===========================================================================*/
 void SYS_KBD_GetKeyMapping(uint16_t *TDAL_KBD_KeyCode)
 {
 	uint16_t* ptrTemp;
 	uint8_t index;

	if (TDAL_KBD_KeyCode != NULL)
	{
		ptrTemp = TDAL_KBD_KeyCode;
		for (index=0;index <SYS_CODE_NUMBER;index++)
		{
			if (SYS_KeyEvtCodeMapping[index] != NO_PHYSICAL_CODE)
			{
				*ptrTemp = SYS_KeyEvtCodeMapping[index];
				ptrTemp++;
			}
		}
	}
 }


/*===========================================================================
 * SYS_KDB_ConvertPhysicalCodeToSYSCode
 *
 *
 * Parameters:
 *
 * Description:
 *      return
 *
 * Returns:
 *
 *
 *===========================================================================*/
 tSYS_KeyEvtCode SYS_KDB_ConvertPhysicalCodeToSYSCode(tSYS_KBD_KeyCode KeyCode)
 {
	uint8_t index;
    tSYS_KeyEvtCode     return_index = eSYS_KEY_EVT_LAST;

	if(KeyCode != NO_PHYSICAL_CODE)
	{
        for (index=0;(index <SYS_CODE_NUMBER) &&
                     (return_index == eSYS_KEY_EVT_LAST);index++)
    	{
    		if (SYS_KeyEvtCodeMapping[index] == KeyCode)
    		{
                return_index = index;
            }
		}
	}

	return return_index;
}


/*===========================================================================
 *
 * SYS_KDB_CFG_CheckIfWakeUpKey
 *
 * Parameters:
 *      KeyCode
 *
 * Description: Returns TRUE if the key pressed wakes up the application
 *              FALSE otherwise
 *
 *
 * Returns:
 *      boolean
 *
 * Comments: eSYS_POWER_KEY always wakes up the application
 *           Note that if you add/remove keys to this list the screen 9400 MUST
 *           be modified accordingly.
 *
 *===========================================================================*/
bool SYS_KDB_CFG_CheckIfWakeUpKey(tSYS_KeyEvtCode KeyCode)
{


    /*************************************************/
    /* Note that if you add/remove keys to this list */
    /* the screen 9400 MUST be modified accordingly: */
    /*                                               */
    /*    ...                                        */
    /*    <KeyDown>                                  */
    /*        <Goto param="48">                      */
    /*            <Id>5000</Id>                      */
    /*        </Goto>                                */
    /*    ...                                        */
    /*                                               */
    /* Refer to file player_keys.h to have the ID    */
    /* corresponding to a key                        */
    /*************************************************/

    if ((KeyCode == eSYS_POWER_KEY) /*||
        (KeyCode == eSYS_PROGRAM_NEXT_KEY) ||
        (KeyCode == eSYS_PROGRAM_PREV_KEY) ||
#ifndef PACKAGE_PVR
        (KeyCode == eSYS_TV_RADIO_KEY) ||
#endif
        (KeyCode == eSYS_KEY_DIGIT0) ||
        (KeyCode == eSYS_KEY_DIGIT1) ||
        (KeyCode == eSYS_KEY_DIGIT2) ||
        (KeyCode == eSYS_KEY_DIGIT3) ||
        (KeyCode == eSYS_KEY_DIGIT4) ||
        (KeyCode == eSYS_KEY_DIGIT5) ||
        (KeyCode == eSYS_KEY_DIGIT6) ||
        (KeyCode == eSYS_KEY_DIGIT7) ||
        (KeyCode == eSYS_KEY_DIGIT8) ||
        (KeyCode == eSYS_KEY_DIGIT9) ||
        (KeyCode == eSYS_UP_ARROW_KEY) ||
        (KeyCode == eSYS_DOWN_ARROW_KEY) ||
        (KeyCode == eSYS_EXIT_KEY)*/)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }

}

/*===========================================================================
 *
 * SYS_KDB_CFG_IsMappedOnPhysicalKey
 *
 * Parameters:
 *      KeyCode
 *
 * Description: Returns TRUE if the key code is mapped on a physical key.
 *
 *
 * Returns:
 *      boolean
 *
 * Comments:
 *
 *===========================================================================*/
bool SYS_KDB_CFG_IsMappedOnPhysicalKey(tSYS_KeyEvtCode keyEvtCode)
{
    bool    result = FALSE;

    if ((keyEvtCode < SYS_CODE_NUMBER) &&
        (SYS_KeyEvtCodeMapping[keyEvtCode] != NO_PHYSICAL_CODE))
    {
        result = TRUE;
    }

    return(result);
}

