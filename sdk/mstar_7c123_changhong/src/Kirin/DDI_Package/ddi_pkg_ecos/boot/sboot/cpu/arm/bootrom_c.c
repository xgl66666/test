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
/// file    bootrom_c.c
/// @brief  C boot rom code function
/// @author MStar Semiconductor Inc.
/// @note   Rom code functions is running on read-only memory (ROM), all function should follow the rules. \n
///         (1) source code should be standalone
///         (2) all functions are running in one thread
///         (3) all variables should be read only except local varaibles in the stack or register.
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "boot.h"
#include "chip.h"
#include "Board.h"
#include "drvNAND.h"
//#include "bootrom_nand.h"
#include "bootrom_spi.h"

extern MS_U32 _var_LDR_run_base;
extern MS_U32 _var_LDR_size;
extern MS_U32 _var_LDR_entry_point;
extern MS_U32 _var_LDR_stack_end;

NAND_NECESS_INFO _nf_info;

MS_U8 _boot_flow_mode = BOOT_FLOW_IS_NONE ;

#if 0
void DelayMS(MS_U32 msec)
{
    MS_U32 tmo = (MS_U32)msec * (CPU_CLK_FREQ / 2 / 1000);
    MS_U32 count;

    asm volatile ("mtc0 $0, $11" : :); // clear cp0 compare
    asm volatile ("mtc0 $0, $9" : :);  // clear cp0

    do
    {
        asm volatile ("mfc0 %0, $9" : "=r" (count) :);
    } while(count < tmo);
}
#else
#define LOOP_COUNT984MHZ 110
void DelayMS(MS_U32 msec)
{
    MS_U32 tmo = (MS_U32)msec * ((LOOP_COUNT984MHZ*(CPU_CLK_FREQ/1000000))/984);
    MS_U32 count;
    for (count=0; count<tmo; count++)//temp
    {
    };

}
#endif
void _PUTC(int c)
{
    while((RREG16(REG_UART_LSR) & UART_LSR_THRE) != UART_LSR_THRE)
        ;
    WREG16(REG_UART_TX, c);
    while((RREG16(REG_UART_LSR) & (UART_LSR_TEMT | UART_LSR_THRE)) != (UART_LSR_TEMT | UART_LSR_THRE))
        ;
}

void _PUTS(const char *s)
{
    while(*s) {
        if(*s == '\n')
            _PUTC('\r');
        _PUTC(*s++);
    }
}

void _PUTX(char val)
{
    if (val > 0x9)
        _PUTC('A' - 10 + val);
    else
        _PUTC('0' + val);
}

void _PUTW(MS_U16 val)
{
    _PUTX((val >> 12) & 0xF);
    _PUTX((val >> 8) & 0xF);
    _PUTX((val >> 4) & 0xF);
    _PUTX(val & 0xF);
}

void _PUTDW(MS_U32 val)
{
    _PUTW((val >> 16) & 0xFFFF);
    _PUTW(val & 0xFFFF);
}

void BOOTROM_C_GetHashOneInfo(const MS_U32 u32HASH1[])
{
    _var_LDR_run_base = u32HASH1[1022];
    _var_LDR_size = u32HASH1[1023];
}

#ifdef CONFIG_MSTAR_KAISER
void BOOTROM_C_InitDramMOBF(void)
{
    #if defined(CONFIG_DRAM_MOBF)

        /* CONFIG_DRAM_MOBF_8BIT */
        #if defined(CONFIG_DRAM_MOBF_8BIT)
        WREG16(((REG_CHIP_TOP_BASE) + (0x84*2)) ,0xffff); // bypass miu_write_cache
        WREG16(((REG_CHIP_TOP_BASE) + (0x86*2)) ,0xffff); // bypass miu_write_cache
        WREG16(((REG_CHIP_TOP_BASE) + (0x88*2)) ,0xffff); // bypass miu_write_cache

        //Dram MOBF mode
        WREG16(((REG_MOBF_MIU0_BASE) + (0xd8*2)) ,0x0400); // Enable dramobf with 8 bit mode
        WREG16(((REG_MOBF_MIU1_BASE) + (0xd8*2)) ,0x0400); // Enable dramobf with 8 bit mode

        WREG16((REG_MOBF_CONTROL_BASE + (0xec*2)) ,0xc800); /* Triggler MOBF */

        #else /* CONFIG_DRAM_MOBF_64BIT, with 1 Queue ultra mode */
        WREG16(((REG_CHIP_TOP_BASE) + (0x84*2)) ,0x0102); // MIU write cache bypass [0:47], (JPD, HVD bypass miu_write_cache)
        WREG16(((REG_CHIP_TOP_BASE) + (0x86*2)) ,0x0002); // VP6 bypass miu_write_cache
        WREG16(((REG_CHIP_TOP_BASE) + (0x88*2)) ,0x0000); // no bypass

        //sec_r2
        WREG16(((REG_CHIP_TOP_BASE) + (0x80*2)) ,0x0003); // [0] sec_r2 miu_write_cache one_queue mode enable [1] sec_r2 miu_write_cache ultra mode enable

        //ca9
        WREG16(((REG_CHIP_TOP_BASE) + (0x8a*2)) ,0x0003); // [0] sec_r2 miu_write_cache one_queue mode enable [1] sec_r2 miu_write_cache ultra mode enable

        //VIVALDI9
        WREG16(((REG_WRITE_CACHE_AU_R2_BASE) + (0x04*2)) ,0x0006);

        //USB20
        WREG16(((REG_WRITE_CACHE_USB20_BASE) + (0x00*2)) ,0x3000);
        WREG16(((REG_WRITE_CACHE_USB20_0_BASE) + (0x00*2)) ,0x3000);
        WREG16(((REG_WRITE_CACHE_USB20_1_BASE) + (0x80*2)) ,0x3000);

        //USB30
        WREG16(((REG_WRITE_CACHE_USB30_BASE) + (0xf8*2)) ,0x0006); // [0] sec_r2 miu_write_cache one_queue mode enable [1] sec_r2 miu_write_cache ultra mode enable

        //Dram MOBF mode
        WREG16(((REG_MOBF_MIU0_BASE) + (0xd8*2)) ,0x0000); // Enable dramobf with 64 bit mode (2 des)
        WREG16(((REG_MOBF_MIU1_BASE) + (0xd8*2)) ,0x0000); // Enable dramobf with 64 bit mode (2 des)
        //WREG16(((REG_MOBF_MIU0_BASE) + (0xd8*2)) ,0x0000); // Enable dramobf with 64 bit mode (4 des)
        //WREG16(((REG_MOBF_MIU1_BASE) + (0xd8*2)) ,0x0000); // Enable dramobf with 64 bit mode (4 des)


        WREG16((REG_MOBF_CONTROL_BASE + (0xec*2)) ,0xc800); /* Triggler MOBF */
        #endif

    #else //NOT CONFIG_DRAM_MOBF
        /* no MOBF and bypass MIU write Cache*/
        WREG16(((REG_CHIP_TOP_BASE) + (0x84*2)) ,0xffff); // bypass miu_write_cache
        WREG16(((REG_CHIP_TOP_BASE) + (0x86*2)) ,0xffff); // bypass miu_write_cache
        WREG16(((REG_CHIP_TOP_BASE) + (0x88*2)) ,0xffff); // bypass miu_write_cache
    #endif
}
#else
void BOOTROM_C_InitDramMOBF(void)
{
    //Do Nothing
}
#endif


