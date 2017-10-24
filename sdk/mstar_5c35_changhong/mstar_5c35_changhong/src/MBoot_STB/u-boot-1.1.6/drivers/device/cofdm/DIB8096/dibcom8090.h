#ifndef __DIBCOM_809X__
#define __DIBCOM_809X__

#define DIBCOM_I2C_MASTER_BASE_REGISTER     1024

#define EFUSE

#define DIB_RETURN_SUCCESS                   0
#define DIB_RETURN_ERROR                    -1
#define DIB_RETURN_NOT_SUPPORTED            -2

#define MAX_NB_OF_FRONTENDS                 8

#define COMPONENT_LAYER_RELEASE             0x1
#define SIP_LAYER_RELEASE                   0x2

/* used in frontend->component and channel_frontend-monitoring */
#define FE_COMPONENT_TUNER                  0x01
#define FE_COMPONENT_DEMOD                  0x02
#define FE_COMPONENT_CHANDEC                0x04
#define FE_COMPONENT_ANTENNA                0x08

#define DIB8090_GPIO_DEFAULT_DIRECTIONS     0xffff
#define DIB8090_GPIO_DEFAULT_VALUES         0x0000
#define DIB8090_GPIO_DEFAULT_PWM_POS        0xffff

#define BAND_LBAND                          0x01
#define BAND_UHF                            0x02
#define BAND_VHF                            0x04
#define BAND_SBAND                          0x08
#define BAND_FM	                            0x10
#define BAND_CBAND                          0x20


struct dib8090_config {
    MS_U8 output_mpeg2_in_188_bytes;
    MS_U16 gpio_dir;
    MS_U16 gpio_val;
    MS_U16 gpio_pwm_pos;
    int dib0090_freq_offset_khz_uhf;
    int dib0090_freq_offset_khz_vhf;
    MS_U8 osc_buffer_state; /* 0= normal, 1= tri-state */
    MS_U32 clock_khz;
    MS_U8 clock_pad_drive;
    MS_U8 tuner_gated_i2c;
    MS_U8 invert_iq;
    MS_U8 use_high_level_adjustment;
    MS_U8 force_crystal_mode; /* if == 0 -> decision is made in the driver */
    MS_U16 dib8k_drives;
    MS_U16 diversity_delay;
    MS_U16 div_cfg;
	MS_U8 clkouttobamse : 1;
	MS_U8 clkoutdrive;
    MS_U8 refclksel;
    MS_U8 fref_clock_ratio;
    MS_U8 cap_value;
};

struct dib0090_io_config {
    MS_U32 clock_khz;
    MS_U8 pll_bypass  : 1;
    MS_U8 pll_range   : 1;
    MS_U8 pll_prediv  : 6;
    MS_U8 pll_loopdiv : 6;
    MS_U8 adc_clock_ratio; /* valid is 8, 7 ,6 */
    MS_U16 pll_int_loop_filt; // internal loop filt value. If not fill in , default is 8165
};

//! Configuration of one demodulator
/**
 * This struct is used to configure the DiB8000-demod-driver to make it handle an IC on a specific board correctly.
 */
#define DIB8000_GPIO_DEFAULT_DIRECTIONS         0xffff
#define DIB8000_GPIO_DEFAULT_VALUES             0x0000
#define DIB8000_GPIO_PWM_POS0(v)                ((v & 0xf) << 12)
#define DIB8000_GPIO_PWM_POS1(v)                ((v & 0xf) << 8 )
#define DIB8000_GPIO_PWM_POS2(v)                ((v & 0xf) << 4 )
#define DIB8000_GPIO_PWM_POS3(v)                (v & 0xf)
#define DIB8000_GPIO_DEFAULT_PWM_POS            0xffff

struct dib8000_config {
    MS_U8     output_mpeg2_in_188_bytes;
    MS_U8     hostbus_diversity;
    //int     (*update_lna) (struct dibFrontend *demod, MS_U16 agc_global);
    MS_U8     agc_config_count;
    struct dibx000_agc_config *agc;
    struct dibx000_bandwidth_config *pll;
    MS_U16    gpio_dir;
    MS_U16    gpio_val;
    MS_U16    gpio_pwm_pos;
    MS_U16    pwm_freq_div;
    //int     (*agc_control) (struct dibFrontend *, MS_U8 before);
    MS_U16    drives;
    MS_U16    diversity_delay;
    MS_U16    div_cfg;
    MS_U8     refclksel;
};

struct dibx000_agc_config {
	/* defines the capabilities of this AGC-setting - using the BAND_-defines*/
	MS_U8  band_caps;

	/* 7000PC/MC: P_agc_use_sd_mod1, P_agc_use_sd_mod2 +
	 * 7000x:     P_agc_freq_pwm_div, P_agc_inv_pwm1, P_agc_inv_pwm2,
	 *            P_agc_inh_dc_rv_est, P_agc_time_est, P_agc_freeze, P_agc_nb_est, P_agc_write */
	/* 3000MC/P:  P_agc_rst_accu_conv, P_agc_freq_pwm_div, P_agc_inv_pwm_hf,
	 *            P_agc_inv_pwm_if, P_agc_inv_pwm_ext, P_agc_win_length */
	MS_U16 setup;

	/* 7000x: P_agc_inv_gain */
	MS_U16 inv_gain;
	/* 7000x: P_agc_time_stabiliz */
	MS_U16 time_stabiliz;

