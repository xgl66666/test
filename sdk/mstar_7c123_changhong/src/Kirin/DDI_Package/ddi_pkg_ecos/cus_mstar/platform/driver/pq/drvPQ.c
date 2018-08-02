//<MStar Software>
//******************************************************************************
// MStar Software
// Copyright (c) 2010 - 2012 MStar Semiconductor, Inc. All rights reserved.
// All software, firmware and related documentation herein ("MStar Software") are
// intellectual property of MStar Semiconductor, Inc. ("MStar") and protected by
// law, including, but not limited to, copyright law and international treaties.
// Any use, modification, reproduction, retransmission, or republication of all
// or part of MStar Software is expressly prohibited, unless prior written
// permission has been granted by MStar.
//
// By accessing, browsing and/or using MStar Software, you acknowledge that you
// have read, understood, and agree, to be bound by below terms ("Terms") and to
// comply with all applicable laws and regulations:
//
// 1. MStar shall retain any and all right, ownership and interest to MStar
//    Software and any modification/derivatives thereof.
//    No right, ownership, or interest to MStar Software and any
//    modification/derivatives thereof is transferred to you under Terms.
//
// 2. You understand that MStar Software might include, incorporate or be
//    supplied together with third party`s software and the use of MStar
//    Software may require additional licenses from third parties.
//    Therefore, you hereby agree it is your sole responsibility to separately
//    obtain any and all third party right and license necessary for your use of
//    such third party`s software.
//
// 3. MStar Software and any modification/derivatives thereof shall be deemed as
//    MStar`s confidential information and you agree to keep MStar`s
//    confidential information in strictest confidence and not disclose to any
//    third party.
//
// 4. MStar Software is provided on an "AS IS" basis without warranties of any
//    kind. Any warranties are hereby expressly disclaimed by MStar, including
//    without limitation, any warranties of merchantability, non-infringement of
//    intellectual property rights, fitness for a particular purpose, error free
//    and in conformity with any international standard.  You agree to waive any
//    claim against MStar for any loss, damage, cost or expense that you may
//    incur related to your use of MStar Software.
//    In no event shall MStar be liable for any direct, indirect, incidental or
//    consequential damages, including without limitation, lost of profit or
//    revenues, lost or damage of data, and unauthorized system use.
//    You agree that this Section 4 shall still apply without being affected
//    even if MStar Software has been modified by MStar in accordance with your
//    request or instruction for your use, except otherwise agreed by both
//    parties in writing.
//
// 5. If requested, MStar may from time to time provide technical supports or
//    services in relation with MStar Software to you for your use of
//    MStar Software in conjunction with your or your customer`s product
//    ("Services").
//    You understand and agree that, except otherwise agreed by both parties in
//    writing, Services are provided on an "AS IS" basis and the warranty
//    disclaimer set forth in Section 4 above shall apply.
//
// 6. Nothing contained herein shall be construed as by implication, estoppels
//    or otherwise:
//    (a) conferring any license or right to use MStar name, trademark, service
//        mark, symbol or any other identification;
//    (b) obligating MStar or any of its affiliates to furnish any person,
//        including without limitation, you and your customers, any assistance
//        of any kind whatsoever, or any information; or
//    (c) conferring any license or right under any intellectual property right.
//
// 7. These terms shall be governed by and construed in accordance with the laws
//    of Taiwan, R.O.C., excluding its conflict of law rules.
//    Any and all dispute arising out hereof or related hereto shall be finally
//    settled by arbitration referred to the Chinese Arbitration Association,
//    Taipei in accordance with the ROC Arbitration Law and the Arbitration
//    Rules of the Association by three (3) arbitrators appointed in accordance
//    with the said Rules.
//    The place of arbitration shall be in Taipei, Taiwan and the language shall
//    be English.
//    The arbitration award shall be final and binding to both parties.
//
//******************************************************************************
//<MStar Software>
////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2008-2009 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// ("MStar Confidential Information") by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

#define  _MDRV_PQ_C_

#if defined (MSOS_TYPE_LINUX)
#include <pthread.h>
#endif

#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#else
#include <string.h>
#endif

#include "MsCommon.h"
//#include "MsVersion.h"
#include "MsOS.h"
#include "MsTypes.h"
#include "hwreg_utility2.h"
#include "color_reg.h"

#include "drvXC_IOPort.h"
#include "apiXC.h"

#include "drvPQ_Define.h"
#include "drvPQ_Declare.h"
#include "drvPQ.h"
#include "mhal_pq.h"
#include "drvMMIO.h"
#include "drvPQ_Bin.h"

#include "QualityMode.c"

#ifndef UNUSED //to avoid compile warnings...
#define UNUSED(var) (void)((var) = (var))
#endif

#define PQTAB_DBG(x) do { if( _u16PQDbgSwitch & PQ_DBG_PQTAB) x; } while(0);
#define SRULE_DBG(x) do { if( _u16PQDbgSwitch & PQ_DBG_SRULE) x; } while(0);
#define CSCRULE_DBG(x) do { if( _u16PQDbgSwitch & PQ_DBG_CSCRULE) x; } while(0);
#define PQGRULE_DBG(x) do { if( _u16PQDbgSwitch & PQ_DBG_GRULE) x; } while(0);
#define PQBW_DBG(x) do { if( _u16PQDbgSwitch & PQ_DBG_BW) x; } while(0);
#define PQINFO_DBG(x) do { if( _u16PQDbgSwitch & PQ_DBG_INFO) x; } while(0);

static MSIF_Version _drv_pq_version = {
    .DDI = { PQ_DRV_VERSION }
};

static MS_U16 _u16PQSrcType[PQ_MAX_WINDOW];
static PQ_INPUT_SOURCE_TYPE _enInputSourceType[PQ_MAX_WINDOW] = {PQ_INPUT_SOURCE_DTV,PQ_INPUT_SOURCE_DTV};
static MS_BOOL _bColorRange0_255[PQ_MAX_WINDOW];

static MS_U16 _u16PQDbgSwitch = FALSE;
static MS_U32 _u32SCBuf_Size[PQ_MAX_WINDOW] = {0, 0};
MS_U8 SWReg[SW_REG_NUM];

MS_U32 gu32LastPQIPSRAM1_Main = 0xFFFFFFFF;
MS_U32 gu32LastPQIPSRAM2_Main = 0xFFFFFFFF;
MS_U32 gu32LastPQIPCSRAM1_Main = 0xFFFFFFFF;
MS_U32 gu32LastPQIPCSRAM2_Main = 0xFFFFFFFF;
MS_U32 gu32LastPQIPVEHSDSRAM1_Main = 0xFFFFFFFF;
MS_U32 gu32LastPQIPVEHSDSRAM2_Main = 0xFFFFFFFF;
MS_U32 gu32LastPQIPVEVSDSRAM1_Main = 0xFFFFFFFF;
MS_U32 gu32LastPQIPVEVSDSRAM2_Main = 0xFFFFFFFF;

typedef struct
{
    // input timing
    MS_U16 u16input_hsize;
    MS_U16 u16input_vtotal;
    MS_U16 u16input_vfreq;

    // output timing
    MS_U16 u16output_hsize;
    MS_U16 u16output_vtotal;
    MS_U16 u16output_vfreq;

    // memory format
    MS_BOOL bFBL;
    MS_BOOL bMemFmt422;
    MS_BOOL bInterlace;
    MS_BOOL b4RMode;
    MS_U8 u8BitsPerPixel;

    // osd
    MS_BOOL bOSD_On;
    MS_U16 u16OSD_hsize;
    MS_U8 u8OSD_BitsPerPixel;

    // dram
    MS_U32 u32MemBW;
    MS_BOOL bSC_MIUSel;
    MS_BOOL bGOP_MIUSel;
    MS_BOOL bDDR2;
    MS_U32  u32DDRFreq;
    MS_U8   u8BusWidth;

    // PVR
    MS_U32 u32PVR_BW;

    //AEON
    MS_U32 u32MISC_BW;
} BW_INFO_t;

static MS_U16 _u16PQSuggestedFrameNum[PQ_MAX_WINDOW];

#if (PQ_ENABLE_PIP)
#if (PQ_ONLY_SUPPORT_BIN == 0)
static PQ_DISPLAY_TYPE genDisplayType = PQ_DISPLAY_ONE;
#endif
#endif

static MS_PQ_INFO   _info = {.u16Input_Src_Num = 128, .u8IP_Num = 217,};
static MS_PQ_Status _status = {.bIsInitialized = FALSE, .bIsRunning = FALSE, };
static MS_PQ_RFBL_INFO _stRFBL_Info = {.bEnable = FALSE, .u8MADiType = 0, };
static BW_INFO_t bw_info;

#if(ENABLE_PQ_BIN)
static MS_BOOL gbPQBinEnable = 0;
MS_PQBin_Header_Info stPQBinHeaderInfo[MAX_PQ_BIN_NUM];
extern MS_PQTextBin_Header_Info stPQTextBinHeaderInfo[MAX_PQ_TEXT_BIN_NUM];
extern MS_BOOL gbEnablePQTextBin;
#endif

MS_S32 _PQ_Mutex = -1;

static EN_POWER_MODE _enPrevPowerState = E_POWER_SUSPEND;

#if defined (MSOS_TYPE_LINUX)
pthread_mutex_t _PQ_MLoad_Mutex;
#endif

//////////////////////////////////////////////////////////////////
// PQ Patch
static MS_BOOL _bOSD_On = FALSE;
static MS_BOOL bSetFrameCount = TRUE;
static MS_BOOL _bPatchFastPlayback = FALSE;
static MS_BOOL _bOneFieldMode = FALSE;

static MS_U8 u8BK22_78_L = 0x00;
static MS_U8 u8BK22_78_H = 0x00;
static MS_U8 u8BK22_79_L = 0x00;
static MS_U8 u8BK22_79_H = 0x00;
static MS_U8 u8BK22_14_L;
static MS_U8 u8BK22_14_H;
static MS_U8 u8BK22_15_L;
static MS_U8 u8BK22_44_L;
static MS_U8 u8BK22_44_H;
static MS_U8 u8BK22_45_L;
//static MS_U8 u8BK23_0B_H;
static MS_U8 u8BK06_21_L;
static MS_U8 u8BK06_22_L;
static MS_U8 u8BK06_01_L;
static MS_U8 u8BK06_02_L;

//////////////////////////////////////////////////////////////////
// set MADi force Y/C motion enable bit
// function:
//           SAVE/RESTORE/SET
//
#define SAVE_FORCE_YC_MOTION_ENABLE(bMainWin) \
do{ \
    if(bMainWin)\
    {\
        u8BK22_78_L = Hal_PQ_get_force_y_motion(bMainWin); \
        u8BK22_79_L = Hal_PQ_get_force_c_motion(bMainWin);\
    }\
    else\
    {\
        u8BK22_78_H = Hal_PQ_get_force_y_motion(bMainWin); \
        u8BK22_79_H = Hal_PQ_get_force_c_motion(bMainWin);\
    }\
} while(0)

#define SAVE_FORCE_Y_MOTION_ENABLE(bMainWin) \
do{ \
    if(bMainWin)\
    {\
        u8BK22_78_L = Hal_PQ_get_force_y_motion(bMainWin); \
    }\
    else\
    {\
        u8BK22_78_H = Hal_PQ_get_force_y_motion(bMainWin); \
    }\
} while(0)

#define SAVE_FORCE_C_MOTION_ENABLE(bMainWin) \
do{ \
    if(bMainWin)\
    {\
        u8BK22_79_L = Hal_PQ_get_force_c_motion(bMainWin);\
    }\
    else\
    {\
        u8BK22_79_H = Hal_PQ_get_force_c_motion(bMainWin);\
    }\
} while(0)

#define RESTORE_FORCE_YC_MOTION_ENABLE(bMainWin) \
do{ \
    if(bMainWin)\
    {\
        Hal_PQ_set_force_y_motion(1, (MS_U16)u8BK22_78_L);\
        Hal_PQ_set_force_c_motion(1, (MS_U16)u8BK22_79_L);\
    }\
    else\
    {\
        Hal_PQ_set_force_y_motion(0, (((MS_U16)u8BK22_78_H)<<8));\
        Hal_PQ_set_force_c_motion(0, (((MS_U16)u8BK22_79_H))<<8);\
    }\
} while(0)

#define RESTORE_FORCE_Y_MOTION_ENABLE(bMainWin) \
do{ \
    if(bMainWin)\
    {\
        Hal_PQ_set_force_y_motion(TRUE, (MS_U16)u8BK22_78_L);\
    }\
    else\
    {\
        Hal_PQ_set_force_y_motion(FALSE, (((MS_U16)u8BK22_78_H)<<8));\
    }\
} while(0)

#define RESTORE_FORCE_C_MOTION_ENABLE(bMainWin) \
do{ \
    if(bMainWin)\
    {\
        Hal_PQ_set_force_c_motion(TRUE, (MS_U16)u8BK22_79_L);\
    }\
    else\
    {\
        Hal_PQ_set_force_c_motion(FALSE, (((MS_U16)u8BK22_79_H))<<8);\
    }\
} while(0)


#define SET_FORCE_YC_MOTION_ENABLE(bMainWin) \
do{ \
    if(bMainWin)\
    {\
        Hal_PQ_set_force_y_motion(TRUE, 0x0080);\
        Hal_PQ_set_force_c_motion(TRUE, 0x0080);\
    }\
    else\
    {\
        Hal_PQ_set_force_y_motion(FALSE, 0x8000);\
        Hal_PQ_set_force_c_motion(FALSE, 0x8000);\
    }\
}while(0)

#define SET_FORCE_YC_FULL_MOTION(bMainWin) \
do{ \
    if(bMainWin)\
    {\
        Hal_PQ_set_force_y_motion(TRUE, 0x008F);\
        Hal_PQ_set_force_c_motion(TRUE, 0x008F);\
    }\
    else\
    {\
        Hal_PQ_set_force_y_motion(FALSE, 0x8F00);\
        Hal_PQ_set_force_c_motion(FALSE, 0x8F00);\
    }\
}while(0)

//////////////////////////////////////////////////////////////////
// set vertical LPF after MADi
// function:
//           SAVE/RESTORE/SET
//
#define SAVE_MADI_LPF(bMainWin) \
do{ \
    if(bMainWin)\
    {\
        u8BK22_14_L = Hal_PQ_get_dipf_temporal(1) & 0x00FF; \
        u8BK22_14_H = (Hal_PQ_get_dipf_temporal(1) & 0xFF00) >> 8;  \
        u8BK22_15_L = Hal_PQ_get_dipf_spatial(1);  \
    }\
    else\
    {\
        u8BK22_44_L = Hal_PQ_get_dipf_temporal(0) & 0x00FF; \
        u8BK22_44_H = (Hal_PQ_get_dipf_temporal(0) & 0xFF00) >> 8;  \
        u8BK22_45_L = Hal_PQ_get_dipf_spatial(0);  \
    }\
} while(0)

#define RESTORE_MADI_LPF(bMainWin) \
do{ \
    if(bMainWin)\
    {\
        Hal_PQ_set_dipf_temporal(1, ((((MS_U16)u8BK22_14_H)<< 8)| (MS_U16)u8BK22_14_L));\
        Hal_PQ_set_dipf_spatial(1, (MS_U16)u8BK22_15_L);\
    }\
    else\
    {\
        Hal_PQ_set_dipf_temporal(0, ((((MS_U16)u8BK22_44_H)<< 8)| (MS_U16)u8BK22_44_L));\
        Hal_PQ_set_dipf_spatial(0, (MS_U16)u8BK22_45_L);\
    }\
} while(0)

#define SET_MADI_LPF(bMainWin) \
do{ \
    if(bMainWin)\
    {\
        Hal_PQ_set_dipf_temporal(1, 0x1880);\
        Hal_PQ_set_dipf_spatial(1, 0x00);\
    }\
    else\
    {\
        Hal_PQ_set_dipf_temporal(0, 0x1880);\
        Hal_PQ_set_dipf_spatial(0, 0x00);\
    }\
}while(0)

//////////////////////////////////////////////////////////////////
// set VSP V scaling filter to Y-ROM 121
// function:
//           SAVE/RESTORE/SET
//
#define SAVE_VSP_YROM121() \
do{ \
    u8BK23_0B_H = Hal_PQ_get_vsp_sram_filter();\
} while(0)

#define RESTORE_VSP_YROM121() \
do{ \
    Hal_PQ_get_vsp_sram_filter(u8BK23_0B_H);\
} while(0)

#define SET_VSP_YROM121() \
do{ \
    Hal_PQ_get_vsp_sram_filter(0x82);\
}while(0)

//////////////////////////////////////////////////////////////////
// set DNR disable
// function:
//           SAVE/RESTORE/SET
//
#define SAVE_NR_DISABLE(bMainWin) \
do{ \
    if(bMainWin)\
    {\
        u8BK06_21_L = Hal_PQ_get_dnr(1);\
    }\
    else\
    {\
        u8BK06_01_L = Hal_PQ_get_dnr(0);\
    }\
} while(0)

#define RESTORE_NR_DISABLE(bMainWin) \
do{ \
    if(bMainWin)\
    {\
        Hal_PQ_set_dnr(1, u8BK06_21_L);\
    }\
    else\
    {\
        Hal_PQ_set_dnr(0, u8BK06_01_L);\
    }\
} while(0)

#define SET_NR_DISABLE(bMainWin) \
do{ \
    Hal_PQ_set_dnr(bMainWin, 0x00);\
}while(0)

//////////////////////////////////////////////////////////////////
// set PreSNR disable
// function:
//           SAVE/RESTORE/SET
//
#define SAVE_PRESNR_DISABLE(bMainWin) \
do{ \
    if(bMainWin)\
    {\
        u8BK06_22_L = Hal_PQ_get_presnr(1);\
    }\
    else\
    {\
        u8BK06_02_L = Hal_PQ_get_presnr(0);\
    }\
} while(0)

#define RESTORE_PRESNR_DISABLE(bMainWin) \
do{ \
    if(bMainWin)\
    {\
        Hal_PQ_set_presnr(1, u8BK06_22_L);\
    }\
    else\
    {\
        Hal_PQ_set_presnr(0, u8BK06_02_L);\
    }\
} while(0)

#define SET_PRESNR_DISABLE(bMainWin) \
do{ \
    if(bMainWin)\
    {\
        Hal_PQ_set_presnr(1, 0x00);\
    }\
    else\
    {\
        Hal_PQ_set_presnr(0, 0x00);\
    }\
}while(0)



