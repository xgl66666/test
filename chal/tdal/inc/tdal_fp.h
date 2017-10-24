/******************************************************************************
 *            COPYRIGHT 2004 IWEDIA TECHNOLOGIES        *
 ******************************************************************************
 *
 * MODULE NAME: TDAL FORNT PANEL
 *
 * FILE NAME: $URL: http://10.50.1.219/svncomedia/sources/chal/core/trunk/tdal/inc/tdal_fp.h $
 *      $Rev: 66 $
 *      $Date: 2008-10-14 10:29:19 +0200 (mar., 14 oct. 2008) $
 *
 * PUBLIC
 *
 * DESCRIPTION - Header of the TDAL Front panel
 *
 *****************************************************************************/

#ifndef _TDAL_FP_H_
#define _TDAL_FP_H_

/*******************************************************/
/*       Includes              */
/*******************************************************/

#ifdef __cplusplus
extern "C" {
#endif



/*******************************************************/
/*       Defines              */
/*******************************************************/

#define kTDAL_FP_NO_ID             -1
/* number to digit to manage */
#define kTDAL_FP_NB_DIGIT           4

#define kTDAL_FP_NO_KEY             0x00
#define kTDAL_FP_POWER_KEY          0x01
#define kTDAL_FP_SELECT_KEY         0x02
#define kTDAL_FP_MENU_KEY           0x04
#define kTDAL_FP_RIGHT_ARROW_KEY    0x08
#define kTDAL_FP_LEFT_ARROW_KEY     0x10
#define kTDAL_FP_UP_ARROW_KEY       0x20
#define kTDAL_FP_DOWN_ARROW_KEY     0x40
#define kTDAL_FP_EXIT_KEY           0x80
#define kTDAL_FP_PROGRAM_NEXT_KEY   0x0f
#define kTDAL_FP_PROGRAM_PREV_KEY   0xf0
#define kTDAL_FP_VOLUME_NEXT_KEY    0x2f
#define kTDAL_FP_VOLUME_PREV_KEY    0xf2
#define kTDAL_FP_INFO_KEY           0x1f
#define kTDAL_FP_CUSTOM_1_KEY       0xf1
#define kTDAL_FP_CUSTOM_2_KEY       0xf4


#define kTDAL_FP_OK_STRING "OK"
#define kTDAL_FP_NOK_STRING "NOK"
#define kTDAL_FP_STRING_PVR_PLAY "PLAY"

/*******************************************************/
/*       Typedefs              */
/*******************************************************/
typedef enum
{
   eTDAL_FP_INFO_BOOTING,
   eTDAL_FP_INFO_ON,
   eTDAL_FP_INFO_OFF,
   eTDAL_FP_INFO_RECORD,
   eTDAL_FP_INFO_FLASH_ERASE,
   eTDAL_FP_INFO_FLASH_RW,
   eTDAL_FP_INFO_RCVDATA,
   eTDAL_FP_INFO_RCVINFRARED,
   eTDAL_FP_INFO_SCANNING,
   eTDAL_FP_INFO_HOUR,
   eTDAL_FP_INFO_MIN,
   eTDAL_FP_INFO_STRING,
   eTDAL_FP_INFO_VIDEO_START,
   eTDAL_FP_INFO_SHOW_MENU,
   eTDAL_FP_INFO_FE_LOCK,
   eTDAL_FP_INFO_DOWNLOAD,
   eTDAL_FP_INFO_ERROR,
   eTDAL_FP_INFO_CHANNELNUMBER,
   eTDAL_FP_INFO_PVR,
   eTDAL_FP_INFO_NO_SIGNAL
} tTDAL_FP_Info;


typedef enum {
   eTDAL_FP_NO_ERROR,
   eTDAL_FP_ERROR
}tTDAL_FP_ErrorCode;

typedef int16_t tTDAL_FP_ClientId;

typedef uint8_t tTDAL_FP_KeyCode;

typedef enum {
   eTDAL_FP_KEY_PRESSED,
   eTDAL_FP_KEY_RELEASED,
   eTDAL_FP_KEY_HELD_DOWN
}tTDAL_FP_KeyStatus;

typedef void (*tTDAL_FP_CallbackFct) (
   tTDAL_FP_KeyCode   KeyCode,
   tTDAL_FP_KeyStatus KeyStatus
);

//KAON_BR_LED
typedef enum
{
	FP_LED_GREEN_ON,
	FP_LED_GREEN_OFF,
	FP_LED_RED_ON,
	FP_LED_RED_OFF,
	FP_LED_ALL_OFF
} tTDAL_FP_LED_STATUS;

/*******************************************************/
/*       Variables Declarations (IMPORT)    */
/*******************************************************/

/*******************************************************/
/*       Functions Declarations        */
/*******************************************************/
IMPORT tTDAL_FP_ErrorCode TDAL_FP_Init(
   void
);

IMPORT tTDAL_FP_ErrorCode TDAL_FP_Terminate(
   void
);

IMPORT tTDAL_FP_ClientId TDAL_FP_Register(
   tTDAL_FP_CallbackFct callback   /* I: Function called upon key events */
);

IMPORT tTDAL_FP_ErrorCode TDAL_FP_UnRegister(
   tTDAL_FP_ClientId clientId     /* I: Id of the client to unregister */
);

IMPORT tTDAL_FP_ErrorCode TDAL_FP_SetInfo(
   tTDAL_FP_Info eInfo,
   const void* param
);

IMPORT tTDAL_FP_ErrorCode TDAL_FP_EnterActiveLowPower(
   uint32_t timeout
);

IMPORT tTDAL_FP_ErrorCode TDAL_FP_ExitActiveLowPower(
   void
);

//KAON_BR_LED
IMPORT void TDAL_FP_SetLedSatus(tTDAL_FP_LED_STATUS u16LED_STATUS);
#ifdef __cplusplus
}
#endif


#endif /*_TDAL_FP_H_*/
