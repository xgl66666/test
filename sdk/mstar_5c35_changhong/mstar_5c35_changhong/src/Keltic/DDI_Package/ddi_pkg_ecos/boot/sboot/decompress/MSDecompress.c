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

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
typedef unsigned char   U8;
typedef unsigned int   U16;
typedef unsigned long  U32;

#define NULL 0

#include "zlib.h"
#include "ms_decompress_priv.h"
#include "version.h"

// the following address has a major problem. we dont fix it!
#if defined(__mips__)
#define AP_IN_FLASH_ADDR    0x94040000      //This address should be 4' alignment
#elif defined(__arm__)
#define AP_IN_FLASH_ADDR    0x14020000      //This address should be 4' alignment
#endif
#define __loader_2nd
#define __loader_2nd_bss

#define PUTC(c) do { *(volatile unsigned short *)0xBF201300 = c; } while(0)

unsigned char __loader_2nd_bss *pDecompressionBuf = NULL;
unsigned char __loader_2nd_bss *pEndofDecompressionBuf = NULL;
unsigned char __loader_2nd_bss *pCurrentBufPtr = NULL;

void MemoryAllocatorInit(unsigned char *ptr, unsigned long nSize);
void *zalloc(void *ptr, unsigned long nNum, unsigned long nSize);
void zfree(void *x, void *address, unsigned nbytes);
U8 MsDecompress(U8 *pSrc, U8 *pDst, U32 srclen, U32 dstlen, U32 *pDecompressionLength);
void DoMSDecompression(U32 addr);
void init_version(void);

void __loader_2nd init_version(void)
{
    //Init version
    static U8 tmpVar;
    tmpVar = _sbt_version.APP.tag[0];
}

void __loader_2nd MemoryAllocatorInit(unsigned char *ptr, unsigned long nSize)
{
    pDecompressionBuf = ptr;
    pEndofDecompressionBuf = pDecompressionBuf + nSize*sizeof(unsigned char);
    pCurrentBufPtr = pDecompressionBuf;
}

void __loader_2nd *zalloc(void *ptr, unsigned long nNum, unsigned long nSize)
{
    // Because C language use call by value
    // so if we modify ptr value, it will not afftect the caller value
    // Considering decompression performance, we don't do memory pool
    // region check here
#if 0
    nSize = ( nSize*nNum + 3 ) & ~3;

    ptr = (void *)pCurrentBufPtr;
    pCurrentBufPtr += nSize;

    if (pCurrentBufPtr > pEndofDecompressionBuf)
    {
        printf("Memory Allocate Fail\n");
        ptr = NULL;
    }
#else
    ptr = (void *)pCurrentBufPtr;
    // 4' alignment
    pCurrentBufPtr += ( nSize*nNum + 3 ) & ~3;
#endif
    return ptr;
}

void __loader_2nd zfree(void *x, void *address, unsigned nbytes)
{
    // Considering decompression performance, we don't
    // do memory free operation here
    // This will cause a lot of memory usage, but I think we can
    // afford this, because our target files are not so big
    x = x;
    address = address;
    nbytes = nbytes;
}

U8 __loader_2nd MsDecompress(U8 *pSrc, U8 *pDst, U32 srclen, U32 dstlen, U32 *pDecompressionLength)
{
    z_stream z;

    z.zalloc = (alloc_func)zalloc;
    z.zfree = zfree;
    z.next_in = pSrc;
    z.avail_in = srclen;
    z.next_out = pDst;
    z.avail_out = dstlen;

    if ( inflateInit2(&z, -MAX_WBITS) != Z_OK )
    {
        //printf("Z Error:%s\n", z.msg);
        inflateEnd(&z);
        return 1;
    }

    if ( inflate(&z, Z_FINISH) < Z_OK )
    {
        //printf("Z Error:%s\n", z.msg);
        inflateEnd(&z);
        return 1;
    }

    *pDecompressionLength = z.total_out;

    inflateEnd(&z);

    return 0;
}

void __loader_2nd DoMSDecompression(U32 addr)
{
    U8 IsCompressMagicNumber = 0;
    U32 CompressedFileLength = 0;
    U32 OriginalFileLength = 0;
    U32 nOutputLength;

    //Compressed Format
    U32 u32ROM_START = addr + 0x100;                               // This address should be 4' alignment
    U32 u32ROM_END = addr + *((U32 *)(addr + 0x8));    // This address should be 4' alignment
    U32 i = 0;

    U32 u32RAM_START, u32RAM_END;

    //Origin Format
    u32RAM_START = *((U32 *)(addr)) + 0x100;
    u32RAM_END = u32RAM_START+ *((U32 *)(addr + 0xC));

    CompressedFileLength = (u32ROM_END - u32ROM_START);
    OriginalFileLength = (u32RAM_END - u32RAM_START);

    if ((CompressedFileLength != OriginalFileLength) && ((OriginalFileLength-CompressedFileLength-0x100)>32)) // It implys that U-Boot is compressed.
    {
        //Copy compressed UBoot from SPI flash to RAM
        for(i = 0; i < ((CompressedFileLength + 7 + 0x100) & ~7UL); i += 4)
        {
            *((U32 *)(DECOMPRESS_BUF_CACHEABLE + i)) = *((U32 *)(u32ROM_START - 0x100 + i));
        }

        if(!LDR_APP_Auth((U8 *)DECOMPRESS_BUF_CACHEABLE, *(U32 *)(addr + 0x8)))
        {
            PUTC('!');
            while(1);
        }

        // Start to decompress
        MemoryAllocatorInit((U8 *)MEMORY_POOL_CACHEABLE, MEMORY_POOL_SIZE);
        MsDecompress((U8 *)DECOMPRESS_BUF_CACHEABLE + 0x100, (U8 *)u32RAM_START, CompressedFileLength, OriginalFileLength, &nOutputLength);

        if ( nOutputLength != OriginalFileLength )
        {
            //Decompression fail! Jump to boot loader to recover FW
            //while(1);
        }
    }
    else //It implys that U-Boot is not compressed.
    {
        U32              *src, *dst, *end;

        src = (U32*)addr;
        dst = (U32*)(*(src));
        end = (U32*)((U32)src + *(src+3));

        // Uncompressed format
        while (src < end)
        {
            *(dst)   = *(src);
            *(dst+1) = *(src+1);
            *(dst+2) = *(src+2);
            *(dst+3) = *(src+3);
            src += 4;
            dst += 4;
        }
    }

    init_version();
}

