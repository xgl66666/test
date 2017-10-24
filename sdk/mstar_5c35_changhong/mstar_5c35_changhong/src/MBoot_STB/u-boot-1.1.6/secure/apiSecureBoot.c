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

////////////////////////////////////////////////////////////////////////////////
//
/// @file apiSecureBoot.h
/// This file includes MStar application interface for Secure Boot
/// @brief API for Secure Boot
/// @author MStar Semiconductor, Inc.
//
////////////////////////////////////////////////////////////////////////////////

#define API_SECUREBOOT_C

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include <common.h>

#include <linux/string.h> //for memset()

#include <secure/apiSecureBoot.h>
#include <sys/drv/drvMBX.h>

//-------------------------------------------------------------------------------------------------
//  Local Compiler Options
//-------------------------------------------------------------------------------------------------
#define SB_API_DBG(x)   x

#if (CONFIG_COMMANDS & CFG_CMD_SECURE)
//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define SECUREBOOT_MBX_QUEUESIZE        8
#define SECUREBOOT_MBX_TIMEOUT          5000

#define RIU_TMPBASE 0xBF200000  //for test
#define CMEM(addr)         (*((volatile unsigned char*)(addr)))

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
static SECUREBOOT_Result gSecureBootResult;
static SECUREBOOT_AckFlags gSecureBootAckFlags;
static MBX_Result gMBXResult;

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------

