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
/**********************************************************************
 Copyright (c) 2006-2009 MStar Semiconductor, Inc.
 All rights reserved.

 Unless otherwise stipulated in writing, any and all information contained
 herein regardless in any format shall remain the sole proprietary of
 MStar Semiconductor Inc. and be kept in strict confidence
 (MStar Confidential Information) by the recipient.
 Any unauthorized act including without limitation unauthorized disclosure,
 copying, use, reproduction, sale, distribution, modification, disassembling,
 reverse engineering and compiling of the contents of MStar Confidential
 Information is unlawful and strictly prohibited. MStar hereby reserves the
 rights to any and all damages, losses, costs and expenses resulting therefrom.

**********************************************************************/



#ifndef _MSB1245_H
#define _MSB1245_H

//DVBS
#define MSB1245_DEMOD_WAIT_TIMEOUT    (6000)
#define MSB1245_TUNER_WAIT_TIMEOUT    (50)

#define MSB1245_TS_INV                  1
#define MSB1245_TS_DATA_SWAP            (0)
#define MSB1245_ADCPLL_IQ_SWAP         1
#define COFDMDMD_MUTEX_TIMEOUT       (2000)
#define TIMING_VERIFICATION              0
#define MSB1245_SELECT_IF_INPUT     1  // 0:dvbt_I, 1:dvbs_I
#define MSB1245_SELECT_AGC_PAD_T    0

#define DBG_AGC_OUT  0x03
#define DBG_AGC_ERR  0x05
#define DBG_AGC_MEAN 0x0a

#define AGC_OUT_ROW_MAX  47
#define AGC_MEAN_ROW_MAX 6

#define SIGNAL_LEVEL_VAR_MAX    200 // max variance = -20 dBm
#define NULL_DATA                0

//mailbox
#define REG_MB_CNTL             0x098a //DIG_SWUSE0FL
#define REG_MB_ADDR_L           0x098b //DIG_SWUSE0FH
#define REG_MB_ADDR_H           0x098c //DIG_SWUSE10L
#define REG_MB_DATA             0x098d //DIG_SWUSE10H

//interrupt usage
#define MSB1245_EVT_MASK      0x0F
#define MSB1245_EVT_PORT0INT       (1<<0)
#define MSB1245_EVT_PORT1INT       (1<<1)
#define MSB1245_EVT_PORT2INT       (1<<2)
#define MSB1245_EVT_PORT3INT       (1<<3)

#define MSB1245_EVT_TASK_STACK_SIZE 4096

#define MSB1245_RST_PIN_NOT_SET     9999

#define MSB1245_STATUS_CHK_PERIOD  1000
#define MSB1245_LOCK_TIMEOUT  6000

