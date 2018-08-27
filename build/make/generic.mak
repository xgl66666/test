
CFLAGS += $(SDK_C_FLAGS) $(OS_C_FLAGS)

CXX_FLAGS += $(SDK_CXX_FLAGS) $(OS_CXX_FLAGS)

#SDK_C_FLAGS=
#SDK_CXX_FLAGS=
#SDK_LD_FLAGS=
#SDK_AS_FLAGS=
#SDK_INCLUDE_PATH=
#SDK_LIBS_PATH=
#SDK_LIBS= 

#OS_C_FLAGS=
#OS_CXX_FLAGS=
#OS_AS_FLAGS=
#OS_LD_FLAGS=
#OS_INCLUDE_PATH=
#OS_LIBS_PATH=
#OS_LIBS=

PRODUCT_NAME ?= comedia
PRODUCT_FLASH_TOOL_NAME ?= flash_tool
PRODUCT_FSTP_NAME ?= fstp
PRODUCT_TDAL_APP_TEST_NAME ?= tdal_app_test
PRODUCT_COMEDIA_TEST_NAME ?= comedia_test
PRODUCT_CHAL_VALIDATOR_NAME ?= chalvalidator
export CHAL_VALIDATOR_TARGET_NAME = $(PRODUCT_CHAL_VALIDATOR_NAME)
PRODUCT_PACK_TEST_NAME ?= pack_test
PRODUCT_DALTS_NAME ?= dalts
PRODUCT_APPDEMO_NAME ?= appdemo
PRODUCT_BOOTLOADER_NAME ?= bootloader

DIST_NAME?=dist
LIB_NAME?=libraries
BIN_NAME?=binaries
RES_NAME?=resources
SDK_NAME?=sdk

# ------ MAIN Settings -----------------
export PRODUCT_MAIN ?= main
export PRODUCT_MAIN_TEST_PVR_CORE ?= main_test_pvr_core
export PRODUCT_MAIN_TDAL_APP_TEST ?= main_tdal_app_test
export PRODUCT_MAIN_CHALVALIDATOR ?= main_chalvalidator
export PRODUCT_MAIN_FLASH_TOOL ?= main_flash_tool
export PRODUCT_MAIN_FSTP ?= main_fstp
export PRODUCT_MAIN_COMEDIA_TEST ?= main_comedia_test
export PRODUCT_MAIN_PACK_TEST ?= main_pack_test
export PRODUCT_MAIN_APPDEMO ?= main_appdemo
export PRODUCT_MAIN_BOOTLOADER ?= main_bootloader
export PRODUCT_MAIN_DALTS ?= main_dalts

PRODUCT_HAL_TYPE   ?= release
PRODUCT_HAL_SUFFIX ?=

ifeq "$(PROGTYPE)" "BOOTLOADER"

	ifeq "$(findstring USB, $(PRODUCT_BOOTLOADER))" "USB"
		PRODUCT_USE_TDAL_FS = YES
		PRODUCT_USE_USB = YES
	endif
	PRODUCT_PVR = PVR_OFF
else
# ****************************************
# PRODUCT_MEDIA_VIEWER
# ****************************************
ifeq "$(PRODUCT_MEDIA_VIEWER)" "YES"
	PRODUCT_USE_TDAL_MP = YES
	ifeq "$(findstring USB, $(PRODUCT_MEDIA_VIEWER_SRC))" "USB"
	PRODUCT_USE_TDAL_FS = YES
	PRODUCT_USE_USB = YES
	PACKAGE_FILE_SYSTEM = YES
    endif
	ifeq "$(findstring UPNP, $(PRODUCT_MEDIA_VIEWER_SRC))" "UPNP"
	endif
	ifeq "$(findstring PICTURE, $(PRODUCT_MEDIA_VIEWER_MEDIATYPE))" "PICTURE"
		PRODUCT_USE_SDEC = YES
	endif
	ifeq "$(findstring AUDIO, $(PRODUCT_MEDIA_VIEWER_MEDIATYPE))" "AUDIO"
		PRODUCT_USE_RDR_MP3 = YES
	endif
	ifeq "$(findstring AV, $(PRODUCT_MEDIA_VIEWER_MEDIATYPE))" "AV"
		PRODUCT_USE_AVP = YES
	endif
	ifeq "$(PRODUCT_MEDIA_VIEWER_ON_LAYER_GFX)" "YES"
		CFLAGS += -DPRODUCT_MEDIA_VIEWER_ON_LAYER_GFX
	endif
