/******************************************************************************
 *                    COPYRIGHT 2004 IWEDIA TECHNOLOGIES                      *
 ******************************************************************************
 *
 * MODULE NAME: TRIGGER CFG
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/product/comedia/branches/idecode_hd/build/src/trigger_cfg.c $
 *            $Rev: 332 $
 *            $Date: 2011-09-07 13:51:23 +0200 (Wed, 07 Sep 2011) $
 *
 * PUBLIC
 *
 * DESCRIPTION: trigger configuration
 *
 *****************************************************************************/

/********************************************************/
/*                Includes                              */
/********************************************************/
#include <string.h>

#include "crules.h"
#include "tkel.h"
#include "tbox.h"
#include "trigger_cfg.h"
#include "version_cfg.h"
#include "downloader_cfg.h"

#include "table.h"
#include "trigger.h"
#include "cm_ssu.h"

#ifdef __USE_CONFIG_FILE__
#include "config_file.h"
#endif

mTBOX_SET_MODULE(TRIGGER);

/********************************************************/
/*              Defines                                 */
/********************************************************/
/* descriptors tags/types */
#define kTRIG_CFG_LINKAGE_TYPE_9			0x09
#define kTRIG_CFG_LINKAGE_TYPE_A			0x0A
#define kTRIG_CFG_LINKAGE_TYPE_4			0x04
#define kTRIG_CFG_SSU_DATA_BROADCAST_ID		0x000A
#define kTRIG_CFG_SYSTEM_HARDWARE_DESC      0x01
#define kTRIG_CFG_SYSTEM_SOFTWARE_DESC      0x02

/* SSU info various update_type */
#define kTRIG_CFG_PROPRIETARY_UPDATE_TYPE   0x00
#define kTRIG_CFG_STANDARD_UPDATE_TYPE      0x01
#define kTRIG_CFG_UNT_BROADCAST_UPDATE_TYPE 0x02
#define kTRIG_CFG_UNT_CHANNEL_UPDATE_TYPE   0x03

/* checking bytes size in bytes */
#define kTRIG_CFG_PROTOCOL_ID_SIZE                      2
#define kTRIG_CFG_HW_COMPATIBILITY_MODEL_SIZE           2
#define kTRIG_CFG_HW_COMPATIBILITY_VERSION_SIZE         2
#define kTRIG_CFG_PRODUCT_COMPATIBILITY_MODEL_SIZE      2
#define kTRIG_CFG_PRODUCT_COMPATIBILITY_VERSION_SIZE    2
#define kTRIG_CFG_SOFTWARE_VERSION_SIZE                 4


#ifdef TRIG_USB
#define kTRIG_CFG_SSUUSB_FILE_NAME (kDLD_CFG_USB_DOWNLOAD_FILE_NAME)
#endif

/*---------------*/
/* Compatibility */
/*---------------*/
#if 1
#define TRIG_CFG_OUI                        	SYS_INFO_GetOui()
#define TRIG_CFG_PROTOCOL_ID                	0x0003	/* Genius manage 1 or 2 */
#define TRIG_CFG_PRODUCT_COMP_MODEL         	SYS_INFO_GetProductModelId()
#define TRIG_CFG_PRODUCT_COMP_VERSION       	SYS_INFO_GetProductVersionId()
#define TRIG_CFG_HARDWARE_COMP_MODEL        	SYS_INFO_GetHardwareModelId()
#define TRIG_CFG_HARDWARE_COMP_VERSION      	SYS_INFO_GetHardwareVersionId()
#define TRIG_CFG_VERSION_ID_MSB         		SYS_INFO_GetVersionIdMsb()
#else
#define TRIG_CFG_OUI                        	0x1982
#define TRIG_CFG_PROTOCOL_ID                	0x0003	/* Genius manage 1 or 2 or 3 */
#define TRIG_CFG_PRODUCT_COMP_MODEL         	1
#define TRIG_CFG_PRODUCT_COMP_VERSION       	1
#define TRIG_CFG_HARDWARE_COMP_MODEL        	0x6041
#define TRIG_CFG_HARDWARE_COMP_VERSION      	0x201
#define TRIG_CFG_VERSION_ID_MSB         		0
#endif
/********************************************************/
/*              Macros                                  */
/********************************************************/


/********************************************************/
/*              Local File Variables (LOCAL)            */
/********************************************************/

/*--------------------------------------------------------------------*/
/* NOTE: the medium (SAT, CAB, TER) is selected in trigger_cfg.h file */
/*--------------------------------------------------------------------*/

#if defined (kTRIG_CFG_MEDIUM_SAT)
/* MEDIUM_SAT */

/* network name used by the trigger for linkage descriptor type 0x04 */
static bool TRIG_CFG_EnableNetworkNameSelection = false;
static char TRIG_CFG_NetworkName[] = "";

#elif defined (kTRIG_CFG_MEDIUM_CAB)
/* MEDIUM_CAB */

#elif defined (kTRIG_CFG_MEDIUM_TER)
/* MEDIUM_TER */
#endif

static tTRIG_CFG_DVBTriplet TRIG_CFG_PrevLinkageTypeA_DVBTriplet = {0,0,0};
                                                                      
/********************************************************/
/*              Local Functions Declarations (LOCAL)    */
/*******************************************************/

LOCAL bool TRIG_CFGi_IsDownloaded (uint32_t uiSoftVersion);


/********************************************************/
/*              Functions Definitions (LOCAL/GLOBAL)    */
/********************************************************/

/*===========================================================================
 *
 * TRIG_CFG_StartSsuSearchInBat
 *
 * Parameters:
 *      None
 *
 * Description: Returns TRUE if the trigger must begin to scan the SSU BAT, and not the NIT,
 *              to detect a new version (SSU).
 *
 * Returns:
 *      Nothing
 *
 *===========================================================================*/
bool TRIG_CFG_StartSsuSearchInBat(void)
{
    /* Reset DVB Triplet save of the linkage type 0x0A */
    TRIG_CFG_PrevLinkageTypeA_DVBTriplet.usTransportStreamID = 0;
    TRIG_CFG_PrevLinkageTypeA_DVBTriplet.usOriginalNetworkID = 0;
    TRIG_CFG_PrevLinkageTypeA_DVBTriplet.usServiceID = 0;
    
    return FALSE;
}
/*
**    @brief
**       Return the Bouquet ID where to looks for SSU linkage descriptor
**
*  @remark
**    BID depends on current usage ID
*/
uint16_t TRIG_CFG_GetSsuBatId(void)
{
   uint16_t u16SsuBid;

   u16SsuBid = 0xC020;
   mTBOX_TRACE((kTBOX_NIV_1,"TRIG_CFG_GetSsuBatId()=0x%02x\n", u16SsuBid));
   return u16SsuBid;
}

/*===========================================================================
 *
 * TRIG_CFG_EnableNetworkNameSelection
 *
 * Parameters:
 *      None
 *
 * Description: Returns TRUE if the trigger uses Network Name from network name descriptor, else FALSE
 *
 * Returns:
 *      Nothing
 *
 *===========================================================================*/
#if defined (kTRIG_CFG_MEDIUM_SAT)
bool TRIG_CFG_NetworkNameSelectionEnabled(void)
{
    return TRIG_CFG_EnableNetworkNameSelection;
}
#endif

/*===========================================================================
 *
 * TRIG_CFG_GetTriggerNetworkName
 *
 * Parameters:
 *      None
 *
 * Description: Returns the Network Name used by the trigger
 *
 * Returns:
 *      Nothing
 *
 *===========================================================================*/
#if defined (kTRIG_CFG_MEDIUM_SAT)
void TRIG_CFG_GetTriggerNetworkName(char* pNetworkName)
{
    strcpy(pNetworkName, TRIG_CFG_NetworkName);

    return;
}
#endif

/*===========================================================================
 *
 * TRIG_CFG_GetOUI
 *
 * Parameters:
 *      None
 *
 * Description:
 *      Returns the manufacturer OUI
 *
 * Returns:
 *
 *===========================================================================*/
uint32_t TRIG_CFG_GetOUI(void)
{
	mTBOX_TRACE((kTBOX_NIV_1,"TRIG_CFG_OUI=%x\n",TRIG_CFG_OUI));
    return TRIG_CFG_OUI;
}

/*===========================================================================
 *
 * TRIG_CFG_GetSSUBroadcastProtocolId
 *
 * Parameters:
 *      None
 *
 * Description:
 *      Returns the product concerned by the SSU
 *
 * Returns:
 *
 *===========================================================================*/
uint16_t TRIG_CFG_GetSSUBroadcastProtocolId(void)
{
	mTBOX_TRACE((kTBOX_NIV_1,"TRIG_CFG_PROTOCOL_ID=%x\n",TRIG_CFG_PROTOCOL_ID));
    return (uint16_t)TRIG_CFG_PROTOCOL_ID;
}

/*===========================================================================
 *
 * TRIG_CFG_GetProductCompatibilityModel
 *
 * Parameters:
 *      None
 *
 * Description:
 *      Returns the Product Compatibility Model
 *
 * Returns:
 *
 *===========================================================================*/
uint16_t TRIG_CFG_GetProductCompatibilityModel(void)
{
	mTBOX_TRACE((kTBOX_NIV_1,"TRIG_CFG_PRODUCT_COMP_MODEL=%x\n",TRIG_CFG_PRODUCT_COMP_MODEL));
    return (uint16_t)TRIG_CFG_PRODUCT_COMP_MODEL;
}

/*===========================================================================
 *
 * TRIG_CFG_GetProductCompatibilityVersion
 *
 * Parameters:
 *      None
 *
 * Description:
 *      Returns the Product Compatibility Version
 *
 * Returns:
 *
 *===========================================================================*/
