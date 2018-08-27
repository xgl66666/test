#include <string.h>
#include <math.h>
#include "mmsdk_interface_def.h"
#include "porting_display.h"

#include "porting_os.h"
#include "porting_sysinfo.h"

#include "UFO.h"

#include "drvSERFLASH.h"
#include "drvMVOP.h"
#include "drvXC_IOPort.h"

#include "apiVDEC_EX.h"
#include "apiHDMITx.h"
#include "apiGFX.h"
#include "apiXC.h"
#if (MMSDK_DISPLAY_DUALXC_PORTING == 1)
#include "apiXC_EX.h"
#endif
#if defined(MMSDK_DMS_ENABLE) && (MMSDK_DMS_ENABLE == 1)
#include "apiDMS.h"
#endif
#include "apiGOP.h"
#include "Panel.h"
#include "drvTVEncoder.h"

#include "msAPI_XC_EX.h"
#include "msAPI_VE.h"
#include "drvPQ.h"
#include "drvDTC.h"

#define PT_DISP_ERR(fmt, arg...)        PT_SYS_PrintLog(E_MMSDK_DBG_LEVEL_ERR,   "\033[1;33m######[%s]###### "fmt" \033[0m\n",__FUNCTION__,##arg);
#define PT_DISP_DBG(fmt, arg...)        PT_SYS_PrintLog(E_MMSDK_DBG_LEVEL_ERR,   "\033[1;35m######[%s]###### "fmt" \033[0m\n",__FUNCTION__,##arg);
#define PT_DISP_TRACE(fmt, arg...)      PT_SYS_PrintLog(E_MMSDK_DBG_LEVEL_TRACE, "\033[1;36m######[%s]###### "fmt" \033[0m\n",__FUNCTION__,##arg);
#define PT_DISP_DBG_DS(fmt, arg...)     PT_SYS_PrintLog(E_MMSDK_DBG_LEVEL_ERR,   "\033[1;34m######[%s]###### "fmt" \033[0m\n",__FUNCTION__,##arg);

#define CHECK_MAPI_XC_RETURN_VALUE(func)  if(func != TRUE) {PT_DISP_ERR("[%d] FAIL", __LINE__); return FALSE;}
#define CHECK_MSAPI_XC_RETURN_VALUE(func) if(func != E_MSAPI_XC_OK) {PT_DISP_ERR("[%d] FAIL", __LINE__); return FALSE;}

#if (MMSDK_DISPLAY_DUALXC_PORTING == 1)
MSAPI_XC_DEVICE_ID g_stXC_PT_DeviceId[2] = {{0, E_MSAPI_XC_DEVICE0}, {0, E_MSAPI_XC_DEVICE1}};
#define DEVICE_ID_0 g_stXC_PT_DeviceId[0]
#define DEVICE_ID_1 g_stXC_PT_DeviceId[1]
#else
MSAPI_XC_DEVICE_ID g_stXC_PT_DeviceId = {0, E_MSAPI_XC_DEVICE0};
#define DEVICE_ID_0 g_stXC_PT_DeviceId
#endif
extern PT_SYS_CUS_FunPtr gstSysCusFunPtr;

#ifdef AVP_ENABLE
MSAPI_XC_DEVICE_ID g_stDIP_PT_DeviceId = {0, E_MSAPI_XC_DEVICE_DIP_0};
static MS_U8 u8Display_Instance_BitMask = 0;
static MS_U8 u8DIP_Window_BitMask = 0;
static MS_U8 u8CountSetSC1Window = 0;
#define Display_Instance_BitMask_NonUse  0
#define DIP_Win_NonUse  0xFF
#define XC_FLOW 0
#define DIP_FLOW 1
#endif

typedef struct
{
    ST_MMSDK_SET_DISPLAY_INFO stDisplayInfo;
    pfnDisplayCallback pfnDisplayCallback;
    void* pDislpayClass;
    MMSDK_BOOL bIsUpDSInfo;
    MMSDK_BOOL bIsOneField;
    MMSDK_BOOL bIsFirstTimeSetOneField;
#ifdef AVP_ENABLE
    MS_U8 u8ID_BitMask;
    MS_U8 u8DIPWin;
    MS_U8 u8Flow;
    MMSDK_BOOL bIsFramePushed;
    MMSDK_BOOL bSetSC1Window;
    MMSDK_U32 u32DispCount;
#endif
    MMSDK_BOOL bFirstTimeSetWindow;
    SCALER_WIN eScalerWin;
    MMSDK_BOOL bIsAvpFlow;
    MMSDK_BOOL bISSet3D;
    MMSDK_BOOL bIsMM3D;
} _ST_MMSDK_DISPLAY_INSTANCE;

//ST_MMSDK_SET_DISPLAY_INFO g_stDisplayInfo;
//pfnDisplayCallback g_pfnDisplayCallback = NULL;
//void* g_pDislpayClass = NULL;

#define CALLBACK(arg1, arg2, arg3)                                \
    (((pDisplayInstance == NULL) || (pDisplayInstance->pfnDisplayCallback == NULL)) ?   \
    (FALSE) :   \
    (pDisplayInstance->pfnDisplayCallback(pDisplayInstance->pDislpayClass,arg1, arg2, arg3)))

#define DYNAMIC_SCALING_VB_H 1920
#define DYNAMIC_SCALING_VB_V 1080

typedef struct
{
    MS_U16 u16VBox_Htotal;
    MS_U16 u16VBox_Vtotal;
} DS_VBOX_INFO;

/// to define the scaler information for dynamic scaling (DS)
typedef struct
{
    // 0x000
    MMSDK_U8 u8DSVersion;                     /// Versin
    MMSDK_U8 bHKIsUpdating;                   /// House keeping is writing
    MMSDK_U8 bFWIsUpdating;                   /// firmware is reading
    MMSDK_U8 bFWGotXCInfo;                    /// for firmware internal use, indicate that firmware received XC current setting
    MMSDK_U8 u8XCInfoUpdateIdx;               /// XC info
    MMSDK_U8 bFWGotNewSetting;                /// for firmware internal use, indicate that firmware received XC current setting
    MMSDK_U8 u8Dummy_3;                       /// u8Dummy_3
    MMSDK_U8 u8MVOPCrop;                       /// u8Dummy_2

    // 0x008
    MS_WINDOW_TYPE stCapWin;            ///capture window
    MS_WINDOW_TYPE stCropWin;           ///croping window

    // 0x018
    MS_WINDOW_TYPE stDispWin;           ///display window
    MMSDK_U16 u16H_SizeAfterPreScaling;       //horizontal size
    MMSDK_U16 u16V_SizeAfterPreScaling;       //vertical size
    MMSDK_U32 u32PNL_WIDTH;                   /// Panel width

    // 0x028
    MMSDK_U32 u32IPMBase0;                    ///<IPM base 0
    MMSDK_U32 u32IPMBase1;                    ///<IPM base 1
    MMSDK_U32 u32IPMBase2;                    ///<IPM base 2

    // 0x034
    MS_WINDOW_TYPE stNewCropWin;        ///< Zoom in/out new crop win
    MS_WINDOW_TYPE stNewDispWin;        ///< Zoom in/out new disp win

    // 0x044
    MMSDK_U8 bLinearMode;                     ///<Is current memory format LinearMode?
    MMSDK_U8 u8BitPerPixel;                   ///<Bits number Per Pixel
    MMSDK_U8 bInterlace;                      ///<Interlaced or Progressive
    MMSDK_U8 u8StoreFrameNum;                 ///<Store frame number

    // 0x048
    MMSDK_U16 u16IPMOffset;                   ///<IPM offset
    MMSDK_U8  u8EnhanceModeCap;
    MMSDK_U8  u8EnhanceModeSetting;

    // 0x04C
    MMSDK_U8 bMirrorMode;                     ///<mirror mode
    MMSDK_U8 u8Dummy2[3];                     ///dummy2

    // internal use only
    // 0x050
    MS_WINDOW_TYPE stDSScaledCropWin;   ///dynamic scalering croping window

    // 0x058
    MMSDK_U16 u16SizeAfterPreScaling;         ///size after pre sacling
    MMSDK_U16 u16Fetch;                       ///fetch
    MMSDK_U16 u16Offset;                      ///offset
    MMSDK_U16 u16PixelOffset;                 ///Pixel offset
    MMSDK_U16 u16LineBufOffset;               ///Line Buffer Offset
    MMSDK_U16 u16Dummy1;                      ///Dummy1

    // 0x064
    MMSDK_U32 u32CropOffset;                  ///crop offset

    // for debugging, 0x68
    MMSDK_U8 OPRegMaxDepth;                   /// point to record max OP reg depth
    MMSDK_U8 IPRegMaxDepth;                   /// point to record max IP reg depth
    MMSDK_U8 ErrorCode;                       /// point to record error code
    MMSDK_U8 u8Dummy3;                        /// dummy

    // 0x6C
    //MAPI_U16 u16CodedWidth;                           /// codec width
    //MAPI_U16 u16CodedHeight;                          /// codec height
    //MAPI_U16 u16DSBufIdx;                               /// dynamic scalering Buffer index
    MS_WINDOW_TYPE stDSCropWin;         /// dynamic scaalering crop window

    // 0x74
    MMSDK_U16 u16HCusScalingSrc;        ///< horizontal scaling down/up source size
    MMSDK_U16 u16VCusScalingSrc;        ///< vertical scaling down/up source size
    MMSDK_U16 u16HCusScalingDst;        ///< horizontal prescaling down/up destination size
    MMSDK_U16 u16VCusScalingDst;        ///< vertical prescaling down/up destination size

    // 0x7C
    E_XC_3D_INPUT_MODE  e3DInputType;        ///< input 3D type decided by APP layer
    E_XC_3D_OUTPUT_MODE e3DOutputType;       ///< output 3D type decided by APP layer

    // 0x84
    MMSDK_U16 u16MVOPHStart;            ///<MVOP HStart
    MMSDK_U16 u16MVOPVStart;            ///<MVOP VStart

    //Sync from CL1370181 strcut ds_xc_data_structure
    // 0x88
    MMSDK_U16 u16CurrentFrameHSize;        ///<Current frame horizontal active size.
    MMSDK_U16 u16CurrentFrameVSize;        ///<Current frame vertical active size.
    MMSDK_U16 u16NextFrameHSize;           ///<Next frame horizontal active size.
    MMSDK_U16 u16NextFrameVSize;           ///<Next frame vertical active size.
    MMSDK_U16 u16VsyncCNT;                 ///<vsync count

    // 0x92
    //unsigned char u8Dummy4;
    MMSDK_U8 u8FRCMStoreFrameNum;
    MMSDK_U8 bUseVBoxOfHK;

    // 0x94
    MMSDK_U16 u16VirtualBoxWidth;
    MMSDK_U16 u16VirtualBoxHeight;

    //0x98
    MMSDK_U16 u16InputVFreq;

    //0x9A
    MMSDK_U16 u16Dummy5;

    //0x9C
    MMSDK_U32 u32FRCMBase0;                  ///<FRCM base 0
    MMSDK_U32 u32FRCMBase1;                  ///<FRCM base 1
    MMSDK_U32 u32FRCMBase2;                  ///<FRCM base 2

    //0xA8
    MMSDK_U8 bFBL;                  ///<bFBL
    MMSDK_U8 bR_FBL;                ///<bR_FBL

    //0xAA
    MMSDK_U8 bFRC;                  ///<bFRC
    MMSDK_U8 bFSC;

    //0xAC
    MMSDK_U16 u16PNL_HST;                  ///<u16PNL_HST
    MMSDK_U16 u16PNL_HEND;                 ///<u16PNL_HEND
    MMSDK_U16 u16PNL_VST;                  ///<u16PNL_VST
    MMSDK_U16 u16PNL_VEND;                 ///<u16PNL_VEND
} ST_MM_DS_XC_STATUS;

#ifdef UFO_XC_HDR
#ifdef UFO_XC_HDR_VERSION
#if UFO_XC_HDR_VERSION == 2
#if (MMSDK_HDR_MODE >= 1)
#if (MI_ENABLE == 0)
#ifdef MMSDK_DOLBY_HDR_ENABLE
#define DOVI_HDR_MODE (MMSDK_DOLBY_HDR_ENABLE == 1)
#endif
#if (DOVI_HDR_MODE == 0)
static MS_U8 _gu8HDRCtrl = 0;// 0 is auto, 1 is force HDR to SDR, 2is force bypass
#endif
#endif
MS_U32 _gu32HDRMetadataType = E_MMSDK_HDR_TYPE_CTL_FLAG_NONE;
#define HDR_MM_DEV
#define STREAM_IS_HDR (_gu32HDRMetadataType != E_MMSDK_HDR_TYPE_CTL_FLAG_NONE)
#define STREAM_HDR_TYPE_TCH (_gu32HDRMetadataType & E_MMSDK_HDR_TYPE_CTL_FLAG_TCH)
#define STREAM_HDR_TYPE_DOVI (_gu32HDRMetadataType & E_MMSDK_HDR_TYPE_CTL_FLAG_DOLBY_HDR)
#define DISP_HDR_SMPTE2084 0x10
#define DISP_HDR_HLG 0x12
#endif
#endif
#endif
#endif

#ifdef HDR_MM_DEV
#include "apiXC_Dlc.h"
#if (MI_ENABLE == 0)
MS_BOOL _gbFireHDR = FALSE;

MS_BOOL SendCfd(EN_XC_CFD_CTRL_TYPE eType, void *pParam, MS_U16 u16Length)
{
    XC_CFD_CONTROL_INFO stXCCFDControlInfo;
    memset(&stXCCFDControlInfo, 0, sizeof(XC_CFD_CONTROL_INFO));
    stXCCFDControlInfo.enCtrlType = eType;
    stXCCFDControlInfo.pParam = pParam;
    stXCCFDControlInfo.u32ParamLen = u16Length;
    MApi_XC_HDR_Control(E_XC_HDR_CTRL_CFD_CONTROL,&stXCCFDControlInfo);
    return TRUE;
}

MS_U16 gstHdrLevelAttributes[3][10] =
{
    //u16Smin,u16Smed,u16Smax,u16Tmin,  u16Tmed,u16Tmax,u16MidSourceOffset,u16MidTargetOffset,u16MidTargetRatio
    {256, 3072, 10000, 256, 544, 768, 410, 420, 300, 500},
    {256, 3072, 10000, 256, 576, 768, 430, 480, 400, 400},
    {256, 3072, 10000, 256, 640, 768, 418, 510, 200, 500}
};

MS_BOOL LoadHdrMetadataByHdrIp(void)
{
    MS_BOOL bRet = FALSE;
    XC_CFD_HDR stCfdHdr;
    memset(&stCfdHdr, 0, sizeof(XC_CFD_HDR));
    stCfdHdr.u32Version = CFD_HDR_VERSION;
    stCfdHdr.u16Length = sizeof(XC_CFD_HDR);
    stCfdHdr.u8Win = 0;
    if(STREAM_HDR_TYPE_TCH)
        stCfdHdr.u8HdrType =  E_XC_CFD_HDR_TYPE_OPEN | E_XC_CFD_HDR_TYPE_TCH;
    else if(STREAM_HDR_TYPE_DOVI)
        stCfdHdr.u8HdrType = E_XC_CFD_HDR_TYPE_DOLBY;
    else
        stCfdHdr.u8HdrType = E_XC_CFD_HDR_TYPE_OPEN;
    MApi_XC_DLC_SetDlcHandlerOnOff(TRUE);
    //SendCfd(E_XC_CFD_CTRL_TYPE_HDR, &stCfdHdr, sizeof(XC_CFD_HDR));
    MSAPI_XC_CFD_SENDCFD_INFO stCFDSendInfo = {};
    stCFDSendInfo.etype = E_XC_CFD_CTRL_TYPE_HDR;
    stCFDSendInfo.pParam = &stCfdHdr;
    stCFDSendInfo.u16Length = sizeof(XC_CFD_HDR);

    msAPI_XC_CFD_Control(NULL, E_MSAPI_XC_CFD_SEND_CFD, &stCFDSendInfo);

    //Open hdr

    // Set Open HDR level.
    ST_XC_DLC_TMO_INFO stTmo;
    stTmo.u32TMOInfo_Version = DLC_TMO_INFO_VERSION;
    stTmo.u16TMOInfo_Length = sizeof(ST_XC_DLC_TMO_INFO);

    stTmo.u16SrcMinRatio = 10;
    stTmo.u16SrcMedRatio = 512;
    stTmo.u16SrcMaxRatio = 1019;

    stTmo.u16TgtMin = 500;
    stTmo.u16TgtMed = 50;
    stTmo.u16TgtMax = 100;

    stTmo.u16FrontSlopeMin = 256;
    stTmo.u16FrontSlopeMax = 512;
    stTmo.u16BackSlopeMin = 128;
    stTmo.u16BackSlopeMax = 256;

    stTmo.u16SceneChangeThrd = 512;
    stTmo.u16SceneChangeRatioMax = 1024;

    stTmo.u8IIRRatio = 31;
    stTmo.u8TMOTargetMode = 1;
    stTmo.u16SDRPanelGain = 60;

    stTmo.u16Smin = gstHdrLevelAttributes[0][0];
    stTmo.u16Smed = gstHdrLevelAttributes[0][1];
    stTmo.u16Smax = gstHdrLevelAttributes[0][2];
    stTmo.u16Tmin = gstHdrLevelAttributes[0][3];
    stTmo.u16Tmed = gstHdrLevelAttributes[0][4];
    stTmo.u16Tmax = gstHdrLevelAttributes[0][5];


    MApi_XC_DLC_SetTMOInfo(&stTmo);
    bRet = TRUE;

    return bRet;
}

#endif

static void _PT_Display_SetHDRInit(void)
{
#if (MI_ENABLE == 0)
#if DOVI_HDR_MODE
    return;
#else
    MS_BOOL bSeamlessEnable = FALSE;
    msAPI_XC_GetHDRSeamlessFlag(&bSeamlessEnable);

    if(STREAM_IS_HDR || bSeamlessEnable)
    {
#if (MMSDK_HDR_MODE == 1)
        PT_DISP_DBG("Set HDR Init\n");
        XC_DLC_HDRinit stHDRData = {0};
        stHDRData.bHDREnable = TRUE;
        stHDRData.u16HDRInitLength = sizeof(XC_DLC_HDRinit);
        stHDRData.u16HDRVerInfo = XC_DLC_HDR_VERSION;
        MApi_XC_DLC_SetHDRInit(&stHDRData);
        MApi_XC_WriteByteMask(0x100B16,0x0,0x1); // enable HDR clock
        MApi_XC_WriteByteMask(0x100A4A, 0x0, 0x11);// enable HDR SRAM clock
        MApi_XC_WriteByteMask(0x100A4B, 0x0, 0x1);
#endif
    }
#endif//#if DOVI_HDR_MODE
#endif//(MI_ENABLE == 0)
}

static void _PT_Display_CFDInit(void)
{
#if (MI_ENABLE == 0)
#if DOVI_HDR_MODE
        MSAPI_XC_CFD_SENDCFD_INFO stCFDSendInfo = {};

        XC_CFD_HDR stCfdHdr;
        memset(&stCfdHdr, 0, sizeof(XC_CFD_HDR));
        stCfdHdr.u32Version = CFD_HDR_VERSION;
        stCfdHdr.u16Length = sizeof(XC_CFD_HDR);
        stCfdHdr.u8Win = 0;

        if(STREAM_IS_HDR)
        {
            if(STREAM_HDR_TYPE_DOVI)
                stCfdHdr.u8HdrType = E_XC_CFD_HDR_TYPE_DOLBY;
            else
                stCfdHdr.u8HdrType = E_XC_CFD_HDR_TYPE_OPEN;
        }
        else
        {
            stCfdHdr.u8HdrType = E_XC_CFD_HDR_TYPE_NONE;
        }

        stCFDSendInfo.etype = E_XC_CFD_CTRL_TYPE_HDR;
        stCFDSendInfo.pParam = &stCfdHdr;
        stCFDSendInfo.u16Length = sizeof(XC_CFD_HDR);

        msAPI_XC_CFD_Control(NULL, E_MSAPI_XC_CFD_SEND_CFD, &stCFDSendInfo);
#else
    MS_BOOL bSeamlessEnable = FALSE;
    msAPI_XC_GetHDRSeamlessFlag(&bSeamlessEnable);

    if(STREAM_IS_HDR || bSeamlessEnable)
    {
        PT_DISP_DBG("Init CFD\n");

        msAPI_XC_CFD_Control(NULL, E_MSAPI_XC_CFD_SET_SINK_EDID_TO_CFD, NULL);

        XC_CFD_INIT stCfdInit;
        stCfdInit.u32Version = CFD_INIT_VERSION;
        stCfdInit.u16Length = sizeof(XC_CFD_INIT);
        stCfdInit.u8Win = 0;
        stCfdInit.u8InputSource = (MS_U8)PQ_INPUT_SOURCE_STORAGE;
        SendCfd(E_XC_CFD_CTRL_TYPE_INIT, &stCfdInit, sizeof(XC_CFD_INIT));
        LoadHdrMetadataByHdrIp();

        // Porting layer does not need set E_VDEC_EX_USER_CMD_SET_DV_XC_SHM_ADDR to VDEC
#if (MMSDK_HDR_MODE == 1)
        MS_BOOL bHDR = TRUE;
        msAPI_XC_SetCommand(NULL, E_MSAPI_XC_CMD_SET_HDR, &bHDR);
#endif
    }
    else
    {
        PT_DISP_DBG("Not HDR Stream\n");

        MSAPI_XC_CFD_SENDCFD_INFO stCFDSendInfo = {};

        XC_CFD_HDR stCfdHdr;
        memset(&stCfdHdr, 0, sizeof(XC_CFD_HDR));
        stCfdHdr.u32Version = CFD_HDR_VERSION;
        stCfdHdr.u16Length = sizeof(XC_CFD_HDR);
        stCfdHdr.u8Win = 0;
        stCfdHdr.u8HdrType = E_XC_CFD_HDR_TYPE_NONE;

        stCFDSendInfo.etype = E_XC_CFD_CTRL_TYPE_HDR;
        stCFDSendInfo.pParam = &stCfdHdr;
        stCFDSendInfo.u16Length = sizeof(XC_CFD_HDR);

        msAPI_XC_CFD_Control(NULL, E_MSAPI_XC_CFD_SEND_CFD, &stCFDSendInfo);
        msAPI_XC_CFD_Control(NULL, E_MSAPI_XC_CFD_SET_SINK_EDID_TO_CFD, NULL);
        msAPI_XC_CFD_Control(NULL, E_MSAPI_XC_CFD_FIRE, NULL);
    }
#endif//#if DOVI_HDR_MODE
#endif

}

static void _PT_Display_EnableFrameBufferLess(void)
{
#if (MI_ENABLE == 0)
    MS_BOOL bSeamlessEnable = FALSE;
    msAPI_XC_GetHDRSeamlessFlag(&bSeamlessEnable);

    if(STREAM_IS_HDR || DOVI_HDR_MODE || bSeamlessEnable)
    {
        _gbFireHDR = TRUE;
#if (MMSDK_HDR_MODE == 1)
        MApi_XC_EnableFrameBufferLess(FALSE);
#endif
    }
#endif
}
#endif

static void _PT_Display_ClearArguments(_ST_MMSDK_DISPLAY_INSTANCE* pDispInstance)
{
    if (pDispInstance == NULL)
    {
        return;
    }
    pDispInstance->pfnDisplayCallback = NULL;
    pDispInstance->pDislpayClass = NULL;
    memset(&pDispInstance->stDisplayInfo, 0, sizeof(ST_MMSDK_SET_DISPLAY_INFO));
#ifdef AVP_ENABLE
    pDispInstance->u8Flow = XC_FLOW;
    pDispInstance->u8ID_BitMask = Display_Instance_BitMask_NonUse;
    pDispInstance->u8DIPWin = DIP_Win_NonUse;
    pDispInstance->bIsFramePushed = FALSE;
    pDispInstance->bSetSC1Window = FALSE;
    pDispInstance->u32DispCount = 0;
#endif
    pDispInstance->bFirstTimeSetWindow = TRUE;
    pDispInstance->eScalerWin = MAIN_WINDOW;
    pDispInstance->bIsAvpFlow = FALSE;
    pDispInstance->bISSet3D = FALSE;
    pDispInstance->bIsMM3D = FALSE;
    pDispInstance->bIsOneField = FALSE;
    pDispInstance->bIsFirstTimeSetOneField = TRUE;
}

static MMSDK_U16 _PT_Display_GetPanelHStart(void)
{
#if (MMSDK_DISPLAY_DUALXC_PORTING == 1)
    PNL_DeviceId stPNL_DeviceId = {g_stXC_PT_DeviceId[0].u32Version, g_stXC_PT_DeviceId[0].u32Id};
    return g_IPanelEx.HStart(&stPNL_DeviceId);
#else
    return g_IPanel.HStart();
#endif
}

static MMSDK_U16 _PT_Display_GetPanelVStart(void)
{
#if (MMSDK_DISPLAY_DUALXC_PORTING == 1)
    PNL_DeviceId stPNL_DeviceId = {g_stXC_PT_DeviceId[0].u32Version, g_stXC_PT_DeviceId[0].u32Id};
    return g_IPanelEx.VStart(&stPNL_DeviceId);
#else
    return g_IPanel.VStart();
#endif
}

void _SetFrameBufferSize(MMSDK_U32 u32Address, MMSDK_U32 u32Size)
{
//Y8M4 for this project always ture.
    if(u32Size)
    {
#if (MMSDK_DISPLAY_DUALXC_PORTING == 1)
        MApi_XC_EX_SetFrameBufferAddress((XC_DEVICE_ID*)&g_stXC_PT_DeviceId[0],u32Address, u32Size, E_XC_EX_MAIN_WINDOW);
        MApi_XC_EX_EnableFrameBufferLess((XC_DEVICE_ID*)&g_stXC_PT_DeviceId[0],FALSE);
#else
        MApi_XC_SetFrameBufferAddress(u32Address, u32Size, MAIN_WINDOW);
        MApi_XC_EnableFrameBufferLess(FALSE);
#endif
#if 0//(STB_ENABLE == 1)
//Y8M4 for this project always ture.
        if(u32Size < 0x900000)
        {
            m_bIsY8M4 = TRUE;
        }
#endif
    }
    else
    {
#if (MMSDK_DISPLAY_DUALXC_PORTING == 1)
        MApi_XC_EX_EnableFrameBufferLess((XC_DEVICE_ID*)&g_stXC_PT_DeviceId[0],TRUE);
#else
        MApi_XC_EnableFrameBufferLess(TRUE);
#endif
    }
}