endif

ifeq "$(SUBSCRIPTION_RIGHTS)" "YES"
	CFLAGS += -DSUBSCRIPTION_RIGHTS
endif

ifeq "$(FAKE_SUBSCRIPTION_RIGHTS)" "YES"
	CFLAGS += -DFAKE_SUBSCRIPTION_RIGHTS
endif

#********** MULTIARCH COMPATIBILITY *********
ifeq "$(PRODUCT_MULTIARCH_COMPATIBILITY)" "YES"
    CFLAGS += -DMULTIARCH_COMPATIBILITY
endif

ifneq ($(TARGET_NAME),$(CHAL_VALIDATOR_TARGET_NAME))
	ifeq ($(COMEDIA_RESOURCES_IN_RAM),YES)
		CFLAGS += -DCOMEDIA_RESOURCES_IN_RAM
	endif
else
	CFLAGS += -DCHAL_VALIDATOR
endif

#*********************************************

# ****************************************
# PRODUCT_FEATURE_PVR
# ****************************************
ifeq "$(findstring PVR_INTERNAL, $(PRODUCT_FEATURE_PVR))" "PVR_INTERNAL"
	PRODUCT_PVR = PVR_ON
	PRODUCT_USE_TDAL_PVR = YES
	PRODUCT_USE_XPEL = YES
	PRODUCT_USE_TDAL_FS = YES
	PRODUCT_USE_ATAPI = YES
	PACKAGE_FILE_SYSTEM = YES
ifneq "$(PVR_CORE_IMPLEMENTATION)" "TDAL_PVR"	
	PRODUCT_USE_TDAL_PVR = NO
	PRODUCT_USE_FMDX = YES
	PRODUCT_TABLE_USE_FDMX = YES
endif
endif
ifeq "$(findstring PVR_EXTERNAL, $(PRODUCT_FEATURE_PVR))" "PVR_EXTERNAL"
	PRODUCT_PVR = PVR_ON
	PRODUCT_USE_TDAL_PVR = YES
	PRODUCT_USE_XPEL = YES
	PRODUCT_USE_TDAL_FS = YES
	PRODUCT_USE_USB = YES
	PACKAGE_FILE_SYSTEM = YES
ifneq "$(PVR_CORE_IMPLEMENTATION)" "TDAL_PVR"	
	PRODUCT_USE_TDAL_PVR = NO
	PRODUCT_USE_FMDX = YES	
	PRODUCT_TABLE_USE_FDMX = YES
endif
endif
	ifeq "$(findstring PVR_EXTERNAL_FAKE, $(PRODUCT_FEATURE_PVR))" "PVR_EXTERNAL_FAKE"
		PRODUCT_USE_ATAPI = YES
endif
ifeq "$(findstring PVR_BOTH, $(PRODUCT_FEATURE_PVR))" "PVR_BOTH"
	PRODUCT_PVR = PVR_ON
	PRODUCT_USE_TDAL_PVR = YES
	PRODUCT_USE_XPEL = YES
	PRODUCT_USE_TDAL_FS = YES
	PRODUCT_USE_ATAPI = YES
	PRODUCT_USE_USB = YES
	PACKAGE_FILE_SYSTEM = YES
ifneq "$(PVR_CORE_IMPLEMENTATION)" "TDAL_PVR"
	PRODUCT_USE_TDAL_PVR = NO
	PRODUCT_USE_FMDX = YES	
	PRODUCT_TABLE_USE_FDMX = YES
endif
endif

endif

# ****************************************
# PRODUCT_TRIGGER
# ****************************************

ifeq "$(findstring USB, $(PRODUCT_TRIGGER))" "USB"
PRODUCT_USE_TDAL_FS = YES
PRODUCT_USE_USB = YES
PRODUCT_USE_FMDX = YES
# for popup only
PACKAGE_FILE_SYSTEM = YES
endif

# ************************************************************************************************************************
# ************************************************************************************************************************

