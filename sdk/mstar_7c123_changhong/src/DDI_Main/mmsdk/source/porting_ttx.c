#include "porting_sysinfo.h"
#include "porting_ttx.h"

#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus

#ifdef AVP_ENABLE

MMSDK_BOOL PT_TtxDisplay(const MMSDK_BOOL bShow)
{
    return FALSE;
}

MMSDK_BOOL PT_TtxPush(const ST_MMSDK_TTX_QUEUE_PUSH_INFO *pstQueueInfo)
{
    return FALSE;
}

MMSDK_BOOL PT_TtxSetSubtitleInfo(const ST_MMSDK_SET_TTX_TRACK_INFO *pstTrackInfo)
{
    return FALSE;
}

MMSDK_BOOL PT_Ttx_Initialize(void* pClass, const pfnMmsdkTtxCallback pCallback)
{
    return FALSE;
}

MMSDK_BOOL PT_TtxInitialize(const ST_MMSDK_TTX_INIT_INFO *pstTtxInfo)
{
    return FALSE;
}

MMSDK_BOOL PT_Ttx_Finalize(void)
{
    return FALSE;
}

#endif

#ifdef __cplusplus
}
#endif //__cplusplus