uint16_t TRIG_CFG_GetProductCompatibilityVersion(void)
{
	mTBOX_TRACE((kTBOX_NIV_1,"TRIG_CFG_PRODUCT_COMP_VERSION=%x\n",TRIG_CFG_PRODUCT_COMP_VERSION));
    return (uint16_t)TRIG_CFG_PRODUCT_COMP_VERSION;
}

/*===========================================================================
 *
 * TRIG_CFG_GetHardwareCompatibilityModel
 *
 * Parameters:
 *      None
 *
 * Description:
 *      Returns the Hardware Compatibility Model
 *
 * Returns:
 *
 *===========================================================================*/
uint16_t TRIG_CFG_GetHardwareCompatibilityModel(void)
{
	mTBOX_TRACE((kTBOX_NIV_1,"TRIG_CFG_HARDWARE_COMP_MODEL=%x\n",TRIG_CFG_HARDWARE_COMP_MODEL));
    return (uint16_t)TRIG_CFG_HARDWARE_COMP_MODEL;
}

/*===========================================================================
 *
 * TRIG_CFG_GetHardwareCompatibilityVersion
 *
 * Parameters:
 *      None
 *
 * Description:
 *      Returns the Hardware Compatibility Version
 *
 * Returns:
 *
 *===========================================================================*/
uint16_t TRIG_CFG_GetHardwareCompatibilityVersion(void)
{
	mTBOX_TRACE((kTBOX_NIV_1,"TRIG_CFG_HARDWARE_COMP_VERSION=%x\n",TRIG_CFG_HARDWARE_COMP_VERSION));
    return (uint16_t)TRIG_CFG_HARDWARE_COMP_VERSION;
}


/*===========================================================================
 *
 * TRIG_CFG_GetProductUsageId
 *
 * Parameters:
 *      None
 *
 * Description:
 *      Returns the usageId of the product
 *
 * Returns:
 *
 *===========================================================================*/
uint8_t TRIG_CFG_GetProductUsageId(void)
{
    bool error;
    uint32_t usageID_NVM = 0;
    
    /* get usage ID from the dialog area */
    error = NVM_IBTL_ReadUsageId(&usageID_NVM);
    if ((error == TRUE) &&
        (usageID_NVM > 0))
    {
    	return((uint8_t)usageID_NVM);
    }
    else
    {
        /* get default usage ID from the config file */
    	tCFGF_Configuration *pConfigFile;
    	uint32_t usageID_CFGF;

    	pConfigFile = CFGF_Read(kCFGF_ID_PROD_ID);
    	usageID_CFGF = pConfigFile->stProductIDs.ui8DefaultUsageId;

        /* free the config ressource */
        CFGF_Free(pConfigFile);

    	return(usageID_CFGF);
    }
}


/*===========================================================================
 *
 * TRIG_CFG_GetProductIDs
 *
 * Parameters:
 *      None
 *
 * Description:
 *      Returns the Product IDs
 *
 * Returns:
 *
 *===========================================================================*/
tTRIG_CFG_TriggerProcessStatus TRIG_CFG_GetProductIDs(uint32_t *ulOUI,
                                                      uint16_t *usPlatformModel,
                                                      uint16_t *ulPlatformVersion,
                                                      uint16_t *ulProductModel,
                                                      uint16_t *ulProductVersion,
                                                      uint8_t  *ucProductUsageId)
{
    tCFGF_Configuration	*pConfigFile;

    pConfigFile=CFGF_Read(kCFGF_ID_PROD_ID);
    if (pConfigFile != NULL)
    {
	    *ulOUI = pConfigFile->stProductIDs.ui32OUI;
        *ulProductModel    = pConfigFile->stProductIDs.ui16ProdModel;
        *ulProductVersion  = pConfigFile->stProductIDs.ui16ProdVersion;
        *usPlatformModel   = pConfigFile->stProductIDs.ui16PltfModel;
        *ulPlatformVersion = pConfigFile->stProductIDs.ui16PltfVersion;
	}
	else
	{
		mTBOX_TRACE((kTBOX_NIV_WARNING,"TRIG_CFG_GetProductIDs Failed line=%d\n",__LINE__));
	    return eTRIG_CFG_FAILED;
	}
	
	/* free the config ressource */
	CFGF_Free(pConfigFile);

    /* retrieve usage_id */
	*ucProductUsageId = TRIG_CFG_GetProductUsageId();

    return eTRIG_CFG_CONTINUE;
}

/*===========================================================================
 *
 * TRIG_CFG_CheckDVBTriplet
 *
 * Parameters:
 *      IN      pDVBTriplet             : Pointer on DVB Triplet structure
 *
 * Description:
 *      Check validity of the DVB Triplet
 *
 * Returns:
 *          eTRIG_CFG_CONTINUE if the checking is OK,
 *          else eTRIG_CFG_FAILED
 *
 *===========================================================================*/
tTRIG_CFG_TriggerProcessStatus TRIG_CFG_CheckDVBTriplet(tTRIG_CFG_DVBTriplet *pDVBTriplet)
{
	mTBOX_TRACE((kTBOX_NIV_1,"TRIG_CFG_CheckDVBTriplet eTRIG_CFG_CONTINUE=%x\n",eTRIG_CFG_CONTINUE));
    return eTRIG_CFG_CONTINUE;
}

/*===========================================================================
 *
 * TRIG_CFG_CheckLinkageDesc
 *
 * Parameters:
 *      IN      pLinkageDesc             : Pointer on Linkage descriptor structure
 *      OUT     ulCurrentSoftwareVersion : Current Software version
 *      IN/OUT  ulNewSoftwareVersion     : New sofware version if returned
 *
 * Description:
 *      Check linkage descriptor : linkage_type, DVB triplet and private data bytes
 *
 * Returns:
 *          eTRIG_CFG_CONTINUE if the checking is OK,
 *          eTRIG_CFG_VERSION_UPTODATE if a version is detected and it's the current one,
 *          else eTRIG_CFG_FAILED
 *
 *===========================================================================*/
tTRIG_CFG_TriggerProcessStatus TRIG_CFG_CheckLinkageDesc(tTRIG_CFG_LinkageDesc *pLinkageDesc,
                                                         uint32_t ulCurrentSoftwareVersion,
                                                         uint32_t *ulNewSoftwareVersion)
{
    /* Check Private data Bytes length */
    /* -> no Private data bytes */
    if (pLinkageDesc->ucAddPrivateDataByteLength != 0 )
    {
		mTBOX_TRACE((kTBOX_NIV_WARNING,"TRIG_CFG_CheckLinkageDesc Failed line=%d\n",__LINE__));
        return eTRIG_CFG_FAILED;
    }
    
    if(TRIG_CFG_CheckDVBTriplet(&(pLinkageDesc->stDVBTriplet)) != eTRIG_CFG_CONTINUE)
    {
		mTBOX_TRACE((kTBOX_NIV_WARNING,"TRIG_CFG_CheckLinkageDesc Failed line=%d\n",__LINE__));
        return eTRIG_CFG_FAILED;
    }

    /* To avoid Linkage type 0x0A infinite loop */
    if(pLinkageDesc->ucLinkageType == kTRIG_CFG_LINKAGE_TYPE_A)
    {
        /* => test if it's the same as the previous one */
        if( (pLinkageDesc->stDVBTriplet.usTransportStreamID == 
             TRIG_CFG_PrevLinkageTypeA_DVBTriplet.usTransportStreamID) &&
            (pLinkageDesc->stDVBTriplet.usOriginalNetworkID == 
             TRIG_CFG_PrevLinkageTypeA_DVBTriplet.usOriginalNetworkID) &&
            (pLinkageDesc->stDVBTriplet.usServiceID == 
             TRIG_CFG_PrevLinkageTypeA_DVBTriplet.usServiceID) )
        {
           /* it's the same => infinite loop */
			mTBOX_TRACE((kTBOX_NIV_WARNING,"TRIG_CFG_CheckLinkageDesc Failed line=%d\n",__LINE__));
           return eTRIG_CFG_FAILED;   
        }

        /* Save DVB Triplet of the linkage type 0x0A */
        TRIG_CFG_PrevLinkageTypeA_DVBTriplet.usTransportStreamID = 
                                   pLinkageDesc->stDVBTriplet.usTransportStreamID;
        TRIG_CFG_PrevLinkageTypeA_DVBTriplet.usOriginalNetworkID =
                                   pLinkageDesc->stDVBTriplet.usOriginalNetworkID;
        TRIG_CFG_PrevLinkageTypeA_DVBTriplet.usServiceID = 
                                   pLinkageDesc->stDVBTriplet.usServiceID;
    }
    
	mTBOX_TRACE((kTBOX_NIV_1,"TRIG_CFG_CheckLinkageDesc eTRIG_CFG_CONTINUE line=%d\n",__LINE__));
    return eTRIG_CFG_CONTINUE;
}

/*===========================================================================
 *
 * TRIG_CFG_CheckSsuLinkItem
 *
 * Parameters:
 *      IN      pSsuLinkItem             : Pointer on SSU Link structure
 *      OUT     ulCurrentSoftwareVersion : Current Software version
 *      IN/OUT  ulNewSoftwareVersion     : New sofware version if returned
 *
 * Description:
 *      Check linkage_type 0x09 linkage_descriptor's SSU Link structure
 *
 * Returns:
 *          eTRIG_CFG_CONTINUE if the checking is OK,
 *          eTRIG_CFG_VERSION_UPTODATE if a version is detected and it's the current one,
 *          else eTRIG_CFG_FAILED
 *
 *===========================================================================*/