	/* 7000x: P_agc_alpha_level, P_agc_thlock */
	MS_U8  alpha_level;
	MS_U16 thlock;

	/* 7000MC/PC: P_agc_wbd_inv, P_sad_wbd_opp */
	MS_U8  wbd_inv;
	/* 7000x:  P_agc_wbd_ref */
	MS_U16 wbd_ref;
	/* 7000MC/PC: P_agc_wbd_sel, P_sad_wbd_sel */
	MS_U8 wbd_sel;
	/* 7000x:  P_agc_wbd_alpha */
	MS_U8 wbd_alpha;

	MS_U16 agc1_max;
	MS_U16 agc1_min;
	MS_U16 agc2_max;
	MS_U16 agc2_min;

	MS_U8 agc1_pt1;
	MS_U8 agc1_pt2;
	/* 7000PC/MC: pt3 */
	MS_U8 agc1_pt3;

	MS_U8 agc1_slope1;
	MS_U8 agc1_slope2;

	MS_U8 agc2_pt1;
	MS_U8 agc2_pt2;

	MS_U8 agc2_slope1;
	MS_U8 agc2_slope2;

	MS_U8 alpha_mant;
	MS_U8 alpha_exp;

	MS_U8 beta_mant;
	MS_U8 beta_exp;

/**
 * tells the demod to use AGC softsplit and not the wideband-detector, the caller has to fill @see agc_split with useful values then.
 */
	MS_U8 perform_agc_softsplit;

//! AGC Split thresholds
/**
 * Defines agc-split thresholds and limits of a board when AGC-Split is used.
 */
	struct {
		MS_U16 min;       // AGC_SPLIT_MIN
		MS_U16 max;       // AGC_SPLIT_MAX
		MS_U16 min_thres; // AGC_GLOBAL_SPLIT_MIN
		MS_U16 max_thres; // AGC_GLOBAL_SPLIT_MAX
	} split;
};

struct dibx000_bandwidth_config {
	MS_U32   internal;  /* internal clock in kHz */
	MS_U32   sampling;  /* sampling frequency in kHz */

	/* pll_cfg */
	MS_U8 pll_prediv;
	MS_U8 pll_ratio;
	MS_U8 pll_range;
	MS_U8 pll_reset;
	MS_U8 pll_bypass;

/* TODO think about adding :<bitsize> */
	MS_U8 enable_refdiv;
	MS_U8 bypclk_div;
	MS_U8 IO_CLK_en_core;
	MS_U8 ADClkSrc;
	MS_U8 modulo;

	MS_U16 sad_cfg;

	MS_U32 ifreq;
	MS_U32 timf;
	MS_U32 xtal_hz;
    MS_U8 refclk;
    MS_U8 divclk;
};

struct dib8090_wbd_target {
    MS_U16 max_freq;  // frequency in kHz
    MS_U16 wbd;       // wbd value
};

enum frontend_tune_state {
    CT_STANDBY = 0,

    CT_START = 1,

    CT_TUNER_START = 10,
    CT_TUNER_STEP_0,
    CT_TUNER_STEP_1,
    CT_TUNER_STEP_2,
    CT_TUNER_STEP_3,
    CT_TUNER_STEP_4,
    CT_TUNER_STEP_5,
    CT_TUNER_STEP_6,
    CT_TUNER_STEP_7,
    CT_TUNER_STOP,

    CT_AGC_START = 20,
    CT_AGC_STEP_0,
    CT_AGC_STEP_1,
    CT_AGC_STEP_2,
    CT_AGC_STEP_3,
    CT_AGC_STEP_4,
    CT_AGC_STOP,

    CT_DEMOD_START = 30,
    CT_DEMOD_STEP_1,
    CT_DEMOD_STEP_2,
    CT_DEMOD_STEP_3,
    CT_DEMOD_STEP_4,
    CT_DEMOD_STEP_5,
    CT_DEMOD_STEP_6,
    CT_DEMOD_STEP_7,
    CT_DEMOD_STEP_8,
    CT_DEMOD_STEP_9,
    CT_DEMOD_STEP_10,
    CT_DEMOD_SEARCH_NEXT = 41,
    CT_DEMOD_STEP_LOCKED,
    CT_DEMOD_STOP,

    CT_CHAN_DEC_START = 50,
    CT_CHAN_DEC_STEP_0,
    CT_CHAN_DEC_STEP_1,
    CT_CHAN_DEC_STEP_2,
    CT_CHAN_DEC_STEP_3,
    CT_CHAN_DEC_STEP_4,
    CT_CHAN_DEC_STEP_LOCKED,
    CT_CHAN_DEC_STOP,

    CT_DONE = 100,
    CT_SHUTDOWN,
};

#define STANDARD_UNKNOWN	  0
#define STANDARD_DVBT		  1
#define STANDARD_ISDBT		  2
#define STANDARD_DAB		  3
#define STANDARD_ANALOG_AUDIO 4
#define STANDARD_ANALOG       5
#define STANDARD_ATSC         6
#define STANDARD_DVBSH        7
#define STANDARD_DVBSHA2      8
#define STANDARD_CMMB         9
#define STANDARD_MISC         20
struct dibChannel {
    MS_U32 RF_kHz;
    MS_U32 bandwidth_kHz;

    MS_U8 type;

    union {
        struct dibISDBTChannel {
            MS_S8 sb_mode; /* 0 = 13seg, 1 = 1 or 3seg */
            MS_S8 partial_reception; /* sb_mode=0 -> 0=no, 1=yes, sb_mode=1 -> 0=1seg, 1=3seg */

