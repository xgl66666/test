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

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    drvIR.c
/// @brief  IR Control Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include "MsCommon.h"
#include "string.h"
#include "Board.h"

#if (IR_TYPE_SEL == IR_TYPE_MSTAR_DTV)
#include "IR_MSTAR_DTV.h"
#elif (defined(IR_TYPE_MSTAR_HWRC5) && IR_TYPE_SEL == IR_TYPE_MSTAR_HWRC5)
#include "IR_MSTAR_HWRC5.h"
#elif (defined(IR_TYPE_MSTAR_SWRC5) && IR_TYPE_SEL == IR_TYPE_MSTAR_SWRC5)
#include "IR_MSTAR_SWRC5.h"
#elif (defined(IR_TYPE_MSTAR_SWRC6A) && IR_TYPE_SEL == IR_TYPE_MSTAR_SWRC6A)
#include "IR_MSTAR_SWRC6A.h"
#elif (defined(IR_TYPE_MSTAR_RAW) && IR_TYPE_SEL == IR_TYPE_MSTAR_RAW)
#include "IR_MSTAR_RAW.h"
#endif


#include "mdrv_ir.h"
#include "mhal_ir_reg.h"


#include "IR_PROTOCOL_INSTALL.h"
#ifdef SUPPORT_MULTI_PROTOCOL
#ifdef IR_MODE_SEL
#undef IR_MODE_SEL
#define IR_MODE_SEL IR_TYPE_SWDECODE_MODE
#endif
#ifndef IR_INT_NP_EDGE_TRIG
#define IR_INT_NP_EDGE_TRIG
#endif
#endif
//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------
#define REG(addr)                   (*(volatile MS_U32 *)(addr))
#define IR_PRINT(fmt, args...)      //printf("%04d %s " fmt, __LINE__, __FUNCTION__, ## args)

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------
#if (IR_MODE_SEL==IR_TYPE_FULLDECODE_MODE)
typedef enum
{
    E_IR_KEY_PROPERTY_INIT,
    E_IR_KEY_PROPERTY_1st,
    E_IR_KEY_PROPERTY_FOLLOWING
} IRKeyProperty;
#endif
//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
// Forward declaration
//--------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Local Variables
//-------------------------------------------------------------------------------------------------
#if (IR_MODE_SEL==IR_TYPE_FULLDECODE_MODE)
static MS_U32  _u32_1stDelayTimeMs;
static MS_U32  _u32_2ndDelayTimeMs;
static IRKeyProperty _ePrevKeyProperty;
static MS_U8   _u8PrevKeyCode;
static unsigned long  _ulPrevKeyTime;
static unsigned long  _ulPrevKeyRepeatTime;
static BOOL  _bCheckQuickRepeat;
static unsigned long  _ulLastKeyPresentTime;
#elif (IR_MODE_SEL==IR_TYPE_SWDECODE_MODE)
#if (defined(IR_TYPE_MSTAR_SWRC5) && IR_TYPE_SEL == IR_TYPE_MSTAR_SWRC5)
static MS_U16 _u16CurKeyCode = 0;
static MS_U8 _u8PrevToggle = 0xff;
static MS_U8 _u8CurrentBitNum = 0;
//static MS_U8 _u8Toggle = 0;
static BOOL _bGotBIT1stHalf = FALSE;
static BOOL _bWaitKey = TRUE;
#endif
#endif
static MS_IR_KeyInfo _KeyReceived;   //temporary solution
static IR_Callback _pfIRCallback = NULL;
static IR_RawModCallback _pfIRRawModCallback = NULL;

#if (IR_MODE_SEL == IR_TYPE_RAWDATA_MODE)
static U8   _u8IRRawModeBuf[IR_RAW_DATA_NUM];
static U8   _u8IRGetRawData[IR_RAW_DATA_NUM];
static U32  _u8IRRawModeCount;
static BOOL flag_repeat = TRUE;
static U8  prev_key_rpt = 0x00;
#endif

#ifdef SUPPORT_MULTI_PROTOCOL
static IR_RawModCallback64 _pfIRRawModCallback64 = NULL;
static IR_PROCOCOL_TYPE _eCurentProtocol=E_IR_PROTOCOL_NONE;
IR_PROCOCOL_TYPE _eLastKeyProtocol=E_IR_PROTOCOL_NONE;//last used protocol of processing key success without timeout
static unsigned long long _u64LastData=0;//last rawdata of processing key success without timeout
static unsigned long long _u64ReleaseData=0;//last rawdata of processing key success without timeout
static unsigned long _ulPreShotTime=0;
static IR_PROCOCOL_TYPE _eDetectList[PROTOCOL_SUPPORT_MAX];
static S32 _s32TimerId=-1;
#endif
//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------

#if((IR_MODE_SEL==IR_TYPE_RAWDATA_MODE) || (IR_MODE_SEL==IR_TYPE_FULLDECODE_MODE)||IR_MODE_SEL==IR_TYPE_HWRC_MODE)
static void _MDrv_IR_ClearFIFO(void)
{
    unsigned long i;
    unsigned long fifo_status_addr,fifo_read_pulse_addr;
    //unsigned long fifo_data_addr;
    unsigned long fifo_empty_flag;

    if((IR_MODE_SEL==IR_TYPE_RAWDATA_MODE) || (IR_MODE_SEL==IR_TYPE_FULLDECODE_MODE))
    {
        fifo_status_addr = REG_IR_SHOT_CNT_H_FIFO_STATUS;
        fifo_read_pulse_addr = REG_IR_FIFO_RD_PULSE;
        //fifo_data_addr = REG_IR_CKDIV_NUM_KEY_DATA;
        fifo_empty_flag = IR_FIFO_EMPTY;
    }
    else
    {
        fifo_status_addr = REG_IR_RC_KEY_FIFO_STATUS;
        fifo_read_pulse_addr = REG_IR_RC_FIFO_RD_PULSE;
        //fifo_data_addr = REG_IR_RC_KEY_COMMAND_ADD;
        fifo_empty_flag = IR_RC_FIFO_EMPTY;
    }
    // Empty the FIFO
    for(i=0; i<8; i++)
    {
        //U8 u8Garbage;

        if(REG(fifo_status_addr) & fifo_empty_flag)
            break;

        //u8Garbage = REG(fifo_data_addr) >> 8;
        REG(fifo_read_pulse_addr) |= 0x0001; //read
    }

}
#endif

#if (IR_MODE_SEL==IR_TYPE_SWDECODE_MODE)
#ifdef SUPPORT_MULTI_PROTOCOL
#if 0
inline static void _MDrv_MultiProtocol_RestartTimer(void)
{
    if(_s32TimerId!=-1)
    {
        MsOS_ResetTimer(_s32TimerId,
                         5,
                         0xFFFFFFFF,
                         TRUE);
        MsOS_StartTimer(_s32TimerId);
    }
}
#endif

inline static void _MDrv_MultiProtocol_StartTimer(void)
{
    DRV_IR_PROTOCOL_TYPE *pstProtocol;
    pstProtocol = _GetProtocolEntry(_eCurentProtocol);
    if(_s32TimerId!=-1)
    {
        MsOS_ResetTimer(_s32TimerId,
                         pstProtocol->u32Timeout/1000,
                         0xFFFFFFFF,
                         TRUE);
        MsOS_StartTimer(_s32TimerId);
    }
}

