/******************************************************************************
 *                         COPYRIGHT 2005 IWEDIA TECHNOLOGIES                         *
 ******************************************************************************
 *
 * MODULE NAME: SYS
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/product/comedia/branches/idecode_hd/build/src/sys_ui_mngr_fp_mapping_cfg.c $
 *            $Rev: 244 $
 *            $Date: 2011-08-18 18:33:39 +0200 (Thu, 18 Aug 2011) $
 *
 * DESCRIPTION
 *
 *****************************************************************************/

/********************************************************/
/*              Includes                                */
/********************************************************/
#include "sys_ui_mngr_fp_mapping_cfg.h"

#define eFP_INVALID_KEY        0xFF

#ifdef PRODUCT_NL_CAB_5197G

#define eFP_NO_KEY             0x00
#define eFP_POWER_KEY          0x40
#define eFP_SELECT_KEY         0x14
#define eFP_MENU_KEY           0x01
#define eFP_RIGHT_ARROW_KEY    0x04
#define eFP_LEFT_ARROW_KEY     0x12
#define eFP_UP_ARROW_KEY       0x11
#define eFP_DOWN_ARROW_KEY     0x02
#define eFP_EXIT_KEY           eFP_INVALID_KEY
#define eFP_PROGRAM_NEXT_KEY   eFP_INVALID_KEY
#define eFP_PROGRAM_PREV_KEY   eFP_INVALID_KEY
#define eFP_VOLUME_NEXT_KEY    eFP_INVALID_KEY
#define eFP_VOLUME_PREV_KEY    eFP_INVALID_KEY
#define eFP_INFO_KEY           eFP_INVALID_KEY
#define eFP_FACTORY_DEFAULT_KEY 0x80
#define eFP_CUSTOM_1_KEY       eFP_INVALID_KEY

#elif defined PRODUCT_NL_CAB_5197D || defined(PRODUCT_NL_CAB_DEN5197)

#define eFP_NO_KEY             0x00
#define eFP_POWER_KEY          eFP_INVALID_KEY
#define eFP_SELECT_KEY         eFP_INVALID_KEY
#define eFP_MENU_KEY           0x40
#define eFP_RIGHT_ARROW_KEY    eFP_INVALID_KEY
#define eFP_LEFT_ARROW_KEY     eFP_INVALID_KEY
#define eFP_UP_ARROW_KEY       eFP_INVALID_KEY
#define eFP_DOWN_ARROW_KEY     eFP_INVALID_KEY
#define eFP_EXIT_KEY           eFP_INVALID_KEY
#define eFP_PROGRAM_NEXT_KEY   0x11
#define eFP_PROGRAM_PREV_KEY   0x02
#define eFP_VOLUME_NEXT_KEY    0x04
#define eFP_VOLUME_PREV_KEY    0x12
#define eFP_INFO_KEY           eFP_INVALID_KEY
#define eFP_FACTORY_DEFAULT_KEY 0x80
#define eFP_CUSTOM_1_KEY       eFP_INVALID_KEY

#elif defined PRODUCT_CH_CAB_8080CL || defined(PRODUCT_CH_CAB_DEN8080CL)

#define eFP_NO_KEY             0x00
#define eFP_POWER_KEY          eFP_INVALID_KEY
#define eFP_SELECT_KEY         eFP_INVALID_KEY
#define eFP_MENU_KEY           0x41
#define eFP_RIGHT_ARROW_KEY    eFP_INVALID_KEY
#define eFP_LEFT_ARROW_KEY     eFP_INVALID_KEY
#define eFP_UP_ARROW_KEY       eFP_INVALID_KEY
#define eFP_DOWN_ARROW_KEY     eFP_INVALID_KEY
#define eFP_EXIT_KEY           eFP_INVALID_KEY
#define eFP_PROGRAM_NEXT_KEY   0x40
#define eFP_PROGRAM_PREV_KEY   0x80
#define eFP_VOLUME_NEXT_KEY    0x09
#define eFP_VOLUME_PREV_KEY    0x11
#define eFP_INFO_KEY           eFP_INVALID_KEY
#define eFP_FACTORY_DEFAULT_KEY 0xd9
#define eFP_CUSTOM_1_KEY       eFP_INVALID_KEY

