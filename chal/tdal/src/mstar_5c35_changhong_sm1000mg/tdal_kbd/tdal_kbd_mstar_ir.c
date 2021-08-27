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
// (��MStar Confidential Information��) by the recipient.
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
#include "IR_MSTAR_DTV.h"
#include "Board.h"
#include "mdrv_ir.h"
#include "mhal_ir_reg.h"
//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------
#define REG(addr)                   (*(volatile MS_U32 *)(addr))
#define IR_PRINT(fmt, args...)     // printf("%04d %s " fmt, __LINE__, __FUNCTION__, ## args)

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------
typedef enum
{
    E_IR_KEY_PROPERTY_INIT,
    E_IR_KEY_PROPERTY_1st,
    E_IR_KEY_PROPERTY_FOLLOWING
} IRKeyProperty;

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
// Forward declaration
//--------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Local Variables
//-------------------------------------------------------------------------------------------------
static MS_U32  _u32_1stDelayTimeMs;
static MS_U32  _u32_2ndDelayTimeMs;
static IRKeyProperty _ePrevKeyProperty;
//static MS_U8   _u8PrevKeyCode;
static MS_IR_KeyInfo _KeyReceived;   //temporary solution
static unsigned long  _ulPrevKeyTime;
static unsigned long  _ulPrevKeyRepeatTime;
static BOOL  _bCheckQuickRepeat;
static unsigned long  _ulLastKeyPresentTime;
static IR_Callback _pfIRCallback = NULL;

#if (IR_MODE_SEL == IR_TYPE_SWDECODE_MODE)
//static MS_U8   _u8PrevSystemCode;
static MS_U32  _u32PrevIRKeyCode;
#define IR_SWDECODE_MODE_BUF_LEN        100
static MS_U32  _u32IRData[IR_SWDECODE_MODE_BUF_LEN];
static MS_U32 _u32IRCount;
#ifndef IR_RAW_DATA_NUM
#define IR_RAW_DATA_NUM         4
#endif
#endif

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
static void _MDrv_IR_ClearFIFO(void)
{
    unsigned long i;

    // Empty the FIFO
    for(i=0; i<8; i++)
    {
        U8 u8Garbage;

        if(REG(REG_IR_SHOT_CNT_H_FIFO_STATUS) & IR_FIFO_EMPTY)
            break;

        u8Garbage = REG(REG_IR_CKDIV_NUM_KEY_DATA) >> 8;
        REG(REG_IR_FIFO_RD_PULSE) |= 0x0001; //read
    }

}

//-------------------------------------------------------------------------------------------------
/// Get IR key. It is a non-blocking function.
/// @param pu8Key  \b IN: Return IR key value.
/// @param pu8Flag \b IN: Return IR repeat code.
///
/// @return TRUE:  Success
/// @return FALSE: No key or repeat key is faster than the specified period
//-------------------------------------------------------------------------------------------------

