#---------------------------------------------
### This makefile use to set targets and paths
#---------------------------------------------

########################################
###-------------SDK env -------------###
########################################
define find_platform_folder
$(or    $(wildcard $(1)/$(PLATFORM_CHIP_VENDOR)_$(PLATFORM_CHIP_MODEL)_$(PLATFORM_BOARD_VENDOR)_$(PLATFORM_BOARD_MODEL)_$(PRODUCT_OS)), \
        $(wildcard $(1)/$(PLATFORM_CHIP_VENDOR)_$(PLATFORM_CHIP_MODEL)_$(PLATFORM_BOARD_VENDOR)_$(PLATFORM_BOARD_MODEL)), \
        $(wildcard $(1)/$(PLATFORM_CHIP_VENDOR)_$(PLATFORM_CHIP_MODEL)_$(PLATFORM_BOARD_VENDOR)_$(PRODUCT_OS)), \
        $(wildcard $(1)/$(PLATFORM_CHIP_VENDOR)_$(PLATFORM_CHIP_MODEL)_$(PLATFORM_BOARD_VENDOR)), \
        $(wildcard $(1)/$(PLATFORM_CHIP_VENDOR)_$(PLATFORM_CHIP_MODEL)_$(PRODUCT_OS)), \
        $(wildcard $(1)/$(PLATFORM_CHIP_VENDOR)_$(PLATFORM_CHIP_MODEL)), \
        $(wildcard $(1)/$(PLATFORM_CHIP_VENDOR)_$(PRODUCT_OS)), \
        $(wildcard $(1)/$(PLATFORM_CHIP_VENDOR)), \
        $(wildcard $(1)/$(PRODUCT_OS)))
endef

export SDK_ROOT := $(call find_platform_folder, $(ROOT_DIR)/sdk)

########################################
###-----------PROJECT env -----------###
########################################
export PROJECTROOT=$(ROOT_DIR)
export UNIX_ROOT_DIR=$(ROOT_DIR)
export DOS_ROOT_DIR=$(ROOT_DIR)

########################################
###-----------BUILD env -------------###
########################################
export BUILD_ROOT=$(ROOT_DIR)/build
export BUILD_MAKE=$(BUILD_ROOT)/make
export BUILD_EXPORTS=$(BUILD_ROOT)/bin
export BUILD_INCLUDE=$(BUILD_ROOT)/inc
export BUILD_EXE=$(BUILD_ROOT)/exe
export BUILD_SET=$(BUILD_ROOT)/set


########################################
###-----------APPLICATION env -------###
########################################
export APP_ROOT=$(ROOT_DIR)/application

# ------- APPLI XML env -----
export COMEDIA_APPLI_XML_ROOT=$(APP_ROOT)


########################################
########################################
###-----------MHAL env --------------###
########################################
########################################

export MHAL_ROOT=$(ROOT_DIR)/chal

# ------- TBOX env ----------------
export TBOX_ROOT=$(MHAL_ROOT)/tbox
export TBOX_INCLUDE=$(TBOX_ROOT)/inc
export TBOX_MAKE=$(BUILD_MAKE)
export TBOX_SRC_ROOT=$(call find_platform_folder, $(TBOX_ROOT)/src)
export TBOX_LIB_ROOT=$(call find_platform_folder, $(TBOX_ROOT)/lib)

# ------- TDAL env ----------------
export TDAL_ROOT=$(MHAL_ROOT)/tdal
export TDAL_INCLUDE=$(TDAL_ROOT)/inc
export TDAL_MAKE=$(BUILD_MAKE)
export TDAL_SRC_ROOT=$(call find_platform_folder, $(TDAL_ROOT)/src)
export TDAL_LIB_ROOT=$(call find_platform_folder, $(TDAL_ROOT)/lib)

