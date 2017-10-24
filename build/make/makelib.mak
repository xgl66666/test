################################################################################
#	Hardware abstraction layer - HAL
################################################################################

$(LIB_PREFFIX)tbox$(PRODUCT_HAL_SUFFIX).$(LIB_SUFFIX):
	@$(ECHO) compiling TBOX ...
	$(MAKE) -C $(TBOX_ROOT)

clean_tbox:
	@$(ECHO) cleaning TBOX ...
	$(MAKE) -ikC $(TBOX_ROOT) clean_all

$(LIB_PREFFIX)tkel$(PRODUCT_HAL_SUFFIX).$(LIB_SUFFIX):
	@$(ECHO) compiling TKEL ...
	$(MAKE) -C $(TKEL_ROOT)

clean_tkel:
ifneq "$(KEEP_TKEL)" "1"
	@$(ECHO) cleaning TKEL ...
	$(MAKE) -ikC $(TKEL_ROOT) clean_all
endif
    
$(LIB_PREFFIX)tdal$(PRODUCT_HAL_SUFFIX).$(LIB_SUFFIX):
	@$(ECHO) compiling TDAL ...
	$(MAKE) -C $(TDAL_ROOT)

clean_tdal:
ifneq "$(KEEP_TDAL)" "1"
	@$(ECHO) cleaning TDAL ...
	$(MAKE) -ikC $(TDAL_ROOT) clean_all
endif	

$(LIB_PREFFIX)tdalbootloader.$(LIB_SUFFIX):
	$(MAKE) clean_tdal
	$(RM) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_EXPORTS)/$(LIB_PREFFIX)tdal_*.$(LIB_SUFFIX))
	@$(ECHO) compiling TDAL
	$(MAKE) -C $(TDAL_ROOT) TDAL_BOOTLOADER=1
	$(CP) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_EXPORTS)/$(LIB_PREFFIX)tdal.$(LIB_SUFFIX)) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_EXPORTS)/$@)
	$(MAKE) clean_tdal
	$(RM) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_EXPORTS)/$(LIB_PREFFIX)tdal_*.$(LIB_SUFFIX))	
	$(RM) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_EXPORTS)/$(LIB_PREFFIX)tdal.$(LIB_SUFFIX))
    
chal: $(LIB_PREFFIX)tbox$(PRODUCT_HAL_SUFFIX).$(LIB_SUFFIX) \
      $(LIB_PREFFIX)tkel$(PRODUCT_HAL_SUFFIX).$(LIB_SUFFIX) \
      $(LIB_PREFFIX)tdal$(PRODUCT_HAL_SUFFIX).$(LIB_SUFFIX)

clean_chal: clean_tdal clean_tbox clean_tkel

################################################################################
#	HAL Validator
################################################################################

$(LIB_PREFFIX)testmanager.$(LIB_SUFFIX):
	@$(ECHO) compiling testmanager ...
	$(MAKE) -C $(TEST_MANAGER_ROOT)

clean_testmanager:
	@$(ECHO) cleaning testmanager ...
	$(MAKE) -ikC $(TEST_MANAGER_ROOT) clean_all
    
$(LIB_PREFFIX)chaltests.$(LIB_SUFFIX):
	@$(ECHO) compiling chaltests ...
	$(MAKE) -C $(CHAL_TESTS_ROOT)

clean_chaltests:
	@$(ECHO) cleaning chaltests ...
	$(MAKE) -ikC $(CHAL_TESTS_ROOT) clean_all
	$(MAKE) -ikC $(TEST_MANAGER_ROOT) clean_all

################################################################################
#	Middleware
################################################################################

ifeq "$(PRODUCT_PREBUILT_APPLICATION)" "YES"
$(LIB_PREFFIX)middleware.$(LIB_SUFFIX):
	@$(ECHO) copying middleware libraries...
ifeq ("$(DEBUG)", "1")
	$(CP) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(SETENV)/$(DIST_LIB_NAME)/debug//*.* $(BUILD_EXPORTS)/)
else
	$(CP) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(SETENV)/$(DIST_LIB_NAME)/release//*.* $(BUILD_EXPORTS)/)
endif
	$(CP) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(SETENV)/$(DIST_RES_NAME)/resources.lzma $(BUILD_EXPORTS)/$(DIST_RES_NAME)/)
