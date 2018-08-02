 /******************************************************************************
	* 		 Kaon Media (C)	*
 ******************************************************************************
 *
 * MODULE NAME: TDAL_SSU_SSU
 *
 * PUBLIC
 *
 * DESCRIPTION
 * KAON definition 2016.12.29 bsjung
 *****************************************************************************/

#ifndef _TDAL_SSU_P_H_
#define _TDAL_SSU_P_H_

/*******************************************************/
/*       Includes                                      */
/*******************************************************/

/*******************************************************/
/*       Defines                                       */
/*******************************************************/

/*******************************************************/
/*       Macros                                        */
/*******************************************************/
#define	FLASH_BCA_BASE_ADDRESS			(0x00110000)
#define	RAM_BCA_DEC_BASE_ADDRESS		(0x8FF90000)
#define	FLASH_BCA_SIZE				(512)

#define	FLASH_FDCA_BASE_ADDRESS			(0x00140000)
#define	FLASH_FDCA_SIZE				(1024)

#define	FLASH_INFO_BASE_ADDRESS			(0x00160000)
#define	FLASH_INFO_SIZE				(32)

#define	KAON_DOWNLOAD_MODEL_ID			(1210)
#define	OTC_FREQ_NUM				(15) //DEN REQUIRED:15
#define	OTC_FREQ_MAX				(25) //max
#define	SW_VER_NUM_MAX_LEN			(16)

/*******************************************************/
/*       Typedefs                                      */
/*******************************************************/
typedef enum UpdateMedia_e {
	UPDATE_MEDIA_NONE		= 0x00,
	UPDATE_MEDIA_OTA 		= 0x01,
	UPDATE_MEDIA_USB 		= 0x02,
	UPDATE_MEDIA_IP  		= 0x03,
	UPDATE_MEDIA_BACKUP		= 0x04,
	UPDATE_MEDIA_DONTCARE		= 0xFE,
	UPDATE_MEDIA_ALL		= 0xFF,
} UpdateMedia_e;

typedef enum UpdateMethod_e {
	UPDATE_METHOD_NONE		= 0x00,
	UPDATE_METHOD_NORMAL		= 0x01,
	UPDATE_METHOD_TECH		= 0x02,
	UPDATE_METHOD_RESCUE		= 0x04,
	UPDATE_METHOD_MANUAL		= 0x08,
	UPDATE_METHOD_DONTCARE		= 0xFE,
} UpdateMethod_e;

typedef enum UpdateRequest_e {
	UPDATE_REQUEST_NONE		= 0x00,
	UPDATE_REQUEST_REQUESTED	= 0x01,
	UPDATE_REQUEST_DONTCARE		= 0xFE,
} UpdateRequest_e;

typedef enum StandbyMode_e {
	STANDBY_MODE_NORMAL		= 0xC0,
	STANDBY_MODE_ACTIVE_STANDBY	= 0xC2,
	STANDBY_MODE_PASSIVE_STANDBY	= 0xC4
} StandbyMode_e;

typedef enum
{
    SOFTWARE_VERSION_ANY        = 0, // highest
    SOFTWARE_VERSION_HIGHER     = 0xFFFFFFFF, // higher than current
} SoftwareVersion_e;

#define 		RSA_KEY_SIZE				(256)
#define 		AES_KEY_SIZE				(16)
#define 		MODEL_NAME_LEN				(32)
#define 		DOWNLOAD_MODEL_ID			(1210)
typedef struct {
	uint32_t       		image_size;
    char        		model_name[MODEL_NAME_LEN];
    uint16_t      		model_id;
    uint16_t      		hardware_id;
    uint16_t      		buyer_id;
    uint16_t      		region_id;
    uint8_t       		reserved1[4];
    uint8_t       		rsa_key[RSA_KEY_SIZE];
    uint8_t       		aes_key1[AES_KEY_SIZE];
    uint8_t       		aes_key2[AES_KEY_SIZE];
    uint8_t       		aes_key3[AES_KEY_SIZE];
    uint8_t       		encrypt_mode;
    uint8_t       		reserved2[137];
    uint16_t      		version;
    uint32_t      		oui;
    char           		build_date[12];
    uint32_t      		crc32;
} BCAInfo_t;

