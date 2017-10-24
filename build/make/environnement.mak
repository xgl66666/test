
include $(SDK_ROOT)/tools.mak
include $(SDK_ROOT)/drivers.mak
include $(BUILD_MAKE)/host_tools.mak
include $(BUILD_MAKE)/product.mak
include $(BUILD_MAKE)/generic.mak
include $(BUILD_MAKE)/package.mak
include $(BUILD_MAKE)/mapping_flash.mak
include $(BUILD_MAKE)/mapping_ram.mak
include $(BUILD_MAKE)/keyboard.mak
include $(BUILD_MAKE)/avs.mak
include $(BUILD_MAKE)/ca.mak
include $(BUILD_MAKE)/externlibs.mak
ifeq "$(PRODUCT_USE_RPC)" "YES"			
include $(BUILD_MAKE)/rpc.mak
endif
