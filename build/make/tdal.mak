# ***************************************************
# List of object files needed to create library
# ***************************************************

TDAL_LIST = \
     $(LIB_PREFFIX)tdal_common.$(LIB_SUFFIX) \
     $(LIB_PREFFIX)tdal_dmd.$(LIB_SUFFIX) \
     $(LIB_PREFFIX)tdal_dmx.$(LIB_SUFFIX) \
     $(LIB_PREFFIX)tdal_fla.$(LIB_SUFFIX) \
     $(LIB_PREFFIX)tdal_nvm.$(LIB_SUFFIX) \
     $(LIB_PREFFIX)tdal_uart.$(LIB_SUFFIX) \
     $(LIB_PREFFIX)tdal_av.$(LIB_SUFFIX) \
     $(LIB_PREFFIX)tdal_gfx.$(LIB_SUFFIX) \
     $(LIB_PREFFIX)tdal_kbd.$(LIB_SUFFIX) \
     $(LIB_PREFFIX)tdal_ptsm.$(LIB_SUFFIX) \
     $(LIB_PREFFIX)tdal_disp.$(LIB_SUFFIX) \
     $(LIB_PREFFIX)tdal_output.$(LIB_SUFFIX) \
     $(LIB_PREFFIX)tdal_vbi.$(LIB_SUFFIX) \
     $(LIB_PREFFIX)tdal_hdmi.$(LIB_SUFFIX) \
     $(LIB_PREFFIX)tdal_ts.$(LIB_SUFFIX)
     
ifeq "$(PRODUCT_USE_SSU)" "TDAL_SSU"
     TDAL_LIST += $(LIB_PREFFIX)tdal_ssu.$(LIB_SUFFIX)
endif

ifeq "$(PRODUCT_USE_TDAL_PVR)" "YES"
TDAL_LIST += $(LIB_PREFFIX)tdal_pvr.$(LIB_SUFFIX)
endif			
	 
ifeq "$(PRODUCT_USE_CA)" "YES"			
TDAL_LIST += $(LIB_PREFFIX)tdal_desc.$(LIB_SUFFIX) \
			$(LIB_PREFFIX)tdal_smc.$(LIB_SUFFIX) \
			$(LIB_PREFFIX)tdal_perso.$(LIB_SUFFIX) \
			$(LIB_PREFFIX)tdal_sec.$(LIB_SUFFIX)
endif



ifeq "$(PRODUCT_USE_CI)" "YES"
TDAL_LIST += $(LIB_PREFFIX)tdal_ci.$(LIB_SUFFIX)
endif

ifeq "$(PRODUCT_USE_RPC)" "YES"			
TDAL_LIST += $(LIB_PREFFIX)tdal_rpc.$(LIB_SUFFIX)
endif

ifeq "$(PRODUCT_USE_TDAL_FS)" "YES"
  TDAL_LIST += $(LIB_PREFFIX)tdal_fs.$(LIB_SUFFIX)
endif

ifeq "$(PRODUCT_USE_FP)" "YES"			
TDAL_LIST += $(LIB_PREFFIX)tdal_fp.$(LIB_SUFFIX)
endif

ifneq "$(PRODUCT_AVS)" "NO_AVS"
     TDAL_LIST += $(LIB_PREFFIX)tdal_avs.$(LIB_SUFFIX)
endif

ifeq "$(PRODUCT_USE_TDAL_MP)" "YES"
     TDAL_LIST += $(LIB_PREFFIX)tdal_mp.$(LIB_SUFFIX)
endif

ifeq "$(USE_TDAL_DIAG)" "ON"
  TDAL_LIST += $(LIB_PREFFIX)tdal_diag.$(LIB_SUFFIX)
endif

