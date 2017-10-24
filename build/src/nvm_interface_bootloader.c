/**************************************************************************//**
** @file nvm_interface_bootloader.c
**
** @brief
**   Implementation API to manage (read/write) the files interface  between
**   the bootloader and applications (comedia/trigger and downloader).
**
**   Using flash configuration to get the NVM areas location in flash of files
**   managed and the TDAL NVM to access this file (read/write).
**
**   The TDAL NVM is not initialized, opened, closed and terminated by this
**   module. These globales operations is performed by the application.
**
** @ingroup NVM MANAGEMENT
**
** @see FRS Bootloader version 1.1, release date 2011-06-29
**
** @see modules flash_cfg (build) and tdal_nvm (chal).
**
** @version $Rev: 10011 $
**          $URL: http://10.50.1.219/svncomedia/idecode/product/comedia/branches/snt_cos/main/build/src/nvm_interface_bootloader.c $
**          $Date: 2012-04-19 16:47:52 +0200 (jeu., 19 avr. 2012) $
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

#if defined(USE_TDAL_SSU)
#include "tdal_ssu.h"
#endif

#if defined(USE_TDAL_OTA)
#include "tdal_common.h"
#endif

/* @@@ #include "tdal_nvm.h" */
#include "flash_cfg.h"
#include "msbf_tools.h"
#include "nvm_interface_bootloader.h"
#include "version_cfg.h"
#include "config_file.h"
#include "ruxi.h"
mTBOX_SET_MODULE(eNVM_IBTL);


/******************************************************************************
* Defines
******************************************************************************/
#ifdef NEWLAND_BOOTLOADER

    #define kNVM_IBTLi_NEWLAND_MAGIC_NUMBER       0x4C4F4144
    #define kNVM_IBTLi_NEWLAND_OTA_TYPE           0x000000FF
    #define kNVM_IBTLi_NEWLAND_OTA_PID            0x1F40
    #define kNVM_IBTLi_NEWLAND_OTA_TABLE_ID       0x0000
    #define kNVM_IBTLi_NEWLAND_OTA_FREQUENCY      0x00000132
    #define kNVM_IBTLi_NEWLAND_OTA_SYMBOL_RATE    0x00001ADB
    #define kNVM_IBTLi_NEWLAND_OTA_MODULATION     0x00000002
    #define kNVM_IBTLi_NEWLAND_NEW_SOFTWARE       0x00000000
    #define kNVM_IBTLi_NEWLAND_CUR_SOFTWARE       0x01090101
    #define kNVM_IBTLi_NEWLAND_MAN_FACTURER_ID    0x0000000C
    #define kNVM_IBTLi_NEWLAND_DOWNLOAD_DATA      0x20110811
    #define kNVM_IBTLi_NEWLAND_FAILED_CNT         0x00000000
    #define kNVM_IBTLi_NEWLAND_HARDWARE_VERSION   0x00050107
    #define kNVM_IBTLi_NEWLAND_DESTROY_FLAG       0x00000000
    #define kNVM_IBTLi_NEWLAND_CRC                0xEA1BD885

#endif

#if defined(SKYWORTH_BOOTLOADER)
        #define kNVM_IBTLi_SKYWORTH_INIT_FLAG_ADDR          0xFDB09800
        #define kNVM_IBTLi_SKYWORTH_INIT_FLAG_MAGIC_NUMBER  0x5a5a5a5a
#endif
/**
 *  @brief
 * MAPPING NVM files
 *
*/
enum {
   eIBTL_NVM_DIALOG,
   eIBTL_NVM_UPD_REQ,
   eIBTL_NVM_EXE_APP_STATE,
   eIBTL_NVM_EXE_CONF,
   eIBTL_NVM_APP_VERS,
   eIBTL_NVM_STDBY
} ;

#if defined(NEWLAND_BOOTLOADER)
static struct
{
	uint32_t addressOffset;
	uint32_t size;
}NVM_IBTL_Location [] =
{
   { 0x007F0000, 0x1000},/* eIBTL_NVM_DIALOG  */
   { 0x007F0000, 0x0 },/*eIBTL_NVM_UPD_REQ */
   { 0x007F0000, 0x0 },/*eIBTL_NVM_EXE_APP_STATE */
   { 0x007F0000, 0x0 },/*eIBTL_NVM_EXE_CONF */
   { 0x007F0028, 0x2 },/*eIBTL_NVM_APP_VERS */
   { 0x007F1600, 0x0 }/*eIBTL_NVM_STDBY */
};
#elif defined(CHANGHONG_BOOTLOADER)
static struct
{
	uint32_t addressOffset;
	uint32_t size;
}NVM_IBTL_Location [] =
{
   { 0x007D0200, 0x200},/* eIBTL_NVM_DIALOG  */
   { 0x007D0000, 0x0 },/*eIBTL_NVM_UPD_REQ */
   { 0x007D0000, 0x0 },/*eIBTL_NVM_EXE_APP_STATE */
   { 0x007D0000, 0x0 },/*eIBTL_NVM_EXE_CONF */
   { 0x007D0000, 0x0 },/*eIBTL_NVM_APP_VERS */
   { 0x007D0000, 0x0 }/*eIBTL_NVM_STDBY */
};
#elif defined(SKYWORTH_BOOTLOADER)
static struct
{
    uint32_t addressOffset;
    uint32_t size;
}NVM_IBTL_Location [] =
{
   { 0x00040082, 0x200},/* eIBTL_NVM_DIALOG  */
   { 0x00040000, 0x0 },/*eIBTL_NVM_UPD_REQ */
   { 0x00040000, 0x0 },/*eIBTL_NVM_EXE_APP_STATE */
   { 0x00040000, 0x0 },/*eIBTL_NVM_EXE_CONF */
   { 0x00040000, 0x0 },/*eIBTL_NVM_APP_VERS */
   { 0x00040000, 0x0 }/*eIBTL_NVM_STDBY */
};
#elif defined(TELESYSTEM_MAP51_BOOTLOADER) || defined(KAON_MSD5C59_BOOTLOADER)
static struct
{
    uint32_t addressOffset;
    uint32_t size;
}NVM_IBTL_Location [] =
{
   { 0x007F0000, 0x200 },/* eIBTL_NVM_DIALOG  */
   { 0x007F0000, 0x0 },/*eIBTL_NVM_UPD_REQ */
   { 0x007F0000, 0x0 },/*eIBTL_NVM_EXE_APP_STATE */
   { 0x007F0000, 0x0 },/*eIBTL_NVM_EXE_CONF */
   { 0x007F0000, 0x0 },/*eIBTL_NVM_APP_VERS */
   { 0x007F0000, 0x0 }/*eIBTL_NVM_STDBY */
};
#else /* unused */
static struct
{
	uint32_t addressOffset;
	uint32_t size;
}NVM_IBTL_Location [] =
{
   { 0x007F0000, 0x0 },/* eIBTL_NVM_DIALOG  */
   { 0x007F0000, 0x0 },/*eIBTL_NVM_UPD_REQ */
   { 0x007F0000, 0x0 },/*eIBTL_NVM_EXE_APP_STATE */
   { 0x007F0000, 0x0 },/*eIBTL_NVM_EXE_CONF */
   { 0x007F0000, 0x0 },/*eIBTL_NVM_APP_VERS */
   { 0x007F0000, 0x0 }/*eIBTL_NVM_STDBY */
};
#endif


#define kNVM_IBTLi_DefaultNetworkType 0xFF


/* Number of elements in the CRC table */
#define kNVM_IBTLi_CRC_TABLE_SIZE            256


/* Initial value of FILE not opened */
#define kNVM_IBTLi_FILE_NULL  {NULL,NULL,-1}


/**
 *  @brief
 *    NVM File Update Requests format.
 *    Storing in NVM independent from alignment: msbf
 *    => use macros mMSBF_GETxx and mMSBF_SETxx
 *
 *    Syntax                    No. of bits
 *    update_request_file() {
 *       version                         32
 *       number_of_class                 32
 *       for (i = 0; i < n; i++) {
 *          class_id                     32
 *          update_reason                32
 *       }
 *       rf_mod_active_flag               1
 *       cvbs_active_flag                 1
 *       reserved                         6
 *       if (rf_mod_active_flag == 1) {
 *          rf_mod_channel                8
 *          rf_mod_standard               8
 *          rf_band                       8
 *       }
 *       else {
 *          reserved                     24
 *       }
 *       if (cvbs_active_flag == 1) {
 *          cvbs_encoding_format          8
 *       }
 *       else {
 *          reserved                      8
 *       }
 *       crc32                           32
 *    }
 *
 *    Note: The table is fixe, 1 element by class id defined
 *          In this version = 6 (constant kNVM_IBTL_CLASS_ID_NUMBER)
 *
 *    3 parts:
 *    - header on 5 bytes with number_of_class=6
 *    - number_of_class=6 requests on 8 bytes => 6x8=48 bytes
 *    - RF modulation on 5 bytes
 *    - crc on 4 bytes
 *
 *    => size = 5 + 48 + 4 + 5 = 62
*/
/* Header */
#define kNVM_IBTLi_FUR_HEAD_VERSION_SIZE       4
#define kNVM_IBTLi_FUR_HEAD_N_CLASS_SIZE       4
#define kNVM_IBTLi_FUR_HEAD_SIZE       (kNVM_IBTLi_FUR_HEAD_VERSION_SIZE+kNVM_IBTLi_FUR_HEAD_N_CLASS_SIZE)
/* Element Update request */
#define kNVM_IBTLi_FUR_UREQ_CLASSID_SIZE       4
#define kNVM_IBTLi_FUR_UREQ_UREASON_SIZE       4
#define kNVM_IBTLi_FUR_UREQ_SIZE       (kNVM_IBTLi_FUR_UREQ_CLASSID_SIZE+kNVM_IBTLi_FUR_UREQ_UREASON_SIZE)
#define kNVM_IBTLi_FUR_UREQ_NUMBER      kNVM_IBTL_CLASS_ID_NUMBER
#define kNVM_IBTLi_FUR_UREQ_TABLE_SIZE (kNVM_IBTLi_FUR_UREQ_NUMBER*kNVM_IBTLi_FUR_UREQ_SIZE)
/* RF Modulation/CVBS */
#define kNVM_IBTLi_FUR_RFMOD_FLAGS_SIZE        1
#define kNVM_IBTLi_FUR_RFMOD_CHANN_SIZE        1
#define kNVM_IBTLi_FUR_RFMOD_STD_SIZE          1
#define kNVM_IBTLi_FUR_RFMOD_BAND_SIZE         1
/* Alignement of CRC32 in update request file */
#define CVBS_ON_4_BYTES 4
#ifndef CVBS_ON_4_BYTES
#define kNVM_IBTLi_FUR_RFMOD_CVBSENC_SIZE      1
#else
#define kNVM_IBTLi_FUR_RFMOD_CVBSENC_SIZE      4
#endif
#define kNVM_IBTLi_FUR_RFMOD_SIZE      (kNVM_IBTLi_FUR_RFMOD_FLAGS_SIZE+kNVM_IBTLi_FUR_RFMOD_CHANN_SIZE+kNVM_IBTLi_FUR_RFMOD_STD_SIZE+kNVM_IBTLi_FUR_RFMOD_BAND_SIZE+kNVM_IBTLi_FUR_RFMOD_CVBSENC_SIZE)
/* Control block */
#define kNVM_IBTLi_FUR_CRC32_SIZE              4
/* Size of file */
#define kNVM_IBTLi_FUR_FILE_SIZE       (kNVM_IBTLi_FUR_HEAD_SIZE+kNVM_IBTLi_FUR_UREQ_TABLE_SIZE+kNVM_IBTLi_FUR_RFMOD_SIZE+kNVM_IBTLi_FUR_CRC32_SIZE)
/* Current file version */
#define kNVM_IBTLi_FUR_CURR_VERSION   0x00000001

/**
 *  @brief
 *    NVM File Update Requests format.
 *    Storing in NVM independent from alignment: msbf
 *    => use macros mMSBF_GETxx and mMSBF_SETxx
 *
 *    Syntax                    No. of bits
 *    SdtvDialogArea() {
			U32SyntaxVersion	32
			u32UpdateID		32
			u16PmtPid			16
			u16DsmccPid		16
			U8DLState			8
			U8ErrorCode		8
			U24reserved		8
			U8NetWorkType		8
			If (U32NetWorkType==0x30) {
				DVB_S_descriptor()	12*8
				}
			u32ModuleCRC32	32
			}
DVB_S_descriptor {
			BIS_frequency	32
			symbol_rate	32
			reserved	5
			continuous_tone_on_flag	1
			lnb_power_on_flag	1
			vertical_polarisation_flag	1
			modulation_type	8
			fecrate	8
			switch_selection	8
	}

*/
/* Header */
#define kNVM_IBTLi_FDA_HEAD_VERSION_SIZE       4
/* Element dialog area download cycle 12 Byte */
#define kNVM_IBTLi_FDA_HEAD_VERSION_SIZE       4
/* Element dialog area DVB-S location 12 byte*/
#define kNVM_IBTLi_FDA_CRC_OFFSET       (4+12+12)
/* Current file version */





#define kNVM_IBTLi_FDA_FILE_SIZE  56
#define kNVM_IBTLi_FDA_CURR_VERSION   0x00000001
#define kNVM_IBTLi_FDA_MIN_VERSION   0x00000001


/**
 *  @brief
 *    NVM File Application format.
 *    Storing in NVM independent from alignment: msbf
 *    => use macros mMSBF_GETxx and mMSBF_SETxx
 *
 *    Syntax                    No. of bits
 *    application_version_file() {
 *       version                         32
 *       number_of_ICS_module            32
 *       for (i = 0; i < n; i++) {
 *          ICS_module_id                32
 *          class_id                     32
 *          update_id                    32
 *          pls_flag                      1
 *          valid_flag                    1
 *          reserved                     30
 *       }
 *       crc32                           32
 *    }
 *
*/
/* Header */
#define kNVM_IBTLi_FAV_HEAD_VERSION_SIZE       4
#define kNVM_IBTLi_FAV_HEAD_N_ICSMOD_SIZE      4
#define kNVM_IBTLi_FAV_HEAD_SIZE       (kNVM_IBTLi_FAV_HEAD_VERSION_SIZE+kNVM_IBTLi_FAV_HEAD_N_ICSMOD_SIZE)
/* Element Update request */
#define kNVM_IBTLi_FAV_ICSM_ICSID_SIZE         4
#define kNVM_IBTLi_FAV_ICSM_CLASSID_SIZE       4
#define kNVM_IBTLi_FAV_ICSM_UPDATEID_SIZE      4
#define kNVM_IBTLi_FAV_ICSM_FLAGS_RFU_SIZE     4
#define kNVM_IBTLi_FAV_ICSM_SIZE       (kNVM_IBTLi_FAV_ICSM_ICSID_SIZE+kNVM_IBTLi_FAV_ICSM_CLASSID_SIZE+kNVM_IBTLi_FAV_ICSM_UPDATEID_SIZE+kNVM_IBTLi_FAV_ICSM_FLAGS_RFU_SIZE)
/* Control block */
#define kNVM_IBTLi_FAV_CRC32_SIZE              4
/* Current file version */
#define kNVM_IBTLi_FAV_CURR_VERSION   0x00000002
#define kNVM_IBTLi_FAV_LOWER_VERSION   0x00000001


/**
 *  @brief
 *    NVM File Executable Confirmation format.
 *    Storing in NVM independent from alignment: msbf
 *    => use macros mMSBF_GETxx and mMSBF_SETxx
 *
 *    Syntax                    No. of bits
 *    exe_confirmation_file() {
 *       version                         32
 *       last_exe_ICS_module_id          32
 *       reserved                        31
 *       exe_confirm_flag                 1
 *       crc32                           32
 *    }
 *
 *    => size = 16 bytes
*/
/* Content (fixe) */
#define kNVM_IBTLi_FEC_VERSION_SIZE            4
#define kNVM_IBTLi_FEC_ICSMID_SIZE             4
#define kNVM_IBTLi_FEC_RESECF_SIZE             4
#define kNVM_IBTLi_FEC_CRC32_SIZE              4
/* Size of file */
#define kNVM_IBTLi_FEC_FILE_SIZE (kNVM_IBTLi_FEC_VERSION_SIZE + kNVM_IBTLi_FEC_ICSMID_SIZE + kNVM_IBTLi_FEC_RESECF_SIZE+kNVM_IBTLi_FEC_CRC32_SIZE)
/* Current file version */
#define kNVM_IBTLi_FEC_CURR_VERSION   0x00000001


/******************************************************************************
* Macros
******************************************************************************/


/**
 * Debug tools only enable in mode TRACE enable
 * to dump raw file content
 *
*/
#if defined (__DEBUG_TRACE__)
/***
#error __DEBUG_TRACE__ defined but SHOULD NOT
****/
#ifndef __NVM_IBTL_DUMP_FILE__
#define __NVM_IBTL_DUMP_FILE__  1
#endif

#else
#ifdef __NVM_IBTL_DUMP_FILE__
#undef __NVM_IBTL_DUMP_FILE__
#endif
#endif


/**
 * To automaticly create the application version file when the
 * version is invalid uncomment this define
 *
 * /!\ it's an INTEGRATION OPTION, activation forbidden in product.
 *
#define __NVM_IBTL_AUTO_CREATE_APP_VERSION__  1
 *
 */

/******************************************************************************
* Typedefs
******************************************************************************/
/**
 *	@brief
 *	  Access mode: eREAD to read the dialog area,
 *	  eWRITE to write new value to dialog area.
*/
typedef enum
{
	eWRITE = 0,
	eREAD
}tNVM_IBTLi_AccessMode;

/**
 *  @brief
 *    NVM File descriptor.
 *
*/
typedef struct
{
   tFLASH_CFG_Location *pstLocation; /**< Area location (NVM offset and size)  */
   uint8_t             *pui8Content; /**< Content allocated (if allocated)     */
   int32_t              i32Size;     /**< Size allocated (-1 if not allocated) */
} tNVM_IBTLi_File;

#if defined(NEWLAND_BOOTLOADER)

typedef struct tagLoaderDbInfo
{
    uint32_t magic_number;
    uint32_t ota_type;
    uint16_t ota_pid;
    uint16_t ota_tableid;
    uint32_t ota_frequency;
    uint32_t ota_symbol_rate;
    uint32_t ota_modulation;
    uint32_t u32NewSoftware;
    uint32_t u32CurSoftware;
    uint32_t u32FactoryID;
    uint32_t download_date;
    uint32_t u32FailedCnt;
    uint32_t u32HardwareVersion;
    uint32_t u32DestroyFlag;
    uint32_t u32Crc;

}tNVM_LOADER_DB_INFO_S;

#elif defined(CHANGHONG_BOOTLOADER)
typedef enum
{
	CH_DEMOD_DVB_S = 0, 				/*DVB-S*/
	CH_DEMOD_DVB_C,						/*DVB-C*/
	CH_DEMOD_DVB_T,						/*DVB-T*/
	CH_DEMOD_DMB_TH,
	CH_DEMOD_MAX
}CH_DEMOD_TYPE_e;


/* Upgrade information */
typedef struct
{
	int8_t          	uc_name[12];			/* magic = "Update Info"*/
	int32_t         	i_length;    			/* message size */
	CH_DEMOD_TYPE_e  	enm_AntType;		 	/* Demodulator Type 0 C 1 T 2 S 3 S2 */
	union
	{
		struct
		{
			uint32_t	ui_FreqKHZ;				/* Frequency */
			uint32_t	ui_SymbKbps;			/* symbol rate */
			uint8_t		uc_QAMMode;	  			/* Modulation , 1: QAM16, 2: QAM32 .... 5: QAM256; */
		}stru_CableFreqInfo;
		struct
		{
			uint32_t	ui_FreqKHZ;
			uint32_t	ui_BandWithKHz;
		}stru_TerrestrialFreqInfo;
		struct
		{
			uint32_t	ui_LNBHighFreKhz;
			uint32_t	ui_LNBLowFreKhz;
			uint8_t		uc_LNBPower;
			uint8_t		uc_Polarization;
			uint8_t		uc_22KType;
			uint8_t		uc_DiseqcVer;
			uint8_t		uc_DieseqcPort;
			uint8_t		uc_DieseqcPos;
			int32_t		i_SatelliteLongitude;
			int32_t		i_LocalLongitude;
			int32_t		i_LocalLatitude;
			uint32_t	ui_FreqKHz;
			uint32_t	ui_SymbKbps;
			uint8_t		uc_QPSKMode;
			uint8_t		uc_FEC;
		}stru_SatelliteFreqInfo;
	}un_FreqInfo;								/* frequency information */
	uint16_t		us_DataPid;					/* data PID, determined by the loader */
	uint16_t		us_DataGid; 			 	/* data table ID, determined by the loader */
	uint8_t			uc_UpdaeFlag	;			/* Upgrade flag bits */
	uint32_t		ui_crc;                     /* CRC information */
}CH_UpdateInfo_t;
#elif defined(SKYWORTH_BOOTLOADER)
/* Structures for Skyworth's loadflag area in flash */
typedef enum
{
    LDR_QAM16 = 1,
    LDR_QAM32,
    LDR_QAM64,
    LDR_QAM128,
    LDR_QAM256
} SKYLoader_ModulatorMode;

typedef enum
{
    LDR_DEMOD_SIGNAL_CONNECTED = 0,
    LDR_DEMOD_SIGNAL_LOST
} Ldr_DemodStatus;

typedef struct tagSKYLoaderInfo
{
/*   char unkown[36];	
	char reserve[20];
	char update[74];
*/
    struct
    {
        struct
        {
            unsigned long           uFrequencyKHz;
            unsigned long           uSymbolRateKHz;
            SKYLoader_ModulatorMode modulatorMode;
        }               tuner;
        unsigned int    pid;
        unsigned int    reserve;
    }               update_info;    // 20bytes

} tNVM_SKYLoaderInfo;

#endif

/******************************************************************************
* Locales Variables
******************************************************************************/

/**
 *  @brief
 *    Initialization indicator: TRUE when initialized (when NVM_IBTL_Init OK)
 *    else FALSE (initial value and when NVM_IBTL_Init OK).
*/
static bool vNVM_IBTLi_isInit = FALSE;

/**
 *  @brief
 *    Area location (offset in NVM and size) of file Update Request in NVM.
 *    Given by the flash configuration function 'FLASH_CFG_GetNvmAreaLocation'
 *    at the initialization
*/
static tFLASH_CFG_Location vNVM_IBTLi_stAreaLocUpdateRequest;

/**
 *  @brief
 *    Area location (offset in NVM and size) of file Update Request in NVM.
 *    Given by the flash configuration function 'FLASH_CFG_GetNvmAreaLocation'
 *    at the initialization
*/
static tFLASH_CFG_Location vNVM_IBTLi_stAreaLocDialogArea;

/**
 *  @brief
 *    Area location (offset in NVM and size) of file execution confirmation in NVM.
 *    Given by the flash configuration function 'FLASH_CFG_GetNvmAreaLocation'
 *    at the initialization
*/
static tFLASH_CFG_Location vNVM_IBTLi_stAreaLocExeConfirmation;

static tFLASH_CFG_Location vNVM_IBTLi_stAreaLocAppliVersion;

uint32_t crc;
uint16_t crc_16;
/**
 *  @brief
 *    CRC Table
*/
#ifdef CHANGHONG_BOOTLOADER
static const uint32_t gui_CHCOMMPEG32Table[256] =
{
   0x00000000L, 0x04C11DB7L, 0x09823B6EL, 0x0D4326D9L,
   0x130476DCL, 0x17C56B6BL, 0x1A864DB2L, 0x1E475005L,
   0x2608EDB8L, 0x22C9F00FL, 0x2F8AD6D6L, 0x2B4BCB61L,
   0x350C9B64L, 0x31CD86D3L, 0x3C8EA00AL, 0x384FBDBDL,
   0x4C11DB70L, 0x48D0C6C7L, 0x4593E01EL, 0x4152FDA9L,
   0x5F15ADACL, 0x5BD4B01BL, 0x569796C2L, 0x52568B75L,
   0x6A1936C8L, 0x6ED82B7FL, 0x639B0DA6L, 0x675A1011L,
   0x791D4014L, 0x7DDC5DA3L, 0x709F7B7AL, 0x745E66CDL,
   0x9823B6E0L, 0x9CE2AB57L, 0x91A18D8EL, 0x95609039L,
   0x8B27C03CL, 0x8FE6DD8BL, 0x82A5FB52L, 0x8664E6E5L,
   0xBE2B5B58L, 0xBAEA46EFL, 0xB7A96036L, 0xB3687D81L,
   0xAD2F2D84L, 0xA9EE3033L, 0xA4AD16EAL, 0xA06C0B5DL,
   0xD4326D90L, 0xD0F37027L, 0xDDB056FEL, 0xD9714B49L,
   0xC7361B4CL, 0xC3F706FBL, 0xCEB42022L, 0xCA753D95L,
   0xF23A8028L, 0xF6FB9D9FL, 0xFBB8BB46L, 0xFF79A6F1L,
   0xE13EF6F4L, 0xE5FFEB43L, 0xE8BCCD9AL, 0xEC7DD02DL,
   0x34867077L, 0x30476DC0L, 0x3D044B19L, 0x39C556AEL,
   0x278206ABL, 0x23431B1CL, 0x2E003DC5L, 0x2AC12072L,
   0x128E9DCFL, 0x164F8078L, 0x1B0CA6A1L, 0x1FCDBB16L,
   0x018AEB13L, 0x054BF6A4L, 0x0808D07DL, 0x0CC9CDCAL,
   0x7897AB07L, 0x7C56B6B0L, 0x71159069L, 0x75D48DDEL,
   0x6B93DDDBL, 0x6F52C06CL, 0x6211E6B5L, 0x66D0FB02L,
   0x5E9F46BFL, 0x5A5E5B08L, 0x571D7DD1L, 0x53DC6066L,
   0x4D9B3063L, 0x495A2DD4L, 0x44190B0DL, 0x40D816BAL,
   0xACA5C697L, 0xA864DB20L, 0xA527FDF9L, 0xA1E6E04EL,
   0xBFA1B04BL, 0xBB60ADFCL, 0xB6238B25L, 0xB2E29692L,
   0x8AAD2B2FL, 0x8E6C3698L, 0x832F1041L, 0x87EE0DF6L,
   0x99A95DF3L, 0x9D684044L, 0x902B669DL, 0x94EA7B2AL,
   0xE0B41DE7L, 0xE4750050L, 0xE9362689L, 0xEDF73B3EL,
   0xF3B06B3BL, 0xF771768CL, 0xFA325055L, 0xFEF34DE2L,
   0xC6BCF05FL, 0xC27DEDE8L, 0xCF3ECB31L, 0xCBFFD686L,
   0xD5B88683L, 0xD1799B34L, 0xDC3ABDEDL, 0xD8FBA05AL,
   0x690CE0EEL, 0x6DCDFD59L, 0x608EDB80L, 0x644FC637L,
   0x7A089632L, 0x7EC98B85L, 0x738AAD5CL, 0x774BB0EBL,
   0x4F040D56L, 0x4BC510E1L, 0x46863638L, 0x42472B8FL,
   0x5C007B8AL, 0x58C1663DL, 0x558240E4L, 0x51435D53L,
   0x251D3B9EL, 0x21DC2629L, 0x2C9F00F0L, 0x285E1D47L,
   0x36194D42L, 0x32D850F5L, 0x3F9B762CL, 0x3B5A6B9BL,
   0x0315D626L, 0x07D4CB91L, 0x0A97ED48L, 0x0E56F0FFL,
   0x1011A0FAL, 0x14D0BD4DL, 0x19939B94L, 0x1D528623L,
   0xF12F560EL, 0xF5EE4BB9L, 0xF8AD6D60L, 0xFC6C70D7L,
   0xE22B20D2L, 0xE6EA3D65L, 0xEBA91BBCL, 0xEF68060BL,
   0xD727BBB6L, 0xD3E6A601L, 0xDEA580D8L, 0xDA649D6FL,
   0xC423CD6AL, 0xC0E2D0DDL, 0xCDA1F604L, 0xC960EBB3L,
   0xBD3E8D7EL, 0xB9FF90C9L, 0xB4BCB610L, 0xB07DABA7L,
   0xAE3AFBA2L, 0xAAFBE615L, 0xA7B8C0CCL, 0xA379DD7BL,
   0x9B3660C6L, 0x9FF77D71L, 0x92B45BA8L, 0x9675461FL,
   0x8832161AL, 0x8CF30BADL, 0x81B02D74L, 0x857130C3L,
   0x5D8A9099L, 0x594B8D2EL, 0x5408ABF7L, 0x50C9B640L,
   0x4E8EE645L, 0x4A4FFBF2L, 0x470CDD2BL, 0x43CDC09CL,
   0x7B827D21L, 0x7F436096L, 0x7200464FL, 0x76C15BF8L,
   0x68860BFDL, 0x6C47164AL, 0x61043093L, 0x65C52D24L,
   0x119B4BE9L, 0x155A565EL, 0x18197087L, 0x1CD86D30L,
   0x029F3D35L, 0x065E2082L, 0x0B1D065BL, 0x0FDC1BECL,
   0x3793A651L, 0x3352BBE6L, 0x3E119D3FL, 0x3AD08088L,
   0x2497D08DL, 0x2056CD3AL, 0x2D15EBE3L, 0x29D4F654L,
   0xC5A92679L, 0xC1683BCEL, 0xCC2B1D17L, 0xC8EA00A0L,
   0xD6AD50A5L, 0xD26C4D12L, 0xDF2F6BCBL, 0xDBEE767CL,
   0xE3A1CBC1L, 0xE760D676L, 0xEA23F0AFL, 0xEEE2ED18L,
   0xF0A5BD1DL, 0xF464A0AAL, 0xF9278673L, 0xFDE69BC4L,
   0x89B8FD09L, 0x8D79E0BEL, 0x803AC667L, 0x84FBDBD0L,
   0x9ABC8BD5L, 0x9E7D9662L, 0x933EB0BBL, 0x97FFAD0CL,
   0xAFB010B1L, 0xAB710D06L, 0xA6322BDFL, 0xA2F33668L,
   0xBCB4666DL, 0xB8757BDAL, 0xB5365D03L, 0xB1F740B4L
};

