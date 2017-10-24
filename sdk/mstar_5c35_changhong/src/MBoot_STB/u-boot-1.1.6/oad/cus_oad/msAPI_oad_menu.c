
#include <common.h>
#include <command.h>
#include <malloc.h>
#include <config.h>
#include <configs/uboot_board_config.h>
#include <sys/common/MsTypes.h>
#include <sys/common/MsIRQ.h>
#include <sys/common/MsOS.h>
#include <IR_MSTAR_DTV.h>

#define GWIN_WIDTH          720
#define GWIN_HEIGHT         576

typedef enum
{
	EM_COLOR_TYPE_WITHE,
	EM_COLOR_TYPE_BLACK,
	EM_COLOR_TYPE_RED,
	EM_COLOR_TYPE_GREEN,
	EM_COLOR_TYPE_BLUE,
	EM_COLOR_TYPE_MAX
}EM_COLOUR_TYPE;



typedef struct 
{
	unsigned long bgColor;
	unsigned long fontColor;
	unsigned int  nXsrc;
	unsigned int  nYsrc;
	unsigned int  nXmax;
	unsigned int  nYmax;
	unsigned int alignType;
	char         *pStrPromt;
}MENU_ITEM_UI;

typedef struct
{
	MENU_ITEM_UI *pMenuHeader;
	MENU_ITEM_UI *pMenuItems;
}MENU_UI;

#define X_BEGIN_POS           160
#define Y_BEGIN_POS           88
#define MENU_ITEM_WEIGHT      400
#define MENU_ITEM_HIGHT       60
#define FOCUS_MENU_COLOR       0x00556B2F
#define MENU_BACKGROUND_COLOR  0x006495ED
#define MENU_FONT_COLOR        0x004682b4
#define BACKGROUND_COLOR_CONST 0x004682b4

enum
{
	EM_MENU_IDX_SET_FREQ = 0,
	EM_MENU_IDX_SET_SYMBOLRATE,
	EM_MENU_IDX_SET_OTAPID,
	EM_MENU_IDX_SET_MODULATION,
	EM_MENU_IDX_OK,
	EM_MENU_IDX_NULL,
};

MENU_ITEM_UI menuHeader = 
{
	.nXsrc = X_BEGIN_POS,
	.nYsrc = Y_BEGIN_POS,
	.nXmax = MENU_ITEM_WEIGHT,
	.nYmax = MENU_ITEM_HIGHT,
	.alignType = 0,
	.bgColor   = MENU_BACKGROUND_COLOR,
	.fontColor = MENU_FONT_COLOR,
	.pStrPromt = "OAD CHANNEL SETTING",
};

MENU_ITEM_UI menuItems[EM_MENU_IDX_NULL] = 
{
	[EM_MENU_IDX_SET_FREQ] = 
	{
	.nXsrc = X_BEGIN_POS,
	.nYsrc = Y_BEGIN_POS+MENU_ITEM_HIGHT,
	.nXmax = MENU_ITEM_WEIGHT,
	.nYmax = MENU_ITEM_HIGHT,
	.alignType = 0,
	.bgColor   = MENU_BACKGROUND_COLOR,
	.fontColor = MENU_FONT_COLOR,
	.pStrPromt = "FREQUENCY ",
	},
	[EM_MENU_IDX_SET_SYMBOLRATE] = 
	{
	.nXsrc = X_BEGIN_POS,
	.nYsrc = Y_BEGIN_POS+2*MENU_ITEM_HIGHT,
	.nXmax = MENU_ITEM_WEIGHT,
	.nYmax = MENU_ITEM_HIGHT,
	.alignType = 0,
	.bgColor   = MENU_BACKGROUND_COLOR,
	.fontColor = MENU_FONT_COLOR,
	.pStrPromt = "SYMBOLRATE",
	},

	[EM_MENU_IDX_SET_OTAPID] = 
	{
	.nXsrc = X_BEGIN_POS,
	.nYsrc = Y_BEGIN_POS+3*MENU_ITEM_HIGHT,
	.nXmax = MENU_ITEM_WEIGHT,
	.nYmax = MENU_ITEM_HIGHT,
	.alignType = 0,
	.bgColor   = MENU_BACKGROUND_COLOR,
	.fontColor = MENU_FONT_COLOR,
	.pStrPromt = "OTAPID",
	},

	[EM_MENU_IDX_SET_MODULATION] = 
	{
	.nXsrc = X_BEGIN_POS,
	.nYsrc = Y_BEGIN_POS+4*MENU_ITEM_HIGHT,
	.nXmax = MENU_ITEM_WEIGHT,
	.nYmax = MENU_ITEM_HIGHT,
	.alignType = 0,
	.bgColor   = MENU_BACKGROUND_COLOR,
	.fontColor = MENU_FONT_COLOR,
	.pStrPromt = "MODULATION(QAM)",
	},
	[EM_MENU_IDX_OK] = 
	{
	.nXsrc = X_BEGIN_POS,
	.nYsrc = Y_BEGIN_POS+5*MENU_ITEM_HIGHT,
	.nXmax = MENU_ITEM_WEIGHT,
	.nYmax = MENU_ITEM_HIGHT,
	.alignType = 1,
	.bgColor   = MENU_BACKGROUND_COLOR,             
	.fontColor = MENU_FONT_COLOR,
	.pStrPromt = "OK                      EXIT",
	},
};

