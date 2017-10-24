#ifndef TESTER_PREFS_H
#define TESTER_PREFS_H

/** \file tester_prefs.h
 *  \brief Main file for preferences
 *  \author Olivier DOLE
 *  \date 14/03/06
 *  \version 1.0
 */

/************************************************************************
 ** Parameters that can be changed by user ******************************
 ************************************************************************/
#if !defined(MEDIUM_SAT) && !defined(MEDIUM_TER) && !defined(MEDIUM_CAB)
    #error "a MEDIUM_XX (XX=SAT, TER or CAB) must be defined"
#endif

/*! \include tester_prefs_dmd.h
 */
#include "tester_prefs_dmd.h"

/** REMOTE CONTROLLER **/
#define USE_REMOTE_HAUPPAUGE    0
#define USE_REMOTE_PSC_TT       0
#define USE_REMOTE_PSC_REF      0
#define USE_REMOTE_PSC_CHINA    1

#if ((USE_REMOTE_HAUPPAUGE + USE_REMOTE_PSC_TT + USE_REMOTE_PSC_REF + USE_REMOTE_PSC_CHINA) != 1)
#error  "You should define only one remote controller"
#endif

/** SERVICES **/
#if defined(MEDIUM_SAT)

#if defined(SELECT_DEVICE_SKARDIN)
#define USE_SERVICES_ON_ASTRA    0
#define USE_SERVICES_ON_HOTBIRD  1
#define USE_SERVICES_TERRESTRIAL 0
#else
#define USE_SERVICES_ON_ASTRA    1
#define USE_SERVICES_ON_HOTBIRD  0
#define USE_SERVICES_TERRESTRIAL 0
#endif 

#elif defined(MEDIUM_TER)

#define USE_SERVICES_ON_ASTRA    0
#define USE_SERVICES_ON_HOTBIRD  0
#define USE_SERVICES_TERRESTRIAL 1

#elif defined(MEDIUM_CAB)

#define USE_SERVICES_ON_ASTRA    0
#define USE_SERVICES_ON_HOTBIRD  0
#define USE_SERVICES_TERRESTRIAL 1

#endif

#if ((USE_SERVICES_ON_ASTRA + USE_SERVICES_ON_HOTBIRD + USE_SERVICES_TERRESTRIAL) != 1)
#error  "You should define only one satellite or choose terrestrial"
#endif





/************************************************************************
 ** RC mapping **********************************************************
 ************************************************************************/
#if USE_REMOTE_HAUPPAUGE
    /* TEST_COMMON RC */
    #define TEST_COMMON_KEY_BLUE            0x29
    #define TEST_COMMON_KEY_RED             0x0b
    #define TEST_COMMON_KEY_PROG_PLUS       0x32
    #define TEST_COMMON_KEY_PROG_MIN        0x37
    #define TEST_COMMON_KEY_VOL_PLUS        0x34
    #define TEST_COMMON_KEY_VOL_MIN         0x30
    #define TEST_COMMON_KEY_UP              0x20
    #define TEST_COMMON_KEY_DOWN            0x21
    #define TEST_COMMON_KEY_POWER           0x3d
    #define TEST_COMMON_KEY_OK              0x25
    #define TEST_COMMON_KEY_EXIT            0x1f
    #define TEST_COMMON_REMOTE_NAME         "HAUPPAUGE"
#endif

#if USE_REMOTE_PSC_TT
    /* TEST_COMMON RC */
    #define TEST_COMMON_KEY_BLUE            0x3557
    #define TEST_COMMON_KEY_RED             0x3554
    #define TEST_COMMON_KEY_PROG_PLUS       0x3563
    #define TEST_COMMON_KEY_PROG_MIN        0x3564
    #define TEST_COMMON_KEY_VOL_PLUS        0x3565
    #define TEST_COMMON_KEY_VOL_MIN         0x3566
    #define TEST_COMMON_KEY_UP              0x354D
    #define TEST_COMMON_KEY_DOWN            0x3551
    #define TEST_COMMON_KEY_POWER           0x3541
    #define TEST_COMMON_KEY_OK              0x354F
    #define TEST_COMMON_KEY_EXIT            0x3553
    #define TEST_COMMON_REMOTE_NAME         "PSC_TT"
#endif

