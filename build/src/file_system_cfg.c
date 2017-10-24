/**************************************************************************//**
** @file file_system_cfg.c
**
** @brief
**   File System Configuration:
**   - Define the properties of each partitions constituting the file system
**     of the product
**   - Mount and Unmount the file system. 
**
** @ingroup Product CONFIGURATION
**
** @see flash configuration (module flash_cfg) and Data Manager JFS (pack
**  
** @see Data Manager JFS (package dmjfs)
**  
** @version $Rev: 9410 $
**          $URL: http://10.50.1.219/svncomedia/idecode/product/comedia/branches/snt_imcl_dlk_312B/build/src/file_system_cfg.c $
**          $Date: 2012-02-28 17:35:47 +0100 (mar., 28 févr. 2012) $
**
** @author  SmarDTV Rennes - LIPPA
**
** COPYRIGHT:
**   2011 SmarDTV
**
******************************************************************************/

/******************************************************************************
* Includes
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "crules.h"
#include "tbox.h"
#include "tkel.h"

#include "flash_cfg.h"
#include "file_system_cfg.h"

#include "zlib.h"
#include "msbf_tools.h"
#include "tdal_fla.h"
#include "tdal_nvm.h"
#include "tdal_fs.h"



mTBOX_SET_MODULE(eFS_CFG);


/******************************************************************************
* Defines
******************************************************************************/

/**
 *  @brief
 *    Comedia Application Partition (partition A, ressources application XML)
 *    properties.
 *
 *    It's a physical partition (not an alias) type resources (not 
 *    upgradable created a once for all and used in read only mode)
 *
 *    The maximum control data size must be defined, the standard size is 
 *    16 Kbytes (0x4000) but is bigger on IP box (use the double)
 *
 *    The control data is unique in first block of data 
 *    => No definition of control blocks partition
 *
 *    In product managed by a bootloader this partition is protected by 
 *    security data (from Nagra here) and start by a the security header
 *    (80 bytes with Nagra header)
 *      
*/
#define kFS_CFGi_PROPS_PART_A_SZMAX_CDATA       0x8000

#define kFS_CFGi_PROPS_PART_A_CRCFILE_ENABLE    FALSE
#define kFS_CFGi_PROPS_PART_A_REC_CDATA_NRETRY  0
#define kFS_CFGi_PROPS_PART_A_REC_FILE_NRETRY   0
#define kFS_CFGi_PROPS_PART_A_WRI_CDATA_NRETRY  0
#define kFS_CFGi_PROPS_PART_A_WRI_FILE_NRETRY   0
#define kFS_CFGi_PROPS_PART_A_DATA_FAREA        eFLASH_CFG_FLASH_AREA_PARTITION_A

#define kFS_CFGi_PROPS_PART_A_DATA_SZ_HEAD      0x50

#define kFS_CFGi_PROPS_PART_A_DATA_MIN_BLOCK    1
#define kFS_CFGi_PROPS_PART_A_CTRL_FAREA        eFLASH_CFG_FLASH_AREA_UNDEFINED
#define kFS_CFGi_PROPS_PART_A_CTRL_SZ_HEAD      0x00
#define kFS_CFGi_PROPS_PART_A_CTRL_MIN_BLOCK    1
#define kFS_CFGi_PROPS_PART_A_MAX_FILES         250
#define kFS_CFGi_PROPS_PART_A_MAX_DIRS          0

/**
 *  @brief
 *    Comedia Data (partition B) properties.
 *    It's a physical partition (not an alias) standard on 2 flash
 *    partitions (data blocks and control blocks).
 *
 *    In product managed by a bootloader theses partitions are not protected by 
 *    security data => No security header.
 *      
*/
#define kFS_CFGi_PROPS_PART_B_CRCFILE_ENABLE    TRUE
#define kFS_CFGi_PROPS_PART_B_REC_CDATA_NRETRY  3
#define kFS_CFGi_PROPS_PART_B_REC_FILE_NRETRY   1
#define kFS_CFGi_PROPS_PART_B_WRI_CDATA_NRETRY  3
#define kFS_CFGi_PROPS_PART_B_WRI_FILE_NRETRY   0
#define kFS_CFGi_PROPS_PART_B_DATA_FAREA        eFLASH_CFG_FLASH_AREA_PARTITION_B
#define kFS_CFGi_PROPS_PART_B_DATA_SZ_HEAD      0x00
#define kFS_CFGi_PROPS_PART_B_DATA_MIN_BLOCK    2
#define kFS_CFGi_PROPS_PART_B_CTRL_FAREA        eFLASH_CFG_FLASH_AREA_PARTITION_B_CTRLBLOCK
#define kFS_CFGi_PROPS_PART_B_CTRL_SZ_HEAD      0x00
#define kFS_CFGi_PROPS_PART_B_CTRL_MIN_BLOCK    1
#define kFS_CFGi_PROPS_PART_B_MAX_FILES         250
#define kFS_CFGi_PROPS_PART_B_MAX_DIRS          50

/**
 *  @brief
 *    Extern Data (partition C) properties.
 *    It's a physical partition (not an alias) standard on 2 flash
 *    partitions (data blocks and control blocks).
 *
 *    In product managed by a bootloader theses partitions are not protected by 
 *    security data => No security header.
 *      
*/
#define kFS_CFGi_PROPS_PART_C_CRCFILE_ENABLE    TRUE
#define kFS_CFGi_PROPS_PART_C_REC_CDATA_NRETRY  3
#define kFS_CFGi_PROPS_PART_C_REC_FILE_NRETRY   1
#define kFS_CFGi_PROPS_PART_C_WRI_CDATA_NRETRY  3
#define kFS_CFGi_PROPS_PART_C_WRI_FILE_NRETRY   0
#define kFS_CFGi_PROPS_PART_C_DATA_FAREA        eFLASH_CFG_FLASH_AREA_PARTITION_C
#define kFS_CFGi_PROPS_PART_C_DATA_SZ_HEAD      0x10
#define kFS_CFGi_PROPS_PART_C_DATA_MIN_BLOCK    2
#define kFS_CFGi_PROPS_PART_C_CTRL_FAREA        eFLASH_CFG_FLASH_AREA_PARTITION_C_CTRLBLOCK
#define kFS_CFGi_PROPS_PART_C_CTRL_SZ_HEAD      0x10
#define kFS_CFGi_PROPS_PART_C_CTRL_MIN_BLOCK    1
#define kFS_CFGi_PROPS_PART_C_MAX_FILES         250
#define kFS_CFGi_PROPS_PART_C_MAX_DIRS          50


/**
 *  @brief
 *    CAK DLK Data (partition N) properties.
 *      
*/
#ifdef kFS_CFG_PART_N
#if defined (eFLASH_CFG_FLASH_AREA_PARTITION_N) && defined (eFLASH_CFG_FLASH_AREA_PARTITION_N_CTRLBLOCK)
#define kFS_CFGi_PROPS_PART_N_CRCFILE_ENABLE    TRUE
#define kFS_CFGi_PROPS_PART_N_REC_CDATA_NRETRY  3
#define kFS_CFGi_PROPS_PART_N_REC_FILE_NRETRY   1
#define kFS_CFGi_PROPS_PART_N_WRI_CDATA_NRETRY  3
#define kFS_CFGi_PROPS_PART_N_WRI_FILE_NRETRY   0
#define kFS_CFGi_PROPS_PART_N_DATA_FAREA        eFLASH_CFG_FLASH_AREA_PARTITION_N
#define kFS_CFGi_PROPS_PART_N_DATA_SZ_HEAD      0x00
#define kFS_CFGi_PROPS_PART_N_DATA_MIN_BLOCK    4
#define kFS_CFGi_PROPS_PART_N_CTRL_FAREA        eFLASH_CFG_FLASH_AREA_PARTITION_N_CTRLBLOCK
#define kFS_CFGi_PROPS_PART_N_CTRL_SZ_HEAD      0x08
#define kFS_CFGi_PROPS_PART_N_CTRL_MIN_BLOCK    1
#define kFS_CFGi_PROPS_PART_N_MAX_FILES         625
#define kFS_CFGi_PROPS_PART_N_MAX_DIRS          100
#else
/* N is an Alias on B => Increases the max file */
#define kFS_CFGi_PROPS_PART_N_ALIAS_ON        kFS_CFG_PART_B
#undef  kFS_CFGi_PROPS_PART_B_MAX_FILES
#undef  kFS_CFGi_PROPS_PART_B_MAX_DIRS
#define kFS_CFGi_PROPS_PART_B_MAX_FILES         750
#define kFS_CFGi_PROPS_PART_B_MAX_DIRS          350
#endif
#endif

/**
 *  @brief
 *    Limit of file and directory open
 *      
*/
#ifdef kFS_CFG_PART_N
/* With DLK partition */
#define kFS_CFGi_MAX_FILES_OPEN          600
#define kFS_CFGi_MAX_FILES_OPEN_INWRITE  600
#define kFS_CFGi_MAX_DIR_OPEN             50
#else
/* Partition A and B => Comedia Historical Canel limits */
#define kFS_CFGi_MAX_FILES_OPEN          250
#ifdef __CAK_DAL_TESTU_DATA_MNGR__
#define kFS_CFGi_MAX_FILES_OPEN_INWRITE   16
#else
#define kFS_CFGi_MAX_FILES_OPEN_INWRITE  100
#endif
#define kFS_CFGi_MAX_DIR_OPEN             50
#endif

#if (!defined(WIN32)||defined(PRODUCT_PC_SIM)) && defined(COMEDIA_RESOURCES_IN_RAM)
#define kFS_CFG_PART_A_IN_RAM
#endif

#ifdef kFS_CFG_PART_A_IN_RAM
#define FS_CFG_RAMPARTITIONA_SIZE 16*128*1024

//#ifdef COMEDIA_BIN

/* Comedia is generated to run in Flash with bootloader support -> Gets the partitionA from RAM -> Will be placed by linker at a fixed address and filled through downloader */
//static const unsigned char FS_CFGi_RAMPartitionA[FS_CFG_RAMPARTITIONA_SIZE]__attribute__((section(".FS_CFGi_RAMPartitionA")));
//#else

/* Comedia is generated to run without bootloader -> Gets the partitionA from RAM C file -> 
    If comedia is compiled for DCU run -> Will be placed by linker at a random address
    If comedia is compiled for Flash run -> Will be placed by linker at a ramdom address "also"
        */
#include "fs_cfg_myRAMPartitionA.h"
//#endif
#endif

/**
 *  @brief
 *    INTERGRATION OPTIONS:
 *    --------------------
 *
 *    - To force a reset of a partition, undefine next define
 * 
#define kDMJFS_PARTi_FORCE_RESET_PART_B  1
#define kDMJFS_PARTi_FORCE_RESET_PART_N  1
 *
 */

#ifdef CAK_DLK
#ifdef NEWLAND_MERLIN_CONVERT

#if defined(IMCL)
#define kFS_CFG_PARTITION_N_START               (0x730000)
#define kFS_CFG_PARTITION_N_START_ALTERNATIVE   (0x770000)
#define kFS_CFG_PARTITION_N_SIZE                (0x040000)
#define kFS_CFG_FLA_MAX_FILE_NAME (64)
#elif defined(GTPL)
#define kFS_CFG_PARTITION_N_START_PART1			(0x740000)
#define kFS_CFG_PARTITION_N_START_PART2         (0x600000)
#define kFS_CFG_PARTITION_N_START_PART1_ALTER   (0x770000)
#define kFS_CFG_PARTITION_N_START_PART2_ALTER   (0x610000)
#define kFS_CFG_PARTITION_N_SIZE                (0x040000)
#define kFS_CFG_FLA_MAX_FILE_NAME (64)
#endif
#endif
#endif

/******************************************************************************
* Macros
******************************************************************************/


/* Boolean Label */
#define mFS_CFG_STRBOOL(b) (b)?"TRUE":"False"
#define mFS_CFG_GETSTR(s)  ((s)==NULL)?"<null>":(s)

/******************************************************************************
* Typedefs
******************************************************************************/


/**
 *  @brief
 *    Product File System Partitions list.
 *
*/
typedef enum
{
#ifdef kFS_CFG_PART_A
   eFS_CFG_PART_COMEDIA_APPL,       /**< Comedia XML application (part A)    */
#endif
#ifdef kFS_CFG_PART_B
   eFS_CFG_PART_COMEDIA_DATA,       /**< Comedia Data (part B)               */
#endif
#ifdef kFS_CFG_PART_C
   eFS_CFG_PART_EXTERN_DATA,       /**< Additianl resources (part C)               */
#endif
#ifdef kFS_CFG_PART_N
   eFS_CFG_PART_CAK_DLK,            /**< CAK DLK Data (part N)               */
#endif
   eFS_CFG_PART_NUMBER
} tFS_CFG_PartitionId;


/**
 *  @brief
 *    File System Partitions Properties.
 *
*/
typedef struct
{
   char     cDevice;                /**< Partition Device ID (use printable 
                                           char 'A', 'B'...,'Z')             */
   char     cAliasToDevice;         /**< If Alias, Device of partition target
                                         Else use kDMJFS_PART_DEVICE_UNDEF   */
   /**< General Properties (significants if not an alias)                    */
   bool     bIsRessources;          /**< TRUE if ressources partition
                                         FALSE if standard (data) partition  */
   uint32_t ui32MaxSizeCData;       /**< Maximum size of control data
                                         (significant only if ressources)    */
   bool     bIsEnableFileCRC;       /**< TRUE to check CRC files at mount    */
   uint8_t  ui8RecRetryLoadCD;      /**< Retry num. in recovering Ctrl Data  */
   uint8_t  ui8RecRetryLoadFile;    /**< Retry number in recovering File     */
   uint8_t  ui8NumRetrySaveCD;      /**< Retry number on write Ctrl Data     */
   uint8_t  ui8NumRetrySaveFile;    /**< Retry number on write File          */
   /**< Data Blocks properties (significants if not an alias)                */
   uint32_t ui32DataFlashAreaID;    /**< Flash Area ID of data blocks (from 
                                         flash_cfg.h)                        */
   uint32_t ui32DataSizePartHeader; /**< Size of partition header in bytes 
                                         (only on first block)               */
   uint8_t  ui16DataMinNumBlocks;    /**< Minimum number of data blocks       */
   /**< Data Blocks properties (significants if not an alias)                */
   uint32_t ui32CtrlFlashAreaID;    /**< Flash Area ID of control blocks 
                                         (from flash_cfg.h). If undef (value 
                                         eFLASH_CFG_FLASH_AREA_UNDEFINED) use
                                         first block of Data                 */
   uint32_t ui32CtrlSizePartHeader; /**< Size of partition header in bytes 
                                         (only on first block)               */
   uint8_t  ui16CtrlMinNumBlocks;    /**< Minimum number of data blocks       */
   uint16_t ui16MaxNumFiles;        /**< Maximum number of files             */
   uint16_t ui16MaxNumDir;          /**< Maximum number of directory         */
#ifdef __DEBUG_TRACE__
   char    *pstrLabel;
#endif
} tFS_CFG_PartitionProps;


#ifdef CAK_DLK
#ifdef NEWLAND_MERLIN_CONVERT

typedef uint8_t tFS_CFGi_FileName[kFS_CFG_FLA_MAX_FILE_NAME];

typedef struct FS_CFGi_File_s
{
    tFS_CFGi_FileName    file_name;   /*file name..*/
    uint32_t             file_size ;  /*show file size.*/
    uint8_t              *file_data ; /*point to file data*/
    uint32_t             crc32;       /*CRC32, include file name to end of file data.*/
}tFS_CFGi_File;

#endif
#endif
/******************************************************************************
* Locales Variables
******************************************************************************/

/**
 *  @brief
 *    TRUE when FS config data initialized
*/
LOCAL bool vFS_CFGi_bIsInit = FALSE;


/**
 *  @brief
 *    TRUE when DM JFS Manager initialized in this module
 *    => Termination at FS Unmounting.
*/
LOCAL bool vFS_CFGi_bIsInitHereDMJFS = FALSE;