#if (MM_3D_ENABLE == 1)
static MS_BOOL _HDMITx_3D_Mapping(E_MSAPI_XC_3D_OUTPUT_MODE eXC3DMode, HDMITX_VIDEO_3D_STRUCTURE *eHDMITx3DMode)
{
    MS_BOOL ret = TRUE;
    switch(eXC3DMode)
    {
    case E_MSAPI_XC_3D_OUTPUT_TOP_BOTTOM:
        *eHDMITx3DMode = HDMITX_VIDEO_3D_TopandBottom;
        break;
    case E_MSAPI_XC_3D_OUTPUT_FRAME_PACKING:
        *eHDMITx3DMode = HDMITX_VIDEO_3D_FramePacking;
        break;
    case E_MSAPI_XC_3D_OUTPUT_LINE_ALTERNATIVE:
        *eHDMITx3DMode = HDMITX_VIDEO_3D_LineAlternative;
        break;
    case E_MSAPI_XC_3D_OUTPUT_SIDE_BY_SIDE_HALF:
        *eHDMITx3DMode = HDMITX_VIDEO_3D_SidebySide_Half;
        break;
    case E_MSAPI_XC_3D_OUTPUT_SIDE_BY_SIDE_FULL:
        *eHDMITx3DMode = HDMITX_VIDEO_3D_SidebySide_FULL;
        break;
    case E_MSAPI_XC_3D_OUTPUT_MODE_NONE:
        *eHDMITx3DMode = HDMITx_VIDEO_3D_Not_in_Use;
        break;
    default:
        ret = FALSE;
        PT_DISP_ERR("No HDMITx 3D mode mapping for XC 3D Mode: %d\n", eXC3DMode);
        break;
    }
    return ret;
}
#endif

void PT_Display_InitializeDisplayPath(PT_DISPLAYITEM DisplayItem)
{
    PT_DISP_DBG("");
    //[River]
    //msAPI_XC_Init and msAPI_XC_SetConnect have be called in sysinit.c initXC
    //So do not init them again?
    //[River]
    //It seems HB disable video mute by self.
    //porting layer doesn't disable video mute here to avoid init flow appear last frame of previous play
    //msAPI_XC_SetVideoMute(FALSE,MAIN_WINDOW);
}

MMSDK_BOOL PT_Display_CloseWindow(PT_DISPLAYITEM DisplayItem)
{
#ifdef HDR_MM_DEV
#if (MI_ENABLE == 0)
    MS_BOOL bSeamlessEnable = FALSE;
#endif
#endif
    PT_DISP_DBG("");

    if (DisplayItem == NULL)
    {
        PT_DISP_ERR("DisplayItem is NULL\n");
        return FALSE;
    }
    _ST_MMSDK_DISPLAY_INSTANCE* pDisplayInstance = (_ST_MMSDK_DISPLAY_INSTANCE*)DisplayItem;
    EN_MMSDK_MEDIA_TYPE enMediaType = pDisplayInstance->stDisplayInfo.enMediaType;

#if (MM_3D_ENABLE == 1)
    if (pDisplayInstance->bISSet3D)
    {
//Disalbe HDMI 3D Mode when window close.
        HDMITX_VIDEO_3D_STRUCTURE eHDMITx3DMode = HDMITx_VIDEO_3D_Not_in_Use;
        _HDMITx_3D_Mapping(E_MSAPI_XC_3D_OUTPUT_MODE_NONE, &eHDMITx3DMode);
        msAPI_XC_HDMITx_Set3DMode(TRUE, eHDMITx3DMode);
    }
#endif

    if (enMediaType == E_MMSDK_MEDIA_TYPE_MOVIE)
    {
        PT_Display_BlackScreen(DisplayItem, TRUE);
    }

#ifdef HDR_MM_DEV
#if (MI_ENABLE == 0)
    msAPI_XC_GetHDRSeamlessFlag(&bSeamlessEnable);

    if(STREAM_IS_HDR || DOVI_HDR_MODE || bSeamlessEnable)
    {
#if DOVI_HDR_MODE
        msAPI_HDMITx_SetDolbyVisionCtrl(FALSE);
#else
        MS_BOOL bSendCFD = FALSE;
        msAPI_XC_CFD_Control(NULL,E_MSAPI_XC_CFD_SET_INFOFRAME_FROM_CFD,&bSendCFD);
#endif
    }
#endif
#endif

#if defined(MMSDK_DMS_ENABLE) && (MMSDK_DMS_ENABLE == 1)
    if(pDisplayInstance ->u8DIPWin == E_MSAPI_XC_MAIN_WINDOW)
#endif
    {
        PT_Display_SetDSOnOFF(DisplayItem,FALSE);
    }

    if (NULL != gstSysCusFunPtr.pCloseWindow)
    {
        MMSDK_BOOL bRet = FALSE;
        MMSDK_U16 u16MediaType;

        u16MediaType = enMediaType;
        bRet = gstSysCusFunPtr.pCloseWindow(u16MediaType);
        return bRet;
    }
    return TRUE;
}


void PT_Display_FinalizeDisplayPath(PT_DISPLAYITEM DisplayItem)
{
    PT_DISP_DBG("");
    //[River]
    //It seems HB disable video mute by self.
    //msAPI_XC_SetVideoMute(TRUE,MAIN_WINDOW);
    // [River]
    // for HB, do not delete display path now?
}


MMSDK_BOOL PT_Display_RegisterCallback(PT_DISPLAYITEM DisplayItem, void * pClass, pfnDisplayCallback pCallback)
{
    PT_DISP_DBG("");
    //g_pfnDisplayCallback = pCallback;
    //g_pDislpayClass = pClass;

    if (DisplayItem == NULL)
    {
        PT_DISP_ERR("DisplayItem is NULL\n");
        return FALSE;
    }
    _ST_MMSDK_DISPLAY_INSTANCE* pDisplayInstance = (_ST_MMSDK_DISPLAY_INSTANCE*)DisplayItem;
    pDisplayInstance->pfnDisplayCallback = pCallback;
    pDisplayInstance->pDislpayClass = pClass;
    return TRUE;
}

MMSDK_BOOL PT_Display_Initialize(PT_DISPLAYITEM* pDisplayItem)
{
    PT_DISP_DBG("");
    if (pDisplayItem == NULL)
    {
        PT_DISP_ERR("pDisplayItem is NULL\n");
        return FALSE;
    }

    _ST_MMSDK_DISPLAY_INSTANCE* pDisplayInstance = NULL;
    pDisplayInstance = (_ST_MMSDK_DISPLAY_INSTANCE*)PT_MsOS_AllocateMemory(sizeof(_ST_MMSDK_DISPLAY_INSTANCE));
    _PT_Display_ClearArguments(pDisplayInstance);
    if(NULL != gstSysCusFunPtr.pInitialize)
    {
        PT_SYS_CusInitInfo stCusInitInfo;
        memset(&stCusInitInfo, 0x00, sizeof(stCusInitInfo));
        stCusInitInfo.bAvpEnable = FALSE;
        stCusInitInfo.pDispItem = (void *)pDisplayInstance;
        stCusInitInfo.u32SizeofDispItem = sizeof(_ST_MMSDK_DISPLAY_INSTANCE);
        stCusInitInfo.u32SizeofStructure = sizeof(stCusInitInfo);
        gstSysCusFunPtr.pInitialize(&stCusInitInfo);
        *pDisplayItem  = (void*)pDisplayInstance;
        return TRUE;
    }
    *pDisplayItem  = (void*)pDisplayInstance;
    return TRUE;
}


MMSDK_BOOL PT_Display_Finalize(PT_DISPLAYITEM* pDisplayItem)
{
#ifdef HDR_MM_DEV
#if (MI_ENABLE == 0)
        MS_BOOL bSeamlessEnable = FALSE;
#endif
#endif
    PT_DISP_DBG("");
    if ((pDisplayItem == NULL) || (*pDisplayItem == NULL))
    {
        PT_DISP_ERR("pDisplayItem is NULL\n");
        return FALSE;
    }

    if(NULL != gstSysCusFunPtr.pDispFinalize)
    {
        gstSysCusFunPtr.pDispFinalize(NULL);
    }

#ifdef HDR_MM_DEV
#if (MI_ENABLE == 0)
    msAPI_XC_GetHDRSeamlessFlag(&bSeamlessEnable);
    if(STREAM_IS_HDR || bSeamlessEnable)
    {
        _gu32HDRMetadataType = E_MMSDK_HDR_TYPE_CTL_FLAG_NONE;
#if (MMSDK_HDR_MODE == 1)
        MS_BOOL bHDR = FALSE;
        msAPI_XC_SetCommand(NULL, E_MSAPI_XC_CMD_SET_HDR, &bHDR);
#endif
    }
#endif
#endif

#ifdef AVP_ENABLE
    _ST_MMSDK_DISPLAY_INSTANCE* pDisplayInstance = (_ST_MMSDK_DISPLAY_INSTANCE*)(*pDisplayItem);

    if(u8CountSetSC1Window >0)  u8CountSetSC1Window--;

    PT_Display_CloseWindow(*pDisplayItem);
    //Clean ID BitMask
    u8Display_Instance_BitMask &= ~( pDisplayInstance->u8ID_BitMask);

    //Clean DIPWIN BitMask
    if((pDisplayInstance->u8Flow == DIP_FLOW) && (pDisplayInstance->u8DIPWin != DIP_Win_NonUse))
    {
#if defined(MMSDK_MultiWin_MODE) && (MMSDK_MultiWin_MODE == 0)
#else
#if (MMSDK_DMS_ENABLE == 1)
        if(pDisplayInstance->u8DIPWin == E_MSAPI_XC_MAIN_WINDOW)
        {
            // Clear MVOP setting
            MApi_DMS_ClearDigitalDecodeSignalInfo(pDisplayInstance->u8DIPWin);
        }
#else
        // Clean DIPWin
        u8DIP_Window_BitMask &= ~( BIT(pDisplayInstance->u8DIPWin));
#endif
        //Finalize DIP
        msAPI_XC_DIPMultiView_Finalize(pDisplayInstance->u8DIPWin);
#endif
    }
    else
#endif
    {
        MDrv_MVOP_Enable(FALSE);
        MDrv_MVOP_Exit();
    }

    _PT_Display_ClearArguments(*pDisplayItem);
    PT_MsOS_FreeMemory(*pDisplayItem);
    *pDisplayItem = NULL;


#ifdef HDR_MM_DEV
#if (MI_ENABLE == 0)
#if DOVI_HDR_MODE
    msAPI_HDMITx_SetDolbyVisionCtrl(FALSE);
    msAPI_XC_CFD_Control(NULL, E_MSAPI_XC_CFD_DOVI_RESET, NULL);
#else
    if(STREAM_IS_HDR || bSeamlessEnable)
    {
        PT_DISP_DBG("PT_Display_Finalize End CFD fire!!!!!!\n");
        msAPI_XC_CFD_Control(NULL, E_MSAPI_XC_CFD_FIRE, NULL);
    }
    MS_BOOL bSendCFD = FALSE;
    msAPI_XC_CFD_Control(NULL,E_MSAPI_XC_CFD_SET_INFOFRAME_FROM_CFD,&bSendCFD);
#endif //#if DOVI_HDR_MODE
#endif //MI_ENABLE
#endif //HDR_MM_DEV
    return TRUE;
}

MMSDK_BOOL PT_Display_BlackScreen(PT_DISPLAYITEM DisplayItem, MMSDK_BOOL bOnOff)
{
#ifdef HDR_MM_DEV
#if (MI_ENABLE == 0)
    MS_BOOL bSeamlessEnable = FALSE;
#endif
#endif
    PT_DISP_DBG("Enable:%d", bOnOff);

    if (DisplayItem == NULL)
    {
        PT_DISP_ERR("DisplayItem is NULL\n");
        return FALSE;
    }

    _ST_MMSDK_DISPLAY_INSTANCE* pDisplayInstance = (_ST_MMSDK_DISPLAY_INSTANCE*)DisplayItem;
    EN_MMSDK_MEDIA_TYPE enMediaType = pDisplayInstance->stDisplayInfo.enMediaType;

    if (NULL != gstSysCusFunPtr.pBlackScreen)
    {
        MMSDK_BOOL bRet = FALSE;
        MMSDK_U16 u16MediaType;

        u16MediaType = enMediaType;
        bRet = gstSysCusFunPtr.pBlackScreen(u16MediaType, bOnOff);
        return bRet;
    }

#if defined(MMSDK_DMS_ENABLE) && (MMSDK_DMS_ENABLE == 1)
    if((pDisplayInstance ->u8DIPWin == E_MSAPI_XC_MAIN_WINDOW) && (bOnOff == FALSE) &&
        (msAPI_XC_IsBlackVideoEnable_EX(&g_stXC_PT_DeviceId[0], E_XC_EX_MAIN_WINDOW) == FALSE)){
        return TRUE;
    }
#endif

    if(bOnOff == FALSE && PT_SYS_GetMMPhotoPath() == 0 && enMediaType == E_MMSDK_MEDIA_TYPE_PHOTO)
        return FALSE;

#ifdef HDR_MM_DEV
#if (MI_ENABLE == 0)
    msAPI_XC_GetHDRSeamlessFlag(&bSeamlessEnable);
    if(bOnOff == FALSE && _gbFireHDR == TRUE && (STREAM_IS_HDR || DOVI_HDR_MODE || bSeamlessEnable)
#ifdef AVP_ENABLE
        && (pDisplayInstance ->u8DIPWin == E_MSAPI_XC_MAIN_WINDOW)
#endif
    )
    {
        MS_U8* aa = (MS_U8*)MsOS_PA2KSEG1((MS_PHY)PT_SYS_GetMmapInfo(NULL, E_MMSDK_BUF_DV_XC, E_MMSDK_BUF_INFO_PHY_ADDR));
        PT_DISP_DBG("####### CFD SHM : 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x\n",
            *aa,*(aa+1),*(aa+2),*(aa+3),*(aa+4),*(aa+5),*(aa+6),*(aa+7),*(aa+8),*(aa+9));
        _gbFireHDR = FALSE;
#if DOVI_HDR_MODE
        msAPI_HDMITx_SetDolbyVisionCtrl(TRUE);
        msAPI_XC_CFD_Control(NULL, E_MSAPI_XC_CFD_FIRE, NULL);
#else
        MS_U32 value = 0;
        value |= _gu8HDRCtrl;
        MS_BOOL bSendCFD = TRUE;
        SendCfd(E_XC_CFD_CTRL_SET_HDR_ONOFF_SETTING, &value, sizeof(MS_U32));
        msAPI_XC_CFD_Control(NULL, E_MSAPI_XC_CFD_FIRE, NULL);
        MsOS_DelayTask(100);
        msAPI_XC_CFD_Control(NULL,E_MSAPI_XC_CFD_SET_INFOFRAME_FROM_CFD,&bSendCFD);
#endif
    }
#if defined(MMSDK_HDR_TMO_VR_ENABLE) && (MMSDK_HDR_TMO_VR_ENABLE == 1)
    MDrv_PQ_LoadTMOSettings(PQ_MAIN_WINDOW);
#endif

#endif
#endif

#if defined(MMSDK_DMS_ENABLE) && (MMSDK_DMS_ENABLE == 1)
    if(enMediaType == E_MMSDK_MEDIA_TYPE_MOVIE)
    {
        E_MSAPI_DIP_MULTIVIEW_MUTE_ACTION eMultiViewMuteAct = (bOnOff == TRUE) ? E_MSAPI_DIP_MULTIVIEW_MUTE_ON : E_MSAPI_DIP_MULTIVIEW_MUTE_OFF;
        msAPI_XC_DIPMultiView_SetMute(pDisplayInstance->u8DIPWin, eMultiViewMuteAct);
    }
    else if (enMediaType == E_MMSDK_MEDIA_TYPE_PHOTO)
#endif
    {

        msAPI_XC_SetVideoMuteByMode_EX(&DEVICE_ID_0, bOnOff, E_MSAPI_XC_MAIN_WINDOW, E_MSAPI_XC_VIDEO_MUTE_MODE_AUTO_SYNC);
        //Don't mute device 1 because of VE OSD can't be muted in OPtoVE mode
        //For DDI flow, mute does not delay task or wait
    }
    return TRUE;
}


MMSDK_BOOL PT_Display_FreezeImage(PT_DISPLAYITEM DisplayItem, MMSDK_BOOL bOnOff)
{
    PT_DISP_DBG("OnOff=%d", bOnOff);

    if (NULL != gstSysCusFunPtr.pFreezeImage)
    {
        MMSDK_BOOL bRet = FALSE;
        bRet = gstSysCusFunPtr.pFreezeImage(bOnOff);
        return bRet;
    }

#if (MMSDK_DISPLAY_DUALXC_PORTING == 1)
    MApi_XC_EX_FreezeImg((XC_DEVICE_ID*)&g_stXC_PT_DeviceId[0], bOnOff, E_XC_EX_MAIN_WINDOW);
#else
    MApi_XC_FreezeImg(bOnOff, MAIN_WINDOW);
#endif
    return TRUE;
}

MMSDK_BOOL PT_Display_SetDSOnOFF(PT_DISPLAYITEM DisplayItem, MMSDK_BOOL bEnableDS)
{
    PT_DISP_DBG("[ (bEnableDS=%d)]", bEnableDS);
    if (DisplayItem == NULL)
    {
        PT_DISP_ERR("DisplayItem is NULL\n");
        return FALSE;
    }
     _ST_MMSDK_DISPLAY_INSTANCE* pDisplayInstance = (_ST_MMSDK_DISPLAY_INSTANCE*)DisplayItem;
    if (NULL != gstSysCusFunPtr.pSetDispParams)
    {
        PT_SYS_CusInitInfo stCusInitInfo;
        memset(&stCusInitInfo, 0x00, sizeof(stCusInitInfo));
        stCusInitInfo.pDispItem = (void *)pDisplayInstance;
        stCusInitInfo.u32SizeofStructure = sizeof(stCusInitInfo);
        stCusInitInfo.bAvpEnable = pDisplayInstance->bIsAvpFlow;
        stCusInitInfo.u32SizeofDispItem = sizeof(_ST_MMSDK_DISPLAY_INSTANCE);
        if(gstSysCusFunPtr.pSetDispParams(&stCusInitInfo) == FALSE)
        {
            PT_DISP_ERR("Failed to Set Display Parameters!!\n");
        }
    }
    if (NULL != gstSysCusFunPtr.pSetDSOnOFF)
    {
        MMSDK_BOOL bRet = FALSE;
        bRet = gstSysCusFunPtr.pSetDSOnOFF(bEnableDS);
        return bRet;
    }

#if (MMSDK_DISPLAY_DUALXC_PORTING == 1)
    XC_EX_DynamicScaling_Info stDS_Info;
#else
    XC_DynamicScaling_Info stDS_Info;
#endif
    memset(&stDS_Info, 0, sizeof(XC_DynamicScaling_Info));
    if (TRUE == bEnableDS)
    {
        stDS_Info.u32DS_Info_BaseAddr = pDisplayInstance->stDisplayInfo.u32DSInfoBaseAddr;
        stDS_Info.u8MIU_Select = pDisplayInstance->stDisplayInfo.u8DSMIUSelect;
        stDS_Info.u8DS_Index_Depth = pDisplayInstance->stDisplayInfo.u8DSIndexDepth;
        stDS_Info.bOP_DS_On = TRUE;
        stDS_Info.bIPS_DS_On = FALSE;
        stDS_Info.bIPM_DS_On = TRUE;
        PT_DISP_DBG("\n DS:IP on %d, OP on %d, depth=%d, Addr=0x%"DTC_MS_PHY_x", MIU=%d\n", stDS_Info.bIPM_DS_On, stDS_Info.bOP_DS_On, stDS_Info.u8DS_Index_Depth,
            stDS_Info.u32DS_Info_BaseAddr, stDS_Info.u8MIU_Select);
    }
    else
    {
        stDS_Info.bOP_DS_On = FALSE;
        stDS_Info.bIPS_DS_On = FALSE;
        stDS_Info.bIPM_DS_On = FALSE;
        stDS_Info.u8DS_Index_Depth = 0;
    }

    msAPI_XC_SetCommand(NULL, E_MSAPI_XC_CMD_SET_DYNAMIC_SCALING, &bEnableDS);

#if (MMSDK_DISPLAY_DUALXC_PORTING == 1)
    return MApi_XC_EX_SetDynamicScaling((XC_DEVICE_ID*)&g_stXC_PT_DeviceId[0], &stDS_Info, sizeof(XC_EX_DynamicScaling_Info), E_XC_EX_MAIN_WINDOW);
#else
    return MApi_XC_SetDynamicScaling( &stDS_Info, sizeof(XC_DynamicScaling_Info), MAIN_WINDOW);
#endif
}

typedef enum
{
    MVOP_SOURCE_FIELD
    , MVOP_OUTPUT_FIELD
} MVOP_OP_FIELD_TYPE;

MS_BOOL _DS_Get_VirtualBox_Info(DS_VBOX_INFO *pstDS_VboxInfo)
{
    if (pstDS_VboxInfo == NULL)
    {
        return FALSE;
    }

#ifdef MMSDK_4K2K_ENABLE
    pstDS_VboxInfo->u16VBox_Htotal = 4096;
    pstDS_VboxInfo->u16VBox_Vtotal = 2160;
#else
    pstDS_VboxInfo->u16VBox_Htotal = 1920;
    pstDS_VboxInfo->u16VBox_Vtotal = 1088;
#endif

    return TRUE;
}

MS_BOOL _DS_SendXCStatus2Firmware(MS_U32 u32FM_Buf_Base, const MSAPI_XC_WINDOW_TYPE *pstCropWin, const MSAPI_XC_WINDOW_TYPE *pstDispWin, SCALER_WIN eWin)
{
    //ST_VIDEOPLAYER_INFO* pVideoPlayer = (ST_VIDEOPLAYER_INFO*)pstVideoPlayer;
#if (MMSDK_DISPLAY_DUALXC_PORTING == 1)
    XC_EX_ApiStatus stXCStatus;
#else
    XC_ApiStatus stXCStatus;
#endif
    ST_MM_DS_XC_STATUS *pstMMDS_Status;

    PT_DISP_DBG_DS("DS_SendXCStatus2Firmware %"DTC_MS_U32_x"\n", u32FM_Buf_Base);

    // direct write to shared memory with firmware
    //pstMMDS_Status = (ST_MM_DS_XC_STATUS *) MsOS_PA2KSEG1(u32FM_Buf_Base);
    pstMMDS_Status = (ST_MM_DS_XC_STATUS *) MsOS_PA2KSEG1(u32FM_Buf_Base);

    // get scaler information
#if (MMSDK_DISPLAY_DUALXC_PORTING == 1)
    CHECK_MAPI_XC_RETURN_VALUE(MApi_XC_EX_GetStatus((XC_DEVICE_ID*)&g_stXC_PT_DeviceId[0], &stXCStatus, eWin));
#else
    CHECK_MAPI_XC_RETURN_VALUE(MApi_XC_GetStatus(&stXCStatus, eWin));
#endif
    MS_U32 u32Timeout = MsOS_GetSystemTime() + 5000;
    // copy scaler related information
    while(pstMMDS_Status->bFWIsUpdating)
    {
        MsOS_DelayTask(1);
        if(MsOS_GetSystemTime() > u32Timeout)
        {
            PT_DISP_ERR("DS Send XC Status to Firmware , bFWIsUpdating time out\n");
            break;
        }
    }


    pstMMDS_Status->bFWGotXCInfo = FALSE;
    pstMMDS_Status->bHKIsUpdating = TRUE;
    MsOS_FlushMemory();

    DS_VBOX_INFO pstDSVBox =  {.u16VBox_Htotal = DYNAMIC_SCALING_VB_H, .u16VBox_Vtotal = DYNAMIC_SCALING_VB_V};

    if (TRUE != _DS_Get_VirtualBox_Info(&pstDSVBox))
    {
        // for backward capability
        PT_DISP_ERR("Warning!!! _DS_Get_VirtualBox_Info return fail, take FHD as VBox size");
    }

    PT_DISP_DBG("vdec_debug VBox Height   %d", pstDSVBox.u16VBox_Vtotal);
    PT_DISP_DBG("vdec_debug VBox Width    %d", pstDSVBox.u16VBox_Htotal);
    stXCStatus.stCapWin.height = pstDSVBox.u16VBox_Vtotal;
    stXCStatus.stCapWin.width = pstDSVBox.u16VBox_Htotal;

    //stXCStatus.stCropWin.height = 1080;
    //stXCStatus.stCropWin.width = 1920;
    PT_DISP_DBG("use video Crop\n");
    stXCStatus.stCropWin.x = pstCropWin->x;
    stXCStatus.stCropWin.y = pstCropWin->y;
    stXCStatus.stCropWin.width = pstCropWin->width;
    stXCStatus.stCropWin.height = pstCropWin->height;

    memcpy(&pstMMDS_Status->stCapWin, &stXCStatus.stCapWin, sizeof(MS_WINDOW_TYPE));
    memcpy(&pstMMDS_Status->stCropWin, &stXCStatus.stCropWin, sizeof(MS_WINDOW_TYPE));
    memcpy(&pstMMDS_Status->stDispWin, &stXCStatus.stDispWin, sizeof(MS_WINDOW_TYPE));
    pstMMDS_Status->u16H_SizeAfterPreScaling = stXCStatus.u16H_SizeAfterPreScaling;
    pstMMDS_Status->u16V_SizeAfterPreScaling = stXCStatus.u16V_SizeAfterPreScaling;

    pstMMDS_Status->u32IPMBase0 = stXCStatus.u32IPMBase0;
    pstMMDS_Status->u32IPMBase1 = stXCStatus.u32IPMBase1;
    pstMMDS_Status->u32IPMBase2 = stXCStatus.u32IPMBase2;
    pstMMDS_Status->bLinearMode = stXCStatus.bLinearMode;
    pstMMDS_Status->u8BitPerPixel = stXCStatus.u8BitPerPixel;
    pstMMDS_Status->bInterlace = stXCStatus.bInterlace;
    pstMMDS_Status->u16IPMOffset = stXCStatus.u16IPMOffset;
    pstMMDS_Status->u8EnhanceModeSetting = pstMMDS_Status->u8EnhanceModeCap = 0;
    //pstMMDS_Status->bFBL = pstMMDS_Status->bR_FBL = pstMMDS_Status->bFRC = FALSE;
#if (MMSDK_DISPLAY_DUALXC_PORTING == 1)
    pstMMDS_Status->bMirrorMode = MApi_XC_EX_GetMirrorModeTypeEx((XC_DEVICE_ID*)&g_stXC_PT_DeviceId[0], E_XC_EX_MAIN_WINDOW);
    pstMMDS_Status->u8StoreFrameNum = MApi_XC_EX_Get_StoreFrameNum((XC_DEVICE_ID*)&g_stXC_PT_DeviceId[0], E_XC_EX_MAIN_WINDOW);

    PNL_DeviceId stPNL_DeviceId = {g_stXC_PT_DeviceId[0].u32Version, g_stXC_PT_DeviceId[0].u32Id};
    pstMMDS_Status->u16PNL_HST = g_IPanelEx.HStart(&stPNL_DeviceId);
    pstMMDS_Status->u16PNL_HEND = g_IPanelEx.HStart(&stPNL_DeviceId) + g_IPanelEx.Width(&stPNL_DeviceId) - 1;
    pstMMDS_Status->u16PNL_VST = g_IPanelEx.VStart(&stPNL_DeviceId);
    pstMMDS_Status->u16PNL_VEND = g_IPanelEx.VStart(&stPNL_DeviceId) + g_IPanelEx.Height(&stPNL_DeviceId) - 1;
#else
    pstMMDS_Status->bMirrorMode = MApi_XC_GetMirrorModeType();
    pstMMDS_Status->u8StoreFrameNum = MApi_XC_Get_StoreFrameNum(MAIN_WINDOW);

    pstMMDS_Status->u16PNL_HST = g_IPanel.HStart();
    pstMMDS_Status->u16PNL_HEND = g_IPanel.HStart() + g_IPanel.Width() - 1;
    pstMMDS_Status->u16PNL_VST = g_IPanel.VStart();
    pstMMDS_Status->u16PNL_VEND = g_IPanel.VStart() + g_IPanel.Height() - 1;
#endif

    pstMMDS_Status->u16MVOPHStart = MDrv_MVOP_GetHStart();
    pstMMDS_Status->u16MVOPVStart = MDrv_MVOP_GetVStart();

    //add by new vdec fw use
    pstMMDS_Status->u16VirtualBoxWidth = stXCStatus.stCapWin.width;
    pstMMDS_Status->u16VirtualBoxHeight = stXCStatus.stCapWin.height;
    pstMMDS_Status->bUseVBoxOfHK = 1;

    PT_DISP_DBG_DS("### IPMBase0 = 0x%"DTC_MS_PHY_x" ", (MS_PHY)stXCStatus.u32IPMBase0);
    PT_DISP_DBG_DS("### IPMBase1 = 0x%"DTC_MS_PHY_x" ", (MS_PHY)stXCStatus.u32IPMBase1);
    PT_DISP_DBG_DS("### IPMBase2 = 0x%"DTC_MS_PHY_x" ", (MS_PHY)stXCStatus.u32IPMBase2);
    PT_DISP_DBG_DS("### pstMMDS_Status->bLinearMode = %u ", pstMMDS_Status->bLinearMode);
    PT_DISP_DBG_DS("### pstMMDS_Status->u8BitPerPixel = %u ", pstMMDS_Status->u8BitPerPixel);
    PT_DISP_DBG_DS("### pstMMDS_Status->bInterlace = %u ", pstMMDS_Status->bInterlace);
    PT_DISP_DBG_DS("### pstMMDS_Status->bMirrorMode = %u ", pstMMDS_Status->bMirrorMode);
    PT_DISP_DBG_DS("### pstMMDS_Status->u16IPMOffset = %u ", pstMMDS_Status->u16IPMOffset);
    PT_DISP_DBG_DS("### pstMMDS_Status->u8StoreFrameNum = %u ", pstMMDS_Status->u8StoreFrameNum);
    PT_DISP_DBG_DS("### pstMMDS_Status->u16VirtualBoxWidth  = %u ", pstMMDS_Status->u16VirtualBoxWidth );
    PT_DISP_DBG_DS("### pstMMDS_Status->u16VirtualBoxHeight = %u ", pstMMDS_Status->u16VirtualBoxHeight);

    PT_DISP_DBG_DS("u8DSVersion %d ", pstMMDS_Status->u8DSVersion);

    PT_DISP_DBG_DS("Cap  %d, %d, %d, %d ", pstMMDS_Status->stCapWin.x, pstMMDS_Status->stCapWin.y,
               pstMMDS_Status->stCapWin.width, pstMMDS_Status->stCapWin.height);
    PT_DISP_DBG_DS("Crop %d, %d, %d, %d ", pstMMDS_Status->stCropWin.x, pstMMDS_Status->stCropWin.y,
               pstMMDS_Status->stCropWin.width, pstMMDS_Status->stCropWin.height);
    PT_DISP_DBG_DS("Disp %d, %d, %d, %d ", pstMMDS_Status->stDispWin.x, pstMMDS_Status->stDispWin.y,
               pstMMDS_Status->stDispWin.width, pstMMDS_Status->stDispWin.height);

    PT_DISP_DBG_DS("IPMBase 0/1/2 = %"DTC_MS_U32_x", %"DTC_MS_U32_x", %"DTC_MS_U32_x" ", pstMMDS_Status->u32IPMBase0,
               pstMMDS_Status->u32IPMBase1, pstMMDS_Status->u32IPMBase2);
    PT_DISP_DBG_DS("Linear %d, Bit/Pixel %d, Interlace %d ", pstMMDS_Status->bLinearMode,
               pstMMDS_Status->u8BitPerPixel, pstMMDS_Status->bInterlace);
    PT_DISP_DBG_DS("PNL %d, %d, %d, %d ", pstMMDS_Status->u16PNL_HST, pstMMDS_Status->u16PNL_HEND, pstMMDS_Status->u16PNL_VST,
               pstMMDS_Status->u16PNL_VEND);

    pstMMDS_Status->bHKIsUpdating = FALSE;
    MsOS_FlushMemory();

    return TRUE;
}

