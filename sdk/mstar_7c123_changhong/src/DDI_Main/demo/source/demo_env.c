//<MStar Software>
//******************************************************************************
// MStar Software
// Copyright (c) 2010 - 2012 MStar Semiconductor, Inc. All rights reserved.
// All software, firmware and related documentation herein ("MStar Software") are
// intellectual property of MStar Semiconductor, Inc. ("MStar") and protected by
// law, including, but not limited to, copyright law and international treaties.
// Any use, modification, reproduction, retransmission, or republication of all
// or part of MStar Software is expressly prohibited, unless prior written
// permission has been granted by MStar.
//
// By accessing, browsing and/or using MStar Software, you acknowledge that you
// have read, understood, and agree, to be bound by below terms ("Terms") and to
// comply with all applicable laws and regulations:
//
// 1. MStar shall retain any and all right, ownership and interest to MStar
//    Software and any modification/derivatives thereof.
//    No right, ownership, or interest to MStar Software and any
//    modification/derivatives thereof is transferred to you under Terms.
//
// 2. You understand that MStar Software might include, incorporate or be
//    supplied together with third party`s software and the use of MStar
//    Software may require additional licenses from third parties.
//    Therefore, you hereby agree it is your sole responsibility to separately
//    obtain any and all third party right and license necessary for your use of
//    such third party`s software.
//
// 3. MStar Software and any modification/derivatives thereof shall be deemed as
//    MStar`s confidential information and you agree to keep MStar`s
//    confidential information in strictest confidence and not disclose to any
//    third party.
//
// 4. MStar Software is provided on an "AS IS" basis without warranties of any
//    kind. Any warranties are hereby expressly disclaimed by MStar, including
//    without limitation, any warranties of merchantability, non-infringement of
//    intellectual property rights, fitness for a particular purpose, error free
//    and in conformity with any international standard.  You agree to waive any
//    claim against MStar for any loss, damage, cost or expense that you may
//    incur related to your use of MStar Software.
//    In no event shall MStar be liable for any direct, indirect, incidental or
//    consequential damages, including without limitation, lost of profit or
//    revenues, lost or damage of data, and unauthorized system use.
//    You agree that this Section 4 shall still apply without being affected
//    even if MStar Software has been modified by MStar in accordance with your
//    request or instruction for your use, except otherwise agreed by both
//    parties in writing.
//
// 5. If requested, MStar may from time to time provide technical supports or
//    services in relation with MStar Software to you for your use of
//    MStar Software in conjunction with your or your customer`s product
//    ("Services").
//    You understand and agree that, except otherwise agreed by both parties in
//    writing, Services are provided on an "AS IS" basis and the warranty
//    disclaimer set forth in Section 4 above shall apply.
//
// 6. Nothing contained herein shall be construed as by implication, estoppels
//    or otherwise:
//    (a) conferring any license or right to use MStar name, trademark, service
//        mark, symbol or any other identification;
//    (b) obligating MStar or any of its affiliates to furnish any person,
//        including without limitation, you and your customers, any assistance
//        of any kind whatsoever, or any information; or
//    (c) conferring any license or right under any intellectual property right.
//
// 7. These terms shall be governed by and construed in accordance with the laws
//    of Taiwan, R.O.C., excluding its conflict of law rules.
//    Any and all dispute arising out hereof or related hereto shall be finally
//    settled by arbitration referred to the Chinese Arbitration Association,
//    Taipei in accordance with the ROC Arbitration Law and the Arbitration
//    Rules of the Association by three (3) arbitrators appointed in accordance
//    with the said Rules.
//    The place of arbitration shall be in Taipei, Taiwan and the language shall
//    be English.
//    The arbitration award shall be final and binding to both parties.
//
//******************************************************************************
//<MStar Software>
////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2007 MStar Semiconductor, Inc.
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
#if (DEMO_ENV_TEST == 1)
//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "MsCommon.h"
#include "MsMemory.h"
#include "MsOS.h"
#include "drvSERFLASH.h"

#include "demo_env.h"
#include "demo_utility.h"