CFLAGS := $(CFLAGS) -DTKEL_$(PRODUCT_OS)

TKEL_USE_BGET = 1
CFLAGS := $(CFLAGS) -DTKEL_USE_BGET=1

#TKEL_USE_DLMALLOC = 1
#CFLAGS := $(CFLAGS) -DTKEL_USE_DLMALLOC=1

ifeq "$(MEMORYCONFIG_16MB)" "1"
CFLAGS += -DMEMORYCONFIG_16MB
endif

ifeq "$(PRODUCT_MEDIUM)" "MEDIUM_SAT"
CFLAGS += -DMEDIUM_SAT
endif

ifeq "$(PRODUCT_MEDIUM)" "MEDIUM_TER"
CFLAGS += -DMEDIUM_TER
CFLAGS += -DDVB_T2_ON
endif

ifeq "$(PRODUCT_MEDIUM)" "MEDIUM_CAB"
CFLAGS += -DMEDIUM_CAB
endif

CFLAGS += -DNB_TUNERS=$(PRODUCT_NB_TUNERS)

ifeq "$(PRODUCT_BROADCAST)" "BROADCAST_DVB"
CFLAGS += -D_BROADCAST_DVB_
endif

ifeq "$(PRODUCT_BROADCAST)" "BROADCAST_ATSC"
CFLAGS += -D_BROADCAST_ATSC_
endif

ifeq "$(PRODUCT_AUTOMATIC_UPDATE)" "AUTOMATIC_UPDATE_AVAILABLE"
CFLAGS += -DAUTOMATIC_UPDATE_AVAILABLE
endif

ifeq "$(PRODUCT_SDT_MONITORING)" "YES"
CFLAGS += -DSDT_MONITORING
endif

ifeq "$(PRODUCT_PERMANENT_SI_MONITORING)" "YES"
#CFLAGS += -DMONITORING_ONLY_ON_HOME_TS
#CFLAGS += -DDEBUG_PERMONIT
CFLAGS += -DPERMANENT_SI_MONITORING
ifeq "$(PERMANENT_SI_MONITORING_NONE)" "YES"
CFLAGS += -DPERMANENT_SI_MONITORING_NONE
endif
ifeq "$(PERMANENT_SI_MONITORING_ON_TS)" "YES"
CFLAGS += -DPERMANENT_SI_MONITORING_ON_TS
endif
ifeq "$(PERMANENT_SI_MONITORING_ON_NETWORK)" "YES"
CFLAGS += -DPERMANENT_SI_MONITORING_ON_NETWORK
endif
ifeq "$(PERMANENT_SI_MONITORING_ON_COMPLETE_NETWORK)" "YES"
CFLAGS += -DPERMANENT_SI_MONITORING_ON_COMPLETE_NETWORK
endif
endif


ifeq "$(PROGTYPE)" "BOOTLOADER"
CFLAGS += -DBOOTLOADER
endif

ifeq "$(PROGTYPE)" "COMEDIA"
CFLAGS += -DCOMEDIA_BIN
endif

ifeq "$(DISABLE_WSS)" "YES"
CFLAGS += -DDISABLE_WSS
endif

# ****************************************
# TKEL memory partition
CFLAGS += -DUSE_TKEL_MEMORY_PARTITIONS

# ****************************************
ifdef PRODUCT_BINARY_FILE_COMPRESSED
ifeq "$(PRODUCT_BINARY_FILE_COMPRESSED)" "YES"
# binary resources will be compressed by flash_tool 
CFLAGS += -DBINARY_FILE_COMPRESSED=TRUE
else
CFLAGS += -DBINARY_FILE_COMPRESSED=FALSE
endif
endif

# ****************************************
# Compression Level of fav lists in File System

ifdef PRODUCT_SLIST_FS_COMPRESSION_LEVEL
CFLAGS += -DSLIST_FS_COMPRESSION_LEVEL=$(PRODUCT_SLIST_FS_COMPRESSION_LEVEL)
endif

ifdef PRODUCT_AC3_DURING_TIMERS
	ifeq "$(PRODUCT_AC3_DURING_TIMERS)" "NO"
		CFLAGS += -DPRODUCT_NO_AC3_DURING_TIMERS
	endif
