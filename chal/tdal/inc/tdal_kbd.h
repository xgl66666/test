/******************************************************************************
 *            COPYRIGHT 2004 IWEDIA TECHNOLOGIES        *
 ******************************************************************************
 *
 * MODULE NAME: TDAL KEYBOARD
 *
 * FILE NAME: $URL: http://10.50.1.219/svncomedia/sources/chal/core/trunk/tdal/inc/tdal_kbd.h $
 *      $Rev: 66 $
 *      $Date: 2008-10-14 10:29:19 +0200 (mar., 14 oct. 2008) $
 *
 * PUBLIC
 *
 * DESCRIPTION - Header of the TDAL Keyboard
 *
 *****************************************************************************/

#ifndef _TDAL_KBD_H_
#define _TDAL_KBD_H_

/*******************************************************/
/*       Includes              */
/*******************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************/
/*       Defines              */
/*******************************************************/
#define kTDAL_KBD_NO_ID      -1
#define kTDAL_KBD_NO_KEY      0

#define kTDAL_KBD_NB_MAX_CLIENTS   1

/*******************************************************/
/*       Macros               */
/*******************************************************/

/*******************************************************/
/*       Typedefs              */
/*******************************************************/
typedef enum {
   eTDAL_KBD_NO_ERROR,
   eTDAL_KBD_ERROR
}tTDAL_KBD_ErrorCode;

typedef int16_t tTDAL_KBD_ClientId;

typedef uint32_t tTDAL_KBD_KeyCode;

typedef enum {
   eTDAL_KBD_KEY_PRESSED,
   eTDAL_KBD_KEY_RELEASED,
   eTDAL_KBD_KEY_HELD_DOWN
}tTDAL_KBD_KeyStatus;

typedef void (*tTDAL_KBD_CallbackFct) (
   tTDAL_KBD_KeyCode KeyCode,
   tTDAL_KBD_KeyStatus KeyStatus
);

/*******************************************************/
/*       Variables Declarations (IMPORT)    */
/*******************************************************/

/*******************************************************/
/*       Functions Declarations        */
/*******************************************************/
tTDAL_KBD_ErrorCode TDAL_KBD_Init(
   void
);

tTDAL_KBD_ErrorCode TDAL_KBD_Term(
   void
);

tTDAL_KBD_ClientId TDAL_KBD_Register(
   tTDAL_KBD_CallbackFct callback   /* I: Function called upon key events */
);

tTDAL_KBD_ErrorCode TDAL_KBD_UnRegister(
   tTDAL_KBD_ClientId clientId     /* I: Id of the client to unregister */
);

tTDAL_KBD_ErrorCode TDAL_KBD_SetKeyMapping(
   unsigned short NbKeys,
   tTDAL_KBD_KeyCode *KeyCode
);

void   TDAL_KBD_ExitKeyPress(void);

#ifdef __cplusplus
}
#endif


#endif /*_TDAL_KBD_H_*/
