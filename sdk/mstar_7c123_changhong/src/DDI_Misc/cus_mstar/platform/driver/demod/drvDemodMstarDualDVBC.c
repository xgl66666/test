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
#include "Board.h"
#if IS_THIS_DEMOD_PICKED(DEMOD_MSINTERN_DVBC_DUAL)
#if defined(CHIP_KAISER) || defined(CHIP_K6LITE)
#include "MsCommon.h"
#include "HbCommon.h"
#include "drvDMD_common.h"
#include "drvDMD_INTERN_DVBC.h"
#include "drvDemod.h"
#include "drvDemodNull.h"
#include "drvDMD_VD_MBX.h"
#include "drvSAR.h"
#include <string.h>


extern MS_BOOL MDrv_DMD_DVBC_Dual_Individual_Init(DMD_DVBC_InitData *pDMD_DVBC_InitData, MS_U32 u32InitDataLen);
extern MS_BOOL MDrv_DMD_DVBC_Dual_Public_Init(MS_U8 u8AGC_Tristate_Ctrl,MS_U8 u8Sar_Channel);

DRV_DEMOD_TABLE_TYPE GET_DEMOD_ENTRY_NODE(DEMOD_MSINTERN_DVBC_DUAL)  DDI_DRV_TABLE_ENTRY(demodtab) ;

#if (defined(UFO_DEMOD_DVBC_SUPPORT_DMD_INT) && INTERN_DVBC_SUPPORT_DMD_INT)
#define DMD_INDEX_INVERT_AUTO 1
#else
#define DMD_INDEX_INVERT_AUTO 0
#endif
#define COFDMDMD_MUTEX_TIMEOUT       (2000)
#define MAX_DMD_NUM                2



static MS_BOOL bInited[MAX_DMD_NUM] = {FALSE,FALSE};
static MS_S32 _s32MutexId = -1;
static DEMOD_MS_INIT_PARAM DualDVBC_INIT_PARAM[MAX_DMD_NUM];
static MS_U32 _u32IFrequency = 5000;
static MS_BOOL bDemodIndexInvert = FALSE; // R858 FALSE
static DMD_IFAGC_SSI* pTuner_IFAGC_SSI_LoRef[MAX_DMD_NUM] = {NULL, NULL};
static MS_U16 u16Tuner_IfagcSsi_LoRef_Size[MAX_DMD_NUM] = {0,0};

#if DMD_INDEX_INVERT_AUTO
static MS_BOOL bIndexCheckDone = FALSE;
static MS_BOOL bIndexInvert_ori = FALSE;
static MS_S32 _s32DMDEventId = -1;
static MS_U32 u32Events = 0;
static MS_U8 u8DMDStatus = 0;
#define EVT_MASK          0x0F
#define EVT_DMD_INT       0x01
#endif

static DMD_RFAGC_SSI ALPS_TUNER_RfagcSsi[] =
{
    {-15.00,    0x19},
    {-25.00,    0x31},
    {-26.00,    0x33},
    {-27.00,    0x35},
    {-28.00,    0x35},
    {-29.00,    0x36},
    {-30.00,    0x37},
    {-31.00,    0x38},
    {-32.00,    0x3A},
    {-33.00,    0x3E},
    {-34.00,    0x40},
    {-35.00,    0x41},
    {-36.00,    0x43},
    {-37.00,    0x44},
    {-38.00,    0x46},
    {-39.00,    0x47},
    {-40.00,    0x49},
    {-41.00,    0x4B},
    {-42.00,    0x4E},
    {-43.00,    0x50},
    {-44.00,    0x53},
    {-45.00,    0x56},
    {-46.00,    0x59},
    {-46.50,    0x5B},
    {-47.00,    0x5D},
    {-48.00,    0x62},
    {-49.00,    0x67},
    {-50.00,    0x6B},
    {-51.00,    0x73},
    {-52.00,    0x7A},
    {-53.00,    0x85},
    {-53.50,    0x8E},
    {-54.00,    0x98},
    {-54.10,    0x9F},
    {-54.20,    0xA4},
    {-54.30,    0xA7},
    {-54.40,    0xAC},
    {-55.00,    0xAC},
    {-55.00,    0xFF},
};


//for mxl 608
static DMD_IFAGC_SSI TunerMxL608_IFAGC_SSI_LoRef[] =
{
    {250,       0x00},
    {249,       0x01},
    {-10.00,    0x96},
    {-20.00,    0x9d},
    {-30.00,    0xa3},
    {-40.00,    0xa8},
    {-50.00,    0xae},
    {-60.00,    0xb3},
    {-70.00,    0xb8},
    {-80.00,    0xbe},
    {-90.00,    0xc2},
    {-100.00,   0xff},
};




static DMD_IFAGC_SSI ALPS_TUNER_IfagcSsi_HiRef[] =
{
    {-54.30,    0x5F},
    {-54.40,    0x60},
    {-55.00,    0x63},
    {-56.00,    0x68},
    {-57.50,    0x6C},
	{-58.00,    0x70},
    {-59.00,    0x75},
    {-60.00,    0x79},
    {-61.40,    0x7D},
    {-62.00,    0x80},
    {-63.00,    0x81},
    {-64.00,    0x83},
    {-65.00,    0x84},
    {-66.00,    0x85},
    {-67.00,    0x87},
    {-68.00,    0x88},
    {-69.00,    0x89},
    {-70.00,    0x8A},
    {-71.00,    0x8C},
    {-72.00,    0x8D},
    {-73.00,    0x8F},
    {-74.00,    0x90},
    {-75.00,    0x92},
    {-76.00,    0x93},
    {-77.00,    0x95},
    {-78.00,    0x96},
    {-79.00,    0x98},
    {-80.00,    0x99},
    {-81.00,    0x9B},
    {-82.00,    0x9D},
    {-83.00,    0x9E},
    {-84.00,    0xA0},
    {-85.00,    0xA2},
    {-86.00,    0xA4},
    {-87.00,    0xA6},
    {-88.00,    0xA8},
    {-89.00,    0xAA},
    {-90.00,    0xAC},
    {-91.00,    0xAD},
    {-92.00,    0xAF},
    {-93.00,    0xB0},
    {-93.00,    0xFF},
};