/**
 *  @brief
 *    Table of File System Partition properties.
 *    Use product define kCFG_CFGi_PROPS_PART_X_xxxx
*/
LOCAL tFS_CFG_PartitionProps vFS_CFGi_tstPartitionsProps[eFS_CFG_PART_NUMBER] =
{
#ifdef kFS_CFG_PART_A
   /* Comedia Application Partition (partition A, ressources appli. XML)      */
   {
      kFS_CFG_PART_A,                         /* Partition Device ID          */
      kDMJFS_PART_DEVICE_UNDEF,               /* Not an Alias                 */
      TRUE,                                   /* Resource partition           */
      kFS_CFGi_PROPS_PART_A_SZMAX_CDATA,      /* Maximum size of control data */
      kFS_CFGi_PROPS_PART_A_CRCFILE_ENABLE,   /* Check CRC file at mount      */
      kFS_CFGi_PROPS_PART_A_REC_CDATA_NRETRY, /* Retry num. recovering CData  */
      kFS_CFGi_PROPS_PART_A_REC_FILE_NRETRY,  /* Retry num. recovering File   */
      kFS_CFGi_PROPS_PART_A_WRI_CDATA_NRETRY, /* Retry num. write CData       */
      kFS_CFGi_PROPS_PART_A_WRI_FILE_NRETRY,  /* Retry num. write File        */
      kFS_CFGi_PROPS_PART_A_DATA_FAREA,       /* Flash Area ID of data blocks */
      kFS_CFGi_PROPS_PART_A_DATA_SZ_HEAD,     /* Size of partition header     */
      kFS_CFGi_PROPS_PART_A_DATA_MIN_BLOCK,   /* Mini number of data blocks   */
      kFS_CFGi_PROPS_PART_A_CTRL_FAREA,       /* Flash Area ID of ctrl blocks */
      kFS_CFGi_PROPS_PART_A_CTRL_SZ_HEAD,     /* Size of partition header     */
      kFS_CFGi_PROPS_PART_A_CTRL_MIN_BLOCK,   /* Mini number of ctrl blocks   */
      kFS_CFGi_PROPS_PART_A_MAX_FILES,        /* Max number of files          */
      kFS_CFGi_PROPS_PART_A_MAX_DIRS,         /* Max number of directory      */
#ifdef __DEBUG_TRACE__
      "Partition A - Comedia Application"
#endif
   },
#endif
#ifdef kFS_CFG_PART_B
   /* Comedia Data (partition B)                                              */
   {
      kFS_CFG_PART_B,                         /* Partition Device ID          */
      kDMJFS_PART_DEVICE_UNDEF,               /* Not an Alias                 */
      FALSE,                                  /* Standard partition           */
      0,                                      /* Maximum size of control data */
      kFS_CFGi_PROPS_PART_B_CRCFILE_ENABLE,   /* Check CRC file at mount      */
      kFS_CFGi_PROPS_PART_B_REC_CDATA_NRETRY, /* Retry num. recovering CData  */
      kFS_CFGi_PROPS_PART_B_REC_FILE_NRETRY,  /* Retry num. recovering File   */
      kFS_CFGi_PROPS_PART_B_WRI_CDATA_NRETRY, /* Retry num. write CData       */
      kFS_CFGi_PROPS_PART_B_WRI_FILE_NRETRY,  /* Retry num. write File        */
      kFS_CFGi_PROPS_PART_B_DATA_FAREA,       /* Flash Area ID of data blocks */
      kFS_CFGi_PROPS_PART_B_DATA_SZ_HEAD,     /* Size of partition header     */
      kFS_CFGi_PROPS_PART_B_DATA_MIN_BLOCK,   /* Mini number of data blocks   */
      kFS_CFGi_PROPS_PART_B_CTRL_FAREA,       /* Flash Area ID of ctrl blocks */
      kFS_CFGi_PROPS_PART_B_CTRL_SZ_HEAD,     /* Size of partition header     */
      kFS_CFGi_PROPS_PART_B_CTRL_MIN_BLOCK,   /* Mini number of ctrl blocks   */
      kFS_CFGi_PROPS_PART_B_MAX_FILES,        /* Max number of files          */
      kFS_CFGi_PROPS_PART_B_MAX_DIRS,         /* Max number of directory      */
#ifdef __DEBUG_TRACE__
      "Partition B - Comedia Data"
#endif
   },
#endif
#ifdef kFS_CFG_PART_C /* Comedia Data (partition B)                                              */
   {
      kFS_CFG_PART_C,                         /* Partition Device ID          */
      kDMJFS_PART_DEVICE_UNDEF,               /* Not an Alias                 */
      FALSE,                                  /* Standard partition           */
      0,                                      /* Maximum size of control data */
      kFS_CFGi_PROPS_PART_C_CRCFILE_ENABLE,   /* Check CRC file at mount      */
      kFS_CFGi_PROPS_PART_C_REC_CDATA_NRETRY, /* Retry num. recovering CData  */
      kFS_CFGi_PROPS_PART_C_REC_FILE_NRETRY,  /* Retry num. recovering File   */
      kFS_CFGi_PROPS_PART_C_WRI_CDATA_NRETRY, /* Retry num. write CData       */
      kFS_CFGi_PROPS_PART_C_WRI_FILE_NRETRY,  /* Retry num. write File        */
      kFS_CFGi_PROPS_PART_C_DATA_FAREA,       /* Flash Area ID of data blocks */
      kFS_CFGi_PROPS_PART_C_DATA_SZ_HEAD,     /* Size of partition header     */
      kFS_CFGi_PROPS_PART_C_DATA_MIN_BLOCK,   /* Mini number of data blocks   */
      kFS_CFGi_PROPS_PART_C_CTRL_FAREA,       /* Flash Area ID of ctrl blocks */
      kFS_CFGi_PROPS_PART_C_CTRL_SZ_HEAD,     /* Size of partition header     */
      kFS_CFGi_PROPS_PART_C_CTRL_MIN_BLOCK,   /* Mini number of ctrl blocks   */
      kFS_CFGi_PROPS_PART_C_MAX_FILES,        /* Max number of files          */
      kFS_CFGi_PROPS_PART_C_MAX_DIRS,         /* Max number of directory      */
#ifdef __DEBUG_TRACE__
      "Partition C - Extern Data"
#endif
   },
#endif


#ifdef kFS_CFG_PART_N
   /* CAK DLK Data  (partition N)                                             */
   {
      kFS_CFG_PART_N,                         /* Partition Device ID          */
#ifdef kFS_CFGi_PROPS_PART_N_ALIAS_ON
      kFS_CFGi_PROPS_PART_N_ALIAS_ON,         /* Alias on previous partition  */
      FALSE,                                  /* Standard partition           */
      0,                                      /* Maximum size of control data */
      FALSE,                                  /* Check CRC file at mount      */
      0,                                      /* Retry num. recovering CData  */
      0,                                      /* Retry num. recovering File   */
      0,                                      /* Retry num. write CData       */
      0,                                      /* Retry num. write File        */
      eFLASH_CFG_FLASH_AREA_UNDEFINED,        /* Flash Area ID of data blocks */
      0,                                      /* Size of partition header     */
      0,                                      /* Mini number of data blocks   */
      eFLASH_CFG_FLASH_AREA_UNDEFINED,        /* Flash Area ID of ctrl blocks */
      0,                                      /* Size of partition header     */
      0,                                      /* Mini number of ctrl blocks   */
      0,                                      /* Max number of files          */
      0,                                      /* Max number of directory      */
#else
      kDMJFS_PART_DEVICE_UNDEF,               /* Not an Alias                 */
      FALSE,                                  /* Standard partition           */
      0,                                      /* Maximum size of control data */
      kFS_CFGi_PROPS_PART_N_CRCFILE_ENABLE,   /* Check CRC file at mount      */
      kFS_CFGi_PROPS_PART_N_REC_CDATA_NRETRY, /* Retry num. recovering CData  */
      kFS_CFGi_PROPS_PART_N_REC_FILE_NRETRY,  /* Retry num. recovering File   */
      kFS_CFGi_PROPS_PART_N_WRI_CDATA_NRETRY, /* Retry num. write CData       */
      kFS_CFGi_PROPS_PART_N_WRI_FILE_NRETRY,  /* Retry num. write File        */
      kFS_CFGi_PROPS_PART_N_DATA_FAREA,       /* Flash Area ID of data blocks */
      kFS_CFGi_PROPS_PART_N_DATA_SZ_HEAD,     /* Size of partition header     */
      kFS_CFGi_PROPS_PART_N_DATA_MIN_BLOCK,   /* Mini number of data blocks   */
      kFS_CFGi_PROPS_PART_N_CTRL_FAREA,       /* Flash Area ID of ctrl blocks */
      kFS_CFGi_PROPS_PART_N_CTRL_SZ_HEAD,     /* Size of partition header     */
      kFS_CFGi_PROPS_PART_N_CTRL_MIN_BLOCK,   /* Mini number of ctrl blocks   */
      kFS_CFGi_PROPS_PART_N_MAX_FILES,        /* Max number of files          */
      kFS_CFGi_PROPS_PART_N_MAX_DIRS,         /* Max number of directory      */
#endif
#ifdef __DEBUG_TRACE__
      "Partition N - CAK DLK Data"
#endif
   }
#endif
};

#ifdef CAK_DLK
#ifdef NEWLAND_MERLIN_CONVERT
/**
 *  @brief
 *    Table for CRC32 calculation.
*/
#if defined(GTPL)
static uint32_t Section_crc_32_tab[] = {
0x00000000, 0x77073096, 0xee0e612c, 0x990951ba, 0x076dc419, 0x706af48f, 0xe963a535, 0x9e6495a3,
0x0edb8832, 0x79dcb8a4, 0xe0d5e91e, 0x97d2d988, 0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91,
0x1db71064, 0x6ab020f2, 0xf3b97148, 0x84be41de, 0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7,
0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec, 0x14015c4f, 0x63066cd9, 0xfa0f3d63, 0x8d080df5,
0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172, 0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b,
0x35b5a8fa, 0x42b2986c, 0xdbbbc9d6, 0xacbcf940, 0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59,
0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116, 0x21b4f4b5, 0x56b3c423, 0xcfba9599, 0xb8bda50f,
0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924, 0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d,
0x76dc4190, 0x01db7106, 0x98d220bc, 0xefd5102a, 0x71b18589, 0x06b6b51f, 0x9fbfe4a5, 0xe8b8d433,
0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818, 0x7f6a0dbb, 0x086d3d2d, 0x91646c97, 0xe6635c01,
0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e, 0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457,
0x65b0d9c6, 0x12b7e950, 0x8bbeb8ea, 0xfcb9887c, 0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65,
0x4db26158, 0x3ab551ce, 0xa3bc0074, 0xd4bb30e2, 0x4adfa541, 0x3dd895d7, 0xa4d1c46d, 0xd3d6f4fb,
0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0, 0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9,
0x5005713c, 0x270241aa, 0xbe0b1010, 0xc90c2086, 0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f,
0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4, 0x59b33d17, 0x2eb40d81, 0xb7bd5c3b, 0xc0ba6cad,
0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a, 0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683,
0xe3630b12, 0x94643b84, 0x0d6d6a3e, 0x7a6a5aa8, 0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1,
0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe, 0xf762575d, 0x806567cb, 0x196c3671, 0x6e6b06e7,
0xfed41b76, 0x89d32be0, 0x10da7a5a, 0x67dd4acc, 0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5,
0xd6d6a3e8, 0xa1d1937e, 0x38d8c2c4, 0x4fdff252, 0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b,
0xd80d2bda, 0xaf0a1b4c, 0x36034af6, 0x41047a60, 0xdf60efc3, 0xa867df55, 0x316e8eef, 0x4669be79,
0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236, 0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f,
0xc5ba3bbe, 0xb2bd0b28, 0x2bb45a92, 0x5cb36a04, 0xc2d7ffa7, 0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d,
0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a, 0x9c0906a9, 0xeb0e363f, 0x72076785, 0x05005713,
0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38, 0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21,
0x86d3d2d4, 0xf1d4e242, 0x68ddb3f8, 0x1fda836e, 0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777,
0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c, 0x8f659eff, 0xf862ae69, 0x616bffd3, 0x166ccf45,
0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2, 0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db,
0xaed16a4a, 0xd9d65adc, 0x40df0b66, 0x37d83bf0, 0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9,
0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6, 0xbad03605, 0xcdd70693, 0x54de5729, 0x23d967bf,
0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94, 0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d
};
#elif defined(IMCL)
static const uint32_t Section_crc_32_tab[] =
{

         0x00000000, 0x04c11db7, 0x09823b6e, 0x0d4326d9, 0x130476dc, 0x17c56b6b, 0x1a864db2, 0x1e475005,

         0x2608edb8, 0x22c9f00f, 0x2f8ad6d6, 0x2b4bcb61, 0x350c9b64, 0x31cd86d3, 0x3c8ea00a, 0x384fbdbd,

         0x4c11db70, 0x48d0c6c7, 0x4593e01e, 0x4152fda9, 0x5f15adac, 0x5bd4b01b, 0x569796c2, 0x52568b75,

         0x6a1936c8, 0x6ed82b7f, 0x639b0da6, 0x675a1011, 0x791d4014, 0x7ddc5da3, 0x709f7b7a, 0x745e66cd,

         0x9823b6e0, 0x9ce2ab57, 0x91a18d8e, 0x95609039, 0x8b27c03c, 0x8fe6dd8b, 0x82a5fb52, 0x8664e6e5,

         0xbe2b5b58, 0xbaea46ef, 0xb7a96036, 0xb3687d81, 0xad2f2d84, 0xa9ee3033, 0xa4ad16ea, 0xa06c0b5d,

         0xd4326d90, 0xd0f37027, 0xddb056fe, 0xd9714b49, 0xc7361b4c, 0xc3f706fb, 0xceb42022, 0xca753d95,

         0xf23a8028, 0xf6fb9d9f, 0xfbb8bb46, 0xff79a6f1, 0xe13ef6f4, 0xe5ffeb43, 0xe8bccd9a, 0xec7dd02d,

         0x34867077, 0x30476dc0, 0x3d044b19, 0x39c556ae, 0x278206ab, 0x23431b1c, 0x2e003dc5, 0x2ac12072,

         0x128e9dcf, 0x164f8078, 0x1b0ca6a1, 0x1fcdbb16, 0x018aeb13, 0x054bf6a4, 0x0808d07d, 0x0cc9cdca,

         0x7897ab07, 0x7c56b6b0, 0x71159069, 0x75d48dde, 0x6b93dddb, 0x6f52c06c, 0x6211e6b5, 0x66d0fb02,

         0x5e9f46bf, 0x5a5e5b08, 0x571d7dd1, 0x53dc6066, 0x4d9b3063, 0x495a2dd4, 0x44190b0d, 0x40d816ba,

         0xaca5c697, 0xa864db20, 0xa527fdf9, 0xa1e6e04e, 0xbfa1b04b, 0xbb60adfc, 0xb6238b25, 0xb2e29692,

         0x8aad2b2f, 0x8e6c3698, 0x832f1041, 0x87ee0df6, 0x99a95df3, 0x9d684044, 0x902b669d, 0x94ea7b2a,

         0xe0b41de7, 0xe4750050, 0xe9362689, 0xedf73b3e, 0xf3b06b3b, 0xf771768c, 0xfa325055, 0xfef34de2,

         0xc6bcf05f, 0xc27dede8, 0xcf3ecb31, 0xcbffd686, 0xd5b88683, 0xd1799b34, 0xdc3abded, 0xd8fba05a,

         0x690ce0ee, 0x6dcdfd59, 0x608edb80, 0x644fc637, 0x7a089632, 0x7ec98b85, 0x738aad5c, 0x774bb0eb,

         0x4f040d56, 0x4bc510e1, 0x46863638, 0x42472b8f, 0x5c007b8a, 0x58c1663d, 0x558240e4, 0x51435d53,

         0x251d3b9e, 0x21dc2629, 0x2c9f00f0, 0x285e1d47, 0x36194d42, 0x32d850f5, 0x3f9b762c, 0x3b5a6b9b,

         0x0315d626, 0x07d4cb91, 0x0a97ed48, 0x0e56f0ff, 0x1011a0fa, 0x14d0bd4d, 0x19939b94, 0x1d528623,

         0xf12f560e, 0xf5ee4bb9, 0xf8ad6d60, 0xfc6c70d7, 0xe22b20d2, 0xe6ea3d65, 0xeba91bbc, 0xef68060b,

         0xd727bbb6, 0xd3e6a601, 0xdea580d8, 0xda649d6f, 0xc423cd6a, 0xc0e2d0dd, 0xcda1f604, 0xc960ebb3,

         0xbd3e8d7e, 0xb9ff90c9, 0xb4bcb610, 0xb07daba7, 0xae3afba2, 0xaafbe615, 0xa7b8c0cc, 0xa379dd7b,

         0x9b3660c6, 0x9ff77d71, 0x92b45ba8, 0x9675461f, 0x8832161a, 0x8cf30bad, 0x81b02d74, 0x857130c3,

         0x5d8a9099, 0x594b8d2e, 0x5408abf7, 0x50c9b640, 0x4e8ee645, 0x4a4ffbf2, 0x470cdd2b, 0x43cdc09c,

         0x7b827d21, 0x7f436096, 0x7200464f, 0x76c15bf8, 0x68860bfd, 0x6c47164a, 0x61043093, 0x65c52d24,

         0x119b4be9, 0x155a565e, 0x18197087, 0x1cd86d30, 0x029f3d35, 0x065e2082, 0x0b1d065b, 0x0fdc1bec,

         0x3793a651, 0x3352bbe6, 0x3e119d3f, 0x3ad08088, 0x2497d08d, 0x2056cd3a, 0x2d15ebe3, 0x29d4f654,

         0xc5a92679, 0xc1683bce, 0xcc2b1d17, 0xc8ea00a0, 0xd6ad50a5, 0xd26c4d12, 0xdf2f6bcb, 0xdbee767c,

         0xe3a1cbc1, 0xe760d676, 0xea23f0af, 0xeee2ed18, 0xf0a5bd1d, 0xf464a0aa, 0xf9278673, 0xfde69bc4,

         0x89b8fd09, 0x8d79e0be, 0x803ac667, 0x84fbdbd0, 0x9abc8bd5, 0x9e7d9662, 0x933eb0bb, 0x97ffad0c,

         0xafb010b1, 0xab710d06, 0xa6322bdf, 0xa2f33668, 0xbcb4666d, 0xb8757bda, 0xb5365d03, 0xb1f740b4

};
#endif


#endif
#endif
/******************************************************************************
* Locales Functions Prototypes
******************************************************************************/

LOCAL bool                    FS_CFGi_Constructor   (void);
LOCAL tFS_CFG_PartitionProps *FS_CFGi_GetParition   (char xcDevice);
LOCAL tFS_CFG_PartitionProps *FS_CFGi_GetAlias      (char xcDevice);
LOCAL bool                    FS_CFGi_MountPart     (tFS_CFG_PartitionProps *pxstPartProps,
                                                     bool xbReset,
                                                     bool xbRetryWithReset);
#if defined (kFS_CFG_PART_A_IN_RAM)
LOCAL bool                    FS_CFGi_MountPartFromRAM (tFS_CFG_PartitionProps *pxstPartProps,
                                                        uint8_t *pxui8PartRAM);
#endif

LOCAL bool                    FS_CFGi_UnmountPart   (tFS_CFG_PartitionProps *pxstPartProps);
LOCAL bool                    FS_CFGi_Reset         (tFS_CFG_PartitionProps *pxstPartProps);
LOCAL bool                    FS_CFGi_CleanFiles    (tFS_CFG_PartitionProps *pxstPartProps);
#ifdef kFS_CFG_PART_N
LOCAL bool                    FS_CFGi_CleanFilesDLK (tFS_CFG_PartitionProps *pxstPartProps,
                                                     char *pxcComediaDir);