//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
// Global Definition
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
// Local Definition
//-------------------------------------------------------------------------------------------------
#define ENV_DBG           0
#define INVALID_MUTEX_ID -1

#if ENV_DBG
#define db_print(fmt, args...)  printf("[%s][%d]" fmt, __FUNCTION__, __LINE__, ## args)
#else
#define db_print(fmt, args...)  while(0);
#endif

//-------------------------------------------------------------------------------------------------
// Global Variables
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
// Local Variables
//-------------------------------------------------------------------------------------------------
ST_SPI_EnvInfo* stEnvInfo = NULL;
MS_U32 u32EnvStart[2];
MS_S32 s32FlashMutexId = INVALID_MUTEX_ID;
MS_S32 gs32MstarNonCachedPoolID;

//-------------------------------------------------------------------------------------------------
// Global function
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
// Local function
//-------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------
///// @brief _Demo_ENV_Crc32 : Check ENV use CRC32
///// @param[in] : none
///// @param[in] : buf data
///// @param[in] : len
///// @return CRC32 Value
///// @sa
///// @note : NA .
////-----------------------------------------------------------------------------
static MS_U32 _Demo_ENV_Crc32 (MS_U32 u32Crc, const MS_U8 *pu8Buf, MS_U32 u32Len)
{
#define DO1(pu8Buf)  u32Crc = crc_table[((int)u32Crc ^ (*pu8Buf)) & 0xff] ^ (u32Crc >> 8);pu8Buf++;
#define DO2(pu8Buf)  DO1(pu8Buf); DO1(pu8Buf);
#define DO4(pu8Buf)  DO2(pu8Buf); DO2(pu8Buf);
#define DO8(pu8Buf)  DO4(pu8Buf); DO4(pu8Buf);

    const static MS_U32 crc_table[] =
    {
        0x00000000L, 0x77073096L, 0xee0e612cL, 0x990951baL, 0x076dc419L,
        0x706af48fL, 0xe963a535L, 0x9e6495a3L, 0x0edb8832L, 0x79dcb8a4L,
        0xe0d5e91eL, 0x97d2d988L, 0x09b64c2bL, 0x7eb17cbdL, 0xe7b82d07L,
        0x90bf1d91L, 0x1db71064L, 0x6ab020f2L, 0xf3b97148L, 0x84be41deL,
        0x1adad47dL, 0x6ddde4ebL, 0xf4d4b551L, 0x83d385c7L, 0x136c9856L,
        0x646ba8c0L, 0xfd62f97aL, 0x8a65c9ecL, 0x14015c4fL, 0x63066cd9L,
        0xfa0f3d63L, 0x8d080df5L, 0x3b6e20c8L, 0x4c69105eL, 0xd56041e4L,
        0xa2677172L, 0x3c03e4d1L, 0x4b04d447L, 0xd20d85fdL, 0xa50ab56bL,
        0x35b5a8faL, 0x42b2986cL, 0xdbbbc9d6L, 0xacbcf940L, 0x32d86ce3L,
        0x45df5c75L, 0xdcd60dcfL, 0xabd13d59L, 0x26d930acL, 0x51de003aL,
        0xc8d75180L, 0xbfd06116L, 0x21b4f4b5L, 0x56b3c423L, 0xcfba9599L,
        0xb8bda50fL, 0x2802b89eL, 0x5f058808L, 0xc60cd9b2L, 0xb10be924L,
        0x2f6f7c87L, 0x58684c11L, 0xc1611dabL, 0xb6662d3dL, 0x76dc4190L,
        0x01db7106L, 0x98d220bcL, 0xefd5102aL, 0x71b18589L, 0x06b6b51fL,
        0x9fbfe4a5L, 0xe8b8d433L, 0x7807c9a2L, 0x0f00f934L, 0x9609a88eL,
        0xe10e9818L, 0x7f6a0dbbL, 0x086d3d2dL, 0x91646c97L, 0xe6635c01L,
        0x6b6b51f4L, 0x1c6c6162L, 0x856530d8L, 0xf262004eL, 0x6c0695edL,
        0x1b01a57bL, 0x8208f4c1L, 0xf50fc457L, 0x65b0d9c6L, 0x12b7e950L,
        0x8bbeb8eaL, 0xfcb9887cL, 0x62dd1ddfL, 0x15da2d49L, 0x8cd37cf3L,
        0xfbd44c65L, 0x4db26158L, 0x3ab551ceL, 0xa3bc0074L, 0xd4bb30e2L,
        0x4adfa541L, 0x3dd895d7L, 0xa4d1c46dL, 0xd3d6f4fbL, 0x4369e96aL,
        0x346ed9fcL, 0xad678846L, 0xda60b8d0L, 0x44042d73L, 0x33031de5L,
        0xaa0a4c5fL, 0xdd0d7cc9L, 0x5005713cL, 0x270241aaL, 0xbe0b1010L,
        0xc90c2086L, 0x5768b525L, 0x206f85b3L, 0xb966d409L, 0xce61e49fL,
        0x5edef90eL, 0x29d9c998L, 0xb0d09822L, 0xc7d7a8b4L, 0x59b33d17L,
        0x2eb40d81L, 0xb7bd5c3bL, 0xc0ba6cadL, 0xedb88320L, 0x9abfb3b6L,
        0x03b6e20cL, 0x74b1d29aL, 0xead54739L, 0x9dd277afL, 0x04db2615L,
        0x73dc1683L, 0xe3630b12L, 0x94643b84L, 0x0d6d6a3eL, 0x7a6a5aa8L,
        0xe40ecf0bL, 0x9309ff9dL, 0x0a00ae27L, 0x7d079eb1L, 0xf00f9344L,
        0x8708a3d2L, 0x1e01f268L, 0x6906c2feL, 0xf762575dL, 0x806567cbL,
        0x196c3671L, 0x6e6b06e7L, 0xfed41b76L, 0x89d32be0L, 0x10da7a5aL,
        0x67dd4accL, 0xf9b9df6fL, 0x8ebeeff9L, 0x17b7be43L, 0x60b08ed5L,
        0xd6d6a3e8L, 0xa1d1937eL, 0x38d8c2c4L, 0x4fdff252L, 0xd1bb67f1L,
        0xa6bc5767L, 0x3fb506ddL, 0x48b2364bL, 0xd80d2bdaL, 0xaf0a1b4cL,
        0x36034af6L, 0x41047a60L, 0xdf60efc3L, 0xa867df55L, 0x316e8eefL,
        0x4669be79L, 0xcb61b38cL, 0xbc66831aL, 0x256fd2a0L, 0x5268e236L,
        0xcc0c7795L, 0xbb0b4703L, 0x220216b9L, 0x5505262fL, 0xc5ba3bbeL,
        0xb2bd0b28L, 0x2bb45a92L, 0x5cb36a04L, 0xc2d7ffa7L, 0xb5d0cf31L,
        0x2cd99e8bL, 0x5bdeae1dL, 0x9b64c2b0L, 0xec63f226L, 0x756aa39cL,
        0x026d930aL, 0x9c0906a9L, 0xeb0e363fL, 0x72076785L, 0x05005713L,
        0x95bf4a82L, 0xe2b87a14L, 0x7bb12baeL, 0x0cb61b38L, 0x92d28e9bL,
        0xe5d5be0dL, 0x7cdcefb7L, 0x0bdbdf21L, 0x86d3d2d4L, 0xf1d4e242L,
        0x68ddb3f8L, 0x1fda836eL, 0x81be16cdL, 0xf6b9265bL, 0x6fb077e1L,
        0x18b74777L, 0x88085ae6L, 0xff0f6a70L, 0x66063bcaL, 0x11010b5cL,
        0x8f659effL, 0xf862ae69L, 0x616bffd3L, 0x166ccf45L, 0xa00ae278L,
        0xd70dd2eeL, 0x4e048354L, 0x3903b3c2L, 0xa7672661L, 0xd06016f7L,
        0x4969474dL, 0x3e6e77dbL, 0xaed16a4aL, 0xd9d65adcL, 0x40df0b66L,
        0x37d83bf0L, 0xa9bcae53L, 0xdebb9ec5L, 0x47b2cf7fL, 0x30b5ffe9L,
        0xbdbdf21cL, 0xcabac28aL, 0x53b39330L, 0x24b4a3a6L, 0xbad03605L,
        0xcdd70693L, 0x54de5729L, 0x23d967bfL, 0xb3667a2eL, 0xc4614ab8L,
        0x5d681b02L, 0x2a6f2b94L, 0xb40bbe37L, 0xc30c8ea1L, 0x5a05df1bL,
        0x2d02ef8dL
    };

    u32Crc = u32Crc ^ 0xffffffffL;
    while(u32Len >= 8)
    {
        DO8(pu8Buf);
        u32Len -= 8;
    }

    if(u32Len)
    {
        do
        {
            DO1(pu8Buf);
        }while(--u32Len);
    }

    db_print("CRC check Value = %08lx \n",u32Crc ^ 0xffffffffL);
    return u32Crc ^ 0xffffffffL;
}

