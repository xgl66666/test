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

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    loader.c
/// @brief  Loader code running on RAM
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "boot.h"
#include "chip.h"
#include "drvPadConf.h"
#include "drvNAND.h"
//#include "bootrom_nand.h"
#include "bootrom_spi.h"

extern MS_U8 _ld_APP_load_start[];
extern MS_U8 _boot_flow_mode;

static NAND_NECESS_INFO LDR_nf_info   _SECTION_LDR_STACK_;

typedef void (*PF_AP_Entry)(void);

#define VERIFICATION_ENABLE 1

#define PADCFG_END        0xFFFF
#define PADCFG_MEM_BASE   0x0000
#define REG_ADDR_BASE     0x1F000000

#define SHA_HASH_DIGI_LEN           8                                   // 32-bit count, 5 for SHA-1, 8 for SHA-256,384,512
void SHA256_Init(MS_U32 *hash);
void SHA256_Hash(const MS_U32 *data, MS_U32 *hash);
void SHA256_Hash_Message(MS_U8 *pu8Message, MS_U32 u32Length);

// SHA256 result
MS_U32   sha_H[SHA_HASH_DIGI_LEN] = {
    0xDEADBEEF, 0xDEADBEEF, 0xDEADBEEF, 0xDEADBEEF, 0xDEADBEEF, 0xDEADBEEF, 0xDEADBEEF, 0xDEADBEEF,
};

// Golden APP SHA256 digest
// Here is the digest of the test.bin for testing
// Will be replaced when append the new APP by 'app_append.py'
MS_U32 AppDigest[SHA_HASH_DIGI_LEN] = {
    0x73F911A1, 0x8C9E84EC, 0xE56D24D1, 0xF01547A4, 0x7B3A0D67, 0x283679F0, 0x9FF9AE14, 0xFFF3D95C
};

//#define PUTC(c) do { *(volatile unsigned short *)0x1F201300 = c; } while(0)
void PUTC(int c)
{
    while((RREG16(REG_UART_LSR) & UART_LSR_THRE) != UART_LSR_THRE)
        ;
    WREG16(REG_UART_TX, c);
    while((RREG16(REG_UART_LSR) & (UART_LSR_TEMT | UART_LSR_THRE)) != (UART_LSR_TEMT | UART_LSR_THRE))
        ;
}

#define ENABLE_PRNT_IN_LDR 0
#if ENABLE_PRNT_IN_LDR
#define LDR_PUTC(val) PUTC(val)

void LDR_PUTX(char val)
{
    if (val > 0x9)
        LDR_PUTC(0x41+(val-10));
    else
        LDR_PUTC(0x30+val);
}
void LDR_PUTW(MS_U16 val)
{
    MS_U8 value;

    value = (val>>12)&0xF;
    LDR_PUTX(value);
    value = (val>>8)&0xF;
    LDR_PUTX(value);
    value = (val>>4)&0xF;
    LDR_PUTX(value);
    value = (val)&0xF;
    LDR_PUTX(value);
}

void LDR_PUTDW(MS_U32 val)
{
    MS_U8 value;

    value = (val>>28)&0xF;
    LDR_PUTX(value);
    value = (val>>24)&0xF;
    LDR_PUTX(value);
    value = (val>>20)&0xF;
    LDR_PUTX(value);
    value = (val>>16)&0xF;
    LDR_PUTX(value);
    value = (val>>12)&0xF;
    LDR_PUTX(value);
    value = (val>>8)&0xF;
    LDR_PUTX(value);
    value = (val>>4)&0xF;
    LDR_PUTX(value);
    value = (val)&0xF;
    LDR_PUTX(value);
}

void LDR_PUTS(const char *s)
{
    while(*s) {
        if(*s == '\n')
            LDR_PUTC('\r');
        LDR_PUTC(*s++);
    }
}
#else
#define LDR_PUTC(val)
#define LDR_PUTX(val)
#define LDR_PUTW(val)
#define LDR_PUTDW(val)
#define LDR_PUTS(val)
#endif


void LDR_PadConfig(void)
{
    U8  u8Length;
    U8* pu8PadConfig = (U8*)(padInitTbl+10);
    U16 u16Index; // register index
    U32 u32MapBase = REG_ADDR_BASE;
    volatile U8* u8Addr;

    while (1)
    {
        u16Index = (U16)(((U16)(*pu8PadConfig)<<8) + *(pu8PadConfig+1));
        switch (u16Index)
        {
            case PADCFG_END: //Check end of table
                return;

            case PADCFG_MEM_BASE: //Switch base address
                u32MapBase = REG_ADDR_BASE + ((U32)(*(pu8PadConfig+2))<<17);
                pu8PadConfig += 4;
                break;

            default: // Write reg
                u8Length = u16Index>>14;
                u16Index &= 0x3FFF;
                pu8PadConfig += 2;
                while (1)
                {
                    U8 u8Mask  = *(pu8PadConfig+1);
                    U8 u8Value = *pu8PadConfig;
                    u8Addr = (U8*)(u32MapBase + (((U32)(u16Index))<<1) - (u16Index & 1));                    

                    *u8Addr = (*u8Addr & ~u8Mask) | (u8Value & u8Mask);

                    pu8PadConfig += 2;                    
                    if (u8Length == 0)
                    {
                        break;
                    }
                    else // Write more than 1 byte
                    {
                        u8Length--;
                        u16Index++;
                    }
                }
                break;
        }
    }
}
//#ifndef bfn

MS_U32 LDR_APP_Auth(MS_U8 *u8From, MS_U32 u32Size)
{
    PUTC('_');
#if defined(VERIFICATION_ENABLE) && VERIFICATION_ENABLE
    int i;
    SHA256_Hash_Message(u8From, u32Size);
    for(i = 0; i < SHA_HASH_DIGI_LEN; i++)
    {
        if(sha_H[i] != AppDigest[i])
            return 0;
    }

#endif

    return 1;
}
extern void core1_loop(void);

