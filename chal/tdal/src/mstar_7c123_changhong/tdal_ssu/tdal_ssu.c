/******************************************************************************
 *                   COPYRIGHT 2004 IWEDIA TECHNOLOGIES                       *
 ******************************************************************************
 *
 * MODULE NAME: TDAL SSU
 *
 * DESCRIPTION
 *
 *****************************************************************************/

/********************************************************
 *        Includes                                      *
 ********************************************************/

#include "tkel.h"
#include "tbox.h"
#include "tdal_common.h"
#include "tdal_ssu.h"
#include "tdal_ssu_p.h"
#include "tdal_fla.h"

/********************************************************
 *        Defines                                       *
 ********************************************************/
#define FLASH_BLOCK_SIZE				(0x10000)

/********************************************************
 *        Macros                                        *
 ********************************************************/

/********************************************************
 *        Global   Variables (GLOBAL/IMPORT)            *
 ********************************************************/

/********************************************************
 *        Local   Module   Variables   (MODULE)         *
 ********************************************************/

/********************************************************
 *        Local   File   Variables   (LOCAL)            *
 ********************************************************/

mTBOX_SET_MODULE(eTDAL_SSU);

/********************************************************
 *        Module   functions                            *
 ********************************************************/

/********************************************************
 *          Local functions                             *
 ********************************************************/

GLOBAL void TDAL_SSU_Init(void)
{
    mTBOX_FCT_ENTER("TDAL_SSU_Init");
    mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_SSU_Init NOT IMPLEMENTED\n"));
    mTBOX_RETURN;
}

GLOBAL void TDAL_SSU_Term(void)
{
    mTBOX_FCT_ENTER("TDAL_SSU_Term");
    mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_SSU_Term NOT IMPLEMENTED\n"));
    mTBOX_RETURN;
}

/*********************************************************************************************
 *
 * KAON definition 2016.12.29 bsjung
 *
 ********************************************************************************************/
FDCAInfo_t						g_currFDCAInfo;
INFOInfo_t						g_currINFOInfo;
BCAInfo_t						g_currBCAInfo;

#define ZIP_CRC32_POLY	0xEDB88320UL
uint32_t calc_crc32( uint8_t* src, uint32_t len )
{
	uint32_t	idx;
	uint32_t 	bit;
	uint32_t 	data;
	uint32_t	crc = 0xFFFFFFFFUL;

    for (idx = 0; idx < len; idx++) {
        for (data = *src++, bit = 0; bit < 8; bit++, data >>= 1) {
            crc = (crc >> 1) ^ (((crc ^ data) & 1) ? ZIP_CRC32_POLY : 0);
        }
    }

    return (~crc);
}

int32_t power(int32_t base, int32_t exp)
{
	int32_t ret					=	1;
	int32_t i;

	for(i=0;i<exp;i++)
	{
		ret	*= base;
	}
	return ret;
}

int32_t stringToInt(int8_t* nrStr)
{
	int32_t nrChars				=	0;
	int32_t result				=	0;
	int32_t i					=	0;
	int32_t j					=	0;
	int32_t k					=	0;
	int32_t digit 				= 	0;
	int32_t exp 				= 	0;
	int32_t add					=	0;

	while(nrStr[k]!='\0')
	{
		if(nrStr[k] != '.')
		{
			nrChars++;
		}

		k++;
	}

	//while you dont get to the end of the string
	while(nrStr[i]!='\0')
	{
		if(nrStr[i] != '.')
		{
			digit		= 	nrStr[i]-48;//48 is zero ascii code
			exp			= 	nrChars-j-1;
			add			= 	digit*power(10,exp);
			result		+=	add;
			j++;
		}

		i++;
	}

	return result;
}

uint32_t DecimalToHex(int32_t decimal)
{
        uint32_t hex = 0x00000000;
        uint32_t  a = 0x00000000, b = 0x00000000, c = 0x00000000, d = 0x00000000, e = 0x00000000, f = 0x00000000, g = 0x00000000, h = 0x00000000;

        d = (decimal/1000000);
        c = (decimal/10000)%100;
        b = (decimal/100)%100;
        a = (decimal%100);

        /* 0xdbca */
        hex = (d << 24) | (c << 16) | (b << 8) | a;

        mTBOX_PRINT((kTBOX_LF, "DecimalToHex [%d -> 0x%08X]\n", decimal, hex));

        return hex;
}

