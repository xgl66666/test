#!/bin/bash

#########################
#                       #
# Do Not Need To Change #
#                       #
#########################

source ${AUTO_GEN_OUT}/var_list
AWKBINPATH=`which awk`
CATBINPATH=`which cat`
DOS2UNIXBINPATH=`which dos2unix`
GREPBINPATH=`which grep`
TEEBINPATH=`which tee`
CHMODBINPATH=`which chmod`
SEDBINPATH=`which sed`
SHBINPATH=`which sh`


##########################################################
# local function define
##########################################################
show_info(){
    echo "***********************************************************"
    echo   "Information from check_mmap.sh:"
    echo   "  CONFIG_PATH         :$CONFIG_PATH"
    echo   "  BOARD               :$BOARD"
    echo   "  BOARD_H             :$BOARD_H"
    echo   "  MMAP                :$CFG_MEMORY_MAP"
    echo   "  UIMAGE_LX_MEM       :$UIMAGE_LX_MEM"
    echo   "  LAST_SIZE           :0x${LAST_SIZE}"
    echo   "  LAST_SECTION        :0x${LAST_SECTION}"
    printf "  KERNEL_DEFAULT_SIZE :0x%X\n" $KERNEL_DEFAULT_SIZE
    printf "  LX_MEM              :0x%X\n" $LX_MEM_LEN
    printf "  REAL_MEM            :0x%X\n" $real_mem
    echo "***********************************************************"
    echo
}

die(){
    echo ""
    echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
    local counter=0
    local total_para=$#
    while [ $counter -lt $total_para ]
    do
        echo $1
        shift
        let counter=counter+1
    done
    show_info
    echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
    echo ""
    exit 1 #end of fail
}

get_uimage_lxmem(){
        UIMAGE_LX_MEM=0x`xxd -l 4 -p -seek 16 $UIMAGE`
}


get_klmem(){
    $READ_ELF -S bin/${IMAGENAME}1 > check.txt
    grep -w 'bss' check.txt > check1.txt 
    TEMP1=`$CATBINPATH check1.txt | awk -W non-decimal-data {'print $4'}`
    TEMP2=`$CATBINPATH check1.txt | awk -W non-decimal-data {'print $6'}`
    LAST_SECTION=`echo $TEMP1 | sed 's/^0*//g'`
    LAST_SIZE=`echo $TEMP2 | sed 's/^0*//g'`
		echo 
    all=$(($((16#${LAST_SIZE}))+$((16#${LAST_SECTION}))+$KERNEL_DEFAULT_SIZE))
    rm -rf check.txt
    rm -rf check1.txt
}



exam_lx_size(){
    let alloc_mem=$LX_MEM_LEN
    let real_mem=$all
    if [ $alloc_mem -lt $real_mem ]; then
        die "linux kernel memory allocation is not correct. You should reduce HW Buffer usage for kernel in MMAP" "kernel load base in $UIMAGE:$UIMAGE_LX_MEM " "LX_MEM in $SET_CONFIG:$LX_MEM" "HW_BUFFER_START_AVAILABLE in MMAP:$LX_MEM_LEN"
    fi
}

##########################################################
#Local Parameters
##########################################################
[ "$1" == "" ] && TARGET_PATH=Target || TARGET_PATH=$1
[ "$2" == "" ] && SET_CONFIG_TYPE=set_config || SET_CONFIG_TYPE=$2
[ "$3" == "" ] && CONFIG_PATH=check_par || CONFIG_PATH=$3

UIMAGE="$TARGET_PATH/image/${KERNEL_IMG_NAME}"
SET_CONFIG="$TARGET_PATH/script/$SET_CONFIG_TYPE"
PLATFORM_CONFIG="cus_config/platform_define"
BOARD=`$DOS2UNIXBINPATH $CONFIG_PATH ; $CATBINPATH $CONFIG_PATH | $GREPBINPATH '^CFG_BOARD' | $AWKBINPATH {'print $3'}`
BOARD_H=include/${BOARD}.h

# temp check
[ -f $UIMAGE ] || die "$UIMAGE is not exist"
[ -f $SET_CONFIG ] || die "$SET_CONFIG is not exist"
[ -f $BOARD_H ] || die "$BOARD_H is not exist"


##########################################################
# Shell Script Start Here
##########################################################

get_uimage_lxmem
get_klmem

exam_lx_size

if [ "$SHOW_CHK_MMAP" == "true" ]
	then show_info
fi

exit 0
