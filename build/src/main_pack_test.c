#include "crules.h"
#include "tkel.h"
#include "tdal_common.h"
#ifdef __DEBUG_TRACE__
#include "trace_cfg.h"
#endif
/*#include "task.h"*/

extern void PACK_TestMain(void);

int main(int argc, char *argv[])
{
    TKEL_err                tkelErr;

    TDAL_Init();

    tkelErr = TKEL_Init();
#ifdef __DEBUG_TRACE__
    TRACE_CFG_Init();
#endif

    /*
     * change the priority since the current priority is the highest (15).
     * The new priority is the same as the priority of RT_Task of TBOX.
    task_priority_set(NULL, 2);
     */

    PACK_TestMain();
    return 0;
}

