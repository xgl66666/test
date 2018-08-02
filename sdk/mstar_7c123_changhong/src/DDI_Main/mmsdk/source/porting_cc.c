#include "porting_sysinfo.h"
#include "porting_cc.h"

#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus

#ifdef AVP_ENABLE

MMSDK_BOOL PT_CCInit(PT_CCITEM* CCItem, const ST_MMSDK_CC_INIT_INFO *pstCCInfo, void* pClass, const pfnMmsdkCCCallback pCallback)
{
    return FALSE;
}

MMSDK_BOOL PT_CCDeInit(PT_CCITEM* CCItem)
{
    return FALSE;
}

MMSDK_BOOL PT_CCDisplay(PT_CCITEM CCItem, const EN_MMSDK_CC_CMD_TYPE eCmdType)
{
    return FALSE;
}

MMSDK_BOOL PT_CCInformSetPositionAck(PT_CCITEM CCItem)
{
    return FALSE;
}

MMSDK_BOOL PT_CCSetTrickMode(PT_CCITEM CCItem, const EN_MMSDK_TRICK_MODE eTrickMode)
{
    return FALSE;
}

#endif

#ifdef __cplusplus
}
#endif //__cplusplus
