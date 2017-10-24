/********************************************************************************
 *                       COPYRIGHT 2006 IWEDIA                                  *
 ********************************************************************************
 *
 * MODULE NAME:
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/product/comedia/branches/idecode_hd/build/src/integrity_cfg.c $
 *            $Rev: 332 $
 *            $Date: 2011-09-07 13:51:23 +0200 (Wed, 07 Sep 2011) $
 *
 *
 *******************************************************************************/
#define kNTGR_MAX_CHECKED_FLASH_BLOCK_COUNT   128
#define kNTGR_MAX_CHECKED_FLASH_BLOCK_SIZE   (128*1024)

#define kNTGR_SSW_SEGMENT_BASE 				0X100
#define kNTGR_SSW_AREA_SIZE 				sizeof(NTGR_ssw_info_t)

/********************************************************/
/*                Includes                              */
/********************************************************/
#include "crules.h"
#include "integrity_cfg.h"
#include "btl_crc.h"
/*
#include "btl_util.h"
*/
#include "tdal_fla.h"
/********************************************************/
/*              Defines                                 */
/********************************************************/
mTBOX_SET_MODULE(eBTL_NTGR);


/********************************************************/
/*              Local Functions Declarations (LOCAL)    */
/*******************************************************/
LOCAL tFLASH_CFG_Location	gsAreaLocation;

LOCAL NTGR_ssw_info_t gsSwInfoInNVM;
LOCAL uint8_t gModuleBuffer[kNTGR_MAX_CHECKED_FLASH_BLOCK_SIZE];

/********************************************************/
/*              Functions Definitions (LOCAL/GLOBAL)    */
/********************************************************/

/******************************************************************************
 * Function Name : NTGR_ComputeAndWriteSSWInfo
 *
 * Description   : Check if the System Software in flash is the first one
 *                 If first word of image_header = 1, then it is the first one.
 *
 * Side effects  :
 *
 * Returns:
 *           NTGR_FLASH_CRC_FAILED    :   a module is corrupted
 *           NTGR_MEMORY_ALLOC_FAILED :   memory allocate error
 *           NTGR_FLASH_READ_FAILED   :   read error in flash
 *           NTGR_NO_ERROR            :   no error
 *
 *****************************************************************************/
