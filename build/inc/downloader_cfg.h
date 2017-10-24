/**************************************************************************//**
** @file downloader_cfg.h
**
** @brief
**   This file defines the configuration of the DOWNLOADER.
**  
**   This component is the interface to the DOWNLOADER configuration
**   customized for the current product.
**
** @ingroup DOWNLOADER
**
** @version $Rev: 9231 $
**          $URL: http://10.50.1.219/svncomedia/idecode/product/comedia/branches/snt_cos/main/build/inc/downloader_cfg.h $
**          $Date: 2012-02-16 18:29:05 +0100 (jeu., 16 févr. 2012) $
**
** @author  SmarDTV Rennes - LIPPA
**
** COPYRIGHT:
**   2011 SmarDTV
**
******************************************************************************/

#ifndef _DOWNLOADER_CFG_H
#define _DOWNLOADER_CFG_H

/******************************************************************************
* Includes
******************************************************************************/

#include "nvm_exchange.h"
#include "nvm_exchange_mapping_cfg.h"
#ifndef __REMOVE_NVMEX__

#include "nvm_interface_bootloader.h"
#include "flash_cfg.h"
#if defined (DLD_BROADCAST)
#include "tdal_dmd.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
* Defines
******************************************************************************/

#define kDLD_CFG_USB_DOWNLOAD_FILE_NAME "mysoft.mux"

/**
 * USB DOWNLOAD AUTOMATIC MODE :
 * A new version on USB is automatically checked. That means we wait a certain 
 * amount of time each time the downloader starts (each reboot) 
 *
#define kDLD_CFG_USB_DOWNLOAD_ENABLE_AUTOMATIC_MODE         1
 *
*/

/**
 * ICS module ID of downloader depend the product generation: factory (initial 
 * version) or regular (update version).
 * 
 * TODO: manage environment variable DOWNLOADER_FACTORY in product.mak and makefile
*/
#ifdef DOWNLOADER_FACTORY
#define kDLD_ICS_MODULE_ID   kNVM_IBTL_ICS_MODID_FACTORY_DOWNLOADER
#else
#define kDLD_ICS_MODULE_ID   kNVM_IBTL_ICS_MODID_DOWNLOADER
#endif

/**
 * 
 * Integration Option: SIMUL FLASH
 * 
 * Uncomment the following define to not flash partition downloaded.
 * 
#define __DLD_OPTION_SIMUL_FLASH_ACCESS__  1
 *
*/


/******************************************************************************
* Macros
******************************************************************************/


/******************************************************************************
* Typedefs
******************************************************************************/

/**
 *  @brief
 *    This type describe downloader right regarding flash partition kind.
 *
*/
typedef enum
{
   eDLD_CFG_PK_UNKNOWN_PARTITION    = 0, /**< Unknown partition: never open */
   eDLD_CFG_PK_CONSTANT_PARTITION   = 1, /**< Read only: boot data, bootstrap, downloader, uboot etc .. */
   eDLD_CFG_PK_BOOT_DATA_PARTITION  = 2, /**< read/write not upgradable: NVM emulation partition  */
   eDLD_CFG_PK_UPGRADABLE_PARTITION = 4, /**< upgradable and checkable: comedia, partition A FS */
   eDLD_CFG_PK_DYNAMIC_PARTITION    = 8  /**< upgradable but not checkable: user file system */
} tDLD_CFG_PartitionKind;