tTRIG_CFG_TriggerProcessStatus TRIG_CFG_CheckSsuLinkItem(tTRIG_CFG_SsuLinkItem *pSsuLinkItem,
                                                         uint32_t ulCurrentSoftwareVersion,
                                                         uint16_t *ulNominalOrSelectivePID)
{
    uint32_t CFG_OUI; 
	*ulNominalOrSelectivePID = 0x1FFF;

    /* Get Trigger config Compatibility values */
    CFG_OUI = TRIG_CFG_GetOUI();

    /* Checking selector byte 1 according to OpenTVOne_SoftwareUpgrade_Specifications_V1.4.pdf , this by is signaling is OTA selective */
	if(cmSSU_GetTargetUpdateID() != 0xFFFFFFFF &&
	   pSsuLinkItem->ucSelectorByteLength == 0)
	{
		return eTRIG_CFG_FAILED;
	}

	/* Check if there is selector information */
	if(pSsuLinkItem->ucSelectorByteLength == 1)
	{
		if( (pSsuLinkItem->ucSelectorBytes[0] != cmSSU_GetSelector()))
		{
			/* selector information does not match, avoid this item */
			mTBOX_TRACE((kTBOX_NIV_WARNING,"TRIG_CFG_CheckSsuLinkItem Failed line=%d Selector mismatch.\n",__LINE__));
			return eTRIG_CFG_FAILED;
		}
		else if (pSsuLinkItem->ucSelectorBytes[0]== 0x01 && cmSSU_GetTargetUpdateID() == 0xFFFFFFFF)
		{
			/* selective update does not contain target version */
			mTBOX_TRACE((kTBOX_NIV_WARNING,"TRIG_CFG_CheckSsuLinkItem Failed line=%d TargetID not set.\n",__LINE__));
			return eTRIG_CFG_FAILED;
		}
		if((pSsuLinkItem->ucSelectorBytes[0] == 0x0) || (pSsuLinkItem->ucSelectorBytes[0] == 0x1))
		{
		    *ulNominalOrSelectivePID = pSsuLinkItem->ucSelectorBytes[0];
		}
	}

    /* Check Compatibility values */
    if( (CFG_OUI == 0xFFFF) ||(pSsuLinkItem->ulOui == CFG_OUI || (pSsuLinkItem->ulOui == kSYS_INFO_DVB_OUI) ) )
    {
		mTBOX_TRACE((kTBOX_NIV_1,"TRIG_CFG_CheckSsuLinkItem eTRIG_CFG_CONTINUE line=%d\n",__LINE__));
        return eTRIG_CFG_CONTINUE;   
    }
    
	mTBOX_TRACE((kTBOX_NIV_WARNING,"TRIG_CFG_CheckSsuLinkItem Failed line=%d CFG_OUI\n",__LINE__));
    return eTRIG_CFG_FAILED;
}

/*===========================================================================
 *
 * TRIG_CFG_CheckDtgMisItem
 *
 * Parameters:
 *      IN      pDtgMisItem             : Pointer on DTG Link structure
 *      OUT     ulCurrentSoftwareVersion : Current Software version
 *      IN/OUT  ulNewSoftwareVersion     : New sofware version if returned
 *
 * Description:
 *      Check DTG Manufacturer information structure:  Manufacturer & Version Id 
 *
 * Returns:
 *          eTRIG_CFG_CONTINUE if the checking is OK,
 *          eTRIG_CFG_VERSION_UPTODATE if a version is detected and it's the current one,
 *          else eTRIG_CFG_FAILED
 *
 *===========================================================================*/
tTRIG_CFG_TriggerProcessStatus TRIG_CFG_CheckDtgMisItem(tTRIG_CFG_DtgMisItem *pDtgMisItem,
                                                         uint32_t ulCurrentSoftwareVersion,
                                                         uint32_t *ulNewSoftwareVersion)
{
    uint32_t uCfgOui;
    uint32_t uBroadcastOui;
    uint16_t uMSB_broadcastVersion;
    uint16_t uLSB_broadcastVersion;
    uint16_t uLSB_currentVersion;
    
    /* check parameters*/
    if (pDtgMisItem == NULL)
    {
		mTBOX_TRACE((kTBOX_NIV_WARNING,"TRIG_CFG_CheckDtgMisItem Failed line=%d\n",__LINE__));
	    return eTRIG_CFG_FAILED;
    }    
    	
    
    /* check OUI : Organization Unique identifier should match or DVB_OUI can be used */
    uCfgOui = TRIG_CFG_GetOUI();
    uBroadcastOui=pDtgMisItem->uiManufacturerID; 
    if ((uBroadcastOui != kSYS_INFO_DVB_OUI) && (uBroadcastOui !=uCfgOui ))
    {
    	    return eTRIG_CFG_FAILED;
    }    
    
    /* check MSB of updateId (software version ) : should match */
    uMSB_broadcastVersion = (uint16_t)(pDtgMisItem->uiVersionID >> 16);
    if ( uMSB_broadcastVersion != (uint16_t)TRIG_CFG_VERSION_ID_MSB)    
    {
    	    return eTRIG_CFG_FAILED;
    }    
    
    if (TRIG_CFGi_IsDownloaded(pDtgMisItem->uiVersionID))
    {
        return eTRIG_CFG_FAILED;
    }

    /* check LSB of current version */        
    uLSB_currentVersion = (uint16_t) ulCurrentSoftwareVersion;
    if ( uLSB_currentVersion == 0xFFFF) 
    {
            /* if current LSB version is 0xFFFF : accept any broadcasted version */
            *ulNewSoftwareVersion = pDtgMisItem->uiVersionID;
            return eTRIG_CFG_CONTINUE;               
    }
    
    uLSB_broadcastVersion = (uint16_t)(pDtgMisItem->uiVersionID);
    if ( uLSB_broadcastVersion > uLSB_currentVersion) 
    {
            /*new version */
            *ulNewSoftwareVersion = pDtgMisItem->uiVersionID;
            return eTRIG_CFG_CONTINUE;               
    }
    
    if  ( uLSB_broadcastVersion == uLSB_currentVersion) 
    {
            /* version up to date */
            return eTRIG_CFG_VERSION_UPTODATE;
    }
    /*
        else the version is out of date 
    */    
    return eTRIG_CFG_FAILED;
}

/*===========================================================================
 *
 * TRIG_CFG_CheckDataBroadcastIdDesc
 *
 * Parameters:
 *      IN      pDataBroadIdDesc         : Pointer on Data broadcast id descriptor structure
 *      OUT     ulCurrentSoftwareVersion : Current Software version
 *      IN/OUT  ulNewSoftwareVersion     : New sofware version if returned
 *
 * Description:
 *      Check data broadcast id descriptor : data_broadcast_id and private data bytes
 *
 * Returns:
 *          eTRIG_CFG_CONTINUE if the checking is OK,
 *          eTRIG_CFG_VERSION_UPTODATE if a version is detected and it's the current one,
 *          else eTRIG_CFG_FAILED
 *
 *===========================================================================*/
tTRIG_CFG_TriggerProcessStatus TRIG_CFG_CheckDataBroadcastIdDesc(tTRIG_CFG_DataBroadcastIdDesc *pDataBroadIdDesc,
                                                                 uint32_t ulCurrentSoftwareVersion,
                                                                 uint32_t *ulNewSoftwareVersion)
{
    /* Check Private data Bytes length */
    /* -> no Private data bytes */
    if (pDataBroadIdDesc->ucAddPrivateDataByteLength != 0 )
    {
		mTBOX_TRACE((kTBOX_NIV_WARNING,"TRIG_CFG_CheckDataBroadcastIdDesc Failed line=%d\n",__LINE__));
        return eTRIG_CFG_FAILED;
    }
    /* check stream type */
    if (pDataBroadIdDesc->ucStreamType != 0x0B )
    {
/*		mTBOX_TRACE((kTBOX_NIV_WARNING,"TRIG_CFG_CheckDataBroadcastIdDesc, bad stream type 0x%02x Failed line=%d\n",pDataBroadIdDesc->ucStreamType, __LINE__));*/
        return eTRIG_CFG_FAILED;
    }
    
	mTBOX_TRACE((kTBOX_NIV_1,"TRIG_CFG_CheckDataBroadcastIdDesc eTRIG_CFG_CONTINUE line=%d\n",__LINE__));
    return eTRIG_CFG_CONTINUE;
}

/*===========================================================================
 *
 * TRIG_CFG_CheckSimpleProfileSsuInfoItem
 *
 * Parameters:
 *      IN      pSsuInfoItem             : Pointer on SSU Info structure
 *      OUT     ulCurrentSoftwareVersion : Current Software version
 *      IN/OUT  ulNewSoftwareVersion     : New sofware version if returned
 *
 * Description:
 *      Check data_broadcast_id_descriptor's SSU Info for SSU Simple Profile (update_type 0x00/0x01)
 *
 * Returns:
 *          eTRIG_CFG_CONTINUE if the checking is OK,
 *          eTRIG_CFG_VERSION_UPTODATE if a version is detected and it's the current one,
 *          else eTRIG_CFG_FAILED
 *
 *===========================================================================*/
