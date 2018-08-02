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
// (i??i??MStar Confidential Informationi??i??) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

#if (DEMO_HDMI_CTS_CERTIFICATION_TEST == 1)

//-----------------------------------------------------------------------------
//
/// @file   Demo_eeprom.c
/// @author MStar Semiconductor Inc.
/// @brief  Scaler setting
//
// @name Box App Command Usage
// @verbatim
//-----------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------

#include "MsCommon.h"
#include <stdio.h>
#include <string.h>
#include <time.h>


#include "drvHWI2C.h"

#include "demo_eeprom.h"
#include "demo_certification.h"

//------------------------------------------------------------------------------
// Structures
//------------------------------------------------------------------------------




//------------------------------------------------------------------------------
// Debug Macros
//------------------------------------------------------------------------------


#define EEPROM_TRACE_LOG 0

//------------------------------------------------------------------------------
// Macros
//------------------------------------------------------------------------------
#define EEPROM_REGISTER_READ 0x63
#define EEPROM_REGISTER_WRITE 0x62
#define EEPROM_SEGMENT_POINT 0x60
#define EEPROM_ADDRESS_POINT 0xA0
#define EEPROM_DATA_READ 0xA1

#define EDID_NULL_ADDR 0
#define EDID_BLOCK 128


#define TEST_EDID_LEN 256


#if EEPROM_TRACE_LOG
#define EEPROM_TRACE(x) x
#else
#define EEPROM_TRACE(x) while(0);
#endif
//------------------------------------------------------------------------------
// Global Variables
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Static Variable
//------------------------------------------------------------------------------

static MS_BOOL _bInit = FALSE;

static unsigned char EDID_DVI_0[TEST_EDID_LEN] =
{
    0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x36, 0x74, 0x30, 0x00, 0x01, 0x00, 0x00, 0x00,
    0x18, 0x0F, 0x01, 0x03, 0x80, 0x73, 0x41, 0x78, 0x0A, 0xCF, 0x74, 0xA3, 0x57, 0x4C, 0xB0, 0x23,
    0x09, 0x48, 0x4C, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0xFF, 0x01, 0xFF, 0xFF, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x20, 0x01, 0x1D, 0x00, 0x72, 0x51, 0xD0, 0x1E, 0x20, 0x6E, 0x28,
    0x55, 0x00, 0xC4, 0x8E, 0x21, 0x00, 0x00, 0x1E, 0x01, 0x1D, 0x80, 0x18, 0x71, 0x1C, 0x16, 0x20,
    0x58, 0x2C, 0x25, 0x00, 0xC4, 0x8E, 0x21, 0x00, 0x00, 0x9E, 0x00, 0x00, 0x00, 0xFC, 0x00, 0x4D,
    0x53, 0x74, 0x61, 0x72, 0x20, 0x44, 0x65, 0x6D, 0x6F, 0x0A, 0x20, 0x20, 0x00, 0x00, 0x00, 0xFD,
    0x00, 0x3B, 0x3C, 0x1F, 0x2D, 0x08, 0x00, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x01, 0x25,
    0x02, 0x03, 0x25, 0xF2, 0x4B, 0x05, 0x84, 0x03, 0x01, 0x12, 0x93, 0x14, 0x16, 0x07, 0x10, 0x1F,
    0x23, 0x09, 0x07, 0x07, 0x83, 0x01, 0x00, 0x00, 0x6C, 0x03, 0x0C, 0x00, 0x20, 0x00, 0xB8, 0x2D,
    0x20, 0xA0, 0x02, 0x01, 0x40, 0x8C, 0x0A, 0xD0, 0x8A, 0x20, 0xE0, 0x2D, 0x10, 0x10, 0x3E, 0x96,
    0x00, 0xC4, 0x8E, 0x21, 0x00, 0x00, 0x18, 0x8C, 0x0A, 0xA0, 0x14, 0x51, 0xF0, 0x16, 0x00, 0x26,
    0x7C, 0x43, 0x00, 0xC4, 0x8E, 0x21, 0x00, 0x00, 0x98, 0x01, 0x1D, 0x00, 0xBC, 0x52, 0xD0, 0x1E,
    0x20, 0xB8, 0x28, 0x55, 0x40, 0xC4, 0x8E, 0x21, 0x00, 0x00, 0x1E, 0x01, 0x1D, 0x80, 0xD0, 0x72,
    0x1C, 0x16, 0x20, 0x10, 0x2C, 0x25, 0x80, 0xC4, 0x8E, 0x21, 0x00, 0x00, 0x9E, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x35,
};


