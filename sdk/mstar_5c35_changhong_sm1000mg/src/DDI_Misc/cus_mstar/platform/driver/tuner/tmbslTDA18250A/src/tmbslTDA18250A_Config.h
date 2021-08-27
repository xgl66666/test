/*
Copyright (C) 2010 NXP B.V., All Rights Reserved.
This source code and any compilation or derivative thereof is the proprietary
information of NXP B.V. and is confidential in nature. Under no circumstances
is this software to be  exposed to or placed under an Open Source License of
any type without the expressed written permission of NXP B.V.
*
* \file          tmbslTDA18250A_Config.h
*
* \date          %modify_time%
*
* \author        Alexandre TANT
*
* \brief         Describe briefly the purpose of this file.
*
* REFERENCE DOCUMENTS :
*                TDA18250A_Driver_User_Guide.pdf
*
* \section info Change Information
*
*/

#ifndef _TDA18250A_CONFIG_H
#define _TDA18250A_CONFIG_H


#ifdef __cplusplus
extern "C"
{
#endif

    /*============================================================================*/
    /* Types and defines:                                                         */
    /*============================================================================*/

    /* Driver settings version definition */
#define TDA18250A_SETTINGS_CUSTOMER_NUM      (0)                     /* SW Settings Customer Number */
#define TDA18250A_SETTINGS_PROJECT_NUM       (0)                     /* SW Settings Project Number  */
#define TDA18250A_SETTINGS_MAJOR_VER         (0)                     /* SW Settings Major Version   */
#define TDA18250A_SETTINGS_MINOR_VER         (20)                     /* SW Settings Minor Version   */

    /* Standard Preset Definitions: */
#define TDA18250A_CONFIG_STD_QAM_6MHZ \
    {                                                                        /* QAM 6MHz */            \
    /****************************************************************/                            \
    /* IF Settings                                                  */                            \
    /****************************************************************/                            \
    5000000,                                                            /* IF */                  \
    0,                                                                  /* CF_Offset */           \
    \
    /****************************************************************/                            \
    /* IF SELECTIVITY Settings                                      */                            \
    /****************************************************************/                            \
    TDA18250A_LPF_9MHz,                                                 /* LPF */                 \
    TDA18250A_LPFOffset_0pcbis,                                         /* LPF_Offset */          \
    TDA18250A_DC_Notch_IF_PPF_Disabled,                                 /* DC notch IF PPF */     \
    TDA18250A_HPF_1_6MHz,                                               /* Hi Pass */             \
    TDA18250A_HPFOffset_0pc,                                            /* HPF Offset */          \
    TDA18250A_IF_Notch_Disabled,                                        /* IF notch */            \
    TDA18250A_IF_Notch_Freq_6_25MHz,                                    /* IF Notch Freq */       \
    TDA18250A_IF_Notch_Offset_0pc,                                      /* IF Notch Offset */     \
    TDA18250A_IFnotchToRSSI_Enabled,                                    /* IF notch To RSSI */    \
    \
    /****************************************************************/                            \
    /* AGC Settings                                                 */                            \
    /****************************************************************/                            \
    TDA18250AAGC1_GAIN_Free,                                            /* AGC1 GAIN */           \
    TDA18250A_AGC1_TOP_I2C_DN_UP_d102_u96dBuV,                           /* AGC1 TOP I2C DN/UP */  \
    TDA18250A_AGC1_TOP_STRATEGY_0,                                      /* AGC1 TOP STRATEGY 0 */ \
    TDA18250A_AGC1_DET_SPEED_62_5KHz,                                   /* AGC1 Det Speed */      \
    TDA18250A_AGC1_SMOOTH_T_CST_51_2ms,                                 /* AGC1 Smooth T Cst */   \
    TDA18250A_LNA_ZIN_S11,                                              /* LNA_Zin */             \
    {                                                                   /* AGC2 TOP */            \
        {                                                                                         \
            (UInt32)0,                                                                                    \
            (UInt8)98,                                                                                  \
            (UInt8)96                                                                                   \
        },                                                                                        \
        {                                                                                         \
            (UInt32)600000000,                                                                                   \
            (UInt8)99,                                                                                   \
            (UInt8)97                                                                                    \
        }                                                                                          \
    },                                                                                            \
    TDA18250A_AGC2_DET_SPEED_62_5KHz,                                   /* AGC2 Det Speed */      \
    TDA18250A_DET12_CINT_200fF,                                         /* Det12 Cint */          \
    {                                                                   /* AGC3 TOP */            \
        {                                                                                         \
            (UInt32)0,                                                                                    \
            (UInt8)110,                                                                                  \
            (UInt8)105                                                                                   \
        },                                                                                        \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            (UInt8)-1,                                                                                   \
            (UInt8)-1                                                                                    \
        }                                                                                         \
    },                                                                                            \
    TDA18250A_IF_Output_Level_1Vpp_min_6_24dB,                          /* IF Output Level */     \
    {                                                                   /* S2D Gain */            \
        {                                                                                         \
            (UInt32)0,                                                                                    \
            TDA18250A_S2D_Gain_6dB                                                                \
        },                                                                                        \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            TDA18250A_S2D_Gain_6dB                                                                \
        }                                                                                         \
    },                                                                                            \
    \
    /****************************************************************/                            \
    /* GSK Settings                                                 */                            \
    /****************************************************************/                            \
    TDA18250A_AGCK_Time_Constant_32_768ms,                              /* AGCK Time Constant */  \
    TDA18250A_RSSI_HP_FC_0_3M,                                          /* RSSI HP FC */          \
    \
    /****************************************************************/                            \
    /* H3H5 Settings                                                */                            \
    /****************************************************************/                            \
    TDA18250A_VHF_III_Mode_Disabled,                                    /* VHF III Mode */        \
    \
    /****************************************************************/                            \
    /*RSSI Settings                                                 */                            \
    /****************************************************************/                            \
    TDA18250A_RSSI_CAP_VAL_3pF,                                         /* RSSI_Ck_speed */       \
    TDA18250A_RSSI_CK_SPEED_31_25kHz,                                   /* RSSI_Cap_Val */        \
    \
    TDA18250APowerSavingMode_Normal,                                    /* Normal or Low */       \
    0x75,                                                               /* Nominal Power Saving Byte 1 */ \
    0x06,                                                               /* Nominal Power Saving Byte 2 */ \
    0x07,                                                               /* Nominal Power Saving Byte 3 */ \
    0xBE,                                                               /* Low Power Saving Byte 1 */ \
    0xA2,                                                               /* Low Power Saving Byte 2 */ \
    0x4A,                                                               /* Low Power Saving Byte 3 */ \
}