void BOOTROM_C_InitMIU_SPI(void)
{
    extern MS_U8 _ld_HASH1_run_start[];

    _PUTS("SPI\n\r");
    BOOTROM_C_GetHashOneInfo((MS_U32 *)_ld_HASH1_run_start);
    BOOTROM_C_InitParams(_ld_HASH1_run_start);

    BOOTROM_C_InitDramMOBF();
}


void arm_init_for_miu_access(void)
{
#if defined(CONFIG_MSTAR_KAISER)
/*
        "        LDR     r12, =0x1F203104\n\t"
        "        LDR     r11, =0x84\n\t"
        "        STRB    r11, [r12]\n\t"
*/
     *(volatile unsigned short *)0x1F203104 = 0x84;

    asm volatile(

        "        MOV     r0, #0\n\t"
        "        MCR     p15, 0, r0, c8, c7, 0\n\t"      //@ Invalidate Inst TLB and Data TLB"
        "        MCR     p15, 0, r0, c7, c5, 0\n\t"      //@ Invalidate I Cache"

        "        MRC     p15, 1, r0, c0, c0, 1\n\t"      //@ read Cache Level ID register (clidr)"
        "        ANDS    r3, r0, #0x7000000\n\t"         //@ extract level of coherency from clidr"
        "        MOV     r3, r3, lsr #23\n\t"            //@ left align level of coherency bit field"
        "        BEQ     finishedd\n\t"                   //@ if loc is 0, then no need to clean"

        "        MOV     r10, #0\n\t"                    //@ start clean at cache level 0 (in r10)"
        "loopp1:"
        "        ADD     r2, r10, r10, lsr #1\n\t"       //@ work out 3x current cache level"
        "        MOV     r1, r0, lsr r2\n\t"             //@ extract cache type bits from clidr"
        "        AND     r1, r1, #7\n\t"                 //@ mask of the bits for current cache only"
        "        CMP     r1, #2\n\t"                     //@ see what cache we have at this level"
        "        BLT     skipp\n\t"                       //@ skip if no cache, or just i-cache"
        "        MCR     p15, 2, r10, c0, c0, 0\n\t"     //@ select current cache level in cssr"
        "        MOV     r1, #0\n\t"
        "        MCR     p15, 0, r1, c7, c5, 4\n\t"      //@ prefetchflush to synch the new cssr&csidr"
        "        MRC     p15, 1, r1, c0, c0, 0\n\t"      //@ read the new csidr"
        "        AND     r2, r1, #7\n\t"                 //@ extract the length of the cache lines"
        "        ADD     r2, r2, #4\n\t"                 //@ add 4 (line length offset)"
//        "        LDR     r6, =0x3ff\n\t"
//        "        ANDS    r6, r6, r1, lsr #3\n\t"         //@ find maximum number on the way size"
        "        MOV     r6, #400\n\t"
        "        SUB     r6, r6, #0x1\n\t"
        "       ANDS    r6, r6, r1, lsr #3\n\t"         //@ find maximum number on the way size"
        "        .WORD   0xE16F5F16\n\t"                 //@DCI   0xE16F5F16 @CLZ r5, r6 @ find bit position of way size increment"
        "        MOV     r7, #800\n\t"
        "        SUB     r7, r7, #0x1\n\t"        
//        "        LDR     r7, =0x7fff\n\t"
//        "        ANDS    r7, r7, r1, lsr #13\n\t"        //@ extract max number of the index size"
        "        ANDS    r7, r7, r1, lsr #13\n\t"        //@ extract max number of the index size"
        "loopp2:\n\t"
        "        MOV     r8, r6\n\t"                     //@ create working copy of max way size"
        "loopp3:\n\t"
        "        ORR     r12, r10, r8, lsl r5\n\t"       //@ factor way and cache number into r12"
        "        ORR     r12, r12, r7, lsl r2\n\t"       //@ factor index number into r12"
        "        MCR     p15, 0, r12, c7, c6, 2\n\t"     //@ invalidate by set/way"
        "        SUBS    r8, r8, #1\n\t"                 //@ decrement the way"
        "        BGE     loopp3\n\t"
        "        SUBS    r7, r7, #1\n\t"                 //@ decrement the index"
        "        BGE     loopp2\n\t"
        "skipp:\n\t"
        "        ADD     r10, r10, #2\n\t"               //@ increment cache number"
        "        CMP     r3, r10\n\t"
        "        BGT     loopp1\n\t"

        "finishedd:\n\t"
        "        nop\n\t"
        :
        :
        : "r0", "r1", "r2", "r3", "r5", "r6", "r7", "r8", "r10", "r12", "memory");
#endif
}

