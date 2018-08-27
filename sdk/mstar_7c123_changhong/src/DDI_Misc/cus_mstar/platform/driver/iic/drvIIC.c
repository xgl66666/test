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
#define FORCE_MY_GPIO_MODE 1
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
#define _IIC_C_

#ifdef __KERNEL__
#include <linux/string.h>
#else
#include <string.h>
#endif
#include "Board.h"
#include "drvIIC.h"
#include "drvGPIO.h"
#include "drvMMIO.h"

#ifdef __KERNEL__
#include <linux/vmalloc.h>
#define free vfree
#define malloc(size) vmalloc((size))
#endif

#if SWI2C_INUSE
static MS_U8* pu8SwIICPort = NULL;
static MS_U8 u8SwIICCount = 0;
static MS_BOOL bSwIICSetupDone = FALSE;
static SWI2C_ReadMode eSW_Read_Mode_Default = E_SWI2C_READ_MODE_DIRECTION_CHANGE;
#endif
static MS_S32 s32IICMutex = -1;
static HWI2C_ReadMode eHW_Read_Mode_Default = E_HWI2C_READ_MODE_DIRECTION_CHANGE;

#define MDrv_IIC_MUTEX_TIMEOUT (2000)
#define MDrv_IIC_LOCK()      do{if(s32IICMutex!=-1)MsOS_ObtainMutex(s32IICMutex, MDrv_IIC_MUTEX_TIMEOUT);}while(0);
#define MDrv_IIC_UNLOCK()    do{if(s32IICMutex!=-1)MsOS_ReleaseMutex(s32IICMutex);}while(0);

#if SWI2C_INUSE
static MS_BOOL _SwIICPort_mapping(MS_IIC_PORT ePort, MS_U8* pu8SwI2CBus)
{
    MS_U8 i;

    if(pu8SwIICPort == NULL)
        return FALSE;

    for(i=0;i<u8SwIICCount;i++)
    {
       if(ePort == (*(pu8SwIICPort + i)))
       {
          *pu8SwI2CBus = i;
          return TRUE;
       }
    }

    return FALSE;
}

MS_BOOL MDrv_IIC_SetupSwPort(MS_U8 u8BusCount, MS_IIC_SwBus* pstConfig)
{
    SWI2C_BusCfg stBusCfg[E_MS_IIC_SW_PORT_MAX - E_MS_IIC_SW_PORT_0 + 1];
    MS_IIC_SwBus* pstConfigI = NULL;
    MS_U8 i;

    if(!bSwIICSetupDone)
    {
        if(u8BusCount > (sizeof(stBusCfg)/sizeof(SWI2C_BusCfg)))
        {
           printf("Number of SW I2C Bus Over Limitation\n");
           printf("Max count of SW I2C Bus is %d", (int)(sizeof(stBusCfg)/sizeof(SWI2C_BusCfg)));
           return FALSE;
        }
        
        if(pu8SwIICPort == NULL)
        {
            pu8SwIICPort = (MS_U8 *)malloc(sizeof(MS_U8) * u8BusCount);
        }

        for(i=0;i<u8BusCount;i++)
        {
            pstConfigI = pstConfig + i;
            if((pstConfigI->ePort >= E_MS_IIC_PORT_NOSUP) ||\
               (pstConfigI->ePort < E_MS_IIC_SW_PORT_0))
            {
                printf("Wrong parameter for SW IIC setup\n");
                return FALSE;
            }

            stBusCfg[i].padSCL = pstConfigI->u16SclPad;
            stBusCfg[i].padSDA = pstConfigI->u16SdaPad;
            stBusCfg[i].defDelay= pstConfigI->u16Delay;
            *(pu8SwIICPort + i) = (MS_U8)pstConfigI->ePort;
            MApi_SWI2C_SetBusReadMode(i,eSW_Read_Mode_Default);
        }

        MApi_SWI2C_Init(stBusCfg, u8BusCount);
        bSwIICSetupDone = TRUE;
        u8SwIICCount = u8BusCount;
    }

    return TRUE;
}
#endif

