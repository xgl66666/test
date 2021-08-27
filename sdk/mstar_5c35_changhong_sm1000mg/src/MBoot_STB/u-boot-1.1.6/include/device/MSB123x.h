////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2008 MStar Semiconductor, Inc.
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

#ifndef _MSB123x_H_
#define _MSB123x_H_
#include <msAPI_Tuner.h>

#define EXTSEL extern

#define     DEMOD_SLAVE_ID          0xd2

/*@ <Definitions> @*/
    // ------------------------------------------------------------
    // Other Define
    // -----------------------------------------------------------



    #define REG_CMD_CTRL                0x809C
    #define REG_DTA_CTRL                0x809D
    #define REG_CMD_ADDR                0x809E
    #define REG_CMD_DATA                0x809F
    #define _REG_START                  REG_CMD_CTRL
    #define _REG_END                    REG_CMD_CTRL
    #define _REG_DRQ                    REG_DTA_CTRL
    #define _REG_FSM                    REG_CMD_CTRL
    #define _REG_ERR                    REG_DTA_CTRL
    #define _BIT_START                  0x02
    #define _BIT_END                    0x01
    #define _BIT_DRQ                    0x01
    #define _BIT_FSM                    0x08
    #define _BIT_ERR                    0x80
    #define MSVIF_0702_ENABLE           1


/*@ </Definitions> @*/

typedef enum
{
	COFDM_FEC_LOCK,
	COFDM_PSYNC_LOCK,
	COFDM_TPS_LOCK,
	COFDM_DCR_LOCK,
	COFDM_AGC_LOCK,
	COFDM_MODE_DET,

} COFDM_LOCK_STATUS;




typedef struct
{
	U8		cmd_code;
	U8		param[64];
} S_CMDPKTREG;



//--------------------------------------------------------------------
EXTSEL BOOLEAN MSB123x_WriteReg(U16 u16Addr, U8 u8Data);
EXTSEL BOOLEAN MSB123x_ReadReg(U16 u16Addr, U8 *pu8Data);
EXTSEL BOOLEAN MSB123x_Reset ( void );
EXTSEL BOOLEAN MSB123x_Active(BOOLEAN bEnable);
EXTSEL BOOLEAN MSB123x_Cmd_Packet_Send(S_CMDPKTREG* pCmdPacket, U8 param_cnt);
EXTSEL BOOLEAN MSB123x_Power_On_Initialization (BOOLEAN bIsDVBT);
EXTSEL void MSB123x_AFC_Routine (DWORD dwFrequency, RF_CHANNEL_BANDWIDTH eBandWidth);
EXTSEL BOOLEAN DTV_GetPlpBitMap(U8* u8PlpBitMap);
EXTSEL BOOLEAN DTV_SetPlpGroupID(U8 u8PlpID, U8 u8GroupID);
EXTSEL BOOLEAN DTV_GetPlpGroupID(U8 u8PlpID, U8* u8GroupID);
EXTSEL BOOLEAN DTV_Set_Demod_TYPE(EN_DEVICE_DEMOD_TYPE enDemodType);
EXTSEL EN_DEVICE_DEMOD_TYPE DTV_Get_Demod_TYPE(void);
//--------------------------------------------------------------------

#undef EXTSEL
#endif

