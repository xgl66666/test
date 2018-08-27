
# Remove atsc_cc.c
#TARGET=apiCC_atsc_modify.h
TARGET=$1

# Get all typedef line number.
# such as:
#     typedef struct BBB
#     { ...
#     } AAA
DelStart=$(awk '/typedef/ {print NR}' $TARGET)
Delend=$(awk '/\}/ {print NR}' $TARGET)
# Get redefine names line number.
RealDel=$(awk '/stCaptionWindowSetting|EN_CC_SYSINFO|EN_CC_SERVICE|EN_CC_FONT|EN_CC_RET|EN_CC_SELECTOR|EN_CC_ERRORCODE|EN_CC_TSPMODE|MsMW_CcMemOp_t|APICC_GOP_DST_TYPE/ {print NR}' $TARGET)
DelStart_array=($(echo $DelStart))
Delend_array=($(echo $Delend))
ReadDel_array=($(echo $RealDel))


declare count start end
declare -i diff diff_pre

# All {} search result array index.
count=0
# real delete target array index.
rel_count=0
# Calc the correcgt line number after each loop execution.
diff=0
diff_pre=0

# Algo: if '}'line number not equal to real delete line number, ignore the {} deletion and shift the count to next round.
for num in $DelStart; do
	start=$((${DelStart_array[$count]}-${diff}));
	end=$((${ReadDel_array[$rel_count]}-${diff}));
	check=${Delend_array[$count]};
	if [ $check != ${ReadDel_array[$rel_count]} ]; then
#	    echo "Ingore check array!!"
#	    echo $start,$end,$check;
#	    start=${DelStart_array[0]};
#	    end=${Delend_array[0]};
	    count=$(($count+1));
	else
#	    echo $start,$end
	    diff_pre=$diff;
	    diff=$((${ReadDel_array[$rel_count]}-${DelStart_array[$count]}+1));
	    diff=$(( ${diff}+${diff_pre}))
#	    echo $diff;
	    count=$(($count+1));
	    rel_count=$(($rel_count+1));
	    sed -i ''"$start"','"$end"'d' $TARGET;
	fi
#	if [ $count == 10 ]; then
#		break;
#	fi
    done
