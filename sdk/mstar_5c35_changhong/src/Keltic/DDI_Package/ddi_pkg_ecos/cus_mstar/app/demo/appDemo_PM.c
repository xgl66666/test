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

//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------
#include "MsCommon.h"
#include "MsTypes2.h"
#include "MsMemory.h"
#include "drvIR.h"
#include <string.h>
#include "drvSERFLASH.h"


//System
#include "MsOS.h"
#include "appDemo_PM.h"

//Api
//Drv
#include "drvMMIO.h"
#include "drvMBX.h"
#include "drvPM.h"
//STR variable table
#include "strVariable.h"
#include "drvUART.h"
#include "drvBDMA.h"
#if (IR_TYPE_SEL == IR_TYPE_MSTAR_DTV)
#include "IR_MSTAR_DTV.h"
#else
#include "IR_MSTAR_DTV.h"
#endif

//------------------------------------------------------------------------------
// Debug Macros
//------------------------------------------------------------------------------
#if ENABLE_DEBUG
#define db_print(fmt, args...)  printf("[%s][%d]" fmt, __FUNCTION__, __LINE__, ## args)
#else
#define db_print(fmt, args...)  while(0);
#endif
#define PM_ALIGN_SHIFT 16
#define BLOCK_SIZE 0x10000
#define PM_SERFLASH_BLOCK_SHIFT 0x10000*3
//-------------------------------------------------------------------------------------------------
// Local Definition
//-------------------------------------------------------------------------------------------------
extern UART_Result MDrv_UART_SetRxMode
       (MS_BOOL bEnable, void *rx_buf, MS_U16 buf_len, ms_uart_rx_callback rx_cb);
extern MS_U8 UartBuf[64];
extern void UartCB(int c);


//For Drviver PM
#define _PA2VA(x) (MS_U32)MsOS_PA2KSEG1((x))
#define _VA2PA(x) (MS_U32)MsOS_VA2PA((x))

MS_U8 pmlitedat[]=
{
    #include "pmlite.dat"
};

MS_U8 pmlitedat_sram[]=
{
    #include "pmlite_sram.dat"
};

