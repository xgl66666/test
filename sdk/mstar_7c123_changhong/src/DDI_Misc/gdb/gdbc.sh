#!/bin/bash

function gdbc_usage()
{
	echo
	echo $PRJ_NAME
	echo
	echo "Usage:"
	echo "    gdbc.sh <cmd-type> [server-ip] [port-no] [core-file]"
	echo "Example:"
	echo "    gdbc.sh cmd  172.16.8.32 6315 [host_ap_path_name] [target_ap_path_name]   "
	echo "    gdbc.sh gui "
	echo "    gdbc.sh core Coredump.gz [host_ap_path_name] [target_ap_path_name]    "
	echo "    gdbc.sh clean"
	exit 1
}


function construct_rootfs()
{
	echo "Construct sysroot for GDB ..."
	[ -d ${MISC_DIR}/gdb/ROOTFS ] && `rm -rf ${MISC_DIR}/gdb/ROOTFS`
	cd ${PRJ_DIR}/bsp/linux/partition
	sudo tar -zcvf ${MISC_DIR}/gdb/ROOTFS.tar.gz ROOTFS
	cd ${MISC_DIR}/gdb/
	sudo tar -zxvf ROOTFS.tar.gz ROOTFS
	rm -f ROOTFS.tar.gz
	cd ${PRJ_DIR}/cus_mstar/project/mstar_demo
}

source ../../../bsp/linux/partition/auto_gen_output/var_list
PRJ_NAME=$CHIP

HOST_AP=bin/${PRJ_NAME}_linux1_gdb
TARGET_AP=application/${PRJ_NAME}_linux1

if [ "$#" -lt "1" ] || [ "$#" -gt "5" ]
  then
    gdbc_usage "!! Invalid input format !!"
fi

if [ "$1" == "core" ]
  then
    if [ "$#" == "4" ]
      then
        HOST_AP=$3
        TARGET_AP=$4
        echo "HOST_AP=$HOST_AP"
        echo "TARGET_AP=$TARGET_AP"
    elif [ "$#" == "2" ]
      then
        echo "Coredump file is $2"  
    else  
      gdbc_usage "!! Invalid input format !!"
    fi
fi

if [ "$1" == "cmd" ]
  then
    if [ "$#" == "5" ]
      then
        HOST_AP=$4
        TARGET_AP=$5
        echo "HOST_AP=$HOST_AP"
        echo "TARGET_AP=$TARGET_AP"
    elif [ "$#" == "3" ]
      then
        echo "Connect to s"  
    else  
      gdbc_usage "!! Invalid input format !!"
    fi
fi
PRJ_DIR=`pwd | sed -e 's/ddi_pkg_linux\/\(.*\)/ddi_pkg_linux/g'`
#PRJ_DIR=`${PWD%/cus_mstar*}`

#prepare sysroot folder
if [ ! -d ${MISC_DIR}/gdb/ROOTFS ] 
	then
		construct_rootfs
else
		if [ ! -f ${MISC_DIR}/gdb/ROOTFS/$TARGET_AP ]
			then
				construct_rootfs
		fi
fi
cp -rf ${PRJ_DIR}/bsp/linux/partition/auto_gen_output/application ${MISC_DIR}/gdb/ROOTFS/
cp -f ${PRJ_DIR}/cus_mstar/project/mstar_demo/$HOST_AP ${MISC_DIR}/gdb/ROOTFS/$TARGET_AP
cp -rf ${PRJ_DIR}/bsp/linux/partition/auto_gen_output/conf* ${MISC_DIR}/gdb/ROOTFS/
if [ "$RELEASE_MW_LIB" == "enable" ]
	then
		cp -rf ${PRJ_DIR}/bsp/linux/partition/auto_gen_output/mwlib/* ${MISC_DIR}/gdb/ROOTFS/vendor/lib
fi
if [ "$LINK_TYPE" == "dynamic" ]
	then
		cp -rf ${PRJ_DIR}/bsp/lib_dynamic/* ${MISC_DIR}/gdb/ROOTFS/vendor/lib/utopia
fi

#re-gen gdbc_cmd
rm -f gdbc_cmd
echo "set solib-absolute-prefix ${MISC_DIR}/gdb/ROOTFS" > gdbc_cmd
echo "set sysroot ${MISC_DIR}/gdb/ROOTFS" >> gdbc_cmd

echo "TOOLCHAIN=${TOOLCHAIN}"

CMD_TYPE=$1
if [ "$CMD_TYPE" == "core" ]
	then
		CORE_FILE=$2
		if [[ "$CORE_FILE" == *".gz" ]]
			then
				gunzip $CORE_FILE
				CORE_FILE=`printf "%s" ${CORE_FILE:0:$((${#CORE_FILE}-3))}`
		fi
		echo "core ${CORE_FILE}" >> gdbc_cmd
		${TOOLCHAIN}-gdb $HOST_AP -x gdbc_cmd
elif [ "$CMD_TYPE" == "clean" ]
	then
		echo "Clean sysroot folder ..."
		[ -d ${MISC_DIR}/gdb/ROOTFS ] && `rm -rf ${MISC_DIR}/gdb/ROOTFS`		
else
		IP=$2
		PORT=$3

		if [ "$CMD_TYPE" == "cmd" ]
		  then
				echo "target remote ${IP}:${PORT}" >> gdbc_cmd
				echo "shell cp -rf ../../../bsp/lib_dynamic/* ${MISC_DIR}/gdb/ROOTFS/vendor/lib/utopia" >> gdbc_cmd
				
				#replace current utopia lib absolute path
				if [ "$TOOLCHAIN" == "arm-none-linux-gnueabi" ]
					then
						FROM_PATH=`${TOOLCHAIN}-readelf ../../../bsp/lib_dynamic/liblinux.so --debug-dump | grep "drvSysInfo.c" | grep "filename" | awk '{print $8}' | sed 's/THEALE.*/THEALE/g'`
				else
						FROM_PATH=`${TOOLCHAIN}-objdump ../../../bsp/lib_dynamic/liblinux.so -x | grep "drvSysInfo.c" | awk '{print $6}' | sed 's/THEALE.*/THEALE/g'`
				fi
				TO_PATH=`pwd | sed 's/THEALE.*/THEALE/g'`
				echo FROM_PATH=$FROM_PATH
				echo TO_PATH=$TO_PATH
				echo "set substitute-path ${FROM_PATH} ${TO_PATH}" >> gdbc_cmd  
		    
				${TOOLCHAIN}-gdb $HOST_AP -x gdbc_cmd
		elif [ "$CMD_TYPE" == "gui" ]
		  then
				ECLIPSE_PATH=`pwd | sed -e "s/\/PERFORCE\/\(.*\)/\/GDB\/eclipse/g"`
				/.${ECLIPSE_PATH}/eclipse
		else
		    gdbc_usage "!! Invalid input format !!"
		fi
fi









