#----------------------------------------------------------------------
#
#           Root Makefile for comedia product
#
#----------------------------------------------------------------------
###################################################
#	set environnement
###################################################
ifeq "$(ROOT_DIR)" ""
export ROOT_DIR=$(CURDIR)/../
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

# Target: Comedia as an executable
$(PRODUCT_NAME): clean_tkel clean_tbox clean_trace $(PRODUCTLIB_CHAL) $(PRODUCTLIB) $(PRODUCT_NAME).$(BIN_SUFFIX)

$(PRODUCT_NAME).$(EXE_SUFFIX):
	$(ECHO) Create $(PRODUCT_NAME).$(EXE_SUFFIX) ...
	$(LD) \
    $(LIBRARY_PREFIX)$(BUILD_EXPORTS) \
    $(addprefix $(LIBRARY_PREFIX), $(OS_LIBS_PATH)) \
    $(addprefix $(LIBRARY_PREFIX), $(SDK_LIBS_PATH)) \
    $(addprefix $(LIBRARY_PREFIX), $(CAK_LIBS_PATH)) \
    $(addprefix $(LIBRARY_PREFIX), $(EXTERN_LIBS_PATH)) \
	$(BUILD_EXPORTS)/$(PRODUCT_MAIN).$(OBJ_SUFFIX) \
	$(WL_OPTION)--start-group $(addprefix -l, $(call REMOVE_LIB_PREFIX, $(OS_LIBS) $(SDK_LIBS) $(CAK_LIBS) $(EXTERN_LIBS) $(PRODUCTLIB_CHAL) $(PRODUCTLIB))) $(WL_OPTION)--end-group \
	$(WL_OPTION)--gc-sections $(SDK_LD_FLAGS) -g \
    -o  $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_EXPORTS)/$(PRODUCT_NAME).$(EXE_SUFFIX)) \
    $(WL_OPTION)-Map=$(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_EXPORTS)/$(PRODUCT_NAME).map)

$(PRODUCT_NAME).$(BIN_SUFFIX): $(PRODUCT_NAME).$(EXE_SUFFIX)
#   Strip application binary
	$(STRIP) -s $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/bin/$(PRODUCT_NAME).$(EXE_SUFFIX)) -o $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/bin/$(PRODUCT_NAME).$(BIN_SUFFIX))
	$(OBJ_COPY) -O binary $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/bin/$(PRODUCT_NAME).$(BIN_SUFFIX))
#   Preparing SSU image
	$(MAKE) -ikC $(subst $(BAD_SLASH),$(GOOD_SLASH),$(SDK_ROOT)) image
	@$(ECHO) --------------------------------------------------
	@$(ECHO) ------- Files ready in build/bin         ---------
	@$(ECHO) --------------------------------------------------

clean_$(PRODUCT_NAME):
	@$(ECHO) cleaning comedia ...
	$(MAKE) -ikC $(MW_ROOT_DIR) clean_all

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

#flashtool:
#	$(MAKE) -C $(FLASHTOOL_ROOT) all

#clean_flashtool:
#	$(MAKE) -C $(FLASHTOOL_ROOT) clean
	
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
#	HAL validator
###################################################

# Target: CHAL_VALIDATOR as an executable
$(PRODUCT_CHAL_VALIDATOR_NAME): clean_tkel clean_tbox clean_trace $(PRODUCTLIB_CHAL) $(CHALVALIDATORLIB) $(PRODUCT_CHAL_VALIDATOR_NAME).$(BIN_SUFFIX)

$(PRODUCT_CHAL_VALIDATOR_NAME).$(EXE_SUFFIX):
	$(ECHO) Create $(PRODUCT_CHAL_VALIDATOR_NAME).$(EXE_SUFFIX) ...
	$(LD) \
    $(LIBRARY_PREFIX)$(BUILD_EXPORTS) \
    $(addprefix $(LIBRARY_PREFIX), $(OS_LIBS_PATH)) \
    $(addprefix $(LIBRARY_PREFIX), $(SDK_LIBS_PATH)) \
    $(addprefix $(LIBRARY_PREFIX), $(EXTERN_LIBS_PATH)) \
	$(BUILD_EXPORTS)/main_$(PRODUCT_CHAL_VALIDATOR_NAME).$(OBJ_SUFFIX) \
	$(WL_OPTION)--start-group $(addprefix -l, $(call REMOVE_LIB_PREFIX, $(OS_LIBS) $(SDK_LIBS) $(EXTERN_LIBS) $(PRODUCTLIB_CHAL) $(CHALVALIDATORLIB))) $(WL_OPTION)--end-group \
	$(WL_OPTION)--gc-sections $(SDK_LD_FLAGS) -g \
    -o  $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_EXPORTS)/$(PRODUCT_CHAL_VALIDATOR_NAME).$(EXE_SUFFIX)) \
    $(WL_OPTION)-Map=$(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_EXPORTS)/$(PRODUCT_CHAL_VALIDATOR_NAME).map)

