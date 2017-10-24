    .include "cpu.inc"

    .section .text
    .set    noreorder

    .global enable_cache_asm
enable_cache_asm:
    CPU_InitIC
    CPU_InitDC

    # Enable cache
    mfc0    $8,     $16
    ehb
    and     $8,     (~M_ConfigK0)
    li      $8,     (K_CacheAttrCN)
    mtc0    $8,     $16
    ehb

    sync
    ehb
    nop
    nop
    nop
    nop

    jr $31
    nop

    .global disable_cache_asm
disable_cache_asm:
    CPU_InitIC
    CPU_InitDC

    # Disable cache
    CPU_DisCache

    sync
    ehb
    nop
    nop
    nop
    nop

    jr $31
    nop


    .global flush_cache_asm
flush_cache_asm:
    CPU_FlushDC
    CPU_InitIC

    sync
    ehb
    nop
    nop
    nop
    nop

    jr $31
    nop
