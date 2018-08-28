/**************************************************************************//**
** @file nvm_interface_bootloader.h
**
** @brief
**   This file defines the API to manage (read/write) the files interface 
**   bewtween the bootloader and applications (comedia/trigger and downloader).
**
** @ingroup NVM MANAGEMENT
**
** @see FRS Bootloader version 1.1, release date 2011-06-29
**  
** @version $Rev: 9944 $
**          $URL: http://10.50.1.219/svncomedia/idecode/product/comedia/branches/snt_cos/main/build/inc/nvm_interface_bootloader.h $
**          $Date: 2012-04-13 09:56:36 +0200 (ven., 13 avr. 2012) $
**
** @author  SmarDTV Rennes - LIPPA
**
** COPYRIGHT:
**   2011 SmarDTV
**
******************************************************************************/

#ifndef _NVM_INTERFACE_BOOTLOADER_H
#define _NVM_INTERFACE_BOOTLOADER_H

/******************************************************************************
* Includes
******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
* Defines
******************************************************************************/

/**
 *  @brief
 *    Class identifier of modules.
 *    Value of fied 'ui32ClassId' of type tNVM_IBTL_UpdateRequest
 *    managed by the functions NVM_IBTL_GetUpdateRequest, 
 *    NVM_IBTL_SetUpdateRequest and NVM_IBTL_UnsetUpdateRequest.
 *
*/
#define kNVM_IBTL_CLASS_ID_NONE                        0x00 
   /**< Undefined class identifier.
   */

#define kNVM_IBTL_CLASS_ID_SSA_DA2                     0x01 
   /**< Contains the SSA DA2 modules (factory, redundant, normal).
   */

#define kNVM_IBTL_CLASS_ID_CONFIG_FILE                 0x02 
   /**< Contains the ConfigFile Application modules (Factory, Redundant,
        Normal).
   */

#define kNVM_IBTL_CLASS_ID_ENGINEERING                 0x03 
   /**< Contains executable Application Modules which are dedicated to 
        engineering (ProdCore, diagnostics, post-mortem...).
   */

#define kNVM_IBTL_CLASS_ID_DOWNLOADER                  0x04 
   /**< Contains executable Application Modules which purpose is to update
        some other Application Modules.
   */

#define kNVM_IBTL_CLASS_ID_USER                        0x05 
   /**< Contains executable Application modules which are dedicated for usage 
        is user household.
   */

#define kNVM_IBTL_CLASS_ID_ASSET                       0x06 
   /**< Contains assets (graphical, translation, byte code...) useful for 
        the executable of the User Class.
   */

#define kNVM_IBTL_CLASS_ID_NUMBER                        6
   /**< Number of class identifier.
   */

#define kNVM_IBTL_CLASS_ID_ALL                  0xFFFFFFFF 
   /**< Reference all class ID, for exemple to set the same reason of all update 
        requests in NVM Update Request File.
   */

/**
 *  @brief
 *    Update reason values for the update request (in update request file).
 *    Value of fied 'ui8UpdateReason' of type tNVM_IBTL_UpdateRequest
 *    managed by the functions NVM_IBTL_GetUpdateRequest, 
 *    NVM_IBTL_SetUpdateRequest and NVM_IBTL_UnsetUpdateRequest.
 *
*/
#define kNVM_IBTL_UPDATE_REASON_NONE                   0x00 
   /**< Undefined updatae request => No request.
   */

#define kNVM_IBTL_UPDATE_REASON_NEW_VERSION            0x01 
   /**< A new version of the application module(s) of the class has been 
        detected by the last executing application (e.g. trigger detection).
   */

#define kNVM_IBTL_UPDATE_REASON_CORRUPTION             0x02 
   /**< At least one of the Application Modules of the class is corrupted.
   */

#define kNVM_IBTL_UPDATE_REASON_FORCED                 0x03 
   /**< The update of the Application Modules of the class has been forced 
        by a combination of keys.
   */

#define kNVM_IBTL_UPDATE_REASON_EXTERNAL               0x04
   /**< The update of the Application Modules of the class has been decided 
        by an external program communicating with the receiver through one of 
        its communication interfaces (serial, USB, Ethernet... )
   */

