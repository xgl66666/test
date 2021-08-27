////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2008-2009 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// ("MStar Confidential Information") by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
/// file    auth.c
/// @brief  authentication Function
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include <common.h>
#include <linux/types.h>
#include <secure/crypto_rsa.h>
#include <secure/crypto_func.h>

#include <sys/common/MsTypes.h>
#include <sys/common/MsIRQ.h>
#include <sys/common/MsOS.h>
#include <sys/drv/drvISR.h>
#include <sys/drv/drvMBX.h>
#include <sys/drv/drvSERFLASH.h>
#include <secure/apiSecureBoot.h>
//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
//#define SEC_DUMP_RAW_DATA
#define SEC_INFOR_BIN_ADDR_OFFSET   0x120000
#define SPI_FLASH_BASE_ADDR         0xBFC00000
#define SEC_INFO_BIN_DRAM_ADDR      0x80A00000//0x80A00000
#define SEC_AUTH_BIN_DRAM_ADDR      0x80A01000//0x80A01000
#define SEC_AUTH_KERNEL_DRAM_ADDR   0x81200000//0x80400000
#define SEC_AUTH_KERNEL_DRAM_LEN    0x400000
#define SEC_AUTH_AP1_DRAM_ADDR      0x81600000//0x80400000
#define SEC_AUTH_AP1_DRAM_LEN       0x800000
#define SEC_AUTH_AP2_DRAM_ADDR      0x81E00000//0x80C00000
#define SEC_AUTH_AP2_DRAM_LEN       0x800000

#define RSA_PUBLIC_KEY_LEN          (256+4)
#define SIGNATURE_LEN               256

#ifndef BOOL
typedef unsigned int BOOL;
#define TRUE    1
#define FALSE   0
#endif

#define Supernova   1

//#define RIU      ((unsigned short volatile *) RIU_MAP)
//#define RIU8    ((unsigned char  volatile *) RIU_MAP)
#define BDMA_REG_BASE 0x100900

//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------
typedef struct
{
    U32 u32Addr;
    U32 u32Size: 31;
    U32 bitInFlash: 1;
}IMAGE_INFOR;

#if (Supernova == 1)
typedef struct
{
    U8 u8SecIdentify[8];
    IMAGE_INFOR stLoader;
    IMAGE_INFOR stSensitive;
    //IMAGE_INFOR stApplication;
    U8 u8RSALoaderPublicKey_N[256];
    U8 u8RSALoaderPublicKey_E[4];
    U8 u8SignatureRoot[SIGNATURE_LEN];
    U8 u8SignatureLoader[SIGNATURE_LEN];
    U8 u8SignatureFw[SIGNATURE_LEN];
    //U8 u8SignatureAp[SIGNATURE_LEN];
}SECURITY_INFO;
#else
typedef struct
{
    U8 u8SecIdentify[8];
    IMAGE_INFOR stLoader;
    IMAGE_INFOR stSensitive;
    IMAGE_INFOR stApplication;
    U8 u8RSALoaderPublicKey_N[256];
    U8 u8RSALoaderPublicKey_E[4];
    U8 u8SignatureRoot[SIGNATURE_LEN];
    U8 u8SignatureLoader[SIGNATURE_LEN];
    U8 u8SignatureFw[SIGNATURE_LEN];
    U8 u8SignatureAp[SIGNATURE_LEN];
}SECURITY_INFO;
#endif

//-------------------------------------------------------------------------------------------------
//  Public Variables
//-------------------------------------------------------------------------------------------------
U32 KL_Size = 0;
U32 AP_Size = 0;

//-------------------------------------------------------------------------------------------------
//  External Functions
//-------------------------------------------------------------------------------------------------
extern void Secure_Switch(void);
extern MS_BOOL MApi_MBX_Init(void);

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local functions
//-------------------------------------------------------------------------------------------------
static void Secure_DumpData(U8 *data, U32 len)
{
    char buffer[128];
    memset(buffer, 0 , 128);
    sprintf(buffer, "md.b 0x%x 0x%x", data, len);
    printf("cmd: %s\n", buffer);
    run_command(buffer, 0);
}

