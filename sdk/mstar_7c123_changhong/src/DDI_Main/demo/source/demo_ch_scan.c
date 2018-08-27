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
// (!¡±MStar Confidential Information!¡L) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @file   Demo_Audio.h
/// @author MStar Semiconductor Inc.
/// @brief  The demo code for audio functions
///////////////////////////////////////////////////////////////////////////////////////////////////
#if (DEMO_CH_SCAN_TEST == 1)
//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------
#include "MsCommon.h"
#include <string.h>
#include "apiDigiTuner.h"
#include "appDemo_DigiTuner.h"
#include "Frontend_Enum_Device.h"
#include "demo_ch_scan.h"
#include "satConstInfo.h"
#include "drvDTC.h"
#include "demo_dmx.h"

//-------------------------------------------------------------------------------------------------
// Local Defines
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------
#define MAX_NUM_OF_PCHBYSATID 3000 //max physical channel of one satellite
#define MAX_TPSOFSAT_NUM   MAX_NUM_OF_PCHBYSATID

#define CH_SCAN_DELAY           30 // ms

#define CH_SCAN_DBG 0
#if CH_SCAN_DBG
#define ch_scan_print(fmt, args...)  printf("[%s][%d]" fmt, __FUNCTION__, __LINE__, ## args)
#else
#define ch_scan_print(fmt, args...)
#endif

#define TEDDY_CH_SCAN_DONE 0//to fix, by Teddy.Chen

#define CH_SCAN_SUPPORT_ATSC 0

//-------------------------------------------------------------------------------------------------
// Global Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
// Local Variables
//-------------------------------------------------------------------------------------------------
static BLIND_SCAN_TYPE  _eBlindScanType = E_SCAN_TYPE_SCAN_TP_ALL_THEN_PG_ALL;
static TaskState _eChScanState;
static ChScan_Param _gstScanParam;
static MS_U32 _gu32NumOfChanScan;
static MS_U32 _gu32LockTimeMax;
static MS_U32 _gu32LockTime;
static MS_U8 _gu8PlpSize = 0;
static MS_U8 _gu8PlpIndex = 0;
static MS_BOOL _gbScanLock = FALSE;


static const FreqTblType _gstFreqTbl[FREQ_TAL_SIZE]  =
{
        //must sort by channel no.  from small to large
        //if the channel is invalid,set the frequency 0
        //No. offset ----->for displaying 0:= No. ; +X:No.-X and add "A"  ; -X:No.-X
        {6,177500,0}, // 6~9
        {10,205500,1}, // 9A
        {11,212500,-1}, // 10~12
        {14,0,-1}, //13~20
        {21,480500,0}, // 21~27
        {28,527500,1}, // 27A
        {29,529500,-1}, // 28~75
};

static MS_U8 _gu8CurScanSATPos = 0;
static BLINDSCAN_STATUS _geBlindScanStatus = BLINDSCAN_READY;
static MS_U16 _gu16GetTPSNum = 0;
static MS_U8 _gu8TPBlindScanProgress = 0;//the progress of blind scan 1 frequency
static MS_U8 _gu8MotorPosition=0;
/*
static MS_SAT_PARAM _gstSat_param = {
                    .e0V12VOnOff = EN_0V12V_ONOFF_ON,
                    .e22KOnOff = EN_22K_AUTO,
                    .eDiseqcLevel = EN_DISEQC_LVL_OFF,
                    .eLNBPwrOnOff = EN_LNBPWR_13OR18V,
                    .eSwt10Port = EN_SWT_PORT_OFF,
                    .eSwt11Port = EN_SWT_PORT_OFF,
                    .eToneburstType = EN_TONEBURST_NONE,
                    .u16Angle = 0,
                    .u16HiLOF = 10600,
                    .u16LoLOF = 9750,
                    .u8Position = 0,
                    .bPorInvert = 0,
                    };
*/
static MS_SAT_PARAM* _gpstSat_param = NULL;
static MS_FE_CARRIER_PARAM _gstAfe_param[MAX_TPSOFSAT_NUM];
static stFreqPG* _gpstProgramList=NULL;

static MS_U32 _gu32TPSOFSAT_NUM = 0;

static MS_U8 _gu8TunerIndex = 0;
static MS_U32 _gu32ScanTime = 0;
static MS_U32 _gu32ScanFEType = BROADCAST_TYPE_NOT_SEETING;

//-------------------------------------------------------------------------------------------------
// Local Functions
//-------------------------------------------------------------------------------------------------
static MS_BOOL _Demo_GetProgram(EN_DEMO_DMX_FLOW eDmxFlow, stFreqPG* pstPG)
{
    MS_BOOL bret = TRUE;
    st_PG* pPGTable;

    pPGTable = pstPG->stPgList;
    bret &= Demo_DMX_Scan(eDmxFlow, TRUE);
    bret &= Demo_DMX_GetProgramInfo(&pPGTable, &pstPG->u32PgNum);

    return bret;
}
//--------------------------------------------------------------------------------------------------
/// Get highest RF channel number by country
/// @param  u8Country               \b IN:  country
/// @return the highest RF channel number
//--------------------------------------------------------------------------------------------------
static MS_U8 _Demo_RfChannel_GetMaxRfChNum(MS_U8 u8Country)
{
    switch(u8Country)
    {
    case COUNTRY_TYPE_CUBA:
        return 69;
    case COUNTRY_TYPE_CHILE:
        return 79;
    case COUNTRY_TYPE_PERU:
    case COUNTRY_TYPE_BRAZIL:
        return 69;
    case COUNTRY_TYPE_ARGENTINA:
        return 15;
    case COUNTRY_TYPE_HONGKONG:
    case COUNTRY_TYPE_CHINA:
    case COUNTRY_TYPE_FRANCE:
        return 69;
    case COUNTRY_TYPE_RUSSIAN:
    case COUNTRY_TYPE_UK:
    case COUNTRY_TYPE_EUROPE:
    case COUNTRY_TYPE_GERMANY:
    case COUNTRY_TYPE_ITALY:
        return 70;
    case COUNTRY_TYPE_AUSTRALIA:
        return 76;
    case COUNTRY_TYPE_TAIWAN:
        return 35;
    case COUNTRY_TYPE_OTHER:
        return 100 ;

    default:
        return 0;
    }
}

//--------------------------------------------------------------------------------------------------
/// Get lowest RF channel number by country
/// @param  u8Country               \b IN:  country
/// @return the lowest RF channel number
//--------------------------------------------------------------------------------------------------
static MS_U8 _Demo_RfChannel_GetMinRfChNum(MS_U8 u8Country)
{
    switch(u8Country)
    {
    case COUNTRY_TYPE_CUBA:
    case COUNTRY_TYPE_CHILE:
        return 7;
    //-------------------------//
    case COUNTRY_TYPE_PERU:
    case COUNTRY_TYPE_BRAZIL:
        return 7;
    //-------------------------//
    case COUNTRY_TYPE_ARGENTINA:
        return 14;
    //-------------------------//
    case COUNTRY_TYPE_HONGKONG:
    case COUNTRY_TYPE_CHINA:
    {
        return 21;
    }
    //-------------------------//
    case COUNTRY_TYPE_UK:
       return 21;
    case COUNTRY_TYPE_RUSSIAN:
    case COUNTRY_TYPE_EUROPE:
    case COUNTRY_TYPE_GERMANY:
    case COUNTRY_TYPE_ITALY:
    case COUNTRY_TYPE_FRANCE:
    {
        if(_gu32ScanFEType != DVBC)
            return 5;
        else
            return 21;
    }
    //-------------------------//
    case COUNTRY_TYPE_AUSTRALIA:
    {
        if(_gu32ScanFEType != DVBC)
            return 6;
        else
            return 21;
    }
    //-------------------------//
    case COUNTRY_TYPE_TAIWAN:
    {
        return 24;
    }
    //-------------------------//
    case COUNTRY_TYPE_OTHER:
        return 0;
    default:
        return 0;
    }
}

