ifneq "$(PROGTYPE)" "BOOTLOADER"
ifneq "$(PROGTYPE)" "DOWNLOADER"
ifeq "$(PRODUCT_USE_CA)" "YES"


#########################################################################################
# We're adding the CA stack, add the global product option to the build flags

CFLAGS += -DPRODUCT_USE_CA

# These are the CCAS this ca.mak supports
SUPPORTED_CCAS = NAGRA_TIGER NAGRA_MERLIN NAGRA_DALTS CONAX SOFTCELL IRDETO_STUB SOFTCELL_EMBT CRYPTO VIACCESS DRE DUMMY

# Sanitize - note that this will be expanded when executing the makefile
SANITIZED_CCAS := $(findstring $(PRODUCT_CCAS), $(SUPPORTED_CCAS))
$(if $(SANITIZED_CCAS),,$(error "Unsupported product CCAS '$(PRODUCT_CCAS)', should be one of $(SUPPORTED_CCAS)."))

CAK_INCLUDE_PATH = 
CAK_LIBS_PATH = 
CAK_LIBS =

#########################################################################################
# Select the CA system from what this ca.mak supports

ifeq "$(PRODUCT_CCAS)" "NAGRA_TIGER"
	CA_PACK_NAGRA = 1
endif
ifeq "$(PRODUCT_CCAS)" "NAGRA_MERLIN"
	CA_PACK_NAGRA = 1
endif
ifeq "$(PRODUCT_CCAS)" "NAGRA_DALTS"
	CA_PACK_NAGRA = 1
endif
ifeq "$(PRODUCT_CCAS)" "SOFTCELL"
	CA_PACK_IRDETO = 1
endif
ifeq "$(PRODUCT_CCAS)" "IRDETO_STUB"
	CA_PACK_IRDETO = 1
endif
ifeq "$(PRODUCT_CCAS)" "SOFTCELL_EMBT"
	CA_PACK_IRDETO = 1
endif
ifeq "$(PRODUCT_CCAS)" "CRYPTO"
	CA_PACK_CRYPTO = 1
endif
ifeq "$(PRODUCT_CCAS)" "CONAX"
	CA_PACK_CONAX = 1
endif
ifeq "$(PRODUCT_CCAS)" "VIACCESS"
	CA_PACK_VIACCESS = 1
endif

ifeq "$(PRODUCT_CCAS)" "DRE"
	CA_PACK_DRE = 1
endif

ifeq "$(PRODUCT_CCAS)" "DUMMY"
	CA_PACK_DUMMY = 1
endif


#########################################################################################

# Configure the various CA systems based on the ccas

#########################################################################################

ifeq "$(CA_PACK_NAGRA)" "1"
# NAGRA CA SYSTEM

# Add Nagra-related options
#
CFLAGS += -DFTA_CA_PACK_NAGRA
CFLAGS += -DNAGRA_MODULE_ID=0x045C0000

#Added NOCS3 support
ifeq "$(USE_NOCS3)" "YES"
CFLAGS += -DUSE_NOCS3
endif

# Add CCAS Support
ifeq "$(PRODUCT_CCAS_DLK)" "YES"
  CFLAGS += -DCAK_DLK
  ifeq "$(PRODUCT_CCAS_MODE_DLK)" "SMARTCARD"
    CFLAGS += -DDLK_MODE_SMARTCARD
  else
    ifeq "$(PRODUCT_CCAS_MODE_DLK)" "CARDLESS"
      CFLAGS += -DDLK_MODE_CARDLESS
    else
      CFLAGS += -DDLK_MODE_HYBRID
    endif
  endif
endif

ifeq "$(NASC)" "1"
CFLAGS += -DNASC
endif
ifeq "$(USE_DLK_1_1)" "YES"
  CFLAGS += -DUSE_DLK_1_1
  ifneq "$(PRODUCT_FEATURE_PVR)" "PVR_OFF"
   CFLAGS += -DSTORE_FINGERPRINT_MESSAGES
  endif
endif

ifeq "$(SUPPRESS_FTA)" "YES"
  CFLAGS += -DSUPPRESS_FTA
endif