void enable_cache(void)
{
    void enable_cache_asm(void);
    asm volatile(
            "bl enable_cache_asm\n"
            "nop\n" : : :
            "r0", "r1", "r2", "r3", "r4", "r5",
            "r6", "r7", "r8", "r9", "r10",
            "r12", "r13", "r14", "15", 
            "memory");
}

void flush_cache(void)
{
    void flush_cache_asm(void);
    asm volatile(
            "bl flush_cache_asm\n"
            "nop\n" : : :
            "r0", "r1", "r2", "r3", "r4", "r5",
            "r6", "r7", "r8", "r9", "r10",
            "r12", "r13", "r14", "15",
            "memory");
}

inline MS_U32 rotrFixed(MS_U32 x, unsigned int y)
{
    return((x>>y) | (x<<(sizeof(MS_U32)*8-y)));
}

static MS_U32 SHA256_K[64] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
    0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
    0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
    0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
    0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
    0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
    0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
    0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
    0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

#define blk0(i)                     (W[i] = data[i])
#define blk2(i)                     (W[i&15] += s1(W[(i-2)&15])+W[(i-7)&15]+s0(W[(i-15)&15]))

#define Ch(x,y,z)                   (z^(x&(y^z)))
#define Maj(x,y,z)                  ((x&y)|(z&(x|y)))

#define a(i)                        T[(0-i)&7]
#define b(i)                        T[(1-i)&7]
#define c(i)                        T[(2-i)&7]
#define d(i)                        T[(3-i)&7]
#define e(i)                        T[(4-i)&7]
#define f(i)                        T[(5-i)&7]
#define g(i)                        T[(6-i)&7]
#define h(i)                        T[(7-i)&7]

// for SHA256
#define S0(x)                       (rotrFixed(x,2)^rotrFixed(x,13)^rotrFixed(x,22))
#define S1(x)                       (rotrFixed(x,6)^rotrFixed(x,11)^rotrFixed(x,25))
#define s0(x)                       (rotrFixed(x,7)^rotrFixed(x,18)^(x>>3))
#define s1(x)                       (rotrFixed(x,17)^rotrFixed(x,19)^(x>>10))

#define R(i)                        h(i) += S1(e(i))+Ch(e(i),f(i),g(i))+SHA256_K[i+j]+(j?blk2(i):blk0(i));  \
                                    d(i) += h(i);                                                           \
                                    h(i) += S0(a(i)) + Maj(a(i),b(i),c(i))

void SHA256_Init(MS_U32 *hash)
{
    hash[0] = 0x6a09e667;
    hash[1] = 0xbb67ae85;
    hash[2] = 0x3c6ef372;
    hash[3] = 0xa54ff53a;
    hash[4] = 0x510e527f;
    hash[5] = 0x9b05688c;
    hash[6] = 0x1f83d9ab;
    hash[7] = 0x5be0cd19;
}


void SHA256_Hash(const MS_U32 *data, MS_U32 *hash)
{
    MS_U32                 W[16];
    MS_U32                 T[8];
    int                 j;

    // Initialize state and local variable
    for (j = 0; j < 16; j++)
    {
        W[j] = 0;
    }
    for (j = 0; j < 8; j++)
    {
        T[j] = hash[j];
    }

    // Interative Hash Computation
    for (j = 0; j < 64; j += 16)
    {
        /* R( 0)
        h(0) += S1(e(0))+Ch(e(0),f(0),g(0))+SHA256_K[0+j]+(j?blk2(0):blk0(0));
        d(0) += h(0);
        h(0) += S0(a(0)) + Maj(a(0),b(0),c(0));
        */
        R( 0); R( 1); R( 2); R( 3);
        R( 4); R( 5); R( 6); R( 7);
        R( 8); R( 9); R(10); R(11);
        R(12); R(13); R(14); R(15);
    }

    // Update new hash value
    hash[0] += a(0);
    hash[1] += b(0);
    hash[2] += c(0);
    hash[3] += d(0);
    hash[4] += e(0);
    hash[5] += f(0);
    hash[6] += g(0);
    hash[7] += h(0);
}

void SHA256_Hash_Message(MS_U8 *pu8Message, MS_U32 u32Length)
{
    MS_U32              sha_M[SHA_HASH_DIGI_LEN * 2]; // temp buffer for message block

    int                 len, shift, i;
    unsigned char       msg[4];
    MS_U32              msg_addr;
    MS_U32              tmp;

    SHA256_Init(sha_H);

    for(i = 0; i < sizeof(sha_M) / sizeof(sha_M[0]); i++)
        sha_M[i] = 0;

    msg_addr = (MS_U32)(pu8Message);
    len = (int)(u32Length);

    while (len >= 0)
    {
        if (len < 64) // last 1 data block
        {
            // move n*32 bits data
            for (i = 0; i < (len&~3);)
            {
                *(MS_U32*)msg = *(MS_U32*)(msg_addr);
                msg_addr += 4;

                sha_M[i/4] = (msg[0] << 24) | (msg[1] << 16) | (msg[2] << 8) | msg[3];
                i += 4;
            }

            // move non-32 bits data
            *(MS_U32*)msg = *(MS_U32*)(msg_addr);
            msg_addr += 4;

            // append and stuffing
            sha_M[i/4] = 0;
            while (i < len)
            {
                sha_M[i/4] |= msg[i%4] << (24-((i%4)*8));
                i++;
            }
            // 1 append
            sha_M[i/4] |= 0x80 << (24-((i%4)*8));
            i = (i + 4) & ~3;


            // If (len * 8 + 1(1 append) + 64(64-bit msg length)) > 512,
            // it's not enough for padding 8-byte(64-bit) length;
            // namely, if len=56~63.
            if (len >= 56)
            {
                // No room for message length

                // 0 append
                for (; i < 64;)
                {
                    sha_M[i/4] = 0;
                    i += 4;
                }
                SHA256_Hash(sha_M, sha_H);
                // reset to next block
                i = 0;
            }
            // 0 append
            for (; i < 56;)
            {
                sha_M[i/4] = 0;
                i += 4;
            }

            sha_M[14] = 0;
            //sha_M[15] = (*(U32*)_Rom_Public.p_message_length) * 8; // bit count
            sha_M[15] = u32Length << 3; // bit count
        }
        else
        {
            // move n*32 bits data
            for (i = 0; i < 64;)
            {
                *(MS_U32*)msg = *(MS_U32*)(msg_addr);
                msg_addr += 4;

                sha_M[i/4] = (msg[0] << 24) | (msg[1] << 16) | (msg[2] << 8) | msg[3];
                i += 4;
            }
        }
        SHA256_Hash(sha_M, sha_H);
        len -= 64;
    }
    // Hash revert to digit
    for (i = 0; i < SHA_HASH_DIGI_LEN/2; i++)
    {
        tmp = sha_H[SHA_HASH_DIGI_LEN-1-i];
        sha_H[SHA_HASH_DIGI_LEN-1-i] = sha_H[i];
        sha_H[i] = tmp;
    }
}