static DMD_IFAGC_ERR ALPS_TUNER_IfagcErr_LoRef[] =
{
    {0.00,        0x0A},
    {0.00,        0xFF},

};

static DMD_IFAGC_ERR ALPS_TUNER_IfagcErr_HiRef[] =
{
    {0.00,        0x2D},
    {0.00,        0xFF},        // stopper
};

static DMD_SQI_CN_NORDIGP1 SqiCnNordigP1[] =
{
    {_QPSK,  _CR1Y2, 5.1 },
    {_QPSK,  _CR2Y3, 6.9 },
    {_QPSK,  _CR3Y4, 7.9 },
    {_QPSK,  _CR5Y6, 8.9 },
    {_QPSK,  _CR7Y8, 9.7 },
    {_16QAM, _CR1Y2, 10.8},
    {_16QAM, _CR2Y3, 13.1},
    {_16QAM, _CR3Y4, 12.2},
    {_16QAM, _CR5Y6, 15.6},
    {_16QAM, _CR7Y8, 16.0},
    {_64QAM, _CR1Y2, 16.5},
    {_64QAM, _CR2Y3, 16.3},
    {_64QAM, _CR3Y4, 17.8},
	{_64QAM, _CR3Y4, 21.2},
    {_64QAM, _CR5Y6, 21.6},
    {_64QAM, _CR7Y8, 22.5},
};

static MS_U8 _IdxMapping(MS_U8 u8DemodIndex)
{
    MS_U8 u8DMD_Index = 0;
    if(bDemodIndexInvert)
    {
        u8DMD_Index = ((u8DemodIndex + 1)%MAX_DMD_NUM);
    }
    else
    {
        u8DMD_Index = (u8DemodIndex % MAX_DMD_NUM);
    }
    //DMD_DBG(("DMD_Index of Dual DVBC = %x\n", u8DMD_Index));
    return u8DMD_Index;
}

#if (defined(UFO_DEMOD_DVBC_SUPPORT_DMD_INT) && INTERN_DVBC_SUPPORT_DMD_INT)
static void _mdrv_dualdvbc_demod_cb(MS_U8 u8arg)
{
    MS_U8 u8DemodIndex;

    u8DemodIndex = (u8arg & 0xf0) >> 4;

    if(u8DemodIndex > 1)
        return;

    #if DMD_INDEX_INVERT_AUTO
    if((((u8arg & 0xf) == 0x01) || ((u8arg & 0xf) == 0x02)) && (bIndexCheckDone == FALSE))
    {
        u8DMDStatus = u8arg;
        MsOS_ClearEvent(_s32DMDEventId, EVT_MASK);
        MsOS_SetEvent(_s32DMDEventId, EVT_DMD_INT);

    }
    #endif

    if(DualDVBC_INIT_PARAM[u8DemodIndex].fpCB!= NULL)
    {
        DualDVBC_INIT_PARAM[u8DemodIndex].fpCB(DualDVBC_INIT_PARAM[u8DemodIndex].u8FrontendIndex,u8arg);
    }
}
#endif

MS_BOOL MDrv_Demod_Dual_Public_Init(void )
{
    //DMD_DVBC_InitData sDMD_DVBC_InitData;
    MS_BOOL ret;

    if (_s32MutexId < 0)
    {
        _s32MutexId = MsOS_CreateMutex(E_MSOS_FIFO, "OfDmd_Mutex", MSOS_PROCESS_SHARED);

        if (_s32MutexId < 0)
        {
            DMD_ERR(("%s: Create mutex failed.\n", __FUNCTION__));
            return FALSE;
        }

    }

    MDrv_SYS_DMD_VD_MBX_Init();

    ret=MDrv_DMD_DVBC_Dual_Public_Init(1,0xff);  //tristate_en,SARchannel

    /*
    if(ret == TRUE)
    {
        bInited = TRUE;
    }
*/

    return ret;
}