TDAL_LIST_LIBOBJS = \
			$(BUILD_EXPORTS)/$(LIB_PREFFIX)tdal_common.$(LIB_SUFFIX) \
			$(BUILD_EXPORTS)/$(LIB_PREFFIX)tdal_dmd.$(LIB_SUFFIX) \
			$(BUILD_EXPORTS)/$(LIB_PREFFIX)tdal_dmx.$(LIB_SUFFIX) \
			$(BUILD_EXPORTS)/$(LIB_PREFFIX)tdal_fla.$(LIB_SUFFIX) \
			$(BUILD_EXPORTS)/$(LIB_PREFFIX)tdal_nvm.$(LIB_SUFFIX) \
			$(BUILD_EXPORTS)/$(LIB_PREFFIX)tdal_uart.$(LIB_SUFFIX) \
            $(BUILD_EXPORTS)/$(LIB_PREFFIX)tdal_av.$(LIB_SUFFIX) \
            $(BUILD_EXPORTS)/$(LIB_PREFFIX)tdal_disp.$(LIB_SUFFIX) \
            $(BUILD_EXPORTS)/$(LIB_PREFFIX)tdal_output.$(LIB_SUFFIX) \
			$(BUILD_EXPORTS)/$(LIB_PREFFIX)tdal_gfx.$(LIB_SUFFIX) \
			$(BUILD_EXPORTS)/$(LIB_PREFFIX)tdal_kbd.$(LIB_SUFFIX) \
			$(BUILD_EXPORTS)/$(LIB_PREFFIX)tdal_ptsm.$(LIB_SUFFIX) \
			$(BUILD_EXPORTS)/$(LIB_PREFFIX)tdal_vbi.$(LIB_SUFFIX) \
            $(BUILD_EXPORTS)/$(LIB_PREFFIX)tdal_hdmi.$(LIB_SUFFIX) \
			$(BUILD_EXPORTS)/$(LIB_PREFFIX)tdal_ts.$(LIB_SUFFIX)
			
ifeq "$(PRODUCT_USE_SSU)" "TDAL_SSU"
			TDAL_LIST_LIBOBJS += $(BUILD_EXPORTS)/$(LIB_PREFFIX)tdal_ssu.$(LIB_SUFFIX)
endif


ifeq "$(PRODUCT_USE_CA)" "YES"			
TDAL_LIST_LIBOBJS += $(BUILD_EXPORTS)/$(LIB_PREFFIX)tdal_desc.$(LIB_SUFFIX) \
			         $(BUILD_EXPORTS)/$(LIB_PREFFIX)tdal_smc.$(LIB_SUFFIX) \
			         $(BUILD_EXPORTS)/$(LIB_PREFFIX)tdal_perso.$(LIB_SUFFIX) \
					 $(BUILD_EXPORTS)/$(LIB_PREFFIX)tdal_sec.$(LIB_SUFFIX)
endif

ifeq "$(PRODUCT_USE_CI)" "YES"			
TDAL_LIST_LIBOBJS += $(BUILD_EXPORTS)/$(LIB_PREFFIX)tdal_ci.$(LIB_SUFFIX)
endif

ifeq "$(PRODUCT_USE_RPC)" "YES"			
TDAL_LIST_LIBOBJS += $(BUILD_EXPORTS)/$(LIB_PREFFIX)tdal_rpc.$(LIB_SUFFIX)
endif

ifeq "$(PRODUCT_USE_TDAL_FS)" "YES"
  TDAL_LIST_LIBOBJS += $(BUILD_EXPORTS)/$(LIB_PREFFIX)tdal_fs.$(LIB_SUFFIX)
endif

ifeq "$(USE_TDAL_DIAG)" "ON"
  TDAL_LIST_LIBOBJS += $(BUILD_EXPORTS)/$(LIB_PREFFIX)tdal_diag.$(LIB_SUFFIX)
endif

ifeq "$(PRODUCT_USE_FP)" "YES"			
TDAL_LIST_LIBOBJS += $(BUILD_EXPORTS)/$(LIB_PREFFIX)tdal_fp.$(LIB_SUFFIX)
endif

ifneq "$(PRODUCT_AVS)" "NO_AVS"
TDAL_LIST_LIBOBJS += $(BUILD_EXPORTS)/$(LIB_PREFFIX)tdal_avs.$(LIB_SUFFIX)
endif

ifeq "$(PRODUCT_USE_TDAL_MP)" "YES"
TDAL_LIST_LIBOBJS += $(BUILD_EXPORTS)/$(LIB_PREFFIX)tdal_mp.$(LIB_SUFFIX)
endif

