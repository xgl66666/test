////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2009 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (¡§MStar Confidential Information¡¨) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////
#ifndef _MSAPI_OAD_PROCESS_C_
#define _MSAPI_OAD_PROCESS_C_

/*------------------------------------------------------------------------------
*   include files
*-----------------------------------------------------------------------------*/
// C Library
#include <common.h>
#include <exports.h>
#include "sys/common/MsTypes.h"
#include <asm/string.h>
#include <linux/string.h>
#include "msAPI_Memory.h"
#include "msAPI_oad_process.h"
#include "msAPI_oad_biop.h"

#if ATSC_SDDS_ENABLE
#define BASEADDRESS_SDDS_INFO_IN_FLASH  0x00500000      // Sync this address with HB's definition.
#define DBMemAlignWithUnit(n, unit)     ((((n)+(unit)-1)/(unit))*(unit))
#endif

#define BOOL    BOOLEAN
typedef MS_U16      UINT16;
#define BCD2Dec(x)      ((((x) >> 4) * 10) + ((x) & 0x0F))

#if 1//ENABLE_OAD
#define SECTION_LENGTH( h ) ( ( ( *( h + 1 ) & 0x0F ) << 8 ) | ( * ( h + 2 ) ) )

#define _BITMASKREFPOS  ( 0x80000000 )
#define _ISRCVDBLOCK( blknum ) ( ( OAD_ModInfo.blkchkbits[ blknum / 32 ] ) & ( _BITMASKREFPOS >> ( blknum % 32 ) ) )
#define _SETNEWBLOCK( blknum ) ( ( OAD_ModInfo.blkchkbits[ blknum / 32 ] ) |= ( _BITMASKREFPOS >> ( blknum % 32 ) ) )

#define DATA_BC_ID_SSU          0x000A // DVB-SSU
#define DATA_BC_ID_UK_EC        0x0111 // UK Engineering Channel
#define UNT_TAG_SCHED           0x01    // scheduling_descriptor
#define UNT_TAG_UPDAP           0x02    // updated_descriptor
#define UNT_TAG_SSULA           0x03    // ssu_location_descriptor

#define SECONDS_PER_MIN         60L
#define SECONDS_PER_HOUR        3600L
#define SECONDS_PER_DAY         86400L
#define DEFAULT_YEAR            1980
#define DEFAULT_MJD             44239

/*------------------------------------------------------------------------------
*   type define of variable types
*-----------------------------------------------------------------------------*/
typedef struct
{
    U32           moduleData;     // Pointer to raw module data

    U32           groupId;        // Transaction Id of DII
    U32           downloadId;
    U16           blockSize;      // Max size of blocks in this Module

    U16           moduleId;       // Id of this module
    U32           moduleSz;       // Size of this module
    U8            version;        // Version of this module

    U16           chkarraycnt;    // Number of bytes in 'blkchkbits'
    U32           *blkchkbits;    // Bit flag marking received blocks

    U16           totBlocks;      // Total number of blocks in module
    U16           rcvBlocks;      // Total received blocks in module
} __attribute__ ((packed)) OAD_MODULE;


#if ATSC_SDDS_ENABLE
extern MS_BOOL MDrv_SERFLASH_Read(MS_U32 u32FlashAddr, MS_U32 u32FlashSize, MS_U8 *user_buffer);
#endif

/*------------------------------------------------------------------------------
*   Global variables.
*-----------------------------------------------------------------------------*/
static OAD_MODULE  OAD_ModInfo ;
static U8          OAD_ModInfos_Size = 0;
static OAD_MODULE  *pOAD_ModInfos = NULL;
static U32      OAD_BlkChkBuffer[80]={0};
static U32      *pOAD_BlkChkBuffer=NULL, g_u32OAD_BlkChkBufferLen=0;
static U8      OAD_Mode = OAD_MODE_INVALID;
static BOOLEAN bIsOADFileFound = FALSE;
static U8 (*msAPI_OAD_VersionCheck)(U16,U8*)=NULL ;
static U8   versionNum = 0 ;
static U32  g_OAD_DDBBufferAddr;
static U32  g_OAD_DDBBufferSize;
static U32  g_OAD_MISVersionId;
static BIOP_BODY_PROFILE_IOR_INFO  OAD_DSI_IOR = {0, 0, 0, FALSE};
BIOP_BODY_PROFILE_IOR_INFO  OAD_OUI_DIR_IOR = {0, 0, 0, FALSE};
BIOP_BODY_PROFILE_IOR_INFO  OAD_FILE_IOR = {0, 0, 0, FALSE};
static U16 SolarDays[28] =
{
    0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365, 396, 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366, 397
};
static MSIF_Version _mw_oad_version = {
       .MW = { MW_OAD_VER },
};

U8 g_OAD_OUI[3]={'M', 'S', 'T'};
U8 OAD_OUI_ASCII[7];

extern MS_U32 MsOS_MPool_PA2KSEG1(MS_U32 pAddrPhys);
/*------------------------------------------------------------------------------
| Functions
*-----------------------------------------------------------------------------*/
static U8 hex_digit(U8 val)
{
    /* assert */
    if (val > 0x0F)
    {
        return '0';
    }

    return (val < 10) ? '0' + val : 'a' + (val - 10);
}