/* Information */
#define	SIZE_INFO			(32)
#define UTC_TIME_LENGTH			(5)
typedef struct {
	uint8_t       		boot_count;
	uint8_t       		last_updated_time[UTC_TIME_LENGTH]; // UTC MJD
	uint8_t       		standby_mode;
	uint8_t       		ts_entry_curr_num;

	uint32_t		application_version;
	uint32_t		downloader_version;
	uint8_t			reserved[6];

	uint16_t		download_model_id;
	uint32_t        	download_error_code;

	uint32_t		crc32;
} INFOInfo_t;

/* Tuning parameter structure for the FDCA */
/*
 * 16QAM = 0x02
 * 64QAM = 0x03
 * 256QAM = 0x04
 *
 */
#define SIZE_TUNINGPARAMETER		(32)
typedef struct {
	uint8_t       		lnb_id;
	uint16_t      		data_pid;
	uint8_t       		system;
	uint32_t        	freq_khz;
	uint32_t        	symbol_ksym;
	uint8_t       		modulation;
	uint8_t       		fec;
	uint8_t				bandwidth;
    uint8_t       		iq_mode;
    uint8_t       		reserved[16];
} TuningParameter_t;

#define SIZE_FDCA_EX        		(32)
typedef struct {
	uint8_t       		ex_enable;
	uint16_t      		video_hd;
	uint16_t      		video_sd;
	uint8_t       		reserved[27];
} FDCAExInfo_t;

#define SIZE_FDCA           		(1024)//(256)
typedef struct {
	uint8_t       		update_request;
	uint8_t       		update_method;
	uint8_t       		update_media;

	uint16_t      		target_software_type;
	uint32_t        	target_software_version;

	uint8_t       		reserved1[8];
	FDCAExInfo_t        	fdca_ex;

	uint32_t		sw_version_number;
	uint32_t		dl_version_number;

	uint8_t			reserved3[10];
	uint8_t       		tuning_param_cnt;
	TuningParameter_t   	tuning_param[OTC_FREQ_MAX];

	uint8_t       		reserved2[42];
	uint16_t      		version;
	uint32_t        	crc32;
} FDCAInfo_t;

typedef enum DownloadErrorCode_e {
	DN_ERR_E000			= 0x0000E000, /* SUCCESS */

	DN_ERR_E100			= 0x0000E100, /* Tuning - No Signal */
	DN_ERR_E101			= 0x0000E101, /* Tuning - Low Signal Strength */
	DN_ERR_E102			= 0x0000E102, /* Tuning - Low Signal Quality */
	DN_ERR_E103			= 0x0000E103, /* Tuning - Tuner parameter settings failure */
	DN_ERR_E104			= 0x0000E104, /* Tuning - Demodulator parameter settings failure */
	DN_ERR_E105			= 0x0000E105, /* Tuning - Demux Settings Failure */

	DN_ERR_E201			= 0x0000E201, /* Downloading - Internal Resource allocation Error */
	DN_ERR_E202			= 0x0000E202, /* Downloading - Header Error */
	DN_ERR_E203			= 0x0000E203, /* Downloading - Sequence Error */
	DN_ERR_E204			= 0x0000E204, /* Downloading - RAM Write Error */

	DN_ERR_E301			= 0x0000E301, /* Extracting - Payload Error */
	DN_ERR_E302			= 0x0000E302, /* Extracting - RAM Write Error */
	DN_ERR_E303			= 0x0000E303, /* Extracting - AES Encryption check failure */

	DN_ERR_E401			= 0x0000E401, /* Programming - Flash unlock Failure */
	DN_ERR_E402			= 0x0000E402, /* Programming - Flash Access Failure */
	DN_ERR_E403			= 0x0000E403, /* Programming - Size Partition Mismatch */
	DN_ERR_E404			= 0x0000E404, /* Programming - Flash Write Failure */
	DN_ERR_E405			= 0x0000E405, /* Programming - Image size is more than FLASH APP area */

	DN_ERR_E500			= 0x0000E500, /* PID unavailability */
} DownloadErrorCode_e;

/*******************************************************/
/*       Variables Declarations (IMPORT)               */
/*******************************************************/

/*******************************************************/
/*       Functions Declarations                        */
/*******************************************************/

#endif /*_TDAL_SSU_P_H_*/