#if 0
#define mips_cp0_count_get() ({MS_U32 count; asm volatile ("mfc0 %0, $9" : "=r" (count) :); count;})

void LDR_Delay(MS_U32 msec)
{
    MS_U32 tmo = (MS_U32)msec * (CPU_CLK_FREQ / 2 / 1000);

    asm volatile ("mtc0 $0, $11"); // clear cp0 compare
    asm volatile ("mtc0 $0, $9");  // clear cp0 compare
    while (mips_cp0_count_get() < tmo)
        ;
}

#else
void LDR_Delay(MS_U32 msec)
{
    MS_U32 tmo = (MS_U32)msec * (CPU_CLK_FREQ / 2 / 1000);
    MS_U32 count;
    for (count=0; count<tmo; count++)//temp
    {	
    };

}
#endif

void LDR_core1_loop(void)
{
   //coied from LDR_Entry_SPI which is used to wakeup arm core1
   //the following hard codes are wait be rewriten & update

    //drv_arm_exldr_exstr_enable 
    *(volatile MS_U16*)(0x1F203114) = 0x0030;
    
    MS_U32 pCoreLoop = (MS_U32)core1_loop;
    //drv_arm_gic_init
    *(volatile MS_U32*)(0x16001000) = 0x1;
    *(volatile MS_U32*)(0x16000104) = 0xf0;
    *(volatile MS_U32*)(0x16001180) = 0xffff0000;
    *(volatile MS_U32*)(0x16001100) = 0x0000ffff;
    *(volatile MS_U32*)(0x16000100) = 0x1;

    *(volatile MS_U16*)(0x1f2067A8) = (pCoreLoop & 0xFFFF);
    *(volatile MS_U16*)(0x1f2067AC) = ((pCoreLoop >> 16) & 0xFFFF);
    *(volatile MS_U16*)(0x1f2067B0) = 0xCAFE;

    *(volatile MS_U32*)(0x16001f00) = 0x00020001;
}

/*=========================================================
  Nand Driver
===========================================================*/
MS_U8 LDR_drvNAND_CountBits(MS_U32 u32_x)
{
        MS_U8 u8_i = 0;

        while (u32_x) {
                u8_i++;
                u32_x >>= 1;
        }

        return u8_i-1;
}

MS_U32 LDR_NC_ConfigContext(void)
{
    MS_U8 u8SShiftCntBits;
	if (LDR_nf_info.u16ECCType < ECC_TYPE_24BIT1KB)
		LDR_nf_info.u16SectorByteCnt = 0x200;
	else
		LDR_nf_info.u16SectorByteCnt = 0x400;

     u8SShiftCntBits = LDR_drvNAND_CountBits(LDR_nf_info.u16SectorByteCnt);
     LDR_nf_info.u16PageSectorCnt = LDR_nf_info.u16PageByteCnt >> u8SShiftCntBits;     

     u8SShiftCntBits = LDR_drvNAND_CountBits(LDR_nf_info.u16PageSectorCnt);
     LDR_nf_info.u16SectorSpareByteCnt = LDR_nf_info.u16SpareByteCnt >> u8SShiftCntBits;
     LDR_nf_info.u16SpareByteCnt = LDR_nf_info.u16SectorSpareByteCnt*LDR_nf_info.u16PageSectorCnt;
  
    return 0;
}

void LDR_NC_ResetFCIE(void)
{
    REG_CLR_BITS_UINT16(NC_TEST_MODE,BIT_FCIE_SOFT_RST);
    LDR_Delay(10);
    REG_SET_BITS_UINT16(NC_TEST_MODE,BIT_FCIE_SOFT_RST);

	// miu request reset
	REG_SET_BITS_UINT16(NC_MMA_PRI_REG, BIT_MIU_REQUEST_RST);
	LDR_Delay(10);
	REG_CLR_BITS_UINT16(NC_MMA_PRI_REG, BIT_MIU_REQUEST_RST);	

	
}

void LDR_NC_Config(void)
{
    HAL_WRITE_UINT16(NC_SDIO_CTL, LDR_nf_info.u16_Reg1B_SdioCtrl);
    /*sector spare size*/
	HAL_WRITE_UINT16(NC_SPARE, LDR_nf_info.u16Reg48_Spare);
	/* page spare size*/
	HAL_WRITE_UINT16(NC_SPARE_SIZE, LDR_nf_info.u16Reg49_SpareSize);
	/* page size and ECC type*/
	HAL_WRITE_UINT16(NC_ECC_CTRL, LDR_nf_info.u16Reg50_EccCtrl);
}