static BOOL  Secure_GetBinFromSPI(U32 u32BinAddr, U32 u32BinLen, U8 *sec)
{
    U32 i;

    if (NULL == sec)
    {
        return FALSE;
    }

    printf("[Secure] Get Bin file from SPI: [srcAddr: 0x%x][len:%d][destAddr:0x%x]\n",u32BinAddr,u32BinLen,sec);

   while (RIU8[(BDMA_REG_BASE + 0x02)*2] & 0x01);

    i = ((U32)(u32BinAddr) & ~0xBFC00000);
    RIU[(BDMA_REG_BASE + 0x08 )] = (U32)(i);
    RIU[(BDMA_REG_BASE + 0x0A )] = (U32)(i >> 16);

    i = (((U32)(sec) | 0x20000000) & ~0xA0000000);
    RIU[(BDMA_REG_BASE + 0x0c )] =(U32)(i);
    RIU[(BDMA_REG_BASE + 0x0E )] =(U32)(i >> 16);

    // DmaSize = ((U32)ap_bin_len + 0x0f ) & ~ 0x0ful;
    i = (u32BinLen + 0x0f) & ~0x0f;
    RIU[(BDMA_REG_BASE + 0x10 )]  =(U16)(i);
    RIU[(BDMA_REG_BASE + 0x12 )]  =(U16)(i >> 16);


    RIU8[(BDMA_REG_BASE + 0x06)*2]=0x00;   //increasing
    RIU[(BDMA_REG_BASE+0x04)]= 0x4045;  //((DstDEv|DstDataWidth)  <<8) |(SrcDataWidth|SrcDEv) ;

    RIU8[ (BDMA_REG_BASE*2)]= 0x01 ;

     while( 1 )
     {
         if ( (RIU8[(BDMA_REG_BASE+0x02)*2] & 0x08) == 0x08 )
         {
             break;
         }
     }

    printf("Get Bin SUCCESS\n");
    return TRUE;
}

static BOOL Secure_doAuthentication(U8 *u8PublicKey_N, U8 *u8PublicKey_E, U8 *u8Signature, U8 *u8AuthData, U32 u32AuthDataLen)
{
    char buffer[128];

    memset(buffer, 0 , 128);
    sprintf(buffer, "secure 0x%x 0x%x 0x%x 0x%x %d", u8PublicKey_N, u8PublicKey_E, u8Signature, u8AuthData,u32AuthDataLen);
    printf("cmd: %s\n", buffer);

    if (run_command(buffer, 0)<0)
    {
        printf("***************************\n");
        printf("*  Authentication FAIL !! *\n");
        printf("***************************\n");
        return FALSE;
    }
    else
    {
        printf("*****************************\n");
        printf("* Authentication SUCCESS !! *\n");
        printf("*****************************\n");
        return TRUE;
    }
}

inline static void Secure_HaltCPU(void)
{
    //halt cpu
    __asm__ __volatile__(
        "\tSDBBP");
}

//-------------------------------------------------------------------------------------------------
//  Global functions
//-------------------------------------------------------------------------------------------------
static SECURITY_INFO *sec = (SECURITY_INFO *)SEC_INFO_BIN_DRAM_ADDR;
static U8 u8RSAPublicKeyFw[RSA_PUBLIC_KEY_LEN];
#if (Supernova == 0)
static U8 u8RSAPublicKeyAp[RSA_PUBLIC_KEY_LEN];
#endif
static U8 u8SignatureFw[SIGNATURE_LEN];
#if (Supernova == 0)
static U8 u8SignatureAp[SIGNATURE_LEN];
#endif

void SPI_CallBack(void)
{
    *(volatile unsigned short *)(0xBF200000+(0x3300*2)+(0x6C*4)) = 0x5A5A;
}

void Secure_SPI_Init(void)
{
    SECUREBOOT_MbxRetStruct SecureBootMbxRetStruct;

    // notify 51 switch to 1K PSRAM
    Secure_MailBox_1KPSRAM(&SecureBootMbxRetStruct, 0x0);
    // wait 51 enter 1K PSRAM
    while((*(volatile unsigned short *)(0xBF200000+(0x3300*2)+(0x6B*4))) != 0x5A5A);
    // write signature to external SPI
    MDrv_SERFLASH_SetMcuCSCallBack(SPI_CallBack);
    run_command("spi_in", 0);
    // notify 51 switch to internal SPI
    *(volatile unsigned short *)(0xBF200000+(0x3300*2)+(0x6A*4)) = 0x5A5A;
}