typedef enum
{
    E_DMD_S2_ZIF_EN = 0x00,
    E_DMD_S2_RF_AGC_EN,
    E_DMD_S2_DCR_EN,
    E_DMD_S2_IQB_EN,
    E_DMD_S2_IIS_EN,
    E_DMD_S2_CCI_EN,
    E_DMD_S2_FORCE_ACI_SELECT,
    E_DMD_S2_IQ_SWAP,               //For DVBS2
    E_DMD_S2_AGC_REF_EXT_0,
    E_DMD_S2_AGC_REF_EXT_1,
    E_DMD_S2_AGC_K,
    E_DMD_S2_ADCI_GAIN,
    E_DMD_S2_ADCQ_GAIN,
    E_DMD_S2_SRD_SIG_SRCH_RNG,
    E_DMD_S2_SRD_DC_EXC_RNG,
    E_DMD_S2_FORCE_CFO_0,            //0FH
    E_DMD_S2_FORCE_CFO_1,
    E_DMD_S2_DECIMATION_NUM,
    E_DMD_S2_PSD_SMTH_TAP,
    E_DMD_S2_CCI_FREQN_0_L,
    E_DMD_S2_CCI_FREQN_0_H,
    E_DMD_S2_CCI_FREQN_1_L,
    E_DMD_S2_CCI_FREQN_1_H,
    E_DMD_S2_CCI_FREQN_2_L,
    E_DMD_S2_CCI_FREQN_2_H,
    E_DMD_S2_TR_LOPF_KP,
    E_DMD_S2_TR_LOPF_KI,
    E_DMD_S2_FINEFE_KI_SWITCH_0,
    E_DMD_S2_FINEFE_KI_SWITCH_1,
    E_DMD_S2_FINEFE_KI_SWITCH_2,
    E_DMD_S2_FINEFE_KI_SWITCH_3,
    E_DMD_S2_FINEFE_KI_SWITCH_4,       //1FH
    E_DMD_S2_PR_KP_SWITCH_0,
    E_DMD_S2_PR_KP_SWITCH_1,
    E_DMD_S2_PR_KP_SWITCH_2,
    E_DMD_S2_PR_KP_SWITCH_3,
    E_DMD_S2_PR_KP_SWITCH_4,
    E_DMD_S2_FS_GAMMA,
    E_DMD_S2_FS_ALPHA0,
    E_DMD_S2_FS_ALPHA1,
    E_DMD_S2_FS_ALPHA2,
    E_DMD_S2_FS_ALPHA3,
    E_DMD_S2_FS_H_MODE_SEL,
    E_DMD_S2_FS_OBSWIN,
    E_DMD_S2_FS_PEAK_DET_TH_L,
    E_DMD_S2_FS_PEAK_DET_TH_H,
    E_DMD_S2_FS_CONFIRM_NUM,
    E_DMD_S2_EQ_MU_FFE_DA,                //2FH
    E_DMD_S2_EQ_MU_FFE_DD,
    E_DMD_S2_EQ_ALPHA_SNR_DA,
    E_DMD_S2_EQ_ALPHA_SNR_DD,
    E_DMD_S2_FEC_ALFA,                    //For DVBS2
    E_DMD_S2_FEC_BETA,                    //For DVBS2
    E_DMD_S2_FEC_SCALING_LLR,            //For DVBS2
    E_DMD_S2_TS_SERIAL,
    E_DMD_S2_TS_CLK_RATE,
    E_DMD_S2_TS_OUT_INV,
    E_DMD_S2_TS_DATA_SWAP,
    //------------------------------------------
    E_DMD_S2_FW_VERSION_L,            //0x3A
    E_DMD_S2_FW_VERSION_H,            //0x3B
    E_DMD_S2_CHIP_VERSION,
    E_DMD_S2_FS_L,            //Frequency
    E_DMD_S2_FS_H,            //Frequency
    E_DMD_S2_MANUAL_TUNE_SYMBOLRATE_L, //0x3F
    E_DMD_S2_MANUAL_TUNE_SYMBOLRATE_H, //
    E_DMD_S2_SYSTEM_TYPE,      //DVBS/S2
    E_DMD_S2_MODULATION_TYPE,  //QPSK/8PSK
    E_DMD_S2_CODERATE,         //1/2
    
    E_DMD_S2_UNCRT_PKT_NUM_7_0,
    E_DMD_S2_UNCRT_PKT_NUM_8_15,
    E_DMD_S2_STATE_FLAG,
    E_DMD_S2_SUBSTATE_FLAG,
    E_DMD_S2_HUM_DETECT_FLAG,
    E_DMD_S2_CCI_DETECT_FLAG,
    E_DMD_S2_IIS_DETECT_FLAG,
    E_DMD_S2_OPEN_HUM_VLD_IRQ_FLAG,
    E_DMD_S2_SRD_COARSE_DONE_FLAG,
    E_DMD_S2_SRD_FINE_DONE_FLAG,
    E_DMD_S2_FINEFE_DONE_FLAG,
    E_DMD_S2_REV_FRAME_FLAG,    //0x4F
    E_DMD_S2_DUMMY_FRAME_FLAG,
    E_DMD_S2_PLSC_DONE_FLAG,
    E_DMD_S2_GET_INFO_FROM_FRAME_LENGTH_DONE_FLAG,
    E_DMD_S2_IQ_SWAP_DETECT_FLAG,
    E_DMD_S2_FRAME_ACQUISITION_DONE_FLAG,
    E_DMD_S2_OLCFE_DONE_FLAG,
    E_DMD_S2_FSYNC_FOUND_FLAG,
    E_DMD_S2_FSYNC_FAIL_SEARCH_FLAG,
    E_DMD_S2_FALSE_ALARM_FLAG,
    E_DMD_S2_VITERBI_IN_SYNC_FLAG,
    E_DMD_S2_INT_CODE_RATE_SEARCH_FAIL_FLAG,
    E_DMD_S2_VITERBI_INT_PRE_FLAG,
    E_DMD_S2_BER_WINDOW_END_FLAG,
    E_DMD_S2_PASS_WRONG_INT_FLAG,
    E_DMD_S2_CLK_CNT_OVER_FLAG,
    E_DMD_S2_UNCRT_OVER_FLAG,    //0x5F
    E_DMD_S2_DISEQC_RX_LENGTH,
    E_DMD_S2_DISEQC_INTERRUPT_FLAG,
    E_DMD_S2_DISEQC_RX_FLAG,
    E_DMD_S2_DISEQC_INTERRUPT_STATUS,
    E_DMD_S2_DISEQC_STATUS_FLAG,
    E_DMD_S2_ACI_FIR_SELECTED,    //0x65
    //LOCK
    E_DMD_S2_AGC_LOCK_FLAG,
    E_DMD_S2_DCR_LOCK_FLAG,
    E_DMD_S2_DAGC0_LOCK_FLAG,
    E_DMD_S2_DAGC1_LOCK_FLAG,
    E_DMD_S2_DAGC2_LOCK_FLAG,
    E_DMD_S2_DAGC3_LOCK_FLAG,
    E_DMD_S2_TR_LOCK_FLAG,
    E_DMD_S2_CLCFE_LOCK_FLAG,
    E_DMD_S2_EQ_LOCK_FLAG,
    E_DMD_S2_PR_LOCK_FLAG,    //0x6F
    E_DMD_S2_FSYNC_LOCK_FLAG,
    E_DMD_S2_FSYNC_FAIL_LOCK_FLAG,

    E_DMD_S2_MB_SWUSE12L,    //0x72
    E_DMD_S2_MB_SWUSE12H,
    E_DMD_S2_MB_SWUSE13L,
    E_DMD_S2_MB_SWUSE13H,
    E_DMD_S2_MB_SWUSE14L,
    E_DMD_S2_MB_SWUSE14H,
    E_DMD_S2_MB_SWUSE15L,
    E_DMD_S2_MB_SWUSE15H,
    E_DMD_S2_MB_SWUSE16L,
    E_DMD_S2_MB_SWUSE16H,
    E_DMD_S2_MB_SWUSE17L,
    E_DMD_S2_MB_SWUSE17H,
    E_DMD_S2_MB_SWUSE18L,
    E_DMD_S2_MB_SWUSE18H, //0x7F
    E_DMD_S2_MB_SWUSE19L,
    E_DMD_S2_MB_SWUSE19H,
    E_DMD_S2_MB_SWUSE1AL,
    E_DMD_S2_MB_SWUSE1AH,
    E_DMD_S2_MB_SWUSE1BL,
    E_DMD_S2_MB_SWUSE1BH,
    E_DMD_S2_MB_SWUSE1CL,
    E_DMD_S2_MB_SWUSE1CH,
    E_DMD_S2_MB_SWUSE1DL,
    E_DMD_S2_MB_SWUSE1DH,
    E_DMD_S2_MB_SWUSE1EL,
    E_DMD_S2_MB_SWUSE1EH,
    E_DMD_S2_MB_SWUSE1FL,
    E_DMD_S2_MB_SWUSE1FH, //0x8D
    
    E_DMD_S2_MB_DMDTOP_DBG_0,
    E_DMD_S2_MB_DMDTOP_DBG_1,  //0x8F
    E_DMD_S2_MB_DMDTOP_DBG_2,
    E_DMD_S2_MB_DMDTOP_DBG_3,
    E_DMD_S2_MB_DMDTOP_DBG_4,
    E_DMD_S2_MB_DMDTOP_DBG_5,
    E_DMD_S2_MB_DMDTOP_DBG_6,
    E_DMD_S2_MB_DMDTOP_DBG_7,
    E_DMD_S2_MB_DMDTOP_DBG_8,
    E_DMD_S2_MB_DMDTOP_DBG_9,
    E_DMD_S2_MB_DMDTOP_DBG_A,
    E_DMD_S2_MB_DMDTOP_DBG_B,
    
    E_DMD_S2_MB_DMDTOP_SWUSE00L,
    E_DMD_S2_MB_DMDTOP_SWUSE00H,
    E_DMD_S2_MB_DMDTOP_SWUSE01L,
    E_DMD_S2_MB_DMDTOP_SWUSE01H,
    E_DMD_S2_MB_DMDTOP_SWUSE02L,
    E_DMD_S2_MB_DMDTOP_SWUSE02H,   //0x9F
    E_DMD_S2_MB_DMDTOP_SWUSE03L,
    E_DMD_S2_MB_DMDTOP_SWUSE03H,
    E_DMD_S2_MB_DMDTOP_SWUSE04L,
    E_DMD_S2_MB_DMDTOP_SWUSE04H,
    E_DMD_S2_MB_DMDTOP_SWUSE05L,
    E_DMD_S2_MB_DMDTOP_SWUSE05H,
    E_DMD_S2_MB_DMDTOP_SWUSE06L,
    E_DMD_S2_MB_DMDTOP_SWUSE06H,
    E_DMD_S2_MB_DMDTOP_SWUSE07L,
    E_DMD_S2_MB_DMDTOP_SWUSE07H,
    
    E_DMD_S2_MB_TOP_WR_DBG_90,
    E_DMD_S2_MB_TOP_WR_DBG_91,
    E_DMD_S2_MB_TOP_WR_DBG_92,
    E_DMD_S2_MB_TOP_WR_DBG_93,
    E_DMD_S2_MB_TOP_WR_DBG_94,
    E_DMD_S2_MB_TOP_WR_DBG_95,    //0xAF
    E_DMD_S2_MB_TOP_WR_DBG_96,
    E_DMD_S2_MB_TOP_WR_DBG_97,
    E_DMD_S2_MB_TOP_WR_DBG_98,
    E_DMD_S2_MB_TOP_WR_DBG_99,
    
    E_DMD_S2_MB_DUMMY_REG_0,
    E_DMD_S2_MB_DUMMY_REG_1,
    E_DMD_S2_MB_DUMMY_REG_2,
    E_DMD_S2_MB_DUMMY_REG_3,
    E_DMD_S2_MB_DUMMY_REG_4,
    E_DMD_S2_MB_DUMMY_REG_5,
    E_DMD_S2_MB_DUMMY_REG_6,
    E_DMD_S2_MB_DUMMY_REG_7,
    E_DMD_S2_MB_DUMMY_REG_8,
    E_DMD_S2_MB_DUMMY_REG_9,
    E_DMD_S2_MB_DUMMY_REG_A,
    E_DMD_S2_MB_DUMMY_REG_B,     //0xBF
    E_DMD_S2_MB_DUMMY_REG_C,
    E_DMD_S2_MB_DUMMY_REG_D,
    E_DMD_S2_MB_DUMMY_REG_E,
    E_DMD_S2_MB_DUMMY_REG_F,
    E_DMD_S2_MB_DUMMY_REG_10,
    E_DMD_S2_MB_DUMMY_REG_11,
    
    E_DMD_S2_MB_DMDTOP_INFO_01,
    E_DMD_S2_MB_DMDTOP_INFO_02,
    E_DMD_S2_MB_DMDTOP_INFO_03,
    E_DMD_S2_MB_DMDTOP_INFO_04,
    E_DMD_S2_MB_DMDTOP_INFO_05,
    E_DMD_S2_MB_DMDTOP_INFO_06,
    E_DMD_S2_MB_DMDTOP_INFO_07,
    E_DMD_S2_MB_DMDTOP_INFO_08,
    
    E_DMD_S2_IDLE_STATE_UPDATED,
    E_DMD_S2_LOG_FLAG,          //0xCF
    E_DMD_S2_LOG_SKIP_INDEX,
    E_DMD_S2_LOCK_COUNT,
    E_DMD_S2_NARROW_STEP_FLAG,
    E_DMD_S2_UNCORRECT_PKT_COUNT,
    E_DMD_S2_DISEQC_INIT_MODE,
    E_DMD_S2_DECIMATE_FORCED,
    E_DMD_S2_SRD_MAX_SRG_FLAG,
    E_DMD_S2_DVBS_OUTER_RETRY,
    
    E_DMD_S2_FORCED_DECIMATE_FLAG,
    E_DMD_S2_NO_SIGNAL_FLAG,
    E_DMD_S2_SPECTRUM_TRACK_FLAG,
    E_DMD_S2_SRD_LOCAL_SEARCH_FLAG,
    E_DMD_S2_NO_SIGNAL_RATIO_CHECK_FLAG,
    E_DMD_S2_LOW_SR_ACI_FLAG,
    E_DMD_S2_SPECTRUM_TRACKER_TIMEOUT,
    E_DMD_S2_TR_TIMEOUT,         //0xDF
    E_DMD_S2_BALANCE_TRACK,
    E_DMD_S2_GAIN_TILT_FLAG,    //0xE1
    
    DVBS2_PARAM_LEN
} DVBS_Param_2;