static U8* convert_key(U8 *key, U8 *ascii_key, U8 size)
{
    U8 i;

    /* convert to a string of hex byte values */
    for (i=0; i<size; i++)
    {
        ascii_key[i * 2] = hex_digit((key[i] >> 4) & 0x0f);
        ascii_key[(i * 2) + 1] = hex_digit(key[i] & 0x0f);
    }
    ascii_key[size * 2] = '\0';

    return ascii_key;
}

void _msAPI_OAD_ClearDynamicData( void )
{
    if((pOAD_BlkChkBuffer != NULL) || (g_u32OAD_BlkChkBufferLen > 0 ))
    {
        if(g_u32OAD_BlkChkBufferLen > 0)
        {
            free((void*)pOAD_BlkChkBuffer);
        }
        pOAD_BlkChkBuffer = NULL;
        g_u32OAD_BlkChkBufferLen = 0;
    }
}

void msAPI_OAD_SetOui(U8 byte1, U8 byte2, U8 byte3)
{
    g_OAD_OUI[0]=byte1;
    g_OAD_OUI[1]=byte2;
    g_OAD_OUI[2]=byte3;
    convert_key(g_OAD_OUI, OAD_OUI_ASCII, 3);
}

void msAPI_OAD_SetDDBBuffer(U32 u32Addr, U32 u32Size)
{
    g_OAD_DDBBufferAddr   =   u32Addr;
    g_OAD_DDBBufferSize   =   u32Size;
}

U32 msAPI_OAD_GetDDBBufferAddr()
{
    return g_OAD_DDBBufferAddr;
}

U32 msAPI_OAD_MemAlloc(U32 u32MemSize)
{
    U32 u32Addr;

    if (u32MemSize > g_OAD_DDBBufferSize)
        u32Addr = OAD_INVALID_ADDR;
    else
    {
        u32Addr = g_OAD_DDBBufferAddr;
        //msAPI_MIU_Clear(0, g_OAD_DDBBufferAddr, g_OAD_DDBBufferSize, 0x00);
    }

    return u32Addr;
}


void msAPI_OAD_SetVersionCheck(U8 (*cb)(U16,U8 *))
{
    msAPI_OAD_VersionCheck=cb ;
}

U8 msAPI_OAD_GetDsiVersionNum(void)
{
    return versionNum ;
}

static U8 msAPI_OAD_GetLeap ( U16 u16year )
{
    if ( u16year % 400 == 0 )
    {
        return 1;
    }
    else if ( u16year % 100 == 0 )
    {
        return 0;
    }
    else if ( u16year % 4 == 0 )
    {
        return 1;
    }

    return 0;
}

static U16 msAPI_OAD_GetDaysOfThisYear ( U16 u16Year )
{
    return msAPI_OAD_GetLeap( u16Year ) ? 366 : 365;
}

static U32 msAPI_OAD_MJDUTC2Seconds(U8 *pau8TDTData)
{
    U8 YY,MM,K;
    U16 u16MJD;
    U32 u32TotalSeconds;
    ST_TIME_OAD _stDate;

    u32TotalSeconds = 0;
    u16MJD = pau8TDTData[0] << 8 | pau8TDTData[1];
    if(u16MJD > DEFAULT_MJD)
    {
        YY = (U8) ((u16MJD - 15078.2) / 365.25);
        MM = (U8) ((u16MJD - 14956.1 - (U16)(YY * 365.25)) / 30.6001);
        K = ((MM == 14) || (MM == 15)) ? 1 : 0;

        _stDate.u16Year = (U16) (1900 + YY + K);
        _stDate.u8Month = (U8) (MM - 1 - (K * 12));
        _stDate.u8Day = (U8) (u16MJD - 14956 - (U16) (YY * 365.25) - (U16) (MM * 30.6001));
        _stDate.u8Hour = BCD2Dec(pau8TDTData[2]);
        _stDate.u8Min = BCD2Dec(pau8TDTData[3]);
        _stDate.u8Sec = BCD2Dec(pau8TDTData[4]);

        /* sec */
        u32TotalSeconds += _stDate.u8Sec;

        /* min */
        u32TotalSeconds += _stDate.u8Min * SECONDS_PER_MIN;

        /* hour */
        u32TotalSeconds += _stDate.u8Hour * SECONDS_PER_HOUR;

        /* day */
        u32TotalSeconds += (_stDate.u8Day - 1) * SECONDS_PER_DAY;

        /* month */
        u32TotalSeconds += SolarDays[msAPI_OAD_GetLeap(_stDate.u16Year) * 14 + _stDate.u8Month - 1] * SECONDS_PER_DAY;

        /* year */
        while(_stDate.u16Year > DEFAULT_YEAR)
        {
            _stDate.u16Year--;
            u32TotalSeconds += msAPI_OAD_GetDaysOfThisYear(_stDate.u16Year) * SECONDS_PER_DAY;
        }
    }

    return u32TotalSeconds;
}

