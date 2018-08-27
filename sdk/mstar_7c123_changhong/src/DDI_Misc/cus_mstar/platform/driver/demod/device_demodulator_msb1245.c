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
// 6. Nothing contained herein shall be consTRUEd as by implication, estoppels
//    or otherwise:
//    (a) conferring any license or right to use MStar name, trademark, service
//        mark, symbol or any other identification;
//    (b) obligating MStar or any of its affiliates to furnish any person,
//        including without limitation, you and your customers, any assistance
//        of any kind whatsoever, or any information; or
//    (c) conferring any license or right under any intellectual property right.
//
// 7. These terms shall be governed by and consTRUEd in accordance with the laws
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
/**********************************************************************
Copyright (c) 2006-2009 MStar Semiconductor, Inc.
All rights reserved.
Unless otherwise stipulated in writing, any and all information contained
herein regardless in any format shall remain the sole proprietary of
MStar Semiconductor Inc. and be kept in strict confidence
(MStar Confidential Information) by the recipient.
Any unauthorized act including without limitation unauthorized disclosure,
copying, use, reproduction, sale, distribution, modification, disassembling,
reverse engineering and compiling of the contents of MStar Confidential
Information is unlawful and strictly prohibited. MStar hereby reserves the
rights to any and all damages, losses, costs and expenses resulting therefrom.
 **********************************************************************/

#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#else
#include <string.h>
#include <math.h>
#endif
#include "MsCommon.h"
#include "drvIIC.h"
#include "MsOS.h"
#include "drvTuner.h"
#include "drvDemod.h"
#include "drvDemodNull.h"
#include "drvGPIO.h"
#include "drvSYS.h"
#define USE_UTOPIA

#if IF_THIS_DEMOD_INUSE(DEMOD_MSB1245)
#include "drvDMD_EXTERN_MSB124x.h"
#include "device_demodulator_msb1245.h"
#include "apiDMX.h"
#include "drvDMD_common.h"
#include "drvMSPI.h"
static MS_U8      u8max_dev_num = 0;
static MS_U8      _u8ToneBurstFlag=0;

#define TS_CLK_SEL         0x00
#define TS_DATA_SWAP       0x00

static MS_BOOL bTS_SERIAL = FALSE;
static MS_BOOL* pDemodRest = NULL;
static MS_BOOL* pbTimerEn = NULL;

MDvr_CofdmDmd_CONFIG* pstMSB1245 = NULL;
MDvr_CofdmDmd_CONFIG MSB1245_Init =
{
    FALSE, //bInited
    FALSE, //bOpen
    -1,    //s32_MSB1245_Mutex
    0,     //u8sramCode
    0,     //u32CurrFreq
    FALSE, //bFECLock
    0,     //u8ScanStatus
    {NULL},//MSB1245_InitParam
    0xF2,  //u8SlaveID
    FALSE, //bIsDVBS2
    -1,    //s32DemodHandle
    0,     //u32DmxInputPath;
    FALSE, //bIsMCP_DMD
    FALSE, //bDiSeqc_Tx22K_Off
};


SLAVE_ID_USAGE* pstMSB1245_slave_ID_TBL = NULL;
static SLAVE_ID_USAGE MSB1245_possible_slave_ID[3] =
{
    {0xF2, FALSE},
    {0xD2, FALSE},
    {0xFF, FALSE}
};


static  MS_U16                       _u16BlindScanStartFreq =0;
static  MS_U16                       _u16BlindScanEndFreq   =0;
static  MS_U16                       _u16TunerCenterFreq    =0;
static  MS_U16                       _u16ChannelInfoIndex    =0;
static  MS_U16                       _u16LockedSymbolRate=0;
static  MS_U16                       _u16LockedCenterFreq=0;
static  MS_U32                       _u32DemodStartTime=0;
static  MS_U16                       _u16ChannelInfoArray[2][1000];

static MS_U8 u8MSB1245_DevCnt = 0;
static MS_BOOL msb1245_SetReg(MS_U8 u8DemodIndex, MS_U16 u16Addr, MS_U8 u8Data);

//interrupt usage
static MS_U8  u8StackBuffer[MSB1245_EVT_TASK_STACK_SIZE];
static MS_S32 _s32DmdEventTaskId = -1;
static MS_S32 _s32DmdEventId = -1;
static MS_U32 u32Events;
static MS_U8 u8TS_VLD_cnt = 0;

static MS_BOOL _mdrv_dmd_msb1245_get_IntNum(MS_U8 u8DemodIndex, InterruptNum* pIntNum);

static MS_BOOL get_i2c_port(MS_U8 u8DemodIndex, MS_IIC_PORT* ePort)
{
    MDvr_CofdmDmd_CONFIG *pMSB1245;

    if(pstMSB1245 == NULL)
        return FALSE;
    else
    {
        pMSB1245 = pstMSB1245 + u8DemodIndex;
        *ePort = pMSB1245->MSB1245_InitParam.stDMDCon.eI2C_PORT;
        return TRUE;
    }
}

static MS_S32* ps32msb1245_TimerID = NULL;
static void _mdrv_dmd_msb1245_timer_cb(MS_U32 u32StTimer, MS_U32 u32TimerID)
{
    MDvr_CofdmDmd_CONFIG *pMSB1245 = NULL;
    EN_LOCK_STATUS eLockStatus = E_DEMOD_UNLOCK;
    InterruptNum IntNum;
    MS_U8 i;
    MS_S32* ps32TimerID = NULL;

    for(i = 0; i<u8max_dev_num; i++)
    {
       ps32TimerID = ps32msb1245_TimerID + i;
       if(*ps32TimerID == (MS_S32)u32TimerID)
       {
         MSB1245_Demod_GetLock(i, &eLockStatus);
         if(pstMSB1245 != NULL)
         {
            pMSB1245 = (pstMSB1245 + i);
            if((pMSB1245->MSB1245_InitParam.fpCB != NULL) && (eLockStatus != E_DEMOD_LOCK))
            {
                pMSB1245->MSB1245_InitParam.fpCB(pMSB1245->MSB1245_InitParam.u8FrontendIndex,2);
                MsOS_StopTimer (*ps32TimerID);
                if(_mdrv_dmd_msb1245_get_IntNum(i, &IntNum))
                {
                    u8TS_VLD_cnt = 0;
                    MsOS_EnableInterrupt(IntNum);
                }
            }
         }
         break;
       }
    }
}


#if(TIMING_VERIFICATION == 1)
static void show_timer(void)
{
    DMD_DBG(("***************************\n"));
    DMD_DBG(("[tmm1]t2-t1 = %ld (%ld - %ld)\n", tmm_2 - tmm_1, tmm_2, tmm_1));
    DMD_DBG(("[tmm2]t4-t3 = %ld (%ld - %ld)\n", tmm_4 - tmm_3, tmm_4, tmm_3));
    DMD_DBG(("[tmm3]t6-t5 = %ld (%ld - %ld)\n", tmm_6 - tmm_5, tmm_6, tmm_5));
    DMD_DBG(("[tmm4]t8-t7 = %ld (%ld - %ld)\n", tmm_8 - tmm_7, tmm_8, tmm_7));
    DMD_DBG(("[tmm5]t10-t9 = %ld (%ld - %ld)\n", tmm_10 - tmm_9, tmm_10, tmm_9));
    DMD_DBG(("[tmm6]t12-t11 = %ld (%ld - %ld)\n", tmm_12 - tmm_11, tmm_12, tmm_11));
    DMD_DBG(("[tmm7]t14-t13 = %ld (%ld - %ld)\n", tmm_14 - tmm_13, tmm_14, tmm_13));
    DMD_DBG(("[tmm8]t16-t15 = %ld (%ld - %ld)\n", tmm_16 - tmm_15, tmm_16, tmm_15));
    DMD_DBG(("[tmm9]t18-t17 = %ld (%ld - %ld)\n", tmm_18 - tmm_17, tmm_18, tmm_17));
    DMD_DBG(("[tmm10]t20-t19 = %ld (%ld - %ld)\n", tmm_20 - tmm_19, tmm_20, tmm_19));
    DMD_DBG(("[tmm11]lock_time# = %ld (%ld - %ld)\n", tmm_14 - u32StartTime, tmm_14, u32StartTime));
    DMD_DBG(("[tmm12]lock-setf = %ld (%ld - %ld)\n", tmm_14 - tmm_11, tmm_14, tmm_11));
    DMD_DBG(("[tmm13]lock-loaddsp = %ld (%ld - %ld)\n", tmm_14 - tmm_9, tmm_14, tmm_9));
    DMD_DBG(("***************************\n"));
}
#endif

#if MS_DVBS_INUSE
MS_BOOL MSB1245_DiSEqC_Init(MS_U8 u8DemodIndex);
#endif

static float g_MSB1245_fSNR = 0.0;
static MS_BOOL _mdrv_dmd_msb1245_get_IntNum(MS_U8 u8DemodIndex, InterruptNum* pIntNum)
{
    MDvr_CofdmDmd_CONFIG *pMSB1245 = NULL;

    if(pstMSB1245 == NULL)
        return FALSE;
    else
        pMSB1245 = pstMSB1245 + u8DemodIndex;

    if(pMSB1245->MSB1245_InitParam.stDMDCon.u32TSVLDInterrupt == DMD_CON_INFO_NOT_DEFINE)
        return FALSE;
    else
    {
        *pIntNum = (InterruptNum)pMSB1245->MSB1245_InitParam.stDMDCon.u32TSVLDInterrupt;
    }

    return TRUE;
}

static MS_BOOL _mdrv_dmd_msb1245_IntNum2DemodIndex(MS_U8* pu8DemodIndex, InterruptNum IntNum)
{
    MDvr_CofdmDmd_CONFIG *pMSB1245 = NULL;
    MS_U8 u8DemodIndex;

    if(pstMSB1245 == NULL)
        return FALSE;

    for(u8DemodIndex=0;u8DemodIndex<u8max_dev_num;u8DemodIndex++)
    {
        pMSB1245 = pstMSB1245 + u8DemodIndex;
        if(pMSB1245->MSB1245_InitParam.stDMDCon.u32TSVLDInterrupt ==IntNum)
        {
            *pu8DemodIndex = u8DemodIndex;
            return TRUE;
        }
    }

    return FALSE;
}


static void _mdrv_dmd_msb1245_event_task(MS_U32 argc, void *argv)
{
  MDvr_CofdmDmd_CONFIG *pMSB1245 = NULL;
  MS_U32 u32WaitEventFlag;
  MS_U8 u8DemodIndex = 0;

  u32WaitEventFlag = MSB1245_EVT_PORT0INT|MSB1245_EVT_PORT1INT|MSB1245_EVT_PORT2INT|MSB1245_EVT_PORT3INT;
  do
  {
     MsOS_WaitEvent(_s32DmdEventId, u32WaitEventFlag,&u32Events, E_OR_CLEAR, MSOS_WAIT_FOREVER);
     switch(u32Events)
     {
         case MSB1245_EVT_PORT0INT:
            u8DemodIndex = 0;
            break;
         case MSB1245_EVT_PORT1INT:
            u8DemodIndex = 1;
            break;
         case MSB1245_EVT_PORT2INT:
            u8DemodIndex = 2;
            break;
         case MSB1245_EVT_PORT3INT:
            u8DemodIndex = 3;
            break;
         default:
            break;
     }

     if(pstMSB1245 != NULL)
     {
         pMSB1245 = (pstMSB1245 + u8DemodIndex);
         if(*(pbTimerEn+u8DemodIndex))
         {
             if(ps32msb1245_TimerID != NULL)
                 MsOS_StartTimer (*(ps32msb1245_TimerID + u8DemodIndex));
         }

         if(pMSB1245->MSB1245_InitParam.fpCB != NULL)
             pMSB1245->MSB1245_InitParam.fpCB(pMSB1245->MSB1245_InitParam.u8FrontendIndex,1);
     }

  }while(1);
}

static void _mdrv_dmd_msb1245_cb(InterruptNum irq)
{
    MS_U8 u8DemodIndex=0;

    u8TS_VLD_cnt++;
    if(u8TS_VLD_cnt < 3)
    {
        MsOS_EnableInterrupt(irq);
    }
    else
    {
        MsOS_DisableInterrupt(irq);
        if(_mdrv_dmd_msb1245_IntNum2DemodIndex(&u8DemodIndex, irq))
        {
            MsOS_ClearEvent(_s32DmdEventId, MSB1245_EVT_MASK);
            MsOS_SetEvent(_s32DmdEventId, (1<<u8DemodIndex));
        }
    }
}

static MS_BOOL MSB1245_Variables_alloc(void)
{
    MS_U8 i,j;
    MDvr_CofdmDmd_CONFIG *pMSB1245 = NULL;
    SLAVE_ID_USAGE* pSlaveIDTBL = NULL;
    MS_U8 u8MaxI2CPort;

    u8MaxI2CPort = (MS_U8)((E_MS_IIC_SW_PORT_0/8) + (E_MS_IIC_PORT_NOSUP - E_MS_IIC_SW_PORT_0));

    if(NULL == pstMSB1245)
    {
        pstMSB1245 = (MDvr_CofdmDmd_CONFIG *)malloc(sizeof(MDvr_CofdmDmd_CONFIG) * u8max_dev_num);
        if(NULL == pstMSB1245)
        {
            return FALSE;
        }
        else
        {
            for(i=0; i< u8max_dev_num; i++)
            {
                pMSB1245 = (pstMSB1245 + i);
                memcpy(pMSB1245, &MSB1245_Init, sizeof(MDvr_CofdmDmd_CONFIG));
                pMSB1245->MSB1245_InitParam.stDMDCon.u32TSVLDInterrupt = DMD_CON_INFO_NOT_DEFINE;
            }
        }
    }

    if(NULL == pDemodRest)
    {
        pDemodRest = (MS_BOOL*)malloc(sizeof(MS_BOOL) * u8max_dev_num);
        if(NULL == pDemodRest)
        {
            return FALSE;
        }
        else
        {
            for(i=0; i< u8max_dev_num; i++)
                *(pDemodRest + i) = TRUE;
        }
    }

    if(NULL == pbTimerEn)
    {
        pbTimerEn = (MS_BOOL*)malloc(sizeof(MS_BOOL) * u8max_dev_num);
        if(NULL == pbTimerEn)
        {
            return FALSE;
        }
        else
        {
            for(i=0; i< u8max_dev_num; i++)
                *(pbTimerEn + i) = FALSE;
        }
    }


    if(NULL == pstMSB1245_slave_ID_TBL)
    {
        pstMSB1245_slave_ID_TBL = (SLAVE_ID_USAGE *)malloc(sizeof(MSB1245_possible_slave_ID) * u8MaxI2CPort);
        if(NULL == pstMSB1245_slave_ID_TBL)
        {
            return FALSE;
        }
        else
        {
            for(i=0; i< u8MaxI2CPort; i++)
            {
                for(j=0; j< (sizeof(MSB1245_possible_slave_ID)/sizeof(SLAVE_ID_USAGE)); j++)
                {
                    pSlaveIDTBL = (pstMSB1245_slave_ID_TBL + i*sizeof(MSB1245_possible_slave_ID)/sizeof(SLAVE_ID_USAGE) + j);
                    memcpy(pSlaveIDTBL, &MSB1245_possible_slave_ID[j], sizeof(SLAVE_ID_USAGE));
                 }
            }

        }
    }

    if(NULL == ps32msb1245_TimerID)
    {
        ps32msb1245_TimerID = (MS_S32 *)malloc(sizeof(MS_S32) * u8max_dev_num);
        if(NULL == ps32msb1245_TimerID)
        {
            return FALSE;
        }
        else
        {
            for(i=0; i< u8max_dev_num; i++)
            {
              *(ps32msb1245_TimerID + i) = (-1);
            }
        }
    }

    return TRUE;

}

static void _variable_free(void** pp)
{
    if(NULL != *pp)
    {
        free(*pp);
        *pp = NULL;
    }
}

static MS_BOOL MSB1245_Variables_free(void)
{
    _variable_free((void*)&pstMSB1245);
    _variable_free((void*)&pDemodRest);
    _variable_free((void*)&pbTimerEn);
    _variable_free((void*)&pstMSB1245_slave_ID_TBL);
    _variable_free((void*)&ps32msb1245_TimerID);

    return TRUE;
}

#if defined(SUPPORT_MULTI_DEMOD) && (SUPPORT_MULTI_DEMOD == 1)
static MS_BOOL _GetDemodIndexByHandle(MS_S32 s32Handle, MS_U8* pu8DemodIndex)
{
    MS_U8 u8DemodIndex;
    MDvr_CofdmDmd_CONFIG *pMSB1245;
    for(u8DemodIndex=0; u8DemodIndex< u8max_dev_num; u8DemodIndex++)
    {
        pMSB1245 = (pstMSB1245 + u8DemodIndex);
        if(pMSB1245->s32DemodHandle == s32Handle)
        {
            *pu8DemodIndex = u8DemodIndex;
            return TRUE;
        }
    }
    return FALSE;
}
#endif

static void _msb1245_hw_reset(MS_U8 u8DemodIndex)
{
    MDvr_CofdmDmd_CONFIG *pMSB1245 = (pstMSB1245 + u8DemodIndex);
    int rstPin;

    if((pstMSB1245 == NULL) || (pDemodRest == NULL))
        return;

    rstPin = (int)(pMSB1245->MSB1245_InitParam.stDMDCon.u32HW_ResetPin);
    mdrv_gpio_set_high(rstPin);
    MsOS_DelayTask(5);
    mdrv_gpio_set_low(rstPin);
    MsOS_DelayTask(10);
    mdrv_gpio_set_high(rstPin);
    MsOS_DelayTask(5);

    pMSB1245->bInited = FALSE;
    pMSB1245->bOpen = FALSE;
    *(pDemodRest + u8DemodIndex) = TRUE;
    MDrv_DMD_MSB124X_Exit_EX(pMSB1245->s32DemodHandle);

}

static void _msb1245_power_onoff(MS_U8 u8DemodIndex, MS_BOOL bOn)
{
    MDvr_CofdmDmd_CONFIG *pMSB1245 = (pstMSB1245 + u8DemodIndex);
    int rst_pin;
    rst_pin = pMSB1245->MSB1245_InitParam.stDMDCon.u32HW_ResetPin;
    if(pMSB1245->bIsMCP_DMD)
    {
        if(bOn)
            mdrv_gpio_set_high(rst_pin);
        else
            mdrv_gpio_set_low(rst_pin);
    }
}

static void msb1245_SPIPAD_TS0_En(MS_BOOL bOnOff);
static void msb1245_SPIPAD_TS1_En(MS_BOOL bOnOff);
static void msb1245_SPIPAD_TS2_En(MS_BOOL bOnOff);
static fpSPIPAD_En SPIPAD_EN[3]= {msb1245_SPIPAD_TS0_En,\
                                   msb1245_SPIPAD_TS1_En,\
                                   msb1245_SPIPAD_TS2_En};

