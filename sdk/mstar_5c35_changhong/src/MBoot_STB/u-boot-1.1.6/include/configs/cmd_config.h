////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2008 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (¡§MStar Confidential Information¡¨) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __CMD_CONFIG_H__
#define __CMD_CONFIG_H__

//#include <cmd_confdefs.h>
#include "uboot_module_config.h"

#if (ENABLE_MODULE_MODEM)
#define MODULE_MODEM	CFG_CMD_LOADS | CFG_CMD_LOADB
#else
#define MODULE_MODEM	CFG_CMD_DUMMY
#endif

#if (ENABLE_MODULE_IMAGE)
#define MODULE_IMAGE	CFG_CMD_IMI | CFG_CMD_IMLS | CFG_CMD_ELF
#else
#define MODULE_IMAGE	CFG_CMD_DUMMY
#endif

#if (ENABLE_MODULE_NET)
#if (ENABLE_MODULE_LWIP)
#define MODULE_NET		CFG_CMD_NET | CFG_CMD_PING | CFG_CMD_DHCP | CFG_CMD_LWIP
#else
#define MODULE_NET		CFG_CMD_NET | CFG_CMD_PING | CFG_CMD_DHCP
#endif
#else
#define MODULE_NET		CFG_CMD_DUMMY
#endif

#if (ENABLE_MODULE_I2C)
#define MODULE_I2C		CFG_CMD_I2C
#else
#define MODULE_I2C		CFG_CMD_DUMMY
#endif

#if (ENABLE_MODULE_SPI)
#define MODULE_SPI		CFG_CMD_SPI
#else
#define MODULE_SPI		CFG_CMD_DUMMY
#endif

#if (ENABLE_MODULE_MICOM)
#define MODULE_MICOM	CFG_CMD_MICOM
#else
#define MODULE_MICOM	CFG_CMD_DUMMY
#endif

#if (ENABLE_MODULE_USB)
#define MODULE_USB		CFG_CMD_USB
#else
#define MODULE_USB		CFG_CMD_DUMMY
#endif

#if (ENABLE_MODULE_FAT)
#define MODULE_FAT		CFG_CMD_FAT
#else
#define MODULE_FAT		CFG_CMD_DUMMY
#endif

#if (ENABLE_MODULE_NAND_FLASH)
#define MODULE_NAND	CFG_CMD_NAND
#else
#define MODULE_NAND	CFG_CMD_DUMMY
#endif

#if (ENABLE_MODULE_FLASH)
#define MODULE_FLASH	CFG_CMD_FLASH
#else
#define MODULE_FLASH	CFG_CMD_DUMMY
#endif

#if (ENABLE_MODULE_SPI || ENABLE_MODULE_NAND_FLASH)
#define MODULE_ENV	CFG_CMD_ENV
#else
#define MODULE_ENV	CFG_CMD_DUMMY
#endif


#if (ENABLE_MODULE_JFFS2)
#define MODULE_JFFS2 CFG_CMD_JFFS2
#else
#define MODULE_JFFS2 CFG_CMD_DUMMY
#endif

#if (ENABLE_MODULE_UBI)
#define MODULE_UBI CFG_CMD_UBI
#else
#define MODULE_UBI CFG_CMD_DUMMY
#endif

#if (ENABLE_MODULE_UBI_FS)
#define MODULE_UBI_FS CFG_CMD_UBIFS
#else
#define MODULE_UBI_FS CFG_CMD_DUMMY
#endif

#if (ENABLE_MODULE_ENV_IN_UBI)
#define MODULE_ENV_IN_UBI CFG_CMD_ENV_IN_UBI
#else
#define MODULE_ENV_IN_UBI CFG_CMD_DUMMY
#endif

#if (ENABLE_MODULE_SECURITY_BOOT)
#define MODULE_SECURE CFG_CMD_SECURE
#else
#define MODULE_SECURE CFG_CMD_DUMMY
#endif

#if (ENABLE_MODULE_KEYPAD)
#define MODULE_KEYPAD CFG_CMD_KEYPAD
#else
#define MODULE_KEYPAD CFG_CMD_DUMMY
#endif