export TDAL_ATA_INCLUDE=$(TDAL_INCLUDE)
export TDAL_AV_INCLUDE=$(TDAL_INCLUDE)
export TDAL_AVS_INCLUDE=$(TDAL_INCLUDE)
export TDAL_CI_INCLUDE=$(TDAL_INCLUDE)
export TDAL_COMMON_INCLUDE=$(TDAL_INCLUDE)
export TDAL_CRYPT_INCLUDE=$(TDAL_INCLUDE)
export TDAL_DESC_INCLUDE=$(TDAL_INCLUDE)
export TDAL_DIAG_INCLUDE=$(TDAL_INCLUDE)
export TDAL_DISP_INCLUDE=$(TDAL_INCLUDE)
export TDAL_DMD_INCLUDE=$(TDAL_INCLUDE)
export TDAL_DMX_INCLUDE=$(TDAL_INCLUDE)
export TDAL_FLA_INCLUDE=$(TDAL_INCLUDE)
export TDAL_FP_INCLUDE=$(TDAL_INCLUDE)
export TDAL_FS_INCLUDE=$(TDAL_INCLUDE)
export TDAL_GFX_INCLUDE=$(TDAL_INCLUDE)
export TDAL_HDMI_INCLUDE=$(TDAL_INCLUDE)
export TDAL_KBD_INCLUDE=$(TDAL_INCLUDE)
export TDAL_MP_INCLUDE=$(TDAL_INCLUDE)
export TDAL_NVM_INCLUDE=$(TDAL_INCLUDE)
export TDAL_OUTPUT_INCLUDE=$(TDAL_INCLUDE)
export TDAL_PANEL_INCLUDE=$(TDAL_INCLUDE)
export TDAL_PERSO_INCLUDE=$(TDAL_INCLUDE)
export TDAL_PTSM_INCLUDE=$(TDAL_INCLUDE)
export TDAL_PVR_INCLUDE=$(TDAL_INCLUDE)
export TDAL_RPC_INCLUDE=$(TDAL_INCLUDE)
export TDAL_SMC_INCLUDE=$(TDAL_INCLUDE)
export TDAL_SEC_INCLUDE=$(TDAL_INCLUDE)
export TDAL_TSIN_INCLUDE=$(TDAL_INCLUDE)
export TDAL_TS_INCLUDE=$(TDAL_INCLUDE)
export TDAL_UART_INCLUDE=$(TDAL_INCLUDE)
export TDAL_VBI_INCLUDE=$(TDAL_INCLUDE)

# ------- TDAL Tests env-----------
export TDAL_APPTESTS_ROOT=$(TDAL_ROOT)/apptests
export TDAL_APPTESTS_MAKE=$(BUILD_MAKE)

# ------- TKEL env ----------------
export TKEL_ROOT=$(MHAL_ROOT)/tkel
export TKEL_INCLUDE=$(TKEL_ROOT)/inc
export TKEL_MAKE=$(BUILD_MAKE)
export TKEL_SRC_ROOT=$(call find_platform_folder, $(TKEL_ROOT)/src)
export TKEL_LIB_ROOT=$(call find_platform_folder, $(TKEL_ROOT)/lib)

########################################
########################################
###--------MIDDLEWARE env -----------###
########################################
########################################

export MW_ROOT_DIR=$(ROOT_DIR)/middleware

# ------- ZLIB env ----------------
export ZLIB_ROOT=$(MW_ROOT_DIR)/core/src/zlib
export ZLIB_MAKE=$(BUILD_MAKE)
export ZLIB_INCLUDE=$(MW_ROOT_DIR)/core/include


########################################
###-----------PACKAGES env ----------###
########################################
export PACKAGES_ROOT=$(MW_ROOT_DIR)/packages

# ------- audio_video env ----------------
export AUDIO_VIDEO_ROOT=$(PACKAGES_ROOT)/audio_video
export AUDIO_VIDEO_MAKE=$(BUILD_MAKE)
export AUDIO_VIDEO_INCLUDE=$(AUDIO_VIDEO_ROOT)/inc

