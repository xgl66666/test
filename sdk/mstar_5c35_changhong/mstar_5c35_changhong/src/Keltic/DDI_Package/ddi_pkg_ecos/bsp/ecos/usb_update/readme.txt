1. copy Perforce\THEALE\Uranus\K1\DDI_Package\ddi_pkg_ecos\bsp\ecos\tftp_root\usb_update\Target folder to USB
2. copy updated bin(zip) file to Target\image
3. replace Target\script\parameters.txt with Perforce\THEALE\Uranus\K1\DDI_Package\ddi_pkg_ecos\bsp\ecos\tftp_root\parameters.txt
3. insert USB and reboot
4. enter cmd -->  usb start
5. enter cmd -->  ustar usb_auto_update.txt
6. remove USB after successfully update