static MS_U8 _u8Byte[TEST_EDID_LEN]={0};

//------------------------------------------------------------------------------
// Local Functions
//------------------------------------------------------------------------------

MS_U8 _Demo_EEPROM_getbank(EN_EDID_BLOCK eEDID)
{
    switch(eEDID)
    {
        case E_EDID_BLOCK_0:
        case E_EDID_BLOCK_1:
        case E_EDID_BLOCK_2:
        case E_EDID_BLOCK_3:
            return 0;
            break;
        case E_EDID_BLOCK_4:
        case E_EDID_BLOCK_5:
        case E_EDID_BLOCK_6:
        case E_EDID_BLOCK_7:
            return 1;
            break;
        default:
            break;
    }

    return 0xFF;
}


MS_U8 _Demo_EEPROM_getsegment(EN_EDID_BLOCK eEDID)
{
    switch(eEDID)
    {
        case E_EDID_BLOCK_0:
        case E_EDID_BLOCK_1:
            return 0;
            break;
        case E_EDID_BLOCK_2:
        case E_EDID_BLOCK_3:
            return 1;
            break;
        case E_EDID_BLOCK_4:
        case E_EDID_BLOCK_5:
            return 2;
            break;
        case E_EDID_BLOCK_6:
        case E_EDID_BLOCK_7:
            return 3;
            break;
        default:
            break;
    }

    return 0xFF;
}

MS_U16 _Demo_EEPROM_getaddress(EN_EDID_BLOCK eEDID)
{
    switch(eEDID)
    {
        case E_EDID_BLOCK_0:
        case E_EDID_BLOCK_2:
        case E_EDID_BLOCK_4:
        case E_EDID_BLOCK_6:
            return 0;
            break;

        case E_EDID_BLOCK_1:
        case E_EDID_BLOCK_3:
        case E_EDID_BLOCK_5:
        case E_EDID_BLOCK_7:
            return 128;
            break;
        default:
            break;
    }

    return 0xFFFF;


}

MS_BOOL Demo_EEPROM_regread(MS_U8 *u8Val)
{


    MS_BOOL bRet = FALSE;

    MS_U8 u8Byte,u8Addr;

     /* READ CONFIGURATION REGISTER TEST ::: START 0110 0011 ACK DATA NO ACK STOP*/
    bRet = MDrv_HWI2C_ReadBytes(EEPROM_REGISTER_READ, 0, &u8Addr, 1, &u8Byte);
    printf("[%04d]  (u16SlaveAddr=0x%x, u8Addr[0]=0x%x)===== ret=%d\n", __LINE__, EEPROM_REGISTER_READ, u8Addr ,bRet);
    
    if(bRet == FALSE)
    {
        printf("[%04d] MDrv_IIC_WriteBytes Error\n", __LINE__);
        return FALSE;
    }
    
    *u8Val = u8Byte;

    EEPROM_TRACE(printf("\n\n\t                     NB : %d\n",u8Byte&0x01));
    EEPROM_TRACE(printf("\tActive Bank Control bit0: %d\n",(u8Byte>>1)&0x01));
    EEPROM_TRACE(printf("\tActive Bank Control bit1: %d\n",(u8Byte>>2)&0x01));
    EEPROM_TRACE(printf("\tWrite Enable : %s\n",(((u8Byte>>3)&0x01)==0)?"Disable":"Enable"));
    EEPROM_TRACE(printf("\n\n"));


    return TRUE;
}




MS_BOOL Demo_EEPROM_regwrite(ST_CONFIG_REG stReg)
{

    MS_BOOL bRet = FALSE;
    MS_U8 u8Byte,u8Addr;


    if(stReg.eActiveBank == E_ACTIVE_BANK_CONTROL_LOWER)
    {
        u8Byte=0x04;
    }
    else 
    {
        u8Byte=0x06;
    }

    if(stReg.bWriteEnable == TRUE)
    {
        u8Byte=u8Byte|0x08;
    }
    
    EEPROM_TRACE(printf("[%s][%d]  Value:0x%X\n",__FUNCTION__, __LINE__,u8Byte));

    /*START 0110 0010 ACK DUMMY ADDRESS ACK XXXX WE AB1 AB0 NB ACK STOP */

    bRet = MDrv_HWI2C_WriteBytes(EEPROM_REGISTER_WRITE, 1, &u8Addr, 1, &u8Byte);
    
    EEPROM_TRACE(printf("[%s][%d] u16SlaveAddr=0x%x , ret=%d\n",__FUNCTION__, __LINE__, EEPROM_REGISTER_WRITE,bRet));
    if(bRet == FALSE)
    {
        printf("[%s][%d] MDrv_IIC_WriteBytes Error\n",__FUNCTION__, __LINE__);
        return FALSE;
    }

    return TRUE;
}



