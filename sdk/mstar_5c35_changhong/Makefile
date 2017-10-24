
# *************************************************
#    TOOLCHAIN PATH
# *************************************************

export PATH:=/tools/mstar/mipsisa32-elf-3.4.4/bin/:/opt/toolchains/mips-4.3-154/bin:$(PATH)

# *************************************************
#    BUILD PARAMETERS
# *************************************************

ifeq ($(SDK_ROOT),)
SDK_ROOT:=.
endif

include $(SDK_ROOT)/tools.mak

BUILD_FOLDER:=$(SDK_ROOT)/src/Keltic/DDI_Package/ddi_pkg_ecos/cus_mstar/project/mstar_demo/

all: build_sdk prepare_libs
	
prepare_libs:
#   make precious libsdk.a after all components are ready
	$(AR) $(AR_REBUILD_OPTIONS) $(SDK_ROOT)/lib/$(LIB_PREFFIX)sdk_deps.$(LIB_SUFFIX) $(SDK_OBJS)
    
build_sdk:
#   call build from new shell to have clear environment
	make -kC $(BUILD_FOLDER) -s

clean:
	make -kC $(BUILD_FOLDER) clean

extract:
	@$(ECHO) --- EXTRACT $(MODULE) $(notdir $(LIBOBJS)) ---
	$(foreach LIBR, $(LIBOBJS), $(shell $(AR) $(AR_EXTRACT_OPTIONS) $(LIBR)))

rebuild:
	@$(ECHO) ---- REBUILD ---- 
	$(AR) $(AR_REBUILD_OPTIONS) $(LIB_PREFFIX)$(MODULE).$(LIB_SUFFIX) *.$(OBJ_SUFFIX)
	$(CP) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(LIB_PREFFIX)$(MODULE).$(LIB_SUFFIX)) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_EXPORTS))
	$(RM) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(LIB_PREFFIX)$(MODULE).$(LIB_SUFFIX))
	$(RM) *.$(OBJ_SUFFIX)

image:
	$(MAKE) -ikC $(subst $(BAD_SLASH),$(GOOD_SLASH),$(SDK_ROOT)/tools/genimage)