void MDrv_IIC_InitPort0(MS_IIC_PORT ePort)
{
    MS_BOOL bResult = FALSE;
    HWI2C_UnitCfg hwI2CCfg;
    memset(&hwI2CCfg, 0x00, sizeof(HWI2C_UnitCfg));
#ifdef BD_CUS75_MBQ213A    
#if (MS_BOARD_TYPE_SEL == BD_CUS75_MBQ213A) // To improve tuner and demod lock time
    hwI2CCfg.eSpeed = E_HWI2C_HIGH;
#else
    hwI2CCfg.eSpeed = E_HWI2C_SLOW;//E_HWI2C_NORMAL
#endif
#else
    hwI2CCfg.eSpeed = E_HWI2C_SLOW;//E_HWI2C_NORMAL
#endif

    hwI2CCfg.sI2CPin.u32Reg = 0;
    hwI2CCfg.sI2CPin.u8BitPos = 0;
    hwI2CCfg.sI2CPin.bEnable = 0;
    hwI2CCfg.ePort = ePort;
    bResult = MDrv_HWI2C_Init(&hwI2CCfg);
    if(bResult)
    {
        printf("MDrv_IIC_InitPort%d: OK\n", (int)ePort);
    }
    else
    {
        printf("MDrv_IIC_InitPort%d: FAIL\n", (int)ePort);
        return;
    }

    MDrv_HWI2C_SelectPort(hwI2CCfg.ePort);
    MDrv_HWI2C_SetClk(hwI2CCfg.eSpeed);
    MDrv_HWI2C_SetReadMode(eHW_Read_Mode_Default);

}

void MDrv_IIC_InitPort1(MS_IIC_PORT ePort)
{
    MS_BOOL bResult;
    HWI2C_UnitCfg hwI2CCfg;
    memset(&hwI2CCfg, 0x00, sizeof(HWI2C_UnitCfg));
#ifdef BD_CUS75_MBQ213A
#if (MS_BOARD_TYPE_SEL == BD_CUS75_MBQ213A) // To improve tuner and demod lock time
    hwI2CCfg.eSpeed = E_HWI2C_HIGH;
#else
    hwI2CCfg.eSpeed = E_HWI2C_SLOW;//E_HWI2C_NORMAL
#endif
#else
    hwI2CCfg.eSpeed = E_HWI2C_SLOW;//E_HWI2C_NORMAL
#endif

    hwI2CCfg.sI2CPin.u32Reg = 0;
    hwI2CCfg.sI2CPin.u8BitPos = 0;
    hwI2CCfg.sI2CPin.bEnable = 0;
    hwI2CCfg.ePort = ePort;
    bResult = MDrv_HWI2C_Init(&hwI2CCfg);
    if(bResult)
    {
        printf("MDrv_IIC_InitPort1: OK\n");
    }
    else
    {
        printf("MDrv_IIC_InitPort1: FAIL\n");
        return;
    }

    MDrv_HWI2C_SelectPort1(hwI2CCfg.ePort);
    MDrv_HWI2C_SetClkP1(hwI2CCfg.eSpeed);
    MDrv_HWI2C_SetReadModeP1(eHW_Read_Mode_Default);

}

void MDrv_IIC_Init(MS_IIC_PORT ePort)
{
    printf("MDrv_IIC_Init  PORT  %d \n", ePort);

    if(ePort >= E_MS_IIC_PORT_NOSUP)
    {
        printf("MDrv_IIC_Init: FAIL, Invalid Port number\n");
        return;
    }

    if(s32IICMutex == -1)
    {
        s32IICMutex = MsOS_CreateMutex(E_MSOS_FIFO, "Mdrv_IIC_Mutex", MSOS_PROCESS_SHARED);
        if(s32IICMutex == -1)
        {
            printf("MDrv_IIC_Init: FAIL, Create Mutex fail\n");
            return;
        }
    }

    if ((ePort >= E_MS_IIC_SW_PORT_0) && (ePort < E_MS_IIC_PORT_NOSUP))
    {
    #if SWI2C_INUSE
        if(!bSwIICSetupDone)
        {
            printf("MDrv_IIC_Init: FAIL, this is SW I2C port, setup I2C SW port by MDrv_IIC_SetupSwPort\n");
        }
    #else
        printf("MDrv_IIC_Init: FAIL, SW I2C is disabled\n");
    #endif
    }
    else
    {
         MDrv_IIC_InitPort0(ePort);
    }

}



