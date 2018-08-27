#! /bin/bash

CUR_DIR=$PWD

function GetMbootFileName()
{
    OUTPUT_FILE_NAME=$1
    BOARD_NAME=$2
    MEMORY_MAP_SIZE=$3
    ENABLE_BOOTLOGO=$4
    DDR_SIZE=$5

    MBOOT_BIN_PATH=""
    KEY_WORD=""
    BOARD_NAME=${BOARD_NAME:6:4}

    MBOOT_BIN_PATH=../../../bsp/ecos/
    find ${MBOOT_BIN_PATH} | grep -i ${BOARD_NAME} | grep -i ${DDR_SIZE} > ${OUTPUT_FILE_NAME}

    if [ "$ENABLE_BOOTLOGO" == "disable" ] ; then
        sed -i '/bootlogo/d' ${OUTPUT_FILE_NAME}
    fi

    PRESERVE_FIRST_MBOOT_IN_FILE=$(awk 'NR==1' ${OUTPUT_FILE_NAME})
    echo ${PRESERVE_FIRST_MBOOT_IN_FILE} > ${OUTPUT_FILE_NAME}

    cat ${OUTPUT_FILE_NAME}
}


function GetMMAPFileName()
{
    OUTPUT_FILE_NAME=$1
    BOARD_NAME=$2
    MEMORY_MAP_SIZE=$3

    MEMORY_MAP_SIZE=$(echo ${MEMORY_MAP_SIZE} | sed 's/[^0-9]//g')
    find . -name "MMAP*" | grep ${MEMORY_MAP_SIZE} | grep "DDI"  > ${OUTPUT_FILE_NAME}
}

function GetHash2Addr()
{
    KEY_WORD=""
    if [ "${1}" == "mboot" ] ; then
        KEY_WORD="CONFIG_HASH2_START_ADDRESS"
    elif [ "${1}" == "mmap" ] ; then
        KEY_WORD="SBOOT_LOADER_ADR"
    fi

    FILE_NAME=$(cat $2)
    HASH2_ADDR=$(strings -a ${FILE_NAME} | grep ${KEY_WORD})

    if [ "${1}" == "mboot" ] ; then
        HASH2_ADDR=${HASH2_ADDR: -7}
    elif [ "${1}" == "mmap" ] ; then
        HASH2_ADDR=${HASH2_ADDR: -22: 7}
    fi

    echo ${HASH2_ADDR}
}

main()
{
    TMP_MBOOT_FILE_NAME=_tmp_mboot_file_name
    TMP_MMAP_FILE_NAME=_tmp_mmap_file_name

    if [ "${MBOOT_PATH}" == "" ] ; then
        GetMbootFileName $TMP_MBOOT_FILE_NAME $@
    else
        echo "mboot path set by user: ${MBOOT_PATH}"
        echo ${MBOOT_PATH} > $TMP_MBOOT_FILE_NAME
    fi

    if [ "${MMAP_PATH}" == "" ] ; then
        GetMMAPFileName $TMP_MMAP_FILE_NAME $@
    else
        echo "mmap path set by user: ${MMAP_PATH}"
        echo "${MMAP_PATH}" > $TMP_MMAP_FILE_NAME
    fi

    HASH2_ADDR_IN_MBOOT=$(GetHash2Addr "mboot" ${TMP_MBOOT_FILE_NAME})
    HASH2_ADDR_IN_MMAP=$(GetHash2Addr "mmap" ${TMP_MMAP_FILE_NAME})

    echo "hash2 address in mboot: ${HASH2_ADDR_IN_MBOOT}"
    echo "hase2 address in mmap : ${HASH2_ADDR_IN_MMAP}"

    rm -f ${TMP_MBOOT_FILE_NAME}
    rm -f ${TMP_MMAP_FILE_NAME}

    if [ "${HASH2_ADDR_IN_MBOOT}" != "${HASH2_ADDR_IN_MMAP}" ] ; then
        echo "hash2 addr in sboot & mmap is mis-match"
        exit 1
    fi
}

main $@