tTRIG_CFG_TriggerProcessStatus TRIG_CFG_CheckSimpleProfileSsuInfoItem(tTRIG_CFG_SsuInfoItem *pSsuInfoItem,
                                                                      uint32_t ulCurrentSoftwareVersion,
                                                                      uint32_t *ulNewSoftwareVersion)
{
    uint32_t CFG_OUI;
    uint16_t CFG_ProtocolId;
    uint16_t CFG_HW_CompModel;
    uint16_t CFG_HW_CompVersion;
    uint16_t CFG_Product_CompModel;
    uint16_t CFG_Product_CompVersion;
    uint16_t SSUInfo_ProtocolId;
    uint16_t SSUInfo_HW_CompModel;
    uint16_t SSUInfo_HW_CompVersion;
    uint16_t SSUInfo_Product_CompModel;
    uint16_t SSUInfo_Product_CompVersion;
    uint16_t SSUInfo_Usage_Id;
    uint32_t SSUInfo_SoftwareVersion;
    uint8_t  SelectorByteItemLength;
    uint16_t SelectorByteOffset;
    uint8_t  *pSelectorBytes;

    /* Get Trigger config Compatibility values */
    CFG_OUI = TRIG_CFG_GetOUI();
    CFG_Product_CompModel = TRIG_CFG_GetProductCompatibilityModel();
    CFG_Product_CompVersion = TRIG_CFG_GetProductCompatibilityVersion();
    CFG_HW_CompModel = TRIG_CFG_GetHardwareCompatibilityModel();
    CFG_HW_CompVersion = TRIG_CFG_GetHardwareCompatibilityVersion();
/*    PRINTF(("ulCurrentSoftwareVersion=%#x\n", ulCurrentSoftwareVersion));*/

    CFG_ProtocolId = TRIG_CFG_GetSSUBroadcastProtocolId();

    SelectorByteOffset = 0;
    pSelectorBytes = pSsuInfoItem->ucSelectorBytes;

	printf("##### TRIG_CFG_CheckSimpleProfileSsuInfoItem\n");

    /* browse all selector byte items */
    while(SelectorByteOffset < pSsuInfoItem->ucSelectorByteLength)
    {
        /* Get Selector bytes item values from SSU info */
        SSUInfo_ProtocolId = (pSelectorBytes[SelectorByteOffset] * 256) + pSelectorBytes[SelectorByteOffset + 1];
        SelectorByteOffset += kTRIG_CFG_PROTOCOL_ID_SIZE;
        SSUInfo_HW_CompModel = (pSelectorBytes[SelectorByteOffset] * 256) + pSelectorBytes[SelectorByteOffset + 1];
        SelectorByteOffset += kTRIG_CFG_HW_COMPATIBILITY_MODEL_SIZE;
        SSUInfo_HW_CompVersion = (pSelectorBytes[SelectorByteOffset] * 256) + pSelectorBytes[SelectorByteOffset + 1];
        SelectorByteOffset += kTRIG_CFG_HW_COMPATIBILITY_VERSION_SIZE;
        SSUInfo_Product_CompModel = (pSelectorBytes[SelectorByteOffset] * 256) + pSelectorBytes[SelectorByteOffset + 1];
        SelectorByteOffset += kTRIG_CFG_PRODUCT_COMPATIBILITY_MODEL_SIZE;
        SSUInfo_Product_CompVersion = (pSelectorBytes[SelectorByteOffset] * 256) + pSelectorBytes[SelectorByteOffset + 1];
     		if (SSUInfo_ProtocolId < 3)
     		{
				SelectorByteOffset += kTRIG_CFG_PRODUCT_COMPATIBILITY_VERSION_SIZE;
				SSUInfo_SoftwareVersion = (pSelectorBytes[SelectorByteOffset] * 256 * 256 * 256) +
										  (pSelectorBytes[SelectorByteOffset + 1] * 256 * 256) +
										  (pSelectorBytes[SelectorByteOffset + 2] * 256) +
										  (pSelectorBytes[SelectorByteOffset + 3]);

				printf("#### pSelectorBytes: %X %X %X %X\n",
						   pSelectorBytes[SelectorByteOffset],
						   pSelectorBytes[SelectorByteOffset + 1],
						   pSelectorBytes[SelectorByteOffset + 2],
						   pSelectorBytes[SelectorByteOffset + 3]);

        		SelectorByteOffset += kTRIG_CFG_SOFTWARE_VERSION_SIZE;
				mTBOX_TRACE((kTBOX_NIV_3,"TRIG_CFG_CheckSimpleProfileSsuInfoItem  SSUInfo_SoftwareVersion=%#x \n",SSUInfo_SoftwareVersion));
        	} else {
        		/* like a joker */
        		SSUInfo_SoftwareVersion = 0xFFFFFFFF;

        		printf("#### pSelectorBytes SSUInfo_SoftwareVersion: %X\n", SSUInfo_SoftwareVersion);
        	}
        	if (SSUInfo_ProtocolId > 1)
        	{
        		SSUInfo_Usage_Id = (pSelectorBytes[SelectorByteOffset]);
        		SelectorByteOffset++;
				mTBOX_TRACE((kTBOX_NIV_3,"TRIG_CFG_CheckSimpleProfileSsuInfoItem  SSUInfo_Usage_Id=%#x \n",SSUInfo_Usage_Id));
        	}
        	        	
    
     if (TRIG_CFGi_IsDownloaded(SSUInfo_SoftwareVersion))
     {
			mTBOX_TRACE((kTBOX_NIV_WARNING,"TRIG_CFGi_IsDownloaded(%#x) already downloaded\n",SSUInfo_SoftwareVersion));
        return eTRIG_CFG_FAILED;
     }

        /* Check Compatibility values */
		mTBOX_TRACE((kTBOX_NIV_3,"TRIG_CFG_CheckSimpleProfileSsuInfoItem  SSUInfo_ProtocolId=%#x \n",SSUInfo_ProtocolId));
		mTBOX_TRACE((kTBOX_NIV_3,"TRIG_CFG_CheckSimpleProfileSsuInfoItem  SSUInfo_Product_CompModel=%#x \n",SSUInfo_Product_CompModel));
		mTBOX_TRACE((kTBOX_NIV_3,"TRIG_CFG_CheckSimpleProfileSsuInfoItem  SSUInfo_Product_CompVersion=%#x \n",SSUInfo_Product_CompVersion));
		mTBOX_TRACE((kTBOX_NIV_3,"TRIG_CFG_CheckSimpleProfileSsuInfoItem  SSUInfo_HW_CompModel=%#x \n",SSUInfo_HW_CompModel));
		mTBOX_TRACE((kTBOX_NIV_3,"TRIG_CFG_CheckSimpleProfileSsuInfoItem  SSUInfo_HW_CompVersion=%#x \n",SSUInfo_HW_CompVersion));

        if ( ((pSsuInfoItem->ucUpdateType == kTRIG_CFG_STANDARD_UPDATE_TYPE) ||
             (pSsuInfoItem->ucUpdateType == kTRIG_CFG_PROPRIETARY_UPDATE_TYPE))&&
         ( (CFG_OUI == 0xFFFFFFFF) ||(pSsuInfoItem->ulOui == CFG_OUI) ) &&
   #ifdef TRIG_DVBSSU
            ( (CFG_HW_CompModel == 0xFFFF) ||(SSUInfo_HW_CompModel == CFG_HW_CompModel) ) &&
            ( (CFG_HW_CompVersion == 0xFFFF) || (SSUInfo_HW_CompVersion == CFG_HW_CompVersion) ) &&
            ( (CFG_Product_CompModel == 0xFFFF) || (SSUInfo_Product_CompModel == CFG_Product_CompModel) ) &&
            ( (CFG_Product_CompVersion == 0xFFFF) || (SSUInfo_Product_CompVersion == CFG_Product_CompVersion) ) &&
   #endif
            TRUE )
        {
            /* 
            ** Receiver matchs => check updateID (version)
            */
        if(cmSSU_GetTargetUpdateID() == 0xFFFFFFFF)
        {

        	printf("##### SSUInfo_SoftwareVersion:%X  ulCurrentSoftwareVersion:%X cmSSU_GetTargetUpdateID:%X[]\n", SSUInfo_SoftwareVersion, ulCurrentSoftwareVersion, cmSSU_GetTargetUpdateID() );

            if (SSUInfo_SoftwareVersion == 0xFFFFFFFF)
                {               
            	/* JOKER always accept 0xFFFFFFFF broadcast version */
              mTBOX_TRACE((kTBOX_NIV_3,"TRIG_CFG_CheckSimpleProfileSsuInfoItem JOKER ulCurrentSoftwareVersion-> FFFFFFFF %#x\n", 
                            ulCurrentSoftwareVersion));
                }
         	else if (ulCurrentSoftwareVersion == 0xFFFFFFFF)
                {               
            	/* JOKER accept any broadcast version when current version is 0xFFFFFFFF */
              mTBOX_TRACE((kTBOX_NIV_3,"TRIG_CFG_CheckSimpleProfileSsuInfoItem JOKER FFFFFFFF -> %#x\n", 
                            SSUInfo_SoftwareVersion));
                }
        		else if (SSUInfo_SoftwareVersion > ulCurrentSoftwareVersion) 
                {               
            	/* NOMINAL accept broadcast version greater  when current version is 0xFFFFFFFF */
              mTBOX_TRACE((kTBOX_NIV_3,"TRIG_CFG_CheckSimpleProfileSsuInfoItem NOMINAL %#x -> %#x\n", 
                      ulCurrentSoftwareVersion, SSUInfo_SoftwareVersion));
                }
        		else
                {
            	/* UPTODATE : do not accept older broadcast version */
              mTBOX_TRACE((kTBOX_NIV_3,"TRIG_CFG_CheckSimpleProfileSsuInfoItem UPTODATE %#x -> %#x\n", 
                      ulCurrentSoftwareVersion, SSUInfo_SoftwareVersion));
                    return eTRIG_CFG_VERSION_UPTODATE;
                }
        }
        else
        {

        	printf("##### SSUInfo_SoftwareVersion:%X  ulCurrentSoftwareVersion:%X cmSSU_GetTargetUpdateID:%X []\n", SSUInfo_SoftwareVersion, ulCurrentSoftwareVersion, cmSSU_GetTargetUpdateID() );

           if(cmSSU_GetTargetUpdateID() != SSUInfo_SoftwareVersion)
           {
                return eTRIG_CFG_VERSION_UPTODATE;
           }
        }
		      *ulNewSoftwareVersion = SSUInfo_SoftwareVersion;
		      /* TO DO : check usage ID with protocol ID 2 and 3 */
		      return eTRIG_CFG_CONTINUE;
        }
    }/* end while selector byte items */
	mTBOX_TRACE((kTBOX_NIV_WARNING,"TRIG_CFG_CheckSimpleProfileSsuInfoItem eTRIG_CFG_FAILED line=%d\n",__LINE__));
    return eTRIG_CFG_FAILED;
}