void Secure_Init(void)
{
    U8 *pu8BinAddr = (U8 *)SEC_AUTH_BIN_DRAM_ADDR;

/********************************************************************************
*                  Relocate the SHA funnctions pointer
*********************************************************************************/
#if 0   //remove relocate operation
    mpfunc_relocate();
#endif
/********************************************************************************
*                  Get secure information binary
*********************************************************************************/
    if (FALSE == Secure_GetBinFromSPI(SPI_FLASH_BASE_ADDR+SEC_INFOR_BIN_ADDR_OFFSET, sizeof(SECURITY_INFO), (U8 *)sec))
    {
        printf("Get Secure Information Bin FAIL\n");
        Secure_HaltCPU();
    }
#ifdef SEC_DUMP_RAW_DATA
    Secure_DumpData((U8 *)sec, sizeof(SECURITY_INFO));
#endif

#if (Supernova == 1)
    // copy signature_fw to local variable array
    memcpy(u8SignatureFw, sec->u8SignatureFw, SIGNATURE_LEN);
#else
    // copy signature_fw to local variable array
    memcpy(u8SignatureFw, sec->u8SignatureFw, SIGNATURE_LEN);
    memcpy(u8SignatureAp, sec->u8SignatureAp, SIGNATURE_LEN);
#endif
#ifdef SEC_DUMP_RAW_DATA
    Secure_DumpData(u8SignatureFw, SIGNATURE_LEN);
    Secure_DumpData(u8SignatureAp, SIGNATURE_LEN);
#endif

/********************************************************************************
*                  Get RSAPublicKeyFw
*********************************************************************************/
    if (FALSE == Secure_GetBinFromSPI(sec->stLoader.u32Addr, sec->stLoader.u32Size, pu8BinAddr))
    {
        printf("Get RSAPublicKeyFw FAIL\n");
        Secure_HaltCPU();
    }
#if (Supernova == 1)
    // copy to local variable array
    memcpy(u8RSAPublicKeyFw, &pu8BinAddr[sec->stLoader.u32Size-RSA_PUBLIC_KEY_LEN], RSA_PUBLIC_KEY_LEN);
#else
    // copy to local variable array
    memcpy(u8RSAPublicKeyFw, &pu8BinAddr[sec->stLoader.u32Size-RSA_PUBLIC_KEY_LEN*2], RSA_PUBLIC_KEY_LEN);
    memcpy(u8RSAPublicKeyAp, &pu8BinAddr[sec->stLoader.u32Size-RSA_PUBLIC_KEY_LEN], RSA_PUBLIC_KEY_LEN);
#endif
#ifdef SEC_DUMP_RAW_DATA
    Secure_DumpData(u8RSAPublicKeyFw, RSA_PUBLIC_KEY_LEN);
    Secure_DumpData(u8RSAPublicKeyAp, RSA_PUBLIC_KEY_LEN);
#endif

/********************************************************************************
*                  Initialize the MailBox
*********************************************************************************/
    Secure_Switch();
    printf("Switch to internal SPI for AES decode\n");

    printf("MApi_MBX_Init\n");
    MApi_MBX_Init();
    printf("Secure_MailBox_Init\n");
    Secure_MailBox_Init();
}

void Secure_GetSSKey(void)
{
    SECUREBOOT_MbxRetStruct SecureBootMbxRetStruct;
    U32 u32KernelAddr = 0xA0100000;

    printf("Start to GetKey...\n");

    {
        U8 i;
        for(i=0;i<0x20;i++)
            *((U8 *)u32KernelAddr + i) = i;
    }

    Secure_DumpData((U8 *)u32KernelAddr, 0x20);
    printf("Secure_MailBox_AESEncode\n");
    Chip_Flush_Memory();
    Secure_MailBox_AESEncode(&SecureBootMbxRetStruct, u32KernelAddr, 0x20, (BIT1|BIT4));
    Chip_Read_Memory();
    Secure_DumpData((U8 *)u32KernelAddr, 0x20);

    printf("Secure_MailBox_AESDecode\n");
    Chip_Flush_Memory();
    Secure_MailBox_AESDecode(&SecureBootMbxRetStruct, u32KernelAddr, 0x20, (BIT1|BIT4));
    Chip_Read_Memory();
    Secure_DumpData((U8 *)u32KernelAddr, 0x20);

}

