# ****************************************
# TDAL_AV makefile
# ****************************************

include $(TDAL_MAKE)/environnement.mak
include $(SDK_ROOT)/drivers.mak



# ****************************************
# Define module name
# ****************************************
MODULE  = tdal_av


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
    $(INCLUDE_PREFIX)$(TDAL_AV_INCLUDE) \
    $(INCLUDE_PREFIX)$(TDAL_DISP_INCLUDE) \
    $(INCLUDE_PREFIX)$(TDAL_OUTPUT_INCLUDE) \
    $(INCLUDE_PREFIX)$(TDAL_COMMON_INCLUDE) \
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
LIBOBJS = 	tdal_av.$(OBJ_SUFFIX) \
			tdal_av_video.$(OBJ_SUFFIX) \
			tdal_av_audio.$(OBJ_SUFFIX)


# ****************************************
# List of dependencies
# ****************************************
tdal_av.$(OBJ_SUFFIX): $(TDAL_AV_INCLUDE)/tdal_av.h tdal_av.c makefile
tdal_av_video.$(OBJ_SUFFIX): $(TDAL_AV_INCLUDE)/tdal_av.h tdal_av_p.h tdal_av_video.c makefile
tdal_av_audio.$(OBJ_SUFFIX): $(TDAL_AV_INCLUDE)/tdal_av.h tdal_av_p.h tdal_av_audio.c makefile

# ****************************************
# Always include crules.mak
# ****************************************
include $(TDAL_MAKE)/crules.mak