MENU_UI mainMenuUI = 
{
	.pMenuHeader = &menuHeader,
	.pMenuItems = menuItems,
};

typedef struct 
{
	char          input[8];
	unsigned char needClear;
	unsigned char inputlen;
	unsigned char maxLen;
}MENU_UI_DATA;

MENU_UI_DATA g_sMenuItemData[EM_MENU_IDX_NULL-1] = 
{
	[EM_MENU_IDX_SET_FREQ] = 
	{
		{'4', '9', '0', '0','0','0',0x00,},
		.needClear = 1,
		.inputlen = 6,
		.maxLen = 6,
	},

	[EM_MENU_IDX_SET_SYMBOLRATE] = 
	{
		{'6', '8', '7', '5', 0x00,},
		.needClear = 1,
		.inputlen = 0,
		.maxLen = 4,
	},

	[EM_MENU_IDX_SET_OTAPID] = 
	{
		{'3', '3', '6', 0x00,},
		.needClear = 1,
		.inputlen = 3,
		.maxLen = 6,
	},

	[EM_MENU_IDX_SET_MODULATION] = 
	{
		{'6', '4', 0x00},
		.needClear = 1,
		.inputlen = 2,
		.maxLen = 6,
	},
	
};

typedef struct 
{
	unsigned char (*getKeyCode)(void);
	unsigned char (*drawRect)(unsigned int xSrc, unsigned int ySrc, unsigned int rectWeigt, unsigned int rectHight, unsigned long cr);
	unsigned char (*TextOut)(unsigned int xPos, unsigned int yPos, char* pstring, unsigned int alignType, unsigned long cr);
	unsigned char (*clearWindow)(unsigned int xPos, unsigned int yPos, unsigned int areaWeight,unsigned int areaHight, unsigned long crFlush);
	unsigned int (*getPanelXmax)(void);
	unsigned int (*getPanelYmax)(void);
}UI_BASE_OPS;


typedef struct __MENU
{
	unsigned long choice;
	unsigned long menuMaxIndex;
	unsigned char (*OnOK)(void);
	unsigned char (*OnExit)(void);
	unsigned char (*init)(void);
	unsigned char (*CheckTimeOut)(void);
	unsigned char (*keyProc)(unsigned char keyCode);
	struct __MENU* (*getParent)(void);
	struct __MENU* (*getSubMenu)(void);
}MENU_CLASS;

UI_BASE_OPS g_UIFUNS;
UI_BASE_OPS *g_pUIFuns = &g_UIFUNS;
static MENU_CLASS g_sMainMenu;
static MENU_CLASS *g_pCurMenu = &g_sMainMenu;

