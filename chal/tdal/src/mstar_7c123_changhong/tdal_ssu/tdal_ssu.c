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
#include "tdal_ssu_priv.h"

/********************************************************
 *        Defines                                       *
 ********************************************************/

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

GLOBAL tTDAL_SSU_Error TDAL_SSU_GetPlatformInfo(tTDAL_SSU_PlatformInfo *pstSsuInfo)
{
    mTBOX_FCT_ENTER("TDAL_SSU_GetPlatformInfo");

    unsigned int stbSerialNumber = 0;
    unsigned char SerialNumber[4] = {0};
    int ret = 0;
    CH_LoaderInfo_t 	rpstru_LoaderInfo={0};
    tTDAL_OTA_ErrorCode	enm_ChComRet;
    CH_SystemInfo_t 	gstru_SystemInfo={0};

    enm_ChComRet = CH_COM_GetLoaderInfo(&rpstru_LoaderInfo);
    enm_ChComRet |= CH_COM_GetSystemInfo( &gstru_SystemInfo );
    if(enm_ChComRet != eTDAL_OTA_STATUS_NO_ERROR)
    {
    	U8 TempBuf[20] = {"0"};

    	memset(&gstru_SystemInfo,0,sizeof(gstru_SystemInfo));

    	strcpy((char*)gstru_SystemInfo.uc_name,"System Info");
    	gstru_SystemInfo.i_length = sizeof(gstru_SystemInfo) - 16;


    	memcpy(gstru_SystemInfo.uc_STBSN,TempBuf,15 );
    	printf("\n ================CH_COM_GetSystemInfo fail\n");

    	//CH_COM_SetSystemInfo(&gstru_SystemInfo);
    }
    gstru_SystemInfo.uc_STBSN[16] = 0;

    memset(SerialNumber,0,sizeof(SerialNumber));

    pstSsuInfo->uiOUI = HARDWARE_MANUFACTURER_ID;

    strcpy(pstSsuInfo->cHardwareModel, (unsigned char*)HARDWARE_MODEL);
    pstSsuInfo->uiHardwareModel = HARDWARE_MODEL_ID;

    strcpy(pstSsuInfo->cHardwareVersion, (unsigned char*)HARDWARE_VERSION);
    pstSsuInfo->uiHardwareVersion = HARDWARE_VERSION_ID;

    strcpy(pstSsuInfo->cDeviceBrandName, (unsigned char*)HARDWARE_DEVICE_NAME);
    strcpy(pstSsuInfo->cHardwareDriverVersion, (unsigned char*)CHAL_VERSION);

    mTBOX_RETURN eTDAL_SSU_NO_ERROR;
}

GLOBAL tTDAL_SSU_Error TDAL_SSU_GetStatusInfo(tTDAL_SSU_StatusInfo *pstSsuStatus)
{
    mTBOX_FCT_ENTER("TDAL_SSU_GetStatus");
    mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_SSU_GetStatus NOT IMPLEMENTED\n"));

    pstSsuStatus->bFirstBoot     = FALSE;
    pstSsuStatus->bForced        = FALSE;
    pstSsuStatus->bInStandBy     = FALSE;

    pstSsuStatus->eErrorStatus   = eTDAL_SSU_NO_ERROR;

    //pstSsuStatus->uiLastUpdateID = stNewlandDialogArea.u32CurSoftware;

    strcpy(pstSsuStatus->cErrorCode, "E000");

    strcpy(pstSsuStatus->cStatus, "Success");

    memset(pstSsuStatus->cUtcTime, 0x0, 5);

    mTBOX_RETURN eTDAL_SSU_NO_ERROR;
}

GLOBAL tTDAL_SSU_Error TDAL_SSU_SetParameters(tTDAL_SSU_ControlParameters *pstSsuParams)
{
    mTBOX_FCT_ENTER("TDAL_SSU_SetParameters");

    tTDAL_OTA_ErrorCode	enm_ChComRet;
    CH_UpdateInfo_t		gsstru_OTAUpdateInfo;
    CH_LoaderInfo_t 	rpstru_LoaderInfo ={0};

    enm_ChComRet = CH_COM_GetLoaderInfo(&rpstru_LoaderInfo);
    if(eTDAL_OTA_STATUS_NO_ERROR != enm_ChComRet)
    {
    	printf("\n CH_COM_GetLoaderInfo fail\n");
    }
    memset(&gsstru_OTAUpdateInfo,0,sizeof(CH_UpdateInfo_t));

    memcpy(gsstru_OTAUpdateInfo.uc_name,"Update Info",strlen("Update Info") );
    gsstru_OTAUpdateInfo.enm_AntType			= CH_DEMOD_DVB_C;
    gsstru_OTAUpdateInfo.uc_UpdaeFlag			= 0x47;

    gsstru_OTAUpdateInfo.un_FreqInfo.stru_CableFreqInfo.ui_FreqKHZ	= pstSsuParams->uiFreqency;
    gsstru_OTAUpdateInfo.un_FreqInfo.stru_CableFreqInfo.ui_SymbKbps	= pstSsuParams->uiSymbolRate;
    gsstru_OTAUpdateInfo.un_FreqInfo.stru_CableFreqInfo.uc_QAMMode	= pstSsuParams->uiModulation -  CHMID_TUNER_QAM_QAM8;

    gsstru_OTAUpdateInfo.us_DataPid			= pstSsuParams->uiPid;
    gsstru_OTAUpdateInfo.us_DataGid			= 0x2;
    rpstru_LoaderInfo.ui_TimeCode			= !pstSsuParams->bInStandBy;
    enm_ChComRet = CH_COM_SetLoaderInfo(&rpstru_LoaderInfo);
    enm_ChComRet |= CH_COM_SetOTAInfo(&gsstru_OTAUpdateInfo);

    mTBOX_RETURN eTDAL_SSU_NO_ERROR;
}

