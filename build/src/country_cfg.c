/********************************************************************************
 *                       COPYRIGHT 2005 IWEDIA                                  *
 ********************************************************************************
 *
 * MODULE NAME: COUNTRY CFG
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/product/comedia/branches/idecode_hd/build/src/country_cfg.c $
 *            $Rev: 1000 $
 *            $Date: 2011-12-23 18:05:20 +0100 (Fri, 23 Dec 2011) $
 *
 *
 *******************************************************************************/

/********************************************************/
/*                Includes                              */
/********************************************************/
#include <string.h>

#include "crules.h"
#include "country_cfg.h"

/********************************************************/
/*                                                      */
/* to change the available countries, you must modify : */
/*   - kCOUNTRY_CFG_NB_COUNTRY                          */
/*   - COUNTRY_CFG_countryList                          */
/*                                                      */
/* List of available translation						*/
/* 	 - COUNTRY_CFG_MenuLanguage							*/
/* List of Default choice for Audio and Subtitle		*/
/*   - COUNTRY_CFG_UserDefaultLanguage					*/
/* List of all language identificators		 			*/
/* !!! Reduce this list only if you want that the       */
/* end-user cannot select by itself a specific track.   */
/* (for almost the whole of the cases, do not touch     */
/* this list) !!!                                       */
/*   - COUNTRY_CFG_EntireLanguage						*/
/* to change the default country, you must modify :     */
/*   - kCOUNTRY_CFG_DEFAULT_COUNTRY                     */
/*                                                      */
/* ISO-3166-1-Alpha3 									*/
/* to add new countries, you must modify :              */
/*   - kCOUNTRY_CFG_NB_COUNTRY                          */
/*   - tCOUNTRY_CFG_Country                             */
/*   - COUNTRY_CFG_countryList                          */
/*   - COUNTRY_CFG_countryInfo                          */
/*                                                      */
/* to change the default menu language, you must modify:*/
/*   - kCOUNTRY_CFG_DEFAULT_MENU_LANGUAGE                */
/* to change the default audio language, you must modify:*/
/*   - kCOUNTRY_CFG_DEFAULT_AUDIO_LANGUAGE               */
/* to change the default subtitle language, you must modify:*/
/*   - kCOUNTRY_CFG_DEFAULT_SUBTITLE_LANGUAGE            */
/* to change the default subtitle state, you must modify:*/
/*	 - kCOUNTRY_CFG_DEFAULT_SUBTITLE_STATE				*/
/*                                                      */
/* NOTE: COUNTRY_CFG_countryInfo MUST be defined in the */
/*       same order as in tCOUNTRY_CFG_Country          */
/*                                                      */
/* If manufacturers use a frequency for factory test    */
/* that can't be found by the scanning, add the country */
/* eCOUNTRY_CFG_FACTORY_TEST in COUNTRY_CFG_countryList */
/* (the factory TS is defined in terinstall_cfg.c)      */
/*                                                      */
/********************************************************/

typedef enum
{   
#if 0
    eCOUNTRY_CFG_ENGLAND,
#ifndef UK_SPECIFIC
    eCOUNTRY_CFG_FRANCE,
	eCOUNTRY_CFG_GERMANY,
    eCOUNTRY_CFG_SPAIN,
	eCOUNTRY_CFG_ITALY,
	eCOUNTRY_CFG_DENMARK,
	eCOUNTRY_CFG_NETHERLANDS,
	eCOUNTRY_CFG_FINLAND,
	eCOUNTRY_CFG_IRELAND,
	eCOUNTRY_CFG_SWEDEN,
	eCOUNTRY_CFG_NORWAY,
	eCOUNTRY_CFG_ESTONIA,
	eCOUNTRY_CFG_ICELAND,
	eCOUNTRY_CFG_PORTUGAL,
	eCOUNTRY_CFG_GREECE,
	eCOUNTRY_CFG_BELGIUM,
	eCOUNTRY_CFG_SWITZERLAND,
    eCOUNTRY_CFG_INDIA,
    eCOUNTRY_CFG_BRAZIL,
#endif /* UK_SPECIFIC */
#endif
    eCOUNTRY_CFG_INDIA
 
} tCOUNTRY_CFG_Country;

/********************************************************/
/*              Defines                                 */
/********************************************************/
#ifdef UK_SPECIFIC
    #define kCOUNTRY_CFG_NB_COUNTRY                 1
    #define kCOUNTRY_CFG_DEFAULT_COUNTRY            eCOUNTRY_CFG_ENGLAND
    #define kCOUNTRY_CFG_NB_USER_DEFAULT_LANGUAGE	3
    #define kCOUNTRY_CFG_NB_LANGUAGE_OF_MENU 		3
#else /* UK_SPECIFIC */
    #define kCOUNTRY_CFG_NB_COUNTRY                 1


    #define kCOUNTRY_CFG_DEFAULT_COUNTRY            eCOUNTRY_CFG_INDIA

    #define kCOUNTRY_CFG_NB_USER_DEFAULT_LANGUAGE   10
    #define kCOUNTRY_CFG_NB_USER_AUDIO_LANGUAGE     9
    #define kCOUNTRY_CFG_NB_LANGUAGE_OF_MENU        2
#endif /* UK_SPECIFIC */

#define	kCOUNTRY_CFG_NB_REGION      2

#define kCOUNTRY_CFG_NB_LANGUAGE              	83
#define kCOUNTRY_CFG_DEFAULT_SUBTITLE_STATE		FALSE


#define kCOUNTRY_CFG_DEFAULT_MENU_LANGUAGE 		"eng"
#define kCOUNTRY_CFG_DEFAULT_AUDIO_LANGUAGE  	"eng"
#define kCOUNTRY_CFG_DEFAULT_SUBTITLE_LANGUAGE 	"eng"



/********** LANGUAGE **********/

tCOUNTRY_CFG_Language	COUNTRY_CFG_UserDefaultLanguage[kCOUNTRY_CFG_NB_USER_DEFAULT_LANGUAGE] =
{
    {"eng","eng","English"},
    {"hin","hin","Hindi"},
    {"mar","mar","Marathi"},
    {"ben","ben","Bengali"},
    {"guj","guj","Gujarati"},
    {"kan","kan","Kannada"},
    {"tel","tel","Telugu"},
    {"pan","pan","Punjabi"},
    {"tel","tel","Telugu"},
    {"tam","tam","Tamil"}

#if 0
	{"eng","eng","English"},
#ifdef UK_SPECIFIC
	{"gla","gae","G�idhlig"},    /* For UK version, the receiver should allow the three language codes */
	{"wel","cym","Cymraeg"},     /* present UK: English, Gaelic, Welsh */
#else /* UK_SPECIFIC */
	{"fre","fra","Fran�ais"},
	{"ger","deu","Deutsch"},
	{"ita","ita","Italiano"},
	{"spa","esl","Espa�ol"},
	{"dan","dan","Dansk"},
	{"fin","fin","Suomi"},
	{"swe","sve","Svenska"},
	{"nor","nor","Norsk"},
	{"ice","isl","�slenska"},
	{"dut","nla","Nederlands"},
	{"gre","ell","Ellinika'"},
	{"por","por","Portugu�s"},
	{"tur","tur","T�rk�e"},
	{"glg","glg","Galego"},     /* For version including Spain country, the receiver */
	{"baq","eus","Euskara"},    /* should allow the four language codes present in Spain: spanish, */
	{"cat","cat","Catal�"},      /* galician, basque and catalan */
	{"cze","ces","Cesky"},
	{"hun","hun","Magyar"},
	{"rus","rus","Russky"},
	{"pol","pol","Polski"}
#endif /* UK_SPECIFIC */

#endif
};

tCOUNTRY_CFG_Language	COUNTRY_CFG_AudioLanguage[kCOUNTRY_CFG_NB_USER_AUDIO_LANGUAGE] =
{
	{"eng","eng","English"},
	{"hin","hin","Hindi"},
    {"mar","mar","Marathi"},
    {"ben","ben","Bengali"},
    {"guj","guj","Gujarati"},
    {"kan","kan","Kannada"},
    {"pan","pan","Punjabi"},
    {"tam","tam","Tamil"},
    {"tel","tel","Telugu"}

};


/*Warning the first trigram must be equals to the first trigram in COUNTRY_CFG_Language */
tCOUNTRY_CFG_Language	COUNTRY_CFG_MenuLanguage[kCOUNTRY_CFG_NB_LANGUAGE_OF_MENU]	=
{
	{"eng","eng","English"},
//	{"hin","hin","Hindi"},
//  {"guj","guj","Gujarati"},
    {"tel","tel","Telugu"}
    /*,{"mar","mar","Marathi"}*/
};