            MS_S8 spectrum_inversion; /* always AUTO */

            MS_S8 nfft;  /* can be AUTO */
            MS_S8 guard; /* can be AUTO */

            MS_S8 sb_conn_total_seg;
            MS_S8 sb_wanted_seg;
            MS_S8 sb_subchannel;

            struct dibISDBTLayer {
                MS_S8 nb_segments;
                MS_S8 constellation;
                MS_S8 code_rate;
                MS_S8 time_intlv;
            } layer[3];
        } isdbt;
    } u;

    // private information per frontend and per channel
    struct dibChannelFEInfo {
        MS_U32 tune_time_locked;  /* the time it took for this frontend to acquired mpeg lock or sh lock */
        MS_U32 tune_time_data;    /* the time it took for this frontend to receiver data */

        MS_U32 tune_start_time;
        MS_U32 tune_time_estimation[2];

        MS_U32 priv[11]; /* memory for a frontend to store some private about this channel */
    } fe_info[MAX_NB_OF_FRONTENDS];

    struct dibChannelContext {
#define CHANNEL_STATUS_PARAMETERS_UNKNOWN   0x01
#define CHANNEL_STATUS_PARAMETERS_SET       0x02
        MS_U8 status;
        MS_U32 tune_time_estimation[2]; /* averaged real tuning time */
        int tps_available;
        MS_U16 tps[9];
    } context;

#define MAX_MONITORING_AGE          50000
#define MIN_MONITORING_REFRESH_RATE 500
    struct dibChannelMonitor {
        MS_U8 refresh; /* internal field to request a refresh of the monitoring - used by the adapter */
        MS_U32 timestamp;

        /* common frontend information */
        MS_U32 adc_power;
        MS_U16 agc_global;
        MS_U16 agc_rf;
        MS_U16 agc_bb;

        MS_U16 agc_wbd;        /* NV = AgcWbd/2^12           */
        MS_U16 agc_wbd_split;  /* NV = AgcSplitOffset/2^8    */

        /* demod */
        MS_U32 noise_power;    /* Power of Noise  - dB = 10*log10(noise_power/2^32)    */
        MS_U32 signal_power;   /* Power of Signal - dB = 10*log10(signal_power/2^32)    */

        MS_U8 quality;
        MS_U8 quality_fast;
        MS_U16 locks;
    } fe_monitor[MAX_NB_OF_FRONTENDS];
};

struct dib0090_config {
    struct dib0090_io_config io;
    /* tuner pins controlled externally */
    //int (*reset) (struct dibFrontend *, int);
    //int (*sleep) (struct dibFrontend *, int);

    /*  offset in kHz */
    int freq_offset_khz_uhf;
    int freq_offset_khz_vhf;

    //int (*get_adc_power) (struct dibFrontend *);

    MS_U8 clkouttobamse : 1; /* activate or deactivate clock output */
    MS_U8 analog_output;

    MS_U8 i2c_address;
    /* add drives and other things if necessary */
    MS_U16 wbd_vhf_offset;
    MS_U16 wbd_cband_offset;
    MS_U8 use_pwm_agc;
	MS_U8 clkoutdrive;
    MS_U8 fref_clock_ratio;
    MS_U8 cap_value;
};

struct dib0090_tuning {
    MS_U32 max_freq; /* for every frequency less than or equal to that field: this information is correct */
    MS_U8 switch_trim;
    MS_U8 lna_tune;
	MS_U8 lna_bias;
	MS_U16 v2i;
	MS_U16 mix;
	MS_U16 load;
    MS_U16 tuner_enable;
};

struct dib0090_pll {
    MS_U32 max_freq; /* for every frequency less than or equal to that field: this information is correct */
	MS_U8 vco_band;
    MS_U8 hfdiv_code;
    MS_U8 hfdiv;
    MS_U8 topresc;
};

struct dibTunerInfo {
	int caps; /*! Defines the capabilities. */
	int use_count; /*! This field is used for shared tuners not to use them in different configurations : add 2 for digital 1 for each analog (internal use only) */
    //struct dibDataBusClient data_bus_client; /*! structure to get access to the registers of the tuner */

    MS_U32 callback_time;
};

struct dib0090_state {
    struct dibTunerInfo tuner_info;
    struct dib0090_config *p_dib0090_config;
    MS_U16 revision;
    MS_U16 wbd_offset;
    MS_S16 wbd_target; /* in dB */
    MS_S16 rf_gain_limit;  /* take-over-point: where to split between bb and rf gain */
    MS_S16 current_gain; /* keeps the currently programmed gain */
    MS_U8 agc_step;     /* new binary search */
    MS_U16 gain[2]; /* for channel monitoring */
    MS_U16 *rf_ramp;
    MS_U16 *bb_ramp;
    /* for the software AGC ramps */
    MS_U16 bb_1_def;
    MS_U16 rf_lt_def;
    MS_U16 gain_reg[4];
    /* for the captrim binary search */
    MS_S8 step;
    MS_U16 adc_diff;
    MS_S8 captrim;
    MS_S8 fcaptrim;
    struct dib0090_tuning *p_current_tune_table_index;
    struct dib0090_pll *p_current_pll_table_index;
    MS_U8 tuner_is_tuned;
    MS_U8 agc_freeze;
    MS_U32 rest;
    MS_U16 wbd_uhf_offset;
    MS_U16 wbd_vhf_offset;
    MS_U16 wbd_cband_offset;
};