# ------- av_cluster env ----------------
export AV_CLUSTER_ROOT=$(PACKAGES_ROOT)/av_cluster
export AV_CLUSTER_MAKE=$(BUILD_MAKE)
export AV_CLUSTER_INCLUDE=$(AV_CLUSTER_ROOT)/inc

# ------- BGET env ----------------
export BGET_ROOT=$(PACKAGES_ROOT)/bget
export BGET_MAKE=$(BUILD_MAKE)
export BGET_INCLUDE=$(BGET_ROOT)/inc

#------- Bootloader Core env --------------------------
export BOOTLOADER_CORE_ROOT=$(PACKAGES_ROOT)/bootloader/core
export BOOTLOADER_CORE_MAKE=$(BUILD_MAKE)
export BOOTLOADER_CORE_INCLUDE=$(BOOTLOADER_CORE_ROOT)/inc

# ------- Bootloader Glue env --------------------------
export BOOTLOADER_GLUE_ROOT=$(PACKAGES_ROOT)/bootloader/external_glue
export BOOTLOADER_GLUE_MAKE=$(BUILD_MAKE)
export BOOTLOADER_GLUE_INCLUDE=$(BOOTLOADER_GLUE_ROOT)/inc

# ------- CAK DAL env -----------------
export CAK_DAL_ROOT=$(PACKAGES_ROOT)/cak_dal
export CAK_DAL_MAKE=$(BUILD_MAKE)
export CAK_DAL_INCLUDE=$(CAK_DAL_ROOT)/inc

# ------- CCAS env --------------------
export CCAS_ROOT=$(PACKAGES_ROOT)/ccas
export CCAS_MAKE=$(BUILD_MAKE)
export CCAS_INCLUDE=$(CCAS_ROOT)/inc

# ------- DateTime env ----------------
export DATETIME_ROOT=$(PACKAGES_ROOT)/datetime
export DATETIME_MAKE=$(BUILD_MAKE)
export DATETIME_INCLUDE=$(DATETIME_ROOT)/inc

ifeq "$(PRODUCT_USE_DDM)" "YES"
# ------- DDM CORE env -------------
export DDM_CORE_ROOT=$(PACKAGES_ROOT)/ddm/core
export DDM_CORE_MAKE=$(BUILD_MAKE)
export DDM_CORE_INCLUDE=$(DDM_CORE_ROOT)/inc

# ------- DDM GLUE env --------------
export DDM_GLUE_ROOT=$(PACKAGES_ROOT)/ddm/glue
export DDM_GLUE_MAKE=$(BUILD_MAKE)
export DDM_GLUE_INCLUDE=$(DDM_GLUE_ROOT)/inc
endif

# ------- DMJFS env ----------------------------
export DMJFS_ROOT=$(PACKAGES_ROOT)/dmjfs
export DMJFS_MAKE=$(BUILD_MAKE)
export DMJFS_INCLUDE=$(DMJFS_ROOT)/inc

# ------- DSMCC STACK env ----------------------------
export DSMCC_STACK_ROOT=$(PACKAGES_ROOT)/dsmcc_stack
export DSMCC_STACK_MAKE=$(BUILD_MAKE)
export DSMCC_STACK_INCLUDE=$(DSMCC_STACK_ROOT)/inc
export UFS_INCLUDE=$(DSMCC_STACK_ROOT)/src/ufs/inc

# ------- EIM env ----------------
export PACKAGE_EIM_ROOT=$(PACKAGES_ROOT)/event_information_manager
export PACKAGE_EIM_MAKE=$(BUILD_MAKE)
export PACKAGE_EIM_INCLUDE=$(PACKAGE_EIM_ROOT)/inc
export PACKAGE_EIT_DVB_INCLUDE=$(PACKAGE_EIM_ROOT)/inc/dvb
export PACKAGE_EIT_ATSC_INCLUDE=$(PACKAGE_EIM_ROOT)/inc/atsc

