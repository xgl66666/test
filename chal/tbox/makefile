# ****************************************
# TBOX makefile
# ****************************************

include $(TBOX_MAKE)/environnement.mak
include $(SDK_ROOT)/drivers.mak

# ****************************************
# Define module name
# ****************************************
MODULE  = tbox$(PRODUCT_HAL_SUFFIX)

# ****************************************
# This define the default command for gmake
# ****************************************
ifeq ("$(PRODUCT_PREBUILT_HAL)","NO")
all: $(LIB_PREFFIX)$(MODULE).$(LIB_SUFFIX)
else
all:
	$(CP) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(TBOX_LIB_ROOT)/$(PRODUCT_HAL_TYPE)/libtbox$(PRODUCT_HAL_SUFFIX).a) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_EXPORTS))
	@- $(CP) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(TBOX_LIB_ROOT)/$(PRODUCT_HAL_TYPE)/libboot_trace.a) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_EXPORTS))
endif  

# ****************************************
# Include paths needed to compile those files
# ****************************************
DIR_INCLUDE = \
    $(INCLUDE_PREFIX)$(TBOX_INCLUDE) \
    $(INCLUDE_PREFIX)$(TDAL_INCLUDE) \
    $(INCLUDE_PREFIX)$(TBOX_SRC_ROOT) \
    $(addprefix $(INCLUDE_PREFIX), $(OS_INCLUDE_PATH)) \
    $(addprefix $(INCLUDE_PREFIX), $(SDK_INCLUDE_PATH)) \

ifeq "$(PRODUCT_CCAS_DLK)" "YES"
    DIR_INCLUDE += $(INCLUDE_PREFIX)$(CAK_INCLUDE)
endif

# ***************************************************
# List of object files needed to create library
# ***************************************************
LIBOBJS = 	$(TBOX_ROOT)/src/tbox.$(OBJ_SUFFIX) \
			$(TBOX_SRC_ROOT)/tbox_hard.$(OBJ_SUFFIX)

# ****************************************
# List of dependencies
# ****************************************
tbox.$(OBJ_SUFFIX): $(TBOX_INCLUDE)/tbox.h tbox.c makefile
tbox_hard.$(OBJ_SUFFIX): $(TBOX_INCLUDE)/tbox.h tbox_hard.c $(TBOX_SRC_ROOT)/tbox_hard_p.h makefile

# ****************************************
# Always include crules.mak
# ****************************************
include $(TKEL_MAKE)/crules.mak

clean_all:
	$(MAKE) -ikC $(TBOX_ROOT)/src clean
	$(MAKE) -ikC $(TBOX_SRC_ROOT) clean

