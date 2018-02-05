# ****************************************
# TDAL_SMC makefile
# ****************************************
include $(TDAL_MAKE)/environnement.mak
include $(SDK_ROOT)/drivers.mak

# ****************************************
# Define module name
# ****************************************
MODULE = tdal_ssu

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
    $(INCLUDE_PREFIX)$(TBOX_INCLUDE) \
    $(INCLUDE_PREFIX)$(TKEL_INCLUDE) \
    $(INCLUDE_PREFIX)$(TDAL_COMMON_INCLUDE) \
    $(INCLUDE_PREFIX)$(TDAL_SRC_ROOT)/tdal_priv_inc

# ***************************************************
# List of object files needed to create library
# ***************************************************
LIBOBJS = tdal_ssu.$(OBJ_SUFFIX)

# ****************************************
# List of dependencies
# ****************************************
tdal_ssu.$(OBJ_SUFFIX): $(TDAL_SMC_INCLUDE)/tdal_ssu.h tdal_ssu.c makefile

# ****************************************
# Always include crules.mak
# ****************************************
include $(TDAL_MAKE)/crules.mak