#if USE_REMOTE_PSC_REF
    /* TEST_COMMON RC */
    #define TEST_COMMON_KEY_BLUE            0x222E
    #define TEST_COMMON_KEY_RED             0x222B
    #define TEST_COMMON_KEY_PROG_PLUS       0x3220
    #define TEST_COMMON_KEY_PROG_MIN        0x3221
    #define TEST_COMMON_KEY_VOL_PLUS        0x3210
    #define TEST_COMMON_KEY_VOL_MIN         0x3211
    #define TEST_COMMON_KEY_UP              0x2210
    #define TEST_COMMON_KEY_DOWN            0x2211
    #define TEST_COMMON_KEY_POWER           0x320C
    #define TEST_COMMON_KEY_OK              0x2217
    #define TEST_COMMON_KEY_EXIT            0x2213
    #define TEST_COMMON_REMOTE_NAME         "PSC_REF"
#endif

#if USE_REMOTE_PSC_CHINA
    /* TEST_COMMON RC */
    #define TEST_COMMON_KEY_BLUE            0x300F /* CALL */
    #define TEST_COMMON_KEY_RED             0x3012 /* LUM + */
    #define TEST_COMMON_KEY_PROG_PLUS       0x3020
    #define TEST_COMMON_KEY_PROG_MIN        0x3021
    #define TEST_COMMON_KEY_VOL_PLUS        0x3010
    #define TEST_COMMON_KEY_VOL_MIN         0x3011
    #define TEST_COMMON_KEY_UP              0x3014 /* COLOR + */
    #define TEST_COMMON_KEY_DOWN            0x3029 /* STORE */
    #define TEST_COMMON_KEY_POWER           0x300C
    #define TEST_COMMON_KEY_OK              0x300A /* SEL */
    #define TEST_COMMON_KEY_EXIT            0x302B /* TUNE + */
    #define TEST_COMMON_REMOTE_NAME         "PSC_CHINA"
#endif




/************************************************************************
 ** SERVICES INFO *******************************************************
 ************************************************************************/
#if USE_SERVICES_ON_ASTRA

#if defined(MODE_HD)
    #define TEST_COMMON_SERV_1_NAME             "HD PROMO (on ASTRA)"
    #define TEST_COMMON_SERV_1_FREQUENCY        12441
    #define TEST_COMMON_SERV_1_SYMBOLRATE       27500
    #define TEST_COMMON_SERV_1_CONVOLUTION      eTDAL_DMD_FEC_5_6
    #define TEST_COMMON_SERV_1_POLARIZATION     eTDAL_DMD_POLAR_VERTICAL
    #define TEST_COMMON_SERV_1_PMT_PID          5124
    #define TEST_COMMON_SERV_1_VIDEO_PID        133
    #define TEST_COMMON_SERV_1_AUDIO_PID        134
    #define TEST_COMMON_SERV_1_PCR_PID          80

    /* define another service on same transponder than SERV_1 */
    #define TEST_COMMON_SERV_1BIS_NAME          "HD Promo 2 (on ASTRA)"
    #define TEST_PVMMON_SERV_1BIS_PMT_PID       5128
    #define TEST_COMMON_SERV_1BIS_VIDEO_PID     333
    #define TEST_COMMON_SERV_1BIS_AUDIO_PID     334
    #define TEST_COMMON_SERV_1BIS_PCR_PID       82

    #define TEST_COMMON_SERV_2_NAME             "C+ HD Promo (on ASTRA)"
    #define TEST_COMMON_SERV_2_FREQUENCY        12581
    #define TEST_COMMON_SERV_2_SYMBOLRATE       22000
    #define TEST_COMMON_SERV_2_CONVOLUTION      eTDAL_DMD_FEC_5_6
    #define TEST_COMMON_SERV_2_POLARIZATION     eTDAL_DMD_POLAR_VERTICAL
    #define TEST_COMMON_SERV_2_PMT_PID          1285
    #define TEST_COMMON_SERV_2_VIDEO_PID        165
    #define TEST_COMMON_SERV_2_AUDIO_PID        100
    #define TEST_COMMON_SERV_2_PCR_PID          165

    /* define another service on same transponder than SERV_2 */
    #define TEST_COMMON_SERV_2BIS_NAME          "RTL 2 (on ASTRA)"
    #define TEST_COMMON_SERV_2BIS_PMT_PID       45
    #define TEST_COMMON_SERV_2BIS_VIDEO_PID     166
    #define TEST_COMMON_SERV_2BIS_AUDIO_PID     128
    #define TEST_COMMON_SERV_2BIS_PCR_PID       166

