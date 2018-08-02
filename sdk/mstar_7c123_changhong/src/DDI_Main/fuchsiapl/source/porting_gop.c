#include <stdio.h>
#include "MsTypes.h"
#include "apiGOP.h"
#include "porting_gop.h"

#ifdef MS_DEBUG
#define MPL_GOP_TRACE(fmt, args...)     printf("[%s][%d]"fmt, __FUNCTION__, __LINE__, ##args)
#define MPL_GOP_DBG(fmt, args...)       printf("[%s][%d]"fmt, __FUNCTION__, __LINE__, ##args)
#define MPL_GOP_ERR(fmt, args...)       printf("[%s][%d]"fmt, __FUNCTION__, __LINE__, ##args)
#else
#define MPL_GOP_TRACE(fmt, args...)     //printf("[%s][%d]"fmt, __FUNCTION__, __LINE__, ##args)
#define MPL_GOP_DBG(fmt, args...)       //printf("[%s][%d]"fmt, __FUNCTION__, __LINE__, ##args)
#define MPL_GOP_ERR(fmt, args...)       printf("[%s][%d]"fmt, __FUNCTION__, __LINE__, ##args)
#endif
E_GOP_API_Result MPL_GOP_GWIN_SwitchGOP(MS_U8 u8GOP)
{
    MPL_GOP_TRACE("Entered.\n");

    return MApi_GOP_GWIN_SwitchGOP(u8GOP);
}

E_GOP_API_Result MPL_GOP_GetChipCaps(EN_GOP_CAPS eCapType, MS_U32 *pRet, MS_U32 ret_size)
{
    MPL_GOP_TRACE("Entered.\n");

    if (pRet == NULL)
    {
        MPL_GOP_ERR("Invalid parameter\n");
        return GOP_API_INVALID_PARAMETERS;
    }

    return MApi_GOP_GetChipCaps(eCapType, pRet, ret_size);
}

E_GOP_API_Result MPL_GOP_SetGOPHStart(MS_U8 u8GOP, MS_U16 u16PanelHStr)
{
    MPL_GOP_TRACE("Entered.\n");

    return MApi_GOP_SetGOPHStart(u8GOP, u16PanelHStr);
}

E_GOP_API_Result MPL_GOP_GWIN_EnableTransClr(EN_GOP_TRANSCLR_FMT fmt, MS_BOOL bEnable)
{
    MPL_GOP_TRACE("Entered.\n");

    return MApi_GOP_GWIN_EnableTransClr(fmt, bEnable);
}

E_GOP_API_Result MPL_GOP_GWIN_Create(MS_U16 width, MS_U16 height, MS_U16 fbFmt)
{
    MPL_GOP_TRACE("Entered.\n");

    return MApi_GOP_GWIN_CreateWin(width, height, fbFmt);
}

E_GOP_API_Result MPL_GOP_GWIN_Enable(MS_U8 winId, MS_BOOL bEnable)
{
    MPL_GOP_TRACE("Entered.\n");

    return MApi_GOP_GWIN_Enable(winId, bEnable);
}

E_GOP_API_Result MPL_GOP_GWIN_Delete(MS_U8 winId)
{
    MPL_GOP_TRACE("Entered.\n");

    return MApi_GOP_GWIN_DeleteWin(winId);
}

E_GOP_API_Result MPL_GOP_GWIN_Release(MS_U8 gId)
{
    MPL_GOP_TRACE("Entered.\n");

    return MApi_GOP_GWIN_ReleaseWin(gId);
}

E_GOP_API_Result MPL_GOP_GWIN_Destroy(MS_U8 gId)
{
    MPL_GOP_TRACE("Entered.\n");

    return MApi_GOP_GWIN_DestroyWin(gId);
}

E_GOP_API_Result MPL_GOP_GWIN_EnableProgressive(MS_BOOL bEnable)
{
    MPL_GOP_TRACE("Entered.\n");

    return MApi_GOP_GWIN_EnableProgressive(bEnable);
}

