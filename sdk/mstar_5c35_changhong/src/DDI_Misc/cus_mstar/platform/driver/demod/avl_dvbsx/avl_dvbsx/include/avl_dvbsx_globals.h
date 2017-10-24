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
/*
 *           Copyright 2010 Availink, Inc.
 *
 *  This software contains Availink proprietary information and
 *  its use and disclosure are restricted solely to the terms in
 *  the corresponding written license agreement. It shall not be 
 *  disclosed to anyone other than valid licensees without
 *  written permission of Availink, Inc.
 *
 */


#ifndef AVL_DVBSX_GLOBALS_H
#define AVL_DVBSX_GLOBALS_H

#define raptor_status_addr						  (0x00000860 + 0x0)
#define rx_rsp_addr                                                             (0x00000200 + 0x0)
#define rx_cmd_addr                                                             (0x00000400 + 0x0)
#define i2cm_cmd_addr                                                           (0x00000404 + 0x0)
#define i2cm_rsp_addr                                                           (0x00000418 + 0x0)
#define error_msg_addr                                                          (0x0000042c + 0x0)
#define core_ready_word_addr                                                    (0x00000434 + 0x0)
#define i2cm_status_addr                                                        (0x00000438 + 0x0)
#define rx_config_addr                                                          (0x0000043c + 0x0)
#define rx_state_addr                                                           (0x00000690 + 0x0)
#define sharemem_addr                                                           (0x000008d8 + 0x0)
#define patchglobalvar_addr                                                     (0x00002614 + 0x0)
#define patchtext_addr                                                          (0x00002654 + 0x0)
#define rom_ver_addr                                                            (0x00100000 + 0x0)
#define rx_config_rom_addr                                                      (0x00100004 + 0x0)


