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
// Copyright (c) 2006-2010 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (!¡±MStar Confidential Information!¡L) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
///
/// file    drvPT6958.c
/// @brief  Front Panel Driver Interface
/// @author MStar Semiconductor Inc.
//////////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include "MsCommon.h"
#include "Board.h"
#include "drvMMIO.h"
#include "drvGPIO.h"
#include "drvPT6958.h"
#include "drvDTC.h"



//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define REG16_NPM(addr)         *((volatile MS_U16*)(u32NonPmBase+ (addr)))
#define REG16_PM(addr )         *((volatile MS_U16*)(u32PmBase+ (addr)))

#define PT6958_DIO   FRONTPNL_DIO
#define PT6958_CLK   FRONTPNL_CLK
#define PT6958_STB   FRONTPNL_STB
#define LED_RED_EN   FRONTPNL_LED_RED
#define LED_GREEN_EN FRONTPNL_LED_GREEN

//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
//static GPIOInfo_t g_PT6958_GpioInfo;
static MS_S32 stPT6958Sem = -1;


static MS_U32 u32NonPmBase = 0;
static MS_U32 u32PmBase = 0;

static MS_U8 disp_code[]={0x3f,0x06,0x6b,0x4f,0x56,0x5d,0x7d,0x07,0x7f,0x5f,0x00};
static MS_U8 gu8disp_data[]={0,0,0,0,0,0,0};
static MS_U8 gu8key_value[]={0,0,0,0,0};
static MS_BOOL gu8disp_dot = 0;
static MS_BOOL gu8ShowOpen = 1;

static MS_BOOL gu8Led_red = 0;
static MS_BOOL gu8Led_green = 0;


static MS_BOOL gu8PT6958_Init = 0;

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------


static void delay(MS_U32 i)
{
    while(i--);
}

static MS_BOOL MDrv_PT6958_CLK_SetHigh(void)
{

    #if 0
    g_PT6958_GpioInfo.gID = 0;
    g_PT6958_GpioInfo.gInvertor = 1;
    g_PT6958_GpioInfo.gPadNo = PT6958_CLK;

    msAPI_Gpio_SetgpioPtr(&g_PT6958_GpioInfo);
    msAPI_Gpio_SetOn();
    #else
    mdrv_gpio_set_high(PT6958_CLK);
    #endif

    return TRUE;
}

static MS_BOOL MDrv_PT6958_CLK_SetLow(void)
{

    #if 0
    g_PT6958_GpioInfo.gID = 0;
    g_PT6958_GpioInfo.gInvertor = 1;
    g_PT6958_GpioInfo.gPadNo = PT6958_CLK;

    msAPI_Gpio_SetgpioPtr(&g_PT6958_GpioInfo);
    msAPI_Gpio_SetOff();
    #else
    mdrv_gpio_set_low(PT6958_CLK);
    #endif
    return TRUE;
}


static MS_BOOL MDrv_PT6958_STB_SetHigh(void)
{

    #if 0
    g_PT6958_GpioInfo.gID = 0;
    g_PT6958_GpioInfo.gInvertor = 1;
    g_PT6958_GpioInfo.gPadNo = PT6958_STB;

    msAPI_Gpio_SetgpioPtr(&g_PT6958_GpioInfo);
    msAPI_Gpio_SetOn();
    #else
    mdrv_gpio_set_high(PT6958_STB);
    #endif
    return TRUE;
}

static MS_BOOL MDrv_PT6958_STB_SetLow(void)
{

    #if 0
    g_PT6958_GpioInfo.gID = 0;
    g_PT6958_GpioInfo.gInvertor = 1;
    g_PT6958_GpioInfo.gPadNo = PT6958_STB;

    msAPI_Gpio_SetgpioPtr(&g_PT6958_GpioInfo);
    msAPI_Gpio_SetOff();
    #else
    mdrv_gpio_set_low(PT6958_STB);
    #endif
    return TRUE;
}


