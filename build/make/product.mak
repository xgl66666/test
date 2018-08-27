# Available PRODUCT_INSTALLATION
# NO_INSTALLATION_TS        No Advanced installation mode available
#

# PRODUCT_ANTICLONING_ENABLED
# NO (default)
# YES

# Available PRODUCT_FEATURE_PVR
# PVR_OFF (default)            
# PVR_INTERNAL
# PVR_EXTERNAL
# PVR_BOTH
# PVR_INTERNAL_FREEVIEW
# PVR_EXTERNAL_FREEVIEW
# PVR_EXTERNAL_FAKE_FREEVIEW
# PVR_BOTH_FREEVIEW
#

# Available PRODUCT_DUAL_DECODING
# DUAL_DECODING_OFF (default)
# DUAL_DECODING_ON
#

# Available PRODUCT_MEDIUM
# MEDIUM_SAT
# MEDIUM_TER
# MEDIUM_CAB
#

# Available PRODUCT_BROADCAST
# BROADCAST_DVB
# BROADCAST_ATSC
#

# Available PRODUCT_ENGINE
# ENGINE_TER_MHEG
# ENGINE_WOTV

# Available PRODUCT_MHEG_DEFAULT_OSD_MODE
# CLUT (default)
# TRUE_COLOR

# Available PRODUCT_DSMCC_STACK value
# DSMCC_STACK_ON


# Available PRODUCT_TELETEXT_OSD
# TTXT_OSD_OFF (default)
# TTXT_OSD_ON
# TTXT_OSD_ON_EXTERNAL
#

# Available PRODUCT_TELETEXT_VBI
# TTXT_VBI_OFF (default)
# TTXT_VBI_ON
#

# Available PRODUCT_SUBTITLE
# SUBT_OFF (default)
# SUBT_ON
#

# Available PRODUCT_MOTOR
# MOTOR_OFF
# MOTOR_ON
#

# Available PRODUCT_TRIGGER
# TRIGGER_OFF
#                   or all combination allowed :
# DVBSSU
# DVBSSUENHANCED
# DTG
# IDT
# USB
# SERIAL
# IRDCMD

# Available PRODUCT_BOOTLOADER
# BOOTLOADER_OFF
# BOOTLOADER_BROADCAST
# BOOTLOADER_USB
# BOOTLOADER_SERIAL
#                   or all combination allowed :
# BOOTLOADER_SERIAL_BROADCAST_USB

# Available PRODUCT_DOWNLOADER
# DOWNLOADER_OFF
# DOWNLOADER_BROADCAST
# DOWNLOADER_USB
# DOWNLOADER_SERIAL
#                   or all combination allowed :
# DOWNLOADER_SERIAL_BROADCAST_USB
#

# Product CA library
#
# The product defaults to using the CAK "Merlin" Library
# Use the setenv batch(es) to customize your product
# to at a finer level.
#
# PRODUCT_USE_CA
# YES             (default)
# NO
#
# PRODUCT_CCAS
# NAGRA_TIGER
# NAGRA_MERLIN    (default)
# NAGRA_DALTS
#

# PRODUCT_USE_CI
# YES             
# NO              (default)

# PRODUCT_USE_PPV
# YES             
# NO              (default)

# PRODUCT_USE_CA_SECURE_CHIPSET
# YES             
# NO              (default)

# PRODUCT_AC3_DURING_TIMERS
# YES             (default)
# NO

# PRODUCT_TIMER_ON
# YES
# NO              (default)

# Available PRODUCT_BOOTLOADER_INTEGRITY_CHECK
# CHECK_SOFTWARE_IMAGE_INTEGRITY
# DO_NOT_CHECK_INTEGRITY

# Available PRODUCT_BOOTLOADER_FORCED_MODE
# NO              (default)
# YES

# Available PRODUCT_BOOTLOADER_KEY_TYPE
# IWEDIA_KEY
# CRYPTO_TEST_KEY
# CRYPTO_PRODUCT_KEY

# Available PRODUCT_ARCH
# HPIForMojo
# HPIForYoda
# Linux
# ST5100
# ST5105
# ST5517
# ST7100
# ST7710
# ST7109
# ST_5162 (default)
# X86_LINUX
# ATOM_LINUX

# Available PRODUCT_EIT
# NO_EIT
# EIT_ONLY_PF
#

# Available PRODUCT_EIT_FULL_CACHING
# YES
# NO  (default)
#

# Available PRODUCT_LCN
# LCN_NO										LCN is not managed
# LCN_YES										LCN is managed depending the selected country
#