tCOUNTRY_CFG_Language	COUNTRY_CFG_EntireLanguage[kCOUNTRY_CFG_NB_LANGUAGE]	=
{
	{"eng","eng","English"},
	{"fre","fra","Francais"},
	{"fin","fin","Suomi"},
	{"swe","sve","Svenska"},
	{"dut","nla","Nederlands"},
	{"ger","deu","Deutsch"},
	{"lav","lav","Latvian"},
	{"rus","rus","Russky"},
	{"alb","sqi","Shqip"},
	{"ara","ara","Alarabia"},
	{"arm","hye","Armenian"},
	{"aze","aze","Azeri"},
	{"ban","ban","Balinese"},
	{"baq","eus","Euskara"},
	{"bel","bel","Belaruski"},
	{"bre","bre","Breizh"},
	{"bul","bul","Balgarski"},
	{"cat","cat","Catal�"},
	{"chi","zho","Chinese"},
	{"ces","cze","Cesky"},
	{"dan","dan","Dansk"},
	{"est","est","Eesti"},
	{"fas","per","Farsi/Persian"},
	{"gla","gae","G�idhlig"},
	{"geo","kat","Georgian"},
	{"glg","glg","Galego"},
	{"gre","ell","Ellinika'"},
	{"heb","heb","Ivrit"},
	{"hin","hin","Hindi"},
	{"hun","hun","Magyar"},
	{"ice","isl","�slenska"},
	{"ind","ind","Indonesian"},
	{"ira","ira","Iranian"},
	{"ita","ita","Italiano"},
	{"jpn","jpn","Nihongo"},
	{"kab","kab","Kabyle"},
	{"khm","khm","Khmer"},
	{"kor","kor","Hankuko"},
	{"kur","kur","Kurdish"},
	{"lit","lit","Lietuviskai"},
	{"mac","mak","Macedonian"},
	{"may","msa","Bahasa Melayu"},
	{"nno","nno","Nynorsk"},
	{"nor","nor","Norsk"},
	{"pol","pol","Polski"},
	{"por","por","Portugu�s"},
	{"ron","rum","Rom�na"},
	{"srb","srb","Serbian"},
	{"scr","scr","Serbo-Croatian"},
	{"slk","slo","Slovencina"},
	{"slv","slv","Slovenscina"},
	{"spa","esl","Espa�ol"},
	{"tha","tha","Thai"},
	{"tur","tur","T�rk�e"},
	{"ukr","ukr","Ukrayins'ka"},
	{"urd","urd","Urdu"},
	{"uzb","uzb"," Uzbek"},
	{"vie","vie","Vietnamese"},
	{"wel","cym","Cymraeg"},
	{"gle","gle","Gaeilge"},
	{"hin","hin","Hindi"},
	{"mar","mar","Marathi"},
	{"ben","ben","Bengali"},
	{"guj","guj","Gujarati"},
	{"kan","kan","Kannada"},
	{"tel","tel","Telugu"},
	{"pan","pan","Punjabi"},
	{"afr","afr","Afrikaans"},
	{"nbl","nbl","Ndebele"},
	{"nso","nso","Sotho, Northern"},
	{"sot","sot","Sotho, Southern"},
	{"ssw","ssw","Swati"},
	{"tso","tso","Tsonga"},
	{"tsn","tsn","Tswana"},
	{"ven","ven","Venda"},
	{"xho","xho","Xhosa"},
	{"zul","zul","Zulu"},
    {"mar","mar","Marathi"},
    {"tam","tam","Tamil"},
	{"qaa","","Original"},
	{"mul","","Multiple"},
	{"und","","Undefined"},
	{"tel","tel","Telugu"}
};

#ifdef UK_SPECIFIC
/* Translation order is the same as in COUNTRY_CFG_MenuLanguage */
char* COUNTRY_CFG_periodicity[kCOUNTRY_CFG_NB_LANGUAGE_OF_MENU][11]=
{
	{"Once","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday","Sunday","Week-end","Weekdays","Daily"},
	{"Once","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday","Sunday","Week-end","Weekdays","Daily"},
	{"Once","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday","Sunday","Week-end","Weekdays","Daily"}
};
#else /* UK_SPECIFIC */
/* Translation order is the same as in COUNTRY_CFG_MenuLanguage */
char* COUNTRY_CFG_periodicity[kCOUNTRY_CFG_NB_LANGUAGE_OF_MENU][11]=
{
/* eng */	{"Once","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday","Sunday","Week-end","Weekdays","Daily"},
        {"Once","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday","Sunday","Week-end","Weekdays","Daily"},
        {"Once","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday","Sunday","Week-end","Weekdays","Daily"}
      /*  {"Once","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday","Sunday","Week-end","Weekdays","Daily"} */
};
#endif /* UK_SPECIFIC */

/* Translation order is the same as in COUNTRY_CFG_MenuLanguage */
/* It must follow the enum values of tCOUNTRY_CFG_ProgStatus */
#ifdef UK_SPECIFIC
char* COUNTRY_CFG_ProgStatus[kCOUNTRY_CFG_NB_LANGUAGE_OF_MENU][18]=
{
	{ "Not checked", "In progress", "Planned", "Conflicted", "No space",
      "Serie", "HDD Err (1)", "HDD Err (2)",
      "REC Err (1)", "REC Err (2)", "REC Err (3)", "REC Err (4)", "REC Err (5)", "REC Err (6)", "REC Err (7)", "REC Err (8)", "REC Err (9)",
      "Unknown" },
	{ "Not checked", "In progress", "Planned", "Conflicted", "No space",
      "Serie", "HDD Err (1)", "HDD Err (2)",
      "REC Err (1)", "REC Err (2)", "REC Err (3)", "REC Err (4)", "REC Err (5)", "REC Err (6)", "REC Err (7)", "REC Err (8)", "REC Err (9)",
      "Unknown" }
    /*,
     { "Not checked", "In progress", "Planned", "Conflicted", "No space",
      "Serie", "HDD Err (1)", "HDD Err (2)",
      "REC Err (1)", "REC Err (2)", "REC Err (3)", "REC Err (4)", "REC Err (5)", "REC Err (6)", "REC Err (7)", "REC Err (8)", "REC Err (9)",
      "Unknown" } */
};

char* COUNTRY_CFG_ProgTarget[kCOUNTRY_CFG_NB_LANGUAGE_OF_MENU][5]=
{
	{ "None", "HDD", "VCR", "Reminder", "External HDD"},
	{ "None", "HDD", "VCR", "Reminder", "External HDD"},
	{ "None", "HDD", "VCR", "Reminder", "External HDD"}
};

#else /* UK_SPECIFIC */
char* COUNTRY_CFG_ProgStatus[kCOUNTRY_CFG_NB_LANGUAGE_OF_MENU][18]=
{
/* eng */	{ "Not checked", "In progress", "Planned", "Conflicted", "No space",
      "Serie", "HDD Err (1)", "HDD Err (2)",
      "REC Err (1)", "REC Err (2)", "REC Err (3)", "REC Err (4)", "REC Err (5)", "REC Err (6)", "REC Err (7)", "REC Err (8)", "REC Err (9)",
      "Unknown" },
      { "Not checked", "In progress", "Planned", "Conflicted", "No space",
      "Serie", "HDD Err (1)", "HDD Err (2)",
      "REC Err (1)", "REC Err (2)", "REC Err (3)", "REC Err (4)", "REC Err (5)", "REC Err (6)", "REC Err (7)", "REC Err (8)", "REC Err (9)",
      "Unknown" },
      { "Not checked", "In progress", "Planned", "Conflicted", "No space",
      "Serie", "HDD Err (1)", "HDD Err (2)",
      "REC Err (1)", "REC Err (2)", "REC Err (3)", "REC Err (4)", "REC Err (5)", "REC Err (6)", "REC Err (7)", "REC Err (8)", "REC Err (9)",
      "Unknown" }
/*,
      { "Not checked", "In progress", "Planned", "Conflicted", "No space",
      "Serie", "HDD Err (1)", "HDD Err (2)",
      "REC Err (1)", "REC Err (2)", "REC Err (3)", "REC Err (4)", "REC Err (5)", "REC Err (6)", "REC Err (7)", "REC Err (8)", "REC Err (9)",
      "Unknown" } */
};

char* COUNTRY_CFG_ProgTarget[kCOUNTRY_CFG_NB_LANGUAGE_OF_MENU][5]=
{
/* eng */	{ "None", "HDD", "VCR", "Reminder", "External HDD"},
    { "None", "HDD", "VCR", "Reminder", "External HDD"},
    { "None", "HDD", "VCR", "Reminder", "External HDD"},
   /* { "None", "HDD", "VCR", "Reminder", "External HDD"}*/
};

