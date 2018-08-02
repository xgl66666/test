# ****************************************
# TDAL_TS makefile
# ****************************************

include $(TDAL_MAKE)/environnement.mak
include $(SDK_ROOT)/drivers.mak



# ****************************************
# Define module name
# ****************************************
MODULE  = tdal_ts


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
    $(INCLUDE_PREFIX)$(TDAL_TS_INCLUDE) \
    $(INCLUDE_PREFIX)$(TDAL_COMMON_INCLUDE) \
    $(INCLUDE_PREFIX)$(TDAL_AV_INCLUDE) \
    $(INCLUDE_PREFIX)$(TBOX_INCLUDE)

# ***************************************************
# List of object files needed to create library
# ***************************************************
LIBOBJS = tdal_ts_route.$(OBJ_SUFFIX) \
	tdal_ts_sink.$(OBJ_SUFFIX) \
	        tdal_ts_source.$(OBJ_SUFFIX)


# ****************************************
# List of dependencies
# ****************************************
tdal_ts_route.$(OBJ_SUFFIX): $(TDAL_TS_INCLUDE)/tdal_ts.h tdal_ts_route.c tdal_ts_route_p.h makefile
tdal_ts_sink.$(OBJ_SUFFIX): $(TDAL_TS_INCLUDE)/tdal_ts.h tdal_ts_sink.c tdal_ts_sink_p.h tdal_ts_buffer_p.h makefile
tdal_ts_source.$(OBJ_SUFFIX): $(TDAL_TS_INCLUDE)/tdal_ts.h tdal_ts_source.c tdal_ts_source_p.h tdal_ts_buffer_p.h makefile

# ****************************************
# Always include crules.mak
# ****************************************
include $(TDAL_MAKE)/crules.mak