//--------------------------------------------------------------------------------------------------
/// Get Next RF channel number by country by current Rf Ch
/// @param  u8Country               \b IN:  country
/// @param  u8RF_CH               \b IN:  current RF channel
/// @return the lowest RF channel number
//--------------------------------------------------------------------------------------------------
static MS_U8 _Demo_RfChannel_GetNextRfChNum(MS_U8 u8Country, MS_U8 u8RF_CH)
{
    //--------------------------------------------
    //wyf 20090324 to avoid invalid channel number when manul scan
    MS_U8 u8MaxRFCh,u8MinRFCh;

    u8MaxRFCh = _Demo_RfChannel_GetMaxRfChNum(u8Country);
    u8MinRFCh = _Demo_RfChannel_GetMinRfChNum(u8Country);
    if(u8RF_CH == u8MaxRFCh)
    {
          u8RF_CH = u8MinRFCh;
       return u8RF_CH;
    }
    //---------------------------------------------
    switch (u8Country)
    {
    case COUNTRY_TYPE_CUBA:
    case COUNTRY_TYPE_CHILE:
    case COUNTRY_TYPE_OTHER:
    case COUNTRY_TYPE_PERU:
    case COUNTRY_TYPE_BRAZIL:
    case COUNTRY_TYPE_ARGENTINA:
    {
        if (u8RF_CH < u8MaxRFCh)
        {
            u8RF_CH++;
        }
        else
        {
            u8RF_CH =  u8MinRFCh;
        }
        return u8RF_CH;
    }
    //------------------------------------------//
    case COUNTRY_TYPE_HONGKONG:
    case COUNTRY_TYPE_CHINA:
    {
        if (u8RF_CH < u8MaxRFCh)
        {
            u8RF_CH++;
        }
        else
        {
            u8RF_CH =  21;
        }

        return u8RF_CH;
    }
    //------------------------------------------//
    case COUNTRY_TYPE_RUSSIAN:
    case COUNTRY_TYPE_UK:
    case COUNTRY_TYPE_EUROPE:
    case COUNTRY_TYPE_GERMANY:
    case COUNTRY_TYPE_ITALY:
    case COUNTRY_TYPE_FRANCE:
    {
        if(_gu32ScanFEType != DVBC)
        {
            if (u8RF_CH >= u8MinRFCh && u8RF_CH < 12)
            {
                u8RF_CH++;
            }
            else if (u8RF_CH >= 12 && u8RF_CH < 21)
            {
                u8RF_CH =  21;
            }
            else if (u8RF_CH >= 21 && u8RF_CH < u8MaxRFCh)
            {
                u8RF_CH++;
            }
            else
            {
                u8RF_CH = u8MinRFCh;
            }
        }
        else
        {
            if (u8RF_CH < u8MaxRFCh)
            {
                u8RF_CH++;
            }
            else
            {
                u8RF_CH =  21;
            }
        }
        return u8RF_CH;
    }
    //------------------------------------------//
    case COUNTRY_TYPE_AUSTRALIA:
    {
        if (u8RF_CH < 13)
        {
            u8RF_CH++;
        }
        else if(u8RF_CH >= 13 && u8RF_CH<21)
        {
            u8RF_CH =  21;
        }
        else if (u8RF_CH >= 21 && u8RF_CH < u8MaxRFCh)
        {
            u8RF_CH++;
        }
        else
        {
            u8RF_CH =  u8MinRFCh;
        }

        return u8RF_CH;
    }
    //------------------------------------------//
    case COUNTRY_TYPE_TAIWAN:
    {
        if (u8RF_CH >= 24 && u8RF_CH < 35)
        {
            u8RF_CH++;
        }
        else
        {
            u8RF_CH = 24;
        }
        return u8RF_CH;
    }


    default:
        return 0;
    }
}

