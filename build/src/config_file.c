/**************************************************************************//**
** @file config_file.c
**
** @brief
**   Implementation API to read the configuration file stored in flash.
**
** @ingroup CONFIGURATION
**
** @see modules flash_cfg (build) and tdal_fla (chal).
**
** @version $Rev: $
**          $URL: $
**          $Date: $
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

#include "tdal_fla.h"
#include "flash_cfg.h"
#include "msbf_tools.h"
#include "config_file.h"
#include "nvm_interface_bootloader.h"


mTBOX_SET_MODULE(eCONFIG_FILE);


/******************************************************************************
* Defines
******************************************************************************/

/* 
* @brief Config file location in flash
*/
#define kCFGFi_ADDRESS  0x00102000     
#define kCFGFi_SIZEMAX  0x0001E000     

/* Offset of begining (after NASC and Bootloader header) */
#define kCFGFi_OFFSET      80     

/**
 *  Configuration sections and fields pre-defined size in bytes and values
*/

/* - Header section definition (size in bytes) */
#define kCFGFi_HEAD_VERSION_SIZE               4
#define kCFGFi_HEAD_PRODOFF_SIZE               2
#define kCFGFi_HEAD_LOCOFF_SIZE                2
#define kCFGFi_HEAD_DLDCOFF_SIZE               2
#define kCFGFi_HEAD_OSDOFF_SIZE                2
#define kCFGFi_HEAD_RFMODOFF_SIZE              2
#define kCFGFi_HEAD_VIDOUTOFF_SIZE             2
#define kCFGFi_HEAD_DLDOPTOFF_SIZE             2
#define kCFGFi_HEAD_SCANOFF_SIZE               2
#define kCFGFi_HEAD_RESERVED_SIZE             24
#define kCFGFi_HEAD_SIZE (kCFGFi_HEAD_VERSION_SIZE+  \
                          kCFGFi_HEAD_PRODOFF_SIZE+  \
                          kCFGFi_HEAD_LOCOFF_SIZE+   \
                          kCFGFi_HEAD_DLDCOFF_SIZE+  \
                          kCFGFi_HEAD_OSDOFF_SIZE+   \
                          kCFGFi_HEAD_RFMODOFF_SIZE+ \
                          kCFGFi_HEAD_VIDOUTOFF_SIZE+\
                          kCFGFi_HEAD_DLDOPTOFF_SIZE+\
                          kCFGFi_HEAD_SCANOFF_SIZE+  \
                          kCFGFi_HEAD_RESERVED_SIZE)

#define kCFGFi_HEAD_VERSION_001       0x00000001
#define kCFGFi_HEAD_VERSION_100       0x00000100
#define kCFGFi_HEAD_VERSION_101       0x00000101
#define kCFGFi_HEAD_VERSION_102       0x00000102
#define kCFGFi_HEAD_VERSION_200       0x00000200
#define kCFGFi_HEAD_VERSION_CURR   kCFGFi_HEAD_VERSION_200

/* - Product section definition since version 1.00 (size in bytes) */
#define kCFGFi_PROD_OUI_SIZE                   3
#define kCFGFi_PROD_PLTFMODL_SIZE              2
#define kCFGFi_PROD_PLTFVERS_SIZE              2
#define kCFGFi_PROD_PRODMODL_SIZE              2
#define kCFGFi_PROD_PRODVERS_SIZE              2
#define kCFGFi_PROD_BNAME_LEN_SIZE             1
#define kCFGFi_PROD_BNAME_LEN_MAX           0xFF
#define kCFGFi_PROD_FIXE_SIZE (kCFGFi_PROD_OUI_SIZE+kCFGFi_PROD_PLTFMODL_SIZE+kCFGFi_PROD_PLTFVERS_SIZE+kCFGFi_PROD_PRODMODL_SIZE+kCFGFi_PROD_PRODVERS_SIZE+kCFGFi_PROD_BNAME_LEN_SIZE)

/* - Product section definition version 0.01 (size in bytes) */
#define kCFGFi_PROD_V001_OUI_SIZE              3
#define kCFGFi_PROD_V001_HWID_SIZE             2
#define kCFGFi_PROD_V001_HWVERS_SIZE           2
#define kCFGFi_PROD_V001_SWID_SIZE             2
#define kCFGFi_PROD_V001_SWVERS_SIZE           2
#define kCFGFi_PROD_V001_SIZE (kCFGFi_PROD_V001_OUI_SIZE+kCFGFi_PROD_V001_HWID_SIZE+kCFGFi_PROD_V001_HWVERS_SIZE+kCFGFi_PROD_V001_SWID_SIZE+kCFGFi_PROD_V001_SWVERS_SIZE)

/* - RF Mod section definition (size in bytes) */
#define kCFGFi_RFMOD_OUTPUTID_SIZE             1
#define kCFGFi_RFMOD_BAND_STD_SIZE             1 // 4 bits for Band (b7..b4) + 4 bits for Standard (b3..b0)
#define kCFGFi_RFMOD_CHANNEL_SIZE              2
#define kCFGFi_RFMOD_I2CADDR_SIZE              1
#define kCFGFi_RFMOD_SIZE (kCFGFi_RFMOD_OUTPUTID_SIZE+kCFGFi_RFMOD_BAND_STD_SIZE+kCFGFi_RFMOD_CHANNEL_SIZE+kCFGFi_RFMOD_I2CADDR_SIZE)

/* - Location Table */
#define kCFGFi_LOCT_COUNT_SIZE              1
#define kCFGFi_LOCT_TYPE_SIZE               1

#define kCFGFi_LOCT_TYPE_SERIAL          0x00
#define kCFGFi_LOCT_TYPE_USB             0x01
#define kCFGFi_LOCT_TYPE_URL             0x02
#define kCFGFi_LOCT_TYPE_DVBT            0x10
#define kCFGFi_LOCT_TYPE_DVBT2           0x11
#define kCFGFi_LOCT_TYPE_DVBC            0x20
#define kCFGFi_LOCT_TYPE_DVBS            0x30
#define kCFGFi_LOCT_TYPE_DVBS2           0x31

/* - Location SERIAL definition (size in bytes) */
#define kCFGFi_SERIAL_PROP1_SIZE            1
#define kCFGFi_SERIAL_PROP2_SIZE            1
#define kCFGFi_SERIAL_SPEED_SIZE            1
#define kCFGFi_SERIAL_SIZE (kCFGFi_SERIAL_PROP1_SIZE+kCFGFi_SERIAL_PROP2_SIZE+kCFGFi_SERIAL_SPEED_SIZE)

/* - Location USB definition (size in bytes) */
#define kCFGFi_USB_PATHLEN_SIZE             1
#define kCFGFi_USB_SIZE_FIXE (kCFGFi_USB_PATHLEN_SIZE)

/* - Location URL definition (size in bytes) */
#define kCFGFi_URL_URL_SIZE                 1
#define kCFGFi_URL_SIZE_FIXE (kCFGFi_URL_URL_SIZE)

/* - Location DVB-T definition (size in bytes) */
#define kCFGFi_DVBT_PMTPID_SIZE               2
#define kCFGFi_DVBT_DSMPID_SIZE               2
#define kCFGFi_DVBT_FREQ_SIZE                 4
#define kCFGFi_DVBT_BANDW_SIZE                4
#define kCFGFi_DVBT_MODUL_SIZE                1
#define kCFGFi_DVBT_MODE_SIZE                 1
#define kCFGFi_DVBT_GUARD_SIZE                1
#define kCFGFi_DVBT_HIERA_SIZE                1
#define kCFGFi_DVBT_PRIO_SIZE                 1
#define kCFGFi_DVBT_CONVHP_SIZE               1
#define kCFGFi_DVBT_CONVLP_SIZE               1
#define kCFGFi_DVBT_FROFF_SIZE                1
#define kCFGFi_DVBT_SIZE (kCFGFi_DVBT_PMTPID_SIZE+kCFGFi_DVBT_DSMPID_SIZE+kCFGFi_DVBT_FREQ_SIZE+kCFGFi_DVBT_BANDW_SIZE+kCFGFi_DVBT_MODUL_SIZE+kCFGFi_DVBT_MODE_SIZE+kCFGFi_DVBT_GUARD_SIZE+kCFGFi_DVBT_HIERA_SIZE+kCFGFi_DVBT_PRIO_SIZE+kCFGFi_DVBT_CONVHP_SIZE+kCFGFi_DVBT_CONVLP_SIZE+kCFGFi_DVBT_FROFF_SIZE)

/* - Location DVB-T2 post V2 definition (size in bytes), not totaly specified, just for exemple but implemented */
#define kCFGFi_DVBT2_V1_PMTPID_SIZE              2
#define kCFGFi_DVBT2_V1_DSMPID_SIZE              2
#define kCFGFi_DVBT2_V1_SIZE (kCFGFi_DVBT2_V1_PMTPID_SIZE+kCFGFi_DVBT2_V1_DSMPID_SIZE)

/* - Location DVB-T2 definition (size in bytes), realy defined since version 2.00 */
#define kCFGFi_DVBT2_PMTPID_SIZE              2
#define kCFGFi_DVBT2_DSMPID_SIZE              2
#define kCFGFi_DVBT2_FREQ_SIZE                4
#define kCFGFi_DVBT2_BANDW_SIZE               4
#define kCFGFi_DVBT2_MODE_SIZE                1
#define kCFGFi_DVBT2_GUARD_SIZE               1
#define kCFGFi_DVBT2_PLPID_SIZE               1
#define kCFGFi_DVBT2_SIZE (kCFGFi_DVBT2_PMTPID_SIZE+kCFGFi_DVBT2_DSMPID_SIZE+kCFGFi_DVBT2_FREQ_SIZE+kCFGFi_DVBT2_BANDW_SIZE+kCFGFi_DVBT2_MODE_SIZE+kCFGFi_DVBT2_GUARD_SIZE+kCFGFi_DVBT2_PLPID_SIZE)

/* - Location DVB-C definition (size in bytes) */
#define kCFGFi_DVBC_PMTPID_SIZE             2
#define kCFGFi_DVBC_DSMPID_SIZE             2
#define kCFGFi_DVBC_NETID_SIZE              2
#define kCFGFi_DVBC_FREQ_SIZE               4
#define kCFGFi_DVBC_SYMBR_SIZE              4
#define kCFGFi_DVBC_MODUL_SIZE              1
#define kCFGFi_DVBC_SIZE (kCFGFi_DVBC_PMTPID_SIZE+kCFGFi_DVBC_DSMPID_SIZE+kCFGFi_DVBC_NETID_SIZE+kCFGFi_DVBC_FREQ_SIZE+kCFGFi_DVBC_SYMBR_SIZE+kCFGFi_DVBC_MODUL_SIZE)

/* - Location DVB-S definition (size in bytes) */
#define kCFGFi_DVBS_PMTPID_SIZE               2
#define kCFGFi_DVBS_DSMPID_SIZE               2
#define kCFGFi_DVBS_FREQ_SIZE                 4
#define kCFGFi_DVBS_SYMBR_SIZE                4
#define kCFGFi_DVBS_POLAR_SIZE                1
#define kCFGFi_DVBS_MODUL_SIZE                1
#define kCFGFi_DVBS_FECRATE_SIZE              1
#define kCFGFi_DVBS_OUTSEL_SIZE               1
#define kCFGFi_DVBS_SIZE (kCFGFi_DVBS_PMTPID_SIZE+kCFGFi_DVBS_DSMPID_SIZE+kCFGFi_DVBS_FREQ_SIZE+kCFGFi_DVBS_SYMBR_SIZE+kCFGFi_DVBS_POLAR_SIZE+kCFGFi_DVBS_MODUL_SIZE+kCFGFi_DVBS_FECRATE_SIZE+kCFGFi_DVBS_OUTSEL_SIZE)

/* - Location DVB-S2 definition (size in bytes) */
/* Note: Not totaly specified, prepare it with same DVB-S structure */
#define kCFGFi_DVBS2_PMTPID_SIZE              2
#define kCFGFi_DVBS2_DSMPID_SIZE              2
#define kCFGFi_DVBS2_FREQ_SIZE                4
#define kCFGFi_DVBS2_SYMBR_SIZE               4
#define kCFGFi_DVBS2_POLAR_SIZE               1
#define kCFGFi_DVBS2_MODUL_SIZE               1
#define kCFGFi_DVBS2_FECRATE_SIZE             1
#define kCFGFi_DVBS2_OUTSEL_SIZE              1
#define kCFGFi_DVBS2_SIZE (kCFGFi_DVBS2_PMTPID_SIZE+kCFGFi_DVBS2_DSMPID_SIZE+kCFGFi_DVBS2_FREQ_SIZE+kCFGFi_DVBS2_SYMBR_SIZE+kCFGFi_DVBS2_POLAR_SIZE+kCFGFi_DVBS2_MODUL_SIZE+kCFGFi_DVBS2_FECRATE_SIZE+kCFGFi_DVBS2_OUTSEL_SIZE)

/* - Downloader custom definition (size in bytes) */
#define kCFGFi_DLDC_TIMEOUT_SIZE               4
#define kCFGFi_DLDC_MAXLOOP_SIZE               1
#define kCFGFi_DLDC_SIZE (kCFGFi_DLDC_TIMEOUT_SIZE+kCFGFi_DLDC_MAXLOOP_SIZE)

/* - Flash ICS definition (size in bytes) */
#define kCFGFi_FLASH_ICS_ID_SIZE               1
#define kCFGFi_FLASH_ICS_CLASS_SIZE            1
#define kCFGFi_FLASH_ICS_ADDR_SIZE             4
#define kCFGFi_FLASH_ICS_SIZE_SIZE             4
#define kCFGFi_FLASH_ICS_SIZE (kCFGFi_FLASH_ICS_ID_SIZE+kCFGFi_FLASH_ICS_CLASS_SIZE+kCFGFi_FLASH_ICS_ADDR_SIZE+kCFGFi_FLASH_ICS_SIZE_SIZE)

/* - Flash mapping definition (size in bytes) */
#define kCFGFi_FLASH_BASE_ADDR_SIZE            4
#define kCFGFi_FLASH_ICS_COUNT_SIZE            1
#define kCFGFi_FLASH_SIZE_FIXE (kCFGFi_FLASH_BASE_ADDR_SIZE+kCFGFi_FLASH_ICS_COUNT_SIZE)

/* - OSD Resources Palette Color elements definition (size in bytes) */
#define kCFGFi_OSD_PALCOL_ALFA_SIZE            1
#define kCFGFi_OSD_PALCOL_RED_SIZE             1
#define kCFGFi_OSD_PALCOL_GREEN_SIZE           1
#define kCFGFi_OSD_PALCOL_BLUE_SIZE            1
#define kCFGFi_OSD_PALCOL_SIZE (kCFGFi_OSD_PALCOL_ALFA_SIZE+kCFGFi_OSD_PALCOL_RED_SIZE+kCFGFi_OSD_PALCOL_GREEN_SIZE+kCFGFi_OSD_PALCOL_BLUE_SIZE)

/* - OSD Resources Palette Color Table definition (size in bytes) */
#define kCFGFi_OSD_PALCOL_TABLE_NUMELT       256
#define kCFGFi_OSD_PALCOL_TABLE_SIZE (kCFGFi_OSD_PALCOL_SIZE*kCFGFi_OSD_PALCOL_TABLE_NUMELT)

/* - OSD Resources Palette Entry definition (size in bytes) */
#define kCFGFi_OSD_PALENT_ENTRY_SIZE           1
#define kCFGFi_OSD_PALENT_SIZE (kCFGFi_OSD_PALENT_ENTRY_SIZE)

/* - OSD Resources Palette Entry Table definition (size in bytes) */
#define kCFGFi_OSD_PALENT_TABLE_NUMELT         8
#define kCFGFi_OSD_PALENT_TABLE_SIZE (kCFGFi_OSD_PALENT_SIZE*kCFGFi_OSD_PALENT_TABLE_NUMELT)

/* - OSD Resource Picture definition (size in bytes) */
#define kCFGFi_OSD_PICT_WIDTH_SIZE             2
#define kCFGFi_OSD_PICT_HEIGHT_SIZE            2
#define kCFGFi_OSD_PICT_TYPE_SIZE              1
#define kCFGFi_OSD_PICT_BMPSIZE_SIZE           2
#define kCFGFi_OSD_PICT_SIZE_FIXE (kCFGFi_OSD_PICT_WIDTH_SIZE+kCFGFi_OSD_PICT_HEIGHT_SIZE+kCFGFi_OSD_PICT_TYPE_SIZE+kCFGFi_OSD_PICT_BMPSIZE_SIZE)

/* - OSD Resource definition (size in bytes) */
#define kCFGFi_OSD_PICT_COUNT_SIZE             1
#define kCFGFi_OSD_SIZE_FIXE (kCFGFi_OSD_PALCOL_TABLE_SIZE+kCFGFi_OSD_PALENT_TABLE_SIZE+kCFGFi_OSD_PICT_COUNT_SIZE)

/* - Downloader options definition (size in bytes) */
#define kCFGFi_DLDO_VERSION_SIZE               2
#define kCFGFi_DLDO_BITFIELD_SIZE              4
#define kCFGFi_DLDO_SIZE (kCFGFi_DLDO_VERSION_SIZE+kCFGFi_DLDO_BITFIELD_SIZE)

#define kCFGFi_DLDO_VERSION_01                 0x0001
#define kCFGFi_DLDO_VERSION_CURR               kCFGFi_DLDO_VERSION_01
#define kCFGFi_DLDO_ENABLE_SCANNING_BIT        0x000000001
#define kCFGFi_DLDO_USE_TUNING_DLG_AREA_BIT    0x000000002


/* - Scanning definition (size in bytes) */
#define kCFGFi_SCANNING_VERSION_SIZE           1
#define kCFGFi_SCANNING_NET_TYPE_SIZE          1
#define kCFGFi_SCANNING_HEADER_SIZE            (kCFGFi_SCANNING_VERSION_SIZE+kCFGFi_SCANNING_NET_TYPE_SIZE)

#define kCFGFi_SCANNING_VERSION_01             0x01
#define kCFGFi_SCANNING_VERSION_CURR           kCFGFi_SCANNING_VERSION_01

/* - Terrestrial scanning definition (size in bytes) */
#define kCFGFi_SCANNING_DVBT_VERSION_SIZE      1
#define kCFGFi_SCANNING_DVBT_NB_FREQ_SIZE      1
#define kCFGFi_SCANNING_DVBT_FREQ_LOC_SIZE     12
#define kCFGFi_SCANNING_DVBT_NB_BAND_SIZE      1
#define kCFGFi_SCANNING_DVBT_BAND_LOC_SIZE     16

#define kCFGFi_SCANNING_DVBT_VERSION_01        0x01
#define kCFGFi_SCANNING_DVBT_VERSION_CURR      kCFGFi_SCANNING_DVBT_VERSION_01


/**
 *  Config file in RAM 
*/
#define kCFGFi_CONFIG_FILE_IN_RAM 1


#ifdef kCFGFi_CONFIG_FILE_IN_RAM
#define kCFGFi_CONFIG_FILE_RAM_SIZE  0x00020000

//#ifdef COMEDIA_BIN 
/* Comedia is generated to run in Flash with bootloader support -> Gets the partitionA from RAM -> Will be placed by linker at a fixed address and filled through downloader */
//static const unsigned char CFGFi_CONFIG_FILE_RAMFile[kCFGFi_CONFIG_FILE_RAM_SIZE]__attribute__((section(".FS_CFGi_RAMEMSConfig")));
//#else
/* Comedia is generated to run without bootloader -> Gets the config file from RAM C file -> 
    If comedia is compiled for DCU run -> Will be placed by linker at a random address
    If comedia is compiled for Flash run -> Will be placed by linker at a ramdom address "also"
        */
#include "fs_cfg_myRAMEMSConfig.h"
//#endif
#endif

/******************************************************************************
* Macros
******************************************************************************/

/******************************************************************************
* Typedefs
******************************************************************************/

/******************************************************************************
* Locales Variables
******************************************************************************/

/**
 *  @brief
 *    Initialization indicator: TRUE when trace initialized.
*/
static bool vCFGFi_isTraceInit = FALSE;


/******************************************************************************
* Locales Functions Prototypes
******************************************************************************/

LOCAL tCFGF_Configuration *CFGFi_GetConfig       (uint32_t u32xEltsSet,
                                                  uint8_t *pu8xConfig,
                                                  int32_t i32xSize);
LOCAL bool                 CFGFi_GetProductIDs   (tCFGF_ProductIDs *pstxProdIDs,
                                                  uint8_t *pu8xContent,
                                                  int32_t i32xSize,
                                                  int32_t i32xOffset,
                                                  uint32_t ui32FileVersion);
LOCAL bool                 CFGFi_GetProductIDs_V001 (tCFGF_ProductIDs *pstxProdIDs,
                                                  uint8_t *pu8xContent,
                                                  int32_t i32xSize,
                                                  int32_t i32xOffset);
LOCAL bool                 CFGFi_GetRFMod        (tCFGF_RFMod *pstxRFMod,
                                                  uint8_t *pu8xContent,
                                                  int32_t i32xSize,
                                                  int32_t i32xOffset);
LOCAL bool                 CFGFi_GetLocations    (tCFGF_Configuration *pstxCfg,
                                                  uint32_t u32xEltsSet,
                                                  uint8_t *pu8xContent,
                                                  int32_t i32xSize,
                                                  int32_t i32xOffset);
LOCAL int32_t              CFGFi_GetLocSerial    (tCFGF_LocSerial *pstxSerial,
                                                  uint8_t *pu8xContent,
                                                  int32_t i32xSize,
                                                  int32_t i32xOffset);
LOCAL int32_t              CFGFi_GetLocUSB       (tCFGF_LocUSB *pstxUSB,
                                                  uint8_t *pu8xContent,
                                                  int32_t  i32xSize,
                                                  int32_t  i32xOffset);
LOCAL int32_t              CFGFi_GetLocURL       (tCFGF_LocURL *pstxURL,
                                                  uint8_t *pu8xContent,
                                                  int32_t  i32xSize,
                                                  int32_t  i32xOffset);
#if defined (MEDIUM_TER)
LOCAL int32_t              CFGFi_GetLocDVBT      (tCFGF_LocDVBT *pstxDVBT,
                                                  uint8_t *pu8xContent,
                                                  int32_t i32xSize,
                                                  int32_t i32xOffset);
LOCAL int32_t              CFGFi_GetLocDVBT2     (tCFGF_LocDVBT2 *pstxDVBT2,
                                                  uint8_t *pu8xContent,
                                                  int32_t i32xSize,
                                                  int32_t i32xOffset);
#endif
#if defined (MEDIUM_CAB)
LOCAL int32_t              CFGFi_GetLocDVBC      (tCFGF_LocDVBC *pstxDVBC,
                                                  uint8_t *pu8xContent,
                                                  int32_t i32xSize,
                                                  int32_t i32xOffset);
#endif
#if defined (MEDIUM_SAT)
LOCAL int32_t              CFGFi_GetLocDVBS      (tCFGF_LocDVBS *pstxDVBS,
                                                  uint8_t *pu8xContent,
                                                  int32_t i32xSize,
                                                  int32_t i32xOffset);
LOCAL int32_t              CFGFi_GetLocDVBS2     (tCFGF_LocDVBS2 *pstxDVBS2,
                                                  uint8_t *pu8xContent,
                                                  int32_t i32xSize,
                                                  int32_t i32xOffset);
#endif
LOCAL bool                 CFGFi_GetDownldCustom (tCFGF_DownldCustom *pstxDldCustom,
                                                  uint8_t *pu8xContent,
                                                  int32_t i32xSize,
                                                  int32_t i32xOffset);
LOCAL bool                 CFGFi_GetFlashMapping (tCFGF_FlashMapping *pstxFlash,
                                                  uint8_t *pu8xContent,
                                                  int32_t i32xSize,
                                                  int32_t i32xOffset);
LOCAL bool                 CFGFi_GetOSD          (tCFGF_OSD *pstxOSD,
                                                  uint8_t *pu8xContent,
                                                  int32_t i32xSize,
                                                  int32_t i32xOffset);
LOCAL bool                 CFGFi_GetDownldOptions(tCFGF_DownldOption *pstxDldOptions,
                                                  uint8_t *pu8xContent,
                                                  int32_t i32xSize,
                                                  int32_t i32xOffset);
LOCAL bool                 CFGFi_GetScanning     (tCFGF_Configuration *pstxCfg,
                                                  uint8_t *pu8xContent,
                                                  int32_t i32xSize,
                                                  int32_t i32xOffset);