#endif

#if defined(NEWLAND_BOOTLOADER) || defined(CHANGHONG_BOOTLOADER) || defined(SKYWORTH_BOOTLOADER) || defined(TELESYSTEM_BOOTLOADER) || defined(TELESYSTEM_MAP34_BOOTLOADER) || defined(TELESYSTEM_MAP51_BOOTLOADER) || defined(TECHNICOLOR_BOOTLOADER) || defined(KAON_MSD5C59_BOOTLOADER)
LOCAL void FS_CFGi_CheckPartChange(tFLASH_CFG_Pack_Area ePackedId,tFLASH_CFG_FLASH_Area SourceId, tFLASH_CFG_FLASH_Area DestinationId);
bool FS_CFGi_UnpackPart(tFLASH_CFG_Pack_Area ePackedId,tFLASH_CFG_FLASH_Area SourceId, tFLASH_CFG_FLASH_Area DestinationId);
void FS_CFGi_UnpackFS();

#ifdef CAK_DLK
#ifdef MIGRATION_N_PARTITION
void FS_CFGi_MovePartitionNCtrlBlock(void);
void FS_CFGi_MovePartitionN(void);
void FS_CFGi_MigrationNPartition(void);
void FS_CFGi_ErasePartitionBCtrlBlock(void);
#endif
#ifdef NEWLAND_MERLIN_CONVERT

#if defined(IMCL)
#define NEWLAND_SEC_FLASH_ADDRESS 0x520000
#define NEWLAND_SEC_FLASH_BAK_ADDRESS 0x530000
#elif defined(GTPL)
#define NEWLAND_SEC_FLASH_ADDRESS 0x7a0000
#define NEWLAND_SEC_FLASH_BAK_ADDRESS 0x7b0000
#endif

#define NEWLAND_SEC_FLASH_SIZE    0x10000
#define NEWLAND_NAGRA_CONVERT_MAGIC 0xb16b00b5

void FS_CFGi_ConvertNagraData();
void FS_CFGi_ConvertNagraNVM();
void FS_CFGi_ConvertNagraVFS();
void FS_CFGi_CopyToDMJFS(uint8_t device,
                        tFS_CFGi_File stFile,
                        uint32_t ui32StartAddress,
                        uint32_t ui32CurPartSize);
LOCAL uint32_t FS_CFGi_GenerateCRC32(uint8_t *pxui8Data,
                                     uint32_t xi32Size);

LOCAL bool FS_CFGi_GetFile(uint32_t        xui32StartAddress,
                           tFS_CFGi_File   *pxstFile,
                           uint32_t        *pxui32EndAddress);
#endif
#endif

#endif

#define UNZIP_BUFFER_SIZE (2*1024*1024)


/******************************************************************************
 * Public Function
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
)
{
   int32_t                 i; 
   bool                    reset,retry;
   tFS_CFG_PartitionProps *p_comedia_toclean = NULL;

   mTBOX_FCT_ENTER(FS_CFG_MountFilesSystem);

   /* Initialization if necessary */
   if (!FS_CFG_Init(cbCriticalError))
   {
      mTBOX_RETURN(FALSE);
   }

   mTBOX_TRACE((kTBOX_NIV_1,"%s(%s,%s): Mount Comedia Files System...\n",
                TBOX_FunctionName,mFS_CFG_STRBOOL(xbResetPartComedia),
                mFS_CFG_STRBOOL(xbRetryPartComediaWithReset)));


#if defined(NEWLAND_BOOTLOADER) || defined(CHANGHONG_BOOTLOADER) || defined(SKYWORTH_BOOTLOADER) || defined(TELESYSTEM_BOOTLOADER) || defined(TELESYSTEM_MAP34_BOOTLOADER) || defined(TELESYSTEM_MAP51_BOOTLOADER) || defined(TECHNICOLOR_BOOTLOADER) || defined(KAON_MSD5C59_BOOTLOADER)

#ifdef CAK_DLK
#ifdef NEWLAND_MERLIN_CONVERT

    FS_CFGi_ConvertNagraData();

#endif

#ifdef MIGRATION_N_PARTITION
	FS_CFGi_MigrationNPartition();
#endif
#endif

#if !defined(COMEDIA_RESOURCES_IN_RAM)
   // entry for NAGRA migration and PARTITO A unpacking
     FS_CFGi_UnpackFS();
#endif

#endif // NEWLAND_BOOTLOADER || CHANGHONG_BOOTLOADER

   /* Mount partitions configured */
   for (i = 0; i < eFS_CFG_PART_NUMBER; i++)
   {
      reset = FALSE;
      retry = FALSE;

#ifdef kFS_CFG_PART_B
      if (i == eFS_CFG_PART_COMEDIA_DATA)
      {
         /* Parition B */
         if (xbResetPartComedia)
         {
            tFS_CFG_PartitionProps *p_alias = FS_CFGi_GetAlias(vFS_CFGi_tstPartitionsProps[i].cDevice);

            if (p_alias == NULL)
            {
               /* No Alias OK reset */
               reset = TRUE;
            }
            else
            {
               mTBOX_TRACE((kTBOX_NIV_1,"%s(%s,%s): Partition %c '%s' is an "
                            "alias on %c => Replace the reset parition '%s' "
                            "by a Clean Files\n",
                            TBOX_FunctionName,mFS_CFG_STRBOOL(xbResetPartComedia),
                            mFS_CFG_STRBOOL(xbRetryPartComediaWithReset),
                            p_alias->cDevice,p_alias->pstrLabel,
                            vFS_CFGi_tstPartitionsProps[i].cDevice,
                            vFS_CFGi_tstPartitionsProps[i].pstrLabel));
               p_comedia_toclean = vFS_CFGi_tstPartitionsProps + i;
            }
         }
         else reset = FALSE;

         retry = xbRetryPartComediaWithReset;

#if defined (kFS_CFGi_PROPS_PART_N_ALIAS_ON) && defined (CAK_DALTS) && !defined (__CAK_DAL_TESTU_DATA_MNGR__)
         /**
          *  DALTS Test suite with N is an alias on B 
         */
         mTBOX_TRACE((kTBOX_NIV_1,"%s(%s,%s): Run DALTS Test Suite "
                      "on N alias B => Mount '%c' = '%s'\n",
                      TBOX_FunctionName,mFS_CFG_STRBOOL(xbResetPartComedia),
                      mFS_CFG_STRBOOL(xbRetryPartComediaWithReset),
                      vFS_CFGi_tstPartitionsProps[i].cDevice,
                      vFS_CFGi_tstPartitionsProps[i].pstrLabel));
         reset             = FALSE;
         p_comedia_toclean = NULL;
#endif
      }
#endif /* ifded PART B */
#ifdef kFS_CFG_PART_N
      if (i == eFS_CFG_PART_CAK_DLK)
      {
         /* Parition N */
#if !defined (kFS_CFGi_PROPS_PART_N_ALIAS_ON) && defined (CAK_DALTS) && !defined (__CAK_DAL_TESTU_DATA_MNGR__)
         /**
          *  DALTS Test suite on PHYSICAL Partition N (not an alias)
         */
         mTBOX_TRACE((kTBOX_NIV_1,"%s(%s,%s): Run DALTS Test Suite "
                      "=> Mount '%c' = '%s'\n",
                      TBOX_FunctionName,mFS_CFG_STRBOOL(xbResetPartComedia),
                      mFS_CFG_STRBOOL(xbRetryPartComediaWithReset),
                      vFS_CFGi_tstPartitionsProps[i].cDevice,
                      vFS_CFGi_tstPartitionsProps[i].pstrLabel));
         reset = FALSE;
#else
         /**
          *  Not reset parition N because:
          *  - Comedia never reset N 
          *  - Run DATLS with POC Data manager 
          *  - Not Reset N it is an alias on another parition
         */
         reset = FALSE;
#endif
         /**
          *  Systematic retry with reset on Partition N except 
          *  if N is an Alias on B
         */
#if defined (kFS_CFGi_PROPS_PART_N_ALIAS_ON)
         retry = FALSE;
#else
         retry = TRUE;
#endif
      }
#endif /* ifded PART N */


#if defined (kFS_CFG_PART_B) && defined (kDMJFS_PARTi_FORCE_RESET_PART_B)
      if ((!reset) && (vFS_CFGi_tstPartitionsProps[i].cDevice == kFS_CFG_PART_B))
      {
         mTBOX_TRACE((kTBOX_NIV_WARNING,"%s(%s,%s) WARNING: "
                      "Force RESET '%c' = '%s'\n",
                      TBOX_FunctionName,mFS_CFG_STRBOOL(xbResetPartComedia),
                      mFS_CFG_STRBOOL(xbRetryPartComediaWithReset),
                      vFS_CFGi_tstPartitionsProps[i].cDevice,
                      vFS_CFGi_tstPartitionsProps[i].pstrLabel));
         reset = TRUE;
      }
#endif
#if defined (kFS_CFG_PART_N) && defined (kDMJFS_PARTi_FORCE_RESET_PART_N)
      if ((!reset) && (vFS_CFGi_tstPartitionsProps[i].cDevice == kFS_CFG_PART_N))
      {
         mTBOX_TRACE((kTBOX_NIV_WARNING,"%s(%s,%s) WARNING: "
                      "Force RESET '%c' = '%s'\n",
                      TBOX_FunctionName,mFS_CFG_STRBOOL(xbResetPartComedia),
                      mFS_CFG_STRBOOL(xbRetryPartComediaWithReset),
                      vFS_CFGi_tstPartitionsProps[i].cDevice,
                      vFS_CFGi_tstPartitionsProps[i].pstrLabel));
         reset = TRUE;
      }
#endif

#ifdef kFS_CFG_PART_C
      if (i == eFS_CFG_PART_EXTERN_DATA)
      {
                 /* Parition C */
                 
                 mTBOX_TRACE((kTBOX_NIV_1,"%s(%s,%s): Partition C "
                              "=> Mount '%c' = '%s'\n",
                              TBOX_FunctionName,mFS_CFG_STRBOOL(xbResetPartComedia),
                              mFS_CFG_STRBOOL(xbRetryPartComediaWithReset),
                              vFS_CFGi_tstPartitionsProps[i].cDevice,
                              vFS_CFGi_tstPartitionsProps[i].pstrLabel));
                 

          /**
                  *  Not reset parition C 
                 */
                 reset = FALSE;
                 retry = FALSE;

      }
#endif /* ifded PART C */


		/* TBD - part of code from other project, maybe we need this */
      /* Mount */
#if defined (kFS_CFG_PART_A) && defined (kFS_CFG_PART_A_IN_RAM)
      if ((i == eFS_CFG_PART_COMEDIA_APPL) && (!reset))
      {
         if (!FS_CFGi_MountPartFromRAM(vFS_CFGi_tstPartitionsProps+i,
                                       FS_CFGi_RAMPartitionA))
         {
            mTBOX_RETURN(FALSE);
         }
      }
      else
#endif
      if (!FS_CFGi_MountPart(vFS_CFGi_tstPartitionsProps+i,reset,retry))
      {
         mTBOX_RETURN(FALSE);
      }
   }

   /* Clean files Comedia if refused (because alias exist) */
   if (p_comedia_toclean != NULL)
   {
      mTBOX_TRACE((kTBOX_NIV_1,"%s(%s,%s): Clean Files Comedia\n",
                   TBOX_FunctionName,mFS_CFG_STRBOOL(xbResetPartComedia),
                   mFS_CFG_STRBOOL(xbRetryPartComediaWithReset)));
      FS_CFGi_CleanFiles(p_comedia_toclean);
   }

   /* Okay */
   mTBOX_TRACE((kTBOX_NIV_1,"%s(%s,%s): Comedia File System MOUNTED\n",
                TBOX_FunctionName,mFS_CFG_STRBOOL(xbResetPartComedia),
                mFS_CFG_STRBOOL(xbRetryPartComediaWithReset)));
#if defined (__DEBUG_TRACE__)
   DMJFS_DisplayInfo();
#endif

   mTBOX_RETURN(TRUE);
}

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
)
{
   int32_t i; 

   mTBOX_FCT_ENTER(FS_CFG_UnmountFilesSystem);

   if (!vFS_CFGi_bIsInit)
   {
      mTBOX_TRACE((kTBOX_NIV_WARNING,"%s() WARNING: "
                   "F.S. Config not initialized\n",
                   TBOX_FunctionName));
      mTBOX_RETURN(FALSE);
   }

   mTBOX_TRACE((kTBOX_NIV_1,"%s(): Unmount Comedia File System...\n",
                TBOX_FunctionName));

   /* Unmount partitions mounted */
   i = eFS_CFG_PART_NUMBER;

   while (i > 0)
   {
      i--;

      /* Unmount */
      if (!FS_CFGi_UnmountPart(vFS_CFGi_tstPartitionsProps+i))
      {
         mTBOX_RETURN(FALSE);
      }
   }

   /* Termination */
   FS_CFG_Term();

   /* Okay */
   mTBOX_TRACE((kTBOX_NIV_1,"%s(): Comedia Files System UNMOUNTED\n",
                TBOX_FunctionName));

   mTBOX_RETURN(TRUE);
}

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
)
{
   tDMJFS_Error rc = eDMJFS_NO_ERROR;

   mTBOX_FCT_ENTER(FS_CFG_MountFilesSystem);

   /* Local initialization if necessary */
   if (!vFS_CFGi_bIsInit)
   {
      FS_CFGi_Constructor();
      vFS_CFGi_bIsInit = TRUE;
   }

   mTBOX_TRACE((kTBOX_NIV_1,"%s(): Initialization...\n",
                TBOX_FunctionName));

   /* Data Manager Initialization if necessary */
   if (!DMJFS_IsInititialized())
   {
      mTBOX_TRACE((kTBOX_NIV_1,"%s(): Initialize Data Manager JFS\n",
                   TBOX_FunctionName));
      vFS_CFGi_bIsInitHereDMJFS = FALSE;

      rc = DMJFS_Init(kFS_CFGi_MAX_FILES_OPEN,
                      kFS_CFGi_MAX_FILES_OPEN_INWRITE,
                      kFS_CFGi_MAX_DIR_OPEN,
                      cbCriticalError);
      if (rc != eDMJFS_NO_ERROR)
      {
         mTBOX_TRACE((kTBOX_NIV_CRITICAL,"%s() ERROR: "
                      "D.M. JFS Initialization failure (error %d: %s)\n",
                      TBOX_FunctionName,
                      rc,DMJFS_LabelError(rc)));
         mTBOX_RETURN(FALSE);
      }

      /* Okay */
      vFS_CFGi_bIsInitHereDMJFS = TRUE;
   }

   /* Okay */
   mTBOX_TRACE((kTBOX_NIV_1,"%s(): Initialization DONE\n",
                TBOX_FunctionName));

   mTBOX_RETURN(TRUE);
}

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
)
{
   tDMJFS_Error rc;

   mTBOX_FCT_ENTER(FS_CFG_Term);

   if (!vFS_CFGi_bIsInit)
   {
      mTBOX_TRACE((kTBOX_NIV_WARNING,"%s() WARNING: "
                   "F.S. Config not initialized\n",
                   TBOX_FunctionName));
      mTBOX_RETURN(FALSE);
   }

   mTBOX_TRACE((kTBOX_NIV_1,"%s(): Termination...\n",
                TBOX_FunctionName));

   /* Data Manager desinitialization if initialized here */
   if (vFS_CFGi_bIsInitHereDMJFS)
   {
      mTBOX_TRACE((kTBOX_NIV_1,"%s(): Terminate Data Manager JFS\n",
                   TBOX_FunctionName));

      rc = DMJFS_Terminate();
      if (rc != eDMJFS_NO_ERROR)
      {
         mTBOX_TRACE((kTBOX_NIV_CRITICAL,"%s() ERROR: "
                      "D.M. JFS Termination failure (error %d: %s)\n",
                      TBOX_FunctionName,rc,DMJFS_LabelError(rc)));
      }

      vFS_CFGi_bIsInitHereDMJFS = FALSE;
   }

   /* Okay */
   mTBOX_TRACE((kTBOX_NIV_1,"%s(): Termination DONE\n",
                TBOX_FunctionName));

   mTBOX_RETURN(TRUE);
}

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
)
{
   tFS_CFG_PartitionProps *p_part_props = NULL;


   mTBOX_FCT_ENTER(FS_CFG_Mount);

   if (!vFS_CFGi_bIsInit)
   {
      mTBOX_TRACE((kTBOX_NIV_WARNING,"%s(%c,%s) ERROR: "
                   "F.S. Config not initialized\n",
                   TBOX_FunctionName,xcDevice,mFS_CFG_STRBOOL(xbReset)));
      mTBOX_RETURN(FALSE);
   }

   /* Search the partition properties */
   p_part_props = FS_CFGi_GetParition(xcDevice);

   if (p_part_props == NULL)
   {
      mTBOX_TRACE((kTBOX_NIV_WARNING,"%s(%c,%s) ERROR: Partition not found\n",
                   TBOX_FunctionName,xcDevice,mFS_CFG_STRBOOL(xbReset)));
      mTBOX_RETURN(FALSE);
   }

   /* Mount */
   mTBOX_RETURN(FS_CFGi_MountPart(p_part_props,xbReset,FALSE));
}

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
)
{
   tFS_CFG_PartitionProps *p_part_props = NULL;

   mTBOX_FCT_ENTER(FS_CFG_Unmount);

   if (!vFS_CFGi_bIsInit)
   {
      mTBOX_TRACE((kTBOX_NIV_WARNING,"%s(%c) ERROR: "
                   "F.S. Config not initialized\n",
                   TBOX_FunctionName,xcDevice));
      mTBOX_RETURN(FALSE);
   }
   /* Search the partition properties */
   p_part_props = FS_CFGi_GetParition(xcDevice);

   if (p_part_props == NULL)
   {
      mTBOX_TRACE((kTBOX_NIV_WARNING,"%s(%c) ERROR: Partition not found\n",
                   TBOX_FunctionName,xcDevice));
      mTBOX_RETURN(FALSE);
   }

   /* Unmount */
   mTBOX_RETURN(FS_CFGi_UnmountPart(p_part_props));
}

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
)
{
   tFS_CFG_PartitionProps *p_part_props = NULL;

   mTBOX_FCT_ENTER(FS_CFG_Reset);

   if (!vFS_CFGi_bIsInit)
   {
      mTBOX_TRACE((kTBOX_NIV_WARNING,"%s(%c) ERROR: "
                   "F.S. Config not initialized\n",
                   TBOX_FunctionName,xcDevice));
      mTBOX_RETURN(FALSE);
   }

   /* Search the partition properties */
   p_part_props = FS_CFGi_GetParition(xcDevice);

   if (p_part_props == NULL)
   {
      mTBOX_TRACE((kTBOX_NIV_WARNING,"%s(%c) ERROR: Partition not found\n",
                   TBOX_FunctionName,xcDevice));
      mTBOX_RETURN(FALSE);
   }

   if (p_part_props->cAliasToDevice != kDMJFS_PART_DEVICE_UNDEF)
   {
      mTBOX_TRACE((kTBOX_NIV_WARNING,"%s(%c) WARNING: Reset forbidden, "
                   "partition '%s' is an alias to parition %c "
                   "=> Process a Clean files\n",
                   TBOX_FunctionName,xcDevice,p_part_props->pstrLabel,
                   p_part_props->cAliasToDevice));
      mTBOX_RETURN(FS_CFGi_CleanFiles(p_part_props));
   }


   /* Reset */
   mTBOX_RETURN(FS_CFGi_Reset(p_part_props));
}

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
)
{
   tFS_CFG_PartitionProps *p_part_props = NULL;
   tFS_CFG_PartitionProps *p_alias      = NULL;

   mTBOX_FCT_ENTER(FS_CFG_CleanFiles);

   if (!vFS_CFGi_bIsInit)
   {
      mTBOX_TRACE((kTBOX_NIV_WARNING,"%s(%c) ERROR: "
                   "F.S. Config not initialized\n",
                   TBOX_FunctionName,xcDevice));
      mTBOX_RETURN(FALSE);
   }

   /* Search the partition properties */
   p_part_props = FS_CFGi_GetParition(xcDevice);

   if (p_part_props == NULL)
   {
      mTBOX_TRACE((kTBOX_NIV_WARNING,"%s(%c) ERROR: Partition not found\n",
                   TBOX_FunctionName,xcDevice));
      mTBOX_RETURN(FALSE);
   }

   if (p_part_props->cAliasToDevice == kDMJFS_PART_DEVICE_UNDEF)
   {
      /* Not an Alias, search if have a Alias */
      p_alias = FS_CFGi_GetAlias(xcDevice);

      if (p_alias == NULL)
      {
         mTBOX_TRACE((kTBOX_NIV_1,"%s(%c): Partition '%s' is not an alias "
                      "and have no alias => Process a Reset\n",
                      TBOX_FunctionName,xcDevice,p_part_props->pstrLabel));
         mTBOX_RETURN(FS_CFGi_Reset(p_part_props));
      }

      mTBOX_TRACE((kTBOX_NIV_1,"%s(%c): Partition '%s' (not an alias) "
                   "have the alias %c '%s' => Process a Clean Files\n",
                   TBOX_FunctionName,xcDevice,p_part_props->pstrLabel,
                   p_alias->cDevice,p_alias->pstrLabel));
   }
   else
   {
      mTBOX_TRACE((kTBOX_NIV_1,"%s(%c): Partition '%s' is an alias to %c "
                   "=> Process a Clean Files\n",
                   TBOX_FunctionName,xcDevice,p_part_props->pstrLabel,
                   p_part_props->cAliasToDevice));
   }

   /* Clean files */
   mTBOX_RETURN(FS_CFGi_CleanFiles(p_part_props));
}

