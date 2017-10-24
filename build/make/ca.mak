ifneq "$(PROGTYPE)" "BOOTLOADER"
ifneq "$(PROGTYPE)" "DOWNLOADER"
ifeq "$(PRODUCT_USE_CA)" "YES"

CAK_CFLAGS =
CAK_INCLUDE_PATH = 
CAK_LIBS_PATH = 
CAK_LIBS =

#########################################################################################
# We're adding the CA stack, add the global product option to the build flags

CAK_CFLAGS += -DPRODUCT_USE_CA

# These are the CCAS this ca.mak supports
SUPPORTED_CCAS = NAGRA_TIGER PC_SIMULATOR NAGRA_MERLIN NAGRA_DALTS CONAX SOFTCELL IRDETO_STUB SOFTCELL_EMBT CRYPTO VIACCESS DRE DUMMY

# Sanitize - note that this will be expanded when executing the makefile
SANITIZED_CCAS := $(findstring $(PRODUCT_CCAS), $(SUPPORTED_CCAS))
$(if $(SANITIZED_CCAS),,$(error "Unsupported product CCAS '$(PRODUCT_CCAS)', should be one of $(SUPPORTED_CCAS)."))

#########################################################################################
# Select the CA system from what this ca.mak supports

ifeq "$(PRODUCT_CCAS)" "NAGRA_TIGER"
	CA_PACK_NAGRA = 1
endif
ifeq "$(PRODUCT_CCAS)" "PC_SIMULATOR"
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

# Include information from CA makefile
include $(CAK_ROOT)/makefile
    
# Add Nagra-related options
#
CAK_CFLAGS += -DFTA_CA_PACK_NAGRA
CAK_CFLAGS += -DNAGRA_MODULE_ID=0x045C0000

#Added NOCS3 support
ifeq "$(USE_NOCS3)" "YES"
CAK_CFLAGS += -DUSE_NOCS3
endif

# Add CCAS Support
ifeq "$(PRODUCT_CCAS_DLK)" "YES"
  CAK_CFLAGS += -DCAK_DLK
  ifeq "$(PRODUCT_CCAS_MODE_DLK)" "SMARTCARD"
    CAK_CFLAGS += -DDLK_MODE_SMARTCARD
  else
    ifeq "$(PRODUCT_CCAS_MODE_DLK)" "CARDLESS"
      CAK_CFLAGS += -DDLK_MODE_CARDLESS
    else
      CAK_CFLAGS += -DDLK_MODE_HYBRID
    endif
  endif
endif

ifeq "$(NASC)" "1"
CAK_CFLAGS += -DNASC
endif

ifeq "$(USE_DLK_1_1)" "YES"
  CAK_CFLAGS += -DUSE_DLK_1_1
  ifneq "$(PRODUCT_FEATURE_PVR)" "PVR_OFF"
   CAK_CFLAGS += -DSTORE_FINGERPRINT_MESSAGES
  endif
endif

ifeq "$(SUPPRESS_FTA)" "YES"
  CAK_CFLAGS += -DSUPPRESS_FTA
endif

ifeq "$(MIGRATION_N_PARTITION)" "YES"
  ifeq "$(PRODUCT_APP)" "CELINE"
  CAK_CFLAGS += -DN_PARTITION_MIGRATION_ADDRESS=0x650000
  CAK_CFLAGS += -DN_CTRL_BLOCK_MIGRATION_ADDRESS=0x6D0000
  endif
  ifeq "$(PRODUCT_APP)" "MAESTRO"
  CAK_CFLAGS += -DN_PARTITION_MIGRATION_ADDRESS=0x560000
  CAK_CFLAGS += -DN_CTRL_BLOCK_MIGRATION_ADDRESS=0x5E0000
  endif
  CAK_CFLAGS += -DMIGRATION_N_PARTITION
endif
# ZONE LOCK Feature
ifeq "$(PRODUCT_CCAS_ZONE_LOCK)" "YES"
   CAK_CFLAGS += -DZONE_LOCK_ENABLE
endif

