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
// (¡§MStar Confidential Information¡¨) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
///
/// @file   appDemo_SerialFlash.h
/// @author MStar Semiconductor Inc.
/// @brief  The demo code for the Serial Flash functions
///
////////////////////////////////////////////////////////////////////////////////
#if (DEMO_SPI_TEST == 1)
//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include <string.h>
#include "MsCommon.h"
#include "drvMSPI.h"
#include "demo_mspi.h"
#include "demo_utility.h"
#include "MsOS.h"
#include "string.h"
#include "drvMMIO.h"
#include "drvSYS.h"

//-------------------------------------------------------------------------------------------------
//                                MACROS
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//  Local Compiler Options
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define MSPI_MULTIPLE_SUPPORTED 0
//SSPI COMMAND
#define EVENT_STOP_MSPI 0x01
#define TEST_DATA       0xAA
#define TEST2_DATA      0xBB
#define MSPI_PAD_MAX    0x4
#define MSPI_CHANNEL    0x3
//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//  Global Variables
//------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
#if MSPI_MULTIPLE_SUPPORTED
static void *mspi2_pTaskStack = NULL;
static MS_S32 mspi2_task_id = -1;
#endif
static void *mspi1_pTaskStack = NULL;
static MS_S32 mspi1_task_id = -1;
static MS_S32 event_id;
static MS_BOOL running = false;
static MS_U8 u8mspi_usage[2]={0xFF,0xFF};
static MS_BOOL b_ret=false;
static unsigned char _MSPI_TESTBIN[]  =
{
  #include "./../../../../../../DDI_Main/demo/data/vertsp.dat"
};
static unsigned char _MSPI_COMPAREBIN[]  =
{
  #include "./../../../../../../DDI_Main/demo/data/vertsp.dat"
};
//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
static void MSPIPAD_TS0_En(MS_BOOL bOnOff);
static void MSPIPAD_TS1_En(MS_BOOL bOnOff);
static void MSPIPAD_TS2_En(MS_BOOL bOnOff);
static void MSPIPAD_CI_En(MS_BOOL bOnOff);
static fpMSPIPAD_En MSPIPAD_EN[MSPI_PAD_MAX]= {
                                   MSPIPAD_TS0_En,\
                                   MSPIPAD_TS1_En,\
                                   MSPIPAD_TS2_En,\
                                   MSPIPAD_CI_En
                                   };

static void MSPIPAD_TS0_En(MS_BOOL bOnOff)
{
    if(bOnOff)
    {
        MDrv_SYS_SetPadMux(E_TS0_PAD_SET, E_MSPI_PAD_ON);
    }
    else
    {
        MDrv_SYS_SetPadMux(E_TS0_PAD_SET, E_MSPI_PAD_GPIO);
    }

}
static void MSPIPAD_TS1_En(MS_BOOL bOnOff)
{
    if(bOnOff)
    {
        MDrv_SYS_SetPadMux(E_TS1_PAD_SET, E_MSPI_PAD_ON);
    }
    else
    {
      MDrv_SYS_SetPadMux(E_TS1_PAD_SET, E_MSPI_PAD_GPIO);
    }
}
static void MSPIPAD_TS2_En(MS_BOOL bOnOff)
{
    if(bOnOff)
    {
        MDrv_SYS_SetPadMux(E_TS2_PAD_SET, E_MSPI_PAD_ON);
    }
    else
    {
       MDrv_SYS_SetPadMux(E_TS2_PAD_SET, E_MSPI_PAD_GPIO);
    }
}
static void MSPIPAD_CI_En(MS_BOOL bOnOff)
{
    if(bOnOff)
    {
       MDrv_SYS_SetPadMux(E_CA_CI_PAD_SET, E_MSPI_PAD_ON);
    }
       else
    {
       MDrv_SYS_SetPadMux(E_CA_CI_PAD_SET, E_MSPI_PAD_GPIO);
    }
}
static void MDrv_RW_LoopBack(MS_U8 *pdata)
{
    MS_U8 Rdata;
    Rdata= TEST_DATA;
    MDrv_MSPI_SlaveEnable(TRUE);
    // send read command to read data
    MDrv_MSPI_Write(&Rdata,1);
    // read operation
    MDrv_MSPI_Read(pdata, 1);
    MDrv_MSPI_SlaveEnable(FALSE);
}
static void mspi1_task(MS_U32 argc, void *argv)
{
  MS_U8 ch_rec=0;
  MS_U32 i=0;
  MS_U8 u8cnterr=0;
  unsigned char* pRead_Bin = NULL;
  pRead_Bin = _MSPI_COMPAREBIN;
  memset(pRead_Bin,0,sizeof(_MSPI_TESTBIN));
  ch_rec=u8mspi_usage[0];
  printf("ch_rec %x\n",ch_rec);
  MDrv_MSPI_Init_Ext(ch_rec);
  MSPIPAD_EN[ch_rec](TRUE);
#if MSPI_MULTIPLE_SUPPORTED
  MDrv_MSPI_SlaveEnable_Channel(ch_rec,TRUE);
  MDrv_MSPI_Write_Channel(ch_rec,_MSPI_TESTBIN,sizeof(_MSPI_TESTBIN));
  MDrv_MSPI_Read_Channel(ch_rec,pRead_Bin,sizeof(_MSPI_COMPAREBIN));
  MDrv_MSPI_SlaveEnable_Channel(ch_rec,FALSE);
#else
  MDrv_MSPI_SlaveEnable(TRUE);
  MDrv_MSPI_Write(_MSPI_TESTBIN,sizeof(_MSPI_TESTBIN));
  MDrv_MSPI_Read(pRead_Bin,sizeof(_MSPI_COMPAREBIN));
  MDrv_MSPI_SlaveEnable(FALSE);
#endif
  for(i=0;i<sizeof(_MSPI_COMPAREBIN);i++)
  {
    if(_MSPI_COMPAREBIN[i] != pRead_Bin[i])
    {
      //printf("mspi1_task:pRead_Bin[%x]:%x\n",i, pRead_Bin[i]);
      u8cnterr=1;
      break;
    }
  }
  if(u8cnterr)
  {
    b_ret=FALSE;
    printf("mspi1 loopbacktest FAIL\n");
  }
  else
  {
    b_ret=TRUE;
    printf("mspi1 loopbacktest PASS\n");
  }
  MSPIPAD_EN[ch_rec](FALSE);
  running = FALSE;
}
#if MSPI_MULTIPLE_SUPPORTED
static void mspi2_task(MS_U32 argc, void *argv)
{
  MS_U8 ch_rec=0;
  MS_U32 i;
  MS_U8 u8cnterr;
  unsigned char* pRead_Bin = NULL;
  pRead_Bin = _MSPI_COMPAREBIN;
  memset(pRead_Bin,0,sizeof(_MSPI_TESTBIN));
  ch_rec=u8mspi_usage[1];
  printf("ch_rec %x\n",ch_rec);
  MDrv_MSPI_Init_Ext(ch_rec);
  MSPIPAD_EN[ch_rec](TRUE);
  MDrv_MSPI_SlaveEnable_Channel(ch_rec,TRUE);
  MDrv_MSPI_Write_Channel(ch_rec,_MSPI_TESTBIN,sizeof(_MSPI_TESTBIN));
  MDrv_MSPI_Read_Channel(ch_rec,pRead_Bin,sizeof(_MSPI_COMPAREBIN));
  MDrv_MSPI_SlaveEnable_Channel(ch_rec,FALSE);
  for(i=0;i<sizeof(_MSPI_COMPAREBIN);i++)
  {
    if(_MSPI_COMPAREBIN[i] != pRead_Bin[i])
    {
      //printf("mspi2_task:pRead_Bin[%x]:%x\n",i, pRead_Bin[i]);
      u8cnterr=1;
      break;
    }
  }
  if(u8cnterr)
  {
    b_ret=FALSE;
    printf("mspi2 loopbacktest FAIL\n");
  }
  else
  {
    b_ret=TRUE;
  printf("mspi2 loopbacktest PASS\n");
  }
  MSPIPAD_EN[ch_rec](FALSE);
  running = FALSE;
}
#endif
MS_BOOL Mspi_test_stop(void)
{
    if (running == FALSE) {
        printf("no instance running\n");
        return FALSE;
    }
    running = FALSE;
    MsOS_SetEvent(event_id, EVENT_STOP_MSPI);
    MsOS_DeleteEventGroup(event_id);
    event_id = -1;
    return TRUE;
}

