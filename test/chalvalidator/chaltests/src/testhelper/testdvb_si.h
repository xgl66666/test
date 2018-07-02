/*****************************************************************************

File name   :  dvb_si.h

Description :  DVB SI Information

COPYRIGHT (C) ST-Microelectronics 2004.

*****************************************************************************/

/* --- prevents recursive inclusion --------------------------------------- */

#ifndef __TESTDVB_SI_H
#define __TESTDVB_SI_H

/* Definitions ------------------------------------------------------------ */

/* dvb section header indices */
#define TABLE_IDX      0
#define PROGRAML_IDX   3
#define PROGRAMH_IDX   4
#define VERSION_IDX    5
#define SECTION_IDX    6
#define LASTSECT_IDX   7

/* table id bit positions */
#define TID_DONT_CARE                      ((U32)1<<31)
#define TID_MASK                           (U32)0xff
#define SET_TID(_x_,_tid_)                 (_x_ = (_tid_ & TID_MASK))
#define SET_TID_DONT_CARE(_x_)             (_x_ = TID_DONT_CARE)

/* version number bit positions */
#define VERSION_DONT_CARE                  ((U32)1<<6)
#define VERSION_NUMBER_MASK                0x1f    /* only 5 bit version number */
#define VERSION_NUMBER_CLEAR               (~(VERSION_NUMBER_MASK << 1))
#define INVALID_VERSION_NUMBER             0x80
#define GET_VERSION_NUMBER(_x_)            ((_x_>>1) & VERSION_NUMBER_MASK)
#define SET_VERSION_NUMBER(_x_,_v_)        {(_x_ &= ~VERSION_DONT_CARE); \
                                            (_x_ &= VERSION_NUMBER_CLEAR); \
                                            (_x_ |= ((_v_ & VERSION_NUMBER_MASK)<<1)); }
#define SET_VERSION_NUMBER_DONT_CARE(_x_)  (_x_ |= VERSION_DONT_CARE)

/* current next bit positions */
#define CURRENT_OR_NEXT_DONT_CARE          ((U32)1<<7)
#define CURRENT_NEXT_INDICATOR_MASK        (1<<0)
#define NEXT_TABLE                         (0<<0)
#define CURRENT_TABLE                      (1<<0)
#define SET_CURRENT_TABLE(_x_)             {(_x_ &= ~CURRENT_OR_NEXT_DONT_CARE); \
                                            (_x_ &= ~CURRENT_NEXT_INDICATOR_MASK); \
                                            (_x_ |= CURRENT_TABLE);}
#define SET_NEXT_TABLE(_x_)                {(_x_ &= ~CURRENT_OR_NEXT_DONT_CARE); \
                                            (_x_ &= ~CURRENT_NEXT_INDICATOR_MASK); \
                                            (_x_ |= NEXT_TABLE);}
#define SET_CURRENT_OR_NEXT_DONT_CARE(_x_) (_x_ |= CURRENT_OR_NEXT_DONT_CARE)

/* section number bit positions */
#define SECTION_DONT_CARE                  ((U32)1<<31)
#define SECTION_NUMBER_MASK                (U32)0xff
#define SET_SECTION_NUMBER(_x_,_sn_)       (_x_ = (_sn_ & SECTION_NUMBER_MASK))
#define SET_SECTION_NUMBER_DONT_CARE(_x_)  (_x_ |= SECTION_DONT_CARE)

/* program number bit positions */
#define PROGRAM_DONT_CARE                  ((U32)1<<31)
#define PROGRAM_NUMBER_MASK                (PROGRAM_NUMBER_MSB_MASK | PROGRAM_NUMBER_LSB_MASK)
#define PROGRAM_NUMBER_MSB_MASK            0x0000ff00
#define PROGRAM_NUMBER_LSB_MASK            0x000000ff
#define SET_PROGRAM_NUMBER(_x_,_pn_)       (_x_ = (_pn_ & PROGRAM_NUMBER_MASK))
#define SET_PROGRAM_NUMBER_DONT_CARE(_x_)  (_x_ |= PROGRAM_DONT_CARE)

