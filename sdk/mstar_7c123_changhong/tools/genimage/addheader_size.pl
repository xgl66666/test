################################################################################
#
# Copyright (c) 2006-2007 MStar Semiconductor, Inc.
# All rights reserved.
#
# Unless otherwise stipulated in writing, any and all information contained
# herein regardless in any format shall remain the sole proprietary of
# MStar Semiconductor Inc. and be kept in strict confidence
# (¡§MStar Confidential Information¡¨) by the recipient.
# Any unauthorized act including without limitation unauthorized disclosure,
# copying, use, reproduction, sale, distribution, modification, disassembling,
# reverse engineering and compiling of the contents of MStar Confidential
# Information is unlawful and strictly prohibited. MStar hereby reserves the
# rights to any and all damages, losses, costs and expenses resulting therefrom.
#

#########################################################################
#
#   File Name: addheader.pl
#
#   Perl script used for adding Compressing flag and file length at the start of file.
#   App Magic flag(4 bytes)+the lengthe of file uraus.zip/compression flag(4 bytes)+Uranus.zip
#   #####################################################################

$arc_cnt = @ARGV;
if ($arc_cnt<2) {
    print ("\n\n This program usage\n");
    print (" Useage $0 <in_file> <out_file> <file len> \n\n");
    exit -1;
}
$input_file  = $ARGV[0];
$output_file = $ARGV[1];
$app_codelen = $ARGV[2];


open (IFILE, "<$input_file") ||
    die("Error: can't open file $input_file \n");
binmode(IFILE);
$length = read(IFILE,$buffer,-s $input_file);
close (IFILE);
@istream = unpack("C*",$buffer);
# Make sure we got a whole number of words in array

$length = @istream;

#
#shift the original stream back
for ($i=$length-1; $i>=0 ; $i-=1) {
     $istream[$i+4] = $istream[$i];
}

#image size
$istream[0]=(($app_codelen>>24)&0xFF);
$istream[1]=(($app_codelen>>16)&0xFF);
$istream[2]=(($app_codelen>>8)&0xFF);
$istream[3]=$app_codelen&0xff;


#$ Let's pack the new stream and write it to the output file.
$ostream = pack("C*", @istream);
open (OFILE, ">$output_file") ||
    die("Error: can't open file $output_file \n");
binmode(OFILE);
print (OFILE $ostream);
close (OFILE);
#end of file

