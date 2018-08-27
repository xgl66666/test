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
/// file    apiSC_T0.c
/// @brief  High level SmartCard Driver Interface for T=0
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
#define SC_T0_SEND_TIMEOUT                  100
#define SC_T0_RECV_TIMEOUT                  1000
#define SC_WAIT_DELAY                       20
#define SC_NULL_DELAY                       10 
#define SC_T0_RECV_RETRY_COUNT              (8000 / SC_T0_RECV_TIMEOUT)//(2000 / SC_T0_RECV_TIMEOUT)

#define T0_WAIT_PROBYTE_TIMEOUT             4000 //if more data send, card need time to handle
#define T0_IS_SW1(ProByte)                  ((((ProByte)&0xF0)==0x60)||(((ProByte)&0xF0)==0x90))
#define T0_IS_ONEBYTE(INS,ACK)              ((((INS)^(ACK))&0xFE)==0xFE)
#define T0_IS_ALLDATA(INS,ACK)              ((((INS)^(ACK))&0xFE)==0x00)
#define T0_ACT_VPP(INS,ACK)                 ((((INS)^(ACK))==0x01)||(((INS)^(ACK))==0xFE))
#define T0_RLS_VPP(INS,ACK)                 ((((INS)^(ACK))==0x00)||(((INS)^(ACK))==0xFF))

#define IsTimeOut(StartTime,TimeOut)    ((MsOS_GetSystemTime()-StartTime)>TimeOut)

#define SC_API_T0_DBG_ENABLE           0

#if SC_API_T0_DBG_ENABLE
#define SC_API_DBG(fmt, args...)         printf(fmt, ##args)
#else
#define SC_API_DBG(fmt, args...)         {}
#endif
//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------
typedef enum _T0_PROCEDURE_BYTE_TYPE
{
    T0_PROCEDURE_BYTE_FAIL=0,
    //T0_PROCEDURE_BYTE_NULL,//null handled by functionself
    //T0_PROCEDURE_BYTE_ACK,
    //T0_PROCEDURE_BYTE_SW1,
    //T0_PROCEDURE_BYTE_DATA,
    T0_PROCEDURE_BYTE_SUC,
}T0_PROCEDURE_BYTE_TYPE;

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
#ifndef SC_USE_IO_51
MS_BOOL _MApi_SC_T0_GetBytes(MS_U8 u8SCID, MS_U8 *pChar,MS_U16 *pu16Size,MS_U32 StartTime,MS_U32 TimeOut)
{
    if ((pChar == NULL) || (pu16Size == NULL))
    {
        return FALSE;
    }

    MS_U16 u16Len=*pu16Size;
    MS_U16 nIndex=0;
    SC_Result eResult;
    while (1)
    {
        eResult = MDrv_SC_Recv(u8SCID, (pChar+nIndex),pu16Size, SC_T0_RECV_TIMEOUT);
        if (eResult == E_SC_OK)
        {
            nIndex+=(*pu16Size);
            (*pu16Size)=(u16Len-nIndex);
        }
        else if (eResult == E_SC_TIMEOUT)
        {
            *pu16Size = u16Len;
        }

        if (nIndex>=u16Len)
        {
            break;
        }
        if (IsTimeOut(StartTime,TimeOut))
        {
            (*pu16Size)=nIndex;
            return FALSE;
        }
    }
    (*pu16Size)=nIndex;
    return TRUE;
}
#else //handle timeout by 51
MS_BOOL _MApi_SC_T0_GetBytes(MS_U8 u8SCID, MS_U8 *pChar,MS_U16 *pu16Size,MS_U32 StartTime,MS_U32 TimeOut)
{
    if ((pChar == NULL) || (pu16Size == NULL))
    {
        return FALSE;
    }

    SC_Result eResult;
    StartTime = StartTime;
    TimeOut = TimeOut;

    eResult = MDrv_SC_Recv(u8SCID, pChar, pu16Size, 0);

    if (eResult != E_SC_OK)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }

}
#endif
T0_PROCEDURE_BYTE_TYPE _MApi_SC_T0_WaitProcedureByte(MS_U8 u8SCID,MS_U8 *pU8,MS_U8 u8Ins,MS_U32 TimeOut)
{
    if (pU8 == NULL)
    {
        return T0_PROCEDURE_BYTE_FAIL;
    }

    MS_U16 DataSize=1;
    MS_U32 StarTime=MsOS_GetSystemTime();
    do
    {
        DataSize=1;
#ifndef SC_USE_IO_51
        if (IsTimeOut(StarTime, TimeOut) || !_MApi_SC_T0_GetBytes(u8SCID, pU8,&DataSize,StarTime,TimeOut))
#else
        if (!_MApi_SC_T0_GetBytes(u8SCID, pU8,&DataSize,StarTime,TimeOut))
#endif
        {
            return T0_PROCEDURE_BYTE_FAIL;
        }
    } while(*pU8==0x60);

    if (T0_ACT_VPP(u8Ins,*pU8))
    {

    }
    else if (T0_RLS_VPP(u8Ins,*pU8))
    {


    }

    return T0_PROCEDURE_BYTE_SUC;
}