NTGR_error_code_t NTGR_ComputeAndWriteSSWInfo(void)
{
    tFLASH_CFG_FLASH_Area ePartition;
    uint16_t			uArea1stBlock;
    uint16_t			uAreaLastBlock;
    uint16_t			uAreaBlockCount;
    uint16_t			uSSWCursor;
    uint16_t            uCpt_block;
    uint32_t			uBlockAddress;			
    uint32_t			uBlockSize;			
    uint32_t            uCRC32Calculated;
    uint32_t 			uReadCount;
    uint32_t 			uWroteByte;

    mTBOX_FCT_ENTER("NTGR_ComputeAndWriteSSWInfo");

    uSSWCursor=0;

    /* for each partition */
    for (ePartition = 0; ePartition < eFLASH_CFG_FLASH_NB_AREA; ePartition++)
    {
    	if (BTL_CFG_GetPartitionKind(ePartition) == eBTL_UPGRADABLE_PARTITION)
    	{
	    	if (!FLASH_CFG_GetFlashAreaLocation(ePartition, &gsAreaLocation))
	    	{
/*	    		printf("NTGR_ComputeAndWriteSSWInfo can not get configuration for partition %d \n",ePartition);*/
	    		continue;
	    	}
	    	if (gsAreaLocation.size == 0)
	    	{
/*	    		printf("NTGR_ComputeAndWriteSSWInfo empty partition %d \n",ePartition);*/
	    		continue;
	    	}
	    	
		    uArea1stBlock = TDAL_FLA_GetBlockNumber(gsAreaLocation.addressOffset);
		    uAreaLastBlock = TDAL_FLA_GetBlockNumber(gsAreaLocation.addressOffset+gsAreaLocation.size -4); /* -4 is to be inside the last block*/
		    uAreaBlockCount = uAreaLastBlock - uArea1stBlock +1;

		    /* loop for all partition flash blocks */
		    for (uCpt_block = 0; uCpt_block < uAreaBlockCount; uCpt_block++)
		    {

		        /* Update the size and start address module */
		        uBlockAddress = TDAL_FLA_GetBlockAddress(uArea1stBlock+uCpt_block);
		        uBlockSize = TDAL_FLA_GetBlockSize(uArea1stBlock+uCpt_block);
		
		        /* reading of the module in flash */
		        uReadCount = TDAL_FLA_Read(uBlockAddress, gModuleBuffer, uBlockSize);

		        if ( uReadCount != uBlockSize)
		        {
/*		        	printf("NTGR_ComputeAndWriteSSWInfo Unable to read flach block @0x%x for 0x%x size 0x%x\n",uBlockAddress,uBlockSize,uReadCount);*/
		        	continue;
		        }
		
		        /* compute CRC module */
		        BTL_CRC_crc32Calculate(gModuleBuffer, uBlockSize, &uCRC32Calculated);
/*
 printf("NTGR_ComputeAndWriteSSWInfo block %d size=%d [ %x %x %x %x %x %x] @%#x CRC=%#x\n",
    uArea1stBlock+uCpt_block,uReadCount,
    gModuleBuffer[0],gModuleBuffer[1],gModuleBuffer[2],gModuleBuffer[3],gModuleBuffer[4],gModuleBuffer[5],
    uBlockAddress,uCRC32Calculated);
*/		
		        /* update the SSW block entry */
		        gsSwInfoInNVM.moduleInfoArray[uSSWCursor].startAddress = uBlockAddress;
		        gsSwInfoInNVM.moduleInfoArray[uSSWCursor].moduleSize = uBlockSize;
		        gsSwInfoInNVM.moduleInfoArray[uSSWCursor].CRC32 = uCRC32Calculated;
		        gsSwInfoInNVM.moduleInfoArray[uSSWCursor].filler1 = uArea1stBlock+uCpt_block;
		        uSSWCursor++;
		
		
			}
		}
/*
		else
			printf( "Partition %d not upgradable\n",ePartition);
*/
	} /* end for (ePartition = 0; ePartition < eFLASH_CFG_FLASH_NB_AREA; ePartition++) */
	
	gsSwInfoInNVM.nbOfModules=uSSWCursor ;
	gsSwInfoInNVM.filler12[0]=0xABCDEF00 ; /* this pattern is only ti allign structure */
	gsSwInfoInNVM.filler12[1]=0xABCDEF00 ;
	gsSwInfoInNVM.filler12[2]=0xABCDEF00 ;

     /* compute CRC32 of SSW area */
    BTL_CRC_crc32Calculate((uint8_t*) &gsSwInfoInNVM, kNTGR_SSW_PAYLOAD_SIZE, &uCRC32Calculated );
	gsSwInfoInNVM.CRC32=uCRC32Calculated;

	uWroteByte = TDAL_NVM_Write(kNTGR_SSW_SEGMENT_BASE, kNTGR_SSW_AREA_SIZE, (uint8_t*) &gsSwInfoInNVM);
    if (uWroteByte != kNTGR_SSW_AREA_SIZE )
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_NVM_Write has failed\n"));
    }

     mTBOX_RETURN(NTGR_NO_ERROR);
}

/******************************************************************************
 * Function Name : NTGR_CheckIfFirstSSW
 *
 * Description   : Check if the System Software in flash is the first one
 *
 * Side effects  :
 *
 * Returns:
 *           NTGR_FLASH_CRC_FAILED    :   a module is corrupted
 *           NTGR_MEMORY_ALLOC_FAILED :   memory allocate error
 *           NTGR_FLASH_READ_FAILED   :   read error in flash
 *           NTGR_NO_ERROR            :   no error
 *
 *****************************************************************************/