GLOBAL tTDAL_SSU_Error TDAL_SSU_Initiate()
{
    mTBOX_FCT_ENTER("TDAL_SSU_Initiate");
    mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_SSU_Initiate NOT IMPLEMENTED\n"));
    mTBOX_RETURN eTDAL_SSU_NO_ERROR;
}


/**===============================The below is for CH OTA PART===========================================================================================*/

unsigned long ulTable_MPEG32[256] =
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

const CH_COM_Params_t gstru_fact_SaveParams[CH_COM_INFOTYPE_MAX] =
{
	/*0:CH_COM_INFOTYPE_LOADER*/
	{
		CH_COM_INFOTYPE_LOADER,
		true,
		{
			CH_COM_SAVEDEVICE_FLASH,
			0,
			NULL,
			0X101000,
			0X80,
		},
		true,
		{
			CH_COM_SAVEDEVICE_FLASH,
			0,
			NULL,
			0X781000,
			0X80,
		}
	},
	/*2:CH_COM_INFOTYPE_UPDATE*/
	{
		CH_COM_INFOTYPE_UPDATE,
		true,
		{
			CH_COM_SAVEDEVICE_FLASH,
			0,
			NULL,
			0X108000,
			0X100,
		},
		true,
		{
			CH_COM_SAVEDEVICE_FLASH,
			0,
			NULL,
			0X788000,
			0X100,
		},

	},
	/*3:CH_COM_INFOTYPE_SYSTEM*/
	{
		CH_COM_INFOTYPE_SYSTEM,
		true,
		{
			CH_COM_SAVEDEVICE_FLASH,
			0,
			NULL,
			0X10C000,
			0X600,
		},
		true,
		{
			CH_COM_SAVEDEVICE_FLASH,
			0,
			NULL,
			0X78C000,
			0X600,
		},

	},
	/*4:CH_COM_INFOTYPE_FACTBOOT*/
	{
		CH_COM_INFOTYPE_FACTBOOT,
		true,
		{
			CH_COM_SAVEDEVICE_FLASH,
			0,
			NULL,
			0X104000,
			0X80,
		},
		true,
		{
			CH_COM_SAVEDEVICE_FLASH,
			0,
			NULL,
			0X784000,
			0X80,
		},
	}

};

uint32_t   CHDRV_FLASH_DirectWrite(   uint32_t   Address   ,
		uint8_t*   Buffer   ,
		uint32_t   NumberToWrite   )
{
	int enm_FlashResult = 0;

	enm_FlashResult = TDAL_FLA_Write_OTA(Address ,Buffer,NumberToWrite);
	return enm_FlashResult;
}

U32 CRC_MPEG32_(U8 * pvStartAddress, U32 ulSize_in_bytes)
{
	unsigned long ulCRC;
	U8 * pbData;
	U32 ulSize_in_bytes_;

	/**********************
	* Lookup Table method *
	**********************/
	ulSize_in_bytes_  =    ulSize_in_bytes;

	/* init the start value */
	ulCRC = 0xFFFFFFFF;

	pbData = pvStartAddress;

	/* calculate CRC */
	while (ulSize_in_bytes_--)
	{
		ulCRC = ulTable_MPEG32[((ulCRC>>24) ^ *pbData++) & 0xFFL] ^ (ulCRC << 8);
	}
	return ((U32)ulCRC);

}  /* CRC_MPEG32 */