GLOBAL tTDAL_SSU_Error TDAL_SSU_GetPlatformInfo(tTDAL_SSU_PlatformInfo *pstSsuInfo)
{
    mTBOX_FCT_ENTER("TDAL_SSU_GetPlatformInfo");

    mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_SSU_GetPlatformInfo IMPLEMENTED BY KAON-LOADER\n"));

	if(pstSsuInfo == NULL) {
		mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_SSU_GetPlatformInfo Failed\n"));
		mTBOX_RETURN eTDAL_OTA_STATUS_ERROR;
	}

    /*********************************************************************************************
     *
     * BCA read
     *
     ********************************************************************************************/
	memset((void*)&g_currBCAInfo, 0x00, FLASH_BCA_SIZE);
	memcpy((void*)&g_currBCAInfo, (const void*)RAM_BCA_DEC_BASE_ADDRESS, FLASH_BCA_SIZE);

	mTBOX_PRINT((kTBOX_LF, "===================== Read BCA ==================================\n"));
	mTBOX_PRINT((kTBOX_LF, "model_name	= %s\n", g_currBCAInfo.model_name));
	mTBOX_PRINT((kTBOX_LF, "model_id	= 0x%04X\n", g_currBCAInfo.model_id));
	mTBOX_PRINT((kTBOX_LF, "hardware_id	= 0x%04X\n", g_currBCAInfo.hardware_id));
	mTBOX_PRINT((kTBOX_LF, "buyer_id	= 0x%04X\n", g_currBCAInfo.buyer_id));
	mTBOX_PRINT((kTBOX_LF, "region_id	= 0x%04X\n", g_currBCAInfo.region_id));
	mTBOX_PRINT((kTBOX_LF, "oui			= 0x%08X\n", g_currBCAInfo.oui));
	mTBOX_PRINT((kTBOX_LF, "===================================================================\n"));

	pstSsuInfo->uiOUI						= g_currBCAInfo.oui;
	strcpy(pstSsuInfo->cHardwareModel, g_currBCAInfo.model_name);
	pstSsuInfo->uiHardwareModel				= DOWNLOAD_MODEL_ID;
	strcpy(pstSsuInfo->cHardwareVersion,"0x001"); //TODO
	pstSsuInfo->uiHardwareVersion			= g_currBCAInfo.hardware_id;
	strcpy(pstSsuInfo->cProductModel, g_currBCAInfo.model_name);
	pstSsuInfo->uiProductModel				= DOWNLOAD_MODEL_ID;
	//pstSsuInfo->cProductVersion  			= N/A
	pstSsuInfo->uiProductVersion			= g_currBCAInfo.hardware_id;
	strcpy(pstSsuInfo->cDeviceBrandName, g_currBCAInfo.model_name);
	strcpy(pstSsuInfo->cHardwareDriverVersion,"0x001"); //TODO
	strcpy(pstSsuInfo->cManufacturerID,"0x001"); //TODO

	mTBOX_PRINT((kTBOX_LF, "===================== Read pstSsuInfo ==================================\n"));
	mTBOX_PRINT((kTBOX_LF, "uiOUI				= 0x%08X\n", pstSsuInfo->uiOUI));
	mTBOX_PRINT((kTBOX_LF, "cHardwareModel		= %s\n", pstSsuInfo->cHardwareModel));
	mTBOX_PRINT((kTBOX_LF, "uiHardwareModel		= %4d\n", pstSsuInfo->uiHardwareModel));
	mTBOX_PRINT((kTBOX_LF, "uiHardwareVersion	= 0x%04X\n", pstSsuInfo->uiHardwareVersion));
	mTBOX_PRINT((kTBOX_LF, "cProductModel		= %s\n", pstSsuInfo->cProductModel));
	mTBOX_PRINT((kTBOX_LF, "uiProductModel		= %4d", pstSsuInfo->uiProductModel));
	mTBOX_PRINT((kTBOX_LF, "uiProductVersion	= 0x%04X\n", pstSsuInfo->uiProductVersion));
	mTBOX_PRINT((kTBOX_LF, "cDeviceBrandName	= %s\n", pstSsuInfo->cDeviceBrandName));
	mTBOX_PRINT((kTBOX_LF, "===================================================================\n"));

    mTBOX_RETURN eTDAL_SSU_NO_ERROR;
}

/*
 * tdal_ssu_GetStatusInfo.c
 *
 *  Created on: 2017. 3. 29.
 *      Author: k161110
 */
