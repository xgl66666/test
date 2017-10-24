/******************************************************************************
 *                         COPYRIGHT 2005 IWEDIA TECHNOLOGIES                         *
 ******************************************************************************
 *
 * MODULE NAME: SYS
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/product/comedia/branches/idecode_hd/build/inc/comedia_keys.h $
 * Header of the Comedia keys
 *            $Rev: 244 $
 *            $Date: 2011-08-18 18:33:39 +0200 (Thu, 18 Aug 2011) $
 *
 * PUBLIC - delete the non-applicable mention
 *
 * DESCRIPTION
 *
 *****************************************************************************/

#ifndef _KEYS_H_
#define _KEYS_H_


typedef enum
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
	eSYS_AD_KEY, /* Audio Description */
    eSYS_BACK_KEY,
    eSYS_SKIP_BACKWARD_KEY,
    eSYS_SKIP_FORWARD_KEY,
    eSYS_HOTKEY1,
    eSYS_HOTKEY2,
    eSYS_HOTKEY3,
    eSYS_MEDIA_LIBRARY_LEY,
    eSYS_PVR_LIBRARY_KEY,
    eSYS_MAIL_KEY,
    eSYS_PLAY_PAUSE_KEY,
    
    /* Second remote control */
    eSYS_KEY_DIGIT0_RC2,
    eSYS_KEY_DIGIT1_RC2,
    eSYS_KEY_DIGIT2_RC2,
    eSYS_KEY_DIGIT3_RC2,
    eSYS_KEY_DIGIT4_RC2,
    eSYS_KEY_DIGIT5_RC2,
    eSYS_KEY_DIGIT6_RC2,
    eSYS_KEY_DIGIT7_RC2,
    eSYS_KEY_DIGIT8_RC2,
    eSYS_KEY_DIGIT9_RC2,
    eSYS_MUTE_KEY_RC2,
    eSYS_POWER_KEY_RC2,
    eSYS_SUBTITLE_KEY_RC2,
    eSYS_VOLUME_NEXT_KEY_RC2,
    eSYS_PROGRAM_NEXT_KEY_RC2,
    eSYS_VOLUME_PREV_KEY_RC2,
    eSYS_PROGRAM_PREV_KEY_RC2,
    eSYS_TV_RADIO_KEY_RC2,
    eSYS_TVFORMAT_KEY_RC2,

    /* hidden keys made from combination of real keys */
    eSYS_FACTORY_DEFAULT_KEY,
    eSYS_OTA_KEY,

    eSYS_KEY_EVT_LAST
}tSYS_KeyEvtCode;

typedef enum
{
    eSYS_KEY_PRESSED,
    eSYS_KEY_RELEASED,
    eSYS_KEY_HELD_DOWN
}tSYS_KeyEvtState;

typedef uint32_t tSYS_KBD_KeyCode;

#endif /*_KEYS_H_*/