endif

# ****************************************
# LCN and LCN in fav lists
ifeq "$(PRODUCT_LCN)" "LCN_NO"
	CFLAGS += -DLCN_NO
endif
ifeq "$(PRODUCT_LCN)" "LCN_YES"
	CFLAGS += -DLCN_YES
	ifeq "$(PRODUCT_LCN_IN_FAV)" "LCN_IN_FAV_YES"
		CFLAGS += -DLCN_IN_FAV
	endif
endif

# ****************************************
# virtual keyboard
ifeq "$(PRODUCT_USE_VIRT_KEYBOARD)" "YES"
    CFLAGS += -DVIRT_KEYBOARD
endif

# ****************************************
# nagra nvod
ifeq "$(PRODUCT_NVOD)" "YES"
	CFLAGS += -DNAGRA_NVOD
endif

# ****************************************


#****************************************
# EIT image icon parser
ifeq "$(PRODUCT_EIT_IMAGE_ICON_PARSER)" "YES"
	CFLAGS += -DPARSE_IMAGE_FROM_EIT
endif
#****************************************

#****************************************
# EIT 24h of scheduled events in info banner
ifeq "$(PRODUCT_EIT_24H_SCH_INFO_BANNER)" "YES"
	CFLAGS += -DEIT_24H_SCH_INFO_BANNER
endif
#****************************************

# ****************************************
# Operator logo descriptor
ifeq "$(OPERATOR_LOGO_DESCRIPTOR)" "YES"
    CFLAGS += -DOPERATOR_LOGO_DESCRIPTOR
endif

# ****************************************
# Software bitmap scaling
ifeq "$(PRODUCT_SOFTWARE_BITMAP_SCALING)" "YES"
		CFLAGS += -DSOFTWARE_BITMAP_SCALING
endif

# ****************************************
# Audio
ifeq "$(PRODUCT_AUDIO_VOLUME_LEVEL_MODE)" "YES"
CFLAGS += -DVOLUME_LEVEL_MODE
endif
ifeq "$(PRODUCT_AUDIO_DOLBY_DEFAULT)" "ON"
CFLAGS += -DAUDIO_DOLBY_ON
endif
ifeq "$(PRODUCT_AUDIO_AC3)" "AUDIO_AC3_NONE"
CFLAGS += -DAUDIO_AC3_NONE
endif
ifeq "$(PRODUCT_AUDIO_AC3)" "AUDIO_AC3_USERCHOICE_MODE1"
CFLAGS += -DAC3_USERCHOICE
CFLAGS += -DAUDIO_AC3_USERCHOICE_MODE1
endif
ifeq "$(PRODUCT_AUDIO_AC3)" "AUDIO_AC3_USERCHOICE_MODE2"
CFLAGS += -DAC3_USERCHOICE
CFLAGS += -DAUDIO_AC3_USERCHOICE_MODE2
endif
ifeq "$(PRODUCT_AUDIO_AC3)" "AUDIO_AC3_ALWAYS"
CFLAGS += -DAUDIO_AC3_ALWAYS
endif
ifeq "$(PRODUCT_AUDIO_LANGUAGES)" "AUDIO_LANGUAGES_ONLY_ISO639"
CFLAGS += -DAUDIO_LANGUAGES_ONLY_ISO639
endif
ifeq "$(PRODUCT_AUDIO_LANGUAGES)" "AUDIO_LANGUAGES_ALL"
CFLAGS += -DAUDIO_LANGUAGES_ALL
endif
ifeq "$(PRODUCT_AUDIO_TRACK_NAMED_DD)" "AUDIO_TRACKDD_NO_SPECIAL_MANAGEMENT"
CFLAGS += -DAUDIO_TRACKDD_NO_SPECIAL_MANAGEMENT
endif
ifeq "$(PRODUCT_AUDIO_TRACK_NAMED_DD)" "AUDIO_TRACKDD_HAS_PRIORITY"
CFLAGS += -DAUDIO_TRACKDD_HAS_PRIORITY
endif

ifeq "$(PRODUCT_EMBEDDED)" "YES"
CFLAGS += -DCM_EMBEDDED
endif

