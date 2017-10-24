# ****************************************
# TDAL_DISP makefile
# ****************************************

include $(TDAL_MAKE)/environnement.mak
include $(SDK_ROOT)/drivers.mak


# ****************************************
# Define module name
# ****************************************
MODULE  = tdal_disp


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
    $(INCLUDE_PREFIX)$(TDAL_DISP_INCLUDE) \
    $(INCLUDE_PREFIX)$(TDAL_COMMON_INCLUDE) \
    $(INCLUDE_PREFIX)$(TDAL_AV_INCLUDE) \
	$(INCLUDE_PREFIX)$(TDAL_OUTPUT_INCLUDE) \
    $(INCLUDE_PREFIX)$(TBOX_INCLUDE) \

ifneq "$(DRIVERS_CFG_CHIP_INCLUDE)" ""
    DIR_INCLUDE += $(INCLUDE_PREFIX)$(DRIVERS_CFG_CHIP_INCLUDE)
endif

ifneq "$(DRIVERS_CFG_PLTF_INCLUDE)" ""
    DIR_INCLUDE += $(INCLUDE_PREFIX)$(DRIVERS_CFG_PLTF_INCLUDE)
endif


# ***************************************************
# List of object files needed to create library
# ***************************************************
LIBOBJS = 	tdal_disp.$(OBJ_SUFFIX) \
			tdal_disp_blender.$(OBJ_SUFFIX) \
			tdal_disp_layer.$(OBJ_SUFFIX)


# ****************************************
# List of dependencies
# ****************************************
tdal_disp.$(OBJ_SUFFIX): tdal_disp.h tdal_disp_priv.h tdal_disp.c makefile tdal_gfx_module_priv.h
tdal_disp_blender.$(OBJ_SUFFIX): tdal_disp.h tdal_disp_priv.h tdal_disp_blender.c makefile tdal_gfx_module_priv.h
tdal_disp_layer.$(OBJ_SUFFIX): tdal_disp.h tdal_disp_priv.h tdal_disp_layer.c makefile tdal_gfx_module_priv.h


# ****************************************
# Always include crules.mak
# ****************************************
include $(TDAL_MAKE)/crules.mak

