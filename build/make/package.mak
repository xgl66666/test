#------------------------------------------
# Common flag between package
#------------------------------------------

ifeq "$(PRODUCT_IP)" "IP_ON"
CFLAGS += -DPACKAGE_IP
endif

#------------------------------------------
# DATAMANGER JFS package
#------------------------------------------
ifeq "$(PRODUCT_USE_DMJFS)" "YES"
CFLAGS += -D_USE_DATAMNGR_JFS_

ifeq "$(PRODUCT_DMJFS_IN_RAM)" "YES"
CFLAGS += -D_DATAMNGR_JFS_RAM_PARTITION_
endif

ifeq "$(PRODUCT_DMJFS_ON_NAND)" "YES"
CFLAGS += -D_DATAMNGR_JFS_ON_NAND_
  ifeq "$(PRODUCT_DMJFS_NAND_WRITE_PAGE)" "YES"
  CFLAGS += -D_DATAMNGR_JFS_NAND_WRITE_PAGE_
  endif
endif
ifeq "$(FORCE_RESET_PART_B)" "YES"
CFLAGS += -D_FORCE_RESET_PART_B_
endif
ifeq "$(FORCE_RESET_PART_N)" "YES"
CFLAGS += -D_FORCE_RESET_PART_N_
endif
ifeq "$(FORCE_RESET_PART_M)" "YES"
CFLAGS += -D_FORCE_RESET_PART_M_
endif
endif

#------------------------------------------
# celine_player package flag
#------------------------------------------

ifeq "$(PRODUCT_MAESTRO_PORTAL)" "PORTAL_ON"
ifeq "$(PRODUCT_APP)" "MAESTRO"
CFLAGS += -DMAESTRO_INTERACTIVITY_ON
endif
endif
CFLAGS += -DRECTANGLE_NOT_USE_BUFFER
ifeq "$(PRODUCT_INSTALLATION)" "NO_INSTALLATION_TS"
CFLAGS += -DNO_INSTALLATION_TS
endif

# SPECTRAL INVERSION flags
ifeq "$(IMCL)" "YES"
    CFLAGS += -DSPECTRUM_INVERSION
endif


#------------------------------------------
# EIM package flag
#------------------------------------------

ifeq "$(PRODUCT_BROADCAST)" "BROADCAST_DVB"
CFLAGS += -D_EIM_MODE_DVB_
# CFLAGS += -D_EIM_DVB_LOG_
# EIM_DVB_LOG=1
endif
ifeq "$(PRODUCT_BROADCAST)" "BROADCAST_ATSC"
CFLAGS += -D_EIM_MODE_ATSC_
endif

ifeq "$(PRODUCT_EPG_ENGINE)" "EIM"
ifneq "$(PRODUCT_EIT)" "NO_EIT"
ifeq "$(PRODUCT_EIT)" "EIT_ONLY_PF"
CFLAGS += -DEIT_PF_ON
else
CFLAGS += -DEIT_PF_ON
CFLAGS += -DEIT_SCHEDULE_ON
endif
endif

ifeq "$(PRODUCT_PVR)" "PVR_ON"
  # For PVR product, because of the gamma-live :
  # we need to have {NB_TUNERS+1} demuxes so ...
CFLAGS += -DkEIM_MAX_DEMUX_COUNT=$(PRODUCT_NB_TUNERS)+1
  endif

ifeq "$(PRODUCT_EIT_FULL_CACHING)" "YES"
CFLAGS += -DEIT_FULL_CACHING_ON
CFLAGS += -DEIT_FULL_CACHING_ON_STATIC
ifeq "$(PRODUCT_APP)" "MAESTRO"
CFLAGS += -DEPG_STRING_COMPRESION
endif
EIT_FULL_CACHING_ON=1
CFLAGS += -DECMM_USE_BGET
endif

# size of EIT memory
ifdef EIT_MEM_SIZE
	CFLAGS += -DSYS_MNGR_EIT_MEM_SIZE=$(EIT_MEM_SIZE)
endif

# Length, in hours, of the event cache
ifdef ECC_CACHE_LENGTH
	CFLAGS += -DkECC_CACHE_LENGTH=$(ECC_CACHE_LENGTH)
