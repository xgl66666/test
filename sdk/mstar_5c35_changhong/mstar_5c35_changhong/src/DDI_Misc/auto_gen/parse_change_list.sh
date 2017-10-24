
source ${AUTO_GEN_OUT}/var_list

if [ "$LINK_TYPE" == "static" ]
	then
		LIB_PATH=${BSP_PRJ}/bsp/lib
else
		LIB_PATH=${BSP_PRJ}/bsp/lib_dynamic
fi
CL_INFO=${BSP_PRJ}/bsp/linux/partition/auto_gen_output/application/auto_gen/cl_info
echo > $CL_INFO || err_exit "file not exit"


function get_cl_info()
{
	printf "\n[$1]\n" >> $CL_INFO
	ITEM_CNT=`readelf --string-dump=.mmodule_version $1 2> /dev/null | grep -c LIBCODE`

	c=1
	PREV_NAME_LEN=0
	PREV_LIST=""
	
	while [ $c -le $ITEM_CNT ]
	do
		MOD_NAME=`readelf --string-dump=.mmodule_version $1 2> /dev/null | grep -m $c LIBCODE | sed 's/\(.*\)LIBCODE\(.*\)/LIBCODE\2/'`
		let NAME_LEN=`expr length "$MOD_NAME"`
		MOD_NAME=`echo ${MOD_NAME:$((PREV_NAME_LEN)):$((NAME_LEN-PREV_NAME_LEN-1))}`
		((PREV_NAME_LEN=NAME_LEN))

		CL_NO=`readelf --string-dump=.mmodule_version $1 2> /dev/null | grep -m $c Change | awk {'print $2'}`
		let CL_LEN=`expr length "$CL_NO"`
		CL_NO=`echo ${CL_NO:(((c-1)*6 + (c-1))):$((CL_LEN-(c-1)))}`
	
		CUR_LIST=`echo "${MOD_NAME:8}:${CL_NO}"`
		if [ "$PREV_LIST" != "$CUR_LIST" ]
			then
				echo $CUR_LIST >> $CL_INFO
		fi
		PREV_LIST=$CUR_LIST
		((c++))
	done

}


#get_cl_info libdrvVE.a

#get_cl_info liblinux.a

cd $LIB_PATH

for i in $(ls)
do
    get_cl_info $i
done

cd -
