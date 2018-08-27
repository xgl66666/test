
DDI_MISC_SRCDIR += \
            $(DDI_MISC_CUS_DIR)/api/frontend_SiLabs                                      \
            $(DDI_MISC_CUS_DIR)/api/frontend_SiLabs/SILABS_SUPERSET/Sources/Si2183         \
            $(DDI_MISC_CUS_DIR)/api/frontend_SiLabs/SILABS_SUPERSET/Sources/Si_I2C
            
DDI_MISC_INCDIR += \
           $(DDI_MISC_CUS_DIR)/api/frontend_SiLabs                                      \
           $(DDI_MISC_CUS_DIR)/api/frontend_SiLabs/SILABS_SUPERSET/Headers/SAT             \
           $(DDI_MISC_CUS_DIR)/api/frontend_SiLabs/SILABS_SUPERSET/Headers/TER             \
           $(DDI_MISC_CUS_DIR)/api/frontend_SiLabs/SILABS_SUPERSET/Headers/TER/Si2141      \
           $(DDI_MISC_CUS_DIR)/api/frontend_SiLabs/SILABS_SUPERSET/Headers/SAT/A8304       \
           $(DDI_MISC_CUS_DIR)/api/frontend_SiLabs/SILABS_SUPERSET/Headers/SAT/AV2018      \
           $(DDI_MISC_CUS_DIR)/api/frontend_SiLabs/SILABS_SUPERSET/Headers/Si2183          \
           $(DDI_MISC_CUS_DIR)/api/frontend_SiLabs/SILABS_SUPERSET/Headers/Si_I2C 
           
ifeq ($(SUPPORT_SAT), TRUE)
           DDI_MISC_SRCDIR += \
               $(DDI_MISC_CUS_DIR)/api/frontend_SiLabs/SILABS_SUPERSET/Sources/SAT           \
               $(DDI_MISC_CUS_DIR)/api/frontend_SiLabs/SILABS_SUPERSET/Sources/SAT/A8304     \
               $(DDI_MISC_CUS_DIR)/api/frontend_SiLabs/SILABS_SUPERSET/Sources/SAT/AV2018
endif

ifeq ($(SUPPORT_TER), TRUE)
           DDI_MISC_SRCDIR += \
            $(DDI_MISC_CUS_DIR)/api/frontend_SiLabs/SILABS_SUPERSET/Sources/TER           \
            $(DDI_MISC_CUS_DIR)/api/frontend_SiLabs/SILABS_SUPERSET/Sources/TER/Si2141
endif


