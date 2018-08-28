*****************
environnement.mak includes
	tools.mak
		-> Tools + Options
	product.mak
		-> Product Description
	generic.mak
		-> Compilation flags for the product component except driver
	mapping_flash.mak
		-> Flash Mapping (needs to be replaced by config C file)
	keyboard.mak
		-> RC indentifier (needs to be replaced by config C file)
	AVS.mak
		-> AVS identifier (needs to be replaced by config C file)
	ca.mak
		-> Ca identifier
*****************
crules.mak
	-> Compilation rules included by C makefiles

makelib.mak
	-> compile paths to generates librairies
	
tdal.mak
	-> Which TDAL driver are needed for this product

*****************
buildexe.mak
	-> Librairies lists needed to create on each exe files

drivers.mak
	-> Compilation flags needed to compile low level components uppon drivers
	
******************
setdriverenv.bat
	-> Environnement to compile drivers