MS_BOOL MDrv_IIC_ReadBytesPort0(MS_U16 u16BusNumSlaveID, MS_U8 ucSubAdr, MS_U8* paddr, MS_U16 ucBufLen, MS_U8* pBuf)
{
    MS_BOOL bRet = FALSE;

    MDrv_IIC_LOCK();
    bRet = MDrv_HWI2C_ReadBytes(u16BusNumSlaveID, (MS_U32)ucSubAdr, paddr, (MS_U32)ucBufLen, pBuf);
    MDrv_IIC_UNLOCK();

    return bRet;
}


MS_BOOL MDrv_IIC_ReadBytesPort1(MS_U16 u16BusNumSlaveID, MS_U8 ucSubAdr, MS_U8* paddr, MS_U16 ucBufLen, MS_U8* pBuf)
{
    MS_BOOL bRet = FALSE;

    MDrv_IIC_LOCK();
    bRet = MDrv_HWI2C_ReadBytesP1(u16BusNumSlaveID, (MS_U32)ucSubAdr, paddr, (MS_U32)ucBufLen, pBuf);
    MDrv_IIC_UNLOCK();

    return bRet;

}

MS_BOOL MDrv_IIC_ReadBytes(MS_IIC_PORT ePort, MS_U16 u16BusNumSlaveID, MS_U8 ucSubAdr, MS_U8* paddr, MS_U16 ucBufLen, MS_U8* pBuf)
{
    MS_U16 u16PortIndex;
    MS_BOOL bRet = FALSE;
#if SWI2C_INUSE    
    MS_U8 u8SwI2CBus = 0;
#endif

    MDrv_IIC_LOCK();

    if(ePort < E_MS_IIC_SW_PORT_0)
    {
        //printf("IIC HW mode Read\n");
        u16PortIndex = (ePort/8)<<8;
        u16BusNumSlaveID |= u16PortIndex;
        bRet = MDrv_HWI2C_ReadBytes(u16BusNumSlaveID, (MS_U32)ucSubAdr, paddr, (MS_U32)ucBufLen, pBuf);
    }
    else if(ePort < E_MS_IIC_PORT_NOSUP)
    {
       //printf("IIC SW mode Read\n");
#if SWI2C_INUSE
       if(bSwIICSetupDone)
      {
        if(_SwIICPort_mapping(ePort, &u8SwI2CBus))
        {
            u16PortIndex = (u8SwI2CBus<<8);
            u16BusNumSlaveID |= u16PortIndex;
            bRet = MApi_SWI2C_ReadBytes(u16BusNumSlaveID, ucSubAdr, paddr, ucBufLen, pBuf);
        }
      }
#endif       
    }
    
    MDrv_IIC_UNLOCK();
    
    return bRet;
}

MS_BOOL MDrv_IIC_ReadWithMode(MS_IIC_PORT ePort, MS_U16 u16BusNumSlaveID, MS_U8 ucSubAdr, MS_U8* paddr, MS_U16 ucBufLen, MS_U8* pBuf, MS_IIC_ReadMode eMode)
{
    MS_U16 u16PortIndex;
    MS_BOOL bRet = TRUE;
#if SWI2C_INUSE    
    MS_U8 u8SwI2CBus = 0;
#endif

    MDrv_IIC_LOCK();

    if(ePort < E_MS_IIC_SW_PORT_0)
    {
        //printf("IIC HW mode Read\n");
        u16PortIndex = (ePort/8)<<8;
        u16BusNumSlaveID |= u16PortIndex;
        bRet &= MDrv_HWI2C_SelectPort((HWI2C_PORT)ePort);
        bRet &= MDrv_HWI2C_SetReadMode((HWI2C_ReadMode)eMode);
        bRet &= MDrv_HWI2C_ReadBytes(u16BusNumSlaveID, (MS_U32)ucSubAdr, paddr, (MS_U32)ucBufLen, pBuf);
        MDrv_HWI2C_SetReadMode(eHW_Read_Mode_Default);
    }
    else if(ePort < E_MS_IIC_PORT_NOSUP)
    {
       //printf("IIC SW mode Read\n");
 #if SWI2C_INUSE      
       if(bSwIICSetupDone)
      {
        if(_SwIICPort_mapping(ePort, &u8SwI2CBus))
        {
            u16PortIndex = (u8SwI2CBus<<8);
            u16BusNumSlaveID |= u16PortIndex;
            bRet &= MApi_SWI2C_SetBusReadMode(u8SwI2CBus, (SWI2C_ReadMode)eMode);
            bRet &= MApi_SWI2C_ReadBytes(u16BusNumSlaveID, ucSubAdr, paddr, ucBufLen, pBuf);
            bRet &= MApi_SWI2C_SetBusReadMode(u8SwI2CBus, eSW_Read_Mode_Default);
        }
      }
 #endif      
    }
    
    MDrv_IIC_UNLOCK();
    
    return bRet;
}