#endif /* UK_SPECIFIC */


/********** COUNTRY **********/

typedef struct
{
    char        name[16];
    uint16_t    regionID;
    int16_t     LocalHourOffset;
}tCOUNTRY_CFG_RegionInfo;



/* FYI : The Comedia standard software define the following rules for ordonnancement
   For UK country :
        usFirstVariantRangeBegin    = 800   (rejected or duplicate LCN will start at 800)
        usFirstVariantRangeEnd      = not used in this case (to be set to 9999)
        usSecondVariantRangeBegin   = not used in this case (to be set to 10000)
        usAnalogVariantRangeBegin   = 20000 (set to 0 if analog must be put in normal rejected list)


    For Italy country :
        usFirstVariantRangeBegin    = 75 (as define in standard)
        usFirstVariantRangeEnd      = 99 (as define in standard)
        usSecondVariantRangeBegin   = 850 (as define in standard)
        usAnalogVariantRangeBegin   = 20000 (set to 0 if analog must be put in normal rejected list)

    For rest of PanEu country :
        usFirstVariantRangeBegin    = 1 (to start following the latest LCN accepted)
        usFirstVariantRangeEnd      = not used in this case (to be set to 9999)
        usSecondVariantRangeBegin   = not used in this case (to be set to 10000)
        usAnalogVariantRangeBegin   = 20000 (set to 0 if analog must be put in normal rejected list)
*/

typedef struct
{
    uint16_t usFirstVariantRangeBegin;
    uint16_t usFirstVariantRangeEnd;
    uint16_t usSecondVariantRangeBegin;
    uint16_t usAnalogVariantRangeBegin;
}tCOUNTRY_CFG_CountryChannelNumberVariant;

typedef struct
{
    char        DVBtrigram[4];
    char        name[16];
    uint16_t    ONID;
    tCOUNTRY_CFG_LCN_Type LCNtype;
    uint32_t    ulInstallFlags;
	uint32_t    ulPrivateDataSpecifier; /* if != 0 -> used for LCN mngt in PIDB */
    tCOUNTRY_CFG_ServiceIdentifierMode serviceIdentifierMode;
    int16_t     LocalHourOffset;
    tCOUNTRY_CFG_RegionInfo regions[kCOUNTRY_CFG_NB_REGION];
	tCOUNTRY_CFG_RankingList *pstRankingList;
    tCOUNTRY_CFG_CountryChannelNumberVariant    stChannelNumberVariant;
    tCOUNTRY_CFG_EIT_Requirements       EITRequirements;
} tCOUNTRY_CFG_CountryInfo;

#ifndef UK_SPECIFIC
#include "deutschland_ranking_list_p.h"
#endif

LOCAL tCOUNTRY_CFG_Country  COUNTRY_CFG_CurrentCountry = kCOUNTRY_CFG_DEFAULT_COUNTRY;
LOCAL tCOUNTRY_CFG_Country  COUNTRY_CFG_countryList[kCOUNTRY_CFG_NB_COUNTRY] =
{
    eCOUNTRY_CFG_INDIA   
};