#define OTA_ENTRY_DEBUG() printf("[%s], ENTRY!\n", __FUNCTION__);
#define OTA_EXIT_DEBUG()  printf("[%s], EXIT!\n", __FUNCTION__);
#define OTA_INFO(x)       x

extern BOOLEAN msIR_GetIRKeyCode(U8 *pkey, U8 *pu8flag);
static unsigned char _getKeyCode(void)
{
	unsigned char keyCode = 0xFF;
	unsigned char isRepeat;
	
	if(msIR_GetIRKeyCode(&keyCode, &isRepeat))
		return keyCode;
		
	return 0xFF;
}

static unsigned char _drawRect(unsigned int xSrc, unsigned int ySrc, unsigned int rectWeigt, unsigned int rectHight, unsigned long cr)
{
	char buffer[256];
	
	OTA_ENTRY_DEBUG();
	
	memset(buffer, 0x00, sizeof(buffer));
	sprintf(buffer, "draw_rect %d %d %d %d 0x%08X", xSrc, ySrc, rectWeigt, rectHight, cr);

	return run_command(buffer, 0);
}

static unsigned char _TextOut(unsigned int xPos, unsigned int yPos, char* pString, unsigned int alignType, unsigned long cr)
{
	char buffer[256];
	
	OTA_ENTRY_DEBUG();
	
	memset(buffer, 0x00, sizeof(buffer));
	sprintf(buffer, "draw_string %d %d %x %d %s", xPos, yPos, cr, alignType, pString);

	return run_command(buffer, 0);
	
}

static unsigned char _clearWindow(unsigned int xPos, unsigned int yPos, unsigned int areaWeight,unsigned int areaHight, unsigned long crFlush)
{
	char buffer[1024];
	OTA_ENTRY_DEBUG();
	memset(buffer, 0x00, sizeof(buffer));
	sprintf(buffer, "draw_rect %d %d %d %d %x", xPos, yPos, areaWeight, areaHight, crFlush);

	return run_command(buffer, 0);
	
}

static unsigned int _getPanelXmax(void)
{
	return GWIN_WIDTH;
}

static unsigned int _getPanelYmax(void)
{
	return GWIN_HEIGHT;
}

MS_U8 MApi_LDR_OADProc(void);

//!######################################(MAINMENU LAYER IMPLEMENT)####################################
static unsigned char mainMenu_OnOK(void)
{
	MS_U32 oadFreq = 0;
	MS_U32 oadSymRate = 0;
	MS_U8  oadQAM = 0;
	MS_U16 oadPID = 0;
	char   cmdBuf[256];
	
	OTA_ENTRY_DEBUG();
	
	if (g_pCurMenu->choice == EM_MENU_IDX_OK)
	{
		oadFreq = simple_strtoul(g_sMenuItemData[EM_MENU_IDX_SET_FREQ].input, NULL, 10);
		oadSymRate = simple_strtoul(g_sMenuItemData[EM_MENU_IDX_SET_SYMBOLRATE].input, NULL, 10);
		oadPID = simple_strtoul(g_sMenuItemData[EM_MENU_IDX_SET_OTAPID].input, NULL, 10);
		oadQAM = simple_strtoul(g_sMenuItemData[EM_MENU_IDX_SET_MODULATION].input, NULL, 10);
		
		setenv("oad_upgrade", "1");
		memset(cmdBuf, 0, sizeof(cmdBuf));
		sprintf(cmdBuf,"%d", oadFreq);
		setenv("oad_upgrade_freq", cmdBuf);

		memset(cmdBuf, 0, sizeof(cmdBuf));
		sprintf(cmdBuf,"%d", oadSymRate);
		setenv("oad_upgrade_sym", cmdBuf);

		memset(cmdBuf, 0, sizeof(cmdBuf));
		if ((oadQAM%32) != 0)
		{
			oadQAM = 2;
		}
		else
			oadQAM /= 32;
			
		sprintf(cmdBuf,"%d", oadQAM);
		setenv("oad_upgrade_qam", cmdBuf);

		memset(cmdBuf, 0, sizeof(cmdBuf));
		sprintf(cmdBuf,"%d", oadPID);
		setenv("oad_upgrade_pid", cmdBuf);
		
		printf("(freq, symrate, pid, qam)(%d,%d,%d,%d)\n", oadFreq, oadSymRate, oadPID, oadQAM);
                   
                  MApi_LDR_OADProc();
		//run_command("ostar", 0);
	}
	
	OTA_EXIT_DEBUG();
	return TRUE;
}

