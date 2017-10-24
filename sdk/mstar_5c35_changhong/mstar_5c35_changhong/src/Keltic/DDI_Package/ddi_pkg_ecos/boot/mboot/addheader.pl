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
if ($arc_cnt<8) {
    print ("\n\n This program usage\n");
    print (" Useage $0 <in_file> <out_file> <App Magic flag> <file len> <App Dram Address> <App Reset Vector Dram Address> <App in SPI Address> <LZMA BUF Address> <LZMA Dec BUF Address>\n\n");
    exit -1;
}
$input_file  = $ARGV[0];
$output_file = $ARGV[1];
$app_magic_flag= $ARGV[2];
$app_codelen= $ARGV[3];
$app_Dram_addr= $ARGV[4];
$app_Rest_addr= $ARGV[5];
$app_SPI_addr= $ARGV[6];
$Lzma_Buf_addr= $ARGV[7];
$Lzma_DecBuf_addr= $ARGV[8];

open (IFILE, "<$input_file") ||
    die("Error: can't open file $input_file \n");
binmode(IFILE);
$length = read(IFILE,$buffer,0x800000);
close (IFILE);
@istream = unpack("C*",$buffer);
# Make sure we got a whole number of words in array
$length = @istream;

if(($length % 4) == 1)
{
    for($i=0;$i<3;$i+=1){
        push (@istream, 0);
    }
}
elsif(($length % 4) == 2)
{
    for($i=0;$i<2;$i+=1){
        push (@istream, 0);
    }
}
elsif(($length % 4) == 3)
{
    for($i=0;$i<1;$i+=1){
        push (@istream, 0);
    }
}

#Add 4 bytes in the end
#for($i=0;$i<4;$i+=1){
#push (@istream, 0);
#}
$length = @istream;
#$length=$length-8;

#
#$app_codelen=$app_codelen-8;
# Data shift 8 bytes right
#for($i=0;$i<8;$i+=1)
#unshift(@istream,0)
for ($i=$length-1; $i>=0; $i-=1) {
    $istream[$i+28]          = $istream[$i];
}
#add header
#<App Magic flag>
$istream[0]=(($app_magic_flag>>24)&0xFF);
$istream[1]=(($app_magic_flag>>16)&0xFF);
$istream[2]=(($app_magic_flag>>8)&0xFF);
$istream[3]=$app_magic_flag&0xff;


##big endian
##if file length is 0x12345678, istream[4]=0x12,istream[5]=0x34,,istream[6]=0x56,istream[5]=0x78
##printf("compress_flag is %d ,%d\n",$compress_flag,$ARGV[2]);
#<file len>
$istream[4]=(($app_codelen>>24)&0xFF);
$istream[5]=(($app_codelen>>16)&0xFF);
$istream[6]=(($app_codelen>>8)&0xFF);
$istream[7]=$app_codelen&0xff;#0x34);

#<App Dram Address>
$istream[8]=(($app_Dram_addr>>24)&0xFF);
$istream[9]=(($app_Dram_addr>>16)&0xFF);
$istream[10]=(($app_Dram_addr>>8)&0xFF);
$istream[11]=$app_Dram_addr&0xff;#0x34);


#<App Reset Vector Dram Address>
$istream[12]=(($app_Rest_addr>>24)&0xFF);
$istream[13]=(($app_Rest_addr>>16)&0xFF);
$istream[14]=(($app_Rest_addr>>8)&0xFF);
$istream[15]=$app_Rest_addr&0xff;#0x34);
#<App in SPI Address>
$istream[16]=(($app_SPI_addr>>24)&0xFF);
$istream[17]=(($app_SPI_addr>>16)&0xFF);
$istream[18]=(($app_SPI_addr>>8)&0xFF);
$istream[19]=$app_SPI_addr&0xff;#0x34);
#<LZMA BUF Address>
$istream[20]=(($Lzma_Buf_addr>>24)&0xFF);
$istream[21]=(($Lzma_Buf_addr>>16)&0xFF);
$istream[22]=(($Lzma_Buf_addr>>8)&0xFF);
$istream[23]=$Lzma_Buf_addr&0xff;#0x34);
#<LZMA Dec BUF address>
$istream[24]=(($Lzma_DecBuf_addr>>24)&0xFF);
$istream[25]=(($Lzma_DecBuf_addr>>16)&0xFF);
$istream[26]=(($Lzma_DecBuf_addr>>8)&0xFF);
$istream[27]=$Lzma_DecBuf_addr&0xff;#0x34);



#$ Let's pack the new stream and write it to the output file.
$ostream = pack("C*", @istream);
open (OFILE, ">$output_file") ||
    die("Error: can't open file $output_file \n");
binmode(OFILE);
print (OFILE $ostream);
close (OFILE);
#end of file