MS_BOOL Demo_MSPI_LoopBack_Test(void)
{
 // MS_S32 s32MstarCachedPoolID = INVALID_POOL_ID;
  MS_U8 u8_tmp;
  MS_U8 u8tspad=0,u8chtmp=0,u8id=0;
  MS_S32 s32MstarCachedPoolID = INVALID_POOL_ID;
  if (running == TRUE) {
        printf("already running\n");
        return FALSE;
  }
  for(u8chtmp=0;u8chtmp<MSPI_CHANNEL;u8chtmp++)
  {
     if(MDrv_MSPI_Init_Ext(u8chtmp) != E_MSPI_OK)
     {
       printf("MSPI%x Init Fail",u8chtmp);
       break;
     }
     else
     {
       for(u8tspad=0; u8tspad<MSPI_PAD_MAX; u8tspad++)
       {
          MSPIPAD_EN[u8tspad](TRUE);
          MDrv_RW_LoopBack(&u8_tmp);
          if(u8_tmp == TEST_DATA)
          {
             u8mspi_usage[u8id]=u8chtmp;
             printf("Find TS %d \n",u8tspad);
             u8id++;
             break;
          }
          else
          {
            printf("Can't Find TS PAD %d\n",u8tspad);
          }
          MSPIPAD_EN[u8tspad](FALSE);
        }
      }
  }

  event_id = MsOS_CreateEventGroup("mspi_event");
  //Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_CACHE, &s32MstarCachedPoolID);
  if(u8mspi_usage[0]!=0xFF)
  {
     mspi1_pTaskStack = MsOS_AllocateMemory(4096, s32MstarCachedPoolID);
     if (mspi1_pTaskStack == NULL) {
        printf("failed to allocate mspi1 Task Stack\n");
        return FALSE;
     }
     mspi1_task_id = MsOS_CreateTask(mspi1_task,
            (MS_U32)NULL,
            E_TASK_PRI_HIGH,
            TRUE,
            mspi1_pTaskStack ,
            4096, // stack size
            "mspi1 task");
      if (mspi1_task_id < 0) {
        printf("failed to create mspi1 task\n");
        return FALSE;
      }
  }
#if MSPI_MULTIPLE_SUPPORTED
  if(u8mspi_usage[1]!=0xFF)
  {
    mspi2_pTaskStack = MsOS_AllocateMemory(4096, s32MstarCachedPoolID);
    if (mspi2_pTaskStack == NULL) {
        printf("failed to allocate mspi2 Task Stack\n");
        return FALSE;
    }
    mspi2_task_id = MsOS_CreateTask(mspi2_task,
            (MS_U32)NULL,
            E_TASK_PRI_HIGH,
            TRUE,
            mspi2_pTaskStack ,
            4096, // stack size
            "mspi2 task");
    if (mspi2_task_id < 0) {
        printf("failed to create mspi2 task\n");
        return FALSE;
    }
  }
#endif
  running = TRUE;

  return  b_ret;
}
#endif
