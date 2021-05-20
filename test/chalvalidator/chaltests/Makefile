# ****************************************
# CHALTests makefile
# ****************************************
include $(BUILD_MAKE)/environnement.mak
include $(SDK_ROOT)/drivers.mak


# ****************************************
# Define module name
# ****************************************
MODULE  = chaltests

# ****************************************
# This define the default command for gmake
# ****************************************
all: $(LIB_PREFFIX)$(MODULE).$(LIB_SUFFIX)

# ****************************************
# Include paths needed to compile those files
# ****************************************
DIR_INCLUDE = \
	$(INCLUDE_PREFIX). \
    $(INCLUDE_PREFIX)inc \
    $(INCLUDE_PREFIX)$(TEST_MANAGER_INCLUDE) \
    $(INCLUDE_PREFIX)$(TBOX_INCLUDE) \
    $(INCLUDE_PREFIX)$(TKEL_INCLUDE) \
    $(INCLUDE_PREFIX)$(TKEL_ARCH_INCLUDE) \
    $(INCLUDE_PREFIX)$(ECOS_LIB_INCLUDE) \
    $(INCLUDE_PREFIX)$(TDAL_AV_INCLUDE) \
    $(INCLUDE_PREFIX)$(TDAL_GFX_INCLUDE) \
    $(INCLUDE_PREFIX)$(TDAL_TS_INCLUDE) \
    $(INCLUDE_PREFIX)$(TDAL_DMX_INCLUDE) \
    $(INCLUDE_PREFIX)$(TDAL_DMD_INCLUDE) \
    $(INCLUDE_PREFIX)$(TDAL_KBD_INCLUDE) \
    $(INCLUDE_PREFIX)$(TDAL_FLA_INCLUDE) \
    $(INCLUDE_PREFIX)$(TDAL_NVM_INCLUDE) \
    $(INCLUDE_PREFIX)$(TDAL_COMMON_INCLUDE) \
    $(INCLUDE_PREFIX)$(TDAL_AVS_INCLUDE) \
	$(INCLUDE_PREFIX)$(TDAL_DISP_INCLUDE) \
	$(INCLUDE_PREFIX)$(TDAL_OUTPUT_INCLUDE) \
	$(INCLUDE_PREFIX)$(TDAL_HDMI_INCLUDE) \
	$(INCLUDE_PREFIX)$(TDAL_MP_INCLUDE) \
	$(INCLUDE_PREFIX)$(TDAL_COMMON_INCLUDE) \
	$(INCLUDE_PREFIX)$(TBOX_INCLUDE) \
	$(INCLUDE_PREFIX)$(TDAL_ROOT)/tdal_priv_inc

ifeq "$(PRODUCT_USE_TDAL_FS)" "YES"
DIR_INCLUDE += $(INCLUDE_PREFIX)$(TDAL_FS_INCLUDE)
endif

ifeq "$(PRODUCT_USE_HDMI_CEC)" "YES"
DIR_INCLUDE += $(INCLUDE_PREFIX)$(TDAL_HDMI_CEC_INCLUDE)
endif

ifeq "$(PRODUCT_USE_TDAL_MP)" "YES"
DIR_INCLUDE += $(INCLUDE_PREFIX)$(TDAL_MP_INCLUDE)
endif

ifeq "$(PRODUCT_USE_TDAL_PVR)" "YES"
DIR_INCLUDE += $(INCLUDE_PREFIX)$(TDAL_PVR_INCLUDE)
endif

ifeq "$(PRODUCT_USE_CI)" "YES"
DIR_INCLUDE += $(INCLUDE_PREFIX)$(TDAL_CI_INCLUDE)
endif

ifeq "$(PRODUCT_USE_CIPLUS)" "YES"
DIR_INCLUDE += $(INCLUDE_PREFIX)$(TDAL_CI_INCLUDE)
DIR_INCLUDE += $(INCLUDE_PREFIX)$(TDAL_DESC_INCLUDE)
DIR_INCLUDE += $(INCLUDE_PREFIX)$(CIPLUS_EXT_INCLUDE)/../CryptoLib/include \
               $(INCLUDE_PREFIX)$(CIPLUS_TOOL_INCLUDE)
endif

ifeq "$(PRODUCT_USE_CA)" "YES"
DIR_INCLUDE += $(INCLUDE_PREFIX)$(TDAL_DESC_INCLUDE)
endif