#if defined(CONFIG_LD_BOOT_FROM_OTP) || defined(CONFIG_LD_BOOT_OTP_SIMULATE_ON_SPINOR)
/* r8: address, 8 byte unit  r10:size, 512 byte unit*/
/*
void GetPartitionDataStage1(MS_U32 addr, MS_U32 size)
{
    void BOOT_CopyUboot_EMMC_Stage1(void);
    asm volatile(
        //"push  {r8,r9,r10,r11,r12}\n\t"
        "mov   r9, lr\n\t"
        "mov   r8, %[adr]\n\t"
        "mov   r10, %[sz]\n\t"
        "bl    BOOT_CopyUboot_EMMC_Stage1\n\t"
        "nop\n\t"
        "mov   lr, r9\n\t"
        //"pop   {r8,r9,r10,r11,r12}\n\t"
        :: [adr] "r" (addr), [sz] "r" (size) :
        "r9","r10","r11","r12","memory");
}
*/

void GetPartitionDataStage2(MS_U32 addr, MS_U32 size, MS_U32 destination)
{
    void BOOT_CopyUboot_EMMC_Stage2(void);

    asm volatile(
        "mov   r7, %[dst]\n\t"
        "mov   r9, lr\n\t"
        "mov   r8, %[adr]\n\t"
        "mov   r10, %[sz]\n\t"
        "bl    BOOT_CopyUboot_EMMC_Stage2\n\t"
        "mov   lr, r9\n\t"
        //"nop\n\t"
        :: [adr] "r" (addr), [sz] "r" (size), [dst] "r" (destination) :
        "r7","r8","r9","r10","r11","r12","memory");
}
void BootEnd_C(void)
{
    void BootEnd(void);
    asm volatile(
        "mov   r9, lr\n\t"
        "bl    BootEnd\n"
        "nop\n"
        "mov   lr, r9\n\t"
        :::
        "r8","r9","r10","r11","r12","memory");
}

void reset_eMMC(void)
{
    void RESET_eMMC(void);
    asm volatile(
        "mov   r9, lr\n\t"
        "bl RESET_eMMC\n"
        "nop\n"
        "mov   lr, r9\n\t"
        :::
        "r8","r9","r10","r11","r12","memory");
}


MS_U32 check_emmc_exist(MS_U32 addr, MS_U32 size)
{
    MS_U32 ret =0;
    void check_emmc_exist_asm(void);
    asm volatile(
        "mov   r9, lr\n\t"
        "mov   r8, %[adr]\n\t"
        "mov   r10, %[sz]\n\t"
        "bl    check_emmc_exist_asm\n\t"
        "nop\n\t"
        "mov   lr, r9\n\t"
        "mov   %[rt], r8\n\t"
        :: [adr] "r" (addr), [sz] "r" (size), [rt] "r" (ret) :
        "r8","r9","r10","r11","r12","memory");

    return ret;
}

#define MMC_2_SRAM 0
#define MMC_2_DRAM 1

MS_U32 BOOTROM_C_InitMIU_EMMC(void)
{
    MS_U32 load_size = 0;
    MS_U32 load_offset = 0;
    MS_U32 load_dst_addr = 0;
    MS_U32 loader_size = 0;
  
    _PUTS("EMMC\n\r");
    load_dst_addr = ((MS_U32)(_nf_info.pBuf)) >> 3 ;  // 8 byte address

    //// find miu page
    //GetPartitionDataStage1(load_dst_addr,1);//512 per 1 unit

    while ( *((MS_U32 *)_nf_info.pBuf) != MIUIDTAG)
    {
        //_PUTW(*(MS_U32 *)_nf_info.pBuf);_PUTC('\n');
        if(load_size > 256)
        {
            _PUTC('>');_PUTC('2');_PUTC('5');_PUTC('6');
            while(1);
        }
        load_size ++;
        GetPartitionDataStage2(load_dst_addr,1,MMC_2_SRAM);//512 per 1 unit
    }

    //Hit MIU page
    load_size = * (MS_U32 *) (_nf_info.pBuf+2);         // load header : byte size info
    if((load_size&0x7)!=0) load_size = load_size + 8;   // TRANS to 8 byte address
    load_size = load_size >> 3;                         //
   
    do{
        if(load_offset > 0x4000)
        {
            _PUTC('>');_PUTC('4');_PUTC('0');_PUTC('0');_PUTC('0');
            while(1);
        }
        GetPartitionDataStage2(load_dst_addr+load_offset,1,MMC_2_SRAM);//512 per 1 unit
        load_offset += 0x40;                  //512 = 0x100 => =/8 =0x40
    }while(load_size > load_offset);

    //save loader image size and start entry pointer
	_var_LDR_entry_point = (* (MS_U32 *) (_nf_info.pBuf+0x7fc))+0x100; 
	loader_size = (* (MS_U32 *) (_nf_info.pBuf+0x7fe)); 

    BOOTROM_C_InitParams(_nf_info.pBuf);

    arm_init_for_miu_access();

	load_dst_addr = (MS_U32)_var_LDR_entry_point >> 3 ;  // 8 byte address

    GetPartitionDataStage2(load_dst_addr,1,MMC_2_DRAM);//512 per 1 unit

	while ( *((MS_U32 *)_var_LDR_entry_point) != LDRIDTAG)
    {
        GetPartitionDataStage2(load_dst_addr,1,MMC_2_DRAM);//512 per 1 unit
    }
    
    //Hit loader
    _var_LDR_stack_end = *((MS_U32*)(_var_LDR_entry_point+8));

	load_size = loader_size;                            
    if((load_size&0x7)!=0) load_size = load_size + 8;   // TRANS to 8 byte address
    load_size = load_size >> 3;                         //
    load_offset = 0;
    do{
        if(load_offset > 0x4000)
        {
            _PUTC('>');_PUTC('4');_PUTC('0');_PUTC('0');_PUTC('0');
            while(1);
        }
        GetPartitionDataStage2(load_dst_addr+load_offset,1,MMC_2_DRAM);//512 per 1 unit
        load_offset += 0x40;                  //512 = 0x100 => =/8 =0x40
    }while(load_size > load_offset);

    BootEnd_C();
}