#define TDA18250A_CONFIG_STD_QAM_8MHZ \
    {																		/* QAM 8MHz */			  \
    /****************************************************************/							  \
    /* IF Settings                                                  */							  \
    /****************************************************************/							  \
    5000000,															/* IF */				  \
    0,																	/* CF_Offset */			  \
    \
    /****************************************************************/							  \
    /* IF SELECTIVITY Settings                                      */							  \
    /****************************************************************/							  \
    TDA18250A_LPF_9MHz,													/* LPF */				  \
    TDA18250A_LPFOffset_min_8pc,											/* LPF_Offset */		  \
    TDA18250A_DC_Notch_IF_PPF_Disabled,                                 /* DC notch IF PPF */     \
    TDA18250A_HPF_1MHz,                                               /* Hi Pass */			  \
    TDA18250A_HPFOffset_plus_12pc,											/* HPF Offset */		  \
    TDA18250A_IF_Notch_Disabled,										/* IF notch */			  \
    TDA18250A_IF_Notch_Freq_6_25MHz,									/* IF Notch Freq */		  \
    TDA18250A_IF_Notch_Offset_0pc,										/* IF Notch Offset */     \
    TDA18250A_IFnotchToRSSI_Disabled,									/* IF notch To RSSI */    \
    \
    /****************************************************************/							  \
    /* AGC Settings                                                 */							  \
    /****************************************************************/							  \
    TDA18250AAGC1_GAIN_Free,                                            /* AGC1 GAIN */           \
    TDA18250A_AGC1_TOP_I2C_DN_UP_d102_u96dBuV,							/* AGC1 TOP I2C DN/UP */  \
    TDA18250A_AGC1_TOP_STRATEGY_0,										/* AGC1 TOP STRATEGY 0 */ \
    TDA18250A_AGC1_DET_SPEED_62_5KHz,									/* AGC1 Det Speed */	  \
    TDA18250A_AGC1_SMOOTH_T_CST_51_2ms,                                 /* AGC1 Smooth T Cst */   \
    TDA18250A_LNA_ZIN_S11,												/* LNA_Zin */			  \
    {                                                                   /* AGC2 TOP */            \
        {                                                                                         \
            (UInt32)0,                                                                                    \
            (UInt8)98,                                                                                  \
            (UInt8)96                                                                                   \
        },                                                                                        \
        {                                                                                         \
            (UInt32)600000000,                                                                                   \
            (UInt8)99,                                                                                   \
            (UInt8)97                                                                                    \
        }                                                                                         \
    },                                                                                            \
    TDA18250A_AGC2_DET_SPEED_62_5KHz,									/* AGC2 Det Speed */	  \
    TDA18250A_DET12_CINT_200fF,											/* Det12 Cint */		  \
    {                                                                   /* AGC3 TOP */            \
        {                                                                                         \
            (UInt32)0,                                                                                    \
            (UInt8)110,                                                                                  \
            (UInt8)105                                                                                   \
        },                                                                                        \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            (UInt8)-1,                                                                                   \
            (UInt8)-1                                                                                    \
        }                                                                                         \
    },                                                                                            \
    TDA18250A_IF_Output_Level_1Vpp_min_6_24dB,							/* IF Output Level */     \
    {                                                                   /* S2D Gain */            \
        {                                                                                         \
            (UInt32)0,                                                                                    \
            TDA18250A_S2D_Gain_6dB                                                                \
        },                                                                                        \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            TDA18250A_S2D_Gain_6dB                                                                \
        }                                                                                         \
    },                                                                                            \
    \
    /****************************************************************/							  \
    /* GSK Settings                                                 */							  \
    /****************************************************************/							  \
    TDA18250A_AGCK_Time_Constant_32_768ms,								/* AGCK Time Constant */  \
    TDA18250A_RSSI_HP_FC_0_3M,											/* RSSI HP FC */		  \
    \
    /****************************************************************/							  \
    /* H3H5 Settings                                                */							  \
    /****************************************************************/							  \
    TDA18250A_VHF_III_Mode_Disabled,									/* VHF III Mode */		  \
    \
    /****************************************************************/							  \
    /*RSSI Settings                                                 */							  \
    /****************************************************************/							  \
    TDA18250A_RSSI_CAP_VAL_3pF,											/* RSSI_Ck_speed */		  \
    TDA18250A_RSSI_CK_SPEED_31_25kHz,									/* RSSI_Cap_Val */		  \
    \
    TDA18250APowerSavingMode_Normal,									/* Normal or Low */		  \
    0x75,                                                               /* Nominal Power Saving Byte 1 */ \
    0x06,                                                               /* Nominal Power Saving Byte 2 */ \
    0x07,                                                               /* Nominal Power Saving Byte 3 */ \
    0xBE,                                                               /* Low Power Saving Byte 1 */ \
    0xA2,                                                               /* Low Power Saving Byte 2 */ \
    0x4A,                                                               /* Low Power Saving Byte 3 */ \
}

