////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2009 MStar Semiconductor, Inc.
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
////////////////////////////////////////////////////////////////////////////////

#ifndef __MHAL_ARCH_H__
#define __MHAL_ARCH_H__

/// @file mhal_arch.h
///
/// @brief MStar HAL for risc32 architecture
/// @par Example: register handler for trap exception
/// @code
/// void trap_isr(MHAL_SavedRegisters *regs, unsigned long data);
/// mhal_exception_attach(E_EXCEPTION_TRAP, trap_isr, 0 /* data pass to isr */);
/// @endcode
///
/// @par Example: ciritical section for interrupt
/// @code
/// // method 1 (suggested)
/// mhal_interrupt_lock();
/// // ... critical section ...
/// mhal_interrupt_unlock();
///
/// // method 2
/// int old = mhal_interrupt_disable();
/// // ... critical section ...
/// mhal_interrupt_restore(old);
/// @endcode

#ifdef LIBPRANA
#include <cpu/risc32_spr.h>
#include <cpu/risc32_util.h>
#endif

#if 1//defined (__mips__)

#ifdef CHIP_FAMILY_TYPE
//#include "chiptype.h"
#if (CHIP_FAMILY_TYPE == CHIP_FAMILY_S7)
#define NONCACHABLE(_x_)    ((_x_) | 0xC0000000)
#else
/// convert address to corresponding noncacheable address
#define NONCACHABLE(_x_)    ((_x_) | 0xA0000000)
#endif
#endif

#if (CHIP_FAMILY_TYPE == CHIP_FAMILY_S7)
    #define MIPS_CLOCK_FREQ     504000000
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_S7LD)
    #define MIPS_CLOCK_FREQ     312000000
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_S7LA)
    #define MIPS_CLOCK_FREQ     312000000
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_S8)
    #define MIPS_CLOCK_FREQ     504000000
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_S7ML)
    #define MIPS_CLOCK_FREQ     504000000
#endif

//#include "datatype.h"

typedef enum {
    E_EXCEPTION_DATA_TLBERROR_ACCESS = 1,    // TLB modification exception
    E_EXCEPTION_DATA_TLBMISS_ACCESS,         // TLB miss (Load or IFetch)
    E_EXCEPTION_DATA_TLBMISS_WRITE,          // TLB miss (Store)
    E_EXCEPTION_DATA_UNALIGNED_ACCESS,       // Address error (Load or Ifetch)
    E_EXCEPTION_DATA_UNALIGNED_WRITE,        // Address error (store)
    E_EXCEPTION_CODE_ACCESS,                 // Bus error (Ifetch)
    E_EXCEPTION_DATA_ACCESS,                 // Bus error (data load or store)
    E_EXCEPTION_SYSTEM_CALL,                 // System call
    E_EXCEPTION_INSTRUCTION_BP,              // Break point
    E_EXCEPTION_ILLEGAL_INSTRUCTION,         // Reserved instruction
    E_EXCEPTION_COPROCESSOR,                 // Coprocessor unusable
    E_EXCEPTION_OVERFLOW,                    // Arithmetic overflow
    E_EXCEPTION_RESERVED_13,                 // Reserved
    E_EXCEPTION_DIV_BY_ZERO,                 // Division-by-zero [reserved vector]
    E_EXCEPTION_FPU,                         // Floating point exception
    E_EXCEPTION_MAX = E_EXCEPTION_FPU,
} MHAL_EXCEPTION_TYPE;

typedef enum {
    E_INTERRUPT_02 = 0,
    E_INTERRUPT_03,
    E_INTERRUPT_04,
    E_INTERRUPT_05,
    E_INTERRUPT_06,
    E_INTERRUPT_07,
} MHAL_INTERRUPT_TYPE;

// Aliases for interrupt number
#define E_INTERRUPT_FIQ         E_INTERRUPT_03
#define E_INTERRUPT_IRQ         E_INTERRUPT_02
#define E_INTERRUPT_TICK_TIMER  E_INTERRUPT_07
#define E_INTERRUPT_MAX         E_INTERRUPT_07

typedef struct
{
    // These are common to all saved states
    unsigned long    d[32];          /* Data regs                    */
    unsigned long    hi;             /* hi word of mpy/div reg       */
    unsigned long    lo;             /* lo word of mpy/div reg       */

    // The status register contains the interrupt-enable bit which needs
    // to be preserved across context switches.
    unsigned long    sr;             /* Status Reg                   */

    // These are only saved for exceptions and interrupts
    unsigned long    vector;         /* Vector number                */
    unsigned long    pc;             /* Program Counter              */

    // These are only saved for exceptions, and are not restored
    // when continued.
    unsigned long    cause;          /* Exception cause register     */
    unsigned long    badvr;          /* Bad virtual address reg      */

} MHAL_SavedRegisters;

#else //#if defined (__mips__)

