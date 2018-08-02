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
/// file    drvFrontPnl.c
/// @brief  FrontPanel Control Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef DDI_MISC_INUSE

//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------
#include "MsCommon.h"
#include "Board.h"
#include "drvFrontPnl.h"
#include "drvKeypad.h"

#if(FRNTPANL_TYPE_SEL==FRNTPANL_TYPE_1688)

#define ENABLE_CUS_1701 0
#define ENABLE_CUS_1703 0
#define ENABLE_CUS_8202 0
#define ENABLE_CUS_5701 0
#define ENABLE_CUS_5702 1

//===================== MARCOS OPERATE GPIO BEGIN =============================
//#define BIT(_x_)          (1 << (_x_))

//#define PIN_SM1668_STB   20//5
//#define PIN_SM1668_DATA  19
//#define PIN_SM1668_CLK  18

#define PM_BASE_ADDR         ((volatile MS_U8*) 0xBF000000)
#define PM_GPIO_SAR_REG      0x1422
#define PM_OEN_REG           0x1423
#define PM_OUT_REG           0x1424
#define PM_IN_REG            0x1425

#define  SM1668_GPIO_SET_HIGH(_GPIO_) mdrv_gpio_set_high(_GPIO_)
#define SM1668_GPIO_SET_LOW(_GPIO_) mdrv_gpio_set_low(_GPIO_)

#define SM1668_STB_H        SM1668_GPIO_SET_LOW(GPIO_FP_STB)
#define SM1668_STB_L        SM1668_GPIO_SET_HIGH(GPIO_FP_STB)

#define SM1668_CLK_H        SM1668_GPIO_SET_LOW(GPIO_FP_CLK)
#define SM1668_CLK_L        SM1668_GPIO_SET_HIGH(GPIO_FP_CLK)

#define SM1668_DATA_H       SM1668_GPIO_SET_HIGH(GPIO_FP_DATA)
#define SM1668_DATA_L       SM1668_GPIO_SET_LOW(GPIO_FP_DATA)


//===================== MARCOS OPERATE GPIO END =============================


//===================== MARCOS FOR SM1668 BEGIN ===================================

void Mdrv_FP_work_status_OnOff(MS_U8 bLightOn);

#define LED_NUM                  (4)
#define DOT_ENABLE_CODE          (0x80)

#define CMD_SET_DISP_MODE        (0x00)
#define CMD_SET_DATA             (0x40)
#define CMD_SET_ADDR             (0xC0)
#define CMD_DISP_CTRL            (0x80)

#define OPT_WRITE_SM1668_RAM     (0x00)
#define OPT_READ_SM1668_KEYVAL   (0x02)
#define ADDR_NOT_INC             (0x04)
#define ADDR_AUTO_INC            (0x00)

#define SM1668_DISP_ON           (0x8F)
#define SM1668_DISP_OFF          (0x87)

#define CLK_DELAY_TIME           400

enum
{
    DISP_MODE_4BITS_13SEG = 0,
    DISP_MODE_5BITS_12SEG,
    DISP_MODE_6BITS_11SEG,
    DISP_MODE_7BITS_10SEG,
};

//=====================MARCOS FOR SM1668 END===================================

typedef struct
{
    MS_U8 ledBitPos;
    MS_U8 ledCharDisp;
    MS_U8 enableDisp;
} LED_DISP_INFO;


typedef struct
{
    MS_U8 u8Char;
    MS_U8 lowByte;
    MS_U8 highByte;
} Char2Segment;
//b0 b1 b2 b3 b4 b5 b6 b7
//A   B   C   D   E   F   G DP

static MS_S32 s32MutexId = -1;
static MS_U8   sdisplay[8] = {"boot"};

static const Char2Segment _char2SegmentTable[] =
{
   //seg A B C D E F G H
   //SEG 0 1 2 3 4 5 6 7
   {'0',0x3F,0x00/*0*/},
   {'1',0x06,0x00/*1*/},
   {'2',0x5B,0x00/*2*/},
   {'3',0x4F,0x00/*3*/},
   {'4',0x66,0x00/*4*/},
   {'5',0x6D,0x00/*5*/},
   {'6',0x7D,0x00/*6*/},
   {'7',0x07,0x00/*7*/},
   {'8',0x7F,0x00/*8*/},
   {'9',0x6F,0x00/*9*/},
   {'A',0x77,0x00/*A*/},
   {'b',0x7C,0x00/*b*/},
   {'C',0x39,0x00/*C*/},
   {'c',0x58,0x00/*c*/},
   {'d',0x5E,0x00/*d*/},
   {'E',0x79,0x00/*E*/},
   {'F',0x71,0x00/*F*/},
   {'H',0x76,0x00/*H*/},
   {'h',0x74,0x00/*h*/},
   {'L',0x38,0x00/*L*/},
   {'n',0x54,0x00/*n*/},
   {'N',0x37,0x00/*N*/},
   {'o',0x5C,0x00/*o*/},
   {'P',0x73,0x00/*P*/},
   {'q',0x67,0x00/*q*/},
   {'r',0x50,0x00/*r*/},
   {'t',0x78,0x00/*t*/},
   {'U',0x3E,0x00/*U*/},
   {'y',0x6E,0x00/*y*/},
   {'-',0x40,0x00/*-*/}
};

