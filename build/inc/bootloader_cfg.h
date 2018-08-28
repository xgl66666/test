/******************************************************************************
 *                    COPYRIGHT 2006 IWEDIA TECHNOLOGIES                      *
 ******************************************************************************
 *
 * MODULE NAME: NVM EXCHANGE
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/product/comedia/branches/idecode_hd/build/inc/bootloader_cfg.h $
 *            $Rev: 332 $
 *            $Date: 2011-09-07 13:51:23 +0200 (Wed, 07 Sep 2011) $
 *
 * PUBLIC
 *
 * DESCRIPTION: bootloader configuration
 *
 *****************************************************************************/

#ifndef _BOOTLOADER_CFG_H_
#define _BOOTLOADER_CFG_H_



#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************/
/*              Includes                               */
/*******************************************************/
#include "nvm_exchange.h"
#include "nvm_exchange_mapping_cfg.h"
#include "flash_cfg.h"
/*
#include "bootloader.h"
*/
#include "version_cfg.h"


/********************************************************/
/*              Defines                                 */
/********************************************************/

#define kBTL_CFG_USB_DOWNLOAD_FILE_NAME                 "mysoft.mux"

/* USB DOWNLOAD AUTOMATIC MODE :                */
/* a new version on usb is automatically checked    */
/* that means we wait a certain amount of time each time the bootloader starts (each reboot) */
/*#define kBTL_CFG_USB_DOWNLOAD_ENABLE_AUTOMATIC_MODE         1*/


/*******************************************************/
/*               typedef                               */
/*******************************************************/

/* type tBTL_PartitionKind describe bootloader right regarding flash partition kind */
typedef enum
{
	eBTL_UNKNOWN_PARTITION=0,	/* Unknown: never open */	
	eBTL_CONSTANT_PARTITION=1,	/* Read only: boot data, bootstrap, bootloader, uboot etc ..*/
	eBTL_BOOT_DATA_PARTITION=2, /* read/write not upgradable: NVM emulation partition */
	eBTL_UPGRADABLE_PARTITION=4,/* upgradable and checkable: comedia, partition A FS */
	eBTL_DYNAMIC_PARTITION=8 	/* upgradable but not checkable: user file system */
} tBTL_PartitionKind;


typedef enum _BTL_CFG_PROGRESS_STEP_
{
	eBTL_CFG_INIT_STEP=0,
	eBTL_CFG_UPTODATE,
	eBTL_CFG_SERIAL_DETECTED,
	eBTL_CFG_FILE_DETECTED,
	eBTL_CFG_CORRUPTION_DETECTED,
	eBTL_CFG_SERIAL_CONNECTED,
	eBTL_CFG_BROADCAST_CONNECTED,
	eBTL_CFG_TUNER_LOCKED,
	eBTL_CFG_SERIAL_CAROUSEL_IDDENTIFIED,
	eBTL_CFG_DATA_CAROUSEL_IDDENTIFIED,
	eBTL_CFG_OBJECT_CAROUSEL_IDDENTIFIED,
	eBTL_CFG_CAROUSEL_FRAME_RECEIVED, 
	eBTL_CFG_CAROUSEL_LOOP,
 	eBTL_CFG_CAROUSEL_COMPLETED,
	eBTL_CFG_SIGNATURE_GRANTED,
	eBTL_CFG_FLASH_BLOCK_ERASED,
	eBTL_CFG_FLASH_BLOCK_WRITTEN,
	eBTL_CFG_INTEGRITY_CRC_WRITTEN,
	eBTL_CFG_USB_DEVICE_NEEDED,
	eBTL_CFG_DOWNLOAD_COMPLETED,
	eBTL_CFG_DOWNLOAD_FAILURE=0xFF
} tBTL_CFG_ProgressStep;


/*******************************************************/
/*              Traces                                 */
/*******************************************************/
typedef enum
{
	eBTL_BTL     	    = 0x04240000,
	eBTL_SERIAL,
	eBTL_DMX,
	eBTL_SECTION,
	eBTL_IWE_CAROUSEL,
	eBTL_DSMCC_CAROUSEL,
	eBTL_DATA_CAROUSEL,
	eBTL_OBJECT_CAROUSEL,
	eBTL_CRC,
	eBTL_CRYPT,
	eBTL_NTGR,
	eBTL_XDMX
}tBTL_TracesID;
	

/*******************************************************/
/*              Functions Declarations                 */
/*******************************************************/
extern tNVM_Error BTL_CFG_GetNvmexDefaultParam(tNVM_Dialog_Area    *pDialogArea);
extern const uint8_t *BTL_CFG_GetUsbDownloadFileName(void);
extern const uint8_t *BTL_CFG_GetUsbMountPoint(uint8_t uIndx);
extern const uint32_t BTL_CFG_getUsbWaitMs(void);
extern const uint32_t BTL_CFG_getSerialWaitMs(void);
extern tBTL_PartitionKind BTL_CFG_GetPartitionKind(tFLASH_CFG_FLASH_Area eArea);
extern void BTL_CFG_NotifyProgress(tBTL_CFG_ProgressStep eStep, uint32_t uCount, uint32_t uTotal);

#ifdef __cplusplus
}
#endif

#endif /* _BOOTLOADER_CFG_H_ */
