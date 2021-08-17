# ****************************************
# TKEL makefile
# ****************************************

include $(TKEL_MAKE)/environnement.mak
include $(SDK_ROOT)/drivers.mak

# ****************************************
# Define module name
# ****************************************
MODULE  = tkel$(PRODUCT_HAL_SUFFIX)

CFLAGS := $(CFLAGS) -D __TKEL_THREAD_PRIO_ENABLE__

# ****************************************
# This define the default command for gmake
# ****************************************
all: $(LIB_PREFFIX)$(MODULE).$(LIB_SUFFIX)

# ****************************************
# Include paths needed to compile those files
# ****************************************
DIR_INCLUDE = \
    $(INCLUDE_PREFIX)$(TKEL_INCLUDE) \
    $(INCLUDE_PREFIX)$(TBOX_INCLUDE) \
    $(addprefix $(INCLUDE_PREFIX), $(OS_INCLUDE_PATH)) \
    $(addprefix $(INCLUDE_PREFIX), $(SDK_INCLUDE_PATH)) \

# ***************************************************
# List of object files needed to create library
# ***************************************************
LIBOBJS = tkel_ecos.$(OBJ_SUFFIX)
LIBOBJS += tkel_ecos_task_communication.$(OBJ_SUFFIX)
LIBOBJS += tkel_ecos_task_management.$(OBJ_SUFFIX)
LIBOBJS += tkel_ecos_task_synchronization.$(OBJ_SUFFIX)
LIBOBJS += tkel_ecos_timer_management.$(OBJ_SUFFIX)

ifeq "$(TKEL_USE_BGET)" "1"
LIBOBJS += tkel_bget.$(OBJ_SUFFIX)
endif

ifeq "$(TKEL_USE_DLMALLOC)" "1"
LIBOBJS += tkel_dlmalloc.$(OBJ_SUFFIX)
endif

# ****************************************
# List of dependencies
# ****************************************
tkel_ecos.$(OBJ_SUFFIX): tkel_ecos.c $(TKEL_INCLUDE)/tkel.h makefile
tkel_ecos_task_communication.$(OBJ_SUFFIX): tkel_ecos_task_communication.c $(TKEL_INCLUDE)/tkel.h makefile
tkel_ecos_task_management.$(OBJ_SUFFIX): tkel_ecos_task_management.c $(TKEL_INCLUDE)/tkel.h makefile
tkel_ecos_task_synchronization$(OBJ_SUFFIX): tkel_ecos_task_synchronization.c $(TKEL_INCLUDE)/tkel.h makefile
tkel_ecos_timer_management.$(OBJ_SUFFIX): tkel_ecos_timer_management.c $(TKEL_INCLUDE)/tkel.h makefile
tkel_bget.$(OBJ_SUFFIX): tkel_bget.c tkel_bget.h makefile
tkel_dlmalloc.$(OBJ_SUFFIX): tkel_dlmalloc.c tkel_dlmalloc.h tkel_dlmalloc_config.h makefile

# ****************************************
# Always include crules.mak
# ****************************************
include $(TKEL_MAKE)/crules.mak

