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
    print (" Useage $0 <in_file> <out_file> <Pading data len>\n\n");
    exit -1;
}
$input_file  = $ARGV[0];
$output_file = $ARGV[1];
$Pading_codelen= $ARGV[2];

open (IFILE, "<$input_file") ||
    die("Error: can't open file $input_file \n");
binmode(IFILE);
$length = read(IFILE,$buffer,0x800000);
close (IFILE);
@istream = unpack("C*",$buffer);
# Make sure we got a whole number of words in array
$length = @istream;

#$Pading_codelen = ($length + 65535);
#$Pading_codelen = (($Pading_codelen >> 16) << 16);
#$Pading_codelen = $Pading_codelen - $length;

#Add 4 bytes in the end
for($i=0;$i<$Pading_codelen;$i+=1){
push (@istream, 0x00);
}
$length = @istream;



#$ Let's pack the new stream and write it to the output file.
$ostream = pack("C*", @istream);
open (OFILE, ">$output_file") ||
    die("Error: can't open file $output_file \n");
binmode(OFILE);
print (OFILE $ostream);
close (OFILE);
#end of file