/*
** Table id defined by SI standard
*/
#define PAT_TABLE_ID            0x00  /* Program Association Table */
#define CAT_TABLE_ID            0x01  /* Conditional Access Table */
#define PMT_TABLE_ID            0x02  /* Program Map Table */
#define NIT_TABLE_ID            0x40  /* Network Info Table (actual transport) */
#define NITO_TABLE_ID           0x41  /* Network Info Table (other transport) */
#define SDT_TABLE_ID            0x42  /* Service Descriptor Table (actual transport) */
#define SDTO_TABLE_ID           0x46  /* Service Descriptor Table (other transport) */
#define BAT_TABLE_ID            0x4a  /* Bouquet Association Table */
#define EIT_TABLE_ID            0x4e  /* Event Information Table (actual transport) */
#define EITO_TABLE_ID           0x4f  /* Event Information Table (other transport) */
#define TDT_TABLE_ID            0x70  /* Time Date Table */
#define RST_TABLE_ID            0x71  /* Running Status Table */
#define ST_TABLE_ID             0x72  /* Stuffing Table */
#define TOT_TABLE_ID            0x73  /* Time Offset Table */
#define DIT_TABLE_ID            0x7E  /* Discontinuity Information Table */
#define SIT_TABLE_ID            0x7F  /* Selection Information Table */
#define INVALID_TABLE           0x100

/*
** User defined table ids
*/
#define PRIVATE_TABLE_ID_LOW    0x80
#define PRIVATE_TABLE_ID        0xFE

/*
** PID definitions
*/
#define PAT_PID                 0x0000
#define CAT_PID                 0x0001
#define NIT_PID                 0x0010
#define SDT_PID                 0x0011
#define BAT_PID                 0x0011
#define EIT_PID                 0x0012
#define RST_PID                 0x0013
#define TDT_PID                 0x0014
#define TOT_PID                 0x0014
#define DIT_PID                 0x001E
#define SIT_PID                 0x001F

/*
** Descriptor Definitions
*/
#define ECM_DESCRIPTOR                  0x09
#define ISO_639_LANGUAGE_DESCRIPTOR     0x0A

#define NETWORK_NAME_DESCRIPTOR         0x40
#define SERVICE_LIST_DESCRIPTOR         0x41
#define STUFFING_DESCRIPTOR             0x42
#define SATELLITE_DELIVERY_DESCRIPTOR   0x43
#define CABLE_DELIVERY_DESCRIPTOR       0x44
#define BOUQUET_NAME_DESCRIPTOR         0x47
#define SERVICE_DESCRIPTOR              0x48
#define COUNTRY_AVAILABILITY_DESCRIPTOR 0x49
#define LINKAGE_DESCRIPTOR              0x4A
#define NVOD_REFERENCE_DESCRIPTOR       0x4B
#define TIME_SHIFTED_SERVICE_DESCRIPTOR 0x4C
#define SHORT_EVENT_DESCRIPTOR          0x4D
#define EXTENDED_EVENT_DESCRIPTOR       0X4E
#define TIME_SHIFTED_EVENT_DESCRIPTOR   0x4F
#define COMPONENT_DESCRIPTOR            0x50
#define MOSAIC_DESCRIPTOR               0x51
#define STREAM_IDENTIFIER_DESCRIPTOR    0x52
#define CA_IDENTIFIER_DESCRIPTOR        0x53
#define CONTENT_DESCRIPTOR              0x54
#define PARENTAL_RATING_DESCRIPTOR      0x55
#define TELETEXT_DESCRIPTOR             0x56
#define TELEPHONE_DESCRIPTOR            0x57
#define LOCAL_TIME_OFFSET_DESCRIPTOR    0x58
#define SUBTITLING_DESCRIPTOR           0x59
#define TERRESTRIAL_DELIVERY_DESCRIPTOR 0x5A
#define MULTILINGUAL_NETWORK_NAME_DESCRIPTOR    0x5B
#define MULTILINGUAL_BOUQUET_NAME_DESCRIPTOR    0x5C
#define MULTILINGUAL_SERVICE_NAME_DESCRIPTOR    0x5D
#define MULTILINGUAL_COMPONENT_DESCRIPTOR       0x5E
#define PRIVATE_DATA_SPECIFIER_DESCRIPTOR       0x5F
#define SERVICE_MOVE_DESCRIPTOR         0x60
#define SMOOTHING_BUFFER_DESCRIPTOR     0x61
#define FREQUENCY_LIST_DESCRIPTOR       0x62
#define PARTIAL_TS_DESCRIPTOR           0x63
#define DATA_BROADCAST_DESCRIPTOR       0x64
#define DATA_BROADCAST_ID_DESCRIPTOR    0x66