endif


# EIM available flags
ifeq "$(IMCL)" "YES"
    CFLAGS += -D_SEL_NB_NEXT_DAYS_CACHING_=1
else
#CFLAGS += -D_SEL_NB_NEXT_DAYS_CACHING_=0
endif
#CFLAGS += -D_SEL_NO_SERVICE_CACHING_
#CFLAGS += -D_SERV_NO_EIT_SERVICE_CACHING_
#CFLAGS += -DkEIM_TASK_SIZE=5*1024
endif


ifeq "$(PRODUCT_IP)" "IP_ON"
CFLAGS += -D_EIM_MODE_IP_
endif

#------------------------------------------
# EPG ENGINE package flag
#------------------------------------------
ifeq "$(PRODUCT_EPG_ENGINE)" "EPG_ENGINE"
CFLAGS += -DEPG_ENGINE_ON
CFLAGS += -DEIT_PF_ON
CFLAGS += -DEIT_SCHEDULE_ON
PACKAGELIB += $(LIB_PREFFIX)epg_engine.$(LIB_SUFFIX)
PACKTESTMAINLIB += $(LIB_PREFFIX)epg_engine.$(LIB_SUFFIX)
APPDEMOMAINLIB += $(LIB_PREFFIX)epg_engine.$(LIB_SUFFIX)
endif
ifeq "$(PRODUCT_EPG_ENGINE)" "EIM"
CFLAGS += -DEIM_ON
endif


#------------------------------------------
# TABLE package flag
#------------------------------------------
ifeq "$(PRODUCT_BAT)" "YES"
CFLAGS += -DBAT_SEARCH
ifeq "$(PRODUCT_BAT_GENRES)" "YES"
CFLAGS += -DBAT_GENRES
endif
endif
ifeq "$(PRODUCT_BROADCAST)" "BROADCAST_DVB"
_TABLE_DVB_=1
CFLAGS += -D_TABLE_DVB_
endif
ifeq "$(PRODUCT_BROADCAST)" "BROADCAST_ATSC"
_TABLE_ATSC_=1
CFLAGS += -D_TABLE_ATSC_
endif

_TABLE_EIT_DVB_SPEED_IMPROVED_=1
CFLAGS += -DTABLE_EIT_DVB_SPEED_IMPROVED

# TABLE available flags
#CFLAGS += -DTABLE_TASK_STACK_SIZE=2*1024
CFLAGS += -DkTABLE_NO_SPECIFIC_TASK

ifeq "$(PRODUCT_TABLE_USE_FDMX)" "YES"
CFLAGS += -DFDMX_USED_BY_TABLE
endif

ifeq "$(PRODUCT_DSMCC_STACK)" "DSMCC_STACK_ON"
_TABLE_DVB_=1
endif

#------------------------------------------
# Frontend flags
#------------------------------------------
ifeq "$(PRODUCT_MEDIUM)" "MEDIUM_CAB"
#CFLAGS += -DTEMPORARY_MEDIUM_CAB_IS_MEDIUM_TER 
endif
#------------------------------------------
# PIDB package flag
#------------------------------------------

ifeq "$(PRODUCT_MEDIUM)" "MEDIUM_SAT"
PIDB_DVB_WITH_FREQUENCY=1
CFLAGS += -D_PIDB_MEDIUM_DVB_WITH_FREQUENCY_
endif

ifeq "$(PRODUCT_MEDIUM)" "MEDIUM_TER"
PIDB_DVB_WITH_FREQUENCY=1
CFLAGS += -D_PIDB_MEDIUM_DVB_WITH_FREQUENCY_
endif

ifeq "$(PRODUCT_BROADCAST)" "BROADCAST_ATSC"
PIDB_ATSC=1
CFLAGS += -D_PIDB_MEDIUM_ATSC_
endif

ifeq "$(PRODUCT_PVR)" "PVR_ON"
NB_PI_DEMUX_INSTANCE:=$(PRODUCT_NB_TUNERS)+1
ifeq "$(PRODUCT_TABLE_USE_FDMX)" "YES"
  NB_PI_DEMUX_INSTANCE:=$(NB_PI_DEMUX_INSTANCE)+1