# ------- File System env --------------
export FILESYS_ROOT=$(PACKAGES_ROOT)/file_system
export FILESYS_MAKE=$(BUILD_MAKE)
export FILESYS_INCLUDE=$(FILESYS_ROOT)/inc

# ------------------ FONT env ------------------
export FONT_ROOT=$(PACKAGES_ROOT)/font
export FONT_INCLUDE=$(FONT_ROOT)/inc 

ifeq "$(USE_FREETYPE2)" "YES"
# ------- FreeType env -------------------------
export FREETYPE_ROOT=$(FONT_ROOT)/freetype
export FREETYPE_MAKE=$(BUILD_MAKE)
export FREETYPE_INCLUDE=$(FREETYPE_ROOT)/include
endif

ifeq "$(USE_CDAC)" "YES"
# ---------------CDAC env-----------------------
export CDAC_ROOT=$(FONT_ROOT)/cdac
export CDAC_INCLUDE=$(CDAC_ROOT)/inc
export CDAC_DEV_INCLUDE=$(CDAC_ROOT)/inc/DEV
export CDAC_BAN_INCLUDE=$(CDAC_ROOT)/inc/BAN
export CDAC_GUJ_INCLUDE=$(CDAC_ROOT)/inc/GUJ
export CDAC_KAN_INCLUDE=$(CDAC_ROOT)/inc/KAN
export CDAC_MAL_INCLUDE=$(CDAC_ROOT)/inc/MAL
export CDAC_PAN_INCLUDE=$(CDAC_ROOT)/inc/PAN
export CDAC_TEL_INCLUDE=$(CDAC_ROOT)/inc/TEL
export CDAC_TML_INCLUDE=$(CDAC_ROOT)/inc/TML
export CDAC_MAKE=$(BUILD_MAKE)
endif

# ------- SUDOKU env ----------------
export SUDOKU_ROOT=$(PACKAGES_ROOT)/games/sudoku
export SUDOKU_MAKE=$(BUILD_MAKE)
export SUDOKU_INCLUDE=$(SUDOKU_ROOT)/inc

# ------- GFX env ----------------
export GFX_ROOT=$(PACKAGES_ROOT)/gfx
export GFX_MAKE=$(BUILD_MAKE)
export GFX_INCLUDE=$(GFX_ROOT)/inc

# ------- libpng env ----------------------------
export PNG_ROOT=$(PACKAGES_ROOT)/libpng
export PNG_MAKE=$(BUILD_MAKE)
export PNG_INCLUDE=$(PNG_ROOT)/inc

# ------------- Maestro Celine Glue env -------------
# -------------- will bi moved in aplication folder!!!
export CELINE_PLAYER_GLUE_ROOT=$(PACKAGES_ROOT)/celine_player_glue
export CELINE_PLAYER_GLUE_MAKE=$(BUILD_MAKE)
export CELINE_PLAYER_GLUE_INCLUDE=$(CELINE_PLAYER_GLUE_ROOT)/inc

# ------- Maestro player env ----------------
export MAESTRO_PLAYER_ROOT=$(PACKAGES_ROOT)/maestro_player
export MAESTRO_PLAYER_MAKE=$(BUILD_MAKE)
export MAESTRO_PLAYER_INCLUDE=$(MAESTRO_PLAYER_ROOT)/inc

# ------- Maestro player glue env ----------------
export MAESTRO_PLAYER_GLUE_ROOT=$(PACKAGES_ROOT)/maestro_player_glue
export MAESTRO_PLAYER_GLUE_MAKE=$(BUILD_MAKE)
export MAESTRO_PLAYER_GLUE_INCLUDE=$(MAESTRO_PLAYER_GLUE_ROOT)/inc

# ------- Maestro player plugin env ----------------
# -------------- will bi moved in aplication folder!!!
export MAESTRO_PLAYER_PLUGIN_ROOT=$(PRODUCT_APPLICATION_PATH)/source/maestro_player_plugin
export MAESTRO_PLAYER_PLUGIN_MAKE=$(BUILD_MAKE)
export MAESTRO_PLAYER_PLUGIN_INCLUDE=$(MAESTRO_PLAYER_PLUGIN_ROOT)/inc

