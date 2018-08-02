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
#if (DEMO_PM_TEST == 1)
//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------
#include "MsCommon.h"
#include "MsMemory.h"
#include "drvIR.h"
#include <string.h>
#include "SysInit.h"
#if (DEMO_PM_STR_LINUX_TEST == 1)
#include "MsTypes2.h"
#include <fcntl.h>
#include <unistd.h>
#endif

#if (DEMO_PM_HK_LINUX_TEST == 1)
#include <sys/ioctl.h>
#include <sys/stat.h>
#include "MsFS.h"
#endif

#if (DEMO_PM_HK_eCos_TEST == 1)
#include "drvPWSV.h"
#endif

#ifdef MSOS_TYPE_ECOS
//#include <cyg/fileio/fileio.h>
//#include <cyg/cpuload/cpuload.h>
#include <cyg/kernel/kapi.h>
#endif
#include "drvSERFLASH.h"

#include "MsOS.h"
#include "demo_pm.h"
#include "demo_utility.h"

#include "drvMMIO.h"
#include "drvMBX.h"
#include "drvPM.h"
#include "drvUART.h"
#include "drvBDMA.h"
#include "drvGPIO.h"
#include "IR_MSTAR_DTV.h"
#include "drvDTC.h"
#include "apiAUDIO.h"
#if (DEMO_PM_STR_eCos_TEST == 1)
#include "drvUSB_eCos.h"
#endif

#if (DEMO_PM_STR_LINUX_TEST == 1 || DEMO_PM_STR_eCos_TEST == 1)
#include "drvHWI2C.h"
#include "apiDigiTuner.h"
#include "drvMVOP.h"
#include "drvMSPI.h"
#include "drvTVEncoder.h"
#include "drvGPIO.h"
#include "drvMIU.h"
#include "drvVBI.h"
#include "drvXC_IOPort.h"
#include "apiXC.h"
#include "apiDMX.h"
#include "drvXC_HDMI_if.h"
#include "apiHDMITx.h"
#if (DEMO_DIP_TEST == 1)
#include "apiXC_DWIN.h"
#endif
#include "apiXC_Ace.h"
#include "drvSYS.h"
#include "apiDAC.h"
#include "msAPI_VE.h"
#include "drvPQ.h"
#include "apiAUDIO.h"
#include "apiVDEC_EX.h"
#include "drvDSCMB.h" 
#endif

#if (DEMO_PM_HK_LINUX_TEST == 1) || (DEMO_PM_HK_eCos_TEST == 1)
#include "apiDigiTuner.h"
#include "drvTVEncoder.h"
#include "apiDMX.h"
#include "apiDAC.h"
#include "msAPI_VE.h"
#include "demo_xc.h"
#else
#include "strVariable.h"
#endif

//------------------------------------------------------------------------------
// Debug Macros
//------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
// Local Definition
//-------------------------------------------------------------------------------------------------
#if (DEMO_PM_HK_LINUX_TEST == 1 || DEMO_PM_HK_eCos_TEST == 1)
#if (DEMO_XC_TEST)
static MSAPI_XC_DEVICE_ID _stXC_DeviceId[2] = {{0, E_MSAPI_XC_DEVICE0}, {0, E_MSAPI_XC_DEVICE1}};
static MS_BOOL _bXC_init_0 = FALSE;
#if (DEMO_XC_DUALXC_TEST == 1)
static MS_BOOL _bXC_init_1 = FALSE;
#endif
#endif //DEMO_XC_TEST
static MS_BOOL _bVE_init = FALSE;
#endif

#define PM_SERFLASH_BLOCK_SHIFT 0x10000*3
#define PM_ALIGN_SHIFT 16

#if (DEMO_PM_HK_LINUX_TEST == 1)
#define IR_1ST_DELAY_TIME_MS    800
#define IR_2ST_DELAY_TIME_MS    100
#endif

#define _PA2VA(x) (MS_U32)MsOS_PA2KSEG1((x))
#define _VA2PA(x) (MS_U32)MsOS_VA2PA((x))

#if (DEMO_PM_USE_STR_FLAG_CHECK_TEST == 1)
#define HK_STR_IDLE                  0xFFFF
#define HK_STR_READY                 0xFF01
#define HK_STR_WAKEUP                0xFF19
static volatile MS_U32 STR_Wakeup_Flag = HK_STR_IDLE;
#endif

#ifdef PM51_BUF_ADR    // for union PM buffer
#ifndef PM51_STR_ADR
#define PM51_STR_ADR (PM51_BUF_ADR)
#endif
#ifndef PM51_LITE_ADR
#define PM51_LITE_ADR (PM51_BUF_ADR + 0x4000)
#endif
#ifndef PM51_MEM_ADR
#define PM51_MEM_ADR (PM51_BUF_ADR + 0x15000)
#endif
#endif

#if (DEMO_PM_STR_LINUX_TEST == 1 || DEMO_PM_STR_eCos_TEST == 1)
///Define PM STR Mode enum
typedef enum
{
    E_STR_NONE = 0,      /// non-str dc on
    E_STR_FAKE_AC,       /// non-str ac on
    E_STR_NORMAL,        /// str
    E_STR_CRC,           /// str and crc
    E_STR_SELFTEST,
    E_STR_NUM,
} EN_STR_MODE;
#endif

#if (DEMO_PM_HK_LINUX_TEST == 1)
typedef struct
{
    MS_U32 u32_1stDelayTimeMs;
    MS_U32 u32_2ndDelayTimeMs;
    MS_U8 data3;
} MS_IR_DelayTime, *PMS_IR_DelayTime;
#endif

//-------------------------------------------------------------------------------------------------
// Global Variable
//-------------------------------------------------------------------------------------------------
#if (DEMO_PM_HK_LINUX_TEST == 0) && (DEMO_PM_HK_eCos_TEST == 0)
static PM_PowerDownCfg tPmPowerDownCfg;
static EN_DDI_PM_MODE _PM_MODE = E_DDI_PM_MODE_SRAM;

#if (DEMO_PM_WOL_TEST == 1)
static MS_BOOL _EnableWOL = FALSE;
static MS_U8 _u8PmWakeMACAddress[PM_MAX_BUF_WAKE_MAC_ADDRESS];
#endif

MS_U8 pmlitedat_flash[]=
{
    #include "pmlite_flash.dat"
};

#if (DEMO_PM_USE_PMLOADER_TEST == 1)
MS_U8 pmloaderdat[]=
{
    #include "pmloader.dat"
};
#else
MS_U8 pmlitedat_sram[]=
{
    #include "pmlite_sram.dat"
};
#endif