#elif defined(PRODUCT_DEN_CAB_5197)
#define eFP_NO_KEY             eFP_INVALID_KEY
#define eFP_POWER_KEY          0x04
#define eFP_SELECT_KEY         eFP_INVALID_KEY
#define eFP_MENU_KEY           eFP_INVALID_KEY
#define eFP_RIGHT_ARROW_KEY    eFP_INVALID_KEY
#define eFP_LEFT_ARROW_KEY     eFP_INVALID_KEY
#define eFP_UP_ARROW_KEY       eFP_INVALID_KEY
#define eFP_DOWN_ARROW_KEY     eFP_INVALID_KEY
#define eFP_EXIT_KEY           eFP_INVALID_KEY
#define eFP_PROGRAM_NEXT_KEY   0x01
#define eFP_PROGRAM_PREV_KEY   0x02
#define eFP_VOLUME_NEXT_KEY    eFP_INVALID_KEY
#define eFP_VOLUME_PREV_KEY    eFP_INVALID_KEY
#define eFP_INFO_KEY           eFP_INVALID_KEY
#define eFP_FACTORY_DEFAULT_KEY 0x03
#define eFP_CUSTOM_1_KEY       eFP_INVALID_KEY

#else
#define eFP_NO_KEY             0x00
#define eFP_POWER_KEY          0xDC
#define eFP_SELECT_KEY         eFP_INVALID_KEY
#define eFP_MENU_KEY           eFP_INVALID_KEY
#define eFP_RIGHT_ARROW_KEY    eFP_INVALID_KEY
#define eFP_LEFT_ARROW_KEY     eFP_INVALID_KEY
#define eFP_UP_ARROW_KEY       0x01
#define eFP_DOWN_ARROW_KEY     0x02
#define eFP_EXIT_KEY           eFP_INVALID_KEY
#define eFP_PROGRAM_NEXT_KEY   0xCA//eFP_INVALID_KEY
#define eFP_PROGRAM_PREV_KEY   0xD2//eFP_INVALID_KEY
#define eFP_VOLUME_NEXT_KEY    eFP_INVALID_KEY
#define eFP_VOLUME_PREV_KEY    eFP_INVALID_KEY
#define eFP_INFO_KEY           eFP_INVALID_KEY
#define eFP_FACTORY_DEFAULT_KEY 0xA5
#define eFP_CUSTOM_1_KEY       eFP_INVALID_KEY

#endif


#define NB_FP_KEY 15




typedef struct
{
    tSYS_FP_KeyCode fpCode;   /* type ->  */
    tSYS_KeyEvtCode sysCode;   /* type -> tSYS_KeyEvtCode */
} tSYS_KeyEvtCodeMapping;

LOCAL tSYS_KeyEvtCodeMapping SYS_KeyEvtCodeMapping[NB_FP_KEY] =
{
    {
        eFP_POWER_KEY,
        eSYS_POWER_KEY
    },
    {
        eFP_SELECT_KEY,
        eSYS_SELECT_KEY
    },
    {
        eFP_EXIT_KEY,
        eSYS_EXIT_KEY
    },
    {
        eFP_MENU_KEY,
        eSYS_MENU_KEY
    },
    {
        eFP_RIGHT_ARROW_KEY,
        eSYS_RIGHT_ARROW_KEY
    },
    {
        eFP_LEFT_ARROW_KEY,
        eSYS_LEFT_ARROW_KEY
    },
    {
        eFP_UP_ARROW_KEY,
        eSYS_UP_ARROW_KEY
    },
    {
        eFP_DOWN_ARROW_KEY,
        eSYS_DOWN_ARROW_KEY
    },
    {
        eFP_PROGRAM_PREV_KEY,
        eSYS_PROGRAM_PREV_KEY
    },
    {
        eFP_PROGRAM_NEXT_KEY,
        eSYS_PROGRAM_NEXT_KEY
    },

	{
		eFP_VOLUME_NEXT_KEY,
		eSYS_VOLUME_NEXT_KEY
	},

	{
		eFP_VOLUME_PREV_KEY,
		eSYS_VOLUME_PREV_KEY
	},
	{
		eFP_INFO_KEY,
		eSYS_INFO_KEY
	},
	{
        eFP_FACTORY_DEFAULT_KEY,
        eSYS_FACTORY_DEFAULT_KEY
	},
	{
		eFP_CUSTOM_1_KEY,
		eSYS_OTHERAPP_KEY
	},

};
/********************************************************/
/*              Functions Definitions (LOCAL/GLOBAL)    */
/********************************************************/
/*===========================================================================
 * SYS_FP_ConvertPhysicalCodeToSYSCode
 *
 *
 * Parameters:
 *
 * Description:
 *      return
 *
 * Returns:
 *
 *
 *===========================================================================*/
tSYS_KeyEvtCode SYS_FP_ConvertPhysicalCodeToSYSCode(tSYS_FP_KeyCode KeyCode)
{
	uint8_t index;

	if(KeyCode == eFP_NO_KEY)
	{
		return(eSYS_KEY_EVT_LAST);
	}

	for (index=0;index <NB_FP_KEY;index++)
	{
		if (SYS_KeyEvtCodeMapping[index].fpCode == KeyCode)
		{
			return(SYS_KeyEvtCodeMapping[index].sysCode);
		}
	}

	return(eSYS_KEY_EVT_LAST);
}
