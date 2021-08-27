////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2009 MStar Semiconductor, Inc.
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
#define _MSKEYPAD_C_

#include <common.h>
#include <sys/common/MsTypes.h>
#include <msKeypad.h>
#if (CONFIG_COMMANDS & CFG_CMD_KEYPAD)

#define ONE_MS_TIME_VALUE     (1000 * (CFG_HZ / 1000000))

static U8 KeyPadCheckCount, PreviousCMD, PressKey;
static U32 KeyPadTimePeriod, g100msTimeCount,g1msTimeCount;
static U32 _gKeypadTimeTick;

static U8 Keypad_Channel[ADC_KEY_CHANNEL_NUM];

#ifndef ADC_KEY_LEVEL_TOLERANCE
#define ADC_KEY_LEVEL_TOLERANCE        0xFF
#endif

static U8 const tADCKeyLevel[] =
{
    ADC_KEY_L0,
    ADC_KEY_L1,
    ADC_KEY_L2,
    ADC_KEY_L3,
};

static U8 const tADCKey1Flag[] =
{
    ADC_KEY_1_L0_FLAG,
    ADC_KEY_1_L1_FLAG,
    ADC_KEY_1_L2_FLAG,
    ADC_KEY_1_L3_FLAG,
};

static U8 const tADCKey2Flag[] =
{
    ADC_KEY_2_L0_FLAG,
    ADC_KEY_2_L1_FLAG,
    ADC_KEY_2_L2_FLAG,
    ADC_KEY_2_L3_FLAG,
};


static void msKeypad_Reset100msTimeCount(void);
static void msKeypad_Update100msTimeCount(void);
static U32 msKeypad_Get100msTimeCount(void);
static BOOLEAN msKeypad_AllocChan(U8 u8KpdCh);
static U8 msKeypad_GetChanIndex(U8 u8KpdCh);
static BOOLEAN msKeypad_CH_GetKey(U8 Channel, U8 *pkey, U8* pflag);


static void msKeypad_Reset100msTimeCount(void)
{
    g100msTimeCount = 0;
    g1msTimeCount = 0;
    _gKeypadTimeTick = get_ticks();
}

static void msKeypad_Update100msTimeCount(void)
{
    U32 u32CurTicks = get_ticks();
    if ((u32CurTicks - _gKeypadTimeTick)>=ONE_MS_TIME_VALUE)
    {
        g1msTimeCount += ((u32CurTicks - _gKeypadTimeTick)/ONE_MS_TIME_VALUE);

        _gKeypadTimeTick = get_ticks();
        if (g1msTimeCount>=100)
        {
            U32 u32Temp = g1msTimeCount;
            g1msTimeCount = (u32Temp%100);
            g100msTimeCount+=(u32Temp/100);
        }
    }
}

static U32 msKeypad_Get100msTimeCount(void)
{
    return g100msTimeCount;
}

static BOOLEAN msKeypad_AllocChan(U8 u8KpdCh)
{
    static U8 u8KpdChIdx=0;

    if(u8KpdChIdx>=ADC_KEY_CHANNEL_NUM)
    {
        printf("[msKeypad_Init]: Allocated channel number > %d !!!\n",ADC_KEY_CHANNEL_NUM);
        return MSRET_ERROR;
    }
    Keypad_Channel[u8KpdChIdx++]=u8KpdCh;
    return MSRET_OK;
}

static U8 msKeypad_GetChanIndex(U8 u8KpdCh)
{
    U8 k;

    for(k=0;k<ADC_KEY_CHANNEL_NUM;k++)
    {
        if(Keypad_Channel[k]==u8KpdCh)
            return k;
    }
    return 0xFF;
}

/******************************************************************************/
///Keypad get ADC Channel value
///@param pkey \b IN ADC Channel
///@param pflag \b IN Pointer to the ADC Channel value
/******************************************************************************/
U8 msKeypad_Get_ADC_Channel(U8 Channel, U8 *pvalue)
{
    U8 u8Status = 0;

    switch ( Channel )
    {
        case KEYPAD_ADC_CHANNEL_1:
            *pvalue = MDrv_ReadByte(REG_SAR_ADCOUT1) & MASK_SAR_ADCOUT;
            break;
        case KEYPAD_ADC_CHANNEL_2:
            *pvalue = MDrv_ReadByte(REG_SAR_ADCOUT2) & MASK_SAR_ADCOUT;
            break;
        case KEYPAD_ADC_CHANNEL_3:
            *pvalue = MDrv_ReadByte(REG_SAR_ADCOUT3) & MASK_SAR_ADCOUT;
            break;
        case KEYPAD_ADC_CHANNEL_4:
            *pvalue = MDrv_ReadByte(REG_SAR_ADCOUT4) & MASK_SAR_ADCOUT;
             break;
        default:
            u8Status = 1;
            *pvalue = 0xFF;
            break;
    }

    return u8Status;
}

