#include "platform_dibcom.h"
#include "dibcom_i2c_app.h"
#include "dibcom8090.h"

#define pgm_read_word(w) (*w)

#define BAND_OF_FREQUENCY(freq_kHz) channel_frequency_band(freq_kHz)
/* useful tools */
#ifndef ARRAY_SIZE
#define ARRAY_SIZE(v)  sizeof(v)/sizeof(*(v))
#endif

//********************DC Calib ***************************
#define steps(u) (((u)>15)?((u)-16):(u))
#define INTERN_WAIT 10


struct dibFrontend fe;
struct dib0090_state state_dib0090;
struct dib8000_state state_dib8000;
struct dib8090_state state_dib8090;

struct dib8090_config nim_dtv_lt_8096_config = {
    1,
    //NULL, // update_lna

    DIB8090_GPIO_DEFAULT_DIRECTIONS,
    DIB8090_GPIO_DEFAULT_VALUES,
    DIB8090_GPIO_DEFAULT_PWM_POS,

    -143, // dib0090_freq_offset_khz_uhf
    -143, // dib0090_freq_offset_khz_vhf

    0,
    12000, // clock_khz
    4,
    0,
    0,
    0,
    0,

    0x2d00, // dib8k_drives
    144,   //diversity_delay
    0x31,  //div_cfg
    1, //clkouttobamse
    1, // clkoutdrive
    3, // refclksel
    6, // fref_clock_ratio
    0x3f, // cap_value
};

struct dib8090_wbd_target nim_dtv_lt_8096_wbd_target[] = {
    { 120, 500 }, // low CBAND
    { 170, 450 }, // high CBAND
    { 380, 125 }, // VHF
    { 600, 350 }, // low UHF
    { 860, 250 }, // high UHF
    { 0xffff, 250 }, // default
};

struct dib8096_adc nim_dtv_lt_8096_adc_tab[] = {
    {174143, 18179756, 20},
    {180143, 19136583, 19},
    {184143, 20199727, 18},
    {186143, 21387946, 17},
    {190143, 18179756, 20},
    {192143, 19136583, 19},
    {196143, 20199727, 18},
    {202143, 18179756, 20},
    {208143, 19136583, 19},
    {214143, 21387946, 17},
    {220143, 18179756, 20},
    {226143, 20199727, 18},
    {232143, 18179756, 20},

    {474143, 18179756, 20},
    {480143, 20199727, 18},
    {486143, 18179756, 20},
    {492143, 21387946, 17},
    {504143, 18179756, 20},
    {510143, 20199727, 18},
    {522143, 18179756, 20},
    {528143, 21387946, 17},
    {534143, 20199727, 18},
    {540143, 21387946, 17},
    {546143, 20199727, 18},
    {552143, 18179756, 20},
    {558143, 20199727, 18},
    {564143, 18179756, 20},
    {570143, 21387946, 17},
    {576143, 20199727, 18},
    {582143, 21387946, 17},
    {588143, 20199727, 18},
    {594143, 21387946, 17},
    {600143, 20199727, 18},
    {606143, 21387946, 17},
    {612143, 18179756, 20},
    {618143, 21387946, 17},
    {624143, 18179756, 20},
    {630143, 20199727, 18},
    {636143, 18179756, 20},
    {642143, 20199727, 18},
    {648143, 21387946, 17},
    {654143, 18179756, 20},
    {660143, 21387946, 17},
    {666143, 18179756, 20},
    {672143, 21387946, 17},
    {678143, 19136583, 19},
    {684143, 18179756, 20},
    {690143, 19136583, 19},
    {702143, 18179756, 20},
    {708143, 20199727, 18},
    {714143, 18179756, 20},
    {720143, 21387946, 17},
    {726143, 20199727, 18},
    {732143, 21387946, 17},
    {738143, 20199727, 18},
    {744143, 18179756, 20},
    {750143, 20199727, 18},
    {756143, 18179756, 20},
    {762143, 20199727, 18},
    {768143, 20199727, 18},
    {774143, 18179756, 20},
    {780143, 20199727, 18},
    {786143, 21387946, 17},
    {792143, 19136583, 19},
    {798143, 21387946, 17},
    {0xffffffff, 20199727, 18},
};

struct dib0090_io_config dibx09x_io_12mhz_120 = {
    12000,

    0,
    1,
    1,
    20, // loop div

    8, // ADC clock ratio
};

MS_U16 dib0090_defaults[] = {
    // RF INITIALISATION + BB
    25, CTRL_BB_1, // nb of values, start register
        0x0000, // 0x01  CTRL_BB_1
        0x99a0,   // 0x02  CTRL_BB_2 Captrim
        0x6008, // 0x03  CTRL_BB_3
        0x0000, // 0x04  CTRL_BB_4 DC servo loop 1KHz
        0x8acb, // 0x05  CTRL_BB_5
        0x0000, // 0x06  CTRL_BB_6
        0x0405, // 0x07  CTRL_BB_7
        0x0000, // 0x08  CTRL_RF_1
        0x0000, // 0x09  CTRL_RF_2 Bias and Tune not set
        0x0000, // 0x0a	 CTRL_RF_3
        0xb802, // 0x0b	 CTRL_RF_SW
        0x0300, // 0x0c  CTRL_RF_V2I
        0x2d12, // 0x0d	 CTRL_RF_MIX
        0xbac0, // 0x0e  CTRL_LOAD
        0x7c00, // 0x0f	 CTRL_LT //CAB
        0xdbb9, // 0x10  CTRL_WBDMUX   // WBD MUX TOWARD BB
        0x0954, // 0x11  CTRL_TX
        0x0743,	// 0x12  IQ_ADC
        0x8000,	// 0x13	 CTRL_BIAS 0x4106 Internal bias
        0x0001, // 0x14  CTRL_CRYSTAL
        0x0040, // 0x15  CTRL_LO_1
        0x0100, // 0x16  CTRL_LO_2
        0x0000, // 0x17  CTRL_LO_3
        0xe910, // 0x18  CTRL_LO_4
        0x149e, // 0x19  CTRL_LO_5 1734

    1, CTRL_LO_6,
        0xff2d,	// 0x1a  CTRL_LO_6

    1, BB_RAMP7,
        0x0000,  // deactivate the ramp and ramp_mux to adjust the gain manually

    1, SLEEP_EN,
		EN_IQADC | EN_BB | EN_BIAS | EN_DIGCLK | EN_PLL | EN_CRYSTAL, //0x1b SLEEP_EN
    2, ADCCLK,
        0x07FF,	// ADC_CLK
        0x0007, // VGA_MODE

    1, TUNER_EN,
	EN_UHF | EN_CRYSTAL, // 0x33 TUNER_EN

    2, PWM1_REG,
	0x3ff, // pwm accu period
	0x111, // (alpha_bb<<8)|(resetn_pwm<<4)|(alpha_rf)
    0
};

struct dib0090_tuning dib0090_tuning_table[] = {
    //max_freq, switch_trim, lna_tune, lna_bias, v2i, mix, load, tuner_enable;
#ifdef CONFIG_BAND_CBAND
    { 170000,  4, 1, 15, 0x280, 0x2912, 0xb94e, EN_CAB }, // FM EN_CAB
#endif
#ifdef CONFIG_BAND_VHF
    { 184000,  1, 1, 15, 0x300, 0x4d12, 0xb94e, EN_VHF }, // VHF EN_VHF
    { 227000,  1, 3, 15, 0x300, 0x4d12, 0xb94e, EN_VHF }, // VHF EN_VHF
    { 380000,  1, 7, 15, 0x300, 0x4d12, 0xb94e, EN_VHF }, // VHF EN_VHF

#endif
#ifdef CONFIG_BAND_UHF
    { 520000,  2, 0, 15, 0x300, 0x1d12, 0xb9ce, EN_UHF }, // UHF
    { 550000,  2, 2, 15, 0x300, 0x1d12, 0xb9ce, EN_UHF }, // UHF
    { 650000,  2, 3, 15, 0x300, 0x1d12, 0xb9ce, EN_UHF }, // UHF
    { 750000,  2, 5, 15, 0x300, 0x1d12, 0xb9ce, EN_UHF }, // UHF
    { 850000,  2, 6, 15, 0x300, 0x1d12, 0xb9ce, EN_UHF }, // UHF
    { 900000,  2, 7, 15, 0x300, 0x1d12, 0xb9ce, EN_UHF }, // UHF
#endif
#ifdef CONFIG_BAND_LBAND
    { 1500000, 4, 0, 20, 0x300, 0x1912, 0x82c9, EN_LBD }, // LBD EN_LBD
    { 1600000, 4, 1, 20, 0x300, 0x1912, 0x82c9, EN_LBD }, // LBD EN_LBD
    { 1800000, 4, 3, 20, 0x300, 0x1912, 0x82c9, EN_LBD }, // LBD EN_LBD
#endif
#ifdef CONFIG_BAND_SBAND
    { 2300000, 1, 4, 20, 0x300, 0x2d2A, 0x82c7, EN_SBD }, // SBD EN_SBD
    { 2900000, 1, 7, 20, 0x280, 0x2deb, 0x8347, EN_SBD }, // SBD EN_SBD
#endif
};

struct dib0090_pll dib0090_pll_table[] = {
#ifdef CONFIG_BAND_CBAND
	{ 56000, 0, 9, 48, 6},	// CAB
	{ 70000, 1, 9, 48, 6},	// CAB
	{ 87000, 0, 8, 32, 4},	// CAB
	{ 105000, 1, 8, 32, 4}, // FM
	{ 115000, 0, 7, 24, 6}, // FM
	{ 140000, 1, 7, 24, 6}, // MID FM VHF
    { 170000, 0, 6, 16, 4}, // MID FM VHF
#endif
#ifdef CONFIG_BAND_VHF
    { 200000, 1, 6, 16, 4}, // VHF
    { 230000, 0, 5, 12, 6}, // VHF
    { 280000, 1, 5, 12, 6}, // MID VHF UHF
	{ 340000, 0, 4, 8, 4},  // MID VHF UHF
	{ 380000, 1, 4, 8, 4},  // MID VHF UHF
	{ 450000, 0, 3, 6, 6},  // MID VHF UHF
#endif
#ifdef CONFIG_BAND_UHF
    { 580000, 1, 3,  6, 6}, // UHF
    { 700000, 0, 2,  4, 4}, // UHF
    { 860000, 1, 2,  4, 4}, // UHF
#endif
#ifdef CONFIG_BAND_LBAND
    { 1800000, 1, 0,  2, 4}, // LBD
#endif
#ifdef CONFIG_BAND_SBAND
    { 2900000, 0, 14, 1, 4}, // SBD
#endif
};

/* this array only covers the variable part of the wbd values, above the floor */
struct slope dib0090_wbd_slopes[3] = {
    {  66, 120 }, /* -64,-52: offset -   65 */
    { 600, 170 }, /* -52,-35: 65     -  665 */
    { 170, 250 }, /* -45,-10: 665    - 835 */
};

MS_U16 dib8000_defaults[] = {
	/* auto search configuration - lock0 by default waiting
	 * for cpil_lock; lock1 cpil_lock; lock2 tmcc_sync_lock */
	3, 7,
		0x0004,
		0x0400,
		0x0814,

	12, 11,
		0x001b,
		0x7740,
		0x005b,
		0x8d80,
		0x01c9,
		0xc380,
		0x0000,
		0x0080,
		0x0000,
		0x0090,
		0x0001,
		0xd4c0,

	/*1, 32,
		0x6680 // P_corm_thres Lock algorithms configuration */

	11, 80, /* set ADC level to -16 */
		(1 << 13) - 825 - 117,
		(1 << 13) - 837 - 117,
		(1 << 13) - 811 - 117,
		(1 << 13) - 766 - 117,
		(1 << 13) - 737 - 117,
		(1 << 13) - 693 - 117,
		(1 << 13) - 648 - 117,
		(1 << 13) - 619 - 117,
		(1 << 13) - 575 - 117,
		(1 << 13) - 531 - 117,
		(1 << 13) - 501 - 117,

        4, 108,
                0,
                0,
                0,
                0,

	1, 175,
		0x0410,
	1, 179,
		8192, // P_fft_nb_to_cut

	6, 181,
		0x2800, // P_coff_corthres_ ( 2k 4k 8k ) 0x2800
		0x2800,
		0x2800,
		0x2800, // P_coff_cpilthres_ ( 2k 4k 8k ) 0x2800
		0x2800,
		0x2800,

	2, 193,
		0x0666, // P_pha3_thres
		0x0000, // P_cti_use_cpe, P_cti_use_prog

	2, 205,
		0x200f, // P_cspu_regul, P_cspu_win_cut
		0x000f, // P_des_shift_work

	5, 215,
		0x023d, // P_adp_regul_cnt
		0x00a4, // P_adp_noise_cnt
		0x00a4, // P_adp_regul_ext
		0x7ff0, // P_adp_noise_ext
		0x3ccc, // P_adp_fil

	1, 230,
		0x0000, // P_2d_byp_ti_num

	1, 263,
		0x800, //P_equal_thres_wgn

	1, 268,
		(2 << 9) | 39, // P_equal_ctrl_synchro, P_equal_speedmode

	1, 270,
		0x0001, // P_div_lock0_wait
	1, 285,
		0x0020, //p_fec_
	1, 299,
		0x0062, // P_smo_mode, P_smo_rs_discard, P_smo_fifo_flush, P_smo_pid_parse, P_smo_error_discard

	1, 338,
		(1 << 12) | // P_ctrl_corm_thres4pre_freq_inh=1
                (1 << 10) | // P_ctrl_pre_freq_mode_sat=1
                (0 <<  9) | // P_ctrl_pre_freq_inh=0
                (3 <<  5) | // P_ctrl_pre_freq_step=3
                (1 <<  0),  // P_pre_freq_win_len=1

	1, 903,
		(0 << 4) | 2, // P_divclksel=0 P_divbitsel=2 (was clk=3,bit=1 for MPW)

	0,
};

struct dib8090_wbd_target dib8090_wbd_target_default[] = {
    { 170, 750 }, // CBAND
    { 380, 125 }, // VHF
    { 600, 350 }, // low UHF
    { 860, 250 }, // high UHF
    { 0xffff, 250 }, // default
};

struct dibx000_bandwidth_config dib8090_pll_config_12mhz = {
    54000, 13500, // internal, sampling
    1, 18, 3, 1, 0, // pll_cfg: prediv, ratio, range, reset, bypass
    0, 0, 1, 1, 2, // misc: refdiv, bypclk_div, IO_CLK_en_core, ADClkSrc, modulo
    (3 << 14) | (1 << 12) | (599 << 0), // sad_cfg: refsel, sel, freq_15k
    (0 << 25) | 0, // ifreq = 0 MHz
    20199727, // timf
    12000000, // xtal_hz
};

struct dibx000_agc_config dib8090_agc_config[2] = {
    {
        BAND_UHF | BAND_VHF | BAND_LBAND | BAND_SBAND,
        /* P_agc_use_sd_mod1=0, P_agc_use_sd_mod2=0, P_agc_freq_pwm_div=1, P_agc_inv_pwm1=0, P_agc_inv_pwm2=0,
         * P_agc_inh_dc_rv_est=0, P_agc_time_est=3, P_agc_freeze=0, P_agc_nb_est=5, P_agc_write=0 */
        (0 << 15) | (0 << 14) | (5 << 11) | (0 << 10) | (0 << 9) | (0 << 8) | (3 << 5) | (0 << 4) | (5 << 1) | (0 << 0), // setup

        787,// inv_gain = 1/ 90.4dB // no boost, lower gain due to ramp quantification
        10,  // time_stabiliz

        0,  // alpha_level
        118,  // thlock

        0,     // wbd_inv
        3530,  // wbd_ref
        1,     // wbd_sel
        5,     // wbd_alpha

#if 0
        27265,  // agc1_max     = 0.415 == 27229 to 27265 = 28 steps
        27265,  // agc1_min
#endif

        65535,  // agc1_max
        0,  // agc1_min

        65535,  // agc2_max
        0,      // agc2_min

        0,      // agc1_pt1
        32,     // agc1_pt2
        114,    // agc1_pt3  // 40.4dB
        143,    // agc1_slope1
        144,    // agc1_slope2
        114,    // agc2_pt1
        227,    // agc2_pt2
        116,    // agc2_slope1
        117,    // agc2_slope2

        28,  // alpha_mant // 5Hz with 90.2dB
        26,  // alpha_exp
        31,  // beta_mant
        51,  // beta_exp

        0,  // perform_agc_softsplit
    },
    {
        BAND_CBAND,
        /* P_agc_use_sd_mod1=0, P_agc_use_sd_mod2=0, P_agc_freq_pwm_div=1, P_agc_inv_pwm1=0, P_agc_inv_pwm2=0,
         * P_agc_inh_dc_rv_est=0, P_agc_time_est=3, P_agc_freeze=0, P_agc_nb_est=5, P_agc_write=0 */
        (0 << 15) | (0 << 14) | (5 << 11) | (0 << 10) | (0 << 9) | (0 << 8) | (3 << 5) | (0 << 4) | (5 << 1) | (0 << 0), // setup

        787,// inv_gain = 1/ 90.4dB // no boost, lower gain due to ramp quantification
        10,  // time_stabiliz

        0,  // alpha_level
        118,  // thlock

        0,     // wbd_inv
        3530,  // wbd_ref
        1,     // wbd_sel
        5,     // wbd_alpha

#if 0
        27265,  // agc1_max     = 0.415 == 27229 to 27265 = 28 steps
        27265,  // agc1_min
#endif

        0,  // agc1_max
        0,  // agc1_min

        65535,  // agc2_max
        0,      // agc2_min

        0,      // agc1_pt1
        32,     // agc1_pt2
        114,    // agc1_pt3  // 40.4dB
        143,    // agc1_slope1
        144,    // agc1_slope2
        114,    // agc2_pt1
        227,    // agc2_pt2
        116,    // agc2_slope1
        117,    // agc2_slope2

        28,  // alpha_mant // 5Hz with 90.2dB
        26,  // alpha_exp
        31,  // beta_mant
        51,  // beta_exp

        0,  // perform_agc_softsplit
    }
};

struct dib0090_config default_dib0090_config = {
    { 0 },

    0,  // freq_offset_khz_uhf
    0,  // freq_offset_khz_vhf

    0,  // clkouttobamse
    0,  // analog_output
    192, // i2c_address
    100, // wbd_vhf_offset
    500, // wbd_cband_offset
    1,  // use_pwm_age
    7   // clkoutdrive
};

struct dib8000_config default_dib8000_config = {
    0, // output_mpeg2_in_188_bytes
    0, // hostbus_diversity
    //NULL, // LNA update

    2, // agc_config_count
    dib8090_agc_config,
    NULL,

    DIB8000_GPIO_DEFAULT_DIRECTIONS, // gpio_dir
    DIB8000_GPIO_DEFAULT_VALUES,  // gpio_val
    DIB8000_GPIO_DEFAULT_PWM_POS, // gpio_pwm_pos
    0,

    //dib8090_agc_control,
};

MS_U16 rf_ramp_sband[] = {
    253, /* max RF gain in 10th of dB */
    141, 141, 127, /* LNA1,  14.1dB */
    112, 253, 255, /* LNA2,  11.2dB */
      0,   0, 127, /* LNA3,  0dB */
      0,   0, 127, /* LNA4,  0dB */
      0,   0, 127, /* CBAND, 0dB */
};

MS_U16 rf_ramp_uhf[] = {
    412, /* max RF gain in 10th of dB */
    132,132, 127, /* LNA1  : total gain = 13.2dB, point on the ramp where this amp is full gain, value to write to get full gain */
    105,412, 255, /* LNA2  : 10.5 dB */
     50,307, 127, /* LNA3  :  5.0 dB */
    125,257, 127, /* LNA4  : 12.5 dB */
      0,   0, 127, /* CBAND :  0.0 dB */
};

MS_U16 rf_ramp_vhf[] = {
    412, /* max RF gain in 10th of dB */
    132, 132, 127, /* LNA1,  13.2dB */
    105, 412, 255, /* LNA2,  10.5dB */
     50, 307, 127, /* LNA3,  5dB */
    125, 257, 127, /* LNA4,  12.5dB */
      0,   0, 127, /* CBAND, 0dB */
};

MS_U16 rf_ramp_cband[] = {
   332, /* max RF gain in 10th of dB */
   132, 132, 127, /* LNA1,  dB */
   80,  332, 255, /* LNA2,  dB */
   0,   0,   127, /* LNA3,  dB */
   0,   0,   127, /* LNA4,  dB */
   120, 242, 127, /* LT1 CBAND */
};

MS_U16 rf_ramp_pwm_uhf[] = {
    398, /* max RF gain in 10th of dB */
    24, /* ramp_slope = 1dB of gain -> clock_ticks_per_db = clk_khz / ramp_slope -> RF_RAMP2 */
    954, /* ramp_max = maximum X used on the ramp */
    (7  << 10) | 0, /* RF_RAMP3, LNA 1 = 13.2dB */
    (0  << 10) | 290, /* RF_RAMP4, LNA 1 */
    (16 << 10) | 699, /* RF_RAMP5, LNA 2 = 10.5dB */
    (0  << 10) | 954, /* RF_RAMP6, LNA 2 */
    (17 << 10) | 580, /* RF_RAMP7, LNA 3 = 5dB */
    (0  << 10) | 699, /* RF_RAMP8, LNA 3 */
    (7  << 10) | 290, /* GAIN_4_1, LNA 4 = 12.5dB */
    (0  << 10) | 580, /* GAIN_4_2, LNA 4 */
};

MS_U16 rf_ramp_pwm_sband[] = {
    253, /* max RF gain in 10th of dB */
    38, /* ramp_slope = 1dB of gain -> clock_ticks_per_db = clk_khz / ramp_slope -> RF_RAMP2 */
    961,
    (4  << 10) | 0, /* RF_RAMP3, LNA 1 = 14.1dB */
    (0  << 10) | 508, /* RF_RAMP4, LNA 1 */
    (9  << 10) | 508, /* RF_RAMP5, LNA 2 = 11.2dB */
    (0  << 10) | 961, /* RF_RAMP6, LNA 2 */
    (0  << 10) | 0, /* RF_RAMP7, LNA 3 = 0dB */
    (0  << 10) | 0, /* RF_RAMP8, LNA 3 */
    (0  << 10) | 0, /* GAIN_4_1, LNA 4 = 0dB */
    (0  << 10) | 0, /* GAIN_4_2, LNA 4 */
};

MS_U16 rf_ramp_pwm_cband[] = {
    0, /* max RF gain in 10th of dB */
    0, /* ramp_slope = 1dB of gain -> clock_ticks_per_db = clk_khz / ramp_slope -> RF_RAMP2 */
    0, /* ramp_max = maximum X used on the ramp */
    (0  << 10) | 0, /* RF_RAMP3, LNA 1 = 0dB */
    (0  << 10) | 0, /* RF_RAMP4, LNA 1 */
    (0  << 10) | 0, /* RF_RAMP5, LNA 2 = 0dB */
    (0  << 10) | 0, /* RF_RAMP6, LNA 2 */
    (0  << 10) | 0, /* RF_RAMP7, LNA 3 = 0dB */
    (0  << 10) | 0, /* RF_RAMP8, LNA 3 */
    (0  << 10) | 0, /* GAIN_4_1, LNA 4 = 0dB */
    (0  << 10) | 0, /* GAIN_4_2, LNA 4 */
};

MS_U16 rf_ramp_pwm_vhf[] = {
    398, /* max RF gain in 10th of dB */
    24, /* ramp_slope = 1dB of gain -> clock_ticks_per_db = clk_khz / ramp_slope -> RF_RAMP2 */
    954, /* ramp_max = maximum X used on the ramp */
    (7  << 10) | 0, /* RF_RAMP3, LNA 1 = 13.2dB */
    (0  << 10) | 290, /* RF_RAMP4, LNA 1 */
    (16 << 10) | 699, /* RF_RAMP5, LNA 2 = 10.5dB */
    (0  << 10) | 954, /* RF_RAMP6, LNA 2 */
    (17 << 10) | 580, /* RF_RAMP7, LNA 3 = 5dB */
    (0  << 10) | 699, /* RF_RAMP8, LNA 3 */
    (7  << 10) | 290, /* GAIN_4_1, LNA 4 = 12.5dB */
    (0  << 10) | 580, /* GAIN_4_2, LNA 4 */
};