#endif //#ifdef CONFIG_LD_BOOT_FROM_OTP 

void BOOTROM_C_GetHashTwoInfo(void)
{
    MS_U32 header = _var_LDR_run_base;
    MS_U32 entry_offset = *(MS_U32*)(header + 0x4);
    _var_LDR_entry_point = ((MS_U32)header + entry_offset); // entry_address
    _var_LDR_stack_end   = _var_LDR_run_base + 0x8;
}


MS_U8 drvNAND_CountBits(MS_U32 u32_x)
{
        MS_U8 u8_i = 0;

        while (u32_x) {
                u8_i++;
                u32_x >>= 1;
        }

        return u8_i-1;
}


MS_U32 NC_ConfigContext(void)
{
     MS_U8 u8SShiftCntBits;
	if (_nf_info.u16ECCType < ECC_TYPE_24BIT1KB)
		_nf_info.u16SectorByteCnt = 0x200;
	else
		_nf_info.u16SectorByteCnt = 0x400;

     //_nf_info.u16PageSectorCnt = _nf_info.u16PageByteCnt/_nf_info.u16SectorByteCnt;
     u8SShiftCntBits = drvNAND_CountBits(_nf_info.u16SectorByteCnt);
     _nf_info.u16PageSectorCnt = _nf_info.u16PageByteCnt >> u8SShiftCntBits;

     //_nf_info.u16SectorSpareByteCnt = _nf_info.u16SpareByteCnt/_nf_info.u16PageSectorCnt;
     u8SShiftCntBits = drvNAND_CountBits(_nf_info.u16PageSectorCnt);
     _nf_info.u16SectorSpareByteCnt = _nf_info.u16SpareByteCnt >> u8SShiftCntBits;
    _nf_info.u16SpareByteCnt = _nf_info.u16SectorSpareByteCnt*_nf_info.u16PageSectorCnt;

    return 0;
}

void NC_ResetFCIE(void)
{
    REG_CLR_BITS_UINT16(NC_TEST_MODE,BIT_FCIE_SOFT_RST);
    DelayMS(10);
    REG_SET_BITS_UINT16(NC_TEST_MODE,BIT_FCIE_SOFT_RST);

	// miu request reset
	REG_SET_BITS_UINT16(NC_MMA_PRI_REG, BIT_MIU_REQUEST_RST);
	DelayMS(10);
	REG_CLR_BITS_UINT16(NC_MMA_PRI_REG, BIT_MIU_REQUEST_RST);	
}

void NC_Config(void)
{
    HAL_WRITE_UINT16(NC_SDIO_CTL, _nf_info.u16_Reg1B_SdioCtrl);
    /*sector spare size*/
	HAL_WRITE_UINT16(NC_SPARE, _nf_info.u16Reg48_Spare);
	/* page spare size*/
	HAL_WRITE_UINT16(NC_SPARE_SIZE, _nf_info.u16Reg49_SpareSize);
	/* page size and ECC type*/
	HAL_WRITE_UINT16(NC_ECC_CTRL, _nf_info.u16Reg50_EccCtrl);
}

MS_U32 NC_ReInit()
{
	_nf_info.u16_Reg1B_SdioCtrl = _nf_info.u16SectorByteCnt + _nf_info.u16SectorSpareByteCnt;
	_nf_info.u16_Reg1B_SdioCtrl |= BIT_SDIO_BLK_MODE;
	_nf_info.u16Reg48_Spare = _nf_info.u16SectorSpareByteCnt;    
	_nf_info.u16Reg49_SpareSize = _nf_info.u16SpareByteCnt;
    _nf_info.u16Reg50_EccCtrl = REG50_ECC_CTRL_INIT_VALUE;

    switch(_nf_info.u16PageByteCnt)
    {
        case 0x0800:
            _nf_info.u16Reg50_EccCtrl |= BIT_NC_PAGE_SIZE_2KB;
            break;
        case 0x1000:
            _nf_info.u16Reg50_EccCtrl |= BIT_NC_PAGE_SIZE_4KB;
            break;
    }

    switch (_nf_info.u16ECCType)
    {
		case ECC_TYPE_RS:
			_nf_info.u16Reg50_EccCtrl |= BIT_NC_ECC_TYPE_RS;
			break;
		case ECC_TYPE_4BIT:
			_nf_info.u16Reg50_EccCtrl &= ~BIT_NC_ECC_TYPE_4b512Bn;
			break;
		case ECC_TYPE_8BIT:
			_nf_info.u16Reg50_EccCtrl |= BIT_NC_ECC_TYPE_8b512B;
			break;
		case ECC_TYPE_12BIT:
			_nf_info.u16Reg50_EccCtrl |= BIT_NC_ECC_TYPE_12b512B;
			break;
		case ECC_TYPE_24BIT1KB:
			_nf_info.u16Reg50_EccCtrl |= BIT_NC_ECC_TYPE_24b1KB;
			break;
   	    case ECC_TYPE_40BIT1KB:
			_nf_info.u16Reg50_EccCtrl |= BIT_NC_ECC_TYPE_40b1KB;    
		default:
		    break;
    }
    _nf_info.u16Reg50_EccCtrl |= BIT_NC_ECCERR_NSTOP;
}

void _NAND_SoftReset(void)
{
    MS_U16 u16Tmp;
    
    HAL_WRITE_UINT16(NC_PATH_CTL,0);
    HAL_WRITE_UINT16(NC_CTRL,0);
    
    NC_ResetFCIE();

    HAL_WRITE_UINT16(NC_PATH_CTL,BIT_NC_EN);
    NC_ConfigContext();
    NC_ReInit();
    NC_Config();


#ifdef CONFIG_MSTAR_KRONUS
    HAL_READ_UINT16(FCIE_NC_CIFD_BASE, u16Tmp);
    HAL_READ_UINT16(FCIE_NC_CIFD_BASE, u16Tmp);
#endif

}


