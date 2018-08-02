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
#define NEW_REMOTE 1

#if 0//def HMS_REMOTE	//Edward_20080408, modify IR code

// the following codes are suitable for HITACHI CLE-997

#define NEC_CUST_CODE1			0x0AF5	//	(for Monitor)
#define NEC_CUST_CODE2			0x6A95	//	(for MS)
#define NEC_CUST_CODE			0x6A95	//	(for MS)

#define NEC_KEY_POWER_ON      	0x7C83	//	1
#define NEC_KEY_STANDBY 		0xFC03	//	2
#define NEC_KEY_RECALL			0x08F7	//	3
#define NEC_KEY_PMODE			0x7A85	//	4
										//	5~7: [None]
#define NEC_KEY_DTT_TV		  	0x50AF	//	8
#define NEC_KEY_1                   	0xB04F	//	9
#define NEC_KEY_2                   	0x708F	//	10
#define NEC_KEY_3                   	0xF00F	//	11
#define NEC_KEY_4                   	0x38C7	//	12
#define NEC_KEY_5                   	0xB847	//	13
#define NEC_KEY_6                   	0x7887	//	14
#define NEC_KEY_7                   	0xF807	//	15
#define NEC_KEY_8                   	0x20DF	//	16
#define NEC_KEY_9                   	0xA05F	//	17
#define NEC_KEY_TTX_FREEZE	  	0x8679	//	18
#define NEC_KEY_0                   	0x30CF	//	19
#define NEC_KEY_USB                   	0xB24D	//	20
#define NEC_KEY_VOL_UP          	0x48B7	//	21
#define NEC_KEY_CH_I_II	  		0x10EF	//	22
#define NEC_KEY_CH_UP           	0x9867	//	23
#define NEC_KEY_VOL_DOWN     	0xA857	//	24
#define NEC_KEY_MUTE		   	0xD02F	//	25
#define NEC_KEY_CH_DOWN      	0x18E7	//	26
#define NEC_KEY_RED      		  	0x32CD	//	27
#define NEC_KEY_GREEN    	  	0xA25D	//	28
#define NEC_KEY_YELLOW   	  	0x629D	//	29
#define NEC_KEY_BLUE     	  	0xE21D	//	30
#define NEC_KEY_TTX			  	0x6699	//	31
#define NEC_KEY_TTX_CANCEL		0x16E9	//	32
#define NEC_KEY_TTX_SUBTITLE 	0xF20D	//	33
#define NEC_KEY_TTX_REVEAL	  	0x56A9	//	34
#define NEC_KEY_MENU             	0xCA35	//	35 (for MS)
#define NEC_KEY_GUIDE			0x9669	//	36
#define NEC_KEY_SLEEP			0x8877	//	37
#define NEC_KEY_EXIT              	0xC53A	//	38
#define NEC_KEY_UP                 	0x0EF1	//	39
#define NEC_KEY_LEFT              	0xCE31	//	40
#define NEC_KEY_SLCT              	0xCC33	//	41
#define NEC_KEY_RIGHT            	0x4EB1	//	42
#define NEC_KEY_DOWN            	0x8E71	//	43
										//	44~45: [None]
#define NEC_KEY_AV1				0x847B	//	46
#define NEC_KEY_AV2				0x44BB	//	47
#define NEC_KEY_HDMI1			0x3CC3	//	48
#define NEC_KEY_HDMI2			0x14EB	//	49
#define NEC_KEY_RGB				0xC837	//	50
#define NEC_KEY_AUTO			0xB54A	//	51
#define NEC_KEY_LIST               	0x26D9	//	52
#define NEC_KEY_FAV               	0xA659	//	53
#define NEC_KEY_PICTURE_SIZE	0xF609	//	54
#define NEC_KEY_UNL				0XE619	//	55
#endif

#if 1
#define NEC_CUST_CODE				0x1000

