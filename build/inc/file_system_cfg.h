/**************************************************************************//**
** @file file_system_cfg.h
**
** @brief
**   File System Configuration:
**   - Define the properties of each partitions constituting the file system
**     of the product
**   - Mount and Unmount the files system. 
**
** @ingroup Product CONFIGURATION
**
** @see flash configuration (module flash_cfg) and Data Manager JFS (pack
**  
** @see Data Manager JFS (package dmjfs)
**  
** @version $Rev: 9410 $
**          $URL: http://10.50.1.219/svncomedia/idecode/product/comedia/branches/snt_imcl_dlk_312B/build/inc/file_system_cfg.h $
**          $Date: 2012-02-28 17:35:47 +0100 (mar., 28 févr. 2012) $
**
** @author  SmarDTV Rennes - LIPPA
**
** COPYRIGHT:
**   2011 SmarDTV
**
******************************************************************************/

#ifndef _FILE_SYSTEM_CFG_H
#define _FILE_SYSTEM_CFG_H

/******************************************************************************
* Includes
******************************************************************************/

#include "dmjfs.h"


#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
* Defines
******************************************************************************/

/**
 *  @brief
 *    Partitions Device ID of Product File System
 *
*/
#ifndef LOADER_EMBEDDED
#ifdef PRODUCT_USE_MAESTRO
#define kFS_CFG_DEV_COMEDIA_APPL  'A'  /**< Comedia XML application (part A)  */
#define kFS_CFG_PART_A            kFS_CFG_DEV_COMEDIA_APPL
#endif
#endif

#define kFS_CFG_DEV_COMEDIA_DATA  'B'  /**< Comedia Data (part B)             */
#define kFS_CFG_PART_B            kFS_CFG_DEV_COMEDIA_DATA

#define kFS_CFG_DEV_EXTERNAL_DATA 'C'
#define kFS_CFG_PART_C            kFS_CFG_DEV_EXTERNAL_DATA


#ifdef CAK_DLK
#define kFS_CFG_DEV_CAK_DLK_DATA  'N'  /**< CAK DLK Data (part N)             */
#define kFS_CFG_PART_N            kFS_CFG_DEV_CAK_DLK_DATA
#endif

/**
 *
 *  @brief
 *    Comedia Directory => Comedia path to cat on prefix of file
 *
 *    If no Comedia directory => Undefine the define
*/
#define kFS_CFG_CM_DIRNAME  "/Comedia"

#ifdef kFS_CFG_CM_DIRNAME
#define kFS_CFG_CM_PATH     kFS_CFG_CM_DIRNAME"/"
#else
#define kFS_CFG_CM_PATH     ""
#endif


/******************************************************************************
* Macros
******************************************************************************/

/******************************************************************************
* Typedefs
******************************************************************************/

typedef enum
{
   eFS_CFG = 0x042DF0CF
} tFS_CFG_TracesID;


/******************************************************************************
* Functions
******************************************************************************/

/**
 *  @brief
 *    Mount the complete product Files System:
 *    - Initialize the Data Manager JFS
 *    - Mount all partitions of Files System
 *
 *  @param[in]     xbResetPartComedia
 *       TRUE to reset the comedia partition (partition B)
 *       If There is an Alias (exemple the DLK partition N) on the partition B,
 *       process only the clean files of the Comedia Partition (= del tree of
 *       COmedia Directory)
 *
 *  @param[in]     xbRetryPartComediaWithReset
 *       TRUE to retry with a reset if the mount is failed (significant
 *       on partition B if reset not set TRUE).
 *
 *  @param[in]     cbCriticalError
 *       Client callback to notify the critical error.
 *
 *  @retval  TRUE
 *       Mount File System successful
 *
 *  @retval  FALSE
 *       Mount File System failure
 *
*/
bool FS_CFG_MountFilesSystem
(
   bool                    xbResetPartComedia,
   bool                    xbRetryPartComediaWithReset,
   tDMJFS_CriticalErrorFct cbCriticalError
);