/******************************************************************************
* Locales Functions Implementations
******************************************************************************/

/**
 *  @brief
 *    File System Configuration constructor.
 *
 *  @retval TRUE
 *       Always TRUE to indicate is executed.
 *
*/
LOCAL bool FS_CFGi_Constructor
(
   void 
)
{
   /* Mode trace initialization */
   mTBOX_INIT_MOD_TRACE((eFS_CFG,kTBOX_TRC_CRITICAL
		   	   	   	   	   	   	 /* +kTBOX_TRC_WARNING*/
                                 /*+kTBOX_TRC_FCT */
                                 /*+kTBOX_TRC_1*/ /* API general traces       */
                                 /*+kTBOX_TRC_2*/  /*Partition Properties     */
                                 /*+kTBOX_TRC_3                           */
                                 /*+kTBOX_TRC_4                           */
                                 /*+kTBOX_TRC_5                           */));

   mTBOX_TRACE((kTBOX_NIV_1,"FS_CFGi_Constructor DONE\n"));
   return(TRUE);
}

/**
 *  @brief
 *    Search a parition properties.
 *
 *  @param[in]     xcDevice
 *       Device of partition to search
 *
 *  @retval  Not NULL
 *       Pointer on parition properties
 *
 *  @retval  NULL
 *       Parition not found
 *
*/
LOCAL tFS_CFG_PartitionProps *FS_CFGi_GetParition
(
   char xcDevice
)
{
   int32_t i; 

   for (i = 0; i < eFS_CFG_PART_NUMBER; i++)
   {
      if (vFS_CFGi_tstPartitionsProps[i].cDevice == xcDevice)
      {
         /* Yes found */
         return(vFS_CFGi_tstPartitionsProps+i);
      }
   }

   return(NULL);
}

/**
 *  @brief
 *    Search a parition have an alias and return it.
 *
 *  @param[in]     xcDevice
 *       Device of partition to search
 *
 *  @retval  Not NULL
 *       Pointer on parition properties
 *
 *  @retval  NULL
 *       Parition not found
 *
*/
LOCAL tFS_CFG_PartitionProps *FS_CFGi_GetAlias
(
   char xcDevice
)
{
   int32_t i; 

   for (i = 0; i < eFS_CFG_PART_NUMBER; i++)
   {
      if ((vFS_CFGi_tstPartitionsProps[i].cDevice != xcDevice) &&
          (vFS_CFGi_tstPartitionsProps[i].cAliasToDevice != kDMJFS_PART_DEVICE_UNDEF) &&
          (vFS_CFGi_tstPartitionsProps[i].cAliasToDevice == xcDevice))
      {
         /* Yes found alias */
         return(vFS_CFGi_tstPartitionsProps+i);
      }
   }
 
   return(NULL);
}

/**
 *  @brief
 *    Mount a partition.
 *
 *  @param[in]     pxstPartProps
 *       Properties of partition to mount.
 *
 *  @param[in]     xbReset
 *       TRUE to reset the data partition (partition B, Comedia Data)
 *
 *  @param[in]     xbRetryWithReset
 *       TRUE retry with a reset if the mount is failed (not significant
 *       if reset already set TRUE or the partition is an Alias).
 *
 *  @retval  TRUE
 *       Mount successful
 *
 *  @retval  FALSE
 *       Mount failure
 *
*/
LOCAL bool FS_CFGi_MountPart
(
   tFS_CFG_PartitionProps *pxstPartProps,
   bool                    xbReset,
   bool                    xbRetryWithReset
)
{
   tDMJFS_Error          rc;
   tDMJFS_PartitionProps props;
   tDMJFS_FlashLocation  data_loc,ctrl_loc;
   tFLASH_CFG_Location   area_loc;

   mTBOX_FCT_ENTER(FS_CFGi_MountPart);

   if (DMJFS_IsMounted(pxstPartProps->cDevice))
   {
      mTBOX_TRACE((kTBOX_NIV_WARNING,"%s(%c,%s,%s) WARNING: "
                   "partition %s already mounted\n",
                   TBOX_FunctionName,pxstPartProps->cDevice,
                   mFS_CFG_STRBOOL(xbReset),
                   mFS_CFG_STRBOOL(xbRetryWithReset),
                   pxstPartProps->pstrLabel));
      mTBOX_RETURN(TRUE);
   }

   if (pxstPartProps->cAliasToDevice == kDMJFS_PART_DEVICE_UNDEF)
   {
      /* Mount partition */
      mTBOX_TRACE((kTBOX_NIV_1,"%s(%c,%s,%s): Mount '%s'...\n",
                   TBOX_FunctionName,pxstPartProps->cDevice,
                   mFS_CFG_STRBOOL(xbReset),mFS_CFG_STRBOOL(xbRetryWithReset),
                   pxstPartProps->pstrLabel));

      /* Format properties */
      props.bIsEnableFileCRC    = pxstPartProps->bIsEnableFileCRC;
      props.ui8RecRetryLoadCD   = pxstPartProps->ui8RecRetryLoadCD;
      props.ui8RecRetryLoadFile = pxstPartProps->ui8RecRetryLoadFile;
      props.ui8NumRetrySaveCD   = pxstPartProps->ui8NumRetrySaveCD;
      props.ui8NumRetrySaveFile = pxstPartProps->ui8NumRetrySaveFile;
      props.ui16MinNumDataBlocks = pxstPartProps->ui16DataMinNumBlocks;
      props.ui16MinNumCtrlBlocks = pxstPartProps->ui16CtrlMinNumBlocks;
      props.ui16MaxNumFiles     = pxstPartProps->ui16MaxNumFiles;
      props.ui16MaxNumDir       = pxstPartProps->ui16MaxNumDir;

      if (pxstPartProps->bIsRessources)
      {
         props.ePartType           = eDMJFS_PT_RESSOURCES;
         props.ui32MaxSizeCtrlData = pxstPartProps->ui32MaxSizeCData;
         props.bIsReadOnly         = !xbReset;
      }
      else
      {
         props.ePartType           = eDMJFS_PT_STANDARD;
         props.ui32MaxSizeCtrlData = 0;
         props.bIsReadOnly         = FALSE;
      }

      mTBOX_TRACE((kTBOX_NIV_2,"- Reset............%s\n",mFS_CFG_STRBOOL(xbReset)));
      mTBOX_TRACE((kTBOX_NIV_2,"- PartType.........%d\n",props.ePartType));
      mTBOX_TRACE((kTBOX_NIV_2,"- IsReadOnly.......%s\n",mFS_CFG_STRBOOL(props.bIsReadOnly)));
      mTBOX_TRACE((kTBOX_NIV_2,"- IsEnableFileCRC..%s\n",mFS_CFG_STRBOOL(props.bIsEnableFileCRC)));
      mTBOX_TRACE((kTBOX_NIV_2,"- RecRetryLoadCD...%u\n",props.ui8RecRetryLoadCD));
      mTBOX_TRACE((kTBOX_NIV_2,"- RecRetryLoadFile.%u\n",props.ui8RecRetryLoadFile));
      mTBOX_TRACE((kTBOX_NIV_2,"- NumRetrySaveCD...%u\n",props.ui8NumRetrySaveCD));
      mTBOX_TRACE((kTBOX_NIV_2,"- NumRetrySaveFile.%u\n",props.ui8NumRetrySaveFile));
      mTBOX_TRACE((kTBOX_NIV_2,"- MinNumDataBlocks.%u\n",props.ui16MinNumDataBlocks));
      mTBOX_TRACE((kTBOX_NIV_2,"- MinNumCtrlBlocks.%u\n",props.ui16MinNumCtrlBlocks));
      mTBOX_TRACE((kTBOX_NIV_2,"- MaxSizeCtrlData..%u\n",props.ui32MaxSizeCtrlData));
      mTBOX_TRACE((kTBOX_NIV_2,"- MaxNumFiles......%u\n",props.ui16MaxNumFiles));
      mTBOX_TRACE((kTBOX_NIV_2,"- MaxNumDir........%u\n",props.ui16MaxNumDir));

      /* Set location of Data blocks  */
      if (!FLASH_CFG_GetFlashAreaLocation(pxstPartProps->ui32DataFlashAreaID, 
                                          &area_loc))
      {
         mTBOX_TRACE((kTBOX_NIV_WARNING,"%s(%c,%s,%s) ERROR: Get Flash Area "
                      "%u location of Data Blocks of '%s' failure\n",
                      TBOX_FunctionName,pxstPartProps->cDevice,
                      mFS_CFG_STRBOOL(xbReset),
                      mFS_CFG_STRBOOL(xbRetryWithReset),
                      pxstPartProps->ui32DataFlashAreaID,
                      pxstPartProps->pstrLabel));
         mTBOX_RETURN(FALSE);
      }

#ifdef _DATAMNGR_JFS_RAM_PARTITION_
      data_loc.pxui8RAM         = NULL;
      data_loc.ui32BlockSize    = 0;
#endif
      data_loc.ui32Address      = area_loc.addressOffset;
      data_loc.ui32Size         = area_loc.size;
      data_loc.ui32HeaderSize   = pxstPartProps->ui32DataSizePartHeader;
      mTBOX_TRACE((kTBOX_NIV_2,"- Data.Address.....%#x\n",data_loc.ui32Address));
      mTBOX_TRACE((kTBOX_NIV_2,"- Data.Size........%#x\n",data_loc.ui32Size));
      mTBOX_TRACE((kTBOX_NIV_2,"- Data.HeaderSize..%#x\n",data_loc.ui32HeaderSize));
#ifdef _DATAMNGR_JFS_RAM_PARTITION_
      mTBOX_TRACE((kTBOX_NIV_2,"- Data.RAM.........%#x\n",(unsigned int)data_loc.pxui8RAM));
#endif
      /* Set location of Control Blocks  */
      if (pxstPartProps->ui32CtrlFlashAreaID == eFLASH_CFG_FLASH_AREA_UNDEFINED)
      {
         /* Use first block of control data => Set undefined value */
         area_loc.addressOffset = 0;
         area_loc.size          = 0;
      }
      else if (!FLASH_CFG_GetFlashAreaLocation(pxstPartProps->ui32CtrlFlashAreaID, 
                                               &area_loc))
      {
         mTBOX_TRACE((kTBOX_NIV_WARNING,"%s(%c,%s,%s) ERROR: Get Flash Area "
                      "%u location of Data Blocks of '%s' failure\n",
                      TBOX_FunctionName,pxstPartProps->cDevice,
                      mFS_CFG_STRBOOL(xbReset),
                      mFS_CFG_STRBOOL(xbRetryWithReset),
                      pxstPartProps->ui32CtrlFlashAreaID,
                      pxstPartProps->pstrLabel));
         mTBOX_RETURN(FALSE);
      }
#ifdef _DATAMNGR_JFS_RAM_PARTITION_
      ctrl_loc.pxui8RAM         = NULL;
      ctrl_loc.ui32BlockSize    = 0;
#endif
      ctrl_loc.ui32Address      = area_loc.addressOffset;
      ctrl_loc.ui32Size         = area_loc.size;
      ctrl_loc.ui32HeaderSize   = pxstPartProps->ui32CtrlSizePartHeader;
#ifdef _DATAMNGR_JFS_RAM_PARTITION_
      mTBOX_TRACE((kTBOX_NIV_2,"- Ctrl.RAM.........%#x\n",(unsigned int)ctrl_loc.pxui8RAM));
#endif
      mTBOX_TRACE((kTBOX_NIV_2,"- Ctrl.Address.....%#x\n",ctrl_loc.ui32Address));
      mTBOX_TRACE((kTBOX_NIV_2,"- Ctrl.Size........%#x\n",ctrl_loc.ui32Size));
      mTBOX_TRACE((kTBOX_NIV_2,"- Ctrl.HeaderSize..%#x\n",ctrl_loc.ui32HeaderSize));

      /* Mount */
      rc = DMJFS_Mount(pxstPartProps->cDevice,
                       &props,&data_loc,&ctrl_loc,xbReset);
      if (rc != eDMJFS_NO_ERROR)
      {
         mTBOX_TRACE((kTBOX_NIV_WARNING,"%s(%c,%s,%s) ERROR: "
                      "Mount '%s' failure (error %d: %s)\n",
                      TBOX_FunctionName,pxstPartProps->cDevice,
                      mFS_CFG_STRBOOL(xbReset),
                      mFS_CFG_STRBOOL(xbRetryWithReset),
                      pxstPartProps->pstrLabel,
                      rc,DMJFS_LabelError(rc)));

         if ((!xbReset) && (xbRetryWithReset))
         {
            mTBOX_TRACE((kTBOX_NIV_1,"%s(%c,%s,%s): Retry mount '%s' "
                         "with reset...\n",
                         TBOX_FunctionName,pxstPartProps->cDevice,
                         mFS_CFG_STRBOOL(xbReset),
                         mFS_CFG_STRBOOL(xbRetryWithReset),
                         pxstPartProps->pstrLabel));

            rc = DMJFS_Mount(pxstPartProps->cDevice,
                             &props,&data_loc,&ctrl_loc,TRUE);
            if (rc == eDMJFS_NO_ERROR)
            {
               mTBOX_TRACE((kTBOX_NIV_1,"%s(%c,%s,%s): Retry Mount and "
                            "Reset '%s' DONE\n",
                            TBOX_FunctionName,pxstPartProps->cDevice,
                            mFS_CFG_STRBOOL(xbReset),
                            mFS_CFG_STRBOOL(xbRetryWithReset),
                            pxstPartProps->pstrLabel));
               mTBOX_RETURN(TRUE);
            }
            else
            {
               mTBOX_TRACE((kTBOX_NIV_WARNING,"%s(%c,%s,%s) ERROR: "
                            "Retry Mount and Reset '%s' "
                            "failure (error %d: %s)\n",
                            TBOX_FunctionName,pxstPartProps->cDevice,
                            mFS_CFG_STRBOOL(xbReset),
                            mFS_CFG_STRBOOL(xbRetryWithReset),
                            pxstPartProps->pstrLabel,
                            rc,DMJFS_LabelError(rc)));
            }
         }

         mTBOX_RETURN(FALSE);
      }

      mTBOX_TRACE((kTBOX_NIV_1,"%s(%c,%s,%s): '%s' MOUNTED\n",
                   TBOX_FunctionName,pxstPartProps->cDevice,
                   mFS_CFG_STRBOOL(xbReset),
                   mFS_CFG_STRBOOL(xbRetryWithReset),
                   pxstPartProps->pstrLabel));
   }
   else
   {
      /* Mount alias partition */
      mTBOX_TRACE((kTBOX_NIV_1,"%s(%c,%s,%s): Create Alias to '%c' = '%s'...\n",
                   TBOX_FunctionName,pxstPartProps->cDevice,
                   mFS_CFG_STRBOOL(xbReset),
                   mFS_CFG_STRBOOL(xbRetryWithReset),
                   pxstPartProps->cAliasToDevice,
                   pxstPartProps->pstrLabel));

      rc = DMJFS_MountAlias(pxstPartProps->cDevice,
                            pxstPartProps->cAliasToDevice);
      if (rc != eDMJFS_NO_ERROR)
      {
         mTBOX_TRACE((kTBOX_NIV_WARNING,"%s(%c,%s,%s) ERROR: "
                      "Create Alias to '%c' = '%s' failure (error %d: %s)\n",
                      TBOX_FunctionName,pxstPartProps->cDevice,
                      mFS_CFG_STRBOOL(xbReset),
                      mFS_CFG_STRBOOL(xbRetryWithReset),
                      pxstPartProps->cAliasToDevice,
                      pxstPartProps->pstrLabel,rc,DMJFS_LabelError(rc)));
         mTBOX_RETURN(FALSE);
      }

      mTBOX_TRACE((kTBOX_NIV_1,"%s(%c,%s,%s): Alias to '%c' = '%s' CREATED\n",
                   TBOX_FunctionName,pxstPartProps->cDevice,
                   mFS_CFG_STRBOOL(xbReset),
                   mFS_CFG_STRBOOL(xbRetryWithReset),
                   pxstPartProps->cAliasToDevice,
                   pxstPartProps->pstrLabel));
   }

   mTBOX_RETURN(TRUE);
}