#if (IR_MODE_SEL == IR_TYPE_SWDECODE_MODE)
static MS_BOOL _MDrv_IR_GetKey(U32 *pu32IRkey, /*MS_U8 *pu8Key, MS_U8 *pu8System,*/ U8 *pu8Flag)
{
    static int count=0;
    MS_BOOL bRet = FALSE;
    MS_U32 u8Byte, u8Bit;
    MS_U8 u8IRSwModeBuf[IR_RAW_DATA_NUM];
    MS_U32 *pu32IRData = NULL;

    if (MsOS_GetSystemTime() - _ulPrevKeyTime > IR_TIMEOUT_CYC/1000)
    {
        _u32IRCount = 0;
        _u32PrevIRKeyCode = 0;
    }
//printf("_u32IRCount=%d\n", _u32IRCount);
    if (_u32IRCount <IR_SWDECODE_MODE_BUF_LEN)
    {
        _u32IRData[_u32IRCount++] = ((REG(REG_IR_SHOT_CNT_H_FIFO_STATUS) & 0xF) << 16) | REG(REG_IR_SHOT_CNT_L);
    }
    _ulPrevKeyTime = MsOS_GetSystemTime();

    for( u8Byte=0; u8Byte<IR_RAW_DATA_NUM; u8Byte++)
    {
       u8IRSwModeBuf[u8Byte] = 0;
    }
//    *pu8System = 0;
    if( _u32IRCount>1 && _u32IRData[_u32IRCount-1] > IR_HDC_UPB)
    {
        _u32IRData[0] = _u32IRData[_u32IRCount-1];
        _u32IRCount = 1;
    }
   	else if(_u32IRCount == 2) //check repeat
    {
        if( _u32IRData[0] > IR_HDC_LOB && _u32IRData[1] > IR_OFC_RP_LOB +IR_LG01H_LOB && _u32IRData[1] < IR_OFC_RP_UPB+IR_LG01H_UPB )
        {
           // *pu8Key = _u8PrevKeyCode;
		   //*pu8System = _u8PrevSystemCode;
			*pu32IRkey = _u32PrevIRKeyCode;		
            *pu8Flag = 1;
            _u32IRCount = 0;
            if( _ePrevKeyProperty == E_IR_KEY_PROPERTY_INIT)
            {
                _ePrevKeyProperty  = E_IR_KEY_PROPERTY_1st;
            }
            else if(_ePrevKeyProperty == E_IR_KEY_PROPERTY_1st)
            {
                if((_ulPrevKeyTime - _ulPrevKeyRepeatTime) > _u32_1stDelayTimeMs)
                {
                    _ulPrevKeyRepeatTime = _ulPrevKeyTime;
                    _ePrevKeyProperty  = E_IR_KEY_PROPERTY_FOLLOWING;
                    bRet = TRUE;
                }
            }
            else //E_IR_KEY_PROPERTY_FOLLOWING
            {
                if( (_ulPrevKeyTime - _ulPrevKeyRepeatTime) > _u32_2ndDelayTimeMs)
                {
                    _ulPrevKeyRepeatTime = _ulPrevKeyTime;
                    bRet = TRUE;
                }
            }
            return bRet;
        }
    }

    if (_u32IRCount< 2+IR_RAW_DATA_NUM*8)
        return FALSE; //not complete yet
    count++;

    if( _u32IRData[0] > IR_HDC_LOB && _u32IRData[1] > IR_OFC_LOB+IR_LG01H_LOB && _u32IRData[1] < REG_IR_OFC_UPB+IR_LG01H_UPB )
    {
        pu32IRData = &_u32IRData[2];
    }
    else if( _u32IRData[1] > IR_HDC_LOB && _u32IRData[2] > IR_OFC_LOB+IR_LG01H_LOB && _u32IRData[2] < REG_IR_OFC_UPB+IR_LG01H_UPB )
    {
        pu32IRData = &_u32IRData[3];
    }
    else
    {
        bRet = FALSE;
        goto done;
    }

    for( u8Byte=0; u8Byte<IR_RAW_DATA_NUM; u8Byte++)
    {
        for( u8Bit=0; u8Bit<8; u8Bit++)
        {
            MS_U32 u32BitLen = pu32IRData[u8Byte*8+u8Bit];
            u8IRSwModeBuf[u8Byte] >>= 1;

            if( u32BitLen > IR_LG0_LOB && u32BitLen < IR_LG0_UPB ) //0
            {
                u8IRSwModeBuf[u8Byte] |= 0x00;
            }
            else if (u32BitLen > IR_LG1_LOB && u32BitLen < IR_LG1_UPB) // 1
            {
                u8IRSwModeBuf[u8Byte] |= 0x80;
            }
            else
            {
                bRet = FALSE;
                goto done;
            }
        }
    }

//printf("u8IRSwModeBuf[0]=0x%x, u8IRSwModeBuf[1]=0x%x--\n", u8IRSwModeBuf[0], u8IRSwModeBuf[1]);
//printf("u8IRSwModeBuf[2]=0x%x, u8IRSwModeBuf[3]=0x%x--\n", u8IRSwModeBuf[2], u8IRSwModeBuf[3]);

    if((u8IRSwModeBuf[2] == (MS_U8)~u8IRSwModeBuf[3])& (u8IRSwModeBuf[0] == (MS_U8)~u8IRSwModeBuf[1])\
		||((u8IRSwModeBuf[2] == (MS_U8)~u8IRSwModeBuf[3])))
    {
        //*pu8Key = u8IRSwModeBuf[2];
		//*pu8System = u8IRSwModeBuf[0];
		*pu32IRkey = ((MS_U32)u8IRSwModeBuf[3]<<24) | ((MS_U32)u8IRSwModeBuf[2]<<16) 
			          | ((MS_U32)u8IRSwModeBuf[1]<<8) | ((MS_U32)u8IRSwModeBuf[0]);
        _ePrevKeyProperty = E_IR_KEY_PROPERTY_INIT;
        _bCheckQuickRepeat = 0;
        _ulPrevKeyRepeatTime = MsOS_GetSystemTime();
        //_u8PrevKeyCode = *pu8Key;
		//_u8PrevSystemCode = *pu8System;
		_u32PrevIRKeyCode = *pu32IRkey ;		
        *pu8Flag = 0;
        bRet = TRUE;
        goto done;
    }

    bRet = FALSE;

done:
    _u32IRCount = 0;
    return bRet;
}
#else
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
#endif
//-------------------------------------------------------------------------------------------------
/// ISR when receive IR key.
/// @return None
//-------------------------------------------------------------------------------------------------