GLOBAL tTDAL_SSU_Error TDAL_SSU_GetStatusInfo(tTDAL_SSU_StatusInfo *pstSsuStatus)
{
	uint8_t		i = 0, INFOReadBuf[FLASH_INFO_SIZE];
	uint32_t 	NumberRead;
	uint32_t	crcCal;
	uint32_t	NumberWritten;
	tTDAL_FLA_ErrorCode	FlashError;

  mTBOX_FCT_ENTER("TDAL_SSU_GetStatusInfo");

  mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_SSU_GetStatusInfo IMPLEMENTED BY KAON-LOADER\n"));

	if(pstSsuStatus == NULL) {
		mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_SSU_GetStatusInfo Failed\n"));
		mTBOX_RETURN eTDAL_OTA_STATUS_ERROR;
	}

    /*********************************************************************************************
     *
     * INFO read
     *
     ********************************************************************************************/
	NumberRead = TDAL_FLA_Read((uint32_t)FLASH_INFO_BASE_ADDRESS, (uint8_t*)INFOReadBuf, (uint32_t)FLASH_INFO_SIZE);
  	if(NumberRead != FLASH_INFO_SIZE)
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL, "INFO Read Failed\n"));
		mTBOX_RETURN eTDAL_OTA_STATUS_ERROR;
	}

	memset(&g_currINFOInfo, 0x00, FLASH_INFO_SIZE);
	memcpy(&g_currINFOInfo, INFOReadBuf, FLASH_INFO_SIZE);

	mTBOX_PRINT((kTBOX_LF, "===================== Read INFO ==================================\n"));
	mTBOX_PRINT((kTBOX_LF, "application_version	= %06D\n", g_currINFOInfo.application_version));
	mTBOX_PRINT((kTBOX_LF, "downloader_version	= %06D\n", g_currINFOInfo.downloader_version));
	mTBOX_PRINT((kTBOX_LF, "standby_mode		= %02X\n", g_currINFOInfo.standby_mode));
	mTBOX_PRINT((kTBOX_LF, "ts_entry_curr_num	= %02D\n", g_currINFOInfo.ts_entry_curr_num));
	mTBOX_PRINT((kTBOX_LF, "download_error_code	= 0x%04X\n", g_currINFOInfo.download_error_code));
	mTBOX_PRINT((kTBOX_LF, "download_model_id	= %04D\n", g_currINFOInfo.download_model_id));
	mTBOX_PRINT((kTBOX_LF, "===================================================================\n"));
	/*********************************************************************************************************************************************
	*
	* Note: Variable bFirstBoot must be set to 1 on first boot after SSU, and on any subsequent boot it must have value equal to 0.
	* 	Variable bForced must be set on 1 if SSU is not triggered by TDAL SSU API, otherwise it should be set to 0.
	*       after SSU, Downloader will save boot_count value as "0xAA".  
	*
	************************************************************************************************************************************************/
	mTBOX_PRINT((kTBOX_LF, "g_currINFOInfo.boot_count	= 0x%02X\n", g_currINFOInfo.boot_count));
	if(g_currINFOInfo.boot_count == 0xAA)
	{
		/* Check Forced OTC */
		if(g_currINFOInfo.reserved[0] == 0xBB) /* Forced OTC */
		{
		  	/* on firstBoot after Forced SSU, please deliver "bForced is TRUE" to MW */
	    		pstSsuStatus->bForced = true;
	    
	    		/* reset value flash */
	    		g_currINFOInfo.reserved[0] = 0xFF;
		}
		else
		{
			/* on firstBoot after Forced SSU, please deliver "bForced is FALSE" to MW */
			pstSsuStatus->bForced = false;		
		}

		/* boot_count reset */
		g_currINFOInfo.boot_count = 0xFF;

		//save INFO with boot_count
		/* Calculate CRC */
		crcCal	= calc_crc32((uint8_t*)&g_currINFOInfo, FLASH_INFO_SIZE - 4);
		g_currINFOInfo.crc32	= crcCal;

		FlashError = TDAL_FLA_Erase( (uint32_t)FLASH_INFO_BASE_ADDRESS, (uint32_t)FLASH_INFO_SIZE/*FLASH_BLOCK_SIZE*/);
		if(FlashError != eTDAL_FLA_NO_ERROR)
		{
		    mTBOX_TRACE((kTBOX_NIV_CRITICAL, "INFO Erase Failed\n\n"));
			mTBOX_RETURN eTDAL_OTA_STATUS_ERROR;
		}

		NumberWritten = TDAL_FLA_Write((uint32_t)FLASH_INFO_BASE_ADDRESS, (uint8_t*)&g_currINFOInfo, (uint32_t)FLASH_INFO_SIZE/*FLASH_BLOCK_SIZE*/);
	    if(NumberWritten != FLASH_INFO_SIZE/*FLASH_BLOCK_SIZE*/)
	    {
			mTBOX_TRACE((kTBOX_NIV_CRITICAL, "INFO Write Failed\n"));
			mTBOX_RETURN eTDAL_OTA_STATUS_ERROR;
	    }

	    /* on firstBoot after SSU, please deliver "FirstFlag is TRUE" to MW */
	    pstSsuStatus->bFirstBoot = true;

	}
	else {
		/* if not firstboot after SSU, please deliver "FirstFlag is FALSE" to MW */
		pstSsuStatus->bFirstBoot = false;
	}
	/************************************************************************************************************************************************/

	if(g_currINFOInfo.standby_mode == STANDBY_MODE_ACTIVE_STANDBY) {
		pstSsuStatus->bInStandBy 			= true;
	}
	else {
		pstSsuStatus->bInStandBy 			= false;
	}

	if(g_currINFOInfo.download_error_code == DN_ERR_E000) {
		pstSsuStatus->eErrorStatus			= eTDAL_SSU_NO_ERROR;
		strcpy(pstSsuStatus->cStatus,"Success");
	}
	else {
		pstSsuStatus->eErrorStatus			= eTDAL_SSU_ERROR;
		strcpy(pstSsuStatus->cStatus,"Failed");
	}

	pstSsuStatus->uiLastUpdateID			= DecimalToHex(g_currINFOInfo.application_version);
	pstSsuStatus->uiLastUpdateID 			= pstSsuStatus->uiLastUpdateID << 8;

	memcpy((void*)pstSsuStatus->cUtcTime, (const void*)g_currINFOInfo.last_updated_time, UTC_TIME_LENGTH);
	sprintf(pstSsuStatus->cErrorCode, "%04X", g_currINFOInfo.download_error_code);

	mTBOX_PRINT((kTBOX_LF, "===================== Read pstSsuStatus ==================================\n"));
	mTBOX_PRINT((kTBOX_LF, "pstSsuStatus->bInStandBy			= 0x%02X\n", pstSsuStatus->bInStandBy));
	mTBOX_PRINT((kTBOX_LF, "pstSsuStatus->eErrorStatus			= 0x%02X\n", pstSsuStatus->eErrorStatus));
	mTBOX_PRINT((kTBOX_LF, "pstSsuStatus->uiLastUpdateID		= 0x%08X\n", pstSsuStatus->uiLastUpdateID));
	mTBOX_PRINT((kTBOX_LF, "pstSsuStatus->cErrorCode			= %s\n", pstSsuStatus->cErrorCode));
	mTBOX_PRINT((kTBOX_LF, "pstSsuStatus->cUtcTime				= [0x%02X][0x%02X][0x%02X][0x%02X][0x%02X]\n",
			pstSsuStatus->cUtcTime[0], pstSsuStatus->cUtcTime[1], pstSsuStatus->cUtcTime[2], pstSsuStatus->cUtcTime[3], pstSsuStatus->cUtcTime[4]));
	mTBOX_PRINT((kTBOX_LF, "===================================================================\n"));

    mTBOX_RETURN eTDAL_SSU_NO_ERROR;
}