struct dibDemodInfo {
	MS_U32 caps;
    MS_U16 pid_filter_count;
    MS_U8 ber_rs_len;
    MS_U32 callback_time;
    MS_U32 callback_time_agc;
};

struct dib8000_state {
    struct dibDemodInfo demod_info;
    struct dib8000_config dib8000_cfg;
	MS_U16 wbd_ref;
	MS_U8 current_band;
	struct dibx000_agc_config *p_current_agc;
	MS_U32 timf_default;
	MS_U32 timf;
	MS_U16 revision;
	/* tempory */
	MS_U8 differential_constellation;
	int diversity_onoff;
	int ber_monitored_layer;
	MS_U16 saved_registers[6];
	MS_U8 layer_b_nb_seg;
	MS_U8 layer_c_nb_seg;
    MS_U8 isdbt_cfg_loaded;
};

struct dib8090_state {

    struct dib0090_config dib0090_cfg;
    struct dib8090_config *p_dib8090_cfg;

    //int (*update_lna)   (struct dibFrontend *, MS_U16);

    MS_U16 wbd_target_uhf;
    MS_U16 wbd_target_vhf;

    struct dib8090_wbd_target *p_wbd_table;
};

struct dibFrontend
{
    MS_U32 current_rf;
    MS_U32 current_bw;
    MS_U32 valid_inputs_caps[MAX_NB_OF_FRONTENDS]; /*! list of capabilities of the frontend */
    MS_S32 status;
    MS_U8 components;

    enum frontend_tune_state tune_state;

    struct dibChannel *autosearch_channel_compat;
    MS_U8 release_compat_fe;
    MS_U32 time_steady;
};

/* private use only */
enum dib8000_power_mode {
	DIB8000M_POWER_ALL = 0,
	DIB8000M_POWER_INTERFACE_ONLY,
};

enum dibx000_adc_states {
	DIBX000_SLOW_ADC_ON = 0,
	DIBX000_SLOW_ADC_OFF,
	DIBX000_ADC_ON,
	DIBX000_ADC_OFF,
	DIBX000_VBG_ENABLE,
	DIBX000_VBG_DISABLE,
};

/* output mode for frontends depending on the capabilities of the streaming interface */
#define OUTPUT_MODE_OFF               (0 << 0)
#define OUTPUT_MODE_ANALOG_ADC        (1 << 0)
#define OUTPUT_MODE_TS_FIFO           (1 << 1)
#define OUTPUT_MODE_TS_PARALLEL_GATED (1 << 2)
#define OUTPUT_MODE_TS_PARALLEL_CONT  (1 << 3)
#define OUTPUT_MODE_TS_SERIAL         (1 << 4)
#define OUTPUT_MODE_ITU               (1 << 5)
#define OUTPUT_MODE_I2S               (1 << 6)
#define OUTPUT_MODE_DIVERSITY         (1 << 7)
#define OUTPUT_MODE_DAB               (1 << 8)

/* additional IDs for GENERIC Monitoring */
#define GENERIC_MONIT_REQUEST_SIZE     (1 << 31)

/* new way to read the viterbi spectrum */
#define GENERIC_MONIT_SPECTRUM         (      1)
#define GENERIC_MONIT_SPECTRUM_SIZE    (GENERIC_MONIT_SPECTRUM | GENERIC_MONIT_REQUEST_SIZE)

/* all COMPAT-requests are not endian-aware - if there is a difference
 * between host and device - it will create a problem. For the others
 * we are big-endian */
/* compatible way to read the viterbi spectrum */
#define GENERIC_MONIT_SET_VITERBI      (      2)
#define GENERIC_MONIT_COMPAT_SPECTRUM  (      3)

/* getting the channel profile - see component_monitor.h */
#define GENERIC_MONIT_COMPAT_PROFILE   (      4)
/* big demod-monitor-structure - see component_monitor.h */
#define GENERIC_MONIT_COMPAT_EXTENDED  (      5)

/* resetting the packet error count */
#define GENERIC_MONIT_RESET_PE_COUNT   (      6)

/* mainly used for ISDB-T */
#define GENERIC_MONIT_SET_BER_LAYER    (      7)
#define GENERIC_MONIT_BER_LAYER        (      8)

#define LAYER_ALL -1
#define LAYER_A   1
#define LAYER_B   2
#define LAYER_C   3

/* SNR */
#define GENERIC_MONIT_SNR              (      9)
/* combined SNR - same as SNR but taking the SNR of the input into account */
#define GENERIC_MONIT_SNR_COMBINED     (     10)

/* a request to the tuner to get the exact tuned frequency - 32bit integer */
#define GENERIC_MONIT_RADIO_FREQUENCY  (     11)

/* PER - 16bit integer */
#define GENERIC_MONIT_PACKET_ERROR_RATE (    12)

/* Data lock (standard independent) - 8bit integer */
#define GENERIC_MONIT_LOCK_DATA        (     13)

/* SH_FEC complete monitoring */
#define GENERIC_MONIT_SH_FEC_EXTENDED  (     14)

/* SNR with signal and noise separate - 64bit integer */
#define GENERIC_MONIT_SNR_DISCRETE      (    15)

/* change ber rs len - 0..2 into offset*/
#define GENERIC_MONIT_SET_BER_RS_LEN    (    16)

