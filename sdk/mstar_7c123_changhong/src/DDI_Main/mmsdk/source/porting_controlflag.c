#include "porting_controlflag.h"
#include "MM_Player_Def.h"
#include "mapp_videoplayer.h"

#define COMPILE_FLAG_ENABLE_4K2K            0
#ifdef DIVX_ENABLE
#define COMPILE_FLAG_ENABLE_DIVX_PLUS       1
#else
#define COMPILE_FLAG_ENABLE_DIVX_PLUS       0
#endif
#define COMPILE_FLAG_ENABLE_DYNSCALING      MMSDK_CTRL_DYNAMIC_SCALING_PORTING
#define COMPILE_FLAG_ENABLE_MVC             MMSDK_CTRL_MVC_PORTING
#define COMPILE_FLAG_ENABLE_STB             1
#define COMPILE_FLAG_ENABLE_FRC             MMSDK_CTRL_FRC_PORTING
#define COMPILE_FLAG_FRC_MODE               MMSDK_FRC_MODE

#if (COMPILE_FLAG_FRC_MODE == 1)
#include "drvMVOP.h"
#include "apiVDEC_EX.h"
#include "apiHDMITx.h"
#include "apiXC.h"
#include "porting_display.h"

#ifdef UFO_XC_HDR
#ifdef UFO_XC_HDR_VERSION
#if UFO_XC_HDR_VERSION == 2
#if (MMSDK_HDR_MODE >= 1)
extern MS_U32 _gu32HDRMetadataType;
#endif
#endif
#endif
#endif

#if (MMSDK_DISPLAY_DUALXC_PORTING == 1)
#include "apiXC_EX.h"
#endif
#include "msAPI_XC_EX.h"
#endif

MMSDK_BOOL PT_GetControlFlag(const EN_MMSDK_CONTROLFLAG eFlag)
{
    MMSDK_BOOL bRet = FALSE;
#if (COMPILE_FLAG_FRC_MODE == 1)
    MSAPI_XC_VDEC_DispInfo stVidStatus;
#endif

    switch (eFlag)
    {
        case E_MMSDK_CONTROLFLAG_ENABLE_4K2K:
                bRet = COMPILE_FLAG_ENABLE_4K2K;
                break;
        case E_MMSDK_CONTROLFLAG_ENABLE_DIVX_PLUS:
                bRet = COMPILE_FLAG_ENABLE_DIVX_PLUS;
                break;
        case E_MMSDK_CONTROLFLAG_ENABLE_DYNSCALING:
                bRet = COMPILE_FLAG_ENABLE_DYNSCALING;
                break;
        case E_MMSDK_CONTROLFLAG_ENABLE_MVC:
                bRet = COMPILE_FLAG_ENABLE_MVC;
                break;
        case E_MMSDK_CONTROLFLAG_ENABLE_STB:
                bRet = COMPILE_FLAG_ENABLE_STB;
                break;
        case E_MMSDK_CONTROLFLAG_ENABLE_FRC:
            #if (COMPILE_FLAG_FRC_MODE == 1)
                stVidStatus.u8Interlace = 0;
                if ( E_MSAPI_XC_OK != msAPI_XC_GetVDECInfo_EX_ByPath(E_MSAPI_XC_MAIN_DECODER_PATH,&stVidStatus) )
                {
                    bRet = COMPILE_FLAG_ENABLE_FRC; //frame rate control
                }
                else
                {
                    #ifdef UFO_XC_HDR
                    #ifdef UFO_XC_HDR_VERSION
                    #if UFO_XC_HDR_VERSION == 2
                    #if (MMSDK_HDR_MODE >= 1)
                    if (_gu32HDRMetadataType != E_MMSDK_HDR_TYPE_CTL_FLAG_NONE)
                    {
                        bRet = FALSE;
                    }
                    else
                    #endif
                    #endif
                    #endif
                    #endif
                    {
                        bRet = (stVidStatus.u8Interlace)? FALSE : TRUE;
                    }
                    printf("\033[1;35m###[%s][%d]### MM VDEC FRC %d\033[0m\n",__FUNCTION__,__LINE__,bRet);
                }
            #else //Mode 0 or other decide FRC by COMPILE_FLAG_ENABLE_FRC
                bRet = COMPILE_FLAG_ENABLE_FRC; //frame rate control
            #endif
                break;
        default:
                break;
    }

    return bRet;
}
