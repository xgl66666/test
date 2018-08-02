#include <string.h>
#include "MsTypes.h"
#include "porting_cusinfo.h"
#include "porting_sysinfo.h"

MMSDK_BOOL PT_GetCustomerInfo(MMSDK_U8 *pu8Info, MMSDK_U8 u8Size)
{
    if(!pu8Info)
        return FALSE;

    PT_SYS_GetCusInfo(pu8Info, u8Size);

    return TRUE;
}

MMSDK_BOOL PT_GetHashKey(MMSDK_U8 *pu8Key, MMSDK_U8 u8Size)
{
    if(!pu8Key)
        return FALSE;

    PT_SYS_GetHashKey(pu8Key, u8Size);

    return TRUE;
}

MMSDK_BOOL PT_GetDRMModelID(MMSDK_U16 *pu16DRMMID)
{
    if(!pu16DRMMID)
        return FALSE;

    PT_SYS_GetDRMID(pu16DRMMID);

    return TRUE;
}
