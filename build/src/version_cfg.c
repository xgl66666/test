/******************************************************************************
 *                         COPYRIGHT 2006 IWEDIA TECHNOLOGIES                 *
 ******************************************************************************
 *
 * MODULE NAME: SYS
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/product/comedia/branches/idecode_hd/build/src/version_cfg.c $
 *            $Rev: 332 $
 *            $Date: 2011-09-07 13:51:23 +0200 (Wed, 07 Sep 2011) $
 *
 * DESCRIPTION : functions needed to get information about the system version
 *
 *****************************************************************************/

/********************************************************/
/*              Includes                                */
/********************************************************/
#include "crules.h"
#include "string.h"
#include "stdio.h"
#include "version_cfg.h"
#if !defined(BOOTLOADER) && !defined(TRIG_OFF)
#include "trigger.h"
#endif

#if defined(USE_TDAL_SSU)
#include "tdal_ssu.h"
#endif

#if defined(USE_TDAL_OTA)
#include "tdal_common.h"
#endif

#ifdef __USE_CONFIG_FILE__
#include "config_file.h"
#endif

#include "sys_mngr_cfg.h"
/********************************************************/
/*              Defines                                 */
/********************************************************/

/* Imported from environment variable VERSION_SUFFIX in make/generic.mak
 * Use double-quoting for the suffix in the environment: this allows the variable
 * to define suffixes with spaces, and avoids problems with delimiters.
 * Note that the shell will remove them anyway when transferred to the makefile.
 * Also note the stringify hack required to convert that suffix.
 * To remove the version suffix, set environment variable VERSION_SUFFIX to "".
 */
#define STR(s) #s
#ifndef VERSION_SUFFIX
/* No version suffix defined, default to a development version to avoid release leaks */
#define VERSION_SUFFIX_STR(v) " DEV"
#else
/* Version suffix defined, append a space and stringify the suffix */
#define VERSION_SUFFIX_STR(v) " " STR(v)
#endif


/* PRODUCT_VERSION_MW, PRODUCT_VERSION_APP, PRODUCT_VERSION_CUST,
 * PRODUCT_RELEASE_DAY, PRODUCT_RELEASE_MONTH, PRODUCT_RELEASE_YEAR
 * are imported from environment variables defined in make/generic.mak
 */
#ifndef PRODUCT_VERSION_MW
/* No build defined defined*/
#define PRODUCT_MW_VER_STR(v) "0.0.0"
#else
/* Build defined */
#define PRODUCT_MW_VER_STR(v) STR(v)
#endif

#ifndef PRODUCT_VERSION_APP
/* No release version defined */
#define PRODUCT_APP_VER_STR(v) "0.0.0"
#else
/* Release version defined */
#define PRODUCT_APP_VER_STR(v) STR(v)
#endif

#ifndef PRODUCT_VERSION_CUST
/* No release version defined */
#define PRODUCT_CUST_VER_STR(v) "0.0.0"
#else
/* Release version defined */
#define PRODUCT_CUST_VER_STR(v) STR(v)
#endif

#ifndef PRODUCT_RELEASE_DAY
/* No release day defined */
#define PRODUCT_RELEASE_DAY_STR(v) "01"
#else
/* Release day defined */
#define PRODUCT_RELEASE_DAY_STR(v) STR(v)
#endif

#ifndef PRODUCT_RELEASE_MONTH
/* No release month defined */
#define PRODUCT_RELEASE_MONTH_STR(v) "01"
#else
/* Release month defined */
#define PRODUCT_RELEASE_MONTH_STR(v) STR(v)
#endif

#ifndef PRODUCT_RELEASE_YEAR
/* No release year defined */
#define PRODUCT_RELEASE_YEAR_STR(v) "2013"
#else
/* Release year defined */
#define PRODUCT_RELEASE_YEAR_STR(v) STR(v)
#endif

#define PRODUCT_RELEASE_DATE "(" PRODUCT_RELEASE_DAY_STR(PRODUCT_RELEASE_DAY) " " PRODUCT_RELEASE_MONTH_STR(PRODUCT_RELEASE_MONTH) " " PRODUCT_RELEASE_YEAR_STR(PRODUCT_RELEASE_YEAR) ") "
#define kSYS_INFO_MW_VERSION "v1.1 b" PRODUCT_MW_VER_STR(PRODUCT_VERSION_MW)
#define kSYS_INFO_APP_VERSION "v" PRODUCT_APP_VER_STR(PRODUCT_VERSION_APP)
#define kSYS_INFO_CUST_VERSION "v" PRODUCT_CUST_VER_STR(PRODUCT_VERSION_CUST)

#define VERSION_CFG_NUMBER kSYS_INFO_MW_VERSION VERSION_SUFFIX_STR(VERSION_SUFFIX)
#define VERSION_CFG_DATE PRODUCT_RELEASE_DATE