static MS_U8 u8PmWakeIRArray[]=
{   //IR wake-up key define
    IRKEY_POWER, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF
};
static PM_PowerDownCfg tPmPowerDownCfg;
static E_DDI_PM_MODE _PM_MODE = E_DDI_PM_MODE_SRAM;
//-------------------------------------------------------------------------------------------------
// Local Function
//-------------------------------------------------------------------------------------------------
MS_BOOL _Load51_IR_Initialize(void)
{
    static IR_RegCfg tIRRegCfg;
    static IR_TimeBnd tIRTimeBnd;

    tIRRegCfg.u8Ctrl0 = 0xBF;
    tIRRegCfg.u8Ctrl1 = 0x01;
    tIRRegCfg.u8Clk_mhz = (IR_CKDIV_NUM_BOOT) / (1000000UL);
    tIRRegCfg.u8HdrCode0 = IR_HEADER_CODE0;
    tIRRegCfg.u8HdrCode1 = IR_HEADER_CODE1;
    tIRRegCfg.u8CCodeBytes = 2;
    tIRRegCfg.u8CodeBits = 32;



    tIRTimeBnd.tHdr.s16Time = IR_HEADER_CODE_TIME;
    tIRTimeBnd.tHdr.s8UpBnd = 20;
    tIRTimeBnd.tHdr.s8LoBnd = -20;

    tIRTimeBnd.tOff.s16Time = IR_OFF_CODE_TIME;
    tIRTimeBnd.tOff.s8UpBnd = 20;
    tIRTimeBnd.tOff.s8LoBnd = -20;

    tIRTimeBnd.tOffRpt.s16Time = IR_OFF_CODE_RP_TIME;
    tIRTimeBnd.tOffRpt.s8UpBnd = 20;
    tIRTimeBnd.tOffRpt.s8LoBnd = -20;

    tIRTimeBnd.tLg01Hg.s16Time = IR_LOGI_01H_TIME;
    tIRTimeBnd.tLg01Hg.s8UpBnd = 35;
    tIRTimeBnd.tLg01Hg.s8LoBnd = -30;

    tIRTimeBnd.tLg0.s16Time = IR_LOGI_0_TIME;
    tIRTimeBnd.tLg0.s8UpBnd = 20;
    tIRTimeBnd.tLg0.s8LoBnd = -20;

    tIRTimeBnd.tLg1.s16Time = IR_LOGI_1_TIME;
    tIRTimeBnd.tLg1.s8UpBnd = 20;
    tIRTimeBnd.tLg1.s8LoBnd = -20;


    IR_RegCfg* pirRegCfg = (IR_RegCfg*)_PA2VA(PM51_MEM_ADR);
    IR_TimeBnd* pirTimeBnd = (IR_TimeBnd*)_PA2VA(PM51_MEM_ADR);

    //(0) For debugging
    //HB_printf("MailBox: MDrv_IR_SetDbgLevel\n");
    //MDrv_IR_SetDbgLevel(E_IR_DBGLVL_INFO);

    //(1) Initialization
    memcpy((void*)pirRegCfg, (void*)&tIRRegCfg, sizeof(IR_RegCfg));

    MsOS_Dcache_Flush(_PA2VA(PM51_MEM_ADR), sizeof(IR_RegCfg)) ;
    MsOS_FlushMemory();
    printf("MDrv_IR_Init... \n");
    if(MDrv_IR_Init((IR_RegCfg*)PM51_MEM_ADR) == E_IR_FAIL)
    {
        printf("MDrv_IR_Init fails...\n");
        return FALSE;
    }
    else
    {
        printf("MDrv_IR_Init success...\n");
    }
    //(2) Configure Time Bounds
    memcpy((void*)pirTimeBnd, (void*)&tIRTimeBnd, sizeof(IR_TimeBnd));

    MsOS_Dcache_Flush(_PA2VA(PM51_MEM_ADR), sizeof(IR_TimeBnd)) ;
    MsOS_FlushMemory();
    printf("MDrv_IR_Config... \n");
    if(MDrv_IR_Config((IR_TimeBnd*)PM51_MEM_ADR) == E_IR_FAIL)
    {
        printf("MDrv_IR_Config fails...\n");
        return FALSE;
    }
    else
    {
        printf("MDrv_IR_Config success...\n");
    }
    return TRUE;
}

MBX_Result _MBX_Init(void)
{
    MBX_CPU_ID eHKCPU;
    MS_U32 u32TimeoutMillSecs = 1500;

    MDrv_MBX_SetDbgLevel(MBX_DBG_LEVEL_ALL);

    eHKCPU = E_MBX_CPU_MIPS;

    return MDrv_MBX_Init(eHKCPU, E_MBX_ROLE_HK, u32TimeoutMillSecs);
}

extern MS_S32 gs32NonCachedPoolID;

