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
#if (MMSDK_DMS_ENABLE == 1)
#include "apiDMS.h"
#endif
#include "apiGOP.h"
#include "Panel.h"
#include "drvTVEncoder.h"

#include "msAPI_XC_EX.h"
#include "msAPI_VE.h"
#include "drvPQ.h"
#include "drvDTC.h"

#define FLOW(fmt, arg...)         //printf("\033[1;35m######[%s]###### "fmt" \033[0m\n",__FUNCTION__,##arg)
#define FLOWDISP(fmt, arg...)         //printf("\033[1;34m######[%s]###### "fmt" \033[0m\n",__FUNCTION__,##arg)
#define FLOWDISPDS(fmt, arg...)         //printf("\033[1;34m######[%s]###### "fmt" \033[0m\n",__FUNCTION__,##arg)

#if (MMSDK_DISPLAY_DUALXC_PORTING == 1)
MSAPI_XC_DEVICE_ID g_stXC_PT_DeviceId[2] = {{0, E_MSAPI_XC_DEVICE0}, {0, E_MSAPI_XC_DEVICE1}};
#else
MSAPI_XC_DEVICE_ID g_stXC_PT_DeviceId = {0, E_MSAPI_XC_DEVICE0};
#endif
extern PT_SYS_CUS_FunPtr gstSysCusFunPtr;

#ifdef AVP_ENABLE
MSAPI_XC_DEVICE_ID g_stDIP_PT_DeviceId = {0, E_MSAPI_XC_DEVICE_DIP_0};
static MS_U8 u8Display_Instance_BitMask = 0;
static MS_U8 u8DIP_Window_BitMask = 0;
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
#ifdef AVP_ENABLE
    MS_U8 u8ID_BitMask;
    MS_U8 u8DIPWin;
    MS_U8 u8Flow;
    MMSDK_BOOL bIsFramePushed;
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
static MS_U8 _gu8HDRCtrl = 0;// 0 is auto, 1 is force HDR to SDR, 2is force bypass
#endif
MS_U32 _gu32HDRMetadataType = E_MMSDK_HDR_TYPE_CTL_FLAG_NONE;
#define HDR_MM_DEV
#define STREAM_IS_HDR (_gu32HDRMetadataType != E_MMSDK_HDR_TYPE_CTL_FLAG_NONE)
#define STREAM_HDR_TYPE_TCH (_gu32HDRMetadataType & E_MMSDK_HDR_TYPE_CTL_FLAG_TCH)
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