/*
** General Packet definitions
*/
#define TS_PACKET_LENGTH                188

/*
** Section Data sizes (in bytes)
*/
#define CRC_SIZE                        4
#define EIT_HEAD_SIZE                   14
#define EIT_DATA_SIZE                   12
#define PAT_HEAD_SIZE                   8
#define PAT_DATA_SIZE                   4
#define PMT_HEAD_SIZE                   12
#define PMT_DATA_SIZE                   5
#define NIT_HEAD_SIZE                   10
#define NIT_DATA_SIZE                   6
#define SDT_HEAD_SIZE                   11
#define SDT_DATA_SIZE                   5

/*
**  masks & data extraction macros
*/
#define LENGTH_MASK             0x0FFF
#define PID_MASK                0x1FFF
#define PROG_MASK               0xFFFF
#define extract(x,m)            (((*x<<8) + *(x+1)) & m)
#define extractBCD(x)           (((((*x) & 0xf0)>>4)*10) + ((*x)&0xf))

#define MAX_SERVICE_NAME_LENGTH 20
#define MAX_SERVICES            30

/* descriptor structures */

typedef struct {
    uint8_t Type;
    uint8_t ProviderLen;
    char *  ProviderName_p; /* not null terminated */
    uint8_t ServiceLen;
    char *  ServiceName_p; /* not null terminated */
} tTestHelper_ServiceInfo;

typedef struct {
    char    Language[4];
    uint8_t NameLen;
    char *  Name_p;
    uint8_t TextLen;
    char *  Text_p;
} tTestHelper_ShortEventInfo;

typedef struct {
    char     Language[4];
    uint8_t  Type;
    uint16_t CompositionId;
    uint16_t AnciliaryId;
} tTestHelper_SubtitleInfo;

typedef struct {
    char    Language[4];
    uint8_t Type;
    uint8_t Magazine;
    uint8_t Page;
} tTestHelper_TeletextInfo;

typedef struct tTestHelper_ServiceInfo_s
{
    uint16_t ProgNo;
    uint16_t PmtPid;
    char     ServiceName[MAX_SERVICE_NAME_LENGTH];
} tTestHelper_ServiceListInfo;

typedef struct tTestHelper_ServiceList_s
{
    uint16_t                    NoOfServices;
    tTestHelper_ServiceListInfo ServiceInfo[MAX_SERVICES];
} tTestHelper_ServiceList;


typedef struct {
    uint8_t Tag;
    uint8_t Length;
    union {
        uint16_t                   ECMPid;
        tTestHelper_ServiceInfo    Service;
        tTestHelper_ShortEventInfo ShortEvent;
        tTestHelper_SubtitleInfo   Subtitle;
        tTestHelper_TeletextInfo   Teletext[2];
    } Info;
} tTestHelper_DescriptorInfo;

#endif /* #ifndef __TESTDVB_SI_H */

/* EOF */