# Available PRODUCT_AUDIO_AC3
# AUDIO_AC3_NONE							AC3 is not managed and therefore not proposed to user
# AUDIO_AC3_ALWAYS							AC3 is always proposed to user and automatically started
# AUDIO_AC3_USERCHOICE_MODE1				AC3 is an IHM user choice :
#												If OFF 	: not started automatically and not proposed to user
#												if ON	: automatically started and proposed to user
# AUDIO_AC3_USERCHOICE_MODE2				AC3 is an IHM user choice :
#												If OFF 	: not started automatically BUT proposed to user
#												if ON	: automatically started and proposed to user
#

# Available PRODUCT_AUDIO_LANGUAGES
# AUDIO_LANGUAGES_ONLY_ISO639				Only managed audio tracks that have ISO639 languages
# AUDIO_LANGUAGES_ALL						Managed all audio tracks
#

# Available PRODUCT_SUBTI_LANGUAGES | PRODUCT_TTXT_LANGUAGES
# LANGUAGES_ONLY_ISO639                           Only managed tracks that have ISO639 languages
# LANGUAGES_ALL                                           Managed all tracks

# Available PRODUCT_AUDIO_TRACK_NAMED_DD
#			Some audio tracks have a no-ISO639 language called dd
# AUDIO_TRACKDD_NO_SPECIAL_MANAGEMENT				these tracks are automatically selected if no default languages are found (and is the first of AC3 tracks)
# AUDIO_TRACKDD_HAS_PRIORITY						these tracks are automatically selected if no AC3 tracks with default languages are found
#			Warning : Of course only if PRODUCT_AUDIO_LANGUAGES=AUDIO_LANGUAGES_ALL
#

# Available PRODUCT_AUDIO_VOLUME
# AUDIO_VOLUME_DONEBY_DECODER                  	Audio volume manage by the decoder
# AUDIO_VOLUME_DONEBY_AVS                   	Audio volume manage by the AVswitch

# Available PRODUCT_AUDIO_VOLUME_LEVEL_MODE
# YES                                        A specific volume level can be selected for each service
#

# PRODUCT_USE_TDAL_TS
# USE_TDAL_TS_OFF (default)
# USE_TDAL_TS_ON
#

# PRODUCT_USE_RPC
# YES             only for OnBoard module
# NO              (default)

# PRODUCT_USE_RPC_PROTOCOL
# YES             serial port remote control
# NO              (default) infra-red remote control  		

# PRODUCT_USE_FP
# YES             for board with Front panel
# NO              (default) no Front panel  		

# PRODUCT_USE_DMJFS
# YES             Use the data manager JFS package, necessary with CAK DLK (default)
# NO              Use old comedia data manager  		


# PRODUCT_EMBEDDED
# YES             only for Comedia without Maestro
# NO              (default) Comedia with Maestro

# Use automatic update in stand-by with PRODUCT_AUTOMATIC_UPDATE
# AUTOMATIC_UPDATE_AVAILABLE
# AUTOMATIC_UPDATE_NOT_AVAILABLE

# PRODUCT_SLIST_FS_COMPRESSION_LEVEL
# Compression Level of fav lists in File System
#	0 : No compression
#	1 to 9 :  Best speed to Best compression
#	if not defined : Z_DEFAULT_COMPRESSION (6)

# PRODUCT_DSMCC
#   DSMCC_ON                       DSMCC package is used
#   DSMCC_OFF     (default)

# PRODUCT_MAESTRO_PORTAL
#   PORTAL_ON
#   PORTAL_OFF    (default)

#	PRODUCT_HD
# YES             
# NO              (default)

#	PRODUCT_MEDIA_VIEWER
# YES             
# NO              (default)

#	PRODUCT_MEDIA_VIEWER_SRC
# USB
# UPNP
#   or all combination allowed :
# USB_UPNP

#	PRODUCT_MEDIA_VIEWER_MEDIATYPE
# PICTURE
# AUDIO
# AV
#   or all combination allowed :
# PICTURE_AUDIO

#	PRODUCT_MEDIA_VIEWER_PICTURE_RDR_LAYER_TYPE
# GFX
# AV		  (default)

#	PRODUCT_MEDIA_VIEWER_PICTURE_RDR_JPEG_COLOR_TYPE
#   YUV444		(default)
#   RGB		  

#   PRODUCT_USE_XPEL=YES
# YES             
# NO              (default)

#	PRODUCT_SDT_MONITORING
# YES             
# NO              (default)

#	PRODUCT_NB_TUNERS 
#        (no default value)
# 1
# 2 

# PRODUCT_TUNER
# STAPI (default)
# DVBLINUX
# STREAMER

#   PRODUCT_BINARY_FILE_COMPRESSED
# YES
# NO


# PRODUCT_PERMANENT_SI_MONITORING
# YES
# NO

# Available PRODUCT_USE_VIRT_KEYBOARD
# NO                                        Virtual keyboard is not used (default)
# YES                                       Virtual keyboard is used
#