NTGR_error_code_t NTGR_CheckIfFirstSSW()
{
    NTGR_ssw_info_t sSwInfo;
    NTGR_error_code_t    eError_code;
    mTBOX_FCT_ENTER("NTGR_CheckIfFirstSSW");

	eError_code=NTGR_UT_ReadSSWInfo(&sSwInfo);	
	
 /*eError_code = NTGR_FLASH_CRC_FAILED; @@@ */	
	
	if (eError_code == NTGR_FLASH_CRC_FAILED)
	{
	    /*printf("**********************FIRST Software In Flash\n");*/
	    eError_code = NTGR_ComputeAndWriteSSWInfo();
	    if (eError_code!= NTGR_NO_ERROR)
	    {
    			mTBOX_TRACE((kTBOX_NIV_WARNING, "NTGR_CheckIfFirstSSW NTGR_ComputeAndWriteSSWInfo FAILED \n"));
               mTBOX_RETURN(eError_code);
	    }
    	mTBOX_TRACE((kTBOX_NIV_WARNING, "NTGR_CheckIfFirstSSW NTGR_ComputeAndWriteSSWInfo succeedded \n"));
    }
    mTBOX_TRACE((kTBOX_NIV_WARNING, "NTGR_CheckIfFirstSSW succeedded \n"));
    mTBOX_RETURN(NTGR_NO_ERROR);
}


/******************************************************************************
 * Function Name : NTGR_UT_ReadSSWInfo
 *
 * Description   : Reads the crc 32 of each module
 *
 * Side effects  :
 *
 * Returns:
 *           NTGR_FLASH_CRC_FAILED    :   a module is corrupted
 *           NTGR_MEMORY_ALLOC_FAILED :   memory allocate error
 *           NTGR_FLASH_READ_FAILED   :   read error in flash
 *           NTGR_NO_ERROR            :   no error
 *
 *****************************************************************************/
NTGR_error_code_t NTGR_UT_ReadSSWInfo(NTGR_ssw_info_t *psSwInfo)
{
    NTGR_error_code_t eBtlError=NTGR_NO_ERROR;
    uint32_t    uReadByte;
    uint32_t    uCRC32Calculated;

    mTBOX_FCT_ENTER("NTGR_UT_ReadSSWInfo");

    /*
    ** check parameter
    */
    if (psSwInfo==NULL)
    {
        eBtlError=NTGR_FAILED;
    }

    /*
    ** we read the parameters from Non Volatile Memory
    */
    if (eBtlError==NTGR_NO_ERROR)
    {
        uReadByte = TDAL_NVM_Read(kNTGR_SSW_SEGMENT_BASE, kNTGR_SSW_AREA_SIZE, (uint8_t*) &gsSwInfoInNVM);
        if ( uReadByte != kNTGR_SSW_AREA_SIZE )
        {
        	mTBOX_TRACE((kTBOX_NIV_CRITICAL,"NTGR_UT -> TDAL_NVM_Write error\n"));
              eBtlError=eNVM_ERROR;
        }
    }
    /*
    ** check encapsulation CRC32
    */
    if (eBtlError==NTGR_NO_ERROR)
    {
        BTL_CRC_crc32Calculate( (uint8_t*) &gsSwInfoInNVM, kNTGR_SSW_PAYLOAD_SIZE,  &uCRC32Calculated );
        if ( uCRC32Calculated != gsSwInfoInNVM.CRC32)
        {
        	mTBOX_TRACE((kTBOX_NIV_CRITICAL, "\rCRC32 failed for SWW info structure\r\n"));
            eBtlError=NTGR_FLASH_CRC_FAILED;
    		mTBOX_RETURN(eBtlError);
        }
    }
    mTBOX_TRACE((kTBOX_NIV_WARNING, "NTGR_UT_ReadSSWInfo return eBtlError=0x%x\r\n",eBtlError));
    mTBOX_RETURN(eBtlError);
}


/******************************************************************************
 * Function Name : NTGR_CheckSSWIntegrity
 *
 * Description   : It checks the crc 32 of each module
 *
 * Side effects  :
 *
 * Returns:
 *           NTGR_FLASH_CRC_FAILED    :   a module is corrupted
 *           NTGR_MEMORY_ALLOC_FAILED :   memory allocate error
 *           NTGR_FLASH_READ_FAILED   :   read error in flash
 *           NTGR_NO_ERROR            :   no error
 *
 *****************************************************************************/