static BOOLEAN msKeypad_CH_GetKey(U8 Channel, U8 *pkey, U8* pflag)
{
    U8 i, j, KEY_LV[ADC_KEY_LEVEL], Key_Value, *Keymapping=NULL;
    U8 u8ChIdx=0;

    *pkey = 0xFF;
    *pflag = 0;

    for(i=0; i<ADC_KEY_LEVEL; i++)
        KEY_LV[i] = 0;

    u8ChIdx = msKeypad_GetChanIndex(Channel);
    switch(u8ChIdx)
    {
        case 0: Keymapping = (U8*)tADCKey1Flag; break;
        case 1: Keymapping = (U8*)tADCKey2Flag; break;
        default: return MSRET_ERROR;
    }

    for ( i = 0; i < KEYPAD_STABLE_NUM; i++ )
    {
        msKeypad_Get_ADC_Channel(Channel,&Key_Value);
        for (j=0;j<ADC_KEY_LEVEL;j++)
        {
            if (Key_Value < tADCKeyLevel[j])
            {
            	if((tADCKeyLevel[j]-Key_Value)<=ADC_KEY_LEVEL_TOLERANCE)
        		{
	                KEY_LV[j]++;
	                break;
        		}
            }
        }
    }

    for(i=0; i<ADC_KEY_LEVEL; i++)
    {
        if(KEY_LV[i] > KEYPAD_STABLE_NUM_MIN)
        {
            PressKey = TRUE;
            *pkey = *(Keymapping+i);
            if (PreviousCMD != *pkey)
            {
                PreviousCMD = *pkey;
                KeyPadCheckCount = 0;
            }
            else
            {
                if (KeyPadCheckCount < KEYPAD_KEY_VALIDATION)
                {
                   KeyPadCheckCount++;
                    return MSRET_ERROR;
                }
                else if (KeyPadCheckCount == KEYPAD_KEY_VALIDATION)
                {
                    KeyPadCheckCount++;
                    msKeypad_Reset100msTimeCount();
                    KeyPadTimePeriod = msKeypad_Get100msTimeCount();
                    return MSRET_OK;
                }

                msKeypad_Update100msTimeCount();
                if (KeyPadCheckCount == KEYPAD_REPEAT_KEY_CHECK)    //3+2
                {
                    if (msKeypad_Get100msTimeCount() > KeyPadTimePeriod + KEYPAD_REPEAT_PERIOD/3)
                    {
                        KeyPadTimePeriod = msKeypad_Get100msTimeCount();
                        KeyPadCheckCount = KEYPAD_REPEAT_KEY_CHECK_1;
                        *pflag = 0x01;
                    }
                    else
                    {
                        *pkey = 0xFF;
                        *pflag = 0x01;
                    }
                    return MSRET_OK;
                }
                else if (KeyPadCheckCount == KEYPAD_REPEAT_KEY_CHECK_1) //3+3
                {
                    if (msKeypad_Get100msTimeCount() > KeyPadTimePeriod)
                    {
                        KeyPadTimePeriod = msKeypad_Get100msTimeCount();
                        KeyPadCheckCount = KEYPAD_REPEAT_KEY_CHECK_1;
                        *pflag = 0x01;
                    }
                    else
                    {
                        *pkey = 0xFF;
                        *pflag = 0x01;
                    }
                    return MSRET_OK;
                }

                if (msKeypad_Get100msTimeCount() > KeyPadTimePeriod + KEYPAD_REPEAT_PERIOD)  //if 200ms
                {
                    KeyPadTimePeriod = msKeypad_Get100msTimeCount();
                    KeyPadCheckCount = KEYPAD_REPEAT_KEY_CHECK; //3+2
                    *pflag = 0x01;
                    return MSRET_OK;
                }
                else
                {
                    return MSRET_ERROR;
                }
            }
        }
    }

    if(u8ChIdx == ADC_KEY_LAST_CHANNEL)
    {
        if (PressKey)
            PressKey = FALSE;

        else
            PreviousCMD = 0xFF;
    }
    return MSRET_ERROR;

}