static void _SecureBoot_MailBoxHandler(MBX_Class eMsgClass, SECUREBOOT_MbxRetStruct *pSecureBootRetStruct)
{
    MBX_Msg MB_Command;

    if(eMsgClass != E_MBX_CLASS_SECUREBOOT_WAIT)
    {
        //printf("Invalid Message Class!\n");
        return;
    }

    gMBXResult = MDrv_MBX_RecvMsg(eMsgClass, &MB_Command, SECUREBOOT_MBX_TIMEOUT/*ms*/, MBX_CHECK_NORMAL_MSG);

    if (gMBXResult == E_MBX_ERR_TIME_OUT)
    {
        printf("Handler Timeout!\n");
        return;
    }

    if (gMBXResult == E_MBX_SUCCESS)
    {
        if ((MB_Command.u8Ctrl != 0) && (MB_Command.u8Ctrl != 1))
        {
            gSecureBootResult = E_SECUREBOOT_FAIL;
            printf("Not Implemented!\n");
            return;
        }

        {
            MS_U8 i;
            printf("Get SECURE command: 0x%02x, 0x%02x\n", MB_Command.u8Index, MB_Command.u8ParameterCount);
            for(i=0; i<MB_Command.u8ParameterCount; i++)
            {
                printf("Parameter[%d]=0x%02x\n", i, MB_Command.u8Parameters[i]);
            }
        }

       gSecureBootResult = E_SECUREBOOT_FAIL;
        switch (MB_Command.u8Index)
        {
            case E_SECUREBOOT_51ToCPU_CMD_ACK_GETKEY:
                gSecureBootResult = E_SECUREBOOT_OK;
                pSecureBootRetStruct->u32Pm51ToCpuAddr = (  \
                    (((MS_U32)(MB_Command.u8Parameters[0]))<<24) | \
                    (((MS_U32)(MB_Command.u8Parameters[1]))<<16) | \
                    (((MS_U32)(MB_Command.u8Parameters[2]))<<8) | \
                    ((MS_U32)(MB_Command.u8Parameters[3]))
                );
                pSecureBootRetStruct->u32Length = (  \
                    (((MS_U32)(MB_Command.u8Parameters[4]))<<24) | \
                    (((MS_U32)(MB_Command.u8Parameters[5]))<<16) | \
                    (((MS_U32)(MB_Command.u8Parameters[6]))<<8) | \
                    ((MS_U32)(MB_Command.u8Parameters[7]))
                );
                pSecureBootRetStruct->u8Status = (MB_Command.u8Parameters[8]);
                gSecureBootAckFlags &= (SECUREBOOT_AckFlags)(~E_SECUREBOOT_ACKFLG_WAIT_GETKEY);
                break;

            case E_SECUREBOOT_51ToCPU_CMD_ACK_GETKEYDONE:
                gSecureBootResult = E_SECUREBOOT_OK;
                pSecureBootRetStruct->u8Status = (MB_Command.u8Parameters[8]);
                gSecureBootAckFlags &= (SECUREBOOT_AckFlags)(~E_SECUREBOOT_ACKFLG_WAIT_GETKEYDONE);
                break;

            case E_SECUREBOOT_51ToCPU_CMD_ACK_AUTHDONE:
                gSecureBootResult = E_SECUREBOOT_OK;
                pSecureBootRetStruct->u8Status = (MB_Command.u8Parameters[8]);
                gSecureBootAckFlags &= (SECUREBOOT_AckFlags)(~E_SECUREBOOT_ACKFLG_WAIT_AUTHDONE);
                break;

            case E_SECUREBOOT_51ToCPU_CMD_ACK_AES_ENCODE:
                gSecureBootResult = E_SECUREBOOT_OK;
                pSecureBootRetStruct->u32Pm51ToCpuAddr = (  \
                    (((MS_U32)(MB_Command.u8Parameters[0]))<<24) | \
                    (((MS_U32)(MB_Command.u8Parameters[1]))<<16) | \
                    (((MS_U32)(MB_Command.u8Parameters[2]))<<8) | \
                    ((MS_U32)(MB_Command.u8Parameters[3]))
                );

                pSecureBootRetStruct->u32Length = (  \
                    (((MS_U32)(MB_Command.u8Parameters[4]))<<24) | \
                    (((MS_U32)(MB_Command.u8Parameters[5]))<<16) | \
                    (((MS_U32)(MB_Command.u8Parameters[6]))<<8) | \
                    ((MS_U32)(MB_Command.u8Parameters[7]))
                );

                pSecureBootRetStruct->u8Status = (MB_Command.u8Parameters[8]);
                gSecureBootAckFlags &= (SECUREBOOT_AckFlags)(~E_SECUREBOOT_ACKFLG_WAIT_AESENCODE);
                break;

                case E_SECUREBOOT_51ToCPU_CMD_ACK_AES_DECODE:
                gSecureBootResult = E_SECUREBOOT_OK;
                pSecureBootRetStruct->u32Pm51ToCpuAddr = (  \
                    (((MS_U32)(MB_Command.u8Parameters[0]))<<24) | \
                    (((MS_U32)(MB_Command.u8Parameters[1]))<<16) | \
                    (((MS_U32)(MB_Command.u8Parameters[2]))<<8) | \
                    ((MS_U32)(MB_Command.u8Parameters[3]))
                );

                pSecureBootRetStruct->u32Length = (  \
                    (((MS_U32)(MB_Command.u8Parameters[4]))<<24) | \
                    (((MS_U32)(MB_Command.u8Parameters[5]))<<16) | \
                    (((MS_U32)(MB_Command.u8Parameters[6]))<<8) | \
                    ((MS_U32)(MB_Command.u8Parameters[7]))
                );

                pSecureBootRetStruct->u8Status = (MB_Command.u8Parameters[8]);
                gSecureBootAckFlags &= (SECUREBOOT_AckFlags)(~E_SECUREBOOT_ACKFLG_WAIT_AESDECODE);
                break;

                case E_SECUREBOOT_51ToCPU_CMD_ACK_1KPSRAM:
                gSecureBootResult = E_SECUREBOOT_OK;
                pSecureBootRetStruct->u8Status = (MB_Command.u8Parameters[8]);
                gSecureBootAckFlags &= (SECUREBOOT_AckFlags)(~E_SECUREBOOT_ACKFLG_WAIT_1KPSRAM);
                break;

            default:
                break;
        }
    }

    return;
}