#if defined(MMSDK_DMS_ENABLE) && (MMSDK_DMS_ENABLE == 1)
static MMSDK_U8 _PT_Display_GetDSIndexDepth(EN_MMSDK_VIDEO_CODEC eVidCodec)
{
    MMSDK_U8 u8IndexDepth = 0;
    #define DS_BUFFER_DEPTH     16
    #define DS_BUFFER_DEPTH_EX  32

    if (eVidCodec == E_MMSDK_VIDEO_CODEC_HEVC_DV
#ifdef HDR_MM_DEV
        || STREAM_HDR_TYPE_TCH
#endif
    )
    {
        u8IndexDepth = 255;
    }
    else
    {
        if (MApi_XC_Is_SupportSWDS())
            u8IndexDepth = DS_BUFFER_DEPTH_EX;
        else
            u8IndexDepth = DS_BUFFER_DEPTH;
    }

    return u8IndexDepth;
}
#endif

#ifdef AVP_ENABLE
static MMSDK_BOOL _PT_Display_GetWindowID(MMSDK_U8 *pu8WindowID)
{
#if defined(MMSDK_DMS_ENABLE) && (MMSDK_DMS_ENABLE == 1)
    E_MSAPI_XC_WINDOW eWindowID = 0;
    E_MSAPI_XC_RESULT eRet = E_MSAPI_XC_NOT_SUPPORT;

    eRet = msAPI_XC_DIPMultiView_CreateWindow(NULL, &eWindowID);
    if (eRet != E_MSAPI_XC_OK)
    {
        PT_DISP_ERR("Get Window ID fail, eRet = %d", eRet);
        return FALSE;
    }
    *pu8WindowID = (MS_U8)eWindowID;
#else //(MMSDK_DMS_ENABLE == 1)
    MS_U8 DIPWIN = 0;

    //Get Free DIPWIN
    for(DIPWIN=0; DIPWIN<sizeof(u8DIP_Window_BitMask)*8; DIPWIN++)
    {
        if(!((u8DIP_Window_BitMask>>DIPWIN) & BIT(0)))
        {
            u8DIP_Window_BitMask |= BIT(DIPWIN);
            break;
        }
    }
    *pu8WindowID = DIPWIN;
#endif //(MMSDK_DMS_ENABLE == 1)

    return TRUE;
}
#endif

MMSDK_BOOL PT_Display_SetXCWindow(PT_DISPLAYITEM DisplayItem, const ST_MMSDK_XC_RECT *pstCropRect, const ST_MMSDK_XC_RECT *pstDispRect, const MMSDK_U32 u32MuteTimeMS)
{
    PT_DISP_DBG("Crop:%d %d (%d,%d), Disp:%d %d (%d,%d)", pstCropRect->u16X, pstCropRect->u16Y,
         pstCropRect->u16Width, pstCropRect->u16Height, pstDispRect->u16X, pstDispRect->u16Y,
         pstDispRect->u16Width, pstDispRect->u16Height);

    if (DisplayItem == NULL)
    {
        PT_DISP_ERR("DisplayItem is NULL\n");
        return FALSE;
    }
    _ST_MMSDK_DISPLAY_INSTANCE* pDisplayInstance = (_ST_MMSDK_DISPLAY_INSTANCE*)DisplayItem;
    EN_MMSDK_MEDIA_TYPE enMediaType = pDisplayInstance->stDisplayInfo.enMediaType;

    if((NULL != gstSysCusFunPtr.pSetXCWindow2) || ((NULL != gstSysCusFunPtr.pSetXCWindow)))
    {
        ST_MMSDK_XC_RECT stCropRect, stDispRect;
        MMSDK_BOOL bRet = FALSE;
        MMSDK_BOOL bEnableDS = FALSE;
        MMSDK_BOOL bsetDsOnOff = pDisplayInstance->stDisplayInfo.bEnableDynScaling;

        PT_DISP_DBG("bsetDsOnOff=%d\n",bsetDsOnOff);
        PT_Display_SetDSOnOFF(DisplayItem, bsetDsOnOff);
        bEnableDS = CALLBACK(E_MMSDK_DISPLAY_CALLBACK_IS_ENABLE_DYNAMIC_SCALING, NULL, NULL);

        memcpy(&stCropRect, pstCropRect, sizeof(stCropRect));
        memcpy(&stDispRect, pstDispRect, sizeof(stDispRect));

        PT_SYS_CusWindowInfo stCusWindowInfo;
        memset(&stCusWindowInfo, 0x00, sizeof(stCusWindowInfo));
        stCusWindowInfo.bIsDSEnabled = bEnableDS;
        stCusWindowInfo.pstCropRect = &stCropRect;
        stCusWindowInfo.pstDispRect = &stDispRect;
        stCusWindowInfo.u32SizeofStructure = sizeof(stCusWindowInfo);
#ifdef AVP_ENABLE
        stCusWindowInfo.u8Flow = pDisplayInstance->u8Flow;
        stCusWindowInfo.u8Win = pDisplayInstance->u8DIPWin;
        u8DIP_Window_BitMask |= BIT(pDisplayInstance->u8Flow);
#endif

        if(NULL != gstSysCusFunPtr.pSetXCWindow2)
        {
            bRet = gstSysCusFunPtr.pSetXCWindow2(enMediaType, &stCusWindowInfo, NULL);
        }
        else if(NULL != gstSysCusFunPtr.pSetXCWindow)
        {
            bRet = gstSysCusFunPtr.pSetXCWindow(enMediaType, bEnableDS, (void*)&stCropRect, (void*)&stDispRect);
        }

#ifdef AVP_ENABLE
        if(pDisplayInstance ->u8Flow == DIP_FLOW)
            return bRet;
#endif
        //Update DS XC info
        if ((pDisplayInstance->bIsUpDSInfo == FALSE) && enMediaType == E_MMSDK_MEDIA_TYPE_MOVIE
                && (CALLBACK(E_MMSDK_DISPLAY_CALLBACK_IS_ENABLE_DYNAMIC_SCALING, NULL, NULL)))
        {
            if (NULL != gstSysCusFunPtr.pDSSendXCStatus2Firmware)
            {
                bRet &= gstSysCusFunPtr.pDSSendXCStatus2Firmware((void*)&stCropRect, (void*)&stDispRect, (MMSDK_U8) MAIN_WINDOW);;
                pDisplayInstance->bIsUpDSInfo = TRUE;
            }
        }
        return bRet;
    }

    MSAPI_XC_WINDOW_TYPE cropWin, dispWin;

    if (u32MuteTimeMS > 0)
    {
        PT_Display_BlackScreen(DisplayItem, TRUE);
    }

    //msAPI_XC_SetDebugLevel(E_MSAPI_XC_DBG_LEVEL_API);

    //[River] - Crop and Display window are calculated by mmsdk (have considered aspect ratio)
    //But msAPI setwin will calculate aspect ratio again.
    //Is any problem here?

    //[River] - DS here
    cropWin.x = pstCropRect->u16X;
    cropWin.y = pstCropRect->u16Y;
    cropWin.width = pstCropRect->u16Width;
    cropWin.height = pstCropRect->u16Height;
    dispWin.x = pstDispRect->u16X;
    dispWin.y = pstDispRect->u16Y;
    dispWin.width = pstDispRect->u16Width;
    dispWin.height = pstDispRect->u16Height;

#if (MM_3D_ENABLE == 1)
    if (pDisplayInstance->bIsMM3D)
    {
        if (pDisplayInstance->stDisplayInfo.eVideoType == E_MMSDK_VIDEO_CODEC_MVC)
        {
            cropWin.height = cropWin.height * 2 + pDisplayInstance->stDisplayInfo.u16CropBottom;
        }
        else
        {
            if ((pDisplayInstance->stDisplayInfo.en3DLayout == E_MMSDK_3DLAYOUT_TOP_BOTTOM_RT) ||
                (pDisplayInstance->stDisplayInfo.en3DLayout == E_MMSDK_3DLAYOUT_TOP_BOTTOM_LT))
            {
                cropWin.height *= 2;
            }
            else if((pDisplayInstance->stDisplayInfo.en3DLayout == E_MMSDK_3DLAYOUT_SIDE_BY_SIDE_LF) ||
                    (pDisplayInstance->stDisplayInfo.en3DLayout == E_MMSDK_3DLAYOUT_SIDE_BY_SIDE_RF))
            {
                cropWin.width *= 2;
            }
        }
    }
#endif

#ifdef HDR_MM_DEV
#if (MI_ENABLE == 0)
#if DOVI_HDR_MODE
    if ((cropWin.width % 4) != 0)
    {
        MS_U32 u32CropWidth = cropWin.width;
        cropWin.width = (u32CropWidth / 4) * 4;
        PT_DISP_DBG("DV HW limition, crop width must be 4-align. %d -> %d\n", u32CropWidth, cropWin.width);
    }
#endif
#endif
#endif

#ifdef AVP_ENABLE
    if(enMediaType == E_MMSDK_MEDIA_TYPE_MOVIE)
    {
        MS_U8 WINID = 0;
        MS_U8 winLayer = 0;

        // Get Window Zorder
        WINID = log2(pDisplayInstance ->u8ID_BitMask);
        winLayer = PT_SYS_GetAVPZOrder(WINID);
        PT_DISP_DBG("[%s][%d]====winLayer=%d===\n",__FUNCTION__,__LINE__, winLayer);

        if(pDisplayInstance ->u8Flow == DIP_FLOW)
        {
#if defined(MMSDK_MultiWin_MODE) && (MMSDK_MultiWin_MODE == 0)
#else //defined(MMSDK_MultiWin_MODE) && (MMSDK_MultiWin_MODE == 0)
#if defined(MMSDK_DMS_ENABLE) && (MMSDK_DMS_ENABLE == 1)
            // Set DS on/off  (patch)
            if(pDisplayInstance ->u8DIPWin == E_MSAPI_XC_MAIN_WINDOW)
            {
                MMSDK_BOOL bDynScaling = TRUE;  // DMS on, DS always open

                ST_MMSDK_BUF_INFO stBuffInfo = {};
                EN_MMSDK_VIDEO_CODEC eVideoCodec = pDisplayInstance->stDisplayInfo.eVideoType;

                if(PT_SYS_GetMmapInfo(&stBuffInfo, E_MMSDK_BUF_XC_DS, 0) != 0)
                {
                    bDynScaling = TRUE;
                    pDisplayInstance->stDisplayInfo.u32DSInfoBaseAddr = stBuffInfo.u32Addr;
                    pDisplayInstance->stDisplayInfo.u32DSSize = stBuffInfo.u32Size;
                    pDisplayInstance->stDisplayInfo.u8DSMIUSelect = stBuffInfo.u32MiuNo;
                    pDisplayInstance->stDisplayInfo.u8DSIndexDepth = _PT_Display_GetDSIndexDepth(eVideoCodec);
                }
                else
                {
                    PT_DISP_DBG("Get E_MMSDK_BUF_XC_DS FAIL, Disable DS\n");
                    bDynScaling = FALSE;
                }
                PT_Display_SetDSOnOFF(DisplayItem, bDynScaling);
#ifdef HDR_MM_DEV
                _PT_Display_SetHDRInit();
                _PT_Display_EnableFrameBufferLess();
#endif
            }
#endif
            PT_DISP_DBG("[%s][%d]====pDisplayInstance ->u8ID_BitMask=%d===\n",__FUNCTION__,__LINE__,pDisplayInstance ->u8ID_BitMask);
            PT_DISP_DBG("[%s][%d]====pDisplayInstance ->u8Flow=%d===\n",__FUNCTION__,__LINE__,pDisplayInstance ->u8Flow);
            PT_DISP_DBG("[%s][%d]====pDisplayInstance->u8DIPWin=%d===\n",__FUNCTION__,__LINE__,pDisplayInstance->u8DIPWin);

            msAPI_XC_DIPMultiView_SetZOrder(&g_stDIP_PT_DeviceId, (E_MSAPI_XC_WINDOW)(pDisplayInstance->u8DIPWin), winLayer);
            msAPI_XC_SetWin_EX(&g_stDIP_PT_DeviceId, NULL, &cropWin, &dispWin, (E_MSAPI_XC_WINDOW)pDisplayInstance ->u8DIPWin);
            if(pDisplayInstance->bSetSC1Window == FALSE)
            {
                if(u8CountSetSC1Window == 0)    msAPI_XC_SetWin_EX(&g_stXC_PT_DeviceId[1], NULL, NULL, NULL, E_XC_EX_MAIN_WINDOW);

                pDisplayInstance->bSetSC1Window = TRUE;
                u8CountSetSC1Window++;
            }
#endif //defined(MMSDK_MultiWin_MODE) && (MMSDK_MultiWin_MODE == 0)

#if defined(MMSDK_DMS_ENABLE) && (MMSDK_DMS_ENABLE == 1)
            PT_Display_BlackScreen(DisplayItem, FALSE);
#endif
            return TRUE;
        }
        else if(pDisplayInstance ->u8Flow == XC_FLOW)
        {
#if defined(MMSDK_MultiWin_MODE) && (MMSDK_MultiWin_MODE == 0)
#else //defined(MMSDK_MultiWin_MODE) && (MMSDK_MultiWin_MODE == 0)
            msAPI_XC_DIPMultiView_SetZOrder(&g_stXC_PT_DeviceId[0], E_MSAPI_XC_MAIN_WINDOW, winLayer);
#endif //defined(MMSDK_MultiWin_MODE) && (MMSDK_MultiWin_MODE == 0)
        }
    }
#endif //AVP_ENABLE

    MMSDK_BOOL bsetDsOnOff = pDisplayInstance->stDisplayInfo.bEnableDynScaling;
    PT_DISP_DBG("bsetDsOnOff=%d\n",bsetDsOnOff);
    PT_Display_SetDSOnOFF(DisplayItem, bsetDsOnOff);

    PT_DISP_DBG("Callback ENABLE_DYNAMIC_SCALING = %d",
             CALLBACK(E_MMSDK_DISPLAY_CALLBACK_IS_ENABLE_DYNAMIC_SCALING, NULL, NULL));

#ifdef HDR_MM_DEV
    _PT_Display_SetHDRInit();
#endif

    if (CALLBACK(E_MMSDK_DISPLAY_CALLBACK_IS_ENABLE_DYNAMIC_SCALING, NULL, NULL) && enMediaType == E_MMSDK_MEDIA_TYPE_MOVIE)
    {
        MMSDK_U32 u32DynScalSize = pDisplayInstance->stDisplayInfo.u32DSSize;
        MMSDK_U32 u32DSAddr = pDisplayInstance->stDisplayInfo.u32DSInfoBaseAddr + u32DynScalSize;
        MMSDK_U32 u32CropBottom = pDisplayInstance->stDisplayInfo.u16CropBottom;
        MMSDK_U32 u32Height = pDisplayInstance->stDisplayInfo.u16SrcHeight;
        MMSDK_U32 u32Width = pDisplayInstance->stDisplayInfo.u16SrcWidth;

        DS_VBOX_INFO pstDSVBox =  {.u16VBox_Htotal = DYNAMIC_SCALING_VB_H, .u16VBox_Vtotal = DYNAMIC_SCALING_VB_V};
        if (TRUE != _DS_Get_VirtualBox_Info(&pstDSVBox))
        {
            // for backward capability
            PT_DISP_ERR("Warning!!! _DS_Get_VirtualBox_Info return fail, take FHD as VBox size");
        }

        /* [Temp note]
        1. For Kris/Kirin with 3M XC buffer and need set small display window, need recaculate Frame buffer level in msAPI_XC_SetWin_EX.
        2. Remove only SetWindow once and may result flash when MM_MoveView. This change is necessary when incorporate DS flow in porting layer into msapi.
        --> Side effect: MM_MoveView may flash.*/
#if (MMSDK_DISPLAY_DUALXC_PORTING == 1)
        if(pDisplayInstance->bFirstTimeSetWindow == TRUE)
        {
#endif
            msAPI_XC_SetAspectRatioType_EX(&DEVICE_ID_0, E_MSAPI_XC_MAIN_WINDOW, E_MSAPI_XC_ASPECT_RATIO_NORMAL);

            //use panel crop
            MSAPI_XC_VDEC_DispInfo stVidStatus;
            memset(&stVidStatus, 0, sizeof(MSAPI_XC_VDEC_DispInfo));

            if ( E_MSAPI_XC_OK != msAPI_XC_GetVDECInfo_EX_ByPath(E_MSAPI_XC_MAIN_DECODER_PATH,&stVidStatus) )
            {
                PT_DISP_ERR(" Video Get Info Failed!! \n");
                return FALSE;
            }
            /*
             * reset VDEC information and Set MVOP for Dynamic scaling
             * set size as maximal width and maximal height (it must be fixed!)
             */

            stVidStatus.u16HorSize          = pstDSVBox.u16VBox_Htotal; //+ stVidStatus.u16CropLeft + stVidStatus.u16CropRight;
            stVidStatus.u16VerSize          = pstDSVBox.u16VBox_Vtotal; //+ stVidStatus.u16CropTop + stVidStatus.u16CropBottom;
            stVidStatus.u32AspectWidth      = pstDSVBox.u16VBox_Htotal;
            stVidStatus.u32AspectHeight     = pstDSVBox.u16VBox_Vtotal;

            msAPI_XC_SetVDECInfo_EX_ByPath(E_MSAPI_XC_MAIN_DECODER_PATH, stVidStatus);
            MS_BOOL bDS = TRUE;
            msAPI_XC_SetCommand(NULL, E_MSAPI_XC_CMD_SET_DYNAMIC_SCALING, &bDS);
            msAPI_XC_SetWin_EX(&DEVICE_ID_0, NULL, &cropWin, &dispWin, E_MSAPI_XC_MAIN_WINDOW);

#if (MMSDK_DISPLAY_DUALXC_PORTING == 1)
            msAPI_XC_SetWin_EX(&DEVICE_ID_1, NULL, NULL, NULL, E_MSAPI_XC_MAIN_WINDOW);
#else
            msAPI_VE_SetMode();
#endif

            if(pDisplayInstance->bIsAvpFlow == FALSE)
            {
                MSAPI_XC_WINDOW_TYPE stDispWin1;

                stDispWin1.x = dispWin.x + _PT_Display_GetPanelHStart();
                stDispWin1.y = dispWin.y + _PT_Display_GetPanelVStart();
                stDispWin1.width = dispWin.width;
                stDispWin1.height = dispWin.height;

                if(stDispWin1.width % 8 != 0)
                {
                    //[patch]
                    //to avoid right green garbage line, set preview window width - 1 if width not align to 8 pixel.
                    //Only DS need this setting because of OPM offset of DS is the same with display window align 8 pixel.
                    //OPM of non-DS is the same with panel size (Do not need to align)
                    stDispWin1.width = stDispWin1.width - 1;
                }

                msAPI_XC_SetDispWinToReg_EX(&DEVICE_ID_0, &stDispWin1, E_MSAPI_XC_MAIN_WINDOW);
            }
            pDisplayInstance->bFirstTimeSetWindow = FALSE;
#if (MMSDK_DISPLAY_DUALXC_PORTING == 1)
        }
#endif
        PT_DISP_DBG_DS("---crop before recaluate x=%d, y=%d, w=%d, h=%d", cropWin.x, cropWin.y,
        cropWin.width, cropWin.height);
        //cropWin.width = (MMSDK_U16)DYNAMIC_SCALING_VB_H * cropWin.width / u32Width;
        //because of panel crop, cropWin.width must be DYNAMIC_SCALING_VB_H always
        //cropWin.width = (MMSDK_U16)DYNAMIC_SCALING_VB_H;
        //cropWin.x = (MMSDK_U16)DYNAMIC_SCALING_VB_H * cropWin.x / u32Width;

        PT_DISP_DBG_DS("----[DBG] (V, crop_h, h, crop_b) (%d,%d,%d,%d)", DYNAMIC_SCALING_VB_V, (int)cropWin.height, (int)u32Height, (int)u32CropBottom);

        //because of panel crop, cropWin.height must be DYNAMIC_SCALING_VB_V always
        //cropWin.height = (MMSDK_U16)DYNAMIC_SCALING_VB_V * cropWin.height / (u32Height - u32CropBottom);
        //cropWin.height = (MMSDK_U16)DYNAMIC_SCALING_VB_V;
        //cropWin.y = (MMSDK_U16)DYNAMIC_SCALING_VB_V * cropWin.y / (u32Height - u32CropBottom);

        PT_DISP_DBG_DS("---crop after reclauate x=%d, y=%d, w=%d, h=%d", cropWin.x, cropWin.y, cropWin.width, cropWin.height);
        PT_DISP_DBG_DS("---decoded, w=%d, h=%d", (int)u32Width, (int)u32Height);

        // if screen mute disabled, change zoom info only
        PT_DISP_DBG_DS("DS_SendZoomInfo2Firmware %"DTC_MS_U32_x"", u32DSAddr);

        // get scaler information
#if (MMSDK_DISPLAY_DUALXC_PORTING == 1)
        XC_EX_ApiStatus stXCStatus;
        MApi_XC_EX_GetStatus((XC_DEVICE_ID*)&g_stXC_PT_DeviceId[0], &stXCStatus, MAIN_WINDOW);
#else
        XC_ApiStatus stXCStatus;
        MApi_XC_GetStatus(&stXCStatus, MAIN_WINDOW);
#endif

        // direct write to shared memory with firmware
        ST_MM_DS_XC_STATUS *pstMMDS_Status = (ST_MM_DS_XC_STATUS*) MsOS_PA2KSEG1(u32DSAddr);

        // copy scaler related information
        pstMMDS_Status->bFWGotNewSetting = FALSE;
        pstMMDS_Status->bHKIsUpdating = TRUE;
        MsOS_FlushMemory();

        pstMMDS_Status->stNewCropWin.x      = cropWin.x;
        pstMMDS_Status->stNewCropWin.y      = cropWin.y;
        pstMMDS_Status->stNewCropWin.width  = cropWin.width;
        pstMMDS_Status->stNewCropWin.height = cropWin.height;

        pstMMDS_Status->stNewDispWin.x      = dispWin.x + _PT_Display_GetPanelHStart();
        pstMMDS_Status->stNewDispWin.y      = dispWin.y + _PT_Display_GetPanelVStart();

        pstMMDS_Status->stNewDispWin.width  = dispWin.width;
        pstMMDS_Status->stNewDispWin.height = dispWin.height;

        //DS must set line offset for crop pixel offset. If don't set this, it will always be 1920.
        pstMMDS_Status->u16IPMOffset = stXCStatus.u16IPMOffset;

        if(MApi_XC_IsCurrentFrameBufferLessMode() || msAPI_XC_GetRFBLMode())
        {
            PT_DISP_DBG_DS("MMSDK porting DS_FBL");
            pstMMDS_Status->u8MVOPCrop = 1;
        }
        else
        {
            PT_DISP_DBG_DS("MMSDK porting DS_FB");
            pstMMDS_Status->u8MVOPCrop = 0;
        }
        pstMMDS_Status->u8EnhanceModeSetting = 1 << 5;
        pstMMDS_Status->bHKIsUpdating = FALSE;
        MsOS_FlushMemory();

        CALLBACK(E_MMSDK_DISPLAY_CALLBACK_UPDATE_ZOOM_INFO, NULL, NULL);
    }
    else
    {
        if( enMediaType == E_MMSDK_MEDIA_TYPE_PHOTO || enMediaType == E_MMSDK_MEDIA_TYPE_MOVIE)
        {
            MS_BOOL bDS = FALSE;

            PT_DISP_DBG("Set DS off before set window\n");
            PT_Display_SetDSOnOFF(DisplayItem, bDS);
            msAPI_XC_SetCommand(NULL, E_MSAPI_XC_CMD_SET_DYNAMIC_SCALING, &bDS);
            msAPI_XC_SetAspectRatioType_EX(&DEVICE_ID_0,E_MSAPI_XC_MAIN_WINDOW,E_MSAPI_XC_ASPECT_RATIO_NORMAL);
            msAPI_XC_SetWin_EX(&DEVICE_ID_0, NULL, &cropWin, &dispWin, E_MSAPI_XC_MAIN_WINDOW);
#if (MMSDK_DISPLAY_DUALXC_PORTING == 1)
            msAPI_XC_SetWin_EX(&DEVICE_ID_1, NULL, NULL, NULL, E_MSAPI_XC_MAIN_WINDOW);
            //[K3_Porting] msAPI_VE_SetMode();
#else
            msAPI_VE_SetMode();
#endif
        }
    }
    //[River] - RFBL will check in msAPI_XC_SetWin
    //MDrv_PQ_SetRFblMode(TRUE, FALSE);
    if(PT_SYS_GetMMPhotoPath()== 1 && enMediaType == E_MMSDK_MEDIA_TYPE_PHOTO)
    {
        //MVOP control for interlace JPEG, select STB_DC output field
        MDrv_MVOP_SEL_OP_FIELD(MVOP_OUTPUT_FIELD);

        // wait 4 input vsync before turn on scalar
#if (MMSDK_DISPLAY_DUALXC_PORTING == 1)
        MApi_XC_EX_WaitInputVSync((XC_DEVICE_ID*)&g_stXC_PT_DeviceId[0], 4, 200, E_XC_EX_MAIN_WINDOW);
#else
        MApi_XC_WaitInputVSync(4, 200, MAIN_WINDOW);
#endif
        //MVOP control for interlace JPEG, select STB_DC source field
        MDrv_MVOP_SEL_OP_FIELD(MVOP_SOURCE_FIELD);
    }

    //Update DS XC info
    if ((pDisplayInstance->bIsUpDSInfo == FALSE) && enMediaType == E_MMSDK_MEDIA_TYPE_MOVIE
            && (CALLBACK(E_MMSDK_DISPLAY_CALLBACK_IS_ENABLE_DYNAMIC_SCALING, NULL, NULL))
            && (pDisplayInstance->bIsAvpFlow == FALSE) )
    {
        MMSDK_U32 u32DynScalSize = pDisplayInstance->stDisplayInfo.u32DSSize;

        _DS_SendXCStatus2Firmware((pDisplayInstance->stDisplayInfo.u32DSInfoBaseAddr + u32DynScalSize),
            &cropWin,&dispWin,MAIN_WINDOW);
        CALLBACK(E_MMSDK_DISPLAY_CALLBACK_UPDATE_XC_INFO, NULL, NULL);
        pDisplayInstance->bIsUpDSInfo = TRUE;
    }

    //msAPI_XC_SetDebugLevel(E_MSAPI_XC_DBG_LEVEL_NONE);
    if (u32MuteTimeMS > 0)
    {
        PT_Display_BlackScreen(DisplayItem, FALSE);
    }

#ifdef HDR_MM_DEV
    _PT_Display_EnableFrameBufferLess();
#endif
    return TRUE;
}