#define TDA18250A_CONFIG_STD_DVBT_1_7MHZ \
    {																		/* DVB-T/T2 1.7MHz */	  \
    /****************************************************************/							  \
    /* IF Settings                                                  */							  \
    /****************************************************************/							  \
    850000,																/* IF */				  \
    0,																	/* CF_Offset */			  \
    \
    /****************************************************************/							  \
    /* IF SELECTIVITY Settings                                      */							  \
    /****************************************************************/							  \
    TDA18250A_LPF_1_5MHz,												/* LPF */				  \
    TDA18250A_LPFOffset_min_8pc,										/* LPF_Offset */		  \
    TDA18250A_DC_Notch_IF_PPF_Disabled,									/* DC notch IF PPF */	  \
    TDA18250A_HPF_Disabled,												/* Hi Pass */			  \
    TDA18250A_HPFOffset_0pc,											/* HPF Offset */		  \
    TDA18250A_IF_Notch_Disabled,										/* IF notch */			  \
    TDA18250A_IF_Notch_Freq_6_25MHz,									/* IF Notch Freq */		  \
    TDA18250A_IF_Notch_Offset_0pc,										/* IF Notch Offset */     \
    TDA18250A_IFnotchToRSSI_Disabled,									/* IF notch To RSSI */    \
    \
    /****************************************************************/							  \
    /* AGC Settings                                                 */							  \
    /****************************************************************/							  \
    TDA18250AAGC1_GAIN_Free,                                            /* AGC1 GAIN */           \
    TDA18250A_AGC1_TOP_I2C_DN_UP_d97_u91dBuV,							/* AGC1 TOP I2C DN/UP */  \
    TDA18250A_AGC1_TOP_STRATEGY_4,										/* AGC1 TOP STRATEGY 0 */ \
    TDA18250A_AGC1_DET_SPEED_125KHz,									/* AGC1 Det Speed */	  \
    TDA18250A_AGC1_SMOOTH_T_CST_1_6ms,                                  /* AGC1 Smooth T Cst */   \
    TDA18250A_LNA_ZIN_S11,												/* LNA_Zin */			  \
    {                                                                   /* AGC2 TOP */            \
        {                                                                                         \
            (UInt32)0,                                                                                    \
            (UInt8)99,                                                                                   \
            (UInt8)97                                                                                    \
        },                                                                                        \
        {                                                                                         \
            (UInt32)320000000,                                                                            \
            (UInt8)102,                                                                                  \
            (UInt8)100                                                                                   \
        }                                                                                         \
    },                                                                                            \
    TDA18250A_AGC2_DET_SPEED_125KHz,									/* AGC2 Det Speed */	  \
    TDA18250A_DET12_CINT_50fF,											/* Det12 Cint */		  \
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
        }                                                                                         \
    },                                                                                            \
    TDA18250A_IF_Output_Level_1Vpp_min_6_24dB,							/* IF Output Level */	  \
    {                                                                   /* S2D Gain */            \
        {                                                                                         \
            (UInt32)0,                                                                                    \
            TDA18250A_S2D_Gain_9dB                                                                \
        },                                                                                        \
        {                                                                                         \
            (UInt32)320000000,                                                                            \
            TDA18250A_S2D_Gain_6dB                                                                \
        }                                                                                         \
    },                                                                                            \
    \
    /****************************************************************/							  \
    /* GSK Settings                                                 */							  \
    /****************************************************************/							  \
    TDA18250A_AGCK_Time_Constant_32_768ms,								/* AGCK Time Constant */  \
    TDA18250A_RSSI_HP_FC_0_3M,											/* RSSI HP FC */		  \
    \
    /****************************************************************/							  \
    /* H3H5 Settings                                                */							  \
    /****************************************************************/							  \
    TDA18250A_VHF_III_Mode_Enabled,										/* VHF III Mode */		  \
    \
    /****************************************************************/							  \
    /*RSSI Settings                                                 */							  \
    /****************************************************************/							  \
    TDA18250A_RSSI_CAP_VAL_3pF,											/* RSSI_Ck_speed */		  \
    TDA18250A_RSSI_CK_SPEED_31_25kHz,									/* RSSI_Cap_Val */		  \
    \
    TDA18250APowerSavingMode_Normal,									/* Normal or Low */       \
    0x44,                                                               /* Nominal Power Saving Byte 1 */ \
    0x06,                                                               /* Nominal Power Saving Byte 2 */ \
    0x07,                                                               /* Nominal Power Saving Byte 3 */ \
    0xBE,                                                               /* Low Power Saving Byte 1 */ \
    0xA2,                                                               /* Low Power Saving Byte 2 */ \
    0x4A,                                                               /* Low Power Saving Byte 3 */ \
}

#define TDA18250A_CONFIG_STD_DVBT_6MHZ \
    {																		/* QAM 6MHz */			  \
    /****************************************************************/							  \
    /* IF Settings                                                  */							  \
    /****************************************************************/							  \
    3250000,															/* IF */				  \
    0,																	/* CF_Offset */			  \
    \
    /****************************************************************/							  \
    /* IF SELECTIVITY Settings                                      */							  \
    /****************************************************************/							  \
    TDA18250A_LPF_6MHz,													/* LPF */				  \
    TDA18250A_LPFOffset_min_8pc,										/* LPF_Offset */		  \
    TDA18250A_DC_Notch_IF_PPF_Disabled,									/* DC notch IF PPF */	  \
    TDA18250A_HPF_0_5MHz,												/* Hi Pass */			  \
    TDA18250A_HPFOffset_0pc,											/* HPF Offset */		  \
    TDA18250A_IF_Notch_Enabled,											/* IF notch */			  \
    TDA18250A_IF_Notch_Freq_6_25MHz,									/* IF Notch Freq */		  \
    TDA18250A_IF_Notch_Offset_plus_4pc,									/* IF Notch Offset */	  \
    TDA18250A_IFnotchToRSSI_Disabled,									/* IF notch To RSSI */	  \
    \
    /****************************************************************/							  \
    /* AGC Settings                                                 */						      \
    /****************************************************************/							  \
    TDA18250AAGC1_GAIN_Free,                                            /* AGC1 GAIN */           \
    TDA18250A_AGC1_TOP_I2C_DN_UP_d97_u91dBuV,							/* AGC1 TOP I2C DN/UP */  \
    TDA18250A_AGC1_TOP_STRATEGY_4,										/* AGC1 TOP STRATEGY 0 */ \
    TDA18250A_AGC1_DET_SPEED_125KHz,									/* AGC1 Det Speed */	  \
    TDA18250A_AGC1_SMOOTH_T_CST_1_6ms,                                  /* AGC1 Smooth T Cst */   \
    TDA18250A_LNA_ZIN_NF,												/* LNA_Zin */			  \
    {                                                                   /* AGC2 TOP */            \
        {                                                                                         \
            (UInt32)0,                                                                                    \
            (UInt8)99,                                                                                   \
            (UInt8)97                                                                                    \
        },                                                                                        \
        {                                                                                         \
            (UInt32)320000000,                                                                            \
            (UInt8)102,                                                                                  \
            (UInt8)100                                                                                   \
        }                                                                                         \
    },                                                                                            \
    TDA18250A_AGC2_DET_SPEED_125KHz,									/* AGC2 Det Speed */	  \
    TDA18250A_DET12_CINT_50fF,											/* Det12 Cint */		  \
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
        }                                                                                         \
    },                                                                                            \
    TDA18250A_IF_Output_Level_1Vpp_min_6_24dB,							/* IF Output Level */     \
    {                                                                   /* S2D Gain */            \
        {                                                                                         \
            (UInt32)0,                                                                                    \
            TDA18250A_S2D_Gain_9dB                                                                \
        },                                                                                        \
        {                                                                                         \
            (UInt32)320000000,                                                                            \
            TDA18250A_S2D_Gain_6dB                                                                \
        }                                                                                          \
    },                                                                                            \
    \
    /****************************************************************/							  \
    /* GSK Settings                                                 */							  \
    /****************************************************************/							  \
    TDA18250A_AGCK_Time_Constant_32_768ms,								/* AGCK Time Constant */  \
    TDA18250A_RSSI_HP_FC_0_3M,											/* RSSI HP FC */		  \
    \
    /****************************************************************/							  \
    /* H3H5 Settings                                                */							  \
    /****************************************************************/							  \
    TDA18250A_VHF_III_Mode_Enabled,										/* VHF III Mode */		  \
    \
    /****************************************************************/							  \
    /*RSSI Settings                                                 */							  \
    /****************************************************************/							  \
    TDA18250A_RSSI_CAP_VAL_3pF,											/* RSSI_Ck_speed */		  \
    TDA18250A_RSSI_CK_SPEED_31_25kHz,									/* RSSI_Cap_Val */		  \
    \
    TDA18250APowerSavingMode_Normal,									/* Normal or Low */		  \
    0x44,                                                               /* Nominal Power Saving Byte 1 */ \
    0x06,                                                               /* Nominal Power Saving Byte 2 */ \
    0x07,                                                               /* Nominal Power Saving Byte 3 */ \
    0xBE,                                                               /* Low Power Saving Byte 1 */ \
    0xA2,                                                               /* Low Power Saving Byte 2 */ \
    0x4A,                                                               /* Low Power Saving Byte 3 */ \
}