static void _MDrv_MultiProtocol_Cb(MS_U32 u32StTimer, MS_U32 u32TimerID)
{
    if(_u64ReleaseData==0||_u64ReleaseData&E_IR_KEY_STATE_RELEASE)
    {
        return;
    }

    _u64ReleaseData = ((_u64ReleaseData>>4)<<4)|E_IR_KEY_STATE_RELEASE;
    if(_pfIRRawModCallback64)
    {
        _pfIRRawModCallback64(_u64ReleaseData);
    }
}

static void _MDrv_MultiProtocol_Init(void)
{
    if(_s32TimerId==-1)
    {
        _s32TimerId = MsOS_CreateTimer(_MDrv_MultiProtocol_Cb, 0xFFFFFFFF, 0xFFFFFFFF, TRUE, "DrvIrTimer");
        if(_s32TimerId==-1)
        {
            IR_PRINT("Init Timer fail\n");
        }
    }
}

static IR_PROCOCOL_TYPE _MDrv_MultiProtocol_GetProByLeadCode(void)
{
    U8 u8Index;
    U8 u8StartIdx=0;
    BOOL bResult = FALSE;
    IR_PROCOCOL_TYPE eTemp=E_IR_PROTOCOL_NONE;
    DRV_IR_PROTOCOL_TYPE *pstProtocol;

    for(u8Index=0; u8Index<PROTOCOL_SUPPORT_MAX; u8Index++)
    {
        if(_eDetectList[u8Index]==E_IR_PROTOCOL_NONE)
            continue;

        pstProtocol = _GetProtocolEntry(_eDetectList[u8Index]);
        bResult = pstProtocol->findleadcode(&u8StartIdx);
        if(bResult==TRUE)
        {
            eTemp = _eDetectList[u8Index];
            if(u8Index!=0 && _eDetectList[0]!=E_IR_PROTOCOL_NONE)
            {//switch newest protocol to list head
                _eDetectList[u8Index] = _eDetectList[0];
                _eDetectList[0] = eTemp;
            }
            return eTemp;//return detect result
        }
    }

    return E_IR_PROTOCOL_NONE;
}

static IR_DECODE_STATUS _MDrv_MultiProtocol_ParseShotCount(IR_PROCOCOL_TYPE eProtocol, unsigned long long *pu64RawData)
{
    IR_DECODE_STATUS eResult = E_IR_DECODE_ERR;
    DRV_IR_PROTOCOL_TYPE *pstProtocol;
    U32 u32CustCode =0;
    U16 u16KeyCode = 0;
    U8 u8State = 0;
    U8 u8Reserved = 0;

    if(eProtocol>E_IR_PROTOCOL_NONE && eProtocol<E_IR_PROTOCOL_MAX)
    {
        pstProtocol = _GetProtocolEntry(eProtocol);
        eResult = pstProtocol->parseprotocol(&u32CustCode, &u16KeyCode, &u8State, &u8Reserved);
    }

    if(eResult ==E_IR_DECODE_DATA_OK)
    {
        *pu64RawData = (((unsigned long long)u8Reserved)<<56)|(((unsigned long long)u32CustCode)<<24) | (u16KeyCode<<8) | u8State;
    }
    return eResult;
}

static U8 _MDrv_MultiProtocol_GetMaxLeadCodeShot(void)
{
    U8 u8Max=0;
    U8 i;
    IR_PROCOCOL_TYPE eProtocol;
    DRV_IR_PROTOCOL_TYPE *pstProtocol;

    for(i=0; i<PROTOCOL_SUPPORT_MAX; i++)
    {
        eProtocol =_eDetectList[i];
        if(eProtocol!= E_IR_PROTOCOL_NONE)
        {
            pstProtocol = _GetProtocolEntry(eProtocol);
            if(pstProtocol->u8LeadCodeMinCount > u8Max)
            {
                u8Max =pstProtocol->u8LeadCodeMinCount;
            }
        }
    }

    return u8Max;
}
#endif


#if (defined(IR_TYPE_MSTAR_SWRC5) && IR_TYPE_SEL == IR_TYPE_MSTAR_SWRC5)
//-------------------------------------------------------------------------------------------------
/// Check if IR key is valid.
/// @return TRUE:  code is valid
/// @return FALSE: code is not valid
//-------------------------------------------------------------------------------------------------

