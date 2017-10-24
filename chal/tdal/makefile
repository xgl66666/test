# ****************************************
# TBOX makefile
# ****************************************

include $(TDAL_MAKE)/environnement.mak
include $(SDK_ROOT)/drivers.mak

# ****************************************
# Define module name
# ****************************************
MODULE  = tdal

# ****************************************
# This define the default command for gmake
# ****************************************

# ****************************************
# Include paths needed to compile those files
# ****************************************

# ***************************************************
# List of object files needed to create library
# ***************************************************

# ****************************************
# List of dependencies
# ****************************************
TARGETS = $(LIB_PREFFIX)$(MODULE)

$(LIB_PREFFIX)$(MODULE):
ifeq ("$(PRODUCT_PREBUILT_HAL)","NO")
	$(MAKE) -C $(TDAL_SRC_ROOT)
else
	$(CP) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(TDAL_LIB_ROOT)/$(PRODUCT_HAL_TYPE)/libtdal$(PRODUCT_HAL_SUFFIX).a) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_EXPORTS))
endif 

# ****************************************
# Always include crules.mak
# ****************************************
include $(TDAL_MAKE)/crules.mak

clean_all:
	$(MAKE) -ikC $(TDAL_SRC_ROOT) clean_all

