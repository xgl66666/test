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

#if (DEMO_TSENSOR_TEST == 1)
//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------
#include "MsCommon.h"
#include "drvSAR.h"
#include "string.h"
#include "MsIRQ.h"

//------------------------------------------------------------------------------
// Debug Macros
//------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
// Macros
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
// Global Variables
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
// Local Variables
//--------------------------------------------------------------------------------------------------
//static void _Demo_TSensor_ISR(MS_U32 u32Vector);

//--------------------------------------------------------------------------------------------------
// Local Defines
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
// Global Functions
//--------------------------------------------------------------------------------------------------
#if (DEMO_SAR_TSENSOR_SUPPORT == 1)
static void _TSensor_INT_CBFunc(void)
{
    MS_S16 s16CurUp,s16CurLow,s16CurTemperature_C;
    s16CurTemperature_C = MDrv_TSensor_GetTemperature();
    MDrv_TSensor_GetTemperatureRange(&s16CurUp,&s16CurLow);
    printf("[Warning] Current Temperature is out of range\n");
    printf("Get T-Sensor Temperature Range , Up Bound : %d ; Low Bound %d; Current Temperature : %d\n",s16CurUp,s16CurLow,s16CurTemperature_C);
}

//Print current chip's temperature and up-bound/low-bound stauts
MS_BOOL Demo_TSensor_GetTemp(void)
{
    MS_S16 s16CurTemperature_C = 0;
    MS_BOOL bRet = TRUE;

    s16CurTemperature_C = MDrv_TSensor_GetTemperature();
    printf("Current Temperature_C : %d\n",s16CurTemperature_C);

    return bRet;
}

MS_BOOL Demo_TSensor_SetTempMonitorRange(const MS_U32 *s16UpBound, const MS_U32 *s16LowBound)
{
    MS_S16 s16CurUp,s16CurLow;
    if(*s16UpBound< *s16LowBound)
    {
        printf("[ERROR] TSensor_SetTempRange [UpBound] [LowBound] , but [UpBound] is lesser than [LowBound]\n");
        return FALSE;
    }
    if(!MDrv_TSensor_SetTemperatureMointerRange((MS_S16) *s16UpBound,(MS_S16) *s16LowBound,_TSensor_INT_CBFunc))
    {
        printf("[ERROR] Set Temperature Up/Low Bound Error\n");
        return FALSE;
    }

    MDrv_TSensor_GetTemperatureRange(&s16CurUp,&s16CurLow);
    printf("Set T-Sensor Range , Up Bound : %d ; Low Bound %d\n",s16CurUp,s16CurLow);
    return TRUE;
}

MS_BOOL Demo_TSensor_GetTempMonitorRange(void)
{
    MS_S16 s16CurUp,s16CurLow,s16CurTemperature_C;
    s16CurTemperature_C = MDrv_TSensor_GetTemperature();
    MDrv_TSensor_GetTemperatureRange(&s16CurUp,&s16CurLow);
    printf("Get T-Sensor Temperature Range , Up Bound : %d ; Low Bound %d; Current Temperature : %d\n",s16CurUp,s16CurLow,s16CurTemperature_C);
    return TRUE;
}

MS_BOOL Demo_TSensor_ClearIRQ(void)
{
    if(MDrv_TSensorIRQClear())
    {
        return TRUE;
    }
    else
    {
        printf("[ERROR] Clear IRQ Fail\n");
        return FALSE;
    }
}
#else
//Set the temperature up-bound and low-bound. An ISR will be triggered 
//if (current temperature < *u8LOB ) || (current temperature > *u8UPB). => note: it is edge triggered 
//The ISR will call Demo_TSensor_PriTemp to print temperature status. 
//Note that the ISR will only be triggered once by the interrupt. 
//You can re-enter the Demo_TSensor_demo to re-enable the IRQ or re-setting the low-bound and high-bound.
MS_BOOL Demo_TSensor_demo (const MS_U8 *u8LOB,const MS_U8 *u8UPB)
{
    static MS_BOOL ISRIsAttched = FALSE;
    MS_BOOL bRet = TRUE;
    
    printf("[TSNSR.APP] Demo_TSensor_demo, set LOB:%d C, UPB:%d C\n",*u8LOB,*u8UPB);

    bRet &= MApi_TSensorEnable();

    //disable interrupt in SAR bank
    bRet &= MApi_TSensorIRQDisable();

    //set tempertaure low bound
    MApi_TSensorSetLowBound(*u8LOB);
    
    //set tempertaure up bound
    MApi_TSensorSetUpBound(*u8UPB);

    if ( !ISRIsAttched )
    {
        bRet &= MsOS_AttachInterrupt(E_INT_IRQ_KEYPAD, (InterruptCb)_Demo_TSensor_ISR);
    }
    else
    {
        //clear interrupt in SAR bank
        bRet &= MApi_TSensorIRQClear(); 
    }
    ISRIsAttched = TRUE;

    //enable interrupt in INTR bank
    bRet &= MsOS_EnableInterrupt(E_INT_IRQ_KEYPAD);

    //enable interrupt in SAR bank
    bRet &= MApi_TSensorIRQEnable();
    
    return bRet;
}

MS_BOOL Demo_TSensor_PriTemp(void)
{
    MS_S16 s16CurTemperature_C = 0;
    MS_BOOL bRet = TRUE;

    bRet &= MApi_TSensorEnable();
    s16CurTemperature_C = MApi_TSensorGetCurTemp_C();
    printf("[TSNSR.APP] Current chip's temperature = %d C\n",s16CurTemperature_C);
    printf("[TSNSR.APP] Current LOB = %d C, UPB = %d C\n",MApi_TSensorGetLowBound(),MApi_TSensorGetUpBound());
    return bRet;
}

//--------------------------------------------------------------------------------------------------
// Local Functions
//--------------------------------------------------------------------------------------------------
static void _Demo_TSensor_ISR(MS_U32 u32Vector)
{
    printf("[TSNSR.APP] In _Demo_TSensor_ISR\n");
    printf("[TSNSR.APP] u32Vector:0x%lx\n",u32Vector);
    
    if ( !MApi_TSensorINTCheck() )
    {
        printf("[TSNSR.APP] Not T-Sensor IRQ\n");
        return;
    }
    printf("[TSNSR.APP] It is T-Sensor IRQ\n");

    
    //Do ISR, you can write demo code here
    Demo_TSensor_PriTemp();

    //to re-enable the IRQ in this ISR you can call below codes
    //MApi_TSensorIRQClear(); 
    //MsOS_EnableInterrupt(E_INT_IRQ_KEYPAD);
}

#endif
#endif