static void msAPI_OAD_UntSchedule(BYTE *pSelector, UNT_DESCRIPTOR *untDescriptor)
{
	pSelector +=2;
    untDescriptor->untSchedule.u32StartTime      = msAPI_OAD_MJDUTC2Seconds( pSelector );    pSelector+=5;
    untDescriptor->untSchedule.u32EndTime        = msAPI_OAD_MJDUTC2Seconds( pSelector );    pSelector+=5;
    untDescriptor->untSchedule.u8FinalAvail      = (*pSelector & 0x80) >> 7;
    untDescriptor->untSchedule.u8PeriodFlag      = (*pSelector & 0x40) >> 6;
    untDescriptor->untSchedule.u8PeriodUnit      = (*pSelector & 0x30) >> 4;
    untDescriptor->untSchedule.u8DurationUnit    = (*pSelector & 0x0C) >> 2;
    untDescriptor->untSchedule.u8EstimateUnit    = (*pSelector & 0x03);   pSelector++;
    untDescriptor->untSchedule.u8Period          = *pSelector++;
    untDescriptor->untSchedule.u8Duration        = *pSelector++;
    untDescriptor->untSchedule.u8Estimate        = *pSelector++;

    OAD_DEBUG(printf("u32StartTime=%x\n",untDescriptor->untSchedule.u32StartTime)) ;
    OAD_DEBUG(printf("u32EndTime=%x\n",untDescriptor->untSchedule.u32EndTime)) ;
}

static void msAPI_OAD_UntLocation(BYTE *pSelector, UNT_DESCRIPTOR *untDescriptor)
{
    UINT16 data_broadcast_id;

    pSelector+=2;
    data_broadcast_id = GET_2BYTE( pSelector );     pSelector+=2;
    if (data_broadcast_id == DATA_BC_ID_SSU)
    {
        untDescriptor->untLocation.association_tag = GET_2BYTE( pSelector );   pSelector+=2;
        OAD_DEBUG(printf("association_tag=%x\n",untDescriptor->untLocation.association_tag)) ;
    }
}


OAD_STATUS_REPORT_TYPE msAPI_OAD_ProcessUNT( U8 *section , UNT_DESCRIPTOR *untDescriptor)
{
    U8 table_id, u8Tag;
    S32 section_length;
    U16 common_desploop_length, compat_desplength, platform_length, target_desplength, operal_desplength;
    U16 u16TotalParseLength, u16DescriptorLength;
    U8 *pu8Descriptor;
	U32  ouiData;
    BOOLEAN foundSchedule = 0 , foundLocation = 0 ;

    table_id = *section++;
    section_length = (S32)((UINT16)GET_2BYTE( section ) & 0x0FFF);   section+=2;

    if (section_length < 9)
    {
        OAD_ASSERT( printf("UNT Header length\n") );
        return OAD_FAIL;
    }

	section += 5;   section_length -= 5;
	ouiData	= GET_3BYTE( section );
	if(ouiData != GET_3BYTE(g_OAD_OUI))
	{
        OAD_ASSERT( printf("OUI error\n") );
		return OAD_FAIL;
	}
	section += 4;	 section_length -= 4;

    // common_descriptor_loop()
    common_desploop_length = ((UINT16)GET_2BYTE( section ) & 0x0FFF);
    if (section_length < common_desploop_length + 2)
    {
        OAD_ASSERT( printf("UNT common length\n") );
        return OAD_FAIL;
    }
    section += common_desploop_length + 2;    section_length -= (common_desploop_length + 2);

    // compatibilityDescriptor()
    compat_desplength = GET_2BYTE( section );
    if (section_length < compat_desplength + 2)
    {
        OAD_ASSERT( printf("UNT compat length\n") );
        return OAD_FAIL;
    }
	if ((msAPI_OAD_VersionCheck!=NULL)&&!(*msAPI_OAD_VersionCheck)(DATA_BC_ID_SSU,section)) // check version
	{
		return OAD_FAIL;
	}
    section += compat_desplength + 2;    section_length -= (compat_desplength + 2);

    // Platform
    platform_length = GET_2BYTE( section );
    if (section_length < platform_length + 2)
    {
        OAD_ASSERT( printf("UNT platform length\n") );
        return OAD_FAIL;
    }
    section += 2;    section_length -= 2;

    // Target Descriptor
    target_desplength = ((UINT16)GET_2BYTE( section ) & 0x0FFF);
    if (section_length < target_desplength + 2)
    {
        OAD_ASSERT( printf("UNT platform length\n") );
        return OAD_FAIL;
    }
    section += target_desplength + 2;    section_length -= (target_desplength + 2);

    // Operational Descriptor
    operal_desplength = ((UINT16)GET_2BYTE( section ) & 0x0FFF);
    if (section_length < operal_desplength + 2)
    {
        OAD_ASSERT( printf("UNT platform length\n") );
        return OAD_FAIL;
    }
    section += 2;    section_length -= 2;

    u16TotalParseLength = 0;
    pu8Descriptor = section;
    while(u16TotalParseLength < operal_desplength)
    {
        u8Tag = pu8Descriptor[0];   //descripter Tag
        switch(u8Tag)
        {
            case UNT_TAG_SCHED:
            {
               msAPI_OAD_UntSchedule(pu8Descriptor,untDescriptor);
               foundSchedule = 1 ;
            }
            break;

            case UNT_TAG_SSULA:
            {
                msAPI_OAD_UntLocation(pu8Descriptor,untDescriptor);
                foundLocation = 1 ;
            }
            break;

            default:
            break;
        }

        /* move to next descriptor */
        u16DescriptorLength = pu8Descriptor[1] + 2;
        u16TotalParseLength += u16DescriptorLength;
        pu8Descriptor += u16DescriptorLength;
    }

    if (foundLocation&&foundSchedule) return OAD_SUCCEED ;
    else return OAD_FAIL ;

}