static BOOL _MDrv_IR_SW_ChkRC5Code(void)
{
  if(((_u16CurKeyCode & 0x3000) != 0x3000) ||\
  	(((_u16CurKeyCode >> 6) & 0x1f) != IR_RC5_ADDR_CODE))
  {
     return FALSE;
  }

  if(((_u16CurKeyCode >> 11) & 0x1) == _u8PrevToggle)
  {
     _KeyReceived.u8Flag = 1;
  }
  else
  {
     _KeyReceived.u8Flag = 0;
  }
    _u8PrevToggle = (_u16CurKeyCode >> 11) & 0x1;
	_KeyReceived.u8System = 0;
	return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Get IR key.
/// @return TRUE:  Decode is done
/// @return FALSE: Decode is in progress or get abnormal pulse width
//-------------------------------------------------------------------------------------------------
static BOOL _MDrv_IR_SW_DecodeRC5(void)
{
    U32 u32IrCounter;
    U16 u16P_Shot;

    u32IrCounter = ((REG(REG_IR_SHOT_CNT_H_FIFO_STATUS)&0xF) << 16) | ((REG(REG_IR_SHOT_CNT_L))&0xFFFF);
    u16P_Shot = REG(REG_IR_SHOT_CNT_H_FIFO_STATUS) & IR_PSHOT_OCCURE;

    if(u32IrCounter < IR_RC5_GLITCH_CNT)//maybe glitch
        return FALSE;

    if(_bWaitKey && u16P_Shot && (u32IrCounter > 0x10000))
    {
      //printf(" === START ===\n");
      _bWaitKey = FALSE;
      _bGotBIT1stHalf = TRUE;
      _u16CurKeyCode = 0;
      _u8CurrentBitNum = 0;
      return FALSE;
    }

    // Pulse width is out of Spec.
    if(!_bWaitKey)
    {
      if((u32IrCounter < IR_RC5_MIN_PULSE_CNT) ||\
        (u32IrCounter > IR_RC5_MAX_BIT_CNT)   ||\
        ((u32IrCounter > IR_RC5_MAX_PULSE_CNT) && (u32IrCounter < IR_RC5_MIN_BIT_CNT)))
       {
        _bWaitKey = TRUE;
        _bGotBIT1stHalf = FALSE;
        _u16CurKeyCode = 0;
        _u8CurrentBitNum = 0;
      //printf("0x%x !!!!!!!\n",u32IrCounter);
       return FALSE;
       }
     }

    if(_bGotBIT1stHalf)
    {
      if(!u16P_Shot)//falling edge
      {
          //printf("BIT %d = 1\n",_u8CurrentBitNum);
           _u16CurKeyCode |= 0x1<<(IR_RC5_BITS - _u8CurrentBitNum - 1);
       }
       else
       {
          //printf("BIT %d = 0\n",_u8CurrentBitNum);
       }

       if(u32IrCounter < IR_RC5_MAX_PULSE_CNT)
       {
             _bGotBIT1stHalf = FALSE;
             if(_u8CurrentBitNum == 12)
             {
                  if(u16P_Shot)
                  {
                      _u8CurrentBitNum++;
                      //printf("Last BIT = 0\n");
                  }
             }
       }
       else
       {
            if(_u8CurrentBitNum == 12)
            {
                _u8CurrentBitNum++;
                if(u16P_Shot)
                {
                     _u16CurKeyCode |= 0x1<<(IR_RC5_BITS - _u8CurrentBitNum - 1);
                    //printf("Last BIT = 1\n");
                }
                else
                {
                   // printf("Last BIT = 0\n");
                 }
            }

       }
       _u8CurrentBitNum++;
    }
    else
    {
        _bGotBIT1stHalf = TRUE;
        return FALSE;
    }

    if(_u8CurrentBitNum>=(IR_RC5_BITS ))
    {
         //printf("reset\n");
        // printf("Decode Key = 0x%x\n", _u16CurKeyCode);
         _bWaitKey = TRUE;
         _bGotBIT1stHalf = FALSE;
         _u8CurrentBitNum = 0;
         if(_MDrv_IR_SW_ChkRC5Code())
         {
              _KeyReceived.u8Valid = 1;
              _KeyReceived.u8Key = _u16CurKeyCode & 0x3f;
            //printf("_KeyReceived.u8Key = 0x%x\n", _KeyReceived.u8Key);
             _u16CurKeyCode = 0;
             return TRUE;
         }
         else
         {
              _KeyReceived.u8Key = 0;
              _KeyReceived.u8Valid = 0;
         }
         _u16CurKeyCode = 0;
    }

    return FALSE;
}
#endif

#if (defined(IR_TYPE_MSTAR_SWRC6A) && IR_TYPE_SEL == IR_TYPE_MSTAR_SWRC6A)

#define RC6_MODE_LEN 4
#define RC6_KEY_LEN  32

typedef struct
{
    U8 u8PulseNum;
    U8 u8ElecLevel;
    MS_BOOL bLastKeyOK;
}S_IR_Key;

static MS_BOOL _bWaitKey = TRUE;
static MS_BOOL _bGetLeaderFirPart = FALSE;
static MS_BOOL _bGetLeaderSecPart = FALSE;
static MS_BOOL _bGetModeBit = FALSE;
static MS_BOOL _bGetToggleBit = FALSE;
static MS_BOOL _bGetKeyBit = FALSE;

static S_IR_Key    _lastKey;

static U8  _aModeKey[4];
static U8  _u8ModeOrder = 0;
static U32 _u32KeyValue = 0x0;
static U8  _u8KeyOrder = 0;

static U8  _u8PreviousKey = 0x0;
static U8  _u8PreRepeatFlag = 0x0;
#if 0
static U32 _u32IrCount[100];
static U16 _P_Shot[100];
static U8  _keyOrder = 0;
#endif

static U8 _PulseToDigital(U32 u32IrCounter)
{
    U8 u8Num = 0xFF;

    if (u32IrCounter > 2100 && u32IrCounter < 2900)
        u8Num = 6;
    else if (u32IrCounter > 1500 && u32IrCounter < 1900)
        u8Num = 4;
    else if (u32IrCounter > 1100 && u32IrCounter < 1400)
        u8Num = 3;
    else if (u32IrCounter > 700 && u32IrCounter < 950)
        u8Num = 2;
    else if (u32IrCounter > 300 && u32IrCounter < 550)
        u8Num = 1;
    else
        u8Num = 0;

    return u8Num;
}

//-------------------------------------------------------------------------------------------------
/// Get IR key.
/// @return TRUE:  Decode is done
/// @return FALSE: Decode is in progress or get abnormal pulse width
//-------------------------------------------------------------------------------------------------
static BOOL _MDrv_IR_SW_DecodeRC6A(void)
{
    U32 u32IrCounter;
    U16 u16P_Shot;
    U8 u8RepeatFlag;

    u32IrCounter = ((REG(REG_IR_SHOT_CNT_H_FIFO_STATUS) & 0xF) << 16) | ((REG(REG_IR_SHOT_CNT_L)) & 0xFFFF);
    u16P_Shot = REG(REG_IR_SHOT_CNT_H_FIFO_STATUS) & IR_PSHOT_OCCURE;

    if (u32IrCounter < IR_RC6_GLITCH_CNT) //maybe glitch
        return FALSE;

    if (u16P_Shot && (u32IrCounter > 20000))
    {
        //printf("%s %d === START ===\n", __FUNCTION__, __LINE__);
        _bWaitKey = FALSE;
        _bGetLeaderFirPart = FALSE;
        _bGetLeaderSecPart = FALSE;
        _bGetToggleBit = FALSE;
        _bGetModeBit = FALSE;
        _bGetKeyBit = FALSE;

        _lastKey.bLastKeyOK = FALSE;
        _lastKey.u8ElecLevel = 0;
        _lastKey.u8PulseNum = 0;

        _u8ModeOrder = 0;
        _u8KeyOrder = 0;
        _u32KeyValue = 0x0;

        return FALSE;
    }

    if ((!_bWaitKey) && (!_bGetLeaderFirPart))
    {
        if (!u16P_Shot && (u32IrCounter > 2100 && u32IrCounter < 2900))
        {
            _bGetLeaderFirPart = TRUE;
            _bGetLeaderSecPart = FALSE;
            _bGetToggleBit = FALSE;
        }
        else
        {
           // printf("%s %d Get error leader data. u32IrCounter:%u!\n",
           //     __FUNCTION__, __LINE__, u32IrCounter);
            _bWaitKey = TRUE;
        }
        return FALSE;
    }

    if ((!_bWaitKey) && (_bGetLeaderFirPart) && (!_bGetLeaderSecPart))
    {
        if (u16P_Shot && (u32IrCounter > 700 && u32IrCounter < 950))
        {
            _bGetLeaderSecPart = TRUE;
        }
        else
        {
           // printf("%s %d Get error leader data. u32IrCounter:%u!\n",
          //      __FUNCTION__, __LINE__, u32IrCounter);
            _bWaitKey = TRUE;
        }
        return FALSE;
    }

    if ((!_bWaitKey) && (_bGetLeaderSecPart)&& (!_bGetModeBit))
    {
        if (FALSE == _lastKey.bLastKeyOK)
        {
            _lastKey.u8PulseNum = _PulseToDigital(u32IrCounter);
            _lastKey.u8ElecLevel = u16P_Shot;
            _lastKey.bLastKeyOK = TRUE;
        }
        else
        {
            if ((_lastKey.u8ElecLevel) && (!u16P_Shot))
            {
                _aModeKey[_u8ModeOrder] = 0;
            }
            else
            {
                _aModeKey[_u8ModeOrder] = 1;
            }
            _u8ModeOrder++;

            if (RC6_MODE_LEN == _u8ModeOrder)
            {
                _lastKey.bLastKeyOK = FALSE;
                _u8ModeOrder = 0;
                _bGetModeBit = TRUE;
               // printf("%s %d %u%u%u%u\n", __FUNCTION__, __LINE__, _aModeKey[0],
                //    _aModeKey[1], _aModeKey[2], _aModeKey[3]);

                return FALSE;
            }

            if (_PulseToDigital(u32IrCounter) >= 2)
            {
                _lastKey.u8ElecLevel = u16P_Shot;
                _lastKey.u8PulseNum = _PulseToDigital(u32IrCounter);
                _lastKey.bLastKeyOK = TRUE;
            }
            else
                _lastKey.bLastKeyOK = FALSE;
        }
        return FALSE;
    }

    if ((!_bWaitKey) && _bGetModeBit &&(!_bGetToggleBit))
    {
        if (FALSE == _lastKey.bLastKeyOK)
        {
            _lastKey.u8ElecLevel = u16P_Shot;
            _lastKey.u8PulseNum = _PulseToDigital(u32IrCounter);
            _lastKey.bLastKeyOK = TRUE;
        }
        else
        {
            if ((_lastKey.u8ElecLevel) && (!u16P_Shot))
            {
                _aModeKey[_u8ModeOrder] = 0;
            }
            else
            {
                _aModeKey[_u8ModeOrder] = 1;
            }

            if (3 == (_PulseToDigital(u32IrCounter)))
            {
                _lastKey.u8ElecLevel = u16P_Shot;
                _lastKey.u8PulseNum = _PulseToDigital(u32IrCounter);
                _lastKey.bLastKeyOK = TRUE;
            }
            else
                _lastKey.bLastKeyOK = FALSE;

            _bGetToggleBit = TRUE;
        }
        return FALSE;
    }

    if ((!_bWaitKey) && _bGetToggleBit && (!_bGetKeyBit))
    {
        if (FALSE == _lastKey.bLastKeyOK)
        {
            if ((((RC6_KEY_LEN - 1) == _u8KeyOrder))
                && 1 == _PulseToDigital(u32IrCounter))
            {
                if (!u16P_Shot)
                    _u32KeyValue |= (0x1) << (RC6_KEY_LEN - _u8KeyOrder - 1);

                _lastKey.bLastKeyOK = FALSE;
                _u8KeyOrder = 0;
                _bGetKeyBit = TRUE;
                _bWaitKey = TRUE;

                _KeyReceived.u8Key = _u32KeyValue & 0xFF;

                u8RepeatFlag = (_u32KeyValue >> 15) & 0x1;

                if ((u8RepeatFlag == _u8PreRepeatFlag)
                    && (_KeyReceived.u8Key == _u8PreviousKey))
                    _KeyReceived.u8Flag = 1;
                else
                    _KeyReceived.u8Flag = 0;

                _u8PreRepeatFlag = u8RepeatFlag;
                _u8PreviousKey = _KeyReceived.u8Key;

               // printf("%s %d the key:0x%x \n", __FUNCTION__, __LINE__, _u32KeyValue);

                return TRUE;
            }
            else
            {
                _lastKey.u8ElecLevel = u16P_Shot;
                _lastKey.u8PulseNum = _PulseToDigital(u32IrCounter);
                _lastKey.bLastKeyOK = TRUE;
            }
        }
        else
        {
            if ((_lastKey.u8ElecLevel) && (!u16P_Shot))
            {
                ;//_aButtonKey[_u8KeyOrder] = 0;
            }
            else
            {
                _u32KeyValue |= (0x1) << (RC6_KEY_LEN - _u8KeyOrder - 1);
            }
            _u8KeyOrder++;

            if (RC6_KEY_LEN == _u8KeyOrder)
            {
                _lastKey.bLastKeyOK = FALSE;
                _u8KeyOrder = 0;
                _bGetKeyBit = TRUE;
                _bWaitKey = TRUE;

                _KeyReceived.u8Key = _u32KeyValue & 0xFF;

                u8RepeatFlag = (_u32KeyValue >> 15) & 0x1;

                if ((u8RepeatFlag == _u8PreRepeatFlag)
                    && (_KeyReceived.u8Key == _u8PreviousKey))
                    _KeyReceived.u8Flag = 1;
                else
                    _KeyReceived.u8Flag = 0;

                _u8PreRepeatFlag = u8RepeatFlag;
                _u8PreviousKey = _KeyReceived.u8Key;

               // printf("%s %d the key:0x%x \n", __FUNCTION__, __LINE__, _u32KeyValue);

                return TRUE;
            }
            else if (((RC6_KEY_LEN - 1) == _u8KeyOrder)
                && (2 == _PulseToDigital(u32IrCounter)))
            {
                if (!u16P_Shot)
                   _u32KeyValue |= (0x1) << (RC6_KEY_LEN - _u8KeyOrder - 1);

                _lastKey.bLastKeyOK = FALSE;
                _u8KeyOrder = 0;
                _bGetKeyBit = TRUE;
                _bWaitKey = TRUE;

                _KeyReceived.u8Key = _u32KeyValue & 0xFF;

                u8RepeatFlag = (_u32KeyValue >> 15) & 0x1;

                if ((u8RepeatFlag == _u8PreRepeatFlag)
                    && (_KeyReceived.u8Key == _u8PreviousKey))
                    _KeyReceived.u8Flag = 1;
                else
                    _KeyReceived.u8Flag = 0;

                _u8PreRepeatFlag = u8RepeatFlag;
                _u8PreviousKey = _KeyReceived.u8Key;

               // printf("%s %d the key:0x%x \n", __FUNCTION__, __LINE__, _u32KeyValue);

                return TRUE;
            }
            else
            {
                if (2 == _PulseToDigital(u32IrCounter))
                {
                    _lastKey.u8ElecLevel = u16P_Shot;
                    _lastKey.u8PulseNum = _PulseToDigital(u32IrCounter);
                    _lastKey.bLastKeyOK = TRUE;
                }
                else
                    _lastKey.bLastKeyOK = FALSE;
            }

            return FALSE;
        }
        return FALSE;
    }

    return FALSE;
}

#endif

#endif

//-------------------------------------------------------------------------------------------------
/// Get IR key. It is a non-blocking function.
/// @param pu8Key  \b IN: Return IR key value.
/// @param pu8Flag \b IN: Return IR repeat code.
///
/// @return TRUE:  Success
/// @return FALSE: No key or repeat key is faster than the specified period
//-------------------------------------------------------------------------------------------------

#if((IR_MODE_SEL==IR_TYPE_FULLDECODE_MODE))
static BOOL _MDrv_IR_GetKey(U8 *pu8Key, U8 *pu8System, U8 *pu8Flag)
{
    unsigned long i;
    BOOL bRet=FALSE;
    *pu8System = 0;

    if(REG(REG_IR_SHOT_CNT_H_FIFO_STATUS) & IR_FIFO_EMPTY)
    {
        _bCheckQuickRepeat = 0;
        return FALSE;
    }


    if(((MsOS_GetSystemTime()- _ulPrevKeyTime) >= IR_TIMEOUT_CYC/1000))
    {
        *pu8Key = REG(REG_IR_CKDIV_NUM_KEY_DATA) >> 8;
        REG(REG_IR_FIFO_RD_PULSE) |= 0x0001; //read
        for(i=0;i<5;i++);   // Delay
        _u8PrevKeyCode = *pu8Key;
        *pu8Flag = 0;
        _ulPrevKeyTime = MsOS_GetSystemTime();
        _ePrevKeyProperty = E_IR_KEY_PROPERTY_INIT;
        _bCheckQuickRepeat = 0;
        _MDrv_IR_ClearFIFO();
        return TRUE;
    }
    else
    {
        if(_bCheckQuickRepeat==0)
        {
           _bCheckQuickRepeat = 1;
            _ulPrevKeyTime = MsOS_GetSystemTime();
            _MDrv_IR_ClearFIFO();
            return FALSE;
        }
        *pu8Key = REG(REG_IR_CKDIV_NUM_KEY_DATA) >> 8;
        REG(REG_IR_FIFO_RD_PULSE) |= 0x0001; //read
        for(i=0;i<5;i++);   // Delay
        *pu8Flag = (REG(REG_IR_SHOT_CNT_H_FIFO_STATUS) & IR_RPT_FLAG)? 1 : 0;
        bRet = FALSE;
        _ulPrevKeyTime = MsOS_GetSystemTime();

        if ( (*pu8Flag == 1) && ( *pu8Key == _u8PrevKeyCode ))
        {
            i = MsOS_GetSystemTime();
            if( _ePrevKeyProperty == E_IR_KEY_PROPERTY_INIT)
            {
                _u8PrevKeyCode     = *pu8Key;
                _ulPrevKeyRepeatTime    = i;
                _ePrevKeyProperty  = E_IR_KEY_PROPERTY_1st;
            }
            else if(_ePrevKeyProperty == E_IR_KEY_PROPERTY_1st)
            {
                if( (i - _ulPrevKeyRepeatTime) > _u32_1stDelayTimeMs)
                {
                    _ulPrevKeyRepeatTime = i;
                    _ePrevKeyProperty  = E_IR_KEY_PROPERTY_FOLLOWING;
                    bRet = TRUE;
                }
            }
            else //E_IR_KEY_PROPERTY_FOLLOWING
            {
                if( (i - _ulPrevKeyRepeatTime) > _u32_2ndDelayTimeMs)
                {
                    _ulPrevKeyRepeatTime = i;
                    bRet = TRUE;
                }
            }
        }
    }

    // Empty the FIFO
    _MDrv_IR_ClearFIFO();
    return bRet;

}
#elif ((IR_MODE_SEL==IR_TYPE_HWRC_MODE))
//-------------------------------------------------------------------------------------------------
/// Get IR key. It is a non-blocking function.
/// @param pu8Key  \b IN: Return IR key value.
/// @param pu8Flag \b IN: Return IR repeat code.
///
/// @return TRUE:  Success
/// @return FALSE: No key
//-------------------------------------------------------------------------------------------------

static BOOL _MDrv_IR_GetKey(U8 *pu8Key, U8 *pu8System, U8 *pu8Flag)
{
    U8 u8KeyAddr,u8KeyCmd;
    BOOL bRet=FALSE;
    *pu8System = 0;

    if(REG(REG_IR_RC_KEY_FIFO_STATUS) & IR_RC_FIFO_EMPTY)
    {
        return FALSE;
    }

    u8KeyAddr = REG(REG_IR_RC_KEY_COMMAND_ADD)&0x3f;//RC5: {2'b0,toggle,address[4:0]} reg[7:0]
    u8KeyCmd = (REG(REG_IR_RC_KEY_COMMAND_ADD)&0x3f00)>>8;//RC5: {repeat,1'b0,command[13:8]} reg[15:8]
    *pu8Flag = (REG(REG_IR_RC_KEY_COMMAND_ADD)&0x8000)>>15;//repeat
    *pu8Key = u8KeyCmd;

	REG(REG_IR_RC_FIFO_RD_PULSE)|=0x1;

	bRet=TRUE;
    // Empty the FIFO
    _MDrv_IR_ClearFIFO();
    return bRet;
}
#elif ((IR_MODE_SEL==IR_TYPE_SWDECODE_MODE))
#ifdef SUPPORT_MULTI_PROTOCOL
#else
//-------------------------------------------------------------------------------------------------
/// Get IR key. It is a non-blocking function.
/// @param pu8Key  \b IN: Return IR key value.
/// @param pu8Flag \b IN: Return IR repeat code.
///
/// @return TRUE:  Success
/// @return FALSE: No key
//-------------------------------------------------------------------------------------------------

static BOOL _MDrv_IR_GetKey(U8 *pu8Key, U8 *pu8System, U8 *pu8Flag)
{
    BOOL bRet = FALSE;
#if (defined(IR_TYPE_MSTAR_SWRC5) && IR_TYPE_SEL == IR_TYPE_MSTAR_SWRC5)
    if (_MDrv_IR_SW_DecodeRC5())
    {
        *pu8Key = _KeyReceived.u8Key;
        *pu8System = 0;
        *pu8Flag = _KeyReceived.u8Flag;
        bRet = TRUE;
    }
#endif
#if (defined(IR_TYPE_MSTAR_SWRC6A) && IR_TYPE_SEL == IR_TYPE_MSTAR_SWRC6A)
    if (_MDrv_IR_SW_DecodeRC6A())
    {
        *pu8Key = _KeyReceived.u8Key;
        *pu8System = 0;
        *pu8Flag = _KeyReceived.u8Flag;
        //printf("%s %d 0x%x, 0x%x;\n", __FUNCTION__, __LINE__, *pu8Key, *pu8Flag);
        bRet = TRUE;
    }
#endif


    return bRet;
}
#endif
#elif ((IR_MODE_SEL==IR_TYPE_RAWDATA_MODE))
static BOOL _MDrv_IR_GetKey(U8 *pu8Key, U8 *pu8System, U8 *pu8Flag)
{
    BOOL bRet = FALSE;

    MS_U32 i, j;
    *pu8System = 0;

    if ((flag_repeat==TRUE) && ((REG(REG_IR_SHOT_CNT_H_FIFO_STATUS)&IR_RPT_FLAG) == IR_RPT_FLAG))
    {
         *pu8Key  = prev_key_rpt;
         *pu8Flag = 1;
         bRet = TRUE;
         goto  Raw_Repeat;
   }

    flag_repeat=TRUE;

    for (j=0; j<IR_RAW_DATA_NUM; j++)
    {
        if ( REG(REG_IR_SHOT_CNT_H_FIFO_STATUS) & IR_FIFO_EMPTY)  // check FIFO empty
            break;

        _u8IRRawModeBuf[_u8IRRawModeCount++] = REG(REG_IR_CKDIV_NUM_KEY_DATA) >> 8;
        REG(REG_IR_FIFO_RD_PULSE) |= 0x0001; //read

       for(i=0;i<5;i++); //Delay

        if(_u8IRRawModeCount == IR_RAW_DATA_NUM)
        {
            _u8IRRawModeCount = 0;
            if(_u8IRRawModeBuf[2] == (U8)(~_u8IRRawModeBuf[3]))
            {
                *pu8Key = _u8IRRawModeBuf[2];
                *pu8Flag = 0;
                 bRet = TRUE;
                 flag_repeat = FALSE;
                 prev_key_rpt = *pu8Key;
                 for(i=0;i<IR_RAW_DATA_NUM;i++)
                 {
                     _u8IRGetRawData[i] = _u8IRRawModeBuf[i];
                 }
                 break;
            }
        }
    }

Raw_Repeat:
    // Empty the FIFO
    //_MDrv_IR_ClearFIFO();
    REG(REG_IR_SEPR_BIT_FIFO_CTRL) |= (0x1 <<15);

    return bRet;
}


#endif

//-------------------------------------------------------------------------------------------------
/// ISR when receive IR key.
/// @return None
//-------------------------------------------------------------------------------------------------

void _MDrv_IR_ISR(InterruptNum irq)
//irqreturn_t _MDrv_IR_ISR(int irq, void *dev_id, struct pt_regs *regs)
{
#ifdef SUPPORT_MULTI_PROTOCOL
    U32 u32ShotValue;
    BOOL bNegShot;
    unsigned long long u64Data=0;
    DRV_IR_PROTOCOL_TYPE *pstProtocol;

    pstProtocol = _GetProtocolEntry(_eCurentProtocol);
    if (MsOS_GetSystemTime() - _ulPreShotTime> pstProtocol->u32Timeout/1000)
    {
        _Mdrv_MulProtCommon_ShotDataReset();
        _eCurentProtocol=E_IR_PROTOCOL_NONE;
        _eLastKeyProtocol= E_IR_PROTOCOL_NONE;
        _u64LastData=0;
    }

    u32ShotValue = ((REG(REG_IR_SHOT_CNT_H_FIFO_STATUS)&0xF) << 16) | ((REG(REG_IR_SHOT_CNT_L))&0xFFFF);
    bNegShot = (REG(REG_IR_SHOT_CNT_H_FIFO_STATUS))&0x10?FALSE:TRUE;
    if(_MulProtCommon_GetShotDataSize()!=0)
    {
        _Mdrv_MulProtCommon_AddShot(u32ShotValue, bNegShot);
    }
    else
    {
        if(bNegShot==FALSE)//First shot must be p-shot
        {
            _Mdrv_MulProtCommon_AddShot(u32ShotValue, bNegShot);
        }
    }

    if(_eCurentProtocol== E_IR_PROTOCOL_NONE &&
        _MulProtCommon_GetShotDataSize() >= _MDrv_MultiProtocol_GetMaxLeadCodeShot())
    {
        _eCurentProtocol = _MDrv_MultiProtocol_GetProByLeadCode();
        if(_eCurentProtocol==E_IR_PROTOCOL_NONE)//fail
        {
            //IR_PRINT("parse lead code fail\n");
            //_MulProtCommon_dumpShotData();
            _Mdrv_MulProtCommon_ShotDataReset();//reset all data
        }
    }

    if(_eCurentProtocol!=E_IR_PROTOCOL_NONE)
    {
        IR_DECODE_STATUS eResult = _MDrv_MultiProtocol_ParseShotCount(_eCurentProtocol, &u64Data);
        if(eResult==E_IR_DECODE_ERR)
        {
            //IR_PRINT("parse error, _eCurentProtocol=%d\n", _eCurentProtocol);
            //_MulProtCommon_dumpShotData();
            _eLastKeyProtocol=E_IR_PROTOCOL_NONE;
            _Mdrv_MulProtCommon_ShotDataReset();
        }
        else if(eResult == E_IR_DECODE_DATA_SHORTAGE)
        {
            //continue to get next shot
        }
        else if(eResult == E_IR_DECODE_DATA_OK)
        {
            //IR_PRINT("parse shot count ok, u64RawData=0x%016llx\n", u64Data);
            //_MulProtCommon_dumpShotData();
            _Mdrv_MulProtCommon_ShotDataReset();
            _eLastKeyProtocol= _eCurentProtocol;
            _u64LastData = u64Data;
            _u64ReleaseData = u64Data;
            if(_pfIRRawModCallback64)
            {
                _pfIRRawModCallback64(u64Data);
            }
            _MDrv_MultiProtocol_StartTimer();
        }
    }

    _ulPreShotTime = MsOS_GetSystemTime();
    MsOS_EnableInterrupt(E_INT_FIQ_IR);
    return;
#else
    U8 u8Key=0, u8RepeatFlag=0;
    U8 u8System = 0;

#if((IR_MODE_SEL==IR_TYPE_SWDECODE_MODE))
	 if(_MDrv_IR_GetKey(&u8Key, &u8System, &u8RepeatFlag) && _pfIRCallback)
	 {
          _pfIRCallback(u8Key, u8RepeatFlag);
	 }
#else
    IR_PRINT("\n");

    if (_MDrv_IR_GetKey(&u8Key, &u8System, &u8RepeatFlag))
    {
#if((IR_MODE_SEL==IR_TYPE_FULLDECODE_MODE))
        _ulLastKeyPresentTime = MsOS_GetSystemTime();
#endif
        IR_PRINT("_MDrv_IR_ISR() -> %x, %x\n", u8Key, u8RepeatFlag);

        //temporary solution, need to implement ring buffer for this
        _KeyReceived.u8Key = u8Key;
        _KeyReceived.u8System = u8System;
        _KeyReceived.u8Flag = u8RepeatFlag;
        _KeyReceived.u8Valid = 1;

        if(_pfIRCallback)
        {
            // FIXME: HAVE to create another task? 100728
            _pfIRCallback(u8Key, u8RepeatFlag);
        }

#if((IR_MODE_SEL==IR_TYPE_RAWDATA_MODE))
        if(_pfIRRawModCallback)
        {
            _pfIRRawModCallback(_u8IRGetRawData, u8RepeatFlag);
        }
#endif
    }
#endif //#if((IR_MODE_SEL==IR_TYPE_SWDECODE_MODE))

    //Disabled already by SA_INTERRUPT at initial
#if((IR_MODE_SEL==IR_TYPE_HWRC_MODE))
    MsOS_EnableInterrupt(E_INT_FIQ_IR_INT_RC);
#else
    MsOS_EnableInterrupt(E_INT_FIQ_IR);
#endif

    return;
#endif
}



//-------------------------------------------------------------------------------------------------
/// Set the timing of IrDa at BOOT stage.
/// @return None
//-------------------------------------------------------------------------------------------------
static void _MDrv_IR_Timing(void)
{
    // header code upper bound
    REG(REG_IR_HDC_UPB) = IR_HDC_UPB;

    // header code lower bound
    REG(REG_IR_HDC_LOB) = IR_HDC_LOB;

    // off code upper bound
    REG(REG_IR_OFC_UPB) = IR_OFC_UPB;

    // off code lower bound
    REG(REG_IR_OFC_LOB) = IR_OFC_LOB;

    // off code repeat upper bound
    REG(REG_IR_OFC_RP_UPB) = IR_OFC_RP_UPB;

    // off code repeat lower bound
    REG(REG_IR_OFC_RP_LOB) = IR_OFC_RP_LOB;

    // logical 0/1 high upper bound
    REG(REG_IR_LG01H_UPB) = IR_LG01H_UPB;

    // logical 0/1 high lower bound
    REG(REG_IR_LG01H_LOB) = IR_LG01H_LOB;

    // logical 0 upper bound
    REG(REG_IR_LG0_UPB) = IR_LG0_UPB;

    // logical 0 lower bound
    REG(REG_IR_LG0_LOB) = IR_LG0_LOB;

    // logical 1 upper bound
    REG(REG_IR_LG1_UPB) = IR_LG1_UPB;

    // logical 1 lower bound
    REG(REG_IR_LG1_LOB) = IR_LG1_LOB;

    // timeout cycles
    REG(REG_IR_TIMEOUT_CYC_L) = IR_RP_TIMEOUT & 0xFFFF;
    //set up ccode bytes and code bytes/bits num
    REG(REG_IR_TIMEOUT_CYC_H_CODE_BYTE) = IR_CCB_CB | 0x30 | ((IR_RP_TIMEOUT >> 16) & 0x0F);

#ifdef SUPPORT_MULTI_PROTOCOL
    REG(REG_IR_CKDIV_NUM_KEY_DATA) = IR_CKDIV_NUM-1;   // clock divider
#else
    REG(REG_IR_CKDIV_NUM_KEY_DATA) = IR_CKDIV_NUM;   // clock divider
#endif

	#if(IR_MODE_SEL == IR_TYPE_HWRC_MODE)
	REG(REG_IR_RC_LONGPULSE_THR) = IR_RC_LONGPULSE_THR;
	REG(REG_IR_RC_LONGPULSE_MAR) = IR_RC_LONGPULSE_MAR;
	REG(REG_IR_RC_CLK_INT_THR) = (IR_RC_INTG_THR(IR_RC_INTG_THR_TIME)<<1) + (IR_RC_CLK_DIV(IR_CLK)<<8) + IR_RC6_ECO_EN;
	REG(REG_IR_RC_WD_TIMEOUT_CNT) = IR_RC_WDOG_CNT(IR_RC_WDOG_TIME) + (IR_RC_TMOUT_CNT(IR_RC_TIMEOUT_TIME)<<8);
	REG(REG_IR_TIMEOUT_CYC_H_CODE_BYTE) = (IR_RC5_BITS<<8) | 0x30;//[6:4]=011 : timeout be clear at Key Data Code check pass
	#else
	//set up ccode bytes and code bytes/bits num
    REG(REG_IR_TIMEOUT_CYC_H_CODE_BYTE) = IR_CCB_CB | 0x30 | ((IR_RP_TIMEOUT >> 16) & 0x0F);
    #endif
}

//-------------------------------------------------------------------------------------------------
/// Get IR key. It is a non-blocking function.
/// @param pu8Key  \b IN: Return IR key value.
/// @param pu8Flag \b IN: Return IR repeat code.
///
/// @return TRUE:  Success
/// @return FALSE: No key.
//-------------------------------------------------------------------------------------------------
//BOOL MDrv_IR_GetKey(U8 *pu8Key, U8 *pu8System, U8 *pu8Flag)
IR_Result MDrv_IR_HK_GetKeyCode(MS_U8 *pu8Key, MS_U8 *pu8Repeat)
{
    //eCos
    #if 0
    //u32 u32OldIntr;
    //MsOS_DiableAllInterrupts(u32OldIntr); //need ???

    //bRet = _MDrv_IR_GetKey(pu8Key, pu8Flag);

    //MsOS_RestoreAllInterrupts(u32OldIntr);
    #endif

    if (_KeyReceived.u8Valid)
    {
        *pu8Key = _KeyReceived.u8Key;
//        *pu8System = _KeyReceived.u8System;
        *pu8Repeat = _KeyReceived.u8Flag ;

        //memset(&_KeyReceived, 0 , sizeof(_KeyReceived) );

        return E_IR_OK;
    }
    else
    {
        return E_IR_FAIL;
    }
}
//-------------------------------------------------------------------------------------------------
/// Set IR callback function when receive IR key. Support only one callback. If call it twice,
/// the first callback does not be called.
/// Note: The callback runs at interrupt mode.
/// @param pCallback \b IN: Set the callback function when generate IR interrupt.
/// @return E_IR_OK: Success
/// @return E_IR_FAIL or other values: Failure
//-------------------------------------------------------------------------------------------------
IR_Result MDrv_IR_HK_SetCallback(IR_Callback pCallback)
{

    //(1) set local callback
    _pfIRCallback = pCallback;

    return E_IR_OK;
}

//-------------------------------------------------------------------------------------------------
/// Set IR callback function when receive IR key. Support only one callback. If call it twice,
/// the first callback does not be called.
/// Note: The callback runs at interrupt mode.
/// @param pCallback \b IN: Set the callback function when generate IR interrupt.
/// @return E_IR_OK: Success
/// @return E_IR_FAIL or other values: Failure
//-------------------------------------------------------------------------------------------------
IR_Result MDrv_IR_HK_SetRawModCallback(IR_RawModCallback pRawModCallback)
{

#if (IR_MODE_SEL == IR_TYPE_RAWDATA_MODE)
    _pfIRRawModCallback = pRawModCallback;
    return E_IR_OK;
#else
    return E_IR_FAIL;
#endif
}

//-------------------------------------------------------------------------------------------------
/// Get IR callback function which receive IR key.
/// @return the callback function when generate IR interrupt
//-------------------------------------------------------------------------------------------------
IR_Callback MDrv_IR_HK_GetCallback(void)
{
    return _pfIRCallback;
}

//-------------------------------------------------------------------------------------------------
/// Get IR callback function which receive IR key.
/// @return the callback function when generate IR interrupt
//-------------------------------------------------------------------------------------------------
IR_RawModCallback MDrv_IR_HK_GetRowModCallback(void)
{
    return _pfIRRawModCallback;
}

//-------------------------------------------------------------------------------------------------
/// Initialize IR timing and enable IR.
/// @return None
//-------------------------------------------------------------------------------------------------
void MDrv_IR_HK_Init(void)
{
    int iResult;

#ifdef SUPPORT_MULTI_PROTOCOL
        _MDrv_MultiProtocol_Init();
#endif

    if(IR_MODE_SEL == IR_TYPE_RAWDATA_MODE)
    {
            REG(REG_IR_CTRL) = IR_TIMEOUT_CHK_EN |
                           IR_INV           |
                           IR_RPCODE_EN     |
                           IR_LG01H_CHK_EN  |
                           IR_LDCCHK_EN     |
                           IR_EN;
    }
    else if(IR_MODE_SEL == IR_TYPE_HWRC_MODE)
    {
     REG(REG_IR_CTRL) = IR_INV;
#if (defined(IR_TYPE_MSTAR_HWRC5) && IR_TYPE_SEL==IR_TYPE_MSTAR_HWRC5)
     REG(REG_IR_RC_CTRL) = IR_RC_EN;
#endif
    }
    else
    {
      REG(REG_IR_CTRL) = IR_TIMEOUT_CHK_EN |
          IR_INV           |
          IR_RPCODE_EN     |
          IR_LG01H_CHK_EN  |
          IR_DCODE_PCHK_EN |
          IR_CCODE_CHK_EN  |
          IR_LDCCHK_EN     |
          IR_EN;
     }

    _MDrv_IR_Timing();

#if ((IR_MODE_SEL == IR_TYPE_RAWDATA_MODE) ||(IR_MODE_SEL==IR_TYPE_FULLDECODE_MODE))
    {
        REG(REG_IR_CCODE) = ((MS_U16)IR_HEADER_CODE1<<8) | IR_HEADER_CODE0;
        REG(REG_IR_GLHRM_NUM) = 0x804;
    }
#endif

    REG(REG_IR_SEPR_BIT_FIFO_CTRL) = 0xF00;

#if (IR_MODE_SEL==IR_TYPE_FULLDECODE_MODE)
    REG(REG_IR_GLHRM_NUM) |= (0x3 <<12);
    REG(REG_IR_FIFO_RD_PULSE) |= 0x0020; //wakeup key sel
#elif (IR_MODE_SEL==IR_TYPE_RAWDATA_MODE)
        REG(REG_IR_GLHRM_NUM) |= (0x2 << 12);
        REG(REG_IR_FIFO_RD_PULSE) |= 0x0020; //wakeup key sel
#elif (IR_MODE_SEL == IR_TYPE_HWRC_MODE)
    //wakeup key sel
    REG(REG_IR_RC_COMP_KEY1_KEY2) = 0xffff;
    REG(REG_IR_RC_CMP_RCKEY) = IR_RC_POWER_WAKEUP_EN + IR_RC_POWER_WAKEUP_KEY;
#else
    REG(REG_IR_GLHRM_NUM) |= (0x1 <<12);//SW mode
	#ifdef IR_INT_NP_EDGE_TRIG	//for N/P edge trigger
	    REG(REG_IR_SEPR_BIT_FIFO_CTRL) |= 0x3 <<12;
    #else
	    REG(REG_IR_SEPR_BIT_FIFO_CTRL) |= 0x2 <<12;
	#endif
#endif


#if((IR_MODE_SEL==IR_TYPE_RAWDATA_MODE) || (IR_MODE_SEL==IR_TYPE_FULLDECODE_MODE) || (IR_MODE_SEL == IR_TYPE_HWRC_MODE))
    _MDrv_IR_ClearFIFO();
#endif

#if (IR_MODE_SEL==IR_TYPE_FULLDECODE_MODE)
    _u32_1stDelayTimeMs = 0;
    _u32_2ndDelayTimeMs = 0;
    _ePrevKeyProperty = E_IR_KEY_PROPERTY_INIT;
    memset(&_KeyReceived, 0 , sizeof(_KeyReceived));
#endif

#if (IR_MODE_SEL==IR_TYPE_HWRC_MODE)
    iResult = MsOS_AttachInterrupt(E_INT_FIQ_IR_INT_RC, _MDrv_IR_ISR);
#else
    iResult = MsOS_AttachInterrupt(E_INT_FIQ_IR, _MDrv_IR_ISR);
#endif
    if (iResult)
    {
        IR_PRINT("IR IRQ registartion OK\n");
	}
	else
    {
        IR_PRINT("IR IRQ registartion ERROR\n");
    }

#if (IR_MODE_SEL==IR_TYPE_HWRC_MODE)
    iResult = MsOS_EnableInterrupt(E_INT_FIQ_IR_INT_RC);
#else
    iResult = MsOS_EnableInterrupt(E_INT_FIQ_IR);
#endif


    if (iResult)
    {
        IR_PRINT("IR IRQ registartion OK\n");
	}
	else
    {
        IR_PRINT("IR IRQ registartion ERROR\n");
    }

#ifdef SUPPORT_MULTI_PROTOCOL
        memset(_eDetectList, 0, sizeof(_eDetectList));
        _eDetectList[0] = E_IR_PROTOCOL_NEC; //default used protocol
#endif

    #if (defined(CONFIG_MSTAR_TITANIA)||defined(CONFIG_MSTAR_TITANIA2))

    #else
    // unmask IR IRQ on PM
    REG(REG_IRQ_MASK_IR) &= IRQ_UNMASK_IR;
    #endif
    //enable_irq(E_FIQ_IR);
}

//-------------------------------------------------------------------------------------------------
/// Set IR enable function.
/// @param bEnable \b IN: TRUE: enable IR, FALSE: disable IR
/// @return E_IR_OK: Success
/// @return E_IR_FAIL or other values: Failure
//-------------------------------------------------------------------------------------------------
IR_Result MDrv_IR_HK_Enable(MS_BOOL bEnable)
{
    MS_BOOL bResult = FALSE;
#if (IR_MODE_SEL==IR_TYPE_HWRC_MODE)
    bResult = MsOS_DisableInterrupt(E_INT_FIQ_IR_INT_RC);
#else
    bResult = MsOS_DisableInterrupt(E_INT_FIQ_IR);
#endif
    if(bResult)
    {
        return E_IR_OK;
    }
    else
    {
        return E_IR_FAIL;
    }
}


//-------------------------------------------------------------------------------------------------
/// Set which protocol will be used in multiple protocol mode.
/// @param  pstProtocolCfg \b IN: carry with used protocol parameter.
/// @return TRUE/FALSE
//-------------------------------------------------------------------------------------------------
IR_Result MDrv_IR_HK_SetProtocol(MS_MultiProtocolCfg *pstProtocolCfg)
{
#ifdef SUPPORT_MULTI_PROTOCOL
    U8 u8Count=0;

    if(pstProtocolCfg==NULL)
    {
        return E_IR_FAIL;
    }

    memset(_eDetectList, 0, sizeof(_eDetectList));
    _eDetectList[u8Count]=pstProtocolCfg->eProtocol;
    u8Count++;
    while(pstProtocolCfg->pNextProtCfg!=NULL && u8Count<PROTOCOL_SUPPORT_MAX)
    {
        pstProtocolCfg = (MS_MultiProtocolCfg *)pstProtocolCfg->pNextProtCfg;
        _eDetectList[u8Count]=pstProtocolCfg->eProtocol;
        u8Count++;
    }

    return E_IR_OK;
#else
    IR_PRINT("Multiple protocol not support");
    return E_IR_FAIL;
#endif
}

IR_Result MDrv_IR_HK_SetRawModCallback64(IR_RawModCallback64 pRawModCallback)
{
#if defined(SUPPORT_MULTI_PROTOCOL)
    _pfIRRawModCallback64 = pRawModCallback;
    return E_IR_OK;
#else
    return E_IR_FAIL;
#endif
}

//-------------------------------------------------------------------------------------------------
/// Set the IR delay time to recognize a valid key.
/// @param  u32_1stDelayTimeMs \b IN: Set the delay time to get the 1st key.
/// @param  u32_2ndDelayTimeMs \b IN: Set the delay time to get the following keys.
/// @return None
//-------------------------------------------------------------------------------------------------
#ifdef DDI_MISC_INUSE
#if (IR_MODE_SEL==IR_TYPE_FULLDECODE_MODE)

void MDrv_IR_HK_SetDelayTime(MS_U32 u32_1stDelayTimeMs, MS_U32 u32_2ndDelayTimeMs)
{
    MS_U32 u32OldIntr;

    u32OldIntr = MsOS_DisableAllInterrupts();

    _u32_1stDelayTimeMs = u32_1stDelayTimeMs;
    _u32_2ndDelayTimeMs = u32_2ndDelayTimeMs;

    MsOS_RestoreAllInterrupts(u32OldIntr);
}
#endif
#endif

