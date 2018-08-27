USE_SiLAB_SUPERSET = FALSE

ifeq ($(USE_SiLAB_SUPERSET), TRUE)
    # Current Support Frontends
    #SiLABS_FE_TYPE = Si2166_AV2018_A8304
    SiLABS_FE_TYPE = Si2180_Si2141
    
    CFG_CC_OPTS += -D'FRONT_END_COUNT=1'
    CFG_CC_OPTS += -DNO_WIN32
    #debug trace
    #CFG_CC_OPTS += -DSiTRACES
    ifeq ($(SiLABS_FE_TYPE), Si2166_AV2018_A8304)
        SUPPORT_SAT = TRUE
        #To build with the SAT tuner wrapper code 
        CFG_CC_OPTS += -DSAT_TUNER_SILABS
        #To select the SAT tuner
        CFG_CC_OPTS += -DSAT_TUNER_AV2018
        #To select the LNB controller 
        CFG_CC_OPTS += -DA8304_COMPATIBLE       
        #To use the Si2183 code as the SILABS SUPERSET code, a compilation flag is required: 
        CFG_CC_OPTS += -DSILABS_SUPERSET 
        #To use the Si2183 code for SAT reception, a compilation flag is required: 
        CFG_CC_OPTS += -DSATELLITE_FRONT_END         
        #For ¡¥general¡¦ SAT selection: 
        CFG_CC_OPTS += -DDEMOD_DVB_S_S2_DSS
        #For DVB-S2X support:        
        CFG_CC_OPTS += -DDEMOD_DVB_S2X        
        CFG_CC_OPTS += -DSi2183_B60_COMPATIBLE             
    else ifeq ($(SiLABS_FE_TYPE), Si2180_Si2141)
        SUPPORT_TER = TRUE
        #To build with the TER tuner wrapper code 
        CFG_CC_OPTS += -DTER_TUNER_SILABS
        #To select the TER tuner
        CFG_CC_OPTS += -DTER_TUNER_Si2141
        #To use the Si2183 code as the SILABS SUPERSET code, a compilation flag is required: 
        CFG_CC_OPTS += -DSILABS_SUPERSET
        #To use the Si2183 code for SAT reception, a compilation flag is required: 
        CFG_CC_OPTS += -DTERRESTRIAL_FRONT_END 
        #For ISDB-T selection: 
        CFG_CC_OPTS += -DDEMOD_ISDB_T
        CFG_CC_OPTS += -DSi2183_A55_COMPATIBLE
    endif
endif            