uint32_t CH_COM_MPEGCRC32 (uint8_t *rpuc_DataAddr,int32_t ri_DataLength)
{
	uint32_t	ulCRC;
	uint8_t*	pbData= NULL;

	/**********************
	* Lookup Table method *
	**********************/
	if( ri_DataLength <= 0 || rpuc_DataAddr == NULL )
	{
		return 0;
	}

	/* init the start value */
	ulCRC = 0xFFFFFFFF;

	pbData = rpuc_DataAddr;

	/* calculate CRC */
	while (ri_DataLength--)
	{
		ulCRC = gui_CHCOMMPEG32Table[((ulCRC>>24) ^ *pbData++) & 0xFFL] ^ (ulCRC << 8);
	}

	return (ulCRC);
}
#endif

static uint32_t vNVM_IBTLi_tui32CrcTable[kNVM_IBTLi_CRC_TABLE_SIZE] =
{  0x00000000L, 0x77073096L, 0xee0e612cL, 0x990951baL, 0x076dc419L,
  0x706af48fL, 0xe963a535L, 0x9e6495a3L, 0x0edb8832L, 0x79dcb8a4L,
  0xe0d5e91eL, 0x97d2d988L, 0x09b64c2bL, 0x7eb17cbdL, 0xe7b82d07L,
  0x90bf1d91L, 0x1db71064L, 0x6ab020f2L, 0xf3b97148L, 0x84be41deL,
  0x1adad47dL, 0x6ddde4ebL, 0xf4d4b551L, 0x83d385c7L, 0x136c9856L,
  0x646ba8c0L, 0xfd62f97aL, 0x8a65c9ecL, 0x14015c4fL, 0x63066cd9L,
  0xfa0f3d63L, 0x8d080df5L, 0x3b6e20c8L, 0x4c69105eL, 0xd56041e4L,
  0xa2677172L, 0x3c03e4d1L, 0x4b04d447L, 0xd20d85fdL, 0xa50ab56bL,
  0x35b5a8faL, 0x42b2986cL, 0xdbbbc9d6L, 0xacbcf940L, 0x32d86ce3L,
  0x45df5c75L, 0xdcd60dcfL, 0xabd13d59L, 0x26d930acL, 0x51de003aL,
  0xc8d75180L, 0xbfd06116L, 0x21b4f4b5L, 0x56b3c423L, 0xcfba9599L,
  0xb8bda50fL, 0x2802b89eL, 0x5f058808L, 0xc60cd9b2L, 0xb10be924L,
  0x2f6f7c87L, 0x58684c11L, 0xc1611dabL, 0xb6662d3dL, 0x76dc4190L,
  0x01db7106L, 0x98d220bcL, 0xefd5102aL, 0x71b18589L, 0x06b6b51fL,
  0x9fbfe4a5L, 0xe8b8d433L, 0x7807c9a2L, 0x0f00f934L, 0x9609a88eL,
  0xe10e9818L, 0x7f6a0dbbL, 0x086d3d2dL, 0x91646c97L, 0xe6635c01L,
  0x6b6b51f4L, 0x1c6c6162L, 0x856530d8L, 0xf262004eL, 0x6c0695edL,
  0x1b01a57bL, 0x8208f4c1L, 0xf50fc457L, 0x65b0d9c6L, 0x12b7e950L,
  0x8bbeb8eaL, 0xfcb9887cL, 0x62dd1ddfL, 0x15da2d49L, 0x8cd37cf3L,
  0xfbd44c65L, 0x4db26158L, 0x3ab551ceL, 0xa3bc0074L, 0xd4bb30e2L,
  0x4adfa541L, 0x3dd895d7L, 0xa4d1c46dL, 0xd3d6f4fbL, 0x4369e96aL,
  0x346ed9fcL, 0xad678846L, 0xda60b8d0L, 0x44042d73L, 0x33031de5L,
  0xaa0a4c5fL, 0xdd0d7cc9L, 0x5005713cL, 0x270241aaL, 0xbe0b1010L,
  0xc90c2086L, 0x5768b525L, 0x206f85b3L, 0xb966d409L, 0xce61e49fL,
  0x5edef90eL, 0x29d9c998L, 0xb0d09822L, 0xc7d7a8b4L, 0x59b33d17L,
  0x2eb40d81L, 0xb7bd5c3bL, 0xc0ba6cadL, 0xedb88320L, 0x9abfb3b6L,
  0x03b6e20cL, 0x74b1d29aL, 0xead54739L, 0x9dd277afL, 0x04db2615L,
  0x73dc1683L, 0xe3630b12L, 0x94643b84L, 0x0d6d6a3eL, 0x7a6a5aa8L,
  0xe40ecf0bL, 0x9309ff9dL, 0x0a00ae27L, 0x7d079eb1L, 0xf00f9344L,
  0x8708a3d2L, 0x1e01f268L, 0x6906c2feL, 0xf762575dL, 0x806567cbL,
  0x196c3671L, 0x6e6b06e7L, 0xfed41b76L, 0x89d32be0L, 0x10da7a5aL,
  0x67dd4accL, 0xf9b9df6fL, 0x8ebeeff9L, 0x17b7be43L, 0x60b08ed5L,
  0xd6d6a3e8L, 0xa1d1937eL, 0x38d8c2c4L, 0x4fdff252L, 0xd1bb67f1L,
  0xa6bc5767L, 0x3fb506ddL, 0x48b2364bL, 0xd80d2bdaL, 0xaf0a1b4cL,
  0x36034af6L, 0x41047a60L, 0xdf60efc3L, 0xa867df55L, 0x316e8eefL,
  0x4669be79L, 0xcb61b38cL, 0xbc66831aL, 0x256fd2a0L, 0x5268e236L,
  0xcc0c7795L, 0xbb0b4703L, 0x220216b9L, 0x5505262fL, 0xc5ba3bbeL,
  0xb2bd0b28L, 0x2bb45a92L, 0x5cb36a04L, 0xc2d7ffa7L, 0xb5d0cf31L,
  0x2cd99e8bL, 0x5bdeae1dL, 0x9b64c2b0L, 0xec63f226L, 0x756aa39cL,
  0x026d930aL, 0x9c0906a9L, 0xeb0e363fL, 0x72076785L, 0x05005713L,
  0x95bf4a82L, 0xe2b87a14L, 0x7bb12baeL, 0x0cb61b38L, 0x92d28e9bL,
  0xe5d5be0dL, 0x7cdcefb7L, 0x0bdbdf21L, 0x86d3d2d4L, 0xf1d4e242L,
  0x68ddb3f8L, 0x1fda836eL, 0x81be16cdL, 0xf6b9265bL, 0x6fb077e1L,
  0x18b74777L, 0x88085ae6L, 0xff0f6a70L, 0x66063bcaL, 0x11010b5cL,
  0x8f659effL, 0xf862ae69L, 0x616bffd3L, 0x166ccf45L, 0xa00ae278L,
  0xd70dd2eeL, 0x4e048354L, 0x3903b3c2L, 0xa7672661L, 0xd06016f7L,
  0x4969474dL, 0x3e6e77dbL, 0xaed16a4aL, 0xd9d65adcL, 0x40df0b66L,
  0x37d83bf0L, 0xa9bcae53L, 0xdebb9ec5L, 0x47b2cf7fL, 0x30b5ffe9L,
  0xbdbdf21cL, 0xcabac28aL, 0x53b39330L, 0x24b4a3a6L, 0xbad03605L,
  0xcdd70693L, 0x54de5729L, 0x23d967bfL, 0xb3667a2eL, 0xc4614ab8L,
  0x5d681b02L, 0x2a6f2b94L, 0xb40bbe37L, 0xc30c8ea1L, 0x5a05df1bL,
  0x2d02ef8dL
};


#define DO1(buf) crc = vNVM_IBTLi_tui32CrcTable[((int)crc ^ (*buf++)) & 0xff] ^ (crc >> 8);
#define DO2(buf)  DO1(buf); DO1(buf);
#define DO4(buf)  DO2(buf); DO2(buf);
#define DO8(buf)  DO4(buf); DO4(buf);

/******************************************************************************
* Locales Functions Prototypes
******************************************************************************/

tNVM_IBTL_Status NVM_IBTLi_AccessUpdReq        (
#if defined (__DEBUG_TRACE__)
                                                const char *TBOX_FunctionName,
#endif
                                                bool xisGetHighest,
                                                tNVM_IBTL_UpdateRequest *pxstUpdRequest,
                                                bool xisSetRFMod,
                                                tNVM_IBTL_RFModulation *pxstRFMod);
#ifdef NEWLAND_BOOTLOADER

tNVM_IBTL_Status NVM_IBTLi_AccessDialogArea
(
   bool                 xisReadOrWrite,
   tNVM_IBTL_DialogArea *pxstDialogArea
);
#else
static tNVM_IBTL_Status NVM_IBTLi_AccessDialogArea    (tNVM_IBTLi_AccessMode uAccessMode,
                                                       tNVM_IBTL_DialogArea *pxstDialogArea);
#endif


static tNVM_IBTL_Status NVM_IBTLi_FileOpen     (tNVM_IBTLi_File *pxstFile,
                                                tFLASH_CFG_Location *pxstLocation,
                                                int32_t xi32Size);
static tNVM_IBTL_Status NVM_IBTLi_FileClose    (tNVM_IBTLi_File *pxstFile);
static int32_t          NVM_IBTLi_FileRead     (tNVM_IBTLi_File *pxstFile,
                                                uint8_t *pui8xContent,
                                                int32_t xi32SizeMax);
static int32_t          NVM_IBTLi_FileWrite    (tNVM_IBTLi_File *pxstFile,
                                                uint8_t *pui8xData,
                                                int32_t xi32Size);
static void             NVM_IBTLi_InitTableCRC (void);
static uint32_t         NVM_IBTLi_ComputeCRC   (uint8_t *pxui8Data,
                                                uint32_t xi32Size,
                                                uint32_t xui32CurrCRC,
                                                bool xisFirstBlock);

#if defined (__NVM_IBTL_AUTO_CREATE_APP_VERSION__)
static bool NVM_IBTLi_DEBUG_WriteAppVersionDefault (tNVM_IBTLi_File *pxstFile);
#endif
#if defined (__NVM_IBTL_DUMP_FILE__)
static void             NVM_IBTLi_DumpFile     (uint8_t *pxui8Data,
                                                int32_t xi32Size,
                                                tFLASH_CFG_Location *pxstFL,
                                                const char *pxaFunctName,
                                                const char *pxaFileLabel,
                                                const char *pxaOperationLabel);
#endif


#ifdef NEWLAND_BOOTLOADER

static uint32_t NVM_IBTLi_ConvertNewlandToIBTL(tNVM_LOADER_DB_INFO_S nl_struc, tNVM_IBTL_DialogArea* ibtl_struc);

static uint32_t NVM_IBTLi_ConvertIBTLToNewland(tNVM_IBTL_DialogArea ibtl_struc, tNVM_LOADER_DB_INFO_S* nl_struc);

static void NVM_IBTLi_ConvertEndian4Bytes(uint32_t *pValue);

static void NVM_IBTLi_ConvertEndian2Bytes(uint16_t *pValue);


#elif defined(CHANGHONG_BOOTLOADER)
static void NVM_IBTLi_ConvertCHToIBTL(CH_UpdateInfo_t ch_struc, tNVM_IBTL_DialogArea* ibtl_struc);

static void NVM_IBTLi_ConvertIBTLToCH(tNVM_IBTL_DialogArea ibtl_struc, CH_UpdateInfo_t* ch_struc);

static void NVM_IBTLi_TraceCHDlgArea(CH_UpdateInfo_t * pstNewlandDialogArea);
#elif defined(SKYWORTH_BOOTLOADER)
    static void NVM_IBTLi_ConvertSKYToIBTL(tNVM_SKYLoaderInfo sky_struc, tNVM_IBTL_DialogArea* ibtl_struc);

    static void NVM_IBTLi_ConvertIBTLToSKY(tNVM_IBTL_DialogArea ibtl_struc, tNVM_SKYLoaderInfo* sky_struc);

    static void NVM_IBTLi_TraceSKYDlgArea(tNVM_SKYLoaderInfo * pstSkyworthDialogArea);
#endif

/*
 *  @brief
 *    Synchronize flash sector with chunk content
 *
 *
 *  @param[in] u32xAddress
 *       Adress offset in flash where to burn chunk
 *
 *  @param[in] pu8xBuffer
 *       Buffer containing the chunk data
 *
 *  @param[in] u32xNumberToWrite
 *      chunk size
 *
 *  @retval N > 0
 *       Number of bytes read.
 *
 *  @retval N = 0
 *       Read 0 bytes => failure
 *
 *
 */
static uint32_t NVM_IBTLi_FLA_Write(
    uint32_t u32xAddress,
    uint8_t* pu8xBuffer,
    uint32_t u32xNumberToWrite
);
/******************************************************************************
 * Public Function
 ******************************************************************************/

/**
 *  @brief
 *    Initialize the using of NVM interface with the bootloader.
 *
 *  @retval  eNVM_IBTL_NO_ERROR
 *              Initialization successful.
 *
 *  @retval  eNVM_IBTL_ERROR
 *              Initialization failed (get NVM area location of files
 *              failed)
 *
 *  @remarks
 *    -# If this interface is already initialized, this function
 *       return immediatly eNVM_IBTL_NO_ERROR.
*/
tNVM_IBTL_Status NVM_IBTL_Init
(
   void
)
{
   mTBOX_FCT_ENTER(NVM_IBTL_Init);

   if (vNVM_IBTLi_isInit)
   {
      /* Warning but return OK */
      mTBOX_TRACE((kTBOX_NIV_WARNING,
                   "%s() WARNING: "
                   "NVM Interface with bootloader already initialized\n",
                   TBOX_FunctionName));
      mTBOX_RETURN(eNVM_IBTL_NO_ERROR);
   }

   /* Mode trace initialization */
   mTBOX_INIT_MOD_TRACE((eNVM_IBTL, kTBOX_TRC_CRITICAL
   									/*+kTBOX_TRC_WARNING   */
                                    /*+kTBOX_TRC_FCT */
                                    /*+kTBOX_TRC_1 A PI general traces       */
                                    /*+kTBOX_TRC_2 Files location and access */
                                    /*+kTBOX_TRC_3 Files data                */
                                    /*+kTBOX_TRC_4 Not used                  */
                                    /*+kTBOX_TRC_5 Not used                  */));


   /* eIBTL_NVM_UPD_REQ file */
   vNVM_IBTLi_stAreaLocUpdateRequest . addressOffset = NVM_IBTL_Location[eIBTL_NVM_UPD_REQ].addressOffset;
   vNVM_IBTLi_stAreaLocUpdateRequest . size = NVM_IBTL_Location[eIBTL_NVM_UPD_REQ].size;
   mTBOX_TRACE((kTBOX_NIV_2,
                "[%s] Update Request Location:   offset=%08X size=%08X=%u\n",
                TBOX_FunctionName,
                vNVM_IBTLi_stAreaLocUpdateRequest.addressOffset,
                vNVM_IBTLi_stAreaLocUpdateRequest.size,
                vNVM_IBTLi_stAreaLocUpdateRequest.size));

   /* eIBTL_NVM_DIALOG file */
   vNVM_IBTLi_stAreaLocDialogArea . addressOffset = NVM_IBTL_Location[eIBTL_NVM_DIALOG].addressOffset;
   vNVM_IBTLi_stAreaLocDialogArea . size = NVM_IBTL_Location[eIBTL_NVM_DIALOG].size;
   mTBOX_TRACE((kTBOX_NIV_2,
                "[%s] Update Request Location:   offset=%08X size=%08X=%u\n",
                TBOX_FunctionName,
                vNVM_IBTLi_stAreaLocDialogArea.addressOffset,
                vNVM_IBTLi_stAreaLocDialogArea.size,
                vNVM_IBTLi_stAreaLocDialogArea.size));

   /* eIBTL_NVM_EXE_CONF file */
   vNVM_IBTLi_stAreaLocExeConfirmation . addressOffset = NVM_IBTL_Location[eIBTL_NVM_EXE_CONF].addressOffset;
   vNVM_IBTLi_stAreaLocExeConfirmation . size = NVM_IBTL_Location[eIBTL_NVM_EXE_CONF].size;
   mTBOX_TRACE((kTBOX_NIV_2,
                "[%s] Exe Confirmation Location: offset=%08X size=%08X=%u\n",
                TBOX_FunctionName,
                vNVM_IBTLi_stAreaLocExeConfirmation.addressOffset,
                vNVM_IBTLi_stAreaLocExeConfirmation.size,
                vNVM_IBTLi_stAreaLocExeConfirmation.size));

   /* eIBTL_NVM_APP_VERS file */
   vNVM_IBTLi_stAreaLocAppliVersion . addressOffset = NVM_IBTL_Location[eIBTL_NVM_APP_VERS].addressOffset;
   vNVM_IBTLi_stAreaLocAppliVersion . size = NVM_IBTL_Location[eIBTL_NVM_APP_VERS].size;
   mTBOX_TRACE((kTBOX_NIV_2,
                "[%s] Application Version: offset=%08X size=%08X=%u\n",
                TBOX_FunctionName,
                vNVM_IBTLi_stAreaLocAppliVersion.addressOffset,
                vNVM_IBTLi_stAreaLocAppliVersion.size,
                vNVM_IBTLi_stAreaLocAppliVersion.size));

   /* Okay */
   vNVM_IBTLi_isInit = TRUE;
   mTBOX_TRACE((kTBOX_NIV_1,
                "[%s] NVM Interface with bootloader INITIALIZED\n",
                TBOX_FunctionName));

#ifdef NEWLAND_BOOTLOADER

// HERE we are writing update ID in current software field of flash. If some one wants to force update it can overwrite it with
{
   tNVM_IBTL_DialogArea dialog;
   //VERSION_CFG_UDATE_ID
    NVM_IBTLi_AccessDialogArea(TRUE, &dialog);

    dialog.stDLCycle.u8DLState = 0;
    dialog.stDLCycle.u32UpdateID = SYS_INFO_GetSoftwareVersionId();

    NVM_IBTLi_AccessDialogArea(FALSE, &dialog);

}

#endif
   mTBOX_RETURN(eNVM_IBTL_NO_ERROR);
}

/**
 *  @brief
 *    Termination of the using of NVM interface with the bootloader.
 *
 *  @retval  eNVM_IBTL_NO_ERROR
 *              Termination successful.
 *
 *  @retval  eNVM_IBTL_ERROR
 *              Termination failed
 *
 *  @remarks
 *    -# If this interface is not initialized, this function
 *       return immediatly eNVM_IBTL_NO_ERROR.
*/
tNVM_IBTL_Status NVM_IBTL_Term
(
   void
)
{
   mTBOX_FCT_ENTER(NVM_IBTL_Term);

   if (!vNVM_IBTLi_isInit)
   {
      /* Warning but return OK */
      mTBOX_TRACE((kTBOX_NIV_WARNING,
                   "%s() WARNING: "
                   "NVM Interface with bootloader is not initialized\n",
                   TBOX_FunctionName));
      mTBOX_RETURN(eNVM_IBTL_NO_ERROR);
   }

   /* Reset area location */
   memset(&vNVM_IBTLi_stAreaLocUpdateRequest,0,sizeof(tFLASH_CFG_Location));
   memset(&vNVM_IBTLi_stAreaLocDialogArea,0,sizeof(tFLASH_CFG_Location));
   memset(&vNVM_IBTLi_stAreaLocExeConfirmation,0,sizeof(tFLASH_CFG_Location));
   memset(&vNVM_IBTLi_stAreaLocAppliVersion,0,sizeof(tFLASH_CFG_Location));

   /* Okay */
   vNVM_IBTLi_isInit = FALSE;
   mTBOX_TRACE((kTBOX_NIV_1,
                "[%s] NVM Interface with bootloader TERMINATED\n",
                TBOX_FunctionName));

   mTBOX_RETURN(eNVM_IBTL_NO_ERROR);
}

/**
 *  @brief
 *    Set execution flag in NVM Executable Confirmation file.
 *    A flag set to 1 indicate that the application has executed correctly
 *    (0 => not executed, normaly value set by the bootloader application).
 *
 *  @param[in] xui32ICSModuleId
 *       ICS module identifier to identify the Application Modules that set
 *       the execution flag (see defines kNVM_IBTL_ICS_MODID_xxx).
 *       Just to identify caller (for trace)
 *
 *  @param[in] xIsExecuted
 *       TRUE to set execution confirm flag to 1.
 *       FALSE to set execution confirm flag to 0.
 *
 *  @retval eNVM_IBTL_NO_ERROR
 *       Open successful.
 *
 *  @retval eNVM_IBTL_ERROR_MEMORY
 *       Content allocation failure.
 *
 *  @retval eNVM_IBTL_ERROR_WRITE
 *       Write file failure.
 *
 *  @retval eNVM_IBTL_ERROR
 *       Bad parameters or module not initialized.
 *
 *  @remarks
 *    -# If an read error or a invalid value read (version), set the default
 *       values in this file and not return an error..
 *
 *    -# If the module ID is undefined, not update it and set only the
 *       execution flag.
*/
tNVM_IBTL_Status NVM_IBTL_SetExecutionFlag
(
   uint32_t xui32ICSModuleId,
   bool     xIsExecuted
)
{
   tNVM_IBTL_Status rc;
   tNVM_IBTLi_File  file = kNVM_IBTLi_FILE_NULL;
   uint32_t         version,mod_id,res_flag,calc_crc,crc32;
   int32_t          offset,n;

   mTBOX_FCT_ENTER(NVM_IBTL_SetExecutionFlag);

   if (!vNVM_IBTLi_isInit)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"%s(%#x,%d) ERROR: "
                   "NVM Interface with bootloader is not initialized\n",
                   TBOX_FunctionName,xui32ICSModuleId,(int)xIsExecuted));
      mTBOX_RETURN(eNVM_IBTL_ERROR);
   }

   mTBOX_TRACE((kTBOX_NIV_1,
                "[%s] Set execution flag of ICS module '%s' %#x = %s "
                "in Executable Confirmation File {%#x,%#x}\n",
                TBOX_FunctionName,
                NVM_IBTL_DEBUG_LabelModuleID(xui32ICSModuleId),
                xui32ICSModuleId,(xIsExecuted)?"TRUE":"FALSE",
                vNVM_IBTLi_stAreaLocExeConfirmation.addressOffset,
                vNVM_IBTLi_stAreaLocExeConfirmation.size));

   /* Open the file */
   rc = NVM_IBTLi_FileOpen(&file,&vNVM_IBTLi_stAreaLocExeConfirmation,
                           kNVM_IBTLi_FEC_FILE_SIZE);

   if (rc != eNVM_IBTL_NO_ERROR)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"%s(%#x,%d) ERROR: "
                   "Open Executable Confirmation File {%#x,%#x} "
                   "failure (error %d, size=%d)\n",
                   TBOX_FunctionName,xui32ICSModuleId,(int)xIsExecuted,
                   vNVM_IBTLi_stAreaLocExeConfirmation.addressOffset,
                   vNVM_IBTLi_stAreaLocExeConfirmation.size,rc,
                   kNVM_IBTLi_FEC_FILE_SIZE));
      mTBOX_RETURN(rc);
   }

   /* Read in internal buffer allocated at the open */
   n = NVM_IBTLi_FileRead(&file,NULL,-1);

   if (n != file.i32Size)
   {
      /* Warning set default value and continue */
      mTBOX_TRACE((kTBOX_NIV_WARNING,"%s(%#x,%d) ERROR: "
                   "Read Executable Confirmation File {%#x,%#x} "
                   "failure (read %d/%d) => Set default value\n",
                   TBOX_FunctionName,xui32ICSModuleId,(int)xIsExecuted,
                   vNVM_IBTLi_stAreaLocExeConfirmation.addressOffset,
                   vNVM_IBTLi_stAreaLocExeConfirmation.size,
                   n,file.i32Size));
      version  = kNVM_IBTLi_FEC_CURR_VERSION;
      mod_id   = kNVM_IBTL_ICS_MODID_NONE;
      res_flag = 0;
   }
   else
   {
#if defined (__NVM_IBTL_DUMP_FILE__)
      NVM_IBTLi_DumpFile(file.pui8Content,file.i32Size,
                         &vNVM_IBTLi_stAreaLocExeConfirmation,
                         TBOX_FunctionName,"Exe Confirmation File","READ");
#endif
      /* Extract data, compute crc (and dump) */
      offset   = 0;
      mMSBF_GET32_O(file.pui8Content,offset,version);
      mMSBF_GET32_O(file.pui8Content,offset,mod_id);
      mMSBF_GET32_O(file.pui8Content,offset,res_flag);
      calc_crc = NVM_IBTLi_ComputeCRC(file.pui8Content,offset,0,TRUE);
      mMSBF_GET32_O(file.pui8Content,offset,crc32);

      mTBOX_TRACE((kTBOX_NIV_3,"[%s] - EXECUTABLE CONFIRMATION (%d bytes):\n",
                   TBOX_FunctionName,file.i32Size));
      mTBOX_TRACE((kTBOX_NIV_3,"- version................0x%08X\n",
                   version));
      mTBOX_TRACE((kTBOX_NIV_3,"- last_exe_ICS_module_id.0x%08X (%s)\n",
                   mod_id,NVM_IBTL_DEBUG_LabelModuleID(mod_id)));
      mTBOX_TRACE((kTBOX_NIV_3,"- Reserved...............0x%08X\n",
                   res_flag>>1));
      mTBOX_TRACE((kTBOX_NIV_3,"- exe_confim_flag........%u\n",
                   res_flag&0x00000001));
      mTBOX_TRACE((kTBOX_NIV_3,"- crc32..................0x%08X (calc=0x%08X)\n",
                   crc32,calc_crc));

      /* Check the version */
      if (version != kNVM_IBTLi_FEC_CURR_VERSION)
      {
         /* Warning set default value and continue */
         mTBOX_TRACE((kTBOX_NIV_WARNING,"%s(%#x,%d) ERROR: "
                      "Version %#x of Executable Confirmation File {%#x,%#x} "
                      "invalid (wait %#x) => Set default value\n",
                      TBOX_FunctionName,xui32ICSModuleId,(int)xIsExecuted,
                      version,
                      vNVM_IBTLi_stAreaLocExeConfirmation.addressOffset,
                      vNVM_IBTLi_stAreaLocExeConfirmation.size,
                      kNVM_IBTLi_FEC_CURR_VERSION));
         version  = kNVM_IBTLi_FEC_CURR_VERSION;
         mod_id   = kNVM_IBTL_ICS_MODID_NONE;
         res_flag = 0;
      }
      else if (crc32 != calc_crc)
      {
         /* Warning set default value and continue */
         mTBOX_TRACE((kTBOX_NIV_WARNING,"%s(%#x,%d) ERROR: "
                      "CRC32 0x%08X of Executable Confirmation File {%#x,%#x} "
                      "invalid (wait %#x) => Set default value\n",
                      TBOX_FunctionName,xui32ICSModuleId,(int)xIsExecuted,
                      crc32,vNVM_IBTLi_stAreaLocExeConfirmation.addressOffset,
                      vNVM_IBTLi_stAreaLocExeConfirmation.size,
                      calc_crc));
         mod_id   = kNVM_IBTL_ICS_MODID_NONE;
         res_flag = 0;
      }
      else if (mod_id == kNVM_IBTL_ICS_MODID_NONE)
      {
         /* Bootloader want do nothing */
         res_flag = 0;
      }
      else if (xIsExecuted)
      {
         /* Set is excuted = 1 */
         res_flag |= 0x00000001;
      }
      else
      {
         /* Normaly never call with is executed = FALSE */
         res_flag &= 0xFFFFFFFE;
      }
   }

	if (res_flag != 0)
	{
      /* Format content file */
      offset = 0;
      mMSBF_SET32_O(file.pui8Content,offset,version);
      mMSBF_SET32_O(file.pui8Content,offset,xui32ICSModuleId);
      mMSBF_SET32_O(file.pui8Content,offset,res_flag);
      calc_crc = NVM_IBTLi_ComputeCRC(file.pui8Content,offset,0,TRUE);
      mMSBF_SET32_O(file.pui8Content,offset,calc_crc);

#if defined (__NVM_IBTL_DUMP_FILE__)
      NVM_IBTLi_DumpFile(file.pui8Content,file.i32Size,
                         &vNVM_IBTLi_stAreaLocExeConfirmation,
                         TBOX_FunctionName,"Exe Confirmation File","WRITE");
#endif

		/* write only if change */
  		n = NVM_IBTLi_FileWrite(&file,NULL,-1);

      if (n == file.i32Size)
      {
         /* OKAY */
         mTBOX_TRACE((kTBOX_NIV_1,
                      "[%s] ics=%#x exec_flag=%d=0x%08x crc=0x%08x written "
                      "in Executable Confirmation File {%#x,%#x}\n",
                      TBOX_FunctionName,xui32ICSModuleId,(int)xIsExecuted,
                      res_flag,calc_crc,
                      vNVM_IBTLi_stAreaLocExeConfirmation.addressOffset,
                      vNVM_IBTLi_stAreaLocExeConfirmation.size));
         rc = eNVM_IBTL_NO_ERROR;
      }
      else
      {
         /* Error */
         mTBOX_TRACE((kTBOX_NIV_WARNING,"%s(%#x,%d) ERROR: "
                      "Write Executable Confirmation File {%#x,%#x} "
                      "failure (write %d/%d)\n",
                      TBOX_FunctionName,xui32ICSModuleId,(int)xIsExecuted,
                      vNVM_IBTLi_stAreaLocExeConfirmation.addressOffset,
                      vNVM_IBTLi_stAreaLocExeConfirmation.size,
                      n,file.i32Size));
         rc = eNVM_IBTL_ERROR_WRITE;
      }
   }

   NVM_IBTLi_FileClose(&file);

   mTBOX_RETURN(rc);
}

