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
// Copyright (c) 2008-2009 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// ("MStar Confidential Information") by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    apiSC_T1.c
/// @brief  High level SmartCard Driver Interface for T=1
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include "MsCommon.h"

#include "apiSC.h"
#include "drvSC.h"

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
//
#define SC_DEV_NUM_MAX                  2
#define SC_T1_SEND_TIMEOUT             70// 1.5 //50
#define SC_T1_RECV_TIMEOUT             70// 1.5 //80
#define SC_T1_DEFAULT_IFSC             32

#define SC_API_T1_DBG_ENABLE            1 
//#define SC_ATR_LEN_MAX                 32
//#define SC_ATR_LEN_MIN                  2
#if SC_API_T1_DBG_ENABLE
#define SC_API_DBG(fmt, args...)        printf(fmt, ##args)
#else
#define SC_API_DBG(fmt, args...)        {}
#endif

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------
typedef struct
{
    MS_U8                           u8NAD;
    MS_U8                           u8NS;
    MS_U8                           u8NR;
    MS_BOOL                         bMore;
    MS_U8                           u8IFSC;
    MS_U8                           u8RCType;
    MS_BOOL                         bInit;
} SC_T1_State;

typedef enum
{
    SC_T1_R_OK,
    SC_T1_R_EDC_ERROR,
    SC_T1_R_OTHER_ERROR,
} SC_T1_RBlock_State;

typedef enum
{
    SC_T1_S_IFS,
    SC_T1_S_ABORT,
    SC_T1_S_WTX,
    SC_T1_S_RESYNCH,
    SC_T1_S_REQUEST,
    SC_T1_S_RESPONSE,
} SC_T1_SBlock_State;

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

static SC_T1_State                  T1State[SC_DEV_NUM_MAX] = {
                                        {
                                        .u8NAD      = 0,
                                        .u8NS       = 0,
                                        .u8NR       = 0,
                                        .bMore      = FALSE,
                                        .u8IFSC     = SC_T1_DEFAULT_IFSC, //254
                                        .u8RCType   = 0x00,
					.bInit      = FALSE,
                                        }
                                    };

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------

static void _MApi_SC_T1AppendRC( SC_T1_State *pT1, MS_U8 *pu8Block, MS_U16 *pu16BlockLen  )
{
    if ((pT1 == NULL) || (pu8Block == NULL) || (pu16BlockLen == NULL))
    {
        return;
    }
    //RC-CRC
    if (pT1->u8RCType & 0x1)
    {
        MS_U32 i;
        MS_U16 u16CRC = 0xFFFF;
        MS_U16 u16BlockLenLocal = *pu16BlockLen;

        while (u16BlockLenLocal--)
        {
            u16CRC ^= *pu8Block++;
            for (i=0;i<8;i++)
            {
                if (u16CRC & 0x01)
                {
                    // LSB = 1
                    u16CRC = (u16CRC>>1) ^ 0xA001;
                }
                else
                {
                    // LSB = 2
                    u16CRC = u16CRC >>1;
                }
            }
        }
        pu8Block[*pu16BlockLen]=(u16CRC >> 8) & 0xFF;
        pu8Block[*pu16BlockLen+1]=u16CRC & 0xFF;
        *pu16BlockLen +=2;
    }
    //RC-LRC
    else
    {
        MS_U8 u8LRC=0x00;
        MS_U8 i;
        for (i=0; i<(*pu16BlockLen); i++)
        {
            u8LRC ^= pu8Block[i];
        }
        pu8Block[*pu16BlockLen] = u8LRC;
        *pu16BlockLen +=1;
    }

}


static MS_BOOL _MApi_SC_T1IBlock( SC_T1_State *pT1, MS_BOOL ubMore, MS_U8 *pu8Data, MS_U16 u16DataLen, MS_U8 *pu8Block, MS_U16 *pu16BlockLen )
{
    if ((pT1 == NULL) || (pu8Data == NULL) || (pu8Block == NULL) || (pu16BlockLen == NULL))
    {
        return FALSE;
    }

    //[1] NAD
    pu8Block[0] = pT1->u8NAD;

    //[2] PCB - N(S),More bit
    pu8Block[1] = 0x00;
    if (pT1->u8NS)
    {
        pu8Block[1] |= 0x40;
    }
    if (ubMore)
    {
        pu8Block[1] |= 0x20;
    }
    if (u16DataLen > pT1->u8IFSC)
    {
        return FALSE;
    }

    //[3] LEN
    pu8Block[2]=(MS_U8) u16DataLen;

    //[4] DATA
    memcpy( pu8Block+3, pu8Data, u16DataLen );

    *pu16BlockLen=u16DataLen+3;

    //[5] EDC
    _MApi_SC_T1AppendRC(pT1,pu8Block,pu16BlockLen);

    return TRUE;
}