MS_U32 LDR_NC_ReInit()
{
	LDR_nf_info.u16_Reg1B_SdioCtrl = LDR_nf_info.u16SectorByteCnt + LDR_nf_info.u16SectorSpareByteCnt;
	LDR_nf_info.u16_Reg1B_SdioCtrl |= BIT_SDIO_BLK_MODE;
	LDR_nf_info.u16Reg48_Spare = LDR_nf_info.u16SectorSpareByteCnt;    
	LDR_nf_info.u16Reg49_SpareSize = LDR_nf_info.u16SpareByteCnt;
    LDR_nf_info.u16Reg50_EccCtrl = REG50_ECC_CTRL_INIT_VALUE;

    switch(LDR_nf_info.u16PageByteCnt)
    {
        case 0x0800:
            LDR_nf_info.u16Reg50_EccCtrl |= BIT_NC_PAGE_SIZE_2KB;
            break;
        case 0x1000:
            LDR_nf_info.u16Reg50_EccCtrl |= BIT_NC_PAGE_SIZE_4KB;
            break;
    }

    switch (LDR_nf_info.u16ECCType)
    {
		case ECC_TYPE_RS:
			LDR_nf_info.u16Reg50_EccCtrl |= BIT_NC_ECC_TYPE_RS;
			break;
		case ECC_TYPE_4BIT:
			LDR_nf_info.u16Reg50_EccCtrl &= ~BIT_NC_ECC_TYPE_4b512Bn;
			break;
		case ECC_TYPE_8BIT:
			LDR_nf_info.u16Reg50_EccCtrl |= BIT_NC_ECC_TYPE_8b512B;
			break;
		case ECC_TYPE_12BIT:
			LDR_nf_info.u16Reg50_EccCtrl |= BIT_NC_ECC_TYPE_12b512B;
			break;
		case ECC_TYPE_24BIT1KB:
			LDR_nf_info.u16Reg50_EccCtrl |= BIT_NC_ECC_TYPE_24b1KB;
			break;
		default:
		    break;
    }
    LDR_nf_info.u16Reg50_EccCtrl |= BIT_NC_ECCERR_NSTOP;
}

void LDR_NAND_SoftReset(void)
{  
    MS_U16 u16Tmp;
    
    HAL_WRITE_UINT16(NC_PATH_CTL,0);
    HAL_WRITE_UINT16(NC_CTRL,0);
    
    LDR_NC_ResetFCIE();

    HAL_WRITE_UINT16(NC_PATH_CTL,BIT_NC_EN);
    LDR_NC_ConfigContext();
    LDR_NC_ReInit();
    LDR_NC_Config();


#ifdef CONFIG_MSTAR_KRONUS
    HAL_READ_UINT16(FCIE_NC_CIFD_BASE, u16Tmp);
    HAL_READ_UINT16(FCIE_NC_CIFD_BASE, u16Tmp);
#endif

}


// ===========================================================
void LDR_NC_WaitComplete(MS_U16 u16_WaitEvent)
{
	MS_U16 u16_Reg;

	do
	{
		REG_READ_UINT16(NC_MIE_EVENT, u16_Reg);

	}while((u16_Reg & u16_WaitEvent) != u16_WaitEvent );

	REG_W1C_BITS_UINT16(NC_MIE_EVENT, u16_WaitEvent); /*clear events*/
	

}

void LDR_NC_waitFifoClkReady(void)
{
	 MS_U16 u16_Reg;

	do
	{
		REG_READ_UINT16(NC_MMA_PRI_REG, u16_Reg);
		
	}while((u16_Reg & BIT_NC_FIFO_CLKRDY) != BIT_NC_FIFO_CLKRDY);


}

MS_U32 LDR_nand_translate_DMA_address_Ex(MS_U32 u32_DMAAddr, MS_U32 u32_ByteCnt)
{
    flush_cache();
	REG_CLR_BITS_UINT16(NC_MIU_DMA_SEL,BIT_MIU1_SELECT);
    u32_DMAAddr -= 0x20000000;


	return u32_DMAAddr;
}


void LDR_NC_GetCIFD(MS_U8 *pu8_Buf, MS_U32 u32_CIFDPos, MS_U32 u32_ByteCnt)
{
	MS_U32 u32_i, u32_BufPos;
	MS_U16 u16_Tmp;

	if (u32_CIFDPos & 1) {
		REG_READ_UINT16(NC_CIFD_ADDR(u32_CIFDPos>>1), u16_Tmp);
		pu8_Buf[0] = (MS_U8)(u16_Tmp >> 8);
		u32_CIFDPos += 1;
		u32_ByteCnt -= 1;
		u32_BufPos = 1;
	} else
		u32_BufPos = 0;

	for (u32_i=0; u32_i<u32_ByteCnt>>1; u32_i++) {
		REG_READ_UINT16(NC_CIFD_ADDR(u32_i+(u32_CIFDPos>>1)), u16_Tmp);
		pu8_Buf[(u32_i<<1)+u32_BufPos] = (MS_U8)u16_Tmp;
		pu8_Buf[(u32_i<<1)+u32_BufPos+1] = (MS_U8)(u16_Tmp>>8);
	}

	if (u32_ByteCnt - (u32_i<<1)) {
		REG_READ_UINT16(NC_CIFD_ADDR(u32_i+(u32_CIFDPos>>1)), u16_Tmp);
		pu8_Buf[(u32_i<<1)+u32_BufPos] = (MS_U8)u16_Tmp;
	}

}