#define VERSION_CFG_UDATE_ID  0x0101050A


#ifdef MEDIUM_SAT
#define kSYS_INFO_DEVICE_NAME "SAT" VERSION_CFG_DATE
#endif
#ifdef MEDIUM_CAB
#define kSYS_INFO_DEVICE_NAME "CAB" VERSION_CFG_DATE
#endif
#ifdef MEDIUM_TER
#define kSYS_INFO_DEVICE_NAME "TER" VERSION_CFG_DATE
#endif
#define kSYS_INFO_SOFTWARE_VERSION VERSION_CFG_NUMBER

#define kSYS_INFO_HARDWARE_MODEL        "CHN"

#define kSYS_INFO_HARDWARE_RELEASE		"IDC3000_7105"
#define kSYS_INFO_HARDWARE_VERSION		"RefBoard X.X"
#define kSYS_INFO_PRODUCT_VERSION       "ABC"
#define kSYS_INFO_PRODUCT_MODEL         "ABC"

#define kSYS_INFO_HARDWARE_DRIVER_VERSION       "XX.YY"
#define kSYS_INFO_MANUFACTURER_ID               "ABC"

#define kSYS_INFO_OUI                       0x001a9a /* 0x00015A is DVB OUI you need CUSTOM it with your own OUI*/

#define kSYS_INFO_HARDWARE_COMP_MODEL       0x7640
#define kSYS_INFO_HARDWARE_COMP_VERSION     0x1957
#define kSYS_INFO_PRODUCT_COMP_MODEL        0x7640  /* Comedia V2 */
#define kSYS_INFO_PRODUCT_COMP_VERSION      0x0202  /* Comedia main version [V2].2.2 you need CUSTOM it with your own versionning*/

#define kSYS_INFO_VERSIONID_MSBF	        0x5197

#define kSYS_INFO_NVMEXCHANGE_VERSION      	0x00040000
#define kSYS_INFO_BOOTLOADER_VERSION      	0x00040000
/*
    kSYS_INFO_MHEG_WHOAMI: mmmcccvvv
    mmm: manufacturer 'IWE' for Iwedia
    ccc: model code 'I09' for IDTV7109
    vvv: version number '222' for v2.2.2.XX
*/
#define kSYS_INFO_MHEG_WHOAMI   "IWEI67222"

#define kOTA_STATUS_DIAGNOSTIC_DEFAULT "FF"
#define kOTA_STATUS_DIAGNOSTIC_DEFAULT_STRLEN 3

#define mSTRLEN_SAFE(str, maxlen, len)  { for(len = 0; (len < maxlen) && (str[len] != '\0'); len++); }

/********************************************************/
/*			  Local File Variables (LOCAL)			*/
/********************************************************/

#ifdef __USE_CONFIG_FILE__
/**
 *  Cache product IDs from config file to not load at each
 *  get element.
*/
LOCAL bool             vVCFGi_isProdIDsLoaded = FALSE;
LOCAL tCFGF_ProductIDs vVCFGi_stProdIDs;
LOCAL char			   vVCFGi_stBrandName[256];
#endif

#if defined(USE_TDAL_OTA)
LOCAL tTDAL_DiagnosticHWParameters hwParameters;
LOCAL tTDAL_OTA_HWParameters otaParameters;
#endif

#if defined(USE_TDAL_SSU)
LOCAL tTDAL_SSU_StatusInfo    ssuStatusInfo = {0};
LOCAL tTDAL_SSU_PlatformInfo  ssuPltfInfo   = {0};
#endif

/******************************************************************************
* Locales Functions Prototypes
******************************************************************************/
void SYS_INFOp_MJDtoYMD( int32_t siMJD, uint8_t * pucYear, uint8_t * pucMonth, uint8_t * pucDay );

#ifdef __USE_CONFIG_FILE__
LOCAL bool VCFGi_GetProdIDsFromConfigFile (void);
#endif

/********************************************************/
/*			  Functions Definitions (GLOBAL)			*/
/********************************************************/

void SYS_INFOp_MJDtoYMD( int32_t siMJD, uint8_t * pucYear, uint8_t * pucMonth, uint8_t * pucDay )
{
    int32_t     siYear, siMonth , K;
    uint32_t temp1;

    siYear  = (int32_t)(((siMJD*100) - 1507820) / 36525);
    temp1 = (uint32_t)(siYear*365.25);
    siMonth = (int32_t)(((siMJD*10000) - (149561000) - (temp1*10000))/306001);

    if (siMonth == 14 || siMonth == 15)
        K = 1;
    else
        K = 0;

    *pucDay   = (unsigned char)(siMJD - 14956 - (int32_t)(siYear*365.25) - (int32_t)(siMonth*30.6001));
    *pucYear  = (unsigned char)(siYear + K - 70);   /* UTC notation (i.e. from 1970 on). */
    *pucMonth = (unsigned char)(siMonth - 1 - K * 12);
}

