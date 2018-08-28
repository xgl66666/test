/******************************************************************************
 *                    COPYRIGHT 2004 IWEDIA TECHNOLOGIES                      *
 ******************************************************************************
 *
 * MODULE NAME: TRIGGER CFG
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/product/comedia/branches/idecode_hd/build/inc/trigger_cfg.h $
 *            $Rev: 332 $
 *            $Date: 2011-09-07 13:51:23 +0200 (Wed, 07 Sep 2011) $
 *
 * PUBLIC
 *
 * DESCRIPTION: trigger configuration header
 *
 *****************************************************************************/

#ifndef _TRIGGER_CFG_H_
#define _TRIGGER_CFG_H_

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************/
/*              Defines                                */
/*******************************************************/

/*******************************************************/
/*               typedef                               */
/*******************************************************/

/* MEDIUM_SAT */
#if defined (MEDIUM_SAT)
#define kTRIG_CFG_MEDIUM_SAT

/* MEDIUM_CAB */
#elif defined (MEDIUM_CAB)
#define kTRIG_CFG_MEDIUM_CAB

/* MEDIUM_TER */
#elif defined (MEDIUM_TER)
#define kTRIG_CFG_MEDIUM_TER

#endif

typedef enum
{
    eTRIG_CFG_FAILED,           /* Checking KO */
    eTRIG_CFG_CONTINUE,         /* Checking OK */
    eTRIG_CFG_VERSION_UPTODATE=3 /* Version up to date (sync with eTRIG_UP_TO_DATE)*/
}tTRIG_CFG_TriggerProcessStatus;

typedef struct
{
    uint16_t usTransportStreamID;
	uint16_t usOriginalNetworkID;
	uint16_t usServiceID;
} tTRIG_CFG_DVBTriplet;

typedef struct
{
    tTRIG_CFG_DVBTriplet    stDVBTriplet;
    uint8_t                 *ucAdditionalPrivateDataBytes;
	uint8_t                 ucLinkageType;
    uint8_t                 ucAddPrivateDataByteLength;
} tTRIG_CFG_LinkageDesc;

typedef struct
{
    uint8_t   *ucSelectorBytes;
    uint32_t  ulOui;
    uint8_t   ucSelectorByteLength;
} tTRIG_CFG_SsuLinkItem;

typedef struct
{
	uint32_t uiManufacturerID;
	uint32_t uiVersionID;
	uint32_t uiPrivateDataLength;
	uint8_t * pPrivateData;
} tTRIG_CFG_DtgMisItem;

typedef struct
{
    uint8_t   *ucAdditionalPrivateDataBytes;
	uint16_t  usDataBroadcastId;
    uint8_t   ucAddPrivateDataByteLength;
    uint8_t ucStreamType;
} tTRIG_CFG_DataBroadcastIdDesc;

typedef struct
{
    uint8_t   *ucSelectorBytes;
    uint32_t  ulOui;
    uint8_t   ucUpdateType;
    uint8_t   ucUpdateVersioningFlag;
    uint8_t   ucUpdateVersion;
    uint8_t   ucSelectorByteLength;
} tTRIG_CFG_SsuInfoItem;

typedef struct
{
    uint8_t   *ucPrivateDataBytes;
    uint8_t   ucUpdateFlag;
    uint8_t   ucUpdateMethod;
    uint8_t   ucUpdatePriority;
    uint8_t   ucPrivateDataByteLength;
} tTRIG_CFG_UpdateDesc;

typedef struct
{
    uint8_t   *ucPrivateDataBytes;
    uint16_t  usDataBroadcastId;
    uint16_t  usAssociationTag;
    uint8_t   ucPrivateDataByteLength;
} tTRIG_CFG_SsuLocationDesc;

typedef struct
{
    uint16_t  Year;
    uint8_t	  Month;
    uint8_t	  Day;
	
} tTRIG_CFG_Date;


typedef struct
{
    uint8_t  Hour;		/* 0..23 */
    uint8_t  Minute;
    uint8_t  Second;
	
} tTRIG_CFG_Time;