MS_U16 bb_ramp_boost[] = {
    550, /* max BB gain in 10th of dB */
    260, 260,  26, /* BB1, 26dB */
    290, 550,  29, /* BB2, 29dB */
};

MS_U16 bb_ramp_pwm_boost[] = {
    550, /* max RF gain in 10th of dB */
    8, /* ramp_slope = 1dB of gain -> clock_ticks_per_db = clk_khz / ramp_slope -> BB_RAMP2 */
    440,
    (2  << 9) | 0, /* BB_RAMP3 = 26dB */
    (0  << 9) | 208, /* BB_RAMP4 */
    (2  << 9) | 208, /* BB_RAMP5 = 29dB */
    (0  << 9) | 440, /* BB_RAMP6 */
};

MS_U16 bb_ramp_pwm_normal[] = {
    500, /* max RF gain in 10th of dB */
    8, /* ramp_slope = 1dB of gain -> clock_ticks_per_db = clk_khz / ramp_slope -> BB_RAMP2 */
    400,
    (2  << 9) | 0, /* BB_RAMP3 = 21dB */
    (0  << 9) | 168, /* BB_RAMP4 */
    (2  << 9) | 168, /* BB_RAMP5 = 29dB */
    (0  << 9) | 400, /* BB_RAMP6 */
};

MS_S16 coeff_2k_sb_1seg_dqpsk[8] = {
  (769 << 5) | 0x0a, (745 << 5) | 0x03, (595 << 5) | 0x0d, (769 << 5) | 0x0a, (920 << 5) | 0x09, (784 << 5) | 0x02, (519 << 5) | 0x0c, (920 << 5) | 0x09
};

MS_S16 coeff_2k_sb_1seg[8] = {
  (692 << 5) | 0x0b, (683 << 5) | 0x01, (519 << 5) | 0x09, (692 << 5) | 0x0b, 0 | 0x1f, 0 | 0x1f, 0 | 0x1f, 0 | 0x1f
};

MS_S16 coeff_2k_sb_3seg_0dqpsk_1dqpsk[8] = {
  (832 << 5) | 0x10, (912 << 5) | 0x05, (900 << 5) | 0x12, (832 << 5) | 0x10, (-931 << 5) | 0x0f, (912 << 5) | 0x04, (807 << 5) | 0x11, (-931 << 5) | 0x0f
};

MS_S16 coeff_2k_sb_3seg_0dqpsk[8] = {
  (622 << 5) | 0x0c, (941 << 5) | 0x04, (796 << 5) | 0x10, (622 << 5) | 0x0c, (982 << 5) | 0x0c, (519 << 5) | 0x02, (572 << 5) | 0x0e, (982 << 5) | 0x0c
};

MS_S16 coeff_2k_sb_3seg_1dqpsk[8] = {
  (699 << 5) | 0x14, (607 << 5) | 0x04, (944 << 5) | 0x13, (699 << 5) | 0x14, (-720 << 5) | 0x0d, (640 << 5) | 0x03, (866 << 5) | 0x12, (-720 << 5) | 0x0d
};

MS_S16 coeff_2k_sb_3seg[8] = {
  (664 << 5) | 0x0c, (925 << 5) | 0x03, (937 << 5) | 0x10, (664 << 5) | 0x0c, (-610 << 5) | 0x0a, (697 << 5) | 0x01, (836 << 5) | 0x0e, (-610 << 5) | 0x0a
};

MS_S16 coeff_4k_sb_1seg_dqpsk[8] = {
  (-955 << 5) | 0x0e, (687 << 5) | 0x04, (818 << 5) | 0x10, (-955 << 5) | 0x0e, (-922 << 5) | 0x0d, (750 << 5) | 0x03, (665 << 5) | 0x0f, (-922 << 5) | 0x0d
};

MS_S16 coeff_4k_sb_1seg[8] = {
  (638 << 5) | 0x0d, (683 << 5) | 0x02, (638 << 5) | 0x0d, (638 << 5) | 0x0d, (-655 << 5) | 0x0a, (517 << 5) | 0x00, (698 << 5) | 0x0d, (-655 << 5) | 0x0a
};

MS_S16 coeff_4k_sb_3seg_0dqpsk_1dqpsk[8] = {
  (-707 << 5) | 0x14, (910 << 5) | 0x06, (889 << 5) | 0x16, (-707 << 5) | 0x14, (-958 << 5) | 0x13, (993 << 5) | 0x05, (523 << 5) | 0x14, (-958 << 5) | 0x13
};

MS_S16 coeff_4k_sb_3seg_0dqpsk[8] = {
  (-723 << 5) | 0x13, (910 << 5) | 0x05, (777 << 5) | 0x14, (-723 << 5) | 0x13, (-568 << 5) | 0x0f, (547 << 5) | 0x03, (696 << 5) | 0x12, (-568 << 5) | 0x0f
};

MS_S16 coeff_4k_sb_3seg_1dqpsk[8] = {
  (-940 << 5) | 0x15, (607 << 5) | 0x05, (915 << 5) | 0x16, (-940 << 5) | 0x15, (-848 << 5) | 0x13, (683 << 5) | 0x04, (543 << 5) | 0x14, (-848 << 5) | 0x13
};

MS_S16 coeff_4k_sb_3seg[8] = {
  (612 << 5) | 0x12, (910 << 5) | 0x04, (864 << 5) | 0x14, (612 << 5) | 0x12, (-869 << 5) | 0x13, (683 << 5) | 0x02, (869 << 5) | 0x12, (-869 << 5) | 0x13
};

MS_S16 coeff_8k_sb_1seg_dqpsk[8] = {
  (-835 << 5) | 0x12, (684 << 5) | 0x05, (735 << 5) | 0x14, (-835 << 5) | 0x12, (-598 << 5) | 0x10, (781 << 5) | 0x04, (739 << 5) | 0x13, (-598 << 5) | 0x10
};

MS_S16 coeff_8k_sb_1seg[8] = {
  (673 << 5) | 0x0f, (683 << 5) | 0x03, (808 << 5) | 0x12, (673 << 5) | 0x0f, (585 << 5) | 0x0f, (512 << 5) | 0x01, (780 << 5) | 0x0f, (585 << 5) | 0x0f
};

MS_S16 coeff_8k_sb_3seg_0dqpsk_1dqpsk[8] = {
  (863 << 5) | 0x17, (930 << 5) | 0x07, (878 << 5) | 0x19, (863 << 5) | 0x17, (0   << 5) | 0x14, (521 << 5) | 0x05, (980 << 5) | 0x18, (0   << 5) | 0x14
};

MS_S16 coeff_8k_sb_3seg_0dqpsk[8] = {
  (-924 << 5) | 0x17, (910 << 5) | 0x06, (774 << 5) | 0x17, (-924 << 5) | 0x17, (-877 << 5) | 0x15, (565 << 5) | 0x04, (553 << 5) | 0x15, (-877 << 5) | 0x15
};

MS_S16 coeff_8k_sb_3seg_1dqpsk[8] = {
  (-921 << 5) | 0x19, (607  << 5) | 0x06, (881  << 5) | 0x19, (-921 << 5) | 0x19, (-921 << 5) | 0x14, (713  << 5) | 0x05, (1018 << 5) | 0x18, (-921  << 5) | 0x14
};

MS_S16 coeff_8k_sb_3seg[8] = {
  (514 << 5) | 0x14, (910 << 5) | 0x05, (861 << 5) | 0x17, (514 << 5) | 0x14, (690 << 5) | 0x14, (683 << 5) | 0x03, (662 << 5) | 0x15, (690 << 5) | 0x14
};

MS_S16 ana_fe_coeff_3seg[24]  = { 81, 80, 78, 74, 68, 61, 54, 45, 37, 28, 19, 11, 4, 1022, 1017, 1013, 1010, 1008, 1008, 1008, 1008, 1010, 1014, 1017 };
MS_S16 ana_fe_coeff_1seg[24]  = { 249, 226, 164, 82, 5, 981, 970, 988, 1018, 20, 31, 26, 8, 1012, 1000, 1018, 1012, 8, 15, 14, 9, 3, 1017, 1003 };
MS_S16 ana_fe_coeff_13seg[24] = { 396, 305, 105, -51, -77, -12, 41, 31, -11, -30, -11, 14, 15, -2, -13, -7, 5, 8, 1, -6, -7, -3, 0, 1 };


MS_S32 lut_1000ln_mant[] = {6908,7003,7090,7170,7244,7313,7377,7438,7495,7549,7600};

int gain_reg_addr[4] = {
    CTRL_RXRF1, CTRL_RXRF3, CTRL_RF_LT, CTRL_BB_1
};


int ABS(int x)
{
    return x < 0 ? -x : x;
}

void init_isdbt_channel(struct dibChannel *ch)
{
    int i=0;

    ch->bandwidth_kHz = 6000;

    ch->u.isdbt.sb_mode             = 0;
    ch->u.isdbt.partial_reception   = -1;
    ch->u.isdbt.nfft                = FFT_AUTO;
    ch->u.isdbt.guard               = GUARD_INTERVAL_AUTO;
    ch->u.isdbt.spectrum_inversion  = -1;
    ch->u.isdbt.sb_conn_total_seg   = 8;
    ch->u.isdbt.sb_wanted_seg       = 0;
    ch->u.isdbt.sb_subchannel       = 22;

    for (i = 0; i < 3; i++) {
        ch->u.isdbt.layer[i].constellation = QAM_AUTO;
        ch->u.isdbt.layer[i].code_rate     = VIT_CODERATE_AUTO;
        ch->u.isdbt.layer[i].time_intlv    = -1;
        ch->u.isdbt.layer[i].nb_segments   = i == 0 ? 13 : 0;
    }
}

void channel_init(struct dibChannel *ch, MS_U8 type)
{
    //DibZeroMemory(ch, sizeof(struct dibChannel));
    ch->type   = type;

    ch->context.status               = CHANNEL_STATUS_PARAMETERS_UNKNOWN;
    ch->context.tune_time_estimation[TUNE_TIME_LOCKED] = 300; // channel_get_default_tune_time(ch->type);
    ch->context.tune_time_estimation[TUNE_TIME_DATA] = ch->context.tune_time_estimation[TUNE_TIME_LOCKED];

    if(STANDARD_ISDBT == type)
        init_isdbt_channel(ch);
 }

void INIT_CHANNEL(struct dibChannel *ch, MS_U8 type)
{
    channel_init(ch, type);
}

int frontend_generic_monitoring(MS_U32 type, MS_U8 *buf, MS_U32 size, MS_U32 offset)
{
    int ret = DIB_RETURN_NOT_SUPPORTED;

	ret = dib8000_generic_monitoring(type, buf, size, offset);
    ret = dib0090_generic_monitoring(type, buf, size, offset);

    return ret;
}

int tuner_wakeup(void)
{
    return dib0090_wakeup();
}

int frontend_wakeup(void)
{
    if(DIB_RETURN_SUCCESS != tuner_wakeup())
    {
        Out(printf("-E- Tuner wake up error\n"));
        return DIB_RETURN_ERROR;
    }

    if(DIB_RETURN_SUCCESS != demod_wakeup())
    {
        Out(printf("-E- Tuner wake up error\n"));
        return DIB_RETURN_ERROR;
    }

    return DIB_RETURN_SUCCESS;
}

int channel_frequency_band(MS_U32 freq_kHz)
{
    if (freq_kHz <= 170000)
        return BAND_CBAND;
    if (freq_kHz <= 115000)
        return BAND_FM;
    if (freq_kHz <= 380000)
        return BAND_VHF;
    if (freq_kHz <= 863000)
        return BAND_UHF;
    if (freq_kHz <= 2000000)
        return BAND_LBAND;
    return BAND_SBAND;
}

/* backward compat */
int tuner_tune(struct dibChannel *ch)
{
    if (state_dib0090.tuner_info.caps & TUNER_CAN_FAST_TUNE) {
        int ret;
        fe.tune_state = CT_TUNER_START;
        do {
            ret = dib0090_tune(ch);
            DibUSleep(ret * 100);
        } while (fe.tune_state != CT_TUNER_STOP);
        return DIB_RETURN_SUCCESS;
    } else
        return dib0090_tune(ch);
    return DIB_RETURN_SUCCESS;
}

struct dibChannelMonitor * channel_frontend_monitoring(struct dibChannel *ch)
{
    return &ch->fe_monitor[0];
}

MS_U16 slopes_to_scale(struct slope *slopes, MS_U8 num, MS_S16 val)
{
    MS_U8 i;
    MS_U16 rest;
    MS_U16 ret = 0;
    for (i = 0; i < num; i++) {
        if (val  > slopes[i].range)
            rest = slopes[i].range;
        else
            rest = val;
        ret += (rest * slopes[i].slope) / slopes[i].range;
        val -= rest;
    }
    return ret;
}

int nim_dtv_lt_8096_agc_tune (struct dibChannel *channel)
{
    struct dibx000_bandwidth_config pll;
    struct dib8096_adc *adc_table = &nim_dtv_lt_8096_adc_tab[0];

    DibZeroMemory(&pll, sizeof(struct dibx000_bandwidth_config));
    if (fe.tune_state == CT_AGC_START) {
        while(channel->RF_kHz > adc_table->freq)
            adc_table++;
        pll.pll_ratio = adc_table->pll_div;
        dib8000_update_pll(pll);
        dib8000_ctrl_timf(DEMOD_TIMF_SET, adc_table->timf);
    }
    return dib8090_agc_startup(channel);
}

MS_U32 frontend_capabilities(void)
{
    MS_U32 c = 0;

    c |= state_dib8000.demod_info.caps;
    return c;
}

int component_ready(MS_U32 t, enum frontend_tune_state st0, enum frontend_tune_state st1)
{
    if (st0 == st1 || (t <= systime() && st0 > st1) || (st0 == CT_SHUTDOWN) )
        return 1;

    return 0;
}

MS_U16 fft_to_mode(struct dibChannel *ch)
{
	MS_U16 mode;
	switch(ch->u.isdbt.nfft) {
		case FFT_2K: mode = 1; break;
		case FFT_4K: mode = 2; break;
		default:
		case FFT_AUTO:
		case FFT_8K: mode = 3; break;
	}
	return mode;
}

#define MAX_AUTOSEARCH_ITERATIONS 10000
/* Do the autosearch with num demods */
int do_fe_autosearch(int num, struct dibChannel *ch)
{
    int ret,i=0,k,stat = 0, num_failed = 0, succeeded = 0;

    if ((ret = demod_autosearch_start_ex(ch)) != 0) {
        Out(printf("starting autosearch failed for demod %d\n",i));
        return ret;
    }

    for (i = 0; !succeeded && num != num_failed && i < MAX_AUTOSEARCH_ITERATIONS; i++) {
        for (k = 0; k < num; k++) {
            if (!(stat & (1 << k))) { // if not already excluded from autosearch
                switch (demod_autosearch_irq()) {
                    case 0: break;
                    case 2:
                            Out(printf("Autosearch succeeded for demod %d - done.\n",k));
                            succeeded = 1;
                            break;
                    default:
                            Out(printf("Autosearch IRQ return unhandled value for demod %d.\n",k));
                    case 1: /* fall throught wanted */
                            Out(printf("Autosearch failed for demod %d - exluding from list.\n",k));
                            stat |= (1 << k);
                            num_failed++;
                            break;
                }
                if (succeeded)
                    break;
            }
        }
        DibMSleep(1);
    }

    if (!succeeded) {
        Out(printf("Autosearch failed for %d demod(s)\n",num_failed));
        return 1;
    }

    ch->context.status = CHANNEL_STATUS_PARAMETERS_SET;
    return demod_get_channel(ch);
}

void dump_digital_channel_params(struct dibChannel *cd)
{
#ifdef CONFIG_STANDARD_ISDBT
    if (cd->type == STANDARD_ISDBT) {
        int i;
        Out(printf("STANDARD ISDBT: "));
        Out(printf("RF=%dkHz, ",cd->RF_kHz));
            switch (cd->u.isdbt.nfft) {
                    case FFT_8K: Out(printf("8K ")); break;
                    case FFT_4K: Out(printf("4K ")); break;
                    case FFT_2K: Out(printf("2K ")); break;
                    default: Out(printf("FFT_UNK ")); break;
            }

            Out(printf("1/%i  ", 32 / (1 << cd->u.isdbt.guard)));

                if (cd->u.isdbt.spectrum_inversion)
                        Out(printf("inverted  "));
        Out(printf("%s ", cd->u.isdbt.partial_reception ? "PARTIAL_RCPT" : "NATIVE_RCPT"));
        Out(printf("%s ", cd->u.isdbt.sb_mode ? "SB_mode":""));
        for (i = 0; i < 3; i ++) {
            Out(printf("| L%c: nb_seg: %d ", 'a' + i, cd->u.isdbt.layer[i].nb_segments));
            switch (cd->u.isdbt.layer[i].constellation) {
                        case QAM_QPSK: Out(printf("QPSK ")); break;
                        case QAM_16QAM: Out(printf("16QAM ")); break;
                        case QAM_64QAM: Out(printf("64QAM ")); break;
                case QAM_DQPSK: Out(printf("DQPSK")); break;
                        default: Out(printf("QAM_UNK ")); break;
                }
            Out(printf("%d/%d ",cd->u.isdbt.layer[i].code_rate, cd->u.isdbt.layer[i].code_rate+1));
            Out(printf("%d ", cd->u.isdbt.layer[i].time_intlv));
        }
        Out(printf("\n"));
    }
#endif
}

struct dibChannelFEInfo * channel_frontend_info(struct dibChannel *ch)
{
    return &ch->fe_info[0];
}

void frontend_tune_restart(enum restart_tune_process option, struct dibChannel * ch)
{

    struct dibChannelFEInfo *info = channel_frontend_info(ch);

    fe.status = FE_STATUS_TUNE_PENDING;
    switch(option) {
        default:
        case FE_RESTART_TUNE_PROCESS_FROM_TUNER:
            info->tune_time_locked = info->tune_time_estimation[TUNE_TIME_LOCKED] = ch->context.tune_time_estimation[TUNE_TIME_LOCKED];
            info->tune_time_data = info->tune_time_estimation[TUNE_TIME_DATA] = ch->context.tune_time_estimation[TUNE_TIME_DATA];

            info->tune_start_time = fe.time_steady = systime();
            Out(printf("Full restart of fe (from TUNER)\n"));
            fe.tune_state = CT_START;

            state_dib8000.demod_info.callback_time_agc = FE_CALLBACK_TIME_NEVER;
            state_dib8000.demod_info.callback_time = FE_CALLBACK_TIME_NEVER;
            break;
        case FE_RESTART_TUNE_PROCESS_FROM_AGC:
            Out(printf("Partial restart of fe (from AGC only)\n"));
            fe.tune_state = CT_AGC_START;

            state_dib8000.demod_info.callback_time = FE_CALLBACK_TIME_NEVER;
            break;
        case FE_RESTART_TUNE_PROCESS_FROM_DEMOD:
            Out(printf("Partial restart of fe (from DEMOD only)\n"));
            fe.tune_state = CT_DEMOD_START;
            break;
        case FE_SHUTDOWN:
            fe.tune_state = CT_SHUTDOWN;
            break;
    }
}

/* tune (do autosearch in case of unknown parameters) */
void frontend_tune_diversity_sync(int num, struct dibChannel *ch)
{
    int sleep_time, ret;

    dib8000_i2c_gate_ctrl(1);
    if(DIB_RETURN_SUCCESS != frontend_wakeup())
    {
        Out(printf("-E- frontend wake up error\n"));
        return;
    }

    if (DIB_RETURN_SUCCESS !=  tuner_tune(ch))
    {
        Out(printf("Tuner tune_digital failed\n"));
        return;
    }
    /* prepare the agc startup loop */
    demod_agc_restart();

    do {
        sleep_time = -1;
        {
            ret = demod_agc_startup_ex(ch);
            if (ret > sleep_time)
                sleep_time = ret;
        }
        if (sleep_time != -1)
            DibMSleep(sleep_time);
    } while (sleep_time != -1);

    if (ch->context.status == CHANNEL_STATUS_PARAMETERS_UNKNOWN)
        if (do_fe_autosearch(num, ch) != 0) {
            Out(printf("autosearching parameters failed.\n"));
            return;
        }

    dump_digital_channel_params(ch);

    frontend_tune_restart(FE_RESTART_TUNE_PROCESS_FROM_DEMOD, ch);
    if (demod_tune_ex(ch) != 0)
        Out(printf("tuning failed for demod - this is just a warning could be normal in diversity.\n"));

    {
        Out(printf("setting diversity in on for demod \n"));
        demod_set_diversity_in(1);

        Out(printf("setting normal output for demod \n"));
        demod_set_output_mode(OUTMODE_MPEG2_PAR_GATED_CLK);
    }
    dib8000_i2c_gate_ctrl(0);
    return;
}

void HARD_RESET(void)
{
    dib8090_tuner_sleep(0);
    DibMSleep(10);

    dib8090_tuner_reset(1);

    DibMSleep(10);

    dib8090_tuner_reset(0);

    DibMSleep(10);
}

#if 1 // dib0090 source code

void dib0090_dcc_freq(MS_U8 fast)
{
  //dib8000_i2c_gate_ctrl(1);
  if(fast)
    dib0090_write_reg(CTRL_BB_4, 0);//1kHz
  else
    dib0090_write_reg(CTRL_BB_4, 1);//almost frozen
  //dib8000_i2c_gate_ctrl(0);
}

MS_U16 dib0090_find_DC(MS_U8 addr, MS_U8 off, MS_U8 pga)
{
    MS_S16 delta_meas,delta_cur=1023;
    MS_U8 cur=0;
    MS_U16 val;

    DibMSleep(5);

    delta_meas= dib0090_get_offset();
    Out(printf("IQC read=%d,current=%x\n", (MS_U32)delta_meas ,0));

    if ((pga) && (delta_meas<0)) cur=0x10;
    if ((pga==0) && (delta_meas>0)) cur=0x10;

    val = dib0090_read_reg(addr);
    val &= ~(0x1f << off) ; //empty the bits concerned even if NOT necessary

    delta_meas= (MS_U16)(ABS(delta_meas));

    while ((delta_meas<delta_cur) && (steps(cur)<15)) //Will stop when the delta to 0 is increasing
    {delta_cur=delta_meas;
        cur++;
        dib0090_write_reg(addr, val | (cur<<off));
        delta_meas= (MS_U16)(ABS(dib0090_get_offset()));
        //          dbgpl(&dib0090_dbg, "IQC read=%d,current=%x",delta_meas ,cur);
    }
    cur--;
    val |= (cur<<off);
    dib0090_write_reg(addr, val );
    Out(printf("BB Offset Cal,BBreg=%hd,Offset=%hd,Value Set=%hd\n", addr,delta_cur,cur));
    return val;
}

