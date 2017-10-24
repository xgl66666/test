# ****************************************
# TDAL_COMMON makefile
# ****************************************

include $(TDAL_MAKE)/environnement.mak
include $(SDK_ROOT)/drivers.mak

# ****************************************
# Define module name
# ****************************************
MODULE  = tdal_common

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
    $(INCLUDE_PREFIX)$(TDAL_COMMON_INCLUDE) \
    $(INCLUDE_PREFIX)$(TBOX_INCLUDE)

ifeq "$(USE_TDAL_DIAG)" "ON"
  DIR_INCLUDE += $(INCLUDE_PREFIX)$(TDAL_DIAG_INCLUDE)
endif

# ***************************************************
# List of object files needed to create library
# ***************************************************
LIBOBJS = 	tdal_common.$(OBJ_SUFFIX) \
            tdal_common_priv.$(OBJ_SUFFIX)
            
# ****************************************
# List of dependencies
#### ****************************************
tdal_common.$(OBJ_SUFFIX): $(TDAL_COMMON_INCLUDE)/tdal_common.h tdal_common.c makefile
tdal_common_priv.$(OBJ_SUFFIX): $(TDAL_COMMON_INCLUDE)/tdal_common.h tdal_common_priv.c tdal_common_priv.h makefile

# ****************************************
# Always include crules.mak
# ****************************************
include $(TDAL_MAKE)/crules.mak