/* country infos MUST be defined in the same order as in tCOUNTRY_CFG_Country */
LOCAL tCOUNTRY_CFG_CountryInfo COUNTRY_CFG_countryInfo[kCOUNTRY_CFG_NB_COUNTRY + 1] =
{

/* eCOUNTRY_CFG_INDIA */
{
    "ind",
    "India",
    0xa751,
    eCOUNTRY_CFG_LCN_TYPE_EACEM_AND_HD_SIMULCAST_MANAGEMENT,
    /*
    for some streams, for some demos, we need to check the ONID
    (kCOUNTRY_CFG_INSTALL_NID_PRIVATE_TMP_USE |
     kCOUNTRY_CFG_INSTALL_ONID_PRIVATE_TMP_USE)
    */
    /* network_ID in the range "private temporary use" are used in MHEG test streams */
    kCOUNTRY_CFG_INSTALL_NID_PRIVATE_TMP_USE,
    0,
    eCOUNTRY_CFG_SERVICE_IDENTIFIER_DAE,
    0,
    {{"",0,1},{"",0,1}},
    NULL,
    {1, 9999, 10000, 20000},
    {(kCOUNTRY_CFG_EIT_PRESENT_FOLLOWING_FLAG |
            kCOUNTRY_CFG_EIT_SCHEDULE_FLAG |
            kCOUNTRY_CFG_EIT_LAST_SECTION_NUMBER |
            kCOUNTRY_CFG_EIT_LAST_TABLE_ID |
            kCOUNTRY_CFG_EIT_SCHEDULE_OTHER |
            kCOUNTRY_CFG_EIT_PRESENT_FOLLOWING_OTHER),
     6,
     60,
     75,
     500,
     2}
},
#if 0

#ifdef UK_SPECIFIC
    /* eCOUNTRY_CFG_ENGLAND */
    {
        "gbr",
        "United Kingdom",
        0x233A,
        eCOUNTRY_CFG_LCN_TYPE_DBOOK,
        kCOUNTRY_CFG_INSTALL_USE_LCN_IF_PDSD_OK |
        kCOUNTRY_CFG_INSTALL_NID_PRIVATE_TMP_USE, /* network_ID in the range "private temporary use" are used in MHEG test streams */
		0x0000233A,
#ifdef PRODUCT_USE_NX2_UI
        eCOUNTRY_CFG_SERVICE_IDENTIFIER_DAE,
#else
        eCOUNTRY_CFG_SERVICE_IDENTIFIER_DTF,
#endif
        0,
        {{"",0,0},{"",0,0}},
		NULL,
        {800, 9999, 10000, 900},
        {(kCOUNTRY_CFG_EIT_PRESENT_FOLLOWING_FLAG |
          kCOUNTRY_CFG_EIT_SCHEDULE_FLAG |
          kCOUNTRY_CFG_EIT_LAST_SECTION_NUMBER |
          kCOUNTRY_CFG_EIT_LAST_TABLE_ID |
          kCOUNTRY_CFG_EIT_SCHEDULE_OTHER |
          kCOUNTRY_CFG_EIT_PRESENT_FOLLOWING_OTHER),
#ifdef PRODUCT_PVR_FREEVIEW_PLAYBACK
		 6,     /*for freeview certif streams*/
		 60,	/*for freeview certif streams*/
#else
         2,
         5,
#endif
#ifdef PRODUCT_PVR_FREEVIEW_PLAYBACK
         60,
         500,
#else /* PRODUCT_PVR_FREEVIEW_PLAYBACK */
         75, /* EITschActualRepetitionRate value resulting from UK field tests */
         300,
#endif /* PRODUCT_PVR_FREEVIEW_PLAYBACK */
         2}
    },
#else /* UK_SPECIFIC*/


   /* eCOUNTRY_CFG_SOUTHAFRICA */
   {
       "zaf",
       "South Africa",
       0x22C6,
       eCOUNTRY_CFG_LCN_TYPE_DBOOK,
       /*
       for some streams, for some demos, we need to check the ONID
       (kCOUNTRY_CFG_INSTALL_NID_PRIVATE_TMP_USE |
        kCOUNTRY_CFG_INSTALL_ONID_PRIVATE_TMP_USE)
       */
       /* network_ID in the range "private temporary use" are used in MHEG test streams */
       /* original network_ID in the range "private temporary use" are used in Dolby test streams */
       kCOUNTRY_CFG_INSTALL_USE_LCN_IF_PDSD_OK |
       kCOUNTRY_CFG_INSTALL_NID_PRIVATE_TMP_USE |
       kCOUNTRY_CFG_INSTALL_ONID_PRIVATE_TMP_USE,
       0x22C6,
       eCOUNTRY_CFG_SERVICE_IDENTIFIER_DAE,
       5,
       {{"Region 1",1,1},{"Region 2",2,0}},
       NULL,
       {1, 9999, 10000, 20000},
       {(kCOUNTRY_CFG_EIT_PRESENT_FOLLOWING_FLAG |
               kCOUNTRY_CFG_EIT_SCHEDULE_FLAG |
               kCOUNTRY_CFG_EIT_LAST_SECTION_NUMBER |
               kCOUNTRY_CFG_EIT_LAST_TABLE_ID |
               kCOUNTRY_CFG_EIT_SCHEDULE_OTHER |
               kCOUNTRY_CFG_EIT_PRESENT_FOLLOWING_OTHER),
        7,
        20,
        30,
        300,
        2}
   },

    /* eCOUNTRY_CFG_SERBIA */
    {
        "srb",
        "Serbia",
        0x22B0,
        eCOUNTRY_CFG_LCN_TYPE_DBOOK,
        /* 
        for some streams, for some demos, we need to check the ONID
        (kCOUNTRY_CFG_INSTALL_NID_PRIVATE_TMP_USE |
         kCOUNTRY_CFG_INSTALL_ONID_PRIVATE_TMP_USE)
        */
        /* network_ID in the range "private temporary use" are used in MHEG test streams */
        kCOUNTRY_CFG_INSTALL_USE_LCN_IF_PDSD_OK |
        kCOUNTRY_CFG_INSTALL_NID_PRIVATE_TMP_USE |
        kCOUNTRY_CFG_INSTALL_ONID_PRIVATE_TMP_USE,
        0,
        eCOUNTRY_CFG_SERVICE_IDENTIFIER_DAE,
        -3,
        {{"Region 1",1,1}},
        NULL,
        {1, 9999, 10000, 20000},
        {(kCOUNTRY_CFG_EIT_PRESENT_FOLLOWING_FLAG |
               kCOUNTRY_CFG_EIT_SCHEDULE_FLAG |
               kCOUNTRY_CFG_EIT_LAST_SECTION_NUMBER |
          kCOUNTRY_CFG_EIT_LAST_TABLE_ID |
               kCOUNTRY_CFG_EIT_SCHEDULE_OTHER |
          kCOUNTRY_CFG_EIT_PRESENT_FOLLOWING_OTHER),
         7,
         20,
         30,
         300,
         2}
    },

    /* eCOUNTRY_CFG_FRANCE */
    {
        "fra",
        "France",
        0x20FA,
        eCOUNTRY_CFG_LCN_TYPE_EACEM_AND_HD_SIMULCAST_MANAGEMENT,
        /*
        for some streams, for some demos, we need to check the ONID
        (kCOUNTRY_CFG_INSTALL_NID_PRIVATE_TMP_USE |
         kCOUNTRY_CFG_INSTALL_ONID_PRIVATE_TMP_USE)
        */
        /* network_ID in the range "private temporary use" are used in MHEG test streams */
        kCOUNTRY_CFG_INSTALL_NID_PRIVATE_TMP_USE,
		0,
#ifdef PRODUCT_USE_NX2_UI
        eCOUNTRY_CFG_SERVICE_IDENTIFIER_DAE,
#else
		eCOUNTRY_CFG_SERVICE_IDENTIFIER_DTF,
#endif
        1,
        {{"",0,1},{"",0,1}},
		NULL,
        {1, 9999, 10000, 20000},
        {(kCOUNTRY_CFG_EIT_LAST_SECTION_NUMBER |
          kCOUNTRY_CFG_EIT_LAST_TABLE_ID |
          kCOUNTRY_CFG_EIT_PRESENT_FOLLOWING_OTHER),
         7,
         20,
         30,
         300,
         2}
    },
    /* eCOUNTRY_CFG_GERMANY */
    {
        "deu",
        "Deutschland",
        0x2114,
        eCOUNTRY_CFG_LCN_TYPE_EACEM,
        kCOUNTRY_CFG_INSTALL_USE_LCN_IF_PDSD_OK,
		0xDEADBEEF,	/* Trick to avoid use of LCN */
#ifdef PRODUCT_USE_NX2_UI
        eCOUNTRY_CFG_SERVICE_IDENTIFIER_DAE,
#else
        eCOUNTRY_CFG_SERVICE_IDENTIFIER_DTF,
#endif
        1,
        {{"",0,1},{"",0,1}},
		COUNTRY_DeutschlandRankingList,
        {1, 9999, 10000, 20000},
        {(kCOUNTRY_CFG_EIT_LAST_SECTION_NUMBER |
          kCOUNTRY_CFG_EIT_LAST_TABLE_ID |
          kCOUNTRY_CFG_EIT_PRESENT_FOLLOWING_OTHER),
         7,
         20,
         30,
         300,
         2}
    },
    /* eCOUNTRY_CFG_SPAIN */
    {
        "esp",
        "Espa�a",
        0x22D4,
        eCOUNTRY_CFG_LCN_TYPE_EACEM,
        kCOUNTRY_CFG_INSTALL_USE_LCN_IF_PDSD_OK,
		0xDEADBEEF,	/* Trick to avoid use of LCN */
#ifdef PRODUCT_USE_NX2_UI
        eCOUNTRY_CFG_SERVICE_IDENTIFIER_DAE,
#else
        eCOUNTRY_CFG_SERVICE_IDENTIFIER_DTF,
#endif
        1,
        {{"Pen�nsula",1,1},{"Islas Canarias",2,0}},
		NULL,
        {1, 9999, 10000, 20000},
        {(kCOUNTRY_CFG_EIT_LAST_SECTION_NUMBER |
          kCOUNTRY_CFG_EIT_LAST_TABLE_ID |
          kCOUNTRY_CFG_EIT_PRESENT_FOLLOWING_OTHER),
         7,
         20,
         30,
         300,
         2}
    },
    /* eCOUNTRY_CFG_ITALY */
    {
        "ita",
        "Italia",
        0x217c,
        eCOUNTRY_CFG_LCN_TYPE_EACEM,
        kCOUNTRY_CFG_INSTALL_USE_LCN_EVEN_WITH_NO_PDSD |
        kCOUNTRY_CFG_INSTALL_LCN_0_NOT_SELECTABLE,
		0x00000028,
#ifdef PRODUCT_USE_NX2_UI
        eCOUNTRY_CFG_SERVICE_IDENTIFIER_DAE,
#else
        eCOUNTRY_CFG_SERVICE_IDENTIFIER_DTF,
#endif
        1,
        {{"",0,1},{"",0,1}},
		NULL,
        {75, 99, 850, 20000},
        {(kCOUNTRY_CFG_EIT_LAST_SECTION_NUMBER |
          kCOUNTRY_CFG_EIT_LAST_TABLE_ID |
          kCOUNTRY_CFG_EIT_PRESENT_FOLLOWING_OTHER),
         7,
         20,
         30,
         300,
         2}
    },
    /* eCOUNTRY_CFG_DENMARK */
    {
        "dnk",
        "Danmark",
        0x20D0,
        eCOUNTRY_CFG_LCN_TYPE_NORDIG_V2_COUNTRY_DENMARK,
        kCOUNTRY_CFG_INSTALL_USE_LCN_IF_PDSD_OK,
		0x00000029,
#ifdef PRODUCT_USE_NX2_UI
        eCOUNTRY_CFG_SERVICE_IDENTIFIER_DAE,
#else
        eCOUNTRY_CFG_SERVICE_IDENTIFIER_DTN,
#endif
        1,
        {{"",0,1},{"",0,1}},
		NULL,
        {1, 9999, 10000, 20000},
        {(kCOUNTRY_CFG_EIT_LAST_SECTION_NUMBER |
          kCOUNTRY_CFG_EIT_LAST_TABLE_ID |
          kCOUNTRY_CFG_EIT_PRESENT_FOLLOWING_OTHER),
         7,
         20,
         30,
         300,
         2}
    },
    /* eCOUNTRY_CFG_NETHERLANDS */
    {
        "nld",
        "Nederland",
        0x2210,
        eCOUNTRY_CFG_LCN_TYPE_EACEM,
        kCOUNTRY_CFG_INSTALL_DEFAULT,
		0,
#ifdef PRODUCT_USE_NX2_UI
        eCOUNTRY_CFG_SERVICE_IDENTIFIER_DAE,
#else
        eCOUNTRY_CFG_SERVICE_IDENTIFIER_DTF,
#endif
        1,
        {{"",0,1},{"",0,1}},
		NULL,
        {1, 9999, 10000, 20000},
        {(kCOUNTRY_CFG_EIT_LAST_SECTION_NUMBER |
          kCOUNTRY_CFG_EIT_LAST_TABLE_ID |
          kCOUNTRY_CFG_EIT_PRESENT_FOLLOWING_OTHER),
         7,
         20,
         30,
         300,
         2}
    },
    /* eCOUNTRY_CFG_FINLAND */
    {
        "fin",
        "Finland",
        0x20F6,
        eCOUNTRY_CFG_LCN_TYPE_NORDIG_V2_COUNTRY_FINLAND,
        kCOUNTRY_CFG_INSTALL_USE_LCN_IF_PDSD_OK,
		0x00000029,
#ifdef PRODUCT_USE_NX2_UI
        eCOUNTRY_CFG_SERVICE_IDENTIFIER_DAE,
#else
        eCOUNTRY_CFG_SERVICE_IDENTIFIER_DTN,
#endif
        2,
        {{"",0,2},{"",0,2}},
		NULL,
        {1, 9999, 10000, 20000},
        {(kCOUNTRY_CFG_EIT_LAST_SECTION_NUMBER |
          kCOUNTRY_CFG_EIT_LAST_TABLE_ID |
          kCOUNTRY_CFG_EIT_PRESENT_FOLLOWING_OTHER),
         7,
         20,
         30,
         300,
         2}
    },
    /* eCOUNTRY_CFG_IRELAND */
    {
        "irl",
        "Ireland",
        0x2174,
        eCOUNTRY_CFG_LCN_TYPE_NORDIG_V2_COUNTRY_IRELAND, /* Nordig V2 requires modifs in PIDB */
        kCOUNTRY_CFG_INSTALL_USE_LCN_IF_PDSD_OK|
        kCOUNTRY_CFG_INSTALL_NID_PRIVATE_TMP_USE,
		0x00000029,
#ifdef PRODUCT_USE_NX2_UI
        eCOUNTRY_CFG_SERVICE_IDENTIFIER_DAE,
#else
        eCOUNTRY_CFG_SERVICE_IDENTIFIER_DTN,
#endif
        0,
        {{"",0,0},{"",0,0}},
		NULL,
        {1, 9999, 10000, 20000},
        {(kCOUNTRY_CFG_EIT_LAST_SECTION_NUMBER |
          kCOUNTRY_CFG_EIT_LAST_TABLE_ID |
          kCOUNTRY_CFG_EIT_PRESENT_FOLLOWING_OTHER),
         7,
         20,
         30,
         300,
         2}
    },
    /* eCOUNTRY_CFG_SWEDEN */
    {
        "swe",
        "Sverige",
        0x22F1,
        eCOUNTRY_CFG_LCN_TYPE_NORDIG_V2_COUNTRY_SWEDEN,
        kCOUNTRY_CFG_INSTALL_USE_LCN_IF_PDSD_OK,
		0x00000029,
#ifdef PRODUCT_USE_NX2_UI
        eCOUNTRY_CFG_SERVICE_IDENTIFIER_DAE,
#else
        eCOUNTRY_CFG_SERVICE_IDENTIFIER_DTN,
#endif
        1,
        {{"",0,1},{"",0,1}},
		NULL,
        {1, 9999, 10000, 20000},
        {(kCOUNTRY_CFG_EIT_LAST_SECTION_NUMBER |
          kCOUNTRY_CFG_EIT_LAST_TABLE_ID |
          kCOUNTRY_CFG_EIT_PRESENT_FOLLOWING_OTHER),
         7,
         20,
         30,
         300,
         2}
    },
    /* eCOUNTRY_CFG_NORWAY */
    {
        "nor",
        "Norge",
        0x2242,
        eCOUNTRY_CFG_LCN_TYPE_NORDIG_V2_COUNTRY_NORWAY,
        kCOUNTRY_CFG_INSTALL_USE_LCN_EVEN_WITH_NO_PDSD,
		0x00000029,
#ifdef PRODUCT_USE_NX2_UI
        eCOUNTRY_CFG_SERVICE_IDENTIFIER_DAE,
#else
        eCOUNTRY_CFG_SERVICE_IDENTIFIER_DTN,
#endif
        1,
        {{"",0,1},{"",0,1}},
		NULL,
        {1, 9999, 10000, 20000},
        {(kCOUNTRY_CFG_EIT_LAST_SECTION_NUMBER |
          kCOUNTRY_CFG_EIT_LAST_TABLE_ID |
          kCOUNTRY_CFG_EIT_PRESENT_FOLLOWING_OTHER),
         7,
         20,
         30,
         300,
         2}
    },
    /* eCOUNTRY_CFG_ESTONIA */
    {
        "est",
        "Eesti",
        0x20E9,
        eCOUNTRY_CFG_LCN_TYPE_NORDIG_V2_COUNTRY_ESTONIA,
        kCOUNTRY_CFG_INSTALL_USE_LCN_EVEN_WITH_NO_PDSD,
		0x00000029,
#ifdef PRODUCT_USE_NX2_UI
        eCOUNTRY_CFG_SERVICE_IDENTIFIER_DAE,
#else
        eCOUNTRY_CFG_SERVICE_IDENTIFIER_DTN,
#endif
        2,
        {{"",0,2},{"",0,2}},
		NULL,
        {1, 9999, 10000, 20000},
        {(kCOUNTRY_CFG_EIT_LAST_SECTION_NUMBER |
          kCOUNTRY_CFG_EIT_LAST_TABLE_ID |
          kCOUNTRY_CFG_EIT_PRESENT_FOLLOWING_OTHER),
         7,
         20,
         30,
         300,
         2}
    },
    /* eCOUNTRY_CFG_ICELAND */
    {
        "isl",
        "�sland",
        0x2160,
        eCOUNTRY_CFG_LCN_TYPE_NORDIG_V2_COUNTRY_ICELAND,
        kCOUNTRY_CFG_INSTALL_USE_LCN_IF_PDSD_OK,
		0x00000029,
#ifdef PRODUCT_USE_NX2_UI
        eCOUNTRY_CFG_SERVICE_IDENTIFIER_DAE,
#else
        eCOUNTRY_CFG_SERVICE_IDENTIFIER_DTN,
#endif
        0,
        {{"",0,0},{"",0,0}},
		NULL,
        {1, 9999, 10000, 20000},
        {(kCOUNTRY_CFG_EIT_LAST_SECTION_NUMBER |
          kCOUNTRY_CFG_EIT_LAST_TABLE_ID |
          kCOUNTRY_CFG_EIT_PRESENT_FOLLOWING_OTHER),
         7,
         20,
         30,
         300,
         2}
    },
    /* eCOUNTRY_CFG_PORTUGAL */
    {
        "por",
        "Portugal",
        0x226C,
        eCOUNTRY_CFG_LCN_TYPE_EACEM,
        kCOUNTRY_CFG_INSTALL_DEFAULT,
		0,
#ifdef PRODUCT_USE_NX2_UI
        eCOUNTRY_CFG_SERVICE_IDENTIFIER_DAE,
#else
        eCOUNTRY_CFG_SERVICE_IDENTIFIER_DTF,
#endif
        0,
        {{"",0,0},{"",0,0}},
		NULL,
        {1, 9999, 10000, 20000},
        {(kCOUNTRY_CFG_EIT_LAST_SECTION_NUMBER |
          kCOUNTRY_CFG_EIT_LAST_TABLE_ID |
          kCOUNTRY_CFG_EIT_PRESENT_FOLLOWING_OTHER),
         7,
         20,
         30,
         300,
         2}
    },
    /* eCOUNTRY_CFG_GREECE */
    {
        "grc",
        "Ellinik� Dhimo.",
        0x212C,
        eCOUNTRY_CFG_LCN_TYPE_EACEM,
        kCOUNTRY_CFG_INSTALL_DEFAULT |
		kCOUNTRY_CFG_INSTALL_NID_PRIVATE_TMP_USE |
		kCOUNTRY_CFG_INSTALL_ONID_PRIVATE_TMP_USE,
		0,
#ifdef PRODUCT_USE_NX2_UI
        eCOUNTRY_CFG_SERVICE_IDENTIFIER_DAE,
#else
        eCOUNTRY_CFG_SERVICE_IDENTIFIER_DTF,
#endif
        2,
        {{"",0,2},{"",0,2}},
		NULL,
        {1, 9999, 10000, 20000},
        {(kCOUNTRY_CFG_EIT_LAST_SECTION_NUMBER |
          kCOUNTRY_CFG_EIT_LAST_TABLE_ID |
          kCOUNTRY_CFG_EIT_PRESENT_FOLLOWING_OTHER),
         7,
         20,
         30,
         300,
         2}
    },
    /* eCOUNTRY_CFG_BELGIUM */
    {
        "bel",
        "Belgium",
        0x2038,
        eCOUNTRY_CFG_LCN_TYPE_EACEM,
        kCOUNTRY_CFG_INSTALL_DEFAULT,
		0,
#ifdef PRODUCT_USE_NX2_UI
        eCOUNTRY_CFG_SERVICE_IDENTIFIER_DAE,
#else
        eCOUNTRY_CFG_SERVICE_IDENTIFIER_DTF,
#endif
        1,
        {{"",0,1},{"",0,1}},
		NULL,
        {1, 9999, 10000, 20000},
        {(kCOUNTRY_CFG_EIT_LAST_SECTION_NUMBER |
          kCOUNTRY_CFG_EIT_LAST_TABLE_ID |
          kCOUNTRY_CFG_EIT_PRESENT_FOLLOWING_OTHER),
         7,
         20,
         30,
         300,
         2}
    },
    /* eCOUNTRY_CFG_SWITZERLAND */
    {
        "che",
        "Suisse",
        0x22F4,
        eCOUNTRY_CFG_LCN_TYPE_EACEM,
        kCOUNTRY_CFG_INSTALL_DEFAULT,
		0,
#ifdef PRODUCT_USE_NX2_UI
        eCOUNTRY_CFG_SERVICE_IDENTIFIER_DAE,
#else
        eCOUNTRY_CFG_SERVICE_IDENTIFIER_DTF,
#endif

        1,
        {{"",0,1},{"",0,1}},
		NULL,
        {1, 9999, 10000, 20000},
        {(kCOUNTRY_CFG_EIT_LAST_SECTION_NUMBER |
          kCOUNTRY_CFG_EIT_LAST_TABLE_ID |
          kCOUNTRY_CFG_EIT_PRESENT_FOLLOWING_OTHER),
         7,
         20,
         30,
         300,
         2}
    },

    /* eCOUNTRY_CFG_INDIA */
    {
        "ind",
        "India",
        0x2B67,
        eCOUNTRY_CFG_LCN_TYPE_EACEM_AND_HD_SIMULCAST_MANAGEMENT,
        /*
        for some streams, for some demos, we need to check the ONID
        (kCOUNTRY_CFG_INSTALL_NID_PRIVATE_TMP_USE |
         kCOUNTRY_CFG_INSTALL_ONID_PRIVATE_TMP_USE)
        */
        /* network_ID in the range "private temporary use" are used in MHEG test streams */
        kCOUNTRY_CFG_INSTALL_NID_PRIVATE_TMP_USE,
		0,
        eCOUNTRY_CFG_SERVICE_IDENTIFIER_DAE,
        5,
        {{"",0,1},{"",0,1}},
		NULL,
        {1, 9999, 10000, 20000},
        {(kCOUNTRY_CFG_EIT_PRESENT_FOLLOWING_FLAG |
                kCOUNTRY_CFG_EIT_SCHEDULE_FLAG |
                kCOUNTRY_CFG_EIT_LAST_SECTION_NUMBER |
                kCOUNTRY_CFG_EIT_LAST_TABLE_ID |
                kCOUNTRY_CFG_EIT_SCHEDULE_OTHER |
                kCOUNTRY_CFG_EIT_PRESENT_FOLLOWING_OTHER),
         7,
         20,
         30,
         300,
         2}
    },

    /* eCOUNTRY_CFG_BRAZIL */
    {
        "bra",
        "Brazil",
        0x0001,
        eCOUNTRY_CFG_LCN_TYPE_EACEM_AND_HD_SIMULCAST_MANAGEMENT,
        /*
        for some streams, for some demos, we need to check the ONID
        (kCOUNTRY_CFG_INSTALL_NID_PRIVATE_TMP_USE |
         kCOUNTRY_CFG_INSTALL_ONID_PRIVATE_TMP_USE)
        */
        /* network_ID in the range "private temporary use" are used in MHEG test streams */
        kCOUNTRY_CFG_INSTALL_NID_PRIVATE_TMP_USE,
		0,
        eCOUNTRY_CFG_SERVICE_IDENTIFIER_DAE,
        -3,
        {{"",0,1},{"",0,1}},
		NULL,
        {1, 9999, 10000, 20000},
        {(kCOUNTRY_CFG_EIT_LAST_SECTION_NUMBER |
          kCOUNTRY_CFG_EIT_LAST_TABLE_ID |
          kCOUNTRY_CFG_EIT_PRESENT_FOLLOWING_OTHER),
         7,
         20,
         30,
         300,
         2}
    },
#endif /* UK_SPECIFIC */

#endif 
    /* eCOUNTRY_CFG_FACTORY_TEST */
    {
        "fac",
        "Factory Test",
        0,
        eCOUNTRY_CFG_LCN_TYPE_EACEM,
        kCOUNTRY_CFG_INSTALL_DEFAULT,
		0,
        eCOUNTRY_CFG_SERVICE_IDENTIFIER_DAE,
        1,
        {{"",0,1},{"",0,1}},
		NULL,
        {1, 9999, 10000, 20000},
        {(kCOUNTRY_CFG_EIT_LAST_SECTION_NUMBER |
          kCOUNTRY_CFG_EIT_LAST_TABLE_ID |
          kCOUNTRY_CFG_EIT_PRESENT_FOLLOWING_OTHER),
         7,
         20,
         30,
         300,
         2}
    }
};