/*===========================================================================
 *
 * TRIG_CFG_CheckEnhancedProfileSsuInfoItem
 *
 * Parameters:
 *      IN      pSsuInfoItem             : Pointer on SSU Info structure
 *      OUT     ulCurrentSoftwareVersion : Current Software version
 *      IN/OUT  ulNewSoftwareVersion     : New sofware version if returned
 *
 * Description:
 *      Check data_broadcast_id_descriptor's SSU Info for SSU Enhanced Profile (update_type 0x02/0x03)
 *
 * Returns:
 *          eTRIG_CFG_CONTINUE if the checking is OK,
 *          eTRIG_CFG_VERSION_UPTODATE if a version is detected and it's the current one,
 *          else eTRIG_CFG_FAILED
 *
 *===========================================================================*/
tTRIG_CFG_TriggerProcessStatus TRIG_CFG_CheckEnhancedProfileSsuInfoItem(tTRIG_CFG_SsuInfoItem *pSsuInfoItem,
                                                                        uint32_t ulCurrentSoftwareVersion,
                                                                        uint32_t *ulNewSoftwareVersion)
{
    uint32_t CFG_OUI;
    
    /* Check Selector Bytes length */
    /* -> no Selector bytes */
    if (pSsuInfoItem->ucSelectorByteLength != 0 )
    {
        return eTRIG_CFG_FAILED;
    }

    /* Get Trigger config Compatibility values */
    CFG_OUI = TRIG_CFG_GetOUI();

    /* Check Compatibility values */
    if( ( (pSsuInfoItem->ucUpdateType == kTRIG_CFG_UNT_BROADCAST_UPDATE_TYPE) ||
          (pSsuInfoItem->ucUpdateType == kTRIG_CFG_UNT_CHANNEL_UPDATE_TYPE) ) &&
        ( (CFG_OUI == 0xFFFFFFFF) ||(pSsuInfoItem->ulOui == 0x00015A) || (pSsuInfoItem->ulOui == CFG_OUI) ) )
    {
        return eTRIG_CFG_CONTINUE;   
    }
    
    return eTRIG_CFG_FAILED;
}

/*===========================================================================
 *
 * TRIG_CFG_CheckUpdateDesc
 *
 * Parameters:
 *      IN      pUpdateDesc             : Pointer on Update descriptor structure
 *      OUT     ulCurrentSoftwareVersion : Current Software version
 *      IN/OUT  ulNewSoftwareVersion     : New sofware version if returned
 *
 * Description:
 *      Check SSU Enhanced Profile update_descriptor values
 *
 * Returns:
 *          eTRIG_CFG_CONTINUE if the checking is OK,
 *          eTRIG_CFG_VERSION_UPTODATE if a version is detected and it's the current one,
 *          else eTRIG_CFG_FAILED
 *
 *===========================================================================*/
tTRIG_CFG_TriggerProcessStatus TRIG_CFG_CheckUpdateDesc(tTRIG_CFG_UpdateDesc *pUpdateDesc,
                                                        uint32_t ulCurrentSoftwareVersion,
                                                        uint32_t *ulNewSoftwareVersion)
{
    uint32_t  UpdateDescSoftVersion;
    uint8_t   *pPrivateDataBytes;
    uint16_t uLSBDescVersion;
    uint16_t uLSBCurVersion;
    tTRIG_CFG_TriggerProcessStatus eStatus = eTRIG_CFG_FAILED;
 
    /* Check Private data Bytes length */
    if (pUpdateDesc->ucPrivateDataByteLength != kTRIG_CFG_SOFTWARE_VERSION_SIZE )
    {
        return eTRIG_CFG_FAILED;
    }

    /* Get Private data bytes values from Update descriptor */
    pPrivateDataBytes = pUpdateDesc->ucPrivateDataBytes;
    UpdateDescSoftVersion = (pPrivateDataBytes[0] * 256 * 256 * 256) +
                            (pPrivateDataBytes[1] * 256 * 256) +
                            (pPrivateDataBytes[2] * 256) +
                            (pPrivateDataBytes[3]);

    if (TRIG_CFGi_IsDownloaded(UpdateDescSoftVersion))
    {
       return eTRIG_CFG_FAILED;
    }

     uLSBDescVersion = (uint16_t )(UpdateDescSoftVersion&0xFFFF);
    uLSBCurVersion = (uint16_t )(ulCurrentSoftwareVersion&0xFFFF);
    if ( (UpdateDescSoftVersion>>16) != TRIG_CFG_VERSION_ID_MSB)
    {
        mTBOX_TRACE((kTBOX_NIV_WARNING,"TRIG_CFG_CheckUpdateDesc : version MSB does not match %#x, %#x\n",
                    UpdateDescSoftVersion, TRIG_CFG_VERSION_ID_MSB));
        eStatus = eTRIG_CFG_FAILED;
    }
    else if( ( uLSBDescVersion == 0xFFFF) ||
        ( uLSBCurVersion == 0xFFFF) )
    {
        mTBOX_TRACE((kTBOX_NIV_1,"TRIG_CFG_CheckUpdateDesc : Forced version %#x -> %#x\n",
                    UpdateDescSoftVersion, ulCurrentSoftwareVersion));
        *ulNewSoftwareVersion = UpdateDescSoftVersion;
        eStatus = eTRIG_CFG_CONTINUE;   
        
    }
    else if(uLSBDescVersion > uLSBCurVersion)
    {
        mTBOX_TRACE((kTBOX_NIV_1,"TRIG_CFG_CheckUpdateDesc : New version %#x > %#x\n",
                    UpdateDescSoftVersion, ulCurrentSoftwareVersion));
        *ulNewSoftwareVersion = UpdateDescSoftVersion;
        eStatus = eTRIG_CFG_CONTINUE;   
        
    }
    else if(uLSBDescVersion == uLSBCurVersion)
    {
        /* version up to date */
        mTBOX_TRACE((kTBOX_NIV_1,"TRIG_CFG_CheckUpdateDesc : uptodate version %#x = %#x\n",
                    UpdateDescSoftVersion, ulCurrentSoftwareVersion));
        eStatus = eTRIG_CFG_VERSION_UPTODATE;
    }
    else
    {
        mTBOX_TRACE((kTBOX_NIV_1,"TRIG_CFG_CheckUpdateDesc : Older version LSB  %#x < %#x\n",
                    UpdateDescSoftVersion, ulCurrentSoftwareVersion));
        eStatus = eTRIG_CFG_FAILED;
    }

    return eStatus;
}

/*===========================================================================
 *
 * TRIG_CFG_CheckSsuLocationDesc
 *
 * Parameters:
 *      IN      pSsuLocationDesc         : Pointer on SSU Location descriptor structure
 *      OUT     ulCurrentSoftwareVersion : Current Software version
 *      IN/OUT  ulNewSoftwareVersion     : New sofware version if returned
 *
 * Description:
 *      Check SSU Enhanced Profile ssu_location_descriptor values
 *
 * Returns:
 *          eTRIG_CFG_CONTINUE if the checking is OK,
 *          eTRIG_CFG_VERSION_UPTODATE if a version is detected and it's the current one,
 *          else eTRIG_CFG_FAILED
 *
 *===========================================================================*/
tTRIG_CFG_TriggerProcessStatus TRIG_CFG_CheckSsuLocationDesc(tTRIG_CFG_SsuLocationDesc *pSsuLocationDesc,
                                                             uint32_t ulCurrentSoftwareVersion,
                                                             uint32_t *ulNewSoftwareVersion)
{
    /* Check Private data Bytes length */
    if (pSsuLocationDesc->ucPrivateDataByteLength != 0 )
    {
        return eTRIG_CFG_FAILED;
    }
    
    return eTRIG_CFG_CONTINUE;
}

/*===========================================================================
 *
 * TRIG_CFG_CheckSchedulingDesc
 *
 * Parameters:
 *      IN      pSchedulingDesc          : Pointer on Scheduling descriptor structure
 *      OUT     ulCurrentSoftwareVersion : Current Software version
 *      IN/OUT  ulNewSoftwareVersion     : New sofware version if returned
 *
 * Description:
 *      Check SSU Enhanced Profile scheduling_descriptor values
 *
 * Returns:
 *          eTRIG_CFG_CONTINUE if the checking is OK,
 *          eTRIG_CFG_VERSION_UPTODATE if a version is detected and it's the current one,
 *          else eTRIG_CFG_FAILED
 *
 *===========================================================================*/
