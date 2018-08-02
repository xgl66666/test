#!/bin/bash
declare -A chip;
chip=([k1]=k1 [kappa]=kappa [keltic]=keltic [kriti]=kriti [keres]=keres)
declare -A ddr_size;
ddr_size=( [32M]=CONFIG_MSTAR_DDR_32MB=y
          [64M]=CONFIG_MSTAR_DDR_64MB=y
          [128M]=CONFIG_MSTAR_DDR_128MB=y
          [256M]=CONFIG_MSTAR_DDR_256MB=y
          [512M]=CONFIG_MSTAR_DDR_512MB=y
          [32M*32M]=CONFIG_MSTAR_DDR_32MB_32MB=y
          [64M*32M]=CONFIG_MSTAR_DDR_64MB_32MB=y
          [64M*64M]=CONFIG_MSTAR_DDR_64MB_64MB=y
          [128M*64M]=CONFIG_MSTAR_DDR_128MB_64MB=y
          [128M*128M]=CONFIG_MSTAR_DDR_128MB_128MB=y
          [256M*256M]=CONFIG_MSTAR_DDR_256MB_256MB=y
          [512M*512M]=CONFIG_MSTAR_DDR_512MB_512MB=y)
declare -A mmap;
mmap=(    [32M]=CONFIG_MSTAR_MMAP_32MB=y
          [64M]=CONFIG_MSTAR_MMAP_64MB=y
          [128M]=CONFIG_MSTAR_MMAP_128MB=y
          [256M]=CONFIG_MSTAR_MMAP_256MB=y
          [512M]=CONFIG_MSTAR_MMAP_512MB=y
          [32M*32M]=CONFIG_MSTAR_MMAP_32MB_32MB=y
          [64M*32M]=CONFIG_MSTAR_MMAP_64MB_32MB=y
          [64M*64M]=CONFIG_MSTAR_MMAP_64MB_64MB=y
          [128M*64M]=CONFIG_MSTAR_MMAP_128MB_64MB=y
          [128M*128M]=CONFIG_MSTAR_MMAP_128MB_128MB=y
          [256M*256M]=CONFIG_MSTAR_MMAP_256MB_256MB=y
          [512M*512M]=CONFIG_MSTAR_MMAP_512MB_512MB=y)
declare -A board_k1;
board_k1=( [MST124A_D01A]=CONFIG_MSTAR_KRONUS_BD_MST124A_D01A=y
          [MST124A_D01A_S]=CONFIG_MSTAR_KRONUS_BD_MST124A_D01A_S=y
          [MST124A_D02A_S]=CONFIG_MSTAR_KRONUS_BD_MST124A_D02A_S=y
          [MST124B_D01A_S]=CONFIG_MSTAR_KRONUS_BD_MST124B_D01A_S=y
          [MST124B_D02A]=CONFIG_MSTAR_KRONUS_BD_MST124B_D02A=y
          [MST124B_D02A_S]=CONFIG_MSTAR_KRONUS_BD_MST124B_D02A_S=y
          [MST124C_D01A_S]=CONFIG_MSTAR_KRONUS_BD_MST124C_D01A_S=y
          [MST124C_D02A]=CONFIG_MSTAR_KRONUS_BD_MST124C_D02A=y
          [MST124D_D01A]=CONFIG_MSTAR_KRONUS_BD_MST124D_D01A=y
          [MST124D_D02A]=CONFIG_MSTAR_KRONUS_BD_MST124D_D02A=y
          [MST124D_D02A_S]=CONFIG_MSTAR_KRONUS_BD_MST124D_D02A_S=y
          [MST124E_D01A_S]=CONFIG_MSTAR_KRONUS_BD_MST124E_D01A_S=y
          [MST124E_D01A_SZ]=CONFIG_MSTAR_KRONUS_BD_MST124E_D01A_SZ=y
          [MST124SZ]=CONFIG_MSTAR_KRONUS_BD_MST124SZ=y
          [MST124G_D01A]=CONFIG_MSTAR_KRONUS_BD_MST124G_D01A=y
          [MST124G_D01A_S]=CONFIG_MSTAR_KRONUS_BD_MST124G_D01A_S=y
          [MST140A_D01A]=CONFIG_MSTAR_KRONUS_BD_MST140A_D01A=y
          [MST140A_D01A_S]=CONFIG_MSTAR_KRONUS_BD_MST140A_D01A_S=y
          [MST163A_D01A_S]=CONFIG_MSTAR_KRONUS_BD_MST163A_D01A_S=y
          [MST124F_D01A_S]=CONFIG_MSTAR_KRONUS_BD_MST124F_D01A_S=y
          [MST124F_SZ_Demo]=CONFIG_MSTAR_KRONUS_BD_MST124F_SZ_Demo=y
          [MST124F_D02A]=CONFIG_MSTAR_KRONUS_BD_MST124F_D02A=y
          [MST124I_D01A_S]=CONFIG_MSTAR_KRONUS_BD_MST124I_D01A_S=y
          [MST124H_D01A]=CONFIG_MSTAR_KRONUS_BD_MST124H_D01A=y
          [MST124K_D01A_S]=CONFIG_MSTAR_KRONUS_BD_MST124K_D01A_S=y
          [MST124K_D01A]=CONFIG_MSTAR_KRONUS_BD_MST124K_D01A=y
          [SS2000_DT0A]=CONFIG_MSTAR_KRONUS_BD_SS2000_DT0A=y)