MS_BOOL MDrv_IIC_WriteBytesPort0(MS_U16 u16BusNumSlaveID, MS_U8 AddrCnt, MS_U8* pu8addr, MS_U16 u16size, MS_U8* pBuf)
{
    MS_BOOL bRet = FALSE;

    MDrv_IIC_LOCK();
    bRet = MDrv_HWI2C_WriteBytes(u16BusNumSlaveID, (MS_U32)AddrCnt, pu8addr, (MS_U32)u16size, pBuf);
    MDrv_IIC_UNLOCK();

    return bRet;
}

MS_BOOL MDrv_IIC_WriteBytesPort1(MS_U16 u16BusNumSlaveID, MS_U8 AddrCnt, MS_U8* pu8addr, MS_U16 u16size, MS_U8* pBuf)
{
    MS_BOOL bRet = FALSE;

    MDrv_IIC_LOCK();
    bRet = MDrv_HWI2C_WriteBytesP1(u16BusNumSlaveID, (MS_U32)AddrCnt, pu8addr, (MS_U32)u16size, pBuf);
    MDrv_IIC_UNLOCK();

    return bRet;
}


MS_BOOL MDrv_IIC_WriteBytes(MS_IIC_PORT ePort, MS_U16 u16BusNumSlaveID, MS_U8 AddrCnt, MS_U8* pu8addr, MS_U16 u16size, MS_U8* pBuf)
{
   MS_U16 u16PortIndex;
   MS_BOOL bRet = FALSE;
#if SWI2C_INUSE    
   MS_U8 u8SwI2CBus = 0;
#endif

   MDrv_IIC_LOCK();

   if(ePort < E_MS_IIC_SW_PORT_0)
   {
        u16PortIndex = (ePort/8)<<8;
        u16BusNumSlaveID |= u16PortIndex;
        bRet = MDrv_HWI2C_WriteBytes(u16BusNumSlaveID, (MS_U32)AddrCnt, pu8addr, (MS_U32)u16size, pBuf);
   }
   else if(ePort < E_MS_IIC_PORT_NOSUP)
   {
 #if SWI2C_INUSE    
      if(bSwIICSetupDone)
      {
        if(_SwIICPort_mapping(ePort, &u8SwI2CBus))
        {
            u16PortIndex = (u8SwI2CBus<<8);
            u16BusNumSlaveID |= u16PortIndex;
            bRet = MApi_SWI2C_WriteBytes(u16BusNumSlaveID, AddrCnt, pu8addr, u16size, pBuf);
        }
      }
 #endif      
   }
    
   MDrv_IIC_UNLOCK();
    
   return bRet;
}



MS_BOOL MDrv_IIC_Read(MS_U16 u16BusNumSlaveID, MS_U8* pu8addr, MS_U8 AddrCnt, MS_U8* pBuf,  MS_U16 u16size)
{
    MS_BOOL bRet;
    
    MDrv_IIC_LOCK();
    bRet = MDrv_HWI2C_ReadBytes(u16BusNumSlaveID, (MS_U32)AddrCnt, pu8addr, (MS_U32)u16size, pBuf);
    MDrv_IIC_UNLOCK();
    return bRet;
}