typedef struct
{
    MS_BOOL    bInited;
    MS_BOOL    bOpen;
    MS_S32     s32_MSB1245_Mutex;
    MS_U8      u8sramCode;
    MS_U32     u32CurrFreq;
    MS_BOOL    bFECLock;
    MS_U8      u8ScanStatus;
    DEMOD_MS_INIT_PARAM  MSB1245_InitParam;
    MS_U8     u8SlaveID;
    MS_BOOL   bIsDVBS2;
    MS_S32    s32DemodHandle;
    MS_U32    u32DmxInputPath;
    MS_BOOL   bIsMCP_DMD;
    MS_BOOL   bDiSeqc_Tx22K_Off;
} MDvr_CofdmDmd_CONFIG;

#if(TIMING_VERIFICATION == 1)
static MS_U32 tmm_1 = 0x00;
static MS_U32 tmm_2 = 0x00;
static MS_U32 tmm_3 = 0x00;
static MS_U32 tmm_4 = 0x00;
static MS_U32 tmm_5 = 0x00;
static MS_U32 tmm_6 = 0x00;
static MS_U32 tmm_7 = 0x00;
static MS_U32 tmm_8 = 0x00;
static MS_U32 tmm_9 = 0x00;
static MS_U32 tmm_10 = 0x00;
static MS_U32 tmm_11 = 0x00;
static MS_U32 tmm_12 = 0x00;
static MS_U32 tmm_13 = 0x00;
static MS_U32 tmm_14 = 0x00;
static MS_U32 tmm_15 = 0x00;
static MS_U32 tmm_16 = 0x00;
static MS_U32 tmm_17 = 0x00;
static MS_U32 tmm_18 = 0x00;
static MS_U32 tmm_19 = 0x00;
static MS_U32 tmm_20 = 0x00;