# PRODUCT_FONT_ENGINE
# FONT_ENGINE_FREETYPE
# FONT_ENGINE_CDAC

##################################################
# Note that most product characteristics may be
# overriden in your environment configuration.
##################################################

ifeq "$(PLATFORM_CHIP_VENDOR)" "mstar"
PRODUCT_USE_MSTAR ?= YES
CFLAGS += -DPRODUCT_USE_MSTAR
endif

ifeq "$(PLATFORM_CHIP_VENDOR)" "st"
PRODUCT_USE_ST ?= YES
CFLAGS += -DPRODUCT_USE_ST
endif

ifeq "$(PRODUCT_ARCH)" "pthreads"
CFLAGS += -DPRODUCT_PC_SIM
endif

# product_use_maestro  is variable
ifeq "$(PRODUCT_APP)" "MAESTRO"
#PRODUCT_APP=MAESTRO
#PRODUCT_USE_MAESTRO ?= YES   
CFLAGS += -DPRODUCT_USE_MAESTRO
endif
# product_use_celine  is variable
ifeq "$(PRODUCT_APP)" "CELINE"
#PRODUCT_APP=CELINE
#PRODUCT_USE_CELINE ?= YES   
CFLAGS += -DPRODUCT_USE_CELINE
endif


# Platform Architecture
PRODUCT_ARCH ?= MS5017
# CHAL Implementation
PRODUCT_PREBUILT_HAL ?= NO

# Defaults for Conditional Access
ifeq "$(PRODUCT_USE_CA)" ""
PRODUCT_USE_CA	= YES
PRODUCT_CCAS	= NAGRA_MERLIN
endif

ifeq "$(PRODUCT_USE_CA)" "YES"
	ifeq "$(PRODUCT_USE_PVR_SEC)" "YES"
	PVR_ENCRYPTION_ENABLED = YES
	endif
endif
USE_DLK_1_1	?= NO

# Version of CAK libraries
# Possible values: DEBUG, RELEASE, PRODUCTION
PRODUCT_CAK_LIBVER ?= DEBUG

# Non-overridable specifics for MHEG
ifeq "$(MHEG5_VERSION)" "mheg5"
PRODUCT_ENGINE					= ENGINE_TER_MHEG
PRODUCT_DSMCC_STACK				= DSMCC_STACK_ON
PRODUCT_MHEG_DEFAULT_OSD_MODE	= CLUT
#else
#PRODUCT_TELETEXT_VBI			= TTXT_VBI_OFF
#PRODUCT_TELETEXT_OSD			= TTXT_OSD_ON
endif

# Hardware
PRODUCT_HD                       ?= YES
PRODUCT_NB_TUNERS                ?= 1
PRODUCT_TUNER                    ?= STAPI
PRODUCT_USE_TDAL_TS              ?= USE_TDAL_TS_ON
PRODUCT_USE_TDAL_TS_SOURCE_SINK  ?= NO
PRODUCT_USE_FP                   ?= NO
PRODUCT_USE_DMJFS                ?= YES
PRODUCT_DMJFS_IN_RAM             ?= YES
PRODUCT_DMJFS_ON_NAND            ?= NO
PRODUCT_USE_HDCP	             ?= NO
PRODUCT_CONCURRENT_KEYBOARDS_NUM ?= 1

PRODUCT_AVS                      ?= AVS_ST
PRODUCT_AUDIO_VOLUME             ?= AUDIO_VOLUME_DONEBY_DECODER
PRODUCT_ANTICLONING_ENABLED      ?= YES
PRODUCT_AUX_VIDEO_DISPLAY_SD     ?= YES
DISABLE_WSS                      ?= YES

# Broadcast
PRODUCT_BROADCAST		?= BROADCAST_DVB
PRODUCT_MEDIUM			?= MEDIUM_CAB
PRODUCT_LCN			?= LCN_YES
PRODUCT_SDT_MONITORING		?= YES
PRODUCT_PERMANENT_SI_MONITORING	?= NO

ifeq "$(PRODUCT_LCN)" "LCN_YES"
	PRODUCT_LCN_IN_FAV = LCN_IN_FAV_YES
else
	PRODUCT_LCN_IN_FAV = LCN_IN_FAV_NO
endif

# downloader
PRODUCT_DOWNLOADER?=DOWNLOADER_BROADCAST
PRODUCT_DOWNLOADER_USE_CFGFILE?=YES

# Bootloader 
PRODUCT_BOOTLOADER?=BOOTLOADER_OFF
PRODUCT_BOOTLOADER_INTEGRITY_CHECK?=DO_NOT_CHECK_INTEGRITY
PRODUCT_BOOTLOADER_FORCED_MODE?=NO
PRODUCT_BOOTLOADER_KEY_TYPE?=IWEDIA_KEY

