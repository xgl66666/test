# ****************************************
# TDAL_FP makefile
# ****************************************

include $(TDAL_MAKE)/environnement.mak
include $(SDK_ROOT)/drivers.mak



# ****************************************
# Define module name
# ****************************************
MODULE  = tdal_fp


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
    $(INCLUDE_PREFIX)$(TDAL_FP_INCLUDE) \
    $(INCLUDE_PREFIX)$(TDAL_COMMON_INCLUDE) \
    $(INCLUDE_PREFIX)$(TBOX_INCLUDE)

# ***************************************************
# List of object files needed to create library
# ***************************************************
LIBOBJS = 	tdal_fp.$(OBJ_SUFFIX) \
            tdal_fp_key.$(OBJ_SUFFIX)
            
LIBOBJS += $(MSD_CUST_DEMO)/obj/drvFrontPnl.$(OBJ_SUFFIX)

# ****************************************
# List of dependencies
# ****************************************
tdal_fp.$(OBJ_SUFFIX): $(TDAL_FP_INCLUDE)/tdal_fp.h tdal_fp.c makefile
tdal_fp_key.$(OBJ_SUFFIX): $(TDAL_FP_INCLUDE)/tdal_fp.h tdal_fp_key.c makefile

# ****************************************
# Always include crules.mak
# ****************************************
include $(TDAL_MAKE)/crules.mak
