#----------------------------------------------------------------------
#
#           Root Makefile for comedia product
#
#----------------------------------------------------------------------
###################################################
#	set environnement
###################################################
export ROOT_DIR=$(CURDIR)
export BUILD_ROOT=$(ROOT_DIR)/build
export BUILD_MAKE=$(BUILD_ROOT)/make
export BUILD_EXPORTS=$(BUILD_ROOT)/bin
export BUILD_INCLUDE=$(BUILD_ROOT)/inc
export BUILD_EXE=$(BUILD_ROOT)/exe
export BUILD_SET=$(BUILD_ROOT)/set

CONFIGURATIONS=$(sort $(basename $(notdir $(wildcard $(BUILD_SET)/*.mak))))

define NEWLINE


endef

###################################################
#	Route target build
###################################################

.PHONY: flashtool help sdk help_all chal

export TARGET_NAME=$(MAKECMDGOALS)

help:
	$(info --------------------------------------------------------------)
	$(info |                           Help                             |)
	$(info --------------------------------------------------------------)
	$(info                                                               )
	$(info To build DTV software set configuration:                      )
	$(info - On Windows: set SETENV=XXXXXX                               )
	$(info - On Linux:   export SETENV=XXXXXX                            )
	$(info                                                               )
	$(info $(addsuffix $(NEWLINE), Configurations: $(CONFIGURATIONS))    )
	$(info --------------------------------------------------------------)
	$(info                                                               )
	$(info Common targets:                                               )
	$(info make flashtool     - Builds flash tool                        )
	$(info make resources     - Builds application resources             )
	$(info make sdk           - Builds platform SDK                      )
	$(info make chal          - Builds HAL                               )
	$(info make chalvalidator - Builds HAL validator                     )
	$(info make comedia       - Builds DTV software                      )
	$(info make dist          - Builds DTV distribuition                 )
	$(info make dist_sdk      - Builds DTV SDK distribuition             )
	$(info                                                               )
	$(info For more help run:                                            )
	$(info make help_all                                                 )
ifneq ("$(SETENV)","")
	$(info --------------------------------------------------------------)
	$(info Configuration : $(SETENV)                                     )
	$(info --------------------------------------------------------------)
endif

check_configuration:
	$(info )
ifeq ("$(wildcard $(BUILD_SET)/$(SETENV).mak)","")
	$(info Please check Help with command: make help)
	$(info Configuration is not set properly!       )
	$(error )
endif

help_all: check_configuration help
	@$(MAKE) -kC $(BUILD_ROOT) SETENV=$(SETENV) help -s

chal sdk:
	$(MAKE) -C $(BUILD_ROOT) SETENV=$(SETENV) $@
        
flashtool:
	$(MAKE) -kC $(ROOT_DIR)/tools/flashtool clean
	$(MAKE) -C  $(ROOT_DIR)/tools/flashtool all
	
tests:
	$(MAKE) -C  $(ROOT_DIR)/test/MTE all

dist dist_sdk: check_configuration flashtool
	$(MAKE) -C $(BUILD_ROOT) SETENV=$(SETENV) $@

%.out %.exe: check_configuration
	$(MAKE) -C $(BUILD_ROOT) SETENV=$(SETENV) $@

%:
ifeq ($(MAKELEVEL), 1)
	@$(MAKE) -C $(ROOT_DIR) SETENV=$(SETENV) check_configuration -s
endif
	$(MAKE) -C $(BUILD_ROOT) SETENV=$(SETENV) $@