DIST_NAME?=dist
DIST_LIB_NAME?=libraries
DIST_BIN_NAME?=binaries
DIST_RES_NAME?=resources
DIST_SDK_NAME?=sdk
DIST_DOC_NAME?=doc

DIST_RELEASE?=release
DIST_DEBUG?=debug

###################################################
#	Generating distrubution
###################################################

dist: dist_clean dist_prep clean_sdk sdk dist_res dist_release dist_debug

dist_clean:
	$(RMDIR) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(SETENV))

dist_prep:
	$(MKDIR) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(SETENV))
	$(MKDIR) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(SETENV)/$(DIST_DOC_NAME))
	$(MKDIR) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(SETENV)/$(DIST_LIB_NAME))
	$(MKDIR) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(SETENV)/$(DIST_LIB_NAME)/release)
	$(MKDIR) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(SETENV)/$(DIST_LIB_NAME)/debug)
	$(MKDIR) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(SETENV)/$(DIST_BIN_NAME))
	$(MKDIR) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(SETENV)/$(DIST_BIN_NAME)/release)
	$(MKDIR) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(SETENV)/$(DIST_BIN_NAME)/debug)
	$(MKDIR) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(SETENV)/$(DEST_RES_NAME)/$(DIST_RES_NAME)/)

dist_sdk_clean:
	$(RMDIR) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(DIST_SDK_NAME))

dist_sdk_prep:
	-$(MKDIR) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(DIST_SDK_NAME))
	-$(MKDIR) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(DIST_SDK_NAME)/build)
	-$(MKDIR) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(DIST_SDK_NAME)/build/bin)
	-$(MKDIR) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(DIST_SDK_NAME)/build/bin/resources)
	-$(MKDIR) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(DIST_SDK_NAME)/build/set)
	-$(MKDIR) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(DIST_SDK_NAME)/build/inc)
	-$(MKDIR) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(DIST_SDK_NAME)/build/src)
	-$(MKDIR) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(DIST_SDK_NAME)/build/make)
	-$(MKDIR) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(DIST_SDK_NAME)/build/dist)
	-$(MKDIR) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(DIST_SDK_NAME)/test)
	-$(MKDIR) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(DIST_SDK_NAME)/test/chalvalidator)
	-$(MKDIR) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(DIST_SDK_NAME)/sdk)
	-$(MKDIR) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(DIST_SDK_NAME)/sdk/$(notdir $(SDK_ROOT)))
	-$(MKDIR) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(DIST_SDK_NAME)/chal)
	-$(MKDIR) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(DIST_SDK_NAME)/chal/tdal)
	-$(MKDIR) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(DIST_SDK_NAME)/chal/tdal/inc)
	-$(MKDIR) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(DIST_SDK_NAME)/chal/tdal/src)
	-$(MKDIR) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(DIST_SDK_NAME)/chal/tdal/src/$(notdir $(TDAL_SRC_ROOT)))
	-$(MKDIR) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(DIST_SDK_NAME)/chal/tdal/lib)
	-$(MKDIR) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(DIST_SDK_NAME)/chal/tdal/lib/$(notdir $(TDAL_SRC_ROOT)))
	-$(MKDIR) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(DIST_SDK_NAME)/chal/tdal/doc)
	-$(MKDIR) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(DIST_SDK_NAME)/chal/tbox)
	-$(MKDIR) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(DIST_SDK_NAME)/chal/tbox/inc)
	-$(MKDIR) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(DIST_SDK_NAME)/chal/tbox/src)
	-$(MKDIR) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(DIST_SDK_NAME)/chal/tbox/src/$(notdir $(TBOX_SRC_ROOT)))
	-$(MKDIR) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(DIST_SDK_NAME)/chal/tbox/lib)
	-$(MKDIR) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(DIST_SDK_NAME)/chal/tbox/lib/$(notdir $(TBOX_SRC_ROOT)))
	-$(MKDIR) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(DIST_SDK_NAME)/chal/tbox/doc)
	-$(MKDIR) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(DIST_SDK_NAME)/chal/tkel)
	-$(MKDIR) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(DIST_SDK_NAME)/chal/tkel/inc)
	-$(MKDIR) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(DIST_SDK_NAME)/chal/tkel/src)
	-$(MKDIR) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(DIST_SDK_NAME)/chal/tkel/src/$(notdir $(TKEL_SRC_ROOT)))
	-$(MKDIR) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(DIST_SDK_NAME)/chal/tkel/lib)
	-$(MKDIR) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(DIST_SDK_NAME)/chal/tkel/lib/$(notdir $(TKEL_SRC_ROOT)))
	-$(MKDIR) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(DIST_SDK_NAME)/chal/tkel/doc)
	-$(MKDIR) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(DIST_SDK_NAME)/external_libs)
	-$(if $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(SETENV)), $(MKDIR) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(DIST_SDK_NAME)/external_libs/CAK_V7_Merlin))
