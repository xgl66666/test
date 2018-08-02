#!/bin/sh
sed -i 's/DDI_DEMO_NAND_ECOS\ =\ disable/DDI_DEMO_NAND_ECOS\ =\ enable/' configurations/feature_configuration
make clean
make