LOCAL bool                 CFGFi_GetScanningDVBT (tCFGF_Configuration *pstxCfg,
                                                  uint8_t *pu8xContent,
                                                  int32_t i32xSize,
                                                  int32_t i32xOffset);

LOCAL tCFGF_Configuration *CFGFi_NewConfig       (void);
LOCAL void                 CFGFi_FreeConfig      (tCFGF_Configuration *pstxCfg);
LOCAL void                 CFGFi_FreeTableUSB    (tCFGF_Configuration *pstxCfg);
LOCAL void                 CFGFi_FreeTableURL    (tCFGF_Configuration *pstxCfg);
LOCAL void                 CFGFi_FreeOSD         (tCFGF_OSD *pstxOSD);
#ifndef kCFGFi_CONFIG_FILE_IN_RAM
LOCAL uint32_t             CFGFi_MBFLA_Read      ( uint32_t Address, uint8_t* Buffer, uint32_t NumberToRead);
#endif


/******************************************************************************
 * Public Function
 ******************************************************************************/

 /**
 *  @brief
 *    Load the configuration file in memory and return in the allocated 
 *    pointer returned the set of element asked by the caller.
 *
 *  @param[in] u32xEltsSet
 *              Set of identifier kCFGF_ID_PROD_xxx) of elements asked.
 *              A 'or' on identifiers of each desired elements, for
 *              example: kCFGF_ID_PROD_ID|kCFGF_ID_FLASH|kCFGF_ID_LOC_DVBC
 *
 *  @retval  Not NULL
 *              Pointer on configuration loaded.
 *
 *  @retval  NULL
 *              Loading failed (read, allocation or parsing error)
 *
 *  @remarks
 *    -# The pointer returned must be released by next function 
 *       configuration 'CFGF_Free'.
 *
*/
#ifdef kCFGFi_CONFIG_FILE_IN_RAM
/**
 *  Read config file in RAM
*/
tCFGF_Configuration *CFGF_Read
(
   uint32_t u32xEltsSet
)
{
   tCFGF_Configuration *p_cfg  = NULL;

   mTBOX_FCT_ENTER(CFGF_Read);

   if (!vCFGFi_isTraceInit)
   {
      /* Mode trace initialization */
      mTBOX_INIT_MOD_TRACE((eCONFIG_FILE,kTBOX_TRC_CRITICAL+kTBOX_TRC_WARNING
                                         /*+kTBOX_TRC_FCT */
                                         +kTBOX_TRC_1 /* API general traces   */
                                         +kTBOX_TRC_2 /* Data loaded          */
                                         /*+kTBOX_TRC_3  Get data functions   */
                                         /*+kTBOX_TRC_4  Palette colors       */
                                         /*+kTBOX_TRC_5  Not used             */));
      vCFGFi_isTraceInit = TRUE;
   }

   mTBOX_TRACE((kTBOX_NIV_1,
                "[%s] Loading configuration file in RAM (address %#x - size %#x)...\n",
                TBOX_FunctionName,(unsigned int)CFGFi_CONFIG_FILE_RAMFile,
                kCFGFi_CONFIG_FILE_RAM_SIZE));

   /* Create and get configuration */
   p_cfg = CFGFi_GetConfig(u32xEltsSet,
                           CFGFi_CONFIG_FILE_RAMFile+kCFGFi_OFFSET,
                           (int32_t)(kCFGFi_SIZEMAX-kCFGFi_OFFSET));

   if (p_cfg != NULL)
   {
      /* Okay */
      mTBOX_TRACE((kTBOX_NIV_1,
                   "[%s] Configuration file version %#x "
                   "LOADED from RAM in pointer %#x\n",
                   TBOX_FunctionName,p_cfg->ui32Version,
                   (unsigned int)p_cfg));
   }

   mTBOX_RETURN(p_cfg);
}
#else
/**
 *  Read config file in flash
*/
tCFGF_Configuration *CFGF_Read
(
   uint32_t u32xEltsSet
)
{
   tCFGF_Configuration *p_cfg  = NULL;
   tFLASH_CFG_Location  flash_loc;
   TKEL_err             tkerr;
   uint32_t             size,nbr;
   uint8_t             *p_data = NULL;

   mTBOX_FCT_ENTER(CFGF_Read);

   if (!vCFGFi_isTraceInit)
   {
      /* Mode trace initialization */
      mTBOX_INIT_MOD_TRACE((eCONFIG_FILE,kTBOX_TRC_CRITICAL+kTBOX_TRC_WARNING
                                         /*+kTBOX_TRC_FCT */
                                         +kTBOX_TRC_1 /* API general traces   */
                                         +kTBOX_TRC_2 /* Data loaded          */
                                         /*+kTBOX_TRC_3  Get data functions   */
                                         /*+kTBOX_TRC_4  Palette colors       */
                                         /*+kTBOX_TRC_5  Not used             */));
      vCFGFi_isTraceInit = TRUE;
   }

   mTBOX_TRACE((kTBOX_NIV_1,
                "[%s] Loading configuration file...\n",
                TBOX_FunctionName));
   flash_loc.addressOffset = kCFGFi_ADDRESS;
   flash_loc.size = kCFGFi_SIZEMAX;
   mTBOX_TRACE((kTBOX_NIV_1,
                "[%s] Flash Location(configFile): addr %#x - size %#x (max=%#x)\n",
                TBOX_FunctionName,flash_loc.addressOffset,
                flash_loc.size,kCFGFi_SIZEMAX));

   /* Allocate the read buffer */
   if (flash_loc.size < kCFGFi_SIZEMAX) size = (uint32_t)flash_loc.size;
   else size = kCFGFi_SIZEMAX;

   tkerr = TKEL_Malloc((TKEL_msize)size,(void**)&p_data);
   if (p_data == NULL)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,
                   "%s ERROR: Allocation %u bytes to load config file from "
                   "flash {%#x,%#x} failed (tkel error %d)\n",
                   TBOX_FunctionName,size,flash_loc.addressOffset,
                   flash_loc.size,tkerr));
      mTBOX_RETURN(NULL);
   }

   /* Read configuration file */
   nbr = CFGFi_MBFLA_Read(flash_loc.addressOffset,p_data,size);

   if (nbr < size)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,
                   "%s ERROR: Read configuration file at [%#x,%#x] "
                   "failed (read %u/%u bytes)\n",
                   TBOX_FunctionName,flash_loc.addressOffset,
                   flash_loc.size,nbr,size));
   }
   else
   {
      mTBOX_TRACE((kTBOX_NIV_1,
                   "[%s] Read %u/%u bytes in flash\n",
                   TBOX_FunctionName,nbr,size));
 
      /* Config file start by NASC header and Bootloader header */
      if (kCFGFi_OFFSET > size)
      {
         mTBOX_TRACE((kTBOX_NIV_CRITICAL,
                      "%s ERROR: Configuration file undefined "
                      "(start at %d and config size=%u)\n",
                      TBOX_FunctionName,kCFGFi_OFFSET,size));
      }
      else
      {
         /* Ok create and get configuration */
         p_cfg = CFGFi_GetConfig(u32xEltsSet,
                                 p_data+kCFGFi_OFFSET,
                                 (int32_t)(size-kCFGFi_OFFSET));
      }
   }

   if (p_cfg != NULL)
   {
      /* Okay */
      mTBOX_TRACE((kTBOX_NIV_1,
                   "[%s] Configuration file version %#x "
                   "LOADED in pointer %#x\n",
                   TBOX_FunctionName,p_cfg->ui32Version,
                   (unsigned int)p_cfg));
   }

   TKEL_Free(p_data);

   mTBOX_RETURN(p_cfg);
}
#endif

/**
 *  @brief
 *    Free a configuration structure allocated and returned by CFGF_Read.
 *
 *  @param[in] pstxConfiguration
 *              Pointer on configuration to free
 *
 *
*/
void CFGF_Free
(
   tCFGF_Configuration *pstxConfiguration
)
{
   mTBOX_FCT_ENTER(CFGF_Free);

   if (pstxConfiguration == NULL)
   {
      mTBOX_TRACE((kTBOX_NIV_WARNING,
                   "%s WARNING: Configuration undefined\n",
                   TBOX_FunctionName));
      mTBOX_RETURN;
   }

   CFGFi_FreeConfig(pstxConfiguration);

   mTBOX_TRACE((kTBOX_NIV_1,
                "[%s] Configuration data on pointer %#x RELEASED\n",
                TBOX_FunctionName,(unsigned int)pstxConfiguration));

   mTBOX_RETURN;
}

/******************************************************************************
* Locales Functions Implementations
******************************************************************************/

/**
 *  @brief
 *    Extract configuration data (all section necessary) from the 
 *    configuration file content loaded and store this data the 
 *    configuration structure allocated and returned to caller.
 *
 *    Syntax                             No. of bits
 *    configuration_file {
 *       version                                  32
 *       product_offset                           16
 *       location_offset                          16
 *       downloader_custom_offset                 16
 *       osd_resources_offset                     16
 *       rfmod_resources_offset                   16
 *       video_output_offset                      16
 *       downloader_options_offset                16
 *       scanning_offset                          16
 *       reserved                                192 
 *       product_dvb_ssu_id ()
 *       rf_modulator_resources ()
 *       carousel_default_location()
 *       downloader_custom()
 *       flash_description()
 *       osd_resources()
 *       downloader_options()
 *       scanning()
 *    }
 *
 *  @param[in] u32xEltsSet
 *              Set of identifier kCFGF_ID_PROD_xxx) of elements asked.
 *
 *  @param[in] pu8xConfig
 *              Pointer on the config file content
 *
 *  @param[in] i32xSize
 *              Size of config file in bytes
 *
 *  @retval  Not NULL
 *              Pointer on configuration loaded.
 *
 *  @retval  NULL
 *              Loading failed (read, allocation or parsing error)
 *
 *  @remarks
 *    -# Version supported : 1.
 *
 *  @remarks
 *    -# The pointer returned must be released by next function 
 *       configuration 'CFGF_Free'.
 *
 *  @remarks
 *    -# There is config file without rf mod section => Verify 
 *       offset (>0) before parsing.
 *
*/
LOCAL tCFGF_Configuration *CFGFi_GetConfig
(
   uint32_t u32xEltsSet,
   uint8_t *pu8xConfig,
   int32_t  i32xSize
)
{
   bool                 ok     = TRUE;
   tCFGF_Configuration *p_cfg  = NULL;
   int32_t              offset = 0;
   uint32_t             version;
   uint16_t             off_prd,off_loc,off_dld,off_flh,off_osd,off_rfm;  
   uint16_t             off_vid,off_opt,off_scn;
   
   mTBOX_FCT_ENTER(CFGFi_GetConfig);

   /* Header present ? */
   if (kCFGFi_HEAD_SIZE > i32xSize)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,
                   "%s ERROR: Header section on %d bytes not found "
                   "in the %d bytes of config file\n",
                   TBOX_FunctionName,kCFGFi_HEAD_SIZE,i32xSize));
      mTBOX_RETURN(NULL);
   }

   /* Load header */
   mMSBF_GET32_O(pu8xConfig,offset,version);
   mMSBF_GET16_O(pu8xConfig,offset,off_prd);
   mMSBF_GET16_O(pu8xConfig,offset,off_loc);
   mMSBF_GET16_O(pu8xConfig,offset,off_dld);
   mMSBF_GET16_O(pu8xConfig,offset,off_osd);
   mMSBF_GET16_O(pu8xConfig,offset,off_rfm);

   mMSBF_GET16_O(pu8xConfig,offset,off_vid);
   mMSBF_GET16_O(pu8xConfig,offset,off_opt);
   mMSBF_GET16_O(pu8xConfig,offset,off_scn);
   offset += kCFGFi_HEAD_RESERVED_SIZE;
   off_flh = off_dld + kCFGFi_DLDC_SIZE;

   mTBOX_TRACE((kTBOX_NIV_2,"* HEADER (%d bytes):\n",kCFGFi_HEAD_SIZE));
   mTBOX_TRACE((kTBOX_NIV_2,"- Version...............%#x\n",version));
   mTBOX_TRACE((kTBOX_NIV_2,"- Product Offset........%u\n",off_prd));
   mTBOX_TRACE((kTBOX_NIV_2,"- Location Offset.......%u\n",off_loc));
   mTBOX_TRACE((kTBOX_NIV_2,"- Downloader Offset.....%u\n",off_dld));
   mTBOX_TRACE((kTBOX_NIV_2,"- OSD Offset............%u\n",off_osd));
   mTBOX_TRACE((kTBOX_NIV_2,"- RF Mod Offset.........%u\n",off_rfm));

   mTBOX_TRACE((kTBOX_NIV_2,"- Video output Offset...%u\n",off_vid));
   mTBOX_TRACE((kTBOX_NIV_2,"- Dld options Offset....%u\n",off_opt));
   mTBOX_TRACE((kTBOX_NIV_2,"- Scanning Offset.......%u\n",off_scn));

   /* Manage supported versions */
   if ((version != kCFGFi_HEAD_VERSION_001) &&
       (version != kCFGFi_HEAD_VERSION_100))
   {
      /**
       * LIPPA fix bug #3666: Factory downloader not robust against config file evolution
       * 
       * => If config file version > 1.00, load the section known in version 1 and not 
       *    to process new information not implemented in v1.
       */
      if (version < kCFGFi_HEAD_VERSION_100)
      {
         mTBOX_TRACE((kTBOX_NIV_WARNING,
                      "%s ERROR: Version %#x unsupported\n",
                      TBOX_FunctionName,version));
         mTBOX_RETURN(NULL);
      }
      else   if (version > kCFGFi_HEAD_VERSION_CURR)
      {
         /* Continu in version 2.00 */
         mTBOX_TRACE((kTBOX_NIV_WARNING,
                      "%s WARNING: Version %#x not supported but load parameter version 1.00\n",
                      TBOX_FunctionName,version));
         version = kCFGFi_HEAD_VERSION_CURR;
      }
   }

   /* Ok create the configuration structure */
   p_cfg = CFGFi_NewConfig();

   if (p_cfg == NULL)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,
                   "%s ERROR: Create configuration version %#x failed\n",
                   TBOX_FunctionName,version));
      mTBOX_RETURN(NULL);
   }

   p_cfg->ui32Version = version;

   /* Get product IDs if necessary */
   if (u32xEltsSet & kCFGF_ID_PROD_ID)
   {
      switch (version)
      {
      case kCFGFi_HEAD_VERSION_001:
         /* v 0.01 not have the BrandName */
         ok = CFGFi_GetProductIDs_V001(&p_cfg->stProductIDs,
                                       pu8xConfig,i32xSize,
                                       (int32_t)off_prd);
         break;

      case kCFGFi_HEAD_VERSION_100:
      case kCFGFi_HEAD_VERSION_101:
      case kCFGFi_HEAD_VERSION_200:
      default:
         /* Since the 1.00 */
         ok = CFGFi_GetProductIDs(&p_cfg->stProductIDs,
                                  pu8xConfig,i32xSize,
                                  (int32_t)off_prd, version);
      }
   }
   /* Get RF mod if necessary */
   if ((ok) && (u32xEltsSet & kCFGF_ID_RFMOD_RES)  && (off_rfm > 0))
   {
      ok = CFGFi_GetRFMod(&p_cfg->stRFMod,
                          pu8xConfig,i32xSize,
                          (int32_t)off_rfm);
   }

   /* Get set of locations if necessary */
   if ((ok) && (u32xEltsSet & kCFGF_ID_LOC_ALL))
   {
      ok = CFGFi_GetLocations(p_cfg,u32xEltsSet,
                              pu8xConfig,i32xSize,
                              (int32_t)off_loc);
   }

   /* Get Downloader Custom if necessary */
   if ((ok) && (u32xEltsSet & kCFGF_ID_DLD_CUST))
   {
      ok = CFGFi_GetDownldCustom(&p_cfg->stDownldCustom,
                                 pu8xConfig,i32xSize,
                                 (int32_t)off_dld);
   }

   /* Get Flash mapping if necessary */
   if ((ok) && (u32xEltsSet & kCFGF_ID_FLASH))
   {
      ok = CFGFi_GetFlashMapping(&p_cfg->stFlashMapping,
                                 pu8xConfig,i32xSize,
                                 (int32_t)off_flh);
   }

   /* Get OSD Resources if necessary */
   if ((ok) && (u32xEltsSet & kCFGF_ID_OSD_RES))
   {
      ok = CFGFi_GetOSD(&p_cfg->stOSD,
                        pu8xConfig,i32xSize,
                        (int32_t)off_osd);
   }

   /* Get Downloader options if necessary */
   if ((ok) && (u32xEltsSet & kCFGF_ID_DLD_OPTIONS))
   {
      switch (version)
      {
      case kCFGFi_HEAD_VERSION_001:
      case kCFGFi_HEAD_VERSION_100:
      case kCFGFi_HEAD_VERSION_101:
         /* force use of tuning parameters from dialog area and scanning */
         p_cfg->stDownldOptions.ui8UseTuningFromDialogArea = TRUE;
         p_cfg->stDownldOptions.ui8EnableScanning = TRUE;
         break;

      case kCFGFi_HEAD_VERSION_200:
      default:
         ok = CFGFi_GetDownldOptions(&p_cfg->stDownldOptions,
                           pu8xConfig,i32xSize,
                           (int32_t)off_opt);
         break;
      }
   }

   /* Get Scanning parameters if necessary */
   if ((ok) && (u32xEltsSet & kCFGF_ID_SCANNING))
   {
      switch (version)
      {
      case kCFGFi_HEAD_VERSION_001:
      case kCFGFi_HEAD_VERSION_100:
      case kCFGFi_HEAD_VERSION_101:
         /* all fields of tCFGF_Configuration structure have been set to NULL in CFGFi_NewConfig */
         break;

      case kCFGFi_HEAD_VERSION_200:
      default:
         ok = CFGFi_GetScanning(p_cfg,
                           pu8xConfig,i32xSize,
                           (int32_t)off_scn);
         break;
      }
   }

   if (!ok)
   {
      /* Release configuration created */
      CFGFi_FreeConfig(p_cfg);
      mTBOX_RETURN(NULL);
   }

   mTBOX_RETURN(p_cfg);
}

/**
 *  @brief
 *    Extract the Produtc IDs section data.
 *
 *    Syntax                             No. of bits
 *    product_dvb_ssu_id {
 *       oui                                      24
 *       platform_model                           16
 *       platform_version                         16
 *       product_model                            16
 *       product_version                          16
 *       brandname_len                             8
 *       for (i = 0; i < brandname_len; j++) {
 *          brandname_character                    8
 *       }
 *       zone ID                                            8
 *    }
 *
 *  @param[in/out] pstxProdIDs
 *              Product identifiers extracted
 *
 *  @param[in] pu8xContent
 *              Pointer on begining of config file
 *
 *  @param[in] i32xSize
 *              Size of config file in bytes
 *
 *  @param[in] i32xOffset
 *              Offset of begining Product IDs section
 *              in content data.
 *
 *  @param[in] ui32FileVersion
 *              Version of the file for compatibility management
 *
 *  @retval  TRUE
 *              Successful
 *
 *  @retval  FALSE
 *              Extraction failed
 *
 *  @remarks
 *    -# INVARIANT: The pointers on section pstxProdIDs and
 *                  file content pu8xContent are not NULL.
 *
*/
LOCAL bool CFGFi_GetProductIDs
(
   tCFGF_ProductIDs *pstxProdIDs,
   uint8_t          *pu8xContent,
   int32_t           i32xSize,
   int32_t           i32xOffset,
   uint32_t          ui32FileVersion
)
{
   int32_t  offset = i32xOffset;
   int32_t  bn_len = 0;
   uint8_t  b;
   TKEL_err tkerr;

   mTBOX_FCT_ENTER(CFGFi_GetProductIDs);

   if (pstxProdIDs->pcBrandName != NULL)
   {
      TKEL_Free(pstxProdIDs->pcBrandName);
      pstxProdIDs->pcBrandName = NULL;
   }

   if ((offset + kCFGFi_PROD_FIXE_SIZE) > i32xSize)
   {
      mTBOX_TRACE((kTBOX_NIV_WARNING,
                   "%s ERROR: Product IDs section on %d bytes minimum not "
                   "found at offset %d in the %d bytes of config file\n",
                   TBOX_FunctionName,kCFGFi_PROD_FIXE_SIZE,offset,i32xSize));
      mTBOX_RETURN(FALSE);
   }

   mMSBF_GET24_O(pu8xContent,offset,pstxProdIDs->ui32OUI);
   mMSBF_GET16_O(pu8xContent,offset,pstxProdIDs->ui16PltfModel);
   mMSBF_GET16_O(pu8xContent,offset,pstxProdIDs->ui16PltfVersion);
   mMSBF_GET16_O(pu8xContent,offset,pstxProdIDs->ui16ProdModel);
   mMSBF_GET16_O(pu8xContent,offset,pstxProdIDs->ui16ProdVersion);
   mMSBF_GET08_O(pu8xContent,offset,b);
   bn_len = (int32_t)b;

   if ((offset + bn_len) > i32xSize)
   {
      mTBOX_TRACE((kTBOX_NIV_WARNING,
                   "%s ERROR: Brand name in Product section on %d bytes not "
                   "found at offset %d in the %d bytes of config file\n",
                   TBOX_FunctionName,bn_len,offset,i32xSize));
      mTBOX_RETURN(-1);
   }

   /* Allocate the brandname */
   tkerr = TKEL_Malloc((TKEL_msize)bn_len + 1,
                       (void**)&pstxProdIDs->pcBrandName);
   if (pstxProdIDs->pcBrandName == NULL)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,
                   "%s ERROR: Allocation %d bytes to store the "
                   "Brand name failed (tkel error %d)\n",
                   TBOX_FunctionName,bn_len+1,tkerr));
      mTBOX_RETURN(-1);
   }

   if (bn_len > 0)
   {
      memcpy(pstxProdIDs->pcBrandName,pu8xContent+offset,bn_len);
      offset += bn_len;
   }
   pstxProdIDs->pcBrandName[bn_len] = 0;
   
   if (ui32FileVersion >= kCFGFi_HEAD_VERSION_101)
   {
      mMSBF_GET08_O(pu8xContent,offset, pstxProdIDs->ui8DefaultUsageId);
   } else {
      pstxProdIDs->ui8DefaultUsageId = 0;
      mTBOX_TRACE((kTBOX_NIV_2,"- DEFAULT usage ID. 0..............version =.%d\n", ui32FileVersion));
   }
   

   mTBOX_TRACE((kTBOX_NIV_2,"* PRODUCT IDs (%d bytes):\n",kCFGFi_PROD_FIXE_SIZE+bn_len));
   mTBOX_TRACE((kTBOX_NIV_2,"- OUI...................%#x\n",pstxProdIDs->ui32OUI));
   mTBOX_TRACE((kTBOX_NIV_2,"- PlatformModel.........%#x\n",pstxProdIDs->ui16PltfModel));
   mTBOX_TRACE((kTBOX_NIV_2,"- PlatformVersion.......%#x\n",pstxProdIDs->ui16PltfVersion));
   mTBOX_TRACE((kTBOX_NIV_2,"- ProductModel..........%#x\n",pstxProdIDs->ui16ProdModel));
   mTBOX_TRACE((kTBOX_NIV_2,"- ProductVersion........%#x\n",pstxProdIDs->ui16ProdVersion));
   mTBOX_TRACE((kTBOX_NIV_2,"- BrandName.............'%s'\n",pstxProdIDs->pcBrandName));
   mTBOX_TRACE((kTBOX_NIV_2,"- Default Usage ID.....%d\n",pstxProdIDs->ui8DefaultUsageId));

   mTBOX_RETURN(TRUE);
}