ifeq "$(MIGRATION_N_PARTITION)" "YES"
  ifeq "$(PRODUCT_APP)" "CELINE"
  CFLAGS += -DN_PARTITION_MIGRATION_ADDRESS=0x650000
  CFLAGS += -DN_CTRL_BLOCK_MIGRATION_ADDRESS=0x6D0000
  endif
  ifeq "$(PRODUCT_APP)" "MAESTRO"
  CFLAGS += -DN_PARTITION_MIGRATION_ADDRESS=0x560000
  CFLAGS += -DN_CTRL_BLOCK_MIGRATION_ADDRESS=0x5E0000
  endif
  CFLAGS += -DMIGRATION_N_PARTITION
endif
# ZONE LOCK Feature
ifeq "$(PRODUCT_CCAS_ZONE_LOCK)" "YES"
   CFLAGS += -DZONE_LOCK_ENABLE
endif

# Add options and rules for building with the Nagra CAK "Tiger"
#
ifeq "$(PRODUCT_CCAS)" "NAGRA_TIGER"

# We're compiling for the Nagra CAK "Tiger"
# TODO: Find the version for "Tiger"
CFLAGS += -DCAK_TIGER

# Add libraries
#
PRODUCTLIB += $(CAK_ROOT)/lib/cak-sec.release.st20cc.$(LIB_SUFFIX) $(LIB_PREFFIX)cak_dal.$(LIB_SUFFIX)
PRODUCTLIB += $(DRIVERS_LIB)/stnagracsd_REL_1.0.1.lib $(DRIVERS_LIB)/stcryptoman_REL_2.0.4.lib $(DRIVERS_LIB)/stsectoolfuse_REL_1.4.2.lib $(DRIVERS_LIB)/stsectoolnuid_REL_3.1.0.lib
PRODUCTLIB_FULL += $(BUILD_EXPORTS)/cak-sec.release.st20cc.$(LIB_SUFFIX) $(BUILD_EXPORTS)/$(LIB_PREFFIX)cak_dal.$(LIB_SUFFIX)

endif # PRODUCT_CCAS == NAGRA_TIGER



# Add options and rules for building with the Nagra CAK "Merlin"
#
ifeq "$(PRODUCT_CCAS)" "NAGRA_MERLIN"

# We're compiling for the Nagra CAK "Merlin"
# TODO: Change that version to a real number to test ( v#x < v#y )
CFLAGS += -DCAK_MERLIN -DCAK_V7

#
# WARNING: When linking with libraries, make sure the Pairing Keys are
# relevant, located in build/src/ca_cfg/nagra_cfg/nagra_cfg.c.
#

# Add CAK libraries
#

ifeq "$(PLATFORM_CHIP_VENDOR)" "mstar"
    CAKLIB_TOOLCHAIN=mipsisa32-elf-gcc
endif

ifeq "$(PLATFORM_CHIP_VENDOR)" "st"
    CAKLIB_TOOLCHAIN=sh4gcc
endif

ifeq "$(PRODUCT_CAK_LIBVER)" "PRODUCTION"
ifeq "$(PRODUCT_CUSTOMER_HEADEND)" "GHITS"
    CAKLIB_SUFFIX=IME
endif

ifeq "$(PRODUCT_CUSTOMER_HEADEND)" "VSM"
    CAKLIB_SUFFIX=IME
endif

ifeq "$(PRODUCT_CUSTOMER_HEADEND)" "GTPL"
    CAKLIB_SUFFIX=DG2
endif
ifeq "$(PRODUCT_CUSTOMER_HEADEND)" "DEN"
    CAKLIB_SUFFIX=DEN
endif
endif
endif # PRODUCT_CCAS == NAGRA_MERLIN



# Add options and rules for building with the Nagra DAL_TS
#
ifeq "$(PRODUCT_CCAS)" "NAGRA_DALTS"

# We're compiling for the Nagra DAL_TS
# TODO: Change that version to a real number to test ( v#x < v#y )
CFLAGS += -DCAK_DALTS -DCAK_V7

endif # PRODUCT_CCAS == NAGRA_DALTS



# Add logging option for DAL
#
# Sanitize - note that this will be expanded when executing the makefile
SUPPORTED_DAL_TRACE = UART DCU OFF
SANITIZED_DAL_TRACE := $(findstring $(CAK_DAL_TRACE), $(SUPPORTED_DAL_TRACE))
$(if $(SANITIZED_DAL_TRACE),,$(error "Unsupported DAL logging option '$(CAK_DAL_TRACE)', should be one of $(SUPPORTED_DAL_TRACE)."))
# Add option
ifeq "$(CAK_DAL_TRACE)" "UART"
  ifeq "$(TRACES)" ""
    CFLAGS += -D__TRACE_UART__
  else
    ifneq "$(CAK_DAL_TRACE)" "$(TRACES)"
      $(warning "DAL logging option '$(CAK_DAL_TRACE)' is conflicting with global TRACES option '$(TRACES)'.")
    endif
  endif