/**
 *  @brief
 *    Progression step to inform user with front panel 
 *    or with a specific application.
 *
*/
typedef enum _DLD_CFG_PROGRESS_STEP_
{
	eDLD_CFG_PSTEP_INIT_STEP=0,
	eDLD_CFG_PSTEP_UPTODATE,
	eDLD_CFG_PSTEP_SERIAL_DETECTED,
	eDLD_CFG_PSTEP_FILE_DETECTED,
	eDLD_CFG_PSTEP_CORRUPTION_DETECTED,
	eDLD_CFG_PSTEP_SERIAL_CONNECTED,
	eDLD_CFG_PSTEP_BROADCAST_CONNECTED,
	eDLD_CFG_PSTEP_TUNER_LOCKED,
	eDLD_CFG_PSTEP_SERIAL_CAROUSEL_IDDENTIFIED,
	eDLD_CFG_PSTEP_DATA_CAROUSEL_IDDENTIFIED,
	eDLD_CFG_PSTEP_OBJECT_CAROUSEL_IDDENTIFIED,
	eDLD_CFG_PSTEP_CAROUSEL_FRAME_RECEIVED, 
	eDLD_CFG_PSTEP_CAROUSEL_LOOP,
 	eDLD_CFG_PSTEP_CAROUSEL_COMPLETED,
	eDLD_CFG_PSTEP_SIGNATURE_GRANTED,
	eDLD_CFG_PSTEP_FLASH_BLOCK_ERASED,
	eDLD_CFG_PSTEP_FLASH_BLOCK_WRITTEN,
	eDLD_CFG_PSTEP_INTEGRITY_CRC_WRITTEN,
	eDLD_CFG_PSTEP_USB_DEVICE_NEEDED,
	eDLD_CFG_PSTEP_DOWNLOAD_COMPLETED,
	eDLD_CFG_PSTEP_DOWNLOAD_FAILURE=0xFE,
	eDLD_CFG_PSTEP_TERM_STEP=0xFF
} tDLD_CFG_ProgressStep;

/**
 *  @brief
 *    TBOX Trace Identifier
 *
*/
typedef enum
{
	eDLD_DLD = 0x04240000,
	eDLD_SERIAL,
	eDLD_DMX,
	eDLD_DMD,
	eDLD_SECTION,
	eDLD_IMAGES,
	eDLD_DSMCC_CARO,
	eDLD_DATA_CARO,
	eDLD_OBJ_CARO,
	eDLD_CRC,
	eDLD_CRYPT,
	eDLD_NTGR,
	eDLD_XDMX,
	eDLD_FLASH,
	eDLD_OSD,
	eDLD_CFG,
	eDLD_ModuleCount
} tDLD_CFG_TracesID;


/******************************************************************************
* Functions
******************************************************************************/

/**
 *  @brief
 *    Initialization downloader configuration management:
 *    - Load the configuration
 *
 *  @retval TRUE
 *       Initialization successful
 *
 *  @retval FALSE
 *       Initialization failure
 *
 *  @remarks
 *    -# if already initialized, log an error but return successful (TRUE).
 *
*/
bool DLD_CFG_Init 
(
   void
);

/**
 *  @brief
 *    Termination downloader configuration management:
 *    - Unload the configuration
 *
 *  @retval TRUE
 *       Termination successful
 *
 *  @retval FALSE
 *       Termination failure
 *
 *  @remarks
 *    -# if not initialized, log an error but return successful (TRUE).
 *
*/
bool DLD_CFG_Term 
(
   void
);

/**
 *  @brief
 *    Return the product IDs.
 *
 *  @param[out] pu32xOUI
 *      OUI value (not returned if pointer undefined).
 *
 *  @param[out] pu16xPlatformModel
 *      Platform model (not returned if pointer undefined).
 *
 *  @param[out] pu16xPlatformVersion
 *      Platform version (not returned if pointer undefined).
 *
 *  @param[out] pu16xProductModel
 *      Product model (not returned if pointer undefined).
 *
 *  @param[out] pu16xProductVersion
 *      Product version (not returned if pointer undefined).
 *
 *  @param[out] pu32xCheckFlags
 *      Indicate the IDS to check (not returned if pointer undefined).
 *
 *  @retval TRUE
 *       Success.
 *
 *  @retval FALSE
 *       Failure
 *
*/
bool DLD_CFG_GetProductIDs
(
   uint32_t *pu32xOUI,
   uint16_t *pu16xPlatformModel,
   uint16_t *pu16xPlatformVersion,
   uint16_t *pu16xProductModel,
   uint16_t *pu16xProductVersion,
   uint32_t *pu32xCheckFlags
);