static unsigned char mainMenu_OnExit(void)
{
	OTA_ENTRY_DEBUG();
	if(!run_command("reset", 0))
	{
		OTA_EXIT_DEBUG();
		return TRUE;
	}

	else
	{
		OTA_EXIT_DEBUG();
		return FALSE;
	}
}

static void _updateMenuItemText(unsigned char menuIdx, char *Text, unsigned long crText)
{
	MENU_ITEM_UI *pUI;
	OTA_ENTRY_DEBUG();
	if (menuIdx < 0 || menuIdx > EM_MENU_IDX_NULL-1)
		return;
	pUI = &mainMenuUI.pMenuItems[menuIdx];

	g_pUIFuns->TextOut(pUI->nXsrc+pUI->nXmax-10, pUI->nYsrc+15, Text, 2, crText);
}

static void _updateMenuItemDisp(unsigned char menuIdx, unsigned long bgColor)
{
	MENU_ITEM_UI *pUI;
	
	OTA_ENTRY_DEBUG();
	if (menuIdx < 0 || menuIdx > EM_MENU_IDX_NULL-1)
		return;
	
	pUI = &mainMenuUI.pMenuItems[menuIdx];
	pUI->bgColor = bgColor;
	
	if (menuIdx == EM_MENU_IDX_OK)
	{
		g_pUIFuns->drawRect(pUI->nXsrc, pUI->nYsrc+5, pUI->nXmax, pUI->nYmax, 0x004b0082);
		g_pUIFuns->TextOut(pUI->nXsrc+80, pUI->nYsrc+15, "OK", 0, pUI->bgColor);
		g_pUIFuns->TextOut(pUI->nXsrc+pUI->nXmax-80, pUI->nYsrc+15, "EXIT", 2, pUI->fontColor);
		g_pUIFuns->drawRect(pUI->nXsrc, pUI->nYsrc+pUI->nYmax, pUI->nXmax, 5, 0x00696969);
		return;
	}
	
	g_pUIFuns->drawRect(pUI->nXsrc, pUI->nYsrc+5, pUI->nXmax, pUI->nYmax, pUI->bgColor);
	g_pUIFuns->TextOut(pUI->nXsrc+10, pUI->nYsrc+15, pUI->pStrPromt, pUI->alignType, pUI->fontColor);

	
	//pUI = &mainMenuUI.pMenuItems[menuIdx];
	g_pUIFuns->clearWindow(pUI->nXsrc+250, pUI->nYsrc+5, pUI->nXmax-250, pUI->nYmax, pUI->bgColor);
	g_pUIFuns->TextOut(pUI->nXsrc+pUI->nXmax-10, pUI->nYsrc+15, g_sMenuItemData[menuIdx].input, 2, 0x00D0D0D0);
	g_pUIFuns->drawRect(pUI->nXsrc, pUI->nYsrc+pUI->nYmax, pUI->nXmax, 5, 0x00696969);
}