static void _SecureBoot_CheckFlashID1_SwitchDone(void)
{
    while( 0x5A5A != (*(volatile unsigned short *)(RIU_TMPBASE+(0x3300*2)+(0x68*4))));
}

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
/******************************************************************************/
/// API for Secure Boot init Mailbox:
/// Secure_MailBox_Init
/// @param \b IN:
/// @param \b IN:
/// @return BOOLEAN: Success or not
/// - TRUE: Success
/// - FALSE: Fail
/******************************************************************************/

MS_BOOL Secure_MailBox_Init(void)
{
    if( E_MBX_SUCCESS != MDrv_MBX_RegisterMSG(E_MBX_CLASS_SECUREBOOT_WAIT, SECUREBOOT_MBX_QUEUESIZE))
    {
        printf("@@ E_MBX_CLASS_SECURE_WAIT - MBX register msg error\n");
    }

    return TRUE;
}

MS_BOOL Secure_MailBox_Deinit(void)
{
    if( E_MBX_SUCCESS != MDrv_MBX_UnRegisterMSG(E_MBX_CLASS_SECUREBOOT_WAIT, TRUE))
    {
        printf("@@ E_MBX_CLASS_SECURE_WAIT - MBX unregister msg error\n");
    }

    return TRUE;
}

SECUREBOOT_Result Secure_MailBox_GetKey(SECUREBOOT_MbxRetStruct *pSecureBootMbxRetStruct)
{
    MBX_Msg MB_Command;

    memset((void*)&MB_Command, 0, sizeof(MBX_Msg));
    gSecureBootAckFlags |= E_SECUREBOOT_ACKFLG_WAIT_GETKEY;

    //(1) send to PM51
    MB_Command.eRoleID = E_MBX_ROLE_PM;
    MB_Command.eMsgType = E_MBX_MSG_TYPE_INSTANT;
    MB_Command.u8Ctrl = 0;
    MB_Command.u8MsgClass = E_MBX_CLASS_SECUREBOOT_NOWAIT;
    MB_Command.u8Index = E_SECUREBOOT_CPUTo51_CMD_GETKEY;
    MB_Command.u8ParameterCount = 0;

    MDrv_MBX_SendMsg(&MB_Command);

    //(2) Waiting for SECURE BOOT message done
    do
    {
        _SecureBoot_MailBoxHandler(E_MBX_CLASS_SECUREBOOT_WAIT, pSecureBootMbxRetStruct);
    }
    while((gSecureBootAckFlags & E_SECUREBOOT_ACKFLG_WAIT_GETKEY) && (gMBXResult != E_MBX_ERR_TIME_OUT));

    //(3) check result
    if(gSecureBootResult == E_SECUREBOOT_FAIL)
    {
        printf("Fail\n");
        while(1);
    }
    else
    {
        printf("OK\n");
    }

    return gSecureBootResult;

}

SECUREBOOT_Result Secure_MailBox_KeyDone(void)
{
    MBX_Msg MB_Command;

    memset((void*)&MB_Command, 0, sizeof(MBX_Msg));
    gSecureBootAckFlags |= E_SECUREBOOT_ACKFLG_WAIT_GETKEYDONE;

    //(1) send to PM51
    MB_Command.eRoleID = E_MBX_ROLE_PM;
    MB_Command.eMsgType = E_MBX_MSG_TYPE_INSTANT;
    MB_Command.u8Ctrl = 0;
    MB_Command.u8MsgClass = E_MBX_CLASS_SECUREBOOT_NOWAIT;
    MB_Command.u8Index = E_SECUREBOOT_CPUTo51_CMD_GETKEYDONE;
    MB_Command.u8ParameterCount = 0;

    MDrv_MBX_SendMsg(&MB_Command);

    //(2) Waiting for SECURE BOOT message done
    do
    {
        _SecureBoot_MailBoxHandler(E_MBX_CLASS_SECUREBOOT_WAIT, NULL);
    }
    while((gSecureBootAckFlags & E_SECUREBOOT_ACKFLG_WAIT_GETKEYDONE) && (gMBXResult != E_MBX_ERR_TIME_OUT));

    //(3) check result
    if(gSecureBootResult == E_SECUREBOOT_FAIL)
    {
        //printf("Fail\n");
        while(1);
    }
    else
    {
        //printf("OK\n");
        //check swith to Flash ID1 done
        _SecureBoot_CheckFlashID1_SwitchDone();
    }

    return gSecureBootResult;
}