/**
 *  @brief
 *    Unmount the product File System:
 *    - Unmount all partitions of Files System
 *    - If Data Manager JFS initialized at mounting desinitialize it
 *
 *  @retval  TRUE
 *       Unmount File System successful
 *
 *  @retval  FALSE
 *       Unmount File System failure
 *
*/
bool FS_CFG_UnmountFilesSystem
(
   void
);


/**
 *
 *  BASIC INIT, MOUNT, UNMOUNT & TERM FUNCTIONS
 *  -------------------------------------------
 *
 *  Not use with previous functions.
*/

/**
 *  @brief
 *    Initialize the product File System management: Initialize the Data 
 *    Manager JFS if not initilized
 *
 *  @param[in]     cbCriticalError
 *       Client callback to notify the critical error.
 *
 *  @retval  TRUE
 *       Initialization successful
 *
 *  @retval  FALSE
 *       Initialization failure
 *
 *  @remarks
 *    -# For use with the functions of mount/unmout partition by partition: 
 *       FS_CFG_Mount(part) and FS_CFG_Unmount(part). When using files system 
 *       is complete use the function FS_CFG_Term()
 *
*/
bool FS_CFG_Init 
(
   tDMJFS_CriticalErrorFct cbCriticalError
);

/**
 *  @brief
 *    Terminate the product File System management: If Data Manager JFS 
 *    initialized at initialization desinitialize it
 *
 *  @retval  TRUE
 *       Termination successful
 *
 *  @retval  FALSE
 *       Termination failure
 *
 *  @remarks
 *    -# For use with the functions of mount/unmout partition by partition: 
 *       FS_CFG_Mount(part) and FS_CFG_Unmount(part). Use it after 
 *       Initialization by FS_CFG_Init().
 *
*/
bool FS_CFG_Term
(
   void
);

/**
 *  @brief
 *    Mount a partition
 *
 *  @param[in]     xcDevice
 *       Device of partition to mount
 *
 *  @param[in]     xbReset
 *       TRUE to reset the partition
 *
 *  @retval  TRUE
 *       Mount successful
 *
 *  @retval  FALSE
 *       Mount failure
 *
*/
bool FS_CFG_Mount
(
   char xcDevice,
   bool xbReset
);

/**
 *  @brief
 *    Unmount a partition
 *
 *  @param[in]     xcDevice
 *       Device of partition to mount
 *
 *  @param[in]     xbReset
 *       TRUE to reset the partition
 *
 *  @retval  TRUE
 *       Mount successful
 *
 *  @retval  FALSE
 *       Mount failure
 *
*/
bool FS_CFG_Unmount
(
   char xcDevice
);

/**
 *  @brief
 *    Reinitialize a partition: with a umount and a mount
 *    in mode reset to erase all data and control block.
 *
 *    If the partition is not mounted after reset umount the partition.
 *
 *    If the partition is a Alias on another parition, the reset is refused
 *    and a clean is processed.
 *
 *  @param[in]     xcDevice
 *       Device of partition to clean
 *
 *  @retval  TRUE
 *       Reset successful
 *
 *  @retval  FALSE
 *       Reset failure
 *
*/
bool FS_CFG_Reset
(
   char xcDevice
);

/**
 *  @brief
 *    Clean a partition: Delete all files.
 *
 *    Note: if the partition is not an alias and have not a alias 
 *          process a reset.
 *
 *  @param[in]     xcDevice
 *       Device of partition to clean
 *
 *  @retval  TRUE
 *       Clean successful
 *
 *  @retval  FALSE
 *       Clean failure
 *
*/
bool FS_CFG_CleanFiles
(
   char xcDevice
);

#ifdef __cplusplus
}
#endif

#endif /* _FILE_SYSTEM_CFG_H */

/* file_system_cfg.h */