/**
 *  @brief
 *    Return from the NVM Update Request File the update (download) request
 *    (application class id + update reason) with the highest reason priority
 *    in order new version of  application detected, corruption and update forced
 *    and external.
 *
 *  @param[out] pxstUpdRequest
 *       Pointer on the highest request with the class if (see defines
 *       kNVM_IBTL_CLASS_ID_xxx) and the update reason (see defines
 *       kNVM_IBTL_UPDATE_REASON_xxx).
 *
 *  @retval eNVM_IBTL_NO_ERROR
 *       Open successful.
 *
 *  @retval eNVM_IBTL_ERROR_MEMORY
 *       Content allocation failure.
 *
 *  @retval eNVM_IBTL_ERROR_READ
 *       Read file failure.
 *
 *  @retval eNVM_IBTL_ERROR_BAD_VERSION
 *       Invalid file version.
 *
 *  @retval eNVM_IBTL_ERROR_BAD_CRC
 *       Invalid file CRC.
 *
 *  @retval eNVM_IBTL_ERROR_NOT_FOUND
 *       Highest Update request not found (file empty, no request stored).
 *
 *  @retval eNVM_IBTL_ERROR
 *       Bad parameters or module not initialized.
 *
 *  @remarks
 *    -# If an read error or a invalid value read (version or CRC),
 *       set the default values in this file and return an error.
*/
tNVM_IBTL_Status NVM_IBTL_GetUpdateRequest
(
   tNVM_IBTL_UpdateRequest *pxstUpdRequest
)
{
   tNVM_IBTL_Status rc;

   mTBOX_FCT_ENTER(NVM_IBTL_GetUpdateRequest);

   if (pxstUpdRequest == NULL)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"%s(...) ERROR: "
                   "Pointer on update request undefined\n",
                   TBOX_FunctionName));
      mTBOX_RETURN(eNVM_IBTL_ERROR);
   }

#if defined (__DEBUG_TRACE__)
   rc = NVM_IBTLi_AccessUpdReq(TBOX_FunctionName,TRUE,pxstUpdRequest,FALSE,NULL);
#else
   rc = NVM_IBTLi_AccessUpdReq(TRUE,pxstUpdRequest,FALSE,NULL);
#endif

   mTBOX_RETURN(rc);
}

/**
 *  @brief
 *    Modify the update reason associed to a class ID in the NVM Update Request
 *    File .
 *
 *  @param[in] pxstUpdRequest
 *       Pointer on request contain the class ID to modify with its
 *       new update reason.
 *       To set the same reason to all requests of list use the class ID
 *       kNVM_IBTL_CLASS_ID_ALL.
 *
 *  @retval eNVM_IBTL_NO_ERROR
 *       Open successful.
 *
 *  @retval eNVM_IBTL_ERROR_MEMORY
 *       Content allocation failure.
 *
 *  @retval eNVM_IBTL_ERROR_READ
 *       Read file failure.
 *
 *  @retval eNVM_IBTL_ERROR_WRITE
 *       Write file failure.
 *
 *  @retval eNVM_IBTL_ERROR_BAD_VERSION
 *       Invalid file version.
 *
 *  @retval eNVM_IBTL_ERROR_BAD_CRC
 *       Invalid file CRC.
 *
 *  @retval eNVM_IBTL_ERROR_NOT_FOUND
 *       Request to update not found.
 *
 *  @retval eNVM_IBTL_ERROR
 *       Bad parameters or module not initialized.
 *
 *  @remarks
 *    -# If an read error or a invalid value read (version or CRC),
 *       set the default values in this file and return an error
 *       except in mode 'set' when the write is a success.
*/
tNVM_IBTL_Status NVM_IBTL_SetUpdateRequest
(
   tNVM_IBTL_UpdateRequest *pxstUpdRequest
)
{
   tNVM_IBTL_Status rc;

   mTBOX_FCT_ENTER(NVM_IBTL_SetUpdateRequest);

   if (pxstUpdRequest == NULL)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"%s(...) ERROR: "
                   "Pointer on update request undefined\n",
                   TBOX_FunctionName));
      mTBOX_RETURN(eNVM_IBTL_ERROR);
   }

#if defined (__DEBUG_TRACE__)
   rc = NVM_IBTLi_AccessUpdReq(TBOX_FunctionName,FALSE,pxstUpdRequest,FALSE,NULL);
#else
   rc = NVM_IBTLi_AccessUpdReq(FALSE,pxstUpdRequest,FALSE,NULL);
#endif

   mTBOX_RETURN(rc);
}

/**
 *  @brief
 *    Return the RF Modulation setting from the NVM Update Request File.
 *
 *  @param[out] pxstRFMod
 *       Pointer on the RF Modulation setting read.
 *
 *  @retval eNVM_IBTL_NO_ERROR
 *       Open successful.
 *
 *  @retval eNVM_IBTL_ERROR_MEMORY
 *       Content allocation failure.
 *
 *  @retval eNVM_IBTL_ERROR_READ
 *       Read file failure.
 *
 *  @retval eNVM_IBTL_ERROR_BAD_VERSION
 *       Invalid file version.
 *
 *  @retval eNVM_IBTL_ERROR_BAD_CRC
 *       Invalid file CRC.
 *
 *  @retval eNVM_IBTL_ERROR
 *       Bad parameters or module not initialized.
 *
 *  @remarks
 *    -# If not found or a read error or a invalid value read
 *       (version or CRC), a RF Modulation setting is returned
 *       with the actives flags set to FALSE.
*/
tNVM_IBTL_Status NVM_IBTL_GetRFModulation
(
   tNVM_IBTL_RFModulation *pxstRFMod
)
{
   tNVM_IBTL_Status rc = eNVM_IBTL_ERROR;

   mTBOX_FCT_ENTER(NVM_IBTL_GetRFModulation);

   if (pxstRFMod == NULL)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"%s(...) ERROR: "
                   "Pointer on RF modulation undefined\n",
                   TBOX_FunctionName));
      mTBOX_RETURN(eNVM_IBTL_ERROR);
   }

#if defined (__DEBUG_TRACE__)
   rc = NVM_IBTLi_AccessUpdReq(TBOX_FunctionName,FALSE,NULL,FALSE,pxstRFMod);
#else
   rc = NVM_IBTLi_AccessUpdReq(FALSE,NULL,FALSE,pxstRFMod);
#endif

   mTBOX_RETURN(rc);
}

/**
 *  @brief
 *    Write the RF Modulation setting in the NVM Update Request File.
 *
 *  @param[in] pxstRFMod
 *       Pointer on the RF Modulation setting read.
 *
 *  @retval eNVM_IBTL_NO_ERROR
 *       Open successful.
 *
 *  @retval eNVM_IBTL_ERROR_MEMORY
 *       Content allocation failure.
 *
 *  @retval eNVM_IBTL_ERROR_READ
 *       Read file failure.
 *
 *  @retval eNVM_IBTL_ERROR_WRITE
 *       Write file failure.
 *
 *  @retval eNVM_IBTL_ERROR_BAD_VERSION
 *       Invalid file version.
 *
 *  @retval eNVM_IBTL_ERROR_BAD_CRC
 *       Invalid file CRC.
 *
 *  @retval eNVM_IBTL_ERROR
 *       Bad parameters or module not initialized.
 *
 *  @remarks
 *    -# If not found or a read error or a invalid value read
 *       (version or CRC), a RF Modulation setting is returned
 *       with the actives flags set to FALSE.
*/
tNVM_IBTL_Status NVM_IBTL_SetRFModulation
(
   tNVM_IBTL_RFModulation *pxstRFMod
)
{
   tNVM_IBTL_Status rc = eNVM_IBTL_ERROR;

   mTBOX_FCT_ENTER(NVM_IBTL_SetRFModulation);

   if (pxstRFMod == NULL)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"%s(...) ERROR: "
                   "Pointer on RF modulation undefined\n",
                   TBOX_FunctionName));
      mTBOX_RETURN(eNVM_IBTL_ERROR);
   }

#if defined (__DEBUG_TRACE__)
   rc = NVM_IBTLi_AccessUpdReq(TBOX_FunctionName,FALSE,NULL,TRUE,pxstRFMod);
#else
   rc = NVM_IBTLi_AccessUpdReq(FALSE,NULL,TRUE,pxstRFMod);
#endif

   mTBOX_RETURN(rc);
}

/**
 *  @brief
 *    Return from the NVM Application Version file the application version
 *    descriptor identified by its ICS identifier given.
 *
 *  @pre
 *    None.
 *
 *  @post
 *    None.
 *
 *  @param[in] xbTheGreatestUpdateID
 *       Set to TRUE to return the ICS module with the greatest
 *       request id (the ICS identifier is not significant).
 *       Set to FALSE to return the ICS module identified by
 *       the next parameter xui32ICSModuleId.
 *
 *  @param[in] xui32ICSModuleId
 *       The Identifier of the ICS module requested (see defines
 *       kNVM_IBTL_ICS_MODID_xxx).
 *
 *  @param[out] pxstAppliVersion
 *       Pointer on the application version descriptor found.
 *
 *  @retval eNVM_IBTL_NO_ERROR
 *       Open successful.
 *
 *  @retval eNVM_IBTL_ERROR_MEMORY
 *       Content allocation failure.
 *
 *  @retval eNVM_IBTL_ERROR_READ
 *       Read file failure.
 *
 *  @retval eNVM_IBTL_ERROR_BAD_VERSION
 *       Invalid file version.
 *
 *  @retval eNVM_IBTL_ERROR_BAD_CRC
 *       Invalid file CRC.
 *
 *  @retval eNVM_IBTL_ERROR_NOT_FOUND
 *       ICS module not found.
 *
 *  @retval eNVM_IBTL_ERROR
 *       Bad parameters or module not initialized.
 *
 *  @remarks
 *    -# If search the greatest update id and no module is updated
 *       return the factory update ID kNVM_IBTL_UPDATE_ID_FACTORY
 *       with a ICS module ID equal to kNVM_IBTL_ICS_MODID_NONE.
*/
tNVM_IBTL_Status NVM_IBTL_GetApplicationVersion
(
   bool                          xbTheGreatestUpdateID,
   uint32_t                      xui32ICSModuleId,
   tNVM_IBTL_ApplicationVersion *pxstAppliVersion
)
{
   tNVM_IBTL_Status rc      = eNVM_IBTL_NO_ERROR;
   tNVM_IBTLi_File  file    = kNVM_IBTLi_FILE_NULL;
   uint32_t         version,n_mod,crc32,calc_crc,i;
   uint32_t         ics_id,class_id,upd_id,flags,curr_updid;
   int32_t          offset,n;
   uint8_t 				software_version_label[kNVM_IBTLi_FAV_SV_LABEL_LENGTH+1];
   uint8_t 				software_date_label[kNVM_IBTLi_FAV_SD_LABEL_LENGTH+1];


   mTBOX_FCT_ENTER(NVM_IBTL_GetApplicationVersion);

   if (!vNVM_IBTLi_isInit)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"%s(%#x=%s,...) ERROR: "
                   "NVM Interface with bootloader is not initialized\n",
                   TBOX_FunctionName,xui32ICSModuleId,
                   NVM_IBTL_DEBUG_LabelModuleID(xui32ICSModuleId)));
      mTBOX_RETURN(eNVM_IBTL_ERROR);
   }
   if (pxstAppliVersion == NULL)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"%s(%#x=%s,...) ERROR: "
                   "Pointer on update request undefined\n",
                   TBOX_FunctionName,xui32ICSModuleId,
                   NVM_IBTL_DEBUG_LabelModuleID(xui32ICSModuleId)));
      mTBOX_RETURN(eNVM_IBTL_ERROR);
   }

#ifndef CHANGHONG_BOOTLOADER
   tNVM_IBTL_DialogArea pxstDialogArea;
   rc = NVM_IBTLi_AccessDialogArea(TRUE, &pxstDialogArea);
   if( rc == eNVM_IBTL_NO_ERROR)
   {
	   pxstAppliVersion->ui32UpdateId = pxstDialogArea.stDLCycle.u32UpdateID;
   }
   else
#endif /* Changhong dialog area does not keep version info */
   {
	   pxstAppliVersion->ui32UpdateId = kNVM_IBTL_UPDATE_ID_FACTORY;
   }

   mTBOX_RETURN(rc);
}


bool NVM_IBTL_ResetApplicationVersion()
{
#ifndef CHANGHONG_BOOTLOADER

    tNVM_IBTL_DialogArea pxstDialogArea;
    tNVM_IBTL_Status rc;
    bool bRC;
    tNVM_IBTL_DialogArea stLocalDialogArea;

    rc = NVM_IBTLi_AccessDialogArea(TRUE, &pxstDialogArea) ;

    if( rc == eNVM_IBTL_NO_ERROR)
    {
        pxstDialogArea.stDLCycle.u32UpdateID = kNVM_IBTL_UPDATE_ID_FACTORY;

       rc = NVM_IBTLi_AccessDialogArea(FALSE, &pxstDialogArea) ;
    }
    else
    {
        return false;
    }
#endif /* Changhong dialog area does not keep version info */

    return true;



}

#if defined (__DEBUG_TRACE__)
/**
 *  Debug functions for integration
*/
char *NVM_IBTL_DEBUG_LabelClassID (uint32_t xui32Value)
{
   static char *__nvm_ibtl_label_class_id__[9] =
   {
      "NONE",
      "SSA DA2",
      "CONFIG FILE",
      "ENGINEERING",
      "DOWNLOADER",
      "USER",
      "ASSET",
      "ALL",
      "Invalid"
   };
   uint8_t i;

   switch (xui32Value)
   {
   case kNVM_IBTL_CLASS_ID_NONE:        i = 0; break;
   case kNVM_IBTL_CLASS_ID_SSA_DA2:     i = 1; break;
   case kNVM_IBTL_CLASS_ID_CONFIG_FILE: i = 2; break;
   case kNVM_IBTL_CLASS_ID_ENGINEERING: i = 3; break;
   case kNVM_IBTL_CLASS_ID_DOWNLOADER:  i = 4; break;
   case kNVM_IBTL_CLASS_ID_USER:        i = 5; break;
   case kNVM_IBTL_CLASS_ID_ASSET:       i = 6; break;
   case kNVM_IBTL_CLASS_ID_ALL:         i = 7; break;
   default:                             i = 8;
   }

   return __nvm_ibtl_label_class_id__[i];
}

char *NVM_IBTL_DEBUG_LabelReason (uint32_t xui32Value)
{
   static char *__nvm_ibtl_label_reason__[6] =
   {
      "NONE",
      "NEW VERSION",
      "CORRUPTION",
      "FORCED",
      "EXTERNAL",
      "Invalid"
   };
   uint8_t i;

   switch (xui32Value)
   {
   case kNVM_IBTL_UPDATE_REASON_NONE:        i = 0; break;
   case kNVM_IBTL_UPDATE_REASON_NEW_VERSION: i = 1; break;
   case kNVM_IBTL_UPDATE_REASON_CORRUPTION:  i = 2; break;
   case kNVM_IBTL_UPDATE_REASON_FORCED:      i = 3; break;
   case kNVM_IBTL_UPDATE_REASON_EXTERNAL:    i = 4; break;
   default:                                  i = 5;
   }

   return __nvm_ibtl_label_reason__[i];
}

char *NVM_IBTL_DEBUG_LabelModuleID (uint32_t xui32Value)
{
   static char *__nvm_ibtl_label_module_id__[17] =
   {
      "NONE",
      "FACTORY DOWNLOADER",
      "FACTORY CONFIG FILE",
      "DOWNLOADER",
      "EMS CONFIG ILE",
      "EMS REDUNDANT CONFIG FILE",
      "APPLICATION USER 0",
      "APPLICATION USER 1",
      "APPLICATION USER 2",
      "APPLICATION ENG 0",
      "APPLICATION ENG 1",
      "APPLICATION ENG 2",
      "ASSET USER 0",
      "ASSET USER 1",
      "ASSET USER 2",
      "SSA DA2",
      "Invalid"
   };
   uint8_t i;

   switch (xui32Value)
   {
   case kNVM_IBTL_ICS_MODID_NONE:                      i =  0; break;
   case kNVM_IBTL_ICS_MODID_FACTORY_DOWNLOADER:        i =  1; break;
   case kNVM_IBTL_ICS_MODID_FACTORY_CONFIG_FILE:       i =  2; break;
   case kNVM_IBTL_ICS_MODID_DOWNLOADER:                i =  3; break;
   case kNVM_IBTL_ICS_MODID_EMS_CONFIG_FILE:           i =  4; break;
   case kNVM_IBTL_ICS_MODID_EMS_REDUNDANT_CONFIG_FILE: i =  5; break;
   case kNVM_IBTL_ICS_MODID_EXE_APPLICATION_USER_0:    i =  6; break;
   case kNVM_IBTL_ICS_MODID_EXE_APPLICATION_USER_1:    i =  7; break;
   case kNVM_IBTL_ICS_MODID_EXE_APPLICATION_USER_2:    i =  8; break;
   case kNVM_IBTL_ICS_MODID_EXE_APPLICATION_ENG_0:     i =  9; break;
   case kNVM_IBTL_ICS_MODID_EXE_APPLICATION_ENG_1:     i = 10; break;
   case kNVM_IBTL_ICS_MODID_EXE_APPLICATION_ENG_2:     i = 11; break;
   case kNVM_IBTL_ICS_MODID_EXE_ASSET_USER_0:          i = 12; break;
   case kNVM_IBTL_ICS_MODID_EXE_ASSET_USER_1:          i = 13; break;
   case kNVM_IBTL_ICS_MODID_EXE_ASSET_USER_2:          i = 14; break;
   case kNVM_IBTL_ICS_MODID_SSA_DA2:                   i = 15; break;
   default:                                            i = 16;
   }

   return __nvm_ibtl_label_module_id__[i];
}

char *NVM_IBTL_DEBUG_LabelRFModStandard (uint8_t xui8Value)
{
   static char *__nvm_ibtl_label_rfmod_standard__[10] =
   {
      "BG",
      "DK",
      "H",
      "I",
      "K",
      "L",
      "M",
      "N",
      "B",
      "Invalid"
   };
   uint8_t i;

   switch (xui8Value)
   {
   case kNVM_IBTL_RFMOD_STANDARD_BG: i = 0; break;
   case kNVM_IBTL_RFMOD_STANDARD_DK: i = 1; break;
   case kNVM_IBTL_RFMOD_STANDARD_H:  i = 2; break;
   case kNVM_IBTL_RFMOD_STANDARD_I:  i = 3; break;
   case kNVM_IBTL_RFMOD_STANDARD_K:  i = 4; break;
   case kNVM_IBTL_RFMOD_STANDARD_L:  i = 5; break;
   case kNVM_IBTL_RFMOD_STANDARD_M:  i = 6; break;
   case kNVM_IBTL_RFMOD_STANDARD_N:  i = 7; break;
   case kNVM_IBTL_RFMOD_STANDARD_B:  i = 8; break;
   default:                          i = 9;
   }

   return __nvm_ibtl_label_rfmod_standard__[i];
}

char *NVM_IBTL_DEBUG_LabelRFModBand (uint8_t xui8Value)
{
   static char *__nvm_ibtl_label_rfmod_band__[4] =
   {
      "VH1",
      "VH2",
      "UHF",
      "Invalid"
   };
   uint8_t i;

   switch (xui8Value)
   {
   case kNVM_IBTL_RFMOD_BAND_VHF1: i = 0; break;
   case kNVM_IBTL_RFMOD_BAND_VHF2: i = 1; break;
   case kNVM_IBTL_RFMOD_BAND_UHF:  i = 2; break;
   default:                        i = 3;
   }

   return __nvm_ibtl_label_rfmod_band__[i];
}

char *NVM_IBTL_DEBUG_LabelCVBSEncoding (uint8_t xui8Value)
{
   static char *__nvm_ibtl_label_cvbs_encoding__[9] =
   {
      "NTSC",
      "NTSC-J",
      "NTSC-443",
      "PAL BGHDI",
      "PAL M",
      "PAL N",
      "PAL N-C",
      "SECAM",
      "Invalid"
   };
   uint8_t i;

   switch (xui8Value)
   {
   case kNVM_IBTL_RFMOD_CVBS_ENCOD_NTSC:      i = 0; break;
   case kNVM_IBTL_RFMOD_CVBS_ENCOD_NTSC_J:    i = 1; break;
   case kNVM_IBTL_RFMOD_CVBS_ENCOD_NTSC_443:  i = 2; break;
   case kNVM_IBTL_RFMOD_CVBS_ENCOD_PAL_BGHDI: i = 3; break;
   case kNVM_IBTL_RFMOD_CVBS_ENCOD_PAL_M:     i = 4; break;
   case kNVM_IBTL_RFMOD_CVBS_ENCOD_PAL_N:     i = 5; break;
   case kNVM_IBTL_RFMOD_CVBS_ENCOD_PAL_N_C:   i = 6; break;
   case kNVM_IBTL_RFMOD_CVBS_ENCOD_SECAM:     i = 7; break;
   default:                                   i = 8;
   }

   return __nvm_ibtl_label_cvbs_encoding__[i];
}
#endif


/******************************************************************************
* Locales Functions Implementations
******************************************************************************/