MS_BOOL FireCfd(void)
{
    XC_ApiStatus stXcStatus;
    memset(&stXcStatus, 0, sizeof(XC_ApiStatus));
    MApi_XC_GetStatus(&stXcStatus, MAIN_WINDOW);

    XC_CFD_FIRE stCfdFire;
    stCfdFire.u32Version = CFD_FIRE_VERSION;
    stCfdFire.u16Length = sizeof(XC_CFD_FIRE);
    stCfdFire.u8Win = 0;
    stCfdFire.u8InputSource = (MS_U8)PQ_INPUT_SOURCE_STORAGE;
    stCfdFire.u8UpdateType = E_XC_CFD_UPDATE_TYPE_ALL;
    stCfdFire.bIsRgbBypass = stXcStatus.bForceRGBin;
    stCfdFire.bIsHdMode = TRUE;
    return SendCfd(E_XC_CFD_CTRL_TYPE_FIRE, &stCfdFire, sizeof(XC_CFD_FIRE));
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
    else
        stCfdHdr.u8HdrType =  E_XC_CFD_HDR_TYPE_OPEN;
    MApi_XC_DLC_SetDlcHandlerOnOff(TRUE);
    SendCfd(E_XC_CFD_CTRL_TYPE_HDR, &stCfdHdr, sizeof(XC_CFD_HDR));

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

/// Following define reference CEA-861-E.pdf & CEA-861.3_V16BallotDraft.pdf.
/// YCbCr(YUV) 444 mask define in CEA extend header.
#define EDID_CEA_HEADER_YUV444_MASK 0x20
/// YCbCr(YUV) 422 mask define in CEA extend header.
#define EDID_CEA_HEADER_YUV422_MASK 0x10
/// CEA tag code mask
#define EDID_CEA_TAG_CODE_MASK 0xE0
/// CEA length mask
#define EDID_CEA_TAG_LENGTH_MASK 0x1F
/// YCC Quantization Range at video capability data block in EDID.
#define VIDEO_CAPABILITY_DB_QY_MASK 0x80
/// RGB Quantization Range at video capability data block in EDID.
#define VIDEO_CAPABILITY_DB_QS_MASK 0x40
/// EOEF at HDR data block in EDID.
#define HDR_DB_EOTF_MASK 0x3F
//EDID Block Size
#define EDID_BLOCK_SIZE     (128)
//EDID Extend Block Size
#define EDID_EXT_BLOCK_SIZE (128)
/// Get CEA tag code
#define EDID_CEA_TAG_CODE(x) (((x)&EDID_CEA_TAG_CODE_MASK)>>5)
/// Get CEA tag length
#define EDID_CEA_TAG_LENGTH(x) ((x)&EDID_CEA_TAG_LENGTH_MASK)
/// Get EOTF in HDR data block
#define HDR_DB_EOTF(x) ((x)&HDR_DB_EOTF_MASK)
/// Get Rx in EDID
#define COLOR_CHARACTERISTICS_RX(x, y) (((x)<<2) + (((y)&0xC0)>>6))
/// Get Ry in EDID
#define COLOR_CHARACTERISTICS_RY(x, y) (((x)<<2) + (((y)&0x30)>>4))
/// Get Gx in EDID
#define COLOR_CHARACTERISTICS_GX(x, y) (((x)<<2) + (((y)&0x0C)>>2))
/// Get Gy in EDID
#define COLOR_CHARACTERISTICS_GY(x, y) (((x)<<2) + ((y)&0x03))
/// Get Bx in EDID
#define COLOR_CHARACTERISTICS_BX(x, y) (((x)<<2) + (((y)&0xC0)>>6))
/// Get By in EDID
#define COLOR_CHARACTERISTICS_BY(x, y) (((x)<<2) + (((y)&0x30)>>4))
/// Get Wx in EDID
#define COLOR_CHARACTERISTICS_WX(x, y) (((x)<<2) + (((y)&0x0C)>>2))
/// Get Wy in EDID
#define COLOR_CHARACTERISTICS_WY(x, y) (((x)<<2) + ((y)&0x03))
/// CFD EDID YUV444 support mask. This define is used in ST_MAPI_CFD_EDID -> u8HDMISinkSupportYUVFormat.
#define EDID_YUV444_SUPPORT_MASK 0x01
/// CFD EDID YUV422 support mask. This define is used in ST_MAPI_CFD_EDID -> u8HDMISinkSupportYUVFormat.
#define EDID_YUV422_SUPPORT_MASK 0x02
/// CFD EDID YUV420 support mask. This define is used in ST_MAPI_CFD_EDID -> u8HDMISinkSupportYUVFormat.
#define EDID_YUV420_SUPPORT_MASK 0x04
/// CFD EDID QY support mask. This define is used in ST_MAPI_CFD_EDID -> u8HDMISinkSupportYUVFormat.
#define EDID_QY_SUPPORT_MASK 0x10
/// CFD EDID QS support mask. This define is used in ST_MAPI_CFD_EDID -> u8HDMISinkSupportYUVFormat.
#define EDID_QS_SUPPORT_MASK 0x08
//STB HDR Bypass
#define HDMITX_HDMITX_INFOFRAME_DYNAMIC_RANGE (7)
#define HDMITX_HDMITX_INFOFRAME_DYNAMIC_RANGE_VERSION (1)
#define HDMITX_HDMITX_INFOFRAME_DYNAMIC_RANGE_PACKET_LENGHT (26)
#define HDMITX_HDMITX_INFOFRAME_DYNAMIC_RANGE_ARRAY_SIZE (3+HDMITX_HDMITX_INFOFRAME_DYNAMIC_RANGE_PACKET_LENGHT)
#define LOW_BYTE(x) ((x)&(0xFF))
#define HIGH_BYTE(x) (((x)&(0xFF00)) >> 8)

MS_BOOL SetSinkEdidCfd(void)
{
    /// DEID documents: CEA-861-E.pdf, CEA-861.3_V16BallotDraft.pdf
    MS_U8 u8EdidRawData[EDID_BLOCK_SIZE+EDID_EXT_BLOCK_SIZE];
    MS_BOOL bRet = FALSE;
    memset(u8EdidRawData, 0, sizeof(u8EdidRawData));

    bRet = MApi_HDMITx_GetEDIDData(u8EdidRawData, FALSE);
    bRet = bRet & MApi_HDMITx_GetEDIDData(&u8EdidRawData[EDID_BLOCK_SIZE], TRUE);
    if (bRet == TRUE)
    {
        XC_CFD_EDID stCfdEdid;
        memset(&stCfdEdid, 0, sizeof(XC_CFD_EDID));

        stCfdEdid.u32Version = CFD_EDID_VERSION;
        stCfdEdid.u16Length = sizeof(XC_CFD_EDID);

        MS_U16 u16Offset = 0;

        /// 0x00~0x7F bytes
        #define EDID_HEADER_VERSION_OFFSET 18
        #define EDID_HEADER_RED_GREEN_OFFSET 25

        stCfdEdid.u8HDMISinkEDIDBaseBlockVersion = u8EdidRawData[EDID_HEADER_VERSION_OFFSET]; //version
        stCfdEdid.u8HDMISinkEDIDBaseBlockReversion = u8EdidRawData[EDID_HEADER_VERSION_OFFSET+1]; //Revision;
        stCfdEdid.u16Display_Primaries_x[0] = COLOR_CHARACTERISTICS_RX(u8EdidRawData[EDID_HEADER_RED_GREEN_OFFSET+2], u8EdidRawData[EDID_HEADER_RED_GREEN_OFFSET]);
        stCfdEdid.u16Display_Primaries_x[1] = COLOR_CHARACTERISTICS_GX(u8EdidRawData[EDID_HEADER_RED_GREEN_OFFSET+4], u8EdidRawData[EDID_HEADER_RED_GREEN_OFFSET]);
        stCfdEdid.u16Display_Primaries_x[2] = COLOR_CHARACTERISTICS_BX(u8EdidRawData[EDID_HEADER_RED_GREEN_OFFSET+6], u8EdidRawData[EDID_HEADER_RED_GREEN_OFFSET+1]);
        stCfdEdid.u16Display_Primaries_y[0] = COLOR_CHARACTERISTICS_RY(u8EdidRawData[EDID_HEADER_RED_GREEN_OFFSET+3], u8EdidRawData[EDID_HEADER_RED_GREEN_OFFSET]);
        stCfdEdid.u16Display_Primaries_y[1] = COLOR_CHARACTERISTICS_GY(u8EdidRawData[EDID_HEADER_RED_GREEN_OFFSET+5], u8EdidRawData[EDID_HEADER_RED_GREEN_OFFSET]);
        stCfdEdid.u16Display_Primaries_y[2] = COLOR_CHARACTERISTICS_BY(u8EdidRawData[EDID_HEADER_RED_GREEN_OFFSET+7], u8EdidRawData[EDID_HEADER_RED_GREEN_OFFSET+1]);
        stCfdEdid.u16White_point_x = COLOR_CHARACTERISTICS_WX(u8EdidRawData[EDID_HEADER_RED_GREEN_OFFSET+8], u8EdidRawData[EDID_HEADER_RED_GREEN_OFFSET+1]);
        stCfdEdid.u16White_point_y = COLOR_CHARACTERISTICS_WY(u8EdidRawData[EDID_HEADER_RED_GREEN_OFFSET+9], u8EdidRawData[EDID_HEADER_RED_GREEN_OFFSET+1]);
        stCfdEdid.u8HDMISinkEDIDValid = 1;

        /// 0x80~0x83
        // Parse extend
        u16Offset += 128;

        stCfdEdid.u8HDMISinkEDIDCEABlockReversion = u8EdidRawData[u16Offset+1];
        if (u8EdidRawData[u16Offset+3]&EDID_CEA_HEADER_YUV444_MASK)
        {
            /// YUV444
            stCfdEdid.u8HDMISinkSupportYUVFormat |= EDID_YUV444_SUPPORT_MASK;
        }

        if (u8EdidRawData[u16Offset+3]&EDID_CEA_HEADER_YUV422_MASK)
        {
            /// YUV422
            stCfdEdid.u8HDMISinkSupportYUVFormat |= EDID_YUV422_SUPPORT_MASK;
        }
        MS_U8 u8ByteOffset = u8EdidRawData[u16Offset+2];
        ////////////m_bSendHDRinfoFrame = MAPI_FALSE;
        u16Offset += 4;

        while ((u16Offset - 128) < u8ByteOffset)
        {
            MS_U8 u8TagCode = EDID_CEA_TAG_CODE((*(u8EdidRawData+u16Offset)));
            MS_U8 u8Length = EDID_CEA_TAG_LENGTH((*(u8EdidRawData+u16Offset)));
            u16Offset += 1;
            if(u8TagCode == 7) //CEA_EXTENDED_TAG_DATA_BLOCK
            {
                MS_U16 u16ExtendOffset = 0;

                MS_U8 u8ExtendedTagCode = *(u8EdidRawData+u16Offset+u16ExtendOffset);
                u16ExtendOffset++;

                if (u8ExtendedTagCode == 0)//CEA_VIDEO_CAPABILITY_DATA_BLOCK
                {
                    // Video capability data block
                    stCfdEdid.u8HDMISinkVCDBValid = 1;
                    MS_U8 u8VideoCapability = *(u8EdidRawData+u16Offset+u16ExtendOffset);
                    u16ExtendOffset++;

                    if (u8VideoCapability&VIDEO_CAPABILITY_DB_QY_MASK)
                    {
                        // QY
                        stCfdEdid.u8HDMISinkSupportYUVFormat |= EDID_QY_SUPPORT_MASK;
                    }

                    if (u8VideoCapability&VIDEO_CAPABILITY_DB_QS_MASK)
                    {
                        // QS
                        stCfdEdid.u8HDMISinkSupportYUVFormat |= EDID_QS_SUPPORT_MASK;
                    }
                }
                else if (u8ExtendedTagCode == 5)//CEA_COLORIMETRY_DATA_BLOCK
                {
                    // Colorimetry data block
                    MS_U8 u8ColorimetryData = *(u8EdidRawData+u16Offset+u16ExtendOffset);
                    u16ExtendOffset++;

                    stCfdEdid.u8HDMISinkExtendedColorspace = u8ColorimetryData;
                }
                else if (u8ExtendedTagCode == 6)//CEA_HDR_STATIC_METADATA_DATA_BLOCK
                {
                    //////////m_bSendHDRinfoFrame = MAPI_TRUE;
                    MS_U8 u8Eotf = *(u8EdidRawData+u16Offset+u16ExtendOffset);
                    u16ExtendOffset++;

                    MS_U8 u8StaticMetadata = *(u8EdidRawData+u16Offset+u16ExtendOffset);
                    u16ExtendOffset++;

                    MS_U8 u8ContentMaxLuminance = 0;
                    MS_U8 u8ContentMaxFrameAvgLuminance = 0;
                    MS_U8 u8ContentMinLuminance = 0;


                    if (u16ExtendOffset < u8Length)
                    {
                        u8ContentMaxLuminance = *(u8EdidRawData+u16Offset+u16ExtendOffset);
                        u16ExtendOffset++;
                    }

                    if (u16ExtendOffset < u8Length)
                    {
                        u8ContentMaxFrameAvgLuminance = *(u8EdidRawData+u16Offset+u16ExtendOffset);
                        u16ExtendOffset++;
                    }

                    if (u16ExtendOffset < u8Length)
                    {
                        u8ContentMinLuminance = *(u8EdidRawData+u16Offset+u16ExtendOffset);
                        u16ExtendOffset++;
                    }

                    stCfdEdid.u8HDMISinkHDRDataBlockValid = 1;
                    stCfdEdid.u8HDMISinkEOTF = HDR_DB_EOTF(u8Eotf);
                    stCfdEdid.u8HDMISinkSM = u8StaticMetadata;
                    stCfdEdid.u8HDMISinkHDRDataBlockLength = u8Length;
                    stCfdEdid.u8HDMISinkDesiredContentMaxLuminance = u8ContentMaxLuminance;           //need a LUT to transfer
                    stCfdEdid.u8HDMISinkDesiredContentMaxFrameAvgLuminance = u8ContentMaxFrameAvgLuminance; //need a LUT to transfer
                    stCfdEdid.u8HDMISinkDesiredContentMinLuminance = u8ContentMinLuminance;           //need a LUT to transfer

                }
                else if (u8ExtendedTagCode == 14)//CEA_YCBCR420_VIDEO_DATA_BLOCK
                {
                    /// YCbCr 4:2:0  Video data block
                    /// YUV420
                    stCfdEdid.u8HDMISinkSupportYUVFormat |= EDID_YUV420_SUPPORT_MASK;
                }
                else if (u8ExtendedTagCode == 15)//CEA_YCBCR420_CAPABILITY_MAP_DATA_BLOCK)
                {
                    /// YCbCr 4:2:0  Capability Map Data Block
                    /// YUV420
                    stCfdEdid.u8HDMISinkSupportYUVFormat |= EDID_YUV420_SUPPORT_MASK;
                }
            }
            u16Offset += u8Length;

        }

        XC_CFD_CONTROL_INFO stXCCFDControlInfo;
        memset(&stXCCFDControlInfo, 0, sizeof(XC_CFD_CONTROL_INFO));
        stXCCFDControlInfo.enCtrlType = E_XC_CFD_CTRL_TYPE_EDID;
        stXCCFDControlInfo.pParam = &stCfdEdid;
        stXCCFDControlInfo.u32ParamLen = sizeof(XC_CFD_EDID);
        if (E_APIXC_RET_OK != MApi_XC_HDR_Control(E_XC_HDR_CTRL_CFD_CONTROL,&stXCCFDControlInfo))
        {
            printf("MApi_XC_HDR_Control failed\n");
            return FALSE;
        }
        return TRUE;
    }
    printf("Get TX EDID failed\n");
    return FALSE;
}

MS_BOOL SendTxHDRInfo(MS_BOOL bSendHDRInfoFrame)
{
    //1. Get CFD Status
    XC_CFD_HDMI stCfdHdmi;
    memset(&stCfdHdmi, 0, sizeof(XC_CFD_HDMI));
    XC_CFD_CONTROL_INFO stXCCFDControlInfo;
    memset(&stXCCFDControlInfo, 0, sizeof(XC_CFD_CONTROL_INFO));
    stXCCFDControlInfo.enCtrlType = E_XC_CFD_CTRL_GET_HDMI_STATUS;
    stXCCFDControlInfo.pParam = &stCfdHdmi;
    stXCCFDControlInfo.u32ParamLen = sizeof(XC_CFD_HDMI);
    MApi_XC_HDR_Control(E_XC_HDR_CTRL_CFD_CONTROL,&stXCCFDControlInfo);

    //2. Set HDR InfoFrame
    MS_U8 u8HDRInfoFrame[HDMITX_HDMITX_INFOFRAME_DYNAMIC_RANGE_ARRAY_SIZE];
    if(bSendHDRInfoFrame == TRUE)
    {
        memset(u8HDRInfoFrame, 0, sizeof(u8HDRInfoFrame));

        u8HDRInfoFrame[0] = HDMITX_HDMITX_INFOFRAME_DYNAMIC_RANGE;                //InfoFrame type
        u8HDRInfoFrame[1] = HDMITX_HDMITX_INFOFRAME_DYNAMIC_RANGE_VERSION;        //Version
        u8HDRInfoFrame[2] = HDMITX_HDMITX_INFOFRAME_DYNAMIC_RANGE_PACKET_LENGHT;  //Length of the following HDR InfoFrame
        u8HDRInfoFrame[3] = stCfdHdmi.u8EOTF;
        u8HDRInfoFrame[4] = stCfdHdmi.u8SMDID;
        u8HDRInfoFrame[5] = (MS_U8)LOW_BYTE(stCfdHdmi.u16Display_Primaries_x[0]);
        u8HDRInfoFrame[6] = (MS_U8)HIGH_BYTE(stCfdHdmi.u16Display_Primaries_x[0]);
        u8HDRInfoFrame[7] = (MS_U8)LOW_BYTE(stCfdHdmi.u16Display_Primaries_y[0]);
        u8HDRInfoFrame[8] = (MS_U8)HIGH_BYTE(stCfdHdmi.u16Display_Primaries_y[0]);
        u8HDRInfoFrame[9] = (MS_U8)LOW_BYTE(stCfdHdmi.u16Display_Primaries_x[1]);
        u8HDRInfoFrame[10] = (MS_U8)HIGH_BYTE(stCfdHdmi.u16Display_Primaries_x[1]);
        u8HDRInfoFrame[11] = (MS_U8)LOW_BYTE(stCfdHdmi.u16Display_Primaries_y[1]);
        u8HDRInfoFrame[12] = (MS_U8)HIGH_BYTE(stCfdHdmi.u16Display_Primaries_y[1]);
        u8HDRInfoFrame[13] = (MS_U8)LOW_BYTE(stCfdHdmi.u16Display_Primaries_x[2]);
        u8HDRInfoFrame[14] = (MS_U8)HIGH_BYTE(stCfdHdmi.u16Display_Primaries_x[2]);
        u8HDRInfoFrame[15] = (MS_U8)LOW_BYTE(stCfdHdmi.u16Display_Primaries_y[2]);
        u8HDRInfoFrame[16] = (MS_U8)HIGH_BYTE(stCfdHdmi.u16Display_Primaries_y[2]);
        u8HDRInfoFrame[17] = (MS_U8)LOW_BYTE(stCfdHdmi.u16White_point_x);
        u8HDRInfoFrame[18] = (MS_U8)HIGH_BYTE(stCfdHdmi.u16White_point_x);
        u8HDRInfoFrame[19] = (MS_U8)LOW_BYTE(stCfdHdmi.u16White_point_y);
        u8HDRInfoFrame[20] = (MS_U8)HIGH_BYTE(stCfdHdmi.u16White_point_y);
        u8HDRInfoFrame[21] = (MS_U8)LOW_BYTE(stCfdHdmi.u16MasterPanelMaxLuminance);
        u8HDRInfoFrame[22] = (MS_U8)HIGH_BYTE(stCfdHdmi.u16MasterPanelMaxLuminance);
        u8HDRInfoFrame[23] = (MS_U8)LOW_BYTE(stCfdHdmi.u16MasterPanelMinLuminance);
        u8HDRInfoFrame[24] = (MS_U8)HIGH_BYTE(stCfdHdmi.u16MasterPanelMinLuminance);
        u8HDRInfoFrame[25] = (MS_U8)LOW_BYTE(stCfdHdmi.u16MaxContentLightLevel);
        u8HDRInfoFrame[26] = (MS_U8)HIGH_BYTE(stCfdHdmi.u16MaxContentLightLevel);
        u8HDRInfoFrame[27] = (MS_U8)LOW_BYTE(stCfdHdmi.u16MaxFrameAvgLightLevel);
        u8HDRInfoFrame[28] = (MS_U8)HIGH_BYTE(stCfdHdmi.u16MaxFrameAvgLightLevel);

        MApi_HDMITx_PKT_Content_Define(HDMITX_HDR_INFOFRMAE, u8HDRInfoFrame, sizeof(u8HDRInfoFrame));
        MApi_HDMITx_PKT_User_Define(HDMITX_HDR_INFOFRMAE, TRUE, HDMITX_CYCLIC_PACKET, 1);

        printf("HDMITX_CYCLIC_PACKET:\n");
        printf("0x%02x 0x%02x 0x%02x 0x%02x 0x%02x\n", u8HDRInfoFrame[0], u8HDRInfoFrame[1], u8HDRInfoFrame[2], u8HDRInfoFrame[3], u8HDRInfoFrame[4]);
        printf("0x%02x 0x%02x 0x%02x 0x%02x 0x%02x\n", u8HDRInfoFrame[5], u8HDRInfoFrame[6], u8HDRInfoFrame[7], u8HDRInfoFrame[8], u8HDRInfoFrame[9]);
        printf("0x%02x 0x%02x 0x%02x 0x%02x 0x%02x\n", u8HDRInfoFrame[10], u8HDRInfoFrame[11], u8HDRInfoFrame[12], u8HDRInfoFrame[13], u8HDRInfoFrame[14]);
        printf("0x%02x 0x%02x 0x%02x 0x%02x 0x%02x\n", u8HDRInfoFrame[15], u8HDRInfoFrame[16], u8HDRInfoFrame[17], u8HDRInfoFrame[18], u8HDRInfoFrame[19]);
        printf("0x%02x 0x%02x 0x%02x 0x%02x 0x%02x\n", u8HDRInfoFrame[20], u8HDRInfoFrame[21], u8HDRInfoFrame[22], u8HDRInfoFrame[23], u8HDRInfoFrame[24]);
        printf("0x%02x 0x%02x 0x%02x 0x%02x\n", u8HDRInfoFrame[25], u8HDRInfoFrame[26], u8HDRInfoFrame[27], u8HDRInfoFrame[28]);
    }
    else
    {
        MApi_HDMITx_PKT_User_Define(HDMITX_HDR_INFOFRMAE, FALSE, HDMITX_STOP_PACKET, 1);
    }

    //3. Set AVI InfoFrame
    MApi_HDMITx_SetAVIInfoExtColorimetry((HDMITX_AVI_EXTENDED_COLORIMETRY)stCfdHdmi.u8ExtendedColorimetry, (HDMITX_AVI_YCC_QUANT_RANGE)stCfdHdmi.u8YccQuantizationRange);
    printf("SetAVIInfoExtColorimetry(%d, %d)\n", stCfdHdmi.u8ExtendedColorimetry, stCfdHdmi.u8YccQuantizationRange);
    MApi_HDMITx_PKT_User_Define(HDMITX_AVI_INFOFRAME, FALSE, HDMITX_CYCLIC_PACKET, 1);
    return TRUE;
}
#endif

static void _PT_Display_SetHDRInit(void)
{
#if (MI_ENABLE == 0)
    if(STREAM_IS_HDR)
    {
#if (MMSDK_HDR_MODE == 1)
        FLOW("Set HDR Init\n");
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
#endif
}

static void _PT_Display_CFDInit(void)
{
#if (MI_ENABLE == 0)
    if(STREAM_IS_HDR)
    {
        FLOW("Init CFD\n");
        SetSinkEdidCfd();

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
        FLOW("Not HDR Stream\n");
        XC_CFD_HDR stCfdHdr;
        memset(&stCfdHdr, 0, sizeof(XC_CFD_HDR));
        stCfdHdr.u32Version = CFD_HDR_VERSION;
        stCfdHdr.u16Length = sizeof(XC_CFD_HDR);
        stCfdHdr.u8Win = 0;
        stCfdHdr.u8HdrType = E_XC_CFD_HDR_TYPE_NONE;
        SendCfd(E_XC_CFD_CTRL_TYPE_HDR, &stCfdHdr, sizeof(XC_CFD_HDR));

        SetSinkEdidCfd();

        FireCfd();
    }
#endif
}

static void _PT_Display_EnableFrameBufferLess(void)
{
#if (MI_ENABLE == 0)
    if(STREAM_IS_HDR)
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
#endif
    pDispInstance->bFirstTimeSetWindow = TRUE;
    pDispInstance->eScalerWin = MAIN_WINDOW;
    pDispInstance->bIsAvpFlow = FALSE;
    pDispInstance->bISSet3D = FALSE;
    pDispInstance->bIsMM3D = FALSE;
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
        FLOW("No HDMITx 3D mode mapping for XC 3D Mode: %d\n", eXC3DMode);
        break;
    }
    return ret;
}
#endif

void PT_Display_InitializeDisplayPath(PT_DISPLAYITEM DisplayItem)
{
    FLOW("");
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
    FLOW("");

    if (DisplayItem == NULL)
    {
        FLOW("DisplayItem is NULL\n");
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

#if (MMSDK_DMS_ENABLE == 1)
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
    FLOW("");
    //[River]
    //It seems HB disable video mute by self.
    //msAPI_XC_SetVideoMute(TRUE,MAIN_WINDOW);
    // [River]
    // for HB, do not delete display path now?
}


MMSDK_BOOL PT_Display_RegisterCallback(PT_DISPLAYITEM DisplayItem, void * pClass, pfnDisplayCallback pCallback)
{
    FLOW("");
    //g_pfnDisplayCallback = pCallback;
    //g_pDislpayClass = pClass;

    if (DisplayItem == NULL)
    {
        FLOW("DisplayItem is NULL\n");
        return FALSE;
    }
    _ST_MMSDK_DISPLAY_INSTANCE* pDisplayInstance = (_ST_MMSDK_DISPLAY_INSTANCE*)DisplayItem;
    pDisplayInstance->pfnDisplayCallback = pCallback;
    pDisplayInstance->pDislpayClass = pClass;
    return TRUE;
}

MMSDK_BOOL PT_Display_Initialize(PT_DISPLAYITEM* pDisplayItem)
{
    FLOW("");
    if (pDisplayItem == NULL)
    {
        FLOW("pDisplayItem is NULL\n");
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
    FLOW("");
    if ((pDisplayItem == NULL) || (*pDisplayItem == NULL))
    {
        FLOW("pDisplayItem is NULL\n");
        return FALSE;
    }

    if(NULL != gstSysCusFunPtr.pDispFinalize)
    {
        gstSysCusFunPtr.pDispFinalize(NULL);
    }

#ifdef HDR_MM_DEV
#if (MI_ENABLE == 0)
    if(STREAM_IS_HDR)
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

    PT_Display_CloseWindow(*pDisplayItem);
    //Clean ID BitMask
    u8Display_Instance_BitMask &= ~( pDisplayInstance->u8ID_BitMask);

    //Clean DIPWIN BitMask
    if((pDisplayInstance->u8Flow == DIP_FLOW) && (pDisplayInstance->u8DIPWin != DIP_Win_NonUse))
    {
#if defined(MMSDK_MultiWin_MODE) && (MMSDK_MultiWin_MODE == 0)
#else
        //Finalize DIP
        msAPI_XC_DIPMultiView_Finalize(pDisplayInstance->u8DIPWin);
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
    if(STREAM_IS_HDR)
    {
        printf("PT_Display_Finalize End CFD fire!!!!!!\n");
        FireCfd();
        SendTxHDRInfo(FALSE);
    }
#endif
#endif
    return TRUE;
}

MMSDK_BOOL PT_Display_BlackScreen(PT_DISPLAYITEM DisplayItem, MMSDK_BOOL bOnOff)
{
    FLOW("Enable:%d", bOnOff);
    if (DisplayItem == NULL)
    {
        FLOW("DisplayItem is NULL\n");
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

    if(bOnOff == FALSE && PT_SYS_GetMMPhotoPath() == 0 && enMediaType == E_MMSDK_MEDIA_TYPE_PHOTO)
        return FALSE;

#ifdef HDR_MM_DEV
#if (MI_ENABLE == 0)
    if(bOnOff == FALSE && _gbFireHDR == TRUE && STREAM_IS_HDR)
    {
        MS_U8* aa = (MS_U8*)MsOS_PA2KSEG1((MS_PHY)PT_SYS_GetMmapInfo(NULL, E_MMSDK_BUF_DV_XC, E_MMSDK_BUF_INFO_PHY_ADDR));
        printf("####### CFD SHM : 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x\n",
            *aa,*(aa+1),*(aa+2),*(aa+3),*(aa+4),*(aa+5),*(aa+6),*(aa+7),*(aa+8),*(aa+9));
        MS_U32 value = 0;
        value |= _gu8HDRCtrl;
        SendCfd(E_XC_CFD_CTRL_SET_HDR_ONOFF_SETTING, &value, sizeof(MS_U32));
        FireCfd();
        _gbFireHDR = FALSE;
        MsOS_DelayTask(100);
        SendTxHDRInfo(TRUE);
    }
    else
    {
        SendTxHDRInfo(FALSE);
    }
#endif
#endif

#if (MMSDK_DMS_ENABLE == 1)
    if(enMediaType == E_MMSDK_MEDIA_TYPE_MOVIE)
    {
        E_MSAPI_DIP_MULTIVIEW_MUTE_ACTION eMultiViewMuteAct = (bOnOff == TRUE) ? E_MSAPI_DIP_MULTIVIEW_MUTE_ON : E_MSAPI_DIP_MULTIVIEW_MUTE_OFF;
        msAPI_XC_DIPMultiView_SetMute(pDisplayInstance->u8DIPWin, eMultiViewMuteAct);
    }
    else if (enMediaType == E_MMSDK_MEDIA_TYPE_PHOTO)
#endif
    {
#if (MMSDK_DISPLAY_DUALXC_PORTING == 1)
        msAPI_XC_SetVideoMuteByMode_EX(&g_stXC_PT_DeviceId[0], bOnOff, E_MSAPI_XC_MAIN_WINDOW, E_MSAPI_XC_VIDEO_MUTE_MODE_AUTO_SYNC);
        //Don't mute device 1 because of VE OSD can't be muted in OPtoVE mode
#else
        //For DDI flow, mute does not delay task or wait
        msAPI_XC_SetVideoMuteByMode_EX(&g_stXC_PT_DeviceId, bOnOff, E_MSAPI_XC_MAIN_WINDOW, E_MSAPI_XC_VIDEO_MUTE_MODE_AUTO_SYNC);
#endif
    }
    return TRUE;
}


MMSDK_BOOL PT_Display_FreezeImage(PT_DISPLAYITEM DisplayItem, MMSDK_BOOL bOnOff)
{
    FLOW("OnOff=%d", bOnOff);

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
    FLOW("[ (bEnableDS=%d)]", bEnableDS);
    if (DisplayItem == NULL)
    {
        FLOW("DisplayItem is NULL\n");
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
            printf("Failed to Set Display Parameters!!\n");
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
        printf("\n DS:IP on %d, OP on %d, depth=%d, Addr=0x%"DTC_MS_PHY_x", MIU=%d\n", stDS_Info.bIPM_DS_On, stDS_Info.bOP_DS_On, stDS_Info.u8DS_Index_Depth,
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

    FLOWDISPDS("DS_SendXCStatus2Firmware %"DTC_MS_U32_x"\n", u32FM_Buf_Base);

    // direct write to shared memory with firmware
    //pstMMDS_Status = (ST_MM_DS_XC_STATUS *) MsOS_PA2KSEG1(u32FM_Buf_Base);
    pstMMDS_Status = (ST_MM_DS_XC_STATUS *) MsOS_PA2KSEG1(u32FM_Buf_Base);

    // get scaler information
#if (MMSDK_DISPLAY_DUALXC_PORTING == 1)
    MApi_XC_EX_GetStatus((XC_DEVICE_ID*)&g_stXC_PT_DeviceId[0], &stXCStatus, eWin);
#else
    MApi_XC_GetStatus(&stXCStatus, eWin);
#endif
    MS_U32 u32Timeout = MsOS_GetSystemTime() + 5000;
    // copy scaler related information
    while(pstMMDS_Status->bFWIsUpdating)
    {
        MsOS_DelayTask(1);
        if(MsOS_GetSystemTime() > u32Timeout)
        {
            printf("DS Send XC Status to Firmware , bFWIsUpdating time out\n");
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
        FLOWDISP("Warning!!! _DS_Get_VirtualBox_Info return fail, take FHD as VBox size");
    }

    FLOWDISP("vdec_debug VBox Height   %d", pstDSVBox.u16VBox_Vtotal);
    FLOWDISP("vdec_debug VBox Width    %d", pstDSVBox.u16VBox_Htotal);
    stXCStatus.stCapWin.height = pstDSVBox.u16VBox_Vtotal;
    stXCStatus.stCapWin.width = pstDSVBox.u16VBox_Htotal;

    //stXCStatus.stCropWin.height = 1080;
    //stXCStatus.stCropWin.width = 1920;
    printf("use video Crop\n");
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

    FLOWDISPDS("### IPMBase0 = 0x%"DTC_MS_PHY_x" ", (MS_PHY)stXCStatus.u32IPMBase0);
    FLOWDISPDS("### IPMBase1 = 0x%"DTC_MS_PHY_x" ", (MS_PHY)stXCStatus.u32IPMBase1);
    FLOWDISPDS("### IPMBase2 = 0x%"DTC_MS_PHY_x" ", (MS_PHY)stXCStatus.u32IPMBase2);
    FLOWDISPDS("### pstMMDS_Status->bLinearMode = %u ", pstMMDS_Status->bLinearMode);
    FLOWDISPDS("### pstMMDS_Status->u8BitPerPixel = %u ", pstMMDS_Status->u8BitPerPixel);
    FLOWDISPDS("### pstMMDS_Status->bInterlace = %u ", pstMMDS_Status->bInterlace);
    FLOWDISPDS("### pstMMDS_Status->bMirrorMode = %u ", pstMMDS_Status->bMirrorMode);
    FLOWDISPDS("### pstMMDS_Status->u16IPMOffset = %u ", pstMMDS_Status->u16IPMOffset);
    FLOWDISPDS("### pstMMDS_Status->u8StoreFrameNum = %u ", pstMMDS_Status->u8StoreFrameNum);
    FLOWDISPDS("### pstMMDS_Status->u16VirtualBoxWidth  = %u ", pstMMDS_Status->u16VirtualBoxWidth );
    FLOWDISPDS("### pstMMDS_Status->u16VirtualBoxHeight = %u ", pstMMDS_Status->u16VirtualBoxHeight);

    FLOWDISPDS("u8DSVersion %d ", pstMMDS_Status->u8DSVersion);

    FLOWDISPDS("Cap  %d, %d, %d, %d ", pstMMDS_Status->stCapWin.x, pstMMDS_Status->stCapWin.y,
               pstMMDS_Status->stCapWin.width, pstMMDS_Status->stCapWin.height);
    FLOWDISPDS("Crop %d, %d, %d, %d ", pstMMDS_Status->stCropWin.x, pstMMDS_Status->stCropWin.y,
               pstMMDS_Status->stCropWin.width, pstMMDS_Status->stCropWin.height);
    FLOWDISPDS("Disp %d, %d, %d, %d ", pstMMDS_Status->stDispWin.x, pstMMDS_Status->stDispWin.y,
               pstMMDS_Status->stDispWin.width, pstMMDS_Status->stDispWin.height);

    FLOWDISPDS("IPMBase 0/1/2 = %"DTC_MS_U32_x", %"DTC_MS_U32_x", %"DTC_MS_U32_x" ", pstMMDS_Status->u32IPMBase0,
               pstMMDS_Status->u32IPMBase1, pstMMDS_Status->u32IPMBase2);
    FLOWDISPDS("Linear %d, Bit/Pixel %d, Interlace %d ", pstMMDS_Status->bLinearMode,
               pstMMDS_Status->u8BitPerPixel, pstMMDS_Status->bInterlace);
    FLOWDISPDS("PNL %d, %d, %d, %d ", pstMMDS_Status->u16PNL_HST, pstMMDS_Status->u16PNL_HEND, pstMMDS_Status->u16PNL_VST,
               pstMMDS_Status->u16PNL_VEND);

    pstMMDS_Status->bHKIsUpdating = FALSE;
    MsOS_FlushMemory();

    return TRUE;
}

#if (MMSDK_DMS_ENABLE == 1)
static MMSDK_U8 _PT_Display_GetDSIndexDepth(EN_MMSDK_VIDEO_CODEC eVidCodec)
{
    MMSDK_U8 u8IndexDepth = 0;
    #define DS_BUFFER_DEPTH     16
    #define DS_BUFFER_DEPTH_EX  32

    if (eVidCodec == E_MMSDK_VIDEO_CODEC_HEVC_DV
#ifdef HDR_MM_DEV
        || _gu32HDRMetadataType == E_MMSDK_HDR_TYPE_CTL_FLAG_TCH
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
static MS_U8 _PT_Display_GetWindowID(void)
{
#if (MMSDK_DMS_ENABLE == 1)
    E_MSAPI_XC_WINDOW eWindowID = 0;
    E_MSAPI_XC_RESULT eRet = E_MSAPI_XC_NOT_SUPPORT;

    eRet = msAPI_XC_DIPMultiView_CreateWindow(NULL, &eWindowID);
    if (eRet != E_MSAPI_XC_OK)
    {
        FLOW("Get Window ID fail, eRet = %d", eRet);
        //return FALSE;
    }
    return (MS_U8)eWindowID;
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
    return DIPWIN;
#endif //(MMSDK_DMS_ENABLE == 1)
}
#endif

MMSDK_BOOL PT_Display_SetXCWindow(PT_DISPLAYITEM DisplayItem, const ST_MMSDK_XC_RECT *pstCropRect, const ST_MMSDK_XC_RECT *pstDispRect, const MMSDK_U32 u32MuteTimeMS)
{
    FLOW("Crop:%d %d (%d,%d), Disp:%d %d (%d,%d)", pstCropRect->u16X, pstCropRect->u16Y,
         pstCropRect->u16Width, pstCropRect->u16Height, pstDispRect->u16X, pstDispRect->u16Y,
         pstDispRect->u16Width, pstDispRect->u16Height);

    if (DisplayItem == NULL)
    {
        FLOW("DisplayItem is NULL\n");
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

        FLOWDISP("bsetDsOnOff=%d\n",bsetDsOnOff);
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

#ifdef AVP_ENABLE
    if(enMediaType == E_MMSDK_MEDIA_TYPE_MOVIE)
    {
        MS_U8 WINID = 0;
        MS_U8 winLayer = 0;

        // Get Window Zorder
        WINID = log2(pDisplayInstance ->u8ID_BitMask);
        winLayer = PT_SYS_GetAVPZOrder(WINID);
        printf("[%s][%d]====winLayer=%d===\n",__FUNCTION__,__LINE__, winLayer);

        if(pDisplayInstance ->u8Flow == DIP_FLOW)
        {
#if defined(MMSDK_MultiWin_MODE) && (MMSDK_MultiWin_MODE == 0)
#else //defined(MMSDK_MultiWin_MODE) && (MMSDK_MultiWin_MODE == 0)

            if( pDisplayInstance ->u8DIPWin == DIP_Win_NonUse)
            {
                // Get Window ID
                pDisplayInstance ->u8DIPWin = _PT_Display_GetWindowID();
            }

#if (MMSDK_DMS_ENABLE == 1)
            // Set DS on/off  (patch)
            if(pDisplayInstance ->u8DIPWin == E_MSAPI_XC_MAIN_WINDOW)
            {
#if (MMSDK_CTRL_DYNAMIC_SCALING_PORTING == 1)
                MMSDK_BOOL bDynScaling = TRUE;
#else
                MMSDK_BOOL bDynScaling = FALSE;
#endif
                ST_MMSDK_BUF_INFO stBuffInfo = {};
                EN_MMSDK_VIDEO_CODEC eVideoCodec = pDisplayInstance->stDisplayInfo.eVideoType;

                if((bDynScaling == TRUE) && (PT_SYS_GetMmapInfo(&stBuffInfo, E_MMSDK_BUF_XC_DS, 0) != 0))
                {
                    bDynScaling = TRUE;
                    pDisplayInstance->stDisplayInfo.u32DSInfoBaseAddr = stBuffInfo.u32Addr;
                    pDisplayInstance->stDisplayInfo.u32DSSize = stBuffInfo.u32Size;
                    pDisplayInstance->stDisplayInfo.u8DSMIUSelect = stBuffInfo.u32MiuNo;
                    pDisplayInstance->stDisplayInfo.u8DSIndexDepth = _PT_Display_GetDSIndexDepth(eVideoCodec);
                }
                else
                {
                    FLOW("Get E_MMSDK_BUF_XC_DS FAIL, Disable DS\n");
                    bDynScaling = FALSE;
                }
                PT_Display_SetDSOnOFF(DisplayItem, bDynScaling);
#ifdef HDR_MM_DEV
                _PT_Display_SetHDRInit();
                _PT_Display_EnableFrameBufferLess();
#endif
            }
#endif
            printf("[%s][%d]====pDisplayInstance ->u8ID_BitMask=%d===\n",__FUNCTION__,__LINE__,pDisplayInstance ->u8ID_BitMask);
            printf("[%s][%d]====pDisplayInstance ->u8Flow=%d===\n",__FUNCTION__,__LINE__,pDisplayInstance ->u8Flow);
            printf("[%s][%d]====pDisplayInstance->u8DIPWin=%d===\n",__FUNCTION__,__LINE__,pDisplayInstance->u8DIPWin);

            msAPI_XC_DIPMultiView_SetZOrder(&g_stDIP_PT_DeviceId, (E_MSAPI_XC_WINDOW)(pDisplayInstance->u8DIPWin), winLayer);
            msAPI_XC_SetWin_EX(&g_stDIP_PT_DeviceId, NULL, &cropWin, &dispWin, (E_MSAPI_XC_WINDOW)pDisplayInstance ->u8DIPWin);
#endif //defined(MMSDK_MultiWin_MODE) && (MMSDK_MultiWin_MODE == 0)
#if (MMSDK_DMS_ENABLE == 1)
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
    FLOWDISP("bsetDsOnOff=%d\n",bsetDsOnOff);
    PT_Display_SetDSOnOFF(DisplayItem, bsetDsOnOff);

    FLOWDISP("Callback ENABLE_DYNAMIC_SCALING = %d",
             CALLBACK(E_MMSDK_DISPLAY_CALLBACK_IS_ENABLE_DYNAMIC_SCALING, NULL, NULL));

#ifdef HDR_MM_DEV
    _PT_Display_SetHDRInit();
#endif

    if (CALLBACK(E_MMSDK_DISPLAY_CALLBACK_IS_ENABLE_DYNAMIC_SCALING, NULL, NULL) && enMediaType == E_MMSDK_MEDIA_TYPE_MOVIE)
    {
#if (MMSDK_DISPLAY_DUALXC_PORTING == 1)
        PNL_DeviceId stPNL_DeviceId = {g_stXC_PT_DeviceId[0].u32Version, g_stXC_PT_DeviceId[0].u32Id};
        MMSDK_U32 u32DynScalSize = pDisplayInstance->stDisplayInfo.u32DSSize;
        MMSDK_U32 u32DSAddr = pDisplayInstance->stDisplayInfo.u32DSInfoBaseAddr + u32DynScalSize;
        MMSDK_U32 u32CropBottom = pDisplayInstance->stDisplayInfo.u16CropBottom;
        MMSDK_U32 u32Height = pDisplayInstance->stDisplayInfo.u16SrcHeight;
        MMSDK_U32 u32Width = pDisplayInstance->stDisplayInfo.u16SrcWidth;

        DS_VBOX_INFO pstDSVBox =  {.u16VBox_Htotal = DYNAMIC_SCALING_VB_H, .u16VBox_Vtotal = DYNAMIC_SCALING_VB_V};
        if (TRUE != _DS_Get_VirtualBox_Info(&pstDSVBox))
        {
            // for backward capability
            FLOWDISP("Warning!!! _DS_Get_VirtualBox_Info return fail, take FHD as VBox size");
        }

        MS_BOOL bIsLoadPQ = FALSE;
        E_XC_FB_LEVEL eFBLevel = MApi_XC_Get_FB_Level(MAIN_WINDOW);
        XC_GET_FB_LEVEL stXCGetFBLevel;
        memset(&stXCGetFBLevel, 0, sizeof(XC_GET_FB_LEVEL));

        stXCGetFBLevel.stCropWin.width  = cropWin.width;
        stXCGetFBLevel.stCropWin.height = cropWin.height;
        stXCGetFBLevel.stDispWin.width  = dispWin.width;
        stXCGetFBLevel.stDispWin.height = dispWin.height;

        if (MApi_XC_GetChipCaps(E_XC_FB_CAPS_GET_FB_LEVEL, (MS_U32*)&stXCGetFBLevel, sizeof(XC_GET_FB_LEVEL)) == E_APIXC_RET_OK)
        {
            if(stXCGetFBLevel.eFBLevel != eFBLevel)
            {
                bIsLoadPQ = TRUE;
            }
        }

        if((bIsLoadPQ == TRUE) && (stXCGetFBLevel.eFBLevel == E_XC_FB_LEVEL_FB))  //FBL ==> FB case: Need to load PQ first for data to frame buffer.
        {   printf("[%s][%d] FBL ==> FB\n", __FUNCTION__,__LINE__);
            MApi_XC_Set_FB_Level(E_XC_FB_LEVEL_FB ,MAIN_WINDOW);
        }
        else if((bIsLoadPQ == TRUE) && (stXCGetFBLevel.eFBLevel == E_XC_FB_LEVEL_FBL))  //FB ==> FBL case:
        {   printf("[%s][%d] FB ==> FBL\n", __FUNCTION__,__LINE__);
            MApi_XC_Set_FB_Level(E_XC_FB_LEVEL_FBL ,MAIN_WINDOW);
        }

        if(pDisplayInstance->bFirstTimeSetWindow == TRUE)
        {
            msAPI_XC_SetAspectRatioType_EX(&g_stXC_PT_DeviceId[0],E_MSAPI_XC_MAIN_WINDOW,E_MSAPI_XC_ASPECT_RATIO_NORMAL);

            //use panel crop
            MSAPI_XC_VDEC_DispInfo stVidStatus;
            memset(&stVidStatus, 0, sizeof(MSAPI_XC_VDEC_DispInfo));

            if ( E_MSAPI_XC_OK != msAPI_XC_GetVDECInfo_EX_ByPath(E_MSAPI_XC_MAIN_DECODER_PATH,&stVidStatus) )
            {
                FLOWDISP(" Video Get Info Failed!! \n");
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

            msAPI_XC_SetVDECInfo_EX_ByPath(E_MSAPI_XC_MAIN_DECODER_PATH,stVidStatus);
            MS_BOOL bDS = TRUE;
            msAPI_XC_SetCommand(NULL, E_MSAPI_XC_CMD_SET_DYNAMIC_SCALING, &bDS);
            msAPI_XC_SetWin_EX(&g_stXC_PT_DeviceId[0], NULL, &cropWin, &dispWin, E_XC_EX_MAIN_WINDOW);
            msAPI_XC_SetWin_EX(&g_stXC_PT_DeviceId[1], NULL, NULL, NULL, E_XC_EX_MAIN_WINDOW);
            //[K3_Porting] msAPI_VE_SetMode();

            if(pDisplayInstance->bIsAvpFlow == FALSE)
            {
                XC_EX_WINDOW_TYPE stDispWin1;

                stDispWin1.x = dispWin.x + g_IPanelEx.HStart(&stPNL_DeviceId);
                stDispWin1.y = dispWin.y + g_IPanelEx.VStart(&stPNL_DeviceId);
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
                MApi_XC_EX_SetDispWinToReg((XC_DEVICE_ID*)&g_stXC_PT_DeviceId[0], &stDispWin1, E_XC_EX_MAIN_WINDOW);
            }
            pDisplayInstance->bFirstTimeSetWindow = FALSE;
        }
#else

        MMSDK_U32 u32DynScalSize = pDisplayInstance->stDisplayInfo.u32DSSize;
        MMSDK_U32 u32DSAddr = pDisplayInstance->stDisplayInfo.u32DSInfoBaseAddr + u32DynScalSize;
//        MMSDK_U32 u32CropBottom = pDisplayInstance->stDisplayInfo.u16CropBottom;
//        MMSDK_U32 u32Height = pDisplayInstance->stDisplayInfo.u16SrcHeight;
//        MMSDK_U32 u32Width = pDisplayInstance->stDisplayInfo.u16SrcWidth;

        DS_VBOX_INFO pstDSVBox =  {.u16VBox_Htotal = DYNAMIC_SCALING_VB_H, .u16VBox_Vtotal = DYNAMIC_SCALING_VB_V};

        if (TRUE != _DS_Get_VirtualBox_Info(&pstDSVBox))
        {
            // for backward capability
            FLOWDISP("Warning!!! _DS_Get_VirtualBox_Info return fail, take FHD as VBox size");
        }

        /* [Temp note]
        1. For Kris/Kirin with 3M XC buffer and need set small display window, need recaculate Frame buffer level in msAPI_XC_SetWin_EX.
        2. Remove only SetWindow once and may result flash when MM_MoveView. This change is necessary when incorporate DS flow in porting layer into msapi.
        --> Side effect: MM_MoveView may flash.
        */
        //if(pDisplayInstance->bFirstTimeSetWindow == TRUE)
        //{
            msAPI_XC_SetAspectRatioType_EX(&g_stXC_PT_DeviceId, E_MSAPI_XC_MAIN_WINDOW, E_MSAPI_XC_ASPECT_RATIO_NORMAL);

            //use panel crop
            MSAPI_XC_VDEC_DispInfo stVidStatus;
            if ( TRUE != msAPI_XC_GetVDECInfo_EX_ByPath(E_MSAPI_XC_MAIN_DECODER_PATH, &stVidStatus) )
            {
                FLOWDISP(" Video Get Info Failed!! \n");
                return FALSE;
            }

            //MApi_VDEC_SetBlueScreen(FALSE);
            /*//Newer chip doesn't have this limitation
            if ( TRUE == stVidStatus.u8Interlace )
            {
                FLOWDISP(" Only Support Progressive mode!! \n");
                return FALSE;
            }*/

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
            msAPI_XC_SetWin_EX(&g_stXC_PT_DeviceId, NULL, &cropWin, &dispWin, MAIN_WINDOW);
            msAPI_VE_SetMode();

            if(pDisplayInstance->bIsAvpFlow == FALSE)
            {
                MS_WINDOW_TYPE stDispWin1;

                stDispWin1.x = dispWin.x + g_IPanel.HStart();
                stDispWin1.y = dispWin.y + g_IPanel.VStart();
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
                MApi_XC_SetDispWinToReg(&stDispWin1, MAIN_WINDOW);
            }
            pDisplayInstance->bFirstTimeSetWindow = FALSE;
        //}
#endif
        FLOWDISPDS("---crop before recaluate x=%d, y=%d, w=%d, h=%d", cropWin.x, cropWin.y,
        cropWin.width, cropWin.height);
        //cropWin.width = (MMSDK_U16)DYNAMIC_SCALING_VB_H * cropWin.width / u32Width;
        //because of panel crop, cropWin.width must be DYNAMIC_SCALING_VB_H always
        //cropWin.width = (MMSDK_U16)DYNAMIC_SCALING_VB_H;
        //cropWin.x = (MMSDK_U16)DYNAMIC_SCALING_VB_H * cropWin.x / u32Width;

        FLOWDISPDS("----[DBG] (V, crop_h, h, crop_b) (%d,%d,%d,%d)", DYNAMIC_SCALING_VB_V, (int)cropWin.height, (int)u32Height, (int)u32CropBottom);

        //because of panel crop, cropWin.height must be DYNAMIC_SCALING_VB_V always
        //cropWin.height = (MMSDK_U16)DYNAMIC_SCALING_VB_V * cropWin.height / (u32Height - u32CropBottom);
        //cropWin.height = (MMSDK_U16)DYNAMIC_SCALING_VB_V;
        //cropWin.y = (MMSDK_U16)DYNAMIC_SCALING_VB_V * cropWin.y / (u32Height - u32CropBottom);

        FLOWDISPDS("---crop after reclauate x=%d, y=%d, w=%d, h=%d", cropWin.x, cropWin.y, cropWin.width, cropWin.height);
        FLOWDISPDS("---decoded, w=%d, h=%d", (int)u32Width, (int)u32Height);

        // if screen mute disabled, change zoom info only
        FLOWDISPDS("DS_SendZoomInfo2Firmware %"DTC_MS_U32_x"", u32DSAddr);

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

#if (MMSDK_DISPLAY_DUALXC_PORTING == 1)
        pstMMDS_Status->stNewDispWin.x      = dispWin.x + g_IPanelEx.HStart(&stPNL_DeviceId);
        pstMMDS_Status->stNewDispWin.y      = dispWin.y + g_IPanelEx.VStart(&stPNL_DeviceId);
#else
        pstMMDS_Status->stNewDispWin.x      = dispWin.x + g_IPanel.HStart();
        pstMMDS_Status->stNewDispWin.y      = dispWin.y + g_IPanel.VStart();
#endif
        pstMMDS_Status->stNewDispWin.width  = dispWin.width;
        pstMMDS_Status->stNewDispWin.height = dispWin.height;

        //DS must set line offset for crop pixel offset
        //If don't set this, it will always be 1920.
        //MS_U16 u16IPMOffset = MApi_XC_R2BYTE(0x121C);
        MS_U16 u16IPMOffset = pstDSVBox.u16VBox_Htotal;
        pstMMDS_Status->u16IPMOffset = u16IPMOffset;
        if(MApi_XC_IsCurrentFrameBufferLessMode() || msAPI_XC_GetRFBLMode())
        {
            FLOWDISPDS("MMSDK porting DS_FBL");
            pstMMDS_Status->u8MVOPCrop = 1;
        }
        else
        {
            FLOWDISPDS("MMSDK porting DS_FB");
            pstMMDS_Status->u8MVOPCrop = 0;
        }
        pstMMDS_Status->u8EnhanceModeSetting = 1 << 5;
        pstMMDS_Status->bHKIsUpdating = FALSE;
        MsOS_FlushMemory();

        CALLBACK(E_MMSDK_DISPLAY_CALLBACK_UPDATE_ZOOM_INFO, NULL, NULL);
    }
    else
    {
#if (MMSDK_DISPLAY_DUALXC_PORTING == 1)
        if( enMediaType == E_MMSDK_MEDIA_TYPE_PHOTO || enMediaType == E_MMSDK_MEDIA_TYPE_MOVIE)
        {
            MS_BOOL bDS = FALSE;

            FLOW("Set DS off before set window\n");
            PT_Display_SetDSOnOFF(DisplayItem, bDS);
            msAPI_XC_SetCommand(NULL, E_MSAPI_XC_CMD_SET_DYNAMIC_SCALING, &bDS);
            msAPI_XC_SetAspectRatioType_EX(&g_stXC_PT_DeviceId[0],E_MSAPI_XC_MAIN_WINDOW,E_MSAPI_XC_ASPECT_RATIO_NORMAL);
            msAPI_XC_SetWin_EX(&g_stXC_PT_DeviceId[0], NULL, &cropWin, &dispWin, E_XC_EX_MAIN_WINDOW);
            msAPI_XC_SetWin_EX(&g_stXC_PT_DeviceId[1], NULL, NULL, NULL, E_XC_EX_MAIN_WINDOW);
            //[K3_Porting] msAPI_VE_SetMode();
        }
#else
        if(enMediaType == E_MMSDK_MEDIA_TYPE_PHOTO || enMediaType == E_MMSDK_MEDIA_TYPE_MOVIE)
        {
            MS_BOOL bDS = FALSE;

            FLOW("Set DS off before set window\n");
            PT_Display_SetDSOnOFF(DisplayItem, bDS);
            msAPI_XC_SetCommand(NULL, E_MSAPI_XC_CMD_SET_DYNAMIC_SCALING, &bDS);
            msAPI_XC_SetAspectRatioType_EX(&g_stXC_PT_DeviceId, E_MSAPI_XC_MAIN_WINDOW, E_MSAPI_XC_ASPECT_RATIO_NORMAL);
            msAPI_XC_SetWin_EX(&g_stXC_PT_DeviceId, NULL, &cropWin, &dispWin, E_MSAPI_XC_MAIN_WINDOW);
            msAPI_VE_SetMode();
        }
#endif
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
    FLOW("");
    if ((DisplayItem == NULL) || (pDisplayInfo == NULL))
    {
        FLOW("DisplayItem or pDisplayInfo is NULL\n");
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
        FLOW("Show Photo by MVOP!!");
#if (MMSDK_DISPLAY_DUALXC_PORTING == 1)
        FLOWDISP("bMIU0 = %d", pDisplayInfo->bMIU0);

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
            printf("JPEG is progressive!\n");
        }
        else
        {
            stVidStatus.u8Interlace   = 1;
            printf("JPEG is NOT progressive!\n");
        }
        MMSDK_U16 u16ImageWidth = pDisplayInfo->u16SrcWidth;
        MMSDK_U16 u16ImageHeight = pDisplayInfo->u16SrcHeight;
        FLOWDISP("%u,%u", u16ImageWidth, u16ImageHeight);
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
        FLOWDISP("JPD output fb addr = 0x%"DTC_MS_U32_X", MIU = %d", stJpegInfo.u32BufAddr, stJpegInfo.u8MiuSel);
        msAPI_XC_SetJPEGInfo(stJpegInfo);

        //set MVOP config
        msAPI_XC_ForceMVOPDramType_EX(E_MVOP_DEV_0, E_MSAPI_XC_MVOP_SOURCE_TYPE_422);
        msAPI_XC_SetMVOPMiuSel_EX(E_MSAPI_XC_MAIN_DECODER_PATH, TRUE, (pDisplayInfo->bMIU0)? 0 : 1);
        msAPI_XC_SetMVOPConfig_EX(E_MSAPI_XC_MAIN_DECODER_PATH, MVOP_INPUT_DRAM);

        msAPI_XC_SetConnect_EX(&g_stXC_PT_DeviceId[0], E_XC_EX_INPUT_SOURCE_DTV, E_MSAPI_XC_MAIN_WINDOW);

        //for SD out [temp], use AA mode
        msAPI_XC_SetConnect_EX(&g_stXC_PT_DeviceId[1], E_XC_EX_INPUT_SOURCE_SCALER_OP, E_MSAPI_XC_MAIN_WINDOW);
        msAPI_XC_SetWin_EX(&g_stXC_PT_DeviceId[1], NULL, NULL, NULL, E_XC_EX_MAIN_WINDOW);
        msAPI_XC_SetVideoMuteByMode_EX(&g_stXC_PT_DeviceId[1], FALSE, E_XC_EX_MAIN_WINDOW, E_MSAPI_XC_VIDEO_MUTE_MODE_AUTO_SYNC);
        //Because of VE sett output timing will enable ve black screen,
        //disable black screen here
        msAPI_VE_SetVideoMute(DISABLE);
#else
        FLOWDISP("bMIU0 = %d", pDisplayInfo->bMIU0);

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
            printf("JPEG is progressive!\n");
        }
        else
        {
            stVidStatus.u8Interlace   = 1;
            printf("JPEG is NOT progressive!\n");
        }
        MMSDK_U16 u16ImageWidth = pDisplayInfo->u16SrcWidth;
        MMSDK_U16 u16ImageHeight = pDisplayInfo->u16SrcHeight;
        FLOWDISP("%u,%u", u16ImageWidth, u16ImageHeight);
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
        FLOWDISP("JPD output fb addr = 0x%lX, MIU = %d", stJpegInfo.u32BufAddr, stJpegInfo.u8MiuSel);
        msAPI_XC_SetJPEGInfo(stJpegInfo);

        //set MVOP config
        msAPI_XC_ForceMVOPDramType_EX(E_MVOP_DEV_0, E_MSAPI_XC_MVOP_SOURCE_TYPE_422);
        msAPI_XC_SetMVOPMiuSel_EX(E_MSAPI_XC_MAIN_DECODER_PATH, TRUE, (pDisplayInfo->bMIU0)? 0 : 1);
        msAPI_XC_SetMVOPConfig_EX(E_MSAPI_XC_MAIN_DECODER_PATH, MVOP_INPUT_DRAM);
        msAPI_XC_SetConnect_EX(&g_stXC_PT_DeviceId, INPUT_SOURCE_DTV, E_MSAPI_XC_MAIN_WINDOW);
        //Because of VE sett output timing will enable ve black screen,
        //disable black screen here
        msAPI_VE_SetVideoMute(DISABLE);
#endif
    }
    else if(enMediaType == E_MMSDK_MEDIA_TYPE_MOVIE)
    {
#if (MMSDK_DISPLAY_DUALXC_PORTING == 1)
        FLOWDISP("Video Type: %d.\n", pDisplayInfo->eVideoType);
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
                FLOWDISP("Warning!!! _DS_Get_VirtualBox_Info return fail, take FHD as VBox size");
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
        msAPI_XC_SetMVOPConfig_EX(E_MSAPI_XC_MAIN_DECODER_PATH, tMvopInputSel);

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
        FLOWDISP("_DS_Get_VirtualBox_Info");
        DS_VBOX_INFO pstDSVBox =  {.u16VBox_Htotal = DYNAMIC_SCALING_VB_H, .u16VBox_Vtotal = DYNAMIC_SCALING_VB_V};
        MS_U16 u16SrcWidth = pDisplayInfo->u16SrcWidth;
        MS_U16 u16SrcHeight = pDisplayInfo->u16SrcHeight;
        if(pDisplayInfo->bEnableDynScaling)
        {
            if (TRUE != _DS_Get_VirtualBox_Info(&pstDSVBox))
            {
                // for backward capability
                FLOWDISP("Warning!!! _DS_Get_VirtualBox_Info return fail, take FHD as VBox size");
            }

            FLOWDISP("vdec_debug VBox Height   %d", pstDSVBox.u16VBox_Vtotal);
            FLOWDISP("vdec_debug VBox Width    %d", pstDSVBox.u16VBox_Htotal);
            u16SrcWidth  = pstDSVBox.u16VBox_Htotal;
            u16SrcHeight = pstDSVBox.u16VBox_Vtotal;
        }

        FLOWDISP("Video Type: %d.\n", pDisplayInfo->eVideoType);

        MVOP_InputSel tMvopInputSel;
        MS_U32 u32CodecType;
        MSAPI_XC_VDEC_DispInfo stVidStatus;
        //MS504x set MVOP as MIU1 Group1

        MDrv_MVOP_Init();
        FLOWDISP("pDisplayInfo->bMIU0=%d", pDisplayInfo->bMIU0);
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

        FLOWDISP("MM_DispInfo dWidth %d, dHeight %d, dFrameRate %d dInterlace %d",
                 pDisplayInfo->u16SrcWidth, pDisplayInfo->u16SrcHeight, pDisplayInfo->u16FrameRate, pDisplayInfo->u8Interlace);
        FLOWDISP("MM_DispInfo dCropLeft %d, dCropRight %d, dCropTop %d, dCropBottom %d",
                 pDisplayInfo->u16CropLeft, pDisplayInfo->u16CropRight, pDisplayInfo->u16CropTop, pDisplayInfo->u16CropBottom);

        msAPI_XC_SetMVOPConfig_EX(E_MSAPI_XC_MAIN_DECODER_PATH, tMvopInputSel);
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
        FLOW("Show Photo by GOP!!");
#if (MMSDK_DISPLAY_DUALXC_PORTING == 1)
        //for SD out [temp], use AA mode
        msAPI_XC_SetConnect_EX(&g_stXC_PT_DeviceId[1], E_XC_EX_INPUT_SOURCE_SCALER_OP, E_MSAPI_XC_MAIN_WINDOW);
        MApi_XC_SetOSD2VEMode(E_VOP_SEL_OSD_LAST);
        msAPI_XC_SetWin_EX(&g_stXC_PT_DeviceId[1], NULL, NULL, NULL, E_XC_EX_MAIN_WINDOW);
        msAPI_XC_SetVideoMuteByMode_EX(&g_stXC_PT_DeviceId[1], FALSE, E_XC_EX_MAIN_WINDOW, E_MSAPI_XC_VIDEO_MUTE_MODE_AUTO_SYNC);
#endif
    }
    //[River] - Need set frame buffer again?
    //printf("Scaler start: 0x%lx, size: 0x%lx.\n", pDisplayInfo->u32ScalerBuffStart, pDisplayInfo->u32ScalerBuffSize);
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
    FLOW("%u", pXC_ApiInfo->u8MaxFrameNumInMem);
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
    FLOW("%u", pXC_ApiInfo->u8MaxFieldNumInMem);

    return pXC_ApiInfo->u8MaxFieldNumInMem;
}

MMSDK_U16 PT_Display_GetPanelWidth(PT_DISPLAYITEM DisplayItem)
{
#if (MMSDK_DISPLAY_DUALXC_PORTING == 1)
    PNL_DeviceId stPNL_DeviceId = {g_stXC_PT_DeviceId[0].u32Version, g_stXC_PT_DeviceId[0].u32Id};
    FLOW("%u", g_IPanelEx.Width(&stPNL_DeviceId));
    return g_IPanelEx.Width(&stPNL_DeviceId);
#else
    FLOW("%u", g_IPanel.Width());
    return g_IPanel.Width();
#endif
}

MMSDK_U16 PT_Display_GetPanelHeight(PT_DISPLAYITEM DisplayItem)
{
#if (MMSDK_DISPLAY_DUALXC_PORTING == 1)
    PNL_DeviceId stPNL_DeviceId = {g_stXC_PT_DeviceId[0].u32Version, g_stXC_PT_DeviceId[0].u32Id};
    FLOW("%u", g_IPanelEx.Height(&stPNL_DeviceId));
    return g_IPanelEx.Height(&stPNL_DeviceId);
#else
    FLOW("%u", g_IPanel.Height());
    return g_IPanel.Height();
#endif
}

void PT_Display_WaitVSync(PT_DISPLAYITEM DisplayItem, MMSDK_BOOL bIsInput, MMSDK_U8 u8NumVSyncs, MMSDK_U16 u16Timeout)
{
    FLOW("");
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
    FLOW("");

    //[River] in mmsdk_photoplayer function deletemediaitem call this.
    //STB set frame rate convert when in Y8M4 mode
    //For K1(If always Y8M4 mode), Only do set RFBL mode?
    //But msAPI set win will check RFBL, so don't reset it.
    //MDrv_PQ_SetRFblMode(TRUE, FALSE);
}

MMSDK_BOOL PT_Display_CaptureMVopOutput(PT_DISPLAYITEM DisplayItem, ST_MMSDK_VIDEO_FRAME_INFO* pstFrameInfo, const ST_MMSDK_RECT* pstCropRect, MMSDK_U32 u32BuffAddr, MMSDK_U32 u32BuffSize)
{
    FLOW("");
    return TRUE;
}

#if (MM_3D_ENABLE == 1)
static MMSDK_BOOL _PT_Display_SetPhoto3D(PT_DISPLAYITEM DisplayItem)
{
    FLOW("");
    if (DisplayItem == NULL)
    {
        FLOW("DisplayItem is NULL\n");
        return FALSE;
    }

    _ST_MMSDK_DISPLAY_INSTANCE* pDisplayInstance = (_ST_MMSDK_DISPLAY_INSTANCE*)DisplayItem;
    MS_U32 u32OutMod = E_MSAPI_XC_3D_OUTPUT_SIDE_BY_SIDE_HALF;
    HDMITX_VIDEO_3D_STRUCTURE eHDMITx3DMode = HDMITx_VIDEO_3D_Not_in_Use;

    if (TRUE == pDisplayInstance->bISSet3D)
    {
        FLOW("Skip mm set 3D\n");
        return TRUE;
    }

    _HDMITx_3D_Mapping(u32OutMod, &eHDMITx3DMode);
    msAPI_XC_HDMITx_Set3DMode(TRUE, eHDMITx3DMode);
    pDisplayInstance->bISSet3D = TRUE;

    return TRUE;
}

static MMSDK_BOOL _PT_Display_SetVideo3D(PT_DISPLAYITEM DisplayItem)
{
    FLOW("");
    if (DisplayItem == NULL)
    {
        FLOW("DisplayItem is NULL\n");
        return FALSE;
    }

    _ST_MMSDK_DISPLAY_INSTANCE* pDisplayInstance = (_ST_MMSDK_DISPLAY_INSTANCE*)DisplayItem;
    HDMITX_VIDEO_3D_STRUCTURE eHDMITx3DMode = HDMITx_VIDEO_3D_Not_in_Use;
    E_MSAPI_XC_3D_OUTPUT_MODE enCurrent3DType = E_MSAPI_XC_3D_OUTPUT_MODE_NONE;
    //E_MSAPI_XC_3D_INPUT_MODE enInput3DType = E_MSAPI_XC_3D_INPUT_MODE_NONE;

    if (CALLBACK(E_MMSDK_DISPLAY_CALLBACK_IS_ENABLE_DYNAMIC_SCALING, NULL, NULL))
    {
        FLOW("Disable 3D mode, becasue DS need to be set OFF!\n");
        return FALSE;
    }

    if (TRUE == pDisplayInstance->bISSet3D)
    {
        FLOW("Skip mm set 3D\n");
        return TRUE;
    }

    FLOW("3D enum from stream %d\n", pDisplayInstance->stDisplayInfo.en3DLayout);

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
                    FLOW("Enable 3D with Top Bottom!\n");
                    break;
                case E_MMSDK_3DLAYOUT_FRAME_PACKING:
                    enCurrent3DType = E_MSAPI_XC_3D_OUTPUT_FRAME_PACKING;
                    //enInput3DType = E_MSAPI_XC_3D_INPUT_FRAME_PACKING;
                    FLOW("Enable 3D with Frame Packing!\n");
                    break;
                default:
                    FLOW("Unsupported 3D mode!\n");
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
                    FLOW("Enable 3D with Top Bottom!\n");
                    break;
                case E_MMSDK_3DLAYOUT_SIDE_BY_SIDE_LF:
                case E_MMSDK_3DLAYOUT_SIDE_BY_SIDE_RF:
                    enCurrent3DType = E_MSAPI_XC_3D_OUTPUT_SIDE_BY_SIDE_HALF;
                    //enInput3DType = E_MSAPI_XC_3D_INPUT_SIDE_BY_SIDE_HALF;
                    FLOW("Enable 3D with SIDE BY SIDE!\n");
                    break;
                default:
                    FLOW("Unsupported 3D mode!\n");
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
        FLOW("Not 3D Video!\n");
        return FALSE;
    }

    return TRUE;
}
#endif

MMSDK_BOOL PT_Display_Set3DOnOff(PT_DISPLAYITEM DisplayItem, MMSDK_BOOL bEnable3D)
{
    FLOW("[ (bEnable3D=%d)]", bEnable3D);
    if (DisplayItem == NULL)
    {
         FLOW("DisplayItem is NULL\n");
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

        FLOWDISP("PT_Display_Set3DOnOff: unsupported operation\n");
#else
        FLOWDISP("Attention: 3D feature is not enabled by this project!\n");
#endif
     }
     return TRUE;
}

MMSDK_U16 PT_Display_GetOutputFrameRate(PT_DISPLAYITEM DisplayItem)
{
    MMSDK_U16 FrameRate = 0;
    //FLOW("");
    E_MSAPI_XC_OUTPUT_TIMING_TYPE eTiming;

#if (MMSDK_DISPLAY_DUALXC_PORTING == 1)
    msAPI_XC_GetOutputResolution_EX(&g_stXC_PT_DeviceId[0],&eTiming);
#else
    msAPI_XC_GetOutputResolution_EX(&g_stXC_PT_DeviceId, &eTiming);
#endif
    //FLOW("E_MSAPI_XC_OUTPUT_TIMING_TYPE = %d\n", eTiming);
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
            //FLOW("warning!! no-match timing, use 60 hz\n");
            FrameRate = 60;
            break;
    }
    //FLOW("PT_Display_GetOutputFrameRate = %d\n", FrameRate);
    return FrameRate;
}

MMSDK_BOOL PT_Display_IsOutputInterlace(PT_DISPLAYITEM DisplayItem)
{
    FLOW("");
    MMSDK_BOOL IsOutputInterlace = 0;
    E_MSAPI_XC_OUTPUT_TIMING_TYPE eTiming;

#if (MMSDK_DISPLAY_DUALXC_PORTING == 1)
    msAPI_XC_GetOutputResolution_EX(&g_stXC_PT_DeviceId[0],&eTiming);
#else
    msAPI_XC_GetOutputResolution_EX(&g_stXC_PT_DeviceId, &eTiming);
#endif
    FLOW("E_MSAPI_XC_OUTPUT_TIMING_TYPE = %d\n", eTiming);
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
    FLOW("PT_Display_IsOutputInterlace = %d\n", IsOutputInterlace);
    return IsOutputInterlace;
}

MMSDK_BOOL PT_Display_SetHighDynamicRange(PT_DISPLAYITEM DisplayItem, ST_MMSDK_HDR_METADATA* pHDRMetaData)
{
#ifdef HDR_MM_DEV
    if (DisplayItem == NULL || pHDRMetaData == NULL)
    {
        FLOW("DisplayItem or pHDRMetaData is NULL\n");
        return FALSE;
    }
    _gu32HDRMetadataType = pHDRMetaData->u32HDRMetadataType;
    FLOW("HDR type = 0x%"DTC_MS_U32_X" ", pHDRMetaData->u32HDRMetadataType);

/*
    // Diff Type of HDR
    if (pHDRMetaData->u32HDRMetadataType & E_MMSDK_HDR_TYPE_CTL_FLAG_VUI)
    {
    }

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
#endif
    return TRUE;
}

#ifdef OPTEE_ENABLE
MMSDK_BOOL PT_Display_GetXCPipeId(PT_DISPLAYITEM DisplayItem, ST_MMSDK_PIPE_INFO * pstPipeInfo)
{
    FLOW("");
    if ((DisplayItem == NULL) || (pstPipeInfo == NULL))
    {
        FLOW("DisplayItem or pstPipeInfo is NULL\n");
        return FALSE;
    }

    _ST_MMSDK_DISPLAY_INSTANCE* pDisplayInstance = (_ST_MMSDK_DISPLAY_INSTANCE*)DisplayItem;
    XC_OPTEE_HANDLER stOpteeHandler;
    memset((void *)&stOpteeHandler, 0, sizeof(XC_OPTEE_HANDLER));

    stOpteeHandler.eWindow = pDisplayInstance->eScalerWin;

    MApi_XC_OPTEE_Control(E_XC_OPTEE_GET_PIPE_ID, &stOpteeHandler);
    pstPipeInfo->u32SvpPplID = stOpteeHandler.pipeID;
    FLOW("XC pipe ID = %d\n", pstPipeInfo->u32SvpPplID);

    return TRUE;
}

MMSDK_BOOL PT_Display_XC_PipeLock(PT_DISPLAYITEM DisplayItem, ST_MMSDK_PIPE_INFO * pstPipeInfo, MMSDK_BOOL bEnable)
{
    FLOW("Enable = %d", bEnable);
    if ((DisplayItem == NULL) || (pstPipeInfo == NULL))
    {
        FLOW("DisplayItem or pstPipeInfo is NULL\n");
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
    FLOW("");

#if (MMSDK_DISPLAY_DUALXC_PORTING == 1)
    msAPI_XC_EnableMADiForceMotion_EX(&g_stXC_PT_DeviceId[0], E_MSAPI_XC_MAIN_WINDOW, E_MSAPI_XC_MADI_MOTION_YC, bEnable);
#else
    msAPI_XC_EnableMADiForceMotion_EX(&g_stXC_PT_DeviceId, E_MSAPI_XC_MAIN_WINDOW, E_MSAPI_XC_MADI_MOTION_YC, bEnable);
#endif

    return TRUE;
}
#endif

#ifdef AVP_ENABLE
MMSDK_BOOL PT_Display_InitializeEx(PT_DISPLAYITEM* pDisplayItem, void* pWindow)
{
     FLOW("");
    if (pDisplayItem == NULL)
    {
        FLOW("pDisplayItem is NULL\n");
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
   printf("[%s][%d]====pDisplayInstance ->u8ID_BitMask=%d===\n",__FUNCTION__,__LINE__,pDisplayInstance ->u8ID_BitMask);
   *pDisplayItem  = (void*)pDisplayInstance;
   return TRUE;
}

MMSDK_BOOL PT_Display_SetModeEX(PT_DISPLAYITEM DisplayItem, ST_MMSDK_SET_DISPLAY_INFO_EXT *pDisplayInfoExt)
{
    FLOW("");
    if ((DisplayItem == NULL) || (pDisplayInfoExt == NULL))
    {
        FLOW("DisplayItem or pDisplayInfoExt is NULL\n");
        return FALSE;
    }

    printf("[%s][%d]========TRACE=== DisplayItem=%p==\n",__FUNCTION__,__LINE__,DisplayItem);
    _ST_MMSDK_DISPLAY_INSTANCE* pDisplayInstance = (_ST_MMSDK_DISPLAY_INSTANCE*)DisplayItem;
    printf("[%s][%d]====pDisplayInfoExt ->u8DisplayOutputPath=%d===\n",__FUNCTION__,__LINE__,pDisplayInfoExt ->u8DisplayOutputPath);
    printf("[%s][%d]====pDisplayInstance ->u8ID_BitMask=%d===\n",__FUNCTION__,__LINE__,pDisplayInstance ->u8ID_BitMask);

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
                printf("[%s][%d]====DIP_FLOW====\n",__FUNCTION__,__LINE__);
                stCusSetModeInfo.u8Flow = DIP_FLOW;
                pDisplayInstance ->u8Flow = DIP_FLOW;
                bRet = gstSysCusFunPtr.pSetMode2(u16MediaType, &stCusSetModeInfo, NULL);
                break;
            case XC_FLOW:
            default:
                printf("[%s][%d]====XC_FLOW====\n",__FUNCTION__,__LINE__);
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
        printf("[%s][%d]====DIP_FLOW====\n",__FUNCTION__,__LINE__);
#if (MMSDK_DMS_ENABLE == 1)
        PT_Display_BlackScreen(DisplayItem, TRUE);
#endif

#ifdef HDR_MM_DEV
        _PT_Display_CFDInit();
#endif
        pDisplayInstance ->u8Flow = DIP_FLOW;
        msAPI_XC_DIPMultiView_Init(2, E_MSAPI_XC_DIP_FMT_YUV422);

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
        printf("[%s][%d]====XC_FLOW====\n",__FUNCTION__,__LINE__);
        pDisplayInstance ->u8Flow = XC_FLOW;
        PT_Display_SetMode(DisplayItem, &(pDisplayInfoExt->stDispInfo));
        break;
    }

    return TRUE;
}

MMSDK_BOOL PT_Display_SetOneField(PT_DISPLAYITEM DisplayItem, MMSDK_BOOL bEnable)
{
    FLOW("");
    return FALSE;
}

MMSDK_BOOL PT_Display_MvopSendCommand(PT_DISPLAYITEM DisplayItem, void* pHandle, MMSDK_U32 u32Cmd, void *pPara)
{
    FLOW("");
    return FALSE;
}

MMSDK_BOOL PT_Display_MvopGetCommand(PT_DISPLAYITEM DisplayItem, void* pHandle, MMSDK_U32 u32Cmd, void *pPara, MMSDK_U32 u32ParamSize)
{
    FLOW("");
    return FALSE;
}

MMSDK_BOOL PT_Display_IsCurrentFBLMode(PT_DISPLAYITEM DisplayItem, MMSDK_BOOL *pbFBL)
{
    FLOW("");
    return FALSE;
}

MMSDK_BOOL PT_Display_Enable_FBL_TwoInitFactor(PT_DISPLAYITEM DisplayItem, MMSDK_BOOL bEnable)
{
    FLOW("");
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
    //FLOW("");
    if ((DisplayItem == NULL) || (pstPushInfo == NULL))
    {
        FLOW("DisplayItem or pstPushInfo is NULL\n");
        return FALSE;
    }

#if (MMSDK_MultiWin_MODE == 0)
    FLOW("Unsupport DIP Flow!!");
    return FALSE;

#else
    MMSDK_U8 u8FieldNum = 0;
    MMSDK_U8 u8FrameIdx = 0;
    _ST_MMSDK_DISPLAY_INSTANCE* pDisplayInstance = (_ST_MMSDK_DISPLAY_INSTANCE*)DisplayItem;
    MSAPI_XC_VDECFRAME_INFO VDECDispFrame={};

    if(pDisplayInstance ->u8DIPWin == DIP_Win_NonUse)          //invalid winID
        return FALSE;
    //FLOW("[%d]====pDisplayInstance ->u8DIPWin=%d===",__LINE__,pDisplayInstance ->u8DIPWin);
    //FLOW("[%d]====pDisplayInstance ->u8ID_BitMask=%d===",__LINE__,pDisplayInstance ->u8ID_BitMask);
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
            //FLOW("b10bit stream");
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
//          FLOW("MFDec_Enable : [%d] ",VDECDispFrame.stMFdecInfo.bMFDec_Enable);
//          FLOW("MFDec enable u8MFDec_Select : [%d] ",VDECDispFrame.stMFdecInfo.u8MFDec_Select );
//          FLOW("MFDec enable phyBitlen_Base : [0x%"DTC_MS_PHY_x"] ",VDECDispFrame.stMFdecInfo.phyBitlen_Base);
        }
        else
        {
            VDECDispFrame.stFrames[u8FrameIdx].stHWFormat.stMFdecInfo.bMFDec_Enable      = 0;
//          FLOW("MFDec disable");
        }

//          FLOW("u8FrameIdx:%d u32ScanType:%d bFieldOrder:%d eFieldType:%d\n",u8FrameIdx, VDECDispFrame.eScanType,VDECDispFrame.eFieldOrderType, VDECDispFrame.stFrames[u8FrameIdx].eFieldType);
//          FLOW("id[%d] bMFDec_Enable:[%d]  bMFDec_Sel:[%d] 10bit:[%d]\n",VDECDispFrame.stVDECStream_Id_Version.u32VDECStreamID,VDECDispFrame.stFrames[u8FrameIdx].stHWFormat.stMFdecInfo.bMFDec_Enable, VDECDispFrame.stFrames[u8FrameIdx].stHWFormat.stMFdecInfo.u8MFDec_Select, VDECDispFrame.stFrames[u8FrameIdx].b10bitData);
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
    //FLOW("");
    if ((DisplayItem == NULL) || (pstFrameInfo == NULL))
    {
        FLOW("DisplayItem or pstFrameInfo is NULL\n");
        return FALSE;
    }

#if (MMSDK_MultiWin_MODE == 0)
    FLOW("Unsupport DIP Flow!!");
    return FALSE;

#elif (MMSDK_MultiWin_MODE == 1)
    _ST_MMSDK_DISPLAY_INSTANCE* pDisplayInstance = (_ST_MMSDK_DISPLAY_INSTANCE*)DisplayItem;
    MSAPI_XC_VDECFRAME_INFO VDECDispFrame;
    MMSDK_U8 u8FieldNum = 0;
    MMSDK_U8 u8FrameIdx = 0;
    MMSDK_U16 u16VsyncTime = 0;

    //FLOW("[%d]====pDisplayInstance ->u8DIPWin=%d===",__LINE__,pDisplayInstance ->u8DIPWin);
    //FLOW("[%d]====pDisplayInstance ->u8ID_BitMask=%d===",__LINE__,pDisplayInstance ->u8ID_BitMask);
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

#endif