endif
ifeq "$(CAK_DAL_TRACE)" "DCU"
  ifeq "$(TRACES)" ""
    CFLAGS += -D__TRACE_DCU__
  else
    ifneq "$(CAK_DAL_TRACE)" "$(TRACES)"
      $(warning "DAL logging option '$(CAK_DAL_TRACE)' is conflicting with global TRACES option '$(TRACES)'.")
    endif
  endif
endif



# Add broadcast medium option
#
ifeq "$(PRODUCT_MEDIUM)" "MEDIUM_CAB"
  CFLAGS += -DUSE_MEDIUM_CAB
endif
ifeq "$(PRODUCT_MEDIUM)" "MEDIUM_SAT"
  CFLAGS += -DUSE_MEDIUM_SAT
endif
ifeq "$(PRODUCT_MEDIUM)" "MEDIUM_TER"
  CFLAGS += -DUSE_MEDIUM_TER
endif



# Optional: use PIDB to recover PMT tables in the RS API
# MUSTUSE: no easy way to recover the PMT PID of the current service
CFLAGS += -DUSE_PIDB_FOR_MAPPING_NOTIFICATION

# Optional: use the PSI module in the DAL
# DONTUSE: this module is deprecated
# CFLAGS += -DCAK_DAL_SUPPORT_PSI
# CAK_DAL_SUPPORT_SIZE = 1

# Optional: use the Return Path module in the DAL
# CFLAGS += -DCAK_DAL_SUPPORT_RPH
# CAK_DAL_SUPPORT_RPH = 1
# Optional: blank video frame when losing rights
CFLAGS += -DUSE_BLANKING_ON_CA_ERROR


ifeq "$(PRODUCT_CCAS)" "NAGRA_MERLIN"
CAK_LIBS_PATH+=$(CAK_ROOT)/CAK_ME/lib
endif
ifeq "$(PRODUCT_CCAS)" "NAGRA_DALTS"
CAK_LIBS_PATH+=$(CAK_ROOT)/DALTS/lib
endif

ifeq "$(PRODUCT_USE_CA)" "YES"
ifeq "$(PRODUCT_CCAS)" "NAGRA_MERLIN"
ifeq "$(CAK_LIBDIR)" "Mstar_5C59"

ifeq "$(PRODUCT_CAK_LIBVER)" "PRODUCTION"
CAK_LIBS += \
	$(LIB_PREFFIX)cak_icme.std_nocert.release.mipsisa32-elf-gcc.$(LIB_SUFFIX) \
	$(LIB_PREFFIX)cak_icme.std_nocert.release.mipsisa32-elf-gcc-DEN.$(LIB_SUFFIX)
else
ifeq "$(PRODUCT_CAK_LIBVER)" "RELEASE"
CAK_LIBS += \
	$(LIB_PREFFIX)cak_icme.std_cert.release.mipsisa32-elf-gcc.$(LIB_SUFFIX) \
	$(LIB_PREFFIX)cak_icme.std_cert.release.mipsisa32-elf-gcc-NAK.$(LIB_SUFFIX)
else
CAK_LIBS += \
	$(LIB_PREFFIX)cak_icme.std_cert.debug.mipsisa32-elf-gcc.$(LIB_SUFFIX) \
	$(LIB_PREFFIX)cak_icme.std_cert.debug.mipsisa32-elf-gcc-NAK.$(LIB_SUFFIX)
endif
endif

else
ifeq "$(PRODUCT_CAK_LIBVER)" "PRODUCTION"
ifeq "$(PRODUCT_CUSTOMER_HEADEND)" "DEN"
CAK_LIBS += \
	$(LIB_PREFFIX)cak_icme.production.release.$(CAKLIB_TOOLCHAIN).$(LIB_SUFFIX) \
	$(LIB_PREFFIX)cak_icme.production.release.$(CAKLIB_TOOLCHAIN)-$(CAKLIB_SUFFIX).$(LIB_SUFFIX)
else
CAK_LIBS += \
	$(LIB_PREFFIX)cak_icme.standard.release.$(CAKLIB_TOOLCHAIN).$(LIB_SUFFIX) \
	$(LIB_PREFFIX)cak_icme.standard.release.$(CAKLIB_TOOLCHAIN)-$(CAKLIB_SUFFIX).$(LIB_SUFFIX)