typedef struct
{
    tTRIG_CFG_Date	stStartDate;
    tTRIG_CFG_Time	stStartTime;
    tTRIG_CFG_Date	stEndDate;
    tTRIG_CFG_Time	stEndTime;
    uint8_t         *ucPrivateDataBytes;
    uint8_t			ucFinalAvailability;
    uint8_t			ucPeriodicityFlag;
    uint8_t			ucPeriodUnit;
    uint8_t			ucDurationUnit;
    uint8_t			ucEstimatedCycleTimeUnit;
    uint8_t			ucPeriod ;
    uint8_t			ucDuration;
    uint8_t			ucEstimatedCycleTime;
    uint8_t         ucPrivateDataByteLength;
} tTRIG_CFG_SchedulingDesc;

typedef struct
{
    uint16_t  *usAssociationTagsLoop;
    uint8_t   *ucPrivateDataBytes;
    uint16_t  usTransportStreamID;
	uint16_t  usProgramNumber;
    uint8_t   ucAssociationTagsLoopLength;
    uint8_t   ucPrivateDataByteLength;
} tTRIG_CFG_DeferredAssociationTagsDesc;

typedef struct
{
    uint8_t   ucDescriptorType;
    uint8_t   ucSpecifierType;
    uint32_t  uiSpecifierData;
    uint16_t  usModel;
    uint16_t  usVersion;
    uint8_t   ucSubDescriptorCount;
} tTRIG_CFG_CompatibilityDesc;

typedef struct
{
    tTRIG_CFG_CompatibilityDesc   stCompDesc;
    uint8_t                       *ucSubDescriptorData;
    uint8_t                       ucSubDescriptorIndex;
    uint8_t                       ucSubDescriptorType;
    uint8_t                       ucSubDescriptorLength;
} tTRIG_CFG_CompatibilitySubDesc;

typedef struct
{
    uint8_t ucSystemHardwareCompatibilityFound;
    uint8_t ucSystemHardwareCompatibilityFailed;
    uint8_t ucSystemSoftwareCompatibilityFound;
    uint8_t ucSystemSoftwareCompatibilityFailed;
} tTRIG_CFG_SystemCompatibilitySummary;

/* TARGETS */

typedef struct
{
	uint8_t byte1;	/* MSB Byte */
	uint8_t byte2;
	uint8_t byte3;
	uint8_t byte4;
	uint8_t byte5;
	uint8_t byte6;	/* LSB Byte */
	
} tTRIG_MacAddress;

typedef struct
{
	uint8_t byte1;		/* MSB Byte */
	uint8_t byte2;
	uint8_t byte3;
	uint8_t byte4;
	uint8_t byte5;
	uint8_t byte6;
	uint8_t byte7;
	uint8_t byte8;
	uint8_t byte9;
	uint8_t byte10;
	uint8_t byte11;
	uint8_t byte12;
	uint8_t byte13;
	uint8_t byte14;
	uint8_t byte15;
	uint8_t byte16;	/* LSB Byte */
} tTRIG_IPV6Address;

/* Target Smartcard Descriptor */
typedef struct
{
	uint32_t	    uiSuperCASystemId;
	uint8_t			*pcPrivateDataByte;
	uint8_t			ucPrivateDataLength;
} tTRIG_CFG_TargetSmartcardDescriptor;

/* Target Mac Address Descriptor */
typedef struct
{
	tTRIG_MacAddress	MACAddrMask;
	tTRIG_MacAddress	*ptMacAddrMatch;
	uint8_t				ucMacAddrMatchLength;
} tTRIG_CFG_TargetMACAddressDescriptor;

/* Target IP Address Descriptor */
typedef struct
{
	uint32_t	uiIPAddrMask;
	uint32_t	*piIPAddrMatch;
	/* IP address Match Array of uint32_t */
	uint8_t		ucIpAddrMatchLength;
} tTRIG_CFG_TargetIPAddressDescriptor;


/* Target Serial Number Descriptor */
typedef struct
{
	uint8_t		*pcSerialDataByte;
	uint8_t		ucSerialDataByteLength;
} tTRIG_CFG_TargetSerialNumberDescriptor;


/* Target IP V6 Address Descriptor */
typedef struct
{
	tTRIG_IPV6Address	IPV6AddrMask;
	tTRIG_IPV6Address	*ptIPV6AddrMatch;
	/* IP V6 address Match Array of tTRIG_UNT_IPV6_Adress */
	uint8_t				ucIpV6AddrMatchLength;
} tTRIG_CFG_TargetIPv6AddressDescriptor;

/* Private Data Descriptor */
typedef struct
{
	uint32_t	uiPrivateDataSpecifier;
} tTRIG_CFG_TargetPrivateDataDescriptor;