SECUREBOOT_Result Secure_MailBox_AESEncode(SECUREBOOT_MbxRetStruct *pSecureBootMbxRetStruct, U32 u32Addr, U32 u32Len, U8 u8Mode)
{
    MBX_Msg MB_Command;

    memset((void*)&MB_Command, 0, sizeof(MBX_Msg));
    gSecureBootAckFlags |= E_SECUREBOOT_ACKFLG_WAIT_AESENCODE;

    //(1) send to PM51
    MB_Command.eRoleID = E_MBX_ROLE_PM;
    MB_Command.eMsgType = E_MBX_MSG_TYPE_INSTANT;
    MB_Command.u8Ctrl = 0;
    MB_Command.u8MsgClass = E_MBX_CLASS_SECUREBOOT_NOWAIT;
    MB_Command.u8Index = E_SECUREBOOT_CPUTo51_CMD_AES_ENCODE;
    MB_Command.u8ParameterCount = 9;
    MB_Command.u8Parameters[0] =  (U8)(u32Addr>>24);
    MB_Command.u8Parameters[1] =  (U8)(u32Addr>>16);
    MB_Command.u8Parameters[2] =  (U8)(u32Addr>>8);
    MB_Command.u8Parameters[3] =  (U8)(u32Addr&0x000000FF);

    MB_Command.u8Parameters[4] =  (U8)(u32Len>>24);
    MB_Command.u8Parameters[5] =  (U8)(u32Len>>16);
    MB_Command.u8Parameters[6] =  (U8)(u32Len>>8);
    MB_Command.u8Parameters[7] =  (U8)(u32Len&0x000000FF);

    MB_Command.u8Parameters[8] =  u8Mode;

    MDrv_MBX_SendMsg(&MB_Command);

    //(2) Waiting for SECURE BOOT message done
    do
    {
        _SecureBoot_MailBoxHandler(E_MBX_CLASS_SECUREBOOT_WAIT, pSecureBootMbxRetStruct);
    }
    while((gSecureBootAckFlags & E_SECUREBOOT_ACKFLG_WAIT_AESENCODE) && (gMBXResult != E_MBX_ERR_TIME_OUT));


    //(3) check result
    if(gSecureBootResult == E_SECUREBOOT_FAIL)
    {
        printf("Secure_MailBox_AESEncode Fail\n");
        //while(1);
    }
    else
    {
        printf("Secure_MailBox_AESEncode OK\n");
    }

    return gSecureBootResult;
}

