#ifndef _MXL661_H
#define _MXL661_H

#define FREQ_STEP                       62.5//50  //Khz
#define MXL661_VHF_LOWMIN_FREQ             47000L
#define MXL661_VHF_LOWMAX_FREQ             160000L
#define MXL661_VHF_HIGHMIN_FREQ            160000L
#define MXL661_VHF_HIGHMAX_FREQ            445000L
#define MXL661_UHF_MIN_FREQ_UK             445000L
#define MXL661_UHF_MIN_FREQ                434000L
#define MXL661_UHF_MAX_FREQ                894000L

#define VIF_TUNER_TYPE 1     // 0: RF Tuner; 1: Silicon Tuner
#define MAX_FINE_TUNE_STEP              32
#define VIF_CR_RATE_B                   0x000ABDA1 // 5+2.25M   // [21:0], CR_RATE for 6.4 MHz, HEX((xxx/43.2) * (2^22))
#define VIF_CR_INVERT_B                     0                   // Video carrier position; 0: high side; 1:low side
#define VIF_CR_RATE_GH                  0x000B7B42  //5+2.75M         // [21:0], CR_RATE for 1.52 MHz, HEX((xxx/43.2) * (2^22))
#define VIF_CR_INVERT_GH                    0                   // Video carrier position; 0: high side; 1:low side
#define VIF_CR_RATE_DK                  0x000B7B42// //5+2.75M           // [21:0], CR_RATE for 1.52 MHz, HEX((xxx/43.2) * (2^22))
#define VIF_CR_INVERT_DK                    0                   // Video carrier position; 0: high side; 1:low side
#define VIF_CR_RATE_I                   0x000B7B42 //5+2.75M          // [21:0], CR_RATE for 1.52 MHz, HEX((xxx/43.2) * (2^22))
#define VIF_CR_INVERT_I                     0                   // Video carrier position; 0: high side; 1:low side
#define VIF_CR_RATE_L                   0x000B7B42 // 5+2.75M         // [21:0], CR_RATE for 1.52 MHz, HEX((xxx/43.2) * (2^22))
#define VIF_CR_INVERT_L                     0                   // Video carrier position; 0: high side; 1:low side
#define VIF_CR_RATE_LL                  0x000B7B42// //5+2.75M          // [21:0], CR_RATE for 7.02 MHz, HEX((xxx/43.2) * (2^22))
#define VIF_CR_INVERT_LL                    0                   // Video carrier position; 0: high side; 1:low side
#define VIF_CR_RATE_MN                  0x000A0000// //5+1.75M         // [21:0], CR_RATE for 1.75 MHz, HEX((xxx/43.2) * (2^22))
#define VIF_CR_INVERT_MN                    0                   // Video carrier position; 0: high side; 1:low side


#define VIF_SOS_21_FILTER_C0            0x032D
#define VIF_SOS_21_FILTER_C1            0x069B
#define VIF_SOS_21_FILTER_C2            0x0209
#define VIF_SOS_21_FILTER_C3            0x04D3
#define VIF_SOS_21_FILTER_C4            0x015C

#define VIF_SOS_22_FILTER_C0            0x034D
#define VIF_SOS_22_FILTER_C1            0x068E
#define VIF_SOS_22_FILTER_C2            0x01F8
#define VIF_SOS_22_FILTER_C3            0x04B3
#define VIF_SOS_22_FILTER_C4            0x017A

#define VIF_SOS_31_FILTER_C0            0x0217
#define VIF_SOS_31_FILTER_C1            0x0754
#define VIF_SOS_31_FILTER_C2            0x00AC
#define VIF_SOS_31_FILTER_C3            0x05E9
#define VIF_SOS_31_FILTER_C4            0x0200

#define VIF_SOS_32_FILTER_C0            0x0000
#define VIF_SOS_32_FILTER_C1            0x0000
#define VIF_SOS_32_FILTER_C2            0x0000
#define VIF_SOS_32_FILTER_C3            0x0000
#define VIF_SOS_32_FILTER_C4            0x0200

//B_Stereo_A2 ========================
// 1.75Mhz
#define N_A1_C0_B_A2 0x03A4
#define N_A1_C1_B_A2 0x063C
#define N_A1_C2_B_A2 0x0421
// 1.51Mhz
#define N_A2_C0_B_A2 0x03AB
#define N_A2_C1_B_A2 0x063C
#define N_A2_C2_B_A2 0x0419
// 8.75Mhz
#define S_11_C0_B_A2 0x011B
#define S_11_C1_B_A2 0x063C
#define S_11_C2_B_A2 0x0200
#define S_11_C3_B_A2 0x06D3
#define S_11_C4_B_A2 0x0200
// 1.75Mhz
#define S_12_C0_B_A2 0x03A4
#define S_12_C1_B_A2 0x063C
#define S_12_C2_B_A2 0x0200
#define S_12_C3_B_A2 0x0421
#define S_12_C4_B_A2 0x0200