#if (DEMO_PM_STR_LINUX_TEST == 1 || DEMO_PM_STR_eCos_TEST == 1)
MS_U8 pmlitedat_str[]=
{
    #include "pmlite_str.dat"
};
#endif

#endif

#if (DEMO_PM_HK_LINUX_TEST == 1)
static int gIR_fd = -1;
static MS_S32 stRCUSem = -1;
MS_U8 pm_bin[]=
{
    #include "pm.dat"
};
#endif

#if (DEMO_PM_STR_LINUX_TEST == 1 || DEMO_PM_STR_eCos_TEST == 1 || DEMO_PM_HK_LINUX_TEST == 1 || DEMO_PM_HK_eCos_TEST == 1)
//extern MS_U8 u8TSPFirmware[];
static MS_U8 u8TSPFirmware[] = {
    #include "fwTSP.dat"
};
#endif
//-------------------------------------------------------------------------------------------------
// Local Function
//-------------------------------------------------------------------------------------------------

#if (DEMO_PM_HK_LINUX_TEST == 1)
static void _PM_WriteReg(MS_U32 reg, MS_U16 value)
{
    *(volatile unsigned short *)(reg) = value;
}
#endif

#if(DEMO_PM_STR_eCos_TEST == 1)
extern UART_Result MDrv_UART_SetRxMode
       (MS_BOOL bEnable, void *rx_buf, MS_U16 buf_len, ms_uart_rx_callback rx_cb);
extern MS_U8 UartBuf[64];
extern void UartCB(int c);
#endif

#if (DEMO_PM_HK_LINUX_TEST == 1) || (DEMO_PM_HK_eCos_TEST == 1)
extern UART_Result MDrv_UART_SetRxMode
       (MS_BOOL bEnable, void *rx_buf, MS_U16 buf_len, ms_uart_rx_callback rx_cb);
extern MS_U8 UartBuf[64];
extern void UartCB(int c);
extern void MDrv_IR_HK_Init(void);
#else
#if (DEMO_PM_USE_PMLOADER_TEST == 1)
static MS_BOOL _PM_CopyPMCodeToSRAM(EN_DDI_PM_PWR_MODE ePWR_Mode)
{
    PM_Result pmResult;

    memcpy((void*)_PA2VA((MS_U32)PM51_LOADER_ADR) , pmloaderdat, sizeof(pmloaderdat));
    memcpy((void*)_PA2VA((MS_U32)PM51_LITE_ADR) , pmlitedat_flash, sizeof(pmlitedat_flash));

    //Init 51 to run PM Loader
    MDrv_PM_SetDRAMOffsetForMCU(((MS_U32)PM51_LOADER_ADR) >> 16);

    //Transfer address of PM-Lite on Dram to PM loader.
    //This will not really configure wakeup sources.
    pmResult= MDrv_PM_Init((PM_WakeCfg*)PM51_LITE_ADR);

    if(!pmResult)
    {
        printf("pm loader fail\n");
        return FALSE;
    }

    return TRUE;

}

#else
static MS_BOOL _PM_CopyPMCodeToSRAM(EN_DDI_PM_PWR_MODE ePWR_Mode)
{
    MS_U8 *pmlitedat_buffer = NULL;
    MS_U32 pmlitedat_buffer_pa = 0;
    MS_S32 gs32NonCachedPoolID = INVALID_POOL_ID;

    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE,&gs32NonCachedPoolID);

    if (TRUE != MDrv_BDMA_Init(0))
    {
        printf("[%s][%d] BDMA init fail\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

#if ((DEMO_PM_STR_LINUX_TEST == 1) || (DEMO_PM_STR_eCos_TEST == 1))
    if (E_DDI_PM_PWR_MODE_FASTSTANDBY != ePWR_Mode) // STR or Normal standby
    {
        pmlitedat_buffer = MsOS_AllocateMemory(sizeof(pmlitedat_str), gs32NonCachedPoolID);

        if (pmlitedat_buffer == NULL)
        {
            printf("[%s][%d] Memory Allocate fail\n", __FUNCTION__, __LINE__);
            return FALSE;
        }

        memcpy(pmlitedat_buffer, pmlitedat_str, sizeof(pmlitedat_str));

        pmlitedat_buffer_pa = MsOS_VA2PA((MS_U32)pmlitedat_buffer);

        if (MDrv_BDMA_CopyHnd((MS_PHYADDR)pmlitedat_buffer_pa,(MS_PHYADDR)0x0 ,sizeof(pmlitedat_str),E_BDMA_SDRAM2SRAM1K_HK51,0x08)!= E_BDMA_OK)
        {
            printf("[%s][%d] BDMA init fail\n", __FUNCTION__, __LINE__);
            return FALSE;
        }

    }
    else
#endif
    {
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
            return FALSE;
        }
    }
    MsOS_FreeMemory(pmlitedat_buffer,gs32NonCachedPoolID);

    return TRUE;

}

#endif

static MS_BOOL _Load51_IR_Initialize(void)
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

static MBX_Result _MBX_Init(void)
{
    MBX_CPU_ID eHKCPU;
    MS_U32 u32TimeoutMillSecs = 1500;

    MDrv_MBX_SetDbgLevel(MBX_DBG_LEVEL_ALL);

    eHKCPU = E_MBX_CPU_MIPS;

    return MDrv_MBX_Init(eHKCPU, E_MBX_ROLE_HK, u32TimeoutMillSecs);
}