$(PRODUCT_CHAL_VALIDATOR_NAME).$(BIN_SUFFIX): $(PRODUCT_CHAL_VALIDATOR_NAME).$(EXE_SUFFIX)
	$(STRIP) -s $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/bin/$(PRODUCT_CHAL_VALIDATOR_NAME).$(EXE_SUFFIX)) -o $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/bin/$(PRODUCT_CHAL_VALIDATOR_NAME).$(BIN_SUFFIX))
	$(OBJ_COPY) -O binary $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/bin/$(PRODUCT_CHAL_VALIDATOR_NAME).$(BIN_SUFFIX))
	$(MAKE) -ikC $(subst $(BAD_SLASH),$(GOOD_SLASH),$(SDK_ROOT)) image IMAGE_BIN=$(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_EXPORTS)/$(PRODUCT_CHAL_VALIDATOR_NAME).$(BIN_SUFFIX)) IMAGE_RES=NONE

$(PRODUCT_CHAL_VALIDATOR_NAME)_run:
	@$(ECHO) running chalvalidator application ...
	$(MAKE) -kC $(BUILD_ROOT) $(PRODUCT_CHAL_VALIDATOR_NAME)_run

$(PRODUCT_CHAL_VALIDATOR_NAME)_debug:
	@$(ECHO) debugging chalvalidator application ...
	$(MAKE) -kC $(BUILD_ROOT) $(PRODUCT_CHAL_VALIDATOR_NAME)_debug

clean_$(PRODUCT_CHAL_VALIDATOR_NAME): clean_chaltests clean_testmanager
	
###################################################
#	Various clean
###################################################

clean: clean_chal clean_middleware clean_chalvalidator clean_configuration
	$(ECHO) cleaning ...
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


###################################################
#	help
###################################################

help: 
	@$(ECHO) --------------------------------------------------------------
	@$(ECHO)   Configuration : $(SETENV)
	@$(ECHO)   Environment   : $(PRODUCT_ENV)
	@$(ECHO) --------------------------------------------------------------
	@$(ECHO)   Targets: 
	@$(ECHO)   NOTE : default target is $(PRODUCT_NAME) application
	@$(ECHO)   - $(PRODUCT_NAME).$(EXE_SUFFIX)	: generate a executable file
	@$(ECHO)   - $(PRODUCT_NAME).$(BIN_SUFFIX)	: generate a binary file
	@$(ECHO) --------------------------------------------------------------
	@$(ECHO)   Targets clean: 
	@$(ECHO)   - clean              	: clean only objects and libraries 	
	@$(ECHO)   - clean_all          	: clean all	
	@$(ECHO)   HAL
	@$(ECHO)   - clean_chal         	: clean complete HAL
	@$(ECHO)   - clean_tdal         	: clean TDAL module
	@$(ECHO)   - clean_tkel         	: clean TKEL module
	@$(ECHO)   - clean_tbox         	: clean TBOX module
	@$(ECHO)   HAL Validator
	@$(ECHO)   - clean_validator    	: clean complete HAL
	@$(ECHO)   Middleware
	@$(ECHO)   - clean_$(PRODUCT_NAME)	: clean objects files from middleware
	@$(ECHO)   - clean_images       	: clean all build/bin images
	@$(ECHO)   - clean_[module]     	: clean all module [module] from
	@$(ECHO)                              core, packages or interface
	@$(ECHO) --------------------------------------------------------------
	@$(ECHO)   Help: 
	@$(ECHO)   - help               	: display help
	@$(ECHO)   - help_all           	: display extended help
	@$(ECHO)   Execution: 
	@$(ECHO)   - run                	: run $(PRODUCT_NAME) exe file
	@$(ECHO)   - debug              	: run $(PRODUCT_NAME) exe file 
	@$(ECHO)                        	  with graphical debugger
	@$(ECHO) --------------------------------------------------------------
	@$(ECHO)   Compilation flags:
	@$(ECHO)   - TRACES=1           	: Compile TBOX to log traces on DCU
	@$(ECHO)   - TRACES=UART        	: Compile TBOX to log traces on UART
	@$(ECHO)   - TRACES=TRACES_FOR_INTEGRATION : Compile TBOX log traces
	@$(ECHO)                        	         on DCU for a comedia with 
	@$(ECHO)                        	         traces for integration
	@$(ECHO)   - DEBUG=1            	: generates an exe file with debug 
	@$(ECHO)                        	  symbols
	@$(ECHO) --------------------------------------------------------------

include $(BUILD_MAKE)/dist.mak
include $(BUILD_MAKE)/makelib.mak

