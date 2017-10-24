/******************************************************************************
 *                         COPYRIGHT 2005 IWEDIA TECHNOLOGIES                 *
 ******************************************************************************
 *
 * MODULE NAME: SYS
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/product/comedia/branches/idecode_hd/build/src/play_glue_evt_cfg.c $
 *            $Rev: 244 $
 *            $Date: 2011-08-18 18:33:39 +0200 (Thu, 18 Aug 2011) $
 *
 * DESCRIPTION
 *
 *****************************************************************************/
/********************************************************/
/*              Includes                                */
/********************************************************/
#include "crules.h"
#include "play_glue_evt_cfg.h"


/********************************************************/
/*              Variables Definitions (LOCAL/GLOBAL)    */
/********************************************************/
LOCAL tPLAY_KeyMapping PLAY_KeyMapping_Pressed[] =
    {
        {
            eSYS_KEY_DIGIT0,
            REMOTE_KEY_0
        },
        {
            eSYS_KEY_DIGIT1,
            REMOTE_KEY_1
        },
        {
            eSYS_KEY_DIGIT2,
            REMOTE_KEY_2
        },
        {
            eSYS_KEY_DIGIT3,
            REMOTE_KEY_3
        },
        {
            eSYS_KEY_DIGIT4,
            REMOTE_KEY_4
        },
        {
            eSYS_KEY_DIGIT5,
            REMOTE_KEY_5
        },
        {
            eSYS_KEY_DIGIT6,
            REMOTE_KEY_6
        },
        {
            eSYS_KEY_DIGIT7,
            REMOTE_KEY_7
        },
        {
            eSYS_KEY_DIGIT8,
            REMOTE_KEY_8
        },
        {
            eSYS_KEY_DIGIT9,
            REMOTE_KEY_9
        },
        {
            eSYS_EPG_KEY,
            REMOTE_KEY_GUIDE
        },
        {
            eSYS_TELETEXT_KEY,
            REMOTE_KEY_TELETEXT
        },
        {
            eSYS_SUBTITLE_KEY,
            REMOTE_KEY_SUBTITLE
        },
        {
            eSYS_POWER_KEY,
            REMOTE_KEY_POWER
        },
        {
            eSYS_TV_STB_KEY,
            REMOTE_KEY_TV_STB
        },
        {
            eSYS_TV_RADIO_KEY,
            REMOTE_KEY_TV_RADIO
        },
        {
            eSYS_RED_KEY,
            REMOTE_KEY_RED
        },
        {
            eSYS_GREEN_KEY,
            REMOTE_KEY_GREEN
        },
        {
            eSYS_YELLOW_KEY,
            REMOTE_KEY_YELLOW
        },
        {
            eSYS_BLUE_KEY,
            REMOTE_KEY_BLUE
        },
        {
            eSYS_PAUSE_KEY,
            REMOTE_PVR_PAUSE// REMOTE_KEY_PAUSE
        },
        {
        	eSYS_STOP_KEY,
        	REMOTE_KEY_STOP //REMOTE_PVR_STOP
        },
        {
        	eSYS_PLAY_KEY,
        	REMOTE_KEY_PLAY
        },
        {
            eSYS_SWAP_KEY,
            REMOTE_KEY_SWAP
        },
        {
            eSYS_TVFORMAT_KEY,
            REMOTE_KEY_TV_FORMAT
        },
        {
            eSYS_OTHERAPP_KEY,
            REMOTE_KEY_OTHER_APP
        },
        {
            eSYS_AD_KEY,
            REMOTE_KEY_AD
        },
        {
            eSYS_PRF_NEXT_KEY,
            REMOTE_KEY_PREF_MORE
        },
        {
            eSYS_PRF_PREV_KEY,
            REMOTE_KEY_PREF_LESS
        },
        {
            eSYS_VOLUME_NEXT_KEY,
            REMOTE_KEY_VOL_MORE
        },
        {
            eSYS_VOLUME_PREV_KEY,
            REMOTE_KEY_VOL_LESS
        },
        {
            eSYS_PROGRAM_NEXT_KEY,
            REMOTE_KEY_PROG_MORE
        },
        {
            eSYS_PROGRAM_PREV_KEY,
            REMOTE_KEY_PROG_LESS
        },
        {
            eSYS_EXIT_KEY,
            REMOTE_KEY_QUIT
        },
        {
            eSYS_BACK_KEY,
            REMOTE_KEY_BACK

        },
        {
            eSYS_MENU_KEY,
            REMOTE_KEY_MENU
        },
        {
            eSYS_SELECT_KEY,
            REMOTE_KEY_ENTER
        },
        {
            eSYS_UP_ARROW_KEY,
            REMOTE_KEY_UP
        },
        {
            eSYS_LEFT_ARROW_KEY,
            REMOTE_KEY_LEFT
        },
        {
            eSYS_RIGHT_ARROW_KEY,
            REMOTE_KEY_RIGHT
        },
        {
            eSYS_DOWN_ARROW_KEY,
            REMOTE_KEY_DOWN
        },
        {
            eSYS_TIMER_KEY,
            REMOTE_KEY_TIMER
        },
        {
            eSYS_AUDIO_KEY,
            REMOTE_KEY_SOUND
        },
        {
            eSYS_INFO_KEY,
            REMOTE_KEY_INFO
        },
        {
            eSYS_LIST_KEY,
            REMOTE_KEY_LIST
        },
        {
            eSYS_MUTE_KEY,
            REMOTE_KEY_MUTE
        },
        {
            eSYS_PAGE_PREV_KEY,
            REMOTE_KEY_PAGE_MORE
        },
        {
            eSYS_PAGE_NEXT_KEY,
            REMOTE_KEY_PAGE_LESS
        },
        {
            eSYS_FAVORITE_KEY,
            REMOTE_KEY_FAV
        },
        {
            eSYS_PIP_PAP_KEY,
            REMOTE_KEY_PIP_PAP
        },
        {
            eSYS_PIP_KEY,
            REMOTE_KEY_PIP
        },
        {
            eSYS_PAP_KEY,
            REMOTE_KEY_PAP
        },

        /* Additionnal mappings depending on the remote control */
        {
            eSYS_VOLUME_NEXT_KEY,
            REMOTE_KEY_EDIT_MORE
        },
        {
            eSYS_VOLUME_PREV_KEY,
            REMOTE_KEY_EDIT_LESS
        },
        {
            eSYS_RECORD_KEY,
            REMOTE_PVR_START
        },
        {
            eSYS_FORWARD_KEY,
            REMOTE_PVR_FORWARD
        },
        {
            eSYS_BACKWARD_KEY,
            REMOTE_PVR_BACKWARD
        },
        {
            eSYS_SKIP_BACKWARD_KEY,
            REMOTE_PVR_LAST
        },
        {
            eSYS_SKIP_FORWARD_KEY,
            REMOTE_PVR_FIRST
        },
        {
            eSYS_HOTKEY1,
            REMOTE_KEY_HOTKEY1
        },
        {
            eSYS_HOTKEY2,
            REMOTE_KEY_HOTKEY2
        },
        {
            eSYS_HOTKEY3,
            REMOTE_KEY_HOTKEY3
        },
        {
            eSYS_MEDIA_LIBRARY_LEY,
            REMOTE_KEY_MEDIA_LIBRARY
        },
        {
            eSYS_PVR_LIBRARY_KEY,
            REMOTE_KEY_PVR_LIBRARY
        },
        {
            eSYS_MAIL_KEY,
            REMOTE_KEY_MAIL
        },
        {
        	eSYS_FACTORY_DEFAULT_KEY,
        	REMOTE_KEY_FACTORY_DEFAULT
        },
        {
            eSYS_OTA_KEY,
            REMOTE_KEY_OTA
        },
        {
            eSYS_KEY_EVT_LAST, /* Must be the last element of the array */
            0
        }
          
    };