ifeq "$(PRODUCT_USE_TDAL_PVR)" "YES"
TDAL_LIST_LIBOBJS += $(BUILD_EXPORTS)/$(LIB_PREFFIX)tdal_pvr.$(LIB_SUFFIX)
endif

##########################################################################
#
# Necessary for tdal test
#


ifeq "$(TDAL_ALL_TEST)" "1"
	TDAL_AV_TEST=1
    TDAL_CI_TEST=1
	TDAL_DMD_TEST=1
	TDAL_DMX_TEST=1
	TDAL_FLA_TEST=1
	TDAL_GFX_TEST=1
	TDAL_KBD_TEST=1
    TDAL_FP_TEST=1
	TDAL_NVM_TEST=1
    TDAL_SMC_TEST = 1
	TDAL_UART_TEST=1
endif

TDAL_TEST_INCLUDE =$(INCLUDE_PREFIX)$(TDAL_APPTESTS_ROOT)/include \


ifeq "$(TDAL_AV_TEST)" "1"
	TDAL_TEST_INCLUDE += $(INCLUDE_PREFIX)$(TDAL_AV_INCLUDE) \
			            $(INCLUDE_PREFIX)$(TDAL_ROOT)/tdal_av/tst
	TDAL_TEST_OBJS += $(TDAL_ROOT)/tdal_av/tst/tdal_av_test.$(OBJ_SUFFIX) \
		$(TDAL_ROOT)/tdal_av/tst/cotdalav.$(OBJ_SUFFIX)

	CFLAGS += -DTDAL_AV_TEST_INCLUDED
endif
ifeq "$(TDAL_AVS_TEST)" "1"
	TDAL_TEST_INCLUDE += $(INCLUDE_PREFIX)$(TDAL_AVS_INCLUDE) \
			$(INCLUDE_PREFIX)$(TDAL_ROOT)/tdal_avs/tst
	TDAL_TEST_OBJS += $(TDAL_ROOT)/tdal_avs/tst/cotdalavs.$(OBJ_SUFFIX) \
		    $(TDAL_ROOT)/tdal_avs/tst/tdal_avs_test.$(OBJ_SUFFIX)

	CFLAGS += -DTDAL_AVS_TEST_INCLUDED
endif
ifeq "$(TDAL_CI_TEST)" "1"
	TDAL_TEST_INCLUDE += $(INCLUDE_PREFIX)$(TDAL_CI_INCLUDE) \
			$(INCLUDE_PREFIX)$(TDAL_ROOT)/tdal_ci/tst
	TDAL_TEST_OBJS += $(TDAL_ROOT)/tdal_ci/tst/cotdalci.$(OBJ_SUFFIX) \
		    $(TDAL_ROOT)/tdal_ci/tst/tdal_ci_test.$(OBJ_SUFFIX)

	CFLAGS += -DTDAL_CI_TEST_INCLUDED
endif
ifeq "$(TDAL_DMD_TEST)" "1"
	TDAL_TEST_INCLUDE += $(INCLUDE_PREFIX)$(TDAL_DMD_INCLUDE) \
			$(INCLUDE_PREFIX)$(TDAL_ROOT)/tdal_dmd/tst
	TDAL_TEST_OBJS += $(TDAL_ROOT)/tdal_dmd/tst/cotdaldmd.$(OBJ_SUFFIX) \
		     $(TDAL_ROOT)/tdal_dmd/tst/tdal_dmd_test.$(OBJ_SUFFIX)
		
	CFLAGS += -DTDAL_DMD_TEST_INCLUDED