/*******************************************************/
/*              Functions Declarations                 */
/*******************************************************/

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
bool TRIG_CFG_StartSsuSearchInBat(void);

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
bool TRIG_CFG_NetworkNameSelectionEnabled(void);

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
void TRIG_CFG_GetTriggerNetworkName(char* pNetworkName);

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
uint32_t TRIG_CFG_GetOUI(void);

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
uint16_t TRIG_CFG_GetSSUBroadcastProtocolId(void);

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
uint16_t TRIG_CFG_GetProductCompatibilityModel(void);

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
uint16_t TRIG_CFG_GetProductCompatibilityVersion(void);

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
uint16_t TRIG_CFG_GetHardwareCompatibilityModel(void);

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
uint16_t TRIG_CFG_GetHardwareCompatibilityVersion(void);

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
uint8_t TRIG_CFG_GetProductUsageId(void);

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
                                                      uint8_t  *ucProductUsageId);


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
tTRIG_CFG_TriggerProcessStatus TRIG_CFG_CheckDVBTriplet(tTRIG_CFG_DVBTriplet *pDVBTriplet);

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
                                                         uint32_t *ulNewSoftwareVersion);

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
                                                         uint16_t *ulNominalOrSelectivePID);

/*===========================================================================
 *
 * TRIG_CFG_CheckDtgLinkItem
 *
 * Parameters:
 *      IN      pDtgLinkItem             : Pointer on DTG Link structure
 *      OUT     ulCurrentSoftwareVersion : Current Software version
 *      IN/OUT  ulNewSoftwareVersion     : New sofware version if returned
 *
 * Description:
 *      Check linkage_type 0x81 linkage_descriptor's DTG Manufacturer/Version Id
 *
 * Returns:
 *          eTRIG_CFG_CONTINUE if the checking is OK,
 *          eTRIG_CFG_VERSION_UPTODATE if a version is detected and it's the current one,
 *          else eTRIG_CFG_FAILED
 *
 *===========================================================================*/
tTRIG_CFG_TriggerProcessStatus TRIG_CFG_CheckDtgMisItem(tTRIG_CFG_DtgMisItem *pDtgMisItem,
                                                         uint32_t ulCurrentSoftwareVersion,
                                                         uint32_t *ulNewSoftwareVersion);

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
                                                                 uint32_t *ulNewSoftwareVersion);

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
                                                                      uint32_t *ulNewSoftwareVersion);

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
                                                                        uint32_t *ulNewSoftwareVersion);

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
                                                        uint32_t *ulNewSoftwareVersion);

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
                                                             uint32_t *ulNewSoftwareVersion);

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
                                                             uint32_t *ulNewSoftwareVersion);

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
                                                                      uint32_t *ulNewSoftwareVersion);

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
                                                                         uint32_t *ulNewSoftwareVersion);

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
                                                               uint32_t *ulNewSoftwareVersion);

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
                                                                  uint32_t *ulNewSoftwareVersion);

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
                                                                        uint32_t *ulNewSoftwareVersion);


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
bool TRIG_CFG_SmartCardTargetDescriptorMatching (tTRIG_CFG_TargetSmartcardDescriptor *pUntTargetSmartCard);

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
bool TRIG_CFG_MacAddressTargetDescriptorMatching (tTRIG_CFG_TargetMACAddressDescriptor *pUntTargetMacAddress);

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
bool TRIG_CFG_SerialNumberTargetDescriptorMatching (tTRIG_CFG_TargetSerialNumberDescriptor *pUntTargetSerialNumber);

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
bool TRIG_CFG_IpAddressTargetDescriptorMatching (tTRIG_CFG_TargetIPAddressDescriptor *pUntTargetIpAddress);

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
bool TRIG_CFG_IpV6AddressTargetDescriptorMatching (tTRIG_CFG_TargetIPv6AddressDescriptor *pUntTargetIpV6Address);

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
bool TRIG_CFG_PrivateTargetDescriptorMatching (tTRIG_CFG_TargetPrivateDataDescriptor *pUntPrivateAddress);


void TRIG_CFG_SetBAT(void** ppstSectionTable);
void TRIG_CFG_GetBAT(void** ppstSectionTable);

#ifdef TRIG_USB
char* TRIG_CFG_Get_SSU_Usb_FileName(void);
bool TRIG_CFG_Is_Downloader_Automatic_Usb_Download_enabled(void);
#endif

#ifdef __cplusplus
}
#endif

#endif /* _TRIGGER_CFG_H_ */