#if defined (kFS_CFG_PART_A_IN_RAM)
LOCAL bool FS_CFGi_MountPartFromRAM
(
   tFS_CFG_PartitionProps *pxstPartProps,
   uint8_t                *pxui8PartRAM
)
{
   tDMJFS_Error          rc;
   tDMJFS_PartitionProps props;
   tDMJFS_FlashLocation  data_loc,ctrl_loc;
   tFLASH_CFG_Location   area_loc;

   mTBOX_FCT_ENTER(FS_CFGi_MountPartFromRAM);

   if (DMJFS_IsMounted(pxstPartProps->cDevice))
   {
      mTBOX_TRACE((kTBOX_NIV_WARNING,"%s(%c,%#x) WARNING: "
                   "Partition %s already mounted\n",
                   TBOX_FunctionName,pxstPartProps->cDevice,
                   (unsigned int)pxui8PartRAM,
                   pxstPartProps->pstrLabel));
      mTBOX_RETURN(TRUE);
   }

   if (pxstPartProps->cAliasToDevice == kDMJFS_PART_DEVICE_UNDEF)
   {
      /* Mount partition */
      mTBOX_TRACE((kTBOX_NIV_1,"%s(%c,%#x): Mount '%s' from RAM...\n",
                   TBOX_FunctionName,pxstPartProps->cDevice,
                   (unsigned int)pxui8PartRAM,
                   pxstPartProps->pstrLabel));

      /* Format properties */
      props.bIsEnableFileCRC    = pxstPartProps->bIsEnableFileCRC;
      props.ui8RecRetryLoadCD   = pxstPartProps->ui8RecRetryLoadCD;
      props.ui8RecRetryLoadFile = pxstPartProps->ui8RecRetryLoadFile;
      props.ui8NumRetrySaveCD   = pxstPartProps->ui8NumRetrySaveCD;
      props.ui8NumRetrySaveFile = pxstPartProps->ui8NumRetrySaveFile;
      props.ui16MinNumDataBlocks = pxstPartProps->ui16DataMinNumBlocks;
      props.ui16MinNumCtrlBlocks = pxstPartProps->ui16CtrlMinNumBlocks;
      props.ui16MaxNumFiles     = pxstPartProps->ui16MaxNumFiles;
      props.ui16MaxNumDir       = pxstPartProps->ui16MaxNumDir;

      if (pxstPartProps->bIsRessources)
      {
         props.ePartType           = eDMJFS_PT_RESSOURCES;
         props.ui32MaxSizeCtrlData = pxstPartProps->ui32MaxSizeCData;
         props.bIsReadOnly         = TRUE;
      }
      else
      {
         props.ePartType           = eDMJFS_PT_STANDARD;
         props.ui32MaxSizeCtrlData = 0;
         props.bIsReadOnly         = TRUE;
      }

      mTBOX_TRACE((kTBOX_NIV_2,"- Reset............FALSE\n"));
      mTBOX_TRACE((kTBOX_NIV_2,"- PartType.........%d\n",props.ePartType));
      mTBOX_TRACE((kTBOX_NIV_2,"- IsReadOnly.......TRUE\n"));
      mTBOX_TRACE((kTBOX_NIV_2,"- IsEnableFileCRC..%s\n",mFS_CFG_STRBOOL(props.bIsEnableFileCRC)));
      mTBOX_TRACE((kTBOX_NIV_2,"- RecRetryLoadCD...%u\n",props.ui8RecRetryLoadCD));
      mTBOX_TRACE((kTBOX_NIV_2,"- RecRetryLoadFile.%u\n",props.ui8RecRetryLoadFile));
      mTBOX_TRACE((kTBOX_NIV_2,"- NumRetrySaveCD...%u\n",props.ui8NumRetrySaveCD));
      mTBOX_TRACE((kTBOX_NIV_2,"- NumRetrySaveFile.%u\n",props.ui8NumRetrySaveFile));
      mTBOX_TRACE((kTBOX_NIV_2,"- MinNumDataBlocks.%u\n",props.ui16MinNumDataBlocks));
      mTBOX_TRACE((kTBOX_NIV_2,"- MinNumCtrlBlocks.%u\n",props.ui16MinNumCtrlBlocks));
      mTBOX_TRACE((kTBOX_NIV_2,"- MaxSizeCtrlData..%u\n",props.ui32MaxSizeCtrlData));
      mTBOX_TRACE((kTBOX_NIV_2,"- MaxNumFiles......%u\n",props.ui16MaxNumFiles));
      mTBOX_TRACE((kTBOX_NIV_2,"- MaxNumDir........%u\n",props.ui16MaxNumDir));

      /* Set location of Data blocks  */
      if (!FLASH_CFG_GetFlashAreaLocation(pxstPartProps->ui32DataFlashAreaID, 
                                          &area_loc))
      {
         mTBOX_TRACE((kTBOX_NIV_WARNING,"%s(%c,%#x) ERROR: Get Flash Area "
                      "%u location of Data Blocks of '%s' failure\n",
                      TBOX_FunctionName,pxstPartProps->cDevice,
                      (unsigned int)pxui8PartRAM,
                      pxstPartProps->ui32DataFlashAreaID,
                      pxstPartProps->pstrLabel));
         mTBOX_RETURN(FALSE);
      }

      data_loc.pxui8RAM         = pxui8PartRAM;
      data_loc.ui32BlockSize    = 0x10000;
      data_loc.ui32Address      = area_loc.addressOffset;
      data_loc.ui32Size         = area_loc.size;
      data_loc.ui32HeaderSize   = pxstPartProps->ui32DataSizePartHeader;
      mTBOX_TRACE((kTBOX_NIV_2,"- Data.RAM.........%#x\n",(unsigned int)data_loc.pxui8RAM));
      mTBOX_TRACE((kTBOX_NIV_2,"- Data.Address.....%#x\n",data_loc.ui32Address));
      mTBOX_TRACE((kTBOX_NIV_2,"- Data.Size........%#x\n",data_loc.ui32Size));
      mTBOX_TRACE((kTBOX_NIV_2,"- Data.HeaderSize..%#x\n",data_loc.ui32HeaderSize));

      ctrl_loc.pxui8RAM         = NULL;
      ctrl_loc.ui32BlockSize    = 0;
      ctrl_loc.ui32Address      = 0;
      ctrl_loc.ui32Size         = 0;
      ctrl_loc.ui32HeaderSize   = 0;
      mTBOX_TRACE((kTBOX_NIV_2,"- Ctrl.RAM.........%#x\n",(unsigned int)ctrl_loc.pxui8RAM));
      mTBOX_TRACE((kTBOX_NIV_2,"- Ctrl.Address.....%#x\n",ctrl_loc.ui32Address));
      mTBOX_TRACE((kTBOX_NIV_2,"- Ctrl.Size........%#x\n",ctrl_loc.ui32Size));
      mTBOX_TRACE((kTBOX_NIV_2,"- Ctrl.HeaderSize..%#x\n",ctrl_loc.ui32HeaderSize));

      /* Mount */
      rc = DMJFS_Mount(pxstPartProps->cDevice,
                       &props,&data_loc,&ctrl_loc,FALSE);
      if (rc != eDMJFS_NO_ERROR)
      {
         mTBOX_TRACE((kTBOX_NIV_WARNING,"%s(%c,%#x) ERROR: "
                      "Mount '%s' from RAM failure (error %d: %s)\n",
                      TBOX_FunctionName,pxstPartProps->cDevice,
                      (unsigned int)pxui8PartRAM,
                      pxstPartProps->pstrLabel,
                      rc,DMJFS_LabelError(rc)));
         mTBOX_RETURN(FALSE);
      }

      mTBOX_TRACE((kTBOX_NIV_1,"%s(%c,%#x): '%s' MOUNTED\n",
                   TBOX_FunctionName,pxstPartProps->cDevice,
                   (unsigned int)pxui8PartRAM,
                   pxstPartProps->pstrLabel));
   }
   else
   {
      /* Mount alias partition */
      mTBOX_TRACE((kTBOX_NIV_1,"%s(%c,%#x): Create Alias to '%c' = '%s'...\n",
                   TBOX_FunctionName,pxstPartProps->cDevice,
                   (unsigned int)pxui8PartRAM,
                   pxstPartProps->cAliasToDevice,
                   pxstPartProps->pstrLabel));

      rc = DMJFS_MountAlias(pxstPartProps->cDevice,
                            pxstPartProps->cAliasToDevice);
      if (rc != eDMJFS_NO_ERROR)
      {
         mTBOX_TRACE((kTBOX_NIV_WARNING,"%s(%c,%#x) ERROR: "
                      "Create Alias to '%c' = '%s' failure (error %d: %s)\n",
                      TBOX_FunctionName,pxstPartProps->cDevice,
                      (unsigned int)pxui8PartRAM,
                      pxstPartProps->cAliasToDevice,
                      pxstPartProps->pstrLabel,rc,DMJFS_LabelError(rc)));
         mTBOX_RETURN(FALSE);
      }

      mTBOX_TRACE((kTBOX_NIV_1,"%s(%c,%#x): Alias to '%c' = '%s' CREATED\n",
                   TBOX_FunctionName,pxstPartProps->cDevice,
                   (unsigned int)pxui8PartRAM,
                   pxstPartProps->cAliasToDevice,
                   pxstPartProps->pstrLabel));
   }

   mTBOX_RETURN(TRUE);
}
#endif

/**
 *  @brief
 *    Unmount a partition.
 *
 *  @param[in]     pxstPartProps
 *       Properties of partition to mount.
 *
 *  @retval  TRUE
 *       Unmount successful
 *
 *  @retval  FALSE
 *       Unmount failure
 *
*/
LOCAL bool FS_CFGi_UnmountPart
(
   tFS_CFG_PartitionProps *pxstPartProps
)
{
   tDMJFS_Error rc = eDMJFS_NO_ERROR;

   mTBOX_FCT_ENTER(FS_CFGi_UnmountPart);

   if (DMJFS_IsMounted(pxstPartProps->cDevice))
   {
      /* Unmount partition */
      mTBOX_TRACE((kTBOX_NIV_1,"%s(%c): Unmount '%s'...\n",
                   TBOX_FunctionName,pxstPartProps->cDevice,
                   pxstPartProps->pstrLabel));

      rc = DMJFS_Unmount(pxstPartProps->cDevice);
      if (rc != eDMJFS_NO_ERROR)
      {
         mTBOX_TRACE((kTBOX_NIV_WARNING,"%s(%c) ERROR: "
                      "Unmount '%s' failure (error %d: %s)\n",
                      TBOX_FunctionName,pxstPartProps->cDevice,
                      pxstPartProps->pstrLabel,
                      rc,DMJFS_LabelError(rc)));
         mTBOX_RETURN(FALSE);
      }

      mTBOX_TRACE((kTBOX_NIV_1,"%s(%c): '%s' UNMOUNTED\n",
                   TBOX_FunctionName,pxstPartProps->cDevice,
                   pxstPartProps->pstrLabel));
   }
   else
   {
      mTBOX_TRACE((kTBOX_NIV_1,"%s(%c): '%s' not mounted\n",
                   TBOX_FunctionName,pxstPartProps->cDevice,
                   pxstPartProps->pstrLabel));
   }

   mTBOX_RETURN(TRUE);
}

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
 *  @param[in]     pxstPartProps
 *       Properties of partition to reset.
 *
 *  @retval  TRUE
 *       Reset successful
 *
 *  @retval  FALSE
 *       Reset failure
 *
*/
LOCAL bool FS_CFGi_Reset
(
   tFS_CFG_PartitionProps *pxstPartProps
)
{
   bool is_mounted   = FALSE;

   mTBOX_FCT_ENTER(FS_CFGi_Reset);

   /* If partition mounted, unmount it */
   is_mounted = DMJFS_IsMounted(pxstPartProps->cDevice);
   if (is_mounted)
   {
      mTBOX_TRACE((kTBOX_NIV_1,"%s(%c): Unmount partition '%s'\n",
                   TBOX_FunctionName,pxstPartProps->cDevice,
                   pxstPartProps->pstrLabel));

      if (!FS_CFGi_UnmountPart(pxstPartProps))
      {
         mTBOX_TRACE((kTBOX_NIV_CRITICAL,"%s(%c) ERROR: Unmount '%s' failure\n",
                      TBOX_FunctionName,pxstPartProps->cDevice,
                      pxstPartProps->pstrLabel));
         mTBOX_RETURN(FALSE);
      }
   }

   /* Mount it in reset mode */
   mTBOX_TRACE((kTBOX_NIV_1,"%s(%c): Mount/Reset partition '%s'\n",
                TBOX_FunctionName,pxstPartProps->cDevice,
                pxstPartProps->pstrLabel));
   if (!FS_CFGi_MountPart(pxstPartProps,TRUE,TRUE))
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"%s(%c) ERROR: "
                   "Reset partition '%s' failure\n",
                   TBOX_FunctionName,pxstPartProps->cDevice,
                   pxstPartProps->pstrLabel));
      mTBOX_RETURN(FALSE);
   }

   /* Unmount partition if not mouted at calling */
   if (!is_mounted) 
   {
      mTBOX_TRACE((kTBOX_NIV_1,"%s(%c): Unmount partition '%s'\n",
                   TBOX_FunctionName,pxstPartProps->cDevice,
                   pxstPartProps->pstrLabel));
      FS_CFGi_UnmountPart(pxstPartProps);
   }

   mTBOX_TRACE((kTBOX_NIV_1,"%s(%c): Partition '%s' RESETED\n",
                TBOX_FunctionName,pxstPartProps->cDevice,
                pxstPartProps->pstrLabel));

   /* Okay */
   mTBOX_RETURN(TRUE);
}