static MS_BOOL MDrv_PT6958_DIO_SetHigh(void)
{

    #if 0
    g_PT6958_GpioInfo.gID = 0;
    g_PT6958_GpioInfo.gInvertor = 0;
    g_PT6958_GpioInfo.gPadNo = PT6958_DIO;

    msAPI_Gpio_SetgpioPtr(&g_PT6958_GpioInfo);
    msAPI_Gpio_SetOn();
    #else
    mdrv_gpio_set_high(PT6958_DIO);
    #endif
    return TRUE;
}

static MS_BOOL MDrv_PT6958_DIO_SetLow(void)
{
    #if 0
    g_PT6958_GpioInfo.gID = 0;
    g_PT6958_GpioInfo.gInvertor = 0;
    g_PT6958_GpioInfo.gPadNo = PT6958_DIO;

    msAPI_Gpio_SetgpioPtr(&g_PT6958_GpioInfo);
    msAPI_Gpio_SetOff();
    #else
    mdrv_gpio_set_low(PT6958_DIO);
    #endif
    return TRUE;
}

static MS_BOOL MDrv_PT6958_DIO_SetInput(void)
{

    #if 0
    g_PT6958_GpioInfo.gID = 0;
    g_PT6958_GpioInfo.gInvertor = 0;
    g_PT6958_GpioInfo.gPadNo = PT6958_DIO;

    msAPI_Gpio_SetgpioPtr(&g_PT6958_GpioInfo);
    msAPI_Gpio_SetInput();
    #else
    mdrv_gpio_set_input(PT6958_DIO);
    #endif
    return TRUE;
}


static MS_BOOL MDrv_PT6958_DIO_GetLevel(MS_U32* InputLevel)
{

    #if 0
    g_PT6958_GpioInfo.gID = 0;
    g_PT6958_GpioInfo.gInvertor = 0;
    g_PT6958_GpioInfo.gPadNo = PT6958_DIO;

    msAPI_Gpio_SetgpioPtr(&g_PT6958_GpioInfo);

    msAPI_Gpio_SetInput();
     delay(500);
    *InputLevel = msAPI_Gpio_GetLevel();
    #else
    mdrv_gpio_set_input(PT6958_DIO);
    delay(500);
    *InputLevel = (MS_U32)mdrv_gpio_get_level(PT6958_DIO);
    #endif
    return TRUE;
}


static MS_BOOL MDrv_PT6958_SendCommand(MS_U8 u8Cmd, MS_BOOL bStbHigh)
{

    MS_U8 index;

    MDrv_PT6958_STB_SetHigh();
    MDrv_PT6958_CLK_SetHigh();
    //MsOS_DelayTaskUs(2);
    delay(500);

    MDrv_PT6958_STB_SetLow();
    //MsOS_DelayTaskUs(2);
    for(index =0;index<8;index++)
    {
        MDrv_PT6958_CLK_SetLow();
        //MsOS_DelayTaskUs(1);
        delay(500);

        if((u8Cmd & 0x1) == 0x1)
        {
            MDrv_PT6958_DIO_SetHigh();
        }
        else
        {
            MDrv_PT6958_DIO_SetLow();
        }

        //MsOS_DelayTaskUs(1);
        delay(500);
        MDrv_PT6958_CLK_SetHigh();
        //MsOS_DelayTaskUs(1);
        delay(500);
        u8Cmd = u8Cmd >> 1;
    }

#if 1
    if(bStbHigh == TRUE)
    {
        MDrv_PT6958_STB_SetHigh();
    }
#endif
    return TRUE;
}



static MS_BOOL MDrv_PT6958_WriteByte(MS_U8 u8Byte, MS_BOOL bStbHigh)
{

    MS_U8 index;

    MDrv_PT6958_CLK_SetHigh();
    //MsOS_DelayTaskUs(1);
    delay(500);

    MDrv_PT6958_STB_SetLow();
    //MsOS_DelayTaskUs(2);
    for(index =0;index<8;index++)
    {
        MDrv_PT6958_CLK_SetLow();
        //MsOS_DelayTaskUs(1);
        delay(1000);

        if((u8Byte & 0x1) == 0x1)
        {
            MDrv_PT6958_DIO_SetHigh();
        }
        else
        {
            MDrv_PT6958_DIO_SetLow();
        }
        //MsOS_DelayTaskUs(1);
        delay(1000);

        MDrv_PT6958_CLK_SetHigh();
        //MsOS_DelayTaskUs(1);
        delay(1000);
        u8Byte = u8Byte >> 1;
    }

#if 1
    if(bStbHigh == TRUE)
    {
        MDrv_PT6958_STB_SetHigh();
    }
#endif
    return TRUE;
}