// ===========================================================
void NC_WaitComplete( MS_U16 u16_WaitEvent)
{
	MS_U16 u16_Reg;

	do
	{
		REG_READ_UINT16(NC_MIE_EVENT, u16_Reg);

	}while((u16_Reg & u16_WaitEvent) != u16_WaitEvent );

	REG_W1C_BITS_UINT16(NC_MIE_EVENT, u16_WaitEvent); /*clear events*/


}

void NC_WaitComplete_RM(MS_U16 u16_WaitEvent)
{
	MS_U16 u16_Reg;

	do
	{
		REG_READ_UINT16(NC_R2N_STAT, u16_Reg);

	}while((u16_Reg & u16_WaitEvent) != u16_WaitEvent );

}



void NC_waitFifoClkReady(void)
{
	 MS_U16 u16_Reg;

	do
	{
		REG_READ_UINT16(NC_MMA_PRI_REG, u16_Reg);
		
	}while((u16_Reg & BIT_NC_FIFO_CLKRDY) != BIT_NC_FIFO_CLKRDY);

}

void NC_GetCIFD(MS_U8 *pu8_Buf, MS_U32 u32_CIFDPos, MS_U32 u32_ByteCnt)
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


MS_U32 NC_ResetNandFlash(void)
{
	HAL_WRITE_UINT16(NC_MIE_EVENT,BIT_NC_JOB_END);

	REG_WRITE_UINT16(NC_AUXREG_ADR, AUXADR_INSTQUE);
	REG_WRITE_UINT16(NC_AUXREG_DAT, (ACT_WAITRB << 8) | CMD_0xFF);
	REG_WRITE_UINT16(NC_AUXREG_DAT, ACT_BREAK);

	REG_WRITE_UINT16(NC_CTRL, BIT_NC_JOB_START);

	NC_WaitComplete(BIT_NC_JOB_END);

	return UNFD_ST_SUCCESS;
}


MS_U32  NC_ReadSector_RIUMode(MS_U32 u32_PhyRowIdx, MS_U8 u8_SectorInPage, MS_U16* pu16_DataBuf,MS_U8 *pu8_SpareBuf)
{
	MS_U16 u16_Tmp, u16_i, u16_j, u16_k, u16_Cnt, u16_ByteLoc;

	REG_SET_BITS_UINT16(NC_R2N_CTRL, BIT_R2N_MODE_EN); // enable RIU Mode
	REG_WRITE_UINT16(NC_MIE_EVENT, BIT_NC_R2N_ECC|BIT_NC_R2N_RDY|BIT_NC_JOB_END|BIT_MMA_DATA_END);

	// if Partial Mode (if not 512B/page, set Partial Mode)
	REG_WRITE_UINT16(NC_PART_MODE, BIT_PARTIAL_MODE_EN);
	REG_CLR_BITS_UINT16(NC_PART_MODE, BIT_START_SECTOR_IDX_MASK);
	REG_SET_BITS_UINT16(NC_PART_MODE, u8_SectorInPage<<BIT_START_SECTOR_IDX_SHIFT);

	REG_WRITE_UINT16(NC_JOB_BL_CNT, 1); /* read 1 sector only */
	REG_CLR_BITS_UINT16(NC_MMA_PRI_REG, BIT_NC_DMA_DIR_W);
	NC_waitFifoClkReady();
 
	REG_SET_BITS_UINT16(NC_PATH_CTL, BIT_MMA_EN);

	REG_WRITE_UINT16(NC_AUXREG_ADR, AUXADR_ADRSET);
	REG_WRITE_UINT16(NC_AUXREG_DAT, u8_SectorInPage * _nf_info.u16SectorByteCnt);
	REG_WRITE_UINT16(NC_AUXREG_DAT, u32_PhyRowIdx & 0xFFFF);
	REG_WRITE_UINT16(NC_AUXREG_DAT, u32_PhyRowIdx >> 16);

	REG_WRITE_UINT16(NC_AUXREG_ADR, AUXADR_INSTQUE);
	REG_WRITE_UINT16(NC_AUXREG_DAT, (ADR_C5TFS0<< 8) | (CMD_0x00));

	REG_WRITE_UINT16(NC_AUXREG_DAT, (ACT_WAITRB << 8) | CMD_0x30);
	REG_WRITE_UINT16(NC_AUXREG_DAT, (ACT_BREAK << 8) | ACT_SER_DIN);

	REG_WRITE_UINT16(NC_CTRL,
           (BIT_NC_CIFD_ACCESS|BIT_NC_JOB_START)&~(BIT_NC_IF_DIR_W));

	u16_Cnt = _nf_info.u16SectorByteCnt >> 9; // how many 512B per sector
	for (u16_j=0; u16_j<u16_Cnt; u16_j++)
	{   // read data
		NC_WaitComplete_RM(BIT_RIU_RDY_MMA);

		if (0 == u16_j)
			REG_SET_BITS_UINT16(NC_R2N_CTRL, BIT_R2N_DI_START);

		u16_k = u16_j << 8;

		for (u16_i=0; u16_i<256; u16_i++)
		{
			NC_WaitComplete(BIT_NC_R2N_RDY); 

			REG_READ_UINT16(NC_R2N_DATA_RD, pu16_DataBuf[u16_k + u16_i]);

			REG_SET_BITS_UINT16(NC_R2N_CTRL, BIT_R2N_DI_EN);
		}
	}
  	REG_WRITE_UINT16(NC_R2N_CTRL, BIT_R2N_MODE_EN|BIT_R2N_DI_END);

	// wait for events
	u16_Tmp = 0;
	do{
 		REG_READ_UINT16(NC_MIE_EVENT, u16_Tmp);
	}while(((u16_Tmp & BIT_NC_R2N_ECC)==0) && ((u16_Tmp & (BIT_NC_JOB_END | BIT_MMA_DATA_END)) != (BIT_NC_JOB_END | BIT_MMA_DATA_END)));

    // if ECC Fail
	REG_READ_UINT16(NC_ECC_STAT2, u16_Tmp);
	if ((u16_Tmp & BIT_NC_ECC_FLGA_MASK) == BIT_NC_ECC_FLGA_FAIL)
	  return UNFD_ST_ERR_ECC_FAIL_RM;

	REG_READ_UINT16(NC_MIE_EVENT, u16_Tmp);
	if (u16_Tmp & BIT_NC_R2N_ECC)
	{
		// make CIFD available for SW
		// [CAUTION]: before clear BIT_NC_R2N_ECC, JOB_END would not set.
		REG_W1C_BITS_UINT16(NC_MIE_EVENT, BIT_NC_R2N_ECC);
		NC_WaitComplete(BIT_NC_JOB_END); 

		REG_READ_UINT16(NC_ECC_STAT2, u16_Tmp);
		u16_Cnt = (u16_Tmp & BIT_NC_ECC_CNT_MASK) >> BIT_NC_ECC_CNT_SHIFT;

		for (u16_i=0; u16_i<u16_Cnt; u16_i++)
		{
			REG_CLR_BITS_UINT16(NC_ECC_STAT2, BIT_NC_ECC_SEL_LOC_MASK);
			REG_SET_BITS_UINT16(NC_ECC_STAT2, u16_i<<BIT_NC_ECC_SEL_LOC_SHIFT);
			LABEL_ECC_LOC_READ:
			//REG_READ_UINT16(NC_ECC_LOC, u16_Tmp);
			REG_READ_UINT16(NC_ECC_LOC, u16_j);
			REG_READ_UINT16(NC_ECC_LOC, u16_k);
			if(u16_j != u16_k)
				goto LABEL_ECC_LOC_READ;

			u16_ByteLoc = u16_j >> 3;
			if (u16_ByteLoc < _nf_info.u16SectorByteCnt)
			{
				*(MS_U8*)(pu16_DataBuf+u16_ByteLoc) ^= 1<<(u16_j%8);
			}
		}

    	u16_Tmp = BIT_NC_R2N_ECC;
  	}

	if(0 == (u16_Tmp & BIT_NC_R2N_ECC)){
		NC_WaitComplete(BIT_NC_JOB_END|BIT_MMA_DATA_END);
	}
	
	NC_GetCIFD(pu8_SpareBuf, 0, _nf_info.u16SectorSpareByteCnt);

  	REG_WRITE_UINT16(NC_PART_MODE, 0);
  	REG_CLR_BITS_UINT16(NC_R2N_CTRL, BIT_R2N_MODE_EN);

	return UNFD_ST_SUCCESS;
}