MS_U32  LDR_NC_ReadSector(MS_U32 u32_PhyRowIdx, MS_U8 u8_SectorInPage, MS_U8 *pu8_DataBuf,MS_U8 *pu8_SpareBuf)
{
	MS_U32 u32_Err = 0;
	MS_U32 u32_DataDMAAddr;
	MS_U16 u16_Tmp;
		
	REG_WRITE_UINT16(NC_MIE_EVENT, BIT_NC_JOB_END|BIT_MMA_DATA_END|BIT_MIU_LAST_DONE);
	u32_DataDMAAddr = LDR_nand_translate_DMA_address_Ex((U32)pu8_DataBuf, LDR_nf_info.u16SectorByteCnt);
	REG_WRITE_UINT16(NC_JOB_BL_CNT, 1); /* read 1 sector only */

	REG_WRITE_UINT16(NC_PART_MODE, BIT_PARTIAL_MODE_EN);
	REG_CLR_BITS_UINT16(NC_PART_MODE, BIT_START_SECTOR_IDX_MASK);
	REG_SET_BITS_UINT16(NC_PART_MODE, u8_SectorInPage<<BIT_START_SECTOR_IDX_SHIFT);

	REG_WRITE_UINT16(NC_SDIO_ADDR0, u32_DataDMAAddr & 0xFFFF);
	REG_WRITE_UINT16(NC_SDIO_ADDR1, u32_DataDMAAddr >> 16);
	REG_CLR_BITS_UINT16(NC_MMA_PRI_REG, BIT_NC_DMA_DIR_W);
	LDR_NC_waitFifoClkReady();

	REG_SET_BITS_UINT16(NC_PATH_CTL, BIT_MMA_EN);
	
	REG_WRITE_UINT16(NC_AUXREG_ADR, AUXADR_ADRSET);
	REG_WRITE_UINT16(NC_AUXREG_DAT, u8_SectorInPage * LDR_nf_info.u16SectorByteCnt);
	REG_WRITE_UINT16(NC_AUXREG_DAT, u32_PhyRowIdx & 0xFFFF);
	REG_WRITE_UINT16(NC_AUXREG_DAT, u32_PhyRowIdx >> 16);
	
	REG_WRITE_UINT16(NC_AUXREG_ADR, AUXADR_INSTQUE);
	REG_WRITE_UINT16(NC_AUXREG_DAT, (ADR_C5TFS0<< 8) | (CMD_0x00));

	REG_WRITE_UINT16(NC_AUXREG_DAT, (ACT_WAITRB << 8) | CMD_0x30);
	REG_WRITE_UINT16(NC_AUXREG_DAT, (ACT_BREAK << 8) | ACT_SER_DIN);

	REG_WRITE_UINT16(NC_CTRL,
           (BIT_NC_CIFD_ACCESS|BIT_NC_JOB_START)&~(BIT_NC_IF_DIR_W));
	
	LDR_NC_WaitComplete(BIT_NC_JOB_END|BIT_MMA_DATA_END|BIT_MIU_LAST_DONE);

	// Check ECC
	REG_READ_UINT16(NC_ECC_STAT0, u16_Tmp);
	if (u16_Tmp & BIT_NC_ECC_FAIL)
        return UNFD_ST_ERR_ECC_FAIL;

    LDR_NC_GetCIFD(pu8_SpareBuf, 0, LDR_nf_info.u16SectorSpareByteCnt);
	
	REG_WRITE_UINT16(NC_PART_MODE, 0);
	return UNFD_ST_SUCCESS;
}

MS_U32 LDR_NAND_ReadPage(MS_U32 u32_PhyRowIdx, MS_U8* pu8_DataBuf, MS_U8 *pu8_SpareBuf) // unit is the number of sector in the page
{
	MS_U32 u32_Err = 0;
	MS_U32 u32_DataDMAAddr;
	MS_U16 u16_Tmp;

	REG_WRITE_UINT16(NC_MIE_EVENT, BIT_NC_JOB_END|BIT_MMA_DATA_END|BIT_MIU_LAST_DONE);
	u32_DataDMAAddr = LDR_nand_translate_DMA_address_Ex((U32)pu8_DataBuf, LDR_nf_info.u16PageByteCnt);

	
	REG_WRITE_UINT16(NC_JOB_BL_CNT, LDR_nf_info.u16PageSectorCnt);
    REG_WRITE_UINT16(NC_SDIO_ADDR0, u32_DataDMAAddr & 0xFFFF);//>>MIU_BUS_WIDTH_BITS));
    REG_WRITE_UINT16(NC_SDIO_ADDR1, u32_DataDMAAddr >> 16);//(MIU_BUS_WIDTH_BITS+16)));
    REG_CLR_BITS_UINT16(NC_MMA_PRI_REG, BIT_NC_DMA_DIR_W);
    LDR_NC_waitFifoClkReady();
	
	REG_SET_BITS_UINT16(NC_PATH_CTL, BIT_MMA_EN);

	REG_WRITE_UINT16(NC_AUXREG_ADR, AUXADR_ADRSET);
	REG_WRITE_UINT16(NC_AUXREG_DAT, 0);
	REG_WRITE_UINT16(NC_AUXREG_DAT, u32_PhyRowIdx & 0xFFFF);
	REG_WRITE_UINT16(NC_AUXREG_DAT, u32_PhyRowIdx >> 16);
   
	REG_WRITE_UINT16(NC_AUXREG_ADR, AUXADR_INSTQUE);
	REG_WRITE_UINT16(NC_AUXREG_DAT, (ADR_C5TFS0<< 8) | (CMD_0x00));    
	REG_WRITE_UINT16(NC_AUXREG_DAT, (ACT_WAITRB << 8) | CMD_0x30);
	REG_WRITE_UINT16(NC_AUXREG_DAT, (ACT_REPEAT << 8) | ACT_SER_DIN);
	REG_WRITE_UINT16(NC_AUXREG_DAT, ACT_BREAK);
	REG_WRITE_UINT16(NC_CTRL, (BIT_NC_CIFD_ACCESS|BIT_NC_JOB_START) & ~(BIT_NC_IF_DIR_W));

    LDR_NC_WaitComplete(BIT_NC_JOB_END|BIT_MMA_DATA_END|BIT_MIU_LAST_DONE);

	// Check ECC
	REG_READ_UINT16(NC_ECC_STAT0, u16_Tmp);
	if (u16_Tmp & BIT_NC_ECC_FAIL)
    {   
        _PUTC('f');
        return UNFD_ST_ERR_ECC_FAIL;
    }

	LDR_NC_GetCIFD(pu8_SpareBuf, 0, LDR_nf_info.u16PageByteCnt);

    return UNFD_ST_SUCCESS;
}