void dib0090_gain_apply(MS_S16 gain_delta, MS_S16 top_delta, MS_U8 force)
{
    MS_U16 rf, bb, ref;
    MS_U16 i, v, gain_reg[4] = { 0 }, gain;
      MS_U16 *g;

    if (force) {
        top_delta *= (1 << WBD_ALPHA);
        gain_delta *= (1 << GAIN_ALPHA);
    }

    if(top_delta >= ((MS_S16)(state_dib0090.rf_ramp[0]<<WBD_ALPHA)-state_dib0090.rf_gain_limit)) /* overflow*/
        state_dib0090.rf_gain_limit = state_dib0090.rf_ramp[0] << WBD_ALPHA;
    else
        state_dib0090.rf_gain_limit += top_delta;
    if (state_dib0090.rf_gain_limit < 0) /*underflow*/
        state_dib0090.rf_gain_limit = 0;

    /* use gain as a temporary variable and correct current_gain */
    gain = ((state_dib0090.rf_gain_limit >> WBD_ALPHA) + state_dib0090.bb_ramp[0]) << GAIN_ALPHA;
    if(gain_delta >= ((MS_S16)gain - state_dib0090.current_gain)) /* overflow*/
	  state_dib0090.current_gain = gain ;
	else
	  state_dib0090.current_gain += gain_delta;
    /* cannot be less than 0 (only if gain_delta is less than 0 we can have current_gain < 0) */
    if (state_dib0090.current_gain < 0)
        state_dib0090.current_gain = 0;

    /* now split total gain to rf and bb gain */
    gain = state_dib0090.current_gain >> GAIN_ALPHA;

    /* requested gain is bigger than rf gain limit - ACI/WBD adjustment */
    if (gain > (state_dib0090.rf_gain_limit >> WBD_ALPHA)) {
        rf = state_dib0090.rf_gain_limit >> WBD_ALPHA;
        bb = gain - rf;
        if (bb > state_dib0090.bb_ramp[0])
            bb = state_dib0090.bb_ramp[0];
    } else { /* high signal level -> all gains put on RF */
        rf = gain;
        bb = 0;
    }

    state_dib0090.gain[0] = rf;
    state_dib0090.gain[1] = bb;

    /* software ramp */
    /* Start with RF gains */
    g = state_dib0090.rf_ramp + 1; /* point on RF LNA1 max gain */
    ref = rf;
    for (i = 0; i < 7; i++) { /* Go over all amplifiers => 5RF amps + 2 BB amps = 7 amps */
        if (g[0] == 0 || ref < (g[1] - g[0])) /* if total gain of the current amp is null or this amp is not concerned because it starts to work from an higher gain value */
            v = 0; /* force the gain to write for the current amp to be null */
        else if (ref >= g[1]) /* Gain to set is higher than the high working point of this amp */
            v = g[2]; /* force this amp to be full gain */
        else /* compute the value to set to this amp because we are somewhere in his range */
            v = ((ref - (g[1] - g[0])) * g[2]) / g[0];

        if (i == 0) /* LNA 1 reg mapping */
            gain_reg[0] = v;
        else if(i == 1) /* LNA 2 reg mapping */
            gain_reg[0] |= v << 7;
        else if(i == 2) /* LNA 3 reg mapping */
            gain_reg[1] = v;
        else if(i == 3) /* LNA 4 reg mapping */
            gain_reg[1] |= v << 7;
        else if(i == 4) /* CBAND LNA reg mapping */
            gain_reg[2] = v | state_dib0090.rf_lt_def;
        else if(i == 5) /* BB gain 1 reg mapping */
            gain_reg[3] = v << 3;
        else if(i == 6) /* BB gain 2 reg mapping */
            gain_reg[3] |= v << 8;

        g += 3; /* go to next gain bloc */

        /* When RF is finished, start with BB */
        if (i == 4) {
            g = state_dib0090.bb_ramp + 1; /* point on BB gain 1 max gain */
            ref = bb;
        }
    }
    gain_reg[3] |= state_dib0090.bb_1_def;
    gain_reg[3] |= ((bb % 10) * 100) / 125;

    /* Write the amplifier regs */
    for (i = 0; i < 4; i++) {
        v = gain_reg[i];
        if (force || state_dib0090.gain_reg[i] != v) {
            state_dib0090.gain_reg[i]  = v;
            dib0090_write_reg(gain_reg_addr[i], v);
        }
    }
}

/* software AGC function */
int dib0090_gain_control(struct dibChannel *ch)
{
    enum frontend_tune_state *tune_state = &fe.tune_state;
    int ret = 10;
    struct dibChannelMonitor *mon = channel_frontend_monitoring(ch);
    MS_U16 wbd_val = 0;
    MS_U8 apply_gain_immediatly = 1;
    MS_S16 wbd_error = 0, adc_error = 0;

    if (*tune_state == CT_AGC_START) {
        state_dib0090.agc_freeze = 0;
        dib0090_write_reg(CTRL_BB_4, 0x0);

#ifdef CONFIG_BAND_SBAND
        if (BAND_OF_FREQUENCY(fe.current_rf) == BAND_SBAND) {
            dib0090_set_rframp(rf_ramp_sband);
            dib0090_set_bbramp(bb_ramp_boost);
        } else
#endif
#ifdef CONFIG_BAND_VHF
        if (BAND_OF_FREQUENCY(fe.current_rf) == BAND_VHF) {
            dib0090_set_rframp(rf_ramp_vhf);
            dib0090_set_bbramp(bb_ramp_boost);
        } else
#endif
#ifdef CONFIG_BAND_CBAND
        if (BAND_OF_FREQUENCY(fe.current_rf) == BAND_CBAND) {
            dib0090_set_rframp(rf_ramp_cband);
            dib0090_set_bbramp(bb_ramp_boost);
        } else
#endif
        {
            dib0090_set_rframp(rf_ramp_uhf);
            dib0090_set_bbramp(bb_ramp_boost);
        }

        // deactivate the ramp generator using PWM control
        dib0090_write_reg(RF_RAMP9, 0);
        dib0090_write_reg(BB_RAMP7, 0);

        dib0090_wbd_target(fe.current_rf);

        state_dib0090.rf_gain_limit = state_dib0090.rf_ramp[0] << WBD_ALPHA;
        state_dib0090.current_gain = ((state_dib0090.rf_ramp[0] + state_dib0090.bb_ramp[0]) / 2) << GAIN_ALPHA;

        *tune_state = CT_AGC_STEP_0;
    }
    else if (!state_dib0090.agc_freeze) {
        MS_S16 wbd = 0, i, cnt;
        int adc;
        wbd_val = dib0090_read_reg(ADCVAL);

        if (*tune_state == CT_AGC_STEP_0)
            cnt = 5;
        else
            cnt = 1;

        for (i = 0; i < cnt; i++) {
            wbd_val  = dib0090_read_reg(ADCVAL);
            wbd += dib0090_wbd_to_db(wbd_val);
        }
        wbd /= cnt;
        wbd_error = state_dib0090.wbd_target - wbd;

        if (*tune_state == CT_AGC_STEP_0) {
            if (wbd_error < 0 && state_dib0090.rf_gain_limit > 0) {
#ifdef CONFIG_BAND_CBAND
                /* in case of CBAND tune reduce first the lt_gain2 before adjusting the RF gain */
                MS_U8 ltg2 = (state_dib0090.rf_lt_def >> 10) & 0x7;
                if (BAND_OF_FREQUENCY(ch->RF_kHz) == BAND_CBAND && ltg2) {
                    ltg2 >>= 1;
                    state_dib0090.rf_lt_def &= ltg2 << 10; /* reduce in 3 steps from 7 to 0 */
                }
#endif
            } else {
                state_dib0090.agc_step = 0;
                *tune_state = CT_AGC_STEP_1;
            }
        } else {
            /* calc the adc power */
            adc = dib8090_get_adc_power();
            adc = (adc * ((MS_S32) 355774) + (((MS_S32)1) << 20)) >> 21;  /* included in [0:-700] */

            adc_error = (MS_S16)( ((MS_S32) ADC_TARGET) - adc);
#ifdef CONFIG_STANDARD_ISDBT
            if ((ch->type == STANDARD_ISDBT)&&(
				 ((ch->u.isdbt.layer[0].nb_segments>0)
					&&((ch->u.isdbt.layer[0].constellation == QAM_64QAM)||(ch->u.isdbt.layer[0].constellation == QAM_16QAM)))
				 ||((ch->u.isdbt.layer[1].nb_segments>0)
					  &&((ch->u.isdbt.layer[1].constellation == QAM_64QAM)||(ch->u.isdbt.layer[1].constellation == QAM_16QAM)))
				 ||((ch->u.isdbt.layer[2].nb_segments>0)
					  &&((ch->u.isdbt.layer[2].constellation == QAM_64QAM)||(ch->u.isdbt.layer[2].constellation == QAM_16QAM)))
				 )
				)
			  adc_error += 60;
#endif

            if (*tune_state == CT_AGC_STEP_1) { /* quickly go to the correct range of the ADC power */
                if ((adc > -290 && adc < -30) || state_dib0090.agc_step++ > 5) {
                        dib0090_write_reg(CTRL_BB_4, 0x01); /*0 = 1KHz ; 1 = 150Hz ; 2 = 50Hz ; 3 = 50KHz ; 4 = servo fast*/

                    *tune_state = CT_AGC_STOP;
                }
            } else {
                /* everything higher than or equal to CT_AGC_STOP means tracking */
                ret = 100; /* 10ms interval */
                apply_gain_immediatly = 0;
            }
        }
    }

    /* monitoring refresh was requested */
    if (mon->refresh & FE_COMPONENT_TUNER) {
        mon->agc_global    = (MS_U16) (((MS_U32) (state_dib0090.current_gain >> GAIN_ALPHA) * 0xffff) / (state_dib0090.rf_ramp[0] + state_dib0090.bb_ramp[0]));
        mon->agc_rf        = (MS_U16) (((MS_U32) state_dib0090.gain[0] * 0xffff) / state_dib0090.rf_ramp[0]);
        mon->agc_bb        = (MS_U16) (((MS_U32) state_dib0090.gain[1] * 0xffff) / state_dib0090.bb_ramp[0]);
        mon->agc_wbd       = wbd_val << 2; /* here wbd is on 10 bits - previously it was on 12 */
        mon->agc_wbd_split = (MS_U16) (((state_dib0090.rf_gain_limit >> WBD_ALPHA) * 0xff) / state_dib0090.rf_ramp[0]);
        mon->refresh &= ~FE_COMPONENT_TUNER;
    }

    /* apply gain */
    if (!state_dib0090.agc_freeze)
        dib0090_gain_apply(adc_error, wbd_error, apply_gain_immediatly);
    return ret;
}

int dib0090_generic_monitoring(MS_U32 cmd, MS_U8 *b, MS_U32 size, MS_U32 offset)
{
    int ret = DIB_RETURN_NOT_SUPPORTED;

    switch (cmd & ~GENERIC_MONIT_REQUEST_SIZE) {
    case GENERIC_MONIT_AGC_FREEZE:
        if(!state_dib0090.p_dib0090_config->use_pwm_agc) {
            ret = 0;
            state_dib0090.agc_freeze = offset;
        }
        break;
    default:
        break;
    }

    return ret;
}

MS_S16 dib0090_get_offset(void)
{
    MS_U16 wbdp,wbdn;

    dib0090_write_reg(VGA_MODE,0x7); //Turns to Pos
    DibMSleep(INTERN_WAIT);
    wbdp=dib0090_read_reg(ADCVAL);

    dib0090_write_reg(VGA_MODE,0x4); //Turns to neg
    DibMSleep(INTERN_WAIT);
    wbdn=dib0090_read_reg(ADCVAL);
    // dbgpl(&dib0090_dbg, "Pos=%.3f;Neg=%.3f;Delta=%.3f", wbdp*1.8/1024, wbdn*1.8/1024,(wbdp-wbdn)*1.8/1024);
    return wbdp-wbdn;
}

MS_U16 dib0090_get_wbd_offset(void)
{
    return state_dib0090.wbd_offset;
}

MS_U16 dib0090_identify(void)
{
    MS_U16 v;

    v = dib0090_read_reg(DIG_CFG_RO);

    /* without PLL lock info */
    v &= 0x3ff;
    Out(printf("P/V: %04x:\n", v));

    if((v >> 8)&0xf)
        Out(printf("FE : Product ID = 0x%x : KROSUS\n", (v >> 8)&0xf));
    else
        return 0xff;

    v&=0xff;
    if(((v>>5)&0x7) == 0x1)
        Out(printf("FE : MP001 : 9090/8096\n"));
    else if(((v>>5)&0x7) == 0x4)
        Out(printf("FE : MP005 : Single Sband\n"));
    else if(((v>>5)&0x7) == 0x6)
        Out(printf("FE : MP008 : diversity VHF-UHF-LBAND\n"));
    else if(((v>>5)&0x7) == 0x7)
        Out(printf("FE% : MP009 : diversity 29098 VHF-UHF-SBAND\n"));
    else
        return 0xff;

    /* revision only */
    v&=0x1f;
    if (v == 0x3)
        Out(printf("FE : P1-D/E/F detected\n"));
    else if (v == 0x1)
        Out(printf("FE : P1C detected\n"));
    else if (v == 0x0) {
        Out(printf("FE : P1-A/B detected: driver is deactivated - not available\n"));
        return 0xff;
    }

    return v;
}

void dib0090_Offset_Calib(void)
{
    //Preaload the actual values
    MS_U16 val7 = 0x48d; //Force Vcm2=0.8V
    /******************Init*************************/
    Out(printf("Internal DC calibration\n"));
    dib0090_write_reg(CTRL_BB_7,val7); //BB Test enabled on I

    //The LNA is off
    dib0090_write_reg(TUNER_EN,0x2ED);

    /****************** Step1 BB gain1= 26 with boost 1, gain 2 = 0 *************************/
    dib0090_write_reg(CTRL_BB_1, (1<<13) | (0<<8) | (26<<3));

    //PGA I
    dib0090_find_DC(6,5,1);
    //PGA Q
    val7 &= ~(0x80); // force to Q
    dib0090_write_reg(CTRL_BB_7,val7);
    val7=dib0090_find_DC(7,11,1);

    /****************** Step 2 BB gain 1 = 26 with boost = 1 & gain 2 = 29 *************************/
    dib0090_write_reg(CTRL_BB_1, (1<<13) | (29<<8) | (26<<3));

    /*Servo I*/
    val7 |= (0x80); // force to I
    dib0090_write_reg(CTRL_BB_7,val7);
    dib0090_find_DC(6,0,0);

    /*Servo Q*/
    val7 &= ~(0x80); // force to Q
    dib0090_write_reg(CTRL_BB_7,val7);
    dib0090_find_DC(6,10,0);

    /******************Exit*************************/
    dib0090_write_reg(CTRL_BB_7,val7 & (~0x8)); //Force the test bus to be off
    dib0090_write_reg(VGA_MODE,0x7); //Turns to Pos
}

void dib0090_pwm_gain_reset(struct dibChannel *ch)
{
    Out(printf("-I- dib0090_pwm_gain_reset called\n"));
    Out(printf("-I- state_dib0090.p_dib0090_config->use_pwm_agc= %d\n", state_dib0090.p_dib0090_config->use_pwm_agc));
    /* reset the AGC */
    if (state_dib0090.p_dib0090_config->use_pwm_agc) {
#ifdef CONFIG_BAND_SBAND
        if (BAND_OF_FREQUENCY(fe.current_rf) == BAND_SBAND) {
            dib0090_set_rframp_pwm(rf_ramp_pwm_sband);
            dib0090_set_bbramp_pwm(bb_ramp_pwm_boost);
        } else
#endif
#ifdef CONFIG_BAND_CBAND
        if (BAND_OF_FREQUENCY(fe.current_rf) == BAND_CBAND) {
              dib0090_set_rframp_pwm(rf_ramp_pwm_cband);
              dib0090_set_bbramp_pwm(bb_ramp_pwm_normal);
        } else
#endif
#ifdef CONFIG_BAND_VHF
        if (BAND_OF_FREQUENCY(fe.current_rf) == BAND_VHF) {
            dib0090_set_rframp_pwm(rf_ramp_pwm_vhf);
            dib0090_set_bbramp_pwm(bb_ramp_pwm_normal);
        } else
#endif
        {
            dib0090_set_rframp_pwm(rf_ramp_pwm_uhf);
            dib0090_set_bbramp_pwm(bb_ramp_pwm_normal);
        }

        // activate the ramp generator using PWM control
        if (state_dib0090.rf_ramp[0] != 0)
            dib0090_write_reg(RF_RAMP9, (3 << 11));
        else
            dib0090_write_reg(RF_RAMP9, (0 << 11));

        dib0090_write_reg(BB_RAMP7, (1 << 10));// 0 gain by default
    }
}

int dib0090_reset(void)
{
    MS_U16 l, r, *n;
    dib8000_i2c_gate_ctrl(1);
    dib0090_reset_digital(state_dib0090.p_dib0090_config);
    state_dib0090.revision = dib0090_identify();

    /* Revision definition */
    if (state_dib0090.revision == 0xff)
        return DIB_RETURN_ERROR;
#ifdef EFUSE
    else if (state_dib0090.revision >= 3) /* Update the efuse : Only available for KROSUS > P1C */
        dib0090_set_EFUSE();
#endif

    /* Upload the default values */
    n = (MS_U16 *) dib0090_defaults;
    l = pgm_read_word(n++);
    while (l) {
        r = pgm_read_word(n++);
        do {
          /* DEBUG_TUNER */
          /* dbgpl(&dib0090_dbg,"%d, %d, %d", l, r, pgm_read_word(n)); */
            dib0090_write_reg(r, pgm_read_word(n++));
            r++;
        } while (--l);
	    l = pgm_read_word(n++);
    }

#ifndef FIRMWARE_FIREFLY
    if (state_dib0090.p_dib0090_config->cap_value != 0)
        dib0090_write_reg(CTRL_BB_2, (dib0090_read_reg(CTRL_BB_2)&0xffc0)|(state_dib0090.p_dib0090_config->cap_value&0x3f));
#endif

    /* Congigure in function of the crystal */
    if (state_dib0090.p_dib0090_config->io.clock_khz >= 24000)
        l = 1;
    else
        l = 2;
    dib0090_write_reg(CTRL_CRYSTAL, l);
    Out(printf("Pll lock : %d\n", (dib0090_read_reg(DIG_CFG_RO)>>11)&0x1));
    dib0090_Offset_Calib();
    dib0090_wbd_calibration();
    dib8000_i2c_gate_ctrl(0);

    return DIB_RETURN_SUCCESS;
}

void dib0090_reset_digital(struct dib0090_config *cfg)
{
    HARD_RESET();

    dib0090_write_reg(TUNER_EN, EN_PLL);
    dib0090_write_reg(SLEEP_EN, EN_DIGCLK | EN_PLL | EN_CRYSTAL); /* PLL, DIG_CLK and CRYSTAL remain */

    /* adcClkOutRatio=8->7, release reset */
    dib0090_write_reg(DIG_CFG_3, ((cfg->io.adc_clock_ratio-1) << 11) | (0 << 10) | (1 << 9) | (1 << 8) | (0 << 4) | 0);
	if (cfg->clkoutdrive != 0)
		dib0090_write_reg(DIG_CFG, (0 << 15) | ((!cfg->analog_output) << 14) | (1 << 10) | (1 << 9) | (0 << 8) | (cfg->clkoutdrive << 5) | (cfg->clkouttobamse << 4) | (0 << 2) | (0));
	else
		dib0090_write_reg(DIG_CFG, (0 << 15) | ((!cfg->analog_output) << 14) | (1 << 10) | (1 << 9) | (0 << 8) | (7 << 5) | (cfg->clkouttobamse << 4) | (0 << 2) | (0));


    /* enable pll, de-activate reset, ratio: 2/1 = 60MHz */
    dib0090_write_reg(PLL_CFG, (cfg->io.pll_bypass << 15) | (1 << 13) | (cfg->io.pll_range << 12) | (cfg->io.pll_loopdiv << 6) | (cfg->io.pll_prediv));
}

// Changes the baseband filter settings
void dib0090_set_bandwidth(struct dibChannel *ch)
{
	MS_U16 tmp;

    //dib8000_i2c_gate_ctrl(1);
    if (ch->bandwidth_kHz <= 5000)
        tmp = (3 << 14);
    else if (ch->bandwidth_kHz <= 6000)
        tmp = (2 << 14);
    else if (ch->bandwidth_kHz <= 7000)
        tmp = (1 << 14);
    else
        tmp = (0 << 14);

    state_dib0090.bb_1_def &= 0x3fff;
    state_dib0090.bb_1_def |= tmp;

    dib0090_write_reg(CTRL_BB_1, state_dib0090.bb_1_def); /* be sure that we have the right bb-filter */
    //dib8000_i2c_gate_ctrl(0);
}

void dib0090_set_bbramp(MS_U16 *cfg)
{
    state_dib0090.bb_ramp = cfg;
    dib0090_set_boost(cfg[0] > 500); /* we want the boost if the gain is higher that 50dB */
}

void dib0090_set_bbramp_pwm(MS_U16 *cfg)
{
    state_dib0090.bb_ramp = cfg;
    dib0090_set_boost(cfg[0] > 500); /* we want the boost if the gain is higher that 50dB */
    dib0090_write_reg(BB_RAMP1, 0xffff); //(state->config->io.clock_khz / cfg[1]) * 100);
    Out(printf("total BB gain: %ddB, step: %d\n", (MS_U32) cfg[0], dib0090_read_reg(BB_RAMP1)));
    dib0090_write_regs(BB_RAMP3, cfg + 3, 4);
}

void dib0090_set_boost(int onoff)
{
    state_dib0090.bb_1_def &= 0xdfff;
    state_dib0090.bb_1_def |= onoff << 13;
}

void dib0090_set_EFUSE(void)
{
#ifdef EFUSE
    MS_U8 c,h,n;
    MS_U16 e2,e4;

    e2=dib0090_read_reg(EFUSE_2);
    e4=dib0090_read_reg(EFUSE_4);
    //e2 = e4 = (5<<12) | (27<<6) | (0x18); /* Just for test */
    e2 &= e4; /* Remove the redundancy  */
    if (e2 != 0xffff) {
        c = e2 & 0x3f;
        n = (e2 >> 12) & 0xf;
        h= (e2 >> 6) & 0x3f;
        dib0090_write_reg(CTRL_BIAS, (h << 10)) ;
        e2 = (n<<11) | ((h>>2)<<6) | (c);
        dib0090_write_reg(CTRL_BB_2, e2) ; /* Load the BB_2 */
    }
#endif
}

void dib0090_set_rframp(MS_U16 *cfg)
{
    state_dib0090.rf_ramp = cfg;
}

void dib0090_set_rframp_pwm(MS_U16 *cfg)
{
    state_dib0090.rf_ramp = cfg;

    dib0090_write_reg(RF_RAMP1, 0xffff); //(state->config->io.clock_khz / cfg[1]) * 100);

    Out(printf("total RF gain: %ddB, step: %d\n", (MS_U32) cfg[0], dib0090_read_reg(RF_RAMP1)));

    dib0090_write_regs(RF_RAMP3, cfg + 3, 6);
    dib0090_write_regs(GAIN4_1, cfg + 9, 2);
}

void dib0090_set_wbd_target(MS_U16 offset_uhf, MS_U16 offset_vhf, MS_U16 offset_cband)
{
    state_dib0090.wbd_uhf_offset = offset_uhf;
    state_dib0090.wbd_vhf_offset = offset_vhf;
    state_dib0090.wbd_cband_offset = offset_cband;
}

