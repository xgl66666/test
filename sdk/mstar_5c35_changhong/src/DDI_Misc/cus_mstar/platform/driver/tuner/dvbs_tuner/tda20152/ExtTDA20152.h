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
#ifndef NXPTDA20152_h_h
	#define NXPTDA20152_h_h

#include "avl_dvbsx.h"
#include "ITuner.h"
#include "II2CRepeater.h"

//Following macro definition indicates the spectral sense of the tuner output signal.
//0- No signal inversion caused by the tuner, 1- Signal spectrum inverted by the tuner.
#define TUNER_SPECTRUM_INVERT 1

/*******************************************************************************************************/
/* Software version info                                                                               */
/*******************************************************************************************************/
#define TDA20152_VERSION_MAJOR   0
#define TDA20152_VERSION_RELEASE 0
#define TDA20152_VERSION_BUILD   14
#define TDA20152_VERSION_MINOR   3
#define tuner_slave_address 0xa8
#define tuner_I2Cbus_clock      200
#define tuner_LPF               340

/*******************************************************************************************************/
/* Hardware version info                                                                               */
/*******************************************************************************************************/
#define TDA20152_PRODUCT_NAME_STRING     ("BLACKWIDOW")
#define TDA20152_PRODUCT_VERSION_STRING   "0.0.14.3"

#define OSCILLATION_FREQUENCY_HZ       27000000


#ifndef TRUE
#define TRUE    1
#define FALSE   0
#endif

#ifndef True
#define True    TRUE
#define False   FALSE
#endif

/*******************************************************************************************************/
/* common-use types */
/*******************************************************************************************************/
#ifndef BOOL
typedef int BOOL;
#endif  /* BOOL */

typedef signed char     SAT_TUNER_INT8;
typedef unsigned char   SAT_TUNER_UINT8;
typedef signed short    SAT_TUNER_INT16;
typedef unsigned short  SAT_TUNER_UINT16;
typedef signed long     SAT_TUNER_INT32;
typedef unsigned long   SAT_TUNER_UINT32;

/* constants */
#define SAT_TUNER_MAX_VERSION_LENGTH          25   /* max length of driver-version string */

/* Dividers */
#define SAT_TUNER_REF_CLOCK_DIV_1             0
#define SAT_TUNER_REF_CLOCK_DIV_2             1
/* -- Powers (base 2) */
#define SAT_TUNER_2POW18                      (0x00040000)
#define SAT_TUNER_2POW31				      (0x80000000)
#define SAT_TUNER_2POW25				      (0x02000000)
#define SAT_TUNER_2POW23				      (0x00800000)
#define SAT_TUNER_2POW34_OVER_2POW3	          (0x80000000)
#define SAT_TUNER_2POW3                       (0x08)

/*******************************************************************************************************/
/* Function prototypes passed by STB software */
/*******************************************************************************************************/
typedef unsigned char   SAT_TUNER_ADDRESS;
typedef unsigned long*  SAT_TUNER_IOSTATUS;

/* type pointer to function to read byte from communication bus */
typedef unsigned char  (*SAT_TUNER_RD)(void*, SAT_TUNER_ADDRESS, SAT_TUNER_IOSTATUS);
/* type pointer to function to write byte to communication bus */
typedef void (*SAT_TUNER_WR)(void*, SAT_TUNER_ADDRESS, unsigned char, SAT_TUNER_IOSTATUS);
/* type pointer to function to write multiple-byte to communication bus */
typedef void (*SAT_TUNER_MULTI_WR)(void*, unsigned long, SAT_TUNER_ADDRESS, unsigned char*, SAT_TUNER_IOSTATUS);

/* Mask */
#define  SAT_TUNER_ADDR_MASK                  0x000000FF
#define  SAT_TUNER_START_BIT_MASK             0x00000F00
#define  SAT_TUNER_NUM_BYTES_MASK             0x00007000
#define  SAT_TUNER_NUM_BITS_MASK              0x003F0000
#define  SAT_TUNER_OPTIONS_MASK               0x0F000000
#define  SAT_TUNER_ENDIAN_MASK                0x01000000
#define  SAT_TUNER_READABLE_MASK              0x02000000
#define  SAT_TUNER_WRITEABLE_MASK             0x04000000
#define  SAT_TUNER_READWRITE_MASK             0x06000000
#define  SAT_TUNER_RMW_MASK                   0x08000000

/* Bit check */
#define  SAT_TUNER_REG_READABLE               0x01
#define  SAT_TUNER_REG_WRITEABLE              0x02

/* Bitfield positions */
#define  SAT_TUNER_MAP_RO                     SAT_TUNER_READABLE_MASK
#define  SAT_TUNER_MAP_WO                     SAT_TUNER_WRITEABLE_MASK
#define  SAT_TUNER_MAP_RW                     SAT_TUNER_READWRITE_MASK
#define  SAT_TUNER_MAP_RMW                    SAT_TUNER_RMW_MASK
#define  SAT_TUNER_NUM_BYTES_DEFAULT          0x00000000 /*  1 byte */
#define  SAT_TUNER_NUM_BYTES1                 0x00001000
#define  SAT_TUNER_NUM_BYTES2                 0x00002000
#define  SAT_TUNER_NUM_BYTES3                 0x00003000
#define  SAT_TUNER_NUM_BYTES4                 0x00004000

#define  SAT_TUNER_LITTLE_ENDIAN              0x00000000
#define  SAT_TUNER_BIG_ENDIAN                 0x01000000
#define  SAT_TUNER_BYTE_ORDER				SAT_TUNER_LITTLE_ENDIAN /* default byte order */

/* Bitfield shifts */
#define  SAT_TUNER_REGACCESS_SHIFT     (24)
#define  SAT_TUNER_BYTEORDER_SHIFT     (23)
#define  SAT_TUNER_NUM_BYTES_SHIFT     (12)
#define  SAT_TUNER_START_BIT_SHIFT     (8)
#define  SAT_TUNER_NUM_BITS_SHIFT      (16)

/* Offsets */
#define  SAT_TUNER_CHANNEL1_OFFSET			(0x00)
#define  SAT_TUNER_CHANNEL2_OFFSET			(0x20)

#define SAT_TUNER_CHANGE_BYTE_ORDER_LONG(long_value) \
{ \
	unsigned char* __p_byte = (unsigned char*)&long_value; \
	__p_byte[0] ^= __p_byte[3]; \
	__p_byte[3] ^= __p_byte[0]; \
	__p_byte[0] ^= __p_byte[3]; \
\
	__p_byte[1] ^= __p_byte[2]; \
	__p_byte[2] ^= __p_byte[1]; \
	__p_byte[1] ^= __p_byte[2];	\
}

#define SAT_TUNER_CHANGE_BYTE_ORDER_SHORT(short_value) \
{ \
	unsigned char* __p_byte = (unsigned char*)&short_value; \
	__p_byte[0] ^= __p_byte[1]; \
	__p_byte[1] ^= __p_byte[0]; \
	__p_byte[0] ^= __p_byte[1]; \
}

/*  -- Register map 32-bit encoding -- */
/*  Bit positions and meaning:
 *  [31	30	29	28	27	26	25	24]
 *   x	x	x	x	RMW	W	R	E
 *
 *  [23	22	21	20	19	18	17	16]
 *   x	x	Nb	Nb	Nb	Nb	Nb	Nb
 *
 *  [15	14	13	12	11	10	9	8]
 *   x	B	B	B	S	S	S	S
 *
 *  [7	6	5	4	3	2	1	0]
 *   A	A	A	A	A	A	A	A
 *
 *  A = address, S = start_bit, R = Readable, W = Writable,
 *  RMW = Read-Modify-Write, Nb = number_of_bits, x = reserved.
 *  B = number of units to read/write [0/1 = 1 byte, 2 = 2 bytes, 3 = 3 bytes and 4 = 4 bytes].
 *  E = Endianism; 0 = little (0x12345678), 1 = big (0x78563412)
 *
 */

/* bit-field extraction */
#define TDA20152_SAT_TUNER_GET_BYTE_ORDER(options)		   ((unsigned char)((options >> SAT_TUNER_BYTEORDER_SHIFT) & 0x0F))
#define TDA20152_SAT_TUNER_GET_ADDR(options)                ((unsigned short)(options  & SAT_TUNER_ADDR_MASK))
#define TDA20152_SAT_TUNER_GET_START_BIT(options)           ((unsigned char)((options >> SAT_TUNER_START_BIT_SHIFT) & 0x0F))
#define TDA20152_SAT_TUNER_GET_NUM_BIT(options)             ((unsigned char)((options >> SAT_TUNER_NUM_BITS_SHIFT)  & 0x3F))
#define TDA20152_SAT_TUNER_GET_RMW(options)                 ((BOOL) ( (options & SAT_TUNER_MAP_RMW) == (SAT_TUNER_MAP_RMW) ) )
#define TDA20152_SAT_TUNER_GET_WO(options)                  ((BOOL)((options & SAT_TUNER_MAP_WO) && !(options & SAT_TUNER_MAP_RO)))
#define TDA20152_SAT_TUNER_GET_RO(options)                  ((BOOL)((options & SAT_TUNER_MAP_RO)  && !(options & SAT_TUNER_MAP_WO)))
#define TDA20152_SAT_TUNER_GET_RW(options)                  ((BOOL)((options & SAT_TUNER_MAP_RW) == (SAT_TUNER_MAP_RW)))
#define TDA20152_SAT_TUNER_GET_REGACCESS(options)           ((unsigned char)((options >> SAT_TUNER_REGACCESS_SHIFT) & 0x07))
#define TDA20152_SAT_TUNER_GET_NUM_BYTES_TO_READ(options)   ((unsigned char)((options >> SAT_TUNER_NUM_BYTES_SHIFT) & 0x07))

/*******************************************************************************************************/
/* TDA20152_SAT_TUNER_COMMUNICATION */
/*******************************************************************************************************/
typedef struct TDA20152_sat_tuner_communication_
{
	void*              p_handle;
    SAT_TUNER_MULTI_WR multi_write;
    SAT_TUNER_WR       write;
    SAT_TUNER_RD       read;
}   TDA20152_SAT_TUNER_COMMUNICATION;

/* High level generic interfaces */
/* type pointer to function to read byte from tuner */
typedef BOOL (*TDA20152_SAT_TUNER_GENERIC_READ)(const TDA20152_SAT_TUNER_COMMUNICATION*, unsigned short, unsigned short, unsigned char, unsigned char, unsigned char, unsigned long*);
/* type pointer to function to write byte to to */
typedef BOOL (*TDA20152_SAT_TUNER_GENERIC_WRITE)(const TDA20152_SAT_TUNER_COMMUNICATION*, unsigned short, unsigned short, unsigned char, unsigned char, unsigned char, BOOL, unsigned char, unsigned long);

/*******************************************************************************************************/
/* SAT_TUNER_DRIVER_VERSION */
/*******************************************************************************************************/
typedef struct _tda20152_driver_version_
{
	char version_str[SAT_TUNER_MAX_VERSION_LENGTH];    /* string containing driver-specific version info */
}   TDA20152_SAT_TUNER_DRIVER_VERSION;


typedef enum _tda20152_sat_tuner_channel_
{
	TDA20152_SAT_TUNER_CHANNEL_A,
	TDA20152_SAT_TUNER_CHANNEL_B,
    TDA20152_SAT_TUNER_CHANNEL_NONE /* undefined */
}   TDA20152_SAT_TUNER_CHANNEL;

typedef enum _tda20152_sat_tuner_rolloff_
{   /* matched filter roll-off factors */
    TDA20152_SAT_TUNER_ROLLOFF_02  = 0,  /* roll-off factor is 0.2  */
    TDA20152_SAT_TUNER_ROLLOFF_025 = 1,  /* roll-off factor is 0.25 */
    TDA20152_SAT_TUNER_ROLLOFF_035 = 2   /* roll-off factor is 0.35 */
}   TDA20152_SAT_TUNER_ROLLOFF;
#if 0
typedef enum _tda20152_sat_tuner_chip_type_
{
	SAT_TUNER_CHIP_TYPE_CX24113A, /* Rattler */
	SAT_TUNER_CHIP_TYPE_CX24113AH, /* Rattler AH */
	SAT_TUNER_CHIP_TYPE_CX24118A, /* Mongoose */
	SAT_TUNER_CHIP_TYPE_CX24124,  /* Mongoose XT */
	SAT_TUNER_CHIP_TYPE_CX24128,  /* Viper   */
	SAT_TUNER_CHIP_TYPE_CX24132,  /* Spyder  */
	SAT_TUNER_CHIP_TYPE_CX24133,  /* Charger */
    SAT_TUNER_CHIP_TYPE_CX24134,   /* Recluse */
	SAT_TUNER_CHIP_TYPE_TDA20140   /* Black Widow */
}   TDA20152_SAT_TUNER_CHIP_TYPE;
#endif

/*******************************************************************************************************/
/* SAT_TUNER_CHANOBJ -- Channel object */
/*******************************************************************************************************/
typedef struct _tda20152_sat_tuner_chanobj_
{   /* Channel object for tuning the tuner */
	unsigned long      frequency_khz;             /* Tuner frequency in kHz             */
	TDA20152_SAT_TUNER_ROLLOFF  roll_off;                  /* Matched filter roll-off factor     */
	unsigned long      symbol_rate_ksps;          /* symbol rate (kSps)             */
	unsigned short     requested_search_range_khz;/* Requested freq search range in kHz */
    unsigned char      flags;                     /* Refer SAT_TUNER_CHANNEL_FLAGS      */
}   TDA20152_SAT_TUNER_CHANOBJ;

typedef struct _tda20152_preload_values_
{
	unsigned short band_select_frequency_value1;
	unsigned short band_select_frequency_value2;
	unsigned short band_select_divider_count1;
	unsigned short band_select_divider_count2;
} TDA20152_SAT_TUNER_PRELOAD_VALUES;

//bool
//sat_tuner_regwrite(const TDA20152_SAT_TUNER_COMMUNICATION* p_communication,
//				   unsigned short				  address,
//				   unsigned short				  offset,
//				   unsigned char				  start_bit,
//				   unsigned char				  num_bit,
//				   unsigned char				  num_bytes,
//				   bool							  rmw,
//				   unsigned char				  access,
//				   unsigned long				  data);
//
//bool
//sat_tuner_regread(const TDA20152_SAT_TUNER_COMMUNICATION* p_communication,
//                  unsigned short				 address,
//                  unsigned short				 offset,
//                  unsigned char					 start_bit,
//                  unsigned char					 num_bit,
//                  unsigned char					 num_bytes,
//                  unsigned long*				 p_data);

/* High-level Register read/write macros */
#define TDA20152_SAT_TUNER_READ(communication, hwreg, data)    SAT_TUNER_RegRead(communication, \
                                                                    TDA20152_SAT_TUNER_GET_ADDR(hwreg), \
                                                                    0, \
                                                                    TDA20152_SAT_TUNER_GET_START_BIT(hwreg), \
                                                                    TDA20152_SAT_TUNER_GET_NUM_BIT(hwreg), \
                                                                    TDA20152_SAT_TUNER_GET_NUM_BYTES_TO_READ(hwreg), \
                                                                    data)

#define TDA20152_SAT_TUNER_WRITE(communication, hwreg, value)  SAT_TUNER_RegWrite(communication, \
                                                                     TDA20152_SAT_TUNER_GET_ADDR(hwreg), \
                                                                     0, \
                                                                     TDA20152_SAT_TUNER_GET_START_BIT(hwreg), \
                                                                     TDA20152_SAT_TUNER_GET_NUM_BIT(hwreg), \
                                                                     TDA20152_SAT_TUNER_GET_NUM_BYTES_TO_READ(hwreg), \
                                                                     TDA20152_SAT_TUNER_GET_RMW(hwreg), \
                                                                     TDA20152_SAT_TUNER_GET_REGACCESS(hwreg), \
                                                                     value)

/* High-level Register read/write with offset macros - helps in looping */
#define TDA20152_SAT_TUNER_READ_OFFSET(communication, hwreg, offset, data)   sat_generic_rd(communication, \
                                                                                  TDA20152_SAT_TUNER_GET_ADDR(hwreg), \
                                                                                  offset, \
                                                                                  TDA20152_SAT_TUNER_GET_START_BIT(hwreg), \
                                                                                  TDA20152_SAT_TUNER_GET_NUM_BIT(hwreg), \
                                                                                  TDA20152_SAT_TUNER_GET_NUM_BYTES_TO_READ(hwreg), \
                                                                                  data)

#define TDA20152_SAT_TUNER_WRITE_OFFSET(communication, hwreg, offset, value)  sat_generic_wr(communication, \
                                                                                    TDA20152_SAT_TUNER_GET_ADDR(hwreg), \
                                                                                    offset, \
                                                                                    TDA20152_SAT_TUNER_GET_START_BIT(hwreg), \
                                                                                    TDA20152_SAT_TUNER_GET_NUM_BIT(hwreg), \
                                                                                    TDA20152_SAT_TUNER_GET_NUM_BYTES_TO_READ(hwreg), \
                                                                                    TDA20152_SAT_TUNER_GET_RMW(hwreg), \
                                                                                    TDA20152_SAT_TUNER_GET_REGACCESS(hwreg), \
                                                                                    value)