# Add options and rules for building with the Nagra CAK "Tiger"
#
ifeq "$(PRODUCT_CCAS)" "NAGRA_TIGER"

    # We're compiling for the Nagra CAK "Tiger"
    # TODO: Find the version for "Tiger"
    CAK_CFLAGS += -DCAK_TIGER

    # Add libraries
    #
    ifeq "$(PRODUCT_CAK_LIBVER)" "PRODUCTION"
        CAK_LIBS += $(CAK_PROD_RELEASE_LIBS)
    endif

    ifeq "$(PRODUCT_CAK_LIBVER)" "RELEASE"
        CAK_LIBS += $(CAK_NAK_RELEASE_LIBS)
    endif

    ifeq "$(PRODUCT_CAK_LIBVER)" "DEBUG"
        CAK_LIBS += $(CAK_NAK_DEBUG_LIBS)
    endif

    CAK_LIBS_PATH += $(CAK_ROOT)/CAK_TI/lib
    CAK_INCLUDE_PATH += $(CAK_ROOT)/include
    
endif # PRODUCT_CCAS == NAGRA_TIGER

# Add options and rules for building with PC simulator MTE CA
#
ifeq "$(PRODUCT_CCAS)" "PC_SIMULATOR"

    # We're compiling for the PC Simulator MTE CA
    # TODO: Change that version to a real number to test ( v#x < v#y )
    CAK_CFLAGS += -DCAK_MERLIN -DCAK_V7
    CAK_CFLAGS += -DIWEDIA_MTE_CA
    
    # Add libraries
    #
    ifeq "$(PRODUCT_CAK_LIBVER)" "PRODUCTION"
        CAK_LIBS += $(CAK_PROD_RELEASE_LIBS)
        CAK_CFLAGS += -DCAK_PRODUCTION
    endif

    ifeq "$(PRODUCT_CAK_LIBVER)" "RELEASE"
        CAK_LIBS += $(CAK_NAK_RELEASE_LIBS)
        CAK_CFLAGS += -DCAK_BETA
    endif

    ifeq "$(PRODUCT_CAK_LIBVER)" "DEBUG"
        CAK_LIBS += $(CAK_NAK_DEBUG_LIBS)
    endif

    CAK_LIBS_PATH += $(CAK_ROOT)/CAK_ME/lib
    CAK_INCLUDE_PATH += $(CAK_ROOT)/include
   
    CAK_LIBS += $(LIB_PREFFIX)xml2.$(LIB_SUFFIX)
    CAK_LIBS_PATH += $(LIBXML2_LIB)

endif # PRODUCT_CCAS == PC_SIMULATOR

# Add options and rules for building with the Nagra CAK "Merlin"
#
ifeq "$(PRODUCT_CCAS)" "NAGRA_MERLIN"

    # We're compiling for the Nagra CAK "Merlin"
    # TODO: Change that version to a real number to test ( v#x < v#y )
    CAK_CFLAGS += -DCAK_MERLIN -DCAK_V7

    # Add libraries
    #
    ifeq "$(PRODUCT_CAK_LIBVER)" "PRODUCTION"
        CAK_LIBS += $(CAK_PROD_RELEASE_LIBS)
        CAK_CFLAGS += -DCAK_PRODUCTION
    endif

    ifeq "$(PRODUCT_CAK_LIBVER)" "RELEASE"
        CAK_LIBS += $(CAK_NAK_RELEASE_LIBS)
        CAK_CFLAGS += -DCAK_BETA
    endif

    ifeq "$(PRODUCT_CAK_LIBVER)" "DEBUG"
        CAK_LIBS += $(CAK_NAK_DEBUG_LIBS)
        CAK_CFLAGS += -DCAK_BETA
    endif

    ifeq "$(PRODUCT_CAK_LIBVER)" "CERT_RELEASE"
        CAK_LIBS += $(CAK_NAK_CERT_RELEASE_LIBS)
        CAK_CFLAGS += -DCAK_CERT -DCAK_BETA
    endif

    ifeq "$(PRODUCT_CAK_LIBVER)" "CERT_DEBUG"
        CAK_LIBS += $(CAK_NAK_CERT_DEBUG_LIBS)
        CAK_CFLAGS += -DCAK_CERT -DCAK_BETA
    endif

    CAK_LIBS_PATH += $(CAK_ROOT)/CAK_ME/lib
    CAK_INCLUDE_PATH += $(CAK_ROOT)/include
    
endif # PRODUCT_CCAS == NAGRA_MERLIN

# Add options and rules for building with the Nagra DAL_TS
#
ifeq "$(PRODUCT_CCAS)" "NAGRA_DALTS"

    # We're compiling for the Nagra DAL_TS
    # TODO: Change that version to a real number to test ( v#x < v#y )
    CAK_CFLAGS += -DCAK_DALTS -DCAK_V7

    CAK_LIBS += $(CAK_DALTS_LIBS)

    CAK_LIBS_PATH += $(CAK_ROOT)/DALTS/lib
    CAK_INCLUDE_PATH += $(CAK_ROOT)/include
    
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
    CAK_CFLAGS += -D__TRACE_UART__
  else
    ifneq "$(CAK_DAL_TRACE)" "$(TRACES)"
      $(warning "DAL logging option '$(CAK_DAL_TRACE)' is conflicting with global TRACES option '$(TRACES)'.")
    endif
  endif