SECUREBOOT_Result Secure_MailBox_AESDecode(SECUREBOOT_MbxRetStruct *pSecureBootMbxRetStruct, U32 u32Addr, U32 u32Len, U8 u8Mode)
{
    MBX_Msg MB_Command;

    memset((void*)&MB_Command, 0, sizeof(MBX_Msg));
    gSecureBootAckFlags |= E_SECUREBOOT_ACKFLG_WAIT_AESDECODE;

    //(1) send to PM51
    MB_Command.eRoleID = E_MBX_ROLE_PM;
    MB_Command.eMsgType = E_MBX_MSG_TYPE_INSTANT;
    MB_Command.u8Ctrl = 0;
    MB_Command.u8MsgClass = E_MBX_CLASS_SECUREBOOT_NOWAIT;
    MB_Command.u8Index = E_SECUREBOOT_CPUTo51_CMD_AES_DECODE;
    MB_Command.u8ParameterCount = 9;
    MB_Command.u8Parameters[0] =  (U8)(u32Addr>>24);
    MB_Command.u8Parameters[1] =  (U8)(u32Addr>>16);
    MB_Command.u8Parameters[2] =  (U8)(u32Addr>>8);
    MB_Command.u8Parameters[3] =  (U8)(u32Addr&0x000000FF);

    MB_Command.u8Parameters[4] =  (U8)(u32Len>>24);
    MB_Command.u8Parameters[5] =  (U8)(u32Len>>16);
    MB_Command.u8Parameters[6] =  (U8)(u32Len>>8);
    MB_Command.u8Parameters[7] =  (U8)(u32Len&0x000000FF);

    MB_Command.u8Parameters[8] =  u8Mode;

    MDrv_MBX_SendMsg(&MB_Command);

    //(2) Waiting for SECURE BOOT message done
    do
    {
        _SecureBoot_MailBoxHandler(E_MBX_CLASS_SECUREBOOT_WAIT, pSecureBootMbxRetStruct);
    }
    while((gSecureBootAckFlags & E_SECUREBOOT_ACKFLG_WAIT_AESDECODE) && (gMBXResult != E_MBX_ERR_TIME_OUT));


    //(3) check result
    if(gSecureBootResult == E_SECUREBOOT_FAIL)
    {
        printf("Secure_MailBox_AESDecode Fail\n");
        //while(1);
    }
    else
    {
        printf("Secure_MailBox_AESDecode OK\n");
    }

    return gSecureBootResult;

}

SECUREBOOT_Result Secure_MailBox_1KPSRAM(SECUREBOOT_MbxRetStruct *pSecureBootMbxRetStruct, U8 u8Mode)
{
    MBX_Msg MB_Command;

    memset((void*)&MB_Command, 0, sizeof(MBX_Msg));
    gSecureBootAckFlags |= E_SECUREBOOT_ACKFLG_WAIT_1KPSRAM;

    //(1) send to PM51
    MB_Command.eRoleID = E_MBX_ROLE_PM;
    MB_Command.eMsgType = E_MBX_MSG_TYPE_INSTANT;
    MB_Command.u8Ctrl = 0;
    MB_Command.u8MsgClass = E_MBX_CLASS_SECUREBOOT_NOWAIT;
    MB_Command.u8Index = E_SECUREBOOT_CPUTo51_CMD_1KPSRAM;
    MB_Command.u8ParameterCount = 1;

    MB_Command.u8Parameters[0] =  u8Mode;

    MDrv_MBX_SendMsg(&MB_Command);

    //(2) Waiting for SECURE BOOT message done
    do
    {
        _SecureBoot_MailBoxHandler(E_MBX_CLASS_SECUREBOOT_WAIT, NULL);
    }
    while((gSecureBootAckFlags & E_SECUREBOOT_ACKFLG_WAIT_1KPSRAM) && (gMBXResult != E_MBX_ERR_TIME_OUT));

    //(3) check result
    if(gSecureBootResult == E_SECUREBOOT_FAIL)
    {
        printf("MApi_Secure_MailBox_1KPSRAM Fail\n");
        //while(1);
    }
    else
    {
        printf("MApi_Secure_MailBox_1KPSRAM OK\n");
    }

    return gSecureBootResult;
}
#endif