tTRIG_CFG_TriggerProcessStatus TRIG_CFG_CheckSchedulingDesc(tTRIG_CFG_SchedulingDesc *pSchedulingDesc,
                                                             uint32_t ulCurrentSoftwareVersion,
                                                             uint32_t *ulNewSoftwareVersion)
{
    /* Check Private data Bytes length */
    if (pSchedulingDesc->ucPrivateDataByteLength != 0 )
    {
        return eTRIG_CFG_FAILED;
    }
    
    return eTRIG_CFG_CONTINUE;
}

/*===========================================================================
 *
 * TRIG_CFG_CheckPrivateDataSpecifierDesc
 *
 * Parameters:
 *      IN      ulPrivateDataSpecifier   : Private Data Specifier value from descriptor
 *      OUT     ulCurrentSoftwareVersion : Current Software version
 *      IN/OUT  ulNewSoftwareVersion     : New sofware version if returned
 *
 * Description:
 *      Check SSU Enhanced Profile private data specifier value
 *
 * Returns:
 *          eTRIG_CFG_CONTINUE if the checking is OK,
 *          eTRIG_CFG_VERSION_UPTODATE if a version is detected and it's the current one,
 *          else eTRIG_CFG_FAILED
 *
 *===========================================================================*/
tTRIG_CFG_TriggerProcessStatus TRIG_CFG_CheckPrivateDataSpecifierDesc(uint32_t ulPrivateDataSpecifier,
                                                                      uint32_t ulCurrentSoftwareVersion,
                                                                      uint32_t *ulNewSoftwareVersion)
{
    return eTRIG_CFG_FAILED;
}

/*===========================================================================
 *
 * TRIG_CFG_CheckDeferredAssociationTagsDesc
 *
 * Parameters:
 *      IN      pDefAssoTagsDesc         : Pointer on Deferred association tags descriptor structure
 *      OUT     ulCurrentSoftwareVersion : Current Software version
 *      IN/OUT  ulNewSoftwareVersion     : New sofware version if returned
 *
 * Description:
 *      Check SSU Enhanced Profile deferred_association_tags_descriptor values
 *
 * Returns:
 *          eTRIG_CFG_CONTINUE if the checking is OK,
 *          eTRIG_CFG_VERSION_UPTODATE if a version is detected and it's the current one,
 *          else eTRIG_CFG_FAILED
 *
 *===========================================================================*/
tTRIG_CFG_TriggerProcessStatus TRIG_CFG_CheckDeferredAssociationTagsDesc(tTRIG_CFG_DeferredAssociationTagsDesc *pDefAssoTagsDesc,
                                                                         uint32_t ulCurrentSoftwareVersion,
                                                                         uint32_t *ulNewSoftwareVersion)
{
    /* Check Private data Bytes length */
    if (pDefAssoTagsDesc->ucPrivateDataByteLength != 0 )
    {
        return eTRIG_CFG_FAILED;
    }
    
    return eTRIG_CFG_CONTINUE;
}

/*===========================================================================
 *
 * TRIG_CFG_CheckCompatibilityDesc
 *
 * Parameters:
 *      IN      pCompDesc                : Pointer on CompatibilityDescriptor structure
 *      OUT     ulCurrentSoftwareVersion : Current Software version
 *      IN/OUT  ulNewSoftwareVersion     : New sofware version if returned
 *
 * Description:
 *      Check SSU Enhanced Profile CompatibilityDescriptor structure
 *
 * Returns:
 *          eTRIG_CFG_CONTINUE if the checking is OK,
 *          eTRIG_CFG_VERSION_UPTODATE if a version is detected and it's the current one,
 *          else eTRIG_CFG_FAILED
 *
 *===========================================================================*/
tTRIG_CFG_TriggerProcessStatus TRIG_CFG_CheckCompatibilityDesc(tTRIG_CFG_CompatibilityDesc *pCompDesc,
                                                               uint32_t ulCurrentSoftwareVersion,
                                                               uint32_t *ulNewSoftwareVersion)
{
    uint16_t CFG_HW_CompModel;
    uint16_t CFG_HW_CompVersion;
    uint16_t CFG_Product_CompModel;
    uint16_t CFG_Product_CompVersion;

    uint32_t CFG_OUI;
    /* Check subDescriptor count */
    if (pCompDesc->ucSubDescriptorCount != 0 )
    {
        return eTRIG_CFG_FAILED;
    }
    
    /* Check specifier type */
    if (pCompDesc->ucSpecifierType != 0x01)
    {
        return eTRIG_CFG_FAILED;
    }
    
    /* Get Trigger config Compatibility values */
    CFG_OUI = TRIG_CFG_GetOUI();
    CFG_Product_CompModel = TRIG_CFG_GetProductCompatibilityModel();
    CFG_Product_CompVersion = TRIG_CFG_GetProductCompatibilityVersion();
    CFG_HW_CompModel = TRIG_CFG_GetHardwareCompatibilityModel();
    CFG_HW_CompVersion = TRIG_CFG_GetHardwareCompatibilityVersion();

    /* Check specifier data (OUI) */
    if (pCompDesc->uiSpecifierData != CFG_OUI)
    {
        return eTRIG_CFG_FAILED;
    }

    /* Check sytem Hardware/Software */
    switch (pCompDesc->ucDescriptorType)
    {
        case kTRIG_CFG_SYSTEM_HARDWARE_DESC :
            
            if( ((pCompDesc->usModel == CFG_HW_CompModel) || (pCompDesc->usModel == 0xFFFF)) &&
                ((pCompDesc->usVersion == CFG_HW_CompVersion) || (pCompDesc->usVersion == 0xFFFF)) )
            {
               return eTRIG_CFG_CONTINUE;
            } 
            break;
            
        case kTRIG_CFG_SYSTEM_SOFTWARE_DESC :
        
            if( ((pCompDesc->usModel == CFG_Product_CompModel) || (pCompDesc->usModel == 0xFFFF)) &&
                ((pCompDesc->usVersion == CFG_Product_CompVersion) || (pCompDesc->usVersion == 0xFFFF)) )
            {
               return eTRIG_CFG_CONTINUE;
            } 
            break;
           
        default :
            break;
            
    }    
        
    return eTRIG_CFG_FAILED;
}

/*===========================================================================
 *
 * TRIG_CFG_CheckCompatibilitySubDesc
 *
 * Parameters:
 *      IN      pSubDesc                 : Pointer on Compatibility subDescriptor structure
 *      OUT     ulCurrentSoftwareVersion : Current Software version
 *      IN/OUT  ulNewSoftwareVersion     : New sofware version if returned
 *
 * Description:
 *      Check SSU Enhanced Profile CompatibilityDescriptor's subDescriptor (with his context)
 *
 * Returns:
 *          eTRIG_CFG_CONTINUE if the checking is OK,
 *          eTRIG_CFG_VERSION_UPTODATE if a version is detected and it's the current one,
 *          else eTRIG_CFG_FAILED
 *
 *===========================================================================*/
tTRIG_CFG_TriggerProcessStatus TRIG_CFG_CheckCompatibilitySubDesc(tTRIG_CFG_CompatibilitySubDesc *pSubDesc,
                                                                  uint32_t ulCurrentSoftwareVersion,
                                                                  uint32_t *ulNewSoftwareVersion)
{
    /* Check subDescriptor count */
    if (pSubDesc->stCompDesc.ucSubDescriptorCount != 0 )
    {
        return eTRIG_CFG_FAILED;
    }
    
    return eTRIG_CFG_CONTINUE;
}

/*===========================================================================
 *
 * TRIG_CFG_CheckSystemCompatibilitySummary
 *
 * Parameters:
 *      IN      pSysCompSummary          : Pointer on  System compatibility summary structure
 *      OUT     ulCurrentSoftwareVersion : Current Software version
 *      IN/OUT  ulNewSoftwareVersion     : New sofware version if returned
 *
 * Description:
 *      Check SSU Enhanced CompatibilityDescriptor's system compatibility Hardware/Software summary
 *
 * Returns:
 *          eTRIG_CFG_CONTINUE if the checking is OK,
 *          eTRIG_CFG_VERSION_UPTODATE if a version is detected and it's the current one,
 *          else eTRIG_CFG_FAILED
 *
 *===========================================================================*/
tTRIG_CFG_TriggerProcessStatus TRIG_CFG_CheckSystemCompatibilitySummary(tTRIG_CFG_SystemCompatibilitySummary *pSysCompSummary,
                                                                        uint32_t ulCurrentSoftwareVersion,
                                                                        uint32_t *ulNewSoftwareVersion)
{
    if( (pSysCompSummary->ucSystemHardwareCompatibilityFound >= 1) &&
        (pSysCompSummary->ucSystemSoftwareCompatibilityFound >= 1) )
    {
        return eTRIG_CFG_CONTINUE;
    }
    
    return eTRIG_CFG_FAILED;
}

/*===========================================================================
 *
 * TRIG_CFG_SmartCardTargetDescriptorMatching
 *
 * Parameters:
 *      IN      pUntTargetSmartCard          : Pointeur on Target SmartCard descriptor data
 *
 * Description:
 *      Search if the smartcard target is matching
 *
 * Returns:
 *          true if the target match,
 *          false otherwise
 *
 *===========================================================================*/
bool TRIG_CFG_SmartCardTargetDescriptorMatching (tTRIG_CFG_TargetSmartcardDescriptor *pUntTargetSmartCard)
{
	uint32_t	size = 5;
	uint8_t		pSmartcardNumber[5] ={65, 66, 67, 68, 69}; /* The SmartCard Number to match is 6566676869 */
	uint8_t		*ptr1;
	uint8_t		*ptr2;
	uint8_t		i;

	mTBOX_FCT_ENTER ("TRIG_CFG_SmartCardTargetDescriptorMatching");

	
	if ((pSmartcardNumber != NULL) && (size == pUntTargetSmartCard->ucPrivateDataLength))
	{
		ptr2 = pSmartcardNumber;
		ptr1 = pUntTargetSmartCard->pcPrivateDataByte;
		for (i=0;i<size;i++)
		{
			if (*ptr1 != *ptr2)
			{
				mTBOX_TRACE((kTBOX_NIV_FCT,"SmartCard Target doesn't match\n"));
				return false;
			}
			ptr1++;
			ptr2++;
		}
		/** The SmartCard number are matching => return true */
		mTBOX_TRACE((kTBOX_NIV_FCT,"SmartCard Target matches\n"));
		return true;
	}
	
	return false;
}