#define TDA18250A_CONFIG_STD_DVBT_7MHZ \
    {																		/* DVB-T/T2 7MHz */		  \
    /****************************************************************/							  \
    /* IF Settings                                                  */							  \
    /****************************************************************/							  \
    3500000,															/* IF */				  \
    0,																	/* CF_Offset */			  \
    \
    /****************************************************************/							  \
    /* IF SELECTIVITY Settings                                      */							  \
    /****************************************************************/							  \
    TDA18250A_LPF_7MHz,													/* LPF */				  \
    TDA18250A_LPFOffset_min_8pc,										/* LPF_Offset */		  \
    TDA18250A_DC_Notch_IF_PPF_Disabled,									/* DC notch IF PPF */	  \
    TDA18250A_HPF_Disabled,												/* Hi Pass */			  \
    TDA18250A_HPFOffset_0pc,											/* HPF Offset */		  \
    TDA18250A_IF_Notch_Enabled,											/* IF notch */			  \
    TDA18250A_IF_Notch_Freq_7_25MHz,									/* IF Notch Freq */		  \
    TDA18250A_IF_Notch_Offset_0pc,										/* IF Notch Offset */	  \
    TDA18250A_IFnotchToRSSI_Disabled,									/* IF notch To RSSI */    \
    \
    /****************************************************************/							  \
    /* AGC Settings                                                 */							  \
    /****************************************************************/							  \
    TDA18250AAGC1_GAIN_Free,                                            /* AGC1 GAIN */           \
    TDA18250A_AGC1_TOP_I2C_DN_UP_d97_u91dBuV,							/* AGC1 TOP I2C DN/UP */  \
    TDA18250A_AGC1_TOP_STRATEGY_4,										/* AGC1 TOP STRATEGY 0 */ \
    TDA18250A_AGC1_DET_SPEED_125KHz,									/* AGC1 Det Speed */	  \
    TDA18250A_AGC1_SMOOTH_T_CST_1_6ms,                                  /* AGC1 Smooth T Cst */   \
    TDA18250A_LNA_ZIN_NF,												/* LNA_Zin */			  \
    {                                                                   /* AGC2 TOP */            \
        {                                                                                         \
            (UInt32)0,                                                                                    \
            (UInt8)99,                                                                                   \
            (UInt8)97                                                                                    \
        },                                                                                        \
        {                                                                                         \
            (UInt32)320000000,                                                                            \
            (UInt8)102,                                                                                  \
            (UInt8)100                                                                                   \
        }                                                                                         \
    },                                                                                            \
    TDA18250A_AGC2_DET_SPEED_125KHz,									/* AGC2 Det Speed */	  \
    TDA18250A_DET12_CINT_50fF,											/* Det12 Cint */		  \
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
        }                                                                                         \
    },                                                                                            \
    TDA18250A_IF_Output_Level_1Vpp_min_6_24dB,							/* IF Output Level */	  \
    {                                                                   /* S2D Gain */            \
        {                                                                                         \
            (UInt32)0,                                                                                    \
            TDA18250A_S2D_Gain_9dB                                                                \
        },                                                                                        \
        {                                                                                         \
            (UInt32)320000000,                                                                            \
            TDA18250A_S2D_Gain_6dB                                                                \
        }                                                                                          \
    },                                                                                            \
    \
    /****************************************************************/							  \
    /* GSK Settings                                                 */							  \
    /****************************************************************/							  \
    TDA18250A_AGCK_Time_Constant_32_768ms,								/* AGCK Time Constant */  \
    TDA18250A_RSSI_HP_FC_0_3M,											/* RSSI HP FC */		  \
    \
    /****************************************************************/							  \
    /* H3H5 Settings                                                */							  \
    /****************************************************************/							  \
    TDA18250A_VHF_III_Mode_Enabled,										/* VHF III Mode */		  \
    \
    /****************************************************************/							  \
    /*RSSI Settings                                                 */							  \
    /****************************************************************/							  \
    TDA18250A_RSSI_CAP_VAL_3pF,											/* RSSI_Ck_speed */		  \
    TDA18250A_RSSI_CK_SPEED_31_25kHz,									/* RSSI_Cap_Val */		  \
    \
    TDA18250APowerSavingMode_Normal,									/* Normal or Low */		  \
    0x44,                                                               /* Nominal Power Saving Byte 1 */ \
    0x06,                                                               /* Nominal Power Saving Byte 2 */ \
    0x07,                                                               /* Nominal Power Saving Byte 3 */ \
    0xBE,                                                               /* Low Power Saving Byte 1 */ \
    0xA2,                                                               /* Low Power Saving Byte 2 */ \
    0x4A,                                                               /* Low Power Saving Byte 3 */ \
}