/**
 *  @brief
 *    Clean the files of a partition if the specfic method is implemented.
 *
 *  @param[in]     pxstPartProps
 *       Properties of partition to clean.
 *
 *  @retval  TRUE
 *       Clean successful
 *
 *  @retval  FALSE
 *       Clean failure
 *
*/
LOCAL bool FS_CFGi_CleanFiles
(
   tFS_CFG_PartitionProps *pxstPartProps
)
{
   mTBOX_FCT_ENTER(FS_CFGi_CleanFiles);

#ifdef kFS_CFG_PART_A
   if (pxstPartProps->cDevice == kFS_CFG_PART_A)
   {
      mTBOX_TRACE((kTBOX_NIV_WARNING,"%s(%c) WARNING: "
                   "Not implemented on Partition '%s'\n",
                   TBOX_FunctionName,pxstPartProps->cDevice,
                   pxstPartProps->pstrLabel));
      mTBOX_RETURN(FALSE);
   }
#endif

#ifdef kFS_CFG_PART_B
   if (pxstPartProps->cDevice == kFS_CFG_PART_B)
   {
      /**
       * Del Tree 'Comedia', possible only if Path defined
       * and exist.
      */
#ifdef kFS_CFG_CM_DIRNAME
      tDMJFS_Error    rc;
      tDMJFS_DirInfos dir_info;

      rc = DMJFS_DirInfos(pxstPartProps->cDevice,kFS_CFG_CM_DIRNAME,
                          &dir_info);
      if (rc == eDMJFS_ERROR_DIR_NOT_FOUND)
      {
         /* Okay directory not exist */
         mTBOX_TRACE((kTBOX_NIV_1,"%s(%c): Directory '%s' of Partition '%s' "
                      "not exist => Clean OKAY\n",
                      TBOX_FunctionName,pxstPartProps->cDevice,
                      kFS_CFG_CM_DIRNAME,pxstPartProps->pstrLabel));
         mTBOX_RETURN(TRUE);
      }
      if (rc != eDMJFS_NO_ERROR)
      {
         mTBOX_TRACE((kTBOX_NIV_WARNING,"%s(%c) ERROR: Get Directory '%s' of "
                      "Partition '%s' infos failure (error %d)\n",
                      TBOX_FunctionName,pxstPartProps->cDevice,
                      kFS_CFG_CM_DIRNAME,pxstPartProps->pstrLabel));
         mTBOX_RETURN(FALSE);
      }
      
      /* Del Tree Comedia */
      mTBOX_TRACE((kTBOX_NIV_1,"%s(%c): Delete Directory and Tree '%s' of "
                   "Partition '%s' (%u subdirs, %u files, %u bytes)\n",
                   TBOX_FunctionName,pxstPartProps->cDevice,
                   kFS_CFG_CM_DIRNAME,pxstPartProps->pstrLabel,
                   dir_info.ui32TotalDirs,dir_info.ui32TotalFiles,
                   dir_info.ui32TotalSize));

      rc = DMJFS_DeleteDir(pxstPartProps->cDevice,kFS_CFG_CM_DIRNAME,TRUE);
      if (rc != eDMJFS_NO_ERROR)
      {
         mTBOX_TRACE((kTBOX_NIV_WARNING,"%s(%c) ERROR: Del Tree '%s' of "
                      "Partition '%s' failure (error %d)\n",
                      TBOX_FunctionName,pxstPartProps->cDevice,
                      kFS_CFG_CM_DIRNAME,pxstPartProps->pstrLabel));
         mTBOX_RETURN(FALSE);
      }

      /* Okay */
      mTBOX_TRACE((kTBOX_NIV_1,"%s(%c): Delete Tree '%s' of Partition '%s' DONE\n",
                   TBOX_FunctionName,pxstPartProps->cDevice,
                   kFS_CFG_CM_DIRNAME,pxstPartProps->pstrLabel));

      mTBOX_RETURN(TRUE);
#else
      mTBOX_TRACE((kTBOX_NIV_WARNING,"%s(%c) WARNING: Not implemented "
                   "on Partition '%s' without Comedia path defined\n",
                   TBOX_FunctionName,pxstPartProps->cDevice,
                   pxstPartProps->pstrLabel));
      mTBOX_RETURN(FALSE);
#endif
   }
#endif

#ifdef kFS_CFG_PART_N
   if (pxstPartProps->cDevice == kFS_CFG_PART_N)
   {
      if (pxstPartProps->cAliasToDevice == kDMJFS_PART_DEVICE_UNDEF)
      {
         /* Not a alias => Delete all files */
         mTBOX_TRACE((kTBOX_NIV_1,"%s(%c): Delete all files of Partition '%s'...\n",
                      TBOX_FunctionName,pxstPartProps->cDevice,
                      pxstPartProps->pstrLabel));

         mTBOX_RETURN(FS_CFGi_CleanFilesDLK(pxstPartProps,NULL));
      }
      else
      {
#ifdef kFS_CFG_CM_DIRNAME
         /* Alias on B => Delete all files except Comedia Dir*/
         mTBOX_TRACE((kTBOX_NIV_1,"%s(%c): Delete all files of Partition "
                      "'%s' alias on %c except dir '%s'...\n",
                      TBOX_FunctionName,pxstPartProps->cDevice,
                      pxstPartProps->pstrLabel,pxstPartProps->cAliasToDevice,
                      kFS_CFG_CM_DIRNAME));
         mTBOX_RETURN(FS_CFGi_CleanFilesDLK(pxstPartProps,kFS_CFG_CM_DIRNAME));
#else
         /* Alias on B but no Comedia Dir => Delete all files */
         mTBOX_TRACE((kTBOX_NIV_1,"%s(%c): Delete all files of Partition "
                      "'%s' alias on %c...\n",
                      TBOX_FunctionName,pxstPartProps->cDevice,
                      pxstPartProps->pstrLabel,pxstPartProps->cAliasToDevice));
         mTBOX_RETURN(FS_CFGi_CleanFilesDLK(pxstPartProps,NULL));
#endif
      }
   }
#endif

   mTBOX_TRACE((kTBOX_NIV_CRITICAL,"%s(%c) ERROR: PARTITION NOT SUPPORTED\n",
                TBOX_FunctionName,pxstPartProps->cDevice));
   mTBOX_RETURN(FALSE);
}

#ifdef kFS_CFG_PART_N
/**
 *  @brief
 *    Clean the files of the DLK data base.
 *    Scan the dir and clean all file and directory except the
 *    comedia DIR if defined
 *
 *  @param[in]     pxstPartProps
 *       Properties of partition to clean.
 *
 *  @param[in]     pxcComediaDir
 *       Comedia Directory (NULL if undefined).
 *
 *  @retval  TRUE
 *       Clean successful
 *
 *  @retval  FALSE
 *       Clean failure
 *
*/
LOCAL bool FS_CFGi_CleanFilesDLK
(
   tFS_CFG_PartitionProps *pxstPartProps,
   char                   *pxcComediaDir
)
{
   oDMJFS_Dir      o_dir;
   tDMJFS_DirEntry entry;
   tDMJFS_Error    rc = eDMJFS_NO_ERROR;
   bool            is_cm;
   char            path[255+1];

   mTBOX_FCT_ENTER(FS_CFGi_CleanFilesDLK);

   mTBOX_TRACE((kTBOX_NIV_1,"%s(%c,%s): Clean files...\n",
                TBOX_FunctionName,pxstPartProps->cDevice,
                mFS_CFG_GETSTR(pxcComediaDir)));
   o_dir = DMJFS_DirOpen(pxstPartProps->cDevice,"/");
   if (o_dir == NULL)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"%s(%c,%s) ERROR: Open dir '/' failure\n",
                   TBOX_FunctionName,pxstPartProps->cDevice,
                   mFS_CFG_GETSTR(pxcComediaDir)));
      mTBOX_RETURN(FALSE);
   }

   do
   {
      rc = DMJFS_DirRead(o_dir,&entry);

      if (rc == eDMJFS_NO_ERROR)
      {
         if (entry.pcName == NULL)
         {
            mTBOX_TRACE((kTBOX_NIV_WARNING,"%s(%c,%s) WARNING: "
                         "Entry name NULL (type %d)\n",
                         TBOX_FunctionName,pxstPartProps->cDevice,
                         mFS_CFG_GETSTR(pxcComediaDir),entry.eType));
         }
         else if (strlen(entry.pcName) < 1)
         {
            mTBOX_TRACE((kTBOX_NIV_WARNING,"%s(%c,%s) WARNING: "
                         "Entry name empty (type %d)\n",
                         TBOX_FunctionName,pxstPartProps->cDevice,
                         mFS_CFG_GETSTR(pxcComediaDir),entry.eType));
         }
         else
         {
            sprintf(path,"/%s",entry.pcName);
            switch (entry.eType)
            {
            case eDMJFS_DIR_ENTRY_FILE:
               mTBOX_TRACE((kTBOX_NIV_1,"%s(%c,%s): "
                            "Delete file '%s'\n",
                            TBOX_FunctionName,pxstPartProps->cDevice,
                            mFS_CFG_GETSTR(pxcComediaDir),path));
               rc = DMJFS_FileDelete(pxstPartProps->cDevice,path);
               if (rc != eDMJFS_NO_ERROR)
               {
                  mTBOX_TRACE((kTBOX_NIV_WARNING,"%s(%c,%s) WARNING: "
                               "Delete file '%s' failure (error %d))\n",
                               TBOX_FunctionName,pxstPartProps->cDevice,
                               mFS_CFG_GETSTR(pxcComediaDir),path,rc));
                  /* Continue */
                  rc = eDMJFS_NO_ERROR;
               }
               break;

            case eDMJFS_DIR_ENTRY_DIR:
               if (pxcComediaDir == NULL) is_cm = FALSE;
               else is_cm = (strcmp(pxcComediaDir,path) == 0);

               if (is_cm)
               {
                  mTBOX_TRACE((kTBOX_NIV_1,"%s(%c,%s): "
                               "Not delete comedia directory '%s'\n",
                               TBOX_FunctionName,pxstPartProps->cDevice,
                               mFS_CFG_GETSTR(pxcComediaDir),path));
               }
               else
               {
                  mTBOX_TRACE((kTBOX_NIV_1,"%s(%c,%s): "
                               "Delete tree  '%s'\n",
                               TBOX_FunctionName,pxstPartProps->cDevice,
                               mFS_CFG_GETSTR(pxcComediaDir),path));
                  rc = DMJFS_DeleteDir(pxstPartProps->cDevice,path,TRUE);
                  if (rc != eDMJFS_NO_ERROR)
                  {
                     mTBOX_TRACE((kTBOX_NIV_WARNING,"%s(%c,%s) WARNING: "
                                  "Delete tree '%s' failure (error %d))\n",
                                  TBOX_FunctionName,pxstPartProps->cDevice,
                                  mFS_CFG_GETSTR(pxcComediaDir),path,rc));
                     /* Continue */
                     rc = eDMJFS_NO_ERROR;
                  }

               }
               break;

            default:
               mTBOX_TRACE((kTBOX_NIV_WARNING,"%s(%c,%s) WARNING: "
                            "Entry '%s' type UNDEFINED\n",
                            TBOX_FunctionName,pxstPartProps->cDevice,
                            mFS_CFG_GETSTR(pxcComediaDir),path));
            }
         }
      }
   }
   while (rc == eDMJFS_NO_ERROR);

   DMJFS_DirClose(o_dir);

   if ((rc == eDMJFS_NO_ERROR) || (rc == eDMJFS_ERROR_REFUSED))
   {
      /* Okay */
      mTBOX_TRACE((kTBOX_NIV_1,"%s(%c,%s): Clean files DONE\n",
                   TBOX_FunctionName,pxstPartProps->cDevice,
                   mFS_CFG_GETSTR(pxcComediaDir)));
      mTBOX_RETURN(TRUE);
   }

   mTBOX_TRACE((kTBOX_NIV_WARNING,"%s(%c,%s) WARNING: "
                "Scan dir '/' failure (error %d)\n",
                TBOX_FunctionName,pxstPartProps->cDevice,
                mFS_CFG_GETSTR(pxcComediaDir),rc));

   mTBOX_RETURN(FALSE);
}
#endif

#if defined(NEWLAND_BOOTLOADER) || defined(CHANGHONG_BOOTLOADER) || defined(SKYWORTH_BOOTLOADER) || defined(TELESYSTEM_BOOTLOADER) || defined(TELESYSTEM_MAP34_BOOTLOADER) || defined(TELESYSTEM_MAP51_BOOTLOADER) || defined(TECHNICOLOR_BOOTLOADER) || defined(KAON_MSD5C59_BOOTLOADER)

#ifdef CAK_DLK
#ifdef MIGRATION_N_PARTITION
void FS_CFGi_ErasePartitionBCtrlBlock(void)
{
    tFLASH_CFG_Location storage_location;
    uint32_t uiBlockSize;
	uint8_t i = 0;

    mTBOX_FCT_ENTER("FS_CFGi_ErasePartitionBCtrlBlock");
    FLASH_CFG_GetFlashAreaLocation( eFLASH_CFG_FLASH_AREA_PARTITION_B_CTRLBLOCK, &storage_location ) ;

    uiBlockSize = TDAL_FLA_GetBlockSize(TDAL_FLA_GetBlockNumber(storage_location.addressOffset));

	for(i = 0; (i*uiBlockSize) < storage_location.size ; i++)
	{
		TDAL_FLA_Erase(storage_location.addressOffset+ i*uiBlockSize, uiBlockSize);
	}
    mTBOX_RETURN;
}

void FS_CFGi_MovePartitionNCtrlBlock(void)
{
    tFLASH_CFG_Location storage_location;
    uint32_t read,uiBlockSize;
    uint8_t *pucBuffer = NULL;
	uint8_t i = 0;

    mTBOX_FCT_ENTER("FS_CFGi_MovePartitionNCtrlBlock");

    FLASH_CFG_GetFlashAreaLocation( eFLASH_CFG_FLASH_AREA_PARTITION_N_CTRLBLOCK, &storage_location ) ;

    uiBlockSize = TDAL_FLA_GetBlockSize(TDAL_FLA_GetBlockNumber(storage_location.addressOffset));

	if(TKEL_Malloc(uiBlockSize, (void * *)&pucBuffer) != TKEL_NO_ERR)
    {
         mTBOX_TRACE((kTBOX_NIV_CRITICAL,"Error allocating memory\n"));
         mTBOX_RETURN;
    }
	else
	{
		for(i = 0; (i*uiBlockSize) < storage_location.size ; i++)
		{
			memset(pucBuffer, 0xFF, uiBlockSize);
			read = TDAL_FLA_Read(N_CTRL_BLOCK_MIGRATION_ADDRESS + (i * uiBlockSize), pucBuffer,uiBlockSize);
			if(read != uiBlockSize)
			{
				 TKEL_Free(pucBuffer);
			}

			TDAL_FLA_Erase(storage_location.addressOffset+ i*uiBlockSize, uiBlockSize);
			read = TDAL_FLA_Write(storage_location.addressOffset+ (i*uiBlockSize),pucBuffer,uiBlockSize);
			if(read !=uiBlockSize)
			{
				 mTBOX_TRACE((kTBOX_NIV_CRITICAL,"Error readin from NVM ERROR\n"));
			//     mTBOX_RETURN;
			}
		}
	}
	TKEL_Free(pucBuffer);
}

void FS_CFGi_MovePartitionN(void)
{
    tFLASH_CFG_Location storage_location;
    uint32_t read,magic_flash,uiBlockSize;
    uint8_t *pucBuffer = NULL;
	uint8_t i = 0;

    mTBOX_FCT_ENTER("FS_CFGi_MovePartitionN");

    FLASH_CFG_GetFlashAreaLocation( eFLASH_CFG_FLASH_AREA_PARTITION_N, &storage_location ) ;

    uiBlockSize = TDAL_FLA_GetBlockSize(TDAL_FLA_GetBlockNumber(storage_location.addressOffset));

	if(TKEL_Malloc(uiBlockSize, (void * *)&pucBuffer) != TKEL_NO_ERR)
    {
         mTBOX_TRACE((kTBOX_NIV_CRITICAL,"Error allocating memory\n"));
         mTBOX_RETURN;
    }
	else
	{

		for(i = 0; (i*uiBlockSize) < storage_location.size ; i++)
		{
			memset(pucBuffer, 0xFF, uiBlockSize);
			read = TDAL_FLA_Read(N_PARTITION_MIGRATION_ADDRESS + (i * uiBlockSize), pucBuffer,uiBlockSize);
			if(read != uiBlockSize)
			{
				 TKEL_Free(pucBuffer);
			}

			TDAL_FLA_Erase(storage_location.addressOffset+ i*uiBlockSize, uiBlockSize);
			read = TDAL_FLA_Write(storage_location.addressOffset+ i*uiBlockSize,pucBuffer,uiBlockSize);
			if(read !=uiBlockSize)
			{
				 mTBOX_TRACE((kTBOX_NIV_CRITICAL,"Error readin from NVM ERROR\n"));
			//     mTBOX_RETURN;
			}
		}
	}
	TKEL_Free(pucBuffer);
}

void FS_CFGi_MigrationNPartition()
{
    mTBOX_FCT_ENTER("FS_CFGi_MigrationNPartition");

	tFLASH_CFG_Location storage_location;
	uint32_t read = 0,uiBlockSize = 0;
	uint8_t *pucBuffer = NULL;
	static uint8_t secure_conversion_id[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x44, 0x4D, 0x4A, 0x46};
    FLASH_CFG_GetFlashAreaLocation( eFLASH_CFG_FLASH_AREA_PARTITION_N_CTRLBLOCK, &storage_location ) ;
    uiBlockSize = TDAL_FLA_GetBlockSize(TDAL_FLA_GetBlockNumber(storage_location.addressOffset));

	if(TKEL_Malloc(uiBlockSize, (void * *)&pucBuffer) != TKEL_NO_ERR)
    {
         mTBOX_TRACE((kTBOX_NIV_CRITICAL,"Error allocating memory\n"));
    }
	else
	{
		read = TDAL_FLA_Read(storage_location.addressOffset, pucBuffer,uiBlockSize);
		if(read != uiBlockSize)
		{
			 mTBOX_TRACE((kTBOX_NIV_CRITICAL,"Error readin from FLASH ERROR\n"));
		}
		else
		{
			if(memcmp(&secure_conversion_id,pucBuffer,12 * sizeof(uint8_t)) != 0)
			{
				FS_CFGi_ErasePartitionBCtrlBlock();
				FS_CFGi_MovePartitionNCtrlBlock();
				FS_CFGi_MovePartitionN();
			}
		}
		TKEL_Free(pucBuffer);
	}
	mTBOX_RETURN;
}
#endif
#ifdef NEWLAND_MERLIN_CONVERT

void FS_CFGi_ConvertNagraData()
{

    tFLASH_CFG_Location stPartNLoc;
    uint32_t magic_flash, read ,uiBlockSize;
    uint8_t* pucBlockBuffer = NULL;



    mTBOX_FCT_ENTER("FS_CFGi_ConvertNagraNVM");

    FLASH_CFG_GetFlashAreaLocation(eFLASH_CFG_FLASH_AREA_PARTITION_N_CTRLBLOCK ,&stPartNLoc );
    
    
    read = TDAL_FLA_Read(stPartNLoc.addressOffset, (uint8_t *)&magic_flash ,sizeof(uint32_t) );
    if(read != sizeof(uint32_t))
    {
         mTBOX_TRACE((kTBOX_NIV_CRITICAL,"Error readin from FLASH ERROR\n"));
         mTBOX_RETURN;
    }

    magic_flash = mMSBF_GET32((uint8_t*)&magic_flash);

    if (magic_flash != NEWLAND_NAGRA_CONVERT_MAGIC )    
    {
        
        FS_CFGi_ConvertNagraNVM();
        FS_CFGi_ConvertNagraVFS();


        // SAFE Magic writing

        
        uiBlockSize = TDAL_FLA_GetBlockSize(TDAL_FLA_GetBlockNumber(stPartNLoc.addressOffset));
        
        TKEL_Malloc(uiBlockSize, (void **)&pucBlockBuffer);
        
        TDAL_FLA_Read(stPartNLoc.addressOffset, pucBlockBuffer,uiBlockSize );

        magic_flash = NEWLAND_NAGRA_CONVERT_MAGIC;

        //Write Magic value after this we consider unpacking sucesfull and CRCs will match 
        mMSBF_SET32(pucBlockBuffer,magic_flash);

        // ERASE whole block 
        TDAL_FLA_Erase(stPartNLoc.addressOffset, uiBlockSize);

        //Write all but CRC area 
        read = TDAL_FLA_Write( stPartNLoc.addressOffset + sizeof(uint32_t) ,pucBlockBuffer + sizeof(uint32_t),uiBlockSize - sizeof(uint32_t));
        if(read !=  uiBlockSize - sizeof(uint32_t))
        {
            mTBOX_TRACE((kTBOX_NIV_CRITICAL,"Error readin from FLASH ERROR\n"));
            mTBOX_RETURN;
        }

        //Finally write CRC and only CRC so here we are sure that whole block is in there OK 
        read = TDAL_FLA_Write( stPartNLoc.addressOffset ,pucBlockBuffer ,sizeof(uint32_t) );
        if(read != sizeof(uint32_t) )
        {
            mTBOX_TRACE((kTBOX_NIV_CRITICAL,"Error readin from FLASH ERROR\n"));
            mTBOX_RETURN;
        }
        
                
    }
    else
    {
       mTBOX_TRACE((kTBOX_NIV_CRITICAL,"NAGRA magic  MATCHING no conversion\n"));
    }


    
}