#ifdef CHIP_FAMILY_TYPE
#include "chiptype.h"
#if (CHIP_FAMILY_TYPE == CHIP_FAMILY_S7)
#define NONCACHABLE(_x_)    ((_x_) | 0xC0000000)
#else
/// convert address to corresponding noncacheable address
#define NONCACHABLE(_x_)    ((_x_) | 0x80000000)
#endif
#endif

#include "datatype.h"

/// exception type for risc32
typedef enum {
    E_EXCEPTION_BUS_ERROR = 2,
    E_EXCEPTION_DATA_PAGE_FAULT,
    E_EXCEPTION_INSTRUCTION_PAGE_FAULT,
    E_EXCEPTION_TICK_TIMER,             ///< tick timer, do not use directly
    E_EXCEPTION_UNALIGNED_ACCESS,
    E_EXCEPTION_ILLEGAL_INSTRUCTION,
    E_EXCEPTION_EXTERNAL_INTERRUPT,     ///< external interrupt, do not use directly
    E_EXCEPTION_DTLB_MISS,
    E_EXCEPTION_ITLB_MISS,
    E_EXCEPTION_RANGE,
    E_EXCEPTION_SYSCALL,                ///< caused by l.sys
    E_EXCEPTION_RESERVED,
    E_EXCEPTION_TRAP,                   ///< caused by l.trap
    E_EXCEPTION_MAX = E_EXCEPTION_TRAP,
} MHAL_EXCEPTION_TYPE;

/// interrupt type for PIC on external interrupt exception including tick_timer
typedef enum {
    E_INTERRUPT_TICK_TIMER, //< risc32 builtin tick timer
    E_INTERRUPT_00 = 1,     //< PIC interrupt start from 1 for handler performance
    E_INTERRUPT_01,
    E_INTERRUPT_02,
    E_INTERRUPT_03,
    E_INTERRUPT_04,
    E_INTERRUPT_05,
    E_INTERRUPT_06,
    E_INTERRUPT_07,
    E_INTERRUPT_08,
    E_INTERRUPT_09,
    E_INTERRUPT_10,
    E_INTERRUPT_11,
    E_INTERRUPT_12,
    E_INTERRUPT_13,
    E_INTERRUPT_14,
    E_INTERRUPT_15,
    E_INTERRUPT_16,
    E_INTERRUPT_17,
    E_INTERRUPT_18,
    E_INTERRUPT_19,
    E_INTERRUPT_20,
    E_INTERRUPT_21,
    E_INTERRUPT_22,
    E_INTERRUPT_23,
    E_INTERRUPT_24,
    E_INTERRUPT_25,
    E_INTERRUPT_26,
    E_INTERRUPT_27,
    E_INTERRUPT_28,
    E_INTERRUPT_29,
    E_INTERRUPT_30,
    E_INTERRUPT_31,
} MHAL_INTERRUPT_TYPE;

// Aliases for interrupt number
#define E_INTERRUPT_FIQ         E_INTERRUPT_02
#define E_INTERRUPT_IRQ         E_INTERRUPT_03
#define E_INTERRUPT_UART        E_INTERRUPT_19
#define E_INTERRUPT_MAX         E_INTERRUPT_31

/// task context used for context switch
typedef struct
{
    unsigned long   r1;         ///< stack pointer
    unsigned long   r2;
    unsigned long   r3;         ///< parameter 0
    unsigned long   r9;         ///< link register
    unsigned long   r10;        ///< frame pointer
    unsigned long   r12;
    unsigned long   r14;
    unsigned long   r16;
    unsigned long   r18;
    unsigned long   r20;
    unsigned long   r22;
    unsigned long   r24;
    unsigned long   r26;
    unsigned long   r28;
    unsigned long   r30;
    unsigned long   sr;
} MHAL_Context;

/// interrupt/exception context
typedef struct
{
    unsigned long   r[32];          ///< GPR registers
#ifdef __AEONR2__
    unsigned long   machi2;         // Highest 32-bits of new 32x32=64 multiplier
#endif
    unsigned long   machi;          // High and low words of
    unsigned long   maclo;          //   multiply/accumulate reg

    // These are only saved for exceptions and interrupts
    int             vector;         ///< vector number
    int             sr;             ///< status register
    unsigned long   pc;             ///< program counter

    // Saved only for exceptions, and not restored when continued:
    // Effective address of instruction/data access that caused exception
    unsigned long   eear;           ///< exception effective address
} MHAL_SavedRegisters;

#endif

/// interrupt service routine prototype
typedef void (*mhal_isr_t)(MHAL_SavedRegisters *regs, unsigned long vector);

//MST_BEGIN_DECL

/////////////////////////////////////////////////////////////////////////////
/// @brief register ISR of exception
/// @param <IN>     \b expt_num     : exception type
/// @param <IN>     \b isr          : exception interrupt service routine
/// @param <IN>     \b data         : user data will be passed to ISR
/// @see   mhal_exception_detach
/////////////////////////////////////////////////////////////////////////////
void mhal_exception_attach(MHAL_EXCEPTION_TYPE expt_num, mhal_isr_t isr, unsigned long data);

