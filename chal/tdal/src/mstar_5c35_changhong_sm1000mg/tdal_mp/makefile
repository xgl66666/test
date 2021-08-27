# ****************************************
# TDAL_MP makefile
# ****************************************

include $(TDAL_MAKE)/environnement.mak
include $(SDK_ROOT)/drivers.mak


# ****************************************
# Define module name
# ****************************************
MODULE  = tdal_mp


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
	$(INCLUDE_PREFIX)$(TDAL_MP_INCLUDE) \
	$(INCLUDE_PREFIX)$(TDAL_COMMON_INCLUDE) \
	$(INCLUDE_PREFIX)$(TBOX_INCLUDE) \

# ***************************************************
# List of object files needed to create library
# ***************************************************
LIBOBJS = 	tdal_mp_generic.$(OBJ_SUFFIX) \
			tdal_mp_control.$(OBJ_SUFFIX) \
			tdal_mp_subrender.$(OBJ_SUFFIX) \
			tdal_mp_status.$(OBJ_SUFFIX) \
			tdal_mp_windowing.$(OBJ_SUFFIX)

# ****************************************
# List of dependencies
# ****************************************
tdal_mp_generic.$(OBJ_SUFFIX): $(TDAL_MP_INCLUDE)/tdal_mp.h tdal_mp_generic.c tdal_mp_p.h makefile
tdal_mp_control.$(OBJ_SUFFIX): $(TDAL_FS_INCLUDE)/tdal_mp.h tdal_mp_control.c tdal_mp_p.h makefile
tdal_mp_subrender.$(OBJ_SUFFIX): $(TDAL_FS_INCLUDE)/tdal_mp.h tdal_mp_subrender.c tdal_mp_p.h makefile
tdal_mp_status.$(OBJ_SUFFIX): $(TDAL_MP_INCLUDE)/tdal_mp.h tdal_mp_status.c tdal_mp_p.h makefile
tdal_mp_windowing.$(OBJ_SUFFIX): $(TDAL_MP_INCLUDE)/tdal_mp.h tdal_mp_windowing.c tdal_mp_p.h makefile

# ****************************************
# Always include crules.mak
# ****************************************
include $(TDAL_MAKE)/crules.mak