ifeq "$(PRODUCT_SUBTI_LANGUAGES)" "LANGUAGES_ALL"
CFLAGS += -DSUBTI_LANGUAGES_ALL
endif
ifeq "$(PRODUCT_TTXT_LANGUAGES)" "LANGUAGES_ALL"
CFLAGS += -DTTXT_LANGUAGES_ALL
endif

# ****************************************
# EPG
ifeq "$(PRODUCT_EPG_GRID)" "YES"
CFLAGS += -DUSE_EPG_GRID
endif

# ****************************************
# ANTI CLONING
ifeq "$(PRODUCT_ANTICLONING_ENABLED)" "YES"
CFLAGS += -DANTICLONING_ENABLED
endif

# ****************************************
# TIMER
ifeq "$(PRODUCT_TIMER_ON)" "YES"
CFLAGS += -DTIMER_ON
endif

# ****************************************
# PVR
ifeq "$(PRODUCT_PVR)" "PVR_ON"
ifeq "$(findstring PVR_INTERNAL, $(PRODUCT_FEATURE_PVR))" "PVR_INTERNAL"
CFLAGS += -DPACKAGE_PVR -DPACKAGE_PVR_INTERNAL
endif
ifeq "$(findstring PVR_EXTERNAL, $(PRODUCT_FEATURE_PVR))" "PVR_EXTERNAL"
CFLAGS += -DPACKAGE_PVR -DPACKAGE_PVR_EXTERNAL
endif
ifeq "$(findstring PVR_BOTH, $(PRODUCT_FEATURE_PVR))" "PVR_BOTH"
CFLAGS += -DPACKAGE_PVR -DPACKAGE_PVR_INTERNAL -DPACKAGE_PVR_EXTERNAL
endif
endif

# ****************************************
# DUAL DECODING
ifeq "$(PRODUCT_DUAL_DECODING)" "DUAL_DECODING_ON"
CFLAGS += -DDUAL_DECODING_ON
endif

# ****************************************
# CHALV2 support
CFLAGS += -DCHALV2_ON

# ****************************************
# IP
ifeq "$(PRODUCT_IP)" "IP_ON"
CFLAGS += -DPACKAGE_IP
endif

# ****************************************
# FP
ifeq "$(PRODUCT_USE_FP)" "YES"
CFLAGS += -DPRODUCT_USE_FP
endif

# ****************************************
# FS with USB
ifeq "$(PRODUCT_USE_USB)" "YES"
	CFLAGS += -DPRODUCT_USE_USB
endif

# ****************************************
# FS with ATAPI
ifeq "$(PRODUCT_USE_ATAPI)" "YES"
  CFLAGS += -DPRODUCT_USE_ATAPI
endif

# ****************************************
# FILE_SYSTEM
ifeq "$(PACKAGE_FILE_SYSTEM)" "YES"
	CFLAGS += -DPACKAGE_FILE_SYSTEM
endif

# ****************************************
# XPEL
ifeq "$(PRODUCT_USE_XPEL)" "YES"
	CFLAGS += -DPRODUCT_USE_XPEL
endif
ifeq "$(findstring FREEVIEW, $(PRODUCT_FEATURE_PVR))" "FREEVIEW"
	PRODUCT_PVR_FREEVIEW_PLAYBACK=1
	CFLAGS += -DPRODUCT_PVR_FREEVIEW_PLAYBACK
endif

# number of scheduled programme : default value 15
# CFLAGS += -DkXPEL_MAX_PROG=15

# number of max record simultaneous linked to source number (front-end ?)
# default value in xpel.h : 2
CFLAGS += -DkXPEL_MAX_REC=$(PRODUCT_NB_TUNERS)

# ****************************************
ifeq "$(PRODUCT_HD)" "YES"
CFLAGS += -DHD_ENABLE
CFLAGS += -DUI_V4
ifeq "$(PRODUCT_USE_NX2_UI)" "YES"
CFLAGS += -DUI_SIZE_720
else
CFLAGS += -DUI_SIZE_1080
endif
ifeq "$(PRODUCT_SUBTITLE)" "SUBT_ON"
CFLAGS += -DDVB_SUBTITLE_HD
endif
ifeq "$(PRODUCT_USE_HDCP)" "YES"
CFLAGS += -DUSE_HDCP
endif
endif