#define NEC_KEY_MUTE				0x5A00
#define NEC_KEY_TTX					0x0A00
#define NEC_KEY_CC					0x0900
#define NEC_KEY_EPG					0x4A00
#define NEC_KEY_PP		          	0xA05F//NOT
#define NEC_KEY_FACTORY				0xFFFE//NOT
#define NEC_KEY_EXITALL				0xFFFD//NOT
#define NEC_KEY_STANDBY        		0x4600
#define NEC_KEY_0                   0x5000
#define NEC_KEY_1                   0x4900
#define NEC_KEY_2                   0x5500
#define NEC_KEY_3                   0x5900
#define NEC_KEY_4                  	0x4D00
#define NEC_KEY_5                   0x5100
#define NEC_KEY_6                   0x5D00
#define NEC_KEY_7                   0x4800
#define NEC_KEY_8                   0x5400
#define NEC_KEY_9                   0x5800
#define NEC_KEY_AUDIO               0x4400
#define NEC_KEY_INFO                0x1400
#define NEC_KEY_FAV                 0x1800//NOT
#define NEC_KEY_LIST                0x1000
#define NEC_KEY_EXIT                0x1B00
#define NEC_KEY_MENU                0x0700
#define NEC_KEY_UP                  0x5200
#define NEC_KEY_DOWN                0x1300
#define NEC_KEY_LEFT                0x0600
#define NEC_KEY_RIGHT               0x1A00
#define NEC_KEY_SLCT                0x0F00
#define NEC_KEY_CH_UP               0x1F00
#define NEC_KEY_CH_DOWN             0x1900
#define NEC_KEY_VOL_UP              0x1600
#define NEC_KEY_VOL_DOWN            0x1500
#define NEC_KEY_VFORMAT             0x0400//NOT
#define NEC_KEY_TV_RADIO            0x5300//NOT
#define NEC_KEY_PIP					0x4000
#define NEC_KEY_AR                  0x4F00//NOT
#define NEC_KEY_RED      			0x4700
#define NEC_KEY_GREEN    			0x4B00
#define NEC_KEY_YELLOW   			0x5700
#define NEC_KEY_BLUE     			0x5B00
/* islandhuang add for new remote keys*/
//#define NEC_KEY_TEXT				0x42BD
#define NEC_KEY_FILE				0x0000//NOT
#define NEC_KEY_PICTURE				0xC03F//NOT
/* LB new keys for 640 series */
#define NEC_KEY_PLAY     			0x0100//0xFFFB
#define NEC_KEY_PAUSE     			0x1D00//0xFFFA
#define NEC_KEY_STOP     			0x1100//0xFFF9
#define NEC_KEY_FF     				0x1E00//0xFFF8
#define NEC_KEY_REW     			0x0200//0xFFF7
#define NEC_KEY_EXT	   				0x5C00
#define NEC_KEY_TEXT				0x0C00//NOT

#define NEC_KEY_REC_LIST		    0x4300//0x00FF//NOT

#define NEC_KEY_REC					0x0D00
#define NEC_KEY_SUBT				0x1C00
#else
#define NEC_CUST_CODE				0x1000

#define NEC_KEY_MUTE				0x1C00
#define NEC_KEY_TTX					0x1300
#define NEC_KEY_CC					0xFFF0//NOT
#define NEC_KEY_EPG					0x0600
#define NEC_KEY_PP		          	0xFFF1//NOT
#define NEC_KEY_FACTORY				0xFFF2//NOT
#define NEC_KEY_EXITALL				0xFFF3//NOT
#define NEC_KEY_STANDBY        		0x1000
#define NEC_KEY_0                   0x1600
#define NEC_KEY_1                   0x0900
#define NEC_KEY_2                   0x1D00
#define NEC_KEY_3                   0x1F00
#define NEC_KEY_4                  	0x0D00
#define NEC_KEY_5                   0x1900
#define NEC_KEY_6                   0x1B00
#define NEC_KEY_7                   0x1100
#define NEC_KEY_8                   0x1500
#define NEC_KEY_9                   0x1700
#define NEC_KEY_AUDIO               0x1400
#define NEC_KEY_INFO                0x1200
#define NEC_KEY_FAV                 0x0700
#define NEC_KEY_LIST                0x0C00
#define NEC_KEY_EXIT                0x5B00
#define NEC_KEY_MENU                0x1E00
#define NEC_KEY_UP                  0x5C00
#define NEC_KEY_DOWN                0x5D00
#define NEC_KEY_LEFT                0x5E00
#define NEC_KEY_RIGHT               0x5F00
#define NEC_KEY_SLCT                0x0800
#define NEC_KEY_CH_UP               0x4800
#define NEC_KEY_CH_DOWN             0x4700
#define NEC_KEY_VOL_UP              0x5900
#define NEC_KEY_VOL_DOWN            0x5800
#define NEC_KEY_VFORMAT             0x4D00
#define NEC_KEY_TV_RADIO            0x4E00
#define NEC_KEY_PIP					0xFFF4//NOT
#define NEC_KEY_AR                  0x4000
#define NEC_KEY_RED      			0x5700
#define NEC_KEY_GREEN    			0x0E00
#define NEC_KEY_YELLOW   			0x1A00
#define NEC_KEY_BLUE     			0x0A00
/* islandhuang add for new remote keys*/
//#define NEC_KEY_TEXT				0x42BD
#define NEC_KEY_FILE				0x0300
#define NEC_KEY_PICTURE				0xFFF5//NOT
/* LB new keys for 640 series */
#define NEC_KEY_PLAY     			0x5200
#define NEC_KEY_PAUSE     			0xC200
#define NEC_KEY_STOP     			0x2A00
#define NEC_KEY_FF     				0x6A00
#define NEC_KEY_REW     			0x3200
#define NEC_KEY_EXT	   				0xFFF6//NOT
#define NEC_KEY_TEXT				0x4200