endif
ifeq "$(CAK_DAL_TRACE)" "DCU"
  ifeq "$(TRACES)" ""
    CAK_CFLAGS += -D__TRACE_DCU__
  else
    ifneq "$(CAK_DAL_TRACE)" "$(TRACES)"
      $(warning "DAL logging option '$(CAK_DAL_TRACE)' is conflicting with global TRACES option '$(TRACES)'.")
    endif
  endif
endif



# Add broadcast medium option
#
ifeq "$(PRODUCT_MEDIUM)" "MEDIUM_CAB"
  CAK_CFLAGS += -DUSE_MEDIUM_CAB
endif
ifeq "$(PRODUCT_MEDIUM)" "MEDIUM_SAT"
  CAK_CFLAGS += -DUSE_MEDIUM_SAT
endif
ifeq "$(PRODUCT_MEDIUM)" "MEDIUM_TER"
  CAK_CFLAGS += -DUSE_MEDIUM_TER
endif



# Optional: use PIDB to recover PMT tables in the RS API
# MUSTUSE: no easy way to recover the PMT PID of the current service
CAK_CFLAGS += -DUSE_PIDB_FOR_MAPPING_NOTIFICATION

# Optional: use the PSI module in the DAL
# DONTUSE: this module is deprecated
# CAK_CFLAGS += -DCAK_DAL_SUPPORT_PSI
# CAK_DAL_SUPPORT_SIZE = 1

# Optional: use the Return Path module in the DAL
# CAK_CFLAGS += -DCAK_DAL_SUPPORT_RPH
# CAK_DAL_SUPPORT_RPH = 1
# Optional: blank video frame when losing rights
CAK_CFLAGS += -DUSE_BLANKING_ON_CA_ERROR

endif # CA_PACK_NAGRA



#########################################################################################

ifeq "$(CA_PACK_CONAX)" "1"
# CONAX CA SYSTEM

CAK_CFLAGS += -DFTA_CA_PACK_CONAX
CAK_CFLAGS += -DPRODUCT_USE_CA_PIN_CODE_ONLY
CAK_CFLAGS += -DPRODUCT_USE_CA_MATURITY_LEVEL_ONLY
CAK_CFLAGS += -DPRODUCT_USE_TDAL_CRYPT

endif # CA_PACK_CONAX



#########################################################################################

ifeq "$(CA_PACK_CRYPTO)" "1"
# CRYPTOWORKS CA SYSTEM

CAK_CFLAGS += -DFTA_CA_PACK_CRYPTO
CAK_CFLAGS += -DIPPV_NOT_SUPPORTED
CAK_CFLAGS += -DHOST_LANGUAGES_INCLUDED

endif # CA_PACK_CRYPTO



#########################################################################################

ifeq "$(CA_PACK_IRDETO)" "1"
# IRDETO CA SYSTEM

# Add Irdeto-related options
#
CAK_CFLAGS += -DFTA_CA_PACK_IRDETO
CAK_CFLAGS += -DIRDETO_MODULE_ID=0x045C0000



# Add options and rules for building the Softcell stub
#
ifeq "$(PRODUCT_CCAS)" "IRDETO_STUB"

# We're compiling the Softcell stub
CAK_CFLAGS += -DIRDETO_STUB

# CCAS Tests rule
ccas_tests:
	@$(ECHO) compiling CCAS tests - SoftCell Stub ...
	$(MAKE) -C $(IRDETO_TEST_ROOT)

endif # PRODUCT_CCAS == IRDETO_STUB



# Add options and rules for building Softcell with EMBT
#
ifeq "$(PRODUCT_CCAS)" "SOFTCELL_EMBT"

# We're compiling the EMBT along Softcell
CAK_CFLAGS += -DSOFTCELL_EMBT

# CCAS Tests rule
ccas_tests:
	@$(ECHO) compiling CCAS tests - SoftCell+EMBT ...
	$(MAKE) -C $(IRDETO_TEST_ROOT)

endif # PRODUCT_CCAS == SOFTCELL_EMBT