//------------------------------------------------------------------------------
///// @brief _Demo_ENV_Dump : dump env data
///// @param[in] : struct to store env
///// @return NA
///// @sa
///// @note : NA .
////-----------------------------------------------------------------------------
static void _Demo_ENV_Dump(ST_SPI_EnvInfo *pstr)
{

    MS_U32 i ;

    db_print("\n============== Dump Data ==============\n");

    for( i =  0 ; i < 0x40 ; i ++ )
    {
        if( 0 == (i % 16) )
            db_print("\n");

        printf("%02x ",pstr->u8data[i]);

    }

    db_print("\n============== Dump End ==============\n");



}

//------------------------------------------------------------------------------
///// @brief _Demo_ENV_WriteFlash : write env data to SPI
///// @param[in] : addr
///// @param[in] : env data to store
///// @param[in] : size
///// @return TRUE  : Success .
///// @return FALSE : Fail .
///// @sa
///// @note : NA .
////-----------------------------------------------------------------------------
static MS_BOOL _Demo_ENV_WriteFlash(MS_U32 u32Addr, MS_U8* pu8Src, MS_U32 u32Size)
{
    MS_BOOL ret = FALSE;

    db_print("Write Flash Addr = 0x%08lx , Size = 0x%08lx \n",u32Addr,u32Size);
    MDrv_SERFLASH_WriteProtect(0);

    if (MDrv_SERFLASH_AddressErase(u32Addr, u32Size, TRUE))
    {
        if (MDrv_SERFLASH_Write(u32Addr, u32Size, pu8Src))
        {
            ret = TRUE;
        }
    }
    MDrv_SERFLASH_WriteProtect(1);
    return ret;
}