int dib0090_tune(struct dibChannel *ch)
{
    struct dib0090_tuning *tune = state_dib0090.p_current_tune_table_index;
    struct dib0090_pll *pll = state_dib0090.p_current_pll_table_index;
    enum frontend_tune_state *tune_state = &fe.tune_state;

    MS_U32 rf = ch->RF_kHz + (BAND_OF_FREQUENCY(ch->RF_kHz) == BAND_UHF ? state_dib0090.p_dib0090_config->freq_offset_khz_uhf : state_dib0090.p_dib0090_config->freq_offset_khz_vhf);
    MS_U8 band = (MS_U8)BAND_OF_FREQUENCY(rf);
    MS_U16 lo4 = 0xe900, lo5, lo6, Den;
    MS_U32 FBDiv, Rest, FREF, VCOF_kHz = 0;
    MS_U16 adc;
    MS_S8 step_sign;
    int ret = 10; /* 1ms is the default delay most of the time */
    MS_U8 c,i;

    /************************* VCO ***************************/
    /* Default values for FG                                 */
    /* from these are needed :                               */
    /* Cp,HFdiv,VCOband,SD,Num,Den,FB and REFDiv             */

    //dbgpl(&dib0090_dbg, "fe %p : Tuning for Band: %d (%d kHz); tune_state = %d",fe, band, rf, fe->tune_state);

    if (fe.tune_state == CT_TUNER_START) {
        switch(BAND_OF_FREQUENCY(ch->RF_kHz)) {
        default:
            DBG_DIB8096("Warning : this frequency is not in the supported range, using VHF switch");
        case BAND_VHF: //gpio0 : 1;
            demod_set_gpio(8, 0, 1);
            break;
        case BAND_UHF: //gpio0 : 0;
            demod_set_gpio(8, 0, 0);
            break;
        }
    }

    if (fe.current_rf != rf) {
        state_dib0090.tuner_is_tuned = 0;

        tune = dib0090_tuning_table;
		pll = dib0090_pll_table;
        /* Look for the interval */
        while (rf > tune->max_freq)
            tune++;
		while (rf > pll->max_freq)
            pll++;
        state_dib0090.p_current_tune_table_index = tune;
        state_dib0090.p_current_pll_table_index = pll;
    }

    //dbgpl(&dib0090_dbg, "FE %d TUNER STEP %d callback time = %d",fe->id, fe->tune_state,fe->tuner_info->callback_time);

    if (*tune_state == CT_TUNER_START) {
        if(state_dib0090.tuner_is_tuned == 0)
            fe.current_rf = 0;

        if (fe.current_rf != rf) {
            // select internal switch
            dib0090_write_reg(CTRL_RF_SW, 0xb800 | (tune->switch_trim));

            // Find the VCO frequency in MHz
            VCOF_kHz = (pll->hfdiv * rf) * 2;

            FREF = state_dib0090.p_dib0090_config->io.clock_khz; // REFDIV is 1FREF Has to be as Close as possible to 10MHz
            if (state_dib0090.p_dib0090_config->fref_clock_ratio != 0)
                FREF /= state_dib0090.p_dib0090_config->fref_clock_ratio;

            // Determine the FB divider
            // The reference is 10MHz, Therefore the FBdivider is on the first digits
            FBDiv = (VCOF_kHz / pll->topresc / FREF);
            Rest  = (VCOF_kHz / pll->topresc) - FBDiv * FREF; //in kHz

            // Avoid Spurs in the loopfilter bandwidth
            if (Rest < LPF)                   Rest = 0;
            else if (Rest < 2 * LPF)          Rest = 2 * LPF;
            else if (Rest > (FREF - LPF))   { Rest = 0 ; FBDiv += 1; } //Go to the next FB
            else if (Rest > (FREF - 2 * LPF)) Rest = FREF - 2 * LPF;
            Rest = (Rest * 6528) / (FREF / 10);
            state_dib0090.rest = Rest;

            /* external loop filter, otherwise:
             * lo5 = (0 << 15) | (0 << 12) | (0 << 11) | (3 << 9) | (4 << 6) | (3 << 4) | 4;
             * lo6 = 0x0e34 */
            if (Rest == 0)
            {
                if (pll->vco_band)
                    lo5 = 0x049f;
                /*          else if (state->config->analog_output)
                            lo5 = 0x041f; */
                else
                    lo5 = 0x041f;
            }
            else { // Rest != 0
                if (pll->vco_band)
                    lo5 = 0x049e;
                else if (state_dib0090.p_dib0090_config->analog_output)
                    lo5 = 0x041d;
                else
                    lo5 = 0x041c;
            }

            lo5 |= (pll->hfdiv_code << 11) | (pll->vco_band << 7); /* bit 15 is the split to the slave, we do not do it here */

            //Internal loop filter set...
            if(!state_dib0090.p_dib0090_config->io.pll_int_loop_filt)
            {
                if (Rest != 0)
                    lo6 = 0xff28;
                else
                    lo6 = 0xfff8;
            }
            else
                lo6 = (state_dib0090.p_dib0090_config->io.pll_int_loop_filt << 3); // take the loop filter value given by the layout
            //dbgpl(&dib0090_dbg, "FE %d lo6 = 0x%04x", (MS_U32)fe->id, (MS_U32)lo6);

            Den = 1;

            Out(printf(" *****  ******* Rest value = %d\n", Rest )) ; //state->wbd_offset);


            if (Rest > 0) {
                if (state_dib0090.p_dib0090_config->analog_output)
                    lo6 |= (1 << 2) | 2; //SigmaDelta and Dither
                else
                    lo6 |= (1 << 2) | 1; //SigmaDelta and Dither
                Den = 255;
            }

#ifdef CONFIG_BAND_SBAND
            if (band == BAND_SBAND)
                lo6 &= 0xfffb; //Remove the Dither for SBAND
#endif

            // Now we have to define the Num and Denum
            // LO1 gets the FBdiv
            dib0090_write_reg(CTRL_LO_1, (MS_U16) FBDiv);
            // LO2 gets the REFDiv
            if (state_dib0090.p_dib0090_config->fref_clock_ratio != 0)
                dib0090_write_reg(CTRL_LO_2, (Den << 8) | state_dib0090.p_dib0090_config->fref_clock_ratio);
            else
                dib0090_write_reg(CTRL_LO_2, (Den << 8) | 1);

            // LO3 for the Numerator
            dib0090_write_reg(CTRL_LO_3, (MS_U16)Rest);
            // VCO and HF DIV
            dib0090_write_reg(CTRL_LO_5, lo5);
            // SIGMA Delta
            dib0090_write_reg(CTRL_LO_6, lo6);

            fe.current_rf = rf;

            // Check if the 0090 is analogged configured
            //Disable ADC and DigPLL =0xFF9F, 0xffbf for test purposes.
            //Enable The Outputs of the BB on DATA_Tx
            lo6 = tune->tuner_enable;
            if (state_dib0090.p_dib0090_config->analog_output)
                lo6 = (lo6 & 0xff9f) | 0x2;

            dib0090_write_reg(TUNER_EN, lo6 | EN_LO
                    | state_dib0090.p_dib0090_config->use_pwm_agc * EN_CRYSTAL
                    );

            *tune_state = CT_TUNER_STEP_0;
        } else { /* we are already tuned to this frequency - the configuration is correct  */
            ret = 50; /* wakeup time */
            *tune_state = CT_TUNER_STEP_5;
        }
    } else if (*tune_state == CT_TUNER_STEP_0) { // Captrim INIT
        c=0;
        state_dib0090.step = 64;
        state_dib0090.captrim = state_dib0090.fcaptrim = 64;
        state_dib0090.adc_diff = 3000; // start with a unreachable high number
        ret = 20;

        /* WBDMUX=FG */
        dib0090_write_reg(CTRL_WBDMUX, 0x2B1);
        dib0090_write_reg(ADCCLK, 0x0032);
        *tune_state = CT_TUNER_STEP_1;

    } else if (*tune_state == CT_TUNER_STEP_1) {
        state_dib0090.step /= 2;
        dib0090_write_reg(CTRL_LO_4, lo4 | state_dib0090.captrim);
        *tune_state = CT_TUNER_STEP_2;
    } else if (*tune_state == CT_TUNER_STEP_2) {
        MS_S16 adc_target;

        adc = dib0090_read_reg(ADCVAL);
        Out(printf("FE CAPTRIM=%d; ADC = %d (ADC) & %dmV\n", (MS_U32)state_dib0090.captrim, (MS_U32)adc, (MS_U32) (adc)*(MS_U32)1800/(MS_U32)1024));

		if (state_dib0090.rest == 0)
			adc_target = 200;
		else
			adc_target = 400;

        if (adc >= adc_target) {
            adc -= adc_target;
            step_sign = -1;
        } else {
            adc = adc_target - adc;
            step_sign = 1;
        }

        if (adc < state_dib0090.adc_diff) {
            Out(printf("FE CAPTRIM=%d is closer to target (%d/%d)\n", (MS_U32)state_dib0090.captrim, (MS_U32)adc, (MS_U32)state_dib0090.adc_diff));
            state_dib0090.adc_diff = adc;
            state_dib0090.fcaptrim = state_dib0090.captrim;
            //we could break here, to save time, if we reached a close-enough value
            //e.g.: if (state->adc_diff < 20)
            //break;
        }

        state_dib0090.captrim += step_sign * state_dib0090.step;
        if (state_dib0090.step >= 1)
            *tune_state = CT_TUNER_STEP_1;
        else
            *tune_state = CT_TUNER_STEP_3;

        ret = 15;
    } else if (*tune_state == CT_TUNER_STEP_3) {
        /*write the final cptrim config*/
        dib0090_write_reg(CTRL_LO_4, lo4 | state_dib0090.fcaptrim);

        *tune_state = CT_TUNER_STEP_4;
    } else if (*tune_state == CT_TUNER_STEP_4) {
        dib0090_write_reg(ADCCLK, 0x07ff);

        Out(printf("FE Final Captrim: %d\n", (MS_U32)state_dib0090.fcaptrim));
        Out(printf("FE HFDIV code: %d\n", (MS_U32)pll->hfdiv_code));
        Out(printf("FE VCO = %d\n", (MS_U32)pll->vco_band));
        Out(printf("FE VCOF in kHz: %d ((%d*%d) << 1))\n", (MS_U32)((pll->hfdiv * rf) * 2), (MS_U32)pll->hfdiv, (MS_U32)rf));
        Out(printf("FE REFDIV: %d, FREF: %d\n", (MS_U32)1, (MS_U32)state_dib0090.p_dib0090_config->io.clock_khz));
        Out(printf("FE FBDIV: %d, Rest: %d\n", (MS_U32)dib0090_read_reg(CTRL_LO_1), (MS_U32)dib0090_read_reg(CTRL_LO_3)));
        Out(printf("FE Num: %d, Den: %d, SD: %d\n", (MS_U32)dib0090_read_reg(CTRL_LO_3), (MS_U32)(dib0090_read_reg(CTRL_LO_2) >> 8), (MS_U32)dib0090_read_reg(CTRL_LO_6) & 0x3));

#define WBD     0x781 /* 1 1 1 1 0000 0 0 1 */
        c=4; //wbdmux_gain
        i=3; //wbdmux_bias
#if defined(CONFIG_BAND_LBAND) || defined(CONFIG_BAND_SBAND)
        if ((band==BAND_LBAND) || (band==BAND_SBAND)) {
            c=2;
            i=2;
        }
#endif
        dib0090_write_reg(CTRL_WBDMUX, (c << 13) | (i << 11) | (WBD
#ifdef CONFIG_DIB0090_USE_PWM_AGC
                                       |(state_dib0090.p_dib0090_config->use_pwm_agc<<1)
#endif
                                   ));
        dib0090_write_reg(CTRL_RXRF2, (tune->lna_tune << 5) | (tune->lna_bias << 0));
        dib0090_write_reg(CTRL_RF_V2I, tune->v2i);
        dib0090_write_reg(CTRL_RF_MIX, tune->mix);
        dib0090_write_reg(CTRL_RF_LOAD, tune->load);

        *tune_state = CT_TUNER_STEP_5;
    } else if (*tune_state == CT_TUNER_STEP_5) {

        /* initialize the lt gain register */
        state_dib0090.rf_lt_def = 0x7c00;
        dib0090_write_reg(CTRL_RF_LT, state_dib0090.rf_lt_def);

        dib0090_set_bandwidth(ch);
        state_dib0090.tuner_is_tuned = 1;
        *tune_state = CT_TUNER_STOP;
    } else
        ret = FE_CALLBACK_TIME_NEVER;
    return ret;
}

/* wakeup from sleep */
int dib0090_wakeup(void)
{
	return dib8090_tuner_sleep(0);
}

void dib0090_wbd_calibration(void)
{
    MS_U16 tuner_en = dib0090_read_reg(TUNER_EN);
    MS_U16 wbdmux= dib0090_read_reg(CTRL_WBDMUX);

    /* WBD-mode=log, Bias=2, Gain=6, Testmode=1, en=1, WBDMUX=1 */
    dib0090_write_reg(CTRL_WBDMUX, (wbdmux & 0xfb0f) | (1<<10)); //Force the WBD enable
    dib0090_write_reg(TUNER_EN, EN_UHF & 0x0fff); //Discard the LNA

    DibMSleep(9);  // wait for the WBDMUX to switch and for the ADC to sample
    state_dib0090.wbd_offset = dib0090_read_reg(ADCVAL);
    dib0090_write_reg(TUNER_EN, tuner_en);
    Out(printf("WBD offset = %d\n", state_dib0090.wbd_offset));
}

void dib0090_wbd_target(MS_U32 rf)
{
    MS_U16 offset = 250;
    MS_U8 band = (MS_U8)BAND_OF_FREQUENCY(rf);

    if (band == BAND_VHF)
        offset = 650;
    if (band == BAND_UHF)
        offset = state_dib0090.wbd_uhf_offset;
    if (band == BAND_VHF)
       offset = state_dib0090.wbd_vhf_offset;
    if (band == BAND_CBAND)
       offset = state_dib0090.wbd_cband_offset;

    //for (offset = 0; offset < 1000; offset += 4)
    //    dbgp("offset = %d -> %d\n", offset, dib0090_wbd_to_db(state, offset));

	state_dib0090.wbd_target = dib0090_wbd_to_db(state_dib0090.wbd_offset + offset); // get the value in dBm from the offset
    Out(printf("wbd-target: %d dB\n", (MS_U32) state_dib0090.wbd_target));
}

MS_S16 dib0090_wbd_to_db(MS_U16 wbd)
{
    wbd &= 0x3ff;
    if (wbd < state_dib0090.wbd_offset)
        wbd = 0;
    else
        wbd -= state_dib0090.wbd_offset;
    /* -64dB is the floor */
    return -640 + (MS_S16) slopes_to_scale(dib0090_wbd_slopes, ARRAY_SIZE(dib0090_wbd_slopes), wbd);
}


#endif

#if 1 // dib8000

void dib8000_i2c_gate_ctrl(MS_U8 on)
{
    MS_U16 gateval =0;

    if(on)
    {
        /*Open the I2C gate of DIBCOM*/
        gateval = 0xc000 ;
    }
    else
    {
        /*close the I2C gate of DIBCOM*/
        gateval = 0x0100 ;
    }

    dib8000_write_word(DIBCOM_I2C_MASTER_BASE_REGISTER + 1, gateval);
}

int dib8000_agc_soft_split(void)
{
	MS_U16 agc,split_offset;

	if(!state_dib8000.p_current_agc || !state_dib8000.p_current_agc->perform_agc_softsplit || state_dib8000.p_current_agc->split.max == 0)
		return FE_CALLBACK_TIME_NEVER;

	// n_agc_global
	agc = dib8000_read_word(390);

	if (agc > state_dib8000.p_current_agc->split.min_thres)
		split_offset = state_dib8000.p_current_agc->split.min;
	else if (agc < state_dib8000.p_current_agc->split.max_thres)
		split_offset = state_dib8000.p_current_agc->split.max;
	else
		split_offset = state_dib8000.p_current_agc->split.max *
			(agc - state_dib8000.p_current_agc->split.min_thres) /
			(state_dib8000.p_current_agc->split.max_thres - state_dib8000.p_current_agc->split.min_thres);

	Out(printf("AGC split_offset: %d\n",split_offset));

	// P_agc_force_split and P_agc_split_offset
	dib8000_write_word(107, (dib8000_read_word(107) & 0xff00) | split_offset);
    return 5000;
}

int dib8000_agc_startup(struct dibChannel *ch)
{
	enum frontend_tune_state *tune_state = &fe.tune_state;

    int ret = 0;

	switch (*tune_state) {
    case CT_AGC_START:
        // set power-up level: interf+analog+AGC

        dib8000_set_adc_state(DIBX000_ADC_ON);

        if (dib8000_set_agc_config((unsigned char)(BAND_OF_FREQUENCY(ch->RF_kHz))) != DIB_RETURN_SUCCESS) {
            *tune_state = CT_AGC_STOP;
            fe.status = FE_STATUS_TUNE_FAILED;
			break;
        }

        ret = 70;
        *tune_state = CT_AGC_STEP_0;
        break;

    case CT_AGC_STEP_0:
        //AGC initialization
        dib8090_agc_control(1);

        dib8000_restart_agc();

        // wait AGC rough lock time
        ret = 50;
        *tune_state = CT_AGC_STEP_1;
        break;

    case CT_AGC_STEP_1:
        // wait AGC accurate lock time
        ret = 70;

        if (dib8000_update_lna())
            // wait only AGC rough lock time
            ret = 50;
        else
            *tune_state = CT_AGC_STEP_2;
        break;

    case CT_AGC_STEP_2:
        dib8000_agc_soft_split();

        dib8090_agc_control(0);

        *tune_state = CT_AGC_STOP;
        break;
    default:
        ret = dib8000_agc_soft_split();
        break;
	}
	return ret;

}

MS_U8 dib8000_autosearch_irq(void)
{
    MS_U8 b[2];
    dib8000_read(1284, b, 2);

	if (b[1] & 0x1) { // failed
		Out(printf("autosearch IRQ: failed\n"));
		return 1;
    }

	if (b[1] & 0x2) { // succeeded
		Out(printf("autosearch IRQ: success\n"));
		return 2;
    }

    return 0; // still pending
}

/* autosearch workarounded for DIB8000*/
int dib8000_autosearch_is_irq(void)
{
    return dib8000_autosearch_irq();
}

int dib8000_autosearch_start(struct dibChannel *ch)
{
	MS_U8 factor;
	MS_U32 value;

	int slist = 0;
	struct dibChannel schan;
	INIT_CHANNEL(&schan, STANDARD_ISDBT);
	schan = *ch;
	schan.u.isdbt.nfft                   = FFT_8K;
	schan.u.isdbt.guard                  = GUARD_INTERVAL_1_8;
	//schan.u.isdbt.sb_mode                = 0;
	//schan.u.isdbt.partial_reception      = 0;
	schan.u.isdbt.spectrum_inversion     = 0;
	if (!schan.u.isdbt.sb_mode)
		schan.u.isdbt.layer[0].nb_segments   = 13;
	schan.u.isdbt.layer[0].constellation = QAM_64QAM;
	schan.u.isdbt.layer[0].code_rate     = VIT_CODERATE_2_3;
	schan.u.isdbt.layer[0].time_intlv    = 0;

	//choose the right list, in sb, always do everything
	if (schan.u.isdbt.sb_mode) {
		slist = 7;
		dib8000_write_word(0, (dib8000_read_word(0) & 0x9fff) | (1 << 13));
	}
	else {
		if (ch->u.isdbt.guard == GUARD_INTERVAL_AUTO) {
			if (ch->u.isdbt.nfft == FFT_AUTO) {
				slist = 7;
				dib8000_write_word(0, (dib8000_read_word(0) & 0x9fff) | (1 << 13));  // P_mode = 1 to have autosearch start ok with mode2
			}
			else {
				slist = 3;
				schan.u.isdbt.nfft = ch->u.isdbt.nfft;
			}
		} else {
			if (ch->u.isdbt.nfft == FFT_AUTO) {
				slist = 2;
				schan.u.isdbt.guard = ch->u.isdbt.guard;
				dib8000_write_word(0, (dib8000_read_word(0) & 0x9fff) | (1 << 13));  // P_mode = 1
			}
			else {
				slist = 0;
				schan.u.isdbt.nfft = ch->u.isdbt.nfft;
				schan.u.isdbt.guard = ch->u.isdbt.guard;
			}
		}

		Out(printf("using list for autosearch : %d\n", slist));
		dib8000_set_channel(&schan, (unsigned char)slist, 1);

		factor = 1;

		//set lock_mask values
		dib8000_write_word(6, 0x4   );
		dib8000_write_word(7, 0x8   );
		dib8000_write_word(8, 0x1000 );

		//set lock_mask wait time values
 		value = 50 * state_dib8000.dib8000_cfg.pll->internal * factor;
		dib8000_write_word(11, (MS_U16) ((value >> 16) & 0xffff)); // lock0 wait time
		dib8000_write_word(12, (MS_U16)  (value        & 0xffff)); // lock0 wait time
 		value = 100 * state_dib8000.dib8000_cfg.pll->internal * factor;
		dib8000_write_word(13, (MS_U16) ((value >> 16) & 0xffff)); // lock1 wait time
		dib8000_write_word(14, (MS_U16)  (value        & 0xffff)); // lock1 wait time
 		value = 1000 * state_dib8000.dib8000_cfg.pll->internal * factor;
		dib8000_write_word(15, (MS_U16) ((value >> 16) & 0xffff)); // lock2 wait time
		dib8000_write_word(16, (MS_U16)  (value        & 0xffff)); // lock2 wait time

		value = dib8000_read_word(0);
		dib8000_write_word(0, (MS_U16) ((1 << 15) | value));
		dib8000_read_word(1284);  // reset the INT. n_irq_pending
		dib8000_write_word(0, (MS_U16) value);
    }

	return DIB_RETURN_SUCCESS;
}

int dib8000_cfg_gpio(MS_U8 num, MS_U8 dir, MS_U8 val)
{
	state_dib8000.dib8000_cfg.gpio_dir = dib8000_read_word(1029);
	state_dib8000.dib8000_cfg.gpio_dir &= ~(1 << num);      /* reset the direction bit */
	state_dib8000.dib8000_cfg.gpio_dir |= (dir & 0x1) << num; /* set the new direction */
	dib8000_write_word(1029, state_dib8000.dib8000_cfg.gpio_dir);

	state_dib8000.dib8000_cfg.gpio_val = dib8000_read_word(1030);
	state_dib8000.dib8000_cfg.gpio_val &= ~(1 << num);          /* reset the direction bit */
	state_dib8000.dib8000_cfg.gpio_val |= (val & 0x01) << num; /* set the new value */
	dib8000_write_word(1030, state_dib8000.dib8000_cfg.gpio_val);

	Out(printf("gpio dir: %x: gpio val: %x\n", state_dib8000.dib8000_cfg.gpio_dir, state_dib8000.dib8000_cfg.gpio_val));

	return DIB_RETURN_SUCCESS;
}

MS_U32 dib8000_ctrl_timf(MS_U8 op, MS_U32 timf)
{
	switch (op) {
		case DEMOD_TIMF_SET: state_dib8000.timf = timf; break;
		case DEMOD_TIMF_UPDATE: dib8000_update_timf(); break;
		case DEMOD_TIMF_GET: break;
	}
	dib8000_set_bandwidth(fe.current_bw);
	return state_dib8000.timf;
}

/* demod operations */
int dib8000_demod_set_output_mode(int mode)
{
	return dib8000_set_output_mode(mode);
}

int dib8000_generic_monitoring(MS_U32 cmd, MS_U8 *b, MS_U32 size, MS_U32 offset)
{
    int ret = DIB_RETURN_NOT_SUPPORTED;

    switch (cmd & ~GENERIC_MONIT_REQUEST_SIZE) {

    case GENERIC_MONIT_COMPAT_EXTENDED:
        if (size > 0)
            dib8000_get_demod_monitor((struct dibDemodMonitor *) b);
        ret = sizeof(struct dibDemodMonitor);
        break;

    case GENERIC_MONIT_SET_BER_RS_LEN:
        ret = dib8000_write_word(285, (dib8000_read_word(285) & ~0x60) | ((MS_U16) ((offset&0x3) << 5)) );
        break;

    }
    return ret;
}

MS_S32 dib8000_get_adc_power(MS_U8 mode)
{
	MS_U32 ix =0, tmp_val =0, exp = 0, mant = 0;
        MS_S32 val;

        val = dib8000_read32(384);
        /* mode = 1 : ln_agcpower calc using mant-exp conversion and mantis look up table */
        if(mode) {
            tmp_val = val;
            while(tmp_val>>=1)
                exp++;
            mant = (val * 1000 / (1<<exp));
            ix = (MS_U8)((mant-1000)/100); /* index of the LUT */
            val = (lut_1000ln_mant[ix] + 693*(exp-20) - 6908); /* 1000 * ln(adcpower_real) ; 693 = 1000ln(2) ; 6908 = 1000*ln(1000) ; 20 comes from adc_real = adc_pow_int / 2**20 */
            val = (val*256)/1000;
         }
    return val;
}

int dib8000_get_channel(struct dibChannel *cd)
{
    MS_U16 v;
    int i;

    cd->type = STANDARD_ISDBT;

    v = (dib8000_read_word(570) & 0x30 ) >> 4; // read mode
	switch (v) {
		case 1: cd->u.isdbt.nfft = FFT_2K; break;
		case 2: cd->u.isdbt.nfft = FFT_4K; break;
		default:
		case 3: cd->u.isdbt.nfft = FFT_8K; break;
	}
	v = dib8000_read_word(570) & 0x3;
	switch (v) {
		case 0: cd->u.isdbt.guard = GUARD_INTERVAL_1_32; break;
		case 1: cd->u.isdbt.guard = GUARD_INTERVAL_1_16; break;
		case 2: cd->u.isdbt.guard = GUARD_INTERVAL_1_8 ; break;
		default:
		case 3: cd->u.isdbt.guard = GUARD_INTERVAL_1_4 ; break;
	}
	cd->u.isdbt.partial_reception  =  dib8000_read_word(505) & 0x1;
	cd->u.isdbt.sb_mode            =  dib8000_read_word(508) & 0x1;
	cd->u.isdbt.spectrum_inversion = (dib8000_read_word(570) & 0x40) >> 6;

    for (i=0; i<3; i++) {
        cd->u.isdbt.layer[i].code_rate = dib8000_read_word(481+i) & 0x7;

        switch (dib8000_read_word(487+i)) {
            case 0: cd->u.isdbt.layer[i].constellation = QAM_DQPSK; break;
            case 1: cd->u.isdbt.layer[i].constellation = QAM_QPSK ; break;
            case 2: cd->u.isdbt.layer[i].constellation = QAM_16QAM; break;
            default :
            case 3: cd->u.isdbt.layer[i].constellation = QAM_64QAM; break;
        }
        cd->u.isdbt.layer[i].nb_segments = dib8000_read_word(493+i) & 0xf;
        cd->u.isdbt.layer[i].time_intlv  = dib8000_read_word(499+i) & 0x3;
    }

    return DIB_RETURN_SUCCESS;
}