/********************************************************/
/*              Local Functions Declarations (LOCAL)    */
/*******************************************************/
/********************************************************/
/*              Functions Definitions (LOCAL/GLOBAL)    */
/********************************************************/


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
uint32_t COUNTRY_CFG_GetNumberOfCountries(void)
{
    return kCOUNTRY_CFG_NB_COUNTRY;
}

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
char* COUNTRY_CFG_GetAvailableCountryName(uint32_t index)
{
    if (index >= kCOUNTRY_CFG_NB_COUNTRY)
    {
        return COUNTRY_CFG_countryInfo[COUNTRY_CFG_countryList[0]].name;
    }
    else
    {
        return COUNTRY_CFG_countryInfo[COUNTRY_CFG_countryList[index]].name;
    }
}

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
char* COUNTRY_CFG_GetAvailableCountryDVBtrigram(uint32_t index)
{
    if (index >= kCOUNTRY_CFG_NB_COUNTRY)
    {
        return COUNTRY_CFG_countryInfo[COUNTRY_CFG_countryList[0]].DVBtrigram;
    }
    else
    {
        return COUNTRY_CFG_countryInfo[COUNTRY_CFG_countryList[index]].DVBtrigram;
    }
}

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
char* COUNTRY_CFG_GetDefaultCountryDVBtrigram(void)
{
    return COUNTRY_CFG_countryInfo[kCOUNTRY_CFG_DEFAULT_COUNTRY].DVBtrigram;
}

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
char* COUNTRY_CFG_GetDefaultMenuLanguage(void)
{
    return kCOUNTRY_CFG_DEFAULT_MENU_LANGUAGE;
}

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
char* COUNTRY_CFG_GetDefaultAudioLanguage(void)
{
	return kCOUNTRY_CFG_DEFAULT_AUDIO_LANGUAGE;
}
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
char* COUNTRY_CFG_GetDefaultSubtitleLanguage(void)
{
	return kCOUNTRY_CFG_DEFAULT_SUBTITLE_LANGUAGE;
}

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
bool COUNTRY_CFG_GetDefaultSubtitleState(void)
{
	return kCOUNTRY_CFG_DEFAULT_SUBTITLE_STATE;
}

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
uint16_t COUNTRY_CFG_GetNumberOfMenuLanguage(void)
{
	return kCOUNTRY_CFG_NB_LANGUAGE_OF_MENU;
}

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
uint16_t COUNTRY_CFG_GetNumberOfLanguage(void)
{
	return kCOUNTRY_CFG_NB_USER_DEFAULT_LANGUAGE;
}

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
uint16_t COUNTRY_CFG_GetNumberOfAudioLanguage(void)
{
	return kCOUNTRY_CFG_NB_USER_AUDIO_LANGUAGE;
}