#define GIVE_ME_TIME MsOS_GetSystemTime();
#endif

// power level
#if IF_THIS_TUNER_INUSE(TUNER_AV2012) || IF_THIS_TUNER_INUSE(TUNER_AV2011)
static const MS_U16 _u16SignalLevel_AV2012[AGC_OUT_ROW_MAX][2]=
{
    {  255,      920},
    {  255,      900},
    {  255,      880},
    {  255,      860},
    {  255,      840},
    {  6076,     820},
    {  29526,    800},
    {  30032 ,   780},
    {  30868 ,   760},
    {  31418 ,   740},
    {  32021 ,   720},
    {  32413 ,   700},
    {  32617 ,   680},
    {  32733 ,   660},
    {  33057 ,   640},
    {  33431 ,   620},
    {  34080 ,   600},
    {  34418 ,   580},
    {  34350 ,   560},
    {  34757 ,   540},
    {  34919 ,   520},
    {  35389 ,   500},
    {  36051 ,   480},
    {  36575 ,   460},
    {  36959 ,   440},
    {  37396 ,   420},
    {  37578 ,   400},
    {  37850 ,   380},
    {  38084 ,   360},
    {  38317 ,   340},
    {  38585 ,   320},
    {  38881 ,   300},
    {  39149 ,   280},
    {  39433 ,   260},
    {  39678 ,   240},
    {  39927 ,   220},
    {  40166 ,   200},
    {  40426 ,   180},
    {  40670 ,   160},
    {  40911 ,   140},
    {  41189 ,   120},
    {  41465 ,   100},
    {  41710 ,    80},
    {  41982 ,    60},
    {  42302 ,    40},
    {  42504 ,    20},
    {  42619 ,    0},
};
#endif