/**
 *  @brief
 *    Manage the get or set in the NVM Update Request File the update request
 *    and/or the RF modulation setting.
 *
 *
 *  @param[in] xisGetHighest
 *       If TRUE, return the highest update request (request with the
 *       high reason)
 *       If FALSE, update a request with the reason set in next parameter
 *       Significant only if the update request pointer is defined
 *
 *  @param[in/out] pxstUpdRequest
 *       If xisGetHighest=TRUE, output parameter, pointer on the highest request
 *       with the class and the update reason.
 *       If xisGetHighest=FALSE, input parameter, pointer update request to
 *       modify reason(classID)=update reason.
 *
 *  @param[in] xisSetRFMod
 *       If TRUE, update the RF modulation with the new setting
 *       If FALSE, return the RF modulation read in the RF modulation pointer
 *       Significant only if the RF modulation pointer is defined
 *
 *  @param[in/out] pxstRFMod
 *       If xisSetRFMod=TRUE, input parameter, pointer on new RF modulation
 *       stetting to write.
 *       If xisSetRFMod=FALSE, output parameter, pointer on the RF Modulation
 *       setting read.
 *
 *  @retval eNVM_IBTL_NO_ERROR
 *       Open successful.
 *
 *  @retval eNVM_IBTL_ERROR_MEMORY
 *       Content allocation failure.
 *
 *  @retval eNVM_IBTL_ERROR_READ
 *       Read file failure.
 *
 *  @retval eNVM_IBTL_ERROR_WRITE
 *       Write file failure.
 *
 *  @retval eNVM_IBTL_ERROR_BAD_VERSION
 *       Invalid file version.
 *
 *  @retval eNVM_IBTL_ERROR_BAD_CRC
 *       Invalid file CRC.
 *
 *  @retval eNVM_IBTL_ERROR_NOT_FOUND
 *       Update request not found.
 *
 *  @retval eNVM_IBTL_ERROR
 *       Bad parameters or module not initialized.
 *
 *  @remarks
 *    -# If an read error or a invalid value read (version or CRC),
 *       set the default values in this file and return an error
 *       except in mode 'set' when the write is a success.
*/
tNVM_IBTL_Status NVM_IBTLi_AccessUpdReq
(
#if defined (__DEBUG_TRACE__)
   const char              *TBOX_FunctionName,
#endif
   bool                     xisGetHighest,
   tNVM_IBTL_UpdateRequest *pxstUpdRequest,
   bool                     xisSetRFMod,
   tNVM_IBTL_RFModulation  *pxstRFMod
)
{
   tNVM_IBTL_Status rc      = eNVM_IBTL_NO_ERROR;
   tNVM_IBTLi_File  file    = kNVM_IBTLi_FILE_NULL;
   bool             rewrite = FALSE;
   uint32_t         version,class_id,upd_reason,crc32,calc_crc;
   /* bootloder HNT fgv0.0.3 use n32_class on 32 bits instead of n_class on 8 bits */
   uint32_t         n32_class;
   uint8_t          i,i_found,i_free;
   int32_t          offset,n;
   bool             rfmod_act = kNVM_IBTL_RFMOD_ACTIVE_DEFAULT;
   bool             cvbs_act  = kNVM_IBTL_CVBS_ACTIVE_DEFAULT;
   bool             umcs6_act = kNVM_IBTL_UMCS6_ACTIVE_DEFAULT;
   uint8_t          chann     = kNVM_IBTL_RFMOD_CHANNEL_DEFAULT;
   uint8_t          std       = kNVM_IBTL_RFMOD_STANDARD_DEFAULT;
   uint8_t          band      = kNVM_IBTL_RFMOD_BAND_DEFAULT;
   uint8_t          cvbs      = kNVM_IBTL_RFMOD_CVBS_DEFAULT;
   uint8_t				reserved;

   if (!vNVM_IBTLi_isInit)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"%s(...) ERROR: "
                   "NVM Interface with bootloader is not initialized\n",
                   TBOX_FunctionName));
      mTBOX_RETURN(eNVM_IBTL_ERROR);
   }
   if ((pxstUpdRequest == NULL) && (pxstRFMod == NULL))
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"%s(...) ERROR: "
                   "All in/out pointers undefined\n",
                   TBOX_FunctionName));
      mTBOX_RETURN(eNVM_IBTL_ERROR);
   }

   /* Initialization update request management */
   if (pxstUpdRequest != NULL)
   {
      if (xisGetHighest)
      {
         mTBOX_TRACE((kTBOX_NIV_1,
                      "[%s] Get highest update request from File {%#x,%#x}\n",
                      TBOX_FunctionName,
                      vNVM_IBTLi_stAreaLocUpdateRequest.addressOffset,
                      vNVM_IBTLi_stAreaLocUpdateRequest.size));
         pxstUpdRequest->ui32ClassId      = kNVM_IBTL_CLASS_ID_NONE;
         pxstUpdRequest->ui32UpdateReason = kNVM_IBTL_UPDATE_REASON_NONE;
      }
      else
      {
         /* Check value to set */
         switch (pxstUpdRequest->ui32ClassId)
         {
         case kNVM_IBTL_CLASS_ID_SSA_DA2:
         case kNVM_IBTL_CLASS_ID_CONFIG_FILE:
         case kNVM_IBTL_CLASS_ID_ENGINEERING:
         case kNVM_IBTL_CLASS_ID_DOWNLOADER:
         case kNVM_IBTL_CLASS_ID_USER:
         case kNVM_IBTL_CLASS_ID_ASSET:
         case kNVM_IBTL_CLASS_ID_ALL:
            /* Okay */
            break;
         default:
            mTBOX_TRACE((kTBOX_NIV_CRITICAL,"%s(%#x,%#x) ERROR: "
                         "Invalid class ID\n",
                         TBOX_FunctionName,pxstUpdRequest->ui32ClassId,
                         pxstUpdRequest->ui32UpdateReason));
            mTBOX_RETURN(eNVM_IBTL_ERROR);
         }
         switch (pxstUpdRequest->ui32UpdateReason)
         {
         case kNVM_IBTL_UPDATE_REASON_NONE:
         case kNVM_IBTL_UPDATE_REASON_NEW_VERSION:
         case kNVM_IBTL_UPDATE_REASON_CORRUPTION:
         case kNVM_IBTL_UPDATE_REASON_FORCED:
         case kNVM_IBTL_UPDATE_REASON_EXTERNAL:
            /* Okay */
            break;
         default:
            mTBOX_TRACE((kTBOX_NIV_CRITICAL,"%s(%#x,%#x) ERROR: "
                         "Invalid update reason\n",
                         TBOX_FunctionName,pxstUpdRequest->ui32ClassId,
                         pxstUpdRequest->ui32UpdateReason));
            mTBOX_RETURN(eNVM_IBTL_ERROR);
         }

         mTBOX_TRACE((kTBOX_NIV_1,
                      "[%s] Set update request '%s' %#x = %#x (=%s) "
                      "in File {%#x,%#x}\n",
                      TBOX_FunctionName,
                      NVM_IBTL_DEBUG_LabelClassID(pxstUpdRequest->ui32ClassId),
                      pxstUpdRequest->ui32ClassId,
                      pxstUpdRequest->ui32UpdateReason,
                      NVM_IBTL_DEBUG_LabelReason(pxstUpdRequest->ui32UpdateReason),
                      vNVM_IBTLi_stAreaLocUpdateRequest.addressOffset,
                      vNVM_IBTLi_stAreaLocUpdateRequest.size));
      }
   }

   /* Open the file */
   rc = NVM_IBTLi_FileOpen(&file,&vNVM_IBTLi_stAreaLocUpdateRequest,
                           kNVM_IBTLi_FUR_FILE_SIZE);

   if (rc != eNVM_IBTL_NO_ERROR)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"%s() ERROR: "
                   "Open Update Requests File {%#x,%#x} failure "
                   "(error %d, size %u bytes)\n",
                   TBOX_FunctionName,
                   vNVM_IBTLi_stAreaLocUpdateRequest.addressOffset,
                   vNVM_IBTLi_stAreaLocUpdateRequest.size,rc,
                   kNVM_IBTLi_FUR_FILE_SIZE));
      mTBOX_RETURN(rc);
   }

   /* Read in internal buffer allocated at the open */
   n = NVM_IBTLi_FileRead(&file,NULL,-1);

   if (n != file.i32Size)
   {
      /* Warning set default value and continue */
      mTBOX_TRACE((kTBOX_NIV_WARNING,"%s() ERROR: "
                   "Read Update Requests File {%#x,%#x} "
                   "failure (read %d/%d)\n",
                   TBOX_FunctionName,
                   vNVM_IBTLi_stAreaLocUpdateRequest.addressOffset,
                   vNVM_IBTLi_stAreaLocUpdateRequest.size,
                   n,file.i32Size));
      rc = eNVM_IBTL_ERROR_READ;
   }
   else
   {
#if defined (__NVM_IBTL_DUMP_FILE__)
      NVM_IBTLi_DumpFile(file.pui8Content,file.i32Size,
                         &vNVM_IBTLi_stAreaLocUpdateRequest,
                         TBOX_FunctionName,"Update Request File","READ");
#endif

      /**
       * Init scanning of the internal buffer allocated and read
       * Start CRC32 calculation on part header
      */
      offset   = 0;
      i_found  = 0xFF;
      i_free   = 0xFF;

      /* Extract header */
      mMSBF_GET32_O(file.pui8Content,offset,version);
      mMSBF_GET32_O(file.pui8Content,offset,n32_class);
      mTBOX_TRACE((kTBOX_NIV_3,"[%s] - UPDATE REQUESTS (%d bytes):\n",
                   TBOX_FunctionName,file.i32Size));
      mTBOX_TRACE((kTBOX_NIV_3,"- version...............%#x\n",version));
      mTBOX_TRACE((kTBOX_NIV_3,"- number_of_class.......%u\n",n32_class));

      /* Check the version */
      if (version != kNVM_IBTLi_FUR_CURR_VERSION)
      {
         /* Warning, continue to set default values */
         mTBOX_TRACE((kTBOX_NIV_WARNING,"%s() ERROR: "
                      "Version %#x of Update Requests "
                      "File {%#x,%#x} invalid (wait %#x)\n",
                      TBOX_FunctionName,version,
                      vNVM_IBTLi_stAreaLocUpdateRequest.addressOffset,
                      vNVM_IBTLi_stAreaLocUpdateRequest.size,
                      kNVM_IBTLi_FUR_CURR_VERSION));
         rc = eNVM_IBTL_ERROR_BAD_VERSION;
      }
      else if (n32_class != kNVM_IBTLi_FUR_UREQ_NUMBER)
      {
         mTBOX_TRACE((kTBOX_NIV_WARNING,"%s() ERROR: "
                      "Number of class %d of Update Requests "
                      "File {%#x,%#x} invalid (wait %u)\n",
                      TBOX_FunctionName,n32_class,
                      vNVM_IBTLi_stAreaLocUpdateRequest.addressOffset,
                      vNVM_IBTLi_stAreaLocUpdateRequest.size,
                      kNVM_IBTLi_FUR_UREQ_NUMBER));
         rc = eNVM_IBTL_ERROR_BAD_VERSION;
      }
   }

   /* Parse the table of updates request */
   i = 0;
   while ((rc == eNVM_IBTL_NO_ERROR) && (i < n32_class))
   {
      /* Extract request */
      mMSBF_GET32_O(file.pui8Content,offset,class_id);
      mMSBF_GET32_O(file.pui8Content,offset,upd_reason);
      mTBOX_TRACE((kTBOX_NIV_3,"- Req[%02u].class_id......%#x (%s)\n",
                   i,class_id,
                   NVM_IBTL_DEBUG_LabelClassID(class_id)));
      mTBOX_TRACE((kTBOX_NIV_3,"- Req[%02u].update_reason.%#x (%s)\n",
                   i,upd_reason,
                   NVM_IBTL_DEBUG_LabelReason(upd_reason)));

      /* Perform update request case of access type */
      if (pxstUpdRequest != NULL)
      {
         if (xisGetHighest)
         {
            if ((class_id != kNVM_IBTL_CLASS_ID_NONE) &&
                (upd_reason != kNVM_IBTL_UPDATE_REASON_NONE) &&
                ((pxstUpdRequest->ui32UpdateReason == kNVM_IBTL_UPDATE_REASON_NONE) ||
                 (upd_reason < pxstUpdRequest->ui32UpdateReason)))
            {
               /* higher priority */
               pxstUpdRequest->ui32ClassId      = class_id;
               pxstUpdRequest->ui32UpdateReason = upd_reason;
               i_found = i;
            }
         }
         else if (pxstUpdRequest->ui32ClassId == class_id)
         {
            /* Class to set found */
            i_found = i;
         }
         else if (class_id == kNVM_IBTL_CLASS_ID_NONE)
         {
            /* Free element found */
            i_free = i;
         }
      }

      i++;
   }

   /**
    *  Parse the RF Modulation and CVBS
   */
   if (rc == eNVM_IBTL_NO_ERROR)
   {
      mMSBF_GET08_O(file.pui8Content,offset,i);

         rfmod_act = ((i & 0x80) == 0x80);
         cvbs_act  = ((i & 0x40) == 0x40);
         mMSBF_GET08_O(file.pui8Content,offset,chann);
         mMSBF_GET08_O(file.pui8Content,offset,std);
         mMSBF_GET08_O(file.pui8Content,offset,band);
         mMSBF_GET08_O(file.pui8Content,offset,cvbs);
#ifdef CVBS_ON_4_BYTES
{
         mMSBF_GET08_O(file.pui8Content,offset,reserved);
         mMSBF_GET08_O(file.pui8Content,offset,reserved);
         mMSBF_GET08_O(file.pui8Content,offset,reserved);
}
#endif
         mTBOX_TRACE((kTBOX_NIV_3,"- RF Modulation.........%s\n",
                     (rfmod_act)?"ACTIVE":"Inactive"));
         mTBOX_TRACE((kTBOX_NIV_3,"- CVBS..................%s\n",
                     (cvbs_act)?"ACTIVE":"Inactive"));
         mTBOX_TRACE((kTBOX_NIV_3,"- UMCS6..................%s\n",
                     (umcs6_act)?"ACTIVE":"Inactive"));
         mTBOX_TRACE((kTBOX_NIV_3,"- RF Mod Channel........%#x\n",
                      chann));
         mTBOX_TRACE((kTBOX_NIV_3,"- RF Mod Standard.......%#x (%s)\n",
                      std,NVM_IBTL_DEBUG_LabelRFModStandard(std)));
         mTBOX_TRACE((kTBOX_NIV_3,"- RF Mod Band...........%#x (%s)\n",
                      band,NVM_IBTL_DEBUG_LabelRFModBand(band)));
         mTBOX_TRACE((kTBOX_NIV_3,"- CVBS Encoding format..%#x (%s)\n",
                      cvbs,NVM_IBTL_DEBUG_LabelCVBSEncoding(cvbs)));
   }

   /* Parse and Check CRC32 */
   if (rc == eNVM_IBTL_NO_ERROR)
   {
      /* Compute crc */
      calc_crc = NVM_IBTLi_ComputeCRC(file.pui8Content,offset,0,TRUE);

      /* Extract crc */
      mMSBF_GET32_O(file.pui8Content,offset,crc32);
      mTBOX_TRACE((kTBOX_NIV_3,"- crc32.................%#x\n",crc32));

      /* Check the crc32 */
      if (crc32 != calc_crc)
      {
         /* Warning set default value and continue */
         mTBOX_TRACE((kTBOX_NIV_WARNING,"%s() ERROR: "
                      "CRC32 0x%08X of Update Requests "
                      "File {%#x,%#x} invalid (compute 0x%08X)\n",
                      TBOX_FunctionName,crc32,
                      vNVM_IBTLi_stAreaLocUpdateRequest.addressOffset,
                      vNVM_IBTLi_stAreaLocUpdateRequest.size,
                      calc_crc));
         rc = eNVM_IBTL_ERROR_BAD_CRC;
         /*
         mTBOX_TRACE((kTBOX_NIV_WARNING,"%s() ERROR: error ignored!!!\n", TBOX_FunctionName));
         rc = eNVM_IBTL_NO_ERROR;
         */
      }
   }

   /* Read done, perform expected access mode */
   if (rc != eNVM_IBTL_NO_ERROR)
   {
      /* Reform the file and rewrite it */
      mTBOX_TRACE((kTBOX_NIV_1,
                   "[%s] Error read or corruption => "
                   "Rewrite File with default values\n",
                   TBOX_FunctionName));
      offset = 0;
      mMSBF_SET32_O(file.pui8Content,offset,kNVM_IBTLi_FUR_CURR_VERSION);
      mMSBF_SET32_O(file.pui8Content,offset,kNVM_IBTLi_FUR_UREQ_NUMBER);
      mMSBF_SET32_O(file.pui8Content,offset,kNVM_IBTL_CLASS_ID_SSA_DA2);
      mMSBF_SET32_O(file.pui8Content,offset,kNVM_IBTL_UPDATE_REASON_NONE);
      mMSBF_SET32_O(file.pui8Content,offset,kNVM_IBTL_CLASS_ID_CONFIG_FILE);
      mMSBF_SET32_O(file.pui8Content,offset,kNVM_IBTL_UPDATE_REASON_NONE);
      mMSBF_SET32_O(file.pui8Content,offset,kNVM_IBTL_CLASS_ID_ENGINEERING);
      mMSBF_SET32_O(file.pui8Content,offset,kNVM_IBTL_UPDATE_REASON_NONE);
      mMSBF_SET32_O(file.pui8Content,offset,kNVM_IBTL_CLASS_ID_DOWNLOADER);
      mMSBF_SET32_O(file.pui8Content,offset,kNVM_IBTL_UPDATE_REASON_NONE);
      mMSBF_SET32_O(file.pui8Content,offset,kNVM_IBTL_CLASS_ID_USER);
      mMSBF_SET32_O(file.pui8Content,offset,kNVM_IBTL_UPDATE_REASON_NONE);
      mMSBF_SET32_O(file.pui8Content,offset,kNVM_IBTL_CLASS_ID_ASSET);
      mMSBF_SET32_O(file.pui8Content,offset,kNVM_IBTL_UPDATE_REASON_NONE);

      /* Set the good reason */
      if ((pxstUpdRequest != NULL) && (!xisGetHighest))
      {
         switch (pxstUpdRequest->ui32ClassId)
         {
         case kNVM_IBTL_CLASS_ID_SSA_DA2:
            i_found = 0;
            break;
         case kNVM_IBTL_CLASS_ID_CONFIG_FILE:
            i_found = 1;
            break;
         case kNVM_IBTL_CLASS_ID_ENGINEERING:
            i_found = 2;
            break;
         case kNVM_IBTL_CLASS_ID_DOWNLOADER:
            i_found = 3;
            break;
         case kNVM_IBTL_CLASS_ID_USER:
            i_found = 4;
            break;
         case kNVM_IBTL_CLASS_ID_ASSET:
            i_found = 5;
            break;
         default:
            i_found = 0xFF;
            break;
         }
      }

      /* Indicate the rewrite */
      rewrite = TRUE;
   }

   if ((rc == eNVM_IBTL_NO_ERROR) && (pxstUpdRequest != NULL))
   {
      /* Update request management */
      if (xisGetHighest)
      {
         if (i_found < kNVM_IBTLi_FUR_UREQ_NUMBER)
         {
            /* Okay found */
            mTBOX_TRACE((kTBOX_NIV_1,
                         "[%s] FOUND highest update request %u "
                         "(%#x,%#x) %s=%s\n",
                         TBOX_FunctionName,i_found,
                         pxstUpdRequest->ui32ClassId,
                         pxstUpdRequest->ui32UpdateReason,
                         NVM_IBTL_DEBUG_LabelClassID(pxstUpdRequest->ui32ClassId),
                         NVM_IBTL_DEBUG_LabelReason(pxstUpdRequest->ui32UpdateReason)));
            rewrite = FALSE;
         }
         else
         {
            mTBOX_TRACE((kTBOX_NIV_1,
                         "[%s] Highest update request NOT FOUND\n",
                         TBOX_FunctionName));
            rewrite = FALSE;
            rc      = eNVM_IBTL_ERROR_NOT_FOUND;
         }
      }
      else if (pxstUpdRequest->ui32ClassId == kNVM_IBTL_CLASS_ID_ALL)
      {
         /* Set same reason of all request ID */
         mTBOX_TRACE((kTBOX_NIV_1,
                      "[%s] Set all update request reason = %#x\n",
                      TBOX_FunctionName,pxstUpdRequest->ui32UpdateReason));
         offset = 0;
         mMSBF_SET32_O(file.pui8Content,offset,kNVM_IBTLi_FUR_CURR_VERSION);
         mMSBF_SET32_O(file.pui8Content,offset,kNVM_IBTLi_FUR_UREQ_NUMBER);
         mMSBF_SET32_O(file.pui8Content,offset,kNVM_IBTL_CLASS_ID_SSA_DA2);
         mMSBF_SET32_O(file.pui8Content,offset,pxstUpdRequest->ui32UpdateReason);
         mMSBF_SET32_O(file.pui8Content,offset,kNVM_IBTL_CLASS_ID_CONFIG_FILE);
         mMSBF_SET32_O(file.pui8Content,offset,pxstUpdRequest->ui32UpdateReason);
         mMSBF_SET32_O(file.pui8Content,offset,kNVM_IBTL_CLASS_ID_ENGINEERING);
         mMSBF_SET32_O(file.pui8Content,offset,pxstUpdRequest->ui32UpdateReason);
         mMSBF_SET32_O(file.pui8Content,offset,kNVM_IBTL_CLASS_ID_DOWNLOADER);
         mMSBF_SET32_O(file.pui8Content,offset,pxstUpdRequest->ui32UpdateReason);
         mMSBF_SET32_O(file.pui8Content,offset,kNVM_IBTL_CLASS_ID_USER);
         mMSBF_SET32_O(file.pui8Content,offset,pxstUpdRequest->ui32UpdateReason);
         mMSBF_SET32_O(file.pui8Content,offset,kNVM_IBTL_CLASS_ID_ASSET);
         mMSBF_SET32_O(file.pui8Content,offset,pxstUpdRequest->ui32UpdateReason);

         /* Indicate the rewrite */
         i_found = 0xFF;
         rewrite = TRUE;
      }
      else if (i_found < kNVM_IBTLi_FUR_UREQ_NUMBER)
      {
         rewrite = TRUE;
      }
      else if (i_free < kNVM_IBTLi_FUR_UREQ_NUMBER)
      {
         mTBOX_TRACE((kTBOX_NIV_1,
                      "[%s] Update request %#x not found, "
                      "use free request %u\n",
                      TBOX_FunctionName,
                      pxstUpdRequest->ui32ClassId,i_free));
         rewrite = TRUE;
         i_found = i_free;
      }
      else
      {
         /* Class id to set not found and no free element */
         mTBOX_TRACE((kTBOX_NIV_WARNING,"%s(%#x,%d) WARNING: "
                      "Impossible to set (%#x,%#x), "
                      "the table is full (%u elements)\n",
                      TBOX_FunctionName,pxstUpdRequest->ui32ClassId,
                      pxstUpdRequest->ui32UpdateReason,n32_class));
         rewrite = FALSE;
         rc = eNVM_IBTL_ERROR;
      }
   }

   if ((rc == eNVM_IBTL_NO_ERROR) && (pxstRFMod != NULL))
   {
      /* RF Mod management */
      if (xisSetRFMod)
      {
         /* Write the RF mod setting */
         rewrite   = true;
         rfmod_act = pxstRFMod->isActiveRFMod;
         cvbs_act  = pxstRFMod->isActiveCVBS;
         if (rfmod_act)
         {
            chann = pxstRFMod->ui8Channel;
            std   = pxstRFMod->ui8Standard;
            band  = pxstRFMod->ui8Band;
            mTBOX_TRACE((kTBOX_NIV_1,"[%s] Set RF modulation ACTIVE "
                         "=> channel=%#x, std=%#x=%s, band=%#x=%s\n",
                         TBOX_FunctionName,pxstRFMod->ui8Channel,
                         pxstRFMod->ui8Standard,
                         NVM_IBTL_DEBUG_LabelRFModStandard(pxstRFMod->ui8Standard),
                         pxstRFMod->ui8Band,
                         NVM_IBTL_DEBUG_LabelRFModBand(pxstRFMod->ui8Band)));
         }
         else
         {
            mTBOX_TRACE((kTBOX_NIV_1,"[%s] Set RF modulation DISABLE "
                         "(current value: channel=%#x, std=%#x=%s, band=%#x=%s)\n",
                         TBOX_FunctionName,chann,std,
                         NVM_IBTL_DEBUG_LabelRFModStandard(std),
                         band,NVM_IBTL_DEBUG_LabelRFModBand(band)));
         }

         if (rfmod_act)
         {
            cvbs = pxstRFMod->ui8EncodingCVBS;
            mTBOX_TRACE((kTBOX_NIV_1,"[%s] Set CVBS ACTIVE => encoding=%#x=%s\n",
                         TBOX_FunctionName,pxstRFMod->ui8EncodingCVBS,
                         NVM_IBTL_DEBUG_LabelCVBSEncoding(pxstRFMod->ui8EncodingCVBS)));
         }
         else
         {
            mTBOX_TRACE((kTBOX_NIV_1,"[%s] Set CVBS DISABLE "
                         "(current value: encoding=%#x=%s)\n",
                         TBOX_FunctionName,cvbs,
                         NVM_IBTL_DEBUG_LabelCVBSEncoding(cvbs)));
         }
      }
      else
      {
         /* Return RF modulation read */
         pxstRFMod->isActiveRFMod   = rfmod_act;
         pxstRFMod->isActiveCVBS    = cvbs_act;
         pxstRFMod->ui8Channel      = chann;
         pxstRFMod->ui8Standard     = std;
         pxstRFMod->ui8Band         = band;
         pxstRFMod->ui8EncodingCVBS = cvbs;
         mTBOX_TRACE((kTBOX_NIV_1,"[%s] Get RF modulation: "
                      "RFmod %s (channel=%#x std=%#x=%s band=%#x=%s), "
                      "CVBS %s (encoding=%#x=%s)\n",
                      TBOX_FunctionName,(pxstRFMod->isActiveRFMod)?"Active":"disable",
                      pxstRFMod->ui8Channel,pxstRFMod->ui8Standard,
                      NVM_IBTL_DEBUG_LabelRFModStandard(pxstRFMod->ui8Standard),
                      pxstRFMod->ui8Band,NVM_IBTL_DEBUG_LabelRFModBand(pxstRFMod->ui8Band),
                      (pxstRFMod->isActiveCVBS)?"Active":"disable",
                      pxstRFMod->ui8EncodingCVBS,
                      NVM_IBTL_DEBUG_LabelCVBSEncoding(pxstRFMod->ui8EncodingCVBS)));
      }
   }

   if (rewrite)
   {
      if ((pxstUpdRequest != NULL) &&
          (!xisGetHighest) && (i_found < kNVM_IBTLi_FUR_UREQ_NUMBER))
      {
         /* Update the request */
         offset = kNVM_IBTLi_FUR_HEAD_SIZE +
                  ((int32_t)i_found * kNVM_IBTLi_FUR_UREQ_SIZE);
         mTBOX_TRACE((kTBOX_NIV_1,
                      "[%s] Set update request %u %#x = %#x (offset %d)\n",
                      TBOX_FunctionName,i_found,
                      pxstUpdRequest->ui32ClassId,
                      pxstUpdRequest->ui32UpdateReason,
                      offset));
         mMSBF_SET32_O(file.pui8Content,offset,pxstUpdRequest->ui32ClassId);
         mMSBF_SET32_O(file.pui8Content,offset,pxstUpdRequest->ui32UpdateReason);

         /* If read error, but update success => NO ERROR */
         rc = eNVM_IBTL_NO_ERROR;
      }

      /* Set the RF mod */
      offset = kNVM_IBTLi_FUR_HEAD_SIZE + kNVM_IBTLi_FUR_UREQ_TABLE_SIZE;
      i = 0;
      if (rfmod_act)
      {
   	 	i |= 0x80;
   	} else {
   		chann=0xFF;
   		std=0xFF;
   		band=0xFF;
   	}

   	reserved = 0xFF;
      if (cvbs_act)
      {
      	i |= 0x40;
      } else {
      	cvbs = 0xFF;
      }
      if (umcs6_act)
      {
      	i |= 0x20;
      }

      mMSBF_SET08_O(file.pui8Content,offset,i);
      mMSBF_SET08_O(file.pui8Content,offset,chann);
      mMSBF_SET08_O(file.pui8Content,offset,std);
      mMSBF_SET08_O(file.pui8Content,offset,band);
      mMSBF_SET08_O(file.pui8Content,offset,cvbs);
#ifdef CVBS_ON_4_BYTES
      mMSBF_SET08_O(file.pui8Content,offset,reserved);
      mMSBF_SET08_O(file.pui8Content,offset,reserved);
      mMSBF_SET08_O(file.pui8Content,offset,reserved);
#endif
      /* Compute CRC and set it */
      calc_crc = NVM_IBTLi_ComputeCRC(file.pui8Content,offset,0,TRUE);
      mMSBF_SET32_O(file.pui8Content,offset,calc_crc);

      /* Write the new file content */
      n = NVM_IBTLi_FileWrite(&file,NULL,-1);

      if (n != file.i32Size)
      {
         /* Error */
         mTBOX_TRACE((kTBOX_NIV_WARNING,"%s() ERROR: "
                      "Write Update Requests File {%#x,%#x} failure "
                      "(write %d/%d, crc32=0x%08X, data size=%d)\n",
                      TBOX_FunctionName,
                      vNVM_IBTLi_stAreaLocUpdateRequest.addressOffset,
                      vNVM_IBTLi_stAreaLocUpdateRequest.size,
                      n,file.i32Size,calc_crc,offset));

         if (((!xisGetHighest) || (xisSetRFMod)) && (rc == eNVM_IBTL_NO_ERROR))
         {
            /* Set/Unset mode : Return a write error */
            rc = eNVM_IBTL_ERROR_WRITE;
         }
      }
      else
      {
         /* OKAY */
         mTBOX_TRACE((kTBOX_NIV_1,
                      "[%s] Table of requests (crc32=0x%08X) written "
                      "in Update Requests File {%#x,%#x} (data size=%d)\n",
                      TBOX_FunctionName,calc_crc,
                      vNVM_IBTLi_stAreaLocUpdateRequest.addressOffset,
                      vNVM_IBTLi_stAreaLocUpdateRequest.size,offset));
      }
   }

   /* Close the file */
   NVM_IBTLi_FileClose(&file);

   return(rc);
}