int dib8000_get_demod_monitor(struct dibDemodMonitor *m)
{
    MS_U16 val = 0,v;
    MS_U16 i;

    val = dib8000_read_word(568);
    m->locks.agc        = (val >> 14) & 1;
    m->locks.corm       = (val >> 13) & 1;
    m->locks.coff       = (val >> 12) & 1;
    m->locks.coff_cpil  = (val >> 11) & 1;
    m->locks.lmod4      = (val >> 10) & 1;
    m->locks.pha3       = (val >>  9) & 1;
    m->locks.equal      = (val >>  8) & 1;
    m->locks.fec_mpeg   = (val >> 7) & 1;
    m->locks.fec_mpeg_b = (val >> 6) & 1;
    m->locks.fec_mpeg_c = (val >> 5) & 1;
    m->locks.tmcc_dec   = (val >> 4) & 1;
    m->locks.tmcc_sync  = (val >> 3) & 1;
    m->locks.tmcc_data  = (val >> 1) & 1;
    m->locks.dvsy       = (val >>  0) & 1;

    m->locks.vit        = dib8000_read_word(554) & 1;
    m->locks.vit_b      = dib8000_read_word(555) & 1;
    m->locks.vit_c      = dib8000_read_word(556) & 1;

    m->iq_misgain = dib8000_read_word(396);
    m->iq_misphi = dib8000_read_word(395);
    if (m->iq_misphi & 0x800)
        m->iq_misphi -= 0x1000;

    val = dib8000_read_word(542);
    m->equal_noise_mant = (val >> 6) & 0xFF;
    m->equal_noise_exp = ((val & 0x3F));

    val = dib8000_read_word(543);
    m->equal_signal_mant = (val >> 6) & 0xFF;
    m->equal_signal_exp  = (val & 0x3F);

    if ((m->equal_noise_exp & 0x20) != 0)
        m->equal_noise_exp -= 0x40;
    if ((m->equal_signal_exp & 0x20) != 0)
        m->equal_signal_exp -= 0x40;

    val = dib8000_read_word(544);
    m->mer_mant = (val >> 6) & 0xFF;
    m->mer_exp = (val & 0x3F);
    if ((m->mer_exp & 0x20) == 0x20)
        m->mer_exp -= 0x40;

    if (state_dib8000.revision < 0x8002)
    {
        m->can_display_ber_several_layers = 0;

        m->ber_raw          = dib8000_read32(560);
        m->PacketErrors     = dib8000_read_word(565);
    }
    else
    {
        m->can_display_ber_several_layers = 1;

        m->ber_raw_A        = dib8000_read32(560);
        m->ber_raw_B        = dib8000_read32(576);
        m->ber_raw_C        = dib8000_read32(581);
        m->PacketErrors_A   = dib8000_read_word(562);
        m->PacketErrors_B   = dib8000_read_word(578);
        m->PacketErrors_C   = dib8000_read_word(583);

        if (state_dib8000.ber_monitored_layer == LAYER_C)
        {
            m->ber_raw      = m->ber_raw_C;
            m->PacketErrors = m->PacketErrors_C;
        }
        else if (state_dib8000.ber_monitored_layer == LAYER_B)
        {
            m->ber_raw      = m->ber_raw_B;
            m->PacketErrors = m->PacketErrors_B;
        }
        else // layer A by default
        {
            m->ber_raw      = m->ber_raw_A;
            m->PacketErrors = m->PacketErrors_A;
        }
    }

    m->PacketErrorCount = dib8000_read_word(567);
    m->isdbt_err_sec_layerA = dib8000_read_word(551) != 0;
    m->isdbt_err_sec_layerB = dib8000_read_word(552) != 0;
    m->isdbt_err_sec_layerC = dib8000_read_word(553) != 0;

    // Timing offset read
    m->timing_offset = (dib8000_read32(437) >> 1);
    if (m->timing_offset  & 0x20000000)
        m->timing_offset -= 0x40000000;

    m->timf_current = dib8000_read32(435);
    if (state_dib8000.dib8000_cfg.pll)
        m->timf_default = state_dib8000.dib8000_cfg.pll->timf;
    else
        m->timf_default = 20776863;

    m->dds_freq = dib8000_read32(433);
    if (m->dds_freq  & 0x2000000)
        m->dds_freq -= 0x4000000;

    val = dib8000_read_word(26);
    m->invspec = val & 0x01;
    if (m->invspec > 0)
        m->dds_freq = (1 << 26) - m->dds_freq;

    m->p_dds_freq = dib8000_read32(27) & 0x1ffffff;

    m->viterbi_syndrome = dib8000_read_word(557);
    m->viterbi_syndrome_b = dib8000_read_word(558);
    m->viterbi_syndrome_c = dib8000_read_word(559);

    INIT_CHANNEL(&m->cur_digital_channel, STANDARD_ISDBT);
    m->cur_digital_channel.type = STANDARD_ISDBT;

    /* for the DiB8000A we need to read the config */
    switch ((dib8000_read_word(0) >> 13) & 0x3) {
    case 1: m->cur_digital_channel.u.isdbt.nfft = FFT_2K; break;
    case 2: m->cur_digital_channel.u.isdbt.nfft = FFT_4K; break;
    default: m->cur_digital_channel.u.isdbt.nfft = FFT_8K; break;
    }
    m->cur_digital_channel.u.isdbt.guard = dib8000_read_word(1) & 0x3;

    m->cur_digital_channel.u.isdbt.sb_mode = dib8000_read_word(508) & 0x3;
    m->cur_digital_channel.u.isdbt.partial_reception = (dib8000_read_word(505)) & 0x1;
    for (i = 0; i < 3; i++) {
        m->cur_digital_channel.u.isdbt.layer[i].nb_segments = dib8000_read_word(493+i) & 0xF;
        m->cur_digital_channel.u.isdbt.layer[i].code_rate   = dib8000_read_word(481+i) & 0x7;
        m->cur_digital_channel.u.isdbt.layer[i].time_intlv  = dib8000_read_word(499+i) & 0x7;

        val = dib8000_read_word(487+i) & 0x3;
        switch(val) {
        case 0: m->cur_digital_channel.u.isdbt.layer[i].constellation = QAM_DQPSK; break;
        case 1: m->cur_digital_channel.u.isdbt.layer[i].constellation = QAM_QPSK; break;
        case 2: m->cur_digital_channel.u.isdbt.layer[i].constellation = QAM_16QAM; break;
        case 3:
        default : m->cur_digital_channel.u.isdbt.layer[i].constellation = QAM_64QAM; break;
        }
    }

    val = dib8000_read_word(548);
    m->way0_ok = (val >> 1) & 1;
    m->way1_ok = (val     ) & 1;
    m->dvsy_delay = ((val >> 2) & 0x7ff) * 2;
    m->dvsy_first_arrived = (val >> 14) & 1;

    val = dib8000_read_word(547);
    m->divin_fifo_overflow = val >> 14;
    m->divin_nberr_sym = val & 0x3fff;

    m->adc_power_i = dib8000_read32(384) & 0xfffff;

    m->adc_monitor_only = 0;
    m->adc_power_q = dib8000_read32(386) & 0xfffff;

    m->rf_power = dib8000_read_word(390);

    m->agc1_raw = dib8000_read_word(388);
    m->agc2_raw = dib8000_read_word(389);
    m->agc_wbd_raw = dib8000_read_word(393);
    m->agc_split = (MS_U8) dib8000_read_word(392);

    m->internal_clk = dib8000_read32(23) /1000;

    m->n_2dpa_monitoring = 1;

    val = dib8000_read_word(536);
    m->n_2dpa_mu_int  = (val >> 3) & 0x7f;
    m->n_2dpa_num_fir = (val >> 0) & 0x07;

    for (v = 0; v < 4; v++) {
        MS_U32 v32 = dib8000_read32(459 + v*2);
        m->cti_def_raw[v] = v32 & 0xffff;
        if((v32 >> 16 )&0x1)
            m->cti_def_raw[v] -= (1 << 16);
    }

    demod_monitor_post_process(m);
    return DIB_RETURN_SUCCESS;
}

MS_U8 dib8000_get_signal_status(void)
{
    MS_U16 v = dib8000_read_word(568);
	//status->tps_data_lock  = (v >> 1) & 1; /* TMCC Data lock */
	//status->mpeg_data_lock = !!((v >> 5) & 7); /* if one of the fec_locks is one, we say data is present */

    // only mpeg locked can play the video.
    return !!((v >> 5) & 7)&&((v >> 1) & 1);
}

MS_U32 dib8000_get_signal_quality(void)
{
    return dib8000_read32(560);
}

MS_U16 dib8000_get_signal_strength(void)
{
    return dib8000_read_word(390);
}

MS_U16 dib8000_identify(void)
{
	MS_U16 value = 0;

	//because of glitches sometimes
	value = dib8000_read_word(896);

	if (value != 0x01b3) {
		Out(printf("wrong Vendor ID (read=0x%x)\n", value));
		return 0;
	}

    value = dib8000_read_word(897);
	if (value != 0x8000 &&
        value != 0x8001 &&
        value != 0x8002) {
        Out(printf("wrong Device ID (%x)\n", value));
		return 0;
    }

    switch (value) {
        case 0x8000: Out(printf("found DiB8000A\n")); break;
        case 0x8001: Out(printf("found DiB8000B\n")); break;
        case 0x8002: Out(printf("found DiB8000C\n")); break;
    }
	return value;
}

void dib8000_pwm_agc_reset(struct dibChannel *ch)
{
    dib8000_set_adc_state(DIBX000_ADC_ON);
    dib8000_set_agc_config((unsigned char)(BAND_OF_FREQUENCY(ch->RF_kHz)));
}

int dib8000_reset(void)
{
    dib8000_write_word(1287, 0x0003); /* sram lead in, rdy */

    if((state_dib8000.revision = dib8000_identify()) == 0)
        return 1; // error

	if (state_dib8000.revision == 0x8000)
		Out(printf("error : dib8000 MA not supported"));

    dib8000_set_power_mode(DIB8000M_POWER_ALL);

	/* always leave the VBG voltage on - it consumes almost nothing but takes a long time to start */
	dib8000_set_adc_state(DIBX000_VBG_ENABLE);

	/* restart all parts */
    dib8000_write_word(770, 0xffff);
    dib8000_write_word(771, 0xffff);
    dib8000_write_word(772, 0xfffc);
    dib8000_write_word(898, 0x000c); // sad
    dib8000_write_word(1280, 0x004d);
    dib8000_write_word(1281, 0x000c);

    dib8000_write_word(770, 0x0000);
    dib8000_write_word(771, 0x0000);
    dib8000_write_word(772, 0x0000);
    dib8000_write_word(898, 0x0004); // sad
    dib8000_write_word(1280, 0x0000);
    dib8000_write_word(1281, 0x0000);

    /* drives */
    if (state_dib8000.dib8000_cfg.drives)
        dib8000_write_word(906, state_dib8000.dib8000_cfg.drives);
    else {
        Out(printf("using standard PAD-drive-settings, please adjust settings in config-struct to be optimal."));
        dib8000_write_word(906, 0x2d98); // min drive SDRAM - not optimal - adjust
    }

	dib8000_reset_pll();

	if (dib8000_reset_gpio() != DIB_RETURN_SUCCESS)
		Out(printf("GPIO reset was not successful.\n"));

	if (dib8000_set_output_mode(OUTPUT_MODE_OFF) != DIB_RETURN_SUCCESS)
        Out(printf("OUTPUT_MODE could not be resetted.\n"));

    state_dib8000.p_current_agc = NULL;

	// P_iqc_alpha_pha, P_iqc_alpha_amp, P_iqc_dcc_alpha, ...
    /* P_iqc_ca2 = 0; P_iqc_impnc_on = 0; P_iqc_mode = 0;*/
	if(state_dib8000.dib8000_cfg.pll->ifreq == 0)
		dib8000_write_word(40, 0x0755); /* P_iqc_corr_inh = 0 enable IQcorr block*/
	else
		dib8000_write_word(40, 0x1f55); /* P_iqc_corr_inh = 1 disable IQcorr block*/

	{
		MS_U16 l = 0, r;
          MS_U16 *n;
		n = dib8000_defaults;
		l = *n++;
		while (l) {
			r = *n++;
			do {
				dib8000_write_word(r, *n++);
				r++;
			} while (--l);
			l = *n++;
		}
	}
        state_dib8000.isdbt_cfg_loaded=0;

    //div_cfg override for special configs
	if (state_dib8000.dib8000_cfg.div_cfg != 0)
		dib8000_write_word(903, state_dib8000.dib8000_cfg.div_cfg);

	/* unforce divstr regardless whether i2c enumeration was done or not */
	dib8000_write_word(1285, dib8000_read_word(1285) & ~(1 << 1) );

	dib8000_set_bandwidth(6000);

	dib8000_set_adc_state(DIBX000_SLOW_ADC_ON);
	dib8000_sad_calib();
	dib8000_set_adc_state(DIBX000_SLOW_ADC_OFF);

    demod_set_ber_rs_len(1);

	dib8000_set_power_mode( DIB8000M_POWER_INTERFACE_ONLY);

	return DIB_RETURN_SUCCESS;
}

int dib8000_set_gpio(MS_U8 num, MS_U8 dir, MS_U8 val)
{
	return dib8000_cfg_gpio(num, dir, val);
}

int dib8000_wakeup(void)
{
	dib8000_set_power_mode(DIB8000M_POWER_ALL);
	dib8000_set_adc_state(DIBX000_ADC_ON);
	if (dib8000_set_adc_state(DIBX000_SLOW_ADC_ON) != DIB_RETURN_SUCCESS)
		Out(printf("could not start Slow ADC\n"));

	return DIB_RETURN_SUCCESS;
}

int dib8000_update_pll(struct dibx000_bandwidth_config pll)
{
    dib8000_write_word(901, (1 << 8) | (pll.pll_ratio << 0));
	return DIB_RETURN_SUCCESS;
}

void dib8000_update_timf(void)
{
	MS_U32 timf = state_dib8000.timf = dib8000_read32(435);

	dib8000_write_word(29, (MS_U16) (timf >> 16));
	dib8000_write_word(30, (MS_U16) (timf & 0xffff));
	Out(printf("Updated timing frequency: %d (default: %d)\n",state_dib8000.timf, state_dib8000.timf_default));
}

int dib8000_reset_gpio(void)
{
	/* reset the GPIOs */
	dib8000_write_word(1029, state_dib8000.dib8000_cfg.gpio_dir);
	dib8000_write_word(1030, state_dib8000.dib8000_cfg.gpio_val);

	/* TODO 782 is P_gpio_od */

	dib8000_write_word(1032, state_dib8000.dib8000_cfg.gpio_pwm_pos);

	dib8000_write_word(1037, state_dib8000.dib8000_cfg.pwm_freq_div);
	return DIB_RETURN_SUCCESS;
}

void dib8000_set_acquisition_mode(void)
{
    MS_U16 nud = dib8000_read_word(298);
    nud |= (1 << 3) | (1 << 0);
    Out(printf("acquisition mode activated"));
    dib8000_write_word(298, nud);
}

int dib8000_set_output_mode(int mode)
{
    MS_U16 outreg, fifo_threshold, smo_mode,
             sram = 0x0205; /* by default SDRAM deintlv is enabled */

    outreg = 0;
    fifo_threshold = 1792;
    smo_mode = (dib8000_read_word(299) & 0x0050) | (1 << 1);

    switch (mode) {
    case OUTPUT_MODE_TS_PARALLEL_GATED:   // STBs with parallel gated clock
        outreg = (1 << 10);  /* 0x0400 */
        break;
    case OUTPUT_MODE_TS_PARALLEL_CONT:    // STBs with parallel continues clock
        outreg = (1 << 10) | (1 << 6); /* 0x0440 */
        break;
    case OUTPUT_MODE_TS_SERIAL:          // STBs with serial input
        outreg = (1 << 10) | (2 << 6) | (0 << 1); /* 0x0482 */
        break;
    case OUTPUT_MODE_DIVERSITY:
        if (state_dib8000.dib8000_cfg.hostbus_diversity) {
            outreg = (1 << 10) | (4 << 6); /* 0x0500 */
            sram &= 0xfdff;
        } else
            sram |= 0x0c00;
        break;
    case OUTPUT_MODE_TS_FIFO:            // e.g. USB feeding
        smo_mode |= (3 << 1);
        fifo_threshold = 512;
        outreg = (1 << 10) | (5 << 6);
        break;
    case OUTPUT_MODE_OFF:  // disable
        outreg = 0;
        break;

    case OUTPUT_MODE_ANALOG_ADC:
        outreg = (1 << 10) | (3 << 6);
        dib8000_set_acquisition_mode();
        break;

    default:
        Out(printf("Unhandled output_mode passed to be set for demod\n"));
        return DIB_RETURN_ERROR;
    }

    if (state_dib8000.dib8000_cfg.output_mpeg2_in_188_bytes)
        smo_mode |= (1 << 5) ;

    dib8000_write_word(299, smo_mode);
    dib8000_write_word(300, fifo_threshold); /* synchronous fread */
    dib8000_write_word(1286, outreg);
    dib8000_write_word(1291, sram);

    return DIB_RETURN_SUCCESS;
}

int dib8000_set_bandwidth(MS_U32 bw)
{
	MS_U32 timf;

	if (bw == 0)
		bw = 6000;

	if (state_dib8000.timf == 0) {
		Out(printf("using default timf\n"));
		timf = state_dib8000.timf_default;
	} else {
		Out(printf("using updated timf\n"));
		timf = state_dib8000.timf;
	}

	dib8000_write_word(29, (MS_U16) ((timf >> 16) & 0xffff));
	dib8000_write_word(30, (MS_U16) ((timf      ) & 0xffff));

	return DIB_RETURN_SUCCESS;
}

int dib8000_sad_calib(void)
{
/* internal */
	dib8000_write_word(923, (0 << 1) | (0 << 0));
	dib8000_write_word(924, 776); // 0.625*3.3 / 4096

	/* do the calibration */
	dib8000_write_word(923, (1 << 0));
	dib8000_write_word(923, (0 << 0));

	DibMSleep(1);
	return DIB_RETURN_SUCCESS;
}

void dib8000_reset_pll(void)
{
	struct dibx000_bandwidth_config *pll = state_dib8000.dib8000_cfg.pll;
	MS_U16 clk_cfg1;

	// clk_cfg0
	dib8000_write_word(901, (pll->pll_prediv << 8) | (pll->pll_ratio << 0));

	// clk_cfg1
	clk_cfg1 = (0 << 10) | (0 << 9) | (pll->IO_CLK_en_core << 8) |
		(pll->bypclk_div << 5) | (pll->enable_refdiv << 4) |
		(1 << 3) | (pll->pll_range << 1) | (pll->pll_reset << 0);

	dib8000_write_word(902, clk_cfg1);
	clk_cfg1 = (clk_cfg1 & 0xfff7) | (pll->pll_bypass << 3);
	dib8000_write_word(902, clk_cfg1);

	Out(printf("clk_cfg1: 0x%04x\n", clk_cfg1)); /* 0x507 1 0 1 000 0 0 11 1 */

	/* smpl_cfg: P_refclksel=2, P_ensmplsel=1 nodivsmpl=1*/
    if (state_dib8000.dib8000_cfg.pll->ADClkSrc == 0)
        dib8000_write_word(904, (0 << 15) |  (0 << 12) | (0 << 10) | (pll->modulo << 8) | (pll->ADClkSrc << 7) | (0 << 1));
    else if (state_dib8000.dib8000_cfg.refclksel != 0)
        dib8000_write_word(904, (0 << 15) |  (1 << 12) | ((state_dib8000.dib8000_cfg.refclksel&0x3) << 10) | (pll->modulo << 8) | (pll->ADClkSrc << 7) | (0 << 1));
    else
        dib8000_write_word(904, (0 << 15) |  (1 << 12) | (3 << 10) | (pll->modulo << 8) | (pll->ADClkSrc << 7) | (0 << 1));

    dib8000_reset_pll_common(pll);
}

int dib8000_set_adc_state(enum dibx000_adc_states no)
{
	int ret = 0;
	MS_U16 reg_907 = dib8000_read_word(907),
	       reg_908 = dib8000_read_word(908);

	switch (no) {
		case DIBX000_SLOW_ADC_ON:
			reg_908 |= (1 << 1) | (1 << 0);
			ret |= dib8000_write_word(908, reg_908);
			reg_908 &= ~(1 << 1);
			break;

		case DIBX000_SLOW_ADC_OFF:
			reg_908 |=  (1 << 1) | (1 << 0);
			break;

		case DIBX000_ADC_ON:
			reg_907 &= 0x0fff;
			reg_908 &= 0x0003;
			break;

		case DIBX000_ADC_OFF: // leave the VBG voltage on
			reg_907 |= (1 << 14) | (1 << 13) | (1 << 12);
			reg_908 |= (1 << 5) | (1 << 4) | (1 << 3) | (1 << 2);
			break;

		case DIBX000_VBG_ENABLE:
			reg_907 &= ~(1 << 15);
			break;

		case DIBX000_VBG_DISABLE:
			reg_907 |= (1 << 15);
			break;

		default:
			break;
	}

	ret |= dib8000_write_word(907, reg_907);
	ret |= dib8000_write_word(908, reg_908);

	return ret;
}

void dib8000_reset_pll_common(struct dibx000_bandwidth_config *bw)
{
	Out(printf("ifreq: %d %x, inversion: %d\n", bw->ifreq, bw->ifreq, bw->ifreq >> 25));
	dib8000_write_word(23, (MS_U16)(((bw->internal*1000) >> 16) & 0xffff)); /* P_sec_len */
	dib8000_write_word(24,  (MS_U16)((bw->internal*1000)        & 0xffff));
    dib8000_write_word(27, (MS_U16)(( bw->ifreq          >> 16) & 0x01ff));
	dib8000_write_word(28,   (MS_U16)(bw->ifreq                 & 0xffff));
    dib8000_write_word(26,  (MS_U16)((bw->ifreq          >> 25) & 0x0003));

    dib8000_write_word(922, bw->sad_cfg);
}

void dib8000_set_power_mode(enum dib8000_power_mode mode)
{
	/* by default everything is going to be powered off */
	MS_U16 reg_774 = 0x3fff, reg_775 = 0xffff, reg_776 = 0xffff,
		reg_900  = (dib8000_read_word(900)  & 0xfffc) | 0x3,
		reg_1280 = (dib8000_read_word(1280) & 0x00ff) | 0xff00;

	/* now, depending on the requested mode, we power on */
	switch (mode) {
		/* power up everything in the demod */
		case DIB8000M_POWER_ALL:
            reg_774 = 0x0000; reg_775 = 0x0000; reg_776 = 0x0000; reg_900 &= 0xfffc;
			reg_1280 &= 0x00ff;
			break;
		case DIB8000M_POWER_INTERFACE_ONLY:
            reg_1280 &= 0x00ff;
			break;
    }

    Out(printf("powermode : 774 : %x ; 775 : %x; 776 : %x ; 900 : %x; 1280 : %x\n", reg_774, reg_775, reg_776, reg_900, reg_1280));
	dib8000_write_word( 774, reg_774);
	dib8000_write_word( 775, reg_775);
	dib8000_write_word( 776, reg_776);
	dib8000_write_word( 900, reg_900);
    dib8000_write_word(1280, reg_1280);
}

int dib8000_set_wbd_ref(MS_U16 value)
{
	if (value > 4095)
		value = 4095;
	state_dib8000.wbd_ref = value;
	return dib8000_write_word(106, value);
}