SC_Result _MApi_SC_T0_SendCAPDU(MS_U8 u8SCID, MS_U8 u8Ins,MS_U16 u16Lc, MS_U8 *pData, MS_U8 u8AckSw[2],MS_U32 *pu32RetLen)
{
    SC_Result           eResult;
    MS_U8               u8Temp1;
    MS_U16              u16Len,pos;

    if ((pData == NULL) || (pu32RetLen == NULL))
    {
        return E_SC_PARMERROR;
    }
    //SC_API_DBG("%s,%d: u16Lc=%d\n",__FUNCTION__,__LINE__,u16Lc);
    (*pu32RetLen)=0;
    pos=0;
    while (u16Lc)
    {
        // Get ACK ------------------------------------------------------------------------------------
        if (_MApi_SC_T0_WaitProcedureByte(u8SCID, &u8Temp1,u8Ins,T0_WAIT_PROBYTE_TIMEOUT)==T0_PROCEDURE_BYTE_FAIL)
        {
            SC_API_DBG("%s, %d: Wait Ack fail\n",__FUNCTION__,__LINE__);
            eResult = E_SC_FAIL;
            return eResult;
        }
        if (T0_IS_SW1(u8Temp1))
        {
            u8AckSw[(*pu32RetLen)++]=u8Temp1;
            u16Len=1;
            if (!_MApi_SC_T0_GetBytes(u8SCID,&u8Temp1, &u16Len,MsOS_GetSystemTime(),SC_T0_RECV_TIMEOUT))
            {
                eResult = E_SC_FAIL;
                SC_API_DBG("%s, %d: Wait Ack fail\n",__FUNCTION__,__LINE__);
                return eResult;
            }
            u8AckSw[(*pu32RetLen)++]=u8Temp1;
            SC_API_DBG("%s,%d: get pro byte2=%x\n",__FUNCTION__,__LINE__,u8Temp1);
            eResult = E_SC_OK;
            return eResult;
        }
        else if (T0_IS_ONEBYTE(u8Ins,u8Temp1))
        {
            u16Len=1;
            if (((eResult = MDrv_SC_Send(u8SCID, &pData[pos], u16Len, SC_T0_SEND_TIMEOUT)) != E_SC_OK))
            {
                SC_API_DBG("%s, %d: Send Data error, Lc=%d\n",__FUNCTION__,__LINE__,u16Lc);
                return eResult;
            }
            u16Lc -= 1;
            pos+=1;
        }
        else if (T0_IS_ALLDATA(u8Ins,u8Temp1))
        {
            while (u16Lc)
            {
                u16Len=u16Lc;
                if (((eResult = MDrv_SC_Send(u8SCID, pData+pos, u16Len, (u16Len>120)?(SC_T0_SEND_TIMEOUT*2):(SC_T0_SEND_TIMEOUT))) != E_SC_OK))
                {
                    SC_API_DBG("%s, %d: Send data fail, Lc=%d\n",__FUNCTION__,__LINE__,u16Lc);
                    return eResult;
                }
                u16Lc-=u16Len;
                pos+=u16Len;
            }
        }
        else
        {
            eResult = E_SC_FAIL;
            SC_API_DBG("%s, %d: Error Ack=%x pos=%d, ins=%x,\n",__FUNCTION__,__LINE__,u8Temp1,pos,u8Ins);
            return eResult;
        }
    }
    eResult = E_SC_OK;

    return eResult;
}
SC_Result _MApi_SC_T0_RcvRAPDU(MS_U8 u8SCID, MS_U8 u8Ins,MS_U16 u16Le, MS_U8 *pData,MS_U32 *pu32RetLen)
{
    SC_Result           eResult;
    MS_U8               u8Temp1;
    MS_U16              u16Len;

    if((pData == NULL) || (pu32RetLen == NULL))
    {
	return E_SC_PARMERROR;
    }

    (*pu32RetLen)=0;
    while(1)
    {
        // Get ACK ------------------------------------------------------------------------------------
        if (_MApi_SC_T0_WaitProcedureByte(u8SCID, &u8Temp1,u8Ins,T0_WAIT_PROBYTE_TIMEOUT)==T0_PROCEDURE_BYTE_FAIL)
        {
             SC_API_DBG("%s, %d: Wait Ack fail,Le=%d\n",__FUNCTION__,__LINE__,u16Le);
             eResult = E_SC_FAIL;
             return eResult;
        }
        if (T0_IS_SW1(u8Temp1))
        {
            pData[(*pu32RetLen)++]=u8Temp1;
            u16Len=1;
            if (!_MApi_SC_T0_GetBytes(u8SCID,&u8Temp1, &u16Len,MsOS_GetSystemTime(),SC_T0_RECV_TIMEOUT))
            {
                SC_API_DBG("%s, %d: Wait Ack fail\n",__FUNCTION__,__LINE__);
                eResult = E_SC_FAIL;
                return eResult;
            }
            SC_API_DBG("%s,%d: get pro byte2=%x\n",__FUNCTION__,__LINE__,u8Temp1);
            pData[(*pu32RetLen)++]=u8Temp1;
            eResult = E_SC_OK;
            return eResult;
        }
        else if (T0_IS_ONEBYTE(u8Ins,u8Temp1))
        {
            #if 0
            if (u16Le==0)
            {
                pData[(*pu32RetLen)++]=u8Temp1;
                eResult = E_SC_FAIL;
                return eResult;
            }
            #endif
            u16Len=1;
            if (!_MApi_SC_T0_GetBytes(u8SCID,&u8Temp1, &u16Len,MsOS_GetSystemTime(),SC_T0_RECV_TIMEOUT))
            {
                SC_API_DBG("%s, %d: Get data fail,Le=%d\n",__FUNCTION__,__LINE__,u16Le);
                eResult = E_SC_FAIL;
                return eResult;
            }
            pData[(*pu32RetLen)++]=u8Temp1;
            u16Le-=1;
        }
        else if (T0_IS_ALLDATA(u8Ins,u8Temp1))
        {
            #if 0
            if (u16Le==0)
            {
                pData[(*pu32RetLen)++]=u8Temp1;
                eResult = E_SC_FAIL;
                return eResult;
            }
            #endif
#ifndef SC_USE_IO_51
            while (u16Le)
#else
            if (u16Le)
#endif
            {
                u16Len=u16Le;
                if (!_MApi_SC_T0_GetBytes(u8SCID,&pData[(*pu32RetLen)], &u16Len, MsOS_GetSystemTime(),(u16Len>120)?(u16Len*5):(SC_T0_RECV_TIMEOUT)))
                {
                    SC_API_DBG("%s, %d: Get Data fail,Le=%d\n",__FUNCTION__,__LINE__,u16Le);
                    (*pu32RetLen)+=u16Len;
                    u16Le-=u16Len;
                    eResult = E_SC_FAIL;
                    return eResult;
                }
                (*pu32RetLen)+=u16Len;
                u16Le-=u16Len;
            }
        }
        else
        {
            eResult = E_SC_FAIL;
            SC_API_DBG("%s, %d: Error Ack\n",__FUNCTION__,__LINE__);
            return eResult;
        }
    }
    eResult = E_SC_FAIL;
    SC_API_DBG("%s, %d: Should never here\n",__FUNCTION__,__LINE__);
    return eResult;
}