tTDAL_OTA_ErrorCode CH_COM_SetOTAInfo(CH_UpdateInfo_t *rpstru_OTAInfo)
{
		tTDAL_OTA_ErrorCode enm_Result = eTDAL_OTA_STATUS_NO_ERROR;
#if 1 	/*<!-- gongwenqing 2013/12/13 13:59:46 --!>*/
	int enm_FlashResult = 0;
	S32 i_WriteSize = 0;
	S32 i_ActualWriteSize = 0;
	S32 i_ActualBackupWriteSize = 0;
	U32 ui_Crc = 0;

	if( rpstru_OTAInfo == NULL )
	{
		return eTDAL_OTA_STATUS_ERROR;
	}
	if( strcmp(rpstru_OTAInfo->uc_name,UPDATE_INFO_MAGIC) )
	{
		return eTDAL_OTA_STATUS_ERROR;
	}


		printf("\nset CH_COM_SetOTAInfo 1111\n");

		i_WriteSize = sizeof(CH_UpdateInfo_t);
		i_ActualWriteSize = i_WriteSize;
		i_ActualBackupWriteSize = i_WriteSize;

		rpstru_OTAInfo->i_length = sizeof(CH_UpdateInfo_t) -16;
		ui_Crc = CRC_MPEG32_((U8 *)&(rpstru_OTAInfo->enm_AntType), rpstru_OTAInfo->i_length -4);
		rpstru_OTAInfo->ui_crc = ui_Crc;

		if( gstru_fact_SaveParams[CH_COM_INFOTYPE_UPDATE].stru_MainInfo.enm_Device == CH_COM_SAVEDEVICE_FLASH )
		{
			printf("##############%s: %d: ui_OffsetAddress = 0x%06X\n", __FUNCTION__, __LINE__, gstru_fact_SaveParams[CH_COM_INFOTYPE_UPDATE].stru_MainInfo.ui_OffsetAddress);
			enm_FlashResult = TDAL_FLA_Write_OTA( gstru_fact_SaveParams[CH_COM_INFOTYPE_UPDATE].stru_MainInfo.ui_OffsetAddress,(U8 *)rpstru_OTAInfo,i_ActualWriteSize);
			if( enm_FlashResult == 0 )
			{
				mTBOX_TRACE((kTBOX_NIV_CRITICAL,"CH_COM_SetOTAInfo-->08-->Result[%d]\r\n",enm_FlashResult));
				enm_Result = eTDAL_OTA_STATUS_ERROR;
				goto FAIL0;
			}

			if( gstru_fact_SaveParams[CH_COM_INFOTYPE_UPDATE].stru_BackUpInfo.ui_OffsetAddress != -1 )
			{
			    printf("##############%s: %d: ui_OffsetAddress = 0x%06X\n", __FUNCTION__, __LINE__, gstru_fact_SaveParams[CH_COM_INFOTYPE_UPDATE].stru_BackUpInfo.ui_OffsetAddress);
				enm_FlashResult = TDAL_FLA_Write_OTA(gstru_fact_SaveParams[CH_COM_INFOTYPE_UPDATE].stru_BackUpInfo.ui_OffsetAddress ,(U8 *)rpstru_OTAInfo,i_ActualBackupWriteSize);
				if( enm_FlashResult == 0 )
				{
					mTBOX_TRACE((kTBOX_NIV_CRITICAL,"CH_COM_SetOTAInfo-->09-->Result[%d]\r\n",enm_FlashResult));
					enm_Result = eTDAL_OTA_STATUS_ERROR;
					goto FAIL0;
				}
			}
		}
		else
		{
			mTBOX_TRACE((kTBOX_NIV_CRITICAL,"CH_COM_SetOTAInfo-->12-->XXXX\r\n"));
			enm_Result = eTDAL_OTA_STATUS_ERROR;
			goto FAIL0;
		}
		if( gstru_fact_SaveParams[CH_COM_INFOTYPE_UPDATE].stru_BackUpInfo.ui_OffsetAddress != -1 )
		{
			if( i_ActualBackupWriteSize != i_ActualWriteSize )
			{
				mTBOX_TRACE((kTBOX_NIV_CRITICAL,"CH_COM_SetOTAInfo-->13-->XXXX\r\n"));
				enm_Result = eTDAL_OTA_STATUS_ERROR;
				goto FAIL0;
			}
		}
		if( i_ActualWriteSize != i_WriteSize )
		{
			mTBOX_TRACE((kTBOX_NIV_CRITICAL,"CH_COM_SetOTAInfo-->14-->i_WriteSize=%d,i_ActualWriteSize=%d\r\n",i_WriteSize,i_ActualWriteSize));
			enm_Result = eTDAL_OTA_STATUS_ERROR;
		}
#endif 	/*<!-- gongwenqing 2013/12/13 13:59:46 --!>*/
	FAIL0:
	return enm_Result;
}



