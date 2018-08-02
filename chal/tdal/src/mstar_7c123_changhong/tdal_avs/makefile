# ****************************************
# TDAL_AVS makefile
# ****************************************

include $(TDAL_MAKE)/environnement.mak
include $(SDK_ROOT)/drivers.mak



# ****************************************
# Define module name
# ****************************************
MODULE  = tdal_avs


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
    $(INCLUDE_PREFIX)$(TDAL_AVS_INCLUDE) \
    $(INCLUDE_PREFIX)$(TDAL_COMMON_INCLUDE) \
    $(INCLUDE_PREFIX)$(TBOX_INCLUDE)

ifneq "$(DRIVERS_CFG_CHIP_INCLUDE)" ""
  DIR_INCLUDE += $(INCLUDE_PREFIX)$(DRIVERS_CFG_CHIP_INCLUDE)
endif

ifneq "$(DRIVERS_CFG_PLTF_INCLUDE)" ""
  DIR_INCLUDE += $(INCLUDE_PREFIX)$(DRIVERS_CFG_PLTF_INCLUDE)
endif

# ***************************************************
# List of object files needed to create library
# ***************************************************
#ifeq "$(PRODUCT_AVS)" "AVS_AKM"
#LIBOBJS = 	tdal_avs_akm.$(OBJ_SUFFIX)
#endif
#ifeq "$(PRODUCT_AVS)" "AVS_ST"
#LIBOBJS = 	tdal_avs.$(OBJ_SUFFIX)
#endif

LIBOBJS = 	tdal_avs.$(OBJ_SUFFIX)

# ****************************************
# List of dependencies
# ****************************************
tdal_avs.$(OBJ_SUFFIX): $(TDAL_AVS_INCLUDE)/tdal_avs.h tdal_avs.c makefile
#tdal_avs_akm.$(OBJ_SUFFIX): $(TDAL_AVS_INCLUDE)/tdal_avs.h tdal_avs_akm.c makefile

# ****************************************
# Always include crules.mak
# ****************************************
include $(TDAL_MAKE)/crules.mak