# ***************************************************
# List of object files needed to create library
# ***************************************************
LIBSOURCES = $(wildcard src/testroot.c) \
		$(wildcard src/tdaltests/dmdtests/common*.c) \
		$(wildcard src/tdaltests/dmxtests/*.c) \
		$(wildcard src/tdaltests/gfxtests/*.c) \
		$(wildcard src/tdaltests/avtests/*.c) \
		$(wildcard src/tdaltests/flatests/*.c) \
        $(wildcard src/tdaltests/nvmtests/*.c) \
		$(wildcard src/tdaltests/hdmitests/*.c) \
		$(wildcard src/tdaltests/kbdtests/*.c) \
		$(wildcard src/tdaltests/disptests/*.c) \
		$(wildcard src/tdaltests/outputtests/*.c) \
		$(wildcard src/testhelper/common*.c) \
		$(wildcard src/tkeltests/*.c) \

ifeq "$(PRODUCT_HANDLE_ANALOG_SERVICES)" "YES"
LIBSOURCES += $(wildcard src/tdaltests/dmdtests/analog*.c)
LIBSOURCES += $(wildcard src/testhelper/analog*.c)
LIBSOURCES += $(wildcard src/tdaltests/analogtests/*.c)
else
LIBSOURCES += $(wildcard src/tdaltests/dmdtests/digital*.c)
LIBSOURCES += $(wildcard src/testhelper/digital*.c)
endif

ifeq "$(PRODUCT_USE_HDMI_CEC)" "YES"
LIBSOURCES += $(wildcard src/tdaltests/hdmicectests/*.c)
endif

ifeq "$(PRODUCT_USE_TDAL_FS)" "YES"
LIBSOURCES += $(wildcard src/tdaltests/fstests/*.c)
endif

ifeq "$(PRODUCT_USE_TDAL_MP)" "YES"
LIBSOURCES += $(wildcard src/tdaltests/mptests/*.c)
endif

ifeq "$(PRODUCT_USE_TDAL_PVR)" "YES"
LIBSOURCES += $(wildcard src/tdaltests/pvrtests/*.c)
endif

ifeq "$(PRODUCT_USE_CI)" "YES"
LIBSOURCES += $(wildcard src/tdaltests/citests/*.c)
endif

ifeq "$(PRODUCT_USE_CIPLUS)" "YES"
LIBSOURCES += $(wildcard src/tdaltests/ciplustests/*.c) \
              $(wildcard src/tdaltests/desctests/*.c)
endif

ifeq "$(PRODUCT_USE_TDAL_TS)" "USE_TDAL_TS_ON"
LIBSOURCES += $(wildcard src/tdaltests/tstests/*.c)
endif

ifeq "$(PRODUCT_USE_SSU)" "TDAL_SSU"
LIBSOURCES += $(wildcard src/tdaltests/ssutests/*.c)
endif

LIBOBJS = ${LIBSOURCES:%.c=%.$(OBJ_SUFFIX)}

# ****************************************
# List of dependencies
# ****************************************

# ****************************************
# Always include crules.mak
# ****************************************
include $(BUILD_MAKE)/crules.mak

clean_all:
	@ echo Cleaning chaltests lib
	$(MAKE) -ikC src clean
	$(MAKE) -ikC src/tkeltests clean
	$(MAKE) -ikC src/tdaltests/analogtests clean
	$(MAKE) -ikC src/tdaltests/dmdtests clean
	$(MAKE) -ikC src/tdaltests/dmxtests clean
	$(MAKE) -ikC src/tdaltests/gfxtests clean
	$(MAKE) -ikC src/tdaltests/avtests clean
	$(MAKE) -ikC src/tdaltests/flatests clean
	$(MAKE) -ikC src/tdaltests/nvmtests clean
	$(MAKE) -ikC src/tdaltests/hdmitests clean
	$(MAKE) -ikC src/tdaltests/citests clean
	$(MAKE) -ikC src/tdaltests/kbdtests clean
	$(MAKE) -ikC src/tdaltests/disptests clean
	$(MAKE) -ikC src/tdaltests/tstests clean
	$(MAKE) -ikC src/tdaltests/fstests clean
	$(MAKE) -ikC src/tdaltests/mptests clean
ifeq "$(PRODUCT_USE_HDMI_CEC)" "YES"
	$(MAKE) -ikC src/tdaltests/hdmicectests clean
endif
	$(MAKE) -ikC src/tdaltests/outputtests clean
	$(MAKE) -ikC src/testhelper clean
ifeq "$(PRODUCT_USE_CIPLUS)" "YES"
	$(MAKE) -ikC src/tdaltests/ciplustests clean
	$(MAKE) -ikC src/tdaltests/desctests clean
endif
ifeq "$(PRODUCT_USE_TDAL_PVR)" "YES"
	$(MAKE) -ikC src/tdaltests/pvrtests clean
endif
ifeq "$(PRODUCT_USE_SSU)" "TDAL_SSU"
	$(MAKE) -ikC src/tdaltests/ssutests clean
endif

ifeq "$(PRODUCT_NB_TUNERS)" "2"
CFLAGS += -DkTDAL_TEST_COMMON_MAX_CONTEXT=$(PRODUCT_NB_TUNERS)
else
CFLAGS += -DkTDAL_TEST_COMMON_MAX_CONTEXT=1
endif

ifneq "$(PRODUCT_AVS)" "NO_AVS"
ifneq "$(PRODUCT_AVS)" "AVS_TVSET"
CFLAGS += -DAVS_STB
endif
endif
