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
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/fs.h>
#include <linux/vmalloc.h>
#include <linux/err.h>
#include <asm/uaccess.h>
#else
#include <stdio.h>
#endif
#include "error.h"
#include "MsCommon.h"
#include "HbCommon.h"
#include "drvIIC.h"
#include "apiDigiTuner.h"
#include "Board.h"
#include "drvDemod.h"

#if IF_THIS_TUNER_INUSE(TUNER_NXP_TD1611ALF)

#define TUNER_DEV_ADR           0xC0
#define TUNER_IF_FREQ           36160
#define NXP_TD1611ALF_INT_AGC   1
#define DBG_MSB(x)              x

MS_BOOL Tuner_IIC_Write(HWI2C_PORT ePort, MS_U8 u8SlaveID, MS_U8 u8AddrNum, MS_U8* paddr, MS_U16 u16size, MS_U8* pu8data)
{
    MDrv_Demod_I2C_ByPass(1);
    if ( FALSE == MDrv_IIC_WriteBytes(ePort, u8SlaveID, u8AddrNum, paddr, u16size, pu8data) )
    {
        HB_printf("PassThroughI2C_WriteBytes Error \n");
        MDrv_Demod_I2C_ByPass(0);
        return TRUE;
    }
    MDrv_Demod_I2C_ByPass(0);
    MsOS_DelayTask(1); // delay 1ms
    return ERR_NO_ERROR;
}
MS_BOOL Tuner_IIC_Read(HWI2C_PORT ePort, MS_U8 u8SlaveID, MS_U8 u8AddrNum, MS_U8* paddr, MS_U16 u16size, MS_U8* pu8data)
{
    MDrv_Demod_I2C_ByPass(1);
    //MDrv_IIC_SelectCLK(ePort, E_HWI2C_HIGH);
    MDrv_IIC_ReadBytes(ePort, u8SlaveID, u8AddrNum, paddr, u16size, pu8data);
    //MDrv_IIC_SelectCLK(ePort, E_HWI2C_SLOW);
    MDrv_Demod_I2C_ByPass(0);
    return ERR_NO_ERROR;
}
static MS_U32 NXP_TD1611ALF_SetTuner(MS_U8 ucTuner2WireAddr, MS_U32 dwFreq /* KHz*/, MS_U32 ucBw /*MHz*/)
{
    // TD1611
    // IF Frequency: 36.16Mhz
    //
    // 60uS delay
    //ADDR , FREQ_HIGH, FREQ_LOW,CB1,CB2.
    // 60uS delay
    MS_U8    CB0,CB1;
    MS_U32   dwError = ERR_NO_ERROR;
    MS_U8    Buf[5];
    MS_U8    ucfreq[2];
    MS_U16   wFrequency;
    MS_U8    ucDividerRatio = 6;

    //According to TD1600ALF-4, the tuner need 60us to process frequency-setting I2C command
    //During the period, we should not send other I2C command to the tuner
    //We let I2C driver delay 100us after send each command here
    MDrv_HWI2C_ConfigDelay(20, 100);

    // Use critical section to protect Tuner frequency setting
    //SysEnterCriticalSection();

    CB0 = 0xC8;
    CB1 = 0;
    //P5 P4
    if(ucBw == BW_MODE_8MHZ)
    {
        CB0 |= 0x01;
        CB1 |= 0x08;//according to -4 spec only.
        ucDividerRatio = 7;
    }
    else
    {
        CB0 |= 0x02; //R2R1R0
        ucDividerRatio = 6;
    }
    dwFreq*=1000;
    wFrequency = (MS_U16)(((dwFreq + TUNER_IF_FREQ) * ucDividerRatio + 500) / 1000);
    ucfreq[1] = (MS_U8)((wFrequency >> 8) & 0x00FF);
    ucfreq[0] = (MS_U8)(wFrequency & 0x00FF);

    //P3 P2 P1
    if ( dwFreq <= 174000 ) // VHF low
    {
        CB1 |= 0x00;
    }
    else if ( dwFreq  <= 470000 ) // VHF high
    {
        CB1 |= 0x02;
    }
    else  // UHF
    {
        CB1 |= 0x04;
    }
    //Charge Pump  CP2 CP1 CP0
    dwFreq += TUNER_IF_FREQ;
    if(dwFreq<=200000)
        CB1|= 0x60;// not used
    else if(dwFreq<=290000)
        CB1|= 0x60;
    else if(dwFreq<=420000)
        CB1|= 0xA0;
    else if(dwFreq<=480000)
        CB1|= 0xC0;
    else if(dwFreq<=620000)
        CB1|= 0x60;
    else if(dwFreq<=830000)
        CB1|= 0xA0;
    else
        CB1|= 0xE0;

    Buf[0]  = ucfreq[1];
    Buf[1]  = ucfreq[0];
    Buf[2]  = CB0;
    Buf[3]  = CB1;

    dwError = Tuner_IIC_Write(FRONTEND_TUNER_PORT, TUNER_DEV_ADR, 0, NULL, 4,Buf);
    //dwError = Tuner_I2C_Write(ucTuner2WireAddr, &Buf[0],4);
    if (!dwError)
    {
        MsOS_DelayTask(2);
        #if (NXP_TD1611ALF_INT_AGC)
        Buf[2]  = 0x8C;
        #else
        Buf[2]  = 0x8E;
        #endif
        dwError = Tuner_IIC_Write(FRONTEND_TUNER_PORT, TUNER_DEV_ADR, 0, NULL, 4,Buf);
    }

    DBG_MSB( HB_printf("tuner status:%x\n",Buf[0]));
    #if 1
        if (ERR_NO_ERROR == dwError)
        {
            MS_U8 n=0;
            while(n<20)
            {
                MsOS_DelayTask(1);
                //dwError=iptr->ReadBytes(0,NULL,1, CONFIG);
                Buf[0] = 0x0;
                dwError = Tuner_IIC_Read(FRONTEND_TUNER_PORT, TUNER_DEV_ADR, 0, NULL, 1, Buf);
                n++;
                /*if((CONFIG[0]&0x08)==0x08)
                         HB_printf("Internal RF AGC!\n");
                         else
                         HB_printf("external RF AGC\n");*/
                if(dwError != ERR_NO_ERROR)
                {
                    break;
                }
                else if(Buf[0]==0xFF) // not by pass
                {
                    dwError = 1; // FIXME: Should assign a correct error id here
                    break;
                }
                else if((Buf[0]&0x40)==0x40)
                {
                    HB_printf("Tuner locked %d!\n",n);
                    break;
                }
                else
                {
                    HB_printf("tuner lock failed\n");
                }

            }

            if((n>=20)||(dwError != ERR_NO_ERROR))
            {
                HB_printf("tuner config fail; status:%x\n",Buf[0]);
                dwError=FALSE;
            }
        }
    #else
        MsOS_DelayTask(1);
    #endif

    //Let I2C driver reverse to default start and stop delays
    MDrv_HWI2C_ConfigDelay(20, 60);

    // Release ownership of the critical section
    //SysLeaveCriticalSection();
    return (dwError);
}