#if IF_THIS_TUNER_INUSE(TUNER_AV2018)
static const MS_U16 _u16SignalLevel_AV2018[AGC_OUT_ROW_MAX][2]=
{
    {  255,  922},
    {  255,  902},
    {  255,  882},
    {  255,  862},
    {  255,  842},
    {  255,  822},
    {  255,  802},
    {  255,  782},
    {  255,  762},
    { 3900,  742},
    { 6143,  722},
    { 7935,  702},
    { 9215,  682},
    { 9727,  662},
    {11263,  642},
    {12543,  622},
    {13823,  602},
    {15103,  582},
    {16127,  562},
    {16895,  542},
    {17919,  522},
    {19455,  502},
    {21119,  482},
    {23295,  462},
    {23551,  442},
    {25599,  422},
    {27647,  402},
    {28415,  382},
    {29183,  362},
    {30463,  342},
    {30975,  322},
    {32255,  302},
    {32752,  282},
    {34271,  262},
    {35551,  242},
    {36655,  222},
    {37663,  202},
    {38511,  182},
    {39391,  162},
    {40164,  142},
    {40847,  122},
    {41487,  102},
    {42079,  82},
    {42607,  62},
    {43103,  42},
    {43567,  22},
    {43807,  12}
};
#endif

#if IF_THIS_TUNER_INUSE(TUNER_RDA5815M)
static const MS_U16 _u16SignalLevel_RDA5815M[AGC_OUT_ROW_MAX][2]=
{
    {  255,  922},
    {  255,  902},
    {  255,  882},
    {  255,  862},
    {  255,  842},
    {  255,  822},
    {  255,  802},
    {  255,  782},
    {  255,  762},
    {  255,  742},
    {  767,  722},
    { 1279,  702},
    { 2047,  682},
    { 3583,  662},
    { 5119,  642},
    { 6655,  622},
    { 8703,  602},
    {11007,  582},
    {12799,  562},
    {14079,  542},
    {15103,  522},
    {16895,  502},
    {18687,  482},
    {21247,  462},
    {22015,  442},
    {23295,  422},
    {24831,  402},
    {27135,  382},
    {29695,  362},
    {31231,  342},
    {32255,  322},
    {34047,  302},
    {35839,  282},
    {37375,  262},
    {39167,  242},
    {40703,  222},
    {41983,  202},
    {43263,  182},
    {44543,  162},
    {46079,  142},
    {47359,  122},
    {48639,  102},
    {49919,   82},
    {51711,   62},
    {53247,   42},
    {54271,   22},
    {54783,   12}
};
#endif