static MS_BOOL MDrv_PT6958_ReadByte(MS_U8 *u8Byte, MS_BOOL bStbHigh)
{

    MS_U8 index;
    MS_U8 u8Value;
    MS_U32 u32Level;

    u8Value = 0;
    MDrv_PT6958_CLK_SetHigh();
    //MDrv_PT6958_DIO_SetHigh();
    delay(500);
    MDrv_PT6958_DIO_SetInput();
    //MsOS_DelayTaskUs(1);
    delay(500);

    MDrv_PT6958_STB_SetLow();
    //MsOS_DelayTaskUs(2);

    for(index =0;index<8;index++)
    {

        u8Value = u8Value << 1;
        MDrv_PT6958_CLK_SetHigh();
        delay(1000);

        MDrv_PT6958_CLK_SetLow();
        //MsOS_DelayTaskUs(1);
        delay(1000);

        MDrv_PT6958_DIO_GetLevel(&u32Level);

        if(u32Level == 0x0)
        {
            u8Value = u8Value | 0;
        }
        else
        {
            u8Value = u8Value | 1;
        }

        //MDrv_PT6958_CLK_SetHigh();
        //MsOS_DelayTaskUs(1);
        delay(1000);

    }

#if 1
    if(bStbHigh == TRUE)
    {
        MDrv_PT6958_STB_SetHigh();
    }
#endif
    *u8Byte = u8Value;
    return TRUE;
}

static MS_U8 MDrv_PT6958_ReadKey(void)
{
    MS_U8 u8Key;

    MDrv_PT6958_ReadByte(&u8Key,FALSE);
    return u8Key;
}


#ifdef UTOPIA_TYPE
static MS_BOOL MDrv_PT6958_SAR_Pad_Setting(void)
{
    MS_VIRT virtMMIOBaseAdr;
    MS_PHY phyBankSize;

    if(stPT6958Sem < 0)
    {
        stPT6958Sem = MsOS_CreateMutex (E_MSOS_FIFO, "MUTEX_PT6958", MSOS_PROCESS_SHARED);
    }

    if(stPT6958Sem < 0)
    {
        return FALSE;
    }

    if(FALSE == MsOS_ObtainMutex(stPT6958Sem, MSOS_WAIT_FOREVER))
    {
        return FALSE;
    }

    if( !MDrv_MMIO_GetBASE(&virtMMIOBaseAdr, &phyBankSize, MS_MODULE_HW))
    {
        printf("Get IOMap failure\n");
        MS_ASSERT(0);
        return FALSE;
    }
    else
    {
        printf("Get IOMap u32NonPmBase = 0x%"DTC_MS_U32_x" \n", virtMMIOBaseAdr);
    }

    u32NonPmBase = virtMMIOBaseAdr;
    if( !MDrv_MMIO_GetBASE(&virtMMIOBaseAdr, &phyBankSize, MS_MODULE_PM))
    {
        printf("Get IOMap failure\n");
        MS_ASSERT(0);
        return FALSE;
    }
    else
    {
        printf("Get IOMap u32PmBase = 0x%"DTC_MS_U32_x" \n", virtMMIOBaseAdr);
    }
    u32PmBase = virtMMIOBaseAdr;

    REG16_PM(0xa11*4) &= 0xFFF3;

    return TRUE;
}
#else
static MS_BOOL MDrv_PT6958_SAR_Pad_Setting(void)
{
    MS_U32 u32MMIOBaseAdr, u32BankSize;

    if(stPT6958Sem < 0)
    {
        stPT6958Sem = MsOS_CreateMutex (E_MSOS_FIFO, "MUTEX_PT6958", MSOS_PROCESS_SHARED);
    }

    if(stPT6958Sem < 0)
    {
        return FALSE;
    }

    if(FALSE == MsOS_ObtainMutex(stPT6958Sem, MSOS_WAIT_FOREVER))
    {
        return FALSE;
    }


    if( !MDrv_MMIO_GetBASE(&u32MMIOBaseAdr, &u32BankSize, MS_MODULE_HW))
    {
        printf("Get IOMap failure\n");
        MS_ASSERT(0);
        return FALSE;
    }
    else
    {
        printf("Get IOMap u32NonPmBase = 0x%lx \n",(MS_U32)u32MMIOBaseAdr);
    }

    u32NonPmBase = u32MMIOBaseAdr;
    if( !MDrv_MMIO_GetBASE(&u32MMIOBaseAdr, &u32BankSize, MS_MODULE_PM))
    {
        printf("Get IOMap failure\n");
        MS_ASSERT(0);
        return FALSE;
    }
    else
    {
        printf("Get IOMap u32PmBase = 0x%lx \n",(MS_U32)u32MMIOBaseAdr);
    }
    u32PmBase = u32MMIOBaseAdr;



    //MDrv_SAR_Gpio_CfgDir(2,0);// SAR2 , output
     //MDrv_SAR_Gpio_CfgDir(3,0);// SAR3 , output
    REG16_PM(0xa11*4) &= 0xFFF3;

    //_RVM1(0x0E6A, GPIO_PM10_IS_GPIO, BIT3),
    //REG16_PM(0x1CD4) |= 0x8;

    return TRUE;
}

