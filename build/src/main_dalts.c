/**************************************************************************//**
* @file    main_dalts.c
*
* @brief   Main program to test only the DAL with the DALTS library
*          without comedia, the packages...
*          Start the DALTS (merlin V7) and the CAK DAL.
*
* @author  SmarDTV Rennes - LIPPA
******************************************************************************
* SmarDTV Copyright 2011
******************************************************************************
* $URL: http://10.50.1.219/svncomedia/idecode/product/comedia/branches/snt_imcl_dlk_312B/build/src/main_dalts.c $
* $Rev: 9410 $
* $Date: 2012-02-28 17:35:47 +0100 (mar., 28 févr. 2012) $
******************************************************************************/

/******************************************************************************
* Includes
******************************************************************************/

#include "crules.h"
#include "tkel.h"
#include "tbox.h"
#include "tdal_common.h"
#include "tdal_fla.h"
#include "tdal_nvm.h"
#include "tdal_dmd.h"
#include "tdal_dmx.h"
#include "tdal_uart.h"
#include "tdal_desc.h"
#include "tdal_smc.h"
#include "tdal_sec.h"
#include "tdal_perso.h"

#ifdef _USE_DATAMNGR_JFS_
#include "file_system_cfg.h"
#endif

#ifdef __DEBUG_TRACE__
#include "trace_cfg.h"
#endif

#include "ca_cak.h"
#include "cak_dal.h"

#ifndef CAK_V7
#error "This implementation require the CAK version 7"
#endif /* CAK_V7 */


/******************************************************************************
* Defines
******************************************************************************/

#define kMDALTSi_MODULE_PAIRING_KEY_SIZE     512


/******************************************************************************
* Macros
******************************************************************************/

/*
 */
#define __MAIN_DALTS_TRACE_DEBUG__ 1
/*
 */

#if defined(__DEBUG_TRACE__) && !defined(__MAIN_DALTS_TRACE_DEBUG__)
#define __MAIN_DALTS_TRACE_DEBUG__ 1
#endif


#if defined(__MAIN_DALTS_TRACE_DEBUG__)
#define mMDALTSi_DEBUG(x) printf x
#else
#define mMDALTSi_DEBUG(x)
#endif


/******************************************************************************
* Typedefs
******************************************************************************/

/******************************************************************************
* Locales Variables
******************************************************************************/

/******************************************************************************
* Locales Functions Prototypes
******************************************************************************/

static TCaStatus MDALTSi_PairingDataCallback    (const TUnsignedInt8** ppxNaspPairingData);
static TCaStatus MDALTSi_UtcTimeCallback        (TCalendarTime* pxUtcTime);


/******************************************************************************
 * Public Function
 ******************************************************************************/

