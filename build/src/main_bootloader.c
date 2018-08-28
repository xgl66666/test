/******************************************************************************
 *                         COPYRIGHT 2006 IWEDIA TECHNOLOGIES                 *
 ******************************************************************************
 *
 * MODULE NAME: SYS
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/product/comedia/branches/idecode_hd/build/src/main_bootloader.c $
 *            $Rev: 332 $
 *            $Date: 2011-09-07 13:51:23 +0200 (Wed, 07 Sep 2011) $
 *
 * DESCRIPTION : 
 *
 *****************************************************************************/

#include "crules.h"
#include "tbox.h"
#include "tkel.h"

#include "bootloader_cfg.h"

extern void BTL_Loader(void);

int main(int argc, char *argv[])
{
	
#ifdef __DEBUG_TRACE__
    TRACE_CFG_Init();
#endif

    /**--------------------------**
     *     Init TDAL drivers      *
     * ( Init low level drivers ) *
     **--------------------------**/
    TDAL_Init();


    /**--------------------------**
     *     Init TKEL drivers      *
     **--------------------------**/
    TKEL_Init();

    /**--------------------------**
     *     Launch Bootloader      *
     **--------------------------**/

    BTL_Loader();
	
    return 0;
}
