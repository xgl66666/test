# -------------------------------------------------
# By default, the product use the RC NEC_DTTRC3:
# To use the RC NEC_DTTRC3 set "KBD_ST_NEC_DTTRC3"
# To use the RC black "ST" unset "KBD_ST_NEC_DTTRC3"
# -------------------------------------------------

#KBD_SMARTDTV=1

ifeq "$(PRODUCT_MULTI_RCUS)" "1"
	MULTI_RCUS=1
endif

ifeq "$(PRODUCT_KBD)" "PC_KBD"
KBD_PC=1
else
ifeq "$(PRODUCT_KBD)" "KBD_SKYWORTH"
KBD_SKYWORTH=1
else 
ifeq "$(PRODUCT_KBD)" "NEWLAND_NEC_RC"
KBD_NEWLAND=1
else 
ifeq "$(PRODUCT_KBD)" "GTPL_DTV"
KBD_GTPL=1
else 
ifeq "$(PRODUCT_KBD)" "GILHITS"
KBD_GHITS=1
else
KBD_MSTAR=1
endif
endif
endif
endif
endif 

#KBD_ST_NEC_DTTRC3=1
#KBD_BLKFORCE_PROT=1
ifeq "$(KBD_PC)" "1"
CFLAGS += -DKBD_PC
else
ifeq "$(KBD_SKYWORTH)" "1"
CFLAGS += -DKBD_DEN_SKYWORTH
else 
ifeq "$(KBD_ST_NEC_DTTRC3)" "1"
CFLAGS += -DKBD_ST_NEC_DTTRC3
else 
ifeq "$(KBD_BLKFORCE_PROT)" "1"
CFLAGS += -DKBD_BLKFORCE_PROT
else 
ifeq "$(KBD_SMARTDTV)" "1"
CFLAGS += -DKBD_SMARDTV
else 
ifeq "$(KBD_MSTAR)" "1"
CFLAGS += -DKBD_MSTAR
else 
ifeq "$(KBD_GTPL)" "1"
CFLAGS += -DKBD_GTPL
else 
ifeq "$(KBD_GHITS)" "1"
CFLAGS += -DKBD_GHITS
else 
ifeq "$(KBD_NEWLAND)" "1"
CFLAGS += -DKBD_NEWLAND
else
ifeq "$(PRODUCT_MAESTRO_RCU_FINE_TUNING)" "YES"
CFLAGS += -DMAESTRO_RCU_FINE_TUNING
endif
endif
endif
endif
endif
endif
endif
endif
endif
endif