#define TDA18250A_CONFIG_STD_DVBT_8MHZ \
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
    TDA18250A_LPF_9MHz,													/* LPF */				  \
    TDA18250A_LPFOffset_min_8pc,										/* LPF_Offset */		  \
    TDA18250A_DC_Notch_IF_PPF_Disabled,									/* DC notch IF PPF */	  \
    TDA18250A_HPF_1MHz,                                                 /* Hi Pass */			  \
    TDA18250A_HPFOffset_0pc,											/* HPF Offset */		  \
    TDA18250A_IF_Notch_Enabled,											/* IF notch */			  \
    TDA18250A_IF_Notch_Freq_9_25MHz,									/* IF Notch Freq */		  \
    TDA18250A_IF_Notch_Offset_0pc,										/* IF Notch Offset */	  \
    TDA18250A_IFnotchToRSSI_Disabled,									/* IF notch To RSSI */	  \
    \
    /****************************************************************/							  \
    /* AGC Settings                                                 */							  \
    /****************************************************************/							  \
    TDA18250AAGC1_GAIN_Free,                                            /* AGC1 GAIN */           \
    TDA18250A_AGC1_TOP_I2C_DN_UP_d97_u91dBuV,							/* AGC1 TOP I2C DN/UP */  \
    TDA18250A_AGC1_TOP_STRATEGY_4,										/* AGC1 TOP STRATEGY 0 */ \
    TDA18250A_AGC1_DET_SPEED_125KHz,									/* AGC1 Det Speed */	  \
    TDA18250A_AGC1_SMOOTH_T_CST_51_2ms,                                 /* AGC1 Smooth T Cst */   \
    TDA18250A_LNA_ZIN_NF,												/* LNA_Zin */			  \
    {                                                                   /* AGC2 TOP */            \
        {                                                                                         \
            (UInt32)0,                                                                                    \
            (UInt8)100,                                                                                  \
            (UInt8)99                                                                                    \
        },                                                                                        \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            (UInt8)-1,                                                                                   \
            (UInt8)-1                                                                                    \
        }                                                                                         \
    },                                                                                            \
    TDA18250A_AGC2_DET_SPEED_125KHz,									/* AGC2 Det Speed */	  \
    TDA18250A_DET12_CINT_50fF,											/* Det12 Cint */		  \
    {                                                                   /* AGC3 TOP */            \
        {                                                                                         \
            (UInt32)0,                                                                                    \
            (UInt8)110,                                                                                  \
            (UInt8)104                                                                                   \
        },                                                                                        \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            (UInt8)-1,                                                                                   \
            (UInt8)-1                                                                                    \
        }                                                                                         \
    },                                                                                            \
    TDA18250A_IF_Output_Level_1Vpp_min_6_24dB,							/* IF Output Level */	  \
    {                                                                   /* S2D Gain */            \
        {                                                                                         \
            (UInt32)0,                                                                                    \
            TDA18250A_S2D_Gain_9dB                                                                \
        },                                                                                        \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            TDA18250A_S2D_Gain_6dB                                                                \
        }                                                                                         \
    },                                                                                            \
    \
    /****************************************************************/							  \
    /* GSK Settings                                                 */						      \
    /****************************************************************/							  \
    TDA18250A_AGCK_Time_Constant_32_768ms,								/* AGCK Time Constant */  \
    TDA18250A_RSSI_HP_FC_0_3M,											/* RSSI HP FC */		  \
    \
    /****************************************************************/							  \
    /* H3H5 Settings                                                */							  \
    /****************************************************************/							  \
    TDA18250A_VHF_III_Mode_Enabled,										/* VHF III Mode */		  \
    \
    /****************************************************************/							  \
    /*RSSI Settings                                                 */							  \
    /****************************************************************/							  \
    TDA18250A_RSSI_CAP_VAL_3pF,											/* RSSI_Ck_speed */		  \
    TDA18250A_RSSI_CK_SPEED_31_25kHz,									/* RSSI_Cap_Val */		  \
    \
    TDA18250APowerSavingMode_Normal,									/* Normal or Low */		  \
    0x44,                                                               /* Nominal Power Saving Byte 1 */ \
    0x06,                                                               /* Nominal Power Saving Byte 2 */ \
    0x07,                                                               /* Nominal Power Saving Byte 3 */ \
    0xBE,                                                               /* Low Power Saving Byte 1 */ \
    0xA2,                                                               /* Low Power Saving Byte 2 */ \
    0x4A,                                                               /* Low Power Saving Byte 3 */ \
}

