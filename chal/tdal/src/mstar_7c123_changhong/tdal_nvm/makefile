# ****************************************
# TDAL_NVM makefile
# ****************************************

include $(TDAL_MAKE)/environnement.mak
include $(SDK_ROOT)/drivers.mak



# ****************************************
# Define module name
# ****************************************
MODULE  = tdal_nvm


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
    $(INCLUDE_PREFIX)$(TDAL_NVM_INCLUDE) \
    $(INCLUDE_PREFIX)$(TDAL_FLA_INCLUDE) \
    $(INCLUDE_PREFIX)$(TDAL_COMMON_INCLUDE) \
    $(INCLUDE_PREFIX)$(TBOX_INCLUDE)

# ***************************************************
# List of object files needed to create library
# ***************************************************
LIBOBJS = 	tdal_nvm.$(OBJ_SUFFIX)


# ****************************************
# List of dependencies
# ****************************************
tdal_nvm.$(OBJ_SUFFIX): $(TDAL_NVM_INCLUDE)/tdal_nvm.h tdal_nvm.c makefile

# ****************************************
# Always include crules.mak
# ****************************************
include $(TDAL_MAKE)/crules.mak