/**
 *  @brief
 *    Extract the Produtc IDs section data version 0.01 to support old
 *    config file generated.
 *
 *    Syntax                             No. of bits
 *    product_dvb_ssu_id_v001 {
 *       oui                                      24
 *       hardware_id                              16
 *       hardware_version                         16
 *       software_id                              16
 *       software_version                         16
 *    }
 *
 *  @param[in/out] pstxProdIDs
 *              Product identifiers extracted
 *
 *  @param[in] pu8xContent
 *              Pointer on begining of config file
 *
 *  @param[in] i32xSize
 *              Size of config file in bytes
 *
 *  @param[in] i32xOffset
 *              Offset of begining Product IDs section
 *              in content data.
 *
 *  @retval  TRUE
 *              Successful
 *
 *  @retval  FALSE
 *              Extraction failed
 *
 *  @remarks
 *    -# INVARIANT: The pointers on section pstxProdIDs and
 *                  file content pu8xContent are not NULL.
 *
*/
LOCAL bool CFGFi_GetProductIDs_V001
(
   tCFGF_ProductIDs *pstxProdIDs,
   uint8_t          *pu8xContent,
   int32_t           i32xSize,
   int32_t           i32xOffset
)
{
   int32_t offset = i32xOffset;

   mTBOX_FCT_ENTER(CFGFi_GetProductIDs_V001);

   if (pstxProdIDs->pcBrandName != NULL)
   {
      TKEL_Free(pstxProdIDs->pcBrandName);
      pstxProdIDs->pcBrandName = NULL;
   }

   if ((offset + kCFGFi_PROD_V001_SIZE) > i32xSize)
   {
      mTBOX_TRACE((kTBOX_NIV_WARNING,
                   "%s ERROR: Product IDs section on %d bytes not found "
                   "at offset %d in the %d bytes of config file\n",
                   TBOX_FunctionName,kCFGFi_PROD_V001_SIZE,offset,i32xSize));
      mTBOX_RETURN(FALSE);
   }

   mMSBF_GET24_O(pu8xContent,offset,pstxProdIDs->ui32OUI);
   mMSBF_GET16_O(pu8xContent,offset,pstxProdIDs->ui16PltfModel);
   mMSBF_GET16_O(pu8xContent,offset,pstxProdIDs->ui16PltfVersion);
   mMSBF_GET16_O(pu8xContent,offset,pstxProdIDs->ui16ProdModel);
   mMSBF_GET16_O(pu8xContent,offset,pstxProdIDs->ui16ProdVersion);

   mTBOX_TRACE((kTBOX_NIV_2,"* PRODUCT IDs v 0.01 (%d bytes):\n",kCFGFi_PROD_V001_SIZE));
   mTBOX_TRACE((kTBOX_NIV_2,"- OUI...................%#x\n",pstxProdIDs->ui32OUI));
   mTBOX_TRACE((kTBOX_NIV_2,"- PlatformModel.........%#x\n",pstxProdIDs->ui16PltfModel));
   mTBOX_TRACE((kTBOX_NIV_2,"- PlatformVersion.......%#x\n",pstxProdIDs->ui16PltfVersion));
   mTBOX_TRACE((kTBOX_NIV_2,"- ProductModel..........%#x\n",pstxProdIDs->ui16ProdModel));
   mTBOX_TRACE((kTBOX_NIV_2,"- ProductVersion........%#x\n",pstxProdIDs->ui16ProdVersion));
   mTBOX_TRACE((kTBOX_NIV_2,"- BrandName.............NULL\n"));

   mTBOX_RETURN(TRUE);
}

/**
 *  @brief
 *    Extract the RF modulator resource section data.
 *
 *    Syntax                             No. of bits
 *    rf_modulator_resources {
 *       RFModOutputID                             8
 *       RFModBand                                 4
 *       RFModStandard                             4
 *       RFModChannel                             16
 *       RFModI2cAddress                           8
 *    }
 *
 *  @param[in/out] pstxRFMod
 *              RF modulator resource extracted
 *
 *  @param[in] pu8xContent
 *              Pointer on begining of config file
 *
 *  @param[in] i32xSize
 *              Size of config file in bytes
 *
 *  @param[in] i32xOffset
 *              Offset of begining RF Mod section
 *              in content data.
 *
 *  @retval  TRUE
 *              Successful
 *
 *  @retval  FALSE
 *              Extraction failed
 *
 *  @remarks
 *    -# INVARIANT: The pointers on section pstxProdIDs and
 *                  file content pu8xContent are not NULL.
 *
*/
LOCAL bool CFGFi_GetRFMod
(
   tCFGF_RFMod *pstxRFMod,
   uint8_t     *pu8xContent,
   int32_t      i32xSize,
   int32_t      i32xOffset
)
{
   int32_t  offset = i32xOffset;
   uint8_t  n8;

   mTBOX_FCT_ENTER(CFGFi_GetRFMod);

   if ((offset + kCFGFi_RFMOD_SIZE) > i32xSize)
   {
      mTBOX_TRACE((kTBOX_NIV_WARNING,
                   "%s ERROR: RF Mod section on %d bytes minimum not "
                   "found at offset %d in the %d bytes of config file\n",
                   TBOX_FunctionName,kCFGFi_RFMOD_SIZE,offset,i32xSize));
      mTBOX_RETURN(FALSE);
   }

   mMSBF_GET08_O(pu8xContent,offset,pstxRFMod->ui8RFModOutputId);
   mMSBF_GET08_O(pu8xContent,offset,n8);
   pstxRFMod->ui8RFModBand     = (n8 & 0xF0) >> 4;
   pstxRFMod->ui8RFModStandard = n8 & 0x0F;
   mMSBF_GET16_O(pu8xContent,offset,pstxRFMod->ui16RFModChannel);
   mMSBF_GET08_O(pu8xContent,offset,pstxRFMod->ui8RFModI2cAddress);

   mTBOX_TRACE((kTBOX_NIV_2,"* RF MOD (%d bytes):\n",kCFGFi_RFMOD_SIZE));
   mTBOX_TRACE((kTBOX_NIV_2,"- Output ID.............%#x\n",pstxRFMod->ui8RFModOutputId));
   switch (pstxRFMod->ui8RFModBand)
   {
   case kCFGF_RFMOD_BAND_UHF:
      mTBOX_TRACE((kTBOX_NIV_2,"- Band..................%#x = UHF\n",pstxRFMod->ui8RFModBand));
      break;
   case kCFGF_RFMOD_BAND_VHF:
      mTBOX_TRACE((kTBOX_NIV_2,"- Band..................%#x = VHF\n",pstxRFMod->ui8RFModBand));
      break;
   default:
      mTBOX_TRACE((kTBOX_NIV_2,"- Band..................%#x = Other (not repertoried)\n",pstxRFMod->ui8RFModBand));
   }
   mTBOX_TRACE((kTBOX_NIV_2,"- Standard..............%#x\n",pstxRFMod->ui8RFModStandard));
   mTBOX_TRACE((kTBOX_NIV_2,"- Channel...............%#x=%u\n",pstxRFMod->ui16RFModChannel,
                                                               pstxRFMod->ui16RFModChannel));
   mTBOX_TRACE((kTBOX_NIV_2,"- I2c Address...........%#x=%u\n",pstxRFMod->ui8RFModI2cAddress,
                                                               pstxRFMod->ui8RFModI2cAddress));

   mTBOX_RETURN(TRUE);
}

/**
 *  @brief
 *    Extract the location table (only the elements 
 *    desired).
 *
 *    Syntax                             No. of bits
 *    carousel_default_location {
 *       location_count                            8
 *       for (i = 0; i < location_count; i++) {
 *          network_type                           8
 *          -- corresponding section
 *       }
 *    }
 *
 *  @param[in/out] pstxConfig
 *              Configuration with location elements 
 *              extracted
 *
 *  @param[in] u32xEltsSet
 *              Set of identifier kCFGF_ID_PROD_xxx) of elements asked.
 *
 *  @param[in] pu8xContent
 *              Pointer on begining of config file
 *
 *  @param[in] i32xSize
 *              Size of config file in bytes
 *
 *  @param[in] i32xOffset
 *              Offset of begining location table in content data.
 *
 *  @retval  TRUE
 *              Successful
 *
 *  @retval  FALSE
 *              Extraction failed
 *
 *  @remarks
 *    -# INVARIANT: The pointers on section pstxConfig and
 *                  file content pu8xContent are not NULL.
 *
*/
LOCAL bool CFGFi_GetLocations
(
   tCFGF_Configuration *pstxConfig,
   uint32_t             u32xEltsSet,
   uint8_t             *pu8xContent,
   int32_t              i32xSize,
   int32_t              i32xOffset
)
{
   int32_t    offset   = i32xOffset;
   int32_t    off_table;
   uint8_t    i;
   uint8_t    count    = 0;
   uint8_t    n_serial = 0;
   uint8_t    n_usb    = 0;
   uint8_t    n_url    = 0;
   uint8_t    n_dvbt   = 0;
   uint8_t    n_dvbt2  = 0;
   uint8_t    n_dvbc   = 0;
   uint8_t    n_dvbs   = 0;
   uint8_t    n_dvbs2  = 0;
   uint8_t    netype;
   TKEL_err   tkerr;
   TKEL_msize size;

   mTBOX_FCT_ENTER(CFGFi_GetLocations);

   /* Start by the element counter */
   if ((offset + kCFGFi_LOCT_COUNT_SIZE) > i32xSize)
   {
      mTBOX_TRACE((kTBOX_NIV_WARNING,
                   "%s ERROR: Location counter on %d byte not found "
                   "at offset %d in the %d bytes of config file\n",
                   TBOX_FunctionName,kCFGFi_LOCT_COUNT_SIZE,offset,i32xSize));
      mTBOX_RETURN(FALSE);
   }

   /* Get locations counts and count all type of location */
   mMSBF_GET08_O(pu8xContent,offset,count);
   mTBOX_TRACE((kTBOX_NIV_2,"* LOCATION TABLE (%d elements):\n",count));
   off_table = offset; /* Save offset of table begining for the loading */

   for (i = 0; i < count; i++)
   {
      /* Type of element */
      if ((offset + kCFGFi_LOCT_TYPE_SIZE) > i32xSize)
      {
         mTBOX_TRACE((kTBOX_NIV_WARNING,
                      "%s ERROR: Network type on %d byte not found "
                      "at offset %d in the %d bytes of config file\n",
                      TBOX_FunctionName,kCFGFi_LOCT_TYPE_SIZE,offset,i32xSize));
         mTBOX_RETURN(FALSE);
      }

      mMSBF_GET08_O(pu8xContent,offset,netype);
      switch (netype)
      {
      case kCFGFi_LOCT_TYPE_SERIAL:
         n_serial++;
         offset = CFGFi_GetLocSerial(NULL,pu8xContent,i32xSize,offset);
         break;
      case kCFGFi_LOCT_TYPE_USB:
         n_usb++;
         offset = CFGFi_GetLocUSB(NULL,pu8xContent,i32xSize,offset);
         break;
      case kCFGFi_LOCT_TYPE_URL:
         n_url++;
         offset = CFGFi_GetLocURL(NULL,pu8xContent,i32xSize,offset);
         break;
      case kCFGFi_LOCT_TYPE_DVBT:
         n_dvbt++;
#if defined (MEDIUM_TER)
         offset = CFGFi_GetLocDVBT(NULL,pu8xContent,i32xSize,offset);
#else
         offset += kCFGFi_DVBT_SIZE;
#endif
         break;
      case kCFGFi_LOCT_TYPE_DVBT2:
         n_dvbt2++;
#if defined (MEDIUM_TER)
         /* Supported only since version 2.00 */
         if (pstxConfig->ui32Version < kCFGFi_HEAD_VERSION_200)
            offset += kCFGFi_DVBT2_V1_SIZE;
         else 
            offset = CFGFi_GetLocDVBT2(NULL,pu8xContent,i32xSize,offset);
#else
         if (pstxConfig->ui32Version < kCFGFi_HEAD_VERSION_200)
            offset += kCFGFi_DVBT2_V1_SIZE;
         else 
            offset += kCFGFi_DVBT2_SIZE;
#endif
         break;
      case kCFGFi_LOCT_TYPE_DVBC:
         n_dvbc++;
#if defined (MEDIUM_CAB)
         offset = CFGFi_GetLocDVBC(NULL,pu8xContent,i32xSize,offset);
#else
         offset += kCFGFi_DVBC_SIZE;
#endif
         break;
      case kCFGFi_LOCT_TYPE_DVBS:
         n_dvbs++;
#if defined (MEDIUM_SAT)
         offset = CFGFi_GetLocDVBS(NULL,pu8xContent,i32xSize,offset);
#else
         offset += kCFGFi_DVBS_SIZE;
#endif
         break;
      case kCFGFi_LOCT_TYPE_DVBS2:
         n_dvbs2++;
#if defined (MEDIUM_SAT)
         offset = CFGFi_GetLocDVBS2(NULL,pu8xContent,i32xSize,offset);
#else
         offset += kCFGFi_DVBS2_SIZE;
#endif
         break;
      default:
         mTBOX_TRACE((kTBOX_NIV_WARNING,
                      "%s ERROR: Network type %#x of location %u/%u "
                      "unsupported (compute state at offset %d)\n",
                      TBOX_FunctionName,netype,i,count,offset));
         mTBOX_RETURN(FALSE);
      }

      if (offset < 1) 
      {
         mTBOX_RETURN(FALSE);
      }
   }

   mTBOX_TRACE((kTBOX_NIV_2,"- Serial count..........%u\n",n_serial));
   mTBOX_TRACE((kTBOX_NIV_2,"- USB count.............%u\n",n_usb));
   mTBOX_TRACE((kTBOX_NIV_2,"- URL count.............%u\n",n_url));
   mTBOX_TRACE((kTBOX_NIV_2,"- DVB-T count...........%u\n",n_dvbt));
   mTBOX_TRACE((kTBOX_NIV_2,"- DVB-T2 count..........%u\n",n_dvbt2));
   mTBOX_TRACE((kTBOX_NIV_2,"- DVB-C count...........%u\n",n_dvbc));
   mTBOX_TRACE((kTBOX_NIV_2,"- DVB-S count...........%u\n",n_dvbs));
   mTBOX_TRACE((kTBOX_NIV_2,"- DVB-S2 count..........%u\n",n_dvbs2));

   /* Allocate locations tables desired */
   if (u32xEltsSet & kCFGF_ID_LOC_SERIAL)
   {
      if (pstxConfig->pstTableSerial != NULL) 
         TKEL_Free(pstxConfig->pstTableSerial);
      pstxConfig->pstTableSerial = NULL;
      pstxConfig->ui8CountSerial = 0;

      if (n_serial > 0)
      {
         size = (TKEL_msize)n_serial * sizeof(tCFGF_LocSerial);
         tkerr = TKEL_Malloc(size,(void**)&pstxConfig->pstTableSerial);
         if (pstxConfig->pstTableSerial == NULL)
         {
            mTBOX_TRACE((kTBOX_NIV_CRITICAL,
                         "%s ERROR: Allocation %d bytes to store %u "
                         "serial location failed (tkel error %d)\n",
                         TBOX_FunctionName,size,n_serial,tkerr));
            mTBOX_RETURN(FALSE);
         }

         memset(pstxConfig->pstTableSerial,0,(size_t)size);
      }
   }
   if (u32xEltsSet & kCFGF_ID_LOC_USB)
   {
      CFGFi_FreeTableUSB(pstxConfig);

      if (n_usb > 0)
      {
         size = (TKEL_msize)n_usb * sizeof(tCFGF_LocUSB);
         tkerr = TKEL_Malloc(size,(void**)&pstxConfig->pstTableUSB);
         if (pstxConfig->pstTableUSB == NULL)
         {
            mTBOX_TRACE((kTBOX_NIV_CRITICAL,
                         "%s ERROR: Allocation %d bytes to store %u "
                         "USB location failed (tkel error %d)\n",
                         TBOX_FunctionName,size,n_usb,tkerr));
            mTBOX_RETURN(FALSE);
         }

         memset(pstxConfig->pstTableUSB,0,(size_t)size);
      }
   }
   if (u32xEltsSet & kCFGF_ID_LOC_URL)
   {
      CFGFi_FreeTableURL(pstxConfig);

      if (n_url > 0)
      {
         size = (TKEL_msize)n_url * sizeof(tCFGF_LocURL);
         tkerr = TKEL_Malloc(size,(void**)&pstxConfig->pstTableURL);
         if (pstxConfig->pstTableURL == NULL)
         {
            mTBOX_TRACE((kTBOX_NIV_CRITICAL,
                         "%s ERROR: Allocation %d bytes to store %u "
                         "URL location failed (tkel error %d)\n",
                         TBOX_FunctionName,size,n_url,tkerr));
            mTBOX_RETURN(FALSE);
         }

         memset(pstxConfig->pstTableURL,0,(size_t)size);
      }
   }
#if defined (MEDIUM_TER)
   if (u32xEltsSet & kCFGF_ID_LOC_DVBT)
   {
      if (pstxConfig->pstTableDVBT != NULL) 
         TKEL_Free(pstxConfig->pstTableDVBT);
      pstxConfig->pstTableDVBT = NULL;
      pstxConfig->ui8CountDVBT = 0;

      if (n_dvbt > 0)
      {
         size = (TKEL_msize)n_dvbt * sizeof(tCFGF_LocDVBT);
         tkerr = TKEL_Malloc(size,(void**)&pstxConfig->pstTableDVBT);
         if (pstxConfig->pstTableDVBT == NULL)
         {
            mTBOX_TRACE((kTBOX_NIV_CRITICAL,
                         "%s ERROR: Allocation %d bytes to store %u "
                         "DVB-T location failed (tkel error %d)\n",
                         TBOX_FunctionName,size,n_dvbt,tkerr));
            mTBOX_RETURN(FALSE);
         }

         memset(pstxConfig->pstTableDVBT,0,(size_t)size);
      }
   }
   if (u32xEltsSet & kCFGF_ID_LOC_DVBT2)
   {
      if (pstxConfig->pstTableDVBT2 != NULL) 
         TKEL_Free(pstxConfig->pstTableDVBT2);
      pstxConfig->pstTableDVBT2 = NULL;
      pstxConfig->ui8CountDVBT2 = 0;

      if (n_dvbt2 > 0)
      {
         size = (TKEL_msize)n_dvbt2 * sizeof(tCFGF_LocDVBT2);
         tkerr = TKEL_Malloc(size,(void**)&pstxConfig->pstTableDVBT2);
         if (pstxConfig->pstTableDVBT2 == NULL)
         {
            mTBOX_TRACE((kTBOX_NIV_CRITICAL,
                         "%s ERROR: Allocation %d bytes to store %u "
                         "DVB-T2 location failed (tkel error %d)\n",
                         TBOX_FunctionName,size,n_dvbt2,tkerr));
            mTBOX_RETURN(FALSE);
         }

         memset(pstxConfig->pstTableDVBT2,0,(size_t)size);
      }
   }
#endif /* MEDIUM_TER */
#if defined (MEDIUM_CAB)
   if (u32xEltsSet & kCFGF_ID_LOC_DVBC)
   {
      if (pstxConfig->pstTableDVBC != NULL) 
         TKEL_Free(pstxConfig->pstTableDVBC);
      pstxConfig->pstTableDVBC = NULL;
      pstxConfig->ui8CountDVBC = 0;

      if (n_dvbc > 0)
      {
         size = (TKEL_msize)n_dvbc * sizeof(tCFGF_LocDVBC);
         tkerr = TKEL_Malloc(size,(void**)&pstxConfig->pstTableDVBC);
         if (pstxConfig->pstTableDVBC == NULL)
         {
            mTBOX_TRACE((kTBOX_NIV_CRITICAL,
                         "%s ERROR: Allocation %d bytes to store %u "
                         "DVB-C location failed (tkel error %d)\n",
                         TBOX_FunctionName,size,n_dvbc,tkerr));
            mTBOX_RETURN(FALSE);
         }

         memset(pstxConfig->pstTableDVBC,0,(size_t)size);
      }
   }
#endif /* MEDIUM_CAB */
#if defined (MEDIUM_SAT)
   if (u32xEltsSet & kCFGF_ID_LOC_DVBS)
   {
      if (pstxConfig->pstTableDVBS != NULL) 
         TKEL_Free(pstxConfig->pstTableDVBS);
      pstxConfig->pstTableDVBS = NULL;
      pstxConfig->ui8CountDVBS = 0;

      if (n_dvbs > 0)
      {
         size = (TKEL_msize)n_dvbs * sizeof(tCFGF_LocDVBS);
         tkerr = TKEL_Malloc(size,(void**)&pstxConfig->pstTableDVBS);
         if (pstxConfig->pstTableDVBS == NULL)
         {
            mTBOX_TRACE((kTBOX_NIV_CRITICAL,
                         "%s ERROR: Allocation %d bytes to store %u "
                         "DVB-S location failed (tkel error %d)\n",
                         TBOX_FunctionName,size,n_dvbs,tkerr));
            mTBOX_RETURN(FALSE);
         }

         memset(pstxConfig->pstTableDVBS,0,(size_t)size);
      }
   }
   if (u32xEltsSet & kCFGF_ID_LOC_DVBS2)
   {
      if (pstxConfig->pstTableDVBS2 != NULL) 
         TKEL_Free(pstxConfig->pstTableDVBS2);
      pstxConfig->pstTableDVBS2 = NULL;
      pstxConfig->ui8CountDVBS2 = 0;

      if (n_dvbs2 > 0)
      {
         size = (TKEL_msize)n_dvbs2 * sizeof(tCFGF_LocDVBS2);
         tkerr = TKEL_Malloc(size,(void**)&pstxConfig->pstTableDVBS2);
         if (pstxConfig->pstTableDVBS2 == NULL)
         {
            mTBOX_TRACE((kTBOX_NIV_CRITICAL,
                         "%s ERROR: Allocation %d bytes to store %u "
                         "DVB-S2 location failed (tkel error %d)\n",
                         TBOX_FunctionName,size,n_dvbs2,tkerr));
            mTBOX_RETURN(FALSE);
         }

         memset(pstxConfig->pstTableDVBS2,0,(size_t)size);
      }
   }
#endif /* MEDIUM_SAT */

   /* Loading desired element in allocated table */
   offset = off_table;

   for (i = 0; i < count; i++)
   {
      /* Type of element */
      if ((offset + kCFGFi_LOCT_TYPE_SIZE) > i32xSize)
      {
         mTBOX_TRACE((kTBOX_NIV_WARNING,
                      "%s ERROR: Network type on %d byte not found "
                      "at offset %d in the %d bytes of config file\n",
                      TBOX_FunctionName,kCFGFi_LOCT_TYPE_SIZE,offset,i32xSize));
         mTBOX_RETURN(FALSE);
      }

      mMSBF_GET08_O(pu8xContent,offset,netype);
      switch (netype)
      {
      case kCFGFi_LOCT_TYPE_SERIAL:
         if (u32xEltsSet & kCFGF_ID_LOC_SERIAL)
         {
            if (pstxConfig->ui8CountSerial < n_serial)
            {
               offset = CFGFi_GetLocSerial(pstxConfig->pstTableSerial+pstxConfig->ui8CountSerial,
                                           pu8xContent,i32xSize,offset);
               pstxConfig->ui8CountSerial++;
            }
            else
            {
               mTBOX_TRACE((kTBOX_NIV_CRITICAL,
                            "%s ERROR: ABNORMAL Serial presence when "
                            "count %u and detect %u at offset %d in "
                            "the %d bytes of config file\n",
                            TBOX_FunctionName,pstxConfig->ui8CountSerial,
                            n_serial,offset,i32xSize));
               mTBOX_RETURN(FALSE);
            }
         }
         else offset = CFGFi_GetLocSerial(NULL,pu8xContent,i32xSize,offset);
         break;
      case kCFGFi_LOCT_TYPE_USB:
         if (u32xEltsSet & kCFGF_ID_LOC_USB)
         {
            if (pstxConfig->ui8CountUSB < n_usb)
            {
               offset = CFGFi_GetLocUSB(pstxConfig->pstTableUSB+pstxConfig->ui8CountUSB,
                                        pu8xContent,i32xSize,offset);
               pstxConfig->ui8CountUSB++;
            }
            else
            {
               mTBOX_TRACE((kTBOX_NIV_CRITICAL,
                            "%s ERROR: ABNORMAL USB presence when "
                            "count %u and detect %u at offset %d in "
                            "the %d bytes of config file\n",
                            TBOX_FunctionName,pstxConfig->ui8CountUSB,
                            n_usb,offset,i32xSize));
               mTBOX_RETURN(FALSE);
            }
         }
         else offset = CFGFi_GetLocUSB(NULL,pu8xContent,i32xSize,offset);
         break;
      case kCFGFi_LOCT_TYPE_URL:
         if (u32xEltsSet & kCFGF_ID_LOC_URL)
         {
            if (pstxConfig->ui8CountURL < n_url)
            {
               offset = CFGFi_GetLocURL(pstxConfig->pstTableURL+pstxConfig->ui8CountURL,
                                        pu8xContent,i32xSize,offset);
               pstxConfig->ui8CountURL++;
            }
            else
            {
               mTBOX_TRACE((kTBOX_NIV_CRITICAL,
                            "%s ERROR: ABNORMAL URL presence when "
                            "count %u and detect %u at offset %d in "
                            "the %d bytes of config file\n",
                            TBOX_FunctionName,pstxConfig->ui8CountURL,
                            n_url,offset,i32xSize));
               mTBOX_RETURN(FALSE);
            }
         }
         else offset = CFGFi_GetLocURL(NULL,pu8xContent,i32xSize,offset);
         break;
      case kCFGFi_LOCT_TYPE_DVBT:
#if defined (MEDIUM_TER)
         if (u32xEltsSet & kCFGF_ID_LOC_DVBT)
         {
            if (pstxConfig->ui8CountDVBT < n_dvbt)
            {
               offset = CFGFi_GetLocDVBT(pstxConfig->pstTableDVBT+pstxConfig->ui8CountDVBT,
                                         pu8xContent,i32xSize,offset);
               pstxConfig->ui8CountDVBT++;
            }
            else
            {
               mTBOX_TRACE((kTBOX_NIV_CRITICAL,
                            "%s ERROR: ABNORMAL DVB-T presence when "
                            "count %u and detect %u at offset %d in "
                            "the %d bytes of config file\n",
                            TBOX_FunctionName,pstxConfig->ui8CountDVBT,
                            n_dvbt,offset,i32xSize));
               mTBOX_RETURN(FALSE);
            }
         }
         else offset = CFGFi_GetLocDVBT(NULL,pu8xContent,i32xSize,offset);
#else
         offset += kCFGFi_DVBT_SIZE;
#endif
         break;
      case kCFGFi_LOCT_TYPE_DVBT2:
#if defined (MEDIUM_TER)
         /* Supported only since version 2.00 */
         if (pstxConfig->ui32Version < kCFGFi_HEAD_VERSION_200)
            offset += kCFGFi_DVBT2_V1_SIZE;
         else if (u32xEltsSet & kCFGF_ID_LOC_DVBT2)
         {
            if (pstxConfig->ui8CountDVBT2 < n_dvbt2)
            {
               offset = CFGFi_GetLocDVBT2(pstxConfig->pstTableDVBT2+pstxConfig->ui8CountDVBT2,
                                          pu8xContent,i32xSize,offset);
               pstxConfig->ui8CountDVBT2++;
            }
            else
            {
               mTBOX_TRACE((kTBOX_NIV_CRITICAL,
                            "%s ERROR: ABNORMAL DVB-T2 presence when "
                            "count %u and detect %u at offset %d in "
                            "the %d bytes of config file\n",
                            TBOX_FunctionName,pstxConfig->ui8CountDVBT2,
                            n_dvbt2,offset,i32xSize));
               mTBOX_RETURN(FALSE);
            }
         }
         else offset = CFGFi_GetLocDVBT2(NULL,pu8xContent,i32xSize,offset);
#else
         if (pstxConfig->ui32Version < kCFGFi_HEAD_VERSION_200)
            offset += kCFGFi_DVBT2_V1_SIZE;
         else 
            offset += kCFGFi_DVBT2_SIZE;
#endif
         break;
      case kCFGFi_LOCT_TYPE_DVBC:
#if defined (MEDIUM_CAB)
         if (u32xEltsSet & kCFGF_ID_LOC_DVBC)
         {
            if (pstxConfig->ui8CountDVBC < n_dvbc)
            {
               offset = CFGFi_GetLocDVBC(pstxConfig->pstTableDVBC+pstxConfig->ui8CountDVBC,
                                        pu8xContent,i32xSize,offset);
               pstxConfig->ui8CountDVBC++;
            }
            else
            {
               mTBOX_TRACE((kTBOX_NIV_CRITICAL,
                            "%s ERROR: ABNORMAL DVB-C presence when "
                            "count %u and detect %u at offset %d in "
                            "the %d bytes of config file\n",
                            TBOX_FunctionName,pstxConfig->ui8CountDVBC,
                            n_dvbc,offset,i32xSize));
               mTBOX_RETURN(FALSE);
            }
         }
         else offset = CFGFi_GetLocDVBC(NULL,pu8xContent,i32xSize,offset);
#else
         offset += kCFGFi_DVBC_SIZE;
#endif
         break;
      case kCFGFi_LOCT_TYPE_DVBS:
#if defined (MEDIUM_SAT)
         if (u32xEltsSet & kCFGF_ID_LOC_DVBS)
         {
            if (pstxConfig->ui8CountDVBS < n_dvbs)
            {
               offset = CFGFi_GetLocDVBS(pstxConfig->pstTableDVBS+pstxConfig->ui8CountDVBS,
                                         pu8xContent,i32xSize,offset);
               pstxConfig->ui8CountDVBS++;
            }
            else
            {
               mTBOX_TRACE((kTBOX_NIV_CRITICAL,
                            "%s ERROR: ABNORMAL DVB-S presence when "
                            "count %u and detect %u at offset %d in "
                            "the %d bytes of config file\n",
                            TBOX_FunctionName,pstxConfig->ui8CountDVBS,
                            n_dvbs,offset,i32xSize));
               mTBOX_RETURN(FALSE);
            }
         }
         else offset = CFGFi_GetLocDVBS(NULL,pu8xContent,i32xSize,offset);
#else
         offset += kCFGFi_DVBS_SIZE;
#endif
         break;
      case kCFGFi_LOCT_TYPE_DVBS2:
#if defined (MEDIUM_SAT)
         if (u32xEltsSet & kCFGF_ID_LOC_DVBS2)
         {
            if (pstxConfig->ui8CountDVBS2 < n_dvbs2)
            {
               offset = CFGFi_GetLocDVBS2(pstxConfig->pstTableDVBS2+pstxConfig->ui8CountDVBS2,
                                          pu8xContent,i32xSize,offset);
               pstxConfig->ui8CountDVBS2++;
            }
            else
            {
               mTBOX_TRACE((kTBOX_NIV_CRITICAL,
                            "%s ERROR: ABNORMAL DVB-S2 presence when "
                            "count %u and detect %u at offset %d in "
                            "the %d bytes of config file\n",
                            TBOX_FunctionName,pstxConfig->ui8CountDVBS2,
                            n_dvbs2,offset,i32xSize));
               mTBOX_RETURN(FALSE);
            }
         }
         else offset = CFGFi_GetLocDVBS2(NULL,pu8xContent,i32xSize,offset);
#else
         offset += kCFGFi_DVBS2_SIZE;
#endif
         break;
      default:
         mTBOX_TRACE((kTBOX_NIV_WARNING,
                      "%s ERROR: Network type %#x of location %u/%u "
                      "unsupported (loading state at offset %d)\n",
                      TBOX_FunctionName,netype,i,count,offset));
         mTBOX_RETURN(FALSE);
      }

      if (offset < 1) 
      {
         mTBOX_RETURN(FALSE);
      }
   }

   return(TRUE);
}

