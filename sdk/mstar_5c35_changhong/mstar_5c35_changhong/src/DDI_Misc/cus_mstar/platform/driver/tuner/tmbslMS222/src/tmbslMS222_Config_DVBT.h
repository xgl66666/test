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

#ifndef _MS222_CONFIG_DVBT_H
#define _MS222_CONFIG_DVBT_H

#ifdef __cplusplus
extern "C"
{
#endif

    /*============================================================================*/
    /* Types and defines:                                                         */
    /*============================================================================*/

    /* Driver settings version definition */
#define MS222_DVBT_SETTINGS_CUSTOMER_NUM      (0)                     /* SW Settings Customer Number */
#define MS222_DVBT_SETTINGS_PROJECT_NUM       (0)                     /* SW Settings Project Number  */
#define MS222_DVBT_SETTINGS_MAJOR_VER         (1)                     /* SW Settings Major Version   */
#define MS222_DVBT_SETTINGS_MINOR_VER         (5)                     /* SW Settings Minor Version   */

#define MS222_CONFIG_STD_DVBT_1_7MHZ_VCO_PULLING_MINUS \
    {																		/* DVB-T/T2 1.7MHz */	  \
    /****************************************************************/							  \
    /* IF Settings                                                  */							  \
    /****************************************************************/							  \
    850000-50000,																/* IF */				  \
    0,																	/* CF_Offset */			  \
    \
    /****************************************************************/							  \
    /* IF SELECTIVITY Settings                                      */							  \
    /****************************************************************/							  \
    MS222_LPF_1_5MHz,												/* LPF */				  \
    MS222_LPFOffset_plus_4pc,										/* LPF_Offset */		  \
    MS222_DC_Notch_IF_PPF_Disabled,									/* DC notch IF PPF */	  \
    MS222_HPF_Disabled,												/* Hi Pass */			  \
    MS222_HPFOffset_0pc,											/* HPF Offset */		  \
    MS222_IF_Notch_Disabled,										/* IF notch */			  \
    MS222_IF_Notch_Freq_6_25MHz,									/* IF Notch Freq */		  \
    MS222_IF_Notch_Offset_0pc,										/* IF Notch Offset */     \
    MS222_IFnotchToRSSI_Enabled,									/* IF notch To RSSI */    \
    \
    /****************************************************************/							  \
    /* AGC Settings                                                 */							  \
    /****************************************************************/							  \
    MS222AGC1_GAIN_Free,                                            /* AGC1 GAIN */           \
    MS222AGC1_GAIN_SMOOTH_ALGO_Enabled,                             /* AGC1 GAIN SMOOTH ALGO */ \
    {                                                                   /* AGC1 TOP DN/UP ES1 */  \
        {                                                                                         \
            (UInt32)0,                                                                            \
            MS222_AGC1_TOP_I2C_DN_UP_d97_u91dBuV                                              \
        },                                                                                        \
        {                                                                                         \
            (UInt32)100000000,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d97_u91dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        }                                                                                         \
    },                                                                                            \
    {                                                                   /* AGC1 TOP DN/UP ES2 */  \
        {                                                                                         \
            (UInt32)0,                                                                            \
            MS222_AGC1_TOP_I2C_DN_UP_d97_u91dBuV                                              \
        },                                                                                        \
        {                                                                                         \
            (UInt32)100000000,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d97_u91dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        }                                                                                         \
    },                                                                                            \
    {                                                                   /* AGC1 TOP DN/UP ES3 */  \
        {                                                                                         \
            (UInt32)0,                                                                            \
            MS222_AGC1_TOP_I2C_DN_UP_d97_u91dBuV                                              \
        },                                                                                        \
        {                                                                                         \
            (UInt32)100000000,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d97_u91dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        }                                                                                         \
    },                                                                                            \
    MS222_AGC1_TOP_STRATEGY_4,										/* AGC1 TOP STRATEGY 0 */ \
    MS222_AGC1_DET_SPEED_125KHz,									/* AGC1 Det Speed */	  \
    MS222_AGC1_SMOOTH_T_CST_1_6ms,                                  /* AGC1 Smooth T Cst */   \
    MS222_AGC1_Do_Step_0_512ms,                                      /* AGC1 Do Step  */                \
    MS222_LNA_ZIN_NF,												/* LNA_Zin */			  \
    True,                                                               /* AGC2 Gain Control En ES1 */ \
    False,                                                              /* AGC2 Gain Control En ES2 */ \
    True,                                                               /* AGC2 Gain Control En ES3 */ \
    {                                                                   /* AGC2 TOP ES1 */            \
        {                                                                                         \
            (UInt32)0,                                                                                    \
            (UInt8)99,                                                                                   \
            (UInt8)97                                                                                    \
        },                                                                                        \
        {                                                                                         \
            (UInt32)320000000,                                                                            \
            (UInt8)102,                                                                                  \
            (UInt8)100                                                                                   \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        }                                                                                         \
    },                                                                                            \
    {                                                                   /* AGC2 TOP ES2 */            \
        {                                                                                         \
            (UInt32)0,                                                                                    \
            (UInt8)103,                                                                                   \
            (UInt8)102                                                                                    \
        },                                                                                        \
        {                                                                                         \
            (UInt32)320000000,                                                                            \
            (UInt8)106,                                                                                  \
            (UInt8)105                                                                                   \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        }                                                                                         \
    },                                                                                         \
    {                                                                   /* AGC2 TOP ES3 */            \
        {                                                                                         \
            (UInt32)0,                                                                                    \
            (UInt8)97,                                                                                   \
            (UInt8)96                                                                                    \
        },                                                                                        \
        {                                                                                         \
            (UInt32)320000000,                                                                            \
            (UInt8)99,                                                                                  \
            (UInt8)98                                                                                   \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        }                                                                                         \
    },                                                                                         \
    MS222_AGC2_DET_SPEED_125KHz,									/* AGC2 Det Speed */	  \
    False,                                                              /* AGC2 Adapt TOP23 Enable ES2 */     \
    0,                                                                  /* AGC2 Adapt TOP23 Delta (in dB) ES2 */     \
    False,                                                              /* AGC2 Adapt TOP23 Enable ES3 */     \
    0,                                                                  /* AGC2 Adapt TOP23 Delta (in dB) ES3 */     \
    True,                                                               /* AGC2 Gain Control Speed (False 1ms ; True 0.5ms)   */     \
    MS222_AGC2_Do_Step_0_512ms,                                      /* AGC2 Do Step  */                \
    MS222_AGC2_Up_Step_23_8,                                         /* AGC2 Up Step  */                \
    MS222_AGC2_Up_Udld_Step_23_8,                                    /* AGC2 Up Udld Step */ \
    -8,                                                                 /* AGC2 TOP Up Udld */ \
    5,                                                                  /* AGC2 Fast Auto Delta */ \
    MS222_DET12_CINT_50fF,											/* Det12 Cint */		  \
    {                                                                   /* AGC3 TOP */            \
        {                                                                                         \
            (UInt32)0,                                                                                    \
            (UInt8)112,                                                                                  \
            (UInt8)107                                                                                   \
        },                                                                                        \
        {                                                                                         \
            (UInt32)320000000,                                                                            \
            (UInt8)110,                                                                                  \
            (UInt8)105                                                                                   \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            (UInt8)-1,                                                                                   \
            (UInt8)-1                                                                                    \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            (UInt8)-1,                                                                                   \
            (UInt8)-1                                                                                    \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            (UInt8)-1,                                                                                   \
            (UInt8)-1                                                                                    \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            (UInt8)-1,                                                                                   \
            (UInt8)-1                                                                                    \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            (UInt8)-1,                                                                                   \
            (UInt8)-1                                                                                    \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            (UInt8)-1,                                                                                   \
            (UInt8)-1                                                                                    \
        }                                                                                         \
    },                                                                                            \
    MS222_IF_Output_Level_1Vpp_min_6_24dB,							/* IF Output Level */	  \
    {                                                                   /* S2D Gain */            \
        {                                                                                         \
            (UInt32)0,                                                                                    \
            MS222_S2D_Gain_9dB                                                                \
        },                                                                                        \
        {                                                                                         \
            (UInt32)320000000,                                                                            \
            MS222_S2D_Gain_6dB                                                                \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            MS222_S2D_Gain_6dB                                                                \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            MS222_S2D_Gain_6dB                                                                \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            MS222_S2D_Gain_6dB                                                                \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            MS222_S2D_Gain_6dB                                                                \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            MS222_S2D_Gain_6dB                                                                \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            MS222_S2D_Gain_6dB                                                                \
        }                                                                                         \
    },                                                                                            \
    \
    True,                                                                   /* Fast Auto Launch */          \
    MS222_AGC_Timer_Mod_Fast_Auto_01,                                   /* AGC Timer Mod Fast Auto */   \
    False,                                                                  /* AGC2 Up Udld Fast Reduce */  \
    False,                                                                  /* AGC2 Up Fast Reduce */       \
    MS222_AGC_Type_Fast_Auto_10,                                        /* AGC Type Fast Auto */        \
    /****************************************************************/							  \
    /* GSK Settings                                                 */							  \
    /****************************************************************/							  \
    MS222_AGCK_Time_Constant_32_768ms,								/* AGCK Time Constant */  \
    MS222_RSSI_HP_FC_2_25M,											/* RSSI HP FC */		  \
    \
    /****************************************************************/							  \
    /* H3H5 Settings                                                */							  \
    /****************************************************************/							  \
    MS222_VHF_III_Mode_Disabled,										/* VHF III Mode */		  \
    \
    /****************************************************************/							  \
    /*RSSI Settings                                                 */							  \
    /****************************************************************/							  \
    MS222_RSSI_CAP_VAL_3pF,											/* RSSI_Ck_speed */		  \
    MS222_RSSI_CK_SPEED_31_25kHz,									/* RSSI_Cap_Val */		  \
    \
    0x44,                                                               /* ES1 Power Saving Byte 1 */ \
    0x06,                                                               /* ES1 Power Saving Byte 2 */ \
    0x07,                                                               /* ES1 Power Saving Byte 3 */ \
    0x88,                                                               /* ES2 Power Saving Byte 1 */ \
    0x42,                                                               /* ES2 Power Saving Byte 2 */ \
    0x0B,                                                               /* ES2 Power Saving Byte 3 */ \
    0x88,                                                               /* ES3 Power Saving Byte 1 */ \
    0x42,                                                               /* ES3 Power Saving Byte 2 */ \
    0x0B,                                                               /* ES3 Power Saving Byte 3 */ \
}

#define MS222_CONFIG_STD_DVBT_1_7MHZ_VCO_PULLING_PLUS \
    {																		/* DVB-T/T2 1.7MHz */	  \
    /****************************************************************/							  \
    /* IF Settings                                                  */							  \
    /****************************************************************/							  \
    850000+50000,																/* IF */				  \
    0,																	/* CF_Offset */			  \
    \
    /****************************************************************/							  \
    /* IF SELECTIVITY Settings                                      */							  \
    /****************************************************************/							  \
    MS222_LPF_1_5MHz,												/* LPF */				  \
    MS222_LPFOffset_0pc,    										/* LPF_Offset */		  \
    MS222_DC_Notch_IF_PPF_Disabled,									/* DC notch IF PPF */	  \
    MS222_HPF_Disabled,												/* Hi Pass */			  \
    MS222_HPFOffset_0pc,											/* HPF Offset */		  \
    MS222_IF_Notch_Disabled,										/* IF notch */			  \
    MS222_IF_Notch_Freq_6_25MHz,									/* IF Notch Freq */		  \
    MS222_IF_Notch_Offset_0pc,										/* IF Notch Offset */     \
    MS222_IFnotchToRSSI_Enabled,									/* IF notch To RSSI */    \
    \
    /****************************************************************/							  \
    /* AGC Settings                                                 */							  \
    /****************************************************************/							  \
    MS222AGC1_GAIN_Free,                                            /* AGC1 GAIN */           \
    MS222AGC1_GAIN_SMOOTH_ALGO_Enabled,                             /* AGC1 GAIN SMOOTH ALGO */ \
    {                                                                   /* AGC1 TOP DN/UP ES1 */  \
        {                                                                                         \
            (UInt32)0,                                                                            \
            MS222_AGC1_TOP_I2C_DN_UP_d97_u91dBuV                                              \
        },                                                                                        \
        {                                                                                         \
            (UInt32)100000000,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d97_u91dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        }                                                                                         \
    },                                                                                            \
    {                                                                   /* AGC1 TOP DN/UP ES2 */  \
        {                                                                                         \
            (UInt32)0,                                                                            \
            MS222_AGC1_TOP_I2C_DN_UP_d97_u91dBuV                                              \
        },                                                                                        \
        {                                                                                         \
            (UInt32)100000000,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d97_u91dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        }                                                                                         \
    },                                                                                            \
    {                                                                   /* AGC1 TOP DN/UP ES3 */  \
        {                                                                                         \
            (UInt32)0,                                                                            \
            MS222_AGC1_TOP_I2C_DN_UP_d97_u91dBuV                                              \
        },                                                                                        \
        {                                                                                         \
            (UInt32)100000000,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d97_u91dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        }                                                                                         \
    },                                                                                            \
    MS222_AGC1_TOP_STRATEGY_4,										/* AGC1 TOP STRATEGY 0 */ \
    MS222_AGC1_DET_SPEED_125KHz,									/* AGC1 Det Speed */	  \
    MS222_AGC1_SMOOTH_T_CST_1_6ms,                                  /* AGC1 Smooth T Cst */   \
    MS222_AGC1_Do_Step_0_512ms,                                      /* AGC1 Do Step  */                \
    MS222_LNA_ZIN_NF,												/* LNA_Zin */			  \
    True,                                                               /* AGC2 Gain Control En ES1 */ \
    False,                                                              /* AGC2 Gain Control En ES2 */ \
    True,                                                              /* AGC2 Gain Control En ES3 */ \
    {                                                                   /* AGC2 TOP ES1 */            \
        {                                                                                         \
            (UInt32)0,                                                                                    \
            (UInt8)99,                                                                                   \
            (UInt8)97                                                                                    \
        },                                                                                        \
        {                                                                                         \
            (UInt32)320000000,                                                                            \
            (UInt8)102,                                                                                  \
            (UInt8)100                                                                                   \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        }                                                                                         \
    },                                                                                            \
    {                                                                   /* AGC2 TOP ES2 */            \
        {                                                                                         \
            (UInt32)0,                                                                                    \
            (UInt8)103,                                                                                   \
            (UInt8)102                                                                                    \
        },                                                                                        \
        {                                                                                         \
            (UInt32)320000000,                                                                            \
            (UInt8)106,                                                                                  \
            (UInt8)105                                                                                   \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        }                                                                                         \
    },                                                                                         \
    {                                                                   /* AGC2 TOP ES3 */            \
        {                                                                                         \
            (UInt32)0,                                                                                    \
            (UInt8)97,                                                                                   \
            (UInt8)96                                                                                    \
        },                                                                                        \
        {                                                                                         \
            (UInt32)320000000,                                                                            \
            (UInt8)98,                                                                                  \
            (UInt8)99                                                                                   \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        }                                                                                         \
    },                                                                                         \
    MS222_AGC2_DET_SPEED_125KHz,									/* AGC2 Det Speed */	  \
    False,                                                              /* AGC2 Adapt TOP23 Enable ES2 */     \
    0,                                                                  /* AGC2 Adapt TOP23 Delta (in dB) ES2 */     \
    False,                                                              /* AGC2 Adapt TOP23 Enable ES3 */     \
    0,                                                                  /* AGC2 Adapt TOP23 Delta (in dB) ES3 */     \
    True,                                                               /* AGC2 Gain Control Speed (False 1ms ; True 0.5ms)   */     \
    MS222_AGC2_Do_Step_0_512ms,                                      /* AGC2 Do Step  */                \
    MS222_AGC2_Up_Step_23_8,                                         /* AGC2 Up Step  */                \
    MS222_AGC2_Up_Udld_Step_23_8,                                    /* AGC2 Up Udld Step */ \
    -8,                                                                 /* AGC2 TOP Up Udld */ \
    5,                                                                  /* AGC2 Fast Auto Delta */ \
    MS222_DET12_CINT_50fF,											/* Det12 Cint */		  \
    {                                                                   /* AGC3 TOP */            \
        {                                                                                         \
            (UInt32)0,                                                                                    \
            (UInt8)112,                                                                                  \
            (UInt8)107                                                                                   \
        },                                                                                        \
        {                                                                                         \
            (UInt32)320000000,                                                                            \
            (UInt8)110,                                                                                  \
            (UInt8)105                                                                                   \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            (UInt8)-1,                                                                                   \
            (UInt8)-1                                                                                    \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            (UInt8)-1,                                                                                   \
            (UInt8)-1                                                                                    \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            (UInt8)-1,                                                                                   \
            (UInt8)-1                                                                                    \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            (UInt8)-1,                                                                                   \
            (UInt8)-1                                                                                    \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            (UInt8)-1,                                                                                   \
            (UInt8)-1                                                                                    \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            (UInt8)-1,                                                                                   \
            (UInt8)-1                                                                                    \
        }                                                                                         \
    },                                                                                            \
    MS222_IF_Output_Level_1Vpp_min_6_24dB,							/* IF Output Level */	  \
    {                                                                   /* S2D Gain */            \
        {                                                                                         \
            (UInt32)0,                                                                                    \
            MS222_S2D_Gain_9dB                                                                \
        },                                                                                        \
        {                                                                                         \
            (UInt32)320000000,                                                                            \
            MS222_S2D_Gain_6dB                                                                \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            MS222_S2D_Gain_6dB                                                                \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            MS222_S2D_Gain_6dB                                                                \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            MS222_S2D_Gain_6dB                                                                \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            MS222_S2D_Gain_6dB                                                                \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            MS222_S2D_Gain_6dB                                                                \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            MS222_S2D_Gain_6dB                                                                \
        }                                                                                          \
    },                                                                                            \
    \
    True,                                                                   /* Fast Auto Launch */          \
    MS222_AGC_Timer_Mod_Fast_Auto_01,                                   /* AGC Timer Mod Fast Auto */   \
    False,                                                                  /* AGC2 Up Udld Fast Reduce */  \
    False,                                                                  /* AGC2 Up Fast Reduce */       \
    MS222_AGC_Type_Fast_Auto_10,                                        /* AGC Type Fast Auto */        \
    /****************************************************************/							  \
    /* GSK Settings                                                 */							  \
    /****************************************************************/							  \
    MS222_AGCK_Time_Constant_32_768ms,								/* AGCK Time Constant */  \
    MS222_RSSI_HP_FC_2_25M,											/* RSSI HP FC */		  \
    \
    /****************************************************************/							  \
    /* H3H5 Settings                                                */							  \
    /****************************************************************/							  \
    MS222_VHF_III_Mode_Disabled,										/* VHF III Mode */		  \
    \
    /****************************************************************/							  \
    /*RSSI Settings                                                 */							  \
    /****************************************************************/							  \
    MS222_RSSI_CAP_VAL_3pF,											/* RSSI_Ck_speed */		  \
    MS222_RSSI_CK_SPEED_31_25kHz,									/* RSSI_Cap_Val */		  \
    \
    0x44,                                                               /* ES1 Power Saving Byte 1 */ \
    0x06,                                                               /* ES1 Power Saving Byte 2 */ \
    0x07,                                                               /* ES1 Power Saving Byte 3 */ \
    0x88,                                                               /* ES2 Power Saving Byte 1 */ \
    0x42,                                                               /* ES2 Power Saving Byte 2 */ \
    0x0B,                                                               /* ES2 Power Saving Byte 3 */ \
    0x88,                                                               /* ES3 Power Saving Byte 1 */ \
    0x42,                                                               /* ES3 Power Saving Byte 2 */ \
    0x0B,                                                               /* ES3 Power Saving Byte 3 */ \
}

#define MS222_CONFIG_STD_DVBT_6MHZ_VCO_PULLING_MINUS \
    {																		/* QAM 6MHz */			  \
    /****************************************************************/							  \
    /* IF Settings                                                  */							  \
    /****************************************************************/							  \
    4000000-50000,															/* IF */				  \
    0,																	/* CF_Offset */			  \
    \
    /****************************************************************/							  \
    /* IF SELECTIVITY Settings                                      */							  \
    /****************************************************************/							  \
    MS222_LPF_7MHz,													/* LPF */				  \
    MS222_LPFOffset_min_8pc,										/* LPF_Offset */		  \
    MS222_DC_Notch_IF_PPF_Disabled,									/* DC notch IF PPF */	  \
    MS222_HPF_1MHz,												/* Hi Pass */			  \
    MS222_HPFOffset_0pc,											/* HPF Offset */		  \
    MS222_IF_Notch_Enabled,											/* IF notch */			  \
    MS222_IF_Notch_Freq_7_25MHz,									/* IF Notch Freq */		  \
    MS222_IF_Notch_Offset_0pc,									/* IF Notch Offset */	  \
    MS222_IFnotchToRSSI_Enabled,									/* IF notch To RSSI */	  \
    \
    /****************************************************************/							  \
    /* AGC Settings                                                 */						      \
    /****************************************************************/							  \
    MS222AGC1_GAIN_Free,                                            /* AGC1 GAIN */           \
    MS222AGC1_GAIN_SMOOTH_ALGO_Enabled,                             /* AGC1 GAIN SMOOTH ALGO */ \
    {                                                                   /* AGC1 TOP DN/UP ES1 */  \
        {                                                                                         \
            (UInt32)0,                                                                            \
            MS222_AGC1_TOP_I2C_DN_UP_d97_u91dBuV                                              \
        },                                                                                        \
        {                                                                                         \
            (UInt32)100000000,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d97_u91dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        }                                                                                         \
    },                                                                                            \
    {                                                                   /* AGC1 TOP DN/UP ES2 */  \
        {                                                                                         \
            (UInt32)0,                                                                            \
            MS222_AGC1_TOP_I2C_DN_UP_d97_u91dBuV                                              \
        },                                                                                        \
        {                                                                                         \
            (UInt32)100000000,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d97_u91dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        }                                                                                         \
    },                                                                                            \
    {                                                                   /* AGC1 TOP DN/UP ES3 */  \
        {                                                                                         \
            (UInt32)0,                                                                            \
            MS222_AGC1_TOP_I2C_DN_UP_d97_u91dBuV                                              \
        },                                                                                        \
        {                                                                                         \
            (UInt32)100000000,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d97_u91dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        }                                                                                         \
    },                                                                                            \
    MS222_AGC1_TOP_STRATEGY_4,										/* AGC1 TOP STRATEGY 0 */ \
    MS222_AGC1_DET_SPEED_125KHz,									/* AGC1 Det Speed */	  \
    MS222_AGC1_SMOOTH_T_CST_1_6ms,                                  /* AGC1 Smooth T Cst */   \
    MS222_AGC1_Do_Step_0_512ms,                                      /* AGC1 Do Step  */                \
    MS222_LNA_ZIN_NF,												/* LNA_Zin */			  \
    True,                                                               /* AGC2 Gain Control En ES1 */ \
    False,                                                              /* AGC2 Gain Control En ES2 */ \
    True,                                                              /* AGC2 Gain Control En ES3 */ \
    {                                                                   /* AGC2 TOP ES1 */            \
        {                                                                                         \
            (UInt32)0,                                                                                    \
            (UInt8)99,                                                                                   \
            (UInt8)97                                                                                    \
        },                                                                                        \
        {                                                                                         \
            (UInt32)320000000,                                                                            \
            (UInt8)102,                                                                                  \
            (UInt8)100                                                                                   \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        }                                                                                         \
    },                                                                                            \
    {                                                                   /* AGC2 TOP ES2 */            \
        {                                                                                         \
            (UInt32)0,                                                                                    \
            (UInt8)103,                                                                                   \
            (UInt8)102                                                                                    \
        },                                                                                        \
        {                                                                                         \
            (UInt32)320000000,                                                                            \
            (UInt8)106,                                                                                  \
            (UInt8)105                                                                                   \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        }                                                                                         \
    },                                                                                         \
    {                                                                   /* AGC2 TOP ES3 */            \
        {                                                                                         \
            (UInt32)0,                                                                                    \
            (UInt8)97,                                                                                   \
            (UInt8)96                                                                                    \
        },                                                                                        \
        {                                                                                         \
            (UInt32)320000000,                                                                            \
            (UInt8)99,                                                                                  \
            (UInt8)98                                                                                   \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        }                                                                                         \
    },                                                                                         \
    MS222_AGC2_DET_SPEED_125KHz,									/* AGC2 Det Speed */	  \
    False,                                                              /* AGC2 Adapt TOP23 Enable ES2 */     \
    0,                                                                  /* AGC2 Adapt TOP23 Delta (in dB) ES2 */     \
    True,                                                              /* AGC2 Adapt TOP23 Enable ES3 */     \
    0,                                                                  /* AGC2 Adapt TOP23 Delta (in dB) ES3 */     \
    True,                                                               /* AGC2 Gain Control Speed (False 1ms ; True 0.5ms)   */     \
    MS222_AGC2_Do_Step_0_512ms,                                      /* AGC2 Do Step  */                \
    MS222_AGC2_Up_Step_23_8,                                         /* AGC2 Up Step  */                \
    MS222_AGC2_Up_Udld_Step_23_8,                                    /* AGC2 Up Udld Step */ \
    -8,                                                                 /* AGC2 TOP Up Udld */ \
    5,                                                                  /* AGC2 Fast Auto Delta */ \
    MS222_DET12_CINT_50fF,											/* Det12 Cint */		  \
    {                                                                   /* AGC3 TOP */            \
        {                                                                                         \
            (UInt32)0,                                                                                    \
            (UInt8)112,                                                                                  \
            (UInt8)107                                                                                   \
        },                                                                                        \
        {                                                                                         \
            (UInt32)320000000,                                                                            \
            (UInt8)110,                                                                                  \
            (UInt8)105                                                                                   \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            (UInt8)-1,                                                                                   \
            (UInt8)-1                                                                                    \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            (UInt8)-1,                                                                                   \
            (UInt8)-1                                                                                    \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            (UInt8)-1,                                                                                   \
            (UInt8)-1                                                                                    \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            (UInt8)-1,                                                                                   \
            (UInt8)-1                                                                                    \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            (UInt8)-1,                                                                                   \
            (UInt8)-1                                                                                    \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            (UInt8)-1,                                                                                   \
            (UInt8)-1                                                                                    \
        }                                                                                         \
    },                                                                                            \
    MS222_IF_Output_Level_1Vpp_min_6_24dB,							/* IF Output Level */     \
    {                                                                   /* S2D Gain */            \
        {                                                                                         \
            (UInt32)0,                                                                                    \
            MS222_S2D_Gain_9dB                                                                \
        },                                                                                        \
        {                                                                                         \
            (UInt32)320000000,                                                                            \
            MS222_S2D_Gain_6dB                                                                \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            MS222_S2D_Gain_6dB                                                                \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            MS222_S2D_Gain_6dB                                                                \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            MS222_S2D_Gain_6dB                                                                \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            MS222_S2D_Gain_6dB                                                                \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            MS222_S2D_Gain_6dB                                                                \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            MS222_S2D_Gain_6dB                                                                \
        }                                                                                          \
    },                                                                                            \
    \
    True,                                                                   /* Fast Auto Launch */          \
    MS222_AGC_Timer_Mod_Fast_Auto_01,                                   /* AGC Timer Mod Fast Auto */   \
    False,                                                                  /* AGC2 Up Udld Fast Reduce */  \
    False,                                                                  /* AGC2 Up Fast Reduce */       \
    MS222_AGC_Type_Fast_Auto_10,                                        /* AGC Type Fast Auto */        \
    /****************************************************************/							  \
    /* GSK Settings                                                 */							  \
    /****************************************************************/							  \
    MS222_AGCK_Time_Constant_32_768ms,								/* AGCK Time Constant */  \
    MS222_RSSI_HP_FC_2_25M,											/* RSSI HP FC */		  \
    \
    /****************************************************************/							  \
    /* H3H5 Settings                                                */							  \
    /****************************************************************/							  \
    MS222_VHF_III_Mode_Disabled,										/* VHF III Mode */		  \
    \
    /****************************************************************/							  \
    /*RSSI Settings                                                 */							  \
    /****************************************************************/							  \
    MS222_RSSI_CAP_VAL_3pF,											/* RSSI_Ck_speed */		  \
    MS222_RSSI_CK_SPEED_31_25kHz,									/* RSSI_Cap_Val */		  \
    \
    0x44,                                                               /* ES1 Power Saving Byte 1 */ \
    0x06,                                                               /* ES1 Power Saving Byte 2 */ \
    0x07,                                                               /* ES1 Power Saving Byte 3 */ \
    0x88,                                                               /* ES2 Power Saving Byte 1 */ \
    0x42,                                                               /* ES2 Power Saving Byte 2 */ \
    0x0B,                                                               /* ES2 Power Saving Byte 3 */ \
    0x88,                                                               /* ES3 Power Saving Byte 1 */ \
    0x42,                                                               /* ES3 Power Saving Byte 2 */ \
    0x0B,                                                               /* ES3 Power Saving Byte 3 */ \
}

#define MS222_CONFIG_STD_DVBT_6MHZ_VCO_PULLING_PLUS \
    {																		/* QAM 6MHz */			  \
    /****************************************************************/							  \
    /* IF Settings                                                  */							  \
    /****************************************************************/							  \
    4000000+50000,															/* IF */				  \
    0,																	/* CF_Offset */			  \
    \
    /****************************************************************/							  \
    /* IF SELECTIVITY Settings                                      */							  \
    /****************************************************************/							  \
    MS222_LPF_7MHz,													/* LPF */				  \
    MS222_LPFOffset_0pc,										/* LPF_Offset */		  \
    MS222_DC_Notch_IF_PPF_Disabled,									/* DC notch IF PPF */	  \
    MS222_HPF_1MHz,												/* Hi Pass */			  \
    MS222_HPFOffset_0pc,											/* HPF Offset */		  \
    MS222_IF_Notch_Enabled,											/* IF notch */			  \
    MS222_IF_Notch_Freq_7_25MHz,									/* IF Notch Freq */		  \
    MS222_IF_Notch_Offset_0pc,									/* IF Notch Offset */	  \
    MS222_IFnotchToRSSI_Enabled,									/* IF notch To RSSI */	  \
    \
    /****************************************************************/							  \
    /* AGC Settings                                                 */						      \
    /****************************************************************/							  \
    MS222AGC1_GAIN_Free,                                            /* AGC1 GAIN */           \
    MS222AGC1_GAIN_SMOOTH_ALGO_Enabled,                             /* AGC1 GAIN SMOOTH ALGO */ \
    {                                                                   /* AGC1 TOP DN/UP ES1 */  \
        {                                                                                         \
            (UInt32)0,                                                                            \
            MS222_AGC1_TOP_I2C_DN_UP_d97_u91dBuV                                              \
        },                                                                                        \
        {                                                                                         \
            (UInt32)100000000,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d97_u91dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        }                                                                                         \
    },                                                                                            \
    {                                                                   /* AGC1 TOP DN/UP ES2 */  \
        {                                                                                         \
            (UInt32)0,                                                                            \
            MS222_AGC1_TOP_I2C_DN_UP_d97_u91dBuV                                              \
        },                                                                                        \
        {                                                                                         \
            (UInt32)100000000,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d97_u91dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        }                                                                                         \
    },                                                                                            \
    {                                                                   /* AGC1 TOP DN/UP ES3 */  \
        {                                                                                         \
            (UInt32)0,                                                                            \
            MS222_AGC1_TOP_I2C_DN_UP_d97_u91dBuV                                              \
        },                                                                                        \
        {                                                                                         \
            (UInt32)100000000,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d97_u91dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        }                                                                                         \
    },                                                                                            \
    MS222_AGC1_TOP_STRATEGY_4,										/* AGC1 TOP STRATEGY 0 */ \
    MS222_AGC1_DET_SPEED_125KHz,									/* AGC1 Det Speed */	  \
    MS222_AGC1_SMOOTH_T_CST_1_6ms,                                  /* AGC1 Smooth T Cst */   \
    MS222_AGC1_Do_Step_0_512ms,                                      /* AGC1 Do Step  */                \
    MS222_LNA_ZIN_NF,												/* LNA_Zin */			  \
    True,                                                               /* AGC2 Gain Control En ES1 */ \
    False,                                                              /* AGC2 Gain Control En ES2 */ \
    True,                                                              /* AGC2 Gain Control En ES3 */ \
    {                                                                   /* AGC2 TOP ES1 */            \
        {                                                                                         \
            (UInt32)0,                                                                                    \
            (UInt8)99,                                                                                   \
            (UInt8)97                                                                                    \
        },                                                                                        \
        {                                                                                         \
            (UInt32)320000000,                                                                            \
            (UInt8)102,                                                                                  \
            (UInt8)100                                                                                   \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        }                                                                                         \
    },                                                                                            \
    {                                                                   /* AGC2 TOP ES2 */            \
        {                                                                                         \
            (UInt32)0,                                                                                    \
            (UInt8)103,                                                                                   \
            (UInt8)102                                                                                    \
        },                                                                                        \
        {                                                                                         \
            (UInt32)320000000,                                                                            \
            (UInt8)106,                                                                                  \
            (UInt8)105                                                                                   \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        }                                                                                         \
    },                                                                                         \
    {                                                                   /* AGC2 TOP ES3 */            \
        {                                                                                         \
            (UInt32)0,                                                                                    \
            (UInt8)97,                                                                                   \
            (UInt8)96                                                                                    \
        },                                                                                        \
        {                                                                                         \
            (UInt32)320000000,                                                                            \
            (UInt8)99,                                                                                  \
            (UInt8)98                                                                                   \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        }                                                                                         \
    },                                                                                         \
    MS222_AGC2_DET_SPEED_125KHz,									/* AGC2 Det Speed */	  \
    False,                                                              /* AGC2 Adapt TOP23 Enable ES2 */     \
    0,                                                                  /* AGC2 Adapt TOP23 Delta (in dB) ES2 */     \
    True,                                                              /* AGC2 Adapt TOP23 Enable ES3 */     \
    0,                                                                  /* AGC2 Adapt TOP23 Delta (in dB) ES3 */     \
    True,                                                               /* AGC2 Gain Control Speed (False 1ms ; True 0.5ms)   */     \
    MS222_AGC2_Do_Step_0_512ms,                                      /* AGC2 Do Step  */                \
    MS222_AGC2_Up_Step_23_8,                                         /* AGC2 Up Step  */                \
    MS222_AGC2_Up_Udld_Step_23_8,                                    /* AGC2 Up Udld Step */ \
    -8,                                                                 /* AGC2 TOP Up Udld */ \
    5,                                                                  /* AGC2 Fast Auto Delta */ \
    MS222_DET12_CINT_50fF,											/* Det12 Cint */		  \
    {                                                                   /* AGC3 TOP */            \
        {                                                                                         \
            (UInt32)0,                                                                                    \
            (UInt8)112,                                                                                  \
            (UInt8)107                                                                                   \
        },                                                                                        \
        {                                                                                         \
            (UInt32)320000000,                                                                            \
            (UInt8)110,                                                                                  \
            (UInt8)105                                                                                   \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            (UInt8)-1,                                                                                   \
            (UInt8)-1                                                                                    \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            (UInt8)-1,                                                                                   \
            (UInt8)-1                                                                                    \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            (UInt8)-1,                                                                                   \
            (UInt8)-1                                                                                    \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            (UInt8)-1,                                                                                   \
            (UInt8)-1                                                                                    \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            (UInt8)-1,                                                                                   \
            (UInt8)-1                                                                                    \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            (UInt8)-1,                                                                                   \
            (UInt8)-1                                                                                    \
        }                                                                                         \
    },                                                                                            \
    MS222_IF_Output_Level_1Vpp_min_6_24dB,							/* IF Output Level */     \
    {                                                                   /* S2D Gain */            \
        {                                                                                         \
            (UInt32)0,                                                                                    \
            MS222_S2D_Gain_9dB                                                                \
        },                                                                                        \
        {                                                                                         \
            (UInt32)320000000,                                                                            \
            MS222_S2D_Gain_6dB                                                                \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            MS222_S2D_Gain_6dB                                                                \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            MS222_S2D_Gain_6dB                                                                \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            MS222_S2D_Gain_6dB                                                                \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            MS222_S2D_Gain_6dB                                                                \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            MS222_S2D_Gain_6dB                                                                \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            MS222_S2D_Gain_6dB                                                                \
        }                                                                                          \
    },                                                                                            \
    \
    True,                                                                   /* Fast Auto Launch */          \
    MS222_AGC_Timer_Mod_Fast_Auto_01,                                   /* AGC Timer Mod Fast Auto */   \
    False,                                                                  /* AGC2 Up Udld Fast Reduce */  \
    False,                                                                  /* AGC2 Up Fast Reduce */       \
    MS222_AGC_Type_Fast_Auto_10,                                        /* AGC Type Fast Auto */        \
    /****************************************************************/							  \
    /* GSK Settings                                                 */							  \
    /****************************************************************/							  \
    MS222_AGCK_Time_Constant_32_768ms,								/* AGCK Time Constant */  \
    MS222_RSSI_HP_FC_2_25M,											/* RSSI HP FC */		  \
    \
    /****************************************************************/							  \
    /* H3H5 Settings                                                */							  \
    /****************************************************************/							  \
    MS222_VHF_III_Mode_Disabled,										/* VHF III Mode */		  \
    \
    /****************************************************************/							  \
    /*RSSI Settings                                                 */							  \
    /****************************************************************/							  \
    MS222_RSSI_CAP_VAL_3pF,											/* RSSI_Ck_speed */		  \
    MS222_RSSI_CK_SPEED_31_25kHz,									/* RSSI_Cap_Val */		  \
    \
    0x44,                                                               /* ES1 Power Saving Byte 1 */ \
    0x06,                                                               /* ES1 Power Saving Byte 2 */ \
    0x07,                                                               /* ES1 Power Saving Byte 3 */ \
    0x88,                                                               /* ES2 Power Saving Byte 1 */ \
    0x42,                                                               /* ES2 Power Saving Byte 2 */ \
    0x0B,                                                               /* ES2 Power Saving Byte 3 */ \
    0x88,                                                               /* ES3 Power Saving Byte 1 */ \
    0x42,                                                               /* ES3 Power Saving Byte 2 */ \
    0x0B,                                                               /* ES3 Power Saving Byte 3 */ \
}

#define MS222_CONFIG_STD_DVBT_7MHZ_VCO_PULLING_MINUS \
    {																		/* DVB-T/T2 7MHz */		  \
    /****************************************************************/							  \
    /* IF Settings                                                  */							  \
    /****************************************************************/							  \
    4500000-50000,															/* IF */				  \
    0,																	/* CF_Offset */			  \
    \
    /****************************************************************/							  \
    /* IF SELECTIVITY Settings                                      */							  \
    /****************************************************************/							  \
    MS222_LPF_8MHz,													/* LPF */				  \
    MS222_LPFOffset_min_8pc,										/* LPF_Offset */		  \
    MS222_DC_Notch_IF_PPF_Disabled,									/* DC notch IF PPF */	  \
    MS222_HPF_1MHz,												/* Hi Pass */			  \
    MS222_HPFOffset_0pc,											/* HPF Offset */		  \
    MS222_IF_Notch_Enabled,											/* IF notch */			  \
    MS222_IF_Notch_Freq_8_25MHz,									/* IF Notch Freq */		  \
    MS222_IF_Notch_Offset_0pc,										/* IF Notch Offset */	  \
    MS222_IFnotchToRSSI_Enabled,									/* IF notch To RSSI */    \
    \
    /****************************************************************/							  \
    /* AGC Settings                                                 */							  \
    /****************************************************************/							  \
    MS222AGC1_GAIN_Free,                                            /* AGC1 GAIN */           \
    MS222AGC1_GAIN_SMOOTH_ALGO_Enabled,                             /* AGC1 GAIN SMOOTH ALGO */ \
    {                                                                   /* AGC1 TOP DN/UP ES1 */  \
        {                                                                                         \
            (UInt32)0,                                                                            \
            MS222_AGC1_TOP_I2C_DN_UP_d97_u91dBuV                                              \
        },                                                                                        \
        {                                                                                         \
            (UInt32)100000000,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d97_u91dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        }                                                                                         \
    },                                                                                            \
    {                                                                   /* AGC1 TOP DN/UP ES2 */  \
        {                                                                                         \
            (UInt32)0,                                                                            \
            MS222_AGC1_TOP_I2C_DN_UP_d97_u91dBuV                                              \
        },                                                                                        \
        {                                                                                         \
            (UInt32)100000000,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d97_u91dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        }                                                                                         \
    },                                                                                            \
    {                                                                   /* AGC1 TOP DN/UP ES3 */  \
        {                                                                                         \
            (UInt32)0,                                                                            \
            MS222_AGC1_TOP_I2C_DN_UP_d97_u91dBuV                                              \
        },                                                                                        \
        {                                                                                         \
            (UInt32)100000000,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d97_u91dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        }                                                                                         \
    },                                                                                            \
    MS222_AGC1_TOP_STRATEGY_4,										/* AGC1 TOP STRATEGY 0 */ \
    MS222_AGC1_DET_SPEED_125KHz,									/* AGC1 Det Speed */	  \
    MS222_AGC1_SMOOTH_T_CST_1_6ms,                                  /* AGC1 Smooth T Cst */   \
    MS222_AGC1_Do_Step_0_512ms,                                      /* AGC1 Do Step  */                \
    MS222_LNA_ZIN_NF,												/* LNA_Zin */			  \
    True,                                                               /* AGC2 Gain Control En ES1 */ \
    False,                                                              /* AGC2 Gain Control En ES2 */ \
    True,                                                              /* AGC2 Gain Control En ES3 */ \
    {                                                                   /* AGC2 TOP ES1 */            \
        {                                                                                         \
            (UInt32)0,                                                                                    \
            (UInt8)99,                                                                                   \
            (UInt8)97                                                                                    \
        },                                                                                        \
        {                                                                                         \
            (UInt32)320000000,                                                                            \
            (UInt8)102,                                                                                  \
            (UInt8)100                                                                                   \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        }                                                                                         \
    },                                                                                            \
    {                                                                   /* AGC2 TOP ES2 */            \
        {                                                                                         \
            (UInt32)0,                                                                                    \
            (UInt8)100,                                                                                  \
            (UInt8)99                                                                                    \
        },                                                                                        \
        {                                                                                         \
            (UInt32)320000000,                                                                             \
            (UInt8)102,                                                                                   \
            (UInt8)101                                                                                    \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        }                                                                                         \
    },                                                                                         \
    {                                                                   /* AGC2 TOP ES3 */            \
        {                                                                                         \
            (UInt32)0,                                                                                    \
            (UInt8)97,                                                                                  \
            (UInt8)96                                                                                    \
        },                                                                                        \
        {                                                                                         \
            (UInt32)320000000,                                                                             \
            (UInt8)99,                                                                                   \
            (UInt8)98                                                                                    \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        }                                                                                         \
    },                                                                                         \
    MS222_AGC2_DET_SPEED_125KHz,									/* AGC2 Det Speed */	  \
    False,                                                              /* AGC2 Adapt TOP23 Enable ES2 */     \
    0,                                                                  /* AGC2 Adapt TOP23 Delta (in dB) ES2 */     \
    True,                                                              /* AGC2 Adapt TOP23 Enable ES3 */     \
    0,                                                                  /* AGC2 Adapt TOP23 Delta (in dB) ES3 */     \
    True,                                                               /* AGC2 Gain Control Speed (False 1ms ; True 0.5ms)   */     \
    MS222_AGC2_Do_Step_0_512ms,                                      /* AGC2 Do Step  */                \
    MS222_AGC2_Up_Step_23_8,                                         /* AGC2 Up Step  */                \
    MS222_AGC2_Up_Udld_Step_23_8,                                    /* AGC2 Up Udld Step */ \
    -8,                                                                 /* AGC2 TOP Up Udld */ \
    5,                                                                  /* AGC2 Fast Auto Delta */ \
    MS222_DET12_CINT_50fF,											/* Det12 Cint */		  \
    {                                                                   /* AGC3 TOP */            \
        {                                                                                         \
            (UInt32)0,                                                                                    \
            (UInt8)112,                                                                                  \
            (UInt8)107                                                                                   \
        },                                                                                        \
        {                                                                                         \
            (UInt32)320000000,                                                                            \
            (UInt8)110,                                                                                  \
            (UInt8)105                                                                                   \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            (UInt8)-1,                                                                                   \
            (UInt8)-1                                                                                    \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            (UInt8)-1,                                                                                   \
            (UInt8)-1                                                                                    \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            (UInt8)-1,                                                                                   \
            (UInt8)-1                                                                                    \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            (UInt8)-1,                                                                                   \
            (UInt8)-1                                                                                    \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            (UInt8)-1,                                                                                   \
            (UInt8)-1                                                                                    \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            (UInt8)-1,                                                                                   \
            (UInt8)-1                                                                                    \
        }                                                                                         \
    },                                                                                            \
    MS222_IF_Output_Level_1Vpp_min_6_24dB,							/* IF Output Level */	  \
    {                                                                   /* S2D Gain */            \
        {                                                                                         \
            (UInt32)0,                                                                                    \
            MS222_S2D_Gain_9dB                                                                \
        },                                                                                        \
        {                                                                                         \
            (UInt32)320000000,                                                                            \
            MS222_S2D_Gain_6dB                                                                \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            MS222_S2D_Gain_6dB                                                                \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            MS222_S2D_Gain_6dB                                                                \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            MS222_S2D_Gain_6dB                                                                \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            MS222_S2D_Gain_6dB                                                                \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            MS222_S2D_Gain_6dB                                                                \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            MS222_S2D_Gain_6dB                                                                \
        }                                                                                          \
    },                                                                                            \
    \
    True,                                                                   /* Fast Auto Launch */          \
    MS222_AGC_Timer_Mod_Fast_Auto_01,                                   /* AGC Timer Mod Fast Auto */   \
    False,                                                                  /* AGC2 Up Udld Fast Reduce */  \
    False,                                                                  /* AGC2 Up Fast Reduce */       \
    MS222_AGC_Type_Fast_Auto_10,                                        /* AGC Type Fast Auto */        \
    /****************************************************************/							  \
    /* GSK Settings                                                 */							  \
    /****************************************************************/							  \
    MS222_AGCK_Time_Constant_32_768ms,								/* AGCK Time Constant */  \
    MS222_RSSI_HP_FC_2_25M,											/* RSSI HP FC */		  \
    \
    /****************************************************************/							  \
    /* H3H5 Settings                                                */							  \
    /****************************************************************/							  \
    MS222_VHF_III_Mode_Disabled,										/* VHF III Mode */		  \
    \
    /****************************************************************/							  \
    /*RSSI Settings                                                 */							  \
    /****************************************************************/							  \
    MS222_RSSI_CAP_VAL_3pF,											/* RSSI_Ck_speed */		  \
    MS222_RSSI_CK_SPEED_31_25kHz,									/* RSSI_Cap_Val */		  \
    \
    0x44,                                                               /* ES1 Power Saving Byte 1 */ \
    0x06,                                                               /* ES1 Power Saving Byte 2 */ \
    0x07,                                                               /* ES1 Power Saving Byte 3 */ \
    0x88,                                                               /* ES2 Power Saving Byte 1 */ \
    0x42,                                                               /* ES2 Power Saving Byte 2 */ \
    0x0B,                                                               /* ES2 Power Saving Byte 3 */ \
    0x88,                                                               /* ES3 Power Saving Byte 1 */ \
    0x42,                                                               /* ES3 Power Saving Byte 2 */ \
    0x0B,                                                               /* ES3 Power Saving Byte 3 */ \
}

#define MS222_CONFIG_STD_DVBT_7MHZ_VCO_PULLING_PLUS \
    {																		/* DVB-T/T2 7MHz */		  \
    /****************************************************************/							  \
    /* IF Settings                                                  */							  \
    /****************************************************************/							  \
    4500000+50000,															/* IF */				  \
    0,																	/* CF_Offset */			  \
    \
    /****************************************************************/							  \
    /* IF SELECTIVITY Settings                                      */							  \
    /****************************************************************/							  \
    MS222_LPF_8MHz,													/* LPF */				  \
    MS222_LPFOffset_0pc,										/* LPF_Offset */		  \
    MS222_DC_Notch_IF_PPF_Disabled,									/* DC notch IF PPF */	  \
    MS222_HPF_1MHz,												/* Hi Pass */			  \
    MS222_HPFOffset_0pc,											/* HPF Offset */		  \
    MS222_IF_Notch_Enabled,											/* IF notch */			  \
    MS222_IF_Notch_Freq_8_25MHz,									/* IF Notch Freq */		  \
    MS222_IF_Notch_Offset_0pc,										/* IF Notch Offset */	  \
    MS222_IFnotchToRSSI_Enabled,									/* IF notch To RSSI */    \
    \
    /****************************************************************/							  \
    /* AGC Settings                                                 */							  \
    /****************************************************************/							  \
    MS222AGC1_GAIN_Free,                                            /* AGC1 GAIN */           \
    MS222AGC1_GAIN_SMOOTH_ALGO_Enabled,                             /* AGC1 GAIN SMOOTH ALGO */ \
    {                                                                   /* AGC1 TOP DN/UP ES1 */  \
        {                                                                                         \
            (UInt32)0,                                                                            \
            MS222_AGC1_TOP_I2C_DN_UP_d97_u91dBuV                                              \
        },                                                                                        \
        {                                                                                         \
            (UInt32)100000000,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d97_u91dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        }                                                                                         \
    },                                                                                            \
    {                                                                   /* AGC1 TOP DN/UP ES2 */  \
        {                                                                                         \
            (UInt32)0,                                                                            \
            MS222_AGC1_TOP_I2C_DN_UP_d97_u91dBuV                                              \
        },                                                                                        \
        {                                                                                         \
            (UInt32)100000000,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d97_u91dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        }                                                                                         \
    },                                                                                            \
    {                                                                   /* AGC1 TOP DN/UP ES3 */  \
        {                                                                                         \
            (UInt32)0,                                                                            \
            MS222_AGC1_TOP_I2C_DN_UP_d97_u91dBuV                                              \
        },                                                                                        \
        {                                                                                         \
            (UInt32)100000000,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d97_u91dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        }                                                                                         \
    },                                                                                            \
    MS222_AGC1_TOP_STRATEGY_4,										/* AGC1 TOP STRATEGY 0 */ \
    MS222_AGC1_DET_SPEED_125KHz,									/* AGC1 Det Speed */	  \
    MS222_AGC1_SMOOTH_T_CST_1_6ms,                                  /* AGC1 Smooth T Cst */   \
    MS222_AGC1_Do_Step_0_512ms,                                      /* AGC1 Do Step  */                \
    MS222_LNA_ZIN_NF,												/* LNA_Zin */			  \
    True,                                                               /* AGC2 Gain Control En ES1 */ \
    False,                                                              /* AGC2 Gain Control En ES2 */ \
    True,                                                              /* AGC2 Gain Control En ES3 */ \
    {                                                                   /* AGC2 TOP ES1 */            \
        {                                                                                         \
            (UInt32)0,                                                                                    \
            (UInt8)99,                                                                                   \
            (UInt8)97                                                                                    \
        },                                                                                        \
        {                                                                                         \
            (UInt32)320000000,                                                                            \
            (UInt8)102,                                                                                  \
            (UInt8)100                                                                                   \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        }                                                                                         \
    },                                                                                            \
    {                                                                   /* AGC2 TOP ES2 */            \
        {                                                                                         \
            (UInt32)0,                                                                                    \
            (UInt8)100,                                                                                  \
            (UInt8)99                                                                                    \
        },                                                                                        \
        {                                                                                         \
            (UInt32)320000000,                                                                             \
            (UInt8)102,                                                                                   \
            (UInt8)101                                                                                    \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        }                                                                                         \
    },                                                                                         \
    {                                                                   /* AGC2 TOP ES3 */            \
        {                                                                                         \
            (UInt32)0,                                                                                    \
            (UInt8)97,                                                                                  \
            (UInt8)96                                                                                    \
        },                                                                                        \
        {                                                                                         \
            (UInt32)320000000,                                                                             \
            (UInt8)99,                                                                                   \
            (UInt8)98                                                                                    \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        }                                                                                         \
    },                                                                                         \
    MS222_AGC2_DET_SPEED_125KHz,									/* AGC2 Det Speed */	  \
    False,                                                              /* AGC2 Adapt TOP23 Enable ES2 */     \
    0,                                                                  /* AGC2 Adapt TOP23 Delta (in dB) ES2 */     \
    True,                                                              /* AGC2 Adapt TOP23 Enable ES3 */     \
    0,                                                                  /* AGC2 Adapt TOP23 Delta (in dB) ES3 */     \
    True,                                                               /* AGC2 Gain Control Speed (False 1ms ; True 0.5ms)   */     \
    MS222_AGC2_Do_Step_0_512ms,                                      /* AGC2 Do Step  */                \
    MS222_AGC2_Up_Step_23_8,                                         /* AGC2 Up Step  */                \
    MS222_AGC2_Up_Udld_Step_23_8,                                    /* AGC2 Up Udld Step */ \
    -8,                                                                 /* AGC2 TOP Up Udld */ \
    5,                                                                  /* AGC2 Fast Auto Delta */ \
    MS222_DET12_CINT_50fF,											/* Det12 Cint */		  \
    {                                                                   /* AGC3 TOP */            \
        {                                                                                         \
            (UInt32)0,                                                                                    \
            (UInt8)112,                                                                                  \
            (UInt8)107                                                                                   \
        },                                                                                        \
        {                                                                                         \
            (UInt32)320000000,                                                                            \
            (UInt8)110,                                                                                  \
            (UInt8)105                                                                                   \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            (UInt8)-1,                                                                                   \
            (UInt8)-1                                                                                    \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            (UInt8)-1,                                                                                   \
            (UInt8)-1                                                                                    \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            (UInt8)-1,                                                                                   \
            (UInt8)-1                                                                                    \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            (UInt8)-1,                                                                                   \
            (UInt8)-1                                                                                    \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            (UInt8)-1,                                                                                   \
            (UInt8)-1                                                                                    \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            (UInt8)-1,                                                                                   \
            (UInt8)-1                                                                                    \
        }                                                                                         \
    },                                                                                            \
    MS222_IF_Output_Level_1Vpp_min_6_24dB,							/* IF Output Level */	  \
    {                                                                   /* S2D Gain */            \
        {                                                                                         \
            (UInt32)0,                                                                                    \
            MS222_S2D_Gain_9dB                                                                \
        },                                                                                        \
        {                                                                                         \
            (UInt32)320000000,                                                                            \
            MS222_S2D_Gain_6dB                                                                \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            MS222_S2D_Gain_6dB                                                                \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            MS222_S2D_Gain_6dB                                                                \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            MS222_S2D_Gain_6dB                                                                \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            MS222_S2D_Gain_6dB                                                                \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            MS222_S2D_Gain_6dB                                                                \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            MS222_S2D_Gain_6dB                                                                \
        }                                                                                          \
    },                                                                                            \
    \
    True,                                                                   /* Fast Auto Launch */          \
    MS222_AGC_Timer_Mod_Fast_Auto_01,                                   /* AGC Timer Mod Fast Auto */   \
    False,                                                                  /* AGC2 Up Udld Fast Reduce */  \
    False,                                                                  /* AGC2 Up Fast Reduce */       \
    MS222_AGC_Type_Fast_Auto_10,                                        /* AGC Type Fast Auto */        \
    /****************************************************************/							  \
    /* GSK Settings                                                 */							  \
    /****************************************************************/							  \
    MS222_AGCK_Time_Constant_32_768ms,								/* AGCK Time Constant */  \
    MS222_RSSI_HP_FC_2_25M,											/* RSSI HP FC */		  \
    \
    /****************************************************************/							  \
    /* H3H5 Settings                                                */							  \
    /****************************************************************/							  \
    MS222_VHF_III_Mode_Disabled,										/* VHF III Mode */		  \
    \
    /****************************************************************/							  \
    /*RSSI Settings                                                 */							  \
    /****************************************************************/							  \
    MS222_RSSI_CAP_VAL_3pF,											/* RSSI_Ck_speed */		  \
    MS222_RSSI_CK_SPEED_31_25kHz,									/* RSSI_Cap_Val */		  \
    \
    0x44,                                                               /* ES1 Power Saving Byte 1 */ \
    0x06,                                                               /* ES1 Power Saving Byte 2 */ \
    0x07,                                                               /* ES1 Power Saving Byte 3 */ \
    0x88,                                                               /* ES2 Power Saving Byte 1 */ \
    0x42,                                                               /* ES2 Power Saving Byte 2 */ \
    0x0B,                                                               /* ES2 Power Saving Byte 3 */ \
    0x88,                                                               /* ES3 Power Saving Byte 1 */ \
    0x42,                                                               /* ES3 Power Saving Byte 2 */ \
    0x0B,                                                               /* ES3 Power Saving Byte 3 */ \
}

#define MS222_CONFIG_STD_DVBT_8MHZ_VCO_PULLING_MINUS \
    {																		/* DVB-T/T2 8MHz */		  \
    /****************************************************************/							  \
    /* IF Settings                                                  */							  \
    /****************************************************************/							  \
    5000000,															/* IF */				  \
    0,																	/* CF_Offset */			  \
    \
    /****************************************************************/							  \
    /* IF SELECTIVITY Settings                                      */							  \
    /****************************************************************/							  \
    MS222_LPF_9MHz,													/* LPF */				  \
    MS222_LPFOffset_min_8pc,										/* LPF_Offset */		  \
    MS222_DC_Notch_IF_PPF_Disabled,									/* DC notch IF PPF */	  \
    MS222_HPF_1MHz,                                                 /* Hi Pass */			  \
    MS222_HPFOffset_0pc,											/* HPF Offset */		  \
    MS222_IF_Notch_Enabled,											/* IF notch */			  \
    MS222_IF_Notch_Freq_9_25MHz,									/* IF Notch Freq */		  \
    MS222_IF_Notch_Offset_0pc,										/* IF Notch Offset */	  \
    MS222_IFnotchToRSSI_Enabled,									/* IF notch To RSSI */	  \
    \
    /****************************************************************/							  \
    /* AGC Settings                                                 */							  \
    /****************************************************************/							  \
    MS222AGC1_GAIN_Free,                                            /* AGC1 GAIN */           \
    MS222AGC1_GAIN_SMOOTH_ALGO_Enabled,                             /* AGC1 GAIN SMOOTH ALGO */ \
    {                                                                   /* AGC1 TOP DN/UP ES1 */  \
        {                                                                                         \
            (UInt32)0,                                                                            \
            MS222_AGC1_TOP_I2C_DN_UP_d97_u91dBuV                                              \
        },                                                                                        \
        {                                                                                         \
            (UInt32)100000000,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d97_u91dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        }                                                                                         \
    },                                                                                            \
    {                                                                   /* AGC1 TOP DN/UP ES2 */  \
        {                                                                                         \
            (UInt32)0,                                                                            \
            MS222_AGC1_TOP_I2C_DN_UP_d97_u91dBuV                                              \
        },                                                                                        \
        {                                                                                         \
            (UInt32)100000000,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d97_u91dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        }                                                                                         \
    },                                                                                            \
    {                                                                   /* AGC1 TOP DN/UP ES3 */  \
        {                                                                                         \
            (UInt32)0,                                                                            \
            MS222_AGC1_TOP_I2C_DN_UP_d97_u91dBuV                                              \
        },                                                                                        \
        {                                                                                         \
            (UInt32)100000000,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d97_u91dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        }                                                                                         \
    },                                                                                            \
    MS222_AGC1_TOP_STRATEGY_4,										/* AGC1 TOP STRATEGY 0 */ \
    MS222_AGC1_DET_SPEED_125KHz,									/* AGC1 Det Speed */	  \
    MS222_AGC1_SMOOTH_T_CST_51_2ms,                                 /* AGC1 Smooth T Cst */   \
    MS222_AGC1_Do_Step_0_512ms,                                      /* AGC1 Do Step  */                \
    MS222_LNA_ZIN_NF,												/* LNA_Zin */			  \
    True,                                                               /* AGC2 Gain Control En ES1 */ \
    False,                                                              /* AGC2 Gain Control En ES2 */ \
    True,                                                              /* AGC2 Gain Control En ES3 */ \
    {                                                                   /* AGC2 TOP ES1 */            \
        {                                                                                         \
            (UInt32)0,                                                                                    \
            (UInt8)103,                                                                                  \
            (UInt8)102                                                                                    \
        },                                                                                        \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            (UInt8)-1,                                                                                   \
            (UInt8)-1                                                                                    \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        }                                                                                         \
    },                                                                                            \
    {                                                                   /* AGC2 TOP ES2 */            \
        {                                                                                         \
            (UInt32)0,                                                                                    \
            (UInt8)100,                                                                                  \
            (UInt8)99                                                                                    \
        },                                                                                        \
        {                                                                                         \
            (UInt32)320000000,                                                                             \
            (UInt8)102,                                                                                   \
            (UInt8)101                                                                                    \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        }                                                                                         \
    },                                                                                             \
    {                                                                   /* AGC2 TOP ES3 */            \
        {                                                                                         \
            (UInt32)0,                                                                                    \
            (UInt8)97,                                                                                  \
            (UInt8)96                                                                                    \
        },                                                                                        \
        {                                                                                         \
            (UInt32)320000000,                                                                             \
            (UInt8)98,                                                                                   \
            (UInt8)97                                                                                    \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        }                                                                                         \
    },                                                                                         \
    MS222_AGC2_DET_SPEED_125KHz,									/* AGC2 Det Speed */	  \
    False,                                                              /* AGC2 Adapt TOP23 Enable ES2 */     \
    7,                                                                  /* AGC2 Adapt TOP23 Delta (in dB) ES2 */     \
    True,                                                              /* AGC2 Adapt TOP23 Enable ES3 */     \
    7,                                                                  /* AGC2 Adapt TOP23 Delta (in dB) ES3 */     \
    True,                                                               /* AGC2 Gain Control Speed (False 1ms ; True 0.5ms)   */     \
    MS222_AGC2_Do_Step_0_512ms,                                      /* AGC2 Do Step  */                \
    MS222_AGC2_Up_Step_131ms,                                         /* AGC2 Up Step  */                \
    MS222_AGC2_Up_Udld_Step_23_8,                                    /* AGC2 Up Udld Step */ \
    -8,                                                                 /* AGC2 TOP Up Udld */ \
    5,                                                                  /* AGC2 Fast Auto Delta */ \
    MS222_DET12_CINT_50fF,											/* Det12 Cint */		  \
    {                                                                   /* AGC3 TOP */            \
        {                                                                                         \
            (UInt32)0,                                                                                    \
            (UInt8)112,                                                                                  \
            (UInt8)107                                                                                   \
        },                                                                                        \
        {                                                                                         \
            (UInt32)320000000,                                                                                   \
            (UInt8)110,                                                                                   \
            (UInt8)105                                                                                    \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            (UInt8)-1,                                                                                   \
            (UInt8)-1                                                                                    \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            (UInt8)-1,                                                                                   \
            (UInt8)-1                                                                                    \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            (UInt8)-1,                                                                                   \
            (UInt8)-1                                                                                    \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            (UInt8)-1,                                                                                   \
            (UInt8)-1                                                                                    \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            (UInt8)-1,                                                                                   \
            (UInt8)-1                                                                                    \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            (UInt8)-1,                                                                                   \
            (UInt8)-1                                                                                    \
        }                                                                                         \
    },                                                                                            \
    MS222_IF_Output_Level_1Vpp_min_6_24dB,							/* IF Output Level */	  \
    {                                                                   /* S2D Gain */            \
        {                                                                                         \
            (UInt32)0,                                                                                    \
            MS222_S2D_Gain_9dB                                                                \
        },                                                                                        \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            MS222_S2D_Gain_6dB                                                                \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            MS222_S2D_Gain_6dB                                                                \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            MS222_S2D_Gain_6dB                                                                \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            MS222_S2D_Gain_6dB                                                                \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            MS222_S2D_Gain_6dB                                                                \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            MS222_S2D_Gain_6dB                                                                \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            MS222_S2D_Gain_6dB                                                                \
        }                                                                                         \
    },                                                                                            \
    \
    True,                                                                   /* Fast Auto Launch */          \
    MS222_AGC_Timer_Mod_Fast_Auto_01,                                   /* AGC Timer Mod Fast Auto */   \
    False,                                                                  /* AGC2 Up Udld Fast Reduce */  \
    False,                                                                  /* AGC2 Up Fast Reduce */       \
    MS222_AGC_Type_Fast_Auto_10,                                        /* AGC Type Fast Auto */        \
    /****************************************************************/							  \
    /* GSK Settings                                                 */						      \
    /****************************************************************/							  \
    MS222_AGCK_Time_Constant_32_768ms,								/* AGCK Time Constant */  \
    MS222_RSSI_HP_FC_2_25M,											/* RSSI HP FC */		  \
    \
    /****************************************************************/							  \
    /* H3H5 Settings                                                */							  \
    /****************************************************************/							  \
    MS222_VHF_III_Mode_Disabled,										/* VHF III Mode */		  \
    \
    /****************************************************************/							  \
    /*RSSI Settings                                                 */							  \
    /****************************************************************/							  \
    MS222_RSSI_CAP_VAL_3pF,											/* RSSI_Ck_speed */		  \
    MS222_RSSI_CK_SPEED_31_25kHz,									/* RSSI_Cap_Val */		  \
    \
    0x44,                                                               /* ES1 Power Saving Byte 1 */ \
    0x06,                                                               /* ES1 Power Saving Byte 2 */ \
    0x07,                                                               /* ES1 Power Saving Byte 3 */ \
    0x88,                                                               /* ES2 Power Saving Byte 1 */ \
    0x42,                                                               /* ES2 Power Saving Byte 2 */ \
    0x0B,                                                               /* ES2 Power Saving Byte 3 */ \
    0x88,                                                               /* ES3 Power Saving Byte 1 */ \
    0x42,                                                               /* ES3 Power Saving Byte 2 */ \
    0x0B,                                                               /* ES3 Power Saving Byte 3 */ \
}

