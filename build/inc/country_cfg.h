/******************************************************************************
 *                           COPYRIGHT 2005 IWEDIA                            *
 ******************************************************************************
 *
 * MODULE NAME: CFG COUNTRY
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/product/comedia/branches/idecode_hd/build/inc/country_cfg.h $
 *            $Rev: 244 $
 *            $Date: 2011-08-18 18:33:39 +0200 (Thu, 18 Aug 2011) $
 *
 * PUBLIC
 *
 * DESCRIPTION
 *
 *****************************************************************************/

#ifndef _COUNTRY_CFG_H_
#define _COUNTRY_CFG_H_

#ifdef __cplusplus
extern "C" {
#endif


/*******************************************************/
/*               define                                */
/*******************************************************/
#define kLANGUAGE_NB_CHAR_BY_TRIGRAM       4

/*******************************************************/
/*               typedef                               */
/*******************************************************/
typedef enum
{
    eCOUNTRY_CFG_LCN_TYPE_EACEM,
    eCOUNTRY_CFG_LCN_TYPE_EACEM_AND_HD_SIMULCAST_MANAGEMENT,
    eCOUNTRY_CFG_LCN_TYPE_DBOOK,
    eCOUNTRY_CFG_LCN_TYPE_NORDIG,
    eCOUNTRY_CFG_LCN_TYPE_NORDIG_V2_COUNTRY_NORWAY,
    eCOUNTRY_CFG_LCN_TYPE_NORDIG_V2_COUNTRY_SWEDEN,
    eCOUNTRY_CFG_LCN_TYPE_NORDIG_V2_COUNTRY_FINLAND,
    eCOUNTRY_CFG_LCN_TYPE_NORDIG_V2_COUNTRY_DENMARK,
    eCOUNTRY_CFG_LCN_TYPE_NORDIG_V2_COUNTRY_ICELAND,
    eCOUNTRY_CFG_LCN_TYPE_NORDIG_V2_COUNTRY_ESTONIA,
    eCOUNTRY_CFG_LCN_TYPE_NORDIG_V2_COUNTRY_IRELAND
} tCOUNTRY_CFG_LCN_Type;

typedef enum
{
    eCOUNTRY_CFG_TRIGGER_TYPE_DVB_SSU,
    eCOUNTRY_CFG_TRIGGER_TYPE_DVB_SSU_ENHANCED,
    eCOUNTRY_CFG_TRIGGER_TYPE_DTG,
    eCOUNTRY_CFG_TRIGGER_TYPE_NONE
} tCOUNTRY_CFG_Trigger_Type;

typedef enum
{
    eCOUNTRY_CFG_SERVICE_IDENTIFIER_ATW, /* Analog Any medium Worlwide */
    eCOUNTRY_CFG_SERVICE_IDENTIFIER_ACA, /* ATSC Cable WorldWide */
    eCOUNTRY_CFG_SERVICE_IDENTIFIER_ATA, /* ATSC Ter WorldWide */
	eCOUNTRY_CFG_SERVICE_IDENTIFIER_DAE, /* DVB Any Medium ETSI */
	eCOUNTRY_CFG_SERVICE_IDENTIFIER_DAF, /* DVB Any Medium Use Frequency */
	eCOUNTRY_CFG_SERVICE_IDENTIFIER_DSW, /* DVB Sat Worldwide */
	eCOUNTRY_CFG_SERVICE_IDENTIFIER_DTN, /* DVB Ter Nordig */
	eCOUNTRY_CFG_SERVICE_IDENTIFIER_DIL, /* DVB IP Live IP */
	eCOUNTRY_CFG_SERVICE_IDENTIFIER_DTF, /* DVB Ter Use Frequency */
	eCOUNTRY_CFG_SERVICE_IDENTIFIER_M2IL,/* MPEG2 IP Live IP */
	eCOUNTRY_CFG_SERVICE_IDENTIFIER_M2IV,/* MPEG2 IP VOD */
	eCOUNTRY_CFG_SERVICE_IDENTIFIER_M2PW /* MPEG2 PVR Worldwide */
} tCOUNTRY_CFG_ServiceIdentifierMode;

typedef enum
{
    eCOUNTRY_CFG_PROG_STATUS_NOT_CHECKED       = 0,
    eCOUNTRY_CFG_PROG_STATUS_IN_PROGRESS       = 1,
    eCOUNTRY_CFG_PROG_STATUS_PLANNED           = 2,
    eCOUNTRY_CFG_PROG_STATUS_CONFLICTED        = 3,
    eCOUNTRY_CFG_PROG_STATUS_NO_SPACE          = 4,
    eCOUNTRY_CFG_PROG_STATUS_SERIE             = 5,
    eCOUNTRY_CFG_PROG_STATUS_HDD_NOT_CONNECTED = 6,
    eCOUNTRY_CFG_PROG_STATUS_HDD_NOT_READY     = 7,
    eCOUNTRY_CFG_PROG_STATUS_RECORD_ERROR_UNKNOWN      			= 8,
    eCOUNTRY_CFG_PROG_STATUS_FORMAT_ERROR      = 9,
    eCOUNTRY_CFG_PROG_STATUS_RECORD_ERROR_INTHEPAST_1      		= 10,
    eCOUNTRY_CFG_PROG_STATUS_RECORD_ERROR_INTHEPAST_2      		= 11,
    eCOUNTRY_CFG_PROG_STATUS_RECORD_ERROR_INTHEPAST_3      		= 12,
    eCOUNTRY_CFG_PROG_STATUS_RECORD_ERROR_NO_FREE_INSTANCE      = 13,
    eCOUNTRY_CFG_PROG_STATUS_RECORD_ERROR_START_FAILED      	= 14,
    eCOUNTRY_CFG_PROG_STATUS_RECORD_ERROR_NO_SIGNAL      		= 15,
    eCOUNTRY_CFG_PROG_STATUS_RECORD_ERROR_NO_SERVICE_CONNECTED  = 16,
    eCOUNTRY_CFG_PROG_STATUS_UNKNOWN           = 17
} tCOUNTRY_CFG_ProgStatus;


typedef enum
{
    eCOUNTRY_CFG_PROG_TARGET_NONE              = 0,
    eCOUNTRY_CFG_PROG_TARGET_HDD               = 1,
    eCOUNTRY_CFG_PROG_TARGET_VCR               = 2,
    eCOUNTRY_CFG_PROG_TARGET_REMINDER          = 3,
    eCOUNTRY_CFG_PROG_TARGET_EXTERNAL_HDD      = 4,
    eCOUNTRY_CFG_PROG_TARGET_UNKNWON           = 5
} tCOUNTRY_CFG_ProgTarget;

typedef enum
{
    eCOUNTRY_CFG_CHAR_TABLE_ISO_IEC_6937,
    eCOUNTRY_CFG_CHAR_TABLE_ISO_IEC_8859_1,
    eCOUNTRY_CFG_CHAR_TABLE_ISO_IEC_8859_2,
    eCOUNTRY_CFG_CHAR_TABLE_ISO_IEC_8859_3,
    eCOUNTRY_CFG_CHAR_TABLE_ISO_IEC_8859_4,
    eCOUNTRY_CFG_CHAR_TABLE_ISO_IEC_8859_5,
    eCOUNTRY_CFG_CHAR_TABLE_ISO_IEC_8859_6,
    eCOUNTRY_CFG_CHAR_TABLE_ISO_IEC_8859_7,
    eCOUNTRY_CFG_CHAR_TABLE_ISO_IEC_8859_8,
    eCOUNTRY_CFG_CHAR_TABLE_ISO_IEC_8859_9,
    eCOUNTRY_CFG_CHAR_TABLE_ISO_IEC_8859_10,
    eCOUNTRY_CFG_CHAR_TABLE_ISO_IEC_8859_11,
    eCOUNTRY_CFG_CHAR_TABLE_ISO_IEC_8859_13,
    eCOUNTRY_CFG_CHAR_TABLE_ISO_IEC_8859_14,
    eCOUNTRY_CFG_CHAR_TABLE_ISO_IEC_8859_15
} tCOUNTRY_CFG_CharacterTable;

typedef struct
{
    char    DVBtrigramFirst[4];
    char    DVBtrigramSecond[4];
    char    EntireName[16];
}tCOUNTRY_CFG_Language;

typedef struct
{
	uint32_t	ONID;
	uint32_t	SID;
}tCOUNTRY_CFG_RankingList;

/* Bitmap for installation */
#define kCOUNTRY_CFG_INSTALL_USE_LCN_IF_PDSD_OK         0x00000000
#define kCOUNTRY_CFG_INSTALL_USE_LCN_EVEN_WITH_NO_PDSD  0x00000001
#define kCOUNTRY_CFG_INSTALL_LCN_0_NOT_SELECTABLE       0x00000002
#define kCOUNTRY_CFG_INSTALL_ONID_PRIVATE_TMP_USE       0x00000004
#define kCOUNTRY_CFG_INSTALL_NID_PRIVATE_TMP_USE        0x00000008

#define kCOUNTRY_CFG_INSTALL_DEFAULT                    kCOUNTRY_CFG_INSTALL_USE_LCN_EVEN_WITH_NO_PDSD


/* Bitmap for implementation of DVB standard specifics */
#define kCOUNTRY_CFG_EIT_PRESENT_FOLLOWING_FLAG     0x0001
#define kCOUNTRY_CFG_EIT_SCHEDULE_FLAG              0x0002
#define kCOUNTRY_CFG_EIT_LAST_SECTION_NUMBER        0x0004
#define kCOUNTRY_CFG_EIT_LAST_TABLE_ID              0x0008
#define kCOUNTRY_CFG_EIT_EMPTY_SEGMENT              0x0010
#define kCOUNTRY_CFG_EIT_LAST_TABLE_TIMEOUT         0x0020
#define kCOUNTRY_CFG_EIT_PRESENT_FOLLOWING_OTHER    0x0100
#define kCOUNTRY_CFG_EIT_SCHEDULE_OTHER             0x0200

typedef struct
{
    uint16_t    flags;
    uint8_t     EITpfActualRepetitionRate;
    uint8_t     EITpfOtherRepetitionRate;
    uint16_t    EITschActualRepetitionRate;
    uint16_t    EITschOtherRepetitionRate;
    uint8_t     unavailableEITschSegmentsThreshold;
}tCOUNTRY_CFG_EIT_Requirements;

typedef enum
{
    eCOUNTRY_CFG_PERMANENT_SI_MONITORING_NONE,
    eCOUNTRY_CFG_PERMANENT_SI_MONITORING_ON_TS,
    eCOUNTRY_CFG_PERMANENT_SI_MONITORING_ON_NETWORK,
    eCOUNTRY_CFG_PERMANENT_SI_MONITORING_ON_COMPLETE_NETWORK    /* SDT other also */
}eCOUNTRY_CFG_PermanentSiMonitoringMode;

/*===========================================================================
 *
 * COUNTRY_CFG_GetNumberOfCountries
 *
 * Parameters:
 *      none
 *
 * Description:
 *      Return the number of available countries
 *
 * Returns:
 *      number of countries
 *
 *===========================================================================*/
uint32_t COUNTRY_CFG_GetNumberOfCountries(void);

/*===========================================================================
 *
 * COUNTRY_CFG_GetAvailableCountryName
 *
 * Parameters:
 *      index
 *
 * Description:
 *      Return the name of available countries
 *
 * Returns:
 *      name of available country
 *
 *===========================================================================*/
char* COUNTRY_CFG_GetAvailableCountryName(uint32_t index);

/*===========================================================================
 *
 * COUNTRY_CFG_GetAvailableCountryDVBtrigram
 *
 * Parameters:
 *      index
 *
 * Description:
 *      Return the DVB trigram of available countries
 *
 * Returns:
 *      name of available DVB trigram
 *
 *===========================================================================*/
char* COUNTRY_CFG_GetAvailableCountryDVBtrigram(uint32_t index);

/*===========================================================================
 *
 * COUNTRY_CFG_GetDefaultCountryDVBtrigram
 *
 * Parameters:
 *      none
 *
 * Description:
 *      Return the DVB trigram of the default country
 *
 * Returns:
 *      DVB trigram of the default country
 *
 *===========================================================================*/
char* COUNTRY_CFG_GetDefaultCountryDVBtrigram(void);

/*===========================================================================
 *
 * COUNTRY_CFG_GetDefaultMenuLanguage
 *
 * Parameters:
 *      none
 *
 * Description:
 *      Return the default menu language
 *
 * Returns:
 *      trigram of the default menu language
 *
 *===========================================================================*/
char* COUNTRY_CFG_GetDefaultMenuLanguage(void);

/*===========================================================================
 *
 * COUNTRY_CFG_GetDefaultAudioLanguage
 *
 * Parameters:
 *      none
 *
 * Description:
 *      Return the default audio language
 *
 * Returns:
 *      trigram of the default audio language
 *
 *===========================================================================*/
char* COUNTRY_CFG_GetDefaultAudioLanguage(void);

/*===========================================================================
 *
 * COUNTRY_CFG_GetDefaultSubtitleLanguage
 *
 * Parameters:
 *      none
 *
 * Description:
 *      Return the default subtitle language
 *
 * Returns:
 *      trigram of the default subtitle language
 *
 *===========================================================================*/
char* COUNTRY_CFG_GetDefaultSubtitleLanguage(void);


/*===========================================================================
 *
 * COUNTRY_CFG_GetDefaultSubtitleState
 *
 * Parameters:
 *      none
 *
 * Description:
 *      Return the default subtitle state
 *
 * Returns:
 *      FALSE if OFF, TRUE if ON
 *
 *===========================================================================*/
bool COUNTRY_CFG_GetDefaultSubtitleState(void);

/*===========================================================================
 *
 * COUNTRY_CFG_GetNumberOfMenuLanguage
 *
 * Parameters:
 *      none
 *
 * Description:
 *      Return the number of menu language
 *
 * Returns:
 *      number of menu langage
 *
 *===========================================================================*/
uint16_t COUNTRY_CFG_GetNumberOfMenuLanguage(void);

/*===========================================================================
 *
 * COUNTRY_CFG_GetMenuLanguageList
 *
 * Parameters:
 *      none
 *
 * Description:
 *      Return the language list information available for menu
 *
 * Returns:
 *      pointer on allocated and filled tCOUNTRY_CFG_Language
 *
 *===========================================================================*/
tCOUNTRY_CFG_Language* COUNTRY_CFG_GetMenuLanguageList(void);

/*===========================================================================
 *
 * COUNTRY_CFG_GetNumberOfLanguage
 *
 * Parameters:
 *      none
 *
 * Description:
 *      Return the number of language
 *
 * Returns:
 *      number of menu langage
 *
 *===========================================================================*/
uint16_t COUNTRY_CFG_GetNumberOfLanguage(void);

/*===========================================================================
 *
 * COUNTRY_CFG_GetNumberOfAudioLanguage
 *
 * Parameters:
 *      none
 *
 * Description:
 *      Return the number of language
 *
 * Returns:
 *      number of menu langage
 *
 *===========================================================================*/
uint16_t COUNTRY_CFG_GetNumberOfAudioLanguage(void);

/*===========================================================================
 *
 * COUNTRY_CFG_GetLanguageList
 *
 * Parameters:
 *      none
 *
 * Description:
 *      Return the language list information
 *
 * Returns:
 *      pointer on allocated and filled tCOUNTRY_CFG_Language
 *
 *===========================================================================*/
tCOUNTRY_CFG_Language* COUNTRY_CFG_GetLanguageList(void);

/*===========================================================================
 *
 * COUNTRY_CFG_GetAudioLanguageList
 *
 * Parameters:
 *      none
 *
 * Description:
 *      Return the language list information
 *
 * Returns:
 *      pointer on allocated and filled tCOUNTRY_CFG_Language
 *
 *===========================================================================*/
tCOUNTRY_CFG_Language* COUNTRY_CFG_GetAudioLanguageList(void);

/*===========================================================================
 *
 * COUNTRY_CFG_GetEntireLanguageList
 *
 * Parameters:
 *      none
 *
 * Description:
 *      Return the entire language list information
 *
 * Returns:
 *      pointer on allocated and filled tCOUNTRY_CFG_Language
 *
 *===========================================================================*/
tCOUNTRY_CFG_Language* COUNTRY_CFG_GetEntireLanguageList(uint16_t *nbLanguage);

/*===========================================================================
 *
 * COUNTRY_CFG_GetDayTranslation
 *
 * Parameters:
 *      index of language wished and day (1-7)
 *
 * Description:
 *      Return the day translation
 *
 * Returns:
 *      pointer on allocated and filled string
 *
 *===========================================================================*/
char* COUNTRY_CFG_GetDayTranslation(uint16_t LanguageIndex, uint8_t DayIndex);

/*===========================================================================
 *
 * COUNTRY_CFG_GetPeriodicityString
 *
 * Parameters:
 *      language index to use and periodicity
 *
 * Description:
 *      Return the periodicity translation
 *
 * Returns:
 *      pointer on allocated and filled string
 *
 *===========================================================================*/
char* COUNTRY_CFG_GetPeriodicityTranslation(uint16_t LanguageIndex, uint8_t uiPeriodicity);

/*===========================================================================
 *
 * COUNTRY_CFG_CheckEventContentNibble
 *
 * Parameters:
 *      input : EventContent0 and EventContent1
 *      output : pEventContentNibbleLevel1 and pEventContentNibbleLevel2
 *
 * Description:
 *      This function checks the consistancy of the event content descriptor
 *      values according to the contry selected and returns the nibbles to
 *      send to the application to display the right string.
 *      Note: it is possible to manage user_nibble by using nibble1 values
 *      that are not defined in ETSI EN 300 468 and managing these values in
 *      the application (e.g. screen_1200.xml and screen_9100.xml)
 *      
 *
 * Returns:
 *      Nothng
 *
 *===========================================================================*/
void COUNTRY_CFG_CheckEventContentNibble(uint8_t EventContent0, uint8_t EventContent1, uint8_t* pEventContentNibbleLevel1, uint8_t* pEventContentNibbleLevel2);

/*===========================================================================
 *
 * COUNTRY_CFG_SetCountryIndex
 *
 * Parameters:
 *      index
 *
 * Description:
 *      Set the current country
 *
 * Returns:
 *      nothing
 *
 *===========================================================================*/
void COUNTRY_CFG_SetCountryIndex(uint32_t index);

/*===========================================================================
 *
 * COUNTRY_CFG_GetCountryIndex
 *
 * Parameters:
 *
 *
 * Description:
 *      Return the index of a country
 *
 * Returns:
 *      index
 *
 *===========================================================================*/
uint32_t COUNTRY_CFG_GetCountryIndex(char* DVBtrigram);

/*===========================================================================
 *
 * COUNTRY_CFG_GetCountryONID
 *
 * Parameters:
 *
 *
 * Description:
 *      Return the ONID of a country
 *
 * Returns:
 *      ONID
 *
 *===========================================================================*/
uint16_t COUNTRY_CFG_GetCountryONID(char* DVBtrigram);

/*===========================================================================
 *
 * COUNTRY_CFG_GetCurrentCountryName
 *
 * Parameters:
 *      none
 *
 * Description:
 *      Return the name of the current country
 *
 * Returns:
 *      name of current country
 *
 *===========================================================================*/
char* COUNTRY_CFG_GetCurrentCountryName(void);

/*===========================================================================
 *
 * COUNTRY_CFG_GetCurrentCountryDVBtrigram
 *
 * Parameters:
 *      none
 *
 * Description:
 *      Return the DVB trigram of the current country
 *
 * Returns:
 *      DVB trigram of current country
 *
 *===========================================================================*/
char* COUNTRY_CFG_GetCurrentCountryDVBtrigram(void);

/*===========================================================================
 *
 * COUNTRY_CFG_GetCurrentCountryONID
 *
 * Parameters:
 *      none
 *
 * Description:
 *      Return the ONID of the current country
 *
 * Returns:
 *      ONID of current country
 *
 *===========================================================================*/
uint16_t COUNTRY_CFG_GetCurrentCountryONID(void);

/*===========================================================================
 *
 * COUNTRY_CFG_GetCurrentCountryLocalOffsetHour
 *
 * Parameters:
 *      none
 *
 * Description:
 *      Return the local offset hour of the current country
 *
 * Returns:
 *      local offset hour of current country
 *
 *===========================================================================*/
int COUNTRY_CFG_GetCurrentCountryLocalOffsetHour(void);

/*===========================================================================
 *
 * COUNTRY_CFG_GetCurrentCountryNbRegions
 *
 * Parameters:
 *      none
 *
 * Description:
 *      Return the number of regions defined for the current country
 *
 * Returns:
 *      number of regions
 *
 *===========================================================================*/
uint32_t COUNTRY_CFG_GetCurrentCountryNbRegions(void);

/*===========================================================================
 *
 * COUNTRY_CFG_GetCurrentCountryRegionName
 *
 * Parameters:
 *      none
 *
 * Description:
 *      Return the name of the regions defined for the current country
 *
 * Returns:
 *      name of the regions
 *
 *===========================================================================*/
char* COUNTRY_CFG_GetCurrentCountryRegionName(uint32_t index);

/*===========================================================================
 *
 * COUNTRY_CFG_GetCurrentCountryRegionID
 *
 * Parameters:
 *      none
 *
 * Description:
 *      Return the ID of the regions defined for the current country
 *
 * Returns:
 *      ID of the regions
 *
 *===========================================================================*/
uint16_t COUNTRY_CFG_GetCurrentCountryRegionID(uint32_t index);

/*===========================================================================
 *
 * COUNTRY_CFG_GetCurrentCountryRegionLocalHourOffset
 *
 * Parameters:
 *      none
 *
 * Description:
 *      Return the local hour offset of the regions defined for the current country
 *
 * Returns:
 *      local hour offset of the regions
 *
 *===========================================================================*/
int COUNTRY_CFG_GetCurrentCountryRegionLocalHourOffset(uint32_t index);

/*===========================================================================
 *
 * COUNTRY_CFG_GetCurrentCountryRegionLocalHourOffsetByRegionID
 *
 * Parameters:
 *      none
 *
 * Description:
 *      Return the local hour offset of the region defined by regionID
 *
 * Returns:
 *      local hour offset of the region
 *
 *===========================================================================*/
int COUNTRY_CFG_GetCurrentCountryRegionLocalHourOffsetByRegionID(uint32_t regionID);

/*===========================================================================
 *
 * COUNTRY_CFG_GetCountryLCNtype
 *
 * Parameters:
 *      none
 *
 * Description:
 *      Return the LCN type of the current country
 *
 * Returns:
 *      LCN type
 *
 *===========================================================================*/
tCOUNTRY_CFG_LCN_Type COUNTRY_CFG_GetCountryLCNtype(void);

/*===========================================================================
 *
 * COUNTRY_CFG_GetCountryPrivateDataSpecifier
 *
 * Parameters:
 *      none
 *
 * Description:
 *      Return the private data specifier to be used of the current country
 *
 * Returns:
 *      uint32_t
 *
 *===========================================================================*/
uint32_t COUNTRY_CFG_GetCountryPrivateDataSpecifier(void);

/*===========================================================================
 *
 * COUNTRY_CFG_GetCountryServiceIdentifierMode
 *
 * Parameters:
 *      none
 *
 * Description:
 *      Return the service identifier mode of the current country
 *
 * Returns:
 *      service identifier mode
 *
 *===========================================================================*/
tCOUNTRY_CFG_ServiceIdentifierMode COUNTRY_CFG_GetCountryServiceIdentifierMode(void);

#ifdef PRODUCT_USE_XPEL
/*===========================================================================
 *
 * COUNTRY_CFG_GetProgStatusTranslation
 *
 * Parameters:
 *      language index to use and ProgStatus index
 *
 * Description:
 *      Return the status translation
 *
 * Returns:
 *      pointer on allocated and filled string
 *
 *===========================================================================*/
char* COUNTRY_CFG_GetProgStatusTranslation(uint16_t LanguageIndex, tCOUNTRY_CFG_ProgStatus ProgStatus );

/*===========================================================================
 *
 * COUNTRY_CFG_GetTargetTranslation
 *
 * Parameters:
 *      language index to use and target index
 *
 * Description:
 *      Return the target translation
 *
 * Returns:
 *      pointer on allocated and filled string
 *
 *===========================================================================*/
char* COUNTRY_CFG_GetTargetTranslation( uint16_t                LanguageIndex,
                                        tCOUNTRY_CFG_ProgTarget     Target );


#endif
/*===========================================================================
 *
 * COUNTRY_CFG_GetCountryRankingList
 *
 * Parameters:
 *      none
 *
 * Description:
 *      Return the Ranking List associated to the current country
 *
 * Returns:
 *      service identifier mode
 *
 *===========================================================================*/
tCOUNTRY_CFG_RankingList* COUNTRY_CFG_GetCountryRankingList(void);

/*===========================================================================
 *
 * COUNTRY_CFG_GetCountryEITRequirements
 *
 * Parameters:
 *      none
 *
 * Description:
 *      Return the EIT requirements for the current country
 *
 * Returns:
 *      EIT requirements
 *
 *===========================================================================*/
void COUNTRY_CFG_GetCountryEITRequirements(
                    tCOUNTRY_CFG_EIT_Requirements *EITRequirements);


/*===========================================================================
 *
 * COUNTRY_CFG_CurrentCountrySupportMheg
 *
 * Parameters:
 *      none
 *
 * Description:
 *      Return TRUE if current country support MHEG, FALSE otherwise
 *
 * Returns:
 *      
 *
 *===========================================================================*/
bool COUNTRY_CFG_CurrentCountrySupportMheg(void);

/*===========================================================================
 *
 * COUNTRY_CFG_GetCurrentCountryTriggerType
 *
 * Parameters:
 *      none
 *
 * Description:
 *      Return the type of trigger to be used with the current country
 *
 * Returns:
 *
 *===========================================================================*/
tCOUNTRY_CFG_Trigger_Type COUNTRY_CFG_GetCurrentCountryTriggerType(void);

/*===========================================================================
 *
 * COUNTRY_CFG_GetInstallFlags
 *
 * Parameters:
 *      none
 *
 * Description:
 *      Returns the installation flags for the current country
 *
 * Returns:
 *      installation flags (bitfield)
 *
 *===========================================================================*/
uint32_t COUNTRY_CFG_GetInstallFlags(void);

/*===========================================================================
 *
 * COUNTRY_CFG_CurrentCountryUseFrequencyListDescriptor
 *
 * Parameters:
 *      none
 *
 * Description:
 *      Return TRUE if frequency list descriptor must be used according to the
 *      current country selected, FALSE otherwise
 *
 * Returns:
 *      
 *
 *===========================================================================*/
bool COUNTRY_CFG_CurrentCountryUseFrequencyListDescriptor(void);

/*===========================================================================
 *
 * COUNTRY_CFG_CurrentCountryDefaultCharacterTable
 *
 * Parameters:
 *      none
 *
 * Description:
 *      "ETSI EN 300 468 V1.8.1 Annex A: Coding of text characters" defines 
 *      that when the first byte of a DVB string is in the range "0x20" to 
 *      "0xFF", the character table to use is ISO/IEC6937.
 *      But in some european countries (like in France, Spain, ...), the 
 *      character table to use as default is ISO/IEC8859-9.
 *
 *      This function returns the character table to use as default according
 *      to the country selected.
 *      
 *      cf. ETSI EN 300 468 V1.8.1 Annex A: Coding of text characters for
 *      further information
 *
 * Returns:
 *      character table to use as default
 *
 *===========================================================================*/
tCOUNTRY_CFG_CharacterTable COUNTRY_CFG_CurrentCountryDefaultCharacterTable(void);

/*===========================================================================
 *
 * COUNTRY_CFG_GetCountryAnalogRangeBegin
 *
 * Parameters:
 *      none
 *
 * Description:
 *      Returns the analog range begin
 *
 * Returns:
 *      
 *
 *===========================================================================*/
uint16_t COUNTRY_CFG_GetCountryAnalogRangeBegin(void);


/*===========================================================================
 *
 * COUNTRY_CFG_GetLanguageString 
 *
 * Parameters:
 *      none
 *
 * Description:
 *      Return the full language string information
 *
 * Returns:
 *
 *
 *===========================================================================*/
bool COUNTRY_CFG_GetLanguageString ( uint8_t *LangCode, uint8_t *LangString, uint8_t    Sizetoget );

/*===========================================================================
 *
 * COUNTRY_CFG_GetCountryChannelNumberVariant
 *
 * Parameters:
 *      none
 *
 * Description:
 *      Returns the country channel number variant for the current country
 *
 * Returns:
 *
 *===========================================================================*/
void COUNTRY_CFG_GetCountryChannelNumberVariant(uint16_t *pusFirstVariantRangeBegin,
                                                uint16_t *pusFirstVariantRangeEnd,
                                                uint16_t *pusSecondVariantRangeBegin,
                                                uint16_t *pusAnalogVariantRangeBegin);

/*===========================================================================
 *
 * COUNTRY_CFG_GetCountrySortingInformation
 *
 * Parameters:
 *      none
 *
 * Description:
 *      Returns the country localisation information for sorting (cm_mslist)
 *
 * Returns:
 *
 *===========================================================================*/
void COUNTRY_CFG_GetCountrySortingInformation(bool  *pbUseNetworkId,
                                              bool  *pbUseSignalQuality);


/*===========================================================================
 *
 * COUNTRY_CFG_GetPermanentSiMonitoringMode
 *
 * Parameters:
 *      none
 *
 * Description:
 *      
 *
 * Returns:
 *
 *===========================================================================*/
eCOUNTRY_CFG_PermanentSiMonitoringMode COUNTRY_CFG_GetPermanentSiMonitoringMode(void);

/*===========================================================================
 *
 * COUNTRY_CFG_GetPermanentSiMonitoringDelay
 *
 * Parameters:
 *      none
 *
 * Description:
 *  return in second the delay to start the permanent nomitorind in seconds
 *  0 : immediate
 *
 * Returns:
 *
 *
 *===========================================================================*/
uint32_t COUNTRY_CFG_GetPermanentSiMonitoringDelay(void);

#ifdef __cplusplus
}
#endif

#endif /* _COUNTRY_CFG_H_ */