declare -A board_kappa;
board_kappa=( [FPGA]=CONFIG_MSTAR_KAPPA_BD_FPGA=y
              [MST173A_D01A_S]=CONFIG_MSTAR_KAPPA_BD_MST173A_D01A_S=y
              [MST173A_D01A]=CONFIG_MSTAR_KAPPA_BD_MST173A_D01A=y
              [MST173A_D02A]=CONFIG_MSTAR_KAPPA_BD_MST173A_D02A=y
              [MST173B_D01A_S]=CONFIG_MSTAR_KAPPA_BD_MST173B_D01A_S=y
              [MST173B_D01A]=CONFIG_MSTAR_KAPPA_BD_MST173B_D01A=y
              [MST173C_D01A_S]=CONFIG_MSTAR_KAPPA_BD_MST173C_D01A_S=y
              [MST173C_D01A]=CONFIG_MSTAR_KAPPA_BD_MST173C_D01A=y)
declare -A board_kriti;
board_kriti=( [FPGA]=CONFIG_MSTAR_KRITI_BD_FPGA=y
              [MST204A_D01A_S_DDR2]=CONFIG_MSTAR_KRITI_BD_MST204A_D01A_S_DDR2=y
              [MST204A_D01A_S_DDR3]=CONFIG_MSTAR_KRITI_BD_MST204A_D01A_S_DDR3=y
              [MST204A_D01A_DDR3]=CONFIG_MSTAR_KRITI_BD_MST204A_D01A_DDR3=y
              [MST204A_D01A_DDR2]=CONFIG_MSTAR_KRITI_BD_MST204A_D01A_DDR2=y)
declare -A board_keltic;
board_keltic=( [FPGA]=CONFIG_MSTAR_KELTIC_FPGA=y
             [MST183A_D01A_S]=CONFIG_MSTAR_KELTIC_BD_MST183A_D01A_S=y
             [MST183A_D01A]=CONFIG_MSTAR_KELTIC_BD_MST183A_D01A=y
             [MST183B_D01A_S]=CONFIG_MSTAR_KELTIC_BD_MST183B_D01A_S=y
             [MST183B_D01A]=CONFIG_MSTAR_KELTIC_BD_MST183B_D01A=y)
declare -A board_keres;
board_keres=( [FPGA]=CONFIG_MSTAR_KERES_FPGA=y
            [MST200A_D01A_S]=CONFIG_MSTAR_KERES_BD_MST200A_D01A_S=y
            [MST200A_D01A]=CONFIG_MSTAR_KERES_BD_MST200A_D01A=y
            [MST200B_D01A_S]=CONFIG_MSTAR_KERES_BD_MST200B_D01A_S=y
            [MST200B_D01A]=CONFIG_MSTAR_KERES_BD_MST200B_D01A=y
            [MST200C_D01A_S]=CONFIG_MSTAR_KERES_BD_MST200C_D01A_S=y
            [MST200C_D01A]=CONFIG_MSTAR_KERES_BD_MST200C_D01A=y)