//------------------------------------------------------------------------------
///// @brief _Demo_ENV_GetStr : get env data from SPI
///// @param[in] : u32Index
///// @param[in] : env data buffer
///// @param[in] : buffer array size
///// @param[in][out] : env size
///// @return TRUE  : Success .
///// @return FALSE : Fail .
///// @sa
///// @note : NA .
////-----------------------------------------------------------------------------

MS_BOOL _Demo_ENV_GetStr(MS_U32 u32Index, MS_U8* pu8Buf, MS_U32 u32Bufsize,MS_U32* pu32size)
{
    MS_U32 index  = u32Index;
    MS_U32 u32Size = 0;

    pu8Buf[0] = '\0';

    while ( '\0' !=  stEnvInfo->u8data[index])
    {
        if((u32Bufsize == u32Size) && (u32Size != 0))
        {
            printf("[%s][%d]=============YOU HAVE SOME SIZE WRONG, PLEASE CHECK IT =============\n", __FUNCTION__, __LINE__);
            return FALSE ;
        }
        pu8Buf[u32Size] = stEnvInfo->u8data[index];
        u32Size ++;
        index ++;
    }
    pu8Buf[u32Size] = '\0';

    *pu32size  = u32Size ;

    if (0 == u32Size)
    {
        return FALSE ;
    }
    return TRUE ;

}