MS_BOOL MDrv_Demod_Individual_Init_0(void)
{
    DMD_DVBC_InitData sDMD_DVBC_InitData;
    MS_BOOL ret;

    static MS_U8 u8DMD_DVBC_InitExt_0[]={
       4, // version
       0, // reserve
       0x15, // TS_CLK
       (MS_U8)(AutoSymbol_Timeout>>8), // AutoSymbol_Timeout  10000ms~
	   (MS_U8)(AutoSymbol_Timeout>>0),
	   (MS_U8)(FixSymbol_AutoQam_Timeout>>8), // FixSymbol_AutoQam_Timeout 2000ms~
	   (MS_U8)(FixSymbol_AutoQam_Timeout>>0),
	   (MS_U8)(FixSymbol_FixQam_Timeout>>8), // FixSymbol_FixQam_Timeout  2000ms~
	   (MS_U8)(FixSymbol_FixQam_Timeout>>0),
       };                // tuner parameter

    // tuner parameter
    sDMD_DVBC_InitData.u8SarChannel=0xff; // 0xFF means un-connected
    sDMD_DVBC_InitData.pTuner_RfagcSsi=ALPS_TUNER_RfagcSsi;
    sDMD_DVBC_InitData.u16Tuner_RfagcSsi_Size=sizeof(ALPS_TUNER_RfagcSsi)/sizeof(DMD_RFAGC_SSI);
    sDMD_DVBC_InitData.pTuner_IfagcSsi_LoRef=pTuner_IFAGC_SSI_LoRef[0];
    sDMD_DVBC_InitData.u16Tuner_IfagcSsi_LoRef_Size=u16Tuner_IfagcSsi_LoRef_Size[0];
    sDMD_DVBC_InitData.pTuner_IfagcSsi_HiRef=ALPS_TUNER_IfagcSsi_HiRef;
    sDMD_DVBC_InitData.u16Tuner_IfagcSsi_HiRef_Size=sizeof(ALPS_TUNER_IfagcSsi_HiRef)/sizeof(DMD_IFAGC_SSI);
    sDMD_DVBC_InitData.pTuner_IfagcErr_LoRef=ALPS_TUNER_IfagcErr_LoRef;
    sDMD_DVBC_InitData.u16Tuner_IfagcErr_LoRef_Size=sizeof(ALPS_TUNER_IfagcErr_LoRef)/sizeof(DMD_IFAGC_SSI);
    sDMD_DVBC_InitData.pTuner_IfagcErr_HiRef=ALPS_TUNER_IfagcErr_HiRef;
    sDMD_DVBC_InitData.u16Tuner_IfagcErr_HiRef_Size=sizeof(ALPS_TUNER_IfagcErr_HiRef)/sizeof(DMD_IFAGC_SSI);
    sDMD_DVBC_InitData.pSqiCnNordigP1=SqiCnNordigP1;
    sDMD_DVBC_InitData.u16SqiCnNordigP1_Size=sizeof(SqiCnNordigP1)/sizeof(DMD_SQI_CN_NORDIGP1);

    // register init
    sDMD_DVBC_InitData.u8DMD_DVBC_DSPRegInitExt=NULL; // TODO use system variable type
    sDMD_DVBC_InitData.u8DMD_DVBC_DSPRegInitSize=0;
    sDMD_DVBC_InitData.u8DMD_DVBC_InitExt=u8DMD_DVBC_InitExt_0; // TODO use system variable type

    MDrv_DMD_DVBC_ActiveDmdSwitch(0);
    ret = MDrv_DMD_DVBC_Dual_Individual_Init(&sDMD_DVBC_InitData, sizeof(sDMD_DVBC_InitData)); // _UTOPIA
    if(ret == TRUE)
    {
        bInited[0] = TRUE;
    }


    return ret;
}

MS_BOOL MDrv_Demod_Individual_Init_1(void)
{
    DMD_DVBC_InitData sDMD_DVBC_InitData;
    MS_BOOL ret;

    static MS_U8 u8DMD_DVBC_InitExt_1[]={
       4, // version
       0, // reserve
       0x15, // TS_CLK
       (MS_U8)(AutoSymbol_Timeout>>8), // AutoSymbol_Timeout  10000ms~
	   (MS_U8)(AutoSymbol_Timeout>>0),
	   (MS_U8)(FixSymbol_AutoQam_Timeout>>8), // FixSymbol_AutoQam_Timeout 2000ms~
	   (MS_U8)(FixSymbol_AutoQam_Timeout>>0),
	   (MS_U8)(FixSymbol_FixQam_Timeout>>8), // FixSymbol_FixQam_Timeout  2000ms~
	   (MS_U8)(FixSymbol_FixQam_Timeout>>0),
       };                // tuner parameter

    // tuner parameter
    sDMD_DVBC_InitData.u8SarChannel=0xff; // 0xFF means un-connected
    sDMD_DVBC_InitData.pTuner_RfagcSsi=ALPS_TUNER_RfagcSsi;
    sDMD_DVBC_InitData.u16Tuner_RfagcSsi_Size=sizeof(ALPS_TUNER_RfagcSsi)/sizeof(DMD_RFAGC_SSI);
    sDMD_DVBC_InitData.pTuner_IfagcSsi_LoRef=pTuner_IFAGC_SSI_LoRef[1];
    sDMD_DVBC_InitData.u16Tuner_IfagcSsi_LoRef_Size=u16Tuner_IfagcSsi_LoRef_Size[1];
    sDMD_DVBC_InitData.pTuner_IfagcSsi_HiRef=ALPS_TUNER_IfagcSsi_HiRef;
    sDMD_DVBC_InitData.u16Tuner_IfagcSsi_HiRef_Size=sizeof(ALPS_TUNER_IfagcSsi_HiRef)/sizeof(DMD_IFAGC_SSI);
    sDMD_DVBC_InitData.pTuner_IfagcErr_LoRef=ALPS_TUNER_IfagcErr_LoRef;
    sDMD_DVBC_InitData.u16Tuner_IfagcErr_LoRef_Size=sizeof(ALPS_TUNER_IfagcErr_LoRef)/sizeof(DMD_IFAGC_SSI);
    sDMD_DVBC_InitData.pTuner_IfagcErr_HiRef=ALPS_TUNER_IfagcErr_HiRef;
    sDMD_DVBC_InitData.u16Tuner_IfagcErr_HiRef_Size=sizeof(ALPS_TUNER_IfagcErr_HiRef)/sizeof(DMD_IFAGC_SSI);
    sDMD_DVBC_InitData.pSqiCnNordigP1=SqiCnNordigP1;
    sDMD_DVBC_InitData.u16SqiCnNordigP1_Size=sizeof(SqiCnNordigP1)/sizeof(DMD_SQI_CN_NORDIGP1);

    // register init
    sDMD_DVBC_InitData.u8DMD_DVBC_DSPRegInitExt=NULL; // TODO use system variable type
    sDMD_DVBC_InitData.u8DMD_DVBC_DSPRegInitSize=0;
    sDMD_DVBC_InitData.u8DMD_DVBC_InitExt=u8DMD_DVBC_InitExt_1; // TODO use system variable type

    MDrv_DMD_DVBC_ActiveDmdSwitch(1);
    ret = MDrv_DMD_DVBC_Dual_Individual_Init(&sDMD_DVBC_InitData, sizeof(sDMD_DVBC_InitData)); // _UTOPIA
    if(ret == TRUE)
    {
        bInited[1] = TRUE;
    }


    return ret;
}