void FS_CFGi_ConvertNagraNVM()
{

    tFLASH_CFG_Location storage_location;
    uint32_t read;
    uint8_t *pucBuffer , *pucEraseBuff = NULL;


    mTBOX_FCT_ENTER("FS_CFGi_ConvertNagraNVM");
    
    FLASH_CFG_GetNvmAreaLocation( eFLASH_CFG_NVM_AREA_CA_2, &storage_location ) ;

    //Allocate memory to read packed block
    if(TKEL_Malloc(NEWLAND_SEC_FLASH_SIZE, (void * *)&pucBuffer) != TKEL_NO_ERR)
    {
         mTBOX_TRACE((kTBOX_NIV_CRITICAL,"Error allocating memory\n"));
         mTBOX_RETURN;
    }
    else
    {
        if(TKEL_Malloc(NEWLAND_SEC_FLASH_SIZE, (void * *)&pucEraseBuff) != TKEL_NO_ERR)
        {
             TKEL_Free(pucBuffer);
             mTBOX_TRACE((kTBOX_NIV_CRITICAL,"Error allocating memory\n"));
             mTBOX_RETURN;
        }

        memset(pucEraseBuff, 0xFF, NEWLAND_SEC_FLASH_SIZE);

        // Load Newland Secure flash
        read = TDAL_FLA_Read(NEWLAND_SEC_FLASH_ADDRESS, pucBuffer,NEWLAND_SEC_FLASH_SIZE);
        if(read != NEWLAND_SEC_FLASH_SIZE)
        {
             TKEL_Free(pucEraseBuff);
             TKEL_Free(pucBuffer);
             mTBOX_TRACE((kTBOX_NIV_CRITICAL,"Error readin from FLASH ERROR\n"));
             mTBOX_RETURN;
        }

        //Check is erased 
        if(memcmp(pucEraseBuff,pucBuffer,NEWLAND_SEC_FLASH_SIZE) == 0)
        {
            read = TDAL_FLA_Read(NEWLAND_SEC_FLASH_BAK_ADDRESS, pucBuffer,NEWLAND_SEC_FLASH_SIZE);
            if(read != NEWLAND_SEC_FLASH_SIZE)
            {
                 TKEL_Free(pucEraseBuff);
                 TKEL_Free(pucBuffer);
                 mTBOX_TRACE((kTBOX_NIV_CRITICAL,"Error readin from FLASH ERROR\n"));
                 mTBOX_RETURN;
            }            
        }

       
       if(memcmp(pucEraseBuff,pucBuffer,NEWLAND_SEC_FLASH_SIZE) != 0)
       {

            read = TDAL_NVM_Write(storage_location.addressOffset,storage_location.size,pucBuffer);

            if(read !=storage_location.size)
            {
                 mTBOX_TRACE((kTBOX_NIV_CRITICAL,"Error readin from NVM ERROR\n"));
            //     mTBOX_RETURN;
            }   
       }
       TKEL_Free(pucEraseBuff); 
       TKEL_Free(pucBuffer);   
    }



}

void FS_CFGi_ConvertNagraVFS()
{
    uint32_t ui32StartAddress;
    uint32_t ui32EndAddress;
    uint32_t ui32CurPartSize;
    tFS_CFGi_File stFile;
    uint8_t  device = 'N';

    mTBOX_FCT_ENTER("FS_CFGi_ConvertNagraVFS");

#if defined(IMCL)
    ui32CurPartSize 	= kFS_CFG_PARTITION_N_SIZE;
    ui32StartAddress 	= kFS_CFG_PARTITION_N_START;
#elif defined(GTPL)
    ui32CurPartSize 	= kFS_CFG_PARTITION_N_SIZE - 0x10000;
    ui32StartAddress 	= kFS_CFG_PARTITION_N_START_PART1;
#endif

    if (!FS_CFG_Mount(device, FALSE))
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL,"Failed to mount device: %s.\n", device));

        mTBOX_RETURN;
    }

    if (FS_CFGi_GetFile(ui32StartAddress, &stFile, &ui32EndAddress) == FALSE)
    {
        /* reading first file failed, switching to alternative partition */
    #if defined(IMCL)
        ui32StartAddress = kFS_CFG_PARTITION_N_START_ALTERNATIVE;
    #elif defined(GTPL)
        ui32StartAddress = kFS_CFG_PARTITION_N_START_PART1_ALTER;
    #endif
    }

    FS_CFGi_CopyToDMJFS(device, stFile, ui32StartAddress, ui32CurPartSize);
#if defined(GTPL)
    ui32CurPartSize = 0x10000;
    if (ui32StartAddress == kFS_CFG_PARTITION_N_START_PART1)
        ui32StartAddress = kFS_CFG_PARTITION_N_START_PART2;
    else
        ui32StartAddress = kFS_CFG_PARTITION_N_START_PART2_ALTER;

    FS_CFGi_CopyToDMJFS(device, stFile, ui32StartAddress, ui32CurPartSize);
#endif

    if (!FS_CFG_Unmount(device))
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL,"Failed to unmount device: %s.\n", device));

        mTBOX_RETURN;
    }

}

void FS_CFGi_CopyToDMJFS(uint8_t device,
                        tFS_CFGi_File stFile,
                        uint32_t ui32StartAddress,
                        uint32_t ui32CurPartSize)
{
	uint8_t  slash = '/';
	uint8_t *pos_last_slash;
    oDMJFS_Dir  stDmjfsDir;
    oDMJFS_File stDmjfsFile;
    tDMJFS_Error error;
    uint32_t bytes_written;
    uint32_t ui32EndAddress;

    uint32_t ui32CurrentAddress = ui32StartAddress;
    while( ui32CurrentAddress + sizeof(tFS_CFGi_FileName) < ui32StartAddress + ui32CurPartSize &&
            FS_CFGi_GetFile(ui32CurrentAddress, &stFile, &ui32EndAddress))
    {
        /* Creating directory if necessary */
        /* find last slash in file name */
        pos_last_slash = strrchr(stFile.file_name, slash);

        if (pos_last_slash != stFile.file_name)
        {
            *pos_last_slash = 0; /* temporary terminate string */

            stDmjfsDir = DMJFS_DirOpen(device, stFile.file_name);
            if (stDmjfsDir == NULL && *stFile.file_name != '\0')
            {
                /* create directory */
                error = DMJFS_CreateDir(device, stFile.file_name , TRUE);
                if (error != eDMJFS_NO_ERROR)
                {
                    mTBOX_TRACE((kTBOX_NIV_CRITICAL,"Failed to create directory path %s (error:%u).\n",
                            stFile.file_name, error));
                }
                else
                {
                    mTBOX_TRACE((kTBOX_NIV_5,"Created directory path %s.\n",
                            stFile.file_name));
                }
            }
            else
            {
                mTBOX_TRACE((kTBOX_NIV_5,"Opened directory path %s.\n",
                       stFile.file_name));
                DMJFS_DirClose(stDmjfsDir);
            }

            *pos_last_slash = slash; /* recover original filename path */
        }

        /* Creating new file on partition N  */
        stDmjfsFile =  DMJFS_FileOpen(device, stFile.file_name, "w");
        if (stDmjfsFile != NULL)
        {
            bytes_written = DMJFS_FileWrite(stFile.file_data,
                    1,
                    stFile.file_size,
                    stDmjfsFile);
            if(bytes_written != stFile.file_size)
            {
                mTBOX_TRACE((kTBOX_NIV_CRITICAL,"Failed to write all data! Written %u of %u.\n",
                        bytes_written,
                        stFile.file_size));
            }
            else
            {
                mTBOX_TRACE((kTBOX_NIV_5,"Data written %u to %s.\n",
                        bytes_written,
                        stFile.file_name));
            }

            error = DMJFS_FileClose(stDmjfsFile);
            if(error != eDMJFS_NO_ERROR)
            {
                mTBOX_TRACE((kTBOX_NIV_CRITICAL,"Failed to close file %s.\n", stFile.file_name));
            }
        }
        else
        {
            mTBOX_TRACE((kTBOX_NIV_CRITICAL,"Failed to open file %s for writing.\n", stFile.file_name));
        }

        ui32CurrentAddress = ui32EndAddress; /* address of new file to read */
    }
}

/**
 *  @brief
 *       Calculates CRC32 for give memory buffer.
 *
 *  @param[in]     pxui8Data
 *       Pointer to buffer for CRC32 calculation.
 *
 *  @param[in]     xi32Size
 *       Size of buffer.
 *
 *  @retval
 *       CRC32 value.
*/

LOCAL uint32_t FS_CFGi_GenerateCRC32(uint8_t *pxui8Data,
                                     uint32_t xi32Size)
{
#if defined(GTPL)
    uint32_t oldcrc32 = 0;
    uint32_t crc32 = 0;
    uint32_t oldcrc = 0;
    uint32_t charcnt = 0;
    uint8_t c = 0;
    uint8_t t = 0;
    oldcrc32 = 0x00000000;
    charcnt = 0;
    while (xi32Size--)
    {
        t = (uint8_t) (oldcrc32 >> 24) & 0xFF;
        oldcrc = Section_crc_32_tab[t];
        c = pxui8Data[charcnt];
        oldcrc32 = (oldcrc32 << 8) | c;
        oldcrc32 = oldcrc32 ^ oldcrc;
        charcnt++;
    }
    crc32 = oldcrc32;
    return crc32;
#elif defined(IMCL)
    uint32_t u32Crc = 0xffffffff;
    uint32_t i = 0;

    if (pxui8Data == NULL)
    {
	    return 0;
    }

    for (i = 0; i < xi32Size; i++)
    {
        u32Crc = Section_crc_32_tab[((u32Crc>>24) ^ pxui8Data[i]) & 0xff] ^ (u32Crc << 8);
    }

    return u32Crc;
#endif
}

/**
 *  @brief
 *       Gets file data for given start address.
 *
 *  @param[in]     xui32StartAddress
 *       Starting address of file to read.
 *
 *  @param[out]    pxstFile
 *       File structure with filled information
 *  @param[out]    pxui32EndAddress
 *       Starting address of next file.
 *
 *
 *  @retval  TRUE
 *       File read successful.
 *
 *  @retval  FALSE
 *       File read fail.
 *
*/

LOCAL bool FS_CFGi_GetFile(uint32_t        xui32StartAddress,
                           tFS_CFGi_File   *pxstFile,
                           uint32_t        *pxui32EndAddress)
{
    uint32_t currentAddress = xui32StartAddress;
    uint32_t bytesRead      = 0;

    TKEL_err errorTKEL      = TKEL_NO_ERR;
    bool     bIsOK          = TRUE;

    mTBOX_FCT_ENTER("FS_CFGi_GetFile");

    if (pxui32EndAddress == NULL || pxstFile == NULL)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Bad parametars."));
        mTBOX_RETURN(FALSE);
    }

    memset(pxstFile, 0, sizeof(tFS_CFGi_File));

    if (bIsOK == TRUE)
    {
        bytesRead = TDAL_FLA_Read(currentAddress, pxstFile->file_name, sizeof(tFS_CFGi_FileName));
        if (bytesRead != sizeof(tFS_CFGi_FileName) || pxstFile->file_name[0] == '\0')
        {
            bIsOK = FALSE;
            mTBOX_TRACE((kTBOX_NIV_CRITICAL,"Failed to read from flash address %X.\n", currentAddress));
        }
        currentAddress += sizeof(tFS_CFGi_FileName);
    }

    if (bIsOK == TRUE)
    {
        bytesRead = TDAL_FLA_Read(currentAddress, (uint8_t *)&pxstFile->file_size, sizeof(uint32_t));
        if (bytesRead != sizeof(uint32_t))
        {
            bIsOK = FALSE;
            mTBOX_TRACE((kTBOX_NIV_CRITICAL,"Failed to read from flash address %X.\n", currentAddress));
        }
        currentAddress += sizeof(uint32_t);
    }

    if ( pxstFile->file_size > kFS_CFG_PARTITION_N_SIZE )
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL,
                "Irregular file size %u bytes. Size exceeds remaining partition space of %u bytes. Aborting!\n",
                pxstFile->file_size, kFS_CFG_PARTITION_N_SIZE));
        bIsOK = FALSE;
    }

    if (bIsOK == TRUE)
    {
        errorTKEL = TKEL_Malloc(pxstFile->file_size, (void ** )&pxstFile->file_data);
        if (errorTKEL != TKEL_NO_ERR)
        {
            bIsOK = FALSE;
            mTBOX_TRACE((kTBOX_NIV_CRITICAL,"Failed to allocate memory block of size %u bytes.\n", pxstFile->file_size));
        }
    }

    if (bIsOK == TRUE)
    {
        uint32_t blockAddress;
        uint32_t blockSize;

        uint32_t size_to_read = pxstFile->file_size;

        while (size_to_read>0 && bIsOK == TRUE)
        {
            blockSize    = TDAL_FLA_GetBlockSize(TDAL_FLA_GetBlockNumber(currentAddress));
            blockAddress = TDAL_FLA_GetBlockAddress(TDAL_FLA_GetBlockNumber(currentAddress));

            blockSize -= currentAddress - blockAddress; /* space to end of block */

            if (size_to_read < blockSize)
            {
                blockSize = size_to_read; /* read just remaining file space */
            }

            bytesRead = TDAL_FLA_Read(currentAddress, pxstFile->file_data, blockSize);
            if (bytesRead != blockSize)
            {
                bIsOK = FALSE;
                mTBOX_TRACE((kTBOX_NIV_CRITICAL,"Failed to read from flash address %X (%u bytes).\n", currentAddress, size_to_read));
            }
            size_to_read   -= blockSize;
            currentAddress += blockSize;
        }
    }

    if (bIsOK == TRUE)
    {
        bytesRead = TDAL_FLA_Read(currentAddress, (uint8_t *)&pxstFile->crc32, sizeof(uint32_t));
        if (bytesRead != sizeof(uint32_t))
        {
            bIsOK = FALSE;
            mTBOX_TRACE((kTBOX_NIV_CRITICAL,"Failed to read from flash address %X.\n", currentAddress));
        }
        currentAddress += sizeof(uint32_t);
    }

    if (bIsOK == TRUE)
    {
        uint8_t *crc_buffer;

        errorTKEL = TKEL_Malloc(sizeof(tFS_CFGi_FileName)+sizeof(uint32_t)+pxstFile->file_size, (void ** )&crc_buffer);
        if (errorTKEL != TKEL_NO_ERR)
        {
            bIsOK = FALSE;
            mTBOX_TRACE((kTBOX_NIV_CRITICAL,"Failed to allocate crc memory buffer of size %u bytes.\n",
                         sizeof(tFS_CFGi_FileName)+sizeof(uint32_t)+pxstFile->file_size));
        }

        memcpy(crc_buffer, pxstFile->file_name, sizeof(tFS_CFGi_FileName));

        memcpy(crc_buffer+sizeof(tFS_CFGi_FileName), &pxstFile->file_size, sizeof(uint32_t));

        memcpy(crc_buffer+sizeof(tFS_CFGi_FileName)+sizeof(uint32_t), pxstFile->file_data, pxstFile->file_size);

        /* check CRC32 */
        if (FS_CFGi_GenerateCRC32(crc_buffer, sizeof(tFS_CFGi_FileName)+sizeof(uint32_t)+pxstFile->file_size) != pxstFile->crc32)
        {
            bIsOK = FALSE;
            mTBOX_TRACE((kTBOX_NIV_CRITICAL,"Failed CRC32 check for file %s of size %u bytes.\n", pxstFile->file_name, pxstFile->file_size ));
        }
        *pxui32EndAddress = currentAddress;

        errorTKEL = TKEL_Free(crc_buffer);
    }

    if (bIsOK == TRUE)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL,"File read %s of size %u bytes.\n", pxstFile->file_name, pxstFile->file_size));
    }
    else if (pxstFile->file_data != NULL)
    {
        /* file read failed, if we allocated memory we will free it here */
        errorTKEL = TKEL_Free(pxstFile->file_data);
        pxstFile->file_data = NULL;
    }

    mTBOX_RETURN(bIsOK);
}
#endif //NEWLAND_MERLIN_CONVERT

#endif  //CAK_DLK