#ifdef NEWLAND_BOOTLOADER
/**
 *  @brief
 *    Manage the new trigger downloader dialog area
 *
 *
 *  @param[in] xisReadOrWrite
 *       If TRUE, return the current content
 *       If FALSE, update the content
 *
 *  @param[in/out] pxstDialogArea
 *       If xisReadOrWrite=TRUE, return the current content into pxstDialogArea
 *       If xisReadOrWrite=FALSE, update the content with pxstDialogArea
 *
 *  @retval eNVM_IBTL_NO_ERROR
 *       Open successful.
 *
 *  @retval eNVM_IBTL_ERROR_MEMORY
 *       Content allocation failure.
 *
 *  @retval eNVM_IBTL_ERROR_READ
 *       Read file failure.
 *
 *  @retval eNVM_IBTL_ERROR_WRITE
 *       Write file failure.
 *
 *  @retval eNVM_IBTL_ERROR_BAD_VERSION
 *       Invalid file version.
 *
 *  @retval eNVM_IBTL_ERROR_BAD_CRC
 *       Invalid file CRC.
 *
 *  @retval eNVM_IBTL_ERROR_NOT_FOUND
 *       Update request not found.
 *
 *  @retval eNVM_IBTL_ERROR
 *       Bad parameters or module not initialized.
 *
*/
tNVM_IBTL_Status NVM_IBTLi_AccessDialogArea
(
   bool                  xisReadOrWrite,
   tNVM_IBTL_DialogArea *pxstDialogArea
)
{
   tNVM_IBTL_Status     rc      = eNVM_IBTL_NO_ERROR;
   tNVM_IBTLi_File      file    = kNVM_IBTLi_FILE_NULL;
   bool                 rewrite = FALSE;
   uint32_t             crc32,calc_crc;
   int32_t              offset,n;
   tNVM_IBTL_DialogArea	stLocalDialogArea;


   tNVM_LOADER_DB_INFO_S stNewlandDialogArea;


   mTBOX_FCT_ENTER(NVM_IBTLi_AccessDialogArea);

   if (!vNVM_IBTLi_isInit)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"%s(...) ERROR: "
                   "NVM Interface with bootloader is not initialized\n",
                   TBOX_FunctionName));
      mTBOX_RETURN(eNVM_IBTL_ERROR);
   }
   if (pxstDialogArea == NULL)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"%s(...) ERROR: "
                   "All in/out pointers undefined\n",
                   TBOX_FunctionName));
      mTBOX_RETURN(eNVM_IBTL_ERROR);
   }

   memset(&stLocalDialogArea,0,sizeof(stLocalDialogArea));

   /* Initialization update request management */
   if (pxstDialogArea != NULL)
   {
		/*INPUT CHECKS*/
   }

   /* Open the file */
   rc = NVM_IBTLi_FileOpen(&file,&vNVM_IBTLi_stAreaLocDialogArea,
                           kNVM_IBTLi_FDA_FILE_SIZE);
   if (rc != eNVM_IBTL_NO_ERROR)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"%s() ERROR: "
                   "Open dialog area File {%#x,%#x} failure "
                   "(error %d, size %u bytes)\n",
                   TBOX_FunctionName,
                   vNVM_IBTLi_stAreaLocDialogArea.addressOffset,
                   vNVM_IBTLi_stAreaLocDialogArea.size,rc,
                   kNVM_IBTLi_FUR_FILE_SIZE));
      mTBOX_RETURN(rc);
   }

   /* Read in internal buffer allocated at the open */
   n = NVM_IBTLi_FileRead(&file,NULL,-1);
   if (n != file.i32Size)
   {
      /* Warning set default value and continue */
      mTBOX_TRACE((kTBOX_NIV_WARNING,"%s() ERROR: "
                   "Read Update Requests File {%#x,%#x} "
                   "failure (read %d/%d)\n",
                   TBOX_FunctionName,
                   vNVM_IBTLi_stAreaLocDialogArea.addressOffset,
                   vNVM_IBTLi_stAreaLocDialogArea.size,
                   n,file.i32Size));
      rc = eNVM_IBTL_ERROR_READ;
   }
   else
   {
#if defined (__NVM_IBTL_DUMP_FILE__)
       NVM_IBTLi_DumpFile(file.pui8Content,file.i32Size,
               &vNVM_IBTLi_stAreaLocDialogArea,
               TBOX_FunctionName,"Dialog area File","READ");
#endif

     offset   = 0;

    mMSBF_GET32_O(file.pui8Content,offset,stNewlandDialogArea.magic_number);
    NVM_IBTLi_ConvertEndian4Bytes(&stNewlandDialogArea.magic_number);
    mMSBF_GET32_O(file.pui8Content,offset,stNewlandDialogArea.ota_type);
    NVM_IBTLi_ConvertEndian4Bytes(&stNewlandDialogArea.ota_type);
    mMSBF_GET16_O(file.pui8Content,offset,stNewlandDialogArea.ota_pid);
    NVM_IBTLi_ConvertEndian2Bytes(&stNewlandDialogArea.ota_pid);
    mMSBF_GET16_O(file.pui8Content,offset,stNewlandDialogArea.ota_tableid);
    NVM_IBTLi_ConvertEndian2Bytes(&stNewlandDialogArea.ota_tableid);
    mMSBF_GET32_O(file.pui8Content,offset,stNewlandDialogArea.ota_frequency);
    NVM_IBTLi_ConvertEndian4Bytes(&stNewlandDialogArea.ota_frequency);
    mMSBF_GET32_O(file.pui8Content,offset,stNewlandDialogArea.ota_symbol_rate);
    NVM_IBTLi_ConvertEndian4Bytes(&stNewlandDialogArea.ota_symbol_rate);
    mMSBF_GET32_O(file.pui8Content,offset,stNewlandDialogArea.ota_modulation);
    NVM_IBTLi_ConvertEndian4Bytes(&stNewlandDialogArea.ota_modulation);
    mMSBF_GET32_O(file.pui8Content,offset,stNewlandDialogArea.u32NewSoftware);
    NVM_IBTLi_ConvertEndian4Bytes(&stNewlandDialogArea.u32NewSoftware);
    mMSBF_GET32_O(file.pui8Content,offset,stNewlandDialogArea.u32CurSoftware);
    NVM_IBTLi_ConvertEndian4Bytes(&stNewlandDialogArea.u32CurSoftware);
    mMSBF_GET32_O(file.pui8Content,offset,stNewlandDialogArea.u32FactoryID);
    NVM_IBTLi_ConvertEndian4Bytes(&stNewlandDialogArea.u32FactoryID);
    mMSBF_GET32_O(file.pui8Content,offset,stNewlandDialogArea.download_date);
    NVM_IBTLi_ConvertEndian4Bytes(&stNewlandDialogArea.download_date);
    mMSBF_GET32_O(file.pui8Content,offset,stNewlandDialogArea.u32FailedCnt);
    NVM_IBTLi_ConvertEndian4Bytes(&stNewlandDialogArea.u32FailedCnt);
    mMSBF_GET32_O(file.pui8Content,offset,stNewlandDialogArea.u32HardwareVersion);
    NVM_IBTLi_ConvertEndian4Bytes(&stNewlandDialogArea.u32HardwareVersion);
    mMSBF_GET32_O(file.pui8Content,offset,stNewlandDialogArea.u32DestroyFlag);
    NVM_IBTLi_ConvertEndian4Bytes(&stNewlandDialogArea.u32DestroyFlag);



   mTBOX_TRACE((kTBOX_NIV_3,"Readinng to Newland bootlader:\n"));
   mTBOX_TRACE((kTBOX_NIV_3," stNewlandDialogArea.magic_number 0x%x \n",stNewlandDialogArea.magic_number));
   mTBOX_TRACE((kTBOX_NIV_3," stNewlandDialogArea.ota_type 0x%x\n",stNewlandDialogArea.ota_type));
   mTBOX_TRACE((kTBOX_NIV_3," stNewlandDialogArea.ota_pid 0x%x\n",stNewlandDialogArea.ota_pid));
   mTBOX_TRACE((kTBOX_NIV_3," stNewlandDialogArea.ota_tableid 0x%x\n",stNewlandDialogArea.ota_tableid));
   mTBOX_TRACE((kTBOX_NIV_3," stNewlandDialogArea.ota_frequency %d\n",stNewlandDialogArea.ota_frequency));
   mTBOX_TRACE((kTBOX_NIV_3," stNewlandDialogArea.ota_symbol_rate %d\n",stNewlandDialogArea.ota_symbol_rate));
   mTBOX_TRACE((kTBOX_NIV_3," stNewlandDialogArea.ota_modulation 0x%x\n",stNewlandDialogArea.ota_modulation));
   mTBOX_TRACE((kTBOX_NIV_3," stNewlandDialogArea.u32NewSoftware 0x%x\n",stNewlandDialogArea.u32NewSoftware));
   mTBOX_TRACE((kTBOX_NIV_3," stNewlandDialogArea.u32CurSoftware 0x%x\n",stNewlandDialogArea.u32CurSoftware));
   mTBOX_TRACE((kTBOX_NIV_3," stNewlandDialogArea.u32FactoryID 0x%x\n",stNewlandDialogArea.u32FactoryID));
   mTBOX_TRACE((kTBOX_NIV_3," stNewlandDialogArea.download_date 0x%x\n",stNewlandDialogArea.download_date));
   mTBOX_TRACE((kTBOX_NIV_3," stNewlandDialogArea.u32FailedCnt 0x%x\n",stNewlandDialogArea.u32FailedCnt));
   mTBOX_TRACE((kTBOX_NIV_3," stNewlandDialogArea.u32HardwareVersion 0x%x\n",stNewlandDialogArea.u32HardwareVersion));
   mTBOX_TRACE((kTBOX_NIV_3," stNewlandDialogArea.u32DestroyFlag 0x%x\n",stNewlandDialogArea.u32DestroyFlag));
   mTBOX_TRACE((kTBOX_NIV_3," stNewlandDialogArea.u32Crc 0x%x\n",stNewlandDialogArea.u32Crc));



       NVM_IBTLi_ConvertNewlandToIBTL(stNewlandDialogArea,&stLocalDialogArea );

       /* Check magic */
      if (stNewlandDialogArea.magic_number != kNVM_IBTLi_NEWLAND_MAGIC_NUMBER)
      {
         /* Warning, continue to set default values */
         mTBOX_TRACE((kTBOX_NIV_WARNING,"%s() ERROR: "
                      "Magic nubmer mismatch\n",
                      TBOX_FunctionName));

         rc = eNVM_IBTL_ERROR_BAD_VERSION;
      }

   }

   /* Parse and Check CRC32 */
   if (rc == eNVM_IBTL_NO_ERROR)
   {
       /* Compute crc */
       calc_crc = NVM_IBTLi_ComputeCRC(file.pui8Content,offset,0,TRUE);

       /* Extract crc */

       mMSBF_GET32_O(file.pui8Content,offset,crc32);

       NVM_IBTLi_ConvertEndian4Bytes(&crc32);

       mTBOX_TRACE((kTBOX_NIV_3,"- crc32.................%#x,      calc_crc : %#x\n",crc32,calc_crc));

       /* Check the crc32 */
       if (crc32 != calc_crc)
       {
           /* Warning set default value and continue */
           mTBOX_TRACE((kTBOX_NIV_WARNING,"%s() ERROR: "
                   "CRC32 0x%08X of dialog area "
                   "File {%#x,%#x} invalid (compute 0x%08X)\n",
                   TBOX_FunctionName, crc32,
                   vNVM_IBTLi_stAreaLocDialogArea.addressOffset,
                   vNVM_IBTLi_stAreaLocDialogArea.size,
                   calc_crc));
           rc = eNVM_IBTL_ERROR_BAD_CRC;
       }
   }
   else
   {
       tCFGF_Configuration *pConfigFile;

       /* Reform the file and rewrite it */
       mTBOX_TRACE((kTBOX_NIV_WARNING,
               "[%s] Error read or corruption => "
               "Rewrite File with default values\n",
               TBOX_FunctionName));

       /* reset file */
       pxstDialogArea->u32syntaxVersion = kNVM_IBTLi_FDA_CURR_VERSION;
       pxstDialogArea->stDLCycle.u32UpdateID = 0xFFFFFFFF;
       pxstDialogArea->stDLCycle.u16DsmccPid = 0xFFFF;
       pxstDialogArea->stDLCycle.u8DLState = 0x0;
       pxstDialogArea->stDLCycle.u8ErrorCode = 0x0;
       /* get default usage ID from the config file */
       pConfigFile = CFGF_Read(kCFGF_ID_PROD_ID);
       pxstDialogArea->stDLCycle.u8UsageId = pConfigFile->stProductIDs.ui8DefaultUsageId;
       pxstDialogArea->stDLCycle.u8NetWorkType = 0xFF;
       
       CFGF_Free(pConfigFile);

       offset = 52;

       /* compute CRC and set it */
       calc_crc = NVM_IBTLi_ComputeCRC(file.pui8Content,offset,0,TRUE);
       NVM_IBTLi_ConvertEndian4Bytes(&calc_crc);

       mMSBF_SET32_O(file.pui8Content,offset,calc_crc);
       /* force writing of file */
       rewrite = TRUE;
   }

   /*
   ** Read done
   */

   if (xisReadOrWrite == TRUE)
   {
      if (rc == eNVM_IBTL_NO_ERROR)
      {
         /* Copy content to parameter structure */
         if (pxstDialogArea != NULL)
         {
            memcpy((void*)pxstDialogArea, (void*)&stLocalDialogArea, sizeof(stLocalDialogArea));
         }
      }
   }
   else
   {
      if (pxstDialogArea != NULL)
      {
         rewrite = TRUE;
      }
   }

      /*
      ** write
      */
   if (rewrite == TRUE)
   {
       offset   = 0;

       NVM_IBTLi_ConvertIBTLToNewland(*pxstDialogArea,&stNewlandDialogArea);

        mTBOX_TRACE((kTBOX_NIV_3,"Writing to Newland bootlader:\n"));
        mTBOX_TRACE((kTBOX_NIV_3," stNewlandDialogArea.magic_number 0x%x \n",stNewlandDialogArea.magic_number));
        mTBOX_TRACE((kTBOX_NIV_3," stNewlandDialogArea.ota_type 0x%x\n",stNewlandDialogArea.ota_type));
        mTBOX_TRACE((kTBOX_NIV_3," stNewlandDialogArea.ota_pid 0x%x\n",stNewlandDialogArea.ota_pid));
        mTBOX_TRACE((kTBOX_NIV_3," stNewlandDialogArea.ota_tableid 0x%x\n",stNewlandDialogArea.ota_tableid));
        mTBOX_TRACE((kTBOX_NIV_3," stNewlandDialogArea.ota_frequency %d\n",stNewlandDialogArea.ota_frequency));
        mTBOX_TRACE((kTBOX_NIV_3," stNewlandDialogArea.ota_symbol_rate %d\n",stNewlandDialogArea.ota_symbol_rate));
        mTBOX_TRACE((kTBOX_NIV_3," stNewlandDialogArea.ota_modulation 0x%x\n",stNewlandDialogArea.ota_modulation));
        mTBOX_TRACE((kTBOX_NIV_3," stNewlandDialogArea.u32NewSoftware 0x%x\n",stNewlandDialogArea.u32NewSoftware));
        mTBOX_TRACE((kTBOX_NIV_3," stNewlandDialogArea.u32CurSoftware 0x%x\n",stNewlandDialogArea.u32CurSoftware));
        mTBOX_TRACE((kTBOX_NIV_3," stNewlandDialogArea.u32FactoryID 0x%x\n",stNewlandDialogArea.u32FactoryID));
        mTBOX_TRACE((kTBOX_NIV_3," stNewlandDialogArea.download_date 0x%x\n",stNewlandDialogArea.download_date));
        mTBOX_TRACE((kTBOX_NIV_3," stNewlandDialogArea.u32FailedCnt 0x%x\n",stNewlandDialogArea.u32FailedCnt));
        mTBOX_TRACE((kTBOX_NIV_3," stNewlandDialogArea.u32HardwareVersion 0x%x\n",stNewlandDialogArea.u32HardwareVersion));
        mTBOX_TRACE((kTBOX_NIV_3," stNewlandDialogArea.u32DestroyFlag 0x%x\n",stNewlandDialogArea.u32DestroyFlag));
        mTBOX_TRACE((kTBOX_NIV_3," stNewlandDialogArea.u32Crc 0x%x\n",stNewlandDialogArea.u32Crc));


        NVM_IBTLi_ConvertEndian4Bytes(&stNewlandDialogArea.magic_number);
        mMSBF_SET32_O(file.pui8Content,offset,stNewlandDialogArea.magic_number);
        NVM_IBTLi_ConvertEndian4Bytes(&stNewlandDialogArea.ota_type);
        mMSBF_SET32_O(file.pui8Content,offset,stNewlandDialogArea.ota_type);
        NVM_IBTLi_ConvertEndian2Bytes(&stNewlandDialogArea.ota_pid);
        mMSBF_SET16_O(file.pui8Content,offset,stNewlandDialogArea.ota_pid);
        NVM_IBTLi_ConvertEndian2Bytes(&stNewlandDialogArea.ota_tableid);
        mMSBF_SET16_O(file.pui8Content,offset,stNewlandDialogArea.ota_tableid);
        NVM_IBTLi_ConvertEndian4Bytes(&stNewlandDialogArea.ota_frequency);
        mMSBF_SET32_O(file.pui8Content,offset,stNewlandDialogArea.ota_frequency);
        NVM_IBTLi_ConvertEndian4Bytes(&stNewlandDialogArea.ota_symbol_rate);
        mMSBF_SET32_O(file.pui8Content,offset,stNewlandDialogArea.ota_symbol_rate);
        NVM_IBTLi_ConvertEndian4Bytes(&stNewlandDialogArea.ota_modulation);
        mMSBF_SET32_O(file.pui8Content,offset,stNewlandDialogArea.ota_modulation);
        NVM_IBTLi_ConvertEndian4Bytes(&stNewlandDialogArea.u32NewSoftware);
        mMSBF_SET32_O(file.pui8Content,offset,stNewlandDialogArea.u32NewSoftware);
        NVM_IBTLi_ConvertEndian4Bytes(&stNewlandDialogArea.u32CurSoftware);
        mMSBF_SET32_O(file.pui8Content,offset,stNewlandDialogArea.u32CurSoftware);
        NVM_IBTLi_ConvertEndian4Bytes(&stNewlandDialogArea.u32FactoryID);
        mMSBF_SET32_O(file.pui8Content,offset,stNewlandDialogArea.u32FactoryID);
        NVM_IBTLi_ConvertEndian4Bytes(&stNewlandDialogArea.download_date);
        mMSBF_SET32_O(file.pui8Content,offset,stNewlandDialogArea.download_date);
        NVM_IBTLi_ConvertEndian4Bytes(&stNewlandDialogArea.u32FailedCnt);
        mMSBF_SET32_O(file.pui8Content,offset,stNewlandDialogArea.u32FailedCnt);
        NVM_IBTLi_ConvertEndian4Bytes(&stNewlandDialogArea.u32HardwareVersion);
        mMSBF_SET32_O(file.pui8Content,offset,stNewlandDialogArea.u32HardwareVersion);
        NVM_IBTLi_ConvertEndian4Bytes(&stNewlandDialogArea.u32DestroyFlag);
        mMSBF_SET32_O(file.pui8Content,offset,stNewlandDialogArea.u32DestroyFlag);

       /* Compute CRC and set it */

       calc_crc = NVM_IBTLi_ComputeCRC(file.pui8Content, offset,0,TRUE);


       NVM_IBTLi_ConvertEndian4Bytes(&calc_crc);

       /* TODO */
       mMSBF_SET32_O(file.pui8Content,offset,calc_crc);

       /* TODO */
       //mMSBF_SET32_O(file.pui8Content,offset,calc_crc);

       /* Write the new file content */

       n = NVM_IBTLi_FileWrite(&file,NULL,-1);

       if (n != file.i32Size)
       {
           /* Error */
           mTBOX_TRACE((kTBOX_NIV_WARNING,"%s() ERROR: "
                   "Write Update Requests File {%#x,%#x} failure "
                   "(write %d/%d, crc32=0x%08X, data size=%d)\n",
                   TBOX_FunctionName,
                   vNVM_IBTLi_stAreaLocDialogArea.addressOffset,
                   vNVM_IBTLi_stAreaLocDialogArea.size,
                   n,file.i32Size,calc_crc,offset));
           /* Set/Unset mode : Return a write error */
           rc = eNVM_IBTL_ERROR_WRITE;
       }
       else
       {
           /* OKAY */
           mTBOX_TRACE((kTBOX_NIV_1,
                   "[%s] Table of requests (crc32=0x%08X) written "
                   "in Update Requests File {%#x,%#x} (data size=%d)\n",
                   TBOX_FunctionName,calc_crc,
                   vNVM_IBTLi_stAreaLocDialogArea.addressOffset,
                   vNVM_IBTLi_stAreaLocDialogArea.size,offset));
           rc = eNVM_IBTL_NO_ERROR;
#if defined (__NVM_IBTL_DUMP_FILE__)
           NVM_IBTLi_DumpFile(file.pui8Content, kNVM_IBTLi_FDA_FILE_SIZE,
                   &vNVM_IBTLi_stAreaLocDialogArea,
                   TBOX_FunctionName,"Update Request File","WRITE");
#endif
       }
   }

   /* Close the file */
   NVM_IBTLi_FileClose(&file);

   return(rc);
}

#elif defined (CHANGHONG_BOOTLOADER)
/**
 *  @brief
 *    Manage the new trigger downloader dialog area
 *
 *
 *  @param[in] uAccessMode
 *       If eREAD, return the current content
 *       If eWRITE, update the content
 *
 *  @param[in/out] pxstDialogArea
 *       If uAccessMode=eREAD, return the current content into pxstDialogArea
 *       If uAccessMode=eWRITE, update the content with pxstDialogArea
 *
 *  @retval eNVM_IBTL_NO_ERROR
 *       Open successful.
 *
 *  @retval eNVM_IBTL_ERROR_MEMORY
 *       Content allocation failure.
 *
 *  @retval eNVM_IBTL_ERROR_READ
 *       Read file failure.
 *
 *  @retval eNVM_IBTL_ERROR_WRITE
 *       Write file failure.
 *
 *  @retval eNVM_IBTL_ERROR_BAD_VERSION
 *       Invalid file version.
 *
 *  @retval eNVM_IBTL_ERROR_BAD_CRC
 *       Invalid file CRC.
 *
 *  @retval eNVM_IBTL_ERROR_NOT_FOUND
 *       Update request not found.
 *
 *  @retval eNVM_IBTL_ERROR
 *       Bad parameters or module not initialized.
 *
*/
static tNVM_IBTL_Status NVM_IBTLi_AccessDialogArea
(
   tNVM_IBTLi_AccessMode uAccessMode,
   tNVM_IBTL_DialogArea *pxstDialogArea
)
{
   int32_t              n, calc_crc;
   tNVM_IBTL_Status     rc      = eNVM_IBTL_NO_ERROR;
   tNVM_IBTLi_File      file    = kNVM_IBTLi_FILE_NULL;
   tNVM_IBTL_DialogArea	stLocalDialogArea;
   CH_UpdateInfo_t      stChanghongDialogArea;

   mTBOX_FCT_ENTER(NVM_IBTLi_AccessDialogArea);

   if (!vNVM_IBTLi_isInit)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"%s(...) ERROR: "
                   "NVM Interface with bootloader is not initialized\n",
                   TBOX_FunctionName));
      mTBOX_RETURN(eNVM_IBTL_ERROR);
   }

   if (pxstDialogArea == NULL)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"%s(...) ERROR: "
                   "All in/out pointers undefined\n",
                   TBOX_FunctionName));
      mTBOX_RETURN(eNVM_IBTL_ERROR);
   }

   /* Initialization update request management */
   if (pxstDialogArea != NULL)
   {
		/*INPUT CHECKS*/
   }

   /* Open the file */
   rc = NVM_IBTLi_FileOpen(&file,&vNVM_IBTLi_stAreaLocDialogArea,
                           sizeof(CH_UpdateInfo_t));
   if (rc != eNVM_IBTL_NO_ERROR)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"%s() ERROR: "
                   "Open dialog area File {%#x,%#x} failure "
                   "(error %d, size %u bytes)\n",
                   TBOX_FunctionName,
                   vNVM_IBTLi_stAreaLocDialogArea.addressOffset,
                   vNVM_IBTLi_stAreaLocDialogArea.size,rc,
                   kNVM_IBTLi_FUR_FILE_SIZE));
      mTBOX_RETURN(rc);
   }

   /*
   ** Read done
   */
   if (uAccessMode == eREAD)
   {
	   memset(&stLocalDialogArea,0,sizeof(stLocalDialogArea));

	   /* Read in internal buffer allocated at the open */
	   n = NVM_IBTLi_FileRead(&file,NULL,-1);
	   if (n != file.i32Size)
	   {
		   /* Warning set default value and continue */
		   mTBOX_TRACE((kTBOX_NIV_WARNING,"%s() ERROR: "
				   "Read Update Requests File {%#x,%#x} "
				   "failure (read %d/%d)\n",
				   TBOX_FunctionName,
				   vNVM_IBTLi_stAreaLocDialogArea.addressOffset,
				   vNVM_IBTLi_stAreaLocDialogArea.size,
				   n,file.i32Size));
		   rc = eNVM_IBTL_ERROR_READ;
	   }

	   /* Parse and Check CRC32 */
	   if (rc == eNVM_IBTL_NO_ERROR)
	   {
#if defined (__NVM_IBTL_DUMP_FILE__)
		   NVM_IBTLi_DumpFile(file.pui8Content,file.i32Size,
				   &vNVM_IBTLi_stAreaLocDialogArea,
				   TBOX_FunctionName,"Dialog area File","READ");
#endif

		   /* Loading from Newland dialog area buffer to local structure */
		   memcpy(&stChanghongDialogArea, file.pui8Content, sizeof(CH_UpdateInfo_t)); /* little endian */
		   /*NVM_IBTLi_GetBatchConversionEndian4Bytes(file, &offset, &stChanghongDialogArea);*/

		   /* Compute crc */
		   calc_crc = CH_COM_MPEGCRC32((uint8_t *)&(stChanghongDialogArea.enm_AntType), stChanghongDialogArea.i_length -4);
		   mTBOX_TRACE((kTBOX_NIV_3,"- crc from flash.................%#x,      calculated crc : %#x\n", stChanghongDialogArea.ui_crc, calc_crc));

		   /* Check the crc32 */
		   if(stChanghongDialogArea.ui_crc != calc_crc)
		   {
			   /* Warning set default value and continue */
			   mTBOX_TRACE((kTBOX_NIV_WARNING,"%s() ERROR: "
					   "CRC32 0x%08X of dialog area "
					   "File {%#x,%#x} invalid (compute 0x%08X)\n",
					   TBOX_FunctionName, stChanghongDialogArea.ui_crc,
					   vNVM_IBTLi_stAreaLocDialogArea.addressOffset,
					   vNVM_IBTLi_stAreaLocDialogArea.size,
					   calc_crc));
			   rc = eNVM_IBTL_ERROR_BAD_CRC;
		   }
	   }

	   if (rc == eNVM_IBTL_NO_ERROR)
	   {
		   /* TRACE NEWLAND DIALOG AREA */
		   mTBOX_TRACE((kTBOX_NIV_3,"Loading from Changhong bootlader:\n"));
		   NVM_IBTLi_TraceCHDlgArea(&stChanghongDialogArea);

		   NVM_IBTLi_ConvertCHToIBTL(stChanghongDialogArea,&stLocalDialogArea );

		   /* Copy content to parameter structure */
		   if (pxstDialogArea != NULL)
		   {
			   memcpy((void*)pxstDialogArea, (void*)&stLocalDialogArea, sizeof(stLocalDialogArea));
		   }
	   }

   }

   /*
   ** Write done
   */
   if (pxstDialogArea != NULL && uAccessMode == eWRITE)
   {
       NVM_IBTLi_ConvertIBTLToCH(*pxstDialogArea,&stChanghongDialogArea);

       mTBOX_TRACE((kTBOX_NIV_3,"Storing to Changhong bootloader:\n"));
       NVM_IBTLi_TraceCHDlgArea(&stChanghongDialogArea);

       /* Storing local structure to Newland dialog area*/
       memcpy(file.pui8Content, &stChanghongDialogArea, sizeof(CH_UpdateInfo_t)); /* little endian */
       /*NVM_IBTLi_SetBatchConversionEndian4Bytes(file, &offset, &stChanghongDialogArea);*/

       /* Write the new file content */
       n = NVM_IBTLi_FileWrite(&file,NULL,-1);
       if (n != file.i32Size)
       {
           /* Error */
           mTBOX_TRACE((kTBOX_NIV_WARNING,"%s() ERROR: "
                   "Write Update Requests File {%#x,%#x} failure "
                   "(write %d/%d)\n",
                   TBOX_FunctionName,
                   vNVM_IBTLi_stAreaLocDialogArea.addressOffset,
                   vNVM_IBTLi_stAreaLocDialogArea.size,
                   n,file.i32Size));
           /* Set/Unset mode : Return a write error */
           rc = eNVM_IBTL_ERROR_WRITE;
       }
       else
       {
           /* OKAY */
           mTBOX_TRACE((kTBOX_NIV_1,
                   "[%s] Table of requests (crc32=0x%08X) written "
                   "in Update Requests File {%#x,%#x} (data size=%d)\n",
                   TBOX_FunctionName,calc_crc,
                   vNVM_IBTLi_stAreaLocDialogArea.addressOffset,
                   vNVM_IBTLi_stAreaLocDialogArea.size,file.i32Size));
           rc = eNVM_IBTL_NO_ERROR;
#if defined (__NVM_IBTL_DUMP_FILE__)
           NVM_IBTLi_DumpFile(file.pui8Content, kNVM_IBTLi_FDA_FILE_SIZE,
                   &vNVM_IBTLi_stAreaLocDialogArea,
                   TBOX_FunctionName,"Update Request File","WRITE");
#endif
       }
   }

   /* Close the file */
   NVM_IBTLi_FileClose(&file);

   return(rc);
}