/**
 *  @brief
 *    Run DALTS program:
 *    - TDAL initialization
 *    - TKEL initialization
 *    - CAK DAL initialization
 *    - CAK initilization and startup.
 *
 *  @retval  0
 *            Successful
 *
 *  @retval  1
 *            Error
 *
 *  @remarks
 *    -# Implementaion for CAK version V7.
*/
int main(int argc, char *argv[])
{
   bool                 core = TRUE;
   TKEL_err             tk_rc;
   tCAK_DAL_Status      dal_rc;
   TCaStatus            cak_rc;
   tTDAL_FLA_ErrorCode  fla_rc;
   tTDAL_NVM_ErrorCode  nvm_rc;
	tTDAL_DMD_Error      dmd_rc;
   tTDAL_DMX_Error      dmx_rc;
   tTDAL_UART_ErrorCode uart_rc;
   tTDAL_DESC_Error     desc_rc;
   tTDAL_SMC_ErrorCode  smc_rc;
   tTDAL_SEC_ErrorCode  sec_rc;
   eTDAL_PERSO_ErrorCode perso_rc;

   /* TDAL initialization */
   TDAL_Init();

   /* TKEL initialization */
   tk_rc = TKEL_Init();
   if (tk_rc != TKEL_NO_ERR)
   {
      mMDALTSi_DEBUG(("TKEL initialization failure (error %d)\n",tk_rc));
      return 1;
   }

#ifdef __DEBUG_TRACE__
   TRACE_CFG_Init();
#endif

   /* TDAL modules initialization */
   mMDALTSi_DEBUG(("[main_dalts] TDAL modules initialization...\n"));
   fla_rc = TDAL_FLA_Init();
   if (fla_rc != eTDAL_FLA_NO_ERROR)
   {
      mMDALTSi_DEBUG(("TDAL FLA initialization failure (error %d)\n",fla_rc));
      return 1;
   }
   nvm_rc = TDAL_NVM_Init();
   if (nvm_rc != eTDAL_NVM_NO_ERROR)
   {
      mMDALTSi_DEBUG(("TDAL NVM initialization failure (error %d)\n",nvm_rc));
      return 1;
   }
   nvm_rc = TDAL_NVM_Open();
   if (nvm_rc != eTDAL_NVM_NO_ERROR)
   {
      mMDALTSi_DEBUG(("TDAL NVM open failure (error %d)\n",nvm_rc));
      return 1;
   }
   dmd_rc = TDAL_DMD_Init();
   if (dmd_rc != eTDAL_DMD_NO_ERROR)
   {
      mMDALTSi_DEBUG(("TDAL DMD initialization failure (error %d)\n",dmd_rc));
      return 1;
   }
   dmx_rc = TDAL_DMX_Init();
   if (dmx_rc != kTDAL_DMX_NO_ERROR)
   {
      mMDALTSi_DEBUG(("TDAL DMX initialization failure (error %d)\n",dmx_rc));
      return 1;
   }
   uart_rc = TDAL_UART_Init();
   if(uart_rc != kTDAL_DMX_NO_ERROR)
   {
      mMDALTSi_DEBUG(("TDAL UART initialization failure (error %d)\n",uart_rc));
      return 1;
   }
   desc_rc = TDAL_DESC_Init();
   if(desc_rc != eTDAL_DESC_NO_ERROR)
   {
      mMDALTSi_DEBUG(("TDAL DESC initialization failure (error %d)\n",desc_rc));
      return 1;
   }
   smc_rc = TDAL_SMC_Init();
   if (smc_rc != eTDAL_SMC_NO_ERROR)
   {
      mMDALTSi_DEBUG(("TDAL SMC initialization failure (error %d)\n",smc_rc));
      return 1;
   }
   sec_rc = TDAL_SEC_Init();
   if (sec_rc != eTDAL_SEC_NO_ERROR)
   {
      mMDALTSi_DEBUG(("TDAL SEC initialization failure (error %d)\n",sec_rc));
      return 1;
   }
 	perso_rc = TDAL_PERSO_Init();
	if (perso_rc != eTDAL_PERSO_NO_ERROR)
	{
      mMDALTSi_DEBUG(("TDAL PERSO initialization failure (error %d)\n",sec_rc));
      return 1;
   }
#ifdef _USE_DATAMNGR_JFS_
   /* Mount Files System Partition */
   mMDALTSi_DEBUG(("[main_dalts] Mount FILES SYSTEM PARTIONS..\n"));
   if (!FS_CFG_MountFilesSystem(FALSE,TRUE,NULL))
   {
      mMDALTSi_DEBUG(("Mount Files System ERROR\n"));
      return 1;
   }
#endif

   mMDALTSi_DEBUG(("[main_dalts] Initialization...\n"));

   /* Initialize DAL */
   mMDALTSi_DEBUG(("[CAK_MNGR_Init] - Init DAL\n"));
   dal_rc = CAK_DAL_Init();
   if (dal_rc != eCAK_DAL_NO_ERROR)
   {
      mMDALTSi_DEBUG(("main_dalts ERROR: DAL initialization failure (error %d)\n",
                        dal_rc));
      return 1;
   }

   /* CAK initialization */
   mMDALTSi_DEBUG(("[main_dalts] - Init CAK\n"));
   cak_rc = caInitialization(MDALTSi_PairingDataCallback, 
                             MDALTSi_UtcTimeCallback, 
                             1);
   if(cak_rc != CA_NO_ERROR)
   {
      mMDALTSi_DEBUG(("main_dalts ERROR: caInitialization failure (error %d)\n",cak_rc));
      CAK_DAL_Term();
      return 1;
   }

   /* Start library */
   mMDALTSi_DEBUG(("[main_dalts] - Start CAK\n"));
   cak_rc = caStartUp();
   if (cak_rc != CA_NO_ERROR)
   {
      mMDALTSi_DEBUG(("main_dalts ERROR: caStartUp failure (error %d)\n",cak_rc));
      caTermination();
      CAK_DAL_Term();
      return 1;
   }

   /* Okay */
   mMDALTSi_DEBUG(("[main_dalts] CAK startup DONE\n"));

   while (core)
   {
      TKEL_DelayTask(ms2tick(60000));
      mMDALTSi_DEBUG(("[main_dalts] Awake\n"));
   }

   caTermination();
   CAK_DAL_Term();

   return 0;
}