#if (defined(CONFIG_TITANIA8) || defined(CONFIG_TITANIA12) || defined(CONFIG_AMBER2) || defined(CONFIG_TITANIA4) || defined(CONFIG_TITANIA13))
#define AES_BUF_LEN     0x1000
#define ONE_SEC         1000
#define BIT(bits)       (1<<bits)
#define SECURE_DEBUG    0
MS_U32 difftime = 0;
static DRVAESDMA_RESULT CommonAES128Encrypt(AES_ParamStruct *AESPara)
{
    DRVAESDMA_RESULT result = DRVAESDMA_OK;
    MS_U32 u32AESInBuf, u32AESOutBuf;

    if((AESPara->pChiperBuf == NULL)||(AESPara->pPlainBuf == NULL)||(AESPara->pKey == NULL))
    {
        return DRVAESDMA_INVALID_PARAM;
    }

    flush_cache(AESPara->pPlainBuf, AESPara->u32Len);
#if (SECURE_DEBUG)
{
    char buffer[256];
    U32 u32NonCacheAddr, u32CacheAddr;
    u32NonCacheAddr = AESPara->pPlainBuf | 0xA0000000;
    u32CacheAddr = AESPara->pPlainBuf | 0x80000000;
    printf("\n");
    memset(buffer, 0 , 256);
    sprintf(buffer, "md.b 0x%x 0x%x", u32NonCacheAddr, AESPara->u32Len);
    run_command(buffer, 0);
    memset(buffer, 0 , 256);
    sprintf(buffer, "md.b 0x%x 0x%x", u32CacheAddr, AESPara->u32Len);
    run_command(buffer, 0);
}
#endif
    u32AESInBuf = AESPara->pPlainBuf & (~0xA0000000);
    u32AESOutBuf = AESPara->pChiperBuf & (~0xA0000000);

    MDrv_AESDMA_Reset();
    MDrv_AESDMA_SelEng(AESPara->eMode, FALSE);
    //printf("AESPara->u32Len = %d\n",AESPara->u32Len);
    //result = MDrv_AESDMA_SetFileInOut(u32AESInBuf, AESPara->u32Len, u32AESOutBuf, 0xFFFFFFFF);
    result = MDrv_AESDMA_SetFileInOut(u32AESInBuf, AESPara->u32Len, u32AESOutBuf, u32AESOutBuf+AESPara->u32Len);
    if (result == DRVAESDMA_MIU_ADDR_ERROR)
    {
        return DRVAESDMA_FAIL;
    }

    if(AESPara->eMode == E_DRVAESDMA_CIPHER_CBC)
    {
        MDrv_AESDMA_SetIV(AESPara->pIV);
    }

    MDrv_AESDMA_SetKey(AESPara->pKey);

    {
        MS_U32 time=0;
        time=get_ticks();

        MDrv_AESDMA_Start(TRUE);

        while ((MDrv_AESDMA_GetStatus() & BIT(16)) == 0);

        difftime=(get_ticks()-time)/((1000 * (CFG_HZ / 1000000)));
        printf("AES encode performance\n");
    }

    flush_cache(AESPara->pChiperBuf, AESPara->u32Len);
#if (SECURE_DEBUG)
{
    char buffer[256];
    U32 u32NonCacheAddr, u32CacheAddr;
    u32NonCacheAddr = AESPara->pPlainBuf | 0xA0000000;
    u32CacheAddr = AESPara->pPlainBuf | 0x80000000;
    printf("\n");
    memset(buffer, 0 , 256);
    sprintf(buffer, "md.b 0x%x 0x%x", u32NonCacheAddr, AESPara->u32Len);
    run_command(buffer, 0);
    memset(buffer, 0 , 256);
    sprintf(buffer, "md.b 0x%x 0x%x", u32CacheAddr, AESPara->u32Len);
    run_command(buffer, 0);
}
#endif

    return DRVAESDMA_OK;
}