MS_BOOL MDrv_Demod_MSDVBC_51_Init(MS_U8 u8DemodIndex,DEMOD_MS_INIT_PARAM* pParam)
{
    MS_BOOL ret = TRUE;
    #if DMD_INDEX_INVERT_AUTO
    MS_U8 i;
    #endif

    u8DemodIndex = _IdxMapping(u8DemodIndex);

    if(NULL == pParam)
        return FALSE;

    if ( (bInited[0] == TRUE) || (bInited[1] == TRUE) )
    {
    }
    else
    {
        ret &= MDrv_Demod_Dual_Public_Init();
    }

#if DMD_INDEX_INVERT_AUTO
    // Both demod are 1st time init, fill same init parameter before checking if index need to invert
    if((!bIndexCheckDone) && (bInited[0] == FALSE) && (bInited[1] == FALSE))
    {
        DMD_DBG(("[DualDVBC] Init ALL demods\n"));
        for(i=0;i<MAX_DMD_NUM;i++)
        {
            memcpy(&DualDVBC_INIT_PARAM[i], pParam, sizeof(DEMOD_MS_INIT_PARAM));
            if(DualDVBC_INIT_PARAM[i].pstTunertab->data == TUNER_MXL608)
            {
                pTuner_IFAGC_SSI_LoRef[i] = TunerMxL608_IFAGC_SSI_LoRef;
                u16Tuner_IfagcSsi_LoRef_Size[i] = sizeof(TunerMxL608_IFAGC_SSI_LoRef)/sizeof(DMD_IFAGC_SSI);
            }
            else
            {
                pTuner_IFAGC_SSI_LoRef[i] = NULL;
                u16Tuner_IfagcSsi_LoRef_Size[i] = 0;
                DMD_DBG(("SSI NOT support\n"));
            }
            if(i)
                ret &= MDrv_Demod_Individual_Init_1();
            else
                ret &= MDrv_Demod_Individual_Init_0();
        }
    }
    else
#endif        
    {
        memcpy(&DualDVBC_INIT_PARAM[u8DemodIndex], pParam, sizeof(DEMOD_MS_INIT_PARAM));
        DMD_DBG(("[DualDVBC]Init signal demod %x,FE index = %x\n", u8DemodIndex, DualDVBC_INIT_PARAM[u8DemodIndex].u8FrontendIndex));
        if(DualDVBC_INIT_PARAM[u8DemodIndex].pstTunertab->data == TUNER_MXL608)
        {
            pTuner_IFAGC_SSI_LoRef[u8DemodIndex] = TunerMxL608_IFAGC_SSI_LoRef;
            u16Tuner_IfagcSsi_LoRef_Size[u8DemodIndex] = sizeof(TunerMxL608_IFAGC_SSI_LoRef)/sizeof(DMD_IFAGC_SSI);
        }
        else
        {
            pTuner_IFAGC_SSI_LoRef[u8DemodIndex] = NULL;
            u16Tuner_IfagcSsi_LoRef_Size[u8DemodIndex] = 0;
            DMD_DBG(("SSI NOT support\n"));
        }

        switch (u8DemodIndex)
        {
            case 0:
                ret &= MDrv_Demod_Individual_Init_0();
                break;
            case 1:
                ret &= MDrv_Demod_Individual_Init_1();
                break;
            default:
                DMD_ERR(("Error : invalid u8DemodIndex in MDrv_Demod_MSDVBC_51_Init\n"));
                ret = FALSE;
                break;
        }
    }


#if (defined(UFO_DEMOD_DVBC_SUPPORT_DMD_INT) && INTERN_DVBC_SUPPORT_DMD_INT)
    MDrv_DMD_DVBC_Reg_INT_CB(_mdrv_dualdvbc_demod_cb);
    #endif
    
#if DMD_INDEX_INVERT_AUTO
        if (_s32DMDEventId < 0)
        {
            _s32DMDEventId = MsOS_CreateEventGroup("INDEX_INVERT_AUTO_Event");
    
            if (_s32DMDEventId > 0)
            {
                DMD_DBG(("[%s][%d] Event create ok\n",__FUNCTION__,__LINE__));
            }
            else
            {
                DMD_ERR(("[%s][%d] create failed \n",__FUNCTION__,__LINE__));
            }
        }
#endif

    return ret;
}

MS_BOOL MDrv_Demod_MSDVBC_51_Open(MS_U8 u8DemodIndex)
{
    u8DemodIndex = _IdxMapping(u8DemodIndex);

    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    MDrv_DMD_DVBC_ActiveDmdSwitch(u8DemodIndex);
    HB_ReleaseMutex(_s32MutexId);

    return TRUE;
}

MS_BOOL MDrv_Demod_MSDVBC_51_Close(MS_U8 u8DemodIndex)
{
    MS_BOOL ret;

    u8DemodIndex = _IdxMapping(u8DemodIndex);

    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    MDrv_DMD_DVBC_ActiveDmdSwitch(u8DemodIndex);
    ret = MDrv_DMD_DVBC_Exit();
    if(ret == TRUE)
    {
        bInited[u8DemodIndex] = FALSE;
    }
    HB_ReleaseMutex(_s32MutexId);
    return ret;

}

MS_BOOL MDrv_Demod_MSDVBC_51_SetSerialControl(MS_U8 u8DemodIndex, MS_BOOL bEnable)
{
    MS_BOOL ret;

    u8DemodIndex = _IdxMapping(u8DemodIndex);


    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    if(bInited[u8DemodIndex] == FALSE)
    {
        HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }
    MDrv_DMD_DVBC_ActiveDmdSwitch(u8DemodIndex);
    ret = MDrv_DMD_DVBC_SetSerialControl(bEnable);
    HB_ReleaseMutex(_s32MutexId);
    return ret;
}

