/******************************************************************************
 *                         COPYRIGHT 2005 IWEDIA TECHNOLOGIES                         *
 ******************************************************************************
 *
 * MODULE NAME: SYS
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/product/comedia/branches/idecode_hd/build/inc/player_keys.h $
 * Header of the player keys
 *            $Rev: 244 $
 *            $Date: 2011-08-18 18:33:39 +0200 (Thu, 18 Aug 2011) $
 *
 * PUBLIC - delete the non-applicable mention
 *
 * DESCRIPTION
 *
 *****************************************************************************/

#ifndef _PLAYER_KEYS_H_
#define _PLAYER_KEYS_H_


typedef enum
{
	REMOTE_KEY_ENTER			  =	 13,			/* \n			*/
	REMOTE_KEY_BACK				  =	 27,			/* Escape		*/
	REMOTE_KEY_LEFT				  =	  8,			/* Backspace 	*/
	REMOTE_KEY_RIGHT			  =	  9,			/* HT 			*/
	REMOTE_KEY_UP				  =	 11,			/* VF 			*/
	REMOTE_KEY_DOWN				  =	 10,			/* LF 			*/

	REMOTE_KEY_UP_RELEASED		  =	100,
	REMOTE_KEY_DOWN_RELEASED 	  =	101,
	REMOTE_KEY_PAGE_MORE		  =	102,
	REMOTE_KEY_PAGE_LESS		  =	103,
	REMOTE_KEY_EDIT_MORE		  =	104,
	REMOTE_KEY_EDIT_LESS		  =	105,

	REMOTE_KEY_MENU				  =	110,
	REMOTE_KEY_INFO				  =	111,
	REMOTE_KEY_POWER			  =	112,
	REMOTE_KEY_TV_RADIO			  =	113,
	REMOTE_KEY_GUIDE			  =	114,
	REMOTE_KEY_PAUSE			  =	115,
	REMOTE_KEY_TELETEXT			  =	116,
	REMOTE_KEY_SUBTITLE			  =	117,
	REMOTE_KEY_TIMER			  =	118,
	REMOTE_KEY_TV_STB			  =	119,
	REMOTE_KEY_SWAP				  =	120,
	REMOTE_KEY_QUIT				  = 121,
	REMOTE_KEY_PIP_PAP			  =	122,
	REMOTE_KEY_PIP    			  =	123,
	REMOTE_KEY_PAP    			  =	124,
	REMOTE_KEY_TV_FORMAT		  = 125,
	REMOTE_KEY_OTHER_APP 	      =	126,
	REMOTE_KEY_AD				  = 127,
	REMOTE_KEY_PLAY               = 128,
	REMOTE_KEY_STOP               = 129,

	REMOTE_KEY_MUTE				  =	130,
	REMOTE_KEY_VOL_MORE			  =	131,
	REMOTE_KEY_VOL_LESS			  =	132,
	REMOTE_KEY_AUDIO			  =	133,
	REMOTE_KEY_SOUND			  =	134,

	REMOTE_KEY_PROG_MORE		  =	140,
	REMOTE_KEY_PROG_MORE_RELEASED =	141,
	REMOTE_KEY_PROG_LESS		  =	142,
	REMOTE_KEY_PROG_LESS_RELEASED =	143,

	REMOTE_KEY_BLUE				  =	150,
	REMOTE_KEY_YELLOW			  =	151,
	REMOTE_KEY_GREEN			  =	152,
	REMOTE_KEY_RED				  =	153,

	REMOTE_KEY_FAV				  =	160,
	REMOTE_KEY_LIST				  =	161,
	REMOTE_KEY_PREF_LESS		  =	162,
	REMOTE_KEY_PREF_MORE		  =	163,

	REMOTE_KEY_0				  =	 48,			/* '0'		*/
	REMOTE_KEY_1				  =	 49,			/* '1'		*/
	REMOTE_KEY_2				  =	 50,			/* '2'		*/
	REMOTE_KEY_3				  =	 51,			/* '3'		*/
	REMOTE_KEY_4				  =	 52,			/* '4'		*/
	REMOTE_KEY_5				  =	 53,			/* '5'		*/
	REMOTE_KEY_6				  =	 54,			/* '6'		*/
	REMOTE_KEY_7				  =	 55,			/* '7'		*/
	REMOTE_KEY_8				  =	 56,			/* '8'		*/
	REMOTE_KEY_9				  =	 57,			/* '9'		*/

	REMOTE_PVR_FORWARD			  =	200,
	REMOTE_PVR_BACKWARD			  =	201,
	REMOTE_PVR_FIRST			  =	202,
	REMOTE_PVR_LAST				  =	203,
	REMOTE_PVR_STOP				  =	204,
	REMOTE_PVR_START			  =	205,
	REMOTE_PVR_PAUSE			  =	206,
	REMOTE_PVR_ENREG			  =	207,

	REMOTE_KEY_PROG_MORE_HELDOWN  =	208,
	REMOTE_KEY_PROG_LESS_HELDOWN  =	209,
	REMOTE_KEY_UP_HELDOWN		  =	210,
	REMOTE_KEY_DOWN_HELDOWN 	  =	211,
	REMOTE_KEY_LEFT_HELDOWN		  =	212,
	REMOTE_KEY_RIGHT_HELDOWN 	  =	213,
	REMOTE_KEY_GREEN_HELDOWN	  =	214,
	REMOTE_KEY_YELLOW_HELDOWN	  =	215,
	REMOTE_KEY_PAGE_MORE_HELDOWN  =	216,
	REMOTE_KEY_PAGE_LESS_HELDOWN  =	217,

	REMOTE_KEY_HOTKEY1            = 218,
	REMOTE_KEY_HOTKEY2            = 219,
	REMOTE_KEY_HOTKEY3            = 220,

	REMOTE_KEY_FACTORY_DEFAULT    = 221,

    REMOTE_KEY_OTA                = 222,

    REMOTE_KEY_MEDIA_LIBRARY      = 223,
    REMOTE_KEY_PVR_LIBRARY        = 224,
    REMOTE_KEY_MAIL               = 225,
}t_class_rcu_type;

#endif /* _PLAYER_KEYS_H_ */