/*****************************************************************************/
/* DEFINES                                                                   */
/*****************************************************************************/
#define TDA20152_TM_LOCK_LO 2
#define TDA20152_TM_LOCK_LO_SPI 20
#define TDA20152_PLL_LOCK_DELAY_US 4000
#define TDA20152_ALC_T16_CHECK 2

#define TDA20152_BANDWIDTH_19MHZ 19000
#define TDA20152_BANDWIDTH_25MHZ 25000

/*****************************************************************************/
/* STRUCTURE DEFINITION                                                      */
/*****************************************************************************/

/******************************************************************************
 * ENABLE BLOCKS
 *      bBaseband
 *          baseband
 *      bDcOffset
 *          DC offset
 *      bRfMixer
 *          RF mixer
 *      bRfvga
 *          RF vga
 *      bChargePump
 *          charge pump
 *      bPowerDetect
 *          power detector
 *      bLna
 *          LNA
 *      bLpt
 *          LPT
 *      bVco
 *          VCO
 *      bBiasCoreEnable
 *          enable PLL BIAS core
 *      bEnableBothDividers
 *          enable PLL pre-divider and main-divider
 ******************************************************************************/
typedef struct _TDA20152_enable_blocks_
{
    BOOL bBaseband;
    BOOL bDcOffset;
    BOOL bRfMixer;
    BOOL bRfvga;
    BOOL bChargePump;
    BOOL bPowerDetect;
    BOOL bLna;
    BOOL bLpt;
    BOOL bVco;
    BOOL bBiasCoreEnable;
    BOOL bEnableBothDividers;
} TDA20152_ENABLE_BLOCKS;

/******************************************************************************
 * XTAL Automatic Level Control (ALC) nominal voltage level
 *      register values are:
 *          0.5  Vpp    0x00
 *          0.75 Vpp    0x01
 *          1.2  Vpp    0x02
 *          1.6  Vpp    0x03
 ******************************************************************************/
typedef enum _TDA20152_xtal_alc_lvl_cntl_
{
    TDA20152_XTAL_ALC_LVL_CNTL_0P5_VPP  = 0x00,
    TDA20152_XTAL_ALC_LVL_CNTL_0P75_VPP = 0x01,
    TDA20152_XTAL_ALC_LVL_CNTL_1P2_VPP  = 0x02,
    TDA20152_XTAL_ALC_LVL_CNTL_1P6_VPP  = 0x03,
    TDA20152_XTAL_ALC_LVL_CNTL_INVALID       /* boundary value. should never be reached*/
} TDA20152_XTAL_ALC_LVL_CNTL;

/******************************************************************************
 * XTAL REFERENCE DIVIDER
 *      division ratio for PLL reference clock
 *          divide by 1     0x00
 *          divide by 2     0x01
 *          divide by 3     0x02
 *          divide by 4     0x03
 ******************************************************************************/
typedef enum _TDA20152_xtal_ref_div_
{
    TDA20152_XTAL_REF_DIV_BY_1  = 0x00,
    TDA20152_XTAL_REF_DIV_BY_2  = 0x01,
    TDA20152_XTAL_REF_DIV_BY_3  = 0x02,
    TDA20152_XTAL_REF_DIV_BY_4  = 0x03,
    TDA20152_XTAL_REF_DIV_INVALID       /* boundary value. should never be reached*/
} TDA20152_XTAL_REF_DIV;

/******************************************************************************
 * XTAL ALC High pass to low frequency
 *     0 = XTAL ALC high pass ~ 1.6MHz (2 zeros)
 *     1 = XTAL ALC high pass ~ 500kHz (1 zero)
 ******************************************************************************/
typedef enum _TDA20152_xtal_hpf_low_freq_
{
    TDA20152_XTAL_HPF_LOW_FREQ_1_6_MHZ  = 0x00,
    TDA20152_XTAL_HPF_LOW_FREQ_500_KHZ  = 0x01,
    TDA20152_XTAL_HPF_LOW_FREQ_INVALID       /* boundary value. should never be reached*/
} TDA20152_XTAL_HPF_LOW_FREQ;


/******************************************************************************
 * XTAL EXTERNAL FILTER
 *      filter bandwidth for external clock
 *          No filter    0x00
 *          20 MHz       0x01
 *          40 MHz       0x02
 *          80 MHz       0x03
 ******************************************************************************/
typedef enum _TDA20152_xtal_ext_flt_
{
    TDA20152_XTAL_EXT_FLT_NONE  = 0x00,
    TDA20152_XTAL_EXT_FLT_20MHZ = 0x01,
    TDA20152_XTAL_EXT_FLT_40MHZ = 0x02,
    TDA20152_XTAL_EXT_FLT_80MHZ = 0x03,
    TDA20152_XTAL_EXT_FLT_INVALID       /* boundary value. should never be reached*/
} TDA20152_XTAL_EXT_FLT;

/******************************************************************************
 * XTAL EXTERNAL DIVIDER
 *      division ratio for external clock
 *          divide by 1     0x00
 *          divide by 2     0x01
 *          divide by 3     0x02
 *          divide by 4     0x03
 ******************************************************************************/
typedef enum _TDA20152_xtal_ext_div_
{
    TDA20152_XTAL_EXT_DIV_BY_1  = 0x00,
    TDA20152_XTAL_EXT_DIV_BY_2  = 0x01,
    TDA20152_XTAL_EXT_DIV_BY_3  = 0x02,
    TDA20152_XTAL_EXT_DIV_BY_4  = 0x03,
    TDA20152_XTAL_EXT_DIV_INVALID       /* boundary value. should never be reached*/
} TDA20152_XTAL_EXT_DIV;

/******************************************************************************
 * CRYSTAL setting structure
 * values does not refect registers
 *      bXtalOscEnable
 *          Enable Crystal oscillator
 *      bXtalStartupBoostEnable
 *          Enable start up boost
 *      bXtalAlcEnable
 *          Enable Auto Level Control
 *      eXtalAlcLevel
 *          Configure Auto Level Control
 *      bForceSquareWave
 *          in the XTAL Osc output to external output path
 *          do not use divider and filter
 *          note: divider and filter are not shut down
 *          note: if bExtClockEnable is disabled, no ext output
 *      bXtalExtInputEnable
 *          to be enable when the input is not a Xtal
 *          but a ref clk from another tuner
 *      bPllRefEnable
 *          Enable XTAL Osc output to PLL path
 *      eXtalRefDivider
 *          Configure divider between XTAL Osc output and PLL
 *      eXtalExtFilter
 *          Configure divider between XTAL Osc output and external output
 *      eXtalExtDivider
 *          Configure filter  between XTAL Osc output and external output
 *      bExtClockEnable
 *          Enable divider, filter and mux between XTAL Osc output and external output
 *      bExtClockOverride
 *          extDivider is driven by ExtPin when disabled
 *      eAlcHighPathToLowFreq
 *          specify high path filter
 ******************************************************************************/
typedef struct _TDA20152_xtal_settings_
{
    BOOL                          bXtalOscEnable;
    BOOL                          bXtalStartupBoostEnable;
    BOOL                          bXtalAlcEnable;
    TDA20152_XTAL_ALC_LVL_CNTL  eXtalAlcLevel;
    BOOL                          bForceSquareWave;
    BOOL                          bXtalExtInputEnable;
    BOOL                          bPllRefEnable;
    TDA20152_XTAL_REF_DIV       eXtalRefDivider;
    TDA20152_XTAL_EXT_FLT       eXtalExtFilter;
    TDA20152_XTAL_EXT_DIV       eXtalExtDivider;
    BOOL                          bExtClockEnable;
    BOOL                          bExtClockOverride;
    TDA20152_XTAL_HPF_LOW_FREQ  eAlcHighPathToLowFreq;
} TDA20152_XTAL_SETTINGS;

/******************************************************************************
 * LO configuration
 *   Block enabling
 *      bDoublerEnable
 *          Multiplier by 2
 *      bDcc1Enable;
 *          Duty cycle correction 1
 *      bDcc2Enable;
 *          Duty cycle correction 2
 *      bFinalDividerEnable;
 *          Final Divider by 2
 *      bInputBufEnable;
 *          buffer following LO buf 2:1 mux
 *      bMainDivEnable;
 *          Main Divider
 *      bPpfEnable;
 *          poly phase output
 *      bInputMuxEnable;
 *          Enable input mux (LO 2:1 MUX)
 *      bOutputMuxEnable;
 *          Mux "LO Test" path, "LO chain" path and "8 divider" path
 *
 *   LO chain configuration
 *      Main divider is spitted into 2 parts: Div1 and Div2
 *      bDiv1ConfigInDivideBy3
 *          Div1 have 2 configs: 2 or 3
 *      bDiv2ConfigInDivideBy3
 *          Div2 have 2 configs: 2 or 3
 *      bSelectDivideBy4Or5Or6Or7Path
 *          Enable path with the divider
 *      bSelectDivideBy8Path
 *          Enable path without the divider (used for divide by 8)
 ******************************************************************************/
typedef struct _TDA20152_lo_settings_
{
    BOOL bDoublerEnable;
    BOOL bDcc1Enable;
    BOOL bDcc2Enable;
    BOOL bFinalDividerEnable;
    BOOL bInputBufEnable;
    BOOL bMainDivEnable;
    BOOL bPpfEnable;
    BOOL bInputMuxEnable;
    BOOL bOutputMuxEnable;
    BOOL bDiv1ConfigInDivideBy3;
    BOOL bDiv2ConfigInDivideBy3;
    BOOL bSelectDivideBy4Or5Or6Or7Path;
    BOOL bSelectDivideBy8Path;
} TDA20152_LO_SETTINGS;

/******************************************************************************
 * V Power Detector amplitude
 *     in some case, this information is not available
 ******************************************************************************/
typedef enum _TDA20152_vpd_amplitude_
{
    TDA20152_VPD_AMPLITUDE_ABOVE_VREFH,
    TDA20152_VPD_AMPLITUDE_BELOW_VREFH,
    TDA20152_VPD_AMPLITUDE_NA
} TDA20152_VPD_AMPLITUDE;

/******************************************************************************
 * VCO
 *    indicate which VCO is selected (VCO A or VCO B)
 ******************************************************************************/
typedef enum _TDA20152_vco_
{
    TDA20152_VCO_A  = 0,
    TDA20152_VCO_B,
    TDA20152_VCO_INVALID
} TDA20152_VCO;

/******************************************************************************
 * VCO Status
 *  ( indicate also if manual mode is enable )
 *
 *      ulBiasValue
 *          Bias value
 *      bCalibrationDone
 *          Calibration finished
 *      AlcAmplitudeStable
 *          indicates amplitude change
 *      eVpdAmplitude
 *          V power detector amplitude
 *      bCalibrationOvertime
 *          indicates whether calibration has been done over 16 ticks
 *      bCalibrationHitLimit
 *          calibration hits counter limit
 *      VTuneAboveThreshold
 *          Vtune is higher then specified threshold at high side
 *      VTuneBelowThreshold
 *          Vtune is lower then specified threshold at high side
 *      eVcoEnabled
 *          which VCO is enabled
 *            | VCO B | VCO A |
 *          0 |       |   x   |
 *          1 |   x   |       |
 *      bVTuneMargin
 *          Vtune is in the right range to provide margin
 *
 *   manual configuration status
 *      BiasControlBySpiEnable
 *      VcoSelectionBySpi
 *      TuningMachineEnabledBySpi
 *      TuningMachineClockEnableBySpi
 ******************************************************************************/
typedef struct _TDA20152_vco_status_
{
    unsigned int              ulBiasValue;
    BOOL                      bCalibrationDone;
    BOOL                      bAlcAmplitudeStable;
    TDA20152_VPD_AMPLITUDE  eVpdAmplitude;
    BOOL                      bCalibrationOvertime;
    BOOL                      bCalibrationHitLimit;
    BOOL                      bVTuneAboveThreshold;
    BOOL                      bVTuneBelowThreshold;
    TDA20152_VCO            eVcoEnabled;
    BOOL                      bVTuneMargin;
    BOOL                      bBiasControlBySpiEnable;
    BOOL                      bVcoSelectionBySpi;
    BOOL                      bTuningMachineEnabledBySpi;
    BOOL                      bTuningMachineClockEnableBySpi;
} TDA20152_VCO_STATUS;

/******************************************************************************
 * PLL Predivider division ratio
 *      2 modes are available
 *          0x00  -> divide by 2
 *          0x01  -> divide by 4
 ******************************************************************************/
typedef enum _TDA20152_pll_prediv_ratio_
{
    TDA20152_PLL_PREDIV_RATIO_2 = 0x00,
    TDA20152_PLL_PREDIV_RATIO_4 = 0x01,
    TDA20152_PLL_PREDIV_RATIO_INVALID
} TDA20152_PLL_PREDIV_RATIO;

/******************************************************************************
 * Configuration of PLL dividers
 *      ePredividerRatio
 *          configure division ratio for PreDivider
 *          (divide by 2 or 4)
 *      ulDsmIntegerBits
 *          Integer bits to DSM for PLL frequency programming
 *      ulDsmFractionalBits
 *          Fractional bits for PLL frequency programming
 ******************************************************************************/
typedef struct _TDA20152_pll_divider_config_
{
    TDA20152_PLL_PREDIV_RATIO  ePredividerRatio;
    unsigned long                ulDsmIntegerBits;
    unsigned long                ulDsmFractionalBits;
} TDA20152_PLL_DIVIDER_CONFIG;

/******************************************************************************
 * PLL DSM mash mode
 *      DSM configuration (DSM number of stage)
 *          0x00   Mash111
 *          0x01   Mash11
 ******************************************************************************/
typedef enum _TDA20152_dsm_mash_mode_
{
    TDA20152_DSM_MASH_MODE_MASH111 = 0x00,
    TDA20152_DSM_MASH_MODE_MASH11  = 0x01,
    TDA20152_DSM_MASH_MODE_INVALID
} TDA20152_DSM_MASH_MODE;

/******************************************************************************
 * PLL Mode (Fractional or integer)
 *      Pll mode interger or fractionnal
 *          Fractionnal     0
 *          integer         1
 ******************************************************************************/
typedef enum _TDA20152_pll_mode_
{
    TDA20152_PLL_MODE_FRACTIONNAL  = 0x00,
    TDA20152_PLL_MODE_INTEGER      = 0x01,
    TDA20152_PLL_MODE_INVALID
} TDA20152_PLL_MODE;

/******************************************************************************
 *  POWER DETECTOR THRESHOLD VALUE
 *  those value should match with power detector interval
 *  in order to aloow easy comparisons
 *******************************************************************************/
typedef enum _TDA20152_pwr_detect_therm_thres_val_
{
    TDA20152_PWR_DETECT_THERM_THRES_VAL_45       = 0x000,
    TDA20152_PWR_DETECT_THERM_THRES_VAL_40       = 0x001,
    TDA20152_PWR_DETECT_THERM_THRES_VAL_35       = 0x003,
    TDA20152_PWR_DETECT_THERM_THRES_VAL_30       = 0x007,
    TDA20152_PWR_DETECT_THERM_THRES_VAL_25       = 0x00F,
    TDA20152_PWR_DETECT_THERM_THRES_VAL_20       = 0x01F,
    TDA20152_PWR_DETECT_THERM_THRES_VAL_15       = 0x03F,
    TDA20152_PWR_DETECT_THERM_THRES_VAL_10       = 0x07F,
    TDA20152_PWR_DETECT_THERM_THRES_VAL_5        = 0x0FF,
    TDA20152_PWR_DETECT_THERM_THRES_VAL_INVALID  = 0x1FF
} TDA20152_PWR_DETECT_THERM_THRES_VAL;


/******************************************************************************
 * Power detector threshold
 *      thresholds used to define power detector reagions
 *      Following describes how modes are defined
 *      (TH is threshold input value and Mode is output)
 *             TH3     TH2     TH1
 *        Mode3 | Mode2 | Mode1 | Mode0
 ******************************************************************************/
typedef struct
{
    TDA20152_PWR_DETECT_THERM_THRES_VAL  eThrehold1;
    TDA20152_PWR_DETECT_THERM_THRES_VAL  eThrehold2;
    TDA20152_PWR_DETECT_THERM_THRES_VAL  eThrehold3;
} TDA20152_PWR_DETECT;

/******************************************************************************
 * LNA POWER MODE
 *      concidering ICD
 *      lna_halfref_en  lna_lowc
 *         0               0           => M1 (47mA)
 *         0               1           => M2 (30mA)
 *         1               0           => M3 (25mA)
 *         1               1           => M4 (16mA)
 ******************************************************************************/
typedef enum _TDA20152_lna_power_more_
{
    TDA20152_LNA_POWER_MODE_1 = 0,
    TDA20152_LNA_POWER_MODE_2,
    TDA20152_LNA_POWER_MODE_3,
    TDA20152_LNA_POWER_MODE_4,
    TDA20152_LNA_POWER_MODE_INVALID                     /* boundary value. should never be reached*/
}   TDA20152_LNA_POWER_MODE;