static unsigned char mainMenu_init(void)
{
	MENU_ITEM_UI *pUI;
	unsigned char i;
	
	OTA_ENTRY_DEBUG();
	run_command("osd_create 720 576", 0);
	g_pUIFuns->drawRect(0, 0, GWIN_WIDTH, GWIN_HEIGHT, BACKGROUND_COLOR_CONST);

	pUI = mainMenuUI.pMenuHeader;
	g_pUIFuns->drawRect(pUI->nXsrc, pUI->nYsrc-5, pUI->nXmax, 5, 0x00696969);
	g_pUIFuns->drawRect(pUI->nXsrc, pUI->nYsrc, pUI->nXmax, pUI->nYmax, 0x004b0082);
	g_pUIFuns->TextOut(pUI->nXsrc+10, pUI->nYsrc+10, pUI->pStrPromt, pUI->alignType, pUI->fontColor);
	g_pUIFuns->drawRect(pUI->nXsrc, pUI->nYsrc+pUI->nYmax, pUI->nXmax, 5, 0x00696969);
	
	for (i = 0; i < EM_MENU_IDX_NULL-1; i++)
	{
		pUI = &mainMenuUI.pMenuItems[i];
		if (i == 0)
			pUI->bgColor = FOCUS_MENU_COLOR;
		g_pUIFuns->drawRect(pUI->nXsrc, pUI->nYsrc+5, pUI->nXmax, pUI->nYmax, pUI->bgColor);
		g_pUIFuns->TextOut(pUI->nXsrc+10, pUI->nYsrc+15, pUI->pStrPromt, pUI->alignType, pUI->fontColor);	
		g_pUIFuns->drawRect(pUI->nXsrc, pUI->nYsrc+pUI->nYmax, pUI->nXmax, 5, 0x00696969);
	}
	
	pUI = &mainMenuUI.pMenuItems[i];
	g_pUIFuns->drawRect(pUI->nXsrc, pUI->nYsrc+5, pUI->nXmax, pUI->nYmax, 0x004b0082);
	g_pUIFuns->TextOut(pUI->nXsrc+80, pUI->nYsrc+15, "OK", 0, pUI->fontColor);
	g_pUIFuns->TextOut(pUI->nXsrc+pUI->nXmax-80, pUI->nYsrc+15, "EXIT", 2, pUI->fontColor);
	g_pUIFuns->drawRect(pUI->nXsrc, pUI->nYsrc+pUI->nYmax, pUI->nXmax, 5, 0x00696969);

	_updateMenuItemText(0, "490000", 0x00D0D0D0);
	_updateMenuItemText(1, "6875", 0x00D0D0D0);
	_updateMenuItemText(2, "336", 0x00D0D0D0);
	_updateMenuItemText(3, "64",   0x00D0D0D0);
	OTA_EXIT_DEBUG();
	return TRUE;
}

static unsigned char  mainMenu_CheckTimeOut(void)
{
	OTA_ENTRY_DEBUG();
	OTA_EXIT_DEBUG();
	return TRUE;
}

static unsigned char _handleInputKey(char keyCode)
{
	if (g_pCurMenu->choice == g_pCurMenu->menuMaxIndex)
		return TRUE;
		
	if (g_sMenuItemData[g_pCurMenu->choice].needClear)
	{
		g_sMenuItemData[g_pCurMenu->choice].needClear = 0;
		memset(g_sMenuItemData[g_pCurMenu->choice].input, 0, 8);
		g_sMenuItemData[g_pCurMenu->choice].inputlen = 0;
	}
	
	if (g_sMenuItemData[g_pCurMenu->choice].inputlen++ < g_sMenuItemData[g_pCurMenu->choice].maxLen)
		g_sMenuItemData[g_pCurMenu->choice].input[g_sMenuItemData[g_pCurMenu->choice].inputlen-1]=keyCode;


	else
	{
		memcpy(g_sMenuItemData[g_pCurMenu->choice].input, g_sMenuItemData[g_pCurMenu->choice].input+1, \
		       g_sMenuItemData[g_pCurMenu->choice].maxLen-1);
		g_sMenuItemData[g_pCurMenu->choice].input[g_sMenuItemData[g_pCurMenu->choice].maxLen-1] = keyCode;
	}
	
	_updateMenuItemDisp(g_pCurMenu->choice, mainMenuUI.pMenuItems[g_pCurMenu->choice].bgColor);

	return TRUE;
}