endif
ifeq "$(TDAL_DMX_TEST)" "1"
	TDAL_TEST_INCLUDE += $(INCLUDE_PREFIX)$(TDAL_DMX_INCLUDE) \
			$(INCLUDE_PREFIX)$(TDAL_ROOT)/tdal_dmx/tst
	TDAL_TEST_OBJS += $(TDAL_ROOT)/tdal_dmx/tst/cotdaldmx.$(OBJ_SUFFIX) \
		     $(TDAL_ROOT)/tdal_dmx/tst/tdal_dmx_data_test.$(OBJ_SUFFIX) \
		     $(TDAL_ROOT)/tdal_dmx/tst/tdal_dmx_dmd_test.$(OBJ_SUFFIX) \
		     $(TDAL_ROOT)/tdal_dmx/tst/tdal_dmx_test.$(OBJ_SUFFIX)

	CFLAGS += -DTDAL_DMX_TEST_INCLUDED
endif
ifeq "$(TDAL_FLA_TEST)" "1"
	TDAL_TEST_INCLUDE +=	$(INCLUDE_PREFIX)$(TDAL_FLA_INCLUDE) \
			$(INCLUDE_PREFIX)$(TDAL_ROOT)/tdal_fla/tst
	TDAL_TEST_OBJS += $(TDAL_ROOT)/tdal_fla/tst/cotdal_fla.$(OBJ_SUFFIX) \
		    $(TDAL_ROOT)/tdal_fla/tst/tdal_fla_test.$(OBJ_SUFFIX)

	CFLAGS += -DTDAL_FLA_TEST_INCLUDED
endif
ifeq "$(TDAL_GFX_TEST)" "1"
	TDAL_TEST_INCLUDE +=	$(INCLUDE_PREFIX)$(TDAL_GFX_INCLUDE) \
			$(INCLUDE_PREFIX)$(TDAL_ROOT)/tdal_gfx/tst
	TDAL_TEST_OBJS += $(TDAL_ROOT)/tdal_gfx/tst/cotdalgfx.$(OBJ_SUFFIX) \
		    $(TDAL_ROOT)/tdal_gfx/tst/tdal_gfx_test.$(OBJ_SUFFIX)
		
	CFLAGS += -DTDAL_GFX_TEST_INCLUDED
endif
ifeq "$(TDAL_KBD_TEST)" "1"
	TDAL_TEST_INCLUDE +=	$(INCLUDE_PREFIX)$(TDAL_KBD_INCLUDE) \
			$(INCLUDE_PREFIX)$(TDAL_ROOT)/tdal_kbd/tst
	TDAL_TEST_OBJS += $(TDAL_ROOT)/tdal_kbd/tst/cotdalkbd.$(OBJ_SUFFIX) \
		    $(TDAL_ROOT)/tdal_kbd/tst/tdal_kbd_test.$(OBJ_SUFFIX)

	CFLAGS += -DTDAL_KBD_TEST_INCLUDED
endif
ifeq "$(TDAL_FP_TEST)" "1"
        TDAL_TEST_INCLUDE +=    $(INCLUDE_PREFIX)$(TDAL_FP_INCLUDE) \
                        $(INCLUDE_PREFIX)$(TDAL_ROOT)/tdal_fp/tst
        TDAL_TEST_OBJS += $(TDAL_ROOT)/tdal_fp/tst/cotdalfp.$(OBJ_SUFFIX) \
                    $(TDAL_ROOT)/tdal_fp/tst/tdal_fp_test.$(OBJ_SUFFIX)

        CFLAGS += -DTDAL_FP_TEST_INCLUDED
endif
ifeq "$(TDAL_RPC_TEST)" "1"
	TDAL_TEST_INCLUDE +=	$(INCLUDE_PREFIX)$(TDAL_RPC_INCLUDE) \
			$(INCLUDE_PREFIX)$(TDAL_ROOT)/tdal_rpc/tst
	TDAL_TEST_OBJS += $(TDAL_ROOT)/tdal_rpc/tst/cotdalrpc.$(OBJ_SUFFIX) \
		    $(TDAL_ROOT)/tdal_rpc/tst/tdal_rpc_test.$(OBJ_SUFFIX)

	CFLAGS += -DTDAL_RPC_TEST_INCLUDED