tTDAL_OTA_ErrorCode CH_COM_SetSystemInfo(CH_SystemInfo_t *rpstru_SystemInfo)
{
	tTDAL_OTA_ErrorCode enm_Result = eTDAL_OTA_STATUS_NO_ERROR;
#if 1	/*<!-- gongwenqing 2013/12/13 14:01:01 --!>*/
	int enm_FlashResult = 0;
	S32 i_WriteSize = 0;
	S32 i_ActualWriteSize = 0;
	S32 i_ActualBackupWriteSize = 0;
	U32 ui_Crc = 0;

	/*判断是否已经初始化*/

	if( rpstru_SystemInfo == NULL )
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL,"CH_COM_SetSystemInfo-->02-->rpstru_SystemInfo[0x%x]\r\n",rpstru_SystemInfo));
		return eTDAL_OTA_STATUS_ERROR;
	}

	if( strcmp(rpstru_SystemInfo->uc_name,SYSTEM_INFO_MAGIC) )
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL,"CH_COM_SetSystemInfo-->03-->uc_name Error\r\n"));
		return eTDAL_OTA_STATUS_ERROR;
	}

	i_WriteSize = sizeof(CH_SystemInfo_t);
	i_ActualWriteSize = i_WriteSize;
	i_ActualBackupWriteSize = i_WriteSize;

	rpstru_SystemInfo->i_length = sizeof(CH_SystemInfo_t) -16;
	ui_Crc = CRC_MPEG32_((U8 *)&(rpstru_SystemInfo->uc_STBSN[0]), rpstru_SystemInfo->i_length -4);
	rpstru_SystemInfo->ui_CRC = ui_Crc;

	if( gstru_fact_SaveParams[CH_COM_INFOTYPE_SYSTEM].stru_MainInfo.enm_Device == CH_COM_SAVEDEVICE_FLASH )
	{
		enm_FlashResult = CHDRV_FLASH_DirectWrite(gstru_fact_SaveParams[CH_COM_INFOTYPE_SYSTEM].stru_MainInfo.ui_OffsetAddress,(U8 *)rpstru_SystemInfo,i_ActualWriteSize);
		if( enm_FlashResult == 0 )
		{
			mTBOX_TRACE((kTBOX_NIV_CRITICAL,"CH_COM_SetSystemInfo-->09-->Result[%d]\r\n",enm_FlashResult));
			enm_Result = eTDAL_OTA_STATUS_ERROR;
			goto FAIL0;
		}

		if( gstru_fact_SaveParams[CH_COM_INFOTYPE_SYSTEM].stru_BackUpInfo.ui_OffsetAddress != -1 )
		{
			enm_FlashResult = CHDRV_FLASH_DirectWrite(gstru_fact_SaveParams[CH_COM_INFOTYPE_SYSTEM].stru_BackUpInfo.ui_OffsetAddress,(U8 *)rpstru_SystemInfo,i_ActualBackupWriteSize);
			if( enm_FlashResult == 0 )
			{
				mTBOX_TRACE((kTBOX_NIV_CRITICAL,"CH_COM_SetSystemInfo-->10-->Result[%d]\r\n",enm_FlashResult));
				enm_Result = eTDAL_OTA_STATUS_ERROR;
				goto FAIL0;
			}
		}
	}
	else
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL,"CH_COM_SetSystemInfo-->13-->XXXX\r\n"));
		enm_Result = eTDAL_OTA_STATUS_ERROR;
		goto FAIL0;
	}

	if( i_ActualBackupWriteSize != i_ActualWriteSize )
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL,"CH_COM_SetSystemInfo-->14-->i_ActualWriteSize[%d],i_ActualBackupWriteSize[%d]\r\n",
			i_ActualWriteSize,i_ActualBackupWriteSize));
		enm_Result = eTDAL_OTA_STATUS_ERROR;
		goto FAIL0;
	}

	if( i_ActualWriteSize != i_WriteSize )
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL,"CH_COM_SetSystemInfo-->15-->XXXX\r\n"));
		enm_Result = eTDAL_OTA_STATUS_ERROR;
		goto FAIL0;
	}
FAIL0:

#endif 	/*<!-- gongwenqing 2013/12/13 14:01:01 --!>*/

	return enm_Result;
}