void Secure_Kernel_Authentication(U32 u32kernelAddr)
{
    SECUREBOOT_MbxRetStruct SecureBootMbxRetStruct;
    U32 time=0;

    printf("Start to Kernel authentication...\n");
/********************************************************************************
*                  AES Decode Kernel image
*********************************************************************************/

    printf("Secure_MailBox_AESDecode\n");
    Chip_Flush_Memory();
    Secure_MailBox_AESDecode(&SecureBootMbxRetStruct, u32kernelAddr, sec->stSensitive.u32Size, (BIT0|BIT3));
    //printf("@@ AESDecode: u32Pm51ToCpuAddr = 0x%08x\n", (MS_U32)(SecureBootMbxRetStruct.u32Pm51ToCpuAddr));
    //printf("@@ AESDecode: u32Length = 0x%08x\n", (MS_U32)(SecureBootMbxRetStruct.u32Length));

/********************************************************************************
*                   Authentication to kernel
*********************************************************************************/

    time=get_ticks();

    Chip_Read_Memory();
    if (FALSE == Secure_doAuthentication(u8RSAPublicKeyFw, &u8RSAPublicKeyFw[256], u8SignatureFw, (U8 *)u32kernelAddr, sec->stSensitive.u32Size))
    {
        printf("Auth. Kernel FAIL\n");
        Secure_DumpData((U8 *)u32kernelAddr, 0x100);
        Secure_HaltCPU();
    }

    printf("Kernel Time Period: %d s\n", (get_ticks()-time)/(CFG_HZ / 1000000));

}

void Secure_Application_Authentication(void)
{
#if (Supernova == 0)
    U8 *pu8BinAddr = (U8 *)SEC_AUTH_BIN_DRAM_ADDR;
    char buffer[256];
    SECUREBOOT_MbxRetStruct SecureBootMbxRetStruct;
    U32 time=0;

    printf("Start to Application authentication...\n");
/********************************************************************************
*                  Get Application data from NAND Flash
*********************************************************************************/

    memset(buffer, 0 , 256);
    sprintf(buffer, "ubi part UBI;ubifsmount CK;ubifsls;ubifsload 0x%x AP.aes 0x%x", (((U32)pu8BinAddr)|0x20000000), sec->stApplication.u32Size);
    //sprintf(buffer, "ubi part UBI;ubifsmount CK;ubifsls;ubifsload 0x%x AP.aes 0x%x", pu8BinAddr, sec->stApplication.u32Size);
    printf("cmd: %s\n", buffer);
    run_command(buffer, 0);

/********************************************************************************
*                  AES Decode Application image
*********************************************************************************/

    printf("Secure_MailBox_AESDecode\n");
    Chip_Flush_Memory();
    Secure_MailBox_AESDecode(&SecureBootMbxRetStruct, (U32)pu8BinAddr, sec->stApplication.u32Size, (BIT0|BIT3));
    //printf("@@ AESDecode: u32Pm51ToCpuAddr = 0x%08x\n", (MS_U32)(SecureBootMbxRetStruct.u32Pm51ToCpuAddr));
    //printf("@@ AESDecode: u32Length = 0x%08x\n", (MS_U32)(SecureBootMbxRetStruct.u32Length));

/********************************************************************************
*                   Authentication to Application
*********************************************************************************/

    time=get_ticks();

    Chip_Read_Memory();
    if (FALSE == Secure_doAuthentication(u8RSAPublicKeyAp, &u8RSAPublicKeyAp[256], u8SignatureAp, pu8BinAddr, sec->stApplication.u32Size))
    {
        printf("Auth. Application FAIL\n");
        Secure_DumpData((U8 *)pu8BinAddr, 0x100);
        Secure_HaltCPU();
    }

    printf("Kernel Time Period: %d s\n", (get_ticks()-time)/(CFG_HZ / 1000000));

#endif
}

