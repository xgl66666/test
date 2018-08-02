#include "porting_database.h"
#include "porting_sysinfo.h"
#include "mmsdk_interface_def.h"

MMSDK_BOOL PT_SetDivXDRMSetting(ST_MMSDK_DIVXDRM_INFO *pstDRMInfo)
{
    if(!pstDRMInfo)
        return FALSE;

    PT_SYS_WriteToDB(pstDRMInfo);

    return TRUE;
}

MMSDK_BOOL PT_GetDivXDRMSetting(ST_MMSDK_DIVXDRM_INFO *pstDRMInfo)
{
    if(!pstDRMInfo)
        return FALSE;

    PT_SYS_ReadFromDB(pstDRMInfo);

    return TRUE;
}