//        	CH_UpdateInfo_t		gsstru_OTAUpdateInfo;
//
//       	memset(&gsstru_OTAUpdateInfo,0,sizeof(CH_UpdateInfo_t));
//
//       	memcpy(gsstru_OTAUpdateInfo.uc_name,"Update Info",strlen("Update Info") );
//       	gsstru_OTAUpdateInfo.enm_AntType			= CH_DEMOD_DVB_C;
//       	gsstru_OTAUpdateInfo.uc_UpdaeFlag			= 0x47;
//       	gsstru_OTAUpdateInfo.un_FreqInfo.stru_CableFreqInfo.ui_FreqKHZ	= 555000;
//       	gsstru_OTAUpdateInfo.un_FreqInfo.stru_CableFreqInfo.ui_SymbKbps	= 6875;
//       	gsstru_OTAUpdateInfo.un_FreqInfo.stru_CableFreqInfo.uc_QAMMode	= 3;
//       	gsstru_OTAUpdateInfo.us_DataPid			= 3007;
//       	gsstru_OTAUpdateInfo.us_DataGid			= 0x2;
//       	gsstru_OTAUpdateInfo.i_length = sizeof(CH_UpdateInfo_t) -16;
//       	gsstru_OTAUpdateInfo.ui_crc = CH_COM_MPEGCRC32((uint8_t*)&gsstru_OTAUpdateInfo.enm_AntType , gsstru_OTAUpdateInfo.i_length - 4);
//
//       	(int32_t)NVM_IBTLi_FLA_Write(0x7d0200, &gsstru_OTAUpdateInfo, sizeof(gsstru_OTAUpdateInfo));
//       	free(1234);//crash
#elif defined(SKYWORTH_BOOTLOADER)
static tNVM_IBTL_Status NVM_IBTLi_AccessDialogArea
(
   tNVM_IBTLi_AccessMode uAccessMode,
   tNVM_IBTL_DialogArea *pxstDialogArea
)
{
   mTBOX_FCT_ENTER(NVM_IBTLi_AccessDialogArea);
   int32_t              n;
   tNVM_IBTL_Status     rc      = eNVM_IBTL_NO_ERROR;
   tNVM_IBTLi_File      file    = kNVM_IBTLi_FILE_NULL;
   tNVM_IBTL_DialogArea stLocalDialogArea;
   tNVM_SKYLoaderInfo   stSkyworthDialogArea;

   if (!vNVM_IBTLi_isInit)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"%s(...) ERROR: "
                   "NVM Interface with bootloader is not initialized\n",
                   TBOX_FunctionName));
      mTBOX_RETURN(eNVM_IBTL_ERROR);
   }

   if (pxstDialogArea == NULL)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"%s(...) ERROR: "
                   "All in/out pointers undefined\n",
                   TBOX_FunctionName));
      mTBOX_RETURN(eNVM_IBTL_ERROR);
   }

   /* Open the file */
   rc = NVM_IBTLi_FileOpen(&file, &vNVM_IBTLi_stAreaLocDialogArea, sizeof(tNVM_SKYLoaderInfo));
   if (rc != eNVM_IBTL_NO_ERROR)
   {
       mTBOX_TRACE((kTBOX_NIV_CRITICAL,"%s() ERROR: "
               "Open dialog area File {%#x,%#x} failure "
               "(error %d, size %u bytes)\n",
               TBOX_FunctionName,
               vNVM_IBTLi_stAreaLocDialogArea.addressOffset,
               vNVM_IBTLi_stAreaLocDialogArea.size,rc,
               kNVM_IBTLi_FUR_FILE_SIZE));
       mTBOX_RETURN(rc);
   }

   /*
    ** Read done
    */
   if (uAccessMode == eREAD)
   {
       memset(&stLocalDialogArea, 0, sizeof(stLocalDialogArea));

       /* Read in internal buffer allocated at the open */
       n = NVM_IBTLi_FileRead(&file,NULL,-1);
       if (n != file.i32Size)
       {
           /* Warning set default value and continue */
           mTBOX_TRACE((kTBOX_NIV_WARNING,"%s() ERROR: "
                   "Read Update Requests File {%#x,%#x} "
                   "failure (read %d/%d)\n",
                   TBOX_FunctionName,
                   vNVM_IBTLi_stAreaLocDialogArea.addressOffset,
                   vNVM_IBTLi_stAreaLocDialogArea.size,
                   n,file.i32Size));
           rc = eNVM_IBTL_ERROR_READ;
       }

       /* Parse and Check CRC32 */
       if (rc == eNVM_IBTL_NO_ERROR)
       {
#if defined (__NVM_IBTL_DUMP_FILE__)
           NVM_IBTLi_DumpFile(file.pui8Content,file.i32Size,
                   &vNVM_IBTLi_stAreaLocDialogArea,
                   TBOX_FunctionName,"Dialog area File","READ");
#endif

           /* Loading from Newland dialog area buffer to local structure */
           memcpy(&stSkyworthDialogArea, file.pui8Content, sizeof(tNVM_SKYLoaderInfo)); /* little endian */
           /*NVM_IBTLi_GetBatchConversionEndian4Bytes(file, &offset, &stChanghongDialogArea);*/
       }

       if (rc == eNVM_IBTL_NO_ERROR)
       {
           /* TRACE NEWLAND DIALOG AREA */
           mTBOX_TRACE((kTBOX_NIV_3,"Loading from Changhong bootlader:\n"));
           NVM_IBTLi_TraceSKYDlgArea(&stSkyworthDialogArea);

           NVM_IBTLi_ConvertSKYToIBTL(stSkyworthDialogArea, &stLocalDialogArea);

           /* Copy content to parameter structure */
           if (pxstDialogArea != NULL)
           {
               memcpy((void*)pxstDialogArea, (void*)&stLocalDialogArea, sizeof(stLocalDialogArea));
           }
       }

   }

   /*
    ** Write done
    */
   if (pxstDialogArea != NULL && uAccessMode == eWRITE)
   {
       NVM_IBTLi_ConvertIBTLToSKY(*pxstDialogArea, &stSkyworthDialogArea);

       NVM_IBTLi_TraceSKYDlgArea(&stSkyworthDialogArea);

       /* Storing local structure to Skyworth dialog area*/
       memcpy(file.pui8Content, &stSkyworthDialogArea, sizeof(tNVM_SKYLoaderInfo)); /* little endian */

       /* Write the new file content */
       n = NVM_IBTLi_FileWrite(&file,NULL,-1);
       if (n != file.i32Size)
       {
           /* Error */
           mTBOX_TRACE((kTBOX_NIV_WARNING,"%s() ERROR: "
                   "Write Update Requests File {%#x,%#x} failure "
                   "(write %d/%d)\n",
                   TBOX_FunctionName,
                   vNVM_IBTLi_stAreaLocDialogArea.addressOffset,
                   vNVM_IBTLi_stAreaLocDialogArea.size,
                   n,file.i32Size));
           /* Set/Unset mode : Return a write error */
           rc = eNVM_IBTL_ERROR_WRITE;
       }
       else
       {
           /* OKAY */
           mTBOX_TRACE((kTBOX_NIV_1,
                   "[%s] Table of requests"
                   "in Update Requests File {%#x,%#x} (data size=%d)\n",
                   TBOX_FunctionName,
                   vNVM_IBTLi_stAreaLocDialogArea.addressOffset,
                   vNVM_IBTLi_stAreaLocDialogArea.size,file.i32Size));
           rc = eNVM_IBTL_NO_ERROR;
#if defined (__NVM_IBTL_DUMP_FILE__)
           NVM_IBTLi_DumpFile(file.pui8Content, kNVM_IBTLi_FDA_FILE_SIZE,
                   &vNVM_IBTLi_stAreaLocDialogArea,
                   TBOX_FunctionName,"Update Request File","WRITE");
#endif
       }
   }

   /* Close the file */
   NVM_IBTLi_FileClose(&file);

   mTBOX_RETURN(eNVM_IBTL_NO_ERROR);
}

#elif defined (TELESYSTEM_BOOTLOADER) || defined(TELESYSTEM_MAP34_BOOTLOADER) || defined(TELESYSTEM_MAP51_BOOTLOADER) || defined(KAON_MSD5C59_BOOTLOADER)
static tNVM_IBTL_Status NVM_IBTLi_AccessDialogArea
(
   tNVM_IBTLi_AccessMode uAccessMode,
   tNVM_IBTL_DialogArea *pxstDialogArea
)
{
   tNVM_IBTL_Status     rc      = eNVM_IBTL_NO_ERROR;
   tNVM_IBTLi_File      file    = kNVM_IBTLi_FILE_NULL;
   bool                 rewrite = FALSE;
   uint32_t             crc32,calc_crc;
   int32_t              offset,n;
   tNVM_IBTL_DialogArea	stLocalDialogArea;

#if defined(MEDIUM_SAT)
   uint8_t              flags = 0;
#endif

   mTBOX_FCT_ENTER(NVM_IBTLi_AccessDialogArea);

   if (!vNVM_IBTLi_isInit)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"%s(...) ERROR: "
                   "NVM Interface with bootloader is not initialized\n",
                   TBOX_FunctionName));
      mTBOX_RETURN(eNVM_IBTL_ERROR);
   }
   if (pxstDialogArea == NULL)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"%s(...) ERROR: "
                   "All in/out pointers undefined\n",
                   TBOX_FunctionName));
      mTBOX_RETURN(eNVM_IBTL_ERROR);
   }

   memset(&stLocalDialogArea,0,sizeof(stLocalDialogArea));

   /* Initialization update request management */
   if (pxstDialogArea != NULL)
   {
		/*INPUT CHECKS*/
   }

   /* Open the file */
   rc = NVM_IBTLi_FileOpen(&file,&vNVM_IBTLi_stAreaLocDialogArea,
                           kNVM_IBTLi_FDA_FILE_SIZE);

   if (rc != eNVM_IBTL_NO_ERROR)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"%s() ERROR: "
                   "Open dialog area File {%#x,%#x} failure "
                   "(error %d, size %u bytes)\n",
                   TBOX_FunctionName,
                   vNVM_IBTLi_stAreaLocDialogArea.addressOffset,
                   vNVM_IBTLi_stAreaLocDialogArea.size,rc,
                   kNVM_IBTLi_FUR_FILE_SIZE));
      mTBOX_RETURN(rc);
   }

   /* Read in internal buffer allocated at the open */
   n = NVM_IBTLi_FileRead(&file,NULL,-1);

   if (n != file.i32Size)
   {
      /* Warning set default value and continue */
      mTBOX_TRACE((kTBOX_NIV_WARNING,"%s() ERROR: "
                   "Read Update Requests File {%#x,%#x} "
                   "failure (read %d/%d)\n",
                   TBOX_FunctionName,
                   vNVM_IBTLi_stAreaLocDialogArea.addressOffset,
                   vNVM_IBTLi_stAreaLocDialogArea.size,
                   n,file.i32Size));
      rc = eNVM_IBTL_ERROR_READ;
   }
   else
   {
#if defined (__NVM_IBTL_DUMP_FILE__)
      NVM_IBTLi_DumpFile(file.pui8Content,file.i32Size,
                         &vNVM_IBTLi_stAreaLocDialogArea,
                         TBOX_FunctionName,"Dialog area File","READ");
#endif

      /** 
       * Init scanning of the internal buffer allocated and read
       * Start CRC32 calculation on part header
      */
      offset   = 0;

      /* Extract header */
      mMSBF_GET32_O(file.pui8Content,offset,stLocalDialogArea.u32syntaxVersion);
      mTBOX_TRACE((kTBOX_NIV_3,"[%s] - stLocalDialogArea.u32syntaxVersion=%d\n",
                   TBOX_FunctionName,stLocalDialogArea.u32syntaxVersion));
      /* 
      * extract stDownload cycle 
      */
      mMSBF_GET32_O(file.pui8Content,offset,stLocalDialogArea.stDLCycle.u32UpdateID);
      mMSBF_GET16_O(file.pui8Content,offset,stLocalDialogArea.stDLCycle.u16PmtPid);
      mMSBF_GET16_O(file.pui8Content,offset,stLocalDialogArea.stDLCycle.u16DsmccPid);
      mMSBF_GET08_O(file.pui8Content,offset,stLocalDialogArea.stDLCycle.u8DLState);
      mMSBF_GET08_O(file.pui8Content,offset,stLocalDialogArea.stDLCycle.u8ErrorCode);
      mMSBF_GET08_O(file.pui8Content,offset,stLocalDialogArea.stDLCycle.u8UsageId);
      mMSBF_GET08_O(file.pui8Content,offset,stLocalDialogArea.stDLCycle.u8NetWorkType);
      mTBOX_TRACE((kTBOX_NIV_3,"[%s] - stLocalDialogArea.stDLCycle=....\n UpdateID=0x%x, PIDs 0x%x 0x%x, error %d, %d %d\n",
                   TBOX_FunctionName,
                   stLocalDialogArea.stDLCycle.u32UpdateID,
                   stLocalDialogArea.stDLCycle.u16PmtPid,
                   stLocalDialogArea.stDLCycle.u16DsmccPid,
                   stLocalDialogArea.stDLCycle.u8ErrorCode,
                   stLocalDialogArea.stDLCycle.u8UsageId,
                   stLocalDialogArea.stDLCycle.u8NetWorkType ));
      if ((stLocalDialogArea.stDLCycle.u8NetWorkType == kNVM_IBTL_NWT_SERIAL)
      ||(stLocalDialogArea.stDLCycle.u8NetWorkType == kNVM_IBTL_NWT_USB)
      ||(stLocalDialogArea.stDLCycle.u8NetWorkType == kNVM_IBTL_NWT_URL))
      {
         mTBOX_TRACE((kTBOX_NIV_WARNING,"[%s] - stLocalDialogArea.u8NetWorkType=%d unexpected for nominal download !\n",
                   TBOX_FunctionName,stLocalDialogArea.stDLCycle.u8NetWorkType));
      }        
#if defined(MEDIUM_SAT)
      else if (stLocalDialogArea.stDLCycle.u8NetWorkType == kNVM_IBTL_NWT_DVB_S)
      {
         /* 
         * extract stDownload DVB-s location 
         */
         mMSBF_GET32_O(file.pui8Content,offset,stLocalDialogArea.stDvbsLocation.BIS_frequency);
         mMSBF_GET32_O(file.pui8Content,offset,stLocalDialogArea.stDvbsLocation.symbol_rate);
         mMSBF_GET08_O(file.pui8Content,offset,flags);
         stLocalDialogArea.stDvbsLocation.continuous_tone_on_flag    = ((flags&0x04) != 0);
         stLocalDialogArea.stDvbsLocation.lnb_power_on_flag              = ((flags&0x02) != 0);
         stLocalDialogArea.stDvbsLocation.vertical_polarisation_flag    = ((flags&0x01) != 0);
         mMSBF_GET08_O(file.pui8Content,offset,stLocalDialogArea.stDvbsLocation.modulation_type);
         mMSBF_GET08_O(file.pui8Content,offset,stLocalDialogArea.stDvbsLocation.fecrate);
         mMSBF_GET08_O(file.pui8Content,offset,stLocalDialogArea.stDvbsLocation.switch_selection);
         mTBOX_TRACE((kTBOX_NIV_3,"[%s] - stLocalDialogArea.stDvbsLocation=....\n Freq=%d, SR=%d, Flags %d %d %d mod=%d Fec=%d ODU=%d\n",
                      TBOX_FunctionName,
                      stLocalDialogArea.stDvbsLocation.BIS_frequency,
                      stLocalDialogArea.stDvbsLocation.symbol_rate,
                      stLocalDialogArea.stDvbsLocation.continuous_tone_on_flag,
                      stLocalDialogArea.stDvbsLocation.lnb_power_on_flag,
                      stLocalDialogArea.stDvbsLocation.vertical_polarisation_flag,
                      stLocalDialogArea.stDvbsLocation.modulation_type,
                      stLocalDialogArea.stDvbsLocation.fecrate,
                      stLocalDialogArea.stDvbsLocation.switch_selection ));
      }
#elif defined(MEDIUM_CAB)
      else if (stLocalDialogArea.stDLCycle.u8NetWorkType == kNVM_IBTL_NWT_DVB_C)
      {
         mMSBF_GET32_O(file.pui8Content,offset,stLocalDialogArea.stDvbcLocation.BIS_frequency);
         mMSBF_GET32_O(file.pui8Content,offset,stLocalDialogArea.stDvbcLocation.symbol_rate);
         mMSBF_GET08_O(file.pui8Content,offset,stLocalDialogArea.stDvbcLocation.modulation_type);
         mTBOX_TRACE((kTBOX_NIV_3,"[%s] - stLocalDialogArea.stDvbcLocation=....\n Freq=%d, SR=%d, mod=%d \n",
                      TBOX_FunctionName,
                      stLocalDialogArea.stDvbcLocation.BIS_frequency,
                      stLocalDialogArea.stDvbcLocation.symbol_rate,
                      stLocalDialogArea.stDvbcLocation.modulation_type));
      }
#elif defined(MEDIUM_TER)  
      else if (stLocalDialogArea.stDLCycle.u8NetWorkType == kNVM_IBTL_NWT_DVB_T)
      {
         mMSBF_GET32_O(file.pui8Content,offset,stLocalDialogArea.stDvbtLocation.frequency);
         mMSBF_GET32_O(file.pui8Content,offset,stLocalDialogArea.stDvbtLocation.bandwidth);
         mMSBF_GET08_O(file.pui8Content,offset,stLocalDialogArea.stDvbtLocation.modulation);
         mMSBF_GET08_O(file.pui8Content,offset,stLocalDialogArea.stDvbtLocation.mode);
         mMSBF_GET08_O(file.pui8Content,offset,stLocalDialogArea.stDvbtLocation.guard);
         mMSBF_GET08_O(file.pui8Content,offset,stLocalDialogArea.stDvbtLocation.hierarchy);
         mMSBF_GET08_O(file.pui8Content,offset,stLocalDialogArea.stDvbtLocation.priority);
         mMSBF_GET08_O(file.pui8Content,offset,stLocalDialogArea.stDvbtLocation.code_rate_HP);
         mMSBF_GET08_O(file.pui8Content,offset,stLocalDialogArea.stDvbtLocation.code_rate_LP);
         mMSBF_GET08_O(file.pui8Content,offset,stLocalDialogArea.stDvbtLocation.freq_offset);
         mTBOX_TRACE((kTBOX_NIV_3,"[%s] - stLocalDialogArea.stDvbtLocation=....\n Freq=%d, BW=%d, mod=%d \n",
                      TBOX_FunctionName,
                      stLocalDialogArea.stDvbtLocation.frequency,
                      stLocalDialogArea.stDvbtLocation.bandwidth,
                      stLocalDialogArea.stDvbtLocation.modulation));
      }
      else if (stLocalDialogArea.stDLCycle.u8NetWorkType == kNVM_IBTL_NWT_DVB_T2)
      {
         mMSBF_GET32_O(file.pui8Content,offset,stLocalDialogArea.stDvbt2Location.frequency);
         mMSBF_GET32_O(file.pui8Content,offset,stLocalDialogArea.stDvbt2Location.bandwidth);
         mMSBF_GET08_O(file.pui8Content,offset,stLocalDialogArea.stDvbt2Location.mode);
         mMSBF_GET08_O(file.pui8Content,offset,stLocalDialogArea.stDvbt2Location.guard);
         mMSBF_GET08_O(file.pui8Content,offset,stLocalDialogArea.stDvbt2Location.PLP_ID);
         mTBOX_TRACE((kTBOX_NIV_3,"[%s] - stLocalDialogArea.stDvbt2Location=....\n Freq=%d, BW=%d, mode=%d, guard=%d, PLP_ID=%d \n",
                      TBOX_FunctionName,
                      stLocalDialogArea.stDvbt2Location.frequency,
                      stLocalDialogArea.stDvbt2Location.bandwidth,
                      stLocalDialogArea.stDvbt2Location.mode,
                      stLocalDialogArea.stDvbt2Location.guard,
                      stLocalDialogArea.stDvbt2Location.PLP_ID));
      }
#endif    
      else /* networktype does not match */
      {
         mTBOX_TRACE((kTBOX_NIV_WARNING,"[%s] - stLocalDialogArea.stDLCycle.u8NetWorkType =%d  -> NOT a known Location\n",
                      TBOX_FunctionName, stLocalDialogArea.stDLCycle.u8NetWorkType));
          /* @@@@ Do not break the process 
         rc = eNVM_IBTL_ERROR_READ;
         @@@@ */

      }
      /* Check the version */
      if ((stLocalDialogArea.u32syntaxVersion > kNVM_IBTLi_FDA_CURR_VERSION)
         || (stLocalDialogArea.u32syntaxVersion < kNVM_IBTLi_FDA_MIN_VERSION))
      {
         /* Warning, continue to set default values */
         mTBOX_TRACE((kTBOX_NIV_WARNING,"%s() ERROR: "
                      "Version %#x of Update Requests "
                      "File {%#x,%#x} invalid (wait %#x)\n",
                      TBOX_FunctionName,
                      stLocalDialogArea.u32syntaxVersion,
                      vNVM_IBTLi_stAreaLocDialogArea.addressOffset,
                      vNVM_IBTLi_stAreaLocDialogArea.size,
                      kNVM_IBTLi_FDA_CURR_VERSION));

         rc = eNVM_IBTL_ERROR_BAD_VERSION;
      }    
   }

   


   /* Parse and Check CRC32 */
   if (rc == eNVM_IBTL_NO_ERROR)
   {
      /* Compute crc */
      calc_crc = NVM_IBTLi_ComputeCRC(file.pui8Content,offset,0,TRUE);

      /* Extract crc */
      mMSBF_GET32_O(file.pui8Content,offset,crc32);

      mTBOX_TRACE((kTBOX_NIV_3,"- crc32.................%#x,      calc_crc : %#x\n",crc32,calc_crc));

      /* Check the crc32 */
      if (crc32 != calc_crc)
      {
         /* Warning set default value and continue */
         mTBOX_TRACE((kTBOX_NIV_WARNING,"%s() ERROR: "
                      "CRC32 0x%08X of dialog area "
                      "File {%#x,%#x} invalid (compute 0x%08X)\n",
                      TBOX_FunctionName, crc32,
                      vNVM_IBTLi_stAreaLocDialogArea.addressOffset,
                      vNVM_IBTLi_stAreaLocDialogArea.size,
                      calc_crc));
         rc = eNVM_IBTL_ERROR_BAD_CRC;
      }
   }
   else
   {
	   tCFGF_Configuration *pConfigFile;
   		uint32_t usageID_CFGF;

		/* Reform the file and rewrite it */
		mTBOX_TRACE((kTBOX_NIV_WARNING,
				   "[%s] Error read or corruption => "
				   "Rewrite File with default values\n",
				   TBOX_FunctionName));

		/* reset file */
		pxstDialogArea->u32syntaxVersion = kNVM_IBTLi_FDA_CURR_VERSION;
		pxstDialogArea->stDLCycle.u32UpdateID = 0xFFFFFFFF;
		pxstDialogArea->stDLCycle.u16DsmccPid = 0xFFFF;
		pxstDialogArea->stDLCycle.u8DLState = 0x0;
		pxstDialogArea->stDLCycle.u8ErrorCode = 0x0;
        /* get default usage ID from the config file */
    	pConfigFile = CFGF_Read(kCFGF_ID_PROD_ID);
		pxstDialogArea->stDLCycle.u8UsageId = pConfigFile->stProductIDs.ui8DefaultUsageId;
		pxstDialogArea->stDLCycle.u8NetWorkType = 0xFF;
        
        CFGF_Free(pConfigFile);

		offset   = 0;
       
		/* write header */
		mMSBF_SET32_O(file.pui8Content,offset, kNVM_IBTLi_FDA_CURR_VERSION);
		mMSBF_SET32_O(file.pui8Content,offset,pxstDialogArea->stDLCycle.u32UpdateID);
		mMSBF_SET16_O(file.pui8Content,offset,pxstDialogArea->stDLCycle.u16PmtPid);
		mMSBF_SET16_O(file.pui8Content,offset,pxstDialogArea->stDLCycle.u16DsmccPid);
		mMSBF_SET08_O(file.pui8Content,offset,pxstDialogArea->stDLCycle.u8DLState);
		mMSBF_SET08_O(file.pui8Content,offset,pxstDialogArea->stDLCycle.u8ErrorCode);
		mMSBF_SET08_O(file.pui8Content,offset,pxstDialogArea->stDLCycle.u8UsageId);
		mMSBF_SET08_O(file.pui8Content,offset,pxstDialogArea->stDLCycle.u8NetWorkType);

		/* compute CRC and set it */
		calc_crc = NVM_IBTLi_ComputeCRC(file.pui8Content,offset,0,TRUE);

		mMSBF_SET32_O(file.pui8Content,offset,calc_crc);
		/* force writing of file */
		rewrite = TRUE;
   }


   /* 
   ** Read done
   */
   if (uAccessMode == eREAD)
   {   
      if (rc == eNVM_IBTL_NO_ERROR)
      {
         /* Copy content to parameter structure */
         if (pxstDialogArea != NULL)
         {
            memcpy((void*)pxstDialogArea, (void*)&stLocalDialogArea, sizeof(stLocalDialogArea));
         }
      }
   }
   else 
   {
      if (pxstDialogArea != NULL)
      {
         rewrite = TRUE;
      }
   }
   
      /*
      ** write
      */
   if (rewrite == TRUE)
   {

      offset   = 0;
      
      /* Write header */
      mMSBF_SET32_O(file.pui8Content,offset, kNVM_IBTLi_FDA_CURR_VERSION);

      mMSBF_SET32_O(file.pui8Content,offset,pxstDialogArea->stDLCycle.u32UpdateID);
      mMSBF_SET16_O(file.pui8Content,offset,pxstDialogArea->stDLCycle.u16PmtPid);
      mMSBF_SET16_O(file.pui8Content,offset,pxstDialogArea->stDLCycle.u16DsmccPid);
      mMSBF_SET08_O(file.pui8Content,offset,pxstDialogArea->stDLCycle.u8DLState);
      mMSBF_SET08_O(file.pui8Content,offset,pxstDialogArea->stDLCycle.u8ErrorCode);
      mMSBF_SET08_O(file.pui8Content,offset,pxstDialogArea->stDLCycle.u8UsageId);
      mMSBF_SET08_O(file.pui8Content,offset,pxstDialogArea->stDLCycle.u8NetWorkType);

#if defined(MEDIUM_SAT)
      mMSBF_SET32_O(file.pui8Content,offset,pxstDialogArea->stDvbsLocation.BIS_frequency);
      mMSBF_SET32_O(file.pui8Content,offset,pxstDialogArea->stDvbsLocation.symbol_rate);
      flags=0;
      if (pxstDialogArea->stDvbsLocation.continuous_tone_on_flag != FALSE) 
         flags = (uint8_t)flags|0x04;
      if (pxstDialogArea->stDvbsLocation.lnb_power_on_flag != FALSE) 
         flags = (uint8_t)flags|0x02;
      if (pxstDialogArea->stDvbsLocation.vertical_polarisation_flag != FALSE) 
         flags = (uint8_t)flags|0x01;
      mMSBF_SET08_O(file.pui8Content,offset,flags);
      mMSBF_SET08_O(file.pui8Content,offset,pxstDialogArea->stDvbsLocation.modulation_type);
      mMSBF_SET08_O(file.pui8Content,offset,pxstDialogArea->stDvbsLocation.fecrate);
      mMSBF_SET08_O(file.pui8Content,offset,pxstDialogArea->stDvbsLocation.switch_selection);
#elif defined(MEDIUM_TER)
      if (pxstDialogArea->stDLCycle.u8NetWorkType == kNVM_IBTL_NWT_DVB_T)
      {
         mMSBF_SET32_O(file.pui8Content,offset,pxstDialogArea->stDvbtLocation.frequency);
         mMSBF_SET32_O(file.pui8Content,offset,pxstDialogArea->stDvbtLocation.bandwidth);
         mMSBF_SET08_O(file.pui8Content,offset,pxstDialogArea->stDvbtLocation.modulation);
         mMSBF_SET08_O(file.pui8Content,offset,pxstDialogArea->stDvbtLocation.mode);
         mMSBF_SET08_O(file.pui8Content,offset,pxstDialogArea->stDvbtLocation.guard);
         mMSBF_SET08_O(file.pui8Content,offset,pxstDialogArea->stDvbtLocation.hierarchy);
         mMSBF_SET08_O(file.pui8Content,offset,pxstDialogArea->stDvbtLocation.priority);
         mMSBF_SET08_O(file.pui8Content,offset,pxstDialogArea->stDvbtLocation.code_rate_HP);
         mMSBF_SET08_O(file.pui8Content,offset,pxstDialogArea->stDvbtLocation.code_rate_LP);
         mMSBF_SET08_O(file.pui8Content,offset,pxstDialogArea->stDvbtLocation.freq_offset);
      }
      else if (pxstDialogArea->stDLCycle.u8NetWorkType == kNVM_IBTL_NWT_DVB_T2)
      {
         mMSBF_SET32_O(file.pui8Content,offset,pxstDialogArea->stDvbt2Location.frequency);
         mMSBF_SET32_O(file.pui8Content,offset,pxstDialogArea->stDvbt2Location.bandwidth);
         mMSBF_SET08_O(file.pui8Content,offset,pxstDialogArea->stDvbt2Location.mode);
         mMSBF_SET08_O(file.pui8Content,offset,pxstDialogArea->stDvbt2Location.guard);
         mMSBF_SET08_O(file.pui8Content,offset,pxstDialogArea->stDvbt2Location.PLP_ID);
      }
#elif defined(MEDIUM_CAB)
      mMSBF_SET32_O(file.pui8Content,offset,pxstDialogArea->stDvbcLocation.BIS_frequency);
      mMSBF_SET32_O(file.pui8Content,offset,pxstDialogArea->stDvbcLocation.symbol_rate);
      mMSBF_SET08_O(file.pui8Content,offset,pxstDialogArea->stDvbcLocation.modulation_type);
#endif

      /* Compute CRC and set it */
      calc_crc = NVM_IBTLi_ComputeCRC(file.pui8Content,offset,0,TRUE);

      mMSBF_SET32_O(file.pui8Content,offset,calc_crc);

      /* Write the new file content */
      n = NVM_IBTLi_FileWrite(&file,NULL,-1);










      if (n != file.i32Size)
      {
         /* Error */
         mTBOX_TRACE((kTBOX_NIV_WARNING,"%s() ERROR: "
                      "Write Update Requests File {%#x,%#x} failure "
                      "(write %d/%d, crc32=0x%08X, data size=%d)\n",
                      TBOX_FunctionName,
                      vNVM_IBTLi_stAreaLocDialogArea.addressOffset,
                      vNVM_IBTLi_stAreaLocDialogArea.size,
                      n,file.i32Size,calc_crc,offset));
            /* Set/Unset mode : Return a write error */
            rc = eNVM_IBTL_ERROR_WRITE;
      }
      else
      {
         /* OKAY */
         mTBOX_TRACE((kTBOX_NIV_1,
                      "[%s] Table of requests (crc32=0x%08X) written "
                      "in Update Requests File {%#x,%#x} (data size=%d)\n",
                      TBOX_FunctionName,calc_crc,
                      vNVM_IBTLi_stAreaLocDialogArea.addressOffset,
                      vNVM_IBTLi_stAreaLocDialogArea.size,offset));
            rc = eNVM_IBTL_NO_ERROR;
#if defined (__NVM_IBTL_DUMP_FILE__)
      NVM_IBTLi_DumpFile(file.pui8Content, kNVM_IBTLi_FDA_FILE_SIZE,
                         &vNVM_IBTLi_stAreaLocDialogArea,
                         TBOX_FunctionName,"Update Request File","WRITE");
#endif
      }
   }

   /* Close the file */
   NVM_IBTLi_FileClose(&file);

   return(rc);
}


#else /* unused */

static tNVM_IBTL_Status NVM_IBTLi_AccessDialogArea
(
   tNVM_IBTLi_AccessMode uAccessMode,
   tNVM_IBTL_DialogArea *pxstDialogArea
)
{
   mTBOX_FCT_ENTER(NVM_IBTLi_AccessDialogArea);

   if (!vNVM_IBTLi_isInit)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"%s(...) ERROR: "
                   "NVM Interface with bootloader is not initialized\n",
                   TBOX_FunctionName));
      mTBOX_RETURN(eNVM_IBTL_ERROR);
   }

   if (pxstDialogArea == NULL)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"%s(...) ERROR: "
                   "All in/out pointers undefined\n",
                   TBOX_FunctionName));
      mTBOX_RETURN(eNVM_IBTL_ERROR);
   }

   /* Open the file */