E_GOP_API_Result MPL_GOP_GWIN_SetForceWrite(MS_BOOL bEnable)
{
    MPL_GOP_TRACE("Entered.\n");

    return MApi_GOP_GWIN_SetForceWrite(bEnable);
}

E_GOP_API_Result MPL_GOP_GWIN_UpdateRegOnce(MS_BOOL bWriteRegOnce)
{
    MPL_GOP_TRACE("Entered.\n");

    return MApi_GOP_GWIN_UpdateRegOnce(bWriteRegOnce);
}

E_GOP_API_Result MPL_GOP_GWIN_SetFieldInver(MS_BOOL enable)
{
    MPL_GOP_TRACE("Entered.\n");

    return MApi_GOP_GWIN_SetFieldInver(enable);
}

E_GOP_API_Result MPL_GOP_GWIN_SetGOPDst(MS_U8 u8GOP, EN_GOP_DST_TYPE dsttype)
{
    MPL_GOP_TRACE("Entered.\n");

    return MApi_GOP_GWIN_SetGOPDst(u8GOP, dsttype);
}

E_GOP_API_Result MPL_GOP_GWIN_Set_Hscale(MS_BOOL bEnable, MS_U16 src, MS_U16 dst)
{
    MPL_GOP_TRACE("Entered.\n");

    MPL_GOP_DBG("Source %d Destination %d\n", src, dst);
    return MApi_GOP_GWIN_Set_HSCALE(bEnable, src, dst);
}

E_GOP_API_Result MPL_GOP_GWIN_Set_Vscale(MS_BOOL bEnable, MS_U16 src, MS_U16 dst)
{
    MPL_GOP_TRACE("Entered.\n");

    MPL_GOP_DBG("Source %d Destination %d\n", src, dst);
    return MApi_GOP_GWIN_Set_VSCALE(bEnable, src, dst);
}

E_GOP_API_Result MPL_GOP_GWIN_Set_StretchWin(MS_U8 u8GOP_num, EN_GOP_DST_TYPE eDstType, MS_U16 x, MS_U16 y, MS_U16 width, MS_U16 height)
{
    MPL_GOP_TRACE("Entered.\n");

    MPL_GOP_DBG("Type %d Horizontal %d Vertical %d Width %d Height %d\n", eDstType, x, y, width, height);
    return MApi_GOP_GWIN_Set_STRETCHWIN(u8GOP_num, eDstType, x, y, width, height);
}

E_GOP_API_Result MPL_GOP_GWIN_OutputColor(EN_GOP_OUTPUT_COLOR type)
{
    MPL_GOP_TRACE("Entered.\n");

    return MApi_GOP_GWIN_OutputColor(type);
}

E_GOP_API_Result MPL_GOP_GWIN_Switch2Gwin(MS_U8 winId)
{
    MPL_GOP_TRACE("Entered.\n");

    return MApi_GOP_GWIN_Switch2Gwin(winId);
}

E_GOP_API_Result MPL_GOP_GWIN_SetWinInfo(MS_U8 u8win, GOP_GwinInfo *pinfo)
{
    MPL_GOP_TRACE("Entered.\n");

    if (pinfo == NULL)
    {
        MPL_GOP_ERR("Invalid parameter\n");
        return GOP_API_INVALID_PARAMETERS;
    }

    return MApi_GOP_GWIN_SetWinInfo(u8win, pinfo);
}

E_GOP_API_Result MPL_GOP_GWIN_GetWinInfo(MS_U8 u8win, GOP_GwinInfo *pinfo)
{
    MPL_GOP_TRACE("Entered.\n");

    if (pinfo == NULL)
    {
        MPL_GOP_ERR("Invalid parameter\n");
        return GOP_API_INVALID_PARAMETERS;
    }

    return MApi_GOP_GWIN_GetWinInfo(u8win, pinfo);
}