static void _delaySomeNop(MS_U16 cnt)
{
    while (cnt-- > 0)
    {
        asm ("nop");
        asm ("nop");
        asm ("nop");
        asm ("nop");
        asm ("nop");
        asm ("nop");
        asm ("nop");
        asm ("nop");
    }
}

static void __WriteByte_2_SM1668(MS_U8 value)
{
    MS_U8 i;

    SM1668_CLK_H;
    _delaySomeNop(CLK_DELAY_TIME);

    for (i = 8; i != 0; i--)
    {
        if (value & 0x01)
            SM1668_DATA_H;
        else
            SM1668_DATA_L;

        SM1668_CLK_L;
        _delaySomeNop(CLK_DELAY_TIME);
        SM1668_CLK_H;
        _delaySomeNop(CLK_DELAY_TIME);

        value >>= 1;
    }
}

//////////////////////////////////////////////////////////////////////////
//write data to the specific ram address of SM1668
//////////////////////////////////////////////////////////////////////////
static void __WriteData_2_SM1668_Addr(MS_U8 value, MS_U8 addr)
{
    SM1668_STB_L;
    _delaySomeNop(CLK_DELAY_TIME);
    __WriteByte_2_SM1668(CMD_SET_DATA | ADDR_NOT_INC);
    SM1668_STB_H;
    _delaySomeNop(CLK_DELAY_TIME);

    SM1668_STB_L;
    _delaySomeNop(CLK_DELAY_TIME);
    __WriteByte_2_SM1668(CMD_SET_ADDR | addr);
    __WriteByte_2_SM1668(value);
    SM1668_STB_H;
    _delaySomeNop(CLK_DELAY_TIME);
}

/////////////////////////////////////////////////////////////////////////////
//clear all the SM1668 ram, if we want to display a new char
/////////////////////////////////////////////////////////////////////////////
static void Mdrv_Clear_SM1668_RAM(void)
{
    MS_U16 i;

    SM1668_STB_L;
    _delaySomeNop(CLK_DELAY_TIME);
    __WriteByte_2_SM1668(CMD_SET_DATA | ADDR_AUTO_INC);
    SM1668_STB_H;
    _delaySomeNop(CLK_DELAY_TIME);

    SM1668_STB_L;
    _delaySomeNop(CLK_DELAY_TIME);
    __WriteByte_2_SM1668(CMD_SET_ADDR | 0x00);


    for (i = 14; i != 0; i--)
    {
        __WriteByte_2_SM1668(0x00);
    }

    SM1668_STB_H;
    _delaySomeNop(CLK_DELAY_TIME);


}

/////////////////////////////////////////////////////////////////////////////
//SM1668 turn on display
/////////////////////////////////////////////////////////////////////////////
void Mdrv_SM1668_DispOn(void)
{

    SM1668_STB_L;
    _delaySomeNop(CLK_DELAY_TIME);
    __WriteByte_2_SM1668(SM1668_DISP_ON);
    SM1668_STB_H;
    _delaySomeNop(CLK_DELAY_TIME);
}


void Mdrv_FrontPnl_SM1668_Init(void)
{
    volatile MS_U16 i;
    extern void Enable_SAR_GPIO(MS_U8 u8SarNo,MS_BOOL bEnable);

    Enable_SAR_GPIO(GPIO_FP_STB,TRUE);
    _delaySomeNop(CLK_DELAY_TIME);
    Enable_SAR_GPIO(GPIO_FP_CLK,TRUE);
    _delaySomeNop(CLK_DELAY_TIME);
    Enable_SAR_GPIO(GPIO_FP_DATA,TRUE);
    _delaySomeNop(CLK_DELAY_TIME);

    SM1668_STB_H;
    _delaySomeNop(CLK_DELAY_TIME);
    SM1668_CLK_H;
    _delaySomeNop(CLK_DELAY_TIME);
    SM1668_DATA_H;
    _delaySomeNop(CLK_DELAY_TIME);

    for (i = 10000; i != 0; i--);
    for (i = 10000; i != 0; i--);

    Mdrv_Clear_SM1668_RAM();

    SM1668_STB_L;
    _delaySomeNop(CLK_DELAY_TIME);
    __WriteByte_2_SM1668(CMD_SET_DISP_MODE | DISP_MODE_7BITS_10SEG);
    SM1668_STB_H;
    _delaySomeNop(CLK_DELAY_TIME);

    Mdrv_SM1668_DispOn();

}