#if 0 //def _PDEBUG
#define PTH_RET_CHK(_pf_) \
    ({ \
        int r = _pf_; \
        if (r != 0 && r != ETIMEDOUT) \
            fprintf(stderr, "[PTHREAD] %s: %d: %s: %s\n", __FILE__, __LINE__, #_pf_, strerror(r)); \
        r; \
    })
#else
#define PTH_RET_CHK(_pf_) //_pf_
#endif


#if(ENABLE_PQ_MLOAD)
#if defined (MSOS_TYPE_LINUX)
#define PQ_MLOAD_ENTRY() PTH_RET_CHK(pthread_mutex_lock(&_PQ_MLoad_Mutex))
#define PQ_MLOAD_RETURN() PTH_RET_CHK(pthread_mutex_unlock(&_PQ_MLoad_Mutex))
#else
#define PQ_MLOAD_ENTRY()
#define PQ_MLOAD_RETURN()
#endif //MSOS_TYPE_LINUX_KERNEL
#endif

//////////////////////////////////////////////////////////////////
// OS related
//
//
#define XC_PQ_WAIT_MUTEX              (TICK_PER_ONE_MS * 50)              // 50 ms

/*
// Mutex & Lock
static MS_S32 _s32XC_PQMutex = -1;


#define XC_PQ_ENTRY()                 {   if (_s32XC_PQMutex < 0) { return E_XC_ACE_FAIL; }        \
                                        if(OS_OBTAIN_MUTEX(_s32XC_PQMutex, XC_PQ_WAIT_MUTEX) == FALSE) return E_XC_ACE_OBTAIN_MUTEX_FAIL;  }

#define XC_PQ_RETURN(_ret)            {   OS_RELEASE_MUTEX(_s32XC_PQMutex); return _ret; }
*/

//////////////////////////////////////////////////////////////////

static void _CalAvailableBW(BW_INFO_t *bw_infom)
{
    //printf("DDRFreq=%lu, buswidth=%u, bDDR2=%u\n", bw_infom->u32DDRFreq, bw_infom->u8BusWidth, bw_infom->bDDR2);
    bw_infom->u32MemBW = bw_infom->u32DDRFreq * (bw_infom->u8BusWidth/8) * (bw_infom->bDDR2?2:1) * 1000000;
    bw_infom->u32MemBW = bw_infom->u32MemBW /100*ESTIMATE_AVAILABLE_RATE;

    // AEON BW is unknown, add to avoid HDMI 1080P 60Hz + GOP BW insufficient
    bw_infom->u32MISC_BW = 35*1000000;
}


void MDrv_PQ_Init(MS_PQ_Init_Info *pstPQInitInfo)
{
    PQTABLE_INFO PQTableInfo;
    MS_U8 i;
#if(ENABLE_PQ_BIN)
    MS_U8 u8ID;
#endif

    _PQ_Mutex = MsOS_CreateMutex(E_MSOS_FIFO, "_XC_Mutex", MSOS_PROCESS_SHARED);

    if (_PQ_Mutex == -1)
    {
        (printf("[MAPI PQ][%06d] create mutex fail\n", __LINE__));
        return;
    }

    memset(&PQTableInfo, 0, sizeof(PQTableInfo));

#if(ENABLE_PQ_MLOAD)
#if defined (MSOS_TYPE_LINUX)
    PTH_RET_CHK(pthread_mutexattr_t attr);
    PTH_RET_CHK(pthread_mutexattr_init(&attr));
    PTH_RET_CHK(pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE));
    PTH_RET_CHK(pthread_mutex_init(&_PQ_MLoad_Mutex, &attr));
#endif
#endif

    _status.bIsInitialized = TRUE;
    _status.bIsRunning     = TRUE;

#if(ENABLE_PQ_BIN)
    if(pstPQInitInfo->u8PQTextBinCnt)
    {
        gbEnablePQTextBin = 1;
        for(i=0;i <pstPQInitInfo->u8PQTextBinCnt; i++)
        {
            u8ID = pstPQInitInfo->stPQTextBinInfo[i].u8PQID;
            stPQTextBinHeaderInfo[u8ID].u8BinID = pstPQInitInfo->stPQTextBinInfo[i].u8PQID;
            stPQTextBinHeaderInfo[u8ID].u32BinStartAddress = /*MS_PA2KSEG0*/(pstPQInitInfo->stPQTextBinInfo[i].PQBin_PhyAddr);
            MDrv_PQTextBin_Parsing(&stPQTextBinHeaderInfo[u8ID]);
        }
    }
    else
    {
        //printf("NonPQBin_Text !!\n");
        gbEnablePQTextBin = 0;
    }

    if(pstPQInitInfo->u8PQBinCnt)
    {
        gbPQBinEnable = 1;
        MDrv_PQBin_SetDisplayType(PQ_BIN_DISPLAY_ONE);

        switch(pstPQInitInfo->u16PnlWidth)
        {
        case 1366:
            MDrv_PQBin_SetPanelID(1);
            break;
        case 1920:
        default:
            MDrv_PQBin_SetPanelID(0);
            break;
        }


        for(i=0; i<pstPQInitInfo->u8PQBinCnt; i++)
        {
            u8ID = pstPQInitInfo->stPQBinInfo[i].u8PQID;
            stPQBinHeaderInfo[u8ID].u8BinID = (MS_U8)pstPQInitInfo->stPQBinInfo[i].u8PQID;
            stPQBinHeaderInfo[u8ID].u32BinStartAddress = /*MS_PA2KSEG0*/(pstPQInitInfo->stPQBinInfo[i].PQBin_PhyAddr);
            MDrv_PQBin_Parsing(&stPQBinHeaderInfo[u8ID]);
            MDrv_PQBin_LoadCommTable(MDrv_PQBin_GetPanelIdx(), &stPQBinHeaderInfo[u8ID]);
        }
    }
    else
#endif
    {
#if(PQ_ONLY_SUPPORT_BIN)
        PQTableInfo = PQTableInfo;
    #if(ENABLE_PQ_BIN)
        gbPQBinEnable = 1;
    #endif
#else
        //printf("NonPQBin !! \n");
    #if(ENABLE_PQ_BIN)
        gbPQBinEnable = 0;
    #endif

#if (PQ_ENABLE_PIP)
        switch(pstPQInitInfo->u16PnlWidth)
        {
            case 1366:
                PQTableInfo.pQuality_Map_Aray = (void*)QMAP_1366_Sub;
                PQTableInfo.pGRule_Level[E_GRULE_NR] = (void*)MST_GRule_1366_NR_Sub;
            #if (PQ_GRULE_STB_PQ_ENABLE == 1)
                PQTableInfo.pGRule_Level[E_GRULE_STB_PQ] = (void*)MST_GRule_1366_STB_PQ_Sub;
            #endif
            #if (PQ_GRULE_OSD_BW_ENABLE == 1)
                PQTableInfo.pGRule_Level[E_GRULE_OSD_BW] = (void*)MST_GRule_1366_OSD_BW_Sub;
            #endif
                break;
            case 1920:
            default:
                PQTableInfo.pQuality_Map_Aray = (void*)QMAP_1920_Sub;
                PQTableInfo.pGRule_Level[E_GRULE_NR] = (void*)MST_GRule_1920_NR_Sub;
            #if (PQ_GRULE_STB_PQ_ENABLE == 1)
                PQTableInfo.pGRule_Level[E_GRULE_STB_PQ] = (void*)MST_GRule_1920_STB_PQ_Sub;
            #endif
            #if (PQ_GRULE_OSD_BW_ENABLE == 1)
                PQTableInfo.pGRule_Level[E_GRULE_OSD_BW] = (void*)MST_GRule_1920_OSD_BW_Sub;
            #endif
                break;
        }

        // table config parameter
        PQTableInfo.u8PQ_InputType_Num = QM_INPUTTYPE_NUM_Sub;
        PQTableInfo.u8PQ_IP_Num = PQ_IP_NUM_Sub;
        PQTableInfo.pIPTAB_Info = (void*)PQ_IPTAB_INFO_Sub;
        PQTableInfo.pSkipRuleIP = (void*)MST_SkipRule_IP_Sub;

        PQTableInfo.u8PQ_XRule_IP_Num[E_XRULE_HSD] = PQ_HSDRule_IP_NUM_Sub;
        PQTableInfo.u8PQ_XRule_IP_Num[E_XRULE_VSD] = PQ_VSDRule_IP_NUM_Sub;
        PQTableInfo.u8PQ_XRule_IP_Num[E_XRULE_HSP] = PQ_HSPRule_IP_NUM_Sub;
        PQTableInfo.u8PQ_XRule_IP_Num[E_XRULE_VSP] = PQ_VSPRule_IP_NUM_Sub;
        PQTableInfo.u8PQ_XRule_IP_Num[E_XRULE_CSC] = PQ_CSCRule_IP_NUM_Sub;
    #if (PQ_XRULE_DB_ENABLE == 1)
        PQTableInfo.u8PQ_XRule_IP_Num[E_XRULE_DB_NTSC] = PQ_DBRule_NTSC_IP_NUM_Sub;
        PQTableInfo.u8PQ_XRule_IP_Num[E_XRULE_DB_PAL] = PQ_DBRule_PAL_IP_NUM_Sub;
    #endif

        PQTableInfo.pXRule_IP_Index[E_XRULE_HSD] = (void*)MST_HSDRule_IP_Index_Sub;
        PQTableInfo.pXRule_IP_Index[E_XRULE_VSD] = (void*)MST_VSDRule_IP_Index_Sub;
        PQTableInfo.pXRule_IP_Index[E_XRULE_HSP] = (void*)MST_HSPRule_IP_Index_Sub;
        PQTableInfo.pXRule_IP_Index[E_XRULE_VSP] = (void*)MST_VSPRule_IP_Index_Sub;
        PQTableInfo.pXRule_IP_Index[E_XRULE_CSC] = (void*)MST_CSCRule_IP_Index_Sub;
    #if (PQ_XRULE_DB_ENABLE == 1)
        PQTableInfo.pXRule_IP_Index[E_XRULE_DB_NTSC] = (void*)MST_DBRule_NTSC_IP_Index_Sub;
        PQTableInfo.pXRule_IP_Index[E_XRULE_DB_PAL]  = (void*)MST_DBRule_PAL_IP_Index_Sub;
    #endif

        PQTableInfo.pXRule_Array[E_XRULE_HSD] = (void*)MST_HSDRule_Array_Sub;
        PQTableInfo.pXRule_Array[E_XRULE_VSD] = (void*)MST_VSDRule_Array_Sub;
        PQTableInfo.pXRule_Array[E_XRULE_HSP] = (void*)MST_HSPRule_Array_Sub;
        PQTableInfo.pXRule_Array[E_XRULE_VSP] = (void*)MST_VSPRule_Array_Sub;
        PQTableInfo.pXRule_Array[E_XRULE_CSC] = (void*)MST_CSCRule_Array_Sub;
    #if (PQ_XRULE_DB_ENABLE == 1)
        PQTableInfo.pXRule_Array[E_XRULE_DB_NTSC] = (void*)MST_DBRule_NTSC_Array_Sub;
        PQTableInfo.pXRule_Array[E_XRULE_DB_PAL]  = (void*)MST_DBRule_PAL_Array_Sub;
    #endif

        PQTableInfo.u8PQ_GRule_Num[E_GRULE_NR] = PQ_GRULE_NR_NUM_Sub;
        PQTableInfo.u8PQ_GRule_IPNum[E_GRULE_NR] = PQ_GRULE_NR_IP_NUM_Sub;
        PQTableInfo.pGRule_IP_Index[E_GRULE_NR] = (void*)MST_GRule_NR_IP_Index_Sub;
        PQTableInfo.pGRule_Array[E_GRULE_NR] = (void*)MST_GRule_NR_Sub;

#if (PQ_GRULE_STB_PQ_ENABLE == 1)
        PQTableInfo.u8PQ_GRule_Num[E_GRULE_STB_PQ] = PQ_GRULE_STB_PQ_NUM_Sub;
        PQTableInfo.u8PQ_GRule_IPNum[E_GRULE_STB_PQ] = PQ_GRULE_STB_PQ_IP_NUM_Sub;
        PQTableInfo.pGRule_IP_Index[E_GRULE_STB_PQ] = (void*)MST_GRule_STB_PQ_IP_Index_Sub;
        PQTableInfo.pGRule_Array[E_GRULE_STB_PQ] = (void*)MST_GRule_STB_PQ_Sub;
#endif

#if (PQ_GRULE_OSD_BW_ENABLE == 1)
        PQTableInfo.u8PQ_GRule_Num[E_GRULE_OSD_BW] = PQ_GRULE_OSD_BW_NUM_Sub;
        PQTableInfo.u8PQ_GRule_IPNum[E_GRULE_OSD_BW] = PQ_GRULE_OSD_BW_IP_NUM_Sub;
        PQTableInfo.pGRule_IP_Index[E_GRULE_OSD_BW] = (void*)MST_GRule_OSD_BW_IP_Index_Sub;
        PQTableInfo.pGRule_Array[E_GRULE_OSD_BW] = (void*)MST_GRule_OSD_BW_Sub;
#endif

        _u16PQSrcType[PQ_SUB_WINDOW] = QM_InputSourceToIndex(PQ_SUB_WINDOW, _enInputSourceType[PQ_SUB_WINDOW]);
        MDrv_PQ_AddTable_(SUB, &PQTableInfo);
        MDrv_PQ_LoadCommTable_(SUB);

        // table config parameter
        PQTableInfo.u8PQ_InputType_Num = QM_INPUTTYPE_NUM_Sub_Ex;
        PQTableInfo.u8PQ_IP_Num = PQ_IP_NUM_Sub_Ex;
        PQTableInfo.pIPTAB_Info = (void*)PQ_IPTAB_INFO_Sub_Ex;
        PQTableInfo.pSkipRuleIP = (void*)MST_SkipRule_IP_Sub_Ex;

        switch(pstPQInitInfo->u16PnlWidth)
        {
            case 1366:
                PQTableInfo.pQuality_Map_Aray = (void*)QMAP_1366_Sub_Ex;
                break;
            case 1920:
            default:
                PQTableInfo.pQuality_Map_Aray = (void*)QMAP_1920_Sub_Ex;
                break;
        }

        MDrv_PQ_AddTable_(SUBEX, &PQTableInfo);
        MDrv_PQ_LoadCommTable_(SUBEX);

        if (QM_INPUTTYPE_NUM_Sub != QM_INPUTTYPE_NUM_Sub_Ex)
        {
            printf("PQ INPUTTYPE NUM MISMATCH: SUB != SUBEX");
            MS_ASSERT(0);
        }
#endif // end of if (PQ_ENABLE_PIP)
//Main Window
        switch(pstPQInitInfo->u16PnlWidth)
        {
            case 1366:
                PQTableInfo.pQuality_Map_Aray = (void*)QMAP_1366_Main;
                PQTableInfo.pGRule_Level[E_GRULE_NR] = (void*)MST_GRule_1366_NR_Main;
            #if (PQ_GRULE_STB_PQ_ENABLE == 1)
                PQTableInfo.pGRule_Level[E_GRULE_STB_PQ] = (void*)MST_GRule_1366_STB_PQ_Main;
            #endif
            #if (PQ_GRULE_OSD_BW_ENABLE == 1)
                PQTableInfo.pGRule_Level[E_GRULE_OSD_BW] = (void*)MST_GRule_1366_OSD_BW_Main;
            #endif
                break;
            case 1920:
            default:
                PQTableInfo.pQuality_Map_Aray = (void*)QMAP_1920_Main;
                PQTableInfo.pGRule_Level[E_GRULE_NR] = (void*)MST_GRule_1920_NR_Main;
            #if (PQ_GRULE_STB_PQ_ENABLE == 1)
                PQTableInfo.pGRule_Level[E_GRULE_STB_PQ] = (void*)MST_GRule_1920_STB_PQ_Main;
            #endif
            #if (PQ_GRULE_OSD_BW_ENABLE == 1)
                PQTableInfo.pGRule_Level[E_GRULE_OSD_BW] = (void*)MST_GRule_1920_OSD_BW_Main;
            #endif
                break;
        }

        // table config parameter
        PQTableInfo.u8PQ_InputType_Num = QM_INPUTTYPE_NUM_Main;
        PQTableInfo.u8PQ_IP_Num = PQ_IP_NUM_Main;
        PQTableInfo.pIPTAB_Info = (void*)PQ_IPTAB_INFO_Main;
        PQTableInfo.pSkipRuleIP = (void*)MST_SkipRule_IP_Main;

        PQTableInfo.u8PQ_XRule_IP_Num[E_XRULE_HSD] = PQ_HSDRule_IP_NUM_Main;
        PQTableInfo.u8PQ_XRule_IP_Num[E_XRULE_VSD] = PQ_VSDRule_IP_NUM_Main;
        PQTableInfo.u8PQ_XRule_IP_Num[E_XRULE_HSP] = PQ_HSPRule_IP_NUM_Main;
        PQTableInfo.u8PQ_XRule_IP_Num[E_XRULE_VSP] = PQ_VSPRule_IP_NUM_Main;
        PQTableInfo.u8PQ_XRule_IP_Num[E_XRULE_CSC] = PQ_CSCRule_IP_NUM_Main;
    #if (PQ_XRULE_DB_ENABLE == 1)
        PQTableInfo.u8PQ_XRule_IP_Num[E_XRULE_DB_NTSC] = PQ_DBRule_NTSC_IP_NUM_Main;
        PQTableInfo.u8PQ_XRule_IP_Num[E_XRULE_DB_PAL]  = PQ_DBRule_PAL_IP_NUM_Main;
    #endif

        PQTableInfo.pXRule_IP_Index[E_XRULE_HSD] = (void*)MST_HSDRule_IP_Index_Main;
        PQTableInfo.pXRule_IP_Index[E_XRULE_VSD] = (void*)MST_VSDRule_IP_Index_Main;
        PQTableInfo.pXRule_IP_Index[E_XRULE_HSP] = (void*)MST_HSPRule_IP_Index_Main;
        PQTableInfo.pXRule_IP_Index[E_XRULE_VSP] = (void*)MST_VSPRule_IP_Index_Main;
        PQTableInfo.pXRule_IP_Index[E_XRULE_CSC] = (void*)MST_CSCRule_IP_Index_Main;
    #if (PQ_XRULE_DB_ENABLE == 1)
        PQTableInfo.pXRule_IP_Index[E_XRULE_DB_NTSC] = (void*)MST_DBRule_NTSC_IP_Index_Main;
        PQTableInfo.pXRule_IP_Index[E_XRULE_DB_PAL]  = (void*)MST_DBRule_PAL_IP_Index_Main;
    #endif
        PQTableInfo.pXRule_Array[E_XRULE_HSD] = (void*)MST_HSDRule_Array_Main;
        PQTableInfo.pXRule_Array[E_XRULE_VSD] = (void*)MST_VSDRule_Array_Main;
        PQTableInfo.pXRule_Array[E_XRULE_HSP] = (void*)MST_HSPRule_Array_Main;
        PQTableInfo.pXRule_Array[E_XRULE_VSP] = (void*)MST_VSPRule_Array_Main;
        PQTableInfo.pXRule_Array[E_XRULE_CSC] = (void*)MST_CSCRule_Array_Main;
    #if (PQ_XRULE_DB_ENABLE == 1)
        PQTableInfo.pXRule_Array[E_XRULE_DB_NTSC] = (void*)MST_DBRule_NTSC_Array_Main;
        PQTableInfo.pXRule_Array[E_XRULE_DB_PAL]  = (void*)MST_DBRule_PAL_Array_Main;
    #endif
        PQTableInfo.u8PQ_GRule_Num[E_GRULE_NR] = PQ_GRULE_NR_NUM_Main;
        PQTableInfo.u8PQ_GRule_IPNum[E_GRULE_NR] = PQ_GRULE_NR_IP_NUM_Main;
        PQTableInfo.pGRule_IP_Index[E_GRULE_NR] = (void*)MST_GRule_NR_IP_Index_Main;
        PQTableInfo.pGRule_Array[E_GRULE_NR] = (void*)MST_GRule_NR_Main;

#if (PQ_GRULE_STB_PQ_ENABLE == 1)
        PQTableInfo.u8PQ_GRule_Num[E_GRULE_STB_PQ] = PQ_GRULE_STB_PQ_NUM_Main;
        PQTableInfo.u8PQ_GRule_IPNum[E_GRULE_STB_PQ] = PQ_GRULE_STB_PQ_IP_NUM_Main;
        PQTableInfo.pGRule_IP_Index[E_GRULE_STB_PQ] = (void*)MST_GRule_STB_PQ_IP_Index_Main;
        PQTableInfo.pGRule_Array[E_GRULE_STB_PQ] = (void*)MST_GRule_STB_PQ_Main;
#endif

#if (PQ_GRULE_OSD_BW_ENABLE == 1)
        PQTableInfo.u8PQ_GRule_Num[E_GRULE_OSD_BW] = PQ_GRULE_OSD_BW_NUM_Main;
        PQTableInfo.u8PQ_GRule_IPNum[E_GRULE_OSD_BW] = PQ_GRULE_OSD_BW_IP_NUM_Main;
        PQTableInfo.pGRule_IP_Index[E_GRULE_OSD_BW] = (void*)MST_GRule_OSD_BW_IP_Index_Main;
        PQTableInfo.pGRule_Array[E_GRULE_OSD_BW] = (void*)MST_GRule_OSD_BW_Main;
#endif

        _u16PQSrcType[PQ_MAIN_WINDOW] = QM_InputSourceToIndex(PQ_MAIN_WINDOW, _enInputSourceType[PQ_MAIN_WINDOW]);
        MDrv_PQ_AddTable_(MAIN, &PQTableInfo);
        MDrv_PQ_LoadCommTable_(MAIN);

        // table config parameter
        PQTableInfo.u8PQ_InputType_Num = QM_INPUTTYPE_NUM_Main_Ex;
        PQTableInfo.u8PQ_IP_Num = PQ_IP_NUM_Main_Ex;
        PQTableInfo.pIPTAB_Info = (void*)PQ_IPTAB_INFO_Main_Ex;
        PQTableInfo.pSkipRuleIP = (void*)MST_SkipRule_IP_Main_Ex;

        switch(pstPQInitInfo->u16PnlWidth)
        {
            case 1366:
                PQTableInfo.pQuality_Map_Aray = (void*)QMAP_1366_Main_Ex;
                break;
            case 1920:
            default:
                PQTableInfo.pQuality_Map_Aray = (void*)QMAP_1920_Main_Ex;
                break;
        }

        MDrv_PQ_AddTable_(MAINEX, &PQTableInfo);
        MDrv_PQ_LoadCommTable_(MAINEX);
	
        if (QM_INPUTTYPE_NUM_Main != PQTableInfo.u8PQ_InputType_Num)
        {
            printf("PQ INPUTTYPE NUM MISMATCH: MAIN != EX");
            MS_ASSERT(0);
        }
#endif // end of #if(PQ_ONLY_SUPPORT_BIN == 0)
    }




#if 1
    // Set BK12 [40] [1:0] = b'11 to enable hw double buffer write
    // at blanking area for changing memory format by L_BK_SCMI(0x02)
    // this register is added after T2 U04
    Hal_PQ_set_memfmt_doublebuffer(ENABLE);

    memcpy(&_stPQ_Info, pstPQInitInfo, sizeof(MS_PQ_Init_Info));

    memset(&bw_info, 0x00, sizeof(bw_info));
    bw_info.bDDR2 = pstPQInitInfo->bDDR2;
    bw_info.u32DDRFreq = pstPQInitInfo->u32DDRFreq;
    bw_info.u8BusWidth = pstPQInitInfo->u8BusWidth;
    bw_info.u16output_vtotal = pstPQInitInfo->u16Pnl_vtotal;
    bw_info.u16output_hsize = pstPQInitInfo->u16PnlWidth;
    bw_info.u16OSD_hsize = pstPQInitInfo->u16OSD_hsize;

    _CalAvailableBW(&bw_info);

    for(i=0; i<PQ_MAX_WINDOW; i++)
    {
        _bColorRange0_255[i] = TRUE;
        _u16PQSuggestedFrameNum[i] = 0;
    }
#endif
}

MS_BOOL MDrv_PQ_Exit(void)
{
    return true;
}

MS_U16 MDrv_PQ_GetIPNum(PQ_WIN eWindow)
{
    if (eWindow == PQ_MAIN_WINDOW)
    {
    #if(ENABLE_PQ_BIN)
        if(gbPQBinEnable)
        {
            return MDrv_PQBin_GetIPNum(&stPQBinHeaderInfo[PQ_BIN_STD_MAIN]);
        }
        else
    #endif
        {
        #if(PQ_ONLY_SUPPORT_BIN)
            return 0xFFFF;
        #else
            return MDrv_PQ_GetIPNum_(MAIN);
        #endif
        }
    }

#if (PQ_ENABLE_PIP)
    if (eWindow == PQ_SUB_WINDOW)
    {
    #if(ENABLE_PQ_BIN)
        if(gbPQBinEnable)
        {
            return MDrv_PQBin_GetIPNum(&stPQBinHeaderInfo[PQ_BIN_STD_SUB]);
        }
        else
    #endif
        {
        #if(PQ_ONLY_SUPPORT_BIN)
                return 0xFFFF;
        #else
            return MDrv_PQ_GetIPNum_(SUB);
        #endif
        }
    }
#endif

    else
    {
        MS_ASSERT(0);
        return 0xFFFF;
    }
}

MS_U16 MDrv_PQ_GetTableNum(PQ_WIN eWindow, MS_U16 u16PQIPIdx)
{
    if (eWindow == PQ_MAIN_WINDOW)
    {
    #if(ENABLE_PQ_BIN)
        if(gbPQBinEnable)
        {
            return MDrv_PQBin_GetTableNum(MDrv_PQBin_GetPanelIdx(), &stPQBinHeaderInfo[PQ_BIN_STD_MAIN]);
        }
        else
    #endif
        {
        #if(PQ_ONLY_SUPPORT_BIN)
            u16PQIPIdx = u16PQIPIdx;
            return 0xFFFF;
        #else
            return MDrv_PQ_GetTableNum_(MAIN, (MS_U8)u16PQIPIdx);
        #endif
        }
    }
#if (PQ_ENABLE_PIP)
    if (eWindow == PQ_SUB_WINDOW)
    {
    #if(ENABLE_PQ_BIN)
        if(gbPQBinEnable)
        {
           return MDrv_PQBin_GetTableNum(MDrv_PQBin_GetPanelIdx(), &stPQBinHeaderInfo[PQ_BIN_STD_SUB]);
        }
        else
    #endif
        {
        #if(PQ_ONLY_SUPPORT_BIN)
            return 0xFFFF;
        #else
            return MDrv_PQ_GetTableNum_(SUB, (MS_U8)u16PQIPIdx);
        #endif
        }
    }
#endif
    else
    {
        MS_ASSERT(0);
        return 0xFFFF;
    }
}

MS_U16 MDrv_PQ_GetCurrentTableIndex(PQ_WIN eWindow, MS_U16 u16PQIPIdx)
{
    if (eWindow == PQ_MAIN_WINDOW)
    {
    #if(ENABLE_PQ_BIN)
        if(gbPQBinEnable)
        {
            return MDrv_PQBin_GetCurrentTableIndex(MDrv_PQBin_GetPanelIdx(),  u16PQIPIdx);
        }
        else
    #endif
        {
        #if(PQ_ONLY_SUPPORT_BIN)
                return 0xFFFF;
        #else
            return MDrv_PQ_GetCurrentTableIndex_(MAIN, (MS_U8)u16PQIPIdx);
        #endif
        }
    }
#if (PQ_ENABLE_PIP)
    if (eWindow == PQ_SUB_WINDOW)
    {
    #if(ENABLE_PQ_BIN)
        if(gbPQBinEnable)
        {
            return MDrv_PQBin_GetCurrentTableIndex(MDrv_PQBin_GetPanelIdx(),  u16PQIPIdx);
        }
        else
    #endif
        {
        #if(PQ_ONLY_SUPPORT_BIN)
            return 0xFFFF;
        #else
            return MDrv_PQ_GetCurrentTableIndex_(SUB, (MS_U8)u16PQIPIdx);
        #endif
        }
    }
#endif
    else
    {
        MS_ASSERT(0);
        return 0xFFFF;
    }
}


MS_U16 MDrv_PQ_GetTableIndex(PQ_WIN eWindow, MS_U16 u16IPIdx)
{
    MS_U8 u16TabIdx = 0;

    if (eWindow == PQ_MAIN_WINDOW)
    {
    #if(ENABLE_PQ_BIN)
        if(gbPQBinEnable)
        {
            u16TabIdx = MDrv_PQBin_GetTableIndex(_u16PQSrcType[eWindow],
                                                 u16IPIdx,
                                                 MDrv_PQBin_GetPanelIdx(),
                                                 &stPQBinHeaderInfo[PQ_BIN_STD_MAIN]);
        }
        else
    #endif
        {
        #if(PQ_ONLY_SUPPORT_BIN)
            u16TabIdx = 0;
        #else
            u16TabIdx = MDrv_PQ_GetTableIndex_(MAIN, _u16PQSrcType[eWindow], (MS_U8)u16IPIdx);
        #endif
        }
    }
#if (PQ_ENABLE_PIP)
    else if (eWindow == PQ_SUB_WINDOW)
    {
    #if(ENABLE_PQ_BIN)
        if(gbPQBinEnable)
        {
            u16TabIdx = MDrv_PQBin_GetTableIndex(_u16PQSrcType[eWindow],
                                                 u16IPIdx,
                                                 MDrv_PQBin_GetPanelIdx(),
                                                 &stPQBinHeaderInfo[PQ_BIN_STD_SUB]);
        }
        else
    #endif
        {
        #if(PQ_ONLY_SUPPORT_BIN)
            u16TabIdx = 0;
        #else
            u16TabIdx = MDrv_PQ_GetTableIndex_(SUB, _u16PQSrcType[eWindow], (MS_U8)u16IPIdx);
        #endif
        }
    }
#endif
    else
    {
        MS_ASSERT(0);
    }
    return u16TabIdx;
}

static MS_U16 MDrv_PQ_GetXRuleIPIndex(PQ_WIN eWindow, MS_U16 u16XRuleType, MS_U16 u16XRuleIP)
{
    MS_U8 u16IPIdx = 0;
    if (eWindow == PQ_MAIN_WINDOW)
    {
    #if(ENABLE_PQ_BIN)
        if(gbPQBinEnable)
        {
            u16IPIdx = MDrv_PQBin_GetXRuleIPIndex(u16XRuleType,
                                                 u16XRuleIP,
                                                 &stPQBinHeaderInfo[PQ_BIN_STD_MAIN]);
        }
        else
    #endif
        {
        #if(PQ_ONLY_SUPPORT_BIN)
            u16IPIdx = 0;
        #else
            u16IPIdx = MDrv_PQ_GetXRuleIPIndex_(MAIN, (MS_U8)u16XRuleType, (MS_U8)u16XRuleIP);
        #endif
        }
    }
#if (PQ_ENABLE_PIP)
    else if (eWindow == PQ_SUB_WINDOW)
    {
    #if(ENABLE_PQ_BIN)
        if(gbPQBinEnable)
        {
            u16IPIdx = MDrv_PQBin_GetXRuleIPIndex(u16XRuleType,
                                                 u16XRuleIP,
                                                 &stPQBinHeaderInfo[PQ_BIN_STD_SUB]);
        }
        else
    #endif
        {
        #if(PQ_ONLY_SUPPORT_BIN)
            u16IPIdx = 0;
        #else
            u16IPIdx = MDrv_PQ_GetXRuleIPIndex_(SUB, (MS_U8)u16XRuleType, (MS_U8)u16XRuleIP);
        #endif
        }
    }
#endif
    else
    {
        MS_ASSERT(0);
    }
    return u16IPIdx;
}

static MS_U16 MDrv_PQ_GetXRuleTableIndex(PQ_WIN eWindow, MS_U16 u16XRuleType, MS_U16 u16XRuleIdx, MS_U16 u16XRuleIP)
{
    MS_U16 u16TabIdx = 0;

    if (eWindow == PQ_MAIN_WINDOW)
    {
    #if(ENABLE_PQ_BIN)
        if(gbPQBinEnable)
        {
            u16TabIdx = MDrv_PQBin_GetXRuleTableIndex(u16XRuleType,
                                                      u16XRuleIdx,
                                                      u16XRuleIP,
                                                      &stPQBinHeaderInfo[PQ_BIN_STD_MAIN]);
        }
        else
    #endif
        {
        #if(PQ_ONLY_SUPPORT_BIN)
            u16TabIdx = 0;
        #else
            u16TabIdx = MDrv_PQ_GetXRuleTableIndex_(MAIN, (MS_U8)u16XRuleType, (MS_U8)u16XRuleIdx, (MS_U8)u16XRuleIP);
        #endif
        }
    }
#if (PQ_ENABLE_PIP)
    else if (eWindow == PQ_SUB_WINDOW)
    {
    #if(ENABLE_PQ_BIN)
        if(gbPQBinEnable)
        {
            u16TabIdx = MDrv_PQBin_GetXRuleTableIndex(u16XRuleType,
                                                      u16XRuleIdx,
                                                      u16XRuleIP,
                                                      &stPQBinHeaderInfo[PQ_BIN_STD_SUB]);
        }
        else
    #endif
        {
        #if(PQ_ONLY_SUPPORT_BIN)
            u16TabIdx = 0;
        #else
            u16TabIdx = MDrv_PQ_GetXRuleTableIndex_(SUB, (MS_U8)u16XRuleType, (MS_U8)u16XRuleIdx, (MS_U8)u16XRuleIP);
        #endif
        }
    }
#endif
    else
    {
        MS_ASSERT(0);
    }
    return u16TabIdx;
}