/* FPGA_SHA2 complete monitoring */
#define GENERIC_MONIT_FPGA_SHA2_EXTENDED (   17)

/* request to freeze the AGC - (offset = 1 -> freeze) */
#define GENERIC_MONIT_AGC_FREEZE         (   18)

/* handle services when using the SDK wrapper */
#define GENERIC_MONIT_ADD_SERVICE        (   19)
#define GENERIC_MONIT_RM_SERVICE         (   20)
#define GENERIC_MONIT_SERVICE            (   21)
#define GENERIC_MONIT_SET_POWER_SAVING   (   22)

#define GENERIC_MONIT_CHANDEC_FROM_DEMOD (   23)

#define FFT_AUTO	90
#define FFT_2K		0
#define FFT_8K		1
#define FFT_4K		2
#define FFT_1K		3

#define GUARD_INTERVAL_AUTO 90
#define GUARD_INTERVAL_1_32  0
#define GUARD_INTERVAL_1_16  1
#define GUARD_INTERVAL_1_8   2
#define GUARD_INTERVAL_1_4   3

#define QAM_AUTO   90
#define QAM_QPSK    0
#define QAM_16QAM   1
#define QAM_64QAM   2
#define QAM_DQPSK   3
#define QAM_256QAM  4
#define VSB_8       10
#define VSB_16      11

#define INTLV_NATIVE_AUTO 90
#define INTLV_NATIVE_ON    1
#define INTLV_NATIVE_OFF   0

#define VIT_HRCH_AUTO 90
#define VIT_HRCH_OFF   0
#define VIT_HRCH_ON    1

#define VIT_PRIORITY_AUTO 90
#define VIT_PRIORITY_LP    0
#define VIT_PRIORITY_HP    1

#define VIT_ALPHA_AUTO 90
#define VIT_ALPHA_1     1
#define VIT_ALPHA_2     2
#define VIT_ALPHA_4     4

// VITERBI DECODER DVB-T/H code rates
#define VIT_CODERATE_AUTO 90
#define VIT_CODERATE_1_2   1
#define VIT_CODERATE_2_3   2
#define VIT_CODERATE_3_4   3
#define VIT_CODERATE_5_6   5
#define VIT_CODERATE_7_8   7

/* for the Demod */
struct dibDemodMonitor {
/* raw part */

	struct {
            MS_U8 agc;
            MS_U8 corm;
            MS_U8 coff;
            MS_U8 coff_cpil;
            MS_U8 lmod4;
            MS_U8 pha3;
            MS_U8 equal;
            MS_U8 vit;
            MS_U8 fec_frm;
            MS_U8 fec_mpeg;
            MS_U8 tps_dec;
            MS_U8 tps_sync;
            MS_U8 tps_data;
            MS_U8 tps_cellid;
            MS_U8 dvsy;
        /*specific ISDBT locks; for layer a, using old ones*/
            MS_U8 vit_b;
            MS_U8 vit_c;
            MS_U8 fec_mpeg_b;
            MS_U8 fec_mpeg_c;
            MS_U8 tmcc_dec;
            MS_U8 tmcc_sync;
            MS_U8 tmcc_data;
        /*specific TDMB/DAB locks*/
            MS_U8 corm_lock_dmb;
            MS_U8 ndec_tmode_lock;
            MS_U8 ndec_frame_lock;
            MS_U8 dabcoff_lock;
	} locks;

	MS_S16 iq_misgain;
	MS_S16 iq_misphi;

	MS_U32 equal_noise_mant;
	MS_S16 equal_noise_exp;
        double equal_noise_dB;

	MS_U32 equal_signal_mant;
	MS_S16 equal_signal_exp;
        double equal_signal_dB;

	MS_U16 mer_mant;
	MS_S16 mer_exp;

	MS_U32 ber_raw;

	MS_U16 PacketErrors;
	MS_U16 PacketErrorCount;

	int timing_offset;
	MS_U32 timf_current;
	MS_U32 timf_default;
	MS_U32 current_bandwidth;

	int dds_freq;
	MS_U8 invspec;
	int p_dds_freq;

	MS_U16 viterbi_syndrome;
        /*specific ISDBT*/
            MS_U16 viterbi_syndrome_b;
            MS_U16 viterbi_syndrome_c;

    struct dibChannel cur_digital_channel;

	MS_U8 way0_ok;
	MS_U8 way1_ok;
	MS_U8 divin_fifo_overflow;
	MS_U16 divin_nberr_sym;
	MS_U16 dvsy_delay;
	MS_U8 dvsy_first_arrived;

	MS_U8 adc_monitor_only;

	MS_U32 adc_power_i;
	MS_U32 adc_power_q;

	MS_U16 rf_power;

	MS_U16 agc1_raw;
	MS_U16 agc2_raw;
	MS_U16 agc_wbd_raw;
	MS_U8  agc_split;

	MS_S16 adc_lnpower_raw;
	MS_U8 adsample_lock;

	MS_U8 tuner_is_baseband;
	int tun_max_gain;
	int tun_ref_level;
	int lna_gain_step;

	int internal_clk;

	double ber;
	double viewable_dds_freq;

	double I_adc_power;  // in dBVrms
	double Q_adc_power;  // in dBVrms

	double rf_power_dbm; // in dBm
	double agc_db;       // in dB

	double agc_global;
	double agc_wbd;

	double agc1;         // in V
	double agc2;         // in V