void _MApi_SC_T0_ClearFifo(MS_U8 u8SCID)
{
    MS_U32              u32PollingCount;
    MS_U16              u16Len;
    MS_U8               u8Temp1;
    u32PollingCount = 0;
    {
        int i = 0;
        do
        {
            u16Len = 1;
            if (MDrv_SC_Recv(u8SCID, &u8Temp1, &u16Len, 5) != E_SC_OK)
            {
                break;
            }
            if (u16Len == 0)
            {
                break;
            }
            i = i + u16Len;
        }
        while (u32PollingCount++ < SC_T0_RECV_RETRY_COUNT);
        if (i)
        {
            SC_API_DBG("CAUTION: fifo not empty %d\n", i);
        }
    }
}


//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
/// Send/Recv T=0 smartcard command and data
/// @param  u8SCID                  \b IN: Uart device pad.
/// @param  pu8SendData             \b IN:  pointer of send data
/// @param  pu16SendDataLen         \b IN:  length of send data
/// @param  pu8ReadData             \b OUT: pointer of received data
/// @param  pu16ReadDataLen             \b OUT: length of received data
/// @return @ref SC_API_Result
//-------------------------------------------------------------------------------------------------
SC_API_Result MApi_SC_T0_Communication(MS_U8 u8SCID, MS_U8 *pu8SendData, MS_U16 *pu16SendDataLen, MS_U8 *pu8ReadData, MS_U16 *pu16ReadDataLen)
{
    SC_Result           eResult;
    MS_U32              u32RetLen;
    MS_U16              u16Lc,u16Le;


//[DEBUG]
/*
    {
        int             i;
        SC_API_DBG("    Send: ");
        for (i = 0; i < *pu16SendDataLen; i++)
        {
            SC_API_DBG("0x%02x ", pu8SendData[i]);
            if (((i+1) % 16)==0) {
                SC_API_DBG("\n          ");
            }
        }
        SC_API_DBG("\n");
    }
*/
//[DEBUG]
    if ((pu8SendData == NULL) || (pu16SendDataLen == NULL) || (pu8ReadData == NULL) || (pu16ReadDataLen == NULL))
    {
        return E_API_SC_PARMERROR;
    }

    *pu16ReadDataLen = 0;

    // Command APDU
    // Case 1: [Command Header]
    if (*pu16SendDataLen==4)
    {
        u16Lc=0;
        u16Le=0;
    }
    // Case 2: [Command Header][Le field]
    else if (*pu16SendDataLen==5)
    {
        u16Lc=0;
        u16Le=pu8SendData[4];
        if(u16Le==0)
	{
            u16Le=256;
	}
    }
    // Case 3: [Command Header][Lc field][Data Field]
    else if ((*pu16SendDataLen>5)&&(*pu16SendDataLen==(5+pu8SendData[4])))
    {
        u16Lc=pu8SendData[4];
        u16Le=0;
    }
    // Case 4: [Command Header][Lc field][Data Field][Le field]
    else if ((*pu16SendDataLen>5)&&(*pu16SendDataLen==(5+pu8SendData[4]+1)))
    {
        u16Lc=pu8SendData[4];
        u16Le=pu8SendData[5+u16Lc];
        if (u16Le==0)
	{
            u16Le=256;
	}
    }
    #if 0
    // FIXME:This is a patch. There is one command not compatible with T=0 protocol in Tongfang CA
    else if ((*pu16SendDataLen>5)&&(*pu16SendDataLen !=(5+pu8SendData[4]+1)))
    {
        SC_API_DBG("%s, %d: Wrong Data Len\n",__FUNCTION__,__LINE__);
        u16Lc=pu8SendData[4];
        // u16Le=pu8SendData[5+u16Lc];
        // if(u16Le==0)
        u16Le=256;
    }
    #endif
    else
    {
        eResult = E_API_SC_DATAERROR;
        SC_API_DBG("%s, %d: Wrong Data Len\n",__FUNCTION__,__LINE__);
        goto clear_fifo;
    }

    MS_U8 u8Header[5] = {*pu8SendData, *(pu8SendData+1), *(pu8SendData+2), *(pu8SendData+3), 0 };
    if (*pu16SendDataLen > 4)
    {
        if (u16Lc)
        {
             u8Header[4] = (MS_U8)u16Lc;
        }
        else
        {
             u8Header[4] = (MS_U8)u16Le; 
        }
    }

    eResult = MDrv_SC_Send(u8SCID, u8Header, 5, SC_T0_SEND_TIMEOUT);
    if (eResult != E_SC_OK)
    {
        SC_API_DBG("%s, %d: Send Header Fail\n",__FUNCTION__,__LINE__);
        return E_API_SC_FAIL;
    }
    
    if (u16Lc)
    {
        eResult = _MApi_SC_T0_SendCAPDU(u8SCID, pu8SendData[1],u16Lc, &pu8SendData[5], pu8ReadData,&u32RetLen);
        if (eResult != E_SC_OK)
        {
            *pu16ReadDataLen=(MS_U16)u32RetLen;
            SC_API_DBG("%s, %d: Send CAPDU fail\n",__FUNCTION__,__LINE__);
            goto clear_fifo;
        }
        *pu16ReadDataLen=(MS_U16)u32RetLen;
        //SC_API_DBG("%s,%d: acksw len=%lu\n",__FUNCTION__,__LINE__,u32RetLen);
	if ((*pu16ReadDataLen)==2)
        {
            eResult = E_SC_OK;
            goto clear_fifo;
        }
        else if ((*pu16ReadDataLen))
        {
            eResult = E_SC_FAIL;
            SC_API_DBG("%s, %d: Send CAPDU fail\n",__FUNCTION__,__LINE__);
            goto clear_fifo;
        }
    }

    eResult = _MApi_SC_T0_RcvRAPDU(u8SCID, pu8SendData[1],u16Le,pu8ReadData,&u32RetLen);
    if (eResult != E_SC_OK)
    {
        *pu16ReadDataLen=(MS_U16)u32RetLen;
#if 0
        if(((*pu16ReadDataLen)>=2)&&(T0_IS_SW1(pu8ReadData[u32RetLen-2])))//we thought Le is the max data to rcv
        {
            eResult = E_SC_OK;
            SC_API_DBG("%s, %d: Rcv CRPDU not enough data suc\n",__FUNCTION__,__LINE__);
        }
        else if(u32RetLen==u16Le)
        {
            pu8ReadData[u32RetLen++]=0x90;
            pu8ReadData[u32RetLen++]=0x00;
            *pu16ReadDataLen=(MS_U16)u32RetLen;
            eResult = E_SC_OK;
            SC_API_DBG("%s, %d: Rcv CRPDU sw fail, add\n",__FUNCTION__,__LINE__);
        }
        else
#endif
        {
            //  eResult = E_SC_FAIL;
            SC_API_DBG("%s, %d: Rcv CRPDU fail\n",__FUNCTION__,__LINE__);
        }
        goto clear_fifo;
    }
    else
    {
        *pu16ReadDataLen=(MS_U16)u32RetLen;
        eResult = E_API_SC_OK;
    }
clear_fifo:
#ifndef SC_USE_IO_51
    _MApi_SC_T0_ClearFifo(u8SCID);
#endif

//[DEBUG]
/*
    if(eResult == E_SC_OK)
    {
        SC_DBG_(8, "Leo-MDrv_SC_T0_SendRecv Suc!\n");
    }
    else
    {
        SC_DBG_(8, "Leo-MDrv_SC_T0_SendRecv Failed!\n");
    }
    {
        int             i;
        SC_DBG_(8, "    Recv: ");
        for (i = 0; i < *pu16ReadDataLen; i++)
        {
            SC_DBG_(8, "0x%02x ", pu8ReadData[i]);
            if (((i+1) % 16)==0)
            {
                SC_DBG_(8, "\n          ");
            }
        }
        SC_DBG_(8, "\n");
    }
#if 0
    if (eResult == E_SC_OK)
    {
        int             i;
        SC_DBG_(8, "    Recv: ");
        for (i = 0; i < *pu16ReadDataLen; i++)
        {
            SC_DBG_(8, "0x%02x ", pu8ReadData[i]);
            if (((i+1) % 16)==0)
            {
                SC_DBG_(8, "\n          ");
            }
        }
        SC_DBG_(8, "\n");
    }
    else
    {
        SC_DBG_(8, "MDrv_SC_T0_SendRecv Failed!\n");
    }
#endif
*/
//[DEBUG]

    if (eResult != E_SC_OK)
    {
        return E_API_SC_FAIL;
    }
    else 
    {
        return E_API_SC_OK;
    }
}