// build R block
static void _MApi_SC_T1RBlock( SC_T1_State *pT1, MS_U32 u32Type, MS_U8 *pu8Block, MS_U16 *pu16BlockLen )
{
    if ((pT1 == NULL) || (pu8Block == NULL) || (pu16BlockLen == NULL))
    {
        return;
    }

    pu8Block[0] = pT1->u8NAD;
    pu8Block[2] = 0x00;

    if (u32Type == SC_T1_R_OK)
    {
        if(pT1->u8NR)
        {
            pu8Block[1]=0x90;
        }
        else
        {
            pu8Block[1]=0x80;
        }
    }
    else if (u32Type == SC_T1_R_EDC_ERROR)
    {
        if (pT1->u8NR)
        {
            pu8Block[1]=0x91;
        }
        else
        {
            pu8Block[1]=0x81;
        }
    }
    else if (u32Type == SC_T1_R_OTHER_ERROR)
    {
        if (pT1->u8NR)
        {
            pu8Block[1]=0x92;
        }
        else
        {
            pu8Block[1]=0x82;
        }
    }
    *pu16BlockLen=3;

    _MApi_SC_T1AppendRC(pT1,pu8Block,pu16BlockLen);
}


static MS_BOOL _MApi_SC_T1SBlock( SC_T1_State *pT1, MS_U32 u32Type, MS_U32 u8Dir, MS_U8 u8Param, MS_U8 *pu8Block,
    MS_U16 *pu16len )
{
    if ((pT1 == NULL) || (pu8Block == NULL) || (pu16len ==NULL))
    {
        return FALSE;
    }

    pu8Block[0]=pT1->u8NAD;

    switch (u32Type)
    {
    case SC_T1_S_RESYNCH:
        if (u8Dir==SC_T1_S_REQUEST)
        {
            pu8Block[1]=0xC0;
        }
        else
        {
            pu8Block[1]=0xE0;
        }
        pu8Block[2]=0x00;
        *pu16len=3;
        break;
    case SC_T1_S_IFS:
        if (u8Dir == SC_T1_S_REQUEST)
        {
            pu8Block[1]=0xC1;
        }
        else
        {
            pu8Block[1]=0xE1;
        }
        pu8Block[2]=0x01;
        pu8Block[3]= u8Param;
        *pu16len=4;
        break;
    case SC_T1_S_ABORT:
        if (u8Dir == SC_T1_S_REQUEST)
        {
            pu8Block[1]=0xC2;
        }
        else
        {
            pu8Block[1]=0xE2;
        }
        pu8Block[2]=0x00;
        *pu16len=3;
        break;
    case SC_T1_S_WTX:
        if (u8Dir == SC_T1_S_REQUEST)
        {
            pu8Block[1]=0xC3;
        }
        else
        {
            pu8Block[1]=0xE3;
        }
        pu8Block[2]=0x01;
        pu8Block[3]= u8Param;
        *pu16len=4;
        break;
    default:
        return FALSE;
    }
    _MApi_SC_T1AppendRC(pT1, pu8Block, pu16len);

    return TRUE;
}

//get block S(n),R(n)
static MS_U8  _MApi_SC_T1GetBlockN( MS_U8 *pu8Block )
{
    if (pu8Block == NULL)
    {
        return 0;
    }
    // I-Block
    if ((pu8Block[1] & 0x80) == 0x00)
    {
        return((pu8Block[1]>>6) & 0x01);
    }

    // R-Block
    if ((pu8Block[1]&0xC0) == 0x80)
    {
        return((pu8Block[1]>>4) & 0x01);
    }
    return 0;
}

static MS_BOOL _MApi_SC_T1SendRecvBlock( MS_U8 u8SCID,MS_U8 *pu8Block, MS_U16 *pu16BlockLen, MS_U8 *pu8RspBlock, MS_U16 *pu16RspBlockLen)
{
    SC_Result ret_Result = E_SC_FAIL;

    if ((pu8Block == NULL) || (pu16BlockLen == NULL) || (pu8RspBlock == NULL) || (pu16RspBlockLen == NULL))
    {
        return FALSE;
    }

    MsOS_DelayTask(300);
    //send data
    ret_Result = MDrv_SC_Send(u8SCID, pu8Block, *pu16BlockLen, SC_T1_SEND_TIMEOUT);
    if (ret_Result != E_SC_OK)
    {
        return FALSE;
    }
    //block waiting time
    MsOS_DelayTask(300);

    //receive data
    ret_Result = MDrv_SC_Recv(u8SCID, pu8RspBlock, pu16RspBlockLen, SC_T1_RECV_TIMEOUT);
    
    if (ret_Result != E_SC_OK)
    {
        return FALSE;
    }
    return TRUE;
}