/******************************************************************************
 * Function Name : SYS_INFO_GetDeviceName
 *
 * Description   : This function give the Product Name
 *
 * Side effects  : none
 *
 * Comment	     : none
 *
 *****************************************************************************/
char * SYS_INFO_GetDeviceName(void)
{
    char* pcDeviceName = kSYS_INFO_DEVICE_NAME;

#if defined(USE_TDAL_SSU)
    tTDAL_SSU_Error ssuError = eTDAL_SSU_NO_ERROR;

    ssuError = TDAL_SSU_GetPlatformInfo(&ssuPltfInfo);
    if (ssuError == eTDAL_SSU_NO_ERROR)
    {
        pcDeviceName = ssuPltfInfo.cDeviceBrandName;
    }
#endif

#ifdef USE_TDAL_OTA
   tTDAL_OTA_ErrorCode error = eTDAL_OTA_STATUS_NO_ERROR;

   error = TDAL_GetHWParameters(eTDAL_HW_PARAMETERS_DIAGNOSTIC, (tTDAL_HW_Handle)&hwParameters);
   if (error == eTDAL_OTA_STATUS_NO_ERROR)
   {
       pcDeviceName = hwParameters.cDeviceBrandName;
   }
#endif

   return (pcDeviceName);
}

/******************************************************************************
 * Function Name : SYS_INFO_GetSoftwareVersion
 *
 * Description   : This function give the System Software version
 *
 * Side effects  : none
 *
 * Comment	     : none
 *
 *****************************************************************************/
char * SYS_INFO_GetSoftwareVersion(void)
{
    static char version[100];

    strcpy(version, kSYS_INFO_SOFTWARE_VERSION);

#if 0
#if !defined(BOOTLOADER) && !defined(TRIG_OFF)
    char updateIdString[24];
    sprintf(updateIdString, " / %X", TRIG_GetCurrentUpdateID());
    strcat(version, updateIdString);
#endif
#endif

    return version;
}

/******************************************************************************
 * Function Name : SYS_INFO_GetSoftwareVersion
 *
 * Description   : This function give the System Software version OTA id
 *
 * Side effects  : none
 *
 * Comment	     : none
 *
 *****************************************************************************/
uint32_t SYS_INFO_GetSoftwareVersionId(void)
{
    uint32_t uiVersionId = VERSION_CFG_UDATE_ID;

#if defined(USE_TDAL_SSU)
    tTDAL_SSU_Error ssuError = eTDAL_SSU_NO_ERROR;

    ssuError = TDAL_SSU_GetStatusInfo(&ssuStatusInfo);
    if (ssuError == eTDAL_SSU_NO_ERROR)
    {
        uiVersionId = ssuStatusInfo.uiLastUpdateID;
    }
#endif

	return uiVersionId;
}

#if defined(CM_EMBEDDED)
/******************************************************************************
 * Function Name : SYS_INFO_CheckSoftwareVersion
 *
 * Description   : This function checks the System Software version
 *
 * Side effects  : none
 *
 * Comment	     : none
 *
 *****************************************************************************/
bool SYS_INFO_CheckSoftwareVersion(char* pSoftwareVersion)
{
    bool    result = FALSE;
    char    *pRunningSoftwareVersion;
    int     cmpResult;

    /* compare with the running software version */
    pRunningSoftwareVersion = SYS_INFO_GetSoftwareVersion();

    cmpResult = strncmp(
                    pSoftwareVersion,
                    pRunningSoftwareVersion,
                    strlen(pRunningSoftwareVersion));

    if (cmpResult == 0)
    {
        /* versions are the same */
        result = TRUE;
    }
    else
{
        /* versions are not the same */
        result = FALSE;

        printf(
            "SYS_INFO_CheckSoftwareVersion: the tested version (%s) is not the running software version (%s)\n",
            pSoftwareVersion,
            pRunningSoftwareVersion);
    }

    return(result);
}
#endif /* if CM_EMBEDDED */

/******************************************************************************
 * Function Name : SYS_INFO_GetHardwareModel
 *
 * Description   : This function give the Hardware model
 *
 * Side effects  : none
 *
 * Comment	     : none
 *
 *****************************************************************************/
char * SYS_INFO_GetHardwareModel(void)
{
    char* pcHardwareModel = kSYS_INFO_HARDWARE_MODEL;

#if defined(USE_TDAL_SSU)
    tTDAL_SSU_Error ssuError = eTDAL_SSU_NO_ERROR;

    ssuError = TDAL_SSU_GetPlatformInfo(&ssuPltfInfo);
    if (ssuError == eTDAL_SSU_NO_ERROR)
    {
        pcHardwareModel = ssuPltfInfo.cHardwareModel;
    }
#endif

#ifdef USE_TDAL_OTA
   tTDAL_OTA_ErrorCode error = eTDAL_OTA_STATUS_NO_ERROR;

   error = TDAL_GetHWParameters(eTDAL_HW_PARAMETERS_DIAGNOSTIC, (tTDAL_HW_Handle)&hwParameters);
   if (error == eTDAL_OTA_STATUS_NO_ERROR)
   {
       pcHardwareModel = hwParameters.cHardwareModel;
   }
#endif

    return(pcHardwareModel);
}

