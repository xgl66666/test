# **********************************************
# Release shared libs path
# **********************************************

ifeq ($(DDI_MAIN), enable)

ifneq ($(MI_BSP), enable)
    include $(PRJ_DIR)/configurations/platform_configuration
    include $(PRJ_DIR)/configurations/toolchain_configuration

    ifeq ($(CHIP), k2)
        include $(PRJ_DIR)/
    else
        include $(PRJ_DIR)/configurations/feature_configuration
    endif

    include ./ddi_main_libs.mk
else
    include $(PRJ_DIR)/configurations/platform_configuration
    include $(PRJ_DIR)/configurations/feature_configuration
    include $(PRJ_DIR)/configure_mi
    include ./ddi_main_libs.mk
endif
else

include $(PRJ_DIR)/config
include ./libs.mk

endif

# =====================================
# file name define
# =====================================
TAR=.tar.gz

FREETYPE=freetype-2.5.3
ifeq ($(OS_TYPE), linux)
    FREETYPE_TAR=$(FREETYPE)$(TAR)
else ifeq ($(OS_TYPE), ecos)
    FREETYPE_TAR=$(FREETYPE)-$(CHIP)$(TAR)
endif

ifeq ($(CPU), mips)
    ZLIB=zlib-1.2.8
else
    ZLIB=zlib-1.2.3
endif

ZLIB_TAR=$(ZLIB)$(TAR)
INIPARSER_TAR=iniparser.tar.gz

# temp dir to untar file
TMP_DIR=./_release
ifeq ($(OS_TYPE), linux)

ifeq ($(CPU), mips)
    TARFILE_DIR=../mw_util_lib/$(OS_TYPE)/$(CPU)/$(TOOLCHAIN_VERSION)/
else
    TARFILE_DIR=../mw_util_lib/$(OS_TYPE)/$(CPU)/
endif

else ifeq ($(OS_TYPE), ecos)
    TARFILE_DIR=../mw_util_lib/$(OS_TYPE)/$(CPU)/$(TOOLCHAIN_VERSION)/
endif

# the support toolchain is mips/aeonR2
CROSS_TOOL_PREFIX = "mips-linux-gnu"

INCLUDE_PATH=./api/mwlibs_include
FREETYPE_STATIC_PATH = ./api/mm/freetype
INIPARSER_STATIC_PATH = ./api/mm/iniparser
CONFIG_PATH = $(BSP_PRJ)/bsp/linux/partition/config




# =====================================
# Rules
# =====================================
.PHONY: all release releaseFREETYPE releaseINIPARSER releaseZLIB

all: msgprint release

msgprint:
	@echo "making!! in cus/Makefile"
	@echo "extra mstar_mw lib: $(EXTRAMWLIBS)"

release: clean makedir $(EXTRAMWLIBS)

releaseFREETYPE:
#	@echo "############################################################"
	@echo "Release freetype 2.5.3....."
#	@echo "############################################################"
#untar file
	@[ -d $(TMP_DIR).$@ ] && rm -rf $(TMP_DIR).$@ ; mkdir $(TMP_DIR).$@
	@cd $(TMP_DIR).$@;tar zxf $(TARFILE_DIR)$(FREETYPE_TAR)
#copy header file
	@[ -d $(INCLUDE_PATH)/freetype ] || mkdir -p $(INCLUDE_PATH)/freetype
	@cp -rf $(TMP_DIR).$@/$(FREETYPE)/include/* $(INCLUDE_PATH)/freetype
ifeq ($(LINK_TYPE), dynamic)
#copy libs
	@find $(TMP_DIR).$@ -name '*.so*' -exec cp -f -d {} $(TMP_MWLIB) \;
else
#copy static lib
ifeq ($(OS_TYPE), linux)
	@find $(TMP_DIR).$@ -name '*.so*' -exec cp -f -d {} $(TMP_MWLIB) \;
else ifeq ($(OS_TYPE), ecos)
	@[ -d $(FREETYPE_STATIC_PATH) ] || rm -rf $(FREETYPE_STATIC_PATH);mkdir -p $(FREETYPE_STATIC_PATH)
	@find $(TMP_DIR).$@ -name '*.a*' -exec cp -f -d {} $(FREETYPE_STATIC_PATH) \;
endif
endif
	@rm -rf $(TMP_DIR).$@

releaseINIPARSER:
	@echo "############################################################"
	@echo   "Release iniparser....."
	@echo "############################################################"

#untar file
	@[ -d $(TMP_DIR).$@ ] && rm -rf $(TMP_DIR).$@ ; mkdir $(TMP_DIR).$@
	@cd $(TMP_DIR).$@;tar zxf $(TARFILE_DIR)$(INIPARSER_TAR)
#copy header file
#	@[ -d $(TMP_MWLIB)/directfb ] ||  mkdir -p $(TMP_MWLIB)/directfb
	@[ -d $(INCLUDE_PATH)/iniparser ] ||  mkdir -p $(INCLUDE_PATH)/iniparser
	@cp -rf $(TMP_DIR).$@/iniparser/inc/* $(INCLUDE_PATH)/iniparser
#copy libs
ifeq ($(OS_TYPE), linux)
	@cp -rf $(TMP_DIR).$@/iniparser/*.so* $(TMP_MWLIB)/
else
	@[ -d $(INIPARSER_STATIC_PATH) ] || rm -rf $(INIPARSER_STATIC_PATH);mkdir -p $(INIPARSER_STATIC_PATH)
	@find $(TMP_DIR).$@ -name '*.a*' -exec cp -f -d {} $(INIPARSER_STATIC_PATH) \;
endif
	@rm -rf $(TMP_DIR).$@


releaseZLIB:
ifeq ($(OS_TYPE), linux)
#	@echo "############################################################"
	@echo "Release zlib....."
#	@echo "############################################################"
#untar file
	@[ -d $(TMP_DIR).$@ ] && rm -rf $(TMP_DIR).$@ ; mkdir $(TMP_DIR).$@
	@cd $(TMP_DIR).$@;tar zxf $(TARFILE_DIR)$(ZLIB_TAR)
#copy header file
	@[ -d $(INCLUDE_PATH)/zlib ] || mkdir -p $(INCLUDE_PATH)/zlib
	@cp -rf $(TMP_DIR).$@/$(ZLIB)/include/* $(INCLUDE_PATH)/zlib
#copy libs
	@find $(TMP_DIR).$@ -name '*.so*' -exec cp -f -d {} $(TMP_MWLIB) \;
	@rm -rf $(TMP_DIR).$@
endif

makedir:
	@echo "Create MISC mwlib_include folder"
	@[ -d $(INCLUDE_PATH) ] || mkdir -p $(INCLUDE_PATH)


clean:
	@echo "MISC: clean up headers & libs"
	@echo "MISC: include path = $(INCLUDE_PATH)"
	@rm -rf $(INCLUDE_PATH)
	@rm -rf $(TMP_DIR).*