/**
 *  @brief
 *    Extract the Serial Location section data.
 *
 *    Syntax                             No. of bits
 *    location_serial {
 *       serial_port_id                            4
 *       serial_data_bit                           4
 *       serial_parity                             2
 *       serial_stop_bit                           4
 *       serial_flow_control                       2
 *       serial_speed                              8
 *    }
 *
 *  @param[in/out] pstxSerial
 *              Serial location extracted if defined (not NULL)
 *
 *  @param[in] pu8xContent
 *              Pointer on begining of config file
 *
 *  @param[in] i32xSize
 *              Size of config file in bytes
 *
 *  @param[in] i32xOffset
 *              Offset of begining Serial section
 *              in content data.
 *
 *  @retval  > 0
 *              Successful, offset of next section begining 
 *
 *  @retval  < 1
 *              Extraction failed
 *
 *  @remarks
 *    -# INVARIANT: The pointer on file content pu8xContent 
 *                  is not NULL.
 *
 *    -# If the pointer on data pstxSerial is NULL this function
 *       is used to jump on this section.
 *
*/
LOCAL int32_t CFGFi_GetLocSerial
(
   tCFGF_LocSerial *pstxSerial,
   uint8_t         *pu8xContent,
   int32_t          i32xSize,
   int32_t          i32xOffset
)
{
   int32_t offset = i32xOffset;
   uint8_t prop1,prop2;

   mTBOX_FCT_ENTER(CFGFi_GetLocSerial);

   if ((offset + kCFGFi_SERIAL_SIZE) > i32xSize)
   {
      mTBOX_TRACE((kTBOX_NIV_WARNING,
                   "%s ERROR: Serial Location section on %d bytes not found "
                   "at offset %d in the %d bytes of config file\n",
                   TBOX_FunctionName,kCFGFi_SERIAL_SIZE,offset,i32xSize));
      mTBOX_RETURN(-1);
   }

   if (pstxSerial == NULL)
   {
      /* Jump after this section */
      mTBOX_RETURN(offset+kCFGFi_SERIAL_SIZE);
   }

   mMSBF_GET08_O(pu8xContent,offset,prop1);
   mMSBF_GET08_O(pu8xContent,offset,prop2);
   mMSBF_GET08_O(pu8xContent,offset,pstxSerial->ui8Speed);
   pstxSerial->ui8PortId   = (prop1 >> 4) & 0x0F;
   pstxSerial->ui8DataBit  =  prop1       & 0x0F;
   pstxSerial->ui8Parity   = (prop2 >> 6) & 0x03;
   pstxSerial->ui8StopBit  = (prop2 >> 2) & 0x0F;
   pstxSerial->ui8FlowCtrl =  prop2       & 0x03;

   mTBOX_TRACE((kTBOX_NIV_2,"  * LOCATION SERIAL (%d bytes %#x %#x %#x):\n",
                            kCFGFi_SERIAL_SIZE,prop1,prop2,pstxSerial->ui8Speed));
   mTBOX_TRACE((kTBOX_NIV_2,"  - Port ID.............%u\n",pstxSerial->ui8PortId));
   mTBOX_TRACE((kTBOX_NIV_2,"  - Data bit............%u\n",pstxSerial->ui8DataBit));
   switch (pstxSerial->ui8Parity)
   {
   case kCFGF_SERIAL_PARITY_NONE:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Parity..............%#x = None\n",pstxSerial->ui8Parity));
      break;
   case kCFGF_SERIAL_PARITY_ODD:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Parity..............%#x = Odd\n",pstxSerial->ui8Parity));
      break;
   case kCFGF_SERIAL_PARITY_EVEN:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Parity..............%#x = Even\n",pstxSerial->ui8Parity));
      break;
   default:
      mTBOX_TRACE((kTBOX_NIV_WARNING,
                   "%s ERROR: Parity %#x of Serial Location unsupported\n",
                   TBOX_FunctionName,pstxSerial->ui8Parity));
      mTBOX_RETURN(-1);
   }
   mTBOX_TRACE((kTBOX_NIV_2,"  - Stop bit............%u\n",pstxSerial->ui8StopBit));
   switch (pstxSerial->ui8FlowCtrl)
   {
   case kCFGF_SERIAL_FCTRL_NONE:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Flow control........%#x = None\n",pstxSerial->ui8FlowCtrl));
      break;
   case kCFGF_SERIAL_FCTRL_HARDW:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Flow control........%#x = Hardware\n",pstxSerial->ui8FlowCtrl));
      break;
   case kCFGF_SERIAL_FCTRL_XONOFF:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Flow control........%#x = Xon/Xoff\n",pstxSerial->ui8FlowCtrl));
      break;
   default:
      mTBOX_TRACE((kTBOX_NIV_WARNING,
                   "%s ERROR: Flow control %#x of Serial Location unsupported\n",
                   TBOX_FunctionName,pstxSerial->ui8FlowCtrl));
      mTBOX_RETURN(-1);
   }
   switch (pstxSerial->ui8Speed)
   {
   case kCFGF_SERIAL_SPEED_600:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Speed...............%#x = 600\n",pstxSerial->ui8Speed));
      break;
   case kCFGF_SERIAL_SPEED_1200:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Speed...............%#x = 1200\n",pstxSerial->ui8Speed));
      break;
   case kCFGF_SERIAL_SPEED_2400:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Speed...............%#x = 2400\n",pstxSerial->ui8Speed));
      break;
   case kCFGF_SERIAL_SPEED_4800:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Speed...............%#x = 4800\n",pstxSerial->ui8Speed));
      break;
   case kCFGF_SERIAL_SPEED_9600:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Speed...............%#x = 9600\n",pstxSerial->ui8Speed));
      break;
   case kCFGF_SERIAL_SPEED_14400:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Speed...............%#x = 14400\n",pstxSerial->ui8Speed));
      break;
   case kCFGF_SERIAL_SPEED_19200:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Speed...............%#x = 19200\n",pstxSerial->ui8Speed));
      break;
   case kCFGF_SERIAL_SPEED_38400:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Speed...............%#x = 38400\n",pstxSerial->ui8Speed));
      break;
   case kCFGF_SERIAL_SPEED_57600:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Speed...............%#x = 57600\n",pstxSerial->ui8Speed));
      break;
   case kCFGF_SERIAL_SPEED_115200:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Speed...............%#x = 115200\n",pstxSerial->ui8Speed));
      break;
   default:
      mTBOX_TRACE((kTBOX_NIV_WARNING,
                   "%s ERROR: Speed %#x of Serial Location unsupported\n",
                   TBOX_FunctionName,pstxSerial->ui8Speed));
      mTBOX_RETURN(-1);
   }

   mTBOX_RETURN(offset);
}

/**
 *  @brief
 *    Extract the USB Location section data.
 *
 *    Syntax                             No. of bits
 *    location_usb {
 *       usb_path_len                              8
 *       for (j = 0; j < usb_path_len; j++) {
 *          usb_path_character                     8
 *       }
 *    }
 *
 *  @param[in/out] tCFGF_LocUSB
 *              USB location extracted if defined (not NULL)
 *
 *  @param[in] pu8xContent
 *              Pointer on begining of config file
 *
 *  @param[in] i32xSize
 *              Size of config file in bytes
 *
 *  @param[in] i32xOffset
 *              Offset of begining USB section
 *              in content data.
 *
 *  @retval  > 0
 *              Successful, offset of next section begining 
 *
 *  @retval  < 1
 *              Extraction failed
 *
 *  @remarks
 *    -# INVARIANT: The pointer on file content pu8xContent 
 *                  is not NULL.
 *
 *    -# If the pointer on data pstxUSB is NULL this function
 *       is used to jump on this section.
 *
 *    -# The path name is allocated and set with the string terminator '\0'.
 *
*/
LOCAL int32_t CFGFi_GetLocUSB
(
   tCFGF_LocUSB *pstxUSB,
   uint8_t      *pu8xContent,
   int32_t       i32xSize,
   int32_t       i32xOffset
)
{
   int32_t  offset = i32xOffset;
   int32_t  len    = 0;
   uint8_t  b;
   TKEL_err tkerr;

   mTBOX_FCT_ENTER(CFGFi_ExtractLocUSB);


   if ((offset + kCFGFi_USB_SIZE_FIXE) > i32xSize)
   {
      mTBOX_TRACE((kTBOX_NIV_WARNING,
                   "%s ERROR: USB Location section path len on %d bytes not "
                   "found at offset %d in the %d bytes of config file\n",
                   TBOX_FunctionName,kCFGFi_USB_SIZE_FIXE,offset,i32xSize));
      mTBOX_RETURN(-1);
   }

   mMSBF_GET08_O(pu8xContent,offset,b);
   len = (int32_t)b;

   if ((offset + len) > i32xSize)
   {
      mTBOX_TRACE((kTBOX_NIV_WARNING,
                   "%s ERROR: USB Location section path on %d bytes not "
                   "found at offset %d in the %d bytes of config file\n",
                   TBOX_FunctionName,len,offset,i32xSize));
      mTBOX_RETURN(-1);
   }

   if (pstxUSB == NULL)
   {
      /* Jump after this section */
      mTBOX_RETURN(offset+len);
   }

   /* Free previous path name */
   if (pstxUSB->pcPathName != NULL)
   {
      TKEL_Free(pstxUSB->pcPathName);
      pstxUSB->pcPathName = NULL;
   }

   /* Allocate the pathname */
   tkerr = TKEL_Malloc((TKEL_msize)len + 1,
                       (void**)&pstxUSB->pcPathName);
   if (pstxUSB->pcPathName == NULL)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,
                   "%s ERROR: Allocation %d bytes to store the USB "
                   "path name failed (tkel error %d)\n",
                   TBOX_FunctionName,len+1,tkerr));
      mTBOX_RETURN(-1);
   }

   if (len > 0)
   {
      memcpy(pstxUSB->pcPathName,pu8xContent+offset,len);
      offset += len;
   }
   pstxUSB->pcPathName[len] = 0;

   mTBOX_TRACE((kTBOX_NIV_2,"  * LOCATION USB (%d bytes):\n",
                            kCFGFi_USB_SIZE_FIXE+len));
   mTBOX_TRACE((kTBOX_NIV_2,"  - Path name...........'%s'\n",
                            pstxUSB->pcPathName));
   mTBOX_RETURN(offset);
}

/**
 *  @brief
 *    Extract the URL Location section data.
 *
 *    Syntax                             No. of bits
 *    location_url {
 *       url_len                              8
 *       for (j = 0; j < usb_path_len; j++) {
 *          url_character                     8
 *       }
 *    }
 *
 *  @param[in/out] tCFGF_LocURL
 *              URL location extracted if defined (not NULL)
 *
 *  @param[in] pu8xContent
 *              Pointer on begining of config file
 *
 *  @param[in] i32xSize
 *              Size of config file in bytes
 *
 *  @param[in] i32xOffset
 *              Offset of begining URL section
 *              in content data.
 *
 *  @retval  > 0
 *              Successful, offset of next section begining 
 *
 *  @retval  < 1
 *              Extraction failed
 *
 *  @remarks
 *    -# INVARIANT: The pointer on file content pu8xContent 
 *                  is not NULL.
 *
 *    -# If the pointer on data pstxURL is NULL this function
 *       is used to jump on this section.
 *
 *    -# The path name is allocated and set with the string terminator '\0'.
 *
*/
LOCAL int32_t CFGFi_GetLocURL
(
   tCFGF_LocURL *pstxURL,
   uint8_t      *pu8xContent,
   int32_t       i32xSize,
   int32_t       i32xOffset
)
{
   int32_t  offset = i32xOffset;
   int32_t  len    = 0;
   uint8_t  b;
   TKEL_err tkerr;

   mTBOX_FCT_ENTER(CFGFi_ExtractLocURL);

   if ((offset + kCFGFi_URL_SIZE_FIXE) > i32xSize)
   {
      mTBOX_TRACE((kTBOX_NIV_WARNING,
                   "%s ERROR: URL Location section path len on %d bytes not "
                   "found at offset %d in the %d bytes of config file\n",
                   TBOX_FunctionName,kCFGFi_URL_SIZE_FIXE,offset,i32xSize));
      mTBOX_RETURN(-1);
   }

   mMSBF_GET08_O(pu8xContent,offset,b);
   len = (int32_t)b;

   if ((offset + len) > i32xSize)
   {
      mTBOX_TRACE((kTBOX_NIV_WARNING,
                   "%s ERROR: URL Location section path on %d bytes not "
                   "found at offset %d in the %d bytes of config file\n",
                   TBOX_FunctionName,len,offset,i32xSize));
      mTBOX_RETURN(-1);
   }

   if (pstxURL == NULL)
   {
      /* Jump after this section */
      mTBOX_RETURN(offset+len);
   }

   /* Free previous path name */
   if (pstxURL->pcURL != NULL)
   {
      TKEL_Free(pstxURL->pcURL);
      pstxURL->pcURL = NULL;
   }

   /* Allocate the pathname */
   tkerr = TKEL_Malloc((TKEL_msize)len + 1,
                       (void**)&pstxURL->pcURL);
   if (pstxURL->pcURL == NULL)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,
                   "%s ERROR: Allocation %d bytes to store the URL "
                   "path name failed (tkel error %d)\n",
                   TBOX_FunctionName,len+1,tkerr));
      mTBOX_RETURN(-1);
   }

   if (len > 0)
   {
      memcpy(pstxURL->pcURL,pu8xContent+offset,len);
      offset += len;
   }
   pstxURL->pcURL[len] = 0;

   mTBOX_TRACE((kTBOX_NIV_2,"  * LOCATION URL (%d bytes):\n",
                            kCFGFi_URL_SIZE_FIXE+len));
   mTBOX_TRACE((kTBOX_NIV_2,"  - URL.................'%s'\n",
                            pstxURL->pcURL));
   mTBOX_RETURN(offset);
}