static MS_BOOL _PM_Init(EN_DDI_PM_MODE ePM_Mode, EN_DDI_PM_PWR_MODE ePWR_Mode)
{
    PM_Result pmResult = E_PM_FAIL;
    MBX_Result mbxResult = E_MBX_ERR_NOT_INITIALIZED;
    MS_U32 idx = 0,u32EraseSize = 0,u32StartAddr = 0,u32EndBlock = 0,u32StartBlock  = 0;
    const SERFLASH_Info *pm_serial_info = NULL;
    MS_S32 gs32NonCachedPoolID = INVALID_POOL_ID;
    MS_U8 *pmlitedat_buffer = NULL;
    MS_U32 pmlitedat_buffer_pa = 0;

    MS_U8 u8PmWakeIRArray[]=
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

    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE,&gs32NonCachedPoolID);

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
        _PM_CopyPMCodeToSRAM(ePWR_Mode);
    }
    else if (ePM_Mode == E_DDI_PM_MODE_DRAM)
    {
        if (TRUE != MDrv_BDMA_Init(0))
        {
            printf("[%s][%d] BDMA init fail\n", __FUNCTION__, __LINE__);
            return FALSE;
        }

        pmlitedat_buffer = MsOS_AllocateMemory(sizeof(pmlitedat_flash), gs32NonCachedPoolID);

        if (pmlitedat_buffer == NULL)
        {
            printf("[%s][%d] Memory Allocate fail\n", __FUNCTION__, __LINE__);
            return FALSE;
        }

        memcpy(pmlitedat_buffer, pmlitedat_flash, sizeof(pmlitedat_flash));

        pmlitedat_buffer_pa = MsOS_VA2PA((MS_U32)pmlitedat_buffer);

        if (MDrv_BDMA_CopyHnd((MS_PHYADDR)pmlitedat_buffer_pa,(MS_PHYADDR)PM51_LITE_ADR ,sizeof(pmlitedat_flash),E_BDMA_SDRAM2SDRAM,0x08)!= E_BDMA_OK)
        {
            printf("[%s][%d] BDMA init fail\n", __FUNCTION__, __LINE__);
            return FALSE;
        }

        MsOS_FreeMemory(pmlitedat_buffer,gs32NonCachedPoolID);
    }
    else if (ePM_Mode == E_DDI_PM_MODE_SERIALFLASH)
    {
        MDrv_SERFLASH_Init();
        pm_serial_info = MDrv_SERFLASH_GetInfo();
        MDrv_SERFLASH_WriteProtect(FALSE);
        printf("[%s][%d] u32TotalSize %"DTC_MS_U32_d" \n", __FUNCTION__, __LINE__,pm_serial_info->u32TotalSize);
        printf("[%s][%d] u32SecSize %"DTC_MS_U32_d" \n", __FUNCTION__, __LINE__,pm_serial_info->u32SecSize);
        printf("[%s][%d] u32SecNum %"DTC_MS_U32_d" \n", __FUNCTION__, __LINE__,pm_serial_info->u32SecNum);
        u32EraseSize = sizeof(pmlitedat_flash);
        u32StartAddr = pm_serial_info->u32TotalSize - PM_SERFLASH_BLOCK_SHIFT;
        printf("[%s][%d] u32SecNum %"DTC_MS_U32_d" \n", __FUNCTION__, __LINE__,u32StartAddr);
        if(FALSE == MDrv_SERFLASH_AddressToBlock(u32StartAddr, &u32StartBlock))
        {
            printf("[%s][%d] u32StartAddr[0x%"DTC_MS_U32_x"] is error \n", __FUNCTION__, __LINE__,u32StartAddr);
            return FALSE;
        }
        else
        {
            printf("[%s][%d] u32StartBlock[0x%"DTC_MS_U32_x"] \n", __FUNCTION__, __LINE__,u32StartBlock);
        }

        if(FALSE == MDrv_SERFLASH_AddressToBlock((u32StartAddr) + u32EraseSize - 1, &u32EndBlock))
        {
            printf("[%s][%d] u32StartAddr[0x%"DTC_MS_U32_x"] + u32EraseSize[0x%"DTC_MS_U32_x"] = [0x%"DTC_MS_U32_x"] is error \n",
                __FUNCTION__, __LINE__,u32StartAddr,u32EraseSize,(u32StartAddr) + u32EraseSize - 1);
            return FALSE;
        }
        else
        {
            printf("[%s][%d] u32EndBlock[0x%"DTC_MS_U32_x"] \n", __FUNCTION__, __LINE__,u32EndBlock);
        }

        if(FALSE == MDrv_SERFLASH_BlockErase(u32StartBlock, u32EndBlock, TRUE))
        {
            printf("[%s][%d]  BlockErase u32StartBlock[0x%"DTC_MS_U32_x"], u32EndBlock[0x%"DTC_MS_U32_x"] is FAIL \n",
                __FUNCTION__, __LINE__,u32StartBlock,u32EndBlock);
            return FALSE;
        }
        else
        {
            printf("[%s][%d] BlockErase u32StartBlock[0x%"DTC_MS_U32_x"], u32EndBlock[0x%"DTC_MS_U32_x"] success!!\n",
                __FUNCTION__, __LINE__,u32StartBlock,u32EndBlock);
        }

        if (MDrv_SERFLASH_Write(u32StartAddr,u32EraseSize,pmlitedat_flash) != TRUE)
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

    if (ePM_Mode == E_DDI_PM_MODE_SRAM)
    {
        MDrv_PM_SetSRAMOffsetForMCU();
    }
    else if (ePM_Mode == E_DDI_PM_MODE_DRAM)
    {
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
#if (DEMO_PM_WOL_TEST == 1)
    pPmWakeCfg->bPmWakeEnableWOL    = _EnableWOL;
    memcpy(pPmWakeCfg->u8PmWakeMACAddress, _u8PmWakeMACAddress,PM_MAX_BUF_WAKE_MAC_ADDRESS);
#endif

#if ((DEMO_PM_STR_LINUX_TEST == 1) || (DEMO_PM_STR_eCos_TEST == 1))
    if (E_DDI_PM_PWR_MODE_STR == ePWR_Mode)
    {
        pPmWakeCfg->u8PmStrMode = E_STR_NORMAL;
    }
    else
    {
        pPmWakeCfg->u8PmStrMode = E_STR_NONE;
    }
#endif
    for(idx = 0; idx < PM_MAX_BUF_WAKE_IR; idx++)
    {
        pPmWakeCfg->u8PmWakeIR[idx] = u8PmWakeIRArray[idx];
    }

    MsOS_Dcache_Flush((MS_U32)pPmWakeCfg, sizeof(PM_WakeCfg));
    MsOS_FlushMemory();

    printf("PM51_MEM_ADR = %8x \n", PM51_MEM_ADR);
    printf("pPmWakeCfg addr  = %"DTC_MS_U32_d" \n", (MS_U32)pPmWakeCfg);

    pmResult = MDrv_PM_Init((PM_WakeCfg*)PM51_MEM_ADR);
    if(pmResult)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

static MS_BOOL _PM_PowerDown(MS_BOOL bSTRMode)
{
    tPmPowerDownCfg.u8PowerDownMode = E_PM_STANDBY;//E_PM_SLEEP;
#if ((DEMO_PM_STR_LINUX_TEST == 1) || (DEMO_PM_STR_eCos_TEST == 1))
    if (bSTRMode)
    {
        tPmPowerDownCfg.u8WakeAddress = E_PM_LAST_TWOSTAGE_POWERDOWN;
    }
    else
#endif
    {
    #if (DEMO_PM_WOL_TEST == 1)
        if(_EnableWOL == FALSE)
            MApi_System_TurnOff_LAN();
    #else
        MApi_System_TurnOff_LAN();
    #endif
        tPmPowerDownCfg.u8WakeAddress = E_PM_WAKE_LAST;
    }
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
        return FALSE;
    }

	return TRUE;

}

#endif

#if (DEMO_PM_HK_LINUX_TEST == 1)
static MS_BOOL IrInput_init(void)
{
    MS_VIRT virtBaseAddr = 0;
    MS_PHY phyBaseSize = 0;
    if(stRCUSem < 0)
    {
        stRCUSem = MsOS_CreateMutex (E_MSOS_FIFO, "MUTEX_IrInput", MSOS_PROCESS_PRIVATE);
    }

    if(FALSE == MsOS_ObtainMutex(stRCUSem, MSOS_WAIT_FOREVER))
    {
        return FALSE;
    }

    if(gIR_fd >= 0)
    {
        if(FALSE == MsOS_ReleaseMutex(stRCUSem))
        {
            return FALSE;
        }
        return TRUE;
    }

    if(!MDrv_MMIO_GetBASE(&virtBaseAddr, &phyBaseSize, MS_MODULE_PM))
    {
        if(FALSE == MsOS_ReleaseMutex(stRCUSem))
        {
            return FALSE;
        }
        return FALSE;
    }
    printf("[%s][%d] virtBaseAddr=0x%08"DTC_MS_VIRT_x" phyBaseSize=0x%08"DTC_MS_PHY_x"\n",
        __FUNCTION__, __LINE__, virtBaseAddr, phyBaseSize);

    gIR_fd = open("/dev/ir", O_RDWR);
    if (gIR_fd < 0)
    {
        fprintf(stderr, "Fail to open IR Kernal Module\n");
        if(FALSE == MsOS_ReleaseMutex(stRCUSem))
        {
            return FALSE;
        }
        return FALSE;
    }

    ioctl(gIR_fd, MDRV_IR_INIT);

    MS_IR_DelayTime time;
    time.u32_1stDelayTimeMs = IR_1ST_DELAY_TIME_MS;
    time.u32_2ndDelayTimeMs = IR_2ST_DELAY_TIME_MS;
    ioctl(gIR_fd, MDRV_IR_SET_DELAYTIME, &time);

    if(FALSE == MsOS_ReleaseMutex(stRCUSem))
        {
            return FALSE;
        }
    return TRUE;
}

static MS_BOOL _write_PM_data_to_File(char* pu8FilePath, char* u8Buf, MS_U32 u32Size)
{
    FILE* fp = NULL;
    MS_U32 u32WriteSize = 0;

    fp = MsFS_Fopen(pu8FilePath, "w");
    if(!fp)
    {
        printf("[%s][%d] open file(%s) fail\n", __FUNCTION__, __LINE__, pu8FilePath);
        return FALSE;
    }

    u32WriteSize=MsFS_Fwrite((void *)u8Buf, 1, u32Size,fp);

    if (u32WriteSize == 0)
    {
        printf("[%s][%d] write file(%s) fail\n", __FUNCTION__, __LINE__, pu8FilePath);
        return FALSE;
    }

    MsFS_Fflush(fp);
    MsFS_Fclose(fp);

    return TRUE;
}

#endif

#if (DEMO_PM_STR_LINUX_TEST == 1) || (DEMO_PM_HK_LINUX_TEST == 1)
void strKernelProcess(void)
{
    int writePowerStatefd = 0;
    int readPowerStatefd = 0;
    char buf = 0xff;
    writePowerStatefd = open("/sys/power/state", O_RDWR);
    if (writePowerStatefd < 0) {
        printf("open /sys/power/state failed");
    } else {
        if (write(writePowerStatefd, "mstr", 4) < 0) {
            printf("write mstr /sys/power/state failed");
        } else {
            readPowerStatefd = open("/sys/power/state_entering", O_RDWR);
            if (readPowerStatefd < 0) {
                printf("open /sys/power/state_entering failed");
            } else {
                while (true) {
                    lseek(readPowerStatefd, 0, SEEK_SET);
                    if (read(readPowerStatefd, &buf, 1)) {
                        if (buf == '0') {
                            break;
                        }
                    }
                    usleep(10000);
                }
                close(readPowerStatefd);
            }
        }
        close(writePowerStatefd);
    }
}
#elif (DEMO_PM_STR_eCos_TEST == 1)
void strKernelProcess(void)
{
   MApi_eCos_STR_Enter();
}

#endif

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
MS_BOOL Demo_PM_RTC_Init(void)
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
MS_BOOL Demo_PM_RTC_GetTime(void)
{
    MS_U32 u32SysTime = MDrv_PM_RTCGetSystemTime();
    printf("Get system time = %"DTC_MS_U32_u" \n", u32SysTime);

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
MS_BOOL Demo_PM_RTC_SetTime(MS_U32 *pu32secs)
{
    MDrv_PM_RTCSetSystemTime(*pu32secs);
    printf("Set system time = %"DTC_MS_U32_u" \n", *pu32secs);

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
MS_BOOL Demo_PM_RTC_SetMatchTime(MS_U32 *pu32secs)
{
    MDrv_PM_RTCSetMatchTime(*pu32secs);
    printf("Set match time = %"DTC_MS_U32_u" \n", *pu32secs);
    return TRUE;
}

#if (DEMO_PM_HK_LINUX_TEST == 1) || (DEMO_PM_HK_eCos_TEST == 1)

MS_BOOL Demo_PM_PowerSaving_Driver_Suspend(void)
{
    _bVE_init = msAPI_VE_CheckInitStatus();
    if (_bVE_init)
    {
        MS_U32 u32VEmute = 1;
        Demo_VE_SetVideoMute(&u32VEmute);
        Demo_VE_Exit();
    }

#if (DEMO_XC_TEST)
    _bXC_init_0 = msAPI_XC_CheckInitStatus_EX(&_stXC_DeviceId[0]);
    if (_bXC_init_0)
    {
        //generate black screen
        Demo_XC_GenerateBlackVideo(&_stXC_DeviceId[0].u32Id, TRUE, MAIN_WINDOW);

        Demo_XC_Exit(&_stXC_DeviceId[0].u32Id);
    }
#if (DEMO_XC_DUALXC_TEST == 1)
    _bXC_init_1 = msAPI_XC_CheckInitStatus_EX(&_stXC_DeviceId[1]);
    if (_bXC_init_1)
    {
        //generate black screen
        Demo_XC_GenerateBlackVideo(&_stXC_DeviceId[0].u32Id, TRUE, SUB_WINDOW);
        Demo_XC_GenerateBlackVideo(&_stXC_DeviceId[1].u32Id, TRUE, MAIN_WINDOW);
        Demo_XC_Exit(&_stXC_DeviceId[1].u32Id);
    }
#endif
#endif //DEMO_XC_TEST

    MApi_DigiTuner_SetPowerState(E_POWER_SUSPEND);

#if (DEMO_PM_HK_LINUX_TEST == 1)
    MApi_DMX_Suspend();
#endif

    MApi_AUDIO_ExitAudioSystem();

    return TRUE;
}

#if (DEMO_PM_HK_LINUX_TEST == 1) || (DEMO_PM_HK_eCos_TEST == 1)
MS_BOOL Demo_PM_PowerSaving_Driver_Resume(void)
{
    MApi_DigiTuner_SetPowerState(E_POWER_RESUME);
    memcpy((void*)MS_PA2KSEG1((MS_U32)MEM_ADR_BY_MIU(TSP_FW_BUF_ADR, TSP_FW_BUF_MEMORY_TYPE)/*u8TSPFirmware*/) , u8TSPFirmware, sizeof(u8TSPFirmware));
    MApi_DMX_Resume(MEM_ADR_BY_MIU(TSP_FW_BUF_ADR, TSP_FW_BUF_MEMORY_TYPE), sizeof(u8TSPFirmware));

#if (DEMO_XC_TEST == 1)
    if(_bXC_init_0)
    {
        Demo_XC_Init(&_stXC_DeviceId[0].u32Id);
    }
#if (DEMO_XC_DUALXC_TEST == 1)
    if(_bXC_init_1)
    {
        Demo_XC_Init(&_stXC_DeviceId[1].u32Id);
    }
#endif
#endif //    DEMO_XC_TEST

    if(_bVE_init)
    {
        Demo_VE_Init();
    }

    return TRUE;
}
#endif

//------------------------------------------------------------------------------
/// @brief Entering Power Saving Mode
/// @param[in] u8Mode Power Saving Mode Selection. 0: Normal 1: Suspend to RAM(STR)
/// @return TRUE - Process success
/// @return FALSE - Process fail
/// @sa
/// @note
/// Command: \b PM_PowerSaving \n
///
/// Sample Command: \b PM_PowerSaving \n
///
//------------------------------------------------------------------------------
MS_BOOL Demo_PM_PowerSaving(MS_U8 *u8Mode)
{
#if (DEMO_PM_HK_eCos_TEST == 1)

    MS_U32 u32Irq;

    // PM will run in polling mode without interrupt
    u32Irq = MsOS_DisableAllInterrupts();

    MDrv_IR_HK_Init();

    printf("enter STR\n");

    //appDemo_TurnOffLED();
    MDrv_PWSV_Init();

    Demo_PM_PowerSaving_Driver_Suspend();

    if(*u8Mode == 0)
    {
        MDrv_PWSV_PowerDown_Run_In_Cache(E_PWSV_MODE_POWER_SAVING);
    }
    else if (*u8Mode == 1)
    {
        MDrv_PWSV_PowerDown_Run_In_Cache(E_PWSV_MODE_SUSPEND_TO_RAM);
    }
    else
    {
        printf("Wrong Power-saving Mode Selected\n");
    }

    // Re-Init UART
    MDrv_UART_Init(E_UART_PIU_UART0, 115200);
    MDrv_UART_SetRxMode(TRUE,UartBuf,64,UartCB);

    //appDemo_TurnOnLED();
    // Restore interrupt
    MsOS_RestoreAllInterrupts(u32Irq);

    Demo_PM_PowerSaving_Driver_Resume();

    printf("exit STR\n");

#elif (DEMO_PM_HK_LINUX_TEST == 1)

    MS_U32 u32BaseAddr = 0;
    MS_U32 u32BaseSize = 0;

    char pm_Iaddr[20] = {0};
    char pm_Isize[20] = {0};
    char pm_Daddr[20] = {0};
    char pm_Dsize[20] = {0};
    char pm_run[20] = {0};

    sprintf(pm_Iaddr, "0x%x", STR_AP_BIN_ADR);
    sprintf(pm_Daddr, "0x%x", STR_AP_BIN_ADR+0x8000);
    sprintf(pm_Isize, "0x%x", 0x8000);
    sprintf(pm_Dsize, "0x%x", 0x8000);
    sprintf(pm_run, "0x%x", STR_AP_BIN_ADR+0x400);

    //load pm code to dram
    memcpy((void*)MS_PA2KSEG0(STR_AP_BIN_ADR), pm_bin, sizeof(pm_bin));

    //set addr, size to kernel
    if (_write_PM_data_to_File("/proc/pm/pm_Iaddr", (char*)&pm_Iaddr, sizeof(pm_Iaddr)) == FALSE)
    {
        return FALSE;
    }

    if (_write_PM_data_to_File("/proc/pm/pm_Isize", pm_Isize, sizeof(pm_Isize)) == FALSE)
    {
        return FALSE;
    }

    if (_write_PM_data_to_File("/proc/pm/pm_Daddr", pm_Daddr, sizeof(pm_Daddr)) == FALSE)
    {
        return FALSE;
    }

    if (_write_PM_data_to_File("/proc/pm/pm_Dsize", pm_Dsize, sizeof(pm_Dsize)) == FALSE)
    {
        return FALSE;
    }

    if (_write_PM_data_to_File("/proc/pm/pm_run", pm_run, sizeof(pm_run)) == FALSE)
    {
        return FALSE;
    }

    //init IR
    IrInput_init();

    //set standby mode
    MDrv_MMIO_GetBASE(&u32BaseAddr, &u32BaseSize, MS_MODULE_PM);

    if(*u8Mode == 0)
    {
        //normal standby
        _PM_WriteReg((u32BaseAddr | (PM_HK_STATUS_REG*2)), PM_HK_STATUS_NORMAL);
    }
    else if(*u8Mode == 1)
    {
        //fast standby
        _PM_WriteReg((u32BaseAddr | (PM_HK_STATUS_REG*2)), PM_HK_STATUS_STR);
    }

    printf("enter STR\n");

    Demo_PM_PowerSaving_Driver_Suspend();

    strKernelProcess();

    Demo_PM_PowerSaving_Driver_Resume();

    printf("exit STR\n");

#endif

    return TRUE;
}

#else

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
MS_BOOL Demo_PM_Init(MS_U32 *u32Mode)
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

    if (_PM_Init(_PM_MODE, E_DDI_PM_PWR_MODE_NORMAL) == FALSE)
    {
        printf("Fail - [%s][%d] \n",__FUNCTION__,__LINE__);
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
MS_BOOL Demo_PM_PowerDown(void)
{
    //MDrv_SERFLASH_SetPowerState(E_POWER_SUSPEND); //wait utopia1 driver to implement
    if (_PM_PowerDown(FALSE) == FALSE)
    {
        printf("Fail - [%s][%d] \n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    return TRUE;
}

static void PM_STR_StoreRegTbl(MS_U16 *SrcAddr)
{
    MS_U16 *u16SrcAddr = SrcAddr;
    MS_U16 val,reg;
    MS_U32 u32Width = 0;
    MS_U32 u32BaseAddr = 0;
    MS_BOOL bRestoreFlag = 0;

    while (1)
    {
        val = *(u16SrcAddr+1);
        reg = *u16SrcAddr;

        switch (reg)
        {
        case CHIPCFG_CMD_BASE: // write 2 byte
            u32BaseAddr = val << 16;
            u32Width = 16;
            break;

        case CHIPCFG_CMD_BASE_8BITS: // write byte
            u32BaseAddr = val << 16;
            u32Width = 8;
            break;

        case CHIPCFG_CMD_END:
            return;

        case CHIPCFG_CMD_WAIT:
            break;

        case CHIPCFG_CMD_STORE_TO_RECOVER:
            bRestoreFlag = val;
            break;

        default: // start to store
            if (bRestoreFlag == 0x1)
            {
                    MS_VIRT virtAddr = 0;
                    MS_PHY phySize = 0;
                    if (u32BaseAddr == 0xbf000000)
                    {
                        MDrv_MMIO_GetBASE(&virtAddr, &phySize, MS_MODULE_PM);
                    }
                    else
                    {
                        MDrv_MMIO_GetBASE(&virtAddr, &phySize, MS_MODULE_HW);
                        virtAddr += u32BaseAddr & 0xfffff;
                    }
                    //printf("@@--u32BaseAddr = 0x%x ; virtAddr = 0x%x ; phySize = 0x%x\n", (unsigned int)u32BaseAddr, (unsigned int)virtAddr, (unsigned int)phySize);

                    if(u32Width == 8)
                    {
                        MS_U8 u8Value = 0;

                        if ((reg >> 1) & 0x1)
                        {
                            u8Value = *(volatile MS_U8 *)((virtAddr + reg) - 1);
                        }
                        else
                        {
                            u8Value = *(volatile MS_U8 *)(virtAddr + reg);
                        }

                        *(u16SrcAddr+1) = (u8Value << 8) | 0xff;
                    }
                    else
                    {
                        MS_U16 u16Value;

                        u16Value = *(volatile MS_U16 *)(virtAddr + reg);

                        *(u16SrcAddr+1) = u16Value;
                    }
            }
            break;
        }
        u16SrcAddr += 2;
    }
}

#if (DEMO_PM_USE_STR_FLAG_CHECK_TEST == 1)
static MS_U16 _PM_STR_ReadReg(MS_U32 reg)
{
    return *(volatile unsigned short *)(reg);
}

static void _PM_STR_WriteReg(MS_U32 reg, MS_U16 value)
{
    *(volatile unsigned short *)(reg) = value;
}

static void _PM_STR_INTHandler(InterruptNum eIntNum)
{
    MS_U32 u32BaseAddr = 0;
    MS_U32 u32BaseSize = 0;
    MS_U16 u16RegData = 0;

    MDrv_MMIO_GetBASE(&u32BaseAddr, &u32BaseSize, MS_MODULE_IRQ);

    u16RegData = _PM_STR_ReadReg((u32BaseAddr | STR_IRQ_FORCE_REG));

    u16RegData &= (~STR_IRQ_FORCE_CLEAR_NUM);

    _PM_STR_WriteReg((u32BaseAddr | STR_IRQ_FORCE_REG), u16RegData);

    STR_Wakeup_Flag = HK_STR_WAKEUP;
}

static void _PM_Check_STR_Wakeup_Flag(void)
{
    MS_U32 u32BaseAddr = 0;
    MS_U32 u32BaseSize = 0;

    MDrv_MMIO_GetBASE(&u32BaseAddr, &u32BaseSize, MS_MODULE_PM);

    __asm__ volatile (
        "lw $11, %0\n"
        ::"m"(STR_Wakeup_Flag):"$11"
    );

    MsOS_AttachInterrupt(STR_IRQ_FORCE_NUM, (InterruptCb)_PM_STR_INTHandler);
    MsOS_EnableInterrupt(STR_IRQ_FORCE_NUM);

    _PM_STR_WriteReg((u32BaseAddr | STR_STATUS_REG), HK_STR_READY);

    __asm__ volatile (
        "STR_CHECK:\n"
        "lw $11, %0\n"
        "li $12, 0xFF19\n"
        "bne $11, $12, STR_CHECK"
        ::"m"(STR_Wakeup_Flag):"$11", "$12"
    );

    STR_Wakeup_Flag = HK_STR_IDLE;

    MsOS_DisableInterrupt(STR_IRQ_FORCE_NUM);
    MsOS_DetachInterrupt(STR_IRQ_FORCE_NUM);
}
#endif

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
MS_BOOL Demo_PM_FastStandby(void)
{
    MS_U32 Enter_SR_Addr = (MS_U32)PM51_STR_ADR;
    MS_U32 Exit_SR_Addr = (MS_U32)(PM51_STR_ADR + 0x1000);
    MS_U32 HWIP_Close_Addr = (MS_U32)(PM51_STR_ADR + 0x2000);
    MS_U32 HWIP_Open_Addr = (MS_U32)(PM51_STR_ADR + 0x3000);
    MBX_Result eMbxResult;

#if (DEMO_PM_USE_STR_FLAG_CHECK_TEST == 1)
    MS_U32 u32BaseAddr = 0;
    MS_U32 u32BaseSize = 0;

    MDrv_MMIO_GetBASE(&u32BaseAddr, &u32BaseSize, MS_MODULE_PM);

    STR_Wakeup_Flag = HK_STR_IDLE;
    _PM_STR_WriteReg((u32BaseAddr | STR_STATUS_REG), HK_STR_IDLE);
#endif

    printf("Fast Standby!\n");
    _PM_MODE = E_DDI_PM_MODE_SRAM;

    if (_PM_Init(_PM_MODE, E_DDI_PM_PWR_MODE_FASTSTANDBY) == FALSE)
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

#if (DEMO_PM_USB_POWER_SWITCH_TEST == 1)
    mdrv_gpio_set_low(GPIO_USB0_POWER);
#endif

    PM_STR_StoreRegTbl((MS_U16 *)HWIP_OPEN);

#if 0
    {
        int i;
        MS_U16 *TempPtr = (MS_U16 *)HWIP_OPEN;

        for (i=0; i<(sizeof(HWIP_OPEN) >> 1); i+=2)
        {
            switch (TempPtr[i])
            {
                case CHIPCFG_CMD_BASE:
                    printf("##[16Bits]\n");
                    break;
                case CHIPCFG_CMD_BASE_8BITS:
                    printf("##[8Bits]\n");
                    break;
                case CHIPCFG_CMD_END:
                    break;
                case CHIPCFG_CMD_WAIT:
                    break;
                default:
                    printf("REG[0x%x] ; VALUE[0x%x]\n", TempPtr[i]>>1, TempPtr[i+1]);
            }

        }
    }
#endif
    //_MDrv_Power_FillRegisterTable();
    memcpy(pEntSelf, MIU_ENTER_SR, sizeof(MIU_ENTER_SR));
    memcpy(pExtSelf, MIU_EXIT_SR, sizeof(MIU_EXIT_SR));
    memcpy(pClsHWIP, HWIP_CLOSE, sizeof(HWIP_CLOSE));
    memcpy(pOpnHWIP, HWIP_OPEN, sizeof(HWIP_OPEN));
#if 0
    printf("@@--HWIP_CLOSE size = %d ; HWIP_OPEN size = %d\n",
        sizeof(HWIP_CLOSE), sizeof(HWIP_OPEN));
#endif
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

#if (DEMO_PM_USE_STR_FLAG_CHECK_TEST == 1)
    _PM_Check_STR_Wakeup_Flag();
#endif
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

#if (DEMO_PM_USB_POWER_SWITCH_TEST == 1)
    mdrv_gpio_set_high(GPIO_USB0_POWER);
#endif
    return TRUE;

}

//------------------------------------------------------------------------------
/// @brief Sleep with STR and wait for wakeup source
/// @param[in] None
/// @return TRUE - Process success
/// @return FALSE - Process fail
/// @sa
/// @note
/// Command: \b PM_STR \n
///
/// Sample Command: \b PM_STR \n
///
//------------------------------------------------------------------------------
#if (DEMO_PM_STR_LINUX_TEST == 1 || DEMO_PM_STR_eCos_TEST == 1)
MS_BOOL Demo_PM_STR_Driver_Suspend(void)
{
   MDrv_GPIO_SetPowerState(E_POWER_SUSPEND);
   MDrv_HWI2C_SetPowerState(E_POWER_SUSPEND);
   MDrv_MIU_SetPowerState(E_POWER_SUSPEND);
   //secondary driver suspend

   //MDrv_PWM_SetPowerState(E_POWER_SUSPEND);
   MDrv_SYS_SetPowerState(E_POWER_SUSPEND);
   //MDrv_VBI_SetPowerState(E_POWER_SUSPEND);
   MDrv_BDMA_SetPowerState(E_POWER_SUSPEND);
   #if (DEMO_DIP_TEST == 1)
   MApi_XC_DIP_SetPowerState(E_POWER_SUSPEND);
   #endif
   MApi_XC_SetPowerState(E_POWER_SUSPEND);
   #if (DEMO_XC_HDMIRX_TEST == 1)
   MDrv_HDMI_SetPowerState(E_POWER_SUSPEND);
   #endif
   MApi_HDMITx_SetPowerState(E_POWER_SUSPEND);
   //MDrv_AVD_SetPowerState(E_POWER_SUSPEND);
   //MDrv_PCMCIA_SetPowerState(E_POWER_SUSPEND);
   //MDrv_MBX_SetPowerState(E_POWER_SUSPEND);
   //MDrv_SC_SetPowerState(E_POWER_SUSPEND);
   //MApi_PNL_SetPowerState(E_POWER_SUSPEND);
   MDrv_MVOP_SetPowerState(E_POWER_SUSPEND);
   MDrv_MSPI_SetPowerState(E_POWER_SUSPEND);
   //MDrv_PWS_SetPowerState(E_POWER_SUSPEND);
   //MDrv_SAR_SetPowerState(E_POWER_SUSPEND);
   #if (DEMO_SPI_TEST == 1)
   MDrv_SERFLASH_SetPowerState(E_POWER_SUSPEND);
   #endif
   //MDrv_VIF_SetPowerState(E_POWER_SUSPEND);
   //MApi_CEC_SetPowerState(E_POWER_SUSPEND);
   MApi_XC_ACE_SetPowerState(E_POWER_SUSPEND);
   MDrv_TVE_SetPowerState(E_POWER_SUSPEND);
   Mapi_DAC_SetPowerState(E_POWER_SUSPEND);
   MDrv_PQ_SetPowerState(E_POWER_SUSPEND);
   MApi_DigiTuner_SetPowerState(E_POWER_SUSPEND);
   MApi_DMX_Suspend();
   #if (DEMO_PM_STR_eCos_TEST == 1)
   MDrv_Usb_STR_Off(0);
   MDrv_Usb_STR_Off(1);
   #endif
   MApi_AUDIO_ExitAudioSystem();
   MDrv_MBX_DeInit(TRUE);
   MDrv_DSCMB2_SetPowerState(E_POWER_SUSPEND);
   return TRUE;

}

MS_BOOL Demo_PM_STR_Driver_Resume(void)
{
    MDrv_GPIO_SetPowerState(E_POWER_RESUME);
    MDrv_HWI2C_SetPowerState(E_POWER_RESUME);
    MDrv_MIU_SetPowerState(E_POWER_RESUME);
    //secondary driver suspend
    //MDrv_PWM_SetPowerState(E_POWER_RESUME);
    MDrv_SYS_SetPowerState(E_POWER_RESUME);
    //MDrv_VBI_SetPowerState(E_POWER_RESUME);
    MDrv_BDMA_SetPowerState(E_POWER_RESUME);
    #if (DEMO_DIP_TEST == 1)
    MApi_XC_DIP_SetPowerState(E_POWER_RESUME);
    #endif
    MApi_XC_SetPowerState(E_POWER_RESUME);
    #if (DEMO_XC_HDMIRX_TEST == 1)
    MDrv_HDMI_SetPowerState(E_POWER_RESUME);
    #endif
    MApi_HDMITx_SetPowerState(E_POWER_RESUME);
    //MDrv_AVD_SetPowerState(E_POWER_RESUME);
    //MDrv_PCMCIA_SetPowerState(E_POWER_RESUME);
    //MDrv_MBX_SetPowerState(E_POWER_RESUME);
    //MDrv_SC_SetPowerState(E_POWER_RESUME);
    //MApi_PNL_SetPowerState(E_POWER_RESUME);
    MDrv_MVOP_SetPowerState(E_POWER_RESUME);
    MDrv_MSPI_SetPowerState(E_POWER_RESUME);
    //MDrv_PWS_SetPowerState(E_POWER_RESUME);
    //MDrv_SAR_SetPowerState(E_POWER_RESUME);
    #if (DEMO_SPI_TEST == 1)
    MDrv_SERFLASH_SetPowerState(E_POWER_RESUME);
    #endif
    //MDrv_VIF_SetPowerState(E_POWER_RESUME);
    //MApi_CEC_SetPowerState(E_POWER_RESUME);
    MApi_XC_ACE_SetPowerState(E_POWER_RESUME);
    Mapi_DAC_SetPowerState(E_POWER_RESUME);
    MDrv_TVE_SetPowerState(E_POWER_RESUME);
    MDrv_PQ_SetPowerState(E_POWER_RESUME);
    MApi_DigiTuner_SetPowerState(E_POWER_RESUME);

    memcpy((void*)MS_PA2KSEG1((MS_U32)MEM_ADR_BY_MIU(TSP_FW_BUF_ADR, TSP_FW_BUF_MEMORY_TYPE)/*u8TSPFirmware*/) , u8TSPFirmware, sizeof(u8TSPFirmware));
    MApi_DMX_Resume(MEM_ADR_BY_MIU(TSP_FW_BUF_ADR, TSP_FW_BUF_MEMORY_TYPE), sizeof(u8TSPFirmware));
    #if (DEMO_PM_STR_eCos_TEST == 1)
    // Re-Init UART
    MDrv_UART_Init(E_UART_PIU_UART0, 115200);
    MDrv_UART_SetRxMode(TRUE,UartBuf,64,UartCB);
    MDrv_Usb_STR_On(0);
    MDrv_Usb_STR_On(1);
    #endif
    MDrv_DSCMB2_SetPowerState(E_POWER_RESUME);
    return TRUE;

}

MS_BOOL Demo_PM_STR(void)
{
    if (_PM_Init(E_DDI_PM_MODE_SRAM, E_DDI_PM_PWR_MODE_STR) == FALSE)
    {
        printf("Fail - [%s][%d] \n",__FUNCTION__,__LINE__);
        return FALSE;
    }


    if (_PM_PowerDown(TRUE) == FALSE)
    {
        printf("Fail - [%s][%d] \n",__FUNCTION__,__LINE__);
        return FALSE;
    }

#ifdef MSOS_TYPE_ECOS
     MApi_eCos_STR_Init();
#endif
    Demo_PM_STR_Driver_Suspend();
    strKernelProcess();
    Demo_PM_STR_Driver_Resume();
    return TRUE;
}

#endif

//------------------------------------------------------------------------------
/// @brief Process RTC wakeup from Standby/PowerDown mode.
/// @param[in] u32WakeUpTime, SysTime in second.
/// @return TRUE - Process success
/// @return FALSE - Process fail
/// @giga
/// @note
/// Command: \b PM_RTC_Wakeup \n
///
/// Sample Command: \b PM_RTC_Wakeup 1000 100 0 \n
///
//------------------------------------------------------------------------------
MS_BOOL Demo_PM_RTC_WakeUp(MS_U32 *WakeUpTime, MS_U32 *SysTime, MS_U8 *mode)
{
    PM_RtcParam* PmRtcParam = (PM_RtcParam*)_PA2VA(PM51_MEM_ADR);
    MS_U32 u32mode;

    printf("Wake up in %"DTC_MS_U32_d" seconds in mode %d\n" , (*WakeUpTime-*SysTime), *mode);
    PmRtcParam->u8PmRtcIndex = 0;                                    // Use RTC0 to wake up
    PmRtcParam->u32RtcCtrlWord = XTAL_CLOCK_FREQ;     // Set Clock freq
    PmRtcParam->u32RtcSetMatchCounter = *WakeUpTime;  // Set wakeup time in second after sleep
    PmRtcParam->u32RtcGetMatchCounter = 0;
    PmRtcParam->u32RtcSetCounter = *SysTime;          // Set the counter to zero
    PmRtcParam->u32RtcGetCounter = 0;

    MDrv_PM_RtcInit((PM_RtcParam*)((MS_U32)PM51_MEM_ADR));
    MDrv_PM_RtcSetMatchCounter((PM_RtcParam*)((MS_U32)PM51_MEM_ADR));
    MDrv_PM_RtcSetCounter((PM_RtcParam*)((MS_U32)PM51_MEM_ADR));

    if (*mode == 0)
    {
        u32mode = E_DDI_PM_MODE_SRAM;
        Demo_PM_Init(&u32mode);
        Demo_PM_PowerDown();       // WakeUp from PowerDown mode.
    }
    else
    {
        Demo_PM_FastStandby();     // WakeUp from Standby mode.
    }

    return TRUE;
}

#if (DEMO_PM_WOL_TEST == 1)
MS_BOOL Demo_PM_ConfigWOL(MS_BOOL *On, char*mac)
{
    int i=0;
    _EnableWOL = *On;

    printf("_EnableWOL = %d\n", _EnableWOL);
    if(strlen(mac)<12) return FALSE;
    memset(_u8PmWakeMACAddress, 0, PM_MAX_BUF_WAKE_MAC_ADDRESS);
    for (i=0; i<12;i++)
    {
        MS_U8 charCode = mac[i];
        if(charCode>='A' && charCode<='F')
           charCode = (0xa+charCode-'A');
        else if(charCode>='a' && charCode<='f')
           charCode = (0xa+charCode-'a');
        else if(charCode>='0' && charCode<='9')
           charCode = charCode-'0';
        _u8PmWakeMACAddress[i/2]=(_u8PmWakeMACAddress[i/2]<<4)+charCode;
    }
    printf("mac address is set to %02x:%02x:%02x:%02x:%02x:%02x \n",
           _u8PmWakeMACAddress[0],
           _u8PmWakeMACAddress[1],
           _u8PmWakeMACAddress[2],
           _u8PmWakeMACAddress[3],
           _u8PmWakeMACAddress[4],
           _u8PmWakeMACAddress[5]);
    return TRUE;
}
#endif
#endif

#endif