#ifdef PRODUCT_USE_MSTAR
   rc = NVM_IBTLi_FileOpen(&file,&vNVM_IBTLi_stAreaLocDialogArea,
           sizeof(CH_UpdateInfo_t));
   if (rc != eNVM_IBTL_NO_ERROR)
   {
       mTBOX_TRACE((kTBOX_NIV_CRITICAL,"%s() ERROR: "
               "Open dialog area File {%#x,%#x} failure "
               "(error %d, size %u bytes)\n",
               TBOX_FunctionName,
               vNVM_IBTLi_stAreaLocDialogArea.addressOffset,
               vNVM_IBTLi_stAreaLocDialogArea.size,rc,
               kNVM_IBTLi_FUR_FILE_SIZE));
       mTBOX_RETURN(rc);
   }
#endif
   mTBOX_RETURN(eNVM_IBTL_NO_ERROR);
}
#endif

/**
 *  @brief
 *   read the doawnload status from the dialog area
 *
 *  @param[out] *pUpdateID
 *       the update ID (FileID) of current download cycle
 *
 *  @param[out] *peStatus
 *       0 No download cycle
 *       1 start download or download not completed
 *       2 download competed
 *
 *  @param[out] *peError
 *       0 No error
 *
 *  @retval TRUE
 *       action successful.
 *
 *  @retval FALSE
 *       Action failure.
 *
*/
bool NVM_IBTL_ReadDownloadStatus(uint32_t *pUpdateID, uint32_t *peStatus, uint32_t *peError)
{
   tNVM_IBTL_Status rc;
   bool bRC;

   tNVM_IBTL_DialogArea	stLocalDialogArea;
   mTBOX_FCT_ENTER(NVM_IBTL_ReadDownloadStatus);
   rc=NVM_IBTLi_AccessDialogArea(TRUE,&stLocalDialogArea);
   if (rc==eNVM_IBTL_NO_ERROR)
   {
      if (pUpdateID!=NULL)
      {
         *pUpdateID=stLocalDialogArea.stDLCycle.u32UpdateID;
      }
      if (peStatus!=NULL)
      {
         *peStatus=stLocalDialogArea.stDLCycle.u8DLState;
      }
      if (peError!=NULL)
      {
         *peError=stLocalDialogArea.stDLCycle.u8ErrorCode;
      }
      bRC = TRUE;
   }
   else
   {
      if (pUpdateID!=NULL)
      {
         *pUpdateID=0xFFFFFFFF;
      }
      if (peStatus!=NULL)
      {
         *peStatus=0;
      }
      if (peError!=NULL)
      {
         *peError=0;
      }
      bRC = FALSE;
   }
   mTBOX_RETURN(bRC);
}
/**
 *  @brief
 *   reset the download status from the dialog area (update ID=0, status=0, error=0)
 *
 *
 *  @param -none -
 *
 *  @retval TRUE
 *       action successful.
 *
 *  @retval FALSE
 *       Action failure.
 *
*/
bool NVM_IBTL_ResetDownloadStatus(void)
{
   mTBOX_FCT_ENTER(NVM_IBTL_ResetDownloadStatus);

   mTBOX_RETURN(TRUE);
}



/**
 *  @brief
 *   Update the dialog area file with state, update ID and error code
 *
 *
 *  @param[in] u32xUpdateID
 *   the update ID found in a carousel or joker UpdateID=0xFFFFFFFF
 *
 *  @param[in] u8xErrorCode
 *   the errorcode
 *
 *  @param[in] u8xDLState
 *   the downloader status
 *
 *  @param[in] u8xDefaultUsageId
 *   the usage ID to store if not yet valid
 *
 *  @param[in] u8xDefaultNetworkType
 *   the network type to store if not valid (0xFF)
*
 * *  @retval eNVM_IBTL_NO_ERROR
 *       action successful.
 *
 *  @retval eNVM_IBTL_ERROR
 *       Action failure.
 *
*/
bool NVM_IBTL_UpdateDownloadStatus(uint32_t u32xUpdateID, uint8_t u8xErrorCode, uint8_t u8xDLState, uint8_t u8xDefaultUsageId, uint8_t u8xDefaultNetworkType )
{
   tNVM_IBTL_Status rc;
   bool bRC;
   tNVM_IBTL_DialogArea	stLocalDialogArea;

   mTBOX_FCT_ENTER(NVM_IBTL_UpdateDownloadStatus);

   rc=NVM_IBTLi_AccessDialogArea(TRUE,&stLocalDialogArea);

   stLocalDialogArea.stDLCycle.u32UpdateID=u32xUpdateID;
   stLocalDialogArea.stDLCycle.u8DLState=u8xDLState;
   stLocalDialogArea.stDLCycle.u8ErrorCode=u8xErrorCode;
   if (rc!=eNVM_IBTL_NO_ERROR)
   {
      /* Not intialized so keep u8NetWorkType=0xFF */
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,
                      "%s() Wrong initialized dialog area file ==> Default : u8NetWorkType=%d u8UsageId=%d \n",
                      TBOX_FunctionName, u8xDefaultNetworkType, u8xDefaultUsageId));
      stLocalDialogArea.stDLCycle.u8NetWorkType = u8xDefaultNetworkType;
      stLocalDialogArea.stDLCycle.u8UsageId = u8xDefaultUsageId;
   }
   rc=NVM_IBTLi_AccessDialogArea(FALSE,&stLocalDialogArea);

   if (rc==eNVM_IBTL_NO_ERROR)
   {
      bRC = TRUE;
   }
   else
   {
      bRC = FALSE;
   }
   mTBOX_RETURN(bRC);
}

/**
 *  @brief
 *   read the usageId from the dialog area
 *
 *  @param[out] *pUsageID
 *       the usage ID of box
 *
 *  @retval TRUE
 *       action successful.
 *
 *  @retval FALSE
 *       action failure.
 *
*/
bool NVM_IBTL_ReadUsageId(uint32_t *pUsageID)
{
   tNVM_IBTL_Status rc;
   bool bRC;

   tNVM_IBTL_DialogArea	stLocalDialogArea;
   mTBOX_FCT_ENTER(NVM_IBTL_ReadUsageId);
   rc=NVM_IBTLi_AccessDialogArea(TRUE,&stLocalDialogArea);
   if (rc==eNVM_IBTL_NO_ERROR)
   {
      if (pUsageID!=NULL)
      {
         *pUsageID=stLocalDialogArea.stDLCycle.u8UsageId;
      }
      bRC = TRUE;
   }
   else
   {
      bRC = FALSE;
   }
   mTBOX_RETURN(bRC);
}
/**
 *  @brief
 *   update the usageId in the dialog area
 *
 *  @param[in] u8xUsageID
 *       the usage ID of box
 *
 *  @retval TRUE
 *       action successful.
 *
 *  @retval FALSE
 *       action failure.
 *
*/
bool NVM_IBTL_UpdateUsageId(uint8_t u8xUsageID)
{
   tNVM_IBTL_Status rc;
   bool bRC;
   tNVM_IBTL_DialogArea	stLocalDialogArea;

   mTBOX_FCT_ENTER(NVM_IBTL_UpdateUsageId);
   rc=NVM_IBTLi_AccessDialogArea(TRUE,&stLocalDialogArea);

   stLocalDialogArea.stDLCycle.u8UsageId = u8xUsageID;

   if (rc!=eNVM_IBTL_NO_ERROR)
   {
	   /* Not initialized */
	   mTBOX_TRACE((kTBOX_NIV_CRITICAL,
					 "%s() Wrong initialized dialog area file ==> Default : u8NetWorkType=%d\n",
					 TBOX_FunctionName, kNVM_IBTLi_DefaultNetworkType));
	   stLocalDialogArea.stDLCycle.u8NetWorkType = kNVM_IBTLi_DefaultNetworkType;
	   stLocalDialogArea.stDLCycle.u8ErrorCode = 0;
	   stLocalDialogArea.stDLCycle.u8DLState = 0;
	   stLocalDialogArea.stDLCycle.u32UpdateID = 0xFFFFFFFF;
	   stLocalDialogArea.stDLCycle.u16PmtPid = 0x1FFF;
	   stLocalDialogArea.stDLCycle.u16DsmccPid = 0x1FFF;
   }

   rc=NVM_IBTLi_AccessDialogArea(FALSE,&stLocalDialogArea);
   if (rc==eNVM_IBTL_NO_ERROR)
   {
      bRC = TRUE;
   }
   else
   {
      bRC = FALSE;
   }
   mTBOX_RETURN(bRC);
}


/**
 *  @brief
 *   get the dialog area file
 *
 *
 *  @param[out] *pxstDialogArea
 *    fully filled in from the dialog file
 *
 *  @retval eNVM_IBTL_NO_ERROR
 *       action successful.
 *
 *  @retval eNVM_IBTL_ERROR
 *       Action failure.
 *
*/
tNVM_IBTL_Status   NVM_IBTL_GetDialogFile(tNVM_IBTL_DialogArea *pxstDialogArea)
{
   tNVM_IBTL_Status rc = eNVM_IBTL_NO_ERROR;
   mTBOX_FCT_ENTER(NVM_IBTL_GetDialogFile);
   if (pxstDialogArea == NULL)
   {
         mTBOX_TRACE((kTBOX_NIV_CRITICAL,
                      "[%s] NULL parameter for pxstDialogArea\n",
                      TBOX_FunctionName));
      rc = eNVM_IBTL_ERROR;
   }
   else
   {
      rc = NVM_IBTLi_AccessDialogArea(TRUE, pxstDialogArea);
      if (rc != eNVM_IBTL_NO_ERROR)
      {
         mTBOX_TRACE((kTBOX_NIV_WARNING,
                      "[%s] NVM_IBTLi_AccessDialogArea(READ) return %d\n",
                      TBOX_FunctionName, rc));
         rc = eNVM_IBTL_ERROR;
      }
      else
      {
         mTBOX_TRACE((kTBOX_NIV_1,
                      "[%s] NVM_IBTLi_AccessDialogArea(READ) SUCCEEDED\n",
                      TBOX_FunctionName, rc));
      }
   }
   /* check consistency and reliability */
   if (rc == eNVM_IBTL_NO_ERROR)
   {
      /* check network type */
      switch (pxstDialogArea->stDLCycle.u8NetWorkType)
      {
#if defined(MEDIUM_TER)
         case kNVM_IBTL_NWT_DVB_T:
         case kNVM_IBTL_NWT_DVB_T2:
#elif defined(MEDIUM_SAT)
         case kNVM_IBTL_NWT_DVB_S:
         case kNVM_IBTL_NWT_DVB_S2:
#elif defined(MEDIUM_CAB)
         case kNVM_IBTL_NWT_DVB_C:
#endif
         mTBOX_TRACE((kTBOX_NIV_1,
                   "[%s]() network type is 0x%x\n",
                   TBOX_FunctionName, pxstDialogArea->stDLCycle.u8NetWorkType));
         break;
         default:
         mTBOX_TRACE((kTBOX_NIV_WARNING,
                   "[%s]() Invalid network type 0x%x\n",
                   TBOX_FunctionName, pxstDialogArea->stDLCycle.u8NetWorkType));
         rc = eNVM_IBTL_ERROR_BAD_LOCATION;
      }
   }
   if (rc == eNVM_IBTL_NO_ERROR)
   {
      /* check PID */
	  if ((pxstDialogArea->stDLCycle.u16DsmccPid >= 0x1FFF)
	  ||(pxstDialogArea->stDLCycle.u16DsmccPid <= 0x0014))
	  {
         mTBOX_TRACE((kTBOX_NIV_WARNING,
                   "[%s]() Invalid dsmcc PID 0x%04x\n",
                   TBOX_FunctionName, pxstDialogArea->stDLCycle.u16DsmccPid));
         rc = eNVM_IBTL_ERROR_BAD_PID;
      }
   }
   if (rc == eNVM_IBTL_NO_ERROR)
   {
      mTBOX_TRACE((kTBOX_NIV_1,
                   "[%s]() SUCCESS\n",
                   TBOX_FunctionName));
   }
   mTBOX_RETURN(rc);
}


/**
 *  @brief
 *   Prepare dialog area file and update request file for a nominal download
 *
 *
 *  @param [in] file_ID
 *
 *  @param [in] *pstDownloadDialogFile
 *		This structure shall be filled in with
 *			- stDLCycle.u32UpdateID
 *			- stDLCycle.u8NetWorkType
 *			(if a broadcast network type is specified)
 *				- stIbtlDialog.stDLCycle.u16DsmccPid
 *				- tNVM_IBTL_Dvb-X-Location (depending of current  )
 *
 *  @retval TRUE
 *       action successful.
 *
 *  @retval FALSE
 *       Action failure.
 *
*/
bool NVM_IBTL_LaunchDownloader(tNVM_IBTL_DialogArea *pstDownloadDialogFile)
{
   tNVM_IBTL_Status rc;
   bool bRC;
   uint32_t u32UpdateReason = kNVM_IBTL_UPDATE_REASON_NEW_VERSION;
   tNVM_IBTL_DialogArea	stLocalDialogArea;

   mTBOX_FCT_ENTER(NVM_IBTL_LaunchDownloader);

   /* check parameters */
   if (pstDownloadDialogFile == NULL)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"%s() ERROR: NO pstDownloadDialogFile provided \n", TBOX_FunctionName));
	   mTBOX_RETURN(FALSE);
   }

#if defined(USE_TDAL_SSU)
    tTDAL_SSU_ControlParameters ssuCtrParam = {0};
    tTDAL_SSU_Error             ssuError;

    ssuCtrParam.bInStandBy  = pstDownloadDialogFile->useStdbyState;
    ssuCtrParam.uiPid       = 0x1FFF;

    ssuCtrParam.uiFreqency	= pstDownloadDialogFile->stDvbcLocation.BIS_frequency;
    ssuCtrParam.uiModulation	= pstDownloadDialogFile->stDvbcLocation.modulation_type;
    ssuCtrParam.uiSymbolRate	= pstDownloadDialogFile->stDvbcLocation.symbol_rate;

    ssuCtrParam.uiPid		= pstDownloadDialogFile->stDLCycle.u16DsmccPid;
    ssuCtrParam.uiUpdateID	= pstDownloadDialogFile->stDLCycle.u32UpdateID;

    ssuError = TDAL_SSU_SetParameters(&ssuCtrParam);

    mTBOX_RETURN(ssuError == eTDAL_SSU_NO_ERROR ? TRUE : FALSE);
#endif

#if defined(USE_TDAL_OTA)
    tTDAL_OTA_ErrorCode tdal_ota_err = eTDAL_OTA_STATUS_NO_ERROR;

   if((pstDownloadDialogFile->useStdbyState) && (pstDownloadDialogFile->isSTBinStdby))
   {
       tdal_ota_err = TDAL_OTA_Initiate(eTDAL_NORMAL_OTA, eTDAL_STANDBY_ON);
   }
   else
   {
       tdal_ota_err = TDAL_OTA_Initiate(eTDAL_NORMAL_OTA, eTDAL_STANDBY_OFF);
   }

   if (tdal_ota_err == eTDAL_OTA_STATUS_ERROR)
   {
       bRC = FALSE;
       mTBOX_TRACE((kTBOX_NIV_CRITICAL,"%s() ERROR: TDAL_OTA_Initiate FAILS \n", TBOX_FunctionName));
   }
   else
   {
       bRC = TRUE;
       mTBOX_TRACE((kTBOX_NIV_CRITICAL,"%s() ERROR: TDAL_OTA_Initiate SUCCEEDS \n", TBOX_FunctionName));
   }

   mTBOX_RETURN(bRC);
#endif

   switch (pstDownloadDialogFile->stDLCycle.u8NetWorkType)
   {
#if defined(MEDIUM_SAT)
   	case kNVM_IBTL_NWT_DVB_S:
   	case kNVM_IBTL_NWT_DVB_S2:
   		break;
#elif defined(MEDIUM_CAB)
   	case kNVM_IBTL_NWT_DVB_C:
   		break;
#elif defined(MEDIUM_TER)
   	case kNVM_IBTL_NWT_DVB_T:
  	case kNVM_IBTL_NWT_DVB_T2:
   		break;
#endif
/* external download not available from application */
   	case kNVM_IBTL_NWT_SERIAL:
   	case kNVM_IBTL_NWT_USB:
   	case kNVM_IBTL_NWT_URL:
   		u32UpdateReason = kNVM_IBTL_UPDATE_REASON_EXTERNAL;
   		break;
   	default:
      	mTBOX_TRACE((kTBOX_NIV_WARNING,
	      				"%s() ERROR: unknown u8NetWorkType=%#x\n",
	       				TBOX_FunctionName,
	       				pstDownloadDialogFile->stDLCycle.u8NetWorkType));
	   	mTBOX_RETURN(FALSE);
   } /* switch (pstDownloadDialogFile->stDLCycle.u8NetWorkType) */

   /* read current dialog file*/
   rc = NVM_IBTLi_AccessDialogArea(TRUE,&stLocalDialogArea);
   /* drop rc, because no mater if current dialog file is wrong or right */
   /*
   ** fill-in download cycle
   */
   stLocalDialogArea.stDLCycle.u32UpdateID
   	= pstDownloadDialogFile->stDLCycle.u32UpdateID;
   stLocalDialogArea.stDLCycle.u16PmtPid = 0x1FFF; /*  fake */
   stLocalDialogArea.stDLCycle.u16DsmccPid
   	= pstDownloadDialogFile->stDLCycle.u16DsmccPid;
   stLocalDialogArea.stDLCycle.u8DLState=1; /* DL_START */
   stLocalDialogArea.stDLCycle.u8ErrorCode=0;
   stLocalDialogArea.stDLCycle.u8UsageId
   	= pstDownloadDialogFile->stDLCycle.u8UsageId;
   stLocalDialogArea.stDLCycle.u8NetWorkType
   	= pstDownloadDialogFile->stDLCycle.u8NetWorkType;
   /*
   ** Fill in DVB location
   */
   switch (pstDownloadDialogFile->stDLCycle.u8NetWorkType)
   {
#if defined(MEDIUM_SAT)
   	case kNVM_IBTL_NWT_DVB_S:
   	case kNVM_IBTL_NWT_DVB_S2:
      	memcpy((void*)&(stLocalDialogArea.stDvbsLocation),
      			 (void*)&(pstDownloadDialogFile->stDvbsLocation),
      			 sizeof (tNVM_IBTL_DvbsLocation));
   		break;
#elif defined(MEDIUM_CAB)
   	case kNVM_IBTL_NWT_DVB_C:
      	memcpy((void*)&(stLocalDialogArea.stDvbcLocation),
      			 (void*)&(pstDownloadDialogFile->stDvbcLocation),
      			 sizeof (tNVM_IBTL_DvbcLocation));
   		break;
#elif defined(MEDIUM_TER)
   	case kNVM_IBTL_NWT_DVB_T:
  		case kNVM_IBTL_NWT_DVB_T2:
      	memcpy((void*)&(stLocalDialogArea.stDvbtLocation),
      			 (void*)&(pstDownloadDialogFile->stDvbtLocation),
      			 sizeof (tNVM_IBTL_DvbtLocation));
   		break;
#endif
   	case kNVM_IBTL_NWT_SERIAL:
   	case kNVM_IBTL_NWT_USB:
   	case kNVM_IBTL_NWT_URL:
   	default:
   		/* do not erase DVB location content */
   		break;
   } /* switch (pstDownloadDialogFile->stDLCycle.u8NetWorkType) */
   /*
   ** Write the dialog file
   */
   rc=NVM_IBTLi_AccessDialogArea(FALSE,&stLocalDialogArea);
   if (rc!=eNVM_IBTL_NO_ERROR)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"%s() ERROR: dialog area file writing FAILURE !\n", TBOX_FunctionName));
      mTBOX_RETURN(FALSE);
   }

   /*
   ** Update the update request file
   */
/*   stLocalUpdRequest.ui32ClassId = kNVM_IBTL_CLASS_ID_USER;
   stLocalUpdRequest.ui32UpdateReason = u32UpdateReason;
   rc = NVM_IBTL_SetUpdateRequest(&stLocalUpdRequest);
 */  

#ifdef NEWLAND_BOOTLOADER
	if (rc==eNVM_IBTL_NO_ERROR)
	{
		FS_CFG_ResetUnpackFS();
		
	}

#endif

   if (rc==eNVM_IBTL_NO_ERROR)
   {
      bRC = TRUE;
   }
   else
   {
      bRC = FALSE;
   }

   mTBOX_RETURN(bRC);
}

/**
 *  @brief
 *    Open a NVM file and allocate its internal read/write buffer
 *    with its size a priori if it is defined.
 *
 *  @pre
 *    This module is initialized (check by caller).
 *
 *  @param[out] pxstFile
 *       File descriptor opened with content buffer allocated if the
 *       size is defined.
 *
 *  @param[in] pxstLocation
 *       NVM location of file to open.
 *
 *  @param[in] xi32Size
 *       Size of content to allocate.
 *       If < 1, not allocation => the caller manage its read/write buffer.
 *
 *  @retval eNVM_IBTL_NO_ERROR
 *       Open successful.
 *
 *  @retval eNVM_IBTL_ERROR_MEMORY
 *       Content allocation failure.
 *
 *  @retval eNVM_IBTL_ERROR
 *       Bad parameters.
 *
 *  @remarks
 *    -# If defined, the size of content to allocate must be inferior or equal
 *       to the area size defined in the file location. If it is superior
 *       return the bad parameter ERROR with status eNVM_IBTL_ERROR.
*/
static tNVM_IBTL_Status NVM_IBTLi_FileOpen
(
   tNVM_IBTLi_File     *pxstFile,
   tFLASH_CFG_Location *pxstLocation,
   int32_t              xi32Size
)
{
   TKEL_err tkerr;

   mTBOX_FCT_ENTER(NVM_IBTLi_FileOpen);

   if (pxstFile == NULL)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,
                   "%s() ERROR: File descriptor undefined\n",
                   TBOX_FunctionName));
      mTBOX_RETURN(eNVM_IBTL_ERROR);
   }
   if (pxstLocation == NULL)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,
                   "%s() ERROR: File location undefined\n",
                   TBOX_FunctionName));
      mTBOX_RETURN(eNVM_IBTL_ERROR);
   }

   pxstFile->pstLocation = NULL;
   pxstFile->pui8Content = NULL;
   pxstFile->i32Size     = -1;

   /* Content allocation if necessary and OK */
   if (xi32Size > 0)
   {
      if ((uint32_t)xi32Size > pxstLocation->size)
      {
         mTBOX_TRACE((kTBOX_NIV_CRITICAL,
                      "%s() ERROR: Content size to allocate (%d=%#x) "
                      "is too big for NVM File {%#x,%#x}\n",
                      TBOX_FunctionName,xi32Size,
                      (uint32_t)xi32Size,
                      pxstLocation->addressOffset,
                      pxstLocation->size));
         mTBOX_RETURN(eNVM_IBTL_ERROR);
      }

      /* Allocation */
      tkerr = TKEL_Malloc((TKEL_msize)xi32Size,
                          (void**)&pxstFile->pui8Content);
      if (pxstFile->pui8Content == NULL)
      {
         mTBOX_TRACE((kTBOX_NIV_CRITICAL,
                      "%s() ERROR: Allccation %u bytes to store content of "
                      "NVM File {%#x,%#x} failure (tkel error %d)\n",
                      TBOX_FunctionName,xi32Size,
                      pxstLocation->addressOffset,
                      pxstLocation->size,tkerr));
         mTBOX_RETURN(eNVM_IBTL_ERROR_MEMORY);
      }

      pxstFile->i32Size = xi32Size;
   }

   /* Okay */
   pxstFile->pstLocation = pxstLocation;

   mTBOX_TRACE((kTBOX_NIV_2,
                "[%s] NVM File {%#x,%#x} OPENED (%d bytes allocated)\n",
                TBOX_FunctionName,pxstFile->pstLocation->addressOffset,
                pxstFile->pstLocation->size,pxstFile->i32Size));

   mTBOX_RETURN(eNVM_IBTL_NO_ERROR);
}

/**
 *  @brief
 *    Close a NVM file and release its internal read/write buffer
 *    if allocated.
 *
 *  @pre
 *    This module is initialized (check by caller).
 *
 *  @param[in/out] pxstFile
 *       File descriptor to close and reset to undefined value.
 *
 *  @retval eNVM_IBTL_NO_ERROR
 *       Close successful.
 *
 *  @retval eNVM_IBTL_ERROR
 *       Bad parameters.
 *
 *  @remarks
 *    -# A file is opened when the location is defined (not NULL).
 *
 *    -# After close the file descriptor with set with NULL values.
 *
*/
static tNVM_IBTL_Status NVM_IBTLi_FileClose
(
   tNVM_IBTLi_File *pxstFile
)
{
   mTBOX_FCT_ENTER(NVM_IBTLi_FileClose);

   if (pxstFile == NULL)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,
                   "%s() ERROR: File descriptor undefined\n",
                   TBOX_FunctionName));
      mTBOX_RETURN(eNVM_IBTL_ERROR);
   }
   if (pxstFile->pstLocation == NULL)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,
                   "%s() ERROR: File not opened\n",
                   TBOX_FunctionName));
      mTBOX_RETURN(eNVM_IBTL_ERROR);
   }

   mTBOX_TRACE((kTBOX_NIV_2,
                "[%s] NVM File {%#x,%#x} CLOSED (%d bytes allocated)\n",
                TBOX_FunctionName,pxstFile->pstLocation->addressOffset,
                pxstFile->pstLocation->size,pxstFile->i32Size));

   /* Release internal content buffer if defined */
   if (pxstFile->pui8Content != NULL)
   {
      TKEL_Free(pxstFile->pui8Content);
      pxstFile->pui8Content = NULL;
   }

   pxstFile->pstLocation = NULL;
   pxstFile->i32Size     = -1;


   mTBOX_RETURN(eNVM_IBTL_NO_ERROR);
}

/**
 *  @brief
 *    Read in NVM file in a user buffer or in the internal buffer.
 *
 *
 *  @pre
 *    This module is initialized (check by caller).
 *
 *  @param[in] pxstFile
 *       File descriptor to read.
 *
 *  @param[out] pui8xContent
 *       Pointer on read buffer. If undefined (NULL) use
 *       the internal buffer allocated at the opening.
 *
 *  @param[in] xi32SizeMax
 *       Maximum size to read in bytes.
 *       If undefined (<0) and the read buffer is undefined
 *       use the internal size allocated at the opening.
 *
 *  @retval N > 0
 *       Number of bytes read.
 *
 *  @retval N = 0
 *       Read 0 bytes => certainly a read error in TDAL.
 *
 *  @retval N < 0
 *       Bad parameters detected.
 *
 *  @remarks
 *    -# A file is opened when the location is defined (not NULL).
 *
 *    -# To rzad in the internal buffer with the pre-defined size call
 *       this function with the read buffer = NULL and the maximum size = -1.
 *
*/
static int32_t NVM_IBTLi_FileRead
(
   tNVM_IBTLi_File *pxstFile,
   uint8_t         *pui8xContent,
   int32_t          xi32SizeMax
)
{
   uint8_t *ptr = pui8xContent;
   int32_t  max = xi32SizeMax;
   int32_t  n;

   mTBOX_FCT_ENTER(NVM_IBTLi_FileRead);

   if (pxstFile == NULL)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,
                   "%s() ERROR: File descriptor undefined\n",
                   TBOX_FunctionName));
      mTBOX_RETURN(-1);
   }
   if (pxstFile->pstLocation == NULL)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,
                   "%s() ERROR: File not opened\n",
                   TBOX_FunctionName));
      mTBOX_RETURN(-1);
   }

   /* Use internal buffer allocated if content buffer undefined */
   if (pui8xContent == NULL)
   {
      if (pxstFile->pui8Content == NULL)
      {
         mTBOX_TRACE((kTBOX_NIV_CRITICAL,
                      "%s() ERROR: Internal content buffer of NVM File "
                      "{%#x,%#x} not allocated\n",
                      TBOX_FunctionName,pxstFile->pstLocation->addressOffset,
                      pxstFile->pstLocation->size));
         mTBOX_RETURN(-1);
      }

      ptr = pxstFile->pui8Content;

      /* Use default size allocated if size undefined */
      if (xi32SizeMax < 0) max = pxstFile->i32Size;
   }

   /* Check size */
   if (max < 0)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,
                   "%s() ERROR: Impossible to read the negative size %d "
                   "in NVM File {%#x,%#x} not allocated\n",
                   TBOX_FunctionName,max,
                   pxstFile->pstLocation->addressOffset,
                   pxstFile->pstLocation->size));
      mTBOX_RETURN(-1);
   }
   if (max == 0)
   {
      mTBOX_TRACE((kTBOX_NIV_WARNING,
                   "%s() ERROR: Read %d byte in NVM File {%#x,%#x} "
                   "has no interest\n",
                   TBOX_FunctionName,max,
                   pxstFile->pstLocation->addressOffset,
                   pxstFile->pstLocation->size));
      mTBOX_RETURN(0);
   }
   if ((uint32_t)max > pxstFile->pstLocation->size)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,
                   "%s() ERROR: Size to read (%d=%#x) is too big "
                   "in NVM File {%#x,%#x}\n",
                   TBOX_FunctionName,max,(uint32_t)max,
                   pxstFile->pstLocation->addressOffset,
                   pxstFile->pstLocation->size));
      mTBOX_RETURN(-1);
   }

   /* Read... */
   n = (int32_t)TDAL_FLA_Read(pxstFile->pstLocation->addressOffset, ptr ,(uint32_t)max);

   if (n != max)
   {
      mTBOX_TRACE((kTBOX_NIV_WARNING,
                   "%s() ERROR: Read only %d bytes/%d "
                   "in in NVM File {%#x,%#x}\n",
                   TBOX_FunctionName,n,max,
                   pxstFile->pstLocation->addressOffset,
                   pxstFile->pstLocation->size));
      mTBOX_RETURN(n);
   }

   mTBOX_TRACE((kTBOX_NIV_2,
                "[%s] %d bytes/%d READED in NVM File {%#x,%#x}\n",
                TBOX_FunctionName,n,max,
                pxstFile->pstLocation->addressOffset,
                pxstFile->pstLocation->size));

   mTBOX_RETURN(n);
}