/******************************************************************************
 * Gain low current config
 *   Low current - True indicates low current mode
 *      eLnaLowCurrent
 *          LNA low current
 *      bRfVgaLowCurrent
 *          RfVga low current
 *      bAmpOutLowCurrent
 *          Amp out low current
 *      bMixerLowCurrent
 *          Mixer low current
 ******************************************************************************/
typedef struct _TDA20152_gain_low_current_
{
    TDA20152_LNA_POWER_MODE  eLnaLowCurrent;
    BOOL                       bRfVgaLowCurrent;
    BOOL                       bAmpOutLowCurrent;
    BOOL                       bMixerLowCurrent;
} TDA20152_GAIN_LOW_CURRENT;

/******************************************************************************
 * LNA GAIN setting
 *    first four values are register values
 *    last value indicate that LNA is disable
 *
 * following values match with register values
 * except the last one
 ******************************************************************************/
typedef enum _TDA20152_lna_gain_
{
    TDA20152_LNA_GAIN_7dB           = 0x00,
    TDA20152_LNA_GAIN_10dB          = 0x01,
    TDA20152_LNA_GAIN_13dB          = 0x02,
    TDA20152_LNA_GAIN_18dB          = 0x03,
    TDA20152_LNA_GAIN_NEGATIVE_11dB = 0x04,
    TDA20152_LNA_GAIN_INVALID
} TDA20152_LNA_GAIN;

/******************************************************************************
 * LPT GAIN setting
 *    first four values are register values
 *    last value indicate that LNA is disable
 *
 * following values match with register values
 * except the last one
 ******************************************************************************/
typedef enum _TDA20152_lpt_gain_
{
    TDA20152_LPT_GAIN_NEGATIVE_8dB  = 0x00,
    TDA20152_LPT_GAIN_NEGATIVE_10dB = 0x01,
    TDA20152_LPT_GAIN_NEGATIVE_14dB = 0x02,
    TDA20152_LPT_GAIN_NEGATIVE_16dB = 0x03,
    TDA20152_LPT_GAIN_INVALID
} TDA20152_LPT_GAIN;

/******************************************************************************
 * RF VGA offset
 *   register values:
 *       [000] = Offset0
 *       [001] = Offset1
 *       [010] = Offset2
 *       [011] = Offset3
 *       [100] = Offset4
 *       [101] = Offset5
 *       [110] = Offset6
 *       [111] = Offset7
 ******************************************************************************/
typedef enum _TDA20152_rfvga_offset_
{
    TDA20152_RFVGA_OFFSET_0 = 0x00,
    TDA20152_RFVGA_OFFSET_1 = 0x01,
    TDA20152_RFVGA_OFFSET_2 = 0x02,
    TDA20152_RFVGA_OFFSET_3 = 0x03,
    TDA20152_RFVGA_OFFSET_4 = 0x04,
    TDA20152_RFVGA_OFFSET_5 = 0x05,
    TDA20152_RFVGA_OFFSET_6 = 0x06,
    TDA20152_RFVGA_OFFSET_7 = 0x07,
    TDA20152_RFVGA_OFFSET_INVALID
} TDA20152_RFVGA_OFFSET;

/******************************************************************************
 * VGA1 offset
 *  DOES NOT REFLECT register values:
 *       [000] = VGA1Off0
 *       [001] = VGA1Off4
 *       [010] = VGA1Off2
 *       [011] = VGA1Off6
 *       [100] = VGA1Off1
 *       [101] = VGA1Off5
 *       [110] = VGA1Off3
 *       [111] = VGA1Off7
 ******************************************************************************/
typedef enum _TDA20152_vga1_offset_
{
    TDA20152_VGA1_OFFSET_0 = 0x00,
    TDA20152_VGA1_OFFSET_1 = 0x01,
    TDA20152_VGA1_OFFSET_2 = 0x02,
    TDA20152_VGA1_OFFSET_3 = 0x03,
    TDA20152_VGA1_OFFSET_4 = 0x04,
    TDA20152_VGA1_OFFSET_5 = 0x05,
    TDA20152_VGA1_OFFSET_6 = 0x06,
    TDA20152_VGA1_OFFSET_7 = 0x07,
    TDA20152_VGA1_OFFSET_INVALID
} TDA20152_VGA1_OFFSET;

/******************************************************************************
 * VGA2 offset
 *   DOES NOT REFLECT register values:
 *       [000] = VGA2Off0
 *       [001] = VGA2Off4
 *       [010] = VGA2Off2
 *       [011] = VGA2Off6
 *       [100] = VGA2Off1
 *       [101] = VGA2Off5
 *       [110] = VGA2Off3
 *       [111] = VGA2Off7
 ******************************************************************************/
typedef enum _TDA20152_vga2_offset_
{
    TDA20152_VGA2_OFFSET_0 = 0x00,
    TDA20152_VGA2_OFFSET_1 = 0x01,
    TDA20152_VGA2_OFFSET_2 = 0x02,
    TDA20152_VGA2_OFFSET_3 = 0x03,
    TDA20152_VGA2_OFFSET_4 = 0x04,
    TDA20152_VGA2_OFFSET_5 = 0x05,
    TDA20152_VGA2_OFFSET_6 = 0x06,
    TDA20152_VGA2_OFFSET_7 = 0x07,
    TDA20152_VGA2_OFFSET_INVALID
} TDA20152_VGA2_OFFSET;

/******************************************************************************
 * AMPOUT gain
 *   register values for discrete gain control
 *       0000 = 12dB
 *       0001 = 18dB
 *       0010 = 18dB (same than previous)
 *       0011 = 21dB
 *       0110 = 24dB
 *       0111 = 27dB
 *       1110 = 30dB
 *       1111 = 33dB
 ******************************************************************************/
typedef enum _TDA20152_amp_out_
{
    TDA20152_AMPOUT_15DB = 0x00,
    TDA20152_AMPOUT_18DB = 0x01,
    TDA20152_AMPOUT_18DB_b = 0x02,
    TDA20152_AMPOUT_21DB = 0x03,
    TDA20152_AMPOUT_24DB = 0x06,
    TDA20152_AMPOUT_27DB = 0x07,
    TDA20152_AMPOUT_30DB = 0x0E,
    TDA20152_AMPOUT_33DB = 0x0F,
    TDA20152_AMPOUT_INVALID
} TDA20152_AMPOUT;

/******************************************************************************
 * Gain configuration
 *      eLnaGain
 *          LNA gain value
 *      eLptGain
 *          loop through output gain
 *      eRfvgaOffset
 *          RF VGA offset settings
 *      eVga1Offset
 *          VGA1 offset settings
 *      eVga2Offset
 *          VGA2 offset settings
 *      eAmpOut
 *          AMPOUT gain
 *      eLnaLowCurrent;
 *          LNA low current
 *      bRfVgaLowCurrent;
 *          RfVga low current
 *      bAmpOutLowCurrent;
 *          Amp out low current
 ******************************************************************************/
typedef struct _TDA20152_gain_
{
    TDA20152_LNA_GAIN      eLnaGain;
    TDA20152_LPT_GAIN      eLptGain;
    TDA20152_RFVGA_OFFSET  eRfvgaOffset;
    TDA20152_VGA1_OFFSET   eVga1Offset;
    TDA20152_VGA2_OFFSET   eVga2Offset;
    TDA20152_AMPOUT        eAmpOut;
} TDA20152_GAIN;

/******************************************************************************
 * Gain Mode settings
 * defines gain settings for one mode
 ******************************************************************************/
typedef struct _TDA20152_gain_mode_settings_
{
    TDA20152_GAIN_LOW_CURRENT tGainLowCurrent;
    TDA20152_GAIN             tGainModeConfiguration;
} TDA20152_GAIN_MODE_SETTINGS;

/******************************************************************************
 * Gain Mode Config
 * defines gain configuration for each mode
 ******************************************************************************/
typedef struct _TDA20152_gain_mode_config_
{
    TDA20152_GAIN_MODE_SETTINGS tMode0;
    TDA20152_GAIN_MODE_SETTINGS tMode1;
    TDA20152_GAIN_MODE_SETTINGS tMode2;
    TDA20152_GAIN_MODE_SETTINGS tMode3;
} TDA20152_GAIN_MODE_CONFIG;

typedef struct _TDA20152_context_
{
    unsigned long               ulXtalFreqHz;

    /* threshold configuration */
    TDA20152_PWR_DETECT       tPwrDetectThreshold;
    /* Gain mode configuration */
    TDA20152_GAIN_MODE_CONFIG tGainModeConfig;

#ifdef FOR_DEBUG_PURPOSE
    unsigned long ulLoFreqkHz;
    unsigned long ulVcoFreqkHz;
    unsigned long ulNFloat_;
    unsigned long DsmIntInReg;
    unsigned long DsmIntInRegCheck;
    unsigned long DsmFracInReg;
    unsigned long DsmFracInRegCheck;
    unsigned long ulR;
    unsigned long ulPremain;

    unsigned long ulTiming;
#endif

} TDA20152_CONTEXT;


/******************************************************************************
 * GmC filter Bandwidth
 *      following are register settings
 *       BW          CTRL value (decimal)
 *       2.3MHz      19
 *       3.4MHz      24
 *       4.4MHz      34
 *       5.3MHz      37
 *       6.3MHz      49
 *       7MHz        51
 *       8.4MHz      65
 *       9.4MHz      67
 *       10.4MHz     81
 *       11MHz       82
 *       11.8MHz     96
 *       13.2MHz     98
 *       13.8MHz     112
 *       15.3MHz     114
 *       16.3MHz     115
 *       17.3MHz     116
 *       18.2MHz     61
 *       18.5MHz     117
 *       19.9MHz     118
 *       20.5MHz     90
 *       21.5MHz     119
 *       23.4MHz     91
 *       24.4MHz     106
 *       25.6MHz     121
 *       27.4MHz     107
 *       28.4MHz     122
 *       29.1MHz     78
 *       30.3MHz     93
 *       31.1MHz     108
 *       31.8MHz     123
 *       36.1MHz     124
 *       41.9MHz     125
 *       43.1MHz     110
 *       44.8MHz     95
 *       49.9MHz     126
 *       53.5MHz     111
 *       62MHz       127
 ******************************************************************************/
typedef enum _TDA20152_gmc_bw_
{
    TDA20152_GMC_BW_2P3MHZ            = 0x13,
    TDA20152_GMC_BW_3P4MHZ            = 0x18,
    TDA20152_GMC_BW_4P4MHZ            = 0x22,
    TDA20152_GMC_BW_5P3MHZ            = 0x25,
    TDA20152_GMC_BW_6P3MHZ            = 0x31,
    TDA20152_GMC_BW_7MHZ              = 0x33,
    TDA20152_GMC_BW_8P4MHZ            = 0x41,
    TDA20152_GMC_BW_9P4MHZ            = 0x43,
    TDA20152_GMC_BW_10P4MHZ           = 0x51,
    TDA20152_GMC_BW_11MHZ             = 0x52,
    TDA20152_GMC_BW_11P8MHZ           = 0x60,
    TDA20152_GMC_BW_13P2MHZ           = 0x62,
    TDA20152_GMC_BW_13P8MHZ           = 0x70,
    TDA20152_GMC_BW_15P3MHZ           = 0x72,
    TDA20152_GMC_BW_16P3MHZ           = 0x73,
    TDA20152_GMC_BW_17P3MHZ           = 0x74,
    TDA20152_GMC_BW_18P2MHZ           = 0x3D,
    TDA20152_GMC_BW_18P5MHZ           = 0x75,
    TDA20152_GMC_BW_19P9MHZ           = 0x76,
    TDA20152_GMC_BW_20P5MHZ           = 0x5A,
    TDA20152_GMC_BW_21P5MHZ           = 0x77,
    TDA20152_GMC_BW_23P4MHZ           = 0x5B,
    TDA20152_GMC_BW_24P4MHZ           = 0x6A,
    TDA20152_GMC_BW_25P6MHZ           = 0x79,
    TDA20152_GMC_BW_27P4MHZ           = 0x6B,
    TDA20152_GMC_BW_28P4MHZ           = 0x7A,
    TDA20152_GMC_BW_29P1MHZ           = 0x4E,
    TDA20152_GMC_BW_30P3MHZ           = 0x5D,
    TDA20152_GMC_BW_31P1MHZ           = 0x6C,
    TDA20152_GMC_BW_31P8MHZ           = 0x7B,
    TDA20152_GMC_BW_36P1MHZ           = 0x7C,
    TDA20152_GMC_BW_41P9MHZ           = 0x7D,
    TDA20152_GMC_BW_43P1MHZ           = 0x6E,
    TDA20152_GMC_BW_44P8MHZ           = 0x5F,
    TDA20152_GMC_BW_49P9MHZ           = 0x7E,
    TDA20152_GMC_BW_53P5MHZ           = 0x6F,
    TDA20152_GMC_BW_62MHZ             = 0x7F
} TDA20152_GMC_BW;

/******************************************************************************
 * Power Mode
 *      High power
 *      Low power
 *      Sleep (every thing is shut down but the Xtal Oscillator)
 *      Power down
 *      Manual (only for get function)
 ******************************************************************************/
typedef enum _TDA20152_power_mode_
{
    TDA20152_POWER_MODE_HIGH_POWER = 0,
    TDA20152_POWER_MODE_LOW_POWER,
    TDA20152_POWER_MODE_SLEEP,
    TDA20152_POWER_MODE_POWER_DOWN,
    TDA20152_POWER_MODE_MANUAL,
    TDA20152_POWER_MODE_INVALID,
} TDA20152_POWER_MODE;

#define TDA20152_GMC_BANDWIDTH_TABLE_MAX 37

typedef struct _TDA20152_gmc_bandwidth_table_
{
    unsigned long     ulBandwidthKHz;
    TDA20152_GMC_BW eGmcBandwidth;
} TDA20152_GMC_BANDWIDTH_TABLE;

/******************************************************************************
 * DSM FRAC
 *      0x00  ->  0.25<k<0.75
 *      0x01  ->  0<k<1
 ******************************************************************************/
typedef enum _TDA20152_dsm_frac_
{
    TDA20152_DSM_FRAC_0P25_TO_0P75  = 0x00,
    TDA20152_DSM_FRAC_0_TO_1        = 0x01,
    TDA20152_DSM_FRAC_INVALID
} TDA20152_DSM_FRAC;


/*****************************************************************************/
/* DEFINES                                                                   */
/*****************************************************************************/
#define TDA20152_MAX_TUNERS  1 /* was 2 for Recluse */

/* define to acces enable registers */
#define TDA20152_DISABLE 0
#define TDA20152_ENABLE  1

#define TDA20152_SPI_FREQ_KHZ_MIN 100
#define TDA20152_SPI_FREQ_KHZ_MAX 2800
#define TDA20152_SPI_FREQ_KHZ_STEP 100

#define START_BIT(x) (x)<<8
#define NB_UNIT(x)   (x)<<12
#define NB_BITS(x)   (x)<<16

#define TDA20152_SPARE_REG3_CCAL_VTH_MAX        31
#define TDA20152_VCO_TM1_TM_DELAY_S_MAX         3
#define TDA20152_VCO_ALC2_ALC_VTH_MAX           3
#define TDA20152_VCO_CAL_CLK_CLKGEN_SPDSEL_MAX  3
#define TDA20152_VCO_MUX_VCO_TMUX_S_MAX         0x1F
#define TDA20152_PLL1_DSM_INT_IN_MAX            0xFF
#define TDA20152_PLL2_DSM_FRAC_IN_MAX           0xFFFF

/* Xtal frequency range */
#define TDA20152_XTAL_FREQ_MIN_HZ 10000000 /* 10 Mhz */
#define TDA20152_XTAL_FREQ_MAX_HZ 60000000 /* 60 Mhz */

/* LO frequency range */
#define TDA20152_LO_FREQ_MIN_KHZ 300000    /* 300 Mhz */
#define TDA20152_LO_FREQ_MAX_KHZ 3000000   /* 3000 Mhz */

/* indicate the method used to calculate frequency */
/* by default BCD */
/*            UInt32 - when a flag is set */
/*            Double - when a flag is set */


//#define FOR_DEBUG_PURPOSE
//#define UINT32_CALCULATION
//#define DOUBLE_CALCULATION

#define TDA20152_LO_DIV_BY_5_LO_MAX_KHZ 1720000
#define TDA20152_LO_DIV_BY_6_LO_MAX_KHZ 1433000
#define TDA20152_LO_DIV_BY_7_LO_MAX_KHZ 1228000
#define TDA20152_LO_DIV_BY_8_LO_MAX_KHZ 1075000

/* N Float range defintion */
/* depending on DSM order and fractional correction */
#define TDA20152_N_FLOAT_BY_10_MIN_DSM_MASH11_FRAC 1305
#define TDA20152_N_FLOAT_BY_10_MAX_DSM_MASH11_FRAC 2525

#define TDA20152_N_FLOAT_BY_10_MIN_DSM_MASH11_INT 1290
#define TDA20152_N_FLOAT_BY_10_MAX_DSM_MASH11_INT 2530

