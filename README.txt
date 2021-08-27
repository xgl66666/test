==========================================================================
How to build target :
==========================================================================
1 build DALTS :
    make SETENV=mstar_5C35_changhong_sm1000mg_maestro_base_sd_merlin_dalts sdk comedia

2 build NAK REL:
    make SETENV=mstar_5C35_changhong_sm1000mg_maestro_base_sd_merlin_nak_rel sdk comedia
	
3 build NAK DBG:
	make SETENV=mstar_5C35_changhong_sm1000mg_maestro_base_sd_merlin_nak_dbg sdk comedia

4 build CHAL Validator:
    make SETENV=mstar_5C35_changhong_sm1000mg_maestro_base_sd sdk chalvalidator
	
5 build GTPL production software:
    make SETENV=mstar_5C35_changhong_sm1000mg_maestro_gtpl_sd_merlin_prod_rel sdk comedia

==========================================================================
How to run from USB:
==========================================================================

1) Copy the file bin to USB disk and plug to the board.
	  
2) Write file to flash

- CHAL validator (from USB)
usb start 0; spi_rdc 0x83FFA000 0x50000 0x002000; fatload usb 0 0x82F70000 chalvalidator_lzma_ota.bin; go 0x82F70044;

- Application (from USB, note that image copy must be in flash also)
usb start 0; spi_rdc 0x83FFA000 0x50000 0x002000; fatload usb 0 0x82F70000 comedia_lzma_ota.bin; go 0x82F70044;

==========================================================================
How to run Application from SPI flash memory :
==========================================================================
1) Flash image 
usb start 0; fatload usb 0 0x80000000 comedia_lzma_ota.bin; spi_wrc 0x80000000 0x120000 $(filesize);

2) Flash PK
a) For production libraries
usb start 0; spi_rdc 0x80000000 0x50000 0x010000; fatload usb 0 0x80000000 PK_Gujarat_000K0256.D33; spi_wrc 0x80000000 0x50000 0x010000;
b) For certification testing
usb start 0; spi_rdc 0x80000000 0x50000 0x010000; fatload usb 0 0x80000000 PK_CSO_TEST_PK_2005_10000.D33; spi_wrc 0x80000000 0x50000 0x010000;

3) Flash CSCD
usb start 0; fatload usb 0 0x80000000 D020_XXXXXX.BIN; spi_wrc 0x80000000 0x7d0000 $(filesize);

4) Boot image directly from flash
spi_rdc 0x83FFA000 0x50000 0x002000; spi_rdc 0x82F70000 0x120000 400000;go 0x82F70044;

*) Prepare auto-boot option 
setenv bootcmd "spi_rdc 0x83FFA000 0x50000 0x002000; spi_rdc 0x82F70000 0x120000 400000;go 0x82F70044;"
seveenv
reset

==========================================================================
Others :
==========================================================================
1 TRACES=UART will enabled log
2 DEBUG=1 will enabled debug symbols



 