# TODO: Move probes to a separate module in the future
ifeq "$(PRODUCT_PROBES)" "1"
CAK_CFLAGS += -DTBOX_PROBES=1 $(INCLUDE_PREFIX)$(IRDETO_TEST_INCLUDE)
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
  CAK_CFLAGS += -DUSE_MEDIUM_CAB
endif
ifeq "$(PRODUCT_MEDIUM)" "MEDIUM_SAT"
  CAK_CFLAGS += -DUSE_MEDIUM_SAT
endif
ifeq "$(PRODUCT_MEDIUM)" "MEDIUM_TER"
  CAK_CFLAGS += -DUSE_MEDIUM_TER
endif



# Add build options
#
 
# Optional: product list update on smartcard change
CAK_CFLAGS += -DUPDATE_PRODUCT_LIST_ON_SMC_NOTIF

# Optional: forced smartcard nationality notification on smartcard error
CAK_CFLAGS += -DUPDATE_NATIONALITY_ON_SMC_ERROR

# Optional: display banner when a message is unread (incoming or current state)
# DONTUSE: a bug prevents the present event content from being displayed
# CAK_CFLAGS += -DSHOW_BANNER_ON_UNREAD_MESSAGE

# Optional: configure and manage notifications from the copy control driver
CAK_CFLAGS += -DUSE_COPY_CONTROL_NOTIFICATION

# Optional: use the Irdeto trigger
CAK_CFLAGS += -DUSE_SOFTWARE_DOWNLOAD

# Optional: hack into the OSD regions to make the mail/announce icon always visible
CAK_CFLAGS += -DUSE_ICON_HACK

# Optional: use PIDB to recover PMT tables in the RS API
# MUSTUSE: no easy way to recover the PMT PID of the current service
CAK_CFLAGS += -DUSE_PIDB_FOR_MAPPING_NOTIFICATION

# Optional: use the demux virtual driver for acquiring trigger tables
# DONTUSE: not conform to the Irdeto specification
# CAK_CFLAGS += -DUSE_VD_FOR_MPEG_TABLES

# Optional: send a reconnection when cancelling a running trigger search
CAK_CFLAGS += -DUSE_RECONNECT_ON_TRIGGER_ERROR

# Optional: use a filter list for preventing specific errors from being displayed
CAK_CFLAGS += -DUSE_FILTERED_OUT_ERRORS

# Optional: blank video frame when losing rights
CAK_CFLAGS += -DUSE_BLANKING_ON_CA_ERROR

endif



#########################################################################################

ifeq "$(PRODUCT_USE_CI)" "YES"
# CI CA SYSTEM

CAK_CFLAGS += -DPRODUCT_USE_CI
CAK_CFLAGS += -DHOST_LANGUAGES_INCLUDED

endif



#########################################################################################

ifeq "$(CA_PACK_VIACCESS)" "1"
# VIACCESS CA SYSTEM

CAK_CFLAGS += -DFTA_CA_PACK_VIACCESS

endif

ifeq "$(CA_PACK_DRE)" "1"
# DRE CA SYSTEM

CAK_CFLAGS += -DFTA_CA_PACK_DRE
CAK_CFLAGS += -DPRODUCT_USE_CA_PIN_CODE_ONLY
CAK_CFLAGS += -DPRODUCT_USE_CA_MATURITY_LEVEL_ONLY
CAK_CFLAGS += -DPRODUCT_USE_TDAL_CRYPT

endif # CA_PACK_DRE

ifeq "$(CA_PACK_DUMMY)" "1"
# DUMMY CA SYSTEM

CAK_CFLAGS += -DFTA_CA_PACK_DUMMY

endif # CA_PACK_DUMMY

#########################################################################################
# Insert generic options related to all CA systems

ifeq "$(PRODUCT_USE_PPV)" "YES"
CAK_CFLAGS += -DPRODUCT_USE_PPV
endif

ifeq "$(PRODUCT_USE_CA_SECURE_CHIPSET)" "YES"	
CAK_CFLAGS += -DTDAL_DESC_DVB_SECURE_CHIPSET
endif

ifeq "$(PRODUCT_USE_CA_ENCRYPTED_CW)" "YES"
CAK_CFLAGS += -DTDAL_DESC_ENABLE_SECURE_MODE
endif

CFLAGS += $(CAK_CFLAGS)

#########################################################################################

else # PRODUCT_USE_CA == YES

# Nothing special to do
# ccas_tests:

endif # PRODUCT_USE_CA == YES
endif # PROGTYPE != DOWNLOADER
endif # PROGTYPE != BOOTLOADER