//**********************************************************************
// Set Tuner to desired frequency
//
//**********************************************************************
MS_U32 MDrv_Tuner_SetTuner(MS_U32 dwFreq /*Khz*/, MS_U8 ucBw /*MHz*/)
{
    MS_U32   dwError = ERR_NO_ERROR;

    DBG_MSB(HB_printf("\n NXP TD1611ALF SetTuner \n"));
    if ((dwError = NXP_TD1611ALF_SetTuner(
                                        TUNER_DEV_ADR,
                                        dwFreq,
                                        ucBw)))
    {
        DBG_MSB(HB_printf("\n\n#### %s Fail\n\n", __FUNCTION__));
    }

    // Release ownership of the critical section
    //SysLeaveCriticalSection();
    return (dwError);
}

//**********************************************************************
// This function is not needed for LG_TDTMG551D.
//
// Please just keep it empty.
//
//**********************************************************************
MS_U32 InitTuner(void * pData)
{
    return (ERR_NO_ERROR);
}

//**********************************************************************
// This function is not needed for LG_TDTMG551D.
//
// Please just keep it empty.
//
//**********************************************************************
MS_U32 AdjustTuner(void)
{
    return (ERR_NO_ERROR);
}

//**********************************************************************
// This function is not needed for LG_TDTMG551D.
//
// Please just keep it empty.
//
//**********************************************************************
MS_U32 CalibrateTuner(void)
{
    return (ERR_NO_ERROR);
}


MS_U32 MDrv_Tuner_Loop_Through_On(MS_BOOL bOn)
{

    return (ERR_NO_ERROR);
}


MS_U32 MDrv_Tuner_Stand_By(void)
{

    return (ERR_NO_ERROR);
}


MS_U32 MDrv_Tuner_Wake_Up(void)
{

    return (ERR_NO_ERROR);
}

#endif

