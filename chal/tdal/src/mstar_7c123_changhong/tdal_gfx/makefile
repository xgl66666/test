# ****************************************
# TDAL_GFX makefile
# ****************************************

include $(TDAL_MAKE)/environnement.mak
include $(SDK_ROOT)/drivers.mak



# ****************************************
# Define module name
# ****************************************
MODULE  = tdal_gfx


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
    $(INCLUDE_PREFIX)$(TDAL_GFX_INCLUDE) \
	$(INCLUDE_PREFIX)$(TDAL_DISP_INCLUDE) \
    $(INCLUDE_PREFIX)$(TDAL_COMMON_INCLUDE) \
    $(INCLUDE_PREFIX)$(TBOX_INCLUDE) \

# ***************************************************
# List of object files needed to create library
# ***************************************************
LIBOBJS = 	tdal_gfx.$(OBJ_SUFFIX) \
			tdal_gfx_blit.$(OBJ_SUFFIX) \
			tdal_gfx_region.$(OBJ_SUFFIX)


# ****************************************
# List of dependencies
# ****************************************
tdal_gfx.$(OBJ_SUFFIX): $(TDAL_GFX_INCLUDE)/tdal_gfx.h tdal_gfx.c makefile tdal_gfx_module_priv.h
tdal_gfx_blit.$(OBJ_SUFFIX): $(TDAL_GFX_INCLUDE)/tdal_gfx.h tdal_gfx_blit.c makefile tdal_gfx_module_priv.h
tdal_gfx_region.$(OBJ_SUFFIX): $(TDAL_GFX_INCLUDE)/tdal_gfx.h tdal_gfx_region.c makefile tdal_gfx_module_priv.h

# ****************************************
# Always include crules.mak
# ****************************************
include $(TDAL_MAKE)/crules.mak
