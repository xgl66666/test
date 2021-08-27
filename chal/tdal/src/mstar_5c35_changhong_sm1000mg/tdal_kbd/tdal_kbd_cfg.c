#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "crules.h"
#include "tbox.h"

#include "tdal_kbd.h"
#include "tdal_kbd_module_priv.h"

#ifdef   __cplusplus
extern   "C"   {
#endif

tNativeCode TDAL_KBD_NativePhysicalCodes[TDAL_KBD_CONCURRENT_NUM][TDAL_KBD_ADDRESS_NUMBER_MAX] =
{{
        { kTDAL_KEY_DIGIT0,         0x13},/*KEY_DIGIT0*/
        { kTDAL_KEY_DIGIT1,         0x00},/*KEY_DIGIT1*/
        { kTDAL_KEY_DIGIT2,         0x07},/*KEY_DIGIT2*/
        { kTDAL_KEY_DIGIT3,         0x06},/*KEY_DIGIT3*/
        { kTDAL_KEY_DIGIT4,         0x04},/*KEY_DIGIT4*/
        { kTDAL_KEY_DIGIT5,         0x0b},/*KEY_DIGIT5*/
        { kTDAL_KEY_DIGIT6,         0x0a},/*KEY_DIGIT6*/
        { kTDAL_KEY_DIGIT7,         0x08},/*KEY_DIGIT7*/
        { kTDAL_KEY_DIGIT8,         0x0f},/*KEY_DIGIT8*/
        { kTDAL_KEY_DIGIT9,         0x0e},/*KEY_DIGIT9*/
        { kTDAL_MUTE_KEY,           0x12},/*MUTE_KEY*/
        { kTDAL_POWER_KEY,          0x01},/*POWER_KEY*/
        { kTDAL_FAVORITE_KEY,       0x5c},/*FAVORITE_KEY*/
        { kTDAL_AUDIO_KEY,          0x5d},/*AUDIO_KEY - LANG_KEY GTPL: Cisco RCU NEC*/
        { kTDAL_RED_KEY,            0x51},/*RED_KEY*/
        { kTDAL_GREEN_KEY,          0x52},/*GREEN_KEY*/
        { kTDAL_YELLOW_KEY,         0x53},/*YELLOW_KEY*/
        { kTDAL_BLUE_KEY,           0x54},/*BLUE_KEY*/
        { kTDAL_EPG_KEY,            0x43},/*EPG_KEY*/
        { kTDAL_INFO_KEY,           0x58},/*INFO_KEY - NAV_KEY GTPL: Cisco RCU NEC*/
        { kTDAL_MENU_KEY,           0x5b},/*MENU_KEY*/
        { kTDAL_UP_ARROW_KEY,       0x17},/*UP_ARROW_KEY*/
        { kTDAL_LEFT_ARROW_KEY,     0x42},/*LEFT_ARROW_KEY*/
        { kTDAL_SELECT_KEY,         0x15},/*SELECT_KEY*/
        { kTDAL_RIGHT_ARROW_KEY,    0x41},/*RIGHT_ARROW_KEY*/
        { kTDAL_DOWN_ARROW_KEY,     0x1a},/*DOWN_ARROW_KEY*/
        { kTDAL_BACK_KEY,           0x09},/*BACK_KEY - LAST_KEY GTPL: Cisco RCU NEC*/
        { kTDAL_PROGRAM_NEXT_KEY,   0x4b},/*PROGRAM_NEXT_KEY*/
        { kTDAL_PROGRAM_PREV_KEY,   0x4a},/*PROGRAM_PREV_KEY*/
        { kTDAL_VOLUME_NEXT_KEY,    TDAL_KBD_NON_PHYSICAL_CODE},/*VOLUME_NEXT_KEY*/
        { kTDAL_VOLUME_PREV_KEY,    TDAL_KBD_NON_PHYSICAL_CODE},/*VOLUME_PREV_KEY*/
        { kTDAL_TV_RADIO_KEY,       0x1d},/*TV_RADIO_KEY*/
        { kTDAL_EXIT_KEY,           0x16},/*EXIT_KEY*/
        { kTDAL_STOP_KEY,           0x56},/*STOP_KEY*/
        { kTDAL_LIST_KEY,           TDAL_KBD_NON_PHYSICAL_CODE},/*LIST_KEY*/
        { kTDAL_RECORD_KEY,         0x5a},/*RECORD_KEY*/
        { kTDAL_PLAY_KEY,           0x4c},/*PLAY_KEY*/
        { kTDAL_PAUSE_KEY,          0x55},/*PAUSE_KEY*/
        { kTDAL_BACKWARD_KEY,       0x14},/*BACKWARD_KEY*/
        { kTDAL_FORWARD_KEY,        0x0d},/*EXIT_KEY*/
        { kTDAL_HOTKEY1,            0x19},/*HELP_KEY*/
        { kTDAL_PVR_LIBRARY_KEY,    0x57},/*USB_KEY*/
        { kTDAL_MEDIA_LIBRARY_KEY,  0x10},/*MEDIA_KEY*/
        { kTDAL_PAUSE_KEY,          0x4e},/*TIMESHIFT_KEY*/
        { kTDAL_HOTKEY2,            0x1b},/*MyACT*/
        { 0x2C, TDAL_KBD_NON_PHYSICAL_CODE},
        { 0x2D, TDAL_KBD_NON_PHYSICAL_CODE},
        { 0x2E, TDAL_KBD_NON_PHYSICAL_CODE},
        { 0x2F, TDAL_KBD_NON_PHYSICAL_CODE},
        { 0x30, TDAL_KBD_NON_PHYSICAL_CODE},
        { 0x31, TDAL_KBD_NON_PHYSICAL_CODE}
},
{
        { kTDAL_KEY_DIGIT0,         0x13},/*KEY_DIGIT0*/
        { kTDAL_KEY_DIGIT1,         0x00},/*KEY_DIGIT1*/
        { kTDAL_KEY_DIGIT2,         0x07},/*KEY_DIGIT2*/
        { kTDAL_KEY_DIGIT3,         0x06},/*KEY_DIGIT3*/
        { kTDAL_KEY_DIGIT4,         0x04},/*KEY_DIGIT4*/
        { kTDAL_KEY_DIGIT5,         0x0b},/*KEY_DIGIT5*/
        { kTDAL_KEY_DIGIT6,         0x0a},/*KEY_DIGIT6*/
        { kTDAL_KEY_DIGIT7,         0x08},/*KEY_DIGIT7*/
        { kTDAL_KEY_DIGIT8,         0x0f},/*KEY_DIGIT8*/
        { kTDAL_KEY_DIGIT9,         0x0e},/*KEY_DIGIT9*/
        { kTDAL_MUTE_KEY,           0x12},/*MUTE_KEY*/
        { kTDAL_POWER_KEY,          0x01},/*POWER_KEY*/
        { kTDAL_FAVORITE_KEY,       0x5c},/*FAVORITE_KEY*/
        { kTDAL_AUDIO_KEY,          0x5d},/*AUDIO_KEY - LANG_KEY GTPL: Cisco RCU NEC*/
        { kTDAL_RED_KEY,            0x51},/*RED_KEY*/
        { kTDAL_GREEN_KEY,          0x52},/*GREEN_KEY*/
        { kTDAL_YELLOW_KEY,         0x53},/*YELLOW_KEY*/
        { kTDAL_BLUE_KEY,           0x54},/*BLUE_KEY*/
        { kTDAL_EPG_KEY,            0x43},/*EPG_KEY*/
        { kTDAL_INFO_KEY,           0x58},/*INFO_KEY - NAV_KEY GTPL: Cisco RCU NEC*/
        { kTDAL_MENU_KEY,           0x5b},/*MENU_KEY*/
        { kTDAL_UP_ARROW_KEY,       0x17},/*UP_ARROW_KEY*/
        { kTDAL_LEFT_ARROW_KEY,     0x42},/*LEFT_ARROW_KEY*/
        { kTDAL_SELECT_KEY,         0x15},/*SELECT_KEY*/
        { kTDAL_RIGHT_ARROW_KEY,    0x41},/*RIGHT_ARROW_KEY*/
        { kTDAL_DOWN_ARROW_KEY,     0x1a},/*DOWN_ARROW_KEY*/
        { kTDAL_BACK_KEY,           0x09},/*BACK_KEY - LAST_KEY GTPL: Cisco RCU NEC*/
        { kTDAL_PROGRAM_NEXT_KEY,   TDAL_KBD_NON_PHYSICAL_CODE},/*PROGRAM_NEXT_KEY*/
        { kTDAL_PROGRAM_PREV_KEY,   TDAL_KBD_NON_PHYSICAL_CODE},/*PROGRAM_PREV_KEY*/
        { kTDAL_VOLUME_NEXT_KEY,    TDAL_KBD_NON_PHYSICAL_CODE},/*VOLUME_NEXT_KEY*/
        { kTDAL_VOLUME_PREV_KEY,    TDAL_KBD_NON_PHYSICAL_CODE},/*VOLUME_PREV_KEY*/
        { kTDAL_TV_RADIO_KEY,       0x1d},/*TV_RADIO_KEY*/
        { kTDAL_EXIT_KEY,           0x16},/*EXIT_KEY*/
        { kTDAL_STOP_KEY,           0x56},/*STOP_KEY*/
        { kTDAL_LIST_KEY,           TDAL_KBD_NON_PHYSICAL_CODE},/*LIST_KEY*/
        { kTDAL_RECORD_KEY,         0x5a},/*RECORD_KEY*/
        { kTDAL_PLAY_KEY,           0x4c},/*PLAY_KEY*/
        { kTDAL_PAUSE_KEY,          0x55},/*PAUSE_KEY*/
        { kTDAL_BACKWARD_KEY,       0x0d},/*BACKWARD_KEY*/
        { kTDAL_FORWARD_KEY,        0x14},/*FORWARD_KEY*/
        { kTDAL_HOTKEY1,            TDAL_KBD_NON_PHYSICAL_CODE},/*HELP_KEY*/
        { kTDAL_PVR_LIBRARY_KEY,    0x57},/*USB_KEY*/
        { kTDAL_MEDIA_LIBRARY_KEY,  0x10},/*MEDIA_KEY*/
        { kTDAL_PAUSE_KEY,          0x4e},/*TIMESHIFT_KEY*/
        { kTDAL_SUBTITLE_KEY,       TDAL_KBD_NON_PHYSICAL_CODE},/*SUBTITLE_KEY*/
        { 0x2C, TDAL_KBD_NON_PHYSICAL_CODE},
        { 0x2D, TDAL_KBD_NON_PHYSICAL_CODE},
        { 0x2E, TDAL_KBD_NON_PHYSICAL_CODE},
        { 0x2F, TDAL_KBD_NON_PHYSICAL_CODE},
        { 0x30, TDAL_KBD_NON_PHYSICAL_CODE},
        { 0x31, TDAL_KBD_NON_PHYSICAL_CODE}
},
    {
		{ kTDAL_KEY_DIGIT0,         0x87},/*KEY_DIGIT0*/
		{ kTDAL_KEY_DIGIT1,         0x92},/*KEY_DIGIT1*/
		{ kTDAL_KEY_DIGIT2,         0x93},/*KEY_DIGIT2*/
		{ kTDAL_KEY_DIGIT3,         0xcc},/*KEY_DIGIT3*/
		{ kTDAL_KEY_DIGIT4,         0x8e},/*KEY_DIGIT4*/
		{ kTDAL_KEY_DIGIT5,         0x8f},/*KEY_DIGIT5*/
        { kTDAL_KEY_DIGIT6,         0xc8},/*KEY_DIGIT6*/
        { kTDAL_KEY_DIGIT7,         0x8a},/*KEY_DIGIT7*/
        { kTDAL_KEY_DIGIT8,         0x8b},/*KEY_DIGIT8*/
        { kTDAL_KEY_DIGIT9,         0xc4},/*KEY_DIGIT9*/
        { kTDAL_MUTE_KEY,           0x9c},/*MUTE_KEY*/
        { kTDAL_POWER_KEY,          0xdc},/*POWER_KEY*/
        { kTDAL_FAVORITE_KEY,       0xcf},/*FAVORITE_KEY*/
        { kTDAL_AUDIO_KEY,          0x95},/*AUDIO_KEY - LANG_KEY GTPL: Cisco RCU NEC*/
        { kTDAL_RED_KEY,            0x89},/*RED_KEY*/
        { kTDAL_GREEN_KEY,          0x84},/*GREEN_KEY*/
        { kTDAL_YELLOW_KEY,         0xd9},/*YELLOW_KEY*/
        { kTDAL_BLUE_KEY,           0x96},/*BLUE_KEY*/
        { kTDAL_EPG_KEY,            0xda},/*EPG_KEY*/
        { kTDAL_INFO_KEY,           0x88},/*INFO_KEY - NAV_KEY GTPL: Cisco RCU NEC*/
        { kTDAL_MENU_KEY,           0x98},/*MENU_KEY*/
        { kTDAL_UP_ARROW_KEY,       0xca},/*UP_ARROW_KEY*/
        { kTDAL_LEFT_ARROW_KEY,     0x99},/*LEFT_ARROW_KEY*/
        { kTDAL_SELECT_KEY,         0xce},/*SELECT_KEY*/
        { kTDAL_RIGHT_ARROW_KEY,    0xc1},/*RIGHT_ARROW_KEY*/
        { kTDAL_DOWN_ARROW_KEY,     0xd2},/*DOWN_ARROW_KEY*/
        { kTDAL_BACK_KEY,           0x82},/*BACK_KEY - LAST_KEY GTPL: Cisco RCU NEC*/
        { kTDAL_PROGRAM_NEXT_KEY,   0x85},/*PROGRAM_NEXT_KEY*/
        { kTDAL_PROGRAM_PREV_KEY,   0x86},/*PROGRAM_PREV_KEY*/
        { kTDAL_VOLUME_NEXT_KEY,    0x80},/*VOLUME_NEXT_KEY*/
        { kTDAL_VOLUME_PREV_KEY,    0x81},/*VOLUME_PREV_KEY*/
        { kTDAL_TV_RADIO_KEY,       0xc3},/*TV_RADIO_KEY*/
        { kTDAL_EXIT_KEY,           0xc5},/*EXIT_KEY*/
        { kTDAL_STOP_KEY,           0x8d},/*STOP_KEY*/
        { kTDAL_LIST_KEY,           TDAL_KBD_NON_PHYSICAL_CODE},/*LIST_KEY*/
        { kTDAL_RECORD_KEY,         0xd1},/*RECORD_KEY*/
        { kTDAL_PLAY_KEY,           0x97},/*PLAY_KEY*/
        { kTDAL_PAUSE_KEY,          0x91},/*PAUSE_KEY*/
        { kTDAL_BACKWARD_KEY,       0xcd},/*BACKWARD_PREV_KEY*/
        { kTDAL_FORWARD_KEY,        0xd0},/*EXIT_KEY*/
        { kTDAL_HOTKEY1,            0x8c},/*HELP_KEY*/
        { 0x28, TDAL_KBD_NON_PHYSICAL_CODE},
        { 0x29, TDAL_KBD_NON_PHYSICAL_CODE},
        { 0x2A, TDAL_KBD_NON_PHYSICAL_CODE},
        { 0x2B, TDAL_KBD_NON_PHYSICAL_CODE},
        { 0x2C, TDAL_KBD_NON_PHYSICAL_CODE},
        { 0x2D, TDAL_KBD_NON_PHYSICAL_CODE},
        { 0x2E, TDAL_KBD_NON_PHYSICAL_CODE},
        { 0x2F, TDAL_KBD_NON_PHYSICAL_CODE},
        { 0x30, TDAL_KBD_NON_PHYSICAL_CODE},
        { 0x31, TDAL_KBD_NON_PHYSICAL_CODE}
    }

};

void SortCMKeyCodeArray(tNativeCode *Array, uint8_t length)
{
    int i, j;

    for(i = 0; i < length; i++)
    {
        bool exit = true;
        for(j = 0; j < length-1; j++)
        {
            if (Array[j].nativeKeyCode > Array[j+1].nativeKeyCode)
            {
                tNativeCode temp = Array[j+1];
                Array[j+1] = Array[j];
                Array[j] = temp;
                exit = false;
            }
        }
        if (exit)
            break;
    }
}

uint16_t QSearch_CM_PhysicalCode(uint16_t keyCode, tNativeCode * nativeCodeArray, uint8_t startIdx, uint8_t endIdx,uint8_t nNativeCodeIdx)
{
    bool bFound = false;
    uint8_t pivot = 0xFF;
    uint16_t result = 0xFFFF;
    while (bFound==false && startIdx!=endIdx)
    {
        pivot = (startIdx + endIdx) / 2 + 1;

        if (keyCode == nativeCodeArray[pivot].nativeKeyCode)
        {
            result = nativeCodeArray[pivot].cmPhysicalCode;
            bFound = true;
        }
        else if (keyCode < nativeCodeArray[pivot].nativeKeyCode)
        {
            if (pivot == endIdx)
            {
                if (keyCode == nativeCodeArray[pivot-1].nativeKeyCode)
                {
                    result = nativeCodeArray[pivot-1].cmPhysicalCode;
                    bFound = true;
                }
                else
                {
                    endIdx = pivot-1;
                }
            }
            else
            {
                endIdx = pivot;
            }
        }
        else
        {
            if (pivot == endIdx - 1)
            {
                if (keyCode == nativeCodeArray[pivot + 1].nativeKeyCode)
                {
                    result = nativeCodeArray[pivot + 1].cmPhysicalCode;
                    bFound = true;
                }
                else
                {
                    startIdx = endIdx;
                }
            }
            else
            {
                startIdx = pivot;
            }
        }
    }
#ifdef TDAL_MULTI_RCUS
#ifdef CUSTOMIZATION_GTPL
    // first and second RCU (nNativeCodeIdx == 0 and nNativeCodeIdx == 1) does not have separate key for CH+/CH-
    if((TDAL_KBD_NATIVE_CODE_IDX_FIRST_RCU == nNativeCodeIdx) || (TDAL_KBD_NATIVE_CODE_IDX_SECOND_RCU == nNativeCodeIdx))
    {
    	if(kTDAL_DOWN_ARROW_KEY == result)
    	{
    		result = kTDAL_DOWN_MIXED_KEY;
    	}
    	else if (kTDAL_UP_ARROW_KEY == result)
    	{
    		result = kTDAL_UP_MIXED_KEY;
    	}
    }
#endif
#endif
    return result;
}

#ifdef   __cplusplus
}
#endif