	double adc_lnpower;

	int freq_offset;
	double timing_offset_ppm; // in ppm
	double carrier_offset;

	double iq_gain_mismatch;
	double iq_phase_mismatch;

	double CoN;

	double mer;

	MS_U8 n_adc_mode;
	MS_U8 n_2dpa_monitoring;

	int cti_def_raw[4];
	double cti_def[4];

	MS_U8 n_2dpa_mu_int;
	MS_U8 n_2dpa_num_fir;

        struct {
            MS_U8 nb_bit_quant;
            MS_U8 nb_bit_reverse_quant;
            MS_U8 tdint_full_internal;
            MS_U8 tmode;
            MS_U8 ndec_tmode;
            MS_U8 ndec_cor_max;
            MS_U8 dmb_lock_frm;

            double  small_freqoff;
            MS_S16 large_freqoff;

            MS_U8 vit_syn_subc_id;
            MS_U8 vit_syn_mode;
            MS_S16 syn;
            MS_S16 syn_fic;
            MS_S16 syn_msc;
            MS_S16 syn_subc;
            MS_S8  syn_subid;

            MS_U16 subc_id;
            MS_U16 isMSC;
            MS_U16 recfg;
            MS_U16 subc_new;
            MS_U16 cifcounter;
            MS_U16 subc_staddr;
            MS_U16 subc_sz;
            MS_U16 subc_staddr_prev;
            MS_U16 subc_sz_prev;
            MS_U16 subc_form;
            MS_U16 subc_index;
            MS_U16 subc_L1punct;
            MS_U16 subc_L2punct;
            MS_U16 subc_PI1;
            MS_U16 subc_PI2;
            MS_U16 subc_I;

            MS_U16 fec_state;
            MS_U8 fec_subid[12];
        } dab;

	double iqc_dcoff_i;
	double iqc_dcoff_q;
        MS_U8 isdbt_err_sec_layerA;
        MS_U8 isdbt_err_sec_layerB;
        MS_U8 isdbt_err_sec_layerC;

	MS_U8 mpefec_available;
	MS_U8 mpefec_request;

	struct {
		/*  DVB-H Monitoring */
		MS_U16 pid;
		MS_U32 ulTotalTables;
		MS_U32 ulErrorTablesBeforeFec;
		MS_U32 ulErrorTablesAfterFec;
		MS_U32 ulTotalPacketData;
		MS_U32 ulErrorPacketDataBeforeFec;
		MS_U32 ulErrorPacketDataAfterFec;
		MS_U8 MPEPadding;
		MS_U8 MPEPuncturing;
		MS_U16 uiTableSize;
		MS_U16 usDeltaT_Stream;
		MS_U16 usDeltaT_Detected;
		MS_U16 usBurst_Stream;
		MS_U16 usBurst_Detected;
		MS_U16 usPowerUpTime;
		MS_U32 Debug_Value_1;
		MS_U32 Debug_Value_2;
		MS_U8 tsChannelId;
	} mpefec;

    MS_U8 can_display_ber_several_layers;
	MS_U32 ber_raw_A;
	MS_U32 ber_raw_B;
	MS_U32 ber_raw_C;
	MS_U16 PacketErrors_A;
	MS_U16 PacketErrors_B;
	MS_U16 PacketErrors_C;
    double berA;
    double berB;
    double berC;

};

/* be carrefull to change those defines : used as array index in the code */
#define TUNE_TIME_LOCKED 0
#define TUNE_TIME_DATA 1

#define TUNER_CAN_VHF   0x001
#define TUNER_CAN_UHF   0x002
#define TUNER_CAN_LBAND 0x004
#define TUNER_CAN_SBAND     0x008
#define TUNER_CAN_FAST_TUNE 0x010

#define CONFIG_BAND_VHF
#define CONFIG_BAND_CBAND
#define CONFIG_BAND_UHF
#define CONFIG_BAND_LBAND
#define CONFIG_BAND_SBAND

// dib0090 priv
/* Krosus Registers */

#define KROSUS_P1C    0x001

#define CTRL_BB_1 1
#define CTRL_BB_2 2
#define CTRL_BB_3 3
#define CTRL_BB_4 4
#define CTRL_BB_5 5
#define CTRL_BB_6 6
#define CTRL_BB_7 7
#define CTRL_RXRF1 8
#define CTRL_RXRF2 9
#define CTRL_RXRF3 10
#define CTRL_RF_SW 11
#define CTRL_RF_V2I 12
#define CTRL_RF_MIX  13
#define CTRL_RF_LOAD 14
#define CTRL_RF_LT 15
#define CTRL_WBDMUX 16
#define CTRL_TX 17
#define IQ_ADC_CTRL 18
#define CTRL_BIAS 19
#define CTRL_CRYSTAL 20
#define CTRL_LO_1 21
#define CTRL_LO_2 22
#define CTRL_LO_3 23
#define CTRL_LO_4 24
#define CTRL_LO_5 25
#define DIG_CFG_RO 26
#define SLEEP_EN 27
#define CTRL_LO_6 28
#define ADCVAL 29
#define ADCCLK 30
#define VGA_MODE 31
#define DIG_CFG_3 32
#define PLL_CFG 33
#define CALIBRATE 34
#define DIG_CFG 35
#define TUNER_EN 36
#define EFUSE_1 37
#define EFUSE_2 38
#define EFUSE_3 39
#define EFUSE_4 40
#define EFUSE_CTRL 41
#define RF_RAMP1 42
#define RF_RAMP2 43
#define RF_RAMP3 44
#define RF_RAMP4 45
#define RF_RAMP5 46
#define RF_RAMP6 47
#define RF_RAMP7 48
#define RF_RAMP8 49
#define RF_RAMP9 50
#define BB_RAMP1 51
#define BB_RAMP2 52
#define BB_RAMP3 53
#define BB_RAMP4 54
#define BB_RAMP5 55
#define BB_RAMP6 56
#define BB_RAMP7 57
#define CURRGAINBB 58
#define CURRGAINRF 59
#define PWM1_REG 60
#define PWM2_REG 61
#define GAIN4_1 62
#define GAIN4_2 63

