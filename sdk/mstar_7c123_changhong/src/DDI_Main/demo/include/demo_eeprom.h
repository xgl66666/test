/*****************************************************************************
 *  Copyright (c) 2005 EchoStar Technologies Corporation
 *  All Rights Reserved
 *  Confidential Property of EchoStar Technologies Corporation
 *
 *  THE MATERIAL CONTAINED HEREIN MAY ONLY BE USED SUBJECT TO AN EXECUTED
 *  AGREEMENT BETWEEN THE USER AND ECHOSTAR TECHNOLOGIES CORPORATION.
 *  DISCLOSURE OF THIS MATERIAL IS STRICTLY PROHIBITED EXCEPT SUBJECT TO
 *  THE TERMS OF SUCH AN AGREEMENT.
 *****************************************************************************/

#ifndef _DEMO_EEPROM_H_
#define _DEMO_EEPROM_H_

#ifdef __cplusplus
extern "C" {
#endif



//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define INTERFACE extern


//-------------------------------------------------------------------------------------------------
//  Enum Types
//-------------------------------------------------------------------------------------------------

typedef enum
{
    E_ACTIVE_BANK_CONTROL_UPPER,
    E_ACTIVE_BANK_CONTROL_LOWER,
}EN_ACTIVE_BANK_CONTROL;


typedef struct
{
    MS_BOOL bWriteEnable;
    EN_ACTIVE_BANK_CONTROL eActiveBank;
}ST_CONFIG_REG;


typedef enum
{
    E_EDID_BLOCK_0,
    E_EDID_BLOCK_1,
    E_EDID_BLOCK_2,
    E_EDID_BLOCK_3,
    E_EDID_BLOCK_4,
    E_EDID_BLOCK_5,
    E_EDID_BLOCK_6,
    E_EDID_BLOCK_7,
}EN_EDID_BLOCK;

//-------------------------------------------------------------------------------------------------
//  Structures
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------

INTERFACE MS_BOOL Demo_EEPROM_init(void);

INTERFACE MS_BOOL Demo_EEPROM_regread(MS_U8 *u8Val);

INTERFACE MS_BOOL Demo_EEPROM_regwrite(ST_CONFIG_REG stReg);

INTERFACE MS_BOOL Demo_EEPROM_edidblockwrite(EN_EDID_BLOCK eEDID, MS_U8 *u8Byte, MS_U16 u16Len);

INTERFACE MS_BOOL Demo_EEPROM_edidblockread(EN_EDID_BLOCK eEDID, MS_U8 *u8Byte, MS_U16 u16Len);

INTERFACE MS_BOOL Demo_EEPROM_sequenceread(MS_U8 u8Bank, MS_U8 u8Segment, MS_U8 u8Address ,MS_U8 *u8Byte, MS_U16 u16Len);

INTERFACE MS_BOOL Demo_EEPROM_pagewrite(MS_U8 u8Bank, MS_U8 u8Segment, MS_U8 u8Address, MS_U8 *u8Byte, MS_U16 u16Len);

INTERFACE MS_BOOL Demo_EEPROM_writeEdidCase(void);

INTERFACE MS_BOOL Demo_EEPROM_readEdidCase(void);

#ifdef __cplusplus
}
#endif

#endif  // _DEMO_EEPROM_H_