////////////////////////////////////////////////////////////////////////////////////////////////////
//change a char to BCD code for led disp
/////////////////////////////////////////////////////////////////////////////////////////////////////
static MS_U8 __GetCharCode(MS_U8 ch)
{
    const Char2Segment *ptbl;

    for (ptbl = _char2SegmentTable; \
            ptbl < (_char2SegmentTable + sizeof(_char2SegmentTable) / sizeof(Char2Segment)); ptbl++)
    {
        if (ch == ptbl->u8Char)
        {
            return ptbl->lowByte;
        }
    }
    //need promt invalid char?
    return 0xFF;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////
//Frontpnl update a specified led
////////////////////////////////////////////////////////////////////////////////////////////////////////


#define SW1_KEY  25
#define SW2_KEY  9
#define SW3_KEY  1
#define SW4_KEY  4
#define SW5_KEY  21
#define SW6_KEY  18

static MS_U8 __Get_SM1668_KeyVal(void)
{
    MS_U8 i;
    MS_U8 value = 0;
    MS_U8 bitLevel;
    MS_U8 count = 0;

    //SM1668_DIN_INPUT;
    mdrv_gpio_set_low(GPIO_FP_DATA);      //changed by YF
    mdrv_gpio_set_input(GPIO_FP_DATA);
    mdrv_gpio_get_level(GPIO_FP_DATA);

    for (i = 0; i < 40; i++)
    {
        SM1668_CLK_L;
        _delaySomeNop(CLK_DELAY_TIME);
        //_delaySomeNop(1000);

        bitLevel = mdrv_gpio_get_level(GPIO_FP_DATA);
        if (bitLevel != 0)
        {
            value = (i + 1);
            count++;
        }

        SM1668_CLK_H;
        _delaySomeNop(CLK_DELAY_TIME);
        //_delaySomeNop(CLK_DELAY_TIME);
        //_delaySomeNop(1000);
    }
    if (count > 1)  //more than one key down, inorge the scan result
    {
        value = 0;
    }
    return (value);
}


/////////////////////////////////////////////////////////////////////////////////////
//key test default Process
////////////////////////////////////////////////////////////////////////////////////
static MS_U8 __FrontPnl_KeyProc(MS_U8 keyval)
{
    switch (keyval)
    {
    case SW1_KEY:
        printf("<key = [VOL+]>\n");
        break;
    case SW2_KEY:
        printf("<key = [CH- ]>\n");
        //Mdrv_FrontPnl_DispStr((const U8 *)"CH-");
        break;
    case SW3_KEY:
        printf("<key = [VOL-]>\n");
        break;
    case SW4_KEY:
        printf("<key = [CH+ ]>\n");
        break;
    case SW5_KEY:
        printf("<key = [MENU]>\n");
        break;
    case SW6_KEY:
        printf("<key = [ OK ]>\n");
        break;


    }
    return TRUE;
}

typedef MS_U8 (*FrontPnl_GetKey_Callback)(MS_U8 key);
FrontPnl_GetKey_Callback gFrontPnlProcKey = __FrontPnl_KeyProc;

////////////////////////////////////////////////////////////////////////////////////////////////
// global   function
////////////////////////////////////////////////////////////////////////////////////////////////

//Frontpnl disp str

void MDrv_FrontPnl_Update(MS_U8 *str,MS_BOOL colon)
{
    MS_U8        i;
    MS_U8        charPos = 4;
    const MS_U8 *pStrEnd;
    MS_U8        dispBuf[8];

    MsOS_ObtainMutex(s32MutexId, MSOS_WAIT_FOREVER);

    memset(sdisplay,0,sizeof(sdisplay));
    strcpy(sdisplay,str);

    for (pStrEnd = str; '\0' != *pStrEnd; pStrEnd++);

    for (i = 0; i < 8; i++)
    {
        dispBuf[i] = 0x00;
    }

    while (pStrEnd > str && charPos > 0)
    {
        if ('.' == pStrEnd[-1])
        {
            dispBuf[(charPos-1) * 2] = DOT_ENABLE_CODE;
        }
        else
        {
            dispBuf[(charPos-1) * 2] |= __GetCharCode(pStrEnd[-1]);
            charPos -= 1;
        }
        pStrEnd--;
    }

    if (pStrEnd > str)
    {
        printf ("string is too long\n");
    }

    SM1668_STB_L;
    _delaySomeNop(CLK_DELAY_TIME);
    __WriteByte_2_SM1668(CMD_SET_DATA | ADDR_AUTO_INC);
    SM1668_STB_H;
    _delaySomeNop(CLK_DELAY_TIME);

    SM1668_STB_L;
    _delaySomeNop(CLK_DELAY_TIME);
    __WriteByte_2_SM1668(CMD_SET_ADDR | 0x00);

    for (i = 0; i < 8; i++)
    {
        __WriteByte_2_SM1668(dispBuf[i]);
    }

    SM1668_STB_H;
    _delaySomeNop(CLK_DELAY_TIME);
    Mdrv_SM1668_DispOn();

    MsOS_ReleaseMutex(s32MutexId);
}


void MDrv_FrontPnl_Time_Update(void)
{
    MS_U8        i;
    MS_U8        charPos = 4;
    const MS_U8 *pStrEnd;
    MS_U8        dispBuf[8];


    MsOS_ObtainMutex(s32MutexId, MSOS_WAIT_FOREVER);

    Mdrv_Clear_SM1668_RAM();

    for (pStrEnd = sdisplay; '\0' != *pStrEnd; pStrEnd++);

    for (i = 0; i < 8; i++)
    {
        dispBuf[i] = 0x00;
    }

    while (pStrEnd > sdisplay && charPos > 0)
    {
        if ('.' == pStrEnd[-1])
        {
            dispBuf[(charPos-1) * 2] = DOT_ENABLE_CODE;
        }
        else
        {
            dispBuf[(charPos-1) * 2] |= __GetCharCode(pStrEnd[-1]);
            charPos -= 1;
        }
        pStrEnd--;
    }

    if (pStrEnd > sdisplay)
    {
        printf ("string is too long\n");
    }

    SM1668_STB_L;
    _delaySomeNop(CLK_DELAY_TIME);
    __WriteByte_2_SM1668(CMD_SET_DATA | ADDR_AUTO_INC);
    SM1668_STB_H;
    _delaySomeNop(CLK_DELAY_TIME);

    SM1668_STB_L;
    _delaySomeNop(CLK_DELAY_TIME);
    __WriteByte_2_SM1668(CMD_SET_ADDR | 0x00);

    for (i = 0; i < 8; i++)
    {
        __WriteByte_2_SM1668(dispBuf[i]);
    }

    SM1668_STB_H;
    _delaySomeNop(CLK_DELAY_TIME);
    Mdrv_SM1668_DispOn();

    MsOS_ReleaseMutex(s32MutexId);
}


void MDrv_FrontPnl_Init(void)
{
    if (s32MutexId < 0)
    {
        s32MutexId = MsOS_CreateMutex(E_MSOS_FIFO, "FrontPnl_Mutex", MSOS_PROCESS_SHARED);
        if (s32MutexId < 0)
        {
            printf("_appZapper_Init: Create mutex failed.\n");
            return;
        }
    }
    else
    {
        printf("_appZapper_Init: Mutex is already created.\n");
    }
    Mdrv_FrontPnl_SM1668_Init();
    Mdrv_FP_work_status_OnOff(TRUE);
    MDrv_FrontPnl_Update((MS_U8*)"boot",FALSE);


}

MS_U8 Mdrv_FrontPnl_GetKeyValue(void)
{
    MS_U8 keyval;

    SM1668_STB_L;
    //_delaySomeNop(CLK_DELAY_TIME);
    __WriteByte_2_SM1668(CMD_SET_DATA | OPT_READ_SM1668_KEYVAL);

   // _delaySomeNop(1000);

    keyval = __Get_SM1668_KeyVal();

    SM1668_STB_H;
    _delaySomeNop(CLK_DELAY_TIME);

    return keyval;

}

MS_U8  Mdrv_FrontPnl_getkey(void)
{
    MS_U8 keyval;

    keyval = Mdrv_FrontPnl_GetKeyValue();
    return keyval;
}

void Mdrv_FP_work_status_OnOff(MS_U8 bLightOn)
{
    MS_U8 GRID1_REG;
    if(bLightOn)
    {
        GRID1_REG = 0x02;//grid5 0x00
    }
    else
    {
        GRID1_REG = 0x01;//grid5 0x01
    }
    __WriteData_2_SM1668_Addr(GRID1_REG, 0x01);//grid 1
    Mdrv_SM1668_DispOn();
}
#endif

#endif //#ifdef DDI_MISC_INUSE

