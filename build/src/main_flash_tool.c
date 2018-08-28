#include "crules.h"
#ifdef __DEBUG_TRACE__
#include "trace_cfg.h"
#endif
#include "tbox.h"

#define MODULE_TRC_FS 4
#define MODULE_TRC_FLASH_TOOL 0x50000000

extern void flash_tool_main(void);

int main(int argc, char *argv[])
{
    mTBOX_INIT_MOD_TRACE((MODULE_TRC_FS, kTBOX_TRC_CRITICAL));
    mTBOX_INIT_MOD_TRACE((MODULE_TRC_FLASH_TOOL, kTBOX_TRC_CRITICAL));
    
#ifdef __DEBUG_TRACE__
    TRACE_CFG_Init();
#endif

    flash_tool_main();
    return 0;
}