/******************************************************************************
 * Function Name : SYS_INFO_GetHardwareRelease
 *
 * Description   : This function give the Hardware release
 *
 * Side effects  : none
 *
 * Comment	     : none
 *
 *****************************************************************************/
char * SYS_INFO_GetHardwareRelease(void)
{
	return(kSYS_INFO_HARDWARE_RELEASE);
}

/******************************************************************************
 * Function Name : SYS_INFO_GetHardwareVersion
 *
 * Description   : This function give the Hardware version
 *
 * Side effects  : none
 *
 * Comment	     : none
 *
 *****************************************************************************/
char * SYS_INFO_GetHardwareVersion(void)
{
    char* pcHardwareVersion = kSYS_INFO_HARDWARE_MODEL;

#if defined(USE_TDAL_SSU)
    tTDAL_SSU_Error ssuError = eTDAL_SSU_NO_ERROR;

    ssuError = TDAL_SSU_GetPlatformInfo(&ssuPltfInfo);
    if (ssuError == eTDAL_SSU_NO_ERROR)
    {
        pcHardwareVersion = ssuPltfInfo.cHardwareVersion;
    }
#endif

#ifdef USE_TDAL_OTA
    tTDAL_OTA_ErrorCode error = eTDAL_OTA_STATUS_NO_ERROR;

    error = TDAL_GetHWParameters(eTDAL_HW_PARAMETERS_DIAGNOSTIC, (tTDAL_HW_Handle)&hwParameters);
    if (error == eTDAL_OTA_STATUS_NO_ERROR)
    {
       pcHardwareVersion = hwParameters.cHardwareVersion;
    }
#endif

    return (pcHardwareVersion);
}

/******************************************************************************
 * Function Name : SYS_INFO_GetProductVersion
 *
 * Description   : This function give the Product version
 *
 * Side effects  : none
 *
 * Comment	     : none
 *
 *****************************************************************************/
char * SYS_INFO_GetProductVersion(void)
{
    char* pcProductVersion = kSYS_INFO_PRODUCT_VERSION;

 #if defined(USE_TDAL_SSU)
    tTDAL_SSU_Error ssuError = eTDAL_SSU_NO_ERROR;

    ssuError = TDAL_SSU_GetPlatformInfo(&ssuPltfInfo);
    if (ssuError == eTDAL_SSU_NO_ERROR)
    {
        pcProductVersion = ssuPltfInfo.cProductVersion;
    }
#endif

#ifdef USE_TDAL_OTA
    tTDAL_OTA_ErrorCode error = eTDAL_OTA_STATUS_NO_ERROR;

    error = TDAL_GetHWParameters(eTDAL_HW_PARAMETERS_DIAGNOSTIC, (tTDAL_HW_Handle)&hwParameters);
    if (error == eTDAL_OTA_STATUS_NO_ERROR)
    {
        pcProductVersion = hwParameters.cProductVersion;
    }
#endif

	return (pcProductVersion);
}

/******************************************************************************
 * Function Name : SYS_INFO_GetProductModel
 *
 * Description   : This function give the Product model
 *
 * Side effects  : none
 *
 * Comment	     : none
 *
 *****************************************************************************/
char * SYS_INFO_GetProductModel(void)
{
    char* pcProductModel = kSYS_INFO_PRODUCT_MODEL;

#if defined(USE_TDAL_SSU)
    tTDAL_SSU_Error ssuError = eTDAL_SSU_NO_ERROR;

    ssuError = TDAL_SSU_GetPlatformInfo(&ssuPltfInfo);
    if (ssuError == eTDAL_SSU_NO_ERROR)
    {
        pcProductModel = ssuPltfInfo.cProductModel;
    }
#endif

#ifdef USE_TDAL_OTA
    tTDAL_OTA_ErrorCode error = eTDAL_OTA_STATUS_NO_ERROR;

    error = TDAL_GetHWParameters(eTDAL_HW_PARAMETERS_DIAGNOSTIC, (tTDAL_HW_Handle)&hwParameters);
    if (error == eTDAL_OTA_STATUS_NO_ERROR)
    {
        pcProductModel = hwParameters.cProductModel;
    }
#endif

    return (pcProductModel);
}

/******************************************************************************
 * Function Name : SYS_INFO_GetHardwareDriverVersion
 *
 * Description   : This function give the Hardware driver version
 *
 * Side effects  : none
 *
 * Comment	     : none
 *
 *****************************************************************************/