#define MS222_CONFIG_STD_DVBT_8MHZ_VCO_PULLING_PLUS \
    {																		/* DVB-T/T2 8MHz */		  \
    /****************************************************************/							  \
    /* IF Settings                                                  */							  \
    /****************************************************************/							  \
    5000000+50000,															/* IF */				  \
    0,																	/* CF_Offset */			  \
    \
    /****************************************************************/							  \
    /* IF SELECTIVITY Settings                                      */							  \
    /****************************************************************/							  \
    MS222_LPF_9MHz,													/* LPF */				  \
    MS222_LPFOffset_0pc,    										/* LPF_Offset */		  \
    MS222_DC_Notch_IF_PPF_Disabled,									/* DC notch IF PPF */	  \
    MS222_HPF_1MHz, 												/* Hi Pass */			  \
    MS222_HPFOffset_0pc,											/* HPF Offset */		  \
    MS222_IF_Notch_Enabled,											/* IF notch */			  \
    MS222_IF_Notch_Freq_9_25MHz,									/* IF Notch Freq */		  \
    MS222_IF_Notch_Offset_0pc,										/* IF Notch Offset */	  \
    MS222_IFnotchToRSSI_Enabled,									/* IF notch To RSSI */	  \
    \
    /****************************************************************/							  \
    /* AGC Settings                                                 */							  \
    /****************************************************************/							  \
    MS222AGC1_GAIN_Free,                                            /* AGC1 GAIN */           \
    MS222AGC1_GAIN_SMOOTH_ALGO_Enabled,                             /* AGC1 GAIN SMOOTH ALGO */ \
    {                                                                   /* AGC1 TOP DN/UP ES1 */  \
        {                                                                                         \
            (UInt32)0,                                                                            \
            MS222_AGC1_TOP_I2C_DN_UP_d97_u91dBuV                                              \
        },                                                                                        \
        {                                                                                         \
            (UInt32)100000000,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d97_u91dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        }                                                                                         \
    },                                                                                            \
    {                                                                   /* AGC1 TOP DN/UP ES2 */  \
        {                                                                                         \
            (UInt32)0,                                                                            \
            MS222_AGC1_TOP_I2C_DN_UP_d97_u91dBuV                                              \
        },                                                                                        \
        {                                                                                         \
            (UInt32)100000000,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d97_u91dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        }                                                                                         \
    },                                                                                            \
    {                                                                   /* AGC1 TOP DN/UP ES3 */  \
        {                                                                                         \
            (UInt32)0,                                                                            \
            MS222_AGC1_TOP_I2C_DN_UP_d97_u91dBuV                                              \
        },                                                                                        \
        {                                                                                         \
            (UInt32)100000000,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d97_u91dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        }                                                                                         \
    },                                                                                            \
    MS222_AGC1_TOP_STRATEGY_4,										/* AGC1 TOP STRATEGY 0 */ \
    MS222_AGC1_DET_SPEED_125KHz,									/* AGC1 Det Speed */	  \
    MS222_AGC1_SMOOTH_T_CST_51_2ms,                                 /* AGC1 Smooth T Cst */   \
    MS222_AGC1_Do_Step_0_512ms,                                      /* AGC1 Do Step  */                \
    MS222_LNA_ZIN_NF,												/* LNA_Zin */			  \
    True,                                                               /* AGC2 Gain Control En ES1 */ \
    False,                                                              /* AGC2 Gain Control En ES2 */ \
    True,                                                              /* AGC2 Gain Control En ES3 */ \
    {                                                                   /* AGC2 TOP ES1 */            \
        {                                                                                         \
            (UInt32)0,                                                                                    \
            (UInt8)103,                                                                                  \
            (UInt8)102                                                                                    \
        },                                                                                        \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            (UInt8)-1,                                                                                   \
            (UInt8)-1                                                                                    \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        }                                                                                         \
    },                                                                                            \
    {                                                                   /* AGC2 TOP ES2 */            \
        {                                                                                         \
            (UInt32)0,                                                                                    \
            (UInt8)100,                                                                                  \
            (UInt8)99                                                                                    \
        },                                                                                        \
        {                                                                                         \
            (UInt32)320000000,                                                                             \
            (UInt8)102,                                                                                   \
            (UInt8)101                                                                                    \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        }                                                                                         \
    },                                                                                             \
    {                                                                   /* AGC2 TOP ES3 */            \
        {                                                                                         \
            (UInt32)0,                                                                                    \
            (UInt8)97,                                                                                  \
            (UInt8)96                                                                                    \
        },                                                                                        \
        {                                                                                         \
            (UInt32)320000000,                                                                             \
            (UInt8)98,                                                                                   \
            (UInt8)97                                                                                    \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        }                                                                                         \
    },                                                                                         \
    MS222_AGC2_DET_SPEED_125KHz,									/* AGC2 Det Speed */	  \
    False,                                                              /* AGC2 Adapt TOP23 Enable ES2 */     \
    7,                                                                  /* AGC2 Adapt TOP23 Delta (in dB) ES2 */     \
    True,                                                              /* AGC2 Adapt TOP23 Enable ES3 */     \
    7,                                                                  /* AGC2 Adapt TOP23 Delta (in dB) ES3 */     \
    True,                                                               /* AGC2 Gain Control Speed (False 1ms ; True 0.5ms)   */     \
    MS222_AGC2_Do_Step_0_512ms,                                      /* AGC2 Do Step  */                \
    MS222_AGC2_Up_Step_131ms,                                         /* AGC2 Up Step  */                \
    MS222_AGC2_Up_Udld_Step_23_8,                                    /* AGC2 Up Udld Step */ \
    -8,                                                                 /* AGC2 TOP Up Udld */ \
    5,                                                                  /* AGC2 Fast Auto Delta */ \
    MS222_DET12_CINT_50fF,											/* Det12 Cint */		  \
    {                                                                   /* AGC3 TOP */            \
        {                                                                                         \
            (UInt32)0,                                                                                    \
            (UInt8)112,                                                                                  \
            (UInt8)107                                                                                   \
        },                                                                                        \
        {                                                                                         \
            (UInt32)320000000,                                                                                   \
            (UInt8)110,                                                                                   \
            (UInt8)105                                                                                    \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            (UInt8)-1,                                                                                   \
            (UInt8)-1                                                                                    \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            (UInt8)-1,                                                                                   \
            (UInt8)-1                                                                                    \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            (UInt8)-1,                                                                                   \
            (UInt8)-1                                                                                    \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            (UInt8)-1,                                                                                   \
            (UInt8)-1                                                                                    \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            (UInt8)-1,                                                                                   \
            (UInt8)-1                                                                                    \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            (UInt8)-1,                                                                                   \
            (UInt8)-1                                                                                    \
        }                                                                                         \
    },                                                                                            \
    MS222_IF_Output_Level_1Vpp_min_6_24dB,							/* IF Output Level */	  \
    {                                                                   /* S2D Gain */            \
        {                                                                                         \
            (UInt32)0,                                                                                    \
            MS222_S2D_Gain_9dB                                                                \
        },                                                                                        \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            MS222_S2D_Gain_6dB                                                                \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            MS222_S2D_Gain_6dB                                                                \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            MS222_S2D_Gain_6dB                                                                \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            MS222_S2D_Gain_6dB                                                                \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            MS222_S2D_Gain_6dB                                                                \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            MS222_S2D_Gain_6dB                                                                \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            MS222_S2D_Gain_6dB                                                                \
        }                                                                                         \
    },                                                                                            \
    \
    True,                                                                   /* Fast Auto Launch */          \
    MS222_AGC_Timer_Mod_Fast_Auto_01,                                   /* AGC Timer Mod Fast Auto */   \
    False,                                                                  /* AGC2 Up Udld Fast Reduce */  \
    False,                                                                  /* AGC2 Up Fast Reduce */       \
    MS222_AGC_Type_Fast_Auto_10,                                        /* AGC Type Fast Auto */        \
    /****************************************************************/							  \
    /* GSK Settings                                                 */						      \
    /****************************************************************/							  \
    MS222_AGCK_Time_Constant_32_768ms,								/* AGCK Time Constant */  \
    MS222_RSSI_HP_FC_2_25M,											/* RSSI HP FC */		  \
    \
    /****************************************************************/							  \
    /* H3H5 Settings                                                */							  \
    /****************************************************************/							  \
    MS222_VHF_III_Mode_Disabled,										/* VHF III Mode */		  \
    \
    /****************************************************************/							  \
    /*RSSI Settings                                                 */							  \
    /****************************************************************/							  \
    MS222_RSSI_CAP_VAL_3pF,											/* RSSI_Ck_speed */		  \
    MS222_RSSI_CK_SPEED_31_25kHz,									/* RSSI_Cap_Val */		  \
    \
    0x44,                                                               /* ES1 Power Saving Byte 1 */ \
    0x06,                                                               /* ES1 Power Saving Byte 2 */ \
    0x07,                                                               /* ES1 Power Saving Byte 3 */ \
    0x88,                                                               /* ES2 Power Saving Byte 1 */ \
    0x42,                                                               /* ES2 Power Saving Byte 2 */ \
    0x0B,                                                               /* ES2 Power Saving Byte 3 */ \
    0x88,                                                               /* ES3 Power Saving Byte 1 */ \
    0x42,                                                               /* ES3 Power Saving Byte 2 */ \
    0x0B,                                                               /* ES3 Power Saving Byte 3 */ \
}

