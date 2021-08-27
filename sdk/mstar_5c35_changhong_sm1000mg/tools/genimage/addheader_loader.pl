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
$magic_id = 0x11234;
$version_1 =0x0;
$version_2 =0x0;
$version_3 =0x0;
$version_4 =0x1;

$arc_cnt = @ARGV;
if ($arc_cnt<3) {
    print ("\n\n This program usage\n");
    print (" Useage $0 <in_file> <out_file> <file len> <file checksum>\n\n");
    exit -1;
}
$input_file  = $ARGV[0];
$output_file = $ARGV[1];
$app_codelen = $ARGV[2];
$crc_file = $ARGV[3];

open (CRCFILE, "<$crc_file") ||
    die("Error: can't open file $crc_file \n");
binmode(CRCFILE);
$CRClength = read(CRCFILE,$CRCbuffer,0x10);
close (CRCFILE);
@CRCstream = unpack("C*",$CRCbuffer);

open (IFILE, "<$input_file") ||
    die("Error: can't open file $input_file \n");
binmode(IFILE);
$length = read(IFILE,$buffer,0x2200000);
close (IFILE);
@istream = unpack("C*",$buffer);
# Make sure we got a whole number of words in array

$length = @istream;

#magic ID
$istream[3]=(($magic_id>>24)&0xFF);
$istream[2]=(($magic_id>>16)&0xFF);
$istream[1]=(($magic_id>>8)&0xFF);
$istream[0]=$magic_id&0xff;

#version 1~ 4
$istream[7]=(($version_1>>24)&0xFF);
$istream[6]=(($version_1>>16)&0xFF);
$istream[5]=(($version_1>>8)&0xFF);
$istream[4]=$version_1&0xff;

$istream[11]=(($version_2>>24)&0xFF);
$istream[10]=(($version_2>>16)&0xFF);
$istream[9]=(($version_2>>8)&0xFF);
$istream[8]=$version_2&0xff;

$istream[15]=(($version_3>>24)&0xFF);
$istream[14]=(($version_3>>16)&0xFF);
$istream[13]=(($version_3>>8)&0xFF);
$istream[12]=$version_3&0xff;

$istream[19]=(($version_4>>24)&0xFF);
$istream[18]=(($version_4>>16)&0xFF);
$istream[17]=(($version_4>>8)&0xFF);
$istream[16]=$version_4&0xff;

#image size
$istream[23]=(($app_codelen>>24)&0xFF);
$istream[22]=(($app_codelen>>16)&0xFF);
$istream[21]=(($app_codelen>>8)&0xFF);
$istream[20]=$app_codelen&0xff;

for ($i=0; $i< 36 ; $i+=1) {
    $istream[$i+24] = 0;
}

#checksum
$istream[60]=$CRCstream[0];
$istream[61]=$CRCstream[1];
$istream[62]=$CRCstream[2];
$istream[63]=$CRCstream[3];

#$ Let's pack the new stream and write it to the output file.
$ostream = pack("C*", @istream);
open (OFILE, ">$output_file") ||
    die("Error: can't open file $output_file \n");
binmode(OFILE);
print (OFILE $ostream);
close (OFILE);
#end of file

