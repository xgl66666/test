makerootfs source URL => http://makerootfs.sourceforge.net/

makescript/ =>  	shell scripts, which download, cross-compile and install selected packages into staging directory
patches/    =>    contains patches, configfiles and initscripts that do not come with the original software package

original script step as below
1build-toolchain.sh 	    =>build a toolchain first using crosstool (only if no toolchain is found in PATH)
2build-rootfs_staging.sh 	=>run selected scripts (MYMAKE_LIST) in makescripts/, configured in makerootfs.conf
3create-rootfs-fromstaging.sh =>create skeleton root filesystem from patches/rootfs-skel.tar.gz and copy binaries (libraries, executables, config files, ...) into final root filesystem
4create-rootfs-toolchainlibraries.sh =>copy libraries from current toolchain into final root filesystem
5create-rootfs-kernelmodules.sh =>copy kernel modules into final root filesystem
6create-jffs2-fromrootfs.sh =>create final jffs2 including device nodes and the files in rootfs/

modify for Mstar step as below
1. run make in DDI Makefile   =>untar ROOTFS.tar.gz
2. run make create_rootfs_staging in DDI Makefile 	=>create rootfs_staging and run selected scripts (MYMAKE_LIST) in makescripts/, configured in makerootfs.conf
3. run make create_rootfs_fromstaging in DDI Makefile   =>copy rootfs_staging to ROOTFS into final root filesystem
4. run make build_rootfs in DDI Makefile  =>execute autogen.sh to build rootfs image