ifeq "$(PRODUCT_USE_NX2_UI)" "YES"
CFLAGS += -DPRODUCT_USE_NX2_UI
CFLAGS += -DUI_V4
endif

# PRODUCT_AUX_VIDEO_DISPLAY_SD
ifeq "$(PRODUCT_AUX_VIDEO_DISPLAY_SD)" "YES"
	CFLAGS += -DAUXILIARY_VIDEO_DISPLAY_SD
endif


# ****************************************
# Needed by TKEL : at least one must be defined
#	See ms2tick macro
#		if ticksPerSec > (2^32 / 999) ~ 4300000 => Huge value else Humble value
#CFLAGS += -DOS_TICKPERSEC_HUGEVALUE
CFLAGS += -DOS_TICKPERSEC_HUMBLEVALUE

# ****************************************
# TDAL_TS
ifeq "$(PRODUCT_USE_TDAL_TS)" "USE_TDAL_TS_ON"
CFLAGS += -DUSE_TDAL_TS 
endif

# ****************************************
# TDAL_FS
ifneq "$(FLASH_TOOL_TARGET)" "1"
ifeq "$(PRODUCT_USE_TDAL_FS)" "YES"
CFLAGS += -DUSE_TDAL_FS 
endif
endif

# ****************************************
# TDAL_MP
ifeq "$(PRODUCT_USE_TDAL_MP)" "YES"
CFLAGS += -DUSE_TDAL_MP 
endif

# ****************************************
# TDAL_PVR
ifeq "$(PRODUCT_USE_TDAL_PVR)" "YES"
CFLAGS += -DUSE_TDAL_PVR 
endif

# ****************************************
# MHEG_FS
ifeq "$(PRODUCT_USE_MHEG_FS)" "YES"
CFLAGS += -DUSE_MHEG_FS 
endif

# ****************************************
ifeq "$(PRODUCT_USE_SDEC)" "YES"
CFLAGS += -DPRODUCT_USE_SDEC
endif

ifeq "$(PRODUCT_USE_RDR_MP3)" "YES"
CFLAGS += -DPRODUCT_USE_RDR_MP3
endif

ifeq "$(PRODUCT_USE_AVP)" "YES"
CFLAGS += -DPRODUCT_USE_AVP
endif

# ****************************************
# With bootloader
ifeq "$(PRODUCT_BOOTLOADER)" "BOOTLOADER_ON"
CFLAGS += -DBOOTLOADER_ON
endif


# ****************************************
# override default priorities
CFLAGS += -DPLAY_TASK_PRIO=30
CFLAGS += -DkPITABLE_TASK_PRIORITY=50
CFLAGS += -DkPIMISC_ZAPPER_TASK_PRIORITY=50
CFLAGS += -DkPIMISC_MONITOR_TASK_PRIORITY=50
CFLAGS += -DkPIMISC_BUILDER_TASK_PRIORITY=50

# ****************************************
# available flags to override default stack sizes
#CFLAGS += -DSYS_MNGR_STATE_MACHINE_STACK=5*1024
#CFLAGS += -DPLAY_TIMER_STACKSIZE=1*1024
CFLAGS += -DPLAY_TASK_STACKSIZE=16*1024


# ****************************************
# DEBUG
ifeq "$(DEBUG)" "1"
CFLAGS += -D__DEBUG__
endif

# ****************************************
# Specific platforms

ifeq "$(findstring ST, $(PRODUCT_ARCH))" "ST"
endif


# ****************************************
# OTA

ifeq "$(PRODUCT_USE_OTA)" "YES"
    CFLAGS += -DUSE_OTA
endif
# ****************************************


# ****************************************
# TRACES
#ifneq "$(origin TRACES)" "undefined"
  ifeq "$(TRACES)" "UART"
    CFLAGS += -D__DEBUG_TRACE__ -D__TRACE_UART__
  else
    ifeq "$(TRACES)" "TRACES_FOR_INTEGRATION"
      CFLAGS += -DCOMEDIA_TRACES_FOR_INTEGRATION_ENABLE -D__TRACE_DCU__
    endif
  endif
#endif

