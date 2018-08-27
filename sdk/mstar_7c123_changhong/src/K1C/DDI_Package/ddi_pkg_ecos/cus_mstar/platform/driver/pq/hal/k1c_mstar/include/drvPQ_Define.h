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

#ifndef _DRVPQ_DEFINE_H_
#define _DRVPQ_DEFINE_H_

#define PQ_ONLY_SUPPORT_BIN 0
#define PQ_ENABLE_DEBUG     0
#define PQ_ENABLE_CHECK     0
#ifndef ENABLE_PQ_BIN
#define ENABLE_PQ_BIN       0
#endif
#define ENABLE_PQ_MLOAD         1

#define PQ_ENABLE_VD_SAMPLING   0
#define PQ_ENABLE_PIP           0
#define PQ_ENABLE_3D_VIDEO      0
#define PQ_ENABLE_IOCTL         1
#define PQ_QM_CVBS              1
#define PQ_QM_YPBPR             1
#define PQ_QM_PC                1
#define PQ_QM_HMDI              1
#define PQ_QM_HDMI_PC           1
#define PQ_QM_DTV               1
#define PQ_QM_MM_VIDEO          1
#define PQ_QM_MM_PHOTO          1
#define PQ_QM_3D                0
#define PQ_QM_4K2K              0
#define PQ_QM_VE_HVSP           1
#define PQ_SKIPRULE_ENABLE      1
#define PQ_GRULE_NR_ENABLE      1
#define PQ_GRULE_MPEG_NR_ENABLE 0
#define PQ_GRULE_OSD_BW_ENABLE  0
#define PQ_GRULE_STB_PQ_ENABLE  1
#define PQ_XRULE_DB_ENABLE      0
#define PQ_XRULE_SCALINGUP_ENABLE      1

//if Up-samping is controlled by QMAP
#define PQ_ENABLE_420UPSAMPLING             0

#define ESTIMATE_AVAILABLE_RATE 85

#define U8 MS_U8
#define code

extern MS_S32 _PQ_Mutex ;

#if defined (MSOS_TYPE_LINUX)
extern pthread_mutex_t _PQ_MLoad_Mutex;
#endif

#if PQ_ENABLE_DEBUG
extern MS_BOOL _u16DbgSwitch;
#define PQ_DBG(x)  \
do{                \
    if (_u16DbgSwitch){ \
        x;         \
    }              \
}while(0)

#define PQ_DUMP_DBG(x) x
#define PQ_DUMP_FILTER_DBG(x) x
#else
#define PQ_DBG(x) //x
#define PQ_DUMP_DBG(x)          //x
#define PQ_DUMP_FILTER_DBG(x)   //x
#endif

#define PQ_MAP_REG(reg) (((reg)>>8)&0xFF), ((reg)&0xFF)
#define REG_ADDR_SIZE   2
#define REG_MASK_SIZE   1

#define PQ_IP_NULL          0xFF

#define PQTBL_EX            0x10
#define PQTBL_NORMAL        0x11

#define E_XRULE_HSD         0
#define E_XRULE_VSD         1
#define E_XRULE_HSP         2
#define E_XRULE_VSP         3
#define E_XRULE_CSC         4

#if PQ_XRULE_DB_ENABLE
#define E_XRULE_DB_NTSC     5
#define E_XRULE_DB_PAL      6
#define E_XRULE_NUM         7
#else
#define E_XRULE_NUM         5
#endif

#define E_GRULE_NR          0
#define E_GRULE_STB_PQ      1
#define E_GRULE_NUM         2

#define _END_OF_TBL_        0xFFFF


typedef enum {
    PQ_TABTYPE_GENERAL,
    PQ_TABTYPE_COMB,
    PQ_TABTYPE_SCALER,
    PQ_TABTYPE_SRAM1,
    PQ_TABTYPE_SRAM2,
    PQ_TABTYPE_SRAM3,
    PQ_TABTYPE_SRAM4,
    PQ_TABTYPE_C_SRAM1,
    PQ_TABTYPE_C_SRAM2,
    PQ_TABTYPE_C_SRAM3,
    PQ_TABTYPE_C_SRAM4,
    PQ_TABTYPE_SRAM_COLOR_INDEX,
    PQ_TABTYPE_SRAM_COLOR_GAIN_SNR,
    PQ_TABTYPE_SRAM_COLOR_GAIN_DNR,
    PQ_TABTYPE_VE_HSD_SRAM,
    PQ_TABTYPE_VE_VSD_SRAM,
    PQ_TABTYPE_VE_HSP_SRAM,
    PQ_TABTYPE_VE_VSP_SRAM,
    PQ_TABTYPE_SWDRIVER,
} EN_PQ_TABTYPE;



