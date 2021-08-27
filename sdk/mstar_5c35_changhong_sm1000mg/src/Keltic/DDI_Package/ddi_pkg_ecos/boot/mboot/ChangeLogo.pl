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
#   File Name: PadingData.pl
#
#   Perl script used for Pading Data in the end of input file.
#
#   #####################################################################

$arc_cnt = @ARGV;
if ($arc_cnt<2) {
    print ("\n\n This program usage\n");
    print (" Useage $0 <in_file> <logo_file> <out_file>\n\n");
    exit -1;
}
$input_file  = $ARGV[0];
$logo_file = $ARGV[1];
$output_file= $ARGV[2];

open (IFILE, "<$input_file") ||
    die("Error: can't open file $input_file \n");
binmode(IFILE);
$length = read(IFILE,$buffer,0x800000);
close (IFILE);


@istream = unpack("C*",$buffer);
# Make sure we got a whole number of words in array
$length = @istream;

$logo_start = 0;
# 0x20020 ~ 0x20023 --> 131104 ~ 131107
$logo_start = ($istream[131104]&0xFF);
$logo_start = $logo_start | (($istream[131105]&0xFF) << 8);
$logo_start = $logo_start | (($istream[131106]&0x0F) << 16);



open (IFILE, "<$input_file") ||
    die("Error: can't open file $input_file \n");
binmode(IFILE);
$length = read(IFILE,$buffer,$logo_start);
close (IFILE);

@istream = unpack("C*",$buffer);


#$ Let's pack the new stream and write it to the output file.
$ostream = pack("C*", @istream);
open (OFILE, ">$output_file") ||
    die("Error: can't open file $output_file \n");
binmode(OFILE);
print (OFILE $ostream);



#########################################################################
#
#   Align logo to 0x10000 and pading to outfile.
#
#########################################################################




open (IFILE, "<$logo_file") ||
    die("Error: can't open file $input_file \n");
binmode(IFILE);
$length = read(IFILE,$buffer,0x800000);
close (IFILE);

@istream = unpack("C*",$buffer);
# Make sure we got a whole number of words in array
$logo_len = @istream;

$Pading_datalen = ($logo_len + 65535);
$Pading_datalen = (($Pading_datalen >> 16) << 16);
$Pading_datalen = $Pading_datalen - $logo_len;

for($i=0;$i<$Pading_datalen;$i+=1){
push (@istream, 0x00);
}


#$ Let's pack the new stream and write it to the output file.
$ostream = pack("C*", @istream);

print (OFILE $ostream);


close (OFILE);

#=====================================================================

open (IFILE, "<$input_file") ||
    die("Error: can't open file $input_file \n");
binmode(IFILE);
$length = read(IFILE,$buffer,0x800000);
close (IFILE);


@istream = unpack("C*",$buffer);
# Make sure we got a whole number of words in array
$length = @istream;

$app_start = 0;
# 0x20024 ~ 0x20027 --> 131108 ~ 131111
$app_start = ($istream[131108]&0xFF);
$app_start = $app_start | (($istream[131109]&0xFF) << 8);
$app_start = $app_start | (($istream[131110]&0x0F) << 16);


$app_start = ($app_start + 65535);
$app_start = (($app_start >> 16) << 16);



# the last 4 byte is CRC
for($i=$app_start;$i<$length-4;$i+=1){
    $app_content[$i-$app_start] =  ($istream[$i]&0xFF);
}

$app_len = $length - 4 - $app_start;

open (IFILE, "<$output_file") ||
    die("Error: can't input file $output_file \n");
binmode(IFILE);
$length = read(IFILE,$buffer,0x800000);
close (IFILE);

@istream = unpack("C*",$buffer);
# Make sure we got a whole number of words in array
$app_start = @istream;


for($i=0;$i<$app_len;$i+=1){
push (@istream, $app_content[$i]);
}
$length = @istream;

$logo_modified = 0xBFC00000;
$logo_modified = $logo_modified | ($logo_start + $logo_len);


# 0x20024 ~ 0x20027 --> 131108 ~ 131111
$istream[131108] = ($logo_modified & 0xFF);
$istream[131109] = (($logo_modified>> 8) & 0xFF);
$istream[131110]= (($logo_modified>> 16) & 0xFF);
$istream[131111]= (($logo_modified>> 24) & 0xFF);





$istream[$app_start + 0x10] = 0x00;
$istream[$app_start + 0x11] = ((($app_start + 28)>> 16) & 0xFF);
$istream[$app_start + 0x12]= ((($app_start + 28)>> 8) & 0xFF);
$istream[$app_start + 0x13]= (($app_start + 28) & 0xFF);

$app_start = $app_start + 28;


#$ Let's pack the new stream and write it to the output file.
$ostream = pack("C*", @istream);
open (OFILE, ">$output_file") ||
    die("Error: can't open file $output_file \n");
binmode(OFILE);
print (OFILE $ostream);
close (OFILE);

#end of file