GLOBAL tTDAL_SSU_Error TDAL_SSU_SetParameters(tTDAL_SSU_ControlParameters *pstSsuParams)
{
	uint32_t 	bResult;
	uint8_t		i = 0, INFOReadBuf[FLASH_INFO_SIZE], FDCAReadBuf[FLASH_FDCA_SIZE];
	uint8_t		buff[SW_VER_NUM_MAX_LEN];
	uint32_t	crcCal;
	uint32_t 					NumberRead, NumberWritten;
	tTDAL_FLA_ErrorCode      	FlashError;

    mTBOX_FCT_ENTER("TDAL_SSU_SetParameters");

    mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_SSU_SetParameters IMPLEMENTED BY KAON-LOADER\n"));

	if(pstSsuParams == NULL) {
		mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_SSU_SetParameters Failed\n"));
		mTBOX_RETURN eTDAL_OTA_STATUS_ERROR;
	}

    /*********************************************************************************************
     *
     * pstSsuParams read
     *
     ********************************************************************************************/
    mTBOX_TRACE((kTBOX_NIV_CRITICAL, "bInStandBy [0x%02X]\n", pstSsuParams->bInStandBy));
    mTBOX_TRACE((kTBOX_NIV_CRITICAL, "uiModulation [0x%02X]\n", pstSsuParams->uiModulation));
    mTBOX_TRACE((kTBOX_NIV_CRITICAL, "uiFreqency [0x%08X]\n", pstSsuParams->uiFreqency));
    mTBOX_TRACE((kTBOX_NIV_CRITICAL, "uiPid [0x%04X]\n", pstSsuParams->uiPid));
    mTBOX_TRACE((kTBOX_NIV_CRITICAL, "uiSymbolRate [0x%02X]\n", pstSsuParams->uiSymbolRate));
    mTBOX_TRACE((kTBOX_NIV_CRITICAL, "uiUpdateID [0x%08X]\n", pstSsuParams->uiUpdateID));

    /*********************************************************************************************
     *
     * INFO read
     *
     ********************************************************************************************/
	NumberRead = TDAL_FLA_Read((uint32_t)FLASH_INFO_BASE_ADDRESS, (uint8_t*)INFOReadBuf, (uint32_t)/*FLASH_BLOCK_SIZE*/FLASH_INFO_SIZE);
  	if(NumberRead != FLASH_INFO_SIZE/*FLASH_BLOCK_SIZE*/)
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL, "INFO Read Failed\n"));
		mTBOX_RETURN eTDAL_OTA_STATUS_ERROR;
	}

	memset(&g_currINFOInfo, 0x00, FLASH_INFO_SIZE);
	memcpy(&g_currINFOInfo, INFOReadBuf, FLASH_INFO_SIZE);

	mTBOX_PRINT((kTBOX_LF, "===================== Read INFO ==================================\n"));
	mTBOX_PRINT((kTBOX_LF, "application_version	= %06D\n", g_currINFOInfo.application_version));
	mTBOX_PRINT((kTBOX_LF, "downloader_version	= %06D\n", g_currINFOInfo.downloader_version));
	mTBOX_PRINT((kTBOX_LF, "standby_mode		= %02X\n", g_currINFOInfo.standby_mode));
	mTBOX_PRINT((kTBOX_LF, "ts_entry_curr_num	= %02D\n", g_currINFOInfo.ts_entry_curr_num));
	mTBOX_PRINT((kTBOX_LF, "download_error_code	= %04X\n", g_currINFOInfo.download_error_code));
	mTBOX_PRINT((kTBOX_LF, "download_model_id	= %04D\n", g_currINFOInfo.download_model_id));
	mTBOX_PRINT((kTBOX_LF, "===================================================================\n"));

    /*********************************************************************************************
     *
     * INFO save
     * standby_mode is need to be stored.
     *
     ********************************************************************************************/
	if(pstSsuParams->bInStandBy == 0) {
		g_currINFOInfo.standby_mode = STANDBY_MODE_NORMAL;
	}
	else {
		g_currINFOInfo.standby_mode = STANDBY_MODE_ACTIVE_STANDBY;
	}

	/* Calculate CRC */
	crcCal	= calc_crc32((uint8_t*)&g_currINFOInfo, FLASH_INFO_SIZE - 4);
	g_currINFOInfo.crc32	= crcCal;

	FlashError = TDAL_FLA_Erase( (uint32_t)FLASH_INFO_BASE_ADDRESS, (uint32_t)FLASH_INFO_SIZE/*FLASH_BLOCK_SIZE*/);
	if(FlashError != eTDAL_FLA_NO_ERROR)
	{
	  mTBOX_TRACE((kTBOX_NIV_CRITICAL, "INFO Erase Failed\n\n"));
		mTBOX_RETURN eTDAL_OTA_STATUS_ERROR;
	}

	NumberWritten = TDAL_FLA_Write((uint32_t)FLASH_INFO_BASE_ADDRESS, (uint8_t*)&g_currINFOInfo, (uint32_t)FLASH_INFO_SIZE/*FLASH_BLOCK_SIZE*/);
  	if(NumberWritten != FLASH_INFO_SIZE/*FLASH_BLOCK_SIZE*/)
  	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL, "INFO Write Failed\n"));
		mTBOX_RETURN eTDAL_OTA_STATUS_ERROR;
	}

	/*********************************************************************************************
	 *
	 * FDCA read
	 *
	 ********************************************************************************************/
	NumberRead = TDAL_FLA_Read((uint32_t)FLASH_FDCA_BASE_ADDRESS, (uint8_t*)FDCAReadBuf, (uint32_t)FLASH_FDCA_SIZE/*FLASH_BLOCK_SIZE*/);
  	if(NumberRead != FLASH_FDCA_SIZE/*FLASH_BLOCK_SIZE*/)
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL, "FDCA Read Failed\n"));
		mTBOX_RETURN eTDAL_OTA_STATUS_ERROR;
	}

	memset(&g_currFDCAInfo, 0x00, FLASH_FDCA_SIZE);
	memcpy(&g_currFDCAInfo, FDCAReadBuf, FLASH_FDCA_SIZE);

	mTBOX_PRINT((kTBOX_LF, "===================== Read FDCA ==================================\n"));
	mTBOX_PRINT((kTBOX_LF, "update_request		= %02X\n", g_currFDCAInfo.update_request));
	mTBOX_PRINT((kTBOX_LF, "update_method		= %02X\n", g_currFDCAInfo.update_method));
	mTBOX_PRINT((kTBOX_LF, "update_media		= %02X\n", g_currFDCAInfo.update_media));