void msKeypad_Init(void)
{
  #if(ENABLE_KPDCHAN_1==ENABLE)//channel 1
    MDrv_WriteByte(REG_SAR1_UPB, KEYPAD_CH1_UB);
    MDrv_WriteByte(REG_SAR1_LOB, KEYPAD_CH1_LB);
    msKeypad_AllocChan(KEYPAD_ADC_CHANNEL_1);
  #endif

  #if(ENABLE_KPDCHAN_2==ENABLE)//channel 2
    MDrv_WriteByte(REG_SAR2_UPB, KEYPAD_CH2_UB);
    MDrv_WriteByte(REG_SAR2_LOB, KEYPAD_CH2_LB);
    msKeypad_AllocChan(KEYPAD_ADC_CHANNEL_2);
  #endif

  #if(ENABLE_KPDCHAN_3==ENABLE)//channel 3
    MDrv_WriteByte(REG_SAR3_UPB, KEYPAD_CH3_UB);
    MDrv_WriteByte(REG_SAR3_LOB, KEYPAD_CH3_LB);
    msKeypad_AllocChan(KEYPAD_ADC_CHANNEL_3);
  #endif

  #if(ENABLE_KPDCHAN_4==ENABLE)//channel 4
    MDrv_WriteByte(REG_SAR4_UPB, KEYPAD_CH4_UB);
    MDrv_WriteByte(REG_SAR4_LOB, KEYPAD_CH4_LB);
    msKeypad_AllocChan(KEYPAD_ADC_CHANNEL_4);
  #endif

    MDrv_WriteByte(REG_SAR_CTRL0, (SAR_MODE_FREERUN|SAR_SIGNAL_CH));
    MDrv_WriteByte(REG_SAR_CTRL1, MDrv_ReadByte(REG_SAR_CTRL1)&(~SAR_ADC_PWRDN));
    MDrv_WriteByte(REG_SAR_CTRL1, MDrv_ReadByte(REG_SAR_CTRL1)|(SAR_ADC_FREERUN));
    MDrv_WriteByte(REG_SAR_SELCH, MDrv_ReadByte(REG_SAR_SELCH)&(~SAR_NCH_EN));
    MDrv_WriteByte(REG_SAR_CKSAMP_PRD, 0x05);   //sample period

  #if(ENABLE_KPDCHAN_1==ENABLE)//channel 1
    //select pad as analog input
    MDrv_WriteByte(REG_SAR_AISEL, MDrv_ReadByte(REG_SAR_AISEL)|(SAR_AISEL_CH0_MSK));
    //select pad direction as input mode
    MDrv_WriteByte(REG_SAR_GPIOOEN, MDrv_ReadByte(REG_SAR_GPIOOEN)|(SAR_GPIOOEN_CH0_MSK));
  #endif

  #if(ENABLE_KPDCHAN_2==ENABLE)//channel 2
    //select pad as analog input
    MDrv_WriteByte(REG_SAR_AISEL, MDrv_ReadByte(REG_SAR_AISEL)|(SAR_AISEL_CH1_MSK));
    //select pad direction as input mode
    MDrv_WriteByte(REG_SAR_GPIOOEN, MDrv_ReadByte(REG_SAR_GPIOOEN)|(SAR_GPIOOEN_CH1_MSK));
  #endif

  #if(ENABLE_KPDCHAN_3==ENABLE)//channel 3
    //select pad as analog input
    MDrv_WriteByte(REG_SAR_AISEL, MDrv_ReadByte(REG_SAR_AISEL)|(SAR_AISEL_CH2_MSK));
    //select pad direction as input mode
    MDrv_WriteByte(REG_SAR_GPIOOEN, MDrv_ReadByte(REG_SAR_GPIOOEN)|(SAR_GPIOOEN_CH2_MSK));
  #endif

  #if(ENABLE_KPDCHAN_4==ENABLE)//channel 4
    //select pad as analog input
    MDrv_WriteByte(REG_SAR_AISEL, MDrv_ReadByte(REG_SAR_AISEL)|(SAR_AISEL_CH3_MSK));
    //select pad direction as input mode
    MDrv_WriteByte(REG_SAR_GPIOOEN, MDrv_ReadByte(REG_SAR_GPIOOEN)|(SAR_GPIOOEN_CH3_MSK));
  #endif
    //disable sar mask
    MDrv_WriteByte(REG_SAR_INT_MASK, MDrv_ReadByte(REG_SAR_INT_MASK)|(SAR_INT_MASK));
}

/******************************************************************************/
///Keypad get key value and repeat flag
///@param pkey \b IN return the key value(The same as Irda key value)
///@param pflag \b IN return the repeat flag(1:Repeat)
/******************************************************************************/
BOOLEAN msKeypad_GetKey(U8 *pkey, U8 *pflag)
{
    U8 Channel;

    for (Channel=0; Channel<KEYPAD_CHANNEL_SUPPORT; Channel++)
    {
        if (msKeypad_CH_GetKey(Channel, pkey, pflag))
        {
            return MSRET_OK;
        }
    }
    return MSRET_ERROR;
}

#undef _MSKEYPAD_C_
#endif