char* SYS_INFO_GetHardwareDriverVersion(void)
{
    char* pcDriverVersion = kSYS_INFO_HARDWARE_DRIVER_VERSION;

#if defined(USE_TDAL_SSU)
    tTDAL_SSU_Error ssuError = eTDAL_SSU_NO_ERROR;

    ssuError = TDAL_SSU_GetPlatformInfo(&ssuPltfInfo);
    if (ssuError == eTDAL_SSU_NO_ERROR)
    {
        pcDriverVersion = ssuPltfInfo.cHardwareDriverVersion;
    }
#endif

#ifdef USE_TDAL_OTA
    tTDAL_OTA_ErrorCode error = eTDAL_OTA_STATUS_NO_ERROR;

    error = TDAL_GetHWParameters(eTDAL_HW_PARAMETERS_DIAGNOSTIC, (tTDAL_HW_Handle)&hwParameters);
    if (error == eTDAL_OTA_STATUS_NO_ERROR)
    {
        pcDriverVersion = hwParameters.cHardwareDriverVersion;
    }
#endif

    return (pcDriverVersion);
}

/******************************************************************************
 * Function Name : SYS_INFO_GetLoadVersion
 *
 * Description   : This function give the Hardware driver version
 *
 * Side effects  : none
 *
 * Comment	     : none
 *
 *****************************************************************************/
char* SYS_INFO_GetLoadVersion(void)
{
   static char LoadVersion[16]="";

#if defined(USE_TDAL_SSU)
   uint32_t version = 0;

   version = SYS_INFO_GetSoftwareVersionId();
   snprintf(LoadVersion, 16, "%d.%d.%d",
            (version>>24) & 0xFF,
            (version>>16) & 0xFF,
            (version>>8)  & 0xFF);
#endif

#ifdef USE_TDAL_OTA
   TDAL_GetLoadVersion(LoadVersion);
#endif

   return(LoadVersion);
}

/******************************************************************************
 * Function Name : SYS_INFO_GetOTAStatus_Status
 *
 * Description   : 
 *
 * Side effects  : none
 *
 * Comment	     : Caller must allocate memory for cStatus
 *
 *****************************************************************************/
bool SYS_INFO_GetOTAStatus_Status(char *pcStatus)
{
    bool status = FALSE;
    uint8_t stringSize = 0;

#if defined(USE_TDAL_SSU)
    tTDAL_SSU_Error ssuError = eTDAL_SSU_NO_ERROR;

    ssuError = TDAL_SSU_GetStatusInfo(&ssuStatusInfo);
    if (ssuError == eTDAL_SSU_NO_ERROR)
    {
        mSTRLEN_SAFE(ssuStatusInfo.cStatus, sizeof(ssuStatusInfo.cStatus), stringSize);
        if(stringSize < sizeof(ssuStatusInfo.cStatus))
        {
            memcpy(pcStatus, ssuStatusInfo.cStatus, stringSize+1);
        }
        else
        {
            memcpy(pcStatus, kOTA_STATUS_DIAGNOSTIC_DEFAULT, kOTA_STATUS_DIAGNOSTIC_DEFAULT_STRLEN);
        }
        status = TRUE;
    }
#endif

#ifdef USE_TDAL_OTA
    tTDAL_DiagnosticOTAStatus stOTAStatus;
    if(TDAL_OTA_GetDiagnosticStatus(&stOTAStatus) == eTDAL_OTA_STATUS_NO_ERROR)
    {
        /* check whether the string is available*/
        mSTRLEN_SAFE(stOTAStatus.cStatus, sizeof(stOTAStatus.cStatus), stringSize);

        if(stringSize < sizeof(stOTAStatus.cStatus))
        {
            memcpy(pcStatus, stOTAStatus.cStatus, stringSize+1);
        }
        else
        {
            memcpy(pcStatus, kOTA_STATUS_DIAGNOSTIC_DEFAULT, kOTA_STATUS_DIAGNOSTIC_DEFAULT_STRLEN);
        }
        status = TRUE;
    }
#endif

    return status;
}

/******************************************************************************
 * Function Name : SYS_INFO_GetOTAStatus_Error
 *
 * Description   : 
 *
 * Side effects  : none
 *
 * Comment	     : Caller must allocate memory for cError
 *
 *****************************************************************************/