/******************************************************************************
* Locales Functions Implementations
******************************************************************************/

/**
 *  @brief
 *    Called by the CAK library, provides the pairing information .
 *
 *  @param[out] ppxNaspPairingData
 *                Pointer on pairing data table.
 *
 *  @retval  CA_NO_ERROR
 *              If success
 *
 *  @retval  CA_ERROR
 *              If error
 *
 *  @remarks
 *    -# Use operator tests Key.
*/
static TCaStatus MDALTSi_PairingDataCallback 
(
   const TUnsignedInt8** ppxNaspPairingData
)
{
   TCaStatus       caStatus = CA_NO_ERROR;
   TUnsignedInt8  *pbExtendedPairingData;
   TSize           sizeData = kMDALTSi_MODULE_PAIRING_KEY_SIZE;
   TUnsignedInt8   pbExtendedPairingDataTemp[] = {
   0x00, 0x00, 0x01, 0x6C, 0x00, 0x01, 0x00, 0x00, 0x03, 0x03, 0xFA, 0x13, 0x35, 0xE8, 0x17, 0xFE,
   0xE0, 0xCE, 0x47, 0x61, 0x0B, 0xDA, 0x6C, 0xC7, 0xF0, 0xFB, 0xDD, 0x07, 0x77, 0x95, 0x91, 0xA8,
   0xF5, 0xBA, 0x1A, 0x3F, 0xAC, 0x22, 0x8A, 0x29, 0x13, 0x1C, 0x19, 0x7E, 0xD4, 0x07, 0xD7, 0xBA,
   0x91, 0x0F, 0xB8, 0x28, 0x27, 0x1D, 0x1F, 0x8E, 0x0F, 0xB0, 0x70, 0xC3, 0x4C, 0x5C, 0x6F, 0x41,
   0xA4, 0x12, 0xC3, 0xE6, 0xF8, 0x38, 0x3E, 0x4A, 0x92, 0xFA, 0x17, 0x71, 0x73, 0x17, 0x01, 0x09,
   0x2E, 0xD5, 0x69, 0x79, 0x31, 0x24, 0x6A, 0xA1, 0xF6, 0xA5, 0xED, 0x6A, 0x54, 0xBC, 0xB3, 0xCA,
   0x25, 0xB9, 0xCA, 0x7A, 0x8B, 0xD7, 0x08, 0x4D, 0x40, 0x3D, 0x9C, 0xA6, 0xA8, 0x86, 0x07, 0x72,
   0x00, 0x51, 0x1E, 0x23, 0x20, 0xA3, 0xC9, 0x7B, 0x37, 0x3E, 0x56, 0x9E, 0xA0, 0x12, 0x45, 0x8B,
   0x37, 0x3D, 0x61, 0x69, 0x1E, 0xC8, 0x49, 0x54, 0x42, 0x67, 0x68, 0x1C, 0x1F, 0x67, 0xCE, 0x01,
   0x57, 0x91, 0x17, 0xF9, 0x8C, 0xEA, 0x93, 0xD6, 0x5D, 0x89, 0x23, 0x02, 0x02, 0xA0, 0x4D, 0x42,
   0x1C, 0x0E, 0xD9, 0x71, 0x84, 0x56, 0x4C, 0xC5, 0x02, 0xD7, 0x54, 0x00, 0x46, 0x02, 0x66, 0x9E,
   0x81, 0x77, 0x12, 0x90, 0xC3, 0x85, 0x31, 0x99, 0x98, 0xAD, 0x64, 0x7D, 0xE3, 0x52, 0xC9, 0xC7,
   0x55, 0x09, 0x94, 0xE7, 0xC2, 0xDE, 0x4D, 0x72, 0xBE, 0x71, 0x08, 0x1E, 0x9A, 0x5B, 0x5B, 0x26,
   0x9E, 0x98, 0xC7, 0x48, 0x60, 0x2E, 0x35, 0x07, 0x36, 0x27, 0xF7, 0x54, 0x23, 0xB9, 0x4C, 0x27,
   0xA5, 0x49, 0x31, 0x1E, 0x34, 0x9B, 0xC6, 0x55, 0xA6, 0xAE, 0xFA, 0xEF, 0x79, 0x44, 0x10, 0xAB,
   0xA6, 0xD2, 0x80, 0x8F, 0x81, 0x51, 0x00, 0x3B, 0x3B, 0x56, 0xB0, 0x29, 0x98, 0x1F, 0xCD, 0xB3,
   0xA7, 0xE0, 0xD3, 0xB7, 0xA5, 0x70, 0x97, 0x4D, 0x7B, 0x87, 0x01, 0x91, 0x6B, 0x5D, 0x8D, 0x2C,
   0x7D, 0x3F, 0x9F, 0xD8, 0xEC, 0xA3, 0x7B, 0xCD, 0x6F, 0xAE, 0x47, 0xBC, 0xF7, 0x0B, 0xDE, 0x11,
   0x40, 0x14, 0xA1, 0xC8, 0x4A, 0x16, 0xAA, 0x6A, 0xBD, 0x66, 0x66, 0xEA, 0x62, 0xBF, 0xA2, 0xBC,
   0xFF, 0x18, 0x1D, 0xC4, 0x19, 0xC9, 0xF3, 0x5E, 0x28, 0x79, 0xFB, 0x92, 0xA6, 0xC4, 0x17, 0xD7,
   0xDA, 0x10, 0xC6, 0xFD, 0xEA, 0x43, 0x74, 0xBA, 0x04, 0xB6, 0xAD, 0xB9, 0x17, 0x74, 0x82, 0xF5,
   0xF8, 0xE7, 0xEC, 0x87, 0xCE, 0x92, 0x53, 0xF3, 0x91, 0xA7, 0x53, 0x22, 0x89, 0x96, 0xCF, 0x2A,
   0xE5, 0x0E, 0x50, 0x42, 0x62, 0x53, 0x3D, 0xE8, 0xD2, 0xA8, 0x64, 0x9D, 0x16, 0xA9, 0x02, 0xE9,
   0x3E, 0x84, 0xF1, 0x23, 0x1A, 0x4A, 0x00, 0x28, 0x7B, 0x19, 0xDD, 0x7E, 0x05, 0x31, 0xEA, 0x2F,
   0xC9, 0x92, 0xF1, 0x77, 0x0A, 0xF1, 0x3B, 0x33, 0x1D, 0xBB, 0x89, 0x01, 0xAB, 0x18, 0xA8, 0xBD,
   0xCC, 0xFD, 0x46, 0x8D, 0x9D, 0xD8, 0x5D, 0x94, 0x97, 0xD9, 0x92, 0x65, 0x07, 0x06, 0xDA, 0xC4,
   0x6C, 0x1E, 0x47, 0xAE, 0x60, 0x15, 0x00, 0x55, 0x0B, 0x05, 0xA0, 0x89, 0xC4, 0x94, 0xCB, 0x64,
   0x95, 0x90, 0x1B, 0x75, 0x4C, 0x6C, 0x77, 0xD0, 0xE1, 0xE4, 0x54, 0x3A, 0x32, 0x6F, 0xE6, 0xFE,
   0x61, 0xE1, 0x9A, 0xA0, 0xE8, 0x7F, 0x51, 0x81, 0xBF, 0x3E, 0x7E, 0x12, 0x1C, 0x1B, 0xDE, 0x4F,
   0x28, 0xF2, 0xA6, 0x72, 0xA2, 0x2E, 0x14, 0xC1, 0x27, 0x87, 0xF4, 0x8F, 0x2A, 0x88, 0x29, 0x0E,
   0x52, 0x52, 0x43, 0x83, 0x09, 0x1C, 0x9B, 0x7C, 0x62, 0xC1, 0x38, 0x41};

   mMDALTSi_DEBUG(("MDALTSi_PairingDataCallback: Allocate %d bytes",sizeData));

   /* Allocate the buffer */
   pbExtendedPairingData = (TUnsignedInt8*)osMemoryAllocate(sizeData);
   if(pbExtendedPairingData == NULL)
   {
      mMDALTSi_DEBUG(("MDALTSi_PairingDataCallback ERROR: Unable to allocate %d bytes",sizeData));
      return OS_ERROR;
   }

   // Copy the hardcoded pairing key
   memcpy(pbExtendedPairingData, pbExtendedPairingDataTemp, sizeof(pbExtendedPairingDataTemp) );

   // Copy the buffer
   *ppxNaspPairingData = pbExtendedPairingData;

   return caStatus;
}