#define TDA18250A_CONFIG_STD_DVBT_10MHZ \
    {																		/* DVB-T/T2 10MHz */	  \
    /****************************************************************/							  \
    /* IF Settings                                                  */							  \
    /****************************************************************/							  \
    5000000,															/* IF */                  \
    0,																	/* CF_Offset */			  \
    \
    /****************************************************************/							  \
    /* IF SELECTIVITY Settings                                      */							  \
    /****************************************************************/							  \
    TDA18250A_LPF_10MHz,												/* LPF */				  \
    TDA18250A_LPFOffset_min_8pc,										/* LPF_Offset */		  \
    TDA18250A_DC_Notch_IF_PPF_Disabled,									/* DC notch IF PPF */	  \
    TDA18250A_HPF_Disabled,												/* Hi Pass */			  \
    TDA18250A_HPFOffset_0pc,											/* HPF Offset */		  \
    TDA18250A_IF_Notch_Enabled,											/* IF notch */			  \
    TDA18250A_IF_Notch_Freq_10_25MHz,									/* IF Notch Freq */		  \
    TDA18250A_IF_Notch_Offset_0pc,										/* IF Notch Offset */	  \
    TDA18250A_IFnotchToRSSI_Disabled,									/* IF notch To RSSI */	  \
    \
    /****************************************************************/							  \
    /* AGC Settings                                                 */							  \
    /****************************************************************/							  \
    TDA18250AAGC1_GAIN_Free,                                            /* AGC1 GAIN */           \
    TDA18250A_AGC1_TOP_I2C_DN_UP_d97_u91dBuV,							/* AGC1 TOP I2C DN/UP */  \
    TDA18250A_AGC1_TOP_STRATEGY_4,										/* AGC1 TOP STRATEGY 0 */ \
    TDA18250A_AGC1_DET_SPEED_125KHz,									/* AGC1 Det Speed */	  \
    TDA18250A_AGC1_SMOOTH_T_CST_1_6ms,                                  /* AGC1 Smooth T Cst */   \
    TDA18250A_LNA_ZIN_S11,												/* LNA_Zin */			  \
    {                                                                   /* AGC2 TOP */            \
        {                                                                                         \
            (UInt32)0,                                                                                    \
            (UInt8)99,                                                                                   \
            (UInt8)97                                                                                    \
        },                                                                                        \
        {                                                                                         \
            (UInt32)320000000,                                                                            \
            (UInt8)102,                                                                                  \
            (UInt8)100                                                                                   \
        }                                                                                         \
    },                                                                                            \
    TDA18250A_AGC2_DET_SPEED_125KHz,									/* AGC2 Det Speed */	  \
    TDA18250A_DET12_CINT_50fF,											/* Det12 Cint */		  \
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
        }                                                                                         \
    },                                                                                            \
    TDA18250A_IF_Output_Level_1Vpp_min_6_24dB,							/* IF Output Level */	  \
    {                                                                   /* S2D Gain */            \
        {                                                                                         \
            (UInt32)0,                                                                                    \
            TDA18250A_S2D_Gain_9dB                                                                \
        },                                                                                        \
        {                                                                                         \
            (UInt32)320000000,                                                                            \
            TDA18250A_S2D_Gain_6dB                                                                \
        }                                                                                          \
    },                                                                                            \
    \
    /****************************************************************/							  \
    /* GSK Settings                                                 */							  \
    /****************************************************************/							  \
    TDA18250A_AGCK_Time_Constant_32_768ms,								/* AGCK Time Constant */  \
    TDA18250A_RSSI_HP_FC_0_3M,											/* RSSI HP FC */		  \
    \
    /****************************************************************/							  \
    /* H3H5 Settings                                                */							  \
    /****************************************************************/							  \
    TDA18250A_VHF_III_Mode_Enabled,										/* VHF III Mode */		  \
    \
    /****************************************************************/							  \
    /*RSSI Settings                                                 */							  \
    /****************************************************************/							  \
    TDA18250A_RSSI_CAP_VAL_3pF,											/* RSSI_Ck_speed */		  \
    TDA18250A_RSSI_CK_SPEED_31_25kHz,									/* RSSI_Cap_Val */		  \
    \
    TDA18250APowerSavingMode_Normal,									/* Normal or Low */		  \
    0x44,                                                               /* Nominal Power Saving Byte 1 */ \
    0x06,                                                               /* Nominal Power Saving Byte 2 */ \
    0x07,                                                               /* Nominal Power Saving Byte 3 */ \
    0xBE,                                                               /* Low Power Saving Byte 1 */ \
    0xA2,                                                               /* Low Power Saving Byte 2 */ \
    0x4A,                                                               /* Low Power Saving Byte 3 */ \
}

#define TDA18250A_CONFIG_STD_DTMB_8MHz \
    {																		/* DMB-T */				  \
    /****************************************************************/							  \
    /* IF Settings                                                  */							  \
    /****************************************************************/							  \
    5000000,															/* IF */				  \
    0,																	/* CF_Offset */			  \
    \
    /****************************************************************/							  \
    /* IF SELECTIVITY Settings                                      */							  \
    /****************************************************************/							  \
    TDA18250A_LPF_9MHz,													/* LPF */				  \
    TDA18250A_LPFOffset_0pc,                                            /* LPF_Offset */		  \
    TDA18250A_DC_Notch_IF_PPF_Disabled,									/* DC notch IF PPF */	  \
    TDA18250A_HPF_1MHz,                                                 /* Hi Pass */			  \
    TDA18250A_HPFOffset_0pc,											/* HPF Offset */		  \
    TDA18250A_IF_Notch_Enabled,											/* IF notch */			  \
    TDA18250A_IF_Notch_Freq_9_25MHz,									/* IF Notch Freq */		  \
    TDA18250A_IF_Notch_Offset_0pc,										/* IF Notch Offset */     \
    TDA18250A_IFnotchToRSSI_Disabled,									/* IF notch To RSSI */    \
    \
    /****************************************************************/							  \
    /* AGC Settings                                                 */						      \
    /****************************************************************/							  \
    TDA18250AAGC1_GAIN_Free,                                            /* AGC1 GAIN */           \
    TDA18250A_AGC1_TOP_I2C_DN_UP_d97_u91dBuV,							/* AGC1 TOP I2C DN/UP */  \
    TDA18250A_AGC1_TOP_STRATEGY_4,										/* AGC1 TOP STRATEGY 0 */ \
    TDA18250A_AGC1_DET_SPEED_125KHz,									/* AGC1 Det Speed */	  \
    TDA18250A_AGC1_SMOOTH_T_CST_1_6ms,                                  /* AGC1 Smooth T Cst */   \
    TDA18250A_LNA_ZIN_S11,												/* LNA_Zin */			  \
    {                                                                   /* AGC2 TOP */            \
        {                                                                                         \
            (UInt32)0,                                                                                    \
            (UInt8)99,                                                                                   \
            (UInt8)97                                                                                    \
        },                                                                                        \
        {                                                                                         \
            (UInt32)320000000,                                                                            \
            (UInt8)102,                                                                                  \
            (UInt8)100                                                                                   \
        }                                                                                         \
    },                                                                                            \
    TDA18250A_AGC2_DET_SPEED_125KHz,									/* AGC2 Det Speed */	  \
    TDA18250A_DET12_CINT_50fF,											/* Det12 Cint */		  \
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
        }                                                                                         \
    },                                                                                            \
    TDA18250A_IF_Output_Level_1Vpp_min_6_24dB,							/* IF Output Level */	  \
    {                                                                   /* S2D Gain */            \
        {                                                                                         \
            (UInt32)0,                                                                                    \
            TDA18250A_S2D_Gain_9dB                                                                \
        },                                                                                        \
        {                                                                                         \
            (UInt32)320000000,                                                                            \
            TDA18250A_S2D_Gain_6dB                                                                \
        }                                                                                          \
    },                                                                                            \
    \
    /****************************************************************/							  \
    /* GSK Settings                                                 */							  \
    /****************************************************************/							  \
    TDA18250A_AGCK_Time_Constant_32_768ms,								/* AGCK Time Constant */  \
    TDA18250A_RSSI_HP_FC_0_3M,											/* RSSI HP FC */		  \
    \
    /****************************************************************/							  \
    /* H3H5 Settings                                                */							  \
    /****************************************************************/							  \
    TDA18250A_VHF_III_Mode_Enabled,										/* VHF III Mode */		  \
    \
    /****************************************************************/							  \
    /*RSSI Settings                                                 */							  \
    /****************************************************************/							  \
    TDA18250A_RSSI_CAP_VAL_3pF,											/* RSSI_Ck_speed */		  \
    TDA18250A_RSSI_CK_SPEED_31_25kHz,									/* RSSI_Cap_Val */		  \
    \
    TDA18250APowerSavingMode_Normal,									/* Normal or Low */		  \
    0x44,                                                               /* Nominal Power Saving Byte 1 */ \
    0x06,                                                               /* Nominal Power Saving Byte 2 */ \
    0x07,                                                               /* Nominal Power Saving Byte 3 */ \
    0xBE,                                                               /* Low Power Saving Byte 1 */ \
    0xA2,                                                               /* Low Power Saving Byte 2 */ \
    0x4A,                                                               /* Low Power Saving Byte 3 */ \
}