tTDAL_OTA_ErrorCode CH_COM_GetSystemInfo(CH_SystemInfo_t *rpstru_SystemInfo)
{
	tTDAL_OTA_ErrorCode enm_Result = eTDAL_OTA_STATUS_NO_ERROR;
#if 1 	/*<!-- gongwenqing 2013/12/13 14:00:51 --!>*/
	int enm_FlashResult = 0;
	CH_SystemInfo_t stru_BackupSystemInfo;
	S32 i_ReadSize = 0;
	S32 i_ActualReadSize = 0;
	S32 i_ActualBackupReadSize = 0;
	U32 ui_CrcValue = 0;
	bool b_UseBackup = false;
	bool b_MainSaveOK = false;
	bool b_BackSaveOK = false;

	/*判断是否已经初始化*/
	if( rpstru_SystemInfo == NULL )
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL,"CH_COM_GetSystemInfo-->02-->rpstru_SystemInfo[0x%x]\r\n",rpstru_SystemInfo));
		return eTDAL_OTA_STATUS_ERROR;
	}

	memset(&stru_BackupSystemInfo,0,sizeof(CH_SystemInfo_t));



	memset(&stru_BackupSystemInfo,0,sizeof(CH_SystemInfo_t));

	i_ReadSize = sizeof(CH_SystemInfo_t);
	i_ActualReadSize = i_ReadSize;
	i_ActualBackupReadSize = i_ReadSize;

	if( gstru_fact_SaveParams[CH_COM_INFOTYPE_SYSTEM].stru_MainInfo.enm_Device == CH_COM_SAVEDEVICE_FLASH )
	{
		enm_FlashResult = TDAL_FLA_Read_OTA(gstru_fact_SaveParams[CH_COM_INFOTYPE_SYSTEM].stru_MainInfo.ui_OffsetAddress,(U8 *)rpstru_SystemInfo,i_ActualReadSize);
		if( enm_FlashResult == 0 )
		{
			mTBOX_TRACE((kTBOX_NIV_CRITICAL,"CH_COM_GetSystemInfo-->08-->Result[%d]\r\n",enm_FlashResult));
			enm_Result = eTDAL_OTA_STATUS_ERROR;
			goto FAIL0;
		}

		if( gstru_fact_SaveParams[CH_COM_INFOTYPE_SYSTEM].stru_BackUpInfo.ui_OffsetAddress != -1 )
		{
			enm_FlashResult = TDAL_FLA_Read_OTA(gstru_fact_SaveParams[CH_COM_INFOTYPE_SYSTEM].stru_BackUpInfo.ui_OffsetAddress,(U8 *)&stru_BackupSystemInfo,i_ActualBackupReadSize);
			if( enm_FlashResult == 0 )
			{
				mTBOX_TRACE((kTBOX_NIV_CRITICAL,"CH_COM_GetSystemInfo-->09-->Result[%d]\r\n",enm_FlashResult));
				enm_Result = eTDAL_OTA_STATUS_ERROR;
				goto FAIL0;
			}
			b_UseBackup = true;
		}
	}
	else
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL,"CH_COM_GetSystemInfo-->12-->XXXX\r\n"));
		enm_Result = eTDAL_OTA_STATUS_ERROR;
		goto FAIL0;
	}

	/*判断读出来的数据是否有长度信息*/
	if( rpstru_SystemInfo->i_length < 4 ||rpstru_SystemInfo->i_length > ( i_ActualReadSize -16 ) )
	{

		mTBOX_TRACE((kTBOX_NIV_CRITICAL,"CH_COM_GetSystemInfo-->3-->i_length[%d],i_ActualReadSize[%d]\r\n",
			rpstru_SystemInfo->i_length,i_ActualReadSize));
		b_MainSaveOK = false;/*读出来的数据有问题*/
	}
	else
	{
		/*校验主数据*/
		ui_CrcValue = CRC_MPEG32_((U8 *)&(rpstru_SystemInfo->uc_STBSN[0]), rpstru_SystemInfo->i_length -4);
		if(ui_CrcValue == rpstru_SystemInfo->ui_CRC)
		{
			if(strcmp(rpstru_SystemInfo->uc_name,SYSTEM_INFO_MAGIC) == 0)
			{
				b_MainSaveOK = true;
			}
		}

	}
	if( b_UseBackup == true )
	{
		if(stru_BackupSystemInfo.i_length < 4 ||stru_BackupSystemInfo.i_length > ( i_ActualBackupReadSize -16 ) )
		{
			b_BackSaveOK = false;  /*读出来的数据有问题*/
		}
		else
		{
			/*校验备份数据*/
			ui_CrcValue = CRC_MPEG32_((U8 *)&(stru_BackupSystemInfo.uc_STBSN[0]), stru_BackupSystemInfo.i_length - 4);
			if(ui_CrcValue == stru_BackupSystemInfo.ui_CRC)
			{
				if(strcmp(stru_BackupSystemInfo.uc_name,SYSTEM_INFO_MAGIC) == 0)
				{
					b_BackSaveOK = true;
				}
			}
		}
		/*如果数据都出错，则返回错误*/
		if(!(b_BackSaveOK || b_MainSaveOK))
		{
			mTBOX_TRACE((kTBOX_NIV_CRITICAL,"CH_COM_GetSystemInfo-->13-->b_BackSaveOK[%d],b_MainSaveOK[%d]\r\n",
				b_BackSaveOK,b_MainSaveOK));
			enm_Result = eTDAL_OTA_STATUS_ERROR;
			goto FAIL0;
		}

		/*如果备份出错或两个都对，但不一致，则修复备份*/
		if((b_BackSaveOK== false) ||
			(memcmp((U8*)rpstru_SystemInfo, (U8*)&stru_BackupSystemInfo, sizeof(CH_SystemInfo_t)) && b_MainSaveOK))
		{
			if( gstru_fact_SaveParams[CH_COM_INFOTYPE_SYSTEM].stru_BackUpInfo.enm_Device == CH_COM_SAVEDEVICE_FLASH)
			{
				enm_FlashResult = CHDRV_FLASH_DirectWrite(gstru_fact_SaveParams[CH_COM_INFOTYPE_SYSTEM].stru_BackUpInfo.ui_OffsetAddress,
														(U8*)rpstru_SystemInfo,i_ActualReadSize);
				if( enm_FlashResult == 0 )
				{
					mTBOX_TRACE((kTBOX_NIV_CRITICAL,"CH_COM_GetSystemInfo-->14-->Result[%d]\r\n",enm_FlashResult));
					enm_Result = eTDAL_OTA_STATUS_ERROR;
					goto FAIL0;
				}
			}
		}

		/*主数据出错，使用备份数据，并修复主数据*/
		if(b_MainSaveOK == false)
		{
			memcpy((U8*)rpstru_SystemInfo, (U8*)&stru_BackupSystemInfo, sizeof(CH_SystemInfo_t));
			if( gstru_fact_SaveParams[CH_COM_INFOTYPE_SYSTEM].stru_MainInfo.enm_Device == CH_COM_SAVEDEVICE_FLASH)
			{
				enm_FlashResult = CHDRV_FLASH_DirectWrite(gstru_fact_SaveParams[CH_COM_INFOTYPE_SYSTEM].stru_MainInfo.ui_OffsetAddress,
														(U8*)rpstru_SystemInfo,i_ActualBackupReadSize);
				if( enm_FlashResult == 0 )
				{
					mTBOX_TRACE((kTBOX_NIV_CRITICAL,"CH_COM_GetSystemInfo-->16-->Result[%d]\r\n",enm_FlashResult));
					enm_Result = eTDAL_OTA_STATUS_ERROR;
					goto FAIL0;
				}
			}
		}
	}
	else
	{
		if(b_MainSaveOK == false)
		{
			mTBOX_TRACE((kTBOX_NIV_CRITICAL,"CH_COM_GetSystemInfo-->18-->XXXX\r\n"));
			enm_Result = eTDAL_OTA_STATUS_ERROR;
			goto FAIL0;
		}
	}