#define TDA20152_N_FLOAT_BY_10_MIN_DSM_MASH111_FRAC 1345
#define TDA20152_N_FLOAT_BY_10_MAX_DSM_MASH111_FRAC 2485

#define TDA20152_N_FLOAT_BY_10_MIN_DSM_MASH111_INT 1310
#define TDA20152_N_FLOAT_BY_10_MAX_DSM_MASH111_INT 2510

/* default values after initialisation */
#define TDA20152_PWR_DETECT_THRESHOLD_1_DEFAULT TDA20152_PWR_DETECT_THERM_THRES_VAL_45
#define TDA20152_PWR_DETECT_THRESHOLD_2_DEFAULT TDA20152_PWR_DETECT_THERM_THRES_VAL_30
#define TDA20152_PWR_DETECT_THRESHOLD_3_DEFAULT TDA20152_PWR_DETECT_THERM_THRES_VAL_15

#define TDA20152_GAIN_MODE_0_LOW_CURRENT_AMP_OUT_DEFAULT   False
#define TDA20152_GAIN_MODE_0_LOW_CURRENT_MIXER_DEFAULT     False
#define TDA20152_GAIN_MODE_0_LOW_CURRENT_RF_VGA_DEFAULT    False
#define TDA20152_GAIN_MODE_0_LOW_CURRENT_LNA_DEFAULT       TDA20152_LNA_POWER_MODE_1

#define TDA20152_GAIN_MODE_0_AMP_OUT_DEFAULT               TDA20152_AMPOUT_21DB
#define TDA20152_GAIN_MODE_0_LNA_DEFAULT                   TDA20152_LNA_GAIN_18dB
#define TDA20152_GAIN_MODE_0_LPT_DEFAULT                   TDA20152_LPT_GAIN_NEGATIVE_8dB
#define TDA20152_GAIN_MODE_0_RF_VGA_DEFAULT                TDA20152_RFVGA_OFFSET_5
#define TDA20152_GAIN_MODE_0_VGA1_DEFAULT                  TDA20152_VGA1_OFFSET_4
#define TDA20152_GAIN_MODE_0_VGA2_DEFAULT                  TDA20152_VGA2_OFFSET_3

#define TDA20152_GAIN_MODE_1_LOW_CURRENT_AMP_OUT_DEFAULT   False
#define TDA20152_GAIN_MODE_1_LOW_CURRENT_MIXER_DEFAULT     False
#define TDA20152_GAIN_MODE_1_LOW_CURRENT_RF_VGA_DEFAULT    False
#define TDA20152_GAIN_MODE_1_LOW_CURRENT_LNA_DEFAULT       TDA20152_LNA_POWER_MODE_1

#define TDA20152_GAIN_MODE_1_AMP_OUT_DEFAULT               TDA20152_AMPOUT_27DB
#define TDA20152_GAIN_MODE_1_LNA_DEFAULT                   TDA20152_LNA_GAIN_13dB
#define TDA20152_GAIN_MODE_1_LPT_DEFAULT                   TDA20152_LPT_GAIN_NEGATIVE_8dB
#define TDA20152_GAIN_MODE_1_RF_VGA_DEFAULT                TDA20152_RFVGA_OFFSET_5
#define TDA20152_GAIN_MODE_1_VGA1_DEFAULT                  TDA20152_VGA1_OFFSET_4
#define TDA20152_GAIN_MODE_1_VGA2_DEFAULT                  TDA20152_VGA2_OFFSET_3

#define TDA20152_GAIN_MODE_2_LOW_CURRENT_AMP_OUT_DEFAULT   False
#define TDA20152_GAIN_MODE_2_LOW_CURRENT_MIXER_DEFAULT     False
#define TDA20152_GAIN_MODE_2_LOW_CURRENT_RF_VGA_DEFAULT    False
#define TDA20152_GAIN_MODE_2_LOW_CURRENT_LNA_DEFAULT       TDA20152_LNA_POWER_MODE_1

#define TDA20152_GAIN_MODE_2_AMP_OUT_DEFAULT               TDA20152_AMPOUT_33DB
#define TDA20152_GAIN_MODE_2_LNA_DEFAULT                   TDA20152_LNA_GAIN_7dB
#define TDA20152_GAIN_MODE_2_LPT_DEFAULT                   TDA20152_LPT_GAIN_NEGATIVE_8dB
#define TDA20152_GAIN_MODE_2_RF_VGA_DEFAULT                TDA20152_RFVGA_OFFSET_5
#define TDA20152_GAIN_MODE_2_VGA1_DEFAULT                  TDA20152_VGA1_OFFSET_4
#define TDA20152_GAIN_MODE_2_VGA2_DEFAULT                  TDA20152_VGA2_OFFSET_3

#define TDA20152_GAIN_MODE_3_LOW_CURRENT_AMP_OUT_DEFAULT   False
#define TDA20152_GAIN_MODE_3_LOW_CURRENT_MIXER_DEFAULT     False
#define TDA20152_GAIN_MODE_3_LOW_CURRENT_RF_VGA_DEFAULT    False
#define TDA20152_GAIN_MODE_3_LOW_CURRENT_LNA_DEFAULT       TDA20152_LNA_POWER_MODE_1

#define TDA20152_GAIN_MODE_3_AMP_OUT_DEFAULT               TDA20152_AMPOUT_24DB
#define TDA20152_GAIN_MODE_3_LNA_DEFAULT                   TDA20152_LNA_GAIN_NEGATIVE_11dB
#define TDA20152_GAIN_MODE_3_LPT_DEFAULT                   TDA20152_LPT_GAIN_NEGATIVE_8dB
#define TDA20152_GAIN_MODE_3_RF_VGA_DEFAULT                TDA20152_RFVGA_OFFSET_5
#define TDA20152_GAIN_MODE_3_VGA1_DEFAULT                  TDA20152_VGA1_OFFSET_4
#define TDA20152_GAIN_MODE_3_VGA2_DEFAULT                  TDA20152_VGA2_OFFSET_3


/**********************************************************************************
 * Blackwidow tuner register addresses
 *    Automatically generated in regmap_08_rename_0.9.xls
 **********************************************************************************/
#define TDA20152_CHIP_ID_REG 0x00
#define TDA20152_CHIP_VER_REG 0x01
#define TDA20152_XTAL1_REG 0x02
#define TDA20152_XTAL2_REG 0x03
#define TDA20152_TEST_MUX_SEL_REG 0x04
#define TDA20152_SPI_CTRL_REG 0x05
#define TDA20152_FE_REG 0x06
#define TDA20152_ENABLE1_REG 0x07
#define TDA20152_SPARE_REG1_REG 0x08
#define TDA20152_VGA_REG 0x09
#define TDA20152_BB1_REG 0x0A
#define TDA20152_BB2_REG 0x0B
#define TDA20152_BB3_REG 0x0C
#define TDA20152_PWR_DET_REG 0x0D
#define TDA20152_PD_THERM_REG 0x0E
#define TDA20152_VCO_ALC1_REG 0x0F
#define TDA20152_VCO_ALC2_REG 0x10
#define TDA20152_VCO_ALC_OUT_REG 0x11
#define TDA20152_VCO_CAL_CLK_REG 0x12
#define TDA20152_VCO_TM1_REG 0x13
#define TDA20152_VCO_TM2_REG 0x14
#define TDA20152_VCO_TM_OUT_REG 0x15
#define TDA20152_VCO_MUX_REG 0x16
#define TDA20152_SPARE_REG3_REG 0x17
#define TDA20152_PFD_CP1_REG 0x18
#define TDA20152_PFD_CP2_REG 0x19
#define TDA20152_PLL_EN_REG 0x1A
#define TDA20152_PLL_TEST1_REG 0x1B
#define TDA20152_PLL_TEST2_REG 0x1C
#define TDA20152_PLL_TEST3_REG 0x1D
#define TDA20152_PLL1_REG 0x1E
#define TDA20152_PLL2_REG 0x1F
#define TDA20152_PLL3_REG 0x20
#define TDA20152_PLL4_REG 0x21
#define TDA20152_LO_CHAIN1_REG 0x22
#define TDA20152_LO_CHAIN2_REG 0x23
#define TDA20152_LO_CHAIN3_REG 0x24
#define TDA20152_LO_CHAIN4_REG 0x25
#define TDA20152_LO_TEST_REG 0x26
#define TDA20152_DIG_LDO_REG 0x27

#define TDA20152_CHIP_ID                         (TDA20152_CHIP_ID_REG | START_BIT(0) | NB_UNIT(0) | NB_BITS(8) | SAT_TUNER_MAP_RO )
#define TDA20152_CHIP_VER                        (TDA20152_CHIP_VER_REG | START_BIT(0) | NB_UNIT(0) | NB_BITS(8) | SAT_TUNER_MAP_RO )
#define TDA20152_XTAL1                           (TDA20152_XTAL1_REG | START_BIT(0) | NB_UNIT(0) | NB_BITS(8) | SAT_TUNER_MAP_RW )
#define TDA20152_XTAL2                           (TDA20152_XTAL2_REG | START_BIT(0) | NB_UNIT(0) | NB_BITS(8) | SAT_TUNER_MAP_RW )
#define TDA20152_TEST_MUX_SEL                    (TDA20152_TEST_MUX_SEL_REG | START_BIT(0) | NB_UNIT(0) | NB_BITS(8) | SAT_TUNER_MAP_RW )
#define TDA20152_SPI_CTRL                        (TDA20152_SPI_CTRL_REG | START_BIT(0) | NB_UNIT(0) | NB_BITS(8) | SAT_TUNER_MAP_RW )
#define TDA20152_FE                              (TDA20152_FE_REG | START_BIT(0) | NB_UNIT(0) | NB_BITS(8) | SAT_TUNER_MAP_RW )
#define TDA20152_ENABLE1                         (TDA20152_ENABLE1_REG | START_BIT(0) | NB_UNIT(0) | NB_BITS(8) | SAT_TUNER_MAP_RW )
#define TDA20152_SPARE_REG1                      (TDA20152_SPARE_REG1_REG | START_BIT(0) | NB_UNIT(0) | NB_BITS(8) | SAT_TUNER_MAP_RW )
#define TDA20152_VGA                             (TDA20152_VGA_REG | START_BIT(0) | NB_UNIT(0) | NB_BITS(8) | SAT_TUNER_MAP_RW )
#define TDA20152_BB1                             (TDA20152_BB1_REG | START_BIT(0) | NB_UNIT(0) | NB_BITS(8) | SAT_TUNER_MAP_RW )
#define TDA20152_BB2                             (TDA20152_BB2_REG | START_BIT(0) | NB_UNIT(0) | NB_BITS(8) | SAT_TUNER_MAP_RW )
#define TDA20152_BB3                             (TDA20152_BB3_REG | START_BIT(0) | NB_UNIT(0) | NB_BITS(8) | SAT_TUNER_MAP_RW )
#define TDA20152_PWR_DET                         (TDA20152_PWR_DET_REG | START_BIT(0) | NB_UNIT(0) | NB_BITS(8) | SAT_TUNER_MAP_RW )
#define TDA20152_PD_THERM                        (TDA20152_PD_THERM_REG | START_BIT(0) | NB_UNIT(0) | NB_BITS(8) | SAT_TUNER_MAP_RW )
#define TDA20152_VCO_ALC1                        (TDA20152_VCO_ALC1_REG | START_BIT(0) | NB_UNIT(0) | NB_BITS(8) | SAT_TUNER_MAP_RW )
#define TDA20152_VCO_ALC2                        (TDA20152_VCO_ALC2_REG | START_BIT(0) | NB_UNIT(0) | NB_BITS(8) | SAT_TUNER_MAP_RW )
#define TDA20152_VCO_ALC_OUT                     (TDA20152_VCO_ALC_OUT_REG | START_BIT(0) | NB_UNIT(0) | NB_BITS(8) | SAT_TUNER_MAP_RW )
#define TDA20152_VCO_CAL_CLK                     (TDA20152_VCO_CAL_CLK_REG | START_BIT(0) | NB_UNIT(0) | NB_BITS(8) | SAT_TUNER_MAP_RW )
#define TDA20152_VCO_TM1                         (TDA20152_VCO_TM1_REG | START_BIT(0) | NB_UNIT(0) | NB_BITS(8) | SAT_TUNER_MAP_RW )
#define TDA20152_VCO_TM2                         (TDA20152_VCO_TM2_REG | START_BIT(0) | NB_UNIT(0) | NB_BITS(8) | SAT_TUNER_MAP_RW )
#define TDA20152_VCO_TM_OUT                      (TDA20152_VCO_TM_OUT_REG | START_BIT(0) | NB_UNIT(0) | NB_BITS(8) | SAT_TUNER_MAP_RW )
#define TDA20152_VCO_MUX                         (TDA20152_VCO_MUX_REG | START_BIT(0) | NB_UNIT(0) | NB_BITS(8) | SAT_TUNER_MAP_RW )
#define TDA20152_SPARE_REG3                      (TDA20152_SPARE_REG3_REG | START_BIT(0) | NB_UNIT(0) | NB_BITS(8) | SAT_TUNER_MAP_RW )
#define TDA20152_PFD_CP1                         (TDA20152_PFD_CP1_REG | START_BIT(0) | NB_UNIT(0) | NB_BITS(8) | SAT_TUNER_MAP_RW )
#define TDA20152_PFD_CP2                         (TDA20152_PFD_CP2_REG | START_BIT(0) | NB_UNIT(0) | NB_BITS(8) | SAT_TUNER_MAP_RW )
#define TDA20152_PLL_EN                          (TDA20152_PLL_EN_REG | START_BIT(0) | NB_UNIT(0) | NB_BITS(8) | SAT_TUNER_MAP_RW )
#define TDA20152_PLL_TEST1                       (TDA20152_PLL_TEST1_REG | START_BIT(0) | NB_UNIT(0) | NB_BITS(8) | SAT_TUNER_MAP_RW )
#define TDA20152_PLL_TEST2                       (TDA20152_PLL_TEST2_REG | START_BIT(0) | NB_UNIT(0) | NB_BITS(8) | SAT_TUNER_MAP_RW )
#define TDA20152_PLL_TEST3                       (TDA20152_PLL_TEST3_REG | START_BIT(0) | NB_UNIT(0) | NB_BITS(8) | SAT_TUNER_MAP_RW )
#define TDA20152_PLL1                            (TDA20152_PLL1_REG | START_BIT(0) | NB_UNIT(0) | NB_BITS(8) | SAT_TUNER_MAP_RW )
#define TDA20152_PLL2                            (TDA20152_PLL2_REG | START_BIT(0) | NB_UNIT(0) | NB_BITS(8) | SAT_TUNER_MAP_RW )
#define TDA20152_PLL3                            (TDA20152_PLL3_REG | START_BIT(0) | NB_UNIT(0) | NB_BITS(8) | SAT_TUNER_MAP_RW )
#define TDA20152_PLL4                            (TDA20152_PLL4_REG | START_BIT(0) | NB_UNIT(0) | NB_BITS(8) | SAT_TUNER_MAP_RW )
#define TDA20152_LO_CHAIN1                       (TDA20152_LO_CHAIN1_REG | START_BIT(0) | NB_UNIT(0) | NB_BITS(8) | SAT_TUNER_MAP_RW )
#define TDA20152_LO_CHAIN2                       (TDA20152_LO_CHAIN2_REG | START_BIT(0) | NB_UNIT(0) | NB_BITS(8) | SAT_TUNER_MAP_RW )
#define TDA20152_LO_CHAIN3                       (TDA20152_LO_CHAIN3_REG | START_BIT(0) | NB_UNIT(0) | NB_BITS(8) | SAT_TUNER_MAP_RW )
#define TDA20152_LO_CHAIN4                       (TDA20152_LO_CHAIN4_REG | START_BIT(0) | NB_UNIT(0) | NB_BITS(8) | SAT_TUNER_MAP_RW )
#define TDA20152_LO_TEST                         (TDA20152_LO_TEST_REG | START_BIT(0) | NB_UNIT(0) | NB_BITS(8) | SAT_TUNER_MAP_RW )
#define TDA20152_DIG_LDO                         (TDA20152_DIG_LDO_REG | START_BIT(0) | NB_UNIT(0) | NB_BITS(8) | SAT_TUNER_MAP_RW )

