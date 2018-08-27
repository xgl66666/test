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
#include "stdlib.h"
#include "crules.h"
#include "string.h"
#include "stdio.h"
#include "version_cfg.h"
#if !defined(BOOTLOADER) && !defined(TRIG_OFF)
#include "trigger.h"
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

#if defined(PRODUCT_NL_CAB_5197G)
#define VERSION_CFG_NUMBER "v0.1.1" VERSION_SUFFIX_STR(VERSION_SUFFIX)
#define VERSION_CFG_DATE "(17 October  2013) "
#define VERSION_CFG_UDATE_ID  0x00040000
#elif  defined(PRODUCT_CH_CAB_8080CL)
#define VERSION_CFG_NUMBER "v0.1.3" VERSION_SUFFIX_STR(VERSION_SUFFIX)
#define VERSION_CFG_DATE "(24 April  2014) "
#define VERSION_CFG_UDATE_ID  0x00040000
#elif defined(PRODUCT_NL_CAB_5197D)
#define VERSION_CFG_NUMBER "v0.1.5" VERSION_SUFFIX_STR(VERSION_SUFFIX)
#define VERSION_CFG_DATE "(09 May  2014) "
#define VERSION_CFG_UDATE_ID  0x00040000
#elif defined(PRODUCT_NL_CAB_DEN5197)
#define VERSION_CFG_NUMBER "1.1 Build 3.1.9" VERSION_SUFFIX_STR(VERSION_SUFFIX)
#define VERSION_CFG_DATE "(27 May 2014)"
#define VERSION_CFG_UDATE_ID 0x00000001
#elif  defined(PRODUCT_CH_CAB_DEN8080CL)
#define VERSION_CFG_NUMBER "1.1 Build 3.1.2" VERSION_SUFFIX_STR(VERSION_SUFFIX)
#define VERSION_CFG_DATE "(16 June  2014) "
#define VERSION_CFG_UDATE_ID  0x00040000
#elif defined (PRODUCT_SW_CAB_5197)
#define VERSION_CFG_NUMBER "1.1 Build 4.1.5a" VERSION_SUFFIX_STR(VERSION_SUFFIX)
#define VERSION_CFG_DATE "(09 January  2015) "
#define VERSION_CFG_UDATE_ID  0x0101050A
#else
#define VERSION_CFG_NUMBER "1.1 Build 4.1.6" VERSION_SUFFIX_STR(VERSION_SUFFIX)
#define VERSION_CFG_DATE "(09 January  2015) "
#define VERSION_CFG_UDATE_ID  0x0101050A
//#error Unknown board
#endif

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

#define kSYS_INFO_OUI                        	0x001a9a /* 0x00015A is DVB OUI you need CUSTOM it with your own OUI*/

#define kSYS_INFO_HARDWARE_COMP_MODEL       0x7640
#define kSYS_INFO_HARDWARE_COMP_VERSION     0x1957
#define kSYS_INFO_PRODUCT_COMP_MODEL        0x7640  /* Comedia V2 */
#define kSYS_INFO_PRODUCT_COMP_VERSION      0x0202  /* Comedia main version [V2].2.2 you need CUSTOM it with your own versionning*/

#define kSYS_INFO_VERSIONID_MSBF	        0x5197

#define kSYS_INFO_NVMEXCHANGE_VERSION      	0x00040000
#define kSYS_INFO_BOOTLOADER_VERSION      	0x00040000

#define kSYS_INFO_HARDWARE_DRIVER_VERSION       "XX.YY"

/*
    kSYS_INFO_MHEG_WHOAMI: mmmcccvvv
    mmm: manufacturer 'IWE' for Iwedia
    ccc: model code 'I09' for IDTV7109
    vvv: version number '222' for v2.2.2.XX
*/
#define kSYS_INFO_MHEG_WHOAMI   "IWEI67222"

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

/******************************************************************************
* Locales Functions Prototypes
******************************************************************************/

#ifdef __USE_CONFIG_FILE__
LOCAL bool VCFGi_GetProdIDsFromConfigFile (void);
#endif