endif
else # PVR_OFF
  NB_PI_DEMUX_INSTANCE:=1
endif
CFLAGS += -DkPI_MAX_DEMUX_INSTANCE=$(NB_PI_DEMUX_INSTANCE)

ifeq "$(PRODUCT_PVR)" "PVR_ON"
PIDB_PVR=1
ifeq "$(PVR_CORE_IMPLEMENTATION)" "TDAL_PVR"
CFLAGS += -DPVR_CORE_IMPLEMENTATION_TDAL_PVR
endif
CFLAGS += -D_PIDB_MEDIUM_PVR_
else # PVR_OFF
ifeq "$(PRODUCT_MEDIUM)" "MEDIUM_CAB"
#-----------------------
# 3 == 2 install cables (scan + network) + zap ==> product for demo
# else 2 instances are needed
#-----------------------
CFLAGS += -DkPI_MAX_PIDB_INSTANCE=3 
else
CFLAGS += -DkPI_MAX_PIDB_INSTANCE=2
endif
endif

ifeq "$(findstring FREEVIEW, $(PRODUCT_FEATURE_PVR))" "FREEVIEW"
CFLAGS += -DNETWORK_AUTHORITY_DESCRIPTOR_MANAGED
endif

ifeq "$(UK_SPECIFIC)" "YES"
CFLAGS += -DUK_DESCRIPTOR_MANAGED -DUK_SPECIFIC=1
endif

ifeq "$(PRODUCT_DSMCC_STACK)" "DSMCC_STACK_ON"
  CFLAGS += -D_TABLE_DSMCC_ -D_TABLE_DSMCC_PARSE_EVENT_
	_TABLE_DSMCC_=1
endif

ifeq "$(PRODUCT_IP)" "IP_ON"
CFLAGS += -D_PIDB_MEDIUM_IP_
endif

# When kPI_BUILDER_MAX_WAITING_NIT flag
# is not defined, NIT timeout is 12 seconds
ifeq "$(PRODUCT_ENGINE)" "ENGINE_TER_MHEG"
	ifneq "$(PRODUCT_MEDIUM)" "MEDIUM_CAB"
		# In SI-PSI test si27, NIT timeout is 20 seconds
		CFLAGS += -DkPI_BUILDER_MAX_WAITING_NIT=20000
	endif
endif

ifeq "$(UI_RELIANCE)" "YES"
CFLAGS += -DUI_RELIANCE=YES
endif
ifeq "$(UI_OITV)" "YES"
CFLAGS += -DUI_OITV=YES
endif

ifeq "$(PRODUCT_CUSTOMER_HEADEND)" "IMCL"
	CFLAGS += -DIMCL=YES
endif

ifeq "$(PRODUCT_CUSTOMER_HEADEND)" "GTPL"
    CFLAGS += -DGTPL=YES
endif
ifeq "$(PRODUCT_CUSTOMER_HEADEND)" "GHITS"
	CFLAGS += -DIMCL=YES	
endif

ifeq "$(PRODUCT_CUSTOMER_HEADEND)" "DEN"
	CFLAGS += -DDEN=YES
endif

ifeq "$(PRODUCT_CUSTOMER_HEADEND)" "VSM"
CFLAGS += -DVSM=YES
endif

ifeq "$(PRODUCT_PROPRIETARY_BOOTLOADER)" "CHANGHONG"
	CFLAGS += -DCHANGHONG_BOOTLOADER
endif

ifeq "$(PRODUCT_PROPRIETARY_BOOTLOADER)" "NEWLAND"
	CFLAGS += -DNEWLAND_BOOTLOADER
endif

ifeq "$(PRODUCT_PROPRIETARY_BOOTLOADER)" "SKYWORTH"
	CFLAGS += -DSKYWORTH_BOOTLOADER
endif

ifeq "$(PRODUCT_PROPRIETARY_BOOTLOADER)" "CHANGHONG"
	CFLAGS += -DCHANGHONG_BOOTLOADER