#define EN_LNA0      0x8000
#define EN_LNA1      0x4000
#define EN_LNA2      0x2000
#define EN_LNA3      0x1000
#define EN_MIX0      0x0800
#define EN_MIX1      0x0400
#define EN_MIX2      0x0200
#define EN_MIX3      0x0100
#define EN_IQADC     0x0040
#define EN_PLL       0x0020
#define EN_TX        0x0010
#define EN_BB        0x0008
#define EN_LO        0x0004
#define EN_BIAS      0x0001

#define EN_IQANA     0x0002
#define EN_DIGCLK    0x0080 /* not in the TUNER_EN reg, only in SLEEP_EN */
#define EN_CRYSTAL   0x0002

#define EN_UHF		 0x22E9
#define EN_VHF		 0x44E9
#define EN_LBD		 0x11E9
#define EN_SBD		 0x44E9
#define EN_CAB		 0x88E9

// Define the Mixer settings
#define MIX_S		0x485a
#define V2I_S           0x0240
#define LOAD_S          0x87d0

#define LPF	100

#define CONFIG_DIB0090_USE_PWM_AGC

#ifndef FE_CALLBACK_TIME_NEVER
#define FE_CALLBACK_TIME_NEVER 0xffffffff
#endif

struct dib8096_adc {
    MS_U32 freq;
    MS_U32 timf;
    MS_U32 pll_div;
};

#define DEMOD_TIMF_SET    0x00
#define DEMOD_TIMF_GET    0x01
#define DEMOD_TIMF_UPDATE 0x02

struct slope {
    MS_S16 range;
    MS_S16 slope;
};

#define WBD_ALPHA 6
/* included in [0:962] => 10 bit unsigned => GAIN_ALPHA <= 5 */
#define GAIN_ALPHA 5

#define ADC_TARGET -220

#define CHANNEL_STATUS_PARAMETERS_UNKNOWN   0x01
#define CHANNEL_STATUS_PARAMETERS_SET       0x02

#define FE_CAN_FAST_TUNE  (1 <<  7)

/* one of the following status is returned by the tune-function or more than zero which means are delay is required before calling it again */
#define FE_STATUS_TUNE_FAILED          0 // No channel was found in the given time
#define FE_STATUS_TUNE_TIMED_OUT      -1 // Tune process is not finished in the given time : continue the process later with some more time
#define FE_STATUS_TUNE_TIME_TOO_SHORT -2 // The given time was to short to make the tune process says something about the channel
#define FE_STATUS_TUNE_PENDING        -3 // Tune process is pending
#define FE_STATUS_STD_SUCCESS         -4 // A channel has been found, we know the standard, we can continue
#define FE_STATUS_FFT_SUCCESS         -5 // A channel has been found, we know the main params (Ie: FFT, Guard...) to continue
#define FE_STATUS_DEMOD_SUCCESS       -6 // A channel has been found, we are able to receive all channel params (TPS, TMCC, FIC..)
#define FE_STATUS_LOCKED              -7 // The found channel is now completly and successfully locked, but may not be stable
#define FE_STATUS_DATA_LOCKED         -8 // The found channel is now completly and successfully locked, and datas may output correctly

#define CONFIG_NO_TUNE_ASYNC
#define CONFIG_STANDARD_ISDBT

enum restart_tune_process {
    FE_RESTART_TUNE_PROCESS_FROM_TUNER = 0,
    FE_RESTART_TUNE_PROCESS_FROM_AGC,
    FE_RESTART_TUNE_PROCESS_FROM_DEMOD,
    FE_SHUTDOWN,
};

/* Chip output mode. */
#define OUTMODE_HIGH_Z                      OUTPUT_MODE_OFF
#define OUTMODE_MPEG2_PAR_GATED_CLK         OUTPUT_MODE_TS_PARALLEL_GATED
#define OUTMODE_MPEG2_PAR_CONT_CLK          OUTPUT_MODE_TS_PARALLEL_CONT
#define OUTMODE_MPEG2_SERIAL                OUTPUT_MODE_TS_SERIAL
#define OUTMODE_DIVERSITY                   OUTPUT_MODE_DIVERSITY
#define OUTMODE_MPEG2_FIFO                  OUTPUT_MODE_TS_FIFO
#define OUTMODE_ANALOG_ADC                  OUTPUT_MODE_ANALOG_ADC
#define OUTMODE_DAB                         OUTPUT_MODE_DAB