LOCAL tPLAY_KeyMapping PLAY_KeyMapping_HeldDown[] =
    {
        {
            eSYS_VOLUME_NEXT_KEY,
            REMOTE_KEY_VOL_MORE
        },
        {
            eSYS_VOLUME_PREV_KEY,
            REMOTE_KEY_VOL_LESS
        },
        {
            eSYS_PROGRAM_NEXT_KEY,
            REMOTE_KEY_PROG_MORE_HELDOWN
        },
        {
            eSYS_PROGRAM_PREV_KEY,
            REMOTE_KEY_PROG_LESS_HELDOWN
        },
        {
            eSYS_UP_ARROW_KEY,
            REMOTE_KEY_UP_HELDOWN
        },
        {
            eSYS_LEFT_ARROW_KEY,
            REMOTE_KEY_LEFT_HELDOWN
        },
        {
            eSYS_RIGHT_ARROW_KEY,
            REMOTE_KEY_RIGHT_HELDOWN
        },
        {
            eSYS_DOWN_ARROW_KEY,
            REMOTE_KEY_DOWN_HELDOWN
        },
        {
            eSYS_PAGE_PREV_KEY,
            REMOTE_KEY_PAGE_MORE
        },
        {
            eSYS_PAGE_NEXT_KEY,
            REMOTE_KEY_PAGE_LESS
        },
        {
            eSYS_GREEN_KEY,
            REMOTE_KEY_GREEN_HELDOWN
        },
        {
            eSYS_YELLOW_KEY,
            REMOTE_KEY_YELLOW_HELDOWN
        },
        {
            eSYS_PAGE_PREV_KEY,
            REMOTE_KEY_PAGE_MORE_HELDOWN
        },
        {
            eSYS_PAGE_NEXT_KEY,
            REMOTE_KEY_PAGE_LESS_HELDOWN
        },
#ifdef KBD_ST_NEC_DTTRC3
        {
            eSYS_KEY_DIGIT1,
            REMOTE_KEY_PAGE_MORE_HELDOWN
        },
        {
            eSYS_KEY_DIGIT2,
            REMOTE_KEY_PAGE_LESS_HELDOWN
        },
#endif
        {
            eSYS_FACTORY_DEFAULT_KEY,
            REMOTE_KEY_FACTORY_DEFAULT
        },
        {
            eSYS_KEY_EVT_LAST, /* Must be the last element of the array */
            0
        }
    };


