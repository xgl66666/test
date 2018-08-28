/******************************************************************************
 *                         COPYRIGHT 2005 IWEDIA TECHNOLOGIES                         *
 ******************************************************************************
 *
 * MODULE NAME:
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/product/comedia/branches/idecode_hd/build/inc/sys_ui_mngr_kbd_mapping_cfg.h $
 * Header of the System Manager
 *            $Rev: 244 $
 *            $Date: 2011-08-18 18:33:39 +0200 (Thu, 18 Aug 2011) $
 *
 * PUBLIC -
 *
 * DESCRIPTION: this file contain all RC5 code of remote control supported
 *
 *****************************************************************************/
#ifndef _SYS_UI_MNGR_KDB_MAPPING_CFG_
#define _SYS_UI_MNGR_KDB_MAPPING_CFG_

/*******************************************************/
/*              Includes                               */
/*******************************************************/
#ifdef __cplusplus
extern "C" {
#endif

#include "crules.h"
#include "comedia_keys.h"

#define kSYS_KBD_NB_MAX_CLIENTS      4
#define kSYS_KBD_NO_ID              -1

extern tSYS_KeyEvtCode SYS_AllKeyEvtList[];

/*******************************************************/
/*              Macros                                 */
/*******************************************************/

/*******************************************************/
/*              Typedefs                               */
/*******************************************************/

/*******************************************************/
/*              Variables Declarations (IMPORT)        */
/*******************************************************/

/*******************************************************/
/*              Functions Declarations                 */
/*******************************************************/
 uint8_t SYS_KBD_GetKeyMappingNumber(void);
 void SYS_KBD_GetKeyMapping(uint16_t *TDAL_KBD_KeyCode);

tSYS_KeyEvtCode SYS_KDB_ConvertPhysicalCodeToSYSCode(tSYS_KBD_KeyCode KeyCode);
bool SYS_KDB_CFG_CheckIfWakeUpKey(tSYS_KeyEvtCode KeyCode);

#ifdef __cplusplus
}
#endif


#endif /*_SYS_UI_MNGR_KDB_MAPPING_CFG_*/