MS_BOOL Demo_EEPROM_init(void)
{
    MS_BOOL bResult = FALSE;
    HWI2C_UnitCfg hwI2CCfg;


    if(_bInit)
    {
        return TRUE;
    }
    
    memset(&hwI2CCfg, 0x00, sizeof(HWI2C_UnitCfg));


    hwI2CCfg.eSpeed = E_HWI2C_VSLOW;//E_HWI2C_SLOW;
    hwI2CCfg.sI2CPin.u32Reg = 0;
    hwI2CCfg.sI2CPin.u8BitPos = 0;
    hwI2CCfg.sI2CPin.bEnable = 0;
    hwI2CCfg.ePort = E_HWI2C_PORT0_1;/// HDMI RX: E_HWI2C_PORT0_1;
    bResult = MDrv_HWI2C_Init(&hwI2CCfg);
    if(bResult)
    {
        printf("MDrv_IIC_InitPort0: OK\n");
    }
    else
    {
        printf("[%s][%d]MDrv_IIC_InitPort0: FAIL\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    MDrv_HWI2C_SelectPort(hwI2CCfg.ePort);
    MDrv_HWI2C_SetClk(hwI2CCfg.eSpeed);
    MDrv_HWI2C_SetReadMode(E_HWI2C_READ_MODE_DIRECTION_CHANGE);


    return TRUE;

}


MS_BOOL Demo_EEPROM_pagewrite(MS_U8 u8Bank, MS_U8 u8Segment, MS_U8 u8Address, MS_U8 *u8Byte, MS_U16 u16Len)
{
    #if HDMI_CTS_CERTIFICATION_HDMI_CEC
    MS_BOOL bRet = FALSE;
    MS_U16 u16SlaveAddr = 0;

    EEPROM_TRACE(printf("bank :%u, segment:%u u8Address:%u  u16Len:%u\n",u8Bank,u8Segment,u8Address,u16Len));


    
    /*    START 0110 0000 ACK xxxx xxS1S0 Segment ADDRESS ACK */  
    bRet = MDrv_HWI2C_WriteByteWithoutStop(EEPROM_SEGMENT_POINT, EDID_NULL_ADDR,  u8Segment);
    EEPROM_TRACE(printf("[%s][%d] u16SlaveAddr=0x%x, ===== ret=%d\n",__FUNCTION__, __LINE__, u16SlaveAddr,bRet));
    if(bRet == FALSE)
    {
        printf("[%s][%d] MDrv_IIC_WriteBytes Error\n",__FUNCTION__, __LINE__);
        return FALSE;
    }


    /*    START 1010 0000 ACK A7 - A0 ADDRESS ACK DATA0 ACK ......... DATA15 ACK STOP */      
    bRet = MDrv_HWI2C_WriteBytes(EEPROM_ADDRESS_POINT, 1, &u8Address, u16Len, u8Byte);
    EEPROM_TRACE(printf("[%s][%d] (u16SlaveAddr=0x%x,  )===== ret=%d\n",__FUNCTION__, __LINE__, u16SlaveAddr,bRet));
    
    if(bRet == FALSE)
    {
        printf("[%s][%d] MDrv_IIC_WriteBytes Error\n",__FUNCTION__, __LINE__);
        return FALSE;
    }
    #endif


    return TRUE;
}




MS_BOOL Demo_EEPROM_bytewrite(MS_U8 u8Bank, MS_U8 u8Segment, MS_U8 u8Address, MS_U8 u8Data)
{
    #if HDMI_CTS_CERTIFICATION_HDMI_CEC
    MS_BOOL bRet = FALSE;

    printf("bank :%u, segment:%u u8Address:%u  \n",u8Bank,u8Segment,u8Address);


    /* SEGMENT ADDRESS TEST*/


    /*  START 0110 0000 ACK xxxx xxS1S0 Segment ADDRESS ACK */
    bRet = MDrv_HWI2C_WriteByteWithoutStop(EEPROM_SEGMENT_POINT, EDID_NULL_ADDR,  u8Segment);
    EEPROM_TRACE(printf("[%04d]  (u16SlaveAddr=0x%x, ===== ret=%d\n", __LINE__, EEPROM_SEGMENT_POINT, bRet));
    if(bRet == FALSE)
    {
        printf("[%04d] MDrv_IIC_WriteBytes Error\n", __LINE__);
        return FALSE;
    }





    /* START 1010 0000 ACK A7 - A0 ADDRESS ACK ACK DATA STOP*/    
    bRet = MDrv_HWI2C_WriteBytes(EEPROM_ADDRESS_POINT, 1, &u8Address, 1, &u8Data);
    EEPROM_TRACE(printf("[%04d] u16SlaveAddr=0x%x, ===== ret=%d\n", __LINE__, EEPROM_ADDRESS_POINT,bRet));
    if(bRet == FALSE)
    {
        printf("[%04d] MDrv_IIC_WriteBytes Error\n", __LINE__);
        return FALSE;
    }
    EEPROM_TRACE(printf("Data--->\n\t"));
    #endif
    
    return TRUE;
    

}


MS_BOOL Demo_EEPROM_byteread(MS_U8 u8Bank, MS_U8 u8Segment, MS_U8 u8Address)
{

    #if HDMI_CTS_CERTIFICATION_HDMI_CEC
    MS_U32 swiic_res = FALSE;
    MS_U16 u16SlaveAddr = 0;
    //MS_U32 u16Len = 0;
    //MS_U32 u32RegValue;
    MS_U8 u8Byte;


    EEPROM_TRACE(printf("bank :%u, segment:%u u8Address:%u\n",u8Bank,u8Segment,u8Address));


    /*  START 0110 0000 ACK xxxx xxS1S0 Segment ADDRESS ACK */
    swiic_res = MDrv_HWI2C_WriteByteWithoutStop(EEPROM_SEGMENT_POINT, EDID_NULL_ADDR, u8Segment);
    EEPROM_TRACE(printf("[%s][%d]   u16SlaveAddr=0x%x, ===== ret=%ld\n",__FUNCTION__, __LINE__, u16SlaveAddr, swiic_res));
    
    if(swiic_res == FALSE)
    {
        printf("[%s][%d] MDrv_IIC_WriteBytes Error\n",__FUNCTION__, __LINE__);
        return FALSE;
    }




    /*  START 1010 0000 ACK A7 - A0 ADDRESS ACK START 1010 0001 ACK DATA NOACK STOP */    
    swiic_res = MDrv_HWI2C_ReadBytes(EEPROM_ADDRESS_POINT, 1, &u8Address, 1, &u8Byte);
    EEPROM_TRACE(printf("[%s][%d] u16SlaveAddr=0x%x, u8Addr[0]=0x%x)===== ret=%ld\n",__FUNCTION__, __LINE__, u16SlaveAddr, u8Address,swiic_res));
    if(swiic_res == FALSE)
    {
        printf("[%s][%d] MDrv_IIC_WriteBytes Error\n",__FUNCTION__, __LINE__);
        return FALSE;
    }


    EEPROM_TRACE(printf("Data : %02x ", u8Byte));
    #endif


 

    return TRUE;
}



MS_BOOL Demo_EEPROM_sequenceread(MS_U8 u8Bank, MS_U8 u8Segment, MS_U8 u8Address ,MS_U8 *u8Byte, MS_U16 u16Len)
{
    #if HDMI_CTS_CERTIFICATION_HDMI_CEC
    MS_U32 swiic_res = FALSE;






    EEPROM_TRACE(printf("[%s][%d]bank :%u, segment:%u u8Address:%u u16Len:%u\n",__FUNCTION__, __LINE__,u8Bank,u8Segment,u8Address,u16Len));


    /* SEGMENT ADDRESS TEST*/

    /*  START 0110 0000 ACK xxxx xxS1S0 Segment ADDRESS ACK */ 
    swiic_res = MDrv_HWI2C_WriteByteWithoutStop(EEPROM_SEGMENT_POINT, EDID_NULL_ADDR, u8Segment);


    
    EEPROM_TRACE(printf("[%s][%d]  (u16SlaveAddr=0x%x, u8Addr[0]=0x%x)===== ret=%ld\n",__FUNCTION__, __LINE__, EEPROM_SEGMENT_POINT, u8Address,swiic_res));
    if(swiic_res == FALSE)
    {
        printf("[%s][%d] MDrv_IIC_WriteBytes Error\n",__FUNCTION__, __LINE__);
        return FALSE;
    }




    /*  START 1010 0000 ACK A7 - A0 ADDRESS ACK START 1010 0001 ACK DATA0 ACK ......... DATAN NOACK STOP */    

    swiic_res = MDrv_HWI2C_ReadBytes(EEPROM_ADDRESS_POINT, 1, &u8Address, u16Len, u8Byte);
    EEPROM_TRACE(printf("[%s][%d](u16SlaveAddr=0x%x, u8Addr[0]=0x%x)===== ret=%ld\n",__FUNCTION__, __LINE__, EEPROM_ADDRESS_POINT, u8Address,swiic_res));
    if(swiic_res == FALSE)
    {
        printf("[%s][%d] MDrv_IIC_WriteBytes Error\n",__FUNCTION__, __LINE__);
        return FALSE;
    }
    #endif





    return TRUE;

}


MS_BOOL Demo_EEPROM_edidblockwrite(EN_EDID_BLOCK eEDID, MS_U8 *u8Byte, MS_U16 u16Len)
{
    int i = 0;
    MS_U8 u8Bank = _Demo_EEPROM_getbank(eEDID);
    MS_U8 u8Segment = _Demo_EEPROM_getsegment(eEDID);
    MS_U8 u8Address = _Demo_EEPROM_getaddress(eEDID);

    if((u8Bank == 0xFF) || (u8Segment == 0xFF) || (u8Address == 0xFF))
    {    
        printf("[%s][%d] Invalid Param.%02X.%02X.%02X.\n",__FUNCTION__, __LINE__,u8Bank,u8Segment,u8Address);
        return FALSE;
    }

    for( i = 0 ; i < u16Len ; i=i+16)
    {
        
        Demo_EEPROM_pagewrite(u8Bank ,u8Segment, u8Address+i, &u8Byte[i], 16);
    }
    

    return TRUE;
}


MS_BOOL Demo_EEPROM_edidblockread(EN_EDID_BLOCK eEDID, MS_U8 *u8Byte, MS_U16 u16Len)
{
    MS_U8 u8Bank = _Demo_EEPROM_getbank(eEDID);
    MS_U8 u8Segment = _Demo_EEPROM_getsegment(eEDID);
    MS_U8 u8Address = _Demo_EEPROM_getaddress(eEDID);

    if((u8Bank == 0xFF) || (u8Segment == 0xFF) || (u8Address == 0xFF))
    {
        printf("[%s][%d] Invalid Param.%02X.%02X.%02X.\n",__FUNCTION__, __LINE__,u8Bank,u8Segment,u8Address);
        return FALSE;
    }

    return Demo_EEPROM_sequenceread( u8Bank,  u8Segment,  u8Address ,u8Byte, u16Len);


}




MS_BOOL Demo_EEPROM_writeEdidCase(void)
{

    if(Demo_EEPROM_edidblockwrite(E_EDID_BLOCK_0, &EDID_DVI_0[0], 128)==FALSE)
    {
        printf("[%s][%d] Fail to set EDID\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    if(Demo_EEPROM_edidblockwrite(E_EDID_BLOCK_1, &EDID_DVI_0[128], 128)==FALSE)
    {
        printf("[%s][%d] Fail to set EDID\n",__FUNCTION__,__LINE__);
        return FALSE;
    }


    return TRUE;
}




MS_BOOL Demo_EEPROM_readEdidCase(void)
{

    int i = 0;
    Demo_EEPROM_edidblockread(E_EDID_BLOCK_0, &_u8Byte[0], 128);
    Demo_EEPROM_edidblockread(E_EDID_BLOCK_1, &_u8Byte[128], 128);


    for(i = 0;i<TEST_EDID_LEN; i++)
    {
        printf(" %02X ",_u8Byte[i]);

        if((i%16)==15)
            printf("\n");

    }

    return TRUE;
    
}

#endif

