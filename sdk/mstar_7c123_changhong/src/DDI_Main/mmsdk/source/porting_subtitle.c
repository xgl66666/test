/*-------------------------------------------------------------------------------------------------------
---------------------------------Header Files----------------------------------------------------
---------------------------------------------------------------------------------------------------------
*/
#include <string.h>
#include "MsOS.h"
#include "porting_subtitle.h"
#include "porting_thread.h"
#include "porting_os.h"
#include "porting_sysinfo.h"

#include "MsMMTypes.h"
#include "msAPI_MPEG_Subtitle.h"
#include "msAPI_TEXT_Subtitle.h"

#include "msapi_MM_Common.h"
#include "MApi_MEMConfig_Interface.h"

#include "apiSubtitle.h"
#include "drvDTC.h"

#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus

/*-------------------------------------------------------------------------------------------------------
---------------------------------MARCROs DEBUG APIs----------------------------------------------------
---------------------------------------------------------------------------------------------------------
*/
#define PT_SBTL_FLOW(fmt, arg...)        //printf("\033[1;31m######PortingSubtitle FLOW:[%s]###### "fmt" \033[0m\n",__FUNCTION__,##arg)
#define PT_SBTL_INFO(fmt, arg...)        //printf("\033[1;32m######PortingSubtitle INFO:[%s]###### "fmt" \033[0m\n",__FUNCTION__,##arg)
#define PT_SBTL_ERR(fmt, arg...)         printf("\033[1;33m######PortingSubtitle ERROR:[%s]###### "fmt" \033[0m\n",__FUNCTION__,##arg)

#define AVALIABLE_TIME_TO_FREE_RUN (30)
#define SBTL_PTHREAD_STACK_SIZE 8*1024 //8K
#define SBTL_PTHREAD_NAME  "SubtitleDecoder_Task"

#define SBTL_TTF_DUMP 0

#define LINE_TEXT_MAX (256)

#define MM_MEM_CFG MMPLAYERMEMSIZE_64M

#define MP4_SUBTITLE_BUFFER_LEN    (0x001F0000)
#define MP4_SUBTITLE_TAG_LEN       (0x0001e000)
#define SUBTITLE_BUFFER_LEN        (0x003a0000)

static EN_MMSDK_TRICK_MODE _geTrickMode = E_MMSDK_TRICK_NORMAL;

#ifdef DVB_SUBTITLE_V2
#include "porting_sys.h"
#include "apiSubtitle_Queue.h"
#include "apiPNL.h"
#include "apiDAC.h"
#include "drvTVEncoder.h"
#include "msAPI_VE.h"

#define msAPI_Subtitle_Queue_Push(A,B,C) msAPI_Subtitle_Queue_Push(_dvbSubQueObj,A,B,C)
#define msAPI_Subtitle_Stop()   msAPI_Subtitle_Stop_EX(_gpstSub)
#define msAPI_Subtitle_Start()  msAPI_Subtitle_Start_EX(_gpstSub)
#define msAPI_Subtitle_Clear()  msAPI_Subtitle_Clear_EX(_gpstSub)
#define msAPI_Subtitle_Reset()  msAPI_Subtitle_Reset_EX(_gpstSub)
#define msAPI_Subtitle_DeInit() msAPI_Subtitle_DeInit_EX(_gpstSub)

#define INVLID_DEC_HANDLE (NULL)
#define MEM_ALIGN( unit, val ) (1 + (((val) - 1) | (unit - 1)))

static Sub_ObjHandle *_gpstSub = NULL;
static stQueueObj* _dvbSubQueObj = NULL;
extern PT_SYS_SubRnd_FunPtr gstSysSubRndFunPtr;

typedef struct
{
    MS_BOOL bEnable;
    GFX_BlendCoef eCoef;
    MS_U8 u8Const;
    GFX_AlphaSrcFrom eAlphaSrc;
}_ST_MMSDK_GFX_AlphaInfo;

typedef struct
{
    MS_BOOL bEnable;
    GFX_ColorKeyMode eOpMode;
    GFX_Buffer_Format eFmt;
    void *pvColorStart;
    void *pvColorEnd;
}_ST_MMSDK_GFX_ClrKeyInfo;

typedef struct
{
    GFX_PaletteEntry *pPalArray;
    MS_U16 u16PalStart;
    MS_U16 u16PalEnd;
}_ST_MMSDK_GFX_PaletteInfo;

typedef struct
{
    /* source and destination buffer parameters */
    GFX_BufferInfo *pstSrcBuf;
    GFX_BufferInfo *pstDstBuf;

    /* alpha blending parameters */
    _ST_MMSDK_GFX_AlphaInfo stAlpha;

    /* color key parameters */
    _ST_MMSDK_GFX_ClrKeyInfo stSrcClrKey;
    _ST_MMSDK_GFX_ClrKeyInfo stDstClrKey;

    /* palette parameter */
    _ST_MMSDK_GFX_PaletteInfo stPalette;

    /* DFB parameters */
    MS_BOOL bIsDfbBlend;
    GFX_DFBBldOP eSrcDfbBlendMode;
    GFX_DFBBldOP eDstDfbBlendMode;

    /* YUV parameters */
    GFX_YUV_422 eSrcYUVFmt;
    GFX_YUV_422 eDstYUVFmt;

    /* clip parameter */
    GFX_Block stClipRect;
}ST_MMSDK_GFX_BitbltParam;

typedef struct
{
    MS_BOOL bUsed;
    MS_BOOL bDisplayAble;
    GFX_BufferInfo stBufInfo;
}ST_MMSDK_SUBTITLE_WININFO;

static ST_MMSDK_SUBTITLE_WININFO _gstWinData[SUB_MAX_SUPPORT_WIN]={};
#else
#include "apiSubtitle_Decoder.h"
#endif

#if SBTL_TTF_DUMP
#include "fcntl.h"
#include "apiFS.h"
#include "apiGlobalFunction.h"
#endif

/*-------------------------------------------------------------------------------------------------------
---------------------------------Global Variables----------------------------------------------------------
---------------------------------------------------------------------------------------------------------
*/

typedef struct
{
    MMSDK_BOOL                                    bThreadCreate;      /// thread create or not
    MMSDK_BOOL                                    bThreadDestroy;   ///thread destroy or not
    MMSDK_BOOL                                    bSubtitleDecoding;///thread is decoding or not
    MS_U32                                       u32ThreadId;         /// stThread
    EN_MMSDK_SUBTITLE_TYPE    u32SubtitleType;   ///subtitle type
    MMSDK_BOOL                                    bMutexCreate;      /// Mutex create or not
    void*                                    pstMutex;             /// pstMutex
    void*                                   pStackAddr;         ///Subtitle Thread Stack
    MS_U32                                      u32TaskRet;        ///Task Return value
}_ST_MMSDK_SUBTITLE_PT;

typedef struct
{
    MS_U16 u16CPID;
    MS_U16 u16APID;
}_ST_MMSDK_SUBTITLE_PAGEID;

static _ST_MMSDK_SUBTITLE_PAGEID _gstPageID = {.u16CPID = 0xFFFF, .u16APID = 0xFFFF};
static MMSDK_BOOL _bSubtitleInitialized = FALSE;
#if ENABLE_FREE_TYPE
static MS_BOOL _bSubtitleWithTTF = FALSE;
#if SBTL_TTF_DUMP
static U8 _u8SubtitleTrackID = 0;
#endif
#endif

typedef struct
{
    /// Subtitle codec type info.
    //EN_MMSDK_SUBTITLE_TYPE enSubtitleType;
    /// InternalSubtitle.
    //void* pSubtitle;
    /// The instance of the subtitle's callback function
    void* pCallbackClass;
    /// Subtitle callback function.
    pfnMmsdkSubtitleCallback pCallBack;
    /// Page ID of DVB subtitle.
    //_ST_MMSDK_SUBTITLE_PAGEID stPageID;
    /// The window info of subtitle.
    MMSDK_U8 u8DisplayWin;
} _ST_MMSDK_SUBTITLEITEM;

/* Pointer of MMSDK Class for  g_pCallBack*/
static void * _pMMSDKClass = NULL;
static _ST_MMSDK_SUBTITLE_PT *_pstPTSubtitle = NULL;
static pfnMmsdkSubtitleCallback _pCallBack = NULL;
static _ST_MMSDK_SUBTITLEITEM* _pstNewSubtitleItem = NULL;

static MS_U32 _gu32VDPlayerMP4SUBBUFMEM = (MS_U32)NULL;
static MS_U32 _gu32VDPlayerMP4SUBTAGMEM = (MS_U32)NULL;
static MS_U32 _gu32VDPlayerSUBMEM = (MS_U32)NULL;

/*------------------Internal Functions---------------------------------------------------------------------------*/
#define E_GOP_Sub (MMSDK_SUBTITLE_GOP_PORTING)
#define SUBTITLE_GOP_DST_OP_AS_DEFAULT (1)

static MS_U8 _u8SubGWIN_Id;
static EN_GOP_DST_TYPE _GOPSelType = E_GOP_DST_OP0;
static MS_BOOL _scInterlace = FALSE;

static MS_U8 _u8PrintText[LINE_TEXT_MAX] = {0};
static MS_U16 _u16PrintText[LINE_TEXT_MAX] = {0};

static void _SubtitleMIUSelection(MS_PHY phyAddr)
{
    ST_MMSDK_MMAP_INFO stMIUInfo={};
    PT_SYS_GetMIUInfo(&stMIUInfo);

    MS_U8 u8GOPID = MApi_GOP_GWIN_GetCurrentGOP();
    MApi_GOP_GWIN_SwitchGOP(E_GOP_Sub);

    // Set Miu based on Gwin addr
    if(stMIUInfo.u32MiuBoundary != 0)
    {
        if( phyAddr > stMIUInfo.u32MiuBoundary)
            MApi_GOP_MIUSel(MApi_GOP_GWIN_GetCurrentGOP(), E_GOP_SEL_MIU1);
        else
            MApi_GOP_MIUSel(MApi_GOP_GWIN_GetCurrentGOP(), E_GOP_SEL_MIU0);
    }
    MApi_GOP_GWIN_SwitchGOP(u8GOPID);
}


static void* _SubMem_Alloc_Cache(int size)
{
    return MsOS_AllocateMemory(size, PT_SYS_GetCachePoolID());
}
static void _SubMem_Free_Cache(void* pBuf)
{
    if(pBuf == NULL)
    {
        printf("\n ----------cache error-----------");
        return;
    }
    if (FALSE == MsOS_FreeMemory(pBuf, PT_SYS_GetCachePoolID()))
        printf("!!!! ERROR: FreeCachedMem 0x%"DTC_MS_U32_x" fail \n",  (MS_U32)pBuf);
}
static void* _SubMem_Alloc_NonCache(int size)
{
    return MsOS_AllocateMemory(size, PT_SYS_GetNonCachePoolID());
}
static void _SubMem_Free_NonCache(void* pBuf)
{
    if(pBuf == NULL)
    {
        printf("\n ----------noncache error-----------");
        return;
    }
    if (FALSE ==  MsOS_FreeMemory(pBuf, PT_SYS_GetNonCachePoolID()))
        printf("!!!! ERROR: FreeNoncachedMem 0x%"DTC_MS_U32_x" fail \n",  (MS_U32)pBuf);
}
static MS_U32 _SubMem_VA2PA(MS_VIRT addr)
{
    return MsOS_VA2PA(addr);
}
static MS_U32 _SubMem_PA2VA(MS_PHY addr, int bCached)
{
    if (bCached)
        return MsOS_PA2KSEG0(addr);
    else
        return MsOS_PA2KSEG1(addr);
}

static MS_BOOL _SubOSD_Alloc(GFX_BufferInfo* pBufInfo, GFX_Buffer_Format ColorFmt, unsigned char* pu8Buf, unsigned int u16Width, unsigned int u16Height)
{
    //printf("__Hummingbird__[%s][%d] u16Width = %d, u16Height = %d\n", __FUNCTION__, __LINE__, u16Width, u16Height);
    MS_U8 fbId;
    MS_PHY phyaddr;
    GOP_GwinFBAttr fbAttr;
    fbId = MApi_GOP_GWIN_GetFreeFBID();
    //printf("pu8Buf = 0x%x", (MS_U32)pu8Buf);
    phyaddr = MsOS_VA2PA((MS_VIRT)pu8Buf);
    //printf("pu8Buf(phy) = 0x%lx", phyaddr);
    MApi_GOP_GWIN_CreateFBbyStaticAddr(fbId, 0, 0, u16Width, u16Height, ColorFmt, phyaddr);

    _u8SubGWIN_Id = MApi_GOP_GWIN_CreateWin_Assign_FB(E_GOP_Sub, fbId, 0, 0);
    MApi_GOP_GWIN_GetFBInfo(fbId, &fbAttr);
    pBufInfo->u32Addr = fbAttr.addr;
    pBufInfo->u32Width = fbAttr.width;
    pBufInfo->u32Height = fbAttr.height;
    pBufInfo->u32ColorFmt = ColorFmt;
    pBufInfo->u32Pitch= fbAttr.pitch;

    _SubtitleMIUSelection(pBufInfo->u32Addr);

    //Set GOP blending
    MApi_GOP_GWIN_SwitchGOP(E_GOP_Sub); //U4, only GOP2 support palette table
    MApi_GOP_GWIN_SetBlending(_u8SubGWIN_Id, TRUE, 0xFF);
    MApi_GOP_GWIN_OutputColor(GOPOUT_YUV);

    return TRUE;
}

static MS_BOOL _SubOSD_Free(void)
{
    MApi_GOP_GWIN_DeleteWin(_u8SubGWIN_Id);
    return TRUE;
}

