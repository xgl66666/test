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
#include "Board.h"
#include "string.h"


#if (IR_TYPE_SEL== IR_TYPE_XMP)
#include "IR_CUS_XMP.h"

#include "mdrv_ir.h"
#include "mhal_ir_reg.h"
//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------
#define REG(addr)                   (*(volatile MS_U32 *)(addr))
#if 0
#define IR_PRINT(fmt, args...)      printf("%04d %s " fmt, __LINE__, __FUNCTION__, ## args)
#else
#define IR_PRINT(fmt, args...)
#endif
#define complement(value)           (0xF - (value) + 0x1 )& 0xF // 2's Complement

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define IR_RAW_DATA_NUM	        4
#define IR_EVENT                0x0000FFFF
#define APP_DEMO_INPUT_TASK_STACK         (4096)

//#define IR_DEBUG

#ifdef IR_DEBUG
#define DEBUG_IR(x) (x)
#else
#define DEBUG_IR(x)
#endif


#if (IR_MODE_SEL == IR_TYPE_SWDECODE_MODE)
#define IR_SWDECODE_MODE_BUF_LEN        36
#endif

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
static MS_IR_KeyInfo _KeyReceived;   //temporary solution
static unsigned long  _ulLastKeyPresentTime;
static IR_Callback _pfIRCallback = NULL;

static MS_U32 _u32IRData[IR_SWDECODE_MODE_BUF_LEN];
static MS_U32 _u32IRCount = 0;


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
        MS_U8 u8Garbage;

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


