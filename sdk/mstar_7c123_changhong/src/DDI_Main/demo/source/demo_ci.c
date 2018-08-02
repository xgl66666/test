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
#if (DEMO_CI_TEST == 1)
//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------
#include "MsCommon.h"
#include <string.h>

#include "drvGPIO.h"
#include "apiDMX.h"

#include "msAPP_CI.h"
#include "demo_main.h"
#include "demo_utility.h"
#include "demo_ci.h"

//--------------------------------------------------------------------------------------------------
// Local Variables
//--------------------------------------------------------------------------------------------------
MS_U8 m_u8TS_State;
MMI_INFO *m_pMmiInfo = NULL;
MS_U8 pStrDest[1024]={0};
#if (DEMO_CI_PLUS_TEST==1)
static MS_U16 u16NotifyTS_ID = 0, u16NotifyServiceID = 0;
#endif
static Task_Info _demo_CI_task = {-1, E_TASK_PRI_MEDIUM, NULL, 4096, "Demo Task"};
//------------------------------------------------------------------------------
// Debug Macros
//------------------------------------------------------------------------------
#define CI_DBG              0
#if CI_DBG
#define db_print(fmt, args...)  printf("[%s][%d]" fmt, __FUNCTION__, __LINE__, ## args)
#else
#define db_print(fmt, args...)  while(0);
#endif

static MS_U8* _data_dump(MS_U8* pu8Buf, MS_U16 BufSize)
{
    MS_U16 i ;

    memset(pStrDest, 0, sizeof(pStrDest));
    if(BufSize > 1024)
    {
        BufSize = 1024;
    }

    for( i = 0 ; i < BufSize ; i ++ )
    {
        pStrDest[i] = *(pu8Buf + i);
    }

    return pStrDest;
}

//-------------------------------------------------------------------------------------------------
// Local Definition
//-------------------------------------------------------------------------------------------------
MS_BOOL appMSrv_CIMMI_IsGetData(void);
void appMSrv_CIMMI_FreeData(void);
void appMSrv_CIMMI_GetData(void);
EN_MMI_TYPE appMSrv_CIMMI_GetMMIType(void);
MS_U16 appMSrv_CIMMI_GetMenuTitleLength(void);
void appMSrv_CIMMI_GetMenuTitleStr(void);
MS_U16 appMSrv_CIMMI_GetMenuSubTitleLength(void);
void appMSrv_CIMMI_GetMenuSubTitleStr(void);
MS_U8 appMSrv_CIMMI_GetMenuChoice_nb(void);
void appMSrv_CIMMI_GetMenuSelectionStr(MS_U8 i);
MS_U8 appMSrv_CIMMI_GetEnqbBlindAns(void);
MS_U8 appMSrv_CIMMI_GetEnqLength(void);
void appMSrv_CIMMI_GetEnqStr(void);
void appMSrv_CI_CISetByPassMode(MS_BOOL bByPass);
void appMSrv_CI_CIProtectionNotify(MS_U8* data, MS_U16 u16ONID, MS_U16 u16TSID, MS_U16 u16SID);
void appMSrv_CI_CIHSSActiveCallBack(void);
MS_BOOL appMSrv_CI_CISetSystemTime2Cache(void);
//-------------------------------------------------------------------------------------------------
// Local Function
//-------------------------------------------------------------------------------------------------
void static _Demo_CI_CIEventCallBack(EN_CI_EVENT eType)
{
    MS_U8 i;

    db_print("appDemo_CI_CIEventCallBack(%d).\n", eType);
    switch(eType)
    {
        case CI_EVENT_MMI_ENQ:
        case CI_EVENT_MMI_MENU:
        case CI_EVENT_MMI_LIST:
            appMSrv_CIMMI_GetData();
            db_print("appDemo_CIMMI_GetMMIType(%d)\n", appMSrv_CIMMI_GetMMIType());
            if(appMSrv_CIMMI_GetMMIType() == EN_MMI_TYPE_MENU || appMSrv_CIMMI_GetMMIType() == EN_MMI_TYPE_LIST)
            {
                if(appMSrv_CIMMI_GetMenuTitleLength() != 0)
                {
                    db_print("Title(%d)=(", appMSrv_CIMMI_GetMenuTitleLength());
                    appMSrv_CIMMI_GetMenuTitleStr();
                    db_print(").\n");
                }
                else
                {
                    printf("Title=().\n");
                }
                if(appMSrv_CIMMI_GetMenuSubTitleLength() != 0)
                {
                    db_print("SubTitle(%d)=(", appMSrv_CIMMI_GetMenuSubTitleLength());
                    appMSrv_CIMMI_GetMenuSubTitleStr();
                    db_print(").\n");
                }
                else
                {
                    db_print("SubTitle=().\n");
                }
                for(i = 0; i < appMSrv_CIMMI_GetMenuChoice_nb(); i++)
                {
                    if(appMSrv_CIMMI_GetMMIType() == EN_MMI_TYPE_MENU)
                    {
                        db_print("Menu(%d)=(", i + 1);
                    }
                    else
                    {
                        db_print("List(%d)=(", i + 1);
                    }
                    appMSrv_CIMMI_GetMenuSelectionStr(i);
                    db_print(").\n");
                }
            }
            else if(appMSrv_CIMMI_GetMMIType() == EN_MMI_TYPE_ENQ)
            {
                if(appMSrv_CIMMI_GetEnqbBlindAns()) //show password as star
                {
                    db_print("You shold show password as star!!!\n");
                }
                else //show password as digits
                {
                    db_print("You shold show password as digits!!!\n");
                }
                if(appMSrv_CIMMI_GetEnqLength() != 0)
                {
                    db_print("Title(%d)=(", appMSrv_CIMMI_GetEnqLength());
                    appMSrv_CIMMI_GetEnqStr();
                    db_print(").\n");
                }
                else
                {
                    db_print("Title=().\n");
                }
                db_print("Please input the four digits:\n");
            }
            break;
        case CI_EVENT_MMI_CLOSE:
            db_print("CI_EVENT_MMI_CLOSEMMI\n");
            appMSrv_CIMMI_FreeData();
            break;
        case CI_EVENT_DT_ENQ:
            db_print("CI_EVENT_DT_ENQ\n");
            appMSrv_CI_CISetSystemTime2Cache();
            break;
        case CI_EVENT_HSS:
            db_print("CI_EVENT_HSS\n");
            appMSrv_CI_CIHSSActiveCallBack();
            break;
        default:
            break;
    }
}

