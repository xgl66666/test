///////////////////////////////////////////////////////////////////////////////////////////////////
// @file   LzmaDec.c
// @author MStar Semiconductor Inc.
// @brief  Decompression Module
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------
#include "LzmaDec.h"
#include "zipbin.h"

//-------------------------------------------------------------------------------------------------
// Local Defines
//-------------------------------------------------------------------------------------------------
//#define TEST_DECOMPRESS_ON_HOST

// Uranus4
#define UART_BASE         (0xBF201300)
#define UART_LSR_THRE     (0x20)
#define UART_LSR_TEMT     (0x40)
#define UART_BOTH_EMPTY   (UART_LSR_TEMT | UART_LSR_THRE)
#define UART_RX           (*((volatile unsigned int *)(UART_BASE + (0 * 8))))
#define UART_TX           (*((volatile unsigned int *)(UART_BASE + (0 * 8))))
#define UART_LSR          (*((volatile unsigned int *)(UART_BASE + (5 * 8))))


//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Global Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Local Variables
//-------------------------------------------------------------------------------------------------
static unsigned char *g_alloc_buf;

//-------------------------------------------------------------------------------------------------
// Local Function Prototypes
//-------------------------------------------------------------------------------------------------
void uart_putc(int c)
{
    while((UART_LSR & UART_LSR_THRE) != UART_LSR_THRE)
        ;
    UART_TX = c;
    while((UART_LSR & UART_BOTH_EMPTY) != UART_BOTH_EMPTY)
        ;
}

void uart_puts(char *s)
{
    while(*s)
        uart_putc(*s++);
}

static void *SzAlloc(void *p, size_t size)
{
    unsigned char *x = g_alloc_buf;
    p = p;
    g_alloc_buf += size;
    return x;
}

static void SzFree(void *p, void *address)
{
    p = p;
    address = address;
}

void *memcpy(void *dest, const void *src, size_t n)
{
    unsigned char *d = dest;
    const unsigned char *s = src;
    while(n--)
        *d++ = *s++;
    return dest;
}

//-------------------------------------------------------------------------------------------------
// Decompress API
// @param inStream  \b IN: ptr to compressed data
// @param outStream \b OUT: ptr to decompressed data
// @param internalMem  \b IN: ptr decompression buffer (4-Byte aligned). see below for required size
// @return <0 : error
// @return otherwise: decompressed data size
// @note   Assign a0, a1, a2 and Set sp first before calling this subroutine.
// @note   Set optimization level = ?
//-------------------------------------------------------------------------------------------------
int LzmaDec(const unsigned char *in, unsigned int in_size, unsigned char *out, unsigned char *alloc_buf)
{
    const unsigned char *header = in;
    unsigned long long unpack_size;
    int i;
    ELzmaStatus status;
    SRes res;

    uart_puts("\r\nLZMA Decompression...");

    SizeT in_size_pure = in_size - (LZMA_PROPS_SIZE + 8);
    SizeT out_size;
    ISzAlloc g_Alloc = { SzAlloc, SzFree };

    g_alloc_buf = alloc_buf;

    unpack_size = 0;
    for (i = 0; i < 8; i++)
        unpack_size += (unsigned long long)header[LZMA_PROPS_SIZE + i] << (i * 8);
    out_size = unpack_size;

    //printf("in_size=%d out_size=%d unpack_size=%d\n", in_size, out_size, unpack_size);
    res = LzmaDecode(out, &out_size, in + (LZMA_PROPS_SIZE + 8), &in_size_pure, header, LZMA_PROPS_SIZE, LZMA_FINISH_ANY, &status, &g_Alloc);
    if(res != SZ_OK) {
        uart_puts("fail\r\n");
        return -1;
    }
    //printf("res=%d status=%d out_size=%d\n", res, status, out_size);

    uart_puts("ok\r\n");
    return unpack_size;
}


#ifndef TEST_DECOMPRESS_ON_HOST

static unsigned char alloc_buf[1024 * 64];
extern unsigned char _ld_zipbin_start[], _ld_zipbin_end[];

void enable_cache(void)
{
    void enable_cache_asm(void);
    asm volatile(
            "jal enable_cache_asm\n"
            "nop\n" : : :
            "$1", "$2", "$3", "$4", "$5", "$6", "$7",
            "$8", "$9", "$10", "$11", "$12", "$13", "$14", "$15",
            "$16", "$17", "$18", "$19", "$20", "$21", "$22", "$23",
            "$24", "$25", "$26", "$27", "$29", "$31",
            "memory");
}

void disable_cache(void)
{
    void disable_cache_asm(void);
    asm volatile(
            "jal disable_cache_asm\n"
            "nop\n" : : :
            "$1", "$2", "$3", "$4", "$5", "$6", "$7",
            "$8", "$9", "$10", "$11", "$12", "$13", "$14", "$15",
            "$16", "$17", "$18", "$19", "$20", "$21", "$22", "$23",
            "$24", "$25", "$26", "$27", "$29", "$31",
            "memory");
}