void Secure_USB_KL_Upgrade(void)
{
    U8 u8SignatureKL[SIGNATURE_LEN];
    char buffer[256];
    U8 *pu8KernelAddr = (U8 *)SEC_AUTH_KERNEL_DRAM_ADDR;
    SECUREBOOT_MbxRetStruct SecureBootMbxRetStruct;

    printf("*****************************\n");
    printf("*  Secure USB KL upgrade !! *\n");
    printf("*****************************\n");

#ifdef SEC_DUMP_RAW_DATA
    Secure_DumpData(u8RSAPublicKeyFw, RSA_PUBLIC_KEY_LEN);
#endif

    // Get FW signature, Get FW image and do authentication
    memset(buffer, 0 , 256);
    sprintf(buffer, "fatls usb 0 /FwUpgrade;fatload usb 0 0x%x /FwUpgrade/t_signature_fw.bin;fatload usb 0 0x%x /FwUpgrade/uImage.aes", (U32)SEC_AUTH_BIN_DRAM_ADDR, (U32)pu8KernelAddr);
    printf("cmd: %s\n", buffer);
    run_command(buffer, 0);

    memcpy(u8SignatureKL, (U8 *)SEC_AUTH_BIN_DRAM_ADDR, SIGNATURE_LEN);
#ifdef SEC_DUMP_RAW_DATA
    Secure_DumpData(u8SignatureKL, SIGNATURE_LEN);
#endif

    printf("Secure_MailBox_AESDecode\n");
    Chip_Flush_Memory();
    Secure_MailBox_AESDecode(&SecureBootMbxRetStruct, (U32)pu8KernelAddr, KL_Size, (BIT0|BIT3));
    //printf("@@ AESDecode: u32Pm51ToCpuAddr = 0x%08x\n", (MS_U32)(SecureBootMbxRetStruct.u32Pm51ToCpuAddr));
    //printf("@@ AESDecode: u32Length = 0x%08x\n", (MS_U32)(SecureBootMbxRetStruct.u32Length));

    Chip_Read_Memory();
    if (FALSE == Secure_doAuthentication(u8RSAPublicKeyFw, &u8RSAPublicKeyFw[256], u8SignatureKL, pu8KernelAddr, KL_Size))
    {
        memset(buffer, 0 , 256);
        sprintf(buffer, "usb start 0;fatwrite usb 0 0x%x KER_USB.lzo 0x%x", pu8KernelAddr, KL_Size);
        printf("cmd: %s\n", buffer);
        run_command(buffer, 0);
        Secure_HaltCPU();
    }
    else
    {
        SECUREBOOT_MbxRetStruct SecureBootMbxRetStruct;
        memset(buffer, 0 , 256);
        sprintf(buffer, "fatload usb 0 0x%x /FwUpgrade/uImage.aes;nand erase KL;nand write.e 0x%x KL 0x%x", (U32)pu8KernelAddr, (U32)pu8KernelAddr, (U32)SEC_AUTH_KERNEL_DRAM_LEN);
        printf("cmd: %s\n", buffer);
        run_command(buffer, 0);

        // notify 51 switch to 1K PSRAM
        Secure_MailBox_1KPSRAM(&SecureBootMbxRetStruct, 0x0);
        // wait 51 enter 1K PSRAM
        while((*(volatile unsigned short *)(0xBF200000+(0x3300*2)+(0x6B*4))) != 0x5A5A);
        // write signature to external SPI
        memset(buffer, 0 , 256);
        sprintf(buffer, "spi_rdc 0x%x 0x%x 0x%x", (U32)sec, (U32)SEC_INFOR_BIN_ADDR_OFFSET, 0x10000);
        printf("cmd: %s\n", buffer);
        run_command(buffer, 0);
        memcpy(sec->u8SignatureFw, u8SignatureKL, SIGNATURE_LEN);
        memset(buffer, 0 , 256);
        sprintf(buffer, "spi_wrc 0x%x 0x%x 0x%x", (U32)sec, (U32)SEC_INFOR_BIN_ADDR_OFFSET, 0x10000);
        printf("cmd: %s\n", buffer);
        run_command(buffer, 0);
        // notify 51 switch to internal SPI
        *(volatile unsigned short *)(0xBF200000+(0x3300*2)+(0x6A*4)) = 0x5A5A;
    }
}