FAIL0:


#endif 	/*<!-- gongwenqing 2013/12/13 14:00:51 --!>*/

	return enm_Result;
}

/*******************************************************************************
*函数名　　　　:  CH_COM_SetLoaderInfo
*函数功能　　　:  存储LOADER信息
*输入参数　　　:  无
*输出参数　　　:
*		*rpstru_LoaderInfo:LOADER信息
*返回值说明　　:
*　　	CH_COM_NOT_INIT:未初始化
*	    	eTDAL_OTA_STATUS_ERROR:非法参数
*		eTDAL_OTA_STATUS_ERROR:操作失败
*　　	eTDAL_OTA_STATUS_NO_ERROR:操作成功
*全局变量和结构:
*算法说明　　　:  无
*调用注意事项　:  无
*******************************************************************************/
tTDAL_OTA_ErrorCode CH_COM_SetLoaderInfo(CH_LoaderInfo_t *rpstru_LoaderInfo)
{
	tTDAL_OTA_ErrorCode enm_Result = eTDAL_OTA_STATUS_NO_ERROR;
#if 1 	/*<!-- gongwenqing 2013/12/13 13:59:11 --!>*/
	int enm_FlashResult = 0;
	S32 i_WriteSize = 0;
	S32 i_ActualWriteSize = 0;
	S32 i_ActualBackupWriteSize = 0;
	U32 ui_Crc = 0;

	if( rpstru_LoaderInfo == NULL )
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL,"CH_COM_SetLoaderInfo -->02-->rpstru_LoaderInfo=0x%x\n",rpstru_LoaderInfo));
		return eTDAL_OTA_STATUS_ERROR;
	}
	if( strcmp(rpstru_LoaderInfo->uc_name,LOADER_INFO_MAGIC) )
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL,"CH_COM_SetLoaderInfo -->03-->uc_name Error\n"));
		return eTDAL_OTA_STATUS_ERROR;
	}




	i_WriteSize = sizeof(CH_LoaderInfo_t);
	i_ActualWriteSize = i_WriteSize;
	i_ActualBackupWriteSize = i_WriteSize;

	rpstru_LoaderInfo->i_length = sizeof(CH_LoaderInfo_t) -16;
	ui_Crc = CRC_MPEG32_((U8 *)&(rpstru_LoaderInfo->ui_OperatorNumber), rpstru_LoaderInfo->i_length - 4);
	rpstru_LoaderInfo->ui_crc = ui_Crc;
	if( gstru_fact_SaveParams[CH_COM_INFOTYPE_LOADER].stru_MainInfo.enm_Device == CH_COM_SAVEDEVICE_FLASH )
	{
		enm_FlashResult = CHDRV_FLASH_DirectWrite(gstru_fact_SaveParams[CH_COM_INFOTYPE_LOADER].stru_MainInfo.ui_OffsetAddress,(U8 *)rpstru_LoaderInfo,i_ActualWriteSize);
		if( enm_FlashResult == 0 )
		{
			mTBOX_TRACE((kTBOX_NIV_CRITICAL,"CH_COM_SetLoaderInfo-->09-->Result[%d]\r\n",enm_FlashResult));
			enm_Result = eTDAL_OTA_STATUS_ERROR;
			goto FAIL0;
		}

		if( gstru_fact_SaveParams[CH_COM_INFOTYPE_LOADER].stru_BackUpInfo.ui_OffsetAddress != -1)
		{
			enm_FlashResult = CHDRV_FLASH_DirectWrite(gstru_fact_SaveParams[CH_COM_INFOTYPE_LOADER].stru_BackUpInfo.ui_OffsetAddress,(U8 *)rpstru_LoaderInfo,i_ActualBackupWriteSize);
			if( enm_FlashResult == 0 )
			{
				mTBOX_TRACE((kTBOX_NIV_CRITICAL,"CH_COM_SetLoaderInfo-->10-->Result[%d]\r\n",enm_FlashResult));
				enm_Result = eTDAL_OTA_STATUS_ERROR;
				goto FAIL0;
			}
		}
	}
	else
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL,"CH_COM_SetLoaderInfo-->13-->XXXX\r\n"));
		enm_Result = eTDAL_OTA_STATUS_ERROR;
		goto FAIL0;
	}


	if( i_ActualWriteSize != i_WriteSize )
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL,"CH_COM_SetLoaderInfo-->14-->i_ActualWriteSize=%d,i_WriteSize=%d\r\n",i_ActualWriteSize,i_WriteSize));
		enm_Result = eTDAL_OTA_STATUS_ERROR;
		goto FAIL0;
	}
	if( (gstru_fact_SaveParams[CH_COM_INFOTYPE_LOADER].stru_BackUpInfo.ui_OffsetAddress != -1)&& i_ActualBackupWriteSize != i_WriteSize)
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL,"CH_COM_SetLoaderInfo-->15-->XXXX[%d]-[%d]\r\n",i_ActualBackupWriteSize,i_WriteSize));
		enm_Result = eTDAL_OTA_STATUS_ERROR;
		goto FAIL0;
	}