MS_U32 NC_ReadSectors(MS_U32 u32_PhyRowIdx, MS_U16* pu16_DataBuf, MS_U8* pu8_SpareBuf,MS_U32 unit) // unit is the number of sector in the page
{
    MS_U8 u8_i;
    MS_U32 u32_retry;

    for (u8_i=0;u8_i<unit;u8_i++)
    {
        u32_retry = NC_ReadSector_RIUMode(u32_PhyRowIdx,u8_i,pu16_DataBuf+u8_i*(_nf_info.u16SectorByteCnt>>1),pu8_SpareBuf);

        if (u32_retry)
        {
            _PUTC('W');
            return 0;
        }
    }

    return 1;
}


MS_U32 BOOTROM_C_InitMIU_NAND(void)
{
    MS_U32 BINSize,u32_err=0;
    _PUTS("NAND\n\r");
    //asm volatile (" DBGREG  0x0014");
    //asm volatile ("PUTC 'O");
    // BIT6
    //_nf_info.u32OneCol_cfg = 1;
    //************************************
    // switch pad
    //_NAND_SwitchPAD();


    // switch pad
#if defined(CONFIG_MSTAR_KAISER)
    // Disable all pad in
    REG_CLR_BITS_UINT16(reg_allpad_in, BIT15);

    REG_CLR_BITS_UINT16(reg_nf_en, CFG_SD_PAD_MASK|CFG_NAND_PAD_MASK|CFG_EMMC_PAD_MASK);
    REG_SET_BITS_UINT16(reg_nf_en, BIT_NAND_PAD_MODE2);
    REG_SET_BITS_UINT16(reg_fcie2macro_sd_bypass, BIT_FCIE2MACRO_SD_BYPASS);    
    REG_CLR_BITS_UINT16(reg_fcie2macro_sd_bypass, BIT_SD_USE_BYPASS|BIT_EMMC_SWRSTZ_EN|BIT_EMMC_SWRSTZ|BIT_PAD_IN_SEL_SD|BIT_PAD_IN_SEL_SDIO);      
    //REG_CLR_BITS_UINT16(NC_REG_2Fh, BIT8|BIT9|BIT10|BIT11|BIT12);
    //REG_CLR_BITS_UINT16(NC_REG_2Fh, BIT0|BIT4); //add by hill's oponion
    HAL_WRITE_UINT16(NC_REG_2Fh, 0);            //add by hill's oponion
    REG_CLR_BITS_UINT16(FCIE_NC_REORDER, BIT7|BIT14);  
	HAL_WRITE_UINT16(NC_R2N_CTRL, 0); 

    //************************************
    // set clock
    REG_CLR_BITS_UINT16(reg_ckg_fcie, BIT6-1);
    REG_SET_BITS_UINT16(reg_ckg_fcie, NFIE_CLK_27M);
    //!!!!!!NC_LATCH_DATA = 0x2 if NFIE_CLK > 40M !!!!!!
    //HAL_WRITE_UINT16(NC_LATCH_DATA, 0x2);
    //************************************

    _nf_info.u16PageByteCnt = 0x800;
	_nf_info.u16SpareByteCnt = 0x100;
	_nf_info.u16ECCType = NANDINFO_ECC_TYPE;
    _NAND_SoftReset();
    HAL_WRITE_UINT16(NC_SIGNAL, (BIT_NC_WP_AUTO | BIT_NC_WP_H | BIT_NC_CE_AUTO | BIT_NC_CE_H) &
                     ~(BIT_NC_CHK_RB_EDGEn | BIT_NC_CE_SEL_MASK));


    HAL_WRITE_UINT16(NC_CIF_FIFO_CTL,0);

    //REG_SET_BITS_UINT16((FCIE_REG_BASE_ADDR+(0x2F<<REG_OFFSET_SHIFT_BITS)), BIT0);


    //NC_ResetNandFlash
	u32_err = NC_ResetNandFlash();
	if(u32_err)
        _PUTC('D');

    // NC_WaitComplete 
#else
   #error "no such project currently applied to boot from nand"
#endif

#if 0 // leave below codes for dugug usage, which is useful to know nand pass init
    // read ID flow
    HAL_WRITE_UINT16(NC_AUXR_ADR, AUXADR_ADRSET);
    HAL_WRITE_UINT16(NC_AUXR_DAT, 0x00); // Address for Read ID

    HAL_WRITE_UINT16(NC_AUXR_ADR, AUXADR_INSTQUE);
    HAL_WRITE_UINT16(NC_AUXR_DAT,(ADR_C2TIS0<<8)+OP_CMD90_READID);
    HAL_WRITE_UINT16(NC_AUXR_DAT,(OP_A_BREAK<<8)+OP_A_RANDOM_DATA_IN);

    HAL_WRITE_UINT16(NC_AUXR_ADR, AUXADR_RAN_CNT);

    //HAL_WRITE_UINT16(NC_AUXR_DAT,0x6);  // read 5 bytes
    HAL_WRITE_UINT16(NC_AUXR_DAT,30);  
    HAL_WRITE_UINT16(NC_AUXR_DAT,0x0);

    HAL_WRITE_UINT16(NC_CTRL,R_NC_CIFC_ACCESS+R_NC_JOB_START);

    //for(;j<10000;j++){
    //    _PUTC('_');
   // }

    _PUTW(*(((volatile MS_U32 *)CIFIFO_CMD_ADDR)));
   // _PUTC(' ');
    _PUTW(*(((volatile MS_U32 *)(CIFIFO_CMD_ADDR+4))));
    //_PUTC(' ');
    _PUTW(*(((volatile MS_U32 *)(CIFIFO_CMD_ADDR+8))));
    //_PUTC(' ');
    _PUTW(*(((volatile MS_U32 *)(CIFIFO_CMD_ADDR+12))));
    //_PUTC(' ');
    _PUTW(*(((volatile MS_U32 *)(CIFIFO_CMD_ADDR+16))));
    //_PUTC(' ');
    _PUTW(*(((volatile MS_U32 *)(CIFIFO_CMD_ADDR+20))));

    //_NAND_wNCJOBEND(&_nf_info,0);
    _NAND_wNCJOBEND(0);

    _NAND_SoftReset(); 
#endif

    MS_U32 row_pos = 0; 
    MS_U32 i=0;
    MS_U32 miu_page_size = 0;

    //read first page
    REG_SET_BITS_UINT16(NC_SPARE, BIT_NC_HW_AUTO_RANDOM_CMD_DISABLE);
    while (1)
    {
        BINSize = NC_ReadSectors(row_pos,_nf_info.pBuf,_nf_info.pu8_SpareBuf,1);
		//_PUTDW(*((MS_U32*)(_nf_info.pBuf)));
        //_PUTC(' ');
        //_PUTDW(row_pos);
        //_PUTC('\n');_PUTC('\r');
        
        if ((BINSize) && (*((MS_U32*)(_nf_info.pBuf)) == CISIDTAG))
        {
            _PUTS("CISTAG");
            _nf_info.u16PageByteCnt = ((NAND_FLASH_INFO_t*)(_nf_info.pBuf))->u16_PageByteCnt;
            _nf_info.u16BlockPageCnt = ((NAND_FLASH_INFO_t*)(_nf_info.pBuf))->u16_BlkPageCnt;
            _nf_info.u16SpareByteCnt = ((NAND_FLASH_INFO_t*)(_nf_info.pBuf))->u16_SpareByteCnt;
            _nf_info.u16ECCType = ((NAND_FLASH_INFO_t*)(_nf_info.pBuf))->u16_ECCType;

            BINSize = NC_ReadSectors(row_pos+10,_nf_info.pBuf,_nf_info.pu8_SpareBuf,1);
            BINSize &= NC_ReadSectors(row_pos+11,_nf_info.pBuf+0x200,_nf_info.pu8_SpareBuf,1);
            BINSize &= NC_ReadSectors(row_pos+12,_nf_info.pBuf+0x400,_nf_info.pu8_SpareBuf,1);
            BINSize &= NC_ReadSectors(row_pos+13,_nf_info.pBuf+0x600,_nf_info.pu8_SpareBuf,1);
            if (BINSize == 1)
            {
                break;
            }
            else
            {
                _PUTC('E');
                break;
            }
        }
        _PUTC('.');

        row_pos += 32;
        if (row_pos == 640){  //2*10*32
            while(1){};
        }
    }
    REG_CLR_BITS_UINT16(NC_SPARE, BIT_NC_HW_AUTO_RANDOM_CMD_DISABLE);
    //_nf_info.u16PageNum = *((MS_U32*)(_nf_info.pSprBuf+2));
    //_PUTDW(_nf_info.u16PageNum);
    //_PUTDW(_nf_info.u16PageSize);
    //_PUTC('B');
    //BOOTROM_C_InitMIU_O();
    //flush_cache();
    BOOTROM_C_InitParams(_nf_info.pBuf);

    NC_ConfigContext();
    NC_ReInit();
    NC_Config();

    arm_init_for_miu_access();

    row_pos = 2*_nf_info.u16BlockPageCnt; 

    _var_LDR_run_base = *( (MS_U32*)(_nf_info.pBuf+0x7fc) );
    _var_LDR_size = *( (MS_U32*)(_nf_info.pBuf+0x7fe) );

    
    
    MS_U16* MIUPos = (MS_U16*)_var_LDR_run_base;  

    while (1)
    {
        NC_ReadSectors(row_pos,MIUPos,_nf_info.pu8_SpareBuf, _nf_info.u16PageSectorCnt);
        if (*((MS_U32*)(MIUPos)) == LDRIDTAG)
        {
            _PUTS("LDRTAG ");
            _var_LDR_size        -= 0x100;  //BINSize = *((MS_U32*)(MIUPos+0xa));  
            _var_LDR_entry_point = *((MS_U32*)(MIUPos+0x2));//MIU pos is the fake position and the offset
            _var_LDR_stack_end   = *((MS_U32*)(MIUPos+0x12));
            MIUPos = (MS_U16*) _var_LDR_entry_point;     

            break;
        }
        
        row_pos += _nf_info.u16BlockPageCnt;
         
        _PUTC('.');
        
        if (row_pos == 20*_nf_info.u16BlockPageCnt*_nf_info.u16PageSectorCnt){
            while(1){};
        }
    }

    i=0;

    while(_var_LDR_size)
    {
        NC_ReadSectors(row_pos+i,MIUPos,_nf_info.pu8_SpareBuf, _nf_info.u16PageSectorCnt);
        if (i==0)
        {
            if (*((MS_U32*)(MIUPos)) != LDRIDTAG)
            {
                row_pos += _nf_info.u16BlockPageCnt;
            }
            else
            {
                i++;
            }
            continue;
        }

        i++;
        MIUPos += (_nf_info.u16PageByteCnt>>1);
        _var_LDR_size -= ((_var_LDR_size < _nf_info.u16PageByteCnt)?_var_LDR_size:_nf_info.u16PageByteCnt);
        if (i==_nf_info.u16BlockPageCnt)
        {
            row_pos += _nf_info.u16BlockPageCnt;
            i=0;
        }
    }

}