#if defined (DLD_BROADCAST)
#if defined (MEDIUM_CAB)
/**
 *  @brief
 *    Get default DVB-C location defined in the configuration file.
 *
 *  @param[out] pu32xOUI
 *      OUI value (not returned if pointer undefined).
 *
 *  @param[out] pu16xDsmccPID
 *      DSMCC PID (not returned if pointer undefined).
 *
 *  @param[out] pu32xFrequency
 *      Frequency (not returned if pointer undefined).
 *
 *  @param[out] tTDAL_DMD_CabTunData
 *      Specific tune data cable (not returned if pointer undefined).
 *
 *  @param[out] pi32xModulation
 *      Modulationn (not returned if pointer undefined).
 *
 *  @param[out] pi32xSpecInv
 *      Spectrum (not returned if pointer undefined).
 *
 *  @retval TRUE
 *       Success.
 *
 *  @retval FALSE
 *       Failure
 *
 *  @remarks
 *    -# Use the first DVB-C location found in configuration file.
*/
bool DLD_CFG_GetDefaultLocationDVBC
(
   uint32_t *pu32xOUI,
   uint16_t *pu16xDsmccPID,
   uint32_t *pu32xFrequency,
   tTDAL_DMD_CabTunData *pstxCableTuneData
);
#endif /* MEDIUM_CAB */
#endif /* DLD_BROADCAST */

/**
 *  @brief
 *    Partition Kind identification.
 *    The partition is defined by the flash area type.
 *
 *  @param[in] eArea
 *      Flash area type.
 *
 *  @retval eDLD_CFG_PK_UNKNOWN_PARTITION
 *      Unknown partition: never open.
 *
 *  @retval eDLD_CFG_PK_CONSTANT_PARTITION
 *      Read only: boot data, bootstrap, downloader, uboot...
 *
 *  @retval eDLD_CFG_PK_BOOT_DATA_PARTITION
 *      Read/write not upgradable.
 *
 *  @retval eDLD_CFG_PK_UPGRADABLE_PARTITION
 *      Upgradable and checkable: comedia, partition A FS.
 *
 *  @retval eDLD_CFG_PK_DYNAMIC_PARTITION
 *      Upgradable but not checkable: user file system.
 *
*/
tDLD_CFG_PartitionKind DLD_CFG_GetPartitionKind 
(
   tFLASH_CFG_FLASH_Area exArea
);

/**
 *  @brief
 *    Module Kind identification.
 *    The module (partition) is defined by the ICS module ID.
 *
 *  @param[in] u32xICSModuleId
 *      ICS Module ID.
 *
 *  @param[out] pu8xPriority
 *      Flash update priority (ascendente) (not returned if pointer 
 *      undefined).
 *
 *  @param[out] pu8xNbTry
 *      Flash update try number (not returned if pointer undefined).
 *
 *  @retval eDLD_CFG_PK_UNKNOWN_PARTITION
 *      Unknown partition.
 *
 *  @retval eDLD_CFG_PK_CONSTANT_PARTITION
 *      Read only (factory modules, redundant module)
 *
 *  @retval eDLD_CFG_PK_BOOT_DATA_PARTITION
 *      Read/write not upgradable (Prodcore, dignostic, post-mortem...)
 *
 *  @retval eDLD_CFG_PK_UPGRADABLE_PARTITION
 *      Upgradable and checkable (DA2, config, comedia, partition A)
 *
 *  @retval eDLD_CFG_PK_DYNAMIC_PARTITION
 *      Upgradable but not checkable: (partition B, NVM...)
 *
*/
tDLD_CFG_PartitionKind DLD_CFG_GetIcsModuleKind 
(
   uint32_t u32xICSModuleId,
   uint8_t *pu8xPriority,
   uint8_t *pu8xNbTry
);