endif
ifeq "$(TDAL_NVM_TEST)" "1"
	TDAL_TEST_INCLUDE +=	$(INCLUDE_PREFIX)$(TDAL_NVM_INCLUDE) \
			$(INCLUDE_PREFIX)$(TDAL_ROOT)/tdal_nvm/tst
	TDAL_TEST_OBJS += $(TDAL_ROOT)/tdal_nvm/tst/cotdalnvm.$(OBJ_SUFFIX) \
		    $(TDAL_ROOT)/tdal_nvm/tst/tdal_nvm_test.$(OBJ_SUFFIX)
		
	CFLAGS += -DTDAL_NVM_TEST_INCLUDED
endif
ifeq "$(TDAL_SMC_TEST)" "1"
	TDAL_TEST_INCLUDE +=	$(INCLUDE_PREFIX)$(TDAL_SMC_INCLUDE) \
			$(INCLUDE_PREFIX)$(TDAL_ROOT)/tdal_smc/tst
	TDAL_TEST_OBJS += $(TDAL_ROOT)/tdal_smc/tst/cotdalsmc.$(OBJ_SUFFIX) \
		    $(TDAL_ROOT)/tdal_smc/tst/tdal_smc_test.$(OBJ_SUFFIX)

	CFLAGS += -DTDAL_SMC_TEST_INCLUDED
endif
ifeq "$(TDAL_TSIN_TEST)" "1"
	TDAL_TEST_INCLUDE += $(INCLUDE_PREFIX)$(TDAL_TSIN_INCLUDE) \
			$(INCLUDE_PREFIX)$(TDAL_ROOT)/tdal_tsin/tst
	TDAL_TEST_OBJS += $(TDAL_ROOT)/tdal_tsin/tst/cotdal_tsin.$(OBJ_SUFFIX) \
		    $(TDAL_ROOT)/tdal_tsin/tst/tdal_tsin_test.$(OBJ_SUFFIX)
		
	CFLAGS += -DTDAL_TSIN_TEST_INCLUDED
endif
ifeq "$(TDAL_UART_TEST)" "1"
	TDAL_TEST_INCLUDE +=	$(INCLUDE_PREFIX)$(TDAL_UART_INCLUDE) \
			$(INCLUDE_PREFIX)$(TDAL_ROOT)/tdal_uart/tst
	TDAL_TEST_OBJS += $(TDAL_ROOT)/tdal_uart/tst/cotdaluart.$(OBJ_SUFFIX) \
		    $(TDAL_ROOT)/tdal_uart/tst/tdal_uart_test.$(OBJ_SUFFIX)

	CFLAGS += -DTDAL_UART_TEST_INCLUDED
endif
ifeq "$(TDAL_VBI_TEST)" "1"
	TDAL_TEST_INCLUDE +=	$(INCLUDE_PREFIX)$(TDAL_VBI_INCLUDE) \
			$(INCLUDE_PREFIX)$(TDAL_ROOT)/tdal_vbi/tst			
	TDAL_TEST_OBJS += $(TDAL_ROOT)/tdal_vbi/tst/cotdalvbi.$(OBJ_SUFFIX) \
		    $(TDAL_ROOT)/tdal_vbi/tst/tdal_vbi_test.$(OBJ_SUFFIX)
	CFLAGS += -DTDAL_VBI_TEST_INCLUDED
endif

ifeq "$(TDAL_INTEGRATION_TEST)" "1"
	TDAL_TEST_INCLUDE +=	$(INCLUDE_PREFIX)$(TDAL_APPTESTS_ROOT)/src/integration \
		$(INCLUDE_PREFIX)$(TDAL_AV_INCLUDE) \
		$(INCLUDE_PREFIX)$(TDAL_DMD_INCLUDE) \
		$(INCLUDE_PREFIX)$(TDAL_DMX_INCLUDE) \
		$(INCLUDE_PREFIX)$(TDAL_KBD_INCLUDE) \
		$(INCLUDE_PREFIX)$(TDAL_GFX_INCLUDE)
	TDAL_TEST_OBJS += $(TDAL_APPTESTS_ROOT)/src/integration/cotdalintegration.$(OBJ_SUFFIX) \
                         $(TDAL_APPTESTS_ROOT)/src/integration/tdal_integration_test.$(OBJ_SUFFIX)
	CFLAGS += -DTDAL_INTEGRATION_TEST_INCLUDED
endif

