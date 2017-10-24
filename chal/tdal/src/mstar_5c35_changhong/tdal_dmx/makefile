# ****************************************
# TDAL_DMX makefile
# ****************************************

include $(TDAL_MAKE)/environnement.mak
include $(SDK_ROOT)/drivers.mak


# ****************************************
# Define module name
# ****************************************
MODULE  = tdal_dmx


# ****************************************
# This define the default command for gmake
# ****************************************
all: $(LIB_PREFFIX)$(MODULE).$(LIB_SUFFIX)

# ****************************************
# Include paths needed to compile those files
# ****************************************
DIR_INCLUDE = \
    $(addprefix $(INCLUDE_PREFIX), $(OS_INCLUDE_PATH)) \
    $(addprefix $(INCLUDE_PREFIX), $(SDK_INCLUDE_PATH)) \
	$(INCLUDE_PREFIX)$(TDAL_SRC_ROOT)/tdal_priv_inc \
    $(INCLUDE_PREFIX)$(TDAL_DMX_INCLUDE) \
    $(INCLUDE_PREFIX)$(TDAL_COMMON_INCLUDE) \
    $(INCLUDE_PREFIX)$(TBOX_INCLUDE) \

ifeq "$(PRODUCT_USE_CA)" "YES"
DIR_INCLUDE += $(INCLUDE_PREFIX)$(TDAL_DESC_INCLUDE)
endif


# ***************************************************
# List of object files needed to create library
# ***************************************************
LIBOBJS = 	tdal_dmx.$(OBJ_SUFFIX) \
			tdal_dmx_channel.$(OBJ_SUFFIX) \
			tdal_dmx_filter.$(OBJ_SUFFIX)


# ****************************************
# List of dependencies
# ****************************************
tdal_dmx.$(OBJ_SUFFIX): $(TDAL_DMX_INCLUDE)/tdal_dmx.h tdal_dmx_priv.h tdal_dmx.c makefile
tdal_dmx_channel.$(OBJ_SUFFIX): $(TDAL_DMX_INCLUDE)/tdal_dmx.h tdal_dmx_priv.h tdal_dmx_channel.c makefile
tdal_dmx_filter.$(OBJ_SUFFIX): $(TDAL_DMX_INCLUDE)/tdal_dmx.h tdal_dmx_priv.h tdal_dmx_filter.c makefile

# ****************************************
# Always include crules.mak
# ****************************************
include $(TDAL_MAKE)/crules.mak