//#endif
static MS_U32 _HAL_SERFLASH_WaitReadDataRdy(void)
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

static MS_U32 _HAL_SERFLASH_WaitWriteDone(void)
{
    MS_U32 bRet = 0;
    MS_U32 u32Timer;

    SER_FLASH_TIME(u32Timer, SERFLASH_SAFETY_FACTOR*5);
    do
    {
        ISP_WRITE(REG_ISP_SPI_COMMAND, ISP_SPI_CMD_RDSR); // RDSR

        ISP_WRITE(REG_ISP_SPI_RDREQ, ISP_SPI_RDREQ); // SPI read request

        if (_HAL_SERFLASH_WaitReadDataRdy() == 0 )
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

MS_U32 HAL_SERFLASH_ReadID()
{
#ifdef CONFIG_LD_BOOT_FROM_OTP
    //_PUTS("ID=0\n");
    //return 0;
    #if defined(CONFIG_MSTAR_KAISER)
        REG_SET_BITS_UINT16(PM_SLEEP_0x39,0x40);
        //MS_U32 u32_chip_id=0;
        //HAL_READ_UINT16(CHIPTOP_CHIP_ID,u32_chip_id);
        //_PUTW(u32_chip_id);
        //if (u32_chip_id == 0x0101)  REG_SET_BITS_UINT16(PM_SLEEP_0x39,0x20);
    #endif

    // HW doesn't support ReadID on MX/ST flash; use trigger mode instead.
    MS_U32 bRet = 0;
    MS_U32 u32I;
    MS_U32 u32Size;
    MS_U8 u8ptr ;

    ISP_WRITE(REG_ISP_PASSWORD, 0xAAAA);//_HAL_ISP_Enable();

    if (!_HAL_SERFLASH_WaitWriteDone())
    {
        goto HAL_SERFLASH_ReadID_return;
    }
    bRet = 1;

HAL_SERFLASH_ReadID_return:
    ISP_WRITE(REG_ISP_SPI_CECLR, ISP_SPI_CECLR); // SPI CEB dis

    ISP_WRITE(REG_ISP_PASSWORD, 0x5555);//_HAL_ISP_Disable();

    //_HAL_SPI_Rest(); // to prevent BDMA fail
    ISP_WRITE_MASK(REG_ISP_CHIP_RST, SFSH_CHIP_RESET, SFSH_CHIP_RESET_MASK);
    ISP_WRITE_MASK(REG_ISP_CHIP_RST, SFSH_CHIP_NOTRESET, SFSH_CHIP_RESET_MASK);
    return bRet;
#elif CONFIG_LD_BOOT_OTP_SIMULATE_ON_SPINOR
    return 0;
#else
    // if we dont plan to use OTP, no necessary for SPI boot
    return 1;
#endif

}



void BOOTROM_C_InitMIU_ENTRY(void)
{
    _PUTS("\n\rENTRY_");
    if (HAL_SERFLASH_ReadID())
    {
        _boot_flow_mode = BOOT_FLOW_IS_SPI;
        BOOTROM_C_InitMIU_SPI();
    }
#if defined(CONFIG_LD_BOOT_FROM_OTP)||defined(CONFIG_LD_BOOT_OTP_SIMULATE_ON_SPINOR)
    else if (check_emmc_exist(((MS_U32)(_nf_info.pBuf)) >> 3,1))
    {
        //check_emmc_exist(((MS_U32)(_nf_info.pBuf)) >> 3,1);
        _boot_flow_mode = BOOT_FLOW_IS_EMMC;
        BOOTROM_C_InitMIU_EMMC();
    }
#endif    
    else
    {
        _boot_flow_mode = BOOT_FLOW_IS_NAND;
        BOOTROM_C_InitMIU_NAND();
    }
}