MS_BOOL MDrv_IIC1_Read(MS_U16 u16BusNumSlaveID, MS_U8* pu8addr, MS_U8 AddrCnt, MS_U8* pBuf,  MS_U16 u16size)
{
    MS_BOOL bRet;
    
    MDrv_IIC_LOCK();
    bRet = MDrv_HWI2C_ReadBytesP1(u16BusNumSlaveID, (MS_U32)AddrCnt, pu8addr, (MS_U32)u16size, pBuf);
    MDrv_IIC_UNLOCK();
    return bRet;

}

MS_BOOL MDrv_IIC_Write(MS_U16 u16BusNumSlaveID, MS_U8* pu8addr, MS_U8 AddrCnt, MS_U8* pBuf,  MS_U16 u16size)
{
    MS_BOOL bRet;

    bRet = MDrv_IIC_WriteBytesPort0(u16BusNumSlaveID,AddrCnt, pu8addr,u16size, pBuf);

    return bRet;

}

MS_BOOL MDrv_IIC1_Write(MS_U16 u16BusNumSlaveID, MS_U8* pu8addr, MS_U8 AddrCnt, MS_U8* pBuf,  MS_U16 u16size)
{
    MS_BOOL bRet;
    
    bRet = MDrv_IIC_WriteBytesPort1(u16BusNumSlaveID,AddrCnt, pu8addr,u16size, pBuf);

    return bRet;

}


MS_BOOL MDrv_IIC_SelectCLK(MS_IIC_PORT ePort, HWI2C_CLKSEL eSpeed)
{
    MS_BOOL bRet;

    MDrv_IIC_LOCK();
    if(ePort >= E_MS_IIC_PORT_1)
    {       
      bRet = MDrv_HWI2C_SetClkP1(eSpeed);
    }
    else
    {
      bRet =MDrv_HWI2C_SetClk(eSpeed);
    }
    MDrv_IIC_UNLOCK();

    return bRet;
}


void IIC_Stop(void)
{
    printf("IIC_N");
}

MS_BOOL IIC_Start(void)
{
    printf("IIC_N");
    return 0;
}

MS_BOOL IIC_SendByte(MS_U8 u8dat)
{
    printf("IIC_N");
    return 0;
}

MS_IIC_PORT getI2CPort(MS_U8 drv_frontend_index)
{
    HWI2C_PORT ePort;
    switch(drv_frontend_index)
    {
        case 0:
            ePort = FRONTEND_TUNER_PORT0;
        break;
        case 1:
            ePort = FRONTEND_TUNER_PORT1;
        break;
        case 2:
            ePort = FRONTEND_TUNER_PORT2;
        break;
        case 3:
            ePort = FRONTEND_TUNER_PORT3;
        break;
        default:
            ePort = FRONTEND_TUNER_PORT0;
            break;
    }
    return ePort;
}

MS_IIC_PORT getDishI2CPort(MS_U8 drv_frontend_index)
{
    HWI2C_PORT ePort;
    switch(drv_frontend_index)
    {
        case 0:
        #ifdef DISH_IIC_PORT
            ePort = DISH_IIC_PORT;
        #else
            ePort = FRONTEND_TUNER_PORT0;
        #endif
        break;

        case 1:
        #ifdef DISH_IIC_PORT1
             ePort = DISH_IIC_PORT1;
        #else
             ePort = FRONTEND_TUNER_PORT1;
        #endif

        break;
        case 2:
        #ifdef DISH_IIC_PORT2
             ePort = DISH_IIC_PORT2;
        #else
             ePort = FRONTEND_TUNER_PORT2;
        #endif

        break;
        case 3:
        #ifdef DISH_IIC_PORT3
              ePort = DISH_IIC_PORT3;
        #else
              ePort = FRONTEND_TUNER_PORT3;
        #endif

        break;
        default:
            ePort = FRONTEND_TUNER_PORT0;
            break;
    }
    return ePort;
}

#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/module.h>   // for EXPORT_SYMBOL
MODULE_LICENSE("LGPL");
EXPORT_SYMBOL(MDrv_IIC_ReadBytes);
EXPORT_SYMBOL(MDrv_IIC_WriteBytes);
EXPORT_SYMBOL(MDrv_IIC_Init);
EXPORT_SYMBOL(MDrv_IIC_SelectCLK);
#endif

#undef _IIC_C_