OAD_STATUS_REPORT_TYPE msAPI_OAD_ProcessDSI( U8 *section )
{
    U16  section_length;

    // DSM MSG header
    U32  trans_id;
    U8  adaptLength;
    U16  messageLength;

    //U16 descriptorCount,model, version,i;
    //U8  descriptorType, descriptorLength, specifierType ,subDescriptorCount;
    //U32 specifierData;
    U16  numberOfGroups ,privateDataLength ,gCount, compatibilityLength;
    U32  groupId, groupSize;
    U16  groupInfoLength;
    OAD_STATUS_REPORT_TYPE retValue = OAD_FAIL;
    U8  *ptr;
    U16  messageId;

    ptr = section + SECTION_HEADER_SIZE + 2;
    messageId = GET_2BYTE(ptr);

    if (messageId!=OAD_DSMCC_MSGID_DSI)
    {
        OAD_ASSERT(printf("Not DSI => messageId=%x\n",messageId)) ;
        return OAD_FAIL;
    }

    section_length = SECTION_LENGTH( section );
    versionNum = (0x3e&section[5]) >> 1 ;
    section += SECTION_HEADER_SIZE;

    if ( section_length <= SECTION_HEADER_SIZE + DSMMSG_HEADER_SIZE + DSIMSG_HEADER_SIZE )
    {
        OAD_ASSERT( printf("Error> msAPI_OAD_ProcessDSI : dsmcc_section_length = 0x%x\n", section_length) );
        return OAD_FAIL;
    }

    // DSM MSG Header
    section += 4;   // protocolDiscriminator, dsmccType, messageId pass

    trans_id = GET_4BYTE( section );

    section += 5;   // trans_id, reserved pass

    adaptLength = *section++;
    messageLength = GET_2BYTE( section ); section += 2;

    section += adaptLength;


    // DSI Payload
    section += 20;  //serverId pass

    compatibilityLength = GET_2BYTE( section ); section += 2;
    section += compatibilityLength;

    privateDataLength = GET_2BYTE( section ); section += 2;

    if ( privateDataLength < 2 )
    {
        OAD_ASSERT( printf("Error> msAPI_OAD_ProcessDSI : privateDataLength = 0x%x\n", privateDataLength) );
        return OAD_FAIL;
    }

    ////////////////////////////////////////////////////////////////////////////
    // Here is privateDataByte
    ////////////////////////////////////////////////////////////////////////////
    // ServiceGatewayInfo() - UK Spec
    if ( section[4] == 's' && section[5] == 'r' && section[6] == 'g' && section[7] == 0x00 )
    {
        BIOP_USERINFO_MIS misinfo;

        OAD_DEBUG(printf("msAPI_OAD_ProcessDSI>> OAD_MODE_OBJCAR\n"));

        OAD_Mode = OAD_MODE_OBJCAR;
        process_biop_service_gateway_info(section);
        process_userinfo_GetMISInfo(&misinfo);
        g_OAD_MISVersionId = misinfo.version_id;

        OAD_DEBUG(printf("manufacturer_id>> TS = %lX, TV = %lX\n", misinfo.manufacturer_id, (U32)GET_3BYTE(g_OAD_OUI)));
        if ( misinfo.manufacturer_id == GET_3BYTE(g_OAD_OUI) )
        {
            process_serviceGateway_IOR_Get(&OAD_DSI_IOR);

            if ((msAPI_OAD_VersionCheck!=NULL)&&(*msAPI_OAD_VersionCheck)(DATA_BC_ID_UK_EC,misinfo.reserved)) // check version
            {
                OAD_ModInfo.groupId = process_profile_Body_GetTransID();
                return OAD_SUCCEED;
            }
            else
            {
                return OAD_FAIL;
            }
        }
        else return OAD_FAIL;
    }
    else
    {
        OAD_DEBUG(printf("msAPI_OAD_ProcessDSI>> OAD_MODE_DATACAR\n"));

        // GroupInfoIndication() - SSU Spec
        OAD_Mode = OAD_MODE_DATACAR;
        numberOfGroups = GET_2BYTE( section ); section += 2;

        for ( gCount = 0; gCount < numberOfGroups; gCount++ )
        {
            groupId     = GET_4BYTE( section ); section += 4;
            groupSize   = GET_4BYTE( section ); section += 4;

            // GroupCompatibility()
            compatibilityLength = GET_2BYTE( section );
            if ((msAPI_OAD_VersionCheck!=NULL)&&(*msAPI_OAD_VersionCheck)(DATA_BC_ID_SSU,section)) // check version
            {
               OAD_ModInfo.groupId = groupId;
               retValue = OAD_SUCCEED;
            }
            else
            {
                retValue = OAD_FAIL;
            }

            section += (2+compatibilityLength) ;

            // GroupInfoLength
            groupInfoLength = GET_2BYTE( section ); section += 2;
            section += groupInfoLength;

            if( (OAD_SUCCEED == retValue)&&(1 < numberOfGroups) )
            {
                //SSU_DEBUG( printf("%s@%d, found the matched group. [%d]th group of GroupNum[%d]. \n", __func__, __LINE__, gCount, numberOfGroups ));
                break;
            }

        }

        // privateDataLength
        privateDataLength = GET_2BYTE( section ); section += 2;
        //SSU_DEBUG( printf("%s @ %d, PrivateDataLength[%d]. \n", __func__, __LINE__, privateDataLength ) );
        section += privateDataLength;
    }
    return retValue;
}