/*===========================================================================
 *
 * COUNTRY_CFG_GetMenuLanguageList
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
tCOUNTRY_CFG_Language* COUNTRY_CFG_GetMenuLanguageList(void)
{
	return COUNTRY_CFG_MenuLanguage;
}

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
tCOUNTRY_CFG_Language* COUNTRY_CFG_GetLanguageList(void)
{
	return COUNTRY_CFG_UserDefaultLanguage;
}

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
tCOUNTRY_CFG_Language* COUNTRY_CFG_GetAudioLanguageList(void)
{
	return COUNTRY_CFG_AudioLanguage;
}

/*===========================================================================
 *
 * COUNTRY_CFG_GetEntireLanguageList
 *
 * Parameters:
 *      none
 *
 * Description:
 *      Return the complete language list information
 *
 * Returns:
 *      pointer on allocated and filled tCOUNTRY_CFG_Language
 *
 *===========================================================================*/
tCOUNTRY_CFG_Language* COUNTRY_CFG_GetEntireLanguageList(uint16_t *nbLanguage)
{
	if (nbLanguage == NULL)
		return NULL;

	*nbLanguage = kCOUNTRY_CFG_NB_LANGUAGE;
	return COUNTRY_CFG_EntireLanguage;
}

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
char* COUNTRY_CFG_GetDayTranslation(uint16_t LanguageIndex, uint8_t DayIndex)
{
    /* days are indexed in the range 1-7 */
    /* translations of days are available in COUNTRY_CFG_periodicity, in the same range (index 0 is used for "once") */
	return COUNTRY_CFG_periodicity[LanguageIndex][DayIndex];
}