bool SYS_INFO_GetOTAStatus_Error(char *pcError)
{
    bool    status = FALSE;
    uint8_t stringSize = 0;

#if defined(USE_TDAL_SSU)
    tTDAL_SSU_Error ssuError = eTDAL_SSU_NO_ERROR;

    ssuError = TDAL_SSU_GetStatusInfo(&ssuStatusInfo);
    if (ssuError == eTDAL_SSU_NO_ERROR)
    {
        mSTRLEN_SAFE(ssuStatusInfo.cErrorCode, sizeof(ssuStatusInfo.cErrorCode), stringSize);
        if(stringSize < sizeof(ssuStatusInfo.cErrorCode))
        {
            memcpy(pcError, ssuStatusInfo.cErrorCode, stringSize+1);
        }
        else
        {
            memcpy(pcError, kOTA_STATUS_DIAGNOSTIC_DEFAULT, kOTA_STATUS_DIAGNOSTIC_DEFAULT_STRLEN);
        }
        status = TRUE;
    }
#endif

#ifdef USE_TDAL_OTA
    tTDAL_DiagnosticOTAStatus stOTAStatus;
    if(TDAL_OTA_GetDiagnosticStatus(&stOTAStatus) == eTDAL_OTA_STATUS_NO_ERROR)
    {
        /* check whether the string is available*/
        mSTRLEN_SAFE(stOTAStatus.cErrorCode, sizeof(stOTAStatus.cErrorCode), stringSize);

        if(stringSize < sizeof(stOTAStatus.cErrorCode))
        {
            memcpy(pcError, stOTAStatus.cErrorCode, stringSize+1);
        }
        else
        {
            memcpy(pcError, kOTA_STATUS_DIAGNOSTIC_DEFAULT, kOTA_STATUS_DIAGNOSTIC_DEFAULT_STRLEN);
        }
        status = TRUE;
    }
#endif

    return status;
}

/******************************************************************************
 * Function Name : SYS_INFO_GetOTAStatus_TimeAndDate
 *
 * Description   : 
 *
 * Side effects  : none
 *
 * Comment	     : Caller must allocate memory for cDate
 *
 *****************************************************************************/
bool SYS_INFO_GetOTAStatus_TimeAndDate(char *pcDate)
{
    bool    status = FALSE;

#if defined(USE_TDAL_SSU)
    tTDAL_SSU_Error ssuError = eTDAL_SSU_NO_ERROR;

    ssuError = TDAL_SSU_GetStatusInfo(&ssuStatusInfo);
    if (ssuError == eTDAL_SSU_NO_ERROR)
    {
        uint16_t mjd = 0;
        uint8_t y = 0;
        uint8_t m = 0;
        uint8_t d = 0;
        uint8_t hour = 0;
        uint8_t min = 0;
        uint8_t sec = 0;

        /* take first 2 bytes for MJD */
        mjd = (((uint8_t)ssuStatusInfo.cUtcTime[0]) << 8) + ((uint8_t)ssuStatusInfo.cUtcTime[1]);
        SYS_INFOp_MJDtoYMD(mjd, &y, &m, &d);
        snprintf (pcDate, 12, "%02d/%02d/%d.", d, m, y+1970);

        hour = 10 * (((uint8_t)ssuStatusInfo.cUtcTime[2] >> 4) & 0x0F) + ((uint8_t)ssuStatusInfo.cUtcTime[2] & 0x0F);
        min  = 10 * (((uint8_t)ssuStatusInfo.cUtcTime[3] >> 4) & 0x0F) + ((uint8_t)ssuStatusInfo.cUtcTime[3] & 0x0F);
        sec  = 10 * (((uint8_t)ssuStatusInfo.cUtcTime[4] >> 4) & 0x0F) + ((uint8_t)ssuStatusInfo.cUtcTime[4] & 0x0F);

        status = TRUE;
    }
#endif

#ifdef USE_TDAL_OTA
    tTDAL_DiagnosticOTAStatus stOTAStatus;
    uint8_t stringSize = 0;

    if(TDAL_OTA_GetDiagnosticStatus(&stOTAStatus) == eTDAL_OTA_STATUS_NO_ERROR)
    {
        /* check whether the string is available*/
        mSTRLEN_SAFE(stOTAStatus.cDateAndTime, sizeof(stOTAStatus.cDateAndTime), stringSize);

        if(stringSize < sizeof(stOTAStatus.cDateAndTime))
        {
            memcpy(pcDate, stOTAStatus.cDateAndTime, stringSize+1);
        }
        else
        {
            memcpy(pcDate, kOTA_STATUS_DIAGNOSTIC_DEFAULT, kOTA_STATUS_DIAGNOSTIC_DEFAULT_STRLEN);
        }
        status = TRUE;
    }
#endif

    return status;
}

/******************************************************************************
 * Function Name : SYS_INFO_GetManufacturerID
 *
 * Description   : This function give the manufacturer ID
 *
 * Side effects  : none
 *
 * Comment	     : none
 *
 *****************************************************************************/
