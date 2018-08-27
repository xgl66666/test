# FILE NAME: $URL:$
#            $Rev:$
#            $Date: $

#  DESCRIPTION: This file aims to collect all CFLAGS relative to RAM memory mapping


# TKEL_MEM_SIZE
# for bootloader need 2*binary image maximum size + 200K,
# binary image maximum size = highest end address of partition A - start address of comedia
ifeq "$(PROGTYPE)" "BOOTLOADER"
    CFLAGS += -DTKEL_MEM_SIZE=(5*1024*1024)
else
    ifeq "$(PRODUCT_ENGINE)" "ENGINE_TER_MHEG"
        CFLAGS += -DTKEL_MEM_SIZE=0xF00000
    else
        ifeq "$(PRODUCT_TELETEXT_OSD)" "TTXT_OSD_ON"
            CFLAGS += -DTKEL_MEM_SIZE=0x1312d00
        else
			ifeq "$(PRODUCT_APP)" "MAESTRO"	
				ifeq "$(PRODUCT_CCAS)" "NAGRA_DALTS"
					CFLAGS += -DTKEL_MEM_SIZE=0xA00000
				else 
					CFLAGS += -DTKEL_MEM_SIZE=0x880000
				endif 
			endif 
			ifeq "$(PRODUCT_APP)" "CELINE"
				CFLAGS += -DTKEL_MEM_SIZE=0x300000
			endif 
        endif
    endif
endif