static DRVAESDMA_RESULT CommonAES128Decrypt(AES_ParamStruct *AESPara)
{

    DRVAESDMA_RESULT result = DRVAESDMA_OK;
    MS_U32 u32AESInBuf, u32AESOutBuf;

    if((AESPara->pChiperBuf == NULL)||(AESPara->pPlainBuf == NULL)||(AESPara->pKey == NULL))
    {
        return DRVAESDMA_INVALID_PARAM;
    }

    flush_cache(AESPara->pChiperBuf, AESPara->u32Len);
#if (SECURE_DEBUG)
{
    char buffer[256];
    U32 u32NonCacheAddr, u32CacheAddr;
    u32NonCacheAddr = AESPara->pPlainBuf | 0xA0000000;
    u32CacheAddr = AESPara->pPlainBuf | 0x80000000;
    printf("\n");
    memset(buffer, 0 , 256);
    sprintf(buffer, "md.b 0x%x 0x%x", u32NonCacheAddr, AESPara->u32Len);
    run_command(buffer, 0);
    memset(buffer, 0 , 256);
    sprintf(buffer, "md.b 0x%x 0x%x", u32CacheAddr, AESPara->u32Len);
    run_command(buffer, 0);
}
#endif
    u32AESInBuf = AESPara->pChiperBuf & (~0xA0000000);
    u32AESOutBuf = AESPara->pPlainBuf & (~0xA0000000);

    MDrv_AESDMA_Reset();
    MDrv_AESDMA_SelEng(AESPara->eMode, TRUE);
    result=MDrv_AESDMA_SetFileInOut(u32AESInBuf, AESPara->u32Len, u32AESOutBuf, 0xFFFFFFFF);
    if (result == DRVAESDMA_MIU_ADDR_ERROR)
    {
        return DRVAESDMA_FAIL;
    }

    if(AESPara->eMode == E_DRVAESDMA_CIPHER_CBC)
    {
        MDrv_AESDMA_SetIV(AESPara->pIV);
    }

    MDrv_AESDMA_SetKey(AESPara->pKey);
    {
        MS_U32 time=0;
        time=get_ticks();

        MDrv_AESDMA_Start(TRUE);

        while ((MDrv_AESDMA_GetStatus() & BIT(16)) == 0);

        difftime=(get_ticks()-time)/((1000 * (CFG_HZ / 1000000)));
        printf("AES decode performance\n");
    }

    flush_cache(AESPara->pPlainBuf, AESPara->u32Len);
#if (SECURE_DEBUG)
{
    char buffer[256];
    U32 u32NonCacheAddr, u32CacheAddr;
    u32NonCacheAddr = AESPara->pPlainBuf | 0xA0000000;
    u32CacheAddr = AESPara->pPlainBuf | 0x80000000;
    printf("\n");
    memset(buffer, 0 , 256);
    sprintf(buffer, "md.b 0x%x 0x%x", u32NonCacheAddr, AESPara->u32Len);
    run_command(buffer, 0);
    memset(buffer, 0 , 256);
    sprintf(buffer, "md.b 0x%x 0x%x", u32CacheAddr, AESPara->u32Len);
    run_command(buffer, 0);
}
#endif

    return DRVAESDMA_OK;
}

DRVAESDMA_RESULT CommonSHA(MS_U32 u32InBuf, MS_U32 u32OutBuf, MS_U32 u32Len)
{
    DRVAESDMA_RESULT result = DRVAESDMA_OK;
    MS_U32 u32SHAInBuf, u32SHAOutBuf;

    flush_cache(u32InBuf, u32Len);

    u32SHAInBuf = u32InBuf & (~0xA0000000);
    u32SHAOutBuf = u32OutBuf & (~0xA0000000);

    MDrv_AESDMA_Init(0x00, 0x00, 1);
    {
        MS_U32 time=0;
        time=get_ticks();

        MDrv_SHA_Calculate(E_DRVAESDMA_SHA256, u32SHAInBuf, u32Len, u32SHAOutBuf);

        difftime=(get_ticks()-time)/((1000 * (CFG_HZ / 1000000)));
        printf("SHA performance %08d ms\n",difftime);
    }

    flush_cache(u32OutBuf, u32Len);

    return result;
}