MMSDK_BOOL PT_Display_SetMode(PT_DISPLAYITEM DisplayItem, ST_MMSDK_SET_DISPLAY_INFO *pDisplayInfo)
{
    PT_DISP_DBG("");
    if ((DisplayItem == NULL) || (pDisplayInfo == NULL))
    {
        PT_DISP_ERR("DisplayItem or pDisplayInfo is NULL\n");
        return FALSE;
    }
    _ST_MMSDK_DISPLAY_INSTANCE* pDisplayInstance = (_ST_MMSDK_DISPLAY_INSTANCE*)DisplayItem;
    pDisplayInstance->bFirstTimeSetWindow = TRUE;
    //msAPI_XC_SetDebugLevel(E_MSAPI_XC_DBG_LEVEL_API);
    memcpy(&(pDisplayInstance->stDisplayInfo), pDisplayInfo, sizeof(ST_MMSDK_SET_DISPLAY_INFO));
    EN_MMSDK_MEDIA_TYPE enMediaType = pDisplayInstance->stDisplayInfo.enMediaType;
    if (NULL != gstSysCusFunPtr.pSetMode)
    {
        MMSDK_BOOL bRet = FALSE;
        MMSDK_BOOL bEnableDS;
        MMSDK_U16 u16MediaType;

        u16MediaType   = enMediaType;
        bEnableDS      = pDisplayInfo->bEnableDynScaling;

        bRet = gstSysCusFunPtr.pSetMode(u16MediaType, bEnableDS);
        pDisplayInstance->bIsUpDSInfo = FALSE;
        return bRet;
    }
#ifdef HDR_MM_DEV
    _PT_Display_CFDInit();
#endif

    pDisplayInstance->bIsUpDSInfo = FALSE;

    //If decode JPEG to XC
    if (enMediaType == E_MMSDK_MEDIA_TYPE_PHOTO && PT_SYS_GetMMPhotoPath() == 1)
    {
        PT_DISP_DBG("Show Photo by MVOP!!");
#if (MMSDK_DISPLAY_DUALXC_PORTING == 1)
        PT_DISP_DBG("bMIU0 = %d", pDisplayInfo->bMIU0);

        MSAPI_XC_VDEC_DispInfo stVidStatus = {0};
        memset(&stVidStatus, 0, sizeof(MSAPI_XC_VDEC_DispInfo));
        MSAPI_XC_JPEG_INFO stJpegInfo = {0};
        PNL_DeviceId stPNL_DeviceId = {g_stXC_PT_DeviceId[0].u32Version, g_stXC_PT_DeviceId[0].u32Id};

        if (g_IPanelEx.DefaultVFreq(&stPNL_DeviceId) == 600)
        {
            stVidStatus.u32FrameRate  = 30000;
        }
        else
        {
            stVidStatus.u32FrameRate  = 25000;
        }

        if (pDisplayInfo->u8Interlace == FALSE)
        {
            stVidStatus.u8Interlace   = 0;
            PT_DISP_DBG("JPEG is progressive!\n");
        }
        else
        {
            stVidStatus.u8Interlace   = 1;
            PT_DISP_DBG("JPEG is NOT progressive!\n");
        }
        MMSDK_U16 u16ImageWidth = pDisplayInfo->u16SrcWidth;
        MMSDK_U16 u16ImageHeight = pDisplayInfo->u16SrcHeight;
        PT_DISP_DBG("%u,%u", u16ImageWidth, u16ImageHeight);
        stVidStatus.u16HorSize      = (u16ImageWidth + 7) & 0xfff8;
        stVidStatus.u16VerSize      = u16ImageHeight;
        stVidStatus.u8AspectRate    = 2;
        stVidStatus.u16SarWidth     = 1;
        stVidStatus.u16SarHeight    = 1;
        stVidStatus.u16CropRight    = 0;
        stVidStatus.u16CropLeft     = 0;
        stVidStatus.u16CropBottom   = 0;
        stVidStatus.u16CropTop      = 0;
        stVidStatus.u32AspectWidth  = (u16ImageWidth + 7) & 0xfff8;
        stVidStatus.u32AspectHeight = u16ImageHeight;

        msAPI_XC_SetVDECInfo_EX_ByPath(E_MSAPI_XC_MAIN_DECODER_PATH, stVidStatus);
        //set JPEG still image config
        stJpegInfo.u8MiuSel = (pDisplayInfo->bMIU0) ? 0 : 1;
        stJpegInfo.u32BufAddr = pDisplayInfo->u32DisplayBuffPhysicalAddr;
        PT_DISP_DBG("JPD output fb addr = 0x%"DTC_MS_U32_X", MIU = %d", stJpegInfo.u32BufAddr, stJpegInfo.u8MiuSel);
        msAPI_XC_SetJPEGInfo(stJpegInfo);

        //set MVOP config
        msAPI_XC_ForceMVOPDramType_EX(E_MVOP_DEV_0, E_MSAPI_XC_MVOP_SOURCE_TYPE_422);
        msAPI_XC_SetMVOPMiuSel_EX(E_MSAPI_XC_MAIN_DECODER_PATH, TRUE, (pDisplayInfo->bMIU0)? 0 : 1);
        CHECK_MAPI_XC_RETURN_VALUE(msAPI_XC_SetMVOPConfig_EX(E_MSAPI_XC_MAIN_DECODER_PATH, MVOP_INPUT_DRAM));

        msAPI_XC_SetConnect_EX(&g_stXC_PT_DeviceId[0], E_XC_EX_INPUT_SOURCE_DTV, E_MSAPI_XC_MAIN_WINDOW);

        //for SD out [temp], use AA mode
        msAPI_XC_SetConnect_EX(&g_stXC_PT_DeviceId[1], E_XC_EX_INPUT_SOURCE_SCALER_OP, E_MSAPI_XC_MAIN_WINDOW);
        msAPI_XC_SetWin_EX(&g_stXC_PT_DeviceId[1], NULL, NULL, NULL, E_XC_EX_MAIN_WINDOW);
        msAPI_XC_SetVideoMuteByMode_EX(&g_stXC_PT_DeviceId[1], FALSE, E_XC_EX_MAIN_WINDOW, E_MSAPI_XC_VIDEO_MUTE_MODE_AUTO_SYNC);
        //Because of VE sett output timing will enable ve black screen,
        //disable black screen here
        msAPI_VE_SetVideoMute(DISABLE);
#else
        PT_DISP_DBG("bMIU0 = %d", pDisplayInfo->bMIU0);

        MSAPI_XC_VDEC_DispInfo stVidStatus = {0};
        memset(&stVidStatus, 0, sizeof(MSAPI_XC_VDEC_DispInfo));
        MSAPI_XC_JPEG_INFO stJpegInfo = {0};
        if (g_IPanel.DefaultVFreq() == 600)
        {
            stVidStatus.u32FrameRate  = 30000;
        }
        else
        {
            stVidStatus.u32FrameRate  = 25000;
        }

        if (pDisplayInfo->u8Interlace == FALSE)
        {
            stVidStatus.u8Interlace   = 0;
            PT_DISP_DBG("JPEG is progressive!\n");
        }
        else
        {
            stVidStatus.u8Interlace   = 1;
            PT_DISP_DBG("JPEG is NOT progressive!\n");
        }
        MMSDK_U16 u16ImageWidth = pDisplayInfo->u16SrcWidth;
        MMSDK_U16 u16ImageHeight = pDisplayInfo->u16SrcHeight;
        PT_DISP_DBG("%u,%u", u16ImageWidth, u16ImageHeight);
        stVidStatus.u16HorSize      = (u16ImageWidth + 7) & 0xfff8;
        stVidStatus.u16VerSize      = u16ImageHeight;
        stVidStatus.u8AspectRate    = 2;
        stVidStatus.u16SarWidth     = 1;
        stVidStatus.u16SarHeight    = 1;
        stVidStatus.u16CropRight    = 0;
        stVidStatus.u16CropLeft     = 0;
        stVidStatus.u16CropBottom   = 0;
        stVidStatus.u16CropTop      = 0;
        stVidStatus.u32AspectWidth  = (u16ImageWidth + 7) & 0xfff8;
        stVidStatus.u32AspectHeight = u16ImageHeight;

        msAPI_XC_SetVDECInfo_EX_ByPath(E_MSAPI_XC_MAIN_DECODER_PATH, stVidStatus);
        //set JPEG still image config
        stJpegInfo.u8MiuSel = (pDisplayInfo->bMIU0) ? 0 : 1;
        stJpegInfo.u32BufAddr = pDisplayInfo->u32DisplayBuffPhysicalAddr;
        PT_DISP_DBG("JPD output fb addr = 0x%lX, MIU = %d", stJpegInfo.u32BufAddr, stJpegInfo.u8MiuSel);
        msAPI_XC_SetJPEGInfo(stJpegInfo);

        //set MVOP config
        msAPI_XC_ForceMVOPDramType_EX(E_MVOP_DEV_0, E_MSAPI_XC_MVOP_SOURCE_TYPE_422);
        msAPI_XC_SetMVOPMiuSel_EX(E_MSAPI_XC_MAIN_DECODER_PATH, TRUE, (pDisplayInfo->bMIU0)? 0 : 1);
        CHECK_MAPI_XC_RETURN_VALUE(msAPI_XC_SetMVOPConfig_EX(E_MSAPI_XC_MAIN_DECODER_PATH, MVOP_INPUT_DRAM));
        msAPI_XC_SetConnect_EX(&g_stXC_PT_DeviceId, INPUT_SOURCE_DTV, E_MSAPI_XC_MAIN_WINDOW);
        //Because of VE sett output timing will enable ve black screen,
        //disable black screen here
        msAPI_VE_SetVideoMute(DISABLE);
#endif
    }
    else if(enMediaType == E_MMSDK_MEDIA_TYPE_MOVIE)
    {
#if (MMSDK_DISPLAY_DUALXC_PORTING == 1)
        PT_DISP_DBG("Video Type: %d.\n", pDisplayInfo->eVideoType);
        DS_VBOX_INFO pstDSVBox =  {.u16VBox_Htotal = DYNAMIC_SCALING_VB_H, .u16VBox_Vtotal = DYNAMIC_SCALING_VB_V};
        MVOP_InputSel tMvopInputSel = MVOP_INPUT_UNKNOWN;
        MS_U32 u32CodecType;
        MSAPI_XC_VDEC_DispInfo stVidStatus = {0};
        memset(&stVidStatus, 0, sizeof(MSAPI_XC_VDEC_DispInfo));

        //Fill Video information for msAPI_XC lib
        stVidStatus.u16CropLeft = pDisplayInfo->u16CropLeft;
        stVidStatus.u16CropRight = pDisplayInfo->u16CropRight;
        stVidStatus.u16CropTop = pDisplayInfo->u16CropTop;
        stVidStatus.u16CropBottom = pDisplayInfo->u16CropBottom;
        stVidStatus.u16SarWidth   = 0;
        stVidStatus.u16SarHeight = 0;
        stVidStatus.u16HorSize    = pDisplayInfo->u16SrcWidth;
        stVidStatus.u16VerSize    = pDisplayInfo->u16SrcHeight;

        if(pDisplayInfo->bEnableDynScaling)
        {
            if (TRUE != _DS_Get_VirtualBox_Info(&pstDSVBox))
            {
                // for backward capability
                PT_DISP_DBG("Warning!!! _DS_Get_VirtualBox_Info return fail, take FHD as VBox size");
            }

            stVidStatus.u16HorSize  = pstDSVBox.u16VBox_Htotal;
            stVidStatus.u16VerSize = pstDSVBox.u16VBox_Vtotal;
        }

#if (MM_3D_ENABLE == 1)
// [Calvin] To set XC to show two frames
        if (pDisplayInstance->stDisplayInfo.en3DLayout != E_MMSDK_3DLAYOUT_NOT_3D)
        {
            pDisplayInstance->bIsMM3D = TRUE;
            if ((pDisplayInstance->stDisplayInfo.en3DLayout == E_MMSDK_3DLAYOUT_TOP_BOTTOM_RT) ||
                (pDisplayInstance->stDisplayInfo.en3DLayout == E_MMSDK_3DLAYOUT_TOP_BOTTOM_LT))
            {
                if(stVidStatus.u16VerSize<(stVidStatus.u16CropBottom)*2)
                {
                    stVidStatus.u16CropBottom = stVidStatus.u16VerSize - ((stVidStatus.u16VerSize-stVidStatus.u16CropBottom)*2);
                }
            }
            else if((pDisplayInstance->stDisplayInfo.en3DLayout == E_MMSDK_3DLAYOUT_SIDE_BY_SIDE_LF) ||
                    (pDisplayInstance->stDisplayInfo.en3DLayout == E_MMSDK_3DLAYOUT_SIDE_BY_SIDE_RF))
            {
                stVidStatus.u16CropLeft = 0;
                stVidStatus.u16CropRight = 0;
            }
        }
#endif

        stVidStatus.u32FrameRate  = pDisplayInfo->u16FrameRate;
        stVidStatus.u8Interlace   = pDisplayInfo->u8Interlace;

        /////////////////////////
        //get video information//
        /////////////////////////
        msAPI_XC_SetVDECInfo_EX_ByPath(E_MSAPI_XC_MAIN_DECODER_PATH, stVidStatus);

        u32CodecType = pDisplayInfo->eVideoType;
        if((u32CodecType == E_MMSDK_VIDEO_CODEC_MPEG4) ||
                (u32CodecType == E_MMSDK_VIDEO_CODEC_VC1) ||
                (u32CodecType == E_MMSDK_VIDEO_CODEC_FLV))
            tMvopInputSel = MVOP_INPUT_MVD;
        else if((u32CodecType == E_MMSDK_VIDEO_CODEC_H264) ||
                (u32CodecType == E_MMSDK_VIDEO_CODEC_VP8) ||
                (u32CodecType == E_MMSDK_VIDEO_CODEC_AVS))
            tMvopInputSel = MVOP_INPUT_H264;
        else if(u32CodecType == E_MMSDK_VIDEO_CODEC_MJPEG)
            tMvopInputSel = MVOP_INPUT_JPD;
        else if(u32CodecType == E_MMSDK_VIDEO_CODEC_HEVC)
            tMvopInputSel = MVOP_INPUT_EVD;
        else if(u32CodecType == E_MMSDK_VIDEO_CODEC_VP9)
            tMvopInputSel = MVOP_INPUT_VP9;
        else if(u32CodecType == E_MMSDK_VIDEO_CODEC_RM)
            tMvopInputSel = MVOP_INPUT_RVD;
        else if(u32CodecType == E_MMSDK_VIDEO_CODEC_MVC)
            tMvopInputSel = MVOP_INPUT_HVD_3DLR;
        else //MPEG2
            tMvopInputSel = MVOP_INPUT_MVD;

        msAPI_XC_SetMVOPMiuSel_EX(E_MSAPI_XC_MAIN_DECODER_PATH, TRUE, (pDisplayInfo->bMIU0)? 0 : 1);
        CHECK_MAPI_XC_RETURN_VALUE(msAPI_XC_SetMVOPConfig_EX(E_MSAPI_XC_MAIN_DECODER_PATH, tMvopInputSel));

        msAPI_XC_SetConnect_EX(&g_stXC_PT_DeviceId[0], E_XC_EX_INPUT_SOURCE_DTV, E_MSAPI_XC_MAIN_WINDOW);

        //for SD out [temp], use AA mode
        msAPI_XC_SetConnect_EX(&g_stXC_PT_DeviceId[1], E_XC_EX_INPUT_SOURCE_SCALER_OP, E_MSAPI_XC_MAIN_WINDOW);
        MApi_XC_SetOSD2VEMode(E_VOP_SEL_OSD_LAST);
        msAPI_XC_SetWin_EX(&g_stXC_PT_DeviceId[1], NULL, NULL, NULL, E_XC_EX_MAIN_WINDOW);
        msAPI_XC_SetVideoMuteByMode_EX(&g_stXC_PT_DeviceId[1], FALSE, E_XC_EX_MAIN_WINDOW, E_MSAPI_XC_VIDEO_MUTE_MODE_AUTO_SYNC);
        MApi_XC_EX_SetOutputCapture((XC_DEVICE_ID*)&g_stXC_PT_DeviceId[0],TRUE,E_XC_EX_OP2);
        //Because of VE sett output timing will enable ve black screen,
        //disable black screen here
        msAPI_VE_SetVideoMute(DISABLE);
#else
        PT_DISP_DBG("_DS_Get_VirtualBox_Info");
        DS_VBOX_INFO pstDSVBox =  {.u16VBox_Htotal = DYNAMIC_SCALING_VB_H, .u16VBox_Vtotal = DYNAMIC_SCALING_VB_V};
        MS_U16 u16SrcWidth = pDisplayInfo->u16SrcWidth;
        MS_U16 u16SrcHeight = pDisplayInfo->u16SrcHeight;
        if(pDisplayInfo->bEnableDynScaling)
        {
            if (TRUE != _DS_Get_VirtualBox_Info(&pstDSVBox))
            {
                // for backward capability
                PT_DISP_DBG("Warning!!! _DS_Get_VirtualBox_Info return fail, take FHD as VBox size");
            }

            PT_DISP_DBG("vdec_debug VBox Height   %d", pstDSVBox.u16VBox_Vtotal);
            PT_DISP_DBG("vdec_debug VBox Width    %d", pstDSVBox.u16VBox_Htotal);
            u16SrcWidth  = pstDSVBox.u16VBox_Htotal;
            u16SrcHeight = pstDSVBox.u16VBox_Vtotal;
        }

        PT_DISP_DBG("Video Type: %d.\n", pDisplayInfo->eVideoType);

        MVOP_InputSel tMvopInputSel;
        MS_U32 u32CodecType;
        MSAPI_XC_VDEC_DispInfo stVidStatus;
        //MS504x set MVOP as MIU1 Group1

        MDrv_MVOP_Init();
        PT_DISP_DBG("pDisplayInfo->bMIU0=%d", pDisplayInfo->bMIU0);
        MDrv_MVOP_MiuSwitch((pDisplayInfo->bMIU0)? 0 : 1);
        MDrv_MVOP_Enable(FALSE); //wait setting done
        u32CodecType = pDisplayInfo->eVideoType;
        //u32CodecType = msAPI_MM_GetOption((int)MM_OPTION_MOVIE_INFO, (int)MM_INFO_VIDEO_CODEC);
        if((u32CodecType == E_MMSDK_VIDEO_CODEC_MPEG4) ||
                (u32CodecType == E_MMSDK_VIDEO_CODEC_VC1) ||
                (u32CodecType == E_MMSDK_VIDEO_CODEC_FLV))
            tMvopInputSel = MVOP_INPUT_MVD;
        else if((u32CodecType == E_MMSDK_VIDEO_CODEC_H264) ||
                (u32CodecType == E_MMSDK_VIDEO_CODEC_RM) ||
                (u32CodecType == E_MMSDK_VIDEO_CODEC_VP8) ||
                (u32CodecType == E_MMSDK_VIDEO_CODEC_AVS))
            tMvopInputSel = MVOP_INPUT_H264;
        else if(u32CodecType == E_MMSDK_VIDEO_CODEC_MJPEG)
            tMvopInputSel = MVOP_INPUT_JPD;
        else if(u32CodecType == E_MMSDK_VIDEO_CODEC_HEVC)
            tMvopInputSel = MVOP_INPUT_EVD;
        else if(u32CodecType == E_MMSDK_VIDEO_CODEC_VP9)
            tMvopInputSel = MVOP_INPUT_VP9;
        else //MPEG2
            tMvopInputSel = MVOP_INPUT_MVD;
        MDrv_MVOP_SetInputCfg(tMvopInputSel, NULL);
        memset(&stVidStatus, 0, sizeof(MSAPI_XC_VDEC_DispInfo));

        //Fill Video information for msAPI_XC lib
        stVidStatus.u16CropLeft = pDisplayInfo->u16CropLeft;
        stVidStatus.u16CropRight = pDisplayInfo->u16CropRight;
        stVidStatus.u16CropTop = pDisplayInfo->u16CropTop;
        stVidStatus.u16CropBottom = pDisplayInfo->u16CropBottom;
        stVidStatus.u16SarWidth   = 0;
        stVidStatus.u16SarHeight = 0;
        stVidStatus.u16HorSize	  = u16SrcWidth;
        stVidStatus.u16VerSize	  = u16SrcHeight;
        stVidStatus.u32FrameRate  = pDisplayInfo->u16FrameRate;
        stVidStatus.u8Interlace   = pDisplayInfo->u8Interlace;

#if (MM_3D_ENABLE == 1)
// [Calvin] To set XC to show two frames
        if (pDisplayInstance->stDisplayInfo.en3DLayout != E_MMSDK_3DLAYOUT_NOT_3D)
        {
            pDisplayInstance->bIsMM3D = TRUE;
            if ((pDisplayInstance->stDisplayInfo.en3DLayout == E_MMSDK_3DLAYOUT_TOP_BOTTOM_RT) ||
                (pDisplayInstance->stDisplayInfo.en3DLayout == E_MMSDK_3DLAYOUT_TOP_BOTTOM_LT))
            {
                stVidStatus.u16CropBottom = stVidStatus.u16VerSize - ((stVidStatus.u16VerSize-stVidStatus.u16CropBottom)*2);
            }
            else if((pDisplayInstance->stDisplayInfo.en3DLayout == E_MMSDK_3DLAYOUT_SIDE_BY_SIDE_LF) ||
                    (pDisplayInstance->stDisplayInfo.en3DLayout == E_MMSDK_3DLAYOUT_SIDE_BY_SIDE_RF))
            {
                stVidStatus.u16CropLeft = 0;
                stVidStatus.u16CropRight = 0;
            }
        }
#endif

        /////////////////////////
        //get video information//
        /////////////////////////
        msAPI_XC_SetVDECInfo_EX_ByPath(E_MSAPI_XC_MAIN_DECODER_PATH, stVidStatus);

        PT_DISP_DBG("MM_DispInfo dWidth %d, dHeight %d, dFrameRate %d dInterlace %d",
                 pDisplayInfo->u16SrcWidth, pDisplayInfo->u16SrcHeight, pDisplayInfo->u16FrameRate, pDisplayInfo->u8Interlace);
        PT_DISP_DBG("MM_DispInfo dCropLeft %d, dCropRight %d, dCropTop %d, dCropBottom %d",
                 pDisplayInfo->u16CropLeft, pDisplayInfo->u16CropRight, pDisplayInfo->u16CropTop, pDisplayInfo->u16CropBottom);

        CHECK_MAPI_XC_RETURN_VALUE(msAPI_XC_SetMVOPConfig_EX(E_MSAPI_XC_MAIN_DECODER_PATH, tMvopInputSel));
        msAPI_XC_SetConnect_EX(&g_stXC_PT_DeviceId, INPUT_SOURCE_DTV, E_MSAPI_XC_MAIN_WINDOW);
        //Because of VE sett output timing will enable ve black screen,
        //disable black screen here
        msAPI_VE_SetVideoMute(DISABLE);

        MS_PQ_MuliMedia_Info stPQMMInfo;
        stPQMMInfo.eType = E_PQ_MULTIMEDIA_MOVIE;
        MDrv_PQ_Set_MultiMediaInfo(E_MSAPI_XC_MAIN_WINDOW, &stPQMMInfo);
#endif
    }
    else
    {
        PT_DISP_DBG("Show Photo by GOP!!");
#if (MMSDK_DISPLAY_DUALXC_PORTING == 1)
        //for SD out [temp], use AA mode
        msAPI_XC_SetConnect_EX(&g_stXC_PT_DeviceId[1], E_XC_EX_INPUT_SOURCE_SCALER_OP, E_MSAPI_XC_MAIN_WINDOW);
        MApi_XC_SetOSD2VEMode(E_VOP_SEL_OSD_LAST);
        msAPI_XC_SetWin_EX(&g_stXC_PT_DeviceId[1], NULL, NULL, NULL, E_XC_EX_MAIN_WINDOW);
        msAPI_XC_SetVideoMuteByMode_EX(&g_stXC_PT_DeviceId[1], FALSE, E_XC_EX_MAIN_WINDOW, E_MSAPI_XC_VIDEO_MUTE_MODE_AUTO_SYNC);
#endif
    }
    //[River] - Need set frame buffer again?
    //PT_DISP_DBG("Scaler start: 0x%lx, size: 0x%lx.\n", pDisplayInfo->u32ScalerBuffStart, pDisplayInfo->u32ScalerBuffSize);
    //_SetFrameBufferSize(pDisplayInfo->u32ScalerBuffStart, pDisplayInfo->u32ScalerBuffSize);

    //msAPI_XC_SetDebugLevel(E_MSAPI_XC_DBG_LEVEL_NONE);
    return TRUE;
}