# ------- mediacentre env ----------------
export MEDIACENTRE_ROOT=$(PACKAGES_ROOT)/mediacentre

# ------- AVP env --------------
export AVP_ROOT=$(MEDIACENTRE_ROOT)/avp
export AVP_MAKE=$(BUILD_MAKE)
export AVP_INCLUDE=$(AVP_ROOT)/inc

# ------- MEDIA_BROWSER env ----------------
export MEDIA_BROWSER_ROOT=$(MEDIACENTRE_ROOT)/media_browser
export MEDIA_BROWSER_INCLUDE=$(MEDIA_BROWSER_ROOT)/inc

# ------- MEDIA_METADATA env ----------------
export MEDIA_METADATA_ROOT=$(MEDIACENTRE_ROOT)/media_metadata
export MEDIA_METADATA_INCLUDE=$(MEDIA_METADATA_ROOT)/inc

# ------- MEDIA_RENDERER env ----------------
export MEDIA_RENDERER_ROOT=$(MEDIACENTRE_ROOT)/media_renderer
export MEDIA_RENDERER_INCLUDE=$(MEDIA_RENDERER_ROOT)/inc

# ------- MEDIA_SOURCE env ----------------
export MEDIA_SOURCE_ROOT=$(MEDIACENTRE_ROOT)/media_source
export MEDIA_SOURCE_INCLUDE=$(MEDIA_SOURCE_ROOT)/inc

ifeq "$(PRODUCT_USE_SDEC)" "YES"
# ------- SDEC env --------------
export SDEC_ROOT=$(MEDIACENTRE_ROOT)/sdec
export SDEC_MAKE=$(BUILD_MAKE)
export SDEC_INCLUDE=$(SDEC_ROOT)/inc

# ------- SDEC GLUE env ----------------
export SDEC_GLUE_ROOT=$(MEDIACENTRE_ROOT)/sdec_glue
export SDEC_GLUE_MAKE=$(BUILD_MAKE)
export SDEC_GLUE_INCLUDE=$(SDEC_GLUE_ROOT)/inc
endif

ifeq "$(PRODUCT_APP)" "MAESTRO"
# ------- JPEG env --------------
export JPEG_ROOT=$(MEDIACENTRE_ROOT)/sdec/src/deco_jpeg
export JPEG_INCLUDE=$(JPEG_ROOT)
endif

ifeq "$(PRODUCT_APP)" "CELINE"
# ------- JPEG env --------------
export JPEG_ROOT=$(LIB_ROOT)/jpeg
export JPEG_INCLUDE=$(JPEG_ROOT)/include
export JPEG_LIB=$(call find_platform_folder, $(JPEG_ROOT)/lib)
endif

ifeq "$(MHEG5_VERSION)" "mheg5"
# ------- MHEG env ----------------
export MHEG5_ROOT=$(PACKAGES_ROOT)/mheg5

# ------- MHEG Core env ----------------
export MHEG5_CORE_ROOT=$(MHEG5_ROOT)/core
export MHEG5_CORE_MAKE=$(BUILD_MAKE)
export MHEG5_CORE_INCLUDE=$(MHEG5_CORE_ROOT)/inc

# ------- MHEG GLUE env ----------------
export MHEG5_GLUE_ROOT=$(MHEG5_ROOT)/external_glue
export MHEG5_GLUE_MAKE=$(BUILD_MAKE)
export MHEG5_GLUE_INCLUDE=$(MHEG5_GLUE_ROOT)/inc
export PRODUCT_USE_MHEG_FS=YES

# -------- FONT_ENGINE env -----------
export FNT_ROOT=$(MHEG5_ROOT)/font_engine
export NT_MAKE=$(BUILD_MAKE)
export FNT_INCLUDE=$(FNT_ROOT)/inc