#endif

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------



static MS_BOOL MDrv_PT6958_Show(void)
{
    MS_U8 u8Index;
    MS_U8 u8LEDOut = 0;


    if(gu8PT6958_Init == 0)
    {
        return FALSE;
    }

    if(FALSE == MsOS_ObtainMutex(stPT6958Sem, MSOS_WAIT_FOREVER))
    {
        return FALSE;
    }

    //cmd 2, when power init bit 4 ~ bit 1 must be 0
    MDrv_PT6958_SendCommand(0x40,TRUE);

    //cmd 3, clean RAM.
    MDrv_PT6958_SendCommand(0xC0,FALSE);

    for(u8Index = 0; u8Index<0x4; u8Index++)
    {

        if(gu8disp_dot == 1)
        {
            MDrv_PT6958_WriteByte(disp_code[gu8disp_data[u8Index]] | 0x80 ,FALSE);
        }
        else
        {
            MDrv_PT6958_WriteByte(disp_code[gu8disp_data[u8Index]] & 0x7F ,FALSE);
        }

        if(u8Index == 0x3)
        {
            MDrv_PT6958_WriteByte(u8LEDOut,TRUE);
        }
        else
        {
            MDrv_PT6958_WriteByte(u8LEDOut,FALSE);
        }
    }

    //cmd 1, 13x4
    MDrv_PT6958_SendCommand(0x0,TRUE);

    //cmd 4, show open
    if(gu8ShowOpen == 0)
    {
        MDrv_PT6958_SendCommand(0x80,TRUE);
    }
    else
    {
        MDrv_PT6958_SendCommand(0x88,TRUE);
    }

    if(FALSE == MsOS_ReleaseMutex(stPT6958Sem))
    {
        return FALSE;
    }

    return TRUE;
}