bool FS_CFGi_UnpackPart(tFLASH_CFG_Pack_Area ePackedId,tFLASH_CFG_FLASH_Area SourceId, tFLASH_CFG_FLASH_Area DestinationId)
{

    tFLASH_CFG_Location stPartDestination, stPackedLoc, stPartSource;
    uint32_t uiPackSize,uiFinalSize, read;
    uint8_t *pucPackedBuffer = NULL, *pucBuffer= NULL;
    int32_t zlib_report;

    mTBOX_FCT_ENTER("FS_CFGi_UnpackPart");

    FLASH_CFG_GetPackingAreaLocation(ePackedId, &stPackedLoc);
    FLASH_CFG_GetFlashAreaLocation(SourceId, &stPartSource);
    FLASH_CFG_GetFlashAreaLocation(DestinationId, &stPartDestination);

    if(stPackedLoc.size > 0)
    {
        //Unpacking partition
        
        uiFinalSize = stPartDestination.size;
        
        // Read packed block size

        read = TDAL_FLA_Read(stPartSource.addressOffset +  stPackedLoc.addressOffset , (uint8_t *)&uiPackSize,sizeof(uint32_t));
        if(read != sizeof(uint32_t))
        {
             mTBOX_TRACE((kTBOX_NIV_CRITICAL,"Error readin from FLASH ERROR\n"));
             mTBOX_RETURN(FALSE);
        }
        uiPackSize = mMSBF_GET32((uint8_t*)&uiPackSize);

        mTBOX_TRACE((kTBOX_NIV_1,"Packed size %x\n",uiPackSize));   

        //Allocate memory to read packed block
        if(TKEL_Malloc(uiPackSize, (void * *)&pucPackedBuffer) != TKEL_NO_ERR)
        {
             mTBOX_TRACE((kTBOX_NIV_CRITICAL,"Error allocating memory\n"));
             mTBOX_RETURN(FALSE);
        }
        else
        {
            // Read packed blocks, data packed optima and on several blocks

            uint32_t offset = 0 ,startAddr = 0, readPart = 0, blockSize = 0, blockAddr = 0;
        
        
            startAddr = stPartSource.addressOffset +  stPackedLoc.addressOffset + 2*sizeof(uint32_t);
            blockSize = TDAL_FLA_GetBlockSize(TDAL_FLA_GetBlockNumber(startAddr));
            blockAddr = TDAL_FLA_GetBlockAddress(TDAL_FLA_GetBlockNumber(startAddr));
            readPart = blockSize - (startAddr - blockAddr);

            if(readPart > uiPackSize)
            {
                readPart = uiPackSize;
            }

            read = TDAL_FLA_Read(startAddr , pucPackedBuffer + offset, readPart);
            offset += readPart;
            startAddr += offset;
            readPart = ((uiPackSize - offset) > blockSize) ? blockSize : (uiPackSize - offset) ;
        
            while(offset < uiPackSize)
            {
                read = TDAL_FLA_Read(startAddr , pucPackedBuffer + offset, readPart);
                offset += readPart;
                startAddr += readPart;
                readPart = ((uiPackSize - offset) > blockSize) ? blockSize : (uiPackSize - offset) ;
            }
        }              
        
        //Allocate memory for decompression
        if (TKEL_Malloc(UNZIP_BUFFER_SIZE, (void * *) &pucBuffer) != TKEL_NO_ERR)
        {
            TKEL_Free(pucPackedBuffer);
            mTBOX_TRACE((kTBOX_NIV_CRITICAL,"Error allocating memory\n"));
            mTBOX_RETURN(FALSE);
        }

        //Prepare decompression stream similarly to what is done in uncompr.c:uncompress function
        z_stream stream;
        int inflateStatus;

        stream.next_in = (Bytef*) pucPackedBuffer;
        stream.avail_in = (uInt) uiPackSize;

        stream.zalloc = (alloc_func) 0;
        stream.zfree = (free_func) 0;

        inflateStatus = inflateInit(&stream);
        if (inflateStatus != Z_OK)
        {
            TKEL_Free(pucPackedBuffer);
            TKEL_Free(pucBuffer);
            mTBOX_TRACE((kTBOX_NIV_CRITICAL,"Error initializing decompression\n"));
            mTBOX_RETURN(FALSE);
        }

        //First block to write to is the first block of partition
        uint16_t currentBlock = TDAL_FLA_GetBlockNumber(stPartDestination.addressOffset);

        //Decompress data into pucBuffer repeatedly until everything is decompressed
        do
        {
            uint16_t tmpBlock;
            //Calculate the number of blocks that can fit into a buffer and memory they will take
            //Although blocks are generally the same size, calculation is always done to be aligned
            //with CHAL API which allows different block sizes
            uint16_t blocksInBuffer = 0;
            uint32_t usedMemoryInBuffer = 0;

            for (tmpBlock = currentBlock;; tmpBlock++)
            {
                uint32_t blockSize = TDAL_FLA_GetBlockSize(currentBlock);
                if (usedMemoryInBuffer + blockSize <= UNZIP_BUFFER_SIZE)
                {
                    blocksInBuffer++;
                    usedMemoryInBuffer += blockSize;
                }
                else
                {
                    break;
                }
            }

            //Decompress data into pucBuffer
            stream.next_out = (Bytef*) pucBuffer;
            stream.avail_out = (uInt) usedMemoryInBuffer;

            inflateStatus = inflate(&stream, Z_SYNC_FLUSH);

            // If decompression failed or if total decompressed data is bigger than the destination partition, abort decompression
            if ( (inflateStatus != Z_OK && inflateStatus != Z_STREAM_END) || stream.total_out > stPartDestination.size)
            {
                inflateStatus = inflateEnd(&stream);
                TKEL_Free(pucPackedBuffer);
                TKEL_Free(pucBuffer);
                mTBOX_TRACE((kTBOX_NIV_CRITICAL,"Error in decompression\n"));
                mTBOX_RETURN(FALSE);
            }

            //Write decompressed data to flash block by block, first erasing
            uint32_t offset = 0;
            for (tmpBlock = currentBlock; tmpBlock < TDAL_FLA_GetBlockNumber(stPartDestination.addressOffset + stream.total_out); tmpBlock++)
            {
                TDAL_FLA_Erase(TDAL_FLA_GetBlockAddress(tmpBlock), TDAL_FLA_GetBlockSize(tmpBlock));
                read = TDAL_FLA_Write(TDAL_FLA_GetBlockAddress(tmpBlock), pucBuffer + offset, TDAL_FLA_GetBlockSize(tmpBlock));

                offset += TDAL_FLA_GetBlockSize(tmpBlock);

                if (read != TDAL_FLA_GetBlockSize(tmpBlock))
                {
                    inflateStatus = inflateEnd(&stream);
                    TKEL_Free(pucPackedBuffer);
                    TKEL_Free(pucBuffer);
                    mTBOX_TRACE((kTBOX_NIV_CRITICAL,"Error writing to flash\n"));

                    mTBOX_RETURN(FALSE);
                }
            }

            //Next block for writing is the first one after the last written block
            currentBlock = TDAL_FLA_GetBlockNumber(stPartDestination.addressOffset + stream.total_out);

        }while(inflateStatus != Z_STREAM_END);

        inflateEnd(&stream);
        TKEL_Free(pucPackedBuffer);
        TKEL_Free(pucBuffer);
        mTBOX_TRACE((kTBOX_NIV_CRITICAL,"Partition %d decompression SUCCESS\n",DestinationId));
    }

    mTBOX_RETURN(TRUE);
}

void FS_CFGi_UnpackFS( )
{

    mTBOX_FCT_ENTER("FS_CFGi_UnpackFS");

    mTBOX_TRACE((kTBOX_NIV_1,"UNPACKING C DATA\n" ));
    FS_CFGi_CheckPartChange(eFLASH_CFG_PACK_PART_C_DATA_AREA, eFLASH_CFG_FLASH_AREA_APPLICATION_PARTITION_MIRROR_1, eFLASH_CFG_FLASH_AREA_EXTERN_PARTITION);

    mTBOX_TRACE((kTBOX_NIV_1,"UNPACKING C CTRL\n" ));
    FS_CFGi_CheckPartChange(eFLASH_CFG_PACK_PART_C_CTRL_AREA, eFLASH_CFG_FLASH_AREA_APPLICATION_PARTITION_MIRROR_1, eFLASH_CFG_FLASH_AREA_EXTERN_PARTITION_CTRL_BLOCK);

    mTBOX_TRACE((kTBOX_NIV_1,"UNPACKING A partition\n" ));
    FS_CFGi_CheckPartChange(eFLASH_CFG_PACK_PART_A_AREA, eFLASH_CFG_FLASH_AREA_APPLICATION_PARTITION_MIRROR_1, eFLASH_CFG_FLASH_AREA_RESOURCES_PARTITION_MIRROR_1);

}

void FS_CFGi_CheckPartChange(tFLASH_CFG_Pack_Area ePackedId,tFLASH_CFG_FLASH_Area SourceId, tFLASH_CFG_FLASH_Area DestinationId)
{
    tFLASH_CFG_Location stPartALoc, stPackALoc, stExeLoc;
    uint8_t crc_packed[4], crc_flash[4];
    uint32_t crc32_flash, crc32_pack, read, uiBlockSize;
    uint8_t* pucBlockBuffer = NULL;
    mTBOX_FCT_ENTER("FS_CFGi_CheckPartChange");

    /* Offset for partition inside OTA image relative to beginning of OTA image */
    FLASH_CFG_GetPackingAreaLocation(ePackedId, &stPackALoc);
    /* Offset for executable(Maestro + Comedia, zipped) inside full flash image relative to start address of full flash map */
    FLASH_CFG_GetFlashAreaLocation(SourceId, &stExeLoc );
    /* Offset for partition inside full flash image relative to start address of full flash map */
    FLASH_CFG_GetFlashAreaLocation(DestinationId, &stPartALoc );

    mTBOX_TRACE((kTBOX_NIV_1,"Read CRC from address %x (image)\n", stExeLoc.addressOffset + stPackALoc.addressOffset + sizeof(uint32_t)  ));
    read = TDAL_FLA_Read(stExeLoc.addressOffset + stPackALoc.addressOffset + sizeof(uint32_t) , (uint8_t *)crc_packed,sizeof(uint32_t));
    if(read != sizeof(uint32_t))
    {
         mTBOX_TRACE((kTBOX_NIV_CRITICAL,"Error reading from FLASH ERROR\n"));
         mTBOX_RETURN;
    }

    mTBOX_TRACE((kTBOX_NIV_1,"Read CRC from address %x (flash)\n", stPartALoc.addressOffset  ));
    read = TDAL_FLA_Read(stPartALoc.addressOffset , (uint8_t *)crc_flash,sizeof(uint32_t));
    if(read != sizeof(uint32_t))
    {
         mTBOX_TRACE((kTBOX_NIV_CRITICAL,"Error reading from FLASH ERROR\n"));
         mTBOX_RETURN;
    }

    crc32_flash = mMSBF_GET32(crc_flash);
    crc32_pack = mMSBF_GET32(crc_packed);

    mTBOX_TRACE((kTBOX_NIV_1,"CRC in flash %x and CRC in image %x \n", crc32_flash, crc32_pack ));

    /* Checks if packed OTA is different(newer) from unpacked OTA:
     * If CRC inside OTA area in flash is different than CRC from
     * beginning of unpacked partition area then that sufficiently
     * indices that executable is not compatible with partition, so
     * new OTA unpacking must be started */
    if (crc32_flash != crc32_pack )
    {

        mTBOX_TRACE((kTBOX_NIV_1,"UNPACKING PARTITION %d DATA\n",DestinationId));
        if (FS_CFGi_UnpackPart(ePackedId, SourceId, DestinationId) != TRUE)
        {
            mTBOX_TRACE((kTBOX_NIV_CRITICAL,"Error unpacking partition %d\n",DestinationId));
            mTBOX_RETURN;
        }

        // SAFE CRC writing
        uiBlockSize = TDAL_FLA_GetBlockSize(TDAL_FLA_GetBlockNumber(stPartALoc.addressOffset));

        TKEL_Malloc(uiBlockSize, (void **)&pucBlockBuffer);

        TDAL_FLA_Read(stPartALoc.addressOffset, pucBlockBuffer, uiBlockSize );

        // Write CRC value after this we consider unpacking successful and CRCs will match
        mMSBF_SET32(pucBlockBuffer,crc32_pack);

        // ERASE whole block
        TDAL_FLA_Erase(stPartALoc.addressOffset, uiBlockSize );

        // Write all but CRC area
        read = TDAL_FLA_Write( stPartALoc.addressOffset + sizeof(uint32_t) ,pucBlockBuffer + sizeof(uint32_t) ,uiBlockSize - sizeof(uint32_t));
        if(read != uiBlockSize - sizeof(uint32_t) )
        {
            mTBOX_TRACE((kTBOX_NIV_CRITICAL,"Error writing from FLASH ERROR\n"));
            mTBOX_RETURN;
        }

        // Finally write CRC and only CRC so here we are sure that whole block is in there OK
        read = TDAL_FLA_Write( stPartALoc.addressOffset ,pucBlockBuffer, sizeof(uint32_t) );
        if(read != sizeof(uint32_t) )
        {
            mTBOX_TRACE((kTBOX_NIV_CRITICAL,"Error writing from FLASH ERROR\n"));
            mTBOX_RETURN;
        }
    }
    else
    {
       mTBOX_TRACE((kTBOX_NIV_CRITICAL,"Packed and FLASH version MATCHING no unpacking\n"));
    }

    mTBOX_RETURN;
}

void FS_CFG_ResetUnpackFS( )
{

    tFLASH_CFG_Location stPartALoc;
    uint32_t read;
    uint32_t pucEraseBuffer = 0xFFFFFFFF;
    
    
    mTBOX_FCT_ENTER("FS_CFGi_UnpackFS");



    FLASH_CFG_GetFlashAreaLocation(eFLASH_CFG_FLASH_AREA_RESOURCES_PARTITION_MIRROR_1,&stPartALoc );


 	//Finally write CRC and only CRC so here we are sure that whole block is in there OK 
	read = TDAL_FLA_Write( stPartALoc.addressOffset ,(uint8_t *)&pucEraseBuffer ,sizeof(uint32_t) );
	if(read != sizeof(uint32_t) )
 	{
		 mTBOX_TRACE((kTBOX_NIV_CRITICAL,"Error readin from FLASH ERROR\n"));
 	}



}


#endif

bool FS_CFG_DumpPartToUsb(uint32_t index)
{
		char xcDevice;

		switch(index)
		{
		case 0:
			xcDevice = 'A';
			break;
		case 1:
			xcDevice = 'B';
			break;
		case 2:
			xcDevice = 'C';
			break;
		case 3:
			xcDevice = 'N';
			break;
		}

	   tFS_CFG_PartitionProps *p_part_props = NULL;
	   tFLASH_CFG_Location pLocation;
	   tFLASH_CFG_Location pAreaLocation;
	   uint32_t BLOCK_SIZE = 0x10000;

	   mTBOX_FCT_ENTER(FS_CFG_DumpPartToUsb);

	   if (!vFS_CFGi_bIsInit)
	   {
	      mTBOX_TRACE((kTBOX_NIV_WARNING,"%s(%c) ERROR: "
	                   "F.S. Config not initialized\n",
	                   TBOX_FunctionName,xcDevice));
	      mTBOX_RETURN(FALSE);
	   }

	   /* Search the partition properties */
	   p_part_props = FS_CFGi_GetParition(xcDevice);

	   if (p_part_props == NULL)
	   {
	      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"%s(%c) ERROR: Partition not found\n",
	                   TBOX_FunctionName,xcDevice));
	      mTBOX_RETURN(FALSE);
	   }

	   if(FLASH_CFG_GetFlashLocation(&pLocation) == FALSE)
	   {
		   mTBOX_TRACE((kTBOX_NIV_CRITICAL,"%s ERROR: Could not get flash location\n",
		   	                   TBOX_FunctionName));
		   mTBOX_RETURN(FALSE);
	   }

	   if(FLASH_CFG_GetFlashAreaLocation(p_part_props->ui32DataFlashAreaID, &pAreaLocation) == FALSE)
	   {
		   mTBOX_TRACE((kTBOX_NIV_CRITICAL,"%s ERROR: Could not get flash area location\n",
				   	                   TBOX_FunctionName));
		   mTBOX_RETURN(FALSE);
	   }

	   char partName[32];
	   tTDAL_FS_File file;
	   int i;
	   for(i=1; i<100;i++){
		   sprintf(partName, "/Mount%d/partition_%c.bin", i, xcDevice);
		   file = TDAL_FS_Open(partName, kTDAL_FS_MODE_CREATE | kTDAL_FS_MODE_WRITE | kTDAL_FS_MODE_TRUNC);
		   if(file == kTDAL_FS_INVALID_HANDLE || file == -1)
		   {
			   continue;
		   }else{
			   break;
		   }
	   }

	   char *buffer;
	   if(TKEL_Malloc(BLOCK_SIZE, (void **) &buffer) != TKEL_NO_ERR)
	   {
		   mTBOX_TRACE((kTBOX_NIV_CRITICAL,"%s ERROR: Could not allocate memory for buffer\n",
		   				   	                   TBOX_FunctionName));
		   mTBOX_RETURN(FALSE);
	   }

	   uint32_t bytesRead = 0;
	   uint32_t bytesWritten = 0;
	   uint32_t counter;
	   tTDAL_FS_Size size;

	   for(counter = 0; counter < pAreaLocation.size; counter += BLOCK_SIZE){
		   if(TDAL_FLA_Read(pAreaLocation.addressOffset + counter, buffer, BLOCK_SIZE) != BLOCK_SIZE)
		   {
			   mTBOX_TRACE((kTBOX_NIV_WARNING,"%s ERROR: Error reading partition data\n",
														   TBOX_FunctionName));
			   mTBOX_RETURN(FALSE);
		   }

		   size = TDAL_FS_Write(file, buffer, BLOCK_SIZE);
		   if(size != BLOCK_SIZE)
		   {
			   mTBOX_TRACE((kTBOX_NIV_WARNING,"%s ERROR: Error writting partition data to usb\n",
			   														   TBOX_FunctionName));
			   mTBOX_RETURN(FALSE);
		   }
	   }

	   TDAL_FS_Close(file);

	   mTBOX_RETURN(TRUE);
}

/* file_system_cfg.c */

