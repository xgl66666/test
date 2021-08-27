/*
Copyright (C) 2010 NXP B.V., All Rights Reserved.
This source code and any compilation or derivative thereof is the proprietary
information of NXP B.V. and is confidential in nature. Under no circumstances
is this software to be  exposed to or placed under an Open Source License of
any type without the expressed written permission of NXP B.V.
*
* \file          tmbslMS222_Config.h
*
* \date          %modify_time%
*
* \author        Alexandre TANT
*
* \brief         Describe briefly the purpose of this file.
*
* REFERENCE DOCUMENTS :
*                MS222_Driver_User_Guide.pdf
*
* \section info Change Information
*
*/

#ifndef _MS222_CONFIG_COMMON_H
#define _MS222_CONFIG_COMMON_H


#ifdef __cplusplus
extern "C"
{
#endif

    /*============================================================================*/
    /* Types and defines:                                                         */
    /*============================================================================*/

    /* Driver settings version definition */
#define MS222_SETTINGS_CUSTOMER_NUM      (0)                     /* SW Settings Customer Number */
#define MS222_SETTINGS_PROJECT_NUM       (0)                     /* SW Settings Project Number  */
#define MS222_SETTINGS_MAJOR_VER         (1)                     /* SW Settings Major Version   */
#define MS222_SETTINGS_MINOR_VER         (0)                     /* SW Settings Minor Version   */

#define MS222_INSTANCE_CUSTOM_COMMON_LVL_REF_GAIN_MAX 19
#define MS222_INSTANCE_CUSTOM_COMMON_LVL_AGC1_MAX 12
#define MS222_INSTANCE_CUSTOM_COMMON_LVL_AGC2_MAX 14
#define MS222_INSTANCE_CUSTOM_COMMON_LVL_AGC3_MAX 8

#define MS222_RF_MAX         1200000000

#define MS222_BP_FILTER_1      39424000
#define MS222_BP_FILTER_2      61952000
#define MS222_BP_FILTER_3      86528000
#define MS222_BP_FILTER_4     123392000
#define MS222_BP_FILTER_5     172544000
#define MS222_BP_FILTER_6     244222000
#define MS222_BP_FILTER_7     320000000

/* {    X     ,    X    ,   X  ,           R           },         */
/* {    0     ,    1    ,   2  ,           F           },         */
/* {          ,         ,      ,                       },         */
/* {    x     ,    x    ,   x  ,                       },         */
/* {    1     ,    1    ,   1  ,           M           },         */
/* {    0     ,    0    ,   0  ,           H           },         */
/* {    0     ,    0    ,   0  ,           z           },         */
/* {    0     ,    0    ,   0  ,                       },         */
/* {    0     ,    0    ,   0  ,                       },         */
/* {    0     ,    0    ,   0  ,                       },         */
/* {    0     ,    0    ,   0  ,                       },         */
/* {          ,         ,   0  ,                       },         */
/* {          ,         ,      ,                       },         */
/* {          ,         ,      ,                       },         */
#define MS222_INSTANCE_CUSTOM_COMMON_LVL_REF_GAIN_MAX_NB    \
{                                                              \
   { 50544946,   314452,  -25806,     MS222_BP_FILTER_1  }, \
   { 50544946,   314452,  -25806,     MS222_BP_FILTER_2  }, \
   { 51239049,   232294,  -13936,     MS222_BP_FILTER_3  }, \
   { 52388115,   156738,   -7029,     MS222_BP_FILTER_4  }, \
   { 53961378,    99065,   -3425,     MS222_BP_FILTER_5  }, \
   { 59788149,     6642,    -150,     MS222_BP_FILTER_6  }, \
   { 53121988,    47726,    -805,     MS222_BP_FILTER_7  }, \
   { 60101688,      140,    -154,     335360000              }, \
   { 65022437,   -32531,     334,     390656000              }, \
   { 54268908,    22541,    -372,     430592000              }, \
   { 73524942,   -64465,     641,     473500000              }, \
   { 52585046,    24369,    -326,     535040000              }, \
   { 68451433,   -35142,     254,     562688000              }, \
   { 69310100,   -38493,     264,     600000000              }, \
   { 50755238,    18868,    -176,     654848000              }, \
   { 42143765,    45877,    -383,     720384000              }, \
   { 92511767,   -93070,     592,     793088000              }, \
   { 56275795,     4022,     -69,     865792000              }, \
   { 47648303,    24175,    -174,     MS222_RF_MAX}         \
}

/* {    X     ,    X    ,   X  ,           A           },                */
/* {    0     ,    1    ,   2  ,           G           },                */
/* {          ,         ,      ,           C           },                */
/* {    x     ,    x    ,   x  ,           1           },                */
/* {    1     ,    1    ,   1  ,           N           },                */
/* {    0     ,    0    ,   0  ,           B           },                */
/* {    0     ,    0    ,   0  ,                       },                */
/* {    0     ,    0    ,   0  ,                       },                */
/* {    0     ,    0    ,   0  ,                       },                */
/* {    0     ,    0    ,   0  ,                       },                */
/* {    0     ,    0    ,   0  ,                       },                */
/* {    0     ,    0    ,   0  ,                       },                */
/* {          ,         ,   0  ,                       },                */
/* {          ,         ,      ,                       },                */
/* {          ,         ,      ,                       },                */
/* {          ,         ,      ,                       },                */
#define MS222_INSTANCE_CUSTOM_COMMON_LVL_AGC1NB_TUNER_LT_OFF_ST_OFF        \
{                                                                                \
   { -353296473,      15022,   -135,      MS222AGC1_GAIN_Minus_11dB   }, \
   { -326948330,       8002,    -40,      MS222AGC1_GAIN_Minus_8dB    }, \
   { -295755910,     -14866,    198,      MS222AGC1_GAIN_Minus_5dB    }, \
   { -265929070,      -6964,    115,      MS222AGC1_GAIN_Minus_2dB    }, \
   { -230494030,       5669,    -10,      MS222AGC1_GAIN_Plus_1dB         }, \
   { -204057517,     -13333,    181,      MS222AGC1_GAIN_Plus_4dB         }, \
   { -172095183,     -14280,    203,      MS222AGC1_GAIN_Plus_7dB         }, \
   { -140936067,     -15536,    240,      MS222AGC1_GAIN_Plus_10dB        }, \
   { -106777320,     -17578,    230,      MS222AGC1_GAIN_Plus_13dB        }, \
   { -79847410,       14190,    -96,      MS222AGC1_GAIN_Plus_16dB        }, \
   { -45311661,        9327,    -25,      MS222AGC1_GAIN_Plus_19dB       }, \
   {         0,           0,      0,      MS222AGC1_GAIN_Plus_22dB       }  \
}

#define MS222_INSTANCE_CUSTOM_COMMON_LVL_AGC1NB_TUNER_LT_OFF_ST_ON          \
{                                                                                \
   { -352797083,       9716,    -88,      MS222AGC1_GAIN_Minus_11dB   }, \
   { -327520540,       3161,     18,      MS222AGC1_GAIN_Minus_8dB    }, \
   { -296171260,     -17660,    223,      MS222AGC1_GAIN_Minus_5dB    }, \
   { -266310830,     -10556,    147,      MS222AGC1_GAIN_Minus_2dB    }, \
   { -231036293,       2548,     14,      MS222AGC1_GAIN_Plus_1dB         }, \
   { -204386737,     -18262,    222,      MS222AGC1_GAIN_Plus_4dB         }, \
   { -172356037,     -19467,    236,      MS222AGC1_GAIN_Plus_7dB         }, \
   { -141172937,     -22381,    283,      MS222AGC1_GAIN_Plus_10dB        }, \
   { -106660657,     -27852,    295,      MS222AGC1_GAIN_Plus_13dB        }, \
   { -79232070,        -341,     -6,      MS222AGC1_GAIN_Plus_16dB        }, \
   { -44114841,      -11686,    102,      MS222AGC1_GAIN_Plus_19dB       }, \
   {   2544050,      -36685,    247,      MS222AGC1_GAIN_Plus_22dB       }  \
}

#define MS222_INSTANCE_CUSTOM_COMMON_LVL_AGC1NB_TUNER_LT_ON_ST_ON          \
{                                                                                \
   { -354109980,       9060,    -90,      MS222AGC1_GAIN_Minus_11dB   }, \
   { -328349970,       2598,     -2,      MS222AGC1_GAIN_Minus_8dB    }, \
   { -296721540,     -20309,    228,      MS222AGC1_GAIN_Minus_5dB    }, \
   { -267087650,     -13325,    148,      MS222AGC1_GAIN_Minus_2dB    }, \
   { -231461830,      -1953,     21,      MS222AGC1_GAIN_Plus_1dB         }, \
   { -204928130,     -23081,    223,      MS222AGC1_GAIN_Plus_4dB         }, \
   { -172642377,     -26687,    244,      MS222AGC1_GAIN_Plus_7dB         }, \
   { -141464157,     -28576,    280,      MS222AGC1_GAIN_Plus_10dB        }, \
   { -106945840,     -34895,    305,      MS222AGC1_GAIN_Plus_13dB        }, \
   { -79208435,       -8696,     19,      MS222AGC1_GAIN_Plus_16dB        }, \
   { -43828137,      -21667,    145,      MS222AGC1_GAIN_Plus_19dB       }, \
   {   2842769,      -48852,    325,      MS222AGC1_GAIN_Plus_22dB       }  \
}

#define MS222_INSTANCE_CUSTOM_COMMON_LVL_AGC1NB_MASTER_LT_OFF_ST_ON          \
{                                                                                \
   { -157256753,      17527,   -268,      MS222AGC1_GAIN_Minus_11dB   }, \
   { -131424170,      12492,   -223,      MS222AGC1_GAIN_Minus_8dB    }, \
   {  -99878629,      -9616,     -6,      MS222AGC1_GAIN_Minus_5dB    }, \
   {  -70569703,        343,   -123,      MS222AGC1_GAIN_Minus_2dB    }, \
   {  -34899443,      12381,   -263,      MS222AGC1_GAIN_Plus_1dB         }, \
   {   -8394208,      -7670,    -65,      MS222AGC1_GAIN_Plus_4dB         }, \
   {   23527409,      -8799,    -51,      MS222AGC1_GAIN_Plus_7dB         }, \
   {   54525144,     -10098,    -24,      MS222AGC1_GAIN_Plus_10dB        }, \
   {   88588461,     -12752,    -41,      MS222AGC1_GAIN_Plus_13dB        }, \
   {  115477980,      17416,   -364,      MS222AGC1_GAIN_Plus_16dB        }, \
   {  149721600,      11315,   -308,      MS222AGC1_GAIN_Plus_19dB       }, \
   {  194569980,      -3087,   -269,      MS222AGC1_GAIN_Plus_22dB       }  \
}

#define MS222_INSTANCE_CUSTOM_COMMON_LVL_AGC1NB_MASTER_LT_ON_ST_ON          \
{                                                                                \
   { -159939870,     18757,    -328,      MS222AGC1_GAIN_Minus_11dB   }, \
   { -133171943,      9275,    -238,      MS222AGC1_GAIN_Minus_8dB    }, \
   { -102226642,    -10213,     -51,      MS222AGC1_GAIN_Minus_5dB    }, \
   { -72577822,      -3199,    -140,      MS222AGC1_GAIN_Minus_2dB    }, \
   { -37130337,       9232,    -293,      MS222AGC1_GAIN_Plus_1dB         }, \
   { -10339955,     -11934,     -98,      MS222AGC1_GAIN_Plus_4dB         }, \
   {  21728243,     -14937,     -86,      MS222AGC1_GAIN_Plus_7dB         }, \
   {  53095912,     -15887,     -51,      MS222AGC1_GAIN_Plus_10dB        }, \
   {  87477003,     -19799,     -47,      MS222AGC1_GAIN_Plus_13dB        }, \
   { 114890843,       8832,    -349,      MS222AGC1_GAIN_Plus_16dB        }, \
   { 149495227,       1330,    -272,      MS222AGC1_GAIN_Plus_19dB       }, \
   { 194827333,     -16445,    -186,      MS222AGC1_GAIN_Plus_22dB       }  \
}

/* {    X     ,    X    ,   X  ,           A           },                */
/* {    0     ,    1    ,   2  ,           G           },                */
/* {          ,         ,      ,           C           },                */
/* {    x     ,    x    ,   x  ,           2           },                */
/* {    1     ,    1    ,   1  ,           N           },                */
/* {    0     ,    0    ,   0  ,           B           },                */
/* {    0     ,    0    ,   0  ,                       },                */
/* {    0     ,    0    ,   0  ,                       },                */
/* {    0     ,    0    ,   0  ,                       },                */
/* {    0     ,    0    ,   0  ,                       },                */
/* {    0     ,    0    ,   0  ,                       },                */
/* {    0     ,    0    ,   0  ,                       },                */
/* {          ,         ,   0  ,                       },                */
/* {          ,         ,      ,                       },                */
/* {          ,         ,      ,                       },                */
/* {          ,         ,      ,                       },                */
#define MS222_INSTANCE_CUSTOM_COMMON_LVL_AGC2NB  \
{                                                    \
   { -262186,    2085,    -120,       10    },       \
   { -264194,    2079,    -120,       21    },       \
   { -266973,    2079,    -119,       32    },       \
   { -269833,    2078,    -116,       43    },       \
   { -272289,    2076,    -114,       54    },       \
   { -274733,    2070,    -99,        65    },       \
   { -277588,    2067,    -81,        76    },       \
   { -280562,    2063,    -62,        87    },       \
   { -282784,    2054,    -43,        98    },       \
   { -284701,    2043,    -30,       109    },       \
   { -284809,    2027,    -22,       120    },       \
   { -283553,    2007,    -14,       131    },       \
   {  -281681,    1984,     0,       142    }        \
}

/* {    X     ,    X    ,   X  ,           A           },                */
/* {    0     ,    1    ,   2  ,           G           },                */
/* {          ,         ,      ,           C           },                */
/* {    x     ,    x    ,   x  ,           3           },                */
/* {    1     ,    1    ,   1  ,           N           },                */
/* {    0     ,    0    ,   0  ,           B           },                */
/* {    0     ,    0    ,   0  ,                       },                */
/* {    0     ,    0    ,   0  ,                       },                */
/* {    0     ,    0    ,   0  ,                       },                */
/* {    0     ,    0    ,   0  ,                       },                */
/* {    0     ,    0    ,   0  ,                       },                */
/* {    0     ,    0    ,   0  ,                       },                */
/* {          ,         ,   0  ,                       },                */
/* {          ,         ,      ,                       },                */
/* {          ,         ,      ,                       },                */
/* {          ,         ,      ,                       },                */
#define MS222_INSTANCE_CUSTOM_COMMON_LVL_AGC3NB						        \
{                                                                               \
   { -208116100,   -688,     2,    MS222AGC3_GAIN_Minus_6dB      }, \
   { -178917030,   -732,     3,    MS222AGC3_GAIN_Minus_3dB      }, \
   { -148438740,   -798,     3,    MS222AGC3_GAIN_0dB            }, \
   { -118542815,   -777,     3,    MS222AGC3_GAIN_Plus_3dB      }, \
   {  -88269478,   -778,     2,    MS222AGC3_GAIN_Plus_6dB       }, \
   {  -58950548,   -606,     1,    MS222AGC3_GAIN_Plus_9dB       }, \
   {  -29234591,   -457,     1,    MS222AGC3_GAIN_Plus_12dB      }, \
   {          0,      0,     0,    MS222AGC3_GAIN_Plus_15dB      }  \
}

/* Default configuration */
#define MS222_CONFIG_CURPOWERSTATE_DEF tmPowerMax
#define MS222_CONFIG_CURLLPOWERSTATE_DEF MS222_PowerMax
#define MS222_CONFIG_RF_DEF 0
#define MS222_CONFIG_PROG_RF_DEF 0
#define MS222_CONFIG_STANDARDMODE_DEF MS222_StandardMode_Max

    /* Power settings */
#define MS222_CONFIG_POWER_DEF_MASTER \
    MS222_CONFIG_CURPOWERSTATE_DEF,                     /* curPowerState */       \
    MS222_CONFIG_CURLLPOWERSTATE_DEF,                   /* curLLPowerState */     \
    {                                                       /* mapLLPowerState */     \
    MS222_PowerNormalMode,                          /* tmPowerOn (D0) */      \
    MS222_PowerStandbyWithLtOnWithXtalOn,           /* tmPowerStandby (D1) */ \
    MS222_PowerStandbyWithLtOnWithXtalOn,           /* tmPowerSuspend (D2) */ \
    MS222_PowerStandbyWithLtOnWithXtalOn            /* tmPowerOff (D3) */     \
}

#define MS222_CONFIG_POWER_DEF_SLAVE \
    MS222_CONFIG_CURPOWERSTATE_DEF,                     /* curPowerState */       \
    MS222_CONFIG_CURLLPOWERSTATE_DEF,                   /* curLLPowerState */     \
    {                                                       /* mapLLPowerState */     \
    MS222_PowerNormalMode,                          /* tmPowerOn (D0) */      \
    MS222_PowerStandbyWithXtalOn,			        /* tmPowerStandby (D1) */ \
    MS222_PowerStandbyWithXtalOn,					/* tmPowerSuspend (D2) */ \
    MS222_PowerStandbyWithXtalOn					/* tmPowerOff (D3) */     \
}

    /* Standard Presets Aggregation: */
#define MS222_CONFIG_STD_DEF_VCO_PULLING_MINUS \
    { \
    MS222_CONFIG_STD_DVBT_1_7MHZ_VCO_PULLING_MINUS, \
    MS222_CONFIG_STD_DVBT_6MHZ_VCO_PULLING_MINUS,   \
    MS222_CONFIG_STD_DVBT_7MHZ_VCO_PULLING_MINUS,   \
    MS222_CONFIG_STD_DVBT_8MHZ_VCO_PULLING_MINUS,   \
    MS222_CONFIG_STD_DVBT_10MHZ_VCO_PULLING_MINUS,  \
}

#define MS222_CONFIG_STD_DEF_VCO_PULLING_PLUS \
    { \
    MS222_CONFIG_STD_DVBT_1_7MHZ_VCO_PULLING_PLUS, \
    MS222_CONFIG_STD_DVBT_6MHZ_VCO_PULLING_PLUS,   \
    MS222_CONFIG_STD_DVBT_7MHZ_VCO_PULLING_PLUS,   \
    MS222_CONFIG_STD_DVBT_8MHZ_VCO_PULLING_PLUS,   \
    MS222_CONFIG_STD_DVBT_10MHZ_VCO_PULLING_PLUS,  \
}

#define MS222_CONFIG_REGMAP_DEF \
    { \
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0           \
}


#define MS222_CONFIG_MASTER \
    MS222_CONFIG_POWER_DEF_MASTER, \
    MS222_CONFIG_RF_DEF,                                /* uRF */                            \
    MS222_CONFIG_PROG_RF_DEF,                           /* uProgRF */                        \
    MS222_CONFIG_STANDARDMODE_DEF,                      /* StandardMode */                   \
    Null,                                                   /* pStandard */                      \
    False,                                                  /* bBufferMode */                    \
    False,                                                  /* Manual PLL Calc */                \
    MS222_LoopThrough_Enabled,                          /* single app 18274 */               \
    MS222_Mode_Dual,                                    /* single app */                     \
    MS222_XtalFreq_24000000,                            /* eXtalFreq */                      \
    MS222_XTOUT_Amp_400mV,                              /* eXTOutAmp */                      \
    MS222_XTOUT_Sinusoidal,                             /* eXTOut */                         \
    MS222_LNA_RANGE_Minus8dB_Plus22dB,					/* eLNA_Range */                     \
    MS222_IRQ_POLARITY_LOW,                             /* eIRQPolarity */                   \
    MS222_HwState_InitNotDone,                          /* eHwState */                       \
    MS222_CONFIG_STD_DEF_VCO_PULLING_MINUS, \
    MS222_CONFIG_REGMAP_DEF

#define MS222_CONFIG_SLAVE \
    MS222_CONFIG_POWER_DEF_SLAVE, \
    MS222_CONFIG_RF_DEF,                                /* uRF */                            \
    MS222_CONFIG_PROG_RF_DEF,                           /* uProgRF */                        \
    MS222_CONFIG_STANDARDMODE_DEF,                      /* StandardMode */                   \
    Null,                                                   /* pStandard */                      \
    True,                                                   /* bBufferMode */                    \
    False,                                                  /* Manual PLL Calc */                \
    MS222_LoopThrough_Disabled,                         /* single app 18274 */               \
    MS222_Mode_Dual,                                    /* single app */                     \
    MS222_XtalFreq_24000000,                            /* eXtalFreq */                      \
    MS222_XTOUT_Amp_400mV,                              /* eXTOutAmp */                      \
    MS222_XTOUT_Sinusoidal,                             /* eXTOut */                         \
    MS222_LNA_RANGE_Minus8dB_Plus22dB,					/* eLNA_Range */                     \
    MS222_IRQ_POLARITY_LOW,                             /* eIRQPolarity */                   \
    MS222_HwState_InitNotDone,                          /* eHwState */                       \
    MS222_CONFIG_STD_DEF_VCO_PULLING_PLUS, \
    MS222_CONFIG_REGMAP_DEF


    /* Custom Driver Instance Parameters: (Path 0) */
#define MS222_CONFIG_0 \
    MS222_CONFIG_MASTER

    /* Custom Driver Instance Parameters: (Path 1) */
#define MS222_CONFIG_1 \
    MS222_CONFIG_SLAVE


#ifdef __cplusplus
}
#endif

#endif /* _MS222_CONFIG_COMMON_H */