MMSDK_U16 PT_Display_GetMaxFrameNumInMem(PT_DISPLAYITEM DisplayItem)
{
#if (MMSDK_DISPLAY_DUALXC_PORTING == 1)
    const XC_EX_ApiInfo *pXC_ApiInfo = MApi_XC_EX_GetInfo((XC_DEVICE_ID*)&g_stXC_PT_DeviceId[0]);
#else
    const XC_ApiInfo *pXC_ApiInfo = MApi_XC_GetInfo();
#endif
    if(pXC_ApiInfo == NULL)
    {
        return FALSE;
    }
    PT_DISP_DBG("%u", pXC_ApiInfo->u8MaxFrameNumInMem);
    return pXC_ApiInfo->u8MaxFrameNumInMem;
}
MMSDK_U16 PT_Display_GetMaxFieldNumInMem(PT_DISPLAYITEM DisplayItem)
{
#if (MMSDK_DISPLAY_DUALXC_PORTING == 1)
    const XC_EX_ApiInfo *pXC_ApiInfo = MApi_XC_EX_GetInfo((XC_DEVICE_ID*)&g_stXC_PT_DeviceId[0]);
#else
    const XC_ApiInfo *pXC_ApiInfo = MApi_XC_GetInfo();
#endif
    if(pXC_ApiInfo == NULL)
    {
        return FALSE;
    }
    PT_DISP_DBG("%u", pXC_ApiInfo->u8MaxFieldNumInMem);

    return pXC_ApiInfo->u8MaxFieldNumInMem;
}

MMSDK_U16 PT_Display_GetPanelWidth(PT_DISPLAYITEM DisplayItem)
{
#if (MMSDK_DISPLAY_DUALXC_PORTING == 1)
    PNL_DeviceId stPNL_DeviceId = {DEVICE_ID_0.u32Version, DEVICE_ID_0.u32Id};
    PT_DISP_DBG("%u", g_IPanelEx.Width(&stPNL_DeviceId));
    return g_IPanelEx.Width(&stPNL_DeviceId);
#else
    PT_DISP_DBG("%u", g_IPanel.Width());
    return g_IPanel.Width();
#endif
}

MMSDK_U16 PT_Display_GetPanelHeight(PT_DISPLAYITEM DisplayItem)
{
#if (MMSDK_DISPLAY_DUALXC_PORTING == 1)
    PNL_DeviceId stPNL_DeviceId = {g_stXC_PT_DeviceId[0].u32Version, g_stXC_PT_DeviceId[0].u32Id};
    PT_DISP_DBG("%u", g_IPanelEx.Height(&stPNL_DeviceId));
    return g_IPanelEx.Height(&stPNL_DeviceId);
#else
    PT_DISP_DBG("%u", g_IPanel.Height());
    return g_IPanel.Height();
#endif
}

void PT_Display_WaitVSync(PT_DISPLAYITEM DisplayItem, MMSDK_BOOL bIsInput, MMSDK_U8 u8NumVSyncs, MMSDK_U16 u16Timeout)
{
    PT_DISP_DBG("");
    if (bIsInput)
    {
        //[River] - how about subwin? Oringianl MM use class member variable
#if (MMSDK_DISPLAY_DUALXC_PORTING == 1)
        MApi_XC_EX_WaitInputVSync((XC_DEVICE_ID*)&g_stXC_PT_DeviceId[0], u8NumVSyncs, u16Timeout, E_XC_EX_MAIN_WINDOW);
#else
        MApi_XC_WaitInputVSync(u8NumVSyncs, u16Timeout, MAIN_WINDOW);
#endif
    }
    else
    {
        //[River] - how about subwin? Oringianl MM use class member variable
#if (MMSDK_DISPLAY_DUALXC_PORTING == 1)
        MApi_XC_EX_WaitOutputVSync((XC_DEVICE_ID*)&g_stXC_PT_DeviceId[0], u8NumVSyncs, u16Timeout, E_XC_EX_MAIN_WINDOW);
#else
        MApi_XC_WaitOutputVSync(u8NumVSyncs, u16Timeout, MAIN_WINDOW);
#endif
    }
}

void PT_Display_Reset(PT_DISPLAYITEM DisplayItem)
{
    PT_DISP_DBG("");

    //[River] in mmsdk_photoplayer function deletemediaitem call this.
    //STB set frame rate convert when in Y8M4 mode
    //For K1(If always Y8M4 mode), Only do set RFBL mode?
    //But msAPI set win will check RFBL, so don't reset it.
    //MDrv_PQ_SetRFblMode(TRUE, FALSE);
}

MMSDK_BOOL PT_Display_CaptureMVopOutput(PT_DISPLAYITEM DisplayItem, ST_MMSDK_VIDEO_FRAME_INFO* pstFrameInfo, const ST_MMSDK_RECT* pstCropRect, MMSDK_U32 u32BuffAddr, MMSDK_U32 u32BuffSize)
{
    PT_DISP_DBG("");
    return TRUE;
}

#if (MM_3D_ENABLE == 1)
static MMSDK_BOOL _PT_Display_SetPhoto3D(PT_DISPLAYITEM DisplayItem)
{
    PT_DISP_DBG("");
    if (DisplayItem == NULL)
    {
        PT_DISP_ERR("DisplayItem is NULL\n");
        return FALSE;
    }

    _ST_MMSDK_DISPLAY_INSTANCE* pDisplayInstance = (_ST_MMSDK_DISPLAY_INSTANCE*)DisplayItem;
    MS_U32 u32OutMod = E_MSAPI_XC_3D_OUTPUT_SIDE_BY_SIDE_HALF;
    HDMITX_VIDEO_3D_STRUCTURE eHDMITx3DMode = HDMITx_VIDEO_3D_Not_in_Use;

    if (TRUE == pDisplayInstance->bISSet3D)
    {
        PT_DISP_DBG("Skip mm set 3D\n");
        return TRUE;
    }

    _HDMITx_3D_Mapping(u32OutMod, &eHDMITx3DMode);
    msAPI_XC_HDMITx_Set3DMode(TRUE, eHDMITx3DMode);
    pDisplayInstance->bISSet3D = TRUE;

    return TRUE;
}

static MMSDK_BOOL _PT_Display_SetVideo3D(PT_DISPLAYITEM DisplayItem)
{
    PT_DISP_DBG("");
    if (DisplayItem == NULL)
    {
        PT_DISP_ERR("DisplayItem is NULL\n");
        return FALSE;
    }

    _ST_MMSDK_DISPLAY_INSTANCE* pDisplayInstance = (_ST_MMSDK_DISPLAY_INSTANCE*)DisplayItem;
    HDMITX_VIDEO_3D_STRUCTURE eHDMITx3DMode = HDMITx_VIDEO_3D_Not_in_Use;
    E_MSAPI_XC_3D_OUTPUT_MODE enCurrent3DType = E_MSAPI_XC_3D_OUTPUT_MODE_NONE;
    //E_MSAPI_XC_3D_INPUT_MODE enInput3DType = E_MSAPI_XC_3D_INPUT_MODE_NONE;

    if (CALLBACK(E_MMSDK_DISPLAY_CALLBACK_IS_ENABLE_DYNAMIC_SCALING, NULL, NULL))
    {
        PT_DISP_ERR("Disable 3D mode, becasue DS need to be set OFF!\n");
        return FALSE;
    }

    if (TRUE == pDisplayInstance->bISSet3D)
    {
        PT_DISP_DBG("Skip mm set 3D\n");
        return TRUE;
    }

    PT_DISP_DBG("3D enum from stream %d\n", pDisplayInstance->stDisplayInfo.en3DLayout);

    // Get 3D info from VDEC
    // Cover play the special stream received 3D info after first time set display
    if (pDisplayInstance->bIsMM3D)
    {
        //mvc have special crop window, take 1080p mvc as example: it's capture window's 1920*2205,
        //but crop window report by codec is 1920*1080, so we need special handle it
        if (pDisplayInstance->stDisplayInfo.eVideoType == E_MMSDK_VIDEO_CODEC_MVC)
        {
            switch (pDisplayInstance->stDisplayInfo.en3DLayout)
            {
                case E_MMSDK_3DLAYOUT_TOP_BOTTOM_RT:
                case E_MMSDK_3DLAYOUT_TOP_BOTTOM_LT:
                    enCurrent3DType = E_MSAPI_XC_3D_OUTPUT_TOP_BOTTOM;
                    //enInput3DType = E_MSAPI_XC_3D_INPUT_TOP_BOTTOM;
                    PT_DISP_DBG("Enable 3D with Top Bottom!\n");
                    break;
                case E_MMSDK_3DLAYOUT_FRAME_PACKING:
                    enCurrent3DType = E_MSAPI_XC_3D_OUTPUT_FRAME_PACKING;
                    //enInput3DType = E_MSAPI_XC_3D_INPUT_FRAME_PACKING;
                    PT_DISP_DBG("Enable 3D with Frame Packing!\n");
                    break;
                default:
                    PT_DISP_ERR("Unsupported 3D mode!\n");
                    return FALSE;
            }
        }
        else
        {
            switch (pDisplayInstance->stDisplayInfo.en3DLayout)
            {
                case E_MMSDK_3DLAYOUT_TOP_BOTTOM_RT:
                case E_MMSDK_3DLAYOUT_TOP_BOTTOM_LT:
                    enCurrent3DType = E_MSAPI_XC_3D_OUTPUT_TOP_BOTTOM;
                    //enInput3DType = E_MSAPI_XC_3D_INPUT_TOP_BOTTOM;
                    PT_DISP_DBG("Enable 3D with Top Bottom!\n");
                    break;
                case E_MMSDK_3DLAYOUT_SIDE_BY_SIDE_LF:
                case E_MMSDK_3DLAYOUT_SIDE_BY_SIDE_RF:
                    enCurrent3DType = E_MSAPI_XC_3D_OUTPUT_SIDE_BY_SIDE_HALF;
                    //enInput3DType = E_MSAPI_XC_3D_INPUT_SIDE_BY_SIDE_HALF;
                    PT_DISP_DBG("Enable 3D with SIDE BY SIDE!\n");
                    break;
                default:
                    PT_DISP_ERR("Unsupported 3D mode!\n");
                    return FALSE;
            }
        }
        if (enCurrent3DType != E_MSAPI_XC_3D_OUTPUT_MODE_NONE)
        {
            _HDMITx_3D_Mapping(enCurrent3DType, &eHDMITx3DMode);
            msAPI_XC_HDMITx_Set3DMode(TRUE, eHDMITx3DMode);
            pDisplayInstance->bISSet3D = TRUE;
        }
    }
    else
    {
        PT_DISP_ERR("Not 3D Video!\n");
        return FALSE;
    }

    return TRUE;
}
#endif

MMSDK_BOOL PT_Display_Set3DOnOff(PT_DISPLAYITEM DisplayItem, MMSDK_BOOL bEnable3D)
{
    PT_DISP_DBG("[ (bEnable3D=%d)]", bEnable3D);
    if (DisplayItem == NULL)
    {
         PT_DISP_ERR("DisplayItem is NULL\n");
         return FALSE;
    }

    if (bEnable3D == 1)
    {
#if (MM_3D_ENABLE == 1)
        _ST_MMSDK_DISPLAY_INSTANCE* pDisplayInstance = (_ST_MMSDK_DISPLAY_INSTANCE*)DisplayItem;

        if (pDisplayInstance->stDisplayInfo.enMediaType == E_MMSDK_MEDIA_TYPE_MOVIE)
        {
             return _PT_Display_SetVideo3D(DisplayItem);
        }
        else if (pDisplayInstance->stDisplayInfo.enMediaType == E_MMSDK_MEDIA_TYPE_PHOTO)
        {
             return _PT_Display_SetPhoto3D(DisplayItem);
        }

        PT_DISP_DBG("PT_Display_Set3DOnOff: unsupported operation\n");
#else
        PT_DISP_DBG("Attention: 3D feature is not enabled by this project!\n");
#endif
     }
     return TRUE;
}

MMSDK_U16 PT_Display_GetOutputFrameRate(PT_DISPLAYITEM DisplayItem)
{
    MMSDK_U16 FrameRate = 0;
    //PT_DISP_DBG("");
    E_MSAPI_XC_OUTPUT_TIMING_TYPE eTiming;

    msAPI_XC_GetOutputResolution_EX(&DEVICE_ID_0, &eTiming);

    //PT_DISP_DBG("E_MSAPI_XC_OUTPUT_TIMING_TYPE = %d\n", eTiming);
    switch (eTiming)
    {
        case E_MSAPI_XC_RES_720x576I_50Hz:
        case E_MSAPI_XC_RES_720x576P_50Hz:
        case E_MSAPI_XC_RES_1280x720P_50Hz:
        case E_MSAPI_XC_RES_1920x1080I_50Hz:
        case E_MSAPI_XC_RES_1920x1080P_50Hz:
        case E_MSAPI_XC_RES_1280x1470P_3DFP_50Hz:
        case E_MSAPI_XC_RES_3840x2160P_50Hz:
        case E_MSAPI_XC_RES_4096x2160P_50Hz:
            FrameRate = 50;
            break;
        case E_MSAPI_XC_RES_1920x1080P_24Hz:
        case E_MSAPI_XC_RES_1920x2205P_3DFP_24Hz:
        case E_MSAPI_XC_RES_3840x2160P_24Hz:
        case E_MSAPI_XC_RES_4096x2160P_24Hz:
            FrameRate = 24;
            break;
        case E_MSAPI_XC_RES_1920x1080P_30Hz:
        case E_MSAPI_XC_RES_3840x2160P_30Hz:
        case E_MSAPI_XC_RES_4096x2160P_30Hz:
            FrameRate = 30;
            break;
        case E_MSAPI_XC_RES_1920x1080P_25Hz:
        case E_MSAPI_XC_RES_3840x2160P_25Hz:
        case E_MSAPI_XC_RES_4096x2160P_25Hz:
            FrameRate = 25;
            break;
        case E_MSAPI_XC_RES_720x480I_60Hz:
        case E_MSAPI_XC_RES_720x480P_60Hz:
        case E_MSAPI_XC_RES_1280x720P_60Hz:
        case E_MSAPI_XC_RES_1920x1080I_60Hz:
        case E_MSAPI_XC_RES_1920x1080P_60Hz:
        case E_MSAPI_XC_RES_640x480P_60Hz:
        case E_MSAPI_XC_RES_1280x1470P_3DFP_60Hz:
        case E_MSAPI_XC_RES_3840x2160P_60Hz:
        case E_MSAPI_XC_RES_4096x2160P_60Hz:
            FrameRate = 60;
            break;
        default:
            //PT_DISP_DBG("warning!! no-match timing, use 60 hz\n");
            FrameRate = 60;
            break;
    }
    //PT_DISP_DBG("PT_Display_GetOutputFrameRate = %d\n", FrameRate);
    return FrameRate;
}

MMSDK_BOOL PT_Display_IsOutputInterlace(PT_DISPLAYITEM DisplayItem)
{
    PT_DISP_DBG("");
    MMSDK_BOOL IsOutputInterlace = 0;
    E_MSAPI_XC_OUTPUT_TIMING_TYPE eTiming;

    msAPI_XC_GetOutputResolution_EX(&DEVICE_ID_0,&eTiming);

    PT_DISP_DBG("E_MSAPI_XC_OUTPUT_TIMING_TYPE = %d\n", eTiming);
    switch (eTiming)
    {
        case E_MSAPI_XC_RES_720x480I_60Hz:
        case E_MSAPI_XC_RES_720x576I_50Hz:
        case E_MSAPI_XC_RES_1920x1080I_50Hz:
        case E_MSAPI_XC_RES_1920x1080I_60Hz:
            IsOutputInterlace = TRUE;
            break;
        default:
            IsOutputInterlace = FALSE;
            break;
    }
    PT_DISP_DBG("PT_Display_IsOutputInterlace = %d\n", IsOutputInterlace);
    return IsOutputInterlace;
}

MMSDK_BOOL PT_Display_SetHighDynamicRange(PT_DISPLAYITEM DisplayItem, ST_MMSDK_HDR_METADATA* pHDRMetaData)
{
#ifdef HDR_MM_DEV
    if (DisplayItem == NULL || pHDRMetaData == NULL)
    {
        PT_DISP_DBG("DisplayItem or pHDRMetaData is NULL\n");
        return FALSE;
    }
    _gu32HDRMetadataType = pHDRMetaData->u32HDRMetadataType;

    // Diff Type of HDR
    if (pHDRMetaData->u32HDRMetadataType & E_MMSDK_HDR_TYPE_CTL_FLAG_VUI)
    {
        MMSDK_U8 u8transferCharateristics = pHDRMetaData->stHdrMetadataMpegVUI.u8TransferCharacteristics;

        if ((u8transferCharateristics != DISP_HDR_SMPTE2084) && (u8transferCharateristics != DISP_HDR_HLG))
        {
            // If HDR type == 0x1, but transferCharateristics != 0x10 or 0x12 -> Not HDR stream
            PT_DISP_DBG("Fake HDR10 stream, u8transferCharateristics = %d\n", u8transferCharateristics);
            _gu32HDRMetadataType = E_MMSDK_HDR_TYPE_CTL_FLAG_NONE;
        }
    }
/*
    if (pHDRMetaData->u32HDRMetadataType & E_MMSDK_HDR_TYPE_CTL_FLAG_SEI_COLOR_VOLUME)
    {
    }

    if (pHDRMetaData->u32HDRMetadataType & E_MMSDK_HDR_TYPE_CTL_FLAG_SEI_KNEE_FUNCTION)
    {
    }

    if (pHDRMetaData->u32HDRMetadataType & E_MMSDK_HDR_TYPE_CTL_FLAG_SEI_TONE_MAPPING)
    {
    }
*/

    PT_DISP_DBG("HDR type = 0x%"DTC_MS_U32_X" ", _gu32HDRMetadataType);
#endif
    return TRUE;
}

#ifdef OPTEE_ENABLE
MMSDK_BOOL PT_Display_GetXCPipeId(PT_DISPLAYITEM DisplayItem, ST_MMSDK_PIPE_INFO * pstPipeInfo)
{
    PT_DISP_DBG("");
    if ((DisplayItem == NULL) || (pstPipeInfo == NULL))
    {
        PT_DISP_ERR("DisplayItem or pstPipeInfo is NULL\n");
        return FALSE;
    }

    _ST_MMSDK_DISPLAY_INSTANCE* pDisplayInstance = (_ST_MMSDK_DISPLAY_INSTANCE*)DisplayItem;
    XC_OPTEE_HANDLER stOpteeHandler;
    memset((void *)&stOpteeHandler, 0, sizeof(XC_OPTEE_HANDLER));

    stOpteeHandler.eWindow = pDisplayInstance->eScalerWin;

    MApi_XC_OPTEE_Control(E_XC_OPTEE_GET_PIPE_ID, &stOpteeHandler);
    pstPipeInfo->u32SvpPplID = stOpteeHandler.pipeID;
    PT_DISP_DBG("XC pipe ID = %d\n", pstPipeInfo->u32SvpPplID);

    return TRUE;
}

MMSDK_BOOL PT_Display_XC_PipeLock(PT_DISPLAYITEM DisplayItem, ST_MMSDK_PIPE_INFO * pstPipeInfo, MMSDK_BOOL bEnable)
{
    PT_DISP_DBG("Enable = %d", bEnable);
    if ((DisplayItem == NULL) || (pstPipeInfo == NULL))
    {
        PT_DISP_DBG("DisplayItem or pstPipeInfo is NULL\n");
        return FALSE;
    }

    _ST_MMSDK_DISPLAY_INSTANCE* pDisplayInstance = (_ST_MMSDK_DISPLAY_INSTANCE*)DisplayItem;
    XC_OPTEE_HANDLER stOpteeHandler;
    memset((void *)&stOpteeHandler, 0, sizeof(XC_OPTEE_HANDLER));

    stOpteeHandler.eWindow = pDisplayInstance->eScalerWin;
    stOpteeHandler.pipeID = pstPipeInfo->u32SvpPplID;
    stOpteeHandler.isEnable = bEnable;

    if (bEnable)
    {
        MApi_XC_OPTEE_Control(E_XC_OPTEE_ENABLE, &stOpteeHandler);
    }
    else
    {
        MApi_XC_OPTEE_Control(E_XC_OPTEE_DISABLE, &stOpteeHandler);
    }
    return TRUE;
}
#endif

#ifndef AVP_ENABLE
MMSDK_BOOL PT_Display_SetOneField(PT_DISPLAYITEM DisplayItem, MMSDK_BOOL bEnable)
{
    PT_DISP_DBG("bEnable=%d", bEnable);

    if (DisplayItem == NULL)
    {
        PT_DISP_ERR("DisplayItem is NULL\n");
        return FALSE;
    }

    _ST_MMSDK_DISPLAY_INSTANCE* pDisplayInstance = (_ST_MMSDK_DISPLAY_INSTANCE*)DisplayItem;

    if(!(pDisplayInstance->bIsFirstTimeSetOneField))
    {
        if(pDisplayInstance->bIsOneField == bEnable)
        {
            return TRUE;
        }
    }
    else
    {
        pDisplayInstance->bIsFirstTimeSetOneField = FALSE;
    }
    pDisplayInstance->bIsOneField = bEnable;

    msAPI_XC_EnableMADiForceMotion_EX(&DEVICE_ID_0, E_MSAPI_XC_MAIN_WINDOW, E_MSAPI_XC_MADI_MOTION_YC, bEnable);

    return TRUE;
}
#endif

#ifdef AVP_ENABLE
MMSDK_BOOL PT_Display_InitializeEx(PT_DISPLAYITEM* pDisplayItem, void* pWindow)
{
     PT_DISP_DBG("");
    if (pDisplayItem == NULL)
    {
        PT_DISP_DBG("pDisplayItem is NULL\n");
        return FALSE;
    }

    int i = 0;
    _ST_MMSDK_DISPLAY_INSTANCE* pDisplayInstance = NULL;
    pDisplayInstance = (_ST_MMSDK_DISPLAY_INSTANCE*)PT_MsOS_AllocateMemory(sizeof(_ST_MMSDK_DISPLAY_INSTANCE));
    _PT_Display_ClearArguments(pDisplayInstance);
    if(NULL != gstSysCusFunPtr.pInitialize)
    {
        PT_SYS_CusInitInfo stCusInitInfo;
        memset(&stCusInitInfo, 0x00, sizeof(stCusInitInfo));
        stCusInitInfo.bAvpEnable = TRUE;
        stCusInitInfo.pDispItem = (void *)pDisplayInstance;
        stCusInitInfo.u32SizeofStructure = sizeof(stCusInitInfo);
        stCusInitInfo.u32SizeofDispItem = sizeof(_ST_MMSDK_DISPLAY_INSTANCE);
        gstSysCusFunPtr.pInitialize(&stCusInitInfo);
        *pDisplayItem  = (void*)pDisplayInstance;
        return TRUE;
    }

   //Get Free Instance ID
   for(i=0; i<sizeof(u8Display_Instance_BitMask)*8; i++)
   {
       if(!((u8Display_Instance_BitMask>>i) & BIT(0)))
       {
           pDisplayInstance ->u8ID_BitMask = BIT(i);
           u8Display_Instance_BitMask |= BIT(i);
           break;
       }
   }
   PT_DISP_DBG("[%s][%d]====pDisplayInstance ->u8ID_BitMask=%d===\n",__FUNCTION__,__LINE__,pDisplayInstance ->u8ID_BitMask);
   *pDisplayItem  = (void*)pDisplayInstance;
   msAPI_XC_DIPMultiView_Init(2, E_MSAPI_XC_DIP_FMT_YUV422);
   return TRUE;
}