# ------- text_rendering env ---------------
export TRD_ROOT=$(MHEG5_ROOT)/text_rendering
export TRD_MAKE=$(BUILD_MAKE)
export TRD_INCLUDE=$(TRD_ROOT)/inc
endif

# ------- PIDB env ----------------
export PIDB_ROOT=$(PACKAGES_ROOT)/pidb
export PIDB_MAKE=$(BUILD_MAKE)
export PIDB_INCLUDE=$(PIDB_ROOT)/inc

# ------- PVR MANAGER env ----------------
export PVRM_ROOT=$(PACKAGES_ROOT)/pvr_manager
export PVRM_MAKE=$(BUILD_MAKE)
export PVRM_INCLUDE=$(PVRM_ROOT)/inc

# ------- SLIST env ----------------
export SLIST_ROOT=$(PACKAGES_ROOT)/slist
export SLIST_MAKE=$(BUILD_MAKE)
export SLIST_INCLUDE=$(SLIST_ROOT)/inc

# ------- SUBTITLE env ----------------
export SUBTITLE_ROOT=$(PACKAGES_ROOT)/subtitle
export SUBTITLE_MAKE=$(BUILD_MAKE)
export SUBTITLE_INCLUDE=$(SUBTITLE_ROOT)/inc

# ------- SUBTITLE GLUE env ----------------
export SUBTITLE_GLUE_ROOT=$(PACKAGES_ROOT)/subtitle_glue
export SUBTITLE_GLUE_MAKE=$(BUILD_MAKE)
export SUBTITLE_GLUE_INCLUDE=$(SUBTITLE_GLUE_ROOT)/inc

# ------- TABLE env ----------------
export TABLE_ROOT=$(PACKAGES_ROOT)/table
export TABLE_MAKE=$(BUILD_MAKE)
export TABLE_INCLUDE=$(TABLE_ROOT)/inc

# ------- TEXT env ----------------
export TEXT_ROOT=$(PACKAGES_ROOT)/text
export TEXT_MAKE=$(BUILD_MAKE)
export TEXT_INCLUDE=$(TEXT_ROOT)/inc

# ------- TRIGGER env -------------
export TRIGGER_ROOT=$(PACKAGES_ROOT)/trigger
export TRIGGER_MAKE=$(BUILD_MAKE)
export TRIGGER_INCLUDE=$(TRIGGER_ROOT)/inc

# ------- TTXT env ----------------
export TTXT_ROOT=$(PACKAGES_ROOT)/ttxt
export TTXT_MAKE=$(BUILD_MAKE)
export TTXT_INCLUDE=$(TTXT_ROOT)/inc

# ------- TTXT GLUE env ----------------
export TTXT_GLUE_ROOT=$(PACKAGES_ROOT)/ttxt_glue
export TTXT_GLUE_MAKE=$(BUILD_MAKE)
export TTXT_GLUE_INCLUDE=$(TTXT_GLUE_ROOT)/inc

# ------- XPEL env ----------------
export XPEL_ROOT=$(PACKAGES_ROOT)/xpel
export XPEL_MAKE=$(BUILD_MAKE)
export XPEL_INCLUDE=$(XPEL_ROOT)/inc


########################################
###-------------TEST env ------------###
########################################
export TEST_ROOT=$(ROOT_DIR)/test

# ------- chal tests env --------
export CHAL_TESTS_ROOT=$(TEST_ROOT)/chalvalidator/chaltests
export CHAL_TESTS_INC=$(CHAL_TESTS_ROOT)/inc

# ------- test manager env --------
export TEST_MANAGER_ROOT=$(TEST_ROOT)/chalvalidator/testmanager
export TEST_MANAGER_INCLUDE=$(TEST_MANAGER_ROOT)/inc

# ------- PACK Test env-----------
export PACK_TEST_ROOT=$(TEST_ROOT)/pack_test
export PACK_TEST_MAKE=$(BUILD_MAKE)
export PACK_TEST_INCLUDE=$(PACK_TEST_ROOT)/inc