#if defined (MEDIUM_TER)
/**
 *  @brief
 *    Extract the DVB-T Location section data.
 *
 *    Syntax                             No. of bits
 *    location_dvb-t {
 *       pmt_pid                                  16
 *       dsmcc_pid                                16
 *       frequency                                32
 *       band_width                               32
 *       modulation                                8
 *       mode                                      8
 *       guard                                     8
 *       hierarchy                                 8
 *       priority                                  8
 *       convolution_hp                            8
 *       convolution_lp                            8
 *       freq_offset                               8
 *    }
 *
 *  @param[in/out] pstxDVBT
 *              DVB-T location extracted if defined (not NULL)
 *
 *  @param[in] pu8xContent
 *              Pointer on begining of config file
 *
 *  @param[in] i32xSize
 *              Size of config file in bytes
 *
 *  @param[in] i32xOffset
 *              Offset of begining Serial section
 *              in content data.
 *
 *  @retval  > 0
 *              Successful, offset of next section begining 
 *
 *  @retval  < 1
 *              Extraction failed
 *
 *  @remarks
 *    -# INVARIANT: The pointer on file content pu8xContent 
 *                  is not NULL.
 *
 *    -# If the pointer on data pstxDVBT is NULL this function
 *       is used to jump on this section.
 *
*/
LOCAL int32_t CFGFi_GetLocDVBT
(
   tCFGF_LocDVBT *pstxDVBT,
   uint8_t       *pu8xContent,
   int32_t        i32xSize,
   int32_t        i32xOffset
)
{
   int32_t offset = i32xOffset;

   mTBOX_FCT_ENTER(CFGFi_GetLocDVBT);

   if ((offset + kCFGFi_DVBT_SIZE) > i32xSize)
   {
      mTBOX_TRACE((kTBOX_NIV_WARNING,
                   "%s ERROR: DVB-T Location section on %d bytes not found "
                   "at offset %d in the %d bytes of config file\n",
                   TBOX_FunctionName,kCFGFi_DVBT_SIZE,offset,i32xSize));
      mTBOX_RETURN(-1);
   }

   if (pstxDVBT == NULL)
   {
      /* Jump after this section */
      mTBOX_RETURN(offset+kCFGFi_DVBT_SIZE);
   }

   mMSBF_GET16_O(pu8xContent,offset,pstxDVBT->ui16PmtPID);
   mMSBF_GET16_O(pu8xContent,offset,pstxDVBT->ui16DsmccPID);
   mMSBF_GET32_O(pu8xContent,offset,pstxDVBT->ui32Frequency);
   mMSBF_GET32_O(pu8xContent,offset,pstxDVBT->ui32BandWidth);
   mMSBF_GET08_O(pu8xContent,offset,pstxDVBT->ui8Modulation);
   mMSBF_GET08_O(pu8xContent,offset,pstxDVBT->ui8Mode);
   mMSBF_GET08_O(pu8xContent,offset,pstxDVBT->ui8Guard);
   mMSBF_GET08_O(pu8xContent,offset,pstxDVBT->ui8Hierarchy);
   mMSBF_GET08_O(pu8xContent,offset,pstxDVBT->ui8Priority);
   mMSBF_GET08_O(pu8xContent,offset,pstxDVBT->ui8ConvolutionHP);
   mMSBF_GET08_O(pu8xContent,offset,pstxDVBT->ui8ConvolutionLP);
   mMSBF_GET08_O(pu8xContent,offset,pstxDVBT->ui8FreqOffset);

   mTBOX_TRACE((kTBOX_NIV_2,"  * LOCATION DVB-T (%d bytes):\n",kCFGFi_DVBT_SIZE));
   mTBOX_TRACE((kTBOX_NIV_2,"  - PMT PID.............%#x\n",pstxDVBT->ui16PmtPID));
   mTBOX_TRACE((kTBOX_NIV_2,"  - DSMCC PID...........%#x\n",pstxDVBT->ui16DsmccPID));
   mTBOX_TRACE((kTBOX_NIV_2,"  - Frequency...........%u Hz\n",pstxDVBT->ui32Frequency));
   mTBOX_TRACE((kTBOX_NIV_2,"  - Band width..........%u\n",pstxDVBT->ui32BandWidth));
   switch (pstxDVBT->ui8Modulation)
   {
   case kCFGF_DVBT_MODULATION_BPSK:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Modulation..........%#x = BPSK\n",pstxDVBT->ui8Modulation));
      break;
   case kCFGF_DVBT_MODULATION_QPSK:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Modulation..........%#x = QPSK\n",pstxDVBT->ui8Modulation));
      break;
   case kCFGF_DVBT_MODULATION_QAM4:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Modulation..........%#x = QAM4\n",pstxDVBT->ui8Modulation));
      break;
   case kCFGF_DVBT_MODULATION_8PSK:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Modulation..........%#x = 8PSK\n",pstxDVBT->ui8Modulation));
      break;
   case kCFGF_DVBT_MODULATION_QAM16:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Modulation..........%#x = QAM16\n",pstxDVBT->ui8Modulation));
      break;
   case kCFGF_DVBT_MODULATION_QAM32:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Modulation..........%#x = QAM32\n",pstxDVBT->ui8Modulation));
      break;
   case kCFGF_DVBT_MODULATION_QAM64:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Modulation..........%#x = QAM64\n",pstxDVBT->ui8Modulation));
      break;
   case kCFGF_DVBT_MODULATION_QAM128:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Modulation..........%#x = QAM128\n",pstxDVBT->ui8Modulation));
      break;
   case kCFGF_DVBT_MODULATION_QAM256:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Modulation..........%#x = QAM256\n",pstxDVBT->ui8Modulation));
      break;
   case kCFGF_DVBT_MODULATION_AUTO:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Modulation..........%#x = AUTO\n",pstxDVBT->ui8Modulation));
      break;
   case kCFGF_DVBT_MODULATION_ANALOG:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Modulation..........%#x = ANALOG\n",pstxDVBT->ui8Modulation));
      break;
   default:
      mTBOX_TRACE((kTBOX_NIV_WARNING,
                   "%s ERROR: Modulation %#x of DVB-T Location unsupported\n",
                   TBOX_FunctionName,pstxDVBT->ui8Modulation));
      mTBOX_RETURN(-1);
   }
   switch (pstxDVBT->ui8Mode)
   {
   case kCFGF_DVBT_MODE_2K:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Mode................%#x = 2K\n",pstxDVBT->ui8Mode));
      break;
   case kCFGF_DVBT_MODE_8K:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Mode................%#x = 8K\n",pstxDVBT->ui8Mode));
      break;
   case kCFGF_DVBT_MODE_AUTO:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Mode................%#x = AUTO\n",pstxDVBT->ui8Mode));
      break;
   case kCFGF_DVBT_MODE_MAX:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Mode................%#x = MAX\n",pstxDVBT->ui8Mode));
      break;
   default:
      mTBOX_TRACE((kTBOX_NIV_WARNING,
                   "%s ERROR: Mode %#x of DVB-T Location unsupported\n",
                   TBOX_FunctionName,pstxDVBT->ui8Mode));
      mTBOX_RETURN(-1);
   }
   switch (pstxDVBT->ui8Guard)
   {
   case kCFGF_DVBT_GUARD_1_32:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Guard...............%#x = 1/32\n",pstxDVBT->ui8Guard));
      break;
   case kCFGF_DVBT_GUARD_1_16:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Guard...............%#x = 1/16\n",pstxDVBT->ui8Guard));
      break;
   case kCFGF_DVBT_GUARD_1_8:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Guard...............%#x = 1/8\n",pstxDVBT->ui8Guard));
      break;
   case kCFGF_DVBT_GUARD_1_4:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Guard...............%#x = 1/4\n",pstxDVBT->ui8Guard));
      break;
   case kCFGF_DVBT_GUARD_AUTO:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Guard...............%#x = AUTO\n",pstxDVBT->ui8Guard));
      break;
   case kCFGF_DVBT_GUARD_MAX:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Guard...............%#x = MAX\n",pstxDVBT->ui8Guard));
      break;
   default:
      mTBOX_TRACE((kTBOX_NIV_WARNING,
                   "%s ERROR: Guard %#x of DVB-T Location unsupported\n",
                   TBOX_FunctionName,pstxDVBT->ui8Guard));
      mTBOX_RETURN(-1);
   }
   switch (pstxDVBT->ui8Hierarchy)
   {
   case kCFGF_DVBT_HIER_NONE:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Hierarchy...........%#x = NONE\n",pstxDVBT->ui8Hierarchy));
      break;
   case kCFGF_DVBT_HIER_1:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Hierarchy...........%#x = 1\n",pstxDVBT->ui8Hierarchy));
      break;
   case kCFGF_DVBT_HIER_2:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Hierarchy...........%#x = 2\n",pstxDVBT->ui8Hierarchy));
      break;
   case kCFGF_DVBT_HIER_4:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Hierarchy...........%#x = 4\n",pstxDVBT->ui8Hierarchy));
      break;
   case kCFGF_DVBT_HIER_AUTO:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Hierarchy...........%#x = AUTO\n",pstxDVBT->ui8Hierarchy));
      break;
   case kCFGF_DVBT_HIER_MAX:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Hierarchy...........%#x = MAX\n",pstxDVBT->ui8Hierarchy));
      break;
   default:
      mTBOX_TRACE((kTBOX_NIV_WARNING,
                   "%s ERROR: Hierarchy %#x of DVB-T Location unsupported\n",
                   TBOX_FunctionName,pstxDVBT->ui8Hierarchy));
      mTBOX_RETURN(-1);
   }
  switch (pstxDVBT->ui8Priority)
   {
   case kCFGF_DVBT_PRIO_HIGH:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Priority............%#x = HIGH\n",pstxDVBT->ui8Priority));
      break;
   case kCFGF_DVBT_PRIO_LOW:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Priority............%#x = LOW\n",pstxDVBT->ui8Priority));
      break;
   case kCFGF_DVBT_PRIO_MAX:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Priority............%#x = MAX\n",pstxDVBT->ui8Priority));
      break;
   default:
      mTBOX_TRACE((kTBOX_NIV_WARNING,
                   "%s ERROR: Priority %#x of DVB-T Location unsupported\n",
                   TBOX_FunctionName,pstxDVBT->ui8Priority));
      mTBOX_RETURN(-1);
   }
   switch (pstxDVBT->ui8ConvolutionHP)
   {
   case kCFGF_DVBT_CONVOL_NONE:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Convolution HP......%#x = NONE\n",pstxDVBT->ui8ConvolutionHP));
      break;
   case kCFGF_DVBT_CONVOL_1_2:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Convolution HP......%#x = 1/2\n",pstxDVBT->ui8ConvolutionHP));
      break;
   case kCFGF_DVBT_CONVOL_2_3:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Convolution HP......%#x = 2/3\n",pstxDVBT->ui8ConvolutionHP));
      break;
   case kCFGF_DVBT_CONVOL_3_4:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Convolution HP......%#x = 3/4\n",pstxDVBT->ui8ConvolutionHP));
      break;
   case kCFGF_DVBT_CONVOL_4_5:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Convolution HP......%#x = 4/5\n",pstxDVBT->ui8ConvolutionHP));
      break;
   case kCFGF_DVBT_CONVOL_5_6:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Convolution HP......%#x = 5/6\n",pstxDVBT->ui8ConvolutionHP));
      break;
   case kCFGF_DVBT_CONVOL_6_7:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Convolution HP......%#x = 6/7\n",pstxDVBT->ui8ConvolutionHP));
      break;
   case kCFGF_DVBT_CONVOL_7_8:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Convolution HP......%#x = 7/8\n",pstxDVBT->ui8ConvolutionHP));
      break;
   case kCFGF_DVBT_CONVOL_8_9:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Convolution HP......%#x = 8/9\n",pstxDVBT->ui8ConvolutionHP));
      break;
   case kCFGF_DVBT_CONVOL_AUTO:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Convolution HP......%#x = AUTO\n",pstxDVBT->ui8ConvolutionHP));
      break;
   case kCFGF_DVBT_CONVOL_MAX:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Convolution HP......%#x = MAX\n",pstxDVBT->ui8ConvolutionHP));
      break;
   default:
      mTBOX_TRACE((kTBOX_NIV_WARNING,
                   "%s ERROR: Convolution HP %#x of DVB-T Location unsupported\n",
                   TBOX_FunctionName,pstxDVBT->ui8ConvolutionHP));
      mTBOX_RETURN(-1);
   }
   switch (pstxDVBT->ui8ConvolutionLP)
   {
   case kCFGF_DVBT_CONVOL_NONE:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Convolution LP......%#x = NONE\n",pstxDVBT->ui8ConvolutionLP));
      break;
   case kCFGF_DVBT_CONVOL_1_2:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Convolution LP......%#x = 1/2\n",pstxDVBT->ui8ConvolutionLP));
      break;
   case kCFGF_DVBT_CONVOL_2_3:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Convolution LP......%#x = 2/3\n",pstxDVBT->ui8ConvolutionLP));
      break;
   case kCFGF_DVBT_CONVOL_3_4:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Convolution LP......%#x = 3/4\n",pstxDVBT->ui8ConvolutionLP));
      break;
   case kCFGF_DVBT_CONVOL_4_5:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Convolution LP......%#x = 4/5\n",pstxDVBT->ui8ConvolutionLP));
      break;
   case kCFGF_DVBT_CONVOL_5_6:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Convolution LP......%#x = 5/6\n",pstxDVBT->ui8ConvolutionLP));
      break;
   case kCFGF_DVBT_CONVOL_6_7:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Convolution LP......%#x = 6/7\n",pstxDVBT->ui8ConvolutionLP));
      break;
   case kCFGF_DVBT_CONVOL_7_8:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Convolution LP......%#x = 7/8\n",pstxDVBT->ui8ConvolutionLP));
      break;
   case kCFGF_DVBT_CONVOL_8_9:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Convolution LP......%#x = 8/9\n",pstxDVBT->ui8ConvolutionLP));
      break;
   case kCFGF_DVBT_CONVOL_AUTO:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Convolution LP......%#x = AUTO\n",pstxDVBT->ui8ConvolutionLP));
      break;
   case kCFGF_DVBT_CONVOL_MAX:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Convolution LP......%#x = MAX\n",pstxDVBT->ui8ConvolutionLP));
      break;
   default:
      mTBOX_TRACE((kTBOX_NIV_WARNING,
                   "%s ERROR: Convolution LP %#x of DVB-T Location unsupported\n",
                   TBOX_FunctionName,pstxDVBT->ui8ConvolutionLP));
      mTBOX_RETURN(-1);
   }
   switch (pstxDVBT->ui8FreqOffset)
   {
   case kCFGF_DVBT_FREQOFF_NONE:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Frequency Offset....%#x = NONE\n",pstxDVBT->ui8FreqOffset));
      break;
   case kCFGF_DVBT_FREQOFF_1_6_MORE:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Frequency Offset....%#x = + 1/6 Mhz\n",pstxDVBT->ui8FreqOffset));
      break;
   case kCFGF_DVBT_FREQOFF_1_6_MINUS:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Frequency Offset....%#x = - 1/6 Mhz\n",pstxDVBT->ui8FreqOffset));
      break;
   case kCFGF_DVBT_FREQOFF_2_6_MORE:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Frequency Offset....%#x = + 2/6 Mhz\n",pstxDVBT->ui8FreqOffset));
      break;
   case kCFGF_DVBT_FREQOFF_2_6_MINUS:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Frequency Offset....%#x = - 2/6 Mhz\n",pstxDVBT->ui8FreqOffset));
      break;
   case kCFGF_DVBT_FREQOFF_3_6_MORE:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Frequency Offset....%#x = + 3/6 Mhz\n",pstxDVBT->ui8FreqOffset));
      break;
   case kCFGF_DVBT_FREQOFF_3_6_MINUS:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Frequency Offset....%#x = - 3/6 Mhz\n",pstxDVBT->ui8FreqOffset));
      break;
   case kCFGF_DVBT_FREQOFF_AUTO:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Frequency Offset....%#x = AUTO\n",pstxDVBT->ui8FreqOffset));
      break;
   case kCFGF_DVBT_FREQOFF_MAX:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Frequency Offset....%#x = MAX\n",pstxDVBT->ui8FreqOffset));
      break;
   default:
      mTBOX_TRACE((kTBOX_NIV_WARNING,
                   "%s ERROR: Frequency Offset %#x of DVB-T Location unsupported\n",
                   TBOX_FunctionName,pstxDVBT->ui8FreqOffset));
      mTBOX_RETURN(-1);
   }

   mTBOX_RETURN(offset);
}

/**
 *  @brief
 *    Extract the DVB-T2 Location section data.
 *
 *    Syntax                             No. of bits
 *    location_dvb-t2 {
 *       pmt_pid                                  16
 *       dsmcc_pid                                16
 *       frequency                                32
 *       band_width                               32
 *       mode                                      8
 *       guard                                     8
 *       plp_id                                    8
 *    }
 *
 *  @param[in/out] pstxDVBT2
 *              DVB-T2 location extracted if defined (not NULL)
 *
 *  @param[in] pu8xContent
 *              Pointer on begining of config file
 *
 *  @param[in] i32xSize
 *              Size of config file in bytes
 *
 *  @param[in] i32xOffset
 *              Offset of begining Serial section
 *              in content data.
 *
 *  @retval  > 0
 *              Successful, offset of next section begining 
 *
 *  @retval  < 1
 *              Extraction failed
 *
 *  @remarks
 *    -# INVARIANT: The pointer on file content pu8xContent 
 *                  is not NULL.
 *
 *    -# If the pointer on data pstxDVBT2 is NULL this function
 *       is used to jump on this section.
 *
*/
LOCAL int32_t CFGFi_GetLocDVBT2
(
   tCFGF_LocDVBT2 *pstxDVBT2,
   uint8_t        *pu8xContent,
   int32_t         i32xSize,
   int32_t         i32xOffset
)
{
   int32_t offset = i32xOffset;

   mTBOX_FCT_ENTER(CFGFi_GetLocDVBT2);

   if ((offset + kCFGFi_DVBT2_SIZE) > i32xSize)
   {
      mTBOX_TRACE((kTBOX_NIV_WARNING,
                   "%s ERROR: DVB-T2 Location section on %d bytes not found "
                   "at offset %d in the %d bytes of config file\n",
                   TBOX_FunctionName,kCFGFi_DVBT2_SIZE,offset,i32xSize));
      mTBOX_RETURN(-1);
   }

   if (pstxDVBT2 == NULL)
   {
      /* Jump after this section */
      mTBOX_RETURN(offset+kCFGFi_DVBT2_SIZE);
   }

   mMSBF_GET16_O(pu8xContent,offset,pstxDVBT2->ui16PmtPID);
   mMSBF_GET16_O(pu8xContent,offset,pstxDVBT2->ui16DsmccPID);
   mMSBF_GET32_O(pu8xContent,offset,pstxDVBT2->ui32Frequency);
   mMSBF_GET32_O(pu8xContent,offset,pstxDVBT2->ui32BandWidth);
   mMSBF_GET08_O(pu8xContent,offset,pstxDVBT2->ui8Mode);
   mMSBF_GET08_O(pu8xContent,offset,pstxDVBT2->ui8Guard);
   mMSBF_GET08_O(pu8xContent,offset,pstxDVBT2->ui8PlpID);

   mTBOX_TRACE((kTBOX_NIV_2,"  * LOCATION DVB-T2 (%d bytes):\n",kCFGFi_DVBT2_SIZE));
   mTBOX_TRACE((kTBOX_NIV_2,"  - PMT PID.............%#x\n",pstxDVBT2->ui16PmtPID));
   mTBOX_TRACE((kTBOX_NIV_2,"  - DSMCC PID...........%#x\n",pstxDVBT2->ui16DsmccPID));
   mTBOX_TRACE((kTBOX_NIV_2,"  - Frequency...........%u Hz\n",pstxDVBT2->ui32Frequency));
   mTBOX_TRACE((kTBOX_NIV_2,"  - Band width..........%u\n",pstxDVBT2->ui32BandWidth));
   switch (pstxDVBT2->ui8Mode)
   {
   case kCFGF_DVBT2_MODE_2K:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Mode................%#x = 2K\n",pstxDVBT2->ui8Mode));
      break;
   case kCFGF_DVBT2_MODE_8K:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Mode................%#x = 8K\n",pstxDVBT2->ui8Mode));
      break;
   case kCFGF_DVBT2_MODE_4K:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Mode................%#x = 4K\n",pstxDVBT2->ui8Mode));
      break;
   case kCFGF_DVBT2_MODE_1K:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Mode................%#x = 1K\n",pstxDVBT2->ui8Mode));
      break;
   case kCFGF_DVBT2_MODE_16K:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Mode................%#x = 16K\n",pstxDVBT2->ui8Mode));
      break;
   case kCFGF_DVBT2_MODE_32K:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Mode................%#x = 32K\n",pstxDVBT2->ui8Mode));
      break;
      case kCFGF_DVBT2_MODE_AUTO:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Mode................%#x = AUTO\n",pstxDVBT2->ui8Mode));
      break;
   case kCFGF_DVBT2_MODE_MAX:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Mode................%#x = MAX\n",pstxDVBT2->ui8Mode));
      break;
   default:
      mTBOX_TRACE((kTBOX_NIV_WARNING,
                   "%s ERROR: Mode %#x of DVB-T2 Location unsupported\n",
                   TBOX_FunctionName,pstxDVBT2->ui8Mode));
      mTBOX_RETURN(-1);
   }
   switch (pstxDVBT2->ui8Guard)
   {
   case kCFGF_DVBT2_GUARD_1_32:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Guard...............%#x = 1/32\n",pstxDVBT2->ui8Guard));
      break;
   case kCFGF_DVBT2_GUARD_1_16:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Guard...............%#x = 1/16\n",pstxDVBT2->ui8Guard));
      break;
   case kCFGF_DVBT2_GUARD_1_8:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Guard...............%#x = 1/8\n",pstxDVBT2->ui8Guard));
      break;
   case kCFGF_DVBT2_GUARD_1_4:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Guard...............%#x = 1/4\n",pstxDVBT2->ui8Guard));
      break;
   case kCFGF_DVBT2_GUARD_1_128:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Guard...............%#x = 1/128\n",pstxDVBT2->ui8Guard));
      break;
   case kCFGF_DVBT2_GUARD_19_128:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Guard...............%#x = 19/128\n",pstxDVBT2->ui8Guard));
      break;
   case kCFGF_DVBT2_GUARD_19_256:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Guard...............%#x = 19/256\n",pstxDVBT2->ui8Guard));
      break;
      case kCFGF_DVBT2_GUARD_AUTO:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Guard...............%#x = AUTO\n",pstxDVBT2->ui8Guard));
      break;
   case kCFGF_DVBT2_GUARD_MAX:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Guard...............%#x = MAX\n",pstxDVBT2->ui8Guard));
      break;
   default:
      mTBOX_TRACE((kTBOX_NIV_WARNING,
                   "%s ERROR: Guard %#x of DVB-T2 Location unsupported\n",
                   TBOX_FunctionName,pstxDVBT2->ui8Guard));
      mTBOX_RETURN(-1);
   }
   mTBOX_TRACE((kTBOX_NIV_2,"  - PLP ID..............%u\n",pstxDVBT2->ui8PlpID));

   mTBOX_RETURN(offset);
}
#endif /* MEDIUM_TER */

#if defined (MEDIUM_CAB)
/**
 *  @brief
 *    Extract the DVB-C Location section data.
 *
 *    Syntax                             No. of bits
 *    location_dvb-c {
 *       pmt_pid                                  16
 *       dsmcc_pid                                16
 *       network_id                               16
 *       frequency                                32
 *       symbol_rate                              32
 *       modulation                                8
 *    }
 *
 *  @param[in/out] pstxDVBC
 *              DVB-C location extracted if defined (not NULL)
 *
 *  @param[in] pu8xContent
 *              Pointer on begining of config file
 *
 *  @param[in] i32xSize
 *              Size of config file in bytes
 *
 *  @param[in] i32xOffset
 *              Offset of begining Serial section
 *              in content data.
 *
 *  @retval  > 0
 *              Successful, offset of next section begining 
 *
 *  @retval  < 1
 *              Extraction failed
 *
 *  @remarks
 *    -# INVARIANT: The pointer on file content pu8xContent 
 *                  is not NULL.
 *
 *    -# If the pointer on data pstxDVBC is NULL this function
 *       is used to jump on this section.
 *
*/
LOCAL int32_t CFGFi_GetLocDVBC
(
   tCFGF_LocDVBC *pstxDVBC,
   uint8_t       *pu8xContent,
   int32_t        i32xSize,
   int32_t        i32xOffset
)
{
   int32_t offset = i32xOffset;

   mTBOX_FCT_ENTER(CFGFi_GetLocDVBC);

   if ((offset + kCFGFi_DVBC_SIZE) > i32xSize)
   {
      mTBOX_TRACE((kTBOX_NIV_WARNING,
                   "%s ERROR: DVB-C Location section on %d bytes not found "
                   "at offset %d in the %d bytes of config file\n",
                   TBOX_FunctionName,kCFGFi_DVBC_SIZE,offset,i32xSize));
      mTBOX_RETURN(-1);
   }

   if (pstxDVBC == NULL)
   {
      /* Jump after this section */
      mTBOX_RETURN(offset+kCFGFi_DVBC_SIZE);
   }

   mMSBF_GET16_O(pu8xContent,offset,pstxDVBC->ui16PmtPID);
   mMSBF_GET16_O(pu8xContent,offset,pstxDVBC->ui16DsmccPID);
   mMSBF_GET16_O(pu8xContent,offset,pstxDVBC->ui16NetworkID);
   mMSBF_GET32_O(pu8xContent,offset,pstxDVBC->ui32Frequency);
   mMSBF_GET32_O(pu8xContent,offset,pstxDVBC->ui32SymbolRate);
   mMSBF_GET08_O(pu8xContent,offset,pstxDVBC->ui8Modulation);

   mTBOX_TRACE((kTBOX_NIV_2,"  * LOCATION DVB-C (%d bytes):\n",kCFGFi_DVBC_SIZE));
   mTBOX_TRACE((kTBOX_NIV_2,"  - PMT PID.............%#x\n",pstxDVBC->ui16PmtPID));
   mTBOX_TRACE((kTBOX_NIV_2,"  - DSMCC PID...........%#x\n",pstxDVBC->ui16DsmccPID));
   mTBOX_TRACE((kTBOX_NIV_2,"  - Network ID..........%u\n",pstxDVBC->ui16NetworkID));
   mTBOX_TRACE((kTBOX_NIV_2,"  - Frequency...........%u Hz\n",pstxDVBC->ui32Frequency));
   mTBOX_TRACE((kTBOX_NIV_2,"  - Symbol rate.........%u Baud\n",pstxDVBC->ui32SymbolRate));
   switch (pstxDVBC->ui8Modulation)
   {
   case kCFGF_DVBC_MODULATION_BPSK:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Modulation..........%#x = BPSK\n",pstxDVBC->ui8Modulation));
      break;
   case kCFGF_DVBC_MODULATION_QPSK:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Modulation..........%#x = QPSK\n",pstxDVBC->ui8Modulation));
      break;
   case kCFGF_DVBC_MODULATION_QAM4:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Modulation..........%#x = QAM4\n",pstxDVBC->ui8Modulation));
      break;
   case kCFGF_DVBC_MODULATION_8PSK:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Modulation..........%#x = 8PSK\n",pstxDVBC->ui8Modulation));
      break;
   case kCFGF_DVBC_MODULATION_QAM16:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Modulation..........%#x = QAM16\n",pstxDVBC->ui8Modulation));
      break;
   case kCFGF_DVBC_MODULATION_QAM32:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Modulation..........%#x = QAM32\n",pstxDVBC->ui8Modulation));
      break;
   case kCFGF_DVBC_MODULATION_QAM64:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Modulation..........%#x = QAM64\n",pstxDVBC->ui8Modulation));
      break;
   case kCFGF_DVBC_MODULATION_QAM128:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Modulation..........%#x = QAM128\n",pstxDVBC->ui8Modulation));
      break;
   case kCFGF_DVBC_MODULATION_QAM256:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Modulation..........%#x = QAM256\n",pstxDVBC->ui8Modulation));
      break;
   case kCFGF_DVBC_MODULATION_AUTO:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Modulation..........%#x = AUTO\n",pstxDVBC->ui8Modulation));
      break;
   case kCFGF_DVBC_MODULATION_ANALOG:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Modulation..........%#x = ANALOG\n",pstxDVBC->ui8Modulation));
      break;
   default:
      mTBOX_TRACE((kTBOX_NIV_WARNING,
                   "%s ERROR: Modulation %#x of DVB-C Location unsupported\n",
                   TBOX_FunctionName,pstxDVBC->ui8Modulation));
      mTBOX_RETURN(-1);
   }

   mTBOX_RETURN(offset);
}
#endif /* MEDIUM_CAB */