MMSDK_BOOL PT_Display_SetModeEX(PT_DISPLAYITEM DisplayItem, ST_MMSDK_SET_DISPLAY_INFO_EXT *pDisplayInfoExt)
{
    PT_DISP_DBG("");
    if ((DisplayItem == NULL) || (pDisplayInfoExt == NULL))
    {
        PT_DISP_ERR("DisplayItem or pDisplayInfoExt is NULL\n");
        return FALSE;
    }

    PT_DISP_DBG("[%s][%d]========TRACE=== DisplayItem=%p==\n",__FUNCTION__,__LINE__,DisplayItem);
    _ST_MMSDK_DISPLAY_INSTANCE* pDisplayInstance = (_ST_MMSDK_DISPLAY_INSTANCE*)DisplayItem;
    PT_DISP_DBG("[%s][%d]====pDisplayInfoExt ->u8DisplayOutputPath=%d===\n",__FUNCTION__,__LINE__,pDisplayInfoExt ->u8DisplayOutputPath);
    PT_DISP_DBG("[%s][%d]====pDisplayInstance ->u8ID_BitMask=%d===\n",__FUNCTION__,__LINE__,pDisplayInstance ->u8ID_BitMask);

    pDisplayInstance->bIsAvpFlow = TRUE;
    memcpy(&(pDisplayInstance->stDisplayInfo), &(pDisplayInfoExt->stDispInfo), sizeof(ST_MMSDK_SET_DISPLAY_INFO));

    if(NULL != gstSysCusFunPtr.pSetMode2)
    {
        MMSDK_BOOL bRet = FALSE;
        MMSDK_BOOL bEnableDS;
        MMSDK_U16 u16MediaType;
        EN_MMSDK_MEDIA_TYPE enMediaType = pDisplayInstance->stDisplayInfo.enMediaType;
        PT_SYS_CusSetModeInfo stCusSetModeInfo;
        memset(&stCusSetModeInfo, 0x00, sizeof(stCusSetModeInfo));

        u16MediaType   = enMediaType;
        bEnableDS      = pDisplayInfoExt->stDispInfo.bEnableDynScaling;
        stCusSetModeInfo.u32SizeofStructure = sizeof(stCusSetModeInfo);
        stCusSetModeInfo.bIsDSEnabled = bEnableDS;

        switch(pDisplayInfoExt ->u8DisplayOutputPath)
        {
            case DIP_FLOW:
                PT_DISP_DBG("[%s][%d]====DIP_FLOW====\n",__FUNCTION__,__LINE__);
                stCusSetModeInfo.u8Flow = DIP_FLOW;
                pDisplayInstance ->u8Flow = DIP_FLOW;
                bRet = gstSysCusFunPtr.pSetMode2(u16MediaType, &stCusSetModeInfo, NULL);
                break;
            case XC_FLOW:
            default:
                PT_DISP_DBG("[%s][%d]====XC_FLOW====\n",__FUNCTION__,__LINE__);
                pDisplayInstance ->u8Flow = XC_FLOW;
                bRet = PT_Display_SetMode(DisplayItem, &(pDisplayInfoExt->stDispInfo));
                break;
        }
        return bRet;
    }

    switch(pDisplayInfoExt ->u8DisplayOutputPath)
    {
        case DIP_FLOW:
#if defined(MMSDK_MultiWin_MODE) && (MMSDK_MultiWin_MODE == 0)
#else
        PT_DISP_DBG("[%s][%d]====DIP_FLOW====\n",__FUNCTION__,__LINE__);
#if defined(MMSDK_DMS_ENABLE) && (MMSDK_DMS_ENABLE == 1)
        PT_Display_BlackScreen(DisplayItem, TRUE);
#endif

#ifdef HDR_MM_DEV
        _PT_Display_CFDInit();
#endif
        pDisplayInstance ->u8Flow = DIP_FLOW;
//        msAPI_XC_DIPMultiView_Init(2, E_MSAPI_XC_DIP_FMT_YUV422);

        // Get Window ID
        if(_PT_Display_GetWindowID(&(pDisplayInstance ->u8DIPWin)) == FALSE)
        {
            PT_DISP_DBG("DMS get Window ID fail\n");
            return FALSE;
        }

        //for SD out [temp], use AA mode
        msAPI_XC_SetConnect_EX(&g_stXC_PT_DeviceId[1], E_XC_EX_INPUT_SOURCE_SCALER_OP, E_MSAPI_XC_MAIN_WINDOW);
        MApi_XC_SetOSD2VEMode(E_VOP_SEL_OSD_LAST);
        msAPI_XC_SetWin_EX(&g_stXC_PT_DeviceId[1], NULL, NULL, NULL, E_XC_EX_MAIN_WINDOW);
        msAPI_XC_SetVideoMuteByMode_EX(&g_stXC_PT_DeviceId[1], FALSE, E_XC_EX_MAIN_WINDOW, E_MSAPI_XC_VIDEO_MUTE_MODE_AUTO_SYNC);
        //Because of VE sett output timing will enable ve black screen,
        //disable black screen here
        msAPI_VE_SetVideoMute(DISABLE);
#endif
        break;
        case XC_FLOW:
        default:
        PT_DISP_DBG("[%s][%d]====XC_FLOW====\n",__FUNCTION__,__LINE__);
        pDisplayInstance ->u8Flow = XC_FLOW;
        PT_Display_SetMode(DisplayItem, &(pDisplayInfoExt->stDispInfo));
        break;
    }

    return TRUE;
}

MMSDK_BOOL PT_Display_SetOneField(PT_DISPLAYITEM DisplayItem, MMSDK_BOOL bEnable)
{
    PT_DISP_DBG("");
    return FALSE;
}

MMSDK_BOOL PT_Display_MvopSendCommand(PT_DISPLAYITEM DisplayItem, void* pHandle, MMSDK_U32 u32Cmd, void *pPara)
{
    PT_DISP_DBG("");
    return FALSE;
}

MMSDK_BOOL PT_Display_MvopGetCommand(PT_DISPLAYITEM DisplayItem, void* pHandle, MMSDK_U32 u32Cmd, void *pPara, MMSDK_U32 u32ParamSize)
{
    PT_DISP_DBG("");
    return FALSE;
}

MMSDK_BOOL PT_Display_IsCurrentFBLMode(PT_DISPLAYITEM DisplayItem, MMSDK_BOOL *pbFBL)
{
    PT_DISP_DBG("");
    return FALSE;
}

MMSDK_BOOL PT_Display_Enable_FBL_TwoInitFactor(PT_DISPLAYITEM DisplayItem, MMSDK_BOOL bEnable)
{
    PT_DISP_DBG("");
    return FALSE;
}

E_MSAPI_XC_VIDEO_DATA_FMT _PT_Display_GetDIPDataFMT(EN_MMSDK_VIDEO_CODEC eCodec,MS_BOOL b10bit)
{
    E_MSAPI_XC_VIDEO_DATA_FMT eFmt = E_MSAPI_XC_VIDEO_DATA_FMT_MAX;

    switch(eCodec)
    {
        case E_MMSDK_VIDEO_CODEC_MJPEG:
                eFmt = E_MSAPI_XC_VIDEO_DATA_FMT_YUV422;
                break;
        case E_MMSDK_VIDEO_CODEC_HEVC:
                {
                    if(b10bit)
                        eFmt = E_MSAPI_XC_VIDEO_DATA_FMT_YUV420_H265_10BITS;
                    else
                        eFmt = E_MSAPI_XC_VIDEO_DATA_FMT_YUV420_H265;
                    break;
                }
        case E_MMSDK_VIDEO_CODEC_H264:
        default :
                eFmt = E_MSAPI_XC_VIDEO_DATA_FMT_YUV420;
                break;
    }

    return eFmt;
}

E_MSAPI_XC_VIDEO_SCAN_TYPE _PT_Display_GetXCScanType(MS_U8 u8ScanType)
{
    // ProgressiveFrame = 0,   /** Picture is a progressive frame */
    // InterlacedFrame  = 2,    /** Picture is an interlaced frame */
    // InterlacedField  = 3,      /** Picture is two interlaced fields */

    E_MSAPI_XC_VIDEO_SCAN_TYPE eMsapiScanType = E_MSAPI_XC_VIDEO_SCAN_TYPE_MAX;

    switch (u8ScanType)
    {
        case 0:
            eMsapiScanType = E_MSAPI_XC_VIDEO_SCAN_TYPE_PROGRESSIVE;
            break;
        case 2:
            eMsapiScanType = E_MSAPI_XC_VIDEO_SCAN_TYPE_INTERLACE_FRAME;
            break;
        case 3:
            eMsapiScanType = E_MSAPI_XC_VIDEO_SCAN_TYPE_INTERLACE_FIELD;
            break;
        default:
            eMsapiScanType = E_MSAPI_XC_VIDEO_SCAN_TYPE_MAX;
            break;
    }
    return eMsapiScanType;
}

MMSDK_BOOL PT_Display_PushVideoFrame(PT_DISPLAYITEM DisplayItem, ST_MMSDK_VIDEO_FRAME_INFO_EXT *pstPushInfo)
{
    //PT_DISP_DBG("");
    if ((DisplayItem == NULL) || (pstPushInfo == NULL))
    {
        PT_DISP_ERR("DisplayItem or pstPushInfo is NULL\n");
        return FALSE;
    }

#if (MMSDK_MultiWin_MODE == 0)
    PT_DISP_ERR("Unsupport DIP Flow!!");
    return FALSE;

#else
    MMSDK_U8 u8FieldNum = 0;
    MMSDK_U8 u8FrameIdx = 0;
    _ST_MMSDK_DISPLAY_INSTANCE* pDisplayInstance = (_ST_MMSDK_DISPLAY_INSTANCE*)DisplayItem;
    MSAPI_XC_VDECFRAME_INFO VDECDispFrame={};

    if(pDisplayInstance ->u8DIPWin == DIP_Win_NonUse)          //invalid winID
        return FALSE;
    //PT_DISP_DBG("[%d]====pDisplayInstance ->u8DIPWin=%d===",__LINE__,pDisplayInstance ->u8DIPWin);
    //PT_DISP_DBG("[%d]====pDisplayInstance ->u8ID_BitMask=%d===",__LINE__,pDisplayInstance ->u8ID_BitMask);
    VDECDispFrame.u32FrameRate                                 = pstPushInfo->u32FrameRate;
    VDECDispFrame.eWindow                                      = (E_MSAPI_XC_WINDOW)(pDisplayInstance->u8DIPWin);
    VDECDispFrame.eCODEC                                       = pstPushInfo->astFrameInfo[0].enType;
    VDECDispFrame.eTileMode                                    = pstPushInfo->eTileMode;
    VDECDispFrame.u64Pts                                       = pstPushInfo->u64PTS;
    VDECDispFrame.eFmt                                         = _PT_Display_GetDIPDataFMT(pstPushInfo->astFrameInfo[0].enType, pstPushInfo->astFrameFormat[0].bIs10Bit);
    VDECDispFrame.eFieldOrderType                              = !(pstPushInfo->bIsBottomFieldFirst);
    VDECDispFrame.stVDECStream_Id_Version.u32VDECStreamID      = pstPushInfo->u32VDECStreamID;
    VDECDispFrame.stVDECStream_Id_Version.u32VDECStreamVersion = pstPushInfo->u32VDECStreamVersion;
    VDECDispFrame.eScanType                                    = _PT_Display_GetXCScanType(pstPushInfo->u8ScanType);

    u8FieldNum = (VDECDispFrame.eScanType == E_MSAPI_XC_VIDEO_SCAN_TYPE_INTERLACE_FIELD);
    for(u8FrameIdx = 0;u8FrameIdx <= u8FieldNum; u8FrameIdx++)
    {
        VDECDispFrame.stFrames[u8FrameIdx].u16SrcWidth                               = pstPushInfo->astFrameInfo[u8FrameIdx].u32Width;
        VDECDispFrame.stFrames[u8FrameIdx].u16SrcHeight                              = pstPushInfo->astFrameInfo[u8FrameIdx].u32Height;
        VDECDispFrame.stFrames[u8FrameIdx].eFieldType                                = pstPushInfo->astFrameFormat[u8FrameIdx].eFieldType;
        VDECDispFrame.stFrames[u8FrameIdx].u16CropRight                              = (MS_U16)pstPushInfo->astFrameFormat[u8FrameIdx].u32CropRight;
        VDECDispFrame.stFrames[u8FrameIdx].u16CropLeft                               = (MS_U16)pstPushInfo->astFrameFormat[u8FrameIdx].u32CropLeft;
        VDECDispFrame.stFrames[u8FrameIdx].u16CropBottom                             = (MS_U16)pstPushInfo->astFrameFormat[u8FrameIdx].u32CropBottom;
        VDECDispFrame.stFrames[u8FrameIdx].u16CropTop                                = (MS_U16)pstPushInfo->astFrameFormat[u8FrameIdx].u32CropTop;
        VDECDispFrame.stFrames[u8FrameIdx].u32FrameIndex                             = pstPushInfo->astFrameFormat[u8FrameIdx].u32FrameIndex;
        VDECDispFrame.stFrames[u8FrameIdx].u32PriData                                = pstPushInfo->astFrameFormat[u8FrameIdx].u32PriData;
        VDECDispFrame.stFrames[u8FrameIdx].b10bitData                                = pstPushInfo->astFrameFormat[u8FrameIdx].bIs10Bit;
        VDECDispFrame.stFrames[u8FrameIdx].stHWFormat.u32SrcLumaAddr                 = pstPushInfo->astFrameInfo[u8FrameIdx].u32LumaAddr;
        VDECDispFrame.stFrames[u8FrameIdx].stHWFormat.u32SrcChromaAddr               = pstPushInfo->astFrameInfo[u8FrameIdx].u32ChromAddr;
        VDECDispFrame.stFrames[u8FrameIdx].stHWFormat.u16SrcPitch                    = pstPushInfo->astFrameInfo[u8FrameIdx].u32Pitch;

#if defined(MMSDK_FRAME_FORMAT_EXT_VER) && (MMSDK_FRAME_FORMAT_EXT_VER == 1)
        VDECDispFrame.stFrames[u8FrameIdx].stHWFormat.u32LumaAddr_subsample          = pstPushInfo->astFrameFormatExt[u8FrameIdx].u64SubsampleLumaAddr;
        VDECDispFrame.stFrames[u8FrameIdx].stHWFormat.u32ChromaAddr_subsample        = pstPushInfo->astFrameFormatExt[u8FrameIdx].u64SubsampleChromaAddr;
        VDECDispFrame.stFrames[u8FrameIdx].stHWFormat.u16Pitch_subsample             = pstPushInfo->astFrameFormatExt[u8FrameIdx].u16SubsamplePitch;
        VDECDispFrame.stFrames[u8FrameIdx].stHWFormat.u8TileMode_subsample           = pstPushInfo->astFrameFormatExt[u8FrameIdx].u8SubsampleTileMode;
        VDECDispFrame.stFrames[u8FrameIdx].stHWFormat.u32HTLBTableAddr               = pstPushInfo->astFrameFormatExt[u8FrameIdx].u64HTLBTableAddr;
        VDECDispFrame.stFrames[u8FrameIdx].stHWFormat.u8HTLBPageSizes                = pstPushInfo->astFrameFormatExt[u8FrameIdx].u8HTLBPageSizes;
        VDECDispFrame.stFrames[u8FrameIdx].stHWFormat.u8HTLBChromaEntriesSize        = pstPushInfo->astFrameFormatExt[u8FrameIdx].u8HTLBChromaEntriesSize;
        VDECDispFrame.stFrames[u8FrameIdx].stHWFormat.u32HTLBChromaEntriesAddr       = pstPushInfo->astFrameFormatExt[u8FrameIdx].u64HTLBChromaEntriesAddr;
        VDECDispFrame.stFrames[u8FrameIdx].stHWFormat.u16MaxContentLightLevel        = pstPushInfo->astFrameFormatExt[u8FrameIdx].u16MaxContentLightLevel;
        VDECDispFrame.stFrames[u8FrameIdx].stHWFormat.u16MaxPicAverageLightLevel     = pstPushInfo->astFrameFormatExt[u8FrameIdx].u16MaxPicAverageLightLevel;
        VDECDispFrame.stFrames[u8FrameIdx].stHWFormat.u8V7DataValid                  = pstPushInfo->astFrameFormatExt[u8FrameIdx].u8V7DataValid;
        VDECDispFrame.stFrames[u8FrameIdx].stHWFormat.u16Width_subsample             = pstPushInfo->astFrameFormatExt[u8FrameIdx].u16SubsampleWidth;
        VDECDispFrame.stFrames[u8FrameIdx].stHWFormat.u16Height_subsample            = pstPushInfo->astFrameFormatExt[u8FrameIdx].u16SubsampleHeight;
        VDECDispFrame.stFrames[u8FrameIdx].stHWFormat.u64NumUnitsInTick              = (MS_U64)pstPushInfo->astFrameFormatExt[u8FrameIdx].u32NumUnitsInTick;
        VDECDispFrame.stFrames[u8FrameIdx].stHWFormat.u64TimeScale                   = (MS_U64)pstPushInfo->astFrameFormatExt[u8FrameIdx].u32TimeScale;
        memcpy(&(VDECDispFrame.stFrames[u8FrameIdx].stHWFormat.u16Histogram), &(pstPushInfo->astFrameFormatExt[u8FrameIdx].au16Histogram), sizeof(MS_U16) * MMSDK_SWDRHISTOGRAM_INDEX_NUM);
#endif

        if(VDECDispFrame.stFrames[u8FrameIdx].b10bitData == TRUE)
        {
            //PT_DISP_DBG("b10bit stream");
            VDECDispFrame.stFrames[u8FrameIdx].stHWFormat.u16Src10bitPitch               = pstPushInfo->astFrameFormat[u8FrameIdx].u16Pitch_2bit ;
            VDECDispFrame.stFrames[u8FrameIdx].stHWFormat.u32SrcLumaAddr_2bit            = pstPushInfo->au32LumaAddrExt[E_MMSDK_DISP_FRM_INFO_EXT_TYPE_10BIT];
            VDECDispFrame.stFrames[u8FrameIdx].stHWFormat.u32SrcChromaAddr_2bit          = pstPushInfo->au32ChromaAddrExt[E_MMSDK_DISP_FRM_INFO_EXT_TYPE_10BIT];
            VDECDispFrame.stFrames[u8FrameIdx].stHWFormat.u8LumaBitdepth                 = pstPushInfo->astFrameFormat[u8FrameIdx].u8LumaBitDepth;
            VDECDispFrame.stFrames[u8FrameIdx].stHWFormat.u8ChromaBitdepth               = pstPushInfo->astFrameFormat[u8FrameIdx].u8ChromaBitdepth;
        }

        //mfcodec info
        VDECDispFrame.stFrames[u8FrameIdx].stHWFormat.stMFdecInfo.bMFDec_Enable = pstPushInfo->u32MFCodecInfo & 0xFF ;
        if(VDECDispFrame.stFrames[u8FrameIdx].stHWFormat.stMFdecInfo.bMFDec_Enable != 0 && VDECDispFrame.stFrames[u8FrameIdx].stHWFormat.stMFdecInfo.bMFDec_Enable != 0xFF)
        {
            VDECDispFrame.stFrames[u8FrameIdx].stHWFormat.stMFdecInfo.bMFDec_Enable      = 1;
            VDECDispFrame.stFrames[u8FrameIdx].stHWFormat.stMFdecInfo.bBypass_codec_mode = 0;
            VDECDispFrame.stFrames[u8FrameIdx].stHWFormat.stMFdecInfo.u8MFDec_Select     = (pstPushInfo->u32MFCodecInfo >>  8) & 0x3;  //Bit[8]      MFDec ID. 1: MFDec 1; 0: MFDec 0; 2: MFDec 2
            VDECDispFrame.stFrames[u8FrameIdx].stHWFormat.stMFdecInfo.bUncompress_mode   = (pstPushInfo->u32MFCodecInfo >> 28) & 0x1;  //Bit[28]     1: MFCodec uncompress mode; 0: data is compressed
            VDECDispFrame.stFrames[u8FrameIdx].stHWFormat.stMFdecInfo.en_MFDecVP9_mode   = (pstPushInfo->u32MFCodecInfo >> 29) & 0x1;  //Bit[29]     1: MFCodec 3.0 vp9 mode; 0: MFCodec 3.0 h26x mode
            VDECDispFrame.stFrames[u8FrameIdx].stHWFormat.stMFdecInfo.u16Bitlen_Pitch    = (pstPushInfo->u32MFCodecInfo >> 16) & 0xFF; //Bits[23:16] MFCodec pitch setting
            VDECDispFrame.stFrames[u8FrameIdx].stHWFormat.stMFdecInfo.u16StartX          = (MS_U16)pstPushInfo->astFrameFormat[u8FrameIdx].u32CropLeft;
            VDECDispFrame.stFrames[u8FrameIdx].stHWFormat.stMFdecInfo.u16StartY          = (MS_U16)pstPushInfo->astFrameFormat[u8FrameIdx].u32CropTop;

            VDECDispFrame.stFrames[u8FrameIdx].stHWFormat.stMFdecInfo.u8MIU_Select       = (pstPushInfo->u32MFCodecInfo >> 24) & 0xF;
            VDECDispFrame.stFrames[u8FrameIdx].stHWFormat.stMFdecInfo.phyBitlen_Base     = (MS_PHY)pstPushInfo->u32MFCBitlenAddr;
//          PT_DISP_DBG("MFDec_Enable : [%d] ",VDECDispFrame.stMFdecInfo.bMFDec_Enable);
//          PT_DISP_DBG("MFDec enable u8MFDec_Select : [%d] ",VDECDispFrame.stMFdecInfo.u8MFDec_Select );
//          PT_DISP_DBG("MFDec enable phyBitlen_Base : [0x%"DTC_MS_PHY_x"] ",VDECDispFrame.stMFdecInfo.phyBitlen_Base);
        }
        else
        {
            VDECDispFrame.stFrames[u8FrameIdx].stHWFormat.stMFdecInfo.bMFDec_Enable      = 0;
//          PT_DISP_DBG("MFDec disable");
        }

//          PT_DISP_DBG("u8FrameIdx:%d u32ScanType:%d bFieldOrder:%d eFieldType:%d\n",u8FrameIdx, VDECDispFrame.eScanType,VDECDispFrame.eFieldOrderType, VDECDispFrame.stFrames[u8FrameIdx].eFieldType);
//          PT_DISP_DBG("id[%d] bMFDec_Enable:[%d]  bMFDec_Sel:[%d] 10bit:[%d]\n",VDECDispFrame.stVDECStream_Id_Version.u32VDECStreamID,VDECDispFrame.stFrames[u8FrameIdx].stHWFormat.stMFdecInfo.bMFDec_Enable, VDECDispFrame.stFrames[u8FrameIdx].stHWFormat.stMFdecInfo.u8MFDec_Select, VDECDispFrame.stFrames[u8FrameIdx].b10bitData);
    }

    if (E_MSAPI_XC_OK == msAPI_XC_DIPMultiView_PushFrame(&VDECDispFrame))
    {
        pDisplayInstance->bIsFramePushed = TRUE;
        return TRUE;
    }
    else
    {
        return FALSE;
    }

#endif
}

MMSDK_BOOL PT_Display_WaitPushVideoFrameDone(PT_DISPLAYITEM DisplayItem, ST_MMSDK_VIDEO_FRAME_INFO_EXT *pstFrameInfo)
{
    //PT_DISP_DBG("");
    if ((DisplayItem == NULL) || (pstFrameInfo == NULL))
    {
        PT_DISP_ERR("DisplayItem or pstFrameInfo is NULL\n");
        return FALSE;
    }

#if (MMSDK_MultiWin_MODE == 0)
    PT_DISP_ERR("Unsupport DIP Flow!!");
    return FALSE;

#elif (MMSDK_MultiWin_MODE == 1)
    _ST_MMSDK_DISPLAY_INSTANCE* pDisplayInstance = (_ST_MMSDK_DISPLAY_INSTANCE*)DisplayItem;
    MSAPI_XC_VDECFRAME_INFO VDECDispFrame;
    MMSDK_U8 u8FieldNum = 0;
    MMSDK_U8 u8FrameIdx = 0;
    MMSDK_U16 u16VsyncTime = 0;

    //PT_DISP_DBG("[%d]====pDisplayInstance ->u8DIPWin=%d===",__LINE__,pDisplayInstance ->u8DIPWin);
    //PT_DISP_DBG("[%d]====pDisplayInstance ->u8ID_BitMask=%d===",__LINE__,pDisplayInstance ->u8ID_BitMask);
    u16VsyncTime = (MMSDK_U16)(1000/PT_Display_GetOutputFrameRate(pDisplayInstance));
    VDECDispFrame.eWindow      = (E_MSAPI_XC_WINDOW)(pDisplayInstance->u8DIPWin);

    u8FieldNum = (_PT_Display_GetXCScanType(pstFrameInfo->u8ScanType) == E_MSAPI_XC_VIDEO_SCAN_TYPE_INTERLACE_FIELD);
    for(u8FrameIdx = 0;u8FrameIdx <= u8FieldNum; u8FrameIdx++)
    {
        VDECDispFrame.stFrames[u8FrameIdx].stHWFormat.u32SrcLumaAddr   = pstFrameInfo->astFrameInfo[u8FrameIdx].u32LumaAddr;
        VDECDispFrame.stFrames[u8FrameIdx].stHWFormat.u32SrcChromaAddr = pstFrameInfo->astFrameInfo[u8FrameIdx].u32ChromAddr;
    }

    while(pDisplayInstance->bIsFramePushed && pDisplayInstance ->u8DIPWin != DIP_Win_NonUse)
    {
        if(msAPI_XC_DIPMultiView_PullFrame(&VDECDispFrame) == E_MSAPI_XC_OK)
        {
            return TRUE;
        }

        MsOS_DelayTask((MS_U32)u16VsyncTime);
    }

    return FALSE;

#else
    return TRUE;

#endif
}


#if defined(MMSDK_DMS_ENABLE) && (MMSDK_DMS_ENABLE == 1)
E_MSAPI_XC_VIDEO_CODEC _PT_Display_GetXCVideoCodec(EN_DMS_CODECTYPE eCodecType)
{
    EN_MMSDK_VIDEO_CODEC eVideoCodec = E_MMSDK_VIDEO_CODEC_UNKNOWN;

    switch(eCodecType)
    {
        case E_DMS_CODEC_TYPE_MPEG4:
            eVideoCodec = E_MSAPI_XC_VIDEO_CODEC_MPEG4;
            break;
        case E_DMS_CODEC_TYPE_MJPEG:
            eVideoCodec = E_MSAPI_XC_VIDEO_CODEC_MJPEG;
            break;
        case E_DMS_CODEC_TYPE_H264:
            eVideoCodec = E_MSAPI_XC_VIDEO_CODEC_H264;
            break;
        case E_DMS_CODEC_TYPE_RV8:
            eVideoCodec = E_MSAPI_XC_VIDEO_CODEC_TYPE_RV8;
            break;
        case E_DMS_CODEC_TYPE_RV9:
            eVideoCodec = E_MSAPI_XC_VIDEO_CODEC_TYPE_RV9;
            break;
        case E_DMS_CODEC_TYPE_MPEG2:
            eVideoCodec = E_MSAPI_XC_VIDEO_CODEC_MPEG;
            break;
        case E_DMS_CODEC_TYPE_VC1_MAIN:
            eVideoCodec = E_MSAPI_XC_VIDEO_CODEC_VC1;
            break;
        case E_DMS_CODEC_TYPE_AVS:
            eVideoCodec = E_MSAPI_XC_VIDEO_CODEC_AVS;
            break;
        case E_DMS_CODEC_TYPE_FLV:
            eVideoCodec = E_MSAPI_XC_VIDEO_CODEC_FLV;
            break;
        case E_DMS_CODEC_TYPE_MVC:
            eVideoCodec = E_MSAPI_XC_VIDEO_CODEC_MVC;
            break;
        //case E_MMSDK_VIDEO_CODEC_VP6:
        //    eVideoCodec = E_MMSDK_VIDEO_CODEC_VP6;
        //    break;
        case E_DMS_CODEC_TYPE_VP8:
            eVideoCodec = E_MSAPI_XC_VIDEO_CODEC_VP8;
            break;
        case E_DMS_CODEC_TYPE_HEVC:
            eVideoCodec = E_MSAPI_XC_VIDEO_CODEC_HEVC;
            break;
        case E_DMS_CODEC_TYPE_VP9:
            eVideoCodec = E_MSAPI_XC_VIDEO_CODEC_VP9;
            break;
        case E_DMS_CODEC_TYPE_HEVC_DV:
            eVideoCodec = E_MSAPI_XC_VIDEO_CODEC_HEVC_DV;
            break;
        default:
            eVideoCodec = E_MSAPI_XC_VIDEO_CODEC_UNKNOWN;
            break;
    }

    return eVideoCodec;

}