// dib0090 source code
void dib0090_dcc_freq(MS_U8 fast);
MS_U16 dib0090_find_DC(MS_U8 addr, MS_U8 off, MS_U8 pga);
void dib0090_gain_apply(MS_S16 gain_delta, MS_S16 top_delta, MS_U8 force);
int dib0090_gain_control(struct dibChannel *ch);
int dib0090_generic_monitoring(MS_U32 cmd, MS_U8 *b, MS_U32 size, MS_U32 offset);
MS_S16 dib0090_get_offset(void);
MS_U16 dib0090_get_wbd_offset(void);
MS_U16 dib0090_identify(void);
void dib0090_Offset_Calib(void);
void dib0090_pwm_gain_reset(struct dibChannel *ch);
int dib0090_reset(void);
void dib0090_reset_digital(struct dib0090_config *cfg);
void dib0090_set_bandwidth(struct dibChannel *ch);
void dib0090_set_bbramp(MS_U16 *cfg);
void dib0090_set_bbramp_pwm(MS_U16 *cfg);
void dib0090_set_boost(int onoff);
void dib0090_set_EFUSE(void);
void dib0090_set_rframp(MS_U16 *cfg);
void dib0090_set_rframp_pwm(MS_U16 *cfg);
void dib0090_set_wbd_target(MS_U16 offset_uhf, MS_U16 offset_vhf, MS_U16 offset_cband);
int dib0090_tune(struct dibChannel *ch);
int dib0090_wakeup(void);
void dib0090_wbd_calibration(void);
void dib0090_wbd_target(MS_U32 rf);
MS_S16 dib0090_wbd_to_db(MS_U16 wbd);

 // dib8000
void dib8000_i2c_gate_ctrl(MS_U8 on);
int dib8000_agc_soft_split(void);
int dib8000_agc_startup(struct dibChannel *ch);
MS_U8 dib8000_autosearch_irq(void);
int dib8000_autosearch_is_irq(void);
int dib8000_autosearch_start(struct dibChannel *ch);
int dib8000_cfg_gpio(MS_U8 num, MS_U8 dir, MS_U8 val);
MS_U32 dib8000_ctrl_timf(MS_U8 op, MS_U32 timf);
int dib8000_demod_set_output_mode(int mode);
int dib8000_generic_monitoring(MS_U32 cmd, MS_U8 *b, MS_U32 size, MS_U32 offset);
MS_S32 dib8000_get_adc_power(MS_U8 mode);
int dib8000_get_channel(struct dibChannel *cd);
int dib8000_get_demod_monitor(struct dibDemodMonitor *m);
MS_U8 dib8000_get_signal_status(void);
MS_U32 dib8000_get_signal_quality(void);
MS_U16 dib8000_get_signal_strength(void);
MS_U16 dib8000_identify(void);
void dib8000_pwm_agc_reset(struct dibChannel *ch);
int dib8000_reset(void);
int dib8000_set_gpio(MS_U8 num, MS_U8 dir, MS_U8 val);
int dib8000_wakeup(void);
int dib8000_update_pll(struct dibx000_bandwidth_config pll);
void dib8000_update_timf(void);
int dib8000_reset_gpio(void);
void dib8000_set_acquisition_mode(void);
int dib8000_set_output_mode(int mode);
int dib8000_set_bandwidth(MS_U32 bw);
int dib8000_sad_calib(void);
void dib8000_reset_pll(void);
int dib8000_set_adc_state(enum dibx000_adc_states no);
void dib8000_reset_pll_common(struct dibx000_bandwidth_config *bw);
void dib8000_set_power_mode(enum dib8000_power_mode mode);
int dib8000_set_wbd_ref(MS_U16 value);
int dib8000_set_agc_config(MS_U8 band);
void dib8000_restart_agc(void);
int dib8000_update_lna(void);
int dib8000_set_diversity_in(int onoff);
void dib8000_set_channel(struct dibChannel *ch, MS_U8 seq, MS_U8 autosearching);
int dib8000_tune(struct dibChannel *ch);
MS_S32 dib8000_check_no_signal(void);

 // dib8090
void dib8090_sip_register(void);
void dib8090_set_wbd_table_target(void);
int dib8090_tuner_sleep(int onoff);
int dib8090_agc_control(MS_U8 restart);
int dib8090_agc_startup(struct dibChannel *ch);
int dib8090_get_adc_power(void);
int dib8090_tuner_reset(int onoff);

void demod_agc_restart(void);
int demod_agc_startup_ex(struct dibChannel *ch);
int demod_autosearch_irq(void);
int demod_autosearch_start_ex(struct dibChannel *ch);
int demod_get_channel(struct dibChannel *ch);
void demod_monitor_post_process(struct dibDemodMonitor *m);
void demod_set_ber_rs_len(MS_U8 ber_rs_len);
int demod_set_diversity_in(int onoff);
int demod_set_gpio(MS_U8 num, MS_U8 dir, MS_U8 val);
int demod_set_output_mode(int mode);
int demod_set_wbd_ref(MS_U16 value);
int demod_tune_ex(struct dibChannel *ch);
int demod_wakeup(void);
void INIT_CHANNEL(struct dibChannel *ch, MS_U8 type);
void frontend_tune_diversity_sync(int num, struct dibChannel *ch);

#ifdef DIBCOM_EXTENDED_MONITORING
#ifdef CONFIG_STANDARD_ISDBT
void dump_isdbt_channel(struct dibChannel *cd);
#endif
int demod_get_monitoring(struct dibDemodMonitor *m);
int demods_have_standard(struct dibDemodMonitor m[], MS_U8 st);
void dib7000_print_monitor(struct dibDemodMonitor m[], int stat_count);
#endif

#endif
