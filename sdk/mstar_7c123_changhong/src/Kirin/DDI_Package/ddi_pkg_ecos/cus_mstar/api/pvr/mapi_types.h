/**********************************************************************
 Copyright (c) 2006-2009 MStar Semiconductor, Inc.
 All rights reserved.

 Unless otherwise stipulated in writing, any and all information contained
 herein regardless in any format shall remain the sole proprietary of
 MStar Semiconductor Inc. and be kept in strict confidence
 (MStar Confidential Information) by the recipient.
 Any unauthorized act including without limitation unauthorized disclosure,
 copying, use, reproduction, sale, distribution, modification, disassembling,
 reverse engineering and compiling of the contents of MStar Confidential
 Information is unlawful and strictly prohibited. MStar hereby reserves the
 rights to any and all damages, losses, costs and expenses resulting therefrom.

* File  : mapi_types.h
**********************************************************************/

#ifndef __MAPI_TYPES_H__
#define __MAPI_TYPES_H__

#ifndef UNUSED
#define UNUSED(x) (void)x
#endif

// Types
typedef unsigned char       MAPI_BYTE;  // 1 byte
typedef unsigned char       MAPI_BOOL;  // 1 byte

typedef signed char         MAPI_S8;    // 1 byte
typedef signed short        MAPI_S16;   // 2 bytes
typedef signed long         MAPI_S32;   // 4 bytes
typedef signed long long    MAPI_S64;   // 8 bytes

typedef unsigned char       MAPI_U8;    // 1 byte
typedef unsigned short      MAPI_U16;   // 2 bytes
typedef unsigned long       MAPI_U32;   // 4 bytes
typedef unsigned long long  MAPI_U64;   // 8 bytes

typedef unsigned short      MAPI_WORD;  // 2 bytes

#define MAPI_TRUE 1 // true
#define MAPI_FALSE 0 // false

// PID
#define PVRMW_INVALID_PID                             0x1FFF //MSAPI_DMX_INVALID_PID

// BIT
#define PVRMW_BIT0  0x0001
#define PVRMW_BIT1  0x0002
#define PVRMW_BIT2  0x0004
#define PVRMW_BIT3  0x0008
#define PVRMW_BIT4  0x0010
#define PVRMW_BIT5  0x0020
#define PVRMW_BIT6  0x0040
#define PVRMW_BIT7  0x0080
#define PVRMW_BIT8  0x0100
#define PVRMW_BIT9  0x0200
#define PVRMW_BIT10 0x0400
#define PVRMW_BIT11 0x0800
#define PVRMW_BIT12 0x1000
#define PVRMW_BIT13 0x2000
#define PVRMW_BIT14 0x4000
#define PVRMW_BIT15 0x8000

/// ISO 639 Language
typedef struct _LANG_ISO639
{
    MAPI_U8 u8ISOLangInfo[3]; //    :24
    MAPI_U8 u8AudMode;  //          : 2    ///< 0x00: Stereo, 0x01: Mono right, 0x02: Mono left
    MAPI_U8 u8AudType; //           : 3    ///< 0x00: Undefined, 0x01: Clean effects, 0x02: Hearing impaired, 0x03: Visual impaired commentary, 0x04~0xFF: Reserved.
    MAPI_U8 u8IsValid; //           : 1    ///< Valid or not
} LANG_ISO639;

/// Audio information
#define PVRMW_MAX_AUD_LANG_NUM                        16 //NZ default 16
typedef struct _AUD_INFO
{
    LANG_ISO639 aISOLangInfo;//       :30    ///< ISO Language Info
    MAPI_U16 u16AudPID; //            :13    ///< Audio PID
    MAPI_U8 u8AudType; //             : 3    ///<  0x01: MPEG, 0x02: AC-3, 0x03: MPEG4_AUD
    //WORD Reserved                   : 1;   ///< Reserved
    MAPI_BOOL bBroadcastMixAD;
    ///AAC component type
    MAPI_U8 u8AACType;
    ///AAC profile and level
    MAPI_U8 u8AACProfileAndLevel;
} AUD_INFO;