//--------------------------------------------------------------------------------------------------
/// Get RF Channel parameters
/// @param  u8RF_CH                 \b IN:  RF channel number
/// @param  u8Country               \b IN:  country
/// @param  u16BandWidth            \b IN:  channel bandwidth (kHz)
/// @param  u32Frequency            \b IN:  the frequency of the RF channel
/// @return TRUE: get the corresponding frequency
///         FALSE: failed to get the frequency.
//--------------------------------------------------------------------------------------------------
static MS_BOOL _Demo_RfChannel_GetTPSetting(MS_U8 u8RF_CH, MS_U8 u8Country, MS_U16 *u16BandWidth, MS_U32 *u32Frequency)
{
    //--------------------------------------------
    //wyf 20090324 to avoid  invalid channel number when manul scan
    MS_U8 u8MaxRFCh,u8MinRFCh;

    u8MaxRFCh = _Demo_RfChannel_GetMaxRfChNum(u8Country);
    u8MinRFCh = _Demo_RfChannel_GetMinRfChNum(u8Country);
    if(u8RF_CH<u8MinRFCh ||u8RF_CH>u8MaxRFCh)
    {
          *u32Frequency = 0;
       return FALSE;
    }

    //---------------------------------------------
    switch(u8Country)
    {
        case COUNTRY_TYPE_CUBA:
            if ((u8RF_CH >= 7) && (u8RF_CH <= 13))   //177 ~ 213 Mhz
            {
                *u32Frequency = 177000L + (u8RF_CH - 7) * 6000L;
                *u16BandWidth = 6000;
            }
            else if ((u8RF_CH >= 14) && (u8RF_CH <= 69))   //473 ~ 803Mhz
            {
                *u32Frequency = 473000L + (u8RF_CH - 14) * 6000L;
                *u16BandWidth = 6000;
            }
            else
            {
                *u32Frequency = 0;
            }
            break;
        case COUNTRY_TYPE_CHILE:
            if ((u8RF_CH >= 14) && (u8RF_CH <= 79))   //473.000 ~ 803.000 Mhz
            {
                *u32Frequency = 473000L + (u8RF_CH - 14) * 6000L;
                *u16BandWidth = 6000;
            }
            else if((u8RF_CH >= 7) && (u8RF_CH <= 13))//177.000~473.000 Mhz
            {
                *u32Frequency = 177000L + (u8RF_CH - 7) * 6000L;
                *u16BandWidth = 6000;
            }
            else
            {
                *u32Frequency = 0;
            }
            break;

    //-------------------------------------//
        case COUNTRY_TYPE_PERU:
        case COUNTRY_TYPE_BRAZIL:
        if ((u8RF_CH >= 14) && (u8RF_CH <= 69))   //473.143 ~ 803.143 Mhz
        {
            *u32Frequency = 473143L + (u8RF_CH - 14) * 6000L;
            *u16BandWidth = 6000;
        }
        else if((u8RF_CH >= 7) && (u8RF_CH <= 13))//177.143~473.143 Mhz
        {
            *u32Frequency = 177143L + (u8RF_CH - 7) * 6000L;
            *u16BandWidth = 6000;
        }
        else
        {
            *u32Frequency = 0;
        }
        break;
        case COUNTRY_TYPE_ARGENTINA:
        {
            if ((u8RF_CH ==14))
            {
                 *u32Frequency = 527000;
                 *u16BandWidth = 6000;
            }
            else
            {
                 *u32Frequency = 647000;
                 *u16BandWidth = 6000;          }
            break;
        }
        break;

    //-------------------------------------//
    case COUNTRY_TYPE_UK:
    case COUNTRY_TYPE_HONGKONG:
    case COUNTRY_TYPE_EUROPE:
    case COUNTRY_TYPE_CHINA:
    case COUNTRY_TYPE_RUSSIAN:
        if ((u8RF_CH >= 5) && (u8RF_CH <= 12))   //177.5 ~ 226.5 Mhz
        {
            *u32Frequency = 177500L + (u8RF_CH - 5) * 7000L;
            *u16BandWidth = 7000;
        }
        else if ((u8RF_CH >= 21) && (u8RF_CH <= 70))   //474 ~ 858 Mhz
        {
            *u32Frequency = 474000L + (u8RF_CH - 21) * 8000L;
            *u16BandWidth = 8000;
        }
        else
        {
            *u32Frequency = 0;
        }
        break;
    //-------------------------------------//
    case COUNTRY_TYPE_GERMANY:
    case COUNTRY_TYPE_FRANCE:
        if ((u8RF_CH >= 5) && (u8RF_CH <= 12))   //177.5 ~ 226.5 Mhz
        {
            *u32Frequency = 177500L + (u8RF_CH - 5) * 7000L;
            *u16BandWidth = 7000;
        }
        else if ((u8RF_CH >= 21) && (u8RF_CH <= 70))   //474 ~ 858 Mhz
        {
            *u32Frequency = 474000L + (u8RF_CH - 21) * 8000L;
            *u16BandWidth = 8000;
        }
        else
        {
            *u32Frequency = 0;
        }
        break;

    case COUNTRY_TYPE_AUSTRALIA:
        {
            MS_S8 s8Index = FREQ_TAL_SIZE -1;

            for(s8Index = FREQ_TAL_SIZE -1;s8Index>=0;s8Index--)
            {
                  if(u8RF_CH >= _gstFreqTbl[s8Index].u8CHNo)
                  {
                        break;
                  }
            }
            if(s8Index>=0&&_gstFreqTbl[s8Index].u32Freq>0)
            {
                  *u32Frequency = _gstFreqTbl[s8Index].u32Freq + (u8RF_CH -_gstFreqTbl[s8Index].u8CHNo) * 7000L;
            }
            else
            {
                  *u32Frequency = 0;
            }

         *u16BandWidth = 7000;
            break;
        }

    //-------------------------------------//
    case COUNTRY_TYPE_ITALY:
    {
     MS_U32 u32FreqTbl[]={177500,186000,194500,203500,212500,219500,226500,233500}; // 5 ~ 12

        if ((u8RF_CH >= 5) && (u8RF_CH <= 12))   //177.5 ~ 226.5 Mhz  6~9 9A~12
        {
            *u32Frequency = u32FreqTbl[u8RF_CH - 5];
             *u16BandWidth = 7000;
        }
        else if ((u8RF_CH >= 21) && (u8RF_CH <= 70))   //474 ~ 858 Mhz
        {
            *u32Frequency = 474000L + (u8RF_CH - 21) * 8000L;
         *u16BandWidth = 8000;
        }
        else
        {
            *u32Frequency = 0;
        }
        break;
    }
    //-------------------------------------//
    case COUNTRY_TYPE_TAIWAN:
        if ((u8RF_CH >= 24) && (u8RF_CH <= 36))   //533 ~ 605 Mhz
        {
            *u32Frequency = 533000L + (u8RF_CH - 24) * 6000L;
            *u16BandWidth = 6000;
        }
        else
        {
            *u32Frequency = 0;
        }
        break;

    //-------------------------------------//

    case COUNTRY_TYPE_OTHER:
        *u32Frequency  = 98000 + (u8RF_CH * 8000L);
        *u16BandWidth = 8000;
    //-------------------------------------//
        break;
    default:
        *u32Frequency = 0;
        break;
    }

    /* Frequency and symbol rate already verified in Scan Menu.
       0 = default value. */
    if (*u32Frequency != 0)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

//--------------------------------------------------------------------------------------------------
/// Get RF channel number by frequency
/// @param  u8Country               \b IN:  country
/// @param  u32Frequency            \b IN:  the frequency of the RF channel
/// @return RF channel number
//--------------------------------------------------------------------------------------------------
MS_U8 _Demo_RfChannel_Freq2ChannelNumber(MS_U8 u8Country, MS_U32 u32Frequency)
{
    MS_U8               u8RF_CH = 0;
    switch(u8Country)
    {
    default:
        u8RF_CH = 0;
        break;
    //-------------------------------------//
    case COUNTRY_TYPE_CHILE:
    case COUNTRY_TYPE_CUBA:
    {
        if (u32Frequency < 473000L)
        {           // 177.143 ~ 473.143
            u8RF_CH = (MS_U8)((u32Frequency - 177000L) / 6000L) + 7;
        }
        else
        {
            u8RF_CH = (MS_U8)((u32Frequency - 473000L) / 6000L) + 14;
        }
        break;
    }

   //-------------------------------------//
    case COUNTRY_TYPE_PERU:
    case COUNTRY_TYPE_BRAZIL:
    {
        if (u32Frequency < 473143L)
        {           // 177.143 ~ 473.143
            u8RF_CH = (MS_U8)((u32Frequency - 177143L) / 6000L) + 7;
        }
        else
        {
            u8RF_CH = (MS_U8)((u32Frequency - 473143L) / 6000L) + 14;
        }
        break;
    }
    //-------------------------------------//
    case COUNTRY_TYPE_ARGENTINA:
    {
        if (u32Frequency ==527000)
        {           // 177.143 ~ 473.143
            u8RF_CH = 14;
        }
        else
        {
            u8RF_CH = 15;
        }

        break;
    }
    //-------------------------------------//
    case COUNTRY_TYPE_HONGKONG:
    case COUNTRY_TYPE_EUROPE:
    case COUNTRY_TYPE_UK:
    case COUNTRY_TYPE_CHINA:
    case COUNTRY_TYPE_RUSSIAN:
    {
        if (u32Frequency < 474000L)
        {           // 177.5 ~ 226.5
            u8RF_CH = (MS_U8)((u32Frequency - 177500L) / 7000L) + 5;
        }
        else
        {
            u8RF_CH = (MS_U8)((u32Frequency - 474000L) / 8000L) + 21;
        }
        break;
    }
    //-------------------------------------//
    case COUNTRY_TYPE_GERMANY:
    case COUNTRY_TYPE_FRANCE:
    {
        if (u32Frequency < 474000L)
        {           // 177.5 ~ 226.5
            u8RF_CH = (MS_U8)((u32Frequency - 177500L) / 7000L) + 5;
        }
        else
        {
            u8RF_CH = (MS_U8)((u32Frequency - 474000L) / 8000L) + 21;
        }
        break;
    }
    //-------------------------------------//
    case COUNTRY_TYPE_ITALY:
    {
     MS_U32 u32FreqTbl[]={177500,186000,194500,203500,212500,219500,226500,233500}; // 5 ~ 12
     MS_U16 i;
        if(u32Frequency < 474000L)
        {
            for(i=0;i<sizeof(u32FreqTbl)/sizeof(MS_U32);i++)
            {
                if(u32Frequency <= u32FreqTbl[i])
                {
                    u8RF_CH = 5 + i;
                    break;
                }
            }
        }
        else
        {
            u8RF_CH = (MS_U8)((u32Frequency - 474000L) / 8000L) + 21;
        }
        break;
    }
    //-------------------------------------//
    case COUNTRY_TYPE_AUSTRALIA:
    {
        MS_S8 s8Index = FREQ_TAL_SIZE -1;

        for(s8Index = FREQ_TAL_SIZE -1;s8Index>=0;s8Index--)
        {
              if(_gstFreqTbl[s8Index].u32Freq>0&&u32Frequency >= _gstFreqTbl[s8Index].u32Freq)
              {
                    break;
              }
        }
        if(s8Index >= 0)
        {
              u8RF_CH = (MS_U8)((u32Frequency -_gstFreqTbl[s8Index].u32Freq) / 7000L)+_gstFreqTbl[s8Index].u8CHNo;
        }
        else
        {
             u8RF_CH = _gstFreqTbl[0].u8CHNo;
        }
        break;
    }

    //-------------------------------------//
    case COUNTRY_TYPE_TAIWAN:
    {
        //if (u8RF_CH >= 24 && u8RF_CH <= 36)
        if (u32Frequency >= 533000L && u32Frequency <= 605000L)
        {            //533 ~ 605 Mhz
            u8RF_CH = (MS_U8)((u32Frequency - 533000L) / 6000L) + 24;
        }
        break;
    }
    //-------------------------------------//
    case COUNTRY_TYPE_OTHER:
        u8RF_CH = (MS_U8)((u32Frequency-98000 )/8000) ;
        break;
    //-------------------------------------//
    }

    return u8RF_CH;
}

#if   DISEQC13_ONLY_GOTOX
static MS_BOOL _Demo_TurnMotor(MS_SAT_PARAM* pstSatParam, MS_BOOL bForceTune)
{
    MS_BOOL bRet = FALSE;

    //_GetSatInfor(_ScanParam.pu8SatIDList[_gu8CurScanSATPos],&stSatParam);
    if(_gpstSat_param == NULL)
    {
        if(!appDemo_DigiTuner_GetSatParam(EN_CABLE_LNB_NOT_SET, &_gpstSat_param))
            return FALSE;
    }
    memcpy((void*)pstSatParam,(void*)_gpstSat_param,sizeof(MS_SAT_PARAM));
    if(pstSatParam->u8Position > 0)
    {
        if(pstSatParam->u8Position & DISEQC_USALS_BIT)
        {
            if(bForceTune&&!(_gu8MotorPosition&DISEQC_USALS_BIT))
            {
              MApi_DigiTuner_DiSEqC_GoSatPos(pstSatParam);
            }
            bRet = TRUE;
        }
        else if((pstSatParam->u8Position&DISEQC12_USALS_POSITION_MASK) != (_gu8MotorPosition&DISEQC12_USALS_POSITION_MASK))
        {
            if(bForceTune)
            {
              MApi_DigiTuner_DiSEqC_GoSatPos(pstSatParam);
            }
            bRet = TRUE;
        }
    }
    _gu8MotorPosition = pstSatParam->u8Position;
    return bRet;
}
#else
static MS_BOOL _Demo_TurnMotor(MS_SAT_PARAM* pstSatParam, MS_BOOL bForceTune)
{
    MS_BOOL bRet = FALSE;

    //_GetSatInfor(_gu8CurScanSATPos,&stSatParam);
    if(_gpstSat_param == NULL)
    {
        if(!appDemo_DigiTuner_GetSatParam(EN_CABLE_LNB_NOT_SET, &_gpstSat_param))
            return FALSE;
    }
    memcpy((void*)pstSatParam,(void*)_gpstSat_param,sizeof(MS_SAT_PARAM));
    if((pstSatParam->u8Position&DISEQC12_USALS_POSITION_MASK) > 0)
    {
        if((pstSatParam->u8Position&DISEQC12_USALS_POSITION_MASK) != _gu8MotorPosition)
        {
            if(bForceTune)
            {
              MApi_DigiTuner_DiSEqC_GoSatPos(_gu8TunerIndex, pstSatParam->u8Position&DISEQC12_USALS_POSITION_MASK);
            }
            bRet = TRUE;
        }
    }
    _gu8MotorPosition = pstSatParam->u8Position&DISEQC12_USALS_POSITION_MASK;
    return bRet;
}
#endif

static MS_BOOL _BlindScan_AddTP2List(MS_FE_CARRIER_PARAM* TP)
{
    MS_U32 index;
    for(index =0; index < _gu32TPSOFSAT_NUM; index++)
    {
        if((_gstAfe_param[index].u32Frequency == TP->u32Frequency) && (_gstAfe_param[index].SatParam.u8Polarity == TP->SatParam.u8Polarity))
        {
            return TRUE;
        }
    }

    if(_gu32TPSOFSAT_NUM<MAX_TPSOFSAT_NUM)
    {
        memcpy((void*)&(_gstAfe_param[_gu32TPSOFSAT_NUM]),(void*)TP,sizeof(MS_FE_CARRIER_PARAM));

        if(E_SCAN_TYPE_SCAN_EACH_TP_AND_ITS_PG == _eBlindScanType)
        {
            memcpy(&_gstScanParam.stTPSInfo,&_gstScanParam.pstTPIdxTable[_gu32TPSOFSAT_NUM],sizeof(MS_FE_CARRIER_PARAM));
        }

        _gu32TPSOFSAT_NUM ++;
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

static MS_FE_CARRIER_PARAM* _BlindScan_GetTP2List(MS_U32 u32Index)
{
    if(u32Index < _gu32TPSOFSAT_NUM)
    {
        return &(_gstAfe_param[u32Index]);
    }
    else
    {
        return NULL;
    }
}

static MS_U32 _BlindScan_GetTP2ListNum(void)
{
    return _gu32TPSOFSAT_NUM;
}

static MS_BOOL _BlindScan_RestTP2List(void)
{
    _gu32TPSOFSAT_NUM = 0;
    memset((void *)&_gstAfe_param,0x0,sizeof(MS_FE_CARRIER_PARAM)*MAX_TPSOFSAT_NUM);
    return TRUE;
}

static TaskState _Demo_ChBlindScan_Start(void)
{
    MS_U8 u8Status;
    MS_U8 u8Polarity = 0;
    MS_BOOL bHiLoF = FALSE;

    u8Status = _geBlindScanStatus;
    u8Status ++;
    u8Status %= BLINDSCAN_STATUS_NUM;

    if(_gpstSat_param == NULL)
    {
        if(!appDemo_DigiTuner_GetSatParam(EN_CABLE_LNB_NOT_SET, &_gpstSat_param))
            return E_TASK_STATE_IDLE;
    }

    if(_gpstSat_param->eSwt10Port == EN_SWT_PORT_OFF
      &&_gpstSat_param->eSwt11Port == EN_SWT_PORT_OFF)
    {
        if(_gpstSat_param->eLNBPwrOnOff == EN_LNBPWR_13V)
        {
            if(u8Status == BLINDSCAN_H_LoLOF
            ||u8Status == BLINDSCAN_H_HiLOF)
            {
                u8Status ++;
            }
        }
        else if(_gpstSat_param->eLNBPwrOnOff == EN_LNBPWR_18V)
        {
            if(u8Status == BLINDSCAN_V_LoLOF
            ||u8Status == BLINDSCAN_V_HiLOF)
            {
                u8Status ++;
            }
        }
    }

    if(_gpstSat_param->u16LoLOF == _gpstSat_param->u16HiLOF)
    {
        if(u8Status >= BLINDSCAN_H_HiLOF)
        {
            u8Status = BLINDSCAN_GETTP_FINISH;
        }
    }
    else if(_gpstSat_param->u16LoLOF < MAX_C_LOF_FREQ) //c band   L-LOF for H  H-LOF for V
    {
        if(u8Status == BLINDSCAN_V_LoLOF
        ||u8Status == BLINDSCAN_H_HiLOF)
        {
            u8Status = BLINDSCAN_V_HiLOF;
            if(_gpstSat_param->eLNBPwrOnOff == EN_LNBPWR_18V)
            {
               u8Status = BLINDSCAN_GETTP_FINISH;
            }
        }
    }

    _geBlindScanStatus = (BLINDSCAN_STATUS)u8Status;
    switch(_geBlindScanStatus)
    {
        case BLINDSCAN_H_LoLOF:
            u8Polarity = 0;
            bHiLoF = FALSE;
            break;
        case BLINDSCAN_V_LoLOF:
            u8Polarity = 1;
            bHiLoF = FALSE;
            break;
        case BLINDSCAN_H_HiLOF:
            u8Polarity = 0;
            bHiLoF = TRUE;
            break;
        case BLINDSCAN_V_HiLOF:
            u8Polarity = 1;
            bHiLoF = TRUE;
            break;
        case BLINDSCAN_GETTP_FINISH:
            return E_TASK_STATE_BLIND_END;
            break;
        default:
            return E_TASK_STATE_IDLE;
            break;
    }

    if(MApi_DigiTuner_BlindScan_Start(_gu8TunerIndex, _gpstSat_param,u8Polarity,bHiLoF))
    {
        _gu16GetTPSNum = 0;

        _Demo_TurnMotor(_gpstSat_param,FALSE);
        return E_TASK_STATE_BLIND_NEXTFREQ;
    }

    return E_TASK_STATE_IDLE;
}

static TaskState _Demo_ChBlindScan_NextFreq(void)
{
    MS_BOOL bBlindScanEnd = FALSE;

    _gu8TPBlindScanProgress = 0;
    if(MApi_DigiTuner_BlindScan_NextFreq(_gu8TunerIndex, &bBlindScanEnd))
    {
        if(bBlindScanEnd)
        {
            return E_TASK_STATE_BLIND_START;
        }
        else
        {
            return E_TASK_STATE_BLIND_WAITFINISH;
        }
    }

    return E_TASK_STATE_IDLE;
}

static TaskState _Demo_ChBlindScan_WaitFinished(void)
{
    MS_U8 u8Find = 0;

    if(MApi_DigiTuner_BlindScan_WaitCurFeqFinished(_gu8TunerIndex, &_gu8TPBlindScanProgress,&u8Find))
    {
        if(_gu8TPBlindScanProgress == 100)
        {
            if((u8Find -_gu16GetTPSNum)> 0)
            {
                return E_TASK_STATE_BLIND_GETTPLIST;
            }
            else
            {
                return E_TASK_STATE_BLIND_NEXTFREQ;
            }
        }
        else
        {
            return E_TASK_STATE_BLIND_WAITFINISH;
        }
    }

    return E_TASK_STATE_IDLE;
}

static TaskState _Demo_ChBlindScan_GetTPlist(void)
{
    MS_U16 u16TPNum = MAX_TPS_LPF,idx=0;
    MS_FE_CARRIER_PARAM carrierTPList[MAX_TPS_LPF];
    MS_BOOL bAddTPStatus = FALSE;

    memset(carrierTPList, 0, sizeof(MS_FE_CARRIER_PARAM)*MAX_TPS_LPF);//coverity issue.
    if(MApi_DigiTuner_BlindScan_GetChannel(_gu8TunerIndex, _gu16GetTPSNum,&u16TPNum, carrierTPList))
    {
        if(u16TPNum > 0)
        {
            MS_U8 u8Polarity = 0;
            MS_U16 u16LOFFreq;
            MS_SAT_PARAM SATParam;

            //_GetSatInfor(_gu8CurScanSATPos,&SATParam);
            memcpy((void*)&SATParam,(void*)_gpstSat_param,sizeof(SATParam));

            if(_geBlindScanStatus == BLINDSCAN_V_LoLOF || _geBlindScanStatus == BLINDSCAN_V_HiLOF)
            {
                u8Polarity = 1;
            }
            if(_geBlindScanStatus == BLINDSCAN_V_HiLOF || _geBlindScanStatus == BLINDSCAN_H_HiLOF)
            {
                u16LOFFreq = SATParam.u16HiLOF;
            }
            else
            {
                u16LOFFreq = SATParam.u16LoLOF;
            }

            for(idx = 0;idx < u16TPNum;idx ++)
            {
                if(u16LOFFreq <MAX_C_LOF_FREQ) //C band
                {
                    carrierTPList[idx].u32Frequency = u16LOFFreq -carrierTPList[idx].u32Frequency ;
                }
                else
                {
                    carrierTPList[idx].u32Frequency += u16LOFFreq;
                }

                carrierTPList[idx].SatParam.u8Polarity = u8Polarity;
                carrierTPList[idx].SatParam.u8SatID = _gu8CurScanSATPos;
                carrierTPList[idx].SatParam.eRollOff = SAT_RO_20;

                printf("_BlindScan_AddTP2List u32Frequency = %"DTC_MS_U32_u" \n", carrierTPList[idx].u32Frequency);
                printf("_BlindScan_AddTP2List u16SymbolRate = %d \n", (int)carrierTPList[idx].SatParam.u16SymbolRate);
                printf("_BlindScan_AddTP2List Is_DVBS2 = %x \n", carrierTPList[idx].SatParam.bIs_DVBS2);

                bAddTPStatus = _BlindScan_AddTP2List(&carrierTPList[idx]);
            }
            _gu16GetTPSNum += u16TPNum;
            //_gstScanParam.u32TPSCount = _gu16GetTPSNum;
            if(E_SCAN_TYPE_SCAN_EACH_TP_AND_ITS_PG == _eBlindScanType)
            {
               if(bAddTPStatus)
                    return E_TASK_STATE_TUNE2RF;
            }
        }
        return E_TASK_STATE_BLIND_NEXTFREQ;
    }

    return E_TASK_STATE_IDLE;
}

static TaskState _Demo_ChBlindScan_End(void)
{
    MApi_DigiTuner_BlindScan_Cancel(_gu8TunerIndex);
    if(MApi_DigiTuner_BlindScan_End(_gu8TunerIndex))
    {
        printf("%s: Scan time = %"DTC_MS_U32_d"ms.\n", __FUNCTION__, MsOS_GetSystemTime() - _gu32ScanTime);
        if(E_SCAN_TYPE_SCAN_EACH_TP_AND_ITS_PG == _eBlindScanType)
            return E_TASK_STATE_IDLE;
        else
            return E_TASK_STATE_START;
    }

    return E_TASK_STATE_IDLE;
}

//-------------------------------------------------------------------------------------------------
/// E_TASK_STATE_START state handler
/// @return Next auto scan state
//-------------------------------------------------------------------------------------------------
static TaskState _Demo_ChScan_Start(void)
{
    MS_U32 u32BW;
    MS_U8 u8CurChNo;
    MS_U16 u16Bandwidth = 0;
    MS_U32 u32NULLArg=0;

    if (_gstScanParam.bScanMode == E_CHSCAN_MODE_CHANNEL_NO)
    {

        u8CurChNo = _Demo_RfChannel_Freq2ChannelNumber(_gstScanParam.eCountry, _gstScanParam.u32StartFreq);
        if ( (_Demo_RfChannel_GetTPSetting(u8CurChNo, _gstScanParam.eCountry, &u16Bandwidth, &(_gstScanParam.stTPSInfo.u32Frequency)) == TRUE) &&
                (_gstScanParam.stTPSInfo.u32Frequency <= _gstScanParam.u32LastFreq) )
        {
            _gstScanParam.stTPSInfo.u32Frequency = _gstScanParam.u32StartFreq;
        }
        else
        {
            return E_TASK_STATE_IDLE;
        }
    }
    else if (_gstScanParam.bScanMode == E_CHSCAN_MODE_TPSIDX_TABLE)
    {
       _gstScanParam.u32TPSCount = _BlindScan_GetTP2ListNum();
       if(_gstScanParam.u32TPSCount >0)
        {
             _gu32NumOfChanScan = 0;
              memcpy(&_gstScanParam.stTPSInfo,&_gstScanParam.pstTPIdxTable[0],sizeof(MS_FE_CARRIER_PARAM));
              _gpstProgramList = (stFreqPG*)malloc(sizeof(stFreqPG)*_gstScanParam.u32TPSCount);
              if(_gpstProgramList == NULL)
                  free(_gpstProgramList);
              else
                  memset(_gpstProgramList, 0x0, sizeof(stFreqPG)*_gstScanParam.u32TPSCount);
        }
       //else if(_gstScanParam.pu8SatIDList[_u8CurScanSATPos]!=INVALID_SAT_ID )
       // {
       //      return E_TASK_STATE_SETTPS;
       // }
        else
        {
             return E_TASK_STATE_IDLE;
        }
    }

    MS_U32 u32SymbolRate = _gstScanParam.stTPSInfo.CabParam.u16SymbolRate;
    if(_gu32ScanFEType == DVBC)
    {
        if(!appDemo_DigiTuner_SetFreqCheck(&_gstScanParam.stTPSInfo.u32Frequency, (MS_U32*)&_gstScanParam.stTPSInfo.CabParam.eConstellation, &u32SymbolRate))
        {
            printf("%s: Fail!!!\n", __FUNCTION__);
            return E_TASK_STATE_IDLE;
        }
    }
    else if(_gu32ScanFEType == DVBS)
    {
        if(!appDemo_DigiTuner_SetFreqCheck(&_gstScanParam.stTPSInfo.u32Frequency, &u32SymbolRate, &u32NULLArg))
        {
            printf("%s: Fail!!!\n", __FUNCTION__);
            return E_TASK_STATE_IDLE;
        }
    }else if(_gu32ScanFEType == DVBT2)
    {
         u32BW = (MS_U32)((u16Bandwidth - 6000)/1000 + 1);// 1: 6M, 2:7M,  3:8M
        if(!appDemo_DigiTuner_SetFreqCheck(&_gstScanParam.stTPSInfo.u32Frequency, &u32BW, &u32NULLArg))
        {
            printf("%s: Fail!!!\n", __FUNCTION__);
            return E_TASK_STATE_IDLE;
        }
    }


    if(_gu32ScanFEType != DVBS)
        _gu32ScanTime = MsOS_GetSystemTime();

    return E_TASK_STATE_TUNE2RF;
}

//-------------------------------------------------------------------------------------------------
/// E_TASK_STATE_TUNE2RF state handler
/// @return Next auto scan state
//-------------------------------------------------------------------------------------------------
static TaskState _Demo_ChScan_Tune2RF(void)
{
    printf("%s: %"DTC_MS_U32_d"KHz\n", __FUNCTION__, _gstScanParam.stTPSInfo.u32Frequency);
    MS_U32 u32FrontendType = MApi_DigiTuner_GetFrontendType(_gu8TunerIndex);

    _gu32LockTime = MsOS_GetSystemTime();

    if(_gu32ScanFEType == DVBT2)
    {
        // Tune to RF channel
        if(FE_TUNE_AUTO  == _gstScanParam.eAutoTune)
        {
            if (TRUE == _gbScanLock)      //demod type is t2, and locked in auto scan mode
            {
                _gbScanLock = FALSE;   //clear the flag
                _gu8PlpSize = MApi_DigiTuner_GetPlpIDList(_gu8TunerIndex);    //get plp id list
                if(_gu8PlpSize >= 1 )
                {
                    _gstScanParam.stTPSInfo.TerParam.u8PlpID = MApi_DigiTuner_GetNextPlpID(_gu8TunerIndex, _gu8PlpIndex);  //get the first plp id
                    _gu8PlpIndex++;
                }
            }

            if(_gstScanParam.stTPSInfo.TerParam.u8ScanType == E_DEMOD_TYPE_T2 && u32FrontendType != DVBT2)
            {
                MApi_Frontend_SetBroadcastType(_gu8TunerIndex, DVBT2);
            }
            else if(_gstScanParam.stTPSInfo.TerParam.u8ScanType == E_DEMOD_TYPE_T && u32FrontendType != DVBT)
            {
                MApi_Frontend_SetBroadcastType(_gu8TunerIndex, DVBT);
            }

            if(!appDemo_DigiTuner_Tune2RF(&_gstScanParam.stTPSInfo))
            {
                printf("%s: failed.\n", __FUNCTION__);
                return E_TASK_STATE_IDLE;
            }
        }
    }
    else if(_gu32ScanFEType == DVBS)
    {
        if(_gstScanParam.stTPSInfo.SatParam.bIs_DVBS2)
            MApi_Frontend_SetBroadcastType(_gu8TunerIndex, DVBS2);
        else
            MApi_Frontend_SetBroadcastType(_gu8TunerIndex, DVBS);

        if(!MApi_DigiTuner_Tune2RfCh_DVBS(_gu8TunerIndex, _gpstSat_param, &_gstScanParam.stTPSInfo, FE_TUNE_AUTO))
        {
            printf("%s: failed.\n", __FUNCTION__);
            return E_TASK_STATE_IDLE;
        }
    }
    else
    {
        // Tune to RF channel
        if((_gstScanParam.eAutoTune != FE_TUNE_MANUAL) &&
           (!appDemo_DigiTuner_Tune2RF(&_gstScanParam.stTPSInfo)))
        {
            printf("%s: failed.\n", __FUNCTION__);
            return E_TASK_STATE_IDLE;
        }
    }

    MApi_DigiTuner_GetMaxLockTime(_gu8TunerIndex, _gstScanParam.eAutoTune, &_gu32LockTimeMax);

    return E_TASK_STATE_WAITLOCK;
}

//-------------------------------------------------------------------------------------------------
/// E_TASK_STATE_WAITLOCK state handler
/// @return Next auto scan state
//-------------------------------------------------------------------------------------------------
static TaskState _Demo_ChScan_WaitLock(void)
{
    EN_LOCK_STATUS eStatus = E_DEMOD_CHECKING;
    stFreqPG* pstPG = NULL;
#if (CH_SCAN_SUPPORT_ATSC == 1)
    if(_gu32ScanFEType == ATSC)
        if(_Check_ATSC_SyncLock(_gu8TunerIndex, &eDemodstatus))
            if( E_DEMOD_LOCK != eDemodstatus) //CHANNEL EMPTY
            {
                printf("ATSC channel is empty\n");
                return FALSE;
            }
#endif
    if(!MApi_DigiTuner_GetLock(_gu8TunerIndex, &eStatus))
    {
        return FALSE;
    }

    if ( eStatus == E_DEMOD_LOCK)
    {
        if(_gu32ScanFEType == DVBT2)
        {
            if((E_DEMOD_TYPE_T2 == MApi_DigiTuner_GetCurrentDemodType(_gu8TunerIndex)) && (0 == _gu8PlpSize))
            {
                _gbScanLock = TRUE;    //to get t2 plp id list
                return E_TASK_STATE_TUNE2RF;
            }
        }

        printf("%s: CH %"DTC_MS_U32_d" found, LockTime = %"DTC_MS_U32_d".\n", __FUNCTION__,
            _gstScanParam.stTPSInfo.u32Frequency, MsOS_GetSystemTime() - _gu32LockTime);

        //get program
        if(_gu32ScanFEType == DVBS)
        {
           if(_gpstProgramList == NULL)
               return E_TASK_STATE_IDLE;
           else
               pstPG = _gpstProgramList + _gu32NumOfChanScan;

           if(!_Demo_GetProgram(E_DMX_FLOW_LIVE0, pstPG))
              pstPG->u32PgNum = 0;


           if(E_SCAN_TYPE_SCAN_EACH_TP_AND_ITS_PG == _eBlindScanType)
                _gu32NumOfChanScan++;
        }
        // Next Channel
        if((E_SCAN_TYPE_SCAN_EACH_TP_AND_ITS_PG == _eBlindScanType) && (_gu32ScanFEType == DVBS))
        {
            //MApi_DigiTuner_Demod_ClearStatus(_gu8TunerIndex);
            return E_TASK_STATE_BLIND_NEXTFREQ;
        }
        else
            return E_TASK_STATE_SETTPS;
    }
    else if ( /*(eStatus == E_DEMOD_UNLOCK)  || */((MsOS_GetSystemTime() - _gu32LockTime) > _gu32LockTimeMax)
              || (eStatus == E_DEMOD_T_T2_UNLOCK))
    {
        printf("%s: Timeout!\n", __FUNCTION__);

        if(_gu32ScanFEType == DVBT2)
        {
            if((FE_TUNE_AUTO == _gstScanParam.eAutoTune) &&(0 == MApi_DigiTuner_GetScanTypeStatus(_gu8TunerIndex)))
            {
                _gstScanParam.stTPSInfo.TerParam.u8PlpID = 0xff;

                if ( eStatus == E_DEMOD_T_T2_UNLOCK )
                {
                    return E_TASK_STATE_SETTPS;
                }
                else
                {
                    MApi_DigiTuner_SetScanTypeStatus(_gu8TunerIndex, 1);
                    if(E_DEMOD_TYPE_T == MApi_DigiTuner_GetCurrentDemodType(_gu8TunerIndex))
                        _gstScanParam.stTPSInfo.TerParam.u8ScanType = E_DEMOD_TYPE_T2;
                    else
                        _gstScanParam.stTPSInfo.TerParam.u8ScanType = E_DEMOD_TYPE_T;

                    return E_TASK_STATE_TUNE2RF;
                }
            }
        }
        if((E_SCAN_TYPE_SCAN_EACH_TP_AND_ITS_PG == _eBlindScanType) && (_gu32ScanFEType == DVBS))
        {
            _gu32NumOfChanScan++;
            return E_TASK_STATE_BLIND_NEXTFREQ;
        }
        else
            return E_TASK_STATE_SETTPS;
    }
    return E_TASK_STATE_WAITLOCK;
}

//-------------------------------------------------------------------------------------------------
/// E_TASK_STATE_SETTPS state handler
/// @return Next auto scan state
//-------------------------------------------------------------------------------------------------
static TaskState _Demo_ChScan_SetTPS(void)
{
    if(_gu32ScanFEType == DVBT2)
    {
        if((_gu8PlpSize  >= 1) && (E_DEMOD_TYPE_T2 == MApi_DigiTuner_GetCurrentDemodType(_gu8TunerIndex)))
        {
            if(_gu8PlpIndex < _gu8PlpSize)
            {
                _gstScanParam.stTPSInfo.TerParam.u8PlpID =  MApi_DigiTuner_GetNextPlpID(_gu8TunerIndex, _gu8PlpIndex);
                _gu8PlpIndex++;

                return E_TASK_STATE_TUNE2RF;
            }
            else if(_gu8PlpIndex >= _gu8PlpSize)
            {
                _gu8PlpIndex = 0;
                _gu8PlpSize = 0;
            }
        }
    }

    // Get next RF channel frequency
    if (_gstScanParam.bScanMode == E_CHSCAN_MODE_CHANNEL_NO)
    {
        MS_U8 u8CurChNo;
        MS_U16 u16Bandwidth;
#if (TEDDY_CH_SCAN_DONE == 1)
        MS_U32 u32DemodType;
        MApi_DigiTuner_GetDemodType(_gu8TunerIndex, &u32DemodType);
        if (u32DemodType == DEMOD_MSB1236C ||
            (u32DemodType == DEMOD_MSB124X && _gu32ScanFEType == DVBT2))
            _gstScanParam.stTPSInfo.TerParam.u8ScanType = E_DEMOD_TYPE_T;//for auto search time optimization
#endif

        u8CurChNo = _Demo_RfChannel_Freq2ChannelNumber(_gstScanParam.eCountry, _gstScanParam.stTPSInfo.u32Frequency);
        if (u8CurChNo < _Demo_RfChannel_GetMaxRfChNum(_gstScanParam.eCountry))
        {
            u8CurChNo = _Demo_RfChannel_GetNextRfChNum(_gstScanParam.eCountry, u8CurChNo);
        }
        else
        {
            u8CurChNo                       = ~0;
            _gstScanParam.stTPSInfo.u32Frequency = ~0;
        }

        if ((_Demo_RfChannel_GetTPSetting(u8CurChNo, _gstScanParam.eCountry, &u16Bandwidth, &(_gstScanParam.stTPSInfo.u32Frequency)) == TRUE)
                && (_gstScanParam.stTPSInfo.u32Frequency <= _gstScanParam.u32LastFreq))
        {
            if(_gstScanParam.eTvSysType == E_TV_DVBT)
            {
                if(_gu32ScanFEType == DVBT2)
                    MApi_DigiTuner_InitParameter(_gu8TunerIndex);

                _gstScanParam.stTPSInfo.TerParam.eBandWidth = (EN_TER_BW_MODE)(BW_MODE_6MHZ + (u16Bandwidth / 1000 - 6));
            }
            _gu32NumOfChanScan++;
        }
        else
        {
            if(_gu32ScanFEType != DVBS)
                printf("%s: Scan time = %"DTC_MS_U32_d"ms.\n", __FUNCTION__, MsOS_GetSystemTime() - _gu32ScanTime);

            printf("%s: Last RF channel %"DTC_MS_U32_d".\n", __FUNCTION__, _gstScanParam.stTPSInfo.u32Frequency);
            return E_TASK_STATE_IDLE;
        }
    }
    else if (_gstScanParam.bScanMode == E_CHSCAN_MODE_TPSIDX_TABLE)
    {
        _gu32NumOfChanScan++;
        if (_gu32NumOfChanScan < _gstScanParam.u32TPSCount)
        {
             memcpy(&_gstScanParam.stTPSInfo,&_gstScanParam.pstTPIdxTable[_gu32NumOfChanScan],sizeof(MS_FE_CARRIER_PARAM));
        }
        else
        {
              _gu32NumOfChanScan --; // avoid to get invalid number
              return E_TASK_STATE_IDLE;
        }
    }

    return E_TASK_STATE_TUNE2RF;
}

//-------------------------------------------------------------------------------------------------
/// E_TASK_STATE_IDLE state handler
/// @return Next auto scan state
//-------------------------------------------------------------------------------------------------
static TaskState _Demo_ChScan_Idle(void)
{

    return E_TASK_STATE_IDLE;
}

//-------------------------------------------------------------------------------------------------
/// Start channel scan process
/// @param  pScanParam                      \b IN: setting parameters for scan process
/// @return @ref ChScan_Result
//-------------------------------------------------------------------------------------------------
static MS_BOOL _Demo_ChanScan_Start(ChScan_Param* pstScanParam)
{
    memcpy(&_gstScanParam, pstScanParam, sizeof(ChScan_Param));

    if(E_SCAN_TYPE_SCAN_EACH_TP_AND_ITS_PG == _eBlindScanType)
        _gu32NumOfChanScan = 0;
    else
        _gu32NumOfChanScan = 1;

    _eChScanState = E_TASK_STATE_START;
    _geBlindScanStatus = BLINDSCAN_READY;
    _gu16GetTPSNum = 0;
    _gu8TPBlindScanProgress = 0;//the progress of blind scan 1 frequency
    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Get state function for terrestrial
/// @param eState \b IN: CHannel scan state
/// @return Channel scan state function for terrestrial
//-------------------------------------------------------------------------------------------------
static TaskStateFunc _Demo_ChScan_StateFunc(TaskState eState)
{
    switch (eState)
    {
    case E_TASK_STATE_BLIND_START:
        return _Demo_ChBlindScan_Start;
    case E_TASK_STATE_BLIND_NEXTFREQ:
        return _Demo_ChBlindScan_NextFreq;
    case E_TASK_STATE_BLIND_WAITFINISH:
        return _Demo_ChBlindScan_WaitFinished;
    case E_TASK_STATE_BLIND_GETTPLIST:
        return _Demo_ChBlindScan_GetTPlist;
    case E_TASK_STATE_BLIND_END:
        return _Demo_ChBlindScan_End;
    case E_TASK_STATE_START:
        return _Demo_ChScan_Start;
    case E_TASK_STATE_TUNE2RF:
        return _Demo_ChScan_Tune2RF;
    case E_TASK_STATE_WAITLOCK:
        return _Demo_ChScan_WaitLock;
    case E_TASK_STATE_SETTPS:
        return _Demo_ChScan_SetTPS;
    case E_TASK_STATE_IDLE:
        return _Demo_ChScan_Idle;

    default:
        printf("%s: Invalid State!\n", __FUNCTION__);
        return _Demo_ChScan_Idle;
    }
}

//------------------------------------------------------------------------------
/// @brief The sample code to do DVB-S blind scan
/// @param none
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
//------------------------------------------------------------------------------
MS_BOOL _Demo_DigiTuner_BlindScan(void)
{
    _gu8TunerIndex = appDemo_DigiTuner_GetIndex();
    _eChScanState = E_TASK_STATE_BLIND_START;
    MS_U32 u32TpNum = 0;
    MS_U32 u32index;
    MS_FE_CARRIER_PARAM* pTP;
    stFreqPG* pPG = NULL;
    MS_U16 u16PGSum = 0;
    MS_U32 u32StartTime = 0;


    _gu8CurScanSATPos = 0;

    if(_gpstSat_param == NULL)
    {
        if(!appDemo_DigiTuner_GetSatParam(EN_CABLE_LNB_NOT_SET, &_gpstSat_param))
            return FALSE;
    }

    memcpy(_gpstSat_param->aSatName,AntennaSatList_BuiltIn[_gu8CurScanSATPos].pSatName,strlen((char *)AntennaSatList_BuiltIn[_gu8CurScanSATPos].pSatName));
/*
    _gstSat_param.e0V12VOnOff = EN_0V12V_ONOFF_ON;
    _gstSat_param.e22KOnOff = EN_22K_AUTO;    // effective
    _gstSat_param.eDiseqcLevel = EN_DISEQC_LVL_OFF;
    _gstSat_param.eLNBPwrOnOff = EN_LNBPWR_13OR18V;
    _gstSat_param.eLNBType = AntennaSatList_BuiltIn[_gu8CurScanSATPos].u16Band; // Sidney T.B.D
    _gstSat_param.eSwt10Port = EN_SWT_PORT_OFF;
    _gstSat_param.eSwt11Port = EN_SWT_PORT_OFF;
    _gstSat_param.eToneburstType = EN_TONEBURST_NONE;
    _gstSat_param.u16Angle = AntennaSatList_BuiltIn[_gu8CurScanSATPos].u16Angle;
    _gstSat_param.u16HiLOF = 10600;
    _gstSat_param.u16LoLOF = 9750;
    _gstSat_param.u8Position = 0;
*/
    if(MApi_DigiTuner_Init((MS_U8)(_gu8TunerIndex))== FALSE)
    {
        printf("MApi_DigiTuner_Init fail \n");
        return FALSE;
    }

    _BlindScan_RestTP2List();
    u32StartTime = MsOS_GetSystemTime();

    while(_eChScanState != E_TASK_STATE_IDLE)
    {
        _eChScanState = _Demo_ChScan_StateFunc(_eChScanState)();
        //Currently only support first half of blind scan
        //if (E_TASK_STATE_START == _eChScanState)
        //    break;

        if (_eChScanState== E_TASK_STATE_WAITLOCK)
             MsOS_DelayTask(CH_SCAN_DELAY);
    }

    u32TpNum = _BlindScan_GetTP2ListNum();
    for(u32index = 0;u32index< u32TpNum;u32index++)
    {
        pTP = _BlindScan_GetTP2List(u32index);
        if(_gpstProgramList != NULL)
            pPG = _gpstProgramList + u32index;

        if(pTP != NULL && pPG != NULL)
        {
            u16PGSum += pPG->u32PgNum;
            printf("Frequency = %"DTC_MS_U32_u" SR = %d Polarity = %x  Program Count = %d\n", pTP->u32Frequency,\
            (int)pTP->SatParam.u16SymbolRate,pTP->SatParam.u8Polarity, (int)pPG->u32PgNum);
        }
    }

    printf(" Total TP = %d, Total Program = %d\n", (int)u32TpNum, (int)u16PGSum);
    printf("===== Blind scan complete ====\n");
    printf("===== Scan Time = %d (ms) ====\n", (int)(MsOS_GetSystemTime() - u32StartTime));

    return TRUE;
}

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
MS_BOOL Demo_ChScan_Auto(MS_U32* puCountry, MS_U32* pu32Arg1, MS_U32* pu32Arg2)
{
    _gu8TunerIndex = appDemo_DigiTuner_GetIndex();
    ChScan_Param stScanParam;
    MS_U16 u16BandWidth = 0;
    MS_U8 u8RFCh = 0;
    MS_U32 u32FullStartFrequency = 115000;
    MS_U32 u32FullEndFrequency = 858000;
    _gu32ScanFEType = MApi_DigiTuner_GetFrontendType(_gu8TunerIndex);

    //Currently only support DVBC, DVBT2, ISDBT, DVBS(/DVBS2)
    if((_gu32ScanFEType != DVBC) && (_gu32ScanFEType != DVBT2) &&
        (_gu32ScanFEType != ISDBT) && (_gu32ScanFEType != DVBS))
    {
        printf("%s: Fail!!! CH scan doesn't support current Broadcast Type ( = %"DTC_MS_U32_d")!!!\n", __FUNCTION__, _gu32ScanFEType);
        return FALSE;
    }

    if(_gu32ScanFEType == DVBS)
    {
        printf("===== Do blind scan ====\n");
        stScanParam.bScanMode = E_CHSCAN_MODE_TPSIDX_TABLE;
        stScanParam.eAutoTune = FE_TUNE_AUTO;
        stScanParam.eTvSysType = E_TV_DVBS;
        //stScanParam.pu8SatIDList= NULL;
        stScanParam.pstTPIdxTable = _gstAfe_param;
        _Demo_ChanScan_Start(&stScanParam);


        if(E_SCAN_TYPE_SCAN_EACH_TP_AND_ITS_PG == _eBlindScanType)
        {
          if(_gpstProgramList == NULL)
          {
            _gpstProgramList = (stFreqPG*)malloc(sizeof(stFreqPG)*100); // temp set 100
            if(_gpstProgramList == NULL)
            {
                free(_gpstProgramList);
                printf("Allocate program TABLE Fail !!!\n");
                return FALSE;
            }
            else
                memset(_gpstProgramList, 0x0, sizeof(stFreqPG)*100);
           }
        }

        return _Demo_DigiTuner_BlindScan();
    }

    printf("===== Do auto scan ====\n");

    switch(*puCountry)
    {
        case COUNTRY_TYPE_EUROPE:
            printf("Country: COUNTRY_TYPE_EUROPE\n");
            break;
        case COUNTRY_TYPE_GERMANY:
            printf("Country: COUNTRY_TYPE_GERMANY\n");
            break;
        case COUNTRY_TYPE_TAIWAN:
            printf("Country: COUNTRY_TYPE_TAIWAN\n");
            break;
        case COUNTRY_TYPE_ITALY:
            printf("Country: COUNTRY_TYPE_ITALY\n");
            break;
        case COUNTRY_TYPE_FRANCE:
            printf("Country: COUNTRY_TYPE_FRANCE\n");
            break;
        case COUNTRY_TYPE_CHINA:
            printf("Country: COUNTRY_TYPE_CHINA\n");
            break;
        case COUNTRY_TYPE_AUSTRALIA:
            printf("Country: COUNTRY_TYPE_AUSTRALIA\n");
            break;
        case COUNTRY_TYPE_BRAZIL:
            printf("Country: COUNTRY_TYPE_BRAZIL\n");
            break;
        case COUNTRY_TYPE_CHILE:
            printf("Country: COUNTRY_TYPE_CHILE\n");
            break;
        case COUNTRY_TYPE_HONGKONG:
            printf("Country: COUNTRY_TYPE_HONGKONG\n");
            break;
        case COUNTRY_TYPE_ARGENTINA:
            printf("Country: COUNTRY_TYPE_ARGENTINA\n");
            break;
        case COUNTRY_TYPE_PERU:
            printf("Country: COUNTRY_TYPE_PERU\n");
            break;
        case COUNTRY_TYPE_UK:
            printf("Country: COUNTRY_TYPE_UK\n");
            break;
        case COUNTRY_TYPE_RUSSIAN:
            printf("Country: COUNTRY_TYPE_RUSSIAN\n");
            break;
        case COUNTRY_TYPE_INDIA:
            printf("Country: COUNTRY_TYPE_INDIA\n");
            break;
        case COUNTRY_TYPE_CUBA:
            printf("Country: COUNTRY_TYPE_CUBA\n");
            break;
        case COUNTRY_TYPE_OTHER:
            printf("Country: COUNTRY_TYPE_OTHER\n");
            break;

        default:
            printf("%s: Country Error!!!\n", __FUNCTION__);
            return FALSE;
    }

    // Channel Scan Parameter
    stScanParam.bScanMode = E_CHSCAN_MODE_CHANNEL_NO;
    stScanParam.eAutoTune = FE_TUNE_AUTO;
    stScanParam.bIsNitSearch = FALSE;
    if(_gu32ScanFEType == DVBC)
        stScanParam.eTvSysType = E_TV_DVBC;
    else if((_gu32ScanFEType == DVBT2) || (_gu32ScanFEType == ISDBT))
        stScanParam.eTvSysType = E_TV_DVBT;

    stScanParam.eCountry = *puCountry;

    if(_gu32ScanFEType == ISDBT)
    {
#if (TEDDY_CH_SCAN_DONE == 1)
        MApp_MenuChannel_SetScanFrequency(180000);
#endif
        MsOS_DelayTask(1000);
    }

    if (stScanParam.eCountry  != COUNTRY_TYPE_OTHER)//gc.huang.nit add country limit
    {
        u8RFCh = _Demo_RfChannel_GetMaxRfChNum(stScanParam.eCountry);
        ch_scan_print("Max Channel = %d \n", u8RFCh);
        _Demo_RfChannel_GetTPSetting(u8RFCh, stScanParam.eCountry , &u16BandWidth, &stScanParam.u32LastFreq);
        u8RFCh = _Demo_RfChannel_GetMinRfChNum(stScanParam.eCountry);
        ch_scan_print("Min Channel = %d \n", u8RFCh);
        _Demo_RfChannel_GetTPSetting(u8RFCh, stScanParam.eCountry , &u16BandWidth, &stScanParam.u32StartFreq);
        ch_scan_print(" === [%s]==== Start FQ = %"DTC_MS_U32_d" , End FQ = %"DTC_MS_U32_d" \n",
            __FUNCTION__, stScanParam.u32StartFreq, stScanParam.u32LastFreq);
    }
    else
    {
        ch_scan_print("[%s]>>>>>>> Set Freq Start = %"DTC_MS_U32_d" , End = %"DTC_MS_U32_d" \n",
            __FUNCTION__, u32FullStartFrequency, u32FullEndFrequency);
        stScanParam.u32LastFreq = u32FullEndFrequency ;
        stScanParam.u32StartFreq = u32FullStartFrequency;
#if (TEDDY_CH_SCAN_DONE == 1)
        MApp_MenuChannel_SetScanFrequency(u32FullStartFrequency);
#endif
    }

    // Carrier Parameter
    if((_gu32ScanFEType == DVBC)
#if (CH_SCAN_SUPPORT_ATSC == 1)
        || (_gu32ScanFEType == J83B)
#endif
        )
    {
        stScanParam.stTPSInfo.CabParam.eConstellation = *pu32Arg1;
        stScanParam.stTPSInfo.CabParam.u16SymbolRate = *pu32Arg2;
        stScanParam.stTPSInfo.CabParam.eIQMode = CAB_IQ_NORMAL;
#if (CH_SCAN_SUPPORT_ATSC == 1)
        if(_gu32ScanFEType == J83B)
            stScanParam.stTPSInfo.CabParam.eBandWidth = DEMOD_CAB_BW_6M;
        else
#endif
            stScanParam.stTPSInfo.CabParam.eBandWidth = DEMOD_CAB_BW_8M;
    }
    else if(_gu32ScanFEType == DVBT2)
    {
        MApi_DigiTuner_InitParameter(_gu8TunerIndex);
        stScanParam.stTPSInfo.TerParam.u8PlpID = 0xff;
        stScanParam.stTPSInfo.TerParam.u8ScanType = E_DEMOD_TYPE_T;
        stScanParam.stTPSInfo.TerParam.eBandWidth = (EN_TER_BW_MODE)(BW_MODE_6MHZ + (u16BandWidth / 1000 - 6));
        stScanParam.stTPSInfo.TerParam.eLevelSel = TER_HP_SEL;
    }
    else
    {
        stScanParam.stTPSInfo.TerParam.u8PlpID = 0;
        stScanParam.stTPSInfo.TerParam.eBandWidth = (EN_TER_BW_MODE)(BW_MODE_6MHZ + (u16BandWidth / 1000 - 6));
        stScanParam.stTPSInfo.TerParam.eLevelSel = TER_HP_SEL;
    }

#if (TEDDY_CH_SCAN_DONE == 1)
    extern void MApi_SI_SetScanSectionTimeout(SI_Mon_TableType TableType, MS_U32 u32Timeout);
    MApi_SI_SetScanSectionTimeout(E_SI_MON_NIT, SCAN_NIT_TIMEOUT);
    MApi_SI_SetScanSectionTimeout(E_SI_MON_SDT, SCAN_SDT_TIMEOUT);
    MApi_SI_SetScanSectionTimeout(E_SI_MON_PAT, SCAN_PAT_TIMEOUT);
    MApi_SI_SetScanSectionTimeout(E_SI_MON_PMT, SCAN_PMT_TIMEOUT);
#endif
    // Start channel scan
    _Demo_ChanScan_Start(&stScanParam);

    while(_eChScanState != E_TASK_STATE_IDLE)
    {
        if(_gu32ScanFEType == ISDBT)
            MsOS_DelayTask(CH_SCAN_DELAY);

        _eChScanState = _Demo_ChScan_StateFunc(_eChScanState)();

        if(_eChScanState == E_TASK_STATE_WAITLOCK)
            MsOS_DelayTask(CH_SCAN_DELAY);
    }

    if(_gu32ScanFEType == DVBT2)
        //Recover DVB-T2 broadcast type
        MApi_Frontend_SetBroadcastType(_gu8TunerIndex, _gu32ScanFEType);

    printf("===== Auto scan complete ====\n");

    return TRUE;
}
#endif