typedef enum
{
    PQ_MD_720x480_60I,      // 00
    PQ_MD_720x480_60P,      // 01
    PQ_MD_720x576_50I,      // 02
    PQ_MD_720x576_50P,      // 03
    PQ_MD_1280x720_50P,     // 04
    PQ_MD_1280x720_60P,     // 05
    PQ_MD_1920x1080_50I,    // 06
    PQ_MD_1920x1080_60I,    // 07
    PQ_MD_1920x1080_24P,    // 08
    PQ_MD_1920x1080_30P,    // 09
    PQ_MD_1920x1080_50P,    // 10
    PQ_MD_1920x1080_60P,    // 11
    PQ_MD_Num,
}PQ_MODE_INDEX;


typedef struct
{
    MS_U8 *pIPCommTable;
    MS_U8 *pIPTable;
    MS_U8 u8TabNums;
    MS_U8 u8TabType;
} EN_IPTAB_INFO;

typedef struct
{
    MS_U8 *pIPTable;
    MS_U8 u8TabNums;
    MS_U8 u8TabType;
    MS_U8 u8TabIdx;
} EN_IP_Info;

enum
{
    PQ_FUNC_DUMP_REG,
    PQ_FUNC_CHK_REG,
};

#define PQ_MUX_DEBUG    0

#if(PQ_MUX_DEBUG)
// store bank
#define SC_BK_STORE     \
        MS_U8 u8Bank;      \
        if(!MsOS_ObtainMutex(_PQ_Mutex, MSOS_WAIT_FOREVER))                     \
        {                                                                        \
            printf("==========================\n");                              \
            printf("[%s][%s][%06d] Mutex taking timeout\n",__FILE__,__FUNCTION__,__LINE__);    \
        }        \
        u8Bank = MApi_XC_ReadByte(BK_SCALER_BASE)

// restore bank
#define SC_BK_RESTORE   \
        MApi_XC_WriteByte(BK_SCALER_BASE, u8Bank); \
        MsOS_ReleaseMutex(_PQ_Mutex);
#else
#define SC_BK_STORE     \
        MS_U8 u8Bank;      \
        if(!MsOS_ObtainMutex(_PQ_Mutex, MSOS_WAIT_FOREVER))                     \
        {                                                                        \
        }        \
        u8Bank = MApi_XC_ReadByte(BK_SCALER_BASE)

// restore bank
#define SC_BK_RESTORE   \
        MApi_XC_WriteByte(BK_SCALER_BASE, u8Bank); \
        MsOS_ReleaseMutex(_PQ_Mutex);
#endif

// switch bank
#define SC_BK_SWITCH(_x_)\
        MApi_XC_WriteByte(BK_SCALER_BASE, _x_)

#define SC_BK_CURRENT   \
        MApi_XC_ReadByte(BK_SCALER_BASE)



// store bank
#define COMB_BK_STORE     \
        MS_U8 u8Bank;      \
        u8Bank = MApi_XC_ReadByte(COMB_REG_BASE)

// restore bank
#define COMB_BK_RESTORE   \
        MApi_XC_WriteByte(COMB_REG_BASE, u8Bank)

// switch bank
#define COMB_BK_SWITCH(_x_)\
        MApi_XC_WriteByte(COMB_REG_BASE, _x_)

#define COMB_BK_CURRENT   \
        MApi_XC_ReadByte(COMB_REG_BASE)

#define SC_BK_SWDRIVER  0x3F

#define SW_REG_NUM  0x80
extern MS_U8 SWReg[SW_REG_NUM];
#define SWReg_ReadByte( u16Reg )  SWReg[u16Reg]

#define SWReg_WriteByteMask( u16Reg, u8Value, u8Mask )    \
    (SWReg[u16Reg] = (SWReg[u16Reg] & ~(u8Mask)) | ((u8Value) & (u8Mask)))

#define PQ_IP_COMM  0xfe
#define PQ_IP_ALL   0xff


typedef struct {
  MS_U8 u8PQ_InputType_Num;
  MS_U8 u8PQ_IP_Num;
  MS_U8 *pQuality_Map_Aray;
  EN_IPTAB_INFO *pIPTAB_Info;
  MS_U8 *pSkipRuleIP;

  MS_U8 u8PQ_XRule_IP_Num[E_XRULE_NUM];
  MS_U8 *pXRule_IP_Index[E_XRULE_NUM];
  MS_U8 *pXRule_Array[E_XRULE_NUM];

  MS_U8 u8PQ_GRule_Num[E_GRULE_NUM];
  MS_U8 u8PQ_GRule_IPNum[E_GRULE_NUM];
  MS_U8 u8PQ_GRule_LevelNum[E_GRULE_NUM];
  MS_U8 *pGRule_IP_Index[E_GRULE_NUM];
  MS_U8 *pGRule_Array[E_GRULE_NUM];
  MS_U8 *pGRule_Level[E_GRULE_NUM];
} PQTABLE_INFO;

#endif /* _DRVPQ_DEFINE_H_ */

