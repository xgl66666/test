#include <config.h>

#define __TEXT_ALIGN      .align 4

#define ENTRY(name)                 \
        __TEXT_ALIGN;               \
        .globl _##name;             \
        .type _##name,@function;    \
_##name:

.section ".text", "ax"

ENTRY(mhal_dcache_flush)
    l.movhi r6,hi(CFG_DCACHE_SIZE)
    l.ori   r6, r0, CFG_DCACHE_SIZE
    l.movhi r7,hi(CFG_DCACHE_WAYS)
    l.ori   r7, r0, CFG_DCACHE_WAYS
    l.movhi r5,hi(CFG_DCACHE_LINE_SIZE)
    l.ori   r5, r0, CFG_DCACHE_LINE_SIZE

    ## (RISC32_DCACHE_SIZE / RISC32_DCACHE_WAYS - RISC32_DCACHE_LINE_SIZE)
    l.divu  r6,r6,r7
    l.sub   r6,r6,r5

    l.sfleu r4,r6               ## Set flag if r4 <= r6
    l.add   r4,r3,r4            ## r4 = r3 + r4
    l.bf    disable_interrupt
    l.add   r4,r3,r6

disable_interrupt:
    l.addi  r7,r0,SPR_SR
    l.mfspr r23,r7,0            ## save SR in r23
    l.addi  r6,r0,-7            ## ~(SPR_SR_IEE | SPR_SR_TEE)
    l.and   r6,r23,r6
    l.mtspr r7,r6,0

flush_line:
    l.flush_line       0(r3), 0
    l.flush_line       0(r3), 1
    l.flush_line       0(r3), 2
    l.flush_line       0(r3), 3
    l.invalidate_line  0(r3), 0
    l.invalidate_line  0(r3), 1
    l.invalidate_line  0(r3), 2
    l.invalidate_line  0(r3), 3

    l.sfltu r3,r4
    l.add   r3,r3,r5
    l.bf    flush_line

    l.syncwritebuffer

    l.mtspr r7,r23,0        ## restore interrupt mask
    l.jr    r9