MS_BOOL MDrv_Demod_MSDVBC_51_PowerOnOff(MS_U8 u8DemodIndex, MS_BOOL bPowerOn)
{

    u8DemodIndex = _IdxMapping(u8DemodIndex);


    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    MDrv_DMD_DVBC_ActiveDmdSwitch(u8DemodIndex);
    HB_ReleaseMutex(_s32MutexId);
    return TRUE;
}

MS_BOOL MDrv_Demod_MSDVBC_51_GetLock(MS_U8 u8DemodIndex, EN_LOCK_STATUS *peLockStatus)
{
    DMD_DVBC_LOCK_STATUS LockStatus;

    u8DemodIndex = _IdxMapping(u8DemodIndex);


    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    //DMD_DBG(("[%s][%d] \n",__FUNCTION__,__LINE__));
    if(bInited[u8DemodIndex] == FALSE)
    {
        HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }
    MDrv_DMD_DVBC_ActiveDmdSwitch(u8DemodIndex);
    //DMD_DBG(("[%s][%d] \n",__FUNCTION__,__LINE__));
    if(MDrv_DMD_DVBC_GetLock(DMD_DVBC_GETLOCK, &LockStatus) != TRUE)
    {
        HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }
    DMD_DBG(("[%s][%d] LockStatus %d \n",__FUNCTION__,__LINE__,LockStatus));
    switch (LockStatus)
    {
        case DMD_DVBC_LOCK:
            *peLockStatus = E_DEMOD_LOCK;
            break;
        case DMD_DVBC_CHECKEND:
            *peLockStatus = E_DEMOD_CHECKEND;
            break;
        case DMD_DVBC_UNLOCK:
            *peLockStatus = E_DEMOD_UNLOCK;
            break;
        case DMD_DVBC_CHECKING:
        default:
            *peLockStatus = E_DEMOD_CHECKING;
            break;
    }
    HB_ReleaseMutex(_s32MutexId);
    return TRUE;
}

MS_BOOL MDrv_Demod_MSDVBC_51_GetSNR(MS_U8 u8DemodIndex, float *pfSNR)
{
    MS_BOOL ret;

    u8DemodIndex = _IdxMapping(u8DemodIndex);


    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    if(bInited[u8DemodIndex] == FALSE)
    {
        *pfSNR = 0;
        HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }

    MDrv_DMD_DVBC_ActiveDmdSwitch(u8DemodIndex);
    ret = MDrv_DMD_DVBC_GetSNR(pfSNR);
    HB_ReleaseMutex(_s32MutexId);
    return ret;
}

MS_BOOL MDrv_Demod_MSDVBC_51_GetBER(MS_U8 u8DemodIndex, float *pfBER)
{
    MS_BOOL ret;

    u8DemodIndex = _IdxMapping(u8DemodIndex);


    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    if(bInited[u8DemodIndex] == FALSE)
    {
        *pfBER = 0;
        HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }
    MDrv_DMD_DVBC_ActiveDmdSwitch(u8DemodIndex);
    ret = MDrv_DMD_DVBC_GetPostViterbiBer(pfBER);

    HB_ReleaseMutex(_s32MutexId);
    return ret;

}

MS_BOOL MDrv_Demod_MSDVBC_51_GetPWR(MS_U8 u8DemodIndex, MS_S32 *ps32Signal)
{
    MS_BOOL ret;
    MS_U8 u8TunerIndex;

    u8TunerIndex = u8DemodIndex;
    u8DemodIndex = _IdxMapping(u8DemodIndex);

    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    if(bInited[u8DemodIndex] == FALSE)
    {
        *ps32Signal = 0;
        HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }
    MDrv_DMD_DVBC_ActiveDmdSwitch(u8DemodIndex);
    ret = DualDVBC_INIT_PARAM[u8DemodIndex].pstTunertab->Extension_Function(u8TunerIndex,TUNER_EXT_FUNC_GET_POWER_LEVEL, ps32Signal);
    HB_ReleaseMutex(_s32MutexId);
    return ret;
}

MS_BOOL MDrv_Demod_MSDVBC_51_GetSSI(MS_U8 u8DemodIndex, MS_U16 *pu16SSI)
{
    MS_BOOL ret = TRUE;
    float fRFPowerDbm = 0;
    MS_U8 u8TunerIndex;
    int PowerLevel = 0;
        
    u8TunerIndex = u8DemodIndex;
    u8DemodIndex = _IdxMapping(u8DemodIndex);

    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    if(bInited[u8DemodIndex] == FALSE)
    {
        *pu16SSI = 0;
        HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }
    MDrv_DMD_DVBC_ActiveDmdSwitch(u8DemodIndex);

    ret &= DualDVBC_INIT_PARAM[u8DemodIndex].pstTunertab->Extension_Function(u8TunerIndex, TUNER_EXT_FUNC_GET_POWER_LEVEL, &PowerLevel);
    fRFPowerDbm = (float)PowerLevel;

    if(fRFPowerDbm == 0)
    {
        ret &= MDrv_DMD_DVBC_GetSignalStrength(pu16SSI);
    }
    else
    {
        ret &= MDrv_DMD_DVBC_GetSignalStrengthWithRFPower(pu16SSI, fRFPowerDbm);
    }

    HB_ReleaseMutex(_s32MutexId);
    return ret;
}


