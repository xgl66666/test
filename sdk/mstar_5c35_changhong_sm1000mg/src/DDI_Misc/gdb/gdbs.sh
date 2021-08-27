#!/bin/bash

source /application/auto_gen/conf_info
PRJ_NAME=$CHIP

#check input format
if [ "$#" != "1" ]
  then
		echo $PRJ_NAME
    echo
		echo "Invalid input format!!"
    echo
    echo "Usage:"
    echo "    gdbs.sh <ext-no> [target_ap_path_name]"
    echo "Example:"
    echo "    gdbs.sh 6315                             "
    echo "    gdbs.sh 6315 appilcation/kaiser_linux1   "
    exit 1
fi

#check ext-no format
if [ "${#1}" != "4" ]
  then
    echo "Invalid extension number!!"
    exit 1
fi

TARGET_AP=application/${PRJ_NAME}_linux1
if [ "$#" == "2" ]
  then
    TARGET_AP=$5
    echo "TARGET_AP=$TARGET_AP"
else  
  gdbc_usage "!! Invalid input format !!"
fi

EXT_NO=$1
PORT_NO=$EXT_NO
MAC_ADR=00:${EXT_NO:0:2}:${EXT_NO:2:2}:00:00:01
echo "MAC Address = $MAC_ADR"

#set mac addr
ifconfig eth0 hw ether $MAC_ADR

#get ip
udhcpc

#start gdb server
gdbserver :$PORT_NO /$TARGET_AP