MMSDK_BOOL _PT_Display_ConvertFrmFormat(MMSDK_U32 u32WindowID, const ST_MMSDK_DMS_DISPFRAMEFORMAT *pstDispFrameFormat, MSAPI_XC_VDECFRAME_INFO *pstVDECDispFrame)
{
    MMSDK_U8 u8FrameNum = 0;
    MMSDK_U8 u8FrameIdx = 0;
    MMSDK_BOOL bIs10bit = FALSE;

    memset(pstVDECDispFrame, 0x00, sizeof(MSAPI_XC_VDECFRAME_INFO));
    pstVDECDispFrame->u32Version = pstDispFrameFormat->u32Version;
    pstVDECDispFrame->u32Length = pstDispFrameFormat->u32Length;
//    pstVDECDispFrame->u32OverlayID = pstDispFrameFormat->u32OverlayID;
//    pstVDECDispFrame->bValid =

    bIs10bit = (pstDispFrameFormat->enColorFormat ==  E_MMSDK_DMS_COLOR_FORMAT_10BIT_TILE);
    u8FrameNum = pstDispFrameFormat->u32FrameNum; // DV Dual layer & interlace field = 2

    pstVDECDispFrame->u32FrameRate                                  = pstDispFrameFormat->u32FrameRate;
    pstVDECDispFrame->eWindow                                       = (E_MSAPI_XC_WINDOW)u32WindowID;
    pstVDECDispFrame->eCODEC                                        = _PT_Display_GetXCVideoCodec(pstDispFrameFormat->u32CodecType);   /// need convert
    pstVDECDispFrame->eTileMode                                     = pstDispFrameFormat->u32TileMode; /// need convert
    pstVDECDispFrame->u64Pts                                        = pstDispFrameFormat->u64Pts;
    pstVDECDispFrame->eFmt                                          = _PT_Display_GetDIPDataFMT(pstVDECDispFrame->eCODEC, bIs10bit); /// need convert
    pstVDECDispFrame->eFieldOrderType                               = !(pstDispFrameFormat->u8BottomFieldFirst);
    pstVDECDispFrame->stVDECStream_Id_Version.u32VDECStreamID       = pstDispFrameFormat->u32VdecStreamId;
    pstVDECDispFrame->stVDECStream_Id_Version.u32VDECStreamVersion  = pstDispFrameFormat->u32VdecStreamVersion;
    pstVDECDispFrame->eScanType                                     = _PT_Display_GetXCScanType(pstDispFrameFormat->u8Interlace);

    pstVDECDispFrame->u8FieldCtrl                                   = pstDispFrameFormat->u8FieldCtrl;          // control one field mode, always top or bot when FF or FR
    pstVDECDispFrame->u8AspectRate                                  = pstDispFrameFormat->u8AspectRate;
    pstVDECDispFrame->u8Interlace                                   = pstDispFrameFormat->u8Interlace;
    pstVDECDispFrame->u8FrcMode                                     = pstDispFrameFormat->u8FrcMode;
    pstVDECDispFrame->u83DMode                                      = pstDispFrameFormat->u83DMode;
    pstVDECDispFrame->u8FreezeThisFrame                             = pstDispFrameFormat->u8FreezeThisFrame;
    pstVDECDispFrame->u8ToggleTime                                  = pstDispFrameFormat->u8ToggleTime;
    pstVDECDispFrame->u83DLayout                                    = pstDispFrameFormat->u83DLayout;           // 3D layout from SEI, the possible value is OMX_3D_LAYOUT enum in OMX_Video.h
    pstVDECDispFrame->u8ColorInXVYCC                                = pstDispFrameFormat->u8ColorInXVYCC;
    pstVDECDispFrame->u8LowLatencyMode                              = pstDispFrameFormat->u8LowLatencyMode;     // for CTS or other application, drop new frame when render too fast
    pstVDECDispFrame->u8VdecComplexity                              = pstDispFrameFormat->u8VdecComplexity;
    pstVDECDispFrame->u8AFD                                         = pstDispFrameFormat->u8AFD;               //active frame code
    pstVDECDispFrame->u8HTLBTableId                                 = pstDispFrameFormat->u8HTLBTableId;
    pstVDECDispFrame->u8HTLBEntriesSize                             =  pstDispFrameFormat->u8HTLBEntriesSize;
    pstVDECDispFrame->phyHTLBEntriesAddr                            = pstDispFrameFormat->phyHTLBEntriesAddr;

//    PT_DISP_DBG("\033[36m [%s][%d] scan type = %d, codec = %d, color fmt = %d  \033[0m\n", __FUNCTION__, __LINE__,
//        pstVDECDispFrame->eScanType, pstVDECDispFrame->eCODEC, pstVDECDispFrame->eFmt );


    for(u8FrameIdx = 0;u8FrameIdx < u8FrameNum; u8FrameIdx++)
    {
        pstVDECDispFrame->stFrames[u8FrameIdx].u32Version                                = pstDispFrameFormat->stFrames[u8FrameIdx].u32Version;
        pstVDECDispFrame->stFrames[u8FrameIdx].u32Length                                 = pstDispFrameFormat->stFrames[u8FrameIdx].u32Length;
        pstVDECDispFrame->stFrames[u8FrameIdx].u16SrcWidth                               = (MS_U16) pstDispFrameFormat->stFrames[u8FrameIdx].u32Width;
        pstVDECDispFrame->stFrames[u8FrameIdx].u16SrcHeight                              = (MS_U16) pstDispFrameFormat->stFrames[u8FrameIdx].u32Height;
        pstVDECDispFrame->stFrames[u8FrameIdx].eFieldType                                = (E_MSAPI_XC_VIDEO_FIELD_TYPE)pstDispFrameFormat->stFrames[u8FrameIdx].enFieldType;
        pstVDECDispFrame->stFrames[u8FrameIdx].u16CropRight                              = (MS_U16) pstDispFrameFormat->stFrames[u8FrameIdx].u32CropRight;
        pstVDECDispFrame->stFrames[u8FrameIdx].u16CropLeft                               = (MS_U16) pstDispFrameFormat->stFrames[u8FrameIdx].u32CropLeft;
        pstVDECDispFrame->stFrames[u8FrameIdx].u16CropBottom                             = (MS_U16) pstDispFrameFormat->stFrames[u8FrameIdx].u32CropBottom;
        pstVDECDispFrame->stFrames[u8FrameIdx].u16CropTop                                = (MS_U16) pstDispFrameFormat->stFrames[u8FrameIdx].u32CropTop;
        pstVDECDispFrame->stFrames[u8FrameIdx].u32FrameIndex                             = pstDispFrameFormat->stFrames[u8FrameIdx].u32Idx;
        pstVDECDispFrame->stFrames[u8FrameIdx].u32PriData                                = pstDispFrameFormat->stFrames[u8FrameIdx].u32PriData;
        pstVDECDispFrame->stFrames[u8FrameIdx].b10bitData                                = bIs10bit;

        pstVDECDispFrame->stFrames[u8FrameIdx].stHWFormat.u32Version                     = pstDispFrameFormat->stFrames[u8FrameIdx].stHWFormat.u32Version;
        pstVDECDispFrame->stFrames[u8FrameIdx].stHWFormat.u32Length                      = pstDispFrameFormat->stFrames[u8FrameIdx].stHWFormat.u32Length;
        pstVDECDispFrame->stFrames[u8FrameIdx].stHWFormat.u32SrcLumaAddr                 = pstDispFrameFormat->stFrames[u8FrameIdx].stHWFormat.phyLumaAddr;
        pstVDECDispFrame->stFrames[u8FrameIdx].stHWFormat.u32SrcChromaAddr               = pstDispFrameFormat->stFrames[u8FrameIdx].stHWFormat.phyChromaAddr;
        pstVDECDispFrame->stFrames[u8FrameIdx].stHWFormat.u16SrcPitch                    = pstDispFrameFormat->stFrames[u8FrameIdx].stHWFormat.u32LumaPitch;

#if defined(MMSDK_FRAME_FORMAT_EXT_VER) && (MMSDK_FRAME_FORMAT_EXT_VER == 1)
        pstVDECDispFrame->stFrames[u8FrameIdx].stHWFormat.u8V7DataValid                  = pstDispFrameFormat->stFrames[u8FrameIdx].stHWFormat.u8V7DataValid;
        pstVDECDispFrame->stFrames[u8FrameIdx].stHWFormat.u32LumaAddr_subsample          = pstDispFrameFormat->stFrames[u8FrameIdx].stHWFormat.phyLumaAddr_subsample;
        pstVDECDispFrame->stFrames[u8FrameIdx].stHWFormat.u32ChromaAddr_subsample        = pstDispFrameFormat->stFrames[u8FrameIdx].stHWFormat.phyChromaAddr_subsample;
        pstVDECDispFrame->stFrames[u8FrameIdx].stHWFormat.u16Pitch_subsample             = pstDispFrameFormat->stFrames[u8FrameIdx].stHWFormat.u16Pitch_subsample;
        pstVDECDispFrame->stFrames[u8FrameIdx].stHWFormat.u8TileMode_subsample           = pstDispFrameFormat->stFrames[u8FrameIdx].stHWFormat.u8TileMode_subsample; // vsync do not send it
        pstVDECDispFrame->stFrames[u8FrameIdx].stHWFormat.u32HTLBTableAddr               = pstDispFrameFormat->stFrames[u8FrameIdx].stHWFormat.phyHTLBTableAddr;
        pstVDECDispFrame->stFrames[u8FrameIdx].stHWFormat.u8HTLBPageSizes                = pstDispFrameFormat->stFrames[u8FrameIdx].stHWFormat.u8HTLBPageSizes;
        pstVDECDispFrame->stFrames[u8FrameIdx].stHWFormat.u8HTLBChromaEntriesSize        = pstDispFrameFormat->stFrames[u8FrameIdx].stHWFormat.u8HTLBChromaEntriesSize;
        pstVDECDispFrame->stFrames[u8FrameIdx].stHWFormat.u32HTLBChromaEntriesAddr       = pstDispFrameFormat->stFrames[u8FrameIdx].stHWFormat.phyHTLBChromaEntriesAddr;
        pstVDECDispFrame->stFrames[u8FrameIdx].stHWFormat.u16MaxContentLightLevel        = pstDispFrameFormat->stFrames[u8FrameIdx].stHWFormat.u16MaxContentLightLevel;
        pstVDECDispFrame->stFrames[u8FrameIdx].stHWFormat.u16MaxPicAverageLightLevel     = pstDispFrameFormat->stFrames[u8FrameIdx].stHWFormat.u16MaxPicAverageLightLevel;

        pstVDECDispFrame->stFrames[u8FrameIdx].stHWFormat.u16Width_subsample             = pstDispFrameFormat->stFrames[u8FrameIdx].stHWFormat.u16Width_subsample;
        pstVDECDispFrame->stFrames[u8FrameIdx].stHWFormat.u16Height_subsample            = pstDispFrameFormat->stFrames[u8FrameIdx].stHWFormat.u16Height_subsample;
        pstVDECDispFrame->stFrames[u8FrameIdx].stHWFormat.u64NumUnitsInTick              = pstDispFrameFormat->stFrames[u8FrameIdx].stHWFormat.u64NumUnitsInTick;
        pstVDECDispFrame->stFrames[u8FrameIdx].stHWFormat.u64TimeScale                   = pstDispFrameFormat->stFrames[u8FrameIdx].stHWFormat.u64TimeScale;
        memcpy(pstVDECDispFrame->stFrames[u8FrameIdx].stHWFormat.u16Histogram, pstDispFrameFormat->stFrames[u8FrameIdx].stHWFormat.u16Histogram, sizeof(MS_U16) * MMSDK_SWDRHISTOGRAM_INDEX_NUM);
#endif

        if(bIs10bit)
        {
            //PT_DISP_DBG("b10bit stream");
            pstVDECDispFrame->stFrames[u8FrameIdx].stHWFormat.u16Src10bitPitch               = pstDispFrameFormat->stFrames[u8FrameIdx].stHWFormat.u32LumaPitch2Bit;
            pstVDECDispFrame->stFrames[u8FrameIdx].stHWFormat.u32SrcLumaAddr_2bit            = pstDispFrameFormat->stFrames[u8FrameIdx].stHWFormat.phyLumaAddr2Bit;
            pstVDECDispFrame->stFrames[u8FrameIdx].stHWFormat.u32SrcChromaAddr_2bit          = pstDispFrameFormat->stFrames[u8FrameIdx].stHWFormat.phyChromaAddr2Bit;
            pstVDECDispFrame->stFrames[u8FrameIdx].stHWFormat.u8LumaBitdepth                 = pstDispFrameFormat->stFrames[u8FrameIdx].stHWFormat.phyLumaAddr2Bit;
            pstVDECDispFrame->stFrames[u8FrameIdx].stHWFormat.u8ChromaBitdepth               = pstDispFrameFormat->stFrames[u8FrameIdx].stHWFormat.phyChromaAddr2Bit;
        }
        //mfcodec info

        MMSDK_U32 u32MFCInfo = pstDispFrameFormat->stFrames[u8FrameIdx].stHWFormat.u32MFCodecInfo;
        pstVDECDispFrame->stFrames[u8FrameIdx].stHWFormat.stMFdecInfo.bMFDec_Enable = u32MFCInfo & 0xFF ;
        if(pstVDECDispFrame->stFrames[u8FrameIdx].stHWFormat.stMFdecInfo.bMFDec_Enable != 0 && pstVDECDispFrame->stFrames[u8FrameIdx].stHWFormat.stMFdecInfo.bMFDec_Enable != 0xFF)
        {
            pstVDECDispFrame->stFrames[u8FrameIdx].stHWFormat.stMFdecInfo.bMFDec_Enable      = 1;
            pstVDECDispFrame->stFrames[u8FrameIdx].stHWFormat.stMFdecInfo.bBypass_codec_mode = 0;
            pstVDECDispFrame->stFrames[u8FrameIdx].stHWFormat.stMFdecInfo.u8MFDec_Select     = (u32MFCInfo >>  8) & 0x3;  //Bit[8]      MFDec ID. 1: MFDec 1; 0: MFDec 0; 2: MFDec 2
            pstVDECDispFrame->stFrames[u8FrameIdx].stHWFormat.stMFdecInfo.bUncompress_mode   = (u32MFCInfo >> 28) & 0x1;  //Bit[28]     1: MFCodec uncompress mode; 0: data is compressed
            pstVDECDispFrame->stFrames[u8FrameIdx].stHWFormat.stMFdecInfo.en_MFDecVP9_mode   = (u32MFCInfo >> 29) & 0x1;  //Bit[29]     1: MFCodec 3.0 vp9 mode; 0: MFCodec 3.0 h26x mode
            pstVDECDispFrame->stFrames[u8FrameIdx].stHWFormat.stMFdecInfo.u16Bitlen_Pitch    = (u32MFCInfo >> 16) & 0xFF; //Bits[23:16] MFCodec pitch setting
            pstVDECDispFrame->stFrames[u8FrameIdx].stHWFormat.stMFdecInfo.u16StartX          = (MS_U16) pstDispFrameFormat->stFrames[u8FrameIdx].u32CropLeft;
            pstVDECDispFrame->stFrames[u8FrameIdx].stHWFormat.stMFdecInfo.u16StartY          = (MS_U16) pstDispFrameFormat->stFrames[u8FrameIdx].u32CropTop;

            pstVDECDispFrame->stFrames[u8FrameIdx].stHWFormat.stMFdecInfo.u8MIU_Select       = (u32MFCInfo >> 24) & 0xF;
            pstVDECDispFrame->stFrames[u8FrameIdx].stHWFormat.stMFdecInfo.phyBitlen_Base     =  pstDispFrameFormat->stFrames[u8FrameIdx].stHWFormat.phyMFCBITLEN;
            //          PT_DISP_DBG("MFDec_Enable : [%d] ",VDECDispFrame.stMFdecInfo.bMFDec_Enable);
            //          PT_DISP_DBG("MFDec enable u8MFDec_Select : [%d] ",VDECDispFrame.stMFdecInfo.u8MFDec_Select );
            //          PT_DISP_DBG("MFDec enable phyBitlen_Base : [0x%"DTC_MS_PHY_x"] ",VDECDispFrame.stMFdecInfo.phyBitlen_Base);
        }
        else
        {
            pstVDECDispFrame->stFrames[u8FrameIdx].stHWFormat.stMFdecInfo.bMFDec_Enable      = 0;
            //          PT_DISP_DBG("MFDec disable");
        }
    }

    pstVDECDispFrame->stHDRInfo.u32Version                                      = pstDispFrameFormat->stHDRInfo.u32Version;
    pstVDECDispFrame->stHDRInfo.u32Length                                       = pstDispFrameFormat->stHDRInfo.u32Length;
    pstVDECDispFrame->stHDRInfo.u32FrmInfoExtAvail                              = pstDispFrameFormat->stHDRInfo.u32FrmInfoExtAvail;

    pstVDECDispFrame->stHDRInfo.stColorDescription.u32Version                   = pstDispFrameFormat->stHDRInfo.stColorDescription.u32Version;
    pstVDECDispFrame->stHDRInfo.stColorDescription.u32Length                    = pstDispFrameFormat->stHDRInfo.stColorDescription.u32Length;
    pstVDECDispFrame->stHDRInfo.stColorDescription.u8ColorPrimaries             = pstDispFrameFormat->stHDRInfo.stColorDescription.u8ColorPrimaries;
    pstVDECDispFrame->stHDRInfo.stColorDescription.u8TransferCharacteristics    = pstDispFrameFormat->stHDRInfo.stColorDescription.u8TransferCharacteristics;
    pstVDECDispFrame->stHDRInfo.stColorDescription.u8MatrixCoefficients         = pstDispFrameFormat->stHDRInfo.stColorDescription.u8MatrixCoefficients;

#if 0        // Vsync do not send MasterColorDisplay Info
    pstVDECDispFrame->stHDRInfo.stMasterColorDisplay.u32Version = pstDispFrameFormat->stHDRInfo.stMasterColorDisplay.u32Version;      /// MSAPI_XC_MASTERCOLORDISPLAY version
    pstVDECDispFrame->stHDRInfo.stMasterColorDisplay.u32Length = pstDispFrameFormat->stHDRInfo.stMasterColorDisplay.u32Length;        /// sizeof(MSAPI_XC_MASTERCOLORDISPLAY)
    //mastering color display: color volumne of a display
    pstVDECDispFrame->stHDRInfo.stMasterColorDisplay.u32MaxLuminance = pstDispFrameFormat->stHDRInfo.stMasterColorDisplay.u32MaxLuminance;
    pstVDECDispFrame->stHDRInfo.stMasterColorDisplay.u32MinLuminance = pstDispFrameFormat->stHDRInfo.stMasterColorDisplay.u32MinLuminance;
    memcpy(pstVDECDispFrame->stHDRInfo.stMasterColorDisplay.u16DisplayPrimaries, pstDispFrameFormat->stHDRInfo.stMasterColorDisplay.u16DisplayPrimaries, sizeof(pstDispFrameFormat->stHDRInfo.stMasterColorDisplay.u16DisplayPrimaries));
    memcpy(pstVDECDispFrame->stHDRInfo.stMasterColorDisplay.u16WhitePoint, pstDispFrameFormat->stHDRInfo.stMasterColorDisplay.u16WhitePoint, sizeof(pstDispFrameFormat->stHDRInfo.stMasterColorDisplay.u16WhitePoint));
#endif

    pstVDECDispFrame->stHDRInfo.stDolbyHDRInfo.u32Version                       = pstDispFrameFormat->stHDRInfo.stDolbyHDRInfo.u32Version;        /// MSAPI_XC_DOLBYHDRINFO version
    pstVDECDispFrame->stHDRInfo.stDolbyHDRInfo.u32Length                        = pstDispFrameFormat->stHDRInfo.stDolbyHDRInfo.u32Length;          /// sizeof(MSAPI_XC_DOLBYHDRINFO)
    // bit[0:1] 0: Disable 1:Single layer 2: Dual layer, bit[2] 0:Base Layer 1:Enhance Layer
    pstVDECDispFrame->stHDRInfo.stDolbyHDRInfo.u8DVMode                         = pstDispFrameFormat->stHDRInfo.stDolbyHDRInfo.u8DVMode;
    pstVDECDispFrame->stHDRInfo.stDolbyHDRInfo.phyHDRMetadataAddr               = pstDispFrameFormat->stHDRInfo.stDolbyHDRInfo.phyHDRMetadataAddr;
    pstVDECDispFrame->stHDRInfo.stDolbyHDRInfo.u32HDRMetadataSize               = pstDispFrameFormat->stHDRInfo.stDolbyHDRInfo.u32HDRMetadataSize;
    pstVDECDispFrame->stHDRInfo.stDolbyHDRInfo.phyHDRRegAddr                    = pstDispFrameFormat->stHDRInfo.stDolbyHDRInfo.phyHDRRegAddr;
    pstVDECDispFrame->stHDRInfo.stDolbyHDRInfo.u32HDRRegSize                    = pstDispFrameFormat->stHDRInfo.stDolbyHDRInfo.u32HDRRegSize;
    pstVDECDispFrame->stHDRInfo.stDolbyHDRInfo.phyHDRLutAddr                    = pstDispFrameFormat->stHDRInfo.stDolbyHDRInfo.phyHDRLutAddr;
    pstVDECDispFrame->stHDRInfo.stDolbyHDRInfo.u32HDRLutSize                    = pstDispFrameFormat->stHDRInfo.stDolbyHDRInfo.u32HDRLutSize;
    pstVDECDispFrame->stHDRInfo.stDolbyHDRInfo.u8DMEnable                       = pstDispFrameFormat->stHDRInfo.stDolbyHDRInfo.u8DMEnable;
    pstVDECDispFrame->stHDRInfo.stDolbyHDRInfo.u8CompEnable                     = pstDispFrameFormat->stHDRInfo.stDolbyHDRInfo.u8CompEnable;
    pstVDECDispFrame->stHDRInfo.stDolbyHDRInfo.u8CurrentIndex                   = pstDispFrameFormat->stHDRInfo.stDolbyHDRInfo.u8CurrentIndex;

#if 0        // msapi XC has no interface to  receive
    pstVDECDispFrame->stFrames[0].eFrameType = (EN_MMSDK_DMS_FRAMETYPE)pstDispFrameFormat->stFrames[0].eFrameType;
    pstVDECDispFrame->stFrames[0].enViewType = (EN_MMSDK_DMS_VIEWTYPE)pstDispFrameFormat->stFrames[0].eViewType;
    pstVDECDispFrame->stFrames[0].stHWFormat.u32ChromaPitch = pstDispFrameFormat->stFrames[0].stHWFormat.u32ChromaPitch;

    // FIXME: 2 bit patch
    pstVDECDispFrame->stFrames[0].stHWFormat.u32ChromaPitch2Bit = pstDispFrameFormat->stFrames[0].stHWFormat.u32ChromaPitch2Bit

    //pstVDECDispFrame->u32FrameNum = pstDispFrameFormat->FrameNum;
    pstVDECDispFrame->u32AspectWidth = pstDispFrameFormat->u32AspectWidth;
    pstVDECDispFrame->u32AspectHeight = pstDispFrameFormat->u32AspectHeight;
    pstVDECDispFrame->u32UniqueId = pstDispFrameFormat->u32UniqueId;
    pstVDECDispFrame->u8MCUMode = pstDispFrameFormat->u8MCUMode;
    pstVDECDispFrame->u8ApplicationType = pstDispFrameFormat->u8ApplicationType;
    memcpy(&pDMSDff->stDispFrmInfoExt, &pDff->sDispFrmInfoExt, sizeof(ST_MMSDK_DMS_DISP_FRM_INFO_EXT));
    //pstVDECDispFrame->u16MIUBandwidth;
    //pstVDECDispFrame->u16Bitrate;
    pstVDECDispFrame->phyVsyncBridgeAddr = pstDispFrameFormat->u32SHMAddr;
    pstVDECDispFrame->phyVsyncBridgeExtAddr = pstDispFrameFormat->phyVsyncBridgeExtAddr;
#endif

    return TRUE;
}