#if 0
	for(i=0; i<g_currFDCAInfo.tuning_param_cnt; i++)
	{
		mTBOX_PRINT((kTBOX_LF, "FREQ[%d]		= %D\n", i, g_currFDCAInfo.tuning_param[i].freq_khz));
	}

	for(i=0; i<g_currFDCAInfo.tuning_param_cnt; i++)
	{
		mTBOX_PRINT((kTBOX_LF, "SYM[%D]			= %D\n", i, g_currFDCAInfo.tuning_param[i].symbol_ksym));
	}

	for(i=0; i<g_currFDCAInfo.tuning_param_cnt; i++)
	{
		mTBOX_PRINT((kTBOX_LF, "MOD[%D]			= %02X\n", i, g_currFDCAInfo.tuning_param[i].modulation));
	}
#endif

	mTBOX_PRINT((kTBOX_LF, "===================================================================\n"));

	/*********************************************************************************************
	 *
	 * FDCA save
	 * followings are need to be stored.
	 *
	 * update_request
	 * update_method
	 * update_media
	 *
	 ********************************************************************************************/
	g_currFDCAInfo.update_request 			= UPDATE_REQUEST_REQUESTED;
	g_currFDCAInfo.update_method 			= UPDATE_METHOD_NORMAL;
	g_currFDCAInfo.update_media 			= UPDATE_MEDIA_OTA;
	g_currFDCAInfo.target_software_version 	= SOFTWARE_VERSION_HIGHER;

	for(i=0; i<g_currFDCAInfo.tuning_param_cnt; i++)
	{
		g_currFDCAInfo.tuning_param[i].data_pid		= 4000;
		mTBOX_PRINT((kTBOX_LF, "[%d]DATA_PID			= %d\n", i, g_currFDCAInfo.tuning_param[i].data_pid));
	}
	/* Calculate CRC */
	crcCal	= calc_crc32((uint8_t*)&g_currFDCAInfo, FLASH_FDCA_SIZE - 4);
	g_currFDCAInfo.crc32	= crcCal;

	mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_SSU_SetParameters. begin to write\n"));

	FlashError = TDAL_FLA_Erase( (uint32_t)FLASH_FDCA_BASE_ADDRESS, (uint32_t)FLASH_FDCA_SIZE/*FLASH_BLOCK_SIZE*/);
	if(FlashError != eTDAL_FLA_NO_ERROR)
	{
	  mTBOX_TRACE((kTBOX_NIV_CRITICAL, "FDCA Erase Failed\n\n"));
		mTBOX_RETURN eTDAL_OTA_STATUS_ERROR;
	}

	NumberWritten = TDAL_FLA_Write((uint32_t)FLASH_FDCA_BASE_ADDRESS, (uint8_t*)&g_currFDCAInfo, (uint32_t)FLASH_FDCA_SIZE/*FLASH_BLOCK_SIZE*/);
  	if(NumberWritten != FLASH_FDCA_SIZE/*FLASH_BLOCK_SIZE*/)
  	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL, "FDCA Write Failed\n"));
		mTBOX_RETURN eTDAL_OTA_STATUS_ERROR;
	}

	mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_SSU_SetParameters. writing done.\n"));

    mTBOX_RETURN eTDAL_OTA_STATUS_NO_ERROR;
}

GLOBAL tTDAL_SSU_Error TDAL_SSU_Initiate()
{
    mTBOX_FCT_ENTER("TDAL_SSU_Initiate");
    mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_SSU_GetStatus NOT IMPLEMENTED\n"));
    mTBOX_RETURN eTDAL_SSU_NO_ERROR;
}