void flush_cache(void)
{
    void flush_cache_asm(void);
    asm volatile(
            "jal flush_cache_asm\n"
            "nop\n" : : :
            "$1", "$2", "$3", "$4", "$5", "$6", "$7",
            "$8", "$9", "$10", "$11", "$12", "$13", "$14", "$15",
            "$16", "$17", "$18", "$19", "$20", "$21", "$22", "$23",
            "$24", "$25", "$26", "$27", "$29", "$31",
            "memory");
}

//-------------------------------------------------------------------------------------------------
// Application program to test the Decompression API on target
// @return None
//-------------------------------------------------------------------------------------------------
void ReInitHardwareMini(void)
{
    flush_cache();
    disable_cache();
// ---- put your specific HW init code here ----
#ifdef ENABLE_KRONUS_U04
    *(volatile MS_U16*)(0xBF200000+(0x0B3E<<1)) = 0x18; // set clk_miu = 288M
#endif
// ---- end of put your specific HW init code here ----
    enable_cache();

}

int main(void) __attribute__((section(".text.entry")));
int main(void)
{
    unsigned char *base = (void *)ZIP_APP_BIN;
    volatile void (*entry)(void) = (void *)ZIP_APP_BIN_ENTRY;

    asm __volatile__ (
        "la      $sp,    __stack_begin;"
    );

#ifdef ENABLE_KRONUS_U04
    ReInitHardwareMini();
#endif



    if(LzmaDec(_ld_zipbin_start, _ld_zipbin_end - _ld_zipbin_start, base, alloc_buf) < 0)
    {
        #if 0
        while(1)
            ;
        #else
        asm __volatile__ (
        "1:;"

        // reset chip
        // #define REG_PM_SLEEP_BASE    0xbf001C00
        //  *(volatile unsigned long*)(REG_PM_SLEEP_BASE + 0xB8) = 0xff;    // reg_top_sw_rst.
        "lui	$26,0xbf00;"
        "ori	$27,$26,0x1cb8;"
        "li	$26,255;"
        "sw	$26,0($27);"
        // *(volatile unsigned long*)(REG_PM_SLEEP_BASE + 0xB8) = 0x79;    // reg_top_sw_rst
        "lui	$26,0xbf00;"
        "ori	$27,$26,0x1cb8;"
        "li	$26,121;"
        "sw	$26,0($27);"
        // end reset chip

        "2:;"
        "li	$26,121;"
        "bnez    $26,    2b;" // while(TRUE)


        "li	$26,121;"
        "bnez    $26,    1b;" // while(TRUE)

        // reset chip
        //  *(volatile unsigned long*)(REG_PM_SLEEP_BASE + 0xB8) = 0xff;    // reg_top_sw_rst.
        "lui	$26,0xbf00;"
        "ori	$27,$26,0x1cb8;"
        "li	$26,255;"
        "sw	$26,0($27);"
        // *(volatile unsigned long*)(REG_PM_SLEEP_BASE + 0xB8) = 0x79;    // reg_top_sw_rst
        "lui	$26,0xbf00;"
        "ori	$27,$26,0x1cb8;"
        "li	$26,121;"
        "sw	$26,0($27);"
        // end reset chip

        "3:;"
        "li	$26,121;"
        "bnez    $26,    3b;" // while(TRUE)
        );
        #endif
    }

    flush_cache();

    entry();
    return 0;
}

#else

//-------------------------------------------------------------------------------------------------
// Application program to test the Decompression API on Host using Visual C++
// @param  arg1 \b IN: compressed file name
// @param  arg2 \b OUT: decompressed file name
// @return 0: succeed
// @return -1: fail
//-------------------------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    FILE *fin, *fout;
    unsigned char *in, *out;
    unsigned int in_size;
    char buf[1024 * 32];

    if(argc != 3)
        return 1;

    if((fin = fopen(argv[1], "rb")) == NULL) {
        perror(argv[1]);
        return 1;
    }

    if((fout = fopen(argv[2], "wb")) == NULL) {
        perror(argv[2]);
        return 1;
    }

    fseek(fin, 0, SEEK_END);
    in_size = ftell(fin);
    rewind(fin);

    if((in = malloc(in_size)) == NULL) {
        perror("malloc");
        return 1;
    }

    if((out = malloc(in_size * 10)) == NULL) {
        perror("malloc");
        return 1;
    }

    if(fread(in, 1, in_size, fin) != in_size) {
        perror("fread");
        return 1;
    }

    int e = LzmaDec(in, in_size, out, buf);
    printf("unpack size = %d\n", e);

    if(e >= 0) {
        if(fwrite(out, 1, e, fout) != e) {
            perror("fwrite");
            return 1;
        }
    }

    return 0;
}

#endif