#define MS222_CONFIG_STD_DVBT_10MHZ_VCO_PULLING_MINUS \
    {																		/* DVB-T/T2 10MHz */	  \
    /****************************************************************/							  \
    /* IF Settings                                                  */							  \
    /****************************************************************/							  \
    6000000-50000,															/* IF */                  \
    0,																	/* CF_Offset */			  \
    \
    /****************************************************************/							  \
    /* IF SELECTIVITY Settings                                      */							  \
    /****************************************************************/							  \
    MS222_LPF_11_2MHz,												/* LPF */				  \
    MS222_LPFOffset_0pc,										/* LPF_Offset */		  \
    MS222_DC_Notch_IF_PPF_Enabled,									/* DC notch IF PPF */	  \
    MS222_HPF_1MHz,												/* Hi Pass */			  \
    MS222_HPFOffset_0pc,											/* HPF Offset */		  \
    MS222_IF_Notch_Enabled,											/* IF notch */			  \
    MS222_IF_Notch_Freq_11_45MHz,									/* IF Notch Freq */		  \
    MS222_IF_Notch_Offset_0pc,										/* IF Notch Offset */	  \
    MS222_IFnotchToRSSI_Enabled,									/* IF notch To RSSI */	  \
    \
    /****************************************************************/							  \
    /* AGC Settings                                                 */							  \
    /****************************************************************/							  \
    MS222AGC1_GAIN_Free,                                            /* AGC1 GAIN */           \
    MS222AGC1_GAIN_SMOOTH_ALGO_Enabled,                             /* AGC1 GAIN SMOOTH ALGO */ \
    {                                                                   /* AGC1 TOP DN/UP ES1 */  \
        {                                                                                         \
            (UInt32)0,                                                                            \
            MS222_AGC1_TOP_I2C_DN_UP_d97_u91dBuV                                              \
        },                                                                                        \
        {                                                                                         \
            (UInt32)100000000,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d97_u91dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        }                                                                                         \
    },                                                                                            \
    {                                                                   /* AGC1 TOP DN/UP ES2 */  \
        {                                                                                         \
            (UInt32)0,                                                                            \
            MS222_AGC1_TOP_I2C_DN_UP_d97_u91dBuV                                              \
        },                                                                                        \
        {                                                                                         \
            (UInt32)100000000,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d97_u91dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        }                                                                                         \
    },                                                                                            \
    {                                                                   /* AGC1 TOP DN/UP ES3 */  \
        {                                                                                         \
            (UInt32)0,                                                                            \
            MS222_AGC1_TOP_I2C_DN_UP_d97_u91dBuV                                              \
        },                                                                                        \
        {                                                                                         \
            (UInt32)100000000,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d97_u91dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        }                                                                                         \
    },                                                                                            \
    MS222_AGC1_TOP_STRATEGY_4,										/* AGC1 TOP STRATEGY 0 */ \
    MS222_AGC1_DET_SPEED_125KHz,									/* AGC1 Det Speed */	  \
    MS222_AGC1_SMOOTH_T_CST_1_6ms,                                  /* AGC1 Smooth T Cst */   \
    MS222_AGC1_Do_Step_0_512ms,                                      /* AGC1 Do Step  */                \
    MS222_LNA_ZIN_NF,												/* LNA_Zin */			  \
    True,                                                               /* AGC2 Gain Control En ES1 */ \
    False,                                                              /* AGC2 Gain Control En ES2 */ \
    True,                                                              /* AGC2 Gain Control En ES3 */ \
    {                                                                   /* AGC2 TOP ES1 */            \
        {                                                                                         \
            (UInt32)0,                                                                                    \
            (UInt8)99,                                                                                   \
            (UInt8)97                                                                                    \
        },                                                                                        \
        {                                                                                         \
            (UInt32)320000000,                                                                            \
            (UInt8)102,                                                                                  \
            (UInt8)100                                                                                   \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        }                                                                                         \
    },                                                                                            \
    {                                                                   /* AGC2 TOP ES2 */            \
        {                                                                                         \
            (UInt32)0,                                                                                    \
            (UInt8)103,                                                                                   \
            (UInt8)102                                                                                    \
        },                                                                                        \
        {                                                                                         \
            (UInt32)320000000,                                                                            \
            (UInt8)106,                                                                                  \
            (UInt8)105                                                                                   \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        }                                                                                         \
    },                                                                                         \
    {                                                                   /* AGC2 TOP ES3 */            \
        {                                                                                         \
            (UInt32)0,                                                                                    \
            (UInt8)97,                                                                                   \
            (UInt8)96                                                                                    \
        },                                                                                        \
        {                                                                                         \
            (UInt32)320000000,                                                                            \
            (UInt8)99,                                                                                  \
            (UInt8)98                                                                                  \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        }                                                                                         \
    },                                                                                         \
    MS222_AGC2_DET_SPEED_125KHz,									/* AGC2 Det Speed */	  \
    False,                                                              /* AGC2 Adapt TOP23 Enable ES2 */     \
    0,                                                                  /* AGC2 Adapt TOP23 Delta (in dB) ES2 */     \
    True,                                                              /* AGC2 Adapt TOP23 Enable ES3 */     \
    0,                                                                  /* AGC2 Adapt TOP23 Delta (in dB) ES3 */     \
    True,                                                               /* AGC2 Gain Control Speed (False 1ms ; True 0.5ms)   */     \
    MS222_AGC2_Do_Step_0_512ms,                                      /* AGC2 Do Step  */                \
    MS222_AGC2_Up_Step_23_8,                                         /* AGC2 Up Step  */                \
    MS222_AGC2_Up_Udld_Step_23_8,                                    /* AGC2 Up Udld Step */ \
    -8,                                                                 /* AGC2 TOP Up Udld */ \
    5,                                                                  /* AGC2 Fast Auto Delta */ \
    MS222_DET12_CINT_50fF,											/* Det12 Cint */		  \
    {                                                                   /* AGC3 TOP */            \
        {                                                                                         \
            (UInt32)0,                                                                                    \
            (UInt8)112,                                                                                  \
            (UInt8)107                                                                                   \
        },                                                                                        \
        {                                                                                         \
            (UInt32)320000000,                                                                            \
            (UInt8)110,                                                                                  \
            (UInt8)105                                                                                   \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            (UInt8)-1,                                                                                   \
            (UInt8)-1                                                                                    \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            (UInt8)-1,                                                                                   \
            (UInt8)-1                                                                                    \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            (UInt8)-1,                                                                                   \
            (UInt8)-1                                                                                    \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            (UInt8)-1,                                                                                   \
            (UInt8)-1                                                                                    \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            (UInt8)-1,                                                                                   \
            (UInt8)-1                                                                                    \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            (UInt8)-1,                                                                                   \
            (UInt8)-1                                                                                    \
        }                                                                                         \
    },                                                                                            \
    MS222_IF_Output_Level_1Vpp_min_6_24dB,							/* IF Output Level */	  \
    {                                                                   /* S2D Gain */            \
        {                                                                                         \
            (UInt32)0,                                                                                    \
            MS222_S2D_Gain_9dB                                                                \
        },                                                                                        \
        {                                                                                         \
            (UInt32)320000000,                                                                            \
            MS222_S2D_Gain_6dB                                                                \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            MS222_S2D_Gain_6dB                                                                \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            MS222_S2D_Gain_6dB                                                                \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            MS222_S2D_Gain_6dB                                                                \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            MS222_S2D_Gain_6dB                                                                \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            MS222_S2D_Gain_6dB                                                                \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            MS222_S2D_Gain_6dB                                                                \
        }                                                                                          \
    },                                                                                            \
    \
    True,                                                                   /* Fast Auto Launch */          \
    MS222_AGC_Timer_Mod_Fast_Auto_01,                                   /* AGC Timer Mod Fast Auto */   \
    False,                                                                  /* AGC2 Up Udld Fast Reduce */  \
    False,                                                                  /* AGC2 Up Fast Reduce */       \
    MS222_AGC_Type_Fast_Auto_10,                                        /* AGC Type Fast Auto */        \
    /****************************************************************/							  \
    /* GSK Settings                                                 */							  \
    /****************************************************************/							  \
    MS222_AGCK_Time_Constant_32_768ms,								/* AGCK Time Constant */  \
    MS222_RSSI_HP_FC_2_25M,											/* RSSI HP FC */		  \
    \
    /****************************************************************/							  \
    /* H3H5 Settings                                                */							  \
    /****************************************************************/							  \
    MS222_VHF_III_Mode_Disabled,										/* VHF III Mode */		  \
    \
    /****************************************************************/							  \
    /*RSSI Settings                                                 */							  \
    /****************************************************************/							  \
    MS222_RSSI_CAP_VAL_3pF,											/* RSSI_Ck_speed */		  \
    MS222_RSSI_CK_SPEED_31_25kHz,									/* RSSI_Cap_Val */		  \
    \
    0x44,                                                               /* ES1 Power Saving Byte 1 */ \
    0x06,                                                               /* ES1 Power Saving Byte 2 */ \
    0x07,                                                               /* ES1 Power Saving Byte 3 */ \
    0x88,                                                               /* ES2 Power Saving Byte 1 */ \
    0x42,                                                               /* ES2 Power Saving Byte 2 */ \
    0x0B,                                                               /* ES2 Power Saving Byte 3 */ \
    0x88,                                                               /* ES3 Power Saving Byte 1 */ \
    0x42,                                                               /* ES3 Power Saving Byte 2 */ \
    0x0B,                                                               /* ES3 Power Saving Byte 3 */ \
}