//B_Mono_NICAM ====================
// 1.75Mhz
#define N_A1_C0_B_NICAM 0x03A3
#define N_A1_C1_B_NICAM 0x063C
#define N_A1_C2_B_NICAM 0x0421
// 1.4Mhz
#define N_A2_C0_B_NICAM 0x03AF
#define N_A2_C1_B_NICAM 0x063C
#define N_A2_C2_B_NICAM 0x0415
// 8.75Mhz
#define S_11_C0_B_NICAM 0x011B
#define S_11_C1_B_NICAM 0x063C
#define S_11_C2_B_NICAM 0x0200
#define S_11_C3_B_NICAM 0x06D3
#define S_11_C4_B_NICAM 0x0200
// 1.75Mhz
#define S_12_C0_B_NICAM 0x03A4
#define S_12_C1_B_NICAM 0x063C
#define S_12_C2_B_NICAM 0x0200
#define S_12_C3_B_NICAM 0x0421
#define S_12_C4_B_NICAM 0x0200

//GH_Stereo_A2 =======================
// 2.25Mhz
#define N_A1_C0_GHA2 0x038F
#define N_A1_C1_GHA2 0x063C
#define N_A1_C2_GHA2 0x0436
// 2.01Mhz
#define N_A2_C0_GHA2 0x039A
#define N_A2_C1_GHA2 0x063C
#define N_A2_C2_GHA2 0x042B
// 10.25Mhz
#define S_11_C0_GHA2 0x004D
#define S_11_C1_GHA2 0x063C
#define S_11_C2_GHA2 0x0200
#define S_11_C3_GHA2 0x07AE
#define S_11_C4_GHA2 0x0200
// 2.25Mhz
#define S_12_C0_GHA2 0x038F
#define S_12_C1_GHA2 0x063C
#define S_12_C2_GHA2 0x0200
#define S_12_C3_GHA2 0x0436
#define S_12_C4_GHA2 0x0200

//GH_Mono_NICAM ===================
// 2.25Mhz
#define N_A1_C0_GHMN 0x038F
#define N_A1_C1_GHMN 0x063C
#define N_A1_C2_GHMN 0x0436
// 1.9Mhz
#define N_A2_C0_GHMN 0x039E
#define N_A2_C1_GHMN 0x063C
#define N_A2_C2_GHMN 0x0427
// 10.25Mhz
#define S_11_C0_GHMN 0x004D
#define S_11_C1_GHMN 0x063C
#define S_11_C2_GHMN 0x0200
#define S_11_C3_GHMN 0x07AE
#define S_11_C4_GHMN 0x0200
// 2.25Mhz
#define S_12_C0_GHMN 0x038F
#define S_12_C1_GHMN 0x063C
#define S_12_C2_GHMN 0x0200
#define S_12_C3_GHMN 0x0436
#define S_12_C4_GHMN 0x0200

//DK1_Stero_A2 ======================
// 1.25Mhz
#define N_A1_C0_DK1A2 0x03B3
#define N_A1_C1_DK1A2 0x063C
#define N_A1_C2_DK1A2 0x0411
// 1.49Mhz
#define N_A2_C0_DK1A2 0x03AC
#define N_A2_C1_DK1A2 0x063C
#define N_A2_C2_DK1A2 0x0418
// 9.25Mhz
#define S_11_C0_DK1A2 0x00D7
#define S_11_C1_DK1A2 0x063C
#define S_11_C2_DK1A2 0x0200
#define S_11_C3_DK1A2 0x071B
#define S_11_C4_DK1A2 0x0200
// 1.25Mhz
#define S_12_C0_DK1A2 0x03B3
#define S_12_C1_DK1A2 0x063C
#define S_12_C2_DK1A2 0x0200
#define S_12_C3_DK1A2 0x0411
#define S_12_C4_DK1A2 0x0200

//DK2_Stero_A2 ======================
// 1.25Mhz
#define N_A1_C0_DK2A2 0x03B3
#define N_A1_C1_DK2A2 0x063C
#define N_A1_C2_DK2A2 0x0411
// 1.01Mhz
#define N_A2_C0_DK2A2 0x03B3
#define N_A2_C1_DK2A2 0x063C
#define N_A2_C2_DK2A2 0x040B
// 9.25Mhz
#define S_11_C0_DK2A2 0x00D7
#define S_11_C1_DK2A2 0x063C
#define S_11_C2_DK2A2 0x0200
#define S_11_C3_DK2A2 0x071B
#define S_11_C4_DK2A2 0x0200
// 1.25Mhz
#define S_12_C0_DK2A2 0x03B3
#define S_12_C1_DK2A2 0x063C
#define S_12_C2_DK2A2 0x0200
#define S_12_C3_DK2A2 0x0411
#define S_12_C4_DK2A2 0x0200