static void _Demo_CI_CIStateMonitor(void)
{
    if(m_u8TS_State != msAPP_CI_CardDetect())
    {
#if (DEMO_CI_PLUS_TEST==1)
        appMSrv_CI_CISetByPassMode(TRUE);
#endif
        if(!m_u8TS_State) // Card inserted
        {
            mdrv_gpio_set_high(164);
#if (DEMO_CI_PLUS_TEST==1)
            appMSrv_CI_CISetByPassMode(FALSE);
#endif
            db_print("Card inserted.\n");
        }
        else // Card removed
        {
            mdrv_gpio_set_low(164);
#if (DEMO_CI_PLUS_TEST==1)
            appMSrv_CI_CISetByPassMode(TRUE);
#endif
            db_print("Card removed.");
        }
        m_u8TS_State = msAPP_CI_CardDetect();
        db_print("---Current Card state:(%d).\n", m_u8TS_State);
    }
    if(msAPP_CI_CardDetect() == 1)
    {
        msAPP_CI_Main();
    }
}

//-------------------------------------------------------------------------------------------------
/// CI Monitoring.
///
///
//-------------------------------------------------------------------------------------------------

MS_BOOL appMSrv_CIMMI_IsGetData(void)
{
    if(m_pMmiInfo != NULL)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

void appMSrv_CIMMI_FreeData(void)
{
    if(!appMSrv_CIMMI_IsGetData())
    {
        return;
    }
    if(m_pMmiInfo->MMI_BUF != NULL)
    {
        free((void *)(m_pMmiInfo->MMI_BUF));
    }
    free((void *)m_pMmiInfo);
    m_pMmiInfo = NULL;
}

void appMSrv_CIMMI_GetData(void)
{
    appMSrv_CIMMI_FreeData();
    m_pMmiInfo = msAPP_CIMMI_GetData();
}

EN_MMI_TYPE appMSrv_CIMMI_GetMMIType(void)
{
    //ASSERT(m_pMmiInfo);
    return m_pMmiInfo->enType;
}

//------------------------------------------------------------------------------
/// @brief The sample code to enter the root CIMMI menu
/// @note
/// Command: \b Command:CIMMI_EnterMenu \n
//------------------------------------------------------------------------------
void appMSrv_CIMMI_EnterMenu(void)
{
    msAPP_CIMMI_EnterMenu();
}

//------------------------------------------------------------------------------
/// @brief The sample code to enter CIMMI menu
/// @param[in] index menu index
/// @note
/// Command: \b Command:CIMMI_AnswerMenu \n
//------------------------------------------------------------------------------
void appMSrv_CIMMI_AnswerMenu(MS_U8* index)
{
    msAPP_CIMMI_AnswerMenu(*index);
}


MS_U16 appMSrv_CIMMI_GetMenuTitleLength(void)
{
    //ASSERT(m_pMmiInfo);
    return m_pMmiInfo->content.menu.u16TitleLen;
}

void appMSrv_CIMMI_GetMenuTitleStr(void)
{
    //ASSERT(m_pMmiInfo);
    db_print("%s %u \n", _data_dump(m_pMmiInfo->content.menu.pu8Title, m_pMmiInfo->content.menu.u16TitleLen));
}

MS_U16 appMSrv_CIMMI_GetMenuSubTitleLength(void)
{
    //ASSERT(m_pMmiInfo);
    return m_pMmiInfo->content.menu.u16SubtitleLen;
}

void appMSrv_CIMMI_GetMenuSubTitleStr(void)
{
    //ASSERT(m_pMmiInfo);
    db_print("%s  \n", _data_dump(m_pMmiInfo->content.menu.pu8Subtitle, m_pMmiInfo->content.menu.u16SubtitleLen));
}

MS_U8 appMSrv_CIMMI_GetMenuChoice_nb(void)
{
    //ASSERT(m_pMmiInfo);
    return m_pMmiInfo->content.menu.u8Choice_nb;
}

void appMSrv_CIMMI_GetMenuSelectionStr(MS_U8 i)
{
    MS_U16 u16StringLen;
    //ASSERT(m_pMmiInfo);
    if(i == m_pMmiInfo->content.menu.u8Choice_nb - 1)
    {
        u16StringLen =  m_pMmiInfo->content.menu.pStringEnd - m_pMmiInfo->content.menu.pString[i];
    }
    else
    {
        u16StringLen = m_pMmiInfo->content.menu.pString[i+1] - m_pMmiInfo->content.menu.pString[i];
    }

    printf("%s \n", (char *)_data_dump(m_pMmiInfo->content.menu.pString[i], u16StringLen));
}

MS_U8 appMSrv_CIMMI_GetEnqbBlindAns(void)
{
    //ASSERT(m_pMmiInfo);
    return m_pMmiInfo->content.enq.bBlindAns;
}

MS_U8 appMSrv_CIMMI_GetEnqLength(void)
{
    //ASSERT(m_pMmiInfo);
    return m_pMmiInfo->content.enq.u8EnqLen;
}

void appMSrv_CIMMI_GetEnqStr(void)
{
    //ASSERT(m_pMmiInfo);
    db_print("%s \n", _data_dump(m_pMmiInfo->content.enq.pu8Enq, m_pMmiInfo->content.enq.u8EnqLen));
}

//------------------------------------------------------------------------------
/// @brief The sample code to enter CI password
/// @param[in] pBuffer0 password digit 0
/// @param[in] pBuffer1 password digit 1
/// @param[in] pBuffer2 password digit 2
/// @param[in] pBuffer3 password digit 3
/// @note
/// Command: \b Command:CIMMI_answerEnq \n
//------------------------------------------------------------------------------
MS_BOOL appMSrv_CIMMI_answerEnq(MS_U8 *pBuffer0, MS_U8 *pBuffer1, MS_U8 *pBuffer2, MS_U8 *pBuffer3)
{
    MS_U8 pBuffer[4];
    MS_U8 u8Length = 4;

    pBuffer[0] = '0' + *pBuffer0;
    pBuffer[1] = '0' + *pBuffer1;
    pBuffer[2] = '0' + *pBuffer2;
    pBuffer[3] = '0' + *pBuffer3;

    return msAPP_CIMMI_AnswerEnq(pBuffer, u8Length);
}

//------------------------------------------------------------------------------
/// @brief The sample code to exit CIMMI current menu
/// @note
/// Command: \b Command:CIMMI_BackMenu \n
//------------------------------------------------------------------------------
void appMSrv_CIMMI_BackMenu(void)
{
    if(appMSrv_CIMMI_IsGetData())
    {
        if(appMSrv_CIMMI_GetMMIType() == EN_MMI_TYPE_ENQ)
        {
            msAPP_CIMMI_AnswerEnq(NULL, 0);
        }
        else if(appMSrv_CIMMI_GetMMIType() == EN_MMI_TYPE_MENU || appMSrv_CIMMI_GetMMIType() == EN_MMI_TYPE_LIST)
        {
            msAPP_CIMMI_AnswerMenu(0);
        }
    }
}

void appMSrv_CI_CISetByPassMode(MS_BOOL bByPass)
{
    if (bByPass)
        mdrv_gpio_set_high(21);
    else
        mdrv_gpio_set_low(21);
}

void appMSrv_CI_CIProtectionNotify(MS_U8* data, MS_U16 u16ONID, MS_U16 u16TSID, MS_U16 u16SID)
{
    msAPP_CI_SetHSS(data, u16TSID, u16SID);
}

void appMSrv_CI_CIHSSActiveCallBack(void)
{
    if(msAPP_CI_CardDetect())
    {
        printf("msAPP_CI_getHSS=%d.\n", msAPP_CI_GetHSS());
        appMSrv_CI_CISetByPassMode(msAPP_CI_GetHSS());
    }
}

MS_BOOL appMSrv_CI_CISetSystemTime2Cache(void)
{
    static MS_U8 u8FakeUTC[5] = { 0xD7, 0x90, 0x00, 0x00, 0x00 };

    return msAPP_CI_SendDateTimeUTC(u8FakeUTC);
}

//-------------------------------------------------------------------------------------------------
// Demo Functions
//-------------------------------------------------------------------------------------------------

#if (DEMO_CI_PLUS_TEST==1)

//------------------------------------------------------------------------------
/// @brief The sample code to notify CI the HSS service information
/// @param[in] u16TS_ID transport stream ID
/// @param[in] u16ServiceID service ID
/// @note
/// Command: \b Command:CI_HSSNotify \n
//------------------------------------------------------------------------------
MS_BOOL Demo_CI_HSSNotify_ServiceInfo(MS_U32* u32Param0, MS_U32* u32Param1)
{
    if ((u32Param0 == NULL)||(u32Param1 == NULL))
    {
        db_print("INVALID VARIABLE\n");
        return FALSE;

    }

    if (((*u32Param0 >> 16) != 0)||((*u32Param1 >> 16) != 0))
    {
        db_print("INVALID VARIABLE\n");
        return FALSE;
    }


    u16NotifyTS_ID = *u32Param0;
    u16NotifyServiceID = *u32Param1;

    db_print("TS_ID=%d, ServiceID=%d\n", u16NotifyTS_ID, u16NotifyServiceID);
    msAPP_CI_NotifyCurrentServiceInfo(u16NotifyTS_ID, u16NotifyServiceID);

    if(msAPP_CI_CardDetect())
    {
        msAPP_CI_HSSCheck();
    }

    return TRUE;
}


//------------------------------------------------------------------------------
/// @brief The sample code to set CI HSS information
/// @param[in] u16ONID ON ID
/// @param[in] u16TSID transport stream ID
/// @param[in] u16SID service ID
/// @note
/// Command: \b Command:CI_CIProtectionNotify \n
//------------------------------------------------------------------------------
MS_BOOL Demo_CI_CIProtectionNotify(MS_U16* u16ONID, MS_U16* u16TSID, MS_U16* u16SID)
{
    db_print("ON_ID=%d, TS_ID=%d, ServiceID=%d\n", *u16ONID, *u16TSID, *u16SID);
    appMSrv_CI_CIProtectionNotify(NULL, *u16ONID, *u16TSID, *u16SID);

    if(msAPP_CI_CardDetect())
    {
        msAPP_CI_HSSCheck();
    }

    return TRUE;
}

#endif
//------------------------------------------------------------------------------
/// @brief The sample code to init CI and monitor main state
/// @param[in] argc useless, just for system task prototype
/// @param[in] argv useless, just for system task prototype
//------------------------------------------------------------------------------
void Demo_CI_DoMonitor(MS_U32 argc, void *argv)
{
    MS_U8 CI_Plus_Enable = 0;

    db_print("Start CI monitor.\n");
#if (DEMO_CI_PLUS_TEST==1)
    CI_Plus_Enable = 1;
#endif
    msAPP_CI_Init(CI_Plus_Enable);
    db_print("ENABLE_CI_PLUS=%d\n", CI_Plus_Enable);
    db_print("CI/CIMMI initialized.\n");
    msAPP_CI_InstallCallback_CI_Event(_Demo_CI_CIEventCallBack);
    db_print("CI Callback initialized.\n");

    while(1)
    {
        _Demo_CI_CIStateMonitor();
        MsOS_DelayTask(5);
    }
}
//------------------------------------------------------------------------------
/// @brief Demo_CI_Start start CI Monitor Task
/// @param[in] argc useless, just for system task prototype
/// @param[in] argv useless, just for system task prototype
//------------------------------------------------------------------------------
MS_BOOL Demo_CI_Start(void)
{
    MS_S32 NON_CACHE_POOL_ID = INVALID_POOL_ID;
    /// - Create Demo CI Monitor Task
    /// - Allocate Memory


    if (Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE,&NON_CACHE_POOL_ID) == FALSE)
    {
        db_print("Init Failed \n");
        return FALSE;
    }

    _demo_CI_task.pStack = MsOS_AllocateMemory(_demo_CI_task.u32StackSize, NON_CACHE_POOL_ID);
    if(_demo_CI_task.pStack == NULL)
    {
        db_print("Init Failed \n");
        return FALSE;

    }

    /// - Create Task Application
    _demo_CI_task.iId = MsOS_CreateTask( Demo_CI_DoMonitor,
                                          (MS_U32)NULL,
                                            _demo_CI_task.ePriority,
                                            TRUE,
                                            _demo_CI_task.pStack,
                                            _demo_CI_task.u32StackSize,
                                            _demo_CI_task.szName);
    if(_demo_CI_task.iId < 0)
    {
        db_print("Init Failed \n");
        return FALSE;
    }

    return TRUE;
}