/**
 *  @brief
 *    Called by the library, provides the UTC time.
 *
 *  @param[out] pxUtcTime
 *                Pointer to time information.
 *
 *  @retval  CA_NO_ERROR
 *              If success
 *
 *  @retval  CA_ERROR
 *              If error
 *
 *  @remarks
 *    -# Use TBOX current time (without date) and start at the 1/1/2011
*/
static TCaStatus MDALTSi_UtcTimeCallback(TCalendarTime* pxUtcTime)
{
   tTBOX_Time now;

   if(pxUtcTime == NULL)
   {
      mMDALTSi_DEBUG(("MDALTSi_UtcTimeCallback ERROR: pxUtcTime undefinedL"));
      return CA_ERROR;
   }

   TBOX_GetCurrentTime(&now);
   pxUtcTime->year   = (TUnsignedInt8)111;      /**<  Years since 1900                  */
   pxUtcTime->month  = (TUnsignedInt8)1;        /**<  Month of the year (1 to 12)       */
   pxUtcTime->day    = (TUnsignedInt8)1;        /**<  Day of the month (1 to 31)        */
   pxUtcTime->hour   = (TUnsignedInt8)now.hour; /**<  Hours since midnight (0 to 23)    */
   pxUtcTime->minute = (TUnsignedInt8)now.min;  /**<  Minute after the hour (0 to 59)   */
   pxUtcTime->second = (TUnsignedInt8)now.sec;  /**<  Second after the minute (0 to 59) */

   mMDALTSi_DEBUG(("[MDALTSi_UtcTimeCallback] Date: %02d-%02d-%04d - Time: %02d:%02d:%02d",
                     pxUtcTime->day,pxUtcTime->month,pxUtcTime->year+1900,
                     pxUtcTime->hour,pxUtcTime->minute,pxUtcTime->second));

   return CA_NO_ERROR;
}

