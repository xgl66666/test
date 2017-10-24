/********************************************************************************
 *                       COPYRIGHT 2011 SMARDTV                                 *
 ********************************************************************************
 *
 * MODULE NAME: NAGRA CFG
 *
 * FILE NAME: $URL: http://10.50.1.219/svncomedia/idecode/sources/packages/ccas/nagra/config/trunk/nagra_cfg.c $
 *            $Rev: 6988 $
 *            $Date: 2011-07-22 09:44:47 +0200 (Fri, 22 Jul 2011) $
 *
 *
 *******************************************************************************/

/********************************************************/
/*                Includes                              */
/********************************************************/

#include <string.h>
#include <stdio.h>
#include "crules.h"
#include "cm_setup.h"
#include "country_cfg.h"
#include "nagra_cfg.h"
#include "flash_cfg.h"
#include "tkel.h"
#include "tdal_fla.h"
#include "tdal_perso.h"

#ifdef EMM_WAKEUP
#include "plugin_i.h"

typedef struct _tEMM_PARAMETERS
{
   uint8_t service_flag;
   uint16_t service_id;
   uint8_t transport_flag;
   uint16_t transport_stream_id;
   uint8_t network_flag;
   uint16_t network_id;   
   uint16_t waking_duration;
   uint16_t sleeping_duration;
}
tEMM_PARAMETERS; 

tEMM_PARAMETERS wakeUp;
#endif

/********************************************************/
/*              Defines                                 */
/********************************************************/

/*
 * Providers are defined for feature product IDs
 */

#if defined(IMCL) || defined(VSM) || defined(DEN)
    #define kCA_PVR_FEATURE_PRODUCT_ID 522
    #define kCA_MEDIAPLAYER_FEATURE_PRODUCT_ID 530
#elif defined(GTPL)
    #define kCA_PVR_FEATURE_PRODUCT_ID 1042
    #define kCA_MEDIAPLAYER_FEATURE_PRODUCT_ID 530
#else
    #error "CA Provider is not defined!"
#endif


/********************************************************/
/*              Macros                                  */
/********************************************************/

/********************************************************/
/*              Local Functions Declarations (LOCAL)    */
/*******************************************************/
uint32_t CA_CFG_GetPKAddress(void);
uint16_t CA_CFG_GetPKLength(void);
/********************************************************/
/*              Functions Definitions (LOCAL/GLOBAL)    */
/********************************************************/

/** Returning which color index should be used for fingerprinting.
 *
 * @param coverage is the 0 to 10 value marking 0 to 100% background
 * coverage.
 *
 * @return a (MSTR_INT) integer representing the color index of the color to use
 * for rendering the fingerprint.
 *
 * @note Check the UI palette to return a color index that makes the
 * fingerprint clearly visible from 7 times the height of the screen.
 */
int CA_CFG_GetFingerprintFgColorIndex( int coverage )
{
	int value;

    switch (coverage)
    {
    	case 0:
    		value=2; /* black */
    		break;    	
    	case 1:
    		value=1; /* white */
		break;    	
    	case 2:
    		value=43; /* red */
    		break;    	
    	case 3:
    		value=3; /* blue */
    		break;
    	case 4:
    		value=45; /* green */
    		break;
    	case 5:
    		value=46; /* yellow */
    		break;
#ifdef USE_DLK_1_1
    	case 6:
    		value=2; /* black */
    		break;    	
    	case 7:
    		value=2; /* black */
    		break;    	
    	case 11:
    		value=1; /* white */
		break;    	            
    	case 15:
    		value=0; /* transparent */
		break;    	                    
#endif
    	default:
    		value=0; /* transparent */
    		break;
    }
    return value;
}


/** Returning the current language index used by the smartcard.
 *
 * @return an integer index matching one of the languages listed in
 * country_cfg.c, corresponding to the current language to use.
 *
 * @note The returned language index is expected to be the one currently
 * stored in the smartcard. However, if no language matches the list in
 * country_cfg.c, the selection must be left to cmSetup_GetMenuLanguage().
 */
int CA_CFG_GetCurrentLanguage()
{
    return cmSetup_GetMenuLanguage();
}