#define	rc_rfagc_pol_addr                                                       (rx_config_addr + 0x0)
#define	rc_alpha_addr                                                           (rx_config_addr + 0x4)
#define	rc_equalizer_addr                                                       (rx_config_addr + 0x8)
#define	rc_internal_decode_mode_addr                                            (rx_config_addr + 0xc)
#define	rc_format_addr                                                          (rx_config_addr + 0x10)
#define	rc_input_addr                                                           (rx_config_addr + 0x14)
#define	rc_interleave_mode_addr                                                 (rx_config_addr + 0x18)
#define	rc_pkt_err_pol_addr                                                     (rx_config_addr + 0x1c)
#define	rc_mpeg_mode_addr                                                       (rx_config_addr + 0x20)
#define	rc_outpin_sel_addr                                                      (rx_config_addr + 0x24)
#define	rc_pkt_int_pattern_addr                                                 (rx_config_addr + 0x28)
#define	rc_mpeg_seri_seq_addr                                                   (rx_config_addr + 0x2c)
#define	rc_alpha_setting_addr                                                   (rx_config_addr + 0x30)
#define	rc_specinv_addr                                                         (rx_config_addr + 0x34)
#define	rc_stdout_port_addr                                                     (rx_config_addr + 0x38)
#define	rc_dvbs_snr_mode_addr                                                   (rx_config_addr + 0x3c)
#define	rc_blind_scan_avg_blk_num_addr                                          (rx_config_addr + 0x40)
#define	rc_fec_coderate_addr                                                    (rx_config_addr + 0x44)
#define	rc_fec_modulation_addr                                                  (rx_config_addr + 0x48)
#define	rc_show_mod_pilot_addr                                                  (rx_config_addr + 0x4c)
#define	rc_btr_cd_th_addr                                                       (rx_config_addr + 0x50)
#define	rc_int_sym_rate_MHz_addr                                                (rx_config_addr + 0x54)
#define	rc_blind_scan_sweep_range_hz_addr                                       (rx_config_addr + 0x58)
#define	rc_blind_scan_acq_th_addr                                               (rx_config_addr + 0x5c)
#define	rc_blind_scan_cd_th_addr                                                (rx_config_addr + 0x60)
#define	rc_dvbs2_eq_adpt_sw_gain_acq_addr                                       (rx_config_addr + 0x64)
#define	rc_dvbs_dss_eq_adpt_sw_gain_acq_addr                                    (rx_config_addr + 0x68)
#define	rc_old_lwm_addr                                                         (rx_config_addr + 0x6c)
#define	rc_residue_addr                                                         (rx_config_addr + 0x70)
#define	rc_old_frame_cnt_addr                                                   (rx_config_addr + 0x74)
#define	rc_error_addr                                                           (rx_config_addr + 0x78)
#define	rc_patch_ver_addr                                                       (rx_config_addr + 0x7c)
#define	rc_expected_ROM_CRC_addr                                                (rx_config_addr + 0x80)
#define	rc_ROM_CRC_start_addr_addr                                              (rx_config_addr + 0x84)
#define	rc_ROM_CRC_end_addr_addr                                                (rx_config_addr + 0x88)
#define	rc_modcode_corr_addr                                                    (rx_config_addr + 0x8c)
#define	rc_softcode_corr_addr                                                   (rx_config_addr + 0x90)
#define	rc_softcode_uw_addr                                                     (rx_config_addr + 0x94)
#define	rc_dvbs_ber_addr                                                        (rx_config_addr + 0x98)
#define	rc_blind_scan_sym_rate_tol_hz_addr                                      (rx_config_addr + 0x9c)
#define	rc_coarse_num_adjust_factor_addr                                        (rx_config_addr + 0xa0)
#define	rc_num_freq_candidate_addr                                              (rx_config_addr + 0xa4)
#define	rc_rfagc_dcm_addr                                                       (rx_config_addr + 0xa6)
#define	rc_dagc_ref_addr                                                        (rx_config_addr + 0xa8)
#define	rc_aagc_ref_addr                                                        (rx_config_addr + 0xaa)
#define	rc_acq_coincidence_threshold_addr                                       (rx_config_addr + 0xac)
#define	rc_acq_timeout_frames_addr                                              (rx_config_addr + 0xae)
#define	rc_fec_wait_timeout_frames_addr                                         (rx_config_addr + 0xb0)
#define	rc_freq_init_est_frames_addr                                            (rx_config_addr + 0xb2)
#define	rc_freq_est_frames_addr                                                 (rx_config_addr + 0xb4)
#define	rc_lfsr_fb_invert_addr                                                  (rx_config_addr + 0xb6)
#define	rc_lfsr_mode_addr                                                       (rx_config_addr + 0xb8)
#define	rc_pkt_lfsr_en_addr                                                     (rx_config_addr + 0xba)
#define	rc_mpeg_posedge_addr                                                    (rx_config_addr + 0xbc)
#define	rc_mpeg_serial_addr                                                     (rx_config_addr + 0xbe)
#define	rc_seri_rate_frac_n_addr                                                (rx_config_addr + 0xc0)
#define	rc_seri_rate_frac_d_addr                                                (rx_config_addr + 0xc2)
#define	rc_para_rate_adj_n_addr                                                 (rx_config_addr + 0xc4)
#define	rc_para_rate_adj_d_addr                                                 (rx_config_addr + 0xc6)
#define	rc_show_demod_status_addr                                               (rx_config_addr + 0xc8)
#define	rc_show_per_addr                                                        (rx_config_addr + 0xca)
#define	rc_show_ber_addr                                                        (rx_config_addr + 0xcc)
#define	rc_show_wer_addr                                                        (rx_config_addr + 0xce)
#define	rc_show_detail_addr                                                     (rx_config_addr + 0xd0)
#define	rc_show_scatter_addr                                                    (rx_config_addr + 0xd2)
#define	rc_show_period_frames_addr                                              (rx_config_addr + 0xd4)
#define	rc_show_coeffs_addr                                                     (rx_config_addr + 0xd6)
#define	rc_pkt_err_mode_addr                                                    (rx_config_addr + 0xd8)
#define	rc_uw_lock_det_frames_addr                                              (rx_config_addr + 0xda)
#define	rc_uw_lock_det_thresh_addr                                              (rx_config_addr + 0xdc)
#define	rc_ddc_buffer_offset_addr                                               (rx_config_addr + 0xde)
#define	rc_eq_buffer_offset_addr                                                (rx_config_addr + 0xe0)
#define	rc_disable_freq_adj_addr                                                (rx_config_addr + 0xe2)
#define	rc_crc_enabled_addr                                                     (rx_config_addr + 0xe4)
#define	rc_interleave_shift_addr                                                (rx_config_addr + 0xe6)
#define	rc_skip_tune_addr                                                       (rx_config_addr + 0xe8)
#define	rc_skip_freq_est_addr                                                   (rx_config_addr + 0xea)
#define	rc_uw_corr_peak_num_addr                                                (rx_config_addr + 0xec)
#define	rc_lock_detect_frm_num_addr                                             (rx_config_addr + 0xee)
#define	rc_max_demod_reset_num_addr                                             (rx_config_addr + 0xf0)
#define	rc_max_freq_tune_num_addr                                               (rx_config_addr + 0xf2)
#define	rc_dagc_acq_gain_addr                                                   (rx_config_addr + 0xf4)
#define	rc_dagc_trk_gain_addr                                                   (rx_config_addr + 0xf6)
#define	rc_aagc_acq_gain_addr                                                   (rx_config_addr + 0xf8)
#define	rc_aagc_trk_gain_addr                                                   (rx_config_addr + 0xfa)
#define	rc_eq_loop_gain_addr                                                    (rx_config_addr + 0xfc)
#define	rc_eq_center_tap_position_addr                                          (rx_config_addr + 0xfe)
#define	rc_eq_cnvg_frames_addr                                                  (rx_config_addr + 0x100)
#define	rc_dvbs2_eq_adpt_sw_gain_trk_addr                                       (rx_config_addr + 0x102)
#define	rc_dvbs_dss_eq_adpt_sw_gain_trk_addr                                    (rx_config_addr + 0x104)
#define	rc_dvbs2_eq_adpt_sw_symbol_acq_addr                                     (rx_config_addr + 0x106)
#define	rc_dvbs2_eq_adpt_sw_symbol_trk_addr                                     (rx_config_addr + 0x108)
#define	rc_dvbs_dss_eq_adpt_sw_symbol_acq_addr                                  (rx_config_addr + 0x10a)
#define	rc_dvbs_dss_eq_adpt_sw_symbol_trk_addr                                  (rx_config_addr + 0x10c)
#define	rc_pnref_addr                                                           (rx_config_addr + 0x10e)
#define	rc_dc_comp_acq_gain_addr                                                (rx_config_addr + 0x110)
#define	rc_dc_comp_trk_gain_addr                                                (rx_config_addr + 0x112)
#define	rc_dc_comp_init_i_addr                                                  (rx_config_addr + 0x114)
#define	rc_dc_comp_init_q_addr                                                  (rx_config_addr + 0x116)
#define	rc_iq_comp_acq_gain_addr                                                (rx_config_addr + 0x118)
#define	rc_iq_comp_trk_gain_addr                                                (rx_config_addr + 0x11a)
#define	rc_iq_amp_imb_addr                                                      (rx_config_addr + 0x11c)
#define	rc_iq_phs_imb_addr                                                      (rx_config_addr + 0x11e)
#define	rc_dc_comp_enable_addr                                                  (rx_config_addr + 0x120)
#define	rc_iq_comp_enable_addr                                                  (rx_config_addr + 0x122)
#define	rc_snr_settle_frames_addr                                               (rx_config_addr + 0x124)
#define	rc_btr_acq_lpbw_addr                                                    (rx_config_addr + 0x126)
#define	rc_btr_uw_lpbw_addr                                                     (rx_config_addr + 0x128)
#define	rc_cpr_lpbw_addr                                                        (rx_config_addr + 0x12a)
#define	rc_cpr_acq_lpbw_addr                                                    (rx_config_addr + 0x12c)
#define	rc_cpr_trk_lpbw_addr                                                    (rx_config_addr + 0x12e)
#define	rc_cpr_freq_acq_rshft_addr                                              (rx_config_addr + 0x130)
#define	rc_cpr_freq_trk_rshft_np_addr                                           (rx_config_addr + 0x132)
#define	rc_cpr_freq_trk_ini_rshft_np_addr                                       (rx_config_addr + 0x134)
#define	rc_cpr_freq_trk_rshft_p_addr                                            (rx_config_addr + 0x136)
#define	rc_cpr_freq_trk_ini_rshft_p_addr                                        (rx_config_addr + 0x138)
#define	rc_btr_dwell_time_ksym_addr                                             (rx_config_addr + 0x13a)
#define	rc_cpr_output_en_addr                                                   (rx_config_addr + 0x13c)
#define	rc_cpr_lck_window_len_addr                                              (rx_config_addr + 0x13e)
#define	rc_cpr_lck_unlock_cnt_addr                                              (rx_config_addr + 0x140)
#define	rc_cpr_lck_thr_cpr_addr                                                 (rx_config_addr + 0x142)
#define	rc_cpr_dvbs_cnt_len_addr                                                (rx_config_addr + 0x144)
#define	rc_cpr_freq_swp_upd_rate_addr                                           (rx_config_addr + 0x146)
#define	rc_cpr_freq_swp_step_addr                                               (rx_config_addr + 0x148)
#define	rc_cpr_freq_swp_times_addr                                              (rx_config_addr + 0x14a)
#define	rc_cpr_freq_swp_rng_addr                                                (rx_config_addr + 0x14c)
#define	rc_cpr_dvbs_dss_acq_lpbw_addr                                           (rx_config_addr + 0x14e)
#define	rc_cpr_dvbs_dss_trk_lpbw_addr                                           (rx_config_addr + 0x150)
#define	rc_cpr_freq_swp_init_val_addr                                           (rx_config_addr + 0x152)
#define	rc_dvbs_dss_snr_est_blk_size_addr                                       (rx_config_addr + 0x154)
#define	rc_dvbs_dss_snr_est_lpbw_addr                                           (rx_config_addr + 0x156)
#define	rc_dvbs_dss_fec_init_sel_pkt_number_addr                                (rx_config_addr + 0x158)
#define	rc_fec_min_indx_repetitions_cm_addr                                     (rx_config_addr + 0x15a)
#define	rc_fec_max_frame_chk_cm_addr                                            (rx_config_addr + 0x15c)
#define	rc_dvbs_bypass_bit_descrambler_addr                                     (rx_config_addr + 0x15e)
#define	rc_int_nom_carrier_freq_MHz_addr                                        (rx_config_addr + 0x160)
#define	rc_int_dmd_clk_MHz_addr                                                 (rx_config_addr + 0x162)
#define	rc_int_fec_clk_MHz_addr                                                 (rx_config_addr + 0x164)
#define	rc_int_mpeg_clk_MHz_addr                                                (rx_config_addr + 0x166)
#define	rc_int_btr_loop_bandwidth_addr                                          (rx_config_addr + 0x168)
#define	rc_int_cpr_loop_bandwidth_addr                                          (rx_config_addr + 0x16a)
#define	rc_int_carrier_freq_half_range_MHz_addr                                 (rx_config_addr + 0x16c)
#define	rc_int_normalized_freq_step_addr                                        (rx_config_addr + 0x16e)
#define	rc_int_lock_detect_loop_bw_addr                                         (rx_config_addr + 0x170)
#define	rc_int_snr_measurement_tolerance_dB_addr                                (rx_config_addr + 0x172)
#define	rc_dvbs2_low_power_mode_addr                                            (rx_config_addr + 0x174)
#define	rc_demap_scale_addr                                                     (rx_config_addr + 0x176)
#define	rc_fec_lock_num_addr                                                    (rx_config_addr + 0x178)
#define	rc_fec_lock_leak_addr                                                   (rx_config_addr + 0x17a)
#define	rc_fec_lock_threshold_addr                                              (rx_config_addr + 0x17c)
#define	rc_dvbs_fec_lock_leak_addr                                              (rx_config_addr + 0x17e)
#define	rc_dvbs_fec_lock_threshold_addr                                         (rx_config_addr + 0x180)
#define	rc_modcod_descramble_flag_addr                                          (rx_config_addr + 0x182)
#define	rc_fec_min_indx_repetitions_addr                                        (rx_config_addr + 0x184)
#define	rc_dvbs_fec_fcnt_step_addr                                              (rx_config_addr + 0x186)
#define	rc_fec_only_addr                                                        (rx_config_addr + 0x188)
#define	rc_toggle_fec_mode_addr                                                 (rx_config_addr + 0x18a)
#define	rc_fec_bypass_depunc_addr                                               (rx_config_addr + 0x18c)
#define	rc_fec_max_frame_chk_addr                                               (rx_config_addr + 0x18e)
#define	rc_dvbs_fec_max_trial_for_same_phase_addr                               (rx_config_addr + 0x190)
#define	rc_fec_deintrlvr_rs_rst_addr                                            (rx_config_addr + 0x192)
#define	rc_fec_bypass_coderate_addr                                             (rx_config_addr + 0x194)
#define	rc_vtrbi_performance_test_addr                                          (rx_config_addr + 0x196)
#define	rc_fec_pk_thrshld_addr                                                  (rx_config_addr + 0x198)
#define	rc_dss_fec_max_trial_for_same_phase_addr                                (rx_config_addr + 0x19a)
#define	rc_dss_fec_fcnt_step_addr                                               (rx_config_addr + 0x19c)
#define	rc_btr_cd_num_addr                                                      (rx_config_addr + 0x19e)
#define	rc_accel_track_addr                                                     (rx_config_addr + 0x1a0)
#define	rc_mpeg_sync_byte_addr                                                  (rx_config_addr + 0x1a2)
#define	rc_dvbs_equalizer_cnvg_iter_no_addr                                     (rx_config_addr + 0x1a4)
#define	rc_modcod_scrmb_mmsb_addr                                               (rx_config_addr + 0x1a6)
#define	rc_modcod_scrmb_mlsb_addr                                               (rx_config_addr + 0x1a8)
#define	rc_modcod_scrmb_lmsb_addr                                               (rx_config_addr + 0x1aa)
#define	rc_modcod_scrmb_llsb_addr                                               (rx_config_addr + 0x1ac)
#define	rc_i2cm_speed_kHz_addr                                                  (rx_config_addr + 0x1ae)
#define	rc_i2cm_time_out_addr                                                   (rx_config_addr + 0x1b0)
#define	rc_tuner_frequency_100kHz_addr                                          (rx_config_addr + 0x1b2)
#define	rc_tuner_LPF_100kHz_addr                                                (rx_config_addr + 0x1b4)
#define	rc_tuner_slave_addr_addr                                                (rx_config_addr + 0x1b6)
#define	rc_tuner_max_LPF_100kHz_addr                                            (rx_config_addr + 0x1b8)
#define	rc_tuner_LPF_margin_100kHz_addr                                         (rx_config_addr + 0x1ba)
#define	rc_tuner_use_internal_control_addr                                      (rx_config_addr + 0x1bc)
#define	rc_blind_scan_start_freq_100kHz_addr                                    (rx_config_addr + 0x1be)
#define	rc_blind_scan_end_freq_100kHz_addr                                      (rx_config_addr + 0x1c0)
#define	rc_blind_scan_min_sym_rate_kHz_addr                                     (rx_config_addr + 0x1c2)
#define	rc_blind_scan_max_sym_rate_kHz_addr                                     (rx_config_addr + 0x1c4)
#define	rc_blind_scan_channel_info_offset_addr                                  (rx_config_addr + 0x1c6)
#define	rc_blind_scan_process_range_100kHz_addr                                 (rx_config_addr + 0x1c8)
#define	rc_blind_scan_sweep_scalor_addr                                         (rx_config_addr + 0x1ca)
#define	rc_blind_scan_sweep_max_addr                                            (rx_config_addr + 0x1cc)
#define	rc_blind_scan_low_sym_swp_range_addr                                    (rx_config_addr + 0x1ce)
#define	rc_blind_scan_acq_num_addr                                              (rx_config_addr + 0x1d0)
#define	rc_blind_scan_cd_num_addr                                               (rx_config_addr + 0x1d2)
#define	rc_blind_scan_acq_btr_lbw_addr                                          (rx_config_addr + 0x1d4)
#define	rc_blind_scan_cd_btr_lbw_addr                                           (rx_config_addr + 0x1d6)
#define	rc_blind_scan_uw_en_addr                                                (rx_config_addr + 0x1d8)
#define	rc_blind_scan_ts_retry_addr                                             (rx_config_addr + 0x1da)
#define	rc_blind_scan_freq_step_100kHz_addr                                     (rx_config_addr + 0x1dc)
#define	rc_blind_scan_freq_decimation_addr                                      (rx_config_addr + 0x1de)
#define	rc_blind_scan_mode_addr                                                 (rx_config_addr + 0x1e0)
#define	rc_blind_scan_show_detail_addr                                          (rx_config_addr + 0x1e2)
#define	rc_blind_scan_max_left1_100kHz_addr                                     (rx_config_addr + 0x1e4)
#define	rc_blind_scan_max_left2_100kHz_addr                                     (rx_config_addr + 0x1e6)
#define	rc_blind_scan_freq_back_100kHz_addr                                     (rx_config_addr + 0x1e8)
#define	rc_descram_X_init_msb_addr                                              (rx_config_addr + 0x1ea)
#define	rc_descram_X_init_lsb_addr                                              (rx_config_addr + 0x1ec)
#define	rc_descram_Y_init_msb_addr                                              (rx_config_addr + 0x1ee)
#define	rc_descram_Y_init_lsb_addr                                              (rx_config_addr + 0x1f0)
#define	rc_pl_descram_en_addr                                                   (rx_config_addr + 0x1f2)
#define	rc_iq_try_times_addr                                                    (rx_config_addr + 0x1f4)
#define	rc_iq_retries_addr                                                      (rx_config_addr + 0x1f6)
#define	rc_pll_fec_addr                                                         (rx_config_addr + 0x1f8)
#define	rc_pll_mpeg_addr                                                        (rx_config_addr + 0x1fa)
#define	rc_in_soft_sleep_mode_addr                                              (rx_config_addr + 0x1fc)
#define	rc_in_hard_sleep_mode_addr                                              (rx_config_addr + 0x1fe)
#define	rc_in_sleep_mode_addr                                                   (rx_config_addr + 0x200)
#define	rc_decode_mode_addr                                                     (rx_config_addr + 0x202)
#define	rc_iq_mode_addr                                                         (rx_config_addr + 0x204)
#define	rc_blind_scan_carrier_db_addr                                           (rx_config_addr + 0x206)
#define	rc_dishpoint_mode_addr                                                  (rx_config_addr + 0x208)
#define	rc_lock_mode_addr                                                       (rx_config_addr + 0x20a)
#define	rc_functional_mode_addr                                                 (rx_config_addr + 0x20c)
#define	rc_blind_scan_reset_addr                                                (rx_config_addr + 0x20e)
#define	rc_mpeg_continuous_mode_control_addr                                    (rx_config_addr + 0x210)
#define	rc_hwm_threshold_addr                                                   (rx_config_addr + 0x212)
#define	rc_lwm_threshold_addr                                                   (rx_config_addr + 0x214)
#define	rc_pll_adust_period_addr                                                (rx_config_addr + 0x216)
#define	rc_time_constant_addr                                                   (rx_config_addr + 0x218)
#define	rc_pkt_fifo_hwm_addr                                                    (rx_config_addr + 0x21a)
#define	rc_pkt_fifo_lwm_addr                                                    (rx_config_addr + 0x21c)
#define	rc_bs_pk_to_avg_threshold_addr                                          (rx_config_addr + 0x21e)
#define	rc_blind_scan_tuner_spectrum_inversion_addr                             (rx_config_addr + 0x220)