endif    
else
ifeq "$(PRODUCT_CAK_LIBVER)" "RELEASE"
CAK_LIBS += \
	$(LIB_PREFFIX)cak_icme.standard.release.$(CAKLIB_TOOLCHAIN).$(LIB_SUFFIX) \
	$(LIB_PREFFIX)cak_icme.standard.release.$(CAKLIB_TOOLCHAIN)-NAK.$(LIB_SUFFIX)
else
CAK_LIBS += \
	$(LIB_PREFFIX)cak_icme.standard.debug.$(CAKLIB_TOOLCHAIN).$(LIB_SUFFIX) \
	$(LIB_PREFFIX)cak_icme.standard.debug.$(CAKLIB_TOOLCHAIN)-NAK.$(LIB_SUFFIX)
endif
endif
endif
endif
endif

ifeq "$(PRODUCT_CCAS)" "NAGRA_DALTS"
ifeq "$(CAK_LIBDIR)" "Mstar_5C59"
CAK_LIBS += $(LIB_PREFFIX)dalts.cak7-dlk-hybrid.release.mipsisa32-elf-gcc.$(LIB_SUFFIX)
else
CAK_LIBS += $(LIB_PREFFIX)dalts.cak7-dlk.release.mipsisa32-elf-gcc.$(LIB_SUFFIX)
endif
endif