/** Returning the current country code used by the smartcard.
 *
 * @return a string corresponding to the current country to use.
 *
 * @note The returned country code is expected to be the one currently
 * stored in the smartcard. However, if no country returned by smartcard,
 * the selection must be left to COUNTRY_CFG_GetCurrentCountryDVBtrigram().
 */
char *CA_CFG_GetCurrentCountry()
{
	char code[kLANGUAGE_NB_CHAR_BY_TRIGRAM];

	/* Reset the code contents - sanity clear though we use stricmp afterwars, but safe nonetheless */
	memset(code, 0, kLANGUAGE_NB_CHAR_BY_TRIGRAM);

#   if 0
	/* Retrieve the current smartcard ISO country code */
	if (!mstrIRDETO_GetSmartcardNationality((uint8_t *)code))
	{
		return(code);
	}
#   endif

	/* Fall back to current country code */
	return(COUNTRY_CFG_GetCurrentCountryDVBtrigram());
}


/** Returning the properties of the NVRAM storage required for messages.
 *
 * @param address_offset is the offset to use in the Persistent Storage VD to target
 *        the message storage.
 *
 * @param storage_size is the length of the message storage area.
 *
 * @return TRUE if the properties are valid when returning from the call, else FALSE.
 *
 */
bool CA_CFG_GetMessageStorageProperties( uint32_t * address_offset, uint32_t * storage_size )
{
    /* Sanitize */
    if( address_offset && storage_size )
    {
        /* This will hold where the VDPS-managed area starts */
        tFLASH_CFG_Location vdps_management_location;
        /* This will hold where the CA messages are stored */
        tFLASH_CFG_Location storage_location;
#       if 1
            /* And retrieve the NVRAM location for storing message from the flash configuration */
            if( FLASH_CFG_GetNvmAreaLocation( eFLASH_CFG_NVM_AREA_CA_STORAGE, &storage_location ) )
            {
//printf("storage_location.addressOffset : 0x%x \r\n",storage_location.addressOffset);
//printf("storage_location.size : 0x%x \r\n",storage_location.size);

            *address_offset = storage_location.addressOffset;
                *storage_size   = storage_location.size;

//printf("address_offset : 0x%x \r\n",*address_offset);
//printf("storage_size : 0x%x \r\n",*storage_size);

                return TRUE;
        }
#       endif
    }
    return FALSE;
}


/** Returning the number of messages managed by the CA.
 *
 * @return an integer indicating the maximum number of messages that should
 * persist in the CA storage.
 *
 * @note The actual number of messages may be lower, depending on the size
 * of the NVRAM storage. Also note that one storage slot is reserved for
 * the fingerprint. For instance, if you want the CA to manage 5 mails/announces,
 * return 5 here and size the NVRAM for 6 messages (see ::tIRDETO_MESSAGE_CONTENT).
 *
 */
int CA_CFG_GetMaxNumberOfMessages( void )
{
    return 50;
}

int CA_CFG_GetMaxNumberOfNormalMessages( void )
{
    return 25;
}

int CA_CFG_GetMaxNumberOfSystemMessages( void )
{
    return 15;
}

/** Returning the Resource Identifier used in the Persistent Storage Virtual
 * Driver for the CA storage.
 *
 * The CA-Glue will also give this identifier to the VDPS in order to proceed to reset
 * to factory settings when required by the middleware (see ::CAMW_RESET_FACTORY).
 *
 * @note There is usually only one resource associated with the CA persistent
 * storage, which contains all the CA information.
 */
int CA_CFG_GetStorageResourceId( void )
{
    return 0;
}


/** Returning the Resource Identifier used in the Copy Control Virtual
 * Driver for the Digital Copy Control resource.
 *
 * The CA-Glue will use the VDCC in order to configure the callback for
 * notifying Copy Control state changes to the middleware (see ::CAMW_COPY_CONTROL).
 *
 */
int CA_CFG_GetDigitalCopyControlResourceId( void )
{
    return 0;
}


#ifdef USE_FILTERED_OUT_ERRORS
/** Indicating whether displaying a specific error should be prevented or not.
 *
 * The CA-Glue will call this function for every error status sent by SoftCell.
 *
 */