/*===========================================================================
 *
 * TRIG_CFG_MacAddressTargetDescriptorMatching
 *
 * Parameters:
 *      IN      pUntTargetMacAddress          : Pointeur on Target Mac Address descriptor data
 *
 * Description:
 *      Search if the mac address target is matching
 *
 * Returns:
 *          true if the target match,
 *          false otherwise
 *
 *===========================================================================*/
bool TRIG_CFG_MacAddressTargetDescriptorMatching (tTRIG_CFG_TargetMACAddressDescriptor *pUntTargetMacAddress)
{
	tTRIG_MacAddress		MacAddress;
	tTRIG_MacAddress		MacAddressMask;
	uint8_t					i;
	tTRIG_MacAddress		*ptr1;


	mTBOX_FCT_ENTER ("TRIG_CFG_MacAddressTargetDescriptorMatching");

	/* The Mac Address Mask to match is 112233445566 */
	MacAddressMask.byte1 = 0x11;
	MacAddressMask.byte2	= 0x22;
	MacAddressMask.byte3	= 0x33;
	MacAddressMask.byte4	= 0x44;
	MacAddressMask.byte5	= 0x55;
	MacAddressMask.byte6	= 0x66;


	/* Verification if the Mac Address Mask Match or not */
	if ((MacAddressMask.byte1	!= pUntTargetMacAddress->MACAddrMask.byte1) ||
		(MacAddressMask.byte2	!= pUntTargetMacAddress->MACAddrMask.byte2) ||
		(MacAddressMask.byte3	!= pUntTargetMacAddress->MACAddrMask.byte3) ||
		(MacAddressMask.byte4	!= pUntTargetMacAddress->MACAddrMask.byte4) ||
		(MacAddressMask.byte5	!= pUntTargetMacAddress->MACAddrMask.byte5) ||
		(MacAddressMask.byte6	!= pUntTargetMacAddress->MACAddrMask.byte6))
	{
		mTBOX_TRACE((kTBOX_NIV_FCT,"Mac Adress Target doesn't match (mask are different)\n"));
		return false;
	}
	/* The Mac Address to match is 223344556677 */
	MacAddress.byte1    = 0x22;
	MacAddress.byte2	= 0X33;
	MacAddress.byte3	= 0x44;
	MacAddress.byte4	= 0x55;
	MacAddress.byte5	= 0x66;
	MacAddress.byte6  	= 0x77;
	
	ptr1 = pUntTargetMacAddress->ptMacAddrMatch;
	/* Verification if a Mac address match or not */
	for (i=0;i<pUntTargetMacAddress->ucMacAddrMatchLength;i++)
	{
		if ((MacAddress.byte1	== ptr1->byte1) &&
			(MacAddress.byte2	== ptr1->byte2) &&
			(MacAddress.byte3	== ptr1->byte3) &&
			(MacAddress.byte4	== ptr1->byte4) &&
			(MacAddress.byte5	== ptr1->byte5) &&
			(MacAddress.byte6	== ptr1->byte6))
		{
			mTBOX_TRACE((kTBOX_NIV_FCT,"Mac Adress Target match\n"));
			return true;
		}
		ptr1++;
	}
	mTBOX_TRACE((kTBOX_NIV_FCT,"Mac Adress Target doesn't match (no address match)\n"));
	return false;
}

/*===========================================================================
 *
 * TRIG_ENH_SerialNumberTargetDescriptorMatching
 *
 * Parameters:
 *      IN      pUntTargetSerialNumber          : Pointeur on Target Serial Number descriptor data
 *
 * Description:
 *      Search if the serial number target is matching
 *
 * Returns:
 *          true if the target match,
 *          false otherwise
 *
 *===========================================================================*/
bool TRIG_CFG_SerialNumberTargetDescriptorMatching (tTRIG_CFG_TargetSerialNumberDescriptor *pUntTargetSerialNumber)
{
	uint32_t	size = 5;
	uint8_t		pSerialNumber[5] = {65, 66, 67, 68, 69}; /* The serial number to match is 6566676869 */
	uint8_t		*ptr1;
	uint8_t		*ptr2;
	uint8_t		i;

	mTBOX_FCT_ENTER ("TRIG_CFG_SerialNumberTargetDescriptorMatching");

	
	if ((pSerialNumber != NULL) && (size == pUntTargetSerialNumber->ucSerialDataByteLength))
	{
		ptr2 = pSerialNumber;
		ptr1 = pUntTargetSerialNumber->pcSerialDataByte;
		for (i=0;i<size;i++)
		{
			if (*ptr1 != *ptr2)
			{
				mTBOX_TRACE((kTBOX_NIV_FCT,"Serial Number Target doesn't match\n"));
				return false;
			}
			ptr1++;
			ptr2++;
		}
		/** The Serial number are matching => return true */
		mTBOX_TRACE((kTBOX_NIV_FCT,"Serial Number Target matches\n"));
		return true;
	}
	
	return false;
}

/*===========================================================================
 *
 * TRIG_CFG_IpAddressTargetDescriptorMatching
 *
 * Parameters:
 *      IN      pUntTargetIpAddress          : Pointeur on Target Ip Address descriptor data
 *
 * Description:
 *      Search if the Ip Address target is matching
 *
 * Returns:
 *          true if the target match,
 *          false otherwise
 *
 *===========================================================================*/
bool TRIG_CFG_IpAddressTargetDescriptorMatching (tTRIG_CFG_TargetIPAddressDescriptor *pUntTargetIpAddress)
{
	uint32_t		IpAddress;
	uint32_t		IpAddressMask;
	uint8_t			i;
	uint32_t		*ptr1;

	mTBOX_FCT_ENTER ("TRIG_CFG_IpAddressTargetDescriptorMatching");
	
	/* Ip Address Mask*/
    IpAddressMask = 0x11223344;
    
	/* Verification if the Ip Address Mask Match or not */
	if (IpAddressMask != pUntTargetIpAddress->uiIPAddrMask)
	{
		mTBOX_TRACE((kTBOX_NIV_FCT,"IP address Target doesn't match (mask are different)\n"));
		return false;
	}
	/* Ip Address */
    IpAddress = 0x22334455;
	
	ptr1 = pUntTargetIpAddress->piIPAddrMatch;
	/* Verification if a Ip address match or not */
	for (i=0;i<pUntTargetIpAddress->ucIpAddrMatchLength;i++)
	{
		if (IpAddress == *ptr1)
		{
			mTBOX_TRACE((kTBOX_NIV_FCT,"IP address Target match\n"));
			return true;
		}
	}
	mTBOX_TRACE((kTBOX_NIV_FCT,"IP address Target doesn't match (no address match)\n"));
	return false;
}

/*===========================================================================
 *
 * TRIG_CFG_IpV6AddressTargetDescriptorMatching
 *
 * Parameters:
 *      IN      pUntTargetIpV6Address          : Pointeur on Target Ip V6 Address descriptor data
 *
 * Description:
 *      Search if the Ip V6 Address target is matching
 *
 * Returns:
 *          true if the target match,
 *          false otherwise
 *
 *===========================================================================*/
