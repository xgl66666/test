1.��BintoDat.exe��Ҫ�Q�ĈDƬ����ͬһ��·���£��\��exe,ݔ��DƬ�����֣�������Y������
enter�I֮�������һ��.dat
 
2.���@��dat���ָĞ�Mstar_logo.dat������Qu-boot-1.1.6\display_logo��
��ͬ���ļ���
 
3.����build mboot
 


Note:
����DƬ�^������F���Q������g��overlap�e�`�Ć��},���£�
mips-linux-gnu-ld: section .text [0000000087610000 -> 00000000876834ab]
overlaps section .logo [00000000875f2000 -> 00000000876216df]
��Q�k����
1.���͈DƬ�|��,Ҳ����ʹ.logo�ĽY����ַ�pС,�����.text���_ʼ��ַoverlap
2.һ����.logo ��.text overlap�ˣ����Կ��԰�.txt����ʼ��ַ����һЩ�����磺
�޸�\MBoot_STB\u-boot-1.1.6\board\kappa\config.mk�е�
TEXT_BASE = 0x87610000 ------>�Ğ� TEXT_BASE = 0x876216E0