else 
$(LIB_PREFFIX)middleware.$(LIB_SUFFIX): $(LIB_PREFFIX)configuration.$(LIB_SUFFIX)
	@$(ECHO) compiling middleware ...
	$(MAKE) -C $(MW_ROOT_DIR)
endif

clean_middleware:
	@$(ECHO) cleaning middleware ...
ifneq "$(PRODUCT_PREBUILT_APPLICATION)" "YES"
	$(MAKE) -C $(MW_ROOT_DIR) clean_all
endif
    
$(LIB_PREFFIX)configuration.$(LIB_SUFFIX): $(LIB_PREFFIX)$(PRODUCT_MAIN).$(LIB_SUFFIX)
	$(ECHO) compiling configuration ...
	$(MAKE) -C $(BUILD_ROOT)/src $(LIB_PREFFIX)configuration.$(LIB_SUFFIX) MAIN=1

clean_configuration:
	$(ECHO) cleaning configuration ...
	$(MAKE) -C $(BUILD_ROOT)/src clean_all
	
$(LIB_PREFFIX)$(PRODUCT_MAIN).$(LIB_SUFFIX):
	$(ECHO) compiling Main ...
	$(MAKE) -C $(BUILD_ROOT)/src $(PRODUCT_MAIN).$(OBJ_SUFFIX) MAIN=1
	$(CP) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/src/$(PRODUCT_MAIN).$(OBJ_SUFFIX) $(BUILD_EXPORTS)/)

$(LIB_PREFFIX)$(PRODUCT_MAIN_CHALVALIDATOR).$(LIB_SUFFIX):
	$(ECHO) compiling chalvalidator main...
	$(MAKE) -C $(BUILD_ROOT)/src $(LIB_PREFFIX)$(PRODUCT_MAIN_CHALVALIDATOR).$(LIB_SUFFIX) MAIN_CHALVALIDATOR=1
	$(CP) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/src/$(PRODUCT_MAIN_CHALVALIDATOR).$(OBJ_SUFFIX) $(BUILD_EXPORTS)/)

#-------------------------------------------------------------------------------
#   Middleware Packages modules
#-------------------------------------------------------------------------------  

# Build targets
$(notdir $(patsubst %/,%,$(dir $(wildcard $(MW_PACKAGES_ROOT)/*/)))):
	@$(ECHO) Building only $@...
	$(MAKE) -C $(MW_PACKAGES_ROOT)/$@ all

# Clean targets
$(addprefix clean_,$(notdir $(patsubst %/,%,$(dir $(wildcard $(MW_PACKAGES_ROOT)/*/))))):
	@$(ECHO) cleaning $(subst clean_,,$@)...
	$(MAKE) -ikC $(MW_PACKAGES_ROOT)/$(subst clean_,,$@) clean_all

#-------------------------------------------------------------------------------
#   Middleware Core modules
#-------------------------------------------------------------------------------

# Build targets
$(notdir $(patsubst %/,%,$(dir $(wildcard $(MW_CORE_ROOT)/src/*/)))):
	@$(ECHO) Building only $@...
	$(MAKE) -C $(MW_CORE_ROOT)/src/$@ all

# Clean targets
$(addprefix clean_,$(notdir $(patsubst %/,%,$(dir $(wildcard $(MW_CORE_ROOT)/src/*/))))):
	@$(ECHO) cleaning $@...
	$(MAKE) -ikC $(MW_CORE_ROOT)/src/$@ clean_all

#-------------------------------------------------------------------------------
#   Middleware Interface modules
#-------------------------------------------------------------------------------

# Build targets
$(notdir $(patsubst %/,%,$(dir $(wildcard $(MW_INTERFACE_ROOT)/src/*/)))):
	@$(ECHO) Building only $@...
	$(MAKE) -C $(MW_INTERFACE_ROOT)/src/$@ all

# Clean targets
$(addprefix clean_,$(notdir $(patsubst %/,%,$(dir $(wildcard $(MW_INTERFACE_ROOT)/src/*/))))):
	@$(ECHO) cleaning $@...
	$(MAKE) -ikC $(MW_INTERFACE_ROOT)/src/$@ clean_all