# ------- trigger_test env-----------
export PACK_TRIGGER_TEST_ROOT=$(TEST_ROOT)/trigger_test
export PACK_TRIGGER_TEST_INCLUDE=$(PACK_TRIGGER_TEST_ROOT)/inc



########################################
###----------LIBRARIES env ----------###
########################################
export LIB_ROOT=$(ROOT_DIR)/external_libs

# ----------- CAK env -------------
export CAK_ROOT=$(LIB_ROOT)/$(CAK_LIBDIR)
export CAK_INCLUDE=$(CAK_ROOT)/include

# ------- NOTIFY env ----------------
export NOTIFY_ROOT=$(LIB_ROOT)/Notify
export NOTIFY_INCLUDE=$(NOTIFY_ROOT)/inc
export NOTIFY_LIB= $(call find_platform_folder, $(NOTIFY_ROOT)/lib)/release

#----------- LUA -------------------
export LUA_ROOT=$(LIB_ROOT)/Lua
export LUA_INCLUDE=$(LUA_ROOT)/include
export LUA_LIB=$(call find_platform_folder, $(LUA_ROOT)/lib)

#--------------- TOLUA++---------
export TOLUAPP_ROOT=$(LIB_ROOT)/tolua++
export TOLUAPP_INCLUDE=$(TOLUAPP_ROOT)/include
export TOLUAPP_LIB=$(call find_platform_folder, $(TOLUAPP_ROOT)/lib)

#----------- MINIZIP---------------
export MINIZIP_ROOT=$(LIB_ROOT)/minizip
export MINIZIP_INCLUDE=$(MINIZIP_ROOT)/include
export MINIZIP_LIB=$(call find_platform_folder, $(MINIZIP_ROOT)/lib)

#------------- SILLY----------
export SILLY_ROOT=$(LIB_ROOT)/SILLY-0.1.0
export SILLY_INCLUDE=$(SILLY_ROOT)/include
export SILLY_LIB=$(call find_platform_folder, $(SILLY_ROOT)/lib)

#------------- TinyXML-----------
export TiXML_ROOT=$(LIB_ROOT)/tinyxml
export TiXML_INCLUDE=$(TiXML_ROOT)/include
export TiXML_LIB=$(call find_platform_folder, $(TiXML_ROOT)/lib)

#------------ Minizip----------
export MINIZIP_ROOT=$(LIB_ROOT)/minizip
export MINIZIP_INCLUDE=$(MINIZIP_ROOT)/include
export MINIZIP_LIB=$(call find_platform_folder, $(MINIZIP_ROOT)/lib)


########################################
###------------TOOLS env ------------###
########################################
export TOOL_ROOT=$(ROOT_DIR)/tools

# ------- Bootloader env ----------------
export BOOTLOADER_ROOT=$(TOOL_ROOT)/bootcode_tools

# ------- Flash Tool env ----------------
export FLASHTOOL_ROOT?=$(TOOL_ROOT)/flashtool
export FLASHTOOL_MAKE=$(BUILD_MAKE)
export FLASHTOOL_PLANS_ROOT?=$(BUILD_ROOT)/plans
export FLASHTOOL_MAPS_ROOT?=$(BUILD_ROOT)/maps

# ------- UNZIP Tool env ----------------
export UNZIP_TOOL_ROOT=$(TOOL_ROOT)/unzip

### ------- celine_player env -------------
export CELINE_PLAYER_ROOT=$(PACKAGES_ROOT)/celine_player
export CELINE_PLAYER_INCLUDE=$(CELINE_PLAYER_ROOT)/include
export PLAYER_CONFIG_INCLUDE=$(CELINE_PLAYER_ROOT)/include/config
export CELINE_PLAYER_DEP_INCLUDE=$(CELINE_PLAYER_ROOT)/deps/include
export CELINE_PLAYER_SRC=$(CELINE_PLAYER_ROOT)/src