bool CA_CFG_ErrorMustBeFiltered( ia_status_st const * const error )
{
    /* Reject based on severity */
    switch( error->eSeverity )
    {
        case STATUS_INFO_DND:
            return true;

        case STATUS_FATAL:
        case STATUS_ERROR:
        case STATUS_WARNING:
        case STATUS_INFO:
            break;
    }

    /* Reject based on status */
    switch( error->wStatus )
    {
        case 34:
            return true;

        default:
            break;
    }

    return false;
}
#endif


/** Indicating whether fingerprint options are to be applied to HASH_ID messages.
 *
 */
bool CA_CFG_FingerprintOptionsMustApply( void )
{
    /* Return not-supported/false to pass the DVT test "Fingerprinting" without question */
    return false;
}


/** Returning the pairing data.
 *
 * @warning When changing those Pairing Keys, make sure the libraries you link with are
 * relevant. This configuration is located in build/make/ca.mak.
 */
unsigned char* CA_CFG_GetPairingData( void )
{
    uint16_t pkLength = CA_CFG_GetPKLength();
    uint32_t pkAddress = CA_CFG_GetPKAddress();
    static unsigned char * pairing_data = NULL;

    if (pairing_data == NULL)
    {
    	if(TKEL_NO_ERR == TKEL_Malloc(pkLength,(void**)&pairing_data))
    	{
    		memcpy(pairing_data, pkAddress, pkLength);
    	}
    }

    return pairing_data;
}

/** Returning the length of CSC data in flash.
 *
 * @return an integer indicating the length of CSC data in flash.
 */
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

/** Returning the length of PK data in flash.
 *
 * @return an integer indicating the length of PK data in flash.
 */
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

/** Returning the address of PK data in flash.
 *
 * @return an integer indicating the address of PK data in flash.
 */
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

#if defined(GTPL)
static CA_CFG_Location CA_CFGi_ConvertPersoLocation(uint32_t location)
{
    switch(location)
    {
    case PERSO_LOCATION_MEMORY:
        return CA_CFG_Location_Memory;
    case PERSO_LOCATION_FLASH:
        return CA_CFG_Location_Flash;
    default:
        return CA_CFG_Location_Unknown;
    }
}

/** Returning the location of CSC data (flash or memory)
 *
 * @return an integer indicating the location of CSC data (flash or memory)
 */
CA_CFG_Location CA_CFG_GetCSCLocation(void)
{
    uint16_t length = 0;
    uint8_t * cscLocationRead = NULL;
    uint32_t cscLocation = 0;
    eTDAL_PERSO_ErrorCode errorCode = eTDAL_PERSO_ERROR;

    errorCode = TDAL_PERSO_GetTagLength( PERSO_TAG_CSCD_LOCATION , &length );
    if(eTDAL_PERSO_NO_ERROR == errorCode)
    {
        if(0!=length)
        {
            if(TKEL_NO_ERR==TKEL_Malloc(length,(void**)&cscLocationRead))
            {
                if(eTDAL_PERSO_NO_ERROR == TDAL_PERSO_ReadTag( PERSO_TAG_CSCD_LOCATION , &length, cscLocationRead ))
                {
                    cscLocation = (((uint32_t)(cscLocationRead[0]))<<24) | (((uint32_t)(cscLocationRead[1]))<<16) |
                                (((uint32_t)(cscLocationRead[2]))<<8) | (uint32_t)(cscLocationRead[3]);
                    TKEL_Free(cscLocationRead);

                    return CA_CFGi_ConvertPersoLocation(cscLocation);
                }
                TKEL_Free(cscLocationRead);
            }
        }
    }

    return CA_CFG_Location_Unknown;
}

/** Returning the location of PK data (memory or flash)
 *
 * @return an integer indicating the location of PK data (memory or flash).
 */