#define TDA20152_XTAL1_XTAL_EN                   (TDA20152_XTAL1_REG | START_BIT(7) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_XTAL1_XTAL_SU_BOOST_DIS         (TDA20152_XTAL1_REG | START_BIT(6) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RW )
#define TDA20152_XTAL1_XTAL_ALC_DIS              (TDA20152_XTAL1_REG | START_BIT(5) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_XTAL1_XTAL_ALC_LVL_CTRL         (TDA20152_XTAL1_REG | START_BIT(3) | NB_UNIT(0) | NB_BITS(2) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_XTAL1_XTAL_EXT_SQUARWAVE_FORCE  (TDA20152_XTAL1_REG | START_BIT(2) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_XTAL1_XTAL_EXT_INPUT_EN         (TDA20152_XTAL1_REG | START_BIT(1) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_XTAL1_PLL_REF_EN                (TDA20152_XTAL1_REG | START_BIT(0) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_XTAL2_REF_DIV_SEL               (TDA20152_XTAL2_REG | START_BIT(6) | NB_UNIT(0) | NB_BITS(2) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_XTAL2_EXT_CLK_FILT_SEL          (TDA20152_XTAL2_REG | START_BIT(4) | NB_UNIT(0) | NB_BITS(2) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_XTAL2_EXT_DIV_SEL               (TDA20152_XTAL2_REG | START_BIT(2) | NB_UNIT(0) | NB_BITS(2) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_XTAL2_EXT_CLK_EN                (TDA20152_XTAL2_REG | START_BIT(1) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_XTAL2_EXT_CLK_OVR               (TDA20152_XTAL2_REG | START_BIT(0) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_TEST_MUX_SEL_TESTMUX_EN         (TDA20152_TEST_MUX_SEL_REG | START_BIT(5) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_TEST_MUX_SEL_STM                (TDA20152_TEST_MUX_SEL_REG | START_BIT(0) | NB_UNIT(0) | NB_BITS(5) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_SPI_CTRL_SPI_RESET              (TDA20152_SPI_CTRL_REG | START_BIT(7) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_SPI_CTRL_XTAL_HPF_LOW_FREQ      (TDA20152_SPI_CTRL_REG | START_BIT(6) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_FE_LNA_EN                       (TDA20152_FE_REG | START_BIT(7) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_FE_LNA_LOWC                     (TDA20152_FE_REG | START_BIT(6) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_FE_LNA_GC                       (TDA20152_FE_REG | START_BIT(4) | NB_UNIT(0) | NB_BITS(2) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_FE_LPT_EN                       (TDA20152_FE_REG | START_BIT(3) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_FE_LPT_GC                       (TDA20152_FE_REG | START_BIT(1) | NB_UNIT(0) | NB_BITS(2) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_FE_LNA_HG_EN                    (TDA20152_FE_REG | START_BIT(0) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_ENABLE1_RF_VGA_EN               (TDA20152_ENABLE1_REG | START_BIT(7) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_ENABLE1_RF_VGA_LOWC             (TDA20152_ENABLE1_REG | START_BIT(6) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_ENABLE1_RF_MIX_EN               (TDA20152_ENABLE1_REG | START_BIT(5) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_ENABLE1_RF_MIX_LOWC             (TDA20152_ENABLE1_REG | START_BIT(4) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_ENABLE1_BB_EN                   (TDA20152_ENABLE1_REG | START_BIT(3) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_ENABLE1_SERVO_EN                (TDA20152_ENABLE1_REG | START_BIT(2) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_ENABLE1_AMPOUT_CUR              (TDA20152_ENABLE1_REG | START_BIT(1) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_ENABLE1_LNA_HALFREF_EN          (TDA20152_ENABLE1_REG | START_BIT(0) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_SPARE_REG1_CCAL_EN              (TDA20152_SPARE_REG1_REG | START_BIT(7) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_SPARE_REG1_CCAL_RST             (TDA20152_SPARE_REG1_REG | START_BIT(6) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_SPARE_REG1_CCAL_FREQ            (TDA20152_SPARE_REG1_REG | START_BIT(1) | NB_UNIT(0) | NB_BITS(5) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_VGA_RF_VGA_OFF                  (TDA20152_VGA_REG | START_BIT(5) | NB_UNIT(0) | NB_BITS(3) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_VGA_VGA1_OFF                    (TDA20152_VGA_REG | START_BIT(2) | NB_UNIT(0) | NB_BITS(3) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_BB1_VGA2_OFF                    (TDA20152_BB1_REG | START_BIT(5) | NB_UNIT(0) | NB_BITS(3) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_BB1_FLTA_BW                     (TDA20152_BB1_REG | START_BIT(0) | NB_UNIT(0) | NB_BITS(4) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_BB2_GMC_BW                      (TDA20152_BB2_REG | START_BIT(1) | NB_UNIT(0) | NB_BITS(7) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_BB3_AMPOUT                      (TDA20152_BB3_REG | START_BIT(4) | NB_UNIT(0) | NB_BITS(4) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_BB3_SERVO_TST                   (TDA20152_BB3_REG | START_BIT(3) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_BB3_BB_OFFCOR_TUNE              (TDA20152_BB3_REG | START_BIT(0) | NB_UNIT(0) | NB_BITS(3) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_PWR_DET_PWR_DET_EN              (TDA20152_PWR_DET_REG | START_BIT(7) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_PWR_DET_PWR_DET_CTRL            (TDA20152_PWR_DET_REG | START_BIT(5) | NB_UNIT(0) | NB_BITS(2) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_PWR_DET_PWR_DET_VAL             (TDA20152_PWR_DET_REG | START_BIT(1) | NB_UNIT(0) | NB_BITS(4) | SAT_TUNER_MAP_RO )
#define TDA20152_PWR_DET_PD_THERM                (TDA20152_PWR_DET_REG | START_BIT(0) | NB_UNIT(2) | NB_BITS(9) | SAT_TUNER_MAP_RO )
#define TDA20152_PD_THERM_PD_THERM               (TDA20152_PD_THERM_REG | START_BIT() | NB_UNIT() | NB_BITS()ERR )
#define TDA20152_VCO_ALC1_IVREF_EN               (TDA20152_VCO_ALC1_REG | START_BIT(7) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_VCO_ALC1_ALC_SPIC_EN            (TDA20152_VCO_ALC1_REG | START_BIT(6) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_VCO_ALC1_ALC_EN                 (TDA20152_VCO_ALC1_REG | START_BIT(5) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_VCO_ALC1_ALC_SPI_BIAS           (TDA20152_VCO_ALC1_REG | START_BIT(0) | NB_UNIT(0) | NB_BITS(4) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_VCO_ALC2_ALC_VTH                (TDA20152_VCO_ALC2_REG | START_BIT(6) | NB_UNIT(0) | NB_BITS(2) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_VCO_ALC2_VT_ALC_C               (TDA20152_VCO_ALC2_REG | START_BIT(5) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_VCO_ALC2_PSK8_EN                (TDA20152_VCO_ALC2_REG | START_BIT(4) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_VCO_ALC2_ALC_BIAS               (TDA20152_VCO_ALC2_REG | START_BIT(0) | NB_UNIT(0) | NB_BITS(4) | SAT_TUNER_MAP_RO )
#define TDA20152_VCO_ALC_OUT_ALC_DONE            (TDA20152_VCO_ALC_OUT_REG | START_BIT(7) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RO )
#define TDA20152_VCO_ALC_OUT_ALC_AM_STABLE       (TDA20152_VCO_ALC_OUT_REG | START_BIT(6) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RO )
#define TDA20152_VCO_ALC_OUT_ALC_AM_LOW          (TDA20152_VCO_ALC_OUT_REG | START_BIT(5) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RO )
#define TDA20152_VCO_ALC_OUT_ALC_T16_OVER        (TDA20152_VCO_ALC_OUT_REG | START_BIT(4) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RO )
#define TDA20152_VCO_ALC_OUT_ALC_HIT_LIMIT       (TDA20152_VCO_ALC_OUT_REG | START_BIT(3) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RO )
#define TDA20152_VCO_CAL_CLK_CLKGEN_EN           (TDA20152_VCO_CAL_CLK_REG | START_BIT(7) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_VCO_CAL_CLK_CLCKGEN_INTCLK      (TDA20152_VCO_CAL_CLK_REG | START_BIT(6) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_VCO_CAL_CLK_CLKGEN_SPDSEL       (TDA20152_VCO_CAL_CLK_REG | START_BIT(4) | NB_UNIT(0) | NB_BITS(2) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_VCO_TM1_TM_VCOA_SPIC            (TDA20152_VCO_TM1_REG | START_BIT(7) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_VCO_TM1_TM_VCOA_EN              (TDA20152_VCO_TM1_REG | START_BIT(6) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_VCO_TM1_TM_SPIC_EN              (TDA20152_VCO_TM1_REG | START_BIT(5) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_VCO_TM1_TM_EN                   (TDA20152_VCO_TM1_REG | START_BIT(4) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_VCO_TM1_TM_COMP_EN              (TDA20152_VCO_TM1_REG | START_BIT(2) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_VCO_TM1_TM_DELAY_S              (TDA20152_VCO_TM1_REG | START_BIT(0) | NB_UNIT(0) | NB_BITS(2) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_VCO_TM2_TM_VREFH                (TDA20152_VCO_TM2_REG | START_BIT(6) | NB_UNIT(0) | NB_BITS(2) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_VCO_TM2_TM_VREFL                (TDA20152_VCO_TM2_REG | START_BIT(4) | NB_UNIT(0) | NB_BITS(2) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_VCO_TM_OUT_TM_VT_HIGH           (TDA20152_VCO_TM_OUT_REG | START_BIT(7) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RO )
#define TDA20152_VCO_TM_OUT_TM_VT_LOW            (TDA20152_VCO_TM_OUT_REG | START_BIT(6) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RO )
#define TDA20152_VCO_TM_OUT_VCOB_IND             (TDA20152_VCO_TM_OUT_REG | START_BIT(5) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RO )
#define TDA20152_VCO_TM_OUT_TM_LOCK_LO           (TDA20152_VCO_TM_OUT_REG | START_BIT(4) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RO )
#define TDA20152_VCO_TM_OUT_LO_LOCK_FLAG         (TDA20152_VCO_TM_OUT_REG | START_BIT(2) | NB_UNIT(0) | NB_BITS(2) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_VCO_MUX_VCO_TMUX_OUT            (TDA20152_VCO_MUX_REG | START_BIT(6) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RO )
#define TDA20152_VCO_MUX_VCO_TMUX_S              (TDA20152_VCO_MUX_REG | START_BIT(0) | NB_UNIT(0) | NB_BITS(5) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_SPARE_REG3_CCAL_OUT             (TDA20152_SPARE_REG3_REG | START_BIT(7) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_SPARE_REG3_CCAL_VTH             (TDA20152_SPARE_REG3_REG | START_BIT(2) | NB_UNIT(0) | NB_BITS(5) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_PFD_CP1_CP_EN                   (TDA20152_PFD_CP1_REG | START_BIT(7) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_PFD_CP1_PFD_RESET_DELAY         (TDA20152_PFD_CP1_REG | START_BIT(5) | NB_UNIT(0) | NB_BITS(2) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_PFD_CP1_CP_FORCE_SRCE_ON        (TDA20152_PFD_CP1_REG | START_BIT(4) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_PFD_CP1_CP_FORCE_SRCE_OFF       (TDA20152_PFD_CP1_REG | START_BIT(3) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_PFD_CP1_CP_FORCE_SINK_ON        (TDA20152_PFD_CP1_REG | START_BIT(2) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_PFD_CP1_CP_FORCE_SINK_OFF       (TDA20152_PFD_CP1_REG | START_BIT(1) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_PFD_CP1_CP_FORCE_UNLOCK         (TDA20152_PFD_CP1_REG | START_BIT(0) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_PFD_CP2_CP_IOFST_EN             (TDA20152_PFD_CP2_REG | START_BIT(7) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_PFD_CP2_CP_IOFST_SELFREQ        (TDA20152_PFD_CP2_REG | START_BIT(5) | NB_UNIT(0) | NB_BITS(2) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_PFD_CP2_CP_IOFST_2X_EN          (TDA20152_PFD_CP2_REG | START_BIT(4) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_PFD_CP2_ICP_SEL                 (TDA20152_PFD_CP2_REG | START_BIT(2) | NB_UNIT(0) | NB_BITS(2) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_PFD_CP2_CP_IOFST_SINKMODE       (TDA20152_PFD_CP2_REG | START_BIT(1) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_PLL_EN_SX_BIAS_EN               (TDA20152_PLL_EN_REG | START_BIT(7) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_PLL_EN_PLL_DIVIDERS_EN          (TDA20152_PLL_EN_REG | START_BIT(6) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_PLL_EN_PREDIV_SEL               (TDA20152_PLL_EN_REG | START_BIT(5) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_PLL_EN_CLK_SD_EN                (TDA20152_PLL_EN_REG | START_BIT(4) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_PLL_EN_MD_ECL2CMOS_EN           (TDA20152_PLL_EN_REG | START_BIT(3) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_PLL_EN_PLL_LOWCUR_EN            (TDA20152_PLL_EN_REG | START_BIT(2) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_PLL_EN_PLL_LOCK_DELAY           (TDA20152_PLL_EN_REG | START_BIT(0) | NB_UNIT(0) | NB_BITS(2) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_PLL_TEST1_DSM_RESET_N           (TDA20152_PLL_TEST1_REG | START_BIT(7) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_PLL_TEST1_DSM_INT_MODE_EN       (TDA20152_PLL_TEST1_REG | START_BIT(6) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_PLL_TEST1_DSM_CLKB_CTRL         (TDA20152_PLL_TEST1_REG | START_BIT(5) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_PLL_TEST1_DSM_DITH_MODE         (TDA20152_PLL_TEST1_REG | START_BIT(3) | NB_UNIT(0) | NB_BITS(2) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_PLL_TEST1_DSM_TEST_CLK_SEL      (TDA20152_PLL_TEST1_REG | START_BIT(2) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_PLL_TEST1_DSM_TEST_CLK          (TDA20152_PLL_TEST1_REG | START_BIT(1) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_PLL_TEST1_DSM_MASH11_MODE_EN    (TDA20152_PLL_TEST1_REG | START_BIT(0) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_PLL_TEST2_PLL_LOCK_DET          (TDA20152_PLL_TEST2_REG | START_BIT(7) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RO )
#define TDA20152_PLL_TEST2_DSM_OUT               (TDA20152_PLL_TEST2_REG | START_BIT(0) | NB_UNIT(0) | NB_BITS(7) | SAT_TUNER_MAP_RO )
#define TDA20152_PLL_TEST3_DSM_CKOUT_EN          (TDA20152_PLL_TEST3_REG | START_BIT(7) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_PLL_TEST3_MD_FDIV2_OUT_EN       (TDA20152_PLL_TEST3_REG | START_BIT(6) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_PLL_TEST3_PLL_CLKDIV_MUXOUT_EN  (TDA20152_PLL_TEST3_REG | START_BIT(5) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_PLL_TEST3_PLL_DSM_SPI_EN        (TDA20152_PLL_TEST3_REG | START_BIT(0) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_PLL1_DSM_INT_IN                 (TDA20152_PLL1_REG | START_BIT(0) | NB_UNIT(0) | NB_BITS(8) | SAT_TUNER_MAP_RW )
#define TDA20152_PLL2_DSM_FRAC_IN                (TDA20152_PLL2_REG | START_BIT(0) | NB_UNIT(2) | NB_BITS(16) | SAT_TUNER_MAP_RW )
#define TDA20152_PLL3_DSM_FRAC_IN                (TDA20152_PLL3_REG | START_BIT() | NB_UNIT() | NB_BITS()ERR )
#define TDA20152_PLL4_PLL_DSM_FRAC_SEL           (TDA20152_PLL4_REG | START_BIT(6) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_PLL4_CP_FORCE_LOCK              (TDA20152_PLL4_REG | START_BIT(5) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_PLL4_EN_IDZ_RMCTRL_VIH          (TDA20152_PLL4_REG | START_BIT(4) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_PLL4_SEL_RM_IDZ_FREQ_VIH        (TDA20152_PLL4_REG | START_BIT(2) | NB_UNIT(0) | NB_BITS(2) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_PLL4_SEL_RM_IDZ_ICP_VIH         (TDA20152_PLL4_REG | START_BIT(0) | NB_UNIT(0) | NB_BITS(2) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_LO_CHAIN1_DBL_EN                (TDA20152_LO_CHAIN1_REG | START_BIT(7) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_LO_CHAIN1_DCC1_EN               (TDA20152_LO_CHAIN1_REG | START_BIT(6) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_LO_CHAIN1_DCC2_EN               (TDA20152_LO_CHAIN1_REG | START_BIT(5) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_LO_CHAIN1_DIV2_EN               (TDA20152_LO_CHAIN1_REG | START_BIT(4) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_LO_CHAIN1_INBUF_EN              (TDA20152_LO_CHAIN1_REG | START_BIT(3) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_LO_CHAIN1_MAIN_DIV_EN           (TDA20152_LO_CHAIN1_REG | START_BIT(2) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_LO_CHAIN1_OP_MUX_EN             (TDA20152_LO_CHAIN1_REG | START_BIT(1) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_LO_CHAIN1_POLY_EN               (TDA20152_LO_CHAIN1_REG | START_BIT(0) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_LO_CHAIN2_DIV1_MODE3            (TDA20152_LO_CHAIN2_REG | START_BIT(7) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_LO_CHAIN2_DIV2_MODE3            (TDA20152_LO_CHAIN2_REG | START_BIT(5) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_LO_CHAIN2_OP_4TO7_SEL           (TDA20152_LO_CHAIN2_REG | START_BIT(3) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_LO_CHAIN2_OP_DIV8_SEL           (TDA20152_LO_CHAIN2_REG | START_BIT(2) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_LO_CHAIN2_OP_EXT2LO_SEL         (TDA20152_LO_CHAIN2_REG | START_BIT(1) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_LO_CHAIN3_CAP_EN                (TDA20152_LO_CHAIN3_REG | START_BIT(7) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_LO_CHAIN3_PREMMODE_EN           (TDA20152_LO_CHAIN3_REG | START_BIT(6) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_LO_CHAIN3_DBL_HICUR_EN          (TDA20152_LO_CHAIN3_REG | START_BIT(5) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_LO_CHAIN3_DCC1BPF_HICUR_EN      (TDA20152_LO_CHAIN3_REG | START_BIT(4) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_LO_CHAIN3_DCC2BPF_HICUR_EN      (TDA20152_LO_CHAIN3_REG | START_BIT(3) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_LO_CHAIN3_DCC1LIMIT_HICUR_EN    (TDA20152_LO_CHAIN3_REG | START_BIT(2) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_LO_CHAIN3_DCC2LIMIT_HICUR_EN    (TDA20152_LO_CHAIN3_REG | START_BIT(1) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_LO_CHAIN3_DIV2_HICUR_EN         (TDA20152_LO_CHAIN3_REG | START_BIT(0) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_LO_CHAIN4_MAINDIV_HICUR_EN      (TDA20152_LO_CHAIN4_REG | START_BIT(7) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_LO_CHAIN4_INBUF_HICUR_EN        (TDA20152_LO_CHAIN4_REG | START_BIT(6) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_LO_CHAIN4_OUTMUX_HICUR_EN       (TDA20152_LO_CHAIN4_REG | START_BIT(5) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_LO_CHAIN4_POLY_HICUR_EN         (TDA20152_LO_CHAIN4_REG | START_BIT(4) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_LO_CHAIN4_LOMUX_EN              (TDA20152_LO_CHAIN4_REG | START_BIT(3) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_LO_CHAIN4_DIV8_HUICUR_ENH       (TDA20152_LO_CHAIN4_REG | START_BIT(2) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_LO_CHAIN4_FINALEF_HICUR_ENH     (TDA20152_LO_CHAIN4_REG | START_BIT(1) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_LO_CHAIN4_LOMUX_HICUR_EN        (TDA20152_LO_CHAIN4_REG | START_BIT(0) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_LO_TEST_LOTEST_MEAS_SEL         (TDA20152_LO_TEST_REG | START_BIT(7) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_LO_TEST_LOTEST_EN               (TDA20152_LO_TEST_REG | START_BIT(6) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_LO_TEST_LOTEST_INJECT_EN        (TDA20152_LO_TEST_REG | START_BIT(5) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_LO_TEST_LOTEST_MEAS_INJ_HICUR   (TDA20152_LO_TEST_REG | START_BIT(3) | NB_UNIT(0) | NB_BITS(2) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_LO_TEST_LOTEST_EXTLO_SEL        (TDA20152_LO_TEST_REG | START_BIT(2) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_LO_TEST_LOTEST_D2S2D_HICUR      (TDA20152_LO_TEST_REG | START_BIT(0) | NB_UNIT(0) | NB_BITS(2) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_DIG_LDO_LDO_EN                  (TDA20152_DIG_LDO_REG | START_BIT(7) | NB_UNIT(0) | NB_BITS(1) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )
#define TDA20152_DIG_LDO_LDO_V                   (TDA20152_DIG_LDO_REG | START_BIT(5) | NB_UNIT(0) | NB_BITS(2) | SAT_TUNER_MAP_RW | SAT_TUNER_MAP_RMW )