#if (ENABLE_MODULE_EEPROM)
#define MODULE_EEPROM CFG_CMD_EEPROM
#else
#define MODULE_EEPROM CFG_CMD_DUMMY
#endif

#if (ENABLE_MODULE_STANDBY)
#define MODULE_STANDBY CFG_CMD_STANDBY
#else
#define MODULE_STANDBY CFG_CMD_DUMMY
#endif

#if (ENABLE_MODULE_DISPLAY_LOGO)
#define MODULE_LOGO_DISP CFG_CMD_BOOTLOGO
#else
#define MODULE_LOGO_DISP CFG_CMD_DUMMY
#endif

#if (ENABLE_MODULE_USB_LAN)
    #if ((0==ENABLE_MODULE_USB) ||(0==ENABLE_MODULE_NET))
        #undef      ENABLE_USB_LAN_MODULE
        #error "ERROE! The USB and NET module must be also enabled for USB LAN feature..."
    #else
        #define     ENABLE_USB_LAN_MODULE
    #endif
#else
#undef      ENABLE_USB_LAN_MODULE
#endif


#if (ENABLE_MODULE_FRONT_END)
#define MODULE_TUNER CFG_CMD_TUNER
#define MODULE_DEMOD CFG_CMD_DEMOD
#define MODULE_DMX CFG_CMD_DMX
    #if (ENABLE_MODULE_TUNER_NUTUNE_FT2125)
        #ifdef FRONTEND_TUNER_TYPE
            #undef FRONTEND_TUNER_TYPE
        #endif
            #define FRONTEND_TUNER_TYPE NUTUNE_FT2125_TUNER
    #elif (ENABLE_MODULE_TUNER_NUTUNE_FJ2207)
        #ifdef FRONTEND_TUNER_TYPE
            #undef FRONTEND_TUNER_TYPE
        #endif
            #define FRONTEND_TUNER_TYPE NUTUNE_FJ2207_TUNER
    #elif (ENABLE_MODULE_TUNER_DIBCOM_DIB8096)
        #ifdef FRONTEND_TUNER_TYPE
            #undef FRONTEND_TUNER_TYPE
        #endif
            #define FRONTEND_TUNER_TYPE DIBCOM_DIB8096_TUNER
    #elif (ENABLE_MODULE_TUNER_NXP_CD1616LF)
        #ifdef FRONTEND_TUNER_TYPE
            #undef FRONTEND_TUNER_TYPE
        #endif
            #define FRONTEND_TUNER_TYPE NXP_CD1616LF_TUNER
    #elif (ENABLE_MODULE_TUNER_NXP_TD1611ALF)
        #ifdef FRONTEND_TUNER_TYPE
            #undef FRONTEND_TUNER_TYPE
        #endif
            #define FRONTEND_TUNER_TYPE NXP_TD1611ALF_TUNER
    #elif (ENABLE_MODULE_TUNER_MXL_MXL5007)
        #ifdef FRONTEND_TUNER_TYPE
            #undef FRONTEND_TUNER_TYPE
        #endif
            #define FRONTEND_TUNER_TYPE MXL_MXL5007_TUNER
    #elif (ENABLE_MODULE_TUNER_TDVG_H252F)
        #ifdef FRONTEND_TUNER_TYPE
            #undef FRONTEND_TUNER_TYPE
        #endif
            #define FRONTEND_TUNER_TYPE TDVG_H252F_TUNER
    #elif (ENABLE_MODULE_TUNER_MXL201RF)
        #ifdef FRONTEND_TUNER_TYPE
            #undef FRONTEND_TUNER_TYPE
        #endif
            #define FRONTEND_TUNER_TYPE TUNER_MXL201RF
    #endif

    #if (ENABLE_MODULE_DEMOD_MSTAR_MSB123x)
        #ifdef FRONTEND_DEMOD_TYPE
            #undef FRONTEND_DEMOD_TYPE
        #endif
            #define FRONTEND_DEMOD_TYPE MSTAR_MSB123x_DEMOD
    #elif (ENABLE_MODULE_DEMOD_MSTAR_MSB122x)
        #ifdef FRONTEND_DEMOD_TYPE
            #undef FRONTEND_DEMOD_TYPE
        #endif
             #define FRONTEND_DEMOD_TYPE MSTAR_MSB122x_DEMOD
    #elif (ENABLE_MODULE_DEMOD_LGDT3305)
        #ifdef FRONTEND_DEMOD_TYPE
            #undef FRONTEND_DEMOD_TYPE
        #endif
            #define FRONTEND_DEMOD_TYPE LGDT3305_DEMOD
    #elif (ENABLE_MODULE_DEMOD_DIBCOM_DIB8096)
        #ifdef FRONTEND_DEMOD_TYPE
            #undef FRONTEND_DEMOD_TYPE
        #endif
            #define FRONTEND_DEMOD_TYPE DIBCOM_DIB8096_DEMOD
    #elif (ENABLE_MODULE_DEMOD_URANUS4_INTERNAL)
        #ifdef FRONTEND_DEMOD_TYPE
            #undef FRONTEND_DEMOD_TYPE
        #endif
            #define FRONTEND_DEMOD_TYPE EMBEDDED_DVBC_DEMOD
    #elif (ENABLE_MODULE_DEMOD_KRONUS_INTERNAL | ENABLE_MODULE_DEMOD_KAISERIN_INTERNAL |ENABLE_MODULE_DEMOD_KAPPA_INTERNAL|ENABLE_MODULE_DEMOD_KRITI_INTERNAL)
        #ifdef FRONTEND_DEMOD_TYPE
            #undef FRONTEND_DEMOD_TYPE
        #endif
        #if (ENABLE_MODULE_TUNER_NXP_CD1616LF)
            #define FRONTEND_DEMOD_TYPE EMBEDDED_DVBC51_DEMOD
        #else
            #define FRONTEND_DEMOD_TYPE EMBEDDED_DVBT51_DEMOD
		#endif
		
	#elif 1
	    #if (ENABLE_HUMMINGBIRD_CUSTOMER)
	    
	    #ifdef FRONTEND_DEMOD_TYPE
            #undef FRONTEND_DEMOD_TYPE
        #endif
	    #define FRONTEND_DEMOD_TYPE MSTAR_MSB131X_DEMOD
		#define FRONTEND_TUNER_TYPE TUNER_AV2012
		
		#endif // end ENABLE_HUMMINGBIRD_CUSTOMER just for no warning
    #else
        #warning NoMatchFRONTEND
