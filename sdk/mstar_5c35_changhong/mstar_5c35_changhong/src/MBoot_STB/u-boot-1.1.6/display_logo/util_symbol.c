#include <common.h>
#include <display_logo/util_symbol.h>

U32 debug_symbol_tbl[SYMBOL_LAST];

void Util_InitSymbolTBL(void)
{
    // Write the address of debug information for MSTV_Tool to fix location (0x02F0)
    #define debug_symbol_tbl_loc 0x02F0
#if defined(__aeon__)
    *(U32 *)debug_symbol_tbl_loc = (U32)debug_symbol_tbl;
#else
    *(U32 *)(((U32)debug_symbol_tbl_loc)|0x80000000) = (U32)debug_symbol_tbl;
#endif
    // no need to clear the tbl, because it is in BSS and already cleared by reset.S
    //memset(&debug_symbol_tbl[0], 0, sizeof(U32)*SYMBOL_LAST);
}

BOOLEAN Util_AddSymbol(DEBUG_SYMBOL symbol, U32 value)
{
#if defined(__aeon__)
    U32 *pU32  =  (U32 *)(((U32)&debug_symbol_tbl[0])|0x80000000);
#else
    U32 *pU32  =  (U32 *)(((U32)&debug_symbol_tbl[0])|0xA0000000);
#endif

    if (symbol >= SYMBOL_LAST)
        return FALSE;
    pU32[symbol] = value;
    //printf("set symbol %d-->%08x, r1=%08x\n", symbol,  value, tmp);
    return TRUE;
}