//DK3_Stero_A2=====================
// 1.25Mhz
#define N_A1_C0_DK3A2 0x03B3
#define N_A1_C1_DK3A2 0x063C
#define N_A1_C2_DK3A2 0x0411
// 2.01Mhz
#define N_A2_C0_DK3A2 0x039A
#define N_A2_C1_DK3A2 0x063C
#define N_A2_C2_DK3A2 0x042B
// 9.25Mhz
#define S_11_C0_DK3A2 0x00D7
#define S_11_C1_DK3A2 0x063C
#define S_11_C2_DK3A2 0x0200
#define S_11_C3_DK3A2 0x071B
#define S_11_C4_DK3A2 0x0200
// 1.25Mhz
#define S_12_C0_DK3A2 0x03B3
#define S_12_C1_DK3A2 0x063C
#define S_12_C2_DK3A2 0x0200
#define S_12_C3_DK3A2 0x0411
#define S_12_C4_DK3A2 0x0200

//DK_Mono_NICAM===================
// 1.25Mhz
#define N_A1_C0_DKMN 0x03B3
#define N_A1_C1_DKMN 0x063C
#define N_A1_C2_DKMN 0x0411
// 1.9Mhz
#define N_A2_C0_DKMN 0x039E
#define N_A2_C1_DKMN 0x063C
#define N_A2_C2_DKMN 0x0427
// 9.25Mhz
#define S_11_C0_DKMN 0x00D7
#define S_11_C1_DKMN 0x063C
#define S_11_C2_DKMN 0x0200
#define S_11_C3_DKMN 0x071B
#define S_11_C4_DKMN 0x0200
// 1.25Mhz
#define S_12_C0_DKMN 0x03B3
#define S_12_C1_DKMN 0x063C
#define S_12_C2_DKMN 0x0200
#define S_12_C3_DKMN 0x0411
#define S_12_C4_DKMN 0x0200

//Sound : I ===========================
// 1.75Mhz
#define N_A1_C0_I 0x03A4
#define N_A1_C1_I 0x063C
#define N_A1_C2_I 0x0421
// 1.2Mhz
#define N_A2_C0_I 0x03B4
#define N_A2_C1_I 0x063C
#define N_A2_C2_I 0x0410
// 9.75Mhz
#define S_11_C0_I 0x0092
#define S_11_C1_I 0x063C
#define S_11_C2_I 0x0200
#define S_11_C3_I 0x0764
#define S_11_C4_I 0x0200
// 1.75Mhz
#define S_12_C0_I 0x03A4
#define S_12_C1_I 0x063C
#define S_12_C2_I 0x0200
#define S_12_C3_I 0x0421
#define S_12_C4_I 0x0200
//Sound : MN=========================
// 2.25Mhz
#define N_A1_C0_NTSC 0x038F
#define N_A1_C1_NTSC 0x063C
#define N_A1_C2_NTSC 0x0436
// 2.03Mhz
#define N_A2_C0_NTSC 0x0399
#define N_A2_C1_NTSC 0x063C
#define N_A2_C2_NTSC 0x042C
// 8.25Mhz
#define S_11_C0_NTSC 0x015D
#define S_11_C1_NTSC 0x063C
#define S_11_C2_NTSC 0x0200
#define S_11_C3_NTSC 0x068D
#define S_11_C4_NTSC 0x0200
// 2.25Mhz
#define S_12_C0_NTSC 0x038F
#define S_12_C1_NTSC 0x063C
#define S_12_C2_NTSC 0x0200
#define S_12_C3_NTSC 0x0436
#define S_12_C4_NTSC 0x0200

//Sound : L ==========================
// 1.25Mhz
#define N_A1_C0_L 0x03B3
#define N_A1_C1_L 0x063C
#define N_A1_C2_L 0x0411
// 1.9Mhz
#define N_A2_C0_L 0x039E
#define N_A2_C1_L 0x063C
#define N_A2_C2_L 0x0427
// 9.25Mhz
#define S_11_C0_L 0x00D7
#define S_11_C1_L 0x063C
#define S_11_C2_L 0x0200
#define S_11_C3_L 0x071B
#define S_11_C4_L 0x0200
// 1.25Mhz
#define S_12_C0_L 0x03B3
#define S_12_C1_L 0x063C
#define S_12_C2_L 0x0200
#define S_12_C3_L 0x0411
#define S_12_C4_L 0x0200

//End of Table===================

MS_BOOL MDrv_Tuner_MXL661_CheckExist(MS_U8 u8TunerIndex, MS_U32* pu32channel_cnt);
//MS_BOOL MDrv_Tuner_MXL661_GetPeakingParamter(MS_U8 u8TunerIndex, MS_U32 u32Band, MS_U32 u32IfFreq, DEMOD_VIFUserFilter* pstVifUserFilter);

#endif 