//------------------------------------------------------------------------------
///// @brief _Demo_ENV_Name : get env data ID Size from SPI
///// @param[in] : env data buffer
///// @param[in] : env size
///// @return TRUE  : size of IDNAME
///// @return FALSE : Fail .
///// @sa
///// @note : NA .
////-----------------------------------------------------------------------------
MS_U32 _Demo_ENV_Name(MS_U8* pu8Buf , MS_U32 u32size )
{
    MS_U32 i = 0 ;

    for(i = 0 ;  i < u32size ; i ++)
    {
        if( '=' == pu8Buf[i])
            return i ;
    }
    return 0;
}

//------------------------------------------------------------------------------
///// @brief _Demo_ENV_SetEnvVal : get env data value from SPI
///// @param[in] : env data struct
///// @param[in] : env ID or name
///// @param[in] : env value
///// @return TRUE  : Successs
///// @return FALSE : Fail .
///// @sa
///// @note : NA .
////-----------------------------------------------------------------------------

MS_BOOL _Demo_ENV_SetEnvVal(ST_SPI_EnvInfo *pstEnvInfo  ,MS_U8* pu8Name , MS_U8* pu8Value)
{
    MS_U32 i = 0 , Cnt = 0;

    while(1)
    {
        if ((i + 1) >= ENV_DATA_SIZE)
        {
            return FALSE;
        }
        if ((0 == pstEnvInfo->u8data[i] ) && ( 0 == pstEnvInfo->u8data[i+1])) // find the tail .
        {
            i = i + 1 ;
            while ( 0 != pu8Name[Cnt] ) // copy name section ;
            {
                pstEnvInfo->u8data[i]  = pu8Name[Cnt] ;
                i ++;
                Cnt ++;
                if (i >= ENV_DATA_SIZE)
                {
                    printf("[%s][%d]Environment size OverfLow \n", __FUNCTION__, __LINE__);
                    return FALSE ;
                }
            }

            pstEnvInfo->u8data[i] = '=' ;
            i ++;
            Cnt = 0 ;

            while ( 0 != pu8Value[Cnt] ) // copy value section
            {
                if(i<(ENV_SIZE - sizeof(MS_U32)))
                {
                    pstEnvInfo->u8data[i]  = pu8Value[Cnt] ;
                }
                else
                {
                    return FALSE ;
                }
                i ++;
                Cnt ++;
                if (i >= ENV_DATA_SIZE)
                {
                    printf("[%s][%d]Environment size OverfLow \n", __FUNCTION__, __LINE__);
                    return FALSE ;
                }
            }

            if ((i + 1) >= ENV_DATA_SIZE )
            {
                printf("[%s][%d]Environment size OverfLow \n", __FUNCTION__, __LINE__);
                return FALSE ;
            }
            // add tail flag
            pstEnvInfo->u8data[i] = 0 ;
            i ++;
            pstEnvInfo->u8data[i] = 0 ;
            i ++;
            return TRUE ;
        }
        i ++ ;
    }
    return FALSE;

}