/**
 *  @brief
 *    ICS Module Identifiers values: The SSA library uses some unique identifiers 
 *    for Application Modules. These identifiers are used to build the SBP 
 *    proprietary section.
 *    Values used to identify the module that indicates its execution in the file
 *    Executable Confirmation with the function NVM_IBTL_SetExecutionFlag.
 *
*/
#define kNVM_IBTL_ICS_MODID_NONE                          0xFFFFFFFF 
#define kNVM_IBTL_ICS_MODID_FACTORY_DOWNLOADER         0x01 
#define kNVM_IBTL_ICS_MODID_FACTORY_CONFIG_FILE        0x02 
#define kNVM_IBTL_ICS_MODID_DOWNLOADER                 0x03 
#define kNVM_IBTL_ICS_MODID_EMS_CONFIG_FILE            0x04 
#define kNVM_IBTL_ICS_MODID_EMS_REDUNDANT_CONFIG_FILE  0x05 
#define kNVM_IBTL_ICS_MODID_EXE_APPLICATION_USER_0     0x06 
#define kNVM_IBTL_ICS_MODID_EXE_APPLICATION_USER_1     0x07 
#define kNVM_IBTL_ICS_MODID_EXE_APPLICATION_USER_2     0x08 
#define kNVM_IBTL_ICS_MODID_EXE_APPLICATION_ENG_0      0x09 
#define kNVM_IBTL_ICS_MODID_EXE_APPLICATION_ENG_1      0x0A 
#define kNVM_IBTL_ICS_MODID_EXE_APPLICATION_ENG_2      0x0B 
#define kNVM_IBTL_ICS_MODID_EXE_ASSET_USER_0           0x0C 
#define kNVM_IBTL_ICS_MODID_EXE_ASSET_USER_1           0x0D
#define kNVM_IBTL_ICS_MODID_EXE_ASSET_USER_2           0x0E
#define kNVM_IBTL_ICS_MODID_SSA_DA2                    0x90 /* Used only to download the DA2 */

#define kNVM_IBTL_ICS_MODID_N_APPLICATION				4

/**
 *  @brief
 *    Factory Update ID.
 *
*/
#define kNVM_IBTL_UPDATE_ID_FACTORY              0xFFFFFFFF

#define kstNVM_IBTL_APP_VERS_NONE {kNVM_IBTL_ICS_MODID_NONE, kNVM_IBTL_CLASS_ID_NONE, kNVM_IBTL_UPDATE_ID_FACTORY, FALSE, FALSE, 0, "", "" }
#define kNVM_IBTLi_FAV_SV_LABEL_LENGTH	12
#define kNVM_IBTLi_FAV_SD_LABEL_LENGTH	20

/**
 *  @brief
 *    RF modulation/CVBS active flags (boolean) defaults values
 *
*/
#define kNVM_IBTL_RFMOD_ACTIVE_DEFAULT       TRUE
#define kNVM_IBTL_CVBS_ACTIVE_DEFAULT        TRUE
#define kNVM_IBTL_UMCS6_ACTIVE_DEFAULT       TRUE

/**
 *  @brief
 *    RF modulation: Television channel that the RF output stage shall 
 *    transmit video on when displaying the splash screen
 *
*/
#define kNVM_IBTL_RFMOD_CHANNEL_IMCL                     65
#define kNVM_IBTL_RFMOD_CHANNEL_DEFAULT      kNVM_IBTL_RFMOD_CHANNEL_IMCL

/**
 *  @brief
 *    RF modulation: Standard or type used by the RF modulator hardware.
 *    Values used in the Update Request file to define the RF modutation.
 *
*/
#define kNVM_IBTL_RFMOD_STANDARD_BG                    0x00 
#define kNVM_IBTL_RFMOD_STANDARD_DK                    0x01 
#define kNVM_IBTL_RFMOD_STANDARD_H                     0x02 
#define kNVM_IBTL_RFMOD_STANDARD_I                     0x03 
#define kNVM_IBTL_RFMOD_STANDARD_K                     0x04 
#define kNVM_IBTL_RFMOD_STANDARD_L                     0x05 
#define kNVM_IBTL_RFMOD_STANDARD_M                     0x06 
#define kNVM_IBTL_RFMOD_STANDARD_N                     0x07 
#define kNVM_IBTL_RFMOD_STANDARD_B                     0x08 
#define kNVM_IBTL_RFMOD_STANDARD_DEFAULT     kNVM_IBTL_RFMOD_STANDARD_B 

/**
 *  @brief
 *    RF modulation: Frequency band.
 *    Values used in the Update Request file to define the RF modulation.
 *
*/
#define kNVM_IBTL_RFMOD_BAND_VHF1                      0x00 
#define kNVM_IBTL_RFMOD_BAND_VHF2                      0x01 
#define kNVM_IBTL_RFMOD_BAND_UHF                       0x02 
#define kNVM_IBTL_RFMOD_BAND_DEFAULT         kNVM_IBTL_RFMOD_BAND_UHF 
 