LOCAL tPLAY_KeyMapping PLAY_KeyMapping_Released[] =
	{
		{
			eSYS_PROGRAM_NEXT_KEY,
			REMOTE_KEY_PROG_MORE_RELEASED
		},
		{
			eSYS_PROGRAM_PREV_KEY,
			REMOTE_KEY_PROG_LESS_RELEASED
		},
		{
			eSYS_UP_ARROW_KEY,
			REMOTE_KEY_UP_RELEASED
		},
		{
			eSYS_DOWN_ARROW_KEY,
			REMOTE_KEY_DOWN_RELEASED
		},

		{
			eSYS_KEY_EVT_LAST, /* Must be the last element of the array */
			0
		}
	};

/*===========================================================================
 * PLAY_NumberOfRtmCode
 *
 *
 * Parameters: tSYS_KeyEvtCode, tSYS_KeyEvtState
 *
 * Description:
 *     return the number of rtm code associated to the KeyCode in parameter
 *
 * Returns:
 *
 *
 *===========================================================================*/
uint8_t  PLAY_NumberOfRtmCode(tSYS_KeyEvtCode KeyCode, tSYS_KeyEvtState KeyStatus)
{
	uint8_t i=0;
	uint8_t numberRtm=0;

	if (KeyStatus == eSYS_KEY_PRESSED)
	{
		do
		{
			if (KeyCode == PLAY_KeyMapping_Pressed[i].sysCode)
			{
				numberRtm++;
			}
			i++;
		}while(PLAY_KeyMapping_Pressed[i].sysCode != eSYS_KEY_EVT_LAST);
	}
	else if (KeyStatus == eSYS_KEY_HELD_DOWN)
	{
		do
		{
			if (KeyCode == PLAY_KeyMapping_HeldDown[i].sysCode)
			{
				numberRtm++;
			}
			i++;
		}while(PLAY_KeyMapping_HeldDown[i].sysCode != eSYS_KEY_EVT_LAST);
	}

	else if (KeyStatus == eSYS_KEY_RELEASED)
	{
		do
		{
			if (KeyCode == PLAY_KeyMapping_Released[i].sysCode)
			{
				numberRtm++;
			}
			i++;
		}while(PLAY_KeyMapping_Released[i].sysCode != eSYS_KEY_EVT_LAST);
	}

	return(numberRtm);
}

/*===========================================================================
 * PLAY_SysCodeToRtmCode
 *
 *
 * Parameters: tSYS_KeyEvtCode, tSYS_KeyEvtState
 *
 * Description:
 *     return a array of rtm code associated of KeyCode in paramater
 *
 * Returns:
 *	Return true if succeed else false
 *
 *
 *===========================================================================*/
bool PLAY_SysCodeToRtmCode(tSYS_KeyEvtCode KeyCode, tSYS_KeyEvtState KeyStatus, uint8_t* ptrRtm)
{
	uint8_t i=0;

	if (ptrRtm == NULL)
	{
		return false;
	}

	if (KeyStatus == eSYS_KEY_PRESSED)
	{
		do
		{
			if (KeyCode == PLAY_KeyMapping_Pressed[i].sysCode)
			{
				*ptrRtm = PLAY_KeyMapping_Pressed[i].rtmCode;
				ptrRtm++;
			}
			i++;
		}while(PLAY_KeyMapping_Pressed[i].sysCode != eSYS_KEY_EVT_LAST);
	}
	else if (KeyStatus == eSYS_KEY_HELD_DOWN)
	{
		do
		{
			if (KeyCode == PLAY_KeyMapping_HeldDown[i].sysCode)
			{
				*ptrRtm = PLAY_KeyMapping_HeldDown[i].rtmCode;
				ptrRtm++;
			}
			i++;
		}while(PLAY_KeyMapping_HeldDown[i].sysCode != eSYS_KEY_EVT_LAST);
	}

	else if (KeyStatus == eSYS_KEY_RELEASED)
	{
		do
		{
			if (KeyCode == PLAY_KeyMapping_Released[i].sysCode)
			{
				*ptrRtm = PLAY_KeyMapping_Released[i].rtmCode;
				ptrRtm++;
			}
			i++;
		}while(PLAY_KeyMapping_Released[i].sysCode != eSYS_KEY_EVT_LAST);
	}

	return true;
}