ifneq "$(wildcard $(BUILD_ROOT)/$(DIST_NAME)/$(SETENV))" "" 
	-$(MKDIR) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(DIST_SDK_NAME)/build/dist/$(SETENV))
endif
    
dist_release: dist_res
	$(MAKE) -kC $(BUILD_ROOT) clean_all
	$(MAKE) -C $(BUILD_ROOT) $(PRODUCT_NAME)
	$(CP) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_EXPORTS)/main.o) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(SETENV)/$(DIST_LIB_NAME)/release)
	$(CP) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_EXPORTS)/libmiddleware.a) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(SETENV)/$(DIST_LIB_NAME)/release)
	$(CP) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_EXPORTS)/comedia.map) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(SETENV)/$(DIST_DOC_NAME)/release.map)
	$(CP) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/src/flash_array.h) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(SETENV)/$(DIST_DOC_NAME)/flash_map.h)
ifneq (,$(filter $(PRODUCT_OS),windows linux))
	$(CP) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_EXPORTS)/comedia.$(EXE_SUFFIX)) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(SETENV)/$(DIST_BIN_NAME)/release/application.$(EXE_SUFFIX))
ifneq "$(wildcard $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_EXPORTS)/*.$(SHAREDLIB_SUFFIX)))" ""
	$(CP) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_EXPORTS)/*.$(SHAREDLIB_SUFFIX)) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(SETENV)/$(DIST_BIN_NAME)/release/)
endif
else
	-$(CP) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_EXPORTS)/comedia_lzma_ota.bin) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(SETENV)/$(DIST_BIN_NAME)/release/application.$(BIN_SUFFIX))
endif

dist_debug: dist_res
	$(MAKE) -kC $(BUILD_ROOT) clean_all
	$(MAKE) -C $(BUILD_ROOT) $(PRODUCT_NAME) TRACES=UART
	$(CP) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_EXPORTS)/main.o) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(SETENV)/$(DIST_LIB_NAME)/debug)
	$(CP) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_EXPORTS)/libmiddleware.a) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(SETENV)/$(DIST_LIB_NAME)/debug)
	$(CP) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_EXPORTS)/comedia.map) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(SETENV)/$(DIST_DOC_NAME)/debug.map)
	$(CP) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/src/flash_array.h) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(SETENV)/$(DIST_DOC_NAME)/flash_map.h)
ifneq (,$(filter $(PRODUCT_OS),windows linux))
	$(CP) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_EXPORTS)/comedia.$(EXE_SUFFIX)) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(SETENV)/$(DIST_BIN_NAME)/debug/application.$(EXE_SUFFIX))
ifneq "$(wildcard $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_EXPORTS)/*.$(SHAREDLIB_SUFFIX)))" ""
	$(CP) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_EXPORTS)/*.$(SHAREDLIB_SUFFIX)) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(SETENV)/$(DIST_BIN_NAME)/debug/)
endif
else
	-$(CP) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_EXPORTS)/comedia_lzma_ota.bin) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(SETENV)/$(DIST_BIN_NAME)/debug/application.$(BIN_SUFFIX))
endif

dist_sdk: dist_sdk_prep
ifeq ($(PRODUCT_ENV),windows)
	$(CP) /S $(subst $(BAD_SLASH),$(GOOD_SLASH),$(TDAL_INCLUDE)) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(DIST_SDK_NAME)/chal/tdal/inc)
	$(CP) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(TDAL_ROOT)/makefile) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(DIST_SDK_NAME)/chal/tdal/)
	$(CP) /S $(subst $(BAD_SLASH),$(GOOD_SLASH),$(TDAL_ROOT)/doc) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(DIST_SDK_NAME)/chal/tdal/doc/)
	$(if  $(TDAL_SRC_ROOT), $(CP) /S $(subst $(BAD_SLASH),$(GOOD_SLASH),$(TDAL_SRC_ROOT)) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(DIST_SDK_NAME)/chal/tdal/src/$(notdir $(TDAL_SRC_ROOT))/))
	$(if  $(TDAL_LIB_ROOT), $(CP) /S $(subst $(BAD_SLASH),$(GOOD_SLASH),$(TDAL_LIB_ROOT)) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(DIST_SDK_NAME)/chal/tdal/lib/$(notdir $(TDAL_SRC_ROOT))/))
	$(CP) /S $(subst $(BAD_SLASH),$(GOOD_SLASH),$(TBOX_INCLUDE)) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(DIST_SDK_NAME)/chal/tbox/inc/)
	$(CP) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(TBOX_ROOT)/makefile) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(DIST_SDK_NAME)/chal/tbox/)
	$(CP) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(TBOX_ROOT)/src/tbox.c) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(DIST_SDK_NAME)/chal/tbox/src/)
	$(CP) /S $(subst $(BAD_SLASH),$(GOOD_SLASH),$(TBOX_ROOT)/doc) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(DIST_SDK_NAME)/chal/tbox/doc/)
	$(if  $(TBOX_SRC_ROOT), $(CP) /S $(subst $(BAD_SLASH),$(GOOD_SLASH),$(TBOX_SRC_ROOT)) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(DIST_SDK_NAME)/chal/tbox/src/$(notdir $(TBOX_SRC_ROOT))/))
	$(if  $(TBOX_LIB_ROOT), $(CP) /S $(subst $(BAD_SLASH),$(GOOD_SLASH),$(TBOX_LIB_ROOT)) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(DIST_SDK_NAME)/chal/tbox/lib/$(notdir $(TBOX_SRC_ROOT))/))
	$(CP) /S $(subst $(BAD_SLASH),$(GOOD_SLASH),$(TKEL_INCLUDE)) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(DIST_SDK_NAME)/chal/tkel/inc/)
	$(CP) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(TKEL_ROOT)/makefile) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(DIST_SDK_NAME)/chal/tkel/)
	$(CP) /S $(subst $(BAD_SLASH),$(GOOD_SLASH),$(TKEL_ROOT)/doc) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(DIST_SDK_NAME)/chal/tkel/doc/)
	$(if  $(TKEL_SRC_ROOT), $(CP) /S $(subst $(BAD_SLASH),$(GOOD_SLASH),$(TKEL_SRC_ROOT)) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(DIST_SDK_NAME)/chal/tkel/src/$(notdir $(TKEL_SRC_ROOT))/))
	$(if  $(TKEL_LIB_ROOT), $(CP) /S $(subst $(BAD_SLASH),$(GOOD_SLASH),$(TKEL_LIB_ROOT)) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(DIST_SDK_NAME)/chal/tkel/lib/$(notdir $(TKEL_SRC_ROOT))/))
	$(CP) /S $(subst $(BAD_SLASH),$(GOOD_SLASH),$(SDK_ROOT)) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(DIST_SDK_NAME)/sdk/$(notdir $(SDK_ROOT))/)
	$(CP) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/set/$(SETENV).mak) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(DIST_SDK_NAME)/build/set/)
	$(ECHO) export PRODUCT_PREBUILT_APPLICATION=YES>> $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(DIST_SDK_NAME)/build/set/$(SETENV).mak)
	$(CP) /S $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/make) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(DIST_SDK_NAME)/build/make/)
	$(CP) /S $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/inc) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(DIST_SDK_NAME)/build/inc/)
	$(CP) /S $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/src) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(DIST_SDK_NAME)/build/src/)
	$(CP) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/makefile) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(DIST_SDK_NAME)/build/)
	$(CP) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/../makefile) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(DIST_SDK_NAME)/)
	$(CP) /S $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/../test/chalvalidator) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(DIST_SDK_NAME)/test/chalvalidator/)
	$(if  $(CAK_LIBDIR), $(CP) /S $(subst $(BAD_SLASH),$(GOOD_SLASH),$(CAK_ROOT)) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(DIST_SDK_NAME)/external_libs/CAK_V7_Merlin/$(notdir $(CAK_ROOT))/))
ifneq "$(wildcard $(BUILD_ROOT)/$(DIST_NAME)/$(SETENV))" "" 
	$(CP) /S $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(SETENV) $(BUILD_ROOT)/$(DIST_NAME)/$(DIST_SDK_NAME)/build/dist/$(SETENV)/)
endif
endif
ifeq ($(PRODUCT_ENV),unix)
	$(CP) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(TDAL_INCLUDE)) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(DIST_SDK_NAME)/chal/tdal)
	$(CP) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(TDAL_ROOT)/makefile) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(DIST_SDK_NAME)/chal/tdal/)
	$(CP) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(TDAL_ROOT)/doc) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(DIST_SDK_NAME)/chal/tdal/)
	$(if  $(TDAL_SRC_ROOT), $(CP) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(TDAL_SRC_ROOT)) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(DIST_SDK_NAME)/chal/tdal/src/))
	$(if  $(TDAL_LIB_ROOT), $(CP) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(TDAL_LIB_ROOT)) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(DIST_SDK_NAME)/chal/tdal/lib/))
	$(CP) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(TBOX_INCLUDE)) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(DIST_SDK_NAME)/chal/tbox)
	$(CP) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(TBOX_ROOT)/makefile) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(DIST_SDK_NAME)/chal/tbox/)
	$(CP) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(TBOX_ROOT)/src/tbox.c) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(DIST_SDK_NAME)/chal/tbox/src/)
	$(CP) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(TBOX_ROOT)/doc) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(DIST_SDK_NAME)/chal/tbox/)
	$(if  $(TBOX_SRC_ROOT), $(CP) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(TBOX_SRC_ROOT)) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(DIST_SDK_NAME)/chal/tbox/src/))
	$(if  $(TBOX_LIB_ROOT), $(CP) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(TBOX_LIB_ROOT)) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(DIST_SDK_NAME)/chal/tbox/lib/))
	$(CP) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(TKEL_INCLUDE)) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(DIST_SDK_NAME)/chal/tkel)
	$(CP) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(TKEL_ROOT)/makefile) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(DIST_SDK_NAME)/chal/tkel/)
	$(CP) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(TKEL_ROOT)/doc) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(DIST_SDK_NAME)/chal/tkel/)
	$(if  $(TKEL_SRC_ROOT), $(CP) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(TKEL_SRC_ROOT)) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(DIST_SDK_NAME)/chal/tkel/src/))
	$(if  $(TKEL_LIB_ROOT), $(CP) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(TKEL_LIB_ROOT)) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(DIST_SDK_NAME)/chal/tkel/lib/))
	$(CP) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(SDK_ROOT)) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(DIST_SDK_NAME)/sdk)
	$(CP) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/set/$(SETENV).mak) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(DIST_SDK_NAME)/build/set/)
	$(ECHO) export PRODUCT_PREBUILT_APPLICATION=YES>> $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(DIST_SDK_NAME)/build/set/$(SETENV).mak)
	$(CP) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/make) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(DIST_SDK_NAME)/build/)
	$(CP) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/inc) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(DIST_SDK_NAME)/build/)
	$(CP) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/src) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(DIST_SDK_NAME)/build/)
	$(CP) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/makefile) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(DIST_SDK_NAME)/build/)
	$(CP) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/../makefile) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(DIST_SDK_NAME)/)
	$(CP) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/../test/chalvalidator) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(DIST_SDK_NAME)/test)
	$(if  $(CAK_LIBDIR), $(CP) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(CAK_ROOT)) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(DIST_SDK_NAME)/external_libs/CAK_V7_Merlin))
ifneq "$(wildcard $(BUILD_ROOT)/$(DIST_NAME)/$(SETENV))" "" 
	$(CP) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(SETENV) $(BUILD_ROOT)/$(DIST_NAME)/$(DIST_SDK_NAME)/build/dist)
endif
endif

dist_res: flashtool resources
	$(CP) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_EXPORTS)/resources/resources.lzma) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/$(DIST_NAME)/$(SETENV)/$(DEST_RES_NAME)/$(DIST_RES_NAME)/)