//#define TEMPBUFFER  4096
void LDR_Entry_NAND(void)
{
    PF_AP_Entry         pfEntry;
    MS_U32 row_pos =0;
    MS_U32 blk_size = 0;
    MS_U32 decompress_pos = 0;

    PUTC('N'); PUTC('A'); PUTC('N'); PUTC('D'); PUTC('\n'); PUTC('\r');

    #if 1
    enable_cache();
    #endif

    HAL_WRITE_UINT16(NC_R2N_CTRL, 0); 
    LDR_nf_info.u16PageByteCnt = 0x800;    
	LDR_nf_info.u16SpareByteCnt = 0x100;    
	LDR_nf_info.u16ECCType = NANDINFO_ECC_TYPE;

    LDR_NAND_SoftReset();
    HAL_WRITE_UINT16(NC_SIGNAL, (BIT_NC_WP_AUTO | BIT_NC_WP_H | BIT_NC_CE_AUTO | BIT_NC_CE_H) &
                     ~(BIT_NC_CHK_RB_EDGEn | BIT_NC_CE_SEL_MASK));

    HAL_WRITE_UINT16(NC_CIF_FIFO_CTL,0);



    // read the page size and page number from MIU block area
    REG_SET_BITS_UINT16(NC_SPARE, BIT_NC_HW_AUTO_RANDOM_CMD_DISABLE);
    while(1)
    {
         
        LDR_NC_ReadSector(row_pos, 0, (MS_U8*)LDR_nf_info.pBuf,LDR_nf_info.pu8_SpareBuf);
        
        if (*((MS_U32*)(LDR_nf_info.pBuf)) == CISIDTAG)
        {
            LDR_nf_info.u16PageByteCnt = ((NAND_FLASH_INFO_t*)(LDR_nf_info.pBuf))->u16_PageByteCnt;
            LDR_nf_info.u16BlockPageCnt = ((NAND_FLASH_INFO_t*)(LDR_nf_info.pBuf))->u16_BlkPageCnt;
			LDR_nf_info.u16SpareByteCnt = ((NAND_FLASH_INFO_t*)(LDR_nf_info.pBuf))->u16_SpareByteCnt;
            LDR_nf_info.u16ECCType = ((NAND_FLASH_INFO_t*)(LDR_nf_info.pBuf))->u16_ECCType;
            break;
        }

        row_pos += 32;
    }
    REG_CLR_BITS_UINT16(NC_SPARE, BIT_NC_HW_AUTO_RANDOM_CMD_DISABLE);

    LDR_NC_ConfigContext();
    LDR_NC_ReInit();
    LDR_NC_Config();
 
    MS_U16* MIUPos;
    MS_U32 BINSize;
    MS_U32 Entry_Offset = 0;
    MS_U32 i;
    row_pos = 3*LDR_nf_info.u16BlockPageCnt;

    LDR_PUTC('B');LDR_PUTC('I');LDR_PUTC('N');LDR_PUTC('\n');LDR_PUTC('\r');
 
    while(1)
    {        
        LDR_NC_ReadSector(row_pos, 0 ,(MS_U8*)LDR_nf_info.pBuf,LDR_nf_info.pu8_SpareBuf);
        
        if (*((MS_U32*)(LDR_nf_info.pBuf)) == BINIDTAG)
        { 
            MIUPos = (MS_U16*)(*((MS_U32*)(LDR_nf_info.pBuf+0x2)));//MIU pos is the loading position and the jump offset
            BINSize = *((MS_U32*)(LDR_nf_info.pBuf+0xa));
            Entry_Offset = *((MS_U32*)(LDR_nf_info.pBuf+0x12));
            PUTC('A'); PUTC('P'); PUTC('P'); PUTC('T');PUTC('A');PUTC('G'); PUTC('\n'); PUTC('\r');
            break;
        }
        
        row_pos += LDR_nf_info.u16BlockPageCnt;
        
        _PUTC('.');
        if (row_pos == 20*LDR_nf_info.u16BlockPageCnt*LDR_nf_info.u16PageSectorCnt){
            while(1){};
        }
    }

    LDR_PUTDW((MS_U32)MIUPos);
    LDR_PUTC(' ');
    LDR_PUTDW((MS_U32)BINSize);
    LDR_PUTC(' ');	
    LDR_PUTDW((MS_U32)Entry_Offset); 
    LDR_PUTC('\n');
    LDR_PUTC('\r');
     
    pfEntry = (PF_AP_Entry)(MIUPos)+ Entry_Offset;

    row_pos++;
    while(1)
    {
        if ((row_pos & (LDR_nf_info.u16BlockPageCnt-1))==0)
        {
			LDR_NC_ReadSector(row_pos, 0 ,(MS_U8*)LDR_nf_info.pBuf,LDR_nf_info.pu8_SpareBuf);
		    if (*((MS_U32*)(LDR_nf_info.pBuf)) == BINIDTAG)
		    {
                row_pos++;
		    }
			else	
		    {
                row_pos += LDR_nf_info.u16BlockPageCnt;
		    }			
        }
		else
		{
			 LDR_PUTDW((MS_U32)MIUPos);
			 LDR_PUTC(' ');
			 LDR_PUTDW((MS_U32)row_pos);
			 LDR_PUTC(' ');		
             LDR_NAND_ReadPage(row_pos, (MS_U8*)MIUPos,LDR_nf_info.pu8_SpareBuf);

		     LDR_PUTDW(*(MS_U32*)MIUPos); 
			 LDR_PUTC('\n');
			 LDR_PUTC('\r');

			 MIUPos += (LDR_nf_info.u16PageByteCnt>>1);
		 
			 if(BINSize >= LDR_nf_info.u16PageByteCnt)
			 {
				 BINSize -= LDR_nf_info.u16PageByteCnt;
                 if(BINSize==0)
			         break;				 
			 }
			 else
			 {
                 if(BINSize==0)
			         break;		
				 
				 BINSize =0;
			 }
			 row_pos++;
		}
    }

    //LDR_PUTC('P');
    LDR_PadConfig();
    //LDR_PUTC('P');
    LDR_core1_loop();

    if (1)
    {
        extern void DoMSDecompression(U32 addr);

        flush_cache();
        PUTC('\r');PUTC('\n');
        PUTC('J');PUTC('2');PUTC('A');PUTC('P');PUTC('P');
        PUTC('\r');PUTC('\n');
             
        pfEntry();
    }

}
//#endif // bfn