#define MS222_CONFIG_STD_DVBT_10MHZ_VCO_PULLING_PLUS \
    {																		/* DVB-T/T2 10MHz */	  \
    /****************************************************************/							  \
    /* IF Settings                                                  */							  \
    /****************************************************************/							  \
    6000000+50000,															/* IF */                  \
    0,																	/* CF_Offset */			  \
    \
    /****************************************************************/							  \
    /* IF SELECTIVITY Settings                                      */							  \
    /****************************************************************/							  \
    MS222_LPF_11_2MHz,												/* LPF */				  \
    MS222_LPFOffset_0pc,										/* LPF_Offset */		  \
    MS222_DC_Notch_IF_PPF_Enabled,									/* DC notch IF PPF */	  \
    MS222_HPF_1MHz,												/* Hi Pass */			  \
    MS222_HPFOffset_0pc,											/* HPF Offset */		  \
    MS222_IF_Notch_Enabled,											/* IF notch */			  \
    MS222_IF_Notch_Freq_11_45MHz,									/* IF Notch Freq */		  \
    MS222_IF_Notch_Offset_0pc,										/* IF Notch Offset */	  \
    MS222_IFnotchToRSSI_Enabled,									/* IF notch To RSSI */	  \
    \
    /****************************************************************/							  \
    /* AGC Settings                                                 */							  \
    /****************************************************************/							  \
    MS222AGC1_GAIN_Free,                                            /* AGC1 GAIN */           \
    MS222AGC1_GAIN_SMOOTH_ALGO_Enabled,                             /* AGC1 GAIN SMOOTH ALGO */ \
    {                                                                   /* AGC1 TOP DN/UP ES1 */  \
        {                                                                                         \
            (UInt32)0,                                                                            \
            MS222_AGC1_TOP_I2C_DN_UP_d97_u91dBuV                                              \
        },                                                                                        \
        {                                                                                         \
            (UInt32)100000000,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d97_u91dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        }                                                                                         \
    },                                                                                            \
    {                                                                   /* AGC1 TOP DN/UP ES2 */  \
        {                                                                                         \
            (UInt32)0,                                                                            \
            MS222_AGC1_TOP_I2C_DN_UP_d97_u91dBuV                                              \
        },                                                                                        \
        {                                                                                         \
            (UInt32)100000000,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d97_u91dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        }                                                                                         \
    },                                                                                            \
    {                                                                   /* AGC1 TOP DN/UP ES3 */  \
        {                                                                                         \
            (UInt32)0,                                                                            \
            MS222_AGC1_TOP_I2C_DN_UP_d97_u91dBuV                                              \
        },                                                                                        \
        {                                                                                         \
            (UInt32)100000000,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d97_u91dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            MS222_AGC1_TOP_I2C_DN_UP_d94_u88dBuV                                              \
        }                                                                                         \
    },                                                                                            \
    MS222_AGC1_TOP_STRATEGY_4,										/* AGC1 TOP STRATEGY 0 */ \
    MS222_AGC1_DET_SPEED_125KHz,									/* AGC1 Det Speed */	  \
    MS222_AGC1_SMOOTH_T_CST_1_6ms,                                  /* AGC1 Smooth T Cst */   \
    MS222_AGC1_Do_Step_0_512ms,                                      /* AGC1 Do Step  */                \
    MS222_LNA_ZIN_NF,												/* LNA_Zin */			  \
    True,                                                               /* AGC2 Gain Control En ES1 */ \
    False,                                                              /* AGC2 Gain Control En ES2 */ \
    True,                                                              /* AGC2 Gain Control En ES3 */ \
    {                                                                   /* AGC2 TOP ES1 */            \
        {                                                                                         \
            (UInt32)0,                                                                                    \
            (UInt8)99,                                                                                   \
            (UInt8)97                                                                                    \
        },                                                                                        \
        {                                                                                         \
            (UInt32)320000000,                                                                            \
            (UInt8)102,                                                                                  \
            (UInt8)100                                                                                   \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        }                                                                                         \
    },                                                                                            \
    {                                                                   /* AGC2 TOP ES2 */            \
        {                                                                                         \
            (UInt32)0,                                                                                    \
            (UInt8)103,                                                                                   \
            (UInt8)102                                                                                    \
        },                                                                                        \
        {                                                                                         \
            (UInt32)320000000,                                                                            \
            (UInt8)106,                                                                                  \
            (UInt8)105                                                                                   \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        }                                                                                         \
    },                                                                                         \
    {                                                                   /* AGC2 TOP ES3 */            \
        {                                                                                         \
            (UInt32)0,                                                                                    \
            (UInt8)97,                                                                                   \
            (UInt8)96                                                                                    \
        },                                                                                        \
        {                                                                                         \
            (UInt32)320000000,                                                                            \
            (UInt8)99,                                                                                  \
            (UInt8)98                                                                                  \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                    \
            (UInt8)99,                                                                            \
            (UInt8)97                                                                             \
        }                                                                                         \
    },                                                                                         \
    MS222_AGC2_DET_SPEED_125KHz,									/* AGC2 Det Speed */	  \
    False,                                                              /* AGC2 Adapt TOP23 Enable ES2 */     \
    0,                                                                  /* AGC2 Adapt TOP23 Delta (in dB) ES2 */     \
    True,                                                              /* AGC2 Adapt TOP23 Enable ES3 */     \
    0,                                                                  /* AGC2 Adapt TOP23 Delta (in dB) ES3 */     \
    True,                                                               /* AGC2 Gain Control Speed (False 1ms ; True 0.5ms)   */     \
    MS222_AGC2_Do_Step_0_512ms,                                      /* AGC2 Do Step  */                \
    MS222_AGC2_Up_Step_23_8,                                         /* AGC2 Up Step  */                \
    MS222_AGC2_Up_Udld_Step_23_8,                                    /* AGC2 Up Udld Step */ \
    -8,                                                                 /* AGC2 TOP Up Udld */ \
    5,                                                                  /* AGC2 Fast Auto Delta */ \
    MS222_DET12_CINT_50fF,											/* Det12 Cint */		  \
    {                                                                   /* AGC3 TOP */            \
        {                                                                                         \
            (UInt32)0,                                                                                    \
            (UInt8)112,                                                                                  \
            (UInt8)107                                                                                   \
        },                                                                                        \
        {                                                                                         \
            (UInt32)320000000,                                                                            \
            (UInt8)110,                                                                                  \
            (UInt8)105                                                                                   \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            (UInt8)-1,                                                                                   \
            (UInt8)-1                                                                                    \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            (UInt8)-1,                                                                                   \
            (UInt8)-1                                                                                    \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            (UInt8)-1,                                                                                   \
            (UInt8)-1                                                                                    \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            (UInt8)-1,                                                                                   \
            (UInt8)-1                                                                                    \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            (UInt8)-1,                                                                                   \
            (UInt8)-1                                                                                    \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            (UInt8)-1,                                                                                   \
            (UInt8)-1                                                                                    \
        }                                                                                         \
    },                                                                                            \
    MS222_IF_Output_Level_1Vpp_min_6_24dB,							/* IF Output Level */	  \
    {                                                                   /* S2D Gain */            \
        {                                                                                         \
            (UInt32)0,                                                                                    \
            MS222_S2D_Gain_9dB                                                                \
        },                                                                                        \
        {                                                                                         \
            (UInt32)320000000,                                                                            \
            MS222_S2D_Gain_6dB                                                                \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            MS222_S2D_Gain_6dB                                                                \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            MS222_S2D_Gain_6dB                                                                \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            MS222_S2D_Gain_6dB                                                                \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            MS222_S2D_Gain_6dB                                                                \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            MS222_S2D_Gain_6dB                                                                \
        },                                                                                         \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            MS222_S2D_Gain_6dB                                                                \
        }                                                                                          \
    },                                                                                            \
    \
    True,                                                                   /* Fast Auto Launch */          \
    MS222_AGC_Timer_Mod_Fast_Auto_01,                                   /* AGC Timer Mod Fast Auto */   \
    False,                                                                  /* AGC2 Up Udld Fast Reduce */  \
    False,                                                                  /* AGC2 Up Fast Reduce */       \
    MS222_AGC_Type_Fast_Auto_10,                                        /* AGC Type Fast Auto */        \
    /****************************************************************/							  \
    /* GSK Settings                                                 */							  \
    /****************************************************************/							  \
    MS222_AGCK_Time_Constant_32_768ms,								/* AGCK Time Constant */  \
    MS222_RSSI_HP_FC_2_25M,											/* RSSI HP FC */		  \
    \
    /****************************************************************/							  \
    /* H3H5 Settings                                                */							  \
    /****************************************************************/							  \
    MS222_VHF_III_Mode_Disabled,										/* VHF III Mode */		  \
    \
    /****************************************************************/							  \
    /*RSSI Settings                                                 */							  \
    /****************************************************************/							  \
    MS222_RSSI_CAP_VAL_3pF,											/* RSSI_Ck_speed */		  \
    MS222_RSSI_CK_SPEED_31_25kHz,									/* RSSI_Cap_Val */		  \
    \
    0x44,                                                               /* ES1 Power Saving Byte 1 */ \
    0x06,                                                               /* ES1 Power Saving Byte 2 */ \
    0x07,                                                               /* ES1 Power Saving Byte 3 */ \
    0x88,                                                               /* ES2 Power Saving Byte 1 */ \
    0x42,                                                               /* ES2 Power Saving Byte 2 */ \
    0x0B,                                                               /* ES2 Power Saving Byte 3 */ \
    0x88,                                                               /* ES3 Power Saving Byte 1 */ \
    0x42,                                                               /* ES3 Power Saving Byte 2 */ \
    0x0B,                                                               /* ES3 Power Saving Byte 3 */ \
}

#ifdef __cplusplus
}
#endif

#endif /* _MS222_CONFIG_DVBT_H */