/**
 *  @brief
 *    RF modulation: The CVBS encoding format.
 *    Values used in the Update Request file to define the RF modulation.
 *
*/
#define kNVM_IBTL_RFMOD_CVBS_ENCOD_NTSC                0x00 
#define kNVM_IBTL_RFMOD_CVBS_ENCOD_NTSC_J              0x01 
#define kNVM_IBTL_RFMOD_CVBS_ENCOD_NTSC_443            0x02 
#define kNVM_IBTL_RFMOD_CVBS_ENCOD_PAL_BGHDI           0x03 
#define kNVM_IBTL_RFMOD_CVBS_ENCOD_PAL_M               0x04 
#define kNVM_IBTL_RFMOD_CVBS_ENCOD_PAL_N               0x05 
#define kNVM_IBTL_RFMOD_CVBS_ENCOD_PAL_N_C             0x06 
#define kNVM_IBTL_RFMOD_CVBS_ENCOD_SECAM               0x07 
#define kNVM_IBTL_RFMOD_CVBS_DEFAULT         kNVM_IBTL_RFMOD_CVBS_ENCOD_PAL_BGHDI 

/**
 *  @brief
 *    IBTL_NETWORK_TYPE
 *
*/
#define kNVM_IBTL_NWT_SERIAL  0
#define kNVM_IBTL_NWT_USB     1
#define kNVM_IBTL_NWT_URL     2
#ifdef MEDIUM_TER
#define kNVM_IBTL_NWT_DVB_T   0x10
#define kNVM_IBTL_NWT_DVB_T2  0x11
#endif
#ifdef MEDIUM_CAB
#define kNVM_IBTL_NWT_DVB_C   0x20
#endif
#ifdef MEDIUM_SAT
#define kNVM_IBTL_NWT_DVB_S   0x30
#define kNVM_IBTL_NWT_DVB_S2  0x31
#endif

/******************************************************************************
* Macros
******************************************************************************/


/******************************************************************************
* Typedefs
******************************************************************************/

typedef enum
{
   eNVM_IBTL = 0x04280201
} tNVM_IBTL_TracesID;

/**
 *  @brief
 *    Execution statuses returned by the nvm interface with 
 *    bootloader functions.
*/
typedef enum
{
    eNVM_IBTL_NO_ERROR = 0,      /**< Success                      */
    eNVM_IBTL_ERROR_READ,        /**< NVM read file error          */
    eNVM_IBTL_ERROR_WRITE,       /**< NVM write file error         */
    eNVM_IBTL_ERROR_MEMORY,      /**< Memory allocation error      */
    eNVM_IBTL_ERROR_BAD_CRC,     /**< File read have a bad CRC     */
    eNVM_IBTL_ERROR_BAD_VERSION, /**< File read have a bad version */
    eNVM_IBTL_ERROR_NOT_FOUND,   /**< Requested element not found  */
    eNVM_IBTL_ERROR_BAD_LOCATION,/**< error parsing stream location*/
    eNVM_IBTL_ERROR_BAD_PID,     /**< error parsing stream location*/
    eNVM_IBTL_ERROR              /**< Other error                  */
} tNVM_IBTL_Status;

/**
 *  @brief
 *    Update request element of update list stored in update request file.
*/
typedef struct
{
   uint32_t ui32ClassId; 
   /**< The identifier of a class for which Application modules shall 
        be updated (see class identifier pre-defined values 
        kNVM_IBTL_CLASS_ID_xxx fixed in part 'defines').
   */
   uint32_t ui32UpdateReason; 
   /**< The reason for the update request (see update reason pre-defined 
        values  kNVM_IBTL_UPDATE_REASON_xxx fixed in part 'defines').
   */
} tNVM_IBTL_UpdateRequest;

/**
 *  @brief
 *    DVB_S structure from trigger dialog area
*/
typedef struct
{
	uint32_t BIS_frequency;
	uint32_t	symbol_rate;
	bool continuous_tone_on_flag;
	bool lnb_power_on_flag;
	bool vertical_polarisation_flag;
	uint8_t modulation_type;
	uint8_t fecrate;
	uint8_t switch_selection;
}tNVM_IBTL_DvbsLocation;
/**
 *  @brief
 *    DVB_C structure from trigger dialog area
*/
typedef struct
{
	uint32_t BIS_frequency;		/* KHZ */
	uint32_t	symbol_rate;	/* Kbps */
	uint8_t modulation_type;	/* see tOAD_Modulation enum */
}tNVM_IBTL_DvbcLocation;
/**
 *  @brief
 *    DVB_T structure from trigger dialog area
*/
typedef struct
{
	uint32_t frequency;
	uint32_t bandwidth;
	uint8_t  modulation;
	uint8_t  mode;
	uint8_t  guard;
	uint8_t  hierarchy;
	uint8_t  priority;
	uint8_t  code_rate_HP;
	uint8_t  code_rate_LP;
	uint8_t  freq_offset;
}tNVM_IBTL_DvbtLocation;

