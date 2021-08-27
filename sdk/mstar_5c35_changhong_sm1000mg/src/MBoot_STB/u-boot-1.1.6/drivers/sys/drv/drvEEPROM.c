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
// Description: EEPROM access subroutines.
//
////////////////////////////////////////////////////////////////////////////////

#include <common.h>
#include <config.h>
#include <sys/common/MsTypes.h>
#include <sys/common/MsIRQ.h>
#include <sys/common/MsOS.h>
#error "This platform is Not supported!!!"
#include <sys/drv/drvEEPROM.h>

static MS_U8 g_RmEeprom_Adr = RM_DEVICE_ADR;

///////////////////////////////////////////////////////////////////////////////
// <Function>: rmReadByte
//
// <Description>: Read byte from EEPROM.
//
// <Returns> : Read byte.
//
// <Parameter>:  -  <Flow>  -  <Description>
//   wIndex      -    In    -   index
//-----------------------------------------------------------------------------
//
///////////////////////////////////////////////////////////////////////////////
/******************************************************************************/
/// API for Reading byte from RAM
/// @param wIndex \b IN: Index
/// @return U8: RAM data
/******************************************************************************/
U8 msAPI_rmReadByte(U16 wIndex)
{
    U8 ucBuf; // byte buffer

    msAPI_rmBurstReadBytes(wIndex, &ucBuf, 1);

    return ucBuf;
}

///////////////////////////////////////////////////////////////////////////////
// <Function>: rmBurstReadBytes
//
// <Description>: EEPROM burst read bytes
//
// <Parameter>:  -  <Flow>  -  <Description>
//   wIndex      -    In    -   index
//   pBuf        -    Out   -   buffer pointer
//   ucBufLen    -    In    -   buffer length
//-----------------------------------------------------------------------------
//
///////////////////////////////////////////////////////////////////////////////
/******************************************************************************/
/// API for Burst Reading bytes from RAM
/// @param wIndex \b IN: Index
/// @param *pBuf \b OUT: pointer to buffer for RAM data read
/// @param ucBufLen \b IN: buffer length
/******************************************************************************/
void msAPI_rmBurstReadBytes(U16 wIndex, U8 *pBuf, U16 u16BufLen)
{
    U8 pu8addr[2];
    U8 u8ID;
    U8 u8AddrLen;

    if (RM_EEPROM_TYPE <= RM_TYPE_24C02)
    {
        u8ID = g_RmEeprom_Adr;

        u8AddrLen = 1;
        pu8addr[0] = LOBYTE(wIndex);
    }
    else if( RM_EEPROM_TYPE >= RM_TYPE_24C32)
    {
        u8ID = g_RmEeprom_Adr;

        u8AddrLen = 2;
        pu8addr[0] = HIBYTE(wIndex);
        pu8addr[1] = LOBYTE(wIndex);
    }
    else
    {
        u8ID = rmCalDeviceAdr(g_RmEeprom_Adr, wIndex);

        u8AddrLen = 1;
        pu8addr[0] = LOBYTE(wIndex);
    }

    #if 0
    MDrv_MST_IIC_SetSpeed(4);
    MDrv_MST_IIC_ReadBytes(u8ID, u8AddrLen, pu8addr, (U16)ucBufLen, pBuf);
    MDrv_MST_IIC_SetSpeed(4);
    #else
    if(FALSE == MDrv_IIC_ReadBytes(u8ID, u8AddrLen, pu8addr, u16BufLen, pBuf))
    {
        printf("MDrv_IIC_ReadBytes fail---%x\n",u8ID);
    }
    #endif
}

///////////////////////////////////////////////////////////////////////////////
// <Function>: rmPageWrite
//
// <Description>: EEPROM page write

