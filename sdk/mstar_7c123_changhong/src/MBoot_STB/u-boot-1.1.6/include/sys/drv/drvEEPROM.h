////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2009 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (MStar Confidential Information!¡L) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _RAM_H_
#define _RAM_H_

#include <sys/common/MsTypes.h>
#include <common.h>

#ifndef RM_DEVICE_ADR
#define RM_DEVICE_ADR    0xA0
#endif

#ifndef RM_HDCP_ADR
#define RM_HDCP_ADR    0xA4
#endif

#if (ENABLE_OAD || (INPUT_HDMI_VIDEO_COUNT > 0))

#ifdef _RAM_C_
#define _RAMDEC_
#else
#define _RAMDEC_ extern
#endif

#define HIBYTE(value)  ((BYTE)((value) / 0x100))
#define LOBYTE(value)  ((BYTE)(value))

// debug
//#define RAM_DEBUG_EN

#ifdef RAM_DEBUG_EN
#define RAM_PUTSTR(str)            putstr(str)
#define RAM_PRINTF(str,para)    printf(str,para)
#else
#define RAM_PUTSTR(str)            printf(str)           //jaly
#define RAM_PRINTF(str,para)    printf(str,para)    //jaly
#endif

//_RAMDEC_ bit g_bDebugRam;

/////////////////////////////////////
// Definition
/////////////////////////////////////


#if (RM_EEPROM_TYPE <= RM_TYPE_24C02)
#define RM_PAGE_WRITE_LEN    8
#elif (RM_EEPROM_TYPE == RM_TYPE_24C16)
#define RM_PAGE_WRITE_LEN    16
#elif (RM_EEPROM_TYPE == RM_TYPE_24C32 || RM_EEPROM_TYPE == RM_TYPE_24C64)
#define RM_PAGE_WRITE_LEN    32
#elif  ( RM_EEPROM_TYPE == RM_TYPE_24C512 )
#define RM_PAGE_WRITE_LEN    128
#else
#define RM_PAGE_WRITE_LEN    16
#endif

// <24c16 EEPROM>      ------------------------------------
// [Device Address] = | 1 | 0 | 1 | 0 | P2 | P1 | P0 | R/W |
//                     ------------------------------------
#define rmCalDeviceAdr(rmID, wIndex)    (rmID | ((HIBYTE(wIndex) & 0x07)) << 1)

/////////////////////////////////////
// Subroutines
/////////////////////////////////////
_RAMDEC_ U8 msAPI_rmReadByte(U16 wIndex);
_RAMDEC_ void msAPI_rmBurstReadBytes(U16 wIndex, U8 *pBuf, U16 u16BufLen);

_RAMDEC_ void msAPI_rmPageWrite(U16 wIndex, U8 *pBuf, U16 u16BufLen);
_RAMDEC_ void msAPI_rmWriteByte(U16 wIndex, U8 ucVal);
_RAMDEC_ void msAPI_rmBurstWriteBytes(U16 wIndex, U8 *pBuf, U16 ucBufLen);

_RAMDEC_ void msAPI_hdcpkeyBurstReadBytes(U16 wIndex, U8 *pBuf, U16 ucBufLen);
_RAMDEC_ void msAPI_hdcpkeyBurstWriteBytes(U16 wIndex, U8 *pBuf, U16 ucBufLen);
_RAMDEC_ void msAPI_hdcpkeyReadByte(U16 wIndex, U8 *pBuf);
_RAMDEC_ void msAPI_hdcpkeyWriteByte(U16 wIndex, U8 ucVal);

#endif
#endif