endif

ifeq "$(PRODUCT_PROPRIETARY_BOOTLOADER)" "TECHNICOLOR"
	CFLAGS += -DTECHNICOLOR_BOOTLOADER
	CFLAGS += -DPK_IN_RAM
endif

ifeq "$(PRODUCT_PROPRIETARY_BOOTLOADER)" "TELESYSTEM"
	CFLAGS += -DTELESYSTEM_BOOTLOADER
endif

ifeq "$(PRODUCT_PK_IN_RAM)" "YES"
	CFLAGS += -DPK_IN_RAM
endif

ifeq "$(PRODUCT_PROPRIETARY_BOOTLOADER)" "TELESYSTEM_MAP51_MSD5024"
	CFLAGS += -DTELESYSTEM_MAP51_BOOTLOADER
endif

ifeq "$(PRODUCT_PROPRIETARY_BOOTLOADER)" "TELESYSTEM_MAP34_MSD5029"
	CFLAGS += -DTELESYSTEM_MAP34_BOOTLOADER
endif

ifeq "$(PRODUCT_PROPRIETARY_BOOTLOADER)" "KAON_MSD5C59_BOOTLOADER"
	CFLAGS += -DKAON_MSD5C59_BOOTLOADER
ifeq "$(PRODUCT_BOOTLOADER_TYPE)" "PRODUCTION"
	CFLAGS += -DPAIRING_KEY_ENCRYPTED=1
endif
endif

ifeq "$(PLATFORM_CHIP_VENDOR)" "pc"
CFLAGS += -DNEWLAND_MERLIN_CONVERT
endif

ifeq "$(SERVICE_LOGO)" "YES"
CFLAGS += -DSERVICE_LOGO
#CFLAGS += -DSERVICE_LOGO_STORE_GFX_BMP
endif

CFLAGS += -DNIT_VERSION_FILTER

# PIDB available flags for debug purpose
#ifeq "$(PRODUCT_MEDIUM)" "MEDIUM_CAB"
# 60 second  for tests in DEBUG */
#CFLAGS += -DkPI_BUILDER_MAX_WAITING_NIT=12000    # from ziggo value 15 s max
#CFLAGS += -DkPI_BUILDER_MAX_WAITING_NIT=30000 #16000   # from ziggo value 15 s max
#CFLAGS += -DkPI_BUILDER_MAX_WAITING_SDT=11000    # from ziggo value 20 s max
#endif
#CFLAGS += -DkPI_BUILDER_MAX_WAITING_SDT=11000
#CFLAGS += -DkPI_BUILDER_TASK_STACK_SIZE=3*1024
#CFLAGS += -DkPI_ZAPPER_TASK_STACK_SIZE=2*1024
#CFLAGS += -DkPI_MONITOR_TEMP_TASK_STACK_SIZE=2*1024
#CFLAGS += -DkPI_MONITOR_PERM_TASK_STACK_SIZE=2*1024
#CFLAGS += -DkPI_TABLE_RECEIVED_TASK_STACK_SIZE=2*1024

# needed by stream DTG SI/PSI si05.mpg service SIT U2. see issue 6524
# 2 byte text, length 32, with emphasis characters
# ".SIT_U2_len_emphasis_2_byte_fill"
# 
##CFLAGS += -DkPI_MAX_NAME_LENGTH=71

#------------------------------------------
# text package flag
#------------------------------------------

# TEXT available flags
_NO_UNICODE_=1
_NO_ISO_8859_=1
CFLAGS += -D_NO_UNICODE_
CFLAGS += -D_NO_ISO_8859_

ifeq "$(USE_DLK_1_1)" "YES"
#------------------------------------------
# DSC flag
#------------------------------------------
CFLAGS += -DDSC_SUPPORT
CFLAGS += -DDSC_L1_SUPPORT
endif

#------------------------------------------
# datetime package flag
#------------------------------------------

# datetime available flags
#CFLAGS += -DkDAT_TIME_STACKSIZE=2*1024

#------------------------------------------
# Subtitle package flag
#------------------------------------------