declare -A board_kirin;
board_kirin=( [FPGA]=CONFIG_MSTAR_KIRIN_FPGA=y
            [MST221A_D01A_S]=CONFIG_MSTAR_KIRIN_BD_MST221A_D01A_S=y
            [MST221A_D01A]=CONFIG_MSTAR_KIRIN_BD_MST221A_D01A=y
            [MST221B_D01A_S]=CONFIG_MSTAR_KIRIN_BD_MST221B_D01A_S=y
            [MST221B_D01A]=CONFIG_MSTAR_KIRIN_BD_MST221B_D01A=y
            [MST221C_D01A_S]=CONFIG_MSTAR_KIRIN_BD_MST221C_D01A_S=y
            [MST221C_D01A]=CONFIG_MSTAR_KIRINS_BD_MST221C_D01A=y)
declare -A mem_freq;
mem_freq=( [1866MHZ]=CONFIG_MEM1866MHz=y 
           [1600MHZ]=CONFIG_MEM1600MHz=y 
           [1300MHZ]=CONFIG_MEM1300MHz=y
           [1066MHZ]=CONFIG_MEM1066MHz=y 
           [1333MHZ]=CONFIG_MEM1333MHz=y 
           [1200MHZ]=CONFIG_MEM1200MHz=y
           [800MHZ]=CONFIG_MEM800MHz=y 
           [667MHZ]=CONFIG_MEM667MHz=y
           [400MHZ]=CONFIG_MEM400MHz=y  
           [333MHZ]=CONFIG_MEM333MHz=y)

declare -A dtv_sys;
dtv_sys=([DVBC]=CONFIG_DVB_C_SYS=y
         [DVBT]=CONFIG_DVB_T_SYS=y
         [DVBT2]=CONFIG_DVB_T2_SYS=y
         [ISDBT]=CONFIG_ISDB_T_SYS=y
         [DVBS]=CONFIG_DVB_S_SYS=y
         [DVBS2]=CONFIG_DVB_S2_SYS=y)
declare -A tuner;
tuner=( [NUTUNE_FT2125]=CONFIG_MSTAR_TUNER_NUTUNE_FT2125=y
       [NUTUNE_FJ2207]=CONFIG_MSTAR_TUNER_NUTUNE_FJ2207=y
       [MXL201RF]=CONFIG_MSTAR_TUNER_MXL201RF=y
       [AV2012]=CONFIG_MSTAR_TUNER_AV2012=y
       [TD1611ALF]=CONFIG_MSTAR_TUNER_NXP_TD1611ALF=y
       [H252F]=CONFIG_TDVG_TUNER_H252F=y
       [DIBCOM_DIB8096]=CONFIG_MSTAR_TUNER_DIBCOM_DIB8096=y
       [NXP_CD1616LF]=CONFIG_MSTAR_TUNER_NXP_CD1616LF=y
       [MXL_MXL5007]=CONFIG_MSTAR_TUNER_MXL_MXL5007=y
       [THOMSON_DCT70707]=CONFIG_MSTAR_TUNER_THOMSON_DCT70707=y
       [MXL603]=CONFIG_MSTAR_TUNER_MXL_MXL603=y
       [MXL608]=CONFIG_TUNER_MXL608=y
       [RT820T]=CONFIG_MSTAR_TUNER_RT820T=y
       [ERROR]=CONFIG_MSTAR_TUNER_ERROR=y)
declare -A demod;
demod=( [MSB1236C]=CONFIG_MSTAR_MSB1236C_DEMOD=y
       [MSB123x]=CONFIG_MSTAR_MSB123x_DEMOD=y
       [MSB122x]=CONFIG_MSTAR_MSB122x_DEMOD=y
       [MSB124x]=CONFIG_MSTAR_MSB124x_DEMOD=y
       [DIB8096]=CONFIG_DIBCOM_DIB8096_DEMOD=y
       [LGDT3305]=CONFIG_LGDT3305_DEMOD=y
       [MSB1400]=CONFIG_MSTAR_MSB1400_DEMOD=y
       [MSB131X]=CONFIG_MSTAR_MSB131X_DEMOD=y
       [KRONUS_INTERNAL]=CONFIG_MSTAR_KRONUS_INTERNAL_DEMOD=y
       [KAPPA_INTERNAL]=CONFIG_MSTAR_KAPPA_INTERNAL_DEMOD=y
       [KRITI_INTERNAL]=CONFIG_MSTAR_KRITI_INTERNAL_DEMOD=y
       [URANUS4_INTERNAL]=CONFIG_MSTAR_URANUS4_INTERNAL_DEMOD=y
       [KAISERIN_INTERNAL]=CONFIG_MSTAR_KAISERIN_INTERNAL_DEMOD=y
      )