static MS_BOOL _SubOSD_StreamIDSet(SUB_STREAM_INFO *pStreamInfo, VDEC_StreamId *pStreamId)
{
    VDEC_StreamId stVDECInfo;

    if(MApi_VDEC_EX_GetControl(&stVDECInfo, E_VDEC_EX_USER_CMD_GET_MAIN_STREAM_ID, (MS_U32 *)pStreamId) == E_VDEC_EX_OK)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

static MS_BOOL _SubOSD_DestinationSet(SUB_OSD_DST *pGOPDstType)
{
#if SUBTITLE_GOP_DST_OP_AS_DEFAULT
    *pGOPDstType = SBTL_OSD_DST_OP;
#else
    *pGOPDstType = SBTL_OSD_DST_AUTO;
#endif
     return TRUE;
 }

MS_BOOL _check_sc_interlace(void)
{
    VDEC_EX_DispInfo info={0};
    VDEC_EX_Result ret;
    VDEC_StreamId StreamId;

    if(0 == _SubOSD_StreamIDSet(NULL, &StreamId))
    {
        PT_SBTL_ERR("Fail to get stream id.\n");
        return FALSE;
    }

    ret = MApi_VDEC_EX_GetDispInfo(&StreamId, &info);

    if (E_VDEC_EX_OK != ret)
    {
        return FALSE;
    }
    else
    {
        _scInterlace = info.u8Interlace;
        //printf("[%s][%d]Interlace = %u \n", __FUNCTION__, __LINE__,_scInterlace);
        return TRUE;
    }
}

static MS_BOOL _SubOSD_ResulotionSet(unsigned int u16X, unsigned int u16Y, unsigned int u16W, unsigned int u16H, unsigned int u16ScaledW, unsigned int u16ScaledH, MS_BOOL bHScale, MS_BOOL bVScale)
{
    if(_GOPSelType == E_GOP_DST_IP0)
    {
        if( !_check_sc_interlace())
        {
            PT_SBTL_ERR("Get MApi_VDEC_EX_GetDispInfo failed \n");
        }
    }
    //printf("__Hummingbird__[%s][%d] u16X = %d, u16Y = %d, u16W = %d, u16H = %d, u16ScaledW= %d, u16ScaledH = %d\n",
    //        __FUNCTION__, __LINE__, u16X, u16Y, u16W, u16H, u16ScaledW, u16ScaledH);

    //Set Stretch win
    MApi_GOP_GWIN_SwitchGOP(E_GOP_Sub);
    MApi_GOP_GWIN_Set_STRETCHWIN(E_GOP_Sub, _GOPSelType, u16X, u16Y, u16W, u16H);
    MApi_GOP_GWIN_Set_HSCALE(bHScale, u16W, u16ScaledW);
    MApi_GOP_GWIN_Set_VSCALE(bVScale, u16H, u16ScaledH);
    return TRUE;
}

static MS_BOOL _SubOSD_Reset(EN_GOP_DST_TYPE eGOPDstType)
{
    //printf("__Hummingbird__[%s][%d]OSD_Reset \n", __FUNCTION__, __LINE__);

    //Set Init Palette table
    MApi_GOP_GWIN_SwitchGOP(E_GOP_Sub); //U4, only GOP2 support palette table

    GOP_PaletteEntry _gopI8Palette[GOP_PALETTE_ENTRY_NUM];
    //memset( (void *)_gopI8Palette, 0x00 , GOP_PALETTE_ENTRY_NUM*4 ) ;
    int i;
    for (i=0; i<GOP_PALETTE_ENTRY_NUM; i++)
    {
        _gopI8Palette[i].RGB.u8A =  0x00;
        _gopI8Palette[i].RGB.u8R =  0x00;
        _gopI8Palette[i].RGB.u8G =  0x00;
        _gopI8Palette[i].RGB.u8B =  0x00;
    }
    // Wait the REGDMA data is really write to memory, then just ask GOP to update.
    // Testcase: \\mslab7369_2\Streams\China\From_Yurian\MTC2\9\Winter Hill_Ch-66_1a.ts
    MsOS_DelayTask(1);
    if(GOP_API_SUCCESS != MApi_GOP_GWIN_SetPaletteOpt((GOP_PaletteEntry *)_gopI8Palette, 0, GOP_PALETTE_ENTRY_NUM - 1, E_GOP_PAL_ARGB8888))
    {
        printf("\nMApi_GOP_GWIN_SetPaletteOpt failed\n");
    }

    //Set GOP Destination
    _GOPSelType = eGOPDstType;
    MApi_GOP_GWIN_SetGOPDst(E_GOP_Sub, _GOPSelType);
    if(_GOPSelType == E_GOP_DST_IP0)
    {
        MApi_GOP_GWIN_SetFieldInver(TRUE);
    }
    return TRUE;
}

static MS_BOOL _SubOSD_Enable(int bEnable)
{
    //printf("__Hummingbird__[%s][%d] OSD_Enable = %d\n", __FUNCTION__, __LINE__, bEnable);
    MApi_GFX_FlushQueue();
    //MApi_GOP_GWIN_SetForceWrite(TRUE);
    MApi_GOP_GWIN_SwitchGOP(E_GOP_Sub); //U4, only GOP2 support palette table
    MApi_GOP_GWIN_Enable(_u8SubGWIN_Id, bEnable);
    //MApi_GOP_GWIN_SetForceWrite(FALSE);
    return TRUE;
}

static MS_BOOL _SubOSD_PaletteSet(unsigned char* pPalArray, unsigned char u32PalCount, unsigned int u32PalOffset)
{
    GOP_PaletteEntry _gopI8Palette[GOP_PALETTE_ENTRY_NUM];

    //printf("__Hummingbird__[%s][%d] OSD_PaletteSet\n", __FUNCTION__, __LINE__);
    // Wait the REGDMA data is really write to memory, then just ask GOP to update.
    // Testcase: \\mslab7369_2\Streams\China\From_Yurian\MTC2\9\Winter Hill_Ch-66_1a.ts
    MsOS_DelayTask(1);
#if 1
    MS_U16     u16Loop;
    MS_U32     u32Addr;
    DvbSubPalette_Colour    *pst_colour;
    u32Addr = (MS_U32)pPalArray;

    for (u16Loop = 0; u16Loop < u32PalCount; u16Loop++)
    {
        pst_colour = (DvbSubPalette_Colour *)u32Addr;
        u32Addr += 4;

        _gopI8Palette[u16Loop].RGB.u8A =  0xFF - pst_colour[0].u8_A;
        _gopI8Palette[u16Loop].RGB.u8R =  pst_colour[0].u8_R;
        _gopI8Palette[u16Loop].RGB.u8G =  pst_colour[0].u8_G;
        _gopI8Palette[u16Loop].RGB.u8B =  pst_colour[0].u8_B;
        //printf("Get palette[%d] = (0x%x, 0x%x, 0x%x, 0x%x)\n", u16Loop + u32PalOffset,
        //       0xFF - pst_colour[0].u8_A, pst_colour[0].u8_R, pst_colour[0].u8_G, pst_colour[0].u8_B);
    }

    MApi_GOP_GWIN_SwitchGOP(E_GOP_Sub); //U4, only GOP2 support palette table
    MApi_GOP_GWIN_SetPaletteOpt(_gopI8Palette, u32PalOffset, u32PalOffset + u32PalCount - 1, E_GOP_PAL_ARGB8888);

    //printf("Set palette %x to %x\n", u32PalOffset, u32PalOffset + u32PalCount - 1);
#else
    int i;
    for (i=0; i<GOP_PALETTE_ENTRY_NUM; i++)
    {
        _gopI8Palette[i].RGB.u8A =  _gopI8PaletteEntry[i].RGB.u8B;
        _gopI8Palette[i].RGB.u8R =  _gopI8PaletteEntry[i].RGB.u8G;
        _gopI8Palette[i].RGB.u8G =  _gopI8PaletteEntry[i].RGB.u8R;
        _gopI8Palette[i].RGB.u8B =  _gopI8PaletteEntry[i].RGB.u8A;
    }
    MApi_GOP_GWIN_SwitchGOP(2);
    MApi_GOP_GWIN_SetPaletteOpt(_gopI8Palette, 0, GOP_PALETTE_ENTRY_NUM - 1, E_GOP_PAL_ARGB8888);
#endif

#if 0//DUMP_PALETTE_DATA
    MApi_GOP_GWIN_Enable(_u8SubGWIN_Id, FALSE);

    GOP_PaletteEntry _get_gopI8Palette[GOP_PALETTE_ENTRY_NUM];
    MS_U32 palette_clr;

    printf("\n");

    for (u16Loop = u32PalOffset; u16Loop < u32PalOffset + u32PalCount; u16Loop++)
    {
        MApi_GOP_GWIN_ReadPalette(u16Loop, &palette_clr);
        _get_gopI8Palette[u16Loop].RGB.u8A =  palette_clr >> 24;
        _get_gopI8Palette[u16Loop].RGB.u8R =  (palette_clr >> 16) & 0xFF;
        _get_gopI8Palette[u16Loop].RGB.u8G =  (palette_clr >> 8) & 0xFF;
        _get_gopI8Palette[u16Loop].RGB.u8B =  (palette_clr) & 0xFF;
        printf(" Get palette table[%x] = (%x, %x, %x, %x)\n", u16Loop,
               _get_gopI8Palette[u16Loop].RGB.u8A, _get_gopI8Palette[u16Loop].RGB.u8R,
               _get_gopI8Palette[u16Loop].RGB.u8G, _get_gopI8Palette[u16Loop].RGB.u8B);
    }

    MApi_GOP_GWIN_Enable(_u8SubGWIN_Id, TRUE);

    printf("\n");

#endif

    return TRUE;
}

#ifdef DVB_SUBTITLE_V2

static MS_BOOL _GFX_CheckSeg(
    const MS_U16 *pu16Src,
    MS_U16 *pu16Dst,
    const MS_U16 u16Target
    )
{
    MS_BOOL bNeedSeg = FALSE;

    GFX_BLT_DownScaleInfo stBltInfo;

    if(MApi_GFX_GetGECaps(E_GFX_CAP_BLT_DOWN_SCALE, (MS_U32 *)&stBltInfo, 4))
    {
        return FALSE;
    }

    if(*pu16Dst > *pu16Src) //scale up case, return directly
    {
        return FALSE;
    }

    if(stBltInfo.bFullRangeSupport)
    {
        if((*pu16Src /stBltInfo.u8RangeMin)  >= u16Target)   //need segment blit
        {
            if(!(((*pu16Src%stBltInfo.u8RangeMin)==0) && ((*pu16Src /stBltInfo.u8RangeMin) == u16Target)))
            {
                bNeedSeg = TRUE;
                *pu16Dst =  (*pu16Src / stBltInfo.u8RangeMin) +1 ;
            }
        }
    }
    else    //Not fully range support, need check separately between fully and not fully (not continuous) range
    {
        stBltInfo.u8ContinuousRangeMin = 2;
        if(!
            (((*pu16Src /stBltInfo.u8ContinuousRangeMin ) < u16Target) //on fully support range
            ||(((*pu16Src /stBltInfo.u8ContinuousRangeMin ) == u16Target) && ((*pu16Src%stBltInfo.u8ContinuousRangeMin) ==0))) //match continuous range
            )
        {
            bNeedSeg = TRUE;
            MS_U8 i=0;

            //check on not fully support range
            for(i=stBltInfo.u8RangeMin ; i>stBltInfo.u8ContinuousRangeMin; )
            {
                if(((*pu16Src /i) == u16Target) && ((*pu16Src %i) == 0)) //don't need segment, match not fully support range
                {
                    bNeedSeg = FALSE;
                    break;
                }
                i = i>>1;
            }

            //find upperbound range
            if(bNeedSeg)
            {
                MS_U8 u8UpperBound = stBltInfo.u8ContinuousRangeMin;
                *pu16Dst =  (*pu16Src /u8UpperBound) +1;

                for(i=stBltInfo.u8RangeMin ; i>stBltInfo.u8ContinuousRangeMin; )
                {
                    if(((*pu16Src /i) >= u16Target) && ((*pu16Src %i) == 0))
                    {
                        u8UpperBound = i;
                        *pu16Dst =  (*pu16Src /u8UpperBound) ;
                        break;
                    }
                    i = i>>1;
                }
            }
        }
    }

    PT_SBTL_FLOW("NeedSeg (%d) : from (%d) to (%d) \n", bNeedSeg, *pu16Src, *pu16Dst);

    return bNeedSeg;
}

static MS_BOOL _GFX_GetPixUnit(GFX_Buffer_Format eFmt, MS_U8 *pu8Align)
{
    switch(eFmt)
    {
        case GFX_FMT_ARGB8888:
            *pu8Align = 4;
            break;
        case GFX_FMT_RGB565:
        case GFX_FMT_ARGB4444:
        case GFX_FMT_ARGB1555:
        case GFX_FMT_YUV422:
        //case E_MI_OSD_FMT_ARGB1555_DST:
            *pu8Align = 2;
            break;
        case GFX_FMT_I1:
        case GFX_FMT_I2:
        case GFX_FMT_I4:
        case GFX_FMT_I8:
            *pu8Align = 1;
            break;
        default:
            PT_SBTL_ERR("not defined format.\n");
            return FALSE;
    }

    return TRUE;
}

static MS_BOOL _GFX_GetDefaultBitbltParam(ST_MMSDK_GFX_BitbltParam *pstParam)
{
    static GFX_RgbColor stDummyCK = {};

    pstParam->stAlpha.bEnable = FALSE;
    pstParam->stAlpha.eAlphaSrc = ABL_FROM_ASRC;
    pstParam->stAlpha.eCoef = COEF_ADST;

    pstParam->stSrcClrKey.bEnable = FALSE;
    pstParam->stSrcClrKey.eOpMode = CK_OP_EQUAL;
    pstParam->stSrcClrKey.eFmt = GFX_FMT_ARGB8888;
    pstParam->stSrcClrKey.pvColorStart = &stDummyCK;
    pstParam->stSrcClrKey.pvColorEnd= &stDummyCK;

    pstParam->stDstClrKey.bEnable = FALSE;
    pstParam->stDstClrKey.eOpMode = CK_OP_EQUAL;
    pstParam->stDstClrKey.eFmt = GFX_FMT_ARGB8888;
    pstParam->stDstClrKey.pvColorStart = &stDummyCK;
    pstParam->stDstClrKey.pvColorEnd= &stDummyCK;

    pstParam->bIsDfbBlend = FALSE;
    pstParam->eSrcDfbBlendMode = GFX_DFB_BLD_OP_ZERO;
    pstParam->eDstDfbBlendMode = GFX_DFB_BLD_OP_ZERO;

    pstParam->eSrcYUVFmt = GFX_YUV_YVYU;
    pstParam->eDstYUVFmt = GFX_YUV_YVYU;

    memset(&pstParam->stClipRect, 0x00, sizeof(GFX_Block));

    return TRUE;
}

static MS_BOOL _GFX_SetBitbltParam(ST_MMSDK_GFX_BitbltParam *pstParam)
{
    MS_U16 u16Idx;
    GFX_RgbColor stDummyCK[2] = {};
    GFX_RgbColor stColor;
    GFX_PaletteEntry *pPalEntry;

    if(MApi_GFX_SetDC_CSC_FMT(
        GFX_YUV_RGB2YUV_PC,
        GFX_YUV_OUT_PC,
        GFX_YUV_IN_255,
        (GFX_YUV_422)(pstParam->eSrcYUVFmt),
        (GFX_YUV_422)(pstParam->eDstYUVFmt)) != GFX_SUCCESS)
    {
        return FALSE;
    }

    if(!pstParam->stSrcClrKey.bEnable)
    {
        pstParam->stSrcClrKey.pvColorStart = &stDummyCK[0];
        pstParam->stSrcClrKey.pvColorEnd = &stDummyCK[1];
    }

    if(MApi_GFX_SetSrcColorKey(
        pstParam->stSrcClrKey.bEnable,
        pstParam->stSrcClrKey.eOpMode,
        pstParam->stSrcClrKey.eFmt,
        pstParam->stSrcClrKey.pvColorStart,
        pstParam->stSrcClrKey.pvColorEnd) != GFX_SUCCESS)
    {
        return FALSE;
    }

    if(!pstParam->stDstClrKey.bEnable)
    {
        pstParam->stDstClrKey.pvColorStart = &stDummyCK[0];
        pstParam->stDstClrKey.pvColorEnd = &stDummyCK[1];
    }

    if(MApi_GFX_SetDstColorKey(
        pstParam->stDstClrKey.bEnable,
        pstParam->stDstClrKey.eOpMode,
        pstParam->stDstClrKey.eFmt,
        pstParam->stDstClrKey.pvColorStart,
        pstParam->stDstClrKey.pvColorEnd) != GFX_SUCCESS)
    {
        return FALSE;
    }

    if(pstParam->stPalette.pPalArray)
    {
        if( pstParam->pstSrcBuf &&
           ((pstParam->pstSrcBuf->u32ColorFmt == GFX_FMT_I2) ||
           (pstParam->pstSrcBuf->u32ColorFmt == GFX_FMT_I4)) )
        {
            pPalEntry = pstParam->stPalette.pPalArray;

            for(u16Idx = pstParam->stPalette.u16PalStart; (u16Idx < 16) && (u16Idx <= pstParam->stPalette.u16PalEnd); u16Idx++)
            {
                stColor.a = pPalEntry->RGB.u8A;
                stColor.r = pPalEntry->RGB.u8R;
                stColor.g = pPalEntry->RGB.u8G;
                stColor.b = pPalEntry->RGB.u8B;

                MApi_GFX_SetIntensity(
                    (MS_U8)u16Idx,
                    GFX_FMT_ARGB8888,
                    (MS_U32 *)&stColor);

                pPalEntry++;
            }
        }
        else
        {
            if(MApi_GFX_SetPaletteOpt(
                pstParam->stPalette.pPalArray,
                pstParam->stPalette.u16PalStart,
                pstParam->stPalette.u16PalEnd) != GFX_SUCCESS)
            {
                return FALSE;
            }
        }
    }

    if(pstParam->bIsDfbBlend)
    {
        if(MApi_GFX_EnableAlphaBlending(FALSE) != GFX_SUCCESS)
        {
            return FALSE;
        }

        if(MApi_GFX_SetDFBBldOP(pstParam->eSrcDfbBlendMode, pstParam->eDstDfbBlendMode) != GFX_SUCCESS)
        {
            return FALSE;
        }

        if(MApi_GFX_EnableDFBBlending(TRUE) != GFX_SUCCESS)
        {
            return FALSE;
        }
    }
    else
    {
        if(MApi_GFX_EnableDFBBlending(FALSE) != GFX_SUCCESS)
        {
            return FALSE;
        }

        if(MApi_GFX_SetAlpha(
            pstParam->stAlpha.bEnable,
            pstParam->stAlpha.eCoef,
            pstParam->stAlpha.eAlphaSrc,
            pstParam->stAlpha.u8Const) != GFX_SUCCESS)
        {
            return FALSE;
        }
    }

    return TRUE;
}

static MS_BOOL _GFX_GetAlign(GFX_Buffer_Format eFmt, EN_GFX_BUF_TYPE eBufType, MS_U32 *pu32Align)
{
    GFX_CapMultiPixelInfo stMultiPixelInfo;
    GFX_FmtAlignCapsInfo stAlignInfo;

    if(MApi_GFX_GetGECaps(E_GFX_CAP_MULTI_PIXEL, (MS_U32*)&stMultiPixelInfo, sizeof(GFX_CapMultiPixelInfo)) != GFX_SUCCESS)
    {
        return FALSE;
    }

    stAlignInfo.u32ColorFmt = eFmt;
    stAlignInfo.eBufferType = eBufType;
    stAlignInfo.eFmtCapsType = stMultiPixelInfo.bSupportMultiPixel ? E_GFX_FMT_CAP_MULTI_PIXEL: E_GFX_FMT_CAP_NONE;
    if(MApi_GFX_GetAlignCaps(&stAlignInfo) != GFX_SUCCESS)
    {
        return FALSE;
    }

    *pu32Align = stAlignInfo.u8BaseAlign;

    return TRUE;
}

GFX_Result _GFX_BitBlt(GFX_DrawRect *ptDrawRect, ST_MMSDK_GFX_BitbltParam *ptParam)
{
    MS_U32 u32TgtSrcW;
    MS_U32 u32TgtSrcH;
    MS_PHY phyTgtSrcVAddr;
    MS_U32 u32TgtSrcPitch;
    GFX_Buffer_Format u8TgtSrcFmt;

    MS_U32 u32TgtDstW;
    MS_U32 u32TgtDstH;
    MS_PHY phyTgtDstVAddr;
    MS_U32 u32TgtDstPitch;
    GFX_Buffer_Format u8TgtDstFmt;

    MS_U32 u32TmpSrcW;
    MS_U32 u32TmpSrcH;
    MS_PHY phyTmpSrcVAddr;
    MS_U32 u32TmpSrcPitch;
    GFX_Buffer_Format u8TmpSrcFmt;

    MS_U32 u32TmpDstW;
    MS_U32 u32TmpDstH;
    MS_PHY phyTmpDstVAddr;
    MS_U32 u32TmpDstPitch;
    GFX_Buffer_Format u8TmpDstFmt;

    GFX_Block stTmpSrcRect;
    GFX_Block stTmpDstRect;

    MS_BOOL bNeedSegW = TRUE;
    MS_BOOL bNeedSegH = TRUE;

    MS_U16 u32TargetW;
    MS_U16 u32TargetH;

    MS_U32 u32ExtSrcAddr = 0;
    MS_U32 u32ExtDstAddr = 0;

    ST_MMSDK_GFX_BitbltParam stBitBltParam = {};

    GFX_Result eRet = GFX_SUCCESS;

    if(!ptDrawRect || !ptParam)
    {
        return eRet;
    }

    u32TgtSrcW  = ptParam->pstSrcBuf->u32Width;
    u32TgtSrcH  = ptParam->pstSrcBuf->u32Height;
    phyTgtSrcVAddr = ptParam->pstSrcBuf->u32Addr;
    u32TgtSrcPitch = ptParam->pstSrcBuf->u32Pitch;
    u8TgtSrcFmt = ptParam->pstSrcBuf->u32ColorFmt;

    u32TgtDstW  = ptParam->pstDstBuf->u32Width;
    u32TgtDstH  = ptParam->pstDstBuf->u32Height;
    phyTgtDstVAddr = ptParam->pstDstBuf->u32Addr;
    u32TgtDstPitch = ptParam->pstDstBuf->u32Pitch;
    u8TgtDstFmt = ptParam->pstDstBuf->u32ColorFmt;

    u32TmpSrcW  = u32TgtSrcW;
    u32TmpSrcH  = u32TgtSrcH;
    phyTmpSrcVAddr  = phyTgtSrcVAddr;
    u32TmpSrcPitch = u32TgtSrcPitch;
    u8TmpSrcFmt = u8TgtSrcFmt;

    u32TmpDstW  = u32TgtDstW;
    u32TmpDstH  = u32TgtDstH;
    phyTmpDstVAddr  = phyTgtDstVAddr;
    u32TmpDstPitch = u32TgtDstPitch;
    u8TmpDstFmt = u8TgtDstFmt;

    stTmpSrcRect = ptDrawRect->srcblk;
    stTmpDstRect = ptDrawRect->dstblk;

    //parameter check
    if((u32TgtSrcW == 0) ||(u32TgtSrcH == 0)
        ||(u32TgtDstW == 0) ||(u32TgtDstH == 0)
        ||(ptDrawRect->srcblk.width == 0) ||(ptDrawRect->srcblk.height == 0)
        ||(ptDrawRect->dstblk.width == 0) ||(ptDrawRect->dstblk.height == 0)
        )
    {
        PT_SBTL_ERR("BitBlit fail, invalied parameter : width or height = 0 \n");
        return GFX_INVALID_PARAMETERS;
    }

    u32TargetW = ptDrawRect->srcblk.width;
    u32TargetH = ptDrawRect->srcblk.height;

    do
    {
        bNeedSegW &= _GFX_CheckSeg(&(stTmpSrcRect.width), &(stTmpDstRect.width), u32TargetW);
        bNeedSegH &= _GFX_CheckSeg(&(stTmpSrcRect.height), &(stTmpDstRect.height), u32TargetH);

        if(bNeedSegW || bNeedSegH)
        {
            MS_U8 u8TmpSrcPixelSize = 0;
            MS_U32 u32TmpSrcAlign = 0;

            _GFX_GetPixUnit(u8TgtSrcFmt, &u8TmpSrcPixelSize);
            _GFX_GetAlign(u8TgtSrcFmt, E_GFX_BUF_DST, &u32TmpSrcAlign);

            u32ExtDstAddr = (MS_U32)MsOS_AllocateMemory((stTmpDstRect.width*stTmpDstRect.height*u8TmpSrcPixelSize + u32TmpSrcAlign), PT_SYS_GetNonCachePoolID());
            if(u32ExtDstAddr == 0)
            {
                eRet = GFX_FAIL;
                break;
            }

            PT_SBTL_FLOW("allocate tmp buf addr : 0x%"DTC_MS_U32_x"\n", u32ExtDstAddr);
            phyTmpDstVAddr = MEM_ALIGN(u32TmpSrcAlign, u32ExtDstAddr);
            memset((void*)(MS_PHYADDR)phyTmpDstVAddr, 0, (u32TmpDstW*u32TmpDstH*u8TmpSrcPixelSize));

            stTmpDstRect.x = 0;
            stTmpDstRect.y = 0;
            u32TmpDstW = stTmpDstRect.width;
            u32TmpDstH = stTmpDstRect.height;
            u32TmpDstPitch = u32TgtSrcPitch;
            u8TmpDstFmt = u8TgtSrcFmt;

            _GFX_GetDefaultBitbltParam(&stBitBltParam);
        }
        else
        {
            stTmpDstRect = ptDrawRect->dstblk;
            phyTmpDstVAddr = phyTgtDstVAddr;
            u32TmpDstW = u32TgtDstW;
            u32TmpDstH = u32TgtDstH;
            u32TmpDstPitch = u32TgtDstPitch;
            u8TmpDstFmt = u8TgtDstFmt;
            stBitBltParam = *ptParam;
        }

        GFX_DrawRect bitbltInfo;
        GFX_BufferInfo gfxSrcBuf, gfxDstBuf;
        GFX_Point v0, v1;

        gfxSrcBuf.u32Addr = phyTmpSrcVAddr;
        gfxSrcBuf.u32Pitch = u32TmpSrcPitch;
        gfxSrcBuf.u32Width = u32TmpSrcW;
        gfxSrcBuf.u32Height = u32TmpSrcH;
        gfxSrcBuf.u32ColorFmt = u8TmpSrcFmt ;
        if((eRet = MApi_GFX_SetSrcBufferInfo(&gfxSrcBuf, 0)) != GFX_SUCCESS)
        {
            break;
        }

        gfxDstBuf.u32Addr = phyTmpDstVAddr;
        gfxDstBuf.u32Pitch = u32TmpDstPitch;
        gfxDstBuf.u32Width = u32TmpDstW;
        gfxDstBuf.u32Height = u32TmpDstH;
        gfxDstBuf.u32ColorFmt = u8TmpDstFmt;
        if(u8TmpDstFmt == GFX_FMT_ARGB1555)
        {
            gfxDstBuf.u32ColorFmt = GFX_FMT_ARGB1555_DST;
        }

        if((eRet = MApi_GFX_SetDstBufferInfo(&gfxDstBuf, 0)) != GFX_SUCCESS)
        {
            break;
        }

        if((ptParam->stClipRect.width== 0) ||(ptParam->stClipRect.height== 0)) //default : clip rectangle = dest rect
        {
            v0.x = stTmpDstRect.x;
            v0.y = stTmpDstRect.y;
            v1.x = stTmpDstRect.width+ v0.x;
            v1.y = stTmpDstRect.height+ v0.y;
        }
        else
        {
            v0.x = ptParam->stClipRect.x;
            v0.y = ptParam->stClipRect.y;
            v1.x = ptParam->stClipRect.width+ v0.x;
            v1.y = ptParam->stClipRect.height+ v0.y;
        }

        //protect to avoid out of range
        v1.x = (v1.x > u32TmpDstW)?u32TmpDstW:v1.x;
        v1.y = (v1.y > u32TmpDstH)?u32TmpDstH :v1.y;
        PT_SBTL_FLOW("Clip range (%d, %d -> %d, %d )\n",v0.x,v0.y,v1.x,v1.y);
        if((eRet = MApi_GFX_SetClip(&v0, &v1)) != GFX_SUCCESS)
        {
            break;
        }

        bitbltInfo.srcblk = stTmpSrcRect;
        bitbltInfo.dstblk = stTmpDstRect;

        // if non-stretch bitblt, use default draw flag to enable 2P mode
        MS_U32 u32DrawFlag = 0;

        u32DrawFlag = ((stTmpSrcRect.width== stTmpDstRect.width) && (stTmpSrcRect.height== stTmpDstRect.height))?(GFXDRAW_FLAG_DEFAULT):(GFXDRAW_FLAG_SCALE);

        PT_SBTL_FLOW("bitblit from src to dst  ( %d, %d, %d, %d) -> ( %d, %d, %d, %d)\n",
            bitbltInfo.srcblk.x,bitbltInfo.srcblk.y,bitbltInfo.srcblk.width,bitbltInfo.srcblk.height,
            bitbltInfo.dstblk.x,bitbltInfo.dstblk.y,bitbltInfo.dstblk.width,bitbltInfo.dstblk.height);

        if(_GFX_SetBitbltParam(&stBitBltParam) == FALSE)
        {
            break;
        }

        MApi_GFX_SetMirror(FALSE, FALSE);
        MApi_GFX_SetRotate(GEROTATE_0);

        if((eRet = MApi_GFX_BitBlt(&bitbltInfo, u32DrawFlag)) != GFX_SUCCESS)
        {
            break;
        }

        if(u32ExtSrcAddr != 0)
        {
            MsOS_FreeMemory((void*)u32ExtSrcAddr, PT_SYS_GetNonCachePoolID());
            u32ExtSrcAddr = 0;
        }

        //update value, set dst as source and loop again
        stTmpSrcRect = stTmpDstRect;
        phyTmpSrcVAddr = phyTmpDstVAddr;
        u32TmpSrcW = u32TmpDstW;
        u32TmpSrcH = u32TmpDstH;

        u32ExtSrcAddr = u32ExtDstAddr;
        u32ExtDstAddr = 0;
    }while(bNeedSegW || bNeedSegH);

    if(u32ExtSrcAddr != 0)
    {
        MsOS_FreeMemory((void*)u32ExtSrcAddr, PT_SYS_GetNonCachePoolID());
        u32ExtSrcAddr = 0;
    }

    if(u32ExtDstAddr != 0)
    {
        MsOS_FreeMemory((void*)u32ExtDstAddr, PT_SYS_GetNonCachePoolID());
        u32ExtDstAddr = 0;
    }

    return eRet;
}

static MS_U8 _SubRnd_GetFreeWinIdx(void)
{
    MS_U8 u8Idx;

    for(u8Idx=0; u8Idx<SUB_MAX_SUPPORT_WIN; u8Idx++)
    {
        if(_gstWinData[u8Idx].bUsed == FALSE)
        {
            return u8Idx;
        }
    }

    return SUB_INVALID_WIN_ID;
}

static EN_GOP_DST_TYPE _SubRnd_GetGOPPath(SUB_OSD_DST eGOPDst)
{
    E_MSAPI_VE_SOURCE_TYPE eVESrcType;

    if(SBTL_OSD_DST_AUTO == eGOPDst)
    {
        eVESrcType = msAPI_VE_GetSourceType();
        if(eVESrcType == E_MSAPI_VE_SOURCE_SCALER_OP2)
        {
            return E_GOP_DST_OP0;
        }
        else if(eVESrcType == E_MSAPI_VE_SOURCE_SCALER_DI2VE)
        {
            return E_GOP_DST_IP0;
        }
        else
        {
            return E_GOP_DST_OP0;
        }
    }
    else if(SBTL_OSD_DST_IP == eGOPDst)
    {
        return E_GOP_DST_IP0;
    }
    else /* set SBTL_OSD_DST_OP */
    {
        return E_GOP_DST_OP0;
    }
}

static MS_BOOL _SubRnd_GetDispSize(MS_U16 *pu16X, MS_U16 *pu16Y, MS_U16 *pu16Width, MS_U16 *pu16Height)
{
    VDEC_StreamId StreamId;
    VDEC_EX_DispInfo Dispinfo;
    SUB_OSD_DST eDst;
    EN_GOP_DST_TYPE eGOPSelType;

    if(0 == _SubOSD_StreamIDSet(NULL, &StreamId))
    {
        PT_SBTL_ERR("Fail to get stream id.\n");
        return FALSE;
    }

    if(0 == _SubOSD_DestinationSet(&eDst))
    {
        PT_SBTL_ERR("OSD resulotion set fail\n");
        return FALSE;
    }

    eGOPSelType = _SubRnd_GetGOPPath(eDst);

    PT_SBTL_FLOW("GOP destination: %d\n", eGOPSelType);

    if((E_VDEC_EX_OK == MApi_VDEC_EX_GetDispInfo(&StreamId, &Dispinfo)) &&
        (0 != Dispinfo.u16HorSize) &&
        (0 != Dispinfo.u16VerSize))
    {
        if(eGOPSelType == E_GOP_DST_IP0)
        {
            *pu16X = 0;
            *pu16Y = 0;
            *pu16Width = Dispinfo.u16HorSize;
            *pu16Height = Dispinfo.u16VerSize;
        }
        else
        {
            MS_WINDOW_TYPE stDispWin;

            MApi_XC_GetDispWinFromReg(&stDispWin, MAIN_WINDOW);
            *pu16X = stDispWin.x - g_IPanel.HStart();
            *pu16Y = stDispWin.y - g_IPanel.VStart();
            *pu16Width = stDispWin.width;
            *pu16Height = stDispWin.height;
        }
    }
    else
    {
        return FALSE;
    }

    return TRUE;
}

static MsMW_WinHandle _SubRnd_CreateWindow(MsMW_RndWinAttr *ptWinAttr, Sub_ObjHandle hSubObj)
{
    MS_U8 u8Idx;
    ST_MMSDK_SUBTITLE_WININFO *pstWndData;
    SUB_OSD_DST eGOPDst;
    GFX_Buffer_Format eColorFmt;

    u8Idx = _SubRnd_GetFreeWinIdx();
    if(u8Idx == SUB_INVALID_WIN_ID)
    {
        return (MsMW_WinHandle)INVALID_WINDOW_HANDLE;
    }

    pstWndData = &_gstWinData[u8Idx];

    pstWndData->bDisplayAble = ptWinAttr->bDisplayAble;

    /* color format mapping */
    switch(ptWinAttr->eColor)
    {
    case MsMW_CLRFMT_I2:eColorFmt = GFX_FMT_I2;break;
    case MsMW_CLRFMT_I8:eColorFmt = GFX_FMT_I8;break;
    case MsMW_CLRFMT_ARGB4444:eColorFmt = GFX_FMT_ARGB4444;break;
    case MsMW_CLRFMT_ARGB8888:eColorFmt = GFX_FMT_ARGB8888;break;
    default:
        return (MsMW_WinHandle)INVALID_WINDOW_HANDLE;
    }

    if(ptWinAttr->bDisplayAble)
    {
        MS_ASSERT((void*)(ptWinAttr->virtBufAddr) != NULL);

        if (0 == _SubOSD_DestinationSet(&eGOPDst))
        {
            PT_SBTL_ERR("OSD resulotion set fail\n");
            _SubOSD_Free();
            return (MsMW_WinHandle)INVALID_WINDOW_HANDLE;
        }

        if (0 == _SubOSD_Reset(_SubRnd_GetGOPPath(eGOPDst)))
        {
            PT_SBTL_ERR("OSD reset fail\n");
            _SubOSD_Free();
            return (MsMW_WinHandle)INVALID_WINDOW_HANDLE;
        }

        if (0 == _SubOSD_Alloc(&pstWndData->stBufInfo, eColorFmt, (unsigned char *)ptWinAttr->virtBufAddr, ptWinAttr->u16Width, ptWinAttr->u16Height))
        {
            return (MsMW_WinHandle)INVALID_WINDOW_HANDLE;
        }
    }
    else
    {
        pstWndData->stBufInfo.u32Addr = _SubMem_VA2PA(ptWinAttr->virtBufAddr);
        pstWndData->stBufInfo.u32Width = ptWinAttr->u16Width;
        pstWndData->stBufInfo.u32Height = ptWinAttr->u16Height;
        pstWndData->stBufInfo.u32Pitch = ptWinAttr->u16Width;
        pstWndData->stBufInfo.u32ColorFmt = eColorFmt;
    }

    pstWndData->bUsed = TRUE;

    return (MsMW_WinHandle)(MS_U32)u8Idx;
}

static MS_BOOL _SubRnd_DestroyWindow(MsMW_WinHandle hWinHnd, Sub_ObjHandle hSubObj)
{
    MS_U8 u8Idx = (MS_U8)(MS_U32)hWinHnd;

    if((u8Idx >= SUB_MAX_SUPPORT_WIN) || (_gstWinData[u8Idx].bUsed == FALSE))
    {
        return FALSE;
    }

    if(_gstWinData[u8Idx].bDisplayAble == TRUE)
    {
        _SubOSD_Free();
    }

    memset((void *)&_gstWinData[u8Idx], 0, sizeof(ST_MMSDK_SUBTITLE_WININFO));

    return TRUE;
}

static MS_BOOL _SubRnd_SetPalette(MsMW_WinHandle hWinHnd, MS_U8* pPalArray, MS_U16 u16PaletteSize, MS_U8 u8Index, Sub_ObjHandle hSubObj)
{
    MS_U8 u8Idx = (MS_U8)(MS_U32)hWinHnd;

    if((u8Idx >= SUB_MAX_SUPPORT_WIN) || (_gstWinData[u8Idx].bDisplayAble == FALSE))
    {
        return FALSE;
    }

    return ((_SubOSD_PaletteSet(pPalArray, (MS_U32)u16PaletteSize, (MS_U32)u8Index) == 0) ? FALSE : TRUE);
}

static MS_BOOL _SubRnd_Bitblt(MsMW_DrawRect *pstRect, MsMW_BitbltParam *pstParam, Sub_ObjHandle hSubObj)
{
    MS_U8 u8SrcWinIdx = (MS_U8)(MS_U32)pstParam->hSrcWin;
    MS_U8 u8DstWinIdx = (MS_U8)(MS_U32)pstParam->hDstWin;
    MS_U16 u16DispX = 0;
    MS_U16 u16DispY = 0;
    MS_U16 u16DispWidth = 0;
    MS_U16 u16DispHeight = 0;
    MS_BOOL bHScale = FALSE;
    MS_BOOL bVScale = FALSE;
    ST_MMSDK_SUBTITLE_WININFO *pstSrc;
    ST_MMSDK_SUBTITLE_WININFO *pstDst;
    GFX_DrawRect stRect;
    ST_MMSDK_GFX_BitbltParam stParam={0};

    PT_SBTL_FLOW("u8SrcWinIdx = %d, u8DstWinIdx = %d\n", u8SrcWinIdx, u8DstWinIdx);

    if ((u8SrcWinIdx >= SUB_MAX_SUPPORT_WIN) ||
        (u8DstWinIdx >= SUB_MAX_SUPPORT_WIN) ||
        (_gstWinData[u8SrcWinIdx].bUsed == FALSE) ||
        (_gstWinData[u8DstWinIdx].bUsed == FALSE))
    {
        return FALSE;
    }

    pstSrc = &_gstWinData[u8SrcWinIdx];
    pstDst = &_gstWinData[u8DstWinIdx];

    /* reset strech win */
    if (pstDst->bDisplayAble == TRUE)
    {
        /* get display size */
        _SubRnd_GetDispSize(&u16DispX, &u16DispY, &u16DispWidth, &u16DispHeight);

        PT_SBTL_FLOW(
            "display info:\n"
            "  u16DispX = %d\n"
            "  u16DispY = %d\n"
            "  u16DispWidth = %d\n"
            "  u16DispHeight = %d\n",
            u16DispX,
            u16DispY,
            u16DispWidth,
            u16DispHeight);

        bHScale = ((u16DispWidth != pstDst->stBufInfo.u32Width)?TRUE:FALSE);
        bVScale = ((u16DispHeight != pstDst->stBufInfo.u32Height)?TRUE:FALSE);

        _SubOSD_ResulotionSet(
            u16DispX,
            u16DispY,
            pstDst->stBufInfo.u32Width,
            pstDst->stBufInfo.u32Height,
            u16DispWidth,
            u16DispHeight,
            bHScale,
            bVScale);
    }

    /* call porting layer bitblt */
    stRect.srcblk.x = pstRect->stSrcRect.u16X;
    stRect.srcblk.y = pstRect->stSrcRect.u16Y;
    stRect.srcblk.width = pstRect->stSrcRect.u16Width;
    stRect.srcblk.height = pstRect->stSrcRect.u16Height;

    stRect.dstblk.x = pstRect->stDstRect.u16X;
    stRect.dstblk.y = pstRect->stDstRect.u16Y;
    stRect.dstblk.width = pstRect->stDstRect.u16Width;
    stRect.dstblk.height = pstRect->stDstRect.u16Height;

    stParam.pstSrcBuf = &pstSrc->stBufInfo;
    stParam.pstDstBuf = &pstDst->stBufInfo;

    /* alpha mapping */
    stParam.stAlpha.bEnable = pstParam->stAlpha.bEnable;
    stParam.stAlpha.eCoef = pstParam->stAlpha.eCoef;
    stParam.stAlpha.u8Const = pstParam->stAlpha.u8Const;
    stParam.stAlpha.eAlphaSrc = pstParam->stAlpha.eAlphaSrc;

    /* src/dst color key mapping */
    stParam.stSrcClrKey.bEnable = pstParam->stSrcClrKey.bEnable;
    stParam.stSrcClrKey.eOpMode = pstParam->stSrcClrKey.eOpMode;
    stParam.stSrcClrKey.eFmt = pstParam->stSrcClrKey.eFmt;
    stParam.stSrcClrKey.pvColorStart = pstParam->stSrcClrKey.pvColorStart;
    stParam.stSrcClrKey.pvColorEnd = pstParam->stSrcClrKey.pvColorEnd;

    stParam.stDstClrKey.bEnable = pstParam->stDstClrKey.bEnable;
    stParam.stDstClrKey.eOpMode = pstParam->stDstClrKey.eOpMode;
    stParam.stDstClrKey.eFmt = pstParam->stDstClrKey.eFmt;
    stParam.stDstClrKey.pvColorStart = pstParam->stDstClrKey.pvColorStart;
    stParam.stDstClrKey.pvColorEnd = pstParam->stDstClrKey.pvColorEnd;
    PT_SBTL_FLOW("\n");

    /* palette mapping */
    if (pstParam->stPalette.pPalArray)
    {
        stParam.stPalette.pPalArray = (GFX_PaletteEntry *)pstParam->stPalette.pPalArray;
        stParam.stPalette.u16PalStart = pstParam->stPalette.u16PalStart;
        stParam.stPalette.u16PalStart = pstParam->stPalette.u16PalStart;
    }

    /* clip mapping */
    stParam.stClipRect.x = pstParam->stClipRect.u16X;
    stParam.stClipRect.y = pstParam->stClipRect.u16Y;
    stParam.stClipRect.width = pstParam->stClipRect.u16Width;
    stParam.stClipRect.height = pstParam->stClipRect.u16Height;

    if (MApi_GFX_BeginDraw() != GFX_SUCCESS)
    {
        return FALSE;
    }

    MApi_GFX_SetNearestMode(TRUE);

    if (_GFX_BitBlt(&stRect, &stParam) != GFX_SUCCESS)
    {
        return FALSE;
    }

    MApi_GFX_SetNearestMode(FALSE);

    MApi_GFX_EndDraw();

    return TRUE;
}

static MS_BOOL _SubRnd_ClearWindowByColor(MsMW_WinHandle hWinHnd, MsMW_Rect *pRect, MsMW_PaletteColour *pColor, Sub_ObjHandle hSubObj)
{
    MS_U8 u8Idx = (MS_U8)(MS_U32)hWinHnd;
    ST_MMSDK_SUBTITLE_WININFO *pstWinData;
    GFX_Point ptV0;
    GFX_Point ptV1;
    GFX_RectFillInfo bInfo;

    pstWinData = &_gstWinData[u8Idx];

    if (MApi_GFX_BeginDraw() != GFX_SUCCESS)
    {
        return FALSE;
    }
    ptV0.x = 0;
    ptV0.y = 0;
    ptV1.x = pstWinData->stBufInfo.u32Width;
    ptV1.y = pstWinData->stBufInfo.u32Height;
    MApi_GFX_SetClip(&ptV0, &ptV1);

    MApi_GFX_SetDstBufferInfo(&pstWinData->stBufInfo, 0);

    MApi_GFX_SetNearestMode(FALSE); //default setting

    //_MApi_Osd_SetAlphaBlending (pAlpha_Blending);
    bInfo.dstBlock.x = pRect->u16X;
    bInfo.dstBlock.y = pRect->u16Y;
    bInfo.dstBlock.width  = pRect->u16Width;
    bInfo.dstBlock.height = pRect->u16Height;

    bInfo.fmt = pstWinData->stBufInfo.u32ColorFmt;
    bInfo.colorRange.color_s.a = pColor->u8A;
    bInfo.colorRange.color_s.r = pColor->u8R;
    bInfo.colorRange.color_s.g = pColor->u8G;
    bInfo.colorRange.color_s.b = pColor->u8B;
    bInfo.flag = GFXRECT_FLAG_COLOR_CONSTANT;
    bInfo.colorRange.color_e.a = pColor->u8A;
    bInfo.colorRange.color_e.r = pColor->u8R;
    bInfo.colorRange.color_e.g = pColor->u8G;
    bInfo.colorRange.color_e.b = pColor->u8B;

    MApi_GFX_RectFill(&bInfo);
    MApi_GFX_EndDraw();

    return TRUE;
}

static MS_BOOL _SubRnd_ClearWindowByIndex(MsMW_WinHandle hWinHnd, MsMW_Rect *pRect, MS_U8 u8ColorIndex, Sub_ObjHandle hSubObj)
{
    MS_U8 u8Idx = (MS_U8)(MS_U32)hWinHnd;
    MsMW_PaletteColour stColor;

    if((u8Idx >= SUB_MAX_SUPPORT_WIN) || (_gstWinData[u8Idx].bUsed== FALSE))
    {
        return FALSE;
    }

    switch(_gstWinData[u8Idx].stBufInfo.u32ColorFmt)
    {
    case GFX_FMT_I1:
    case GFX_FMT_I2:
    case GFX_FMT_I4:
    case GFX_FMT_I8:
        break;
    default:
        return FALSE;
    }

    stColor.u8A = 0;
    stColor.u8R = 0;
    stColor.u8G = 0;
    stColor.u8B = u8ColorIndex;
    return _SubRnd_ClearWindowByColor(hWinHnd, pRect, &stColor, hSubObj);
}

static MS_BOOL _SubRnd_GetDispalySize(MsMW_Rect *pRect, void *pvArg)
{
    return _SubRnd_GetDispSize(&pRect->u16X, &pRect->u16Y, &pRect->u16Width, &pRect->u16Height);
}

static MS_BOOL _SubRnd_Show(MsMW_WinHandle hWinHnd, MS_BOOL bShow, Sub_ObjHandle hSubObj)
{
    MS_U8 u8Idx = (MS_U8)(MS_U32)hWinHnd;
    ST_MMSDK_SUBTITLE_WININFO *pstWndData;

    pstWndData = &_gstWinData[u8Idx];

    if(pstWndData->bDisplayAble == TRUE)
    {
        return ((_SubOSD_Enable(bShow) == 0) ? FALSE : TRUE);
    }

    return TRUE;
}

static MS_BOOL _SubRnd_Open(Sub_ObjHandle hSubObj)
{
    return TRUE;
}

static MS_BOOL _SubRnd_Close(Sub_ObjHandle hSubObj)
{
    MS_U8 u8Idx;

    for(u8Idx=0; u8Idx<SUB_MAX_SUPPORT_WIN; u8Idx++)
    {
        if(_gstWinData[u8Idx].bUsed)
        {
            _SubRnd_DestroyWindow((MsMW_WinHandle)(MS_U32)u8Idx, NULL);
            _gstWinData[u8Idx].bUsed = FALSE;
        }
    }

    return TRUE;
}

static MS_BOOL _Subtitle_RegisterCB(MsMW_RndOp_t *pstRndOp,MsMW_SubMemOp_t *pstSubtitleMemOp)
{
    eSub_Type eType = SUB_TYPE_DVB;

    if (!_gpstSub)
    {
        if((_gpstSub = msAPI_Subtitle_CreateObject(eType)) == INVLID_DEC_HANDLE)
        {
            PT_SBTL_ERR("Create Object Fail!\n");
            return FALSE;
        }
    }

    /* init decoder */

    MsMW_RndOp_t subRndOP={0};
    stSub_Param stInitParam={0};

    subRndOP.pRndOpen = pstRndOp->pRndOpen?(MsMW_Render_Open)pstRndOp->pRndOpen:(MsMW_Render_Open)_SubRnd_Open;
    subRndOP.pRndClose = pstRndOp->pRndClose?(MsMW_Render_Close)pstRndOp->pRndClose:(MsMW_Render_Close)_SubRnd_Close;
    subRndOP.pRndCreateWindow = pstRndOp->pRndCreateWindow?(MsMW_Render_CreateWindow)pstRndOp->pRndCreateWindow:(MsMW_Render_CreateWindow)_SubRnd_CreateWindow;
    subRndOP.pRndDestroyWindow = pstRndOp->pRndDestroyWindow?(MsMW_Render_DestroyWindow)pstRndOp->pRndDestroyWindow:(MsMW_Render_DestroyWindow)_SubRnd_DestroyWindow;
    subRndOP.pRndSetPalette = pstRndOp->pRndSetPalette?(MsMW_Render_SetPalette)pstRndOp->pRndSetPalette:(MsMW_Render_SetPalette)_SubRnd_SetPalette;
    subRndOP.pRndBitblt = pstRndOp->pRndBitblt?(MsMW_Render_Bitblt)pstRndOp->pRndBitblt:(MsMW_Render_Bitblt)_SubRnd_Bitblt;
    subRndOP.pRndClearWindowByIndex = pstRndOp->pRndClearWindowByIndex?(MsMW_Render_ClearWindowByIndex)pstRndOp->pRndClearWindowByIndex:(MsMW_Render_ClearWindowByIndex)_SubRnd_ClearWindowByIndex;
    subRndOP.pRndClearWindowByColor = pstRndOp->pRndClearWindowByColor?(MsMW_Render_ClearWindowByColor)pstRndOp->pRndClearWindowByColor:(MsMW_Render_ClearWindowByColor)_SubRnd_ClearWindowByColor;
    subRndOP.pRndGetDisplaySize = pstRndOp->pRndGetDisplaySize?(MsMW_Render_GetDispalySize)pstRndOp->pRndGetDisplaySize:(MsMW_Render_GetDispalySize)_SubRnd_GetDispalySize;
    subRndOP.pRndShow = pstRndOp->pRndShow?(MsMW_Render_Show)pstRndOp->pRndShow:(MsMW_Render_Show)_SubRnd_Show;

    /* fill initial parameters */
    /* These parameters ars copied from Fuchsia2.0 source code,  in the msAPI_Subtitle_Init function */
    /* we need these paremeters to give msAPI_Subtitle_Init_EX intial value */
    stInitParam.bAllocFB = TRUE;
    stInitParam.u32FrmBufSize = 960*540;

    if(msAPI_Subtitle_Init_EX((Sub_ObjHandle)_gpstSub, &stInitParam, &subRndOP, (MsMW_SubMemOpT2_t*)pstSubtitleMemOp) == FALSE)
    {
        /* destroy decoder */
        if(msAPI_Subtitle_ReleaseObject(_gpstSub))
        {
            _gpstSub = NULL;
        }

        PT_SBTL_ERR("Subtitle Init Fail!\n");
        return FALSE;
    }
    if ((_dvbSubQueObj = (stQueueObj*)msAPI_Subtitle_GetQueueObj(_gpstSub)) == NULL )
    {
        PT_SBTL_ERR("Get Queue Object Fail!\n");
        return FALSE;
    }

    return TRUE;
}

static void _SubtitleGetAVPSTC(MS_U32 *pu32STCHigh, MS_U32 *pu32STC)
{
       if(_pCallBack && _pMMSDKClass)
       {
           _pCallBack(_pMMSDKClass, E_MMSDK_SUBTITLE_CB_GET_CURRENT_PTS, pu32STC, NULL);
       }
       *pu32STCHigh = 0;
}
#endif

static MS_BOOL _Subtitle_UTF8StingCoverUSC2String(MS_U8* pu8Src, MS_U16* pu16Dst, MS_U16 u16UnicodeStringlen)
{
    MS_U16  temp1,
            temp2;
    MS_U8   *in              = pu8Src;
    MS_U16  i                = 0;

    if (!pu8Src || !pu16Dst)
    {
        return TRUE;
    }

    while (*pu8Src != 0x00 && i < u16UnicodeStringlen)
    {
        if (0x00 == (*in & 0x80))
        {
            /* 1 byte UTF-8 Charater.*/
            *pu16Dst = (MS_U16) * in;
            in += 1;
        }
        else if (0xc0 == (*in & 0xe0)
                 && 0x80 == (*(in + 1) & 0xc0))
        {
            /* 2 bytes UTF-8 Charater.*/
            temp1 = (MS_U16)(*in & 0x1f);
            temp1 <<= 6;
            temp1 |= (MS_U16)(*(in + 1) & 0x3f);
            *pu16Dst = temp1;
            in += 2;
        }
        else if (0xe0 == (*in & 0xf0)
                 && 0x80 == (*(in + 1) & 0xc0)
                 && 0x80 == (*(in + 2) & 0xc0))
        {
            /* 3bytes UTF-8 Charater.*/
            temp1 = (MS_U16)(*in & 0x0f);
            temp1 <<= 12;
            temp2 = (MS_U16)(*(in + 1) & 0x3F);
            temp2 <<= 6;
            temp1 = temp1 | temp2 | (MS_U16)(*(in + 2) & 0x3F);
            *pu16Dst = temp1;
            in += 3;
        }
        else
        {
            /* unrecognize byte. */
            *pu16Dst = 0x22e0;
            in += 1;
        }

        i++;
        pu16Dst++;
    }

    return TRUE;
}

static void _Subtitle_DrawText(MS_U16 *pu16Text, MS_U16 u16Len)
{
    MS_U8 u8TextLen = 0;
    MS_U8 i=0;
    //just safe to use
    if (u16Len > LINE_TEXT_MAX)
    {
        u16Len = LINE_TEXT_MAX-1;
    }
    //pBuffer is I8 Mode
    for( i = 0; i< u16Len; i++)
    {
        _u8PrintText[u8TextLen++] = pu16Text[i] < 0x80 ? (MS_U8)pu16Text[i]:'*';
        if(pu16Text[i] == 0)
        {
            break;
        }
    }
    printf("%s\n", _u8PrintText);
}

static void _SubtitleDecoderTaskProcess(_ST_MMSDK_SUBTITLE_PT * pSubtitle )
{
    if(pSubtitle == NULL)
    {
        PT_SBTL_ERR("pSubtitle is NULL!\n");
        return;
    }

    if(pSubtitle->u32SubtitleType == E_MMSDK_SUBTITLE_TYPE_NONE)
    {
        PT_SBTL_ERR("u32SubtitleType is NONE!\n");
        return;
    }
    PT_SBTL_INFO("_SubtitleDecoderTaskProcess Running [%d]\n",pSubtitle->u32SubtitleType);
    while(!pSubtitle->bThreadDestroy)
    {
        pSubtitle->bSubtitleDecoding = TRUE;
        PT_Thread_MutexLock(pSubtitle->pstMutex);
        switch(pSubtitle->u32SubtitleType)
        {
            case E_MMSDK_SUBTITLE_TYPE_IMAGE_BMP:
            {
                // Internal Subtitle
                MMSDK_U32 u32CurrentPTS = 0;
                if(_pCallBack && _pMMSDKClass)
                {
                    _pCallBack(_pMMSDKClass, E_MMSDK_SUBTITLE_CB_GET_CURRENT_PTS, &u32CurrentPTS, NULL);
                }
                msAPI_MpegSP_Main(_pstNewSubtitleItem ? (u32CurrentPTS) : (u32CurrentPTS/90));

                break;
            }
            case E_MMSDK_SUBTITLE_TYPE_IMAGE_VOBSUB:
            {
                // Internal Subtitle
                MMSDK_U32 u32CurrentPTS = 0;
                if(_pCallBack && _pMMSDKClass)
                {
                    _pCallBack(_pMMSDKClass, E_MMSDK_SUBTITLE_CB_GET_CURRENT_PTS, &u32CurrentPTS, NULL);
                }
                msAPI_MpegSP_Main(u32CurrentPTS);

                break;
            }
            case E_MMSDK_SUBTITLE_TYPE_TEXT_SRT:
            case E_MMSDK_SUBTITLE_TYPE_TEXT_UTF8:
            case E_MMSDK_SUBTITLE_TYPE_TEXT_ASS:
            case E_MMSDK_SUBTITLE_TYPE_TEXT_SSA:
            {
#if ENABLE_FREE_TYPE
                if(_bSubtitleWithTTF)
                {
                    MMSDK_U32 u32CurrentPTS = 0;
                    if(_pCallBack && _pMMSDKClass)
                    {
                        _pCallBack(_pMMSDKClass, E_MMSDK_SUBTITLE_CB_GET_CURRENT_PTS, &u32CurrentPTS, NULL);
                    }
                    msAPI_MpegSP_Main(u32CurrentPTS/90);
                }
                else
#endif
                {
                    MMSDK_U32 u32CurrentPTS = 0;
                    MS_U32 u32SubBuff;
                    MS_U16 u16TagIdx;
                    MPlayer_Subtitle_Tag stTag;

                    if(_pCallBack && _pMMSDKClass)
                    {
                        _pCallBack(_pMMSDKClass, E_MMSDK_SUBTITLE_CB_GET_CURRENT_PTS, &u32CurrentPTS, NULL);
                    }

                    u32SubBuff = MemAlign(MP4_SUBTITLE_BUFFERADR(), 0x01000);
                    u32SubBuff = _PA2VA(u32SubBuff);
                    if(u32SubBuff == 0)
                    {
                        break;
                    }

                    u16TagIdx = msAPI_Text_GetTextSubtitleTag(&stTag, u32CurrentPTS/90);

                    if(u16TagIdx != 0xFFFF)
                    {
                        memset((void *)_u16PrintText, 0, sizeof(MS_U16)*LINE_TEXT_MAX);
                        if(_Subtitle_UTF8StingCoverUSC2String((MS_U8*)(u32SubBuff + stTag.u32SubtitlePos), _u16PrintText, LINE_TEXT_MAX) != FALSE)
                        {
                            printf("[MMSDK Subtitle] Start.\n");
                            _Subtitle_DrawText(_u16PrintText, LINE_TEXT_MAX);
                            printf("[MMSDK Subtitle] End.\n");
                        }
                    }
                }
                break;
            }
            case E_MMSDK_SUBTITLE_TYPE_IMAGE_TS_BMP:
            default:
                break;
        }
        PT_Thread_MutexUnlock(pSubtitle->pstMutex);
        if(pSubtitle->bThreadDestroy)
        {
            PT_SBTL_INFO("_SubtitleDecoderTaskProcess EXIT\n");
            pSubtitle->bSubtitleDecoding = FALSE;
            break;
        }
        MsOS_DelayTask(AVALIABLE_TIME_TO_FREE_RUN);
        pSubtitle->bSubtitleDecoding = FALSE;
    }
    PT_Thread_ThreadExit(NULL);
    return;

}

static void * _SubtitleDecoderTask(void * arg)
{
    /*-> add your code here */
    _ST_MMSDK_SUBTITLE_PT * pSubtitle = (_ST_MMSDK_SUBTITLE_PT *)arg;
    PT_SBTL_INFO("_SubtitleDecoderTask Enters\n");
    PT_Thread_ThreadSetName(SBTL_PTHREAD_NAME);

    if(!pSubtitle)
    {
         PT_SBTL_ERR("_SubtitleDecoderTask pSubtitle Null");
         return NULL;
    }

    pSubtitle->u32TaskRet = 0;
    pSubtitle->bThreadDestroy = FALSE;
    _SubtitleDecoderTaskProcess(pSubtitle);

    return (void *) &pSubtitle->u32TaskRet;
}

#if (SBTL_TTF_DUMP && ENABLE_FREE_TYPE)
static void _SubtitleTTFDump(MS_PHY phyFontTableAddr,MS_U32 u32FontTableSize)
{
    MS_S32 s32FileID=-1;
    MS_U8 u8Name[64] = {0};
    sprintf((char *)u8Name, "F08_DP_HD111_rtl_00_Track%d.ttf", _u8SubtitleTrackID);
    U16 u16Name[64] = {0};
    MApi_U8StringToU16String((MS_U8*)u8Name, u16Name, (MS_U8)strlen(u8Name));
    s32FileID =MApi_FS_Open((char*)u16Name,O_WRONLY|O_CREAT);
    if(s32FileID > 0)
    {
        PT_SBTL_INFO("\n ==================MApi_FS is Ready to write TTF TrackID[%]================\n",_u8SubtitleTrackID);
        MApi_FS_Write(s32FileID, (void *) phyFontTableAddr, u32FontTableSize);
        MApi_FS_Close(s32FileID);
        PT_SBTL_INFO("\n ==================MApi_FS Has Successfully write TTF TrackID[%d]===========\n",_u8SubtitleTrackID);
        _u8SubtitleTrackID++;
    }
    else
    {
        PT_SBTL_INFO("\n MApi_FS Failed to write[%d]\n",_u8SubtitleTrackID);
    }
}
#endif

static MMSDK_BOOL _SubtitleOpen(EN_MMSDK_SUBTITLE_TYPE eType)
{

    PT_SBTL_INFO("_SubtitleOpen\n");
    _pstPTSubtitle = (_ST_MMSDK_SUBTITLE_PT *)PT_MsOS_AllocateMemory(sizeof(_ST_MMSDK_SUBTITLE_PT));
    if(_pstPTSubtitle)
    {
        memset(_pstPTSubtitle, 0x00, sizeof(_ST_MMSDK_SUBTITLE_PT));
        _pstPTSubtitle->u32SubtitleType = eType;
        _pstPTSubtitle->bThreadDestroy = TRUE;
    }
    else
    {
        PT_SBTL_ERR("malloc _pstPTSubtitle fail in.\n");
        return FALSE;
    }

    if(!_pstPTSubtitle->bMutexCreate)
    {
        MMSDK_BOOL bThreadInit = FALSE;
        bThreadInit = PT_Thread_MutexInit( &_pstPTSubtitle->pstMutex);
        if(bThreadInit != TRUE)
        {
            PT_SBTL_ERR("Create mutex fail in .\n");
            _pstPTSubtitle->bMutexCreate = FALSE;
            goto OPEN_ERROR;
        }

        _pstPTSubtitle->bMutexCreate = TRUE;
    }

    /* Create Decoder Task*/
    if((!_pstPTSubtitle->bThreadCreate)
        && (_pstPTSubtitle->u32SubtitleType != E_MMSDK_SUBTITLE_TYPE_IMAGE_TS_BMP))
    {
        _pstPTSubtitle->bThreadDestroy = TRUE;
        _pstPTSubtitle->pStackAddr = PT_MsOS_AllocateMemory(SBTL_PTHREAD_STACK_SIZE);
        if( _pstPTSubtitle->pStackAddr != NULL)
        {
            _pstPTSubtitle->bThreadCreate = PT_Thread_ThreadCreate(
                                    &_pstPTSubtitle->u32ThreadId,
                                    _SubtitleDecoderTask,
                                    (void*)_pstPTSubtitle,
                                    E_MMSDK_TASK_PRI_HIGH,
                                    _pstPTSubtitle->pStackAddr,
                                    SBTL_PTHREAD_STACK_SIZE,
                                    SBTL_PTHREAD_NAME);

            if(_pstPTSubtitle->bThreadCreate == FALSE)
            {
                PT_SBTL_ERR("Create thread fail in.\n");
                _pstPTSubtitle->bThreadDestroy = TRUE;
                goto OPEN_ERROR;
            }

        }
        else
        {
            PT_SBTL_ERR("Failed to Malloc Thread Stack .\n");
            goto OPEN_ERROR;
        }

    }

    return TRUE;

OPEN_ERROR:
    if(_pstPTSubtitle)
    {
        if(_pstPTSubtitle->pStackAddr)
        {
            PT_MsOS_FreeMemory(_pstPTSubtitle->pStackAddr);
            _pstPTSubtitle->pStackAddr = NULL;
        }
        //Free the Thread Stack if Thread create Failed
        if(_pstPTSubtitle->bMutexCreate)
        {
            PT_Thread_MutexDestroy(&_pstPTSubtitle->pstMutex);
            _pstPTSubtitle->bMutexCreate = FALSE;
        }

        PT_MsOS_FreeMemory(_pstPTSubtitle);
        _pstPTSubtitle = NULL;
    }
    return FALSE;
}

static MMSDK_BOOL _SubtitleClose(void)
{
    PT_SBTL_INFO("_bSubtitleDecoding[%d]\n",_pstPTSubtitle->bSubtitleDecoding);

    if(_pstPTSubtitle)
    {
        //Destroy the Thread
        if(_pstPTSubtitle->bThreadCreate)
        {
             _pstPTSubtitle->bThreadDestroy = TRUE;
            PT_Thread_ThreadJoin(_pstPTSubtitle->u32ThreadId);
            _pstPTSubtitle->bThreadCreate = FALSE;
        }
        //Destroy the Mutex
        if(_pstPTSubtitle->bMutexCreate)
        {
            PT_Thread_MutexDestroy(&_pstPTSubtitle->pstMutex);
            _pstPTSubtitle->bMutexCreate = FALSE;
        }

        if(_pstPTSubtitle->pStackAddr)
        {
            PT_MsOS_FreeMemory(_pstPTSubtitle->pStackAddr);
            _pstPTSubtitle->pStackAddr = NULL;
        }

        PT_MsOS_FreeMemory(_pstPTSubtitle);
        _pstPTSubtitle = NULL;
    }
    return TRUE;
}

//Check if it's decoding now, wait at most 1s if yes
static MMSDK_BOOL  _SubtitleDecoderTaskCheck(void)
{
        MMSDK_U8 u8WaitCnt = 0;
        MMSDK_BOOL bRet = TRUE;
        PT_SBTL_INFO(" _bSubtitleDecoding [%d]\n",_pstPTSubtitle->bSubtitleDecoding);
        if(_pstPTSubtitle == NULL)
        {
            PT_SBTL_INFO("no subtitle decoding task been created\n");
            return TRUE;
        }
        _pstPTSubtitle->bThreadDestroy = TRUE;
        while(_pstPTSubtitle->bSubtitleDecoding)
        {
            if(u8WaitCnt >=10)
            {
                PT_SBTL_ERR("_bSubtitleDecoding Timeout!!\n");
                _pstPTSubtitle->bSubtitleDecoding = FALSE;
                bRet = FALSE;
                break;
            }
            MsOS_DelayTask(100);
            u8WaitCnt++;
        }
        return bRet;
}

static MS_BOOL _SubMemClean(EN_MMSDK_SUBTITLE_TYPE eType)
{
    MMPlayerMemSet MM_Mem={};

    msAPI_Memory_SetPoolID(-1);
    MM_Memory_Config(&MM_Mem);

    if (_gu32VDPlayerMP4SUBBUFMEM)
    {
        MsOS_FreeMemory((void*)_gu32VDPlayerMP4SUBBUFMEM, PT_SYS_GetNonCachePoolID());
        _gu32VDPlayerMP4SUBBUFMEM = 0;
    }

    if (_gu32VDPlayerMP4SUBTAGMEM)
    {
        MsOS_FreeMemory((void*)_gu32VDPlayerMP4SUBTAGMEM, PT_SYS_GetNonCachePoolID());
        _gu32VDPlayerMP4SUBTAGMEM = 0;
    }

    if (_gu32VDPlayerSUBMEM)
    {
        MsOS_FreeMemory((void*)_gu32VDPlayerSUBMEM, PT_SYS_GetNonCachePoolID());
        _gu32VDPlayerSUBMEM = 0;
    }

    return TRUE;
}

static MS_BOOL _SubMemSetup(EN_MMSDK_SUBTITLE_TYPE eType)
{
    MMPlayerMemSet MM_Mem=
        {
            .MemorySize = MM_MEM_CFG,//Fuchsia's problem
            .MemoryMIU1Flag = MMPLAYER_MIU1_UNUSE,//Fuchsia's problem
        };

    switch(eType)
    {
#if (MM_MEM_CFG <= MMPLAYERMEMSIZE_64M)
        case E_MMSDK_SUBTITLE_TYPE_IMAGE_BMP:
        case E_MMSDK_SUBTITLE_TYPE_IMAGE_VOBSUB:
        case E_MMSDK_SUBTITLE_TYPE_TEXT_SRT:
        case E_MMSDK_SUBTITLE_TYPE_TEXT_UTF8:
        case E_MMSDK_SUBTITLE_TYPE_TEXT_ASS:
        case E_MMSDK_SUBTITLE_TYPE_TEXT_SSA:
            if (_gu32VDPlayerMP4SUBBUFMEM == (MS_U32)NULL)
            {
                _gu32VDPlayerMP4SUBBUFMEM = (MS_U32)MsOS_AllocateMemory(MP4_SUBTITLE_BUFFER_LEN, PT_SYS_GetNonCachePoolID());
            }

            if (_gu32VDPlayerMP4SUBTAGMEM == (MS_U32)NULL)
            {
                _gu32VDPlayerMP4SUBTAGMEM = (MS_U32)MsOS_AllocateMemory(MP4_SUBTITLE_TAG_LEN, PT_SYS_GetNonCachePoolID());
            }

            if(_gu32VDPlayerMP4SUBBUFMEM == (MS_U32)NULL || _gu32VDPlayerMP4SUBTAGMEM == (MS_U32)NULL)
            {
                _SubMemClean(eType);
                return FALSE;
            }

            MM_Mem.u32MP4SubtitleBufAddr = MsOS_VA2PA(_gu32VDPlayerMP4SUBBUFMEM);
            MM_Mem.u32MP4SubtitleBufSize = MP4_SUBTITLE_BUFFER_LEN;

            MM_Mem.u32MP4SubtitleTAGAddr = MsOS_VA2PA(_gu32VDPlayerMP4SUBTAGMEM);
            MM_Mem.u32MP4SubtitleTAGSize = MP4_SUBTITLE_TAG_LEN;

            msAPI_Memory_SetPoolID(PT_SYS_GetNonCachePoolID());
            MM_Memory_Config(&MM_Mem);
            break;
#else
        case E_MMSDK_SUBTITLE_TYPE_IMAGE_BMP:
        case E_MMSDK_SUBTITLE_TYPE_IMAGE_VOBSUB:
        case E_MMSDK_SUBTITLE_TYPE_TEXT_SRT:
        case E_MMSDK_SUBTITLE_TYPE_TEXT_UTF8:
        case E_MMSDK_SUBTITLE_TYPE_TEXT_ASS:
        case E_MMSDK_SUBTITLE_TYPE_TEXT_SSA:
            if(_gu32VDPlayerSUBMEM == (MS_U32)NULL)
            {
                _gu32VDPlayerSUBMEM = MsOS_VA2PA((MS_U32)MsOS_AllocateMemory(SUBTITLE_BUFFER_LEN, PT_SYS_GetNonCachePoolID()));
            }

            if(_gu32VDPlayerSUBMEM == (MS_U32)NULL)
            {
                return FALSE;
            }
            else
            {
                MM_Mem.u32SubtitleAddr = _gu32VDPlayerSUBMEM;
                MM_Mem.u32SubtitleSize = SUBTITLE_BUFFER_LEN;
            }

            msAPI_Memory_SetPoolID(PT_SYS_GetNonCachePoolID());
            MM_Memory_Config(&MM_Mem);
            break;
#endif
        case E_MMSDK_SUBTITLE_TYPE_IMAGE_TS_BMP: /* support but do nothing */
            break;
        default:
            return FALSE;
        }

    return TRUE;
}

/*------------------External Functions---------------------------------------------------------------------------*/

MMSDK_BOOL PT_Subtitle_Initialize(void* pClass,  const pfnMmsdkSubtitleCallback pCallback)
{
    PT_SBTL_INFO("PT_Subtitle_Initialize Runing\n");

    if(_pstNewSubtitleItem != NULL)
    {
        _pCallBack = _pstNewSubtitleItem->pCallBack;
        _pMMSDKClass = _pstNewSubtitleItem->pCallbackClass;
    }
    else
    {
        _pCallBack = pCallback;
        _pMMSDKClass = pClass;
    }

#if ENABLE_FREE_TYPE
    _bSubtitleWithTTF = FALSE;
#endif

    return TRUE;
}

MMSDK_BOOL PT_Subtitle_Finalize()
{
    PT_SBTL_INFO("PT_Subtitle_Finalize\n");
    _pCallBack = NULL;
    _pMMSDKClass = NULL;
    return TRUE;
}


MMSDK_BOOL PT_SubtitlePalette(const ST_MMSDK_SET_SUBTITLE_PALETTE_INFO *pstInfo)
{
    PT_SBTL_INFO("PT_SubtitlePalette Enters\n");
    return TRUE;
}

MMSDK_BOOL PT_SubtitleInit(const ST_MMSDK_SUBTITLE_INIT_INFO *pstInfo)
{
    MMSDK_BOOL bRet = TRUE;
    PT_SBTL_INFO("PT_SubtitleInit Enters with[%d]\n",pstInfo->eType);

    //INIT And the Decoder Task has not been started yet
    if(/*(pstInfo->bInit)  &&*/ (_bSubtitleInitialized == FALSE) )
    {
        if(_SubMemSetup(pstInfo->eType) == FALSE)
        {
            PT_SBTL_ERR("Memory setup fail.\n");
            return FALSE;
        }

        switch(pstInfo->eType)
        {
            case E_MMSDK_SUBTITLE_TYPE_IMAGE_TS_BMP:
                {
                    if(_SubtitleOpen(pstInfo->eType) == FALSE)
                    {
                        PT_SBTL_ERR("_SubtitleOpen Failed\n");
                        _SubtitleClose();
                        return FALSE;
                    }
                    MsMW_SubMemOp_t MemOp={0};
                    MemOp.pAllocCache  =  (MsMW_Alloc)_SubMem_Alloc_Cache;
                    MemOp.pAllocNonCache  =  (MsMW_Alloc)_SubMem_Alloc_NonCache;
                    MemOp.pFreeCache  =  (MsMW_Free)_SubMem_Free_Cache;
                    MemOp.pFreeNonCache  =  (MsMW_Free)_SubMem_Free_NonCache;
                    MemOp.pPa2Va  =  (MsMW_PA2VA)_SubMem_PA2VA;
                    MemOp.pVa2Pa  =  (MsMW_VA2PA)_SubMem_VA2PA;

                    PT_Thread_MutexLock(_pstPTSubtitle->pstMutex);
#ifdef DVB_SUBTITLE_V2
                    MPL_SYS_SetMemPoolID(PT_SYS_GetCachePoolID(), PT_SYS_GetNonCachePoolID());
                    _Subtitle_RegisterCB((MsMW_RndOp_t*)&gstSysSubRndFunPtr,&MemOp);
                    msAPI_Subtitle_DoCmd(_gpstSub, SUB_DECCMD_SET_PAGE_ID, &_gstPageID, sizeof(_ST_MMSDK_SUBTITLE_PAGEID));

                    if(_pstNewSubtitleItem)
                    {
                        MsMW_SysOp_t SysOp={0};
                        SysOp.pfGetSTC = _SubtitleGetAVPSTC;
                        msAPI_Subtitle_DoCmd(_gpstSub, SUB_DECCMD_SET_SYS_OP_INTF, (void *)&SysOp, sizeof(MsMW_SysOp_t));
                    }
#else
                    MsMW_SubOsdOp_t OsdOp={0};
                    OsdOp.pOsdAlloc  =  (MsMW_SubOsdAlloc)_SubOSD_Alloc;
                    OsdOp.pOsdFree =  (MsMW_SubOsdFree)_SubOSD_Free;
                    OsdOp.pOsdReset   =  (MsMW_SubOsdReset)_SubOSD_Reset;
                    OsdOp.pOsdResulotionSet   =  (MsMW_SubOsdResulotionSet)_SubOSD_ResulotionSet;
                    OsdOp.pOsdEnable  =  (MsMW_SubOsdEnable)_SubOSD_Enable;
                    OsdOp.pOsdPaletteSet    =  (MsMW_SubOsdPaletteSet)_SubOSD_PaletteSet;
                    OsdOp.pOsdDestinationSet = (MsMW_SubOsdDestinationSet)_SubOSD_DestinationSet;
                    OsdOp.pOsdStreamIDSet =(MsMW_SubOsdStreamIDSet)_SubOSD_StreamIDSet;
                    msAPI_Subtitle_Init(&OsdOp, &MemOp);
                    msAPI_Subtitle_SetPageIDs(_gstPageID.u16CPID, _gstPageID.u16APID);
#endif
                    PT_Thread_MutexUnlock(_pstPTSubtitle->pstMutex);
                }
                break;
            case E_MMSDK_SUBTITLE_TYPE_IMAGE_BMP:
            case E_MMSDK_SUBTITLE_TYPE_IMAGE_VOBSUB:
            {
                if(_SubtitleOpen(pstInfo->eType) == FALSE)
                {
                    PT_SBTL_ERR("_SubtitleOpen Failed\n");
                    _SubtitleClose();
                    return FALSE;
                }

                PT_Thread_MutexLock(_pstPTSubtitle->pstMutex);

                if(!_check_sc_interlace())
                {
                    PT_SBTL_ERR("Get MApi_VDEC_EX_GetDispInfo failed \n");
                }
                else
                {
                    _GOPSelType = (_scInterlace) ? E_GOP_DST_OP0 : E_GOP_DST_IP0;
                }

                if((_pstNewSubtitleItem) && (_pstNewSubtitleItem->u8DisplayWin != 0))
                {
                    _GOPSelType = E_GOP_DST_OP0;
                }

                msAPI_MpegSP_Set_GOP(E_GOP_Sub);
                msAPI_MpegSP_Set_GOP_Dst_Type(_GOPSelType);
                MApi_GOP_GWIN_OutputLayerSwitch(E_GOP_Sub);
                if(msAPI_MpegSP_Init() == FALSE)
                {
                    PT_SBTL_ERR("msAPI_MpegSP_Init Failed\n");
                    return FALSE;
                }

                // Choose the correct MIU
                MS_PHY phySubtitleBaseAddr = MsOS_VA2PA((MS_VIRT)msAPI_MpegSP_SpuQueue_GetSDRAM_BaseAddress());
                _SubtitleMIUSelection(phySubtitleBaseAddr);

                /*Set Show Status*/
                msAPI_MpegSP_Show();
                if(pstInfo->eType == E_MMSDK_SUBTITLE_TYPE_IMAGE_BMP)
                {
                    msAPI_MpegSP_SetSubtitleDimension(pstInfo->u16BmpWidth, pstInfo->u16BmpHeight);
                }

                PT_Thread_MutexUnlock(_pstPTSubtitle->pstMutex);
                break;
            }
            case E_MMSDK_SUBTITLE_TYPE_TEXT_SRT:
            case E_MMSDK_SUBTITLE_TYPE_TEXT_UTF8:
            case E_MMSDK_SUBTITLE_TYPE_TEXT_ASS:
            case E_MMSDK_SUBTITLE_TYPE_TEXT_SSA:
            {
#if ENABLE_FREE_TYPE
                MS_PHY phyFontTableAddr = 0;
                MS_U32 u32FontTableSize = 0;

                if(_pCallBack && _pMMSDKClass)
                {
                    _pCallBack(_pMMSDKClass, E_MMSDK_SUBTITLE_CB_GET_ATTACHEDFILE_BUFF_ADDR, &phyFontTableAddr, NULL);
                    _pCallBack(_pMMSDKClass, E_MMSDK_SUBTITLE_CB_GET_ATTACHEDFILE_SIZE, &u32FontTableSize, NULL);
                }
                PT_SBTL_INFO("phyFontTableAddr[%x]--u32FontTableSize[%d]\n",(unsigned int)phyFontTableAddr, (unsigned int)u32FontTableSize);
                if((u32FontTableSize != 0) && (phyFontTableAddr != 0))
                {
#if SBTL_TTF_DUMP
                    _SubtitleTTFDump(phyFontTableAddr, u32FontTableSize);
#endif
                    _bSubtitleWithTTF = TRUE;
                    do
                    {
                        if(msAPI_DivXTTF_InitTTF(phyFontTableAddr, u32FontTableSize) == FALSE)
                        {
                            PT_SBTL_ERR("msAPI_DivXTTF_InitTTF Failed\n");
                            msAPI_DivXTTF_DeinitTTF();
                            break;
                        }
                        if(_SubtitleOpen(pstInfo->eType) == FALSE)
                        {
                            PT_SBTL_ERR("_SubtitleOpen Failed\n");
                            _SubtitleClose();
                            break;
                        }

                        PT_Thread_MutexLock(_pstPTSubtitle->pstMutex);

                        if(!_check_sc_interlace())
                        {
                            PT_SBTL_ERR("Get MApi_VDEC_EX_GetDispInfo failed \n");
                        }
                        else
                        {
                            _GOPSelType = (_scInterlace) ? E_GOP_DST_OP0 : E_GOP_DST_IP0;
                        }

                        if((_pstNewSubtitleItem) && (_pstNewSubtitleItem->u8DisplayWin != 0))
                        {
                            _GOPSelType = E_GOP_DST_OP0;
                        }

                        msAPI_MpegSP_Set_GOP(E_GOP_Sub);
                        msAPI_MpegSP_Set_GOP_Dst_Type(_GOPSelType);
                        MApi_GOP_GWIN_OutputLayerSwitch(E_GOP_Sub);
                        if(msAPI_MpegSP_Init() == FALSE)
                        {
                            PT_SBTL_ERR("msAPI_MpegSP_Init Failed\n");
                            break;
                        }

                        // Choose the correct MIU
                        MS_PHY phySubtitleBaseAddr = MsOS_VA2PA((MS_VIRT)msAPI_MpegSP_SpuQueue_GetSDRAM_BaseAddress());
                        _SubtitleMIUSelection(phySubtitleBaseAddr);

                        /*Set Show Status*/
                        msAPI_MpegSP_Show();

                        PT_Thread_MutexUnlock(_pstPTSubtitle->pstMutex);
                        _bSubtitleInitialized = TRUE;
                        return TRUE;
                    }while(0);
                    /*To display TTF Subtitle as normal Text subtitle if Any EXCEPTION happens
                    with TTF Subtitle Display Procedure*/
                    _bSubtitleWithTTF = FALSE;
                    if(msAPI_Text_Initialize() == FALSE)
                    {
                        PT_SBTL_ERR("msAPI_Text_Initialize Failed\n");
                        return FALSE;
                    }
                }
                else
#endif
                {
                    if(_SubtitleOpen(pstInfo->eType) == FALSE)
                    {
                        PT_SBTL_ERR("_SubtitleOpen Failed\n");
                        _SubtitleClose();
                        break;
                    }

                    if(msAPI_Text_Initialize() == FALSE)
                    {
                        PT_SBTL_ERR("msAPI_Text_Initialize Failed\n");
                        return FALSE;
                    }
                }

                bRet = TRUE;
                break;
            }
            default:
                break;
        }
        _bSubtitleInitialized = TRUE;
    }

    return bRet;
}

MMSDK_BOOL PT_SubtitlePush(const ST_MMSDK_SUBTITLE_PUSH_INFO *pstInfo)
{
    MMSDK_BOOL bRet = FALSE;
    PT_SBTL_INFO("PT_SubtitlePush Enters with[%d]--Addr[%x]\n",pstInfo->eType, (unsigned int)pstInfo->u32QueueStart);

    switch(pstInfo->eType)
    {
    case E_MMSDK_SUBTITLE_TYPE_IMAGE_VOBSUB:
        bRet = msAPI_MpegSP_SpuQueue_Push(E_VDP_CODEC_ID_SUBTITLE_IMAGE_VOBSUB, pstInfo->u32QueueStart, (U16)pstInfo->u32QueueSize, (pstInfo->u32PTSStart * 90), 0);
        break;
    case E_MMSDK_SUBTITLE_TYPE_IMAGE_BMP:
        bRet = msAPI_MpegSP_SpuQueue_Push(E_VDP_CODEC_ID_SUBTITLE_IMAGE_BMP, pstInfo->u32QueueStart, (U16)pstInfo->u32QueueSize, 0,0);
        break;
    case E_MMSDK_SUBTITLE_TYPE_TEXT_UTF8:
#if ENABLE_FREE_TYPE
        if(_bSubtitleWithTTF)
        {
            U32 u32Duration = pstInfo->u32PTSEnd -pstInfo->u32PTSStart;
            bRet = msAPI_MpegSP_SpuQueue_Push(E_VDP_CODEC_ID_SUBTITLE_TEXT_UTF8,pstInfo->u32QueueStart, (U16)pstInfo->u32QueueSize, (pstInfo->u32PTSStart),u32Duration);
        }
        else
#endif
        {
            bRet = msAPI_Text_Queue_Push(pstInfo->u32QueueStart, (U16)pstInfo->u32QueueSize, (pstInfo->u32PTSStart),(pstInfo->u32PTSEnd));
        }
        break;
    case E_MMSDK_SUBTITLE_TYPE_TEXT_ASS:
#if ENABLE_FREE_TYPE
        if(_bSubtitleWithTTF)
        {
            U32 u32Duration = pstInfo->u32PTSEnd -pstInfo->u32PTSStart;
            bRet = msAPI_MpegSP_SpuQueue_Push(E_VDP_CODEC_ID_SUBTITLE_TEXT_ASS,pstInfo->u32QueueStart, (U16)pstInfo->u32QueueSize, (pstInfo->u32PTSStart),u32Duration);
        }
        else
#endif
        {
            bRet = msAPI_Text_Queue_Push(pstInfo->u32QueueStart, (U16)pstInfo->u32QueueSize, (pstInfo->u32PTSStart),(pstInfo->u32PTSEnd));
        }
        break;
    case E_MMSDK_SUBTITLE_TYPE_TEXT_SSA:
#if ENABLE_FREE_TYPE
        if(_bSubtitleWithTTF)
        {
            U32 u32Duration = pstInfo->u32PTSEnd -pstInfo->u32PTSStart;
            bRet = msAPI_MpegSP_SpuQueue_Push(E_VDP_CODEC_ID_SUBTITLE_TEXT_SSA,pstInfo->u32QueueStart, (U16)pstInfo->u32QueueSize, (pstInfo->u32PTSStart),u32Duration);
        }
        else
#endif
        {
            bRet = msAPI_Text_Queue_Push(pstInfo->u32QueueStart, (U16)pstInfo->u32QueueSize, (pstInfo->u32PTSStart),(pstInfo->u32PTSEnd));
        }
        break;
    case E_MMSDK_SUBTITLE_TYPE_IMAGE_TS_BMP:
        bRet = msAPI_Subtitle_Queue_Push(pstInfo->u32QueueStart, (MS_U16)pstInfo->u32QueueSize, (pstInfo->u32PTSStart));
        break;
    default:
        bRet = FALSE;
        break;
    }

    //PT_SBTL_INFO("PT_SubtitlePush Step2 with[%d]\n",bRet);
    return bRet;
}

MMSDK_BOOL PT_SubtitleSetTrack(const ST_MMSDK_SBUTITLE_SET_TRACK_INFO *pstInfo)
{
    PT_SBTL_INFO("PT_SubtitleSetTrack Enters");
    MMSDK_BOOL bRet = FALSE;
    if(pstInfo->eType == E_MMSDK_SUBTITLE_TYPE_IMAGE_TS_BMP)
    {
        _gstPageID.u16CPID = pstInfo->u16CPageID;
        _gstPageID.u16APID = pstInfo->u16APageID;
#ifdef DVB_SUBTITLE_V2
        msAPI_Subtitle_DoCmd(_gpstSub, SUB_DECCMD_SET_PAGE_ID, &_gstPageID, sizeof(_ST_MMSDK_SUBTITLE_PAGEID));
#else
        msAPI_Subtitle_SetPageIDs(_gstPageID.u16CPID, _gstPageID.u16APID);
#endif
    }
    bRet = TRUE;//For check callback function
    return bRet;
}

MMSDK_BOOL PT_SubtitleDisplay(const ST_MMSDK_SUBTITLE_INIT_INFO *pstInfo)
{
    MMSDK_BOOL bRet = FALSE;

    PT_SBTL_INFO("PT_SubtitleDisplay enters[%d]\n",pstInfo->bShow);
    switch(pstInfo->eType)
    {
        case E_MMSDK_SUBTITLE_TYPE_IMAGE_TS_BMP:
#ifdef DVB_SUBTITLE_V2
            msAPI_Subtitle_Show_EX(_gpstSub,pstInfo->bShow);
#else
            msAPI_Subtitle_EnableDisplay(pstInfo->bShow);
#endif
            break;
        case E_MMSDK_SUBTITLE_TYPE_IMAGE_VOBSUB:
        case E_MMSDK_SUBTITLE_TYPE_IMAGE_BMP:
                msAPI_MpegSP_SetShowStatus(pstInfo->bShow);
                break;
        case E_MMSDK_SUBTITLE_TYPE_TEXT_SRT:
        case E_MMSDK_SUBTITLE_TYPE_TEXT_UTF8:
        case E_MMSDK_SUBTITLE_TYPE_TEXT_ASS:
        case E_MMSDK_SUBTITLE_TYPE_TEXT_SSA:
        {
#if ENABLE_FREE_TYPE
            if(_bSubtitleWithTTF)
            {
                msAPI_MpegSP_SetShowStatus(pstInfo->bShow);
            }
            else
#endif
            {
                ;
            }
            break;
        }
        default:
            break;
    }
    bRet = TRUE;//For check callback function
    return bRet;
}

MS_BOOL PT_SubtitleGetIsTTF(void)
{
#if ENABLE_FREE_TYPE
    if (_bSubtitleWithTTF)
    {
        return TRUE;
    }
    else
#endif
    {
        return FALSE;
    }
}

MMSDK_BOOL PT_SubtitleSetTrickMode(const EN_MMSDK_TRICK_MODE eTrickMode)
{
    PT_SBTL_INFO("PT_SubtitleSetTrickMode \n");

    if(_pstPTSubtitle == NULL)
    {
        PT_SBTL_INFO("no subtitle decoding task been created\n");
        return FALSE;
    }

    switch(eTrickMode)
    {
        case E_MMSDK_TRICK_PAUSE :
        if(_pstPTSubtitle->u32SubtitleType == E_MMSDK_SUBTITLE_TYPE_IMAGE_TS_BMP)
        {
            msAPI_Subtitle_Stop();
        }
            break;
        case E_MMSDK_TRICK_NORMAL:
        if(_pstPTSubtitle->u32SubtitleType == E_MMSDK_SUBTITLE_TYPE_IMAGE_TS_BMP)
        {
            if((_geTrickMode != E_MMSDK_TRICK_PAUSE) && (_geTrickMode != E_MMSDK_TRICK_NORMAL))
            {
                msAPI_Subtitle_Reset();
            }
            msAPI_Subtitle_Start();
        }
        else
        {
            msAPI_MpegSP_Show();
        }
            break;
        case E_MMSDK_TRICK_FF_2X :
        case E_MMSDK_TRICK_FFS_2X:
        case E_MMSDK_TRICK_FF_4X :
        case E_MMSDK_TRICK_FF_8X :
        case E_MMSDK_TRICK_FF_16X:
        case E_MMSDK_TRICK_FF_32X:
        case E_MMSDK_TRICK_FB_2X :
        case E_MMSDK_TRICK_FB_4X :
        case E_MMSDK_TRICK_FB_8X :
        case E_MMSDK_TRICK_FB_16X:
        case E_MMSDK_TRICK_FB_32X:
        if(_pstPTSubtitle->u32SubtitleType == E_MMSDK_SUBTITLE_TYPE_IMAGE_TS_BMP)
        {
            msAPI_Subtitle_Stop();
            msAPI_Subtitle_Clear();
        }
        else
        {
            msAPI_MpegSP_Unshow();
        }
            break;
        default:
            break;
    }

    _geTrickMode = eTrickMode;

    return TRUE;
}

MMSDK_BOOL PT_SubtitleDeInit(const ST_MMSDK_SUBTITLE_INIT_INFO *pstInfo)
{
    MMSDK_BOOL bRet = TRUE;
    PT_SBTL_INFO("PT_SubtitleDeInit \n");

    //De-Init And the Decoder Task has already been started
    if (/*(!pstInfo->bInit)  &&*/ (_bSubtitleInitialized == TRUE))
    {
        switch(pstInfo->eType)
        {
            case E_MMSDK_SUBTITLE_TYPE_IMAGE_BMP:
            case E_MMSDK_SUBTITLE_TYPE_IMAGE_VOBSUB:
            {
                if( _SubtitleDecoderTaskCheck() == FALSE)
                {
                    PT_SBTL_ERR("_SubtitleDecoderTaskCheck ERR\n");
                    return FALSE;
                }
                msAPI_MpegSP_Dinit();
                bRet = _SubtitleClose();
                break;
            }
            case E_MMSDK_SUBTITLE_TYPE_TEXT_SRT:
            case E_MMSDK_SUBTITLE_TYPE_TEXT_UTF8:
            case E_MMSDK_SUBTITLE_TYPE_TEXT_ASS:
            case E_MMSDK_SUBTITLE_TYPE_TEXT_SSA:
            {
#if ENABLE_FREE_TYPE
                if(_bSubtitleWithTTF)
                {
                    _bSubtitleWithTTF = FALSE;
                    if( _SubtitleDecoderTaskCheck() == FALSE)
                    {
                        PT_SBTL_ERR("_SubtitleDecoderTaskCheck ERR\n");
                        return FALSE;
                    }
                    msAPI_DivXTTF_DeinitTTF();
                    msAPI_MpegSP_Dinit();
                    bRet = _SubtitleClose();
                }
                else
#endif
                {
                    if( _SubtitleDecoderTaskCheck() == FALSE)
                    {
                        PT_SBTL_ERR("_SubtitleDecoderTaskCheck ERR\n");
                        return FALSE;
                    }

                    msAPI_Text_Finalize();
                    bRet = _SubtitleClose();
                }
                break;
            }
            case E_MMSDK_SUBTITLE_TYPE_IMAGE_TS_BMP:
                if( _SubtitleDecoderTaskCheck() == FALSE)
                {
                    PT_SBTL_ERR("_SubtitleDecoderTaskCheck ERR\n");
                    return FALSE;
                }
                msAPI_Subtitle_Clear();
                msAPI_Subtitle_DeInit();
#ifdef DVB_SUBTITLE_V2
                msAPI_Subtitle_ReleaseObject(_gpstSub);
                _gpstSub = NULL;
                _dvbSubQueObj = NULL;
#endif
                bRet = _SubtitleClose();
                break;
            default:
                break;
        }
        _SubMemClean(pstInfo->eType);
        _bSubtitleInitialized = FALSE;
    }
    return bRet;

}

#ifdef AVP_ENABLE
EN_MMSDK_SUBTITLE_RESULT PT_New_SubtitleInit(PT_SUBTITLEITEM* pSubtitleItem, const ST_MMSDK_SUBTITLE_INIT_INFO *pstInfo, const MMSDK_U8 u8DisplayWin, void* pClass, const pfnMmsdkSubtitleCallback pCallback)
{
    return E_MMSDK_SUBTITLE_NOT_SUPPORT;
/*
     if(pSubtitleItem == NULL)
     {
         PT_SBTL_ERR("Subtitle Item error!!!");
         return E_MMSDK_SUBTITLE_INIT_FAIL;
     }

    if(u8DisplayWin == 0 || u8DisplayWin == 3)
    {
        _ST_MMSDK_SUBTITLEITEM* pSubItem = PT_MsOS_AllocateMemory(sizeof(_ST_MMSDK_SUBTITLEITEM));
        if(pSubItem == NULL)
        {
            PT_SBTL_ERR("Sub Item error!!!");
            return E_MMSDK_SUBTITLE_INIT_FAIL;
        }
        memset(pSubItem,0,sizeof(_ST_MMSDK_SUBTITLEITEM));

        pSubItem->pCallbackClass = pClass;
        pSubItem->pCallBack = pCallback;
        pSubItem->u8DisplayWin = u8DisplayWin;

        *pSubtitleItem = (void*)pSubItem;
        _pstNewSubtitleItem = *pSubtitleItem;
    }

    MMSDK_BOOL bRet = E_MMSDK_SUBTITLE_INIT_UNKNOW_ERROR;
    bRet = (*pSubtitleItem != NULL) ? PT_SubtitleInit(pstInfo) : E_MMSDK_SUBTITLE_INIT_FAIL;

    if(bRet == E_MMSDK_SUBTITLE_INIT_UNKNOW_ERROR)
    {
        PT_MsOS_FreeMemory(_pstNewSubtitleItem);
        _pstNewSubtitleItem = NULL;
        *pSubtitleItem = NULL;
    }

    return bRet;
*/
}

MMSDK_BOOL PT_New_SubtitleDeInit(PT_SUBTITLEITEM* pSubtitleItem, const ST_MMSDK_SUBTITLE_INIT_INFO *pstInfo)
{
    return FALSE;
/*
     if(pSubtitleItem == NULL)
     {
         PT_SBTL_ERR("Subtitle Item error!!!");
         return FALSE;
     }

    MMSDK_BOOL bRet = FALSE;
    bRet = (*pSubtitleItem != NULL) ? PT_SubtitleDeInit(pstInfo) : FALSE;

    if(bRet == TRUE)
    {
        PT_MsOS_FreeMemory(_pstNewSubtitleItem);
        _pstNewSubtitleItem = NULL;
        *pSubtitleItem = NULL;
        PT_Subtitle_Finalize();
    }

    return bRet;
*/
}

MMSDK_BOOL PT_New_SubtitlePush(PT_SUBTITLEITEM pSubtitleItem, const ST_MMSDK_SUBTITLE_PUSH_INFO *pstInfo)
{
    return FALSE;
/*
    _ST_MMSDK_SUBTITLEITEM* pItem = (_ST_MMSDK_SUBTITLEITEM*)pSubtitleItem;
    MMSDK_BOOL bRet = FALSE;
    bRet = (pItem != NULL) ? PT_SubtitlePush(pstInfo) : FALSE;

    return bRet;
*/
}

MMSDK_BOOL PT_New_SubtitleSetTrack(PT_SUBTITLEITEM pSubtitleItem, const ST_MMSDK_SBUTITLE_SET_TRACK_INFO *pstInfo)
{
    return FALSE;
/*
    _ST_MMSDK_SUBTITLEITEM* pItem = (_ST_MMSDK_SUBTITLEITEM*)pSubtitleItem;
    MMSDK_BOOL bRet = FALSE;
    bRet = (pItem != NULL) ? PT_SubtitleSetTrack(pstInfo) : FALSE;

    return bRet;
*/
}

MMSDK_BOOL PT_New_SubtitleDisplay(PT_SUBTITLEITEM pSubtitleItem, const ST_MMSDK_SUBTITLE_INIT_INFO *pstInfo)
{
    return FALSE;
/*
    _ST_MMSDK_SUBTITLEITEM* pItem = (_ST_MMSDK_SUBTITLEITEM*)pSubtitleItem;
    MMSDK_BOOL bRet = FALSE;
    bRet = (pItem != NULL) ? PT_SubtitleDisplay(pstInfo) : FALSE;

    return bRet;
*/
}
#endif

#ifdef __cplusplus
}
#endif //__cplusplus