/**********************************************************************************
 * Blackwidow tuner bitfield access
 *    Automatically generated in regmap_08_rename_0.9.xls
 **********************************************************************************/

#define TDA20152_CLEAN 0xFF

#define TDA20152_XTAL1_XTAL_EN_WR(x)                     (((x) & 0x01)<<7)
#define TDA20152_XTAL1_XTAL_SU_BOOST_DIS_WR(x)           (((x) & 0x01)<<6)
#define TDA20152_XTAL1_XTAL_ALC_DIS_WR(x)                (((x) & 0x01)<<5)
#define TDA20152_XTAL1_XTAL_ALC_LVL_CTRL_WR(x)           (((x) & 0x03)<<3)
#define TDA20152_XTAL1_XTAL_EXT_SQUARWAVE_FORCE_WR(x)    (((x) & 0x01)<<2)
#define TDA20152_XTAL1_XTAL_EXT_INPUT_EN_WR(x)           (((x) & 0x01)<<1)
#define TDA20152_XTAL1_PLL_REF_EN_WR(x)                  ((x) & 0x01)
#define TDA20152_XTAL2_REF_DIV_SEL_WR(x)                 (((x) & 0x03)<<6)
#define TDA20152_XTAL2_EXT_CLK_FILT_SEL_WR(x)            (((x) & 0x03)<<4)
#define TDA20152_XTAL2_EXT_DIV_SEL_WR(x)                 (((x) & 0x03)<<2)
#define TDA20152_XTAL2_EXT_CLK_EN_WR(x)                  (((x) & 0x01)<<1)
#define TDA20152_XTAL2_EXT_CLK_OVR_WR(x)                 ((x) & 0x01)
#define TDA20152_TEST_MUX_SEL_TESTMUX_EN_WR(x)           (((x) & 0x01)<<5)
#define TDA20152_TEST_MUX_SEL_STM_WR(x)                  ((x) & 0x1F)
#define TDA20152_SPI_CTRL_SPI_RESET_WR(x)                (((x) & 0x01)<<7)
#define TDA20152_SPI_CTRL_XTAL_HPF_LOW_FREQ_WR(x)        (((x) & 0x01)<<6)
#define TDA20152_FE_LNA_EN_WR(x)                         (((x) & 0x01)<<7)
#define TDA20152_FE_LNA_LOWC_WR(x)                       (((x) & 0x01)<<6)
#define TDA20152_FE_LNA_GC_WR(x)                         (((x) & 0x03)<<4)
#define TDA20152_FE_LPT_EN_WR(x)                         (((x) & 0x01)<<3)
#define TDA20152_FE_LPT_GC_WR(x)                         (((x) & 0x03)<<1)
#define TDA20152_FE_LNA_HG_EN_WR(x)                      ((x) & 0x01)
#define TDA20152_ENABLE1_RF_VGA_EN_WR(x)                 (((x) & 0x01)<<7)
#define TDA20152_ENABLE1_RF_VGA_LOWC_WR(x)               (((x) & 0x01)<<6)
#define TDA20152_ENABLE1_RF_MIX_EN_WR(x)                 (((x) & 0x01)<<5)
#define TDA20152_ENABLE1_RF_MIX_LOWC_WR(x)               (((x) & 0x01)<<4)
#define TDA20152_ENABLE1_BB_EN_WR(x)                     (((x) & 0x01)<<3)
#define TDA20152_ENABLE1_SERVO_EN_WR(x)                  (((x) & 0x01)<<2)
#define TDA20152_ENABLE1_AMPOUT_CUR_WR(x)                (((x) & 0x01)<<1)
#define TDA20152_ENABLE1_LNA_HALFREF_EN_WR(x)            ((x) & 0x01)
#define TDA20152_SPARE_REG1_CCAL_EN_WR(x)                (((x) & 0x01)<<7)
#define TDA20152_SPARE_REG1_CCAL_RST_WR(x)               (((x) & 0x01)<<6)
#define TDA20152_SPARE_REG1_CCAL_FREQ_WR(x)              (((x) & 0x1F)<<1)
#define TDA20152_VGA_RF_VGA_OFF_WR(x)                    (((x) & 0x07)<<5)
#define TDA20152_VGA_VGA1_OFF_WR(x)                      (((x) & 0x07)<<2)
#define TDA20152_VGA_LOBUF_HG_EN_WR(x)                   (((x) & 0x01)<<1)
#define TDA20152_BB1_VGA2_OFF_WR(x)                      (((x) & 0x07)<<5)
#define TDA20152_BB1_FLTA_BW_WR(x)                       ((x) & 0x0F)
#define TDA20152_BB2_GMC_BW_WR(x)                        (((x) & 0x7F)<<1)
#define TDA20152_BB3_AMPOUT_WR(x)                        (((x) & 0x0F)<<4)
#define TDA20152_BB3_SERVO_TST_WR(x)                     (((x) & 0x01)<<3)
#define TDA20152_BB3_BB_OFFCOR_TUNE_WR(x)                ((x) & 0x07)
#define TDA20152_PWR_DET_PWR_DET_EN_WR(x)                (((x) & 0x01)<<7)
#define TDA20152_PWR_DET_PWR_DET_CTRL_WR(x)              (((x) & 0x03)<<5)
#define TDA20152_PWR_DET_PWR_DET_VAL_WR(x)               (((x) & 0x0F)<<1)
//#NUM!
#define TDA20152_PD_THERM_PD_THERM_WR(x)                 ((x) & 0x00)
#define TDA20152_VCO_ALC1_IVREF_EN_WR(x)                 (((x) & 0x01)<<7)
#define TDA20152_VCO_ALC1_ALC_SPIC_EN_WR(x)              (((x) & 0x01)<<6)
#define TDA20152_VCO_ALC1_ALC_EN_WR(x)                   (((x) & 0x01)<<5)
#define TDA20152_VCO_ALC1_ALC_SPI_BIAS_WR(x)             ((x) & 0x0F)
#define TDA20152_VCO_ALC2_ALC_VTH_WR(x)                  (((x) & 0x03)<<6)
#define TDA20152_VCO_ALC2_VT_ALC_C_WR(x)                 (((x) & 0x01)<<5)
#define TDA20152_VCO_ALC2_PSK8_EN_WR(x)                  (((x) & 0x01)<<4)
#define TDA20152_VCO_ALC2_ALC_BIAS_WR(x)                 ((x) & 0x0F)
#define TDA20152_VCO_ALC_OUT_ALC_DONE_WR(x)              (((x) & 0x01)<<7)
#define TDA20152_VCO_ALC_OUT_ALC_AM_STABLE_WR(x)         (((x) & 0x01)<<6)
#define TDA20152_VCO_ALC_OUT_ALC_AM_LOW_WR(x)            (((x) & 0x01)<<5)
#define TDA20152_VCO_ALC_OUT_ALC_T16_OVER_WR(x)          (((x) & 0x01)<<4)
#define TDA20152_VCO_ALC_OUT_ALC_HIT_LIMIT_WR(x)         (((x) & 0x01)<<3)
#define TDA20152_VCO_CAL_CLK_CLKGEN_EN_WR(x)             (((x) & 0x01)<<7)
#define TDA20152_VCO_CAL_CLK_CLCKGEN_INTCLK_WR(x)        (((x) & 0x01)<<6)
#define TDA20152_VCO_CAL_CLK_CLKGEN_SPDSEL_WR(x)         (((x) & 0x03)<<4)
#define TDA20152_VCO_TM1_TM_VCOA_SPIC_WR(x)              (((x) & 0x01)<<7)
#define TDA20152_VCO_TM1_TM_VCOA_EN_WR(x)                (((x) & 0x01)<<6)
#define TDA20152_VCO_TM1_TM_SPIC_EN_WR(x)                (((x) & 0x01)<<5)
#define TDA20152_VCO_TM1_TM_EN_WR(x)                     (((x) & 0x01)<<4)
#define TDA20152_VCO_TM1_TM_COMP_EN_WR(x)                (((x) & 0x01)<<2)
#define TDA20152_VCO_TM1_TM_DELAY_S_WR(x)                ((x) & 0x03)
#define TDA20152_VCO_TM2_TM_VREFH_WR(x)                  (((x) & 0x03)<<6)
#define TDA20152_VCO_TM2_TM_VREFL_WR(x)                  (((x) & 0x03)<<4)
#define TDA20152_VCO_TM_OUT_TM_VT_HIGH_WR(x)             (((x) & 0x01)<<7)
#define TDA20152_VCO_TM_OUT_TM_VT_LOW_WR(x)              (((x) & 0x01)<<6)
#define TDA20152_VCO_TM_OUT_VCOB_IND_WR(x)               (((x) & 0x01)<<5)
#define TDA20152_VCO_TM_OUT_TM_LOCK_LO_WR(x)             (((x) & 0x01)<<4)
#define TDA20152_VCO_TM_OUT_LO_LOCK_FLAG_WR(x)           (((x) & 0x03)<<2)
#define TDA20152_VCO_MUX_VCO_TMUX_OUT_WR(x)              (((x) & 0x01)<<6)
#define TDA20152_VCO_MUX_VCO_TMUX_S_WR(x)                ((x) & 0x1F)
#define TDA20152_SPARE_REG3_CCAL_OUT_WR(x)               (((x) & 0x01)<<7)
#define TDA20152_SPARE_REG3_CCAL_VTH_WR(x)               (((x) & 0x1F)<<2)
#define TDA20152_PFD_CP1_CP_EN_WR(x)                     (((x) & 0x01)<<7)
#define TDA20152_PFD_CP1_PFD_RESET_DELAY_WR(x)           (((x) & 0x03)<<5)
#define TDA20152_PFD_CP1_CP_FORCE_SRCE_ON_WR(x)          (((x) & 0x01)<<4)
#define TDA20152_PFD_CP1_CP_FORCE_SRCE_OFF_WR(x)         (((x) & 0x01)<<3)
#define TDA20152_PFD_CP1_CP_FORCE_SINK_ON_WR(x)          (((x) & 0x01)<<2)
#define TDA20152_PFD_CP1_CP_FORCE_SINK_OFF_WR(x)         (((x) & 0x01)<<1)
#define TDA20152_PFD_CP1_CP_FORCE_UNLOCK_WR(x)           ((x) & 0x01)
#define TDA20152_PFD_CP2_CP_IOFST_EN_WR(x)               (((x) & 0x01)<<7)
#define TDA20152_PFD_CP2_CP_IOFST_SELFREQ_WR(x)          (((x) & 0x03)<<5)
#define TDA20152_PFD_CP2_CP_IOFST_2X_EN_WR(x)            (((x) & 0x01)<<4)
#define TDA20152_PFD_CP2_ICP_SEL_WR(x)                   (((x) & 0x03)<<2)
#define TDA20152_PFD_CP2_CP_IOFST_SINKMODE_WR(x)         (((x) & 0x01)<<1)
#define TDA20152_PLL_EN_SX_BIAS_EN_WR(x)                 (((x) & 0x01)<<7)
#define TDA20152_PLL_EN_PLL_DIVIDERS_EN_WR(x)            (((x) & 0x01)<<6)
#define TDA20152_PLL_EN_PREDIV_SEL_WR(x)                 (((x) & 0x01)<<5)
#define TDA20152_PLL_EN_CLK_SD_EN_WR(x)                  (((x) & 0x01)<<4)
#define TDA20152_PLL_EN_MD_ECL2CMOS_EN_WR(x)             (((x) & 0x01)<<3)
#define TDA20152_PLL_EN_PLL_LOWCUR_EN_WR(x)              (((x) & 0x01)<<2)
#define TDA20152_PLL_EN_PLL_LOCK_DELAY_WR(x)             ((x) & 0x03)
#define TDA20152_PLL_TEST1_DSM_RESET_N_WR(x)             (((x) & 0x01)<<7)
#define TDA20152_PLL_TEST1_DSM_INT_MODE_EN_WR(x)         (((x) & 0x01)<<6)
#define TDA20152_PLL_TEST1_DSM_CLKB_CTRL_WR(x)           (((x) & 0x01)<<5)
#define TDA20152_PLL_TEST1_DSM_DITH_MODE_WR(x)           (((x) & 0x03)<<3)
#define TDA20152_PLL_TEST1_DSM_TEST_CLK_SEL_WR(x)        (((x) & 0x01)<<2)
#define TDA20152_PLL_TEST1_DSM_TEST_CLK_WR(x)            (((x) & 0x01)<<1)
#define TDA20152_PLL_TEST1_DSM_MASH11_MODE_EN_WR(x)      ((x) & 0x01)
#define TDA20152_PLL_TEST2_PLL_LOCK_DET_WR(x)            (((x) & 0x01)<<7)
#define TDA20152_PLL_TEST2_DSM_OUT_WR(x)                 ((x) & 0x7F)
#define TDA20152_PLL_TEST3_DSM_CKOUT_EN_WR(x)            (((x) & 0x01)<<7)
#define TDA20152_PLL_TEST3_MD_FDIV2_OUT_EN_WR(x)         (((x) & 0x01)<<6)
#define TDA20152_PLL_TEST3_PLL_CLKDIV_MUXOUT_EN_WR(x)    (((x) & 0x01)<<5)
#define TDA20152_PLL_TEST3_PLL_DSM_SPI_EN_WR(x)          ((x) & 0x01)
#define TDA20152_PLL1_DSM_INT_IN_WR(x)                   ((x) & 0xFF)
//#NUM!
#define TDA20152_PLL3_DSM_FRAC_IN_WR(x)                  ((x) & 0x00)
#define TDA20152_PLL4_PLL_DSM_FRAC_SEL_WR(x)             (((x) & 0x01)<<6)
#define TDA20152_PLL4_CP_FORCE_LOCK_WR(x)                (((x) & 0x01)<<5)
#define TDA20152_PLL4_EN_IDZ_RMCTRL_VIH_WR(x)            (((x) & 0x01)<<4)
#define TDA20152_PLL4_SEL_RM_IDZ_FREQ_VIH_WR(x)          (((x) & 0x03)<<2)
#define TDA20152_PLL4_SEL_RM_IDZ_ICP_VIH_WR(x)           ((x) & 0x03)
#define TDA20152_LO_CHAIN1_DBL_EN_WR(x)                  (((x) & 0x01)<<7)
#define TDA20152_LO_CHAIN1_DCC1_EN_WR(x)                 (((x) & 0x01)<<6)
#define TDA20152_LO_CHAIN1_DCC2_EN_WR(x)                 (((x) & 0x01)<<5)
#define TDA20152_LO_CHAIN1_DIV2_EN_WR(x)                 (((x) & 0x01)<<4)
#define TDA20152_LO_CHAIN1_INBUF_EN_WR(x)                (((x) & 0x01)<<3)
#define TDA20152_LO_CHAIN1_MAIN_DIV_EN_WR(x)             (((x) & 0x01)<<2)
#define TDA20152_LO_CHAIN1_OP_MUX_EN_WR(x)               (((x) & 0x01)<<1)
#define TDA20152_LO_CHAIN1_POLY_EN_WR(x)                 ((x) & 0x01)
#define TDA20152_LO_CHAIN2_DIV1_MODE3_WR(x)              (((x) & 0x01)<<7)
#define TDA20152_LO_CHAIN2_DIV2_MODE3_WR(x)              (((x) & 0x01)<<5)
#define TDA20152_LO_CHAIN2_OP_4TO7_SEL_WR(x)             (((x) & 0x01)<<3)
#define TDA20152_LO_CHAIN2_OP_DIV8_SEL_WR(x)             (((x) & 0x01)<<2)
#define TDA20152_LO_CHAIN2_OP_EXT2LO_SEL_WR(x)           (((x) & 0x01)<<1)
#define TDA20152_LO_CHAIN2_DCC1BPF_NOM_CURR_ENH_WR(x)    ((x) & 0x01)
#define TDA20152_LO_CHAIN3_CAP_EN_WR(x)                  (((x) & 0x01)<<7)
#define TDA20152_LO_CHAIN3_PREMMODE_EN_WR(x)             (((x) & 0x01)<<6)
#define TDA20152_LO_CHAIN3_DBL_HICUR_EN_WR(x)            (((x) & 0x01)<<5)
#define TDA20152_LO_CHAIN3_DCC1BPF_HICUR_EN_WR(x)        (((x) & 0x01)<<4)
#define TDA20152_LO_CHAIN3_DCC2BPF_HICUR_EN_WR(x)        (((x) & 0x01)<<3)
#define TDA20152_LO_CHAIN3_DCC1LIMIT_HICUR_EN_WR(x)      (((x) & 0x01)<<2)
#define TDA20152_LO_CHAIN3_DCC2LIMIT_HICUR_EN_WR(x)      (((x) & 0x01)<<1)
#define TDA20152_LO_CHAIN3_DIV2_HICUR_EN_WR(x)           ((x) & 0x01)
#define TDA20152_LO_CHAIN4_MAINDIV_HICUR_EN_WR(x)        (((x) & 0x01)<<7)
#define TDA20152_LO_CHAIN4_INBUF_HICUR_EN_WR(x)          (((x) & 0x01)<<6)
#define TDA20152_LO_CHAIN4_OUTMUX_HICUR_EN_WR(x)         (((x) & 0x01)<<5)
#define TDA20152_LO_CHAIN4_POLY_HICUR_EN_WR(x)           (((x) & 0x01)<<4)
#define TDA20152_LO_CHAIN4_LOMUX_EN_WR(x)                (((x) & 0x01)<<3)
#define TDA20152_LO_CHAIN4_DIV8_HUICUR_ENH_WR(x)         (((x) & 0x01)<<2)
#define TDA20152_LO_CHAIN4_FINALEF_HICUR_ENH_WR(x)       (((x) & 0x01)<<1)
#define TDA20152_LO_CHAIN4_LOMUX_HICUR_EN_WR(x)          ((x) & 0x01)
#define TDA20152_LO_TEST_LOTEST_MEAS_SEL_WR(x)           (((x) & 0x01)<<7)
#define TDA20152_LO_TEST_LOTEST_EN_WR(x)                 (((x) & 0x01)<<6)
#define TDA20152_LO_TEST_LOTEST_INJECT_EN_WR(x)          (((x) & 0x01)<<5)
#define TDA20152_LO_TEST_LOTEST_MEAS_INJ_HICUR_WR(x)     (((x) & 0x03)<<3)
#define TDA20152_LO_TEST_LOTEST_EXTLO_SEL_WR(x)          (((x) & 0x01)<<2)
#define TDA20152_LO_TEST_LOTEST_D2S2D_HICUR_WR(x)        ((x) & 0x03)
#define TDA20152_DIG_LDO_LDO_EN_WR(x)                    (((x) & 0x01)<<7)
#define TDA20152_DIG_LDO_LDO_V_WR(x)                     (((x) & 0x03)<<5)