/** Returning the address of CSC data in flash.
 *
 * @return an integer indicating the address of CSC data in flash.
 * @warning CSC are unique for each STB (NUID).
 */
/** Returning the address of CSC data in flash.
 *
 * @return an integer indicating the address of CSC data in flash.
 * @warning CSC are unique for each STB (NUID).
 */
uint32_t CA_CFG_GetCSCAddress(void)
{
	
		uint16_t length = 0;
		uint8_t * cscAddressRead = NULL;
		uint32_t cscAddress = 0;
		eTDAL_PERSO_ErrorCode errorCode = eTDAL_PERSO_ERROR;
	
	
		errorCode = TDAL_PERSO_GetTagLength( PERSO_TAG_CSCD_ADDRESS , &length );
		if(eTDAL_PERSO_NO_ERROR == errorCode)
		{
			if(0!=length)
			{
				if(TKEL_NO_ERR==TKEL_Malloc(length,(void**)&cscAddressRead))
				{
					if(eTDAL_PERSO_NO_ERROR == TDAL_PERSO_ReadTag( PERSO_TAG_CSCD_ADDRESS , &length, cscAddressRead ))
					{
						cscAddress = (((uint32_t)(cscAddressRead[0]))<<24) | (((uint32_t)(cscAddressRead[1]))<<16) |
									(((uint32_t)(cscAddressRead[2]))<<8) | (uint32_t)(cscAddressRead[3]);
	
						TKEL_Free(cscAddressRead);
						return cscAddress;
					}
					TKEL_Free(cscAddressRead);
				}
			}
		}
	
		return 0;
}