# OTA(Over-The-Air)
PRODUCT_USE_OTA?=NO
PRODUCT_USE_TDAL_OTC_INIT?=NO
ifeq "$(PRODUCT_USE_TDAL_OTC_INIT)" "YES"
USE_TDAL_OTC_INIT=1
else
USE_TDAL_OTC_INIT=0
endif
CFLAGS += -DUSE_TDAL_OTC_INIT


# Table
PRODUCT_BAT	?= YES

#Genre carier
PRODUCT_BAT_GENRES	?= YES

# Trigger
PRODUCT_TRIGGER?=DVBSSU
ifneq "$(PRODUCT_TRIGGER)" "TRIGGER_OFF"
PRODUCT_TRIGGER_MAX_REFUSAL=3
PRODUCT_AUTOMATIC_UPDATE=AUTOMATIC_UPDATE_AVAILABLE
endif

# Teletext and Subtitles
PRODUCT_SUBTITLE		?= SUBT_ON
PRODUCT_SUBTI_LANGUAGES		?= LANGUAGES_ALL
PRODUCT_TTXT_LANGUAGES		?= LANGUAGES_ALL

# Event Manager
PRODUCT_EIT_FULL_CACHING	?= YES
PRODUCT_EPG_GRID		?= YES
# EIT memory management BGET or DLMALLOC
PRODUCT_EIT_MEM_ALLOCATOR ?= BGET

# Audio
PRODUCT_AUDIO_AC3		?= AUDIO_AC3_USERCHOICE_MODE2
PRODUCT_AUDIO_LANGUAGES		?= AUDIO_LANGUAGES_ALL
PRODUCT_AUDIO_TRACK_NAMED_DD	?= AUDIO_TRACKDD_NO_SPECIAL_MANAGEMENT
PRODUCT_AC3_DURING_TIMERS	?= NO

# Recording
PRODUCT_USE_XPEL 		?= YES
PRODUCT_TIMER_ON 		?= YES

# Miscellaneous
PRODUCT_BINARY_FILE_COMPRESSED 	?= YES
PRODUCT_USE_LABEL_ANIMATION ?= YES

# NX 2 UI
PRODUCT_USE_NX2_UI ?= YES

# Freetype2
USE_FREETYPE2 ?= YES

# ONID filtering
PRODUCT_INSTALL_ONLY_CURRENT_COUNTRY ?= NO

# Use software lock of flash blocks
PRODUCT_USE_LOCK_SPLS ?= YES

# Media Viewer
PRODUCT_MEDIA_VIEWER ?= YES
PRODUCT_USE_MEDIA_BROWSER ?= YES
PRODUCT_MEDIA_VIEWER_SRC ?= USB
PRODUCT_MEDIA_VIEWER_MEDIATYPE ?= PICTURE_AUDIO_AV
PRODUCT_MEDIA_VIEWER_ON_LAYER_GFX ?= YES

#Font engine
PRODUCT_FONT_ENGINE ?= FONT_ENGINE_FREETYPE

#------------------------------------------
# TABLE package flag
#------------------------------------------
ifeq "$(PRODUCT_BAT)" "YES"
CFLAGS += -DBAT_SEARCH
endif
ifeq "$(PRODUCT_BROADCAST)" "BROADCAST_DVB"
_TABLE_DVB_=1
CFLAGS += -D_TABLE_DVB_
endif
ifeq "$(PRODUCT_BROADCAST)" "BROADCAST_ATSC"
_TABLE_ATSC_=1
CFLAGS += -D_TABLE_ATSC_
endif

# Virtual keyboard 
PRODUCT_USE_VIRT_KEYBOARD ?= YES

# Operator logo descriptor
OPERATOR_LOGO_DESCRIPTOR ?= YES

#GHITS custom service order for genres
CUSTOM_ORDER ?= YES
#NVOD
PRODUCT_NVOD ?= NO

# Subcription rights enable
SUBSCRIPTION_RIGHTS ?= NO

#EIT Image Icon Parser
PRODUCT_EIT_IMAGE_ICON_PARSER ?= NO

# EIT 24h of scheduled events in info banner
PRODUCT_EIT_24H_SCH_INFO_BANNER ?= YES

# avoid restarting stb when going to standby
PRODUCT_NORESTART_STANDBY ?= YES

# include external libraries within middleware library
export PRODUCT_LIBMW_INCLUDE_EXTERNLIBS ?= YES

PRODUCT_USE_FAST_ZAPP_THREAD ?= YES

#software bitmap scaling
ifeq "$(PRODUCT_APP)" "MAESTRO"
PRODUCT_SOFTWARE_BITMAP_SCALING ?= YES
endif
ifeq "$(PRODUCT_APP)" "CELINE"
PRODUCT_SOFTWARE_BITMAP_SCALING ?= NO
endif