#define TDA20152_XTAL1_XTAL_EN_RD(x)                     (((x)>>7) & 0x01)
#define TDA20152_XTAL1_XTAL_SU_BOOST_DIS_RD(x)           (((x)>>6) & 0x01)
#define TDA20152_XTAL1_XTAL_ALC_DIS_RD(x)                (((x)>>5) & 0x01)
#define TDA20152_XTAL1_XTAL_ALC_LVL_CTRL_RD(x)           (((x)>>3) & 0x03)
#define TDA20152_XTAL1_XTAL_EXT_SQUARWAVE_FORCE_RD(x)    (((x)>>2) & 0x01)
#define TDA20152_XTAL1_XTAL_EXT_INPUT_EN_RD(x)           (((x)>>1) & 0x01)
#define TDA20152_XTAL1_PLL_REF_EN_RD(x)                  ((x) & 0x01)
#define TDA20152_XTAL2_REF_DIV_SEL_RD(x)                 (((x)>>6) & 0x03)
#define TDA20152_XTAL2_EXT_CLK_FILT_SEL_RD(x)            (((x)>>4) & 0x03)
#define TDA20152_XTAL2_EXT_DIV_SEL_RD(x)                 (((x)>>2) & 0x03)
#define TDA20152_XTAL2_EXT_CLK_EN_RD(x)                  (((x)>>1) & 0x01)
#define TDA20152_XTAL2_EXT_CLK_OVR_RD(x)                 ((x) & 0x01)
#define TDA20152_TEST_MUX_SEL_TESTMUX_EN_RD(x)           (((x)>>5) & 0x01)
#define TDA20152_TEST_MUX_SEL_STM_RD(x)                  ((x) & 0x1F)
#define TDA20152_SPI_CTRL_SPI_RESET_RD(x)                (((x)>>7) & 0x01)
#define TDA20152_SPI_CTRL_XTAL_HPF_LOW_FREQ_RD(x)        (((x)>>6) & 0x01)
#define TDA20152_FE_LNA_EN_RD(x)                         (((x)>>7) & 0x01)
#define TDA20152_FE_LNA_LOWC_RD(x)                       (((x)>>6) & 0x01)
#define TDA20152_FE_LNA_GC_RD(x)                         (((x)>>4) & 0x03)
#define TDA20152_FE_LPT_EN_RD(x)                         (((x)>>3) & 0x01)
#define TDA20152_FE_LPT_GC_RD(x)                         (((x)>>1) & 0x03)
#define TDA20152_FE_LNA_HG_EN_RD(x)                      ((x) & 0x01)
#define TDA20152_ENABLE1_RF_VGA_EN_RD(x)                 (((x)>>7) & 0x01)
#define TDA20152_ENABLE1_RF_VGA_LOWC_RD(x)               (((x)>>6) & 0x01)
#define TDA20152_ENABLE1_RF_MIX_EN_RD(x)                 (((x)>>5) & 0x01)
#define TDA20152_ENABLE1_RF_MIX_LOWC_RD(x)               (((x)>>4) & 0x01)
#define TDA20152_ENABLE1_BB_EN_RD(x)                     (((x)>>3) & 0x01)
#define TDA20152_ENABLE1_SERVO_EN_RD(x)                  (((x)>>2) & 0x01)
#define TDA20152_ENABLE1_AMPOUT_CUR_RD(x)                (((x)>>1) & 0x01)
#define TDA20152_ENABLE1_LNA_HALFREF_EN_RD(x)            ((x) & 0x01)
#define TDA20152_SPARE_REG1_CCAL_EN_RD(x)                (((x)>>7) & 0x01)
#define TDA20152_SPARE_REG1_CCAL_RST_RD(x)               (((x)>>6) & 0x01)
#define TDA20152_SPARE_REG1_CCAL_FREQ_RD(x)              (((x)>>1) & 0x1F)
#define TDA20152_VGA_RF_VGA_OFF_RD(x)                    (((x)>>5) & 0x07)
#define TDA20152_VGA_VGA1_OFF_RD(x)                      (((x)>>2) & 0x07)
#define TDA20152_VGA_LOBUF_HG_EN_RD(x)                   (((x)>>1) & 0x01)
#define TDA20152_BB1_VGA2_OFF_RD(x)                      (((x)>>5) & 0x07)
#define TDA20152_BB1_FLTA_BW_RD(x)                       ((x) & 0x0F)
#define TDA20152_BB2_GMC_BW_RD(x)                        (((x)>>1) & 0x7F)
#define TDA20152_BB3_AMPOUT_RD(x)                        (((x)>>4) & 0x0F)
#define TDA20152_BB3_SERVO_TST_RD(x)                     (((x)>>3) & 0x01)
#define TDA20152_BB3_BB_OFFCOR_TUNE_RD(x)                ((x) & 0x07)
#define TDA20152_PWR_DET_PWR_DET_EN_RD(x)                (((x)>>7) & 0x01)
#define TDA20152_PWR_DET_PWR_DET_CTRL_RD(x)              (((x)>>5) & 0x03)
#define TDA20152_PWR_DET_PWR_DET_VAL_RD(x)               (((x)>>1) & 0x0F)
//#define TDA20152_PWR_DET_PD_THERM_RD(x)                  (x)
#define TDA20152_PD_THERM_PD_THERM_RD(x)                 ((x) & 0x00)
#define TDA20152_VCO_ALC1_IVREF_EN_RD(x)                 (((x)>>7) & 0x01)
#define TDA20152_VCO_ALC1_ALC_SPIC_EN_RD(x)              (((x)>>6) & 0x01)
#define TDA20152_VCO_ALC1_ALC_EN_RD(x)                   (((x)>>5) & 0x01)
#define TDA20152_VCO_ALC1_ALC_SPI_BIAS_RD(x)             ((x) & 0x0F)
#define TDA20152_VCO_ALC2_ALC_VTH_RD(x)                  (((x)>>6) & 0x03)
#define TDA20152_VCO_ALC2_VT_ALC_C_RD(x)                 (((x)>>5) & 0x01)
#define TDA20152_VCO_ALC2_PSK8_EN_RD(x)                  (((x)>>4) & 0x01)
#define TDA20152_VCO_ALC2_ALC_BIAS_RD(x)                 ((x) & 0x0F)
#define TDA20152_VCO_ALC_OUT_ALC_DONE_RD(x)              (((x)>>7) & 0x01)
#define TDA20152_VCO_ALC_OUT_ALC_AM_STABLE_RD(x)         (((x)>>6) & 0x01)
#define TDA20152_VCO_ALC_OUT_ALC_AM_LOW_RD(x)            (((x)>>5) & 0x01)
#define TDA20152_VCO_ALC_OUT_ALC_T16_OVER_RD(x)          (((x)>>4) & 0x01)
#define TDA20152_VCO_ALC_OUT_ALC_HIT_LIMIT_RD(x)         (((x)>>3) & 0x01)
#define TDA20152_VCO_CAL_CLK_CLKGEN_EN_RD(x)             (((x)>>7) & 0x01)
#define TDA20152_VCO_CAL_CLK_CLCKGEN_INTCLK_RD(x)        (((x)>>6) & 0x01)
#define TDA20152_VCO_CAL_CLK_CLKGEN_SPDSEL_RD(x)         (((x)>>4) & 0x03)
#define TDA20152_VCO_TM1_TM_VCOA_SPIC_RD(x)              (((x)>>7) & 0x01)
#define TDA20152_VCO_TM1_TM_VCOA_EN_RD(x)                (((x)>>6) & 0x01)
#define TDA20152_VCO_TM1_TM_SPIC_EN_RD(x)                (((x)>>5) & 0x01)
#define TDA20152_VCO_TM1_TM_EN_RD(x)                     (((x)>>4) & 0x01)
#define TDA20152_VCO_TM1_TM_COMP_EN_RD(x)                (((x)>>2) & 0x01)
#define TDA20152_VCO_TM1_TM_DELAY_S_RD(x)                ((x) & 0x03)
#define TDA20152_VCO_TM2_TM_VREFH_RD(x)                  (((x)>>6) & 0x03)
#define TDA20152_VCO_TM2_TM_VREFL_RD(x)                  (((x)>>4) & 0x03)
#define TDA20152_VCO_TM_OUT_TM_VT_HIGH_RD(x)             (((x)>>7) & 0x01)
#define TDA20152_VCO_TM_OUT_TM_VT_LOW_RD(x)              (((x)>>6) & 0x01)
#define TDA20152_VCO_TM_OUT_VCOB_IND_RD(x)               (((x)>>5) & 0x01)
#define TDA20152_VCO_TM_OUT_TM_LOCK_LO_RD(x)             (((x)>>4) & 0x01)
#define TDA20152_VCO_TM_OUT_LO_LOCK_FLAG_RD(x)           (((x)>>2) & 0x03)
#define TDA20152_VCO_MUX_VCO_TMUX_OUT_RD(x)              (((x)>>6) & 0x01)
#define TDA20152_VCO_MUX_VCO_TMUX_S_RD(x)                ((x) & 0x1F)
#define TDA20152_SPARE_REG3_CCAL_OUT_RD(x)               (((x)>>7) & 0x01)
#define TDA20152_SPARE_REG3_CCAL_VTH_RD(x)               (((x)>>2) & 0x1F)
#define TDA20152_PFD_CP1_CP_EN_RD(x)                     (((x)>>7) & 0x01)
#define TDA20152_PFD_CP1_PFD_RESET_DELAY_RD(x)           (((x)>>5) & 0x03)
#define TDA20152_PFD_CP1_CP_FORCE_SRCE_ON_RD(x)          (((x)>>4) & 0x01)
#define TDA20152_PFD_CP1_CP_FORCE_SRCE_OFF_RD(x)         (((x)>>3) & 0x01)
#define TDA20152_PFD_CP1_CP_FORCE_SINK_ON_RD(x)          (((x)>>2) & 0x01)
#define TDA20152_PFD_CP1_CP_FORCE_SINK_OFF_RD(x)         (((x)>>1) & 0x01)
#define TDA20152_PFD_CP1_CP_FORCE_UNLOCK_RD(x)           ((x) & 0x01)
#define TDA20152_PFD_CP2_CP_IOFST_EN_RD(x)               (((x)>>7) & 0x01)
#define TDA20152_PFD_CP2_CP_IOFST_SELFREQ_RD(x)          (((x)>>5) & 0x03)
#define TDA20152_PFD_CP2_CP_IOFST_2X_EN_RD(x)            (((x)>>4) & 0x01)
#define TDA20152_PFD_CP2_ICP_SEL_RD(x)                   (((x)>>2) & 0x03)
#define TDA20152_PFD_CP2_CP_IOFST_SINKMODE_RD(x)         (((x)>>1) & 0x01)
#define TDA20152_PLL_EN_SX_BIAS_EN_RD(x)                 (((x)>>7) & 0x01)
#define TDA20152_PLL_EN_PLL_DIVIDERS_EN_RD(x)            (((x)>>6) & 0x01)
#define TDA20152_PLL_EN_PREDIV_SEL_RD(x)                 (((x)>>5) & 0x01)
#define TDA20152_PLL_EN_CLK_SD_EN_RD(x)                  (((x)>>4) & 0x01)
#define TDA20152_PLL_EN_MD_ECL2CMOS_EN_RD(x)             (((x)>>3) & 0x01)
#define TDA20152_PLL_EN_PLL_LOWCUR_EN_RD(x)              (((x)>>2) & 0x01)
#define TDA20152_PLL_EN_PLL_LOCK_DELAY_RD(x)             ((x) & 0x03)
#define TDA20152_PLL_TEST1_DSM_RESET_N_RD(x)             (((x)>>7) & 0x01)
#define TDA20152_PLL_TEST1_DSM_INT_MODE_EN_RD(x)         (((x)>>6) & 0x01)
#define TDA20152_PLL_TEST1_DSM_CLKB_CTRL_RD(x)           (((x)>>5) & 0x01)
#define TDA20152_PLL_TEST1_DSM_DITH_MODE_RD(x)           (((x)>>3) & 0x03)
#define TDA20152_PLL_TEST1_DSM_TEST_CLK_SEL_RD(x)        (((x)>>2) & 0x01)
#define TDA20152_PLL_TEST1_DSM_TEST_CLK_RD(x)            (((x)>>1) & 0x01)
#define TDA20152_PLL_TEST1_DSM_MASH11_MODE_EN_RD(x)      ((x) & 0x01)
#define TDA20152_PLL_TEST2_PLL_LOCK_DET_RD(x)            (((x)>>7) & 0x01)
#define TDA20152_PLL_TEST2_DSM_OUT_RD(x)                 ((x) & 0x7F)
#define TDA20152_PLL_TEST3_DSM_CKOUT_EN_RD(x)            (((x)>>7) & 0x01)
#define TDA20152_PLL_TEST3_MD_FDIV2_OUT_EN_RD(x)         (((x)>>6) & 0x01)
#define TDA20152_PLL_TEST3_PLL_CLKDIV_MUXOUT_EN_RD(x)    (((x)>>5) & 0x01)
#define TDA20152_PLL_TEST3_PLL_DSM_SPI_EN_RD(x)          ((x) & 0x01)
#define TDA20152_PLL1_DSM_INT_IN_RD(x)                   ((x) & 0xFF)
//#define TDA20152_PLL2_DSM_FRAC_IN_RD(x)                  (x)
#define TDA20152_PLL3_DSM_FRAC_IN_RD(x)                  ((x) & 0x00)
#define TDA20152_PLL4_PLL_DSM_FRAC_SEL_RD(x)             (((x)>>6) & 0x01)
#define TDA20152_PLL4_CP_FORCE_LOCK_RD(x)                (((x)>>5) & 0x01)
#define TDA20152_PLL4_EN_IDZ_RMCTRL_VIH_RD(x)            (((x)>>4) & 0x01)
#define TDA20152_PLL4_SEL_RM_IDZ_FREQ_VIH_RD(x)          (((x)>>2) & 0x03)
#define TDA20152_PLL4_SEL_RM_IDZ_ICP_VIH_RD(x)           ((x) & 0x03)
#define TDA20152_LO_CHAIN1_DBL_EN_RD(x)                  (((x)>>7) & 0x01)
#define TDA20152_LO_CHAIN1_DCC1_EN_RD(x)                 (((x)>>6) & 0x01)
#define TDA20152_LO_CHAIN1_DCC2_EN_RD(x)                 (((x)>>5) & 0x01)
#define TDA20152_LO_CHAIN1_DIV2_EN_RD(x)                 (((x)>>4) & 0x01)
#define TDA20152_LO_CHAIN1_INBUF_EN_RD(x)                (((x)>>3) & 0x01)
#define TDA20152_LO_CHAIN1_MAIN_DIV_EN_RD(x)             (((x)>>2) & 0x01)
#define TDA20152_LO_CHAIN1_OP_MUX_EN_RD(x)               (((x)>>1) & 0x01)
#define TDA20152_LO_CHAIN1_POLY_EN_RD(x)                 ((x) & 0x01)
#define TDA20152_LO_CHAIN2_DIV1_MODE3_RD(x)              (((x)>>7) & 0x01)
#define TDA20152_LO_CHAIN2_DIV2_MODE3_RD(x)              (((x)>>5) & 0x01)
#define TDA20152_LO_CHAIN2_OP_4TO7_SEL_RD(x)             (((x)>>3) & 0x01)
#define TDA20152_LO_CHAIN2_OP_DIV8_SEL_RD(x)             (((x)>>2) & 0x01)
#define TDA20152_LO_CHAIN2_OP_EXT2LO_SEL_RD(x)           (((x)>>1) & 0x01)
#define TDA20152_LO_CHAIN2_DCC1BPF_NOM_CURR_ENH_RD(x)    ((x) & 0x01)
#define TDA20152_LO_CHAIN3_CAP_EN_RD(x)                  (((x)>>7) & 0x01)
#define TDA20152_LO_CHAIN3_PREMMODE_EN_RD(x)             (((x)>>6) & 0x01)
#define TDA20152_LO_CHAIN3_DBL_HICUR_EN_RD(x)            (((x)>>5) & 0x01)
#define TDA20152_LO_CHAIN3_DCC1BPF_HICUR_EN_RD(x)        (((x)>>4) & 0x01)
#define TDA20152_LO_CHAIN3_DCC2BPF_HICUR_EN_RD(x)        (((x)>>3) & 0x01)
#define TDA20152_LO_CHAIN3_DCC1LIMIT_HICUR_EN_RD(x)      (((x)>>2) & 0x01)
#define TDA20152_LO_CHAIN3_DCC2LIMIT_HICUR_EN_RD(x)      (((x)>>1) & 0x01)
#define TDA20152_LO_CHAIN3_DIV2_HICUR_EN_RD(x)           ((x) & 0x01)
#define TDA20152_LO_CHAIN4_MAINDIV_HICUR_EN_RD(x)        (((x)>>7) & 0x01)
#define TDA20152_LO_CHAIN4_INBUF_HICUR_EN_RD(x)          (((x)>>6) & 0x01)
#define TDA20152_LO_CHAIN4_OUTMUX_HICUR_EN_RD(x)         (((x)>>5) & 0x01)
#define TDA20152_LO_CHAIN4_POLY_HICUR_EN_RD(x)           (((x)>>4) & 0x01)
#define TDA20152_LO_CHAIN4_LOMUX_EN_RD(x)                (((x)>>3) & 0x01)
#define TDA20152_LO_CHAIN4_DIV8_HUICUR_ENH_RD(x)         (((x)>>2) & 0x01)
#define TDA20152_LO_CHAIN4_FINALEF_HICUR_ENH_RD(x)       (((x)>>1) & 0x01)
#define TDA20152_LO_CHAIN4_LOMUX_HICUR_EN_RD(x)          ((x) & 0x01)
#define TDA20152_LO_TEST_LOTEST_MEAS_SEL_RD(x)           (((x)>>7) & 0x01)
#define TDA20152_LO_TEST_LOTEST_EN_RD(x)                 (((x)>>6) & 0x01)
#define TDA20152_LO_TEST_LOTEST_INJECT_EN_RD(x)          (((x)>>5) & 0x01)
#define TDA20152_LO_TEST_LOTEST_MEAS_INJ_HICUR_RD(x)     (((x)>>3) & 0x03)
#define TDA20152_LO_TEST_LOTEST_EXTLO_SEL_RD(x)          (((x)>>2) & 0x01)
#define TDA20152_LO_TEST_LOTEST_D2S2D_HICUR_RD(x)        ((x) & 0x03)
#define TDA20152_DIG_LDO_LDO_EN_RD(x)                    (((x)>>7) & 0x01)
#define TDA20152_DIG_LDO_LDO_V_RD(x)                     (((x)>>5) & 0x03)