//------------------------------------------------------------------------------
///// @brief Demo_ENV_LoadEnv : Load env data from SPI flash
///// @param[in] : none
///// @return TRUE  : Success .
///// @return FALSE : Fail .
///// @sa
///// @note : Load env from SPI first .
////-----------------------------------------------------------------------------
MS_BOOL Demo_ENV_LoadEnv(void)
{
    const SERFLASH_Info* stEnvFlash_Info = NULL ;

    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE,&gs32MstarNonCachedPoolID);

    if (NULL != stEnvInfo )
    {
        printf("[%s][%d] ERROR loading Env. stEnvInfo = %08x \n", __FUNCTION__, __LINE__,stEnvInfo->u8data[0]);
        return FALSE ;
    }

    MDrv_SERFLASH_Init();

    stEnvFlash_Info = MDrv_SERFLASH_GetInfo();

    stEnvInfo = MsOS_AllocateMemory( sizeof(ST_SPI_EnvInfo) , gs32MstarNonCachedPoolID );

    db_print("Memory Allocation for Env structure : Addr = %08x , Size = %08x \n", stEnvInfo->u8data[0],sizeof(ST_SPI_EnvInfo));

    u32EnvStart[0] = stEnvFlash_Info->u32TotalSize  -  ( ENV_SIZE ) ;
    u32EnvStart[1] = u32EnvStart[0]  -  ( ENV_SIZE ) ;  // mirror image for backup

    s32FlashMutexId = MsOS_CreateMutex(E_MSOS_FIFO, "Flash_Mutex", MSOS_PROCESS_SHARED);

    if (s32FlashMutexId == INVALID_MUTEX_ID)
    {
        printf("[%s][%d] Mutex Create Failed\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    else
    {
        printf("[%s][%d] Create Mutex OK\n", __FUNCTION__, __LINE__);
    }

    MsOS_ObtainMutex(s32FlashMutexId, MSOS_WAIT_FOREVER);

    if((!MDrv_SERFLASH_Read(u32EnvStart[0],ENV_SIZE,(MS_U8*)(stEnvInfo)))||(stEnvInfo->u32crc!=_Demo_ENV_Crc32(0,stEnvInfo->u8data,ENV_DATA_SIZE)))
    {
        //If fail to read first block
        printf("[%s][%d] [ERROR] >>> Read Env First Block Error , Read mirror block now ... \n", __FUNCTION__, __LINE__);

        _Demo_ENV_Dump(stEnvInfo);

        if((!MDrv_SERFLASH_Read( u32EnvStart[1],ENV_SIZE,(MS_U8*)stEnvInfo))||(stEnvInfo->u32crc != _Demo_ENV_Crc32(0, stEnvInfo->u8data, ENV_DATA_SIZE)) )
        {
            // fail to read second block
            printf("[%s][%d] [ERROR] >>> Read mirror block Fail !!! \n", __FUNCTION__, __LINE__);
            MsOS_FreeMemory(stEnvInfo , gs32MstarNonCachedPoolID);
            stEnvInfo = NULL ;
            MsOS_ReleaseMutex(s32FlashMutexId);
            return FALSE ;
        }
    }

    MsOS_ReleaseMutex(s32FlashMutexId);

    return TRUE ;

}

//------------------------------------------------------------------------------
///// @brief Demo_ENV_StoreEnv : Store env data into SPI flash
///// @param[in] : none
///// @return TRUE  : Success .
///// @return FALSE : Fail .
///// @sa
///// @note : Store env into SPI first .
////-----------------------------------------------------------------------------
MS_BOOL Demo_ENV_StoreEnv(void)
{
    MS_BOOL ret;

    if (!stEnvInfo)
    {
        printf("[%s][%d]ENV not load yet ! \n", __FUNCTION__, __LINE__);
        return false;
    }

    stEnvInfo->u32crc = _Demo_ENV_Crc32(0, stEnvInfo->u8data, ENV_DATA_SIZE);

    MsOS_ObtainMutex(s32FlashMutexId, MSOS_WAIT_FOREVER);

    ret  = _Demo_ENV_WriteFlash(u32EnvStart[0], (MS_U8*)stEnvInfo, ENV_SIZE);
    ret |= _Demo_ENV_WriteFlash(u32EnvStart[1], (MS_U8*)stEnvInfo, ENV_SIZE);


    MsOS_ReleaseMutex(s32FlashMutexId);

    if (ret == FALSE)
        printf("[%s][%d] ENV write back fail !!! \n", __FUNCTION__, __LINE__);

    db_print("Memory Free for Env structure : Addr = %08x , Size = %08x \n",stEnvInfo->u8data[0],sizeof(ST_SPI_EnvInfo));
    MsOS_FreeMemory(stEnvInfo , gs32MstarNonCachedPoolID);

    stEnvInfo = NULL ;

    return ret ;
}

//------------------------------------------------------------------------------
///// @brief Demo_ENV_SetEnv : Add/Modify a environment variable.
///// @param[in] MS_U8* pu8Name : Ptr to name str. MS_U8* pu8Value : Pat to value str.
///// @return TRUE  : Set env Success.
///// @return FALSE : Set Env fail.
///// @sa
///// @note : Load env from SPI first ,then store to SPI after modified .
////-----------------------------------------------------------------------------
MS_BOOL Demo_ENV_SetEnv(MS_U8* pu8Name , MS_U8* pu8Value)
{
    MS_U32 u32Size = 0 , u32Index = 0 , u32Tmpindex = 0 , u32NameSize;
    MS_U8 u8Data[512],u8Name[256];
    ST_SPI_EnvInfo *pstmpEnvInfo ;
    MS_U32 u32Datasize = 0;

    u32Datasize = sizeof(u8Data);

    db_print("SetEnv : %s = %s \n",pu8Name,pu8Value);

    if(!stEnvInfo)
    {
        printf("[%s][%d]Env Not init!!\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    pstmpEnvInfo = MsOS_AllocateMemory( sizeof(ST_SPI_EnvInfo) , gs32MstarNonCachedPoolID );

    if (!pstmpEnvInfo)
        return FALSE ;

    u32Tmpindex = 0 ;
    pstmpEnvInfo->u32crc = 0 ;
    memset(pstmpEnvInfo->u8data,0,ENV_DATA_SIZE);
    while (_Demo_ENV_GetStr(u32Index , u8Data,u32Datasize, &u32Size) )
    {
        u32NameSize = _Demo_ENV_Name(u8Data, u32Size ) ;
        strncpy((char*)u8Name ,(char*)u8Data, u32NameSize ) ;
        u8Name[u32NameSize] = '\0';

        if (0 != strcmp((const char *)u8Name, (const char *)pu8Name))
        {
            memcpy( &pstmpEnvInfo->u8data[u32Tmpindex], u8Data , u32Size );
            u32Tmpindex += u32Size ;
            pstmpEnvInfo->u8data[u32Tmpindex] = 0x0 ;
            u32Tmpindex+= 1;
        }
        u32Index += u32Size + 1;
    }

    pstmpEnvInfo->u8data[u32Tmpindex] = 0 ;
    if (!_Demo_ENV_SetEnvVal( pstmpEnvInfo , pu8Name , pu8Value ))
    {
        printf("[%s][%d]Set Env Fail !\n", __FUNCTION__, __LINE__);
        MsOS_FreeMemory(pstmpEnvInfo , gs32MstarNonCachedPoolID);
        return FALSE ;
    }

    MsOS_FreeMemory(stEnvInfo , gs32MstarNonCachedPoolID);
    stEnvInfo = pstmpEnvInfo ;

    return TRUE;
}

//------------------------------------------------------------------------------
///// @brief Demo_ENV_GetEnv : Get a environment variable.
///// @param[in] MS_U8* pu8Name : Ptr to name str.
///// @return MS_U8 : Value of Environment pu8Name.
///// @return NULL : Fail .
///// @sa
///// @note : Load env from SPI first .
////-----------------------------------------------------------------------------
MS_U8* Demo_ENV_GetEnv(MS_U8* pu8Name)
{
    MS_U8 u8Data[512],u8Name[256];
    MS_U32 u32Size = 0 , u32Index = 0 , u32NameSize = 0;
    MS_U32 u32Datasize = 0;

    if(!stEnvInfo)
    {
        printf("[%s][%d]Env Not init!!\n", __FUNCTION__, __LINE__);
        return NULL;
    }

    u32Datasize = sizeof(u8Data);

    while(_Demo_ENV_GetStr(u32Index, u8Data, u32Datasize, &u32Size) )
    {
        u32NameSize = _Demo_ENV_Name(u8Data, u32Size ) ;
        strncpy((char*)u8Name ,(char*)u8Data, u32NameSize ) ;
        u8Name[u32NameSize] = '\0';

        if (0 == strcmp((const char *)u8Name, (const char *)pu8Name))
        {
            return &(stEnvInfo->u8data[u32Index + u32NameSize + 1 ]);
        }
        u32Index += u32Size + 1;
    }
    return (NULL);
}

#endif