E_GOP_API_Result MPL_GOP_GWIN_SetBlending(MS_U8 u8win, MS_BOOL bEnable, MS_U8 u8coef)
{
    MPL_GOP_TRACE("Entered.\n");

    return MApi_GOP_GWIN_SetBlending(u8win, bEnable, u8coef);
}

E_GOP_API_Result MPL_GOP_GWIN_SetBlink(MS_BOOL bEnable, MS_U8 u8rate)
{
    MPL_GOP_TRACE("Entered.\n");

    return MApi_GOP_GWIN_SetBlink(bEnable, u8rate);
}

E_GOP_API_Result MPL_GOP_GWIN_SetWinProperty(GOP_GwinDispProperty WinProperty)
{
    MPL_GOP_TRACE("Entered.\n");

    return MApi_GOP_GWIN_SetWinProperty(WinProperty);
}

MS_BOOL MPL_GOP_GWIN_IsGWINEnabled(MS_U8 winId)
{
    MPL_GOP_TRACE("Entered.\n");

    return MApi_GOP_GWIN_IsGWINEnabled(winId);
}

MS_BOOL MPL_GOP_GWIN_IsEnabled(void)
{
    MPL_GOP_TRACE("Entered.\n");

    return MApi_GOP_GWIN_IsEnabled();
}

MS_U8 MPL_GOP_GWIN_IsGwinExist(MS_U8 gId)
{
    MPL_GOP_TRACE("Entered.\n");

    return MApi_GOP_GWIN_IsGwinExist(gId);
}

MS_U8 MPL_GOP_GWIN_CreateFB(MS_U8 fbId, MS_U16 dispX, MS_U16 dispY, MS_U16 width, MS_U16 height, MS_U16 fbFmt)
{
    MPL_GOP_TRACE("Entered.\n");

    MPL_GOP_DBG("dispX %d dispY %d width %d height %d\n", dispX, dispY, width, height);
    return MApi_GOP_GWIN_CreateFB(fbId, dispX, dispY, width, height, fbFmt);
}

MS_U8 MPL_GOP_GWIN_CreateFBbyStaticAddr(MS_U8 fbId, MS_U16 dispX, MS_U16 dispY, MS_U16 width, MS_U16 height,  MS_U16 fbFmt, MS_U32 u32FbAddr)
{
    MPL_GOP_TRACE("Entered.\n");

    MPL_GOP_DBG("dispX %d dispY %d width %d height %d u32FbAddr 0x%lx\n", dispX, dispY, width, height, u32FbAddr);
    return MApi_GOP_GWIN_CreateFBbyStaticAddr(fbId, dispX, dispY, width, height, fbFmt, u32FbAddr);
}

MS_U8 MPL_GOP_GWIN_CreateWin_Assign_FB(MS_U8 u8GOP, MS_U8 FBId, MS_U16 dispX, MS_U16 dispY)
{
    MPL_GOP_TRACE("Entered.\n");

    MPL_GOP_DBG("dispX %d dispY %d\n", dispX, dispY);
    return MApi_GOP_GWIN_CreateWin_Assign_FB(u8GOP, FBId, dispX, dispY);
}

E_GOP_API_Result MPL_GOP_GWIN_SetFBInfo(MS_U8 fbId, GOP_GwinFBAttr *fbAttr)
{
    MPL_GOP_TRACE("Entered.\n");

    if (fbAttr == NULL)
    {
        MPL_GOP_ERR("Invalid parameter\n");
        return GOP_API_INVALID_PARAMETERS;
    }

    return MApi_GOP_GWIN_SetFBInfo(fbId, fbAttr);
}

E_GOP_API_Result MPL_GOP_GWIN_GetFBInfo(MS_U8 fbId, GOP_GwinFBAttr *fbAttr)
{
    MPL_GOP_TRACE("Entered.\n");

    if (fbAttr == NULL)
    {
        MPL_GOP_ERR("Invalid parameter\n");
        return GOP_API_INVALID_PARAMETERS;
    }

    return MApi_GOP_GWIN_GetFBInfo(fbId, fbAttr);
}