//--------------------------------------------------------------------------------------------------
// @brief Demo_CI_MMI_EnterMenu enter menu
// @param[in] None.
// @return TRUE is successful, otherwise FALSE
//--------------------------------------------------------------------------------------------------
MS_BOOL Demo_CI_MMI_EnterMenu(void)
{
   appMSrv_CIMMI_EnterMenu();
   return TRUE;
}
//--------------------------------------------------------------------------------------------------
// @brief Demo_CI_MMI_EnterMenu enter menu
// @param[in] None.
// @return TRUE is successful, otherwise FALSE
//--------------------------------------------------------------------------------------------------
MS_BOOL Demo_CI_MMI_AnwsMenu(MS_U32 *u32index)
{
   MS_U8 _u8Index = 0;

   if (u32index == NULL)
   {
       db_print("INVALID VARIABLE\n");
       return FALSE;
   }

   if ((*u32index >> 8) != 0)
   {
       db_print("INVALID VARIABLE\n");
       return FALSE;
   }
   _u8Index = (MS_U8)(*u32index & 0xFF);
   appMSrv_CIMMI_AnswerMenu(&_u8Index);

   return TRUE;
}
//--------------------------------------------------------------------------------------------------
// @brief Demo_CI_MMI_AnwsEnq to enter PWD to CI
// @param[in] None.
// @return TRUE is successful, otherwise FALSE
//--------------------------------------------------------------------------------------------------
MS_BOOL Demo_CI_MMI_AnwsEnq(MS_U32 *u32Digi0,MS_U32 *u32Digi1,MS_U32 *u32Digi2,MS_U32 *u32Digi3)
{
   MS_U8 _u8digi[4] = {0};

   if ((u32Digi0 == NULL)||(u32Digi1 == NULL)||(u32Digi2 == NULL)||(u32Digi3 == NULL))
   {
       db_print("INVALID VARIABLE\n");
       return FALSE;
   }

   if (((*u32Digi0 >> 8) != 0)||((*u32Digi1 >> 8) != 0)||((*u32Digi2 >> 8) != 0)||((*u32Digi3 >> 8) != 0))
   {
       db_print("INVALID VARIABLE\n");
       return FALSE;
   }
   _u8digi[0] = (MS_U8)(*u32Digi0 & 0xFF);
   _u8digi[1] = (MS_U8)(*u32Digi1 & 0xFF);
   _u8digi[2] = (MS_U8)(*u32Digi2 & 0xFF);
   _u8digi[3] = (MS_U8)(*u32Digi3 & 0xFF);
   appMSrv_CIMMI_answerEnq(&_u8digi[0],&_u8digi[1],&_u8digi[2],&_u8digi[3]);

   return TRUE;
}
//--------------------------------------------------------------------------------------------------
// @brief Demo_CI_MMI_AnwsEnq to enter PWD to CI
// @param[in] None.
// @return TRUE is successful, otherwise FALSE
//--------------------------------------------------------------------------------------------------
MS_BOOL Demo_CI_MMI_BacktoMenu(void)
{
    if(appMSrv_CIMMI_IsGetData())
    {
        if(appMSrv_CIMMI_GetMMIType() == EN_MMI_TYPE_ENQ)
        {
            msAPP_CIMMI_AnswerEnq(NULL, 0);
        }
        else if(appMSrv_CIMMI_GetMMIType() == EN_MMI_TYPE_MENU || appMSrv_CIMMI_GetMMIType() == EN_MMI_TYPE_LIST)
        {
            msAPP_CIMMI_AnswerMenu(0);
        }

        return TRUE;
    }
    else
    {
        return FALSE;
    }

}

#endif