int dib8000_set_agc_config(MS_U8 band)
{
	  struct dibx000_agc_config *agc = NULL;
	int i;
	if (state_dib8000.current_band == band  && state_dib8000.p_current_agc != NULL)
		return DIB_RETURN_SUCCESS;
	state_dib8000.current_band = band;

	for (i = 0; i < state_dib8000.dib8000_cfg.agc_config_count; i++)
		if (state_dib8000.dib8000_cfg.agc[i].band_caps & band) {
			agc = &state_dib8000.dib8000_cfg.agc[i];
			break;
		}

	if (agc == NULL) {
		Out(printf("no valid AGC configuration found for band 0x%02x\n",band));
		return DIB_RETURN_ERROR;
	}

	state_dib8000.p_current_agc = agc;

	/* AGC */
	dib8000_write_word(76 ,  agc->setup);
	dib8000_write_word(77 ,  agc->inv_gain);
	dib8000_write_word(78 ,  agc->time_stabiliz);
	dib8000_write_word(101 , (agc->alpha_level << 12) | agc->thlock);

	// Demod AGC loop configuration
	dib8000_write_word(102, (agc->alpha_mant << 5) | agc->alpha_exp);
	dib8000_write_word(103, (agc->beta_mant  << 6) | agc->beta_exp);

	Out(printf("WBD: ref: %d, sel: %d, active: %d, alpha: %d\n",
		state_dib8000.wbd_ref != 0 ? state_dib8000.wbd_ref : agc->wbd_ref, agc->wbd_sel, !agc->perform_agc_softsplit, agc->wbd_sel));

	/* AGC continued */
	if (state_dib8000.wbd_ref != 0)
		dib8000_write_word(106, state_dib8000.wbd_ref);
	else // use default
		dib8000_write_word(106, agc->wbd_ref);
	dib8000_write_word(107, (agc->wbd_alpha << 9) | (agc->perform_agc_softsplit << 8) );
	dib8000_write_word(108,  agc->agc1_max);
	dib8000_write_word(109,  agc->agc1_min);
	dib8000_write_word(110,  agc->agc2_max);
	dib8000_write_word(111,  agc->agc2_min);
	dib8000_write_word(112, (agc->agc1_pt1 << 8) | agc->agc1_pt2 );
	dib8000_write_word(113, (agc->agc1_slope1 << 8) | agc->agc1_slope2);
	dib8000_write_word(114, (agc->agc2_pt1 << 8) | agc->agc2_pt2);
	dib8000_write_word(115, (agc->agc2_slope1 << 8) | agc->agc2_slope2);

	dib8000_write_word(75,   agc->agc1_pt3);
	dib8000_write_word(923, (dib8000_read_word(923) & 0xffe3) | (agc->wbd_inv << 4) | (agc->wbd_sel << 2)); /*LB : 929 -> 923*/

	return DIB_RETURN_SUCCESS;
}

void dib8000_restart_agc(void)
{
	// P_restart_iqc & P_restart_agc
	dib8000_write_word(770, 0x0a00);
	dib8000_write_word(770, 0x0000);
}

int dib8000_update_lna(void)
{
	return 0;
}

int dib8000_set_diversity_in(int onoff)
{
    MS_U16 sync_wait = dib8000_read_word(273) & 0xfff0;

    if (!state_dib8000.differential_constellation) {
        dib8000_write_word(272, 1 << 9); //dvsy_off_lmod4 = 1
        dib8000_write_word(273, sync_wait | (1 << 2) | 2); // sync_enable = 1; comb_mode = 2
    } else {
        dib8000_write_word(272, 0); //dvsy_off_lmod4 = 0
        dib8000_write_word(273, sync_wait); // sync_enable = 0; comb_mode = 0
    }
    state_dib8000.diversity_onoff = onoff;

    switch (onoff) {
    case 0: /* only use the internal way - not the diversity input */
        dib8000_write_word(270, 1);
        dib8000_write_word(271, 0);
        break;
    case 1: /* both ways */
        dib8000_write_word(270, 6);
        dib8000_write_word(271, 6);
        break;
    case 2: /* only the diversity input */
        dib8000_write_word(270, 0);
        dib8000_write_word(271, 1);
        break;
    }
    return DIB_RETURN_SUCCESS;
}

void dib8000_set_channel(struct dibChannel *ch, MS_U8 seq, MS_U8 autosearching)
{
    MS_U16 mode, max_constellation, seg_diff_mask = 0, nbseg_diff = 0 ;
    MS_U8  guard, crate, constellation, timeI;
    MS_U8  permu_seg[] = { 6, 5, 7, 4, 8, 3, 9, 2, 10, 1, 11, 0, 12};
    MS_U16 i, coeff[4], P_cfr_left_edge  = 0, P_cfr_right_edge = 0, seg_mask13 = 0x1fff;  // All 13 segments enabled
      MS_S16 *ncoeff = NULL, *ana_fe = NULL;
    MS_U16 tmcc_pow = 0;
    MS_U16 coff_pow = 0x2800;
    MS_U16 init_prbs = 0xfff;
    MS_U16 ana_gain = 0;
	MS_U16 adc_target_16dB[11] = {
	  (1 << 13) - 825 - 117,
	  (1 << 13) - 837 - 117,
	  (1 << 13) - 811 - 117,
	  (1 << 13) - 766 - 117,
	  (1 << 13) - 737 - 117,
	  (1 << 13) - 693 - 117,
	  (1 << 13) - 648 - 117,
	  (1 << 13) - 619 - 117,
	  (1 << 13) - 575 - 117,
	  (1 << 13) - 531 - 117,
	  (1 << 13) - 501 - 117};


    if (state_dib8000.ber_monitored_layer != LAYER_ALL)
        dib8000_write_word(285, (dib8000_read_word(285) & 0x60)|state_dib8000.ber_monitored_layer);
    else
        dib8000_write_word(285, dib8000_read_word(285) & 0x60);

    i = dib8000_read_word(26) & 1; // P_dds_invspec
    dib8000_write_word(26, ch->u.isdbt.spectrum_inversion^i);

    if (ch->u.isdbt.sb_mode) {
        //compute new dds_freq for the seg and adjust prbs
        int seg_offset = ch->u.isdbt.sb_wanted_seg - (ch->u.isdbt.sb_conn_total_seg/2) - (ch->u.isdbt.sb_conn_total_seg%2);
        int clk =state_dib8000.dib8000_cfg.pll->internal;
        MS_U32 segtodds = ((MS_U32)(430 << 23) / clk ) << 3; // segtodds = SegBW / Fclk * pow(2,26)
        int dds_offset = seg_offset * segtodds;
        int new_dds, sub_channel;
        if ((ch->u.isdbt.sb_conn_total_seg%2) == 0) // if even
            dds_offset -= (int)(segtodds / 2);

        if (state_dib8000.dib8000_cfg.pll->ifreq == 0) {
            if ((ch->u.isdbt.spectrum_inversion^i) == 0) {
                dib8000_write_word(26, dib8000_read_word(26) | 1);
                new_dds = dds_offset;
            } else
                new_dds = dds_offset;

            // We shift tuning frequency if the wanted segment is :
            //  - the segment of center frequency with an odd total number of segments
            //  - the segment to the left of center frequency with an even total number of segments
            //  - the segment to the right of center frequency with an even total number of segments
            if (  (ch->type == STANDARD_ISDBT) && (ch->u.isdbt.sb_mode == 1)
                    && (((ch->u.isdbt.sb_conn_total_seg%2)       && (ch->u.isdbt.sb_wanted_seg == ((ch->u.isdbt.sb_conn_total_seg/2)+1)))
                        || (((ch->u.isdbt.sb_conn_total_seg%2) == 0) && (ch->u.isdbt.sb_wanted_seg ==  (ch->u.isdbt.sb_conn_total_seg/2)))
                        || (((ch->u.isdbt.sb_conn_total_seg%2) == 0) && (ch->u.isdbt.sb_wanted_seg == ((ch->u.isdbt.sb_conn_total_seg/2)+1)))
                       ))
            {
                new_dds -= ((MS_U32)(850 << 22) / clk ) << 4; // new_dds = 850 (freq shift in KHz) / Fclk * pow(2,26)
            }
        } else {
            if ((ch->u.isdbt.spectrum_inversion^i) == 0)
                new_dds = state_dib8000.dib8000_cfg.pll->ifreq - dds_offset;
            else
                new_dds = state_dib8000.dib8000_cfg.pll->ifreq + dds_offset;
        }
        dib8000_write_word(27, (MS_U16)((new_dds >> 16) & 0x01ff));
        dib8000_write_word(28, (MS_U16)( new_dds        & 0xffff));
        if (ch->u.isdbt.sb_conn_total_seg%2) // if odd
            sub_channel = ((ch->u.isdbt.sb_subchannel+(3*seg_offset)    +1 )%41)/3;
        else // if even
            sub_channel = ((ch->u.isdbt.sb_subchannel+(3*seg_offset)       )%41)/3;
        sub_channel -= 6;

        if (ch->u.isdbt.nfft == FFT_2K || ch->u.isdbt.nfft == FFT_4K) {
            dib8000_write_word(219, dib8000_read_word(219) | 0x1); //adp_pass =1
            dib8000_write_word(190, dib8000_read_word(190) | (0x1 << 14)); //pha3_force_pha_shift = 1
        } else {
            dib8000_write_word(219, dib8000_read_word(219) & 0xfffe); //adp_pass =0
            dib8000_write_word(190, dib8000_read_word(190) & 0xbfff); //pha3_force_pha_shift = 0
        }

        switch (ch->u.isdbt.nfft) {
            case FFT_2K :
                switch (sub_channel) {
                    case -6 :  init_prbs = 0x0   ; break; // 41, 0, 1
                    case -5 :  init_prbs = 0x423 ; break; // 02~04
                    case -4 :  init_prbs = 0x9   ; break; // 05~07
                    case -3 :  init_prbs = 0x5C7 ; break; // 08~10
                    case -2 :  init_prbs = 0x7A6 ; break; // 11~13
                    case -1 :  init_prbs = 0x3D8 ; break; // 14~16
                    case  0 :  init_prbs = 0x527 ; break; // 17~19
                    case  1 :  init_prbs = 0x7FF ; break; // 20~22
                    case  2 :  init_prbs = 0x79B ; break; // 23~25
                    case  3 :  init_prbs = 0x3D6 ; break; // 26~28
                    case  4 :  init_prbs = 0x3A2 ; break; // 29~31
                    case  5 :  init_prbs = 0x53B ; break; // 32~34
                    case  6 :  init_prbs = 0x2F4 ; break; // 35~37
                    default:
                    case  7 :  init_prbs = 0x213 ; break; // 38~40
                }
                break;

            case FFT_4K :
                switch (sub_channel) {
                    case -6 :  init_prbs = 0x0   ; break; // 41, 0, 1
                    case -5 :  init_prbs = 0x208 ; break; // 02~04
                    case -4 :  init_prbs = 0xC3  ; break; // 05~07
                    case -3 :  init_prbs = 0x7B9 ; break; // 08~10
                    case -2 :  init_prbs = 0x423 ; break; // 11~13
                    case -1 :  init_prbs = 0x5C7 ; break; // 14~16
                    case  0 :  init_prbs = 0x3D8 ; break; // 17~19
                    case  1 :  init_prbs = 0x7FF ; break; // 20~22
                    case  2 :  init_prbs = 0x3D6 ; break; // 23~25
                    case  3 :  init_prbs = 0x53B ; break; // 26~28
                    case  4 :  init_prbs = 0x213 ; break; // 29~31
                    case  5 :  init_prbs = 0x29  ; break; // 32~34
                    case  6 :  init_prbs = 0xD0  ; break; // 35~37
                    default:
                    case  7 :  init_prbs = 0x48E ; break; // 38~40
                }
                break;

            default:
            case FFT_8K :
                switch (sub_channel) {
                    case -6 :  init_prbs = 0x0   ; break; // 41, 0, 1
                    case -5 :  init_prbs = 0x740 ; break; // 02~04
                    case -4 :  init_prbs = 0x069 ; break; // 05~07
                    case -3 :  init_prbs = 0x7DD ; break; // 08~10
                    case -2 :  init_prbs = 0x208 ; break; // 11~13
                    case -1 :  init_prbs = 0x7B9 ; break; // 14~16
                    case  0 :  init_prbs = 0x5C7 ; break; // 17~19
                    case  1 :  init_prbs = 0x7FF ; break; // 20~22
                    case  2 :  init_prbs = 0x53B ; break; // 23~25
                    case  3 :  init_prbs = 0x29  ; break; // 26~28
                    case  4 :  init_prbs = 0x48E ; break; // 29~31
                    case  5 :  init_prbs = 0x4C4 ; break; // 32~34
                    case  6 :  init_prbs = 0x367 ; break; // 33~37
                    default:
                    case  7 :  init_prbs = 0x684 ; break; // 38~40
                }
                break;
        }
    } else { // if not ch->u.isdbt.sb_mode
        dib8000_write_word(27, (MS_U16)(( state_dib8000.dib8000_cfg.pll->ifreq          >> 16) & 0x01ff));
        dib8000_write_word(28,   (MS_U16)(state_dib8000.dib8000_cfg.pll->ifreq                 & 0xffff));
        dib8000_write_word(26,  (MS_U16)((state_dib8000.dib8000_cfg.pll->ifreq          >> 25) & 0x0003));
    }
    /*P_mode == ??*/
    dib8000_write_word(10, (seq << 4));
    //	dib8000_write_word(state, 287, (dib8000_read_word(state, 287) & 0xe000) | 0x1000);

    switch(ch->u.isdbt.guard) {
        case GUARD_INTERVAL_1_32: guard = 0; break;
        case GUARD_INTERVAL_1_16:  guard = 1; break;
        case GUARD_INTERVAL_1_8: guard = 2; break;
        case GUARD_INTERVAL_1_4:
        default :       guard = 3; break;
    }

    dib8000_write_word(1, (init_prbs<<2) | (guard & 0x3)); // ADDR 1

    max_constellation = QAM_DQPSK;
    for (i = 0 ; i < 3; i++) {
        switch(ch->u.isdbt.layer[i].constellation) {
            case QAM_DQPSK: constellation = 0; break;
            case QAM_QPSK:  constellation = 1; break;
            case QAM_16QAM: constellation = 2; break;
            case QAM_64QAM:
            default :       constellation = 3; break;
        }

        switch(ch->u.isdbt.layer[i].code_rate) {
            case VIT_CODERATE_1_2: crate = 1; break;
            case VIT_CODERATE_2_3: crate = 2; break;
            case VIT_CODERATE_3_4: crate = 3; break;
            case VIT_CODERATE_5_6: crate = 5; break;
            case VIT_CODERATE_7_8:
            default :              crate = 7; break;
        }

        if ( (ch->u.isdbt.layer[i].time_intlv >=0) &&
             ((ch->u.isdbt.layer[i].time_intlv <= 3) ||
              (ch->u.isdbt.layer[i].time_intlv == 4 && ch->u.isdbt.sb_mode == 1))
             )
          timeI = ch->u.isdbt.layer[i].time_intlv;
        else
          timeI = 0;
        dib8000_write_word(2+i, (constellation << 10) | ((ch->u.isdbt.layer[i].nb_segments & 0xf) << 6) |
                (crate << 3) | timeI );
        if (ch->u.isdbt.layer[i].nb_segments > 0) {
            switch (max_constellation) {
                case QAM_DQPSK:
                case QAM_QPSK:
                    if (ch->u.isdbt.layer[i].constellation == QAM_16QAM ||
                            ch->u.isdbt.layer[i].constellation == QAM_64QAM)
                        max_constellation = ch->u.isdbt.layer[i].constellation;
                    break;
                case QAM_16QAM:
                    if (ch->u.isdbt.layer[i].constellation == QAM_64QAM)
                        max_constellation = ch->u.isdbt.layer[i].constellation;
                    break;
            }
        }
    }

    mode = fft_to_mode(ch);

    //dib8000_write_word(state, 5, 13); /*p_last_seg = 13*/

    dib8000_write_word(274, (dib8000_read_word(274) & 0xffcf) |
            ((ch->u.isdbt.partial_reception & 1) << 5) | ((ch->u.isdbt.sb_mode & 1) << 4));

    Out(printf("mode = %d ; guard = %d\n", mode, ch->u.isdbt.guard));

    /* signal optimization parameter */

    if (ch->u.isdbt.partial_reception) {
        seg_diff_mask = (ch->u.isdbt.layer[0].constellation == QAM_DQPSK) << permu_seg[0];
        for (i = 1; i < 3; i++)
            nbseg_diff += (ch->u.isdbt.layer[i].constellation == QAM_DQPSK) * ch->u.isdbt.layer[i].nb_segments;
        for (i= 0; i < nbseg_diff; i++)
            seg_diff_mask |= 1 << permu_seg[i+1];
    } else {
        for (i = 0; i < 3; i++)
            nbseg_diff += (ch->u.isdbt.layer[i].constellation == QAM_DQPSK) * ch->u.isdbt.layer[i].nb_segments;
        for (i= 0 ; i < nbseg_diff; i++)
            seg_diff_mask |= 1 << permu_seg[i];
    }
    Out(printf("nbseg_diff = %X (%d)\n", seg_diff_mask, seg_diff_mask));

    state_dib8000.differential_constellation = (seg_diff_mask != 0);
    dib8000_set_diversity_in(state_dib8000.diversity_onoff);

    if (ch->u.isdbt.sb_mode == 1) { // ISDB-Tsb
        if (ch->u.isdbt.partial_reception == 1) // 3-segments
            seg_mask13 = 0x00E0;
        else // 1-segment
            seg_mask13 =  0x0040;
    } else
        seg_mask13 =  0x1fff;

    // WRITE: Mode & Diff mask
    dib8000_write_word(0, (mode << 13) | seg_diff_mask);

    if ((seg_diff_mask) || (ch->u.isdbt.sb_mode))
        dib8000_write_word(268, (dib8000_read_word(268) & 0xF9FF) | 0x0200);
    else
        dib8000_write_word(268, (2 << 9) | 39); //init value

    // ---- SMALL ----
    // P_small_seg_diff
    dib8000_write_word(352, seg_diff_mask); // ADDR 352

    dib8000_write_word(353, seg_mask13); // ADDR 353

/*     // P_small_narrow_band=0, P_small_last_seg=13, P_small_offset_num_car=5 */
/*     dib8000_write_word(state, 351, (ch->u.isdbt.sb_mode << 8) | (13 << 4) | 5 ); */

    // ---- SMALL ----
    if (ch->u.isdbt.sb_mode == 1) {
      switch(ch->u.isdbt.nfft) {
      case FFT_2K:
        if (ch->u.isdbt.partial_reception == 0) {// 1-seg
          if (ch->u.isdbt.layer[0].constellation == QAM_DQPSK) // DQPSK
            ncoeff = coeff_2k_sb_1seg_dqpsk;
          else // QPSK or QAM
            ncoeff = coeff_2k_sb_1seg;
        } else { // 3-segments
          if (ch->u.isdbt.layer[0].constellation == QAM_DQPSK) { // DQPSK on central segment
            if (ch->u.isdbt.layer[1].constellation == QAM_DQPSK) // DQPSK on external segments
              ncoeff = coeff_2k_sb_3seg_0dqpsk_1dqpsk;
            else // QPSK or QAM on external segments
              ncoeff = coeff_2k_sb_3seg_0dqpsk;
          } else { // QPSK or QAM on central segment
            if (ch->u.isdbt.layer[1].constellation == QAM_DQPSK) // DQPSK on external segments
              ncoeff = coeff_2k_sb_3seg_1dqpsk;
            else // QPSK or QAM on external segments
              ncoeff = coeff_2k_sb_3seg;
          }
        }
        break;

      case FFT_4K:
        if (ch->u.isdbt.partial_reception == 0) { // 1-seg
          if (ch->u.isdbt.layer[0].constellation == QAM_DQPSK) // DQPSK
            ncoeff = coeff_4k_sb_1seg_dqpsk;
          else // QPSK or QAM
            ncoeff = coeff_4k_sb_1seg;
        } else { // 3-segments
          if (ch->u.isdbt.layer[0].constellation == QAM_DQPSK) { // DQPSK on central segment
            if (ch->u.isdbt.layer[1].constellation == QAM_DQPSK) { // DQPSK on external segments
              /*                             // USE_CP = 0 */
              /*                             dib8000_write_word(state, 341, (4<<3) | (1<<2)  | (1<<1) | 0); */
              ncoeff = coeff_4k_sb_3seg_0dqpsk_1dqpsk;
            } else { // QPSK or QAM on external segments
              /*                             // USE_CP = 0 */
              /*                             dib8000_write_word(state, 341, (4<<3) | (1<<2)  | (1<<1) | 0); */
              ncoeff = coeff_4k_sb_3seg_0dqpsk;
            }
          } else { // QPSK or QAM on central segment
            if (ch->u.isdbt.layer[1].constellation == QAM_DQPSK) { // DQPSK on external segments
              /*                             // USE_CP = 0 */
              /*                             dib8000_write_word(state, 341, (4<<3) | (1<<2)  | (1<<1) | 0); */
              ncoeff = coeff_4k_sb_3seg_1dqpsk;
            } else // QPSK or QAM on external segments
              ncoeff = coeff_4k_sb_3seg;
          }
        }
        break;

      case FFT_AUTO:
      case FFT_8K:
      default:
        if (ch->u.isdbt.partial_reception == 0) { // 1-seg
          if (ch->u.isdbt.layer[0].constellation == QAM_DQPSK) // DQPSK
            ncoeff = coeff_8k_sb_1seg_dqpsk;
          else // QPSK or QAM
            ncoeff = coeff_8k_sb_1seg;
        } else { // 3-segments
          if (ch->u.isdbt.layer[0].constellation == QAM_DQPSK) { // DQPSK on central segment
            if (ch->u.isdbt.layer[1].constellation == QAM_DQPSK) { // DQPSK on external segments
              /*                             // USE_CP = 0 */
              /*                             dib8000_write_word(state, 341, (4<<3) | (1<<2)  | (1<<1) | 0); */
              ncoeff = coeff_8k_sb_3seg_0dqpsk_1dqpsk;
            } else { // QPSK or QAM on external segments
              /*                             // USE_CP = 0 */
              /*                             dib8000_write_word(state, 341, (4<<3) | (1<<2)  | (1<<1) | 0); */
              ncoeff = coeff_8k_sb_3seg_0dqpsk;
            }
          } else { // QPSK or QAM on central segment
            if (ch->u.isdbt.layer[1].constellation == QAM_DQPSK) { // DQPSK on external segments
              /*                             // USE_CP = 0 */
              /*                             dib8000_write_word(state, 341, (4<<3) | (1<<2)  | (1<<1) | 0); */
              ncoeff = coeff_8k_sb_3seg_1dqpsk;
            } else // QPSK or QAM on external segments
              ncoeff = coeff_8k_sb_3seg;
          }
        }
        break;
      }
    }
    if (ch->u.isdbt.sb_mode == 1)
      for (i = 0; i < 8; i++)
        dib8000_write_word(343 + i, ncoeff[i]);

    // P_small_coef_ext_enable=ISDB-Tsb, P_small_narrow_band=ISDB-Tsb, P_small_last_seg=13, P_small_offset_num_car=5
    dib8000_write_word(351, (ch->u.isdbt.sb_mode << 9) | (ch->u.isdbt.sb_mode << 8) | (13 << 4) | 5);

    // ---- COFF ----
    // Carloff, the most robust
    if (ch->u.isdbt.sb_mode == 1) {  // Sound Broadcasting mode - use both TMCC and AC pilots

        // P_coff_cpil_alpha=4, P_coff_inh=0, P_coff_cpil_winlen=64
        // P_coff_narrow_band=1, P_coff_square_val=1, P_coff_one_seg=~partial_rcpt, P_coff_use_tmcc=1, P_coff_use_ac=1
        dib8000_write_word(187, (4<<12) | (0<<11) | (63<<5) | (0x3 << 3) |((~ch->u.isdbt.partial_reception & 1) << 2) | 0x3);


/*             // P_small_coef_ext_enable = 1 */
/*             dib8000_write_word(state, 351, dib8000_read_word(state, 351) | 0x200); */

        if (ch->u.isdbt.partial_reception == 0) {   // Sound Broadcasting mode 1 seg

          // P_coff_winlen=63, P_coff_thres_lock=15, P_coff_one_seg_width= (P_mode == 3) , P_coff_one_seg_sym= (P_mode-1)
          if (mode == 3)
            dib8000_write_word(180, 0x1fcf | ((mode-1) << 14));
          else
            dib8000_write_word(180, 0x0fcf | ((mode-1) << 14));
          // P_ctrl_corm_thres4pre_freq_inh=1,P_ctrl_pre_freq_mode_sat=1,
          // P_ctrl_pre_freq_inh=0, P_ctrl_pre_freq_step = 5, P_pre_freq_win_len=4
          dib8000_write_word(338, (1 << 12) | (1 << 10) | (0 << 9) | (5 << 5) | 4);
          // P_ctrl_pre_freq_win_len=16, P_ctrl_pre_freq_thres_lockin=8
          dib8000_write_word(340, (16 << 6) | (8 << 0));
          // P_ctrl_pre_freq_thres_lockout=6, P_small_use_tmcc/ac/cp=1
          dib8000_write_word(341, ( 6 << 3) | (1 << 2) | (1 << 1) | (1 << 0));

          // P_coff_corthres_8k, 4k, 2k and P_coff_cpilthres_8k, 4k, 2k
          dib8000_write_word(181, 300);
          dib8000_write_word(182, 150);
          dib8000_write_word(183, 80 );
          dib8000_write_word(184, 300);
          dib8000_write_word(185, 150);
          dib8000_write_word(186, 80 );
        } else {   // Sound Broadcasting mode 3 seg
          // P_coff_one_seg_sym= 1, P_coff_one_seg_width= 1, P_coff_winlen=63, P_coff_thres_lock=15
          /*                 if (mode == 3) */
          /*                     dib8000_write_word(state, 180, 0x2fca | ((0) << 14)); */
          /*                 else */
          /*                     dib8000_write_word(state, 180, 0x2fca | ((1) << 14)); */
          dib8000_write_word(180, 0x1fcf | (1 << 14));

          // P_ctrl_corm_thres4pre_freq_inh = 1, P_ctrl_pre_freq_mode_sat=1,
          // P_ctrl_pre_freq_inh=0, P_ctrl_pre_freq_step = 4, P_pre_freq_win_len=4
          dib8000_write_word(338, (1 << 12) | (1 << 10) | (0 << 9) | (4 << 5) | 4);
          // P_ctrl_pre_freq_win_len=16, P_ctrl_pre_freq_thres_lockin=8
          dib8000_write_word(340, (16 << 6) | (8 << 0));
          //P_ctrl_pre_freq_thres_lockout=6, P_small_use_tmcc/ac/cp=1
          dib8000_write_word(341, ( 6 << 3) | (1 << 2) | (1 << 1) | (1 << 0));

          // P_coff_corthres_8k, 4k, 2k and P_coff_cpilthres_8k, 4k, 2k
          dib8000_write_word(181, 350);
          dib8000_write_word(182, 300);
          dib8000_write_word(183, 250);
          dib8000_write_word(184, 350);
          dib8000_write_word(185, 300);
          dib8000_write_word(186, 250);
        }

    }
    else if (state_dib8000.isdbt_cfg_loaded == 0) {  // if not Sound Broadcasting mode : put default values for 13 segments
      dib8000_write_word(180, (16 << 6) | 9 );
      dib8000_write_word(187, (4 << 12) | (8 << 5) | 0x2 );
      coff_pow = 0x2800;
      for (i=0;i<6;i++)
        dib8000_write_word(181+i, coff_pow);

      // P_ctrl_corm_thres4pre_freq_inh=1, P_ctrl_pre_freq_mode_sat=1,
      // P_ctrl_pre_freq_mode_sat=1, P_ctrl_pre_freq_inh=0, P_ctrl_pre_freq_step = 3, P_pre_freq_win_len=1
      dib8000_write_word(338, (1 << 12) | (1 << 10) | (0 << 9) | (3 << 5) | 1);

      // P_ctrl_pre_freq_win_len=8, P_ctrl_pre_freq_thres_lockin=6
      dib8000_write_word(340, (8 << 6) | (6 << 0));
      // P_ctrl_pre_freq_thres_lockout=4, P_small_use_tmcc/ac/cp=1
      dib8000_write_word(341, (4 << 3) | (1 << 2) | (1 << 1) | (1 << 0));
    }

    // ---- FFT ----
    if (ch->u.isdbt.sb_mode == 1 && ch->u.isdbt.partial_reception == 0) // 1-seg
        dib8000_write_word(178, 64); // P_fft_powrange=64
    else
      dib8000_write_word(178, 32); // P_fft_powrange=32

    /* make the cpil_coff_lock more robust but slower p_coff_winlen
     * 6bits; p_coff_thres_lock 6bits (for coff lock if needed)
     */
    /* if ( ( nbseg_diff>0)&&(nbseg_diff<13))
       dib8000_write_word(state, 187, (dib8000_read_word(state, 187) & 0xfffb) | (1 << 3));*/

    dib8000_write_word(189,~seg_mask13 | seg_diff_mask);          /* P_lmod4_seg_inh       */
    dib8000_write_word(192,~seg_mask13 | seg_diff_mask);          /* P_pha3_seg_inh        */
    dib8000_write_word(225,~seg_mask13 | seg_diff_mask);          /* P_tac_seg_inh         */
    if ((!ch->u.isdbt.sb_mode) && (state_dib8000.dib8000_cfg.pll->ifreq == 0))
        dib8000_write_word(266,~seg_mask13 | seg_diff_mask | 0x40);      /* P_equal_noise_seg_inh */
    else
        dib8000_write_word( 266,~seg_mask13 | seg_diff_mask);      /* P_equal_noise_seg_inh */
    dib8000_write_word(287, ~seg_mask13 | 0x1000);      /* P_tmcc_seg_inh        */
    //dib8000_write_word(state, 288, ~seg_mask13 | seg_diff_mask); /* P_tmcc_seg_eq_inh */
    if (!autosearching)
        dib8000_write_word(288, (~seg_mask13 | seg_diff_mask) & 0x1fff); /* P_tmcc_seg_eq_inh */
    else
        dib8000_write_word(288, 0x1fff); //disable equalisation of the tmcc when autosearch to be able to find the DQPSK channels.
    Out(printf("287 = %X (%d)\n", ~seg_mask13 | 0x1000));

    dib8000_write_word(211, seg_mask13 & (~seg_diff_mask) ); /* P_des_seg_enabled     */

    /* offset loop parameters */
    if (ch->u.isdbt.sb_mode == 1)  {
      if (ch->u.isdbt.partial_reception == 0)   // Sound Broadcasting mode 1 seg
        /* P_timf_alpha = (11-P_mode), P_corm_alpha=6, P_corm_thres=0x80 */
        dib8000_write_word(32, ((11-mode) << 12) | (6 << 8) | 0x40);

      else // Sound Broadcasting mode 3 seg
        /* P_timf_alpha = (10-P_mode), P_corm_alpha=6, P_corm_thres=0x80 */
        dib8000_write_word(32, ((10-mode) << 12) | (6 << 8) | 0x60);
    } else
      // TODO in 13 seg, timf_alpha can always be the same or not ?
      /* P_timf_alpha = (9-P_mode, P_corm_alpha=6, P_corm_thres=0x80 */
      dib8000_write_word(32, ((9-mode) << 12) | (6 << 8) | 0x80);


    if (ch->u.isdbt.sb_mode == 1)  {
      if (ch->u.isdbt.partial_reception == 0)   // Sound Broadcasting mode 1 seg
        /* P_ctrl_pha_off_max=3   P_ctrl_sfreq_inh =0  P_ctrl_sfreq_step = (11-P_mode)  */
        dib8000_write_word(37, (3 << 5 ) | ( 0 << 4) | (10-mode) );

      else  // Sound Broadcasting mode 3 seg
        /* P_ctrl_pha_off_max=3   P_ctrl_sfreq_inh =0  P_ctrl_sfreq_step = (10-P_mode)  */
        dib8000_write_word(37, (3 << 5 ) | ( 0 << 4) | (9-mode) );
    } else
      /* P_ctrl_pha_off_max=3   P_ctrl_sfreq_inh =0  P_ctrl_sfreq_step = 9  */
      dib8000_write_word(37, (3 << 5 ) | ( 0 << 4) | (8-mode));


    /* P_dvsy_sync_wait - reuse mode */
    switch (ch->u.isdbt.nfft) {
        case FFT_8K: mode = 256; break;
        case FFT_4K: mode = 128; break;
        default:
        case FFT_2K: mode =  64; break;
    }
    if (state_dib8000.dib8000_cfg.diversity_delay == 0)
        mode = (mode * (1 << (guard)) * 3) / 2 + 48; // add 50% SFN margin + compensate for one DVSY-fifo
    else
        mode = (mode * (1 << (guard)) * 3) / 2 + state_dib8000.dib8000_cfg.diversity_delay; // add 50% SFN margin + compensate for DVSY-fifo
    mode <<= 4;
    dib8000_write_word(273, (dib8000_read_word(273) & 0x000f) | mode);

    /* channel estimation fine configuration */
    switch (max_constellation) {
        case QAM_64QAM:
		    ana_gain = 0x7;// -1 : avoid def_est saturation when ADC target is -16dB
            coeff[0] = 0x0148;       /* P_adp_regul_cnt 0.04 */
            coeff[1] = 0xfff0;       /* P_adp_noise_cnt -0.002 */
            coeff[2] = 0x00a4;       /* P_adp_regul_ext 0.02 */
            coeff[3] = 0xfff8;       /* P_adp_noise_ext -0.001 */
            //if (!state->cfg.hostbus_diversity) //if diversity, we should prehaps use the configuration of the max_constallation -1
            break;
        case QAM_16QAM:
		    ana_gain = 0x7;// -1 : avoid def_est saturation when ADC target is -16dB
            coeff[0] = 0x023d;       /* P_adp_regul_cnt 0.07 */
            coeff[1] = 0xffdf;       /* P_adp_noise_cnt -0.004 */
            coeff[2] = 0x00a4;       /* P_adp_regul_ext 0.02 */
            coeff[3] = 0xfff0;       /* P_adp_noise_ext -0.002 */
            //if (!((state->cfg.hostbus_diversity) && (max_constellation == QAM_16QAM)))
            break;
        default:
		    ana_gain = 0;// 0 : goes along with ADC target at -22dB to keep good mobile performance and lock at sensitivity level
            coeff[0] = 0x099a;       /* P_adp_regul_cnt 0.3 */
            coeff[1] = 0xffae;       /* P_adp_noise_cnt -0.01 */
            coeff[2] = 0x0333;       /* P_adp_regul_ext 0.1 */
            coeff[3] = 0xfff8;       /* P_adp_noise_ext -0.002 */
            break;
    }
    for (mode = 0; mode < 4; mode++)
        dib8000_write_word(215 + mode , coeff[mode]);

	// update ana_gain depending on max constellation
	dib8000_write_word(116 , ana_gain);
	// update ADC target depending on ana_gain
	if(ana_gain) {// set -16dB ADC target for ana_gain=-1
      for (i = 0; i < 10; i++)
		dib8000_write_word(80+i , adc_target_16dB[i]);
	}
	else {// set -22dB ADC target for ana_gain=0
      for (i = 0; i < 10; i++)
		dib8000_write_word(80+i , adc_target_16dB[i]-355);
	}

    // ---- ANA_FE ----
    if (ch->u.isdbt.sb_mode){
        if (ch->u.isdbt.partial_reception == 1) // 3-segments
            ana_fe = ana_fe_coeff_3seg;
        else // 1-segment
            ana_fe = ana_fe_coeff_1seg;
    } else
        ana_fe = ana_fe_coeff_13seg;

    if (ch->u.isdbt.sb_mode==1 || state_dib8000.isdbt_cfg_loaded==0)
      for (mode = 0; mode < 24; mode++)
        dib8000_write_word(117 + mode , ana_fe[mode]);

    // ---- CHAN_BLK ----
    for (i=0;i<13;i++) {
      if ( (((~seg_diff_mask) >> i) & 1) == 1 ) {
        P_cfr_left_edge  += (1<<i) * ((i==0 ) || ((((seg_mask13 & (~seg_diff_mask)) >> (i-1)) & 1) == 0));
        P_cfr_right_edge += (1<<i) * ((i==12) || ((((seg_mask13 & (~seg_diff_mask)) >> (i+1)) & 1) == 0));
      }
    }
    dib8000_write_word(222, P_cfr_left_edge ); // P_cfr_left_edge
    dib8000_write_word(223, P_cfr_right_edge); // P_cfr_right_edge
    // "P_cspu_left_edge"  not used => do not care
    // "P_cspu_right_edge" not used => do not care

    if (ch->u.isdbt.sb_mode == 1) { // ISDB-Tsb
        dib8000_write_word(228, 1);                                      // P_2d_mode_byp=1
        dib8000_write_word(205, dib8000_read_word(205) & 0xfff0); // P_cspu_win_cut = 0
        if (ch->u.isdbt.partial_reception == 0 // 1-segment
                && ch->u.isdbt.nfft == FFT_2K) {
            //dib8000_write_word(state, 219, dib8000_read_word(state, 219) & 0xfffe); // P_adp_pass = 0
            dib8000_write_word(265, 15); // P_equal_noise_sel = 15
        }
    } else if (state_dib8000.isdbt_cfg_loaded == 0) {
        dib8000_write_word(228, 0);  // default value
        dib8000_write_word(265, 31); // default value
        dib8000_write_word(205, 0x200f); // init value
    }

    // ---- TMCC ----
    for (i=0; i<3; i++)
        tmcc_pow += ( ((ch->u.isdbt.layer[i].constellation == QAM_DQPSK) * 4 + 1)*ch->u.isdbt.layer[i].nb_segments ) ;
    // Quantif of "P_tmcc_dec_thres_?k" is (0, 5+mode, 9);
    // Threshold is set at 1/4 of max power.
    tmcc_pow *= (1 << (9-2));


    dib8000_write_word(290, tmcc_pow); // P_tmcc_dec_thres_2k
    dib8000_write_word(291, tmcc_pow); // P_tmcc_dec_thres_4k
    dib8000_write_word(292, tmcc_pow); // P_tmcc_dec_thres_8k
    //dib8000_write_word(state, 287, (1 << 13) | 0x1000 );
    // ---- PHA3 ----

    if (state_dib8000.isdbt_cfg_loaded == 0)
      dib8000_write_word(250, 3285); /*p_2d_hspeed_thr0*/

    if (ch->u.isdbt.sb_mode==1)
      state_dib8000.isdbt_cfg_loaded=0;
    else
      state_dib8000.isdbt_cfg_loaded=1;

}