/**
 *  @brief
 *    DVB_T2 structure from trigger dialog area
*/
typedef struct
{
	uint32_t frequency;
	uint32_t bandwidth;
	uint8_t  mode;
    uint8_t  guard;
    uint8_t  PLP_ID;
}tNVM_IBTL_Dvbt2Location;

/**
 *  @brief
 *    Download cycle from trigger dialog area
u32UpdateID		32
u16PmtPid			16	
u16DsmccPid		16
U8DLState			8
U8ErrorCode		8
U24reserved		8
U8NetWorkType		8
*/
typedef struct
{
	uint32_t u32UpdateID;
	uint16_t u16PmtPid;	/* RFU */
	uint16_t u16DsmccPid;
	uint8_t  u8DLState;
	uint8_t	u8ErrorCode;
	uint8_t  u8UsageId; 
	uint8_t 	u8NetWorkType;
}tNVM_IBTL_DownloadCycle;
	
/**
 *  @brief
 *    dialog area between trigger and downloader.
*/
typedef struct
{
	uint32_t u32syntaxVersion;
	bool useStdbyState;
	bool isSTBinStdby;
   tNVM_IBTL_DownloadCycle stDLCycle;
#if defined(MEDIUM_TER)   
   tNVM_IBTL_DvbtLocation  stDvbtLocation; 
   tNVM_IBTL_Dvbt2Location stDvbt2Location; 
#elif defined(MEDIUM_SAT)   
   tNVM_IBTL_DvbsLocation stDvbsLocation; 
#elif defined(MEDIUM_CAB)   
   tNVM_IBTL_DvbcLocation stDvbcLocation; 
#endif
} tNVM_IBTL_DialogArea;
/**
 *  @brief
 *    RF Modulation definition stored in update request file.
*/
typedef struct
{
   bool isActiveRFMod; 
   /**< A flag set to TRUE (resp. FALSE) to indicate that the bootloader should 
        enable (resp. disable) the RF output during its execution.
   */
   bool isActiveCVBS; 
   /**< A flag set to TRUE (resp. FALSE) to indicate that the bootloader should 
        enable (resp. disable) the CVBS output during its execution
   */
   uint8_t ui8Channel; 
   /**< Specifies the television channel that the RF output stage shall transmit 
        video on when displaying the splash screen
        Note: Significant only if RF modulation is active (isActiveRFMod=TRUE)
.   */
   uint8_t ui8Standard; 
   /**< Specifies the modulation standard or type used by the RF modulator hardware,
        (see RF modulation Standard pre-defined values  NVM_IBTL_RFMOD_STANDARD_xxx 
        fixed in part 'defines').
        Note: Significant only if RF modulation is active (isActiveRFMod=TRUE)
.   */
   uint8_t ui8Band; 
   /**< Specifies the frequency band (see RF modulation Band pre-defined values 
        NVM_IBTL_RFMOD_BAND_xxx fixed in part 'defines').
        Note: Significant only if RF modulation is active (isActiveRFMod=TRUE)
.   */
   uint8_t ui8EncodingCVBS; 
   /**< Specifies the CVBS encoding format (see CVBS encoding format pre-defined values 
        kNVM_IBTL_RFMOD_CVBS_ENCOD_xxx fixed in part 'defines').
        Note: Significant only if CVBS is active (isActiveCVBS=TRUE)
.   */
} tNVM_IBTL_RFModulation;

