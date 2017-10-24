#ifndef   _TDAL_KBD_MODULE_PRIV_H_
#define   _TDAL_KBD_MODULE_PRIV_H

#ifdef TDAL_MULTI_RCUS
    #define TDAL_KBD_CONCURRENT_NUM 3
    #if !defined(TDAL_KBD_CONCURRENT_NUM)
        #error Please define TDAL_KBD_CONCURRENT_NUM with number higher than 1!!!
    #endif
	#define TDAL_KBD_NATIVE_CODE_IDX_FIRST_RCU 0
	#define TDAL_KBD_NATIVE_CODE_IDX_SECOND_RCU 1
    #define TDAL_KBD_ADDRESS_NUMBER_MAX 50
    #define TDAL_KBD_NON_PHYSICAL_CODE 0xffff

    /*******************************************/
    /*	Comedia default physical RCU key codes */
    /*******************************************/
    #define kTDAL_KEY_DIGIT0         0x13
    #define kTDAL_KEY_DIGIT1         0x00
    #define kTDAL_KEY_DIGIT2         0x07
    #define kTDAL_KEY_DIGIT3         0x06
    #define kTDAL_KEY_DIGIT4         0x04
    #define kTDAL_KEY_DIGIT5         0x0b
    #define kTDAL_KEY_DIGIT6         0x0a
    #define kTDAL_KEY_DIGIT7         0x08
    #define kTDAL_KEY_DIGIT8         0x0f
    #define kTDAL_KEY_DIGIT9         0x0E
    #define kTDAL_MUTE_KEY           0x12
    #define kTDAL_POWER_KEY          0x01
    #define kTDAL_FAVORITE_KEY       0x5c
    #define kTDAL_SUBTITLE_KEY       0x1b
    #define kTDAL_TELETEXT_KEY       TDAL_KBD_NON_PHYSICAL_CODE
    #define kTDAL_AUDIO_KEY          0x5d
    #define kTDAL_RED_KEY            0x51
    #define kTDAL_GREEN_KEY          0x52
    #define kTDAL_YELLOW_KEY         0x53
    #define kTDAL_BLUE_KEY           0x54
    #define kTDAL_EPG_KEY            0x43
    #define kTDAL_INFO_KEY           0x58
    #define kTDAL_MENU_KEY           0x5b
    #define kTDAL_UP_ARROW_KEY       0x17
    #define kTDAL_LEFT_ARROW_KEY     0x42
    #define kTDAL_SELECT_KEY         0x15
    #define kTDAL_RIGHT_ARROW_KEY    0x41
    #define kTDAL_DOWN_ARROW_KEY     0x1a
    #define kTDAL_BACK_KEY           0x09
    #define kTDAL_PROGRAM_NEXT_KEY   0x4b
    #define kTDAL_PROGRAM_PREV_KEY   0x4a
    #define kTDAL_VOLUME_NEXT_KEY    0x5e
    #define kTDAL_VOLUME_PREV_KEY    0x1e
    #define kTDAL_TV_RADIO_KEY       0x1d
    #define kTDAL_EXIT_KEY           0x16
    #define kTDAL_PAUSE_KEY          0x55
    #define kTDAL_SWAP_KEY           TDAL_KBD_NON_PHYSICAL_CODE
    #define kTDAL_PRF_NEXT_KEY       TDAL_KBD_NON_PHYSICAL_CODE
    #define kTDAL_STOP_KEY           0x56
    #define kTDAL_TIMER_KEY          TDAL_KBD_NON_PHYSICAL_CODE
    #define kTDAL_PAGE_NEXT_KEY      0x4a
    #define kTDAL_PAGE_PREV_KEY      0x4b
    #define kTDAL_STANDBY_KEY        TDAL_KBD_NON_PHYSICAL_CODE
    #define kTDAL_TELETEXT_MIX_KEY   TDAL_KBD_NON_PHYSICAL_CODE
    #define kTDAL_TVFORMAT_KEY       TDAL_KBD_NON_PHYSICAL_CODE
    #define kTDAL_PRF_PREV_KEY       TDAL_KBD_NON_PHYSICAL_CODE
    #define kTDAL_LIST_KEY           0x40
    #define kTDAL_TV_STB_KEY         TDAL_KBD_NON_PHYSICAL_CODE
    #define kTDAL_PIP_PAP_KEY        TDAL_KBD_NON_PHYSICAL_CODE
    #define kTDAL_PIP_KEY            TDAL_KBD_NON_PHYSICAL_CODE
    #define kTDAL_PAP_KEY            TDAL_KBD_NON_PHYSICAL_CODE
    #define kTDAL_RECORD_KEY         0x5a
    #define kTDAL_OTHERAPP_KEY       TDAL_KBD_NON_PHYSICAL_CODE
    #define kTDAL_PLAY_KEY           0x4c
    #define kTDAL_BACKWARD_KEY       0x14
    #define kTDAL_FORWARD_KEY        0x0d
    #define kTDAL_SKIP_BACKWARD_KEY  TDAL_KBD_NON_PHYSICAL_CODE
    #define kTDAL_SKIP_FORWARD_KEY   TDAL_KBD_NON_PHYSICAL_CODE
    #define kTDAL_HOTKEY1            0x19
    #define kTDAL_HOTKEY2            0x1b
    #define kTDAL_HOTKEY3            TDAL_KBD_NON_PHYSICAL_CODE
    #define kTDAL_PVR_LIBRARY_KEY    0x57
    #define kTDAL_MEDIA_LIBRARY_KEY  0x10
    #define kTDAL_MAIL_KEY           TDAL_KBD_NON_PHYSICAL_CODE
	#define kTDAL_DOWN_MIXED_KEY     0x2a
	#define kTDAL_UP_MIXED_KEY       0x27

#endif  /* TDAL_MULTI_RCUS */

#ifdef   __cplusplus
extern   "C"   {
#endif
tTDAL_KBD_KeyCode TDAL_KBDi_Phy2PhyMapping(tTDAL_KBD_KeyCode keyCode);

typedef struct _NativeCode
{
    uint16_t    cmPhysicalCode;
    uint16_t    nativeKeyCode;
} tNativeCode;

#ifdef   __cplusplus
}
#endif

#endif   /*_TDAL_KBD_MODULE_PRIV_H*/