#define	rs_main_state_addr                                                      (rx_state_addr + 0x0)
#define	rs_last_main_state_addr                                                 (rx_state_addr + 0x4)
#define	rs_code_rate_addr                                                       (rx_state_addr + 0x8)
#define	rs_modulation_addr                                                      (rx_state_addr + 0xc)
#define	rs_pilot_addr                                                           (rx_state_addr + 0x10)
#define	rs_frame_length_addr                                                    (rx_state_addr + 0x14)
#define	rs_frame_count_addr                                                     (rx_state_addr + 0x18)
#define	rs_int_decim_samp_rate_MHz_addr                                         (rx_state_addr + 0x1c)
#define	rs_int_freq_MHz_addr                                                    (rx_state_addr + 0x20)
#define	rs_int_SNR_dB_addr                                                      (rx_state_addr + 0x40)
#define	rs_int_ck_i_addr                                                        (rx_state_addr + 0x44)
#define	rs_int_ck_q_addr                                                        (rx_state_addr + 0x48)
#define	rs_int_ek_addr                                                          (rx_state_addr + 0x4c)
#define	rs_int_snr_addr                                                         (rx_state_addr + 0x50)
#define	rs_int_instant_snr_addr                                                 (rx_state_addr + 0x54)
#define	rs_eq_loop_gain_scl_addr                                                (rx_state_addr + 0x58)
#define	rs_eq_loop_gain_rshft_addr                                              (rx_state_addr + 0x5c)
#define	rs_eq_i_coeffs_addr                                                     (rx_state_addr + 0x60)
#define	rs_eq_q_coeffs_addr                                                     (rx_state_addr + 0x90)
#define	rs_corr_val_addr                                                        (rx_state_addr + 0xc0)
#define	rs_mode_code_addr                                                       (rx_state_addr + 0xe0)
#define	rs_demod_reset_times_addr                                               (rx_state_addr + 0xe4)
#define	rs_uw_clk32cnt_acc_addr                                                 (rx_state_addr + 0xe8)
#define	rs_freq_tune_times_addr                                                 (rx_state_addr + 0xec)
#define	rs_fec_err_accum_addr                                                   (rx_state_addr + 0xf0)
#define	rs_equalizer_control_addr                                               (rx_state_addr + 0xf4)
#define	rs_eq_adpt_sw_gain_addr                                                 (rx_state_addr + 0xf8)
#define	rs_int_mer_addr                                                         (rx_state_addr + 0xfc)
#define	rs_int_mer_s_addr                                                       (rx_state_addr + 0x100)
#define	rs_int_mer_e_addr                                                       (rx_state_addr + 0x104)
#define	rs_int_mer_e_i_addr                                                     (rx_state_addr + 0x108)
#define	rs_int_mer_e_q_addr                                                     (rx_state_addr + 0x10c)
#define	rs_blind_scan_bin_size_Hz_addr                                          (rx_state_addr + 0x110)
#define	rs_calc_ROM_CRC_addr                                                    (rx_state_addr + 0x114)
#define	rs_uw_i_ptr_addr                                                        (rx_state_addr + 0x118)
#define	rs_uw_q_ptr_addr                                                        (rx_state_addr + 0x11c)
#define	rs_uw_i_ref_ptr_addr                                                    (rx_state_addr + 0x120)
#define	rs_uw_q_ref_ptr_addr                                                    (rx_state_addr + 0x124)
#define	rs_mc_i_ref_ptr_addr                                                    (rx_state_addr + 0x128)
#define	rs_mc_q_ref_ptr_addr                                                    (rx_state_addr + 0x12c)
#define	rs_mc_ptr_addr                                                          (rx_state_addr + 0x130)
#define	rs_xi_ptr_addr                                                          (rx_state_addr + 0x134)
#define	rs_xq_ptr_addr                                                          (rx_state_addr + 0x138)
#define	rs_int_buf_ptr_addr                                                     (rx_state_addr + 0x13c)
#define	rs_eq_adpt_sw_symbol_addr                                               (rx_state_addr + 0x140)
#define	rs_int_carrier_freq_100kHz_addr                                         (rx_state_addr + 0x142)
#define	rs_num_uw_acquisition_addr                                              (rx_state_addr + 0x144)
#define	rs_next_punc_cnt_addr                                                   (rx_state_addr + 0x146)
#define	rs_next_punc_cnt_mod_32_addr                                            (rx_state_addr + 0x148)
#define	rs_loop_max_addr                                                        (rx_state_addr + 0x14a)
#define	rs_fcnt_step_temp_addr                                                  (rx_state_addr + 0x14c)
#define	rs_acq_state_addr                                                       (rx_state_addr + 0x14e)
#define	rs_indx_trial_addr                                                      (rx_state_addr + 0x150)
#define	rs_min_occurance_addr                                                   (rx_state_addr + 0x152)
#define	rs_fcnt_step_addr                                                       (rx_state_addr + 0x154)
#define	rs_max_trial_addr                                                       (rx_state_addr + 0x156)
#define	rs_loop_addr                                                            (rx_state_addr + 0x158)
#define	rs_max_occ_addr                                                         (rx_state_addr + 0x15a)
#define	rs_sync_indx_addr                                                       (rx_state_addr + 0x15c)
#define	rs_dvbs_dss_fec_lock_addr                                               (rx_state_addr + 0x15e)
#define	rs_sym_offset_addr                                                      (rx_state_addr + 0x160)
#define	rs_uw_lock_addr                                                         (rx_state_addr + 0x162)
#define	rs_fec_lock_addr                                                        (rx_state_addr + 0x164)
#define	rs_snr_count_addr                                                       (rx_state_addr + 0x166)
#define	rs_freq_est_frames_addr                                                 (rx_state_addr + 0x168)
#define	rs_valid_modcod_addr                                                    (rx_state_addr + 0x16a)
#define	rs_eq_cnvg_status_addr                                                  (rx_state_addr + 0x16c)
#define	rs_freq_est_status_addr                                                 (rx_state_addr + 0x16e)
#define	rs_rx_rsp_ptr_addr                                                      (rx_state_addr + 0x170)
#define	rs_demap_table_state_addr                                               (rx_state_addr + 0x172)
#define	rs_ldpc_table_state_addr                                                (rx_state_addr + 0x174)
#define	rs_ROM_CRC_status_addr                                                  (rx_state_addr + 0x176)
#define	rs_blind_scan_btr_lock_addr                                             (rx_state_addr + 0x178)
#define	rs_blind_scan_channel_count_addr                                        (rx_state_addr + 0x17a)
#define	rs_blind_scan_progress_addr                                             (rx_state_addr + 0x17c)
#define	rs_blind_scan_error_code_addr                                           (rx_state_addr + 0x17e)
#define	rs_blind_scan_bin_numbers_addr                                          (rx_state_addr + 0x180)
#define	rs_blind_scan_max_left1_bins_addr                                       (rx_state_addr + 0x182)
#define	rs_blind_scan_max_left2_bins_addr                                       (rx_state_addr + 0x184)
#define	rs_tuner_status_addr                                                    (rx_state_addr + 0x186)
#define	rs_ScatterData_rdy_addr                                                 (rx_state_addr + 0x188)


#define	rp_uint_BER_addr                                                        (raptor_status_addr + 0x0)
#define	rp_uint_PER_addr                                                        (raptor_status_addr + 0x4)
#define	rp_uint_WER_addr                                                        (raptor_status_addr + 0x8)


#define c68k_ready_word_addr                     core_ready_word_addr
#endif
