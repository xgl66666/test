/**************************************************************************//**
* @file    ruxi.h
*
* @brief   Public definition of the functions for redondant unary Xor integer.
*          This structure is used to know if the system is in standby.        
*
* @author  SmarDTV Rennes
******************************************************************************
* SmarDTV Copyright 2011
******************************************************************************
* $URL: $
* $Rev: $
* $Date: $
******************************************************************************/

#ifndef _RUXI_H_
#define _RUXI_H_

/******************************************************************************
* Includes
******************************************************************************/

/******************************************************************************
* Defines
******************************************************************************/
#define RUXI_SIZE 128
typedef char tRUXI_aField[RUXI_SIZE];
/******************************************************************************
* Macros
******************************************************************************/

/******************************************************************************
* Typedefs
******************************************************************************/

/******************************************************************************
* Global variables
******************************************************************************/

/******************************************************************************
* Functions
******************************************************************************/

/*
*   @brief
*   reset overflow flag
*
*   @param[in/out] axField
*
*  @remark
*  # call before programming flash
*/
void RUXI_reset(tRUXI_aField axField);

/*
*   @brief
*   initialize axField
*
*   @param[out] axField
*
*   @param[in] xiInit
*/
void RUXI_Setup(tRUXI_aField axField, bool xiInit);

/*
*   @brief
*   codes xiNew into axField
*
*   @param[in/out] axField
*
*   @param[in] xiNew
*
*   @ret_val
*   1 as long no need to erase target
*   0 else (overflow)
*/
bool RUXI_Set(tRUXI_aField axField, bool xiNew);

/*
*   @brief
*   decodes axField
*
*   @param[in] axField
*
*   @ret_val
*   stored value (bool)
*   
*/
bool RUXI_Get(tRUXI_aField axField);

#endif