MS_BOOL MDrv_Demod_MSDVBC_51_GetSignalQuality(MS_U8 u8DemodIndex, MS_U16 *pu16quality)
{
    MS_BOOL ret;
    u8DemodIndex = _IdxMapping(u8DemodIndex);

    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    if(bInited[u8DemodIndex] == FALSE)
    {
        *pu16quality = 0;
        HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }
    MDrv_DMD_DVBC_ActiveDmdSwitch(u8DemodIndex);
    ret = MDrv_DMD_DVBC_GetSignalQuality(pu16quality);
    HB_ReleaseMutex(_s32MutexId);
    return ret;
}

MS_BOOL MDrv_Demod_MSDVBC_51_GetPacketError(MS_U8 u8DemodIndex, MS_U16 *pu16PktErr)
{
    MS_BOOL ret;
    u8DemodIndex = _IdxMapping(u8DemodIndex);

    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    if(bInited[u8DemodIndex] == FALSE)
    {
        *pu16PktErr = 0;
        HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }
    MDrv_DMD_DVBC_ActiveDmdSwitch(u8DemodIndex);
    ret = MDrv_DMD_DVBC_GetPacketErr(pu16PktErr);
    HB_ReleaseMutex(_s32MutexId);
    return ret;
}



MS_BOOL MDrv_Demod_MSDVBC_51_Config(MS_U8 u8DemodIndex, MS_U8 *pRegParam)
{
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    u8DemodIndex = _IdxMapping(u8DemodIndex);

    MDrv_DMD_DVBC_ActiveDmdSwitch(u8DemodIndex);
    HB_ReleaseMutex(_s32MutexId);
    return TRUE;
}


MS_BOOL MDrv_Demod_MSDVBC_51_GetParam(MS_U8 u8DemodIndex, DEMOD_MS_FE_CARRIER_PARAM* pParam)
{
    DMD_DVBC_MODULATION_TYPE QAMMode;
    MS_U16 u16SymbolRate;
    float FreqOff;

    u8DemodIndex = _IdxMapping(u8DemodIndex);


    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }


    if(bInited[u8DemodIndex] == FALSE)
    {
        HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }
    MDrv_DMD_DVBC_ActiveDmdSwitch(u8DemodIndex);
    if(FALSE == MDrv_DMD_DVBC_GetStatus(&QAMMode, &u16SymbolRate, &FreqOff))
    {
        HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }

    switch (QAMMode)
    {
        case DMD_DVBC_QAM16:
            pParam->CabParam.eConstellation = DEMOD_CAB_QAM16;
            break;
        case DMD_DVBC_QAM32:
            pParam->CabParam.eConstellation = DEMOD_CAB_QAM32;
            break;
        case DMD_DVBC_QAM64:
            pParam->CabParam.eConstellation = DEMOD_CAB_QAM64;
            break;
        case DMD_DVBC_QAM128:
            pParam->CabParam.eConstellation = DEMOD_CAB_QAM128;
            break;
        case DMD_DVBC_QAM256:
            pParam->CabParam.eConstellation = DEMOD_CAB_QAM256;
            break;
        case DMD_DVBC_QAMAUTO:
            pParam->CabParam.eConstellation = DEMOD_CAB_QAMAUTO;
            break;
        default:
            pParam->CabParam.eConstellation = DEMOD_CAB_QAM128;
            break;
    }

    pParam->CabParam.u16SymbolRate = u16SymbolRate;
    pParam->CabParam.u32FreqOffset = FreqOff;

    HB_ReleaseMutex(_s32MutexId);

    return TRUE;

}