MMSDK_BOOL _PT_Display_ConvertFrmFormat_DMS(MMSDK_U32 u32WindowID, const ST_MMSDK_DMS_DISPFRAMEFORMAT *pstDispFrameFormat, ST_DMS_DISPFRAMEFORMAT *pstDispFrameFmt)
{
    int i=0;

    memset(pstDispFrameFmt, 0, sizeof(ST_DMS_DISPFRAMEFORMAT));
    pstDispFrameFmt->u32Version = pstDispFrameFormat->u32Version;
    pstDispFrameFmt->u32Length = pstDispFrameFormat->u32Length;
    pstDispFrameFmt->u32OverlayID = pstDispFrameFormat->u32OverlayID;

    pstDispFrameFmt->stFrames[0].u32Version = pstDispFrameFormat->stFrames[0].u32Version;
    pstDispFrameFmt->stFrames[0].u32Length = pstDispFrameFormat->stFrames[0].u32Length;
    pstDispFrameFmt->stFrames[0].enFrameType = pstDispFrameFormat->stFrames[0].enFrameType;
    pstDispFrameFmt->stFrames[0].enFieldType = pstDispFrameFormat->stFrames[0].enFieldType;
    pstDispFrameFmt->stFrames[0].enViewType = pstDispFrameFormat->stFrames[0].enViewType;
    pstDispFrameFmt->stFrames[0].u32Width = pstDispFrameFormat->stFrames[0].u32Width;
    pstDispFrameFmt->stFrames[0].u32Height = pstDispFrameFormat->stFrames[0].u32Height;
    pstDispFrameFmt->stFrames[0].u32CropLeft = pstDispFrameFormat->stFrames[0].u32CropLeft;
    pstDispFrameFmt->stFrames[0].u32CropRight = pstDispFrameFormat->stFrames[0].u32CropRight;
    pstDispFrameFmt->stFrames[0].u32CropTop = pstDispFrameFormat->stFrames[0].u32CropTop;
    pstDispFrameFmt->stFrames[0].u32CropBottom = pstDispFrameFormat->stFrames[0].u32CropBottom;

    pstDispFrameFmt->stFrames[0].stHWFormat.u32Version = pstDispFrameFormat->stFrames[0].stHWFormat.u32Version;
    pstDispFrameFmt->stFrames[0].stHWFormat.u32Length = pstDispFrameFormat->stFrames[0].stHWFormat.u32Length;
    pstDispFrameFmt->stFrames[0].stHWFormat.phyLumaAddr = pstDispFrameFormat->stFrames[0].stHWFormat.phyLumaAddr;
    pstDispFrameFmt->stFrames[0].stHWFormat.phyChromaAddr = pstDispFrameFormat->stFrames[0].stHWFormat.phyChromaAddr;
    pstDispFrameFmt->stFrames[0].stHWFormat.phyLumaAddr2Bit = pstDispFrameFormat->stFrames[0].stHWFormat.phyLumaAddr2Bit;
    pstDispFrameFmt->stFrames[0].stHWFormat.phyChromaAddr2Bit = pstDispFrameFormat->stFrames[0].stHWFormat.phyChromaAddr2Bit;
    pstDispFrameFmt->stFrames[0].stHWFormat.u32LumaPitch = pstDispFrameFormat->stFrames[0].stHWFormat.u32LumaPitch;
    pstDispFrameFmt->stFrames[0].stHWFormat.u32ChromaPitch = pstDispFrameFormat->stFrames[0].stHWFormat.u32ChromaPitch;

    //  2 bit patch
    pstDispFrameFmt->stFrames[0].stHWFormat.u32LumaPitch2Bit = pstDispFrameFormat->stFrames[0].stHWFormat.u32LumaPitch2Bit;
    pstDispFrameFmt->stFrames[0].stHWFormat.u32ChromaPitch2Bit = pstDispFrameFormat->stFrames[0].stHWFormat.u32ChromaPitch2Bit;

    pstDispFrameFmt->stFrames[0].stHWFormat.u32MFCodecInfo = pstDispFrameFormat->stFrames[0].stHWFormat.u32MFCodecInfo;
    pstDispFrameFmt->stFrames[0].stHWFormat.phyMFCBITLEN = pstDispFrameFormat->stFrames[0].stHWFormat.phyMFCBITLEN;

    pstDispFrameFmt->stFrames[0].stHWFormat.u8V7DataValid = pstDispFrameFormat->stFrames[0].stHWFormat.u8V7DataValid;
    pstDispFrameFmt->stFrames[0].stHWFormat.u16Width_subsample = pstDispFrameFormat->stFrames[0].stHWFormat.u16Width_subsample;
    pstDispFrameFmt->stFrames[0].stHWFormat.u16Height_subsample = pstDispFrameFormat->stFrames[0].stHWFormat.u16Height_subsample;
    pstDispFrameFmt->stFrames[0].stHWFormat.phyLumaAddr_subsample = pstDispFrameFormat->stFrames[0].stHWFormat.phyLumaAddr_subsample;
    pstDispFrameFmt->stFrames[0].stHWFormat.phyChromaAddr_subsample = pstDispFrameFormat->stFrames[0].stHWFormat.phyChromaAddr_subsample;
    pstDispFrameFmt->stFrames[0].stHWFormat.u16Pitch_subsample = pstDispFrameFormat->stFrames[0].stHWFormat.u16Pitch_subsample;
    pstDispFrameFmt->stFrames[0].stHWFormat.u32HTLBTableAddr = pstDispFrameFormat->stFrames[0].stHWFormat.phyHTLBTableAddr;
    pstDispFrameFmt->stFrames[0].stHWFormat.u8HTLBPageSizes = pstDispFrameFormat->stFrames[0].stHWFormat.u8HTLBPageSizes;

    pstDispFrameFmt->stFrames[0].stHWFormat.u8HTLBChromaEntriesSize = pstDispFrameFormat->stFrames[0].stHWFormat.u8HTLBChromaEntriesSize;
    pstDispFrameFmt->stFrames[0].stHWFormat.u32HTLBChromaEntriesAddr = pstDispFrameFormat->stFrames[0].stHWFormat.phyHTLBChromaEntriesAddr;
    pstDispFrameFmt->stFrames[0].stHWFormat.u16MaxContentLightLevel = pstDispFrameFormat->stFrames[0].stHWFormat.u16MaxContentLightLevel;

    pstDispFrameFmt->stFrames[0].stHWFormat.u16MaxPicAverageLightLevel = pstDispFrameFormat->stFrames[0].stHWFormat.u16MaxPicAverageLightLevel;
    pstDispFrameFmt->stFrames[0].stHWFormat.u64NumUnitsInTick = pstDispFrameFormat->stFrames[0].stHWFormat.u64NumUnitsInTick;
    pstDispFrameFmt->stFrames[0].stHWFormat.u64TimeScale = pstDispFrameFormat->stFrames[0].stHWFormat.u64TimeScale;
    for (i = 0; i < MMSDK_SWDRHISTOGRAM_INDEX_NUM; i++) {
        pstDispFrameFmt->stFrames[0].stHWFormat.u16Histogram[i] = pstDispFrameFormat->stFrames[0].stHWFormat.u16Histogram[i];
    }

    pstDispFrameFmt->stFrames[0].u32Idx = pstDispFrameFormat->stFrames[0].u32Idx;
    pstDispFrameFmt->stFrames[0].u32PriData = pstDispFrameFormat->stFrames[0].u32PriData;
    pstDispFrameFmt->stFrames[0].u8LumaBitdepth = pstDispFrameFormat->stFrames[0].u8LumaBitdepth;
    pstDispFrameFmt->stFrames[0].u8ChromaBitdepth = pstDispFrameFormat->stFrames[0].u8ChromaBitdepth;

    if (pstDispFrameFormat->u32FrameNum == 2) {
        pstDispFrameFmt->stFrames[1].u32Version = pstDispFrameFormat->stFrames[1].u32Version;
        pstDispFrameFmt->stFrames[1].u32Length = pstDispFrameFormat->stFrames[1].u32Length;
        pstDispFrameFmt->stFrames[1].enFrameType = pstDispFrameFormat->stFrames[1].enFrameType;
        pstDispFrameFmt->stFrames[1].enFieldType = pstDispFrameFormat->stFrames[1].enFieldType;
        pstDispFrameFmt->stFrames[1].enViewType = pstDispFrameFormat->stFrames[1].enViewType;
        pstDispFrameFmt->stFrames[1].u32Width = pstDispFrameFormat->stFrames[1].u32Width;
        pstDispFrameFmt->stFrames[1].u32Height = pstDispFrameFormat->stFrames[1].u32Height;
        pstDispFrameFmt->stFrames[1].u32CropLeft = pstDispFrameFormat->stFrames[1].u32CropLeft;
        pstDispFrameFmt->stFrames[1].u32CropRight = pstDispFrameFormat->stFrames[1].u32CropRight;
        pstDispFrameFmt->stFrames[1].u32CropTop = pstDispFrameFormat->stFrames[1].u32CropTop;
        pstDispFrameFmt->stFrames[1].u32CropBottom = pstDispFrameFormat->stFrames[1].u32CropBottom;
                                                                                                         ;
        pstDispFrameFmt->stFrames[1].stHWFormat.u32Version = pstDispFrameFormat->stFrames[1].stHWFormat.u32Version;
        pstDispFrameFmt->stFrames[1].stHWFormat.u32Length = pstDispFrameFormat->stFrames[1].stHWFormat.u32Length;
        pstDispFrameFmt->stFrames[1].stHWFormat.phyLumaAddr = pstDispFrameFormat->stFrames[1].stHWFormat.phyLumaAddr;
        pstDispFrameFmt->stFrames[1].stHWFormat.phyChromaAddr = pstDispFrameFormat->stFrames[1].stHWFormat.phyChromaAddr;
        pstDispFrameFmt->stFrames[1].stHWFormat.phyLumaAddr2Bit = pstDispFrameFormat->stFrames[1].stHWFormat.phyLumaAddr2Bit;
        pstDispFrameFmt->stFrames[1].stHWFormat.phyChromaAddr2Bit = pstDispFrameFormat->stFrames[1].stHWFormat.phyChromaAddr2Bit;
        pstDispFrameFmt->stFrames[1].stHWFormat.u32LumaPitch = pstDispFrameFormat->stFrames[1].stHWFormat.u32LumaPitch;
        pstDispFrameFmt->stFrames[1].stHWFormat.u32ChromaPitch = pstDispFrameFormat->stFrames[1].stHWFormat.u32ChromaPitch;

        //  2 bit patch
        pstDispFrameFmt->stFrames[1].stHWFormat.u32LumaPitch2Bit = pstDispFrameFormat->stFrames[1].stHWFormat.u32LumaPitch2Bit;
        pstDispFrameFmt->stFrames[1].stHWFormat.u32ChromaPitch2Bit = pstDispFrameFormat->stFrames[1].stHWFormat.u32ChromaPitch2Bit;

        pstDispFrameFmt->stFrames[1].stHWFormat.u32MFCodecInfo = pstDispFrameFormat->stFrames[1].stHWFormat.u32MFCodecInfo;
        pstDispFrameFmt->stFrames[1].stHWFormat.phyMFCBITLEN = pstDispFrameFormat->stFrames[1].stHWFormat.phyMFCBITLEN;
        //pstDispFrameFormat->stFrames[1].sHWFormat.phyLumaAddr_subsample;
        //pstDispFrameFormat->stFrames[1].sHWFormat.phyChromaAddr_subsample;
        //pstDispFrameFormat->stFrames[1].sHWFormat.u16Pitch_subsample

        pstDispFrameFmt->stFrames[1].u32Idx = pstDispFrameFormat->stFrames[1].u32Idx;
        pstDispFrameFmt->stFrames[1].u32PriData = pstDispFrameFormat->stFrames[1].u32PriData;
        pstDispFrameFmt->stFrames[1].u8LumaBitdepth = pstDispFrameFormat->stFrames[1].u8LumaBitdepth;
        pstDispFrameFmt->stFrames[1].u8ChromaBitdepth = pstDispFrameFormat->stFrames[1].u8ChromaBitdepth;

        for (i = 0; i < MMSDK_SWDRHISTOGRAM_INDEX_NUM; i++) {
            pstDispFrameFmt->stFrames[1].stHWFormat.u16Histogram[i] = pstDispFrameFormat->stFrames[1].stHWFormat.u16Histogram[i];
        }
    }
    pstDispFrameFmt->enColorFormat = pstDispFrameFormat->enColorFormat;
    pstDispFrameFmt->u32FrameNum = pstDispFrameFormat->u32FrameNum;
    pstDispFrameFmt->u64Pts = pstDispFrameFormat->u64Pts;
    pstDispFrameFmt->u32CodecType = pstDispFrameFormat->u32CodecType;
    pstDispFrameFmt->u32FrameRate = pstDispFrameFormat->u32FrameRate;
    pstDispFrameFmt->u32AspectWidth = pstDispFrameFormat->u32AspectWidth;
    pstDispFrameFmt->u32AspectHeight = pstDispFrameFormat->u32AspectHeight;
    pstDispFrameFmt->u32VdecStreamVersion = pstDispFrameFormat->u32VdecStreamVersion;
    pstDispFrameFmt->u32VdecStreamId = pstDispFrameFormat->u32VdecStreamId;
    pstDispFrameFmt->u32UniqueId = pstDispFrameFormat->u32UniqueId;
    pstDispFrameFmt->u8AspectRate = pstDispFrameFormat->u8AspectRate;
    pstDispFrameFmt->u8Interlace = pstDispFrameFormat->u8Interlace;
    pstDispFrameFmt->u8FrcMode = pstDispFrameFormat->u8FrcMode;
    pstDispFrameFmt->u83DMode = pstDispFrameFormat->u83DMode;
    pstDispFrameFmt->u8BottomFieldFirst = pstDispFrameFormat->u8BottomFieldFirst;
    pstDispFrameFmt->u8FreezeThisFrame = pstDispFrameFormat->u8FreezeThisFrame;
    pstDispFrameFmt->u8ToggleTime = pstDispFrameFormat->u8ToggleTime;
    pstDispFrameFmt->u8MCUMode = pstDispFrameFormat->u8MCUMode;
    pstDispFrameFmt->u8FieldCtrl = pstDispFrameFormat->u8FieldCtrl;
    pstDispFrameFmt->u8ApplicationType = pstDispFrameFormat->u8ApplicationType;
    pstDispFrameFmt->u83DLayout = pstDispFrameFormat->u83DLayout;
    pstDispFrameFmt->u8ColorInXVYCC = pstDispFrameFormat->u8ColorInXVYCC;
    pstDispFrameFmt->u8LowLatencyMode = pstDispFrameFormat->u8LowLatencyMode;
    pstDispFrameFmt->u8VdecComplexity = pstDispFrameFormat->u8VdecComplexity;
    pstDispFrameFmt->u8HTLBTableId = pstDispFrameFormat->u8HTLBTableId;
    pstDispFrameFmt->u8HTLBEntriesSize = pstDispFrameFormat->u8HTLBEntriesSize;
    pstDispFrameFmt->u8AFD = pstDispFrameFormat->u8AFD;

    pstDispFrameFmt->stHDRInfo.u32Version = pstDispFrameFormat->stHDRInfo.u32Version;
    pstDispFrameFmt->stHDRInfo.u32Length = pstDispFrameFormat->stHDRInfo.u32Length;
    pstDispFrameFmt->stHDRInfo.u32FrmInfoExtAvail = pstDispFrameFormat->stHDRInfo.u32FrmInfoExtAvail;

    pstDispFrameFmt->stHDRInfo.stColorDescription.u32Version = pstDispFrameFormat->stHDRInfo.stColorDescription.u32Version;
    pstDispFrameFmt->stHDRInfo.stColorDescription.u32Length = pstDispFrameFormat->stHDRInfo.stColorDescription.u32Length;
    pstDispFrameFmt->stHDRInfo.stColorDescription.u8ColorPrimaries = pstDispFrameFormat->stHDRInfo.stColorDescription.u8ColorPrimaries;
    pstDispFrameFmt->stHDRInfo.stColorDescription.u8TransferCharacteristics = pstDispFrameFormat->stHDRInfo.stColorDescription.u8TransferCharacteristics;
    pstDispFrameFmt->stHDRInfo.stColorDescription.u8MatrixCoefficients = pstDispFrameFormat->stHDRInfo.stColorDescription.u8MatrixCoefficients;

    //pstDispFrameFormat->sHDRInfo.stMasterColorDisplay                                                   //pstDispFrameFormat->sHDRInfo.stMasterColorDisplay

    pstDispFrameFmt->stHDRInfo.stDolbyHDRInfo.u32Version = pstDispFrameFormat->stHDRInfo.stDolbyHDRInfo.u32Version;
    pstDispFrameFmt->stHDRInfo.stDolbyHDRInfo.u32Length = pstDispFrameFormat->stHDRInfo.stDolbyHDRInfo.u32Length;
    pstDispFrameFmt->stHDRInfo.stDolbyHDRInfo.u8DVMode = pstDispFrameFormat->stHDRInfo.stDolbyHDRInfo.u8DVMode;
    pstDispFrameFmt->stHDRInfo.stDolbyHDRInfo.phyHDRMetadataAddr = pstDispFrameFormat->stHDRInfo.stDolbyHDRInfo.phyHDRMetadataAddr;
    pstDispFrameFmt->stHDRInfo.stDolbyHDRInfo.u32HDRMetadataSize = pstDispFrameFormat->stHDRInfo.stDolbyHDRInfo.u32HDRMetadataSize;
    pstDispFrameFmt->stHDRInfo.stDolbyHDRInfo.phyHDRRegAddr = pstDispFrameFormat->stHDRInfo.stDolbyHDRInfo.phyHDRRegAddr;
    pstDispFrameFmt->stHDRInfo.stDolbyHDRInfo.u32HDRRegSize = pstDispFrameFormat->stHDRInfo.stDolbyHDRInfo.u32HDRRegSize;
    pstDispFrameFmt->stHDRInfo.stDolbyHDRInfo.phyHDRLutAddr = pstDispFrameFormat->stHDRInfo.stDolbyHDRInfo.phyHDRLutAddr;
    pstDispFrameFmt->stHDRInfo.stDolbyHDRInfo.u32HDRLutSize = pstDispFrameFormat->stHDRInfo.stDolbyHDRInfo.u32HDRLutSize;
    pstDispFrameFmt->stHDRInfo.stDolbyHDRInfo.u8DMEnable = pstDispFrameFormat->stHDRInfo.stDolbyHDRInfo.u8DMEnable;
    pstDispFrameFmt->stHDRInfo.stDolbyHDRInfo.u8CompEnable = pstDispFrameFormat->stHDRInfo.stDolbyHDRInfo.u8CompEnable;
    pstDispFrameFmt->stHDRInfo.stDolbyHDRInfo.u8CurrentIndex = pstDispFrameFormat->stHDRInfo.stDolbyHDRInfo.u8CurrentIndex;

    //memcpy(&pstDispFrameFmt->stDispFrmInfoExt, &pstDispFrameFormat->stDispFrmInfoExt, sizeof(ST_MMSDK_DMS_DISP_FRM_INFO_EXT));
    // FIXME: frame info EXT V7
    //pstDispFrameFormat->u16MIUBandwidth;
    //pstDispFrameFormat->u16Bitrate;
    pstDispFrameFmt->u32TileMode = pstDispFrameFormat->u32TileMode;
    //ALOGD("u32TileMode = %d", pstDispFrameFormat->u32TileMode);
    //pstDispFrameFormat->phyHTLBEntriesAddr;

    pstDispFrameFmt->phyVsyncBridgeAddr = pstDispFrameFormat->phyVsyncBridgeAddr;
    pstDispFrameFmt->phyVsyncBridgeExtAddr = pstDispFrameFormat->phyVsyncBridgeExtAddr;

    return TRUE;
}

MMSDK_BOOL _PT_Display_ConvertWinInfo(const ST_MMSDK_DMS_SETWIN_INFO *pstMMSDKSetWinInfo, ST_DMS_SETWIN_INFO *pstDMSSetWinInfo)
{
    memset(pstDMSSetWinInfo, 0, sizeof(ST_DMS_SETWIN_INFO));

    pstDMSSetWinInfo->u32Version = pstMMSDKSetWinInfo->u32Version;
    pstDMSSetWinInfo->u32Length  = pstMMSDKSetWinInfo->u32Length;

    pstDMSSetWinInfo->stOutputWinInfo.stCropWin.u32x        = pstMMSDKSetWinInfo->stOutputWinInfo.stCropWin.u32x;
    pstDMSSetWinInfo->stOutputWinInfo.stCropWin.u32y        = pstMMSDKSetWinInfo->stOutputWinInfo.stCropWin.u32y;
    pstDMSSetWinInfo->stOutputWinInfo.stCropWin.u32width    = pstMMSDKSetWinInfo->stOutputWinInfo.stCropWin.u32width;
    pstDMSSetWinInfo->stOutputWinInfo.stCropWin.u32height   = pstMMSDKSetWinInfo->stOutputWinInfo.stCropWin.u32height;

    pstDMSSetWinInfo->stOutputWinInfo.stOutputWin.u32x      = pstMMSDKSetWinInfo->stOutputWinInfo.stOutputWin.u32x;
    pstDMSSetWinInfo->stOutputWinInfo.stOutputWin.u32y      = pstMMSDKSetWinInfo->stOutputWinInfo.stOutputWin.u32y;
    pstDMSSetWinInfo->stOutputWinInfo.stOutputWin.u32width  = pstMMSDKSetWinInfo->stOutputWinInfo.stOutputWin.u32width;
    pstDMSSetWinInfo->stOutputWinInfo.stOutputWin.u32height = pstMMSDKSetWinInfo->stOutputWinInfo.stOutputWin.u32height;
    pstDMSSetWinInfo->u32OnOutputLayer = pstMMSDKSetWinInfo->u32OnOutputLayer;
    pstDMSSetWinInfo->enARC = pstMMSDKSetWinInfo->enARC;

    return TRUE;
}

EN_MMSDK_DMS_RESULT PT_Display_SetDigitalDecodeSignalInfo(PT_DISPLAYITEM DisplayItem, const ST_MMSDK_DMS_DISPFRAMEFORMAT *pstDispFrameFormat)
{
    PT_DISP_DBG("");

    _ST_MMSDK_DISPLAY_INSTANCE* pDisplayInstance = (_ST_MMSDK_DISPLAY_INSTANCE*)DisplayItem;
    MMSDK_U32 u32WindowID = (MMSDK_U32)pDisplayInstance->u8DIPWin;
    ST_DMS_DISPFRAMEFORMAT stDispFrameFmt = {};

    _PT_Display_ConvertFrmFormat_DMS(u32WindowID, pstDispFrameFormat, &stDispFrameFmt);

    return (EN_MMSDK_DMS_RESULT)MApi_DMS_SetDigitalDecodeSignalInfo(u32WindowID, &stDispFrameFmt);
}

EN_MMSDK_DMS_RESULT PT_Display_ClearDigitalDecodeSignalInfo(PT_DISPLAYITEM DisplayItem)
{
    PT_DISP_DBG("");

//    _ST_MMSDK_DISPLAY_INSTANCE* pDisplayInstance = (_ST_MMSDK_DISPLAY_INSTANCE*)DisplayItem;
//    MMSDK_U32 u32WindowID = (MMSDK_U32)pDisplayInstance->u8DIPWin;
//    MApi_DMS_ClearDigitalDecodeSignalInfo(u32WindowID);

    return E_MMSDK_DMS_OK;
}

EN_MMSDK_DMS_RESULT PT_Display_SetWindow(PT_DISPLAYITEM DisplayItem, const ST_MMSDK_DMS_SETWIN_INFO *pstDMS_SetWin_Info)
{
    PT_DISP_DBG("");

    _ST_MMSDK_DISPLAY_INSTANCE* pDisplayInstance = (_ST_MMSDK_DISPLAY_INSTANCE*)DisplayItem;
    MMSDK_U32 u32WindowID = (MMSDK_U32)pDisplayInstance->u8DIPWin;
    ST_DMS_SETWIN_INFO stSetWindowInfo = {};
    if(u32WindowID == E_MSAPI_XC_MAIN_WINDOW && !PT_SYS_GetFlagResizeWindowByUser(u32WindowID))
    {
        PT_DISP_DBG("DS stream trigger ResizeWindow, DO NOT call MApi_DMS_SetWindow");
        return E_MMSDK_DMS_OK;
    }

    PT_SYS_SetFlagResizeWindowByUser(u32WindowID, FALSE);

    _PT_Display_ConvertWinInfo(pstDMS_SetWin_Info, &stSetWindowInfo);

    // [temp solution]    1439893 not fixed, Call PT_Display_SetXCWindow to  change Z-order
    //return (EN_MMSDK_DMS_RESULT)MApi_DMS_SetWindow(u32WindowID, &stSetWindowInfo);

    ST_MMSDK_XC_RECT stCropRect = {}, stDispRect = {};

    stCropRect.u16X         = (MMSDK_U16)stSetWindowInfo.stOutputWinInfo.stCropWin.u32x;
    stCropRect.u16Y         = (MMSDK_U16)stSetWindowInfo.stOutputWinInfo.stCropWin.u32y;
    stCropRect.u16Width     = (MMSDK_U16)stSetWindowInfo.stOutputWinInfo.stCropWin.u32width;
    stCropRect.u16Height    = (MMSDK_U16)stSetWindowInfo.stOutputWinInfo.stCropWin.u32height;

    stDispRect.u16X         = (MMSDK_U16)stSetWindowInfo.stOutputWinInfo.stOutputWin.u32x;
    stDispRect.u16Y         = (MMSDK_U16)stSetWindowInfo.stOutputWinInfo.stOutputWin.u32y;
    stDispRect.u16Width     = (MMSDK_U16)stSetWindowInfo.stOutputWinInfo.stOutputWin.u32width;
    stDispRect.u16Height    = (MMSDK_U16)stSetWindowInfo.stOutputWinInfo.stOutputWin.u32height;

    return (PT_Display_SetXCWindow(DisplayItem, &stCropRect, &stDispRect, 0) == TRUE)? E_MMSDK_DMS_OK : E_MMSDK_DMS_FAIL;

}

EN_MMSDK_DMS_RESULT PT_Display_Video_Flip(PT_DISPLAYITEM DisplayItem, const ST_MMSDK_DMS_DISPFRAMEFORMAT* pstDispFrameFormat)
{
//    PT_DISP_DBG("");
    _ST_MMSDK_DISPLAY_INSTANCE* pDisplayInstance = (_ST_MMSDK_DISPLAY_INSTANCE*)DisplayItem;
    MMSDK_U32 u32WindowID = (MMSDK_U32)pDisplayInstance->u8DIPWin;
    MSAPI_XC_VDECFRAME_INFO VDECDispFrame={};
    _PT_Display_ConvertFrmFormat(u32WindowID, pstDispFrameFormat, &VDECDispFrame);

    MMSDK_BOOL bStepDbg = FALSE;

    PT_SYS_GetOptions(u32WindowID, E_PT_SYS_STEP_DBG, (void*)&bStepDbg);

    if(bStepDbg == TRUE && pDisplayInstance->bIsFramePushed == TRUE)
    {
        MMSDK_BOOL bStepNextFrame = FALSE;
        do
        {
            PT_SYS_GetOptions(u32WindowID, E_PT_SYS_STEP_NEXTFRAME, (void*)&bStepNextFrame);
            MsOS_DelayTask(50);
        }while(!bStepNextFrame);

        bStepDbg = FALSE;
        PT_SYS_SetOptions(u32WindowID, E_PT_SYS_STEP_NEXTFRAME, &bStepDbg);
        PT_DISP_DBG("disp cnt=%d", pDisplayInstance->u32DispCount);
    }

    if (E_MSAPI_XC_OK == msAPI_XC_DIPMultiView_PushFrame(&VDECDispFrame))
    {
        pDisplayInstance->bIsFramePushed = TRUE;
        pDisplayInstance->u32DispCount++;
        //PT_DISP_DBG("disp cnt=%d", pDisplayInstance->u32DispCount);

        return E_MMSDK_DMS_OK;
    }
    else
    {
        return E_MMSDK_DMS_FAIL;
    }
}

EN_MMSDK_DMS_RESULT PT_Display_SetZOrder(PT_DISPLAYITEM DisplayItem, const ST_MMSDK_DMS_ZORDER_INFO *pstZorderInfo)
{
    // AVP SN flow does not use this API
    return E_MMSDK_DMS_OK;
}

EN_MMSDK_DMS_RESULT PT_Display_GetCapability(PT_DISPLAYITEM DisplayItem, EN_MMSDK_DMS_CAPABILITY *peCapability)
{
    PT_DISP_DBG("");
    EN_DMS_CAPABILITY eDMSCapability;
    MApi_DMS_GetCapability(&eDMSCapability);
    *peCapability = (EN_MMSDK_DMS_CAPABILITY)eDMSCapability;
    //*peCapability |= E_MMSDK_DMS_CAPABILITY_MULTI_WINDOW;

    return E_MMSDK_DMS_OK;
}

EN_MMSDK_DMS_RESULT PT_Display_SetOutputLayer(PT_DISPLAYITEM DisplayItem, const ST_MMSDK_DMS_WINDOW *pstLayer)
{
    // AVP SN flow does not use this API
    return E_MMSDK_DMS_OK;
}

EN_MMSDK_DMS_RESULT PT_Display_Video_Freeze(PT_DISPLAYITEM DisplayItem, const ST_MMSDK_DMS_FREEZE_WINDOW_INFO *pstWindowFreeze)
{
    // AVP SN flow does not use this API
    return E_MMSDK_DMS_OK;
}
#endif
#endif