/////////////////////////////////////////////////////////////////////////////
/// @brief unregister ISR of exception
/// @param <IN>     \b expt_num     : exception type
/// @see   mhal_exception_attach
/////////////////////////////////////////////////////////////////////////////
void mhal_exception_detach(MHAL_EXCEPTION_TYPE expt_num);

/////////////////////////////////////////////////////////////////////////////
/// @brief disable interrupt controller and timer interrupt
/// @return old status
/// @see   mhal_interrupt_restore
/// @see   mhal_interrupt_enable
/////////////////////////////////////////////////////////////////////////////
int  mhal_interrupt_disable(void);

/////////////////////////////////////////////////////////////////////////////
/// @brief enable interrupt controller and timer interrupt
/// @see   mhal_interrupt_disable
/////////////////////////////////////////////////////////////////////////////
void mhal_interrupt_enable(void);

/////////////////////////////////////////////////////////////////////////////
/// @brief enable interrupt controller and timer interrupt
/// @see   mhal_interrupt_disable
/////////////////////////////////////////////////////////////////////////////
void mhal_interrupt_restore(int old);

/////////////////////////////////////////////////////////////////////////////
/// mask a source on interrupt controller
/////////////////////////////////////////////////////////////////////////////
void mhal_interrupt_mask(MHAL_INTERRUPT_TYPE intr_num);

/////////////////////////////////////////////////////////////////////////////
/// unmask a source on interrupt controller
/////////////////////////////////////////////////////////////////////////////
void mhal_interrupt_unmask(MHAL_INTERRUPT_TYPE intr_num);

/////////////////////////////////////////////////////////////////////////////
/// mask all interrupt sources on interrupt controller
/////////////////////////////////////////////////////////////////////////////
void mhal_interrupt_mask_all(void);

/////////////////////////////////////////////////////////////////////////////
/// lock interrupt, mhal_interrupt_lock() and mhal_interrupt_unlock()
/// should be used in pair, and can be nested.
/////////////////////////////////////////////////////////////////////////////
void mhal_interrupt_lock(void);

/////////////////////////////////////////////////////////////////////////////
/// unlock interrupt
/////////////////////////////////////////////////////////////////////////////
void mhal_interrupt_unlock(void);

/////////////////////////////////////////////////////////////////////////////
/// @brief register ISR of interrupt contoller
/// @param <IN>     \b expt_num     : interrupt type
/// @param <IN>     \b isr          : interrupt service routine
/// @param <IN>     \b data         : user data will be passed to ISR
/// @see   mhal_interrupt_detach
/////////////////////////////////////////////////////////////////////////////
void mhal_interrupt_attach(MHAL_INTERRUPT_TYPE intr_num, mhal_isr_t isr, unsigned long data);

/////////////////////////////////////////////////////////////////////////////
/// @brief unregister ISR of interrupt controller
/// @param <IN>     \b expt_num     : interrupt type
/// @see   mhal_interrupt_attach
/////////////////////////////////////////////////////////////////////////////
void mhal_interrupt_detach(MHAL_INTERRUPT_TYPE intr_num);

/////////////////////////////////////////////////////////////////////////////
/// please note original variables or parameters in current stack frame will be lost
/// @brief set stack of current context
/// @param <IN>     \b stack:   stack pointer (high address)
/////////////////////////////////////////////////////////////////////////////
void mhal_stack_init(void *stack);

/////////////////////////////////////////////////////////////////////////////
/// @brief flush dcache
/// @param <IN>     \b u32Base: memory region base address
/// @param <IN>     \b u32Size: memory region size
/////////////////////////////////////////////////////////////////////////////
void mhal_dcache_flush(unsigned long u32Base, unsigned long u32Size);

/// task main function prototype
typedef int (mhal_task_func)(int arg);

#if 0//defined (__aeon__)

/////////////////////////////////////////////////////////////////////////////
/// @brief setup context structure for task creation
/// @param <IN>     \b stkptr:  stack pointer (bottom: low address)
/// @param <IN>     \b stklen:  stack size
/// @param <IN>     \b start:   task main function
/// @param <IN>     \b arg:     argument for task
/////////////////////////////////////////////////////////////////////////////
MHAL_Context *mhal_context_setup(unsigned long *stkptr, int stklen, mhal_task_func start, int arg);

/////////////////////////////////////////////////////////////////////////////
/// @brief switch to new context and save current context to stack
/// @param <IN>     \b pNextCtx:        new context's stack pointer to context
/// @param <IN>     \b pSavedCtxPtr:    pointer to variable that is used to store
///                                     the pointer of current tack context
/////////////////////////////////////////////////////////////////////////////
void mhal_context_switch(MHAL_Context *pNextCtx, MHAL_Context **pSaveCtxPtr);

/////////////////////////////////////////////////////////////////////////////
/// @brief switch to new context
/// @param <IN>     \b pNextCtx:        new context's stack pointer to context
/////////////////////////////////////////////////////////////////////////////
void mhal_context_load(MHAL_Context *pNextCtx);

#endif

//MST_END_DECL

#endif /* __MHAL_ARCH_H__ */