static MS_BOOL MDrv_PT6958_Init(void)
{

#if 1
    MS_U8 u8Index;

    MDrv_PT6958_SAR_Pad_Setting();
    //msAPI_Gpio_Init();
    mdrv_gpio_init();


    MDrv_PT6958_STB_SetHigh();
    MDrv_PT6958_CLK_SetHigh();
    MDrv_PT6958_DIO_SetHigh();
    MsOS_DelayTask(500);
    // init flow

    //cmd 1, when power init bit 4 ~ bit 1 must be 0
    MDrv_PT6958_SendCommand(0x40,TRUE);

    //cmd 2, clean RAM.
    MDrv_PT6958_SendCommand(0xC0,FALSE);

    for(u8Index = 0; u8Index<0x4; u8Index++)
    {
        if(u8Index == 0x3)
        {
            //MDrv_PT6958_WriteByte(0x0,TRUE);
            MDrv_PT6958_WriteByte(disp_code[u8Index],FALSE);
            MDrv_PT6958_WriteByte(0xff,TRUE);
        }
        else
        {
            //MDrv_PT6958_WriteByte(0x0,FALSE);
            MDrv_PT6958_WriteByte(disp_code[u8Index] | 0x80,FALSE);
            MDrv_PT6958_WriteByte(0xff,FALSE);
        }
    }


    //cmd 3, show open
    if(gu8ShowOpen == 0)
    {
        MDrv_PT6958_SendCommand(0x80,TRUE);
    }
    else
    {
        MDrv_PT6958_SendCommand(0x88,TRUE);
    }

    gu8PT6958_Init = 1;

    if(FALSE == MsOS_ReleaseMutex(stPT6958Sem))
    {
        return FALSE;
    }

#else
    MS_U32 u32Level;
    MDrv_PT6958_DIO_GetLevel(&u32Level);
    printf("u32Level = 0x%x\n",u32Level);
#endif
    return TRUE;
}


static MS_BOOL MDrv_PT6958_ShowOpen_Enable(MS_BOOL bEnable)
{
    if(FALSE == MsOS_ObtainMutex(stPT6958Sem, MSOS_WAIT_FOREVER))
    {
        return FALSE;
    }

    gu8ShowOpen = bEnable;

    if(FALSE == MsOS_ReleaseMutex(stPT6958Sem))
    {
        return FALSE;
    }

    return TRUE;
}


static MS_BOOL MDrv_PT6958_Dot_Enable(MS_BOOL bEnable)
{
    if(FALSE == MsOS_ObtainMutex(stPT6958Sem, MSOS_WAIT_FOREVER))
    {
        return FALSE;
    }

    gu8disp_dot = bEnable;

    if(FALSE == MsOS_ReleaseMutex(stPT6958Sem))
    {
        return FALSE;
    }

    return TRUE;
}

static MS_BOOL MDrv_PT6958_Digital_Set(MS_U8 u8D4, MS_U8 u8D3, MS_U8 u8D2, MS_U8 u8D1)
{
    if(FALSE == MsOS_ObtainMutex(stPT6958Sem, MSOS_WAIT_FOREVER))
    {
        return FALSE;
    }

    gu8disp_data[0] = u8D1;
    gu8disp_data[1] = u8D2;
    gu8disp_data[2] = u8D3;
    gu8disp_data[3] = u8D4;

    if(FALSE == MsOS_ReleaseMutex(stPT6958Sem))
    {
        return FALSE;
    }

    return TRUE;
}

static MS_BOOL MDrv_PT6958_ReadKeyPad(MS_U32 *u32KeyValue)
{

    MS_U8 u8Index;

    //msAPI_Gpio_Init();

    // init flow

    if(gu8PT6958_Init == 0)
    {
        return FALSE;
    }

    if(FALSE == MsOS_ObtainMutex(stPT6958Sem, MSOS_WAIT_FOREVER))
    {
        return FALSE;
    }


    *u32KeyValue = 0x0;

    //cmd 2, scaning keypad bit1 is 1.
    MDrv_PT6958_SendCommand(0x42,FALSE);
    MsOS_DelayTaskUs(1);

    // clean RAM.
    for(u8Index = 0; u8Index<0x3; u8Index++)
    {
        gu8key_value[u8Index] = MDrv_PT6958_ReadKey()&0xFF;

        if((gu8key_value[0] & 0x40) == 0x40)
        {
            *u32KeyValue |= KEY_S1;
        }

        if((gu8key_value[0] & 0x80) == 0x80)
        {
            *u32KeyValue |= KEY_S2;
        }

        if((gu8key_value[0] & 0x02) == 0x02)
        {
            *u32KeyValue |= KEY_S5;
        }

        if((gu8key_value[0] & 0x04) == 0x04)
        {
            *u32KeyValue |= KEY_S3;
        }

        if((gu8key_value[0] & 0x08) == 0x08)
        {
            *u32KeyValue |= KEY_S4;
        }

    }

    MDrv_PT6958_STB_SetHigh();

    if(FALSE == MsOS_ReleaseMutex(stPT6958Sem))
    {
        return FALSE;
    }

    return TRUE;
}

