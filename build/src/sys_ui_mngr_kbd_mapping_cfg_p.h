/******************************************************************************
 *                         COPYRIGHT 2005 IWEDIA TECHNOLOGIES                         *
 ******************************************************************************
 *
 * MODULE NAME:
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/product/comedia/branches/idecode_hd/build/src/sys_ui_mngr_kbd_mapping_cfg_p.h $
 * Header of the System Manager
 *            $Rev: 244 $
 *            $Date: 2011-08-18 18:33:39 +0200 (Thu, 18 Aug 2011) $
 *
 * PUBLIC -
 *
 * DESCRIPTION: this file contain all RC5 code of remote control supported
 *
 *****************************************************************************/
#ifndef _SYS_UI_MNGR_KDB_MAPPING_CFG_P_
#define _SYS_UI_MNGR_KDB_MAPPING_CFG_P_

/*******************************************************/
/*              Includes                               */
/*******************************************************/
#ifdef __cplusplus
extern "C" {
#endif

#define SYS_CODE_NUMBER 65
#define NO_PHYSICAL_CODE 0xFFFFFFFF

/*
 * Standard architecture (not RPC), key mapping depends of the driver.
 */
#ifdef KBD_PC
#include "kbd_cfg/kbd_mapping_pc_cfg_p.h"

#elif KBD_SMARDTV
#include "kbd_cfg/kbd_mapping_smardtv_cfg_p.h"

#elif defined(KBD_MSTAR)
#include "kbd_cfg/kbd_mapping_mstar_cfg_p.h"

#elif defined(KBD_NEWLAND)
#include "kbd_cfg/kbd_mapping_newland_cfg_p.h"

#elif defined(KBD_GTPL) && defined(NEWLAND_BOOTLOADER)
#include "kbd_cfg/kbd_mapping_gtpl_newland_cfg_p.h"

#elif defined(KBD_GTPL) && defined(TELESYSTEM_BOOTLOADER)
#include "kbd_cfg/kbd_mapping_gtpl_telesystem_cfg_p.h"

#elif defined(KBD_DEN_SKYWORTH)
#include "kbd_cfg/kbd_mapping_den_skyworth_cfg_p.h"

#elif defined(KBD_GHITS) && defined(SKYWORTH_BOOTLOADER)
#include "kbd_cfg/kbd_mapping_ghits_skyworth_cfg_p.h"

#elif defined(KBD_GHITS) && defined(CHANGHONG_BOOTLOADER)
#include "kbd_cfg/kbd_mapping_ghits_changhong_cfg_p.h"

#else
#include "kbd_cfg/kbd_mapping_default_cfg_p.h"

#endif


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

#ifdef __cplusplus
}
#endif


#endif /*_SYS_UI_MNGR_KDB_MAPPING_CFG_P_*/