declare -A frontend;
frontend=([frontend_on]='CONFIG_FRONT_END=y'
          [frontend_off]='# CONFIG_FRONT_END is not set')
declare -A OAD;
OAD=([ON]='CONFIG_OAD=y'
     [OFF]='# CONFIG_OAD is not set')
declare -A OAD_IN_MBOOT;
OAD_IN_MBOOT=([ON]='CONFIG_OAD_IN_MBOOT=y'
              [OFF]='# CONFIG_OAD_IN_MBOOT is not set')
declare -A SUPERNOVA_OAD;
SUPERNOVA_OAD=([ON]='CONFIG_SUPERNOVA_OAD=y'
               [OFF]='# CONFIG_SUPERNOVA_OAD is not set')
frontend_type=(DVBC INTERNAL_DVBC DVBT INTERNAL_DVBT DVBT2 INTERNAL_DVBT2 DVBS DVBS2 INTERNAL_DVBS INTERNAL_DVBS2 ISDBT INTERNAL_ISDBT DTMB)
function auto(){
  if [[ $1 == ${chip[k1]} ]]
    then
      cp .config.k1.hb.ecos scripts/defconfig
#change board
      for var in ${board_k1[@]}
        do
          sed -i 's/'"$var"'/# '"${var%=*}"' is not set/g' scripts/defconfig
        done
      sed -i 's/# '"${2%=*}"' .*/'"$2"'/g' scripts/defconfig
  elif [[ $1 == ${chip[kappa]} ]]
    then
      cp .config.kappa.hb.ecos scripts/defconfig
#change board
      for var in ${board_kappa[@]}
        do
          sed -i 's/'"$var"'/# '"${var%=*}"' is not set/g' scripts/defconfig
        done
      sed -i 's/# '"${2%=*}"' .*/'"$2"'/g' scripts/defconfig

  elif [[ $1 == ${chip[keltic]} ]]
    then
      cp .config.keltic.hb.ecos scripts/defconfig
#change board
      for var in ${board_keltic[@]}
        do
          sed -i 's/'"$var"'/# '"${var%=*}"' is not set/g' scripts/defconfig
        done
      sed -i 's/# '"${2%=*}"' .*/'"$2"'/g' scripts/defconfig
  elif [[ $1 == ${chip[kriti]} ]]
    then
      cp .config.kriti.hb.ecos scripts/defconfig
#change board
      for var in ${board_kriti[@]}
        do
          sed -i 's/'"$var"'/# '"${var%=*}"' is not set/g' scripts/defconfig
        done
      sed -i 's/# '"${2%=*}"' .*/'"$2"'/g' scripts/defconfig
   elif [[ $1 == ${chip[keres]} ]]
    then
      cp .config.keres.hb.ecos scripts/defconfig
#change board
      for var in ${board_keres[@]}
        do
          sed -i 's/'"$var"'/# '"${var%=*}"' is not set/g' scripts/defconfig
        done
      sed -i 's/# '"${2%=*}"' .*/'"$2"'/g' scripts/defconfig
  fi
#change memory freq
for var in ${mem_freq[@]}
    do
      sed -i 's/'"$var"'/# '"${var%=*}"' is not set/g' scripts/defconfig
    done
 sed -i 's/# '"${3%=*}"'.*/'"$3"'/g' scripts/defconfig
#change DDR SIZE
for var in ${ddr_size[@]}
    do
      sed -i 's/'"$var"'/# '"${var%=*}"' is not set/g' scripts/defconfig
    done
sed -i 's/# '"${4%=*}"' .*/'"$4"'/g' scripts/defconfig
#change mmap
for var in ${mmap[@]}
    do
      sed -i 's/'"$var"'/# '"${var%=*}"' is not set/g' scripts/defconfig
    done
 sed -i 's/# '"${5%=*}"' .*/'"$5"'/g' scripts/defconfig
#change dtv_sys,demod,tuner choice
FRONTEND_ON=$(sed -n '/'"${frontend[frontend_on]}"'/p' scripts/defconfig) 
if [[ $FRONTEND_ON == ${frontend[frontend_on]} ]] && [[ $6 == ${frontend[frontend_on]} ]]
   then
#change dtv system
       for var in ${dtv_sys[@]}
         do
           sed -i 's/'"$var"'/# '"${var%=*}"' is not set/g' scripts/defconfig
         done
       sed -i 's/# '"${8%=*}"'.*/'"$8"'/g' scripts/defconfig
#change tuner
       for var in ${tuner[@]}
         do
           sed -i 's/'"$var"'/# '"${var%=*}"' is not set/g' scripts/defconfig
         done
       sed -i 's/# '"${9%=*}"'.*/'"$9"'/g' scripts/defconfig
#change demod
       for var in ${demod[@]}
         do
           sed -i 's/'"$var"'/# '"${var%=*}"' is not set/g' scripts/defconfig
         done
         DEMOD=${10}
       sed -i 's/# '"${DEMOD%=*}"'.*/'"$DEMOD"'/g' scripts/defconfig
elif [[ $FRONTEND_ON == ${frontend[frontend_on]} ]] && [[ $6 == ${frontend[frontend_off]} ]]
   then
#delete dtv system
       sed -i 's/'"${frontend[frontend_on]}"'/'"${frontend[frontend_off]}"'/g' scripts/defconfig
       for var in ${dtv_sys[@]}
         do
           sed -i 's/'"${var}"'/'"# ${var%=*}"' is not set/g' scripts/defconfig
         done  
#delete dtv tuner
       for var in ${tuner[@]}
         do
           sed -i 's/'"${var}"'/'"# ${var%=*}"' is not set/g' scripts/defconfig
         done  
#delete dtv tuner
      for var in ${demod[@]}
         do
           sed -i 's/'"${var}"'/'"# ${var%=*}"' is not set/g' scripts/defconfig
         done  
elif [[ $FRONTEND_ON != ${frontend[frontend_on]} ]] && [[ $6 == ${frontend[frontend_on]} ]]
   then
#change frontend
   sed -i 's/'"${frontend[frontend_off]}"'/'"$6"'/g' scripts/defconfig
#increase dtv_system
   sed -i 's/# '"${8%=*}"'.*/'"$8"'/g' scripts/defconfig
   sed -i 's/# '"${9%=*}"'.*/'"$9"'/g' scripts/defconfig
   DEMOD=${10}
   sed -i 's/# '"${DEMOD%=*}"'.*/'"$DEMOD"'/g' scripts/defconfig
fi
#OAD change
OAD_ON=$(sed -n '/'"${OAD[ON]}"'/p' scripts/defconfig) 
if [[ $OAD_ON == ${OAD[ON]} ]] && [[ $7 == ${OAD[ON]} ]]
  then
  sed -i 's/.*'"${SUPERNOVA_OAD[ON]%=*}"'.*/'"${11}"'/g' scripts/defconfig
  sed -i 's/.*'"${OAD_IN_MBOOT[ON]%=*}"'.*/'"${12}"'/g' scripts/defconfig
elif [[ $OAD_ON == ${OAD[ON]} ]] && [[ $7 == ${OAD[OFF]} ]]
  then
  sed -i 's/'"${OAD[ON]}"'/'"${OAD[OFF]}"'/g' scripts/defconfig
  sed -i 's/.*'"${SUPERNOVA_OAD[ON]%=*}"'.*/'"${SUPERNOVA_OAD[OFF]}"'/g' scripts/defconfig
  sed -i 's/.*'"${OAD_IN_MBOOT[ON]%=*}"'.*/'"${OAD_IN_MBOOT[OFF]}"'/g' scripts/defconfig
  
elif [[ $OAD_ON != ${OAD[ON]} ]] && [[ $7 == ${OAD[ON]} ]]
  then
  sed -i 's/'"${OAD[OFF]}"'/'"${OAD[ON]}"'/g' scripts/defconfig
  sed -i 's/.*'"${SUPERNOVA_OAD[ON]%=*}"'.*/'"${11}"'/g' scripts/defconfig
  sed -i 's/.*'"${OAD_IN_MBOOT[ON]%=*}"'.*/'"${12}"'/g' scripts/defconfig
fi
   rm -f .config
   rm -f include/autoconf.h
if [[ $1 != "keres" ]]
  then
   make defconfig>/dev/null
else
   make defconfig TARGET_UBOOT_VERSION=2>/dev/null
fi
   make clean
   make
}
function make_all(){
echo "change config......"
#parameter check:chip
if [[ " $1 " != $(echo " ${!chip[@]} "|grep -o $" $1 ") ]]
then
  echo "error setting chip=$1,please check"
  exit
fi
#parameter check:frontend_type
if [[ " $2 " != $(echo " ${frontend_type[@]} "|grep -o $" $2 ") ]]
then
  echo "error setting frontend_type=$2,please check"
  exit
fi
#parameter check:board
if [[ " $3 " != $(echo " ${!board_kriti[@]} ${!board_kappa[@]} ${!board_k1[@]} ${!board_keltic[@]} ${!board_keres[@]} "|grep -o " $3 ") ]]
then
  echo "error setting board=$3,please check"
  exit
fi
#parameter check:mem_freq
if [[ " $4 " != $(echo " ${!mem_freq[@]} "|grep -o " $4 ") ]]
then
  echo "error setting mem_freq=$4,please check"
  exit
fi
#parameter check:ddr_size
if [[ " $5 " != $(echo " ${!ddr_size[@]} "|grep -o " $5 ") ]]
then
  echo "error setting ddr size=$5,please check"
  exit
fi
#parameter check:frontend
if [[ " $6 " != $(echo " ${!frontend[@]} "|grep -o " $6 ") ]]
then
  echo "error setting frontend=$6,please check"
  exit
fi
if [[ $1 == "kriti" ]]
  then
     Board=${board_kriti[$3]}
elif [[ $1 == "kappa" ]]
  then
     Board=${board_kappa[$3]}
elif [[ $1 == "keltic" ]]
  then
     Board=${board_keltic[$3]}
elif [[ $1 == "k1" ]]
  then
     Board=${board_k1[$3]}
elif [[ $1 == "keres" ]]
  then
     Board=${board_keres[$3]}
fi
#OAD off
if [[ $6 == "frontend_off" ]]
  then
auto "${chip[$1]}" "$Board" "${mem_freq[$4]}" "${ddr_size[$5]}" "${mmap[$5]}" "${frontend[frontend_off]}" "${OAD[OFF]}" "######" "######" "######" "######" "######"
else
#OAD on
#parameter check:dtv_sys
if [[ " $7 " != $(echo " ${!dtv_sys[@]} "|grep -o " $7 ") ]]
then
  echo "error setting dtv_sys=$7,please check"
  exit
fi
#parameter check:tuner
if [[ " $8 " != $(echo " ${!tuner[@]} "|grep -o " $8 ") ]]
then
  echo "error setting tuner=$9,please check"
  exit
fi
#parameter check:demod
if [[ " $9 " != $(echo " ${!demod[@]} "|grep -o " $9 ") ]]
then
  echo "error setting demod=$9,please check"
  exit
fi
auto "${chip[$1]}" "$Board" "${mem_freq[$4]}" "${ddr_size[$5]}" "${mmap[$5]}" "${frontend[frontend_on]}" "${OAD[ON]}" "${dtv_sys[$7]}" "${tuner[$8]}" "${demod[$9]}" "${SUPERNOVA_OAD[ON]}" "${OAD_IN_MBOOT[ON]}"
fi

mkdir -p bin/bin/$1/$2/HF/Normal/$4
if [[ $1 != kriti ]]
then
cp bin/flash_with_uboot.bin bin/bin/$1/$2/HF/Normal/$4/mboot_BD_$3_$5.bin
else
cp bin/flash_with_uboot.bin bin/bin/$1/$2/HF/Normal/$4/mboot_BD_${3%_DDR*}_$5.bin
fi
}

