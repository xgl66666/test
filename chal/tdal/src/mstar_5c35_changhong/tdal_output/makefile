# ****************************************
# TDAL_OUTPUT makefile
# ****************************************

include $(TDAL_MAKE)/environnement.mak
include $(SDK_ROOT)/drivers.mak


# ****************************************
# Define module name
# ****************************************
MODULE  = tdal_output


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
    $(INCLUDE_PREFIX)$(TDAL_OUTPUT_INCLUDE) \
    $(INCLUDE_PREFIX)$(TDAL_COMMON_INCLUDE) \
	$(INCLUDE_PREFIX)$(TDAL_DISP_INCLUDE) \
	$(INCLUDE_PREFIX)$(TDAL_AV_INCLUDE) \
    $(INCLUDE_PREFIX)$(TBOX_INCLUDE) \

# ***************************************************
# List of object files needed to create library
# ***************************************************
LIBOBJS = 	tdal_output.$(OBJ_SUFFIX) \
			tdal_output_audio_analog.$(OBJ_SUFFIX) \
			tdal_output_audio_spdif.$(OBJ_SUFFIX) \
			tdal_output_rf_mod.$(OBJ_SUFFIX) \
			tdal_output_video_denc.$(OBJ_SUFFIX) \
			tdal_output_video_hd_dac.$(OBJ_SUFFIX) \
            tdal_output_headphone.$(OBJ_SUFFIX)


# ****************************************
# List of dependencies
# ****************************************
tdal_output.$(OBJ_SUFFIX): tdal_output.h tdal_output_priv.h tdal_output.c makefile
tdal_output_audio_analog.$(OBJ_SUFFIX): tdal_output.h tdal_output_priv.h tdal_output_audio_analog.c makefile
tdal_output_audio_spdif.$(OBJ_SUFFIX): tdal_output.h tdal_output_priv.h tdal_output_audio_spdif.c makefile
tdal_output_rf_mod.$(OBJ_SUFFIX): tdal_output.h tdal_output_priv.h tdal_output_rf_mod.c makefile
tdal_output_video_denc.$(OBJ_SUFFIX): tdal_output.h tdal_output_priv.h tdal_output_video_denc.c makefile
tdal_output_video_hd_dac.$(OBJ_SUFFIX): tdal_output.h tdal_output_priv.h tdal_output_video_hd_dac.c makefile
tdal_output_headphone.$(OBJ_SUFFIX): tdal_output.h tdal_output_priv.h tdal_output_headphone.c makefile


# ****************************************
# Always include crules.mak
# ****************************************
include $(TDAL_MAKE)/crules.mak