OAD_STATUS_REPORT_TYPE msAPI_OAD_ProcessDII( U8 *section )
{
    U16  section_length;

    // DSM MSG header
    U32  trans_id;
    U8  adaptLength;
    U16  messageLength;

    // DII MSG header
    U32  downloadId;
    U16  blockSize;
    U16  compatibilityLength;
    U16  numberOfModules;

    // DII MSG body
    U16  moduleId;
    U32  moduleSz;
    U8  moduleVer;
    U8  moduleInfoLength;

    section_length = SECTION_LENGTH( section );
    section += SECTION_HEADER_SIZE;

    if ( section_length <= SECTION_HEADER_SIZE + DSMMSG_HEADER_SIZE + DIIMSG_HEADER_SIZE )
    {
        OAD_ASSERT( printf("Error> msAPI_OAD_ProcessDII : dsmcc_section_length = 0x%x\n", section_length) );
        return OAD_FAIL;
    }

    // DSM MSG Header
    section += 4;   // protocolDiscriminator, dsmccType, messageId pass

    trans_id = GET_4BYTE( section );
    OAD_DEBUG(printf("DII Trans_ID: 0x%08LX; GroupID: 0x%08LX\n", trans_id, OAD_ModInfo.groupId));

    if ( trans_id != OAD_ModInfo.groupId )
    {
        OAD_ASSERT( printf("Error> msAPI_OAD_ProcessDII : DII transactionId = 0x%lx, DSI groupId = 0x%lx\n", trans_id, OAD_ModInfo.groupId) );
        return OAD_FAIL;
    }

    section += 5;   // trans_id, reserved pass

    adaptLength = *section++;
    messageLength = GET_2BYTE( section ); section += 2;

    section += adaptLength;


    // DII Payload
    downloadId = GET_4BYTE( section ); section += 4;
    blockSize = GET_2BYTE( section ); section += 2;
    OAD_DEBUG(printf("\nDII DownloadID: 0x%08LX ; BlockSize: 0x%04X", downloadId, blockSize));

    if ( ( !blockSize ) || ( blockSize > 4066 ) )
    {
        OAD_ASSERT( printf("Error> msAPI_OAD_ProcessDII : invalid block size = %d\n", blockSize) );
        return OAD_FAIL;
    }

    if ( ( OAD_Mode == OAD_MODE_OBJCAR ) && ( downloadId != OAD_DSI_IOR.carousel_id ) )
    {
        OAD_ASSERT( printf("Error> Invalid downloadID: %x ; carousel_id: %x\n", downloadId , OAD_DSI_IOR.carousel_id) );
        return OAD_FAIL;
    }

    section += 10; // windowSize, ackPeriod, tCDownloadWindow, tCDownloadScenario pass

    compatibilityLength = GET_2BYTE( section ); section += 2;
    section += compatibilityLength;


    numberOfModules = GET_2BYTE( section ); section += 2;
    OAD_DEBUG(printf("Number of Mudule: 0x%04X\n", numberOfModules));

    if ( OAD_Mode == OAD_MODE_OBJCAR )
    {
        // Support Multiple ModuleID
        U8 i;
        BOOLEAN bFoundModuleID;

        if ( pOAD_ModInfos == NULL )
        {
            pOAD_ModInfos = (OAD_MODULE *) msAPI_Memory_Allocate(numberOfModules * sizeof(OAD_MODULE), BUF_ID_OAD_DOWNLOAD);
            OAD_ModInfos_Size = numberOfModules;
        }

        bFoundModuleID = FALSE;
        for (i=0; i< numberOfModules; i++)
        {
            moduleId = GET_2BYTE( section ); section += 2;
            moduleSz = GET_4BYTE( section ); section += 4;
            moduleVer  = *section++;
            moduleInfoLength = *section++;
            section += moduleInfoLength;

            pOAD_ModInfos[i].totBlocks   = ( moduleSz + blockSize - 1 ) / blockSize;
            pOAD_ModInfos[i].rcvBlocks   = 0;

            pOAD_ModInfos[i].chkarraycnt = ( pOAD_ModInfos[i].totBlocks + 31 ) / 32;
            if((pOAD_BlkChkBuffer == NULL) || (g_u32OAD_BlkChkBufferLen < (pOAD_ModInfos[i].chkarraycnt * sizeof( U32 ))))
            {
                if(g_u32OAD_BlkChkBufferLen > 0)
                    free((void*)pOAD_BlkChkBuffer);
                g_u32OAD_BlkChkBufferLen = pOAD_ModInfos[i].chkarraycnt * sizeof( U32 );
                pOAD_BlkChkBuffer = (U32*)malloc(g_u32OAD_BlkChkBufferLen);
                if(pOAD_BlkChkBuffer == NULL)
                {
                    pOAD_BlkChkBuffer = ( U32 * ) OAD_BlkChkBuffer;
                    g_u32OAD_BlkChkBufferLen = 0;
                }
            }
            pOAD_ModInfos[i].blkchkbits  = ( U32 * ) pOAD_BlkChkBuffer;
            memset(pOAD_ModInfos[i].blkchkbits, 0, pOAD_ModInfos[i].chkarraycnt * sizeof(U32));

            pOAD_ModInfos[i].downloadId  = downloadId;
            pOAD_ModInfos[i].blockSize   = blockSize;

            pOAD_ModInfos[i].moduleId    = moduleId;
            pOAD_ModInfos[i].moduleSz    = moduleSz;
            pOAD_ModInfos[i].version     = moduleVer;

            pOAD_ModInfos[i].moduleData  = OAD_INVALID_ADDR;

            if ( moduleId == OAD_DSI_IOR.module_id )
            {
                bFoundModuleID = TRUE;
                OAD_ModInfo = pOAD_ModInfos[i];
                OAD_DEBUG(printf("Find the Match ModuleID: %u\n", moduleId));
            }
        }

        if (bFoundModuleID == FALSE)
        {
            if ( pOAD_ModInfos != NULL )
            {
                msAPI_Memory_Free(pOAD_ModInfos, BUF_ID_OAD_DOWNLOAD);
                pOAD_ModInfos = NULL;
                OAD_ModInfos_Size = 0;
            }
            return OAD_FAIL;
        }
        else return OAD_SUCCEED ;
    }
    else if ( OAD_Mode == OAD_MODE_DATACAR )
    {
        if ( numberOfModules > 1 )
        {
            OAD_ASSERT( printf("Warning> msAPI_OAD_ProcessDII : invalid numberOfModules = %d\n", numberOfModules) );
            return OAD_FAIL;
        }

        OAD_DEBUG( printf("DII has %d modules, with blockSize %d\n", numberOfModules, blockSize) );

        moduleId    = GET_2BYTE( section ); section += 2;
        moduleSz    = GET_4BYTE( section ); section += 4;
        moduleVer   = *section++;

        OAD_ModInfo.totBlocks   = ( moduleSz + blockSize - 1 ) / blockSize;
        OAD_ModInfo.rcvBlocks   = 0;

        OAD_ModInfo.chkarraycnt = ( OAD_ModInfo.totBlocks + 31 ) / 32;
        if((pOAD_BlkChkBuffer == NULL) || (g_u32OAD_BlkChkBufferLen < (OAD_ModInfo.chkarraycnt * sizeof( U32 ))))
        {
            if(g_u32OAD_BlkChkBufferLen > 0)
                free((void*)pOAD_BlkChkBuffer);
            g_u32OAD_BlkChkBufferLen = OAD_ModInfo.chkarraycnt * sizeof( U32 );
            pOAD_BlkChkBuffer = (U32*)malloc(g_u32OAD_BlkChkBufferLen);
            if(pOAD_BlkChkBuffer == NULL)
            {
                pOAD_BlkChkBuffer = ( U32 * ) OAD_BlkChkBuffer;
                g_u32OAD_BlkChkBufferLen = 0;
            }
        }
        OAD_ModInfo.blkchkbits  = ( U32 * ) pOAD_BlkChkBuffer;
        memset( OAD_ModInfo.blkchkbits, 0, OAD_ModInfo.chkarraycnt * sizeof( U32 ) );

        OAD_ModInfo.downloadId  = downloadId;
        OAD_ModInfo.blockSize   = blockSize;

        OAD_ModInfo.moduleId    = moduleId;
        OAD_ModInfo.moduleSz    = moduleSz;
        OAD_ModInfo.version     = moduleVer;
        OAD_ModInfo.moduleData  = OAD_INVALID_ADDR;
        return OAD_SUCCEED;
    }
    else
    {
        return OAD_FAIL;
    }

    OAD_DEBUG( printf("NEW_M: Did=0x%lx, Mid=0x%04x, Mver=0x%02bx, Msize=%ld, blksz=%d totblkcnt=%d\n",
        downloadId, moduleId, moduleVer, moduleSz, blockSize, OAD_ModInfo.totBlocks ) );
}

