#ifndef _AEON_CACHE_H
#define _AEON_CACHE_H

#ifdef __KERNEL__
#include <asm/processor.h>

#if CONFIG_AEON_DC_LINE!=CONFIG_AEON_IC_LINE
#  warning "instruction cachline is different than data cacheline"
#endif

//#define L1_CACHE_BYTES CONFIG_AEON_DC_LINE
#define L1_CACHE_BYTES CFG_DCACHE_LINE_SIZE

#if L1_CACHE_BYTES==16
#  define L1_CACHE_SHIFT     4
#elif L1_CACHE_BYTES==32
#  define L1_CACHE_SHIFT     5
#else
#  error "please define L1_CACHE_SHIFT, and if neccessery correct L1_CACHE_SHIFT_MAX"
#endif

/* Maximum cache line this arch supports */
#  define L1_CACHE_SHIFT_MAX 5

#endif /* __KERNEL__ */

#endif /* __AEONR2_CACHE_H__ */