static MS_BOOL MDrv_PT6958_LED_RED_Enable(MS_BOOL bEnable)
{
    if(FALSE == MsOS_ObtainMutex(stPT6958Sem, MSOS_WAIT_FOREVER))
    {
        return FALSE;
    }

    gu8Led_red = bEnable;

    //printf("MDrv_PT6958_LED_RED_Enable gu8Led_red = %d\n",gu8Led_red);

    if(gu8Led_red == TRUE)
    {
        mdrv_gpio_set_high(LED_RED_EN);
    }
    else
    {
        mdrv_gpio_set_low(LED_RED_EN);
    }
    if(FALSE == MsOS_ReleaseMutex(stPT6958Sem))
    {
        return FALSE;
    }

    return TRUE;
}

static MS_BOOL MDrv_PT6958_LED_GREEN_Enable(MS_BOOL bEnable)
{
    if(FALSE == MsOS_ObtainMutex(stPT6958Sem, MSOS_WAIT_FOREVER))
    {
        return FALSE;
    }

    gu8Led_green = bEnable;

    //printf("MDrv_PT6958_LED_GREEN_Enable gu8Led_green = %d\n",gu8Led_green);

    if(gu8Led_green == TRUE)
    {
        mdrv_gpio_set_high(LED_GREEN_EN);
    }
    else
    {
        mdrv_gpio_set_low(LED_GREEN_EN);
    }


    if(FALSE == MsOS_ReleaseMutex(stPT6958Sem))
    {
        return FALSE;
    }

    return TRUE;
}

MS_BOOL MDrv_FrontPnl_Init(void)
{
    return MDrv_PT6958_Init();
}

MS_BOOL MDrv_FrontPnl_Show_Enable(MS_BOOL bEnable)
{
    return MDrv_PT6958_ShowOpen_Enable(bEnable);
}

MS_BOOL MDrv_FrontPnl_Show(void)
{
    return MDrv_PT6958_Show();
}

MS_BOOL MDrv_FrontPnl_LED_Enable(MS_BOOL *pEnableArray, MS_U8 u8Count)
{
    if(u8Count >= 1)
    {
        MDrv_PT6958_LED_RED_Enable(pEnableArray[0]);
    }
    else
    {
        MDrv_PT6958_LED_RED_Enable(FALSE);
    }


    if(u8Count >= 2)
    {
        MDrv_PT6958_LED_GREEN_Enable(pEnableArray[1]);
    }
    else
    {
        MDrv_PT6958_LED_GREEN_Enable(FALSE);
    }



    return TRUE;
}

MS_BOOL MDrv_FrontPnl_Dot_Enable(MS_BOOL bEnable)
{
    return MDrv_PT6958_Dot_Enable(bEnable);
}

MS_BOOL MDrv_FrontPnl_Digital_Set(MS_U8 *pDigitalArray, MS_U8 u8Count)
{
    MS_U8 u8D1, u8D2, u8D3, u8D4;

    if(u8Count >= 1)
    {
        u8D1 = pDigitalArray[0];
    }
    else
    {
        u8D1 = 0;
    }

    if(u8Count >= 2)
    {
        u8D2 = pDigitalArray[1];
    }
    else
    {
        u8D2 = 0;
    }

    if(u8Count >= 3)
    {
        u8D3 = pDigitalArray[2];
    }
    else
    {
        u8D3 = 0;
    }

    if(u8Count >= 4)
    {
        u8D4 = pDigitalArray[3];
    }
    else
    {
        u8D4 = 0;
    }

    return MDrv_PT6958_Digital_Set(u8D4,u8D3,u8D2,u8D1);
}

MS_BOOL MDrv_FrontPnl_ReadKeyPad(MS_U32 *u32KeyValue)
{
    return MDrv_PT6958_ReadKeyPad(u32KeyValue);
}