ifeq "$(PRODUCT_SUBTITLE)" "SUBT_ON"
CFLAGS += -DSUBTITLE_ON -DXSERV_COMPONENT_SUBT_USE
endif

# Subtitle available flags
#CFLAGS += -DkDSM_FILTER_TRANSPORT_PACKETS=TRUE
CFLAGS += -DkDSF_SHOW_BEFORE_PALETTE=TRUE


ifeq "$(PRODUCT_USE_LABEL_ANIMATION)" "YES"
CFLAGS += -DPRODUCT_USE_LABEL_ANIMATION
endif

# TRUE COLOR
PRODUCT_USE_PNGLIB = YES

#------------------------------------------
# SDEC package flags
#------------------------------------------
ifeq "$(PRODUCT_USE_SDEC)" "YES"
	# ST_OS21 flag for STAVMEM
	#SDEC_GLUE_JPEG_XXX_PARTITION (XXX=OS20,TKEL,TDAL_MP or STAVMEM) flag must be defined
ifeq "$(PRODUCT_USE_TDAL_MP)" "YES"
	CFLAGS += -DSDEC_GLUE_JPEG_TDAL_MP_PARTITION
else
	CFLAGS += -DSDEC_GLUE_JPEG_TKEL_PARTITION
endif
endif

#------------------------------------------
# Mheg package flag
#------------------------------------------


ifeq "$(PRODUCT_ENGINE)" "ENGINE_TER_MHEG"
	# Mheg available flags
	CFLAGS += -DINTERACTIVE_ENGINE_ON
	PRODUCT_USE_PNGLIB = YES
	CFLAGS += -DJPEG_ARGB_BUFFER
	
ifeq "$(PRODUCT_MHEG_DEFAULT_OSD_MODE)" "TRUE_COLOR"
	CFLAGS += -DMHEG_DEFAULT_OSD_MODE_TRUE_COLOR
endif

	MHEG_USE_OWN_MEMORY_ALLOCATOR=YES
	CFLAGS += -DMHEG_USE_OWN_MEMORY_ALLOCATOR
	PRODUCT_USE_AV_CLUSTER=YES
endif

#------------------------------------------
# teletext package flag
#------------------------------------------

ifeq "$(PRODUCT_TELETEXT_VBI)" "TTXT_VBI_ON"
TTXT_ON=1
CFLAGS += -DTTXT_VBI_ON
endif

ifeq "$(PRODUCT_TELETEXT_OSD)" "TTXT_OSD_ON"
TTXT_ON=1
TTXT_OSD_ON=1
CFLAGS += -DTTXT_OSD_ON -DXSERV_COMPONENT_TTXT_USE
endif

# TTXT available flags

#CFLAGS += -kTTXT_DISPLAY_TASK_PRIORITY=15
#CFLAGS += -kTTXT_DISPLAY_TASK_STACK_SIZE=3*1024

#CFLAGS += -kTTXT_HEADER_TASK_PRIORITY=15
#CFLAGS += -kTTXT_HEADER_TASK_STACK_SIZE=3*1024

#CFLAGS += -kTTXT_BUILDPAGE_TASK_PRIORITY=15
#CFLAGS += -kTTXT_BUILDPAGE_TASK_STACK_SIZE=3*1024

#CFLAGS += -DTTXT_EXTERNGFXGLUE_LOW_MEMORY_USED
#CFLAGS += -DVBI_AND_OSD_DUAL_NOT_SUPPORTED

# Max number of download refusals
ifdef PRODUCT_TRIGGER_MAX_REFUSAL
CFLAGS += -DkPRODUCT_TRIGGER_MAX_REFUSAL=$(PRODUCT_TRIGGER_MAX_REFUSAL)
endif

#CFLAGS += -DTDAL_SMC_USE_READWRITE
#------------------------------------------
# DSMCC package flag
#------------------------------------------

ifeq "$(PRODUCT_DSMCC)" "DSMCC_ON"
CFLAGS += -DPRODUCT_DSMCC
endif

#------------------------------------------
# media_renderer/browser/source/metadata  package flag
#------------------------------------------
ifeq "$(PRODUCT_MEDIA_VIEWER)" "YES"
  CFLAGS += -DPRODUCT_MEDIA_VIEWER