# Print DALTS/CAK library name
$(info [33mCAK_LIBS: [31m$(CAK_LIBS)[0m)

endif # CA_PACK_NAGRA



#########################################################################################

ifeq "$(CA_PACK_CONAX)" "1"
# CONAX CA SYSTEM

CFLAGS += -DFTA_CA_PACK_CONAX
CFLAGS += -DPRODUCT_USE_CA_PIN_CODE_ONLY
CFLAGS += -DPRODUCT_USE_CA_MATURITY_LEVEL_ONLY
CFLAGS += -DPRODUCT_USE_TDAL_CRYPT

endif # CA_PACK_CONAX



#########################################################################################

ifeq "$(CA_PACK_CRYPTO)" "1"
# CRYPTOWORKS CA SYSTEM

CFLAGS += -DFTA_CA_PACK_CRYPTO
CFLAGS += -DIPPV_NOT_SUPPORTED
CFLAGS += -DHOST_LANGUAGES_INCLUDED

endif # CA_PACK_CRYPTO



#########################################################################################

ifeq "$(CA_PACK_IRDETO)" "1"
# IRDETO CA SYSTEM

# Add Irdeto-related options
#
CFLAGS += -DFTA_CA_PACK_IRDETO
CFLAGS += -DIRDETO_MODULE_ID=0x045C0000



# Add options and rules for building the Softcell stub
#
ifeq "$(PRODUCT_CCAS)" "IRDETO_STUB"

# We're compiling the Softcell stub
CFLAGS += -DIRDETO_STUB

# CCAS Tests rule
ccas_tests:
	@$(ECHO) compiling CCAS tests - SoftCell Stub ...
	$(MAKE) -C $(IRDETO_TEST_ROOT)

endif # PRODUCT_CCAS == IRDETO_STUB



# Add options and rules for building Softcell with EMBT
#
ifeq "$(PRODUCT_CCAS)" "SOFTCELL_EMBT"

# We're compiling the EMBT along Softcell
CFLAGS += -DSOFTCELL_EMBT

# CCAS Tests rule
ccas_tests:
	@$(ECHO) compiling CCAS tests - SoftCell+EMBT ...
	$(MAKE) -C $(IRDETO_TEST_ROOT)

endif # PRODUCT_CCAS == SOFTCELL_EMBT



# TODO: Move probes to a separate module in the future
ifeq "$(PRODUCT_PROBES)" "1"
CFLAGS += -DTBOX_PROBES=1 $(INCLUDE_PREFIX)$(IRDETO_TEST_INCLUDE)
# PRODUCTLIB += $(LIB_PREFFIX)irdeto_test.$(LIB_SUFFIX)
endif



# Add libraries
#
PRODUCTLIB+=$(LIB_PREFFIX)s3_dvb_client_3.6.28.$(LIB_SUFFIX)
PRODUCTLIB+=$(LIB_PREFFIX)vd.$(LIB_SUFFIX)
PRODUCTLIB_FULL+=$(BUILD_EXPORTS)/$(LIB_PREFFIX)s3_dvb_client_3.6.28.$(LIB_SUFFIX)
PRODUCTLIB_FULL+=$(BUILD_EXPORTS)/$(LIB_PREFFIX)vd.$(LIB_SUFFIX)



# Add broadcast medium option
#
ifeq "$(PRODUCT_MEDIUM)" "MEDIUM_CAB"
  CFLAGS += -DUSE_MEDIUM_CAB
endif
ifeq "$(PRODUCT_MEDIUM)" "MEDIUM_SAT"
  CFLAGS += -DUSE_MEDIUM_SAT
endif
ifeq "$(PRODUCT_MEDIUM)" "MEDIUM_TER"
  CFLAGS += -DUSE_MEDIUM_TER
endif



# Add build options
#
 
# Optional: product list update on smartcard change
CFLAGS += -DUPDATE_PRODUCT_LIST_ON_SMC_NOTIF

# Optional: forced smartcard nationality notification on smartcard error
CFLAGS += -DUPDATE_NATIONALITY_ON_SMC_ERROR

# Optional: display banner when a message is unread (incoming or current state)
# DONTUSE: a bug prevents the present event content from being displayed
# CFLAGS += -DSHOW_BANNER_ON_UNREAD_MESSAGE

# Optional: configure and manage notifications from the copy control driver
CFLAGS += -DUSE_COPY_CONTROL_NOTIFICATION

# Optional: use the Irdeto trigger
CFLAGS += -DUSE_SOFTWARE_DOWNLOAD

# Optional: hack into the OSD regions to make the mail/announce icon always visible
CFLAGS += -DUSE_ICON_HACK

# Optional: use PIDB to recover PMT tables in the RS API
# MUSTUSE: no easy way to recover the PMT PID of the current service
CFLAGS += -DUSE_PIDB_FOR_MAPPING_NOTIFICATION

# Optional: use the demux virtual driver for acquiring trigger tables
# DONTUSE: not conform to the Irdeto specification
# CFLAGS += -DUSE_VD_FOR_MPEG_TABLES

# Optional: send a reconnection when cancelling a running trigger search
CFLAGS += -DUSE_RECONNECT_ON_TRIGGER_ERROR

# Optional: use a filter list for preventing specific errors from being displayed
CFLAGS += -DUSE_FILTERED_OUT_ERRORS

# Optional: blank video frame when losing rights
CFLAGS += -DUSE_BLANKING_ON_CA_ERROR

endif



#########################################################################################

ifeq "$(PRODUCT_USE_CI)" "YES"
# CI CA SYSTEM

CFLAGS += -DPRODUCT_USE_CI
CFLAGS += -DHOST_LANGUAGES_INCLUDED

endif



#########################################################################################

ifeq "$(CA_PACK_VIACCESS)" "1"
# VIACCESS CA SYSTEM

CFLAGS += -DFTA_CA_PACK_VIACCESS

endif

ifeq "$(CA_PACK_DRE)" "1"
# DRE CA SYSTEM

CFLAGS += -DFTA_CA_PACK_DRE
CFLAGS += -DPRODUCT_USE_CA_PIN_CODE_ONLY
CFLAGS += -DPRODUCT_USE_CA_MATURITY_LEVEL_ONLY
CFLAGS += -DPRODUCT_USE_TDAL_CRYPT

endif # CA_PACK_DRE

ifeq "$(CA_PACK_DUMMY)" "1"
# DUMMY CA SYSTEM

CFLAGS += -DFTA_CA_PACK_DUMMY

endif # CA_PACK_DUMMY

#########################################################################################
# Insert generic options related to all CA systems

ifeq "$(PRODUCT_USE_PPV)" "YES"
CFLAGS += -DPRODUCT_USE_PPV
endif

ifeq "$(PRODUCT_USE_CA_SECURE_CHIPSET)" "YES"	
CFLAGS += -DTDAL_DESC_DVB_SECURE_CHIPSET
endif

ifeq "$(PRODUCT_USE_CA_ENCRYPTED_CW)" "YES"
CFLAGS += -DTDAL_DESC_ENABLE_SECURE_MODE
endif



#########################################################################################

else # PRODUCT_USE_CA == YES

# Nothing special to do
# ccas_tests:

endif # PRODUCT_USE_CA == YES
endif # PROGTYPE != DOWNLOADER
endif # PROGTYPE != BOOTLOADER