/// the service type
typedef enum
{
    E_SERVICETYPE_ATV              = 0x00,      ///< Service type ATV
    E_SERVICETYPE_DTV              = 0x01,      ///< Service type DTV
    E_SERVICETYPE_RADIO            = 0x02,      ///< Service type Radio
    E_SERVICETYPE_DATA             = 0x03,        ///< Service type Data
    E_SERVICETYPE_UNITED_TV        = 0x04,      ///< Service type United TV
    E_SERVICETYPE_INVALID          = 0x05,      ///< Service type INVALID
} MEMBER_SERVICETYPE;

// For linux
#define MAX_CHANNEL_IN_MUX                      120
typedef struct
{
    MAPI_BYTE bBOOL_0    : 1;
    MAPI_BYTE bBOOL_1    : 1;
    MAPI_BYTE bBOOL_2    : 1;
    MAPI_BYTE bBOOL_3    : 1;
    MAPI_BYTE bBOOL_4    : 1;
    MAPI_BYTE bBOOL_5    : 1;
    MAPI_BYTE bBOOL_6    : 1;
    MAPI_BYTE bBOOL_7    : 1;
} MAPI_BOOLS;
typedef enum
{
    E_AUSTRALIA,                    ///< Australia
    E_AUSTRIA,                      ///< Austria
    E_BELGIUM,                      ///< Belgium
    E_BULGARIA,                     ///< Bulgaria
    E_CROATIA,                      ///< Croatia
    E_CZECH,                        ///< Czech
    E_DENMARK,                      ///< Denmark
    E_FINLAND,                      ///< Finland
    E_FRANCE,                       ///< France
    E_GERMANY,                      ///< Germany
    E_GREECE,                       ///< Greece
    E_HUNGARY,                      ///< Hungary
    E_ITALY,                        ///< Italy
    E_LUXEMBOURG,                   ///< Luxembourg
    E_NETHERLANDS,                  ///< Netherland
    E_NORWAY,                       ///< Norway
    E_POLAND,                       ///< Poland
    E_PORTUGAL,                     ///< Portugal
    E_RUMANIA,                      ///< Rumania
    E_RUSSIA,                       ///< Russia
    E_SERBIA,                       ///< Serbia
    E_SLOVENIA,                     ///< Slovenia
    E_SPAIN,                        ///< Spain
    E_SWEDEN,                       ///< Sweden
    E_SWITZERLAND,                  ///< Switzerland
    E_UK,                           ///< UK
    E_NEWZEALAND,                   ///< New Zealand
    E_ARAB,                         ///< Arab
    E_ESTONIA,                      ///< Estonia
    E_HEBREW,                       ///< Hebrew
    E_LATVIA,                       ///< Latvia
    E_SLOVAKIA,                     ///< Slovakia
    E_TURKEY,                       ///< Turkey
    E_IRELAND,                      ///< Ireland
    E_CHINA,                        ///< China
    E_TAIWAN,                       ///< Taiwan
    E_BRAZIL,                       ///< Brazil
    /* ATSC Country Start */
    E_CANADA,                       ///< Canada
    E_MEXICO,                       ///< Mexico
    E_US,                           ///< United States
    E_SOUTHKOREA,                    ///< South Korea
    /* ATSC Country End */
    E_OTHERS,                       ///< Others
    // ------------------------------------
    E_COUNTRY_NUM,

} MEMBER_COUNTRY;

typedef enum
{
    E_FORBIDDEN = 0,
    E_ASP_1TO1,       //    1 : 1
    E_ASP_4TO3,       //    4 : 3
    E_ASP_16TO9,      //   16 : 9
    E_ASP_221TO100,   // 2.21 : 1
    E_ASP_MAXNUM,
} EN_ASPECT_RATIO_CODE;
#endif // __MAPI_TYPES_H__


