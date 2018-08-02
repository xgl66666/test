#include <stdio.h>
#include "MsTypes.h"
#include "apiXC.h"
#include "apiXC_PCMonitor.h"
#include "porting_xc.h"

#ifdef MS_DEBUG
#define MPL_XC_TRACE(fmt, args...)     printf("[%s][%d]"fmt, __FUNCTION__, __LINE__, ##args)
#define MPL_XC_DBG(fmt, args...)       printf("[%s][%d]"fmt, __FUNCTION__, __LINE__, ##args)
#define MPL_XC_ERR(fmt, args...)       printf("[%s][%d]"fmt, __FUNCTION__, __LINE__, ##args)
#else
#define MPL_XC_TRACE(fmt, args...)     //printf("[%s][%d]"fmt, __FUNCTION__, __LINE__, ##args)
#define MPL_XC_DBG(fmt, args...)       //printf("[%s][%d]"fmt, __FUNCTION__, __LINE__, ##args)
#define MPL_XC_ERR(fmt, args...)       printf("[%s][%d]"fmt, __FUNCTION__, __LINE__, ##args)
#endif
MPL_PNL_DEVICE_ID g_stPNL_DeviceId[2] = {{0, E_MPL_PNL_DEVICE0}, {0, E_MPL_PNL_DEVICE1}};

MS_BOOL MPL_XC_GetDispWinFromReg(MS_WINDOW_TYPE *pstDspwin, SCALER_WIN eWindow)
{
    MPL_XC_TRACE("Entered.\n");

    if (pstDspwin == NULL)
    {
        MPL_XC_ERR("Invalid parameter\n");
        return FALSE;
    }

    MApi_XC_GetDispWinFromReg(pstDspwin, eWindow);
    return TRUE;
}

MS_BOOL MPL_XC_PCMonitor_SyncLoss(SCALER_WIN eWindow)
{
    MPL_XC_TRACE("Entered.\n");

    MApi_XC_PCMonitor_SyncLoss(eWindow);
    return TRUE;
}

MS_U32 MPL_PNL_GetPNLInfo(EN_MPL_PNL_INFO ePnlInfo, MS_U32 *pData)
{
    MS_U32 u32Panel = 0;

#ifdef NEW_XC   //@FIXME: [Josh] Use feature to differ XC or XC_EX...?
    PNL_DeviceId stPNL_DeviceId = {g_stPNL_DeviceId[E_MPL_PNL_DEVICE0].u32Version, g_stPNL_DeviceId[E_MPL_PNL_DEVICE0].u32Id};

    MPL_XC_TRACE("Entered.\n");

    switch (ePnlInfo)
    {
        case E_MPL_PNL_WIDTH:
            u32Panel = g_IPanelEx.Width(&stPNL_DeviceId);
            break;

        case E_MPL_PNL_HEIGHT:
            u32Panel = g_IPanelEx.Height(&stPNL_DeviceId);
            break;

        case E_MPL_PNL_HSTART:
            u32Panel = g_IPanelEx.HStart(&stPNL_DeviceId);
            break;

        default:
            MPL_XC_ERR("Invalid parameter\n");
            break;
    }
#else
    MPL_XC_TRACE("Entered.\n");

    switch (ePnlInfo)
    {
        case E_MPL_PNL_WIDTH:
            u32Panel = g_IPanel.Width();
            break;

        case E_MPL_PNL_HEIGHT:
            u32Panel = g_IPanel.Height();
            break;

        case E_MPL_PNL_HSTART:
            u32Panel = g_IPanel.HStart();
            break;

        default:
            MPL_XC_ERR("Invalid parameter\n");
            break;
    }
#endif

    return u32Panel;
}