int dib8000_tune(struct dibChannel *ch)
{
	int ret = 0;
	MS_U16 value, mode = fft_to_mode(ch);

	// we are already tuned - just resuming from suspend
	if (ch == NULL)
		return DIB_RETURN_ERROR;

	dib8000_set_bandwidth(ch->bandwidth_kHz);
	dib8000_set_channel  (ch, 0,0);

 	// restart demod
        ret |= dib8000_write_word(770, 0x4000);
	ret |= dib8000_write_word(770, 0x0000);
	DibMSleep(45);

	/* P_ctrl_inh_cor=0, P_ctrl_alpha_cor=4, P_ctrl_inh_isi=0, P_ctrl_alpha_isi=3 */
    /*	ret |= dib8000_write_word(state, 29, (0 << 9) | (4 << 5) | (0 << 4) | (3 << 0) );  workaround inh_isi stays at 1 */

	// never achieved a lock before - wait for timfreq to update
	if (state_dib8000.timf == 0) {
		if (ch->u.isdbt.sb_mode == 1) {
			if (ch->u.isdbt.partial_reception == 0) // Sound Broadcasting mode 1 seg
				DibMSleep(300);
			else                                    // Sound Broadcasting mode 3 seg
				DibMSleep(500);
		} else                                      // 13 seg
			DibMSleep(200);
	}

	//dump_reg(state);
        if (ch->u.isdbt.sb_mode == 1)  {
          if (ch->u.isdbt.partial_reception == 0) {  // Sound Broadcasting mode 1 seg

            /* P_timf_alpha = (13-P_mode) , P_corm_alpha=6, P_corm_thres=0x40  alpha to check on board */
            dib8000_write_word(32, ((13-mode) << 12) | (6 << 8) | 0x40);
            //  				dib8000_write_word(state, 32, (8 << 12) | (6 << 8) | 0x80);

            /*  P_ctrl_sfreq_step= (12-P_mode)   P_ctrl_sfreq_inh =0     P_ctrl_pha_off_max  */
            value = (0 << 4);
            /* 				switch (ch->u.isdbt.nfft) { */
            /* 					case FFT_2K: value |= (13-mode) | (0x6 << 5) ; break; */
            /* 					case FFT_8K: value |= (13-mode) | (0x8 << 5) ; break; */
            /* 					case FFT_4K: value |= (13-mode) | (0x7 << 5) ; break; */
            /* /\*					case FFT_2K: value |= 0x6 | (0x6 << 5) ; break; */
            /* 					case FFT_8K: value |= 0x8 | (0x8 << 5) ; break; */
            /* 					case FFT_4K: value |= 0x7 | (0x7 << 5) ; break;*\/ */
            /* 				} */
            ret |= dib8000_write_word(37, (12-mode) | ((5+mode) << 5));

          } else {  // Sound Broadcasting mode 3 seg

            /* P_timf_alpha = (12-P_mode) , P_corm_alpha=6, P_corm_thres=0x60  alpha to check on board */
            dib8000_write_word(32, ((12-mode) << 12) | (6 << 8) | 0x60);
            //  				dib8000_write_word(state, 32, (8 << 12) | (6 << 8) | 0x80);

            /*  P_ctrl_sfreq_step =(11-P_mode)   P_ctrl_sfreq_inh =0     P_ctrl_pha_off_max  */
            /* 				value = (0 << 4); */
            /* 				switch (ch->u.isdbt.nfft) { */
            /* 					case FFT_2K: value |= (11-mode) | (0x6 << 5) ; break; */
            /* 					case FFT_8K: value |= (11-mode) | (0x8 << 5) ; break; */
            /* 					case FFT_4K: value |= (11-mode) | (0x7 << 5) ; break; */
            /* /\* 					case FFT_2K: value |= 0x6 | (0x6 << 5) ; break; */
            /*  					case FFT_8K: value |= 0x8 | (0x8 << 5) ; break; */
            /*  					case FFT_4K: value |= 0x7 | (0x7 << 5) ; break;*\/ */
            /* 				} */
            ret |= dib8000_write_word(37, (11-mode) | ((5+mode) << 5));
          }

        } else {  // 13 seg
          /* P_timf_alpha = 8 , P_corm_alpha=6, P_corm_thres=0x80  alpha to check on board */
          dib8000_write_word(32, ((11-mode) << 12) | (6 << 8) | 0x80 );

          /*  P_ctrl_sfreq_step     P_ctrl_sfreq_inh =0     P_ctrl_pha_off_max  */
          /* 			value = (0 << 4); */
          /* 			switch (ch->u.isdbt.nfft) { */
          /* 				case FFT_2K: value |= 0x6 | (0x6 << 5) ; break; */
          /* 				case FFT_8K: value |= 0x8 | (0x8 << 5) ; break; */
          /* 				case FFT_4K: value |= 0x7 | (0x7 << 5) ; break; */
          /* 			} */
          ret |= dib8000_write_word(37,  (10-mode) | ((5+mode) << 5));

        }


	// we achieved a coff_cpil_lock - it's time to update the timf
	if ((dib8000_read_word(568) >> 11)  & 0x1)
    	dib8000_update_timf();

	//now that tune is finished, lock0 should lock on fec_mpeg to output this lock on MP_LOCK. It's changed in autosearch start
	dib8000_write_word(6, 0x200);

    if (state_dib8000.revision == 0x8002) {
        value = dib8000_read_word(903);
        dib8000_write_word(903, value & ~(1 << 3));
        DibMSleep(30);
        dib8000_write_word(903, value | (1 << 3));
    }

    /*** Check Coff Lock and update tune status ***/
    if (dib8000_read_word(568) & (1 << 12))
        fe.status = FE_STATUS_DEMOD_SUCCESS;

	return ret;
}

#endif

#if 1 // dib8090

void dib8090_sip_register(void)
{
    DibZeroMemory(&fe         , sizeof(struct dibFrontend));
    DibZeroMemory(&state_dib0090, sizeof(struct dib0090_state));
    DibZeroMemory(&state_dib8000, sizeof(struct dib8000_state));
    DibZeroMemory(&state_dib8090, sizeof(struct dib8090_state));

    state_dib8090.p_dib8090_cfg = &nim_dtv_lt_8096_config;

    // dib8000_cfg
    state_dib8000.dib8000_cfg.output_mpeg2_in_188_bytes = default_dib8000_config.output_mpeg2_in_188_bytes;
    state_dib8000.dib8000_cfg.hostbus_diversity     = default_dib8000_config.hostbus_diversity;
    state_dib8000.dib8000_cfg.agc_config_count      = default_dib8000_config.agc_config_count;
    state_dib8000.dib8000_cfg.agc                   = default_dib8000_config.agc;
    state_dib8000.dib8000_cfg.pll                   = NULL;
    state_dib8000.dib8000_cfg.gpio_dir              = default_dib8000_config.gpio_dir;
    state_dib8000.dib8000_cfg.gpio_val              = default_dib8000_config.gpio_val;
    state_dib8000.dib8000_cfg.gpio_pwm_pos          = default_dib8000_config.gpio_pwm_pos;
    state_dib8000.dib8000_cfg.pwm_freq_div          = default_dib8000_config.pwm_freq_div;
    //state_dib8000.dib8000_cfg.agc_control           = default_dib8000_config.agc_control;

    // dib0090_cfg
    state_dib8090.dib0090_cfg.io.clock_khz          = default_dib0090_config.io.clock_khz;
    state_dib8090.dib0090_cfg.io.pll_bypass         = default_dib0090_config.io.pll_bypass;
    state_dib8090.dib0090_cfg.io.pll_range          = default_dib0090_config.io.pll_range;
    state_dib8090.dib0090_cfg.io.pll_prediv         = default_dib0090_config.io.pll_prediv;
    state_dib8090.dib0090_cfg.io.pll_loopdiv        = default_dib0090_config.io.pll_loopdiv;
    state_dib8090.dib0090_cfg.io.adc_clock_ratio    = default_dib0090_config.io.adc_clock_ratio;
    state_dib8090.dib0090_cfg.io.pll_int_loop_filt  = default_dib0090_config.io.pll_int_loop_filt;
    state_dib8090.dib0090_cfg.freq_offset_khz_uhf   = default_dib0090_config.freq_offset_khz_uhf;
    state_dib8090.dib0090_cfg.freq_offset_khz_vhf   = default_dib0090_config.freq_offset_khz_vhf;
    state_dib8090.dib0090_cfg.clkouttobamse         = default_dib0090_config.clkouttobamse;
    state_dib8090.dib0090_cfg.analog_output         = default_dib0090_config.analog_output;
    state_dib8090.dib0090_cfg.i2c_address           = default_dib0090_config.i2c_address;
    state_dib8090.dib0090_cfg.wbd_vhf_offset        = default_dib0090_config.wbd_vhf_offset;
    state_dib8090.dib0090_cfg.wbd_cband_offset      = default_dib0090_config.wbd_cband_offset;
    state_dib8090.dib0090_cfg.use_pwm_agc           = default_dib0090_config.use_pwm_agc;
    state_dib8090.dib0090_cfg.clkoutdrive           = default_dib0090_config.clkoutdrive;
    state_dib8090.dib0090_cfg.fref_clock_ratio      = 0;
    state_dib8090.dib0090_cfg.cap_value             = 0;

    if( 12000 == nim_dtv_lt_8096_config.clock_khz)
    {
        state_dib8000.dib8000_cfg.pll = &dib8090_pll_config_12mhz;

        state_dib8090.dib0090_cfg.io.clock_khz       = dibx09x_io_12mhz_120.clock_khz;
        state_dib8090.dib0090_cfg.io.pll_bypass      = dibx09x_io_12mhz_120.pll_bypass;
        state_dib8090.dib0090_cfg.io.pll_range       = dibx09x_io_12mhz_120.pll_range;
        state_dib8090.dib0090_cfg.io.pll_prediv      = dibx09x_io_12mhz_120.pll_prediv;
        state_dib8090.dib0090_cfg.io.pll_loopdiv     = dibx09x_io_12mhz_120.pll_loopdiv;
        state_dib8090.dib0090_cfg.io.adc_clock_ratio  = dibx09x_io_12mhz_120.adc_clock_ratio;
        state_dib8090.dib0090_cfg.io.pll_int_loop_filt  = dibx09x_io_12mhz_120.pll_int_loop_filt;
    }

    state_dib8000.dib8000_cfg.output_mpeg2_in_188_bytes  = nim_dtv_lt_8096_config.output_mpeg2_in_188_bytes;
    state_dib8000.dib8000_cfg.hostbus_diversity         = 1;
    state_dib8000.dib8000_cfg.gpio_dir                  = nim_dtv_lt_8096_config.gpio_dir;
    state_dib8000.dib8000_cfg.gpio_val                  = nim_dtv_lt_8096_config.gpio_val;
    state_dib8000.dib8000_cfg.gpio_pwm_pos              = nim_dtv_lt_8096_config.gpio_pwm_pos;
    state_dib8000.dib8000_cfg.drives                    = nim_dtv_lt_8096_config.dib8k_drives;
    state_dib8000.dib8000_cfg.diversity_delay           = nim_dtv_lt_8096_config.diversity_delay;
    state_dib8000.dib8000_cfg.div_cfg                   = nim_dtv_lt_8096_config.div_cfg;
    state_dib8000.dib8000_cfg.refclksel                 = nim_dtv_lt_8096_config.refclksel;

    state_dib8090.dib0090_cfg.io.pll_bypass         = 1;
    state_dib8090.dib0090_cfg.freq_offset_khz_uhf   = nim_dtv_lt_8096_config.dib0090_freq_offset_khz_uhf;
    state_dib8090.dib0090_cfg.freq_offset_khz_vhf   = nim_dtv_lt_8096_config.dib0090_freq_offset_khz_vhf;
    state_dib8090.dib0090_cfg.clkouttobamse         = nim_dtv_lt_8096_config.clkouttobamse;
    state_dib8090.dib0090_cfg.analog_output         = 1;
    state_dib8090.dib0090_cfg.clkoutdrive           = nim_dtv_lt_8096_config.clkoutdrive;
    state_dib8090.dib0090_cfg.fref_clock_ratio      = nim_dtv_lt_8096_config.fref_clock_ratio;
    state_dib8090.dib0090_cfg.cap_value             = nim_dtv_lt_8096_config.cap_value;

    state_dib8090.wbd_target_vhf                   = 330; // 500
    state_dib8090.wbd_target_uhf                   = 500;
    state_dib8090.p_wbd_table                      = dib8090_wbd_target_default;

    // dib8000_register
    fe.release_compat_fe           &= ~COMPONENT_LAYER_RELEASE;
    state_dib8000.timf_default      = state_dib8000.dib8000_cfg.pll->timf;
    state_dib8000.diversity_onoff   = 0;

    // dib0090_register
    state_dib0090.tuner_info.caps       = TUNER_CAN_VHF | TUNER_CAN_UHF | TUNER_CAN_LBAND | TUNER_CAN_FAST_TUNE;
    fe.components |= FE_COMPONENT_TUNER;
    state_dib0090.p_dib0090_config      = &state_dib8090.dib0090_cfg;
    state_dib0090.wbd_uhf_offset        = state_dib8090.dib0090_cfg.wbd_vhf_offset;
    state_dib0090.wbd_vhf_offset        = state_dib8090.dib0090_cfg.wbd_cband_offset;
}

