#!/bin/bash

VAR_LIST=${AUTO_GEN_OUT}/var_list
source $VAR_LIST

SYS_INFO=${AUTO_GEN_OUT}/application/auto_gen/sys_info
REF_INFO=${AUTO_GEN_OUT}/ref_info
TMP_CONF=${AUTO_GEN_OUT}/application/auto_gen

if [ -f $REF_INFO ]
	then
		# calculate FS_TYPE_LEN
		FS_TYPE_LEN=`expr length "$FS_TYPE"`
		echo "### Definition by sysInfo.sh ###" >> $VAR_LIST
		echo "FS_TYPE_LEN=$FS_TYPE_LEN" >> $VAR_LIST
		
		# get RFS timestamp
		RFS_TAR=`echo $LINUX_KERNEL_VERSION | sed -e 's/\./_/g'`
		if [ "$FLASH_TYPE" == "SPI" ]
		  then
		  	RFS_TAR=../../../bsp/linux/linux_kernel/${LINUX_KERNEL_VERSION}/${FS_TYPE}/SPI/ROOTFS_${RFS_TAR}_${FS_TYPE}.${TAR_EXT_NAME}
		else
				RFS_TAR=../../../bsp/linux/linux_kernel/${LINUX_KERNEL_VERSION}/${FS_TYPE}/ROOTFS_${RFS_TAR}_${FS_TYPE}.${TAR_EXT_NAME}
		fi
		RFS_TS=`ls -al ${RFS_TAR} | awk '{print $6 "@" $7}'`
		echo "RFS_TS=$RFS_TS" >> $VAR_LIST
		
		echo "********************************************"
		echo "************  SYS Info Generate ************"
		echo "********************************************"
		
		# get PREV_TYPE_LEN #
		#PREV_TYPE_LEN=`grep -m1 "TypeLen" $REF_INFO | awk '{print $1}'`
		#PREV_TYPE_LEN=`printf "%d" ${PREV_TYPE_LEN:8:1}`
		
		# get PREV_FS_TYPE #
		PREV_FS_TYPE=`grep -m1 "FsType" $REF_INFO | awk '{print $1}'`
		PREV_FS_TYPE=`printf "%s" ${PREV_FS_TYPE:7}`
		
		# get PREV_LINUX_KERNEL_VERSION #
		PREV_LINUX_KERNEL_VERSION=`grep -m1 "LinuxKernelVersion" $REF_INFO | awk '{print $1}'`
		PREV_LINUX_KERNEL_VERSION=`printf "%s" ${PREV_LINUX_KERNEL_VERSION:19}`
		
		# get PREV_NAND_PAGE_SIZE #
		PREV_NAND_PAGE_SIZE=`grep -m1 "NandPageSize" $REF_INFO | awk '{print $1}'`
		PREV_NAND_PAGE_SIZE=`printf "%d" ${PREV_NAND_PAGE_SIZE:13:4}`

		# get PREV_RFS_TS #
		PREV_RFS_TS=`grep -m1 "RfsTS" $REF_INFO | awk '{print $1}'`
		PREV_RFS_TS=`printf "%s" ${PREV_RFS_TS:6}`
fi
		

echo > $REF_INFO

#echo "TypeLen=${FS_TYPE_LEN}" >> $REF_INFO
#echo "PrevTypeLen=${PREV_TYPE_LEN}" >> $REF_INFO

echo "FsType=${FS_TYPE}" >> $REF_INFO
echo "PrevFsType=${PREV_FS_TYPE}" >> $REF_INFO

echo "LinuxKernelVersion=${LINUX_KERNEL_VERSION}" >> $REF_INFO
echo "PrevLinuxKernelVersion=${PREV_LINUX_KERNEL_VERSION}" >> $REF_INFO

echo "NandPageSize=${NAND_PAGE_SIZE}" >> $REF_INFO
echo "PrevNandPageSize=${PREV_NAND_PAGE_SIZE}" >> $REF_INFO

echo "RfsTS=${RFS_TS}" >> $REF_INFO
echo "PrevRfsTS=${PREV_RFS_TS}" >> $REF_INFO




#mkdir -p ${TMP_CONF}
echo "System Info are appended below ..." > $SYS_INFO


### start to add system information here ###
BUILD_DATE_TIME=`date`
echo "Build Date Time : $BUILD_DATE_TIME" >> $SYS_INFO