OAD_STATUS_REPORT_TYPE msAPI_OAD_ProcessDDB(U8 *section,U16 *blockID,U16 *msgLen,U32 *msgAddr,U16 *msgType)
{
    U16  section_length;

    // DSM MSG header
    U32  downloadId;
    U8  adaptLength;
    U16  messageLength;

    // DDB MSG header
    U16  moduleId;
    U8  moduleVer;
    U16  blockNumber;

    OAD_STATUS_REPORT_TYPE retValue = OAD_FAIL;

    section_length = SECTION_LENGTH( section );
    section += SECTION_HEADER_SIZE;

    if ( section_length <= SECTION_HEADER_SIZE + DSMMSG_HEADER_SIZE + DDBMSG_HEADER_SIZE )
    {
        OAD_ASSERT( printf("Error> msAPI_OAD_ProcessDDB : dsmcc_section_length = 0x%x\n", section_length) );
        goto msAPI_OAD_ProcessDDB_Fail;
    }

    // DSM Download Data Header
    OAD_DEBUG(printf("protocolDiscriminator = 0x%02bX\n", *(section+0)));
    OAD_DEBUG(printf("dsmccType = 0x%02bX\n", *(section+1)));
    OAD_DEBUG(printf("messageId = 0x%04X\n", GET_2BYTE((section+2))));
    section += 4;   // protocolDiscriminator, dsmccType, messageId pass

    downloadId = GET_4BYTE( section );
    OAD_DEBUG(printf("DDB downloadId: 0x%08LX\n", downloadId));

    section += 5;   // downloadId, reserved pass

    adaptLength = *section++;
    messageLength = GET_2BYTE( section ); section += 2;

    section += adaptLength;

    // DDB Payload
    moduleId = GET_2BYTE( section ); section += 2;
    moduleVer = *section++;
    section++;      // reserved pass
    blockNumber = GET_2BYTE( section ); section += 2;

    if (( OAD_ModInfo.downloadId != downloadId ) ||
        ( OAD_ModInfo.moduleId != moduleId ) ||
        ( OAD_ModInfo.version != moduleVer ) ||
        ( OAD_ModInfo.totBlocks == OAD_ModInfo.rcvBlocks ) ||
        ( OAD_ModInfo.totBlocks <= blockNumber ) )
    {
        OAD_DEBUG(printf("Invalid DDB - Did=0x%lx(0x%lx), Mid=0x%x(0x%x), Mver=0x%02bx(0x%02bx), BlkNo=0x%x(0x%x/0x%x)\n",
                  OAD_ModInfo.downloadId, downloadId, OAD_ModInfo.moduleId, moduleId, OAD_ModInfo.version, moduleVer,
                  blockNumber, OAD_ModInfo.rcvBlocks, OAD_ModInfo.totBlocks) );
        return OAD_FAIL;
    }

    *blockID = 0 ;
    *msgLen = 0 ;

    if (!_ISRCVDBLOCK(blockNumber))
    {
        U8  *pSrc;
        U16  msgSize = messageLength - adaptLength - 6;

        _SETNEWBLOCK( blockNumber );

        pSrc = section;

        if ((OAD_Mode==OAD_MODE_OBJCAR))
        {
           if (OAD_ModInfo.moduleId==OAD_FILE_IOR.module_id)
           {
              if (blockNumber==0)
              {
                 pSrc += BIOP_HEADER_LEN ;
                 msgSize -= BIOP_HEADER_LEN ;
              }
              *msgLen = msgSize ;
           }
        }
        else if (OAD_Mode==OAD_MODE_DATACAR)
        {
           *msgLen = msgSize ;
        }

        *blockID = blockNumber ;
        *msgAddr = (U32)pSrc ;
        *msgType = OAD_Mode ;

        //memcpy((U8*)msAPI_OAD_GetDDBBufferAddr(),(U8*)pSrc,msgSize) ;

        OAD_ModInfo.rcvBlocks++;

        OAD_INFO( printf("MID=0x%x, BNO=%u[%u/%u]\n", OAD_ModInfo.moduleId, blockNumber, OAD_ModInfo.rcvBlocks, OAD_ModInfo.totBlocks) );

        if ((OAD_ModInfo.totBlocks == OAD_ModInfo.rcvBlocks )&&(OAD_ModInfo.totBlocks != 0))
        {
            //OAD_DEBUG( printf("[OAD] MODULE Complete[0x%x] Size=%ld\n", OAD_ModInfo.moduleId, moduleSz) );

            if ( OAD_Mode == OAD_MODE_OBJCAR )
            {
                U8 Buf[4];
                U32 dstaddr;

                if (OAD_ModInfo.moduleId == OAD_DSI_IOR.module_id)
                {
                   dstaddr = (U32)Buf;
                   memcpy((U8*)(dstaddr),(U8*)(*msgAddr),(U32)(BIOP_MAGIC_LEN)) ;
                   if ( strncmp((const char *)(Buf), BIOP_MAGIC_STR, BIOP_MAGIC_LEN) != 0 )
                   {
                       goto msAPI_OAD_ProcessDDB_Fail;
                   }

                   if (process_biop(*msgAddr, (U32)(OAD_ModInfo.moduleSz), SDRAM_TYPE))
                   {
                      U8 i = 0;

                      for (i=0; i<OAD_ModInfos_Size; i++)
                      {
                          if ( pOAD_ModInfos[i].moduleId == OAD_FILE_IOR.module_id )
                          {
                              OAD_ModInfo = pOAD_ModInfos[i];
                              if((pOAD_BlkChkBuffer == NULL) || (g_u32OAD_BlkChkBufferLen < (OAD_ModInfo.chkarraycnt * sizeof( U32 ))))
                              {
                                  if(g_u32OAD_BlkChkBufferLen > 0)
                                      free((void*)pOAD_BlkChkBuffer);
                                  g_u32OAD_BlkChkBufferLen = OAD_ModInfo.chkarraycnt * sizeof( U32 );
                                  pOAD_BlkChkBuffer = (U32*)malloc(g_u32OAD_BlkChkBufferLen);
                                  if(pOAD_BlkChkBuffer == NULL)
                                  {
                                      pOAD_BlkChkBuffer = ( U32 * ) OAD_BlkChkBuffer;
                                      g_u32OAD_BlkChkBufferLen = 0;
                                  }
                              }
                              OAD_ModInfo.blkchkbits  = ( U32 * ) pOAD_BlkChkBuffer;
                              memset(OAD_ModInfo.blkchkbits, 0, OAD_ModInfo.chkarraycnt * sizeof(U32));
                              retValue = OAD_CONTINUE;
                              break;
                          }
                      }
                   }
                   else
                   {
                      goto msAPI_OAD_ProcessDDB_Fail;
                   }
                }
                else if (OAD_ModInfo.moduleId == OAD_FILE_IOR.module_id)
                {
                    retValue = OAD_COMPLETE;
                }
            }
            else if ( OAD_Mode == OAD_MODE_DATACAR )
            {
                retValue = OAD_COMPLETE;
            }
        }
        else
        {
           retValue = OAD_CONTINUE;
        }
    }
    else
    {
        retValue = OAD_CONTINUE;
        OAD_DEBUG( printf("OLD BLOCK: MID=0x%x, BNO=0x%x\n", moduleId, blockNumber) );
    }
    if(retValue == OAD_FAIL || retValue == OAD_COMPLETE)
    {
        _msAPI_OAD_ClearDynamicData();
    }
    return retValue ;
msAPI_OAD_ProcessDDB_Fail:
    return OAD_FAIL ;
}