char* SYS_INFO_GetManufacturerID(void)
{
    char* pcManufacturerID = kSYS_INFO_MANUFACTURER_ID;

#if defined(USE_TDAL_SSU)
    tTDAL_SSU_Error ssuError = eTDAL_SSU_NO_ERROR;

    ssuError = TDAL_SSU_GetPlatformInfo(&ssuPltfInfo);
    if (ssuError == eTDAL_SSU_NO_ERROR)
    {
        pcManufacturerID = ssuPltfInfo.cManufacturerID;
    }
#endif

#ifdef USE_TDAL_OTA
    tTDAL_OTA_ErrorCode error = eTDAL_OTA_STATUS_NO_ERROR;

    error = TDAL_GetHWParameters(eTDAL_HW_PARAMETERS_DIAGNOSTIC, (tTDAL_HW_Handle)&hwParameters);
    if (error == eTDAL_OTA_STATUS_NO_ERROR)
    {
        pcManufacturerID = hwParameters.cManufacturerID;
    }
#endif

    return(pcManufacturerID);
}

/******************************************************************************
 * Function Name : SYS_INFO_GetOui
 *
 * Description   : This function return the OUI (Organisation Unique identifier
 *
 * Side effects  : none
 *
 * Comment	     : none
 *
 *****************************************************************************/
uint32_t SYS_INFO_GetOui(void)
{

    uint32_t uiOui = kSYS_INFO_OUI;

#if defined(USE_TDAL_SSU)
    tTDAL_SSU_Error ssuError = eTDAL_SSU_NO_ERROR;

    ssuError = TDAL_SSU_GetPlatformInfo(&ssuPltfInfo);
    if (ssuError == eTDAL_SSU_NO_ERROR)
    {
        uiOui = ssuPltfInfo.uiOUI;
    }
#endif

#ifdef USE_TDAL_OTA
    tTDAL_OTA_ErrorCode error = eTDAL_OTA_STATUS_NO_ERROR;

    error = TDAL_GetHWParameters(eTDAL_HW_PARAMETERS_OTA, (tTDAL_HW_Handle)&otaParameters);
    if (error == eTDAL_OTA_STATUS_NO_ERROR)
    {
        uiOui = otaParameters.uiOUI;
    }
#endif

    return uiOui;
}

/******************************************************************************
 * Function Name : SYS_INFO_GetProductModelId
 *
 * Description   : This function return the
 *
 * Side effects  : none
 *
 * Comment	     : none
 *
 *****************************************************************************/
uint16_t SYS_INFO_GetProductModelId(void)
{
    uint16_t uiProductModelId = kSYS_INFO_PRODUCT_COMP_MODEL;

#if defined(USE_TDAL_SSU)
    tTDAL_SSU_Error ssuError = eTDAL_SSU_NO_ERROR;

    ssuError = TDAL_SSU_GetPlatformInfo(&ssuPltfInfo);
    if (ssuError == eTDAL_SSU_NO_ERROR)
    {
        uiProductModelId = ssuPltfInfo.uiProductModel;
    }
#endif

#ifdef USE_TDAL_OTA
    tTDAL_OTA_ErrorCode error = eTDAL_OTA_STATUS_NO_ERROR;

    error = TDAL_GetHWParameters(eTDAL_HW_PARAMETERS_OTA, (tTDAL_HW_Handle)&otaParameters);
    if (error == eTDAL_OTA_STATUS_NO_ERROR)
    {
        uiProductModelId = otaParameters.uiProductModel;
    }
#endif

   return uiProductModelId;
}

/******************************************************************************
 * Function Name : SYS_INFO_GetProductVersionId
 *
 * Description   : This function return the
 *
 * Side effects  : none
 *
 * Comment	     : none
 *
 *****************************************************************************/
uint16_t SYS_INFO_GetProductVersionId(void)
{
    uint16_t uiProductVersionId = kSYS_INFO_PRODUCT_COMP_VERSION;

#if defined(USE_TDAL_SSU)
    tTDAL_SSU_Error ssuError = eTDAL_SSU_NO_ERROR;

    ssuError = TDAL_SSU_GetPlatformInfo(&ssuPltfInfo);
    if (ssuError == eTDAL_SSU_NO_ERROR)
    {
        uiProductVersionId = ssuPltfInfo.uiProductVersion;
    }
#endif

#ifdef USE_TDAL_OTA
    tTDAL_OTA_ErrorCode error = eTDAL_OTA_STATUS_NO_ERROR;

    error = TDAL_GetHWParameters(eTDAL_HW_PARAMETERS_OTA, (tTDAL_HW_Handle)&otaParameters);
    if (error == eTDAL_OTA_STATUS_NO_ERROR)
    {
        uiProductVersionId = otaParameters.uiProductVersion;
    }
#endif

   return uiProductVersionId;
}

/******************************************************************************
 * Function Name : SYS_INFO_GetHardwareModelId
 *
 * Description   : This function return the
 *
 * Side effects  : none
 *
 * Comment	     : none
 *
 *****************************************************************************/