/**
 *  @brief
 *    Identify a partition location by its flash address (offset):
 *    - Search the partition
 *    - Verify is upgradable
 *    - Return the flash offset, the update flash priority and
 *      the try number
 *
 *  @param[in] u32xFlashOffset
 *      Flash address: offset since the begining of the flash.
 *
 *  @param[out] pu32xPartitionID
 *      Corresponding product partition ID
 *
 *  @param[out] pu8xPriority
 *      Flash update priority (ascending)
 *
 *  @param[out] pu8xNbTry
 *      Flash update try number
 *
 *  @retval TRUE
 *       Identification OK, it's upgradable.
 *
 *  @retval FALSE
 *       Identification failure (not foudn or not upgradable)
 *
 *  @remarks
 *    -# Accept partition inside a partition expect the DA2, the
 *       flash offset must be equal to the DA2 start address.
*/
bool DLD_CFG_IdentifyPartitionByAddress
(
   uint32_t  u32xFlashOffset,
   uint32_t *pu32xPartitionID,
   uint8_t  *pu8xPriority,
   uint8_t  *pu8xNbTry
);

/**
 *  @brief
 *    Identify a partition location by its partition ID:
 *    - Search the partition
 *    - Verify is upgradable
 *    - Return the flash offset, the update flash priority and
 *      the try number
 *
 *  @param[in] u16xPartitionID
 *      Partition to find.
 *
 *  @param[out] pu32xFlashOffset
 *      Flash address (offset) of this partition.
 *
 *  @param[out] pu8xPriority
 *      Flash update priority (ascending)
 *
 *  @param[out] pu8xNbTry
 *      Flash update try number
 *
 *  @retval TRUE
 *       Identification OK, it's upgradable.
 *
 *  @retval FALSE
 *       Identification failure (not found or not upgradable)
 *
*/
bool DLD_CFG_IdentifyPartitionByID
(
   uint32_t  u32xPartitionID,
   uint32_t *pu32xFlashOffset,
   uint8_t  *pu8xPriority,
   uint8_t  *pu8xNbTry
);

/**
 *  @brief
 *    Verify a flash aera is upgradable.
 *
 *  @param[in] u32xFlashOffset
 *      Flash address: offset since the begining of the flash.
 *
 *  @param[in] u32xSize
 *      Aera size, if equal 0 not signficant, verify only the address.
 *
 *  @retval TRUE
 *       Verify OK, it's upgradable.
 *
 *  @retval FALSE
 *       Verify failure, it's not upgradable.
 *
*/
bool DLD_CFG_IsUpgradable
(
   uint32_t u32xFlashOffset, 
   uint32_t u32xSize
);

/**
 *  @brief
 *    Fill-in NVM exchange Dialog area with default values
 *
 *  @param[out] pstxDialogArea
 *      Dialog area initialized with the default param
 *
 *  @retval eNVM_NO_ERROR
 *       Default initialisation successful.
 *
 *  @retval eNVM_ERROR
 *       Default initialisation failure
 *
 *  @remarks
 *    -# Please DO NOT customise this function.
 *
*/
tNVM_Error DLD_CFG_GetNvmexDefaultParam
(
   tNVM_Dialog_Area *pstxDialogArea
);

#if defined (DLD_USB_FILE)
/**
 *  @brief
 *    Return the file name used for usb download
 *
 *  @retval const uint8_t*
 *       Pointer on file name value
 *
*/
const uint8_t *DLD_CFG_GetUsbDownloadFileName
(
   void
);

/**
 *  @brief
 *    Return the mount point used for usb volume
 *
 *  @param[in] u8xIndx
 *      uIndx the index of the volume (usualy 0)
 *
 *  @retval const uint8_t*
 *       Pointer on mount point value
 *
*/
const uint8_t *DLD_CFG_GetUsbMountPoint
(
   uint8_t u8xIndx
);

