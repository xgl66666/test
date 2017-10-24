## ******************************************************
##         COPYRIGHT 2005 IWEDIA TECHNOLOGIES           *
## ******************************************************
##
## FILE NAME: $URL$
##            $Rev$
##            $Date$
##
## DESCRIPTION :
##
## *******************************************************

# ****************************************
# co'media system makefile
# ****************************************
include $(TKEL_MAKE)/environnement.mak

# ****************************************
# Define module name
# ****************************************
MODULE = TKEL

# ****************************************
# Find proper implementations
# ****************************************

# Other components this component is dependant upon
IMPORTS =
EXPORTS =

TARGETS = tkel$(TKEL_OS).lib

tkel$(TKEL_OS).lib:
ifeq ("$(PRODUCT_PREBUILT_HAL)","NO")
	$(MAKE) -C $(TKEL_SRC_ROOT)
else
	$(CP) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(TKEL_LIB_ROOT)/$(PRODUCT_HAL_TYPE)/libtkel$(PRODUCT_HAL_SUFFIX).a) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_EXPORTS)) 
endif    
	
# ****************************************
# Always include crules.mak
# ****************************************
include $(TKEL_MAKE)/crules.mak

clean_all:
ifeq ("$(PRODUCT_PREBUILT_HAL)","NO")
	$(MAKE) -ikC $(TKEL_SRC_ROOT) clean
endif