#define TDA18250A_CONFIG_STD_ISDBT_6MHZ \
    {																		/* ISDB-T 6MHz */		  \
    /****************************************************************/							  \
    /* IF Settings                                                  */							  \
    /****************************************************************/							  \
    3250000,															/* IF */				  \
    0,																	/* CF_Offset */			  \
    \
    /****************************************************************/							  \
    /* IF SELECTIVITY Settings                                      */							  \
    /****************************************************************/							  \
    TDA18250A_LPF_6MHz,													/* LPF */				  \
    TDA18250A_LPFOffset_0pc,										    /* LPF_Offset */		  \
    TDA18250A_DC_Notch_IF_PPF_Disabled,									/* DC notch IF PPF */	  \
    TDA18250A_HPF_0_5MHz,												/* Hi Pass */			  \
    TDA18250A_HPFOffset_0pc,											/* HPF Offset */		  \
    TDA18250A_IF_Notch_Enabled,											/* IF notch */			  \
    TDA18250A_IF_Notch_Freq_6_25MHz,									/* IF Notch Freq */		  \
    TDA18250A_IF_Notch_Offset_plus_8pc,									/* IF Notch Offset */	  \
    TDA18250A_IFnotchToRSSI_Disabled,									/* IF notch To RSSI */    \
    \
    /****************************************************************/							  \
    /* AGC Settings                                                 */							  \
    /****************************************************************/							  \
    TDA18250AAGC1_GAIN_Free,                                            /* AGC1 GAIN */           \
    TDA18250A_AGC1_TOP_I2C_DN_UP_d97_u91dBuV,							/* AGC1 TOP I2C DN/UP */  \
    TDA18250A_AGC1_TOP_STRATEGY_4,										/* AGC1 TOP STRATEGY 0 */ \
    TDA18250A_AGC1_DET_SPEED_125KHz,									/* AGC1 Det Speed */	  \
    TDA18250A_AGC1_SMOOTH_T_CST_1_6ms,                                  /* AGC1 Smooth T Cst */   \
    TDA18250A_LNA_ZIN_NF,												/* LNA_Zin */			  \
    {                                                                   /* AGC2 TOP */            \
        {                                                                                         \
            (UInt32)0,                                                                                    \
            (UInt8)99,                                                                                   \
            (UInt8)97                                                                                    \
        },                                                                                        \
        {                                                                                         \
            (UInt32)-1,                                                                            \
            (UInt8)-1,                                                                                  \
            (UInt8)-1                                                                                   \
        }                                                                                         \
    },                                                                                            \
    TDA18250A_AGC2_DET_SPEED_125KHz,									/* AGC2 Det Speed */	  \
    TDA18250A_DET12_CINT_50fF,											/* Det12 Cint */		  \
    {                                                                   /* AGC3 TOP */            \
        {                                                                                         \
            (UInt32)0,                                                                                    \
            (UInt8)112,                                                                                  \
            (UInt8)107                                                                                   \
        },                                                                                        \
        {                                                                                         \
            (UInt32)-1,                                                                            \
            (UInt8)-1,                                                                                  \
            (UInt8)-1                                                                                   \
        }                                                                                         \
    },                                                                                            \
    TDA18250A_IF_Output_Level_0_6Vpp_min_10_3_19_7dB,					/* IF Output Level */	  \
    {                                                                   /* S2D Gain */            \
        {                                                                                         \
            (UInt32)0,                                                                                    \
            TDA18250A_S2D_Gain_9dB                                                                \
        },                                                                                        \
        {                                                                                         \
            (UInt32)320000000,                                                                            \
            TDA18250A_S2D_Gain_6dB                                                                \
        }                                                                                          \
    },                                                                                            \
    \
    /****************************************************************/							  \
    /* GSK Settings                                                 */							  \
    /****************************************************************/							  \
    TDA18250A_AGCK_Time_Constant_32_768ms,								/* AGCK Time Constant */  \
    TDA18250A_RSSI_HP_FC_0_3M,											/* RSSI HP FC */		  \
    \
    /****************************************************************/							  \
    /* H3H5 Settings                                                */							  \
    /****************************************************************/							  \
    TDA18250A_VHF_III_Mode_Enabled,										/* VHF III Mode */		  \
    \
    /****************************************************************/							  \
    /*RSSI Settings                                                 */							  \
    /****************************************************************/							  \
    TDA18250A_RSSI_CAP_VAL_3pF,											/* RSSI_Ck_speed */		  \
    TDA18250A_RSSI_CK_SPEED_31_25kHz,									/* RSSI_Cap_Val */		  \
    \
    TDA18250APowerSavingMode_Normal,									/* Normal or Low */		  \
    0x44,                                                               /* Nominal Power Saving Byte 1 */ \
    0x06,                                                               /* Nominal Power Saving Byte 2 */ \
    0x07,                                                               /* Nominal Power Saving Byte 3 */ \
    0xBE,                                                               /* Low Power Saving Byte 1 */ \
    0xA2,                                                               /* Low Power Saving Byte 2 */ \
    0x4A,                                                               /* Low Power Saving Byte 3 */ \
}

    /* Default configuration */