endif
ifeq "$(PRODUCT_USE_MEDIA_BROWSER)" "YES"
 CFLAGS += -DPRODUCT_USE_MEDIA_BROWSER
endif

#------------------------------------------
# DDM package
#------------------------------------------
ifeq "$(PRODUCT_USE_DDM)" "YES"
 CFLAGS += -DPACKAGE_DDM	
endif
	
##########################################################################
#
# Necessary for package test
#

ifeq "$(PACK_TEST_USE_TDAL_FOR_LOW_LEVEL_ACCESS)" "1"
CFLAGS += -DUSE_TDAL_API_LOW_LEVEL
endif

ifeq "$(PRODUCT_MEDIUM)" "MEDIUM_TER"
CFLAGS += -DPACK_TEST_DMD_OFDM
endif

ifeq "$(PRODUCT_MEDIUM)" "MEDIUM_SAT"
CFLAGS += -DPACK_TEST_DMD_QPSK
endif


ifeq "$(PACK_TEST_TDAL_DMD)" "1"
PACK_TEST_TDAL_TSIN_USED=1
CFLAGS += -DPACK_TEST_TDAL_DMD_USED
endif

ifeq "$(PACK_TEST_TDAL_TSIN)" "1"
CFLAGS += -DPACK_TEST_TDAL_TSIN_USED
endif

#------------------------------------------
# trigger package flag
#------------------------------------------

ifeq "$(PRODUCT_TRIGGER)" "TRIGGER_OFF"
CFLAGS += -DTRIG_OFF
endif

ifeq "$(findstring DVBSSU, $(PRODUCT_TRIGGER))" "DVBSSU"
CFLAGS += -DTRIG_DVBSSU
endif

ifeq "$(findstring DVBSSUENHANCED, $(PRODUCT_TRIGGER))" "DVBSSUENHANCED"
CFLAGS += -DTRIG_DVBSSU_ENHANCED
endif

ifeq "$(findstring DTG, $(PRODUCT_TRIGGER))" "DTG"
CFLAGS += -DTRIG_DTG
endif

ifeq "$(findstring IDT, $(PRODUCT_TRIGGER))" "IDT"
CFLAGS += -DTRIG_IDT
endif

ifeq "$(findstring USB, $(PRODUCT_TRIGGER))" "USB"
CFLAGS += -DTRIG_USB
FDMX_RELY_TO=TDAL_FS
endif

ifeq "$(findstring IRDCMD, $(PRODUCT_TRIGGER))" "IRDCMD"
CFLAGS += -DTRIG_IRDCMD
endif


ifeq "$(PRODUCT_USE_OTA)" "YES"
ifeq "$(PRODUCT_USE_SSU)" "TDAL_SSU"
CFLAGS += -DUSE_TDAL_SSU
else
CFLAGS += -DUSE_TDAL_OTA
endif
endif

#------------------------------------------
# Downloader package flags
#------------------------------------------
ifneq "$(PRODUCT_DOWNLOADER)" "DOWNLOADER_OFF"
# Select downloader behaviour kind according PRODUCT_DOWNLOADER
CFLAGS += -DDOWNLOADER_ON
endif
# activate or not using PRODUCT_DOWNLOADER_USE_CFGFILE
ifeq "$(PRODUCT_DOWNLOADER_USE_CFGFILE)" "YES"
CFLAGS += -D__USE_CONFIG_FILE__	
endif

#------------------------------------------
# Bootloader package flags
#------------------------------------------
ifneq "$(PRODUCT_BOOTLOADER)" "BOOTLOADER_OFF"
# Select bootloader behaviour kind according PRODUCT_BOOTLOADER
ifeq "$(findstring SERIAL, $(PRODUCT_BOOTLOADER))" "SERIAL"
CFLAGS += -DBTL_SERIAL
endif

ifeq "$(findstring BROADCAST, $(PRODUCT_BOOTLOADER))" "BROADCAST"
CFLAGS += -DBTL_BROADCAST
endif