static BOOL _MDrv_IR_GetKey(U8 *pu8Key, U8 *pu8System, U8 *pu8Flag)
{
        //static int count=0;
    MS_BOOL _bRet = FALSE;
    MS_U32 _u8Byte, _u8Bit;
    MS_U8 _u8IRSwModeBuf[32];
    MS_U32 _u32IRXMPBuf[4][6];
    MS_U32 _u32IRXMPDATA[4];
    MS_U8 _u8IRXMPCOUNT;
    //MS_U32 *_pu32IRData = NULL;
    //MS_U32 _u32RECS80toggle;
    MS_U32 _u32BitLen;
    MS_U8 _u8IRREPEAT;
    MS_U8 _u8XMPBIT;
    MS_U8 _u8nibble;
    MS_U8 _u8XMPPACKET;
    _u8Bit = 0;
    _u8IRREPEAT=0;
    _u8IRXMPCOUNT=0;
    _u8XMPPACKET=0;

    for(_u8XMPBIT=0; _u8XMPBIT<4 ;_u8XMPBIT++)
    {
        _u32IRXMPDATA[_u8XMPBIT]=0;
    }

    for( _u8Byte=0; _u8Byte<6; _u8Byte++) //Owner,Chechsum,RPT,M/B,Tag,Data
    {
       _u8IRSwModeBuf[_u8Byte] = 0;
    }

    if (_u32IRCount< 4+IR_RAW_DATA_NUM*8)
    {
        return FALSE; //not complete yet
    }
    //_u32IRCount = 0; // reset

    //this is to check if the shot count
    for(_u8XMPBIT=0; _u8XMPBIT<36 ;_u8XMPBIT++)
    {
        _u32BitLen = _u32IRData[_u8XMPBIT];
        DEBUG_IR(printf("_u32IRData[%d]=%d\n",_u8XMPBIT,_u32IRData[_u8XMPBIT]));
        DEBUG_IR(printf(" %d", _u32IRData[_u8XMPBIT]));
        if((_u8XMPBIT%9)!=0)
        {
            for(_u8nibble=0;_u8nibble<16;_u8nibble++)
            {
                //DEBUG_IR(printf("u8nibble=%d\n", _u8nibble));
                if (_u32BitLen > IR_LG_LOB(_u8nibble)  && _u32BitLen < IR_LG_UPB(_u8nibble) )
                {
                    _u8IRSwModeBuf[_u8IRXMPCOUNT]=_u8nibble;
                    break;
                }
            }
            _u8IRXMPCOUNT++;
        }
        else
        {
            DEBUG_IR(printf("\nHeader Bit\n"));
        }
    }

    for(_u8XMPBIT=0; _u8XMPBIT<32 ;_u8XMPBIT++)
    {
        _u32IRXMPDATA[_u8XMPPACKET] <<= 4;
        _u32IRXMPDATA[_u8XMPPACKET]+=_u8IRSwModeBuf[_u8XMPBIT];

        if((_u8XMPBIT%8)==7)
        {
            _u8XMPPACKET++;
        }
    }
    for(_u8XMPPACKET=0;_u8XMPPACKET<4;_u8XMPPACKET++)
    {
        DEBUG_IR(printf("_u32IRXMPDATA[%d] = 0x%x\n",_u8XMPPACKET,_u32IRXMPDATA[_u8XMPPACKET]));
    }
    for(_u8XMPPACKET=0;_u8XMPPACKET<4;_u8XMPPACKET++)
    {
        _u32IRXMPBuf[_u8XMPPACKET][0] =((_u32IRXMPDATA[_u8XMPPACKET] & 0xF0000000) >> 28);//Owner
        DEBUG_IR(printf("_u32IRXMPBuf[%d][0] = 0x%x\n",_u8XMPPACKET,_u32IRXMPBuf[_u8XMPPACKET][0]));
        _u32IRXMPBuf[_u8XMPPACKET][1] =((_u32IRXMPDATA[_u8XMPPACKET] & 0x0F000000) >> 24);//CheckSum
        DEBUG_IR(printf("_u32IRXMPBuf[%d][1] = 0x%x\n",_u8XMPPACKET,_u32IRXMPBuf[_u8XMPPACKET][1]));
        _u32IRXMPBuf[_u8XMPPACKET][2] =((_u32IRXMPDATA[_u8XMPPACKET] & 0x00800000) >> 23);//RPT
        DEBUG_IR(printf("_u32IRXMPBuf[%d][2] = 0x%x\n",_u8XMPPACKET,_u32IRXMPBuf[_u8XMPPACKET][2]));
        _u32IRXMPBuf[_u8XMPPACKET][3] =((_u32IRXMPDATA[_u8XMPPACKET] & 0x00100000) >> 20);//Make=0;Break=1;
        DEBUG_IR(printf("_u32IRXMPBuf[%d][3] = 0x%x\n",_u8XMPPACKET,_u32IRXMPBuf[_u8XMPPACKET][3]));
        _u32IRXMPBuf[_u8XMPPACKET][4] =((_u32IRXMPDATA[_u8XMPPACKET] & 0x000F0000) >> 16);//Tag
        DEBUG_IR(printf("_u32IRXMPBuf[%d][4] = 0x%x\n",_u8XMPPACKET,_u32IRXMPBuf[_u8XMPPACKET][4]));
        //_u32IRXMPBuf[_u8XMPPACKET][5] |=( _u32IRXMPDATA[_u8XMPPACKET] & 0x0000FFFF);//Key Data
        _u32IRXMPBuf[_u8XMPPACKET][5] =((_u32IRXMPDATA[_u8XMPPACKET] & 0x0000FF00) >> 8 );//Key Data,d0-d7 default is 0
        DEBUG_IR(printf("_u32IRXMPBuf[%d][5] = 0x%x\n",_u8XMPPACKET,_u32IRXMPBuf[_u8XMPPACKET][5]));
    }

    //check Owner

    if(_u32IRXMPBuf[1][0] != 1) // Owner = 1
    {
        DEBUG_IR(printf("Owner ID is wrong \n"));
        _bRet = FALSE;
        goto done;
    }
    //check Tag
    if(_u32IRXMPBuf[1][3] != 0) // Make=0
    {
        DEBUG_IR(printf("Tag is wrong \n"));
        _bRet = FALSE;
        goto done;
    }

    if(_u32IRXMPBuf[1][4] != 0) // tag = 0
    {
        DEBUG_IR(printf("Tag is wrong \n"));
        _bRet = FALSE;
        goto done;
    }

    //check checksum : only check IRXMPDATA[1]
    MS_U32 u32checksum =0;
    //printf("_u32IRXMPDATA[1] = 0x%X\n",_u32IRXMPDATA[1]);

    u32checksum += (_u32IRXMPDATA[1] & 0xF0000000) >> 28;
    u32checksum += (_u32IRXMPDATA[1] & 0x00F00000) >> 20;
    u32checksum += (_u32IRXMPDATA[1] & 0x000F0000) >> 16;
    u32checksum += (_u32IRXMPDATA[1] & 0x0000F000) >> 12;
    u32checksum += (_u32IRXMPDATA[1] & 0x00000F00) >> 8;
    u32checksum &= 0xF;

    //printf("==>u32checksum = %d\n",u32checksum);

    //printf("==>u32checksum = %d\n",complement(u32checksum));

    u32checksum = complement(u32checksum);

    if(_u32IRXMPBuf[1][1]!=u32checksum) // compare the checksum value
    {
        DEBUG_IR(printf("checksum wrong \n"));
        _bRet = FALSE;
        goto done;
    }


    {
        DEBUG_IR(printf("key === 0x%x\n",_u32IRXMPBuf[1][5]));
        *pu8Key = _u32IRXMPBuf[1][5];
        //*pu8Key = 0x00;
        *pu8Flag= _u32IRXMPBuf[1][2];
        //*pu8Flag= TRUE;
        _bRet = TRUE;
        goto done;
    }


    DEBUG_IR(printf(" invalid data\n"));
    _bRet = FALSE;

done:
    _u32IRCount = 0;
    return _bRet;


}