// <Parameter>:  -  <Flow>  -  <Description>
//   wIndex      -    In    -   index
//   pBuf        -    Out   -   buffer pointer
//   ucBufLen    -    In    -   buffer length
//-----------------------------------------------------------------------------
//
///////////////////////////////////////////////////////////////////////////////
/******************************************************************************/
/// API for page write to RAM
/// @param wIndex \b IN: Index
/// @param *pBuf \b OUT: pointer to buffer for write to RAM
/// @param ucBufLen \b IN: buffer length
/******************************************************************************/
void msAPI_rmPageWrite(U16 wIndex, U8 *pBuf, U16 u16BufLen)
{
    U8 pu8addr[2];
    U8 u8ID;
    U8 u8AddrLen;

    #if( EEPROM512_WP_ENABLE    ==1 )
            EEPROM512_WP_OFF();
    #endif

    if (RM_EEPROM_TYPE <= RM_TYPE_24C02)
    {
        u8ID = g_RmEeprom_Adr;

        u8AddrLen = 1;
        pu8addr[0] = LOBYTE(wIndex);
    }
    else if(RM_EEPROM_TYPE >= RM_TYPE_24C32)
    {
        u8ID = g_RmEeprom_Adr;

        u8AddrLen = 2;
        pu8addr[0] = HIBYTE(wIndex);
        pu8addr[1] = LOBYTE(wIndex);
    }
    else
    {
        u8ID = rmCalDeviceAdr(g_RmEeprom_Adr, wIndex);

        u8AddrLen = 1;
        pu8addr[0] = LOBYTE(wIndex);
    }

    #if 0
    MDrv_MST_IIC_SetSpeed(4);
    MDrv_MST_EepromWpOnOff(DISABLE);
    MDrv_MST_IIC_WriteBytes(u8ID, u8AddrLen, pu8addr, (U16)ucBufLen, pBuf);
    MDrv_MST_EepromWpOnOff(ENABLE);
    MDrv_MST_IIC_SetSpeed(4);
    #else
    if (FALSE == MDrv_IIC_WriteBytes(u8ID, u8AddrLen, pu8addr, u16BufLen, pBuf))
    {
        printf("MDrv_IIC_WriteBytes fail---%x\n", u8ID);
    }
    #endif

    //MsOS_DelayTask(12);
    #if ( EEPROM512_WP_ENABLE  == 1 )
        EEPROM512_WP_ON();
    #endif
}

///////////////////////////////////////////////////////////////////////////////
// <Function>: rmWriteByte
//
// <Description>: Write byte to EEPROM.
//
// <Returns> : None
//
// <Parameter>:  -  <Flow>  -  <Description>
//   wIndex      -    In    -   index
//   ucVal       -    In    -   data
//-----------------------------------------------------------------------------
//
///////////////////////////////////////////////////////////////////////////////
/******************************************************************************/
/// API for writing byte data to RAM
/// @param wIndex \b IN: Index
/// @param ucVal \b IN: value to write
/******************************************************************************/
void msAPI_rmWriteByte(U16 wIndex, U8 ucVal)
{
    msAPI_rmBurstWriteBytes(wIndex, &ucVal, 1);
}

///////////////////////////////////////////////////////////////////////////////
// <Function>: rmBurstWriteBytes
//
// <Description>: EEPROM burst write bytes
//
// <Parameter>:  -  <Flow>  -  <Description>
//   wIndex      -    In    -   index
//   pBuf        -    Out   -   buffer pointer
//   ucBufLen    -    In    -   buffer length
//-----------------------------------------------------------------------------
//
///////////////////////////////////////////////////////////////////////////////
/******************************************************************************/
/// API for Burst write bytes from RAM
/// @param wIndex \b IN: Index
/// @param *pBuf \b OUT: pointer to buffer for data to write to RAM
/// @param ucBufLen \b IN: buffer length
/******************************************************************************/
#define ENABLE_EEPROM_DATA_VERIFY FALSE
void msAPI_rmBurstWriteBytes(U16 wIndex, U8 *pBuf, U16 ucBufLen)
{
    U8 ucWriteCount;  // write counter
    U8 ucRemainLen;   // i2c word address
#if ENABLE_EEPROM_DATA_VERIFY
    U16 wIndexTmp=wIndex;
    U16 ucBufLenTmp=ucBufLen;
    U8 tmp[10];
    U8 *tmp2=NULL;
    U8 *pBufTmp=pBuf;
    if(ucBufLen >10)
    {
        tmp2=(U8*)MsOS_AllocateMemory(ucBufLen,0);
    }
#endif

    while ( 1 )
    {
        // check page write roll over
        if (ucBufLen > RM_PAGE_WRITE_LEN)
        {
            ucWriteCount = RM_PAGE_WRITE_LEN;
        }
        else
        {
            ucWriteCount = (U8)ucBufLen;
        }

        // check word address higher than page write bits
        ucRemainLen = LOBYTE(wIndex) % RM_PAGE_WRITE_LEN;

        if (ucRemainLen) // check ramain
        {
            ucRemainLen = RM_PAGE_WRITE_LEN - ucRemainLen;

            if (ucWriteCount > ucRemainLen) // check not roll over
            {
                ucWriteCount = ucRemainLen;
            }
        }

        // EEPROM page write
        msAPI_rmPageWrite(wIndex, pBuf, ucWriteCount);

        // check end of buffer
        ucBufLen -= ucWriteCount;

        if ( ucBufLen == 0 )
        {
            break;
        }

        wIndex += ucWriteCount; // next word address
        pBuf += ucWriteCount; // next buffer pointer
    }

#if ENABLE_EEPROM_DATA_VERIFY
    if(ucBufLenTmp <= 10)
    {
        msAPI_rmBurstReadBytes(wIndexTmp,tmp,ucBufLenTmp);
        if(memcmp(tmp,pBufTmp,ucBufLenTmp))
        {
            printf("\n\nWarning!   msAPI_rmBurstWriteBytes error address %x size %d\n\n",wIndexTmp,ucBufLenTmp);
        }
    }
    else if(tmp2)
    {
        msAPI_rmBurstReadBytes(wIndexTmp,tmp2,ucBufLenTmp);
        if(memcmp(tmp2,pBufTmp,ucBufLenTmp))
        {
            printf("\n\nWarning!   msAPI_rmBurstWriteBytes error address %x size %d\n\n",wIndexTmp,ucBufLenTmp);
        }
        MsOS_FreeMemory(tmp2,0);
    }
#endif

}

