1.把BintoDat.exe和要換的圖片放在同一個路徑下，運行exe,輸入圖片的名字（包括後綴名），
enter鍵之後會生成一個.dat
 
2.把這個dat名字改為Mstar_logo.dat，并替換u-boot-1.1.6\display_logo下
的同名文件。
 
3.重新build mboot
 


Note:
如果圖片過大會出現更換后會編譯有overlap錯誤的問題,如下：
mips-linux-gnu-ld: section .text [0000000087610000 -> 00000000876834ab]
overlaps section .logo [00000000875f2000 -> 00000000876216df]
解決辦法：
1.降低圖片質量,也就是使.logo的結束地址減小,避免和.text的開始地址overlap
2.一般是.logo 把.text overlap了，所以可以把.txt的起始地址後移一些，例如：
修改\MBoot_STB\u-boot-1.1.6\board\kappa\config.mk中的
TEXT_BASE = 0x87610000 ------>改為 TEXT_BASE = 0x876216E0