MS_U8 MPL_GOP_GWIN_GetFreeFBID(void)
{
    MPL_GOP_TRACE("Entered.\n");

    return MApi_GOP_GWIN_GetFreeFBID();
}

E_GOP_API_Result MPL_GOP_GWIN_DeleteFB(MS_U8 fbId)
{
    MPL_GOP_TRACE("Entered.\n");

    return MApi_GOP_GWIN_DeleteFB(fbId);
}

MS_U8 MPL_GOP_GWIN_DestroyFB(MS_U8 fbId)
{
    MPL_GOP_TRACE("Entered.\n");

    return MApi_GOP_GWIN_DestroyFB(fbId);
}

MS_U8 MPL_GOP_GWIN_IsFBExist(MS_U8 fbId)
{
    MPL_GOP_TRACE("Entered.\n");

    return MApi_GOP_GWIN_IsFBExist(fbId);
}

E_GOP_API_Result MPL_GOP_GWIN_Switch2FB(MS_U8 fbId)
{
    MPL_GOP_TRACE("Entered.\n");

    return MApi_GOP_GWIN_Switch2FB(fbId);
}

E_GOP_API_Result MPL_GOP_GWIN_MapFB2Win(MS_U8 fbId, MS_U8 gwinId)
{
    MPL_GOP_TRACE("Entered.\n");

    return MApi_GOP_GWIN_MapFB2Win(fbId, gwinId);
}

MS_U8 MPL_GOP_GWIN_GetFBfromGWIN(MS_U8 gwinId)
{
    MPL_GOP_TRACE("Entered.\n");

    return MApi_GOP_GWIN_GetFBfromGWIN(gwinId);
}

E_GOP_API_Result MPL_GOP_FB_SEL(MS_U8 u8FB)
{
    MPL_GOP_TRACE("Entered.\n");

    return MApi_GOP_FB_SEL(u8FB);
}

E_GOP_API_Result MPL_GOP_GWIN_ReadPalette(MS_U8 idx, MS_U32 *color)
{
    MPL_GOP_TRACE("Entered.\n");

    if (color == NULL)
    {
        MPL_GOP_ERR("Invalid parameter\n");
        return GOP_API_INVALID_PARAMETERS;
    }

    return MApi_GOP_GWIN_ReadPalette(idx, color);
}

E_GOP_API_Result MPL_GOP_GWIN_SetPaletteOpt(GOP_PaletteEntry *pPalArray, MS_U16 u32PalStart, MS_U16 u32PalEnd, EN_GOP_PALETTE_TYPE ePalType)
{
    MPL_GOP_TRACE("Entered.\n");

    if (pPalArray == NULL)
    {
        MPL_GOP_ERR("Invalid parameter\n");
        return GOP_API_INVALID_PARAMETERS;
    }

    MPL_GOP_DBG("u32PalStart %d u32PalEnd %d\n", u32PalStart, u32PalEnd);
    return MApi_GOP_GWIN_SetPaletteOpt(pPalArray, u32PalStart, u32PalEnd, ePalType);
}

E_GOP_API_Result MPL_GOP_RegisterXCIsInterlaceCB(MS_BOOL (*fpGOP_CB)(void))
{
    MPL_GOP_TRACE("Entered.\n");

    if (fpGOP_CB == NULL)
    {
        MPL_GOP_ERR("Invalid parameter\n");
        return GOP_API_INVALID_PARAMETERS;
    }

    return MApi_GOP_RegisterXCIsInterlaceCB(fpGOP_CB);
}

E_GOP_API_Result MPL_GOP_GWIN_GetFBAddr(MS_U8 fbID, MS_U32 *pAddr)
{
    MPL_GOP_TRACE("Entered.\n");

    if (fbID <= 0)
    {
        MPL_GOP_ERR("Invalid parameter\n");
        return GOP_API_INVALID_PARAMETERS;
    }

    return MApi_GOP_GWIN_GetFBAddr(fbID, pAddr);
}