#else /* MODE_HD */

    #define TEST_COMMON_SERV_1_NAME             "EurVosport !!!! (on ASTRA)"
    #define TEST_COMMON_SERV_1_FREQUENCY        11954
    #define TEST_COMMON_SERV_1_SYMBOLRATE       27500
    #define TEST_COMMON_SERV_1_CONVOLUTION      eTDAL_DMD_FEC_3_4
    #define TEST_COMMON_SERV_1_POLARIZATION     eTDAL_DMD_POLAR_HORIZONTAL
    #define TEST_COMMON_SERV_1_PMT_PID          400
    #define TEST_COMMON_SERV_1_VIDEO_PID        410
    #define TEST_COMMON_SERV_1_AUDIO_PID        420
    #define TEST_COMMON_SERV_1_PCR_PID          410

    /* define another service on same transponder than SERV_1 */
    #define TEST_COMMON_SERV_1BIS_NAME          "Euronews (on ASTRA)"
    #define TEST_PVMMON_SERV_1BIS_PMT_PID       1034
    #define TEST_COMMON_SERV_1BIS_VIDEO_PID     2221
    #define TEST_COMMON_SERV_1BIS_AUDIO_PID     2233
    #define TEST_COMMON_SERV_1BIS_PCR_PID       2221

    #define TEST_COMMON_SERV_2_NAME             "RTL (on ASTRA)"
    #define TEST_COMMON_SERV_2_FREQUENCY        12188
    #define TEST_COMMON_SERV_2_SYMBOLRATE       27500
    #define TEST_COMMON_SERV_2_CONVOLUTION      eTDAL_DMD_FEC_3_4
    #define TEST_COMMON_SERV_2_POLARIZATION     eTDAL_DMD_POLAR_HORIZONTAL
    #define TEST_COMMON_SERV_2_PMT_PID          44
    #define TEST_COMMON_SERV_2_VIDEO_PID        163
    #define TEST_COMMON_SERV_2_AUDIO_PID        104
    #define TEST_COMMON_SERV_2_PCR_PID          163

    /* define another service on same transponder than SERV_2 */
    #define TEST_COMMON_SERV_2BIS_NAME          "RTL 2 (on ASTRA)"
    #define TEST_COMMON_SERV_2BIS_PMT_PID       45
    #define TEST_COMMON_SERV_2BIS_VIDEO_PID     166
    #define TEST_COMMON_SERV_2BIS_AUDIO_PID     128
    #define TEST_COMMON_SERV_2BIS_PCR_PID       166
#endif /* MODE_HD */
#endif


#if USE_SERVICES_ON_HOTBIRD
 
#if defined(SELECT_DEVICE_SKARDIN) 
    #define TEST_COMMON_SERV_1_NAME             "Adjara TV (on HOTBIRD)"
    #define TEST_COMMON_SERV_1_FREQUENCY        10949
    #define TEST_COMMON_SERV_1_SYMBOLRATE       27500
    #define TEST_COMMON_SERV_1_CONVOLUTION      eTDAL_DMD_FEC_3_4
    #define TEST_COMMON_SERV_1_POLARIZATION     eTDAL_DMD_POLAR_VERTICAL
    #define TEST_COMMON_SERV_1_PMT_PID          7405
    #define TEST_COMMON_SERV_1_VIDEO_PID        5401
    #define TEST_COMMON_SERV_1_AUDIO_PID        5411
    #define TEST_COMMON_SERV_1_PCR_PID          5401
#else
    #define TEST_COMMON_SERV_1_NAME             "FashionTV (on HOTBIRD)"
    #define TEST_COMMON_SERV_1_FREQUENCY        12207
    #define TEST_COMMON_SERV_1_SYMBOLRATE       27500
    #define TEST_COMMON_SERV_1_CONVOLUTION      eTDAL_DMD_FEC_3_4
    #define TEST_COMMON_SERV_1_POLARIZATION     eTDAL_DMD_POLAR_HORIZONTAL
    #define TEST_COMMON_SERV_1_PMT_PID          1296
    #define TEST_COMMON_SERV_1_VIDEO_PID        1297
    #define TEST_COMMON_SERV_1_AUDIO_PID        1298
    #define TEST_COMMON_SERV_1_PCR_PID          1302