static MS_BOOL _PM_Init(E_DDI_PM_MODE ePM_Mode)
{
    PM_Result pmResult = E_PM_FAIL;
    MBX_Result mbxResult = E_MBX_ERR_NOT_INITIALIZED;
    MS_U32 idx = 0,u32EraseSize = 0,u32StartAddr = 0,u32EndBlock = 0,u32StartBlock  = 0;
    const SERFLASH_Info *pm_serial_info = NULL;
    MS_U8 *pmlitedat_buffer = NULL;
    MS_U32 pmlitedat_buffer_pa = 0;
    //printf("[%s][%d] \n",__FUNCTION__,__LINE__);
    if (ePM_Mode == E_DDI_PM_MODE_SRAM)
    {
        if (TRUE != MDrv_BDMA_Init(0))
        {
            printf("[%s][%d] BDMA init fail\n", __FUNCTION__, __LINE__);
            return FALSE;
        }

        pmlitedat_buffer = MsOS_AllocateMemory(sizeof(pmlitedat_sram), gs32NonCachedPoolID);

        if (pmlitedat_buffer == NULL)
        {
            printf("[%s][%d] Memory Allocate fail\n", __FUNCTION__, __LINE__);
            return FALSE;
        }
        
        memcpy(pmlitedat_buffer, pmlitedat_sram, sizeof(pmlitedat_sram));

        pmlitedat_buffer_pa = MsOS_VA2PA((MS_U32)pmlitedat_buffer);

        if (MDrv_BDMA_CopyHnd((MS_PHYADDR)pmlitedat_buffer_pa,(MS_PHYADDR)0x0 ,sizeof(pmlitedat_sram),E_BDMA_SDRAM2SRAM1K_HK51,0x08)!= E_BDMA_OK)
        {
            printf("[%s][%d] BDMA init fail\n", __FUNCTION__, __LINE__);
          //  return FALSE;
        }

        MsOS_FreeMemory(pmlitedat_buffer,gs32NonCachedPoolID);
    }
    else if (ePM_Mode == E_DDI_PM_MODE_DRAM)
    {
        if (TRUE != MDrv_BDMA_Init(0))
        {
            printf("[%s][%d] BDMA init fail\n", __FUNCTION__, __LINE__);
            return FALSE;
        }

        pmlitedat_buffer = MsOS_AllocateMemory(sizeof(pmlitedat), gs32NonCachedPoolID);

        if (pmlitedat_buffer == NULL)
        {
            printf("[%s][%d] Memory Allocate fail\n", __FUNCTION__, __LINE__);
            return FALSE;
        }
        
        memcpy(pmlitedat_buffer, pmlitedat, sizeof(pmlitedat));

        pmlitedat_buffer_pa = MsOS_VA2PA((MS_U32)pmlitedat_buffer);
        printf("[%s][%d] PM51_LITE_ADDR %lx \n",__FUNCTION__,__LINE__,PM51_LITE_ADR);
        if (MDrv_BDMA_CopyHnd((MS_PHYADDR)pmlitedat_buffer_pa,(MS_PHYADDR)PM51_LITE_ADR ,sizeof(pmlitedat),E_BDMA_SDRAM2SDRAM,0x08)!= E_BDMA_OK)
        {
            printf("[%s][%d] BDMA init fail\n", __FUNCTION__, __LINE__);
          //  return FALSE;
        }

        MsOS_FreeMemory(pmlitedat_buffer,gs32NonCachedPoolID);
    }
    else if (ePM_Mode == E_DDI_PM_MODE_SERIALFLASH)
    {
        MDrv_SERFLASH_Init();
        pm_serial_info = MDrv_SERFLASH_GetInfo();
        MDrv_SERFLASH_WriteProtect(FALSE);
        printf("[%s][%d] u32TotalSize %ld \n", __FUNCTION__, __LINE__,pm_serial_info->u32TotalSize);
        printf("[%s][%d] u32SecSize %ld \n", __FUNCTION__, __LINE__,pm_serial_info->u32SecSize);
        printf("[%s][%d] u32SecNum %ld \n", __FUNCTION__, __LINE__,pm_serial_info->u32SecNum);
        u32EraseSize = sizeof(pmlitedat);
        u32StartAddr = pm_serial_info->u32TotalSize - PM_SERFLASH_BLOCK_SHIFT;
        printf("[%s][%d] u32SecNum %ld \n", __FUNCTION__, __LINE__,u32StartAddr);
        if(FALSE == MDrv_SERFLASH_AddressToBlock(u32StartAddr, &u32StartBlock))
        {
            printf("[%s][%d] u32StartAddr[0x%lx] is error \n", __FUNCTION__, __LINE__,u32StartAddr);
            return FALSE;
        }
        else
        {
            printf("[%s][%d] u32StartBlock[0x%lx] \n", __FUNCTION__, __LINE__,u32StartBlock);
        }

        if(FALSE == MDrv_SERFLASH_AddressToBlock((u32StartAddr) + u32EraseSize - 1, &u32EndBlock))
        {
            printf("[%s][%d] u32StartAddr[0x%lx] + u32EraseSize[0x%lx] = [0x%lx] is error \n", __FUNCTION__, __LINE__,u32StartAddr,u32EraseSize,(u32StartAddr) + u32EraseSize - 1);
            return FALSE;
        }
        else
        {
            printf("[%s][%d] u32EndBlock[0x%lx] \n", __FUNCTION__, __LINE__,u32EndBlock);
        }

        if(FALSE == MDrv_SERFLASH_BlockErase(u32StartBlock, u32EndBlock, TRUE))
        {
            printf("[%s][%d]  BlockErase u32StartBlock[0x%lx], u32EndBlock[0x%lx] is FAIL \n", __FUNCTION__, __LINE__,u32StartBlock,u32EndBlock);
            return FALSE;
        }
        else
        {
            printf("[%s][%d] BlockErase u32StartBlock[0x%lx], u32EndBlock[0x%lx] success!!\n", __FUNCTION__, __LINE__,u32StartBlock,u32EndBlock);
        }

        if (MDrv_SERFLASH_Write(u32StartAddr,u32EraseSize,pmlitedat) != TRUE)
        {
            printf("[%s][%d]\n", __FUNCTION__, __LINE__);
            return FALSE;
        }
        else
        {
            printf("[%s][%d] flashing ok \n", __FUNCTION__, __LINE__);
        }
        MDrv_SERFLASH_WriteProtect(TRUE);
    }
    else
    {
        printf("[%s][%d] \n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    printf("[%s][%d] \n",__FUNCTION__,__LINE__);
    mbxResult = _MBX_Init();

    if(E_MBX_SUCCESS != mbxResult)
    {
        printf("MBX init error\n");
    }

    MDrv_MBX_Enable(TRUE);

    printf("E_MBX_CLASS_PM_WAIT - MBX register msg\n");
    mbxResult = MDrv_MBX_RegisterMSG(E_MBX_CLASS_PM_WAIT, 6);
    if(E_MBX_SUCCESS != mbxResult)
    {
        printf("E_MBX_CLASS_PM_WAIT - MBX register msg error\n");
    }
    else
    {
        printf("E_MBX_CLASS_PM_WAIT - MBX register msg success\n");
    }
    if (ePM_Mode == E_DDI_PM_MODE_SRAM)
    {
        MDrv_PM_SetSRAMOffsetForMCU();
    }
    else if (ePM_Mode == E_DDI_PM_MODE_DRAM)
    {
        printf("[%s][%d] PM51_LITE_ADDR Offset %lx \n",__FUNCTION__,__LINE__,PM51_LITE_ADR>>PM_ALIGN_SHIFT);
        MDrv_PM_SetDRAMOffsetForMCU(PM51_LITE_ADR>>PM_ALIGN_SHIFT);
    }
    else if (ePM_Mode == E_DDI_PM_MODE_SERIALFLASH)
    {
        MDrv_PM_SetSPIOffsetForMCU(u32StartAddr >> PM_ALIGN_SHIFT);
    }

    if(MDrv_PM_Check_Version(TRUE))  // for Zenonia
    {
        _Load51_IR_Initialize();
    }

    //PM Config
    PM_WakeCfg *pPmWakeCfg = (PM_WakeCfg*)_PA2VA(PM51_MEM_ADR);
    pPmWakeCfg->bPmWakeEnableIR     = TRUE;
    pPmWakeCfg->bPmWakeEnableSAR    = FALSE;
    pPmWakeCfg->bPmWakeEnableGPIO0  = FALSE;
    pPmWakeCfg->bPmWakeEnableGPIO1  = FALSE;
    pPmWakeCfg->bPmWakeEnableUART1  = FALSE;
    pPmWakeCfg->bPmWakeEnableSYNC   = FALSE;
    pPmWakeCfg->bPmWakeEnableESYNC  = FALSE;
    pPmWakeCfg->bPmWakeEnableRTC0   = TRUE;
    pPmWakeCfg->bPmWakeEnableRTC1   = FALSE;
    pPmWakeCfg->bPmWakeEnableDVI0   = FALSE;
    pPmWakeCfg->bPmWakeEnableDVI2   = FALSE;
    pPmWakeCfg->bPmWakeEnableCEC    = FALSE;
    pPmWakeCfg->bPmWakeEnableAVLINK = FALSE;

    for(idx = 0; idx < PM_MAX_BUF_WAKE_IR; idx++)
    {
        pPmWakeCfg->u8PmWakeIR[idx] = u8PmWakeIRArray[idx];
    }

    MsOS_Dcache_Flush((MS_U32)pPmWakeCfg, sizeof(PM_WakeCfg));
    MsOS_FlushMemory();

    printf("PM51_MEM_ADR = %8x \n", PM51_MEM_ADR);
    printf("pPmWakeCfg addr  = %ld \n", (MS_U32)pPmWakeCfg);

    pmResult = MDrv_PM_Init((PM_WakeCfg*)PM51_MEM_ADR);
    if(pmResult)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }

    return TRUE;
}

static void _PM_PowerDown(void)
{


    tPmPowerDownCfg.u8PowerDownMode = E_PM_STANDBY;//E_PM_SLEEP;
    tPmPowerDownCfg.u8WakeAddress = E_PM_WAKE_LAST;

    PM_PowerDownCfg *pPmPowerDownCfg = (PM_PowerDownCfg*)_PA2VA(PM51_MEM_ADR);

    memcpy((void*)pPmPowerDownCfg, (void*)&tPmPowerDownCfg, sizeof(PM_PowerDownCfg));

    MsOS_Dcache_Flush(_PA2VA(PM51_MEM_ADR), sizeof(PM_PowerDownCfg));
    MsOS_FlushMemory();
    if(E_PM_OK == MDrv_PM_PowerDown((PM_PowerDownCfg*)PM51_MEM_ADR))

    {
        printf("MDrv_PM_PowerDown OK!! \n");
    }
    else
    {
        printf("MDrv_PM_PowerDown FAIL!! \n");
    }

}
//-------------------------------------------------------------------------------------------------
// Demo Functions
//-------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// @brief Create and start the app demo task of RTC and initialze all parameters
/// @param[in] None
/// @return TRUE - Process success
/// @return FALSE - Process fail
/// @sa
/// @note
/// Command: \b PM_RTC_Init \n
///
/// Sample Command: \b PM_RTC_Init \n
///
//------------------------------------------------------------------------------
MS_BOOL appDemo_PM_RTC_Init(void)
{
    PM_RtcParam* PmRtcParam=(PM_RtcParam*)_PA2VA(PM51_MEM_ADR);

    PmRtcParam->u8PmRtcIndex = 0;
    PmRtcParam->u32RtcCtrlWord = XTAL_CLOCK_FREQ;
    PmRtcParam->u32RtcSetMatchCounter = 0;
    PmRtcParam->u32RtcGetMatchCounter = 0;
    PmRtcParam->u32RtcSetCounter = 0;
    PmRtcParam->u32RtcGetCounter = 0;

    MDrv_PM_RtcInit((PM_RtcParam*)((MS_U32)PM51_MEM_ADR));

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Print system time by RTC
/// @param[in] None
/// @return TRUE - Process success
/// @return FALSE - Process fail
/// @sa
/// @note
/// Command: \b PM_RTC_GetTime \n
///
/// Sample Command: \b PM_RTC_GetTime \n
///
//------------------------------------------------------------------------------
MS_BOOL appDemo_PM_RTC_GetTime(void)
{
    MS_U32 u32SysTime = MDrv_PM_RTCGetSystemTime();
    db_print("Get system time = %lu \n", u32SysTime);

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Set system time by RTC
/// @param[in] pu32secs Time in second. (365*10)+2)*24*60*60 from 1970/1/1 to 1980/1/1
/// @return TRUE - Process success
/// @return FALSE - Process fail
/// @sa
/// @note
/// Command: \b PM_RTC_SetTime \n
///
/// Sample Command: \b PM_RTC_SetTime 10\n
///
//------------------------------------------------------------------------------
MS_BOOL appDemo_PM_RTC_SetTime(MS_U32 *pu32secs)
{
    MDrv_PM_RTCSetSystemTime(*pu32secs);
    db_print("Set system time = %lu \n", *pu32secs);
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Set match time by RTC
/// @param[in] pu32secs Time in second. (365*10)+2)*24*60*60 from 1970/1/1 to 1980/1/1
/// @return TRUE - Process success
/// @return FALSE - Process fail
/// @sa
/// @note
/// Command: \b PM_RTC_SetMatchTime \n
///
/// Sample Command: \b PM_RTC_SetMatchTime 10\n
///
//------------------------------------------------------------------------------
MS_BOOL appDemo_PM_RTC_SetMatchTime(MS_U32 *pu32secs)
{
    MDrv_PM_RTCSetMatchTime(*pu32secs);
    db_print("Set match time = %lu \n", *pu32secs);
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Initialize PM and set up wakeup configure data
/// @param[in] u32Mode
/// @return TRUE - Process success
/// @return FALSE - Process fail
/// @sa
/// @note
/// Command: \b PM_Init \n
///
/// Sample Command: \b PM_Init \n
///
//------------------------------------------------------------------------------
MS_BOOL appDemo_PM_Init(MS_U32 *u32Mode)
{
    if ((u32Mode == NULL) || (*u32Mode >= E_DDI_PM_MODE_MAX))
    {
        return FALSE;
    }

    switch (*u32Mode)
    {
        case E_DDI_PM_MODE_SRAM:
            _PM_MODE = E_DDI_PM_MODE_SRAM;
            break;
        case E_DDI_PM_MODE_SERIALFLASH:
            _PM_MODE = E_DDI_PM_MODE_SERIALFLASH;
            break;
        case E_DDI_PM_MODE_DRAM:
            _PM_MODE = E_DDI_PM_MODE_DRAM;
            break;
        default:
            _PM_MODE = E_DDI_PM_MODE_SRAM;
            break;
    }
    if (_PM_Init(_PM_MODE) == FALSE)
    {
        printf("[%s][%d] \n",__FUNCTION__,__LINE__);
        return FALSE;
    }
    return TRUE;
}
//------------------------------------------------------------------------------
/// @brief Power down to sleep and wait for wakeup source
/// @param[in] None
/// @return TRUE - Process success
/// @return FALSE - Process fail
/// @sa
/// @note
/// Command: \b PM_PoweDown \n
///
/// Sample Command: \b PM_PoweDown \n
///
//------------------------------------------------------------------------------
MS_BOOL appDemo_PM_PowerDown(void)
{
    _PM_PowerDown();
    return TRUE;
}
//------------------------------------------------------------------------------
/// @brief Fast standby and wait for wakeup source
/// @param[in] None
/// @return TRUE - Process success
/// @return FALSE - Process fail
/// @sa
/// @note
/// Command: \b PM_FastStandby \n
///
/// Sample Command: \b PM_FastStandby \n
///
//------------------------------------------------------------------------------
MS_BOOL appDemo_FastStandby(void)
{
    MS_U32 Enter_SR_Addr = (MS_U32)PM51_STR_ADR;
    MS_U32 Exit_SR_Addr = (MS_U32)(PM51_STR_ADR + 0x1000);
    MS_U32 HWIP_Close_Addr = (MS_U32)(PM51_STR_ADR + 0x2000);
    MS_U32 HWIP_Open_Addr = (MS_U32)(PM51_STR_ADR + 0x3000);

    MBX_Result eMbxResult;
    printf("Fast Standby!\n");
    _PM_MODE = E_DDI_PM_MODE_SRAM;

    if (_PM_Init(_PM_MODE) == FALSE)
    {
        printf("[%s][%d] \n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    printf("announce PM to power down\n");

    static union __attribute__ ((aligned (4096)))
    {
        struct
        {
            MS_U8 u8PowerDownMode;
            MS_U8 u8WakeAddress;
            MS_U8 padding[20];
        } __attribute__((packed)) A;
        struct
        {
            MS_U8 u8PowerDownMode;
            MS_U8 u8WakeAddress;
            MS_U32 magic;
            MS_U16* entSelf;
            MS_U16* extSelf;
            MS_U16* clsHWIP;
            MS_U16* opnHWIP;
        }__attribute__((packed)) B;
    } tPmPowerDownCfg;

    tPmPowerDownCfg.B.u8PowerDownMode = E_PM_STANDBY;
    tPmPowerDownCfg.B.u8WakeAddress = E_PM_WAKE_LAST;

    tPmPowerDownCfg.B.magic = 0xfee1dead;

    tPmPowerDownCfg.B.entSelf = (MS_U16*)Enter_SR_Addr;
    tPmPowerDownCfg.B.extSelf = (MS_U16*)Exit_SR_Addr;
    tPmPowerDownCfg.B.clsHWIP = (MS_U16*)HWIP_Close_Addr;
    tPmPowerDownCfg.B.opnHWIP = (MS_U16*)HWIP_Open_Addr;

    MS_U16 *pEntSelf = (MS_U16 *)_PA2VA(Enter_SR_Addr);
    MS_U16 *pExtSelf = (MS_U16 *)_PA2VA(Exit_SR_Addr);
    MS_U16 *pClsHWIP = (MS_U16 *)_PA2VA(HWIP_Close_Addr);
    MS_U16 *pOpnHWIP = (MS_U16 *)_PA2VA(HWIP_Open_Addr);

    //_MDrv_Power_FillRegisterTable();
    memcpy(pEntSelf, MIU_ENTER_SR, sizeof(MIU_ENTER_SR));
    memcpy(pExtSelf, MIU_EXIT_SR, sizeof(MIU_EXIT_SR));
    memcpy(pClsHWIP, HWIP_CLOSE, sizeof(HWIP_CLOSE));
    memcpy(pOpnHWIP, HWIP_OPEN, sizeof(HWIP_OPEN));

    MsOS_Dcache_Flush(_PA2VA(Enter_SR_Addr),sizeof(MIU_ENTER_SR));
    MsOS_Dcache_Flush(_PA2VA(Exit_SR_Addr),sizeof(MIU_EXIT_SR));
    MsOS_Dcache_Flush(_PA2VA(HWIP_Close_Addr),sizeof(HWIP_CLOSE));
    MsOS_Dcache_Flush(_PA2VA(HWIP_Open_Addr),sizeof(HWIP_OPEN));
    MsOS_FlushMemory();

    PM_PowerDownCfg *pPmPowerDownCfg =(PM_PowerDownCfg*)_PA2VA(PM51_MEM_ADR);

    memcpy((void*)pPmPowerDownCfg,(void*)&tPmPowerDownCfg,22);

    MsOS_Dcache_Flush(_PA2VA(PM51_MEM_ADR),22);
    MsOS_FlushMemory();
    if(E_PM_OK == MDrv_PM_PowerDown((PM_PowerDownCfg*)PM51_MEM_ADR))
    {
        printf("MDrv_PM_PowerDown OK!! \n");
    }
    else
    {
        printf("MDrv_PM_PowerDown FAIL!! \n");
    }

    printf("finish announce PM to power down\n");

    // disable PM51
    MDrv_PM_Disable51();

    // mbx
    eMbxResult = MDrv_MBX_UnRegisterMSG(E_MBX_CLASS_PM_WAIT,0);
    if (eMbxResult != E_MBX_SUCCESS)
    {
        printf("MDrv_MBX_UnRegisterMSG(E_MBX_CLASS_PM_NOWAIT,0) FAIL %x\n", eMbxResult);
    }

    eMbxResult = MDrv_MBX_DeInit(TRUE);
    if (eMbxResult != E_MBX_SUCCESS)
    {
        printf("MDrv_MBX_DeInit FAIL %x\n", eMbxResult);
    }

    return TRUE;
}