SC_API_Result MApi_SC_T1_Parse_Atr(MS_U8 u8SCID, MS_U8 *pu8AtrData, MS_U16 u16AtrLen)
{
    MS_U8               u8TS, u8T0, u8TAx, u8TBx, u8TCx, u8TDx, u8Len, u8TA2;
    MS_U8               u8Ch, u8Crc = 0;
    MS_U8               u8Protocol;

    int                 i, x;


    if ( pu8AtrData == NULL)
    {
        return E_API_SC_PARMERROR;
    }

    if ( (u16AtrLen < SC_ATR_LEN_MIN) || (u16AtrLen > SC_ATR_LEN_MAX) )
    {
        SC_API_DBG("    Length error\n");
        return E_API_SC_PARMERROR;
    }


    if ((pu8AtrData[0] != 0x3B) && (pu8AtrData[0] != 0x3F))
    {
        SC_API_DBG("    first byte error 0x%02X\n", pu8AtrData[0]);
        return E_API_SC_DATAERROR;
    }

    i = 0;
    u8TS = pu8AtrData[i++];
    u8T0 = pu8AtrData[i++];

    u8Crc = u8Crc ^ u8T0;
    u8Len = u8T0 & 0x0F;
    u8TDx = u8T0 & 0xF0;

    x = 1;
    //_scInfo[u8SCID].bSpecMode = FALSE;
    //_scInfo[u8SCID].u8Di = _scInfo[u8SCID].u8Fi = 0;
    u8Protocol = 0;
    while (u8TDx & 0xF0)
    {
        if (i >= u16AtrLen)
        {
            return E_API_SC_FAIL;
        }

        if (u8TDx & 0x10) // TA
        {
            u8TAx = pu8AtrData[i++];
            u8Crc = u8Crc ^ u8TAx;
            if (x == 1)
            {
                //_scInfo[u8SCID].u8Fi = u8TAx >> 4;
                //_scInfo[u8SCID].u8Di = u8TAx & 0xF;
            }
            if (x == 2)
            {
                //_scInfo[u8SCID].bSpecMode = TRUE; // specific mode
                u8TA2 = u8TAx;
            }
            if (x == 3)
            {
		// TA3 is for encoding T1 IFSC
		if ( (u8TAx != 0x00) && (u8TAx != 0xFF) )
	        {
		    T1State[u8SCID].u8IFSC = u8TAx;
		}
            }
        }
        if (u8TDx & 0x20) // TB
        {
            u8TBx = pu8AtrData[i++];
            u8Crc = u8Crc ^ u8TBx;
	    // TB3 is for encoding T1 BWI and CWI

        }
        if (u8TDx & 0x40) // TC
        {
            u8TCx = pu8AtrData[i++];
            u8Crc = u8Crc ^ u8TCx;
            if (x == 3)
            {
		// TC3 is for encoding T1 CRC type
                T1State[u8SCID].u8RCType = u8TCx & 0x1;
            }
        }
        if ((u8TDx & 0x80))
        {
            u8TDx = pu8AtrData[i++];
            if ((u8TDx & 0xF) != 0)
            {
                u8Protocol = u8TDx & 0xF;
            }
            u8Crc = u8Crc ^ u8TDx;

            SC_API_DBG("    T=%d\n", u8Protocol);
            x++;
        }
        else
        {
            break;
        }
    }

    // Get ATR history
    //_scInfo[u8SCID].u16HistLen = u8Len;
    for (x = 0; x < u8Len; x++)
    {
        if (i >= u16AtrLen)
        {
            return E_API_SC_FAIL;
        }
        u8Ch = pu8AtrData[i++];
        u8Crc = u8Crc ^ u8Ch;

        //_scInfo[u8SCID].pu8Hist[x] = u8Ch;
    }

    // Check ATR checksum
    if ((u8Protocol != 0x00) && (u8Protocol != 0xFF)) // If not T=0
    {
        if (i >= u16AtrLen)
        {
            return E_API_SC_FAIL;
        }
        if (u8Crc != pu8AtrData[i++])
        {
	    // CRC error
            return E_API_SC_CRCERROR;
        }
    }

    // Check ATR length
    if (i != u16AtrLen)
    {
        return E_API_SC_FAIL; // len error
    }

    T1State[u8SCID].bInit = TRUE;
    return E_API_SC_OK;

}