ifeq "$(findstring USB, $(PRODUCT_BOOTLOADER))" "USB"
CFLAGS += -DBTL_USB_FILE
endif

# activate or not integrity checking according PRODUCT_BOOTLOADER_INTEGRITY_CHECK
ifeq "$(PRODUCT_BOOTLOADER_INTEGRITY_CHECK)" "CHECK_SOFTWARE_IMAGE_INTEGRITY"
CFLAGS += -DkSW_INTEGRITY_CHECK	
else
    ifneq "$(PRODUCT_BOOTLOADER_INTEGRITY_CHECK)" "DO_NOT_CHECK_INTEGRITY"
        $(error	"PRODUCT_BOOTLOADER_INTEGRITY_CHECK = $(PRODUCT_BOOTLOADER_INTEGRITY_CHECK), not reconized")
        include ERROR_PRODUCT_BOOTLOADER_INTEGRITY_CHECK_value_$(PRODUCT_BOOTLOADER_INTEGRITY_CHECK)
    endif
endif

# activate or not forced mode
ifeq "$(PRODUCT_BOOTLOADER_FORCED_MODE)""YES"
	CFLAGS += -DBOOTLOADER_FORCED_MODE_ENABLED
endif

# Select Public key type according PRODUCT_BOOTLOADER_KEY_TYPE
ifeq "$(PRODUCT_BOOTLOADER_KEY_TYPE)" "IWEDIA_KEY"
       PRODUCT_BOOTLOADER_KEY_TYPE_FLAG=kPUBLIC_DEV_IWEDIA_KEY
endif
ifeq "$(PRODUCT_BOOTLOADER_KEY_TYPE)" "CRYPTO_TEST_KEY"
       PRODUCT_BOOTLOADER_KEY_TYPE_FLAG=kPUBLIC_TEST_CRYPTO_KEY
endif
ifeq "$(PRODUCT_BOOTLOADER_KEY_TYPE)" "CRYPTO_PRODUCT_KEY"
       PRODUCT_BOOTLOADER_KEY_TYPE_FLAG=kPUBLIC_PRODUCT_CRYPTO_KEY
    endif
ifneq "$(origin PRODUCT_BOOTLOADER_KEY_TYPE_FLAG)" "undefined"
    CFLAGS += -D$(PRODUCT_BOOTLOADER_KEY_TYPE_FLAG)=1
      else
    $(error	"PRODUCT_BOOTLOADER_KEY_TYPE = $(PRODUCT_BOOTLOADER_KEY_TYPE), Not reconized ")
      endif
    endif

#------------------------------------------
# CDAC package flag
#------------------------------------------
ifeq "$(USE_CDAC)" "YES"
CFLAGS += -DUSE_CDAC
endif
	
#------------------------------------------
# FreeTye package flag
#------------------------------------------
ifeq "$(USE_FREETYPE2)" "YES"
	CFLAGS += -DTEXT_FREETYPE2
    CFLAGS += -DPRODUCT_USE_EXT_FONT_ENGINE

ifeq "$(USE_FREETYPE2_CACHE)" "YES"
    CFLAGS += -DTEXT_FREETYPE2_CACHE
endif
endif
#------------------------------------------
# ONID filtering flag
#------------------------------------------
ifeq "$(PRODUCT_INSTALL_ONLY_CURRENT_COUNTRY)" "YES"
CFLAGS += -DINSTALL_ONLY_CURRENT_COUNTRY
endif

#------------------------------------------
# Flash lock
#------------------------------------------
ifeq "$(PRODUCT_USE_LOCK_SPLS)" "YES"
CFLAGS += -DUSE_LOCK_SPLS
endif


#------------------------------------------
# avoid restarting stb when going to standby
#------------------------------------------
ifeq "$(PRODUCT_NORESTART_STANDBY)" "YES"
CFLAGS += -DSYS_STANDBY_AV_OFF
endif


#------------------------------------------
# fast channel change optimisation
#------------------------------------------
ifeq "$(PRODUCT_USE_FAST_ZAPP_THREAD)" "YES"
CFLAGS += -DUSE_BAN_ZAP_START_THREAD
endif
