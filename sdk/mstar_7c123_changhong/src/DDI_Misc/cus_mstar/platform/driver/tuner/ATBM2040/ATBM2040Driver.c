/********************************************************************************************************
(C) Copyright 2016, AltoBeam, Inc. All rights reserved.
File:ATBM2040Driver.c
Description:
    ATBM2040 Tuner internal implement.

History:

*********************************************************************************************************/
#include "ATBM2040Api.h"
#include "ATBM2040Driver.h"
#include "ATBM2040Porting.h"
#include "ATBM2040PrivData.h"
#if ATBM2040_DEBUG_PRINT
#include <stdio.h>
#endif
#include "Board.h"

#if IF_THIS_TUNER_INUSE(TUNER_ATBM2040)

static UINT8 ATBM2040Ver = 103;
static UINT8 ATBM2040ChipID = 0;
static ATBM2040Reg_t BGR[] =
{
    {0x0C, 0x0D, 0x01}, /* bg_lna_100u_pdn */
    {0x0C, 0x17, 0x01}, /* bg_uhf_60u_pdn */
    {0x0C, 0x21, 0x01}, /* bg_rssi_100u_pdn */
    {0x0C, 0x23, 0x01}, /* bg_vhf_80u_src_pdn */
    {0x0C, 0x25, 0x01}, /* bg_vhf_100u_sink_pdn */
    {0x0C, 0x27, 0x01}, /* bg_uhf_80u_pdn */
    {0x0C, 0x29, 0x01}, /* bg_uhf_90u_pdn */
    {0x0C, 0x2B, 0x01}, /* bg_mixr_20u_pdn */
    {0x0C, 0x2D, 0x01}, /* bg_mixr_50u_pdn */
    {0x0C, 0x2F, 0x01}, /* bg_rf_12ldo_50u_pdn */

};/*Vensus register setting.docx*/
#define ATBM2040_BGR_CNT ATBM2040_ARRAY_NUM(BGR)


static ATBM2040Reg_t LnaAttenReg[] =
{
    {0x04, 0x00, 0x00}, /*lna_pd */
    {0x04, 0x01, 0x00}, /* [0]       0   lna_ldo_pd */
};
#define ATBM2040_ATTEN_CNT ATBM2040_ARRAY_NUM(LnaAttenReg)

static ATBM2040NLOMixer_t NLOMixer[] =
{
    /* frequency range    Mixer    NLO */
    {0,         60499,      8,      7},
    {60500,     68499,      8,      6},
    {68500,     70499,      8,      6},
    {70500,     118499,     8,      4},
    {118500,    150499,     8,      3},
    {150500,    246499,     8,      2},
    {246500,    333499,     8,      1},
    {333500,    400000,     8,      1},
    {400001,    500499,     4,      2},
    {500500,    1200499,    4,      1},
};/*Vensus_DivRatio.xls*/
#define ATBM2040_NLO_MIXER_CNT ATBM2040_ARRAY_NUM(NLOMixer)

static ATBM2040NLOMixer_t NLOMixerLeoEHMD[] =
{
    /* frequency range    Mixer    NLO */
    {     0,      49999,     8,     8},
    { 50000,      55999,     8,     5},
    { 56000,      60999,     8,     6},
    { 61000,      65499,     8,     4},
    { 65500,      70999,     8,     6},
    { 71000,     112999,     8,     4},
    {113000,     150999,     8,     3},
    {151000,     235499,     8,     2},
    {235500,     400000,     8,     1},
    {400001,     468999,     4,     2},
    {469000,    1200000,     4,     1},
};/*Vensus_DivRatio.xls*/
#define ATBM2040_NLO_MIXER_LEOE_HMD_CNT ATBM2040_ARRAY_NUM(NLOMixerLeoEHMD)

static ATBM2040NLOMixer_t NLOMixerLeoELMD[] =
{
    /* frequency range    Mixer    NLO */
    { 40000,      44999,    8,  8 },
    { 45000,      49999,    8,  10},
    { 50000,      52999,    8,  6 },
    { 53000,      55999,    8,  5 },
    { 56000,      57999,    8,  6 },
    { 58000,      65499,    8,  5 },
    { 65500,      70999,    8,  6 },
    { 71000,     112999,    8,  4 },
    {113000,     150999,    8,  3 },
    {151000,     235499,    8,  2 },
    {235500,     400000,    8,  1 },
    {400001,     468999,    4,  2 },
    {469000,    1200000,    4,  1 },
};/*Vensus_DivRatio.xls*/
#define ATBM2040_NLO_MIXER_LEOE_LMD_CNT ATBM2040_ARRAY_NUM(NLOMixerLeoELMD)

static ATBM2040Reg_t PLLSettingReg[] =
{
    {0x0F, 0x0F, 0x00}, /* pll_kvco[3:0] */
    {0x0E, 0x0C, 0x01}, /* osc_pll_en */
    {0x0F, 0x03, 0x00}, /* pll_pd_ldo_pfd */
    {0x0F, 0x09, 0x00}, /* pll_pd_ldo_cp */
    {0x0F, 0x12, 0x00}, /* pll_pd_ldo_vco */
    {0x0F, 0x15, 0x00}, /* pll_pd_ldo_mmd */
    {0x0F, 0x30, 0x00}, /* pll_rn_mmd */
    {0x0F, 0x30, 0x01}, /* pll_rn_mmd */
    {0x0F, 0x16, 0x00}, /* pll_pd_ldo_dig */
    {0x0F, 0x01, 0x00}, /* pll_en_pfd */
    {0x0F, 0x0A, 0x00}, /* pll_pd_cp */
    {0x0F, 0x0B, 0x00}, /* pll_pd_fastload */
    {0x0F, 0x13, 0x00}, /* pll_pd_vco */
    {0x0F, 0x14, 0x00}, /* pll_pd_sx */
    {0x12, 0x09, 0x00}, /* pll_ld_pd */
    {0x12, 0x07, 0x01}, /* pll_ld_rst */
    {0x12, 0x08, 0x01}, /* pll_ld_sel */
    {0x0F, 0x0E, 0x00}, /* pll_test_open */

};
#define ATBM2040_PLL_SETTING_REG_CNT ATBM2040_ARRAY_NUM(PLLSettingReg)

static ATBM2040ADCDivider_t ADCDividerHMD[] =
{
    {/*40000 */ 0 ,      46000,     7 },
    {46000,          53500,     8 },
    {53500,          60500,     9 },
    {60500,          62000,     8 },
    {62000,          70500,     9 },
    {70500,          74000,     6 },
    {74000,          87500,     7 },
    {87500,          98500,     8 },
    {98500,         107000,     9 },
    {107000,        118500,     10},
    {118500,        129500,     8 },
    {129500,        144500,     9 },
    {144500,        150500,     10},
    {150500,        152000,     6 },
    {152000,        174500,     7 },
    {174500,        197000,     8 },
    {197000,        219500,     9 },
    {219500,        246500,     10},
    {246500,        264500,     5 },
    {264500,        309500,     6 },
    {309500,        357000,     7 },
    {357000,        405000,     8 },
    {405000,        468500,     9 },
    {468500,        500500,     10},
    {500500,        534500,     5 },
    {534500,        638000,     6 },
    {638000,        732500,     7 },
    {732500,        857500,     8 },
    {857500,        962000,     9 },
    {962000,        1200000,    10}
};/*Fs_setting.xlsx*/
#define ATBM2040_ADC_DIVIDER_HMD_CNT ATBM2040_ARRAY_NUM(ADCDividerHMD)

static ATBM2040ADCDivider_t ADCDividerLMD[] =
{
    {/*40000 */0, 43500, 5 },
    {43500  ,   50500,  6 },
    {50500  ,   58500,  7 },
    {58500  ,   60500,  8 },
    {60500  ,   65000,  7 },
    {65000  ,   70500,  8 },
    {70500  ,   72500,  5 },
    {72500  ,   83500,  6 },
    {83500  ,   94500,  7 },
    {94500  ,   107000, 8 },
    {107000 ,   118500, 9 },
    {118500 ,   125000, 7 },
    {125000 ,   139500, 8 },
    {139500 ,   150500, 9 },
    {150500 ,   165000, 6 },
    {165000 ,   184500, 7 },
    {184500 ,   207000, 8 },
    {207000 ,   229500, 9 },
    {229500 ,   246500, 10},
    {246500 ,   274500, 5 },
    {274500 ,   319500, 6 },
    {319500 ,   372500, 7 },
    {372500 ,   423500, 8 },
    {423500 ,   468500, 9 },
    {468500 ,   500500, 10},
    {500500 ,   548500, 5 },
    {548500 ,   647500, 6 },
    {647500 ,   732500, 7 },
    {732500 ,   857500, 8 },
    {857500 ,   962000, 9 },
    {962000 ,  1200000, 10},
};/*Fs_setting.xlsx*/
#define ATBM2040_ADC_DIVIDER_LMD_CNT ATBM2040_ARRAY_NUM(ADCDividerLMD)

static ATBM2040ADCDivider_t LeoEADCDividerHMD[] =
{
    {     0,      49500,    8 },
    { 49500,      49999,    9 },
    { 49999,      55999,    6 },
    { 55999,      60999,    8 },
    { 60999,      62999,    5 },
    { 62999,      65499,    6 },
    { 65499,      70999,    9 },
    { 70999,      74499,    6 },
    { 74499,      87999,    7 },
    { 87999,      98999,    8 },
    { 98999,     107499,    9 },
    {107499,     112999,    10},
    {112999,     114999,    7 },
    {114999,     129999,    8 },
    {129999,     144999,    9 },
    {144999,     150999,    10},
    {150999,     152499,    6 },
    {152499,     174999,    7 },
    {174999,     197499,    8 },
    {197499,     219999,    9 },
    {219999,     235499,    10},
    {235499,     264999,    5 },
    {264999,     309999,    6 },
    {309999,     357499,    7 },
    {357499,     405499,    8 },
    {405499,     468999,    9 },
    {468999,     534999,    5 },
    {534999,     638499,    6 },
    {638499,     732999,    7 },
    {732999,     857999,    8 },
    {857999,     962499,    9 },
    {962499,    1200000,    10}
};/*Fs_setting.xlsx*/
#define ATBM2040_LEOE_ADC_DIVIDER_HMD_CNT ATBM2040_ARRAY_NUM(LeoEADCDividerHMD)

static ATBM2040ADCDivider_t LeoEADCDividerLMD[] =
{
    {     0,      44999,    6 },
    { 44999,      49500,    8 },
    { 49500,      49999,    9 },
    { 49999,      52999,    6 },
    { 52999,      55999,    5 },
    { 55999,      57999,    6 },
    { 57999,      58999,    5 },
    { 58999,      65499,    6 },
    { 65499,      70999,    8 },
    { 70999,      72999,    5 },
    { 72999,      83999,    6 },
    { 83999,      94999,    7 },
    { 94999,     107499,    8 },
    {107499,     112999,    9 },
    {112999,     125499,    7 },
    {125499,     139999,    8 },
    {139999,     150999,    9 },
    {150999,     165999,    6 },
    {165999,     184999,    7 },
    {184999,     207499,    8 },
    {207499,     229999,    9 },
    {229999,     235499,    10},
    {235499,     274999,    5 },
    {274999,     319999,    6 },
    {319999,     372999,    7 },
    {372999,     423999,    8 },
    {423999,     468999,    9 },
    {468999,     548999,    5 },
    {548999,     647999,    6 },
    {647999,     732999,    7 },
    {732999,     857999,    8 },
    {857999,     962499,    9 },
    {962499,    1200000,    10}
};/*Fs_setting.xlsx*/
#define ATBM2040_LEOE_ADC_DIVIDER_LMD_CNT ATBM2040_ARRAY_NUM(LeoEADCDividerLMD)

static ATBM2040Reg_t VHFFilterReg[] =
{
    {0x06, 0x0C, 0x00}, /* vhf_fltr_ldo_pd  */
    {0x05, 0x00, 0x01}, /*uhf_fltr_pwdn_ldo  */
    {0x06, 0x1C, 0x01}, /*vhf_fltr_bias_pd */
    {0x06, 0x1F, 0x00}, /* vhf_fltr_gm*/
    {0x12, 0x2E, 0x01}, /*vhf_filt_cal_clk_en*/
    {0x04, 0x0A, 0x00}, /*lna_band*/
    {0x12, 0x41, 0x00}, /*mixr_vhf_pd*/

    {0x06, 0x20, 0x00}, /*vhf_fltr_osc_rst_sel*/
    {0x06, 0x1E, 0x00}, /*vhf_fltr_cap_code_sel*/
};
#define ATBM2040_VHF_FILTER_REG_CNT ATBM2040_ARRAY_NUM(VHFFilterReg)


static ATBM2040Reg_t UHFFltCalibReg[] =
{
    {0x06, 0x0C, 0x00}, /* vhf_fltr_ldo_pd */
    {0x05, 0x00, 0x00}, /* uhf_fltr_pwdn_ldo */
    {0x04, 0x0A, 0x00}, /* lna_band */
    {0x12, 0x41, 0x01}, /* mixr_vhf_pd */

    {0x05, 0x16, 0x00}, /* Uhf_filt_manual_en=1'b0 */
    {0x05, 0x14, 0x03}, /* uhf_filt_fb_adj_ldo[1:0] */
    {0x05, 0x08, 0x1D}, /* uhf_filt_bias_ctrl[4:0] */
    {0x05, 0x09, 0x0C}, /* uhf_filt_bias_adj[4:0] */
    {0x05, 0x12, 0x00}, /*uhf_fltr_gm_switch[5:0]*/

    {0x05, 0x0C, 0x13}, /* uhf_filt_cal_ctrl[4:0] */
    {0x05, 0x0E, 0x00}, /* uhf_filt_en_cal */
    {0x12, 0x2F, 0x01}, /* uhf_filt_cal_clk_en */
};
#define ATBM2040_UHF_FILTER_REG_CNT ATBM2040_ARRAY_NUM(UHFFltCalibReg)

static ATBM2040Reg_t VHF1FilterReg[] =
{
    {0x04, 0x01, 0x01}, /*lna_ldo_pd*/
    {0x04, 0x0A, 0x01}, /*lna_band*/
    {0x06, 0x24, 0x01}, /*Vhf_fltr_reset*/
    {0x06, 0x25, 0x01}, /*Vhf_fltr_cal_sel*/
    {0x06, 0x26, 0x00}, /*Vhf_fltr_start */
    {0x06, 0x27, 0xFF}, /*Vhf_fltr_code_cal[7:0]*/
    {0x06, 0x28, 0x4C}, /*Vhf_fltr_code_osc[7:0]*/

    {0x06, 0x29, 0x4C}, /*vhf_fltr_code_ref*/

    {0x06, 0x2A, 0x1C}, /*Vhf_fltr_shift_cap1[4:0]*/
    {0x06, 0x2B, 0x00}, /*Vhf_fltr_shift_cap2[4:0]*/
    {0x06, 0x2C, 0x0F}, /*Vhf_fltr_shift_cap3[4:0]*/


    {0x06, 0x2D, 0x00}, /*Vhf_fltr_mode_sel*/
    {0x06, 0x2E, 0x01}, /*Vhf_fltr_vcasc_en*/
    {0x06, 0x2F, 0x03}, /*Vhf_fltr_vcasc_adj[1:0]*/
    {0x06, 0x30, 0x01}, /*Vhf_fltr_vtail_aux_en*/
    {0x06, 0x31, 0x02}, /*Vhf_fltr_iadj_aux[2:0]*/

    {0x06, 0x08, 0x01}, /*Vhf_fltr_vref_sel[1:0]*/
    {0x06, 0x0C, 0x00}, /*Vhf_fltr_ldo_pd*/
    {0x06, 0x1C, 0x00}, /*Vhf_fltr_bias_pd*/

};
#define ATBM2040_VHF1_FILTER_REG_CNT ATBM2040_ARRAY_NUM(VHF1FilterReg)

static ATBM2040Reg_t LeoEVHF2FltCalibReg[] =
{
    {0x06, 0x0C, 0x00}, /* vhf_fltr_ldo_pd */
    {0x05, 0x00, 0x01}, /* uhf_fltr_pwdn_ldo */
    {0x04, 0x0A, 0x01}, /* lna_band */
    {0x12, 0x41, 0x01}, /* mixr_vhf_pd */
    {0x05, 0x0E, 0x00}, /* uhf_filt_en_cal */

    /*step 1*/
    {0x05, 0x16, 0x00}, /* Uhf_filt_manual_en=1'b0 */
    {0x05, 0x22, 0x1D}, /* vhf2_filt_bias_ctrl[4:0] */
    {0x05, 0x09, 0x0B}, /* uhf_filt_bias_adj[4:0] */
    {0x05, 0x14, 0x03}, /* uhf_filt_fb_adj_ldo[1:0] */
    {0x05, 0x21, 0x00}, /* vhf2_fltr_pwdn_ldo */

    /*step 2*/
    {0x05, 0x12, 0x00}, /* uhf_fltr_gm_switch[5:0] */

    /*step 3*/
    {0x05, 0x23, 0x11}, /* vhf2_filt_cal_ctrl[4:0] */
    {0x12, 0x2F, 0x01}, /* uhf_filt_cal_clk_en */

};
#define ATBM2040_LEOE_VHF2_FILTER_REG_CNT ATBM2040_ARRAY_NUM(LeoEVHF2FltCalibReg)


static ATBM2040UHFChIndex_t UHFFreqIndex[] =
{
    {1200,     19,    25,     9},
    {946,     21,    25,     9},
    {938,     23,    25,     9},
    {930,     25,    25,     9},
    {922,     27,    25,     9},
    {914,     29,    25,     9},
    {906,     31,    25,     9},
    {898,     33,    25,     9},
    {890,     35,    25,     9},
    {882,     38,    25,     9},
    {874,     41,    25,     9},
    {866,     43,    25,    10},
    {858,     45,    25,    10},
    {850,     48,    26,    10},
    {842,     50,    26,    10},
    {834,     54,    26,    10},
    {826,     56,    26,    10},
    {818,     60,    26,    10},
    {810,     62,    26,    11},
    {802,     65,    26,    11},
    {794,     69,    26,    11},
    {786,     72,    26,    11},
    {778,     75,    26,    11},
    {770,     79,    26,    11},
    {762,     83,    27,    11},
    {754,     87,    27,    12},
    {746,     91,    27,    12},
    {738,     95,    27,    12},
    {730,     99,    27,    12},
    {722,    104,    27,    12},
    {714,    108,    27,    12},
    {706,    112,    27,    12},
    {698,    117,    27,    13},
    {690,    122,    27,    13},
    {682,    127,    28,    14},
    {674,    132,    29,    14},
    {666,    138,    29,    14},
    {658,    143,    29,    14},
    {650,    149,    29,    14},
    {642,    155,    29,    15},
    {634,    162,    30,    15},
    {626,    168,    30,    15},
    {618,    176,    30,    15},
    {610,    183,    30,    16},
    {602,    190,    30,    16},
    {594,    198,    30,    16},
    {586,    206,    31,    16},
    {578,    214,    31,    17},
    {570,    223,    31,    17},
    {562,    232,    31,    17},
    {554,    241,    32,    18},
    {546,    252,    32,    18},
    {538,    261,    35,    20},
    {530,    272,    35,    20},
    {522,    283,    35,    21},
    {514,    295,    36,    21},
    {506,    308,    36,    21},
    {498,    320,    36,    22},
    {490,    334,    36,    22},
    {482,    349,    37,    22},
    {474,    364,    37,    23},
    {466,    380,    37,    24},
    {458,    396,    38,    24},
    {450,    414,    38,    25},
    {442,    434,    38,    25},
    {434,    456,    39,    25},
    {426,    478,    39,    26},
    {418,    500,    40,    26},
    {410,    511,    41,    26},
    {402,    511,    42,    26},
};/*uhf_filter_gm_and_cap_index.xlsx*/
#define ATBM2040_UHF_FREQ_INDEX_CNT ATBM2040_ARRAY_NUM(UHFFreqIndex)

static ATBM2040VHF2ChIndex_t VHF2FreqIndex[] =
{
    {160, 405,  20},
    {168, 362,  19},
    {176, 326,  17},
    {184, 294,  16},
    {192, 265,  15},
    {200, 241,  14},
    {208, 217,  13},
    {216, 197,  13},
    {224, 179,  12},
    {232, 162,  11},
    {240, 147,  11},
    {248, 133,  10},
    {256, 121,  10},
    {264, 109,  9 },
    {272, 98,   9 },
    {280, 89,   9 },
    {288, 80,   8 },
    {296, 71,   8 },
    {304, 64,   8 },
    {312, 57,   7 },
    {320, 51,   7 },
    {328, 45,   7 },
    {336, 39,   7 },
    {344, 34,   6 },
    {352, 29,   6 },
    {360, 24,   6 },
    {368, 20,   6 },
    {376, 15,   6 },
    {384, 12,   6 },
    {392, 8,    6 },
    {400, 5,    5 },
};
#define ATBM2040_VHF2_FREQ_INDEX_CNT ATBM2040_ARRAY_NUM(VHF2FreqIndex)

static ATBM2040Reg_t ADCSettingReg[] =
{
    {0x0B, 0x00, 0x00}, /* adc_pd */
    {0x0B, 0x03, 0x00}, /* adc_pd_1p3vldo_i */
    {0x0B, 0x04, 0x00}, /* adc_pd_1p3vldo_q */
    {0x0B, 0x27, 0x00}, /* adc_pd_2p6vldo */
    {0x0B, 0x25, 0x00}, /* adc_rstn */
    {0x0B, 0x25, 0x01}, /* adc_rstn */
    /*ADC calibration*/
    {0x12, 0x2A, 0x01}, /* adc_ck_rc_tune_en */
    {0x0B, 0x06, 0x00}, /* adc_rc_tuning_pd */
    {0x0B, 0x05, 0x00}, /* adc_tuning_start */
    {0x0B, 0x05, 0x01}, /* adc_tuning_start */
    {0x0B, 0x05, 0x00}, /* adc_tuning_start */
};
#define ATBM2040_ADC_REG_CNT ATBM2040_ARRAY_NUM(ADCSettingReg)

static ATBM2040Reg_t DACSettingReg[] =
{
    {0x10, 0x00, 0x00}, /*   [0] 0   dac_pd      */
    {0x10, 0x02, 0x00}, /*   [0] 0   dac_fltr_pd */
    {0x10, 0x0E, 0x00}, /*   [0] 0   dac_rst_n   */
    {0x10, 0x0E, 0x01}, /*   [0] 1   dac_rst_n   */

    {0x10, 0x0B, 0x01}, /*dac_fltr_ctrl_sel*/
    {0x10, 0x04, 0x15}, /*dac_fltr_manul_ctrl*/

};
#define ATBM2040_DAC_REG_CNT ATBM2040_ARRAY_NUM(DACSettingReg)

static ATBM2040VHFChIndex_t VHFChannelIndex[] =
{
    { 40000,    7,  220,    12},
    { 52500,    5,  210,    15},
    { 60500,    5,  180,    16},
    { 68500,    3,  160,    17},
    { 80000,    3,  130,    18},
    { 88000,    3,  118,    19},
    {115000,    3,   92,    23},
    {123000,    3,   88,    25},
    {131000,    3,   75,    26},
    {139000,    3,   68,    27},
    {147000,    3,   61,    28},
    {155000,    3,   55,    29},
    {163000,    3,   50,    30},
    {171000,    3,   47,    31},
    {179000,    3,   45,    32},
    {187000,    3,   43,    33},
    {195000,    3,   41,    34},
    {203000,    3,   38,    35},
    {211000,    0,   36,    36},
    {219000,    0,   33,    36},
    {227000,    0,   30,    37},
    {235000,    0,   28,    37},
    {243000,    0,   26,    37},
    {251000,    0,   24,    38},
    {259000,    0,   22,    38},
    {267000,    0,   20,    38},
    {275000,    0,   18,    39},
    {283000,    0,   16,    39},
    {291000,    0,   15,    40},
    {299000,    0,   14,    40},
    {307000,    0,   20,    40},
    {315000,    0,   12,    41},
    {323000,    0,   11,    41},
    {331000,    0,   10,    42},
    {339000,    0,    9,    42},
    {347000,    0,    8,    43},
    {355000,    0,    7,    43},
    {363000,    0,    6,    44},
    {371000,    0,    5,    44},
    {379000,    0,    4,    44},
    {387000,    0,    3,    45},
    {395000,    0,    2,    45},
    {403000,    0,    1,    46},
    {411000,    0,    0,    48},
    {419000,    0,    0,    48},
    {427000,    0,    0,    48},
    {431000,    0,    0,    48},
    {443000,    0,    0,    48},
    {451000,    0,    0,    48},
    {459000,    0,    0,    48},
    {467000,    0,    0,    48},
    {475000,    0,    0,    48},
    {483000,    0,    0,    48},
};/*Leo tuner RTF channel index_wo_cal.xlsx*/
#define ATBM2040_VHF_CH_INDEX_CNT ATBM2040_ARRAY_NUM(VHFChannelIndex)

static ATBM2040VHF1ChIndex_t VHF1ChannelIndex[] =
{
    /* freq(MHz)    code_ref<7:0>*/
    {40,    215},
    {48,    176},
    {56,    146},
    {64,    128},
    {72,    113},
    {80,    102},
    {88,    89 },
    {96,    82 },
    {104,   75 },
    {112,   68 },
    {120,   64 },
    {128,   61 },
    {136,   56 },
    {144,   52 },
    {152,   50 },
    {160,   48 },
    {168,   45 },
    {176,   42 },
    {184,   41 },
    {192,   39 },
    {200,   38 },

};/*vhf_bw_code.xlsx*/
#define ATBM2040_VHF1_CH_INDEX_CNT ATBM2040_ARRAY_NUM(VHF1ChannelIndex)

typedef struct
{
    UINT32 FreqMHz;
    UINT16 Intercept;
} ATBM2040RSSIIntercept_t;

static ATBM2040RSSIIntercept_t RSSIIntercept[] =
{
    {100,   0x0044},
    {200,   0x0043},
    {300,   0x003C},
    {400,   0x0035},
    {500,   0x0030},
    {600,   0x002E},
    {700,   0x0026},
    {800,   0x001C},
    {900,   0x0015},
};
#define ATBM2040_RSSI_INTERCEPT_CNT ATBM2040_ARRAY_NUM(RSSIIntercept)


static ATBM2040Reg_t AGCReg[] =
{
    {0x14, 0x2A, 0x12}, /*params_data_length_spectrum_analysis_4x[4:0]*/
    {0x14, 0x72, 0x67}, /*params_thr_aci_status_sig_above_aci[6:0]*/
    {0x14, 0x73, 0x08}, /*params_thr_bb_power_ini[3:0]*/
    {0x14, 0x74, 0x04}, /*params_thr_bb_power_ready_ini[3:0]*/
    {0x14, 0x75, 0x08}, /*params_thr_bb_power_tracking[3:0]*/

    {0x14, 0x80, 0x08}, /*params_thr_rssi_lna_out_ini[3:0]*/

    {0x14, 0x4D, 0x1E}, /*params_lna_gain_max[6:0]*/

    {0x14, 0x45, 0x14}, /*params_if_vga2_gain_step_max[6:0]*/
    {0x14, 0x46, 0x7C}, /*params_if_vga2_gain_step_min[6:0]*/

    {0x14, 0x89, 0xFF}, /*params_waiting_samples_if_vga2_setting[7:0]*/
    {0x14, 0x8A, 0xFF}, /*params_waiting_samples_if_vga2_setting[15:8]*/
    {0x14, 0x8B, 0x01}, /*params_waiting_samples_if_vga2_setting[17:16]*/
    {0x14, 0x8C, 0xFF}, /*params_waiting_samples_lna_setting[7:0]*/
    {0x14, 0x8D, 0xFF}, /*params_waiting_samples_lna_setting[15:8]*/
    {0x14, 0x8E, 0x01}, /*params_waiting_samples_lna_setting[17:16]*/
    {0x14, 0x8F, 0xFF}, /*params_waiting_samples_rf_vga1_setting[7:0]*/
    {0x14, 0x90, 0xFF}, /*params_waiting_samples_rf_vga1_setting[15:8]*/
    {0x14, 0x91, 0x01}, /*params_waiting_samples_rf_vga1_setting[17:16]*/
    {0x14, 0x92, 0xFF}, /*params_waiting_samples_rf_vga2_setting[7:0]*/
    {0x14, 0x93, 0xFF}, /*params_waiting_samples_rf_vga2_setting[15:8]*/
    {0x14, 0x94, 0x01}, /*params_waiting_samples_rf_vga2_setting[17:16]*/

    {0x16, 0xDE, 0x01}, /*params_agc_mode_change*/


};
#define ATBM2040_AGC_REG_CNT ATBM2040_ARRAY_NUM(AGCReg)

static ATBM2040Reg_t MisReg[] =
{
    {0x16, 0x33, 0xC8}, /*params_rssi_slope[7:0]*/
    {0x16, 0x40, 0x06}, /* params_inital_a_d_thr[3:0] */
    {0x14, 0x00, 0x16}, /* params_adc_quantize_gain[6:0] */
    {0x14, 0x43, 0x10}, /* params_lo_change_thr_db */

};
#define ATBM2040_MIS_REG_CNT ATBM2040_ARRAY_NUM(MisReg)

static ATBM2040Reg_t LeoCMisReg[] =
{
    {0x19, 0xB7, 0x08}, /* params_lte_dect_num       */
    {0x19, 0xB8, 0x02}, /* params_lte_undect_num */
    {0x19, 0xC8, 0x00}, /* params_use_double_thr_n_m_large   */
    {0x19, 0xC9, 0xF1}, /* params_rf_input_power_thr_n_m_large_h */
    {0x19, 0xCA, 0xEC}, /* params_rf_input_power_thr_n_m_large_l */
    {0x19, 0xCB, 0x6C}, /* params_rssi_lna_out_aci_status_4_target_for_n_m_large_h   */
    {0x19, 0xCC, 0x62}, /* params_rssi_lna_out_aci_status_4_target_for_n_m_large_l   */
    {0x19, 0xCF, 0x01}, /* params_use_full_speed */
    {0x19, 0xE5, 0x00}, /* params_aci_status_target_setting_solely   */

};
#define ATBM2040_LEOC_MIS_REG_CNT ATBM2040_ARRAY_NUM(LeoCMisReg)

static ATBM2040Reg_t LeoCATVMisReg[] =
{
    {0x14, 0x14, 0x03}, /* params_banalog_change_num_tracking[3:0]   */
    {0x19, 0xB5, 0x08}, /* params_thr_rssi_papr_lte_h    */
    {0x19, 0xB6, 0x02}, /* params_thr_rssi_papr_lte_l    */
    {0x19, 0xBA, 0x00}, /* params_probablity_rssi_papr_judge_n[7:0]  */
    {0x19, 0xBB, 0x01}, /* params_probablity_rssi_papr_judge_n[12:8] */
    {0x19, 0xBC, 0x62}, /* params_rssi_lna_out_aci_status_2_lte_nplus1   */
    {0x19, 0xBE, 0x01}, /* params_lte_dect_mode  */
    {0x1A, 0x62, 0x76}, /* params_thr_iq_narw_band_filter_l  */
    {0x1A, 0x63, 0x7D}, /* params_thr_iq_narw_band_filter_h  */
    {0x1A, 0x17, 0x2F}, /* params_in_band_aci_thr_peak   */
};
#define ATBM2040_LEOC_ATV_MIS_REG_CNT ATBM2040_ARRAY_NUM(LeoCATVMisReg)

static ATBM2040Reg_t LeoCDTVMisReg[] =
{
    {0x14, 0x14, 0x0F}, /* params_banalog_change_num_tracking[3:0]   */
    {0x19, 0xB5, 0x07}, /* params_thr_rssi_papr_lte_h    */
    {0x19, 0xB6, 0x03}, /* params_thr_rssi_papr_lte_l    */
    {0x19, 0xBA, 0x02}, /* params_probablity_rssi_papr_judge_n[7:0]  */
    {0x19, 0xBB, 0x00}, /* params_probablity_rssi_papr_judge_n[12:8] */
    {0x19, 0xBC, 0x60}, /* params_rssi_lna_out_aci_status_2_lte_nplus1   */
    {0x19, 0xBE, 0x00}, /* params_lte_dect_mode  */
    {0x1A, 0x62, 0x14}, /* params_thr_iq_narw_band_filter_l  */
    {0x1A, 0x63, 0x19}, /* params_thr_iq_narw_band_filter_h  */
    {0x1A, 0x17, 0x03}, /* params_in_band_aci_thr_peak   */
};
#define ATBM2040_LEOC_DTV_MIS_REG_CNT ATBM2040_ARRAY_NUM(LeoCDTVMisReg)


/** Phase and Gain Registers start****/
#if ATBM2040_GAIN_PHASE_COMPENSATION
static ATBM2040Reg_t LNAPhaseReg[] =
{
    {0x15, 0xB1, 0x00},
    {0x15, 0xB2, 0x00},
    {0x15, 0xB3, 0x00},
    {0x15, 0xB4, 0x00},
    {0x15, 0xB5, 0x00},
    {0x15, 0xB6, 0x00},
    {0x15, 0xB7, 0x00},
    {0x15, 0xB8, 0x00},
    {0x15, 0xB9, 0x00},
    {0x15, 0xBA, 0x00},
    {0x15, 0xBB, 0x00},
    {0x15, 0xBC, 0x00},
    {0x15, 0xBD, 0x00},
    {0x15, 0xBE, 0x00},
    {0x15, 0xBF, 0x00},
    {0x15, 0xC0, 0x00},
    {0x15, 0xC1, 0x00},
    {0x15, 0xC2, 0x00},
    {0x15, 0xC3, 0x00},
    {0x15, 0xC4, 0x00},
    {0x15, 0xC5, 0x00},
    {0x15, 0xC6, 0x00},
    {0x15, 0xC7, 0x00},
    {0x15, 0xC8, 0x00},
    {0x15, 0xC9, 0x00},
    {0x15, 0xCA, 0x00},
    {0x15, 0xCB, 0x00},
    {0x15, 0xCC, 0x00},
    {0x15, 0xCD, 0x00},
    {0x15, 0xCE, 0x00},
    {0x15, 0xCF, 0x00},
    {0x15, 0xD0, 0x00},
    {0x15, 0xD1, 0x00},
    {0x15, 0xD2, 0x00},
    {0x15, 0xD3, 0x00},
    {0x15, 0xD4, 0x00},
    {0x15, 0xD5, 0x00},
    {0x15, 0xD6, 0x00},
    {0x15, 0xD7, 0x00},
    {0x15, 0xD8, 0x00},
    {0x15, 0xD9, 0x00},
    {0x15, 0xDA, 0x00},
    {0x15, 0xDB, 0x00},
    {0x15, 0xDC, 0x00},
    {0x15, 0xDD, 0x00},
    {0x15, 0xDE, 0x00},
    {0x15, 0xDF, 0x00},
    {0x15, 0xE0, 0x00},
    {0x15, 0xE1, 0x00},
    {0x15, 0xE2, 0x00},
    {0x15, 0xE3, 0x00},
    {0x15, 0xE4, 0x00},
    {0x15, 0xE5, 0x00},
    {0x15, 0xE6, 0x00},
    {0x15, 0xE7, 0x00},
    {0x15, 0xE8, 0x00},
    {0x15, 0xE9, 0x00},
    {0x15, 0xEA, 0x00},
    {0x15, 0xEB, 0x00},
    {0x15, 0xEC, 0x00},
    {0x15, 0xED, 0x00},
    {0x15, 0xEE, 0x00},
    {0x15, 0xEF, 0x00},
    {0x15, 0xF0, 0x00}
};
#define ATBM2040_LNA_PHASE_REG_CNT ATBM2040_ARRAY_NUM(LNAPhaseReg)

static ATBM2040Reg_t RFVGA1PhaseReg[] =
{
    {0x15, 0x94, 0x00},
    {0x15, 0x95, 0x00},
    {0x15, 0x96, 0x00},
    {0x15, 0x97, 0x00},
    {0x15, 0x77, 0x00}
};
#define ATBM2040_RFVGA1_PHASE_REG_CNT ATBM2040_ARRAY_NUM(RFVGA1PhaseReg)


static ATBM2040Reg_t RFVGA2PhaseReg[] =
{
    {0x15, 0x98, 0x00},
    {0x15, 0x99, 0x00},
    {0x15, 0x9A, 0x00},
    {0x15, 0x9B, 0x00},
    {0x15, 0x9C, 0x00}
};
#define ATBM2040_RFVGA2_PHASE_REG_CNT ATBM2040_ARRAY_NUM(RFVGA2PhaseReg)

static ATBM2040Reg_t IFVGA2PhaseReg[] =
{
    {0x15, 0x9D, 0x00},
    {0x15, 0x9E, 0x00},
    {0x15, 0x9F, 0x00},
    {0x15, 0xA0, 0x00},
    {0x15, 0xA1, 0x00},
    {0x15, 0xA2, 0x00},
    {0x15, 0xA3, 0x00},
    {0x15, 0xA4, 0x00},
    {0x15, 0xA5, 0x00},
    {0x15, 0xA6, 0x00},
    {0x15, 0xA7, 0x00},
    {0x15, 0xA8, 0x00},
    {0x15, 0xA9, 0x00},
    {0x15, 0xAA, 0x00},
    {0x15, 0xAB, 0x00},
    {0x15, 0xAC, 0x00},
    {0x15, 0xAD, 0x00},
    {0x15, 0xAE, 0x00},
    {0x15, 0xAF, 0x00},
    {0x15, 0xB0, 0x00}
};
#define ATBM2040_IFVGA2_PHASE_REG_CNT ATBM2040_ARRAY_NUM(IFVGA2PhaseReg)

static ATBM2040Reg_t LNAGainReg[] =
{
    {0x16, 0x97, 0x00},
    {0x16, 0x98, 0x00},
    {0x16, 0x99, 0x00},
    {0x16, 0x9A, 0x00},
    {0x16, 0x9B, 0x00},
    {0x16, 0x9C, 0x00},
    {0x16, 0x9D, 0x00},
    {0x16, 0x9E, 0x00},
    {0x16, 0x9F, 0x00},
    {0x16, 0xA0, 0x00},
    {0x16, 0xA1, 0x00},
    {0x16, 0xA2, 0x00},
    {0x16, 0xA3, 0x00},
    {0x16, 0xA4, 0x00},
    {0x16, 0xA5, 0x00},
    {0x16, 0xA6, 0x00},
    {0x16, 0xA7, 0x00},
    {0x16, 0xA8, 0x00},
    {0x16, 0xA9, 0x00},
    {0x16, 0xAA, 0x00},
    {0x16, 0xAB, 0x00},
    {0x16, 0xAC, 0x00},
    {0x16, 0xAD, 0x00},
    {0x16, 0xAE, 0x00},
    {0x16, 0xAF, 0x00},
    {0x16, 0xB0, 0x00},
    {0x16, 0xB1, 0x00},
    {0x16, 0xB2, 0x00},
    {0x16, 0xB3, 0x00},
    {0x16, 0xB4, 0x00},
    {0x16, 0xB5, 0x00},
    {0x16, 0xB6, 0x00}
};
#define ATBM2040_LNA_GAIN_REG_CNT ATBM2040_ARRAY_NUM(LNAGainReg)

static ATBM2040Reg_t RFVGA1GainReg[] =
{
    {0x16, 0xB7, 0x00},
    {0x16, 0xB8, 0x00},
    {0x16, 0xB9, 0x00},
    {0x16, 0xBA, 0x00},
    {0x16, 0xBB, 0x00}
};
#define ATBM2040_RFVGA1_GAIN_REG_CNT ATBM2040_ARRAY_NUM(RFVGA1GainReg)

static ATBM2040Reg_t RFVGA2GainReg[] =
{
    {0x16, 0xBC, 0x00},
    {0x16, 0xBD, 0x00},
    {0x16, 0xBE, 0x00},
    {0x16, 0xBF, 0x00},
    {0x16, 0xC0, 0x00}
};
#define ATBM2040_RFVGA2_GAIN_REG_CNT ATBM2040_ARRAY_NUM(RFVGA2GainReg)

static ATBM2040Reg_t IFVGA2GainReg[] =
{
    {0x16, 0xC1, 0x00},
    {0x16, 0xC2, 0x00},
    {0x16, 0xC3, 0x00},
    {0x16, 0xC4, 0x00},
    {0x16, 0xC5, 0x00},
    {0x16, 0xC6, 0x00},
    {0x16, 0xC7, 0x00},
    {0x16, 0xC8, 0x00},
    {0x16, 0xC9, 0x00},
    {0x16, 0xCA, 0x00},
    {0x16, 0xCB, 0x00},
    {0x16, 0xCC, 0x00},
    {0x16, 0xCD, 0x00},
    {0x16, 0xCE, 0x00},
    {0x16, 0xCF, 0x00},
    {0x16, 0xD0, 0x00},
    {0x16, 0xD1, 0x00},
    {0x16, 0xD2, 0x00},
    {0x16, 0xD3, 0x00},
    {0x16, 0xD4, 0x00}
};
#define ATBM2040_IFVGA2_GAIN_REG_CNT ATBM2040_ARRAY_NUM(IFVGA2GainReg)
#endif
/** Phase and Gain Registers end****/



static ATBM2040AgcPowerTarget_t AgcPowerTargetDTMB[] =
{
    /*  start   end                     rssi_lna_out                                    rssi_lna_out2                                   bb_power_target
                         DTV     ATV     DTV     ATV    N+-M        DTV       ATV         DTV          ATV      N+-M
                        aci_0   aci_1   aci_2   aci_3   aci_4       aci_0   aci_1   aci_2   aci_3   aci_4            aci 0      aci 1     aci 2             aci 3             aci_4*/
    {000,   100,    { -25,   -18,    -22,    -21,    -17,        -25,    -18,    -22,    -21,    -17,        0x03, 0x80,  0x03, 0x80,  0x03, 0x80,  0x03, 0x70,  0x03, 0x60}},
    {100,   200,    { -27,   -19,    -21,    -22,    -18,        -27,    -19,    -21,    -22,    -18,        0x03, 0x80,  0x03, 0x80,  0x03, 0x80,  0x03, 0x70,  0x03, 0x60}},
    {200,   300,    { -27,   -19,    -22,    -22,    -16,        -27,    -19,    -22,    -22,    -16,        0x03, 0x80,  0x03, 0x80,  0x03, 0x80,  0x03, 0x70,  0x03, 0x60}},
    {300,   400,    { -28,   -20,    -21,    -21,    -16,        -28,    -20,    -21,    -21,    -16,        0x03, 0x80,  0x03, 0x80,  0x03, 0x80,  0x03, 0x70,  0x03, 0x60}},
    {400,   480,    { -28,   -20,    -25,    -24,    -18,        -28,    -20,    -25,    -24,    -18,        0x03, 0x80,  0x03, 0x80,  0x03, 0x80,  0x03, 0x70,  0x03, 0x60}},
    {480,   600,    { -30,   -21,    -26,    -26,    -18,        -30,    -21,    -26,    -26,    -18,        0x03, 0x80,  0x03, 0x80,  0x03, 0x80,  0x03, 0x70,  0x03, 0x60}},
    {600,   650,    { -32,   -22,    -28,    -24,    -18,        -32,    -22,    -28,    -24,    -18,        0x03, 0x80,  0x03, 0x80,  0x03, 0x80,  0x03, 0x70,  0x03, 0x60}},
    {650,   700,    { -30,   -23,    -26,    -26,    -18,        -30,    -23,    -26,    -26,    -18,        0x03, 0x80,  0x03, 0x80,  0x03, 0x80,  0x03, 0x70,  0x03, 0x60}},
    {700,   750,    { -32,   -24,    -28,    -26,    -18,        -32,    -24,    -28,    -26,    -18,        0x03, 0x80,  0x03, 0x80,  0x03, 0x80,  0x03, 0x70,  0x03, 0x60}},
    {750,   800,    { -32,   -26,    -27,    -27,    -18,        -32,    -26,    -27,    -27,    -18,        0x03, 0x80,  0x03, 0x80,  0x03, 0x80,  0x03, 0x70,  0x03, 0x60}},
    {800,   850,    { -31,   -27,    -26,    -26,    -16,        -31,    -27,    -26,    -26,    -16,        0x03, 0x80,  0x03, 0x80,  0x03, 0x80,  0x03, 0x70,  0x03, 0x60}},
    {850,   900,    { -31,   -27,    -27,    -26,    -20,        -31,    -27,    -27,    -26,    -20,        0x03, 0x80,  0x03, 0x80,  0x03, 0x80,  0x03, 0x70,  0x03, 0x60}},
    {900,   950,    { -32,   -28,    -26,    -27,    -22,        -32,    -28,    -26,    -27,    -22,        0x03, 0x80,  0x03, 0x80,  0x03, 0x80,  0x03, 0x70,  0x03, 0x60}},
    {950, 2000,     { -32,   -28,    -28,    -28,    -22,        -32,    -28,    -28,    -28,    -22,        0x03, 0x80,  0x03, 0x80,  0x03, 0x80,  0x03, 0x70,  0x03, 0x60}},

    {0, 0}, /* End Flag */
};
static ATBM2040AgcPowerTarget_t AgcPowerTargetDVBT[] =
{
    /*  start   end                     rssi_lna_out                                    rssi_lna_out2                                   bb_power_target
                         DTV     ATV     DTV     ATV    N+-M        DTV       ATV         DTV          ATV      N+-M
                        aci_0   aci_1   aci_2   aci_3   aci_4       aci_0   aci_1   aci_2   aci_3   aci_4            aci 0      aci 1     aci 2             aci 3             aci_4*/
    {000,   100,    { -28,   -26,    -26,    -23,    -18,        -28,    -28,    -26,    -23,    -18,        0x03, 0x80,  0x03, 0x80,  0x03, 0x80,  0x03, 0x80,  0x03, 0x80}},
    {100,   200,    { -29,   -26,    -27,    -23,    -17,        -29,    -28,    -27,    -23,    -17,        0x03, 0x80,  0x03, 0x80,  0x03, 0x80,  0x03, 0x80,  0x03, 0x80}},
    {200,   300,    { -28,   -28,    -25,    -24,    -16,        -28,    -28,    -25,    -24,    -16,        0x03, 0x80,  0x03, 0x80,  0x03, 0x80,  0x03, 0x80,  0x03, 0x80}},
    {300,   400,    { -29,   -20,    -24,    -24,    -16,        -29,    -28,    -24,    -24,    -16,        0x03, 0x80,  0x03, 0x80,  0x03, 0x80,  0x03, 0x80,  0x03, 0x80}},
    {400,   480,    { -29,   -24,    -26,    -24,    -18,        -29,    -28,    -26,    -24,    -18,        0x03, 0x80,  0x03, 0x80,  0x03, 0x80,  0x03, 0x80,  0x03, 0x80}},
    {480,   600,    { -30,   -20,    -28,    -26,    -20,        -30,    -28,    -28,    -26,    -20,        0x03, 0x80,  0x03, 0x80,  0x03, 0x80,  0x03, 0x80,  0x03, 0x80}},
    {600,   650,    { -30,   -22,    -28,    -26,    -19,        -30,    -28,    -28,    -26,    -19,        0x03, 0x80,  0x03, 0x80,  0x03, 0x80,  0x03, 0x80,  0x03, 0x80}},
    {650,   750,    { -30,   -22,    -28,    -26,    -21,        -30,    -28,    -28,    -26,    -21,        0x03, 0x80,  0x03, 0x80,  0x03, 0x80,  0x03, 0x80,  0x03, 0x80}},
    {750,   850,    { -30,   -24,    -29,    -27,    -21,        -30,    -28,    -29,    -27,    -21,        0x03, 0x80,  0x03, 0x80,  0x03, 0x80,  0x03, 0x80,  0x03, 0x80}},
    {850,   900,    { -32,   -26,    -29,    -27,    -23,        -32,    -28,    -29,    -27,    -23,        0x03, 0x80,  0x03, 0x80,  0x03, 0x80,  0x03, 0x80,  0x03, 0x80}},
    {900, 2000,     { -33,   -28,    -30,    -28,    -22,        -33,    -28,    -30,    -28,    -22,        0x03, 0x80,  0x03, 0x80,  0x03, 0x80,  0x03, 0x80,  0x03, 0x80}},
    {0, 0}, /* End Flag */
};
static ATBM2040AgcPowerTarget_t AgcPowerTargetDVBC[] =
{
    /*  start   end                     rssi_lna_out                                    rssi_lna_out2                                   bb_power_target
                         DTV     ATV     DTV     ATV    N+-M        DTV       ATV         DTV          ATV      N+-M
                        aci_0   aci_1   aci_2   aci_3   aci_4       aci_0   aci_1   aci_2   aci_3   aci_4            aci 0      aci 1     aci 2             aci 3             aci_4*/
    {000,   100,    { -28,   -16,    -27,    -26,    -22,        -28,    -28,    -27,    -26,    -22,        0x03, 0x40,  0x03, 0x40,  0x03, 0x60,  0x03, 0xA0,  0x03, 0x80}},
    {100,   200,    { -28,   -15,    -26,    -26,    -23,        -28,    -28,    -26,    -26,    -23,        0x03, 0x40,  0x03, 0x40,  0x03, 0x60,  0x03, 0xA0,  0x03, 0x80}},
    {200,   300,    { -28,   -14,    -26,    -26,    -23,        -28,    -28,    -26,    -26,    -23,        0x03, 0x40,  0x03, 0x40,  0x03, 0x60,  0x03, 0xA0,  0x03, 0x80}},
    {300,   400,    { -28,   -13,    -25,    -27,    -24,        -28,    -28,    -25,    -27,    -24,        0x03, 0x40,  0x03, 0x40,  0x03, 0x60,  0x03, 0xA0,  0x03, 0x80}},
    {400,   480,    { -30,   -12,    -28,    -28,    -23,        -30,    -28,    -28,    -28,    -23,        0x03, 0x40,  0x03, 0x40,  0x03, 0x60,  0x03, 0xA0,  0x03, 0x80}},
    {480,   600,    { -32,   -26,    -29,    -30,    -24,        -32,    -28,    -29,    -30,    -24,        0x03, 0x40,  0x03, 0x40,  0x03, 0x60,  0x03, 0xA0,  0x03, 0x80}},
    {600,   650,    { -32,   -25,    -28,    -30,    -24,        -32,    -28,    -28,    -30,    -24,        0x03, 0x40,  0x03, 0x40,  0x03, 0x60,  0x03, 0xA0,  0x03, 0x80}},
    {650,   750,    { -33,   -24,    -29,    -32,    -23,        -33,    -28,    -29,    -32,    -23,        0x03, 0x40,  0x03, 0x40,  0x03, 0x60,  0x03, 0xA0,  0x03, 0x80}},
    {750,   850,    { -33,   -23,    -30,    -29,    -25,        -33,    -28,    -30,    -29,    -25,        0x03, 0x40,  0x03, 0x40,  0x03, 0x60,  0x03, 0xA0,  0x03, 0x80}},
    {850,   900,    { -34,   -21,    -32,    -31,    -23,        -34,    -28,    -32,    -31,    -23,        0x03, 0x40,  0x03, 0x40,  0x03, 0x60,  0x03, 0xA0,  0x03, 0x80}},
    {900, 2000,     { -32,   -19,    -32,    -32,    -26,        -32,    -28,    -32,    -32,    -26,        0x03, 0x40,  0x03, 0x40,  0x03, 0x60,  0x03, 0xA0,  0x03, 0x80}},
    {0, 0}, /* End Flag */
};


static ATBM2040AgcPowerTarget_t AgcPowerTargetATV[] =
{
    /*  start   end                     rssi_lna_out                                    rssi_lna_out2                                   bb_power_target
                         DTV     ATV     DTV     ATV    N+-M        DTV       ATV         DTV          ATV      N+-M
                        aci_0   aci_1   aci_2   aci_3   aci_4       aci_0   aci_1   aci_2   aci_3   aci_4            aci 0      aci 1     aci 2             aci 3             aci_4*/
    {000,   100,    { -28,   -28,    -28,    -28,    -28,        -28,    -28,    -28,    -28,    -28,        0x03, 0x80,  0x03, 0x80,  0x03, 0x80,  0x03, 0x80,  0x03, 0x80}},
    {100,   200,    { -28,   -28,    -28,    -28,    -28,        -28,    -28,    -28,    -28,    -28,        0x03, 0x80,  0x03, 0x80,  0x03, 0x80,  0x03, 0x80,  0x03, 0x80}},
    {200,   300,    { -28,   -28,    -28,    -28,    -28,        -28,    -28,    -28,    -28,    -28,        0x03, 0x80,  0x03, 0x80,  0x03, 0x80,  0x03, 0x80,  0x03, 0x80}},
    {300,   400,    { -28,   -28,    -28,    -28,    -28,        -28,    -28,    -28,    -28,    -28,        0x03, 0x80,  0x03, 0x80,  0x03, 0x80,  0x03, 0x80,  0x03, 0x80}},
    {400,   480,    { -28,   -28,    -28,    -28,    -28,        -28,    -28,    -28,    -28,    -28,        0x03, 0x80,  0x03, 0x80,  0x03, 0x80,  0x03, 0x80,  0x03, 0x80}},
    {480,   600,    { -28,   -28,    -28,    -28,    -28,        -28,    -28,    -28,    -28,    -28,        0x03, 0x80,  0x03, 0x80,  0x03, 0x80,  0x03, 0x80,  0x03, 0x80}},
    {600,   650,    { -28,   -28,    -28,    -28,    -28,        -28,    -28,    -28,    -28,    -28,        0x03, 0x80,  0x03, 0x80,  0x03, 0x80,  0x03, 0x80,  0x03, 0x80}},
    {650,   700,    { -28,   -28,    -28,    -28,    -28,        -28,    -28,    -28,    -28,    -28,        0x03, 0x80,  0x03, 0x80,  0x03, 0x80,  0x03, 0x80,  0x03, 0x80}},
    {700,   750,    { -28,   -28,    -28,    -28,    -28,        -28,    -28,    -28,    -28,    -28,        0x03, 0x80,  0x03, 0x80,  0x03, 0x80,  0x03, 0x80,  0x03, 0x80}},
    {750,   800,    { -28,   -28,    -28,    -28,    -28,        -28,    -28,    -28,    -28,    -28,        0x03, 0x80,  0x03, 0x80,  0x03, 0x80,  0x03, 0x80,  0x03, 0x80}},
    {800,   850,    { -28,   -28,    -28,    -28,    -28,        -28,    -28,    -28,    -28,    -28,        0x03, 0x80,  0x03, 0x80,  0x03, 0x80,  0x03, 0x80,  0x03, 0x80}},
    {850,   900,    { -28,   -28,    -28,    -28,    -28,        -28,    -28,    -28,    -28,    -28,        0x03, 0x80,  0x03, 0x80,  0x03, 0x80,  0x03, 0x80,  0x03, 0x80}},
    {900,   950,    { -32,   -32,    -32,    -32,    -32,        -28,    -32,    -28,    -28,    -28,        0x03, 0x80,  0x03, 0x80,  0x03, 0x80,  0x03, 0x80,  0x03, 0x80}},
    {950,  2000,    { -32,   -32,    -32,    -32,    -32,        -28,    -32,    -28,    -28,    -28,        0x03, 0x80,  0x03, 0x80,  0x03, 0x80,  0x03, 0x80,  0x03, 0x80}},

    {0, 0}, /* End Flag */
};

static ATBM2040AgcPowerTarget_t LeoCAgcPowerTargetDTMB[] =
{
    /*  start   end                     rssi_lna_out                                    rssi_lna_out2                                   bb_power_target
                         DTV     ATV     DTV     ATV    N+-M        DTV       ATV         DTV          ATV      N+-M
                        aci_0   aci_1   aci_2   aci_3   aci_4       aci_0   aci_1   aci_2   aci_3   aci_4            aci 0      aci 1     aci 2             aci 3             aci_4*/
    {000,   100,    { -24,   -18,    -20,    -17,    -13,        -24,    -18,    -20,    -17,    -13,        0x03, 0x40,  0x03, 0x80,  0x03, 0x30,  0x03, 0x60,  0x03, 0x30}},
    {100,   200,    { -24,   -19,    -20,    -19,    -12,        -24,    -19,    -20,    -19,    -12,        0x03, 0x40,  0x03, 0x80,  0x03, 0x30,  0x03, 0x70,  0x03, 0x30}},
    {200,   300,    { -24,   -19,    -20,    -19,    -12,        -24,    -19,    -20,    -19,    -12,        0x03, 0x30,  0x03, 0x80,  0x03, 0x20,  0x03, 0x70,  0x03, 0x20}},
    {300,   400,    { -24,   -20,    -20,    -21,    -12,        -24,    -20,    -20,    -21,    -12,        0x03, 0x40,  0x03, 0x80,  0x03, 0x20,  0x03, 0x60,  0x03, 0x20}},
    {400,   480,    { -24,   -20,    -22,    -22,    -12,        -24,    -20,    -22,    -22,    -12,        0x03, 0x40,  0x03, 0x80,  0x03, 0x20,  0x03, 0x40,  0x03, 0x10}},
    {480,   600,    { -26,   -21,    -22,    -22,    -12,        -26,    -21,    -22,    -22,    -12,        0x03, 0x20,  0x03, 0x80,  0x03, 0x10,  0x03, 0x20,  0x03, 0x10}},
    {600,   700,    { -26,   -22,    -23,    -22,    -14,        -26,    -22,    -23,    -22,    -14,        0x03, 0x40,  0x03, 0x80,  0x03, 0x20,  0x03, 0x70,  0x03, 0x30}},
    {700,   800,    { -27,   -23,    -25,    -24,    -15,        -27,    -23,    -25,    -24,    -15,        0x03, 0x30,  0x03, 0x80,  0x03, 0x20,  0x03, 0x60,  0x03, 0x30}},
    {800,   900,    { -27,   -24,    -28,    -26,    -18,        -27,    -24,    -28,    -26,    -18,        0x03, 0x40,  0x03, 0x80,  0x03, 0x40,  0x03, 0x70,  0x03, 0x20}},
    {900,  2000,    { -26,   -28,    -27,    -28,    -19,        -26,    -28,    -27,    -28,    -19,        0x03, 0x40,  0x03, 0x80,  0x03, 0x20,  0x03, 0x80,  0x03, 0x20}},

    {0, 0}, /* End Flag */
};
static ATBM2040AgcPowerTarget_t LeoCAgcPowerTargetDVBT[] =
{
    /*  start   end                     rssi_lna_out                                    rssi_lna_out2                                   bb_power_target
                         DTV     ATV     DTV     ATV    N+-M        DTV       ATV         DTV          ATV      N+-M
                        aci_0   aci_1   aci_2   aci_3   aci_4       aci_0   aci_1   aci_2   aci_3   aci_4            aci 0      aci 1     aci 2             aci 3             aci_4*/
    {000,   100,    { -27,   -26,    -25,    -18,    -14,        -27,    -28,    -25,    -18,    -14,        0x03, 0x80,  0x03, 0x80,  0x03, 0x20,  0x03, 0x80,  0x03, 0x30}},
    {100,   200,    { -31,   -26,    -24,    -16,    -14,        -31,    -28,    -24,    -16,    -14,        0x03, 0x20,  0x03, 0x80,  0x03, 0x20,  0x03, 0x20,  0x03, 0x30}},
    {200,   300,    { -28,   -28,    -24,    -16,    -14,        -28,    -28,    -24,    -16,    -14,        0x03, 0x40,  0x03, 0x80,  0x03, 0x00,  0x03, 0x20,  0x03, 0x30}},
    {300,   400,    { -28,   -20,    -20,    -18,    -14,        -28,    -28,    -20,    -18,    -14,        0x03, 0x40,  0x03, 0x80,  0x03, 0x60,  0x03, 0x40,  0x03, 0x30}},
    {400,   480,    { -30,   -24,    -23,    -18,    -14,        -30,    -28,    -23,    -18,    -14,        0x03, 0x10,  0x03, 0x80,  0x03, 0x20,  0x03, 0x30,  0x03, 0x30}},
    {480,   600,    { -30,   -20,    -20,    -19,    -14,        -30,    -28,    -20,    -19,    -14,        0x03, 0x40,  0x03, 0x80,  0x03, 0x00,  0x03, 0x40,  0x03, 0x30}},
    {600,   650,    { -31,   -22,    -21,    -20,    -15,        -31,    -28,    -21,    -20,    -15,        0x03, 0x40,  0x03, 0x80,  0x03, 0x10,  0x03, 0x40,  0x03, 0x50}},
    {650,   750,    { -31,   -22,    -23,    -22,    -17,        -31,    -28,    -23,    -22,    -17,        0x03, 0x30,  0x03, 0x80,  0x03, 0x30,  0x03, 0x40,  0x03, 0x40}},
    {750,   850,    { -30,   -24,    -24,    -22,    -18,        -30,    -28,    -24,    -22,    -18,        0x03, 0x40,  0x03, 0x80,  0x03, 0x20,  0x03, 0x40,  0x03, 0x30}},
    {850,   900,    { -33,   -26,    -23,    -22,    -18,        -33,    -28,    -23,    -22,    -18,        0x03, 0x50,  0x03, 0x80,  0x03, 0x20,  0x03, 0x50,  0x03, 0x30}},
    {900,  2000,    { -30,   -28,    -24,    -22,    -18,        -30,    -28,    -24,    -22,    -18,        0x03, 0x40,  0x03, 0x80,  0x03, 0x20,  0x03, 0x60,  0x03, 0x50}},
    {0, 0}, /* End Flag */
};

static ATBM2040AgcPowerTarget_t LeoCAgcPowerTargetATV[] =
{
    /*  start   end                     rssi_lna_out                                    rssi_lna_out2                                   bb_power_target
                         DTV     ATV     DTV     ATV    N+-M        DTV       ATV         DTV          ATV      N+-M
                        aci_0   aci_1   aci_2   aci_3   aci_4       aci_0   aci_1   aci_2   aci_3   aci_4            aci 0      aci 1     aci 2             aci 3             aci_4*/
    {000,   100,    { -28,   -16,    -28,    -18,    -15,        -28,    -16,    -28,    -18,    -15,        0x03, 0x80,  0x03, 0x20,  0x03, 0x80,  0x03, 0x40,  0x03, 0x00}},
    {100,   200,    { -28,   -17,    -28,    -18,    -16,        -28,    -17,    -28,    -18,    -16,        0x03, 0x80,  0x03, 0x20,  0x03, 0x80,  0x03, 0x40,  0x03, 0x20}},
    {200,   300,    { -28,   -17,    -28,    -18,    -16,        -28,    -17,    -28,    -18,    -16,        0x03, 0x80,  0x03, 0x40,  0x03, 0x80,  0x03, 0x40,  0x03, 0x40}},
    {300,   400,    { -28,   -17,    -28,    -18,    -17,        -28,    -17,    -28,    -18,    -17,        0x03, 0x80,  0x03, 0x40,  0x03, 0x80,  0x03, 0x40,  0x03, 0x20}},
    {400,   480,    { -28,   -17,    -28,    -19,    -17,        -28,    -17,    -28,    -19,    -17,        0x03, 0x80,  0x03, 0x40,  0x03, 0x80,  0x03, 0x40,  0x03, 0x20}},
    {480,   600,    { -28,   -16,    -28,    -19,    -16,        -28,    -16,    -28,    -19,    -16,        0x03, 0x80,  0x03, 0x20,  0x03, 0x80,  0x03, 0x40,  0x03, 0x20}},
    {600,   700,    { -28,   -16,    -28,    -19,    -16,        -28,    -16,    -28,    -19,    -16,        0x03, 0x80,  0x03, 0x20,  0x03, 0x80,  0x03, 0x40,  0x03, 0x00}},
    {700,   800,    { -28,   -16,    -28,    -19,    -15,        -28,    -16,    -28,    -19,    -15,        0x03, 0x80,  0x03, 0x20,  0x03, 0x80,  0x03, 0x40,  0x03, 0x00}},
    {800,   900,    { -28,   -16,    -28,    -19,    -16,        -28,    -16,    -28,    -19,    -16,        0x03, 0x80,  0x03, 0x20,  0x03, 0x80,  0x03, 0x40,  0x03, 0x00}},
    {900,   2000,   { -28,   -16,    -28,    -19,    -16,        -28,    -16,    -28,    -19,    -16,        0x03, 0x80,  0x03, 0x20,  0x03, 0x80,  0x03, 0x40,  0x03, 0x00}},

    {0, 0}, /* End Flag */
};

static ATBM2040AgcPowerTarget_t LeoEAgcPowerTargetDTMB[] =
{
    /*  start   end                     rssi_lna_out                                    rssi_lna_out2                                   bb_power_target
                         DTV     ATV     DTV     ATV    N+-M        DTV       ATV         DTV          ATV      N+-M
                        aci_0   aci_1   aci_2   aci_3   aci_4       aci_0   aci_1   aci_2   aci_3   aci_4            aci 0      aci 1     aci 2             aci 3             aci_4*/
    {000,   100,    { -22,   -18,    -21,    -20,    -14,        -22,    -18,    -21,    -20,    -14,        0x03, 0x40,  0x03, 0x80,  0x03, 0x40,  0x03, 0x60,  0x03, 0x30}},
    {100,   160,    { -22,   -19,    -21,    -20,    -14,        -22,    -19,    -21,    -20,    -14,        0x03, 0x40,  0x03, 0x80,  0x03, 0x30,  0x03, 0x70,  0x03, 0x30}},
    {160,   300,    { -22,   -19,    -19,    -18,    -17,        -22,    -19,    -19,    -18,    -17,        0x03, 0x40,  0x03, 0x80,  0x03, 0x20,  0x03, 0x70,  0x03, 0x20}},
    {300,   400,    { -22,   -20,    -19,    -18,    -17,        -22,    -20,    -19,    -18,    -17,        0x03, 0x40,  0x03, 0x80,  0x03, 0x30,  0x03, 0x80,  0x03, 0x20}},
    {400,   480,    { -22,   -20,    -15,    -14,    -16,        -22,    -20,    -15,    -14,    -16,        0x03, 0x40,  0x03, 0x80,  0x03, 0x40,  0x03, 0x60,  0x03, 0x10}},
    {480,   600,    { -22,   -21,    -15,    -14,    -16,        -22,    -21,    -15,    -14,    -16,        0x03, 0x40,  0x03, 0x80,  0x03, 0x40,  0x03, 0x70,  0x03, 0x10}},
    {600,   700,    { -22,   -22,    -15,    -14,    -16,        -22,    -22,    -15,    -14,    -16,        0x03, 0x40,  0x03, 0x80,  0x03, 0x40,  0x03, 0x70,  0x03, 0x30}},
    {700,   800,    { -22,   -23,    -14,    -14,    -15,        -22,    -23,    -14,    -14,    -15,        0x03, 0x30,  0x03, 0x80,  0x03, 0x40,  0x03, 0x80,  0x03, 0x30}},
    {800,   900,    { -22,   -24,    -14,    -14,    -15,        -22,    -24,    -14,    -14,    -15,        0x03, 0x40,  0x03, 0x80,  0x03, 0x40,  0x03, 0x70,  0x03, 0x20}},
    {900,  2000,    { -22,   -28,    -14,    -14,    -15,        -22,    -28,    -14,    -14,    -15,        0x03, 0x40,  0x03, 0x80,  0x03, 0x40,  0x03, 0x70,  0x03, 0x20}},

    {0, 0}, /* End Flag */
};
static ATBM2040AgcPowerTarget_t LeoEAgcPowerTargetDVBT[] =
{
    /*  start   end                     rssi_lna_out                                    rssi_lna_out2                                   bb_power_target
                         DTV     ATV     DTV     ATV    N+-M        DTV       ATV         DTV          ATV      N+-M
                        aci_0   aci_1   aci_2   aci_3   aci_4       aci_0   aci_1   aci_2   aci_3   aci_4            aci 0      aci 1     aci 2             aci 3             aci_4*/
    {000,   100,    { -25,   -26,    -20,    -20,    -14,        -25,    -28,    -20,    -20,    -14,        0x03, 0x80,  0x03, 0x80,  0x03, 0x20,  0x03, 0x80,  0x03, 0x30}},
    {100,   160,    { -25,   -26,    -20,    -20,    -14,        -25,    -28,    -20,    -20,    -14,        0x03, 0x20,  0x03, 0x80,  0x03, 0x20,  0x03, 0x20,  0x03, 0x30}},
    {160,   300,    { -23,   -28,    -20,    -18,    -15,        -23,    -28,    -20,    -18,    -15,        0x03, 0x40,  0x03, 0x80,  0x03, 0x00,  0x03, 0x20,  0x03, 0x30}},
    {300,   400,    { -23,   -20,    -20,    -18,    -15,        -23,    -28,    -20,    -18,    -15,        0x03, 0x40,  0x03, 0x80,  0x03, 0x60,  0x03, 0x40,  0x03, 0x30}},
    {400,   480,    { -23,   -24,    -16,    -14,    -16,        -23,    -28,    -16,    -14,    -16,        0x03, 0x10,  0x03, 0x80,  0x03, 0x20,  0x03, 0x30,  0x03, 0x30}},
    {480,   600,    { -23,   -20,    -16,    -14,    -16,        -23,    -28,    -16,    -14,    -16,        0x03, 0x40,  0x03, 0x80,  0x03, 0x00,  0x03, 0x40,  0x03, 0x30}},
    {600,   700,    { -22,   -22,    -15,    -14,    -15,        -22,    -28,    -15,    -14,    -15,        0x03, 0x40,  0x03, 0x80,  0x03, 0x10,  0x03, 0x40,  0x03, 0x50}},
    {700,   800,    { -22,   -22,    -15,    -14,    -15,        -22,    -28,    -15,    -14,    -15,        0x03, 0x30,  0x03, 0x80,  0x03, 0x40,  0x03, 0x40,  0x03, 0x20}},
    {800,   900,    { -22,   -24,    -15,    -14,    -15,        -22,    -28,    -15,    -14,    -15,        0x03, 0x40,  0x03, 0x80,  0x03, 0x20,  0x03, 0x40,  0x03, 0x30}},
    {900,  2000,    { -22,   -28,    -15,    -14,    -15,        -22,    -28,    -15,    -14,    -15,        0x03, 0x40,  0x03, 0x80,  0x03, 0x20,  0x03, 0x60,  0x03, 0x50}},

    {0, 0}, /* End Flag */
};

static ATBM2040AgcPowerTarget_t LeoEAgcPowerTargetDVBC[] =
{
    /*  start   end                     rssi_lna_out                                    rssi_lna_out2                                   bb_power_target
                         DTV     ATV     DTV     ATV    N+-M        DTV       ATV         DTV          ATV      N+-M
                        aci_0   aci_1   aci_2   aci_3   aci_4       aci_0   aci_1   aci_2   aci_3   aci_4            aci 0      aci 1     aci 2             aci 3             aci_4*/
    {000,   100,    { -22,   -16,    -20,    -20,    -13,        -22,    -28,    -20,    -20,    -13,        0x03, 0x40,  0x03, 0x40,  0x03, 0x60,  0x03, 0xA0,  0x03, 0x80}},
    {100,   160,    { -22,   -15,    -20,    -20,    -13,        -22,    -28,    -20,    -20,    -13,        0x03, 0x40,  0x03, 0x40,  0x03, 0x60,  0x03, 0xA0,  0x03, 0x80}},
    {160,   300,    { -21,   -14,    -15,    -16,    -15,        -21,    -28,    -15,    -16,    -15,        0x03, 0x40,  0x03, 0x40,  0x03, 0x60,  0x03, 0xA0,  0x03, 0x80}},
    {300,   400,    { -21,   -13,    -15,    -16,    -15,        -21,    -28,    -15,    -16,    -15,        0x03, 0x40,  0x03, 0x40,  0x03, 0x60,  0x03, 0xA0,  0x03, 0x80}},
    {400,   480,    { -20,   -12,    -16,    -17,    -17,        -20,    -28,    -16,    -17,    -17,        0x03, 0x40,  0x03, 0x40,  0x03, 0x60,  0x03, 0xA0,  0x03, 0x80}},
    {480,   600,    { -20,   -26,    -16,    -17,    -17,        -20,    -28,    -16,    -17,    -17,        0x03, 0x40,  0x03, 0x40,  0x03, 0x60,  0x03, 0xA0,  0x03, 0x80}},
    {600,   700,    { -20,   -25,    -16,    -16,    -16,        -20,    -28,    -16,    -16,    -16,        0x03, 0x40,  0x03, 0x40,  0x03, 0x60,  0x03, 0xA0,  0x03, 0x80}},
    {700,   800,    { -20,   -23,    -16,    -16,    -16,        -20,    -28,    -16,    -16,    -16,        0x03, 0x40,  0x03, 0x40,  0x03, 0x60,  0x03, 0xA0,  0x03, 0x80}},
    {800,   900,    { -20,   -21,    -16,    -16,    -16,        -20,    -28,    -16,    -16,    -16,        0x03, 0x40,  0x03, 0x40,  0x03, 0x60,  0x03, 0xA0,  0x03, 0x80}},
    {900,  2000,    { -20,   -19,    -16,    -16,    -16,        -20,    -28,    -16,    -16,    -16,        0x03, 0x40,  0x03, 0x40,  0x03, 0x60,  0x03, 0xA0,  0x03, 0x80}},

    {0, 0}, /* End Flag */
};
static ATBM2040AgcPowerTarget_t LeoEAgcPowerTargetATV[] =
{
    /*  start   end                     rssi_lna_out                                    rssi_lna_out2                                   bb_power_target
                         DTV     ATV     DTV     ATV    N+-M        DTV       ATV         DTV          ATV      N+-M
                        aci_0   aci_1   aci_2   aci_3   aci_4       aci_0   aci_1   aci_2   aci_3   aci_4            aci 0      aci 1     aci 2             aci 3             aci_4*/
    {000,   100,    { -28,   -21,    -28,    -26,    -15,        -28,    -21,    -28,    -26,    -15,        0x03, 0x80,  0x03, 0x80,  0x03, 0x80,  0x03, 0x40,  0x03, 0x00}},
    {100,   160,    { -28,   -21,    -28,    -26,    -15,        -28,    -21,    -28,    -26,    -15,        0x03, 0x80,  0x03, 0x80,  0x03, 0x80,  0x03, 0x40,  0x03, 0x20}},
    {160,   200,    { -28,   -22,    -28,    -26,    -14,        -28,    -22,    -28,    -26,    -14,        0x03, 0x80,  0x03, 0x80,  0x03, 0x80,  0x03, 0x40,  0x03, 0x40}},
    {200,   300,    { -28,   -22,    -28,    -26,    -14,        -28,    -22,    -28,    -26,    -14,        0x03, 0x80,  0x03, 0x80,  0x03, 0x80,  0x03, 0x40,  0x03, 0x40}},
    {300,   400,    { -28,   -22,    -28,    -26,    -14,        -28,    -22,    -28,    -26,    -14,        0x03, 0x80,  0x03, 0x80,  0x03, 0x80,  0x03, 0x40,  0x03, 0x20}},
    {400,   480,    { -28,   -19,    -28,    -24,    -14,        -28,    -19,    -28,    -24,    -14,        0x03, 0x80,  0x03, 0x80,  0x03, 0x80,  0x03, 0x40,  0x03, 0x20}},
    {480,   600,    { -28,   -19,    -28,    -24,    -14,        -28,    -19,    -28,    -24,    -14,        0x03, 0x80,  0x03, 0x80,  0x03, 0x80,  0x03, 0x40,  0x03, 0x20}},
    {600,   700,    { -28,   -19,    -28,    -24,    -14,        -28,    -19,    -28,    -24,    -14,        0x03, 0x80,  0x03, 0x80,  0x03, 0x80,  0x03, 0x40,  0x03, 0x00}},
    {700,   800,    { -28,   -18,    -28,    -20,    -14,        -28,    -18,    -28,    -20,    -14,        0x03, 0x80,  0x03, 0x80,  0x03, 0x80,  0x03, 0x40,  0x03, 0x00}},
    {800,   900,    { -28,   -18,    -28,    -20,    -14,        -28,    -18,    -28,    -20,    -14,        0x03, 0x80,  0x03, 0x80,  0x03, 0x80,  0x03, 0x40,  0x03, 0x00}},
    {900,   2000,   { -28,   -18,    -28,    -20,    -14,        -28,    -18,    -28,    -20,    -14,        0x03, 0x80,  0x03, 0x80,  0x03, 0x80,  0x03, 0x40,  0x03, 0x00}},

    {0, 0}, /* End Flag */
};

static ATBM2040AgcPowerTarget_t LeoGAgcPowerTargetDVBT[]=
{
/*    start    end                     rssi_lna_out                                    rssi_lna_out2                                    bb_power_target
                     DTV     ATV     DTV     ATV    N+-M        DTV          ATV           DTV           ATV        N+-M
                    aci_0    aci_1    aci_2    aci_3    aci_4        aci_0    aci_1    aci_2    aci_3    aci_4             aci 0         aci 1      aci 2             aci 3             aci_4*/
    {000,    100,    {-25,    -26,    -24,    -24,    -14,        -25,    -26,    -24,    -24,    -14,        0x03,0x80,    0x03,0x80,    0x03,0x20,    0x03,0x80,    0x03,0x30}},
    {100,    160,    {-25,    -26,    -24,    -24,    -14,        -25,    -26,    -24,    -24,    -14,        0x03,0x20,    0x03,0x80,    0x03,0x20,    0x03,0x20,    0x03,0x30}},
    {160,    300,    {-25,    -28,    -24,    -22,    -15,        -25,    -28,    -24,    -22,    -15,        0x03,0x40,    0x03,0x80,    0x03,0x00,    0x03,0x20,    0x03,0x30}},
    {300,    400,    {-25,    -20,    -24,    -22,    -15,        -25,    -20,    -24,    -22,    -15,        0x03,0x40,    0x03,0x80,    0x03,0x60,    0x03,0x40,    0x03,0x30}},
    {400,    480,    {-25,    -24,    -24,    -18,    -16,        -25,    -24,    -24,    -18,    -16,        0x03,0x10,    0x03,0x80,    0x03,0x20,    0x03,0x30,    0x03,0x30}},
    {480,    600,    {-25,    -20,    -24,    -18,    -16,        -25,    -20,    -24,    -18,    -16,        0x03,0x40,    0x03,0x80,    0x03,0x00,    0x03,0x40,    0x03,0x30}},
    {600,    700,    {-25,    -22,    -24,    -18,    -15,        -25,    -22,    -24,    -18,    -15,        0x03,0x40,    0x03,0x80,    0x03,0x10,    0x03,0x40,    0x03,0x50}},
    {700,    800,    {-25,    -22,    -24,    -18,    -15,        -25,    -22,    -24,    -18,    -15,        0x03,0x30,    0x03,0x80,    0x03,0x40,    0x03,0x40,    0x03,0x20}},
    {800,    900,    {-25,    -24,    -24,    -18,    -15,        -25,    -24,    -24,    -18,    -15,        0x03,0x40,    0x03,0x80,    0x03,0x20,    0x03,0x40,    0x03,0x30}},
    {900,  2000,    {-25,    -28,    -24,    -18,    -15,        -25,    -28,    -24,    -18,    -15,        0x03,0x40,    0x03,0x80,    0x03,0x20,    0x03,0x60,    0x03,0x50}},

    {0, 0}, /* End Flag */
};

static ATBM2040AgcPowerTarget_t LeoGAgcPowerTargetATV[]=
{
/*    start    end                     rssi_lna_out                                    rssi_lna_out2                                    bb_power_target
                     DTV     ATV     DTV     ATV    N+-M        DTV          ATV           DTV           ATV        N+-M
                    aci_0    aci_1    aci_2    aci_3    aci_4        aci_0    aci_1    aci_2    aci_3    aci_4             aci 0         aci 1      aci 2             aci 3             aci_4*/
    {000,    100,    {-28,    -24,    -28,    -12,    -15,        -28,    -24,    -28,    -21,    -15,        0x03,0x80,    0x03,0x80,    0x03,0x80,    0x03,0x40,    0x03,0x00}},    
    {100,    160,    {-28,    -24,    -28,    -12,    -15,        -28,    -24,    -28,    -21,    -15,        0x03,0x80,    0x03,0x80,    0x03,0x80,    0x03,0x40,    0x03,0x20}},    
    {160,    200,    {-28,    -23,    -28,    -12,    -14,        -28,    -23,    -28,    -21,    -14,        0x03,0x80,    0x03,0x80,    0x03,0x80,    0x03,0x40,    0x03,0x40}},    
    {200,    300,    {-28,    -23,    -28,    -12,    -14,        -28,    -23,    -28,    -21,    -14,        0x03,0x80,    0x03,0x80,    0x03,0x80,    0x03,0x40,    0x03,0x40}},    
    {300,    400,    {-28,    -23,    -28,    -12,    -14,        -28,    -23,    -28,    -21,    -14,        0x03,0x80,    0x03,0x80,    0x03,0x80,    0x03,0x40,    0x03,0x20}},    
    {400,    480,    {-28,    -23,    -28,    -12,    -14,        -28,    -23,    -28,    -21,    -14,        0x03,0x80,    0x03,0x80,    0x03,0x80,    0x03,0x40,    0x03,0x20}},    
    {480,    600,    {-28,    -23,    -28,    -12,    -14,        -28,    -23,    -28,    -21,    -14,        0x03,0x80,    0x03,0x80,    0x03,0x80,    0x03,0x40,    0x03,0x20}},    
    {600,    700,    {-28,    -23,    -28,    -12,    -14,        -28,    -23,    -28,    -21,    -14,        0x03,0x80,    0x03,0x80,    0x03,0x80,    0x03,0x40,    0x03,0x00}},    
    {700,    800,    {-28,    -21,    -28,    -10,    -14,        -28,    -21,    -28,    -20,    -14,        0x03,0x80,    0x03,0x80,    0x03,0x80,    0x03,0x40,    0x03,0x00}},    
    {800,    900,    {-28,    -21,    -28,    -10,    -14,        -28,    -21,    -28,    -20,    -14,        0x03,0x80,    0x03,0x80,    0x03,0x80,    0x03,0x40,    0x03,0x00}},    
    {900,    2000,    {-28,    -21,    -28,    -10,    -14,        -28,    -21,    -28,    -20,    -14,        0x03,0x80,    0x03,0x80,    0x03,0x80,    0x03,0x40,    0x03,0x00}},    

    {0, 0}, /* End Flag */
};

static ATBM2040Reg_t AgcPowerTargetReg[] =
{
    {0x14, 0x58, 0x67}, /* -25dB for rssi_lna_out_aci_0 */
    {0x14, 0x59, 0x69}, /* -23dB for rssi_lna_out aci_1 */
    {0x14, 0x5A, 0x67}, /* -25dB for rssi_lna_out aci_2 */
    {0x14, 0x5B, 0x69}, /* -23dB for rssi_lna_out aci_3   */
    {0x14, 0x5C, 0x67}, /* aci_4 */

    {0x17, 0xF1, 0x64}, /* params_rssi_lna_out_aci_status_0_alter */
    {0x17, 0xF2, 0x64}, /* params_rssi_lna_out_aci_status_1_alter */
    {0x17, 0xF3, 0x64}, /* params_rssi_lna_out_aci_status_2_alter */
    {0x17, 0xF4, 0x64}, /* params_rssi_lna_out_aci_status_3_alter */
    {0x17, 0xF5, 0x64}, /* params_rssi_lna_out_aci_status_4_alter */

    {0x14, 0x19, 0x03}, /* bb_power_target for aci 0 high byte */
    {0x14, 0x18, 0xA0}, /*  58  */
    {0x14, 0x1B, 0x03}, /* bb_power_target for aci 1 high byte */
    {0x14, 0x1A, 0xA0}, /* 58 */
    {0x14, 0x1D, 0x03}, /* bb_power_target for aci 2 high byte */
    {0x14, 0x1C, 0xA0}, /*  58 */
    {0x14, 0x1F, 0x03}, /* bb_power_target for aci 3 high byte */
    {0x14, 0x1E, 0xA0}, /*  e0 58  */
    {0x14, 0x21, 0x03}, /* bb_power_target for aci 4 high byte */
    {0x14, 0x20, 0xA0}, /*  e0 58 */
};
#define ATBM2040_AGC_POW_TARGET_REG_CNT ATBM2040_ARRAY_NUM(AgcPowerTargetReg)


typedef struct
{
    /*PAPR LIMIT    */
    UINT8 Ana2DigThr;/*0x14,0x11*/
    UINT8 Dig2AnaThr;/*0x14,0x36*/
    UINT8 IntialAnaDigJudge;/*0x16,0x3F*/
    /*BB POWER*/
    UINT8 BBPowerAlphaAna;/*0x14,0x22*/
    UINT8 BBPowerAlphaDig;/*0x14,0x23*/
    UINT8 BBPowerAnaSumNum;/*0x14,0x24*/
    UINT8 BBSumLen;/*0x14,0x25*/

    UINT8 Dagc1AmpRef1Low;/*0x15,0x70*/
    UINT8 Dagc1AmpRef1High;/*0x15,0x71*/
    UINT8 Dagc1AmpRef0Low;/*0x15,0x5D*/
    UINT8 Dagc1AmpRef0High;/*0x15,0x5E*/
    UINT8 Dagc2AmpRef1Low;/*0x15,0x40*/
    UINT8 Dagc2AmpRef1High;/*0x15,0x41*/
    UINT8 Dagc2AmpRef0Low;/*0x13,0xA6*/
    UINT8 Dagc2AmpRef0High;/*0x13,0xA7*/

} DagcAmpRef_t;
static DagcAmpRef_t DagcAmpRefDTMB =
{
    0x08, /*ana2dig thr*/
    0x04, /*ana2dig thr*/
    0x00, /*initial a/d judge*/
    0x3F, /*bb power alpha analog*/
    0x3F, /*agc speed*/
    0x0F, /*bb power sum num analog*/
    0x12, /*bb power sum len*/
    0x80, /*params_dagc1_amp_ref_1[7:0]*/
    0x04, /*params_dagc1_amp_ref_1[9:8]*/
    0x80, /*params_dagc1_amp_ref_0[7:0]*/
    0x04, /*params_dagc1_amp_ref_0[9:8]*/
    0x80, /*params_dagc2_amp_ref_1[7:0]*/
    0x04, /*params_dagc2_amp_ref_1[9:8]*/
    0x80, /*params_dagc2_amp_ref_0[7:0]*/
    0x04, /*params_dagc2_amp_ref_0[9:8]*/
};
static DagcAmpRef_t DagcAmpRefDVBT =
{
    0x08, /*ana2dig thr*/
    0x04, /*ana2dig thr*/
    0x00, /*initial a/d judge*/
    0x30, /*bb power alpha analog*/
    0x30, /*agc speed*/
    0x0F, /*bb power sum num analog*/
    0x12, /*bb power sum len*/
    0x40, /*params_dagc1_amp_ref_1[7:0]*/
    0x04, /*params_dagc1_amp_ref_1[9:8]*/
    0x40, /*params_dagc1_amp_ref_0[7:0]*/
    0x04, /*params_dagc1_amp_ref_0[9:8]*/
    0x40, /*params_dagc2_amp_ref_1[7:0]*/
    0x04, /*params_dagc2_amp_ref_1[9:8]*/
    0x40, /*params_dagc2_amp_ref_0[7:0]*/
    0x04, /*params_dagc2_amp_ref_0[9:8]*/
};
static DagcAmpRef_t DagcAmpRefDVBC =
{
    0x08, /*ana2dig thr*/
    0x04, /*ana2dig thr*/
    0x00, /*initial a/d judge*/
    0x30, /*bb power alpha analog*/
    0x30, /*agc speed*/
    0x0F, /*bb power sum num analog*/
    0x12, /*bb power sum len*/
    0x80, /*params_dagc1_amp_ref_1[7:0]*/
    0x04, /*params_dagc1_amp_ref_1[9:8]*/
    0x80, /*params_dagc1_amp_ref_0[7:0]*/
    0x04, /*params_dagc1_amp_ref_0[9:8]*/
    0x80, /*params_dagc2_amp_ref_1[7:0]*/
    0x04, /*params_dagc2_amp_ref_1[9:8]*/
    0x80, /*params_dagc2_amp_ref_0[7:0]*/
    0x04, /*params_dagc2_amp_ref_0[9:8]*/
};
static DagcAmpRef_t DagcAmpRefAnalog =
{
    0x09, /*ana2dig thr*/
    0x06, /*dig2ana thr*/
    0x00, /*initial a/d judge   */
    0x3F, /*bb power alpha analog*/
    0x3F, /*agc speed*/
    0x1F, /* bb power sum num analog*/
    0x12, /* bb power sum len*/
    0x80, /*params_dagc1_amp_ref_1[7:0]*/
    0x04, /*params_dagc1_amp_ref_1[9:8]*/
    0x80, /*params_dagc1_amp_ref_0[7:0]*/
    0x04, /*params_dagc1_amp_ref_0[9:8]*/
    0x80, /*params_dagc2_amp_ref_1[7:0]*/
    0x04, /*params_dagc2_amp_ref_1[9:8]*/
    0x80, /*params_dagc2_amp_ref_0[7:0]*/
    0x04, /*params_dagc2_amp_ref_0[9:8]*/
};
static ATBM2040Reg_t DagcAmpRefReg[] =
{
    /*the register value will be repleaced by 'DagcAmpRefxxx'*/
    {0x14, 0x11, 0x08},  /* ana2dig thr */
    {0x14, 0x36, 0x04},  /* dig2ana thr */
    {0x16, 0x3F, 0x00}, /* initial a/d judge     */

    {0x14, 0x22, 0x30},  /* bb power alpha analog */
    {0x14, 0x23, 0x30},  /* agc speed */
    {0x14, 0x24, 0x0F},  /* bb power sum num analog */
    {0x14, 0x25, 0x12},  /* bb power sum len */

    {0x15, 0x70, 0x80},  /*params_dagc1_amp_ref_1[7:0]*/
    {0x15, 0x71, 0x04},  /*params_dagc1_amp_ref_1[9:8]*/
    {0x15, 0x5D, 0x80},  /*params_dagc1_amp_ref_0[7:0]*/
    {0x15, 0x5E, 0x04},  /*params_dagc1_amp_ref_0[9:8]*/
    {0x15, 0x40, 0x80},  /*params_dagc2_amp_ref_1[7:0]*/
    {0x15, 0x41, 0x04},  /*params_dagc2_amp_ref_1[9:8]*/
    {0x13, 0xA6, 0x80},  /*params_dagc2_amp_ref_0[7:0]*/
    {0x13, 0xA7, 0x04},  /*params_dagc2_amp_ref_0[9:8]*/
};
#define ATBM2040_DAGC_AMP_REF_REG_CNT ATBM2040_ARRAY_NUM(DagcAmpRefReg)

static ATBM2040Reg_t DTVRSSIReg[] =
{
    {0x16, 0x23, 0x06}, /* rssi lna out alpha n analog ini  */
    {0x16, 0x24, 0x09}, /* rssi lna out alpha n analog tracking */
    {0x16, 0x25, 0x06}, /* rssi lna out alpha n digital ini */
    {0x16, 0x26, 0x09}, /* rssi lna out alpha n digital tracking */
    {0x16, 0x28, 0x00}, /* rssi lna out sum len [7:0] */
    {0x16, 0x29, 0x01}, /* rssi lna out sum len [13:8]  */
    {0x16, 0x2A, 0x18}, /* params_rssi_lna_out_sum_len_db[5:0]  */

    {0x16, 0x2B, 0x06}, /* rssi rf vga2 out alpha n analog ini  */
    {0x16, 0x2C, 0x09}, /* rssi rf vga2 out alpha n analog tracking */
    {0x16, 0x2D, 0x06}, /* rssi rf vga2 out alpha n digital ini */
    {0x16, 0x2E, 0x09}, /* rssi rf vga2 out alpha n digital tracking */
    {0x16, 0x30, 0x00}, /* rssi rf vga2 out sum len [7:0] */
    {0x16, 0x31, 0x01}, /* rssi rf vga2 out sum len [13:8]  */
    {0x16, 0x32, 0x18}, /* params_rssi_rf_vga2_out_sum_len_db[5:0] */
};
#define ATBM2040_DTV_RSSI_REG_CNT ATBM2040_ARRAY_NUM(DTVRSSIReg)

static ATBM2040Reg_t ATVRSSIReg[] =
{
    {0x16, 0x23, 0x06}, /* rssi lna out alpha n analog ini  */
    {0x16, 0x24, 0x07}, /* rssi lna out alpha n analog tracking */
    {0x16, 0x25, 0x06}, /* rssi lna out alpha n digital ini */
    {0x16, 0x26, 0x07}, /* rssi lna out alpha n digital tracking */
    {0x16, 0x28, 0x00}, /* rssi lna out sum len [7:0] */
    {0x16, 0x29, 0x01}, /* rssi lna out sum len [13:8]  */
    {0x16, 0x2A, 0x18}, /* params_rssi_lna_out_sum_len_db[5:0]  */

    {0x16, 0x2B, 0x06}, /* rssi rf vga2 out alpha n analog ini  */
    {0x16, 0x2C, 0x07}, /* rssi rf vga2 out alpha n analog tracking */
    {0x16, 0x2D, 0x06}, /* rssi rf vga2 out alpha n digital ini */
    {0x16, 0x2E, 0x07}, /* rssi rf vga2 out alpha n digital tracking */
    {0x16, 0x30, 0x00}, /* rssi rf vga2 out sum len [7:0] */
    {0x16, 0x31, 0x01}, /* rssi rf vga2 out sum len [13:8]  */
    {0x16, 0x32, 0x18}, /* params_rssi_rf_vga2_out_sum_len_db[5:0] */
};
#define ATBM2040_ATV_RSSI_REG_CNT ATBM2040_ARRAY_NUM(ATVRSSIReg)

static ATBM2040Reg_t DSPMiscReg[] =
{
    {0x17, 0x90, 0x01}, /* params_aci_status_judge_double_thr */
    {0x17, 0x56, 0x6E}, /* params_if_filter_gain_aci_db_analog,need to be checked */
    {0x14, 0x95, 0x30}, /* data 12bit gain */
    {0x14, 0x96, 0x04}, /* params_if_filter_gain_aci_db[6:0] */

    {0x17, 0x46, 0x01}, /*  papr_spec_logic 0:papr 1:spectrum */
    {0x17, 0x41, 0x00}, /* ad judge logic  1:delta power 2:total power/peak power */
    {0x17, 0x40, 0x19}, /* thr delta power */
    {0x17, 0x4E, 0x10}, /*  aci large for aci apec analyssis */
    {0x17, 0x4D, 0x20}, /* sweep_n */
    {0x17, 0x54, 0x00}, /* use old dagc1 ref */
    {0x17, 0x81, 0x1E}, /* params_use_preset_lo_thr[6:0],30 */
    {0x17, 0x82, 0x00}, /* params_lo_selection_preset_value */

    {0x17, 0x96, 0x1E}, /* params_use_preset_lo_thr_dynamic */
    {0x17, 0x83, 0x01}, /* f_signal_user_set */

    {0x17, 0xED, 0x01}, /* params_use_leo */
    {0x17, 0xEE, 0x02}, /* params_uhf_vhf_filter_gain[5:0] */
    {0x17, 0xF6, 0x5A}, /* params_rf_input_power_thr1_mode0 */
    {0x17, 0xF7, 0x64}, /* params_rf_input_power_thr2_mode0 */
    {0x17, 0xF8, 0x5A}, /* params_rf_input_power_thr1_mode1 */
    {0x17, 0xF9, 0x64}, /* params_rf_input_power_thr2_mode1 */
    {0x17, 0xFA, 0x5A}, /* params_rf_input_power_thr1_mode2 */
    {0x17, 0xFB, 0x64}, /* params_rf_input_power_thr2_mode2 */

    {0x18, 0x00, 0x01}, /* params_fir0_bypass */

    {0x19, 0xA4, 0x7F}, /* if_filter_gain_aci_db_lo */
    {0x19, 0xA5, 0x06}, /* if_filter_gain_aci_db_analog_llo */
    {0x19, 0xA6, 0x7E}, /* if_filter_gain_aci_db_analog_hlo */
    {0x19, 0xA7, 0x73}, /* if_filter_gain_aci_db_analog_llo_lo */
    {0x19, 0xA8, 0x7A}, /* if_filter_gain_aci_db_analog_hlo_lo */
    {0x19, 0xB0, 0x2C}, /* inband_aci_db */
};
#define ATBM2040_DSP_MISC_REG_CNT ATBM2040_ARRAY_NUM(DSPMiscReg)

static ATBM2040Reg_t DSPDTVReg[] =
{
    {0x16, 0x1B, 0x31}, /*params_lar2sma_thr[5:0]  lar2sma  -12  -20  -25  -20*/
    {0x16, 0x35, 0x2C}, /*params_sma2lar_thr[5:0]  sma2lar  -14  -25  -20  -25*/
    {0x17, 0x42, 0x00}, /*  bypass notch aci detection */
    {0x14, 0x4E, 0x12}, /* params_lo_change_thr_db[4:0] */

    {0x1A, 0x5A, 0x00}, /* params_use_narrow_band_filter_iq_compensation */
    {0x14, 0x15, 0x00}, /* b_analog_default */
    {0x17, 0x43, 0x20}, /* thr for peak power */

    //{0x1A,0x70, 0x01}, /* bypass lna gain thr change */
    {0x1A, 0x41, 0x01}, /* bypass sfi_1_detect */

};
#define ATBM2040_DSP_DTV_REG_CNT ATBM2040_ARRAY_NUM(DSPDTVReg)

static ATBM2040Reg_t DSPATVReg[] =
{
    {0x16, 0x1B, 0x31}, /*params_lar2sma_thr[5:0]  lar2sma  -12  -20  -25  -20*/
    {0x16, 0x35, 0x27}, /*params_sma2lar_thr[5:0]  sma2lar  -14  -25  -20  -25*/
    {0x17, 0x42, 0x01}, /*  bypass notch aci detection */
    {0x14, 0x4E, 0x10}, /* params_lo_change_thr_db[4:0] */

    /* narw band filter */
    {0x1A, 0x5A, 0x01}, /* params_use_narrow_band_filter_iq_compensation [0]     1   U(1,0) */
    {0x1A, 0x5F, 0x0D}, /* params_iir_filter_gen_bw[7:0] U(16,-8),28; */
    {0x1A, 0x60, 0x03}, /* params_iir_filter_gen_bw[15:8]    ,09 */
    {0x1A, 0x62, 0x57}, /* params_thr_iq_narw_band_filter_l  [6:0]   S(7,0),14 */
    {0x1A, 0x63, 0x5C}, /* params_thr_iq_narw_band_filter_h  [6:0]   S(7,0),19 */

    {0x1A, 0x17, 0x1F}, /* thr_peak */
    {0x1A, 0x18, 0x03}, /* delta_peak */


    {0x14, 0x15, 0x01}, /* b_analog_default */

    /* aci ad judge */
    {0x17, 0x43, 0x25}, /* ad_peak_power_thr */
    {0x1A, 0x6D, 0x04}, /* notch_iir_a_aci */

    //{0x1A,0x70, 0x01}, /* bypass lna gain thr change */
    {0x1A, 0x72, 0x14}, /* thr_change_lna_gain2 20 */
    {0x1A, 0x73, 0x01}, /* thr_change_gain_set */

    {0x1A, 0x78, 0x08}, /* third_thr_lna_l */
    {0x1A, 0x41, 0x00}, /* bypass sfi_1_detect */

    {0x1A, 0x00, 0x00}, /* bypass sfi1 notch filter */
    {0x1A, 0x01, 0x01}, /* bypass sfi2 notch filter */
    {0x1A, 0x54, 0x00}, /* sfi_1_user_set */
    {0x1A, 0x55, 0x00}, /* sfi_2_user_set */
    {0x1A, 0x42, 0x28}, /* sfi_delta_thr */
    {0x1A, 0x43, 0x0A}, /* sfi_peak_thr */

    {0x1A, 0x02, 0xFF}, /* sfi_1_r */
    {0x1A, 0x03, 0x07}, /* sfi_1_r */

    {0x1A, 0x34, 0xC8}, /* sfi_1_freq */
    {0x1A, 0x35, 0x10}, /*params_sfi_detect_freq_1[15:8]*/
    {0x1A, 0x36, 0x00}, /*params_sfi_detect_freq_1[16]*/
};
#define ATBM2040_DSP_ATV_REG_CNT ATBM2040_ARRAY_NUM(DSPATVReg)

static ATBM2040Reg_t VGAGainThrDTMBReg[] =
{
    {0x16, 0xD9, 0x03}, /* lna_rssi_out thr for mode 1  N+- */
    {0x16, 0xDA, 0x04}, /* lna_rssi_out thr for mode 0 sing */
    {0x16, 0xDB, 0x04}, /* lna_rssi_out thr for mode 2  N+- */
    {0x17, 0x91, 0x71}, /* params_thr_aci_status_n_1_large_h */
    {0x17, 0x92, 0x6C}, /* params_thr_aci_status_n_1_large_l */
};
#define ATBM2040_VGA_GAIN_THR_DTMB_REG_CNT ATBM2040_ARRAY_NUM(VGAGainThrDTMBReg)

static ATBM2040Reg_t VGAGainThrDVBTReg[] =
{
    {0x16, 0xD9, 0x03}, /* lna_rssi_out thr for mode 1  N+- */
    {0x16, 0xDA, 0x03}, /* lna_rssi_out thr for mode 0 sing */
    {0x16, 0xDB, 0x03}, /* lna_rssi_out thr for mode 2  N+- */
    {0x17, 0x91, 0x71}, /* params_thr_aci_status_n_1_large_h */
    {0x17, 0x92, 0x6C}, /* params_thr_aci_status_n_1_large_l */
};
#define ATBM2040_VGA_GAIN_THR_DVBT_REG_CNT ATBM2040_ARRAY_NUM(VGAGainThrDVBTReg)

static ATBM2040Reg_t VGAGainThrDVBCReg[] =
{
    {0x16, 0xD9, 0x04}, /* lna_rssi_out thr for mode 1  N+- */
    {0x16, 0xDA, 0x04}, /* lna_rssi_out thr for mode 0 sing */
    {0x16, 0xDB, 0x04}, /* lna_rssi_out thr for mode 2  N+- */
    {0x17, 0x91, 0x74}, /* params_thr_aci_status_n_1_large_h */
    {0x17, 0x92, 0x6F}, /* params_thr_aci_status_n_1_large_l */

};
#define ATBM2040_VGA_GAIN_THR_DVBC_REG_CNT ATBM2040_ARRAY_NUM(VGAGainThrDVBCReg)

static ATBM2040Reg_t VGAGainThrATVReg[] =
{
    {0x16, 0xD9, 0x04}, /* lna_rssi_out thr for mode 1  N+- */
    {0x16, 0xDA, 0x04}, /* lna_rssi_out thr for mode 0 sing */
    {0x16, 0xDB, 0x04}, /* lna_rssi_out thr for mode 2  N+- */
    {0x17, 0x91, 0x71}, /* params_thr_aci_status_n_1_large_h */
    {0x17, 0x92, 0x6C}, /* params_thr_aci_status_n_1_large_l */
};
#define ATBM2040_VGA_GAIN_THR_ATV_REG_CNT ATBM2040_ARRAY_NUM(VGAGainThrATVReg)

static ATBM2040Reg_t LeoEVGAGainThrATVReg[] =
{
    {0x16, 0xD9, 0x04}, /* lna_rssi_out thr for mode 1  N+- */
    {0x16, 0xDA, 0x04}, /* lna_rssi_out thr for mode 0 sing */
    {0x16, 0xDB, 0x04}, /* lna_rssi_out thr for mode 2  N+- */
    {0x17, 0x91, 0x76}, /* params_thr_aci_status_n_1_large_h */
    {0x17, 0x92, 0x6F}, /* params_thr_aci_status_n_1_large_l */
};
#define ATBM2040_LEOE_VGA_GAIN_THR_ATV_REG_CNT ATBM2040_ARRAY_NUM(LeoEVGAGainThrATVReg)

static ATBM2040Reg_t LeoGVGAGainThrATVReg[]=
{
    {0x16,0xD9, 0x04}, /* lna_rssi_out thr for mode 1  N+- */
    {0x16,0xDA, 0x04}, /* lna_rssi_out thr for mode 0 sing */
    {0x16,0xDB, 0x04}, /* lna_rssi_out thr for mode 2  N+- */
    {0x17,0x91, 0x74}, /* params_thr_aci_status_n_1_large_h */
    {0x17,0x92, 0x70}, /* params_thr_aci_status_n_1_large_l */
};
#define ATBM2040_LEOG_VGA_GAIN_THR_ATV_REG_CNT ATBM2040_ARRAY_NUM(LeoGVGAGainThrATVReg)


static ATBM2040Reg_t StandbyReg[] =
{
    {0x00, 0x04, 0x02}, /* i2c_clk_sel */
    {0x00, 0x0B, 0x01}, /* soft_rst_lo */
    {0x04, 0x01, 0x01}, /* lna_ldo_pd */
    {0x08, 0x00, 0x01}, /* rssi_pd */
    {0x09, 0x00, 0x01}, /* if_fltr_pd */
    {0x09, 0x0D, 0x01}, /* if_fltr_core_pd */
    {0x0B, 0x00, 0x01}, /* adc_pd */
    {0x12, 0x22, 0x01}, /* adc_div_ldo_pd */
    {0x0D, 0x00, 0x01}, /* sar_adc_pd */
    {0x0F, 0x09, 0x01}, /* pll_pd_ldo_cp */
    {0x0F, 0x12, 0x01}, /* pll_pd_ldo_vco */
    {0x0F, 0x15, 0x01}, /* pll_pd_ldo_mmd */
    {0x0F, 0x16, 0x01}, /* pll_pd_ldo_dig */
    {0x10, 0x00, 0x01}, /* dac_pd */
    {0x10, 0x02, 0x01}, /* dac_fltr_pd */
    {0x12, 0x0B, 0x01}, /* mixr_ldo_pd */
    {0x12, 0x1F, 0x01}, /* mixr_div_ldo_pd */

    {0x04, 0x0B, 0x03},
    {0x04, 0x07, 0x05},
    {0x04, 0x08, 0x02},
    {0x04, 0x05, 0x0B},
    {0x14, 0x2B, (UINT8) - 36},
    {0x14, 0x2F, 0x00},
    {0x09, 0x1A, 0x00},
    {0x14, 0x2E, (UINT8) - 4},

};
#define ATBM2040_STANDBY_REG_CNT ATBM2040_ARRAY_NUM(StandbyReg)

static ATBM2040Reg_t WakeupReg[] =
{
    {0x04, 0x01, 0x00}, /* lna_ldo_pd */
    {0x08, 0x00, 0x00}, /* rssi_pd */
    {0x09, 0x00, 0x00}, /* if_fltr_pd */
    {0x09, 0x0D, 0x00}, /* if_fltr_core_pd */
    {0x12, 0x22, 0x00}, /* adc_div_ldo_pd */
    {0x0B, 0x00, 0x00}, /* adc_pd */
    {0x0B, 0x25, 0x00}, /* adc_resetn */
    {0x0B, 0x25, 0x01}, /* adc_resetn */
    {0x0D, 0x00, 0x00}, /* sar_adc_pd */
    {0x0F, 0x09, 0x00}, /* pll_pd_ldo_cp */
    {0x0F, 0x12, 0x00}, /* pll_pd_ldo_vco */
    {0x0F, 0x15, 0x00}, /* pll_pd_ldo_mmd */
    {0x0F, 0x16, 0x00}, /* pll_pd_ldo_dig */
    {0x10, 0x00, 0x00}, /* dac_pd */
    {0x10, 0x02, 0x00}, /* dac_fltr_pd */
    {0x12, 0x0B, 0x00}, /* mixr_ldo_pd */
    {0x12, 0x1F, 0x00}, /* mixr_div_ldo_pd */
    {0x00, 0x04, 0x03}, /* i2c_clk_sel */
    {0x00, 0x0B, 0x00}, /* soft_rst_lo */
};
#define ATBM2040_WAKEUP_REG_CNT ATBM2040_ARRAY_NUM(WakeupReg)


static ATBM2040RegGroup_t RxFltBw8MRCReg1 =
{
    0x13, 0x1A,
    {0x0F, 0x79, 0x00, 0x26, 0xBD, 0x7B, 0x00, 0x08, 0xF5, 0x1B, 0x00, 0x00}, /*from 131AH to 1325H*/
    12
};
static ATBM2040RegGroup_t RxFltBw8MRCReg2 =
{
    0x19, 0xAC,
    {0x03, 0x03, 0x02, 0x02}, /*from 19ACH to 19AF*/
    4
};

static ATBM2040RegGroup_t RxFltBw7MRCReg1 =
{
    0x13, 0x1A,
    {0x5B, 0x6A, 0x00, 0x2C, 0x7B, 0x6C, 0x00, 0x0C, 0x36, 0xE8, 0x9C, 0xF5}, /*from 131AH to 1325H*/
    12
};
static ATBM2040RegGroup_t RxFltBw7MRCReg2 =
{
    0x19, 0xAC,
    {0x01, 0x01, 0x01, 0x01}, /*from 19ACH to 19AF*/
    4
};

static ATBM2040RegGroup_t RxFltBw6MRCReg1 =
{
    0x13, 0x1A,
    {0x22, 0x5C, 0x00, 0x38, 0x39, 0x5D, 0x00, 0x10, 0x99, 0x36, 0x00, 0x00}, /*from 131AH to 1325H*/
    12
};
static ATBM2040RegGroup_t RxFltBw6MRCReg2 =
{
    0x19, 0xAC,
    {0x00, 0x00, 0x00, 0x00}, /*from 19ACH to 19AF*/
    4
};

static ATBM2040RegGroup_t AlterThrRssiLnaOutTracking =
{
    0x1A, 0x74,
    {0x05, 0x05, 0x05, 0x05, 0x05, 0x05}, /*from 1A74H to 1A79H*/
    6
};

static ATBM2040RegGroup_t DVBTAciN234Judge =
{
    0x23,0x29,/*from 2329H to 234FH*/
    {
        0xDC,0xD4,0xDE,0xD8,0xDE,0xD8,0x0A,0x0A,0x0F,0x0F,0x0F,0x00,0x00,0x20,0x20,0x7B,
        0x76,0xDD,0xD8,0x14,0x0F,0x7A,0x75,0xDD,0xD8,0x15,0x10,0x18,0x10,0xDD,0xD8,0xEA,
        0xEA,0xF1,0xF1,0xEF,0x0A,0x0A,0x0F
    },
    39
};
static ATBM2040RegGroup_t OtherAciN234Judge =
{
    0x23,0x29,/*from 2329H to 234FH*/
    {
        0xE2,0xDC,0xE8,0xE2,0xE8,0xE2,0x0F,0x0F,0x0F,0x7A,0x74,0xE2,0xD3,0x14,0x0F,0x7B,
        0x76,0xE8,0xE2,0x1E,0x19,0x7A,0x74,0xE8,0xE2,0x1E,0x19,0x20,0x1A,0xEC,0xE6,0x64,
        0x6C,0x6C,0x6C,0x66,0x00,0x00,0x00
    },
    39
};

static UINT8 AciN234CalResultUHF[]=
{
    0x14,0x96, 0xFD,
    0x14,0x97, 0x16,
    0x19,0xA4, 0xF8,
    0x19,0xB0, 0xFF,
    0x22,0xFB, 0x00,
    0x22,0xFC, 0x00,
    0x22,0xFD, 0xE2,
    0x22,0xFE, 0xD1,
    0x22,0xFF, 0xBC,
    0x23,0x0F, 0x00,
    0x23,0x10, 0xEF,
    0x23,0x11, 0xE5,
    0x23,0x12, 0xD5,
    0x23,0x13, 0x00,
    0x23,0x19, 0x13,
    0x23,0x1A, 0x00,
    0x23,0x1B, 0x18,
    0x23,0x1C, 0x00,
    0x23,0x1D, 0x65,
    0x23,0x1E, 0x00,
    0x23,0x1F, 0x00,
    0x23,0x20, 0x00,
    0x23,0x21, 0x00,
    0x23,0x22, 0x00,
    0x23,0x23, 0x4F,
    0x23,0x24, 0x00,
    0x23,0x25, 0x91,
    0x23,0x26, 0x00,
    0x23,0x27, 0x7B,
    0x23,0x28, 0x0D,
    0x23,0xC0, 0x00,
    0x23,0xC1, 0x00,
    0x23,0xC2, 0x00,
    0x23,0xC3, 0x00,
    0x23,0xC4, 0x00,
    0x23,0xC5, 0x00,
    0x23,0xC6, 0x00,
    0x23,0xC7, 0x00,
    0x23,0xC8, 0x00,
    0x23,0xC9, 0x00
};
#define ATBM2040_ACI234_CAL_RESULT_UHF_SIZE ATBM2040_ARRAY_NUM(AciN234CalResultUHF)

static UINT8 AciN234CalResultVHF2[]=
{
    0x14,0x96, 0xFB,
    0x14,0x97, 0x16,
    0x19,0xA4, 0xF4,
    0x19,0xB0, 0x05,
    0x22,0xFB, 0x00,
    0x22,0xFC, 0x00,
    0x22,0xFD, 0xDA,
    0x22,0xFE, 0xC8,
    0x22,0xFF, 0xB5,
    0x23,0x0F, 0x00,
    0x23,0x10, 0xEC,
    0x23,0x11, 0xD6,
    0x23,0x12, 0xC7,
    0x23,0x13, 0x00,
    0x23,0x19, 0x0B,
    0x23,0x1A, 0x00,
    0x23,0x1B, 0x15,
    0x23,0x1C, 0x00,
    0x23,0x1D, 0x3B,
    0x23,0x1E, 0x00,
    0x23,0x1F, 0x00,
    0x23,0x20, 0x00,
    0x23,0x21, 0x00,
    0x23,0x22, 0x00,
    0x23,0x23, 0x1C,
    0x23,0x24, 0x00,
    0x23,0x25, 0x43,
    0x23,0x26, 0x00,
    0x23,0x27, 0x3E,
    0x23,0x28, 0x00,
    0x23,0xC0, 0x00,
    0x23,0xC1, 0x00,
    0x23,0xC2, 0x00,
    0x23,0xC3, 0x00,
    0x23,0xC4, 0x00,
    0x23,0xC5, 0x00,
    0x23,0xC6, 0x00,
    0x23,0xC7, 0x00,
    0x23,0xC8, 0x00,
    0x23,0xC9, 0x00
};
#define ATBM2040_ACI234_CAL_RESULT_VHF2_SIZE ATBM2040_ARRAY_NUM(AciN234CalResultVHF2)

static UINT8 AciN234CalResultVHF1[]=
{
    0x14,0x96, 0xF9,
    0x14,0x97, 0x18,
    0x19,0xA4, 0xF3,
    0x19,0xB0, 0x04,
    0x22,0xFB, 0x00,
    0x22,0xFC, 0x00,
    0x22,0xFD, 0xE4,
    0x22,0xFE, 0xD8,
    0x22,0xFF, 0xC9,
    0x23,0x0F, 0x00,
    0x23,0x10, 0xF6,
    0x23,0x11, 0xE7,
    0x23,0x12, 0xDC,
    0x23,0x13, 0x00,
    0x23,0x19, 0x0A,
    0x23,0x1A, 0x00,
    0x23,0x1B, 0x1E,
    0x23,0x1C, 0x00,
    0x23,0x1D, 0x5E,
    0x23,0x1E, 0x00,
    0x23,0x1F, 0x00,
    0x23,0x20, 0x00,
    0x23,0x21, 0x00,
    0x23,0x22, 0x00,
    0x23,0x23, 0x2B,
    0x23,0x24, 0x00,
    0x23,0x25, 0x6A,
    0x23,0x26, 0x00,
    0x23,0x27, 0x60,
    0x23,0x28, 0x00,
    0x23,0xC0, 0x00,
    0x23,0xC1, 0x00,
    0x23,0xC2, 0x00,
    0x23,0xC3, 0x00,
    0x23,0xC4, 0x00,
    0x23,0xC5, 0x00,
    0x23,0xC6, 0x00,
    0x23,0xC7, 0x00,
    0x23,0xC8, 0x00,
    0x23,0xC9, 0x00
};
#define ATBM2040_ACI234_CAL_RESULT_VHF1_SIZE ATBM2040_ARRAY_NUM(AciN234CalResultVHF1)

static UINT8 AciN234CalResult[]=
{
    0x14,0x96, 0x04,
    0x14,0x97, 0x15,
    0x19,0xA4, 0x7F,
    0x19,0xB0, 0x2C,
    0x22,0xFB, 0xFF,
    0x22,0xFC, 0xF6,
    0x22,0xFD, 0xD7,
    0x22,0xFE, 0xC3,
    0x22,0xFF, 0xB4,
    0x23,0x0F, 0xFF,
    0x23,0x10, 0xFB,
    0x23,0x11, 0xDE,
    0x23,0x12, 0xC7,
    0x23,0x13, 0xB7,
    0x23,0x19, 0x07,
    0x23,0x1A, 0x00,
    0x23,0x1B, 0x07,
    0x23,0x1C, 0x00,
    0x23,0x1D, 0x14,
    0x23,0x1E, 0x00,
    0x23,0x1F, 0x74,
    0x23,0x20, 0x00,
    0x23,0x21, 0x09,
    0x23,0x22, 0x00,
    0x23,0x23, 0x20,
    0x23,0x24, 0x00,
    0x23,0x25, 0xA0,
    0x23,0x26, 0x00,
    0x23,0x27, 0xA4,
    0x23,0x28, 0x01,
    0x23,0xC0, 0x1E,
    0x23,0xC1, 0x00,
    0x23,0xC2, 0x00,
    0x23,0xC3, 0x00,
    0x23,0xC4, 0x00,
    0x23,0xC5, 0x00,
    0x23,0xC6, 0x1E,
    0x23,0xC7, 0x1C,
    0x23,0xC8, 0x1A,
    0x23,0xC9, 0x18
};
#define ATBM2040_ACI234_CAL_RESULT_SIZE ATBM2040_ARRAY_NUM(AciN234CalResult)


static ATBM2040Reg_t FEFInternalReg[]=
{
    {0x16, 0x00, 0x01}, /* params_add_fef_detect    [0]    Read write       1'b1 */
    {0x16, 0x12, 0xA0}, /* params_fef_detect_length[7:0]         [7:0] Read write       8'b1010_0000 */
    {0x16, 0x13, 0x04}, /* params_fef_detect_length[15:8]      [7:0] Read write       8'b0000_1111 */
    {0x16, 0x14, 0x01}, /* params_fef_freeze    [0]    Read write       1'b0 */
    {0x16, 0x15, 0x4B}, /* params_fef_max_length_samples[7:0]     [7:0] Read write       8'b1000_0110 */
    {0x16, 0x16, 0x4C}, /* params_fef_max_length_samples[15:8]   [7:0] Read write       8'b0000_0001 */
    {0x16, 0x17, 0x00}, /* params_fef_sum_len[7:0]  [7:0] Read write      8'b0000_0000 */
    {0x16, 0x18, 0x04}, /* params_fef_sum_len[15:8]        [7:0] Read write       8'b0000_0100 */
    {0x16, 0x19, 0x00}, /* params_fef_use_mode[2:0]       [2:0] Read write       3'b000 */
    {0x16, 0x39, 0x0A}, /* params_thr_fef_judge[5:0]        [5:0] Read write       6'b00_1010 */
    {0x16, 0x3A, 0x0A}, /* params_thr_fef_judge_aci[5:0] [5:0] Read write      6'b00_1010 */
};
#define ATBM2040_FEF_INTERNAL_REG_CNT ATBM2040_ARRAY_NUM(FEFInternalReg)

#if ATBM2040_FIRMWARE_LOAD
static ATBM2040Firmware_t ATBM2040Firmware =
{
    0x21,0x00,
    {
        0xA7,0x4A,0xC0,0x68,0x00,0xA7,0x4A,0xC0,0x70,0x05,0x5F,0xFF,0x44,0x01,0x08,0xD0,0x02,0x58,0x11,0x70,0x0A,0xAB,0x4C,0x26,0x44,0x14,0x15,0x70,0x00,0x17,0x87,0x42,
        0x70,0x5F,0xFF,0xA3,0x4A,0xA1,0x44,0x02,0x11,0x70,0x21,0x0E,0x2C,0xA2,0x02,0x21,0x44,0x01,0x0A,0x82,0x02,0x20,0x5F,0xFF,0x5F,0xFF,0x0A,0x82,0x02,0x20,0x5F,0xFF,
        0x5F,0xFF,0x5F,0xFF,0xA7,0x4B,0x60,0x70,0x42,0x0E,0x5F,0xFF,0x5F,0xFF,0x44,0x01,0x08,0xD0,0x02,0x58,0x11,0x70,0x4A,0x28,0x44,0x01,0x08,0x15,0x68,0x68,0x44,0x02,
        0x15,0x68,0x72,0x44,0x03,0x15,0x68,0x7C,0x2C,0xBB,0x4D,0x0B,0x93,0x69,0x70,0x28,0x60,0x2C,0x2C,0xBB,0x4D,0x0B,0x93,0x69,0x90,0x28,0x60,0x2C,0xBB,0x4D,0x0B,0x93,
        0x69,0xB0,0xBD,0x69,0x9B,0x10,0x39,0x2C,0x29,0xBB,0x69,0x7B,0x10,0x39,0x2C,0x29,0x2D,0x28,0x44,0xA0,0x09,0x2C,0x29,0x2D,0xA2,0x02,0x21,0x44,0x01,0x0A,0x82,0x02,
        0x20,0x5F,0xFF,0x5F,0xFF,0x0A,0x82,0x02,0x20,0x5F,0xFF,0xA7,0x4B,0x60,0x70,0xA9,0x0E,0x5F,0xFF,0x5F,0xFF,0x44,0x01,0x08,0xD0,0x4E,0x20,0x11,0x70,0xB1,0xBB,0x4D,
        0x0B,0x29,0x11,0x3B,0x1E,0x8B,0x42,0x70,0x68,0x00,0x60,0x98
    },/**/
    204
};

static ATBM2040Firmware_t ATBM2040GFirmware =
{
    0x21,0x00,
    {
        0xA0,0x51,0x25,0x1F,0xA7,0x4A,0xC0,0x5F,0xFF,0x70,0x04,0xA7,0x21,0x30,0x70,0xC6,0xA3,0x21,0xA7,0x44,0x40,0x0C,0x15,0x70,0xC4,0xA3,0x4D,0xF2,0x16,0x22,0x70,0x35,
        0x83,0x21,0x90,0x44,0x00,0x15,0x68,0x8D,0x70,0xAE,0x1E,0xA3,0x21,0x92,0x44,0x04,0x15,0x68,0x55,0x70,0x75,0xA5,0x21,0x92,0x14,0x70,0x46,0x44,0x00,0x15,0x68,0x8D,
        0xA3,0x21,0x92,0x15,0x68,0xAE,0xA3,0x4D,0xF2,0x44,0x04,0x15,0x68,0x50,0x70,0x70,0xA6,0x51,0x60,0x68,0xC4,0x0F,0x82,0x51,0x60,0xA3,0x21,0xA5,0x82,0x51,0x20,0x42,
        0x0B,0x82,0x51,0x50,0x42,0x17,0x82,0x50,0xC0,0x42,0x12,0x83,0x7E,0xE0,0x60,0xC4,0xA6,0x51,0x60,0x70,0xC4,0x0E,0x82,0x51,0x60,0x80,0x51,0x20,0x42,0x08,0x82,0x51,
        0x50,0x42,0x1F,0x82,0x50,0xC0,0x42,0x02,0x83,0x7E,0xE0,0x60,0xC4,0x16,0x86,0x91,0xC0,0x86,0x90,0x50,0x17,0x87,0x22,0xD0,0xA2,0x90,0xF5,0x44,0x04,0x09,0x82,0x91,
        0xC0,0x17,0x86,0x90,0x50,0x16,0x87,0x22,0xD0,0x87,0x21,0x90,0x60,0xC0,0x16,0x86,0x91,0xC0,0x86,0x90,0x50,0x17,0x87,0x22,0xD0,0x87,0x21,0x90,0x16,0x87,0x22,0xD0,
        0x16,0x22,0x68,0x2A,0x60,0x04,0x0E,0x82,0x00,0xD0,0x42,0x02,0x82,0x00,0x40,0x0E,0x82,0xF2,0x10,0x82,0xF1,0xF0,0x50,0x0F,0x0F,0x82,0xF2,0x10,0x82,0xF1,0xF0,0x5F,
        0xFF,0x5F,0xFF,0x82,0x00,0x70,0x82,0x00,0xB0,0x42,0x07,0x82,0x00,0xD0,0x42,0x03,0x82,0x00,0x40,0x0E,0x82,0x00,0x70,0x82,0x00,0xB0,0x60,0x04
    },/**/
    252
};

static UINT8 FuZuDataVHF2[] =
{
                                                                                        0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,
        0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x42,0x0B,0x83,0x7E,0xE0,0x60,0xC4,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,
        0x01,0x01,0x01,0x01,0x01,0x01
};
#define ATBM2040_FUZU_DATA_VHF2_SIZE ATBM2040_ARRAY_NUM(FuZuDataVHF2)

static UINT8 FuZuDataUHF[] =
{
                                                                                        0xA6,0x51,0x60,0x68,0xC4,0x0F,0x82,0x51,0x60,0xA3,0x21,0xA5,0x82,0x51,0x20,0x42,
        0x0B,0x82,0x51,0x50,0x42,0x17,0x82,0x50,0xC0,0x42,0x12,0x83,0x7E,0xE0,0x60,0xC4,0xA6,0x51,0x60,0x70,0xC4,0x0E,0x82,0x51,0x60,0x80,0x51,0x20,0x42,0x08,0x82,0x51,
        0x50,0x42,0x1F,0x82,0x50,0xC0
};
#define ATBM2040_FUZU_DATA_UHF_SIZE ATBM2040_ARRAY_NUM(FuZuDataUHF)


#endif
#define ATBM2040_BURST_BUFF_MAX (45)
static UINT8 ATBM2040BurstBuff[ATBM2040_BURST_BUFF_MAX + 2] = {0};


#if 0
static SINT32  FixPow(SINT32 x, UINT32 y)
{
    SINT32 i32Rst = x;
    UINT32 i;
    if (y == 0)
    {
        return 1;
    }

    for (i = 1; i < y; i++)
    {
        i32Rst *= x;
    }
    return i32Rst;
}

/**********************if add calcu overflow ,use this function*************************/
static void ATBM64Add(ATBM2040Data64_t *pstSum, UINT32 ui32Addend)
{
    UINT32 ui32LowPart;
    ui32LowPart = pstSum->Low;
    pstSum->Low += ui32Addend;
    if (ui32LowPart > pstSum->Low)
    {
        pstSum->High++;
    }
}
#endif

/**********************2 32bit data multipe,then use the next fuction *****************/
static void ATBM64Mult(UINT8 u8TunerIndex, ATBM2040Data64_t *pstRst, UINT32 m1, UINT32 m2)
{
    UINT32 ui32LowPart, ui32Tmp;
    pstRst->Low = (m1 & 0xFFFF) * (m2 & 0xFFFF);
    pstRst->High = (m1 >> 16) * (m2 >> 16);

    ui32LowPart = pstRst->Low;
    ui32Tmp = (m1 >> 16) * (m2 & 0xFFFF);
    pstRst->Low  += (ui32Tmp << 16);
    pstRst->High += (ui32Tmp >> 16);
    if (ui32LowPart > pstRst->Low)
    {
        pstRst->High++;
    }

    ui32LowPart = pstRst->Low;
    ui32Tmp = (m2 >> 16) * (m1 & 0xFFFF);
    pstRst->Low  += (ui32Tmp << 16);
    pstRst->High += (ui32Tmp >> 16);
    if (ui32LowPart > pstRst->Low)
    {
        pstRst->High++;
    }
}

static UINT8  ATBM64ComPare(UINT8 u8TunerIndex, ATBM2040Data64_t stPara, ATBM2040Data64_t stPara1)
{
    UINT8 ui8Flag = 0;/* default :stPara<=stPara1 */
    if (stPara.Low >= stPara1.Low)
    {
        if (stPara.High >= stPara1.High)
        {
            ui8Flag = 1;
        }
    }
    else
    {
        if (stPara.High > stPara1.High)
        {
            ui8Flag = 1;
        }
    }
    return ui8Flag;

}
static UINT32 ATBM64Div(UINT8 u8TunerIndex, ATBM2040Data64_t stDivisor, ATBM2040Data64_t stDividend)
{
    UINT8  ui8DivLeftShit = 0;
    UINT32 ui32Rst = 0;
    UINT32 ui32flag = 1;
    ATBM2040Data64_t stDataTmp = stDividend;
    if (stDivisor.High == 0 && stDivisor.Low == 0)
    {
        return 0;
    }
    else if (stDividend.High == 0 && stDividend.Low == 0)
    {
        return 0;
    }
    else if (stDividend.High > stDivisor.High)
    {
        return 0;
    }
    else if (!ATBM64ComPare(u8TunerIndex, stDivisor, stDividend))
    {
        return 0;
    }
    else if (stDividend.High == 0 && stDivisor.High == 0)
    {
        return stDivisor.Low / stDividend.Low;
    }
    else/*  normal condition  stDivisor >= stDividend */
    {
        while (ui32flag)
        {
            ++ui8DivLeftShit;
            stDataTmp.High <<= 1;
            if (stDataTmp.Low & 0x80000000)
            {
                stDataTmp.High += 1;
            }
            stDataTmp.Low <<= 1;

            if (!ATBM64ComPare(u8TunerIndex, stDivisor, stDataTmp) || stDataTmp.High >= 0x80000000)
            {
                if (stDataTmp.High < 0x80000000)
                {
                    stDataTmp.Low >>= 1;
                    if (stDataTmp.High & 1)
                    {
                        stDataTmp.Low |= 0x80000000;
                    }
                    stDataTmp.High >>= 1;
                    ui8DivLeftShit--;
                }


                stDivisor.High -= stDataTmp.High;
                if (stDivisor.Low >= stDataTmp.Low)
                {
                    stDivisor.Low -= stDataTmp.Low;
                }
                else
                {
                    stDivisor.Low += 0xFFFFFFFF - stDataTmp.Low;
                    stDivisor.Low += 1;
                    stDivisor.High -= 1;
                }
                ui32Rst = (1 << ui8DivLeftShit);;
                break;
            }
        }
        ui32Rst += ATBM64Div(u8TunerIndex, stDivisor, stDividend);
    }
    return ui32Rst;
}

#if 0
static ATBM2040Data64_t ATBM64DivReturn64(ATBM2040Data64_t stDivisor, ATBM2040Data64_t stDividend)
{
    UINT8  ui8DivLeftShit = 0;
    UINT32 ui32LowPart;
    UINT32 ui32flag = 1;
    ATBM2040Data64_t i64Data, i64Data1;
    ATBM2040Data64_t stDataTmp = stDividend;
    i64Data.High = 0;
    i64Data.Low = 0;
    if (stDivisor.High == 0 && stDivisor.Low == 0)
    {
        return i64Data;
    }
    else if (stDividend.High == 0 && stDividend.Low == 0)
    {
        return i64Data;
    }
    else if (stDividend.High > stDivisor.High)
    {
        return i64Data;
    }
    else if (!ATBM64ComPare(stDivisor, stDividend))
    {
        return i64Data;
    }
    else if (stDividend.High == 0 && stDivisor.High == 0)
    {
        i64Data.Low = stDivisor.Low / stDividend.Low;
        return i64Data;
    }
    else/*  normal condition  stDivisor >= stDividend */
    {
        while (ui32flag)
        {
            ++ui8DivLeftShit;
            stDataTmp.High <<= 1;
            if (stDataTmp.Low & 0x80000000)
            {
                stDataTmp.High += 1;
            }
            stDataTmp.Low <<= 1;
            if (!ATBM64ComPare(stDivisor, stDataTmp) || stDataTmp.High >= 0x80000000)
            {
                if (stDataTmp.High < 0x80000000)
                {
                    stDataTmp.Low >>= 1;
                    if (stDataTmp.High & 1)
                    {
                        stDataTmp.Low |= 0x80000000;
                    }
                    stDataTmp.High >>= 1;
                    ui8DivLeftShit--;
                }


                stDivisor.High -= stDataTmp.High;
                if (stDivisor.Low >= stDataTmp.Low)
                {
                    stDivisor.Low -= stDataTmp.Low;
                }
                else
                {
                    stDivisor.Low += 0xFFFFFFFF - stDataTmp.Low;
                    stDivisor.Low += 1;
                    stDivisor.High -= 1;
                }
                if (ui8DivLeftShit >= 32)
                {
                    i64Data.High = (1 << (ui8DivLeftShit - 32));
                    i64Data.Low = 0;
                }
                else
                {
                    i64Data.Low = (1 << ui8DivLeftShit);
                    i64Data.High = 0;
                }
                break;
            }
        }
        i64Data1 = ATBM64DivReturn64(stDivisor, stDividend);
        ui32LowPart = i64Data.Low;
        i64Data.Low += i64Data1.Low;
        i64Data.High += i64Data1.High;
        if (ui32LowPart > i64Data.Low)
        {
            i64Data.High += 1;
        }

    }
    return i64Data;
}
#endif


ATBM2040_ERROR_e ATBM2040RegWrite(UINT8 u8TunerIndex, UINT8 I2CAddr, UINT8 BaseReg, UINT8 OffReg, UINT8 Value)
{
    return ATBM2040I2CWrite(u8TunerIndex, I2CAddr, BaseReg, OffReg, &Value, 1);
}

ATBM2040_ERROR_e ATBM2040RegRead(UINT8 u8TunerIndex, UINT8 I2CAddr, UINT8 BaseReg, UINT8 OffReg, UINT8 *pValue)
{
    return ATBM2040I2CRead(u8TunerIndex, I2CAddr, BaseReg, OffReg, pValue, 1);
}


/************************************************************
*Description: Latch on or off for some read only registers. After latch on, registers will not
*           refresh until latch off
*************************************************************/
ATBM2040_ERROR_e ATBM2040DrvRegLatch(UINT8 u8TunerIndex, UINT8 I2CAddr, BOOL OnOff)
{
    return ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x01, 0x0D, (OnOff) ? (0x01) : (0x00)); /*ro_latch*/
}

/************************************************************
*Description: Find mixer divider according to center frequency
*************************************************************/
void ATBM2040DrvNLOMixer(UINT8 u8TunerIndex, ATBM2040_t *pTuner)
{
    UINT32 i = 0, MixDivCnt = 0;
    ATBM2040NLOMixer_t *pNLOMixer = NULL;
#define MIXER_DIVIDER_SEARCH()  for(i=0;i<MixDivCnt;i++)\
    {\
        if((pNLOMixer[i].FreqStart <= pTuner->FreqKHz)&&(pTuner->FreqKHz <= pNLOMixer[i].FreqEnd))\
        {\
            break;\
        }\
    }\
    if(i >= MixDivCnt)\
    {\
        i = MixDivCnt-1;\
    }

    if ((ATBM_LEOG_CHIP_ID == pTuner->ChipID) || (ATBM_LEOE_CHIP_ID == pTuner->ChipID)
            || (ATBM_LEOF_CHIP_ID == pTuner->ChipID))
    {
        pNLOMixer = NLOMixerLeoEHMD;
        MixDivCnt = ATBM2040_NLO_MIXER_LEOE_HMD_CNT;
        MIXER_DIVIDER_SEARCH();
        pTuner->pNLOMixerHMD = (pNLOMixer + i);

        pNLOMixer = NLOMixerLeoELMD;
        MixDivCnt = ATBM2040_NLO_MIXER_LEOE_LMD_CNT;
        MIXER_DIVIDER_SEARCH();
        pTuner->pNLOMixerLMD = (pNLOMixer + i);

        if (pTuner->HighMD)
        {
            pTuner->pNLOMixer = pTuner->pNLOMixerHMD;
        }
        else
        {
            pTuner->pNLOMixer = pTuner->pNLOMixerLMD;
        }
    }
    else
    {
        pNLOMixer = NLOMixer;
        MixDivCnt = ATBM2040_NLO_MIXER_CNT;
        MIXER_DIVIDER_SEARCH();
        pTuner->pNLOMixer = pTuner->pNLOMixerHMD = pTuner->pNLOMixerLMD = (pNLOMixer + i);
    }

}

/************************************************************
*Description: Calculate PLLDiv according to center frequency and LO selection
*************************************************************/
void ATBM2040DrvPLLDivCal(UINT8 u8TunerIndex, ATBM2040_t *pTuner, UINT8 *pNInt, UINT32 *pNFrac, BOOL HighMD)
{
    ATBM2040NLOMixer_t *pNLOMixer = NULL;
    UINT32 IFKHz = pTuner->MixerIF / 1000;
    UINT32 Fref = 24000; /*24 MHz*/
    ATBM2040Data64_t i64DataF, i64DataF1;
    UINT32  Product = 0, Remainder = 0;
    UINT32 CenterFreqKHz = pTuner->FreqKHz; /* Frequency calculation should be less KHz */

    Fref = pTuner->Config.OSCFreqKHz;
    if (0 == pTuner->PllDivMode)
    {
        Fref *= 2;
    }

    if (HighMD)
    {
        pNLOMixer = pTuner->pNLOMixerHMD;
        Product = (CenterFreqKHz + IFKHz) * pNLOMixer->NLO * pNLOMixer->NMixer ;/*  NLO *  NMixer < 256 */
    }
    else
    {
        pNLOMixer = pTuner->pNLOMixerLMD;
        Product = (CenterFreqKHz - IFKHz) * pNLOMixer->NLO * pNLOMixer->NMixer ;/*  NLO *  NMixer < 256 */
    }

    *pNInt = (UINT8)(Product / Fref);

    Remainder = Product % Fref;
    ATBM64Mult(u8TunerIndex, &i64DataF, Remainder, 0x1000000/*FixPow(2, 24)*/); /* KHz */
    i64DataF1.High = 0;
    i64DataF1.Low = Fref;
    *pNFrac = ATBM64Div(u8TunerIndex, i64DataF, i64DataF1);
}

/************************************************************
*Description: Find ADC divider according to center frequency and LO selection
*************************************************************/
ATBM2040ADCDivider_t *ATBM2040DrvADCDivider(UINT8 u8TunerIndex, UINT32 FreqKHz, BOOL HMD)
{
    UINT32 i = 0;
    UINT32 loopNum = 0;
    ATBM2040ADCDivider_t *pDivider = NULL;
    if ((ATBM_LEOG_CHIP_ID == ATBM2040ChipID) || (ATBM_LEOF_CHIP_ID == ATBM2040ChipID)
            || (ATBM_LEOE_CHIP_ID == ATBM2040ChipID))
    {
        pDivider = LeoEADCDividerHMD;
        if (HMD)
        {
            pDivider = LeoEADCDividerHMD;
            loopNum = ATBM2040_LEOE_ADC_DIVIDER_HMD_CNT;
        }
        else
        {
            pDivider = LeoEADCDividerLMD;
            loopNum = ATBM2040_LEOE_ADC_DIVIDER_LMD_CNT;
        }
    }
    else
    {
        pDivider = ADCDividerHMD;
        if (HMD)
        {
            pDivider = ADCDividerHMD;
            loopNum = ATBM2040_ADC_DIVIDER_HMD_CNT;
        }
        else
        {
            pDivider = ADCDividerLMD;
            loopNum = ATBM2040_ADC_DIVIDER_LMD_CNT;
        }
    }
    for (i = 0; i < loopNum; i++)
    {
        if ((pDivider[i].FreqStart < FreqKHz) && (FreqKHz <= pDivider[i].FreqEnd))
        {
            return (pDivider + i);
        }
    }

    pDivider = pDivider + (loopNum - 1);

    return pDivider;
}

/************************************************************
*Description: Find UHF channel index according to center frequency
*************************************************************/
static ATBM2040UHFChIndex_t *ATBM2040DrvUHFFreqIndex(UINT8 u8TunerIndex, UINT32 FreqKHz)
{
    UINT32 i = 0;
    UINT32 center = 0;
    ATBM2040UHFChIndex_t *pIndex = NULL;
    UINT32 FreqMHz = FreqKHz / 1000;

    pIndex = UHFFreqIndex;
    if (FreqMHz >= pIndex->FreqMHz)
    {
        return pIndex;
    }
    pIndex = &UHFFreqIndex[ATBM2040_UHF_FREQ_INDEX_CNT - 1];
    if (FreqMHz <= pIndex->FreqMHz)
    {
        return pIndex;
    }
    pIndex = UHFFreqIndex;
    for (i = 0; i < ATBM2040_UHF_FREQ_INDEX_CNT - 1; i++)
    {
        if ((FreqMHz <= pIndex->FreqMHz) && (FreqMHz > (pIndex + 1)->FreqMHz))
        {
            break;
        }
        pIndex++;
    }
    center = (pIndex->FreqMHz + (pIndex + 1)->FreqMHz) / 2;
    if (FreqMHz < center)
    {
        /*near to next index*/
        pIndex++;
    }
    return pIndex;
}

/************************************************************
*Description: Find VHF2 channel index according to center frequency
*************************************************************/
static ATBM2040VHF2ChIndex_t *ATBM2040DrvVHF2FreqIndex(UINT8 u8TunerIndex, UINT32 FreqKHz)
{
    UINT32 i = 0;
    UINT32 center = 0;
    ATBM2040VHF2ChIndex_t *pIndex = NULL;
    UINT32 FreqMHz = FreqKHz / 1000;

    pIndex = VHF2FreqIndex;
    if (FreqMHz <= pIndex->FreqMHz)
    {
        return pIndex;
    }
    pIndex = &VHF2FreqIndex[ATBM2040_VHF2_FREQ_INDEX_CNT - 1];
    if (FreqMHz >= pIndex->FreqMHz)
    {
        return pIndex;
    }
    pIndex = VHF2FreqIndex;
    for (i = 0; i < ATBM2040_VHF2_FREQ_INDEX_CNT - 1; i++)
    {
        if ((FreqMHz >= pIndex->FreqMHz) && (FreqMHz < (pIndex + 1)->FreqMHz))
        {
            break;
        }
        pIndex++;
    }
    center = (pIndex->FreqMHz + (pIndex + 1)->FreqMHz) / 2;
    if (FreqMHz < center)
    {
        /*near to next index*/
        pIndex++;
    }
    return pIndex;
}


/************************************************************
*Description: Find VHF channel index according to center frequency
*************************************************************/
static ATBM2040VHFChIndex_t *ATBM2040DrvVHFFreqIndex(UINT8 u8TunerIndex, UINT32 FreqKHz)
{
    UINT32 i = 0;
    UINT32 center = 0;
    ATBM2040VHFChIndex_t *pIndex = NULL;

    pIndex = VHFChannelIndex;
    if (FreqKHz <= pIndex->FreqKHz)
    {
        return pIndex;
    }
    pIndex = &VHFChannelIndex[ATBM2040_VHF_CH_INDEX_CNT - 1];
    if (FreqKHz >= pIndex->FreqKHz)
    {
        return pIndex;
    }
    pIndex = VHFChannelIndex;
    for (i = 0; i < ATBM2040_VHF_CH_INDEX_CNT - 1; i++)
    {
        if ((FreqKHz >= pIndex->FreqKHz) && (FreqKHz < (pIndex + 1)->FreqKHz))
        {
            break;
        }
        pIndex++;
    }
    center = (pIndex->FreqKHz + (pIndex + 1)->FreqKHz) / 2;
    if (FreqKHz > center)
    {
        /*near to next index*/
        pIndex++;
    }
    return pIndex;
}

/************************************************************
*Description: Find VHF1 channel index according to center frequency
*************************************************************/
static ATBM2040VHF1ChIndex_t *ATBM2040DrvVHF1FreqIndex(UINT8 u8TunerIndex, UINT32 FreqKHz)
{
    UINT32 i = 0;
    UINT32 center = 0;
    ATBM2040VHF1ChIndex_t *pIndex = NULL;

    pIndex = VHF1ChannelIndex;
    if (FreqKHz <= pIndex->FreqKHz)
    {
        return pIndex;
    }
    pIndex = &VHF1ChannelIndex[ATBM2040_VHF1_CH_INDEX_CNT - 1];
    if (FreqKHz >= pIndex->FreqKHz)
    {
        return pIndex;
    }
    pIndex = VHF1ChannelIndex;
    for (i = 0; i < ATBM2040_VHF1_CH_INDEX_CNT - 1; i++)
    {
        if ((FreqKHz >= pIndex->FreqKHz) && (FreqKHz < (pIndex + 1)->FreqKHz))
        {
            break;
        }
        pIndex++;
    }
    center = (pIndex->FreqKHz + (pIndex + 1)->FreqKHz) / 2;
    if (FreqKHz > center)
    {
        /*near to next index*/
        pIndex++;
    }
    return pIndex;
}


/************************************************************
*Description: PLL and Mixer divider setting
*************************************************************/
static ATBM2040_ERROR_e ATBM2040DrvPLLMixDivSet(UINT8 u8TunerIndex, ATBM2040_t *pTuner)
{
    ATBM2040_ERROR_e ret = ATBM2040_NO_ERROR;
    UINT8 PLLDivInt = 0, PLLDivInt2 = 0;
    UINT32 PLLDivFrac = 0, PLLDivFrac2 = 0;
    UINT8 reg121c = 0, reg121e = 0;
    UINT8 reg1215 = 0, reg1240 = 0;
    UINT8 val = 0;
    UINT8 mixr_pll_mixr_div_mode_l = 0, mixr_pll_mixr_div_mode_h = 0;

    UINT8 PllDivComp = 0;
    UINT8 I2CAddr = pTuner->Config.I2CAddr;

    /*PLL divider calculate*/
    ATBM2040DrvPLLDivCal(u8TunerIndex, pTuner, &PLLDivInt, &PLLDivFrac, pTuner->HighMD);
    ATBM2040DrvPLLDivCal(u8TunerIndex, pTuner, &PLLDivInt2, &PLLDivFrac2, !pTuner->HighMD);


    if (PLLDivInt > 160)
    {
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x0F, 0x2C, 0x00); /* pll freq rang adjust */
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x0F, 0x2D, 0x07); /* pll freq rang adjust */
    }
    else
    {
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x0F, 0x2C, 0x01); /* pll freq rang adjust */
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x0F, 0x2D, 0x06); /* pll freq rang adjust */
    }
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x0F, 0x04, 0x00);

    if (PLLDivInt > 160)
    {
        PllDivComp = 0x07;
    }
    else if (PLLDivInt > 107)
    {
        PllDivComp = 0x06;
    }
    else if (PLLDivInt > 87)
    {
        PllDivComp = 0x05;
    }
    else if (PLLDivInt > 80)
    {
        PllDivComp = 0x04;
    }
    else if (PLLDivInt > 78)
    {
        PllDivComp = 0x01;
    }
    else
    {
        PllDivComp = 0x00;
    }

    PllDivComp |= 0x10;
    if ((ATBM_LEOG_CHIP_ID == pTuner->ChipID)
    || (ATBM_LEOF_CHIP_ID == pTuner->ChipID) || (ATBM_LEOE_CHIP_ID == pTuner->ChipID)
    || (ATBM_LEOD_CHIP_ID == pTuner->ChipID) || (ATBM_LEOC_CHIP_ID == pTuner->ChipID))
    {
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x0F, 0x39, (PllDivComp >> 4) & 0x01); /*pll_sivco*/
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x0F, 0x3A, PllDivComp & 0x07); /*pll_divco[2:0]*/
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x0F, 0x3B, 0x02); /*pll_vref_ctrl[1:0]*/
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x0F, 0x3C, 0x00); /*pll_rc_byp*/
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x0F, 0x3D, (PllDivComp >> 3) & 0x01); /*pll_pd_vco_opa1*/
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x0F, 0x3E, (PllDivComp >> 7) & 0x01); /*pll_pd_vco_r_byp*/
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x0F, 0x3F, 0x00); /*pll_path_mode*/
    }
    else if (ATBM_LEOB_CHIP_ID == pTuner->ChipID)
    {
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x12, 0x14, PllDivComp); /*rsv0, PLL Divider compensation*/
    }

    ret |= ATBM2040RegRead(u8TunerIndex, I2CAddr, 0x12, 0x15, &reg1215);
    reg1215 &= 0x9F; /*clear <6:5>*/
    reg1215 |= 0x40;
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x12, 0x15, reg1215); /*rsv1<6:5> , ldo voltage for vco*/

    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x0F, 0x2A, 0x00); /*pll_dither_bit_sel_sdm*/
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x0F, 0x15, 0x01); /*pll_pd_ldo_mmd, power down*/

    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x0B, 0x00, 0x01); /* [0]   1   ADC_PD */
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x09, 0x0D, 0x01); /* [0]   1   IF_CORE_PD */
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x12, 0x0B, 0x01); /* [0]   1   MIXR_ANALOG_PD */

    ATBM2040Delay(3000);
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x0F, 0x1D, 0x00); /* [0]   0   pll_rst_sdm */
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x0F, 0x21, 0x00); /* [0]   0   pll_rst_afc_dft */
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x0F, 0x1F, 0x00); /* [0]   0   pll_en_afc_dft */

    /*PLL divider set*/
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x0F, 0x17, PLLDivInt); /* pll_n_int_dft[7:0] */
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x0F, 0x18, (UINT8)(PLLDivFrac & 0xFF)); /* pll_n_frac_dft[7:0] */
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x0F, 0x19, (UINT8)((PLLDivFrac >> 8) & 0xFF)); /* pll_n_frac_dft[15:8] */
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x0F, 0x1A, (UINT8)((PLLDivFrac >> 16) & 0xFF)); /* pll_n_frac_dft[23:16] */

    if (!pTuner->HighMD)
    {
        /*make sure that PLLDivInt and PLLDivFrac are HMD divider, so swap PLLDivInt and PLLDivInt2 in case of LMD*/
        UINT8 tmpPLLDivInt = 0;
        UINT32 tmpPLLDivFrac = 0;
        tmpPLLDivInt = PLLDivInt;
        PLLDivInt = PLLDivInt2;
        PLLDivInt2 = tmpPLLDivInt;

        tmpPLLDivFrac = PLLDivFrac;
        PLLDivFrac = PLLDivFrac2;
        PLLDivFrac2 = tmpPLLDivFrac;

    }
    /*HMD*/
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x14, 0x9E, (UINT8)(PLLDivFrac & 0xFF)); /* ndiv_n[7:0] */
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x14, 0x9F, (UINT8)((PLLDivFrac >> 8) & 0xFF)); /* ndiv_n[15:8] */
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x14, 0xA0, (UINT8)((PLLDivFrac >> 16) & 0xFF)); /* ndiv_n[23:16] */
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x14, 0xA1, PLLDivInt); /* ndiv_n[31:24] */

    /*LMD*/
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x14, 0x9A, (UINT8)(PLLDivFrac2 & 0xFF)); /* ndiv_p[7:0] */
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x14, 0x9B, (UINT8)((PLLDivFrac2 >> 8) & 0xFF)); /* ndiv_p[15:8] */
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x14, 0x9C, (UINT8)((PLLDivFrac2 >> 16) & 0xFF)); /* ndiv_p[23:16] */
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x14, 0x9D, PLLDivInt2); /* ndiv_p[31:24] */


    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x0F, 0x1D, 0x01); /* pll_rst_sdm */
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x0F, 0x21, 0x01); /* pll_rst_afc_dft */
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x0F, 0x1F, 0x01); /* pll_en_afc_dft */
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x0F, 0x15, 0x00); /*pll_pd_ldo_mmd, power on*/

    ret |= ATBM2040RegRead(u8TunerIndex, I2CAddr, 0x0F, 0x26, &val);
    if (0x00 == val)
    {
        ATBM2040Delay(1000);
    }
    ATBM2040Delay(1000);
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x0B, 0x00, 0x00); /* [0]   1   ADC_ON */
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x09, 0x0D, 0x00); /* [0]   1   IF_CORE_ON */
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x12, 0x0B, 0x00); /* [0]   1   MIXR_ANALOG_ON */
    /*Mixer divider */
    if ((ATBM_LEOG_CHIP_ID == pTuner->ChipID)
            || (ATBM_LEOF_CHIP_ID == pTuner->ChipID) || (ATBM_LEOE_CHIP_ID == pTuner->ChipID))
    {
        ATBM2040NLOMixer_t *pNLOMixer = NULL;
        UINT8 MixDivTableLeoF[11][3] =
        {
            /*121CH  1240H  121EH*/
            {0x00,  0x01,  0x01},  /*dummy*/
            {0x00,  0x01,  0x01},  /*NLO: 1*/
            {0x00,  0x01,  0x00},  /*NLO: 2*/
            {0x01,  0x01,  0x00},  /*NLO: 3*/
            {0x06,  0x01,  0x00},  /*NLO: 4*/
            {0x07,  0x01,  0x00},  /*NLO: 5*/
            {0x27,  0x01,  0x00},  /*NLO: 6*/
            {0x27,  0x01,  0x00},  /*NLO: 7*/
            {0x0A,  0x00,  0x00},  /*NLO: 8*/
            {0x0A,  0x00,  0x00},  /*NLO: 9*/
            {0x0F,  0x00,  0x00},  /*NLO: 10*/
        };
        UINT8 MixDivTableLeoE[11][3] =
        {
            /*121CH  1240H  121EH*/
            {0x02, 0x00, 0x00}, /*dummy*/
            {0x02, 0x00, 0x01}, /*NLO: 1*/
            {0x02, 0x00, 0x00}, /*NLO: 2*/
            {0x03, 0x00, 0x00}, /*NLO: 3*/
            {0x04, 0x00, 0x00}, /*NLO: 4*/
            {0x05, 0x00, 0x00}, /*NLO: 5*/
            {0x25, 0x00, 0x00}, /*NLO: 6*/
            {0x08, 0x00, 0x00}, /*NLO: 7*/
            {0x08, 0x00, 0x00}, /*NLO: 8*/
            {0x08, 0x00, 0x00}, /*NLO: 9*/
            {0x0D, 0x00, 0x00}, /*NLO: 10*/
        };

        if (pTuner->HighMD)
        {
            pNLOMixer = pTuner->pNLOMixerHMD;
        }
        else
        {
            pNLOMixer = pTuner->pNLOMixerLMD;
        }
        if (ATBM_LEOE_CHIP_ID == pTuner->ChipID)
        {
            reg121c = MixDivTableLeoE[pNLOMixer->NLO][0];
            reg1240 = MixDivTableLeoE[pNLOMixer->NLO][1];
            reg121e = MixDivTableLeoE[pNLOMixer->NLO][2];

            mixr_pll_mixr_div_mode_l = MixDivTableLeoE[pTuner->pNLOMixerLMD->NLO][0];
            mixr_pll_mixr_div_mode_h = MixDivTableLeoE[pTuner->pNLOMixerHMD->NLO][0];
        }
        else
        {
            /*ATBM_LEOF_CHIP_ID, ATBM_LEOG_CHIP_ID*/
            reg121c = MixDivTableLeoF[pNLOMixer->NLO][0];
            reg1240 = MixDivTableLeoF[pNLOMixer->NLO][1];
            reg121e = MixDivTableLeoF[pNLOMixer->NLO][2];

            mixr_pll_mixr_div_mode_l = MixDivTableLeoF[pTuner->pNLOMixerLMD->NLO][0];
            mixr_pll_mixr_div_mode_h = MixDivTableLeoF[pTuner->pNLOMixerHMD->NLO][0];
        }

        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x12, 0x40, reg1240); /*mixr_div_mode_en*/
        if (ATBM_LEOG_CHIP_ID == pTuner->ChipID)
        {
            ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x16, 0x4A, 0x01); /*params_div_new_en*/
        }
        else
        {
            /*ATBM_LEOE_CHIP_ID  ATBM_LEOF_CHIP_ID*/
            ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x16, 0x4A, (pTuner->pNLOMixerLMD->NLO != pTuner->pNLOMixerHMD->NLO) ? (0x01) : (0x00)); /*params_div_new_en*/
        }

        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x12, 0x1C, reg121c); /*mixr_pll_mixr_div_mode[2:0]*/
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x12, 0x1E, reg121e); /*mixr_pll_mixr_div_mux*/
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x16, 0x44, mixr_pll_mixr_div_mode_l); /*mixr_pll_mixr_div_mode_l*/
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x16, 0x45, mixr_pll_mixr_div_mode_h); /*mixr_pll_mixr_div_mode_h*/
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x16, 0x48, reg121e); /*params_mixr_pll_mixr_div_mux_l*/
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x16, 0x49, reg121e); /*params_mixr_pll_mixr_div_mux_h*/
    }
    else
    {
        switch (pTuner->pNLOMixer->NLO)
        {
            case 7:
                reg121c = 0x03;
                reg121e = 0x00;
                break;
            case 6:
                reg121c = 0x02;
                reg121e = 0x00;
                break;
            case 5:
                reg121c = 0x01;
                reg121e = 0x00;
                break;
            case 4:
                reg121c = 0x00;
                reg121e = 0x00;
                break;
            case 3:
                reg121c = 0x05;
                reg121e = 0x00;
                break;
            case 2:
                reg121c = 0x04;
                reg121e = 0x00;
                break;
            case 1:
                reg121c = 0x00;
                reg121e = 0x01;
                break;
        }

        mixr_pll_mixr_div_mode_l = reg121c;
        mixr_pll_mixr_div_mode_h = reg121c;
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x12, 0x1C, reg121c); /*mixr_pll_mixr_div_mode[2:0]*/
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x12, 0x1E, reg121e); /*mixr_pll_mixr_div_mux*/
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x16, 0x44, mixr_pll_mixr_div_mode_l); /*mixr_pll_mixr_div_mode_l*/
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x16, 0x45, mixr_pll_mixr_div_mode_h); /*mixr_pll_mixr_div_mode_h*/
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x16, 0x48, reg121e); /*params_mixr_pll_mixr_div_mux_l*/
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x16, 0x49, reg121e); /*params_mixr_pll_mixr_div_mux_h*/
    }

    return ret;

}

static UINT8 ATBM2040DrvNAdc2AdcDivPDft(UINT8 u8TunerIndex, UINT32 NAdc)
{
    UINT8 adc_div_p_dft = 0;
    adc_div_p_dft = (UINT8)NAdc;
    return adc_div_p_dft;
}


/************************************************************
*Description: ADC clk module setting
*************************************************************/
static ATBM2040_ERROR_e ATBM2040DrvADCCLKSet(UINT8 u8TunerIndex, ATBM2040_t *pTuner)
{
    ATBM2040_ERROR_e ret = ATBM2040_NO_ERROR;
    ATBM2040ADCDivider_t *pADCDivider = NULL;
    ATBM2040ADCDivider_t *pADCDividerHMD = NULL;
    ATBM2040ADCDivider_t *pADCDividerLMD = NULL;
    UINT8 FsMode = 0;
    UINT32 invFsMul = 0;
    UINT32 FS_H = (UINT32)((pTuner->HMDAdcClkHz / 100000 + 10) / 20), FS_L = (UINT32)((pTuner->LMDAdcClkHz / 100000 + 10) / 20);
    UINT8 I2CAddr = pTuner->Config.I2CAddr;

    ATBM2040Data64_t i64Data, i64Data1;

    /*ADC divider */
    pADCDivider = ATBM2040DrvADCDivider(u8TunerIndex, pTuner->FreqKHz, pTuner->HighMD);
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x12, 0x05, ATBM2040DrvNAdc2AdcDivPDft(u8TunerIndex, pADCDivider->N_ADC)); /*adc_div_p_dft*/

    /*Set params_adc_div_p_dft_l and params_adc_div_p_dft_h for Venus1503*/
    if (pTuner->HighMD)
    {
        pADCDividerHMD = pADCDivider;
        pADCDividerLMD = ATBM2040DrvADCDivider(u8TunerIndex, pTuner->FreqKHz, !pTuner->HighMD);
    }
    else
    {
        pADCDividerLMD = pADCDivider;
        pADCDividerHMD = ATBM2040DrvADCDivider(u8TunerIndex, pTuner->FreqKHz, !pTuner->HighMD);
    }
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x16, 0x42, ATBM2040DrvNAdc2AdcDivPDft(u8TunerIndex, pADCDividerLMD->N_ADC)); /*params_adc_div_p_dft_l[3:0]*/
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x16, 0x43, ATBM2040DrvNAdc2AdcDivPDft(u8TunerIndex, pADCDividerHMD->N_ADC)); /*params_adc_div_p_dft_h[3:0]*/

    /*HMD*/
    if (FS_H <= 190)
    {
        FsMode = 0x00;
    }
    else if ((FS_H > 190) && (FS_H < 210))
    {
        FsMode = 0x01;
    }
    else if (FS_H >= 210)
    {
        FsMode = 0x02;
    }
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x14, 0x3B, FsMode); /*params_fs_mode_1[1:0]*/

    /*params_inv_fs_mul4_1[9:0] = (4/Fs)*2^34 = 2^36/Fs = 2^37/(AdcClk*1000000) = 0x80000000/AdcClk/15625*/
    /*invFsMul = (UINT32)(0x80000000/(pTuner->HMDAdcClkHz/(1000*1000))/15625);*/
    i64Data.High = 0x20;
    i64Data.Low = 0x00;

    i64Data1.High = 0;
    i64Data1.Low = pTuner->HMDAdcClkHz;
    invFsMul = ATBM64Div(u8TunerIndex, i64Data, i64Data1);
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x14, 0x4A, invFsMul & 0xFF); /*params_inv_fs_mul4_1[7:0]*/
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x14, 0x4B, (invFsMul >> 8) & 0x03); /*params_inv_fs_mul4_1[9:8]*/

    /*LMD*/
    if (FS_L <= 190)
    {
        FsMode = 0x00;
    }
    else if ((FS_L > 190) && (FS_L < 210))
    {
        FsMode = 0x01;
    }
    else if (FS_L >= 210)
    {
        FsMode = 0x02;
    }
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x14, 0x3A, FsMode); /*params_fs_mode_0[1:0]*/

    /*params_inv_fs_mul4_0[9:0] = (4/Fs)*2^34 = 2^36/Fs = 2^37/(AdcClk*1000000) = 0x80000000/AdcClk/15625*/
    /* invFsMul = (UINT32)(0x80000000/(pTuner->LMDAdcClkHz/(1000*1000))/15625); */


    i64Data1.High = 0;
    i64Data1.Low = pTuner->LMDAdcClkHz;
    invFsMul = ATBM64Div(u8TunerIndex, i64Data, i64Data1);
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x14, 0x48, invFsMul & 0xFF); /*params_inv_fs_mul4_0[7:0]*/
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x14, 0x49, (invFsMul >> 8) & 0x03); /*params_inv_fs_mul4_0[9:8]*/

    if (((384000000 == pTuner->HMDAdcClkHz) || (384000000 == pTuner->LMDAdcClkHz))
            && (ATBM2040_SIGNAL_MODE_ATV == pTuner->SignalMode))
    {
        FsMode = 0x01;
    }
    else
    {
        FsMode = 0x00;
    }
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x14, 0x6F, FsMode); /*params_t_sample_rssi[7:0]*/
    return ret;
}

/************************************************************
*Description: Calculate ADC clk
*************************************************************/
void ATBM2040DrvADCClkCal(UINT8 u8TunerIndex, ATBM2040_t *pTuner, UINT32 *pHMDAdcClkHz, UINT32  *pLMDAdcClkHz)
{
    ATBM2040NLOMixer_t *pNLOMixer = NULL;
    ATBM2040ADCDivider_t *pADCDivider = NULL;
    UINT32 Multiplier, Multiplier1;
    ATBM2040Data64_t i64Data, i64Data1;

    if(ATBM_LEOG_CHIP_ID == pTuner->ChipID)
    {
        if(ATBM2040_VHF_CHECK(pTuner->FreqKHz))
        {
            if(pTuner->FreqKHz < ATBM2040_VHF2_LOW_KHZ)
            {/*VHF1*/
                pTuner->Band = ATBM2040_RF_BAND_VHF1;
            }
            else
            {/*VHF2*/
                pTuner->Band = ATBM2040_RF_BAND_VHF2;
            }
        }
        else
        {
            pTuner->Band = ATBM2040_RF_BAND_UHF;
        }
    }
#ifndef ATBM2040_CHIP_DEBUG_OPEN
    if(ATBM_LEOG_CHIP_ID == pTuner->ChipID)
    {    
        /*Setting IF frequency*/
        if((ATBM2040_SIGNAL_MODE_DVBT == pTuner->SignalMode)&&(!ATBM2040_VHF_CHECK(pTuner->FreqKHz)))
        {
            pTuner->MixerIF = 7000000;
        }
        else
        {
            pTuner->MixerIF = 5000000;
        }
    }
#endif

    pNLOMixer = pTuner->pNLOMixerHMD;
    pADCDivider = ATBM2040DrvADCDivider(u8TunerIndex, pTuner->FreqKHz, TRUE);


    Multiplier =  pTuner->FreqKHz * 1000 + pTuner->MixerIF;
    Multiplier1 = pNLOMixer->NMixer * pNLOMixer->NLO;
    ATBM64Mult(u8TunerIndex, &i64Data, Multiplier, Multiplier1); /* KHz */
    i64Data1.High = 0;
    i64Data1.Low = pADCDivider->N_ADC;
    pTuner->HMDAdcClkHz = *pHMDAdcClkHz = ATBM64Div(u8TunerIndex, i64Data, i64Data1);


    /* pTuner->LMDAdcClk = *pLMDAdcClk = ((pTuner->FreqKHz/1000.0 - pTuner->MixerIF/1000000.0) * pNLOMixer->NMixer  * pNLOMixer->NLO)/pADCDivider->N_ADC; */
    pNLOMixer = pTuner->pNLOMixerLMD;
    pADCDivider = ATBM2040DrvADCDivider(u8TunerIndex, pTuner->FreqKHz, FALSE);
    Multiplier =  pTuner->FreqKHz * 1000 - pTuner->MixerIF;
    Multiplier1 = pNLOMixer->NMixer * pNLOMixer->NLO;
    ATBM64Mult(u8TunerIndex, &i64Data, Multiplier, Multiplier1); /* KHz */
    i64Data1.High = 0;
    i64Data1.Low = pADCDivider->N_ADC;
    pTuner->LMDAdcClkHz = *pLMDAdcClkHz = ATBM64Div(u8TunerIndex, i64Data, i64Data1);
}

/************************************************************
*Description: Phase and gain compensation setting
*************************************************************/
#if ATBM2040_GAIN_PHASE_COMPENSATION
static ATBM2040_ERROR_e ATBM2040DrvPhaseAndGainConfig(UINT8 u8TunerIndex, ATBM2040_t *pTuner)
{
    ATBM2040_ERROR_e ret = ATBM2040_NO_ERROR;
    UINT32 n = 0, i = 0;
    ATBM2040Reg_t *pReg = NULL;
    SINT32 *pData = NULL;
    UINT8 I2CAddr = pTuner->Config.I2CAddr;

    n = (pTuner->FreqKHz + 20000) / 40000;
    if (n > 0)
    {
        n = n - 1;
    }
    if (n > 24)
    {
        n = 24;
    }
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x15, 0x76, 0x01); /*params_phase_compensation*/
    /*LNA phase compensation*/
    pData = &ATBM2040LNAPhase[n][0];
    for (i = 0; i < ATBM2040_LNA_PHASE_REG_CNT / 2; i++)
    {
        pReg = &LNAPhaseReg[2 * i];
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, pReg->BaseReg, pReg->OffReg, pData[i] & 0xFF);
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, (pReg + 1)->BaseReg, (pReg + 1)->OffReg, (pData[i] & 0xFF00) >> 8);
    }
    /*RTF/RF_VGA1/2  phase compensation*/
    if (ATBM2040_VHF_CHECK(pTuner->FreqKHz))
    {
        pData = &ATBM2040RTFPhase[n][0];
    }
    else
    {
        pData = &ATBM2040RFVGA12Phase[n][0];
    }
    pReg = RFVGA1PhaseReg;
    for (i = 0; i < ATBM2040_RFVGA1_PHASE_REG_CNT; i++)
    {
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, pReg[i].BaseReg, pReg[i].OffReg, (UINT8)pData[i]);
    }

    pReg = RFVGA2PhaseReg;
    pData = &ATBM2040RFVGA12Phase[n][0];
    for (i = 0; i < ATBM2040_RFVGA2_PHASE_REG_CNT; i++)
    {
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, pReg[i].BaseReg, pReg[i].OffReg, (UINT8)pData[i]);
    }

    /*IF_VGA  phase compensation */
    pReg = IFVGA2PhaseReg;
    pData = ATBM2040IFVGAPhase;
    for (i = 0; i < ATBM2040_IFVGA2_PHASE_REG_CNT; i++)
    {
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, pReg[i].BaseReg, pReg[i].OffReg, (UINT8)pData[i]);
    }

    /*LNA_gain compensation*/
    pReg = LNAGainReg;
    pData = &ATBM2040LNAGain[n][0];
    for (i = 0; i < ATBM2040_LNA_GAIN_REG_CNT; i++)
    {
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, pReg[i].BaseReg, pReg[i].OffReg, (UINT8)pData[i]);
    }

    /*RTF/RF_VGA1/2  delta gain compensation*/
    if (ATBM2040_VHF_CHECK(pTuner->FreqKHz))
    {
        pData = &ATBM2040RTFGain[n][0];
    }
    else
    {
        pData = &ATBM2040RFVGA12Gain[n][0];
    }
    pReg = RFVGA1GainReg;
    for (i = 0; i < ATBM2040_RFVGA1_GAIN_REG_CNT; i++)
    {
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, pReg[i].BaseReg, pReg[i].OffReg, (UINT8)pData[i]);
    }

    pReg = RFVGA2GainReg;
    pData = &ATBM2040RFVGA12Gain[n][0];
    for (i = 0; i < ATBM2040_RFVGA2_GAIN_REG_CNT; i++)
    {
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, pReg[i].BaseReg, pReg[i].OffReg, (UINT8)pData[i]);
    }

    /*IF_VGA  delta gain compensation */
    pReg = IFVGA2GainReg;
    pData = ATBM2040IFVGAGain;
    for (i = 0; i < ATBM2040_IFVGA2_GAIN_REG_CNT; i++)
    {
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, pReg[i].BaseReg, pReg[i].OffReg, (UINT8)pData[i]);
    }

    return ret;
}
#endif

static ATBM2040_ERROR_e ATBM2040DrvFreqShiftFromBBtoIF(UINT8 u8TunerIndex, ATBM2040_t *pTuner)
{
    /* hilbert_f1_L = (SINT32)(IF_DSP*(1/(FS_L))*2*(0x01<<17) + 0.5); */
    /* hilbert_f1_H = (SINT32)(IF_DSP*(1/(FS_H))*2*(0x01<<17) + 0.5); */
    ATBM2040_ERROR_e ret = ATBM2040_NO_ERROR;
    UINT32 FS_L = 0, FS_H = 0;
    SINT32 hilbert_f1_L = 0, hilbert_f1_H = 0;
    UINT32 IF_DSP = 0;
    UINT8 I2CAddr = pTuner->Config.I2CAddr;
    UINT32 Multiplier, Multiplier1;
    ATBM2040Data64_t i64Data, i64Data1;

    if (ATBM2040_SIGNAL_MODE_ATV != pTuner->Config.Mode)
    {
        IF_DSP = pTuner->Config.AtvIFOut.IFOutFreqHz;
    }
    else
    {
        IF_DSP = pTuner->Config.DtvIFOut.IFOutFreqHz;
    }

    if ((ATBM_LEOG_CHIP_ID == pTuner->ChipID)
            || (ATBM_LEOF_CHIP_ID == pTuner->ChipID) || (ATBM_LEOE_CHIP_ID == pTuner->ChipID)
            || (ATBM_LEOD_CHIP_ID == pTuner->ChipID) || (ATBM_LEOC_CHIP_ID == pTuner->ChipID))
    {
        IF_DSP *= 4;
    }

    FS_L = pTuner->LMDAdcClkHz / 2;
    FS_H = pTuner->HMDAdcClkHz / 2;


    Multiplier =  IF_DSP;
    Multiplier1 = 4 * (0x01 << 17);
    ATBM64Mult(u8TunerIndex, &i64Data, Multiplier, Multiplier1); /* KHz */
    i64Data1.High = 0;
    i64Data1.Low = FS_L;
    hilbert_f1_L = ATBM64Div(u8TunerIndex, i64Data, i64Data1);
    hilbert_f1_L = ((hilbert_f1_L + 1) / 2);


    Multiplier =  IF_DSP;
    Multiplier1 = 4 * (0x01 << 17);
    ATBM64Mult(u8TunerIndex, &i64Data, Multiplier, Multiplier1); /* KHz */
    i64Data1.High = 0;
    i64Data1.Low = FS_H;
    hilbert_f1_H = ATBM64Div(u8TunerIndex, i64Data, i64Data1);
    hilbert_f1_H = ((hilbert_f1_H + 1) / 2);


    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x13, 0xAD, hilbert_f1_H & 0xFF); /*params_hilbert_f1_1[7:0]*/
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x13, 0xAE, (hilbert_f1_H >> 8) & 0xFF); /*params_hilbert_f1_1[15:8]*/
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x13, 0xAF, (hilbert_f1_H >> 16) & 0x07); /*params_hilbert_f1_1[18:16]*/
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x13, 0xA3, hilbert_f1_L & 0xFF); /*params_hilbert_f1_0[7:0]*/
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x13, 0xA4, (hilbert_f1_L >> 8) & 0xFF); /*params_hilbert_f1_0[15:8]*/
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x13, 0xA5, (hilbert_f1_L >> 16) & 0x07); /*params_hilbert_f1_0[18:16]*/
    return ret;
}

/************************************************************
*Description: Signal frequency parameters setting in DSP module
*************************************************************/
static ATBM2040_ERROR_e ATBM2040DrvDSPSignalFreqParamSetting(UINT8 u8TunerIndex, ATBM2040_t *pTuner)
{
    ATBM2040_ERROR_e ret = ATBM2040_NO_ERROR;
    SINT32 TmpParam = 0;
    SINT32 path1_hlo = 0, path1_llo = 0;
    UINT32 LMDAdcClk = 0, HMDAdcClk = 0;
    SINT32 MixerIF = 0, BandWidth = 0;
    ATBM2040Data64_t i64Data, i64Data1;
    UINT32 i = 0, DataLen = 0;
    UINT32 SigDown21Freq = 21000000,SignalN1Out = 2000000;
    UINT32 SigDown21FreqH = 0x280,SigDown21FreqL = 0xDE800000;
    UINT8 Data[64] = {0};
    UINT8 I2CAddr = pTuner->Config.I2CAddr;

    if(ATBM_LEOG_CHIP_ID == pTuner->ChipID)
    {
        if((ATBM2040_SIGNAL_MODE_DVBT == pTuner->SignalMode)&&(pTuner->BandBak != pTuner->Band))
        {
            switch(pTuner->Band)
            {
                case ATBM2040_RF_BAND_UHF:
                    ret |= ATBM2040DrvRegDataBurstWrite(u8TunerIndex,I2CAddr,AciN234CalResultUHF,ATBM2040_ACI234_CAL_RESULT_UHF_SIZE);
                    if(pTuner->FreqKHz >= 500000)
                    {
                        ret |= ATBM2040RegWrite(u8TunerIndex,I2CAddr,0x14,0x97, 0x14);
                    }
                    break;
                case ATBM2040_RF_BAND_VHF2:
                    ret |= ATBM2040DrvRegDataBurstWrite(u8TunerIndex,I2CAddr,AciN234CalResultVHF2,ATBM2040_ACI234_CAL_RESULT_VHF2_SIZE);
                    break;                
                case ATBM2040_RF_BAND_VHF1:
                    ret |= ATBM2040DrvRegDataBurstWrite(u8TunerIndex,I2CAddr,AciN234CalResultVHF1,ATBM2040_ACI234_CAL_RESULT_VHF1_SIZE);
                    break;
                default:
                    break;
            }
        }
    }

    LMDAdcClk = pTuner->LMDAdcClkHz;
    HMDAdcClk = pTuner->HMDAdcClkHz;
    if(7000000 == pTuner->MixerIF)
    {
        SigDown21Freq = 24000000;
        SignalN1Out = 1000000;
        SigDown21FreqH = 0x2DC; /*2^17 * SigDown21Freq*/
        SigDown21FreqL = 0x6C000000;
    }

    i = 0;
    /*sig_down_2_1_fo_sa_path1_hlo*/
    i64Data.High = SigDown21FreqH;
    i64Data.Low = SigDown21FreqL; 
    i64Data1.High = 0;
    i64Data1.Low = HMDAdcClk;
    path1_hlo = ATBM64Div(u8TunerIndex, i64Data, i64Data1);
    TmpParam = -path1_hlo;
    Data[i++] = (UINT8)(TmpParam & 0xFF);
    Data[i++] = (UINT8)((TmpParam >> 8) & 0xFF);
    /*sig_down_2_1_fo_sa_path1_llo*/
    i64Data.High = SigDown21FreqH;
    i64Data.Low = SigDown21FreqL;
    i64Data1.High = 0;
    i64Data1.Low = LMDAdcClk;
    path1_llo = ATBM64Div(u8TunerIndex, i64Data, i64Data1);
    TmpParam = -path1_llo;
    Data[i++] = (UINT8)(TmpParam & 0xFF);
    Data[i++] = (UINT8)((TmpParam >> 8) & 0xFF);
    /*sig_down_2_1_fo_sa_path2_hlo*/
    TmpParam = path1_hlo;
    Data[i++] = (UINT8)(TmpParam & 0xFF);
    Data[i++] = (UINT8)((TmpParam >> 8) & 0xFF);
    /*sig_down_2_1_fo_sa_path2_llo*/
    TmpParam = path1_llo;
    Data[i++] = (UINT8)(TmpParam & 0xFF);
    Data[i++] = (UINT8)((TmpParam >> 8) & 0xFF);
    DataLen = i;
    ret |= ATBM2040DrvRegDataFlushWrite(u8TunerIndex, I2CAddr, 0x23, 0x50, Data, DataLen);

    if((pTuner->BWKHzBak == pTuner->BWKHz)&&(pTuner->MixerIFBak == pTuner->MixerIF))
    {
        return ret;
    }
    
    MixerIF = pTuner->MixerIF;
    BandWidth = pTuner->BWKHz*1000;

    i = 0;
    if (ATBM2040_SIGNAL_MODE_ATV == pTuner->SignalMode)
    {
        /*signal_n_1_llo*/
        TmpParam = ((MixerIF - BandWidth) + 128) >> 8;
        Data[i++] = (UINT8)(TmpParam & 0xFF);
        Data[i++] = (UINT8)((TmpParam >> 8) & 0xFF);
        Data[i++] = (UINT8)((TmpParam >> 16) & 0xFF);
        /*signal_n_1_hlo*/
        TmpParam = -TmpParam;
        Data[i++] = (UINT8)(TmpParam & 0xFF);
        Data[i++] = (UINT8)((TmpParam >> 8) & 0xFF);
        Data[i++] = (UINT8)((TmpParam >> 16) & 0xFF);
        /*signal_n_1_out_llo*/
        TmpParam = ((MixerIF + BandWidth) + 128) >> 8;
        Data[i++] = (UINT8)(TmpParam & 0xFF);
        Data[i++] = (UINT8)((TmpParam >> 8) & 0xFF);
        Data[i++] = (UINT8)((TmpParam >> 16) & 0xFF);
        /*signal_n_1_out_hlo*/
        TmpParam = -TmpParam;
        Data[i++] = (UINT8)(TmpParam & 0xFF);
        Data[i++] = (UINT8)((TmpParam >> 8) & 0xFF);
        Data[i++] = (UINT8)((TmpParam >> 16) & 0xFF);
    }
    else
    {
        /*signal_n_1_llo*/
        TmpParam = ((MixerIF - BandWidth - 2000000) + 128) >> 8;
        Data[i++] = (UINT8)(TmpParam & 0xFF);
        Data[i++] = (UINT8)((TmpParam >> 8) & 0xFF);
        Data[i++] = (UINT8)((TmpParam >> 16) & 0xFF);
        /*signal_n_1_hlo*/
        TmpParam = -TmpParam;
        Data[i++] = (UINT8)(TmpParam & 0xFF);
        Data[i++] = (UINT8)((TmpParam >> 8) & 0xFF);
        Data[i++] = (UINT8)((TmpParam >> 16) & 0xFF);
        /*signal_n_1_out_llo*/
        TmpParam = ((MixerIF + BandWidth + SignalN1Out) + 128)>>8;
        Data[i++] = (UINT8)(TmpParam & 0xFF);
        Data[i++] = (UINT8)((TmpParam >> 8) & 0xFF);
        Data[i++] = (UINT8)((TmpParam >> 16) & 0xFF);
        /*signal_n_1_out_hlo*/
        TmpParam = -TmpParam;
        Data[i++] = (UINT8)(TmpParam & 0xFF);
        Data[i++] = (UINT8)((TmpParam >> 8) & 0xFF);
        Data[i++] = (UINT8)((TmpParam >> 16) & 0xFF);
    }
    DataLen = i;
    ret |= ATBM2040DrvRegDataFlushWrite(u8TunerIndex, I2CAddr, 0x17, 0x84, Data, DataLen);

    i = 0;
    /*signal_nplus1_llo*/
    TmpParam = ((MixerIF + BandWidth) + 128) >> 8;
    Data[i++] = (UINT8)(TmpParam & 0xFF);
    Data[i++] = (UINT8)((TmpParam >> 8) & 0xFF);
    Data[i++] = (UINT8)((TmpParam >> 16) & 0xFF);
    /*signal_nminus1_llo*/
    TmpParam = ((MixerIF - BandWidth) + 128) >> 8;
    Data[i++] = (UINT8)(TmpParam & 0xFF);
    Data[i++] = (UINT8)((TmpParam >> 8) & 0xFF);
    Data[i++] = (UINT8)((TmpParam >> 16) & 0xFF);
    /*signal_nplus1_hlo*/
    TmpParam = -TmpParam;
    Data[i++] = (UINT8)(TmpParam & 0xFF);
    Data[i++] = (UINT8)((TmpParam >> 8) & 0xFF);
    Data[i++] = (UINT8)((TmpParam >> 16) & 0xFF);
    /*signal_nminus1_hlo*/
    TmpParam = ((-MixerIF - BandWidth) + 128) >> 8;
    Data[i++] = (UINT8)(TmpParam & 0xFF);
    Data[i++] = (UINT8)((TmpParam >> 8) & 0xFF);
    Data[i++] = (UINT8)((TmpParam >> 16) & 0xFF);
    DataLen = i;
    ret |= ATBM2040DrvRegDataFlushWrite(u8TunerIndex, I2CAddr, 0x19, 0x98, Data, DataLen);

    i = 0;
    /*signal_nminus1_llo_sa*/
    TmpParam = ((MixerIF - BandWidth) + 128) >> 8;
    Data[i++] = (UINT8)(TmpParam & 0xFF);
    Data[i++] = (UINT8)((TmpParam >> 8) & 0xFF);
    Data[i++] = (UINT8)((TmpParam >> 16) & 0xFF);
    /*signal_nminus1_hlo_sa*/
    TmpParam = ((-MixerIF - BandWidth) + 128) >> 8;
    Data[i++] = (UINT8)(TmpParam & 0xFF);
    Data[i++] = (UINT8)((TmpParam >> 8) & 0xFF);
    Data[i++] = (UINT8)((TmpParam >> 16) & 0xFF);
    /*signal_nminus2_llo_sa*/
    TmpParam = ((MixerIF - BandWidth*2 + SigDown21Freq) + 128)>>8;
    Data[i++] = (UINT8)(TmpParam & 0xFF);
    Data[i++] = (UINT8)((TmpParam >> 8) & 0xFF);
    Data[i++] = (UINT8)((TmpParam >> 16) & 0xFF);
    /*signal_nminus2_hlo_sa*/
    TmpParam = ((-MixerIF - BandWidth*2 + SigDown21Freq) + 128)>>8;
    Data[i++] = (UINT8)(TmpParam & 0xFF);
    Data[i++] = (UINT8)((TmpParam >> 8) & 0xFF);
    Data[i++] = (UINT8)((TmpParam >> 16) & 0xFF);
    /*signal_nminus3_llo_sa*/
    TmpParam = ((MixerIF - BandWidth*3 + SigDown21Freq) + 128)>>8;
    Data[i++] = (UINT8)(TmpParam & 0xFF);
    Data[i++] = (UINT8)((TmpParam >> 8) & 0xFF);
    Data[i++] = (UINT8)((TmpParam >> 16) & 0xFF);
    /*signal_nminus3_hlo_sa*/
    TmpParam = ((-MixerIF - BandWidth*3 + SigDown21Freq) + 128)>>8;
    Data[i++] = (UINT8)(TmpParam & 0xFF);
    Data[i++] = (UINT8)((TmpParam >> 8) & 0xFF);
    Data[i++] = (UINT8)((TmpParam >> 16) & 0xFF);
    /*signal_nminus4_llo_sa*/
    TmpParam = ((MixerIF - BandWidth*4 + SigDown21Freq) + 128)>>8;
    Data[i++] = (UINT8)(TmpParam & 0xFF);
    Data[i++] = (UINT8)((TmpParam >> 8) & 0xFF);
    Data[i++] = (UINT8)((TmpParam >> 16) & 0xFF);
    /*signal_nminus4_hlo_sa*/
    TmpParam = ((-MixerIF - BandWidth*4 + SigDown21Freq) + 128)>>8;
    Data[i++] = (UINT8)(TmpParam & 0xFF);
    Data[i++] = (UINT8)((TmpParam >> 8) & 0xFF);
    Data[i++] = (UINT8)((TmpParam >> 16) & 0xFF);
    /*signal_nplus1_llo_sa*/
    TmpParam = ((MixerIF + BandWidth) + 128) >> 8;
    Data[i++] = (UINT8)(TmpParam & 0xFF);
    Data[i++] = (UINT8)((TmpParam >> 8) & 0xFF);
    Data[i++] = (UINT8)((TmpParam >> 16) & 0xFF);
    /*signal_nplus1_hlo_sa*/
    TmpParam = ((-MixerIF + BandWidth) + 128) >> 8;
    Data[i++] = (UINT8)(TmpParam & 0xFF);
    Data[i++] = (UINT8)((TmpParam >> 8) & 0xFF);
    Data[i++] = (UINT8)((TmpParam >> 16) & 0xFF);
    /*signal_nplus2_llo_sa*/
    TmpParam = ((MixerIF + BandWidth*2 - SigDown21Freq) + 128)>>8;
    Data[i++] = (UINT8)(TmpParam & 0xFF);
    Data[i++] = (UINT8)((TmpParam >> 8) & 0xFF);
    Data[i++] = (UINT8)((TmpParam >> 16) & 0xFF);
    /*signal_nplus2_hlo_sa*/
    TmpParam = ((-MixerIF + BandWidth*2 - SigDown21Freq) + 128)>>8;
    Data[i++] = (UINT8)(TmpParam & 0xFF);
    Data[i++] = (UINT8)((TmpParam >> 8) & 0xFF);
    Data[i++] = (UINT8)((TmpParam >> 16) & 0xFF);
    /*signal_nplus3_llo_sa*/
    TmpParam = ((MixerIF + BandWidth*3 - SigDown21Freq) + 128)>>8;
    Data[i++] = (UINT8)(TmpParam & 0xFF);
    Data[i++] = (UINT8)((TmpParam >> 8) & 0xFF);
    Data[i++] = (UINT8)((TmpParam >> 16) & 0xFF);
    /*signal_nplus3_hlo_sa*/
    TmpParam = ((-MixerIF + BandWidth*3 - SigDown21Freq) + 128)>>8;
    Data[i++] = (UINT8)(TmpParam & 0xFF);
    Data[i++] = (UINT8)((TmpParam >> 8) & 0xFF);
    Data[i++] = (UINT8)((TmpParam >> 16) & 0xFF);
    /*signal_nplus4_llo_sa*/
    TmpParam = ((MixerIF + BandWidth*4 - SigDown21Freq) + 128)>>8;
    Data[i++] = (UINT8)(TmpParam & 0xFF);
    Data[i++] = (UINT8)((TmpParam >> 8) & 0xFF);
    Data[i++] = (UINT8)((TmpParam >> 16) & 0xFF);
    /*signal_nplus4_hlo_sa*/
    TmpParam = ((-MixerIF + BandWidth*4 - SigDown21Freq) + 128)>>8;
    Data[i++] = (UINT8)(TmpParam & 0xFF);
    Data[i++] = (UINT8)((TmpParam >> 8) & 0xFF);
    Data[i++] = (UINT8)((TmpParam >> 16) & 0xFF);
    DataLen = i;
    ret |= ATBM2040DrvRegDataFlushWrite(u8TunerIndex, I2CAddr, 0x22, 0xC1, Data, DataLen);


    return ret;
}


/************************************************************
*Description: DSP module setting
*************************************************************/
static ATBM2040_ERROR_e ATBM2040DrvDSPRegConfig(UINT8 u8TunerIndex, ATBM2040_t *pTuner, UINT32 HMDAdcClk, UINT32 LMDAdcClk)
{
    ATBM2040_ERROR_e ret = ATBM2040_NO_ERROR;
    SINT32 hilbert_f0_L = 0;
    SINT32 hilbert_f0_H = 0;
    SINT32 Fs_ADC_H = 0, Fs_ADC_L = 0;
    UINT32 FS_L = 0, FS_H = 0;
    SINT32 MixerIF = 0;


    UINT32 Multiplier, Multiplier1;
    ATBM2040Data64_t i64Data, i64Data1;

    /* SINT32 hilbert_f1_L = 0,hilbert_f1_H = 0; */
    UINT8 I2CAddr = pTuner->Config.I2CAddr;

    FS_L = LMDAdcClk / 2;
    FS_H = HMDAdcClk / 2;
    MixerIF = pTuner->MixerIF;


    /* hilbert_f0_H = (SINT32)(-MixerIF*(1/(FS_H/4))*2*(0x01<<17) - offset); */
    /* hilbert_f0_L = (SINT32)(MixerIF*(1/(FS_L/4))*2*(0x01<<17) + offset); */
    Multiplier =  MixerIF;
    Multiplier1 = 16 * (0x01 << 17);
    ATBM64Mult(u8TunerIndex, &i64Data, Multiplier, Multiplier1); /* KHz */
    i64Data1.High = 0;
    i64Data1.Low = FS_H;
    hilbert_f0_H = ATBM64Div(u8TunerIndex, i64Data, i64Data1);
    hilbert_f0_H = ((hilbert_f0_H + 1) / 2) * (-1);


    Multiplier =  MixerIF;
    Multiplier1 = 16 * (0x01 << 17);
    ATBM64Mult(u8TunerIndex, &i64Data, Multiplier, Multiplier1); /* KHz */
    i64Data1.High = 0;
    i64Data1.Low = FS_L;
    hilbert_f0_L = ATBM64Div(u8TunerIndex, i64Data, i64Data1);
    hilbert_f0_L = ((hilbert_f0_L + 1) / 2);
    if ((ATBM_LEOG_CHIP_ID == pTuner->ChipID)
            || (ATBM_LEOF_CHIP_ID == pTuner->ChipID) || (ATBM_LEOE_CHIP_ID == pTuner->ChipID)
            || (ATBM_LEOD_CHIP_ID == pTuner->ChipID) || (ATBM_LEOC_CHIP_ID == pTuner->ChipID))
    {
        hilbert_f0_H = 0 - hilbert_f0_H;
        hilbert_f0_L = 0 - hilbert_f0_L;
    }
    else
    {
        if (pTuner->ADCIQSwap)
        {
            hilbert_f0_H = 0 - hilbert_f0_H;
            hilbert_f0_L = 0 - hilbert_f0_L;
        }
    }

    /*frequency shift from IF to BB in DSP*/
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x13, 0xA0, hilbert_f0_L & 0xFF); /*params_hilbert_fo_0[7:0]*/
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x13, 0xA1, (hilbert_f0_L >> 8) & 0xFF); /*params_hilbert_fo_0[15:8]*/
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x13, 0xA2, (hilbert_f0_L >> 16) & 0x07); /*params_hilbert_fo_0[18:16]*/
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x13, 0xAA, hilbert_f0_H & 0xFF); /*params_hilbert_fo_1[7:0]*/
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x13, 0xAB, (hilbert_f0_H >> 8) & 0xFF); /*params_hilbert_fo_1[15:8]*/
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x13, 0xAC, (hilbert_f0_H >> 16) & 0x07); /*params_hilbert_fo_1[18:16]*/


    /*frequency shift from BB to IF in DSP*/
    ret |= ATBM2040DrvFreqShiftFromBBtoIF(u8TunerIndex, pTuner);

    /*RC filte sampling rate setting*/
    Fs_ADC_H = (SINT32)(FS_H / 8 / (0x01 << 9) + 1) / 2;
    Fs_ADC_L = (SINT32)(FS_L / 8 / (0x01 << 9) + 1) / 2;
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x13, 0x18, Fs_ADC_L & 0xFF); /*params_rc_fs_adc_0[7:0]*/
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x13, 0x19, (Fs_ADC_L >> 8) & 0xFF); /*params_rc_fs_adc_0[15:8]*/
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x13, 0xB8, Fs_ADC_H & 0xFF); /*params_rc_fs_adc_1[7:0]*/
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x13, 0xB9, (Fs_ADC_H >> 8) & 0xFF); /*params_rc_fs_adc_1[15:8]*/

    /*Signal frequency parameters setting*/
    if ((ATBM_LEOG_CHIP_ID == pTuner->ChipID)
            || (ATBM_LEOF_CHIP_ID == pTuner->ChipID) || (ATBM_LEOE_CHIP_ID == pTuner->ChipID))
    {
        ret |= ATBM2040DrvDSPSignalFreqParamSetting(u8TunerIndex, pTuner);
    }

#if ATBM2040_FIRMWARE_LOAD
    if((ATBM_LEOG_CHIP_ID == pTuner->ChipID)&&(ATBM2040_SIGNAL_MODE_DVBT == pTuner->SignalMode))
    {
        if(pTuner->BandBak != pTuner->Band)
        {
            UINT8 *pData = NULL;
            switch(pTuner->Band)
            {
                case ATBM2040_RF_BAND_UHF:
                    pData = FuZuDataUHF;
                    ret |= ATBM2040DrvRegDataFlushWrite(u8TunerIndex,I2CAddr,0x21,0x50,pData,ATBM2040_FUZU_DATA_UHF_SIZE);
                    break;
                case ATBM2040_RF_BAND_VHF2:
                    pData = FuZuDataVHF2;
                    ret |= ATBM2040DrvRegDataFlushWrite(u8TunerIndex,I2CAddr,0x21,0x50,pData,ATBM2040_FUZU_DATA_VHF2_SIZE);
                    break;
                default:
                    break;
            }            
        }
        if(ATBM2040_RF_BAND_UHF == pTuner->Band)
        {
            if(pTuner->FreqKHz < 500000)
            {
                ret |= ATBM2040RegWrite(u8TunerIndex,I2CAddr,0x21,0x6A, 0x0D);
            }
            else if(pTuner->FreqKHz < 600000)
            {
                ret |= ATBM2040RegWrite(u8TunerIndex,I2CAddr,0x21,0x6A, 0x0F);
            }
        }
    }
#endif
    return ret;
}

/************************************************************
*Description: VHF module setting
*************************************************************/
static ATBM2040_ERROR_e ATBM2040DrvVHFSetting(UINT8 u8TunerIndex, ATBM2040_t *pTuner)
{
    ATBM2040_ERROR_e ret = ATBM2040_NO_ERROR;
    ATBM2040VHFChIndex_t *pIndex = NULL;
    UINT8 tmpReg = 0;
    UINT8 I2CAddr = pTuner->Config.I2CAddr;

    if ((ATBM_LEOB_CHIP_ID == pTuner->ChipID) || (ATBM_LEOC_CHIP_ID == pTuner->ChipID)
            || (ATBM_LEOD_CHIP_ID == pTuner->ChipID))
    {
        ret |= ATBM2040RegRead(u8TunerIndex, I2CAddr, 0x12, 0x16, &tmpReg); /*Rsv2*/
        tmpReg = ((tmpReg & 0xF0) | 0x05);
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x12, 0x16, tmpReg); /*Rsv2<3:0>=4'b0101*/
    }
    /*VHF filter registers setting*/
    ATBM2040RegListWrite(u8TunerIndex, I2CAddr, VHFFilterReg, ATBM2040_VHF_FILTER_REG_CNT);
    pIndex = ATBM2040DrvVHFFreqIndex(u8TunerIndex, pTuner->FreqKHz);
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x06, 0x00, pIndex->vhf_fltr_data); /*vhf_fltr_data[7:0]*/
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x06, 0x01, pIndex->vhf_fltr_ref_code); /*vhf_fltr_ref_code[7:0]*/
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x16, 0xF9, pIndex->vhf_fltr_data); /*params_vhf_fltr_data[7:0] */

    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x06, 0x21, pIndex->vhf_fltr_res1_sel); /*vhf_fltr_res1_sel[2:0]*/

    /*GmC calibration */
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x06, 0x1D, 0x01); /*vhf_fltr_cal_rst*/
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x06, 0x1D, 0x00); /*vhf_fltr_cal_rst*/

    ATBM2040Delay(1000);
    ret |= ATBM2040RegRead(u8TunerIndex, I2CAddr, 0x06, 0x22, &tmpReg); /*vhf_fltr_ready_reg*/

    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x12, 0x2E, 0x00); /*vhf_filt_cal_clk_en*/
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x06, 0x1C, 0x00); /*vhf_fltr_bias_pd*/
    if (0x01 != tmpReg)
    {
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x06, 0x1E, 0x01); /*vhf_fltr_cap_code_sel*/
    }
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x14, 0x39, 0x00); /*params_freq_path*/

    return ret;
}

/************************************************************
*Description: UHF module setting
*************************************************************/
static ATBM2040_ERROR_e ATBM2040DrvUHFSetting(UINT8 u8TunerIndex, ATBM2040_t *pTuner)
{
    ATBM2040_ERROR_e ret = ATBM2040_NO_ERROR;
    UINT8 filt_fcal_busy = 0, filt_fcal_fail = 0;
    UINT8 uhf_filt_cal_ctrl_fail = 0, uhf_filt_cal_ctrl_suc = 0;
    UINT32 CenterFreqMHz = 0;
    UINT32 FltrGmSwitch = 0, FltrManual = 0;
    ATBM2040UHFChIndex_t *pIndex = NULL;
    BOOL DebugOpen = FALSE;
    UINT8 I2CAddr = pTuner->Config.I2CAddr;
#ifdef ATBM2040_CHIP_DEBUG_OPEN
    double GmRatio = 1, ManaulRatio = 1;
    double RFRatio = 1;
    SINT32 RFOffset = 0;
#endif
    CenterFreqMHz = (pTuner->FreqKHz + 500) / 1000;
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x04, 0x01, 0x01); /*lna_ldo_pd*/

    /*Setting the UHF filter and  Calibration*/
    if((0 != pTuner->LowCousumMd)&&(ATBM_LEOG_CHIP_ID == pTuner->ChipID))
    {
        ATBM2040Reg_t TmpReg = {0x05,0x09, 0x04};
        ATBM2040RegListWriteExt(u8TunerIndex,I2CAddr,UHFFltCalibReg,ATBM2040_UHF_FILTER_REG_CNT,TmpReg);
    }
    else
    {
        ATBM2040Reg_t TmpReg = {0x05,0x09, 0x0C};
        ATBM2040RegListWriteExt(u8TunerIndex,I2CAddr,UHFFltCalibReg,ATBM2040_UHF_FILTER_REG_CNT,TmpReg);
    }
    if ((ATBM_LEOG_CHIP_ID == pTuner->ChipID)
            || (ATBM_LEOF_CHIP_ID == pTuner->ChipID) || (ATBM_LEOE_CHIP_ID == pTuner->ChipID))
    {
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x05, 0x21, 0x01); /*vhf2_fltr_pwdn_ldo*/
    }

    /* UHF filter frequency index setting*/
    pIndex = ATBM2040DrvUHFFreqIndex(u8TunerIndex, pTuner->FreqKHz);
    FltrGmSwitch = pIndex->uhf_fltr_gm_switch;
    FltrManual = pIndex->uhf_fltr_manual;
    uhf_filt_cal_ctrl_fail = 0x1E;
    uhf_filt_cal_ctrl_suc = 0x1F;

#ifdef ATBM2040_CHIP_DEBUG_OPEN
    ATBM2040DrvChipDebugUHFFilterGMManaulRatioGet(u8TunerIndex, pTuner, &GmRatio, &ManaulRatio, &RFRatio, &RFOffset, &DebugOpen);
    if (DebugOpen)
    {
        /*ATBM2040DrvChipDebugUHFFilterCalibrate(pTuner,pIndex,&CenterFreqMHz,&FltrGmSwitch,&FltrManual,&DebugOpen);*/
        CenterFreqMHz = (UINT32)(CenterFreqMHz * RFRatio + RFOffset + 0.5);
        uhf_filt_cal_ctrl_fail = 0x16;
        uhf_filt_cal_ctrl_suc = 0x17;
    }
#endif


    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x05, 0x0A, (UINT8)(CenterFreqMHz & 0xFF)); /*uhf_filt_freq[7:0]*/
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x05, 0x0B, (UINT8)((CenterFreqMHz >> 8) & 0x03)); /*uhf_filt_freq[9:8]*/
    if ((ATBM_LEOG_CHIP_ID == pTuner->ChipID)
            || (ATBM_LEOF_CHIP_ID == pTuner->ChipID) || (ATBM_LEOE_CHIP_ID == pTuner->ChipID))
    {
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x05, 0x1C, 0x02); /*uhf_fltr_count_h[1:0]*/
    }

    /*reset UHF filter calibration module*/
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x05, 0x0D, 0); /*uhf_filt_rst_fcal*/
    ATBM2040Delay(10);
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x05, 0x0D, 1); /*uhf_filt_rst_fcal*/
    ATBM2040Delay(10);

    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x05, 0x0E, 1); /*uhf_filt_en_cal*/



    ATBM2040Delay(400);

    ret |= ATBM2040RegRead(u8TunerIndex, I2CAddr, 0x05, 0x10, &filt_fcal_busy); /*uhf_filt_fcal_busy*/
    if (0x01 == filt_fcal_busy)
    {
        ATBM2040Delay(200);/*waiting the state changed!*/
    }
    ret |= ATBM2040RegRead(u8TunerIndex, I2CAddr, 0x05, 0x11, &filt_fcal_fail); /*uhf_filt_fcal_fail*/
    if (0x01 == filt_fcal_fail)
    {
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x05, 0x0C, uhf_filt_cal_ctrl_fail); /*uhf_filt_cal_ctrl[5:0]*/
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x12, 0x2F, 0x00); /*uhf_filt_cal_clk_en*/

        /* UHF filter frequency index setting*/
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x05, 0x0A, pIndex->uhf_filt_freq & 0xFF); /*uhf_filt_freq[7:0]*/
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x05, 0x0B, (pIndex->uhf_filt_freq >> 8) & 0x03); /*uhf_filt_freq[9:8]*/
    }
    else
    {
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x05, 0x0C, uhf_filt_cal_ctrl_suc); /*uhf_filt_cal_ctrl[5:0]*/
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x12, 0x2F, 0x00); /*uhf_filt_cal_clk_en*/

        if (ATBM_LEOG_CHIP_ID == pTuner->ChipID)
        {
            UINT8 uhf_fltr_fcal_gm_gain = 0;
            ret |= ATBM2040RegRead(u8TunerIndex, I2CAddr, 0x05, 0x1D, &uhf_fltr_fcal_gm_gain); /*uhf_fltr_fcal_gm_gain */
            FltrGmSwitch = (((UINT32)uhf_fltr_fcal_gm_gain + 1)*225 + 50)/100;
            FltrManual = uhf_fltr_fcal_gm_gain;
        }
    }

    if (DebugOpen)
    {
#ifdef ATBM2040_CHIP_DEBUG_OPEN
        FltrManual = (UINT32)(FltrManual * ManaulRatio + 0.5);
        FltrGmSwitch = (UINT32)(FltrGmSwitch * GmRatio + 0.5);
#endif
    }
    else
    {
        if (ATBM_LEOG_CHIP_ID == pTuner->ChipID)
        {
            FltrManual = (FltrManual * 7 + 5) / 10;
        }
        else
        {
            FltrManual = (FltrManual * 6 + 5) / 10;
        }
        /*FltrGmSwitch = FltrGmSwitch * 1;*/
    }

    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x05, 0x14, 0x03); /*uhf_filt_fb_adj_ldo*/

    if(FltrGmSwitch > 0x3F)
    {
        FltrGmSwitch = 0x3F;
    }
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x05, 0x12, (UINT8)FltrGmSwitch); /*uhf_fltr_gm_switch[5:0]*/

    if (DebugOpen)
    {
        if ((ATBM_LEOG_CHIP_ID == pTuner->ChipID)
                || (ATBM_LEOF_CHIP_ID == pTuner->ChipID) || (ATBM_LEOE_CHIP_ID == pTuner->ChipID)
                || (ATBM_LEOD_CHIP_ID == pTuner->ChipID) || (ATBM_LEOC_CHIP_ID == pTuner->ChipID))
        {
            ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x00, 0x29, 0x1F); /*params_agc_set_en[5:0]*/
        }
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x05, 0x16, 0x01); /*uhf_filt_manual_en*/
    }
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x05, 0x15, (UINT8)FltrManual); /*uhf_filt_manual[5:0]*/

    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x04, 0x0A, 0x01); /*lna_band*/

    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x04, 0x01, 0x00); /*lna_ldo_pd*/

    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x14, 0x39, 0x01); /*params_freq_path*/

    if (ATBM_LEOG_CHIP_ID == pTuner->ChipID)
    {
        ret |= ATBM2040RegWrite(u8TunerIndex,I2CAddr,0x23,0xBE, 0x05); /*params_use_leoe*/
        if ((ATBM2040_SIGNAL_MODE_DVBT == pTuner->SignalMode) && (pTuner->FreqKHz <= 800000))
        {
#if 0
            FltrGmSwitch = (FltrGmSwitch * 3 + 1) / 2;
            if (FltrGmSwitch > 0x3F)
            {
                FltrGmSwitch = 0x3F;
            }
            FltrGmSwitch &= 0x3F;
#endif
            FltrGmSwitch |= 0x40;
            ret |= ATBM2040RegWrite(u8TunerIndex,I2CAddr,0x20,0x63, 0x00); /*params_uhf_filter_on_nplusm*/
        }
        else
        {
            FltrGmSwitch &= 0xBF; /*b'1011 1111*/
            if(ATBM2040_SIGNAL_MODE_ATV == pTuner->SignalMode)
            {
                ret |= ATBM2040RegWrite(u8TunerIndex,I2CAddr,0x20,0x63, 0x00); /*params_uhf_filter_on_nplusm*/
            }
            else
            {
                ret |= ATBM2040RegWrite(u8TunerIndex,I2CAddr,0x20,0x63, 0x01); /*params_uhf_filter_on_nplusm*/
            }
        }
        ret |= ATBM2040RegWrite(u8TunerIndex,I2CAddr,0x12,0x1A, (UINT8)FltrGmSwitch);
    }
    return ret;
}


/************************************************************
*Description: VHF1 module setting
*************************************************************/
static ATBM2040_ERROR_e ATBM2040DrvVHF1Setting(UINT8 u8TunerIndex, ATBM2040_t *pTuner)
{
    ATBM2040_ERROR_e ret = ATBM2040_NO_ERROR;
    ATBM2040VHF1ChIndex_t *pIndex = NULL;
    UINT8 tmpReg = 0, cap_cal = 0;
    UINT8 i = 0;
    UINT8 I2CAddr = pTuner->Config.I2CAddr;

    pIndex = ATBM2040DrvVHF1FreqIndex(u8TunerIndex, (pTuner->FreqKHz + 500) / 1000);

    if(ATBM_LEOG_CHIP_ID == pTuner->ChipID)
    {                
        ret |= ATBM2040RegWrite(u8TunerIndex,I2CAddr,0x23,0xBE, 0x05); /*params_use_leoe*/
        ret |= ATBM2040RegWrite(u8TunerIndex,I2CAddr,0x20,0x63, 0x00); /*params_uhf_filter_on_nplusm*/
    }

    /*VHF filter registers setting*/
    ATBM2040RegRevise(u8TunerIndex, VHF1FilterReg, ATBM2040_VHF1_FILTER_REG_CNT, 0x06, 0x29, pIndex->vhf_fltr_ref_code);
    ATBM2040RegListWrite(u8TunerIndex, I2CAddr, VHF1FilterReg, ATBM2040_VHF1_FILTER_REG_CNT);
    if ((ATBM_LEOG_CHIP_ID == pTuner->ChipID)
            || (ATBM_LEOF_CHIP_ID == pTuner->ChipID) || (ATBM_LEOE_CHIP_ID == pTuner->ChipID))
    {
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x05, 0x21, 0x01); /*vhf2_fltr_pwdn_ldo*/
    }

    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x12, 0x2E, 0x01); /*Vhf_fltr_cal_clk_en*/

    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x06, 0x24, 0x00); /*Vhf_fltr_cal_reset*/
    ATBM2040Delay(1000);
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x06, 0x24, 0x01); /*Vhf_fltr_cal_reset*/

    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x06, 0x26, 0x01); /*Vhf_fltr_start*/

    while (i < 10)
    {
        ATBM2040Delay(10);
        ret |= ATBM2040RegRead(u8TunerIndex, I2CAddr, 0x06, 0x32, &tmpReg); /*vhf_fltr_cal_busy*/
        if (0x00 == tmpReg)
        {
            break;
        }
        i++;
    }

    ret |= ATBM2040RegRead(u8TunerIndex, I2CAddr, 0x06, 0x34, &cap_cal); /*vhf_fltr_ref_code_out*/

    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x12, 0x2E, 0x00); /*Vhf_fltr_cal_clk_en*/
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x06, 0x25, 0x00); /*Vhf_fltr_cal_sel*/

    ret |= ATBM2040RegRead(u8TunerIndex, I2CAddr, 0x06, 0x33, &tmpReg); /*vhf_fltr_cal_fail*/
    if (0x00 == tmpReg)
    {
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x06, 0x29, cap_cal); /*vhf_fltr_code_ref*/
    }

    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x04, 0x0A, 0x00); /*lna_band*/
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x04, 0x01, 0x00); /*lna_ldo_pd*/
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x14, 0x39, 0x00); /*params_freq_path*/
    return ret;
}


/************************************************************
*Description: VHF2 module setting
*************************************************************/
static ATBM2040_ERROR_e ATBM2040DrvVHF2Setting(UINT8 u8TunerIndex, ATBM2040_t *pTuner)
{
    ATBM2040_ERROR_e ret = ATBM2040_NO_ERROR;
    UINT8 filt_fcal_busy = 0, filt_fcal_fail = 0;
    UINT8 uhf_filt_cal_ctrl_fail = 0, uhf_filt_cal_ctrl_suc = 0;
    UINT32 CenterFreqMHz = 0;
    UINT32 FltrGmSwitch = 0, FltrManual = 0;
    ATBM2040VHF2ChIndex_t *pIndex = NULL;
    UINT8 vhf2_fltr_fcal_gm_gain = 0;
    UINT8 I2CAddr = pTuner->Config.I2CAddr;

    if(ATBM_LEOG_CHIP_ID == pTuner->ChipID)
    {    
        if(ATBM2040_SIGNAL_MODE_ATV == pTuner->SignalMode)
        {
            ret |= ATBM2040RegWrite(u8TunerIndex,I2CAddr,0x23,0xBE, 0x05); /*params_use_leoe*/
        }
        else
        {
            ret |= ATBM2040RegWrite(u8TunerIndex,I2CAddr,0x23,0xBE, 0x07); /*params_use_leoe*/
        }
        ret |= ATBM2040RegWrite(u8TunerIndex,I2CAddr,0x20,0x63, 0x00); /*params_uhf_filter_on_nplusm*/
    }

    CenterFreqMHz = (pTuner->FreqKHz+500)/1000;
    ret |= ATBM2040RegWrite(u8TunerIndex,I2CAddr,0x04,0x01, 0x01); /*lna_ldo_pd*/

    /*Setting the VHF2 filter and  Calibration*/ 
    if((0 != pTuner->LowCousumMd)&&(ATBM_LEOG_CHIP_ID == pTuner->ChipID))
    {
        ATBM2040Reg_t TmpReg = {0x05,0x09, 0x04};
        ATBM2040RegListWriteExt(u8TunerIndex,I2CAddr,LeoEVHF2FltCalibReg,ATBM2040_LEOE_VHF2_FILTER_REG_CNT,TmpReg);
    }
    else
    {
        ATBM2040Reg_t TmpReg = {0x05,0x09, 0x0B};
        ATBM2040RegListWriteExt(u8TunerIndex,I2CAddr,LeoEVHF2FltCalibReg,ATBM2040_LEOE_VHF2_FILTER_REG_CNT,TmpReg);
    }

    /* VHF2 filter frequency index setting*/
    pIndex = ATBM2040DrvVHF2FreqIndex(u8TunerIndex, pTuner->FreqKHz);
    FltrGmSwitch = pIndex->vhf2_fltr_gm_switch;
    uhf_filt_cal_ctrl_fail = 0x1C;
    uhf_filt_cal_ctrl_suc = 0x1D;



    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x05, 0x0A, (UINT8)(CenterFreqMHz & 0xFF)); /*uhf_filt_freq[7:0]*/
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x05, 0x0B, (UINT8)((CenterFreqMHz >> 8) & 0x03)); /*uhf_filt_freq[9:8]*/
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x05, 0x1C, 0x00); /*uhf_fltr_count_h[1:0]*/

    /*reset VHF2 filter calibration module*/
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x05, 0x0D, 0); /*uhf_filt_rst_fcal*/
    ATBM2040Delay(10);
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x05, 0x0D, 1); /*uhf_filt_rst_fcal*/
    ATBM2040Delay(10);

    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x05, 0x0E, 1); /*uhf_filt_en_cal*/



    ATBM2040Delay(400);

    ret |= ATBM2040RegRead(u8TunerIndex, I2CAddr, 0x05, 0x24, &filt_fcal_busy); /*uhf_filt_fcal_busy*/
    if (0x01 == filt_fcal_busy)
    {
        ATBM2040Delay(200);/*waiting the state changed!*/
    }
    ret |= ATBM2040RegRead(u8TunerIndex, I2CAddr, 0x05, 0x25, &filt_fcal_fail); /*uhf_filt_fcal_fail*/
    if (0x01 == filt_fcal_fail)
    {
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x05, 0x23, uhf_filt_cal_ctrl_fail); /*vhf2_filt_cal_ctrl[4:0]*/
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x12, 0x2F, 0x00); /*uhf_filt_cal_clk_en*/

        /* UHF filter frequency index setting*/
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x05, 0x0A, pIndex->vhf2_filt_freq & 0xFF); /*uhf_filt_freq[7:0]*/
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x05, 0x0B, (pIndex->vhf2_filt_freq >> 8) & 0x03); /*uhf_filt_freq[9:8]*/

        if(ATBM_LEOG_CHIP_ID == pTuner->ChipID)
        {
            FltrGmSwitch = ((pIndex->vhf2_fltr_gm_switch + 1)*15 + 5)/10;
            FltrManual = (UINT8)((pIndex->vhf2_fltr_gm_switch*7 + 5)/10);
        }
        else
        {
            FltrManual = (UINT8)((FltrGmSwitch*6+5)/10);
        }
    }
    else
    {
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x05, 0x23, uhf_filt_cal_ctrl_suc); /*uhf_filt_cal_ctrl[5:0]*/
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x12, 0x2F, 0x00); /*uhf_filt_cal_clk_en*/

        ret |= ATBM2040RegRead(u8TunerIndex, I2CAddr, 0x05, 0x28, &vhf2_fltr_fcal_gm_gain); /*vhf2_fltr_fcal_gm_gain[5:0] */
        if(ATBM_LEOG_CHIP_ID == pTuner->ChipID)
        {
            FltrGmSwitch = ((vhf2_fltr_fcal_gm_gain + 1)*15 + 5)/10;
            FltrManual = (UINT8)((vhf2_fltr_fcal_gm_gain*7 + 5)/10);
        }
        else
        {
            FltrGmSwitch = ((vhf2_fltr_fcal_gm_gain + 1)*18 + 5)/10;
            FltrManual = (UINT8)((vhf2_fltr_fcal_gm_gain*6+5)/10);
        }
    }


    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x05, 0x12, (UINT8)FltrGmSwitch); /*uhf_fltr_gm_switch[5:0]*/
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x05, 0x15, (UINT8)FltrManual); /*uhf_filt_manual[5:0]*/


    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x04, 0x0A, 0x00); /*lna_band*/
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x04, 0x01, 0x00); /*lna_ldo_pd*/
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x14, 0x39, 0x00); /*params_freq_path*/
    return ret;
}


/************************************************************
*Description: Software reset, all registers will be reset
*************************************************************/
ATBM2040_ERROR_e ATBM2040DrvSwReset(UINT8 u8TunerIndex, ATBM2040_t *pTuner)
{
    ATBM2040_ERROR_e ret = ATBM2040_NO_ERROR;
    UINT8 tmpReg = 0;
    UINT8 I2CAddr = pTuner->Config.I2CAddr;

    /*Reset All*/
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x00, 0x0F, 0x00); /*soft_rst_cfg[4:0]*/
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x00, 0x04, 0x02); /*[0] I2c_clk_sel    [1] i2c_sys_clk_lck_enb*/
    /*Switch I2C clock to OSC clock*/
    ret |= ATBM2040RegRead(u8TunerIndex, I2CAddr, 0x00, 0x04, &tmpReg); /*[0] I2c_clk_sel    [1] i2c_sys_clk_lck_enb*/
    if (0x02 != tmpReg)
    {
        ATBM2040Delay(500);
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x00, 0x04, 0x02); /*[0] I2c_clk_sel    [1] i2c_sys_clk_lck_enb*/
    }
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x00, 0x0C, 0x01); /*soft_rst_all*/
    ATBM2040Delay(500);
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x00, 0x0C, 0x00); /*soft_rst_all*/

    ret |= ATBM2040RegRead(u8TunerIndex, I2CAddr, 0x00, 0x00, &pTuner->ChipID);
    if (ATBM_LEOC_CHIP_ID == pTuner->ChipID)
    {
        ret |= ATBM2040RegRead(u8TunerIndex, I2CAddr, 0x10, 0x15, &tmpReg);
        if (0xFF != tmpReg)
        {
            pTuner->ChipID = ATBM_LEOD_CHIP_ID;
            ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x00, 0x00, ATBM_LEOD_CHIP_ID);
        }
    }
#ifdef ATBM2040_CHIP_DEBUG_OPEN
    if ((ATBM_LEOF_CHIP_ID == pTuner->ChipID) && (0xFF != pTuner->ForceChipID))
    {
        pTuner->ChipID = pTuner->ForceChipID;
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x00, 0x00, pTuner->ForceChipID);
    }
#endif
    ATBM2040ChipID = pTuner->ChipID;
    return ret;
}

/************************************************************
*Description: BB power target setting
*************************************************************/
static ATBM2040_ERROR_e ATBM2040DrvAgcPowerTargetSet(UINT8 u8TunerIndex, ATBM2040_t *pTuner)
{
    ATBM2040_ERROR_e ret = ATBM2040_NO_ERROR;
    UINT32 i = 0;
    ATBM2040Reg_t *pReg = NULL;
    SINT32 *pData = NULL;
    ATBM2040AgcPowerTarget_t *pTarget = AgcPowerTargetDTMB;
    UINT32 FreqMHz = (pTuner->FreqKHz + 500) / 1000;
    UINT32 Cnt = sizeof(AgcPowerTargetDTMB) / sizeof(AgcPowerTargetDTMB[0]);
    UINT8 I2CAddr = pTuner->Config.I2CAddr;

#ifdef ATBM2040_CHIP_DEBUG_OPEN
    if (0 == ATBM2040DrvChipDebugPowerTargetSet(u8TunerIndex, pTuner, AgcPowerTargetReg, ATBM2040_AGC_POW_TARGET_REG_CNT))
    {
        return ATBM2040_NO_ERROR;
    }
#endif

    switch (pTuner->Config.Mode)
    {
        case ATBM2040_SIGNAL_MODE_DTMB:
            if ((ATBM_LEOG_CHIP_ID == pTuner->ChipID)
                    || (ATBM_LEOF_CHIP_ID == pTuner->ChipID) || (ATBM_LEOE_CHIP_ID == pTuner->ChipID))
            {
                pTarget = LeoEAgcPowerTargetDTMB;
                Cnt = sizeof(LeoEAgcPowerTargetDTMB) / sizeof(LeoEAgcPowerTargetDTMB[0]);
            }
            else if ((ATBM_LEOC_CHIP_ID == pTuner->ChipID) || (ATBM_LEOD_CHIP_ID == pTuner->ChipID))
            {
                pTarget = LeoCAgcPowerTargetDTMB;
                Cnt = sizeof(LeoCAgcPowerTargetDTMB) / sizeof(LeoCAgcPowerTargetDTMB[0]);
            }
            else
            {
                pTarget = AgcPowerTargetDTMB;
                Cnt = sizeof(AgcPowerTargetDTMB) / sizeof(AgcPowerTargetDTMB[0]);
            }
            break;
        case ATBM2040_SIGNAL_MODE_DVBT:
            if(ATBM_LEOG_CHIP_ID == pTuner->ChipID)
            {
                pTarget = LeoGAgcPowerTargetDVBT;
                Cnt = sizeof(LeoGAgcPowerTargetDVBT)/sizeof(LeoGAgcPowerTargetDVBT[0]);
            }
            else if((ATBM_LEOF_CHIP_ID == pTuner->ChipID)||(ATBM_LEOE_CHIP_ID == pTuner->ChipID))
            {
                pTarget = LeoEAgcPowerTargetDVBT;
                Cnt = sizeof(LeoEAgcPowerTargetDVBT) / sizeof(LeoEAgcPowerTargetDVBT[0]);
            }
            else if ((ATBM_LEOC_CHIP_ID == pTuner->ChipID) || (ATBM_LEOD_CHIP_ID == pTuner->ChipID))
            {
                pTarget = LeoCAgcPowerTargetDVBT;
                Cnt = sizeof(LeoCAgcPowerTargetDVBT) / sizeof(LeoCAgcPowerTargetDVBT[0]);
            }
            else
            {
                pTarget = AgcPowerTargetDVBT;
                Cnt = sizeof(AgcPowerTargetDVBT) / sizeof(AgcPowerTargetDVBT[0]);
            }
            break;
        case ATBM2040_SIGNAL_MODE_DVBC:
            if ((ATBM_LEOG_CHIP_ID == pTuner->ChipID)
                    || (ATBM_LEOF_CHIP_ID == pTuner->ChipID) || (ATBM_LEOE_CHIP_ID == pTuner->ChipID))
            {
                pTarget = LeoEAgcPowerTargetDVBC;
                Cnt = sizeof(LeoEAgcPowerTargetDVBC) / sizeof(LeoEAgcPowerTargetDVBC[0]);
            }
            else
            {
                pTarget = AgcPowerTargetDVBC;
                Cnt = sizeof(AgcPowerTargetDVBC) / sizeof(AgcPowerTargetDVBC[0]);
            }
            break;
        case ATBM2040_SIGNAL_MODE_ATV:
            if(ATBM_LEOG_CHIP_ID == pTuner->ChipID)
            {
                pTarget = LeoGAgcPowerTargetATV;
                Cnt = sizeof(LeoGAgcPowerTargetATV)/sizeof(LeoGAgcPowerTargetATV[0]);
            }
            else if((ATBM_LEOF_CHIP_ID == pTuner->ChipID)||(ATBM_LEOE_CHIP_ID == pTuner->ChipID))
            {
                pTarget = LeoEAgcPowerTargetATV;
                Cnt = sizeof(LeoEAgcPowerTargetATV) / sizeof(LeoEAgcPowerTargetATV[0]);
            }
            else if ((ATBM_LEOC_CHIP_ID == pTuner->ChipID) || (ATBM_LEOD_CHIP_ID == pTuner->ChipID))
            {
                pTarget = LeoCAgcPowerTargetATV;
                Cnt = sizeof(LeoCAgcPowerTargetATV) / sizeof(LeoCAgcPowerTargetATV[0]);
            }
            else
            {
                pTarget = AgcPowerTargetATV;
                Cnt = sizeof(AgcPowerTargetATV) / sizeof(AgcPowerTargetATV[0]);
            }
            break;
        default:
            break;
    }
    for (i = 0; i < Cnt; i++)
    {
        if ((FreqMHz > pTarget->StartFreqMHz)
                && (FreqMHz <= pTarget->EndFreqMHz))
        {
            break;
        }
        pTarget++;
    }

    pReg = AgcPowerTargetReg;
    pData = pTarget->RegVal;

    for (i = 0; i < ATBM2040_AGC_POW_TARGET_REG_CNT; i++)
    {
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, pReg[i].BaseReg, pReg[i].OffReg, (UINT8)pData[i]);
    }

    return ret;
}

static ATBM2040_ERROR_e ATBM2040DrvDagcAmpRefSet(UINT8 u8TunerIndex, ATBM2040_t *pTuner)
{
    ATBM2040_ERROR_e ret = ATBM2040_NO_ERROR;
    UINT32 i = 0;
    ATBM2040Reg_t *pReg = NULL;
    UINT8 *pData = NULL;
    DagcAmpRef_t *pDagcAmpRef = &DagcAmpRefDTMB;
    UINT8 I2CAddr = pTuner->Config.I2CAddr;

    switch (pTuner->Config.Mode)
    {
        case ATBM2040_SIGNAL_MODE_DTMB:
            pDagcAmpRef = &DagcAmpRefDTMB;
            break;
        case ATBM2040_SIGNAL_MODE_DVBT:
            pDagcAmpRef = &DagcAmpRefDVBT;
            break;
        case ATBM2040_SIGNAL_MODE_DVBC:
            pDagcAmpRef = &DagcAmpRefDVBC;
            break;
        case ATBM2040_SIGNAL_MODE_ATV:
            pDagcAmpRef = &DagcAmpRefAnalog;
            if (ATBM_LEOG_CHIP_ID == pTuner->ChipID)
            {
                pDagcAmpRef->Dagc2AmpRef0Low = 0x70;
            }
            else
            {
                pDagcAmpRef->Dagc2AmpRef0Low = 0x80;
            }
            break;
        default:
            break;
    }

    pReg = DagcAmpRefReg;
    pData = (UINT8 *)pDagcAmpRef;
    for (i = 0; i < ATBM2040_DAGC_AMP_REF_REG_CNT; i++)
    {
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, pReg[i].BaseReg, pReg[i].OffReg, pData[i]);
    }

    return ret;
}

static ATBM2040_ERROR_e ATBM2040DrvVGAGainThrSet(UINT8 u8TunerIndex, ATBM2040_t *pTuner)
{
    ATBM2040_ERROR_e ret = ATBM2040_NO_ERROR;
    UINT8 I2CAddr = pTuner->Config.I2CAddr;

    switch (pTuner->Config.Mode)
    {
        case ATBM2040_SIGNAL_MODE_DTMB:
            ATBM2040RegListWrite(u8TunerIndex, I2CAddr, VGAGainThrDTMBReg, ATBM2040_VGA_GAIN_THR_DTMB_REG_CNT);
            break;
        case ATBM2040_SIGNAL_MODE_DVBT:
            ATBM2040RegListWrite(u8TunerIndex, I2CAddr, VGAGainThrDVBTReg, ATBM2040_VGA_GAIN_THR_DVBT_REG_CNT);
            break;
        case ATBM2040_SIGNAL_MODE_DVBC:
            ATBM2040RegListWrite(u8TunerIndex, I2CAddr, VGAGainThrDVBCReg, ATBM2040_VGA_GAIN_THR_DVBC_REG_CNT);
            break;
        case ATBM2040_SIGNAL_MODE_ATV:
            if(ATBM_LEOG_CHIP_ID == pTuner->ChipID)
            {
                ATBM2040RegListWrite(u8TunerIndex,I2CAddr,LeoGVGAGainThrATVReg,ATBM2040_LEOG_VGA_GAIN_THR_ATV_REG_CNT);
            }
            else if((ATBM_LEOF_CHIP_ID == pTuner->ChipID)||(ATBM_LEOE_CHIP_ID == pTuner->ChipID))
            {
                ATBM2040RegListWrite(u8TunerIndex,I2CAddr,LeoEVGAGainThrATVReg,ATBM2040_LEOE_VGA_GAIN_THR_ATV_REG_CNT);
            }
            else
            {
                ATBM2040RegListWrite(u8TunerIndex,I2CAddr,VGAGainThrATVReg,ATBM2040_VGA_GAIN_THR_ATV_REG_CNT);
            }
            break;
        default:
            break;
    }

    return ret;
}


/************************************************************
*Description: from  IF level to dac_fltr_gain_ctrl(1003H)
*************************************************************/
static UINT8 ATBM2040DrvDACGainCode(UINT8 u8TunerIndex, ATBM2040_t *pTuner, ATBM2040_IF_OUT_LEVEL_e IFLevel)
{
    UINT8 GainCode = 0x05;
    if (ATBM_LEOG_CHIP_ID == pTuner->ChipID)
    {
        switch (IFLevel)
        {
            case ATBM2040_IF_OUT_LEVEL1:
                GainCode = 0x00 ; /*850 mV */
                break;
            case ATBM2040_IF_OUT_LEVEL2:
                GainCode = 0x01 ; /*1150mV*/
                break;
            case ATBM2040_IF_OUT_LEVEL3:
                GainCode = 0x02 ; /*1650mV*/
                break;
            case ATBM2040_IF_OUT_LEVEL4:
                GainCode = 0x03 ; /*1650mV*/
                break;
            default:
                GainCode = 0x00;
                break;
        }
    }
    else if((ATBM_LEOF_CHIP_ID == pTuner->ChipID)||(ATBM_LEOE_CHIP_ID == pTuner->ChipID)
            ||(ATBM_LEOD_CHIP_ID == pTuner->ChipID))
    {
        switch(IFLevel)
        {
            case ATBM2040_IF_OUT_LEVEL1:
                GainCode = 0x00 ; /*850 mV */
                break;
            case ATBM2040_IF_OUT_LEVEL2:
                GainCode = 0x02 ; /*1200mV*/
                break;
            case ATBM2040_IF_OUT_LEVEL3:
            case ATBM2040_IF_OUT_LEVEL4:
                GainCode = 0x03 ; /*1600mV*/
                break;
            default:
                GainCode = 0x00;
                break;
        }
    }
    else
    {
        switch(IFLevel)
        {
            case ATBM2040_IF_OUT_LEVEL1:
                GainCode = 0x00 + (UINT8)((pTuner->IFLevelSpinner>0x02)?(0x02):(pTuner->IFLevelSpinner));
                break;
            case ATBM2040_IF_OUT_LEVEL2:
                GainCode = 0x03 + (UINT8)((pTuner->IFLevelSpinner>0x03)?(0x03):(pTuner->IFLevelSpinner));
                break;
            case ATBM2040_IF_OUT_LEVEL3:
            case ATBM2040_IF_OUT_LEVEL4:
                GainCode = 0x07;
                break;
            default:
                GainCode = 0x04;
                break;
        }
    }
    return GainCode;
}

/************************************************************
*Description: set pin config
*************************************************************/
static ATBM2040_ERROR_e ATBM2040DrvGPOStateSet(UINT8 u8TunerIndex, ATBM2040_t *pTuner, ATBM2040PinCfg_t *pPinCfg, UINT32 PinNum)
{
    ATBM2040_ERROR_e ret = ATBM2040_NO_ERROR;
    UINT32 i = 0;
    UINT8 I2CAddr = pTuner->Config.I2CAddr;
    for (i = 0; i < PinNum; i++)
    {
        if ((ATBM2040_PIN_NAME_GPO1 == pPinCfg->PinName) && (pTuner->Config.FEFMode != ATBM2040_FEF_EXTERNAL_GPO1))
        {
            ret = ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x03, 0x00, (ATBM2040_GPO_STATE_LOW == pPinCfg->PinState) ? (0x01) : (0x03)); /*gpo1_oe,gpo1_dat*/
        }
        else if ((ATBM2040_PIN_NAME_GPO2 == pPinCfg->PinName) && (pTuner->Config.FEFMode != ATBM2040_FEF_EXTERNAL_GPO2))
        {
            ret = ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x03, 0x01, (ATBM2040_GPO_STATE_LOW == pPinCfg->PinState) ? (0x01) : (0x03)); /*gpo2_oe,gpo2_dat*/
        }
        pPinCfg++;
    }

    return ret;
}

/************************************************************
*Description: set IF data output path
*************************************************************/
static ATBM2040_ERROR_e ATBM2040DrvIFOutPath(UINT8 u8TunerIndex, ATBM2040_t *pTuner, ATBM2040_IF_PATH_e IFPath)
{
    ATBM2040_ERROR_e ret = ATBM2040_NO_ERROR;
    UINT8 I2CAddr = pTuner->Config.I2CAddr;
    switch (IFPath)
    {
        case ATBM2040_IF_PATH_1:
            ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x16, 0x08, 0x00); /*params_analog_t_hlo*/
            ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x15, 0x3F, 0x01); /*params_dagc_set_0*/
            break;
        case ATBM2040_IF_PATH_2:
            ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x16, 0x08, 0x01); /*params_analog_t_hlo*/
            ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x15, 0x3F, 0x00); /*params_dagc_set_0*/
            break;
        default:
            break;
    }
    return ret;
}
/************************************************************
*Description: set clock out config
*************************************************************/
static ATBM2040_ERROR_e ATBM2040DrvClkOutControl(UINT8 u8TunerIndex, ATBM2040_t *pTuner)
{
    ATBM2040_ERROR_e ret = ATBM2040_NO_ERROR;
    UINT8 I2CAddr = pTuner->Config.I2CAddr;
    if (ATBM2040_CLK_OUT_ENABLE == pTuner->Config.ClkOut.ClkOutEnable)
    {
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x0E, 0x09, 0x00); /*osc_pd_out*/
        if ((ATBM_LEOG_CHIP_ID == pTuner->ChipID) || (ATBM_LEOF_CHIP_ID == pTuner->ChipID))
        {
            switch (pTuner->Config.ClkOut.ClkOutAmp)
            {
                case ATBM2040_CLK_OUT_AMP_L1:
                    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x0E, 0x0F, 0x10); /*osc_clk_out_res_a*/
                    break;
                case ATBM2040_CLK_OUT_AMP_L2:
                    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x0E, 0x0F, 0x08); /*osc_clk_out_res_a*/
                    break;
                case ATBM2040_CLK_OUT_AMP_L3:
                    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x0E, 0x0F, 0x04); /*osc_clk_out_res_a*/
                    break;
                case ATBM2040_CLK_OUT_AMP_L4:
                    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x0E, 0x0F, 0x02); /*osc_clk_out_res_a*/
                    break;
                case ATBM2040_CLK_OUT_AMP_L5:
                    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x0E, 0x0F, 0x01); /*osc_clk_out_res_a*/
                    break;
                default:
                    break;
            }
        }
        else
        {
            switch (pTuner->Config.ClkOut.ClkOutAmp)
            {
                case ATBM2040_CLK_OUT_AMP_L1:
                    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x0E, 0x10, 0x01); /*osc_clk_out_res_b*/
                    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x0E, 0x12, 0x06); /*osc_core_current*/
                    break;
                case ATBM2040_CLK_OUT_AMP_L2:
                    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x0E, 0x10, 0x01); /*osc_clk_out_res_b*/
                    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x0E, 0x12, 0x06); /*osc_core_current*/
                    break;
                case ATBM2040_CLK_OUT_AMP_L3:
                    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x0E, 0x10, 0x01); /*osc_clk_out_res_b*/
                    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x0E, 0x12, 0x06); /*osc_core_current*/
                    break;
                case ATBM2040_CLK_OUT_AMP_L4:
                    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x0E, 0x10, 0x01); /*osc_clk_out_res_b*/
                    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x0E, 0x12, 0x06); /*osc_core_current*/
                    break;
                default:
                    break;
            }
        }
    }
    else
    {
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x0E, 0x09, 0x01); /*osc_pd_out*/
        if ((ATBM_LEOG_CHIP_ID == pTuner->ChipID) || (ATBM_LEOF_CHIP_ID == pTuner->ChipID))
        {
            ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x0E, 0x0F, 0x01); /*osc_clk_out_res_a*/
        }
        else
        {
            ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x0E, 0x10, 0x01); /*osc_clk_out_res_b*/
            ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x0E, 0x12, 0x02); /*osc_core_current*/
        }
    }

    return ret;
}
static ATBM2040_ERROR_e ATBM2040DrvMixerReset(UINT8 u8TunerIndex, ATBM2040_t *pTuner)
{
    ATBM2040_ERROR_e ret = ATBM2040_NO_ERROR;
    UINT8 I2CAddr = pTuner->Config.I2CAddr;

    if(pTuner->FreqKHz <= ATBM2040_MIXER_MD_DIVISION_KHZ)
    {
        ret |= ATBM2040RegWrite(u8TunerIndex,I2CAddr,0x22,0x14, 0x00); /*params_mixer_en_4x*/
        ret |= ATBM2040RegWrite(u8TunerIndex,I2CAddr,0x12,0x29, 0x01); /*mixr_pd_8x(RFin>333MHz 1:4x;RFin<=333MHz,0:8x) */
        ATBM2040Delay(3000);
        ret |= ATBM2040RegWrite(u8TunerIndex,I2CAddr,0x12,0x29, 0x00); /*mixr_pd_8x(RFin>333MHz 1:4x;RFin<=333MHz,0:8x) */
    }
    else
    {
        ret |= ATBM2040RegWrite(u8TunerIndex,I2CAddr,0x22,0x14, 0x01); /*params_mixer_en_4x*/
        ret |= ATBM2040RegWrite(u8TunerIndex,I2CAddr,0x12,0x29, 0x00); /*mixr_pd_8x(RFin>333MHz 1:4x;RFin<=333MHz,0:8x) */
        ATBM2040Delay(3000);
        ret |= ATBM2040RegWrite(u8TunerIndex,I2CAddr,0x12,0x29, 0x01); /*mixr_pd_8x(RFin>333MHz 1:4x;RFin<=333MHz,0:8x) */
    }
    if((ATBM_LEOG_CHIP_ID == pTuner->ChipID)
        ||(ATBM_LEOF_CHIP_ID == pTuner->ChipID)||(ATBM_LEOE_CHIP_ID == pTuner->ChipID))
    {
        UINT8 mixr_vhf_pd = 0,mixr_vhf2_pd = 0,mixr_uhf_pd = 0;
        if(pTuner->FreqKHz < ATBM2040_VHF2_LOW_KHZ)
        {/*VHF1*/
            mixr_vhf_pd = 0x00;
            mixr_vhf2_pd = 0x01;
            mixr_uhf_pd = 0x01;
        }
        else if(pTuner->FreqKHz <= ATBM2040_UHF_LOW_KHZ)
        {/*VHF2*/
            mixr_vhf_pd = 0x01;
            mixr_vhf2_pd = 0x00;
            mixr_uhf_pd = 0x01;
        }
        else
        {/*UHF*/
            mixr_vhf_pd = 0x01;
            mixr_vhf2_pd = 0x01;
            mixr_uhf_pd = 0x00;
        }
        ret |= ATBM2040RegWrite(u8TunerIndex,I2CAddr,0x12,0x41, mixr_vhf_pd);  /*mixr_vhf_pd */
        ret |= ATBM2040RegWrite(u8TunerIndex,I2CAddr,0x12,0x46, mixr_uhf_pd); /*mixr_uhf_pd */
        ret |= ATBM2040RegWrite(u8TunerIndex,I2CAddr,0x12,0x47, mixr_vhf2_pd); /*mixr_vhf2_pd */
    }
    if(pTuner->FreqKHz <= ATBM2040_MIXER_MD_DIVISION_KHZ)
    {
        ret |= ATBM2040RegWrite(u8TunerIndex,I2CAddr,0x00,0x22, 0x02);
    }
    else
    {
        ret |= ATBM2040RegWrite(u8TunerIndex,I2CAddr,0x00,0x22, 0x03);
    }
    
    return ret;
}

static ATBM2040_ERROR_e ATBM2040DrvBWSet(UINT8 u8TunerIndex, ATBM2040_t *pTuner)
{
    ATBM2040_ERROR_e ret = ATBM2040_NO_ERROR;
    UINT32 BandWidth = 0;
    UINT8 I2CAddr = pTuner->Config.I2CAddr;
    UINT8 Reg146C = 0, Reg1A21 = 0, Reg1A22 = 0, Reg1A23 = 0;

    /*BandWidth*/
    Reg146C = 0x02;
    BandWidth = (8000000 + 128) / 256;
    if (pTuner->BWKHz <= 6000)
    {
        Reg146C = 0x00;
        BandWidth = (6000000 + 128) / 256;
        Reg1A21 = 0x45;
        Reg1A22 = 0x3C;
        Reg1A23 = 0x00;
    }
    else if (pTuner->BWKHz <= 7000)
    {
        Reg146C = 0x01;
        BandWidth = (7000000 + 128) / 256;
        Reg1A21 = 0xE7;
        Reg1A22 = 0x43;
        Reg1A23 = 0x00;
    }
    else if (pTuner->BWKHz <= 8000)
    {
        Reg146C = 0x02;
        BandWidth = (8000000 + 128) / 256;
        Reg1A21 = 0x4B;
        Reg1A22 = 0x4C;
        Reg1A23 = 0x00;
    }
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x14, 0x6C, Reg146C); /*params_sig_bw_mode [1:0]*/
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x14, 0x6D, BandWidth & 0xFF); /*params_signal_filter_bandwidth[7:0]*/
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x14, 0x6E, (BandWidth >> 8) & 0xFF); /*params_signal_filter_bandwidth[15:8]*/

    if (ATBM2040_SIGNAL_MODE_ATV == pTuner->Config.Mode)
    {
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x1A, 0x21, Reg1A21); /*sweep_freq_start_sfo*/
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x1A, 0x22, Reg1A22);
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x1A, 0x23, Reg1A23);
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x1A, 0x29, Reg146C); /*BW_MODE*/
    }

    return ret;
}

/************************************************************
*Description: Find Intercept according to center frequency
*************************************************************/
UINT16 ATBM2040DrvRSSIInterceptGet(UINT8 u8TunerIndex, ATBM2040_t *pTuner)
{
    UINT32 i = 0;
    UINT32 FreqMHz = 0;
    UINT32 FreqMHzTmp = 0xFFFFFFFF;
    UINT32 FreqMHzMin = 0xFFFFFFFF;
    ATBM2040RSSIIntercept_t *pIntercept_t = RSSIIntercept;

    FreqMHz = (pTuner->FreqKHz + 500) / 1000;
    for (i = 0; i < ATBM2040_RSSI_INTERCEPT_CNT; i++)
    {
        FreqMHzTmp = ATBM2040ABS((int)FreqMHz - (int)RSSIIntercept[i].FreqMHz);
        if (FreqMHzTmp < FreqMHzMin)
        {
            FreqMHzMin = FreqMHzTmp;
            pIntercept_t = &RSSIIntercept[i];
        }
    }


    return pIntercept_t->Intercept;
}

/************************************************************
*Description: set DWA
*************************************************************/
ATBM2040_ERROR_e ATBM2040DrvADCDwaSet(UINT8 u8TunerIndex, ATBM2040_t *pTuner)
{
    ATBM2040_ERROR_e ret = ATBM2040_NO_ERROR;
    UINT8 I2CAddr = pTuner->Config.I2CAddr;

    UINT32 i = 0;
    UINT8 agcState = 0, LoFinish = 0;
    for (i = 0; i < 400; i++)
    {
        ret |= ATBM2040RegRead(u8TunerIndex, I2CAddr, 0x14, 0xAC, &LoFinish);
        ret |= ATBM2040RegRead(u8TunerIndex, I2CAddr, 0x14, 0xAA, &agcState);
        if ((0x01 == LoFinish) && (0x02 == agcState))
        {
            break;
        }
        ATBM2040Delay(1000);
    }
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x0B, 0x0A, 0x00); /*adc_dwa_disable*/
    return ret;
}

/*Description: Setting the IF frequency calibration registers */
ATBM2040_ERROR_e ATBM2040DrvIFFilterSet(UINT8 u8TunerIndex, ATBM2040_t *pTuner)
{
    ATBM2040_ERROR_e ret = ATBM2040_NO_ERROR;
    UINT32 SignalIFFreq = 0;
    UINT8 tmpReg = 0;
    UINT8 I2CAddr = pTuner->Config.I2CAddr;

    if (ATBM_LEOG_CHIP_ID == pTuner->ChipID)
    {
        /*Setting IF frequency*/
        SignalIFFreq = pTuner->MixerIF >> 8; /* MixerIF/256 */
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x16, 0x0C, (0 - SignalIFFreq) & 0xFF); /*params_desired_signal_if_freq_h[7:0]*/
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x16, 0x0D, ((0 - SignalIFFreq) >> 8) & 0xFF); /*params_desired_signal_if_freq_h[15:8]*/
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x16, 0x0E, SignalIFFreq & 0xFF); /*params_desired_signal_if_freq_l[7:0]*/
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x16, 0x0F, (SignalIFFreq >> 8) & 0xFF); /*params_desired_signal_if_freq_l[15:8]*/
    }

    if((ATBM_LEOG_CHIP_ID == pTuner->ChipID)
        &&(ATBM2040_SIGNAL_MODE_DVBT == pTuner->SignalMode)&&(!ATBM2040_VHF_CHECK(pTuner->FreqKHz)))
    {
        tmpReg = 0x70;
    }
    else if(((ATBM_LEOG_CHIP_ID == pTuner->ChipID)||(ATBM_LEOF_CHIP_ID == pTuner->ChipID)
        ||(ATBM_LEOE_CHIP_ID == pTuner->ChipID))
        &&(ATBM2040_SIGNAL_MODE_DTMB == pTuner->SignalMode))
    {
        tmpReg = 0x70;
    }
    else
    {
        if((pTuner->MixerIF/1000 + pTuner->BWKHz/2) <= 8000)
        {
            tmpReg = 0x72;
        }
        else if((pTuner->MixerIF/1000 + pTuner->BWKHz/2) <= 9000)
        {
            tmpReg = 0x72; /*mingqing , from 0x71 to 0x72*/
        }
        else if((pTuner->MixerIF/1000 + pTuner->BWKHz/2) <= 10000)
        {
            tmpReg = 0x70;
        }
        else
        {
            tmpReg = 0x70;
        }
    }
    ret |= ATBM2040RegWrite(u8TunerIndex,I2CAddr,0x09,0x05, 0x00); /*if_fltr_ctrl_sel*/
    ret |= ATBM2040RegWrite(u8TunerIndex,I2CAddr,0x09,0x04, 0x00); /*if_fltr_cali_en*/
    ret |= ATBM2040RegWrite(u8TunerIndex,I2CAddr,0x09,0x14, tmpReg); /*if_filt_bw_ctrl[6:0]*/
    ret |= ATBM2040RegWrite(u8TunerIndex,I2CAddr,0x12,0x2D, 0x01); /*if_filtr_cal_clk_en*/
    ret |= ATBM2040RegWrite(u8TunerIndex,I2CAddr,0x09,0x07, 0x01); /*if_fltr_cali_disable, FILTER_CALI_DISABLE_3d3*/
    ret |= ATBM2040RegWrite(u8TunerIndex,I2CAddr,0x09,0x07, 0x00); /*if_fltr_cali_disable*/
    ret |= ATBM2040RegRead(u8TunerIndex,I2CAddr,0x12,0x16, &tmpReg);
    tmpReg |= 0x40; /*bit6: set 1*/
    tmpReg &= 0x7F; /*bit7: set 0*/
    ret |= ATBM2040RegWrite(u8TunerIndex,I2CAddr,0x12,0x16, tmpReg);  /*SAMPLE_DELAY<1:0>*/

    ret |= ATBM2040RegWrite(u8TunerIndex,I2CAddr,0x09,0x04, 0x01); /*if_fltr_cali_en*/
    ret |= ATBM2040RegWrite(u8TunerIndex,I2CAddr,0x09,0x1C, 0x00); /*if_fltr_cap_dctrl[5:0], FILTER_CAP_CTRL_L*/
    ret |= ATBM2040RegWrite(u8TunerIndex,I2CAddr,0x12,0x2D, 0x00); /*if_filtr_cal_clk_en*/

    return ret;
}

/************************************************************
*Description: Config FEF and LTE
*************************************************************/
ATBM2040_ERROR_e ATBM2040DrvFEFSourceAndLTESet(UINT8 u8TunerIndex, ATBM2040_t *pTuner)
{
    ATBM2040_ERROR_e ret = ATBM2040_NO_ERROR;
    UINT8 I2CAddr = pTuner->Config.I2CAddr;

    if (ATBM2040_SIGNAL_MODE_DVBT == pTuner->Config.Mode)
    {
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x19, 0xC2, 0x01); /*params_use_lte_operation  */
        switch (pTuner->Config.FEFMode)
        {
            case ATBM2040_FEF_INTERNAL:
                ATBM2040RegListWrite(u8TunerIndex, I2CAddr, FEFInternalReg, ATBM2040_FEF_INTERNAL_REG_CNT);
                break;
            case ATBM2040_FEF_EXTERNAL_GPO1:
                ret = ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x03, 0x03, 0x01); /*fef_sel: 0--no FEF input; 1--GPO1; 2--GPO2*/
                ret = ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x03, 0x00, 0x00); /*gpo1_oe*/
                ret = ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x16, 0x00, 0x01); /*params_add_fef_detect*/
                ret = ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x16, 0x19, 0x02); /*params_fef_use_mode: 0--internal; 1--internal and external; 2--external GPO*/
                break;
            case ATBM2040_FEF_EXTERNAL_GPO2:
                ret = ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x03, 0x03, 0x02); /*fef_sel: 0--no FEF input; 1--GPO1; 2--GPO2*/
                ret = ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x03, 0x01, 0x00); /*gpo2_oe*/
                ret = ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x16, 0x00, 0x01); /*params_add_fef_detect*/
                ret = ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x16, 0x19, 0x02); /*params_fef_use_mode: 0--internal; 1--internal and external; 2--external GPO*/
                break;
            case ATBM2040_FEF_IGNORE:
                ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x16, 0x00, 0x00); /*params_add_fef_detect */
                break;
            default:
                break;
        }
    }
    else
    {
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x19, 0xC2, 0x00); /*params_use_lte_operation  */
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x16, 0x00, 0x00); /*params_add_fef_detect */
    }
    return ret;
}


/************************************************************
*Description: Hardware init setting with configs
*************************************************************/
ATBM2040_ERROR_e ATBM2040DrvInit(UINT8 u8TunerIndex, ATBM2040_t *pTuner)
{
    ATBM2040_ERROR_e ret = ATBM2040_NO_ERROR;
    UINT8 tmpReg = 0x47;
    UINT32 SignalIFFreq = 0;
    UINT8 RXFltIQSwap = 0x00, ADCIQSwap = 0x00;
    ATBM2040RegGroup_t *pRegGroup = NULL;
    UINT8 I2CAddr = pTuner->Config.I2CAddr;

    /*Init internal debug parameters*/
    pTuner->MixerIF = 5000000;
    pTuner->HighMD = FALSE;
    pTuner->ADCIQSwap = TRUE;
    pTuner->PllDivMode = 0x01;/*pll_mod_mmd, 0:div/2; 1:div/1*/
    pTuner->DSPClkSel = 0;
    pTuner->BWKHzBak = 0;
    pTuner->MixerIFBak= 0;
    pTuner->Band = ATBM2040_RF_BAND_MAX;
    pTuner->BandBak = ATBM2040_RF_BAND_MAX;
    pTuner->SignalMode = pTuner->Config.Mode;    
    pTuner->SarAdcClk = ATBM2040_SAR_ADC_CLK_6MHZ;
    pTuner->LowCousumMd = 0;
#ifdef ATBM2040_CHIP_DEBUG_OPEN
    ret |= ATBM2040DrvChipDebugOption(u8TunerIndex, pTuner);
#endif

    /*Soft reset*/
    ret |= ATBM2040DrvSwReset(u8TunerIndex,pTuner);
#if ATBM2040_FIRMWARE_LOAD
    if(ATBM_LEOG_CHIP_ID == pTuner->ChipID)
    {
        ATBM2040Firmware_t *pFwm = NULL;
        pFwm = &ATBM2040GFirmware;
        ret |= ATBM2040DrvRegDataFlushWrite(u8TunerIndex,I2CAddr,pFwm->BaseReg,pFwm->OffReg,pFwm->Data,pFwm->Len);
    }
    else if(ATBM_LEOB_CHIP_ID == pTuner->ChipID)
    {
        ATBM2040Firmware_t *pFwm = NULL;
        pFwm = &ATBM2040Firmware;
        ret |= ATBM2040DrvRegDataFlushWrite(u8TunerIndex,I2CAddr,pFwm->BaseReg,pFwm->OffReg,pFwm->Data,pFwm->Len);
    }
#endif
        /* ret |= ATBM2040RegWrite(u8TunerIndex,I2CAddr,0x00,0x0E, 0x00);*/ /*self_rst_cfg[3:0]*/
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x00, 0x12, 0x00); /*params_long_afc_rst*/
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x00, 0x13, 0x08); /*enable pll wdog*/
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x00, 0x14, 0x1E); /*only reset dsp*/
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x00, 0x19, 0x00); /*close watch dog*/

        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x0E, 0x16, pTuner->DSPClkSel); /*osc_dsp_clk_sel*/


    if ((ATBM_LEOG_CHIP_ID == pTuner->ChipID) || (ATBM_LEOF_CHIP_ID == pTuner->ChipID)
            || (ATBM_LEOD_CHIP_ID == pTuner->ChipID) || (ATBM_LEOE_CHIP_ID == pTuner->ChipID))
    {
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x10, 0x14, 0x00); /*dac_sequence_sel*/
        if ((ATBM_LEOG_CHIP_ID == pTuner->ChipID)
                || (ATBM_LEOF_CHIP_ID == pTuner->ChipID) || (ATBM_LEOE_CHIP_ID == pTuner->ChipID))
        {
            ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x22, 0x00, 0x67); /*params_use_leod*/
            if (ATBM2040_SIGNAL_MODE_ATV == pTuner->Config.Mode)
            {
                ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x20, 0x63, 0x00); /*params_uhf_filter_on_nplusm*/
                ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x23, 0xBE, 0x05); /*params_use_leoe*/
            }
            else
            {
                ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x20, 0x63, 0x01); /*params_uhf_filter_on_nplusm*/
                ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x23, 0xBE, 0x05); /*params_use_leoe*/
            }
            ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x19, 0xDC, 0x00); /*params_add_n_plus_minus_m_db_offset*/
            ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x19, 0xDD, 0x00); /*params_n_plus_minus_m_db_offset[6:0]*/

        }
        else
        {
            ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x22, 0x00, 0x6F); /*params_use_leod*/
        }
    }

    /*Bias center setting*/
    ATBM2040RegListWrite(u8TunerIndex, I2CAddr, BGR, ATBM2040_BGR_CNT);
    if ((ATBM_LEOG_CHIP_ID == pTuner->ChipID) || (ATBM_LEOF_CHIP_ID == pTuner->ChipID))
    {
        ret |= ATBM2040RegRead(u8TunerIndex, I2CAddr, 0x12, 0x15, &tmpReg);
        tmpReg &= 0xEF;
        tmpReg |= 0x10;
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x12, 0x15, tmpReg); /* rsv1<4>: mixr_ldo_div_h_en*/
    }
    else
    {
        ret |= ATBM2040RegRead(u8TunerIndex, I2CAddr, 0x12, 0x15, &tmpReg);
        tmpReg &= 0xE0;
        tmpReg |= 0x11;
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x12, 0x15, tmpReg); /*Rsv1<4:0> = 5'b10001*/
    }
    /*RF_LDO power up*/
    if ((ATBM_LEOG_CHIP_ID == pTuner->ChipID)
            || (ATBM_LEOF_CHIP_ID == pTuner->ChipID) || (ATBM_LEOE_CHIP_ID == pTuner->ChipID))
    {
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x12, 0x48, 0x00); /*dvdd_ldo_24m_psrr_en*/
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x12, 0x49, 0x00); /*dvdd_ldo_24m_psrr_adj[1:0]*/
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x04, 0x06, 0x00); /*lna_ldo_psrr_en*/
    }
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x06, 0x03, 0x00); /*Vhf_fltr_ldo_psrr_en*/
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x12, 0x3E, 0x00); /*rf_ldo12_pd*/
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x0E, 0x0D, 0x01); /*osc_test_en*/
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x0E, 0x17, 0x00); /*osc_por_en, output disable*/

    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x0E, 0x11, pTuner->Config.OSCCap.CalibValue); /*osc_core_cap*/

    /*Mode selection*/
    if (pTuner->HighMD)
    {
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x16, 0x1A, 0x01);
    }
    else
    {
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x16, 0x1A, 0x00);
    }
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x14, 0x4F, 0x00);

    /*Lna Attenuator setting */
    if (0 == pTuner->LNAGainMD)
    {
        /* LNAGainMode B */
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x1A, 0x71, 0x7C); /*thr_lna_gain1  */
    }
    else
    {
        /* LNAGainMode A */
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x1A, 0x71, 0x00); /*thr_lna_gain1  */
    }
    ATBM2040RegListWrite(u8TunerIndex, I2CAddr, LnaAttenReg, ATBM2040_ATTEN_CNT);

    ret |= ATBM2040RegRead(u8TunerIndex,I2CAddr,0x12,0x16, &tmpReg);
    tmpReg |= 0x10; /*bit4: set 0*/
    tmpReg &= 0xBF; /*bit5: set 1*/
    ret |= ATBM2040RegWrite(u8TunerIndex,I2CAddr,0x12,0x16, tmpReg);  /*Rsv2<5:4> b'10*/
    
    ret |= ATBM2040RegWrite(u8TunerIndex,I2CAddr,0x12,0x1F, 0x00);  /* mixr_div_ldo_pd */
    ret |= ATBM2040RegWrite(u8TunerIndex,I2CAddr,0x12,0x0B, 0x00); /*mixr_ldo_pd*/

    /*Setting IF filter */
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x09, 0x00, 0x00); /*if_fltr_pd*/
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x09, 0x0D, 0x00); /*if_fltr_core_pd*/
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x09, 0x0C, 0x00); /*if_fltr_q_pd*/
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x09, 0x14, 0x47); /*if_filt_bw_ctrl[6:0]*/
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x09, 0x1A, 0x00); /*if_fltr_gain_ctrl*/
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x14, 0x2E, (UINT8) - 4); /*params_default_if_vga2_gain_step*/
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x09, 0x11, 0x00); /*if_filt_input_short*/
    if ((ATBM_LEOG_CHIP_ID == pTuner->ChipID) || (ATBM_LEOF_CHIP_ID == pTuner->ChipID)
            || (ATBM_LEOC_CHIP_ID == pTuner->ChipID) || (ATBM_LEOD_CHIP_ID == pTuner->ChipID)
            || (ATBM_LEOE_CHIP_ID == pTuner->ChipID))
    {
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x09, 0x02, 0x00); /*if_fltr_vcmh_adj*/
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x09, 0x03, 0x00); /*if_fltr_vldoh_adj*/
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x09, 0x06, 0x02); /*if_fltr_vcmo_sel[2:0], 0x01--0.7V;0x02--0.8V;0x04--0.9V;*/
    }


    /*IF filter DCOC calibration*/
    if ((ATBM_LEOF_CHIP_ID == pTuner->ChipID) || (ATBM_LEOE_CHIP_ID == pTuner->ChipID))
    {
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x09, 0x16, 0x02); /*if_fltr_dcoc_bw_adj*/
    }
    else
    {
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x09, 0x16, 0x04); /*if_fltr_dcoc_bw_adj*/
    }
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x09, 0x18, 0x00); /*if_fltr_dcoc_rc_cali_disable,power down DCOC calibration circuit*/
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x09, 0x15, 0x00); /*if_fltr_dcoc_pd*/
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x09, 0x1C, 0x15); /*if_fltr_cap_dctrl*/


    /*Setting DAC and Filter*/
    ATBM2040RegListWrite(u8TunerIndex, I2CAddr, DACSettingReg, ATBM2040_DAC_REG_CNT);

    /*Select IF path*/
    if (ATBM2040_SIGNAL_MODE_ATV != pTuner->Config.Mode)
    {
        /*DTV*/
        pTuner->IFLevelSpinner = pTuner->Config.DtvIFOut.LevelSpinner;
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x10, 0x03, ATBM2040DrvDACGainCode(u8TunerIndex, pTuner, pTuner->Config.DtvIFOut.IFOutLevel)); /*dac_fltr_gain_ctrl[2:0]*/
    }
    else
    {
        /*ATV*/
        pTuner->IFLevelSpinner = pTuner->Config.AtvIFOut.LevelSpinner;
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x10, 0x03, ATBM2040DrvDACGainCode(u8TunerIndex, pTuner, pTuner->Config.AtvIFOut.IFOutLevel)); /*dac_fltr_gain_ctrl[2:0]*/
    }

    ret |= ATBM2040DrvIFOutPath(u8TunerIndex, pTuner, ATBM2040_IF_PATH_1); /*DTV and ATV IF data share one path*/

    ATBM2040DrvDagcAmpRefSet(u8TunerIndex, pTuner); /*PAPR LIMIT , BB POWER, dagc_amp_ref*/
    ATBM2040DrvVGAGainThrSet(u8TunerIndex, pTuner);

    /*Setting IF frequency*/
    SignalIFFreq = pTuner->MixerIF >> 8; /* MixerIF/256 */
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x16, 0x0C, (0 - SignalIFFreq) & 0xFF); /*params_desired_signal_if_freq_h[7:0]*/
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x16, 0x0D, ((0 - SignalIFFreq) >> 8) & 0xFF); /*params_desired_signal_if_freq_h[15:8]*/
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x16, 0x0E, SignalIFFreq & 0xFF); /*params_desired_signal_if_freq_l[7:0]*/
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x16, 0x0F, (SignalIFFreq >> 8) & 0xFF); /*params_desired_signal_if_freq_l[15:8]*/

    /*PLL Registers Setting*/
    if ((ATBM_LEOG_CHIP_ID == pTuner->ChipID)
            || (ATBM_LEOF_CHIP_ID == pTuner->ChipID) || (ATBM_LEOE_CHIP_ID == pTuner->ChipID))
    {
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x0E, 0x15, 0x0D); /*osc_core_gm[3:0]*/
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x0E, 0x18, 0x03); /*osc_ldo_ps[1:0]*/
        /* ret |= ATBM2040RegWrite(u8TunerIndex,I2CAddr,0x0F,0x3B, 0x02);*/ /*pll_vref_ctrl[1:0]*/
        ret |= ATBM2040RegRead(u8TunerIndex, I2CAddr, 0x12, 0x14, &tmpReg);
        if (ATBM_LEOE_CHIP_ID == pTuner->ChipID)
        {
            tmpReg = ((tmpReg & 0xE0) | 0x06); /*rsv0[4:0]*/
        }
        else
        {
            tmpReg = ((tmpReg & 0xC0) | 0x26); /*Rsv0<5>: 1, adc_div_h_en; rsv0[4:0]:5'b00110 */
        }
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x12, 0x14, tmpReg);
    }
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x0F, 0x22, 0x7F); /*pll_n_count_ref_afc*/
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x0F, 0x23, 0x3F); /*pll_n_hold_afc*/
    if (ATBM2040_SIGNAL_MODE_ATV == pTuner->Config.Mode)
    {
        /*ATV, close temperature compensation*/
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x0F, 0x10, 0x00); /* pll_skvco */
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x0F, 0x08, 0x01); /* pll_sicp ,the cp current is provided by the ext*/
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x0F, 0x05, 0x05); /*pll_icp[7:0],  the extra current ctrl word 10#:16uA TYP:81#*/
    }
    else
    {
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x0F, 0x10, 0x01); /* pll_skvco */
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x0F, 0x08, 0x00); /* pll_sicp ,the cp current is provided by the ext*/
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x0F, 0x05, 0x80); /*pll_icp[7:0],  the extra current ctrl word 10#:16uA TYP:81#*/
    }

    ATBM2040RegListWrite(u8TunerIndex, I2CAddr, PLLSettingReg, ATBM2040_PLL_SETTING_REG_CNT);
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x0F, 0x29, (UINT8)pTuner->PllDivMode); /*pll_mod_mmd, 0:div/2; 1:div/1*/
    if ((ATBM_LEOG_CHIP_ID == pTuner->ChipID)
            || (ATBM_LEOF_CHIP_ID == pTuner->ChipID) || (ATBM_LEOE_CHIP_ID == pTuner->ChipID))
    {
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x12, 0x41, 0x01); /*mixr_vhf_pd */
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x12, 0x46, 0x01); /*mixr_uhf_pd */
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x12, 0x47, 0x01); /*mixr_vhf2_pd */
    }
    else
    {
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x12, 0x40, 0x01); /*mixr_div_mode_en*/
    }
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x16, 0x46, 0x01); /*params_mixr_pd_pll_mixr_div_l*/
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x16, 0x47, 0x01); /*params_mixr_pd_pll_mixr_div_h*/

    /*ADC Div setting*/
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x12, 0x22, 0x00); /* adc_div_ldo_pd */
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x12, 0x42, 0x01); /* adc_div_cal_en */
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x12, 0x30, 0x01); /* adc_div_pm */
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x12, 0x43, 0x01); /* adc_div_mode_en */
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x12, 0x20, 0x01); /* adc_div_core_pd */
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x12, 0x44, 0x01); /* adc_div_ldo_bias_en */
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x12, 0x45, 0x04); /* adc_div_ldo_s[4:0] */


    /*Setting ADC*/
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x0B, 0x1E, 0x00); /* adc_comp  ref current adjust*/
    ATBM2040RegListWrite(u8TunerIndex, I2CAddr, ADCSettingReg, ATBM2040_ADC_REG_CNT);
    if ((ATBM_LEOG_CHIP_ID == pTuner->ChipID)
            || (ATBM_LEOF_CHIP_ID == pTuner->ChipID) || (ATBM_LEOE_CHIP_ID == pTuner->ChipID))
    {
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x12, 0x5D, 0x01); /* adc_clk_sys_pd*/
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x12, 0x5E, 0x00); /* adc_clk_sys_mux*/
    }

    /*ADC RCTuning use manual mode*/
    ret |= ATBM2040RegRead(u8TunerIndex, I2CAddr, 0x0B, 0x09, &tmpReg); /*adc_rctuning_results*/
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x0B, 0x18, tmpReg); /* adc_rctuning_manual*/
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x0B, 0x17, 0x01); /* adc_rctuning_manual_en, use Manual RC tuning*/
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x0B, 0x06, 0x01); /* adc_rc_tuning_pd*/
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x12, 0x2A, 0x00); /* adc_ck_rc_tune_en*/

    /*RSSI setting*/
    if((ATBM_LEOB_CHIP_ID == pTuner->ChipID)||(ATBM_LEOC_CHIP_ID == pTuner->ChipID)
        ||(ATBM_LEOD_CHIP_ID == pTuner->ChipID))
    {
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x16, 0x21, 0x30); /*rssi intercept*/
    }
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x08, 0x00, 0x00); /*rssi_pd*/
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x16, 0x4B, 0x00); /*bypass RSSI calibration*/
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x0C, 0x22, 0x0A); /*bg_rssi_100u_con*/

    /*SAR_ADC setting*/
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x12, 0x2B, 0x01); /*sar_adc_ck_en*/ /*Turn on SAR_ADC clk*/
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x0D, 0x01, (ATBM2040_SAR_ADC_CLK_6MHZ == pTuner->SarAdcClk) ? (0x00) : (0x01)); /*sar_adc_clkin_sel, 0: 6M; 1: 1.5M*/
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x0D, 0x04, 0x01); /*sar_adc_sample_rssi_en [0]*/
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x0D, 0x05, 0x00); /*sar_adc_ref_range_sel*/ /*setting 1, the reference voltage (Vref) is 2.666V,setting 0 the Vref is 2V*/
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x0D, 0x06, 0x01); /*sar_adc_format_sel*/ /*setting 1,the output format is binary code*/
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x0D, 0x00, 0x00); /*sar_adc_pd*/ /*Power on SAR ADC*/

#if ATBM2040_GAIN_PHASE_COMPENSATION
    /*Phase and gain compensation config*/
    ret |= ATBM2040DrvPhaseAndGainConfig(pTuner);

    /*Gain offset compensation*/
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x16, 0x93, 0x01); /* params_digital_gain_offset_lna_debug */
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x16, 0x94, 0x01); /* params_digital_gain_offset_rf_vga1_debug */
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x16, 0x95, 0x01); /* params_digital_gain_offset_rf_vga2_debug */
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x16, 0x96, 0x01); /* params_digital_gain_offset_if_vga2_debug */
#else
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x15, 0x76, 0x00); /*params_phase_compensation*/
#endif

    /*iq_swap*/
    if (pTuner->ADCIQSwap)
    {
        RXFltIQSwap = (pTuner->InvertSpectrum) ? (0x01) : (0x00);
        ADCIQSwap = 0x01;
    }
    else
    {
        RXFltIQSwap = (pTuner->InvertSpectrum) ? (0x00) : (0x01);
        ADCIQSwap = 0x00;
    }
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x15, 0x3E, RXFltIQSwap); /*params_rxflt_iq_swap    [0] */
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x15, 0x48, ADCIQSwap); /*params_adc_iq_swap */

    /*About AGC*/
    ATBM2040RegListWrite(u8TunerIndex, I2CAddr, AGCReg, ATBM2040_AGC_REG_CNT);

    /*RSSI threshold*/
    if ((ATBM_LEOG_CHIP_ID == pTuner->ChipID) || (ATBM_LEOF_CHIP_ID == pTuner->ChipID))
    {
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x14, 0x64, 0x5A); /*params_rssi_thr1[7:0]*/
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x14, 0x65, 0x5C); /*params_rssi_thr2[7:0]*/
    }
    else
    {
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x14, 0x64, 0x4A); /*params_rssi_thr1[7:0]*/
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x14, 0x65, 0x4C); /*params_rssi_thr2[7:0]*/
    }

    ATBM2040RegListWrite(u8TunerIndex, I2CAddr, MisReg, ATBM2040_MIS_REG_CNT);
    if ((ATBM_LEOG_CHIP_ID == pTuner->ChipID) || (ATBM_LEOF_CHIP_ID == pTuner->ChipID)
            || (ATBM_LEOC_CHIP_ID == pTuner->ChipID) || (ATBM_LEOD_CHIP_ID == pTuner->ChipID)
            || (ATBM_LEOE_CHIP_ID == pTuner->ChipID))
    {
        ATBM2040RegListWrite(u8TunerIndex, I2CAddr, LeoCMisReg, ATBM2040_LEOC_MIS_REG_CNT);
        if (ATBM2040_SIGNAL_MODE_ATV == pTuner->Config.Mode)
        {
            ATBM2040RegListWrite(u8TunerIndex, I2CAddr, LeoCATVMisReg, ATBM2040_LEOC_ATV_MIS_REG_CNT);
        }
        else
        {
            ATBM2040RegListWrite(u8TunerIndex, I2CAddr, LeoCDTVMisReg, ATBM2040_LEOC_DTV_MIS_REG_CNT);
        }
    }
    else
    {
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x14, 0x14, 0x07); /*params_banalog_change_num_tracking */
    }

    /*FEF And LTE config*/
    ret |= ATBM2040DrvFEFSourceAndLTESet(u8TunerIndex, pTuner);

    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x16, 0x3B, 0xFF); /*params_waiting_samples_digital_gain_setting[7:0] */
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x16, 0x3C, 0x08); /*params_waiting_samples_digital_gain_setting[15:8]*/
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x16, 0x3D, 0x01); /*params_waiting_samples_digital_gain_setting[17:16]*/

    /*watch_dog*/
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x17, 0x00, 0x20); /*params_watch_dog_bypass_bbpow[7:0] */
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x17, 0x02, 0x20); /*params_watch_dog_bypass_rssi1[7:0]*/
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x17, 0x03, 0x20); /*params_watch_dog_bypass_rssi2[7:0]*/

    /*other DSP registers*/
    if (ATBM2040_SIGNAL_MODE_ATV != pTuner->Config.Mode)
    {
        /*DTV*/
        ATBM2040RegListWrite(u8TunerIndex, I2CAddr, DSPDTVReg, ATBM2040_DSP_DTV_REG_CNT);
    }
    else
    {
        /*ATV*/
        ATBM2040RegListWrite(u8TunerIndex, I2CAddr, DSPATVReg, ATBM2040_DSP_ATV_REG_CNT);
    }

    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x15, 0x8F, 0x01); /*params_phase_com_lna       */
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x15, 0x90, 0x01); /*params_phase_com_RF_VGA1 */
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x15, 0x91, 0x01); /*params_phase_com_RF_VGA2   */
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x15, 0x92, 0x01); /*params_phase_com_IF_VGA2   */

    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x16, 0x36, 0x1A); /*params_thr_analog_t_hlo  */

    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x16, 0x8F, 0x14); /*params_thr_vhf_filter_max_n_plus_minus_m_aci_l  */
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x16, 0x90, 0x0A); /*params_thr_vhf_filter_max_n_plus_minus_m_aci_s  */
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x16, 0x91, 0x00); /*params_vhf_filter_max_set_en  */

    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x17, 0x20, 0x00); /*params_bb_inband_detect_enable  */

    ATBM2040RegListWrite(u8TunerIndex, I2CAddr, DSPMiscReg, ATBM2040_DSP_MISC_REG_CNT);
    if (ATBM_LEOG_CHIP_ID == pTuner->ChipID)
    {
        ATBM2040RegGroup_t *pRegGrp = &AlterThrRssiLnaOutTracking;
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x14, 0x97, 0x15); /*params_mixer_if_filter_gain[5:0]   */
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x1A, 0x70, 0x00); /*bypass lna gain thr change */
        ret |= ATBM2040DrvRegDataFlushWrite(u8TunerIndex, I2CAddr, pRegGrp->BaseReg, pRegGrp->OffReg, pRegGrp->Data, pRegGrp->Len);
    }
    else
    {
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x14, 0x97, 0x18); /*params_mixer_if_filter_gain[5:0]   */
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x1A, 0x70, 0x01); /*bypass lna gain thr change */
    }
    
    /*short AGC bus*/
    /* ret |= ATBM2040RegWrite(u8TunerIndex,I2CAddr,0x00,0x34, 0x00); */
    /* ret |= ATBM2040RegWrite(u8TunerIndex,I2CAddr,0x02,0x1B, 0x03);*/ /* intf_sample_n */
    /* ret |= ATBM2040RegWrite(u8TunerIndex,I2CAddr,0x02,0x1C, 0x01);*/ /* intf_sample_m */
    /* ret |= ATBM2040RegWrite(u8TunerIndex,I2CAddr,0x02,0x21, 0x01);*/ /* intf_sample_m_start */

    /*long AGC bus */
    ret |= ATBM2040RegWrite(u8TunerIndex,I2CAddr,0x00,0x34, 0x01);
    ret |= ATBM2040RegWrite(u8TunerIndex,I2CAddr,0x00,0x35, 0x1F); /* lo cycle */
    ret |= ATBM2040RegWrite(u8TunerIndex,I2CAddr,0x00,0x36, 0x04); /* other cycle */
    ret |= ATBM2040RegWrite(u8TunerIndex,I2CAddr,0x02,0x1B, 0x40); /* intf_sample_n */
    ret |= ATBM2040RegWrite(u8TunerIndex,I2CAddr,0x02,0x1C, 0x1D); /* intf_sample_m */
    ret |= ATBM2040RegWrite(u8TunerIndex,I2CAddr,0x02,0x21, 0x02); /* intf_sample_m_start */

    ret |= ATBM2040RegWrite(u8TunerIndex,I2CAddr,0x18,0x40, 0x05); /* lo set zero and juse pll lock flag */
    if((ATBM_LEOG_CHIP_ID == pTuner->ChipID)||(ATBM_LEOB_CHIP_ID == pTuner->ChipID))
    {
        ret |= ATBM2040RegWrite(u8TunerIndex,I2CAddr,0x00,0x24, 0xFF); /* maximum time for judge lock */
    }
    ret |= ATBM2040RegWrite(u8TunerIndex,I2CAddr,0x00,0x26, 0x0F); /* long to judge pll lock */
    ret |= ATBM2040RegWrite(u8TunerIndex,I2CAddr,0x00,0x03, 0x00); /*  directly use pll lock from pll, and delay 128cycle , and combine with time */
    ret |= ATBM2040RegWrite(u8TunerIndex,I2CAddr,0x02,0x1F, 0x1F); /* pll_200us_cfg[15:8] */
    ret |= ATBM2040RegWrite(u8TunerIndex,I2CAddr,0x00,0x2A, 0x01); /*  use fifo */
    ret |= ATBM2040RegWrite(u8TunerIndex,I2CAddr,0x00,0x2B, 0x00); /*  params_agc_keep */


    ret |= ATBM2040RegWrite(u8TunerIndex,I2CAddr,0x17,0x55, 0xD3); /* params_used_old_dagc_ref */
    ret |= ATBM2040RegWrite(u8TunerIndex,I2CAddr,0x1A,0x2D, 0x01); /* params_analog_cfo_bypass */
    /* ret |= ATBM2040RegWrite(u8TunerIndex,I2CAddr,0x1A,0x41, 0x01);*/ /* params_sfi_detect_notch_bypass_1 */
    ret |= ATBM2040RegWrite(u8TunerIndex,I2CAddr,0x1A,0x49, 0x01); /* params_sfi_detect_notch_bypass_2 */
    ret |= ATBM2040RegWrite(u8TunerIndex,I2CAddr,0x1A,0x16, 0x00); /* params_aci_in_band_analysis_bypass */
    ret |= ATBM2040RegWrite(u8TunerIndex,I2CAddr,0x1A,0x6C, 0x01); /* params_notch_iir_a_aci */


    if((ATBM_LEOF_CHIP_ID == pTuner->ChipID)||(ATBM_LEOE_CHIP_ID == pTuner->ChipID))
    {
        ret |= ATBM2040RegWrite(u8TunerIndex,I2CAddr,0x17,0x93, 0x19); /*params_thr_aci_status_n_m_large_h[6:0]*/
        ret |= ATBM2040RegWrite(u8TunerIndex,I2CAddr,0x17,0x94, 0x0F); /*params_thr_aci_status_n_m_large_l[6:0]*/
    }
    else
    {
        ret |= ATBM2040RegWrite(u8TunerIndex,I2CAddr,0x17,0x93, 0x14); /*params_thr_aci_status_n_m_large_h[6:0]*/
        ret |= ATBM2040RegWrite(u8TunerIndex,I2CAddr,0x17,0x94, 0x0F); /*params_thr_aci_status_n_m_large_l[6:0]*/
    }

    if(ATBM_LEOG_CHIP_ID == pTuner->ChipID)
    {
        UINT8 RegData[6] = {0x00};
        if(ATBM2040_SIGNAL_MODE_DVBT == pTuner->Config.Mode)
        {
            if(2 == pTuner->LowCousumMd)
            {
                ret |= ATBM2040RegWrite(u8TunerIndex,I2CAddr,0x22,0x00, 0x67); /*params_use_leod*/
            }
            else
            {
                ret |= ATBM2040RegWrite(u8TunerIndex,I2CAddr,0x22,0x00, 0x7F); /*params_use_leod*/
            }
            ret |= ATBM2040RegWrite(u8TunerIndex,I2CAddr,0x23,0xCB, 0x01); /*params_lo_sign_map_mode[2:0]*/
            pRegGroup = &DVBTAciN234Judge;
        }
        else
        {
            ret |= ATBM2040DrvRegDataBurstWrite(u8TunerIndex,I2CAddr,AciN234CalResult,ATBM2040_ACI234_CAL_RESULT_SIZE);
            ret |= ATBM2040RegWrite(u8TunerIndex,I2CAddr,0x22,0x00, 0x67); /*params_use_leod*/
            ret |= ATBM2040RegWrite(u8TunerIndex,I2CAddr,0x23,0xCB, 0x00); /*params_lo_sign_map_mode[2:0]*/
            pRegGroup = &OtherAciN234Judge;
        }
        ret |= ATBM2040DrvRegDataFlushWrite(u8TunerIndex,I2CAddr,pRegGroup->BaseReg,pRegGroup->OffReg,pRegGroup->Data,pRegGroup->Len);

        
        switch(pTuner->LowCousumMd)
        {
            case 0:
                RegData[0] = 0x00;
                RegData[1] = 0x00;
                RegData[2] = 0x00;
                RegData[3] = 0x02;
                RegData[4] = 0x0F;
                RegData[5] = 0x0A;
                break;
            case 1:
                RegData[0] = 0x01;
                RegData[1] = 0x01;
                RegData[2] = 0x01;
                RegData[3] = 0x02;
                RegData[4] = 0x0F;
                RegData[5] = 0x08;
                break;
            case 2:
                RegData[0] = 0x01;
                RegData[1] = 0x01;
                RegData[2] = 0x01;
                RegData[3] = 0x00;
                RegData[4] = 0x09;
                RegData[5] = 0x08;
                break;
            default:
                break;
        }
        ret |= ATBM2040RegWrite(u8TunerIndex,I2CAddr,0x10,0x10, RegData[0]); 
        ret |= ATBM2040RegWrite(u8TunerIndex,I2CAddr,0x0B,0x0E, RegData[1]); 
        ret |= ATBM2040RegWrite(u8TunerIndex,I2CAddr,0x0B,0x1F, RegData[2]); 
        
        ret |= ATBM2040RegWrite(u8TunerIndex,I2CAddr,0x09,0x01, RegData[3]); 
        ret |= ATBM2040RegWrite(u8TunerIndex,I2CAddr,0x0C,0x2E, RegData[4]); 

        ret |= ATBM2040RegWrite(u8TunerIndex,I2CAddr,0x0C,0x0E, RegData[5]); 
    }


    /*write SDK version into register*/
    ret |= ATBM2040RegWrite(u8TunerIndex,I2CAddr,0x12,0x1B, ATBM2040Ver); /*SDK version*/

    if(ATBM2040_SIGNAL_MODE_ATV == pTuner->Config.Mode)
    {/*ATV*/
        ATBM2040RegListWrite(u8TunerIndex,I2CAddr,ATVRSSIReg,ATBM2040_ATV_RSSI_REG_CNT);
    }
    else
    {/*DTV*/
        ATBM2040RegListWrite(u8TunerIndex,I2CAddr,DTVRSSIReg,ATBM2040_DTV_RSSI_REG_CNT);
    }

    /*Clock out control*/
    ret |= ATBM2040DrvClkOutControl(u8TunerIndex,pTuner);

    /*GPO state set*/
    ret |= ATBM2040DrvGPOStateSet(u8TunerIndex,pTuner,pTuner->Config.PinsCfg,ATBM2040_PIN_MAX);

    /*LTA*/
    ret |= ATBM2040RegWrite(u8TunerIndex,I2CAddr,0x04,0x02, (ATBM2040_RF_LTA_OUT_ENABLE==pTuner->Config.LTAOut)?(0x00):(0x01)); /*lta_pd*/
    
    ret |= ATBM2040RegRead(u8TunerIndex,I2CAddr,0x12,0x15, &tmpReg);
    tmpReg &= 0xFE;
    tmpReg |= ((ATBM2040_RF_LTA_OUT_ENABLE==pTuner->Config.LTAOut)?(0x00):(0x01));
    ret |= ATBM2040RegWrite(u8TunerIndex,I2CAddr,0x12,0x15, tmpReg); /*rsv1[0], LTA_LDO_PD*/

    pTuner->StandbyState = FALSE;

    return ret;
}

/************************************************************
*Description: frequency tune
*************************************************************/
ATBM2040_ERROR_e ATBM2040DrvFreqTune(UINT8 u8TunerIndex, ATBM2040_t *pTuner)
{
    ATBM2040_ERROR_e ret = ATBM2040_NO_ERROR;
    UINT32 HMDAdcClk = 0, LMDAdcClk = 0;
    UINT8 tmpReg = 0;
    UINT8 RXFltIQSwap = 0x00, ADCIQSwap = 0x00;
    UINT8 I2CAddr = pTuner->Config.I2CAddr;

    /*SDK version check */
    ret |= ATBM2040RegRead(u8TunerIndex, I2CAddr, 0x12, 0x1B, &tmpReg);
    if (tmpReg != ATBM2040Ver)
    {
        ret |= ATBM2040DrvInit(u8TunerIndex, pTuner);
    }

    ATBM2040DrvNLOMixer(u8TunerIndex, pTuner); /*Mixer Divider select*/
    ATBM2040DrvADCClkCal(u8TunerIndex, pTuner, &HMDAdcClk, &LMDAdcClk); /*ADC CLK calculate*/

    if (pTuner->StandbyState)
    {
        /*Wakeup it first if in standby state*/
        ret |= ATBM2040DrvWakeup(u8TunerIndex, pTuner);
    }

#ifdef ATBM2040_CHIP_DEBUG_OPEN
    ret |= ATBM2040DrvFirmwareDisable(u8TunerIndex, pTuner);
#else
#if ATBM2040_FIRMWARE_LOAD
    if ((ATBM_LEOG_CHIP_ID == pTuner->ChipID) || (ATBM_LEOB_CHIP_ID == pTuner->ChipID))
    {
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x20, 0x1F, 0x00);
    }
#endif
#endif
    /*Stop DSP*/
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x00, 0x0D, 0x00); /*sw_cfg_done*/
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x00, 0x04, 0x02); /*[0] I2c_clk_sel    [1] i2c_sys_clk_lck_enb*/
    ret |= ATBM2040RegWrite(u8TunerIndex,I2CAddr,0x00,0x07, 0x01); /*soft_rst_dsp*/
    ret |= ATBM2040RegWrite(u8TunerIndex,I2CAddr,0x00,0x0B, 0x01); /*soft_rst_lo*/

    /*Signal mode check*/
    if (pTuner->Config.Mode != pTuner->SignalMode)
    {
        /*new mode is different from old mode*/
        pTuner->Config.Mode = pTuner->SignalMode;
        ret |= ATBM2040DrvInit(u8TunerIndex, pTuner);

        pTuner->Config.Mode = pTuner->SignalMode;
        /*FEF And LTE config*/
        ret |= ATBM2040DrvFEFSourceAndLTESet(u8TunerIndex, pTuner);
        ATBM2040DrvDagcAmpRefSet(u8TunerIndex, pTuner); /*PAPR LIMIT  , BB POWER, dagc_amp_ref*/
        ATBM2040DrvVGAGainThrSet(u8TunerIndex, pTuner);
    }
    /*adc_dwa*/
    if ((ATBM_LEOE_CHIP_ID == pTuner->ChipID) || (ATBM_LEOF_CHIP_ID == pTuner->ChipID))
    {
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x0B, 0x0A, 0x01); /*adc_dwa_disable*/
    }

    /*LNA Gain setting*/
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x04, 0x0B, 0x03); /*lna_mode*/
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x04, 0x07, 0x05); /* [2:0]     101 att_gain_fine[2:0] */
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x04, 0x08, 0x02); /* [1:0]     10  att_gain_coarse[1:0] */
    if ((ATBM_LEOG_CHIP_ID == pTuner->ChipID) || (ATBM_LEOF_CHIP_ID == pTuner->ChipID)
            || (ATBM_LEOC_CHIP_ID == pTuner->ChipID) || (ATBM_LEOD_CHIP_ID == pTuner->ChipID)
            || (ATBM_LEOE_CHIP_ID == pTuner->ChipID))
    {
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x04, 0x03, 0x01); /* lna_gm_ctrl<1:0> */
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x04, 0x04, 0x06); /* lna_rl_ctrl<4:0> */
    }
    else
    {
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x04, 0x05, 0x0B); /* [1:0] 10  lna_gain_fine[1:0] */
    }
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x14, 0x2B, (UINT8) - 36); /* [6:0]   -36 params_default_atten_gain[6:0] */
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x14, 0x2F, 0x00); /* [6:0]     0   params_default_lna_gain[6:0] */


    /*IF Filter Gain setting*/
    /* ret |= ATBM2040RegWrite(u8TunerIndex,I2CAddr,0x0A,0x00, 0x00); */ /* [2:0]   0   if_vga_s1_gain[2:0]                     */
    /* ret |= ATBM2040RegWrite(u8TunerIndex,I2CAddr,0x0A,0x01, 0x04); */ /* [3:0]   11  if_vga_s2_gain[2:0]                     */
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x09, 0x1A, 0x00); /*if_fltr_gain_ctrl*/
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x14, 0x2E, (UINT8) - 4); /* [6:0]    -4 params_default_if_vga2_gain_step[6:0] */

    /*ADC clock setting*/
    ret |= ATBM2040DrvADCCLKSet(u8TunerIndex, pTuner);

    /*VHF/UHF filter setting*/
    if (ATBM2040_VHF_CHECK(pTuner->FreqKHz))
    {
        if ((ATBM_LEOG_CHIP_ID == pTuner->ChipID)
                || (ATBM_LEOF_CHIP_ID == pTuner->ChipID) || (ATBM_LEOE_CHIP_ID == pTuner->ChipID))
        {
            if (pTuner->FreqKHz < ATBM2040_VHF2_LOW_KHZ)
            {
                /*VHF1*/
                ret |= ATBM2040DrvVHF1Setting(u8TunerIndex, pTuner);
                if(ATBM_LEOG_CHIP_ID == pTuner->ChipID)
                {                
                    ret |= ATBM2040RegWrite(u8TunerIndex,I2CAddr,0x12,0x1A, 0x00);
                }
            }
            else
            {/*VHF2*/
                ret |= ATBM2040DrvVHF2Setting(u8TunerIndex,pTuner);
                if(ATBM_LEOG_CHIP_ID == pTuner->ChipID)
                {
                    if(ATBM2040_SIGNAL_MODE_DVBT == pTuner->SignalMode)
                    {
                        tmpReg = 0x40;
                    }
                    else
                    {
                        tmpReg = 0x00;
                    }
                    ret |= ATBM2040RegWrite(u8TunerIndex,I2CAddr,0x12,0x1A, tmpReg);
                }
            }
        }
        else
        {
            ret |= ATBM2040DrvVHFSetting(u8TunerIndex,pTuner);
        }
    }
    else
    {
        ret |= ATBM2040DrvUHFSetting(u8TunerIndex,pTuner);
    }

    /*PLL and Mixer divider setting*/
    ret |= ATBM2040DrvPLLMixDivSet(u8TunerIndex, pTuner);
    ret |= ATBM2040DrvMixerReset(u8TunerIndex, pTuner);

    /*Setting the IF frequency calibration registers*/
    ret |= ATBM2040DrvIFFilterSet(u8TunerIndex, pTuner);

    /*Init ADC*/
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x0B, 0x25, 0x00);
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x0B, 0x25, 0x01);


    /*DSP register config*/
    ATBM2040DrvDSPRegConfig(u8TunerIndex, pTuner, HMDAdcClk, LMDAdcClk);
#if ATBM2040_GAIN_PHASE_COMPENSATION
    /*Phase and gain compensation config*/
    ret |= ATBM2040DrvPhaseAndGainConfig(u8TunerIndex, pTuner);

    /*Gain offset compensation*/
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x16, 0x93, 0x01); /* params_digital_gain_offset_lna_debug */
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x16, 0x94, 0x01); /* params_digital_gain_offset_rf_vga1_debug */
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x16, 0x95, 0x01); /* params_digital_gain_offset_rf_vga2_debug */
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x16, 0x96, 0x01); /* params_digital_gain_offset_if_vga2_debug */
#endif

    /*iq_swap*/
    if ((ATBM_LEOG_CHIP_ID == pTuner->ChipID) || (ATBM_LEOF_CHIP_ID == pTuner->ChipID)
            || (ATBM_LEOC_CHIP_ID == pTuner->ChipID) || (ATBM_LEOD_CHIP_ID == pTuner->ChipID)
            || (ATBM_LEOE_CHIP_ID == pTuner->ChipID))
    {
        if (ATBM2040_VHF_CHECK(pTuner->FreqKHz))
        {
            pTuner->ADCIQSwap = TRUE;
            ADCIQSwap = 0x01;
            RXFltIQSwap = (pTuner->InvertSpectrum) ? (0x01) : (0x00);
        }
        else
        {
            pTuner->ADCIQSwap = FALSE;
            ADCIQSwap = 0x00;
            RXFltIQSwap = (pTuner->InvertSpectrum) ? (0x01) : (0x00);
        }
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x15, 0x48, ADCIQSwap); /*params_adc_iq_swap */
    }
    else
    {
        if (pTuner->ADCIQSwap)
        {
            RXFltIQSwap = (pTuner->InvertSpectrum) ? (0x01) : (0x00);
        }
        else
        {
            RXFltIQSwap = (pTuner->InvertSpectrum) ? (0x00) : (0x01);
        }
    }
    ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x15, 0x3E, RXFltIQSwap); /*params_rxflt_iq_swap [0] */

    /*BandWidth*/
    ret |= ATBM2040DrvBWSet(u8TunerIndex, pTuner);

    /*RSSI Intercept*/
    if ((ATBM_LEOG_CHIP_ID == pTuner->ChipID)
            || (ATBM_LEOF_CHIP_ID == pTuner->ChipID) || (ATBM_LEOE_CHIP_ID == pTuner->ChipID))
    {
        UINT16 Intercept = 0;
        Intercept = ATBM2040DrvRSSIInterceptGet(u8TunerIndex, pTuner);
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x16, 0x21, (UINT8)(Intercept & 0xFF)); /*rssi intercept*/
        /* ret |= ATBM2040RegWrite(u8TunerIndex,I2CAddr,0x16,0x22, (UINT8)((Intercept>>8)&0xFF)); */
    }

    /*RXFLT rc filter bandwidth and spectrum parse setting*/
    ret |= ATBM2040DrvRxFltAndSpectrumParse(u8TunerIndex, pTuner);

    /*BB power target setting*/
    ret |= ATBM2040DrvAgcPowerTargetSet(u8TunerIndex, pTuner);

    ret |= ATBM2040DrvIIRCoefJudge(u8TunerIndex, pTuner);

    /*Switch I2C clock to system clock*/
    /* ret |= ATBM2040RegWrite(u8TunerIndex,I2CAddr,0x00,0x04, 0x03); */    /*[0] I2c_clk_sel    [1] i2c_sys_clk_lck_enb*/
#ifdef ATBM2040_CHIP_DEBUG_OPEN
    ret |= ATBM2040DrvChipDebugSwConfigDone(u8TunerIndex, pTuner);
#else
#if ATBM2040_FIRMWARE_LOAD
    if ((ATBM_LEOG_CHIP_ID == pTuner->ChipID) || (ATBM_LEOB_CHIP_ID == pTuner->ChipID))
    {
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x20, 0x1F, 0x01);
    }
#endif
    ret |= ATBM2040RegWrite(u8TunerIndex,I2CAddr,0x00,0x07, 0x01); /*soft_rst_dsp*/
    ret |= ATBM2040RegWrite(u8TunerIndex,I2CAddr,0x00,0x0B, 0x01); /*soft_rst_lo*/
    ret |= ATBM2040RegWrite(u8TunerIndex,I2CAddr,0x00,0x0D, 0x07); /*sw_cfg_done*/
    ret |= ATBM2040RegWrite(u8TunerIndex,I2CAddr,0x00,0x04, 0x03); /*[0] I2c_clk_sel     [1] i2c_sys_clk_lck_enb*/
    ret |= ATBM2040RegWrite(u8TunerIndex,I2CAddr,0x00,0x07, 0x00); /*soft_rst_dsp*/
    ret |= ATBM2040RegWrite(u8TunerIndex,I2CAddr,0x00,0x0B, 0x00); /*soft_rst_lo*/
#endif

    if ((ATBM_LEOE_CHIP_ID == pTuner->ChipID) || (ATBM_LEOF_CHIP_ID == pTuner->ChipID))
    {
        ret |= ATBM2040DrvADCDwaSet(u8TunerIndex, pTuner);
    }
    /*ret |= ATBM2040DrvRSSICalibration(pTuner);*/

    ATBM2040Delay(50000);
    return ret;
}

/************************************************************
*Description: RxFlt and Spectrum setting
*************************************************************/
ATBM2040_ERROR_e ATBM2040DrvRxFltAndSpectrumParse(UINT8 u8TunerIndex, ATBM2040_t *pTuner)
{
    ATBM2040_ERROR_e ret = ATBM2040_NO_ERROR;
    ATBM2040RegGroup_t *pRegGrp = NULL;
    UINT8 I2CAddr = pTuner->Config.I2CAddr;

    if(pTuner->BWKHzBak == pTuner->BWKHz)
    {
        return ret;
    }

    if(pTuner->BWKHz <= 6000)
    {
        pRegGrp = &RxFltBw6MRCReg1;
        ret |= ATBM2040DrvRegDataFlushWrite(u8TunerIndex,I2CAddr,pRegGrp->BaseReg,pRegGrp->OffReg,pRegGrp->Data,pRegGrp->Len);
        pRegGrp = &RxFltBw6MRCReg2;
        ret |= ATBM2040DrvRegDataFlushWrite(u8TunerIndex,I2CAddr,pRegGrp->BaseReg,pRegGrp->OffReg,pRegGrp->Data,pRegGrp->Len);
    }
    else if(pTuner->BWKHz <= 7000)
    {
        pRegGrp = &RxFltBw7MRCReg1;
        ret |= ATBM2040DrvRegDataFlushWrite(u8TunerIndex,I2CAddr,pRegGrp->BaseReg,pRegGrp->OffReg,pRegGrp->Data,pRegGrp->Len);
        pRegGrp = &RxFltBw7MRCReg2;
        ret |= ATBM2040DrvRegDataFlushWrite(u8TunerIndex,I2CAddr,pRegGrp->BaseReg,pRegGrp->OffReg,pRegGrp->Data,pRegGrp->Len);
    }
    else /* if(pTuner->BWKHz <= 8000)*/
    {
        pRegGrp = &RxFltBw8MRCReg1;
        ret |= ATBM2040DrvRegDataFlushWrite(u8TunerIndex,I2CAddr,pRegGrp->BaseReg,pRegGrp->OffReg,pRegGrp->Data,pRegGrp->Len);
        pRegGrp = &RxFltBw8MRCReg2;
        ret |= ATBM2040DrvRegDataFlushWrite(u8TunerIndex,I2CAddr,pRegGrp->BaseReg,pRegGrp->OffReg,pRegGrp->Data,pRegGrp->Len);
    }
    if((ATBM_LEOG_CHIP_ID == pTuner->ChipID)
        ||(ATBM_LEOF_CHIP_ID == pTuner->ChipID)||(ATBM_LEOE_CHIP_ID == pTuner->ChipID))
    {
        if(ATBM2040_SIGNAL_MODE_ATV == pTuner->SignalMode)
        {
            UINT8 Data[4] = {0x00};
            UINT32 BandWidth = (pTuner->BWKHz+500)/1000;
            Data[0] = (UINT8)(BandWidth-6);
            Data[1] = (UINT8)(BandWidth-6);
            Data[2] = (UINT8)(BandWidth-6);
            Data[3] = (UINT8)(BandWidth-6);
            ret |= ATBM2040DrvRegDataFlushWrite(u8TunerIndex,I2CAddr,0x19,0xAC,Data,sizeof(Data));
        }
    }
    return ret;
}

/************************************************************
*Description: Change some config
*************************************************************/
ATBM2040_ERROR_e ATBM2040DrvCfgSet(UINT8 u8TunerIndex, ATBM2040_t *pTuner, ATBM2040CfgCMD_t *pCfg)
{
    ATBM2040_ERROR_e ret = ATBM2040_NO_ERROR;
    UINT8 tmpReg = 0x00;
    UINT8 I2CAddr = pTuner->Config.I2CAddr;
    switch (pCfg->CfgCmd)
    {
        case ATBM2040_CFG_CMD_DTV_IF_OUT_SETTING:
            if (pCfg->Cfg.IFOut.IFOutFreqHz != pTuner->Config.DtvIFOut.IFOutFreqHz)
            {
                pTuner->Config.DtvIFOut.IFOutFreqHz = pCfg->Cfg.IFOut.IFOutFreqHz;
                ret = ATBM2040DrvFreqShiftFromBBtoIF(u8TunerIndex, pTuner);
            }
            if ((pCfg->Cfg.IFOut.IFOutLevel != pTuner->Config.DtvIFOut.IFOutLevel) || (pCfg->Cfg.IFOut.LevelSpinner != pTuner->Config.DtvIFOut.LevelSpinner))
            {
                pTuner->Config.DtvIFOut.IFOutLevel = pCfg->Cfg.IFOut.IFOutLevel;
                pTuner->IFLevelSpinner = pTuner->Config.DtvIFOut.LevelSpinner;
                ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x10, 0x03, ATBM2040DrvDACGainCode(u8TunerIndex, pTuner, pCfg->Cfg.IFOut.IFOutLevel)); /*dac_fltr_gain_ctrl[2:0]*/
            }
            break;
        case ATBM2040_CFG_CMD_ATV_IF_OUT_SETTING:
            if (pCfg->Cfg.IFOut.IFOutFreqHz != pTuner->Config.AtvIFOut.IFOutFreqHz)
            {
                pTuner->Config.AtvIFOut.IFOutFreqHz = pCfg->Cfg.IFOut.IFOutFreqHz;
                ret = ATBM2040DrvFreqShiftFromBBtoIF(u8TunerIndex, pTuner);
            }
            if ((pCfg->Cfg.IFOut.IFOutLevel != pTuner->Config.AtvIFOut.IFOutLevel) || (pCfg->Cfg.IFOut.LevelSpinner != pTuner->Config.AtvIFOut.LevelSpinner))
            {
                pTuner->Config.AtvIFOut.IFOutLevel = pCfg->Cfg.IFOut.IFOutLevel;
                pTuner->IFLevelSpinner = pTuner->Config.AtvIFOut.LevelSpinner;
                ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x10, 0x03, ATBM2040DrvDACGainCode(u8TunerIndex, pTuner, pCfg->Cfg.IFOut.IFOutLevel)); /*dac_fltr_gain_ctrl[2:0]*/
            }
            break;
        case ATBM2040_CFG_CMD_CLK_OUT:
            pTuner->Config.ClkOut = pCfg->Cfg.ClkOut;
            ret = ATBM2040DrvClkOutControl(u8TunerIndex, pTuner);
            break;
        case ATBM2040_CFG_CMD_PIN_SETTING:
            ret = ATBM2040DrvGPOStateSet(u8TunerIndex, pTuner, &pCfg->Cfg.PinCfg, 1);
            break;
        case ATBM2040_CFG_CMD_OSC_CAP_SET:
            pTuner->Config.OSCCap = pCfg->Cfg.OSCCap;
            ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x0E, 0x11, pTuner->Config.OSCCap.CalibValue); /*osc_core_cap*/
            break;
        case ATBM2040_CFG_CMD_RF_LTA:
            pTuner->Config.LTAOut = pCfg->Cfg.LTAOut;
            ret |= ATBM2040RegWrite(u8TunerIndex,I2CAddr,0x04,0x02, (ATBM2040_RF_LTA_OUT_ENABLE==pCfg->Cfg.LTAOut)?(0x00):(0x01)); /*lta_pd*/
            
            ret |= ATBM2040RegRead(u8TunerIndex,I2CAddr,0x12,0x15, &tmpReg);
            tmpReg &= 0xFE;
            tmpReg |= ((ATBM2040_RF_LTA_OUT_ENABLE==pTuner->Config.LTAOut)?(0x00):(0x01));
            ret |= ATBM2040RegWrite(u8TunerIndex,I2CAddr,0x12,0x15, tmpReg); /*rsv1[0], LTA_LDO_PD*/
            break;
        default:
            break;
    }
    return ret;
}

/************************************************************
*Description: Get current RSSI value
*************************************************************/
ATBM2040_ERROR_e ATBM2040DrvGetRSSI(UINT8 u8TunerIndex, UINT8 I2CAddr, SINT16 *pRSSIx16)
{
    ATBM2040_ERROR_e ret = ATBM2040_NO_ERROR;
    UINT8 Val = 0;
    SINT16 SignalPow = 0;

    ret |= ATBM2040DrvRegLatch(u8TunerIndex, I2CAddr, TRUE);
    ret |= ATBM2040RegRead(u8TunerIndex, I2CAddr, 0x14, 0xD0, &Val);
    SignalPow = Val;
    ret |= ATBM2040RegRead(u8TunerIndex, I2CAddr, 0x14, 0xD1, &Val);
    ret |= ATBM2040DrvRegLatch(u8TunerIndex, I2CAddr, FALSE);
    SignalPow |= ((Val & 0x0F) << 8);
    SignalPow <<= 4;
    SignalPow >>= 4;
    *pRSSIx16 = SignalPow;

    return ret;
}

/************************************************************
*Description: Get ATV CFO(KHz)
*************************************************************/
ATBM2040_ERROR_e ATBM2040DrvGetATVCfo(UINT8 u8TunerIndex, UINT8 I2CAddr, SINT32 *pCfoKHz)
{
    ATBM2040_ERROR_e ret = ATBM2040_NO_ERROR;
    UINT8 Val = 0;
    SINT16 Cfo = 0;

    ret |= ATBM2040DrvRegLatch(u8TunerIndex, I2CAddr, TRUE);
    ret |= ATBM2040RegRead(u8TunerIndex, I2CAddr, 0x1A, 0xDB, &Val);
    if (0x01 != Val)
    {
        *pCfoKHz = 0;
        ret |= ATBM2040DrvRegLatch(u8TunerIndex, I2CAddr, FALSE);
        ret |= ATBM2040_ERROR_CFO_NOT_READY;
        return ret;
    }
    ret |= ATBM2040RegRead(u8TunerIndex, I2CAddr, 0x1A, 0xD9, &Val);
    Cfo = Val;
    ret |= ATBM2040RegRead(u8TunerIndex, I2CAddr, 0x1A, 0xDA, &Val);
    ret |= ATBM2040DrvRegLatch(u8TunerIndex, I2CAddr, FALSE);
    Cfo |= ((Val & 0x7F) << 8);
    Cfo <<= 1;
    Cfo >>= 1;
    *pCfoKHz = (((SINT32)Cfo) * 256 + 500) / 1000;
    /* *pCfoKHz = ((*pCfoKHz+50)/100)*100; */
    return ret;
}

/************************************************************
*Description: Enter standby state
*************************************************************/
ATBM2040_ERROR_e ATBM2040DrvStandby(UINT8 u8TunerIndex, ATBM2040_t *pTuner)
{
    ATBM2040_ERROR_e ret = ATBM2040_NO_ERROR;
    UINT8 I2CAddr = pTuner->Config.I2CAddr;
    if (!pTuner->StandbyState)
    {
#ifdef ATBM2040_CHIP_DEBUG_OPEN
        ret |= ATBM2040DrvFirmwareDisable(u8TunerIndex, pTuner);
#else
#if ATBM2040_FIRMWARE_LOAD
        if ((ATBM_LEOG_CHIP_ID == pTuner->ChipID) || (ATBM_LEOB_CHIP_ID == pTuner->ChipID))
        {
            ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x20, 0x1F, 0x00);
        }
#endif
#endif
        ATBM2040RegListWrite(u8TunerIndex, I2CAddr, StandbyReg, ATBM2040_STANDBY_REG_CNT);
        if (ATBM2040_VHF_CHECK(pTuner->FreqKHz))
        {
            /* ret |= ATBM2040RegWrite(u8TunerIndex,I2CAddr,0x06,0x0C, 0x01); */ /*vhf_fltr_ldo_pd*/
        }
        else
        {
            ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x05, 0x00, 0x01); /*uhf_fltr_pwdn_ldo*/
        }
        pTuner->StandbyState = TRUE;
    }
    return ret;
}

/************************************************************
*Description: Wakeup from standby state
*************************************************************/
ATBM2040_ERROR_e ATBM2040DrvWakeup(UINT8 u8TunerIndex, ATBM2040_t *pTuner)
{
    ATBM2040_ERROR_e ret = ATBM2040_NO_ERROR;
    UINT8 I2CAddr = pTuner->Config.I2CAddr;
    if (pTuner->StandbyState)
    {
        if (ATBM2040_VHF_CHECK(pTuner->FreqKHz))
        {
            /* ret |= ATBM2040RegWrite(u8TunerIndex,I2CAddr,0x06,0x0C, 0x00); */ /*vhf_fltr_ldo_pd*/
        }
        else
        {
            ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x05, 0x00, 0x00); /*uhf_fltr_pwdn_ldo*/
        }
        ATBM2040RegListWrite(u8TunerIndex, I2CAddr, WakeupReg, ATBM2040_WAKEUP_REG_CNT);
#ifdef ATBM2040_CHIP_DEBUG_OPEN
        ret |= ATBM2040DrvFirmwareEnable(u8TunerIndex, pTuner);
#else
#if ATBM2040_FIRMWARE_LOAD
        if ((ATBM_LEOG_CHIP_ID == pTuner->ChipID) || (ATBM_LEOB_CHIP_ID == pTuner->ChipID))
        {
            ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x20, 0x1F, 0x01);
        }
#endif
#endif
        pTuner->StandbyState = FALSE;
    }
    return ret;
}

/************************************************************
*Description: Get SDK version
*************************************************************/
ATBM2040_ERROR_e ATBM2040DrvGetSDKVer(UINT8 u8TunerIndex, ATBM2040_t *pTuner, UINT32 *pVer)
{
    (void)pTuner;
    *pVer = (UINT32)ATBM2040Ver;
    return ATBM2040_NO_ERROR;
}


static BOOL IIRCoefJudgeOpen = FALSE;
void ATBM2040DrvIIRCoefJudgeStateSet(UINT8 u8TunerIndex, BOOL Open)
{
    IIRCoefJudgeOpen = Open;
}

static BOOL ATBM2040DrvIIRCoefJudgeStateGet(UINT8 u8TunerIndex)
{
    return IIRCoefJudgeOpen;
}

static UINT8 IIRCodeJudgeHMD[3][64] =
{
    /*(Params.fs_H/4 <= 47.5e6)*/
    {
        0xB0, 0x04, 0x3F, 0x00, 0xEA, 0x08, 0x34, 0x03, 0xA6, 0x0D, 0xA9, 0x08, 0xB7, 0x03, 0x84, 0x09,
        0x63, 0x05, 0x41, 0x00, 0x20, 0x09, 0x0C, 0x03, 0x38, 0x0F, 0xE6, 0x08, 0xA8, 0x03, 0x2E, 0x0A,
        0x37, 0x06, 0x43, 0x00, 0x57, 0x09, 0xE5, 0x02, 0x97, 0x00, 0x2A, 0x09, 0x9A, 0x03, 0xE9, 0x0A,
        0xFF, 0x07, 0x48, 0x00, 0xC6, 0x09, 0x9D, 0x02, 0xBA, 0x02, 0xC7, 0x09, 0x80, 0x03, 0x7D, 0x0C
    },

    /*(Params.fs_H/4 <= 52.5e6)*/
    {
        0x6C, 0x04, 0x3F, 0x00, 0xD2, 0x08, 0x46, 0x03, 0xE5, 0x0C, 0x90, 0x08, 0xBE, 0x03, 0x3F, 0x09,
        0xFC, 0x04, 0x40, 0x00, 0x02, 0x09, 0x21, 0x03, 0x61, 0x0E, 0xC3, 0x08, 0xB0, 0x03, 0xCE, 0x09,
        0xA9, 0x05, 0x42, 0x00, 0x33, 0x09, 0xFE, 0x02, 0xB9, 0x0F, 0xFD, 0x08, 0xA3, 0x03, 0x6E, 0x0A,
        0x56, 0x07, 0x46, 0x00, 0x96, 0x09, 0xBB, 0x02, 0xEA, 0x01, 0x81, 0x09, 0x8B, 0x03, 0xCF, 0x0B
    },

    /*else*/
    {
        0x39, 0x04, 0x3E, 0x00, 0xBF, 0x08, 0x56, 0x03, 0x44, 0x0C, 0x7D, 0x08, 0xC4, 0x03, 0x0B, 0x09,
        0xB0, 0x04, 0x3F, 0x00, 0xEA, 0x08, 0x34, 0x03, 0xA6, 0x0D, 0xA9, 0x08, 0xB7, 0x03, 0x84, 0x09,
        0x40, 0x05, 0x41, 0x00, 0x16, 0x09, 0x13, 0x03, 0xF2, 0x0E, 0xDA, 0x08, 0xAB, 0x03, 0x0D, 0x0A,
        0xA3, 0x06, 0x44, 0x00, 0x70, 0x09, 0xD4, 0x02, 0x25, 0x01, 0x4B, 0x09, 0x94, 0x03, 0x42, 0x0B
    },

};

static UINT8 IIRCodeJudgeLMD[3][64] =
{
    /*(Params.fs_L/4 <= 47.5e6)*/
    {
        0xB0, 0x04, 0x3F, 0x00, 0xEA, 0x08, 0x34, 0x03, 0xA6, 0x0D, 0xA9, 0x08, 0xB7, 0x03, 0x84, 0x09,
        0x63, 0x05, 0x41, 0x00, 0x20, 0x09, 0x0C, 0x03, 0x38, 0x0F, 0xE6, 0x08, 0xA8, 0x03, 0x2E, 0x0A,
        0x37, 0x06, 0x43, 0x00, 0x57, 0x09, 0xE5, 0x02, 0x97, 0x00, 0x2A, 0x09, 0x9A, 0x03, 0xE9, 0x0A,
        0xFF, 0x07, 0x48, 0x00, 0xC6, 0x09, 0x9D, 0x02, 0xBA, 0x02, 0xC7, 0x09, 0x80, 0x03, 0x7D, 0x0C
    },

    /*(Params.fs_L/4 <= 52.5e6)*/
    {
        0x6C, 0x04, 0x3F, 0x00, 0xD2, 0x08, 0x46, 0x03, 0xE5, 0x0C, 0x90, 0x08, 0xBE, 0x03, 0x3F, 0x09,
        0xFC, 0x04, 0x40, 0x00, 0x02, 0x09, 0x21, 0x03, 0x61, 0x0E, 0xC3, 0x08, 0xB0, 0x03, 0xCE, 0x09,
        0xA9, 0x05, 0x42, 0x00, 0x33, 0x09, 0xFE, 0x02, 0xB9, 0x0F, 0xFD, 0x08, 0xA3, 0x03, 0x6E, 0x0A,
        0x56, 0x07, 0x46, 0x00, 0x96, 0x09, 0xBB, 0x02, 0xEA, 0x01, 0x81, 0x09, 0x8B, 0x03, 0xCF, 0x0B
    },

    /*else*/
    {
        0x39, 0x04, 0x3E, 0x00, 0xBF, 0x08, 0x56, 0x03, 0x44, 0x0C, 0x7D, 0x08, 0xC4, 0x03, 0x0B, 0x09,
        0xB0, 0x04, 0x3F, 0x00, 0xEA, 0x08, 0x34, 0x03, 0xA6, 0x0D, 0xA9, 0x08, 0xB7, 0x03, 0x84, 0x09,
        0x40, 0x05, 0x41, 0x00, 0x16, 0x09, 0x13, 0x03, 0xF2, 0x0E, 0xDA, 0x08, 0xAB, 0x03, 0x0D, 0x0A,
        0xA3, 0x06, 0x44, 0x00, 0x70, 0x09, 0xD4, 0x02, 0x25, 0x01, 0x4B, 0x09, 0x94, 0x03, 0x42, 0x0B
    },


};

ATBM2040_ERROR_e ATBM2040DrvRegDataFlushWrite(UINT8 u8TunerIndex, UINT8 SlvAddr, UINT8 BaseAddr, UINT8 OffAddr, UINT8 *pData, UINT32 Size)
{
    ATBM2040_ERROR_e ret = ATBM2040_NO_ERROR;
    UINT32 i = 0, SizeBak = Size;
    for (i = 0; i < SizeBak; i += 32)
    {
        ret |= ATBM2040I2CWrite(u8TunerIndex, SlvAddr, BaseAddr, (UINT8)(OffAddr + i), pData + i, (Size < 32) ? (Size) : (32));
        if (Size > 32)
        {
            Size -= 32;
        }
        else
        {
            Size = 0;
        }
    }
    if (Size > 0)
    {
        ret |= ATBM2040I2CWrite(u8TunerIndex, SlvAddr, BaseAddr, (UINT8)(OffAddr + i), pData + i, Size);
    }
    return ret;
}

static ATBM2040_ERROR_e ATBM2040DrvBurstWrite(UINT8 u8TunerIndex, UINT8 SlvAddr, UINT8 *pData, UINT32 Length)
{
    /*Length max is 45*/
    ATBM2040_ERROR_e ret = ATBM2040_NO_ERROR;
    UINT8 *pBuff = NULL;
    UINT32 i = 0;

    if (Length > ATBM2040_BURST_BUFF_MAX)
    {
        Length = ATBM2040_BURST_BUFF_MAX;
    }
    pBuff = ATBM2040BurstBuff;
    pBuff[0] = (UINT8)(Length / 3) | (0x02 << 5); /*full addr mode*/
    pBuff[Length + 1] = 0x01;

    pBuff++;
    for (i = 0; i < Length; i++)
    {
        pBuff[i] = pData[i];
    }

    pBuff--;
    ret |= ATBM2040DrvRegDataFlushWrite(u8TunerIndex, SlvAddr, 0x18, 0x80, pBuff, Length + 2);
    return ret;
}

ATBM2040_ERROR_e ATBM2040DrvRegDataBurstWrite(UINT8 u8TunerIndex, UINT8 SlvAddr, UINT8 *pData, UINT32 Length)
{
    ATBM2040_ERROR_e ret = ATBM2040_NO_ERROR;
    UINT32 i = 0, LengthBak = Length;
    for (i = 0; i < LengthBak; i += ATBM2040_BURST_BUFF_MAX)
    {
        ret |= ATBM2040DrvBurstWrite(u8TunerIndex, SlvAddr, pData + i, (Length < ATBM2040_BURST_BUFF_MAX) ? (Length) : (ATBM2040_BURST_BUFF_MAX));
        if (Length > ATBM2040_BURST_BUFF_MAX)
        {
            Length -= ATBM2040_BURST_BUFF_MAX;
        }
        else
        {
            Length = 0;
        }
    }
    if (Length > 0)
    {
        ret |= ATBM2040DrvBurstWrite(u8TunerIndex, SlvAddr, pData + i, Length);
    }

    return ret;
}

ATBM2040_ERROR_e ATBM2040DrvIIRCoefJudge(UINT8 u8TunerIndex, ATBM2040_t *pTuner)
{
    ATBM2040_ERROR_e ret = ATBM2040_NO_ERROR;
    UINT8 I2CAddr = pTuner->Config.I2CAddr;
    UINT8 *pData = NULL;

#ifdef ATBM2040_CHIP_DEBUG_OPEN
    if (0 == pTuner->LNAGainMD)
    {
        /* LNAGainMode B */
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x1A, 0x71, 0x7C); /*thr_lna_gain1  */
    }
    else
    {
        /* LNAGainMode A */
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x1A, 0x71, 0x00); /*thr_lna_gain1  */
    }
#endif
    if (ATBM_LEOG_CHIP_ID == pTuner->ChipID)
    {
        if ((ATBM2040_SIGNAL_MODE_DVBT == pTuner->SignalMode) && (pTuner->FreqKHz >= 650000) && (pTuner->FreqKHz <= 690000))
        {
            ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x16, 0x38, 0x10); /*params_thr_comp_lo_sel[7:0]  */
        }
        else
        {
            ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x16, 0x38, 0x00); /*params_thr_comp_lo_sel[7:0]  */
        }
    }

    if (ATBM2040_SIGNAL_MODE_ATV == pTuner->SignalMode)
    {
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x19, 0xBD, 0x62); /*params_rssi_lna_out_aci_status_4_lte_nplusm  */
    }
    else if ((ATBM2040_SIGNAL_MODE_DVBT == pTuner->SignalMode)
             && ((pTuner->FreqKHz >= 680000) && (pTuner->FreqKHz <= 700000)))
    {
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x19, 0xBD, 0x6A); /*params_rssi_lna_out_aci_status_4_lte_nplusm  */
    }
    else
    {
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x19, 0xBD, 0x70); /*params_rssi_lna_out_aci_status_4_lte_nplusm  */
    }

    if ((ATBM_LEOC_CHIP_ID == pTuner->ChipID) || (ATBM_LEOD_CHIP_ID == pTuner->ChipID))
    {
        if (pTuner->FreqKHz < 700000)
        {
            ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x00, 0x24, 0xFF); /* maximum time for judge lock */
        }
        else
        {
            ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x00, 0x24, 0x0D); /* maximum time for judge lock */
        }
    }
    else if ((ATBM_LEOE_CHIP_ID == pTuner->ChipID) || (ATBM_LEOF_CHIP_ID == pTuner->ChipID)
             /*||(ATBM_LEOG_CHIP_ID == pTuner->ChipID)*/)
    {
        if (pTuner->FreqKHz < 760000)
        {
            ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x00, 0x24, 0xFF); /* maximum time for judge lock */
        }
        else
        {
            ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x00, 0x24, 0x0D); /* maximum time for judge lock */
        }
    }

    if (ATBM_LEOF_CHIP_ID == pTuner->ChipID)
    {
        if ((ATBM2040_SIGNAL_MODE_ATV == pTuner->SignalMode) && (pTuner->FreqKHz == 768000))
        {
            ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x0E, 0x16, 0x01);
        }
        else
        {
            ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x0E, 0x16, pTuner->DSPClkSel);
        }
    }


    if (!ATBM2040DrvIIRCoefJudgeStateGet(u8TunerIndex))
    {
        return ret;
    }
    if (pTuner->HMDAdcClkHz <= 47500000 * 8)
    {
        pData = IIRCodeJudgeHMD[0];
    }
    else if (pTuner->HMDAdcClkHz <= 52500000 * 8)
    {
        pData = IIRCodeJudgeHMD[1];
    }
    else
    {
        pData = IIRCodeJudgeHMD[2];
    }
    ret |= ATBM2040DrvRegDataFlushWrite(u8TunerIndex, I2CAddr, 0x19, 0x02, pData, 64);

    if (pTuner->LMDAdcClkHz <= 47500000 * 8)
    {
        pData = IIRCodeJudgeLMD[0];
    }
    else if (pTuner->LMDAdcClkHz <= 52500000 * 8)
    {
        pData = IIRCodeJudgeLMD[1];
    }
    else
    {
        pData = IIRCodeJudgeLMD[2];
    }
    ret |= ATBM2040DrvRegDataFlushWrite(u8TunerIndex, I2CAddr, 0x19, 0x42, pData, 48); /*0x71-0x42+1*/
    ret |= ATBM2040DrvRegDataFlushWrite(u8TunerIndex, I2CAddr, 0x19, 0x82, pData + 48, 16); /*0x91-0x82+1*/

    return ret;
}

ATBM2040_ERROR_e ATBM2040DrvRSSICalibration(UINT8 u8TunerIndex, ATBM2040_t *pTuner)
{
    UINT8 I2CAddr = 0xC0;
    UINT8 Val = 0, Val0022 = 0, CfgDone = 0;
    ATBM2040_ERROR_e ret = ATBM2040_NO_ERROR;
    UINT32 n = 0;
    SINT8 IFVGAGain = 0;
    SINT16 RFPower = 0;
    SINT16 RFPowerTmp[3] = {0};

    I2CAddr = pTuner->Config.I2CAddr;
    ret |= ATBM2040RegRead(u8TunerIndex, I2CAddr, 0x00, 0x0D, &CfgDone); /*sw_cfg_done*/

    if ((ATBM_LEOB_CHIP_ID != pTuner->ChipID) || (0x07 != CfgDone))
    {
        return ATBM2040_NO_ERROR;
    }

    ATBM2040Delay(60000);
    ret |= ATBM2040RegRead(u8TunerIndex, I2CAddr, 0x14, 0xC2, &Val); /*S(7,0)*/
    IFVGAGain = Val & 0x7F;
    IFVGAGain <<= 1;
    IFVGAGain >>= 1;
    if (IFVGAGain < 32)
    {
        ret |= ATBM2040DrvRegLatch(u8TunerIndex, I2CAddr, TRUE);
        ret |= ATBM2040RegRead(u8TunerIndex, I2CAddr, 0x14, 0xD0, &Val);
        RFPower = Val;
        ret |= ATBM2040RegRead(u8TunerIndex, I2CAddr, 0x14, 0xD1, &Val);
        ret |= ATBM2040DrvRegLatch(u8TunerIndex, I2CAddr, FALSE);
        RFPower |= ((Val & 0x0F) << 8);
        RFPower <<= 4;
        RFPower >>= 8;
        pTuner->RFPower = RFPower;
        return ret;
    }
    ret |= ATBM2040RegRead(u8TunerIndex, I2CAddr, 0x00, 0x22, &Val0022);

    for (n = 0; n < 3; n++)
    {
        Val0022 ^= 0x01;
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x00, 0x22, Val0022);
        Val0022 ^= 0x01;
        ret |= ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x00, 0x22, Val0022);
        ATBM2040Delay(60000);

        ret |= ATBM2040DrvRegLatch(u8TunerIndex, I2CAddr, TRUE);
        ret |= ATBM2040RegRead(u8TunerIndex, I2CAddr, 0x14, 0xD0, &Val);
        RFPowerTmp[n] = Val;
        ret |= ATBM2040RegRead(u8TunerIndex, I2CAddr, 0x14, 0xD1, &Val);
        ret |= ATBM2040DrvRegLatch(u8TunerIndex, I2CAddr, FALSE);
        RFPowerTmp[n] |= ((Val & 0x0F) << 8);
        RFPowerTmp[n] <<= 4;
        RFPowerTmp[n] >>= 8;

        ret |= ATBM2040RegRead(u8TunerIndex, I2CAddr, 0x14, 0xC2, &Val); /*S(7,0)*/
        IFVGAGain = Val & 0x7F;
        IFVGAGain <<= 1;
        IFVGAGain >>= 1;
        if (IFVGAGain < 32)
        {
            pTuner->RFPower = RFPowerTmp[n];
            return ret;
        }
    }

    pTuner->RFPower = RFPowerTmp[0];
    for (n = 0; n < 3; n++)
    {
        if (RFPowerTmp[n] > pTuner->RFPower)
        {
            pTuner->RFPower = RFPowerTmp[n];
        }
    }

    return ret;
}

ATBM2040_ERROR_e ATBM2040DrvProcess(UINT8 u8TunerIndex, ATBM2040_t *pTuner)
{
    ATBM2040_ERROR_e ret = ATBM2040_NO_ERROR;
    UINT8 I2CAddr = pTuner->Config.I2CAddr;

    if ((ATBM_LEOE_CHIP_ID == pTuner->ChipID) || (ATBM_LEOE_CHIP_ID == pTuner->ChipID))
    {
        UINT8 selection_finish = 0, selection = 0;
        UINT8 curAdcDiv = 0, HMDAdcDiv = 0, LMDAdcDiv = 0;

        ATBM2040RegRead(u8TunerIndex, I2CAddr, 0x14, 0xAC, &selection_finish);
        if (0x01 == selection_finish)
        {
            ATBM2040RegRead(u8TunerIndex, I2CAddr, 0x14, 0xAD, &selection);
            ATBM2040RegRead(u8TunerIndex, I2CAddr, 0x12, 0x05, &curAdcDiv);

            ATBM2040RegRead(u8TunerIndex, I2CAddr, 0x16, 0x42, &LMDAdcDiv);
            ATBM2040RegRead(u8TunerIndex, I2CAddr, 0x16, 0x43, &HMDAdcDiv);
            if ((0x00 == selection) && (curAdcDiv != HMDAdcDiv))
            {
                ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x12, 0x05, HMDAdcDiv);
            }
            else if ((0x01 == selection) && (curAdcDiv != LMDAdcDiv))
            {
                ATBM2040RegWrite(u8TunerIndex, I2CAddr, 0x12, 0x05, LMDAdcDiv);
            }

        }
    }
    return ret;
}


#ifdef ATBM2040_CHIP_DEBUG_OPEN
UINT8 ATBM2040DrvSoftwareVerGet(UINT8 u8TunerIndex, void)
{
    return ATBM2040Ver;
}
void ATBM2040DrvMixDivGet(UINT8 u8TunerIndex, ATBM2040_t *pTuner, UINT32 *pNMix, UINT32 *pNLO)
{
    ATBM2040DrvNLOMixer(u8TunerIndex, pTuner);
    if ((ATBM_LEOG_CHIP_ID == pTuner->ChipID)
            || (ATBM_LEOF_CHIP_ID == pTuner->ChipID) || (ATBM_LEOE_CHIP_ID == pTuner->ChipID))
    {
        if (pTuner->HighMD)
        {
            *pNMix = pTuner->pNLOMixerHMD->NMixer;
            *pNLO = pTuner->pNLOMixerHMD->NLO;
        }
        else
        {
            *pNMix = pTuner->pNLOMixerLMD->NMixer;
            *pNLO = pTuner->pNLOMixerLMD->NLO;
        }
    }
    else
    {
        *pNMix = pTuner->pNLOMixer->NMixer;
        *pNLO = pTuner->pNLOMixer->NLO;
    }

}


void ATBM2040DrvPLLFreqGet(UINT8 u8TunerIndex, UINT32 FreqKHz, UINT32 MixIFKHz, double *pHLOFreq, double *pLLOFreq)
{
    ATBM2040NLOMixer_t *pNLOMixer = NULL;
    UINT32 IFKHz = MixIFKHz;
    UINT32 CenterFreqKHz = FreqKHz; /* Frequency calculation should be less KHz */
    UINT32 i = 0;
    UINT32 MixDivCnt = 0;

#define MIXER_DIVIDER_SEARCH_()     for(i=0;i<MixDivCnt;i++)\
    {\
        if((pNLOMixer[i].FreqStart <= FreqKHz)&&(FreqKHz <= pNLOMixer[i].FreqEnd))\
        {\
            break;\
        }\
    }\
    if(i >= MixDivCnt)\
    {\
        i = MixDivCnt-1;\
    }

    if ((ATBM_LEOG_CHIP_ID == ATBM2040ChipID)
            || (ATBM_LEOF_CHIP_ID == ATBM2040ChipID) || (ATBM_LEOE_CHIP_ID == ATBM2040ChipID))
    {
        pNLOMixer = NLOMixerLeoEHMD;
        MixDivCnt = ATBM2040_NLO_MIXER_LEOE_HMD_CNT;
        MIXER_DIVIDER_SEARCH_();
        pNLOMixer = (pNLOMixer + i);
        *pHLOFreq = (CenterFreqKHz + IFKHz) * pNLOMixer->NLO * pNLOMixer->NMixer / 1000.0; /*  NLO *  NMixer < 256 */

        pNLOMixer = NLOMixerLeoELMD;
        MixDivCnt = ATBM2040_NLO_MIXER_LEOE_LMD_CNT;
        MIXER_DIVIDER_SEARCH_();
        pNLOMixer = (pNLOMixer + i);
        *pLLOFreq = (CenterFreqKHz - IFKHz) * pNLOMixer->NLO * pNLOMixer->NMixer / 1000.0; /*  NLO *  NMixer < 256 */
    }
    else
    {
        pNLOMixer = NLOMixer;
        for (i = 0; i < ATBM2040_NLO_MIXER_CNT; i++)
        {
            if ((pNLOMixer->FreqStart <= FreqKHz) && (FreqKHz <= pNLOMixer->FreqEnd))
            {
                break;
            }
            pNLOMixer++;
        }
        if (i >= ATBM2040_NLO_MIXER_CNT)
        {
            pNLOMixer = &NLOMixer[ATBM2040_NLO_MIXER_CNT - 1];
        }
        *pHLOFreq = (CenterFreqKHz + IFKHz) * pNLOMixer->NLO * pNLOMixer->NMixer / 1000.0; /*  NLO *  NMixer < 256 */
        *pLLOFreq = (CenterFreqKHz - IFKHz) * pNLOMixer->NLO * pNLOMixer->NMixer / 1000.0; /*  NLO *  NMixer < 256 */
    }
}

ATBM2040AgcPowerTarget_t* ATBM2040DrvPowerTargetListGet(UINT8 u8TunerIndex,ATBM2040_t *pTuner)
{
    switch(pTuner->Config.Mode)
    {
        case ATBM2040_SIGNAL_MODE_DTMB:
            if((ATBM_LEOG_CHIP_ID == pTuner->ChipID)
                ||(ATBM_LEOF_CHIP_ID == pTuner->ChipID)||(ATBM_LEOE_CHIP_ID == pTuner->ChipID))
            {
                return LeoEAgcPowerTargetDTMB;
            }
            else if((ATBM_LEOC_CHIP_ID == pTuner->ChipID)||(ATBM_LEOD_CHIP_ID == pTuner->ChipID))
            {
                return LeoCAgcPowerTargetDTMB;
            }
            else
            {
                return AgcPowerTargetDTMB;
            }
            break;
        case ATBM2040_SIGNAL_MODE_DVBT:
            if(ATBM_LEOG_CHIP_ID == pTuner->ChipID)
            {
                return LeoGAgcPowerTargetDVBT;
            }
            else if((ATBM_LEOF_CHIP_ID == pTuner->ChipID)||(ATBM_LEOE_CHIP_ID == pTuner->ChipID))
            {
                return LeoEAgcPowerTargetDVBT;
            }
            else if((ATBM_LEOC_CHIP_ID == pTuner->ChipID)||(ATBM_LEOD_CHIP_ID == pTuner->ChipID))
            {
                return LeoCAgcPowerTargetDVBT;
            }
            else
            {
                return AgcPowerTargetDVBT;
            }
            break;
        case ATBM2040_SIGNAL_MODE_DVBC:
            if((ATBM_LEOG_CHIP_ID == pTuner->ChipID)
                ||(ATBM_LEOF_CHIP_ID == pTuner->ChipID)||(ATBM_LEOE_CHIP_ID == pTuner->ChipID))
            {
                return LeoEAgcPowerTargetDVBC;
            }
            else
            {
                return AgcPowerTargetDVBC;
            }
            break;
        case ATBM2040_SIGNAL_MODE_ATV:
            if(ATBM_LEOG_CHIP_ID == pTuner->ChipID)
            {
                return LeoGAgcPowerTargetATV;
            }
            else if((ATBM_LEOF_CHIP_ID == pTuner->ChipID)||(ATBM_LEOE_CHIP_ID == pTuner->ChipID))
            {
                return LeoEAgcPowerTargetATV;
            }
            else if((ATBM_LEOC_CHIP_ID == pTuner->ChipID)||(ATBM_LEOD_CHIP_ID == pTuner->ChipID))
            {
                return LeoCAgcPowerTargetATV;
            }
            else
            {
                return AgcPowerTargetATV;
            }
            break;
        default:
            break;
    }
    return NULL;
}

#endif


/************************************************************
*Description: Print configs of ATBM2040 SDK
*************************************************************/
void ATBM2040CfgPrint(UINT8 u8TunerIndex, ATBM2040_t *pTuner)
{
#if ATBM2040_DEBUG_PRINT
    ATBM2040_ERROR_e ret = ATBM2040_NO_ERROR;
    UINT8 Val = 0;
    UINT8 I2CAddr = 0;
    SINT8 *SignalMode[ATBM2040_SIGNAL_MODE_MAX] = {(SINT8 *)"DTMB", (SINT8 *)"DVBT", (SINT8 *)"DVBC", (SINT8 *)"ANALOG"};

    I2CAddr = pTuner->Config.I2CAddr;
    ATBM2040Print(("------------------ATBM2040 configurations:------------------\n"));

    ret = ATBM2040RegRead(u8TunerIndex, I2CAddr, 0x00, 0x00, &Val);
    if (ATBM2040_NO_ERROR != ret)
    {
        ATBM2040Print(("[ATBM2040] ChipID read fail!\n"));
    }
    ATBM2040Print(("[ATBM2040] ChipID:0x%02X,SDK Version:%d\n", Val, ATBM2040Ver));

    ATBM2040Print(("[ATBM2040] I2CAddr: 0x%X\n", pTuner->Config.I2CAddr));
    ATBM2040Print(("[ATBM2040] SignalMode: %s\n",
                   (pTuner->Config.Mode < ATBM2040_SIGNAL_MODE_MAX) ? (SignalMode[pTuner->Config.Mode]) : ((SINT8 *)"Invalid")));
    ATBM2040Print(("[ATBM2040] IFOutFreqHz: %d Hz\n", pTuner->Config.DtvIFOut.IFOutFreqHz));

    ret = ATBM2040I2CRead(u8TunerIndex, I2CAddr, 0x10, 0x03, &Val, 1);
    if (ATBM2040_NO_ERROR != ret)
    {
        ATBM2040Print(("[ATBM2040] DAC gain read fail!\n"));
    }
    ATBM2040Print(("[ATBM2040] IFOutLevel: 0x%02X\n", Val));

    ATBM2040Print(("[ATBM2040] ClkOut: %s\n", (pTuner->Config.ClkOut.ClkOutEnable == ATBM2040_CLK_OUT_ENABLE) ? ("Enable") : ("Disable")));

    ATBM2040Print(("==============================================================\n"));
#else
    (void)pTuner;
#endif
}

#endif