static MS_U16 MDrv_PQ_GetXRuleIPNum(PQ_WIN eWindow, MS_U16 u16XRuleType)
{
    MS_U16 u16IPNum = 0;
    if (eWindow == PQ_MAIN_WINDOW)
    {
    #if(ENABLE_PQ_BIN)
        if(gbPQBinEnable)
        {
            u16IPNum = MDrv_PQBin_GetXRuleIPNum(u16XRuleType,
                                                &stPQBinHeaderInfo[PQ_BIN_STD_MAIN]);
        }
        else
    #endif
        {
        #if(PQ_ONLY_SUPPORT_BIN)
            u16IPNum = 0;
        #else
            u16IPNum = MDrv_PQ_GetXRuleIPNum_(MAIN, (MS_U8)u16XRuleType);
        #endif
        }
    }
#if (PQ_ENABLE_PIP)
    else if (eWindow == PQ_SUB_WINDOW)
    {
    #if(ENABLE_PQ_BIN)
        if(gbPQBinEnable)
        {
            u16IPNum = MDrv_PQBin_GetXRuleIPNum( u16XRuleType,
                                                &stPQBinHeaderInfo[PQ_BIN_STD_SUB]);
        }
        else
    #endif
        {
        #if(PQ_ONLY_SUPPORT_BIN)
                u16IPNum = 0;
        #else
            u16IPNum = MDrv_PQ_GetXRuleIPNum_(SUB, (MS_U8)u16XRuleType);
        #endif
        }
    }
#endif
    else
    {
        MS_ASSERT(0);
    }
    return u16IPNum;
}

static MS_U16 MDrv_PQ_GetGRule_LevelIndex(PQ_WIN eWindow,MS_U16 u16GRuleType, MS_U16 u16GRuleLevelIndex)
{
    MS_U16 u16Ret = 0;

    if (eWindow == PQ_MAIN_WINDOW)
    {
    #if(ENABLE_PQ_BIN)
        if(gbPQBinEnable)
        {
            u16Ret = (MS_U8) MDrv_PQBin_GetGRule_LevelIndex(MDrv_PQBin_GetPanelIdx(),
                                                           u16GRuleType,
                                                           u16GRuleLevelIndex,
                                                           &stPQBinHeaderInfo[PQ_BIN_STD_MAIN]);
        }
        else
    #endif
        {
        #if(PQ_ONLY_SUPPORT_BIN)
            u16Ret = 0;
        #else
            u16Ret = MDrv_PQ_GetGRule_LevelIndex_(MAIN, (MS_U8)u16GRuleType, (MS_U8)u16GRuleLevelIndex);
        #endif
        }
    }
#if (PQ_ENABLE_PIP)
    else if (eWindow == PQ_SUB_WINDOW)
    {
    #if(ENABLE_PQ_BIN)
        if(gbPQBinEnable)
        {
            u16Ret = (MS_U8) MDrv_PQBin_GetGRule_LevelIndex(MDrv_PQBin_GetPanelIdx(),
                                                           u16GRuleType,
                                                           u16GRuleLevelIndex,
                                                           &stPQBinHeaderInfo[PQ_BIN_STD_SUB]);
        }
        else
    #endif
        {
        #if(PQ_ONLY_SUPPORT_BIN)
            u16Ret = 0;
        #else
            u16Ret = MDrv_PQ_GetGRule_LevelIndex_(SUB, (MS_U8)u16GRuleType, (MS_U8)u16GRuleLevelIndex);
        #endif
        }
    }
#endif
    else
    {
        MS_ASSERT(0);
    }
    return u16Ret;
}

static MS_U16 MDrv_PQ_GetGRule_IPIndex(PQ_WIN eWindow, MS_U16 u16GRuleType, MS_U16 u16GRuleIPIndex)
{
    MS_U16 u16Ret = 0;
    if (eWindow == PQ_MAIN_WINDOW)
    {
    #if(ENABLE_PQ_BIN)
        if(gbPQBinEnable)
        {
            u16Ret = MDrv_PQBin_GetGRule_IPIndex(u16GRuleType,
                                                 u16GRuleIPIndex,
                                                 &stPQBinHeaderInfo[PQ_BIN_STD_MAIN]);
        }
        else
    #endif
        {
        #if(PQ_ONLY_SUPPORT_BIN)
            u16Ret = 0;
        #else
            u16Ret = MDrv_PQ_GetGRule_IPIndex_(MAIN, (MS_U8)u16GRuleType, (MS_U8)u16GRuleIPIndex);
        #endif
        }
    }
#if (PQ_ENABLE_PIP)
    else if (eWindow == PQ_SUB_WINDOW)
    {
    #if(ENABLE_PQ_BIN)
        if(gbPQBinEnable)
        {
            u16Ret = MDrv_PQBin_GetGRule_IPIndex(u16GRuleType,
                                                 u16GRuleIPIndex,
                                                 &stPQBinHeaderInfo[PQ_BIN_STD_SUB]);
        }
        else
    #endif
        {
        #if(PQ_ONLY_SUPPORT_BIN)
            u16Ret = 0;
        #else
            u16Ret = MDrv_PQ_GetGRule_IPIndex_(SUB, (MS_U8)u16GRuleType, (MS_U8)u16GRuleIPIndex);
        #endif
        }
    }
#endif
    else
    {
        MS_ASSERT(0);
    }

    return u16Ret;
}

static MS_U16 MDrv_PQ_GetGRule_TableIndex(PQ_WIN eWindow, MS_U16 u16GRuleType, MS_U16 u16PQ_NRIdx, MS_U16 u16GRuleIPIndex)
{
    MS_U16 u16Ret = 0;
    if (eWindow == PQ_MAIN_WINDOW)
    {
    #if(ENABLE_PQ_BIN)
        if(gbPQBinEnable)
        {
            u16Ret = (MS_U8) MDrv_PQBin_GetGRule_TableIndex(u16GRuleType,
                                                            _u16PQSrcType[eWindow],
                                                            u16PQ_NRIdx,
                                                            u16GRuleIPIndex,
                                                            &stPQBinHeaderInfo[PQ_BIN_STD_MAIN]);
        }
        else
    #endif
        {
        #if(PQ_ONLY_SUPPORT_BIN)
            u16Ret = 0;
        #else
            u16Ret = MDrv_PQ_GetGRule_TableIndex_(MAIN, (MS_U8)u16GRuleType, _u16PQSrcType[eWindow], (MS_U8)u16PQ_NRIdx, (MS_U8)u16GRuleIPIndex);
        #endif
        }
    }
#if (PQ_ENABLE_PIP)
    else if (eWindow == PQ_SUB_WINDOW)
    {
    #if(ENABLE_PQ_BIN)
        if(gbPQBinEnable)
        {
            u16Ret = (MS_U8) MDrv_PQBin_GetGRule_TableIndex(u16GRuleType,
                                                            _u16PQSrcType[eWindow],
                                                            u16PQ_NRIdx,
                                                            u16GRuleIPIndex,
                                                            &stPQBinHeaderInfo[PQ_BIN_STD_SUB]);
        }
        else
    #endif
        {
        #if(PQ_ONLY_SUPPORT_BIN)
            u16Ret = 0;
        #else
            u16Ret = MDrv_PQ_GetGRule_TableIndex_(SUB, (MS_U8)u16GRuleType, _u16PQSrcType[eWindow], (MS_U8)u16PQ_NRIdx, (MS_U8)u16GRuleIPIndex);
        #endif
        }
    }
#endif
    else
    {
        MS_ASSERT(0);
    }

    return u16Ret;
}


void MDrv_PQ_LoadTableData(PQ_WIN eWindow, MS_U16 u16TabIdx, MS_U16 u16PQIPIdx, MS_U8 *pTable, MS_U16 u16TableSize)
{
    if (eWindow == PQ_MAIN_WINDOW)
    {
    #if(ENABLE_PQ_BIN)
        if(gbPQBinEnable)
        {
            MDrv_PQBin_LoadTableData(MDrv_PQBin_GetPanelIdx(),
                                 u16TabIdx,
                                 u16PQIPIdx,
                                 &stPQBinHeaderInfo[PQ_BIN_STD_MAIN],
                                 pTable,
                                 u16TableSize);
        }
        else
    #endif
        {
        #if(PQ_ONLY_SUPPORT_BIN == 0)
            MDrv_PQ_LoadTableData_(MAIN, (MS_U8)u16TabIdx, (MS_U8)u16PQIPIdx, pTable, u16TableSize);
        #endif
        }
    }
#if (PQ_ENABLE_PIP)
    else if (eWindow == PQ_SUB_WINDOW)
    {
    #if(ENABLE_PQ_BIN)
        if(gbPQBinEnable)
        {
            MDrv_PQBin_LoadTableData(MDrv_PQBin_GetPanelIdx(),
                                 u16TabIdx,
                                 u16PQIPIdx,
                                 &stPQBinHeaderInfo[PQ_BIN_STD_SUB],
                                 pTable,
                                 u16TableSize);
        }
        else
    #endif
        {
        #if(PQ_ONLY_SUPPORT_BIN == 0)
            MDrv_PQ_LoadTableData_(SUB, (MS_U8)u16TabIdx, (MS_U8)u16PQIPIdx, pTable, u16TableSize);
        #endif
        }
    }
#endif
    else
    {
        MS_ASSERT(0);
    }
}

void MDrv_PQ_LoadTable(PQ_WIN eWindow, MS_U16 u16TabIdx, MS_U16 u16PQIPIdx)
{
    if (eWindow == PQ_MAIN_WINDOW)
    {
    #if(ENABLE_PQ_BIN)
        if(gbPQBinEnable)
        {
            MDrv_PQBin_LoadTable(MDrv_PQBin_GetPanelIdx(),
                                 u16TabIdx,
                                 u16PQIPIdx,
                                 &stPQBinHeaderInfo[PQ_BIN_STD_MAIN]);
        }
        else
    #endif
        {
        #if(PQ_ONLY_SUPPORT_BIN == 0)
            MDrv_PQ_LoadTable_(MAIN, (MS_U8)u16TabIdx, (MS_U8)u16PQIPIdx);
        #endif
        }
    }
#if (PQ_ENABLE_PIP)
    else if (eWindow == PQ_SUB_WINDOW)
    {
    #if(ENABLE_PQ_BIN)
        if(gbPQBinEnable)
        {
            MDrv_PQBin_LoadTable(MDrv_PQBin_GetPanelIdx(),
                                 u16TabIdx,
                                 u16PQIPIdx,
                                 &stPQBinHeaderInfo[PQ_BIN_STD_SUB]);
        }
        else
    #endif
        {
        #if(PQ_ONLY_SUPPORT_BIN == 0)
            MDrv_PQ_LoadTable_(SUB, (MS_U8)u16TabIdx, (MS_U8)u16PQIPIdx);
        #endif
        }
    }
#endif
    else
    {
        MS_ASSERT(0);
    }
}

void MDrv_PQ_CheckSettings(PQ_WIN eWindow)
{
    if (eWindow == PQ_MAIN_WINDOW)
    {
    #if(ENABLE_PQ_BIN)
        if(gbPQBinEnable)
        {
            //ToDo MAINEX
            MDrv_PQBin_CheckCommTable(MDrv_PQBin_GetPanelIdx(),
                                      &stPQBinHeaderInfo[PQ_BIN_STD_MAIN]);

            MDrv_PQBin_CheckTableBySrcType(_u16PQSrcType[eWindow],
                                           PQ_BIN_IP_ALL,
                                           MDrv_PQBin_GetPanelIdx(),
                                           &stPQBinHeaderInfo[PQ_BIN_STD_MAIN]);
        }
        else
    #endif
        {
            #if(PQ_ONLY_SUPPORT_BIN == 0)
            MDrv_PQ_CheckCommTable_(MAINEX);
            MDrv_PQ_CheckTableBySrcType_(MAINEX, _u16PQSrcType[eWindow], PQ_IP_ALL);
            MDrv_PQ_CheckCommTable_(MAIN);
            MDrv_PQ_CheckTableBySrcType_(MAIN, _u16PQSrcType[eWindow], PQ_IP_ALL);
            #endif
        }
    }
#if (PQ_ENABLE_PIP)
    else if (eWindow == PQ_SUB_WINDOW)
    {
    #if(ENABLE_PQ_BIN)
        if(gbPQBinEnable)
        {
            //ToDo SUBEX
            MDrv_PQBin_CheckCommTable(MDrv_PQBin_GetPanelIdx(),
                                      &stPQBinHeaderInfo[PQ_BIN_STD_SUB]);

            MDrv_PQBin_CheckTableBySrcType(_u16PQSrcType[eWindow],
                                           PQ_BIN_IP_ALL,
                                           MDrv_PQBin_GetPanelIdx(),
                                           &stPQBinHeaderInfo[PQ_BIN_STD_SUB]);
        }
        else
    #endif
        {
            #if(PQ_ONLY_SUPPORT_BIN == 0)
            MDrv_PQ_CheckCommTable_(SUBEX);
            MDrv_PQ_CheckTableBySrcType_(SUBEX, _u16PQSrcType[eWindow], PQ_IP_ALL);
            MDrv_PQ_CheckCommTable_(SUB);
            MDrv_PQ_CheckTableBySrcType_(SUB, _u16PQSrcType[eWindow], PQ_IP_ALL);
            #endif
        }
    }
#endif
    else
    {
        MS_ASSERT(0);
    }
}


#if(ENABLE_PQ_MLOAD)
void MDrv_PQ_Set_MLoadEn(PQ_WIN eWindow, MS_BOOL bEn)
{
    if(MApi_XC_MLoad_GetStatus() != E_MLOAD_ENABLED)
    {
        bEn = FALSE;
    }

    if(bEn)
    {
        PQ_MLOAD_ENTRY();
    }

    if (eWindow == PQ_MAIN_WINDOW)
    {
    #if(ENABLE_PQ_BIN)

        if(gbPQBinEnable)
        {
            MDrv_PQBin_Set_MLoadEn(bEn);
        }
        else
    #endif
        {
        #if(PQ_ONLY_SUPPORT_BIN == 0)
            MDrv_PQ_Set_MLoadEn_(MAIN, bEn);
        #endif
        }

    }
#if (PQ_ENABLE_PIP)
    else if(eWindow == PQ_SUB_WINDOW)
    {
    #if(ENABLE_PQ_BIN)
        if(gbPQBinEnable)
        {
            MDrv_PQBin_Set_MLoadEn(bEn);
        }
        else
    #endif
        {
        #if(PQ_ONLY_SUPPORT_BIN == 0)
            MDrv_PQ_Set_MLoadEn_(SUB, bEn);
        #endif
        }
    }
#endif
    else
    {
        MS_ASSERT(0);
    }


    if(bEn == FALSE)
    {
        PQ_MLOAD_RETURN();
    }
}
#endif

void MDrv_PQ_LoadSettings(PQ_WIN eWindow)
{
    if (eWindow == PQ_MAIN_WINDOW)
    {
    #if(ENABLE_PQ_BIN)
        if(gbPQBinEnable)
        {

            MDrv_PQBin_LoadTableBySrcType(_u16PQSrcType[eWindow],
                                          PQ_BIN_IP_ALL,
                                          MDrv_PQBin_GetPanelIdx(),
                                          &stPQBinHeaderInfo[PQ_BIN_STD_MAIN]);

            MDrv_PQBin_LoadTableBySrcType(_u16PQSrcType[eWindow],
                                          PQ_BIN_IP_ALL,
                                          MDrv_PQBin_GetPanelIdx(),
                                          &stPQBinHeaderInfo[PQ_BIN_EXT_MAIN]);
        }
        else
    #endif
        {
        #if(PQ_ONLY_SUPPORT_BIN == 0)
            PQTAB_DBG(printf("MAIN table\n"));
            MDrv_PQ_LoadTableBySrcType_(MAIN, _u16PQSrcType[eWindow], PQ_IP_ALL);

            PQTAB_DBG(printf("MAINEX table\n"));
            MDrv_PQ_LoadTableBySrcType_(MAINEX, _u16PQSrcType[eWindow], PQ_IP_ALL);
            PQTAB_DBG(printf("...done\n"));
        #endif
        }
    }
#if (PQ_ENABLE_PIP)
    else if (eWindow == PQ_SUB_WINDOW)
    {
    #if(ENABLE_PQ_BIN)
        if(gbPQBinEnable)
        {
            MDrv_PQBin_LoadTableBySrcType(_u16PQSrcType[eWindow],
                                          PQ_BIN_IP_ALL,
                                          MDrv_PQBin_GetPanelIdx(),
                                          &stPQBinHeaderInfo[PQ_BIN_STD_SUB]);

            MDrv_PQBin_LoadTableBySrcType(_u16PQSrcType[eWindow],
                                          PQ_BIN_IP_ALL,
                                          MDrv_PQBin_GetPanelIdx(),
                                          &stPQBinHeaderInfo[PQ_BIN_EXT_SUB]);
        }
        else
    #endif
        {
        #if(PQ_ONLY_SUPPORT_BIN == 0)
            PQTAB_DBG(printf("SUB table\n"));
            MDrv_PQ_LoadTableBySrcType_(SUB, _u16PQSrcType[eWindow], PQ_IP_ALL);

            PQTAB_DBG(printf("SUB MAINEX table\n"));
            MDrv_PQ_LoadTableBySrcType_(SUBEX, _u16PQSrcType[eWindow], PQ_IP_ALL);
            PQTAB_DBG(printf("...done\n"));
        #endif
        }
    }
#endif
    else
    {
        MS_ASSERT(0);
    }
}

void MDrv_PQ_Set_DTVInfo(PQ_WIN eWindow, MS_PQ_Dtv_Info *pstPQDTVInfo)
{
    memcpy(&_stDTV_Info[eWindow], pstPQDTVInfo, sizeof(MS_PQ_Dtv_Info));

    PQINFO_DBG(printf("PQ DTV Info:Win=%u, type=%u\n", eWindow, _stDTV_Info[eWindow].eType));
}

void MDrv_PQ_Set_MultiMediaInfo(PQ_WIN eWindow, MS_PQ_MuliMedia_Info *pstPQMMInfo)
{
    memcpy(&_stMultiMedia_Info[eWindow], pstPQMMInfo, sizeof(MS_PQ_MuliMedia_Info));
    PQINFO_DBG(printf("PQ MM Info:Win=%u, type=%u\n", eWindow, _stMultiMedia_Info[eWindow].eType));
}

void MDrv_PQ_Set_VDInfo(PQ_WIN eWindow, MS_PQ_Vd_Info *pstPQVDInfo)
{
    memcpy(&_stVD_Info[eWindow], pstPQVDInfo, sizeof(MS_PQ_Vd_Info));

    PQINFO_DBG(printf("PQ VD Info:Win=%u, SigType=%u, bSCARTRGB=%u, VIFIn=%u\n",
        eWindow,
        _stVD_Info[eWindow].enVideoStandard,
        _stVD_Info[eWindow].bIsSCART_RGB,
        _stVD_Info[eWindow].bIsVIFIN));

}

void MDrv_PQ_Set_ModeInfo(PQ_WIN eWindow, PQ_INPUT_SOURCE_TYPE enInputSourceType, MS_PQ_Mode_Info *pstPQModeInfo)
{

    memcpy(&_stMode_Info[eWindow], pstPQModeInfo, sizeof(MS_PQ_Mode_Info));

    //printf("PQ Set Mode Info: %s, Src=%x\n", (eWindow)?("SubWin"):("MainWin"), enInputSourceType);
    if(QM_IsSourceYPbPr(enInputSourceType) || QM_IsSourceHDMI(enInputSourceType))
    {
        if( (pstPQModeInfo->u16input_hsize < 1500) &&
            (pstPQModeInfo->u16input_vsize < 500) &&
            (pstPQModeInfo->u16input_vfreq <  650) )
        {
            _u8ModeIndex[eWindow] = (pstPQModeInfo->bInterlace) ?
                           PQ_MD_720x480_60I :
                           PQ_MD_720x480_60P;

        }
        else if( (pstPQModeInfo->u16input_hsize < 1500) &&
                 (pstPQModeInfo->u16input_vsize < 600) &&
                 (pstPQModeInfo->u16input_vfreq <  550) )
        {
            _u8ModeIndex[eWindow] = (pstPQModeInfo->bInterlace) ?
                           PQ_MD_720x576_50I :
                           PQ_MD_720x576_50P;
        }
        else if( (pstPQModeInfo->u16input_hsize < 1300) &&
                 (pstPQModeInfo->u16input_vsize < 800) &&
                 (pstPQModeInfo->bInterlace == FALSE) )
        {
            _u8ModeIndex[eWindow] = (pstPQModeInfo->u16input_vfreq < 550) ?
                           PQ_MD_1280x720_50P :
                           PQ_MD_1280x720_60P;

        }
        else if( (pstPQModeInfo->u16input_hsize < 1930) &&
                 (pstPQModeInfo->u16input_vsize < 1100))
        {
            if(pstPQModeInfo->bInterlace)
            {
                _u8ModeIndex[eWindow] = (pstPQModeInfo->u16input_vfreq < 550) ?
                               PQ_MD_1920x1080_50I :
                               PQ_MD_1920x1080_60I;
            }
            else
            {
                _u8ModeIndex[eWindow] = (pstPQModeInfo->u16input_vfreq < 250) ?  PQ_MD_1920x1080_24P :
                               (pstPQModeInfo->u16input_vfreq < 350) ?  PQ_MD_1920x1080_30P :
                               (pstPQModeInfo->u16input_vfreq < 550) ?  PQ_MD_1920x1080_50P :
                               PQ_MD_1920x1080_60P;

            }
        }
        else
        {
            _u8ModeIndex[eWindow] = PQ_MD_720x576_50I;
        }
    }
    else
    {
        _u8ModeIndex[eWindow] = PQ_MD_Num;
    }

    if(QM_IsSourceYPbPr(enInputSourceType) || QM_IsSourceHDMI(enInputSourceType))
    {
        if(_u8ModeIndex[eWindow] <= (MS_U8)PQ_MD_720x576_50P)
            _gIsSrcHDMode[eWindow] = 0;
        else
            _gIsSrcHDMode[eWindow] = 1;

    }
    else
    {
        if(pstPQModeInfo->u16input_hsize >= 1200)
            _gIsSrcHDMode[eWindow] = 1;
        else
            _gIsSrcHDMode[eWindow] = 0;
    }

    PQINFO_DBG(printf("PQ ModeInfo:%d input(%d, %d), disp(%d, %d), ModeIdx=%d, FBL=%u, Interlace=%u, InV=%u, OutV=%u, inVtt=%u\n",
        eWindow,
        _stMode_Info[eWindow].u16input_hsize,
        _stMode_Info[eWindow].u16input_vsize,
        _stMode_Info[eWindow].u16display_hsize,
        _stMode_Info[eWindow].u16display_vsize,
        _u8ModeIndex[eWindow],
        _stMode_Info[eWindow].bFBL,
        _stMode_Info[eWindow].bInterlace,
        _stMode_Info[eWindow].u16input_vfreq,
        _stMode_Info[eWindow].u16ouput_vfreq,
        _stMode_Info[eWindow].u16input_vtotal));

}

void MDrv_PQ_SetHDMIInfo(PQ_WIN eWindow, MS_PQ_Hdmi_Info *pstPQHDMIInfo)
{
    _stHDMI_Info[eWindow].bIsHDMI = pstPQHDMIInfo->bIsHDMI;
    _stHDMI_Info[eWindow].enColorFmt = pstPQHDMIInfo->enColorFmt;

    PQINFO_DBG(printf("PQ HDMI, bHDMI=%u, colorfmt=%u\n",
        _stHDMI_Info[eWindow].bIsHDMI,
        _stHDMI_Info[eWindow].enColorFmt));
}