static void msb1245_SPIPAD_TS0_En(MS_BOOL bOnOff)
{
    if(bOnOff)
    {
        MDrv_SYS_SetPadMux(E_TS0_PAD_SET, E_MSPI_PAD_ON);
    }
    else
    {
       if(bTS_SERIAL)
           MDrv_SYS_SetPadMux(E_TS0_PAD_SET, E_SERIAL_IN);
       else
           MDrv_SYS_SetPadMux(E_TS0_PAD_SET, E_PARALLEL_IN);
    }

}

static void msb1245_SPIPAD_TS1_En(MS_BOOL bOnOff)
{
    if(bOnOff)
        MDrv_SYS_SetPadMux(E_TS1_PAD_SET, E_MSPI_PAD_ON);
    else
    {
       if(bTS_SERIAL)
          MDrv_SYS_SetPadMux(E_TS1_PAD_SET, E_SERIAL_IN);
       else
          MDrv_SYS_SetPadMux(E_TS1_PAD_SET, E_PARALLEL_IN);
    }
}

static void msb1245_SPIPAD_TS2_En(MS_BOOL bOnOff)
{
    if(bOnOff)
        MDrv_SYS_SetPadMux(E_TS2_PAD_SET, E_MSPI_PAD_ON);
    else
    {
       if(bTS_SERIAL)
          MDrv_SYS_SetPadMux(E_TS2_PAD_SET, E_SERIAL_IN);
       else
          MDrv_SYS_SetPadMux(E_TS2_PAD_SET, E_PARALLEL_IN);
    }
}

static MS_BOOL msb1245_AGC_Info(MS_U8 u8DemodIndex, MS_U8 u8dbg_mode, MS_U16* pu16Data)
{
    MS_BOOL bRet = TRUE;
    MS_U16 u16Address;
    MS_U8 u8Data;
    MDvr_CofdmDmd_CONFIG *pMSB1245 = (pstMSB1245 + u8DemodIndex);

    u16Address=0x3E26;
    bRet&=MSB1245_ReadReg(pMSB1245, u16Address, &u8Data);
    u8Data=(u8Data&0xF0)|u8dbg_mode;
    bRet&=MSB1245_WriteReg(pMSB1245, u16Address, u8Data);
    u16Address=0x3E05;
    bRet&=MSB1245_ReadReg(pMSB1245, u16Address, &u8Data);
    u8Data|=0x80;
    bRet&=MSB1245_WriteReg(pMSB1245, u16Address, u8Data);
    u16Address=0x3E29;
    bRet&=MSB1245_ReadReg(pMSB1245, u16Address, &u8Data);
    *pu16Data=u8Data;
    u16Address=0x3E28;
    bRet&=MSB1245_ReadReg(pMSB1245, u16Address, &u8Data);
    *pu16Data=((*pu16Data)<<8)|u8Data;
    u16Address=0x3E05;
    bRet&=MSB1245_ReadReg(pMSB1245, u16Address, &u8Data);
    u8Data&=~(0x80);
    bRet&=MSB1245_WriteReg(pMSB1245, u16Address, u8Data);
    if (bRet==FALSE)
    {
        DMD_ERR(("msb1245_AGC_Info Error!!! \n"));
    }

    return bRet;

}

static MS_BOOL msb1245_ADCPLL_IQ_SWAP(MS_U8 u8DemodIndex)
{
    MS_BOOL bRet = TRUE;
    MS_U16 u16Address;
    MS_U8 u8Data;
    MDvr_CofdmDmd_CONFIG *pMSB1245 = (pstMSB1245 + u8DemodIndex);

        //ADCPLL IQ swap
    u16Address= 0x0A03;
    bRet&= MSB1245_ReadReg(pMSB1245, u16Address, &u8Data);
    //if(MSB1245_ADCPLL_IQ_SWAP==1)
    if((pMSB1245->MSB1245_InitParam.pstTunertab->data == TUNER_AV2018) || \
       (pMSB1245->MSB1245_InitParam.pstTunertab->data == TUNER_AV2012) || \
       (pMSB1245->MSB1245_InitParam.pstTunertab->data == TUNER_RDA5815M))
    {
       if(pMSB1245->bIsMCP_DMD) // for MCP demod, MSB1245_ADCPLL_IQ_SWAP = 1
            u8Data |= (0x10);
       else
            u8Data &= (0xEF);
    }
    else
    {
       if(!pMSB1245->bIsMCP_DMD)
            u8Data |= (0x10);
       else
            u8Data &= (0xEF);
    }

    bRet&= MSB1245_WriteReg(pMSB1245, u16Address, u8Data);

    return bRet;
}

//=== mailbox function ===
static MS_BOOL  msb1245_ReadMailbox(MS_U8 u8DemodIndex, MS_U16 u16Addr, MS_U8* u8Data)
{
    MS_BOOL     status = true;
    MS_U8     cntl = 0x00;
    MS_U16    cntr = 0x00;
    //MS_U16    bank;
    MS_U16    addr;
    MS_U8     data;
    // MsOS_ObtainMutex(_s32_Demod_DVBS2_RW_Mutex, MSOS_WAIT_FOREVER);
    MDvr_CofdmDmd_CONFIG *pMSB1245 = (pstMSB1245 + u8DemodIndex);

    addr = REG_MB_ADDR_H;
    data = ((u16Addr >> 8)& 0xff) ;
    status &= MSB1245_WriteReg(pMSB1245, addr, data);

    addr = REG_MB_ADDR_L;
    data = u16Addr & 0xff;
    status &= MSB1245_WriteReg(pMSB1245, addr, data);

    addr = REG_MB_CNTL;
    status &= MSB1245_WriteReg(pMSB1245, addr, 0x03); // 3: param read

    addr = REG_MB_DATA ;
    status &=MSB1245_ReadReg(pMSB1245, addr, u8Data);

    do
    {
        addr = REG_MB_CNTL ;
        status &=MSB1245_ReadReg(pMSB1245, addr, &cntl);
        if (cntr++ > 0x7ff)
        {
            //PRINTE(("MSB131X_MB_WRITE_FAILURE\n"));
            //MsOS_ReleaseMutex(_s32_Demod_DVBS2_RW_Mutex);
            return FALSE;
        }
        addr = REG_MB_DATA ;
        status &=MSB1245_ReadReg(pMSB1245, addr, u8Data);
    }
    while(cntl != 0xff);
    //MsOS_ReleaseMutex(_s32_Demod_DVBS2_RW_Mutex);
    return status;
}

static MS_BOOL msb1245_SetInnerDBGPort(MS_U8 u8DemodIndex, MS_U8 u8Port)
{
    MS_BOOL bRet = TRUE;
    MS_U16 u16Address;
    MS_U8 u8Data;
    MDvr_CofdmDmd_CONFIG *pMSB1245 = (pstMSB1245 + u8DemodIndex);

    u16Address=0x1B09;
    bRet&=MSB1245_ReadReg(pMSB1245, u16Address, &u8Data);
    u8Data = (u8Data & (~0x3F))|u8Port;
    bRet&=MSB1245_WriteReg(pMSB1245, u16Address, u8Data);
    return bRet;
}

/*================================================
==                       IIC write/read interface
=================================================*/
MS_U16 MSB1245_Demod_ReadReg(MS_U8 u8DemodIndex,MS_U16 RegAddr);

MS_BOOL MSB1245_WriteBytes_demod(MS_U8 u8DemodIndex, MS_U8 u8AddrSize, MS_U8 *pu8Addr, MS_U16 u16Size, MS_U8 *pu8Data)
{
    MS_BOOL bRet = 0;
    MDvr_CofdmDmd_CONFIG *pMSB1245 = (pstMSB1245 + u8DemodIndex);
    MS_IIC_PORT ePort;

    if (!pMSB1245)
    {
        DMD_ERR(("pMSB124X error !\n"));
        return FALSE;
    }

    get_i2c_port(u8DemodIndex, &ePort);
    bRet = MDrv_IIC_WriteBytes(ePort, (MS_U16)pMSB1245->u8SlaveID, u8AddrSize, pu8Addr, u16Size, pu8Data);
    if (FALSE == bRet)
    {
        DMD_ERR(("Demod IIC write error\n"));
    }
    return bRet;
}

MS_BOOL MSB1245_ReadBytes_demod(MS_U8 u8DemodIndex, MS_U8 u8AddrSize, MS_U8 *pu8Addr, MS_U16 u16Size, MS_U8 *pu8Data)
{
    MS_BOOL bRet = 0;
    MDvr_CofdmDmd_CONFIG *pMSB1245 = (pstMSB1245 + u8DemodIndex);
    MS_IIC_PORT ePort;

    if (!pMSB1245)
    {
        DMD_ERR(("pMSB124X error !\n"));
        return FALSE;
    }

    get_i2c_port(u8DemodIndex, &ePort);
    bRet = MDrv_IIC_ReadBytes(ePort, (MS_U16)pMSB1245->u8SlaveID, u8AddrSize, pu8Addr, u16Size, pu8Data);

    if (FALSE == bRet)
    {
        DMD_ERR(("Demod IIC read error\n"));
    }
    return bRet;
}

typedef struct
{
    MS_BOOL(*WriteBytes)(MS_U8 u8DemodIndex, MS_U8 u8AddrSize, MS_U8 *pu8Addr, MS_U16 u16Size, MS_U8 *pu8Data);
    MS_BOOL(*ReadBytes)(MS_U8 u8DemodIndex, MS_U8 u8AddrSize, MS_U8 *pu8Addr, MS_U16 u16Size, MS_U8 *pu8Data);
} mapi_i2c;

static mapi_i2c DemodI2Chandler/*, SpiI2Chandler*/;
static mapi_i2c* mapi_i2c_GetI2C_Dev(void)
{
    mapi_i2c *handler;

    handler = &DemodI2Chandler;
    handler->WriteBytes = MSB1245_WriteBytes_demod;
    handler->ReadBytes = MSB1245_ReadBytes_demod;
    return handler;
}


static MS_BOOL msb1245_I2C_Access(eDMD_MSB124X_DemodI2CSlaveID eSlaveID, eDMD_MSB124X_DemodI2CMethod eMethod, MS_U8 u8AddrSize, MS_U8 *pu8Addr, MS_U16 u16Size, MS_U8 *pu8Data)
{
    MS_BOOL bRet = TRUE;
    mapi_i2c *i2c_iptr;
    MS_U8 u8DemodIndex = 0;

    i2c_iptr = mapi_i2c_GetI2C_Dev();

#if defined(SUPPORT_MULTI_DEMOD) && (SUPPORT_MULTI_DEMOD == 1)
    if(!_GetDemodIndexByHandle(MDrv_DMD_MSB124X_GetCurrentHandle(), &u8DemodIndex))
    {
        DMD_ERR(("124X_I2C_Access get demod index FAIL\n"));
    }
#endif

    if (i2c_iptr != NULL)
    {
        switch (eMethod)
        {
        case E_DMD_MSB124X_DEMOD_I2C_WRITE_BYTES:
            bRet = i2c_iptr->WriteBytes(u8DemodIndex,u8AddrSize, pu8Addr, u16Size, pu8Data);
            break;
        case E_DMD_MSB124X_DEMOD_I2C_READ_BYTES:
            bRet = i2c_iptr->ReadBytes(u8DemodIndex, u8AddrSize, pu8Addr, u16Size, pu8Data);
        default:
            break;
        }
    }
    else
    {
        bRet = FALSE;
    }

    return bRet;
}


/*================================================
==               demod  msb124x  write/read interface
=================================================*/
MS_BOOL MSB1245_WriteReg(MDvr_CofdmDmd_CONFIG *pMSB1245, MS_U16 u16Addr, MS_U8 u8Data)
{
    if(!pMSB1245) return FALSE;
    return MDrv_DMD_MSB124X_SetReg_EX(pMSB1245->s32DemodHandle, u16Addr, u8Data);
}

MS_BOOL MSB1245_WriteReg2bytes(MDvr_CofdmDmd_CONFIG *pMSB1245 ,MS_U16 u16Addr, MS_U16 u16Data)
{
    if(!pMSB1245) return FALSE;
    return MDrv_DMD_MSB124X_SetReg2Bytes_EX(pMSB1245->s32DemodHandle, u16Addr, u16Data);
}

MS_BOOL MSB1245_ReadReg(MDvr_CofdmDmd_CONFIG *pMSB1245 ,MS_U16 u16Addr, MS_U8 *pu8Data)
{
    if(!pMSB1245) return FALSE;
    return MDrv_DMD_MSB124X_GetReg_EX(pMSB1245->s32DemodHandle, u16Addr, pu8Data);
}

MS_BOOL  MSB1245_WriteDspReg(MDvr_CofdmDmd_CONFIG *pMSB1245 ,MS_U16 u16Addr, MS_U8 u8Data)
{
    if(!pMSB1245) return FALSE;
    return MDrv_DMD_MSB124X_SetDSPReg_EX(pMSB1245->s32DemodHandle, u16Addr, u8Data);
}

MS_BOOL MSB1245_ReadDspReg(MDvr_CofdmDmd_CONFIG *pMSB1245 ,MS_U16 u16Addr, MS_U8* pData)
{
    if(!pMSB1245) return FALSE;
    return MDrv_DMD_MSB124X_GetDSPReg_EX(pMSB1245->s32DemodHandle, u16Addr, pData);
}



/*================================================
==               demod  MSB124X  base interface
=================================================*/

MS_BOOL MSB1245_I2C_CH_Reset(MS_U8 u8DemodIndex, MS_U8 ch_num)
{
    MS_BOOL bRet = TRUE;
    MS_U8     data[5] = {0x53, 0x45, 0x52, 0x44, 0x42};
    mapi_i2c *iptr;


     iptr = mapi_i2c_GetI2C_Dev();

    // ch_num, 0: for MIU access, 3: for RIU access
    DMD_DBG(("[MSB1245][beg]MSB1245_I2C_CH_Reset, CH=0x%x\n", ch_num));

    if (iptr == NULL)
    {
        return FALSE;
    }

    if(pDemodRest == NULL)
        return FALSE;

    if (*(pDemodRest + u8DemodIndex))
    {
        // 8'hb2(SRID)->8,h53(PWD1)->8,h45(PWD2)->8,h52(PWD3)->8,h44(PWD4)->8,h42(PWD5)
        data[0] = 0x53;
        // Don't check Ack because this passward only ack one time for the first time.
        if(iptr->WriteBytes(u8DemodIndex, 0, NULL, 5, data))
            *(pDemodRest + u8DemodIndex) = FALSE;
        else
            return FALSE;
    }

    // 8'hb2(SRID)->8,h71(CMD)  //TV.n_iic_
    data[0] = 0x71;
    bRet &= iptr->WriteBytes(u8DemodIndex, 0, NULL, 1, data);

    // 8'hb2(SRID)->8,h81(CMD)  //TV.n_iic_sel_b0
    data[0] = ((ch_num & 0x01) != 0) ? 0x81 : 0x80;
    bRet &= iptr->WriteBytes(u8DemodIndex, 0, NULL, 1, data);

    // 8'hb2(SRID)->8,h83(CMD)  //TV.n_iic_sel_b1
    data[0] = ((ch_num & 0x02) != 0) ? 0x83 : 0x82;
    bRet &= iptr->WriteBytes(u8DemodIndex, 0, NULL, 1, data);

    // 8'hb2(SRID)->8,h84(CMD)  //TV.n_iic_sel_b2
    data[0] = ((ch_num & 0x04) != 0) ? 0x85 : 0x84;
    bRet &= iptr->WriteBytes(u8DemodIndex, 0, NULL, 1, data);

    // 8'hb2(SRID)->8,h53(CMD)  //TV.n_iic_ad_byte_en2, 32bit read/write
    data[0] = 0x53;
    bRet &= iptr->WriteBytes(u8DemodIndex, 0, NULL, 1, data);

    // 8'hb2(SRID)->8,h7f(CMD)  //TV.n_iic_sel_use_cfg
    data[0] = 0x7f;
    bRet &= iptr->WriteBytes(u8DemodIndex, 0, NULL, 1, data);

    /*
    // 8'hb2(SRID)->8,h35(CMD)  //TV.n_iic_use
    data[0] = 0x35;
    iptr->WriteBytes(0, NULL, 1, data);

    // 8'hb2(SRID)->8,h71(CMD)  //TV.n_iic_Re-shape
    data[0] = 0x71;
    iptr->WriteBytes(0, NULL, 1, data);
    */
    DMD_DBG(("[MSB1245][end]MSB1245_I2C_CH_Reset, CH=0x%x\n", ch_num));
    return bRet;
}


static void MSB1245_Driving_Control(MS_U8 u8DemodIndex, MS_BOOL bEnable)
{
    DMD_DBG(("%s(),%d\n", __FUNCTION__, __LINE__));
    MS_U8 u8Temp;
    MDvr_CofdmDmd_CONFIG *pMSB1245 = (pstMSB1245 + u8DemodIndex);
    if (!pMSB1245)
    {
        DMD_ERR(("pMSB1245 error !\n"));
        return ;
    }
    if ((!pMSB1245->bInited) || (!pMSB1245->bOpen))
    {
        DMD_ERR(("[%s]pMSB1245 have not inited !!!\n", __FUNCTION__));
        return ;
    }

    MSB1245_ReadReg(pMSB1245, 0x0958, &u8Temp);
    if (bEnable)
    {
        u8Temp = 0xFF;
    }
    else
    {
        u8Temp = 0x00;
    }
    MSB1245_WriteReg(pMSB1245, 0x0958, u8Temp);

    MSB1245_ReadReg(pMSB1245, 0x0959, &u8Temp);
    if (bEnable)
    {
        u8Temp = u8Temp | 0x0F;
    }
    else
    {
        u8Temp = u8Temp & (~0x0F);
    }
    MSB1245_WriteReg(pMSB1245, 0x0959, u8Temp);
}

static MS_BOOL MSB1245_DTV_Serial_Control(MS_U8 u8DemodIndex, MS_BOOL bEnable)
{
    DMD_DBG(("%s(),%d\n", __FUNCTION__, __LINE__));
    MDvr_CofdmDmd_CONFIG *pMSB1245 = (pstMSB1245 + u8DemodIndex);

    if (!pMSB1245)
    {
        DMD_ERR(("pMSB1245 error !"));
        return FALSE;
    }
    if ((!pMSB1245->bInited) || (!pMSB1245->bOpen))
    {
        DMD_ERR(("pMSB1245 have not inited !!!"));
        return FALSE;
    }


    MSB1245_Driving_Control(u8DemodIndex, 1);
    return TRUE;
}