/*===========================================================================
 *
 * COUNTRY_CFG_GetPeriodicityTranslation
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
char* COUNTRY_CFG_GetPeriodicityTranslation(uint16_t LanguageIndex, uint8_t uiPeriodicity)
{
	return COUNTRY_CFG_periodicity[LanguageIndex][uiPeriodicity];
}

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
 *      the application (e.g. screen_5000.xml and screen_9100.xml)
 *
 *
 * Returns:
 *      Nothng
 *
 *===========================================================================*/
void COUNTRY_CFG_CheckEventContentNibble(uint8_t EventContent0, uint8_t EventContent1, uint8_t* pEventContentNibbleLevel1, uint8_t* pEventContentNibbleLevel2)
{
    uint8_t	nibble1 = (EventContent0& 0xF0)>>4;
#ifndef UK_SPECIFIC
    uint8_t	nibble2 = EventContent0& 0x0F;
#endif /* UK_SPECIFIC */

#ifdef UK_SPECIFIC
    if ((nibble1 == 1) ||
        (nibble1 == 2) ||
        (nibble1 == 3) ||
        (nibble1 == 4) ||
        (nibble1 == 5) ||
        (nibble1 == 6) ||
        (nibble1 == 7) ||
        (nibble1 == 8) ||
        (nibble1 == 9) ||
        (nibble1 == 10) ||
        (nibble1 == 15))
    {
        /* Use nibble1=15 to display the D-book user defined content strings */
        *pEventContentNibbleLevel1 = 15;
        *pEventContentNibbleLevel2 = nibble1;
    }
    else
    {
        *pEventContentNibbleLevel1 = 0;
        *pEventContentNibbleLevel2 = 0;
    }

#else /* UK_SPECIFIC */

    #define NIBBLE1_1_NB 9
    #define NIBBLE1_2_NB 5
    #define NIBBLE1_3_NB 4
    #define NIBBLE1_4_NB 12
    #define NIBBLE1_5_NB 6
    #define NIBBLE1_6_NB 7
    #define NIBBLE1_7_NB 12
    #define NIBBLE1_8_NB 4
    #define NIBBLE1_9_NB 8
    #define NIBBLE1_10_NB 8
    #define NIBBLE1_11_NB 4

	if (((nibble1 == 1) && (nibble2 < NIBBLE1_1_NB)) ||
	    ((nibble1 == 2) && (nibble2 < NIBBLE1_2_NB)) ||
	    ((nibble1 == 3) && (nibble2 < NIBBLE1_3_NB)) ||
	    ((nibble1 == 4) && (nibble2 < NIBBLE1_4_NB)) ||
	    ((nibble1 == 5) && (nibble2 < NIBBLE1_5_NB)) ||
	    ((nibble1 == 6) && (nibble2 < NIBBLE1_6_NB)) ||
	    ((nibble1 == 7) && (nibble2 < NIBBLE1_7_NB)) ||
	    ((nibble1 == 8) && (nibble2 < NIBBLE1_8_NB)) ||
	    ((nibble1 == 9) && (nibble2 < NIBBLE1_9_NB)) ||
	    ((nibble1 == 10) && (nibble2 < NIBBLE1_10_NB)) ||
	    ((nibble1 == 11) && (nibble2 < NIBBLE1_11_NB)))
	{
        *pEventContentNibbleLevel1 = nibble1;
        *pEventContentNibbleLevel2 = nibble2;
	}
	else
	{
        *pEventContentNibbleLevel1 = 0;
        *pEventContentNibbleLevel2 = 0;
	}

#endif /* UK_SPECIFIC */
}

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
void COUNTRY_CFG_SetCountryIndex(uint32_t index)
{
    if (index >= kCOUNTRY_CFG_NB_COUNTRY)
    {
        COUNTRY_CFG_CurrentCountry = COUNTRY_CFG_countryList[0];
    }
    else
    {
        COUNTRY_CFG_CurrentCountry = COUNTRY_CFG_countryList[index];
    }

    return;
}

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
uint32_t COUNTRY_CFG_GetCountryIndex(char* DVBtrigram)
{
    uint32_t i;

    for (i=0 ; i<kCOUNTRY_CFG_NB_COUNTRY ; i++)
    {
        if(strcmp(DVBtrigram, COUNTRY_CFG_countryInfo[COUNTRY_CFG_countryList[i]].DVBtrigram) == 0)
		{
			return(i);
		}
	}

    return(0);
}

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
uint16_t COUNTRY_CFG_GetCountryONID(char* DVBtrigram)
{
    uint32_t i;

    for (i=0 ; i<kCOUNTRY_CFG_NB_COUNTRY ; i++)
    {
        if(strcmp(DVBtrigram, COUNTRY_CFG_countryInfo[COUNTRY_CFG_countryList[i]].DVBtrigram) == 0)
		{
			return(COUNTRY_CFG_countryInfo[COUNTRY_CFG_countryList[i]].ONID);
		}
	}

    return(0xffff);
}

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
char* COUNTRY_CFG_GetCurrentCountryName(void)
{
    return COUNTRY_CFG_countryInfo[COUNTRY_CFG_CurrentCountry].name;
}

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
char* COUNTRY_CFG_GetCurrentCountryDVBtrigram(void)
{
    return COUNTRY_CFG_countryInfo[COUNTRY_CFG_CurrentCountry].DVBtrigram;
}

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
uint16_t COUNTRY_CFG_GetCurrentCountryONID(void)
{
    return COUNTRY_CFG_countryInfo[COUNTRY_CFG_CurrentCountry].ONID;
}

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
int COUNTRY_CFG_GetCurrentCountryLocalOffsetHour(void)
{
    return COUNTRY_CFG_countryInfo[COUNTRY_CFG_CurrentCountry].LocalHourOffset;
}

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
uint32_t COUNTRY_CFG_GetCurrentCountryNbRegions(void)
{
    int i;
    uint32_t count=0;

    for (i=0 ; i<kCOUNTRY_CFG_NB_REGION ; i++)
    {
        if (COUNTRY_CFG_countryInfo[COUNTRY_CFG_CurrentCountry].regions[i].regionID != 0)
        {
            count++;
        }
    }


    return count;
}

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
char* COUNTRY_CFG_GetCurrentCountryRegionName(uint32_t index)
{
    if (index >= kCOUNTRY_CFG_NB_REGION)
    {
        return COUNTRY_CFG_countryInfo[COUNTRY_CFG_CurrentCountry].regions[0].name;
    }
    else
    {
        return COUNTRY_CFG_countryInfo[COUNTRY_CFG_CurrentCountry].regions[index].name;
    }
}

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
uint16_t COUNTRY_CFG_GetCurrentCountryRegionID(uint32_t index)
{
    if (index >= kCOUNTRY_CFG_NB_REGION)
    {
        return COUNTRY_CFG_countryInfo[COUNTRY_CFG_CurrentCountry].regions[0].regionID;
    }
    else
    {
        return COUNTRY_CFG_countryInfo[COUNTRY_CFG_CurrentCountry].regions[index].regionID;
    }
}

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
int COUNTRY_CFG_GetCurrentCountryRegionLocalHourOffset(uint32_t index)
{
    if (index >= kCOUNTRY_CFG_NB_REGION)
    {
        return COUNTRY_CFG_countryInfo[COUNTRY_CFG_CurrentCountry].regions[0].LocalHourOffset;
    }
    else
    {
        return COUNTRY_CFG_countryInfo[COUNTRY_CFG_CurrentCountry].regions[index].LocalHourOffset;
    }
}

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
int COUNTRY_CFG_GetCurrentCountryRegionLocalHourOffsetByRegionID(uint32_t regionID)
{
    int i;

    for (i=0 ; i<kCOUNTRY_CFG_NB_REGION ; i++)
    {
        if(COUNTRY_CFG_countryInfo[COUNTRY_CFG_CurrentCountry].regions[i].regionID == regionID)
        {
            return COUNTRY_CFG_countryInfo[COUNTRY_CFG_CurrentCountry].regions[i].LocalHourOffset;
        }
    }

    return 0;
}

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
tCOUNTRY_CFG_LCN_Type COUNTRY_CFG_GetCountryLCNtype(void)
{
    return COUNTRY_CFG_countryInfo[COUNTRY_CFG_CurrentCountry].LCNtype;
}

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
uint32_t COUNTRY_CFG_GetCountryPrivateDataSpecifier(void)
{
    return COUNTRY_CFG_countryInfo[COUNTRY_CFG_CurrentCountry].ulPrivateDataSpecifier;
}

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
tCOUNTRY_CFG_ServiceIdentifierMode COUNTRY_CFG_GetCountryServiceIdentifierMode(void)
{
    return COUNTRY_CFG_countryInfo[COUNTRY_CFG_CurrentCountry].serviceIdentifierMode;
}

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
char* COUNTRY_CFG_GetProgStatusTranslation( uint16_t                LanguageIndex,
                                            tCOUNTRY_CFG_ProgStatus ProgStatus )
{
    uint8_t     uiPVRProgStatusIndex;

    uiPVRProgStatusIndex = ProgStatus;
    if ( uiPVRProgStatusIndex > eCOUNTRY_CFG_PROG_STATUS_UNKNOWN )
        uiPVRProgStatusIndex = eCOUNTRY_CFG_PROG_STATUS_UNKNOWN;

	return COUNTRY_CFG_ProgStatus[LanguageIndex][uiPVRProgStatusIndex];
}


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
                                        tCOUNTRY_CFG_ProgTarget Target )
{
    tCOUNTRY_CFG_ProgTarget targetIndex = Target;
    if ( Target >= eCOUNTRY_CFG_PROG_TARGET_UNKNWON )
        targetIndex = eCOUNTRY_CFG_PROG_TARGET_UNKNWON;

	return COUNTRY_CFG_ProgTarget[LanguageIndex][targetIndex];
}


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
tCOUNTRY_CFG_RankingList *COUNTRY_CFG_GetCountryRankingList(void)
{
	return COUNTRY_CFG_countryInfo[COUNTRY_CFG_CurrentCountry].pstRankingList;
}

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
                    tCOUNTRY_CFG_EIT_Requirements *EITRequirements)
{
    if (EITRequirements != NULL)
    {
        *EITRequirements =
            COUNTRY_CFG_countryInfo[COUNTRY_CFG_CurrentCountry].EITRequirements;
    }

    return;
}

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
uint32_t COUNTRY_CFG_GetInstallFlags(void)
{
	return COUNTRY_CFG_countryInfo[COUNTRY_CFG_CurrentCountry].ulInstallFlags;
}

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
bool
COUNTRY_CFG_CurrentCountrySupportMheg(void)
{
#ifdef INTERACTIVE_ENGINE_ON
return TRUE;
#else
return FALSE;
#endif
}

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
tCOUNTRY_CFG_Trigger_Type
COUNTRY_CFG_GetCurrentCountryTriggerType(void)
{
    tCOUNTRY_CFG_Trigger_Type trigger = eCOUNTRY_CFG_TRIGGER_TYPE_NONE;
#if defined(TRIG_DTG)
    trigger = eCOUNTRY_CFG_TRIGGER_TYPE_DTG;
#elif defined(TRIG_DVBSSU_ENHANCED)
    trigger = eCOUNTRY_CFG_TRIGGER_TYPE_DVB_SSU_ENHANCED;
#elif defined(TRIG_DVBSSU)
    trigger = eCOUNTRY_CFG_TRIGGER_TYPE_DVB_SSU;
#endif
    return trigger;
}

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
bool
COUNTRY_CFG_CurrentCountryUseFrequencyListDescriptor(void)
{
#ifdef UK_SPECIFIC
        return TRUE;
#else /* UK_SPECIFIC */
        return FALSE;
#endif /* UK_SPECIFIC */
}


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
void
COUNTRY_CFG_GetCountryChannelNumberVariant(uint16_t *pusFirstVariantRangeBegin,
                                           uint16_t *pusFirstVariantRangeEnd,
                                           uint16_t *pusSecondVariantRangeBegin,
                                           uint16_t *pusAnalogVariantRangeBegin)
{
    *pusFirstVariantRangeBegin  = COUNTRY_CFG_countryInfo[COUNTRY_CFG_CurrentCountry].stChannelNumberVariant.usFirstVariantRangeBegin;
    *pusFirstVariantRangeEnd    = COUNTRY_CFG_countryInfo[COUNTRY_CFG_CurrentCountry].stChannelNumberVariant.usFirstVariantRangeEnd;
    *pusSecondVariantRangeBegin = COUNTRY_CFG_countryInfo[COUNTRY_CFG_CurrentCountry].stChannelNumberVariant.usSecondVariantRangeBegin;
    *pusAnalogVariantRangeBegin = COUNTRY_CFG_countryInfo[COUNTRY_CFG_CurrentCountry].stChannelNumberVariant.usAnalogVariantRangeBegin;

    return;
}

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
                                              bool  *pbUseSignalQuality)
{
    /* Same setup for each European country */
    *pbUseNetworkId = false;
    *pbUseSignalQuality = true;

    return;
}
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
tCOUNTRY_CFG_CharacterTable
COUNTRY_CFG_CurrentCountryDefaultCharacterTable(void)
{
#ifndef UK_SPECIFIC

#if 0
        if ((COUNTRY_CFG_CurrentCountry == eCOUNTRY_CFG_FRANCE) ||
        (COUNTRY_CFG_CurrentCountry == eCOUNTRY_CFG_GERMANY) ||
        (COUNTRY_CFG_CurrentCountry == eCOUNTRY_CFG_SPAIN) ||
        (COUNTRY_CFG_CurrentCountry == eCOUNTRY_CFG_ITALY) ||
        (COUNTRY_CFG_CurrentCountry == eCOUNTRY_CFG_DENMARK) ||
        (COUNTRY_CFG_CurrentCountry == eCOUNTRY_CFG_NETHERLANDS) ||
        (COUNTRY_CFG_CurrentCountry == eCOUNTRY_CFG_FINLAND) ||
        (COUNTRY_CFG_CurrentCountry == eCOUNTRY_CFG_SWEDEN) ||
        (COUNTRY_CFG_CurrentCountry == eCOUNTRY_CFG_NORWAY) ||
        (COUNTRY_CFG_CurrentCountry == eCOUNTRY_CFG_ESTONIA) ||
        (COUNTRY_CFG_CurrentCountry == eCOUNTRY_CFG_ICELAND) ||
        (COUNTRY_CFG_CurrentCountry == eCOUNTRY_CFG_PORTUGAL) ||
        (COUNTRY_CFG_CurrentCountry == eCOUNTRY_CFG_BELGIUM) ||
        (COUNTRY_CFG_CurrentCountry == eCOUNTRY_CFG_SWITZERLAND))
    {
        return eCOUNTRY_CFG_CHAR_TABLE_ISO_IEC_8859_9;
    }
    else if (COUNTRY_CFG_CurrentCountry == eCOUNTRY_CFG_GREECE)
    {
        return eCOUNTRY_CFG_CHAR_TABLE_ISO_IEC_8859_7;
    }
    else if (COUNTRY_CFG_CurrentCountry == eCOUNTRY_CFG_SERBIA)
    {
        return eCOUNTRY_CFG_CHAR_TABLE_ISO_IEC_8859_5;
    }
    else
#endif

    return eCOUNTRY_CFG_CHAR_TABLE_ISO_IEC_8859_5;

#endif /* UK_SPECIFIC */
    {
        return eCOUNTRY_CFG_CHAR_TABLE_ISO_IEC_8859_5;
    }
}

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
 *      boolean
 *
 *===========================================================================*/
eCOUNTRY_CFG_PermanentSiMonitoringMode COUNTRY_CFG_GetPermanentSiMonitoringMode(void)
{
#ifdef PERMANENT_SI_MONITORING_NONE
    return eCOUNTRY_CFG_PERMANENT_SI_MONITORING_NONE;
#endif
#ifdef PERMANENT_SI_MONITORING_ON_TS
    return eCOUNTRY_CFG_PERMANENT_SI_MONITORING_ON_TS;
#endif
#ifdef PERMANENT_SI_MONITORING_ON_NETWORK
    return eCOUNTRY_CFG_PERMANENT_SI_MONITORING_ON_NETWORK;
#endif
#ifdef PERMANENT_SI_MONITORING_ON_COMPLETE_NETWORK
    return eCOUNTRY_CFG_PERMANENT_SI_MONITORING_ON_COMPLETE_NETWORK; //Included SDT other
#endif
    return eCOUNTRY_CFG_PERMANENT_SI_MONITORING_ON_COMPLETE_NETWORK;
}
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
uint32_t COUNTRY_CFG_GetPermanentSiMonitoringDelay(void)
{
    /* start 30 seconds after zapping */
    return 30;
}