static unsigned char mainMenu_keyProc(unsigned char keyCode)
{
	switch (keyCode)
	{
		case IRKEY_NUM_0:
			_handleInputKey('0');
			break;
			
		case IRKEY_NUM_1:
			_handleInputKey('1');
			break;
			
		case IRKEY_NUM_2:
			_handleInputKey('2');
			break;
			
		case IRKEY_NUM_3:
			_handleInputKey('3');
			break;
			
		case IRKEY_NUM_4:
			_handleInputKey('4');
			break;
			
		case IRKEY_NUM_5:
			_handleInputKey('5');
			break;
			
		case IRKEY_NUM_6:
			_handleInputKey('6');
			break;
			
		case IRKEY_NUM_7:
			_handleInputKey('7');
			break;
			
		case IRKEY_NUM_8:
			_handleInputKey('8');
			break;
			
		case IRKEY_NUM_9:
			_handleInputKey('9');
			break;

		case IRKEY_EXIT:
		{
			g_pCurMenu->OnExit();
		}
		break;

		case IRKEY_SELECT:
		{
			g_pCurMenu->OnOK();
		}
		break;

		case IRKEY_UP:
		{
			if (g_pCurMenu->choice > 0)
			{
				if (g_pCurMenu->choice < g_pCurMenu->menuMaxIndex)
					g_sMenuItemData[g_pCurMenu->choice].needClear = 1;
				_updateMenuItemDisp(g_pCurMenu->choice, MENU_BACKGROUND_COLOR);
				g_pCurMenu->choice--;
				_updateMenuItemDisp(g_pCurMenu->choice, FOCUS_MENU_COLOR);
			}
		}	
	
		break;
		
		case IRKEY_DOWN:
		{
			if (g_pCurMenu->choice < g_pCurMenu->menuMaxIndex)
			{
				g_sMenuItemData[g_pCurMenu->choice].needClear = 1;
				_updateMenuItemDisp(g_pCurMenu->choice, MENU_BACKGROUND_COLOR);
				g_pCurMenu->choice++;
				_updateMenuItemDisp(g_pCurMenu->choice, FOCUS_MENU_COLOR);
			}
		}
	
		default:
			break;
		
		
	}

	return TRUE;
}
static MENU_CLASS* mainMenu_GetParent(void)
{
	OTA_ENTRY_DEBUG();
	OTA_EXIT_DEBUG();
	return (MENU_CLASS*)NULL;
}

static MENU_CLASS* mainMenu_GetSubMenu(void)
{
	OTA_ENTRY_DEBUG();
	OTA_EXIT_DEBUG();
	return (MENU_CLASS*)NULL;
}

//!######################################(MAINMENU LAYER IMPLEMENT END)####################################

int OADMenuEntry(void)
{
	unsigned char keyCode;
	//char *pTemp = NULL;

	g_pUIFuns->getKeyCode   = _getKeyCode;
	g_pUIFuns->clearWindow  = _clearWindow;
	g_pUIFuns->drawRect     = _drawRect;
	g_pUIFuns->TextOut      = _TextOut;
	g_pUIFuns->getPanelXmax = _getPanelXmax;
	g_pUIFuns->getPanelYmax = _getPanelYmax;
	g_sMainMenu.init        = mainMenu_init;
	g_sMainMenu.OnOK        = mainMenu_OnOK;
	g_sMainMenu.OnExit      = mainMenu_OnExit;
	g_sMainMenu.keyProc     = mainMenu_keyProc;
	g_sMainMenu.CheckTimeOut = mainMenu_CheckTimeOut;
	g_sMainMenu.getParent   = mainMenu_GetParent;
	g_sMainMenu.getSubMenu  = mainMenu_GetSubMenu;
	g_sMainMenu.choice = 0;
	g_sMainMenu.menuMaxIndex = EM_MENU_IDX_NULL-1;
	extern void msIR_Initialize(MS_U8 irclk_mhz);
	msIR_Initialize(12);
	
	g_pCurMenu->init();
	while (1)
	{
		keyCode = g_pUIFuns->getKeyCode();
		if (keyCode != 0xFF)
		{
			OTA_INFO (printf("[%s] keyCode=<%d>\n", __FUNCTION__, keyCode);)
		}
		g_pCurMenu->keyProc(keyCode);
	}
}

