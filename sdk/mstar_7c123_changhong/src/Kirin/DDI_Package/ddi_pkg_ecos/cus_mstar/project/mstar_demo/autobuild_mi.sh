PRJ_DIR=`pwd`
DDI_MI_DIR=$PRJ_DIR/../../../../../../DDI_MI
CFG_MI=$PRJ_DIR/configure_mi
SET_CFG_MI=$PRJ_DIR/autobuild_mi.cfg

cfg_mi_total_line=$(awk 'END{print NR}' $SET_CFG_MI)
echo -e "configure_mi record num:"$cfg_mi_total_line

function Recovery(){
		source set_mi_env.cfg
		echo -e "source set_env.cfg"
}

function Substitute(){
	local mi_type=$(awk '/MI_TYPE/{print $3}' "$1")
	echo -e "mi_type:"$mi_type
	local record_num=$(awk 'END{print NR}' "$1")
	local i
	for(( i=1; i<=$record_num; i++ ))
	do
		local field_num=$(awk 'NR=='"$i"' {print NF}' "$1")
		local cfg=$(awk 'NR=='"$i"' {print $1}' "$1")
		echo -e "field_num:"$field_num
		local j
		for(( j=2; j<=$field_num; j++ ))
		do
			local val=$(awk 'NR=='"$i"' {print $'"$j"'}' "$1")
			echo -e "set "$cfg" = "$val
			sed -i 's/^'"$cfg"'.*=.*/'"$cfg"' = '"$val"'/' "$2"
			if [ "$cfg" == "MM_BSP" ]; then
				echo -e "set GST_BSP = "$val
				sed -i 's/^GST_BSP.*=.*/GST_BSP = '"$val"'/' "$2"
			fi
			if [ "$cfg" == "MI_DISP_LITE" ]; then
				echo -e "set MI_TYPE = "$mi_type
				sed -i 's/^MI_TYPE.*=.*/MI_TYPE = '"$mi_type"'/' "$2"
			fi

			make -C $DDI_MI_DIR/cus_mstar/mi setup
			#read -p "Please check: " tmp
			make
			make clean

			Recovery $2
		done
	done
}

source set_mi_env.cfg
Substitute $SET_CFG_MI $CFG_MI
cp $CFG_COPY $CFG_SRC
rm $CFG_COPY