#define NEC_KEY_REC_LIST		    0x0000

#define NEC_KEY_REC					0xAA00
#define NEC_KEY_SUBT				0x0F00

#endif

#if 0

#define NEC_CUST_CODE				0x38C7

#define NEC_KEY_MUTE				0x40BF
#define NEC_KEY_TTX					0xB24D
#define NEC_KEY_CC					0x52AD
#define NEC_KEY_EPG					0xFFFF
#define NEC_KEY_PP		          	0xEA15
#define NEC_KEY_FACTORY				0xFFFE
#define NEC_KEY_EXITALL				0xFFFD
#define NEC_KEY_STANDBY        		0x9A65
#define NEC_KEY_0                   0xA857
#define NEC_KEY_1                   0x50AF
#define NEC_KEY_2                   0x906F
#define NEC_KEY_3                   0x10EF
#define NEC_KEY_4                  	0x48B7
#define NEC_KEY_5                   0x8877
#define NEC_KEY_6                   0x08F7
#define NEC_KEY_7                   0x708F
#define NEC_KEY_8                   0xB04F
#define NEC_KEY_9                   0x30CF
#define NEC_KEY_AUDIO               0xD827
#define NEC_KEY_INFO                0x0AF5
#define NEC_KEY_FAV                 0x18E7
#define NEC_KEY_LIST                0x58A7
#define NEC_KEY_EXIT                0x9867
#define NEC_KEY_MENU                0x28D7
#define NEC_KEY_UP                  0xB847
#define NEC_KEY_DOWN                0xC837
#define NEC_KEY_LEFT                0x7887
#define NEC_KEY_RIGHT               0x38C7
#define NEC_KEY_SLCT                0xD02F
#define NEC_KEY_CH_UP               0xAA55
#define NEC_KEY_CH_DOWN             0x728D
#define NEC_KEY_VOL_UP              0x32CD
#define NEC_KEY_VOL_DOWN            0x2AD5
#define NEC_KEY_VFORMAT             0x609F
#define NEC_KEY_TV_RADIO            0x6A95
#define NEC_KEY_PIP					0xFFFC
#define NEC_KEY_AR                  0xFA05
#define NEC_KEY_RED      			0x926D
#define NEC_KEY_GREEN    			0x12ED
#define NEC_KEY_YELLOW   			0xF00F
#define NEC_KEY_BLUE     			0xE817
/* LB new keys for 640 series */
#define NEC_KEY_PLAY     			0x3AC5
#define NEC_KEY_PAUSE     			0xC23D
#define NEC_KEY_STOP     			0x42BD
#define NEC_KEY_FF     				0xE21D
#define NEC_KEY_REW     			0x7A85
#define NEC_KEY_FILE     			0x1AE5
#define NEC_KEY_PICTURE   			0x20DF
#define NEC_KEY_EXT	   				0xDA25
#define NEC_KEY_TEXT				0xFFFB

#endif