/**
 *  @brief
 *    Return the laps needed to detect a usb device is inserted
 *
 *  @retval const uint32_t
 *       Laps in milliseconds
 *
*/
const uint32_t DLD_CFG_GetUsbWaitMs
(
   void
);
#endif /* DLD_USB_FILE */

#if defined (DLD_SERIAL)
/**
 *  @brief
 *    Return the laps PC will respond through serial link
 *
 *  @retval const uint32_t
 *       Laps in milliseconds
 *
*/
const uint32_t DLD_CFG_getSerialWaitMs
(
   void
);
#endif /* DLD_SERIAL */

/**
 *  @brief
 *    This is a notification custom it as you want
 *
 *    Comment/advise :
 *		To get a good graphic rendering progress
 *		you can use 3 bargraphs :
 *		- first one for steps eDLD_CFG_PSTEP_CAROUSEL_FRAME_RECEIVED
 *		- second one for steps eDLD_CFG_PSTEP_FLASH_BLOCK_ERASED
 *		- last one for steps eDLD_CFG_PSTEP_FLASH_BLOCK_WRITTEN
 *		other notification are mainly single event you can display or not as you want
 *		Of course you can merge the 3 main progress step in a single bargraph
 *
 *  @param[in] exStep
 *      The step reached:
 *        eDLD_CFG_PSTEP_INIT_STEP=0,
 *        eDLD_CFG_PSTEP_UPTODATE,
 *        eDLD_CFG_PSTEP_SERIAL_DETECTED,
 *        eDLD_CFG_PSTEP_CORRUPTION_DETECTED,
 *        eDLD_CFG_PSTEP_SERIAL_CONNECTED,
 *        eDLD_CFG_PSTEP_BROADCAST_CONNECTED,
 *        eDLD_CFG_PSTEP_TUNER_LOCKED,
 *        eDLD_CFG_PSTEP_DATA_CAROUSEL_IDDENTIFIED,
 *        eDLD_CFG_PSTEP_OBJECT_CAROUSEL_IDDENTIFIED,
 *        eDLD_CFG_PSTEP_CAROUSEL_FRAME_RECEIVED,
 *        eDLD_CFG_PSTEP_CAROUSEL_LOOP,
 *        eDLD_CFG_PSTEP_SIGNATURE_GRANTED,
 *        eDLD_CFG_PSTEP_FLASH_BLOCK_ERASED,
 *        eDLD_CFG_PSTEP_FLASH_BLOCK_WRITTEN,
 *        eDLD_CFG_PSTEP_INTEGRITY_CRC_WRITTEN,
 *        eDLD_CFG_PSTEP_DOWNLOAD_COMPLETED,
 *        eDLD_CFG_PSTEP_DOWNLOAD_FAILURE=0xFF
 *
 *  @param[in] u32xCount
 *      Counter inside the step
 *
 *  @param[in] u32xTotal
 *      Max value for counter inside the step
 *
*/
void DLD_CFG_NotifyProgress
(
   tDLD_CFG_ProgressStep exStep, 
   uint32_t              u32xCount, 
   uint32_t              u32xTotal
);

/**
 *  @brief
 *    Return TRUE when the forced download has to be launched :
 *    Most of the time, when a fp key is hold down. see DLD_CFGi_FPCallbackFct.
 *
 *  @retval TRUE
 *       Downloader is forced
 *
 *  @retval FALSE
 *       Downloader is not forced
 *
*/
bool DLD_CFG_IsForcedDownloadActivated
(
   void
);

/**
 *  Debug functions for integration
*/
#if defined (__DEBUG_TRACE__)
char *DLD_CFG_DEBUG_LabelPartition 
(
   uint32_t u32xPartitionID
);
#endif

#ifdef __cplusplus
}
#endif
#endif /* __REMOVE_NVMEX__ */

#endif /* _DOWNLOADER_CFG_H */

/* downloader_cfg.h */