#include "bootrom_spi.h"
static MS_U32 _LDR_SERFLASH_WaitReadDataRdy(void)
{
    MS_U32 bRet = 0;
    MS_U32 u32Timer;

    SER_FLASH_TIME(u32Timer, SERFLASH_SAFETY_FACTOR);

    do
    {
        if ((ISP_READ(REG_ISP_SPI_RD_DATARDY) & ISP_SPI_RD_DATARDY_MASK) == ISP_SPI_RD_DATARDY )
        {
            bRet = 1;
            break;
        }
    } while (!SER_FLASH_EXPIRE(u32Timer));

    return bRet;
}

static MS_U32 _LDR_SERFLASH_WaitWriteDone(void)
{
    MS_U32 bRet = 0;
    MS_U32 u32Timer;

    SER_FLASH_TIME(u32Timer, SERFLASH_SAFETY_FACTOR*5);

    do
    {
        ISP_WRITE(REG_ISP_SPI_COMMAND, ISP_SPI_CMD_RDSR); // RDSR

        ISP_WRITE(REG_ISP_SPI_RDREQ, ISP_SPI_RDREQ); // SPI read request

        if (_LDR_SERFLASH_WaitReadDataRdy() == 0 )
        {
            break;
        }

        if ((ISP_READ(REG_ISP_SPI_RDATA) & SF_SR_WIP_MASK) == 0 ) // WIP = 0 write done
        {
            bRet = 1;
            break;
        }
    } while (!SER_FLASH_EXPIRE(u32Timer));

    ISP_WRITE(REG_ISP_SPI_CECLR, ISP_SPI_CECLR); // SPI CEB dis

    return bRet;
}

MS_U32 LDR_SERFLASH_ReadID(void)
{

#ifdef CONFIG_LD_BOOT_FROM_OTP  
    //_PUTS("ID=0\n");
    //return 0;      

    // HW doesn't support ReadID on MX/ST flash; use trigger mode instead.
    MS_U32 bRet = 0;
    MS_U32 u32I;
    MS_U32 u32Size;
    MS_U8 u8ptr ;

    ISP_WRITE(REG_ISP_PASSWORD, 0xAAAA);//_HAL_ISP_Enable();

    if (!_LDR_SERFLASH_WaitWriteDone())
    {
        goto LDR_SERFLASH_ReadID_return;
    }
    bRet = 1;

LDR_SERFLASH_ReadID_return:
    ISP_WRITE(REG_ISP_SPI_CECLR, ISP_SPI_CECLR); // SPI CEB dis

    ISP_WRITE(REG_ISP_PASSWORD, 0x5555);//_HAL_ISP_Disable();

    //_HAL_SPI_Rest(); // to prevent BDMA fail
    ISP_WRITE_MASK(REG_ISP_CHIP_RST, SFSH_CHIP_RESET, SFSH_CHIP_RESET_MASK);
    ISP_WRITE_MASK(REG_ISP_CHIP_RST, SFSH_CHIP_NOTRESET, SFSH_CHIP_RESET_MASK);

    return bRet;

#elif CONFIG_LD_BOOT_OTP_SIMULATE_ON_SPINOR
    return 0;
#else
    return 1;
#endif
}

#if 0
void remap_dspram(void)
{
    void remap_dspram_asm(void);
    asm volatile(
            "jal remap_dspram_asm\n"
            "nop\n" : : :
            "$0","$1", "$2", "$3", "$4", "$5", "$6", "$7",
            "$8", "$9", "$10", "$11", "$12", "$13", "$14", "$15",
            "$16", "$17", "$18", "$19", "$20", "$21", "$22", "$23",
            "$24", "$25", "$26", "$27", "$29", "$31",
            "memory");
}
#endif

int DoUnCompression(unsigned int addr)
{
    //ISzAlloc g_Alloc = { SzAlloc, SzFree };
    //SRes res;
    int useFilter;
    //SizeT inSizePure;
    //ELzmaStatus status;
    unsigned int u32ROM_START = addr + 0x100;                  // This address should be 4' alignment
    unsigned int u32ROM_END = addr + *((unsigned int *)(addr + 0x8));   // This address should be 4' alignment
    unsigned int u32RAM_START, u32RAM_END,i =0;
    unsigned int CompressedFileLength = 0;
    unsigned int OriginalFileLength = 0;
    //unsigned char header[LZMA_PROPS_SIZE + 8];
    /* Original Param */
    //Byte *dest = NULL;
    unsigned int destLen = 0;
    //const Byte *src = NULL;
    unsigned int srcLen = 0;
    unsigned int dst = *(unsigned int*)addr; 

    //info_mem =(unsigned int *)(0xA0880030);

    u32RAM_START = *((unsigned int *)(addr)) + 0x100;
    u32RAM_END = u32RAM_START+ *((unsigned int *)(addr + 0xC));

    CompressedFileLength = (u32ROM_END - u32ROM_START);
    OriginalFileLength = (u32RAM_END - u32RAM_START);

    //Copy compressed UBoot from SPI flash to RAM
    for(i = 0; i < ((CompressedFileLength + 7 + 0x100) & ~7UL); i += 4)
    {
        *((unsigned int *)(dst + i)) = *((unsigned int *)((u32ROM_START - 0x100) + i));
        // Copy from Addr (Storage to DRAM)
    }

    if(!LDR_APP_Auth((unsigned char *)dst, *(unsigned int *)(addr + 0x8)))
    {
        PUTC('!');
        return -1;
    }
 
    return 0;
}


void LDR_Entry_SPI(void)
{
    MS_U32              *src, *dst, *end;
    PF_AP_Entry         pfEntry;
    PUTC('S'); PUTC('P'); PUTC('I'); PUTC('\n'); PUTC('\r');
#if 1
    enable_cache();
#endif
    //Note: src, dst, end should be 16-byte aligned.
    src = (MS_U32*)_ld_APP_load_start;
    dst = (MS_U32*)(*(src));
    end = (MS_U32*)((MS_U32)src + *(src+3));
    pfEntry = (PF_AP_Entry)((MS_U32)dst + *(src+1));

    LDR_PadConfig();

    LDR_core1_loop();

    if (1)
    {

#if defined(CONFIG_LDR_LZMA_COMPRESS)

        PUTC('d');PUTC('e');PUTC('c');PUTC('o');PUTC('m');PUTC('p');PUTC('\r');PUTC('\n');
        extern int DoLZMADecompression(U32 addr);
        if (DoLZMADecompression((U32)(_ld_APP_load_start)) < 0)
        {
            while(1);
        }
        PUTC('d');PUTC('o');PUTC('n');PUTC('e');PUTC('\r');PUTC('\n');
#elif defined(CONFIG_LDR_NOCOMPRESS)
        PUTC('a');PUTC('u');PUTC('t');PUTC('h');PUTC('\r');PUTC('\n');
        if (DoUnCompression((U32)(_ld_APP_load_start)) < 0)
        {
            while(1);
        }
        PUTC('o');PUTC('k');
#else
#error "Error"
#endif
        flush_cache();

        pfEntry();
    }
}