SDK_OBJS += \
$(BUILD_FOLDER)/obj/drvDemodKelticDVBC.$(OBJ_SUFFIX) \
$(BUILD_FOLDER)/obj/drvDemodNull.$(OBJ_SUFFIX) \
$(BUILD_FOLDER)/obj/drvDemodKelticDVBS.$(OBJ_SUFFIX) \
$(BUILD_FOLDER)/obj/drvDemodKelticATSC.$(OBJ_SUFFIX) \
$(BUILD_FOLDER)/obj/drvTuner_R840.$(OBJ_SUFFIX) \
$(BUILD_FOLDER)/obj/drvTunerNull.$(OBJ_SUFFIX) \
$(BUILD_FOLDER)/obj/MxL608_OEM_Drv.$(OBJ_SUFFIX) \
$(BUILD_FOLDER)/obj/MxL608_TunerApi.$(OBJ_SUFFIX) \
$(BUILD_FOLDER)/obj/MxL608_TunerCfg.$(OBJ_SUFFIX) \
$(BUILD_FOLDER)/obj/drvDishA8296.$(OBJ_SUFFIX) \
$(BUILD_FOLDER)/obj/drvTuner_TDA18260.$(OBJ_SUFFIX) \
$(BUILD_FOLDER)/obj/device_demodulator_msb1236c.$(OBJ_SUFFIX) \
$(BUILD_FOLDER)/obj/device_demodulator_msb131x.$(OBJ_SUFFIX) \
$(BUILD_FOLDER)/obj/MxL603_TunerApi.$(OBJ_SUFFIX) \
$(BUILD_FOLDER)/obj/MxL603_TunerCfg.$(OBJ_SUFFIX) \
$(BUILD_FOLDER)/obj/MxL603_OEM_Drv.$(OBJ_SUFFIX) \
$(BUILD_FOLDER)/obj/tmbslTDA18260.$(OBJ_SUFFIX) \
$(BUILD_FOLDER)/obj/tmbslTDA18260_Instance.$(OBJ_SUFFIX) \
$(BUILD_FOLDER)/obj/tmddTDA18260Shared.$(OBJ_SUFFIX) \
$(BUILD_FOLDER)/obj/tmddTDA18260Stream.$(OBJ_SUFFIX) \
$(BUILD_FOLDER)/obj/tmddTDA18260Stream_Instance.$(OBJ_SUFFIX) \
$(BUILD_FOLDER)/obj/tmddTDA18260Shared_Instance.$(OBJ_SUFFIX) \
$(BUILD_FOLDER)/obj/tmddTDA18260Stream_RegAccess.$(OBJ_SUFFIX) \
$(BUILD_FOLDER)/obj/tmddTDA18260Shared_RegAccess.$(OBJ_SUFFIX) \
$(BUILD_FOLDER)/obj/drvTuner_MXL603.$(OBJ_SUFFIX) \
$(BUILD_FOLDER)/obj/drvTuner_TDA18250A.$(OBJ_SUFFIX) \
$(BUILD_FOLDER)/obj/drvTuner_TDA18250HN.$(OBJ_SUFFIX) \
$(BUILD_FOLDER)/obj/drvTuner_AV2012.$(OBJ_SUFFIX) \
$(BUILD_FOLDER)/obj/tmbslTDA18250AdvanceCustom.$(OBJ_SUFFIX) \
$(BUILD_FOLDER)/obj/tmbslTDA18250.$(OBJ_SUFFIX) \
$(BUILD_FOLDER)/obj/tmbslTDA18250Instance.$(OBJ_SUFFIX) \
$(BUILD_FOLDER)/obj/System_Lo_Detuning.$(OBJ_SUFFIX) \
$(BUILD_FOLDER)/obj/System_Manage_HPMTO.$(OBJ_SUFFIX) \
$(BUILD_FOLDER)/obj/tmddTDA18250.$(OBJ_SUFFIX) \
$(BUILD_FOLDER)/obj/tmddTDA18250Instance.$(OBJ_SUFFIX) \
$(BUILD_FOLDER)/obj/apiDigiTuner.$(OBJ_SUFFIX) \
$(BUILD_FOLDER)/obj/SysInit.$(OBJ_SUFFIX) \
$(BUILD_FOLDER)/obj/drvIIC.$(OBJ_SUFFIX) \
$(BUILD_FOLDER)/obj/drvPQ.$(OBJ_SUFFIX) \
$(BUILD_FOLDER)/obj/mhal_pq.$(OBJ_SUFFIX) \
$(BUILD_FOLDER)/obj/QualityMap_MainEx.$(OBJ_SUFFIX) \
$(BUILD_FOLDER)/obj/QualityMap_Main.$(OBJ_SUFFIX) \
$(BUILD_FOLDER)/obj/drvbw.$(OBJ_SUFFIX) \
$(BUILD_FOLDER)/obj/mdrv_dac_tbl.$(OBJ_SUFFIX) \
$(BUILD_FOLDER)/obj/tmbslTDA18250AdvanceCustom.$(OBJ_SUFFIX) \
$(BUILD_FOLDER)/obj/tmbslTDA18250.$(OBJ_SUFFIX) \
$(BUILD_FOLDER)/obj/tmbslTDA18250Instance.$(OBJ_SUFFIX) \
$(BUILD_FOLDER)/obj/tmddTDA18250.$(OBJ_SUFFIX) \
$(BUILD_FOLDER)/obj/tmddTDA18250Instance.$(OBJ_SUFFIX) \
$(BUILD_FOLDER)/obj/tmbslTDA18250A_Advanced.$(OBJ_SUFFIX) \
$(BUILD_FOLDER)/obj/tmbslTDA18250A.$(OBJ_SUFFIX) \
$(BUILD_FOLDER)/obj/tmbslTDA182I5a.$(OBJ_SUFFIX) \
$(BUILD_FOLDER)/obj/tmbslTDA182I5a_Instance.$(OBJ_SUFFIX) \
$(BUILD_FOLDER)/obj/tmbslTDA182I5a_RegDef.$(OBJ_SUFFIX) \
$(BUILD_FOLDER)/obj/drvTuner_MXL608.$(OBJ_SUFFIX) \
$(BUILD_FOLDER)/obj/drvTunerNull.$(OBJ_SUFFIX) \
$(BUILD_FOLDER)/obj/MxL608_OEM_Drv.$(OBJ_SUFFIX) \
$(BUILD_FOLDER)/obj/MxL608_TunerApi.$(OBJ_SUFFIX) \
$(BUILD_FOLDER)/obj/MxL608_TunerCfg.$(OBJ_SUFFIX) \
$(BUILD_FOLDER)/obj/MxL608_TunerSpurTable.$(OBJ_SUFFIX) \
$(BUILD_FOLDER)/obj/msAPI_Scaler_Adaptive.$(OBJ_SUFFIX) 