#if (INPUT_HDMI_VIDEO_COUNT > 0)
///////////////////////////////////////////////////////////////////////////////
// <Function>: hdcpkeyBurstReadBytes
//
// <Description>: Hdcpkey EEPROM burst read bytes
//
// <Parameter>:  -  <Flow>  -  <Description>
//   wIndex      -    In    -   index
//   pBuf        -    Out   -   buffer pointer
//   ucBufLen    -    In    -   buffer length
//-----------------------------------------------------------------------------
//
///////////////////////////////////////////////////////////////////////////////
void msAPI_hdcpkeyBurstReadBytes(U16 wIndex, U8 *pBuf, U16 ucBufLen)
{
#if (HDCP_KEY_TYPE==HDCP_KEY_IN_24C04)
    U8 pu8addr_L,pu8addr_H;
    pu8addr_L = LOBYTE(wIndex);
    pu8addr_H = HIBYTE(wIndex);
    if(pu8addr_H)
        MDrv_IIC_ReadBytes(RM_HDCP_ADR+2, 1, &pu8addr_L, ucBufLen, pBuf);
    else
        MDrv_IIC_ReadBytes(RM_HDCP_ADR,     1, &pu8addr_L, ucBufLen, pBuf);
#elif (HDCP_KEY_TYPE==HDCP_KEY_IN_24C64)
    msAPI_rmBurstReadBytes(wIndex, pBuf, ucBufLen);
#else
    wIndex = wIndex;
    pBuf = pBuf;
    ucBufLen = ucBufLen;
#endif
}

void msAPI_hdcpkeyBurstWriteBytes(U16 wIndex, U8 *pBuf, U16 ucBufLen)
{
#if (HDCP_KEY_TYPE==HDCP_KEY_IN_24C04)
    U8 pu8addr_L,pu8addr_H;
    BOOLEAN bResult;
    pu8addr_L = LOBYTE(wIndex);
    pu8addr_H = HIBYTE(wIndex);

    if(pu8addr_H)
    {
        bResult = MDrv_IIC_WriteBytes(RM_HDCP_ADR+2, 1, &pu8addr_L,ucBufLen, pBuf);
    }
    else
    {
        bResult = MDrv_IIC_WriteBytes(RM_HDCP_ADR, 1, &pu8addr_L,ucBufLen, pBuf);
    }

    if (FALSE == bResult)
    {
        printf("read write fail\n");
    }
#elif (HDCP_KEY_TYPE==HDCP_KEY_IN_24C64)
    msAPI_rmBurstWriteBytes(wIndex, pBuf, ucBufLen);
#else
    wIndex = wIndex;
    pBuf = pBuf;
    ucBufLen = ucBufLen;
#endif
}

void msAPI_hdcpkeyReadByte(U16 wIndex, U8 *pBuf)
{
    msAPI_hdcpkeyBurstReadBytes(wIndex, pBuf, 1);
}

void msAPI_hdcpkeyWriteByte(U16 wIndex, U8 ucVal)
{
    U8 ucValue = ucVal;
    msAPI_hdcpkeyBurstWriteBytes(wIndex, &ucValue, 1);
}

#endif

#undef _RAM_C_