MS_BOOL MDrv_Demod_MSDVBC_51_Restart(MS_U8 u8DemodIndex, DEMOD_MS_FE_CARRIER_PARAM* pParam,MS_U32 u32BroadCastType)
{
    DMD_DVBC_MODULATION_TYPE eModulationType;
    MS_U32 u32IF_Freq;
    MS_U8 u8TunerIndex;
    MS_U8 u8TryCnt = 1, i;
    MS_U8 u8DMDIndex = 0;
#if DMD_INDEX_INVERT_AUTO
    DEMOD_MS_INIT_PARAM stInitParam_tmp;
#endif    

    u8TunerIndex = u8DemodIndex;
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

#if DMD_INDEX_INVERT_AUTO
    if(!bIndexCheckDone)
    {
        u8TryCnt = 2;
        bIndexInvert_ori = bDemodIndexInvert;
        MsOS_ClearEvent(_s32DMDEventId, EVT_MASK);
        u8DMDStatus = 0;
        for(i=0; i<u8TryCnt;i++)
        {
            MDrv_DMD_DVBC_ActiveDmdSwitch(i);
            MDrv_DMD_DVBC_SetActive(FALSE);
        }
    }
#endif

    for(i=0; i<u8TryCnt;i++)//0:current 1:inverse    
    {
        #if DMD_INDEX_INVERT_AUTO
        u8DMDStatus = 0;
        #endif
        if(i)
        {
            if(bDemodIndexInvert)
                bDemodIndexInvert = FALSE;
            else
                bDemodIndexInvert = TRUE; 
        }

        u8DMDIndex = _IdxMapping(u8DemodIndex);

        if(bInited[u8DMDIndex] == FALSE)
        {
            HB_ReleaseMutex(_s32MutexId);
            return FALSE;
        }


        if(!DualDVBC_INIT_PARAM[u8DMDIndex].pstTunertab->GetTunerIF(u8TunerIndex,&u32IF_Freq))
        {
           u32IF_Freq = _u32IFrequency;
           DMD_DBG(("DualDVB-C demod get Tuner IF FAIL, use default IF %d kHz\n", (int)u32IF_Freq));
        }
        else
        {
           DMD_DBG(("DualDVB-C demod get Tuner IF %d kHz\n", (int)u32IF_Freq));
        }

        DMD_DBG(("DualDVB-C DMD %x avtive\n", u8DMDIndex));
        MDrv_DMD_DVBC_ActiveDmdSwitch(u8DMDIndex);
        switch(pParam->CabParam.eConstellation)
        {
            case DEMOD_CAB_QAM16:
                eModulationType = DMD_DVBC_QAM16;
                break;
            case DEMOD_CAB_QAM32:
                eModulationType = DMD_DVBC_QAM32;
                break;
            case DEMOD_CAB_QAM64:
                eModulationType = DMD_DVBC_QAM64;
                break;
            case DEMOD_CAB_QAM128:
                eModulationType = DMD_DVBC_QAM128;
                break;
            case DEMOD_CAB_QAM256:
                eModulationType = DMD_DVBC_QAM256;
                break;
            default:
                eModulationType = DMD_DVBC_QAMAUTO;
                break;
        }

        DMD_DBG(("pParam->CabParam.u16SymbolRate [%d], eModulationType [%d] \n",pParam->CabParam.u16SymbolRate,eModulationType));

        if(FALSE == MDrv_DMD_DVBC_SetConfig(pParam->CabParam.u16SymbolRate, eModulationType, u32IF_Freq, FALSE, FALSE))
        {
            DMD_ERR(("MDrv_DMD_DVBC_SetConfig Fail \n"));
            HB_ReleaseMutex(_s32MutexId);
            return FALSE;
        }
        if(FALSE == MDrv_DMD_DVBC_SetActive(TRUE))
        {
            DMD_ERR(("MDrv_DMD_DVBC_SetActive Fail \n"));
            HB_ReleaseMutex(_s32MutexId);
            return FALSE;
        }

#if DMD_INDEX_INVERT_AUTO
        if(bIndexCheckDone == FALSE)
        {
            DMD_DBG(("Wait for DMD INT...\n"));
            MsOS_WaitEvent(_s32DMDEventId, EVT_DMD_INT,&u32Events, E_OR_CLEAR, 10000);//max timeout:10 sec
             if(((u8DMDStatus == 0x1) && (u8DMDIndex == 0)) ||\
                ((u8DMDStatus == 0x11) && (u8DMDIndex == 1)))//DMD lock
             {
                 bIndexCheckDone = TRUE;
                 DMD_DBG(("DMD Index Check Done.. bDemodIndexInvert = %x\n", bDemodIndexInvert));
                 if(bDemodIndexInvert != bIndexInvert_ori)//init param inverse
                 {
                     memcpy(&stInitParam_tmp, &DualDVBC_INIT_PARAM[0], sizeof(DEMOD_MS_INIT_PARAM));
                     memcpy(&DualDVBC_INIT_PARAM[0], &DualDVBC_INIT_PARAM[1], sizeof(DEMOD_MS_INIT_PARAM));
                     memcpy(&DualDVBC_INIT_PARAM[1], &stInitParam_tmp, sizeof(DEMOD_MS_INIT_PARAM));
                 }
                 break;
             }
             else
             {
                 bDemodIndexInvert = bIndexInvert_ori;
             }
        }
#endif
    }

    HB_ReleaseMutex(_s32MutexId);
    return TRUE;

}


MS_BOOL MDrv_Demod_MSDVBC_51_SetMode(MS_U8 u8DemodIndex, Demod_Mode* pMode)
{
#if 0
    DMD_Mode dmdMod;
    dmdMod.bX4CFE_en            = pMode->bX4CFE_en;
    dmdMod.bPPD_en              = pMode->bPPD_en;
    dmdMod.bIQAutoSwap_en       = pMode->bIQAutoSwap_en;
    dmdMod.bQAMScan_en          = pMode->bQAMScan_en;
    dmdMod.bFHO_en              = pMode->bFHO_en;
    dmdMod.fptTunerSet          = pMode->fptTunerSet;
    return MDrv_Dmd_SetMode((DMD_Mode*)pMode);
#else
    u8DemodIndex = _IdxMapping(u8DemodIndex);


    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    MDrv_DMD_DVBC_ActiveDmdSwitch(u8DemodIndex);
    HB_ReleaseMutex(_s32MutexId);
    return TRUE;
#endif
}

MS_BOOL MDrv_Demod_MSDVBC_51_SetOutoutPath(MS_U8 u8DemodIndex, DEMOD_INTERFACE_MODE path)
{
    u8DemodIndex = _IdxMapping(u8DemodIndex);

    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    MDrv_DMD_DVBC_ActiveDmdSwitch(u8DemodIndex);
    HB_ReleaseMutex(_s32MutexId);
    return TRUE;
}

DEMOD_INTERFACE_MODE MDrv_Demod_MSDVBC_51_GetOutoutPath(MS_U8 u8DemodIndex)
{

    u8DemodIndex = _IdxMapping(u8DemodIndex);

    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    MDrv_DMD_DVBC_ActiveDmdSwitch(u8DemodIndex);
    HB_ReleaseMutex(_s32MutexId);
    return DEMOD_INTERFACE_PARALLEL;
}

MS_BOOL DEMOD_MSDVBC_51_Extension_Function(MS_U8 u8DemodIndex, DEMOD_EXT_FUNCTION_TYPE fuction_type, void *data)
{
    MS_BOOL bret = TRUE;

    u8DemodIndex = _IdxMapping(u8DemodIndex);
    switch(fuction_type)
    {
        case DEMOD_EXT_FUNC_FINALIZE:
          if (_s32MutexId >= 0)
          {
             MsOS_DeleteMutex(_s32MutexId);
            _s32MutexId = -1;
          }
        bInited[u8DemodIndex] = FALSE;
        MDrv_DMD_DVBC_ActiveDmdSwitch(u8DemodIndex);
        bret &= MDrv_DMD_DVBC_Exit();
        break;

#ifdef UFO_DEMOD_DVBC_GET_AGC_INFO
        case DEMOD_EXT_FUNC_GET_IFAGC_OUT:
            MDrv_DMD_DVBC_ActiveDmdSwitch(u8DemodIndex);
            MDrv_DMD_DVBC_GetAGCInfo(DBG_MODE_AGC_OUT, (MS_U16*)(data));
        break;
#endif
#if (defined(UFO_DEMOD_DVBC_SUPPORT_DMD_INT) && INTERN_DVBC_SUPPORT_DMD_INT)
        case DEMOD_EXT_FUNC_SET_INTERRUPT_CALLBACK:
            DualDVBC_INIT_PARAM[u8DemodIndex].fpCB = (fpDemodCB)(data);
            MDrv_DMD_DVBC_Reg_INT_CB(_mdrv_dualdvbc_demod_cb);
            break;
#endif

        default:
            DMD_DBG(("Request extension function (%x) does not exist\n",fuction_type));
            break;
    }

    return bret;
}