void dib8090_set_wbd_table_target(void)
{
    state_dib8090.p_wbd_table = nim_dtv_lt_8096_wbd_target;
}

int dib8090_tuner_sleep(int onoff)
{
    Out(printf("sleep dib0090: %d\n", onoff));

    return demod_set_gpio(0, 0, (MS_U8)onoff);
}

int dib8090_agc_control(MS_U8 restart)
{
    Out(printf("AGC control callback: %d\n", restart));
    dib0090_dcc_freq(restart);
    return DIB_RETURN_SUCCESS;
}

int dib8090_agc_startup(struct dibChannel *ch)
{
    MS_U8 band = (MS_U8) channel_frequency_band(ch->RF_kHz);
    int ret;
    MS_U16 offset;

    if (fe.tune_state == CT_AGC_START) {
          struct dib8090_wbd_target *wbd_table = state_dib8090.p_wbd_table;

        while (ch->RF_kHz > (MS_U32)1000*wbd_table->max_freq)
            wbd_table++;
        offset  = wbd_table->wbd;
        Out(printf("the WBD target is %i\n", wbd_table->wbd));
        if (band == BAND_CBAND)
        {
            dib0090_set_wbd_target(250, state_dib8090.dib0090_cfg.wbd_vhf_offset, offset);
        }
        else
        {
            offset += (dib0090_get_wbd_offset() * 8 * 18 / 33 + 1) / 2;
            demod_set_wbd_ref(offset);
        }
    }

#ifdef CONFIG_DIB0090_USE_PWM_AGC
    if (band == BAND_CBAND) {
        if (fe.tune_state == CT_AGC_START) {
            Out(printf("tuning in CBAND - soft-AGC startup\n"));
            /* TODO specific wbd target for dib0090 - needed for startup ? */
        }

        if (fe.tune_state < CT_AGC_STOP)
#endif
            ret = dib0090_gain_control(ch);
#ifdef CONFIG_DIB0090_USE_PWM_AGC
        else
            ret = FE_CALLBACK_TIME_NEVER;

        if (fe.tune_state == CT_AGC_STOP) {
            Out(printf("switching to PWM AGC\n"));
            dib0090_pwm_gain_reset(ch);
            dib8000_pwm_agc_reset(ch);
        }
    } else { /* for everything else than CBAND we are using standard AGC */
        if (fe.tune_state == CT_AGC_START)
        {
            dib0090_pwm_gain_reset(ch);
        }
        ret = dib8000_agc_startup(ch);
    }
#endif
    return ret;
}

int dib8090_get_adc_power(void)
{
    return dib8000_get_adc_power(1);
}

int dib8090_tuner_reset(int onoff)
{
    Out(printf("reset dib0090: %d\n", onoff));
    demod_set_gpio(5, 0, !onoff);
    return DIB_RETURN_SUCCESS;
}


#endif

#if 1   // demod

void demod_agc_restart(void)
{
    fe.tune_state = CT_AGC_START;
}

int demod_agc_startup_ex(struct dibChannel *ch)
{
    int ret=-1;
    if (fe.tune_state < CT_AGC_STOP || fe.tune_state >= CT_DONE)
        ret = nim_dtv_lt_8096_agc_tune(ch);
    if (fe.tune_state >= CT_AGC_STOP)
        ret=-1;
    return ret;
}

int demod_autosearch_irq(void)
{
    if (frontend_capabilities() & FE_CAN_FAST_TUNE) {
#ifndef CONFIG_NO_TUNE_ASYNC
        if (component_ready(state_dib8000.demod_info.callback_time, fe.tune_state, CT_DEMOD_START)) {
            state_dib8000.demod_info.callback_time = systime() +
#endif
            dib8000_tune(fe.autosearch_channel_compat);
            if (fe.status <= FE_STATUS_DEMOD_SUCCESS)
                return 2;
            else if (fe.status == FE_STATUS_TUNE_PENDING)
                return 0;
            else
                return 1;
#ifndef CONFIG_NO_TUNE_ASYNC
        }
#endif
    } else

    return dib8000_autosearch_is_irq();
}

int demod_autosearch_start_ex(struct dibChannel *ch)
{
    if (frontend_capabilities() & FE_CAN_FAST_TUNE) {
        fe.status = FE_STATUS_TUNE_PENDING;
        fe.tune_state = CT_DEMOD_START;
        fe.autosearch_channel_compat = ch;
        return DIB_RETURN_SUCCESS;
    } else {
        fe.current_bw = (MS_U16)ch->bandwidth_kHz;
            return dib8000_autosearch_start(ch);
    }
    return DIB_RETURN_ERROR;
}

int demod_get_channel(struct dibChannel *ch)
{
    return dib8000_get_channel(ch);
}

void demod_monitor_post_process(struct dibDemodMonitor *m)
{
	MS_U8 k;
	double tmp0;
	//MS_S32 Lu;

	tmp0 = (double) m->adc_power_i;
	if (tmp0 == 0.0)
		tmp0 = 1.0;
	tmp0 /= (double) (1 << 20);
	m->I_adc_power = 10 * log10(tmp0);

	tmp0 = (double) m->adc_power_q;
	if (tmp0 == 0.0)
		tmp0 = 1.0;
	tmp0 /= (double) (1 << 20);
	m->Q_adc_power = 10 * log10(tmp0);

// make it relative values in per cent
	if (m->adc_monitor_only) {
//		m->adc_lnpower = (10 * m->adc_lnpower_raw / 256.0 / log(10));
		m->adc_lnpower = 10.0 * m->adc_lnpower_raw / 256;
	} else {
		tmp0 = m->adc_power_i + m->adc_power_q;
		if (m->tuner_is_baseband)
			tmp0 /= 2.0;
		if (tmp0 == 0.0)
			tmp0 = 1.0;
		tmp0 /= (1 << 20);
		tmp0 = 10 * log10(tmp0);

		tmp0 = tmp0 - (-14.0);
		m->rf_power_dbm = (-m->tun_max_gain * (double) m->rf_power / (double) (1 << 16) + (double) m->tun_ref_level) + tmp0 +
				(m->lna_gain_step * 0); // 0 = current_lna_status - we don't have it in most cases
		m->agc_db = (double) m->tun_max_gain * (double) m->rf_power / (double) (1 << 16);

		m->agc1 = (m->agc1_raw / 65536.0);
		m->agc2 = (m->agc2_raw / 65536.0);

		m->agc_wbd = m->agc_wbd_raw / 4096.0;
	}
	m->iq_gain_mismatch  = 10 * log10((double) m->iq_misgain / 1024.0);
	m->iq_phase_mismatch = 180 * (m->iq_misphi / 2048.0);

    m->equal_signal_dB = 10 * log10((m->equal_signal_mant+1e-10) * pow(2.0,m->equal_signal_exp));
    m->equal_noise_dB = 10 * log10((m->equal_noise_mant+1e-10) * pow(2.0,m->equal_noise_exp));

    m->CoN = m->equal_signal_dB - m->equal_noise_dB;

	m->mer = -10.0 * log10( ((double) m->mer_mant) * pow(2.0, ((double) m->mer_exp) - 8.0) );

	/*
	MS_S16 ps = log_10_100[m->equal_signal_mant] + m->equal_signal_exp * 301,
		  pn = log_10_100[m->equal_noise_mant] + m->equal_noise_exp * 301;
	m->mer = (ps - pn + 5) / 10;
	*/

	m->viewable_dds_freq = (double) (m->p_dds_freq / (1 << 26) * 60);
	m->freq_offset = m->dds_freq - m->p_dds_freq;

	if (m->cur_digital_channel.type == STANDARD_DVBT)
		m->carrier_offset = (m->tuner_is_baseband ? 0 : 60000) - ((double) m->freq_offset / (double) (1 << 26) * (double) m->internal_clk);
	else if (m->cur_digital_channel.type == STANDARD_ISDBT)
		m->carrier_offset = ((double) m->freq_offset / (double) (1 << 26) * (double) m->internal_clk);

	//LB did not put back this modification because on TFE8076MD4, monitored timf was false after
	/*if (m->current_bandwidth !=0)
          m->timf_current = m->timf_current * 80 / (m->current_bandwidth / 100);*/
	m->timing_offset = (int) m->timf_current - (int) m->timf_default;
	m->timing_offset_ppm = 1e6 * (((double) m->timf_current / (double) m->timf_default) - 1);

	m->ber = ((double) m->ber_raw) / 1e8;
    if (m->cur_digital_channel.type == STANDARD_ISDBT) {
        m->berA = ((double) m->ber_raw_A) / 1e8;
        m->berB = ((double) m->ber_raw_B) / 1e8;
        m->berC = ((double) m->ber_raw_C) / 1e8;
    }

	if (m->n_2dpa_monitoring)
		for (k = 0; k < 4; k++)
			m->cti_def[k] = 10 * log10(fabs(m->cti_def_raw[k] / (double)(1 << 18)));
}

void demod_set_ber_rs_len(MS_U8 ber_rs_len)
{
	if (frontend_generic_monitoring(GENERIC_MONIT_SET_BER_RS_LEN, NULL, 0, ber_rs_len) != DIB_RETURN_NOT_SUPPORTED)
		state_dib8000.demod_info.ber_rs_len = ber_rs_len;
}

int demod_set_diversity_in(int onoff)
{
    return dib8000_set_diversity_in(onoff);
}

int demod_set_gpio(MS_U8 num, MS_U8 dir, MS_U8 val)
{
      return dib8000_set_gpio(num, dir, val);
}

int demod_set_output_mode(int mode)
{
    return dib8000_demod_set_output_mode(mode);
}

int demod_set_wbd_ref(MS_U16 value)
{
    return dib8000_set_wbd_ref(value);
}

int demod_tune_ex(struct dibChannel *ch)
{
    int ret;
    if (frontend_capabilities() & FE_CAN_FAST_TUNE) {
        do {
            ret = dib8000_tune(ch);

            if (fe.status == FE_STATUS_LOCKED ||
                fe.status == FE_STATUS_TUNE_FAILED)
                break;
            DibMSleep(ret/10);
        } while (1);
        ret = DIB_RETURN_SUCCESS;
    }
    else {
        frontend_tune_restart(FE_RESTART_TUNE_PROCESS_FROM_DEMOD, ch);
        fe.current_bw = ch->bandwidth_kHz;
        ret = dib8000_tune(ch);
    }
    return ret;
}

int demod_wakeup(void)
{
    return dib8000_wakeup();
}

#endif

#ifdef DIBCOM_EXTENDED_MONITORING

#ifdef CONFIG_STANDARD_ISDBT
void dump_isdbt_channel(struct dibChannel *cd)
{
    MS_U8 i;

    switch (cd->u.isdbt.nfft) {
		case 1:  Out(printf("8K     ")); break;
		case 2:  Out(printf("4K     ")); break;
		case 0:  Out(printf("2K     ")); break;
		default: Out(printf("UNK    ")); break;
	}

	Out(printf("1/%-2i      ", 32 / (1 << cd->u.isdbt.guard)));
    Out(printf("%i       ", cd->u.isdbt.sb_mode));
    Out(printf("%i       ", cd->u.isdbt.partial_reception));
	for (i = 0; i<3; i++)
    {
        Out(printf("%.2i    ", cd->u.isdbt.layer[i].nb_segments));

        switch (cd->u.isdbt.layer[i].constellation) {
		    case 0:  Out(printf("QPSK   ")); break;
		    case 1:  Out(printf("16QAM  ")); break;
		    case 2:  Out(printf("64QAM  ")); break;
            case 3 : Out(printf("DQPSK  ")); break;
		    default: Out(printf("UNK    ")); break;
        }
        Out(printf("%i/%i    ", cd->u.isdbt.layer[i].code_rate, cd->u.isdbt.layer[i].code_rate + 1));
        Out(printf("%i         ", cd->u.isdbt.layer[i].time_intlv));
	}
}
#endif

int demod_get_monitoring(struct dibDemodMonitor *m)
{
    m->current_bandwidth = fe.current_bw;
	frontend_generic_monitoring(GENERIC_MONIT_COMPAT_EXTENDED, (MS_U8 *) m, sizeof(*m), 0);

	return DIB_RETURN_SUCCESS;
}

int demods_have_standard(struct dibDemodMonitor m[], MS_U8 st)
{
    if (m[0].cur_digital_channel.type == st)
        return 1;
    return 0;
}

void dib7000_print_monitor(struct dibDemodMonitor m[], int stat_count)
{
    int k=0,i;

    if(!demods_have_standard(m, STANDARD_DAB)) {
        Out(printf("I adc power dBVrms"));
            Out(printf("%-15.2f", m[k].I_adc_power));
        Out(printf("\n"));

        Out(printf("Q adc power dBVrms"));
            Out(printf("%-15.2f", m[k].Q_adc_power));
        Out(printf("\n"));
    }

    Out(printf("DC offset on I"));
        Out(printf("%-15.2f", m[k].iqc_dcoff_i));
    Out(printf("\n"));

    Out(printf("DC offset on Q"));
    Out(printf("%-15.2f", m[k].iqc_dcoff_q));
    Out(printf("\n"));

    Out(printf("IQ gain mismatch (dB)"));
        Out(printf("%-15.2g", m[k].iq_gain_mismatch));
    Out(printf("\n"));

    Out(printf("IQ phase mismatch (degree)"));
        Out(printf("%-15.2g", m[k].iq_phase_mismatch));
    Out(printf("\n"));

    if (m[0].n_adc_mode <= 1 && !m[0].adc_monitor_only) {
        Out(printf("rf power dBm"));
            Out(printf("%-15.2g", m[k].rf_power_dbm));
        Out(printf(printf("\n")));

        Out(printf("WBD:"));
            Out(printf("%-15.8g", m[k].agc_wbd));
        Out(printf("\n"));

        Out(printf("AGCglobal (norm)"));
            Out(printf("%-15.2g", m[k].rf_power/65536.0));
        Out(printf("\n"));

        Out(printf("AGC1 (norm)"));
            Out(printf("%-15.8g", m[k].agc1));
        Out(printf("\n"));

        Out(printf("AGC2 (norm)"));
            Out(printf("%-15.8g", m[k].agc2));
        Out(printf("\n"));
        Out(printf("AGC Split"));
            Out(printf("%-15d", m[k].agc_split));
        Out(printf("\n"));

        Out(printf("\n"));
    }
        Out(printf("Carrier offset in Khz"));
            Out(printf("%-15.3g", m[k].carrier_offset));
        Out(printf("\n"));

    if (m[0].adc_monitor_only) {
        Out(printf("ADC LNPower"));
            Out(printf("%-15.3g", m[k].adc_lnpower));
        Out(printf("\n"));
    }

    if (m[0].n_adc_mode == 0) {
        Out(printf("Timing offset in ppm"));
            Out(printf("%-15.4g", m[k].timing_offset_ppm));
        Out(printf("\n"));

#ifdef CONFIG_STANDARD_ISDBT
        if (demods_have_standard(m, STANDARD_ISDBT)) {
            Out(printf("Viterbi syndrome layer a"));
                Out(printf("%-15i", m[k].viterbi_syndrome));
            Out(printf("\n"));
            Out(printf("Viterbi syndrome layer b"));
                Out(printf("%-15i", m[k].viterbi_syndrome_b));
            Out(printf("\n"));
            Out(printf("Viterbi syndrome layer c"));
                Out(printf("%-15i", m[k].viterbi_syndrome_c));
            Out(printf("\n"));

        }
#endif
        Out(printf("C/N (dB)"));
        Out(printf("%-15.4g", m[k].CoN));
        Out(printf("\n"));

        if ( demods_have_standard(m, STANDARD_DVBT) || demods_have_standard(m, STANDARD_ISDBT) || demods_have_standard(m, STANDARD_DAB) ) {

#ifdef CONFIG_STANDARD_ISDBT
            if ((demods_have_standard(m, STANDARD_ISDBT)) && (m[0].can_display_ber_several_layers == 1))
            {
                Out(printf("packet error count A"));
                    Out(printf("%-15i", m[k].PacketErrors_A));
                Out(printf("\n"));
                Out(printf("packet error count B"));
                    Out(printf("%-15i", m[k].PacketErrors_B));
                Out(printf("\n"));
                Out(printf("packet error count C"));
                    Out(printf("%-15i", m[k].PacketErrors_C));
                Out(printf("\n"));
            }
            else {
#else
            {
#endif
            Out(printf("packet error count"));
                Out(printf("%-15i", m[k].PacketErrors));
            Out(printf("\n"));
            }

            if (!(demods_have_standard(m, STANDARD_ISDBT)) || (m[0].can_display_ber_several_layers != 1))
            {
        Out(printf("packet error cumul"));
        Out(printf("%-15i", m[k].PacketErrorCount));
        Out(printf("\n"));
            }

#ifdef CONFIG_STANDARD_ISDBT
            if ((demods_have_standard(m, STANDARD_ISDBT)) && (m[0].can_display_ber_several_layers == 1))
            {
                Out(printf("Bit Error Rate A"));
                    Out(printf("%-15g", m[k].berA));
                Out(printf("\n"));
                Out(printf("Bit Error Rate B"));
                    Out(printf("%-15g", m[k].berB));
                Out(printf("\n"));
                Out(printf("Bit Error Rate C"));
                    Out(printf("%-15g", m[k].berC));
                Out(printf("\n"));
            }
            else {
#else
            {
#endif
            Out(printf("Bit Error Rate"));
                Out(printf("%-15g", m[k].ber));
            Out(printf("\n"));
        }
        }

        if ( (demods_have_standard(m, STANDARD_DVBT)) || (demods_have_standard(m, STANDARD_ISDBT))) {

            Out(printf("MER"));
                Out(printf("%-15g", m[k].mer));
            Out(printf("\n"));

            Out(printf("Quasi Error Free"));
                Out(printf("%-15s", m[k].ber < 2e-4 && m[k].PacketErrors == 0 ? "OK" : "*** NOT OK ***"));
            Out(printf("\n"));
        } else if (demods_have_standard(m, STANDARD_DAB)) {
            Out(printf("Quasi Error Free : ber<1e-4"));
            Out(printf("%-15s", m[0].ber < 1e-4 && m[0].PacketErrors == 0 ? "OK" : "*** NOT OK ***"));
            Out(printf("\n"));
        }
#ifdef CONFIG_STANDARD_ISDBT
            if (demods_have_standard(m, STANDARD_ISDBT)) {
                Out(printf("                                            |            layer a          |           layer b           |          layer c            | \n"));
                Out(printf("demod channel: nfft  guard  sb_mode  p_rcpt   nb_seg  modu  crate t_intlv   nb_seg  modu  crate t_intlv   nb_seg  modu  crate t_intlv\n"));
                    Out(printf("demod %2d     : ",k));
                    dump_isdbt_channel(&m[k].cur_digital_channel);
                    Out(printf("\n"));
            }
#endif
        Out(printf("\n"));
#ifdef CONFIG_STANDARD_ISDBT
        if (demods_have_standard(m, STANDARD_ISDBT)) {
            Out(printf("demod locks:  agc    coff        lmod4  equal      tmcc_sync   dvsy\n"));
            Out(printf("              | corm | coff_cpil | pha3 | tmcc_dec | tmcc_data |  vit(a,b,c)  fec_mpeg(a,b,c) \n"));
                Out(printf("demod %2d:     %d %d    %d %d         %d %d    %d %d        %d %d         %d  %d%d%d         %d%d%d\n",k,
                        m[k].locks.agc, m[k].locks.corm, m[k].locks.coff, m[k].locks.coff_cpil, m[k].locks.lmod4, m[k].locks.pha3, m[k].locks.equal,m[k].locks.tmcc_dec, m[k].locks.tmcc_sync, m[k].locks.tmcc_data, m[k].locks.dvsy, m[k].locks.vit, m[k].locks.vit_b,m[k].locks.vit_c, m[k].locks.fec_mpeg,m[k].locks.fec_mpeg_b,m[k].locks.fec_mpeg_c));
            Out(printf("\n"));
        }
#endif
        if ((!demods_have_standard(m, STANDARD_DAB))) {
            if (m[0].n_2dpa_monitoring) {
                Out(printf("mu_int/num_fir"));
                    Out(printf("%d/%d%15s", m[k].n_2dpa_mu_int, m[k].n_2dpa_num_fir, " "));
                Out(printf("\n"));

                for (i = 0; i < 4; i++) {
                    Out(printf("cti_def%d%20s",i," "));
                        Out(printf("%.2f (%5d)   ", m[k].cti_def[i], m[k].cti_def_raw[i]));
                    Out(printf("\n"));
                }
                Out(printf("\n"));
            }

            Out(printf("Divin FIFO overflow"));
                Out(printf("%-15d",m[k].divin_fifo_overflow));
            Out(printf("\n"));

            Out(printf("Divin error symbols"));
                Out(printf("%-15d",m[k].divin_nberr_sym));
            Out(printf("\n"));

            Out(printf("Dvsy first arrived"));
                Out(printf("%-15d",m[k].dvsy_first_arrived));
            Out(printf("\n"));

            Out(printf("Dvsy delay"));
                Out(printf("%-15d",m[k].dvsy_delay));
            Out(printf("\n"));

            Out(printf("Divin OK (Way 0, 1)"));
                Out(printf("%d,%d%12s",m[k].way0_ok,m[k].way1_ok,""));
            Out(printf("\n"));
        }

        Out(printf("Inversion")); //, DDS, (msb,lsb)");
            Out(printf("%-15d",m[k].invspec));
        Out(printf("\n"));

        Out(printf("p DDS Freq"));
            Out(printf("%-15g",m[k].viewable_dds_freq));
        Out(printf("\n"));
    }
}
#endif

