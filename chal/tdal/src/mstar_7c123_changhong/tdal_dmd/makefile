# ****************************************
# TDAL_DMD makefile
# ****************************************

include $(TDAL_MAKE)/environnement.mak
include $(SDK_ROOT)/drivers.mak


# ****************************************
# Define module name
# ****************************************
MODULE  = tdal_dmd


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
    $(INCLUDE_PREFIX)$(TDAL_DMD_INCLUDE) \
    $(INCLUDE_PREFIX)$(TDAL_COMMON_INCLUDE) \
    $(INCLUDE_PREFIX)$(TBOX_INCLUDE) \

# ***************************************************
# List of object files needed to create library
# ***************************************************
ifeq "$(findstring USE_STFRONTEND,$(CFLAGS))" "USE_STFRONTEND"
TDAL_DMD_USE_STFRONTEND = 1
endif

LIBOBJS = 	tdal_dmd.$(OBJ_SUFFIX)

# ****************************************
# List of dependencies
# ****************************************
tdal_dmd.$(OBJ_SUFFIX):             tdal_dmd.h makefile tdal_dmd.c tdal_dmd_priv.h

# ****************************************
# Always include crules.mak
# ****************************************
include $(TDAL_MAKE)/crules.mak