#endif

#else
#define MODULE_TUNER CFG_CMD_DUMMY
#define MODULE_DEMOD CFG_CMD_DUMMY
#define MODULE_DMX CFG_CMD_DUMMY
#endif

#if (ENABLE_MODULE_DEMO_CODE)
#define MODULE_DEMO_CODE CFG_CMD_DEMOCODE
#else
#define MODULE_DEMO_CODE CFG_CMD_DUMMY
#endif


#ifdef CONFIG_COMMANDS
#undef CONFIG_COMMANDS
#endif

#define CONFIG_COMMANDS ( \
					MODULE_MODEM	| \
					MODULE_IMAGE	| \
					MODULE_NET		| \
					MODULE_I2C		| \
					MODULE_SPI		| \
					MODULE_MICOM	| \
					MODULE_USB		| \
					MODULE_FAT		| \
					MODULE_JFFS2    | \
					MODULE_UBI      | \
					MODULE_UBI_FS   | \
					MODULE_ENV_IN_UBI   | \
					MODULE_NAND	    | \
					MODULE_FLASH	| \
					MODULE_ENV		| \
					MODULE_SECURE   | \
					MODULE_KEYPAD   | \
					MODULE_EEPROM   | \
					MODULE_STANDBY  | \
					MODULE_LOGO_DISP    | \
					MODULE_DEMOD    | \
					MODULE_TUNER    | \
					MODULE_DMX  | \
					MODULE_DEMO_CODE  | \
					CFG_CMD_CACHE   | \
					CFG_CMD_BDI     | \
					CFG_CMD_CONSOLE | \
					CFG_CMD_MEMORY  | \
					CFG_CMD_RUN)

#endif