U32 Secure_AES_Performance(U32 u32Len,  DrvAESDMA_CipherMode eMode, MS_BOOL bEnc)
{
    unsigned char u8Key[0x10] = {0x12, 0x34, 0x56, 0x78, 0x12, 0x34, 0x56, 0x78, 0x12, 0x34, 0x56, 0x78, 0x12, 0x34, 0x56, 0x78};
    //unsigned char u8Key[0x10] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
    unsigned char u8IV[0x10] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
    AES_ParamStruct AESParam;
    //int i;
    //unsigned char u8tempArray[0x200] = {0};

    AESParam.eMode = eMode;
    AESParam.pChiperBuf = 0x83000000;
    AESParam.pPlainBuf = 0x83000000;
    AESParam.pKey = (MS_U32 *)u8Key;
    AESParam.u32Len = u32Len;
    AESParam.pIV= (MS_U32 *)u8IV;

    memset((U8*)0x83000000, 0x00, u32Len);

    MDrv_AESDMA_Init(0x00, 0x00, 1);
    printf("AES performance test\n");

#if 0
    for(i=0;i<u32Len;i++)
    {
        if(i%16==0)
            printf("\n");
        printf("%d ", ((U8*)AESParam.pPlainBuf)[i]);
    }
    printf("\n");
#endif

    if(bEnc==1)
    {
        CommonAES128Encrypt( &AESParam);
    }
    else
    {
        CommonAES128Decrypt( &AESParam);
    }
#if 0
    for(i=0;i<u32Len;i++)
    {
        if(i%16==0)
            printf("\n");
        printf("%d ", ((U8*)AESParam.pChiperBuf)[i]);
    }
    printf("\n");
#endif

    printf("AES done\n");
    return difftime;

}

void Secure_Kernel_AES_Decrypt(U32 u32Addr, U32 u32Len)
{
    unsigned char u8Key[0x10];// = {0x12, 0x34, 0x56, 0x78, 0x12, 0x34, 0x56, 0x78, 0x12, 0x34, 0x56, 0x78, 0x12, 0x34, 0x56, 0x78};
    AES_ParamStruct AESParam;

    run_command("getmasterkey", 0);

#if defined(CONFIG_TITANIA3) && (CONFIG_COMMANDS & CFG_CMD_STANDBY)
    memcpy(&u8Key[0], &u8masterkey[0], 0x10);
#endif
    {
        U8 i=0;
        for(i=0;i<0x10;i++)
            printf("0x%02X ", u8Key[i]);
        printf("\n");
    }
    AESParam.eMode = E_DRVAESDMA_CIPHER_ECB;
    AESParam.pChiperBuf = u32Addr;
    AESParam.pPlainBuf = u32Addr;
    AESParam.pKey = (MS_U32 *)u8Key;
    AESParam.u32Len = u32Len;

    MDrv_AESDMA_Init(0x00, 0x00, 1);

    printf("Common_AESDecode\n");
    CommonAES128Decrypt( &AESParam);
}

void Secure_Kernel_AES_Encrypt(U32 u32Addr, U32 u32Len)
{
    unsigned char u8Key[0x10] = {0x12, 0x34, 0x56, 0x78, 0x12, 0x34, 0x56, 0x78, 0x12, 0x34, 0x56, 0x78, 0x12, 0x34, 0x56, 0x78};
    AES_ParamStruct AESParam;

    AESParam.eMode = E_DRVAESDMA_CIPHER_ECB;
    AESParam.pChiperBuf = u32Addr;
    AESParam.pPlainBuf = u32Addr;
    AESParam.pKey = (MS_U32 *)u8Key;
    AESParam.u32Len = u32Len;

    MDrv_AESDMA_Init(0x00, 0x00, 1);

    printf("Common_AESEncode\n");
    CommonAES128Encrypt( &AESParam);
}
#endif
#undef API_SECUREBOOT_C

