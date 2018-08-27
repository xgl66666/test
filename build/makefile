#----------------------------------------------------------------------
#
#           Root Makefile for comedia product
#
#----------------------------------------------------------------------
###################################################
#	set environnement
###################################################
ifeq "$(ROOT_DIR)" ""
export ROOT_DIR=$(abspath $(CURDIR)/..)
endif
export BUILD_ROOT=$(ROOT_DIR)/build
export BUILD_MAKE=$(BUILD_ROOT)/make
export BUILD_EXPORTS=$(BUILD_ROOT)/bin
export BUILD_INCLUDE=$(BUILD_ROOT)/inc
export BUILD_EXE=$(BUILD_ROOT)/exe
export BUILD_SET=$(BUILD_ROOT)/set

ifneq ("$(wildcard $(BUILD_SET)/$(SETENV).mak)","")
include $(BUILD_SET)/$(SETENV).mak
else
$(error File "$(BUILD_SET)/$(SETENV).mak" does not exists!)
endif

include $(BUILD_MAKE)/paths.mak

###################################################
#	load compilation environnement
###################################################

# Discover HOST OS
ifeq ($(OS),Windows_NT)
    HOST_OS=windows
else
    UNAME := $(shell uname -s)
    ifeq ($(UNAME),Linux)
        HOST_OS=linux
    endif
    ifeq ($(UNAME),Darwin)
        HOST_OS=android
    endif
endif


include $(BUILD_MAKE)/buildexe.mak
include $(BUILD_MAKE)/environnement.mak

# Strips the libraries
REMOVE_LIB_PREFIX = $(patsubst lib%.a,%,$(1))

PARTITIONA=partitionA
EMS_CONFIG_FILE=ems_config

.PHONY: flashtool sdk resources fullflash_image dist dist_prep dist_release dist_debug

###################################################
#	DTV application
###################################################

# ****************************************
# exe makefile
# ****************************************
# Target: CHAL
chal: $(PRODUCTLIB_CHAL)

# Target: Comedia as an executable
$(PRODUCT_NAME): $(PRODUCTLIB_CHAL) $(PRODUCTLIB) $(PRODUCT_NAME).$(BIN_SUFFIX)