/*****************************************************************************/
/* STRUCTURE DEFINITION                                                      */
/*****************************************************************************/
typedef TDA20152_SAT_TUNER_COMMUNICATION TDA20152_COMMUNICATION;
typedef TDA20152_SAT_TUNER_CHANNEL TDA20152_TUNER_CHANNEL;

/******************************************************************************
 * POWER DETECTOR SHIFT SETTING
 *     register values are:
 *         -2.5dB        0x00
 *         0dB (default) 0x01
 *         +2.5dB        0x02
 *         +5dB          0x03
 ******************************************************************************/
typedef enum _TDA20152_pwr_detect_shift_
{
    TDA20152_PWR_DETECT_SHIFT_BY_NEGATIVE_2_5_DB = 0x00,
    TDA20152_PWR_DETECT_SHIFT_BY_0_DB            = 0x01,
    TDA20152_PWR_DETECT_SHIFT_BY_2_5_DB          = 0x02,
    TDA20152_PWR_DETECT_SHIFT_BY_5_DB            = 0x03,
    TDA20152_PWR_DETECT_SHIFT_INVALID                     /* boundary value. should never be reached*/
}   TDA20152_PWR_DETECT_SHIFT;

/******************************************************************************
 *  POWER DETECTOR MEASURE
 *   register values are:
 *       < -45           0 0000 0000    0x000
 *       -45 ~ -40       0 0000 0001    0x001
 *       -40 ~ -35       0 0000 0011    0x003
 *       -35 ~ -30       0 0000 0111    0x007
 *       -30 ~ -25       0 0000 1111    0x00F
 *       -25 ~ -20       0 0001 1111    0x01F
 *       -20 ~ -15       0 0011 1111    0x03F
 *       -15 ~ -10       0 0111 1111    0x07F
 *       -10 ~ -5        0 1111 1111    0x0FF
 *       > -5            1 1111 1111    0x1FF
 *
 * following values match with register values
 * except the last one
 *******************************************************************************/
typedef enum _TDA20152_pwr_detect_therm_val_
{
    TDA20152_PWR_DETECT_THERM_VAL_BELOW_45          = 0x000,
    TDA20152_PWR_DETECT_THERM_VAL_BETWEEN_45_AND_40 = 0x001,
    TDA20152_PWR_DETECT_THERM_VAL_BETWEEN_40_AND_35 = 0x003,
    TDA20152_PWR_DETECT_THERM_VAL_BETWEEN_35_AND_30 = 0x007,
    TDA20152_PWR_DETECT_THERM_VAL_BETWEEN_30_AND_25 = 0x00F,
    TDA20152_PWR_DETECT_THERM_VAL_BETWEEN_25_AND_20 = 0x01F,
    TDA20152_PWR_DETECT_THERM_VAL_BETWEEN_20_AND_15 = 0x03F,
    TDA20152_PWR_DETECT_THERM_VAL_BETWEEN_15_AND_10 = 0x07F,
    TDA20152_PWR_DETECT_THERM_VAL_BETWEEN_10_AND_5  = 0x0FF,
    TDA20152_PWR_DETECT_THERM_VAL_ABOVE_5           = 0x1FF,
    TDA20152_PWR_DETECT_THERM_VAL_INVALID           = 0x3FF
} TDA20152_PWR_DETECT_THERM_VAL;



/******************************************************************************
 *  DC_OFFSET_REGISTER
 *
 *     this is not defined in the ICS TO DO
 *
 * following values match with register values
 * except the last one
 *******************************************************************************/
typedef enum _TDA20152_dc_offset_correction_
{
    TDA20152_DC_OFFSET_CORRECTION_2_0KOHM  = 0x00,
    TDA20152_DC_OFFSET_CORRECTION_3_9KOHM  = 0x01,
    TDA20152_DC_OFFSET_CORRECTION_5_7KOHM  = 0x02,
    TDA20152_DC_OFFSET_CORRECTION_7_6KOHM  = 0x03,
    TDA20152_DC_OFFSET_CORRECTION_9_3KOHM  = 0x04,
    TDA20152_DC_OFFSET_CORRECTION_11_2KOHM = 0x05,
    TDA20152_DC_OFFSET_CORRECTION_13_0KOHM = 0x06,
    TDA20152_DC_OFFSET_CORRECTION_15_0KOHM = 0x07,
    TDA20152_DC_OFFSET_CORRECTION_INVALID
}   TDA20152_DC_OFFSET_CORRECTION;


/******************************************************************************
 * Gain Mode
 *   Define in which gain mode the result of RF PWR DET is located
 ******************************************************************************/
typedef enum _TDA20152_gain_mode_
{
    TDA20152_GAIN_MODE_0 = 0x00,
    TDA20152_GAIN_MODE_1 = 0x01,
    TDA20152_GAIN_MODE_2 = 0x02,
    TDA20152_GAIN_MODE_3 = 0x03,
	TDA20152_GAIN_MODE_INVALID
} TDA20152_GAIN_MODE;

/******************************************************************************
 * FltA Bandwidth
 *      following are register settings
 *          352MHz  = 0x00
 *      primary settings
 *          25MHz   = 0x0F
 *          35MHz   = 0x0C
 *          45MHz   = 0x0A
 *          70MHz   = 0x08
 *      secondary settings
 *          27MHz   = 0x0E
 *          31MHz   = 0x0D
 *          38MHz   = 0x0B
 *          57MHz   = 0x09
 ******************************************************************************/
typedef enum _TDA20152_flta_bw_
{
    TDA20152_FLTA_BW_352MHZ           = 0x00,
    TDA20152_FLTA_BW_PRIMARY_25MHZ    = 0x0F,
    TDA20152_FLTA_BW_PRIMARY_35MHZ    = 0x0C,
    TDA20152_FLTA_BW_PRIMARY_45MHZ    = 0x0A,
    TDA20152_FLTA_BW_PRIMARY_70MHZ    = 0x08,
    TDA20152_FLTA_BW_SECONDARY_27MHZ  = 0x0E,
    TDA20152_FLTA_BW_SECONDARY_31MHZ  = 0x0D,
    TDA20152_FLTA_BW_SECONDARY_38MHZ  = 0x0B,
    TDA20152_FLTA_BW_SECONDARY_57MHZ  = 0x09
} TDA20152_FLTA_BW;

/******************************************************************************
 * Configuration structure
 ******************************************************************************/

typedef enum _TDA20152_tuner_ref_clkout_div_
{
    TDA20152_TUNER_REF_CLOCKOUT_DIV1 = 0,
    TDA20152_TUNER_REF_CLOCKOUT_DIV2 = 1,
    TDA20152_TUNER_REF_CLOCKOUT_DIV_UNDEF
} TDA20152_TUNER_REF_CLOCKOUT_DIV;

/******************************************************************************
 * Tuner configuration
 *      chip_type
 *      crystal_frequency_hz
 *          XTAL on the board
 *      tuner_ref_clkout_div
 *          only one tuner for blackwidow
 *          so remove the association
 *          but keep the parameter that was present in the structure
 ******************************************************************************/
typedef struct _TDA20152_config_
{
    //TDA20152_SAT_TUNER_CHIP_TYPE                 eChipType;
    unsigned long                       ulCrystalFrequencyHz;
} TDA20152_CONFIG;


/******************************************************************************
 * LO divider ratio
 *    indicate which Divider ratio is used for LO
 ******************************************************************************/
typedef enum _TDA20152_lo_divider_
{
    TDA20152_LO_DIVIDER_4,
    TDA20152_LO_DIVIDER_5,
    TDA20152_LO_DIVIDER_6,
    TDA20152_LO_DIVIDER_7,
    TDA20152_LO_DIVIDER_8,
    TDA20152_LO_DIVIDER_INVALID
}TDA20152_LO_DIVIDER;


/******************************************************************************
 * Tuner Status
 *      ulBiasValue
 *          Bias value
 *      bCalibrationDone
 *          Calibration finished
 *      AlcAmplitudeStable
 *          indicates amplitude change
 *      eVpdAmplitude
 *          V power detector amplitude
 *      bCalibrationOvertime
 *          indicates whether calibration has been done over 16 ticks
 *      bCalibrationHitLimit
 *          calibration hits counter limit
 *      VTuneAboveThreshold
 *          Vtune is higher then specified threshold at high side
 *      VTuneBelowThreshold
 *          Vtune is lower then specified threshold at high side
 *      eVcoEnabled
 *          which VCO is enabled
 *            | VCO B | VCO A |
 *          0 |       |   x   |
 *          1 |   x   |       |
 *      bVTuneMargin
 *          Vtune is in the right range to provide margin
 *      PLL Status
 *          indicate whether PLL is locked or not
 ******************************************************************************/
typedef struct _TDA20152_status_
{
    BOOL                      bCalibrationDone;
    BOOL                      bAlcAmplitudeStable;
    TDA20152_VPD_AMPLITUDE  eVpdAmplitude;
    BOOL                      bCalibrationOvertime;
    BOOL                      bCalibrationHitLimit;
    BOOL                      bVTuneAboveThreshold;
    BOOL                      bVTuneBelowThreshold;
    TDA20152_VCO            eVcoEnabled;
    BOOL                      bVTuneMargin;
    BOOL                      bPllLocked;
    /*  */
    TDA20152_LO_DIVIDER     eLoDividerRatio;

    /* debug purpose */
#ifdef FOR_DEBUG_PURPOSE
    unsigned long             ulLoFreqkHz;
    unsigned long             ulVcoFreqkHz;
    unsigned long             ulNFloat;
    unsigned long             DsmIntInReg;
    unsigned long             DsmIntInRegCheck;
    unsigned long             DsmFracInReg;
    unsigned long             DsmFracInRegCheck;
    unsigned long             ulR;
    unsigned long             ulPremain;
    unsigned long             ulTiming;
#endif

} TDA20152_STATUS;



/*****************************************************************************/
/* DEFINED                                                                   */
/*****************************************************************************/

#define MULT_MASK 0x01
#define SHIFT_MASK 0x7FFFFFFF
#define MAX_VALUE 0xFFFFFFFF
#define INTEGER_SIZE 32

/*****************************************************************************/
/* LOCAL VARIABLE                                                            */
/*****************************************************************************/


/*****************************************************************************/
/* GLOBAL VARIABLE                                                           */
/*****************************************************************************/

/*****************************************************************************/
/* STATIC VARIABLES                                                          */
/*****************************************************************************/



AVL_DVBSx_ErrorCode ExtTDA20152_Initialize(struct AVL_Tuner * pTuner);
AVL_DVBSx_ErrorCode ExtTDA20152_GetLockStatus(struct AVL_Tuner * pTuner );
AVL_DVBSx_ErrorCode ExtTDA20152_Lock(struct AVL_Tuner * pTuner);


#endif