#if defined (MEDIUM_SAT)
/**
 *  @brief
 *    Extract the DVB-S Location section data.
 *
 *    Syntax                             No. of bits
 *    location_dvb-s {
 *       pmt_pid                                  16
 *       dsmcc_pid                                16
 *       frequency                                32
 *       symbol_rate                              32
 *       polarization                              8
 *       modulation                                8
 *       fec_rate                                  8
 *       outdor_selector                           8
 *    }
 *
 *  @param[in/out] pstxDVBS
 *              DVB-S location extracted if defined (not NULL)
 *
 *  @param[in] pu8xContent
 *              Pointer on begining of config file
 *
 *  @param[in] i32xSize
 *              Size of config file in bytes
 *
 *  @param[in] i32xOffset
 *              Offset of begining Serial section
 *              in content data.
 *
 *  @retval  > 0
 *              Successful, offset of next section begining 
 *
 *  @retval  < 1
 *              Extraction failed
 *
 *  @remarks
 *    -# INVARIANT: The pointer on file content pu8xContent 
 *                  is not NULL.
 *
 *    -# If the pointer on data pstxDVBS is NULL this function
 *       is used to jump on this section.
 *
*/
LOCAL int32_t CFGFi_GetLocDVBS
(
   tCFGF_LocDVBS *pstxDVBS,
   uint8_t       *pu8xContent,
   int32_t        i32xSize,
   int32_t        i32xOffset
)
{
   int32_t offset = i32xOffset;

   mTBOX_FCT_ENTER(CFGFi_GetLocDVBS);

   if ((offset + kCFGFi_DVBS_SIZE) > i32xSize)
   {
      mTBOX_TRACE((kTBOX_NIV_WARNING,
                   "%s ERROR: DVB-S Location section on %d bytes not found "
                   "at offset %d in the %d bytes of config file\n",
                   TBOX_FunctionName,kCFGFi_DVBS_SIZE,offset,i32xSize));
      mTBOX_RETURN(-1);
   }

   if (pstxDVBS == NULL)
   {
      /* Jump after this section */
      mTBOX_RETURN(offset+kCFGFi_DVBS_SIZE);
   }

   mMSBF_GET16_O(pu8xContent,offset,pstxDVBS->ui16PmtPID);
   mMSBF_GET16_O(pu8xContent,offset,pstxDVBS->ui16DsmccPID);
   mMSBF_GET32_O(pu8xContent,offset,pstxDVBS->ui32Frequency);
   mMSBF_GET32_O(pu8xContent,offset,pstxDVBS->ui32SymbolRate);
   mMSBF_GET08_O(pu8xContent,offset,pstxDVBS->ui8Polarization); /* carry both LNB power & polarization & Tone state */
   mMSBF_GET08_O(pu8xContent,offset,pstxDVBS->ui8Modulation);
   mMSBF_GET08_O(pu8xContent,offset,pstxDVBS->ui8FecRate);
   mMSBF_GET08_O(pu8xContent,offset,pstxDVBS->ui8OutSelector);

   mTBOX_TRACE((kTBOX_NIV_2,"  * LOCATION DVB-S (%d bytes):\n",kCFGFi_DVBS_SIZE));
   mTBOX_TRACE((kTBOX_NIV_2,"  - PMT PID.............%#x\n",pstxDVBS->ui16PmtPID));
   mTBOX_TRACE((kTBOX_NIV_2,"  - DSMCC PID...........%#x\n",pstxDVBS->ui16DsmccPID));
   mTBOX_TRACE((kTBOX_NIV_2,"  - Frequency...........%u Hz\n",pstxDVBS->ui32Frequency));
   mTBOX_TRACE((kTBOX_NIV_2,"  - Symbol Rate.........%u Baud\n",pstxDVBS->ui32SymbolRate));
   mTBOX_TRACE((kTBOX_NIV_2,"  - Polarization.........Tone=%d \n",pstxDVBS->ui8Polarization & 0x04));
   mTBOX_TRACE((kTBOX_NIV_2,"  - Polarization.........Power=%d  \n",pstxDVBS->ui8Polarization & 0x02));
   mTBOX_TRACE((kTBOX_NIV_2,"  - Polarization.........Vertical=%d \n",pstxDVBS->ui8Polarization & 0x01));
   if ((pstxDVBS->ui8Polarization & 0x0F8) != 0)
   {
      mTBOX_TRACE((kTBOX_NIV_WARNING,
                   "%s ERROR: Polarization reserved bits not null %#x \n",
                   TBOX_FunctionName,pstxDVBS->ui8Polarization));
      /* mTBOX_RETURN(-1); */
   }
   switch (pstxDVBS->ui8Modulation)
   {
   case kCFGF_DVBS_MODULATION_BPSK:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Modulation..........%#x = BPSK\n",pstxDVBS->ui8Modulation));
      break;
   case kCFGF_DVBS_MODULATION_QPSK:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Modulation..........%#x = QPSK\n",pstxDVBS->ui8Modulation));
      break;
   case kCFGF_DVBS_MODULATION_QAM4:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Modulation..........%#x = QAM4\n",pstxDVBS->ui8Modulation));
      break;
   case kCFGF_DVBS_MODULATION_8PSK:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Modulation..........%#x = 8PSK\n",pstxDVBS->ui8Modulation));
      break;
   case kCFGF_DVBS_MODULATION_QAM16:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Modulation..........%#x = QAM16\n",pstxDVBS->ui8Modulation));
      break;
   case kCFGF_DVBS_MODULATION_QAM32:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Modulation..........%#x = QAM32\n",pstxDVBS->ui8Modulation));
      break;
   case kCFGF_DVBS_MODULATION_QAM64:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Modulation..........%#x = QAM64\n",pstxDVBS->ui8Modulation));
      break;
   case kCFGF_DVBS_MODULATION_QAM128:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Modulation..........%#x = QAM128\n",pstxDVBS->ui8Modulation));
      break;
   case kCFGF_DVBS_MODULATION_QAM256:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Modulation..........%#x = QAM256\n",pstxDVBS->ui8Modulation));
      break;
   case kCFGF_DVBS_MODULATION_AUTO:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Modulation..........%#x = AUTO\n",pstxDVBS->ui8Modulation));
      break;
   case kCFGF_DVBS_MODULATION_ANALOG:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Modulation..........%#x = ANALOG\n",pstxDVBS->ui8Modulation));
      break;
   default:
      mTBOX_TRACE((kTBOX_NIV_WARNING,
                   "%s ERROR: Modulation %#x of DVB-S Location unsupported\n",
                   TBOX_FunctionName,pstxDVBS->ui8Modulation));
      mTBOX_RETURN(-1);
   }
   switch (pstxDVBS->ui8FecRate)
   {
   case kCFGF_DVBS_FECRATE_NONE:
      mTBOX_TRACE((kTBOX_NIV_2,"  - FEC Rate............%#x = NONE\n",pstxDVBS->ui8FecRate));
      break;
   case kCFGF_DVBS_FECRATE_1_2:
      mTBOX_TRACE((kTBOX_NIV_2,"  - FEC Rate............%#x = 1/2\n",pstxDVBS->ui8FecRate));
      break;
   case kCFGF_DVBS_FECRATE_2_3:
      mTBOX_TRACE((kTBOX_NIV_2,"  - FEC Rate............%#x = 2/3\n",pstxDVBS->ui8FecRate));
      break;
   case kCFGF_DVBS_FECRATE_3_4:
      mTBOX_TRACE((kTBOX_NIV_2,"  - FEC Rate............%#x = 3/4\n",pstxDVBS->ui8FecRate));
      break;
   case kCFGF_DVBS_FECRATE_4_5:
      mTBOX_TRACE((kTBOX_NIV_2,"  - FEC Rate............%#x = 4/5\n",pstxDVBS->ui8FecRate));
      break;
   case kCFGF_DVBS_FECRATE_5_6:
      mTBOX_TRACE((kTBOX_NIV_2,"  - FEC Rate............%#x = 5/6\n",pstxDVBS->ui8FecRate));
      break;
   case kCFGF_DVBS_FECRATE_6_7:
      mTBOX_TRACE((kTBOX_NIV_2,"  - FEC Rate............%#x = 6/7\n",pstxDVBS->ui8FecRate));
      break;
   case kCFGF_DVBS_FECRATE_7_8:
      mTBOX_TRACE((kTBOX_NIV_2,"  - FEC Rate............%#x = 7/8\n",pstxDVBS->ui8FecRate));
      break;
   case kCFGF_DVBS_FECRATE_8_9:
      mTBOX_TRACE((kTBOX_NIV_2,"  - FEC Rate............%#x = 8/9\n",pstxDVBS->ui8FecRate));
      break;
   case kCFGF_DVBS_FECRATE_AUTO:
      mTBOX_TRACE((kTBOX_NIV_2,"  - FEC Rate............%#x = AUTO\n",pstxDVBS->ui8FecRate));
      break;
   case kCFGF_DVBS_FECRATE_MAX:
      mTBOX_TRACE((kTBOX_NIV_2,"  - FEC Rate............%#x = MAX\n",pstxDVBS->ui8FecRate));
      break;
   default:
      mTBOX_TRACE((kTBOX_NIV_WARNING,
                   "%s ERROR: FEC Rate %#x of DVB-S Location unsupported\n",
                   TBOX_FunctionName,pstxDVBS->ui8FecRate));
      mTBOX_RETURN(-1);
   }
   switch (pstxDVBS->ui8OutSelector)
   {
   case kCFGF_DVBS_OUTSEL_NONE:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Outdor Selector.....%#x = NONE\n",pstxDVBS->ui8OutSelector));
      break;
   case kCFGF_DVBS_OUTSEL_TRACK1:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Outdor Selector.....%#x = TRACK 1\n",pstxDVBS->ui8OutSelector));
      break;
   case kCFGF_DVBS_OUTSEL_TRACK2:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Outdor Selector.....%#x = TRACK 2\n",pstxDVBS->ui8OutSelector));
      break;
   case kCFGF_DVBS_OUTSEL_TRACK3:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Outdor Selector.....%#x = TRACK 3\n",pstxDVBS->ui8OutSelector));
      break;
   case kCFGF_DVBS_OUTSEL_TRACK4:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Outdor Selector.....%#x = TRACK 4\n",pstxDVBS->ui8OutSelector));
      break;
   default:
      mTBOX_TRACE((kTBOX_NIV_WARNING,
                   "%s ERROR: Outdor Selector %#x of DVB-S Location unsupported\n",
                   TBOX_FunctionName,pstxDVBS->ui8OutSelector));
      mTBOX_RETURN(-1);
   }

   mTBOX_RETURN(offset);
}

/**
 *  @brief
 *    Extract the DVB-S2 Location section data.
 *
 *    Syntax                             No. of bits
 *    location_dvb-s2 {
 *       pmt_pid                                  16
 *       dsmcc_pid                                16
 *       frequency                                32
 *       symbol_rate                              32
 *       polarization                              8
 *       modulation                                8
 *       fec_rate                                  8
 *       outdor_selector                           8
 *    }
 *
 *    Note: Not totaly specified => prepare it with same DVB-S structure
 *          => CFGFi_GetLocDVBS2(...) idem CFGFi_GetLocDVBS(...) 
 *
 *  @param[in/out] pstxDVBS2
 *              DVB-S2 location extracted if defined (not NULL)
 *
 *  @param[in] pu8xContent
 *              Pointer on begining of config file
 *
 *  @param[in] i32xSize
 *              Size of config file in bytes
 *
 *  @param[in] i32xOffset
 *              Offset of begining Serial section
 *              in content data.
 *
 *  @retval  > 0
 *              Successful, offset of next section begining 
 *
 *  @retval  < 1
 *              Extraction failed
 *
 *  @remarks
 *    -# INVARIANT: The pointer on file content pu8xContent 
 *                  is not NULL.
 *
 *    -# If the pointer on data pstxDVBS2 is NULL this function
 *       is used to jump on this section.
 *
*/
LOCAL int32_t CFGFi_GetLocDVBS2
(
   tCFGF_LocDVBS2 *pstxDVBS2,
   uint8_t        *pu8xContent,
   int32_t         i32xSize,
   int32_t         i32xOffset
)
{
   int32_t offset = i32xOffset;

   mTBOX_FCT_ENTER(CFGFi_GetLocDVBS2);

   if ((offset + kCFGFi_DVBS2_SIZE) > i32xSize)
   {
      mTBOX_TRACE((kTBOX_NIV_WARNING,
                   "%s ERROR: DVB-S2 Location section on %d bytes not found "
                   "at offset %d in the %d bytes of config file\n",
                   TBOX_FunctionName,kCFGFi_DVBS2_SIZE,offset,i32xSize));
      mTBOX_RETURN(-1);
   }

   if (pstxDVBS2 == NULL)
   {
      /* Jump after this section */
      mTBOX_RETURN(offset+kCFGFi_DVBS2_SIZE);
   }

   mMSBF_GET16_O(pu8xContent,offset,pstxDVBS2->ui16PmtPID);
   mMSBF_GET16_O(pu8xContent,offset,pstxDVBS2->ui16DsmccPID);
   mMSBF_GET32_O(pu8xContent,offset,pstxDVBS2->ui32Frequency);
   mMSBF_GET32_O(pu8xContent,offset,pstxDVBS2->ui32SymbolRate);
   mMSBF_GET08_O(pu8xContent,offset,pstxDVBS2->ui8Polarization);
   mMSBF_GET08_O(pu8xContent,offset,pstxDVBS2->ui8Modulation);
   mMSBF_GET08_O(pu8xContent,offset,pstxDVBS2->ui8FecRate);
   mMSBF_GET08_O(pu8xContent,offset,pstxDVBS2->ui8OutSelector);

   mTBOX_TRACE((kTBOX_NIV_2,"  * LOCATION DVB-S2 (%d bytes):\n",kCFGFi_DVBS2_SIZE));
   mTBOX_TRACE((kTBOX_NIV_2,"  - PMT PID.............%#x\n",pstxDVBS2->ui16PmtPID));
   mTBOX_TRACE((kTBOX_NIV_2,"  - DSMCC PID...........%#x\n",pstxDVBS2->ui16DsmccPID));
   mTBOX_TRACE((kTBOX_NIV_2,"  - Frequency...........%u Hz\n",pstxDVBS2->ui32Frequency));
   mTBOX_TRACE((kTBOX_NIV_2,"  - Symbol Rate.........%u Baud\n",pstxDVBS2->ui32SymbolRate));
   switch (pstxDVBS2->ui8Polarization)
   {
   case kCFGF_DVBS_POLARIZ_HORIZONTAL:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Polarization........%#x = HORIZONTAL\n",pstxDVBS2->ui8Polarization));
      break;
   case kCFGF_DVBS_POLARIZ_VERTICAL:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Polarization........%#x = VERTICAL\n",pstxDVBS2->ui8Polarization));
      break;
   case kCFGF_DVBS_POLARIZ_CIRC_LEFT:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Polarization........%#x = CIRCULAR LEFT\n",pstxDVBS2->ui8Polarization));
      break;
   case kCFGF_DVBS_POLARIZ_CIRC_RIGHT:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Polarization........%#x = CIRCULAR RIGHT\n",pstxDVBS2->ui8Polarization));
      break;
   case kCFGF_DVBS_POLARIZ_ALL:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Polarization........%#x = ALL\n",pstxDVBS2->ui8Polarization));
      break;
   default:
      mTBOX_TRACE((kTBOX_NIV_WARNING,
                   "%s ERROR: Polarization %#x of DVB-S2 Location unsupported\n",
                   TBOX_FunctionName,pstxDVBS2->ui8Polarization));
      mTBOX_RETURN(-1);
   }
   switch (pstxDVBS2->ui8Modulation)
   {
   case kCFGF_DVBS_MODULATION_BPSK:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Modulation..........%#x = BPSK\n",pstxDVBS2->ui8Modulation));
      break;
   case kCFGF_DVBS_MODULATION_QPSK:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Modulation..........%#x = QPSK\n",pstxDVBS2->ui8Modulation));
      break;
   case kCFGF_DVBS_MODULATION_QAM4:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Modulation..........%#x = QAM4\n",pstxDVBS2->ui8Modulation));
      break;
   case kCFGF_DVBS_MODULATION_8PSK:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Modulation..........%#x = 8PSK\n",pstxDVBS2->ui8Modulation));
      break;
   case kCFGF_DVBS_MODULATION_QAM16:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Modulation..........%#x = QAM16\n",pstxDVBS2->ui8Modulation));
      break;
   case kCFGF_DVBS_MODULATION_QAM32:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Modulation..........%#x = QAM32\n",pstxDVBS2->ui8Modulation));
      break;
   case kCFGF_DVBS_MODULATION_QAM64:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Modulation..........%#x = QAM64\n",pstxDVBS2->ui8Modulation));
      break;
   case kCFGF_DVBS_MODULATION_QAM128:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Modulation..........%#x = QAM128\n",pstxDVBS2->ui8Modulation));
      break;
   case kCFGF_DVBS_MODULATION_QAM256:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Modulation..........%#x = QAM256\n",pstxDVBS2->ui8Modulation));
      break;
   case kCFGF_DVBS_MODULATION_AUTO:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Modulation..........%#x = AUTO\n",pstxDVBS2->ui8Modulation));
      break;
   case kCFGF_DVBS_MODULATION_ANALOG:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Modulation..........%#x = ANALOG\n",pstxDVBS2->ui8Modulation));
      break;
   default:
      mTBOX_TRACE((kTBOX_NIV_WARNING,
                   "%s ERROR: Modulation %#x of DVB-S2 Location unsupported\n",
                   TBOX_FunctionName,pstxDVBS2->ui8Modulation));
      mTBOX_RETURN(-1);
   }
   switch (pstxDVBS2->ui8FecRate)
   {
   case kCFGF_DVBS_FECRATE_NONE:
      mTBOX_TRACE((kTBOX_NIV_2,"  - FEC Rate............%#x = NONE\n",pstxDVBS2->ui8FecRate));
      break;
   case kCFGF_DVBS_FECRATE_1_2:
      mTBOX_TRACE((kTBOX_NIV_2,"  - FEC Rate............%#x = 1/2\n",pstxDVBS2->ui8FecRate));
      break;
   case kCFGF_DVBS_FECRATE_2_3:
      mTBOX_TRACE((kTBOX_NIV_2,"  - FEC Rate............%#x = 2/3\n",pstxDVBS2->ui8FecRate));
      break;
   case kCFGF_DVBS_FECRATE_3_4:
      mTBOX_TRACE((kTBOX_NIV_2,"  - FEC Rate............%#x = 3/4\n",pstxDVBS2->ui8FecRate));
      break;
   case kCFGF_DVBS_FECRATE_4_5:
      mTBOX_TRACE((kTBOX_NIV_2,"  - FEC Rate............%#x = 4/5\n",pstxDVBS2->ui8FecRate));
      break;
   case kCFGF_DVBS_FECRATE_5_6:
      mTBOX_TRACE((kTBOX_NIV_2,"  - FEC Rate............%#x = 5/6\n",pstxDVBS2->ui8FecRate));
      break;
   case kCFGF_DVBS_FECRATE_6_7:
      mTBOX_TRACE((kTBOX_NIV_2,"  - FEC Rate............%#x = 6/7\n",pstxDVBS2->ui8FecRate));
      break;
   case kCFGF_DVBS_FECRATE_7_8:
      mTBOX_TRACE((kTBOX_NIV_2,"  - FEC Rate............%#x = 7/8\n",pstxDVBS2->ui8FecRate));
      break;
   case kCFGF_DVBS_FECRATE_8_9:
      mTBOX_TRACE((kTBOX_NIV_2,"  - FEC Rate............%#x = 8/9\n",pstxDVBS2->ui8FecRate));
      break;
   case kCFGF_DVBS_FECRATE_AUTO:
      mTBOX_TRACE((kTBOX_NIV_2,"  - FEC Rate............%#x = AUTO\n",pstxDVBS2->ui8FecRate));
      break;
   case kCFGF_DVBS_FECRATE_MAX:
      mTBOX_TRACE((kTBOX_NIV_2,"  - FEC Rate............%#x = MAX\n",pstxDVBS2->ui8FecRate));
      break;
   default:
      mTBOX_TRACE((kTBOX_NIV_WARNING,
                   "%s ERROR: FEC Rate %#x of DVB-S2 Location unsupported\n",
                   TBOX_FunctionName,pstxDVBS2->ui8FecRate));
      mTBOX_RETURN(-1);
   }
   switch (pstxDVBS2->ui8OutSelector)
   {
   case kCFGF_DVBS_OUTSEL_NONE:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Outdor Selector.....%#x = NONE\n",pstxDVBS2->ui8OutSelector));
      break;
   case kCFGF_DVBS_OUTSEL_TRACK1:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Outdor Selector.....%#x = TRACK 1\n",pstxDVBS2->ui8OutSelector));
      break;
   case kCFGF_DVBS_OUTSEL_TRACK2:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Outdor Selector.....%#x = TRACK 2\n",pstxDVBS2->ui8OutSelector));
      break;
   case kCFGF_DVBS_OUTSEL_TRACK3:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Outdor Selector.....%#x = TRACK 3\n",pstxDVBS2->ui8OutSelector));
      break;
   case kCFGF_DVBS_OUTSEL_TRACK4:
      mTBOX_TRACE((kTBOX_NIV_2,"  - Outdor Selector.....%#x = TRACK 4\n",pstxDVBS2->ui8OutSelector));
      break;
   default:
      mTBOX_TRACE((kTBOX_NIV_WARNING,
                   "%s ERROR: Outdor Selector %#x of DVB-S2 Location unsupported\n",
                   TBOX_FunctionName,pstxDVBS2->ui8OutSelector));
      mTBOX_RETURN(-1);
   }

   mTBOX_RETURN(offset);
}
#endif /* MEDIUM_SAT */


/**
 *  @brief
 *    Extract the Downloader custom section data.
 *
 *    Syntax                             No. of bits
 *    downloader_custom {
 *       section_acquisition_timeout              32
 *       carousel_maxloop                          8
 *    }
 *
 *  @param[in/out] pstxDldCustom
 *              Downloader Custom extracted
 *
 *  @param[in] pu8xContent
 *              Pointer on begining of config file
 *
 *  @param[in] i32xSize
 *              Size of config file in bytes
 *
 *  @param[in] i32xOffset
 *              Offset of begining Product IDs section
 *              in content data.
 *
 *  @retval  TRUE
 *              Successful
 *
 *  @retval  FALSE
 *              Extraction failed
 *
 *  @remarks
 *    -# INVARIANT: The pointers on section pstxDldCustom and
 *                  file content pu8xContent are not NULL.
 *
*/
LOCAL bool CFGFi_GetDownldCustom
(
   tCFGF_DownldCustom *pstxDldCustom,
   uint8_t            *pu8xContent,
   int32_t             i32xSize,
   int32_t             i32xOffset
)
{
   int32_t offset = i32xOffset;

   mTBOX_FCT_ENTER(CFGFi_GetDownldCustom);

   if ((offset + kCFGFi_DLDC_SIZE) > i32xSize)
   {
      mTBOX_TRACE((kTBOX_NIV_WARNING,
                   "%s ERROR: Downloader Custom section on %d bytes not found "
                   "at offset %d in the %d bytes of config file\n",
                   TBOX_FunctionName,kCFGFi_DLDC_SIZE,offset,i32xSize));
      mTBOX_RETURN(FALSE);
   }

   mMSBF_GET32_O(pu8xContent,offset,pstxDldCustom->ui32SectTimeout);
   mMSBF_GET08_O(pu8xContent,offset,pstxDldCustom->ui8CarouMaxLoop);

   mTBOX_TRACE((kTBOX_NIV_2,"  * DOWNLOADER CUSTOM (%d bytes):\n",kCFGFi_DLDC_SIZE));
   mTBOX_TRACE((kTBOX_NIV_2,"  - Sections timeout....%u ms\n",pstxDldCustom->ui32SectTimeout));
   mTBOX_TRACE((kTBOX_NIV_2,"  - Carousel Max Loop...%u\n",pstxDldCustom->ui8CarouMaxLoop));

   mTBOX_RETURN(TRUE);
}