/**
 *  @brief
 *    ICS module version element of list stored in application version file.
 *
 *    Default values:
 *    rf_mod_active_flag =1
 *    cvbs_active_flag =1
 *    rf_mod_channel = "the default remod channel"
 *    rf_mod_standard = 0x08 (PAL B)
 *    rf_mod_band = 0x02 (UHF)
 *    cvbs_encoding_standard = 0x03 (PAL BGHDI)
*/
typedef struct
{
   uint32_t ui32ICSModuleId; 
   /**< The identifier of the ICS module (see ICS Module Identifiers values 
        pre-defined values  kNVM_IBTL_ICS_MODID_xxx fixed in part 'defines').
   */
   uint32_t ui32ClassId; 
   /**< The identifier of a class for which the module belongs (see class 
        identifier pre-defined values kNVM_IBTL_CLASS_ID_xxx fixed in part 
        'defines').
   */
   uint32_t ui32UpdateId; 
   /**< The version number of the executable application module.
   */
   bool     isInPLS; 
   /**< Flag set to TRUE to indicate that the Application Module is stored in 
        PLS. Set to FALSE otherwise.
   */
   bool     isValid; 
   /**< A Flag set to FALSE to indicate that the Application Module is not 
        valid and shall not be used. A Flag set to TRUE to indicate that the 
        Application Module is valid and can be used. This flah is typically 
        useful for an executing application module to determine which asset 
        file can be used or not.
   */
   uint32_t ui32RFU; 
   /**< Reserved field for future usage.
   */
	/* ADDED in version 2 */
   uint8_t SoftwareVersionLabel[kNVM_IBTLi_FAV_SV_LABEL_LENGTH+1];
   uint8_t SoftwareDateLabel[kNVM_IBTLi_FAV_SD_LABEL_LENGTH+1];
   
} tNVM_IBTL_ApplicationVersion;

 

/******************************************************************************
* Functions
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
);

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
);

/**
 *  @brief
 *    Set execution flag in NVM Executable Confirmation file.
 *    A flag set to 1 indicate that the application has executed correctly
 *    (0 => not executed, normaly value set by the bootloader application).
 *
 *  @pre
 *    None.
 *
 *  @post
 *    None.
 *
 *  @param[in] xui32ICSModuleId
 *       ICS module identifier to identify the Application Modules that set
 *       the execution flag. 
 *       (see defines kNVM_IBTL_ICS_MODID_xxx).
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
*/
tNVM_IBTL_Status NVM_IBTL_SetExecutionFlag
(
   uint32_t xui32ICSModuleId,
   bool     xIsExecuted
);

/**
 *  @brief
 *    Return from the NVM Update Request File the update (download) request 
 *    (application class id + update reason) with the highest reason priority 
 *    in order new version of  application detected, corruption and update forced 
 *    and external.
 *
 *  @pre
 *    None.
 *
 *  @post
 *    None.
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
);

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
 *    -# If a read error or a invalid value read (version or CRC), 
 *       set the default values in this file and return an error
 *       except in mode 'set' when the write is a success.
*/
tNVM_IBTL_Status NVM_IBTL_SetUpdateRequest
(
   tNVM_IBTL_UpdateRequest *pxstUpdRequest
);

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
);

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
);

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
);

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
bool NVM_IBTL_ReadDownloadStatus(uint32_t *pUpdateID, uint32_t *peStatus, uint32_t *peError);
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
bool NVM_IBTL_ResetDownloadStatus(void);
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
bool NVM_IBTL_UpdateDownloadStatus(uint32_t u32xUpdateID, uint8_t u8xErrorCode, uint8_t u8xDLState, uint8_t u8xDefaultUsageId, uint8_t u8xDefaultNetworkType );
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
bool NVM_IBTL_ReadUsageId(uint32_t *pUsageID);
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
bool NVM_IBTL_UpdateUsageId(uint8_t u8xUsageID);

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

tNVM_IBTL_Status   NVM_IBTL_GetDialogFile(tNVM_IBTL_DialogArea *pxstDialogArea);
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
bool NVM_IBTL_LaunchDownloader(tNVM_IBTL_DialogArea *pstDownloadDialogFile);


bool NVM_IBTL_ResetApplicationVersion();



#if defined (__DEBUG_TRACE__)
/**
 *  Debug functions for integration
*/
char *NVM_IBTL_DEBUG_LabelClassID       (uint32_t xui32Value);
char *NVM_IBTL_DEBUG_LabelReason        (uint32_t xui32Value);
char *NVM_IBTL_DEBUG_LabelModuleID      (uint32_t xui32Value);
char *NVM_IBTL_DEBUG_LabelRFModStandard (uint8_t xui8Value);
char *NVM_IBTL_DEBUG_LabelRFModBand     (uint8_t xui8Value);
char *NVM_IBTL_DEBUG_LabelCVBSEncoding  (uint8_t xui8Value);
#endif

#ifdef __cplusplus
}
#endif

#endif /* _NVM_INTERFACE_BOOTLOADER_H */

/* nvm_interface_bootloader.h */