uint32_t CA_CFG_GetCSCLength(void)
{
	uint16_t length = 0;
	uint8_t * cscLengthRead = NULL;
	uint32_t cscLength = 0;
	eTDAL_PERSO_ErrorCode errorCode = eTDAL_PERSO_ERROR;

	errorCode = TDAL_PERSO_GetTagLength( PERSO_TAG_CSCD_SIZE , &length );
	if(eTDAL_PERSO_NO_ERROR == errorCode)
	{
		if(0!=length)
		{
			if(TKEL_NO_ERR==TKEL_Malloc(length,(void**)&cscLengthRead))
			{
				if(eTDAL_PERSO_NO_ERROR == TDAL_PERSO_ReadTag( PERSO_TAG_CSCD_SIZE , &length, cscLengthRead ))
				{
					cscLength = (((uint32_t)(cscLengthRead[0]))<<24) | (((uint32_t)(cscLengthRead[1]))<<16) |
								(((uint32_t)(cscLengthRead[2]))<<8) | (uint32_t)(cscLengthRead[3]);
					TKEL_Free(cscLengthRead);
					return cscLength;
				}
				TKEL_Free(cscLengthRead);
			}
		}
	}

	return 0;
}

uint32_t CA_CFG_GetPKAddress(void)
{
	uint16_t length = 0;
	uint8_t * pkAddressRead = NULL;
	uint32_t pkAddress = 0;
	eTDAL_PERSO_ErrorCode errorCode = eTDAL_PERSO_ERROR;

	errorCode = TDAL_PERSO_GetTagLength( PERSO_TAG_PK_ADDRESS , &length );
	if(eTDAL_PERSO_NO_ERROR == errorCode)
	{
		if(0!=length)
		{
			if(TKEL_NO_ERR==TKEL_Malloc(length,(void**)&pkAddressRead))
			{
				if(eTDAL_PERSO_NO_ERROR == TDAL_PERSO_ReadTag( PERSO_TAG_PK_ADDRESS , &length, pkAddressRead ))
				{
					pkAddress = (((uint32_t)(pkAddressRead[0]))<<24) | (((uint32_t)(pkAddressRead[1]))<<16) |
								(((uint32_t)(pkAddressRead[2]))<<8) | (uint32_t)(pkAddressRead[3]);

					TKEL_Free(pkAddressRead);

					return pkAddress;
				}
				TKEL_Free(pkAddressRead);
			}
		}
	}

	return 0;
}

uint16_t CA_CFG_GetPKLength(void)
{
	uint16_t length = 0;
	uint8_t * pkLengthRead = NULL;
	uint32_t pkLength = 0;
	eTDAL_PERSO_ErrorCode errorCode = eTDAL_PERSO_ERROR;

	errorCode = TDAL_PERSO_GetTagLength( PERSO_TAG_PK_SIZE , &length );
	if(eTDAL_PERSO_NO_ERROR == errorCode)
	{
		if(0!=length)
		{
			if(TKEL_NO_ERR==TKEL_Malloc(length,(void**)&pkLengthRead))
			{
				if(eTDAL_PERSO_NO_ERROR == TDAL_PERSO_ReadTag( PERSO_TAG_PK_SIZE , &length, pkLengthRead ))
				{
					pkLength = (((uint32_t)(pkLengthRead[0]))<<24) | (((uint32_t)(pkLengthRead[1]))<<16) |
								(((uint32_t)(pkLengthRead[2]))<<8) | (uint32_t)(pkLengthRead[3]);
					TKEL_Free(pkLengthRead);
					return pkLength;
				}
				TKEL_Free(pkLengthRead);
			}
		}
	}

	return 0;
}
void CAGLUE_WriteChecksum( uint8_t * const checksum, uint8_t const * block, size_t const length )
{
}

/* main_dalts.c */

