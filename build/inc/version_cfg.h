/******************************************************************************
 *                         COPYRIGHT 2005 IWEDIA TECHNOLOGIES                         *
 ******************************************************************************
 *
 * MODULE NAME:
 *
 * FILE NAME: $URL: http://ren-svn-01/svncomedia/idecode/product/comedia/branches/snt_cos/main/build/inc/version_cfg.h $
 *            $Rev: 9175 $
 *            $Date: 2012-02-14 11:37:55 +0100 (mar., 14 févr. 2012) $
 *
 * PUBLIC
 *
 * DESCRIPTION: functions needed to get information about the system version
 *
 *****************************************************************************/

#ifndef _VERSION_CFG_H_
#define _VERSION_CFG_H_

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************/
/*              Defines                                */
/*******************************************************/

#define kSYS_INFO_MAJOR_VERSION_MASK		0xFFFF0000	
#define kSYS_INFO_DVB_OUI		0x00015A	


/*******************************************************/
/*              Functions                              */
/*******************************************************/

uint32_t    SYS_INFO_GetOui               (void);
uint16_t    SYS_INFO_GetProductModelId    (void);
uint16_t    SYS_INFO_GetProductVersionId  (void);
uint16_t    SYS_INFO_GetHardwareModelId   (void);
uint16_t    SYS_INFO_GetHardwareVersionId (void);
uint32_t    SYS_INFO_NvmExVersion         (void);
uint16_t    SYS_INFO_GetVersionIdMsb      (void);
#if !defined(DOWNLOADER)
/* Only for application COMEDIA */
char       *SYS_INFO_GetDeviceName        (void);
char       *SYS_INFO_GetSoftwareVersion   (void);
uint32_t    SYS_INFO_GetSoftwareVersionId();
char       *SYS_INFO_GetSoftwareDate      (void);
char       *SYS_INFO_GetHardwareModel     (void);
char       *SYS_INFO_GetHardwareDriverVersion(void);
char       *SYS_INFO_GetProductModel   (void);
char       *SYS_INFO_GetProductVersion  (void);
char       *SYS_INFO_GetHardwareRelease   (void);
char       *SYS_INFO_GetHardwareVersion   (void);
char       *SYS_INFO_GetDownloaderVersion (void);
char       *SYS_INFO_GetResourcesUpdateId (void);
char       *SYS_INFO_GetConfigurationUpdateId (void);

uint8_t SYS_INFO_GetUsageIdFromConfigFile(void);
#if defined(CM_EMBEDDED)
bool        SYS_INFO_CheckSoftwareVersion (char *pSoftwareVersion);
const char *SYS_INFO_GetMhegIdString      (void);
#endif
#endif

#ifdef __cplusplus
}
#endif

#endif /*_VERSION_CFG_H_*/