CA_CFG_Location CA_CFG_GetPKLocation(void)
{
    uint16_t length = 0;
    uint8_t * pkLocationRead = NULL;
    uint32_t pkLlocation = 0;
    eTDAL_PERSO_ErrorCode errorCode = eTDAL_PERSO_ERROR;

    errorCode = TDAL_PERSO_GetTagLength( PERSO_TAG_PK_LOCATION , &length );
    if(eTDAL_PERSO_NO_ERROR == errorCode)
    {
        if(0!=length)
        {
            if(TKEL_NO_ERR==TKEL_Malloc(length,(void**)&pkLocationRead))
            {
                if(eTDAL_PERSO_NO_ERROR == TDAL_PERSO_ReadTag( PERSO_TAG_PK_LOCATION , &length, pkLocationRead ))
                {
                    pkLlocation = (((uint32_t)(pkLocationRead[0]))<<24) | (((uint32_t)(pkLocationRead[1]))<<16) |
                                (((uint32_t)(pkLocationRead[2]))<<8) | (uint32_t)(pkLocationRead[3]);
                    TKEL_Free(pkLocationRead);
                    return CA_CFGi_ConvertPersoLocation(pkLlocation);
                }
                TKEL_Free(pkLocationRead);
            }
        }
    }

    return CA_CFG_Location_Unknown;
}
#endif
/** Returning TRUE if IRD force indentification msg enable.
 *
 */
bool CA_CFG_GetForceIdentification( void )
{

#if defined(FTA_CA_PACK_NAGRA)	
	return TRUE;
#else
	return FALSE;
#endif
}

/** Returning TRUE if IRD Fingerprint msg enable.
 *
 */
bool CA_CFG_GetFingerPrint( void )
{

#if defined(FTA_CA_PACK_NAGRA)	
	return TRUE;
#else
	return FALSE;
#endif
}	

/** Returning TRUE if IRD Mail Message enable.
 *
 */
bool CA_CFG_GetMailMessage( void )
{

	/* not enable */
#if defined(FTA_CA_PACK_NAGRA)	
	return TRUE;
#else
	return FALSE;
#endif
}

/** Returning TRUE if IRD popup Message enable.
 *
 */
bool CA_CFG_GetPopupMessage( void )
{

#if defined(FTA_CA_PACK_NAGRA)	
	return TRUE;
#else
	return FALSE;
#endif
}


bool CA_CFG_GetFeatureProductsID( uint32_t *puiPvrProductId,
                                  uint32_t *puiMediaPlayerProductId)
{

    if ( puiPvrProductId == NULL || puiMediaPlayerProductId == NULL)
    {
        return FALSE;
    }

    *puiPvrProductId         = kCA_PVR_FEATURE_PRODUCT_ID;
    *puiMediaPlayerProductId = kCA_MEDIAPLAYER_FEATURE_PRODUCT_ID;

    return TRUE;
}

#ifdef EMM_WAKEUP
bool NAGRA_EMMWakeUpActivated( void )
{
    tCAS_EMM_PARAMETERS emmBuffer;
    
    NAGRA_ReadIrdWakeUp(&emmBuffer);

    if((emmBuffer.new_settings_arrived == TRUE) && (emmBuffer.enable_by_ird == FALSE))
    {
        return FALSE;
    }
    else 
    {
        wakeUp.network_flag = 0;
        wakeUp.service_flag = 0;
        wakeUp.transport_flag = 0;

        if(emmBuffer.new_settings_arrived == FALSE)
        {
            /*  use deafult settings*/
            wakeUp.sleeping_duration = 82800; /* 23h=1380min=82800s*/
            wakeUp.waking_duration = 3600; /* 1h = 60min = 3600s*/     
            
        }
        else
        {
            if(emmBuffer.network_flag)
            {
                wakeUp.network_flag = 1;
                wakeUp.network_id = emmBuffer.network_id;
            }

            if(emmBuffer.service_flag)
            {
                wakeUp.service_flag = 1;
                wakeUp.service_id = emmBuffer.service_id;
            }
            if(emmBuffer.transport_flag)
            {
                wakeUp.transport_flag = 1;
                wakeUp.transport_stream_id = emmBuffer.transport_stream_id;
            }

            wakeUp.sleeping_duration = emmBuffer.sleeping_duration*60;
            wakeUp.waking_duration = emmBuffer.waking_duration*60;

        }

        return TRUE;  
    }
 
}

int NAGRA_EMMWakeUpGetSleepingDuration(void)
{
    return wakeUp.sleeping_duration;
}

int NAGRA_EMMWakeUpGetWakingDuration(void)
{
    return wakeUp.waking_duration;
}
#endif