#if defined(CONFIG_MSTAR_KAISER)

#define LDR_BASE_BLK_ADDR       0x1000
#define LDR_LIMIT_BLK_END_ADDR  0x3800
void LDR_Entry_EMMC(void)
{
    PF_AP_Entry pfEntry;
    MS_U16* MIUPos;
    MS_U32 BINSize = 0;
    MS_U32 load_blk = LDR_BASE_BLK_ADDR;

    PUTC('E'); PUTC('M'); PUTC('M'); PUTC('C'); PUTC('\n'); PUTC('\r');

    enable_cache();
    
    eMMC_Init_Ex(); //init emmc
    
    //find APP BIN page
    do{ 
        if(load_blk > LDR_LIMIT_BLK_END_ADDR)
        {
            PUTC('R');PUTC('D');PUTC('>');PUTC('E');PUTC('N');PUTC('D');
            while(1){};
        }
        eMMC_ReadData((MS_U8*)LDR_nf_info.pBuf, 512, load_blk);
        load_blk ++;
        LDR_PUTDW(load_blk);;LDR_PUTC(' ');
        LDR_PUTDW(*((MS_U32 *)LDR_nf_info.pBuf));LDR_PUTC('\n');LDR_PUTC('\r');
    }while(*((MS_U32 *)LDR_nf_info.pBuf) != BINIDTAG);

    //HIT APP BIN => Start to read Bin header
    BINSize = *(MS_U32 *) (LDR_nf_info.pBuf+2);
    pfEntry  =(PF_AP_Entry)  *(MS_U32 *) (LDR_nf_info.pBuf+4);
    
    //allign to 512B
    if((BINSize&0x1ff)!=0) BINSize = (BINSize&0xFFFFFE00)+0x200;
   
    LDR_PUTDW(BINSize);LDR_PUTC('\n');LDR_PUTC('\r'); 
    LDR_PUTDW((MS_U32)pfEntry);LDR_PUTC('\n');LDR_PUTC('\r');
    LDR_PUTDW((MS_U32)LDR_nf_info.pBuf);LDR_PUTC('\n');LDR_PUTC('\r');

    //pfEntry = (PF_AP_Entry)(MIUPos);
    eMMC_ReadData((MS_U8*) pfEntry, BINSize, load_blk);

    LDR_PadConfig();

    LDR_core1_loop();

    if (1)
    {
        flush_cache();
        PUTC('\r');PUTC('\n');
        PUTC('J');PUTC('2');PUTC('A');PUTC('P');PUTC('P');
        PUTC('\r');PUTC('\n');
        pfEntry();
    }
}
#else
void LDR_Entry_EMMC(void)
{
    PUTC('N');PUTC('o');PUTC('M');PUTC('M');PUTC('C');PUTC('F');PUTC('U');PUTC('N');PUTC('C');
}
#endif


void LDR_BootEnd_C(void)
{    
     void LDR_BootEnd(void);
     asm volatile(
         "push  {r8,r9,r10,r11,r12}\n\t"
         "mov   r9, lr\n\t"
         "bl    LDR_BootEnd\n"
         "nop\n"
         "mov   lr, r9\n\t"
         "pop  {r8,r9,r10,r11,r12}\n\t"
         ::);
}

/*
MS_U32 LDR_check_emmc_exist(MS_U32 addr, MS_U32 size)
{
    MS_U32 ret =0;
    void LDR_check_emmc_exist_asm(void);
    asm volatile(
        "push  {r8,r9,r10,r11,r12}\n\t"
        "mov   r9, lr\n\t"
        "mov   r8, %[adr]\n\t"
        "mov   r10, %[sz]\n\t"
        "bl    LDR_check_emmc_exist_asm\n\t"
        "nop\n\t"
        "mov   %[rt], r8\n\t"
        "mov   lr, r9\n\t"
        "pop  {r8,r9,r10,r11,r12}\n\t"
        :: [adr] "r" (addr), [sz] "r" (size), [rt] "r" (ret) :
        "memory");   
    return ret;
}
*/
/*====================================================================
   LDR Start Entry
======================================================================*/
extern void LDR_Entry(void)  _SECTION_LDR_ENTRY_;
extern NAND_NECESS_INFO _nf_info;

void LDR_Entry(void)
{
#ifdef CONFIG_NO_2ED_LOADER
    PUTC('\r'); PUTC('\n');
    PUTC('T'); PUTC('E'); PUTC('S'); PUTC('T'); PUTC('_');
    PUTC('L'); PUTC('D'); PUTC('R'); PUTC('_'); PUTC('O'); PUTC('K');
    PUTC('\r'); PUTC('\n');

    while(1) {
        // do nothing
    }
#else

    //remap_dspram();
    PUTC('L'); PUTC('D'); PUTC('R'); PUTC('_');

    //Set default SPSR value from CPSR
    //Ecos kernel may misuse the SPSR in load_contex
    //which may copy wrong nig endian seeting to CPSR
    asm volatile(
            "MRS     r0, CPSR\n"
            "MSR     SPSR_fsxc, r0\n"
    : : :);

    if (LDR_SERFLASH_ReadID())
    {
        LDR_Entry_SPI();
        
    }
    else if ( _boot_flow_mode == BOOT_FLOW_IS_EMMC) 
    {
        LDR_Entry_EMMC();
    }
    else
    {
        LDR_Entry_NAND();
    }

#endif
}