/********************************************************/
/*			  Functions Definitions (GLOBAL)			*/
/********************************************************/



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
#ifdef __USE_CONFIG_FILE__
   if (!VCFGi_GetProdIDsFromConfigFile())
      vVCFGi_stBrandName[0]=0; /* empty string */
   return(vVCFGi_stBrandName);
#else
   return(kSYS_INFO_DEVICE_NAME);
#endif
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
/*    static char version[100]; */
/* VSM: remove update ID from SW version*/
/*
#if !defined(BOOTLOADER) && !defined(TRIG_OFF)
    char updateIdString[20];
#endif
    strcpy(version, kSYS_INFO_SOFTWARE_VERSION " / ");
#if !defined(BOOTLOADER) && !defined(TRIG_OFF)
    sprintf(updateIdString, "%X", TRIG_GetCurrentUpdateID());
    strcat(version, updateIdString);
#endif
    return version;
#endif
*/
    return kSYS_INFO_SOFTWARE_VERSION;
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

	return VERSION_CFG_UDATE_ID;
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
/* VSM: HW model is string, not uint as on previous projects,
        so it cannot be read from config file due to constaints of configmake.exe tool
*/
/*
#ifdef __USE_CONFIG_FILE__
   static char __hardware_model__[10];

   if (VCFGi_GetProdIDsFromConfigFile())
      sprintf(__hardware_model__,"%04X",vVCFGi_stProdIDs.ui16PltfModel);
   else
      strcpy(__hardware_model__,"XXXX");

   return(__hardware_model__);
#else
	return(kSYS_INFO_HARDWARE_MODEL);
#endif
*/
	return(kSYS_INFO_HARDWARE_MODEL);
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
#ifdef __USE_CONFIG_FILE__
   static char __hardware_version__[30];

   if (!VCFGi_GetProdIDsFromConfigFile())
      vVCFGi_stProdIDs.ui16PltfVersion=0;
   sprintf(__hardware_version__,"%x",vVCFGi_stProdIDs.ui16PltfVersion);
   return(__hardware_version__);
#else
	return(kSYS_INFO_HARDWARE_VERSION);
#endif
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
    return(SYS_MNGR_CFG_GetHardwareDriverVersion());
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
#ifdef __USE_CONFIG_FILE__
   if (VCFGi_GetProdIDsFromConfigFile()) return(kSYS_INFO_OUI);
   return(0xFFFFFFFF);
#else
	return(kSYS_INFO_OUI);
#endif
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
#ifdef __USE_CONFIG_FILE__
   if (VCFGi_GetProdIDsFromConfigFile()) return(vVCFGi_stProdIDs.ui16ProdModel);
   return(0xFFFF);
#else
	return(kSYS_INFO_PRODUCT_COMP_MODEL);
#endif
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

   char __driver_version__[6];
   uint16_t version;

   sprintf(__driver_version__,"%s",SYS_MNGR_CFG_GetHardwareDriverVersion());
   __driver_version__[2] = __driver_version__[3];
   __driver_version__[3] = __driver_version__[4];
   __driver_version__[4] = '\0';
   version = (uint16_t)atoi(__driver_version__);

   return(version);
#if 0
#ifdef __USE_CONFIG_FILE__
   if (VCFGi_GetProdIDsFromConfigFile()) return(vVCFGi_stProdIDs.ui16ProdVersion);
   return(0xFFFF);
#else
	return(kSYS_INFO_PRODUCT_COMP_VERSION);
#endif
#endif
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
#ifdef __USE_CONFIG_FILE__
   if (VCFGi_GetProdIDsFromConfigFile()) return(kSYS_INFO_HARDWARE_COMP_MODEL);
   return(0xFFFF);
#else
	return(kSYS_INFO_HARDWARE_COMP_MODEL);
#endif
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
#ifdef __USE_CONFIG_FILE__
   if (VCFGi_GetProdIDsFromConfigFile()) return(vVCFGi_stProdIDs.ui16PltfVersion);
   return(0xFFFF);
#else
	return(kSYS_INFO_HARDWARE_COMP_VERSION);
#endif
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