FAIL0:


#endif 	/*<!-- gongwenqing 2013/12/13 13:59:11 --!>*/

	return enm_Result;
}

tTDAL_OTA_ErrorCode CH_COM_GetLoaderInfo(CH_LoaderInfo_t *rpstru_LoaderInfo)
{
		tTDAL_OTA_ErrorCode enm_Result = eTDAL_OTA_STATUS_NO_ERROR;

		int enm_FlashResult = eTDAL_OTA_STATUS_NO_ERROR;
#if 1 	/*<!-- gongwenqing 2013/12/13 13:59:24 --!>*/
	S32 i_ReadSize = 0;
	S32 i_ActualReadSize = 0;
	S32 i_ActualBackupReadSize = 0;
	U32 ui_CrcValue = 0;
	CH_LoaderInfo_t stru_BackupLoaderInfo;
	bool b_UseBackup = false;
	bool b_MainSaveOK = false;
	bool b_BackSaveOK = false;


	if( rpstru_LoaderInfo == NULL )
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL,"CH_COM_GetLoaderInfo -->02-->rpstru_LoaderInfo=0x%x\n",rpstru_LoaderInfo));
		return eTDAL_OTA_STATUS_ERROR;
	}

	memset(&stru_BackupLoaderInfo,0,sizeof(CH_LoaderInfo_t));

	if( gstru_fact_SaveParams[CH_COM_INFOTYPE_LOADER].b_Used == true )
	{



		i_ReadSize = sizeof(CH_LoaderInfo_t);
		i_ActualReadSize = i_ReadSize;
		i_ActualBackupReadSize = i_ReadSize;

		if( gstru_fact_SaveParams[CH_COM_INFOTYPE_LOADER].stru_MainInfo.enm_Device == CH_COM_SAVEDEVICE_FLASH )
		{
			enm_FlashResult = TDAL_FLA_Read_OTA(gstru_fact_SaveParams[CH_COM_INFOTYPE_LOADER].stru_MainInfo.ui_OffsetAddress,(U8 *)rpstru_LoaderInfo,i_ActualReadSize);
			if( enm_FlashResult == 0 )
			{
				mTBOX_TRACE((kTBOX_NIV_CRITICAL,"CH_COM_GetLoaderInfo-->08-->Result[%d]\r\n",enm_FlashResult));
				enm_Result = eTDAL_OTA_STATUS_ERROR;
				goto FAIL0;
			}
			if( gstru_fact_SaveParams[CH_COM_INFOTYPE_LOADER].stru_BackUpInfo.ui_OffsetAddress != -1 )
			{
				enm_FlashResult = TDAL_FLA_Read_OTA(gstru_fact_SaveParams[CH_COM_INFOTYPE_LOADER].stru_BackUpInfo.ui_OffsetAddress,(U8 *)&stru_BackupLoaderInfo,i_ActualBackupReadSize);
				if( enm_FlashResult == 0 )
				{
					mTBOX_TRACE((kTBOX_NIV_CRITICAL,"CH_COM_GetLoaderInfo-->09-->Result[%d]\r\n",enm_FlashResult));
					enm_Result = eTDAL_OTA_STATUS_ERROR;
					goto FAIL0;
				}
				b_UseBackup = true;
			}
		}
		else
		{
			mTBOX_TRACE((kTBOX_NIV_CRITICAL,"CH_COM_GetLoaderInfo-->12-->XXXX\r\n"));
			enm_Result = eTDAL_OTA_STATUS_ERROR;
			goto FAIL0;
		}

		/*判断读出来的数据是否有长度信息*/
		if( rpstru_LoaderInfo->i_length < 4 ||rpstru_LoaderInfo->i_length > ( i_ActualReadSize -16 ) )
		{
			b_MainSaveOK = false;/*读出来的数据有问题*/
		}
		else
		{
			/*校验主数据*/
			ui_CrcValue = CRC_MPEG32_((U8 *)&(rpstru_LoaderInfo->ui_OperatorNumber), rpstru_LoaderInfo->i_length -4);
			if(ui_CrcValue == rpstru_LoaderInfo->ui_crc)
			{
				if(strcmp(rpstru_LoaderInfo->uc_name,LOADER_INFO_MAGIC) == 0)
				{
					b_MainSaveOK = true;
				}
			}

		}
		/*如果使用了备份分区，则进行备份数据校验*/
		if(b_UseBackup == true)
		{
			if(stru_BackupLoaderInfo.i_length < 4 ||stru_BackupLoaderInfo.i_length > ( i_ActualBackupReadSize -16 ) )
			{
				b_BackSaveOK = false;  /*读出来的数据有问题*/
			}
			else
			{
				/*校验备份数据*/
				ui_CrcValue = CRC_MPEG32_((U8 *)&(stru_BackupLoaderInfo.ui_OperatorNumber), stru_BackupLoaderInfo.i_length - 4);
				if(ui_CrcValue == stru_BackupLoaderInfo.ui_crc)
				{
					if(strcmp(stru_BackupLoaderInfo.uc_name,LOADER_INFO_MAGIC) == 0)
					{
						b_BackSaveOK = true;
					}
				}

			}
			/*如果数据都出错，则返回错误*/
			if(!(b_BackSaveOK || b_MainSaveOK))
			{
				mTBOX_TRACE((kTBOX_NIV_CRITICAL,"CH_COM_GetLoaderInfo-->13-->b_BackSaveOK=%d,b_MainSaveOK=%d\r\n",b_BackSaveOK,b_MainSaveOK));
				enm_Result = eTDAL_OTA_STATUS_ERROR;
				goto FAIL0;
			}

			/*如果备份出错或两个都对，但不一致，则修复备份*/
			if((b_BackSaveOK== false) ||
				(memcmp((U8*)rpstru_LoaderInfo, (U8*)&stru_BackupLoaderInfo, sizeof(CH_LoaderInfo_t)) && b_MainSaveOK))
			{
				if( gstru_fact_SaveParams[CH_COM_INFOTYPE_LOADER].stru_BackUpInfo.enm_Device == CH_COM_SAVEDEVICE_FLASH)
				{
					enm_FlashResult = CHDRV_FLASH_DirectWrite(gstru_fact_SaveParams[CH_COM_INFOTYPE_LOADER].stru_MainInfo.ui_OffsetAddress,
															(U8*)rpstru_LoaderInfo,i_ActualReadSize);
					if( enm_FlashResult == 0 )
					{
						mTBOX_TRACE((kTBOX_NIV_CRITICAL,"CH_COM_GetLoaderInfo-->14-->Result[%d]\r\n",enm_FlashResult));
						enm_Result = eTDAL_OTA_STATUS_ERROR;
						goto FAIL0;
					}
				}
			}

			/*主数据出错，使用备份数据，并修复主数据*/
			if(b_MainSaveOK == false)
			{
				memcpy((U8*)rpstru_LoaderInfo, (U8*)&stru_BackupLoaderInfo, sizeof(CH_LoaderInfo_t));
				if( gstru_fact_SaveParams[CH_COM_INFOTYPE_LOADER].stru_MainInfo.enm_Device == CH_COM_SAVEDEVICE_FLASH)
				{
					enm_FlashResult = CHDRV_FLASH_DirectWrite(gstru_fact_SaveParams[CH_COM_INFOTYPE_LOADER].stru_MainInfo.ui_OffsetAddress,
															(U8*)rpstru_LoaderInfo,i_ActualBackupReadSize);
					if( enm_FlashResult == 0 )
					{
						mTBOX_TRACE((kTBOX_NIV_CRITICAL,"CH_COM_GetLoaderInfo-->16-->Result[%d]\r\n",enm_FlashResult));
						enm_Result = eTDAL_OTA_STATUS_ERROR;
						goto FAIL0;
					}
				}
			}
		}
		else
		{
			if(b_MainSaveOK == false)
			{
				mTBOX_TRACE((kTBOX_NIV_CRITICAL,"CH_COM_GetLoaderInfo-->18-->XXXX\r\n"));
				enm_Result = eTDAL_OTA_STATUS_ERROR;
				goto FAIL0;
			}
		}


		if( i_ActualReadSize != i_ReadSize )
		{
			mTBOX_TRACE((kTBOX_NIV_CRITICAL,"CH_COM_GetLoaderInfo-->19-->i_ActualReadSize=%d,i_ReadSize=%d\r\n",i_ActualReadSize,i_ReadSize));
			enm_Result = eTDAL_OTA_STATUS_ERROR;
			goto FAIL0;
		}

		if( (gstru_fact_SaveParams[CH_COM_INFOTYPE_LOADER].stru_BackUpInfo.ui_OffsetAddress != -1)&& i_ActualBackupReadSize != i_ReadSize)
		{
			mTBOX_TRACE((kTBOX_NIV_CRITICAL,"CH_COM_SetLoaderInfo-->20-->i_ActualReadSize=%d,i_ReadSize=%d\r\n",i_ActualBackupReadSize,i_ReadSize));
			enm_Result = eTDAL_OTA_STATUS_ERROR;
			goto FAIL0;
		}
		if( strcmp(rpstru_LoaderInfo->uc_name,LOADER_INFO_MAGIC) )
		{
			mTBOX_TRACE((kTBOX_NIV_CRITICAL,"CH_COM_GetLoaderInfo-->21-->uc_name Error\r\n"));
			enm_Result = eTDAL_OTA_STATUS_ERROR;
			goto FAIL0;
		}
		}
FAIL0:


#endif 	/*<!-- gongwenqing 2013/12/13 13:59:24 --!>*/

	return enm_Result;
}
