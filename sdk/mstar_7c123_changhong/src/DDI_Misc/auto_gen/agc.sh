#!/bin/bash

source ../../../bsp/linux/partition/auto_gen_output/var_list
VAR_LIST=../../../bsp/linux/partition/auto_gen_output/var_list

if [ "$CONF_SIZE" == "" ]
	then
		CONF_SIZE=0
fi
if [ "$LIB_SIZE" == "" ]
	then
		LIB_SIZE=0
fi
if [ "$MWLIB_SIZE" == "" ]
	then
		MWLIB_SIZE=0
fi


function agc_usage()
{
	echo
	echo "Usage:"
	echo "    agc.sh <cmd-type> "
	echo "Example:"
	echo "agc.sh dump cfg                       -> dump configuration info" 
	echo "agc.sh dump psize                     -> dump partition size" 
	echo "agc.sh dump <ufolder | tfolder>       -> dump partition folder (usb or tftp)"
	echo "agc.sh dump <usb | tftp> <quick cmd>  -> dump upgrade script quick commands" 
	echo "agc.sh dump build <quick cmd>         -> dump build script via quick commands" 
	echo "agc.sh exec build <quick cmd>         -> exec build script via quick commands" 
	echo "agc.sh clean rfs                      -> clean ROOTFS folder" 
	exit 1
}

if [ "$#" -lt "1" ] || [ "$#" -gt "4" ]
  then
    agc_usage "!! Invalid input format !!"
fi