/**
 *  @brief
 *    Extract the Flash Mapping section data.
 *
 *    Syntax                             No. of bits
 *    flash_description {
 *       flash_base_address                       32
 *       ics_count                                 8
 *       for (i = 0; i < ics_count; i++) {
 *          ics_id                                 8
 *          ics_class                              8
 *          ics_address                           32
 *          ics_size                              32
 *       }
 *    }
 *
 *  @param[in/out] pstxFlash
 *              Flash Mapping extracted
 *
 *  @param[in] pu8xContent
 *              Pointer on begining of config file
 *
 *  @param[in] i32xSize
 *              Size of config file in bytes
 *
 *  @param[in] i32xOffset
 *              Offset of begining Product IDs section
 *              in content data.
 *
 *  @retval  TRUE
 *              Successful
 *
 *  @retval  FALSE
 *              Extraction failed
 *
 *  @remarks
 *    -# INVARIANT: The pointers on section pstxFlash and
 *                  file content pu8xContent are not NULL.
 *
*/
LOCAL bool CFGFi_GetFlashMapping
(
   tCFGF_FlashMapping *pstxFlash,
   uint8_t            *pu8xContent,
   int32_t             i32xSize,
   int32_t             i32xOffset
)
{
   int32_t    offset = i32xOffset;
   TKEL_msize size;
   TKEL_err   tkerr;
   uint8_t    i;


   mTBOX_FCT_ENTER(CFGFi_GetFlashMapping);

   if ((offset + kCFGFi_FLASH_SIZE_FIXE) > i32xSize)
   {
      mTBOX_TRACE((kTBOX_NIV_WARNING,
                   "%s ERROR: Flash Mapping section on %d bytes not found "
                   "at offset %d in the %d bytes of config file\n",
                   TBOX_FunctionName,kCFGFi_FLASH_SIZE_FIXE,offset,i32xSize));
      mTBOX_RETURN(FALSE);
   }

   /* Clean flash mapping */
   if (pstxFlash->pstICSs != NULL) TKEL_Free(pstxFlash->pstICSs);
   pstxFlash->pstICSs     = NULL;
   pstxFlash->ui8IcsCount = 0;

   /* Extraction */
   mMSBF_GET32_O(pu8xContent,offset,pstxFlash->ui32BaseAddress);
   mMSBF_GET08_O(pu8xContent,offset,pstxFlash->ui8IcsCount);

   mTBOX_TRACE((kTBOX_NIV_2,"  * FLASH MAPPING (%d bytes):\n",
                            kCFGFi_FLASH_SIZE_FIXE+pstxFlash->ui8IcsCount*kCFGFi_FLASH_ICS_SIZE));
   mTBOX_TRACE((kTBOX_NIV_2,"  - Address base........0x%08X\n",pstxFlash->ui32BaseAddress));
   mTBOX_TRACE((kTBOX_NIV_2,"  - ICS number..........%u\n",pstxFlash->ui8IcsCount));

   /* Allocate the ICS table */
   size = (TKEL_msize)pstxFlash->ui8IcsCount * sizeof(tCFGF_FlashICS);
   tkerr = TKEL_Malloc(size,(void**)&pstxFlash->pstICSs);
   if (pstxFlash->pstICSs == NULL)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,
                   "%s ERROR: Allocation %d bytes to store the Flash "
                   "ICS table failed (tkel error %d)\n",
                   TBOX_FunctionName,size,tkerr));
      pstxFlash->ui8IcsCount = 0;
      mTBOX_RETURN(FALSE);
   }

   memset((void*)pstxFlash->pstICSs,0,(size_t)size);

   for (i = 0; i < pstxFlash->ui8IcsCount; i++)
   {
      if ((offset + kCFGFi_FLASH_ICS_SIZE) > i32xSize)
      {
         mTBOX_TRACE((kTBOX_NIV_WARNING,
                      "%s ERROR: Flash ICS %u/%u on %d bytes not found "
                      "at offset %d in the %d bytes of config file\n",
                      TBOX_FunctionName,i+1,pstxFlash->ui8IcsCount,
                      kCFGFi_FLASH_ICS_SIZE,offset,i32xSize));
         mTBOX_RETURN(FALSE);
      }

      mMSBF_GET08_O(pu8xContent,offset,pstxFlash->pstICSs[i].ui8IcsId);
      mMSBF_GET08_O(pu8xContent,offset,pstxFlash->pstICSs[i].ui8IcsClass);
      mMSBF_GET32_O(pu8xContent,offset,pstxFlash->pstICSs[i].ui32IcsAddress);
      mMSBF_GET32_O(pu8xContent,offset,pstxFlash->pstICSs[i].ui32IcsSize);
      mTBOX_TRACE((kTBOX_NIV_2,"  - ICS %u (%d bytes):\n",
                               i,kCFGFi_FLASH_ICS_SIZE));
      switch (pstxFlash->pstICSs[i].ui8IcsId)
      {
      case kNVM_IBTL_ICS_MODID_FACTORY_DOWNLOADER:
         mTBOX_TRACE((kTBOX_NIV_2,"    - Id................%#x = Factory Downloader\n",
                                  pstxFlash->pstICSs[i].ui8IcsId));
         break;
      case kNVM_IBTL_ICS_MODID_FACTORY_CONFIG_FILE:
         mTBOX_TRACE((kTBOX_NIV_2,"    - Id................%#x = Factory Config File\n",
                                  pstxFlash->pstICSs[i].ui8IcsId));
         break;
      case kNVM_IBTL_ICS_MODID_DOWNLOADER:
         mTBOX_TRACE((kTBOX_NIV_2,"    - Id................%#x = Downloader\n",
                                  pstxFlash->pstICSs[i].ui8IcsId));
         break;
      case kNVM_IBTL_ICS_MODID_EMS_CONFIG_FILE:
         mTBOX_TRACE((kTBOX_NIV_2,"    - Id................%#x = Config File\n",
                                  pstxFlash->pstICSs[i].ui8IcsId));
         break;
      case kNVM_IBTL_ICS_MODID_EMS_REDUNDANT_CONFIG_FILE:
         mTBOX_TRACE((kTBOX_NIV_2,"    - Id................%#x = Redundant Config File\n",
                                  pstxFlash->pstICSs[i].ui8IcsId));
         break;
      case kNVM_IBTL_ICS_MODID_EXE_APPLICATION_USER_0:
         mTBOX_TRACE((kTBOX_NIV_2,"    - Id................%#x = Application User 0\n",
                                  pstxFlash->pstICSs[i].ui8IcsId));
         break;
      case kNVM_IBTL_ICS_MODID_EXE_APPLICATION_USER_1:
         mTBOX_TRACE((kTBOX_NIV_2,"    - Id................%#x = Application User 1\n",
                                  pstxFlash->pstICSs[i].ui8IcsId));
         break;
      case kNVM_IBTL_ICS_MODID_EXE_APPLICATION_USER_2:
         mTBOX_TRACE((kTBOX_NIV_2,"    - Id................%#x = Application User 2\n",
                                  pstxFlash->pstICSs[i].ui8IcsId));
         break;
      case kNVM_IBTL_ICS_MODID_EXE_APPLICATION_ENG_0:
         mTBOX_TRACE((kTBOX_NIV_2,"    - Id................%#x = Application Engineering 0\n",
                                  pstxFlash->pstICSs[i].ui8IcsId));
         break;
      case kNVM_IBTL_ICS_MODID_EXE_APPLICATION_ENG_1:
         mTBOX_TRACE((kTBOX_NIV_2,"    - Id................%#x = Application Engineering 1\n",
                                  pstxFlash->pstICSs[i].ui8IcsId));
         break;
      case kNVM_IBTL_ICS_MODID_EXE_APPLICATION_ENG_2:
         mTBOX_TRACE((kTBOX_NIV_2,"    - Id................%#x = Application Engineering 2\n",
                                  pstxFlash->pstICSs[i].ui8IcsId));
         break;
      case kNVM_IBTL_ICS_MODID_EXE_ASSET_USER_0:
         mTBOX_TRACE((kTBOX_NIV_2,"    - Id................%#x = Application Asset 0\n",
                                  pstxFlash->pstICSs[i].ui8IcsId));
         break;
      case kNVM_IBTL_ICS_MODID_EXE_ASSET_USER_1:
         mTBOX_TRACE((kTBOX_NIV_2,"    - Id................%#x = Application Asset 1\n",
                                  pstxFlash->pstICSs[i].ui8IcsId));
         break;
      case kNVM_IBTL_ICS_MODID_EXE_ASSET_USER_2:
         mTBOX_TRACE((kTBOX_NIV_2,"    - Id................%#x = Application Asset 2\n",
                                  pstxFlash->pstICSs[i].ui8IcsId));
         break;
      case kNVM_IBTL_ICS_MODID_SSA_DA2:
         mTBOX_TRACE((kTBOX_NIV_2,"    - Id................%#x = SSA DA2\n",
                                  pstxFlash->pstICSs[i].ui8IcsId));
         break;
      default:
         mTBOX_TRACE((kTBOX_NIV_2,"    - Id................%#x UNKOWN\n",
                                  pstxFlash->pstICSs[i].ui8IcsId));
      }
      switch (pstxFlash->pstICSs[i].ui8IcsClass)
      {
      case kNVM_IBTL_CLASS_ID_NONE:
         mTBOX_TRACE((kTBOX_NIV_2,"    - Class.............%#x = None\n",
                                  pstxFlash->pstICSs[i].ui8IcsClass));
         break;
      case kNVM_IBTL_CLASS_ID_SSA_DA2:
         mTBOX_TRACE((kTBOX_NIV_2,"    - Class.............%#x = SSA DA2\n",
                                  pstxFlash->pstICSs[i].ui8IcsClass));
         break;
      case kNVM_IBTL_CLASS_ID_CONFIG_FILE:
         mTBOX_TRACE((kTBOX_NIV_2,"    - Class.............%#x = Configuration\n",
                                  pstxFlash->pstICSs[i].ui8IcsClass));
         break;
      case kNVM_IBTL_CLASS_ID_ENGINEERING:
         mTBOX_TRACE((kTBOX_NIV_2,"    - Class.............%#x = Engineering\n",
                                  pstxFlash->pstICSs[i].ui8IcsClass));
         break;
      case kNVM_IBTL_CLASS_ID_DOWNLOADER:
         mTBOX_TRACE((kTBOX_NIV_2,"    - Class.............%#x = Downloader\n",
                                  pstxFlash->pstICSs[i].ui8IcsClass));
         break;
      case kNVM_IBTL_CLASS_ID_USER:
         mTBOX_TRACE((kTBOX_NIV_2,"    - Class.............%#x = User\n",
                                  pstxFlash->pstICSs[i].ui8IcsClass));
         break;
      case kNVM_IBTL_CLASS_ID_ASSET:
         mTBOX_TRACE((kTBOX_NIV_2,"    - Class.............%#x = Asset\n",
                                  pstxFlash->pstICSs[i].ui8IcsClass));
         break;
      default:
         mTBOX_TRACE((kTBOX_NIV_2,"    - Class.............%#x UNKOWN\n",
                                  pstxFlash->pstICSs[i].ui8IcsClass));
      }
      mTBOX_TRACE((kTBOX_NIV_2,"    - Address ..........0x%08X\n",pstxFlash->pstICSs[i].ui32IcsAddress));
      mTBOX_TRACE((kTBOX_NIV_2,"    - Size..............0x%08X (=%u)\n",pstxFlash->pstICSs[i].ui32IcsSize,
                                                                        pstxFlash->pstICSs[i].ui32IcsSize));
   }

   mTBOX_RETURN(TRUE);
}

/**
 *  @brief
 *    Extract the OSD Resources section data.
 *
 *    Syntax                             No. of bits
 *       osd_resources {
 *          for (i = 0; i < 256; i++) {
 *             alfa                                8
 *             red                                 8
 *             green                               8
 *             blue                                8
 *          }
 *          for (i = 0; i < 8; i++) {
 *             palette_entry                       8
 *          }
 *          picture_count                          8
 *          for (i = 0; i < picture_count; i++) {
 *             picture_width                      16
 *             picture_height                     16
 *             picture_type                        8
 *             picture_bmprl_size                 16
 *             for (j = 0; j < picture_bmprl_size; j++) {
 *                picture_bmprl_byte               8
 *             }
 *          }
 *       }
 *    }
 *
 *  @param[in/out] pstxOSD
 *              OSD Resources extracted
 *
 *  @param[in] pu8xContent
 *              Pointer on begining of config file
 *
 *  @param[in] i32xSize
 *              Size of config file in bytes
 *
 *  @param[in] i32xOffset
 *              Offset of begining Product IDs section
 *              in content data.
 *
 *  @retval  TRUE
 *              Successful
 *
 *  @retval  FALSE
 *              Extraction failed
 *
 *  @remarks
 *    -# INVARIANT: The pointers on section pstxOSD and
 *                  file content pu8xContent are not NULL.
 *
*/
LOCAL bool CFGFi_GetOSD
(
   tCFGF_OSD *pstxOSD,
   uint8_t   *pu8xContent,
   int32_t    i32xSize,
   int32_t    i32xOffset
)
{
   int32_t    offset = i32xOffset;
   TKEL_msize size;
   TKEL_err   tkerr;
   uint8_t    i;
   uint16_t   n;

   mTBOX_FCT_ENTER(CFGFi_GetOSD);

   mTBOX_TRACE((kTBOX_NIV_2,"  * OSD RESOURCES:\n"));

   /* Clean previous OSD */
   CFGFi_FreeOSD(pstxOSD);

   /* Get Palette Colors */
   pstxOSD->ui16ColorCount = kCFGFi_OSD_PALCOL_TABLE_NUMELT;
   mTBOX_TRACE((kTBOX_NIV_2,"  - Pal Colors Count....%u\n",pstxOSD->ui16ColorCount));

   /* Allocation */
   size = (TKEL_msize)pstxOSD->ui16ColorCount * sizeof(tCFGF_PalColor);
   tkerr = TKEL_Malloc(size,(void**)&pstxOSD->pstPaletteColor);
   if (pstxOSD->pstPaletteColor == NULL)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,
                   "%s ERROR: Allocation %d bytes to store the %d "
                   "colors in the OSD palette failed (tkel error %d)\n",
                   TBOX_FunctionName,size,
                   pstxOSD->ui16ColorCount,tkerr));
      pstxOSD->ui16ColorCount = 0;
      mTBOX_RETURN(FALSE);
   }
   memset((void*)pstxOSD->pstPaletteColor,0,(size_t)size);

   /* Extraction Palette */
   for (n = 0; n < pstxOSD->ui16ColorCount; n++)
   {
      if ((offset + kCFGFi_OSD_PALCOL_SIZE) > i32xSize)
      {
         mTBOX_TRACE((kTBOX_NIV_WARNING,
                      "%s ERROR: OSD Color %u/%u on %d bytes not found "
                      "at offset %d in the %d bytes of config file\n",
                      TBOX_FunctionName,n+1,pstxOSD->ui16ColorCount,
                      kCFGFi_OSD_PALCOL_SIZE,offset,i32xSize));
         mTBOX_RETURN(FALSE);
      }

      mMSBF_GET08_O(pu8xContent,offset,pstxOSD->pstPaletteColor[n].ui8Alpha);
      mMSBF_GET08_O(pu8xContent,offset,pstxOSD->pstPaletteColor[n].ui8Red);
      mMSBF_GET08_O(pu8xContent,offset,pstxOSD->pstPaletteColor[n].ui8Green);
      mMSBF_GET08_O(pu8xContent,offset,pstxOSD->pstPaletteColor[n].ui8Blue);
      mTBOX_TRACE((kTBOX_NIV_4,
                   "    - Colors %03u.......a=0x%02x r=0x%02x g=0x%02x b=0x%02x\n",
                   n,pstxOSD->pstPaletteColor[n].ui8Alpha,
                   pstxOSD->pstPaletteColor[n].ui8Red,
                   pstxOSD->pstPaletteColor[n].ui8Green,
                   pstxOSD->pstPaletteColor[n].ui8Blue));
   }

   /* Get Color Scheme (palette entry) */
   pstxOSD->ui8EntryCount = kCFGFi_OSD_PALENT_TABLE_NUMELT;
   mTBOX_TRACE((kTBOX_NIV_2,"  - Palette Entry Count.%u\n",pstxOSD->ui8EntryCount));

   /* Allocation */
   size = (TKEL_msize)pstxOSD->ui8EntryCount * sizeof(uint8_t);
   tkerr = TKEL_Malloc(size,(void**)&pstxOSD->pui8PalEntry);
   if (pstxOSD->pui8PalEntry == NULL)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,
                   "%s ERROR: Allocation %d bytes to store the %d "
                   "entry in the OSD scheme failed (tkel error %d)\n",
                   TBOX_FunctionName,size,
                   pstxOSD->ui8EntryCount,tkerr));
      pstxOSD->ui8EntryCount = 0;
      mTBOX_RETURN(FALSE);
   }
   memset((void*)pstxOSD->pui8PalEntry,0,(size_t)size);

   /* Extraction Entrys */
   for (i = 0; i < pstxOSD->ui8EntryCount; i++)
   {
      if ((offset + kCFGFi_OSD_PALENT_ENTRY_SIZE) > i32xSize)
      {
         mTBOX_TRACE((kTBOX_NIV_WARNING,
                      "%s ERROR: OSD Entry %u/%u on %d bytes not found "
                      "at offset %d in the %d bytes of config file\n",
                      TBOX_FunctionName,i+1,pstxOSD->ui8EntryCount,
                      kCFGFi_OSD_PALENT_ENTRY_SIZE,offset,i32xSize));
         mTBOX_RETURN(FALSE);
      }

      mMSBF_GET08_O(pu8xContent,offset,pstxOSD->pui8PalEntry[i]);
      mTBOX_TRACE((kTBOX_NIV_2,"    - Entry %u...........%u\n",
                               i,pstxOSD->pui8PalEntry[i]));
   }

   /* Get picture table */
   if ((offset + kCFGFi_OSD_PICT_COUNT_SIZE) > i32xSize)
   {
      mTBOX_TRACE((kTBOX_NIV_WARNING,
                   "%s ERROR: OSD picture count on %d bytes not found "
                   "at offset %d in the %d bytes of config file\n",
                   TBOX_FunctionName,kCFGFi_OSD_PICT_COUNT_SIZE,offset,i32xSize));
      mTBOX_RETURN(FALSE);
   }
   mMSBF_GET08_O(pu8xContent,offset,pstxOSD->ui8PictCount);
   mTBOX_TRACE((kTBOX_NIV_2,"  - Pictures Count......%u\n",pstxOSD->ui8PictCount));

   /* Allocation */
   if (pstxOSD->ui8PictCount > 0)
   {
      size = (TKEL_msize)pstxOSD->ui8PictCount * sizeof(tCFGF_Picture);
      tkerr = TKEL_Malloc(size,(void**)&pstxOSD->pstPictures);
      if (pstxOSD->pstPictures == NULL)
      {
         mTBOX_TRACE((kTBOX_NIV_CRITICAL,
                      "%s ERROR: Allocation %d bytes to store the %d "
                      "pictures in the OSD failed (tkel error %d)\n",
                      TBOX_FunctionName,size,
                      pstxOSD->ui8PictCount,tkerr));
         pstxOSD->ui8PictCount = 0;
         mTBOX_RETURN(FALSE);
      }
      memset((void*)pstxOSD->pstPictures,0,(size_t)size);
   }

   /* Extraction Pictures */
   for (i = 0; i < pstxOSD->ui8PictCount; i++)
   {
      if ((offset + kCFGFi_OSD_PICT_SIZE_FIXE) > i32xSize)
      {
         mTBOX_TRACE((kTBOX_NIV_WARNING,
                      "%s ERROR: OSD Picture %u/%u on %d bytes not found "
                      "at offset %d in the %d bytes of config file\n",
                      TBOX_FunctionName,i+1,pstxOSD->ui8PictCount,
                      kCFGFi_OSD_PICT_SIZE_FIXE,offset,i32xSize));
         mTBOX_RETURN(FALSE);
      }

      mMSBF_GET16_O(pu8xContent,offset,pstxOSD->pstPictures[i].ui16Width);
      mMSBF_GET16_O(pu8xContent,offset,pstxOSD->pstPictures[i].ui16Height);
      mMSBF_GET08_O(pu8xContent,offset,pstxOSD->pstPictures[i].ui8Type);
      mMSBF_GET16_O(pu8xContent,offset,pstxOSD->pstPictures[i].ui16Size);
      mTBOX_TRACE((kTBOX_NIV_2,"    - Picture %02u:\n",i));
      mTBOX_TRACE((kTBOX_NIV_2,"      - Width...........%u\n",
                               pstxOSD->pstPictures[i].ui16Width));
      mTBOX_TRACE((kTBOX_NIV_2,"      - Height..........%u\n",
                               pstxOSD->pstPictures[i].ui16Height));
      switch (pstxOSD->pstPictures[i].ui8Type)
      {
      case kCFGF_OSD_PICT_BMP:
         mTBOX_TRACE((kTBOX_NIV_2,"      - Type............0x%02x = BMP\n",
                                  pstxOSD->pstPictures[i].ui8Type));
         break;
      case kCFGF_OSD_PICT_BMPRL:
         mTBOX_TRACE((kTBOX_NIV_2,"      - Type............0x%02x = BMPRL\n",
                                  pstxOSD->pstPictures[i].ui8Type));
         break;
      case kCFGF_OSD_PICT_GIF:
         mTBOX_TRACE((kTBOX_NIV_2,"      - Type............0x%02x = GIF\n",
                                  pstxOSD->pstPictures[i].ui8Type));
         break;
      case kCFGF_OSD_PICT_RAWZIP:
         mTBOX_TRACE((kTBOX_NIV_2,"      - Type............0x%02x = RAWZIP\n",
                                  pstxOSD->pstPictures[i].ui8Type));
         break;
      case kCFGF_OSD_PICT_BMPZIP:
         mTBOX_TRACE((kTBOX_NIV_2,"      - Type............0x%02x = BMPZIP\n",
                                  pstxOSD->pstPictures[i].ui8Type));
         break;
      default:
         mTBOX_TRACE((kTBOX_NIV_2,"      - Type............0x%02x UNKOWN\n",
                                  pstxOSD->pstPictures[i].ui8Type));
      }
      mTBOX_TRACE((kTBOX_NIV_2,"      - Size............%u\n",
                               pstxOSD->pstPictures[i].ui16Size));

      /* Allocate and extract bit map */
      if ((offset + (int32_t)pstxOSD->pstPictures[i].ui16Size) > i32xSize)
      {
         mTBOX_TRACE((kTBOX_NIV_WARNING,
                      "%s ERROR: OSD Picture Bitmap %u/%u on %u bytes not "
                      "found at offset %d in the %d bytes of config file\n",
                      TBOX_FunctionName,i+1,pstxOSD->ui8PictCount,
                      pstxOSD->pstPictures[i].ui16Size,offset,i32xSize));
         mTBOX_RETURN(FALSE);
      }

      if (pstxOSD->pstPictures[i].ui16Size > 0)
      {
         tkerr = TKEL_Malloc((TKEL_msize)pstxOSD->pstPictures[i].ui16Size,
                             (void**)&pstxOSD->pstPictures[i].pui8Bitmap);
         if (pstxOSD->pstPictures[i].pui8Bitmap == NULL)
         {
            mTBOX_TRACE((kTBOX_NIV_CRITICAL,
                         "%s ERROR: Allocation %u bytes to store the bitmap "
                         "of pictures %d in the OSD failed (tkel error %d)\n",
                         TBOX_FunctionName,pstxOSD->pstPictures[i].ui16Size,
                         i,tkerr));
            pstxOSD->pstPictures[i].ui16Size = 0;
            mTBOX_RETURN(FALSE);
         }

         memcpy((void*)pstxOSD->pstPictures[i].pui8Bitmap,
                pu8xContent+offset,
                (size_t)pstxOSD->pstPictures[i].ui16Size);
         offset += (int32_t)pstxOSD->pstPictures[i].ui16Size;

         if (pstxOSD->pstPictures[i].ui16Size > 3)
         {
            mTBOX_TRACE((kTBOX_NIV_2,"      - Bitmap..........%02X %02X %02X %02X...\n",
                                     pstxOSD->pstPictures[i].pui8Bitmap[0],
                                     pstxOSD->pstPictures[i].pui8Bitmap[1],
                                     pstxOSD->pstPictures[i].pui8Bitmap[2],
                                     pstxOSD->pstPictures[i].pui8Bitmap[3]));
         }
      }
   }

   mTBOX_RETURN(TRUE);
}