/*================================================
==               demod  MSB124X iic bypass interface
=================================================*/
MS_BOOL MSB1245_IIC_Bypass_Mode(MS_U8 u8DemodIndex, MS_BOOL enable)
{
    MS_U8 u8Retry = 10, u8data = 0;
    MS_BOOL bResult;
    MDvr_CofdmDmd_CONFIG *pMSB1245 = (pstMSB1245 + u8DemodIndex);

    if (!pMSB1245)
    {
        DMD_ERR(("pMSB1245 error !\n"));
        return FALSE;
    }
    if ((!pMSB1245->bInited) || (!pMSB1245->bOpen))
    {
        DMD_ERR(("[%s]pMSB1245 have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }

    DMD_DBG((">>>MSB1245: IIC_bypass() set %x\n", enable));

    MSB1245_ReadReg(pMSB1245, 0x0900 + (0x28) * 2 + 1, &u8data);
    u8data = u8data & 0xFE;//clear bit 0
    MSB1245_WriteReg(pMSB1245, 0x0900 + (0x28) * 2 + 1, u8data);
    while (u8Retry--)
    {
        if (enable)
            bResult = MSB1245_WriteReg(pMSB1245, 0x0900 + (0x08) * 2, 0x10); // IIC by-pass mode on
        else
            bResult = MSB1245_WriteReg(pMSB1245, 0x0900 + (0x08) * 2, 0x00); // IIC by-pass mode off

        if (bResult)
            break;
    }
    return bResult;
}

MS_BOOL dvbs2_MSB1245_TS_Enable(MS_U8 u8DemodIndex, MS_BOOL bTsEnable)
{
    MS_BOOL bRet=TRUE;
    MS_U16  u16Address;
    MS_U8   u8Data;
    MDvr_CofdmDmd_CONFIG *pMSB1245 = (pstMSB1245 + u8DemodIndex);

    if (!pMSB1245)
    {
        DMD_ERR(("pMSB1245 error !\n"));
        return FALSE;
    }
    if ((!pMSB1245->bInited) || (!pMSB1245->bOpen))
    {
        DMD_ERR(("[%s]pMSB1245 have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }
    u16Address=0x2A40;
    bRet &= MSB1245_ReadReg(pMSB1245, u16Address, &u8Data);
#if MSB1245_TS_DATA_SWAP
    u8Data |= (0x20);
#else
    u8Data &= (~0x20);
#endif
    bRet&=MSB1245_WriteReg(pMSB1245, u16Address, u8Data);

    if (bTsEnable == TRUE)
    {
        bRet&= MSB1245_WriteReg(pMSB1245, 0x095A, 0xFF);
    }
    else
    {
        bRet&= MSB1245_WriteReg(pMSB1245, 0x095A, 0x00);
    }

    u16Address=0x0924;
    bRet &= MSB1245_ReadReg(pMSB1245, u16Address, &u8Data);
#if MSB1245_TS_INV
    u8Data |= (0x20);
#else
    u8Data &= (~0x20);
#endif
    bRet&=MSB1245_WriteReg(pMSB1245, u16Address, u8Data);
    return bRet;
}

MS_BOOL MSB1245_Lock_S(MS_U8 u8DemodIndex)
{
    MS_BOOL bRet=TRUE;
    MS_U16 u16Address;
    MS_U8 u8Data;
    MDvr_CofdmDmd_CONFIG *pMSB1245 = (pstMSB1245 + u8DemodIndex);
    if (!pMSB1245)
    {
        DMD_ERR(("pMSB1245 error !\n"));
        return FALSE;
    }
    if ((!pMSB1245->bInited) || (!pMSB1245->bOpen))
    {
        DMD_ERR(("[%s]pMSB1245 have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }

    u16Address=0x0990;
    bRet&=MSB1245_ReadReg(pMSB1245, u16Address, &u8Data);
    if ((u8Data&0x02)==0x00)
    {
        u16Address=0xB56;
        bRet&=MSB1245_ReadReg(pMSB1245, u16Address, &u8Data);
        if((u8Data>=15) && (u8Data!=0xcd))
        {
            if (u8Data==15)
            {
                pMSB1245->bIsDVBS2 = FALSE;   //S
                bRet = TRUE;
            }
            else if(u8Data==16)
            {
                pMSB1245->bIsDVBS2 = TRUE;    //S2
                bRet = TRUE;
            }
            else
            {
                bRet = FALSE;
            }
        }
        else
        {
            bRet = FALSE;
        }
    }
    else
    {
        bRet = TRUE;
    }
    dvbs2_MSB1245_TS_Enable(u8DemodIndex, 1);
    return bRet;
}


EN_LOCK_STATUS MSB1245_DTV_GetLockStatus(MS_U8 u8DemodIndex)
{

    if(MSB1245_Lock_S(u8DemodIndex))
        return E_DEMOD_LOCK;
    else
        return E_DEMOD_CHECKING;
}

void MSB1245_DTV_GetSNR(MS_U8 u8DemodIndex)
{
    MS_U8   status = TRUE;
    float   f_snr = (float)0.0;
    //DVBS
    MS_U16 u16Address;
    MS_U32 u32Data;
    MS_U8  u8Data;
    float  fvariance;

    MDvr_CofdmDmd_CONFIG *pMSB1245 = (pstMSB1245 + u8DemodIndex);

    if (!pMSB1245)
    {
        DMD_ERR(("pMSB1245 error !"));
        return ;
    }
    if ((!pMSB1245->bInited) || (!pMSB1245->bOpen))
    {
        DMD_ERR(("pMSB1245 have not inited !!!"));
        return ;
    }


    u16Address=0x1B08;
    status&=MSB1245_ReadReg(pMSB1245, u16Address, &u8Data);
    u8Data|=0x10;
    status&=MSB1245_WriteReg(pMSB1245, u16Address, u8Data);

    u16Address=0x1BA5;
    status&=MSB1245_ReadReg(pMSB1245, u16Address, &u8Data);
    u32Data=(u8Data&0x3F);

    u16Address=0x1BA4;
    status&=MSB1245_ReadReg(pMSB1245, u16Address, &u8Data);
    u32Data=(u32Data<<8)|u8Data;

    u16Address=0x1BA3;
    status&=MSB1245_ReadReg(pMSB1245, u16Address, &u8Data);
    u32Data=(u32Data<<8)|u8Data;

    u16Address=0x1BA2;
    status&=MSB1245_ReadReg(pMSB1245, u16Address, &u8Data);
    u32Data=(u32Data<<8)|u8Data;

    u16Address=0x1B08;
    status&=MSB1245_ReadReg(pMSB1245, u16Address, &u8Data);
    u8Data&=~(0x10);
    status&=MSB1245_WriteReg(pMSB1245, u16Address, u8Data);
    if (status==FALSE)
    {
        f_snr = 0;
    }
    fvariance=(float)u32Data/0x20000000;
    if (fvariance==0)
        fvariance=1;
    f_snr=10*log10(1/fvariance);
    g_MSB1245_fSNR = f_snr;
}


MS_BOOL MSB1245_DTV_GetPostBER(MS_U8 u8DemodIndex, float *p_postBer)
{
    float fber = 0;
    MS_U8 status = TRUE;
    MS_U16 u16Address;
    MS_U8  u8Data;
    MS_U16 u16BitErrPeriod;
    MS_U32 u32BitErr;

    MDvr_CofdmDmd_CONFIG *pMSB1245 = (pstMSB1245 + u8DemodIndex);

    if (!pMSB1245)
    {
        DMD_ERR(("pMSB1245 error !"));
        return FALSE;
    }
    if ((!pMSB1245->bInited) || (!pMSB1245->bOpen))
    {
        DMD_ERR(("pMSB1245 have not inited !!!"));
        return FALSE;
    }

    if (pMSB1245->bIsDVBS2 == FALSE)//S
    {
        u16Address=0x1c2C;
        status&=MSB1245_ReadReg(pMSB1245, u16Address, &u8Data);
        if (status==FALSE)
        {
            DMD_DBG(("dvbs2_MSB124X_DTV_GetSignalQuality fail!!! \n"));
            return 0;
        }
        if ((u8Data&0x02)!=0x02)
        {
            return 0;
        }
        u16Address=0x1c32;
        status&=MSB1245_ReadReg(pMSB1245, u16Address, &u8Data);
        u8Data|=0x80;
        status&=MSB1245_WriteReg(pMSB1245, u16Address, u8Data);
        u16Address=0x1c31;
        status&=MSB1245_ReadReg(pMSB1245, u16Address, &u8Data);
        u16BitErrPeriod=u8Data;
        u16Address=0x1c30;
        status&=MSB1245_ReadReg(pMSB1245, u16Address, &u8Data);
        u16BitErrPeriod=(u16BitErrPeriod<<8)|u8Data;
        u16Address=0x1c3D;
        status&=MSB1245_ReadReg(pMSB1245, u16Address, &u8Data);
        u32BitErr=u8Data;
        u16Address=0x1c3C;
        status&=MSB1245_ReadReg(pMSB1245, u16Address, &u8Data);
        u32BitErr=(u32BitErr<<8)|u8Data;
        u16Address=0x1c3B;
        status&=MSB1245_ReadReg(pMSB1245, u16Address, &u8Data);
        u32BitErr=(u32BitErr<<8)|u8Data;
        u16Address=0x1c3A;
        status&=MSB1245_ReadReg(pMSB1245, u16Address, &u8Data);
        u32BitErr=(u32BitErr<<8)|u8Data;
        u16Address=0x1c32;
        status&=MSB1245_ReadReg(pMSB1245, u16Address, &u8Data);
        u8Data&=~(0x80);
        status&=MSB1245_WriteReg(pMSB1245, u16Address, u8Data);
        if (u16BitErrPeriod == 0)
            u16BitErrPeriod = 1;
        if (u32BitErr <= 0)
            fber = 0.5 / ((float)u16BitErrPeriod *128 * 188 * 8);
        else
            fber = (float)u32BitErr / ((float)u16BitErrPeriod *128 * 188 * 8);

        *p_postBer = fber;
    }
    else    //S2
    {
        u16Address=0x1d04;
        status&=MSB1245_ReadReg(pMSB1245, u16Address, &u8Data);
        u8Data|=0x01;
        status&=MSB1245_WriteReg(pMSB1245, u16Address, u8Data);
        u16Address=0x1d25;
        status&=MSB1245_ReadReg(pMSB1245, u16Address, &u8Data);
        u16BitErrPeriod=u8Data;
        u16Address=0x1d24;
        status&=MSB1245_ReadReg(pMSB1245, u16Address, &u8Data);
        u16BitErrPeriod=(u16BitErrPeriod<<8)|u8Data;

        u16Address=0x1d5b;
        status&=MSB1245_ReadReg(pMSB1245, u16Address, &u8Data);
        u32BitErr=u8Data;
        u16Address=0x1d5a;
        status&=MSB1245_ReadReg(pMSB1245, u16Address, &u8Data);
        u32BitErr=(u32BitErr<<8)|u8Data;
        u16Address=0x1d59;
        status&=MSB1245_ReadReg(pMSB1245, u16Address, &u8Data);
        u32BitErr=(u32BitErr<<8)|u8Data;
        u16Address=0x1d58;
        status&=MSB1245_ReadReg(pMSB1245, u16Address, &u8Data);
        u32BitErr=(u32BitErr<<8)|u8Data;

        u16Address=0x1d04;
        status&=MSB1245_ReadReg(pMSB1245, u16Address, &u8Data);
        u8Data&=~(0x01);
        status&=MSB1245_WriteReg(pMSB1245, u16Address, u8Data);
        if (u16BitErrPeriod == 0)
        {
            u16BitErrPeriod = 1;
        }
        fber = (float)u32BitErr/(u16BitErrPeriod*64800);
        *p_postBer = fber;
    }


    return TRUE;
}


MS_U16 MSB1245_DTV_GetSignalQuality(MS_U8 u8DemodIndex)
{
    MS_U8     status = TRUE;
    float       fber = (float)0.0;
    float       SQI = (float)0.0;
    float log_ber = 0.0;


    MDvr_CofdmDmd_CONFIG *pMSB1245 = (pstMSB1245 + u8DemodIndex);

    if (!pMSB1245)
    {
        DMD_ERR(("pMSB1245 error !"));
        return FALSE;
    }
    if ((!pMSB1245->bInited) || (!pMSB1245->bOpen))
    {
        DMD_ERR(("pMSB1245 have not inited !!!"));
        return FALSE;
    }

    status &= MSB1245_DTV_GetPostBER(u8DemodIndex, &fber);
    if(status)
    {
        log_ber = ( - 1) *log10(1 / fber);
        if (pMSB1245->bIsDVBS2==FALSE)//S
        {
            if (log_ber <= ( - 7.0))
            {
                return 100;
            }
            else if (log_ber < ( - 3.7))
            {
                SQI = (60+((( - 3.7) - log_ber) / (( - 3.7) - ( - 7.0))*(100-60)));
            }
            else if (log_ber < ( - 2.7))
            {
                SQI = (10+((( - 2.7) - log_ber) / (( - 2.7) - ( - 3.7))*(60-10)));
            }
            else
            {
                SQI = 10;
            }
        }
        else //S2
        {
            if (log_ber <= ( - 7.0))
            {
                SQI = 100;
            }
            else if (log_ber < ( - 3.7))
            {
                SQI = (80+((( - 3.7) - log_ber) / (( - 3.7) - ( - 7.0))*(100-80)));
            }
            else if (log_ber < ( - 1.7))
            {
                SQI = (40+((( - 1.7) - log_ber) / (( - 1.7) - ( - 3.7))*(80-40)));
            }
            else if (log_ber < ( - 0.7))
            {
                SQI = (10+((( - 0.7) - log_ber) / (( - 0.7) - ( - 1.7))*(40-10)));
            }
            else
            {
                SQI = 5;
            }
        }
        return SQI;
    }
    else
    {
        return 0;
    }

    return 0;
}

MS_U16  MSB1245_DTV_GetSignalStrength(MS_U8 u8DemodIndex)
{
    MS_BOOL bRet=TRUE;
    MS_U16 u16RetData = 0, u16AGCOut, u16AGCMean;
    MS_U8  u8Index, u8Index_1st = 0;
    MS_U16 u16SignalLevel[AGC_OUT_ROW_MAX][2];
    MS_U16 u16AGCErrorMeanNeg[AGC_MEAN_ROW_MAX][2];
    MS_U16 u16AGCErrorMeanPos[AGC_MEAN_ROW_MAX][2];
    MDvr_CofdmDmd_CONFIG *pMSB1245 = (pstMSB1245 + u8DemodIndex);
    unsigned char RfGainflag = 0;
#if IF_THIS_TUNER_INUSE(TUNER_RT710)
    int RfLevelDbm = 0.0;
#endif

    memset(u16SignalLevel, 0, sizeof(u16SignalLevel));
    memset(u16AGCErrorMeanNeg, 0, sizeof(u16AGCErrorMeanNeg));
    memset(u16AGCErrorMeanPos, 0, sizeof(u16AGCErrorMeanPos));
    #if IF_THIS_TUNER_INUSE(TUNER_AV2012) || IF_THIS_TUNER_INUSE(TUNER_AV2011)
    if(TUNER_AV2012 == pMSB1245->MSB1245_InitParam.pstTunertab->data)
    {
         memcpy(u16SignalLevel, _u16SignalLevel_AV2012, sizeof(u16SignalLevel));
    }
    #endif

    #if IF_THIS_TUNER_INUSE(TUNER_AV2018)
    if(TUNER_AV2018 == pMSB1245->MSB1245_InitParam.pstTunertab->data)
    {
         memcpy(u16SignalLevel, _u16SignalLevel_AV2018, sizeof(u16SignalLevel));
    }
    #endif

    #if IF_THIS_TUNER_INUSE(TUNER_RDA5815M)
    if(TUNER_RDA5815M == pMSB1245->MSB1245_InitParam.pstTunertab->data)
    {
         memcpy(u16SignalLevel, _u16SignalLevel_RDA5815M, sizeof(u16SignalLevel));
    }
    #endif

    #if IF_THIS_TUNER_INUSE(TUNER_RT710)
    if(TUNER_RT710 == pMSB1245->MSB1245_InitParam.pstTunertab->data)
    {
         memcpy(u16SignalLevel, _u16SignalLevel_RT710, sizeof(u16SignalLevel));
         memcpy(u16AGCErrorMeanNeg, _u16AGCErrorMeanNegative_RT710, sizeof(u16AGCErrorMeanNeg));
         memcpy(u16AGCErrorMeanPos, _u16AGCErrorMeanPositive_RT710, sizeof(u16AGCErrorMeanPos));

          MSB1245_IIC_Bypass_Mode(u8DemodIndex, TRUE);
         if (TRUE != pMSB1245->MSB1245_InitParam.pstTunertab->Extension_Function(u8DemodIndex,TUNER_EXT_FUNC_GET_RFAGC_STATUS, &RfGainflag))
         {
            DMD_ERR(("RT710 ReqTunerRxPower failed !!!\n"));
            MSB1245_IIC_Bypass_Mode(u8DemodIndex, FALSE);
            return u16RetData;
         }

         if(RfGainflag == 2) // tuner RF AGC is active gain, get RF strength form tuner directly
         {
             if (TRUE != pMSB1245->MSB1245_InitParam.pstTunertab->Extension_Function(u8DemodIndex,TUNER_EXT_FUNC_GET_POWER_LEVEL, &RfLevelDbm))
             {
                 DMD_ERR(("RT710 ReqTunerRxPower failed !!!\n"));
                 MSB1245_IIC_Bypass_Mode(u8DemodIndex, FALSE);
                 return u16RetData;
             }
             else
             {
                DMD_DBG(("GOT Signal Strength from RT710, %d!!!\n", RfLevelDbm));
                RfLevelDbm = (-1)*(RfLevelDbm*10);
                u16RetData = (MS_U16)RfLevelDbm;
                MSB1245_IIC_Bypass_Mode(u8DemodIndex, FALSE);
                return u16RetData;
             }
         }
    }
    #endif
    bRet &= msb1245_AGC_Info(u8DemodIndex, DBG_AGC_MEAN, &u16AGCMean);
    bRet &= msb1245_AGC_Info(u8DemodIndex, DBG_AGC_OUT, &u16AGCOut);

    DMD_DBG(("[MSB1245] AGC_OUT = %d, AGC_ERROR_MEAN = %d\n",(int)u16AGCOut, (int)u16AGCMean));
    if(bRet)
    {
        for (u8Index=0; u8Index < AGC_OUT_ROW_MAX; u8Index++)
        {
           if(u16SignalLevel[u8Index][0] != NULL_DATA)
           {
              u8Index_1st = u8Index;
              break;
           }
        }

        for (u8Index=u8Index_1st; u8Index < AGC_OUT_ROW_MAX; u8Index++)
        {
            if ((65535 - u16AGCOut) <= u16SignalLevel[u8Index][0])
            {
                // to avoid search error when signal level is close to bounary of tuner AGC active area,
                // if power level has too large variance, return previous data
                if((u8Index != u8Index_1st) && ((u16RetData - u16SignalLevel[u8Index][1]) < SIGNAL_LEVEL_VAR_MAX))
                {
                    u16RetData = u16SignalLevel[u8Index][1];
                }
                break;
            }
            u16RetData = u16SignalLevel[u8Index][1];
        }

        // RT710 would do more estimation by AGC error mean
        if(TUNER_RT710 == pMSB1245->MSB1245_InitParam.pstTunertab->data)
        {
           if((u16AGCMean <= 0x1F4) || (u16AGCMean >= 0xFE00))
           {
              DMD_DBG(("[MSB1245] AGC error mean is close to Zero, estimated by IF AGC only\n"));
           }
           else if(RfGainflag == 1) //Tuner RFAGC max
           {
              if((65535 - u16AGCOut) < u16SignalLevel[u8Index_1st][0])
              {
                  //AGC_error_mean->Power_level;
                  for (u8Index= 0; u8Index < AGC_MEAN_ROW_MAX; u8Index++)
                  {
                      if (u16AGCMean <= u16AGCErrorMeanPos[u8Index][0])
                      {
                          u16RetData = u16AGCErrorMeanPos[u8Index][1];
                          break;
                      }
                   }

                  if(u16AGCMean > u16AGCErrorMeanPos[AGC_MEAN_ROW_MAX-1][0])
                      u16RetData = u16AGCErrorMeanPos[AGC_MEAN_ROW_MAX-1][1]+10;
              }
           }
           else if(RfGainflag == 0) //Tuner RFAGC min
           {
               if((65535 - u16AGCOut) > (u16SignalLevel[AGC_OUT_ROW_MAX-1][0]))
               {
                   //AGC_error_mean->Power_level;
                   for (u8Index=0; u8Index < AGC_MEAN_ROW_MAX; u8Index++)
                   {
                       if (u16AGCMean <= u16AGCErrorMeanNeg[u8Index][0])
                       {
                          u16RetData = u16AGCErrorMeanNeg[u8Index][1];
                          break;
                       }
                   }
               }
           }
        }
    }

    return u16RetData;

}

float  MSB1245_Demod_CarrierFrequencyOffset(MS_U8 u8DemodIndex)
{
    float sr, pr_cfo, fine_cfo, cfo, coarse_cfo;
    MS_BOOL bRet=TRUE;
    MS_U16 u16Address;
    MS_U16 u16Data;
    MS_U32 u32Data;
    MS_U8  u8Data;
    MDvr_CofdmDmd_CONFIG *pMSB1245 = (pstMSB1245 + u8DemodIndex);

    if (MSB1245_DTV_GetLockStatus(u8DemodIndex)==E_DEMOD_UNLOCK)
        return 0;
    u16Address=0x0B53;
    bRet&=MSB1245_ReadReg(pMSB1245, u16Address, &u8Data);
    u16Data=u8Data;
    u16Address=0x0B52;
    bRet&=MSB1245_ReadReg(pMSB1245, u16Address, &u8Data);
    u16Data=(u16Data<<8)|u8Data;
    sr=u16Data;
    //read PR cfo
    //latch
    u16Address=0x1b08;
    bRet&=MSB1245_ReadReg(pMSB1245, u16Address, &u8Data);
    u8Data=u8Data|0x10;
    bRet&=MSB1245_WriteReg(pMSB1245, u16Address, u8Data);
    //cfo (41,43)
    u16Address=0x1bf3;
    bRet&=MSB1245_ReadReg(pMSB1245, u16Address, &u8Data);
    u32Data=u8Data&0x01;
    u16Address=0x1bf2;
    bRet&=MSB1245_ReadReg(pMSB1245, u16Address, &u8Data);
    u32Data=(u32Data<<8)|u8Data;
    u16Address=0x1bf1;
    bRet&=MSB1245_ReadReg(pMSB1245, u16Address, &u8Data);
    u32Data=(u32Data<<8)|u8Data;
    u16Address=0x1bf0;
    bRet&=MSB1245_ReadReg(pMSB1245, u16Address, &u8Data);
    u32Data=(u32Data<<8)|u8Data;
    //release latch
    u16Address=0x1b08;
    bRet&=MSB1245_ReadReg(pMSB1245, u16Address, &u8Data);
    u8Data=u8Data&0xef;
    bRet&=MSB1245_WriteReg(pMSB1245, u16Address, u8Data);
    if(u32Data>=0x1000000)
    {
        u32Data=0x2000000-u32Data;
        pr_cfo=-1*(float)(u32Data);
    }
    else
    {
        pr_cfo=u32Data;
    }
    pr_cfo=pr_cfo/134217728.0*sr;


    //coarse cfo
    bRet&=msb1245_ReadMailbox(u8DemodIndex, E_DMD_S2_MB_SWUSE1AH,&u8Data);
    u32Data=u8Data;
    bRet&=msb1245_ReadMailbox(u8DemodIndex, E_DMD_S2_MB_SWUSE1AL,&u8Data);
    u32Data = (u32Data<<8)|u8Data;
    if(u32Data>=0x8000)
    {
        u32Data=0x10000-u32Data;
        coarse_cfo=-1*(float)u32Data;
    }
    else
    {
        coarse_cfo=u32Data;
    }

    if (pMSB1245->bIsDVBS2==FALSE)//S
    {
        cfo=coarse_cfo+pr_cfo;
    }
    else//S2
    {
        //fine cfo
        //latch
        u16Address=0x1b08;
        bRet&=MSB1245_ReadReg(pMSB1245, u16Address, &u8Data);
        u8Data=u8Data|0x10;
        bRet&=MSB1245_WriteReg(pMSB1245, u16Address, u8Data);
        //cfo (40,36)
        u16Address=0x215a;
        bRet&=MSB1245_ReadReg(pMSB1245, u16Address, &u8Data);
        u32Data=u8Data;
        u16Address=0x2159;
        bRet&=MSB1245_ReadReg(pMSB1245, u16Address, &u8Data);
        u32Data=(u32Data<<8)|u8Data;
        u16Address=0x2158;
        bRet&=MSB1245_ReadReg(pMSB1245, u16Address, &u8Data);
        u32Data=(u32Data<<8)|u8Data;
        //release latch
        u16Address=0x1b08;
        bRet&=MSB1245_ReadReg(pMSB1245, u16Address, &u8Data);
        u8Data=u8Data&0xef;
        bRet&=MSB1245_WriteReg(pMSB1245, u16Address, u8Data);
        if(u32Data>=0x800000)
        {
            u32Data=0x1000000-u32Data;
            fine_cfo=-1*(float)u32Data;
        }
        else
        {
            fine_cfo=u32Data;
        }
        fine_cfo=fine_cfo/1048576.0*sr;

        cfo=coarse_cfo+pr_cfo+fine_cfo;
    }
    if (bRet==FALSE)
    {
        DMD_ERR(("MSB1245_DTV_GetCFO fail!!! \n"));
        return 0;
    }
    return cfo;
}

MS_BOOL  MSB1245_DVBS_SetFrequency(MS_U8 u8DemodIndex, MS_U16 u16CenterFreq, MS_U32 u32SymbolRate_Hz)
{
    MS_BOOL bRet=TRUE;
    MS_U16 u16LockCount;
    MDvr_CofdmDmd_CONFIG *pMSB1245 = (pstMSB1245 + u8DemodIndex);

    bRet &= pMSB1245->MSB1245_InitParam.pstTunertab->SetFreqS2(u8DemodIndex,u16CenterFreq, u32SymbolRate_Hz);

    if (bRet==FALSE)
        return bRet;

    u16LockCount=0;
    do
    {
        bRet&= pMSB1245->MSB1245_InitParam.pstTunertab->GetLock(u8DemodIndex);
        MsOS_DelayTask(1);
        u16LockCount++;
    }
    while((bRet==FALSE) && (u16LockCount<MSB1245_TUNER_WAIT_TIMEOUT)) ;

    if (bRet==TRUE)
    {
        DMD_DBG(("Tuner PLL Lock\n"));
    }
    else
    {
        DMD_DBG(("Tuner PLL Unlock\n"));
    }
    return bRet;
}


MS_BOOL MSB1245_DTV_GetSNR_F(MS_U8 u8DemodIndex, float *fSNR)
{
    MSB1245_DTV_GetSNR(u8DemodIndex);
    *fSNR = g_MSB1245_fSNR;
    return TRUE;
}


void MSB1245_DTV_ControlTsOutput(MS_U8 u8DemodIndex, MS_BOOL bEnable)
{
    MDvr_CofdmDmd_CONFIG *pMSB1245 = (pstMSB1245 + u8DemodIndex);

    if (bEnable)
    {
        MSB1245_WriteReg(pMSB1245, 0x0900 + (0x2D * 2), 0xFF);
    }
    else
    {
        MSB1245_WriteReg(pMSB1245, 0x0900 + (0x2D * 2), 0x00);
    }
}

/****************************************************************************
**function:
**
**
****************************************************************************/
MS_U8 MSB1245_Get_Packet_Error(MS_U8 u8DemodIndex, MS_U16 *u16PktErr)
{
    MS_BOOL    bRet = TRUE;
    MS_U8     u8Data = 0;
    MS_U16    u16Data=0,u16Address = 0;
    //float      Packet_Err=0;
    MS_U16     PRD;
    MS_U16     PER=0;
    MDvr_CofdmDmd_CONFIG *pMSB1245 = (pstMSB1245 + u8DemodIndex);

    if (pMSB1245->bIsDVBS2==FALSE)//S
    {
        u16Address=0x1c32;
        bRet&=MSB1245_ReadReg(pMSB1245, u16Address, &u8Data);
        u8Data|=0x80;
        bRet&=MSB1245_WriteReg(pMSB1245, u16Address, u8Data); //freeze

        // bank 17 0x18 [7:0] reg_bit_err_sblprd_7_0  [15:8] reg_bit_err_sblprd_15_8
        u16Address=0x1c30;
        bRet&=MSB1245_ReadReg(pMSB1245, u16Address, &u8Data);
        u16Data= u8Data;
        u16Address=0x1c31;
        bRet&=MSB1245_ReadReg(pMSB1245, u16Address, &u8Data);
        u16Data= (u8Data<<8)| u16Data;

        PRD=u16Data;
        if (PRD == 0)    // avoide division by 0
            PRD++;

        // bank 17 0x1F [7:0] reg_uncrt_pkt_num_7_0 [15:8] reg_uncrt_pkt_num_15_8
        u16Address=0x1c3E;
        bRet&=MSB1245_ReadReg(pMSB1245, u16Address, &u8Data);
        u16Data= u8Data;
        u16Address=0x1c3F;
        bRet&=MSB1245_ReadReg(pMSB1245, u16Address, &u8Data);
        u16Data= (u8Data<<8)| u16Data; //Packet error
        PER=u16Data;


        u16Address=0x1c32;
        bRet&=MSB1245_ReadReg(pMSB1245, u16Address, &u8Data);
        u8Data&=(~0x80);
        bRet&=MSB1245_WriteReg(pMSB1245, u16Address, u8Data); //unfreeze
        //Packet_Err = (float)PER/(128*PRD);
    }
    else  //s2
    {

        u16Address=0x1d04;
        bRet&=MSB1245_ReadReg(pMSB1245, u16Address, &u8Data);
        u8Data|=0x01;
        bRet&=MSB1245_WriteReg(pMSB1245, u16Address, u8Data); //freeze


        u16Address=0x1d56;
        bRet&=MSB1245_ReadReg(pMSB1245, u16Address, &u8Data);
        u16Data= u8Data;
        u16Address=0x1d57;
        bRet&=MSB1245_ReadReg(pMSB1245, u16Address, &u8Data);
        u16Data= (u8Data<<8)| u16Data;    //E-flag, error
        PER=u16Data;

        u16Address=0x1d04;
        bRet&=MSB1245_ReadReg(pMSB1245, u16Address, &u8Data);
        u8Data&=(~0x01);
        bRet&=MSB1245_WriteReg(pMSB1245, u16Address, u8Data); //unfreeze
    }

    *u16PktErr = PER;

    return bRet;
}


/********************************************
Function: To get chip version
Parameter: pu8Chip   0x01->MSB1230, 0x02->MSB124X,
                    0xff ->unknown
*********************************************/
//MS_BOOL MSB124X_Demod_GetChip(MS_U8 *pu8Chip)
//{
//    MSB124X_I2C_CH_Reset(3);
//    *pu8Chip = MSB124X_DTV_GetHWVer();
//    return TRUE;
//}
MS_BOOL MSB1245_Demod_I2C_ByPass(MS_U8 u8DemodIndex,MS_BOOL bOn)
{
    MDvr_CofdmDmd_CONFIG *pMSB1245 = (pstMSB1245 + u8DemodIndex);
    MS_BOOL bret = TRUE;

    if (!pMSB1245)
    {
        DMD_ERR(("pMSB1245 error !"));
        return FALSE;
    }
    if ((!pMSB1245->bInited) || (!pMSB1245->bOpen))
    {

        bret &= MSB1245_I2C_CH_Reset(u8DemodIndex,3);
        bret &= msb1245_SetReg(u8DemodIndex,0x0951, 0x00);
        bret &= msb1245_SetReg(u8DemodIndex,0x090C, 0x10);
        bret &= msb1245_SetReg(u8DemodIndex,0x090E, 0x10);
        if(bOn)
        {
            DMD_DBG(("set MSB1245 I2C bypass ON\n"));
            bret &= msb1245_SetReg(u8DemodIndex,0x0910, 0x10);
        }
        else
        {
            DMD_DBG(("set MSB1245 I2C bypass OFF\n"));
            bret &= msb1245_SetReg(u8DemodIndex,0x0910, 0x00);
        }
        return bret;
    }
    if (MsOS_ObtainMutex(pMSB1245->s32_MSB1245_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("MDrv_Demod_SetScanTypeStatus:Obtain mutex failed !!!\n"));
        return FALSE;
    }

    MSB1245_IIC_Bypass_Mode(u8DemodIndex, bOn);

    MsOS_ReleaseMutex(pMSB1245->s32_MSB1245_Mutex);
    return TRUE;
}

MS_BOOL MSB1245_Demod_Open(MS_U8 u8DemodIndex)
{
    MDvr_CofdmDmd_CONFIG *pMSB1245 = (pstMSB1245 + u8DemodIndex);
    if (!pMSB1245)
    {
        DMD_ERR(("pMSB1245 error !\n"));
        return FALSE;
    }
    if ((!pMSB1245->bInited) || (!pMSB1245->bOpen))
    {
        DMD_ERR(("[%s]pMSB124X have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }
    if (MsOS_ObtainMutex(pMSB1245->s32_MSB1245_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("MDrv_Demod_Open:Obtain mutex failed !!!\n"));
        return FALSE;
    }

    pMSB1245->bOpen = TRUE;
    MsOS_ReleaseMutex(pMSB1245->s32_MSB1245_Mutex);

    return TRUE;
}

MS_BOOL MSB1245_Demod_PowerOnOff(MS_BOOL bPowerOn)
{
    return TRUE;
}

MS_BOOL MSB1245_Demod_Close(MS_U8 u8DemodIndex)
{
    MDvr_CofdmDmd_CONFIG *pMSB1245 = (pstMSB1245 + u8DemodIndex);

    if (!pMSB1245)
    {
        DMD_ERR(("pMSB1245 error !\n"));
        return FALSE;
    }
    if ((!pMSB1245->bInited) || (!pMSB1245->bOpen))
    {
        DMD_ERR(("[%s]pMSB1245 have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }

    if (MsOS_ObtainMutex(pMSB1245->s32_MSB1245_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("Obtain mutex failed !!!\n"));
        return FALSE;
    }
    pMSB1245->bOpen = FALSE;
    MsOS_ReleaseMutex(pMSB1245->s32_MSB1245_Mutex);

    return TRUE;
}


/****************************************************************************
**function:
**
**
****************************************************************************/
MS_BOOL MSB1245_Demod_GetSNR(MS_U8 u8DemodIndex, float *pfSNR)
{

    MDvr_CofdmDmd_CONFIG *pMSB1245 = (pstMSB1245 + u8DemodIndex);

    if (!pMSB1245)
    {
        DMD_ERR(("pMSB1245 error !\n"));
        return FALSE;
    }
    if ((!pMSB1245->bInited) || (!pMSB1245->bOpen))
    {
        DMD_ERR(("[%s]pMSB1245 have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }

    if (MsOS_ObtainMutex(pMSB1245->s32_MSB1245_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("MDrv_Demod_GetSNR:Obtain mutex failed !!!\n"));
        return FALSE;
    }
    else
    {
        *pfSNR = 0;
        if (E_DEMOD_LOCK == MSB1245_DTV_GetLockStatus(u8DemodIndex))
        {
            MSB1245_DTV_GetSNR_F(u8DemodIndex,pfSNR);
        }
    }
    MsOS_ReleaseMutex(pMSB1245->s32_MSB1245_Mutex);

    return TRUE;
}

/****************************************************************************
**function:
**
**
****************************************************************************/
MS_BOOL MSB1245_Demod_GetBER(MS_U8 u8DemodIndex,float *pfBER)
{
    MS_BOOL status = TRUE;
    MDvr_CofdmDmd_CONFIG *pMSB1245 = (pstMSB1245 + u8DemodIndex);


    if (!pMSB1245)
    {
        DMD_ERR(("pMSB1245error !\n"));
        return FALSE;
    }
    if ((!pMSB1245->bInited) || (!pMSB1245->bOpen))
    {
        DMD_ERR(("[%s]pMSB1245 have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }

    if (MsOS_ObtainMutex(pMSB1245->s32_MSB1245_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("MDrv_Demod_GetBER:Obtain mutex failed !!!\n"));
        return FALSE;
    }

    //*pfBER = (float)MSB124X_DTV_GetSignalQuality();
    status = MSB1245_DTV_GetPostBER(u8DemodIndex, pfBER);

    MsOS_ReleaseMutex(pMSB1245->s32_MSB1245_Mutex);

    return status;
}

MS_BOOL MSB1245_Demod_GetPWR(MS_U8 u8DemodIndex, MS_S32 *ps32Signal)
{
    MDvr_CofdmDmd_CONFIG *pMSB1245 = (pstMSB1245 + u8DemodIndex);
    MS_BOOL bret = TRUE;


    if (!pMSB1245)
    {
        DMD_ERR(("pMSB1245 error !\n"));
        return FALSE;
    }
    if ((!pMSB1245->bInited) || (!pMSB1245->bOpen))
    {
        DMD_ERR(("[%s]pMSB1245 have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }
    if (MsOS_ObtainMutex(pMSB1245->s32_MSB1245_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("MDrv_Demod_GetPWR:Obtain mutex failed !!!\n"));
        return FALSE;
    }
    else
    {
        *ps32Signal = 0;
        if (E_DEMOD_LOCK == MSB1245_DTV_GetLockStatus(u8DemodIndex))
        {
            *ps32Signal=(MS_S32)MSB1245_DTV_GetSignalStrength(u8DemodIndex);
            *ps32Signal=(-1)*(*ps32Signal)/10;
        }

        if(*ps32Signal == 0)
        {
           DMD_DBG(("Get Signal Level ERROR, DMD unlock or Tuner Power Table Not Ready !!\n"));
           bret = FALSE;
        }
    }

    MsOS_ReleaseMutex(pMSB1245->s32_MSB1245_Mutex);
    return bret;
}

MS_BOOL MSB1245_Demod_GetSSI(MS_U8 u8DemodIndex, MS_U16 *pu16SSI)
{
    MDvr_CofdmDmd_CONFIG *pMSB1245 = (pstMSB1245 + u8DemodIndex);
    MS_BOOL bret = TRUE;
    MS_S32 RfLevelDbm = 0;


    if (!pMSB1245)
    {
        DMD_ERR(("pMSB1245 error !\n"));
        return FALSE;
    }
    if ((!pMSB1245->bInited) || (!pMSB1245->bOpen))
    {
        DMD_ERR(("[%s]pMSB1245 have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }
    if (MsOS_ObtainMutex(pMSB1245->s32_MSB1245_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("MDrv_Demod_GetPWR:Obtain mutex failed !!!\n"));
        return FALSE;
    }
    else
    {
        *pu16SSI = 0;
        if (E_DEMOD_LOCK == MSB1245_DTV_GetLockStatus(u8DemodIndex))
        {
            RfLevelDbm=(MS_S32)MSB1245_DTV_GetSignalStrength(u8DemodIndex);
            RfLevelDbm=(-1)*(RfLevelDbm)/10;
        }

        if(RfLevelDbm == 0)
        {
           DMD_DBG(("Get Signal Level ERROR, DMD unlock or Tuner Power Table Not Ready !!\n"));
           bret = FALSE;
        }

        if (RfLevelDbm >= -20)
        {
           *pu16SSI=100;
        }
        else if(RfLevelDbm < -86)
        {
           *pu16SSI= 0 ;
        }
        else
        {
            *pu16SSI=(MS_U16)((RfLevelDbm*1000+80000)*90/60000+10);
        }

    }

    MsOS_ReleaseMutex(pMSB1245->s32_MSB1245_Mutex);
    return bret;
}


MS_BOOL MSB1245_Demod_GetSignalQuality(MS_U8 u8DemodIndex, MS_U16 *pu16quality)
{

    MDvr_CofdmDmd_CONFIG *pMSB1245 = (pstMSB1245 + u8DemodIndex);

    if (!pMSB1245)
    {
        DMD_ERR(("pMSB1245 error !\n"));
        return FALSE;
    }
    if ((!pMSB1245->bInited) || (!pMSB1245->bOpen))
    {
        DMD_ERR(("[%s]pMSB1245 have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }

    if (MsOS_ObtainMutex(pMSB1245->s32_MSB1245_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("MDrv_Demod_GetSNR:Obtain mutex failed !!!\n"));
        return FALSE;
    }
    else
    {
        *pu16quality = 0;
        if (E_DEMOD_LOCK == MSB1245_DTV_GetLockStatus(u8DemodIndex))
        {
            *pu16quality  = MSB1245_DTV_GetSignalQuality(u8DemodIndex);
        }

    }
    MsOS_ReleaseMutex(pMSB1245->s32_MSB1245_Mutex);

    return TRUE;
}

MS_BOOL MSB1245_Demod_GetLock(MS_U8 u8DemodIndex, EN_LOCK_STATUS *peLockStatus)
{
    MDvr_CofdmDmd_CONFIG *pMSB1245 = (pstMSB1245 + u8DemodIndex);

    *peLockStatus = E_DEMOD_CHECKING;

    if (!pMSB1245)
    {
        DMD_ERR(("pMSB1245 error !\n"));
        return FALSE;
    }
    if ((!pMSB1245->bInited) || (!pMSB1245->bOpen))
    {
        DMD_ERR(("[%s]pMSB1245 have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }

    if (MsOS_ObtainMutex(pMSB1245->s32_MSB1245_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("MDrv_Demod_GetLock:Obtain mutex failed !!!\n"));
        return FALSE;
    }
    else
    {
        *peLockStatus = MSB1245_DTV_GetLockStatus(u8DemodIndex);
    }

    MsOS_ReleaseMutex(pMSB1245->s32_MSB1245_Mutex);
    return TRUE;
}




MS_BOOL MSB1245_DVBS_S2_Demod_Restart(MS_U8 u8DemodIndex, DEMOD_MS_FE_CARRIER_PARAM *pParam)
{
    MS_BOOL bRet=TRUE;
    MS_U16 u16Address;
    MS_U8   u8Data;
    MS_U16 u16SymbolRate;
    #if (DMD_DEBUG_OPTIONS & DMD_EN_DBG)
    MS_U16 u16CenterFreq;
    #endif
    MDvr_CofdmDmd_CONFIG *pMSB1245 = (pstMSB1245 + u8DemodIndex);
    InterruptNum IntNum;


    if (!pMSB1245)
    {
        DMD_ERR(("pMSB1245 error !\n"));
        return FALSE;
    }
    if ((!pMSB1245->bInited) || (!pMSB1245->bOpen))
    {
        DMD_ERR(("[%s]pMSB1245 have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }
    if (MsOS_ObtainMutex(pMSB1245->s32_MSB1245_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("MDrv_Demod_Restart:Obtain mutex failed !!!\n"));
        return FALSE;
    }


    if(_mdrv_dmd_msb1245_get_IntNum(u8DemodIndex, &IntNum))
    {
        MsOS_DisableInterrupt(IntNum);
    }

    u16SymbolRate=(pParam->SatParam.u32SymbolRate/1000);
#if (DMD_DEBUG_OPTIONS & DMD_EN_DBG)
    u16CenterFreq=pParam->u32Frequency;
#endif
    DMD_DBG(("MSB1245 MDrv_CofdmDmd_Restart+ Fc:%d SymbolRate %d\n", u16CenterFreq, u16SymbolRate));
    MSB1245_IIC_Bypass_Mode(u8DemodIndex, TRUE);
    bRet&= MSB1245_DVBS_SetFrequency(u8DemodIndex, pParam->u32Frequency,(pParam->SatParam.u32SymbolRate/1000));
    MSB1245_IIC_Bypass_Mode(u8DemodIndex, FALSE);

    u16Address=0x0990;
    bRet&=MSB1245_ReadReg(pMSB1245, u16Address, &u8Data);
    u8Data&=0xF0;
    bRet&=MSB1245_WriteReg(pMSB1245, u16Address,u8Data);

    u16Address=0x0B32;//MCU reset
    u8Data = 0x03;
    bRet&=MSB1245_WriteReg(pMSB1245, u16Address,u8Data);
    MsOS_DelayTask(1);
    u8Data = 0x00;
    bRet&=MSB1245_WriteReg(pMSB1245, u16Address,u8Data);
    MsOS_DelayTask(10);

    u16Address=0x0B52;//DIG_DBG_2
    u8Data=(u16SymbolRate&0xFF);
    bRet&=MSB1245_WriteReg(pMSB1245, u16Address,u8Data);
    u16Address=0x0B53;//DIG_DBG_3
    u8Data=((u16SymbolRate>>8)&0xFF);
    bRet&=MSB1245_WriteReg(pMSB1245, u16Address,u8Data);
    //ADCPLL IQ swap
    bRet&=msb1245_ADCPLL_IQ_SWAP(u8DemodIndex);

    //TS serial mode
    u16Address= 0x2A40;
    bRet&= MSB1245_ReadReg(pMSB1245, u16Address, &u8Data);
    if(!pMSB1245->MSB1245_InitParam.stDMDCon.bTSIsParallel)//MSB1245_TS_SERIAL==1
        u8Data |= (0x01);
    else
        u8Data &= (0xFE);
    bRet&= MSB1245_WriteReg(pMSB1245, u16Address, u8Data);

    u16Address=0x0990;
    bRet&=MSB1245_ReadReg(pMSB1245, u16Address, &u8Data);
    u8Data&=0xF0;
    u8Data|=0x01;
    bRet&=MSB1245_WriteReg(pMSB1245, u16Address,u8Data);
#if MS_DVBS_INUSE
    bRet&= MSB1245_DiSEqC_Init(u8DemodIndex);
#endif
    u8TS_VLD_cnt = 0;
    if(_mdrv_dmd_msb1245_get_IntNum(u8DemodIndex, &IntNum))
    {
       MsOS_EnableInterrupt(IntNum);
    }

    MsOS_ReleaseMutex(pMSB1245->s32_MSB1245_Mutex);

    return bRet;
}


MS_BOOL MSB1245_Demod_ClearStatus(MS_U8 u8DemodIndex)
{
    MS_BOOL bRet=TRUE;
    MS_U16 u16Address;
    MS_U8   u8Data;
    MDvr_CofdmDmd_CONFIG *pMSB1245 = pstMSB1245 + u8DemodIndex;

    if (!pMSB1245)
    {
        DMD_ERR(("pMSB1245 error !\n"));
        return FALSE;
    }
    if ((!pMSB1245->bInited) || (!pMSB1245->bOpen))
    {
        DMD_ERR(("[%s]pMSB1245 have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }
    if (MsOS_ObtainMutex(pMSB1245->s32_MSB1245_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("MDrv_Demod_GetPWR:Obtain mutex failed !!!\n"));
        return FALSE;
    }

    u16Address=0x0990;
    bRet&=MSB1245_ReadReg(pMSB1245, u16Address, &u8Data);
    u8Data&=0xF0;
    bRet&=MSB1245_WriteReg(pMSB1245, u16Address,u8Data);

    MsOS_ReleaseMutex(pMSB1245->s32_MSB1245_Mutex);
    return bRet;
}

MS_BOOL MSB1245_Demod_Restart(MS_U8 u8DemodIndex, DEMOD_MS_FE_CARRIER_PARAM *pParam, MS_U32 u32BroadCastType)
{
    MS_BOOL bRet = TRUE;
    char cTimerName[]="msb1245_timerX";
    MS_S32* ps32TimerID = NULL;

    if((ps32msb1245_TimerID!=NULL) && (pbTimerEn!=NULL))
    {
        ps32TimerID = ps32msb1245_TimerID + u8DemodIndex;
        if(*ps32TimerID > 0)
            MsOS_StopTimer (*ps32TimerID);
    }
    else
    {
        return FALSE;
    }

    bRet &= MSB1245_DVBS_S2_Demod_Restart(u8DemodIndex, pParam);


    if(*ps32TimerID < 0)
    {
        if(*(pbTimerEn+u8DemodIndex))
        {
            cTimerName[13] = u8DemodIndex + '0';
            *ps32TimerID = MsOS_CreateTimer (_mdrv_dmd_msb1245_timer_cb,
                                              MSB1245_LOCK_TIMEOUT,
                                              MSB1245_STATUS_CHK_PERIOD,
                                              TRUE,
                                              cTimerName);
           if (*ps32TimerID > 0)
               printf("[%s][%d] %s create ok\n",__FUNCTION__,__LINE__, cTimerName);
           else
               printf("[%s][%d] create timer failed \n",__FUNCTION__,__LINE__);
        }
    }
    else
    {
        MsOS_StartTimer (*ps32TimerID);
    }

    return bRet;
}


MS_BOOL MSB1245_Demod_Init(MS_U8 u8DemodIndex,DEMOD_MS_INIT_PARAM* pParam)
{
    DMD_DBG(("%s(),%d\n", __FUNCTION__, __LINE__));

    MS_BOOL     status = TRUE;
    sDMD_MSB124X_InitData sMSB1245_InitData;
    MDvr_CofdmDmd_CONFIG *pMSB1245 = (pstMSB1245 + u8DemodIndex);
    InterruptNum IntNum;

    if(NULL == pParam)
        return FALSE;

    if (pMSB1245->bInited)
        return TRUE;

    memcpy(&pMSB1245->MSB1245_InitParam, pParam,sizeof(DEMOD_MS_INIT_PARAM));
   if(pMSB1245->MSB1245_InitParam.stDMDCon.bSupportMSPILoad)
   {
       if(pMSB1245->MSB1245_InitParam.stDMDCon.bEnMSPIPathDet)
       {
           DMD_DBG(("MSPI path is set by detected result\n"));
       }
       else
           pMSB1245->u32DmxInputPath = pParam->u32DmxInputPath;
   }

   if(pMSB1245->MSB1245_InitParam.stDMDCon.u32TSVLDInterrupt != DMD_CON_INFO_NOT_DEFINE)
   {
      *(pbTimerEn+u8DemodIndex) = TRUE;
   }

#if defined(SUPPORT_MULTI_DEMOD) && (SUPPORT_MULTI_DEMOD == 1)
    if (pMSB1245->s32DemodHandle == -1)
    {
        MDrv_MDM_MSB124X_CreateNode(&pMSB1245->s32DemodHandle);
    }

    if (pMSB1245->s32DemodHandle==-1)
    {
        DMD_ERR(("MDrv_MDM_MSB124X_CreateNode failed!!!\n"));
        return FALSE;
    }
#endif

    _msb1245_hw_reset(u8DemodIndex);
    pMSB1245->bInited = TRUE;
    pMSB1245->bOpen = TRUE;
    pMSB1245->u8sramCode = 0;
    pMSB1245->bFECLock = FALSE;
    pMSB1245->u8ScanStatus = 0;
    pMSB1245->u32CurrFreq = 0;

    pMSB1245->s32_MSB1245_Mutex = MsOS_CreateMutex(E_MSOS_FIFO, (char *)"Mutex_MSB1245", MSOS_PROCESS_SHARED);
    if (pMSB1245->s32_MSB1245_Mutex < 0)
    {
        pMSB1245->bInited = FALSE;
        pMSB1245->bOpen = FALSE;
        DMD_ERR(("creat MSB1245 Mutex error!!!\n"));
        return FALSE;
    }
    //void MApi_Demod_HWReset(void);
    //MApi_Demod_HWReset();

    sMSB1245_InitData.fpMSB124X_I2C_Access = msb1245_I2C_Access;
    sMSB1245_InitData.u8WO_SPI_Flash = TRUE;
    sMSB1245_InitData.bPreloadDSPCodeFromMainCHIPI2C = FALSE;
    sMSB1245_InitData.bFlashWPEnable = TRUE;
    sMSB1245_InitData.fpGPIOReset = NULL;
    sMSB1245_InitData.pDVBC_DSP_REG = NULL;
    sMSB1245_InitData.pDVBT_DSP_REG = NULL;
    sMSB1245_InitData.pDVBT2_DSP_REG = NULL;
    sMSB1245_InitData.u8WO_Sdram = TRUE;
    sMSB1245_InitData.bEnableSPILoadCode = FALSE;
    sMSB1245_InitData.fpMSB124x_SPIPAD_En = NULL;

    bTS_SERIAL = !(pMSB1245->MSB1245_InitParam.stDMDCon.bTSIsParallel);

    if(pMSB1245->MSB1245_InitParam.stDMDCon.bSupportMSPILoad)
    {
       if(pMSB1245->MSB1245_InitParam.stDMDCon.bEnMSPIPathDet == TRUE)
       {
          if(EXT_DMD_MSPI_PATH_NONE != pMSB1245->u32DmxInputPath)
          {
              sMSB1245_InitData.bEnableSPILoadCode = TRUE;
              sMSB1245_InitData.fpMSB124x_SPIPAD_En = SPIPAD_EN[pMSB1245->u32DmxInputPath];
          }
       }
       else if(DMX_INPUT_EXT_INPUT0 == pMSB1245->u32DmxInputPath)//manual set
       {
          sMSB1245_InitData.bEnableSPILoadCode = TRUE;
          sMSB1245_InitData.fpMSB124x_SPIPAD_En = SPIPAD_EN[0];
       }
       else if(DMX_INPUT_EXT_INPUT1 == pMSB1245->u32DmxInputPath)
       {
          sMSB1245_InitData.bEnableSPILoadCode = TRUE;
          sMSB1245_InitData.fpMSB124x_SPIPAD_En = SPIPAD_EN[1];
       }
    }

    status &= MDrv_DMD_MSB124X_Init_EX(pMSB1245->s32DemodHandle, &sMSB1245_InitData, sizeof(sDMD_MSB124X_InitData));
    if (status)
    {
        MDrv_DMD_MSB124X_SetCurrentDemodulatorType_EX(pMSB1245->s32DemodHandle, E_DMD_MSB124X_DEMOD_DVBS2);
        status &= MDrv_DMD_MSB1245_LoadDSPCodeToSram_EX(pMSB1245->s32DemodHandle);
#if MSB1245_SELECT_AGC_PAD_T
        status &= MSB1245_WriteReg2bytes(pMSB1245, 0x0A00+(0x18)*2, 0x0001);
#else
        status &= MSB1245_WriteReg2bytes(pMSB1245, 0x0A00+(0x18)*2, 0x0100);
#endif

#if MS_DVBS_INUSE
        MsOS_DelayTask(10);
        status &= MSB1245_DiSEqC_Init(u8DemodIndex);
#endif
    }


    if (!status)
    {
        pMSB1245->bInited = FALSE;
        pMSB1245->bOpen= FALSE;
        MsOS_DeleteMutex(pMSB1245->s32_MSB1245_Mutex);
        return FALSE;
    }

    if(pMSB1245->MSB1245_InitParam.stDMDCon.bTSIsParallel)
        MSB1245_DTV_Serial_Control(u8DemodIndex, FALSE);
    else
        MSB1245_DTV_Serial_Control(u8DemodIndex, TRUE);

    status &= msb1245_ADCPLL_IQ_SWAP(u8DemodIndex);
    if(_mdrv_dmd_msb1245_get_IntNum(u8DemodIndex, &IntNum))
    {
        GET_DEMOD_ENTRY_NODE(DEMOD_MSB1245).SupportINT = TRUE;
        MsOS_AttachInterrupt(IntNum, _mdrv_dmd_msb1245_cb);
        MsOS_DisableInterrupt(IntNum);

        if (_s32DmdEventId < 0)
        {
            _s32DmdEventId = MsOS_CreateEventGroup("MSB1245_Event");
            if (_s32DmdEventId > 0)
            {
                DMD_DBG(("[%s][%d] Event create ok\n",__FUNCTION__,__LINE__));
            }
            else
            {
                DMD_ERR(("[%s][%d] create failed \n",__FUNCTION__,__LINE__));
                pMSB1245->bInited = FALSE;
                pMSB1245->bOpen= FALSE;
                MsOS_DeleteMutex(pMSB1245->s32_MSB1245_Mutex);

                return FALSE;
            }
        }

        if(_s32DmdEventTaskId < 0)
        {
           _s32DmdEventTaskId = MsOS_CreateTask(_mdrv_dmd_msb1245_event_task,
                                                0,
                                                E_TASK_PRI_HIGHEST,
                                                TRUE,
                                                u8StackBuffer,
                                                MSB1245_EVT_TASK_STACK_SIZE,
                                                "MSB1245_EVT_TASK");
            if (_s32DmdEventTaskId > 0)
            {
                DMD_DBG(("[%s][%d] Event task create ok\n",__FUNCTION__,__LINE__));
            }
            else
            {
                DMD_ERR(("[%s][%d] create task failed \n",__FUNCTION__,__LINE__));
                pMSB1245->bInited = FALSE;
                pMSB1245->bOpen= FALSE;
                MsOS_DeleteMutex(pMSB1245->s32_MSB1245_Mutex);
                MsOS_DeleteEventGroup(_s32DmdEventId);

                return FALSE;
            }
        }
    }
    return status;
}

static MS_BOOL msb1245_GetReg(MS_U8 u8DemodIndex, MS_U16 u16Addr, MS_U8 *pu8Data)
{
    MS_BOOL bRet=TRUE;
    MS_U8 u8MsbData[6];
    mapi_i2c *iptr;

    iptr = mapi_i2c_GetI2C_Dev();
    if (iptr == NULL)
    {
        return FALSE;
    }

    u8MsbData[0] = 0x10;
    u8MsbData[1] = 0x00;
    u8MsbData[2] = 0x00;
    u8MsbData[3] = (u16Addr >> 8) &0xff;
    u8MsbData[4] = u16Addr & 0xff;

    u8MsbData[0] = 0x35;
    bRet &=iptr->WriteBytes(u8DemodIndex, 0, NULL, 1, u8MsbData);

    u8MsbData[0] = 0x10;
    bRet &=iptr->WriteBytes(u8DemodIndex, 0, NULL, 5, u8MsbData);
    bRet &=iptr->ReadBytes(u8DemodIndex, 0, NULL, 1, pu8Data);

    u8MsbData[0] = 0x34;
    bRet &=iptr->WriteBytes(u8DemodIndex, 0, NULL, 1, u8MsbData);

    return bRet;
}

static MS_BOOL msb1245_SetReg(MS_U8 u8DemodIndex, MS_U16 u16Addr, MS_U8 u8Data)
{
    MS_BOOL bRet=TRUE;
    MS_U8 u8MsbData[6];
    mapi_i2c *iptr;

    iptr = mapi_i2c_GetI2C_Dev();
    if (iptr == NULL)
    {
        return FALSE;
    }

    if (iptr == NULL)
    {
        return FALSE;
    }

    u8MsbData[0] = 0x10;
    u8MsbData[1] = 0x00;
    u8MsbData[2] = 0x00;
    u8MsbData[3] = (u16Addr >> 8) &0xff;
    u8MsbData[4] = u16Addr &0xff;
    u8MsbData[5] = u8Data;

    u8MsbData[0] = 0x35;
    bRet &=iptr->WriteBytes(u8DemodIndex, 0, NULL, 1, u8MsbData);

    u8MsbData[0] = 0x10;
    bRet &=iptr->WriteBytes(u8DemodIndex, 0, NULL, 6, u8MsbData);

    u8MsbData[0] = 0x34;
    bRet &=iptr->WriteBytes(u8DemodIndex, 0, NULL, 1, u8MsbData);
    return bRet;
}


static MS_BOOL msb1245_SetReg2Bytes(MS_U8 u8DemodIndex,MS_U16 u16Addr, MS_U16 u16Data)
{
    MS_BOOL bRet=TRUE;

    bRet &= msb1245_SetReg(u8DemodIndex, u16Addr, (MS_U8)u16Data&0x00ff);
    bRet &= msb1245_SetReg(u8DemodIndex, u16Addr + 0x0001, (MS_U8)(u16Data>>8)&0x00ff);

    return bRet;
}


MS_U16 MSB1245_Demod_ReadReg(MS_U8 u8DemodIndex, MS_U16 RegAddr)
{
    MDvr_CofdmDmd_CONFIG *pMSB1245 = (pstMSB1245 + u8DemodIndex);
    MS_U8 RegData = 0;

    if (!pMSB1245)
    {
        DMD_ERR(("pMSB124X error !\n"));
        return FALSE;
    }

    MSB1245_ReadReg(pMSB1245, RegAddr, &RegData);

    return (MS_U16) RegData;
}

MS_BOOL MSB1245_Demod_WriteReg(MS_U8 u8DemodIndex,MS_U16 RegAddr, MS_U16 RegData)
{
    MDvr_CofdmDmd_CONFIG *pMSB1245 = (pstMSB1245 + u8DemodIndex);

    if (!pMSB1245)
    {
        DMD_ERR(("pMSB124X error !\n"));
        return FALSE;
    }

    return MSB1245_WriteReg(pMSB1245, RegAddr, RegData);
}

#define MSB1245_CHIP_ID 0x91
static MS_BOOL _msb1245_I2C_CH3_reset(MS_U8 u8DemodIndex)
{
    //Reset twice to check if reset pin is correct
    _msb1245_hw_reset(u8DemodIndex);
    if(!MSB1245_I2C_CH_Reset(u8DemodIndex,3))
    {
        DMD_ERR(("[MSB124X] I2C_CH_Reset fail \n"));
        return FALSE;
    }
    else
    {
        _msb1245_hw_reset(u8DemodIndex);
        if(!MSB1245_I2C_CH_Reset(u8DemodIndex,3))
        {
            DMD_ERR(("[MSB124X] I2C_CH_Reset fail \n"));
            return FALSE;
        }
   }
   return TRUE;
}

MS_BOOL MSB1245_Check_Exist(MS_U8 u8DemodIndex, MS_U8* pu8SlaveID)
{
    MS_U8 u8_tmp = 0;
    MS_U8 i, u8I2C_Port = 0;
    MDvr_CofdmDmd_CONFIG *pMSB1245;
    SLAVE_ID_USAGE *pMSB1245_ID_TBL;
    MS_IIC_PORT ePort = 0;

    if(!MSB1245_Variables_alloc())
    {
        MSB1245_Variables_free();
        return FALSE;
    }
    else
    {
        pMSB1245 = (pstMSB1245 + u8DemodIndex);
    }

    get_i2c_port(u8DemodIndex, &ePort);
    if((int)ePort < (int)E_MS_IIC_SW_PORT_0)
    {
       u8I2C_Port = (MS_U8)ePort/8;
    }
    else if((int)ePort < (int)E_MS_IIC_PORT_NOSUP)//sw i2c
    {
       u8I2C_Port = E_MS_IIC_SW_PORT_0/8 + (ePort - E_MS_IIC_SW_PORT_0);
    }

    i=0;
    do
    {
        pMSB1245_ID_TBL = pstMSB1245_slave_ID_TBL + u8I2C_Port*sizeof(MSB1245_possible_slave_ID)/sizeof(SLAVE_ID_USAGE) + i;
        DMD_DBG(("### %x\n",i));
        if(pMSB1245_ID_TBL->u8SlaveID == 0xFF)
        {
            DMD_DBG(("[MSB1245] All Slave ID have tried but not detect\n"));
            break;
        }

        if(pMSB1245_ID_TBL->bInUse)
        {
            DMD_DBG(("[MSB1245] Slave ID 0x%x have been used\n", pMSB1245_ID_TBL->u8SlaveID));
            i++;
            continue;
        }
        else
        {
            pMSB1245->u8SlaveID = pMSB1245_ID_TBL->u8SlaveID;
            DMD_DBG(("[MSB1245] Try slave ID 0x%x\n",pMSB1245_ID_TBL->u8SlaveID));
        }

        if(_msb1245_I2C_CH3_reset(u8DemodIndex))
        {
            DMD_DBG(("[MSB1245] I2C slave id :%x \n",pMSB1245->u8SlaveID ));
            if(msb1245_GetReg(u8DemodIndex,0x0900,&u8_tmp))
            {
                DMD_DBG(("[MSB1245] read id :%x \n",u8_tmp ));
                if(u8_tmp == MSB1245_CHIP_ID)
                {
                    pMSB1245_ID_TBL->bInUse = TRUE;
                    break;
                }
            }
        }

        i++;
    }while((pMSB1245_ID_TBL->u8SlaveID) != 0xFF);


    if(u8_tmp == MSB1245_CHIP_ID)
    {
        u8MSB1245_DevCnt++;
        *pu8SlaveID = pMSB1245->u8SlaveID;
        msb1245_GetReg(u8DemodIndex,0x0905,&u8_tmp);

        if((u8_tmp>>2) & 0x01)
        {
            DMD_DBG(("[MSB1245] This is External DMD \n"));
            pMSB1245->bIsMCP_DMD = FALSE;
        }
        else
        {
            DMD_DBG(("[MSB1245] This is MCP DMD \n"));
            pMSB1245->bIsMCP_DMD = TRUE;
        }

        if(pMSB1245->MSB1245_InitParam.stDMDCon.bSupportMSPILoad)
        {
            if(pMSB1245->MSB1245_InitParam.stDMDCon.bEnMSPIPathDet)
            {
                    //check TS path
                    MDrv_DMD_SSPI_Init(0);
                    MDrv_MasterSPI_CsPadConfig(0, 0xff);
                    MDrv_MasterSPI_MaxClkConfig(0, 27);

                    msb1245_SetReg2Bytes(u8DemodIndex,0x0900+(0x28)*2, 0x0000);
                    msb1245_SetReg2Bytes(u8DemodIndex,0x0900+(0x2d)*2, 0x00ff);
                    // ------enable to use TS_PAD as SSPI_PAD
                    // [0:0] reg_en_sspi_pad
                    // [1:1] reg_ts_sspi_en, 1: use TS_PAD as SSPI_PAD
                    msb1245_SetReg2Bytes(u8DemodIndex,0x0900+(0x3b)*2, 0x0002);

                    // ------- MSPI protocol setting
                    // [8] cpha
                    // [9] cpol
                    msb1245_GetReg(u8DemodIndex,0x0900+(0x3a)*2+1,&u8_tmp);
                    u8_tmp &= 0xFC;
                    msb1245_SetReg(u8DemodIndex,0x0900+(0x3a)*2+1, u8_tmp);

                    // ------- MSPI driving setting
                    msb1245_SetReg2Bytes(u8DemodIndex,0x0900+(0x2c)*2, 0x07ff);


                    for(i=0; i<EXT_DMD_MSPI_PATH_MAX; i++)
                    {
                        SPIPAD_EN[i](TRUE);

                        MDrv_DMD_SSPI_RIU_Read8(0x900, &u8_tmp);
                        MDrv_DMD_SSPI_RIU_Read8(0x900, &u8_tmp);

                        if(u8_tmp == MSB1245_CHIP_ID)
                        {
                            pMSB1245->u32DmxInputPath = (MS_U32)i;
                            SPIPAD_EN[i](FALSE);
                            DMD_DBG(("Get MSB1245 chip ID by MSPI on TS%d\n", (int)pMSB1245->u32DmxInputPath));
                            break;
                        }
                        else
                        {
                            DMD_DBG(("Cannot get MSB1245 chip ID by MSPI on TS%x\n", i));
                            if( i == (EXT_DMD_MSPI_PATH_MAX - 1))
                                pMSB1245->u32DmxInputPath = EXT_DMD_MSPI_PATH_NONE;
                        }

                        SPIPAD_EN[i](FALSE);
                    }

                    // ------disable to use TS_PAD as SSPI_PAD after load code
                    // [0:0] reg_en_sspi_pad
                    // [1:1] reg_ts_sspi_en, 1: use TS_PAD as SSPI_PAD
                    msb1245_SetReg2Bytes(u8DemodIndex, 0x0900 + (0x3b) * 2, 0x0001);
            }
        }
        return TRUE;
    }
    else
    {
        if(!u8MSB1245_DevCnt)
            MSB1245_Variables_free();
        return FALSE;
    }

}

MS_BOOL MSB1245_Demod_GetTSBitRate(MS_U8 u8DemodIndex,MS_U16* u16TsBitRate)
{
    MS_BOOL bRet=true;
    MS_BOOL bPilot;
    MS_U8 u8Data,u8Constellation;
    MS_U16 u16Address,u16SymbolRate;
    float CR, BitRate, kbch;
    MDvr_CofdmDmd_CONFIG *pMSB1245 = (pstMSB1245 + u8DemodIndex);

    if (!pMSB1245)
    {
        DMD_ERR(("pMSB1245 error !\n"));
        return FALSE;
    }
    if ((!pMSB1245->bInited) || (!pMSB1245->bOpen))
    {
        DMD_ERR(("[%s]pMSB1245 have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }

    if (MsOS_ObtainMutex(pMSB1245->s32_MSB1245_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("MSB1245_Demod_GetTSBitRate:Obtain mutex failed !!!\n"));
        return FALSE;
    }
    if(!MSB1245_Lock_S(u8DemodIndex))
    {
        MsOS_ReleaseMutex(pMSB1245->s32_MSB1245_Mutex);
        return FALSE;
    }

    //get SR
    u16Address=0x0B53;//DIG_DBG_2
    bRet&=MSB1245_ReadReg(pMSB1245, u16Address, &u8Data);
    u16SymbolRate=u8Data;
    u16Address=0x0B52;//DIG_DBG_3
    bRet&=MSB1245_ReadReg(pMSB1245, u16Address, &u8Data);
    u16SymbolRate=(u16SymbolRate<<8) | u8Data;

    //get parameter
    if(pMSB1245->bIsDVBS2)
    {
       //get pilot on/off
       u16Address=0x1b60;
       bRet&=MSB1245_ReadReg(pMSB1245, u16Address, &u8Data);
       if((u8Data & 0x10) == 0x10)
       {
           bPilot=1;
       }
       else
       {
           bPilot=0;
       }
       //get Mod
       u16Address=0x1B80;
       bRet&=MSB1245_ReadReg(pMSB1245, u16Address, &u8Data);
       switch((u8Data & 0x30) >> 4)
       {
           case 0x00:
                u8Constellation= 2;
                break;
           case 0x01:
                u8Constellation= 3;
                break;
           case 0x02:
                u8Constellation= 4;
           default:
                u8Constellation= 2;
                bRet = FALSE;
                break;
       }

       //get kbch
       u16Address=0x1BD7;
       bRet&=MSB1245_ReadReg(pMSB1245, u16Address, &u8Data);
       switch(u8Data & 0x3C)
       {
           case 0x00:
                kbch = 16008;
                break;
           case 0x04:
                kbch = 21408;
                break;
           case 0x08:
                kbch = 25728;
                break;
           case 0x0C:
                kbch = 32208;
                break;
           case 0x10:
                kbch = 38688;
                break;
           case 0x14:
                kbch = 43040;
                break;
           case 0x18:
                kbch = 48408;
                break;
           case 0x1C:
                kbch = 51648;
                break;
           case 0x20:
                kbch = 53840;
                break;
           case 0x24:
                kbch = 57472;
                break;
           case 0x28:
                kbch = 58192;
                break;
           default:
                kbch = 16008;
                bRet = FALSE;
                break;
       }
       //calculate Bit Rate
       if (bPilot == 1)
       {
             if (u8Constellation==2)
             {
                 BitRate = (kbch-80)/((float)(64800/u8Constellation+90+36*22)/(float)u16SymbolRate);
             }
             else
             {
                 BitRate = (kbch-80)/((float)(64800/u8Constellation+90+36*(15-1))/(float)u16SymbolRate);
             }
       }
       else
       {
            BitRate = (kbch-80)/((float)(64800/u8Constellation+90)/(float)u16SymbolRate);
       }
    }
    else
    {
       //get CR
       u16Address=0x1C84;
       bRet&=MSB1245_ReadReg(pMSB1245, u16Address, &u8Data);
       switch(u8Data & 0x07)
       {
           case 0x00:
                CR = (1.0/2.0);
                break;
           case 0x01:
                CR = (2.0/3.0);
                break;
           case 0x02:
                CR = (3.0/4.0);
                break;
           case 0x03:
                CR = (5.0/6.0);
                break;
           case 0x04:
                CR = (7.0/8.0);
                break;
           default:
                CR = (1.0/2.0);
                bRet = FALSE;
                break;
       }
       //get Mod
       u8Constellation= 2;
       //calculate Bit Rate
       BitRate =CR*(188.0/204.0)*(float)u16SymbolRate*(float)u8Constellation;
    }

    * u16TsBitRate=BitRate;
    //DMD_DBG(("MSB1245_Demod_GetTSBitRate: %d\n", * u16TsBitRate));

    MsOS_ReleaseMutex(pMSB1245->s32_MSB1245_Mutex);
    return bRet;
}



MS_BOOL MSB1245_Extension_Function(MS_U8 u8DemodIndex, DEMOD_EXT_FUNCTION_TYPE fuction_type, void *data)
{
    MS_BOOL bret = TRUE;
    MDvr_CofdmDmd_CONFIG *pMSB1245 = (pstMSB1245 + u8DemodIndex);
    DEMOD_EXT_FUNCTION_PARAM* pstData;
    DEMOD_CON_CONFIG* pstCon;
    InterruptNum IntNum;

    switch(fuction_type)
    {
    case DEMOD_EXT_FUNC_OPEN:
        bret &= MSB1245_Demod_Open(u8DemodIndex);
        break;
    case DEMOD_EXT_FUNC_CLOSE:
        bret &= MSB1245_Demod_Close(u8DemodIndex);
        break;
    case DEMOD_EXT_FUNC_FINALIZE:
        if(!pstMSB1245)
        {
          DMD_ERR(("pMSB1245 error !\n"));
          bret = FALSE;
          break;
        }

        if(pMSB1245->s32_MSB1245_Mutex >= 0)
        {
            bret &= MsOS_DeleteMutex(pMSB1245->s32_MSB1245_Mutex);
            pMSB1245->s32_MSB1245_Mutex = -1;
        }
        _msb1245_hw_reset(u8DemodIndex);//for I2C re-write password issue
        break;
    case DEMOD_EXT_FUNC_CLEAR_STATUS:
        bret &= MSB1245_Demod_ClearStatus(u8DemodIndex);
        break;
    case DEMOD_EXT_FUNC_GET_PACKET_ERR:
        bret &= MSB1245_Get_Packet_Error(u8DemodIndex, (MS_U16 *)data);
        break;
    case DEMOD_EXT_FUNC_GET_PACKGE_INFO:
        *(MS_BOOL *)(data) = pMSB1245->bIsMCP_DMD;
        break;
    case DEMOD_EXT_FUNC_POWER_ON_OFF:
        _msb1245_power_onoff(u8DemodIndex, *(MS_BOOL*)(data));
        break;

#if MS_DVBS_INUSE
    case DEMOD_EXT_FUNC_SET_DISEQC_TX_22K_OFF:
        if(!pstMSB1245)
        {
          DMD_ERR(("pMSB1245 error !\n"));
          bret = FALSE;
          break;
        }
        pMSB1245->bDiSeqc_Tx22K_Off = *(MS_BOOL*)(data);
        if(pMSB1245->bInited == TRUE)
        {
            bret &= MSB1245_DiSEqC_Init(u8DemodIndex);
        }
        break;
#endif
    case DEMOD_EXT_FUNC_GET_IFAGC_OUT:
        bret &= msb1245_AGC_Info(u8DemodIndex, DBG_AGC_OUT, (MS_U16*)(data));
        break;

    case DEMOD_EXT_FUNC_GET_BIT_RATE:
        bret &= MSB1245_Demod_GetTSBitRate(u8DemodIndex, (MS_U16*)(data));
        break;
    case DEMOD_EXT_FUNC_SET_INTERRUPT_CALLBACK:
        pMSB1245->MSB1245_InitParam.fpCB= (fpDemodCB)(data);
        break;

    case DEMOD_EXT_FUNC_SET_CON_INFO:
        pstData = (DEMOD_EXT_FUNCTION_PARAM*)data;
        u8max_dev_num = pstData->u32Param1;
        pstCon = (DEMOD_CON_CONFIG*)(pstData->pParam);
        if(!MSB1245_Variables_alloc())
        {
            MSB1245_Variables_free();
            return FALSE;
        }
        else
        {
            pMSB1245 = (pstMSB1245 + u8DemodIndex);
            memcpy(&pMSB1245->MSB1245_InitParam.stDMDCon, pstCon,sizeof(DEMOD_CON_CONFIG));
            if(_mdrv_dmd_msb1245_get_IntNum(u8DemodIndex, &IntNum))
            {
                GET_DEMOD_ENTRY_NODE(DEMOD_MSB1245).SupportINT = TRUE;
            }
            bTS_SERIAL = !pMSB1245->MSB1245_InitParam.stDMDCon.bTSIsParallel;//Will be used in MSPI path detection only
        }
        break;
    default:
        DMD_DBG(("Request extension function (%x) does not exist\n",fuction_type));
        break;
    }
    return bret;
}
#if MS_DVBS_INUSE

MS_BOOL MSB1245_DiSEqC_Init(MS_U8 u8DemodIndex)
{
    MS_BOOL bRet=TRUE;
    MS_U16 u16Address;
    MS_U8 u8Data;
    MDvr_CofdmDmd_CONFIG *pMSB1245 = (pstMSB1245 + u8DemodIndex);

    u16Address=0x0DCD; //Clear Status
    bRet&=MSB1245_ReadReg(pMSB1245, u16Address, &u8Data);
    u8Data=(u8Data&(~0x2E))|(0x2E);
    bRet&=MSB1245_WriteReg(pMSB1245, u16Address, u8Data);

    u16Address=0x0DC0;
    u8Data=0x00;
    bRet&=MSB1245_WriteReg(pMSB1245, u16Address, u8Data);

    //u16Address=0x0DC0;
    bRet&=MSB1245_ReadReg(pMSB1245, u16Address, &u8Data);
    u8Data=(u8Data&(~0x06))|(0x06);
    bRet&=MSB1245_WriteReg(pMSB1245, u16Address, u8Data);

   if(pMSB1245->bIsMCP_DMD) // for MCP demod
    {
        //Set MSPI_DO pin as DiSeqc_out, tone mode
       bRet&=MSB1245_WriteReg(pMSB1245, 0x9D6, 0x80);
       bRet&=MSB1245_WriteReg(pMSB1245, 0x9D7, 0x01);
       bRet&=MSB1245_ReadReg(pMSB1245, 0x976, &u8Data);
       u8Data=(u8Data&(~0x01));
       bRet&=MSB1245_WriteReg(pMSB1245, 0x976, u8Data);
    }

    if(pMSB1245->bDiSeqc_Tx22K_Off)
    {
        //Disable DiSeqc tone mode, depend on LNB control IC
        bRet&=MSB1245_ReadReg(pMSB1245, 0xDD7, &u8Data);
        u8Data|= 0x80;
        bRet&=MSB1245_WriteReg(pMSB1245, 0xDD7, u8Data);
    }
    return bRet;
}

MS_BOOL MSB1245_DiSEqC_SetLNBOut(MS_U8 u8DemodIndex,MS_BOOL bLow)
{
    MS_BOOL bRet=TRUE;
    MS_U16 u16Address;
    MS_U8 u8Data;
    MDvr_CofdmDmd_CONFIG *pMSB1245 = (pstMSB1245 + u8DemodIndex);

    if (!pMSB1245)
    {
        DMD_ERR(("pMSB124X error !\n"));
        return FALSE;
    }
    if ((!pMSB1245->bInited) || (!pMSB1245->bOpen))
    {
        DMD_ERR(("[%s]pMSB124X have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }
    if (MsOS_ObtainMutex(pMSB1245->s32_MSB1245_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("MDrv_Demod_Restart:Obtain mutex failed !!!\n"));
        return FALSE;
    }

    u16Address=0x0DC2;
    bRet&=MSB1245_ReadReg(pMSB1245, u16Address, &u8Data);
    if (bLow==TRUE)
    {
        u8Data=(u8Data|0x40);
    }
    else
    {
        u8Data=(u8Data&(~0x40));
    }
    bRet&=MSB1245_WriteReg(pMSB1245, u16Address, u8Data);
    MsOS_ReleaseMutex(pMSB1245->s32_MSB1245_Mutex);
    return bRet;
}
MS_BOOL MSB1245_DiSEqC_GetLNBOut(MS_U8 u8DemodIndex,MS_BOOL* bLNBOutLow)
{
    MS_BOOL bRet=TRUE;
    MS_U16 u16Address;
    MS_U8 u8Data;
    MDvr_CofdmDmd_CONFIG *pMSB1245 = (pstMSB1245 + u8DemodIndex);

    if (!pMSB1245)
    {
        DMD_ERR(("pMSB1245 error !\n"));
        return FALSE;
    }
    if ((!pMSB1245->bInited) || (!pMSB1245->bOpen))
    {
        DMD_ERR(("[%s]pMSB1245 have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }
    if (MsOS_ObtainMutex(pMSB1245->s32_MSB1245_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("MDrv_Demod_Restart:Obtain mutex failed !!!\n"));
        return FALSE;
    }


    u16Address=0x0DC2;
    bRet&=MSB1245_ReadReg(pMSB1245, u16Address, &u8Data);
    if( (u8Data&0x40)==0x40)
    {
        * bLNBOutLow=TRUE;
    }
    else
    {
        * bLNBOutLow=FALSE;
    }
    MsOS_ReleaseMutex(pMSB1245->s32_MSB1245_Mutex);
    return bRet;
}
MS_BOOL MSB1245_DiSEqC_Set22kOnOff(MS_U8 u8DemodIndex,MS_BOOL b22kOn)
{
    MS_BOOL bRet=TRUE;
    MS_U16 u16Address;
    MS_U8   u8Data;
    MDvr_CofdmDmd_CONFIG *pMSB1245 = (pstMSB1245 + u8DemodIndex);

    if (!pMSB1245)
    {
        DMD_ERR(("pMSB124X error !\n"));
        return FALSE;
    }
    if ((!pMSB1245->bInited) || (!pMSB1245->bOpen))
    {
        DMD_ERR(("[%s]pMSB1245 have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }
    if (MsOS_ObtainMutex(pMSB1245->s32_MSB1245_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("MDrv_Demod_Restart:Obtain mutex failed !!!\n"));
        return FALSE;
    }

    u16Address=0x0DC2;
    bRet&=MSB1245_ReadReg(pMSB1245, u16Address, &u8Data);
    if (b22kOn==TRUE)
    {
        u8Data=(u8Data&0xc7);
        u8Data=(u8Data|0x08);
    }
    else
    {
        u8Data=(u8Data&0xc7);
    }
    bRet&=MSB1245_WriteReg(pMSB1245, u16Address, u8Data);
    MsOS_ReleaseMutex(pMSB1245->s32_MSB1245_Mutex);
    return bRet;
}
MS_BOOL MSB1245_DiSEqC_Get22kOnOff(MS_U8 u8DemodIndex,MS_BOOL* b22kOn)
{
    MS_BOOL bRet=TRUE;
    MS_U16 u16Address;
    MS_U8   u8Data;
    MDvr_CofdmDmd_CONFIG *pMSB1245 = (pstMSB1245 + u8DemodIndex);

    if (!pMSB1245)
    {
        DMD_ERR(("pMSB124X error !\n"));
        return FALSE;
    }
    if ((!pMSB1245->bInited) || (!pMSB1245->bOpen))
    {
        DMD_ERR(("[%s]pMSB1245 have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }
    if (MsOS_ObtainMutex(pMSB1245->s32_MSB1245_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("MDrv_Demod_Restart:Obtain mutex failed !!!\n"));
        return FALSE;
    }

    u16Address=0x0DC2;
    bRet&=MSB1245_ReadReg(pMSB1245, u16Address, &u8Data);
    if ((u8Data&0x38)==0x08)
    {
        *b22kOn=TRUE;
    }
    else
    {
        *b22kOn=FALSE;
    }
    MsOS_ReleaseMutex(pMSB1245->s32_MSB1245_Mutex);
    return bRet;
}

MS_BOOL MSB1245_DiSEqC_SendCmd(MS_U8 u8DemodIndex,MS_U8* pCmd,MS_U8 u8CmdSize)
{
    MS_BOOL bRet=TRUE;
    MS_U16 u16Address;
    MS_U8   u8Data;
    MS_U8   u8Index;
    MS_U16 u16WaitCount;
    MDvr_CofdmDmd_CONFIG *pMSB1245 = (pstMSB1245 + u8DemodIndex);

    if (!pMSB1245)
    {
        DMD_ERR(("pMSB124X error !\n"));
        return FALSE;
    }
    if ((!pMSB1245->bInited) || (!pMSB1245->bOpen))
    {
        DMD_ERR(("[%s]pMSB1245 have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }
    if (MsOS_ObtainMutex(pMSB1245->s32_MSB1245_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("MDrv_Demod_Restart:Obtain mutex failed !!!\n"));
        return FALSE;
    }

    u16Address=0x0990;
    bRet&=MSB1245_ReadReg(pMSB1245, u16Address, &u8Data);
    u8Data=(u8Data&~(0x10));
    bRet&=MSB1245_WriteReg(pMSB1245, u16Address, u8Data);

    //u16Address=0x0BC4;
    for (u8Index=0; u8Index < u8CmdSize; u8Index++)
    {
        u16Address=0x0DC4+u8Index;
        u8Data=*(pCmd+u8Index);
        bRet&=MSB1245_WriteReg(pMSB1245, u16Address, u8Data);
    }
    u8Data=((u8CmdSize-1)&0x07)|0x40;
    //Tone and Burst switch,Mantis 0232220
    //if(((*pCmd)==0xE0)&&((*(pCmd + 1))==0x10)&&((*(pCmd + 2))==0x38)&&((((*(pCmd + 3))&0x0C)==0x0C)||(((*(pCmd + 3))&0x04)==0x04)))
   // {
    //    u8Data|=0x80;   //u8Data|=0x20;Tone Burst1
    //}
    //else if(((*pCmd)==0xE0)&&((*(pCmd + 1))==0x10)&&((*(pCmd + 2))==0x38))
    //{
    //    u8Data|=0x20;   //u8Data|=0x80;ToneBurst0
    //}
   if (_u8ToneBurstFlag==1)
    {
      u8Data|=0x80;//0x20;
    }
   else if (_u8ToneBurstFlag==2)
   {
     u8Data|=0x20;//0x80;
   }

    _u8ToneBurstFlag=0;
    u16Address=0x0B54;
    bRet&=MSB1245_WriteReg(pMSB1245, u16Address, u8Data);
    MsOS_DelayTask(10);
    u16Address=0x0990;
    bRet&=MSB1245_ReadReg(pMSB1245, u16Address, &u8Data);
    u8Data=u8Data|0x10;
    bRet&=MSB1245_WriteReg(pMSB1245, u16Address, u8Data);

#if 1       //For Unicable command timing,mick
    u16WaitCount=0;
    do
    {
        u16Address=0x0990;
        bRet&=MSB1245_ReadReg(pMSB1245, u16Address, &u8Data);
        MsOS_DelayTask(1);
        u16WaitCount++;
    }
    while(((u8Data&0x10)==0x10)&&(u16WaitCount < MSB1245_DEMOD_WAIT_TIMEOUT)) ;

    if (u16WaitCount >= MSB1245_DEMOD_WAIT_TIMEOUT)
    {
        DMD_ERR(("MSB1245 DVBS DiSEqC Send Command Busy!!!\n"));
        MsOS_ReleaseMutex(pMSB1245->s32_MSB1245_Mutex);
        return FALSE;
    }
#endif      //For Unicable command timing,mick

    DMD_DBG(("\r\n MDrv_DiSEqC_SendCmd----"));
    MsOS_ReleaseMutex(pMSB1245->s32_MSB1245_Mutex);
    return bRet;
}

MS_BOOL MSB1245_DiSEqC_SetTone(MS_U8 u8DemodIndex,MS_BOOL bTone1)
{
    MS_BOOL bRet=TRUE;
    MS_U16 u16Address;
    MS_U8 u8Data;
    MS_U8 u8ReSet22k;
    MDvr_CofdmDmd_CONFIG *pMSB1245 = (pstMSB1245 + u8DemodIndex);

    u16Address=0x0DC4;
    u8Data=0x01;
    bRet&=MSB1245_WriteReg(pMSB1245, u16Address, u8Data);

    u16Address=0x0DC0;
    u8Data=0x4E;
    bRet&=MSB1245_WriteReg(pMSB1245, u16Address, u8Data);
    u16Address=0x0DCC;
    u8Data=0x88;
    bRet&=MSB1245_WriteReg(pMSB1245, u16Address, u8Data);

    u16Address=0x0DC2;
    bRet&=MSB1245_ReadReg(pMSB1245, u16Address, &u8Data);
    u8ReSet22k=u8Data;

    if (bTone1==TRUE)
    {
        u8Data=0x19;
        bRet&=MSB1245_WriteReg(pMSB1245, u16Address, u8Data);
        _u8ToneBurstFlag=1;
    }
    else
    {
        u8Data=0x11;
        bRet&=MSB1245_WriteReg(pMSB1245, u16Address, u8Data);
        _u8ToneBurstFlag=2;
    }
    u16Address=0x0DCD;
    u8Data=u8Data&~(0x01);
    bRet&=MSB1245_ReadReg(pMSB1245, u16Address, &u8Data);
    u8Data=u8Data|0x3E;
    bRet&=MSB1245_WriteReg(pMSB1245, u16Address, u8Data);
    MsOS_DelayTask(10);
    bRet&=MSB1245_ReadReg(pMSB1245, u16Address, &u8Data);
    u8Data=u8Data&~(0x3E);
    bRet&=MSB1245_WriteReg(pMSB1245, u16Address, u8Data);
    MsOS_DelayTask(1);
    u8Data=u8Data|0x01;
    bRet&=MSB1245_WriteReg(pMSB1245, u16Address, u8Data);
    MsOS_DelayTask(100);
    u16Address=0x0DC2;
    u8Data=u8ReSet22k;
    bRet&=MSB1245_WriteReg(pMSB1245, u16Address, u8Data);

    //MsOS_DelayTask(100);
    return bRet;
}

MS_BOOL MSB1245_Demod_BlindScan_Start(MS_U8 u8DemodIndex, MS_U16 u16StartFreq,MS_U16 u16EndFreq)
{
    MS_BOOL bRet=TRUE;
    MS_U16 u16Address;
    MS_U8 u8Data;
    MDvr_CofdmDmd_CONFIG *pMSB1245 = (pstMSB1245 + u8DemodIndex);

    if (MsOS_ObtainMutex(pMSB1245->s32_MSB1245_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("MDrv_Demod_Restart:Obtain mutex failed !!!\n"));
        return FALSE;
    }

    DMD_DBG(("MDrv_Demod_BlindScan_Start+\n"));
    _u32DemodStartTime=MsOS_GetSystemTime();//claire test
    _u16BlindScanStartFreq=u16StartFreq;
    _u16BlindScanEndFreq=u16EndFreq;
    _u16TunerCenterFreq=0;
    _u16ChannelInfoIndex=0;
    u16Address=0x0990;
    bRet&=MSB1245_ReadReg(pMSB1245, u16Address, &u8Data);
    u8Data&=0xF0;
    bRet&=MSB1245_WriteReg(pMSB1245, u16Address, u8Data);
    u16Address=0x0992;
    bRet&=MSB1245_WriteReg2bytes(pMSB1245, u16Address, _u16BlindScanStartFreq);
    DMD_DBG(("MDrv_Demod_BlindScan_Start- _u16BlindScanStartFreq%d u16StartFreq %d u16EndFreq %d\n", _u16BlindScanStartFreq, u16StartFreq, u16EndFreq));
    MsOS_ReleaseMutex(pMSB1245->s32_MSB1245_Mutex);
    return bRet;
}

MS_BOOL MSB1245_Demod_BlindScan_NextFreq(MS_U8 u8DemodIndex, MS_BOOL* bBlindScanEnd)
{
    MS_BOOL bRet=TRUE;
    MS_U16 u16Address;
    MS_U8   u8Data;
    MS_U32  u32TunerCutOffFreq;
    MS_U16 u16WaitCount;
    MDvr_CofdmDmd_CONFIG *pMSB1245 = (pstMSB1245 + u8DemodIndex);


    if (MsOS_ObtainMutex(pMSB1245->s32_MSB1245_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("MDrv_Demod_Restart:Obtain mutex failed !!!\n"));
        return FALSE;
    }

    DMD_DBG(("MDrv_Demod_BlindScan_NextFreq+\n"));
    u16Address=0x0990;
    bRet&=MSB1245_ReadReg(pMSB1245, u16Address, &u8Data);
    if ((u8Data&0x02)==0x02)
    {
            u8Data|=0x08;
            bRet&=MSB1245_WriteReg(pMSB1245, u16Address, u8Data);
            u16WaitCount=0;
            do
            {
                u16Address=0x0B56;
                bRet&=MSB1245_ReadReg(pMSB1245, u16Address, &u8Data);
                u16WaitCount++;
                DMD_DBG(("MDrv_Demod_BlindScan_NextFreq u8Data:0x%x u16WaitCount:%d\n", u8Data, u16WaitCount));
                MsOS_DelayTask(1);
            }while((u8Data!=0x01)&&(u16WaitCount<MSB1245_DEMOD_WAIT_TIMEOUT));
     }
    u16WaitCount=0;

    * bBlindScanEnd=FALSE;
    _u16TunerCenterFreq=0;
    u16Address=0x0993;
    bRet&=MSB1245_ReadReg(pMSB1245, u16Address, &u8Data);
    _u16TunerCenterFreq=u8Data;
    u16Address=0x0992;
    bRet&=MSB1245_ReadReg(pMSB1245, u16Address, &u8Data);
    _u16TunerCenterFreq=(_u16TunerCenterFreq<<8)|u8Data;

    if (_u16TunerCenterFreq >=_u16BlindScanEndFreq)
    {
        DMD_DBG(("MDrv_Demod_BlindScan_NextFreq . _u16TunerCenterFreq %d _u16BlindScanEndFreq%d\n", _u16TunerCenterFreq, _u16BlindScanEndFreq));
        DMD_DBG((">>>blind scan time = %ld(ms)<<<\n", (long int)(MsOS_GetSystemTime() - _u32DemodStartTime)));//claire test
        * bBlindScanEnd=TRUE;
        MsOS_ReleaseMutex(pMSB1245->s32_MSB1245_Mutex);
        return bRet;
    }

    if(TUNER_AV2011 == pMSB1245->MSB1245_InitParam.pstTunertab->data)
        u32TunerCutOffFreq=44000;
    else if(TUNER_AV2018 == pMSB1245->MSB1245_InitParam.pstTunertab->data)
        u32TunerCutOffFreq=50000;
    else
        u32TunerCutOffFreq=44000;//34000

    if(_u16TunerCenterFreq<=990)//980
    {
       u16Address=0x0B51;
       bRet&=MSB1245_ReadReg(pMSB1245,u16Address, &u8Data);
       if(u8Data==0x01)
       {
          if(_u16TunerCenterFreq<970)//970
          {
            u32TunerCutOffFreq=10000;
          }
          else
          {
            u32TunerCutOffFreq=20000;
          }
          u16Address=0x0B51;
          u8Data=0x02;
          bRet&=MSB1245_WriteReg(pMSB1245,u16Address, u8Data);
       }
       else if(u8Data==0x02)
       {
          u16Address=0x0B51;
          u8Data=0x00;
          bRet&=MSB1245_WriteReg(pMSB1245,u16Address, u8Data);
       }
    }
    MSB1245_IIC_Bypass_Mode(u8DemodIndex, TRUE);
    bRet&=MSB1245_DVBS_SetFrequency(u8DemodIndex, _u16TunerCenterFreq,u32TunerCutOffFreq);
    MSB1245_IIC_Bypass_Mode(u8DemodIndex, FALSE);

    if(TUNER_RT710 != pMSB1245->MSB1245_InitParam.pstTunertab->data)
        MsOS_DelayTask(10);

    u16Address=0x0990;
    bRet&=MSB1245_ReadReg(pMSB1245, u16Address, &u8Data);
    if ((u8Data&0x02)==0x00)
    {
        u8Data&=~(0x08);
        bRet&=MSB1245_WriteReg(pMSB1245, u16Address, u8Data);
        u8Data|=0x02;
        bRet&=MSB1245_WriteReg(pMSB1245, u16Address, u8Data);
        u8Data|=0x01;
        bRet&=MSB1245_WriteReg(pMSB1245, u16Address, u8Data);
    }
    else
    {
        u8Data&=~(0x08);
        bRet&=MSB1245_WriteReg(pMSB1245, u16Address, u8Data);
    }
    DMD_DBG(("MDrv_Demod_BlindScan_NextFreq _u16TunerCenterFreq:%d-\n", _u16TunerCenterFreq));
    MsOS_ReleaseMutex(pMSB1245->s32_MSB1245_Mutex);
    return bRet;
}

MS_BOOL MSB1245_Demod_BlindScan_WaitCurFreqFinished(MS_U8 u8DemodIndex, MS_U8* u8Progress,MS_U8 *u8FindNum)
{
    MS_BOOL bRet=TRUE;
    MS_U16 u16Address;
    MS_U32  u32Data;
    MS_U16 u16Data;
    MS_U8   u8Data;
    MS_U16  u16WaitCount;
    MDvr_CofdmDmd_CONFIG *pMSB1245 = (pstMSB1245 + u8DemodIndex);

    if (MsOS_ObtainMutex(pMSB1245->s32_MSB1245_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("MDrv_Demod_Restart:Obtain mutex failed !!!\n"));
        return FALSE;
    }

    DMD_DBG(("MDrv_Demod_BlindScan_WaitCurFreqFinished+\n"));
    u16WaitCount=0;
    *u8FindNum=0;
    *u8Progress=0;
    do
    {
        u16Address=0x0B56; // "DIG_DBG_6" means state
        bRet&=MSB1245_ReadReg(pMSB1245, u16Address, &u8Data);
        u16WaitCount++;
        DMD_DBG(("MDrv_Demod_BlindScan_WaitCurFreqFinished+1 u8Data:0x%x u16WaitCount:%d\n", u8Data, u16WaitCount));
        MsOS_DelayTask(1);

    }while((u8Data!=17)&&(u16WaitCount<MSB1245_DEMOD_WAIT_TIMEOUT));//#define     DVBS_BLIND_SCAN      17



    if (u16WaitCount >= MSB1245_DEMOD_WAIT_TIMEOUT)
    {
        bRet=FALSE;
    }
    else
    {
        u16Address=0x0B57;// "DIG_DBG_7" means sub_state
        bRet&=MSB1245_ReadReg(pMSB1245, u16Address, &u8Data);
        if (u8Data==0)
        {
            u16Address=0x0997;
            bRet&=MSB1245_ReadReg(pMSB1245, u16Address, &u8Data);
            u32Data=u8Data;
            u16Address=0x0996;
            bRet&=MSB1245_ReadReg(pMSB1245, u16Address, &u8Data);
            u32Data=(u32Data<<8)|u8Data;
            u16Address=0x0995;
            bRet&=MSB1245_ReadReg(pMSB1245, u16Address, &u8Data);
            u32Data=(u32Data<<8)|u8Data;
            _u16ChannelInfoArray[0][_u16ChannelInfoIndex]=((u32Data+500)/1000); //Center Freq
            _u16LockedCenterFreq=((u32Data+500)/1000);


            u16Address=0x0B53;
            bRet&=MSB1245_ReadReg(pMSB1245, u16Address, &u8Data);
            u16Data=u8Data;
            u16Address=0x0B52;
            bRet&=MSB1245_ReadReg(pMSB1245, u16Address, &u8Data);
            u16Data=(u16Data<<8)|u8Data;
            _u16ChannelInfoArray[1][_u16ChannelInfoIndex]=(u16Data);//Symbol Rate
            _u16LockedSymbolRate=u16Data;
            _u16ChannelInfoIndex++;
            *u8FindNum=_u16ChannelInfoIndex;

        }
    }

    *u8Progress=100;
    DMD_DBG(("MDrv_Demod_BlindScan_WaitCurFreqFinished u8Progress%d u8FindNum %d-\n", *u8Progress, *u8FindNum));

    MsOS_ReleaseMutex(pMSB1245->s32_MSB1245_Mutex);
    return bRet;
}

MS_BOOL MSB1245_Demod_BlindScan_Cancel(MS_U8 u8DemodIndex)
{
    MS_BOOL bRet=TRUE;
    MS_U16 u16Address;
    MS_U8   u8Data;
    MS_U16 u16Data;
    MDvr_CofdmDmd_CONFIG *pMSB1245 = (pstMSB1245 + u8DemodIndex);

    if (MsOS_ObtainMutex(pMSB1245->s32_MSB1245_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("MDrv_Demod_Restart:Obtain mutex failed !!!\n"));
        return FALSE;
    }

    DMD_DBG(("MDrv_Demod_BlindScan_Cancel+\n"));
    u16Address=0x0990;
    bRet&=MSB1245_ReadReg(pMSB1245, u16Address, &u8Data);
    u8Data&=0xF0;
    bRet&=MSB1245_WriteReg(pMSB1245, u16Address, u8Data);
    u16Address=0x0992;
    u16Data=0x0000;
    bRet&=MSB1245_WriteReg2bytes(pMSB1245, u16Address, u16Data);
    _u16TunerCenterFreq=0;
    _u16ChannelInfoIndex=0;
    DMD_DBG(("MDrv_Demod_BlindScan_Cancel-\n"));
    MsOS_ReleaseMutex(pMSB1245->s32_MSB1245_Mutex);
    return bRet;
}

MS_BOOL MSB1245_Demod_BlindScan_End(MS_U8 u8DemodIndex)
{
    MS_BOOL bRet=TRUE;
    MS_U16 u16Address;
    MS_U8   u8Data;
    MS_U16 u16Data;
    MDvr_CofdmDmd_CONFIG *pMSB1245 = (pstMSB1245 + u8DemodIndex);

    if (MsOS_ObtainMutex(pMSB1245->s32_MSB1245_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("MDrv_Demod_Restart:Obtain mutex failed !!!\n"));
        return FALSE;
    }

    DMD_DBG(("MDrv_Demod_BlindScan_End+\n"));
    u16Address=0x0990;
    bRet&=MSB1245_ReadReg(pMSB1245, u16Address, &u8Data);
    u8Data&=0xF0;
    bRet&=MSB1245_WriteReg(pMSB1245, u16Address, u8Data);
    u16Address=0x0992;
    u16Data=0x0000;
    bRet&=MSB1245_WriteReg2bytes(pMSB1245, u16Address, u16Data);
    _u16TunerCenterFreq=0;
    _u16ChannelInfoIndex=0;
    DMD_DBG(("MDrv_Demod_BlindScan_End-\n"));
    MsOS_ReleaseMutex(pMSB1245->s32_MSB1245_Mutex);
    return bRet;
}

MS_BOOL MSB1245_Demod_BlindScan_GetChannel(MS_U8 u8DemodIndex, MS_U16 u16ReadStart,MS_U16* u16TPNum,DEMOD_MS_FE_CARRIER_PARAM *pTable)
{
    MS_BOOL bRet=TRUE;
    MS_U16  u16TableIndex;
    MDvr_CofdmDmd_CONFIG *pMSB1245 = (pstMSB1245 + u8DemodIndex);

    if (MsOS_ObtainMutex(pMSB1245->s32_MSB1245_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("MDrv_Demod_Restart:Obtain mutex failed !!!\n"));
        return FALSE;
    }

    *u16TPNum=_u16ChannelInfoIndex-u16ReadStart;
    for(u16TableIndex = 0; u16TableIndex < (*u16TPNum); u16TableIndex++)
    {
        pTable[u16TableIndex].u32Frequency = _u16ChannelInfoArray[0][_u16ChannelInfoIndex-1];
        pTable[u16TableIndex].SatParam.u32SymbolRate = _u16ChannelInfoArray[1][_u16ChannelInfoIndex-1];
        DMD_DBG(("MDrv_Demod_BlindScan_GetChannel Freq:%ld SymbolRate:%ld\n", (long int)pTable[u16TableIndex].u32Frequency, (long int)pTable[u16TableIndex].SatParam.u32SymbolRate));
    }

    DMD_DBG(("MS124X u16TPNum:%d\n", *u16TPNum));
    MsOS_ReleaseMutex(pMSB1245->s32_MSB1245_Mutex);
    return bRet;
}

MS_BOOL MSB1245_Demod_BlindScan_GetCurrentFreq(MS_U8 u8DemodIndex, MS_U32 *u32CurrentFeq)
{
    MS_BOOL bRet=TRUE;
    MDvr_CofdmDmd_CONFIG *pMSB1245 = (pstMSB1245 + u8DemodIndex);

    if (MsOS_ObtainMutex(pMSB1245->s32_MSB1245_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("MDrv_Demod_Restart:Obtain mutex failed !!!\n"));
        return FALSE;
    }

    *u32CurrentFeq=_u16TunerCenterFreq;
    MsOS_ReleaseMutex(pMSB1245->s32_MSB1245_Mutex);
    return bRet;
}
#endif

MS_BOOL MSB1245_Demod_GetParam(MS_U8 u8DemodIndex,DEMOD_MS_FE_CARRIER_PARAM* pParam)
{
    MS_BOOL bRet=TRUE;
    MDvr_CofdmDmd_CONFIG *pMSB1245 = (pstMSB1245 + u8DemodIndex);
    MS_U16 u16Address;
    MS_U8 u8Data;

    if (!pMSB1245)
    {
        DMD_ERR(("pMSB1245 error !\n"));
        return FALSE;
    }
    if ((!pMSB1245->bInited) || (!pMSB1245->bOpen))
    {
        DMD_ERR(("[%s]pMSB1245 have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }

    if (MsOS_ObtainMutex(pMSB1245->s32_MSB1245_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("MDrv_Demod_GetLock:Obtain mutex failed !!!\n"));
        return FALSE;
    }

    if(!MSB1245_Lock_S(u8DemodIndex))
    {
        MsOS_ReleaseMutex(pMSB1245->s32_MSB1245_Mutex);
        return FALSE;
    }

    pParam->SatParam.bIsDVBS2 = pMSB1245->bIsDVBS2;

    if(pParam->SatParam.bIsDVBS2)
    {
       //spectrum inverse
       bRet&=msb1245_SetInnerDBGPort(u8DemodIndex, 0x0A);
       u16Address=0x1B65;
       bRet&=MSB1245_ReadReg(pMSB1245, u16Address, &u8Data);
       pParam->SatParam.eIQ_Mode = (DEMOD_EN_SAT_IQ_MODE)((u8Data&0x40)>>6);


       bRet&=msb1245_SetInnerDBGPort(u8DemodIndex, 0x00);
       u16Address=0x1BD7;
       bRet&=MSB1245_ReadReg(pMSB1245, u16Address, &u8Data);
       switch(u8Data & 0x3C)
       {
           case 0x00:
                pParam->SatParam.eCodeRate = DEMOD_CONV_CODE_RATE_1_4;
                break;
           case 0x04:
                pParam->SatParam.eCodeRate = DEMOD_CONV_CODE_RATE_1_3;
                break;
           case 0x08:
                pParam->SatParam.eCodeRate = DEMOD_CONV_CODE_RATE_2_5;
                break;
           case 0x0C:
                pParam->SatParam.eCodeRate = DEMOD_CONV_CODE_RATE_1_2;
                break;
           case 0x10:
                pParam->SatParam.eCodeRate = DEMOD_CONV_CODE_RATE_3_5;
                break;
           case 0x14:
                pParam->SatParam.eCodeRate = DEMOD_CONV_CODE_RATE_2_3;
                break;
           case 0x18:
                pParam->SatParam.eCodeRate = DEMOD_CONV_CODE_RATE_3_4;
                break;
           case 0x1C:
                pParam->SatParam.eCodeRate = DEMOD_CONV_CODE_RATE_4_5;
                break;
           case 0x20:
                pParam->SatParam.eCodeRate = DEMOD_CONV_CODE_RATE_5_6;
                break;
           case 0x24:
                pParam->SatParam.eCodeRate = DEMOD_CONV_CODE_RATE_8_9;
                break;
           case 0x28:
                pParam->SatParam.eCodeRate = DEMOD_CONV_CODE_RATE_9_10;
                break;
           default:
                bRet = FALSE;
                break;
       }

       u16Address=0x1B80;
       bRet&=MSB1245_ReadReg(pMSB1245, u16Address, &u8Data);
       pParam->SatParam.eConstellation= (DEMOD_EN_SAT_CONSTEL_TYPE)((u8Data & 0x30) >> 4);

       u16Address = 0x2986;
       bRet &= MSB1245_ReadReg(pMSB1245, u16Address, &u8Data);
       switch((u8Data & 0xC0)>>6)
       {
            case 0:
                pParam->SatParam.eRollOff = DEMOD_SAT_RO_35;
                break;
            case 1:
                pParam->SatParam.eRollOff = DEMOD_SAT_RO_25;
                break;
            case 2:
                pParam->SatParam.eRollOff = DEMOD_SAT_RO_20;
                break;
            default:
                break;
       }
    }
    else
    {
       u16Address=0x1C84;
       bRet&=MSB1245_ReadReg(pMSB1245, u16Address, &u8Data);
       pParam->SatParam.eCodeRate = (DEMOD_EN_CONV_CODE_RATE_TYPE)(u8Data & 0x07);
       pParam->SatParam.eConstellation= DEMOD_SAT_QPSK;
       pParam->SatParam.eRollOff = DEMOD_SAT_RO_35;

       //spectrum inverse
       u16Address=0x1C82;
       bRet&=MSB1245_ReadReg(pMSB1245, u16Address, &u8Data);
       pParam->SatParam.eIQ_Mode = (DEMOD_EN_SAT_IQ_MODE)((u8Data&0x04)>>2);

    }

    pParam->SatParam.fCFO = MSB1245_Demod_CarrierFrequencyOffset(u8DemodIndex);
    //DMD_DBG(("Is DVBS2: %x\n", pParam->SatParam.bIsDVBS2));
    //DMD_DBG(("DEMOD_EN_CONV_CODE_RATE_TYPE: %x\n", pParam->SatParam.eCodeRate));
    //DMD_DBG(("DEMOD_EN_SAT_CONSTEL_TYPE: %x\n", pParam->SatParam.eConstellation));
    MsOS_ReleaseMutex(pMSB1245->s32_MSB1245_Mutex);
    return bRet;
}

#ifdef FE_AUTO_TEST
MS_BOOL  MSB1245_Demod_AutoTestReadReg(MS_U8 u8DemodIndex, MS_U16 RegAddr, MS_U8 *pu8Data)
{
    *pu8Data = (MS_U8)MSB1245_Demod_ReadReg(u8DemodIndex, RegAddr);
    return TRUE;
}

MS_BOOL MSB1245_Demod_AutoTestWriteReg(MS_U8 u8DemodIndex, MS_U16 RegAddr, MS_U16 RegData)
{
    return MSB1245_Demod_WriteReg(u8DemodIndex, RegAddr, RegData);
}
#endif

DRV_DEMOD_TABLE_TYPE GET_DEMOD_ENTRY_NODE(DEMOD_MSB1245) DDI_DRV_TABLE_ENTRY(demodtab) =
{
    .name                         = "DEMOD_MSB1245",
    .data                         = DEMOD_MSB1245,
    .SupportINT                   = FALSE,
    .init                         = MSB1245_Demod_Init,
    .GetLock                      = MSB1245_Demod_GetLock,
    .GetSNR                       = MSB1245_Demod_GetSNR,
    .GetBER                       = MSB1245_Demod_GetBER,
    .GetPWR                       = MSB1245_Demod_GetPWR,
    .GetSSI                       = MSB1245_Demod_GetSSI,
    .GetQuality                   = MSB1245_Demod_GetSignalQuality,
    .GetParam                     = MSB1245_Demod_GetParam,
    .Restart                      = MSB1245_Demod_Restart,
    .I2CByPass                    = MSB1245_Demod_I2C_ByPass,
    .I2CByPassPreSetting          = NULL,
    .CheckExist                   = MSB1245_Check_Exist,
    .Extension_Function           = MSB1245_Extension_Function,
    .Extension_FunctionPreSetting = NULL,
    .Get_Packet_Error             = MSB1245_Get_Packet_Error,
#ifdef FE_AUTO_TEST
     .ReadReg                     = MSB1245_Demod_AutoTestReadReg,
     .WriteReg                    = MSB1245_Demod_AutoTestWriteReg,
#endif
#if MS_DVBT2_INUSE
    .SetCurrentDemodType          = MDrv_Demod_null_SetCurrentDemodType,
    .GetCurrentDemodType          = MDrv_Demod_null_GetCurrentDemodType,
    .GetPlpBitMap                 = MDrv_Demod_null_GetPlpBitMap,
    .GetPlpGroupID                = MDrv_Demod_null_GetPlpGroupID,
    .SetPlpGroupID                = MDrv_Demod_null_SetPlpGroupID,
    .GetNextPLPID                 = MDrv_Demod_null_GetNextPLPID,
    .GetPLPType                   = MDrv_Demod_null_GetPLPType,
#endif
#if MS_DVBS_INUSE
    .BlindScanStart               = MSB1245_Demod_BlindScan_Start,
    .BlindScanNextFreq            = MSB1245_Demod_BlindScan_NextFreq,
    .BlindScanWaitCurFreqFinished = MSB1245_Demod_BlindScan_WaitCurFreqFinished,
    .BlindScanCancel              = MSB1245_Demod_BlindScan_Cancel,
    .BlindScanEnd                 = MSB1245_Demod_BlindScan_End,
    .BlindScanGetChannel          = MSB1245_Demod_BlindScan_GetChannel,
    .BlindScanGetCurrentFreq      = MSB1245_Demod_BlindScan_GetCurrentFreq,
    .DiSEqCSetTone                = MSB1245_DiSEqC_SetTone,
    .DiSEqCSetLNBOut              = MSB1245_DiSEqC_SetLNBOut,
    .DiSEqCGetLNBOut              = MSB1245_DiSEqC_GetLNBOut,
    .DiSEqCSet22kOnOff            = MSB1245_DiSEqC_Set22kOnOff,
    .DiSEqCGet22kOnOff            = MSB1245_DiSEqC_Get22kOnOff,
    .DiSEqC_SendCmd               = MSB1245_DiSEqC_SendCmd,
    .DiSEqC_GetReply              = MDrv_Demod_null_DiSEqC_GetReply,
    .GetISIDInfo                  = MDrv_Demod_null_GetVCM_ISID_INFO,
    .SetISID                      = MDrv_Demod_null_SetVCM_ISID
#endif
};

#endif