NTGR_error_code_t NTGR_CheckSSWIntegrity(bool *pbSswAvailable)
{
    NTGR_error_code_t   eError_code;
    uint32_t            ucCptModule;
    uint32_t           	CRC32Calculated;

    mTBOX_FCT_ENTER("NTGR_CheckSSWIntegrity");

    if (pbSswAvailable!=NULL)
    {
    	*pbSswAvailable = false;
    }
    eError_code =  NTGR_UT_ReadSSWInfo(&gsSwInfoInNVM);
    if (eError_code != NTGR_NO_ERROR )
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL,"NTGR_UT_ReadSSWInfo has failed\n"));
        mTBOX_RETURN(eError_code);
    }
/*printf("nbOfModules =0x%x\n",gsSwInfoInNVM.nbOfModules);*/

    /* loop for all modules */
    for (ucCptModule = 0; ucCptModule < gsSwInfoInNVM.nbOfModules; ucCptModule++)
    {
	    mTBOX_TRACE((kTBOX_NIV_WARNING,"NTGR_CheckSSWIntegrity considers module %d\n",ucCptModule));
        /* reading of the module in flash */
        if (TDAL_FLA_Read(gsSwInfoInNVM.moduleInfoArray[ucCptModule].startAddress,
                          gModuleBuffer,
                          gsSwInfoInNVM.moduleInfoArray[ucCptModule].moduleSize) != gsSwInfoInNVM.moduleInfoArray[ucCptModule].moduleSize)
        {
  	    	mTBOX_TRACE((kTBOX_NIV_CRITICAL,"NTGR_CheckSSWIntegrity block %d unreadable\n",ucCptModule));
            mTBOX_RETURN(NTGR_FLASH_READ_FAILED);
        }
/*
printf("startAddress =0x%x\n",	gsSwInfoInNVM.moduleInfoArray[ucCptModule].startAddress);
printf("moduleSize= 0x%x\n",	gsSwInfoInNVM.moduleInfoArray[ucCptModule].moduleSize);
printf("CRC32= 0x%x\n",			gsSwInfoInNVM.moduleInfoArray[ucCptModule].CRC32);
printf("buffer %x %x %x %x \n",gModuleBuffer[0],gModuleBuffer[1],gModuleBuffer[2],gModuleBuffer[3]);
*/
        /* checking the crc of the module */
        BTL_CRC_crc32Calculate(gModuleBuffer,
                                gsSwInfoInNVM.moduleInfoArray[ucCptModule].moduleSize,
                                &CRC32Calculated);
        if (CRC32Calculated != gsSwInfoInNVM.moduleInfoArray[ucCptModule].CRC32)
        {
/*
            mTBOX_TRACE(("NTGR_CheckSSWIntegrity block %d size %d [%x %x %x] corrupted C=%#x R=%#x\n",
                  ucCptModule,
                  gsSwInfoInNVM.moduleInfoArray[ucCptModule].moduleSize,
                  gModuleBuffer[0],gModuleBuffer[1],gModuleBuffer[2],
                  CRC32Calculated, 
                  gsSwInfoInNVM.moduleInfoArray[ucCptModule].CRC32));
*/
            mTBOX_TRACE((kTBOX_NIV_2, "\r------------- Check CRC32 of Module %d BAD -------------\r\n", ucCptModule+1));

            mTBOX_RETURN(NTGR_FLASH_CRC_FAILED);
        }
        else
        {
            mTBOX_TRACE((kTBOX_NIV_2, "\r------------- Check CRC32 of Module %d OK -------------\r\n", ucCptModule+1));

        }
    } /* end of for */
    /* update bSswAvailable: all modules have a correct CRC */
    if (pbSswAvailable!=NULL)
    {
    	*pbSswAvailable = true;
    }
    mTBOX_RETURN(NTGR_NO_ERROR);
}