void _MDrv_IR_ISR(InterruptNum irq)
//irqreturn_t _MDrv_IR_ISR(int irq, void *dev_id, struct pt_regs *regs)
{
    U8 u8RepeatFlag=0;
	U32 u32IRKey = 0;
    IR_PRINT("\n");

    if (_MDrv_IR_GetKey(&u32IRKey, &u8RepeatFlag))//(_MDrv_IR_GetKey(&u8Key, &u8System, &u8RepeatFlag))
    {
        _ulLastKeyPresentTime = MsOS_GetSystemTime();
       // IR_PRINT("_MDrv_IR_ISR() -> %x, %x\n", u8Key, u8RepeatFlag);

        //temporary solution, need to implement ring buffer for this
       // _KeyReceived.u8Key = u8Key;
       // _KeyReceived.u8System = u8System;u32IRKey
        _KeyReceived.u32IRKey = u32IRKey;
        _KeyReceived.u8Flag = u8RepeatFlag;
        _KeyReceived.u8Valid = 1;

        if(_pfIRCallback)
        {
            // FIXME: HAVE to create another task? 100728
            _pfIRCallback(u32IRKey,/*u8Key, u8System,*/ u8RepeatFlag);
        }
    }

    //Disabled already by SA_INTERRUPT at initial
    MsOS_EnableInterrupt(E_INT_FIQ_IR);


    return;
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

    REG(REG_IR_CKDIV_NUM_KEY_DATA) = IR_CKDIV_NUM;   // clock divider
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
IR_Result MDrv_IR_HK_GetKeyCode(MS_U32 *u32IRKey,/*MS_U8 *pu8Key,*/ MS_U8 *pu8Repeat)
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
       // *pu8Key = _KeyReceived.u8Key;
//        *pu8System = _KeyReceived.u8System;
		*u32IRKey =_KeyReceived.u32IRKey;
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
/// Get IR callback function which receive IR key.
/// @return the callback function when generate IR interrupt
//-------------------------------------------------------------------------------------------------
IR_Callback MDrv_IR_HK_GetCallback(void)
{
    return _pfIRCallback;
}

//-------------------------------------------------------------------------------------------------
/// Initialize IR timing and enable IR.
/// @return None
//-------------------------------------------------------------------------------------------------
void MDrv_IR_HK_Init(void)
{
	int iResult;
#if (IR_MODE_SEL == IR_TYPE_SWDECODE_MODE)
    _u32IRCount = 0;
#endif

    REG(REG_IR_CTRL) =  IR_TIMEOUT_CHK_EN |
                        IR_INV            |
                        IR_RPCODE_EN      |
                        IR_LG01H_CHK_EN   |
                        IR_DCODE_PCHK_EN  |
                        IR_CCODE_CHK_EN   |
                        IR_LDCCHK_EN      |
                        IR_EN;

    _MDrv_IR_Timing();

    REG(REG_IR_CCODE) = ((MS_U16)IR_HEADER_CODE1<<8) | IR_HEADER_CODE0;
    REG(REG_IR_SEPR_BIT_FIFO_CTRL) = 0xF00;
    REG(REG_IR_GLHRM_NUM) = 0x804;

#if (IR_MODE_SEL == IR_TYPE_SWDECODE_MODE)    
    REG(REG_IR_GLHRM_NUM) |= (0x1 <<12);
    REG(REG_IR_SEPR_BIT_FIFO_CTRL) |= 0x2 <<12;
#else
    REG(REG_IR_GLHRM_NUM) |= (0x3 <<12);
#endif
    REG(REG_IR_FIFO_RD_PULSE) |= 0x0020; //wakeup key sel

    _MDrv_IR_ClearFIFO();

    _u32_1stDelayTimeMs = 100;
    _u32_2ndDelayTimeMs = 100;
    _ePrevKeyProperty = E_IR_KEY_PROPERTY_INIT;

    memset(&_KeyReceived, 0 , sizeof(_KeyReceived));

    iResult = MsOS_AttachInterrupt(E_INT_FIQ_IR, _MDrv_IR_ISR);
    if (iResult)
    {
        IR_PRINT("IR IRQ registartion OK\n");
	}
	else
    {
        IR_PRINT("IR IRQ registartion ERROR\n");
    }

    iResult = MsOS_EnableInterrupt(E_INT_FIQ_IR);
    if (iResult)
    {
        IR_PRINT("IR IRQ registartion OK\n");
	}
	else
    {
        IR_PRINT("IR IRQ registartion ERROR\n");
    }


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
    bResult = MsOS_DisableInterrupt(E_INT_FIQ_IR);
    if(bResult)
    {
        return E_IR_OK;
    }
    else
    {
        return E_IR_FAIL;
    }
}