#######K1######
#DTMB
echo -e "\033[41;37m              K1                 \033[0m"
make_all k1 DTMB MST124A_D01A 1066MHZ 64M frontend_off
make_all k1 DTMB MST124A_D01A 1066MHZ 128M frontend_off
make_all k1 DTMB MST124A_D02A_S 1600MHZ 128M frontend_off
make_all k1 DTMB MST124B_D02A 1600MHZ 128M frontend_off
make_all k1 DTMB MST124C_D02A 1600MHZ 128M frontend_off
make_all k1 DTMB MST124H_D01A 1600MHZ 128M frontend_off
make_all k1 DTMB MST163A_D01A_S 1600MHZ 128M frontend_off
make_all k1 DTMB MST124E_D01A_SZ 1600MHZ 128M frontend_off
#DVBC
cp -r bin/bin/k1/DTMB bin/bin/k1/DVBC 
#DVBS2
cp -r bin/bin/k1/DTMB bin/bin/k1/DVBS2 
#DVBT
cp -r bin/bin/k1/DTMB bin/bin/k1/DVBT 
#DVBT2
cp -r bin/bin/k1/DTMB bin/bin/k1/DVBT2 
#INTERNAL_DVBC
cp -r bin/bin/k1/DTMB bin/bin/k1/INTERNAL_DVBC 
#INTERNAL_DVBT
cp -r bin/bin/k1/DTMB bin/bin/k1/INTERNAL_DVBT 
#ISDBT
cp -r bin/bin/k1/DTMB bin/bin/k1/ISDBT 
######kappa#######
echo -e "\033[41;37m              Kappa                 \033[0m"
#DTMB
make_all kappa DTMB MST173A_D01A 1066MHZ 64M frontend_off
make_all kappa DTMB MST173A_D01A 1066MHZ 128M frontend_off
make_all kappa DTMB MST173B_D01A 1066MHZ 64M frontend_off
make_all kappa DTMB MST173C_D01A 1066MHZ 64M frontend_off
make_all kappa DTMB MST173C_D01A 1066MHZ 128M frontend_off
make_all kappa DTMB MST173A_D01A 1600MHZ 128M frontend_off
make_all kappa DTMB MST173A_D02A 1600MHZ 128M frontend_off
make_all kappa DTMB MST173B_D01A 1600MHZ 128M frontend_off
make_all kappa DTMB MST173C_D01A 1600MHZ 128M frontend_off
#DVBS2
cp -r bin/bin/kappa/DTMB bin/bin/kappa/DVBS2
#DVBT2
cp -r bin/bin/kappa/DTMB bin/bin/kappa/DVBT2
#INTERNAL_DVBC
cp -r bin/bin/kappa/DTMB bin/bin/kappa/INTERNAL_DVBC
#INTERNAL_DVBT
cp -r bin/bin/kappa/DTMB bin/bin/kappa/INTERNAL_DVBT
#INTERNAL_ISDBT
cp -r bin/bin/kappa/DTMB bin/bin/kappa/INTERNAL_ISDBT
######keltic#######
#DTMB
echo -e "\033[41;37m              Keltic                 \033[0m"
make_all keltic DTMB MST183A_D01A 800MHZ 32M frontend_off
make_all keltic DTMB MST183A_D01A 800MHZ 64M frontend_off
make_all keltic DTMB MST183A_D01A_S 800MHZ 32M frontend_off
make_all keltic DTMB MST183A_D01A_S 800MHZ 64M frontend_off
make_all keltic DTMB MST183B_D01A 800MHZ 64M frontend_off
make_all keltic DTMB MST183B_D01A_S 800MHZ 64M frontend_off
make_all keltic DTMB MST183B_D01A 1300MHZ 128M frontend_off
make_all keltic DTMB MST183B_D01A_S 1300MHZ 128M frontend_off
#INTERNAL_DVBC
cp -r bin/bin/keltic/DTMB bin/bin/keltic/INTERNAL_DVBC
#INTERNAL_DVBS
cp -r bin/bin/keltic/DTMB bin/bin/keltic/INTERNAL_DVBS
#DVBS2
cp -r bin/bin/keltic/DTMB bin/bin/keltic/DVBS2
make_all keltic DVBS2 MST183B_D01A 1300MHZ 64M frontend_off
make_all keltic DVBS2 MST183B_D01A_S 1300MHZ 64M frontend_off
#ISDBT
cp -r bin/bin/keltic/DTMB bin/bin/keltic/ISDBT
make_all keltic ISDBT MST183B_D01A 1300MHZ 64M frontend_off
make_all keltic ISDBT MST183B_D01A_S 1300MHZ 64M frontend_off
######kriti#######
#INTERNAL_DVBT2
echo -e "\033[41;37m              Kriti                 \033[0m"
make_all kriti INTERNAL_DVBT2 MST204A_D01A_DDR2 1333MHZ 64M frontend_on DVBT2 MXL603 KRITI_INTERNAL
make_all kriti INTERNAL_DVBT2 MST204A_D01A_S_DDR2 1333MHZ 64M frontend_on DVBT2 MXL603 KRITI_INTERNAL
make_all kriti INTERNAL_DVBT2 MST204A_D01A_DDR2 1600MHZ 128M frontend_on DVBT2 MXL603 KRITI_INTERNAL
make_all kriti INTERNAL_DVBT2 MST204A_D01A_S_DDR2 1600MHZ 128M frontend_on DVBT2 MXL603 KRITI_INTERNAL
#INTERNAL_DVBT
cp -r bin/bin/kriti/INTERNAL_DVBT2 bin/bin/kriti/INTERNAL_DVBT