void Secure_USB_CK_Upgrade(void)
{
#if (Supernova == 0)
    U8 u8SignatureCK[SIGNATURE_LEN];
    char buffer[256];
    SECUREBOOT_MbxRetStruct SecureBootMbxRetStruct;

    printf("*****************************\n");
    printf("*  Secure USB CK upgrade !! *\n");
    printf("*****************************\n");

#ifdef SEC_DUMP_RAW_DATA
    Secure_DumpData(u8RSAPublicKeyAp, RSA_PUBLIC_KEY_LEN);
#endif

    // Get FW signature
    memset(buffer, 0 , 256);
    sprintf(buffer, "fatload usb 0 0x%x /FwUpgrade/t_signature_ap.bin", (U32)SEC_AUTH_BIN_DRAM_ADDR);
    printf("cmd: %s\n", buffer);
    run_command(buffer, 0);

    memcpy(u8SignatureCK, (U8 *)SEC_AUTH_BIN_DRAM_ADDR, SIGNATURE_LEN);
#ifdef SEC_DUMP_RAW_DATA
    Secure_DumpData(u8SignatureCK, SIGNATURE_LEN);
#endif

    // read original CK in NAND
    memset(buffer, 0 , 256);
    sprintf(buffer, "ubi part UBI;ubi read 0x%x CK 0x%x", (U32)SEC_AUTH_AP1_DRAM_ADDR, (U32)SEC_AUTH_AP1_DRAM_LEN);
    printf("cmd: %s\n", buffer);
    run_command(buffer, 0);

    // write chakra.ubifs into NAND and mount as CK
    memset(buffer, 0 , 256);
    sprintf(buffer, "fatload usb 0 0x%x /FwUpgrade/chakra.ubifs;ubi write 0x%x CK 0x%x;ubifsmount CK;ubifsls;ubifsload 0x%x AP.aes 0x%x", (U32)SEC_AUTH_AP2_DRAM_ADDR, (U32)SEC_AUTH_AP2_DRAM_ADDR, (U32)SEC_AUTH_AP2_DRAM_LEN, (U32)(SEC_AUTH_BIN_DRAM_ADDR|0x20000000), AP_Size);
    printf("cmd: %s\n", buffer);
    run_command(buffer, 0);

    printf("Secure_MailBox_AESDecode\n");
    Chip_Flush_Memory();
    Secure_MailBox_AESDecode(&SecureBootMbxRetStruct, (U32)SEC_AUTH_BIN_DRAM_ADDR, AP_Size, (BIT0|BIT3));
    //printf("@@ AESDecode: u32Pm51ToCpuAddr = 0x%08x\n", (MS_U32)(SecureBootMbxRetStruct.u32Pm51ToCpuAddr));
    //printf("@@ AESDecode: u32Length = 0x%08x\n", (MS_U32)(SecureBootMbxRetStruct.u32Length));

    Chip_Read_Memory();
    if (FALSE == Secure_doAuthentication(u8RSAPublicKeyAp, &u8RSAPublicKeyAp[256], u8SignatureCK, (U8 *)SEC_AUTH_BIN_DRAM_ADDR, AP_Size))
    {
#if 0
        memset(buffer, 0 , 256);
        sprintf(buffer, "usb start 0;fatwrite usb 0 0x%x AP_USB.elf 0x%x", (U8 *)SEC_AUTH_BIN_DRAM_ADDR, AP_Size);
        printf("cmd: %s\n", buffer);
        run_command(buffer, 0);
#endif
        memset(buffer, 0 , 256);
        sprintf(buffer, "ubi part UBI;ubi write 0x%x CK 0x%x", (U32)SEC_AUTH_AP1_DRAM_ADDR, (U32)SEC_AUTH_AP1_DRAM_LEN);
        printf("cmd: %s\n", buffer);
        run_command(buffer, 0);

        //Secure_HaltCPU();
    }

    // notify 51 switch to 1K PSRAM
    Secure_MailBox_1KPSRAM(&SecureBootMbxRetStruct, 0x0);
    // wait 51 enter 1K PSRAM
    while((*(volatile unsigned short *)(0xBF200000+(0x3300*2)+(0x6B*4))) != 0x5A5A);
    // write signature to external SPI
    memset(buffer, 0 , 256);
    sprintf(buffer, "spi_rd 0x%x 0x%x 0x%x", (U32)sec, (U32)SEC_INFOR_BIN_ADDR_OFFSET, 0x10000);
    printf("cmd: %s\n", buffer);
    run_command(buffer, 0);
    memcpy(sec->u8SignatureAp, u8SignatureCK, SIGNATURE_LEN);
    memset(buffer, 0 , 256);
    sprintf(buffer, "spi_wrc 0x%x 0x%x 0x%x", (U32)sec, (U32)SEC_INFOR_BIN_ADDR_OFFSET, 0x10000);
    printf("cmd: %s\n", buffer);
    run_command(buffer, 0);
    // notify 51 switch to internal SPI
    *(volatile unsigned short *)(0xBF200000+(0x3300*2)+(0x6A*4)) = 0x5A5A;
#endif
}

