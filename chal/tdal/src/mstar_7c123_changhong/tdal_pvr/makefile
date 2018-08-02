# ****************************************
# TDAL_MP makefile
# ****************************************

include $(TDAL_MAKE)/environnement.mak
include $(SDK_ROOT)/drivers.mak


# ****************************************
# Define module name
# ****************************************
MODULE  = tdal_pvr


# ****************************************
# This define the default command for gmake
# ****************************************
all: $(LIB_PREFFIX)$(MODULE).$(LIB_SUFFIX)

ifeq "$(PVR_ENCRYPTION)" "YES"
  CFLAGS += -DENCRYPTION_ENABLED
endif

# ****************************************
# Include paths needed to compile those files
# ****************************************
DIR_INCLUDE = \
    $(addprefix $(INCLUDE_PREFIX), $(OS_INCLUDE_PATH)) \
    $(addprefix $(INCLUDE_PREFIX), $(SDK_INCLUDE_PATH)) \
	$(INCLUDE_PREFIX)$(TDAL_SRC_ROOT)/tdal_priv_inc \
	$(INCLUDE_PREFIX)$(TDAL_PVR_INCLUDE) \
	$(INCLUDE_PREFIX)$(TDAL_COMMON_INCLUDE) \
	$(INCLUDE_PREFIX)$(TBOX_INCLUDE) \

# ***************************************************
# List of object files needed to create library
# ***************************************************
LIBOBJS = 	tdal_pvr.$(OBJ_SUFFIX) \
			tdal_pvr_mem.$(OBJ_SUFFIX)

# ****************************************
# List of dependencies
# ****************************************
tdal_pvr.$(OBJ_SUFFIX): $(TDAL_PVR_INCLUDE)/tdal_pvr.h tdal_pvr_p.h makefile
tdal_pvr_mem.$(OBJ_SUFFIX): $(TDAL_PVR_INCLUDE)/tdal_pvr.h tdal_pvr_p.h makefile

# ****************************************
# Always include crules.mak
# ****************************************
include $(TDAL_MAKE)/crules.mak