ifeq "$(HOST_OS)" "windows"
$(PRODUCT_NAME).$(EXE_SUFFIX):
	@$(ECHO) Create $(PRODUCT_NAME).$(EXE_SUFFIX) ...
	$(CC_PLUS) \
	$(LIBRARY_PREFIX)$(BUILD_EXPORTS) \
    $(addprefix $(LIBRARY_PREFIX), $(OS_LIBS_PATH)) \
    $(addprefix $(LIBRARY_PREFIX), $(SDK_LIBS_PATH)) \
    $(addprefix $(LIBRARY_PREFIX), $(CAK_LIBS_PATH)) \
    $(addprefix $(LIBRARY_PREFIX), $(EXTERN_LIBS_PATH)) \
	$(BUILD_EXPORTS)/$(PRODUCT_MAIN).$(OBJ_SUFFIX) \
	-Wl,--start-group $(addprefix -l, $(call REMOVE_LIB_PREFIX, $(OS_LIBS) $(SDK_LIBS) $(CAK_LIBS) $(EXTERN_LIBS) $(PRODUCTLIB_CHAL) $(PRODUCTLIB))) -Wl,--end-group \
	-Wl,--gc-sections $(SDK_LD_FLAGS) -g \
    -o $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_EXPORTS)/$(PRODUCT_NAME).$(EXE_SUFFIX)) \
    -Wl,-Map=$(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_EXPORTS)/$(PRODUCT_NAME).map) 
	$(CP) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(SDK_DEPS)/*.*) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_EXPORTS))
endif 

ifeq "$(HOST_OS)" "linux"
$(PRODUCT_NAME).$(EXE_SUFFIX):
	$(ECHO) Create $(PRODUCT_NAME).$(EXE_SUFFIX) ...
	$(LD) \
    $(LIBRARY_PREFIX)$(BUILD_EXPORTS) \
    $(addprefix $(LIBRARY_PREFIX), $(OS_LIBS_PATH)) \
    $(addprefix $(LIBRARY_PREFIX), $(SDK_LIBS_PATH)) \
    $(addprefix $(LIBRARY_PREFIX), $(CAK_LIBS_PATH)) \
    $(addprefix $(LIBRARY_PREFIX), $(EXTERN_LIBS_PATH)) \
	$(BUILD_EXPORTS)/$(PRODUCT_MAIN).$(OBJ_SUFFIX) \
	--start-group $(addprefix -l, $(call REMOVE_LIB_PREFIX, $(OS_LIBS) $(SDK_LIBS) $(CAK_LIBS) $(EXTERN_LIBS) $(PRODUCTLIB_CHAL) $(PRODUCTLIB))) --end-group \
	--gc-sections $(SDK_LD_FLAGS) -g \
    -o  $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_EXPORTS)/$(PRODUCT_NAME).$(EXE_SUFFIX)) \
    -Map=$(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_EXPORTS)/$(PRODUCT_NAME).map)
endif

$(PRODUCT_NAME).$(BIN_SUFFIX): $(PRODUCT_NAME).$(EXE_SUFFIX)
#   Strip application binary
	$(STRIP) -s $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/bin/$(PRODUCT_NAME).$(EXE_SUFFIX)) -o $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/bin/$(PRODUCT_NAME).$(BIN_SUFFIX))
	$(OBJ_COPY) -O binary $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/bin/$(PRODUCT_NAME).$(BIN_SUFFIX))
#   Preparing SSU image
	$(MAKE) -ikC $(subst $(BAD_SLASH),$(GOOD_SLASH),$(SDK_ROOT)) image
	@$(ECHO) --------------------------------------------------
	@$(ECHO) ------- Files ready in build/bin         ---------
	@$(ECHO) --------------------------------------------------


## This rules is only useful for developers to save some compilation time.
## When they make small changes that don't need a full compilation : they can
## compile only the modified library and then call 'gmake link" to perform the
## creation of 'lku' files.
## e.g. : "gmake -s comedia.lib link"
## (it must be used with care because all dependencies are skipped)
link:
	@$(ECHO) linking Full comedia ...
	$(MAKE) -kC $(BUILD_EXE) $(PRODUCT_NAME).$(EXE_SUFFIX)

run:
	@$(ECHO) running comedia ...
	$(MAKE) -kC $(subst $(BAD_SLASH),$(GOOD_SLASH),$(SDK_ROOT)) run PRODUCT_NAME=$(PRODUCT_NAME)

debug:
	@$(ECHO) debugging comedia ...
	$(MAKE) -kC $(subst $(BAD_SLASH),$(GOOD_SLASH),$(SDK_ROOT)) debug PRODUCT_NAME=$(PRODUCT_NAME)

run_rom:
	@$(ECHO) debugging comedia in ROM ...
	$(MAKE) -kC $(BUILD_EXE) $(PRODUCT_NAME)_debug_rom

###################################################
#	Flash Tool
###################################################

flashtool:
	@$(ECHO) Execute command manually to build flashtool:
	@$(ECHO) $(MAKE) -C $(FLASHTOOL_ROOT) all

clean_flashtool:
	@$(ECHO) Execute command manually to clean flashtool:
	@$(ECHO) $(MAKE) -C $(FLASHTOOL_ROOT) clean
	
###################################################
#       Generating application resources
###################################################

resources:
ifeq ($(PRODUCT_FLASH_MAP),)
	$(error Variable PRODUCT_FLASH_MAP is not set)
else
ifeq ($(PRODUCT_APPLICATION_PLAN),)
	$(error Variable PRODUCT_APPLICATION_PLAN is not set)
else
ifeq ($(PRODUCT_APPLICATION_PATH),)
    $(error Variable PRODUCT_APPLICATION_PATH is not set)
else
	$(MAKE) -C $(PRODUCT_APPLICATION_PATH)
	$(MAKE) -C $(FLASHTOOL_ROOT) run MAP=$(FLASHTOOL_MAPS_ROOT)/$(PRODUCT_FLASH_MAP) PLAN=$(FLASHTOOL_PLANS_ROOT)/$(PRODUCT_APPLICATION_PLAN)
endif
endif
endif

###################################################
#       Generating full flash image
###################################################

image:
ifeq ($(PRODUCT_FULL_FLASH_MAP),)
	$(error Variable PRODUCT_FULL_FLASH_MAP is not set)
else
ifeq ($(PRODUCT_FULL_FLASH_PLAN),)
	$(error Variable PRODUCT_FULL_FLASH_PLAN is not set)
else
	$(MAKE) -C $(FLASHTOOL_ROOT) run MAP=$(FLASHTOOL_MAPS_ROOT)/$(PRODUCT_FULL_FLASH_MAP) PLAN=$(FLASHTOOL_PLANS_ROOT)/$(PRODUCT_FULL_FLASH_PLAN)
endif
endif 


full_image:
ifeq ($(PRODUCT_FULL_FLASH_MAP),)
	$(error Variable PRODUCT_FULL_FLASH_MAP is not set)
else
ifeq ($(PRODUCT_FULL_FLASH_PLAN),)
	$(error Variable PRODUCT_FULL_FLASH_PLAN is not set)
else
	$(MAKE) -C $(FLASHTOOL_ROOT) run MAP=$(FLASHTOOL_MAPS_ROOT)/$(PRODUCT_FULL_FLASH_MAP) PLAN=$(FLASHTOOL_PLANS_ROOT)/$(PRODUCT_FULL_FLASH_PLAN)
endif
endif 

clean_images: 
	$(RM) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_EXPORTS)/*.$(BIN_SUFFIX))
	$(RM) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_EXPORTS)/*.fakesign)
	$(RM) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_EXPORTS)/*.map)
	$(RM) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_EXPORTS)/images/*.idc)
	$(RM) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_EXPORTS)/images/*.idn)
	
###################################################
#       Building SDK
###################################################

sdk:
	@$(ECHO) Compiling $(PRODUCT_CHIP) SDK ...
	$(MAKE) -C $(subst $(BAD_SLASH),$(GOOD_SLASH),$(SDK_ROOT))
	
clean_sdk:
	@$(ECHO) Cleaning $(PRODUCT_CHIP) SDK ...
	$(MAKE) -C $(subst $(BAD_SLASH),$(GOOD_SLASH),$(SDK_ROOT)) clean

###################################################
#	Generating distrubution
###################################################

dist: dist_clean dist_prep dist_sdk clean_sdk sdk dist_res dist_release dist_debug

dist_clean:
	$(RMDIR) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME))

dist_prep:
	$(MKDIR) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME))
	$(MKDIR) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(LIB_NAME))
	$(MKDIR) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(LIB_NAME)/release)
	$(MKDIR) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(LIB_NAME)/debug)
	$(MKDIR) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(BIN_NAME))
	$(MKDIR) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(BIN_NAME)/release)
	$(MKDIR) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(BIN_NAME)/debug)
	$(MKDIR) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(RES_NAME))
	$(MKDIR) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(SDK_NAME))

dist_release:
	$(MAKE) -kC $(BUILD_ROOT) clean_all
	$(MAKE) -C $(BUILD_ROOT) $(PRODUCT_NAME)
	$(CP) $(BUILD_EXPORTS)/main.o $(BUILD_ROOT)/$(DIST_NAME)/$(LIB_NAME)/release
	$(CP) $(BUILD_EXPORTS)/libmiddleware.a $(BUILD_ROOT)/$(DIST_NAME)/$(LIB_NAME)/release
	$(CP) $(BUILD_EXPORTS)/comedia_lzma_ota.bin $(BUILD_ROOT)/$(DIST_NAME)/$(BIN_NAME)/release/application.bin
	
dist_debug:
	$(MAKE) -kC $(BUILD_ROOT) clean_all
	$(MAKE) -C $(BUILD_ROOT) $(PRODUCT_NAME) TRACES=UART
	$(CP) $(BUILD_EXPORTS)/main.o $(BUILD_ROOT)/$(DIST_NAME)/$(LIB_NAME)/debug
	$(CP) $(BUILD_EXPORTS)/libmiddleware.a $(BUILD_ROOT)/$(DIST_NAME)/$(LIB_NAME)/debug
	$(CP) $(BUILD_EXPORTS)/comedia_lzma_ota.bin $(BUILD_ROOT)/$(DIST_NAME)/$(BIN_NAME)/debug/application.bin

dist_sdk:
	$(MKDIR) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(SDK_NAME)/build)
	$(MKDIR) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(SDK_NAME)/build/bin)
	$(MKDIR) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(SDK_NAME)/build/bin/resources)
	$(MKDIR) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(SDK_NAME)/build/set)
	$(MKDIR) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(SDK_NAME)/test)
	$(MKDIR) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(SDK_NAME)/sdk)
	$(MKDIR) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(SDK_NAME)/chal)
	$(MKDIR) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(SDK_NAME)/chal/tdal)
	$(MKDIR) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(SDK_NAME)/chal/tdal/src)
	$(MKDIR) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(SDK_NAME)/chal/tdal/lib)
	$(MKDIR) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(SDK_NAME)/chal/tbox)
	$(MKDIR) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(SDK_NAME)/chal/tbox/src)
	$(MKDIR) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(SDK_NAME)/chal/tbox/lib)
	$(MKDIR) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(SDK_NAME)/chal/tkel)
	$(MKDIR) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(SDK_NAME)/chal/tkel/src)
	$(MKDIR) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(SDK_NAME)/chal/tkel/lib)
	$(MKDIR) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(SDK_NAME)/external_libs)
	$(CP) $(CAK_ROOT) $(BUILD_ROOT)/$(DIST_NAME)/$(SDK_NAME)/external_libs
	$(CP) $(TDAL_INCLUDE) $(BUILD_ROOT)/$(DIST_NAME)/$(SDK_NAME)/chal/tdal
	$(CP) $(TDAL_ROOT)/makefile $(BUILD_ROOT)/$(DIST_NAME)/$(SDK_NAME)/chal/tdal
	$(CP) $(TDAL_ROOT)/doc $(BUILD_ROOT)/$(DIST_NAME)/$(SDK_NAME)/chal/tdal
	$(if  $(TDAL_SRC_ROOT), $(CP) $(TDAL_SRC_ROOT) $(BUILD_ROOT)/$(DIST_NAME)/$(SDK_NAME)/chal/tdal/src)
	$(if  $(TDAL_LIB_ROOT), $(CP) $(TDAL_LIB_ROOT) $(BUILD_ROOT)/$(DIST_NAME)/$(SDK_NAME)/chal/tdal/lib)
	$(CP) $(TBOX_INCLUDE) $(BUILD_ROOT)/$(DIST_NAME)/$(SDK_NAME)/chal/tbox
	$(CP) $(TBOX_ROOT)/makefile $(BUILD_ROOT)/$(DIST_NAME)/$(SDK_NAME)/chal/tbox
	$(CP) $(TBOX_ROOT)/src/tbox.c $(BUILD_ROOT)/$(DIST_NAME)/$(SDK_NAME)/chal/tbox/src
	$(CP) $(TBOX_ROOT)/doc $(BUILD_ROOT)/$(DIST_NAME)/$(SDK_NAME)/chal/tbox
	$(if  $(TBOX_SRC_ROOT), $(CP) $(TBOX_SRC_ROOT) $(BUILD_ROOT)/$(DIST_NAME)/$(SDK_NAME)/chal/tbox/src)
	$(if  $(TBOX_LIB_ROOT), $(CP) $(TBOX_LIB_ROOT) $(BUILD_ROOT)/$(DIST_NAME)/$(SDK_NAME)/chal/tbox/lib)
	$(CP) $(TKEL_INCLUDE) $(BUILD_ROOT)/$(DIST_NAME)/$(SDK_NAME)/chal/tkel
	$(CP) $(TKEL_ROOT)/makefile $(BUILD_ROOT)/$(DIST_NAME)/$(SDK_NAME)/chal/tkel
	$(CP) $(TKEL_ROOT)/doc $(BUILD_ROOT)/$(DIST_NAME)/$(SDK_NAME)/chal/tkel
	$(if  $(TKEL_SRC_ROOT), $(CP) $(TKEL_SRC_ROOT) $(BUILD_ROOT)/$(DIST_NAME)/$(SDK_NAME)/chal/tkel/src)
	$(if  $(TKEL_LIB_ROOT), $(CP) $(TKEL_LIB_ROOT) $(BUILD_ROOT)/$(DIST_NAME)/$(SDK_NAME)/chal/tkel/lib)
	$(CP) $(SDK_ROOT) $(BUILD_ROOT)/$(DIST_NAME)/$(SDK_NAME)/sdk
	$(CP) $(BUILD_ROOT)/set/$(SETENV).mak $(BUILD_ROOT)/$(DIST_NAME)/$(SDK_NAME)/build/set
	$(CP) $(BUILD_ROOT)/make $(BUILD_ROOT)/$(DIST_NAME)/$(SDK_NAME)/build/
	$(CP) $(BUILD_ROOT)/inc $(BUILD_ROOT)/$(DIST_NAME)/$(SDK_NAME)/build/
	$(CP) $(BUILD_ROOT)/src $(BUILD_ROOT)/$(DIST_NAME)/$(SDK_NAME)/build/
	$(CP) $(BUILD_ROOT)/makefile $(BUILD_ROOT)/$(DIST_NAME)/$(SDK_NAME)/build/
	$(CP) $(BUILD_ROOT)/../makefile $(BUILD_ROOT)/$(DIST_NAME)/$(SDK_NAME)/
	$(CP) $(BUILD_ROOT)/../test/chalvalidator $(BUILD_ROOT)/$(DIST_NAME)/$(SDK_NAME)/test

dist_res: resources
	$(CP) $(BUILD_EXPORTS)/resources/resources.lzma $(BUILD_ROOT)/$(DIST_NAME)/$(RES_NAME)/resources.lzma

###################################################
#	HAL validator
###################################################

# Target: CHAL_VALIDATOR as an executable
$(PRODUCT_CHAL_VALIDATOR_NAME): clean_tkel clean_tbox clean_trace $(PRODUCTLIB_CHAL) $(CHALVALIDATORLIB) $(PRODUCT_CHAL_VALIDATOR_NAME).$(BIN_SUFFIX)

ifeq "$(HOST_OS)" "windows"
$(PRODUCT_CHAL_VALIDATOR_NAME).$(EXE_SUFFIX):
	@$(ECHO) Create $(PRODUCT_CHAL_VALIDATOR_NAME).$(EXE_SUFFIX) ...
	$(CC_PLUS) \
	$(LIBRARY_PREFIX)$(BUILD_EXPORTS) \
    $(addprefix $(LIBRARY_PREFIX), $(OS_LIBS_PATH)) \
    $(addprefix $(LIBRARY_PREFIX), $(SDK_LIBS_PATH)) \
    $(addprefix $(LIBRARY_PREFIX), $(CAK_LIBS_PATH)) \
    $(addprefix $(LIBRARY_PREFIX), $(EXTERN_LIBS_PATH)) \
	$(BUILD_EXPORTS)/main_$(PRODUCT_CHAL_VALIDATOR_NAME).$(OBJ_SUFFIX) \
	-Wl,--start-group $(addprefix -l, $(call REMOVE_LIB_PREFIX, $(OS_LIBS) $(SDK_LIBS) $(CAK_LIBS) $(EXTERN_LIBS) $(PRODUCTLIB_CHAL) $(CHALVALIDATORLIB))) -Wl,--end-group \
	-Wl,--gc-sections -static -g $(SDK_LD_FLAGS)\
    -o $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_EXPORTS)/$(PRODUCT_CHAL_VALIDATOR_NAME).$(EXE_SUFFIX)) \
    -Wl,-Map=$(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_EXPORTS)/$(PRODUCT_CHAL_VALIDATOR_NAME).map)
	$(CP) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(SDK_DEPS)/*.*) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_EXPORTS))
endif


ifeq "$(HOST_OS)" "linux"
$(PRODUCT_CHAL_VALIDATOR_NAME).$(EXE_SUFFIX):
	$(ECHO) Create $(PRODUCT_CHAL_VALIDATOR_NAME).$(EXE_SUFFIX) ...
	$(LD) \
    $(LIBRARY_PREFIX)$(BUILD_EXPORTS) \
    $(addprefix $(LIBRARY_PREFIX), $(OS_LIBS_PATH)) \
    $(addprefix $(LIBRARY_PREFIX), $(SDK_LIBS_PATH)) \
    $(addprefix $(LIBRARY_PREFIX), $(CAK_LIBS_PATH)) \
    $(addprefix $(LIBRARY_PREFIX), $(EXTERN_LIBS_PATH)) \
	$(BUILD_EXPORTS)/main_$(PRODUCT_CHAL_VALIDATOR_NAME).$(OBJ_SUFFIX) \
	--start-group $(addprefix -l, $(call REMOVE_LIB_PREFIX, $(OS_LIBS) $(SDK_LIBS) $(CAK_LIBS) $(EXTERN_LIBS) $(PRODUCTLIB_CHAL) $(CHALVALIDATORLIB))) --end-group \
	--gc-sections $(SDK_LD_FLAGS) -g \
    -o  $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_EXPORTS)/$(PRODUCT_CHAL_VALIDATOR_NAME).$(EXE_SUFFIX)) \
    -Map=$(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_EXPORTS)/$(PRODUCT_CHAL_VALIDATOR_NAME).map)
endif

$(PRODUCT_CHAL_VALIDATOR_NAME).$(BIN_SUFFIX): $(PRODUCT_CHAL_VALIDATOR_NAME).$(EXE_SUFFIX)
	$(STRIP) -s $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/bin/$(PRODUCT_CHAL_VALIDATOR_NAME).$(EXE_SUFFIX)) -o $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/bin/$(PRODUCT_CHAL_VALIDATOR_NAME).$(BIN_SUFFIX))
	$(OBJ_COPY) -O binary $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/bin/$(PRODUCT_CHAL_VALIDATOR_NAME).$(BIN_SUFFIX))

$(PRODUCT_CHAL_VALIDATOR_NAME)_run:
	@$(ECHO) running chalvalidator application ...
	$(MAKE) -kC $(BUILD_ROOT) $(PRODUCT_CHAL_VALIDATOR_NAME)_run

$(PRODUCT_CHAL_VALIDATOR_NAME)_debug:
	@$(ECHO) debugging chalvalidator application ...
	$(MAKE) -kC $(BUILD_ROOT) $(PRODUCT_CHAL_VALIDATOR_NAME)_debug

clean_chaltests:
	@$(ECHO) cleaning chaltests ...
	$(MAKE) -ikC $(CHAL_TESTS_ROOT) clean_all
	$(MAKE) -ikC $(TEST_MANAGER_ROOT) clean_all

clean_testmanager:
	@$(ECHO) cleaning testmanager ...
	$(MAKE) -ikC $(TEST_MANAGER_ROOT) clean_all

###################################################
#	external_libs
###################################################
jpeg:
	@$(ECHO) building jpeg...
	$(MAKE) -ikC $(JPEG_ROOT)/src

clean_jpeg:
	@$(ECHO) cleaning jpeg ...
	$(MAKE) -ikC $(JPEG_ROOT)/src clean

lua:
	@$(ECHO) building Lua...
	$(MAKE) -ikC $(LUA_ROOT)/src

clean_lua:
	@$(ECHO) cleaning Lua...
	$(MAKE) -ikC $(LUA_ROOT)/src clean

minizip:
	@$(ECHO) building minizip...
	$(MAKE) -ikC $(MINIZIP_ROOT)/src

clean_minizip:
	@$(ECHO) cleaning minizip...
	$(MAKE) -ikC $(MINIZIP_ROOT)/src clean

silly:
	@$(ECHO) building SILLY...
	$(MAKE) -ikC $(SILLY_ROOT)/src

clean_silly:
	@$(ECHO) cleaning SILLY...
	$(MAKE) -ikC $(SILLY_ROOT)/src clean_silly

tinyxml:
	@$(ECHO) building TinyXML...
	$(MAKE) -ikC $(TiXML_ROOT)/src

clean_tinyxml:
	@$(ECHO) cleaning TinyXML...
	$(MAKE) -ikC $(TiXML_ROOT)/src clean

toluapp:
	@$(ECHO) building tolua++...
	$(MAKE) -ikC $(TOLUAPP_ROOT)/src

clean_toluapp:
	@$(ECHO) cleaning tolua++...
	$(MAKE) -ikC $(TOLUAPP_ROOT)/src clean
	
###################################################
#	Various clean
###################################################

clean:
	$(ECHO) cleaning ...
	$(MAKE) -ikC $(TDAL_ROOT) clean_all
	$(MAKE) -ikC $(TKEL_ROOT) clean_all
	$(MAKE) -ikC $(TBOX_ROOT) clean_all
	-$(MAKE) -ikC $(TDAL_APPTESTS_ROOT)/src clean_all
	-$(MAKE) -ikC $(CHAL_TESTS_ROOT) clean_all
	-$(MAKE) -ikC $(TEST_MANAGER_ROOT) clean_all
	-$(MAKE) -ikC $(MW_ROOT_DIR) clean_all
ifeq "$(PACK_TEST_CM_TEST)" "1"
	$(MAKE) -ikC $(PACK_CM_TEST_ROOT) clean_all
	$(MAKE) -ikC $(PACK_TEST_ROOT) clean_all
endif
ifeq "$(PACK_TEST_CM_DEMO)" "1"
	$(MAKE) -ikC $(PACK_CM_DEMO_ROOT) clean_all
	$(MAKE) -ikC $(PACK_TEST_ROOT) clean_all
endif
ifeq "$(PACK_TEST_TABLE)" "1"
	$(MAKE) -ikC $(PACK_TABLE_TEST_ROOT) clean_all
endif
ifeq "$(PACK_TEST_PIDB)" "1"
	$(MAKE) -ikC $(PACK_PIDB_TEST_ROOT) clean_all
endif
ifeq "$(PACK_TEST_TRIGGER)" "1"
	$(MAKE) -ikC $(PACK_TRIGGER_TEST_ROOT) clean_all
endif
ifeq "$(PACK_TEST_DATETIME)" "1"
	$(MAKE) -ikC $(PACK_DATETIME_TEST_ROOT) clean_all
endif
ifeq "$(PACK_TEST_EIM)" "1"
	$(MAKE) -ikC $(PACK_EIM_TEST_ROOT) clean_all
endif
ifeq "$(PACK_TEST_RENDERER)" "1"
	$(MAKE) -ikC $(PACK_RENDERER_TEST_ROOT) clean_all
	$(MAKE) -ikC $(AUDIO_VIDEO_ROOT) clean_all
endif
	$(MAKE) -ikC $(BUILD_ROOT)/src clean_all
	$(RM) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_EXPORTS)/*.$(LIB_SUFFIX))
	$(RM) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_EXPORTS)/*.$(OBJ_SUFFIX))
#	$(RM) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/src/$(HEADER_FILE_NAME_PARTITIONA).h)
#	$(RM) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/src/$(HEADER_FILE_NAME_EMS_CONFIG).h)

clean_all: clean clean_images
	@$(ECHO)  *********** clean all in $(BUILD_EXPORTS) directory **********
	$(RM) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_EXPORTS)/*.$(LIB_SUFFIX))
	$(RM) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_EXPORTS)/*.$(EXE_SUFFIX))
	$(RM) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_EXPORTS)/*.$(OBJ_SUFFIX))
	$(RM) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_EXPORTS)/*.$(MAP_SUFFIX))
	$(RM) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_EXPORTS)/*.$(BIN_SUFFIX))
	$(RM) $(BUILD_EXPORTS)/resources/resources.lzma
ifeq "$(PRODUCT_PROPRIETARY_BOOTLOADER)" "TELESYSTEM"
	$(RM) ../tools/gtpl_generate_app_zip/out/*
endif

clean_tdal:
ifneq "$(KEEP_TDAL)" "1"
	@$(ECHO) cleaning TDAL ...
	$(MAKE) -ikC $(TDAL_ROOT) clean_all
endif	

clean_tkel:
ifneq "$(KEEP_TKEL)" "1"
	@$(ECHO) cleaning TKEL ...
	$(MAKE) -ikC $(TKEL_ROOT) clean_all
endif

clean_dmjfs:
	@$(ECHO) cleaning DMJFS ...
	$(MAKE) -ikC $(DMJFS_ROOT) clean_all

clean_tbox:
	@$(ECHO) cleaning TBOX ...
	$(MAKE) -ikC $(TBOX_ROOT) clean_all

clean_trace:
ifeq "$(TRACES)" "1"
	$(MAKE) -ikC $(BUILD_ROOT)/src clean
	$(MAKE) -ikC $(MW_ROOT_DIR)/core/src/sys_mngr clean
endif

clean_comedia:
	@$(ECHO) cleaning comedia ...
	$(MAKE) -ikC $(MW_ROOT_DIR) clean_all

clean_cak_dal:
	@$(ECHO) cleaning comedia ...
	$(MAKE) -ikC $(CAK_DAL_ROOT) clean_all
	
clean_ccas:
	@$(ECHO) cleaning comedia ...
	$(MAKE) -ikC $(CCAS_ROOT) clean_all	

clean_pidb:
	@$(ECHO) cleaning PIDB ...
	$(MAKE) -ikC $(PIDB_ROOT) clean_all

clean_slist:
	@$(ECHO) cleaning SLIST ...
	$(MAKE) -ikC $(SLIST_ROOT) clean_all

clean_ddm:
	@$(ECHO) cleaning DDM ...
	$(MAKE) -ikC $(DDM_CORE_ROOT) clean_all
	$(MAKE) -ikC $(DDM_GLUE_ROOT) clean_all

clean_table:
	@$(ECHO) cleaning TABLE ...
	$(MAKE) -ikC $(TABLE_ROOT) clean_all

clean_subtitle:
	@$(ECHO) cleaning SUBTITLE ...
	$(MAKE) -ikC $(SUBTITLE_ROOT) clean_all
	$(MAKE) -ikC $(SUBTITLE_GLUE_ROOT) clean_all

clean_audio_video:
	@$(ECHO) cleaning AUDIO_VIDEO ...
	$(MAKE) -ikC $(AUDIO_VIDEO_ROOT) clean_all

clean_datetime:
	@$(ECHO) cleaning dateTime ...
	$(MAKE) -ikC $(DATETIME_ROOT) clean_all

clean_xpel:
	$(MAKE) -ikC $(XPEL_ROOT) clean_all

clean_ttxt:
	@$(ECHO) cleaning teletext ...
	$(MAKE) -ikC $(TTXT_ROOT) clean_all
	$(MAKE) -ikC $(TTXT_GLUE_ROOT) clean_all

clean_gfx:
	@$(ECHO) cleaning gfx ...
	$(MAKE) -ikC $(GFX_ROOT) clean_all

clean_text:
	@$(ECHO) cleaning text ...
	$(MAKE) -ikC $(TEXT_ROOT) clean_all
	
clean_font:
	@$(ECHO) cleaning font ...
	$(MAKE) -ikC $(FONT_ROOT) clean_all

clean_png:
	@$(ECHO) cleaning PNG lib ...
	$(MAKE) -ikC $(PNG_ROOT) clean_all

clean_fnt:
	@$(ECHO) cleaning font_engine ...
	$(MAKE) -ikC $(FNT_ROOT) clean_all

clean_trd:
	@$(ECHO) cleaning text_rendering ...
	-$(MAKE) -ikC $(TRD_ROOT) clean_all

clean_eim:
	@$(ECHO) cleaning Package EIM ...
	$(MAKE) -ikC $(PACKAGE_EIM_ROOT) clean_all

clean_bget:
	@$(ECHO) cleaning bget ...
	$(MAKE) -ikC $(BGET_ROOT) clean_all
ifeq "$(PRODUCT_APP)" "CELINE"
# for celine_player_glue
clean_celine_player_glue:
	@$(ECHO) cleaning maestro celine glue ...
	$(MAKE) -ikC $(CELINE_PLAYER_GLUE_ROOT) clean_all
endif
clean_dlmalloc:
	@$(ECHO) cleaning dlmalloc ...
	$(MAKE) -ikC $(DLMALLOC_ROOT) clean_all

clean_player:
	@$(ECHO) cleaning player ...
	$(MAKE) -ikC $(MAESTRO_PLAYER_GLUE_ROOT) clean_all
ifeq "$(PRODUCT_APP)" "MAESTRO"
clean_player_plugin:
	@$(ECHO) cleaning player plugin ...
	$(MAKE) -ikC $(MAESTRO_PLAYER_PLUGIN_ROOT) clean_all
endif
clean_mheg:
	@$(ECHO) cleaning MHEG ...
	$(MAKE) -ikC $(MHEG5_CORE_ROOT) clean_all
	$(MAKE) -ikC $(MHEG5_GLUE_ROOT) clean_all
	$(MAKE) -ikC $(TRD_ROOT) clean_all
	$(MAKE) -ikC $(FNT_ROOT) clean_all

clean_file_system:
	@$(ECHO) cleaning file_system ...
	$(MAKE) -ikC $(FILESYS_ROOT) clean_all

clean_fdmx:
	@$(ECHO) cleaning fdmx ...
	$(MAKE) -ikC $(FDMX_CORE_ROOT) clean_all

clean_trigger:
	@$(ECHO) cleaning trigger ...
	$(MAKE) -ikC $(TRIGGER_ROOT) clean_all

clean_sdec:
	@$(ECHO) cleaning sdec and sdec_glue ...
	$(MAKE) -ikC $(SDEC_ROOT) clean_all
	$(MAKE) -ikC $(SDEC_GLUE_ROOT) clean_all

clean_freetype:
	@$(ECHO) cleaning freetype ...
	$(MAKE) -ikC $(FREETYPE_ROOT) clean_all
    
clean_cdac:
	@$(ECHO) cleaning cdac ...
	$(MAKE) -ikC $(CDAC_ROOT) clean_all
	
clean_media_renderer:
	@$(ECHO) cleaning media_renderer ...
	$(MAKE) -ikC $(MEDIA_RENDERER_ROOT) clean_all

clean_media_source:
	@$(ECHO) cleaning media_source ...
	$(MAKE) -ikC $(MEDIA_SOURCE_ROOT) clean_all

clean_media_browser:
	@$(ECHO) cleaning media_browser ...
	$(MAKE) -ikC $(MEDIA_BROWSER_ROOT) clean_all

clean_media_metadata:
	@$(ECHO) cleaning media_metadata ...
	$(MAKE) -ikC $(MEDIA_METADATA_ROOT) clean_all

ifneq "$(PRODUCT_BOOTLOADER)" "BOOTLOADER_OFF"
clean_bootloader:
	@$(ECHO) cleaning Package Bootloader ...
	$(MAKE) -ikC $(BOOTLOADER_CORE_ROOT) clean_all
	$(MAKE) -ikC $(BOOTLOADER_GLUE_ROOT) clean_all	
ifeq "$(PRODUCT_TRIGGER)" "TRIGGER_SSU_USB"
	$(MAKE) -ikC $(FDMX_CORE_ROOT) clean_all
endif
	$(MAKE) -ikC $(CRYPT_ROOT) clean_all
	$(MAKE) clean_tbox
	$(MAKE) clean_tkel
	$(MAKE) clean_tdal
	$(MAKE) -ikC $(BUILD_ROOT)/src clean_all
endif

###################################################
#	help
###################################################
help:
	@$(ECHO) --------------------------------------------------------------
	@$(ECHO)   Product $(PRODUCT)
	@$(ECHO)   Drivers $(DRIVERS_NAME)
	@$(ECHO)   Environnement $(PRODUCT_ENV)
	@$(ECHO) --------------------------------------------------------------
	@$(ECHO)   Targets: 
	@$(ECHO)   NOTE : default is $(PRODUCT_NAME) application
	@$(ECHO)   - $(PRODUCT_NAME).$(BIN_SUFFIX)	: generate a binary file "$(PRODUCT_NAME).$(BIN_SUFFIX)"
	@$(ECHO)   - $(PRODUCT_NAME).$(BIN_SUFFIX)	: generate a hex file "$(PRODUCT_NAME).$(HEX_SUFFIX)"
	@$(ECHO) --------------------------------------------------------------
	@$(ECHO)   Targets clean: 
	@$(ECHO)   - clean          	    : clean only objects and libraries 	
	@$(ECHO)   - clean_all          	: clean all	
	@$(ECHO)    HAL
	@$(ECHO)   - clean_tdal         	: clean TDAL
	@$(ECHO)   - clean_tkel         	: clean TKEL
	@$(ECHO)   - clean_tbox         	: clean TBOX
	@$(ECHO)   - clean_comedia         	: clean objects files from comedia directory
	@$(ECHO)    packages
	@$(ECHO)   - clean_audio_video      : clean audio_video
	@$(ECHO)   - clean_pidb         	: clean pidb
	@$(ECHO)   - clean_slist         	: clean slist
	@$(ECHO)   - clean_table        	: clean table
ifeq "$(PRODUCT_USE_DDM)" "YES"
	@$(ECHO)   - clean_ddm				: clean ddm
endif	
ifeq "$(PRODUCT_SUBTITLE)" "SUBT_ON"		
	@$(ECHO)   - clean_subtitle        	: clean subtitle
endif	
ifeq "$(PRODUCT_TELETEXT_OSD)" "TTXT_OSD_ON"
	@$(ECHO)   - clean_ttxt	        	: clean teletext
endif
	@$(ECHO)   - clean_eim        		: clean package eim
	@$(ECHO)   - clean_bget        		: clean bget
	@$(ECHO)   - clean_dlmalloc			: clean dlmalloca
	@$(ECHO)   - clean_datetime        	: clean datetime
	@$(ECHO)   - clean_xpel         	: clean xpel
	@$(ECHO)   - clean_mheg	        	: clean mheg
	@$(ECHO)   - clean_gfx 	       		: clean gfx
	@$(ECHO)   - clean_text        		: clean text
	@$(ECHO)   - clean_file_system      : clean file_system
	@$(ECHO)   - clean_fdmx        		: clean fdmx
	@$(ECHO)   - clean_sdec        		: clean sdec
	@$(ECHO)   - clean_freetype        	: clean freetype
	@$(ECHO)   - clean_player     		: clean player	
	@$(ECHO)   - clean_player_plugin	: clean player plugin
	@$(ECHO)   - clean_load_appli   	: clean all the dependencies of the load application
	@$(ECHO)   - clean_cak_dal  	    : clean cak_dal
	@$(ECHO)   - clean_ccas		  	    : clean ccas
	@$(ECHO)   - clean_bootloader  	    : clean all the dependencies of the bootloader	
	@$(ECHO)   - clean_images  	        : clean all build/bin images	
	@$(ECHO) --------------------------------------------------------------
	@$(ECHO)   execution: 
	@$(ECHO)   - help               	: display this help
	@$(ECHO)   - run                	: run $(PRODUCT_NAME) exe file
	@$(ECHO)   - debug              	: run $(PRODUCT_NAME) exe file with graphical debugger
	@$(ECHO)  
	@$(ECHO)   - dalts_run     			: run the CAK DAL test application exe file
	@$(ECHO)   - dalts_debug   			: run the CAK DAL test application exe file with graphical debugger
	@$(ECHO) --------------------------------------------------------------
	@$(ECHO)   Compilation flags:
	@$(ECHO)   - TRACES=1           	: Compil TBOX and print activated traces on DCU
	@$(ECHO)   - TRACES=UART        	: Compil TBOX and print activated traces on UART
	@$(ECHO)   - TRACES=TRACES_FOR_INTEGRATION      : Compil TBOX and print activated traces on DCU for a comedia with traces for integration
	@$(ECHO)   - DEBUG=1				: generates an exe file with debug symbols
	@$(ECHO)   - PROGTYPE=BOOTLOADER    : Generate a bootloader.bin to run with a comedia in flash
	@$(ECHO)   - PROGTYPE=COMEDIA       : Generate a comedia.bin to run with a bootloader in flash
	@$(ECHO) --------------------------------------------------------------

include $(BUILD_MAKE)/makelib.mak