int Secure_NET_KL_Upgrade(void)
{
    U8 u8SignatureKL[SIGNATURE_LEN];
    char buffer[256];
    U8 *pu8KernelAddr = (U8 *)SEC_AUTH_KERNEL_DRAM_ADDR;
    SECUREBOOT_MbxRetStruct SecureBootMbxRetStruct;

    printf("*****************************\n");
    printf("*  Secure NET KL upgrade !! *\n");
    printf("*****************************\n");

#ifdef SEC_DUMP_RAW_DATA
    Secure_DumpData(u8RSAPublicKeyFw, RSA_PUBLIC_KEY_LEN);
#endif

    run_command("estart", 0);

    // Get FW signature
    memset(buffer, 0 , 256);
    sprintf(buffer, "tftp 0x%x /FwUpgrade/t_signature_fw.bin", (U32)SEC_AUTH_BIN_DRAM_ADDR);
    printf("cmd: %s\n", buffer);
    run_command(buffer, 0);
    memcpy(u8SignatureKL, (U8 *)SEC_AUTH_BIN_DRAM_ADDR, SIGNATURE_LEN);
#ifdef SEC_DUMP_RAW_DATA
    Secure_DumpData(u8SignatureKL, SIGNATURE_LEN);
#endif

    // Get FW image and do authentication
    memset(buffer, 0 , 256);
    sprintf(buffer, "tftp 0x%x /FwUpgrade/uImage.aes", (U32)pu8KernelAddr);
    printf("cmd: %s\n", buffer);
    run_command(buffer, 0);

    printf("Secure_MailBox_AESDecode\n");
    Chip_Flush_Memory();
    Secure_MailBox_AESDecode(&SecureBootMbxRetStruct, (U32)pu8KernelAddr, KL_Size, (BIT0|BIT3));
    //printf("@@ AESDecode: u32Pm51ToCpuAddr = 0x%08x\n", (MS_U32)(SecureBootMbxRetStruct.u32Pm51ToCpuAddr));
    //printf("@@ AESDecode: u32Length = 0x%08x\n", (MS_U32)(SecureBootMbxRetStruct.u32Length));

    Chip_Read_Memory();
    if (FALSE == Secure_doAuthentication(u8RSAPublicKeyFw, &u8RSAPublicKeyFw[256], u8SignatureKL, pu8KernelAddr, KL_Size))
    {
        memset(buffer, 0 , 256);
        sprintf(buffer, "usb start 0;fatwrite usb 0 0x%x KER_NET.lzo 0x%x", pu8KernelAddr, KL_Size);
        printf("cmd: %s\n", buffer);
        run_command(buffer, 0);
        //Secure_HaltCPU();
        return -1;
    }
    else
    {
        memset(buffer, 0 , 256);
        sprintf(buffer, "tftp 0x%x /FwUpgrade/uImage.aes;ubi part UBI;ubi write 0x%x KL 0x%x", (U32)pu8KernelAddr, (U32)pu8KernelAddr, (U32)SEC_AUTH_KERNEL_DRAM_LEN);
        printf("cmd: %s\n", buffer);
        run_command(buffer, 0);

        // notify 51 switch to 1K PSRAM
        Secure_MailBox_1KPSRAM(&SecureBootMbxRetStruct, 0x0);
        // wait 51 enter 1K PSRAM
        while((*(volatile unsigned short *)(0xBF200000+(0x3300*2)+(0x6B*4))) != 0x5A5A);
        // write signature to external SPI
        memset(buffer, 0 , 256);
        sprintf(buffer, "spi_rd 0x%x 0x%x 0x%x", (U32)sec, (U32)SEC_INFOR_BIN_ADDR_OFFSET, 0x10000);
        printf("cmd: %s\n", buffer);
        run_command(buffer, 0);
        memcpy(sec->u8SignatureFw, u8SignatureKL, SIGNATURE_LEN);
        memset(buffer, 0 , 256);
        sprintf(buffer, "spi_wrc 0x%x 0x%x 0x%x", (U32)sec, (U32)SEC_INFOR_BIN_ADDR_OFFSET, 0x10000);
        printf("cmd: %s\n", buffer);
        run_command(buffer, 0);
        // notify 51 switch to internal SPI
        *(volatile unsigned short *)(0xBF200000+(0x3300*2)+(0x6A*4)) = 0x5A5A;

        return 0;
    }
}