#-------------------------------------------------------------------------------
#   Middleware specific targets
#-------------------------------------------------------------------------------
    
clean_trace:
ifeq "$(TRACES)" "1"
	$(MAKE) -ikC $(BUILD_ROOT)/src clean
	$(MAKE) -ikC $(MW_ROOT_DIR)/core/src/sys_mngr clean
endif

clean_ddm_all:
	@$(ECHO) cleaning DDM ...
	$(MAKE) -ikC $(DDM_CORE_ROOT) clean_all
	$(MAKE) -ikC $(DDM_GLUE_ROOT) clean_all

clean_subtitle_all:
	@$(ECHO) cleaning SUBTITLE ...
	$(MAKE) -ikC $(SUBTITLE_ROOT) clean_all
	$(MAKE) -ikC $(SUBTITLE_GLUE_ROOT) clean_all

clean_ttxt_all:
	@$(ECHO) cleaning teletext ...
	$(MAKE) -ikC $(TTXT_ROOT) clean_all
	$(MAKE) -ikC $(TTXT_GLUE_ROOT) clean_all

clean_mheg_all:
	@$(ECHO) cleaning MHEG ...
	$(MAKE) -ikC $(MHEG5_CORE_ROOT) clean_all
	$(MAKE) -ikC $(MHEG5_GLUE_ROOT) clean_all
	$(MAKE) -ikC $(TRD_ROOT) clean_all
	$(MAKE) -ikC $(FNT_ROOT) clean_all

clean_fnt:
	@$(ECHO) cleaning font_engine ...
	$(MAKE) -ikC $(FNT_ROOT) clean_all

clean_trd:
	@$(ECHO) cleaning text_rendering ...
	-$(MAKE) -ikC $(TRD_ROOT) clean_all

clean_eim:
	@$(ECHO) cleaning Package EIM ...
	$(MAKE) -ikC $(PACKAGE_EIM_ROOT) clean_all

clean_dlmalloc:
	@$(ECHO) cleaning dlmalloc ...
	$(MAKE) -ikC $(DLMALLOC_ROOT) clean_all

ifeq "$(PRODUCT_APP)" "MAESTRO"
player:
	@$(ECHO) compiling Maestro player ...
	$(MAKE) -C $(MAESTRO_PLAYER_ROOT)/src

clean_player:
	@$(ECHO) cleaning Maestro player ...
	$(MAKE) -ikC $(MAESTRO_PLAYER_ROOT)/src clean_all

player_glue:
	@$(ECHO) compiling Maestro player glue ...
	$(MAKE) -C $(MAESTRO_PLAYER_GLUE_ROOT)

clean_player_glue:
	@$(ECHO) cleaning Maestro player glue ...
	$(MAKE) -ikC $(MAESTRO_PLAYER_GLUE_ROOT) clean_all

player_plugin:
	@$(ECHO) compiling Maestro player plugins ...
	$(MAKE) -C $(MAESTRO_PLAYER_PLUGIN_ROOT)

clean_player_plugin:
	@$(ECHO) cleaning Maestro player plugins ...
	$(MAKE) -ikC $(MAESTRO_PLAYER_PLUGIN_ROOT)/ clean_all
endif

ifeq "$(PRODUCT_APP)" "CELINE"
player:
	@$(ECHO) compiling Celine player ...
	$(MAKE) -C $(CELINE_PLAYER_ROOT)/src
  
clean_player:
	@$(ECHO) cleaning Celine player ...
	$(MAKE) -ikC $(CELINE_PLAYER_ROOT)/src clean_all

player_glue: 
	@$(ECHO) compiling Celine player glue ...
	$(MAKE) -C $(CELINE_PLAYER_GLUE_ROOT)

clean_player_glue: 
	@$(ECHO) cleaning Celine player glue ...
	$(MAKE) -ikC $(CELINE_PLAYER_GLUE_ROOT) clean_all

clean_player_plugin: 
	@$(ECHO) cleaning Celine player plugins ...
endif

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
#	Extern libraries
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

expat:
	@$(ECHO) building Expat...
	$(MAKE) -ikC $(EXPAT_ROOT)/src

clean_expat:
	@$(ECHO) cleaning Expat...
	$(MAKE) -ikC $(EXPAT_ROOT)/src clean

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