//-------------------------------------------------------------------------------------------------
/// ISR when receive IR key.
/// @return None
//-------------------------------------------------------------------------------------------------

void _MDrv_IR_ISR(InterruptNum irq)
{
    U8 u8Key=0, u8RepeatFlag=0;
    U8 u8System = 0;
    IR_PRINT("\n");

    static MS_U32 u32PreTime = 0;

    if (MsOS_GetSystemTime() - u32PreTime > IR_TIMEOUT_CYC/1000) //timeout
    {
       _u32IRCount = 0;
    }

    if (_u32IRCount <IR_SWDECODE_MODE_BUF_LEN)
    {
       _u32IRData[_u32IRCount++] = ((REG(REG_IR_SHOT_CNT_H_FIFO_STATUS) & 0xF) << 16) | REG(REG_IR_SHOT_CNT_L);
    }
    else // more then one IR remote triggered..
    {
       //to do!
    }
    u32PreTime = MsOS_GetSystemTime();

    if (_u32IRCount == IR_SWDECODE_MODE_BUF_LEN)
    {

        if (_MDrv_IR_GetKey(&u8Key, &u8System, &u8RepeatFlag))
        {
            _ulLastKeyPresentTime = MsOS_GetSystemTime();
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
    REG(REG_IR_GLHRM_NUM) |= (0x3 <<12);
    REG(REG_IR_FIFO_RD_PULSE) |= 0x0020; //wakeup key sel

    _MDrv_IR_ClearFIFO();

    _u32_1stDelayTimeMs = 0;
    _u32_2ndDelayTimeMs = 0;
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
    #if (IR_MODE_SEL == IR_TYPE_SWDECODE_MODE)
    REG(REG_IR_SEPR_BIT_FIFO_CTRL) |= 0x1 << 13;
    REG(REG_IR_GLHRM_NUM) &= 0xDFFF;
    #endif
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

//-------------------------------------------------------------------------------------------------
/// Set the IR delay time to recognize a valid key.
/// @param  u32_1stDelayTimeMs \b IN: Set the delay time to get the 1st key.
/// @param  u32_2ndDelayTimeMs \b IN: Set the delay time to get the following keys.
/// @return None
//-------------------------------------------------------------------------------------------------

void MDrv_IR_HK_SetDelayTime(MS_U32 u32_1stDelayTimeMs, MS_U32 u32_2ndDelayTimeMs)
{
    MS_U32 u32OldIntr;

    u32OldIntr = MsOS_DisableAllInterrupts();

    _u32_1stDelayTimeMs = u32_1stDelayTimeMs;
    _u32_2ndDelayTimeMs = u32_2ndDelayTimeMs;

    MsOS_RestoreAllInterrupts(u32OldIntr);
}

#endif  //#if (IR_TYPE_SEL== IR_TYPE_MSTAR_DTV)