#endif 

    #define TEST_COMMON_SERV_2_NAME             "Euronews (on HOTBIRD)"
    #define TEST_COMMON_SERV_2_FREQUENCY        11034
    #define TEST_COMMON_SERV_2_SYMBOLRATE       27500
    #define TEST_COMMON_SERV_2_CONVOLUTION      eTDAL_DMD_FEC_3_4
    #define TEST_COMMON_SERV_2_POLARIZATION     eTDAL_DMD_POLAR_VERTICAL
    #define TEST_COMMON_SERV_2_PMT_PID          900
    #define TEST_COMMON_SERV_2_VIDEO_PID        920
    #define TEST_COMMON_SERV_2_AUDIO_PID        930
    #define TEST_COMMON_SERV_2_PCR_PID          920

#endif


#if USE_SERVICES_TERRESTRIAL

#define TEST_COMMON_SERV_1_NAME           "TF1 numerique"
#define TEST_COMMON_SERV_1_FREQUENCY      562000000
#define TEST_COMMON_SERV_1_SYMBOLRATE     8000000
#define TEST_COMMON_SERV_1_CONVOLUTION    eTDAL_DMD_FEC_3_4
#define TEST_COMMON_SERV_1_POLARIZATION   eTDAL_DMD_POLAR_HORIZONTAL
#define TEST_COMMON_SERV_1_VIDEO_PID      0x78
#define TEST_COMMON_SERV_1_AUDIO_PID      0x82
#define TEST_COMMON_SERV_1_PCR_PID        0x78

#define TEST_COMMON_SERV_2_NAME           "France2 numerique"
#define TEST_COMMON_SERV_2_FREQUENCY      586000000
#define TEST_COMMON_SERV_2_SYMBOLRATE     8000000
#define TEST_COMMON_SERV_2_CONVOLUTION    eTDAL_DMD_FEC_3_4
#define TEST_COMMON_SERV_2_POLARIZATION   eTDAL_DMD_POLAR_HORIZONTAL
#define TEST_COMMON_SERV_2_VIDEO_PID      120
#define TEST_COMMON_SERV_2_AUDIO_PID      130
#define TEST_COMMON_SERV_2_PCR_PID        120

#define TEST_COMMON_SERV_3_NAME           "M6 numerique"
#define TEST_COMMON_SERV_3_FREQUENCY      626000000
#define TEST_COMMON_SERV_3_SYMBOLRATE     8000000
#define TEST_COMMON_SERV_3_CONVOLUTION    eTDAL_DMD_FEC_3_4
#define TEST_COMMON_SERV_3_POLARIZATION   eTDAL_DMD_POLAR_HORIZONTAL
#define TEST_COMMON_SERV_3_VIDEO_PID      120
#define TEST_COMMON_SERV_3_AUDIO_PID      130
#define TEST_COMMON_SERV_3_PCR_PID        120

#define TEST_COMMON_SERV_4_NAME           "C+ numerique"
#define TEST_COMMON_SERV_4_FREQUENCY      650000000
#define TEST_COMMON_SERV_4_SYMBOLRATE     8000000
#define TEST_COMMON_SERV_4_CONVOLUTION    eTDAL_DMD_FEC_3_4
#define TEST_COMMON_SERV_4_POLARIZATION   eTDAL_DMD_POLAR_HORIZONTAL
#define TEST_COMMON_SERV_4_VIDEO_PID      160
#define TEST_COMMON_SERV_4_AUDIO_PID      80
#define TEST_COMMON_SERV_4_PCR_PID        160

#define TEST_COMMON_SERV_5_NAME           "Direct8 numerique"
#define TEST_COMMON_SERV_5_FREQUENCY      674000000
#define TEST_COMMON_SERV_5_SYMBOLRATE     8000000
#define TEST_COMMON_SERV_5_CONVOLUTION    eTDAL_DMD_FEC_3_4
#define TEST_COMMON_SERV_5_POLARIZATION   eTDAL_DMD_POLAR_HORIZONTAL
#define TEST_COMMON_SERV_5_VIDEO_PID      160
#define TEST_COMMON_SERV_5_AUDIO_PID      80
#define TEST_COMMON_SERV_5_PCR_PID        160

#endif

/************************************************************************
 ** File system locations ***********************************************
 ************************************************************************/
#define FS_HDD_LOCATION "/root/sdx1"

#endif //TESTER_PREFS_H