U8 msAPI_OAD_GetProgress( void )
{
    U8  ret = 0;

    if ( OAD_ModInfo.totBlocks )
    {
        ret = 100 * (float)OAD_ModInfo.rcvBlocks / OAD_ModInfo.totBlocks;
    }

    OAD_DEBUG( printf("OAD Progress %d/%d (%d%%)\n", OAD_ModInfo.rcvBlocks, OAD_ModInfo.totBlocks, ret) );

    return ret;
}

void msAPI_OAD_GetModuleInfo( U32 *startAddr, U32 *moduleSize )
{
    *startAddr  = OAD_ModInfo.moduleData;
    *moduleSize = OAD_ModInfo.moduleSz;
}

U32 msAPI_OAD_GetMISVersionId( void )
{
    OAD_DEBUG( printf("OAD g_OAD_MISVersionId 0x%08X\n", g_OAD_MISVersionId) );

    return g_OAD_MISVersionId;
}

void msAPI_OAD_ClearData( void )
{
    _msAPI_OAD_ClearDynamicData();
    memset( &OAD_OUI_DIR_IOR, 0, sizeof( OAD_OUI_DIR_IOR ) );
    memset( &OAD_FILE_IOR, 0, sizeof( OAD_FILE_IOR ) );
    memset( &OAD_ModInfo, 0, sizeof( OAD_MODULE ) );

    OAD_ModInfo.moduleData  = OAD_INVALID_ADDR;
    OAD_Mode                = OAD_MODE_INVALID;
    g_OAD_MISVersionId      = 0;
}

