/*******************************************************
 *      COPYRIGHT 2009 IWEDIA TECHNOLOGIES    *
 *******************************************************
 *
 * MODULE NAME: TDAL_Common_coclass
 *
 * FILE NAME: $URL:  $
 *      $Rev:  $
 *      $Date: $
 *
 * PUBLIC
 *
 * DESCRIPTION
 *
 ********************************************************/

#ifndef _TDAL_CONNECTION_MANAGER_H_
#define _TDAL_CONNECTION_MANAGER_H_

/*******************************************************/
/*       Includes              */
/*******************************************************/

/*******************************************************/
/*       Defines              */
/*******************************************************/

/*******************************************************/
/*       Macros               */
/*******************************************************/


/*******************************************************/
/*       Typedefs              */
/*******************************************************/
typedef enum
{
   eTDAL_USE_CASE_WINDOW_NONE,
   eTDAL_USE_CASE_WINDOW_SINGLE,
   eTDAL_USE_CASE_WINDOW_DUAL_PIP,
   eTDAL_USE_CASE_WINDOW_DUAL_PAP
}tTDAL_UseCaseWindow;

typedef enum
{
   eTDAL_USE_CASE_NONE,
   eTDAL_USE_CASE_DIG_TUNER,
   eTDAL_USE_CASE_ANA_TUNER,
   eTDAL_USE_CASE_ANA_HDMI,
   eTDAL_USE_CASE_ANA_YPrPb,
   eTDAL_USE_CASE_ANA_LR,
   eTDAL_USE_CASE_ANA_SCART,
   eTDAL_USE_CASE_PC,
   eTDAL_USE_CASE_MEMORY_ANY,
   eTDAL_USE_CASE_MEMORY_JPG,
   eTDAL_USE_CASE_MEMORY_MP3,
   eTDAL_USE_CASE_MEMORY_WM,
   eTDAL_USE_CASE_MEMORY_MPG4,
   eTDAL_USE_CASE_MEMORY_AVI,
   eTDAL_USE_CASE_MEMORY_MPG2_PS,
   eTDAL_USE_CASE_AUTO
}tTDAL_UseCaseInputType;

typedef struct
{
   tTDAL_UseCaseWindow   eUseCaseWindow;

   tTDAL_UseCaseInputType  eFirstVideoInput;
   tTDAL_UseCaseInputType  eSecondVideoInput;/* In case eTDAL_USE_CASE_WINDOW_DUAL*/

   tTDAL_UseCaseInputType  eAudioInput;
}tTDAL_UseCaseInfo;

/*******************************************************/
/*       Variables Declarations (IMPORT)    */
/*******************************************************/


/*******************************************************/
/*       Functions Declarations        */
/*******************************************************/

bool 
TDAL_ConnectionManager_SelectUseCase(tTDAL_UseCaseInfo *pstUseCaseInfo);

#endif