bool TRIG_CFG_IpV6AddressTargetDescriptorMatching (tTRIG_CFG_TargetIPv6AddressDescriptor *pUntTargetIpV6Address)
{
	tTRIG_IPV6Address		IpV6Address;
	tTRIG_IPV6Address		IpV6AddressMask;
	uint8_t					i;
	tTRIG_IPV6Address		*ptr1;

	mTBOX_FCT_ENTER ("TRIG_CFG_IpV6AddressTargetDescriptorMatching");

	/* The Ip V6 Address Mask to match is 00112233445566778899AABBCCDDEEFF */
	IpV6AddressMask.byte1  	= 0;
	IpV6AddressMask.byte2	= 0x11;
	IpV6AddressMask.byte3	= 0x22;
	IpV6AddressMask.byte4	= 0x33;
	IpV6AddressMask.byte5	= 0x44;
	IpV6AddressMask.byte6	= 0x55;
	IpV6AddressMask.byte7	= 0x66;
	IpV6AddressMask.byte8	= 0x77;
	IpV6AddressMask.byte9	= 0x88;
	IpV6AddressMask.byte10	= 0x99;
	IpV6AddressMask.byte11	= 0xAA;
	IpV6AddressMask.byte12	= 0xBB;
	IpV6AddressMask.byte13	= 0xCC;
	IpV6AddressMask.byte14	= 0xDD;
	IpV6AddressMask.byte15	= 0xEE;
	IpV6AddressMask.byte16 	= 0xFF;

	/* Verification if the Ip V6 Address Mask Match or not */
	if ((IpV6AddressMask.byte1	!= pUntTargetIpV6Address->IPV6AddrMask.byte1) ||
		(IpV6AddressMask.byte2	!= pUntTargetIpV6Address->IPV6AddrMask.byte2) ||
		(IpV6AddressMask.byte3	!= pUntTargetIpV6Address->IPV6AddrMask.byte3) ||
		(IpV6AddressMask.byte4	!= pUntTargetIpV6Address->IPV6AddrMask.byte4) ||
		(IpV6AddressMask.byte5	!= pUntTargetIpV6Address->IPV6AddrMask.byte5) ||
		(IpV6AddressMask.byte6	!= pUntTargetIpV6Address->IPV6AddrMask.byte6) ||
		(IpV6AddressMask.byte7	!= pUntTargetIpV6Address->IPV6AddrMask.byte7) ||
		(IpV6AddressMask.byte8	!= pUntTargetIpV6Address->IPV6AddrMask.byte8) ||
		(IpV6AddressMask.byte9	!= pUntTargetIpV6Address->IPV6AddrMask.byte9) ||
		(IpV6AddressMask.byte10	!= pUntTargetIpV6Address->IPV6AddrMask.byte10) ||
		(IpV6AddressMask.byte11	!= pUntTargetIpV6Address->IPV6AddrMask.byte11) ||
		(IpV6AddressMask.byte12	!= pUntTargetIpV6Address->IPV6AddrMask.byte12) ||
		(IpV6AddressMask.byte13	!= pUntTargetIpV6Address->IPV6AddrMask.byte13) ||
		(IpV6AddressMask.byte14	!= pUntTargetIpV6Address->IPV6AddrMask.byte14) ||
		(IpV6AddressMask.byte15	!= pUntTargetIpV6Address->IPV6AddrMask.byte15) ||
		(IpV6AddressMask.byte16	!= pUntTargetIpV6Address->IPV6AddrMask.byte16))
	{
		mTBOX_TRACE((kTBOX_NIV_FCT,"IP V6 address Target doesn't match (mask are different)\n"));
		return false;
	}
	/* The Ip V6 Address to match is 112233445566778899AABBCCDDEEFF00 */
	IpV6Address.byte1   = 0x11;
	IpV6Address.byte2	= 0x22;
	IpV6Address.byte3	= 0x33;
	IpV6Address.byte4	= 0x44;
	IpV6Address.byte5	= 0x55;
	IpV6Address.byte6	= 0x66;
	IpV6Address.byte7	= 0x77;
	IpV6Address.byte8	= 0x88;
	IpV6Address.byte9	= 0x99;
	IpV6Address.byte10	= 0xAA;
	IpV6Address.byte11	= 0xBB;
	IpV6Address.byte12	= 0xCC;
	IpV6Address.byte13  = 0xDD;
	IpV6Address.byte14	= 0xEE;
	IpV6Address.byte15	= 0xFF;
	IpV6Address.byte16 	= 0x00;

	ptr1 = pUntTargetIpV6Address->ptIPV6AddrMatch;
	/* Verification if a Ip V6 address match or not */
	for (i=0;i<pUntTargetIpV6Address->ucIpV6AddrMatchLength;i++)
	{
		if ((IpV6Address.byte1	 == ptr1->byte1) &&
			(IpV6Address.byte2	 == ptr1->byte2) &&
			(IpV6Address.byte3	 == ptr1->byte3) &&
			(IpV6Address.byte4	 == ptr1->byte4) &&
			(IpV6Address.byte5	 == ptr1->byte5) &&
			(IpV6Address.byte6	 == ptr1->byte6) &&
			(IpV6Address.byte7	 == ptr1->byte7) &&
			(IpV6Address.byte8	 == ptr1->byte8) &&
			(IpV6Address.byte9	 == ptr1->byte9) &&
			(IpV6Address.byte10	 == ptr1->byte10) &&
			(IpV6Address.byte11	 == ptr1->byte11) &&
			(IpV6Address.byte12	 == ptr1->byte12) &&
			(IpV6Address.byte13	 == ptr1->byte13) &&
			(IpV6Address.byte14	 == ptr1->byte14) &&
			(IpV6Address.byte15	 == ptr1->byte15) &&
			(IpV6Address.byte16	 == ptr1->byte16))
		{
			mTBOX_TRACE((kTBOX_NIV_FCT,"IP V6 address Target match\n"));
			return true;
		}
	}
	mTBOX_TRACE((kTBOX_NIV_FCT,"IP V6 address Target doesn't match (no address match)\n"));
	return false;
}

/*===========================================================================
 *
 * TRIG_CFG_PrivateTargetDescriptorMatching
 *
 * Parameters:
 *      IN      pUntPrivateAddress          : Pointeur on Target Private data specifier descriptor data
 *
 * Description:
 *      Search if the private data specifier target is matching
 *
 * Returns:
 *          true if the target match,
 *          false otherwise
 *
 *===========================================================================*/
bool TRIG_CFG_PrivateTargetDescriptorMatching (tTRIG_CFG_TargetPrivateDataDescriptor *pUntPrivateAddress)
{
	uint32_t PrivateDataSpecifier;

	mTBOX_FCT_ENTER ("TRIG_CFG_PrivateTargetDescriptorMatching");
	/* Private Target*/
    PrivateDataSpecifier = 0x11223344;

	/* Verification if Private Data Specifier match or not */
	if (pUntPrivateAddress->uiPrivateDataSpecifier == PrivateDataSpecifier)
	{
		
		mTBOX_TRACE((kTBOX_NIV_FCT,"Private Data Specifier Target match\n"));
		return true;
	}
	else
	{
		mTBOX_TRACE((kTBOX_NIV_FCT,"Private Data Specifier Target doesn't match\n"));
		return false;
	}
}

#ifdef TRIG_USB
/*===========================================================================
 *
 * TRIG_CFG_Get_SSU_Usb_FileName
 *
 * Parameters:
 *      None
 *
 * Description:
 *      
 *
 * Returns:
 *
 *===========================================================================*/
char* TRIG_CFG_Get_SSU_Usb_FileName(void)
{
    return ((char*)kTRIG_CFG_SSUUSB_FILE_NAME);
}



/*===========================================================================
 *
 * TRIG_CFG_Is_Downloader_Automatic_Usb_Download_enabled
 *
 * Parameters:
 *      None
 *
 * Description:
 *      
 *
 * Returns:
 *
 *===========================================================================*/
bool TRIG_CFG_Is_Downloader_Automatic_Usb_Download_enabled(void)
{
#ifdef kDLD_CFG_USB_DOWNLOAD_ENABLE_AUTOMATIC_MODE
    return true;
#else
    return false;
#endif
}
#endif /* TRIG_USB */

/********************************************************/
/*              Local Functions Implementation          */
/********************************************************/

/*===========================================================================
 *
 * TRIG_CFGi_IsDownloaded
 *
 * Parameters:
 *      uiSoftVersion - Software version
 *
 * Description:
 *      Read the NVM to determin if a new software version is already
 *      downloaded by the downloader (to not rearm its donwloading)
 *
 * Returns:
 *     TRUE:  Yes already downloaded
 *     FALSE: No
 *
 *===========================================================================*/
LOCAL bool TRIG_CFGi_IsDownloaded (uint32_t uiSoftVersion)
{
	/* OBSOLETE FUNCTION */
   return FALSE;
}





/*===========================================================================
 *
 * TRIG_CFGi_AccessBAT
 *
 * Parameters:
 *      bWrite IN
 *      ppstSectionTable IN/OUT
 *
 * Description: 
 *      This function is used to store and retrieve the BAT section 
 *      Typical usage is to store the BAT that is acquired for the channel update
 *      that is performed at wake up and provide it to the trigger that is done
 *      just after.
 *      The section that is stored with this mechanism shall not be freed by the
 *      caller. It will be freed with a new call to TRIG_CFG_SetBAT.
 *
 * Returns:
 *      Nothing
 *
 *===========================================================================*/
void TRIG_CFGi_AccessBAT(bool bWrite, tTable_Section** ppstSectionTable)
{
    static tTable_Section* pstCurrentSectionTable = NULL;
    
    if (bWrite == FALSE)
    {
        /* read access */
        *ppstSectionTable = pstCurrentSectionTable;
        
    }
    else
    {
        /* write access */
        if (pstCurrentSectionTable != NULL)
        {
            TABLE_Free_Buffer(pstCurrentSectionTable);
        }
        
        pstCurrentSectionTable = *ppstSectionTable;
    }
    
    return;
}




/*===========================================================================
 *
 * TRIG_CFG_SetBAT
 *
 * Parameters:
 *      ppstSectionTable IN
 *
 * Description: 
 *      This function is used to store the BAT section 
 *      Typical usage is to store the BAT that is acquired for the channel update
 *      that is performed at wake up and provide it to the trigger that is done
 *      just after
 *      The section that is stored with this mechanism shall not be freed by the
 *      caller. It will be freed with a new call to TRIG_CFG_SetBAT.
 *
 * Returns:
 *      Nothing
 *
 *===========================================================================*/
void TRIG_CFG_SetBAT(void** ppstSectionTable)
{
    TRIG_CFGi_AccessBAT(TRUE, (tTable_Section**)ppstSectionTable);
    *ppstSectionTable = NULL;
}

/*===========================================================================
 *
 * TRIG_CFG_GetBAT
 *
 * Parameters:
 *      ppstSectionTable OUT
 *
 * Description: 
 *      This function is used to retrieve the BAT section 
 *      Typical usage is to store the BAT that is acquired for the channel update
 *      that is performed at wake up and provide it to the trigger that is done
 *      just after
 *      The section that is stored with this mechanism shall not be freed by the
 *      caller. It will be freed with a new call to TRIG_CFG_SetBAT.
 *
 * Returns:
 *      Nothing
 *
 *===========================================================================*/
void TRIG_CFG_GetBAT(void** ppstSectionTable)
{
    TRIG_CFGi_AccessBAT(FALSE, (tTable_Section**)ppstSectionTable);
}