#define TDA18250A_CONFIG_CURPOWERSTATE_DEF tmPowerMax
#define TDA18250A_CONFIG_CURLLPOWERSTATE_DEF TDA18250A_PowerMax
#define TDA18250A_CONFIG_RF_DEF 0
#define TDA18250A_CONFIG_PROG_RF_DEF 0
#define TDA18250A_CONFIG_STANDARDMODE_DEF TDA18250A_StandardMode_Max

    /* Power settings */
#define TDA18250A_CONFIG_POWER_DEF_MASTER \
    TDA18250A_CONFIG_CURPOWERSTATE_DEF,                     /* curPowerState */       \
    TDA18250A_CONFIG_CURLLPOWERSTATE_DEF,                   /* curLLPowerState */     \
    {                                                       /* mapLLPowerState */     \
    TDA18250A_PowerNormalMode,                          /* tmPowerOn (D0) */      \
    TDA18250A_PowerStandbyWithLtOnWithXtalOn,           /* tmPowerStandby (D1) */ \
    TDA18250A_PowerStandbyWithLtOnWithXtalOn,           /* tmPowerSuspend (D2) */ \
    TDA18250A_PowerStandbyWithLtOnWithXtalOn            /* tmPowerOff (D3) */     \
}

#define TDA18250A_CONFIG_POWER_DEF_SLAVE \
    TDA18250A_CONFIG_CURPOWERSTATE_DEF,                     /* curPowerState */       \
    TDA18250A_CONFIG_CURLLPOWERSTATE_DEF,                   /* curLLPowerState */     \
    {                                                       /* mapLLPowerState */     \
    TDA18250A_PowerNormalMode,                          /* tmPowerOn (D0) */      \
    TDA18250A_PowerStandbyWithXtalOn,			        /* tmPowerStandby (D1) */ \
    TDA18250A_PowerStandbyWithXtalOn,					/* tmPowerSuspend (D2) */ \
    TDA18250A_PowerStandbyWithXtalOn					/* tmPowerOff (D3) */     \
}

    /* Standard Presets Aggregation: */
#define TDA18250A_CONFIG_STD_DEF \
    { \
    TDA18250A_CONFIG_STD_QAM_6MHZ,    \
    TDA18250A_CONFIG_STD_QAM_8MHZ,    \
    TDA18250A_CONFIG_STD_DVBT_1_7MHZ, \
    TDA18250A_CONFIG_STD_DVBT_6MHZ,   \
    TDA18250A_CONFIG_STD_DVBT_7MHZ,   \
    TDA18250A_CONFIG_STD_DVBT_8MHZ,   \
    TDA18250A_CONFIG_STD_DVBT_10MHZ,  \
    TDA18250A_CONFIG_STD_DTMB_8MHz,   \
    TDA18250A_CONFIG_STD_ISDBT_6MHZ,  \
}


#define TDA18250A_CONFIG_REGMAP_DEF \
    { \
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0           \
}


#define TDA18250A_CONFIG_MASTER \
    TDA18250A_CONFIG_POWER_DEF_MASTER, \
    TDA18250A_CONFIG_RF_DEF,                                /* uRF */                            \
    TDA18250A_CONFIG_PROG_RF_DEF,                           /* uProgRF */                        \
    TDA18250A_CONFIG_STANDARDMODE_DEF,                      /* StandardMode */                   \
    Null,                                                   /* pStandard */                      \
    False,                                                  /* bBufferMode */                    \
    False,                                                  /* Manual PLL Calc */                \
    TDA18250A_LoopThrough_Enabled,                          /* single app 18274 */               \
    TDA18250A_Mode_Dual,                                    /* single app */                     \
    TDA18250A_XtalFreq_16000000,                            /* eXtalFreq */                      \
    TDA18250A_XTOUT_Amp_800mV,                              /* eXTOutAmp */                      \
    TDA18250A_XTOUT_Sinusoidal,                             /* eXTOut */                         \
    TDA18250A_LNA_RANGE_Minus11dB_Plus22dB,					/* eLNA_Range */                     \
    TDA18250A_IRQ_POLARITY_LOW,                             /* eIRQPolarity */                   \
    TDA18250APowerSavingMode_Normal,						/* normal or High PowerSavingMode */ \
    TDA18250A_HwState_InitNotDone,                          /* eHwState */                       \
    TDA18250A_CONFIG_STD_DEF, \
    TDA18250A_CONFIG_REGMAP_DEF

#define TDA18250A_CONFIG_SLAVE \
    TDA18250A_CONFIG_POWER_DEF_SLAVE, \
    TDA18250A_CONFIG_RF_DEF,                                /* uRF */                            \
    TDA18250A_CONFIG_PROG_RF_DEF,                           /* uProgRF */                        \
    TDA18250A_CONFIG_STANDARDMODE_DEF,                      /* StandardMode */                   \
    Null,                                                   /* pStandard */                      \
    True,                                                   /* bBufferMode */                    \
    False,                                                  /* Manual PLL Calc */                \
    TDA18250A_LoopThrough_Enabled,                          /* single app 18274 */               \
    TDA18250A_Mode_Dual,                                    /* single app */                     \
    TDA18250A_XtalFreq_16000000,                            /* eXtalFreq */                      \
    TDA18250A_XTOUT_Amp_800mV,                              /* eXTOutAmp */                      \
    TDA18250A_XTOUT_Sinusoidal,                             /* eXTOut */                         \
    TDA18250A_LNA_RANGE_Minus11dB_Plus22dB,					/* eLNA_Range */                     \
    TDA18250A_IRQ_POLARITY_LOW,                             /* eIRQPolarity */                   \
    TDA18250APowerSavingMode_Normal,						/* normal or High PowerSavingMode */ \
    TDA18250A_HwState_InitNotDone,                          /* eHwState */                       \
    TDA18250A_CONFIG_STD_DEF, \
    TDA18250A_CONFIG_REGMAP_DEF


    /* Custom Driver Instance Parameters: (Path 0) */
#define TDA18250A_CONFIG_0 \
    TDA18250A_CONFIG_MASTER

    /* Custom Driver Instance Parameters: (Path 1) */
#define TDA18250A_CONFIG_1 \
    TDA18250A_CONFIG_SLAVE


#ifdef __cplusplus
}
#endif

#endif /* _TDA18250A_CONFIG_H */

