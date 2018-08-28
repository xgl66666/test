/******************************************************************************
 *                    COPYRIGHT 2006 IWEDIA TECHNOLOGIES                      *
 ******************************************************************************
 *
 * MODULE NAME: 
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/product/comedia/branches/idecode_hd/build/inc/integrity_cfg.h $
 *            $Rev: 332 $
 *            $Date: 2011-09-07 13:51:23 +0200 (Wed, 07 Sep 2011) $
 *
 * PUBLIC
 *
 * DESCRIPTION:
 *
 *****************************************************************************/

#ifndef _INTEGRITY_CFG_H_
#define _INTEGRITY_CFG_H_



#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************/
/*              Includes                               */
/*******************************************************/

#include "bootloader.h"
#include "bootloader_cfg.h"
#include "tkel.h"
#include "tbox.h"
#include "tdal_fla.h"
#include "tdal_nvm.h"
#include "flash_cfg.h"
#include "btl_crc.h"

typedef struct
{
     uint32_t   startAddress;   /* strart address of one module in flash */
     uint32_t   moduleSize;     /* size of the expected module */
     uint32_t   CRC32;          /* crc32 of the expected module */
     uint32_t   filler1;            /* to align the structure */
} NTGR_module_t;


typedef struct
{
     uint32_t        nbOfModules;
     uint32_t       filler12[3];    /* to align next field */
    NTGR_module_t    moduleInfoArray[kNTGR_MAX_CHECKED_FLASH_BLOCK_COUNT];
   uint32_t        CRC32;
} NTGR_ssw_info_t;

#define kNTGR_MODULE_INFO_SIZE	sizeof(NTGR_module_t)
#define kNTGR_SSW_PAYLOAD_SIZE 	(kNTGR_SSW_AREA_SIZE-4) /* less sizeof CRC32 field */

#define kNTGR_SSW_NB_MOD_BASE          0
#define kNTGR_SSW_NB_MOD_SIZE          4 + 12 /* align filler */

#define kNTGR_SSW_ADDR_MOD_BASE        (kNTGR_SSW_NB_MOD_BASE + kNTGR_SSW_NB_MOD_SIZE)
#define kNTGR_SSW_ADDR_MOD_SIZE        4

#define kNTGR_SSW_SIZE_MOD_BASE        (kNTGR_SSW_ADDR_MOD_BASE + kNTGR_SSW_ADDR_MOD_SIZE)
#define kNTGR_SSW_SIZE_MOD_SIZE        4

#define kNTGR_SSW_CRC32_MOD_BASE       (kNTGR_SSW_SIZE_MOD_BASE + kNTGR_SSW_SIZE_MOD_SIZE)
#define kNTGR_SSW_CRC32_MOD_SIZE       4 + 4  /* align filler */

typedef enum {
           NTGR_NO_ERROR =BTL_NO_ERROR,
           NTGR_FLASH_CRC_FAILED =BTL_FLASH_CRC_FAILED,
           NTGR_MEMORY_ALLOC_FAILED =BTL_MEMORY_ALLOC_FAILED,
           NTGR_FLASH_READ_FAILED =BTL_FLASH_READ_FAILED,
           NTGR_FAILED=BTL_FAILED
    } NTGR_error_code_t;
    
LOCAL NTGR_error_code_t NTGR_UT_ReadSSWInfo(NTGR_ssw_info_t *psSwInfo);

NTGR_error_code_t NTGR_CheckIfFirstSSW(void);
NTGR_error_code_t NTGR_CheckSSWIntegrity(bool *pbSswAvailable);
NTGR_error_code_t NTGR_ComputeAndWriteSSWInfo(void);


#ifdef __cplusplus
}
#endif

#endif /* _INTEGRITY_CFG_H_ */