U8 msAPI_OAD_GetOADMode(void)
{
    return OAD_Mode;
}

void msAPI_OAD_SetOADFileFound(BOOLEAN bFound)
{
    OAD_DEBUG(printf("msAPI_OAD_SetOADFileFound: %bu\n", bFound));
    bIsOADFileFound = bFound;
}

BOOL msAPI_OAD_IsOADFileFound(void)
{
    OAD_DEBUG(printf("msAPI_OAD_IsOADFileFound: %bu\n", bIsOADFileFound));
    return bIsOADFileFound;
}

BOOL msAPI_OAD_GetLibVer(const MSIF_Version **ppVersion)
{
    if (!ppVersion)
    {
        return false;
    }

    *ppVersion = &_mw_oad_version;

    return true ;
}

#else //(!ENABLE_OAD)
void msAPI_OAD_DummyPrint( void )
{
    //This dummy function is just used to pass compiler.
    printf("\n");
}
#endif //(!ENABLE_OAD)

#if ATSC_SDDS_ENABLE
MS_BOOL MApp_LoadSDDSInfoFromFlash(MS_SDDS_PARAM_2_BL *SDDSinfo)
{
    MS_BOOL bStatus = FALSE;

    // Read SDDS information from flash.
    bStatus = MDrv_SERFLASH_Read(BASEADDRESS_SDDS_INFO_IN_FLASH, DBMemAlignWithUnit(sizeof(MS_SDDS_PARAM_2_BL), 4), (MS_U8 *)SDDSinfo);

    OAD_DEBUG( printf(" MApp_LoadSDDSInfoFromFlash  u16SDDS_PID = %d u32Freq = %d flag = %u \n",SDDSinfo->u16SDDS_PID,SDDSinfo->u32SDDS_Frequency,SDDSinfo->bFoundNewSDDS_4BL_Download));
    OAD_DEBUG( printf(" MApp_LoadSDDSInfoFromFlash SW = %u, HW = %u, SchCount = %u, SchTime = %lu  Timeleng = %lu \n",SDDSinfo->u16SDDS_SW_Verion,SDDSinfo->u16SDDS_HW_Verion,SDDSinfo->u8ScheduleDescCount,SDDSinfo->u32SchStartTime,SDDSinfo->u32SchLengthInSeconds));
    OAD_DEBUG( printf(" MApp_LoadSDDSInfoFromFlash SwMID = %u, HwMID = %u, OUI = %u \n",SDDSinfo->u16SDDS_SW_ModelID,SDDSinfo->u16SDDS_HW_ModelID,SDDSinfo->u32SDDS_OUI));

    return bStatus;
}
#endif

#undef _MSAPI_OAD_PROCESS_C_
#endif //_MSAPI_OAD_PROCESS_C_