/**
 *  @brief
 *    Write data in NVM file from a user buffer or from the internal buffer.
 *
 *
 *  @pre
 *    This module is initialized (check by caller).
 *
 *  @param[in] pxstFile
 *       File descriptor to read.
 *
 *  @param[in] pui8xContent
 *       Pointer on buffer data to write. If undefined (NULL) use
 *       the internal buffer allocated at the opening.
 *
 *  @param[in] xi32Size
 *       Size to write in bytes.
 *       If undefined (<0) and the input buffer is undefined
 *       use the internal size allocated at the opening.
 *
 *  @retval N > 0
 *       Number of bytes written.
 *
 *  @retval N = 0
 *       Read 0 bytes => certainly a write error in TDAL.
 *
 *  @retval N < 0
 *       Bad parameters detected.
 *
 *  @remarks
 *    -# A file is opened when the location is defined (not NULL).
 *
 *    -# To use the internal buffer with the pre-defined size call this function
 *       with the data buffer = NULL and the size = -1.
 *
*/
static int32_t NVM_IBTLi_FileWrite
(
   tNVM_IBTLi_File *pxstFile,
   uint8_t         *pui8xData,
   int32_t          xi32Size
)
{
   uint8_t *ptr = pui8xData;
   int32_t  len = xi32Size;
   int32_t  n;

   mTBOX_FCT_ENTER(NVM_IBTLi_FileWrite);

   if (pxstFile == NULL)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,
                   "%s() ERROR: File descriptor undefined\n",
                   TBOX_FunctionName));
      mTBOX_RETURN(-1);
   }
   if (pxstFile->pstLocation == NULL)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,
                   "%s() ERROR: File not opened\n",
                   TBOX_FunctionName));
      mTBOX_RETURN(-1);
   }

   /* Use internal buffer allocated if input buffer undefined */
   if (pui8xData == NULL)
   {
      if (pxstFile->pui8Content == NULL)
      {
         mTBOX_TRACE((kTBOX_NIV_CRITICAL,
                      "%s() ERROR: Internal content buffer of NVM File "
                      "{%#x,%#x} not allocated\n",
                      TBOX_FunctionName,pxstFile->pstLocation->addressOffset,
                      pxstFile->pstLocation->size));
         mTBOX_RETURN(-1);
      }
      ptr = pxstFile->pui8Content;

      /* Use default size allocated if size undefined */
      if (xi32Size < 0) len = pxstFile->i32Size;
   }

   /* Check size */
   if (len < 0)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,
                   "%s() ERROR: Impossible to write the negative size %d "
                   "in NVM File {%#x,%#x} not allocated\n",
                   TBOX_FunctionName,len,
                   pxstFile->pstLocation->addressOffset,
                   pxstFile->pstLocation->size));
      mTBOX_RETURN(-1);
   }
   if (len == 0)
   {
      mTBOX_TRACE((kTBOX_NIV_WARNING,
                   "%s() ERROR: Write %d byte in NVM File {%#x,%#x} "
                   "has no interest\n",
                   TBOX_FunctionName,len,
                   pxstFile->pstLocation->addressOffset,
                   pxstFile->pstLocation->size));
      mTBOX_RETURN(0);
   }
   if ((uint32_t)len > pxstFile->pstLocation->size)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,
                   "%s() ERROR: Size to write (%d=%#x) is too big "
                   "in NVM File {%#x,%#x}\n",
                   TBOX_FunctionName,len,(uint32_t)len,
                   pxstFile->pstLocation->addressOffset,
                   pxstFile->pstLocation->size));
      mTBOX_RETURN(-1);
   }

   /* Write... */
   n = (int32_t)NVM_IBTLi_FLA_Write(pxstFile->pstLocation->addressOffset, ptr, (uint32_t)len);

   if (n != len)
   {
      mTBOX_TRACE((kTBOX_NIV_WARNING,
                   "%s() ERROR: Write only %d bytes/%d "
                   "in in NVM File {%#x,%#x}\n",
                   TBOX_FunctionName,n,len,
                   pxstFile->pstLocation->addressOffset,
                   pxstFile->pstLocation->size));
      mTBOX_RETURN(n);
   }

   mTBOX_TRACE((kTBOX_NIV_2,
                "[%s] %d bytes/%d WRITTEN in NVM File {%#x,%#x}\n",
                TBOX_FunctionName,n,len,
                pxstFile->pstLocation->addressOffset,
                pxstFile->pstLocation->size));

   mTBOX_RETURN(n);
}

/*
 *  @brief
 *    Synchronize flash sector with chunk content
 *
 *
 *  @param[in] u32xAddress
 *       Adress offset in flash where to burn chunk
 *
 *  @param[in] pu8xBuffer
 *       Buffer containing the chunk data
 *
 *  @param[in] u32xNumberToWrite
 *      chunk size
 *
 *  @retval N > 0
 *       Number of bytes read.
 *
 *  @retval N = 0
 *       Read 0 bytes => failure
 *
 *
 */
static uint32_t NVM_IBTLi_FLA_Write(
    uint32_t u32xAddress,
    uint8_t* pu8xBuffer,
    uint32_t u32xNumberToWrite
)
{
   uint32_t                NumberWritten;
   uint8_t                 BlockNumber;
   uint32_t                BaseAddress;
   uint32_t                BlockSize;
   static uint8_t        *pu8BlockBuffer = NULL;
   uint32_t                NumberRead;
   tTDAL_FLA_ErrorCode     FlashError;
   uint32_t                Offset;

   mTBOX_FCT_ENTER("NVM_IBTL_FLA_Write");

   /* get the block number from the address */
   BlockNumber = TDAL_FLA_GetBlockNumber( u32xAddress );

   /* get the base address of the BlockNumber */
   BaseAddress = TDAL_FLA_GetBlockAddress( BlockNumber );

   /* get the block size */
   BlockSize = TDAL_FLA_GetBlockSize( BlockNumber );
   TKEL_Malloc((TKEL_msize)BlockSize,
                          (void**)&pu8BlockBuffer);
   if ( pu8BlockBuffer == NULL )
   {
        mTBOX_TRACE((kTBOX_NIV_WARNING, "%s: TKEL_Malloc (%d) Failed\n",
            TBOX_FunctionName, BlockSize));
   }
    /* read current block and saved */
    NumberRead = TDAL_FLA_Read(BaseAddress, pu8BlockBuffer, BlockSize);
    if (NumberRead != BlockSize)
    {
        mTBOX_TRACE((kTBOX_NIV_WARNING, "kTBOX_NIV_WARNING: %s() TDAL_FLA_Read failed\n", TBOX_FunctionName));
    }

    Offset = u32xAddress - BaseAddress;

    /* erase the block before write */
    FlashError = TDAL_FLA_Erase( BaseAddress , BlockSize );
    if (FlashError != eTDAL_FLA_NO_ERROR)
    {
        mTBOX_TRACE((kTBOX_NIV_WARNING, "%s(): TDAL_FLA_Erase failed\n", TBOX_FunctionName));
    }

    /*  MERGE chunk content to BlockBuffer */
    memcpy( (uint8_t *) (pu8BlockBuffer+Offset) ,
            (uint8_t *) (pu8xBuffer) ,
            u32xNumberToWrite );

    /* BURN  data to the flash */
    NumberWritten = TDAL_FLA_Write(BaseAddress, pu8BlockBuffer, BlockSize);
    if (NumberWritten != BlockSize)
    {
        mTBOX_TRACE((kTBOX_NIV_WARNING, "%s(): TDAL_FLA_Write failed\n", TBOX_FunctionName));
   }
   TKEL_Free(pu8BlockBuffer);

   mTBOX_RETURN(u32xNumberToWrite);
}

/**
 *  @brief
 *     Computes the 32 bits CRC.
 *     Computing block/block : if first block this function initialize
 *     the CRC, else the caller must set the current CRC computed with
 *     the previous block.
 *
 *  @param[in] pxui8Data
 *       Pointer on block of data.
 *
 *  @param[in] xui32Size
 *       Size in bytes of the data block.
 *
 *  @param[in] xui32CurrCRC
 *       Current CRC computed on previous block.
 *       Not significant if it's the first block.
 *
 *  @param[in] xisFirstBlock
 *       TRUE if it's the first block else FALSE.
 *
 *  @retval CRC32
 *       The CRC32 computed
 *
 *  @remarks
 *    -# Use the the local variable vNVM_IBTLi_tui32CrcTable initilized
 *       with NVM_IBTLi_InitTableCRC
 *
*/
static uint32_t NVM_IBTLi_ComputeCRC
(
   uint8_t *pxui8Data,
   uint32_t xi32Size,
   uint32_t xui32CurrCRC,
   bool     xisFirstBlock
)
{
   int32_t  i,idx;
    crc = 0;
    crc = crc ^ 0xffffffffL;
    while (xi32Size >= 8)
    {
      DO8(pxui8Data);
      xi32Size -= 8;
    }
    if (xi32Size) do {
      DO1(pxui8Data);
    } while (--xi32Size);
    crc = crc ^ 0xffffffffL;
    return (crc);

}


#if defined (__NVM_IBTL_AUTO_CREATE_APP_VERSION__)
/**
 * Automatic creation of Application verion File
*/
static bool NVM_IBTLi_DEBUG_WriteAppVersionDefault
(
   tNVM_IBTLi_File *pxstFile
)
{
   int32_t  offset = 0;
   uint32_t crc32;

   if (pxstFile == NULL) return(FALSE);

   /* Create the 14 ICS module */
   mMSBF_SET32_O(pxstFile->pui8Content,offset,kNVM_IBTLi_FAV_CURR_VERSION);
   mMSBF_SET32_O(pxstFile->pui8Content,offset,14);

   mMSBF_SET32_O(pxstFile->pui8Content,offset,kNVM_IBTL_ICS_MODID_FACTORY_DOWNLOADER);
   mMSBF_SET32_O(pxstFile->pui8Content,offset,kNVM_IBTL_CLASS_ID_DOWNLOADER);
   mMSBF_SET32_O(pxstFile->pui8Content,offset,0x51670001);
   mMSBF_SET32_O(pxstFile->pui8Content,offset,0xC0000000);

   mMSBF_SET32_O(pxstFile->pui8Content,offset,kNVM_IBTL_ICS_MODID_FACTORY_CONFIG_FILE);
   mMSBF_SET32_O(pxstFile->pui8Content,offset,kNVM_IBTL_CLASS_ID_CONFIG_FILE);
   mMSBF_SET32_O(pxstFile->pui8Content,offset,0x51670001);
   mMSBF_SET32_O(pxstFile->pui8Content,offset,0xC0000000);

   mMSBF_SET32_O(pxstFile->pui8Content,offset,kNVM_IBTL_ICS_MODID_DOWNLOADER);
   mMSBF_SET32_O(pxstFile->pui8Content,offset,kNVM_IBTL_CLASS_ID_DOWNLOADER);
   mMSBF_SET32_O(pxstFile->pui8Content,offset,0x51670003);
   mMSBF_SET32_O(pxstFile->pui8Content,offset,0x40000000);

   mMSBF_SET32_O(pxstFile->pui8Content,offset,kNVM_IBTL_ICS_MODID_EMS_CONFIG_FILE);
   mMSBF_SET32_O(pxstFile->pui8Content,offset,kNVM_IBTL_CLASS_ID_CONFIG_FILE);
   mMSBF_SET32_O(pxstFile->pui8Content,offset,0x51670007);
   mMSBF_SET32_O(pxstFile->pui8Content,offset,0x40000000);
   mMSBF_SET32_O(pxstFile->pui8Content,offset,kNVM_IBTL_ICS_MODID_EMS_REDUNDANT_CONFIG_FILE);
   mMSBF_SET32_O(pxstFile->pui8Content,offset,kNVM_IBTL_CLASS_ID_CONFIG_FILE);
   mMSBF_SET32_O(pxstFile->pui8Content,offset,0x51670002);
   mMSBF_SET32_O(pxstFile->pui8Content,offset,0x40000000);

   mMSBF_SET32_O(pxstFile->pui8Content,offset,kNVM_IBTL_ICS_MODID_EXE_APPLICATION_USER_0);
   mMSBF_SET32_O(pxstFile->pui8Content,offset,kNVM_IBTL_CLASS_ID_USER);
   mMSBF_SET32_O(pxstFile->pui8Content,offset,0x5167001C);
   mMSBF_SET32_O(pxstFile->pui8Content,offset,0x40000000);
   mMSBF_SET32_O(pxstFile->pui8Content,offset,kNVM_IBTL_ICS_MODID_EXE_APPLICATION_USER_1);
   mMSBF_SET32_O(pxstFile->pui8Content,offset,kNVM_IBTL_CLASS_ID_USER);
   mMSBF_SET32_O(pxstFile->pui8Content,offset,0x00000000);
   mMSBF_SET32_O(pxstFile->pui8Content,offset,0x00000000);
   mMSBF_SET32_O(pxstFile->pui8Content,offset,kNVM_IBTL_ICS_MODID_EXE_APPLICATION_USER_2);
   mMSBF_SET32_O(pxstFile->pui8Content,offset,kNVM_IBTL_CLASS_ID_USER);
   mMSBF_SET32_O(pxstFile->pui8Content,offset,0x00000000);
   mMSBF_SET32_O(pxstFile->pui8Content,offset,0x00000000);

   mMSBF_SET32_O(pxstFile->pui8Content,offset,kNVM_IBTL_ICS_MODID_EXE_APPLICATION_ENG_0);
   mMSBF_SET32_O(pxstFile->pui8Content,offset,kNVM_IBTL_CLASS_ID_ENGINEERING);
   mMSBF_SET32_O(pxstFile->pui8Content,offset,0x00000000);
   mMSBF_SET32_O(pxstFile->pui8Content,offset,0x00000000);
   mMSBF_SET32_O(pxstFile->pui8Content,offset,kNVM_IBTL_ICS_MODID_EXE_APPLICATION_ENG_1);
   mMSBF_SET32_O(pxstFile->pui8Content,offset,kNVM_IBTL_CLASS_ID_ENGINEERING);
   mMSBF_SET32_O(pxstFile->pui8Content,offset,0x00000000);
   mMSBF_SET32_O(pxstFile->pui8Content,offset,0x00000000);
   mMSBF_SET32_O(pxstFile->pui8Content,offset,kNVM_IBTL_ICS_MODID_EXE_APPLICATION_ENG_2);
   mMSBF_SET32_O(pxstFile->pui8Content,offset,kNVM_IBTL_CLASS_ID_ENGINEERING);
   mMSBF_SET32_O(pxstFile->pui8Content,offset,0x00000000);
   mMSBF_SET32_O(pxstFile->pui8Content,offset,0x00000000);

   mMSBF_SET32_O(pxstFile->pui8Content,offset,kNVM_IBTL_ICS_MODID_EXE_ASSET_USER_0);
   mMSBF_SET32_O(pxstFile->pui8Content,offset,kNVM_IBTL_CLASS_ID_ASSET);
   mMSBF_SET32_O(pxstFile->pui8Content,offset,0x5167000A);
   mMSBF_SET32_O(pxstFile->pui8Content,offset,0x40000000);
   mMSBF_SET32_O(pxstFile->pui8Content,offset,kNVM_IBTL_ICS_MODID_EXE_ASSET_USER_1);
   mMSBF_SET32_O(pxstFile->pui8Content,offset,kNVM_IBTL_CLASS_ID_ASSET);
   mMSBF_SET32_O(pxstFile->pui8Content,offset,0x00000000);
   mMSBF_SET32_O(pxstFile->pui8Content,offset,0x00000000);
   mMSBF_SET32_O(pxstFile->pui8Content,offset,kNVM_IBTL_ICS_MODID_EXE_ASSET_USER_2);
   mMSBF_SET32_O(pxstFile->pui8Content,offset,kNVM_IBTL_CLASS_ID_ASSET);
   mMSBF_SET32_O(pxstFile->pui8Content,offset,0x00000000);
   mMSBF_SET32_O(pxstFile->pui8Content,offset,0x00000000);

   /* CRC */
   crc32 = NVM_IBTLi_ComputeCRC(pxstFile->pui8Content,offset,0,TRUE);
   mMSBF_SET32_O(pxstFile->pui8Content,offset,crc32);

   /* Padd with 0 */
   memset(pxstFile->pui8Content+offset,0,pxstFile->i32Size-offset);

   /* Write */
   offset = NVM_IBTLi_FileWrite(pxstFile,NULL,-1);
   if (offset != pxstFile->i32Size)
   {
      /* Error */
      mTBOX_TRACE((kTBOX_NIV_WARNING,
                   "NVM_IBTLi_DEBUG_WriteAppVersionDefault ERROR: "
                   "Write Application Version failure (write %d/%d)\n",
                   offset,pxstFile->i32Size));
      return(FALSE);
   }

   return(TRUE);
}
#endif

#if defined (__NVM_IBTL_DUMP_FILE__)
/**
 * Dump file content
*/
static void NVM_IBTLi_DumpFile
(
   uint8_t             *pxui8Data,
   int32_t              xi32Size,
   tFLASH_CFG_Location *pxstFL,
   const char          *pxaFunctName,
   const char          *pxaFileLabel,
   const char          *pxaOperationLabel
)
{
   int32_t i;
   int32_t i32DumpSize = (xi32Size/16) * 16;

   mTBOX_PRINT((kTBOX_LF,"[%s] DUMP %s, size %d",
                (pxaFunctName==NULL)?"":pxaFunctName,
                (pxaFileLabel==NULL)?"File":pxaFileLabel, xi32Size));
   if (pxstFL != NULL)
   {
      mTBOX_PRINT((kTBOX_LF," {%#x,%#x}",
                   pxstFL->addressOffset,pxstFL->size));
   }

   mTBOX_PRINT((kTBOX_LF," - %s %d bytes:\n",
                (pxaOperationLabel==NULL)?"":pxaOperationLabel,
                i32DumpSize));

   if (pxui8Data != NULL)
   {
      for (i = 0; i < i32DumpSize; i++)
      {
         if ((i%16) == 0)
         {
            mTBOX_PRINT((0,"","\n%04X -",(unsigned short)i));
         }
         mTBOX_PRINT((0,""," %02X",pxui8Data[i]));
      }
      if (xi32Size > i32DumpSize)
      {
    	  mTBOX_PRINT((0,"","\n%04X -",(unsigned short)i32DumpSize));
          for (i = i32DumpSize; i < xi32Size; i++)
          {
             mTBOX_PRINT((0,""," %02X",pxui8Data[i]));
          }
      }
   }

   mTBOX_PRINT((0,"","\n"));
}
#endif

#ifdef NEWLAND_BOOTLOADER

static void NVM_IBTLi_ConvertEndian4Bytes(uint32_t *pValue)
{
    uint32_t Temp1=0, Temp2 = 0, Temp3 = 0, Temp4 = 0;

    Temp1 = (*pValue & (0xff000000))>>24;
    Temp2 = (*pValue & (0x00ff0000))>>8;
    Temp3 = (*pValue & (0x0000ff00))<<8;
    Temp4 = (*pValue & (0x000000ff))<<24;

    *pValue = Temp1 | Temp2 | Temp3 | Temp4;
}

static void NVM_IBTLi_ConvertEndian2Bytes(uint16_t *pValue)
{
    uint32_t Temp1=0, Temp2 = 0;

    Temp1 = (*pValue & (0xff00))>>8;
    Temp2 = (*pValue & (0x00ff))<<8;

    *pValue =  Temp1 | Temp2;
}



static uint32_t NVM_IBTLi_ConvertNewlandToIBTL(tNVM_LOADER_DB_INFO_S nl_struc, tNVM_IBTL_DialogArea* ibtl_struc)
{

    tCFGF_Configuration *pConfigFile;
    uint32_t usageID_CFGF;


    /* get default usage ID from the config file */
    pConfigFile = CFGF_Read(kCFGF_ID_PROD_ID);

    ibtl_struc->u32syntaxVersion = kNVM_IBTLi_FDA_CURR_VERSION;

    ibtl_struc->stDLCycle.u16DsmccPid = nl_struc.ota_pid;
    ibtl_struc->stDLCycle.u32UpdateID = nl_struc.u32CurSoftware;
    ibtl_struc->stDLCycle.u16DsmccPid = 0xFFFF;

    if(nl_struc.ota_type == 0x4C)
    {
        ibtl_struc->stDLCycle.u8DLState = 1;

    }
    else
    {
        ibtl_struc->stDLCycle.u8DLState = 0;

    }

    ibtl_struc->stDLCycle.u8ErrorCode = 0x0;
    ibtl_struc->stDLCycle.u8UsageId = pConfigFile->stProductIDs.ui8DefaultUsageId;
    ibtl_struc->stDLCycle.u8NetWorkType = kNVM_IBTL_NWT_DVB_C;

    ibtl_struc->stDvbcLocation.BIS_frequency = nl_struc.ota_frequency * 1000000;
    ibtl_struc->stDvbcLocation.symbol_rate= nl_struc.ota_symbol_rate * 1000;
    ibtl_struc->stDvbcLocation.modulation_type = nl_struc.ota_modulation + 3;
    

}

static uint32_t NVM_IBTLi_ConvertIBTLToNewland(tNVM_IBTL_DialogArea ibtl_struc, tNVM_LOADER_DB_INFO_S* nl_struc)
{
   nl_struc->magic_number = kNVM_IBTLi_NEWLAND_MAGIC_NUMBER;


    if(ibtl_struc.stDLCycle.u8DLState == 1)
    {
        nl_struc->ota_type = 0x4C;
    }
    else
    {
        nl_struc->ota_type = 0xFF;
    }

    nl_struc->ota_pid = ibtl_struc.stDLCycle.u16DsmccPid;
    nl_struc->ota_tableid = 0xFE;
    nl_struc->ota_frequency = ibtl_struc.stDvbcLocation.BIS_frequency / 1000000;
    nl_struc->ota_symbol_rate = ibtl_struc.stDvbcLocation.symbol_rate /1000 ;
    nl_struc->ota_modulation = ibtl_struc.stDvbcLocation.modulation_type- 3;

    nl_struc->u32CurSoftware = ibtl_struc.stDLCycle.u32UpdateID;
    nl_struc->u32NewSoftware= ibtl_struc.stDLCycle.u32UpdateID;
//    nl_struc->u32FactoryId   ?
    //nl_struc->download_date = 0;
    nl_struc->u32FailedCnt = 0;
//    nl_struc->u32HardwareVersion =

    //nl_struc->u32DestroyFlag = 0;



}


#elif defined(CHANGHONG_BOOTLOADER)

static void NVM_IBTLi_ConvertCHToIBTL(CH_UpdateInfo_t ch_struc, tNVM_IBTL_DialogArea* ibtl_struc)
{
    tCFGF_Configuration *pConfigFile;

    /* get default usage ID from the config file */
    pConfigFile = CFGF_Read(kCFGF_ID_PROD_ID);

    ibtl_struc->u32syntaxVersion = kNVM_IBTLi_FDA_CURR_VERSION;

    ibtl_struc->stDLCycle.u16DsmccPid = ch_struc.us_DataPid;
    //ibtl_struc->stDLCycle.u32UpdateID =
    //ibtl_struc->stDLCycle.u16DsmccPid = 0xFFFF;
    ibtl_struc->stDLCycle.u8DLState = 0x0;
    ibtl_struc->stDLCycle.u8ErrorCode = 0x0;
    ibtl_struc->stDLCycle.u8UsageId = pConfigFile->stProductIDs.ui8DefaultUsageId;
    ibtl_struc->stDLCycle.u8NetWorkType = kNVM_IBTL_NWT_DVB_C;

    ibtl_struc->stDvbcLocation.BIS_frequency = ch_struc.un_FreqInfo.stru_CableFreqInfo.ui_FreqKHZ;
    ibtl_struc->stDvbcLocation.symbol_rate= ch_struc.un_FreqInfo.stru_CableFreqInfo.ui_SymbKbps;
    ibtl_struc->stDvbcLocation.modulation_type = ch_struc.un_FreqInfo.stru_CableFreqInfo.uc_QAMMode;
    
    CFGF_Free(pConfigFile);
}

static void NVM_IBTLi_ConvertIBTLToCH(tNVM_IBTL_DialogArea ibtl_struc, CH_UpdateInfo_t* nl_struc)
{
	memset(nl_struc,0,sizeof(CH_UpdateInfo_t));
    memcpy(nl_struc->uc_name,"Update Info",strlen("Update Info") );
    nl_struc->enm_AntType			= CH_DEMOD_DVB_C;
    nl_struc->uc_UpdaeFlag			= 0x47;
    nl_struc->un_FreqInfo.stru_CableFreqInfo.ui_FreqKHZ	= ibtl_struc.stDvbcLocation.BIS_frequency;
    nl_struc->un_FreqInfo.stru_CableFreqInfo.ui_SymbKbps = ibtl_struc.stDvbcLocation.symbol_rate;
    nl_struc->un_FreqInfo.stru_CableFreqInfo.uc_QAMMode	= ibtl_struc.stDvbcLocation.modulation_type - 2;
    nl_struc->us_DataPid			= ibtl_struc.stDLCycle.u16DsmccPid;
    nl_struc->us_DataGid			= 0x2;
    nl_struc->i_length = sizeof(CH_UpdateInfo_t) - 16;
    nl_struc->ui_crc = CH_COM_MPEGCRC32((uint8_t *)&(nl_struc->enm_AntType), nl_struc->i_length -4);
}

static void NVM_IBTLi_TraceCHDlgArea(CH_UpdateInfo_t * pstCHDialogArea)
{

}
#elif defined(SKYWORTH_BOOTLOADER)
static void NVM_IBTLi_ConvertSKYToIBTL(tNVM_SKYLoaderInfo sky_struc, tNVM_IBTL_DialogArea* ibtl_struc)
{
    tCFGF_Configuration *pConfigFile;

    /* get default usage ID from the config file */
    pConfigFile = CFGF_Read(kCFGF_ID_PROD_ID);

    ibtl_struc->u32syntaxVersion = kNVM_IBTLi_FDA_CURR_VERSION;

    ibtl_struc->stDLCycle.u16DsmccPid   = sky_struc.update_info.pid;
    ibtl_struc->stDLCycle.u8DLState     = 0x0;
    ibtl_struc->stDLCycle.u8ErrorCode   = 0x0;
    ibtl_struc->stDLCycle.u8UsageId     = pConfigFile->stProductIDs.ui8DefaultUsageId;
    ibtl_struc->stDLCycle.u8NetWorkType = kNVM_IBTL_NWT_DVB_C;

    ibtl_struc->stDvbcLocation.BIS_frequency    = sky_struc.update_info.tuner.uFrequencyKHz*1000;
    ibtl_struc->stDvbcLocation.symbol_rate      = sky_struc.update_info.tuner.uSymbolRateKHz*1000;
    ibtl_struc->stDvbcLocation.modulation_type  = sky_struc.update_info.tuner.modulatorMode+3;

    CFGF_Free(pConfigFile);
}

static void NVM_IBTLi_ConvertIBTLToSKY(tNVM_IBTL_DialogArea ibtl_struc, tNVM_SKYLoaderInfo* sky_struc)
{
    memset(sky_struc,0,sizeof(tNVM_SKYLoaderInfo));
    *((volatile unsigned int *) (kNVM_IBTLi_SKYWORTH_INIT_FLAG_ADDR - 4*1)) = (unsigned int)kNVM_IBTLi_SKYWORTH_INIT_FLAG_MAGIC_NUMBER;
    sky_struc->update_info.tuner.uFrequencyKHz  = ibtl_struc.stDvbcLocation.BIS_frequency/1000;
    sky_struc->update_info.tuner.uSymbolRateKHz = ibtl_struc.stDvbcLocation.symbol_rate/1000;
    sky_struc->update_info.tuner.modulatorMode  = ibtl_struc.stDvbcLocation.modulation_type - 3;
	sky_struc->update_info.pid                  = ibtl_struc.stDLCycle.u16DsmccPid;
	if(ibtl_struc.stDLCycle.u16DsmccPid == 0) /*Nominal OTA*/
	{
	    sky_struc->update_info.pid                  = 7000;
	}
	else if(ibtl_struc.stDLCycle.u16DsmccPid == 0x1)
	{
	    sky_struc->update_info.pid                  = 7002;
	}
		
    
}

static void NVM_IBTLi_TraceSKYDlgArea(tNVM_SKYLoaderInfo * pstSkyworthDialogArea)
{

}
#endif





/* nvm_interface_bootloader.c */