void MDrv_PQ_DesideSrcType(PQ_WIN eWindow, PQ_INPUT_SOURCE_TYPE enInputSourceType)
{
    MS_U8 u8Bank;

    _enInputSourceType[eWindow] = enInputSourceType;
    _u16PQSrcType[eWindow] = QM_InputSourceToIndex(eWindow, enInputSourceType);
    PQTAB_DBG(printf("[PQ_DesideSrcType] window=%u, enInputSrcType=%u, SrcType=%u\n",
              eWindow, enInputSourceType, _u16PQSrcType[eWindow]));

    u8Bank = MApi_XC_ReadByte(BK_SELECT_00);
    Hal_PQ_set_sourceidx((eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, _u16PQSrcType[eWindow]);
    MApi_XC_WriteByte(BK_SELECT_00, u8Bank);
}

PQ_INPUT_SOURCE_TYPE MDrv_PQ_GetInputSourceType(PQ_WIN eWindow)
{
    return _enInputSourceType[eWindow];
}

MS_U16 MDrv_PQ_GetSrcType(PQ_WIN eWindow)
{
    return _u16PQSrcType[eWindow];
}

// if not HSD case, ignore return value
static MS_BOOL _MDrv_PQ_LoadScalingTable(PQ_WIN eWindow,
                              MS_U8 eXRuleType,
                              MS_U8 u8XRuleIP,
                              MS_BOOL bPreV_ScalingDown,
                              MS_BOOL bInterlace,
                              MS_BOOL bColorSpaceYUV,
                              MS_U16 u16InputSize,
                              MS_U16 u16SizeAfterScaling)
{
    MS_U32 u32Ratio;
    MS_U16 u16IPIdx;
#if(ENABLE_PQ_BIN)
    MS_U16 u16TabIdx = gbPQBinEnable == 0 ? PQ_IP_NULL : PQ_BIN_IP_NULL;
#else
    MS_U16 u16TabIdx = PQ_IP_NULL;
#endif
    MS_U8 u8XRuleIdx = 0xFF;

    if(u16InputSize == 0)
        u32Ratio = 2000;
    else
        u32Ratio = ((MS_U32) u16SizeAfterScaling * 1000) / u16InputSize;

    u16IPIdx = MDrv_PQ_GetXRuleIPIndex(eWindow, eXRuleType, u8XRuleIP);

    SRULE_DBG(printf("SRuleIP=%d, IPIdx=%d, input=%d, output=%d, ratio=%ld, ",
             (MS_U16)u8XRuleIP, (MS_U16)u16IPIdx,
             u16InputSize, u16SizeAfterScaling, u32Ratio));

    if(bPreV_ScalingDown && bInterlace)
    {
        u8XRuleIdx = PQ_HSDRule_PreV_ScalingDown_Interlace_Main;
        u16TabIdx = MDrv_PQ_GetXRuleTableIndex(eWindow,(MS_U16)eXRuleType, (MS_U16)u8XRuleIdx, (MS_U16)u8XRuleIP);
    }
    else if( (bPreV_ScalingDown) && (!bInterlace) )
    {
        u8XRuleIdx = PQ_HSDRule_PreV_ScalingDown_Progressive_Main;
        u16TabIdx = MDrv_PQ_GetXRuleTableIndex(eWindow, (MS_U16)eXRuleType, (MS_U16)u8XRuleIdx, (MS_U16)u8XRuleIP);
    }
    //SRULE_DBG(printf("[[DEBUG: TabIdx:%u, SRuleIdx:%u, SRuleIP:%u]]\n",
    //    MST_SRULE_Array_Main[u8XRuleIdx][u8SRuleIP], u8XRuleIdx, u8SRuleIP));

#if(ENABLE_PQ_BIN)
    if ((gbPQBinEnable == 0 && u16TabIdx != PQ_IP_NULL) ||
        (gbPQBinEnable == 1 && u16TabIdx != PQ_BIN_IP_NULL))
#else
    if(u16TabIdx != PQ_IP_NULL)
#endif
    {

        SRULE_DBG(printf("u8XRuleIdx: PreV down, interlace:%u", bInterlace));
        SRULE_DBG(printf("(a)tabidx=%u\n", (MS_U16)u16TabIdx));
    }

#if(ENABLE_PQ_BIN)
    if ( (gbPQBinEnable == 1 && u16TabIdx == PQ_BIN_IP_NULL) ||
         (gbPQBinEnable == 0 && u16TabIdx == PQ_IP_NULL))
#else
    if(u16TabIdx == PQ_IP_NULL)
#endif
    {
        if(u32Ratio > 1000) //scaling up case
        {
#if (PQ_XRULE_SCALINGUP_ENABLE == 1)
            if (bColorSpaceYUV)
            {
                if(u32Ratio <= 1500)
                    u8XRuleIdx = PQ_HSDRule_ScalingDown_15x_YUV_Main;
                else if(u32Ratio <= 2000)
                    u8XRuleIdx = PQ_HSDRule_ScalingDown_20x_YUV_Main;
                else if(u32Ratio <= 4000)
                    u8XRuleIdx = PQ_HSDRule_ScalingDown_40x_YUV_Main;
                else
                    u8XRuleIdx = PQ_HSDRule_ScalingDown_40x_YUV_Main;
            }
            else
            {
                if(u32Ratio <= 1500)
                    u8XRuleIdx = PQ_HSDRule_ScalingDown_15x_RGB_Main;
                else if(u32Ratio <= 2000)
                    u8XRuleIdx = PQ_HSDRule_ScalingDown_20x_RGB_Main;
                else if(u32Ratio <= 4000)
                    u8XRuleIdx = PQ_HSDRule_ScalingDown_40x_RGB_Main;
                else
                    u8XRuleIdx = PQ_HSDRule_ScalingDown_40x_RGB_Main;
            }

            u16TabIdx = MDrv_PQ_GetXRuleTableIndex(eWindow, (MS_U16)eXRuleType, (MS_U16)u8XRuleIdx, (MS_U16)u8XRuleIP);
#if(ENABLE_PQ_BIN)
            if ( (gbPQBinEnable == 1 && u16TabIdx == PQ_BIN_IP_NULL) ||
                 (gbPQBinEnable == 0 && u16TabIdx == PQ_IP_NULL))
#else
            if( u16TabIdx == PQ_IP_NULL)
#endif
            {
                u16TabIdx = MDrv_PQ_GetTableIndex(eWindow, u16IPIdx);   //get from main page
                SRULE_DBG(printf("(d)tabidx=%u\n", u16TabIdx));
            }
#else
            u16TabIdx = MDrv_PQ_GetTableIndex(eWindow, u16IPIdx);
#endif

            SRULE_DBG(printf("u8XRuleIdx: >x1, "));
            SRULE_DBG(printf("(c)tabidx=%u\n", (MS_U16)u16TabIdx));
        }
        else
        {
            if (bColorSpaceYUV)
            {
                if(u32Ratio == 1000)
                    u8XRuleIdx = PQ_HSDRule_ScalingDown_10x_YUV_Main;
                else if(u32Ratio >= 900)
                    u8XRuleIdx = PQ_HSDRule_ScalingDown_09x_YUV_Main;
                else if(u32Ratio >= 800)
                    u8XRuleIdx = PQ_HSDRule_ScalingDown_08x_YUV_Main;
                else if(u32Ratio >= 700)
                    u8XRuleIdx = PQ_HSDRule_ScalingDown_07x_YUV_Main;
                else if(u32Ratio >= 600)
                    u8XRuleIdx = PQ_HSDRule_ScalingDown_06x_YUV_Main;
                else if(u32Ratio >= 500)
                    u8XRuleIdx = PQ_HSDRule_ScalingDown_05x_YUV_Main;
                else if(u32Ratio >= 400)
                    u8XRuleIdx = PQ_HSDRule_ScalingDown_04x_YUV_Main;
                else if(u32Ratio >= 300)
                    u8XRuleIdx = PQ_HSDRule_ScalingDown_03x_YUV_Main;
                else if(u32Ratio >= 200)
                    u8XRuleIdx = PQ_HSDRule_ScalingDown_02x_YUV_Main;
                else if(u32Ratio >= 100)
                    u8XRuleIdx = PQ_HSDRule_ScalingDown_01x_YUV_Main;
                else
                    u8XRuleIdx = PQ_HSDRule_ScalingDown_00x_YUV_Main;
            }
            else
            {
                if(u32Ratio == 1000)
                    u8XRuleIdx = PQ_HSDRule_ScalingDown_10x_RGB_Main;
                else if(u32Ratio >= 900)
                    u8XRuleIdx = PQ_HSDRule_ScalingDown_09x_RGB_Main;
                else if(u32Ratio >= 800)
                    u8XRuleIdx = PQ_HSDRule_ScalingDown_08x_RGB_Main;
                else if(u32Ratio >= 700)
                    u8XRuleIdx = PQ_HSDRule_ScalingDown_07x_RGB_Main;
                else if(u32Ratio >= 600)
                    u8XRuleIdx = PQ_HSDRule_ScalingDown_06x_RGB_Main;
                else if(u32Ratio >= 500)
                    u8XRuleIdx = PQ_HSDRule_ScalingDown_05x_RGB_Main;
                else if(u32Ratio >= 400)
                    u8XRuleIdx = PQ_HSDRule_ScalingDown_04x_RGB_Main;
                else if(u32Ratio >= 300)
                    u8XRuleIdx = PQ_HSDRule_ScalingDown_03x_RGB_Main;
                else if(u32Ratio >= 200)
                    u8XRuleIdx = PQ_HSDRule_ScalingDown_02x_RGB_Main;
                else if(u32Ratio >= 100)
                    u8XRuleIdx = PQ_HSDRule_ScalingDown_01x_RGB_Main;
                else
                    u8XRuleIdx = PQ_HSDRule_ScalingDown_00x_RGB_Main;
            }

            SRULE_DBG(printf("u8XRuleIdx=%u, ", (MS_U16)u8XRuleIdx));
            if (u8XRuleIdx == 0xFF)
            {
                MS_ASSERT(0);
                return 1;
            }

            u16TabIdx = MDrv_PQ_GetXRuleTableIndex(eWindow, (MS_U16)eXRuleType, (MS_U16)u8XRuleIdx, (MS_U16)u8XRuleIP);
        #if(ENABLE_PQ_BIN)
            if ( (gbPQBinEnable == 1 && u16TabIdx == PQ_BIN_IP_NULL) ||
                 (gbPQBinEnable == 0 && u16TabIdx == PQ_IP_NULL))
        #else
            if( u16TabIdx == PQ_IP_NULL)
        #endif
            {
                u16TabIdx = MDrv_PQ_GetTableIndex(eWindow, u16IPIdx);
                SRULE_DBG(printf("(d)tabidx=%u\n", u16TabIdx));
            }
            else
            {
                SRULE_DBG(printf("(e)tabidx=%u\n", u16TabIdx));
            }
        }
    }

    MDrv_PQ_LoadTable(eWindow, u16TabIdx, u16IPIdx);

    if(eXRuleType == E_XRULE_VSD)
       return (u16TabIdx  == PQ_IP_VSD_Bilinear_Main) ; // PreVSDMode: 0:Cb, 1:Bilinear
    else
        return (u16TabIdx != PQ_IP_HSD_Y_CB_Main); // PreHSDMode - 0:Cb, 1:Adv

}

MS_BOOL MDrv_PQ_LoadScalingTable(PQ_WIN eWindow,
                                  MS_U8 eXRuleType,
                                  MS_BOOL bPreV_ScalingDown,
                                  MS_BOOL bInterlace,
                                  MS_BOOL bColorSpaceYUV,
                                  MS_U16 u16InputSize,
                                  MS_U16 u16SizeAfterScaling)
{
    MS_BOOL bRet = 0; // default is CB mode
    MS_U16 i;

    if (eXRuleType > 3)
        MS_ASSERT(0);


    SRULE_DBG(printf("[PQ_LoadScalingTable] HSD/VSD/HSP/VSP:%u\n", (MS_U16)eXRuleType));

    for(i=0; i<MDrv_PQ_GetXRuleIPNum(eWindow, (MS_U16)eXRuleType); i++)
    {
        MS_BOOL bSDMode;
        bSDMode = _MDrv_PQ_LoadScalingTable(eWindow,
                                         eXRuleType,
                                         i,
                                         bPreV_ScalingDown,
                                         bInterlace,
                                         bColorSpaceYUV,
                                         u16InputSize,
                                         u16SizeAfterScaling);
        bRet |= (bSDMode << (1*i));
    }

    if(eXRuleType == E_XRULE_VSD)
    {
        bRet &= 0x01;
    }

    return bRet;
}


//////////////////////////////////////////////////////////////////
// set color range of input source and take effect immediately
//
void MDrv_PQ_SetColorRange(PQ_WIN eWindow, MS_BOOL bColorRange0_255)
{
    _bColorRange0_255[eWindow] = bColorRange0_255;
    MDrv_PQ_SetCSC(eWindow, _gFourceColorFmt[eWindow]);
}

//////////////////////////////////////////////////////////////////
// get memory color format
//
MS_BOOL MDrv_PQ_Get_MemYUVFmt(PQ_WIN eWindow, PQ_FOURCE_COLOR_FMT enFourceColor)
{
    MS_U16 u16XRuleIdx, u16XRuleIP;
    MS_U16 u16IPIdx, u16TabIdx;
    MS_U16 eXRuleType = E_XRULE_CSC;
    MS_U16 u16DoCSC = 0;
    MS_BOOL bInputTypeVideo;
    MS_BOOL bInputColorSpaceRGB;
    MS_BOOL bInputResolutionHD = _gIsSrcHDMode[eWindow];
    MS_BOOL bMemYUV;

    _gFourceColorFmt[eWindow] = enFourceColor;

    if(QM_IsSourceVGA(_enInputSourceType[eWindow]) ||
       QM_IsSourceDVI_HDMIPC(_enInputSourceType[eWindow], eWindow))
    {
        CSCRULE_DBG(printf("PC mode\n"));
        bInputTypeVideo = FALSE;
    }
    else
    {
        CSCRULE_DBG(printf("VIDEO mode\n"));
        bInputTypeVideo = TRUE;
    }


    if( QM_IsSourceVGA(_enInputSourceType[eWindow]) ||
       (QM_IsSourceDVI_HDMIPC(_enInputSourceType[eWindow],eWindow)) ||
       (QM_IsSourceHDMI_Video(_enInputSourceType[eWindow],eWindow) && (QM_HDMIPC_COLORRGB(eWindow))) )
    {
        CSCRULE_DBG(printf("Input RGB\n"));
        bInputColorSpaceRGB = TRUE;
    }
    else
    {
        CSCRULE_DBG(printf("Input YUV\n"));
        bInputColorSpaceRGB = FALSE;
    }

    if (bInputTypeVideo)
    {
        if (bInputColorSpaceRGB)
        {
            if (bInputResolutionHD == FALSE)
            {
                if (_bColorRange0_255[eWindow])
                {
                    u16XRuleIdx = enFourceColor == PQ_FOURCE_COLOR_RGB ?
                                 PQ_CSCRule_Video_YUV_SD_Main : // fource to RGB and input is RGB, hence no need change
                                 PQ_CSCRule_Video_RGB_SD_0_255_Main;
                }
                else // bInputDataRange16_235
                {
                    u16XRuleIdx = enFourceColor == PQ_FOURCE_COLOR_RGB ?
                                 PQ_CSCRule_Video_YUV_SD_Main :
                                 PQ_CSCRule_Video_RGB_SD_16_235_Main;
                }
            }
            else // InputResolutionHD
            {
                if (_bColorRange0_255[eWindow])
                {
                    u16XRuleIdx = enFourceColor == PQ_FOURCE_COLOR_RGB ?
                                 PQ_CSCRule_Video_YUV_HD_Main :
                                 PQ_CSCRule_Video_RGB_HD_0_255_Main;
                }
                else // bInputDataRange16_235
                {
                    u16XRuleIdx = enFourceColor == PQ_FOURCE_COLOR_RGB ?
                                 PQ_CSCRule_Video_YUV_HD_Main :
                                 PQ_CSCRule_Video_RGB_HD_16_235_Main;
                }
            }
        }
        else // InputColorSpaceYUV
        {
            if(enFourceColor == PQ_FOURCE_COLOR_RGB )
            {
                printf("InputColorSpace is YUV, Fource Color Space is RGB!!!\n");
                MS_ASSERT(0);
            }

            if (bInputResolutionHD == FALSE)
            {
                u16XRuleIdx = PQ_CSCRule_Video_YUV_SD_Main;
            }
            else // InputResolutionHD
            {
                u16XRuleIdx = PQ_CSCRule_Video_YUV_HD_Main;
            }
        }
    }
    else // InputTypePC
    {
        if (bInputColorSpaceRGB)
        {
            if (bInputResolutionHD == FALSE)
            {
                if (_bColorRange0_255[eWindow])
                {
                    u16XRuleIdx = enFourceColor == PQ_FOURCE_COLOR_RGB ?
                                 PQ_CSCRule_PC_YUV_SD_Main :
                                 PQ_CSCRule_PC_RGB_SD_0_255_Main;
                }
                else // bInputDataRange16_235
                {
                    u16XRuleIdx = enFourceColor == PQ_FOURCE_COLOR_RGB ?
                                 PQ_CSCRule_PC_YUV_SD_Main :
                                 PQ_CSCRule_PC_RGB_SD_16_235_Main;
                }
            }
            else // InputResolutionHD
            {
                if (_bColorRange0_255[eWindow])
                {
                    u16XRuleIdx = enFourceColor == PQ_FOURCE_COLOR_RGB ?
                                 PQ_CSCRule_PC_YUV_HD_Main :
                                 PQ_CSCRule_PC_RGB_HD_0_255_Main;
                }
                else // bInputDataRange16_235
                {
                    u16XRuleIdx = enFourceColor == PQ_FOURCE_COLOR_RGB ?
                                 PQ_CSCRule_PC_YUV_HD_Main :
                                 PQ_CSCRule_PC_RGB_HD_16_235_Main;
                }
            }
        }
        else // InputColorSpaceYUV
        {
            if(enFourceColor == PQ_FOURCE_COLOR_RGB )
            {
                printf("InputColorSpace is YUV, Fource Color Space is RGB!!!\n");
                MS_ASSERT(0);
            }

            if (bInputResolutionHD == FALSE)
            {
                u16XRuleIdx = PQ_CSCRule_PC_YUV_SD_Main;
            }
            else // InputResolutionHD
            {
                u16XRuleIdx = PQ_CSCRule_PC_YUV_HD_Main;
            }
        }
    }


    for(u16XRuleIP=0; u16XRuleIP<MDrv_PQ_GetXRuleIPNum(eWindow, eXRuleType); u16XRuleIP++)
    {
        u16IPIdx = MDrv_PQ_GetXRuleIPIndex(eWindow, eXRuleType, u16XRuleIP);
        u16TabIdx = MDrv_PQ_GetXRuleTableIndex(eWindow, eXRuleType, u16XRuleIdx, u16XRuleIP);
        CSCRULE_DBG(printf("u16XRuleIdx:%u, u16XRuleIP=%u, IPIdx=%u, TabIdx=%u\n", u16XRuleIdx, u16XRuleIP, u16IPIdx, u16TabIdx));

        if(u16TabIdx)
        {
            if(u16IPIdx == Hal_PQ_get_csc_ip_idx(MS_CSC_IP_CSC))
            {
                u16DoCSC |= 0x1;
            }
            else if(u16IPIdx == Hal_PQ_get_csc_ip_idx(MS_CSC_IP_VIP_CSC))
            {
                u16DoCSC |= 0x02;
            }
            else
            {
                u16DoCSC |= 0x00;
            }
        }
    }

    if(bInputColorSpaceRGB)
    {
        if(u16DoCSC & 0x01)
        {
            bMemYUV = TRUE;
        }
        else
        {
            bMemYUV = FALSE;
        }
    }
    else
    {
        bMemYUV = TRUE;
    }

    return bMemYUV;
}


//////////////////////////////////////////////////////////////////
// do color space conversion from RGB to YUV according to input sourcce and context
// return:
//        TRUE: use YUV color space
//        FALSE: otherwise
//
MS_BOOL MDrv_PQ_SetCSC(PQ_WIN eWindow, PQ_FOURCE_COLOR_FMT enFourceColor)
{
    MS_U16 u16XRuleIdx, u16XRuleIP;
    MS_U16 u16IPIdx, u16TabIdx;
    MS_U16 eXRuleType = E_XRULE_CSC;
    MS_U16 u16DoCSC = 0;
    MS_BOOL bInputTypeVideo;
    MS_BOOL bInputColorSpaceRGB;
    MS_BOOL bInputResolutionHD = _gIsSrcHDMode[eWindow];

    _gFourceColorFmt[eWindow] = enFourceColor;

    if(QM_IsSourceVGA(_enInputSourceType[eWindow]) ||
       QM_IsSourceDVI_HDMIPC(_enInputSourceType[eWindow], eWindow))
    {
        CSCRULE_DBG(printf("PC mode\n"));
        bInputTypeVideo = FALSE;
    }
    else
    {
        CSCRULE_DBG(printf("VIDEO mode\n"));
        bInputTypeVideo = TRUE;
    }


    if( QM_IsSourceVGA(_enInputSourceType[eWindow]) ||
       (QM_IsSourceDVI_HDMIPC(_enInputSourceType[eWindow],eWindow)) ||
       (QM_IsSourceHDMI_Video(_enInputSourceType[eWindow],eWindow) && (QM_HDMIPC_COLORRGB(eWindow))) )
    {
        CSCRULE_DBG(printf("Input RGB\n"));
        bInputColorSpaceRGB = TRUE;
    }
    else
    {
        CSCRULE_DBG(printf("Input YUV\n"));
        bInputColorSpaceRGB = FALSE;
    }

    if (bInputTypeVideo)
    {
        if (bInputColorSpaceRGB)
        {
            if (bInputResolutionHD == FALSE)
            {
                if (_bColorRange0_255[eWindow])
                {
                    u16XRuleIdx = enFourceColor == PQ_FOURCE_COLOR_RGB ?
                                 PQ_CSCRule_Video_YUV_SD_Main : // fource to RGB and input is RGB, hence no need change
                                 PQ_CSCRule_Video_RGB_SD_0_255_Main;
                }
                else // bInputDataRange16_235
                {
                    u16XRuleIdx = enFourceColor == PQ_FOURCE_COLOR_RGB ?
                                 PQ_CSCRule_Video_YUV_SD_Main :
                                 PQ_CSCRule_Video_RGB_SD_16_235_Main;
                }
            }
            else // InputResolutionHD
            {
                if (_bColorRange0_255[eWindow])
                {
                    u16XRuleIdx = enFourceColor == PQ_FOURCE_COLOR_RGB ?
                                 PQ_CSCRule_Video_YUV_HD_Main :
                                 PQ_CSCRule_Video_RGB_HD_0_255_Main;
                }
                else // bInputDataRange16_235
                {
                    u16XRuleIdx = enFourceColor == PQ_FOURCE_COLOR_RGB ?
                                 PQ_CSCRule_Video_YUV_HD_Main :
                                 PQ_CSCRule_Video_RGB_HD_16_235_Main;
                }
            }
        }
        else // InputColorSpaceYUV
        {
            if(enFourceColor == PQ_FOURCE_COLOR_RGB )
            {
                printf("InputColorSpace is YUV, Fource Color Space is RGB!!!\n");
                MS_ASSERT(0);
            }

            if (bInputResolutionHD == FALSE)
            {
                u16XRuleIdx = PQ_CSCRule_Video_YUV_SD_Main;
            }
            else // InputResolutionHD
            {
                u16XRuleIdx = PQ_CSCRule_Video_YUV_HD_Main;
            }
        }
    }
    else // InputTypePC
    {
        if (bInputColorSpaceRGB)
        {
            if (bInputResolutionHD == FALSE)
            {
                if (_bColorRange0_255[eWindow])
                {
                    u16XRuleIdx = enFourceColor == PQ_FOURCE_COLOR_RGB ?
                                 PQ_CSCRule_PC_YUV_SD_Main :
                                 PQ_CSCRule_PC_RGB_SD_0_255_Main;
                }
                else // bInputDataRange16_235
                {
                    u16XRuleIdx = enFourceColor == PQ_FOURCE_COLOR_RGB ?
                                 PQ_CSCRule_PC_YUV_SD_Main :
                                 PQ_CSCRule_PC_RGB_SD_16_235_Main;
                }
            }
            else // InputResolutionHD
            {
                if (_bColorRange0_255[eWindow])
                {
                    u16XRuleIdx = enFourceColor == PQ_FOURCE_COLOR_RGB ?
                                 PQ_CSCRule_PC_YUV_HD_Main :
                                 PQ_CSCRule_PC_RGB_HD_0_255_Main;
                }
                else // bInputDataRange16_235
                {
                    u16XRuleIdx = enFourceColor == PQ_FOURCE_COLOR_RGB ?
                                 PQ_CSCRule_PC_YUV_HD_Main :
                                 PQ_CSCRule_PC_RGB_HD_16_235_Main;
                }
            }
        }
        else // InputColorSpaceYUV
        {
            if(enFourceColor == PQ_FOURCE_COLOR_RGB )
            {
                printf("InputColorSpace is YUV, Fource Color Space is RGB!!!\n");
                MS_ASSERT(0);
            }

            if (bInputResolutionHD == FALSE)
            {
                u16XRuleIdx = PQ_CSCRule_PC_YUV_SD_Main;
            }
            else // InputResolutionHD
            {
                u16XRuleIdx = PQ_CSCRule_PC_YUV_HD_Main;
            }
        }
    }

    for(u16XRuleIP=0; u16XRuleIP<MDrv_PQ_GetXRuleIPNum(eWindow, eXRuleType); u16XRuleIP++)
    {
        u16IPIdx = MDrv_PQ_GetXRuleIPIndex(eWindow, eXRuleType, u16XRuleIP);
        u16TabIdx = MDrv_PQ_GetXRuleTableIndex(eWindow, eXRuleType, u16XRuleIdx, u16XRuleIP);

        CSCRULE_DBG(printf("u16XRuleIdx:%u, u16XRuleIP=%u, IPIdx=%u, TabIdx=%u\n", u16XRuleIdx, u16XRuleIP, u16IPIdx, u16TabIdx));

        MDrv_PQ_LoadTable(eWindow, u16TabIdx, u16IPIdx);
        u16DoCSC |= u16TabIdx; // 0:OFF
    }

    u16DoCSC = (u16DoCSC != 0);
    if ( (bInputColorSpaceRGB && u16DoCSC) || (!bInputColorSpaceRGB) )
        return TRUE;    // use YUV space
    else
        return FALSE;   // use RGB space
}

static void _MDrv_PQ_LoadNRTable(PQ_WIN eWindow, MS_U16 u16PQ_NRIdx)
{
    MS_U16 i, u16IPIdx=0, u16TabIdx=0;

    if (u16PQ_NRIdx > PQ_GRULE_NR_NUM_Main)
    {
        //MS_ASSERT(0);
        printf("LoadNRTable PQ_NRIdx = %d is not included in enum PQ_GRULE_NR_NUM_Main\n", u16PQ_NRIdx);
        return;
    }

    for(i=0; i<PQ_GRULE_NR_IP_NUM_Main; i++)
    {
        u16IPIdx = MDrv_PQ_GetGRule_IPIndex(eWindow, PQ_GRule_NR_Main, i);

        if (u16PQ_NRIdx == PQ_GRULE_NR_NUM_Main) // see it as default
            u16TabIdx = MDrv_PQ_GetTableIndex(eWindow, u16IPIdx);
        else
            u16TabIdx = MDrv_PQ_GetGRule_TableIndex(eWindow, PQ_GRule_NR_Main, u16PQ_NRIdx, i);

        PQGRULE_DBG(printf("[NR]SRC: %u, NR: %u, NRIPIdx:%u, IPIdx:%u, u8TabIdx:%u\n",
            _u16PQSrcType[eWindow], u16PQ_NRIdx, i, u16IPIdx, u16TabIdx));

        MDrv_PQ_LoadTable(eWindow, u16TabIdx, u16IPIdx);
    }
}

void MDrv_PQ_LoadNRTable(PQ_WIN eWindow, PQ_3D_NR_FUNCTION_TYPE en3DNRType)
{
    PQGRULE_DBG(printf("[PQ_LoadNRTable] "));
    MS_U16 u16PQ_NRIdx;

    if( en3DNRType == PQ_3D_NR_OFF )
    {
        PQGRULE_DBG(printf("Off\n"));
        u16PQ_NRIdx = MDrv_PQ_GetGRule_LevelIndex(eWindow, PQ_GRule_NR_Main, PQ_GRule_NR_Off_Main);
        _MDrv_PQ_LoadNRTable(eWindow, u16PQ_NRIdx);
    }
#if 0  // ToDo
    else if( en3DNRType == PQ_3D_NR_AUTO )
    {
        PQGRULE_DBG(printf("Auto\n"));
        u16PQ_NRIdx = MDrv_PQ_GetGRule_LevelIndex(eWindow, PQ_GRule_NR_Main, PQ_GRule_NR_Auto_Main);
        _MDrv_PQ_LoadNRTable(eWindow, u16PQ_NRIdx);
    }
#endif
    else if( en3DNRType == PQ_3D_NR_LOW )
    {
        PQGRULE_DBG(printf("Low\n"));
        u16PQ_NRIdx = MDrv_PQ_GetGRule_LevelIndex(eWindow, PQ_GRule_NR_Main, PQ_GRule_NR_Low_Main);
        _MDrv_PQ_LoadNRTable(eWindow, u16PQ_NRIdx);
    }
    else if( en3DNRType == PQ_3D_NR_MID )
    {
        PQGRULE_DBG(printf("Mid\n"));
        u16PQ_NRIdx = MDrv_PQ_GetGRule_LevelIndex(eWindow, PQ_GRule_NR_Main, PQ_GRule_NR_Middle_Main);
        _MDrv_PQ_LoadNRTable(eWindow, u16PQ_NRIdx);
    }
    else if( en3DNRType == PQ_3D_NR_HIGH )
    {
        PQGRULE_DBG(printf("High\n"));
        u16PQ_NRIdx = MDrv_PQ_GetGRule_LevelIndex(eWindow, PQ_GRule_NR_Main, PQ_GRule_NR_High_Main);
        _MDrv_PQ_LoadNRTable(eWindow, u16PQ_NRIdx);
    }
    else if ( en3DNRType == PQ_3D_NR_DEFAULT )
    {
        PQGRULE_DBG(printf("Default\n"));
        _MDrv_PQ_LoadNRTable(eWindow, PQ_GRULE_NR_NUM_Main);
    }
    else
    {
        //MS_ASSERT(0);
        printf("LoadNRTable 3DNRType = %d is not included in enum PQ_3D_NR_NUM\n", en3DNRType);
    }
}

#if PQ_GRULE_STB_PQ_ENABLE
static void _MDrv_PQ_LoadSTBQualityTable(PQ_WIN eWindow, MS_U16 u16PQ_STBQualityIdx)
{
    MS_U16 i, u16IPIdx=0, u16TabIdx=0;

    if (u16PQ_STBQualityIdx > PQ_GRULE_STB_PQ_NUM_Main)
    {
        //MS_ASSERT(0);
        printf("LoadSTBQualityTable u16PQ_STBQualityIdx = %d is not included in enum PQ_GRULE_STB_PQ_NUM_Main\n", u16PQ_STBQualityIdx);
        return;
    }

    for(i=0; i<PQ_GRULE_STB_PQ_IP_NUM_Main; i++)
    {
        u16IPIdx = MDrv_PQ_GetGRule_IPIndex(eWindow, PQ_GRule_STB_PQ_Main, i);

        if (u16PQ_STBQualityIdx == PQ_GRULE_STB_PQ_NUM_Main) // see it as default
            u16TabIdx = MDrv_PQ_GetTableIndex(eWindow, u16IPIdx);
        else
            u16TabIdx = MDrv_PQ_GetGRule_TableIndex(eWindow, PQ_GRule_STB_PQ_Main, u16PQ_STBQualityIdx, i);

        PQGRULE_DBG(printf("[STB_PQ]SRC: %u, STB Quality: %u, STB_PQ IPIdx:%u, IPIdx:%u, u8TabIdx:%u\n",
            _u16PQSrcType[eWindow], u16PQ_STBQualityIdx, i, u16IPIdx, u16TabIdx));

        MDrv_PQ_LoadTable(eWindow, u16TabIdx, u16IPIdx);
    }
}

void MDrv_PQ_LoadSTBQualityTable(PQ_WIN eWindow, PQ_STB_QUALITY_TYPE enSTBQualityType)
{
    PQGRULE_DBG(printf("[PQ_LoadSTBQualityTable] "));
    MS_U16 u16PQ_STBQualityIdx;

    if( enSTBQualityType == PQ_STB_QUALITY_OFF )
    {
        PQGRULE_DBG(printf("Off\n"));
        u16PQ_STBQualityIdx = MDrv_PQ_GetGRule_LevelIndex(eWindow, PQ_GRule_STB_PQ_Main, PQ_GRule_STB_PQ_Off_Main);
        _MDrv_PQ_LoadSTBQualityTable(eWindow, u16PQ_STBQualityIdx);
    }
    else if( enSTBQualityType == PQ_STB_QUALITY_LEVEL1 )
    {
        PQGRULE_DBG(printf("Level 1\n"));
        u16PQ_STBQualityIdx = MDrv_PQ_GetGRule_LevelIndex(eWindow, PQ_GRule_STB_PQ_Main, PQ_GRule_STB_PQ_Level1_Main);
        _MDrv_PQ_LoadSTBQualityTable(eWindow, u16PQ_STBQualityIdx);
    }
    else if( enSTBQualityType == PQ_STB_QUALITY_LEVEL2 )
    {
        PQGRULE_DBG(printf("Level 2\n"));
        u16PQ_STBQualityIdx = MDrv_PQ_GetGRule_LevelIndex(eWindow, PQ_GRule_STB_PQ_Main, PQ_GRule_STB_PQ_Level2_Main);
        _MDrv_PQ_LoadSTBQualityTable(eWindow, u16PQ_STBQualityIdx);
    }
    else
    {
        //MS_ASSERT(0);
        printf("LoadSTBQualityTable STBQualityType = %d is not included in enum PQ_STB_QUALITY_NUM\n", enSTBQualityType);
    }
}
#endif

void MDrv_PQ_LoadDeFlickerTable(PQ_WIN eWindow, PQ_DE_FLICKER_FUNCTION_TYPE enDeFlickType)
{
    PQGRULE_DBG(printf("[MDrv_PQ_LoadDeFlickerTable] "));
    MDrv_PQ_LoadDeFlickerTable_(MAIN,(MS_U8)enDeFlickType);
	return;
}

#if 0 //PQ_GRULE_OSD_BW_ENABLE
static void _MDrv_PQ_LoadOSD_BWTable(PQ_WIN eWindow, MS_U16 u16PQ_OSD_BW_Idx)
{
    MS_U16 i, u16IPIdx=0, u16TabIdx=0;

    if (u16PQ_OSD_BW_Idx > PQ_GRULE_OSD_BW_NUM_Main){
        MS_ASSERT(0); return;
    }

    for(i=0; i<PQ_GRULE_OSD_BW_IP_NUM_Main; i++)
    {
        u16IPIdx = MDrv_PQ_GetGRule_IPIndex(eWindow, PQ_GRule_OSD_BW_Main, i);

        if (u16PQ_OSD_BW_Idx == PQ_GRULE_OSD_BW_NUM_Main) // see it as default
            u16TabIdx = MDrv_PQ_GetTableIndex(eWindow, u16IPIdx);
        else
            u16TabIdx = MDrv_PQ_GetGRule_TableIndex(eWindow, PQ_GRule_OSD_BW_Main, u16PQ_OSD_BW_Idx, i);

        PQGRULE_DBG(printf("[NR]SRC: %u, NR: %u, NRIPIdx:%u, IPIdx:%u, u8TabIdx:%u\n",
            _u16PQSrcType[eWindow], u16PQ_OSD_BW_Idx, i, u16IPIdx, u16IPIdx));

        MDrv_PQ_LoadTable(eWindow, u16TabIdx, u16IPIdx);
    }
}


void MDrv_PQ_LoadOSD_BWTable(PQ_WIN eWindow, PQ_OSD_BW_FUNCTION_TYPE enOSD_BW_Type)
{
    PQGRULE_DBG(printf("[PQ_LoadNRTable] "));
    MS_U16 u16PQ_NRIdx;

     if( enOSD_BW_Type == PQ_OSD_BW_ON )
     {
         PQGRULE_DBG(printf("Off\n"));
         u16PQ_NRIdx = MDrv_PQ_GetGRule_LevelIndex(eWindow, PQ_GRule_OSD_BW_Main, PQ_GRule_Lvl_OSD_BW_On_Main);
         _MDrv_PQ_LoadOSD_BWTable(eWindow, u16PQ_NRIdx);
     }
     else
     {
         MS_ASSERT(0);
     }
}
#endif

static MS_U32 _CalUsageBW(BW_INFO_t *bw_infom)
{
    MS_U32 u32BW_IPM_R, u32BW_IPM_W, u32BW_OPM_R, u32BW_OSD_R, u32UsageBW;

    u32BW_IPM_R = u32BW_IPM_W = u32BW_OPM_R = 0;
    if (!bw_infom->bFBL)
    {
        // IPM
        u32BW_IPM_R = bw_infom->u16input_hsize * bw_infom->u16input_vtotal
                      /10*bw_infom->u16input_vfreq /8*bw_infom->u8BitsPerPixel;

        u32BW_IPM_W = u32BW_IPM_R;

        // OPM
        u32BW_OPM_R = bw_infom->u16output_hsize * bw_infom->u16output_vtotal
                      /10*bw_infom->u16output_vfreq /8*bw_infom->u8BitsPerPixel;
        if (bw_infom->bInterlace && bw_infom->b4RMode)
            u32BW_OPM_R *= 2;
    }

    if (bw_infom->bOSD_On && (bw_infom->bSC_MIUSel == bw_infom->bGOP_MIUSel))
    {
        u32BW_OSD_R = bw_infom->u16OSD_hsize * bw_infom->u16output_vtotal
                      /10*bw_infom->u16output_vfreq /8*bw_infom->u8OSD_BitsPerPixel;
    }
    else
    {
        u32BW_OSD_R = 0;
    }
    u32UsageBW = u32BW_IPM_W + u32BW_IPM_W + u32BW_OPM_R + u32BW_OSD_R + bw_infom->u32PVR_BW
               + bw_infom->u32MISC_BW;


    PQBW_DBG(printf("fbl:%u, 422: %u, osd:%u, i:%u, 4R: %u, bpp:%u\n",
            bw_infom->bFBL,
            bw_infom->bMemFmt422,
            bw_infom->bOSD_On,
            bw_infom->bInterlace,
            bw_infom->b4RMode,
            bw_infom->u8BitsPerPixel));
    PQBW_DBG(printf("input: hact:%u, vtotal:%u, vfreq:%u\n",
            bw_infom->u16input_hsize,
            bw_infom->u16input_vtotal,
            bw_infom->u16input_vfreq));
    PQBW_DBG(printf("output: hact:%u, vtotal:%u, vfreq:%u\n",
            bw_infom->u16output_hsize,
            bw_infom->u16output_vtotal,
            bw_infom->u16output_vfreq));
    PQBW_DBG(printf("IPM_R:%ld, IPM_W:%ld, OPM_R:%ld, GOP:%ld, MISC: %ld, PVR: %ld, usage:%ld, available:%ld\n",
            u32BW_IPM_R/1000000,
            u32BW_IPM_W/1000000,
            u32BW_OPM_R/1000000,
            u32BW_OSD_R/1000000,
            bw_infom->u32MISC_BW/1000000,
            bw_infom->u32PVR_BW/1000000,
            u32UsageBW/1000000,
            bw_infom->u32MemBW/1000000
            ));

    return u32UsageBW;
}


MS_U8 _MDrv_PQ_wait_output_vsync(PQ_WIN ePQWin, MS_U8 u8NumVSyncs, MS_U16 u16Timeout)
{
    MS_U32 u32Time;
    MS_U8 bVSync;
    MS_BOOL bMainWin = (ePQWin == PQ_MAIN_WINDOW) ? TRUE : FALSE;

    bVSync = 0;
    u32Time = MsOS_GetSystemTime();

    while (1)
    {
        if (Hal_PQ_get_output_vsync_value(bMainWin) == bVSync)
        {
            --u8NumVSyncs;
            if ( (bVSync) && (u8NumVSyncs == 0) )
                break;
            bVSync = !bVSync;
        }

        if ( ( MsOS_GetSystemTime() - u32Time   )  >= u16Timeout )
            break;
    }

    return u8NumVSyncs;
}

MS_U8 _MDrv_PQ_wait_input_vsync(PQ_WIN ePQWin,  MS_U8 u8NumVSyncs, MS_U16 u16Timeout)
{
    MS_U32 u32Time;
    MS_BOOL bMainWin = (ePQWin == PQ_MAIN_WINDOW) ? TRUE : FALSE;
    MS_U8 u8VsyncPolarity = Hal_PQ_get_input_vsync_polarity(bMainWin);
    MS_U8 bVSync = !u8VsyncPolarity;

    u32Time = MsOS_GetSystemTime();

    while (1)
    {
        if (Hal_PQ_get_input_vsync_value(bMainWin) == bVSync)
        {
            --u8NumVSyncs;
            if ((bVSync == u8VsyncPolarity) && (u8NumVSyncs == 0))
                break;
            bVSync = !bVSync;
        }

        if ( ( MsOS_GetSystemTime() - u32Time ) >= u16Timeout)
        {
            //printf("!!input vsync timeout\n");
            break;
        }
    }

    return u8NumVSyncs;
}

void _MDrv_PQ_Calc_BufSize(PQ_WIN eWindow, BW_INFO_t *pbwinfo)
{
    MS_U32 u32hsize = _stMode_Info[eWindow].u16input_hsize;
    MS_U32 u32vsize = _stMode_Info[eWindow].u16input_vsize;

    //check to see whether the size of input data exceeds the dnr buffer size.
    //for example, 1080p 25fps, fullHD, RGB/YUV 444 10-bits format may exceed if

    if( (pbwinfo->bInterlace == FALSE) && (pbwinfo->bMemFmt422 == FALSE) )
    {
        while(1)
        {
            //dnr buffer size < width * height * 4 *2
            if(u32hsize * u32vsize * (pbwinfo->u8BitsPerPixel/8*2) <=  _u32SCBuf_Size[eWindow] )
            {
                break;
            }

            PQBW_DBG(printf("hsize =%ld, vsize=%ld, bpp=%d, CurSize=%ld\n",
                u32hsize,
                u32vsize,
                pbwinfo->u8BitsPerPixel, u32hsize * u32vsize * (pbwinfo->u8BitsPerPixel/8*2) ));

            if(pbwinfo->u8BitsPerPixel == 32)
            {
                pbwinfo->u8BitsPerPixel = 24;
            }
            else
            {
                PQBW_DBG(printf("[PQ_Calc_BufSize]reduce quality fail: 444 p mode\n"));
                MS_ASSERT(0);
                break;
            }

        }
    }
}

#if 0
MS_BOOL _IsFrameBufferExceeded(BW_INFO_t *pbw_info)
{
    //check to see whether the size of input data exceeds the dnr buffer size.
    //for example, 1080p 25fps, fullHD, RGB/YUV 444 10-bits format may exceed if
    //dnr buffer size < width * height * 4 *2
    if (!pbw_info->bInterlace)
    {
        if (pbw_info->u16input_hsize*pbw_info->u16input_vtotal*pbw_info->u8BitsPerPixel*2>SCALER_DNR_BUF_LEN*8)
        {
            return TRUE;
        }
    }
    return FALSE;
}
#endif

void _MDrv_PQ_Calc_BW(BW_INFO_t *pbwinfo)
{
    MS_U32 u32UsageBW;


    while(1)
    {
        u32UsageBW = _CalUsageBW(pbwinfo);

        if ((pbwinfo->u32MemBW > u32UsageBW))
            break;

        if (pbwinfo->bInterlace)
        {
            if (pbwinfo->b4RMode)
            {
                pbwinfo->b4RMode = FALSE;
            }
            else
            {
                PQBW_DBG(printf("reduce quality fail: 422 interlace\n"));
                MS_ASSERT(0);
                break;
            }
        }
        else
        {
            // progressive
            if (pbwinfo->bMemFmt422)
            {
                if (pbwinfo->u8BitsPerPixel == 24)
                {
                    pbwinfo->u8BitsPerPixel = 20;
                }
                else if (pbwinfo->u8BitsPerPixel == 20)
                {
                    pbwinfo->u8BitsPerPixel = 16;
                }
                else
                {
                    PQBW_DBG(printf("reduce quality fail: 422 p mode\n"));
                    MS_ASSERT(0);
                    break;
                }
            }
            else // 444 format
            {
                if (pbwinfo->u8BitsPerPixel == 32)
                {
                    pbwinfo->u8BitsPerPixel = 24;
                }
                else
                {
                    PQBW_DBG(printf("reduce quality fail: 444 p mode\n"));
                    MS_ASSERT(0);
                    break;
                }
            }
        }
    }
}

#if (PQ_XRULE_DB_ENABLE == 1)
void MDrv_PQ_Set_DBRule(PQ_WIN eWindow, MS_U16 u16MADi_Idx)
{
    MS_U16 eXRuleType = E_XRULE_CSC;
    MS_U16 u16XRuleIdx, u16XRuleIP;
    MS_U16 u16IPIdx, u16TabIdx;

    eXRuleType = (_stMode_Info[eWindow].u16input_vfreq > 550) ? E_XRULE_DB_NTSC : E_XRULE_DB_PAL;

    if(Hal_PQ_get_madi_idx(MS_MADI_24_4R) == u16MADi_Idx)
    {
        u16XRuleIdx = PQ_DBRule_NTSC_24_4R_Main;
    }
    else if(Hal_PQ_get_madi_idx(MS_MADI_24_2R) == u16MADi_Idx)
    {
        u16XRuleIdx = PQ_DBRule_NTSC_24_2R_Main;
    }
    else if(Hal_PQ_get_madi_idx(MS_MADI_25_4R_MC) == u16MADi_Idx)
    {
        u16XRuleIdx = PQ_DBRule_NTSC_25_4R_MC_Main;
    }
    else if(Hal_PQ_get_madi_idx(MS_MADI_25_4R) == u16MADi_Idx)
    {
        u16XRuleIdx = PQ_DBRule_NTSC_25_4R_Main;
    }
    else if(Hal_PQ_get_madi_idx(MS_MADI_25_2R) == u16MADi_Idx)
    {
        u16XRuleIdx = PQ_DBRule_NTSC_25_2R_Main;
    }
    else if(Hal_PQ_get_madi_idx(MS_MADI_27_4R) == u16MADi_Idx)
    {
        u16XRuleIdx = PQ_DBRule_NTSC_27_4R_Main;
    }
    else if(Hal_PQ_get_madi_idx(MS_MADI_27_2R) == u16MADi_Idx)
    {
        u16XRuleIdx = PQ_DBRule_NTSC_27_2R_Main;
    }
    else if(Hal_PQ_get_madi_idx(MS_MADI_P_MODE8) == u16MADi_Idx)
    {
        u16XRuleIdx = PQ_DBRule_NTSC_P_MODE8_Main;
    }
    else if(Hal_PQ_get_madi_idx(MS_MADI_P_MODE10) == u16MADi_Idx)
    {
        u16XRuleIdx = PQ_DBRule_NTSC_P_MODE10_Main;
    }
    else if(Hal_PQ_get_madi_idx(MS_MADI_P_MODE_MOT8) == u16MADi_Idx)
    {
        u16XRuleIdx = PQ_DBRule_NTSC_P_MODE_MOT8_Main;
    }
    else if(Hal_PQ_get_madi_idx(MS_MADI_P_MODE_MOT10) == u16MADi_Idx)
    {
        u16XRuleIdx = PQ_DBRule_NTSC_P_MODE_MOT10_Main;
    }
    else if(Hal_PQ_get_madi_idx(MS_MADI_24_4R_880) == u16MADi_Idx)
    {
        u16XRuleIdx = PQ_DBRule_NTSC_24_4R_880_Main;
    }
    else if(Hal_PQ_get_madi_idx(MS_MADI_24_2R_880) == u16MADi_Idx)
    {
        u16XRuleIdx = PQ_DBRule_NTSC_24_2R_880_Main;
    }
    else if(Hal_PQ_get_madi_idx(MS_MADI_25_6R_MC) == u16MADi_Idx)
    {
        u16XRuleIdx = PQ_DBRule_NTSC_25_6R_MC_Main;
    }
    else if(Hal_PQ_get_madi_idx(MS_MADI_25_4R_MC_NW) == u16MADi_Idx)
    {
        u16XRuleIdx = PQ_DBRule_NTSC_25_4R_MC_NW_Main;
    }
    else if(Hal_PQ_get_madi_idx(MS_MADI_25_6R_MC_NW) == u16MADi_Idx)
    {
        u16XRuleIdx = PQ_DBRule_NTSC_25_6R_MC_NW_Main;
    }
    else if(Hal_PQ_get_madi_idx(MS_MADI_25_4R_884) == u16MADi_Idx)
    {
        u16XRuleIdx = PQ_DBRule_NTSC_25_4R_884_Main;
    }
    else if(Hal_PQ_get_madi_idx(MS_MADI_25_2R_884) == u16MADi_Idx)
    {
        u16XRuleIdx = PQ_DBRule_NTSC_25_2R_884_Main;
    }
    else if(Hal_PQ_get_madi_idx(MS_MADI_25_4R_880) == u16MADi_Idx)
    {
        u16XRuleIdx = PQ_DBRule_NTSC_24_4R_880_Main;
    }
    else if(Hal_PQ_get_madi_idx(MS_MADI_25_2R_880) == u16MADi_Idx)
    {
        u16XRuleIdx = PQ_DBRule_NTSC_24_2R_880_Main;
    }
    else
    {
        u16XRuleIdx = 0xFF;
    }

    for(u16XRuleIP=0; u16XRuleIP<MDrv_PQ_GetXRuleIPNum(eWindow, eXRuleType); u16XRuleIP++)
    {
        if(u16XRuleIdx == 0xFF)
            continue;

        u16IPIdx = MDrv_PQ_GetXRuleIPIndex(eWindow, eXRuleType, u16XRuleIP);
        u16TabIdx = MDrv_PQ_GetXRuleTableIndex(eWindow, eXRuleType, u16XRuleIdx, u16XRuleIP);
        //(printf("u16XRuleIdx:%u, u16XRuleIP=%u, IPIdx=%u, TabIdx=%u\n", u16XRuleIdx, u16XRuleIP, u16IPIdx, u16TabIdx));

        MDrv_PQ_LoadTable(eWindow, u16TabIdx, u16IPIdx);
    }
}
#endif

MS_BOOL MDrv_PQ_SuggestFrameNum(PQ_WIN eWindow, MS_U16 u16TabIdx_MADi, MS_U8 u8FrameCount)
{
    //the chips, who are older than SCMI_V2, set frame count reg by Qmap.
    //the chips, who are SCMI_V2, the frame count is suggested by Qmap, but sw code judge and replace the value.
    //      and then, set to hw reg
    //note: SUPPORT_SCMI_V2 is corresponding withe utopia flag: _FIELD_PACKING_MODE_SUPPORTED
    //      we can remove SUPPORT_SCMI_V2 later
    if(MApi_XC_IsFieldPackingModeSupported())
    {
        //Set Frame number
        UNUSED(u16TabIdx_MADi);
        if(bSetFrameCount)
        {
            _u16PQSuggestedFrameNum[eWindow] = u8FrameCount;
    	    PQBW_DBG(printf("[%s %d] _u16PQSuggestedFrameNum[%u] = %u\n", __FILE__, (int)__LINE__, eWindow, u8FrameCount));
        }
    }
    return TRUE;
}

//////////////////////////////////////////////////////////////////
// Load MADi/444To422/422To444 table
//
// parameter:
//    [IN]  u8PQTabType
//          bMemFmt422: TRUE  - 422
//                      FALSE - 444
//          bFBL:       TRUE  - framebuffer-less mode
//                      FALSE - framebuffer mode
//    [OUT] u8BitsPerPixel
//                      bits per pixel for decided memory format
//
// return:  deinterlace (MADi) mode
//
PQ_DEINTERLACE_MODE MDrv_PQ_SetMemFormat(PQ_WIN eWindow, MS_BOOL bMemFmt422, MS_BOOL bFBL, MS_U8 *u8BitsPerPixel)
{
    MS_U8 u8memory_data_fmt = MEMFMT24BIT_CLEAR;
    MS_U8 u8FrameCount = 4;
    MS_U16 u16TabIdx_MemFormat=0;
    MS_U16 u16TabIdx_MADi=0;
    MS_U16 u16TabIdx_444To422=0;
    MS_U16 u16TabIdx_422To444=0;
    PQ_DEINTERLACE_MODE eDeInterlaceMode=PQ_DEINT_OFF;
    MS_BOOL bMainWin = (eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE;

    PQBW_DBG(printf("==>In PQ : eWindow =%d\n", eWindow));

    if (!bFBL)
    {
        // if MADi mode change from 25 <=> 27, cropping base address need to be recalculated.
    #if PQ_GRULE_OSD_BW_ENABLE

        if(_bOSD_On)
        {
            MS_U16 u16PQ_OSDBWIdx = 0;
            u16PQ_OSDBWIdx = MDrv_PQ_GetGRule_LevelIndex(eWindow, PQ_GRule_OSD_BW_Main, PQ_GRule_Lvl_OSD_BW_On_Main);
            u16TabIdx_MADi = MDrv_PQ_GetGRule_TableIndex(eWindow, PQ_GRule_OSD_BW_Main, u16PQ_OSDBWIdx, 0); // only MADi ip
        }
        else
    #endif
        {
            u16TabIdx_MADi = MDrv_PQ_GetTableIndex(eWindow, PQ_IP_MADi_Main);
        }
    }
    else // FBL
    {
        MS_U8 u8FblMode = Hal_PQ_get_madi_fbl_mode(bMemFmt422,_stMode_Info[eWindow].bInterlace);
        u16TabIdx_MADi = Hal_PQ_get_madi_idx(u8FblMode);
    }

    if(_stRFBL_Info.bEnable)
    {
        u16TabIdx_MADi = Hal_PQ_get_madi_idx((MS_MADI_TYPE)_stRFBL_Info.u8MADiType);
        //printf("===============================u16TabIdx_MADi = %d\n",u16TabIdx_MADi);
    }

    //printf("FBL = %s", (bFBL)?("Yes"):("No"));
    //printf(", get madi idx = %x\n", u16TabIdx_MADi);
    if (bMemFmt422)
    {
        u16TabIdx_MemFormat = MDrv_PQ_GetTableIndex(eWindow, PQ_IP_MemFormat_Main);
        u16TabIdx_444To422 = PQ_IP_444To422_ON_Main;
        u16TabIdx_422To444 = PQ_IP_422To444_ON_Main;
    }
    else
    {
        // use 444_10BIT mode only if < 1440x1080
        // frame buffer 1920x1080x3x2 == 1440x1080x4x2
        // also bandwidth not enough for 1920x1080 444_10BIT
        u16TabIdx_MemFormat = MDrv_PQ_GetTableIndex(eWindow, PQ_IP_MemFormat_Main);
        u16TabIdx_444To422 = PQ_IP_444To422_OFF_Main;
        u16TabIdx_422To444 = PQ_IP_422To444_OFF_Main;
    }
    //printf("MemFmt is 422 : %s\n",(bMemFmt422)?("Yes"):("No"));
    if (bMemFmt422)
    {
        // decide bits/pixel and deinterlace mode flag
        if (u16TabIdx_MADi == Hal_PQ_get_madi_idx(MS_MADI_24_4R))
        {
            bw_info.b4RMode = TRUE;
            *u8BitsPerPixel = 20;
            eDeInterlaceMode = PQ_DEINT_3DDI_HISTORY;
            PQINFO_DBG(printf("24 4 PQ_DEINT_3DDI_HISTORY\n"));
            //printf("24 4 PQ_DEINT_3DDI_HISTORY\n");
        }
        else if (u16TabIdx_MADi == Hal_PQ_get_madi_idx(MS_MADI_24_2R))
        {
            bw_info.b4RMode = FALSE;
            *u8BitsPerPixel = 20;
            eDeInterlaceMode = PQ_DEINT_3DDI_HISTORY;
            PQINFO_DBG(printf("24 2 PQ_DEINT_3DDI_HISTORY\n"));
            //printf("24 2 PQ_DEINT_3DDI_HISTORY\n");
        }
        else if (u16TabIdx_MADi == Hal_PQ_get_madi_idx(MS_MADI_25_4R))
        {
            bw_info.b4RMode = TRUE;
            *u8BitsPerPixel = 24;
            eDeInterlaceMode = PQ_DEINT_3DDI_HISTORY;
            //printf("25 4 PQ_DEINT_3DDI_HISTORY\n");
        }
        else if (u16TabIdx_MADi == Hal_PQ_get_madi_idx(MS_MADI_25_2R))
        {
            bw_info.b4RMode = FALSE;
            *u8BitsPerPixel = 24;
            eDeInterlaceMode = PQ_DEINT_3DDI_HISTORY;
            //printf("25 2 PQ_DEINT_3DDI_HISTORY\n");
        }
        else if (u16TabIdx_MADi == Hal_PQ_get_madi_idx(MS_MADI_26_4R))
        {
            bw_info.b4RMode = TRUE;
            *u8BitsPerPixel = 20;
            eDeInterlaceMode = PQ_DEINT_3DDI_HISTORY;
            //printf("26 4 PQ_DEINT_3DDI_HISTORY\n");
        }
        else if (u16TabIdx_MADi == Hal_PQ_get_madi_idx(MS_MADI_26_2R))
        {
            bw_info.b4RMode = FALSE;
            *u8BitsPerPixel = 20;
            eDeInterlaceMode = PQ_DEINT_3DDI_HISTORY;
            //printf("26 2 PQ_DEINT_3DDI_HISTORY\n");
        }
        else if (u16TabIdx_MADi == Hal_PQ_get_madi_idx(MS_MADI_27_4R))
        {
            bw_info.b4RMode = TRUE;
            *u8BitsPerPixel = 16;
            eDeInterlaceMode = PQ_DEINT_3DDI;
            PQINFO_DBG(printf("27 4 PQ_DEINT_3DDI\n"));
            //printf("27 4 PQ_DEINT_3DDI\n");
        }
        else if (u16TabIdx_MADi == Hal_PQ_get_madi_idx(MS_MADI_27_2R))
        {
            bw_info.b4RMode = FALSE;
            *u8BitsPerPixel = 16;
            eDeInterlaceMode = PQ_DEINT_3DDI;
            PQINFO_DBG(printf("27 2 PQ_DEINT_3DDI\n"));
            //printf("27 2 PQ_DEINT_3DDI\n");
        }
        else if (u16TabIdx_MADi == Hal_PQ_get_madi_idx(MS_MADI_P_MODE8))
        {
            bw_info.b4RMode = FALSE;
            *u8BitsPerPixel = 16;
            eDeInterlaceMode = PQ_DEINT_2DDI_BOB;
            
	    if(MApi_XC_GetIPAutoNoSignal((SCALER_WIN)eWindow)==ENABLE)
            {
                u8FrameCount = 3;
            }
            else
            {
                u8FrameCount = 2;
            }
            PQINFO_DBG(printf("P 8 PQ_DEINT_2DDI_BOB\n"));
            //printf("P 8 PQ_DEINT_2DDI_BOB\n");
        }
        else if (u16TabIdx_MADi == Hal_PQ_get_madi_idx(MS_MADI_P_MODE10))
        {
            bw_info.b4RMode = FALSE;
            *u8BitsPerPixel = 20;
            
	    if(MApi_XC_GetIPAutoNoSignal((SCALER_WIN)eWindow)==ENABLE)
            {
                u8FrameCount = 3;
            }
            else
            {
                u8FrameCount = 2;
            }
            eDeInterlaceMode = PQ_DEINT_2DDI_BOB;
            //printf("P 10 PQ_DEINT_2DDI_BOB\n");
        }
        else if (u16TabIdx_MADi == Hal_PQ_get_madi_idx(MS_MADI_P_MODE_MOT8))
        {
            bw_info.b4RMode = FALSE;
            *u8BitsPerPixel = 20;
            eDeInterlaceMode = PQ_DEINT_2DDI_BOB;
            
	    if(MApi_XC_GetIPAutoNoSignal((SCALER_WIN)eWindow)==ENABLE)
            {
                u8FrameCount = 3;
            }
            else
            {
                u8FrameCount = 2;
            }
            //printf("P M 8 PQ_DEINT_2DDI_BOB\n");
        }
        else if (u16TabIdx_MADi == Hal_PQ_get_madi_idx(MS_MADI_P_MODE_MOT10))
        {
            bw_info.b4RMode = FALSE;
            *u8BitsPerPixel = 24;
            eDeInterlaceMode = PQ_DEINT_2DDI_BOB;
            
	    if(MApi_XC_GetIPAutoNoSignal((SCALER_WIN)eWindow)==ENABLE)
            {
                u8FrameCount = 3;
            }
            else
            {
                u8FrameCount = 2;
            }
            //printf("P M 10 PQ_DEINT_2DDI_BOB\n");
        }
        else if (u16TabIdx_MADi == Hal_PQ_get_madi_idx(MS_MADI_24_RFBL_FILM))
        {
            bw_info.b4RMode = FALSE;
            *u8BitsPerPixel = 12;
            eDeInterlaceMode = PQ_DEINT_3DDI_HISTORY;
            printf("====================================   MS_MADI_24_RFBL_FILM\n");
            //printf("24 PQ_DEINT_3DDI_HISTORY\n");
        }
        else if (u16TabIdx_MADi == Hal_PQ_get_madi_idx(MS_MADI_24_RFBL_NFILM))
        {
            bw_info.b4RMode = FALSE;
            *u8BitsPerPixel = 12;
            eDeInterlaceMode = PQ_DEINT_3DDI_HISTORY;
            Hal_PQ_set_force_c_motion(TRUE, 0x0080);
            PQINFO_DBG(printf("====================================   MS_MADI_24_RFBL_NFILM\n"));
            //printf("====================================   MS_MADI_24_RFBL_NFILM\n");
            //printf("24 PQ_DEINT_3DDI_HISTORY\n");
        }
        else if (u16TabIdx_MADi == Hal_PQ_get_madi_idx(MS_MADI_FBL_DNR))
        {
            bw_info.b4RMode = FALSE;
            *u8BitsPerPixel = 16;
            eDeInterlaceMode = PQ_DEINT_2DDI_AVG;
            PQINFO_DBG(printf("====================================   MS_MADI_FBL_DNR\n"));
            //printf("====================================   MS_MADI_FBL_DNR\n");
            //printf("24 PQ_DEINT_2DDI_AVG\n");
        }
        else if (u16TabIdx_MADi == Hal_PQ_get_madi_idx(MS_MADI_FBL_MIU))
        {
            bw_info.b4RMode = FALSE;
            *u8BitsPerPixel = 16;
            eDeInterlaceMode = PQ_DEINT_2DDI_AVG;
            PQINFO_DBG(printf("====================================   MS_MADI_FBL_MIU\n"));
            //printf("====================================   MS_MADI_FBL_MIU\n");
            //printf("24 PQ_DEINT_2DDI_AVG\n");
        }
        else if (u16TabIdx_MADi == Hal_PQ_get_madi_idx(MS_MADI_P_MODE8_NO_MIU))
        {
            bw_info.b4RMode = FALSE;
            *u8BitsPerPixel = 16;
            eDeInterlaceMode = PQ_DEINT_2DDI_AVG;
            PQINFO_DBG(printf("====================================   MS_MADI_P_MODE8_NO_MIU\n"));
            //printf("====================================   MS_MADI_P_MODE8_NO_MIU\n");
            //printf("24 PQ_DEINT_2DDI_AVG\n");
        }
        else if (u16TabIdx_MADi == Hal_PQ_get_madi_idx(MS_MADI_25_4R_MC))
        {
            bw_info.b4RMode = TRUE;
            *u8BitsPerPixel = 24;
            eDeInterlaceMode = PQ_DEINT_3DDI_HISTORY;
        }
        else if (u16TabIdx_MADi == Hal_PQ_get_madi_idx(MS_MADI_25_6R_MC))
        {
            bw_info.b4RMode = TRUE;
            *u8BitsPerPixel = 24;
            u8FrameCount = 8;
            eDeInterlaceMode = PQ_DEINT_3DDI_HISTORY;
        }
        else if (u16TabIdx_MADi == Hal_PQ_get_madi_idx(MS_MADI_25_14F_8R_MC))
        {
            bw_info.b4RMode = TRUE;
            *u8BitsPerPixel = 24;
            u8FrameCount = 14;
            eDeInterlaceMode = PQ_DEINT_3DDI_HISTORY;
        }
        else if (u16TabIdx_MADi == Hal_PQ_get_madi_idx(MS_MADI_25_12F_8R_MC))
        {
            bw_info.b4RMode = TRUE;
            *u8BitsPerPixel = 24;
            u8FrameCount = 12;
            eDeInterlaceMode = PQ_DEINT_3DDI_HISTORY;
        }
        else if (u16TabIdx_MADi == Hal_PQ_get_madi_idx(MS_MADI_25_16F_8R_MC))
        {
            bw_info.b4RMode = TRUE;
            *u8BitsPerPixel = 24;
            u8FrameCount = 16;
            eDeInterlaceMode = PQ_DEINT_3DDI_HISTORY;
        }
        else if (u16TabIdx_MADi == Hal_PQ_get_madi_idx(MS_MADI_25_6R_MC_NW))
        {
            bw_info.b4RMode = TRUE;
            *u8BitsPerPixel = 24;
            u8FrameCount = 8;
            eDeInterlaceMode = PQ_DEINT_3DDI_HISTORY;
        }
        else if (u16TabIdx_MADi == Hal_PQ_get_madi_idx(MS_MADI_P_MODE8_3Frame))
        {
            bw_info.b4RMode = FALSE;
            *u8BitsPerPixel = 16;
            eDeInterlaceMode = PQ_DEINT_2DDI_BOB;
        }
        else if (u16TabIdx_MADi == Hal_PQ_get_madi_idx(MS_MADI_24_4R_P))
        {
            bw_info.b4RMode = TRUE;
            *u8BitsPerPixel = 20;
            eDeInterlaceMode = PQ_DEINT_3DDI_HISTORY;
        }
        else if (u16TabIdx_MADi == Hal_PQ_get_madi_idx(MS_MADI_24_2R_P))
        {
            bw_info.b4RMode = FALSE;
            *u8BitsPerPixel = 20;
            eDeInterlaceMode = PQ_DEINT_3DDI_HISTORY;
            PQINFO_DBG(printf("24 2 P PQ_DEINT_3DDI_HISTORY\n"));
        }
        else
        {
            MS_ASSERT(0);
            //printf("None\n");
        }

    }
    else
    {
        if(u16TabIdx_MemFormat == PQ_IP_MemFormat_444_10BIT_Main)
        {
            *u8BitsPerPixel = 32;
			
	    if(MApi_XC_GetIPAutoNoSignal((SCALER_WIN)eWindow)==ENABLE)
            {
        	u8FrameCount = 3;
            }
            else
            {
        	u8FrameCount = 2;
            }
        }
        else
        {
            *u8BitsPerPixel = 24;
            u8FrameCount = 2;
        }
    }
    //printf("4R mode :%s, BitPerPixel =%d, DIMode =%x\n", (bw_info.b4RMode)?("Yes"):("No"), *u8BitsPerPixel, eDeInterlaceMode);

    _gbMemfmt422[eWindow] = bMemFmt422;
    // input timing
    bw_info.u16input_hsize  = _stMode_Info[eWindow].u16input_hsize;
    bw_info.u16input_vtotal = _stMode_Info[eWindow].u16input_vtotal;
    bw_info.u16input_vfreq  = _stMode_Info[eWindow].u16input_vfreq;

    // output timing
    bw_info.u16output_vfreq  = _stMode_Info[eWindow].u16ouput_vfreq;

    // memory format
    bw_info.bFBL = _stMode_Info[eWindow].bFBL;
    bw_info.bMemFmt422 = bMemFmt422;
    bw_info.bInterlace = _stMode_Info[eWindow].bInterlace;
    bw_info.u8BitsPerPixel = *u8BitsPerPixel;

    // osd
    if (!bw_info.bMemFmt422)    // for 444 memory format; 10-bit/8-bit cannot switch dynamically
        bw_info.bOSD_On = TRUE; // we should reserve OSD bandwidth
    else
        bw_info.bOSD_On = _bOSD_On;

    bw_info.u8OSD_BitsPerPixel = 16;

    // dram
    bw_info.bSC_MIUSel = 0;
    bw_info.bGOP_MIUSel = 0;


#if !PQ_GRULE_OSD_BW_ENABLE
    _MDrv_PQ_Calc_BW(&bw_info);
#endif

    _MDrv_PQ_Calc_BufSize(eWindow, &bw_info);

    if (bMemFmt422)
    {
        // decide bits/pixel and deinterlace mode flag
        if(bw_info.bInterlace)
        {
            if ( ((u16TabIdx_MADi == Hal_PQ_get_madi_idx(MS_MADI_25_4R)) && (!bw_info.b4RMode)) || ((u16TabIdx_MADi ==  Hal_PQ_get_madi_idx(MS_MADI_25_4R_MC)) && (!bw_info.b4RMode)))
            {
                u16TabIdx_MADi = Hal_PQ_get_madi_idx(MS_MADI_25_2R);
                PQBW_DBG(printf("[I:ACTION]switch to 25_2R\n"));
            }
            else if ((u16TabIdx_MADi == Hal_PQ_get_madi_idx(MS_MADI_27_4R)) && (!bw_info.b4RMode))
            {
                u16TabIdx_MADi = Hal_PQ_get_madi_idx(MS_MADI_27_2R);
                PQBW_DBG(printf("[I:ACTION]switch to 27_2R\n"));
            }
            else if ((u16TabIdx_MADi == Hal_PQ_get_madi_idx(MS_MADI_24_4R)) && (!bw_info.b4RMode))
            {
                u16TabIdx_MADi = Hal_PQ_get_madi_idx(MS_MADI_24_2R);
                PQBW_DBG(printf("[I:ACTION]switch to 24_2R\n"));
            }
            else if ((u16TabIdx_MADi == Hal_PQ_get_madi_idx(MS_MADI_26_4R)) && (!bw_info.b4RMode))
            {
                u16TabIdx_MADi = Hal_PQ_get_madi_idx(MS_MADI_26_2R);
                PQBW_DBG(printf("[I:ACTION]switch to 26_2R\n"));
            }
            else
            {
                PQBW_DBG(printf("[I:ACTION] no switch\n"));
            }
        }
        else
        {
            // progressive mode
            if (*u8BitsPerPixel != bw_info.u8BitsPerPixel)
            {
                switch(bw_info.u8BitsPerPixel)
                {
                case 24:
                    if(Hal_PQ_get_madi_idx(MS_MADI_P_MODE_MOT10) != 0xFFFF)
                    {
                        u16TabIdx_MADi = Hal_PQ_get_madi_idx(MS_MADI_P_MODE_MOT10);
                        PQBW_DBG(printf("[P24:ACTION]switch to P_MODE_MOT10\n"));
                    }
                    else
                    {
                        PQBW_DBG(printf("[P24:ACTION]switch failed\n"));
                        MS_ASSERT(0);
                    }
                    break;
                case 20:
                    if(Hal_PQ_get_madi_idx(MS_MADI_P_MODE10) != 0xFFFF)
                    {
                        u16TabIdx_MADi = Hal_PQ_get_madi_idx(MS_MADI_P_MODE10);
                        PQBW_DBG(printf("[P20:ACTION]switch to P_MODE10\n"));
                    }
                    else if (Hal_PQ_get_madi_idx(MS_MADI_P_MODE_MOT8) != 0xFFFF)
                    {
                        u16TabIdx_MADi = Hal_PQ_get_madi_idx(MS_MADI_P_MODE_MOT8);
                        PQBW_DBG(printf("[ACTION]switch to P_MODE_MOT8\n"));
                    }
                    else if (Hal_PQ_get_madi_idx(MS_MADI_24_4R_P) != 0xFFFF)
                    {
                        u16TabIdx_MADi = Hal_PQ_get_madi_idx(MS_MADI_24_4R_P);
                        PQBW_DBG(printf("[ACTION]switch to MS_MADI_24_4R_P\n"));
                    }
                    else if (Hal_PQ_get_madi_idx(MS_MADI_24_2R_P) != 0xFFFF)
                    {
                        u16TabIdx_MADi = Hal_PQ_get_madi_idx(MS_MADI_24_2R_P);
                        PQBW_DBG(printf("[ACTION]switch to MS_MADI_24_2R_P\n"));
                    }
                    else
                    {
                        printf("[PQ Madi]: Can not reduce bandwidth by madi\n");
                        MS_ASSERT(0);
                    }
                    break;
                default:
                case 16:
                    if(Hal_PQ_get_madi_idx(MS_MADI_P_MODE8) != 0xFFFF)
                    {
                        u16TabIdx_MADi = Hal_PQ_get_madi_idx(MS_MADI_P_MODE8);
                        PQBW_DBG(printf("[ACTION]switch to P_MODE8\n"));
                    }
                    else
                    {
                        printf("[PQ Madi]: Can not reduce bandwidth by madi\n");
                        MS_ASSERT(0);
                    }
                    break;
                }
            }
        }
    }
    else
    {
        if (*u8BitsPerPixel != bw_info.u8BitsPerPixel)
        {
            switch(bw_info.u8BitsPerPixel)
            {
            case 24:
                u16TabIdx_MemFormat = PQ_IP_MemFormat_444_8BIT_Main;
                break;
            default:
                MS_ASSERT(0);
                u16TabIdx_MemFormat = PQ_IP_MemFormat_444_8BIT_Main;
                break;
            }
            *u8BitsPerPixel = bw_info.u8BitsPerPixel;
        }
    }

    if (!bFBL)
    {
        if(_bPatchFastPlayback)
        {
            if(_bOneFieldMode)
            {
                SET_FORCE_YC_FULL_MOTION(bMainWin);
            }
            else
            {
                SET_FORCE_YC_MOTION_ENABLE(bMainWin);    // conflict with MADi table
            }
        }

        if (!bw_info.bInterlace)
        {
            if (bw_info.bMemFmt422)
            {
                PQBW_DBG(printf("[**] u8BitsPerPixel:%u, bw_info.u8BitsPerPixel:%u\n",
                    *u8BitsPerPixel, bw_info.u8BitsPerPixel));


                // for dynamically memory format changing, we need to keep it as 24 bits
                if(SUPPORT_DYNAMIC_MEM_FMT)
                {
                    *u8BitsPerPixel = 24;
                }

                switch(bw_info.u8BitsPerPixel)
                {
                case 24:
                    u8memory_data_fmt = MEMFMT24BIT_YC10BIT_MOT;
                    PQBW_DBG(printf("[ACTION]P MODE 10BIT_MOT\n"));
                    break;
                case 20:
                    if(u16TabIdx_MADi == Hal_PQ_get_madi_idx(MS_MADI_P_MODE_MOT8))
                    {
                        u8memory_data_fmt = MEMFMT24BIT_YC8BIT_MOT;
                        PQBW_DBG(printf("[ACTION]P MODE 8BIT_MOT\n"));
                    }
                    else if(u16TabIdx_MADi == Hal_PQ_get_madi_idx(MS_MADI_P_MODE10))
                    {
                        u8memory_data_fmt = MEMFMT24BIT_YC10BIT;
                        PQBW_DBG(printf("[ACTION]P MODE 10BIT\n"));
                    }
                    else if(u16TabIdx_MADi == Hal_PQ_get_madi_idx(MS_MADI_24_4R_P))
                    {
                        u8memory_data_fmt = MEMFMT24BIT_YC8BIT_MOT;
                        PQBW_DBG(printf("[ACTION]P MODE 8BIT_MOT\n"));
                    }
                    else if(u16TabIdx_MADi == Hal_PQ_get_madi_idx(MS_MADI_24_2R_P))
                    {
                        u8memory_data_fmt = MEMFMT24BIT_YC8BIT_MOT;
                        PQBW_DBG(printf("[ACTION]P MODE 8BIT_MOT\n"));
                    }
                    else
                    {
                        PQBW_DBG(printf("[ACTION]CLEAR USR MEMFMT\n"));
                    }

                    break;
                case 16:
                    u8memory_data_fmt = MEMFMT24BIT_YC8BIT;
                    //u8memory_data_fmt = MEMFMT24BIT_CLEAR; //2 bytes/pixel
                    PQBW_DBG(printf("[ACTION]P MODE 8BIT\n"));
                    break;
                default:
                    MS_ASSERT(0);
                    break;
                }
            }
            else    // 444
            {
                PQBW_DBG(printf("[ACTION]CLEAR USR MEMFMT\n"));
            }
        }
        else // interlace
        {
            PQBW_DBG(printf("[ACTION]CLEAR USR MEMFMT\n"));
        }
    }
    else
    {
        PQBW_DBG(printf("[ACTION]CLEAR USR MEMFMT\n"));
    }

    PQTAB_DBG(printf("u16TabIdx_MemFormat=%u\n", u16TabIdx_MemFormat));
    MDrv_PQ_LoadTable(eWindow, u16TabIdx_MemFormat, PQ_IP_MemFormat_Main);

    PQBW_DBG(printf("u16TabIdx_MADi=%u\n", u16TabIdx_MADi));

    /* ToDo
    if(MApi_SC_IsForceFreerun())
    {
        //Wait vsync for OPW disable->enable cause green garbage
        _MDrv_PQ_wait_output_vsync(PQ_MAIN_WINDOW, 1, 50);
    }
    */
    MDrv_PQ_LoadTable(eWindow, u16TabIdx_MADi, PQ_IP_MADi_Main);
#if (PQ_XRULE_DB_ENABLE == 1)
    MDrv_PQ_Set_DBRule (eWindow, u16TabIdx_MADi);
#endif

    PQTAB_DBG(printf("u16TabIdx_444To422=%u\n", u16TabIdx_444To422));
    MDrv_PQ_LoadTable(eWindow, u16TabIdx_444To422, PQ_IP_444To422_Main);

    PQTAB_DBG(printf("u16TabIdx_422To444=%u\n", u16TabIdx_422To444));
    MDrv_PQ_LoadTable(eWindow, u16TabIdx_422To444, PQ_IP_422To444_Main);

    /* ToDo
    if(MApi_SC_IsForceFreerun())
    {
        //Disable MCNR by disable OPW request when not do frame lock according to HW designer's suggestion.
        _MDrv_PQ_wait_output_vsync(PQ_MAIN_WINDOW, 1, 50);
        Hal_PQ_OPWriteOff_Enable(TRUE);
    }
    */

    if(eWindow == PQ_MAIN_WINDOW)
    {
        //printf("read Mem FMT =%x\n", (MApi_XC_R2BYTE(REG_SC_BK12_02_L) & 0x00FF));
        //printf("Main Window Mem FMT =%x\n",(MS_U16)u8memory_data_fmt);
        if ( (MApi_XC_R2BYTE(REG_SC_BK12_02_L) & 0x00FF) != (MS_U16)u8memory_data_fmt)
        {
            if (u8memory_data_fmt == MEMFMT24BIT_CLEAR)
            {
                //printf("Is MEMFMT24BIT_CLEAR\n");
                //_MDrv_PQ_wait_input_vsync(eWindow, 1, 90);
                Hal_PQ_set_mem_fmt_en(bMainWin, 0x0000, 0x0100);
                //_MDrv_PQ_wait_output_vsync(eWindow, 1, 90);
                Hal_PQ_set_mem_fmt_en(bMainWin, 0x0000, 0x0200);
                Hal_PQ_set_mem_fmt(bMainWin, 0x0000, 0x00FF);
            }
            else
            {
                //printf("Set Memory FMT\n");
                //_MDrv_PQ_wait_output_vsync(eWindow, 1, 90);
                Hal_PQ_set_mem_fmt(bMainWin, (MS_U16)u8memory_data_fmt, 0x00F0);
                Hal_PQ_set_mem_fmt_en(bMainWin, 0x0200, 0x0200);
                //_MDrv_PQ_wait_input_vsync(eWindow, 1, 90);
                Hal_PQ_set_mem_fmt(bMainWin, (MS_U16)u8memory_data_fmt, 0x000F);
                Hal_PQ_set_mem_fmt_en(bMainWin, 0x0100, 0x100);
                //_MDrv_PQ_wait_output_vsync(eWindow, 1, 90);
            }
        }
    }
    else
    {
        //printf("Main WinDow MADI mode =%x\n", (MApi_XC_R2BYTE(REG_SC_BK12_42_L) & 0x00FF));
        if ( (MApi_XC_R2BYTE(REG_SC_BK12_42_L) & 0x00FF) != (MS_U16)u8memory_data_fmt)
        {
            if (u8memory_data_fmt == MEMFMT24BIT_CLEAR)
            {
                //printf("Is MEMFMT24BIT_CLEAR\n");
                //_MDrv_PQ_wait_input_vsync(eWindow, 1, 90);
                 Hal_PQ_set_mem_fmt_en(bMainWin, 0x0000, 0x0100);
                //_MDrv_PQ_wait_output_vsync(eWindow, 1, 90);
                Hal_PQ_set_mem_fmt_en(bMainWin, 0x0000, 0x0200);
                Hal_PQ_set_mem_fmt(bMainWin, 0x0000, 0x00FF);
            }
            else
            {
                //printf("Set Memory FMT\n");
                //_MDrv_PQ_wait_output_vsync(eWindow, 1, 90);
                Hal_PQ_set_mem_fmt(bMainWin, (MS_U16)u8memory_data_fmt, 0x00F0);
                Hal_PQ_set_mem_fmt_en(bMainWin, 0x0200, 0x0200);
                //_MDrv_PQ_wait_input_vsync(eWindow, 1, 90);
                Hal_PQ_set_mem_fmt(bMainWin, (MS_U16)u8memory_data_fmt, 0x000F);
                Hal_PQ_set_mem_fmt_en(bMainWin, 0x0100, 0x100);
                //_MDrv_PQ_wait_output_vsync(eWindow, 1, 90);
            }
        }
    }

    MDrv_PQ_SuggestFrameNum(eWindow, u16TabIdx_MADi, u8FrameCount);	
    return eDeInterlaceMode;
}

void MDrv_PQ_ReduceBW_ForOSD(PQ_WIN eWindow, MS_BOOL bOSD_On)
{
    MS_U8 u8BitPerPixel = 0;

    if((_stMode_Info[eWindow].u16input_hsize>=1280)&&(_stMode_Info[eWindow].u16input_vsize>=720))
    {
            PQBW_DBG(printf("[PQ_ReduceBW_ForOSD]:%u\n", bOSD_On));

        _bOSD_On = bOSD_On;

        MDrv_PQ_SetMemFormat(
            eWindow,
            _gbMemfmt422[eWindow],
            _stMode_Info[eWindow].bFBL,
            &u8BitPerPixel);
    }
    return;
}

void MDrv_PQ_ReduceBW_ForPVR(PQ_WIN eWindow, MS_BOOL bPVR_On)
{
    MS_U8 u8BitPerPixel = 0;

    PQBW_DBG(printf("[PQ_ReduceBW_ForPVR]:%u\n", bPVR_On));

    if (bPVR_On)
        bw_info.u32PVR_BW = 450*1000000;
    else
        bw_info.u32PVR_BW = 0;

    MDrv_PQ_SetMemFormat(
        eWindow,
        _gbMemfmt422[eWindow],
        _stMode_Info[eWindow].bFBL,
        &u8BitPerPixel);
}

void _MDrv_PQ_Set_MVOP_UVShift(MS_BOOL bEnable)
{
    MS_U8 regval;

    regval = MApi_XC_ReadByte(VOP_MPG_JPG_SWITCH);

    if (((regval & 0x10) == 0x10) && ((regval & 0x3)== 0x2))
    {
        // 422 with MCU control mode
        if (bEnable)
        {
            MS_ASSERT(0);
        }
    }

    // output 420 and interlace
    //[IP - Sheet] : Main Page --- 420CUP
    //[Project] :  Titania2
    //[Description]:   Chroma artifacts when 420to422 is applied duplicate method.
    //[Root cause]: Apply 420to422 average algorithm to all DTV input cases.
    //The average algorithm must cooperate with MVOP.
    MApi_XC_WriteByteMask(VOP_UV_SHIFT, (bEnable)?1:0, 0x3);
}

void MDrv_PQ_Set420upsampling(PQ_WIN eWindow,
                              MS_BOOL bFBL,
                              MS_BOOL bPreV_ScalingDown,
                              MS_U16 u16V_CropStart)
{
    MS_U16 u16TabIdx;
    MS_U8 u8Bank;

    u16TabIdx = MDrv_PQ_GetTableIndex(eWindow, PQ_IP_420CUP_Main);

    PQTAB_DBG(printf("[PQ_Set420upsampling]: SrcType:%u, FBL:%u, PreV down:%u, V_CropStart:%u, u8TabIdx=%u, ",
        _u16PQSrcType[eWindow], bFBL, bPreV_ScalingDown, u16V_CropStart, u16TabIdx));

    if ((u16TabIdx == Hal_PQ_get_420_cup_idx(MS_420_CUP_ON)) && (!bPreV_ScalingDown) && (!bFBL) )
    {
        PQTAB_DBG(printf("UVShift: on\n"));
        _MDrv_PQ_Set_MVOP_UVShift(ENABLE);
        MDrv_PQ_LoadTable(eWindow, Hal_PQ_get_420_cup_idx(MS_420_CUP_ON), PQ_IP_420CUP_Main);
    }
    else    // P mode should not do UV shift
    {
        PQTAB_DBG(printf("UVShift: off\n"));
        _MDrv_PQ_Set_MVOP_UVShift(DISABLE);
        MDrv_PQ_LoadTable(eWindow, Hal_PQ_get_420_cup_idx(MS_420_CUP_OFF), PQ_IP_420CUP_Main);
    }

    u8Bank = MApi_XC_ReadByte(BK_SELECT_00);

    if ((u16V_CropStart & 0x3) == 0)      // crop lines are multiple of 4
        Hal_PQ_set_420upSample(0x6666);
    else if ((u16V_CropStart & 0x1) == 0) // crop lines are multiple of 2
        Hal_PQ_set_420upSample(0x9999);
    else
        MS_ASSERT(0);

    MApi_XC_WriteByte(BK_SELECT_00, u8Bank);
}

void MDrv_PQ_SetFilmMode(PQ_WIN eWindow, MS_BOOL bEnable)
{
    MS_U16 u16TabIdx;

    PQTAB_DBG(printf("[PQ_SetFilmMode]: PQTabType=%u, enable=%u\n", eWindow, bEnable));

    if (bEnable)
        u16TabIdx = MDrv_PQ_GetTableIndex(eWindow, PQ_IP_Film32_Main);
    else
        u16TabIdx = PQ_IP_Film32_OFF_Main;

    MDrv_PQ_LoadTable(eWindow, u16TabIdx, PQ_IP_Film32_Main);

    if (bEnable)
        u16TabIdx = MDrv_PQ_GetTableIndex(eWindow, PQ_IP_Film22_Main);
    else
        u16TabIdx = PQ_IP_Film22_OFF_Main;

    MDrv_PQ_LoadTable(eWindow, u16TabIdx, PQ_IP_Film22_Main);
}


void MDrv_PQ_SetNonLinearScaling(PQ_WIN eWindow, MS_U8 u8Level, MS_BOOL bEnable)
{
    MS_U16 u16TabIdx;
    PQTAB_DBG(printf("[PQ_SetNonLinearScaling]: Level=%u, enable=%u\n", u8Level, bEnable));

    if (bEnable)
    {
        switch(_stPQ_Info.u16PnlWidth)
        {
        case 1920:
            switch(u8Level)
            {
            case 2:
                u16TabIdx = Hal_PQ_get_hnonlinear_idx(MS_HNONLINEAR_1920_2);
                break;
            case 1:
                u16TabIdx = Hal_PQ_get_hnonlinear_idx(MS_HNONLINEAR_1920_1);
                break;
            case 0:
            default:
                 u16TabIdx = Hal_PQ_get_hnonlinear_idx(MS_HNONLINEAR_1920_0);
                 break;
            }
            break;
        case 1680:
            u16TabIdx = Hal_PQ_get_hnonlinear_idx(MS_HNONLINEAR_1680);
            break;
        case 1440:
            u16TabIdx = Hal_PQ_get_hnonlinear_idx(MS_HNONLINEAR_1440);
            break;
        case 1366:
            switch(u8Level)
            {
            case 2:
                u16TabIdx = Hal_PQ_get_hnonlinear_idx(MS_HNONLINEAR_1366_2);
                break;
            case 1:
                u16TabIdx = Hal_PQ_get_hnonlinear_idx(MS_HNONLINEAR_1366_1);
                break;
            case 0:
            default:
                u16TabIdx = Hal_PQ_get_hnonlinear_idx(MS_HNONLINEAR_1366_0);
                break;

            }
            break;
        default:
            u16TabIdx = Hal_PQ_get_hnonlinear_idx(MS_HNONLINEAR_OFF);
            break;

            break;
        }
    }
    else
    {
        u16TabIdx = Hal_PQ_get_hnonlinear_idx(MS_HNONLINEAR_OFF);
    }

    MDrv_PQ_LoadTable(eWindow, u16TabIdx, PQ_IP_HnonLinear_Main);
}

void MDrv_PQ_PatchFastPlayback(PQ_WIN eWindow, MS_BOOL bOn, MS_BOOL bOneField)
{
    static MS_BOOL bOnOld = FALSE;
    MS_U8 u8Bank;
    MS_BOOL bMainWin = (eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE;
    u8Bank = MApi_XC_ReadByte(BK_SELECT_00);

    //printf("MDrv_PQ_PatchFastPlayback: %u\n", (MS_U16)bOn);
    _bOneFieldMode = bOneField;

    if(bOn)
    {
        if(!bOnOld)
        {
            bOnOld = TRUE;
            _bPatchFastPlayback = TRUE;
            if (QM_IsInterlaced(eWindow))
            {
                SAVE_FORCE_YC_MOTION_ENABLE(bMainWin);
                SAVE_MADI_LPF(bMainWin);
                //SAVE_VSP_YROM121();
            }
            SAVE_NR_DISABLE(bMainWin);
            SAVE_PRESNR_DISABLE(bMainWin);
        }
    }
    else
    {
        if (bOnOld)
        {
            bOnOld = FALSE;
            _bPatchFastPlayback = FALSE;

            if (QM_IsInterlaced(eWindow))
            {
                RESTORE_FORCE_YC_MOTION_ENABLE(bMainWin);
                RESTORE_MADI_LPF(bMainWin);
                //RESTORE_VSP_YROM121();
            }
            RESTORE_NR_DISABLE(bMainWin);
            RESTORE_PRESNR_DISABLE(bMainWin);
        }
    }

    if (_bPatchFastPlayback)
    {
        //printf("Enable patch!\n");
        if (QM_IsInterlaced(eWindow))
        {
            if(bOneField)
            {
                SET_FORCE_YC_FULL_MOTION(bMainWin);
            }
            else
            {
                SET_FORCE_YC_MOTION_ENABLE(bMainWin);
                SET_MADI_LPF(bMainWin);
                //SET_VSP_YROM121();  // not use after T2
            }
        }
        SET_NR_DISABLE(bMainWin);      // for save bandwidth
        SET_PRESNR_DISABLE(bMainWin);  // for save bandwidth
    }

    MApi_XC_WriteByte(BK_SELECT_00, u8Bank);
}


void MDrv_PQ_DisableFilmMode(PQ_WIN eWindow, MS_BOOL bOn)
{
    if (bOn)
    {
        MDrv_PQ_SetFilmMode(eWindow, DISABLE);
    }
    else
    {
        MDrv_PQ_SetFilmMode(eWindow, ENABLE);
    }
}


E_DRVPQ_ReturnValue MDrv_PQ_GetLibVer(const MSIF_Version **ppVersion)
{
    if(ppVersion == NULL)
    {
        return E_DRVPQ_RET_FAIL;
    }

    *ppVersion = &_drv_pq_version;
    return E_DRVPQ_RET_OK;
}


PQ_ReturnValue MDrv_PQ_GetInfo(MS_PQ_INFO *pInfo)
{
    memcpy((void*)pInfo, (void*)&_info, sizeof(MS_PQ_INFO));
    return E_PQ_RET_OK;
}

MS_BOOL  MDrv_PQ_GetStatus(MS_PQ_Status *pStatus)
{
    memcpy((void*)pStatus, (void*)&_status, sizeof(MS_PQ_Status));
    return FALSE;
}

MS_BOOL  MDrv_PQ_SetDbgLevel(MS_U16 u16DbgSwitch)
{
    _u16PQDbgSwitch = u16DbgSwitch;
    return TRUE;
}

void MDrv_PQ_Set_DisplayType(MS_U16 u16DisplayWidth, PQ_DISPLAY_TYPE enDisplaType)
{
#if (PQ_ENABLE_PIP)
    if(gbPQBinEnable)
    {
    #if(ENABLE_PQ_BIN)
        MDrv_PQBin_SetPanelID(u16DisplayWidth == 1366 ? 1 : 0);
        MDrv_PQBin_SetDisplayType(enDisplaType == PQ_DISPLAY_ONE ? PQ_BIN_DISPLAY_ONE :
                                  enDisplaType == PQ_DISPLAY_PIP ? PQ_BIN_DISPLAY_PIP :
                                  PQ_BIN_DISPLAY_POP);
    #endif
    }
    else
    {
#if (PQ_ONLY_SUPPORT_BIN == 0)

        PQTABLE_INFO PQTableInfo;
        PQTABLE_INFO PQTableInfoEx;

        memset(&PQTableInfo, 0, sizeof(PQTABLE_INFO));
        memset(&PQTableInfoEx, 0, sizeof(PQTABLE_INFO));

        if(genDisplayType != enDisplaType)
        {
            switch(enDisplaType)
            {
            default:
            case PQ_DISPLAY_ONE:
                if(u16DisplayWidth == 1366)
                {
                    PQTableInfo.pQuality_Map_Aray = (void*)QMAP_1366_Main;
                    PQTableInfo.pGRule_Level[E_GRULE_NR] = (void*)MST_GRule_1366_NR_Main;
                }
                else
                {
                    PQTableInfo.pQuality_Map_Aray = (void*)QMAP_1920_Main;
                    PQTableInfo.pGRule_Level[E_GRULE_NR] = (void*)MST_GRule_1920_NR_Main;
                }
                break;

            case PQ_DISPLAY_PIP:
                if(u16DisplayWidth == 1366)
                {
                    PQTableInfo.pQuality_Map_Aray = (void*)QMAP_1366_PIP_Main;
                    PQTableInfo.pGRule_Level[E_GRULE_NR] = (void*)MST_GRule_1366_PIP_NR_Main;
                }
                else
                {
                    PQTableInfo.pQuality_Map_Aray = (void*)QMAP_1920_PIP_Main;
                    PQTableInfo.pGRule_Level[E_GRULE_NR] = (void*)MST_GRule_1920_PIP_NR_Main;
                }
                break;

            case PQ_DISPLAY_POP:
                if(u16DisplayWidth == 1366)
                {
                    PQTableInfo.pQuality_Map_Aray = (void*)QMAP_1366_POP_Main;
                    PQTableInfo.pGRule_Level[E_GRULE_NR] = (void*)MST_GRule_1366_POP_NR_Main;
                }
                else
                {
                    PQTableInfo.pQuality_Map_Aray = (void*)QMAP_1920_POP_Main;
                    PQTableInfo.pGRule_Level[E_GRULE_NR] = (void*)MST_GRule_1920_POP_NR_Main;
                }
                break;
            }

            // table config parameter
            PQTableInfo.u8PQ_InputType_Num = QM_INPUTTYPE_NUM_Main;
            PQTableInfo.u8PQ_IP_Num = PQ_IP_NUM_Main;
            PQTableInfo.pIPTAB_Info = (void*)PQ_IPTAB_INFO_Main;
            PQTableInfo.pSkipRuleIP = (void*)MST_SkipRule_IP_Main;

            PQTableInfo.u8PQ_XRule_IP_Num[E_XRULE_HSD] = PQ_HSDRule_IP_NUM_Main;
            PQTableInfo.u8PQ_XRule_IP_Num[E_XRULE_VSD] = PQ_VSDRule_IP_NUM_Main;
            PQTableInfo.u8PQ_XRule_IP_Num[E_XRULE_HSP] = PQ_HSPRule_IP_NUM_Main;
            PQTableInfo.u8PQ_XRule_IP_Num[E_XRULE_VSP] = PQ_VSPRule_IP_NUM_Main;
            PQTableInfo.u8PQ_XRule_IP_Num[E_XRULE_CSC] = PQ_CSCRule_IP_NUM_Main;
            PQTableInfo.pXRule_IP_Index[E_XRULE_HSD] = (void*)MST_HSDRule_IP_Index_Main;
            PQTableInfo.pXRule_IP_Index[E_XRULE_VSD] = (void*)MST_VSDRule_IP_Index_Main;
            PQTableInfo.pXRule_IP_Index[E_XRULE_HSP] = (void*)MST_HSPRule_IP_Index_Main;
            PQTableInfo.pXRule_IP_Index[E_XRULE_VSP] = (void*)MST_VSPRule_IP_Index_Main;
            PQTableInfo.pXRule_IP_Index[E_XRULE_CSC] = (void*)MST_CSCRule_IP_Index_Main;
            PQTableInfo.pXRule_Array[E_XRULE_HSD] = (void*)MST_HSDRule_Array_Main;
            PQTableInfo.pXRule_Array[E_XRULE_VSD] = (void*)MST_VSDRule_Array_Main;
            PQTableInfo.pXRule_Array[E_XRULE_HSP] = (void*)MST_HSPRule_Array_Main;
            PQTableInfo.pXRule_Array[E_XRULE_VSP] = (void*)MST_VSPRule_Array_Main;
            PQTableInfo.pXRule_Array[E_XRULE_CSC] = (void*)MST_CSCRule_Array_Main;

            PQTableInfo.u8PQ_GRule_Num[E_GRULE_NR] = PQ_GRULE_NR_NUM_Main;
            PQTableInfo.u8PQ_GRule_IPNum[E_GRULE_NR] = PQ_GRULE_NR_IP_NUM_Main;
            PQTableInfo.pGRule_IP_Index[E_GRULE_NR] = (void*)MST_GRule_NR_IP_Index_Main;
            PQTableInfo.pGRule_Array[E_GRULE_NR] = (void*)MST_GRule_NR_Main;

            MDrv_PQ_AddTable_(MAIN, &PQTableInfo);
            //MDrv_PQ_LoadCommTable_(MAIN);


            // table config parameter
            PQTableInfoEx = PQTableInfo;

            PQTableInfoEx.u8PQ_InputType_Num = QM_INPUTTYPE_NUM_Main_Ex;
            PQTableInfoEx.u8PQ_IP_Num = PQ_IP_NUM_Main_Ex;
            PQTableInfoEx.pIPTAB_Info = (void*)PQ_IPTAB_INFO_Main_Ex;
            PQTableInfoEx.pSkipRuleIP = (void*)MST_SkipRule_IP_Main_Ex;

            switch(enDisplaType)
            {
            default:
            case PQ_DISPLAY_ONE:
                if(u16DisplayWidth == 1366)
                {
                    PQTableInfoEx.pQuality_Map_Aray = (void*)QMAP_1366_Main_Ex;
                }
                else
                {
                    PQTableInfoEx.pQuality_Map_Aray = (void*)QMAP_1920_Main_Ex;
                }
                break;

            case PQ_DISPLAY_PIP:
                if(u16DisplayWidth == 1366)
                {
                    PQTableInfoEx.pQuality_Map_Aray = (void*)QMAP_1366_PIP_Main_Ex;
                }
                else
                {
                    PQTableInfoEx.pQuality_Map_Aray = (void*)QMAP_1920_PIP_Main_Ex;
                }
                break;

            case PQ_DISPLAY_POP:
                if(u16DisplayWidth == 1366)
                {
                    PQTableInfoEx.pQuality_Map_Aray = (void*)QMAP_1366_POP_Main_Ex;
                }
                else
                {
                    PQTableInfoEx.pQuality_Map_Aray = (void*)QMAP_1920_POP_Main_Ex;
                }
                break;
            }

            MDrv_PQ_AddTable_(MAINEX, &PQTableInfoEx);
            //MDrv_PQ_LoadCommTable_(MAINEX);


            // Load Sub PQ
            switch(enDisplaType)
            {
            default:
            case PQ_DISPLAY_ONE:
                if(u16DisplayWidth == 1366)
                {
                    PQTableInfo.pQuality_Map_Aray = (void*)QMAP_1366_Sub;
                    PQTableInfo.pGRule_Level[E_GRULE_NR] = (void*)MST_GRule_1366_NR_Sub;
                }
                else
                {
                    PQTableInfo.pQuality_Map_Aray = (void*)QMAP_1920_Sub;
                    PQTableInfo.pGRule_Level[E_GRULE_NR] = (void*)MST_GRule_1920_NR_Sub;
                }
                break;

            case PQ_DISPLAY_PIP:
                if(u16DisplayWidth == 1366)
                {
                    PQTableInfo.pQuality_Map_Aray = (void*)QMAP_1366_PIP_Sub;
                    PQTableInfo.pGRule_Level[E_GRULE_NR] = (void*)MST_GRule_1366_PIP_NR_Sub;
                }
                else
                {
                    PQTableInfo.pQuality_Map_Aray = (void*)QMAP_1920_PIP_Sub;
                    PQTableInfo.pGRule_Level[E_GRULE_NR] = (void*)MST_GRule_1920_PIP_NR_Sub;
                }
                break;

            case PQ_DISPLAY_POP:
                if(u16DisplayWidth == 1366)
                {
                    PQTableInfo.pQuality_Map_Aray = (void*)QMAP_1366_POP_Sub;
                    PQTableInfo.pGRule_Level[E_GRULE_NR] = (void*)MST_GRule_1366_POP_NR_Sub;
                }
                else
                {
                    PQTableInfo.pQuality_Map_Aray = (void*)QMAP_1920_POP_Sub;
                    PQTableInfo.pGRule_Level[E_GRULE_NR] = (void*)MST_GRule_1920_POP_NR_Sub;
                }
                break;
            }

            // table config parameter
            PQTableInfo.u8PQ_InputType_Num = QM_INPUTTYPE_NUM_Sub;
            PQTableInfo.u8PQ_IP_Num = PQ_IP_NUM_Sub;
            PQTableInfo.pIPTAB_Info = (void*)PQ_IPTAB_INFO_Sub;
            PQTableInfo.pSkipRuleIP = (void*)MST_SkipRule_IP_Sub;

            PQTableInfo.u8PQ_XRule_IP_Num[E_XRULE_HSD] = PQ_HSDRule_IP_NUM_Sub;
            PQTableInfo.u8PQ_XRule_IP_Num[E_XRULE_VSD] = PQ_VSDRule_IP_NUM_Sub;
            PQTableInfo.u8PQ_XRule_IP_Num[E_XRULE_HSP] = PQ_HSPRule_IP_NUM_Sub;
            PQTableInfo.u8PQ_XRule_IP_Num[E_XRULE_VSP] = PQ_VSPRule_IP_NUM_Sub;
            PQTableInfo.u8PQ_XRule_IP_Num[E_XRULE_CSC] = PQ_CSCRule_IP_NUM_Sub;
            PQTableInfo.pXRule_IP_Index[E_XRULE_HSD] = (void*)MST_HSDRule_IP_Index_Sub;
            PQTableInfo.pXRule_IP_Index[E_XRULE_VSD] = (void*)MST_VSDRule_IP_Index_Sub;
            PQTableInfo.pXRule_IP_Index[E_XRULE_HSP] = (void*)MST_HSPRule_IP_Index_Sub;
            PQTableInfo.pXRule_IP_Index[E_XRULE_VSP] = (void*)MST_VSPRule_IP_Index_Sub;
            PQTableInfo.pXRule_IP_Index[E_XRULE_CSC] = (void*)MST_CSCRule_IP_Index_Sub;
            PQTableInfo.pXRule_Array[E_XRULE_HSD] = (void*)MST_HSDRule_Array_Sub;
            PQTableInfo.pXRule_Array[E_XRULE_VSD] = (void*)MST_VSDRule_Array_Sub;
            PQTableInfo.pXRule_Array[E_XRULE_HSP] = (void*)MST_HSPRule_Array_Sub;
            PQTableInfo.pXRule_Array[E_XRULE_VSP] = (void*)MST_VSPRule_Array_Sub;
            PQTableInfo.pXRule_Array[E_XRULE_CSC] = (void*)MST_CSCRule_Array_Sub;

            PQTableInfo.u8PQ_GRule_Num[E_GRULE_NR] = PQ_GRULE_NR_NUM_Sub;
            PQTableInfo.u8PQ_GRule_IPNum[E_GRULE_NR] = PQ_GRULE_NR_IP_NUM_Sub;
            PQTableInfo.pGRule_IP_Index[E_GRULE_NR] = (void*)MST_GRule_NR_IP_Index_Sub;
            PQTableInfo.pGRule_Array[E_GRULE_NR] = (void*)MST_GRule_NR_Sub;

            MDrv_PQ_AddTable_(SUB, &PQTableInfo);
            //MDrv_PQ_LoadCommTable_(SUB);


            // table config parameter
            PQTableInfoEx = PQTableInfo;

            PQTableInfoEx.u8PQ_InputType_Num = QM_INPUTTYPE_NUM_Sub_Ex;
            PQTableInfoEx.u8PQ_IP_Num = PQ_IP_NUM_Sub_Ex;
            PQTableInfoEx.pIPTAB_Info = (void*)PQ_IPTAB_INFO_Sub_Ex;
            PQTableInfoEx.pSkipRuleIP = (void*)MST_SkipRule_IP_Sub_Ex;

            switch(enDisplaType)
            {
            default:
            case PQ_DISPLAY_ONE:
                if(u16DisplayWidth == 1366)
                {
                    PQTableInfoEx.pQuality_Map_Aray = (void*)QMAP_1366_Sub_Ex;
                }
                else
                {
                    PQTableInfoEx.pQuality_Map_Aray = (void*)QMAP_1920_Sub_Ex;
                }
                break;

            case PQ_DISPLAY_PIP:
                if(u16DisplayWidth == 1366)
                {
                    PQTableInfoEx.pQuality_Map_Aray = (void*)QMAP_1366_PIP_Sub_Ex;
                }
                else
                {
                    PQTableInfoEx.pQuality_Map_Aray = (void*)QMAP_1920_PIP_Sub_Ex;
                }
                break;

            case PQ_DISPLAY_POP:
                if(u16DisplayWidth == 1366)
                {
                    PQTableInfoEx.pQuality_Map_Aray = (void*)QMAP_1366_POP_Sub_Ex;
                }
                else
                {
                    PQTableInfoEx.pQuality_Map_Aray = (void*)QMAP_1920_POP_Sub_Ex;
                }
                break;
            }
            MDrv_PQ_AddTable_(SUBEX, &PQTableInfoEx);
            //MDrv_PQ_LoadCommTable_(SUBEX);
        }

        genDisplayType = enDisplaType;
#endif // #if PQ_ONLY_SUPPORT_BIN

    }

#else
    u16DisplayWidth = u16DisplayWidth;
    enDisplaType = enDisplaType;
#endif
}

void MDrv_PQ_MADiForceMotionC(PQ_WIN eWindow, MS_BOOL bEnable)
{
   MS_U16 u16DataC =0;
    u16DataC = Hal_PQ_get_force_c_motion(eWindow == PQ_MAIN_WINDOW);

    if (eWindow == PQ_SUB_WINDOW)
    {
        u16DataC  <<= 8;
    }

    if (bEnable)
    {
        u16DataC |= (eWindow == PQ_MAIN_WINDOW ? 0x0080 : 0x8000);
    }
    else
    {
        u16DataC &= (eWindow == PQ_MAIN_WINDOW ? 0x000F : 0x0F00);
    }
    Hal_PQ_set_force_c_motion(eWindow == PQ_MAIN_WINDOW, u16DataC);

}

void MDrv_PQ_MADiForceMotionY(PQ_WIN eWindow, MS_BOOL bEnable)
{
    MS_U16 u16Data = 0;

    u16Data = Hal_PQ_get_force_y_motion(eWindow == PQ_MAIN_WINDOW);

    if (eWindow == PQ_SUB_WINDOW)
    {
        u16Data <<= 8;
    }
    if (bEnable)
    {
        u16Data |= (eWindow == PQ_MAIN_WINDOW ? 0x0080 : 0x8000);
    }
    else
    {
        u16Data &= (eWindow == PQ_MAIN_WINDOW ? 0x003F : 0x3F00);
    }

    Hal_PQ_set_force_y_motion(eWindow == PQ_MAIN_WINDOW, u16Data);
}

void MDrv_PQ_MADiForceMotion(PQ_WIN eWindow, MS_BOOL bEnableY, MS_U16 u16DataY,
                             MS_BOOL bEnableC, MS_U16 u16DataC)
{
    // Y
    if (bEnableY)
    {
        SAVE_FORCE_Y_MOTION_ENABLE(eWindow == PQ_MAIN_WINDOW);
        if (eWindow == PQ_SUB_WINDOW)
        {
            u16DataY <<= 8;
        }
        u16DataY |= (eWindow == PQ_MAIN_WINDOW ? 0x0080 : 0x8000);
        Hal_PQ_set_force_y_motion(eWindow == PQ_MAIN_WINDOW, u16DataY);
    }
    else
    {
        RESTORE_FORCE_Y_MOTION_ENABLE(eWindow == PQ_MAIN_WINDOW);
    }

    // C
    if (bEnableC)
    {
        SAVE_FORCE_C_MOTION_ENABLE(eWindow == PQ_MAIN_WINDOW);
        if (eWindow == PQ_SUB_WINDOW)
        {
            u16DataC <<= 8;
        }
        u16DataC |= (eWindow == PQ_MAIN_WINDOW ? 0x0080 : 0x8000);
        Hal_PQ_set_force_c_motion(eWindow == PQ_MAIN_WINDOW, u16DataC);
    }
    else
    {
        RESTORE_FORCE_C_MOTION_ENABLE(eWindow == PQ_MAIN_WINDOW);
    }

}

#if 0
static void _MDrv_PQ_Set_MADiMode(MS_BOOL bEnable, MS_MADI_TYPE enType)
{
    MS_U16 u16TabIdx = 0;

#if(ENABLE_PQ_MLOAD)
    MDrv_PQ_Set_MLoadEn(PQ_MAIN_WINDOW, ENABLE);
#endif

    if(bEnable) //Set target MADi mode
    {
        u16TabIdx = Hal_PQ_get_madi_idx(enType);
        MDrv_PQ_LoadTable(PQ_MAIN_WINDOW, u16TabIdx, PQ_IP_MADi_Main);
    }
    else    //Set default QMap MADi mode
    {
        u16TabIdx = MDrv_PQ_GetTableIndex(PQ_MAIN_WINDOW, PQ_IP_MADi_Main);
        MDrv_PQ_LoadTable(PQ_MAIN_WINDOW, u16TabIdx, PQ_IP_MADi_Main);
    }

#if(ENABLE_PQ_MLOAD)
    MDrv_PQ_Set_MLoadEn(PQ_MAIN_WINDOW, DISABLE);
#endif
}
#endif

void MDrv_PQ_SetRFblMode(MS_BOOL bEnable,MS_BOOL bFilm)
{
    _stRFBL_Info.bEnable = bEnable;
    _stRFBL_Info.bFilm = bFilm;

    if(!bEnable)    //default mode
    {
        // No need to control Y motion when setting RFBL mode
        //MDrv_PQ_MADiForceMotionY(PQ_MAIN_WINDOW, FALSE);
        MDrv_PQ_MADiForceMotionC(PQ_MAIN_WINDOW, FALSE);

        _stRFBL_Info.u8MADiType = MS_MADI_24_2R;
        //printf("[%s]-[%d] : Y/C motion : ( 0 , 0 ) \n", __FUNCTION__, __LINE__);
    }
    else
    {
        _stRFBL_Info.u8MADiType = (bFilm) ? (MS_U8)MS_MADI_24_RFBL_FILM : (MS_U8)MS_MADI_24_RFBL_NFILM;

        // No need to control Y motion when setting RFBL mode
        //MDrv_PQ_MADiForceMotionY(PQ_MAIN_WINDOW, FALSE);
        MDrv_PQ_MADiForceMotionC(PQ_MAIN_WINDOW, (!bFilm));

        //printf("[%s]-[%d] : Y/C motion : ( 0 , %d) \n", __FUNCTION__, __LINE__,(!bFilm));
    }

    //_MDrv_PQ_Set_MADiMode(bEnable, (MS_MADI_TYPE)_stRFBL_Info.u8MADiType);
}

void MDrv_PQ_GetRFblMode(MS_PQ_RFBL_INFO * pstInfo)
{
    memcpy(pstInfo, &_stRFBL_Info, sizeof(MS_PQ_RFBL_INFO));
    return;
}

#if (PQ_ENABLE_VD_SAMPLING)

void MDrv_PQ_Get_VDSampling_Info(PQ_WIN eWindow, PQ_INPUT_SOURCE_TYPE enInputSourceType, PQ_VIDEOSTANDARD_TYPE eStandard, MS_PQ_VD_Sampling_Info *pInfo)
{
    PQ_VIDEOSTANDARD_TYPE eCurStandard = _stVD_Info[eWindow].enVideoStandard;
    MS_U16 u16CurSrc = _u16PQSrcType[eWindow];
    MS_U16 u16TableIdx;
    MS_U16 u16IPIdx = Hal_PQ_get_ip_idx(MS_PQ_IP_VD_SAMPLING);
    MS_U8 u8SampleTable[10];

    memset(u8SampleTable, 0, sizeof(u8SampleTable));

    PQTAB_DBG(printf("[PQ] GetVDSampling: win:%d, src:%d, standard:%d, PQIP=%x\n", eWindow, enInputSourceType, eStandard, u16IPIdx));
    if((enInputSourceType >= PQ_INPUT_SOURCE_TV && enInputSourceType <= PQ_INPUT_SOURCE_SVIDEO) &&
       (eStandard < E_PQ_VIDEOSTANDARD_NOTSTANDARD) &&
       u16IPIdx != 0xFFFF)
    {
        _stVD_Info[eWindow].enVideoStandard = eStandard;
        _u16PQSrcType[eWindow] = QM_InputSourceToIndex(eWindow, enInputSourceType);



        u16TableIdx = MDrv_PQ_GetTableIndex(eWindow, u16IPIdx);
        PQTAB_DBG(printf("[PQ] VDSampling: TableIdx=%d\n", u16TableIdx));
    #if(ENABLE_PQ_BIN)
        if ( (gbPQBinEnable == 1 && u16TableIdx == PQ_BIN_IP_NULL) ||
             (gbPQBinEnable == 0 && u16TableIdx == PQ_IP_NULL))
    #else
        if( u16TableIdx == PQ_IP_NULL )
    #endif
        {
            pInfo->eType = PQ_VD_SAMPLING_OFF;
        }
        else
        {
            MDrv_PQ_LoadTableData(eWindow, u16TableIdx, u16IPIdx, u8SampleTable, 10);
            pInfo->u16Vstart = ((MS_U16)u8SampleTable[0]) | (((MS_U16)u8SampleTable[1])<<8);
            pInfo->u16Hstart = ((MS_U16)u8SampleTable[2]) | (((MS_U16)u8SampleTable[3])<<8);
            pInfo->u16Hsize  = ((MS_U16)u8SampleTable[4]) | (((MS_U16)u8SampleTable[5])<<8);
            pInfo->u16Vsize  = ((MS_U16)u8SampleTable[6]) | (((MS_U16)u8SampleTable[7])<<8);
            pInfo->u16Htt    = ((MS_U16)u8SampleTable[8]) | (((MS_U16)u8SampleTable[9])<<8);
            pInfo->eType = PQ_VD_SAMPLING_ON;

            PQTAB_DBG(printf("VD x:%x, y:%x, w:%x, h:%x, Htt=%x \n",
                pInfo->u16Hstart, pInfo->u16Vstart, pInfo->u16Hsize, pInfo->u16Vsize, pInfo->u16Htt));
        }

        _stVD_Info[eWindow].enVideoStandard  = eCurStandard;
        _u16PQSrcType[eWindow] = u16CurSrc;
    }
    else
    {
        pInfo->eType = PQ_VD_SAMPLING_OFF;
    }
}

#endif


MS_BOOL _MDrv_PQ_Get_HSDSampling(PQ_WIN eWindow, MS_U32 *pu32ratio, MS_BOOL *bADVMode)
{
    MS_U16 u16IPIdx = Hal_PQ_get_ip_idx(MS_PQ_IP_HSD_SAMPLING);
    MS_U16 u16TableIdx;
    MS_BOOL bret;

    if(u16IPIdx != 0xFFFF)
    {
        u16TableIdx = MDrv_PQ_GetTableIndex(eWindow, u16IPIdx);
    }
    else
    {
    #if(ENABLE_PQ_BIN)
        u16TableIdx = (gbPQBinEnable) ? PQ_BIN_IP_NULL : PQ_IP_NULL;
    #else
        u16TableIdx = PQ_IP_NULL;
    #endif
    }


#if(ENABLE_PQ_BIN)
    if ( (gbPQBinEnable == 1 && u16TableIdx == PQ_BIN_IP_NULL) ||
         (gbPQBinEnable == 0 && u16TableIdx == PQ_IP_NULL))
#else
    if( u16TableIdx == PQ_IP_NULL )
#endif
    {
        *pu32ratio = 0;
        bret = FALSE;
    }
    else
    {
        if(u16TableIdx == Hal_PQ_get_hsd_sampling_idx(MS_HSD_SAMPLING_X_1_000))
        {
            *pu32ratio = 1000;
        }
        else if(u16TableIdx == Hal_PQ_get_hsd_sampling_idx(MS_HSD_SAMPLING_X_0_875))
        {
            *pu32ratio = 875;
        }
        else if(u16TableIdx == Hal_PQ_get_hsd_sampling_idx(MS_HSD_SAMPLING_X_0_750))
        {
            *pu32ratio = 750;
        }
        else if(u16TableIdx == Hal_PQ_get_hsd_sampling_idx(MS_HSD_SAMPLING_X_0_625))
        {
            *pu32ratio = 625;
        }
        else if(u16TableIdx == Hal_PQ_get_hsd_sampling_idx(MS_HSD_SAMPLING_X_0_500))
        {
            *pu32ratio = 500;
        }
        else if(u16TableIdx == Hal_PQ_get_hsd_sampling_idx(MS_HSD_SAMPLING_X_0_375))
        {
            *pu32ratio = 375;
        }
        else if(u16TableIdx == Hal_PQ_get_hsd_sampling_idx(MS_HSD_SAMPLING_X_0_250))
        {
            *pu32ratio = 250;
        }
        else if(u16TableIdx == Hal_PQ_get_hsd_sampling_idx(MS_HSD_SAMPLING_X_0_125))
        {
            *pu32ratio = 125;
        }
        else
        {
            *pu32ratio = 0;
        }

        bret = TRUE;
    }

    // Load HSD_Y & HSD_C
    if(bret)
    {
        MS_U16 u16HSD_Y_idx, u16HSD_C_idx;

        u16HSD_Y_idx = MDrv_PQ_GetTableIndex(eWindow, PQ_IP_HSD_Y_Main);
        MDrv_PQ_LoadTable(eWindow, u16HSD_Y_idx, PQ_IP_HSD_Y_Main);

        u16HSD_C_idx = MDrv_PQ_GetTableIndex(eWindow, PQ_IP_HSD_C_Main);
        MDrv_PQ_LoadTable(eWindow, u16HSD_C_idx, PQ_IP_HSD_C_Main);

        SRULE_DBG(printf("HSD_Y:%d, HSD_C:%d\n", u16HSD_Y_idx, u16HSD_C_idx));

         *bADVMode = ( (u16HSD_Y_idx == PQ_IP_HSD_Y_CB_Main) &&
                       (u16HSD_C_idx == PQ_IP_HSD_C_CB_Main)) ? FALSE : TRUE;
    }
    else
    {
        *bADVMode = FALSE;
    }


    SRULE_DBG(printf("[PQ] GetHSDSampling, IPIdx=%d, Table=%d, ratio=%ld ret=%d\n", u16IPIdx, u16TableIdx, *pu32ratio, bret));

    return bret;
}

#if PQ_ENABLE_IOCTL
MS_BOOL MDrv_PQ_IOCTL(PQ_WIN eWindow, MS_U32 u32Flag, void *pBuf, MS_U32 u32BufSize)
{
    MS_BOOL bret = TRUE;

    switch(u32Flag)
    {
    case E_PQ_IOCTL_HSD_SAMPLING:
    {
        if(u32BufSize == sizeof(PQ_HSD_SAMPLING_INFO))
        {
            MS_U32 u32ratio;
            MS_BOOL bADVMode;

            bret = _MDrv_PQ_Get_HSDSampling(eWindow, &u32ratio, &bADVMode);
            ((PQ_HSD_SAMPLING_INFO *)pBuf)->u32ratio = u32ratio;
            ((PQ_HSD_SAMPLING_INFO *)pBuf)->bADVMode = bADVMode;
            bret = TRUE;

            //printf("ratio %lx, bADVmode=%d\n\n",((PQ_HSD_SAMPLING_INFO *)pBuf)->u32ratio, ((PQ_HSD_SAMPLING_INFO *)pBuf)->bADVMode);

        }
        else
        {
            //printf("Size is not correct, in=%ld, %d\n", u32BufSize, sizeof(PQ_HSD_SAMPLING_INFO));
            ((PQ_HSD_SAMPLING_INFO *)pBuf)->u32ratio = 0;
            ((PQ_HSD_SAMPLING_INFO *)pBuf)->bADVMode = 0;
            bret = FALSE;
        }
        break;
    }

    case E_PQ_IOCTL_PREVSD_BILINEAR:
        bret = TRUE;
        *((MS_BOOL *)pBuf) = TRUE;
        break;

    case E_PQ_IOCTL_PQ_SUGGESTED_FRAMENUM:
    {
        if(u32BufSize == sizeof(MS_U16))
        {
            *((MS_U16 *)pBuf) = _u16PQSuggestedFrameNum[eWindow];
            bret = TRUE;
        }
        else
        {
            *((MS_U16 *)pBuf) = 0;
            bret = FALSE;
        }
        break;
    }

    default:
        printf("unknown IOCTL %lx\n", u32Flag);
        pBuf = pBuf;
        u32BufSize = u32BufSize;
        bret = FALSE;
        break;
    }

    //printf("[PQ] IOCTL : win:%x, Flag=%08lx, size=%ld, ret:%x\n", eWindow, u32Flag, u32BufSize, bret);

    return bret;
}
#endif

MS_PQ_ADC_SAMPLING_TYPE MDrv_PQ_Get_ADCSampling_Info(PQ_WIN eWindow, PQ_INPUT_SOURCE_TYPE enInputSourceType, MS_PQ_Mode_Info *pstPQModeInfo)
{
    MS_PQ_ADC_SAMPLING_TYPE enRetType;
    MS_U16 u16PQSrcType;
    MS_U16 u16TableIdx;
    MS_PQ_Mode_Info stModeInfo;

    u16PQSrcType = _u16PQSrcType[eWindow];
    memcpy(&stModeInfo, &_stMode_Info[eWindow], sizeof(MS_PQ_Mode_Info));

     MDrv_PQ_Set_ModeInfo(eWindow, enInputSourceType, pstPQModeInfo);
    _u16PQSrcType[eWindow] = QM_InputSourceToIndex(eWindow, enInputSourceType);


    u16TableIdx = MDrv_PQ_GetTableIndex(eWindow, Hal_PQ_get_ip_idx(MS_PQ_IP_ADC_SAMPLING));

    if(u16TableIdx == Hal_PQ_get_adc_sampling_idx(MS_ADC_SAMPLING_X_1))
    {
        enRetType = E_PQ_ADC_SAMPLING_X_1;
    }
    else if(u16TableIdx == Hal_PQ_get_adc_sampling_idx(MS_ADC_SAMPLING_X_2))
    {
        enRetType = E_PQ_ADC_SAMPLING_X_2;
    }
    else
    {
        enRetType = E_PQ_ADC_SAMPLING_NONE;
    }

    memcpy(&_stMode_Info[eWindow], &stModeInfo, sizeof(MS_PQ_Mode_Info));
    _u16PQSrcType[eWindow] = u16PQSrcType;

    return enRetType;
}

void MDrv_PQ_Set_SCBufSize(PQ_WIN eWindow, MS_U32 u32BufSize)
{
    _u32SCBuf_Size[eWindow] = u32BufSize;
}


MS_BOOL MDrv_PQ_Set3D_OnOff(MS_BOOL bEn)
{
    _gIs3D_En = bEn;
    return TRUE;
}

MS_BOOL MDrv_PQ_GetCaps(MS_PQ_CAP_INFO *pInfo)
{
#if(PQ_ENABLE_PIP)
    pInfo->bPIP_Supported = TRUE;
#else
    pInfo->bPIP_Supported = FALSE;
#endif

#if(PQ_ENABLE_3D_VIDEO)
    pInfo->b3DVideo_Supported = TRUE;
#else
    pInfo->b3DVideo_Supported = FALSE;
#endif

    return TRUE;
}

MS_BOOL MDrv_PQ_Get420UpSampling_OnOff(void)
{
    return (MS_BOOL)PQ_ENABLE_420UPSAMPLING;
}

MS_U32 MDrv_PQ_SetPowerState(EN_POWER_MODE enPowerState)
{
    MS_U32 u32Ret = E_PQ_RET_FAIL;

    if (_status.bIsInitialized == FALSE)
    {
        printf("[%s,%5d] PQ is not initialized\n",__FUNCTION__,__LINE__);
        return u32Ret;
    }

    if (enPowerState == E_POWER_SUSPEND)
    {
        _enPrevPowerState = enPowerState;
        u32Ret = E_PQ_RET_OK;
    }
    else if (enPowerState == E_POWER_RESUME)
    {
        _enPrevPowerState = enPowerState;

        if (gu32LastPQIPSRAM1_Main != 0xFFFFFFFF)
        {
            Hal_PQ_set_y_sram_table(SC_FILTER_Y_SRAM1, (void *)gu32LastPQIPSRAM1_Main);
        }

        if (gu32LastPQIPSRAM2_Main != 0xFFFFFFFF)
        {
            Hal_PQ_set_y_sram_table(SC_FILTER_Y_SRAM2, (void *)gu32LastPQIPSRAM2_Main);
        }

        if (gu32LastPQIPCSRAM1_Main != 0xFFFFFFFF)
        {
            Hal_PQ_set_c_sram_table(SC_FILTER_C_SRAM1, (void *)gu32LastPQIPCSRAM1_Main);
        }

        if (gu32LastPQIPCSRAM2_Main != 0xFFFFFFFF)
        {
            Hal_PQ_set_c_sram_table(SC_FILTER_C_SRAM2, (void *)gu32LastPQIPCSRAM2_Main);
        }

        if (gu32LastPQIPVEHSDSRAM1_Main != 0xFFFFFFFF)
        {
            Hal_PQ_set_ve_h_sram_table(SC_FILTER_VE_H_SRAM1, (void *)gu32LastPQIPVEHSDSRAM1_Main);
        }

        if (gu32LastPQIPVEHSDSRAM2_Main != 0xFFFFFFFF)
        {
            Hal_PQ_set_ve_h_sram_table(SC_FILTER_VE_H_SRAM2, (void *)gu32LastPQIPVEHSDSRAM2_Main);
        }

        if (gu32LastPQIPVEVSDSRAM1_Main != 0xFFFFFFFF)
        {
            Hal_PQ_set_ve_v_sram_table(SC_FILTER_VE_V_SRAM1, (void *)gu32LastPQIPVEVSDSRAM1_Main);
        }

        if (gu32LastPQIPVEVSDSRAM2_Main != 0xFFFFFFFF)
        {
            Hal_PQ_set_ve_v_sram_table(SC_FILTER_VE_V_SRAM2, (void *)gu32LastPQIPVEVSDSRAM2_Main);
        }

        //common bw table
        MDrv_BW_LoadInitTable();
        u32Ret = E_PQ_RET_OK;
    }
    else
    {
        printf("[%s,%5d]Do Nothing: %u\n",__FUNCTION__,__LINE__,enPowerState);
        u32Ret = E_PQ_RET_FAIL;
    }
    return u32Ret;

}