uint16_t SYS_INFO_GetHardwareModelId(void)
{
    uint16_t uiHardwareModelId = kSYS_INFO_HARDWARE_COMP_MODEL;

#if defined(USE_TDAL_SSU)
    tTDAL_SSU_Error ssuError = eTDAL_SSU_NO_ERROR;

    ssuError = TDAL_SSU_GetPlatformInfo(&ssuPltfInfo);
    if (ssuError == eTDAL_SSU_NO_ERROR)
    {
        uiHardwareModelId = ssuPltfInfo.uiHardwareModel;
    }
#endif

#ifdef USE_TDAL_OTA
    tTDAL_OTA_ErrorCode error = eTDAL_OTA_STATUS_NO_ERROR;

    error = TDAL_GetHWParameters(eTDAL_HW_PARAMETERS_OTA, (tTDAL_HW_Handle)&otaParameters);
    if (error == eTDAL_OTA_STATUS_NO_ERROR)
    {
        uiHardwareModelId = otaParameters.uiHardwareModel;
    }
#endif

    return uiHardwareModelId;
}

/******************************************************************************
 * Function Name : SYS_INFO_GetHardwareVersionId
 *
 * Description   : This function return the
 *
 * Side effects  : none
 *
 * Comment	     : none
 *
 *****************************************************************************/
uint16_t SYS_INFO_GetHardwareVersionId(void)
{
    uint16_t uiHardwareVersionId = kSYS_INFO_HARDWARE_COMP_VERSION;

#if defined(USE_TDAL_SSU)
    tTDAL_SSU_Error ssuError = eTDAL_SSU_NO_ERROR;

    ssuError = TDAL_SSU_GetPlatformInfo(&ssuPltfInfo);
    if (ssuError == eTDAL_SSU_NO_ERROR)
    {
        uiHardwareVersionId = ssuPltfInfo.uiHardwareVersion;
    }
#endif

#ifdef USE_TDAL_OTA
    tTDAL_OTA_ErrorCode error = eTDAL_OTA_STATUS_NO_ERROR;

    error = TDAL_GetHWParameters(eTDAL_HW_PARAMETERS_OTA, (tTDAL_HW_Handle)&otaParameters);
    if (error == eTDAL_OTA_STATUS_NO_ERROR)
    {
        uiHardwareVersionId = otaParameters.uiHardwareVersion;
    }
#endif

    return uiHardwareVersionId;
}


/******************************************************************************
 * Function Name : SYS_INFO_GetBootloaderVersion
 *
 * Description   : This function return the
 *
 * Side effects  : none
 *
 * Comment	     : none
 *
 *****************************************************************************/
uint32_t SYS_INFO_GetBootloaderVersion(void)
{
	return(kSYS_INFO_BOOTLOADER_VERSION);
}

/******************************************************************************
 * Function Name : SYS_INFO_NvmExVersion
 *
 * Description   : This function return the
 *
 * Side effects  : none
 *
 * Comment	     : none
 *
 *****************************************************************************/
uint32_t SYS_INFO_NvmExVersion(void)
{
	return(kSYS_INFO_NVMEXCHANGE_VERSION);
}


/******************************************************************************
 * Function Name : SYS_INFO_GetVersionIdMsb
 *
 * Description   : This function return the
 *
 * Side effects  : none
 *
 * Comment	     : none
 *
 *****************************************************************************/
uint16_t SYS_INFO_GetVersionIdMsb()
{
	return(kSYS_INFO_VERSIONID_MSBF);
}

#if defined(CM_EMBEDDED)
/******************************************************************************
 * Function Name : SYS_INFO_GetMhegIdString
 *
 * Description   : This function return the
 *
 * Side effects  : none
 *
 * Comment	     : none
 *
 *****************************************************************************/
const char * SYS_INFO_GetMhegIdString(void)
{
	return(kSYS_INFO_MHEG_WHOAMI);
}
#endif /* if CM_EMBEDDED */


/******************************************************************************
* Locales Functions Implementation
******************************************************************************/

#ifdef __USE_CONFIG_FILE__
LOCAL bool VCFGi_GetProdIDsFromConfigFile
(
   void
)
{
   tCFGF_Configuration *p_cfg;

   if (vVCFGi_isProdIDsLoaded) return TRUE;

   p_cfg = CFGF_Read(kCFGF_ID_PROD_ID);
   if (p_cfg == NULL) return FALSE;
   strcpy(vVCFGi_stBrandName,p_cfg->stProductIDs.pcBrandName);
   memcpy(&vVCFGi_stProdIDs,&p_cfg->stProductIDs,sizeof(tCFGF_ProductIDs));
   vVCFGi_stProdIDs.pcBrandName = vVCFGi_stBrandName;
   CFGF_Free(p_cfg);
   vVCFGi_isProdIDsLoaded = TRUE;

   return TRUE;
}

#endif

/* version_cfg.c */