SC_API_Result MApi_SC_T1_Communication(MS_U8 u8SCID, MS_U8 *pu8SendData, MS_U16 *pu16SendDataLen, MS_U8 *pu8ReadData, MS_U16 *pu16ReadDataLen)
{
    //SC_T1_INFO  eSendInfo;               //SC_T1_INFO eRecvInfo;
    SC_Result ret_Result = E_SC_FAIL;
    MS_U8   _u8HandleErrcount = 0;
    MS_U16  u16SentIdx = 3;
    MS_U16  u16BlockAPDULen = 0;
    MS_U8   u8Block[255];
    MS_U8   u8RspBlock[255];
    MS_U16  u8BlockLen = 0;
    MS_U16  u8RspRBlkLen = *pu16ReadDataLen;
    MS_U8   u8EDCByteNum = 0;

    T1State[u8SCID].u8NAD = pu8SendData[0];
    memset(u8Block,0,255);
    memset(u8RspBlock,0,255);

    *pu16ReadDataLen = 0;

    SC_API_DBG("%s\n", __FUNCTION__);

    
    if ((u8SCID >= SC_DEV_NUM_MAX) ||
        (pu8SendData == NULL)      ||
        (pu16SendDataLen == NULL)  ||
        (pu8ReadData == NULL)      ||
	(pu16ReadDataLen == NULL))
    {
        return E_API_SC_PARMERROR;
    }
    if (T1State[u8SCID].bInit == FALSE)
    {
        //T1 parameters have not been parsed from ATR
	printf("ATR should be parsed first\n Call MApi_SC_T1_Parse_Atr()\n");
        return E_API_SC_FAIL;
    }

    //info to get the first block(I-Block,S-Block)
    if (*pu16SendDataLen <=  T1State[u8SCID].u8IFSC)
    {
        T1State[u8SCID].bMore = FALSE;
        u16BlockAPDULen = *pu16SendDataLen - 4;
    }
    else
    {
        T1State[u8SCID].bMore = TRUE;
        u16BlockAPDULen = T1State[u8SCID].u8IFSC;
    }
    u16SentIdx = 3;

    //I-Block
    if ((pu8SendData[1] & 0x80) == 0x00 )
    {
        T1State[u8SCID].u8NS = pu8SendData[1]&0x40;
        _MApi_SC_T1IBlock( &T1State[u8SCID], T1State[u8SCID].bMore,pu8SendData+u16SentIdx, u16BlockAPDULen,u8Block, &u8BlockLen );
    }
    //S-Block
    else if ((pu8SendData[1] & 0xC0) == 0xC0 )
    {
        u8BlockLen = *pu16SendDataLen;
        memcpy(u8Block,pu8SendData,u8BlockLen);
    }

    //set temp EDC byte number
    if (T1State[u8SCID].u8RCType & 0x1)
    {
        u8EDCByteNum = 2;   //CRC
    }
    else
    {
        u8EDCByteNum = 1;   //LRC
    }

    while (u16SentIdx <  *pu16SendDataLen-u8EDCByteNum)
    {

	//send Block
        ret_Result = _MApi_SC_T1SendRecvBlock(u8SCID,u8Block, &u8BlockLen,u8RspBlock, &u8RspRBlkLen);
        if (ret_Result != TRUE)
        {
            if (_u8HandleErrcount < 3)
            {
                _MApi_SC_T1RBlock( &T1State[u8SCID], SC_T1_R_OTHER_ERROR, u8Block, &u8BlockLen);
                _u8HandleErrcount++;
                continue;
            }
            else
            {
                SC_API_DBG("%s Send block fail", __FUNCTION__);
                return E_API_SC_FAIL;
            }
        }
        _u8HandleErrcount = 0;

        //receive R-block
        if ((u8RspBlock[1] & 0xC0) == 0x80)
        {
            if (_u8HandleErrcount > 3)
            {
                SC_API_DBG("%s Receive block fail", __FUNCTION__);
                return E_API_SC_FAIL;
            }
            //resend I-block
            if (_MApi_SC_T1GetBlockN(u8RspBlock) == T1State[u8SCID].u8NS)
            {
                u16SentIdx -= u16BlockAPDULen;
            }
            else
            {
                 T1State[u8SCID].u8NS ^= 1;
                 u16SentIdx += u16BlockAPDULen;
            }
            //build the next I-Block
            u16BlockAPDULen = MIN(T1State[u8SCID].u8IFSC,*pu16SendDataLen-u16SentIdx-u8EDCByteNum);

            // send the remain data
            if (u16BlockAPDULen  != 0)
            {
                //last apdu data
                if (*pu16SendDataLen-u16SentIdx-u8EDCByteNum == u16BlockAPDULen)
                {
                    T1State[u8SCID].bMore = FALSE;
                }
                _MApi_SC_T1IBlock( &T1State[u8SCID],T1State[u8SCID].bMore,pu8SendData+u16SentIdx,u16BlockAPDULen,u8Block, &u8BlockLen );
            }
        }
         // receive I-Block   todo : receive I-Block(complete?) when send I-Block
        else if ((u8RspBlock[1] & 0x80) == 0x00)
        {
            //receive invalid block
            if (_MApi_SC_T1GetBlockN(u8RspBlock) != T1State[u8SCID].u8NR)
            {
                _MApi_SC_T1RBlock( &T1State[u8SCID], SC_T1_R_OTHER_ERROR, u8Block, &u8BlockLen);
                continue;
            }
            //copy receive data
            memcpy( pu8ReadData+*pu16ReadDataLen+3, u8RspBlock+3, u8RspBlock[2] );
            *pu16ReadDataLen += (MS_U16)(u8RspBlock[2]);
            T1State[u8SCID].u8NR ^= 1;

            //send R-Block
            if ((u8RspBlock[1]>>5) & 1)
            {
                _MApi_SC_T1RBlock( &T1State[u8SCID], SC_T1_R_OK, u8Block, &u8BlockLen);
                continue;
            }
            u16SentIdx += u16BlockAPDULen;              //to check

        }
         // receive S-Block
        else if ((u8RspBlock[1] & 0xC0) == 0xC0)
        {
            //IFS request
            if (u8RspBlock[1] == 0xC1)
            {
                T1State[u8SCID].u8IFSC = u8RspBlock[3];
                if (_MApi_SC_T1SBlock( &T1State[u8SCID], SC_T1_S_IFS, SC_T1_S_RESPONSE,u8RspBlock[3], u8Block, &u8BlockLen ) != TRUE)
                {
                    SC_API_DBG("%s build IFS s-block fail", __FUNCTION__);
                    return E_API_SC_FAIL;
                }
                else
                {
                    continue;
                }
            }
            //ABORT reques
            else if (u8RspBlock[1] == 0xC2)
            {
               if (_MApi_SC_T1SBlock( &T1State[u8SCID], SC_T1_S_ABORT, SC_T1_S_RESPONSE,0x00, u8Block, &u8BlockLen ) != TRUE)
               {
                    SC_API_DBG("%s build ABORT s-block fail", __FUNCTION__);
                    return E_API_SC_FAIL;
               }
               else
               {
                    continue;
               }
            }
            //S-Block WTX Reques
            else if (u8RspBlock[1]==0xC3)
            {
                MsOS_DelayTask(u8RspBlock[3]);          // todo check
                if (_MApi_SC_T1SBlock( &T1State[u8SCID], SC_T1_S_WTX, SC_T1_S_RESPONSE, u8RspBlock[3],u8Block, &u8BlockLen )!= TRUE)
                {
                    SC_API_DBG("%s build WTX S-Block fail", __FUNCTION__);
                    return E_API_SC_FAIL;
                }
                else
                {
                    continue;
                }
            }
            else
            {
                memcpy( pu8ReadData+*pu16ReadDataLen+3, u8RspBlock+3, u8RspBlock[2] );
                *pu16ReadDataLen += (MS_U16)(u8RspBlock[2]);
                u16SentIdx += u16BlockAPDULen;
            }

        }
        else
        {
            // error handling
            if (_u8HandleErrcount < 3)
            {
                _MApi_SC_T1RBlock( &T1State[u8SCID], SC_T1_R_OTHER_ERROR, u8Block, &u8BlockLen);
                _u8HandleErrcount++;
                continue;
            }
            else
            {
		return E_API_SC_FAIL;
            }
            _u8HandleErrcount = 0;
        }
    }

    //NAD , PCB , LEN , EDC of TPDU
    pu8ReadData[0] = u8RspBlock[0];   //NAD information
    pu8ReadData[1] = u8RspBlock[1];   //PCB
    pu8ReadData[2] = (MS_U8)*pu16ReadDataLen;
    *pu16ReadDataLen += 3;
    _MApi_SC_T1AppendRC(&T1State[u8SCID],pu8ReadData,pu16ReadDataLen);    //EDC

    return E_API_SC_OK;
}