#if IF_THIS_TUNER_INUSE(TUNER_RT710)
static const MS_S16 _u16AGCErrorMeanNegative_RT710[AGC_MEAN_ROW_MAX][2]=
{
    {-32000,    0},{-30000,    10},{-21000,    20},{-13000,    30},{-6000,    40},{-800,    50},
};

static const MS_S16 _u16AGCErrorMeanPositive_RT710[AGC_MEAN_ROW_MAX][2]=
{
    {6000,    840},{10000,    850},{14500,    860},{18000,    870},{21000,    880},{24000,    890}
};

MS_U16  _u16SignalLevel_RT710[AGC_OUT_ROW_MAX][2]=
{
    //{NULL_DATA,NULL_DATA},
    //{NULL_DATA,NULL_DATA},
    {NULL_DATA,NULL_DATA},
    {NULL_DATA,NULL_DATA},
    {NULL_DATA,NULL_DATA},
    {NULL_DATA,NULL_DATA},
    {NULL_DATA,NULL_DATA},
    {NULL_DATA,NULL_DATA},
    {NULL_DATA,NULL_DATA},
    {NULL_DATA,NULL_DATA},
    {NULL_DATA,NULL_DATA},
    {NULL_DATA,NULL_DATA},
    {27535,    830},
    {28455,    820},
    {29105,    810},
    {29565,    800},
    {29935,    790},
    {30235,    780},
    {30520,    770},
    {30785,    760},
    {31015,    750},
    {31220,    740},
    {31410,    730},
    {31595,    720},
    {31785,    710},
    {31965,    700},
    {32155,    690},
    {32395,    680},
    {32785,    670},
    {33335,    660},
    {33955,    650},
    {34505,    640},
    {34915,    630},
    {35315,    620},
    {35675,    610},
    {35995,    600},
    {36295,    590},
    {36615,    580},
    //{36815,    570}, //In tuner avtive area
    {36835,    160},
    //{37035,    560},
    {37165,    150},
    //{37225,    550},
    {37350,    140},
    {37535,    130},
    {37725,    120},
    {37895,    110},
    {38085,    100},
    {38265,     90},
    {38485,     80},
    {38735,     70},
    {39235,     60}
};

#endif
DRV_DEMOD_TABLE_TYPE GET_DEMOD_ENTRY_NODE(DEMOD_MSB1245) DDI_DRV_TABLE_ENTRY(demodtab);
MS_BOOL MSB1245_I2C_CH_Reset(MS_U8 u8DemodIndex, MS_U8 ch_num);
MS_BOOL MSB1245_WriteReg(MDvr_CofdmDmd_CONFIG *pMSB1245, MS_U16 u16Addr, MS_U8 u8Data);
MS_BOOL MSB1245_ReadReg(MDvr_CofdmDmd_CONFIG *pMSB1245 ,MS_U16 u16Addr, MS_U8 *pu8Data);
MS_BOOL MSB1245_Demod_GetLock(MS_U8 u8DemodIndex, EN_LOCK_STATUS *peLockStatus);
#endif