CMD_TYPE=$1
if [ "$CMD_TYPE" == "dump" ]
  then
    if [ "$2" == "cfg" ]
      then
        echo   "==================== Configuration ====================="
        printf "%-20s   : %-30s  \r\n"    CHIP                    ${CHIP}     
        printf "%-20s   : %-30s  \r\n"    CFG_BOARD               ${CFG_BOARD}     
        printf "%-20s   : %-30s  \r\n"    LINUX_KERNEL_VERSION    ${LINUX_KERNEL_VERSION}     
        printf "%-20s   : %-30s  \r\n"    FLASH_TYPE              ${FLASH_TYPE}     
        printf "%-20s   : %-30s  \r\n"    FS_TYPE                 ${FS_TYPE}     
        printf "%-20s   : %-30s  \r\n"    LINK_TYPE               ${LINK_TYPE}     
        printf "%-20s   : %-30s  \r\n"    MIU_TYPE                ${MIU_TYPE}     
        printf "%-20s   : %-30s  \r\n"    NAND_PAGE_SIZE          ${NAND_PAGE_SIZE}     
        printf "%-20s   : %-30s  \r\n"    CFG_MEMORY_MAP          ${CFG_MEMORY_MAP}     
        printf "%-20s   : %-30s  \r\n"    TOOLCHAIN          			${TOOLCHAIN}     
        printf "%-20s   : %-30s  \r\n"    UTOPIA_PATH             ${UTOPIA_PATH}     
        echo   "========================================================="
    elif [ "$2" == "psize" ]
      then
        let ALL_SIZE=0
        let ALL_EXT_SIZE=0
        echo   "==================== Partition Size ====================="
        printf "%-08s   %-10s       %-10s\r\n" Name    Size            Ext-Size
        echo   "---------------------------------------------------------"
        printf "%-08s   %-10s       %-10s\r\n" APP     ${APP_SIZE}     ${EXT_APP_SIZE}
        if [ "$FLASH_TYPE" != "SPI" ]
        	then
		        printf "%-08s   %-10s       %-10s\r\n" CONF    ${CONF_SIZE}    ${EXT_CONF_SIZE}
        fi
        printf "%-08s   %-10s       %-10s\r\n" KERNEL  ${KERNEL_SIZE}  ${EXT_KERNEL_SIZE}
        printf "%-08s   %-10s       %-10s\r\n" RFS     ${RFS_SIZE}     ${EXT_RFS_SIZE}
        if [ "$LINK_TYPE" == "dynamic" ]
        	then
		        printf "%-08s   %-10s       %-10s\r\n" LIB     ${LIB_SIZE}     ${EXT_LIB_SIZE}
        fi
        if [ "$RELEASE_MW_LIB" == "enable" ]
        	then
		        printf "%-08s   %-10s       %-10s\r\n" MWLIB   ${MWLIB_SIZE}   ${EXT_MWLIB_SIZE}
        fi
        if [ "$MW_FLOW" == "enable" ]
        	then
		        printf "%-08s   %-10s       %-10s\r\n" CUST    ${CUST_SIZE}   ${EXT_MWLIB_SIZE}
        fi
        echo   "---------------------------------------------------------"
        let ALL_SIZE=$(($APP_SIZE + $CONF_SIZE + $KERNEL_SIZE + $RFS_SIZE + $LIB_SIZE + $MWLIB_SIZE))
        let ALL_EXT_SIZE=$(($EXT_APP_SIZE + $EXT_CONF_SIZE + $EXT_KERNEL_SIZE + $EXT_RFS_SIZE + $EXT_LIB_SIZE + $EXT_MWLIB_SIZE))
        let ALL_FREE_SIZE=$(($NAND_SIZE-$ALL_SIZE-ALL_EXT_SIZE))
        printf "%-08s   0x%-10x     0x%-10x\r\n" Total   ${ALL_SIZE}     ${ALL_EXT_SIZE}
        echo   "---------------------------------------------------------"
				if [ "$ALL_FREE_SIZE" -lt "0" ]
        	then
        		CHANGE_COLOR="\e[1;31;40m"
        fi
        printf "Max : 0x%-10x  Used :$CHANGE_COLOR 0x%-10x  Free : 0x%-10x \e[0m\r\n"  ${NAND_SIZE}  $(($ALL_SIZE+$ALL_EXT_SIZE)) $ALL_FREE_SIZE
        echo   "========================================================="
    elif [ "$2" == "tfolder" ]
      then
        ls -al ../../../bsp/linux/tftp_root/$FLASH_TYPE 
    elif [ "$2" == "ufolder" ]
      then
        ls -al ../../../bsp/linux/usb_update/Target/image    
    elif [ "$2" == "usb" ]
      then
      	if [ "$3" == "set_config" ] || [ "$3" == "set_partition" ] || [ "$3" == "all" ] || [ "$3" == "app" ] || [ "$3" == "conf" ] || [ "$3" == "config" ] || [ "$3" == "kernel" ] || [ "$3" == "lib" ] || [ "$3" == "mwlib" ] || [ "$3" == "rfs" ]
        	then
   	    		cat ../../../bsp/linux/usb_update/Target/script/$3 
   	    fi   
    elif [ "$2" == "tftp" ]
      then
      	if [ "$3" == "set_config" ] || [ "$3" == "set_partition" ] || [ "$3" == "all" ] || [ "$3" == "app" ] || [ "$3" == "conf" ] || [ "$3" == "config" ] || [ "$3" == "kernel" ] || [ "$3" == "lib" ] || [ "$3" == "mwlib" ] || [ "$3" == "rfs" ]
        	then
        		cat ../../../bsp/linux/tftp_root/$3
        fi
    elif [ "$2" == "build" ]
      then
      	if [ "$3" == "app" ]
      		then
      			cat ../../../bsp/linux/partition/build_app*
      	elif [ "$3" == "con" ]
      		then
      			cat ../../../bsp/linux/partition/build_con*
      	elif [ "$3" == "rfs" ]
      		then
      			cat ../../../bsp/linux/partition/build_rfs*
      	elif [ "$3" == "lib" ]
      		then
      			cat ../../../bsp/linux/partition/build_lib*
      	elif [ "$3" == "mwlib" ]
      		then
      			cat ../../../bsp/linux/partition/build_mwlib*
      	fi      			
    elif [ "$2" == "var_list" ] || [ "$2" == "conf_list" ] || [ "$2" == "ref_info" ]
      then
      	cat ../../../bsp/linux/partition/auto_gen_output/$2
    elif [ "$2" == "cc_opts" ] || [ "$2" == "cl_info" ] || [ "$2" == "conf_info" ] || [ "$2" == "sys_info" ]
      then
      	cat ../../../bsp/linux/partition/auto_gen_output/application/auto_gen/$2
    else
        agc_usage
    fi
elif [ "$CMD_TYPE" == "exec" ]
  then
    if [ "$2" == "build" ]
      then
      	cd ../../../bsp/linux/partition
      	if [ "$3" == "app" ]
      		then
      			./build_app*
      			cp ../usb_update/Target/image/app* ../tftp_root/${FLASH_TYPE}/
      	elif [ "$3" == "con" ]
      		then
      			./build_con*
      			cp ../usb_update/Target/image/con* ../tftp_root/${FLASH_TYPE}/
      	elif [ "$3" == "rfs" ]
      		then
      			./build_rfs*
      			cp ../usb_update/Target/image/ROOTFS* ../tftp_root/${FLASH_TYPE}/
      	elif [ "$3" == "lib" ]
      		then
      			./build_lib*
      			cp ../usb_update/Target/image/lib* ../tftp_root/${FLASH_TYPE}/
      	elif [ "$3" == "mwlib" ]
      		then
      			./build_mwlib*
      			cp ../usb_update/Target/image/mwlib* ../tftp_root/${FLASH_TYPE}/
      	fi    
      	cd -  			
    fi
elif [ "$CMD_TYPE" == "clean" ]
  then
    if [ "$2" == "rfs" ]
      then
        rm -rf ../../../bsp/linux/partition/ROOTFS/
    fi
else
    agc_usage
fi