int Secure_NET_CK_Upgrade(void)
{
#if (Supernova == 0)
    U8 u8SignatureCK[SIGNATURE_LEN];
    char buffer[256];
    SECUREBOOT_MbxRetStruct SecureBootMbxRetStruct;

    printf("*****************************\n");
    printf("*  Secure NET CK upgrade !! *\n");
    printf("*****************************\n");

#ifdef SEC_DUMP_RAW_DATA
    Secure_DumpData(u8RSAPublicKeyAp, RSA_PUBLIC_KEY_LEN);
#endif

    run_command("estart", 0);
    // Get FW signature
    memset(buffer, 0 , 256);
    sprintf(buffer, "tftp 0x%x /FwUpgrade/t_signature_ap.bin", (U32)SEC_AUTH_BIN_DRAM_ADDR);
    printf("cmd: %s\n", buffer);
    run_command(buffer, 0);

    memcpy(u8SignatureCK, (U8 *)SEC_AUTH_BIN_DRAM_ADDR, SIGNATURE_LEN);
#ifdef SEC_DUMP_RAW_DATA
    Secure_DumpData(u8SignatureCK, SIGNATURE_LEN);
#endif

    // read original CK in NAND to DRAM
    memset(buffer, 0 , 256);
    sprintf(buffer, "ubi part UBI;ubi read 0x%x CK 0x%x", (U32)SEC_AUTH_AP1_DRAM_ADDR, (U32)SEC_AUTH_AP1_DRAM_LEN);
    printf("cmd: %s\n", buffer);
    run_command(buffer, 0);

    // write chakra.ubifs into NAND and mount as CK
    memset(buffer, 0 , 256);
    sprintf(buffer, "tftp 0x%x /FwUpgrade/chakra.ubifs;ubi write 0x%x CK 0x%x;ubifsmount CK;ubifsls;ubifsload 0x%x AP.aes 0x%x", (U32)SEC_AUTH_AP2_DRAM_ADDR, (U32)SEC_AUTH_AP2_DRAM_ADDR, (U32)SEC_AUTH_AP2_DRAM_LEN, (U32)(SEC_AUTH_BIN_DRAM_ADDR|0x20000000), AP_Size);
    printf("cmd: %s\n", buffer);
    run_command(buffer, 0);

    printf("Secure_MailBox_AESDecode\n");
    Chip_Flush_Memory();
    Secure_MailBox_AESDecode(&SecureBootMbxRetStruct, SEC_AUTH_BIN_DRAM_ADDR, AP_Size, (BIT0|BIT3));
    //printf("@@ AESDecode: u32Pm51ToCpuAddr = 0x%08x\n", (MS_U32)(SecureBootMbxRetStruct.u32Pm51ToCpuAddr));
    //printf("@@ AESDecode: u32Length = 0x%08x\n", (MS_U32)(SecureBootMbxRetStruct.u32Length));

    Chip_Read_Memory();
    if (FALSE == Secure_doAuthentication(u8RSAPublicKeyAp, &u8RSAPublicKeyAp[256], u8SignatureCK, (U8 *)SEC_AUTH_BIN_DRAM_ADDR, AP_Size))
    {
        memset(buffer, 0 , 256);
        sprintf(buffer, "ubi part UBI;ubi write 0x%x CK 0x%x", (U32)SEC_AUTH_AP1_DRAM_ADDR, (U32)SEC_AUTH_AP1_DRAM_LEN);
        printf("cmd: %s\n", buffer);
        run_command(buffer, 0);

        //Secure_HaltCPU();
        return -1;
    }
    else
    {
        // notify 51 switch to 1K PSRAM
        Secure_MailBox_1KPSRAM(&SecureBootMbxRetStruct, 0x0);
        // wait 51 enter 1K PSRAM
        while((*(volatile unsigned short *)(0xBF200000+(0x3300*2)+(0x6B*4))) != 0x5A5A);
        // write signature to external SPI
        memset(buffer, 0 , 256);
        sprintf(buffer, "spi_rd 0x%x 0x%x 0x%x", (U32)sec, (U32)SEC_INFOR_BIN_ADDR_OFFSET, 0x10000);
        printf("cmd: %s\n", buffer);
        run_command(buffer, 0);
        memcpy(sec->u8SignatureAp, u8SignatureCK, SIGNATURE_LEN);
        memset(buffer, 0 , 256);
        sprintf(buffer, "spi_wrc 0x%x 0x%x 0x%x", (U32)sec, (U32)SEC_INFOR_BIN_ADDR_OFFSET, 0x10000);
        printf("cmd: %s\n", buffer);
        run_command(buffer, 0);
        // notify 51 switch to internal SPI
        *(volatile unsigned short *)(0xBF200000+(0x3300*2)+(0x6A*4)) = 0x5A5A;

        return 0;
    }
#endif

    return 0;
}