MS_BOOL MDrv_Demod_MSDVBC_51_I2C_ByPass(MS_U8 u8DemodIndex,MS_BOOL bOn)
{
  if(GET_DEMOD_ENTRY_NODE(DEMOD_MSINTERN_DVBC_DUAL).I2CByPassPreSetting != NULL)
  {
      return GET_DEMOD_ENTRY_NODE(DEMOD_MSINTERN_DVBC_DUAL).I2CByPassPreSetting(u8DemodIndex,bOn);
  }
  else
      return MDrv_Demod_null_I2C_ByPass(u8DemodIndex,bOn);
}

DRV_DEMOD_TABLE_TYPE GET_DEMOD_ENTRY_NODE(DEMOD_MSINTERN_DVBC_DUAL)  DDI_DRV_TABLE_ENTRY(demodtab) =
{
     .name                         = "DEMOD_MSINTERN_DVBC_DUAL",
     .data                         = DEMOD_MSINTERN_DVBC_DUAL,
#if (defined(UFO_DEMOD_DVBC_SUPPORT_DMD_INT) && INTERN_DVBC_SUPPORT_DMD_INT)     
     .SupportINT                   = TRUE,
#else
     .SupportINT                   = FALSE,
#endif
     .init                         = MDrv_Demod_MSDVBC_51_Init,
     .GetLock                      = MDrv_Demod_MSDVBC_51_GetLock,
     .GetSNR                       = MDrv_Demod_MSDVBC_51_GetSNR,
     .GetBER                       = MDrv_Demod_MSDVBC_51_GetBER,
     .GetPWR                       = MDrv_Demod_MSDVBC_51_GetPWR,
     .GetSSI                       = MDrv_Demod_MSDVBC_51_GetSSI,
     .GetQuality                   = MDrv_Demod_MSDVBC_51_GetSignalQuality,
     .GetParam                     = MDrv_Demod_MSDVBC_51_GetParam,
     .Restart                      = MDrv_Demod_MSDVBC_51_Restart,
     .I2CByPass                    = MDrv_Demod_MSDVBC_51_I2C_ByPass,
     .I2CByPassPreSetting          = NULL,
     .Extension_Function           = DEMOD_MSDVBC_51_Extension_Function,
     .Extension_FunctionPreSetting = NULL,
     .Get_Packet_Error             = MDrv_Demod_MSDVBC_51_GetPacketError,
     .CheckExist                   = MDrv_Demod_null_CheckExist,
     .CheckExist                   = MDrv_Demod_null_CheckExist,
#if MS_DVBT2_INUSE
     .SetCurrentDemodType          = MDrv_Demod_null_SetCurrentDemodType,
     .GetCurrentDemodType          = MDrv_Demod_null_GetCurrentDemodType,
     .GetPlpBitMap                 = MDrv_Demod_null_GetPlpBitMap,
     .GetPlpGroupID                = MDrv_Demod_null_GetPlpGroupID,
     .SetPlpGroupID                = MDrv_Demod_null_SetPlpGroupID,
     .GetNextPLPID                 = MDrv_Demod_null_GetNextPLPID,
     .GetPLPType                   = MDrv_Demod_null_GetPLPType,
#endif
#if MS_DVBS_INUSE
     .BlindScanStart               = MDrv_Demod_null_BlindScan_Start,
     .BlindScanNextFreq            = MDrv_Demod_null_BlindScan_NextFreq,
     .BlindScanWaitCurFreqFinished = MDrv_Demod_null_BlindScan_WaitCurFreqFinished,
     .BlindScanCancel              = MDrv_Demod_null_BlindScan_Cancel,
     .BlindScanEnd                 = MDrv_Demod_null_BlindScan_End,
     .BlindScanGetChannel          = MDrv_Demod_null_BlindScan_GetChannel,
     .BlindScanGetCurrentFreq      = MDrv_Demod_null_BlindScan_GetCurrentFreq,
     .DiSEqCSetTone                = MDrv_Demod_null_DiSEqC_SetTone,
     .DiSEqCSetLNBOut              = MDrv_Demod_null_DiSEqC_SetLNBOut,
     .DiSEqCGetLNBOut              = MDrv_Demod_null_DiSEqC_GetLNBOut,
     .DiSEqCSet22kOnOff            = MDrv_Demod_null_DiSEqC_Set22kOnOff,
     .DiSEqCGet22kOnOff            = MDrv_Demod_null_DiSEqC_Get22kOnOff,
     .DiSEqC_SendCmd               = MDrv_Demod_null_DiSEqC_SendCmd,
     .DiSEqC_GetReply              = MDrv_Demod_null_DiSEqC_GetReply,
     .GetISIDInfo                  = MDrv_Demod_null_GetVCM_ISID_INFO,
     .SetISID                      = MDrv_Demod_null_SetVCM_ISID
#endif
};

#endif // (FRONTEND_DEMOD_TYPE == DEMOD_MSDVBC_51)
#endif

