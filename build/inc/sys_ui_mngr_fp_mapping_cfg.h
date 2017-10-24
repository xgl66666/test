/******************************************************************************
 *                         COPYRIGHT 2006 IWEDIA TECHNOLOGIES                         *
 ******************************************************************************
 *
 * MODULE NAME:
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/product/comedia/branches/idecode_hd/build/inc/sys_ui_mngr_fp_mapping_cfg.h $
 *            $Rev: 244 $
 *            $Date: 2011-08-18 18:33:39 +0200 (Thu, 18 Aug 2011) $
 *
 * PUBLIC -
 *
 * DESCRIPTION: 
 *
 *****************************************************************************/
#ifndef _SYS_UI_MNGR_FP_MAPPING_CFG_
#define _SYS_UI_MNGR_FP_MAPPING_CFG_

/*******************************************************/
/*              Includes                               */
/*******************************************************/
#ifdef __cplusplus
extern "C" {
#endif

#include "crules.h"
#include "comedia_keys.h"

extern tSYS_KeyEvtCode SYS_AllKeyEvtList[];

/*******************************************************/
/*              Macros                                 */
/*******************************************************/

/*******************************************************/
/*              Typedefs                               */
/*******************************************************/
typedef uint8_t tSYS_FP_KeyCode;
/*******************************************************/
/*              Variables Declarations (IMPORT)        */
/*******************************************************/

/*******************************************************/
/*              Functions Declarations                 */
/*******************************************************/
tSYS_KeyEvtCode SYS_FP_ConvertPhysicalCodeToSYSCode(tSYS_FP_KeyCode KeyCode);

#ifdef __cplusplus
}
#endif


#endif /*_SYS_UI_MNGR_FP_MAPPING_CFG_*/