/**
 *  @brief
 *    Extract the Downloader options section data.
 *
 *    Syntax                             No. of bits
 *    downloader_options {
 *       version                                  16
 *       reserved                                 30
 *       use_tuning_from_dialog_area               1
 *       enable_scanning                           1
 *    }
 *
 *  @param[in/out] pstxDldOptions
 *              Downloader Options extracted
 *
 *  @param[in] pu8xContent
 *              Pointer on begining of config file
 *
 *  @param[in] i32xSize
 *              Size of config file in bytes
 *
 *  @param[in] i32xOffset
 *              Offset of begining Product IDs section
 *              in content data.
 *
 *  @retval  TRUE
 *              Successful
 *
 *  @retval  FALSE
 *              Extraction failed
 *
 *  @remarks
 *    -# INVARIANT: The pointers on section pstxDldOptions and
 *                  file content pu8xContent are not NULL.
 *
*/
LOCAL bool CFGFi_GetDownldOptions
(
   tCFGF_DownldOption *pstxDldOptions,
   uint8_t *pu8xContent,
   int32_t i32xSize,
   int32_t i32xOffset
)
{
   int32_t  offset = i32xOffset;
   uint16_t version;
   uint32_t bitfield;

   mTBOX_FCT_ENTER(CFGFi_GetDownldOptions);

   if ((offset + kCFGFi_DLDO_SIZE) > i32xSize)
   {
      mTBOX_TRACE((kTBOX_NIV_WARNING,
                   "%s ERROR: Downloader Options section on %d bytes not found "
                   "at offset %d in the %d bytes of config file\n",
                   TBOX_FunctionName,kCFGFi_DLDO_SIZE,offset,i32xSize));
      mTBOX_RETURN(FALSE);
   }

   mMSBF_GET16_O(pu8xContent,offset,version);
   if (version != kCFGFi_DLDO_VERSION_CURR)
   {
      mTBOX_TRACE((kTBOX_NIV_WARNING,
                   "%s ERROR: Downloader Options section bad version (%d) "
                   "(version expected: %d)\n",
                   TBOX_FunctionName,version,kCFGFi_DLDO_VERSION_CURR));
      mTBOX_RETURN(FALSE);
   }

   mMSBF_GET32_O(pu8xContent,offset,bitfield);
   pstxDldOptions->ui8UseTuningFromDialogArea = ((bitfield & kCFGFi_DLDO_ENABLE_SCANNING_BIT) == kCFGFi_DLDO_ENABLE_SCANNING_BIT);
   pstxDldOptions->ui8EnableScanning          = ((bitfield & kCFGFi_DLDO_USE_TUNING_DLG_AREA_BIT) == kCFGFi_DLDO_USE_TUNING_DLG_AREA_BIT);

   mTBOX_TRACE((kTBOX_NIV_2,"  * DOWNLOADER OPTIONS (%d bytes):\n",kCFGFi_DLDO_SIZE));
   mTBOX_TRACE((kTBOX_NIV_2,"  - use_tuning_from_dialog_area....%u\n",pstxDldOptions->ui8UseTuningFromDialogArea));
   mTBOX_TRACE((kTBOX_NIV_2,"  - enable_scanning................%u\n",pstxDldOptions->ui8EnableScanning));

   mTBOX_RETURN(TRUE);
}

/**
 *  @brief
 *    Extract the Scanning section data.
 *
 *    Syntax                             No. of bits
 *       scanning_descriptor {
 *          version                                8
 *          network_type                           8
 *          if (network_type == 16 || network_type == 17) {
 *             terrestrial_scanning_descriptor()
 *          }
 *          if (network_type == 32) {
 *             cable_scanning_descriptor()
 *          }
 *          if (network_type == 48 || network_type == 49) {
 *             satellite_scanning_descriptor()
 *          }
 *       }
 *
 *  @param[in/out] pstxCfg
 *              Scanning parameters extracted
 *
 *  @param[in] pu8xContent
 *              Pointer on beginning of config file
 *
 *  @param[in] i32xSize
 *              Size of config file in bytes
 *
 *  @param[in] i32xOffset
 *              Offset of begining Product IDs section
 *              in content data.
 *
 *  @retval  TRUE
 *              Successful
 *
 *  @retval  FALSE
 *              Extraction failed
 *
 *  @remarks
 *    -# INVARIANT: The pointers on section pstxCfg and
 *                  file content pu8xContent are not NULL.
 *
*/
LOCAL bool CFGFi_GetScanning
(
   tCFGF_Configuration *pstxCfg,
   uint8_t *pu8xContent,
   int32_t i32xSize,
   int32_t i32xOffset
)
{
   bool     ok     = TRUE;
   int32_t  offset = i32xOffset;
   uint8_t  version;
   uint8_t  network_type;

   mTBOX_FCT_ENTER(CFGFi_GetScanning);

   if ((offset + kCFGFi_SCANNING_HEADER_SIZE) > i32xSize)
   {
      mTBOX_TRACE((kTBOX_NIV_WARNING,
                   "%s ERROR: Scanning section header on %d bytes not found "
                   "at offset %d in the %d bytes of config file\n",
                   TBOX_FunctionName,kCFGFi_SCANNING_HEADER_SIZE,offset,i32xSize));
      mTBOX_RETURN(FALSE);
   }

   mMSBF_GET08_O(pu8xContent,offset,version);
   if (version != kCFGFi_SCANNING_VERSION_CURR)
   {
      mTBOX_TRACE((kTBOX_NIV_WARNING,
                   "%s ERROR: Scanning section bad version (%d) "
                   "(version expected: %d)\n",
                   TBOX_FunctionName,version,kCFGFi_SCANNING_VERSION_CURR));
      mTBOX_RETURN(FALSE);
   }
   
   mMSBF_GET08_O(pu8xContent,offset,network_type);

   switch (network_type)
   {
      case kCFGFi_LOCT_TYPE_DVBT:
      case kCFGFi_LOCT_TYPE_DVBT2:
         ok = CFGFi_GetScanningDVBT(pstxCfg,pu8xContent,i32xSize,offset);
         break;

      case kCFGFi_LOCT_TYPE_DVBC:
      case kCFGFi_LOCT_TYPE_DVBS:
      case kCFGFi_LOCT_TYPE_DVBS2:
      default:
         mTBOX_TRACE((kTBOX_NIV_WARNING,
                      "%s ERROR: Network type %#x of scanning "
                      "unsupported (compute state at offset %d)\n",
                      TBOX_FunctionName,network_type,offset));
         ok = FALSE;
   }

   if (!ok)
   {
      mTBOX_RETURN(FALSE);
   }

   mTBOX_RETURN(TRUE);
}

/**
 *  @brief
 *    Extract the terrestrial scanning section data.
 *
 *    Syntax                             No. of bits
 *       terrestrial_scanning_descriptor {
 *          version                                8
 *          nb_freq_location                       8
 *          for (i = 0; i < nb_freq_location; i++) {
 *             frequency                          32
 *             bandwidth                          32
 *             channel_number                     32
 *          }
 *          nb_band_location                       8
 *          for (i = 0; i < nb_freq_location; i++) {
 *             first_frequency                    32
 *             last_frequency                     32
 *             common_bandwidth                   32
 *             first_channel_number               32
 *          }
 *       }
 *
 *
 *  @param[in/out] pstxCfg
 *              Configuration with DVB-T scanning elements 
 *              extracted
 *
 *  @param[in] pu8xContent
 *              Pointer on begining of config file
 *
 *  @param[in] i32xSize
 *              Size of config file in bytes
 *
 *  @param[in] i32xOffset
 *              Offset of begining Serial section
 *              in content data.
 *
 *  @retval  TRUE
 *              Successful
 *
 *  @retval  FALSE
 *              Extraction failed
 *
 *  @remarks
 *    -# INVARIANT: The pointer on file content pu8xContent 
 *                  is not NULL.
 *
 *
*/
LOCAL bool CFGFi_GetScanningDVBT
(
   tCFGF_Configuration *pstxCfg,
   uint8_t             *pu8xContent,
   int32_t              i32xSize,
   int32_t              i32xOffset
)
{
   int32_t    offset = i32xOffset;
   uint8_t    version;
   uint8_t    i,count;
#if defined (MEDIUM_TER)
   TKEL_msize size;
   TKEL_err   tkerr;
#endif


   mTBOX_FCT_ENTER(CFGFi_GetScanningDVBT);

   if ((offset + kCFGFi_SCANNING_DVBT_VERSION_SIZE + kCFGFi_SCANNING_DVBT_NB_FREQ_SIZE) > i32xSize)
   {
      mTBOX_TRACE((kTBOX_NIV_WARNING,
                   "%s ERROR: terrestrial scanning section error "
                   "at offset %d in the %d bytes of config file\n",
                   TBOX_FunctionName,offset,i32xSize));
      mTBOX_RETURN(FALSE);
   }

   /* get version */
   mMSBF_GET08_O(pu8xContent,offset,version);
   if (version != kCFGFi_SCANNING_DVBT_VERSION_CURR)
   {
      mTBOX_TRACE((kTBOX_NIV_WARNING,
                   "%s ERROR: terrestrial scanning section bad version (%d) "
                   "(version expected: %d)\n",
                   TBOX_FunctionName,version,kCFGFi_SCANNING_DVBT_VERSION_CURR));
      mTBOX_RETURN(FALSE);
   }

   /* get nb_freq_location */
   mMSBF_GET08_O(pu8xContent,offset,count);
   
#ifdef MEDIUM_TER
   if (pstxCfg->pstTableFreqDVBT != NULL) 
   {
      TKEL_Free(pstxCfg->pstTableFreqDVBT);
   }
   pstxCfg->ui8CountFreqDVBT = 0;

   /* Allocation */
   if (count > 0)
   {
      pstxCfg->ui8CountFreqDVBT = count;
      size = (TKEL_msize)pstxCfg->ui8CountFreqDVBT * sizeof(tCFGF_FreqLocDVBT);
      tkerr = TKEL_Malloc(size,(void**)&pstxCfg->pstTableFreqDVBT);
      if (pstxCfg->pstTableFreqDVBT == NULL)
      {
         mTBOX_TRACE((kTBOX_NIV_CRITICAL,
                      "%s ERROR: Allocation %d bytes to store the %d "
                      "freq locations failed (tkel error %d)\n",
                      TBOX_FunctionName,size,
                      pstxCfg->ui8CountFreqDVBT,tkerr));
         pstxCfg->ui8CountFreqDVBT = 0;
         mTBOX_RETURN(FALSE);
      }
      memset((void*)pstxCfg->pstTableFreqDVBT,0,(size_t)size);
   }
#endif

   /* parse all freq_location */
   for (i = 0; i < count; i++)
   {
      if ((offset + kCFGFi_SCANNING_DVBT_FREQ_LOC_SIZE) > i32xSize)
      {
         mTBOX_TRACE((kTBOX_NIV_WARNING,
                      "%s ERROR: Frequency location %u/%u error "
                      "at offset %d in the %d bytes of config file\n",
                      TBOX_FunctionName,i+1,count,offset,i32xSize));
         mTBOX_RETURN(FALSE);
      }
#ifdef MEDIUM_TER
      mMSBF_GET32_O(pu8xContent,offset,pstxCfg->pstTableFreqDVBT[i].ui32Frequency);
      mMSBF_GET32_O(pu8xContent,offset,pstxCfg->pstTableFreqDVBT[i].ui32Bandwidth);
      mMSBF_GET32_O(pu8xContent,offset,pstxCfg->pstTableFreqDVBT[i].ui32ChannelNumber);
#else
      offset += kCFGFi_SCANNING_DVBT_FREQ_LOC_SIZE;
#endif
   }

   if ((offset + kCFGFi_SCANNING_DVBT_NB_BAND_SIZE) > i32xSize)
   {
      mTBOX_TRACE((kTBOX_NIV_WARNING,
                   "%s ERROR: terrestrial scanning section error "
                   "at offset %d in the %d bytes of config file\n",
                   TBOX_FunctionName,offset,i32xSize));
      mTBOX_RETURN(FALSE);
   }

   /* get nb_band_location */
   mMSBF_GET08_O(pu8xContent,offset,count);

#ifdef MEDIUM_TER
   if (pstxCfg->pstTableBandDVBT != NULL) 
   {
      TKEL_Free(pstxCfg->pstTableBandDVBT);
   }
   pstxCfg->ui8CountBandDVBT = 0;

   /* Allocation */
   if (count > 0)
   {
      pstxCfg->ui8CountBandDVBT = count;
      size = (TKEL_msize)pstxCfg->ui8CountBandDVBT * sizeof(tCFGF_BandLocDVBT);
      tkerr = TKEL_Malloc(size,(void**)&pstxCfg->pstTableBandDVBT);
      if (pstxCfg->pstTableBandDVBT == NULL)
      {
         mTBOX_TRACE((kTBOX_NIV_CRITICAL,
                      "%s ERROR: Allocation %d bytes to store the %d "
                      "freq locations failed (tkel error %d)\n",
                      TBOX_FunctionName,size,
                      pstxCfg->ui8CountBandDVBT,tkerr));
         pstxCfg->ui8CountBandDVBT = 0;
         mTBOX_RETURN(FALSE);
      }
      memset((void*)pstxCfg->pstTableBandDVBT,0,(size_t)size);
   }
#endif

   /* parse all band_location */
   for (i = 0; i < count; i++)
   {
      if ((offset + kCFGFi_SCANNING_DVBT_BAND_LOC_SIZE) > i32xSize)
      {
         mTBOX_TRACE((kTBOX_NIV_WARNING,
                      "%s ERROR: Band location %u/%u error "
                      "at offset %d in the %d bytes of config file\n",
                      TBOX_FunctionName,i+1,count,offset,i32xSize));
         mTBOX_RETURN(FALSE);
      }

#ifdef MEDIUM_TER
      mMSBF_GET32_O(pu8xContent,offset,pstxCfg->pstTableBandDVBT[i].ui32FirstFreq);
      mMSBF_GET32_O(pu8xContent,offset,pstxCfg->pstTableBandDVBT[i].ui32LastFreq);
      mMSBF_GET32_O(pu8xContent,offset,pstxCfg->pstTableBandDVBT[i].ui32CommonBandwidth);
      mMSBF_GET32_O(pu8xContent,offset,pstxCfg->pstTableBandDVBT[i].ui32FirstChNumber);
#else
      offset += kCFGFi_SCANNING_DVBT_BAND_LOC_SIZE;
#endif
   }

#ifdef MEDIUM_TER
   mTBOX_TRACE((kTBOX_NIV_2,"  * SCANNING DVB-T (%d bytes):\n",i32xSize));
   mTBOX_TRACE((kTBOX_NIV_2,"  - nb_freq_location....%u\n",pstxCfg->ui8CountFreqDVBT));
   for (i = 0; i < pstxCfg->ui8CountFreqDVBT; i++)
   {
      mTBOX_TRACE((kTBOX_NIV_2,"   - frequency..........%u\n",pstxCfg->pstTableFreqDVBT[i].ui32Frequency));
      mTBOX_TRACE((kTBOX_NIV_2,"     bandwidth..........%u\n",pstxCfg->pstTableFreqDVBT[i].ui32Bandwidth));
      mTBOX_TRACE((kTBOX_NIV_2,"     channel number.....%u\n",pstxCfg->pstTableFreqDVBT[i].ui32ChannelNumber));
   }
   mTBOX_TRACE((kTBOX_NIV_2,"  - nb_band_location....%u\n",pstxCfg->ui8CountBandDVBT));
   for (i = 0; i < pstxCfg->ui8CountBandDVBT; i++)
   {
      mTBOX_TRACE((kTBOX_NIV_2,"   - first frequency....%u\n",pstxCfg->pstTableBandDVBT[i].ui32FirstFreq));
      mTBOX_TRACE((kTBOX_NIV_2,"     last frequency.....%u\n",pstxCfg->pstTableBandDVBT[i].ui32LastFreq));
      mTBOX_TRACE((kTBOX_NIV_2,"     common bandwidth...%u\n",pstxCfg->pstTableBandDVBT[i].ui32CommonBandwidth));
      mTBOX_TRACE((kTBOX_NIV_2,"     first channel nb...%u\n",pstxCfg->pstTableBandDVBT[i].ui32FirstChNumber));
   }
#endif

   mTBOX_RETURN(TRUE);
}

/**
 *  @brief
 *    Create a new configuration
 *
 *  @retval  Not NULL
 *              Pointer on configuration created.
 *
 *  @retval  NULL
 *              Allocation failure
 *
 *  @remarks
 *    -# The pointer returned must be released by next function 
 *       configuration 'CFGFi_FreeConfig'.
 *
*/
LOCAL tCFGF_Configuration *CFGFi_NewConfig
(
   void
)
{
   TKEL_err             tkerr;
   tCFGF_Configuration *ptr  = NULL;
   TKEL_msize           size = (TKEL_msize)sizeof(tCFGF_Configuration);

   tkerr = TKEL_Malloc(size,(void**)&ptr);
   if (ptr == NULL)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,
                   "CFGFi_NewConfig ERROR: Allocation %u bytes to create the "
                   "configuration failed (tkel error %d)\n",
                   size,tkerr));
      return(NULL);
   }
   
   memset(ptr,0,(size_t)size);

   return(ptr);
}

/**
 *  @brief
 *    Free a configuration structure allocated and 
 *    returned by CFGFi_NewConfig.
 *
 *  @param[in] pstxCfg
 *              Pointer on configuration to free
*/
LOCAL void CFGFi_FreeConfig
(
   tCFGF_Configuration *pstxCfg
)
{
   if (pstxCfg == NULL) return;

   if (pstxCfg->stProductIDs.pcBrandName != NULL) TKEL_Free(pstxCfg->stProductIDs.pcBrandName);
   if (pstxCfg->pstTableSerial != NULL) TKEL_Free(pstxCfg->pstTableSerial);
   CFGFi_FreeTableUSB(pstxCfg);
   CFGFi_FreeTableURL(pstxCfg);
#if defined (MEDIUM_TER)
   if (pstxCfg->pstTableDVBT != NULL) TKEL_Free(pstxCfg->pstTableDVBT);
   if (pstxCfg->pstTableDVBT2 != NULL) TKEL_Free(pstxCfg->pstTableDVBT2);
   if (pstxCfg->pstTableFreqDVBT != NULL) TKEL_Free(pstxCfg->pstTableFreqDVBT);
   if (pstxCfg->pstTableBandDVBT != NULL) TKEL_Free(pstxCfg->pstTableBandDVBT);
#endif
#if defined (MEDIUM_CAB)
   if (pstxCfg->pstTableDVBC != NULL) TKEL_Free(pstxCfg->pstTableDVBC);
#endif
#if defined (MEDIUM_SAT)
   if (pstxCfg->pstTableDVBS != NULL) TKEL_Free(pstxCfg->pstTableDVBS);
   if (pstxCfg->pstTableDVBS2 != NULL) TKEL_Free(pstxCfg->pstTableDVBS2);
#endif
   if (pstxCfg->stFlashMapping.pstICSs != NULL) TKEL_Free(pstxCfg->stFlashMapping.pstICSs);
   CFGFi_FreeOSD(&pstxCfg->stOSD);
   TKEL_Free(pstxCfg);
}

LOCAL void CFGFi_FreeTableUSB
(
   tCFGF_Configuration *pstxCfg
)
{
   uint8_t i;

   if (pstxCfg->pstTableUSB != NULL) 
   {
      for (i = 0; i < pstxCfg->ui8CountUSB; i++)
      {
         if (pstxCfg->pstTableUSB[i].pcPathName != NULL)
            TKEL_Free(pstxCfg->pstTableUSB[i].pcPathName);
      }

      TKEL_Free(pstxCfg->pstTableUSB);
      pstxCfg->pstTableUSB = NULL;
   }

   pstxCfg->ui8CountUSB = 0;
}

LOCAL void CFGFi_FreeTableURL
(
   tCFGF_Configuration *pstxCfg
)
{
   uint8_t i;

   if (pstxCfg->pstTableURL != NULL) 
   {
      for (i = 0; i < pstxCfg->ui8CountURL; i++)
      {
         if (pstxCfg->pstTableURL[i].pcURL != NULL)
            TKEL_Free(pstxCfg->pstTableURL[i].pcURL);
      }

      TKEL_Free(pstxCfg->pstTableURL);
      pstxCfg->pstTableURL = NULL;
   }

   pstxCfg->ui8CountURL = 0;
}

LOCAL void CFGFi_FreeOSD
(
   tCFGF_OSD *pstxOSD
)
{
   uint8_t i;

   if (pstxOSD->pstPaletteColor != NULL) 
   {
      TKEL_Free(pstxOSD->pstPaletteColor);
      pstxOSD->pstPaletteColor = NULL;
   }
   pstxOSD->ui16ColorCount = 0;

   if (pstxOSD->pui8PalEntry != NULL) 
   {
      TKEL_Free(pstxOSD->pui8PalEntry);
      pstxOSD->pui8PalEntry = NULL;
   }
   pstxOSD->ui8EntryCount = 0;

   if (pstxOSD->pstPictures != NULL) 
   {
      for (i = 0; i < pstxOSD->ui8PictCount; i++)
      {
         if (pstxOSD->pstPictures[i].pui8Bitmap != NULL)
            TKEL_Free(pstxOSD->pstPictures[i].pui8Bitmap);
      }

      TKEL_Free(pstxOSD->pstPictures);
      pstxOSD->pstPictures = NULL;
   }

   pstxOSD->ui8PictCount = 0;
}

#ifndef kCFGFi_CONFIG_FILE_IN_RAM
/**
 *  @brief
 *    Read a multi block flash area
 *
 *  @param[in] Address
 *             Flash address
 *  @param[in] Buffer
 *             buffer to fill
 *  @param[in] NumberToRead
 *             Byte amount to read
 *  @retval
 *             Byte red count
*/
LOCAL uint32_t CFGFi_MBFLA_Read( uint32_t Address, uint8_t* Buffer, uint32_t NumberToRead)
{
   uint32_t ui32FlashNbBlock=0;
   uint32_t ui32ItemSize=0;
   uint32_t ui32ItemRead=0;
   uint32_t ui32ReadCount=0;
   uint32_t ui32BlockSize;
   uint32_t ui32BlockAddress;
   uint32_t ui32BlockNumber;
   uint32_t ui32AlignSize;
   
   mTBOX_FCT_ENTER(CFGFi_MBFLA_Read);

   mTBOX_TRACE((kTBOX_NIV_1,
                "%s() Address=%#X Buffer=%#X NumberToRead=%#X\n",
                TBOX_FunctionName,Address,Buffer,NumberToRead));
   
   ui32FlashNbBlock = TDAL_FLA_GetNumberOfBlocks();               
   while (ui32ReadCount < NumberToRead)
   {
      ui32BlockNumber = TDAL_FLA_GetBlockNumber(Address + ui32ReadCount);
      if (ui32BlockNumber >= ui32FlashNbBlock)
      {
         mTBOX_TRACE((kTBOX_NIV_WARNING,"%s() bad flash address, "
                "No Blocks here %#X+%#x only %d available\n",
                TBOX_FunctionName, 
                Address , ui32ReadCount, ui32FlashNbBlock));
         mTBOX_RETURN(ui32ReadCount);
      }
      ui32BlockAddress = TDAL_FLA_GetBlockAddress(ui32BlockNumber);
      ui32BlockSize = TDAL_FLA_GetBlockSize(ui32BlockNumber);
      /* align buffer */
      ui32AlignSize = Address + ui32ReadCount - ui32BlockAddress;
      /* size from the block, 0 stands for full block */
      ui32ItemSize = (NumberToRead + ui32AlignSize - ui32ReadCount) % ui32BlockSize ;
      /* size inside the block 1..ui32BlockSize */
      ui32ItemSize = (ui32BlockSize + ui32ItemSize - ui32AlignSize) % (ui32BlockSize + 1);      
      mTBOX_TRACE((kTBOX_NIV_1,
                "%s() Reading Flash %#x bytes from %#X+%#x\n",
                TBOX_FunctionName, 
                ui32ItemSize, Address, ui32ReadCount));
      /* ACCESS  TDAL_FLA_Read */
      ui32ItemRead = TDAL_FLA_Read( Address + ui32ReadCount,
                                    &Buffer[ui32ReadCount],
                                    ui32ItemSize);
      ui32ReadCount += ui32ItemRead;
      if (ui32ItemSize != ui32ItemRead)
      {
         mTBOX_TRACE((kTBOX_NIV_WARNING,
                "%s() TDAL_FLA_Read(%#X + %#x,%#x) FAILED !=%#x\n",
                TBOX_FunctionName,
                Address,ui32ReadCount,ui32ItemSize,ui32ItemRead));
         mTBOX_RETURN(ui32ReadCount);
      }
   } /* while (ui32ReadCount < NumberToRead) */
   mTBOX_RETURN(ui32ReadCount);
}
#endif

/* config_file.c */


