# Copyright (c) 2008-2011 MStar Semiconductor, Inc.
# All rights reserved.

$2 == "ENABLE_MSTAR_URANUS4" { if($3==1) system("cd ../u-boot-1.1.6/;make -s uranus4_config;make -s")}
$2 == "ENABLE_MSTAR_KRONUS" { if($3==1) system("cd ../u-boot-1.1.6/;make -s kronus_config;make -s")}
$2 == "ENABLE_MSTAR_KAISERIN" { if($3==1) system("cd ../u-boot-1.1.6/;make -s kaiserin_config;make -s")}
$2 == "ENABLE_MSTAR_KAISER" { if($3==1) system("cd ../../../MBoot/u-boot-2011.06/;make kaiser_config;make -s")}
$2 == "ENABLE_MSTAR_KAPPA" { if($3==1) system("cd ../u-boot-1.1.6/;make -s kappa_config;make -s")}
$2 == "ENABLE_MSTAR_KELTIC" { if($3==1) system("cd ../u-boot-1.1.6/;make -s keltic_config;make -s")}
